////////////////////////////////////////////////////////////////////
// $Id: Stub.h 7462 2023-06-30 14:27:11Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Declaration of stub entry point, and TLS callback.
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
////////////////////////////////////////////////////////////////////
#ifndef STUB_H
#define STUB_H

#include <Windows.h>
void WINAPI StubEntryPoint();
void WINAPI StubTlsCallback(LPVOID, DWORD, LPVOID);

#endif // STUB_H
