# HyperscanningApplicationBase
Child class of BCI2000 ApplicationBase and BCI2000 Thread class. Implements client side of BCI2000 Hyperscanning. To be overriden by an application module that will be connected to other clients. Child class will implement inside of the `Shared` version of BCI2000 methods, i.e. `SharedPublish`, `SharedProcess`, etc. instead of `Publish`, `Process`, etc..

# Documentation

## Methods
### `void HyperscanningApplicationBase::Publish() override;`  
Overrides BCI2000 ApplicationBase Publish.  Calls `HyperscanningApplicationBase::SharedPublish()`.
#### Defined Parameters:  
##### `IPAddress` 
IP Address of server to connect to (can be set at command line)  
##### `Port`
Port on server to connect to (can be set at command line)  
##### `ParameterPath`
Path to download parameters from the server to  
##### `SharedStates` 
States to create and share with server and other clients (can be set at command line)  
##### `PreDefinedSharedStates`
States that have been created by other modules to share with server and other clients. (can be set at command line)  

#### Defined States:  
##### `<SharedStates>`
Defines all states set in `SharedStates`  
##### `ClientNumber`
Client ID given by the server. Beggining at 0, increases by 1 for each client.  

---
### `void HyperscanningApplicationBase::AutoConfig( const SignalProperties& Input ) override;`  
Overrides BCI2000 AutoConfig. Calls `HyperscanningApplicationBase::Setup()`. Calls `HyperscanningApplicationBase::SharedAutoConfig()`  

---

### `void HyperscanningApplicationBase::Setup()`
Opens socket connection to server. Waits for the server to send parameters and client number. Sends shared states list to be confirmed with the server and other clients.

---

### `void HyperscanningApplicationBase::Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;`  
Overrrides BCI2000 ApplicationBase Preflight. Checks that the client number state and each of the parameters exists. Calls `HyperscanningApplicationBase::SharedPreflight()`

---

### `void HyperscanningApplicationBase::Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;`  
Overrides BCI2000 ApplicationBase Initialize. Sets ClientNumber to the value sent by the server. Calls `HypercsanningApplicationBase::SharedInitialize()`  

---

### `void HyperscanningApplicationBase::StartRun() override;`  
Overrides BCI2000 ApplicationBase StartRun. Starts the asynchronous client loop. Calls `HyperscanningApplicationBase::SharedStartRun()`.  

---

### `void HyperscanningApplicationBase::Process( const GenericSignal& Input, GenericSignal& Output ) override;`  
Calls `HyperscanningApplicationBase::UpdateStates()`, `HyperscanningApplicationBase::SharedProcess()`, and `HyperscanningApplicationBase::UpdateMessage()`.  

---

### `void HyperscanningApplicationBase::StopRun() override;`  
Overrides BCI2000 ApplicationBase StopRun. Ends Asynchronous Client Loop. Calls `HyperscanningApplicationBase::SharedStopRun()`.  

---

### `void HyperscanningApplicationBase::Halt() override;`  
Overrides BCI2000 ApplicationBase Halt. Ends Asynchronous Client Loop. Calls `HyperscanningApplicationBase::SharedHalt()`.  

---

### `void HyperscanningApplicationBase::Resting() override;`  
Overrides BCI2000 ApplicationBase Resting. Calls `HyperscanningApplicationBase::SharedResting()`.  

---


### `virtual void HyperscanningApplicationBase::SharedPublish();`  
To be optionally overriden by child class. Called during BCI2000 Publish phase. Functions the same as if the child class was a direct child class of ApplicationBase, but with shared state functionallity. 

---


### `virtual void HyperscanningApplicationBase::SharedAutoConfig( const SignalProperties& Input );`  
To be optionally overriden by child class. Called during BCI2000 AutoConfig phase. Functions the same as if the child class was a direct child class of ApplicationBase, but with shared state functionallity. 

---


### `virtual void HyperscanningApplicationBase::SharedPreflight( const SignalProperties& Input, SignalProperties& Output ) const;`  
To be optionally overriden by child class. Called during BCI2000 AutoConfig phase. Functions the same as if the child class was a direct child class of ApplicationBase, but with shared state functionallity. 

---



### `virtual void HyperscanningApplicationBase::SharedInitialize( const SignalProperties& Input, const SignalProperties& Output );`  
To be optionally overriden by child class. Called during BCI2000 AutoConfig phase. Functions the same as if the child class was a direct child class of ApplicationBase, but with shared state functionallity. 

---



### `virtual void HyperscanningApplicationBase::SharedStartRun();`  
To be optionally overriden by child class. Called during BCI2000 StartRun phase. Functions the same as if the child class was a direct child class of ApplicationBase, but with shared state functionallity. 

---


### `virtual void HyperscanningApplicationBase::SharedProcess( const GenericSignal& Input, GenericSignal& Output );`  
To be optionally overriden by child class. Called during BCI2000 Process phase. Functions the same as if the child class was a direct child class of ApplicationBase, but with shared state functionallity. 

---



### `virtual void HyperscanningApplicationBase::SharedResting();`  
To be optionally overriden by child class. Called during BCI2000 Resting phase. Functions the same as if the child class was a direct child class of ApplicationBase, but with shared state functionallity. 

---


### `virtual void HyperscanningApplicationBase::SharedStopRun();`  
To be optionally overriden by child class. Called during BCI2000 StopRun phase. Functions the same as if the child class was a direct child class of ApplicationBase, but with shared state functionallity. 

---


### `virtual void HyperscanningApplicationBase::SharedHalt();`  
To be optionally overriden by child class. Called during BCI2000 Halt phase. Functions the same as if the child class was a direct child class of ApplicationBase, but with shared state functionallity. 

---


### `int HyperscanningApplicationBase::OnExecute() override`
Called when asynchronous thread is started. Implements client side asynchronous loop that handles communication with the server.  

---


### `void HyperscanningApplicationBase::UpdateStates()`
Updates BCI2000 version of any states that have been changed by the server.  

---


### `void HyperscanningApplicationBase::UpdateMessage()`
Updates message to send to the server based on any client state changes.  

---


### `void HyperscanningApplicationBase::Interpret( char\* message )`  
Interprets a server sent message and records the states that need to be changed in BCI2000 in `HyperscanningApplicationBase::mHasUpdated`.  
`char\* message` : Message to be interpreted.

---


### `size\_t HyperscanningApplicationBase::GetServerMessageSize()`  
Recieves the size of an upcoming message from the server.  
Returns:  
`size\_t` containing size of server message.  

---


### `void HyperscanningApplicationBase::GetServerMessage()`  
Recieves a message from the server. Outputs to `HyperscanningApplicationBase::mBuffer`.

---

## Members  

### Private  

#### `std::vector<std::string> HyperscanningApplicationBase::mSharedStates`
list of states to be shared with the server  

#### `std::vecotr<uint64_t> HyperscanningApplicationBase::mStateValues`
coresponding values of the shared states. Updated by `HyperscanningApplicationBase::Interpret()`.  

#### `std::vector<bool> HyperscanningApplicationBase::mHasUpdated`
Whether or not the coresponding shared state has been udpated by the server since the last call of `HyperscanningApplicationBase::UpdateStates()`.  

#### `std::mutex HyperscanningApplicationBase::mStateValuesMutex`
Mutex protecting previous three members.  


#### `std::string HyperscanningApplicationBase::mMessage`
Message to be sent to the server. Written to by `HyperscanningApplicationBase::UpdateMessage()`.

#### `std::mutex HyperscanningApplicationBase::mMessageMutex`
Mutex protecting `HyperscanningApplicationBase::mMessage`



#### `ClientTCPSocket HyperscanningApplicationBase::mSocket`
BCI2000 socket connection to the server  


#### `char\* HyperscanningApplicationBase::mBuffer`
Buffer containing server message.  


#### `std::string HyperscanningApplicationBase::mAddress`
Address of the server to connect to.  

#### `int HyperscanningApplicationBase::mPort`
Port on the server to connect to.  

#### `char HyperscanningApplicationBase::mClientNumber`
Client number ID of this client.  
