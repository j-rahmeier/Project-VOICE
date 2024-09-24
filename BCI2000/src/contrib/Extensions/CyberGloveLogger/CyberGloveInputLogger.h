//////////////////////////////////////////////////////////////////////
// $Id: CyberGloveInputLogger.h 7462 2023-06-30 14:27:11Z mellinger $
// Author: Sidhant Sharma (sidhant.sharma@gmail.com)
// Description: BCI2000 CyberGlove Input Logger v1.0
// 
// Version History
// 
//  03/21/2014 ssharma: Initial version v1.0
//                      Support for 18, 22 sensor gloves
//                      Maximum 2 gloves supported
// 
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
///////////////////////////////////////////////////////////////////////

#ifndef CyberGloveInputLoggerH
#define CyberGloveInputLoggerH

#include "GenericFilter.h"
#include "Environment.h"
#include "Thread.h"

#include <vhtBase.h>

#include <vhtCyberGlove.h>
#include <vhtIOConn.h>
#include <vhtBaseException.h>

#include <list>
#include <string>

typedef vhtIOConn*        (*CGSGETDEFAULT)                   (const char    *key);
typedef bool              (*CGSCONNECT)                       ();
typedef bool              (*CGSDISCONNECT)                    ();
typedef int              (*CGSGETCONNECTSTATUS)               ();
typedef double           (*CGSGETRAWDATA)                   (GHM::HandParts handPart, GHM::Joints aJoint);

const int MAX_SENSORS = 22;
const int SENSOR_PRECISION = 8;


class CyberGloveInputLogger;

class CyberGloveThread : public Thread
{
	public:
		            CyberGloveThread(const CyberGloveInputLogger *logger, bool deriv, int nOfGloves, int iGlove);
		             ~CyberGloveThread();
		int         OnExecute() override;

		std::string GetError();
		int         GetHandedness();
		void        SetIndex(int i);
		void        Cleanup();


	private:
		const CyberGloveInputLogger      *mpLogger;
		vhtIOConn						  *gloveDict;
		vhtCyberGlove					  *glove;
		std::stringstream                 m_err;
		int                         m_hand;
		int                         m_sensors;
		int                         m_index;
		bool                        m_deriv;
		double					mPreviousReading[23];
	    double					mPreviousOutput[23];
	    //uint8_t				mPreviousReading[23];
	    //uint8_t				mPreviousOutput[23];

		
};
typedef std::list<CyberGloveThread*> CyberGloveThreadList;
void DisplayMessage(const char *msg, vhtBaseException *e);
int checkKey(HKEY tree, const char *folder, char *key);

class CyberGloveInputLogger : public EnvironmentExtension
{
	public:
		CyberGloveInputLogger();
		~CyberGloveInputLogger();
		void Publish() override;
		void Preflight() const override;
		void Initialize() override;
		void StartRun() override;
		void StopRun() override;
		void Halt() override;

		void OpenInterface();
		int  CountGloves() const;
		void GetGloves(CyberGloveThreadList & available) const;
		void AssignGloves(CyberGloveThreadList & assigned) const;
		void EmptyList(CyberGloveThreadList & gloves) const;

		

		

	private:

		HINSTANCE						   m_hinstLib;
		CGSGETDEFAULT                     cgsGetDefaultCall;
		CGSCONNECT                        cgsConnectCall;
		CGSDISCONNECT                     cgsDisconnectCall;
		CGSGETCONNECTSTATUS               cgsGetConnectStatusCall;
		CGSGETRAWDATA                     cgsGetRawDataCall;

		bool                        m_enabled;
		bool                        m_deriv;
		CyberGloveThreadList         mGloves;

		

	friend class CyberGloveThread;
};

#endif // CyberGloveInputLoggerH
