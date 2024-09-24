#pragma once
#pragma pack(1)

/**
 *  AmpDataPacket Header.
 */
typedef struct {
  int64_t                ampID;   //The ampID associated with this data packet (if appropriate).
  uint64_t               length;  // Specifies the length of the data field.
} AmpDataPacketHeader;

/**
 *  Packet Format 1 Used by the NA300 and NA400 prior to and including
 *  version 1.4.3 of the NA400 firmware.).
 *
 *  <b>Notes:</b><br>
 *  &nbsp; 1) Used by the NA300 and NA400 prior to and including
 *  version 1.4.3 of the NA400 firmware.<br>
 *
 *  &nbsp; 2) If this is raw data from the amplifier, it will be in analog to digital units
 *  and this will need to be converted to microvolts. Please see the Amp Server Pro SDK documentation
 *  for details.<br>
 *
 *  &nbsp; 3) Data in this packet format is always in network byte order.<br>
 */
typedef struct {
  uint32_t                      header[8];    // DINS (Digital Inputs) 1-8/9-16 at bytes 24/25; net type at byte 26.
  float                         eeg[256];     // EEG Data.
  float                         pib[7];       // PIB data.
  float                         unused1;      // N/A
  float                         ref;          // The reference channel.
  float                         com;          // The common channel.
  float                         unused2;      // N/A
  float                         padding[13];  // N/A
} PacketFormat1;

/**
 *  Packet Format 2 (Used NA400 in firmware versions AFTER 1.4.3.).
 *
 *  <b>Notes:</b><br>
 *  &nbsp; 1) If this is raw data from the amplifier, it will be in analog to digital units
 *  and this will need to be converted to microvolts. Please see the Amp Server Pro SDK documentation
 *  for details.<br>
 *
 *  &nbsp; 2) If this is raw data from the amplifier, it will be in analog to digital units
 *  and this will need to be converted to microvolts. Please see the Amp Server Pro SDK documentation
 *  for details.<br>
 *
 *  &nbsp; 3) Data in this packet format is always in little endian byte order.<br>
 */
typedef struct {
  uint8_t                       digitalInputs;
  uint8_t                       status;
  uint8_t                       batteryLevel[3];
  uint8_t                       temperature[3];
  uint8_t                       sp02;
  uint8_t                       heartRate[2];
} PacketFormat2_PIB_AUX; // 11 bytes

typedef struct {
  uint16_t                      digitalInputs;
  uint8_t                       tr;
  PacketFormat2_PIB_AUX         pib1_aux;
  PacketFormat2_PIB_AUX         pib2_aux;
  uint64_t                      packetCounter;
  uint64_t                      timeStamp;
  uint8_t                       netCode;
  uint8_t                       reserved[38];
} PacketFormat2Header; // 80 bytes
//int32_t                       eegData[256];  // 1024 bytes
typedef struct {
  uint32_t                      auxData[3];
  uint32_t                      refMonitor;
  uint32_t                      comMonitor;
  uint32_t                      driveMonitor;
  uint32_t                      diagnosticsChannel;
  uint32_t                      currentSense;
  int32_t                       pib1_Data[16];
  int32_t                       pib2_Data[16];
} PacketFormat2Footer; // 160 bytes

typedef struct {
  PacketFormat2Header           header;
  int32_t                       eegData[256];
  PacketFormat2Footer           footer;
}PacketFormat2;

#pragma pack()