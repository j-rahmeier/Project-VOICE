////////////////////////////////////////////////////////////////////////////////
// Authors: Robbie@DESKTOP-U5JGQCU.wucon.wustl.edu Wed Jul  5 12:18:50 2023
// Description: KeysightTestLogger header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_KEYSIGHTGENERATOR_H  // makes sure this header is not included more than once
#define INCLUDED_KEYSIGHTGENERATOR_H

#include "Environment.h"
#include <atomic>
#include "visa.h"

// Include any library headers specific to your hardware here:
//#include "KeysightTest.h"
void verifyCommand(ViSession session, int errorCode, int errorString_size, char* errorString)
{
	viQueryf(session, ":SYSTem:ERRor?\n", "%d,\"%#[^\"]\"", &errorCode, &errorString_size, errorString);
	if (errorCode != 0) {
		bcierr << "Error: " << errorString << ", code: " << errorCode;
	}
}
const char* toChar(std::string str)
{
	return str.c_str();
}
class KeysightTestLogger : public EnvironmentExtension
{
public:
	KeysightTestLogger();
	~KeysightTestLogger();
	void Publish() override;
	void Preflight() const override;
	void Initialize() override;
	void Process() override;
	void StopRun() override;
	void Halt() override;


private:
	// Use this space to declare any KeysightTestLogger-specific methods and member variables you'll need.
	// Note that you need to deal with multithreading, so make sure that variables used across
	// threads are of type std::atomic, or use a mutex.
	  //ViSession rm;
	ViSession session;
	ViSession rm;
	char idn[1024];
	int idnSize = 1024;
	float* wave2;
	float* wave1;
	int switch1 = 0;
	int switch2 = 0;
	//int state1 = 0;
	//int state2 = 0;

	int errorCode = 0;
	char errorString[255];
	int errorString_size = 255;
	// If you can register a callback function to asynchronously receive input or hardware data,
	// declare it here as static.
	static void KeysightTestCallback(int position, void* userData);
	// A corresponding member function is called from the implementation of the static function, 
	// and allows us to access our own member variables directly (this is to improve code readability).
	void OnKeysightTestPosition(int position);
	void loadWave(ViSession session, float* data, int channel, int size, const char* name);
	bool detectOnTrigger1();
	bool detectOnTrigger2();
	bool detectOffTrigger1();
	bool detectOffTrigger2();
	void InitializeAndVerify(ViSession session, int errorCode, int errorString_size, char* errorString, std::string ch);
	void ArbSettings(std::string ch);
	void DefaultSettings(int func, int amfunc, int fmfunc, int pmfunc, int pwmfunc, int prbsdata);
	void DefaultSettings2(int func, int amfunc, int fmfunc, int pmfunc, int pwmfunc, int prbsdata);
};

#endif // INCLUDED_KEYSIGHTTESTLOGGER_H
