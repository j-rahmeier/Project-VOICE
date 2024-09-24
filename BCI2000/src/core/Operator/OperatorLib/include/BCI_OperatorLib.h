/*********************************************************************
 * $Id: BCI_OperatorLib.h 8091 2024-05-01 13:48:52Z mellinger $
 * Author:      juergen.mellinger@uni-tuebingen.de
 * Description: C-style interface to the BCI2000 operator library.
 *
 * $BEGIN_BCI2000_LICENSE$
 *
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
 *
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_BCI2000_LICENSE$
 *********************************************************************/
#ifndef BCI_OPERATOR_LIB_H
#define BCI_OPERATOR_LIB_H

#ifdef _WIN32
#define STDCALL __stdcall
#ifdef LIBOPERATOR_LIBRARY
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif /* LIBOPERATOR_LIBRARY */
#elif __GNUC__ && __GNUC__ >= 4
#define STDCALL
#ifdef LIBOPERATOR_LIBRARY
#define DLLEXPORT __attribute__((visibility("default")))
#else
#define DLLEXPORT
#endif /* LIBOPERATOR_LIBRARY */
#else
#define STDCALL
#define DLLEXPORT
#endif

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /*
    function:  BCI_GetInfo
    purpose:   Reports build and source version information.
    arguments: None.
    returns:   Pointer to a null-terminated string holding the information requested.
               The output buffer is allocated by the library, and should be released
               by the caller using BCI_ReleaseObject().
    */
    DLLEXPORT const char *STDCALL BCI_GetInfo(void);

    /*
    function:  BCI_PutParameter
    purpose:   Parses a BCI2000 parameter definition line, and adds the resulting
               parameter object to the internal parameter list, or changes the value of
               a parameter if it exists.
    arguments: Pointer to a null-terminated parameter line string.
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_PutParameter(const char *parameterLine);

    /*
    function:  BCI_GetParameter
    purpose:   Returns the parameter with the given index from the operator's internal
               parameter list.
    arguments: Parameter index.
    returns:   Pointer to a null-terminated string containing a parameter line, or NULL.
               The output buffer is allocated by the library, and should be released
               by the caller using BCI_ReleaseObject().
    */
    DLLEXPORT const char *STDCALL BCI_GetParameter(long index);

    /*
    function:  BCI_GetStream
    purpose:   Returns the stream with the given index from the DLL's internal
               state list.
    arguments: Stream index.
    returns:   Pointer to a null-terminated string containing a stream definition, or NULL.
               The output buffer is allocated by the library, and should be released
               by the caller using BCI_ReleaseObject().
    */
    DLLEXPORT const char *STDCALL BCI_GetStream(long index);

    /*
    function:  BCI_PutState
    purpose:   Parses a BCI2000 state definition line, and adds the resulting
               state to the operator library's state list.
    arguments: Pointer to a null-terminated state line string.
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_PutState(const char *stateLine);

    /*
    function:  BCI_GetState
    purpose:   Returns the state with the given index from the DLL's internal
               state list.
    arguments: State index.
    returns:   Pointer to a null-terminated string containing a state line, or NULL.
               The output buffer is allocated by the library, and should be released
               by the caller using BCI_ReleaseObject().
    */
    DLLEXPORT const char *STDCALL BCI_GetState(long index);

    /*
    function:  BCI_SetStateValue
    purpose:   Sets the value of a state to a given value.
    arguments: Pointer to a null-terminated state name string; new state value.
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_SetStateValue(const char *stateName, long value);

    /*
    function:  BCI_GetStateValue
    purpose:   Returns the value of a state.
    arguments: Pointer to a null-terminated state name string.
    returns:   State value, or 0 if the state does not exist.
    */
    DLLEXPORT long STDCALL BCI_GetStateValue(const char *stateName);

    /*
    function:  BCI_PutEvent
    purpose:   Parses a BCI2000 event definition line, and adds the resulting
               event to the operator library's event list.
    arguments: Pointer to a null-terminated event line string.
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_PutEvent(const char *eventLine);

    /*
    function:  BCI_GetEvent
    purpose:   Returns the event with the given index from the DLL's internal
               event list.
    arguments: Event index.
    returns:   Pointer to a null-terminated string containing an event line, or NULL.
               The output buffer is allocated by the library, and should be released
               by the caller using BCI_ReleaseObject().
    */
    DLLEXPORT const char *STDCALL BCI_GetEvent(long index);

    /*
    function:  BCI_SetEvent
    purpose:   Asynchronously sets an event to a given value.
    arguments: Pointer to a null-terminated event name string; new event value.
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_SetEvent(const char *eventName, long value);

    /*
    function:  BCI_PulseEvent
    purpose:   Asynchronously sets an event to a given value, for a single sample.
    arguments: Pointer to a null-terminated event name string; temporary event value.
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_PulseEvent(const char *eventName, long value);

    /*
    function:  BCI_GetSignalChannels
    purpose:   Returns the number of channels in the control signal.
    arguments: None
    returns:   Number of signal channels.
    */
    DLLEXPORT int STDCALL BCI_GetSignalChannels(void);

    /*
    function:  BCI_GetSignalElements
    purpose:   Returns the number of elements in the control signal.
    arguments: None
    returns:   Number of signal elements.
    */
    DLLEXPORT int STDCALL BCI_GetSignalElements(void);

    /*
    function:  BCI_GetSignal
    purpose:   Returns a value from the control signal.
    arguments: Channel index, element index (zero-based)
    returns:   Signal value.
    */
    DLLEXPORT float STDCALL BCI_GetSignal(int channel, int element);

    /*
    function:  BCI_PutVisProperty
    purpose:   Sets the a property to the given value, or adds the property to
               to the property list if it is not present.
    arguments: Pointer to a null-terminated vis ID string, numeric config ID, and
               a pointer to a null-terminated value string.
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_PutVisProperty(const char *visID, int cfgID, const char *value);

    /*
    function:  BCI_GetVisProperty
    purpose:   Returns the property with the given index from the DLL's internal
               property list. When a visID is given, results are restricted to properties
               with the respective visID.
    arguments: Pointer to a null-terminated vis ID string, numeric config ID.
    returns:   Pointer to a null-terminated string containing a property line, or NULL.
               The output buffer is allocated by the library, and should be released
               by the caller using BCI_ReleaseObject().
    */
    DLLEXPORT const char *STDCALL BCI_GetVisProperty(const char *visID, int cfgID);

    /*
    Enumeration of externally visible state machine states.
    */
    enum BCI_State
    {
        BCI_StateUnavailable,
        BCI_StateStartup,
        BCI_StateConnected,
        BCI_StateInitialization = BCI_StateConnected,
        BCI_StateResting,
        BCI_StateSuspended,
        BCI_StateParamsModified,
        BCI_StateRunning,
        BCI_StateBusy,
        BCI_StateIdle,
    };

    /*
    function:  BCI_GetStateOfOperation
    purpose:   Determines the externally visible state of the state machine, or
               the state of operation of the BCI2000 system.
    arguments: None
    returns:   State of operation.
    */
    DLLEXPORT int STDCALL BCI_GetStateOfOperation();

    /*
    function:  BCI_GetConnectionInfo
    purpose:   Obtains information about a core module connection.
    argument:  Zero-based index of core module connection.
    returns:   Pointer to a null-terminated string containing connection information.
               The output buffer is allocated by the library, and should be released
               by the caller using BCI_ReleaseObject().
    */
    DLLEXPORT const char *STDCALL BCI_GetConnectionInfo(int index);

    /*
    function:  BCI_GetCoreModuleStatus
    purpose:   Obtains a core module's current status message.
    argument:  Zero-based index of core module connection.
    returns:   Pointer to a null-terminated string containing status information.
               The output buffer is allocated by the library, and should be released
               by the caller using BCI_ReleaseObject().
    */
    DLLEXPORT const char *STDCALL BCI_GetCoreModuleStatus(int index);

    /*
    function:  BCI_Startup
    purpose:   Listens on core module connection ports.
    arguments: A string defining core module names and listening ports in the form
                 <ip address> <name1>:<port1> <name2:port2> ... <nameN:portN>
               If NULL, a value of
                 "Source:4000 SignalProcessing:4001 Application:4002"
               representing a standard BCI2000 configuration is used.
               The first argument specifies an IP address on which to listen,
               "localhost", or "134.2.131.251".
               In standard configuration, the Operator module listens on all available
               addresses.
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_Startup(const char *moduleList);

    /*
    function:  BCI_Shutdown
    purpose:   Close connections to core modules, and go into idle state.
    arguments: n/a
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_Shutdown(void);

    /*
    function:  BCI_Initialize
    purpose:   Initialize the library. Must be called before any other library
               function is used. This function should be called from your
               application's main thread.
    arguments: n/a
    returns:   1 if no error occurred, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_Initialize(void);

    /*
    function:  BCI_Dispose
    purpose:   Dispose of all resources allocated by the library. This function
               must be called from the thread that called BCI_Initialize().
    arguments: n/a
    returns:   1 if no error occurred, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_Dispose(void);

    /*
    function:  BCI_TelnetListen
    purpose:   Start a telnet server, listening at the given address.
    arguments: Address in <IP>:<port> format, defaults to "localhost:3999"
    returns:   1 if no error occurred, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_TelnetListen(const char *);

    /*
    function:  BCI_TelnetCloseOne
    purpose:   Stop the telnet server at the given address, keeping open connections.
    arguments: Address in <IP>:<port> format, or NULL for all addresses
    returns:   1 if no error occurred, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_TelnetCloseOne(const char*);

    /*
    function:  BCI_TelnetClose
    purpose:   Stop all telnet servers, keeping open connections.
    arguments: N/A
    returns:   1 if no error occurred, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_TelnetClose(void);

    /*
    function:  BCI_WebsocketListen
    purpose:   Start a websocket server, listening at the given address.
    arguments: Address in <IP>:<port> format, defaults to "localhost:3998"
    returns:   1 if no error occurred, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_WebsocketListen(const char *);

    /*
    function:  BCI_WebsocketCloseOne
    purpose:   Stop the websocket server at the given address, keeping open connections.
    arguments: Address in <IP>:<port> format
    returns:   1 if no error occurred
    , 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_WebsocketCloseOne(const char*);

    /*
    function:  BCI_WebsocketClose
    purpose:   Stop all websocket servers, keeping open connections.
    arguments: N/A
    returns:   1 if no error occurred, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_WebsocketClose(void);

    /*
    function:  BCI_SetConfig
    purpose:   Applies current parameter settings to the BCI2000 system.
    arguments: n/a
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_SetConfig(void);

    /*
    function:  BCI_StartRun
    purpose:   Starts a new run.
    arguments: n/a
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_StartRun(void);

    /*
    function:  BCI_StopRun
    purpose:   Stops the current run.
    arguments: n/a
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_StopRun(void);

    /*
    function:  BCI_ExecuteScript
    purpose:   Interprets and executes the specified script.
    arguments: Null-terminated string specifying script commands.
    returns:   0 if a syntax error is present, 1 otherwise.
    */
    DLLEXPORT int STDCALL BCI_ExecuteScript(const char *script);

    /*
    function:  BCI_ExecuteScriptWithResult
    purpose:   Interprets and executes the specified script.
               After successful execution, the result of the last
               script command is returned.
    arguments: Null-terminated string specifying script commands.
    returns:   Pointer to a null-terminated string containing the result,
               or NULL when a syntax error is present. The result is
               allocated by the library, and should be released by the
               caller using BCI_ReleaseObject().
    */
    DLLEXPORT const char *STDCALL BCI_ExecuteScriptWithResult(const char *script);

    /*
    function:  BCI_AllocateString
    purpose:   Allocate a copy of the given string.
    arguments: Pointer to a null-terminated string.
    returns:   Pointer to a null-terminated copy of the argument string.
               The result is allocated by the library and should be released
               using BCI_ReleaseObject().
    */
    DLLEXPORT const char* STDCALL BCI_AllocateString(const char*);

    /*
    function:  BCI_ReleaseObject
    purpose:   Indicate that an object that has been allocated by the library is no longer
               needed by the library's client.
    arguments: Object to be released, or NULL.
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_ReleaseObject(const char *);

    /*
    Enumeration of callback events.
    Event handler arguments are given following the event.
    */
    enum BCI_Event
    {
        BCI_None = -1,

        BCI_OnSystemStateChange = 0, /* ( void* refdata ) */
        BCI_OnCoreInput,             /* ( void* refdata ) */

        BCI_OnStartup,   /* ( void* refdata ) executed when system is started up */
        BCI_OnConnect,   /* ( void* refdata ) */
        BCI_OnSetConfig, /* ( void* refdata ) */
        BCI_OnStart,     /* ( void* refdata ) executed when user clicked "Start" */
        BCI_OnSuspend,   /* ( void* refdata ) executed when system switches to Suspended mode */
        BCI_OnStopRun = BCI_OnSuspend,
        BCI_OnResume,    /* ( void* refdata ) executed when user clicked "Resume" */
        BCI_OnStartRun,  /* ( void* refdata, const char* filename ) executed when system switches to Running mode */
        BCI_OnShutdown,  /* ( void* refdata ) executed when closing connections */

        BCI_OnLogMessage,     /* ( void* refdata, const char* msg ) */
        BCI_OnWarningMessage, /* ( void* refdata, const char* msg ) */
        BCI_OnErrorMessage,   /* ( void* refdata, const char* msg ) */
        BCI_OnDebugMessage,   /* ( void* refdata, const char* msg ) */
        BCI_OnNoteMessage,   /* ( void* refdata, const char* msg ) */

        BCI_OnParameter,          /* ( void* refdata, const char* parameterline ) */
        BCI_OnState,              /* ( void* refdata, const char* stateline ) */
        BCI_OnVisPropertyMessage, /* ( void* refdata, const char* name, int cfgID, const char* value ) */
        BCI_OnVisProperty,        /* ( void* refdata, const char* name, int cfgID, const char* value )
                                     value may be NULL to indicate that property should be reset */

        BCI_OnInitializeVis, /* ( void* refdata, const char* visID, const char* kind ) */
        BCI_OnVisMemo,       /* ( void* refdata, const char* visID, const char* msg ) */
        BCI_OnVisSignal,     /* ( void* refdata, const char* visID, int channels, int elements, float* data ) */
        BCI_OnVisBitmap,     /* ( void* refdata, const char* visID, int width, int height, unsigned short* data ) */
        BCI_OnVisVideoFrame, /* ( void* refdata, const char* visID, int width, int height, unsigned int* data ) */

        BCI_OnUnknownCommand, /* int ( void* refdata, const char* command, const char** error ) 
                                 the function may allocate an error string using BCI_AllocateString() and assign it to the error output variable, 
                                 it will be deallocated by the caller */
        BCI_OnScriptError,    /* ( void* refdata, const char* msg ) */
        BCI_OnScriptHelp,     /* ( void* refdata, const char** help ) */

        BCI_OnQuitRequest, /* ( void* refdata, const char** message ) executed when a script executes the QUIT command */

        BCI_OnNextFilePart, /* ( void* refdata, int part ) executed when the FilePart event state is incremented, argument is new file part */

        BCI_NumEvents,
    };

    /* Callback result codes. */
    enum BCI_Result
    {
        BCI_NotHandled = 0,
        BCI_Handled = 1,
    };

    /* Callback function pointer type (callbacks must be declared STDCALL) */
    typedef int(STDCALL *BCI_Function)();

    /*
    function:  BCI_SetCallback
    purpose:   Register a callback function. To clear a callback function,
               specify NULL as a function pointer.
    arguments: Event ID, callback function pointer, data pointer.
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_SetCallback(long, BCI_Function, void *);

    /*
    function:  BCI_SetExternalCallback
    purpose:   Register a callback function to be executed in an external thread on
               execution of BCI_CheckPendingCallback(). To clear a callback function,
               specify NULL as a function pointer.
    arguments: Event ID, callback function pointer, data pointer.
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_SetExternalCallback(long, BCI_Function, void *);

    /*
    function:  BCI_CheckPendingCallback
    purpose:   Call this function regularly from within an external thread you want
               external callbacks to run in.
    arguments: None.
    returns:   1 if successful, 0 otherwise.
    */
    DLLEXPORT int STDCALL BCI_CheckPendingCallback();

    /*
    function:  BCI_GetCallbackFunction
    purpose:   Get a registered callback function pointer.
    arguments: Event ID.
    returns:   Callback function pointer, or NULL if no callback function has been
               registered.
    */
    DLLEXPORT BCI_Function STDCALL BCI_GetCallbackFunction(long);

    /*
    function:  BCI_GetCallbackData
    purpose:   Get registered callback data. Callback data is the first argument
               to callback functions, and specified when calling
               SetCallback/SetExternalCallback.
    arguments: Event ID.
    returns:   Callback data, or NULL if no callback data has been
               registered.
    */
    DLLEXPORT void *STDCALL BCI_GetCallbackData(long);

    /*
    function:  BCI_GetCallbackIsExternal
    purpose:   Get information how callback was registered.
    arguments: Event ID.
    returns:   Returns 1 if the function was registered with BCI_SetExternalCallback(),
               and 0 if it was registered with BCI_SetCallback(), or when no callback
               was registered.
    */
    DLLEXPORT int STDCALL BCI_GetCallbackIsExternal(long);

    /*
    function:  BCI_AddWatch
    purpose:   Create a watch consisting of one or more expressions. An event is triggered whenever
               one of the expressions changes its value. Expressions are evaluated as described for
               the EVALUATE EXPRESSION scripting command.
               You may associate a callback with the created watch by specifying its watch ID as an
               event ID to any of the callback registration functions.
               The callback function may contain more than a single change that triggered the watch.
               After the refdata argument, the number of changes is provided, followed with an array
               of const char*, pointing to a string representations of the change's timestamp in seconds,
               followed with all current expression values, separated by tab characters, and 
               terminated with a Windows newline sequence (CRLF).
               The callback function signature is
                 ( void* refdata, int count, const char* const* arrayOfValues )
    arguments: A string containing a list of expressions, separated by tab characters.
    returns:   A watch ID if successful, BCI_None otherwise. The function will fail if the list of expressions
               contains an invalid expression.
    */
    DLLEXPORT long STDCALL BCI_AddWatch(const char *);

    /*
    function:  BCI_SetWatchDecimation
    purpose:   Set decimation for a watch. By default, decimation is 1, and expressions will be evaluated
               for each sample in a data block. For higher values of decimation, every n-th sample will be evaluated.
               If decimation is -1, automatic decimation will occur at a cutoff frequency of 1kHz.
    arguments: A watch ID, and a decimation value.
    returns:   1 if successful, 0 if no watch was registered with the given ID.
    */
    DLLEXPORT int STDCALL BCI_SetWatchDecimation(long, int);

    /*
    function:  BCI_RemoveWatch
    purpose:   Removes a watch that was created using BCI_AddWatch(), and unregisters its associated callback function.
    arguments: Watch ID as returned by BCI_AddWatch.
    returns:   1 if successful, 0 if no watch was registered with the given ID.
    */
    DLLEXPORT int STDCALL BCI_RemoveWatch(long);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BCI_OPERATOR_LIB_H */
