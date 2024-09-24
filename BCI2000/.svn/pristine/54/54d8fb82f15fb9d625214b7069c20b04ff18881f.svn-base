////////////////////////////////////////////////////////////////////////////////
// Authors: Will Engelhardt engelhardt@neurotechcenter.org
// Description: PegasusAstroFilter implementation
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

#include "PegasusAstroFilter.h"
#include "BCIStream.h"

RegisterFilter(PegasusAstroFilter, 2.C);


PegasusAstroFilter::PegasusAstroFilter() :
	mpDevice(nullptr)
{
}

PegasusAstroFilter::~PegasusAstroFilter()
{
	Halt();
	delete mpDevice;
}

void
PegasusAstroFilter::Publish()
{
	BEGIN_PARAMETER_DEFINITIONS

		//nothing is processed if false
		"Filtering:PegasusAstroFilter int EnableUSBHub= 0 0 0 1 // (boolean) ",
		//defne active COM Port
		"Filtering:PegasusAstroFilter string COMPort= COM1 % % % // COM port the USB Hub is connected to",
		//define on/off expressions for each USB
		"Filtering:PegasusAstroFilter:PegasusAstroFilter matrix ToggleUSBs= "
		"{ USB1 USB2 USB3 USB4 USB5 USB6 } "          // column labels
		"{ Expression } " // row labels
		"StimulusCode==1 StimulusCode==2 StimulusCode==3 "
		"StimulusCode==4 StimulusCode==5 StimulusCode==6 " //Expressions that turn off USBs

		END_PARAMETER_DEFINITIONS


		BEGIN_STATE_DEFINITIONS
		//Each USB port has a state variable
		"Port1 1 1 0 0",
		"Port2 1 1 0 0",
		"Port3 1 1 0 0",
		"Port4 1 1 0 0",
		"Port5 1 1 0 0",
		"Port6 1 1 0 0"

		END_STATE_DEFINITIONS
}

void
PegasusAstroFilter::Preflight(const SignalProperties& Input, SignalProperties& Output) const
{
	//pass through original signal
	Output = Input;

	if (!(bool)OptionalParameter("EnableUSBHub", false))
		return;

	//Initialize states and signals
	GenericSignal preflightSignals(Input);
	Parameter("COMPort");
	for (int i = 0; i < 6; i++)
		State("Port" + std::to_string(i + 1));

	//check exactly 1 column exists
	if (Parameter("ToggleUSBs")->NumColumns() != 1)
		bcierr << "PegasusAstroFilter: Each USB can only have 1 expression in ToggleUSBs matrix";

	//check exactly 6 rows exist
	if (Parameter("ToggleUSBs")->NumRows() > 6)
		bcierr << "PegasusAstroFilter: Too many rows in ToggleUSBs matrix, limit to 6";
	else if (Parameter("ToggleUSBs")->NumRows() < 6)
		bcierr << "PegasusAstroFilter: 6 expressions must be defined in ToggleUSBs matrix";

	// check that expressions are valid
	for (int i = 0; i < 6; i++) {
		Expression switchExpression = Parameter("ToggleUSBs")(i, 0);
		switchExpression.Compile();
		switchExpression.Evaluate(&preflightSignals);
	}
}


void
PegasusAstroFilter::Initialize(const SignalProperties& Input, const SignalProperties& Output)
{
	mEnable = (bool)OptionalParameter("EnableUSBHub", false);
	if (!mEnable)
		return;

	//Connect to device
	//Check to see if COM port changed or it hasn't been initialized
	if ((Parameter("COMPort").ToString() != mPastCOM) || (mpDevice == nullptr))
	{
		//if COM changed, delete past obj to make a new one
		if (mpDevice != nullptr)
		{
			delete mpDevice;
			mpDevice = nullptr;
		}
		//save COM port to check for future change
		mPastCOM = Parameter("COMPort").ToString();
		mpDevice = new PegasusAstroUSB(mPastCOM);
	}
	if (!(mpDevice->isConnected()))
	{
		bcierr << "PegasusAstroFilter error: Unable to connect to device at " << Parameter("COMPort").ToString()
			<< ". Check that device is plugged in and present in Device Manager";
	}

	//initialize every USB
	mCounter = 0;
	for (int i = 0; i < 6; i++)
	{
		//extract expression and convert to string to read it
		mTemp = (Expression)Parameter("ToggleUSBs")(i, 0);
		mTempString = mTemp.AsString();

		//check validity of expressions
		//loop through each character for any input
		mEmptyParam = true;
		for (int j = 0; j < mTempString.length(); j++)
		{
			if (!isspace(mTempString[j]) && (mTempString[j] != '\"'))
			{
				//will happen if expression has input
				mEmptyParam = false;
				break;
			}
		}

		//default is to keep USBs on, will happen with no expression
		if (mEmptyParam)
			bciwarn << "PegasusAstroFilter: USB " << i + 1 << " will stay on during this run.";

		//if it has an expression, add it to array that will be processed
		else
		{
			mActivate[mCounter] = mTemp;
			//for convenience in keeping track of indices
			mUSBNames[mCounter] = mIndices[i];
			mCounter++;
		}
	}

	//initialize for port access
	mPortName = "Port";
}

void
PegasusAstroFilter::StartRun()
{
	if (!mEnable)
		return;

	//Make sure all states start off as on
	for (int i = 0; i < 6; i++)
	{
		mpDevice->turnOn(mIndices[i]);
		mPastStatus[i] = true;
		State("Port" + std::to_string(i + 1)) = 1;
	}
}


void
PegasusAstroFilter::Process(const GenericSignal& Input, GenericSignal& Output)
{
	Output = Input;
	if (!mEnable)
		return;

	//only loop through USBs that had valid expressions
	for (int i = 0; i < mCounter; i++)
	{
		//will only turn off if it matches the expression and used to be on
		if (mActivate[i].Evaluate(&Input) && mPastStatus[(mUSBNames[i] - '0') - 1])
		{
			//add USB number to port name
			mPortName += mUSBNames[i];
			//turn off respective USB
			mpDevice->turnOff(mUSBNames[i]);
			bciwarn << "PegasusAstroFilter: Turning off USB " << mUSBNames[i];
			//Remembers that it got turned off for future toggling
			mPastStatus[(mUSBNames[i] - '0') - 1] = false;
			State(mPortName) = 0;
			//Reset port name back to base
			mPortName = "Port";
		}
		//does the opposite of the above if statement
		else if (!mActivate[i].Evaluate(&Input) && !mPastStatus[(mUSBNames[i] - '0') - 1])
		{
			mPortName += mUSBNames[i];
			mpDevice->turnOn(mUSBNames[i]);
			bciwarn << "PegasusAstroFilter: Turning on USB " << mUSBNames[i];
			mPastStatus[(mUSBNames[i] - '0') - 1] = true;
			State(mPortName) = 1;
			mPortName = "Port";
		}
	}
}

void
PegasusAstroFilter::StopRun()
{
	if (!mEnable)
		return;
	
	//loop thru any USBS with valid expressions to make sure all are on
	for (int i = 0; i < mCounter; i++)
		if (!mPastStatus[(mUSBNames[i] - '0') - 1])
		{
			mPortName += mUSBNames[i];
			mpDevice->turnOn(mUSBNames[i]);
			mPastStatus[(mUSBNames[i] - '0') - 1] = true;
			State(mPortName) = 1;
			mPortName = "Port";
		}
}

void
PegasusAstroFilter::Halt()
{
}

