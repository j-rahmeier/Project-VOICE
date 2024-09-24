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
using System.Collections.Generic;
using System.Text;
using System.Linq;

namespace BCI2000RemoteNET {
    /// <summary>
    ///Provides functionality for control of BCI2000.
    /// </summary>
    public class BCI2000Remote {
	/// <summary>
	/// The <see cref="BCI2000Connection"/> which handles connection with BCI2000
	///Note: It is defined as readonly because I can see no possible case in which it would be useful to swap connections in a BCI2000Remote object, especially as BCI2000Remote holds no state.
	/// </summary>
	public readonly BCI2000Connection connection;

	/// <summary>
	///Constructor for <see cref="BCI2000Remote"/>
	/// </summary>
	/// <param name="connection">Connection object which is connected to a BCI2000 instance. Can be set to connect after this object is constructed as long as no methods of this class which require communication with BCI2000 are called beforehand </param>
	public BCI2000Remote(BCI2000Connection connection) {
	    this.connection = connection;
	}

	/// <summary>
	/// Starts up the specified BCI2000 modules. 
	/// </summary>
	/// <param name="modules">The modules to start. A dictionary whose keys are the names of the modules to start ("SignalGenerator", "DummyApplication", etc.), and whose values are a list of arguments to the modules ("LogKeyboard=1", "LogEyetrackerTobiiPro=1". The "--" in front of each argument is optional. Pass a null instead of a parameter list for no parameters. </param>
	public void StartupModules(IDictionary<string, IEnumerable<string>?> modules) {
	    connection.Execute("startup system");
	    foreach((string mod_name, var mod_args) in modules) {
		//Format arguments to start with --
		var args_p = mod_args?.Select(arg => {
			arg = arg.Trim();
			if (!arg.StartsWith("--")) {
				arg = "--" + arg;
			}
			return arg;
		    });

		string? args_str = args_p?.Aggregate(new StringBuilder(),
			(builder, arg) => {
			    builder.Append(" ");
			    return builder.Append(arg);
			},
			builder => builder.ToString());

		//Add --local argument if it does not exist in list
		if (args_p?.Where(str => str.Equals("--local")).Count() == 0) {
		    args_str = " --local" + args_str;
		}
		connection.Execute($"start executable {mod_name} {args_str ?? " --local"}");
	    }

	    WaitForSystemState(new SystemState[] {SystemState.Connected, SystemState.Initialization});
	    remoteState = RemoteState.Connected;
	}

	/// <summary>
	/// BCI2000 Operator states of operation, as documented on the <anchor xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="https://www.bci2000.org/mediawiki/index.php/User_Reference:Operator_Module_Scripting#WAIT_FOR_%3Csystem_state%3E_[%3Ctimeout_seconds%3E]">BCI2000 Wiki</anchor>
	/// </summary>
	public enum SystemState {
	    Idle,
	    Startup,
	    Initialization,
	    Connected,
	    Resting,
	    Suspended,
	    ParamsModified,
	    Running,
	    Termination,
	    Busy
	}

	/// <summary>
	///Waits for the system to be in the specified state.
	///This will block until the system is in the specified state.
	/// </summary>
	/// <param name="timeout">The timeout value (in seconds) that the command will wait before failing. Leave as null to wait indefinitely.</param>
	/// <returns>True if the system state was reached within the timeout time.</returns>
	public bool WaitForSystemState(SystemState state, double? timeout = null) {
#if (DEBUG)
	    Console.WriteLine($"Wait for {state}");
#endif
	    if (timeout != null) {
		return connection.ExecuteBool($"wait for {state} {timeout?.ToString() ?? ""}");
	    } else {
		connection.Execute($"wait for {state} {timeout?.ToString() ?? ""}");
		return true;
	    }

	}

	/// <summary>
	///Waits for the system to be in one of the specified states.
	///This will block until the system is in the specified state.
	/// </summary>
	/// <param name="timeout">The timeout value (in seconds) that the command will wait before failing. Leave as null to wait indefinitely.</param>
	/// <returns>True if one of the states was reached within the timeout time.</returns>
	public bool WaitForSystemState(SystemState[] states, double? timeout = null) {
	    string states_str = string.Join('|', states.Select(state => state.ToString()).ToArray());
#if (DEBUG)
	    Console.WriteLine($"Wait for {states_str}");
#endif
	    if (timeout != null) {
		return connection.ExecuteBool($"wait for {states_str} {timeout?.ToString() ?? ""}");
	    } else {
		connection.Execute($"wait for {states_str} {timeout?.ToString() ?? ""}");
		return true;
	    }
	}

	/// <summary>
	///Gets the current system state
	/// <exception cref="BCI2000CommandException">If response cannot be parsed into a valid system state </exception>
	/// </summary>
	public SystemState GetSystemState() {
	    string resp = connection.ExecuteString("get system state");
	    if (Enum.TryParse(resp, out SystemState r_state)) {
		return r_state;
	    } else {
		throw new BCI2000CommandException("Could not parse response into state type, received response \"{resp}\"");
	    }
	}

	/// <summary>
	///Sets BCI2000 config, readying it to run. Past this point no parameter changes can be made.
	/// </summary>
	public void SetConfig() {
	    connection.Execute("set config");
	    WaitForSystemState(SystemState.Resting);
	    remoteState = RemoteState.SuspendRun;
	}

	/// <summary>
	///Starts a BCI2000 run, setting config if necessary
	/// </summary>
	/// <exception cref="BCI2000CommandException">Thrown if BCI2000 is not in a state in which it can be immediately started or set config. </exception>
	public void Start(){
	    SystemState current_state = GetSystemState();
	    if (current_state == SystemState.Running) {
		throw new BCI2000CommandException($"Could not start BCI2000 run as BCI2000 is already running.");
	    }
	    else if (current_state == SystemState.Initialization || current_state == SystemState.Connected) {
		SetConfig();
	    } else {
		throw new BCI2000CommandException($"Could not start BCI2000 as BCI2000 is not in a valid state. BCI2000's state is currently {current_state}");
	    }

	    connection.Execute("start system");
	    remoteState = RemoteState.SuspendRun;
	}

	/// <summary> 
	///Stops a BCI2000 run.
	/// </summary>
	/// <exception cref="BCI2000CommandException">Thrown if BCI2000 is not currently recording </exception>
	public void Stop() {
	    SystemState current_state = GetSystemState();
	    if (current_state != SystemState.Running) {
		throw new BCI2000CommandException($"Could not stop BCI2000 run because it is not running, BCI2000 currently in system state {current_state}");
	    }
	    
	    connection.Execute("stop system");
	}

	/// <summary>
	///Adds a parameter to BCI2000. Must be called before <see cref="StartupModules(Dictionary{string, List{string}})"/>.
	///BCI2000RemoteNET provides no abstraction over BCI2000 parameters. It treats them as strings, and declares them within BCI2000 as the dynamic variant type.
	/// </summary>
	/// <param name="section">The section of the parameter. This will be the page on which the parameter appears in the BCI2000 parameters menu. </param>
	/// <param name="name">The name of the parameter. </param>
	/// <param name="defaultValue">The default value of the parameter. This argument is optional. </param>
	/// <param name="maxValue">The maximum value of the parameter. This argument is optional. </param>
	/// <param name="minValue">The minimum value of the parameter. This argument is optional. </param>
	/// <exception cref="BCI2000CommandException">Thrown if BCI2000 is in an invalid state for adding parameters </exception>
	public void AddParameter(string section, string name, string defaultValue = "%", string minValue = "%", string maxValue = "%") {
		var pack = new string[] {section, name, defaultValue, minValue, maxValue};
	    var containsWS = pack.Where(str => str.Any(Char.IsWhiteSpace)).Select(str => $"\"{str}\""); 
	    if (containsWS.Count() != 0) {
		throw new BCI2000CommandException($"Parameter definition parameters must not contain whitespace. Parameter(s) {string.Join(',', containsWS)} contain whitespace.");
	    }
	    if (remoteState != RemoteState.Idle) {
		throw new BCI2000CommandException("Parameters cannot be added after system has been initialized. This method must be called before SetConfig().");
	    }
	    connection.Execute($"add parameter {section} variant {name}= {defaultValue} {minValue} {maxValue}");
	}
	
	/// <summary>
	///Loads the specified <c>.prm</c> file. If <paramref name="filename"/> is relative, it is relative to the working directory of BCI2000, which will most likely be the <c>prog</c> directory in the BCI2000 directory.
	///Must be called before <see cref="StartupModules(Dictionary{string, List{string}})"/>.
	/// </summary>
	/// <param name="filename">Path to the parameter file to load </param>
	/// <exception cref="BCI2000CommandException">Thrown if BCI2000 is in an invalid state for loading parameters </exception>
	public void LoadParameters(string filename) {
	    if (remoteState != RemoteState.Idle) {
		throw new BCI2000CommandException("Parameters cannot be loaded after system has been initialized. This method must be called before SetConfig().");
	    }
	    connection.Execute($"load parameters {filename}");
	}

	/// <summary>
	///Sets a BCI2000 parameter. This must be called while the operator is in the Idle or Connected states.
	/// </summary>
	/// <param name="name">The name of the parameter to set </param>
	/// <param name="value">The value to set the parameter to </param>
	/// <exception cref="BCI2000CommandException">Thrown if BCI2000 is in an invalid state for setting parameters </exception>
	public void SetParameter(string name, string value) {
	    if (remoteState != RemoteState.Idle && remoteState != RemoteState.Connected) {
		SystemState current_state = GetSystemState();
		throw new BCI2000CommandException($"Cannot set parameter, system is not in correct state. Operator must be in state Idle or Connected, but was instead in state {current_state}.");
	    }
	    connection.Execute($"set parameter {name} {value}");
	}

	/// <summary>
	///Gets the value of a BCI2000 parameter.
	/// </summary>
	public string GetParameter(string name) {
	    return connection.ExecuteString($"Get parameter {name}");
	}

	/// <summary>
	///Adds a state variable to BCI2000. State variables have a temporal resolution of one block. To log values with a higher temporal resolution, use <see cref="AddEvent"/>
	///Must be called when BCI2000 is in the Idle system state.
	/// </summary>
	/// <param name="name"> The name of the state to be added </param>
	/// <param name="bitWidth">The bit width of the new state. Must be between 1 and 32. </param>
	/// <param name="initialValue">The initial value of the state. </param>
	/// <exception cref="BCI2000CommandException">Thrown if BCI2000 is in invalid state or invalid parameters passed </param>
	public void AddState(string name, int bitWidth, UInt32 initialValue = 0) {
	    if (name.Any(Char.IsWhiteSpace)) {
		throw new BCI2000CommandException($"Error adding state {name}, state names must not contain whitespace");
	    }
	    if (bitWidth > 32 || bitWidth < 1) {
		throw new BCI2000CommandException($"Bit width of {bitWidth} for state {name} is invalid. Bit width must be between 1 and 32.");
	    }
	    if (remoteState != RemoteState.Idle) {
		throw new BCI2000CommandException($"Operator must be in Idle state to add a state variable, but is in state {GetSystemState()}");
	    }
	    connection.Execute($"add state {name} {bitWidth} {initialValue}"); 
	}

	/// <summary>
	///Sets the specified state to the specified value 
	/// </summary>
	/// <param name="name">The name of the state to set </param>
	/// <param name="value">The value of the state to set </param>
	public void SetState(string name, UInt32 value) {
	    connection.Execute($"set state {name} {value}");
	}

	/// <summary>
	///Gets the value of the specified state
	/// </summary>
	/// <param name="name">The name of the state to get </param>
	public UInt32 GetState(string name){
	    return connection.ExecuteUInt32($"get state {name}");
	}

	/// <summary>
	///Adds an event to BCI2000. Events are similar to state variables but with a temporal resolution of one sample.
	///Must be called when BCI2000 is in the Idle system state.
	/// </summary>
	/// <param name="name"> The name of the state to be added </param>
	/// <param name="bitWidth">The bit width of the new state. Must be between 1 and 32. </param>
	/// <param name="initialValue">The initial value of the state. </param>
	/// <exception cref="BCI2000CommandException">Thrown if BCI2000 is in invalid state or invalid parameters passed </param>
	public void AddEvent(string name, int bitWidth, UInt32 initialValue = 0) {
	    if (name.Any(Char.IsWhiteSpace)) {
		throw new BCI2000CommandException($"Error adding event {name}, event names must not contain whitespace");
	    }
	    if (bitWidth > 32 || bitWidth < 1) {
		throw new BCI2000CommandException($"Bit width of {bitWidth} for event {name} is invalid. Bit width must be between 1 and 32.");
	    }
	    if (remoteState != RemoteState.Idle) {
		throw new BCI2000CommandException($"Operator must be in Idle state to add an event, but is in state {GetSystemState()}");
	    }
	    connection.Execute($"add event {name} {bitWidth} {initialValue}");
	}

	/// <summary>
	///Sets the specified event to the specified value. To set an event for a single sample duration, use <see cref="PulseEvent(string, uint)"/>
	/// </summary>
	/// <param name="name">The name of the event to set </param>
	/// <param name="value">The value of the event to set </param>
	public void SetEvent(string name, UInt32 value) {
	    connection.Execute($"set event {name} {value}");
	}

	/// <summary>
	///Sets the specified event to the specified value for a single sample duration. To set an event to a persistent value, use <see cref="SetEvent(string, uint)"/>
	/// </summary>
	/// <param name="name">The name of the event to pulse </param>
	/// <param name="value">The value of the event to pulse </param>
	public void PulseEvent(string name, UInt32 value) {
	    connection.Execute($"pulse event {name} {value}");
	}

	/// <summary>
	///Gets the value of the signal at the specified <paramref name="channel"/> and <paramref name="element"/>
	/// </summary>
	/// <param name="channel">The channel of the signal to get </param>
	/// <param name="element">The element of the signal to get </param>
	public double GetSignal(int channel, int element) {
	    return connection.ExecuteDouble($"get signal({channel},{element})");
	}

	/// <summary>
	///Gets the value of the specified event
	/// </summary>
	/// <param name="name">The name of the event to get </param>
	public UInt32 GetEvent(string name){
	    return connection.ExecuteUInt32($"get event {name}");
	}

	/// <summary>
	/// Visualizes a BCI2000 value, for example, an event.
	/// </summary>
	/// <param name="value">The expression to visualize. For example, if you wish to visualize an event called `event` pass in the value `"event"` </param>
	public void Visualize(string value)
		{
			if (remoteState == RemoteState.Idle)
			{
				throw new BCI2000CommandException("Cannot visualize value during before initialization. Call this method after StartupModules()");
			}
			connection.Execute($"visualize watch {value}");
		}
	
	//Subset of system states relevant to this class. Used to make sure that certain commands are valid.
	private enum RemoteState {
	    Idle,
	    Connected,
	    SuspendRun
	}
	private RemoteState remoteState = RemoteState.Idle;
    }
}
