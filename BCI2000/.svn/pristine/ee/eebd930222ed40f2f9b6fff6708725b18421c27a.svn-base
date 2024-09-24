////////////////////////////////////////////////////////////////////////////////
// Authors: Robbie@DESKTOP-U5JGQCU.wucon.wustl.edu Wed Jul  5 12:18:50 2023
// Description: KeysightTestLogger implementation
////////////////////////////////////////////////////////////////////////////////

#include "KeysightGenerator.h"
#include "BCIEvent.h"
#include "visa.h"
#include <Windows.h>
#include "Expression/Expression.h"
#if 1 // remove the code in this #if statement when you are done
// This is an example API which does nothing. It is just there to make the example
// code compile.
static void* ExampleAPIConnect() { return nullptr; }
static void ExampleAPIDisconnect(void*) {}
static void ExampleAPIRegisterCallback(void*, void (*)(int, void*), void*) {}
static void ExampleAPIUnregisterCallback(void*, void (*)(int, void*), void*) {}
#endif

// Register the extension with the framework.
Extension(KeysightTestLogger);

// C++ does not initialize simple types such as numbers, or pointers, by default.
// Rather, they will have random values.
// Take care to initialize any member variables here, so they have predictable values
// when used for the first time.
KeysightTestLogger::KeysightTestLogger() :
rm(),
wave1(),
wave2(),
session()
{
    PublishEnabler("LogKeysight");
}

KeysightTestLogger::~KeysightTestLogger()
{
    free(wave1);
    free(wave2);
    viPrintf(session, "*RST\n");
    // If you have allocated any memory with malloc(), call free() here.
    // If you have allocated any memory with new[], call delete[] here.
    // If you have created any object using new, call delete here.
    // Either kind of deallocation will silently ignore null pointers, so all
    // you need to do is to initialize those to zero in the constructor, and
    // deallocate them here.
}

void
KeysightTestLogger::Publish()
{
    if ( OptionalParameter("LogKeysight") > 0)
    {
        BEGIN_PARAMETER_DEFINITIONS
            "Keysight:Address string Address= % % % %"
            " // Address of connection to device",
            "Keysight:Channel1 int UsingCh1= 0 0 0 1 " //-------CH1----------
            " // Enable generation with ch1 (boolean)",
            "Keysight:Channel1 float Load1= 50Ohm 50Ohm 1 %"
            " // Impedance of signal from channel 1",
            "Keysight:Channel1 int Function1= 1 1 1 9"
            " // Shape of ch1 waveform: 1 Sinusoid, 2 Square, 3 Triangle, 4 Ramp, 5 Pulse, 6 Noise, 7 DC, 8 PRBS, 9 User (enumeration)",
            "Keysight:Channel1 float Phase1= 0 0 -360 360"
            " // Phase of signal (-360 to 360)",
            "Keysight:Square1 float SquareDutyCycle1= 50 50 0 100"
            " // Adjusts the % of the period that amplitude change occurs (percent)",
            "Keysight:Ramp1 float RampSymmetry1= 50 50 0 100"
            " // Adjusts the shape of the ramp waveform (increasing=100, decreasing=0) (percent)",
            "Keysight:Noise1 float NoiseBandWidth1= 100Hz 100Hz 0Hz 100000Hz"
            " // Adjusts noise bandwidth on channel 1", //Parameter("PulseDutyCycle1") Parameter("PulsePeriod1") Parameter("PulseWidth1") Parameter("PulseLead1") Parameter("PulseTrail1")
            "Keysight:Pulse1 float PulseDutyCycle1= 10 10 0 100"
            " // Pulse duty cycle (percent)",
            "Keysight:Pulse1 float PulsePeriod1= 0ms 0ms 0 1000000s"
            " // Pulse period (s)",
            "Keysight:Pulse1 float PulseWidth1= 0ms 0ms 0 1000000s"
            " // Pulse width (16ns--1,000,000s) (s)",
            "Keysight:Pulse1 float PulseLead1= 0 0 0 1000"
            " // Pulse edge time on leading edge of pulse (8.4ns-1000ns) (ns)",
            "Keysight:Pulse1 float PulseTrail1= 0 0 0 1000"
            " // Pulse edge time on trailing edge of pulse (8.4ns--1000ns) (ns)",
            "Keysight:UserFunction1 floatlist UserWaveData1= 0V % % %" //arbs
            " // Data to be used for channel 1 waveform (for \"User\" function)",
            "Keysight:PRBS1 float PRBSBitRate1= 1000 1000 1 %" //PRBS
            " // Psuedo-random binary sequence bit rate (bits/s)",
            "Keysight:PRBS1 int PRBSData1= 1 1 1 6"
            " // Sets PRBS type: 1 PN7, 2 PN9, 3 PN11, 4 PN15, 5 PN20, 6 PN23 (enumeration)",
            "Keysight:PRBS1 float PRBSTransition1= 10 10 8.4 1000" 
            " // Sets PRBS transition edge time (8.4ns-1 microsec) (ns)",
            "Keysight:Channel1 float Frequency1= 60Hz 60Hz % %" //generic
            " // Frequency for non user functions (channel 1).",
            "Keysight:Channel1 float Voltage1= 1V % % %"
            " // Amplitude of signal (channel 1).",
            "Keysight:Channel1 float VoltageOffset1= 0V % % %"
            " // Offset (channel 1).", 
            "Keysight:Channel1 float HighVoltage1= 3V % % %"
            " // Highest voltage permitted (channel 1) (leave High+LowVoltage=0 to disable).",
            "Keysight:Channel1 float LowVoltage1= -3V % % %"
            " // Lowest voltage permitted (channel 1) (leave High+LowVoltage=0 to disable).",
            "Keysight:AM1 int ModulateAmplitude1= 0 0 0 1 " //AM
            " // Enable amplitude modulation (channel 1) (boolean)",
            "Keysight:AM1 int AMFunction1= 1 1 1 8"
            " // Shape of ch1 AM waveform: 1 Sinusoid, 2 Square, 3 Triangle, 4 Ramp, 5 NRamp, 6 Noise, 7 PRBS, 8 User (enumeration)",
            "Keysight:AM1 float AMDepth1= 100 100 0 120"
            " // Set AM depth (0%-120%). (channel 1).",
            "Keysight:AM1 float AMFrequency1= 100Hz 100Hz % %"
            " // Set AM frequency (channel 1).",
            "Keysight:FM1 int ModulateFrequency1= 0 0 0 1 " //FM
            " // Enable frequency modulation (channel 1) (boolean)",
            "Keysight:FM1 int FMFunction1= 1 1 1 8"
            " // Shape of ch1 FM waveform: 1 Sinusoid, 2 Square, 3 Triangle, 4 Ramp, 5 NRamp, 6 Noise, 7 PRBS, 8 User (enumeration)",
            "Keysight:FM1 float FMDeviation1= 100Hz 100Hz % %"
            " // Set FM deviation. (channel 1).",
            "Keysight:FM1 float FMFrequency1= 100Hz 100Hz % %"
            " // Set FM frequency (channel 1).",
            "Keysight:PM1 int ModulatePhase1= 0 0 0 1 " //PM 
            " // Enable phase modulation (channel 1) (boolean)",
            "Keysight:PM1 int PMFunction1= 1 1 1 8"
            " // Shape of ch1 PM waveform: 1 Sinusoid, 2 Square, 3 Triangle, 4 Ramp, 5 NRamp, 6 Noise, 7 PRBS, 8 User (enumeration)",
            "Keysight:PM1 float PMDeviation1= 180deg 180deg -360deg 360deg"
            " // Set PM deviation (-360deg-360deg). (channel 1).",
            "Keysight:PM1 float PMFrequency1= 10Hz 10Hz % %"
            " // Set PM frequency (channel 1).",
            "Keysight:PWM1 int ModulatePulseWidth1= 0 0 0 1 " //PWM 
            " // Enable PWM modulation (channel 1) (boolean)",
            "Keysight:PWM1 int PWMFunction1= 1 1 1 8"
            " // Shape of ch1 PM waveform: 1 Sinusoid, 2 Square, 3 Triangle, 4 Ramp, 5 NRamp, 6 Noise, 7 PRBS, 8 User (enumeration)",
            "Keysight:PWM1 float PWMDeviation1= 0s 0s 0s 500000s"
            " // Set PWM deviation (0, 500,000S). (channel 1).",
            "Keysight:PWM1 float PWMDutyCycle1= 0 0 0 120"
            " // Set PWM depth (0%-50%). (channel 1).",
            "Keysight:PWM1 float PWMFrequency1= 10Hz 10Hz % %"
            " // Set PWM frequency (channel 1).",
            "Keysight:Sweep1 int Sweep1= 0 0 0 1 " //Sweep 
            " // Enable sweep modulation (channel 1) (boolean)",
            "Keysight:Sweep1 float SweepEndFrequency1= 100Hz 100Hz % %"
            " // Set sweep end frequency. (channel 1).",
            "Keysight:Sweep1 float SweepTime1= 1s 1s 0s 3600s"
            " // Set time required to complete sweep (channel 1).",
            "Keysight:Sweep1 float SweepHoldTime1= 1s 1s 0s 3600s"
            " // Set hold time for the maximum frequency of sweep (channel 1).",
            "Keysight:Sweep1 int SweepLogSpacing1= 0 0 0 1 "
            " // Uses logarithmic spacing rather than linear when enabled (boolean)",
            "Keysight:Burst1 int Burst1= 0 0 0 1 " //Burst
            " // Enable frequency modulation (channel 1) (boolean)",
            "Keysight:Burst1 int BurstCycles1= 10 10 1 %" 
            " // Number of periods generated in the burst. (channel 1).",
            "Keysight:Burst1 float BurstPeriod1= 10ms 10ms % %" 
            " // Burst time duration (channel 1).",
            "Keysight:Burst1 float BurstPhase1= 0deg 0deg -360deg 360deg" 
            " // Set burst phase offset (-360deg-360deg). (channel 1).",
            "Keysight:Channel1 string TriggerOnState1= KeyUp // Name of state which will switch (on) ch1",
            "Keysight:Channel1 int TriggerOnValue1= 32 // Value of state which will switch (on) ch1",
            "Keysight:Channel1 string TriggerOffState1= KeyUp // Name of state which will switch (off) ch1",
            "Keysight:Channel1 int TriggerOffValue1= 32 // Value of state which will switch (off) ch1",



            "Keysight:Channel2 int UsingCh2= 0 0 0 1 " //-------CH2----------
            " // Enable generation with ch2 (boolean)",
            "Keysight:Channel2 float Load2= 50Ohm 50Ohm 1 %"
            " // Impedance of signal from channel 2",
            "Keysight:Channel2 int Function2= 1 1 1 9"
            " // Shape of ch2 waveform: 1 Sinusoid, 2 Square, 3 Triangle, 4 Ramp, 5 Pulse, 6 Noise, 7 DC, 8 PRBS, 9 User (enumeration)",
            "Keysight:Square2 float SquareDutyCycle2= 50 50 0 100"
            " // Adjusts the % of the period that amplitude change occurs (percent)",
            "Keysight:Channel2 float Phase2= 0 0 -360 360"
            " // Phase of signal (-360 to 360)",
            "Keysight:Ramp2 float RampSymmetry2= 50 50 0 100"
            " // Adjusts the shape of the ramp waveform (increasing=100, decreasing=0) (percent)",
            "Keysight:Noise2 float NoiseBandWidth2= 100Hz 100Hz 0Hz 100000Hz"
            " // Adjusts noise bandwidth on channel 2", //Parameter("PulseDutyCycle1") Parameter("PulsePeriod1") Parameter("PulseWidth1") Parameter("PulseLead1") Parameter("PulseTrail1")
            "Keysight:Pulse2 float PulseDutyCycle2= 10 10 0 100"
            " // Pulse duty cycle (percent)",
            "Keysight:Pulse2 float PulsePeriod2= 0ms 0ms 0 1000000s"
            " // Pulse period (s)",
            "Keysight:Pulse2 float PulseWidth2= 0ms 0ms 0 1000000s"
            " // Pulse width (16ns--1,000,000s) (s)",
            "Keysight:Pulse2 float PulseLead2= 0 0 0 1000"
            " // Pulse edge time on leading edge of pulse (8.4ns-1000ns) (ns)",
            "Keysight:Pulse2 float PulseTrail2= 0 0 0 1000"
            " // Pulse edge time on trailing edge of pulse (8.4ns--1000ns) (ns)",
            "Keysight:UserFunction2 floatlist UserWaveData2= 0V % % %" //arbs
            " // Data to be used for channel 2 waveform (for \"User\" function)",
            "Keysight:PRBS2 float PRBSBitRate2= 1000 1000 1 %" //PRBS
            " // Psuedo-random binary sequence bit rate (bits/s)",
            "Keysight:PRBS2 int PRBSData2= 1 1 1 6"
            " // Sets PRBS type: 1 PN7, 2 PN9, 3 PN11, 4 PN15, 5 PN20, 6 PN23 (enumeration)",
            "Keysight:PRBS2 float PRBSTransition2= 10 10 8.4 1000" 
            " // Sets PRBS transition edge time (8.4ns-1 microsec) (ns)",
            "Keysight:Channel2 float Frequency2= 60Hz 60Hz % %" //generic
            " // Frequency for non user functions (channel 2).",
            "Keysight:Channel2 float Voltage2= 1V % % %"
            " // Amplitude of signal (channel 2).",
            "Keysight:Channel2 float VoltageOffset2= 0V % % %"
            " // Offset (channel 2).", 
            "Keysight:Channel2 float HighVoltage2= 3V % % %"
            " // Highest voltage permitted (channel 2) (leave High+LowVoltage=0 to disable).",
            "Keysight:Channel2 float LowVoltage2= -3V % % %"
            " // Lowest voltage permitted (channel 2) (leave High+LowVoltage=0 to disable).",
            "Keysight:AM2 int ModulateAmplitude2= 0 0 0 1 " //AM
            " // Enable amplitude modulation (channel 2) (boolean)",
            "Keysight:AM2 int AMFunction2= 1 1 1 8"
            " // Shape of ch2 AM waveform: 1 Sinusoid, 2 Square, 3 Triangle, 4 Ramp, 5 NRamp, 6 Noise, 7 PRBS, 8 User (enumeration)",
            "Keysight:AM2 float AMDepth2= 100 100 0 120"
            " // Set AM depth (0%-120%). (channel 2).",
            "Keysight:AM2 float AMFrequency2= 100Hz 100Hz % %"
            " // Set AM frequency (channel 2).",
            "Keysight:FM2 int ModulateFrequency2= 0 0 0 1 " //FM
            " // Enable frequency modulation (channel 2) (boolean)",
            "Keysight:FM2 int FMFunction2= 1 1 1 8"
            " // Shape of ch2 FM waveform: 1 Sinusoid, 2 Square, 3 Triangle, 4 Ramp, 5 NRamp, 6 Noise, 7 PRBS, 8 User (enumeration)",
            "Keysight:FM2 float FMDeviation2= 100Hz 100Hz % %"
            " // Set FM deviation. (channel 2).",
            "Keysight:FM2 float FMFrequency2= 100Hz 100Hz % %"
            " // Set FM frequency (channel 2).",
            "Keysight:PM2 int ModulatePhase2= 0 0 0 1 " //PM 
            " // Enable phase modulation (channel 2) (boolean)",
            "Keysight:PM2 int PMFunction2= 1 1 1 8"
            " // Shape of ch2 PM waveform: 1 Sinusoid, 2 Square, 3 Triangle, 4 Ramp, 5 NRamp, 6 Noise, 7 PRBS, 8 User (enumeration)",
            "Keysight:PM2 float PMDeviation2= 180deg 180deg -360deg 360deg"
            " // Set PM deviation (-360deg-360deg). (channel 2).",
            "Keysight:PM2 float PMFrequency2= 10Hz 10Hz % %"
            " // Set PM frequency (channel 2).",
            "Keysight:PWM2 int ModulatePulseWidth2= 0 0 0 1 " //PWM 
            " // Enable PWM modulation (channel 2) (boolean)",
            "Keysight:PWM2 int PWMFunction2= 1 1 1 8"
            " // Shape of ch2 PM waveform: 1 Sinusoid, 2 Square, 3 Triangle, 4 Ramp, 5 NRamp, 6 Noise, 7 PRBS, 8 User (enumeration)",
            "Keysight:PWM2 float PWMDeviation2= 0s 0s 0s 500000s"
            " // Set PWM deviation (0, 500,000S). (channel 2).",
            "Keysight:PWM2 float PWMDutyCycle2= 0 0 0 120"
            " // Set PWM depth (0%-50%). (channel 2).",
            "Keysight:PWM2 float PWMFrequency2= 10Hz 10Hz % %"
            " // Set PWM frequency (channel 2).",
            "Keysight:Sweep2 int Sweep2= 0 0 0 1 " //Sweep 
            " // Enable sweep modulation (channel 2) (boolean)",
            "Keysight:Sweep2 float SweepEndFrequency2= 100Hz 100Hz % %"
            " // Set sweep end frequency. (channel 2).",
            "Keysight:Sweep2 float SweepTime2= 1s 1s 0s 3600s"
            " // Set time required to complete sweep (channel 2).",
            "Keysight:Sweep2 float SweepHoldTime2= 1s 1s 0s 3600s"
            " // Set hold time for the maximum frequency of sweep (channel 2).",
            "Keysight:Sweep2 int SweepLogSpacing2= 0 0 0 1 "
            " // Uses logarithmic spacing rather than linear when enabled (boolean)",
            "Keysight:Burst2 int Burst2= 0 0 0 1 " //Burst
            " // Enable frequency modulation (channel 2) (boolean)",
            "Keysight:Burst2 int BurstCycles2= 10 10 1 %" 
            " // Number of periods generated in the burst. (channel 2).",
            "Keysight:Burst2 float BurstPeriod2= 10ms 10ms % %" 
            " // Burst time duration (channel 2).",
            "Keysight:Burst2 float BurstPhase2= 0deg 0deg -360deg 360deg" 
            " // Set burst phase offset (-360deg-360deg). (channel 2).",
            "Keysight:Channel2 string TriggerOnState2= KeyUp // Name of state which will switch (on) ch2",
            "Keysight:Channel2 int TriggerOnValue2= 32 // Value of state which will switch (on) ch2",
            "Keysight:Channel2 string TriggerOffState2= KeyUp // Name of state which will switch (off) ch2",
            "Keysight:Channel2 int TriggerOffValue2= 32 // Value of state which will switch (off) ch2",
            "Keysight:Sync int PhaseSync= 0 0 0 1 " //Sweep 
            " // Matches ch1/ch2 signals from their starting phase, including after triggers (boolean)",


          
            END_PARAMETER_DEFINITIONS

            BEGIN_EVENT_DEFINITIONS
            "KeysightTestPos   32 0 0 0",
            END_EVENT_DEFINITIONS
    }
}

void
KeysightTestLogger::Preflight() const
{
    if (OptionalParameter("LogKeysight") > 0)
    {
        Parameter("LogKeysight");
        Parameter("Address");
        bciwarn << "Compatibility of parameters with Keysight system will be checked during initialization";
        if (Parameter("ModulateAmplitude1") + Parameter("ModulateFrequency1") + Parameter("ModulatePhase1") + Parameter("ModulatePulseWidth1") + Parameter("Sweep1") + Parameter("Burst1") > 1)
        {
            bcierr << "Only one signal modulation feature can be enabled at a time";
        }
        Parameter("UsingCh1");
        Parameter("Load1"); Parameter("Phase1");
        Parameter("Function1"); Parameter("Voltage1"); Parameter("VoltageOffset1");
        Parameter("RampSymmetry1"); Parameter("SquareDutyCycle1"); Parameter("NoiseBandwidth1");
        Parameter("PulseDutyCycle1"); Parameter("PulsePeriod1"); Parameter("PulseWidth1"); Parameter("PulseLead1"); Parameter("PulseTrail1");
        Parameter("UserWaveData1"); Parameter("Frequency1"); Parameter("LowVoltage1"); Parameter("HighVoltage1");  Parameter("ModulateAmplitude1");  Parameter("AMFunction1"); Parameter("ModulateFrequency1");  Parameter("FMFunction1"); Parameter("FMDeviation1"); Parameter("FMFrequency1");
        Parameter("ModulatePhase1"); Parameter("PMFunction1"); Parameter("PMDeviation1"); Parameter("PMFrequency1"); Parameter("Sweep1"); Parameter("SweepEndFrequency1"); Parameter("SweepTime1"); Parameter("Burst1"); Parameter("BurstCycles1"); Parameter("BurstPeriod1"); Parameter("BurstPhase1");
        Parameter("SweepLogSpacing1");
        Parameter("SweepHoldTime1");
        Parameter("TriggerOnState1");
        Parameter("TriggerOnValue1");
        Parameter("TriggerOffState1");
        Parameter("TriggerOffValue1");
        Parameter("AMFrequency1");
        State(Parameter("TriggerOnState1"));
        State(Parameter("TriggerOffState1"));
        Parameter("PRBSData1"); Parameter("PRBSBitRate1"); Parameter("PRBSTransition1");
        Parameter("PWMFunction1"); Parameter("ModulatePulseWidth1"); Parameter("PWMDeviation1"); Parameter("PWMDutyCycle1"); Parameter("PMFrequency1");
        int numArbs1 = 0;
        int numPrbs1 = 0;
        numArbs1 = (Parameter("Function1") == 9) + (Parameter("AMFunction1") == 8 && Parameter("ModulateAmplitude1") == 1) + (Parameter("PMFunction1") == 8 && Parameter("ModulatePhase1") == 1) + (Parameter("FMFunction1") == 8 && Parameter("ModulateFrequency1") == 1) + (Parameter("PWMFunction1") == 8 && Parameter("ModulatePulseWidth1") == 1);
        numPrbs1 = (Parameter("Function1") == 8) + (Parameter("AMFunction1") == 7 && Parameter("ModulateAmplitude1") == 1) + (Parameter("PMFunction1") == 7 && Parameter("ModulatePhase1") == 1) + (Parameter("FMFunction1") == 7 && Parameter("ModulateFrequency1") == 1) + (Parameter("PWMFunction1") == 7 && Parameter("ModulatePulseWidth1") == 1);
        if (numArbs1 > 1)
        {
            bcierr << "Only one source may be configured to a user function";
        }
        if (numPrbs1 > 1)
        {
            bcierr << "Only one source/modification per channel may be configured to a user function";
        }
        if (Parameter("ModulateAmplitude2") + Parameter("ModulateFrequency2") + Parameter("ModulatePhase2") + Parameter("ModulatePulseWidth2") + Parameter("Sweep2") + Parameter("Burst2") > 1)
        {
            bcierr << "Only one source/modification per channel may be configured to a PRBS function";
        }
        Parameter("UsingCh2");
        Parameter("Load2"); Parameter("Phase2");
        Parameter("Function2"); Parameter("Voltage2"); Parameter("VoltageOffset2");
        Parameter("RampSymmetry2"); Parameter("SquareDutyCycle2"); Parameter("NoiseBandwidth2");
        Parameter("PulseDutyCycle2"); Parameter("PulsePeriod2"); Parameter("PulseWidth2"); Parameter("PulseLead2"); Parameter("PulseTrail2");
        Parameter("UserWaveData2");  Parameter("Frequency2"); Parameter("LowVoltage2"); Parameter("HighVoltage2");  Parameter("ModulateAmplitude2");  Parameter("AMFunction2"); Parameter("ModulateFrequency2");  Parameter("FMFunction2"); Parameter("FMDeviation2"); Parameter("FMFrequency2");
        Parameter("ModulatePhase2"); Parameter("PMFunction2"); Parameter("PMDeviation2"); Parameter("PMFrequency2"); Parameter("Sweep2"); Parameter("SweepEndFrequency2"); Parameter("SweepTime2"); Parameter("Burst2"); Parameter("BurstCycles2"); Parameter("BurstPeriod2"); Parameter("BurstPhase2");
        Parameter("SweepLogSpacing2");
        Parameter("SweepHoldTime2");
        Parameter("TriggerOnState2");
        Parameter("TriggerOnValue2");
        Parameter("TriggerOffState2");
        Parameter("TriggerOffValue2");
        Parameter("AMFrequency2");
        State(Parameter("TriggerOnState2"));
        State(Parameter("TriggerOffState2"));
        Parameter("PRBSData2"); Parameter("PRBSBitRate2"); Parameter("PRBSTransition2");
        Parameter("PWMFunction2"); Parameter("ModulatePulseWidth2"); Parameter("PWMDeviation2"); Parameter("PWMDutyCycle2"); Parameter("PMFrequency2");
        int numArbs2 = 0;
        int numPrbs2 = 0;
        numArbs2 = (Parameter("Function2") == 9) + (Parameter("AMFunction2") == 8 && Parameter("ModulateAmplitude2") == 1) + (Parameter("PMFunction2") == 8 && Parameter("ModulatePhase2") == 1) + (Parameter("FMFunction2") == 8 && Parameter("ModulateFrequency2") == 1) + (Parameter("PWMFunction2") == 8 && Parameter("ModulatePulseWidth2") == 1);
        numPrbs2 = (Parameter("Function2") == 8) + (Parameter("AMFunction2") == 7 && Parameter("ModulateAmplitude2") == 1) + (Parameter("PMFunction2") == 7 && Parameter("ModulatePhase2") == 1) + (Parameter("FMFunction2") == 7 && Parameter("ModulateFrequency2") == 1) + (Parameter("PWMFunction2") == 7 && Parameter("ModulatePulseWidth2") == 1);
        if (numArbs2 > 1)
        {
            bcierr << "Only one source/modification per channel may be configured to a user function";
        }
        if (numPrbs2 > 1)
        {
            bcierr << "Only one source/modification per channel may be configured to a PRBS function";
        }



        Parameter("PhaseSync");
        Parameter("SampleBlockSize");

        // Parameter("");
         // Note that the KeysightTestLogger instance itself, and its members, are read-only during
         // this phase, due to the "const" at the end of the Preflight prototype above.
         // Any methods called by Preflight must also be "const" in the same way.

         // In this example, we have no configuration parameters other than on/off, so
         // there is nothing to do here.


    }

}

void
KeysightTestLogger::Initialize()
{
    // The user has pressed "Set Config" and all Preflight checks have passed.
    // The const limitation has gone, so the KeysightTestLogger instance itself can be modified.
    // If your hardware requires a connection, it is now time to connect.
    // Note that Initialize() may be called multiple times in a row, so don't open the connection
    // if it still exists.

    if (OptionalParameter("LogKeysight") > 0) {
  
        viOpenDefaultRM(&rm);
       // viOpen(rm, "USB0::10893::36097::CN62200145::0::INSTR", VI_NULL, VI_NULL, &session); //REPLACE W PARAMETER
        std::string handle = Parameter("Address");
        viOpen(rm, handle.c_str() , VI_NULL, VI_NULL, &session);
        viQueryf(session, "*IDN?\n", "%#T", &idnSize, idn);
        viPrintf(session, "*RST\n");
        viPrintf(session, "*CLS\n");
        viPrintf(session, ":SOURce:DATA:VOLatile:CLEar\n");
        viPrintf(session, ":UNIT:ANGLe %s\n", "DEGree");
        viPrintf(session, ":SYSTem:BEEPer:STATe %@1d\n", 0);
        
        DefaultSettings(Parameter("Function1"), Parameter("AMFunction1"), Parameter("FMFunction1"), Parameter("PMFunction1"), Parameter("PWMFunction1"),Parameter("PRBSData1"));
        DefaultSettings2(Parameter("Function2"), Parameter("AMFunction2"), Parameter("FMFunction2"), Parameter("PMFunction2"), Parameter("PWMFunction2"), Parameter("PRBSData2"));
        if (Parameter("PhaseSync")) {
            viPrintf(session, ":SOURce:PHASe:SYNChronize\n");
            viPrintf(session, ":SOURce:FUNCtion:ARBitrary:SYNChronize\n");
        }
        verifyCommand(session, errorCode, errorString_size, errorString);
    }
    
}

void
KeysightTestLogger::Process()
{
    if (OptionalParameter("LogKeysight") > 0)
    {
        if (Parameter("UsingCh1") == 1) {
            if (!switch1)
            {
                if (detectOnTrigger1())
                {
                    switch1 = 1;

                    viPrintf(session, ":OUTPut1:STATe %@1d\n", 1);

                }
            }
            else
            {
                if (detectOffTrigger1())
                {
                    switch1 = 0;

                    viPrintf(session, ":OUTPut1:STATe %@1d\n", 0);

                }
            }
        }
        if (Parameter("UsingCh2") == 1) {
            if (!switch2)
            {
                if (detectOnTrigger2())
                {
                    switch2 = 1;

                    viPrintf(session, ":OUTPut2:STATe %@1d\n", 1);

                }
            }
            else
            {
                if (detectOffTrigger2())
                {
                    switch2 = 0;

                    viPrintf(session, ":OUTPut2:STATe %@1d\n", 0);

                }
            }
        }
    }
}

void
KeysightTestLogger::StopRun()
{
    if (OptionalParameter("LogKeysight") > 0)
    {
        if (Parameter("UsingCh1") == 1) {
            viPrintf(session, ":OUTPut1:STATe %@1d\n", 0);
            //state1 = 0;
            switch1 = 0;
        }
        if (Parameter("UsingCh2") == 1) {
            viPrintf(session, ":OUTPut2:STATe %@1d\n", 0);
            //state2 = 0;
            switch2 = 0;
        }
    }
    // The Running state has been set to 0, either because the user has pressed "Suspend",
    // or because the run has reached its natural end.
    // It is now time to unregister the callback because we cannot record events any more.
    //ExampleAPIUnregisterCallback(mKeysightTestHandle, &KeysightTestCallback, this);
}

void
KeysightTestLogger::Halt()
{
    // Stop any threads or other asynchronous activity.
    // If you registered any callback, it is likely to be called from a different thread, too,
    // so call StopRun() to be sure it's unregistered.
}

void
KeysightTestLogger::KeysightTestCallback(int position, void* userData)
{
    // This is the callback function. It does nothing else than forward the call
    // to our handler function, which is a member function and cannot be specified as

        // a callback function.
        static_cast<KeysightTestLogger*>(userData)->OnKeysightTestPosition(position);
    
}

void
KeysightTestLogger::OnKeysightTestPosition(int position)
{
}

void 
KeysightTestLogger::loadWave(ViSession session, float* data, int channel, int size, const char* name) {
    std::string cmd = ":SOURce"+ std::to_string(channel) + ":DATA:ARBitrary %s,%,";
    cmd = cmd + std::to_string(size) + "f\n";
    const char* format = cmd.c_str();
    viPrintf(session, format, name, data);
}

bool
KeysightTestLogger::detectOnTrigger1()
{
    std::string exprstring = Parameter("TriggerOnState1");
    Expression expr = Expression(exprstring); 
    for (unsigned int i = 0; i < Parameter("SampleBlockSize"); i++) {
        if (expr.Evaluate(0,i)== Parameter("TriggerOnValue1").ToNumber()) {
            return true;
        }
    }
    return false;
}
bool
KeysightTestLogger::detectOffTrigger1()
{
    std::string exprstring = Parameter("TriggerOffState1");
    Expression expr = Expression(exprstring);
    for (unsigned int i = 0; i < Parameter("SampleBlockSize"); i++) {
        if (expr.Evaluate(0, i) == Parameter("TriggerOffValue1").ToNumber()) {
            return true;
        }
    }
    return false;
}
bool
KeysightTestLogger::detectOnTrigger2()
{
    std::string exprstring = Parameter("TriggerOnState2");
    Expression expr = Expression(exprstring);
    for (unsigned int i = 0; i < Parameter("SampleBlockSize"); i++) {
        if (expr.Evaluate(0, i) == Parameter("TriggerOnValue2").ToNumber()) {
            return true;
        }
    }
    return false;
}

bool
KeysightTestLogger::detectOffTrigger2()
{
    std::string exprstring = Parameter("TriggerOffState2");
    Expression expr = Expression(exprstring);
    for (unsigned int i = 0; i < Parameter("SampleBlockSize"); i++) {
        if (expr.Evaluate(0, i) == Parameter("TriggerOffValue2").ToNumber()) {
            return true;
        }
    }
    return false;
}

void 
KeysightTestLogger::DefaultSettings(int func, int amfunc, int fmfunc, int pmfunc, int pwmfunc, int prbsdata)
{
    //Signal
    verifyCommand(session, errorCode, errorString_size, errorString);
    if (Parameter("UsingCh1"))
    {
        viPrintf(session, ":OUTPut1:LOAD %@3lf OHM\n", (float) Parameter("Load1"));
        if (func == 9||amfunc==8 && Parameter("ModulateAmplitude1") ||fmfunc==8 && Parameter("ModulateFrequency1") ||pmfunc==8 && Parameter("ModulatePhase1") ||pwmfunc==8 && Parameter("ModulatePulseWidth1")) { //arb
            wave1 = (float*)malloc(sizeof(float) * Parameter("UserWaveData1")->NumValues());
            int offset1 = (int) (Parameter("Phase1") * Parameter("UserWaveData1")->NumValues())/360 ;
            for (int i = 0; i < Parameter("UserWaveData1")->NumValues(); ++i) {
                wave1[i] = Parameter("UserWaveData1")((i+offset1)%(Parameter("UserWaveData1")->NumValues()));
            }
            loadWave(session, wave1, 1, Parameter("UserWaveData1")->NumValues(), "Ch1Wave");
         
            viPrintf(session, ":SOURce1:FUNCtion:ARBitrary \"%s\"\n", "Ch1Wave", 0);
            viPrintf(session, ":SOURce:FUNCtion:ARBitrary:FREQuency %@3lf HZ\n", (float)Parameter("Frequency1").InHertz());

            verifyCommand(session, errorCode, errorString_size, errorString);
        }

        if (func == 8 || amfunc == 7 && Parameter("ModulateAmplitude1") || fmfunc == 7 && Parameter("ModulateFrequency1") || pmfunc == 7 && Parameter("ModulatePhase1") || pwmfunc == 7 && Parameter("ModulatePulseWidth1")) { //PRBS

            viPrintf(session, ":SOURce1:FUNCtion:PRBS:BRATe %@3lf\n", (float) Parameter("PRBSBitRate1"));
            viPrintf(session, ":SOURce:FUNCtion:PRBS:TRANsition:BOTH %@3lf S\n", (float) Parameter("PRBSTransition1")*1e-9);
            switch (prbsdata)
            {
            case 1:
                viPrintf(session, ":SOURce1:FUNCtion:PRBS:DATA %s\n", "PN7");
                break;
            case 2:
                viPrintf(session, ":SOURce1:FUNCtion:PRBS:DATA %s\n", "PN9");
                break;
            case 3:
                viPrintf(session, ":SOURce1:FUNCtion:PRBS:DATA %s\n", "PN11");
                break;
            case 4:
                viPrintf(session, ":SOURce1:FUNCtion:PRBS:DATA %s\n", "PN15");
                break;
            case 5:
                viPrintf(session, ":SOURce1:FUNCtion:PRBS:DATA %s\n", "PN20");
                break;
            case 6:
                viPrintf(session, ":SOURce1:FUNCtion:PRBS:DATA %s\n", "PN23");
                break;
            }
            verifyCommand(session, errorCode, errorString_size, errorString);
        }

        
            switch (func)
            {
            case 1:
                viPrintf(session, ":SOURce1:FUNCtion %s\n", "SINusoid");
                break;
            case 2:
                viPrintf(session, ":SOURce1:FUNCtion %s\n", "SQUare");
                viPrintf(session, ":SOURce1:FUNCtion:SQUare:DCYCle %@3lf PCT\n", (float) Parameter("SquareDutyCycle1"));

                break;
            case 3:
                viPrintf(session, ":SOURce1:FUNCtion %s\n", "TRIangle");
                break;
            case 4:
                viPrintf(session, ":SOURce1:FUNCtion %s\n", "RAMP"); 
                viPrintf(session, ":SOURce1:FUNCtion:RAMP:SYMMetry %@3lf PCT\n", (float) Parameter("RampSymmetry1"));
                break;
            case 5:
                viPrintf(session, ":SOURce1:FUNCtion %s\n", "PULSe");
                if ((float)Parameter("PulseDutyCycle1") != 0)
                {
                    viPrintf(session, ":SOURce1:FUNCtion:PULSe:DCYCle %@3lf PCT\n", (float)Parameter("PulseDutyCycle1"));
                }
                if ((float)Parameter("PulsePeriod1") != 0)
                {
                    viPrintf(session, ":SOURce1:FUNCtion:PULSe:PERiod %@3lf S\n", (float)Parameter("PulsePeriod1").InSeconds());
                }
                if ((float)Parameter("PulseWidth1") != 0)
                {
                    viPrintf(session, ":SOURce1:FUNCtion:PULSe:WIDTh %@3lf S\n", (float)Parameter("PulseWidth1").InSeconds());
                }
                if ((float)Parameter("PulseLead1") != 0)
                {
                    viPrintf(session, ":SOURce1:FUNCtion:PULSe:TRANsition:LEADing %@3lf S\n", (float)Parameter("PulseLead1") * 1e-9);
                }
                if ((float)Parameter("PulseTrail1") != 0)
                {
                    viPrintf(session, ":SOURce1:FUNCtion:PULSe:TRANsition:TRAiling %@3lf S\n", (float)Parameter("PulseTrail1") * 1e-9);
                }
                break;
            case 6:
                viPrintf(session, ":SOURce1:FUNCtion %s\n", "NOISe");
                viPrintf(session, ":SOURce1:FUNCtion:NOISe:BANDwidth %@3lf HZ\n", (float) Parameter("NoiseBandwidth1").InHertz());

                break;
            case 7:
                viPrintf(session, ":SOURce1:FUNCtion %s\n", "DC");
                break;
            case 8:
                viPrintf(session, ":SOURce1:FUNCtion %s\n", "PRBS");
                break;
            case 9:
                viPrintf(session, ":SOURce1:FUNCtion %s\n", "ARB");
                break;
            }
            
            viPrintf(session, ":SOURce1:VOLTage %@3lf V\n", (float) Parameter("Voltage1").InVolts());
            viPrintf(session, ":SOURce1:VOLTage:OFFSet %@3lf V\n", (float)Parameter("VoltageOffset1").InVolts());
            if (func != 9 && func!=8)
            {
             viPrintf(session, ":SOURce1:FREQuency %@3lf HZ\n", (float)Parameter("Frequency1").InHertz());
            }
        verifyCommand(session, errorCode, errorString_size, errorString);
        if (Parameter("LowVoltage1") != 0 && Parameter("HighVoltage1") != 0)
        {
            viPrintf(session, ":SOURce1:VOLTage:LIMit:HIGH %@3lf V\n", (float) Parameter("HighVoltage1").InVolts());
            viPrintf(session, ":SOURce1:VOLTage:LIMit:LOW %@3lf V\n", (float) Parameter("LowVoltage1").InVolts());
            viPrintf(session, ":SOURce1:VOLTage:LIMit:STATe %@1d\n", 1);
        }
        viPrintf(session, ":SOURce1:PHASe %@3lf\n", (float) Parameter("Phase1"));
        verifyCommand(session, errorCode, errorString_size, errorString);
        //End signal

        //AM signal
        if (Parameter("ModulateAmplitude1") == 1)
        {
            viPrintf(session, ":SOURce1:AM:SOURce %s\n", "INT"); //possible modification
            viPrintf(session, ":SOURce1:AM:DSSC %@1d\n", 0); //possible modification
            viPrintf(session, ":SOURce1:AM:DEPTh %@3lf\n", (float) Parameter("AMDepth1"));
            switch (amfunc)
            {
            case 1:
                viPrintf(session, ":SOURce1:AM:INTernal:FUNCtion %s\n", "SINusoid");
                break;
            case 2:
                viPrintf(session, ":SOURce1:AM:INTernal:FUNCtion %s\n", "SQUare");
                break;
            case 3:
                viPrintf(session, ":SOURce1:AM:INTernal:FUNCtion %s\n", "TRIangle");
                break;
            case 4:
                viPrintf(session, ":SOURce1:AM:INTernal:FUNCtion %s\n", "RAMP");
                break;
            case 5:
                viPrintf(session, ":SOURce1:AM:INTernal:FUNCtion %s\n", "NRAMp");
                break;
            case 6:
                viPrintf(session, ":SOURce1:AM:INTernal:FUNCtion %s\n", "NOISe");
                break;
            case 7:
                viPrintf(session, ":SOURce1:AM:INTernal:FUNCtion %s\n", "PRBS");
                break;
            case 8:
                viPrintf(session, ":SOURce1:AM:INTernal:FUNCtion %s\n", "ARB");
                break;
            }
            viPrintf(session, ":SOURce1:AM:INTernal:FREQuency %@3lf HZ\n", (float) Parameter("AMFrequency1").InHertz());
            viPrintf(session, ":SOURce1:AM:STATe %@1d\n", 1);
            verifyCommand(session, errorCode, errorString_size, errorString);
        }
        //end AM signal

        //FM Signal
        if (Parameter("ModulateFrequency1") == 1)
        {
            viPrintf(session, ":SOURce1:FM:SOURce %s\n", "INT"); //possible modification
            viPrintf(session, ":SOURce1:FM:DEViation %@3lf HZ\n", (float) Parameter("FMDeviation1").InHertz());
            viPrintf(session, ":SOURce1:FM:INTernal:FREQuency %@3lf HZ\n", (float) Parameter("FMFrequency1").InHertz());

            switch (fmfunc)
            {
            case 1:
                viPrintf(session, ":SOURce1:FM:INTernal:FUNCtion %s\n", "SINusoid");
                break;
            case 2:
                viPrintf(session, ":SOURce1:FM:INTernal:FUNCtion %s\n", "SQUare");
                break;
            case 3:
                viPrintf(session, ":SOURce1:FM:INTernal:FUNCtion %s\n", "TRIangle");
                break;
            case 4:
                viPrintf(session, ":SOURce1:FM:INTernal:FUNCtion %s\n", "RAMP");
                break;
            case 5:
                viPrintf(session, ":SOURce1:FM:INTernal:FUNCtion %s\n", "NRAMp");
                break;
            case 6:
                viPrintf(session, ":SOURce1:FM:INTernal:FUNCtion %s\n", "NOISe");
                break;
            case 7:
                viPrintf(session, ":SOURce1:FM:INTernal:FUNCtion %s\n", "PRBS");
                break;
            case 8:
                viPrintf(session, ":SOURce1:FM:INTernal:FUNCtion %s\n", "ARB");
                break;
            }
            viPrintf(session, ":SOURce1:FM:STATe %@1d\n", 1);
            verifyCommand(session, errorCode, errorString_size, errorString);
        }
        //End FM Signal

        //PM Signal
        if (Parameter("ModulatePhase1") == 1)
        {
            viPrintf(session, ":SOURce:PM:SOURce %s\n", "INT");
            viPrintf(session, ":SOURce:PM:DEViation %@3lf\n", (float) Parameter("PMDeviation1"));
            viPrintf(session, ":SOURce:PM:INTernal:FREQuency %@3lf HZ\n", (float) Parameter("PMFrequency1").InHertz());
            switch (pmfunc)
            {
            case 1:
                viPrintf(session, ":SOURce1:PM:INTernal:FUNCtion %s\n", "SINusoid");
                break;
            case 2:
                viPrintf(session, ":SOURce1:PM:INTernal:FUNCtion %s\n", "SQUare");
                break;
            case 3:
                viPrintf(session, ":SOURce1:PM:INTernal:FUNCtion %s\n", "TRIangle");
                break;
            case 4:
                viPrintf(session, ":SOURce1:PM:INTernal:FUNCtion %s\n", "RAMP");
                break;
            case 5:
                viPrintf(session, ":SOURce1:PM:INTernal:FUNCtion %s\n", "NRAMp");
                break;
            case 6:
                viPrintf(session, ":SOURce1:PM:INTernal:FUNCtion %s\n", "NOISe");
                break;
            case 7:
                viPrintf(session, ":SOURce1:PM:INTernal:FUNCtion %s\n", "PRBS");
                break;
            case 8:
                viPrintf(session, ":SOURce1:PM:INTernal:FUNCtion %s\n", "ARB");
                break;
            }
            viPrintf(session, ":SOURce1:PM:STATe %@1d\n", 1);
            verifyCommand(session, errorCode, errorString_size, errorString);
        }
        //End PM signal

        //PWM  Parameter("PWMFunction1") Parameter("ModulatePulseWidth1") Parameter("PWMDeviation1") Parameter("PWMDutyCycle1") Parameter("PMFrequency1")
        if (Parameter("ModulatePulseWidth1") == 1) {
            switch (pwmfunc)
            {
            case 1:
                viPrintf(session, ":SOURce1:PWM:INTernal:FUNCtion %s\n", "SINusoid");
                break;
            case 2:
                viPrintf(session, ":SOURce1:PWM:INTernal:FUNCtion %s\n", "SQUare");
                break;
            case 3:
                viPrintf(session, ":SOURce1:PWM:INTernal:FUNCtion %s\n", "TRIangle");
                break;
            case 4:
                viPrintf(session, ":SOURce1:PWM:INTernal:FUNCtion %s\n", "RAMP");
                break;
            case 5:
                viPrintf(session, ":SOURce1:PWM:INTernal:FUNCtion %s\n", "NRAMp");
                break;
            case 6:
                viPrintf(session, ":SOURce1:PWM:INTernal:FUNCtion %s\n", "NOISe");
                break;
            case 7:
                viPrintf(session, ":SOURce1:PWM:INTernal:FUNCtion %s\n", "PRBS");
                break;
            case 8:
                viPrintf(session, ":SOURce1:PWM:INTernal:FUNCtion %s\n", "ARB");
                break;
            }
            viPrintf(session, ":SOURce1:PWM:DEViation %@3lf S\n", (float) Parameter("PWMDeviation1").InSeconds());
            viPrintf(session, ":SOURce1:PWM:DEViation:DCYCle %@3lf\n", (float) Parameter("PWMDutyCycle1"));
            viPrintf(session, ":SOURce1:PWM:INTernal:FREQuency %@3lf HZ\n", (float) Parameter("PMFrequency1").InHertz());
            viPrintf(session, ":SOURce1:PWM:STATe %@1d\n", 1);
            verifyCommand(session, errorCode, errorString_size, errorString);
        }
        //Sweep
        if (Parameter("Sweep1") == 1)
        {
            viPrintf(session, ":SOURce1:FREQuency:STARt %@3lf HZ\n", (float) Parameter("Frequency1").InHertz());
            viPrintf(session, ":SOURce1:FREQuency:STOP %@3lf HZ\n", (float) Parameter("SweepEndFrequency1").InHertz());
            viPrintf(session, ":SOURce1:SWEep:TIME %@3lf S\n", (float) Parameter("SweepTime1").InSeconds());
            if (Parameter("SweepLogSpacing1") == 0) {
                viPrintf(session, ":SOURce1:SWEep:SPACing %s\n", "LINear");
            }
            else {
                viPrintf(session, ":SOURce1:SWEep:SPACing %s\n", "LOGarithmic");
            }
            viPrintf(session, ":SOURce1:SWEep:HTIMe %@3lf S\n", (float) Parameter("SweepHoldTime1").InSeconds());
            viPrintf(session, ":TRIGger1:SOURce %s\n", "IMM");
            viPrintf(session, ":SOURce1:FREQuency:MODE %s\n", "SWE");
            verifyCommand(session, errorCode, errorString_size, errorString);
        }

        //Burst
        if (Parameter("Burst1") == 1)
        {
            verifyCommand(session, errorCode, errorString_size, errorString);
            viPrintf(session, ":SOURce1:BURSt:MODE %s\n", "TRIG");
            viPrintf(session, ":SOURce1:BURSt:NCYCles %@3lf\n", (float) Parameter("BurstCycles1"));
            viPrintf(session, ":SOURce1:BURSt:INTernal:PERiod %@3lf\n", (float) Parameter("BurstPeriod1").InSeconds());
            viPrintf(session, ":SOURce1:BURSt:PHASe %@3lf\n", (float) Parameter("BurstPhase1"));
            viPrintf(session, ":TRIGger1:SOURce %s\n", "IMM");
            viPrintf(session, ":SOURce1:BURSt:STATe %@1d\n", 1);
            verifyCommand(session, errorCode, errorString_size, errorString);
        }
    }
}

void
KeysightTestLogger::DefaultSettings2(int func, int amfunc, int fmfunc, int pmfunc, int pwmfunc, int prbsdata)
{
    //Signal
    verifyCommand(session, errorCode, errorString_size, errorString);
    if (Parameter("UsingCh2"))
    {
        viPrintf(session, ":OUTPut2:LOAD %@3lf OHM\n", (float)Parameter("Load2"));
        if (func == 9 || amfunc == 8 && Parameter("ModulateAmplitude2") || fmfunc == 8 && Parameter("ModulateFrequency2") || pmfunc == 8 && Parameter("ModulatePhase2") || pwmfunc == 8 && Parameter("ModulatePulseWidth2")) { //arb
            wave2 = (float*)malloc(sizeof(float) * Parameter("UserWaveData2")->NumValues());
            int offset2 = (int)(Parameter("Phase2") * Parameter("UserWaveData2")->NumValues()) / 360;
            for (int i = 0; i < Parameter("UserWaveData2")->NumValues(); ++i) {
                wave2[i] = Parameter("UserWaveData2")((i+offset2)%(Parameter("UserWaveData2")->NumValues()));
            }
            loadWave(session, wave2, 2, Parameter("UserWaveData2")->NumValues(), "Ch2Wave");

            viPrintf(session, ":SOURce2:FUNCtion:ARBitrary \"%s\"\n", "Ch2Wave", 0);

           
            viPrintf(session, ":SOURce2:FUNCtion:ARBitrary:FREQuency %@3lf HZ\n", (float)Parameter("Frequency2").InHertz());

            verifyCommand(session, errorCode, errorString_size, errorString);
        }

        if (func == 8 || amfunc == 7 && Parameter("ModulateAmplitude2") || fmfunc == 7 && Parameter("ModulateFrequency2") || pmfunc == 7 && Parameter("ModulatePhase2") || pwmfunc == 7 && Parameter("ModulatePulseWidth2")) { //PRBS

            viPrintf(session, ":SOURce2:FUNCtion:PRBS:BRATe %@3lf\n", (float)Parameter("PRBSBitRate2"));
            viPrintf(session, ":SOURce2:FUNCtion:PRBS:TRANsition:BOTH %@3lf S\n", (float)Parameter("PRBSTransition2") * 1e-9);
            switch (prbsdata)
            {
            case 1:
                viPrintf(session, ":SOURce2:FUNCtion:PRBS:DATA %s\n", "PN7");
                break;
            case 2:
                viPrintf(session, ":SOURce2:FUNCtion:PRBS:DATA %s\n", "PN9");
                break;
            case 3:
                viPrintf(session, ":SOURce2:FUNCtion:PRBS:DATA %s\n", "PN11");
                break;
            case 4:
                viPrintf(session, ":SOURce2:FUNCtion:PRBS:DATA %s\n", "PN15");
                break;
            case 5:
                viPrintf(session, ":SOURce2:FUNCtion:PRBS:DATA %s\n", "PN20");
                break;
            case 6:
                viPrintf(session, ":SOURce2:FUNCtion:PRBS:DATA %s\n", "PN23");
                break;
            }
            verifyCommand(session, errorCode, errorString_size, errorString);
        }


        switch (func)
        {
        case 1:
            viPrintf(session, ":SOURce2:FUNCtion %s\n", "SINusoid");
            break;
        case 2:
            viPrintf(session, ":SOURce2:FUNCtion %s\n", "SQUare");
            viPrintf(session, ":SOURce2:FUNCtion:SQUare:DCYCle %@3lf PCT\n", (float)Parameter("SquareDutyCycle2"));

            break;
        case 3:
            viPrintf(session, ":SOURce2:FUNCtion %s\n", "TRIangle");
            break;
        case 4:
            viPrintf(session, ":SOURce2:FUNCtion %s\n", "RAMP");
            viPrintf(session, ":SOURce2:FUNCtion:RAMP:SYMMetry %@3lf PCT\n", (float)Parameter("RampSymmetry2"));
            break;
        case 5:
            viPrintf(session, ":SOURce2:FUNCtion %s\n", "PULSe");
            if ((float)Parameter("PulseDutyCycle2") != 0)
            {
                viPrintf(session, ":SOURce2:FUNCtion:PULSe:DCYCle %@3lf PCT\n", (float)Parameter("PulseDutyCycle2"));
            }
            if ((float)Parameter("PulsePeriod2") != 0)
            {
                viPrintf(session, ":SOURce2:FUNCtion:PULSe:PERiod %@3lf S\n", (float)Parameter("PulsePeriod2").InSeconds());
            }
            if ((float)Parameter("PulseWidth2") != 0)
            {
                viPrintf(session, ":SOURce2:FUNCtion:PULSe:WIDTh %@3lf S\n", (float)Parameter("PulseWidth2").InSeconds());
            }
            if ((float)Parameter("PulseLead2") != 0)
            {
                viPrintf(session, ":SOURce2:FUNCtion:PULSe:TRANsition:LEADing %@3lf S\n", (float)Parameter("PulseLead2") * 1e-9);
            }
            if ((float)Parameter("PulseTrail2") != 0)
            {
                viPrintf(session, ":SOURce2:FUNCtion:PULSe:TRANsition:TRAiling %@3lf S\n", (float)Parameter("PulseTrail2") * 1e-9);
            }
            break;
        case 6:
            viPrintf(session, ":SOURce2:FUNCtion %s\n", "NOISe");
            viPrintf(session, ":SOURce2:FUNCtion:NOISe:BANDwidth %@3lf HZ\n", (float)Parameter("NoiseBandwidth2").InHertz());

            break;
        case 7:
            viPrintf(session, ":SOURce2:FUNCtion %s\n", "DC");
            break;
        case 8:
            viPrintf(session, ":SOURce2:FUNCtion %s\n", "PRBS");
            break;
        case 9:
            viPrintf(session, ":SOURce2:FUNCtion %s\n", "ARB");
            break;
        }

        viPrintf(session, ":SOURce2:VOLTage %@3lf V\n", (float)Parameter("Voltage2").InVolts());
        viPrintf(session, ":SOURce2:VOLTage:OFFSet %@3lf V\n", (float)Parameter("VoltageOffset2").InVolts());
        if (func != 9 && func != 8)
        {
            viPrintf(session, ":SOURce2:FREQuency %@3lf HZ\n", (float)Parameter("Frequency2").InHertz());
        }
        verifyCommand(session, errorCode, errorString_size, errorString);
        if (Parameter("LowVoltage2") != 0 && Parameter("HighVoltage2") != 0)
        {
            viPrintf(session, ":SOURce2:VOLTage:LIMit:HIGH %@3lf V\n", (float)Parameter("HighVoltage2").InVolts());
            viPrintf(session, ":SOURce2:VOLTage:LIMit:LOW %@3lf V\n", (float)Parameter("LowVoltage2").InVolts());
            viPrintf(session, ":SOURce2:VOLTage:LIMit:STATe %@1d\n", 1);
        }
        viPrintf(session, ":SOURce2:PHASe %@3lf\n", (float) Parameter("Phase2"));
        verifyCommand(session, errorCode, errorString_size, errorString);
        //End signal

        //AM signal
        if (Parameter("ModulateAmplitude2") == 1)
        {
            viPrintf(session, ":SOURce2:AM:SOURce %s\n", "INT"); //possible modification
            viPrintf(session, ":SOURce2:AM:DSSC %@1d\n", 0); //possible modification
            viPrintf(session, ":SOURce2:AM:DEPTh %@3lf\n", (float)Parameter("AMDepth2"));
            switch (amfunc)
            {
            case 1:
                viPrintf(session, ":SOURce2:AM:INTernal:FUNCtion %s\n", "SINusoid");
                break;
            case 2:
                viPrintf(session, ":SOURce2:AM:INTernal:FUNCtion %s\n", "SQUare");
                break;
            case 3:
                viPrintf(session, ":SOURce2:AM:INTernal:FUNCtion %s\n", "TRIangle");
                break;
            case 4:
                viPrintf(session, ":SOURce2:AM:INTernal:FUNCtion %s\n", "RAMP");
                break;
            case 5:
                viPrintf(session, ":SOURce2:AM:INTernal:FUNCtion %s\n", "NRAMp");
                break;
            case 6:
                viPrintf(session, ":SOURce2:AM:INTernal:FUNCtion %s\n", "NOISe");
                break;
            case 7:
                viPrintf(session, ":SOURce2:AM:INTernal:FUNCtion %s\n", "PRBS");
                break;
            case 8:
                viPrintf(session, ":SOURce2:AM:INTernal:FUNCtion %s\n", "ARB");
                break;
            }
            viPrintf(session, ":SOURce2:AM:INTernal:FREQuency %@3lf HZ\n", (float)Parameter("AMFrequency2").InHertz());
            viPrintf(session, ":SOURce2:AM:STATe %@1d\n", 1);
            verifyCommand(session, errorCode, errorString_size, errorString);
        }
        //end AM signal

        //FM Signal
        if (Parameter("ModulateFrequency2") == 1)
        {
            viPrintf(session, ":SOURce2:FM:SOURce %s\n", "INT"); //possible modification
            viPrintf(session, ":SOURce2:FM:DEViation %@3lf HZ\n", (float)Parameter("FMDeviation2").InHertz());
            viPrintf(session, ":SOURce2:FM:INTernal:FREQuency %@3lf HZ\n", (float)Parameter("FMFrequency2").InHertz());

            switch (fmfunc)
            {
            case 1:
                viPrintf(session, ":SOURce2:FM:INTernal:FUNCtion %s\n", "SINusoid");
                break;
            case 2:
                viPrintf(session, ":SOURce2:FM:INTernal:FUNCtion %s\n", "SQUare");
                break;
            case 3:
                viPrintf(session, ":SOURce2:FM:INTernal:FUNCtion %s\n", "TRIangle");
                break;
            case 4:
                viPrintf(session, ":SOURce2:FM:INTernal:FUNCtion %s\n", "RAMP");
                break;
            case 5:
                viPrintf(session, ":SOURce2:FM:INTernal:FUNCtion %s\n", "NRAMp");
                break;
            case 6:
                viPrintf(session, ":SOURce2:FM:INTernal:FUNCtion %s\n", "NOISe");
                break;
            case 7:
                viPrintf(session, ":SOURce2:FM:INTernal:FUNCtion %s\n", "PRBS");
                break;
            case 8:
                viPrintf(session, ":SOURce2:FM:INTernal:FUNCtion %s\n", "ARB");
                break;
            }
            viPrintf(session, ":SOURce2:FM:STATe %@1d\n", 1);
            verifyCommand(session, errorCode, errorString_size, errorString);
        }
        //End FM Signal

        //PM Signal
        if (Parameter("ModulatePhase2") == 1)
        {
            viPrintf(session, ":SOURce2:PM:SOURce %s\n", "INT");
            viPrintf(session, ":SOURce2:PM:DEViation %@3lf\n", (float)Parameter("PMDeviation2"));
            viPrintf(session, ":SOURce2:PM:INTernal:FREQuency %@3lf HZ\n", (float)Parameter("PMFrequency2").InHertz());
            switch (pmfunc)
            {
            case 1:
                viPrintf(session, ":SOURce2:PM:INTernal:FUNCtion %s\n", "SINusoid");
                break;
            case 2:
                viPrintf(session, ":SOURce2:PM:INTernal:FUNCtion %s\n", "SQUare");
                break;
            case 3:
                viPrintf(session, ":SOURce2:PM:INTernal:FUNCtion %s\n", "TRIangle");
                break;
            case 4:
                viPrintf(session, ":SOURce2:PM:INTernal:FUNCtion %s\n", "RAMP");
                break;
            case 5:
                viPrintf(session, ":SOURce2:PM:INTernal:FUNCtion %s\n", "NRAMp");
                break;
            case 6:
                viPrintf(session, ":SOURce2:PM:INTernal:FUNCtion %s\n", "NOISe");
                break;
            case 7:
                viPrintf(session, ":SOURce2:PM:INTernal:FUNCtion %s\n", "PRBS");
                break;
            case 8:
                viPrintf(session, ":SOURce2:PM:INTernal:FUNCtion %s\n", "ARB");
                break;
            }
            viPrintf(session, ":SOURce2:PM:STATe %@1d\n", 1);
            verifyCommand(session, errorCode, errorString_size, errorString);
        }
        //End PM signal


        if (Parameter("ModulatePulseWidth2") == 1) {
            switch (pwmfunc)
            {
            case 1:
                viPrintf(session, ":SOURce2:PWM:INTernal:FUNCtion %s\n", "SINusoid");
                break;
            case 2:
                viPrintf(session, ":SOURce2:PWM:INTernal:FUNCtion %s\n", "SQUare");
                break;
            case 3:
                viPrintf(session, ":SOURce2:PWM:INTernal:FUNCtion %s\n", "TRIangle");
                break;
            case 4:
                viPrintf(session, ":SOURce2:PWM:INTernal:FUNCtion %s\n", "RAMP");
                break;
            case 5:
                viPrintf(session, ":SOURce2:PWM:INTernal:FUNCtion %s\n", "NRAMp");
                break;
            case 6:
                viPrintf(session, ":SOURce2:PWM:INTernal:FUNCtion %s\n", "NOISe");
                break;
            case 7:
                viPrintf(session, ":SOURce2:PWM:INTernal:FUNCtion %s\n", "PRBS");
                break;
            case 8:
                viPrintf(session, ":SOURce2:PWM:INTernal:FUNCtion %s\n", "ARB");
                break;
            }
            viPrintf(session, ":SOURce2:PWM:DEViation %@3lf S\n", (float)Parameter("PWMDeviation2").InSeconds());
            viPrintf(session, ":SOURce2:PWM:DEViation:DCYCle %@3lf\n", (float)Parameter("PWMDutyCycle2"));
            viPrintf(session, ":SOURce2:PWM:INTernal:FREQuency %@3lf HZ\n", (float)Parameter("PMFrequency2").InHertz());
            viPrintf(session, ":SOURce2:PWM:STATe %@1d\n", 1);
            verifyCommand(session, errorCode, errorString_size, errorString);
        }
        //Sweep
        if (Parameter("Sweep2") == 1)
        {
            viPrintf(session, ":SOURce2:FREQuency:STARt %@3lf HZ\n", (float)Parameter("Frequency2").InHertz());
            viPrintf(session, ":SOURce2:FREQuency:STOP %@3lf HZ\n", (float)Parameter("SweepEndFrequency2").InHertz());
            viPrintf(session, ":SOURce2:SWEep:TIME %@3lf S\n", (float)Parameter("SweepTime2").InSeconds());
            if (Parameter("SweepLogSpacing2") == 0) {
                viPrintf(session, ":SOURce2:SWEep:SPACing %s\n", "LINear");
            }
            else {
                viPrintf(session, ":SOURce2:SWEep:SPACing %s\n", "LOGarithmic");
            }
            viPrintf(session, ":SOURce2:SWEep:HTIMe %@3lf S\n", (float)Parameter("SweepHoldTime2").InSeconds());
            viPrintf(session, ":TRIGger2:SOURce %s\n", "IMM");
            viPrintf(session, ":SOURce2:FREQuency:MODE %s\n", "SWE");
            verifyCommand(session, errorCode, errorString_size, errorString);
        }

        //Burst
        if (Parameter("Burst2") == 1)
        {
            verifyCommand(session, errorCode, errorString_size, errorString);
            viPrintf(session, ":SOURce2:BURSt:MODE %s\n", "TRIG");
            viPrintf(session, ":SOURce2:BURSt:NCYCles %@3lf\n", (float)Parameter("BurstCycles2"));
            viPrintf(session, ":SOURce2:BURSt:INTernal:PERiod %@3lf\n", (float)Parameter("BurstPeriod2").InSeconds());
            viPrintf(session, ":SOURce2:BURSt:PHASe %@3lf\n", (float)Parameter("BurstPhase2"));
            viPrintf(session, ":TRIGger2:SOURce %s\n", "IMM");
            viPrintf(session, ":SOURce2:BURSt:STATe %@1d\n", 1);
            verifyCommand(session, errorCode, errorString_size, errorString);
        }
    }
}










       

