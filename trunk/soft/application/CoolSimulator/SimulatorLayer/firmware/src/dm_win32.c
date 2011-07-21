/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*      dm_win32.c                                                                  */
/*                                                                            */
/* DESCRIPTION                                                                */
/*    DM service declarations, constant definitions and macros                */
/*    See CSW developement Manual for more.                                   */
/*                                                                            */
/******************************************************************************/


#include "dm.h"

kal_uint32 lcd_frame_buffer_address;

BOOL DM_AudioInit (DM_SpeakerGain SpeakerGain,DM_MicGain MicGain,UINT8 Audio_mode){}

BOOL DM_SpeakerOpen(void){}
BOOL DM_MicOpen(void){}
BOOL DM_SpeakerClose(void){}
BOOL DM_MicClose(void){}

BOOL DM_SetMicGain ( DM_MicGain nGain){}
BOOL DM_SetSpeakerGain ( DM_SpeakerGain nGain){}
BOOL DM_SetSideGain ( DM_SideToneGain nGain){}


BOOL SRVAPI DM_SetAudioMode (UINT8 nAudioMode){}

BOOL  SRVAPI DM_SetAudioMode_From_Calibration(UINT8 nAudioMode){}

BOOL SRVAPI DM_GetAudioMode (UINT8* pAudioMode){}
BOOL SRVAPI  DM_GetSpeakerVolume_From_Calibration(UINT8* pVolume){}


BOOL SRVAPI DM_SetAudioVolume (DM_SpeakerGain nVolume){}
BOOL SRVAPI  DM_GetSpeakerVolume (UINT8* pVolume){}

BOOL SRVAPI  DM_SetAudio_From_Calibration_Volume (DM_SpeakerGain nVolume){}

BOOL	DM_Audio_StartStream(UINT32*	buffer, UINT32	len, DM_PlayRingType codec, BOOL	loop_mode){}
BOOL	DM_Audio_StopStream(VOID){}
BOOL      DM_Audio_PauseStream(VOID){}
BOOL      DM_Audio_ResumeStream(VOID){}


/*
================================================================================
Function   : DM_Audio_RemainingStream
--------------------------------------------------------------------------------
Scope      : Returns the number of words that can still be written in the record buffer.
Parameters : none
Return     : The number of words that can still be written in the record buffer.
================================================================================
*/

UINT32 DM_Audio_RemainingRecord(VOID){}
/*
================================================================================
Function   : DM_Audio_RemainingStream
--------------------------------------------------------------------------------
Scope      : Returns the number of words yet to be played in the stream buffer.
Parameters : none
Return     : The number of words yet to be played in the stream buffer.
================================================================================
*/
UINT32 DM_Audio_RemainingStream(VOID){}

BOOL SRVAPI DM_SetAudioControl( UINT8 nCmd,  UINT16 nValue){}




BOOL SRVAPI DM_PlayTone (
  UINT8 nToneType,
  UINT8 nToneAttenuation,
  UINT16 nDuration
){}

BOOL SRVAPI DM_StopTone (
  VOID
){}

VOID DM_KeyBackLightOpen(){}
VOID DM_KeyBackLightClose(){}
VOID DM_LpwMmiActive(){}
VOID DM_LpwMmiInactive(){}


BOOL SRVAPI DM_Reset(
  VOID
){}

BOOL SRVAPI DM_DeviceSwithOff (
  VOID
){}

BOOL DM_MicStartRecord(UINT32 *pBuffer, UINT32 nBufferSize,DM_PlayRingType codec,BOOL loop_mode){}


BOOL SRVAPI DM_MicStopRecord (
  VOID
){}


BOOL SRVAPI DM_RegisterHandler( 
  UINT8 nDeviceId,
  PFN_DEVICE_HANDLER pfnDevHandler 
){}


BOOL SRVAPI DM_StartGpadcDetect(UINT16 nPeriod){} // in ms

BOOL SRVAPI DM_StopGpadcDetect(VOID){}
//
// UART
//


//
// Others API to support.
//
BOOL DM_Wait(UINT32 nTick){}     // Wait in 1/256 seconds step.
BOOL DM_Delay(UINT32 nSeconds){} // Wait in 1 seconds step.
BOOL dm_DisableWatchDog(){}
BOOL dm_SetWathcDogTime(UINT32 time){}
BOOL dm_SetWathcDog_KeepAlive(){}
BOOL DM_SendDBGEvent(){}//for telling jade_debug_server the phone is ready.it is just for production line
void lcd_power_up(void)
{
}

void lcd_power_down(void)
{
}

VOID BlockWrite_DI(UINT16* buffer, UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy)
{
}
#ifdef USE_BUZZER

UINT8 DM_BuzzerVibratingSet(UINT8 nGpio){}

VOID DM_BuzzerDelayNmS(UINT32 nMillisecondes){}
UINT8 DM_BuzzerOpen(VOID){}
UINT8 DM_BuzzerStop(VOID){}
UINT8 DM_BuzzerPlay(DM_PWT_OCT Oct, DM_PWT_NOTE Note, UINT8 Vol){}

UINT8 DM_BuzzerVolumeSet(UINT8 Vol){}
UINT8 DM_BuzzerSongStart(UINT8* pBuffer, UINT32 nBufferLen, UINT32 duration, DM_BUZZER_SET Btemp, DM_BUZZER_VIBRATING Vtemp, DM_BUZZER_FLASH Ftemp, BUZZERCALLBACK pbuzzer_callback){}
UINT8 DM_BuzzerSongStop(VOID){}
#endif


UINT8 g_powerdown_cause = 0;
VOID DM_SetPowerDown_Cause(UINT8 cause)
{
    g_powerdown_cause = cause;
}
 void bal_updateSoftware(void(*updateCb)(UINT32 percents))
{
}
void stopKeyPadLED(void)
{
}
//
//
//
void DestroyVideoHandle(void)
{
}


