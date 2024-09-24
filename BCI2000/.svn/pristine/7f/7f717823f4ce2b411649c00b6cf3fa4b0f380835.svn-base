#ifndef MICROLEADS_PROTOCOL_H
#define MICROLEADS_PROTOCOL_H

#pragma pack(push, 1)

struct header {
    unsigned char bgn;
    unsigned char op;
    unsigned char seqaddress;
    unsigned char dlen;
};

struct dataArray {
    unsigned short eng00;
    unsigned short emg0;
    unsigned short eng10;
    unsigned short eng01;
    unsigned short emg1;
    unsigned short eng11;
};

struct oobdata {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
    unsigned char b4;
    unsigned char b5;
    unsigned char b6;
    unsigned char b7;
};

struct FEC {
    long long fec1;
    long long fec2;
};

struct CRC {
    unsigned int crc;
};

struct ender {
    unsigned char crc;
    unsigned char end;
};

struct basestationSOHformat {
    unsigned char device_type;
    short batmon_V;
    short batmon_T;
    unsigned short V_fault;
    unsigned short T_fault;
};

struct powerpadSOHformat {
    unsigned char device_type;
    short batmon_V;
    short batmon_T;
    unsigned short adc_fault;
    unsigned short I_fault;
    unsigned short T_fault;
    short Temp0_degC;
    int Vin_uV;
    int Vcoil_mV;
    int Iin_mA;
    unsigned int Temp0_uVadc;
    unsigned int Vin_uVadc;
    unsigned int Vcoil_uadc;
    unsigned int Iin_uVadc;
    unsigned char flags;
    unsigned char pad0;
    unsigned char pad1;
    unsigned char pad2;
};

struct datapacket {
  header hdr;
  dataArray dataPoints[160];
  oobdata oobdata;
  FEC fec;
  CRC crc;
};

struct basestation_SOH_packet {
  struct header A;
  struct basestationSOHformat B;
  struct ender C;
};

struct powerpad_SOH_packet {
  struct header A;
  struct powerpadSOHformat B;
  struct ender C;
};

#pragma pack(pop)

#endif