////////////////////////////////////////////////////////////////////////////////
// $Id: EventLoop.h 7732 2023-11-23 17:07:03Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: A thin wrapper around a Win32 event loop.
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
#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <Windows.h>

class EventLoop
{
public:
  // Run a Win32 event loop in the current thread.
  // Optionally, restrict event handling to the Window associated with the specified handle.
  static int Run(HWND = NULL);
};

#endif // EVENT_LOOP_H

