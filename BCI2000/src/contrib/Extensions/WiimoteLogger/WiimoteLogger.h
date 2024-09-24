/////////////////////////////////////////////////////////////////////////////
// $Id: WiimoteLogger.h 7195 2023-02-07 18:31:03Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: The wiimote filter supports wiimotes that are interfaced
// with windows via bluetooth.  This class has no support for extensions atm.
//
// Parameterization:
//   Wii Remote logging is enabled from the command line adding
//   --LogWiimote=1
//   as a command line option.
//   Wii Remote Infrared logging is enabled from the command line adding
//   --LogWiimoteIR=1
//   as a command line option.
//
// State Variables:   (Note: # refers to wiimote number)
//   Wiimote#AccelX   Each Accelleration Value Ranges From 0 to 65535.
//   Wiimote#AccelY   RAW Data (No gravity normalization)
//   Wiimote#AccelZ
//   Wiimote#IRX      IR Values from 0 to 65535
//   Wiimote#IRY      (65535,65535) is upper left, (0,0) is lower right
//                    NOTE: If IR leaves camera view, state holds current value
//                    until found again.  IR can see up to 4 dots and reports
//                    system center. X and Y vary with wiimote orientation!
//   Wiimote#ButtonA  1 = On, 0 = Off (applies to all buttons)
//   Wiimote#ButtonB
//   Wiimote#ButtonMinus
//   Wiimote#ButtonPlus
//   Wiimote#ButtonHome
//   Wiimote#Button1
//   Wiimote#Button2
//   Wiimote#ButtonUp
//   Wiimote#ButtonDown
//   Wiimote#ButtonLeft
//   Wiimote#ButtonRight
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
#ifndef WIIMOTE_LOGGER_H
#define WIIMOTE_LOGGER_H

#include "Environment.h"
#include "Thread.h"
#include <wiimote.h>
#include <wiimote_state.h>

//Logging more than 4 wiimotes at a time can get intense
const unsigned int MAX_WIIMOTES = 4;


#define WIIMOTE_MAX_STATENAME_LENGTH 50
class WiimoteReading {
	public:
		WiimoteReading(unsigned int index, const char *name, unsigned int bits);
		const char * GetName(void);
		const char * GetDefinition(void);
		void SetDeriv(bool setting);
		unsigned short Read(unsigned int index, wiimote *W);
		unsigned short GetIRCenter(wiimote *W, char dim);
		virtual unsigned short _read(wiimote *W) = 0;
	private:
		unsigned int mIndex;
		unsigned int mBits;
		bool mDeriv;
		unsigned short mPreviousReading;
		unsigned short mPreviousOutput;
		char mName[WIIMOTE_MAX_STATENAME_LENGTH+1];
		char mDefinition[WIIMOTE_MAX_STATENAME_LENGTH+10];
};
typedef std::vector<WiimoteReading *> WiimoteReadingList;
#define AddNewReading(name,index) AddReading((WiimoteReading*)(new name(index)))
#define DefineReading(name, bits, expr) \
	class name : public WiimoteReading { \
		public: \
			name(unsigned int index) : WiimoteReading(index, #name, bits) {} \
			unsigned short _read(wiimote *W){return (unsigned int)(expr);} \
	}


class WiimoteLogger : public EnvironmentExtension
{
 public:
		  WiimoteLogger();
  virtual ~WiimoteLogger();
  virtual void Publish();
  virtual void Preflight() const;
  virtual void Initialize();
  virtual void StartRun();
  virtual void StopRun();
  virtual void Halt();

  int GetNumWiimotes() const;
  WiimoteReading * AddReading(WiimoteReading *r);

 private:
  WiimoteReadingList mReadingList;
  bool         m_wiimoteEnable;
  bool	       m_wiimoteIREnable;
  wiimote      *m_wiimotes[MAX_WIIMOTES];
  unsigned int m_numFound;


  class WiimoteThread : public Thread
  {
   public:
			WiimoteThread( wiimote *inWiimote, bool ir, unsigned int i, WiimoteReadingList *pReadingList);
	virtual ~WiimoteThread();
	virtual int OnExecute() override;

   private:
   WiimoteReadingList *mpReadingList;
   wiimote      *m_wiimote;
   bool			m_ir;
   unsigned int m_wiimoteNum;

  } *mpWiimoteThread[MAX_WIIMOTES];
};

#endif // WIIMOTE_LOGGER_H





