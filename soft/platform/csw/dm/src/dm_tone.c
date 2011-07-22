/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 2003 CII Technologies, Inc.		         		 */
/*                                                                       */
/* PROPRIETARY RIGHTS of CII Technology are involved in the  		     */
/* subject matter of this material.  All manufacturing, reproduction,    */
/* use, and sales rights pertaining to this subject matter are governed  */
/* by the license agreement.  The recipient of this software implicitly  */
/* accepts the terms of the license.                                     */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                            VERSION          */
/*   dm_tone.c                          			 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/* AUTHOR                                                                */
/*       simon  yang                                    */
/* FUNCTIONS                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      DATE  2007/6            REMARKS                                  */
/*************************************************************************/


#include "chip_id.h"
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#include "hal.h"

#else

#include "ars_m.h"
#include "aud_m.h"
#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
#include "mps_m.h"
#else
#include "aps_m.h"
#endif

#include "vois_m.h"
#include "vpp_speech.h"

#endif
#include "sxr_tls.h"
#include "csw_csp.h"
#include "csw_debug.h"

#include "errorcode.h"
#include <cos.h>
#include <base_prv.h>
#include <cswtype.h>
#include "dm.h"
#include <event.h>
#include "ts.h"
#include "stdkey.h"
#include "cmn_defs.h"
#define PAL_TICK1S 16384  // time constant of the timer - 16384 ticks/s
//#define SYS_EV_MASK_AUDIO_TONE            0xF0000001+4

/***********************************************************
for c1000, use this define to play tone,use the global variable to judge if play music or not
***********************************************************/

//#define AUDIO_DELAY(a) sxr_Sleep( (a) * 16384 / 1000 )
#define AUDIO_DELAY(a) dm_sysMSleep(a)

BOOL Play_music = FALSE;
BOOL audioBusy = FALSE;
BOOL audioBusy_stream = FALSE;
UINT32 gTone_Audio_Total_time = 300;
UINT32 g_playToneAudioTempTime = 0;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
UINT8 g_mic_gain  = HAL_AUDIO_MIC_30dB;
UINT8 g_spk_gain ;
#else
UINT8 g_mic_gain         = AUD_MIC_ENABLE;
UINT8 g_spk_gain ;
#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
MPS_HANDLE_T audioHandle = NULL;
MPS_CONFIG_T audioStreamConfig = {
    .audioPath               = SND_ITF_RECEIVER,
    .windows                 = { {0, 0}, 0, 0},
    .volume                  = SND_SPK_VOL_4,
    .equalizerMode           = SND_EQUALIZER_NORMAL
};
#endif
#endif

BOOL isSPKMute = FALSE;

BOOL mic_record = FALSE;

typedef enum  
{
	DTMF_PLAYING,
	DTMF_IDLE
}CII_DTMF_STATE;


/* Tone types. */
//reference GSM 02.40  or TS 22.001
typedef enum  {
	CII_TONE_DTMF_0,
	CII_TONE_DTMF_1,
	CII_TONE_DTMF_2,
	CII_TONE_DTMF_3,
	CII_TONE_DTMF_4,
	CII_TONE_DTMF_5,
	CII_TONE_DTMF_6,
	CII_TONE_DTMF_7,
	CII_TONE_DTMF_8,
	CII_TONE_DTMF_9,
	CII_TONE_DTMF_A,
	CII_TONE_DTMF_B,
	CII_TONE_DTMF_C,
	CII_TONE_DTMF_D,
	CII_TONE_DTMF_S,//'*'
	CII_TONE_DTMF_P,//'#'
	CII_DIAL_TONE,// Comfort tones #1
	CII_SUBSCRIBER_BUSY,//Comfort tones #2
	CII_CONGESTION,//Comfort tones #4
	CII_RADIO_PATHACKNOWLEDGEMENT,//Comfort tones #5
	CII_CALL_DROPPED,//Comfort tones #6
	CII_SPECIAL_INFORMATION,//Comfort tones #7
				//( Error/Special Information;Number Unobtainable ;Authentication Failure )
	CII_CALL_WAITING_TONE,//Comfort tones #8
	CII_RINGING_TONE,
	CII_TEST_TONE,
	CII_TONE_END
}CII_ToneType;


typedef enum  {
	CII_TONE_0dB,
	CII_TONE_m3dB,
	CII_TONE_m9dB,
	CII_TONE_m15dB,
	CII_TONE_DEFAULT
}CII_volume;

typedef struct 
{
	CII_volume level;
	CII_ToneType Frequency;	
	UINT16 Duration;
}CII_AUDIO_TONE;


/*add some scenario book here */
typedef struct
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	enum hal_ToneType tone;
#else
    AUD_TONE_TYPE_T tone;
#endif
       UINT32  tone_term;
}cii_DTMF_term,*cii_DTMF_pterm;

typedef struct
{
	UINT32  term_num;
	cii_DTMF_pterm DTMF_term;
}cii_DTMF_tone,*cii_DTMF_ptone;



const static cii_DTMF_term term_0_P[2] =
{
	{HAL_AUDIO_DTMF_0,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_1_P[2] =
{
	{HAL_AUDIO_DTMF_1,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_2_P[2] =
{
	{HAL_AUDIO_DTMF_2,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_3_P[2] =
{
	{HAL_AUDIO_DTMF_3,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_4_P[2] =
{
	{HAL_AUDIO_DTMF_4,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_5_P[2] =
{
	{HAL_AUDIO_DTMF_5,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_6_P[2] =
{
	{HAL_AUDIO_DTMF_6,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_7_P[2] =
{
	{HAL_AUDIO_DTMF_7,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_8_P[2] =
{
	{HAL_AUDIO_DTMF_8,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_9_P[2] =
{
	{HAL_AUDIO_DTMF_9,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_A_P[2] =
{
	{HAL_AUDIO_DTMF_0,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_B_P[2] =
{
	{HAL_AUDIO_DTMF_B,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_C_P[2] =
{
	{HAL_AUDIO_DTMF_C,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_D_P[2] =
{
	{HAL_AUDIO_DTMF_D,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_S_P[2] =
{
	{HAL_AUDIO_DTMF_S,330},
	{HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_P_P[2] =
{
	{HAL_AUDIO_DTMF_P,330},
	{HAL_AUDIO_SILENCE,330}
};


const  static cii_DTMF_tone DTMF_0_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_0_P
};
const  static cii_DTMF_tone DTMF_1_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_1_P
};
const  static cii_DTMF_tone DTMF_2_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_2_P
};
const  static cii_DTMF_tone DTMF_3_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_3_P
};
const  static cii_DTMF_tone DTMF_4_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_4_P
};
const  static cii_DTMF_tone DTMF_5_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_5_P
};
const  static cii_DTMF_tone DTMF_6_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_6_P
};
const  static cii_DTMF_tone DTMF_7_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_7_P
};
const  static cii_DTMF_tone DTMF_8_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_8_P
};
const  static cii_DTMF_tone DTMF_9_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_9_P
};
const  static cii_DTMF_tone DTMF_A_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_A_P
};
const  static cii_DTMF_tone DTMF_B_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_B_P
};
const  static cii_DTMF_tone DTMF_C_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_C_P
};
const  static cii_DTMF_tone DTMF_D_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_D_P
};
const  static cii_DTMF_tone DTMF_S_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_S_P
};
const  static cii_DTMF_tone DTMF_P_P=
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_P_P
};

const static cii_DTMF_term term_DIAL[1] =
{
	{HAL_AUDIO_COMFORT_950,1000}
};

const static cii_DTMF_tone DTMF_DIAL =
{
	0,
	(cii_DTMF_pterm)&term_DIAL
};

const static cii_DTMF_term term_SUBSCRIBER_BUSY[2] =
{
	{HAL_AUDIO_COMFORT_425,500},
	{HAL_AUDIO_SILENCE,500}
};


const static cii_DTMF_tone DTMF_SUBSCRIBER_BUSY =
{
	1,//changed by CII 
	(cii_DTMF_pterm)&term_SUBSCRIBER_BUSY
};


const static cii_DTMF_term term_CONGESTION[2] =
{
	{HAL_AUDIO_COMFORT_425,200},
	{HAL_AUDIO_SILENCE,200}
};

const static cii_DTMF_tone DTMF_CONGESTION =
{
	1,//changed by CII
	(cii_DTMF_pterm)&term_CONGESTION
};


const static cii_DTMF_term term_RADIO_PATHACKNOWLEDGEMENT[1] =
{
	{HAL_AUDIO_COMFORT_425,200}
};

const static cii_DTMF_tone DTMF_RADIO_PATHACKNOWLEDGEMENT =
{
	0,
	(cii_DTMF_pterm)&term_RADIO_PATHACKNOWLEDGEMENT
};

const static cii_DTMF_term term_CALL_DROPPED[2] =
{
	{HAL_AUDIO_COMFORT_425,200},
	{HAL_AUDIO_SILENCE,200}
};

const static cii_DTMF_tone DTMF_CALL_DROPPED =
{
	1,//changed by CII
	(cii_DTMF_pterm)&term_CALL_DROPPED
};

const static cii_DTMF_term term_SPECIAL_INFORMATION[4] =
{
	{HAL_AUDIO_COMFORT_950,330},
	{HAL_AUDIO_COMFORT_1400,330},
	{HAL_AUDIO_COMFORT_1800,330},
	{HAL_AUDIO_SILENCE,1000}
};

const static cii_DTMF_tone DTMF_SPECIAL_INFORMATION =
{
	3,//changed by CII
	(cii_DTMF_pterm)&term_SPECIAL_INFORMATION
};

const static cii_DTMF_term term_CALL_WAITING_TONE[7] =
{
	{HAL_AUDIO_COMFORT_425,200},
	{HAL_AUDIO_SILENCE,600},
	{HAL_AUDIO_COMFORT_425,200},
	{HAL_AUDIO_SILENCE,3000},
	{HAL_AUDIO_COMFORT_425,600},
	{HAL_AUDIO_SILENCE,600},
	{HAL_AUDIO_COMFORT_425,200}
};

const static cii_DTMF_tone DTMF_CALL_WAITING_TONE =
{
	6,//changed by CII
	(cii_DTMF_pterm)&term_CALL_WAITING_TONE
};

const static cii_DTMF_term term_RINGING_TONE[2] =
{
	{HAL_AUDIO_COMFORT_425,1000},
	{HAL_AUDIO_SILENCE,4000}//changed by CII,refer GSM 02.40 version 7.0.1
	
};

const static cii_DTMF_tone DTMF_RINGING_TONE =
{
	1,//changed by CII
	(cii_DTMF_pterm)&term_RINGING_TONE
};

const static cii_DTMF_term term_TEST_TONE[2] =
{
	{HAL_AUDIO_DTMF_1,2000},
	{HAL_AUDIO_SILENCE,1000}
};

const static cii_DTMF_tone DTMF_TEST_TONE =
{
	1,//changed by CII
	(cii_DTMF_pterm)&term_TEST_TONE
};

//level , frequency , duration  

CONST cii_DTMF_ptone DTMF_ARRAY[CII_TONE_END]=
{
	(cii_DTMF_ptone)&DTMF_0_P,
	(cii_DTMF_ptone)&DTMF_1_P,
	(cii_DTMF_ptone)&DTMF_2_P,
	(cii_DTMF_ptone)&DTMF_3_P,
	(cii_DTMF_ptone)&DTMF_4_P,
	(cii_DTMF_ptone)&DTMF_5_P,
	(cii_DTMF_ptone)&DTMF_6_P,
	(cii_DTMF_ptone)&DTMF_7_P,
	(cii_DTMF_ptone)&DTMF_8_P,
	(cii_DTMF_ptone)&DTMF_9_P,
	(cii_DTMF_ptone)&DTMF_A_P,
	(cii_DTMF_ptone)&DTMF_B_P,
	(cii_DTMF_ptone)&DTMF_C_P,
	(cii_DTMF_ptone)&DTMF_D_P,
	(cii_DTMF_ptone)&DTMF_S_P,
	(cii_DTMF_ptone)&DTMF_P_P,
	(cii_DTMF_ptone)&DTMF_DIAL,
	(cii_DTMF_ptone)&DTMF_SUBSCRIBER_BUSY,
	(cii_DTMF_ptone)&DTMF_CONGESTION,
	(cii_DTMF_ptone)&DTMF_RADIO_PATHACKNOWLEDGEMENT,
	(cii_DTMF_ptone)&DTMF_CALL_DROPPED,
	(cii_DTMF_ptone)&DTMF_SPECIAL_INFORMATION,
	(cii_DTMF_ptone)&DTMF_CALL_WAITING_TONE,
	(cii_DTMF_ptone)&DTMF_RINGING_TONE,
	(cii_DTMF_ptone)&DTMF_TEST_TONE,
};

extern BOOL COS_SendSysEvent(COS_EVENT* pEvent,UINT16 nOption);
BOOL DM_GetSPKMuteFlag(VOID);
CII_DTMF_STATE cii_tone_sate = DTMF_IDLE ;
CII_AUDIO_TONE cii_currentDTMF ;
UINT32  term_num =0;	

UINT8 gAudio_mode=DM_AUDIO_MODE_EARPIECE;
UINT8 g_speaker_vlume=5;


#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
struct hal_AudioCfg audio_cfg = {
	HAL_AUDIO_SPK_m3dB,
	HAL_AUDIO_SPK,
	HAL_AUDIO_MIC_30dB,
	HAL_AUDIO_MIC,
	HAL_AUDIO_SIDE_MUTE
};
#else
AUD_ITF_T   audioItf = AUD_ITF_RECEIVER;
AUD_LEVEL_T audio_cfg =
{
    AUD_SPK_VOL_4,
    AUD_MIC_ENABLE,
    AUD_SIDE_MUTE,
    AUD_TONE_m3dB
};
HAL_AIF_STREAM_T audioStream =
{
    NULL,
    0,
    HAL_AIF_FREQ_8000HZ,
    HAL_AIF_MONO,
    TRUE,
    NULL,
    NULL
};
AUD_SPK_LEVEL_T spkLevelBackup = AUD_SPK_VOL_4;

#endif
#if 0
static struct hal_AudioCfg audio_cfg_mute = {
	HAL_AUDIO_SPK_MUTE,
	HAL_AUDIO_SPK,
	HAL_AUDIO_MIC_30dB,
	HAL_AUDIO_MIC,
	HAL_AUDIO_SIDE_MUTE
};
#endif

UINT32 g_audioRecBufStart   = 0;
UINT32 g_audioRecBufLen     = 0;
// Current offset from start
UINT32 g_audioRecBufOffset  = 0;

UINT32 g_audioPlayBufStart   = 0;
UINT32 g_audioPlayBufLen     = 0;
// Current offset from start
UINT32 g_audioPlayBufOffset  = 0;

PRIVATE HANDLE g_toneSem = 0xff;

PUBLIC VOID DM_InitToneOperationDoneSem(VOID)
{
    g_toneSem = COS_CreateSemaphore(0);
}

PRIVATE VOID DM_WaitToneOperationDone(VOID)
{
    COS_WaitForSemaphore(g_toneSem, COS_WAIT_FOREVER);
}

PRIVATE VOID DM_NotifyToneOperationDone(VOID)
{
    COS_ReleaseSemaphore(g_toneSem);
}

// LOL CAT
void Delay_for_audio(UINT32 time) 
{
   UINT32 i,j,k;
 for(i=0;i<time;i++)
  for(j=0;j<20;j++)
   for(k=0;k<30;k++)
   {
     asm volatile ("nop");
   }
}

// LOL CAT
VOID dm_sysMSleep(UINT32 n)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    // Very basic delay for non-critical timing
    // This will wait n/256 sec +/- 1/256 sec, so
    // for a delay of at _least_ m/256, use (m+1)
    UINT32 RTC_Start;
    UINT32 RTC_End;

    RTC_Start = hal_timRTC_GetTime();

    if ((RTC_Start & 0x80000000) != 0)
    {   // In upper half of the range - readjust...
        RTC_Start &= ~(0x80000000);
        RTC_End = RTC_Start + n;
        while ((hal_timRTC_GetTime() + 0x80000000) < RTC_End);
    }
    else
    {
        RTC_End = RTC_Start + n;
        while (hal_timRTC_GetTime() < RTC_End);
    }
#else
    hal_TimDelay(n);
#endif
}


#if 0
void dm_sysMSleep(UINT32 uMilliSecond)
{
    u32 RTC_Start;
    u32 RTC_End;
    // Very basic delay for non-critical timing
    // This will wait n/256 sec +/- 1/256 sec, so
    // for a delay of at _least_ m/256, use (m+1)

    u32 n=(u32)(uMilliSecond*256/1000);
    RTC_Start = hal_timRTC_GetTime();

    if ((RTC_Start & 0x80000000) != 0)
    {   // In upper half of the range - readjust...
        RTC_Start &= ~(0x80000000);
        RTC_End = RTC_Start + n;
        while ((hal_timRTC_GetTime() + 0x80000000) < RTC_End);
    }
    else
    {
        RTC_End = RTC_Start + n;
        while (hal_timRTC_GetTime() < RTC_End);
    }
}



CII_AUDIO_TONE	* Audio_Wait(UINT32  time, u8 msgBox) 
{
	UINT32  tick_time = (time * 16) / 100;
	CII_AUDIO_TONE		*msg=0;
	UINT32  evt[2];

	sxs_StartTimer(tick_time,PRV_DM_AUDIO_TIMER_ID, NULL, FALSE, msgBox);
	msg = sxr_Wait(evt, msgBox);:
	if(msg!=0)
	{
		hal_audio_StopTone();
		sxs_StopTimer(PRV_DM_AUDIO_TIMER_ID, NULL, toneMsgBox);
	}
	return msg;
}
#endif



/*
================================================================================
Function   : audio_Init
--------------------------------------------------------------------------------
Scope      : NOTHING!!! 
Parameters : none
Return     : none
================================================================================
*/

#define CLOSED_SPEAKER 0
#define OPENED_SPEAKER 1
#define CLOSED_MIC 0
#define OPENED_MIC 1


BOOL gSpeakerOpen_stasus = CLOSED_SPEAKER; 
BOOL gMicOpen_stasus = CLOSED_MIC; 

BOOL Speeching = FALSE;

BOOL Speaker_Close(VOID)
{
  if(gSpeakerOpen_stasus == OPENED_SPEAKER)
	{
     CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### speaker  close sucessed  \n") );  

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
     hal_audio_SpeakerClose();
#else
#endif
     gSpeakerOpen_stasus = CLOSED_SPEAKER;
     Delay_for_audio(5);
     return TRUE;
        
   }
   else
   {
      CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### speaker  already close by other user \n") );  
      return FALSE;
   }

        
}
BOOL Speaker_Open(VOID)
{
  if(gSpeakerOpen_stasus == CLOSED_SPEAKER)
	{
     CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####  speaker open sucessed  \n") );  

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
     hal_audio_SpeakerOpen();
#endif
     gSpeakerOpen_stasus = OPENED_SPEAKER;
     Delay_for_audio(5);
     return TRUE;
        
   }
   else
   {
      CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####  speaker already open by other user \n") );  
      return FALSE;
   }

}

BOOL Mic_close(VOID)
{
  if(gMicOpen_stasus == OPENED_MIC)
	{
     CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####   close mic sucessed  \n") );  

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
     hal_audio_MicClose();
#endif
     gMicOpen_stasus = CLOSED_MIC;
     Delay_for_audio(5);
     return TRUE;
        
   }
   else
   {
      CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### mic  already close by other user \n") );  
      return FALSE;
   }

        
}
BOOL Mic_open(VOID)
{
  if(gMicOpen_stasus == CLOSED_MIC)
	{
     CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####  mic open sucessed  \n") );  

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
     hal_audio_MicOpen();
#endif
     gMicOpen_stasus = OPENED_MIC;
     Delay_for_audio(5);
     return TRUE;
        
   }
   else
   {
      CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### mic  already open by other user \n") );  
      return FALSE;
   }

}
/*
================================================================================
Function   :audio_ParseMsg
--------------------------------------------------------------------------------
Scope      : 
Parameters : pointer to the received Message (Msg_t *)
Return     : none
================================================================================
*/
UINT16 DTMF_duration = 0;
BOOL  finish =0;//changed by CII 
UINT8 short_time_keytone=0;

/*
This function set speaker level for tone play.
*/
BOOL LOCAL  DM_SetToneVolume (DM_SpeakerGain nVolume)
{

    /* Check the volume argument. */
    if(nVolume>DM_AUDIO_SPK_6dB)
    {
      nVolume = DM_AUDIO_SPK_6dB;
    }

	if (DM_GetSPKMuteFlag())
	{
		g_spk_gain = nVolume;
		return TRUE;
	}

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)

    audio_cfg.spkGain  = nVolume;
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### Tone audio_cfg.spkGain =%d   \n") ,audio_cfg.spkGain);  

    /* Update the audio parameters in the drivers. */
    if (FALSE == hal_audio_Config(&audio_cfg))
    {
      AUDIO_DELAY(2);
      if (FALSE == hal_audio_Config(&audio_cfg))
      {
        return FALSE;
      }
    }
#else
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### Tone nVolume =%d   \n"), nVolume);

#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
    audioStreamConfig.volume = nVolume;
    if(audioHandle)
    {
        mps_SetConfig(audioHandle, &audioStreamConfig);
    }
#else
	//save previous spk level,spkLevelBackup is used to recover the previous spk level
	if(cii_tone_sate == DTMF_IDLE)
	{
		spkLevelBackup = audio_cfg.spkLevel;
	}
	
    audio_cfg.spkLevel  = nVolume;
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### Tone audio_cfg.spkLevel =%d,Speeching=%d,spkLevelBackup=%d\n") ,audio_cfg.spkLevel,Speeching,spkLevelBackup);  

    /* Update the audio parameters in the drivers. */
   if(Speeching == TRUE){
       VOIS_AUDIO_CFG_T voisCfg = 
            {
                    .spkLevel = audio_cfg.spkLevel,
                    .micLevel = audio_cfg.micLevel,
                    .sideLevel = audio_cfg.sideLevel,
                    .toneLevel = audio_cfg.toneLevel,
                    .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
                    .decMute = VPP_SPEECH_UNMUTE
              };
      if (vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO)
      {
        AUDIO_DELAY(2);
        if (AUD_ERR_NO != vois_Setup(audioItf, &voisCfg))
        {
          return FALSE;
        }
      }    
    }
   else{
     if (aud_Setup(audioItf, &audio_cfg) != AUD_ERR_NO)
     {
       AUDIO_DELAY(2);
       if (AUD_ERR_NO != aud_Setup(audioItf, &audio_cfg))
       {
         return FALSE;
       }
     }
   }
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */

#endif
    Delay_for_audio(3);

    return TRUE;
}

VOID audio_ParseMsg( COS_EVENT* pEvent)
{
    UINT32  tick_time =0 ; 
	DM_SpeakerGain SpeakerLevel =0;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    BOOL ret = FALSE;
#endif
    
    if(pEvent->nParam1==PRV_DM_AUDIO_TIMER_ID)
    {
            if(short_time_keytone==1)
            {
                CSW_TRACE(CSW_DM_TS_ID, TSTXT("    the short time tone is over,spkLevelBackup=%d,audio_cfg.spkLevel=%d\n"),spkLevelBackup,audio_cfg.spkLevel );  

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
                if(FALSE == hal_audio_StopTone())
                {
                    AUDIO_DELAY(2);
                    hal_audio_StopTone();
                }
#else
                while (AUD_ERR_NO != aud_ToneStop(audioItf))
                {
                    COS_Sleep(1);
                }
				
				COS_Sleep(1);
				//if previous spk level is not equal to tone spk level,recover previous spk level when stop tone
				if(spkLevelBackup != audio_cfg.spkLevel)
				{
					CSW_TRACE(CSW_DM_TS_ID, TSTXT("recover previous spk level\n"));
					audio_cfg.spkLevel = spkLevelBackup;
					VOIS_AUDIO_CFG_T voisCfg = 
					{
					    .spkLevel = audio_cfg.spkLevel,
					    .micLevel = audio_cfg.micLevel,
					    .sideLevel = audio_cfg.sideLevel,
					    .toneLevel = audio_cfg.toneLevel,
					    .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
					    .decMute = VPP_SPEECH_UNMUTE
					};
					
					if (vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO)
					{
						AUDIO_DELAY(2);
						vois_Setup(audioItf, &voisCfg);
					}   
				}
#endif
                //hal_audio_SpeakerClose();
                if((Speeching == FALSE)&&(Play_music ==FALSE) )
                {
                 Speaker_Close();
                }
                COS_KillTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY),PRV_DM_AUDIO_TIMER_ID);
                cii_tone_sate = DTMF_IDLE;
                g_playToneAudioTempTime = 0;
                short_time_keytone=0;
                return;

              }


        {
        if (cii_tone_sate==DTMF_PLAYING)    
        {
            finish=FALSE;
            term_num++;
            // check if we finished 
            if(term_num>DTMF_ARRAY[cii_currentDTMF.Frequency]->term_num)
            {
                // increase the duration
                DTMF_duration++;
                if (DTMF_duration<cii_currentDTMF.Duration)
                {
                    term_num = 0 ;
                    
                }
                else
                {
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
          if(FALSE == hal_audio_StopTone()) //changed by CII fix the bug that a tone has no silence in the last of it will cause errors 
          {
            AUDIO_DELAY(2);
            hal_audio_StopTone();
          }
#else
          while (AUD_ERR_NO != aud_ToneStop(audioItf)) //changed by CII fix the bug that a tone has no silence in the last of it will cause errors 
          {
            COS_Sleep(1);
          }
#endif
          //hal_audio_SpeakerClose();
          CSW_TRACE(CSW_DM_TS_ID, TSTXT("      hal_audio_StopTone     \n") );  

          cii_tone_sate = DTMF_IDLE;

          finish=TRUE;
                }
            }

            if (!finish)//changed by CII ~(finish=TRUE)==0xfe
            {
                if (0 == DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone)
                {
                     
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
                    ret = hal_audio_StartTone(cii_currentDTMF.Frequency,cii_currentDTMF.level,&audio_cfg);
                    if(ret == FALSE)
                    {
                      AUDIO_DELAY(2);
                      hal_audio_StartTone(cii_currentDTMF.Frequency,cii_currentDTMF.level,&audio_cfg);
                    }
#else
                    {
                        audio_cfg.toneLevel = cii_currentDTMF.level;
                        AUD_ERR_T err = aud_ToneStart(audioItf, cii_currentDTMF.Frequency, &audio_cfg);
                        if (err != AUD_ERR_NO)
                        {
                            AUDIO_DELAY(2);
                            aud_ToneStart(audioItf, cii_currentDTMF.Frequency, &audio_cfg);
                        }
                    }

#endif
        }
                else if (HAL_AUDIO_SILENCE == DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone)
                {               
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
                    hal_audio_PauseTone();
#else
                    aud_TonePause(audioItf, TRUE);
#endif
                    
                }
                else
                {
                    
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
                    ret = hal_audio_StartTone(DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone,cii_currentDTMF.level,&audio_cfg);
          if(ret == FALSE)
          {
            AUDIO_DELAY(2);
            hal_audio_StartTone(DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone,cii_currentDTMF.level,&audio_cfg);
          }
#else
                    {
                        audio_cfg.toneLevel = cii_currentDTMF.level;
                        AUD_ERR_T err = aud_ToneStart(audioItf, DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone,
                                                    &audio_cfg);
                        if (err != AUD_ERR_NO)
                        {
                            AUDIO_DELAY(2);
                            aud_ToneStart(audioItf, DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone,
                                                    &audio_cfg);
                        }
                    }
#endif
        }
                
                tick_time = (DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone_term )MILLI_SECOND;//changed by Shawn fix timer use stack define 2005-3-20
                g_playToneAudioTempTime += tick_time ;
                COS_SetTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,PRV_DM_AUDIO_TIMER_ID, COS_TIMER_MODE_SINGLE,tick_time); 

            }
        

        }
        
    }
    }
    if(pEvent->nEventId==SYS_EV_MASK_AUDIO_TONE)
    {
    // play the firt one term 
    switch(pEvent->nParam2) 
    {
        case CII_TONE_END:
            if (DTMF_PLAYING==cii_tone_sate)
            {
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
                if(FALSE == hal_audio_StopTone())
                {
          AUDIO_DELAY(2);
          hal_audio_StopTone();
                }
#else
                while (aud_ToneStop(audioItf) != AUD_ERR_NO)
                {
                    COS_Sleep(1);;
                }

				//if previous spk level is not equal to tone spk level,recover previous spk level when stop tone
				if(spkLevelBackup != audio_cfg.spkLevel)
				{
					CSW_TRACE(CSW_DM_TS_ID, TSTXT("recover previous spk level\n"));
					audio_cfg.spkLevel = spkLevelBackup;
					VOIS_AUDIO_CFG_T voisCfg = 
					{
					    .spkLevel = audio_cfg.spkLevel,
					    .micLevel = audio_cfg.micLevel,
					    .sideLevel = audio_cfg.sideLevel,
					    .toneLevel = audio_cfg.toneLevel,
					    .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
					    .decMute = VPP_SPEECH_UNMUTE
					};
					
					if (vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO)
					{
						AUDIO_DELAY(2);
						vois_Setup(audioItf, &voisCfg);
					}   
				}
#endif
                //hal_audio_SpeakerClose();  
                Speaker_Close();
                COS_KillTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY),PRV_DM_AUDIO_TIMER_ID);
           
                cii_tone_sate=DTMF_IDLE ;
                CSW_TRACE(CSW_DM_TS_ID, TSTXT("    the long  time tone is over by playstop()  \n") );  

            }
            
            break;
        default :
            //restart a  dtmf tone 

            if (DTMF_PLAYING == cii_tone_sate)
            {
                //hal_audio_StopTone();
                COS_KillTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY),PRV_DM_AUDIO_TIMER_ID);
            
            }

            g_playToneAudioTempTime = 0;
            gTone_Audio_Total_time = pEvent->nParam1;
            cii_currentDTMF.Duration=100;//pEvent->nParam1;
            cii_currentDTMF.Frequency=pEvent->nParam2;
            cii_currentDTMF.level=(pEvent->nParam3 & 0xFFFF0000)>>16;
			SpeakerLevel = pEvent->nParam3 & 0x0000FFFF;

            DM_SetToneVolume(SpeakerLevel);//set spk level for tone play
        
            cii_tone_sate = DTMF_PLAYING;
            term_num = 0 ;
            DTMF_duration =0 ;
            //play the first term of a DTMF
            CSW_TRACE(CSW_DM_TS_ID, TSTXT("    play one tone the playtime = %d,cii_currentDTMF.level=%d,SpeakerLevel=%d \n"),gTone_Audio_Total_time*1000/16384,cii_currentDTMF.level,SpeakerLevel );  

            if(gTone_Audio_Total_time>=((100)MILLI_SECOND)&&gTone_Audio_Total_time<=((300)MILLI_SECOND))
            {
          CSW_TRACE(CSW_DM_TS_ID, TSTXT("      start play the short tone  time=%d      \n") ,gTone_Audio_Total_time*1000/16384);  

            short_time_keytone=1;
            }
            else
            {
            short_time_keytone=0;

            }
			

                if(0 == DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone)
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
                {
                    if(FALSE == hal_audio_StartTone(cii_currentDTMF.Frequency,cii_currentDTMF.level,&audio_cfg))
                    {
            AUDIO_DELAY(2);
            hal_audio_StartTone(cii_currentDTMF.Frequency,cii_currentDTMF.level,&audio_cfg);
                    }
                
                }
                else
                {
                    if(FALSE == hal_audio_StartTone(DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone,cii_currentDTMF.level,&audio_cfg))
                    {
            AUDIO_DELAY(2);
            hal_audio_StartTone(DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone,cii_currentDTMF.level,&audio_cfg);
                    }
                    
                }
#else
                {
                    audio_cfg.toneLevel = cii_currentDTMF.level;
                    AUD_ERR_T err = aud_ToneStart(audioItf, cii_currentDTMF.Frequency,
                                                &audio_cfg);
                    if (err != AUD_ERR_NO)
                    {
                        AUDIO_DELAY(2);
                        aud_ToneStart(audioItf, cii_currentDTMF.Frequency,
                                                &audio_cfg);
                    }
                }
                else
                {
                    audio_cfg.toneLevel = cii_currentDTMF.level;
                    AUD_ERR_T err = aud_ToneStart(audioItf, DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone,
                                                &audio_cfg);
                    if (err != AUD_ERR_NO)
                    {
                        AUDIO_DELAY(2);
                        aud_ToneStart(audioItf, DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone,
                                                &audio_cfg);
                    }
                }
#endif
        if(short_time_keytone==0)
            {
                tick_time = (DTMF_ARRAY[pEvent->nParam2 ]->DTMF_term[term_num].tone_term)MILLI_SECOND;//changed by Shawn fix timer use stack define 2005-3-20
                g_playToneAudioTempTime += tick_time ;
                COS_SetTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,PRV_DM_AUDIO_TIMER_ID, COS_TIMER_MODE_SINGLE,tick_time); 
            }
            else
            {
                COS_SetTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,PRV_DM_AUDIO_TIMER_ID, COS_TIMER_MODE_SINGLE,gTone_Audio_Total_time); 
            }
            
            break;
    }
   }

        CSW_TRACE(CSW_DM_TS_ID, TSTXT("        the play time tone is =%d    \n"),g_playToneAudioTempTime*1000/16384 );  

    if( g_playToneAudioTempTime >= gTone_Audio_Total_time)
        {
            if(short_time_keytone==0)
            { 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
            if(FALSE == hal_audio_StopTone())
            {
          AUDIO_DELAY(2);
          hal_audio_StopTone();
            }
#else
            while (aud_ToneStop(audioItf) != AUD_ERR_NO)
            {
                COS_Sleep(1);;
            }
#endif
                //hal_audio_SpeakerClose();  // in the speeching , speaker shouldn't be closed 
                COS_KillTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY),PRV_DM_AUDIO_TIMER_ID);
                cii_tone_sate = DTMF_IDLE;
                g_playToneAudioTempTime = 0;
                CSW_TRACE(CSW_DM_TS_ID, TSTXT("        the long time tone is over    \n") );  
            }
        }

    if(pEvent->nEventId==SYS_EV_MASK_AUDIO_TONE)
    {
        DM_NotifyToneOperationDone();
    }
}
/*
================================================================================
Function   : audio_ParseEvt
--------------------------------------------------------------------------------
Scope      : 
Parameters : pointer on the received event (evt[SXR_EVT_MBX_SIZE])
Return     :
================================================================================
*/





BOOL DM_AudioInit (DM_SpeakerGain SpeakerGain,DM_MicGain MicGain,UINT8 Audio_mode) 
{
    if((SpeakerGain > DM_AUDIO_SPK_MUTE )||(MicGain > DM_AUDIO_MIC_MUTE)||(Audio_mode > 2))
        return FALSE;
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("   DM_AudioInit  \n"));
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    audio_cfg.spkGain = SpeakerGain;
    audio_cfg.micGain = MicGain;
    audio_cfg.micSel = Audio_mode;
    audio_cfg.spkSel= Audio_mode;
#else
    audio_cfg.spkLevel = SpeakerGain;
    audio_cfg.micLevel = MicGain;
    audioItf = Audio_mode;
#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
    mps_Open();
#endif
#endif
    gAudio_mode = Audio_mode;

    return TRUE;
}



/*
================================================================================
Function   : DM_SpeakerOpen
--------------------------------------------------------------------------------
Scope      : Open the speaker.
Parameters : none
Return     : Always success.
================================================================================
*/
BOOL DM_SpeakerOpen(void)
{
    //hal_audio_SpeakerOpen();
    Speaker_Open();
    return TRUE;
}



BOOL DM_MicOpen(void)
{
    //hal_audio_MicOpen();
    Mic_open();
    Delay_for_audio(1);

    return TRUE;
}


/*
================================================================================
Function   : DM_SpeakerClose
--------------------------------------------------------------------------------
Scope      : Close the speaker.
Parameters : none
Return     : Always success.
================================================================================
*/
BOOL DM_SpeakerClose(void)
{
    //hal_audio_SpeakerClose();
    Speaker_Close();
    return TRUE;
}



BOOL DM_MicClose(void)
{
    //hal_audio_MicClose();
    Mic_close();
    return TRUE;
}


#define DM_SetSpeakerGain DM_SetAudioVolume
#if 0
BOOL DM_SetSpeakerGain ( DM_SpeakerGain nGain)
{
    BOOL ret=FALSE;
    if(nGain>DM_AUDIO_SPK_MUTE)
    {
        return FALSE;
    }
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    audio_cfg.spkGain = nGain;
    ret = hal_audio_Config(&audio_cfg);
  if(FALSE == ret)
  {
    AUDIO_DELAY(2);
    ret = hal_audio_Config(&audio_cfg);
  }
#else
    AUD_ERR_T err;
    audio_cfg.spkLevel = nGain;
    err = aud_Setup(audioItf, &audio_cfg);
    if (err != AUD_ERR_NO)
    {
        AUDIO_DELAY(2);
        err = aud_Setup(audioItf, &audio_cfg);
        ret = (err == AUD_ERR_NO);
    }
#endif
    Delay_for_audio(1);
    return ret;
    
}
#endif

BOOL DM_SetMicGain ( DM_MicGain nGain)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_SetAudioVolume\n"));
    
    CSW_PROFILE_FUNCTION_ENTER(DM_SetAudioVolume);
    
    COS_EVENT ev = { EV_DM_SET_MIC_GAIN_IND,nGain };
    
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    
    CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
    
    return TRUE;


}
BOOL DM_SetMicGainEX ( DM_MicGain nGain)
{
    BOOL ret=FALSE;
    if(nGain>DM_AUDIO_MIC_MUTE)
    {
        return FALSE;
    }
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    audio_cfg.micGain = nGain;
    ret = hal_audio_Config(&audio_cfg);
  if(FALSE == ret)
  {
    AUDIO_DELAY(2);
    ret = hal_audio_Config(&audio_cfg);
  }
#else
    AUD_ERR_T err;
    audio_cfg.micLevel = nGain;
    err = aud_Setup(audioItf, &audio_cfg);
    if (err != AUD_ERR_NO)
    {
        AUDIO_DELAY(2);
        err = aud_Setup(audioItf, &audio_cfg);
        ret = (err == AUD_ERR_NO);
    }
#endif
    Delay_for_audio(1);
    return ret; 
}

#define DM_SetSideGain DM_SetAudioVolume

#if 0
BOOL DM_SetSideGain ( DM_SideToneGain nGain)
{
    BOOL ret=FALSE;
    if(nGain>DM_AUDIO_SIDE_6dB)
    {
        return FALSE;
    }
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    audio_cfg.sideGain = nGain;
    ret =  hal_audio_Config(&audio_cfg);
  if(FALSE == ret)
  {
    AUDIO_DELAY(2);
    ret =  hal_audio_Config(&audio_cfg);
  }
#else
    AUD_ERR_T err;
    audio_cfg.sideLevel = nGain;
    err = aud_Setup(audioItf, &audio_cfg);
    if (err != AUD_ERR_NO)
    {
        AUDIO_DELAY(2);
        err = aud_Setup(audioItf, &audio_cfg);
        ret = (err == AUD_ERR_NO);
    }
#endif
    Delay_for_audio(1);
    return ret; 
}
#endif

/* ********   the follow is the  api supported for the csw  **********************************/


/*The DM_SetAudioMode function set the audio mode required for the connected equipment. 

*/ 
//#define DM_AUDIO_MODE_AUX                    0   // earpiece   
//#define DM_AUDIO_MODE_MAIN                1   //main speaker  
//#define DM_AUDIO_MODE_LOUD                2   // louder speaker  


BOOL  SRVAPI DM_SetAudioMode_From_Calibration(UINT8 nAudioMode)
{       
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    CSW_PROFILE_FUNCTION_ENTER(DM_SetAudioMode_From_Calibration);

    enum hal_SpeakerSel halMode;
    struct hal_AudioCalib *audioCalib;
    BOOL ret = FALSE;
    /* Check the volume argument. */
    if (nAudioMode > 2)
    {
        CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioMode_From_Calibration);
        return FALSE;
    }
    /* Configure the mode. */
    if (nAudioMode == DM_AUDIO_MODE_EARPIECE)
    {
        /* Disable the polyphonic chip. */

        /* Change the audio mode to Handset. */
        audio_cfg.spkSel = HAL_AUDIO_SPK;
        audio_cfg.micSel = HAL_AUDIO_MIC;

        /* Set the mode to pass to HAL to the right value. */
        halMode = HAL_AUDIO_SPK;
    }
    else if (nAudioMode == DM_AUDIO_MODE_HANDSET)
    {
        /* Disable the polyphonic chip. */

        /* Change the audio mode to Earpiece. */
        audio_cfg.spkSel = HAL_AUDIO_SPK_AUX;
        audio_cfg.micSel = HAL_AUDIO_MIC_AUX;

        /* Set the mode to pass to HAL to the right value. */
        halMode = HAL_AUDIO_SPK_AUX;
    }
    else if (nAudioMode == DM_AUDIO_MODE_LOUDSPEAKER)
    {
        /* Change the audio mode to Loudspeaker. */
        audio_cfg.spkSel = HAL_AUDIO_SPK_LOUD;
        audio_cfg.micSel = HAL_AUDIO_MIC_LOUD;

        /* Set the mode to pass to HAL to the right value. */
        halMode = HAL_AUDIO_SPK_LOUD;

        /* Enable the polyphonic chip. */
    }
    else /* Invalid parameter! */
    {
        CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioMode_From_Calibration);
        return FALSE;
    }

    /* Set the pointer to the calibrated audio parameters. */
    audioCalib = hal_audio_GetCalib(halMode);

    /* Update the gain of the polyphonic chip, if needed. */
    if (nAudioMode == DM_AUDIO_MODE_LOUDSPEAKER)
    {
    }
    
    /* Configure the calibrated gains. */
    audio_cfg.micGain  = audioCalib->micGain;   
    audio_cfg.spkGain  = audioCalib->spkGain[g_speaker_vlume];
    audio_cfg.sideGain = audioCalib->sideGain[g_speaker_vlume];

    /* Update the audio parameters in the drivers. */
    ret = hal_audio_Config(&audio_cfg);
    if (FALSE == ret)
    {
    AUDIO_DELAY(2);
    ret = hal_audio_Config(&audio_cfg);

    if (FALSE == ret)
    {
      CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioMode_From_Calibration);
      return FALSE ; 
    }
    }
        Delay_for_audio(1);

    //SXS_TRACE (FW_UH_TRC, "###     CII_SetAudioConfig over ####");

  CSW_TRACE(CSW_DM_TS_ID, TSTXT(" set audio mode From Calibration = %d "),nAudioMode); 

    gAudio_mode=nAudioMode;
  CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioMode_From_Calibration);
    return TRUE;
#else
    // AUD integrates all that already
    return TRUE;
#endif    
        
}

BOOL  SRVAPI DM_SetAudioMode(UINT8 nAudioMode)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_SetAudioMode\n"));
    
    CSW_PROFILE_FUNCTION_ENTER(DM_SetAudioVolume);
    
    COS_EVENT ev = { EV_DM_SET_AUDIO_MODE_IND,nAudioMode };
    
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    
    CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
    
    return TRUE;

}

/*The DM_SetAudioMode function set the audio mode required for the connected equipment. 

*/ 
//#define DM_AUDIO_MODE_AUX                    0   // earpiece   
//#define DM_AUDIO_MODE_MAIN                1   //main speaker  
//#define DM_AUDIO_MODE_LOUD                2   // louder speaker  

BOOL  SRVAPI DM_SetAudioModeEX(UINT8 nAudioMode)

{       
    CSW_PROFILE_FUNCTION_ENTER(DM_SetAudioMode);

    /* Check the volume argument. */
    if (nAudioMode > 3) //guohui add blue tooth
    {
        CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioMode);
        return FALSE;
    }
    /* Configure the mode. */
    switch(nAudioMode)
    {
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    case DM_AUDIO_MODE_EARPIECE:
        /* Disable the polyphonic chip. */
        /* Change the audio mode to Handset. */
        audio_cfg.spkSel = HAL_AUDIO_SPK;
        audio_cfg.micSel = HAL_AUDIO_MIC;
        break;
    case DM_AUDIO_MODE_HANDSET:
        /* Disable the polyphonic chip. */
        /* Change the audio mode to Earpiece. */
        audio_cfg.spkSel = HAL_AUDIO_SPK_AUX;
        audio_cfg.micSel = HAL_AUDIO_MIC_AUX;
        break;
    case DM_AUDIO_MODE_LOUDSPEAKER:
        /* Change the audio mode to Loudspeaker. */
        audio_cfg.spkSel = HAL_AUDIO_SPK_LOUD;
        audio_cfg.micSel = HAL_AUDIO_MIC_LOUD;
        break;
#else
#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
    case DM_AUDIO_MODE_EARPIECE:
        audioItf = SND_ITF_RECEIVER;
        break;
    case DM_AUDIO_MODE_HANDSET:
        audioItf = SND_ITF_EAR_PIECE;
        break;
    case DM_AUDIO_MODE_LOUDSPEAKER:
        audioItf = SND_ITF_LOUD_SPEAKER;
	 break;
	 //guoh add new audio path
    case DM_AUDIO_MODE_BLUETOOTH:
	 audioItf = SND_ITF_BLUETOOTH;
        break;
#else
    case DM_AUDIO_MODE_EARPIECE:
        audioItf = AUD_ITF_RECEIVER;
        break;
    case DM_AUDIO_MODE_HANDSET:
        audioItf = AUD_ITF_EAR_PIECE;
        break;
    case DM_AUDIO_MODE_LOUDSPEAKER:
        audioItf = AUD_ITF_LOUD_SPEAKER;
	 break;
	 //guoh add new audio path
    case DM_AUDIO_MODE_BLUETOOTH:
	 audioItf = AUD_ITF_BLUETOOTH;	
        break;
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */
#endif
    default:
        CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioMode);
        return FALSE;
    }


    /* Update the audio parameters in the drivers. */
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    if (FALSE == hal_audio_Config(&audio_cfg))
    {
        AUDIO_DELAY(2);
        if (FALSE == hal_audio_Config(&audio_cfg))
        {
          CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioMode);
          return FALSE ;
        }
    }
#else
#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
    audioStreamConfig.audioPath = audioItf;
    if(audioHandle)
    {
        mps_SetConfig(audioHandle, &audioStreamConfig);
    }
#else

   if(Speeching == TRUE)
   {
   	    VOIS_AUDIO_CFG_T voisCfg = 
                {
                    .spkLevel = audio_cfg.spkLevel,
                    .micLevel = audio_cfg.micLevel,
                    .sideLevel = audio_cfg.sideLevel,
                    .toneLevel = audio_cfg.toneLevel,
                    .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
                    .decMute = VPP_SPEECH_UNMUTE
                };
    while(vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO);
	
   }else{
   
	       while (aud_Setup(audioItf, &audio_cfg) != AUD_ERR_NO)
           {
              COS_Sleep(1);
	       }
   }
   
#endif  /* CSW_EXTENDED_API_AUDIO_VIDEO */
#endif
    Delay_for_audio(1);


  CSW_TRACE(CSW_DM_TS_ID, TSTXT(" set audio mode =%d"),nAudioMode); 

    gAudio_mode=nAudioMode;
    CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioMode);
    return TRUE;
        
}



/*The CFW_GetAudioMode function select the audio hardware set.*/
BOOL SRVAPI DM_GetAudioMode  (UINT8* pAudioMode)
{
    *pAudioMode=gAudio_mode;
     return TRUE;   
}


BOOL SRVAPI  DM_GetSpeakerVolume (UINT8* pVolume)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    *pVolume=audio_cfg.spkGain;
#else
    *pVolume=audio_cfg.spkLevel;
#endif
    return TRUE;

}


/*The DM_GetLoudspeakerVolume function retrieves the loudspeaker volume level of the phone.
*/

BOOL SRVAPI  DM_GetSpeakerVolume_From_Calibration(UINT8* pVolume)
{
    *pVolume=g_speaker_vlume;
    return TRUE;

}


/*This function adjust speaker volume for voice call purpose.

*/
BOOL SRVAPI  DM_SetAudio_From_Calibration_Volume (DM_SpeakerGain nVolume)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    CSW_PROFILE_FUNCTION_ENTER(DM_SetAudio_From_Calibration_Volume);
  CSW_TRACE(CSW_DM_TS_ID, TSTXT("   DM_SetAudio_From_Calibration_Volume =%d \n"),nVolume);  
    enum hal_SpeakerSel halMode;
    struct hal_AudioCalib *audioCalib;

    /* Check the volume argument. */
    if (nVolume > DM_AUDIO_SPK_MUTE)
    {
    CSW_PROFILE_FUNCTION_EXIT(DM_SetAudio_From_Calibration_Volume);
        return FALSE;
    }

    /* Configure the mode. */
    if (gAudio_mode == DM_AUDIO_MODE_EARPIECE)
    {
        /* Disable the polyphonic chip. */
        //CII_MelodyClose();

        /* Change the audio mode to Handset. */
        audio_cfg.spkSel = HAL_AUDIO_SPK;
        audio_cfg.micSel = HAL_AUDIO_MIC;

        /* Set the mode to pass to HAL to the right value. */
        halMode = HAL_AUDIO_SPK;
    }
    else if (gAudio_mode == DM_AUDIO_MODE_HANDSET)
    {
        /* Disable the polyphonic chip. */
        //CII_MelodyClose();

        /* Change the audio mode to Earpiece. */
        audio_cfg.spkSel = HAL_AUDIO_SPK_AUX;
        audio_cfg.micSel = HAL_AUDIO_MIC_AUX;

        /* Set the mode to pass to HAL to the right value. */
        halMode = HAL_AUDIO_SPK_AUX;
    }
    else if (gAudio_mode == DM_AUDIO_MODE_LOUDSPEAKER)
    {
        /* Change the audio mode to Loudspeaker. */
        audio_cfg.spkSel = HAL_AUDIO_SPK_LOUD;
        audio_cfg.micSel = HAL_AUDIO_MIC_LOUD;

        /* Set the mode to pass to HAL to the right value. */
        halMode = HAL_AUDIO_SPK_LOUD;

        /* Enable the polyphonic chip. */
        //CII_MelodyOpen();
    }
    else /* Invalid parameter! */
    {
        CSW_PROFILE_FUNCTION_EXIT(DM_SetAudio_From_Calibration_Volume);
        return FALSE;
    }

    /* Set the pointer to the calibrated audio parameters. */
    audioCalib = hal_audio_GetCalib(halMode);

    /* Update the gain of the polyphonic chip, if needed. */
    if (gAudio_mode == DM_AUDIO_MODE_LOUDSPEAKER)
    {
        ///CII_MelodyVolume_Manager(1, audioCalib->amplGain[nVolume]);
    }
    
    /* Configure the calibrated gains. */
    audio_cfg.micGain  = audioCalib->micGain;   
    audio_cfg.spkGain  = audioCalib->spkGain[nVolume];
    audio_cfg.sideGain = audioCalib->sideGain[nVolume];
  CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### audio_cfg.spkGain =%d   \n") ,audio_cfg.spkGain);  

    /* Update the audio parameters in the drivers. */
    if (FALSE == hal_audio_Config(&audio_cfg))
    {
    AUDIO_DELAY(2);
    if (FALSE == hal_audio_Config(&audio_cfg))
    {
      CSW_PROFILE_FUNCTION_EXIT(DM_SetAudio_From_Calibration_Volume);
      return FALSE;
    }
    }
    Delay_for_audio(1);

    g_speaker_vlume = nVolume ;
    CSW_PROFILE_FUNCTION_EXIT(DM_SetAudio_From_Calibration_Volume);
    return TRUE;
#else
    // AUD already use the calibrated values
    return TRUE;
#endif
}
BOOL SRVAPI  DM_SetAudioVolume (DM_SpeakerGain nVolume)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_SetAudioVolume\n"));
    
    CSW_PROFILE_FUNCTION_ENTER(DM_SetAudioVolume);
    
    COS_EVENT ev = { EV_DM_SET_AUDIO_VOLUME_IND,nVolume };
    
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    
    CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
    
    return TRUE;

}

/*This function adjust speaker volume for voice call purpose.

*/
BOOL SRVAPI  DM_SetAudioVolumeEX (DM_SpeakerGain nVolume)
{
    CSW_PROFILE_FUNCTION_ENTER(DM_SetAudioVolume);

    /* Check the volume argument. */
    if(nVolume>DM_AUDIO_SPK_6dB)
    {
      nVolume = DM_AUDIO_SPK_6dB;
      //CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
      //return FALSE;
    }

	if (DM_GetSPKMuteFlag())
	{
		g_spk_gain = nVolume;
		CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
		return TRUE;
	}

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)

    audio_cfg.spkGain  = nVolume;
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### audio_cfg.spkGain =%d   \n") ,audio_cfg.spkGain);  

    /* Update the audio parameters in the drivers. */
    if (FALSE == hal_audio_Config(&audio_cfg))
    {
      AUDIO_DELAY(2);
      if (FALSE == hal_audio_Config(&audio_cfg))
      {
        CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
        return FALSE;
      }
    }
#else
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### nVolume =%d   \n"), nVolume);

#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
    audioStreamConfig.volume = nVolume;
    if(audioHandle)
    {
        mps_SetConfig(audioHandle, &audioStreamConfig);
    }
#else
    audio_cfg.spkLevel  = nVolume;
	//save previous spk level,spkLevelBackup is used to recover the previous spk level
	spkLevelBackup = nVolume;
	
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### audio_cfg.spkLevel =%d,Speeching=%d,spkLevelBackup=%d\n") ,audio_cfg.spkLevel,Speeching,spkLevelBackup);  

    /* Update the audio parameters in the drivers. */
   if(Speeching == TRUE){
       VOIS_AUDIO_CFG_T voisCfg = 
            {
                    .spkLevel = audio_cfg.spkLevel,
                    .micLevel = audio_cfg.micLevel,
                    .sideLevel = audio_cfg.sideLevel,
                    .toneLevel = audio_cfg.toneLevel,
                    .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
                    .decMute = VPP_SPEECH_UNMUTE
              };
      if (vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO)
      {
        AUDIO_DELAY(2);
        if (AUD_ERR_NO != vois_Setup(audioItf, &voisCfg))
        {
          CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
          return FALSE;
        }
      }    
    }
   else if (cii_tone_sate == DTMF_IDLE) {
     if (aud_Setup(audioItf, &audio_cfg) != AUD_ERR_NO)
     {
       AUDIO_DELAY(2);
       if (AUD_ERR_NO != aud_Setup(audioItf, &audio_cfg))
       {
         CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
         return FALSE;
       }
     }
   }
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */

#endif
    Delay_for_audio(3);

    CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
    return TRUE;
}

# if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
VOID record_end_Handler(UINT16 status)
{
    COS_EVENT ev;

    CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####     record_end_Handler    \n"));  
    ev.nEventId = EV_CSW_PRV_RECORD_END;
    //  ev.nParam1  = number;
    COS_SendSysEvent(&ev, 0);
}
#else
    // ARS use event passing
    // These DM functions are not used by the MMI
    // and thus not maintained. Use MRS/MPS instead
    // through MCI if needed.
#endif


# if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
VOID stream_end_Handler(UINT16 status)
{
    COS_EVENT ev;

    CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####               stream_end_Handler    \n"));  
    ev.nEventId = EV_CSW_PRV_STREAM_END;
    COS_SendSysEvent(&ev, 0);
    //#ifdef c1000
    Play_music = FALSE;
    //#endif
}

#else
#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
VOID stream_end_Handler(MPS_HANDLE_T handle, MPS_STATE_T state)
{
    COS_EVENT ev;

    if(state == MPS_STATE_EOF)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####               stream_end_Handler    \n"));  
        ev.nEventId = EV_CSW_PRV_STREAM_END;
        COS_SendSysEvent(&ev, 0);
        Play_music = FALSE;
    }
}
#else
VOID stream_end_Handler(APS_STATUS_T status)
{
    if (status == APS_STATUS_END_BUFFER_REACHED)
    {
        COS_EVENT ev;

        CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####               stream_end_Handler    \n"));  
        ev.nEventId = EV_CSW_PRV_STREAM_END;
        COS_SendSysEvent(&ev, 0);
        Play_music = FALSE;
    }
}
#endif
#endif

/*
================================================================================
Function   : CII_MicRecord
--------------------------------------------------------------------------------
Scope      : Start recording a stream.
Parameters : "buffer" is the memory buffer where to record. "uiLen" is the max
number of unsigned int to record.
Return     : ERR_SUCCESS, ERR_DM_INVALID_PARAMETER or ERR_BUSY_DRIVE.
If ERR_DM_INVALID_PARAMETER, the pointer if null, try with an other argument.
If ERR_BUSY_DRIVE, the command cannot be processed now, try later.
================================================================================
*/



BOOL DM_MicStartRecord(UINT32 *pBuffer, UINT32 nBufferSize,DM_PlayRingType codec,BOOL loop_mode)
{
    UINT32 ret_val;
    CSW_PROFILE_FUNCTION_ENTER(DM_MicStartRecord);

    if(pBuffer == NULL)
        return FALSE;
    if((nBufferSize == 0) || (codec > 13))
    {
        return FALSE;
    }
    //hal_audio_MicOpen();
    mic_record = TRUE;
    Mic_open();

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    hal_SetUsrVectorMsk(HAL_AUDIO_IRQ, HAL_AUDIO_END_OF_RECORD);
    hal_SetUsrVector(HAL_AUDIO_IRQ, record_end_Handler);
    ret_val =  hal_audio_StartRecord(pBuffer ,nBufferSize,codec, loop_mode,  &audio_cfg);
#else
    ret_val = TRUE;

#endif
	Delay_for_audio(1);

	CSW_PROFILE_FUNCTION_EXIT(DM_MicStartRecord);
	if(ret_val)
		return TRUE;
	else
		return FALSE;
}

/*
================================================================================
Function   : CII_MicStopRecord
--------------------------------------------------------------------------------
Scope      : Stop recording a stream.
Parameters : none
Return     : ERR_SUCCESS or ERR_BUSY_DRIVE.
If ERR_BUSY_DRIVE, the command cannot be processed now, try later.
================================================================================
*/
BOOL DM_MicStopRecord(void)
{
    mic_record = FALSE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    if(FALSE == hal_audio_StopRecord())
    {
      AUDIO_DELAY(2);
      
          return hal_audio_StopRecord();
    }
#else
#ifndef CSW_EXTENDED_API_MRS
    if (ars_Stop() != ARS_ERR_NO)
    {
        AUDIO_DELAY(2);
        return (ars_Stop() == ARS_ERR_NO);
    }
#endif
#endif
    return TRUE;
}

VOID DM_SetSPKMuteFlag(BOOL muteflag)
{
	isSPKMute = muteflag;
}
BOOL DM_GetSPKMuteFlag()
{
	return isSPKMute;
}
BOOL DM_MuteSpeaker(VOID)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT(" csw DM_MuteSpeaker \n"));
    DM_SetSPKMuteFlag(TRUE);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    g_spk_gain = audio_cfg.spkGain;
    audio_cfg.spkGain = HAL_AUDIO_SPK_MUTE;
    isSPKMute = TRUE;
    return hal_audio_Config(&audio_cfg);
#else
    g_spk_gain = audio_cfg.spkLevel;
    audio_cfg.spkLevel = AUD_SPK_MUTE;
#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
    return (aud_Setup(audioItf, &audio_cfg) == AUD_ERR_NO);
#else
    APS_AUDIO_CFG_T apsAudioCfg = 
            {
                .spkLevel = audio_cfg.spkLevel,
                .filter = NULL
            };

        return (aps_Setup(audioItf,&apsAudioCfg) == AUD_ERR_NO);
       // return (aud_Setup(audioItf, &audio_cfg) == AUD_ERR_NO);
#endif
#endif
}

BOOL DM_ResumSpeaker(VOID)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT(" csw DM_ResumSpeaker \n"));
    DM_SetSPKMuteFlag(FALSE);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    audio_cfg.spkGain = g_spk_gain;
  
    return hal_audio_Config(&audio_cfg);
#else
    audio_cfg.spkLevel = g_spk_gain;
#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
    return (aud_Setup(audioItf, &audio_cfg) == AUD_ERR_NO);
#else
    APS_AUDIO_CFG_T apsAudioCfg = 
            {
                .spkLevel = audio_cfg.spkLevel,
                .filter = NULL
            };
        return (aps_Setup(audioItf,&apsAudioCfg) == AUD_ERR_NO);
        //return (aud_Setup(audioItf, &audio_cfg) == AUD_ERR_NO);
#endif
#endif
}

BOOL dm_MuteMic(VOID)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
  g_mic_gain = audio_cfg.micGain;
    audio_cfg.micGain = HAL_AUDIO_MIC_MUTE;
    
    return hal_audio_Config(&audio_cfg);
#else
    g_mic_gain = audio_cfg.micLevel;
    audio_cfg.micLevel = AUD_MIC_MUTE;
   if(Speeching == TRUE)
   {
   	    VOIS_AUDIO_CFG_T voisCfg = 
                {
                    .spkLevel = audio_cfg.spkLevel,
                    .micLevel = audio_cfg.micLevel,
                    .sideLevel = audio_cfg.sideLevel,
                    .toneLevel = audio_cfg.toneLevel,
                    .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
                    .decMute = VPP_SPEECH_UNMUTE
                };
    while(vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO);
	  return TRUE;
   }
   else
   {

    
    return (aud_Setup(audioItf, &audio_cfg) == AUD_ERR_NO);
   }
#endif
}

BOOL dm_ResumMic(VOID)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    audio_cfg.micGain = g_mic_gain;
  
    return hal_audio_Config(&audio_cfg);
#else
    audio_cfg.micLevel = g_mic_gain;
   if(Speeching == TRUE)
   {
   	    VOIS_AUDIO_CFG_T voisCfg = 
                {
                    .spkLevel = audio_cfg.spkLevel,
                    .micLevel = audio_cfg.micLevel,
                    .sideLevel = audio_cfg.sideLevel,
                    .toneLevel = audio_cfg.toneLevel,
                    .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
                    .decMute = VPP_SPEECH_UNMUTE
                };
    while(vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO);
	return TRUE;
   }else
   {
    return (aud_Setup(audioItf, &audio_cfg) == AUD_ERR_NO);
   }
#endif
}


//
// Control Audio. 
// Mute Mic .
// 
BOOL DM_SetAudioControl(UINT8 nCmd, UINT16 nValue)
{
    if(DM_AUD_IOCTL_MUTE_MIC == nCmd)
    {
        dm_MuteMic();
    }
    else if(DM_AUD_IOCTL_UNMUTE_MIC == nCmd)
    {
        dm_ResumMic();
    }
    else if(DM_AUD_IOCTL_MUTE_SPK == nCmd)
    {
        DM_MuteSpeaker();
    }
    else if(DM_AUD_IOCTL_UNMUTE_SPK == nCmd)
    {
        DM_ResumSpeaker();
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

//
// Close Speaker and Mic
//
BOOL DM_StopAudio(VOID)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_StopAudio\n"));
    
    CSW_PROFILE_FUNCTION_ENTER(DM_StopAudio);
    
    COS_EVENT ev = { EV_DM_AUDIO_CONTROL_IND,0 };
    
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    
    CSW_PROFILE_FUNCTION_EXIT(DM_StopAudio);
    
    return TRUE;
}

BOOL DM_StopAudioEX(VOID)
{
  COS_EVENT ev;
  CSW_TRACE(CSW_DM_TS_ID, TSTXT(" csw DM_StopAudioEX \n"));
  CSW_PROFILE_FUNCTION_ENTER(DM_StopAudio);
  if (Speeching == FALSE)
    return TRUE;
  Speeching = FALSE;
  #if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    while( !hal_audio_StopSpeech())
  #else
    while (vois_Stop() != VOIS_ERR_NO)
  #endif
  {
    COS_Sleep(1);

  }
  //hal_audio_SpeakerClose();
  Speaker_Close();
  //hal_audio_MicClose();
  Mic_close();
  Delay_for_audio(1);
  audioBusy = 0;
    ev.nEventId = EV_DM_SPEECH_IND;//// when stop call, this ind will send to mmi task
                ev.nParam1  = 0;
                ev.nParam2  = 0;
                ev.nParam3  = 0;
  COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
  CSW_PROFILE_FUNCTION_EXIT(DM_StopAudio);
  return TRUE;
}


BOOL DM_StartAudio(VOID)
{
    CSW_PROFILE_FUNCTION_ENTER(DM_StopAudio);

    COS_EVENT ev ={ EV_DM_AUDIO_CONTROL_IND, 1 };
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_StartAudio\n"));
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    CSW_PROFILE_FUNCTION_EXIT(DM_StopAudio);
    return TRUE;
}

BOOL DM_StartAudioEX(VOID) 
{
   
    BOOL ret = TRUE;
    UINT8 times_delay = 0;
  	COS_EVENT ev;
    CSW_PROFILE_FUNCTION_ENTER(DM_StartAudio);
    CSW_TRACE(CSW_DM_TS_ID, TSTXT(" csw DM_StartAudioEX \n"));

    //hal_audio_SpeakerOpen();
    Speaker_Open();
    //hal_audio_MicOpen();
    Mic_open();
   // hal_audio_StopTone();
    Delay_for_audio(1);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    while( !hal_audio_StartSpeech(&audio_cfg))
#else
    VOIS_AUDIO_CFG_T voisCfg = 
                {
                    .spkLevel = audio_cfg.spkLevel,
                    .micLevel = audio_cfg.micLevel,
                    .sideLevel = audio_cfg.sideLevel,
                    .toneLevel = audio_cfg.toneLevel,
                    .encMute = VPP_SPEECH_UNMUTE,
                    .decMute = VPP_SPEECH_UNMUTE
                };
    while(vois_Start(audioItf, &voisCfg) != VOIS_ERR_NO)
    {
        times_delay++;
        sxr_Sleep(5 * 16384 /1000);
        if (times_delay > 10)
        {
          ret = FALSE;
          break;
        }
    }
#endif
     AUDIO_DELAY(2); 
     Delay_for_audio(5);
    if(ret == TRUE)
    {
     Speeching = TRUE;
     audioBusy = 1;
     ev.nEventId = EV_DM_SPEECH_IND;// when start call, this ind will send to mmi task
                ev.nParam1  = 1;
                ev.nParam2  = 0;
                ev.nParam3  = 0;
                COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    }
    CSW_PROFILE_FUNCTION_EXIT(DM_StartAudio);
    return ret;
}


/*
================================================================================
Function   : DM_Audio_StartStream
--------------------------------------------------------------------------------
Scope      :  Manager the play of stream
Parameters : 
Return     : True or False.
================================================================================
*/

BOOL    DM_Audio_StartStream(UINT32*    buffer, UINT32  len, DM_PlayRingType codec, BOOL loop_mode)
{
 	if(Speeching == TRUE)
	{
 		return TRUE;
	}
	CSW_PROFILE_FUNCTION_ENTER(DM_Audio_StartStream);

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    UINT32 buflength;
#endif
  CSW_TRACE(CSW_DM_TS_ID, TSTXT(" DM_Audio_StartStream \n"));
  Play_music = TRUE;
    Speaker_Open();
    BOOL ret=FALSE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    if(codec == HAL_DICTAPHONE_MP3_RING) 
    return ret;
#endif
#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
    if(len == 0)
#else
    if((len == 0) || (codec > 13))
#endif
    {
        return FALSE;
    }
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    hal_SetUsrVectorMsk(HAL_AUDIO_IRQ, HAL_AUDIO_NO_IRQ);
    hal_SetUsrVectorMsk(HAL_AUDIO_IRQ, HAL_AUDIO_END_OF_STREAM);
    hal_SetUsrVector(HAL_AUDIO_IRQ, stream_end_Handler);
    buflength = len -(len%8); // buffer length must be rate of 8

    ret =  hal_audio_StartStream(buffer, buflength, codec, loop_mode, &audio_cfg);  
  if(ret == FALSE)
  {
    AUDIO_DELAY(2);
    ret =  hal_audio_StartStream(buffer, buflength, codec, loop_mode, &audio_cfg);  
  }
#else
#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
    mps_Open();
    g_audioPlayBufStart   = (UINT32) buffer;
    g_audioPlayBufLen     = len*4;
    if(audioHandle == 0)
    {
        mps_OpenBuffer(&audioHandle, (VOID*)buffer, len*4, stream_end_Handler, &audioStreamConfig);

        if(audioHandle != 0)
        {
            if(mps_Play(audioHandle, (loop_mode) ? 0xFFFF : 0) == MPS_ERR_NO)
            {
                ret = TRUE;
            }
        }
    }
#else
    g_audioPlayBufStart   = (UINT32) buffer;
    g_audioPlayBufLen     = len*4;
  	// Current offset from start
    g_audioPlayBufOffset  = 0;
    APS_ENC_STREAM_T stream;
    APS_ERR_T streamError;
    APS_AUDIO_CFG_T apsAudioCfg = 
            {
                .spkLevel = audio_cfg.spkLevel,
                .filter = NULL
            };
    stream.startAddress = (UINT32)buffer;
    stream.length = len*4; // ? Modem2g apiuses bytes to define buffer
                           // size.
    stream.sampleRate = HAL_AIF_FREQ_8000HZ;
    stream.mode = codec;
    stream.channelNb = HAL_AIF_MONO;
    stream.voiceQuality = TRUE;
    if (loop_mode)
    {
        // In loop mode, No interrupt is expected
        stream.handler = NULL;
    }
    else
    {
        stream.handler = stream_end_Handler;
    }
    streamError = aps_Play(audioItf, &stream, &apsAudioCfg, loop_mode);
    if (streamError != APS_ERR_NO)
    {
        AUDIO_DELAY(2);
        streamError = aps_Play(audioItf, &stream, &apsAudioCfg, loop_mode);
    }
    ret = (streamError == APS_ERR_NO);

#endif
#endif
    Delay_for_audio(1);
    if(ret == TRUE)
     audioBusy = 1;
    CSW_PROFILE_FUNCTION_EXIT(DM_Audio_StartStream);
    return ret;

}

VOID DM_Set_SpeechingFlag(BOOL flag)  // according guojian's advice. for bug10462 
{
    Speeching = flag;
}
BOOL    DM_Audio_StopStream(void)
{
    if(Speeching == TRUE)
   	{
    		return TRUE;
   	}
   
  	CSW_PROFILE_FUNCTION_ENTER(DM_Audio_StopStream);
 
    CSW_TRACE(CSW_DM_TS_ID, TSTXT(" DM_Audio_StopStream \n"));
    BOOL ret=FALSE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    ret = hal_audio_StopStream();
  if(ret == FALSE)
  {
    AUDIO_DELAY(2);
    ret = hal_audio_StopStream();
  }

  hal_SetUsrVectorMsk(HAL_AUDIO_IRQ, HAL_AUDIO_NO_IRQ);
#else
#ifdef CSW_EXTENDED_API_AUDIO_VIDEO
    ret = (MPS_ERR_NO == mps_CloseBuffer(audioHandle));
    audioHandle = 0;
#else
    APS_ERR_T streamError;
    streamError = aps_Stop();
    if (streamError != APS_ERR_NO)
    {
        AUDIO_DELAY(2);
        streamError = aps_Stop();
    }
    ret = (streamError == APS_ERR_NO);
#endif
#endif

    //hal_audio_SpeakerClose();
    Speaker_Close();
    //#ifdef c1000
    Play_music = FALSE;
    //#endif
    Delay_for_audio(1);
    audioBusy = 0;
    CSW_PROFILE_FUNCTION_EXIT(DM_Audio_StopStream);
    return ret;

}

/*This function plays the tone specified.
The caller can request the sound to be repeated, and the time between repeats. 

*/


BOOL SRVAPI DM_PlayTone (UINT8 nToneType, UINT8 nToneAttenuation,UINT16 nDuration,DM_SpeakerGain volumn)
{
    CSW_PROFILE_FUNCTION_ENTER(DM_PlayTone);
    COS_EVENT  mtone;
    BOOL ret=0;
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("   csw2007  DM_PlayTone     \n") );  
    
    /*for c1000, DM_PlayTone will conflict to the DM_Audio_StartStream, so when use the
        DM_Audio_StartStream, the DM_PlayTone must be unused*/
    //#ifdef c1000
    if(TRUE == Play_music)
    {
        return FALSE;
    }
    //#endif
    
    if(nToneType> CII_TONE_END)
    {
        CSW_TRACE(CSW_DM_TS_ID, ("Unknown nToneType in AUDIO\n"));
        CSW_PROFILE_FUNCTION_EXIT(DM_PlayTone);
        return FALSE;
    }
    if(nDuration < 100 )
    {
        CSW_TRACE(CSW_DM_TS_ID, ("Too small  nDuration in AUDIO\n"));
        CSW_PROFILE_FUNCTION_EXIT(DM_PlayTone);
        return FALSE;
    }
    mtone.nEventId=SYS_EV_MASK_AUDIO_TONE;
    mtone.nParam1=(nDuration)MILLI_SECOND;
    mtone.nParam2=nToneType;
    if(nToneAttenuation<=DM_TONE_m15dB && volumn<=DM_AUDIO_SPK_MUTE)
        mtone.nParam3=(nToneAttenuation<<16) | volumn;
    else
    {
        CSW_TRACE(CSW_DM_TS_ID, ("Unknown   nToneAttenuation or volume level in AUDIO\n"));
        CSW_PROFILE_FUNCTION_EXIT(DM_PlayTone);
        return FALSE;
    }

    //hal_audio_SpeakerOpen();
    Speaker_Open();
    ret=COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY),&mtone,COS_WAIT_FOREVER,COS_EVENT_PRI_NORMAL);
    DM_WaitToneOperationDone();
    CSW_PROFILE_FUNCTION_EXIT(DM_PlayTone);
    return TRUE;
}


/*This function stops the current playing tone.

 */
BOOL DM_StopTone (VOID)
{
    CSW_PROFILE_FUNCTION_ENTER(DM_StopTone);
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("     DM_StopTone     \n") );  

    COS_EVENT  mtone;
    //mtone = sxr_Malloc(sizeof (CII_AUDIO_TONE));
    mtone.nEventId=SYS_EV_MASK_AUDIO_TONE;
    //mtone->Frequency=CII_TONE_END;
    mtone.nParam2=CII_TONE_END;
    //sxr_Send (mtone, cii_taskmbx[CII_AUDIO_TASK], SXR_SEND_MSG);
    //hal_audio_SpeakerClose();
    //Speaker_Close();
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&mtone, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    DM_WaitToneOperationDone();
    CSW_PROFILE_FUNCTION_EXIT(DM_StopTone);
    return TRUE;

}



VOID DM_Audio_Side_test(UINT8 mode)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_Audio_Side_test") );  
    
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    if(mode == DM_AUDIO_MODE_HANDSET)
    {
        audio_cfg.micGain=HAL_AUDIO_MIC_27dB;
        audio_cfg.spkGain=HAL_AUDIO_SPK_0dB;
        audio_cfg.sideGain=HAL_AUDIO_SIDE_6dB;
        audio_cfg.micSel=HAL_AUDIO_MIC_AUX;
        audio_cfg.spkSel=HAL_AUDIO_MIC_AUX;
    }
    if(mode == DM_AUDIO_MODE_EARPIECE)
    {
        audio_cfg.micGain=HAL_AUDIO_MIC_27dB;
        audio_cfg.spkGain=HAL_AUDIO_SPK_0dB;
        audio_cfg.sideGain=HAL_AUDIO_SIDE_6dB;
        audio_cfg.micSel=HAL_AUDIO_MIC;
        audio_cfg.spkSel=HAL_AUDIO_MIC;
    }
    if(mode == DM_AUDIO_MODE_LOUDSPEAKER)
    {
        audio_cfg.micGain=HAL_AUDIO_MIC_27dB;
        audio_cfg.spkGain=HAL_AUDIO_SPK_0dB;
        audio_cfg.sideGain=HAL_AUDIO_SIDE_6dB;
        audio_cfg.micSel=HAL_AUDIO_MIC_LOUD;
        audio_cfg.spkSel=HAL_AUDIO_MIC_LOUD;
    }
    while (!hal_audio_StopStream());
    while (!hal_audio_StopRecord());
    while (!hal_audio_StopSpeech());
    //hal_audio_MicOpen();
    Mic_open();
  //hal_audio_SpeakerOpen();
    Speaker_Open();
    hal_audio_SideTest(TRUE, &audio_cfg);
    
#else // ! (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)

    // Local variable passed to a function, but copied in the function.
    HAL_AIF_STREAM_T sideAudioStream;

    if (mode == DM_AUDIO_MODE_LOUDSPEAKER)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_Audio_Side_test mode DM_AUDIO_MODE_LOUDSPEAKER") );
        audio_cfg.micLevel  = AUD_MIC_ENABLE;
        audio_cfg.spkLevel  = AUD_SPK_VOL_7;
        audio_cfg.sideLevel = AUD_SIDE_VOL_TEST;
        audioItf            = AUD_ITF_LOUD_SPEAKER;
    }
    else if (mode == DM_AUDIO_MODE_EARPIECE)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_Audio_Side_test mode DM_AUDIO_MODE_EARPIECE") );
        audio_cfg.micLevel  = AUD_MIC_ENABLE;
        audio_cfg.spkLevel  = AUD_SPK_VOL_7;
        audio_cfg.sideLevel = AUD_SIDE_VOL_TEST;
        audioItf            = AUD_ITF_EAR_PIECE;
    }
    else // (mode == DM_AUDIO_MODE_HANDSET)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_Audio_Side_test mode DM_AUDIO_MODE_HANDSET") );
        audio_cfg.micLevel  = AUD_MIC_ENABLE;
        audio_cfg.spkLevel  = AUD_SPK_VOL_7;
        audio_cfg.sideLevel = AUD_SIDE_VOL_TEST;
        audioItf            = AUD_ITF_RECEIVER;
    }
    
    sideAudioStream.startAddress = NULL;
    sideAudioStream.length       = 0;
    sideAudioStream.sampleRate   = HAL_AIF_FREQ_8000HZ;
    sideAudioStream.channelNb    = HAL_AIF_MONO;
    sideAudioStream.voiceQuality = TRUE;
    sideAudioStream.halfHandler  = NULL;
    sideAudioStream.endHandler   = NULL;

    aud_TestModeSetup(audioItf, &sideAudioStream, &audio_cfg, AUD_TEST_SIDE_TEST);
    
#endif // ! (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
}



#ifndef CSW_EXTENDED_API_AUDIO_VIDEO

BOOL DM_GetStreamStatus(VOID)
{
    return Play_music;
}

BOOL DM_GetAudioStatus(VOID)
{
    return audioBusy;
}

#endif

/*
================================================================================
Function   : DM_Audio_RemainingStream
--------------------------------------------------------------------------------
Scope      : Returns the number of words yet to be played in the stream buffer.
Parameters : none
Return     : The number of words yet to be played in the stream buffer.
================================================================================
*/
UINT32 DM_Audio_RemainingStream(VOID)
{
    UINT32 ret;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    ret = hal_audio_RemainingStream();
#else
#ifndef CSW_EXTENDED_API_AUDIO_VIDEO
    UINT32 bufPos = (UINT32) aps_GetBufPosition();
    UINT32 newOffset;
    ret = g_audioPlayBufStart + g_audioPlayBufLen - bufPos;
    newOffset = bufPos - g_audioPlayBufStart;
    UINT32 virtualAddedData = 0;
    if (newOffset > g_audioPlayBufOffset)
    {
        virtualAddedData = newOffset - g_audioPlayBufOffset;
        g_audioPlayBufOffset = newOffset;
    }
    else
    {
        virtualAddedData = g_audioPlayBufLen - g_audioPlayBufOffset + newOffset;
        g_audioPlayBufOffset = newOffset;
    }

#if 1
    aps_AddedData(virtualAddedData);
#endif
#else
	ret = 0;
#endif
#endif
    return ret;
}


/*
================================================================================
Function   : DM_Audio_RemainingStream
--------------------------------------------------------------------------------
Scope      : Returns the number of words that can still be written in the record buffer.
Parameters : none
Return     : The number of words that can still be written in the record buffer.
================================================================================
*/
UINT32 DM_Audio_RemainingRecord(VOID)
{
    UINT32 ret = 0;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    ret = hal_audio_RemainingRecord();
#else
#ifndef CSW_EXTENDED_API_MRS
    UINT32 readData = ars_AvailableData();
    ars_ReadData(readData);
    g_audioRecBufOffset = (g_audioRecBufOffset + readData) % g_audioRecBufLen;
    ret = g_audioRecBufLen - g_audioRecBufOffset;
#endif
#endif
    return ret;
}

BOOL DM_Audio_PauseStream(VOID)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
  BOOL ret;
  ret = hal_audio_PauseStream();
  if(FALSE == ret)
  {
    AUDIO_DELAY(2);
    ret = hal_audio_PauseStream();

  }
  return ret;
#else
  AUD_ERR_T err;
  err = aud_StreamPause(audioItf, TRUE);
  if (err != AUD_ERR_NO)
  {
    AUDIO_DELAY(2);
    err = aud_StreamPause(audioItf, TRUE);
  }
  return (err == AUD_ERR_NO);
#endif
}

BOOL DM_Audio_ResumeStream(VOID)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
  BOOL ret;
  ret = hal_audio_ResumeStream();
  if(FALSE == ret)
  {
    AUDIO_DELAY(2);
    ret = hal_audio_ResumeStream();

  }
  return ret;
#else
  AUD_ERR_T err;
  err = aud_StreamPause(audioItf, FALSE);
  if (err != AUD_ERR_NO)
  {
    AUDIO_DELAY(2);
    err = aud_StreamPause(audioItf, FALSE);
  }
  return (err == AUD_ERR_NO);
#endif
}

BOOL DM_Close_All_Audio(VOID)
{
	BOOL ret=FALSE;
	DM_StopAudio();
	Speeching = FALSE;
	ret = DM_Audio_StopStream();
	return ret;
	
}
