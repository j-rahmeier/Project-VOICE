#pragma once
/*------------------------------------------------------------------------------------------------*/
//  Project:	NeuroConcise flexX  Windows Library
/*------------------------------------------------------------------------------------------------*/
//  Purpose:	Functions for interactiong with EEG headset 
//  Author:		t. moore
//  Date:		  20 Oct 2023
//
//	Change History
//	2023-10-20	Version 1.00	- First Draft
//	2024-02-22	Version 1.10    - NJH made it strictly C and DLL-compatible
//	
/*------------------------------------------------------------------------------------------------*/	
#ifndef FLEXX_H_
#define FLEXX_H_

#include <stdint.h>

#ifdef    __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef   FLEXX_API_FUNC
#define   FLEXX_API_FUNC
#endif // FLEXX_API_FUNC

FLEXX_API_FUNC bool flexX_OpenPort(const char* PortName, int BAUD_RATE);
FLEXX_API_FUNC bool flexX_ClosePort();
FLEXX_API_FUNC bool flexX_Start(void);
FLEXX_API_FUNC bool flexX_Stop(void);
FLEXX_API_FUNC bool flexX_RecNo(uint16_t* recno);
FLEXX_API_FUNC bool flexX_GetChannelData(float* buff);
FLEXX_API_FUNC bool flexX_SetFilter(uint8_t filter);
FLEXX_API_FUNC bool flexX_GetFilter(uint8_t *filter);
FLEXX_API_FUNC bool flexX_SetEnabledChannels(uint8_t value);
FLEXX_API_FUNC bool flexX_GetEnabledChannels(uint8_t* Chn);
FLEXX_API_FUNC bool flexX_SetResolution(uint8_t value);
FLEXX_API_FUNC bool flexX_GetResolution(uint8_t* dest);
FLEXX_API_FUNC bool flexX_SetSampling(uint8_t value);
FLEXX_API_FUNC bool flexX_GetSampling(uint8_t* dest);
FLEXX_API_FUNC bool flexX_SetRegister(uint8_t rgg, uint8_t vll);
FLEXX_API_FUNC bool flexX_GetRegister(uint8_t rgg, uint8_t* vll);
FLEXX_API_FUNC bool flexX_GetBatteryStatus(uint8_t* dest);
FLEXX_API_FUNC bool flexX_GetSWversion(char* dest);

#ifdef    __cplusplus
} // close extern "C"
#endif // __cplusplus


#endif // FLEXX_H_

