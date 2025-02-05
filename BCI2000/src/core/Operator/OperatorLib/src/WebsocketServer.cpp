////////////////////////////////////////////////////////////////////////////////
// $Id: WebsocketServer.cpp 8244 2024-07-09 18:04:31Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A web socket server that runs inside a BCI2000
//   operator module. Functionality is identical to telnet server but allows
//   for connection from JavaScript running in a browser.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "WebsocketServer.h"

#include "BCIException.h"
#include "ScriptInterpreter.h"
#include "Streambuf.h"
#include "VersionInfo.h"
#include "StringUtils.h"
#include "BinaryData.h"

#include "../../extlib/websocketpp/websocketpp/sha1/sha1.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <regex>

#ifndef _WIN32
#include <unistd.h>
#endif // _WIN32
#include <stdbool.h>

namespace {
    template<typename T> T readBigEndianNumber(std::istream& is)
    {
        T t = 0;
        for (int i = 0; i < sizeof(T); ++i) {
            t <<= 8;
            t |= is.get();
        }
        return t;
    }

    template<typename T> void writeBigEndianNumber(std::ostream& os, T t)
    {
        for (int i = sizeof(T) - 1; i >= 0; --i)
            os.put((t >> (i * 8)) & 0xff);
    }

    std::string computeWebSocketAccept(const std::string& key)
    {
        std::string value = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
        unsigned char hash[20];
        websocketpp::sha1::calc(value.data(), value.length(), hash);
        std::ostringstream oss;
        StringUtils::WriteAsBase64(oss, std::string(reinterpret_cast<char*>(hash), sizeof(hash)));
        return oss.str();
    }

    int websocketHandshake(std::iostream& stream)
    {
        struct BadRequest {};
        std::string secWebSocketAccept;
        try {
            std::string line;
            std::getline(stream, line);
            if (line.find("GET ") != 0)
                throw BadRequest();
            std::regex keyHeaderRegex("Sec-WebSocket-Key: ?([a-zA-Z0-9+/=]+)", std::regex::ECMAScript | std::regex::icase);
            std::smatch m;
            std::string secWebSocketKey;
            while (stream && line != "\r")
            {
                std::getline(stream, line);
                if (std::regex_search(line, m, keyHeaderRegex))
                    secWebSocketKey = m[1];
            }
            if (line != "\r" || secWebSocketKey.empty())
                throw BadRequest();
            secWebSocketAccept = computeWebSocketAccept(secWebSocketKey);
        }
        catch (const BadRequest&) {
            std::string response = "HTTP/1.1 400 Bad Request\r\n\r\n";
            stream.write(response.data(), response.length()).flush();
            return -1;
        }

        std::string response =
            "HTTP/1.1 101 Switching Protocols\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Accept: " + secWebSocketAccept + "\r\n"
            "\r\n";
        stream.write(response.data(), response.length()).flush();
        if (!stream)
            return -1;

        return 0;
    }

    struct WebsocketFragment
    {
        bool fin = false, rsv1 = false, rsv2 = false, rsv3 = false;
        enum class Opcode {
            continuation = 0,
            text = 1,
            binary = 2,
            close = 8,
            ping = 9,
            pong = 10,
        } opcode = Opcode::continuation;
        bool masked = false;
        std::vector<char> payload;

        std::istream& read(std::istream&);
        std::ostream& write(std::ostream&) const;
    };

    std::istream& WebsocketFragment::read(std::istream& is)
    {
        int c = is.get();
        if (!is)
            return is;
        fin = c & 0x80;
        rsv1 = c & 0x40;
        rsv2 = c & 0x20;
        rsv3 = c & 0x10;
        opcode = Opcode(c & 0x0f);
        c = is.get();
        if (!is)
            return is;
        masked = c & 0x80;
        uint64_t payloadLength = c & 0x7f;
        if (payloadLength == 126)
            payloadLength = readBigEndianNumber<uint16_t>(is);
        else if (payloadLength == 127)
            payloadLength = readBigEndianNumber<uint64_t>(is);
        char maskingKey[4];
        if (masked)
            is.read(maskingKey, sizeof(maskingKey));
        if (!is)
            return is;
        payload.resize(payloadLength);
        is.read(payload.data(), payload.size());
        if (!is)
            return is;
        if (masked) {
            for (size_t i = 0; i < payload.size(); ++i)
                payload[i] ^= maskingKey[i % 4];
            masked = false;
        }
        return is;
    }

    std::ostream& WebsocketFragment::write(std::ostream& os) const
    {
        int c = 0;
        if (fin)
            c |= 0x80;
        if (rsv1)
            c |= 0x40;
        if (rsv2)
            c |= 0x20;
        if (rsv3)
            c |= 0x10;
        c |= char(opcode);
        os.put(c);
        if (masked)
            throw std::runtime_error("WebsocketServer: Masking not available on server side");
        uint64_t payloadLength = payload.size();
        if (payloadLength > 0xffff) {
            os.put(127);
            writeBigEndianNumber<uint64_t>(os, payloadLength);
        }
        else if(payloadLength >=126) {
            os.put(126);
            writeBigEndianNumber<uint16_t>(os, payloadLength);
        }
        else {
            os.put(payloadLength);
        }
        os.write(payload.data(), payload.size());
        return os;
    }
}

class WebsocketServer::Session : private ScriptInterpreter, private Thread
{
public:
    Session(WebsocketServer* parent);
    ~Session();

    // Thread interface
    int OnExecute() override;
    // ScriptInterpreter interface
    void OnScriptError(const std::string&) override;
    // CommandInterpreter interface
    static bool OnWriteLine(void*, const std::string&);
    static bool OnReadLine(void*, std::string&);
    void RequestTermination() override;

    // Input
    void OnMessage(const std::string&);
    // Output
    Session& WriteHello();
    Session& Write(const std::string&);

    void AbortAndWait();
    void Close();

private:
    std::atomic<WebsocketServer*> mpParent = nullptr;
    std::string mLineBuffer;
    TCPSocket mSocket;
    BufferedIO mBuffer;
    std::iostream mStream;
};

// WebsocketServer
WebsocketServer::WebsocketServer(class StateMachine& inStateMachine, const std::string& inAddress)
    : mrStateMachine(inStateMachine)
{
    mListeningSocket.SetTCPNodelay(true);
    mListeningSocket.Open(inAddress);
    if (!mListeningSocket.IsOpen())
        throw bciexception << "WebsocketServer: Cannot listen at " << inAddress;
    Thread::Start();
}

WebsocketServer::~WebsocketServer()
{
    mListeningSocket.Close();
    Thread::TerminateAndWait();
}

std::string WebsocketServer::Address() const
{
    return mListeningSocket.LocalAddress();
}

int WebsocketServer::OnExecute()
{
    while (mListeningSocket.Wait())
        new Session(this);
    return 0;
}

// WebsocketServer::Session
WebsocketServer::Session::Session(WebsocketServer* pParent)
    : Thread(true), ScriptInterpreter(pParent->mrStateMachine), mpParent(pParent), mStream(&mBuffer)
{
    mSocket.SetBlockingMode(false);
    mBuffer.SetIO(&mSocket);
    ScriptInterpreter::WriteLineHandler(&OnWriteLine, this);
    ScriptInterpreter::ReadLineHandler(&OnReadLine, this);
    mpParent.load()->mListeningSocket.WaitForAccept(mSocket, 0);
    Thread::Start();
}

WebsocketServer::Session::~Session()
{
}

void WebsocketServer::Session::AbortAndWait()
{
    ScriptInterpreter::Abort();
    Thread::TerminateAndWait();
}

void WebsocketServer::Session::Close()
{
    Thread::Terminate();
}

int WebsocketServer::Session::OnExecute()
{
    int result = websocketHandshake(mStream);
    if (result < 0) {
        Close();
        return result;
    }

    LocalVariables()[RemoteHostName()] = mSocket.RemoteAddress();
    WriteHello();

    WebsocketFragment f;
    std::string message;
    while (mStream) {
        f.read(mStream);
        switch (f.opcode) {
        case WebsocketFragment::Opcode::continuation:
            message += std::string(f.payload.data(), f.payload.size());
            if (f.fin) {
                OnMessage(message);
                message.clear();
            }
            break;
        case WebsocketFragment::Opcode::text:
            message = std::string(f.payload.data(), f.payload.size());
            if (f.fin) {
                OnMessage(message);
                message.clear();
            }
            break;
        case WebsocketFragment::Opcode::binary:
            Close();
            return -1;
        case WebsocketFragment::Opcode::close:
            f.write(mStream);
            Close();
            return 0;
        case WebsocketFragment::Opcode::ping:
            f.opcode = WebsocketFragment::Opcode::pong;
            f.write(mStream);
            break;
        case WebsocketFragment::Opcode::pong:
            break;
        }
        if (Thread::Terminating()) {
            WebsocketFragment f;
            f.fin = true;
            f.opcode = WebsocketFragment::Opcode::close;
            f.write(mStream).flush();
            while (f.read(mStream) && f.opcode != WebsocketFragment::Opcode::close)
                ;
        }
    }
    Close();
    return 0;
}

void WebsocketServer::Session::OnMessage(const std::string& inMessage)
{
    if (ScriptInterpreter::Execute(inMessage))
    {
        const std::string& r = ScriptInterpreter::Result();
        Write(r);
    }
}

void WebsocketServer::Session::OnScriptError(const std::string& inMessage)
{
    Write(inMessage);
    ScriptInterpreter::OnScriptError(inMessage);
}

bool WebsocketServer::Session::OnWriteLine(void* inInstance, const std::string& inLine)
{
    Session* this_ = reinterpret_cast<Session*>(inInstance);
    this_->Write(inLine);
    return !!(this_->mStream);
}

bool WebsocketServer::Session::OnReadLine(void* inInstance, std::string& outLine)
{
    Session* this_ = reinterpret_cast<Session*>(inInstance);
    this_->Write("\\AwaitingInput:");
    this_->mStream << std::flush;
    WebsocketFragment f;
    bool result = !!f.read(this_->mStream);
    if (result) {
        this_->Write("\\AcknowledgedInput");
        outLine = std::string(f.payload.data(), f.payload.size());
    }
    return result;
}

WebsocketServer::Session& WebsocketServer::Session::WriteHello()
{
    std::ostringstream oss;
    oss << "BCI2000 Version " << VersionInfo::Current[VersionInfo::VersionID];
    std::string hostname = Socket::Hostname();
    if (!hostname.empty())
        oss << " on " << hostname;
    return Write(oss.str());
}

WebsocketServer::Session& WebsocketServer::Session::Write(const std::string& inString)
{
    WebsocketFragment f;
    f.fin = true;
    f.opcode = WebsocketFragment::Opcode::text;
    f.payload.resize(inString.length());
    ::memcpy(f.payload.data(), inString.data(), inString.length());
    f.write(mStream).flush();
    return *this;
}

void WebsocketServer::Session::RequestTermination()
{
    Thread::Terminate();
}
