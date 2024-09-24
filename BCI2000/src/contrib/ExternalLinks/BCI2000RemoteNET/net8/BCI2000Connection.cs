/////////////////////////////////////////////////////////////////////////////
/// This file is a part of BCI2000RemoteNET, a library
/// for controlling BCI2000 <http://bci2000.org> from .NET programs.
///
///
///
/// BCI20000RemoteNET is free software: you can redistribute it
/// and/or modify it under the terms of the GNU General Public License
/// as published by the Free Software Foundation, either version 3 of
/// the License, or (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////

using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace BCI2000RemoteNET {
    /// <summary>
    ///Provides basic functionality for connection and communication with the BCI2000 operator module.
    /// </summary>
    public class BCI2000Connection {
	//Size of the input read buffer. Should be larger than the largest possible response from BCI2000.
	private const int READ_BUFFER_SIZE = 2048;

	/// <summary>
	/// Timeout value (in milliseconds) of connection to BCI2000
	/// </summary>
	public int Timeout{ get; set; } = 1000;

	/// <summary>
	/// Terminate operator when this object is deleted
	/// </summary>
	public bool TerminateOperatorOnDisconnect { get; set; } = true;

	private string windowTitle = "";
	/// <summary>
	/// The title of the BCI2000 window
	/// </summary>
	public string WindowTitle {
	    get {
		return windowTitle;
	    }
	    set {
		windowTitle = value;
		if (Connected()) {
		    Execute($"set title \"{windowTitle}\"");
		}
	    }
	}

	private bool hideWindow;
	/// <summary>
	/// Hide the BCI2000 window
	/// </summary>
	private bool HideWindow {
	    get {
		return hideWindow;
	    }
	    set {
		hideWindow = value;
		if (Connected()) {
		    switch (hideWindow) {
			case false:
			    Execute("show window");
			    break;
			case true:
			    Execute("hide window");
			    break;
		    }
		}
	    }
	}


	~BCI2000Connection() {
	    Disconnect();
	}

	/// <summary>
	///Disconnects from the operator. Terminated the operator if <see cref="TerminateOperatorOnDisconnect"/> is set.
	/// </summary>
	public void Disconnect() {
	    if (TerminateOperatorOnDisconnect && Connected()) {
		Quit();
	    }
	    if (connection != null) {
		connection.Close();
		connection = null;
	    }
	}

	/// <summary>
	/// Starts an instance of the BCI2000 Operator on the local machine.
	/// </summary>
	/// <param name="operatorPath">The location of the operator binary</param>
	/// <param name="address"> The address on which the Operator will listen for input. Leave as default if you will only connect from the local system.
	/// Note on security: BCI2000Remote uses an unencrypted, unsecured telnet connection. Anyone who can access the connection can run BCI2000 shell scripts. This includes the capability to run arbitrary system shell code from the BCI2000 shell interface. 
	/// Use extreme caution when exposing BCI2000 to the open internet, that is, setting <paramref name="address"/> to a value other than the loopback address (127.0.0.1). Do not leave a connection across machines open unattended. A secure interface is planned for a future release, until then using BCI2000 to communicate between machines on different LANs (not on the same Wi-Fi, in different buildings, etc.) is not recommended. Communication between different machines on the same LAN should be safe provided that the network router does not forward the BCI2000's host machine's BCI2000 port (by default 3999, but can be set on startup.)
	/// </param>
	/// <param name="port"> The port on which the Operator will listen for input. Leave as default unless a specific port is needed.</param>
	/// <param name="delay">Time in milliseconds to wait after starting operator. This is to prevent failure to connect when the operator takes time to start up.</param>
	public void StartOperator(string operatorPath, string address = "127.0.0.1", int port = 3999, int delay = 200) {
	    if (port < 0 || port > 65535) {
		throw new BCI2000ConnectionException($"Port number {port} is not valid");
	    }
	    IPAddress addr = IPAddress.Parse(address);
	    connection = new TcpClient();
	    try {
		connection.Connect(address, port);
		connection.Close();
		connection = null;
		throw new BCI2000ConnectionException($"{address}:{port} is occupied, is BCI2000 already running?");
	    } catch (SocketException) {
		//Socket should not connect if BCI2000 is not already running
	    }
	    connection = null;

	    StringBuilder arguments = new StringBuilder();
	    arguments.Append($" --Telnet \"{addr.ToString()}:{port}\" ");
	    arguments.Append(" --StartupIdle ");
	    if (!string.IsNullOrEmpty(WindowTitle)) {
		arguments.Append($" --Title \"{WindowTitle}\" ");
	    }
	    if (HideWindow) {
		arguments.Append(" --Hide ");
	    }
	    try {
		System.Diagnostics.Process.Start(operatorPath, arguments.ToString());
#if DEBUG
		Console.WriteLine($"Started operator path {operatorPath} at {address}:{port}");
#endif
	    } catch (Exception ex) {
		throw new BCI2000ConnectionException($"Could not start operator at path {operatorPath}: {ex.ToString()}");
	    }
	    Thread.Sleep(delay);
	}

	/// <summary>
	///Establishes a connection to an instance of BCI2000 running at the specified address and port.
	/// </summary>
	/// <param name="address">The IPv4 address to connect to. Note that this may not necessarily be the same as the one used in <see cref="StartOperator">StartOperator</see>, even if running BCI2000 locally. For example, if the operator was started on the local machine with address <c>0.0.0.0</c>, you would connect to it at address <c>127.0.0.1</c></param>
	/// <param name="port">The port on which BCI2000 is listening. If BCI2000 was started locally with <see cref="StartOperator">StartOperator</see>, this must be the same value.</param>
	public void Connect(string address = "127.0.0.1", int port = 3999) {
	    if (port < 0 || port > 65535) {
		throw new BCI2000ConnectionException($"Port number {port} is not valid");
	    }
	    IPAddress addr = IPAddress.Parse(address);

	    if (Connected()) {
		throw new BCI2000ConnectionException("Connect() called while already connected. Call Disconnect() first.");
	    }
	    if (connection != null) {
		throw new BCI2000ConnectionException("Connect called while connection is null. This should not happen and is likely a bug. Please report to the maintainer.");
	    }
	    connection = new TcpClient();
	    try {
		connection.Connect(addr, port);
	    } catch (Exception ex) {
		throw new BCI2000ConnectionException($"Could not connect to operator at {addr.ToString()}:{port}, {ex.ToString()}");
	    }

	    op_stream = connection.GetStream();

	    connection.SendTimeout = Timeout;
	    connection.ReceiveTimeout = Timeout;

	    DiscardResponse(); //Throw out startup messages
	    Execute("change directory $BCI2000LAUNCHDIR");
	}

	/// <summary>
	///Gets whether or not this BCI2000Remote instance is currently connected to the BCI2000 Operator
	/// </summary>
	/// <returns>Whether or not this object is currently connected to BCI2000</returns>
	public bool Connected() {
	    return connection?.Connected ?? false;
	}

	/// <summary>
	///Shuts down the connected BCI2000 instance
	/// </summary>
	public void Quit() {
	    Execute("Quit");
	}

	/// <summary>
	///Executes the given command and returns the result as type <typeparamref name="T"/>. Throws if the response cannot be parsed as <typeparamref name="T"/>. If you are trying to execute a command which does not produce output, use <see cref="Execute(string, bool)"/>.
	/// </summary>
	/// <typeparam name="T">Type of the result of the command. Must implement <see cref="IParsable{TSelf}"/>.</typeparam> 
	/// <param name="command">The command to execute</param>
	public T Execute<T>(string command) where T : IParsable<T> {
	    SendCommand(command);
	    if (!Connected()) {
		throw new BCI2000ConnectionException("No connection to BCI2000 Operator");
	    }
	    return GetResponseAs<T>();
	}

	/// <summary>
	///Executes the given command. Will throw if a non-blank response is received from BCI2000 and <paramref name="expectEmptyResponse"/> is not set to false. 
	/// </summary>
	/// <param name="command">The command to send to BCI2000</param>
	/// <param name="expectEmptyResponse">By default, this function will throw if its command receives a non-empty response from BCI2000. This is because most BCI2000 commands which do not return a value will not send a response if they succeed. If set to false, this function will acceept non-empty responses from BCI2000.
	public void Execute(string command, bool expectEmptyResponse = true) {
	    SendCommand(command);
	    if (!Connected()) {
		throw new BCI2000ConnectionException("No connection to BCI2000 Operator");
	    }
	    if (expectEmptyResponse) {
		ExpectEmptyResponse();
	    } else {
		DiscardResponse();
	    }
	}

	//Sends command to BCI2000
	private void SendCommand(string command){
#if (DEBUG)
			Console.WriteLine("send: " + command);
#endif
			try {
		op_stream!.Write(System.Text.Encoding.ASCII.GetBytes(command + "\r\n"));
	    } catch (Exception ex) {
		throw new BCI2000ConnectionException($"Failed to send command to operator, {ex}");
	    }
	}

	//Gets the response from the operator and attempts to parse into the given type
	private T GetResponseAs<T>() where T : IParsable<T> {
	    string resp = ReceiveResponse();
	    try {
		T result = T.Parse(resp, null);
		return result;
	    } catch (Exception ex) {
		throw new BCI2000CommandException($"Could not parse response {resp} as type {nameof(T)}, {ex}");
	    }

	}

	//Receives response from operator and throws if response is not blank. Used with commands which expect no response, such as setting events and parameters.
	private void ExpectEmptyResponse() { 
	    string resp = ReceiveResponse();
	    if (!IsEmptyOrPrompt(resp)) {
		throw new BCI2000CommandException($"Expected empty response but received {resp} instead");
	    }
	}


	//Receives response and discards the result.
	private void DiscardResponse() {
	    ReceiveResponse();
	}

	private byte[] recv_buffer = new byte[READ_BUFFER_SIZE];
	//Receives response from the operator. Blocks until the prompt character ('>') is received.
	private string ReceiveResponse() {
	    StringBuilder response = new StringBuilder();
			bool receiving = true;
			while (receiving)
			{
				if (!op_stream!.DataAvailable)
				{
					continue;
				}
				int read = op_stream.Read(recv_buffer, 0, recv_buffer.Length);
				if (read > 0)
				{
					string resp_fragment = System.Text.Encoding.ASCII.GetString(recv_buffer, 0, read);
#if (DEBUG)
					Console.WriteLine("fr: " + resp_fragment);
#endif
					if (EndsWithPrompt(resp_fragment))
					{
						response.Append(resp_fragment.AsSpan(0, resp_fragment.LastIndexOf('>'))); //Don't include prompt in response
						receiving = false;
						break;
					} else
					{
                        response.Append(resp_fragment);
					} 
				}
			}
#if (DEBUG)
			Console.WriteLine("recv: " + response.ToString());
#endif
			return response.ToString();
	}

        private bool EndsWithPrompt(string line)
        {
            string lineTrim = line.ToString().Trim();
            if (lineTrim.Length == 0) return false;
            return lineTrim.Substring(lineTrim.Length - 1).Equals(Prompt);
        }

		//Checks if string consists of only whitespace characters or Prompt
		private bool IsEmptyOrPrompt(string s)
		{
			if (s == null)
			{
				return true;
			}
			foreach (char c in s)
			{
                if (c <= 0x20 || c == '>')
                {
                    //Do nothing if char is <= x20 (which means it is whitespace) or is prompt.
                } else
				{
					return false;
				}
            }
			return true;
		}

	private TcpClient? connection;
	private NetworkStream? op_stream;

        private const string ReadlineTag = "\\AwaitingInput:";
        private const string AckTag = "\\AcknowledgedInput";
        private const string ExitCodeTag = "\\ExitCode";
        private const string TerminationTag = "\\Terminating";
        private const string Prompt = ">";
    }
}
