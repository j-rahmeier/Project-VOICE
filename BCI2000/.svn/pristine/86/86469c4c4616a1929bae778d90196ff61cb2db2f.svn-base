////////////////////////////////////////////////////////////////////////////////
// Authors: schalkadmin@DESKTOP-OTQRNP6.wucon.wustl.edu
// Description: MicroLeadsADC implementation
////////////////////////////////////////////////////////////////////////////////
#include "MicroLeadsADC.h"

// Make the source filter known to the framework.
RegisterFilter( MicroLeadsADC, 1 ); // ADC filters must be registered at location "1" in order to work.

MicroLeadsADC::MicroLeadsADC()
//: mDeviceHandle( AMP_API_INVALID_DEVICE_HANDLE ), // Each plain data member should appear in the initializer list.
//  mpBuffer( 0 ) // Make sure we can call delete[] without crashing even if we never called new[].
:myfile("C:/Users/schalkadmin/Desktop/data2.csv", std::ios::app)
{
}

MicroLeadsADC::~MicroLeadsADC()
{
}

void
MicroLeadsADC::OnPublish()
{
  // Declare any parameters that the filter needs....

  // change to auto later
 BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties int SourceCh= 4 "
       "auto 1 4 // number of digitized and stored channels",

    "Source:Signal%20Properties int SampleBlockSize= 160 "
       "auto 1 % // number of samples transmitted at a time",

    "Source:Signal%20Properties float SamplingRate= 21500Hz "
       "auto 0.0 % // sample rate",

    "Source:Signal%20Properties list SourceChGain= 4 1 1 1 1 "
       " // physical units per raw A/D unit",

    "Source:Signal%20Properties list SourceChOffset= 4 0 0 0 0 "
       " // raw A/D offset to subtract, typically 0",

    "Source:Signal%20Properties list ChannelNames= 4 ENG0 EMG0 ENG1 EMG1 "
       " // names of amplifier channels",

   "Source:Signal%20Properties list ChannelNames= 4 ENG0 EMG0 ENG1 EMG1 "
   " // names of amplifier channels",

   // ================ Device Settings Parameters ================

   "Device%20Settings:Device%20Settings int Implant%20StimZ%20Address= 0 0 0 13 "
   " // Sets StimZ Address of Implant for use with BCI2000. Must be between 0-13 and not be the same as Powerpad StimZ Address ",

   "Device%20Settings:Device%20Settings int Powerpad%20StimZ%20Address= 0 0 0 13 "
   " // Sets StimZ Address of Powerpad for use with BCI2000. Must be between 0-13 and not be the same as Implant StimZ Address ",

   "Device%20Settings:Device%20Settings int Implant%20Radio%20Frequency%20Channel= 10 10 10 80 "
   " // Sets Radio Frequency Channel of Implant for use with BCI2000. Must be between 10, 20, 30, ... 80 and not be the same as Powerpad Radio Frequency Channel" ,

   "Device%20Settings:Device%20Settings int Powerpad%20Radio%20Frequency%20Channel= 10 10 10 80 "
   " // Sets Radio Frequency Channel of Powerpad for use with BCI2000. Must be between 10, 20, 30, ... 80 and not be the same as Implant Radio Frequency Channel",

   // ================ Stimulation Parameters ================

   "Stimulation:Stimulation int EnableSTIM0= 0 0 0 1 "
   " // Enable/Disable Stimulation for STIM0 channel (boolean)",

   "Stimulation:Stimulation int EnableSTIM1= 1 0 0 1 "
   " // Enable/Disable Stimulation for STIM1 channel (boolean)",

   "Stimulation:Stimulation int EnableStimulation= 0 0 0 1 "
   " // Enable/Disable streaming of neural data (boolean)",

   "Stimulation:Stimulation int SupplyVoltage= 4 4 4 9 "
   " // Set Supply Voltage in Volts",

   "Stimulation:Stimulation int Phase1DurationCounts= 9"
   " // Set Phase 1 Duration in counts of 11.6 us",

   "Stimulation:Stimulation int InterphaseDurationCounts= 6"
   " // Set Interphase Duration in counts of 11.6 us",

   "Stimulation:Stimulation int Phase2DurationCounts= 9"
   " // Set Phase 2 Duration in counts of 11.6 us",

   "Stimulation:Stimulation int Phase1STIM0AmplitudeCounts= 15"
   " // Set Phase 1 Amplitude for STIM0 in counts of 10 uA",

   "Stimulation:Stimulation int Phase1STIM1AmplitudeCounts= 10"
   " // Set Phase 1 Amplitude for STIM1 in counts of 10 uA",

   "Stimulation:Stimulation int Phase2STIM0AmplitudeCounts= 15"
   " // Set Phase 2 Amplitude for STIM0 in counts of 10 uA",

   "Stimulation:Stimulation int Phase2STIM1Amplitudecounts= 10"
   " // Set Phase 2 Amplitude for STIM1 in counts of 10 uA",

   "Stimulation:Stimulation int ChargeRecoveryCounts= 0"
   " // Set Charge Recovery Duration in counts of 11.6 us",

   "Stimulation:Stimulation int StimPeriodCounts= 60"
   " // Set Stim Period Duration in counts of 11.6 us",

   "Stimulation:Stimulation int ADCGain= 1"
   " // Set ADC Gain",

   "Stimulation:Stimulation int Exfil_Iface= 0"
   " // Set exfil_iface parameter",
 END_PARAMETER_DEFINITIONS

}

void
MicroLeadsADC::OnAutoConfig()
{

}

void
MicroLeadsADC::OnPreflight( SignalProperties& Output ) const
{
  SignalType sigType = SignalType::int32;

  int samplesPerBlock = Output.Elements();
  int numberOfSignalChannels = Output.Channels();
  Output = SignalProperties( numberOfSignalChannels , samplesPerBlock, sigType );
}

void
MicroLeadsADC::OnInitialize( const SignalProperties& Output )
{
  // These are the ports that we are using for packet commands. The UDP_recv_port is the port that we receive data from. This can be changed.
  string ip = "192.168.1.10";
  int TCP_port = 3031;
  int UDP_port1 = 2323;
  int UDP_port2 = 2020;
  int UDP_recv_port = 2020;

  finished = false;
  
  // Creating the TCP connection to port 3031
  if (TCP_Socket.Create_TCP(TCP_port, ip) == -1) {
    bciwarn << "could not create";
  }
  else {
    bciwarn << "created!";
  }
  if (TCP_Socket.Connect() == -1) {
    bciwarn << "could not connect";
  }
  else {
    bciwarn << "connected!";
  }

  // Creating the UDP connection to port 2323 to initialize recording
  if (UDP_2323_Socket.Create_UDP(UDP_port1, ip) == -1) {
    bciwarn << "could not create";
  }
  else {
    bciwarn << "created!";
  }
    
  // Creating the UDP connection to port 2020 for data acquisition
  if (UDP_2020_Socket.Create_UDP(UDP_port2, ip) == -1) {
    bciwarn << "could not create";
  }
  else {
    // If successful, we set the receiving port for the data to port 2021 with this function
    UDP_2020_Socket.Set_Recv_Params(UDP_recv_port);
    bciwarn << "created!";
  }
  
  // Check that you can communicate with the basestation
  // NEED TO CHANGE TO RETURN 0 OR -1 DEPENDING ON RESULT OF REQUEST AND ALSO CREATE ERROR SCHEMA HERE
  basestation_SOH(TCP_Socket);

  Sleep(1000);

  // Check that communication with the powerpad is correct
  powerpad_SOH(TCP_Socket);

  Sleep(1000);
  
  // Send the initialization commands to the port 2323
  record_init(UDP_2323_Socket);

  // parameters
  params[0] = char(Parameter("EnableSTIM0") * 8 + Parameter("EnableSTIM1") * 64);

  //params[1] = char(Parameter("Phase1DurationCounts"));
  //params[2] = char(Parameter("InterphaseDurationCounts"));
  //params[3] = char(Parameter("Phase2DurationCounts"));

  //params[4] = char(Parameter("Phase1STIM0AmplitudeCounts") * 10);
  //params[5] = char(Parameter("Phase1STIM1AmplitudeCounts") * 10);
  //params[6] = char(Parameter("Phase2STIM0AmplitudeCounts") * 10);
  //params[7] = char(Parameter("Phase2STIM1AmplitudeCounts") * 10);  

  params[8] = char(Parameter("Implant StimZ Address"));

  // Communicate with port 2020 to show where to send data to.
  open_data_port(UDP_2020_Socket);

  finished = record_enable(TCP_Socket, params);
}

void
MicroLeadsADC::OnStartAcquisition()
{

}

void
MicroLeadsADC::DoAcquire(GenericSignal& Output)
{
  // Call the amplifier API's function for synchronous data transfer here.
  char buf[992];

  datapacket* packet = getData(UDP_2020_Socket, buf);

  // Order for data packet is ENG0 EMG0 ENG1 EMG1
  // 160 of these packets

  // unsigned short eng00;
  // unsigned short emg0;
  // unsigned short eng10;
  // unsigned short eng01;
  // unsigned short emg1;
  // unsigned short eng11;

  // 
  for (int s = 0; s < Output.Elements(); ++s)
  {

    if ((int)packet->hdr.op == 144 || (int)packet->hdr.op == 192) {
      Output(0, s) = 0;
      Output(1, s) = 0;
      Output(2, s) = 0;
      Output(3, s) = 0;
    }
    else 
    if (s % 2 == 0) {
      Output(0, s) = 0.0195 * (packet->dataPoints[s / 2].eng00 - 32768.0);
      Output(1, s) = 0.195 * (packet->dataPoints[s / 2].emg0 - 32768.0);
      Output(2, s) = 0.0195 * (packet->dataPoints[s / 2].eng10 - 32768.0);
      Output(3, s) = 0.195 * (packet->dataPoints[s / 2].emg1 - 32768.0);
    }
    else {
      Output(0, s) = 0.0195 * (packet->dataPoints[(s - 1) / 2].eng01 - 32768.0);
      Output(1, s) = 0.195 * (packet->dataPoints[(s - 1) / 2].emg0 - 32768.0);
      Output(2, s) = 0.0195 * (packet->dataPoints[(s - 1) / 2].eng11 - 32768.0);
      Output(3, s) = 0.195 * (packet->dataPoints[(s - 1) / 2].emg1 - 32768.0);
    }
  }
};

void
MicroLeadsADC::OnStartRun() {
  //// Communicate with port 2020 to show where to send data to.
  //udp_2020(UDP_2020_Socket);

  //// Enable recording to TCP port 3101
  //finished = record_enable(TCP_Socket, params);
}

void
MicroLeadsADC::OnStopRun()
{

}

void
MicroLeadsADC::OnStopAcquisition()
{
  // This method is called from the acquisition thread just before it exits.  Use this method
  // to shut down the amplifier API (undoing whatever was done in OnStartAcquisition).
  // Immediately after this returns, the system will go into an un-initialized state and
  // OnHalt will be called in the main thread.
  record_disable();

  // Close down everything and cleanup
  closesocket(TCP_Socket.sock);
  closesocket(UDP_2020_Socket.sock);
  closesocket(UDP_2323_Socket.sock);
  myfile.close();

  WSACleanup();
}
