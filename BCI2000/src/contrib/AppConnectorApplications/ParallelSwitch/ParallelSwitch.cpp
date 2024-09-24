////////////////////////////////////////////////////////////////////////////////
// $Id: ParallelSwitch.cpp 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Date: Oct 28, 2005
// Description: A program that uses the BCI2000 AppConnector interface to
//       toggle the state of the printer port in response to zero crossings
//       of the control signal's or a given state's value.
//
//       This program uses the inpout32 library to communicate with the
//       printer port.
//       When executed with administrator privileges, it will install a driver
//       hwinterface.sys which is part of the input32 library.
//       After successful driver installation, the program will work without
//       administrator privileges as well.
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
#include <windows.h>

#include "SockStream.h"

#include <iostream>
#include <string>
#include <sstream>

typedef void ( _stdcall *oupfuncPtr )( short portaddr, short datum );

const char* const libname = "inpout32";

int
main( int argc, char* argv[] )
{
  HINSTANCE hLib = ::LoadLibrary( libname );
  oupfuncPtr oup32 = ( oupfuncPtr )::GetProcAddress( hLib, "Out32" );

  if( hLib == NULL )
  {
    std::cerr << "Could not load library " << libname
              << ", aborting." << std::endl;
    return -1;
  }
  const char* address = "localhost:20320",
            * valuename = "Signal(0,0)";
  bool quiet = false,
       printHelp = false;
  int hwaddress = 0x378;

  int i = 1;
  while( i < argc )
  {
    std::string arg = argv[ i ];
    if( arg == "--help" || arg == "-h" )
      printHelp = true;
    else if( arg == "--valuename" )
      valuename = argv[ ++i ];
    else if( arg == "--quiet" )
      quiet = true;
    else if( arg == "--hwaddress" )
    {
      std::string value = argv[ ++i ];
      std::istringstream iss( value );
      if( !( iss >> std::hex >> hwaddress ) )
        printHelp = true;
    }
    else if( arg.find_first_of( "/-?" ) == 0 )
      printHelp = true;
    else
      address = argv[ i ];
    ++i;
  }

  if( printHelp )
  {
    std::cout << "Usage: ParallelSwitch <option> <ipaddress:port>\n"
              << " Options are:\n"
              << "\t--valuename <name>      BCI2000 state name to listen to,\n"
              << "\t                        defaults to \"Signal(0,0)\"\n"
              << "\t--quiet                 Don't display state changes\n"
              << "\t--hwaddress <hex value> Use given LPT port base address\n"
              << "\t--help                  Show this help\n"
              << "Typing <ctrl>-c will quit the program.\n";
    return 0;
  }
  if( hwaddress < 0x200 || hwaddress >= 0x400 )
  {
    std::cerr << "Will not use 0x" << std::hex << hwaddress
              << " as hardware address for safety reasons."
              << std::endl;
    return -1;
  }
  else
    std::cout << "Using 0x" << std::hex << hwaddress
              << " as hardware address." << std::endl;

  oup32( hwaddress, 0x00 ); // If this is the first call to a function from
                         // inpout32.dll, it will install the associated
                         // kernel driver provided the program has administrative
                         // privileges.

  receiving_udpsocket socket( address );
  sockstream input( socket );
  if( !input.is_open() )
    std::cerr << "Could not open " << address
              << "." << std::endl;

  std::string line;
  bool previousValueGreaterZero = false;
  while( std::getline( input, line ) )
  {
    std::istringstream linestream( line );
    std::string name;
    float value;
    linestream >> name >> value;
    if( name == valuename )
    {
      bool currentValueGreaterZero = ( value > 0 );
      if( currentValueGreaterZero != previousValueGreaterZero )
      {
        oup32( hwaddress, currentValueGreaterZero ? 0xff : 0x00 );
        if( !quiet )
        {
          std::cout << ( currentValueGreaterZero ? "^" : "v" );
          std::cout.flush();
        }
      }
      previousValueGreaterZero = currentValueGreaterZero;
    }
  }
  return 0;
}
