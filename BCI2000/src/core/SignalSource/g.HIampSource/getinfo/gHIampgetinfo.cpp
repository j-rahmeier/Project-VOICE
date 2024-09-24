///////////////////////////////////////////////////////////////////////////////////
// $Id: gHIampgetinfo.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: belsten@neurotechcenter.org
// Description: A command line utility to retrieve information about possible
//   filter configurations of a physically connected g.HIamp.
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
///////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#pragma hdrstop
#define MAX_HIAMPS 32
#include "gHIamp.imports.h"
#include <iostream>
#include <iomanip> 

//---------------------------------------------------------------------------


void print_info();

int main(int, char *[])
{
  std::cout << "*******************************************\r\n"
            << "BCI2000 Information Tool for g.HIamp\r\n"
            << "*******************************************\r\n"
            << "(C)2021 Alexander Belsten\r\n"
            << "        Washington University\r\n"
            << "        Dept. of Neurosurgery\r\n"
            << "        St. Louis, MO, USA\r\n"
            << "*******************************************\r\n";

  print_info();

  return 0;
  }
//---------------------------------------------------------------------------

void print_info()
{
  std::cout << "\r\nFilter types:\r\n"
    << "type 0 = Chebyshev\r\n"
    << "type 1 = Butterworth\r\n"
    << "type 2 = Bessel\r\n";
  HANDLE hdev;

  int firstamp = -1;
  // go through all possible USB connectors to find out what we have connected
  for (int cur_amp = 0; cur_amp < MAX_HIAMPS; cur_amp++)
  {
    hdev = GT_OpenDevice (cur_amp);
    if (hdev)
    {
      char buf[1000];
      GT_GetSerial (hdev, (LPSTR)buf, 1000);
      std::cout << "Amp found at USB address " << cur_amp << " (S/N: " << buf << ")\r\n";
      if (firstamp < 0)
        firstamp = cur_amp;
      GT_CloseDevice (&hdev);
    }
  }

  // no amp detected
  if (firstamp < 0)
  {
    std::cout << "No g.HIamp detected. Aborting ...\r\n";
    return;
  }

  std::cout << "Printing info for first amp (USB address " << firstamp << ")\r\n";
  hdev = GT_OpenDevice(firstamp);
  if (hdev)
  {
    // get filter settings
    int nof;
    FILT *filt;
    GT_GetNumberOfFilter(&nof);
    filt = new _FILT[nof];
    std::cout << "\r\nAvailable bandpass filters\r\n"
              << "======================================\r\n" 
              << "num | hpfr | lpfreq |  sfr | or | type\r\n" 
              << "======================================\r\n";
    for (int no_filt = 0; no_filt < nof; no_filt++)
    {
      GT_GetFilterSpec(filt);
      std::cout << std::fixed;
      std::cout << std::setw (3) << std::setprecision (2) << no_filt             << " | "
                                                          << filt[no_filt].fu    << " | "
                << std::setw (6) << std::setprecision (1) << filt[no_filt].fo    << " | "
                << std::setw (4) << std::setprecision (0) << filt[no_filt].fs    << " | "
                << std::setw (2)                          << filt[no_filt].order << " | "
                << std::setw (1)                          << filt[no_filt].type  << "\r\n"; 
    }
    delete filt;
    // get notch filter settings
    GT_GetNumberOfNotch(&nof);
    filt = new _FILT[nof];
    std::cout << "\r\nAvailable notch filters\r\n"
              << "=======================================\r\n"
              << "num | hpfr  | lpfreq |  sfr | or | type\r\n"
              << "=======================================\r\n";
    for (int no_filt = 0; no_filt < nof; no_filt++)
    {
      GT_GetNotchSpec(filt);
      std::cout << std::fixed;
      std::cout << std::setw (3) << std::setprecision (2) << no_filt             << " | "
                                                          << filt[no_filt].fu    << " | "
                << std::setw (6) << std::setprecision (1) << filt[no_filt].fo    << " | "
                << std::setw (4) << std::setprecision (0) << filt[no_filt].fs    << " | "
                << std::setw (2)                          << filt[no_filt].order << " | "
                << std::setw (1)                          << filt[no_filt].type  << "\r\n";
    }
    delete filt;

    GT_CloseDevice(&hdev);
  }
  std::system("pause");
}
