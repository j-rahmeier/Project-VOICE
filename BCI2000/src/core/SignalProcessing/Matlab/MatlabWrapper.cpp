////////////////////////////////////////////////////////////////////////////////
// $Id: MatlabWrapper.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
//         jeremy.hill@tuebingen.mpg.de
// Description: Wrapper classes for convenient creation and manipulation of
//              Matlab workspace variables, and calling of Matlab functions.
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
#include "MatlabWrapper.h"
#include "EnvVariable.h"
#include "FileUtils.h"
#include "ProcessUtils.h"
#include "Streambuf.h"
#include <sstream>
#include <regex>

namespace
{
const std::string cErrorVariable = "bci_Error";
const std::string cAnsVariable = "bci_Ans";
const std::string cAns2Variable = "bci_Ans2";
} // anonymous namespace

////////////////////////////////////////////////////////////////////////////////
// MatlabConnection::IntMatrix definitions                                        //
////////////////////////////////////////////////////////////////////////////////
MatlabConnection::IntMatrix::IntMatrix(int rows, int cols)
    : std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0))
{
}

int MatlabConnection::IntMatrix::rows() const
{
    return size();
}

int MatlabConnection::IntMatrix::cols() const
{
    return empty() ? 0 : front().size();
}

MatlabConnection::IntMatrix MatlabConnection::IntMatrix::fromSignalProperties(const SignalProperties &inProperties)
{
    IntMatrix m(1, 2);
    m[0][0] = inProperties.Channels();
    m[0][1] = inProperties.Elements();
    return m;
}

SignalProperties MatlabConnection::IntMatrix::toSignalProperties() const
{
    size_t channels = 0, elements = 0;
    if (!empty())
    {
        if (!(*this)[0].empty())
            channels = static_cast<size_t>((*this)[0][0]);
        if ((*this)[0].size() > 1)
            elements = static_cast<size_t>((*this)[0][1]);
    }
    return SignalProperties(channels, elements, SignalType::float32);
}

////////////////////////////////////////////////////////////////////////////////
// MatlabConnection::StringMatrix definitions                                 //
////////////////////////////////////////////////////////////////////////////////
MatlabConnection::StringMatrix::StringMatrix(int rows, int cols)
    : std::vector<std::vector<std::string>>(rows, std::vector<std::string>(cols))
{
}

int MatlabConnection::StringMatrix::rows() const
{
    return size();
}

int MatlabConnection::StringMatrix::cols() const
{
    return empty() ? 0 : front().size();
}

MatlabConnection::StringMatrix MatlabConnection::StringMatrix::fromParam(const Param &p)
{
    StringMatrix m(p.NumRows(), p.NumColumns());
    for (size_t row = 0; row < m.size(); ++row)
        for (size_t column = 0; column < m[row].size(); ++column)
            m[row][column] = p.Value(row, column);
    return m;
}

////////////////////////////////////////////////////////////////////////////////
// MatlabConnection definitions                                                   //
////////////////////////////////////////////////////////////////////////////////
int MatlabConnection::sNumInstances = 0;
Socket *MatlabConnection::spMatlabSocket = nullptr;

MatlabConnection::MatlabConnection()
{
    sNumInstances++;
}

MatlabConnection::~MatlabConnection()
{
    sNumInstances--;
}

std::string MatlabConnection::FindMatlab()
{
#if WIN32
    for (auto var : {"PROGRAMFILES", "PROGRAMFILES(X86)", "ProgramW6432"})
    {
        std::string dir = EnvVariable(var).Get();
        dir += "\\MATLAB\\";
        FileUtils::List list;
        if (FileUtils::ListDirectory(dir, list))
        {
            std::regex matlabRelease("R[0-9]{4}(a|b)");
            for (const auto& subdir : list)
              if (std::regex_match(subdir, matlabRelease))
                return dir + subdir + "\\bin\\matlab.exe";
        }
    }
    return "";
#else
    return "matlab";
#endif
}

void MatlabConnection::Open(const std::string &executable)
{
    if (spMatlabSocket && !spMatlabSocket->Connected())
    {
        spMatlabSocket->Close();
        delete spMatlabSocket;
        spMatlabSocket = nullptr;
    }
    if (!spMatlabSocket)
    {
        int timeout_ms = 100000;
        std::string address = "127.0.0.1:3997";
        TCPSocket *pMatlabSocket = new TCPSocket();
        pMatlabSocket->SetTCPNodelay(true);
        ServerTCPSocket server;
        server.Open(address);
        if (!server.Listening())
        {
            // This test is important to avoid a security flaw:
            // attacker on local machine could listen on localhost and later execute commands
            // with Matlab user's privileges
            bcierr << "Could not listen on " << address << ": " << SysError().Message();
            return;
        }
        std::string connectionClient =
            FileUtils::ExtractDirectory(FileUtils::ExecutablePath()) + "matlab/bci_ConnectionClient.m";
        std::string args = "-r \"run('" + connectionClient + "');\"";
        int exitCode = 0;
        ProcessUtils::Pid pid = 0;
        bool ok = ProcessUtils::StartProcess(executable, args, exitCode, pid);
        if (!ok)
        {
            bcierr << "Could not start Matlab: " << SysError().Message();
        }
        else if (!server.WaitForAccept(*pMatlabSocket, timeout_ms))
        {
            bcierr << "Matlab did not connect within " << timeout_ms / 1000 << " seconds";
            delete pMatlabSocket;
        }
        else
        {
            spMatlabSocket = pMatlabSocket;
            PutString(cErrorVariable, "");
        }
    }
}

bool MatlabConnection::IsOpen()
{
    if (!spMatlabSocket)
        return false;
    return spMatlabSocket->Connected();
}

void MatlabConnection::Close(bool closeMatlab)
{
    if (sNumInstances > 0)
    {
        bcierr << "Cannot close Matlab connection while client instances are still present";
    }
    else if (IsOpen())
    {
        ClearVariable(cErrorVariable);
        if (closeMatlab)
            spMatlabSocket->Write("Q:\n", 3);
        else
            spMatlabSocket->Write("q:\n", 3);
        delete spMatlabSocket;
        spMatlabSocket = nullptr;
    }
}

bool MatlabConnection::Pause()
{
    if (!spMatlabSocket)
        return false;
    spMatlabSocket->Write("p:\n", 3);
    return true;
}

bool MatlabConnection::Continue()
{
    if (!spMatlabSocket)
        return false;
    spMatlabSocket->Write("c:\n", 3);
    return true;
}

bool MatlabConnection::Execute(const std::string &inCommands)
{
    if (!spMatlabSocket)
        return false;

    std::string command = "x:";
    for (auto c : inCommands)
        if (c != '\n')
            command += c;
    command += '\n';

    spMatlabSocket->Write(command.data(), command.size());
    return true;
}

bool MatlabConnection::Execute(const std::string &inCommands, std::string &outError)
{
    outError.clear();
    std::string command = cErrorVariable + "=''; try, " + inCommands + "; catch, " + cErrorVariable + "=lasterr; end;";
    if (!MatlabConnection::Execute(command))
        outError = "Could not execute Matlab command:\n" + command;
    else
    {
        outError = MatlabConnection::GetString(cErrorVariable);
        MatlabConnection::PutString(cErrorVariable, "");
    }
    return outError.empty();
}

bool MatlabConnection::CreateGlobal(const std::string &inName)
{
    return Execute("global " + inName + ";");
}

bool MatlabConnection::ClearVariable(const std::string &inName)
{
    return Execute("clear " + inName + "; clear global " + inName + ";");
}

bool MatlabConnection::ClearObject(const std::string &inName)
{
    return Execute("if( exist('" + inName + "', 'var') ); delete(" + inName + "); end;") && ClearVariable(inName);
}

std::string MatlabConnection::GetString(const std::string &inExp)
{
    std::string result;
    auto m = GetIntMatrix(inExp);
    for (int i = 0; i < m.cols(); ++i)
        result += char(m[0][i]);
    return result;
}

bool MatlabConnection::PutString(const std::string &inExp, const std::string &inValue)
{
    std::string command = inExp + "='";
    for (auto c : inValue)
    {
        switch (c)
        {
        case '\'':
            command += "''";
            break;
        case '\\':
            command += "\\\\";
            break;
        default:
            command += c;
        }
    }
    command += "';";
    bool ok = Execute(command);
    if (!ok)
        bcierr << "Could not evaluate " << inExp;
    return ok;
}

MatlabConnection::IntMatrix MatlabConnection::GetIntMatrix(const std::string &inExp)
{
    std::string command = "g:" + inExp + "\n";
    spMatlabSocket->Write(command.data(), command.size());
    UnbufferedIO buf;
    buf.SetInput(spMatlabSocket);
    std::istream is(&buf);
    int rows = 0, cols = 0;
    is >> rows >> cols;
    IntMatrix result(rows, cols);
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            is >> result[row][col];
        }
    }
    if (!is || is.get() != ' ')
        bcierr << "Unexpected result when retrieving " << inExp;
    return result;
}

bool MatlabConnection::PutIntMatrix(const std::string &inExp, const IntMatrix &inValue)
{
    std::ostringstream command;
    command << inExp << "=[ ";
    for (int row = 0; row < inValue.rows(); ++row)
    {
        for (int col = 0; col < inValue.cols(); ++col)
        {
            command << inValue[row][col] << " ";
        }
        command << "; ";
    }
    command << "];";
    std::string error;
    bool ok = Execute(command.str(), error);
    if (!ok)
        bcierr << "Could not evaluate " << inExp << ": " << error;
    return ok;
}

int MatlabConnection::GetIntScalar(const std::string &inExpr)
{
    auto m = GetIntMatrix(inExpr);
    if (m.rows() < 1 || m.cols() < 1)
        bcierr << "GetIntScalar() called for empty matrix";
    return m[0][0];
}

bool MatlabConnection::PutIntScalar(const std::string &inExpr, int inValue)
{
    IntMatrix value(1, 1);
    value[0][0] = inValue;
    return PutIntMatrix(inExpr, value);
}

MatlabConnection::StringMatrix MatlabConnection::GetCells(const std::string &inExp)
{
    auto size = GetIntMatrix("size(" + inExp + ")");
    if (size.cols() != 2)
        bcierr << "Can only handle two dimensions";
    if (size.rows() > 1)
        bcierr << "Rows in size matrix should be 1, is " << size.rows();
    Execute(cAns2Variable + "=" + inExp + ";");
    StringMatrix result(size[0][0], size[0][1]);
    for (int row = 0; row < result.rows(); ++row)
    {
        for (int col = 0; col < result.cols(); ++col)
        {
            std::ostringstream oss;
            oss << cAns2Variable << "{" << row + 1 << "," << col + 1 << "}";
            result[row][col] = GetString(oss.str());
        }
    }
    ClearVariable(cAns2Variable);
    return result;
}

bool MatlabConnection::PutCells(const std::string &inExp, const StringMatrix &inValue)
{
    bool ok = true;
    if (inValue.rows() == 0 || inValue.cols() == 0)
    {
        ok = Execute(inExp + "={};");
    }
    else
    {
        ClearVariable(cAns2Variable);
        for (int row = 0; row < inValue.rows(); ++row)
        {
            for (int col = 0; col < inValue.cols(); ++col)
            {
                std::ostringstream oss;
                oss << cAns2Variable << "{" << row + 1 << "," << col + 1 << "}";
                ok = ok && PutString(oss.str(), inValue[row][col]);
            }
        }
        ok = ok && Execute(inExp + "=" + cAns2Variable + ";");
        ClearVariable(cAns2Variable);
    }
    if (!ok)
        bcierr << "Could not put cells";
    return ok;
}

////////////////////////////////////////////////////////////////////////////////
// MatlabFunction definitions                                                 //
////////////////////////////////////////////////////////////////////////////////
MatlabFunction::MatlabFunction(const std::string &inName) : mName(inName), mExists(false)
{
    if (MatlabConnection::IsOpen())
    {
        // Check whether there exists a function with the required name in
        // the Matlab search path.
        std::string command = "rehash;";
        command += cAnsVariable + "=exist('" + inName + "');";
        if (MatlabConnection::Execute(command))
        {
            IntMatrix ans = GetIntMatrix(cAnsVariable);
            if (ans.rows() > 0 && ans.cols() > 0)
            {
                int result = ans[0][0];
                enum
                {
                    mfile = 2,
                    mexfile = 3,
                    mbuiltin = 5,
                    pfile = 6,
                };
                switch (result)
                {
                case mfile:
                case mexfile:
                case mbuiltin:
                case pfile:
                    mExists = true;
                    break;
                default:
                    mExists = false;
                }
            }
            ClearVariable(cAnsVariable);
        }
    }
}

MatlabFunction::~MatlabFunction()
{
}

MatlabFunction &MatlabFunction::InputArgument(const std::string &inArg)
{
    mInputArguments.push_back(inArg);
    return *this;
}

MatlabFunction &MatlabFunction::OutputArgument(const std::string &inArg)
{
    mOutputArguments.push_back(inArg);
    return *this;
}

MatlabFunction &MatlabFunction::CodePre(const std::string &inCode)
{
    mCodePre = inCode;
    return *this;
}

MatlabFunction &MatlabFunction::CodePost(const std::string &inCode)
{
    mCodePost = inCode;
    return *this;
}

std::string MatlabFunction::Execute() const
{
    std::string result;
    if (!mExists)
        result = "Function " + mName + " does not exist in the Matlab search path.";
    else
    {
        std::ostringstream command;
        command << mCodePre;
        if (!mOutputArguments.empty())
        {
            command << "[" << mOutputArguments[0];
            for (size_t i = 1; i < mOutputArguments.size(); ++i)
                command << "," << mOutputArguments[i];
            command << "]=";
        }
        command << mName;
        if (!mInputArguments.empty())
        {
            command << "(" << mInputArguments[0];
            for (size_t i = 1; i < mInputArguments.size(); ++i)
                command << "," << mInputArguments[i];
            command << ");";
        }
        command << mCodePost;
        MatlabConnection::Execute(command.str(), result);
    }
    return result;
}
