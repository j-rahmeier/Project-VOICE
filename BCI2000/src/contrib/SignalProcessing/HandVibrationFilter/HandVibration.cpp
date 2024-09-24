#include "HandVibration.h"

HandVibration::HandVibration () :
  mp_arduino(NULL),
  m_is_connected(false)
{
  m_cmd_pkt    =  { Start };
  m_config_pkt = { 10, 10 };
}

HandVibration::~HandVibration ()
{
  this->Close ();
}

bool HandVibration::Open (const std::string& _COMstring)
{
  std::string COMprefix = "\\\\.\\";
  mp_arduino =  new SerialPort ( (COMprefix+_COMstring).c_str() );
  m_is_connected = mp_arduino->isConnected ();
  return m_is_connected;
}


void HandVibration::Close ()
{
  if (m_is_connected)
    mp_arduino->closeSerial ();
  delete mp_arduino;
  mp_arduino = NULL;
}


bool HandVibration::ConfigureVibration (int _amplitude, float _frequency)
{
  if (m_is_connected)
  {
    m_cmd_pkt.Command = Configure;
    if (mp_arduino->writeSerialPort ((char*)& m_cmd_pkt, sizeof (CommandPacket)))
    {
      m_config_pkt.Amplitude = _amplitude;
      m_config_pkt.Frequency = _frequency*10;

      if (mp_arduino->writeSerialPort ((char*)& m_config_pkt, sizeof (ConfigurationPacket)))
        return true;
    }
  }
  return false;
}


bool HandVibration::UpdateVibrationAmplitude (int _amplitude)
{
  if (m_is_connected)
  {
    m_cmd_pkt.Command = UpdateAmplitude;
    if (mp_arduino->writeSerialPort ((char*)& m_cmd_pkt, sizeof (CommandPacket)))
    {
      m_config_pkt.Amplitude = _amplitude;
      m_config_pkt.Frequency = 0;

      if (mp_arduino->writeSerialPort ((char*)& m_config_pkt, sizeof (ConfigurationPacket)))
        return true;
    }
  }
  return false;
}


bool HandVibration::StartVibration ()
{
  if (m_is_connected)
  {
    m_cmd_pkt.Command = Start;
    return mp_arduino->writeSerialPort ((char*)& m_cmd_pkt, sizeof (CommandPacket));
  }
  return false;
}


bool HandVibration::StopVibration ()
{
  if (m_is_connected)
  {
    m_cmd_pkt.Command = Stop;
    return mp_arduino->writeSerialPort ((char*)& m_cmd_pkt, sizeof (CommandPacket));
  }
  return false;
}