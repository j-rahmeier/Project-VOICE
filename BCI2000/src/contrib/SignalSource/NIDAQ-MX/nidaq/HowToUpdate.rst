The ``*.imports.*`` files were generated/updated as follows (note that National Instruments may install
some things in "Program Files" and some other things in "Program Files (x86)"---I found the header file
only in the latter)::

	cd src\contrib\SignalSource\NIDAQ-MX\nidaq
	copy "C:\Program Files (x86)\National Instruments\NI-DAQ\DAQmx ANSI C Dev\include\NIDAQmx.h" .\
	..\..\..\..\..\build\CMakeFiles\buildutils\DylibTool\Release\DylibTool.exe "C:\Windows\System32\nicaiu.dll" NIDAQmx.h

For maintainability, various BCI2000 C++ files refer to the library as "NIDAQmx" rather than "nicaiu".
Therefore we will need to::

	move nicaiu.imports.h   NIDAQmx.imports.h  
	move nicaiu.imports.cpp NIDAQmx.imports.cpp
	edit NIDAQmx.imports.cpp

and change::

	#include "nicaiu.imports.h"

to::

	#include "NIDAQmx.imports.h"

and change::
	
	RegisterDylibWithAliases( nicaiu, "nicaiu", functionImports, notFoundMsg, notFoundURL );

to::

	RegisterDylibWithAliases( nicaiu, "NIDAQmx", functionImports, notFoundMsg, notFoundURL );
