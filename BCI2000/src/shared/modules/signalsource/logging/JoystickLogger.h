/////////////////////////////////////////////////////////////////////////////
// $Id: JoystickLogger.h 7526 2023-08-14 17:08:31Z mmarcus $
// Authors: pbrunner@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The joystick logger supports joysticks with up to 3 axes
//   and 4 buttons that are interfaced with windows via the USB port.
//
// Parameterization:
//   Joystick logging is enabled from the command line adding
//   --LogJoystick=1
//   as a command line option.
//
// State Variables:
//   Each position state is ranging from -16348 to +16347 with a resting
//   position at 0. Each button state is either 1 when pressed or 0 when
//   not pressed.
//   JoystickXpos
//   JoystickYpos
//   JoystickZpos
//   JoystickButtons1
//   JoystickButtons2
//   JoystickButtons3
//   JoystickButtons4
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
/////////////////////////////////////////////////////////////////////////////
#ifndef JOYSTICK_LOGGER_H
#define JOYSTICK_LOGGER_H

// clang-format off
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
// clang-format on
#else
#if __APPLE__
#import <CoreFoundation/CoreFoundation.h>
#import <IOKit/hid/IOHIDManager.h>
#import <IOKit/hid/IOHIDUsageTables.h>
#endif
#endif
#include "Environment.h"
#include "Thread.h"

class JoystickLogger : public EnvironmentExtension
{
  public:
    JoystickLogger();
    ~JoystickLogger();
    void Publish() override;
    void Preflight() const override;
    void Initialize() override;
    void StartRun() override;
    void StopRun() override;
    void Halt() override;

#if __APPLE__
	void Joystick( IOHIDElementRef, IOHIDValueRef );
#endif

  private:
    bool m_joystickenable;
#ifdef _WIN32
    JOYCAPS m_joycaps;
#endif

    class JoystickThread : public Thread
    {
      public:
#ifdef _WIN32
        JoystickThread(const JOYCAPS &);
        ~JoystickThread();
        int OnExecute() override;

		void GetJoyPos(int &x, int &y, int &z, int &b1, int &b2, int &b3, int &b4, int &b9, int &b10);

      private:
        JOYCAPS m_joycaps;
        int m_prevXPos, m_prevYPos, m_prevZPos, m_prevButton1, m_prevButton2, m_prevButton3, m_prevButton4, m_prevButton9, m_prevButton10;
#else
#if __APPLE__
		int OnExecute();
#endif // __APPLE__
#endif // _WIN32
    } * mpJoystickThread;
};

#endif // JOYSTICK_LOGGER_H
