BCI2000RemoteNET
===
Control BCI2000 from .NET
---

[BCI2000](https://www.BCI2000.org)

Description
---
BCI2000 is a neuroscience research toolkit which simplifies the process of performing experiments. BCI2000RemoteNET provides functionality for controlling BCI2000 from .NET programs.

Commmunication
---
BCI2000Remote communicates with BCI2000 over tcp by sending [Operator Scripting Commands](https://www.bci2000.org/mediawiki/index.php/User_Reference:Operator_Module_Scripting) and receiving and processing responses.

Versions
---

BCI2000RemoteNET 1.0.0 is a full rewrite of BCI2000RemoteNET for greater performance and reliability. It brings major interface changes, further documentation coming soon.

For compatibility, BCI2000RemoteNET is split into two versions, targeting .NET 8.0 and .NET Standard 2.1. These are located on the `main` branch and `standard2.1` branch, respectively. 
This is primarily due to the fact that Unity, which is the main usecase of BCI2000RemoteNET, does not yet support newer .NET versions as a result of it using the old Mono runtime.
Unity has plans for moving to the .NET Core runtime, at which point the .NET Standard version of BCI2000RemoteNET will not be needed, but until then it will be maintained alongside the .NET 8.0 version.
The two versions are nearly functionally equivalent except for the `Execute<T>()` method. .NET Standard 2.1 does not support generic parsing via `IParsable`, so `Execute<T>` is not available. 
Instead, the methods `ExecuteString, ExecuteUInt32, ExecuteDouble, and ExecuteBool` are provided, which return their indicated type. 
