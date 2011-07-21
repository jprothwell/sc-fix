/******************************************************************************
*              Copyright (C) 2006, Coolsand Technologies, Inc.
*                            All Rights Reserved
******************************************************************************
* Filename:    adp_AUD_Events.c
*
* Description:
*     change CSD phonebook message into coolsand message .
 
******************************************************************************/



 


/**************************************************************
 * Include Files
**************************************************************/
#include "phonebooktypes.h"
#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"


 
#include "wgui_categories_inputs.h"
#include "imeres.h"
 
#include "globalconstants.h"
#include "taskinit.h"
#include "mmitaskprot.h"
#include "thrdosinterface.h"
#include "globalstruct.h"
#include "debuginitdef.h"
#include "protocolevents.h"
#include "asynceventsgprot.h"
#include "alarmframeworkprot.h"
#include "callmanagementgprot.h"
#include "callmanagementstruct.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "service.h"
#include "satgprots.h"
#include "cphsprotocolhandlers.h"
#include "engineermodeprot.h"
#include "factorymodeprot.h"
#include "gpioinc.h"
#include "phonebookgprot.h"
#include "callhistorymain.h"
#include "funandgamesprots.h"
#include "messagesresourcedata.h"
#include "organizergprot.h"
#include "alarmgprot.h"
#include "scheduledpoweronoffgprot.h"
#include "unicodedcl.h"
#include "unicodedef.h"
#include "organizergprot.h"
#include "idleappprot.h"
#include "conversions.h"
#include "nvramprot.h"
 
#include <csw.h>
#include "mmi.h"
#include "uim.h"
#include "adp_events.h"
#include "adp_eventsredef.h"
#include "cfw.h"   
#include "phonebooktypes.h"  

#include "drv.h"    //added by jinzh 20070617
#include "dm.h"     //added by jinzh 20070617
#include "filesystemdef.h"
 
#ifdef __MMI_THEMES_V2_SUPPORT__
#include "thememanager.h"
#endif
#include "messagesexdcl.h"
/*jinzh Add Start For NEW EARPHONE PLAN Ver: TBM780  on 2007-8-2 14:35 */
#include "adp_newearphoneplan.h"
#ifdef MMI_ON_HARDWARE_P
#include "chip_id.h"
#endif
/*jinzh Add End  For NEW EARPHONE PLAN Ver: TBM780  on 2007-8-2 14:35 */
#ifdef __MMI_USB_SUPPORT__
#include "usbdevicegprot.h"
#ifdef MMI_ON_HARDWARE_P
extern kal_bool INT_USBBoot(void);
#endif
#endif

#ifdef __MMI_IRDA_SUPPORT__
#include "irdammigprots.h"
#endif


#ifdef __MMI_BT_SUPPORT__
#include "btmmiscrgprots.h"
#include "profilesdefs.h"
#endif

#ifdef __MMI_FILE_MANAGER__
#include "filemanagergprot.h"
#include "msdc_def.h"
#endif

#ifdef __MMI_WEBCAM__
#include "mdi_datatype.h"
#include "mdi_webcam.h"
#endif /* __MMI_WEBCAM__ */

#include "soundeffect.h"

#include "shortcutsprots.h"
#include "sublcdhistorygprot.h"
#include "todolistdef.h"

#include "wpssprotos.h"
#include "calleridgprot.h"
#if defined(__MMI_STOPWATCH__)
#include "stopwatchgprot.h"
#endif
#if defined(__MMI_EMAIL__)
#include "emailappgprot.h"
#endif // def __MMI_EMAIL__
#include "osthreadinterfacegprot.h"


#include "eventsgprot.h"
#include "allappgprot.h"
#include "wrappergprot.h"
#include "queuegprot.h"

#include "keybrd.h"
#include "nvramexdcl.h"
#include "simdetectiongprot.h"
#include "wgui_status_icons.h"
#include "poweronchargerprot.h"
#include "settinggprots.h"
#include "simdetectiongexdcl.h"
#include "phonesetupgprots.h" /* phnsetreadnvramcalibrationdata */
#include "vibrator.h"
#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
#endif /*__MMI_SMART_MESSAGE_MO__ && !__MMI_MESSAGES_EMS__*/

 
#ifdef __MMI_POC__
#include "pocgprot.h"
#endif


#undef __NEWSIMULATOR


#ifdef MMI_ON_HARDWARE_P
#include "lcd_sw_inc.h"
#include "sublcd.h"
#else
#include "lcd_if.h"
#endif
#include "composeringtoneprots.h"

#ifdef __MMI_IMPS__
#include "mmi_imps_gprot.h"
#endif


#include "device.h"
#include "stack_msgs.h"
#include <cswtype.h>
#include "mmi_msg_struct.h"
#include "sul.h"
#include "fs.h"
#include "mmi_trace.h"


#include "med_main.h"

#include "dm.h"
//#include "audio.h"
//jiashuo add for jasper
#include "med_global.h"
#include "aud_defs.h"
#include "aud_main.h"
#include "med_api.h"
#include "med_context.h"
//jiashuo add for jasper

#include "mmitask.h"
#include "mci.h"
#ifndef MMI_ON_WIN32
#include "cpu_share.h"//jiashuo add for jasper
#include "pmd_config.h"
#include "pmd_m.h"
#include "tgt_pmd_cfg.h"
#include "chip_id.h"
#include "fmradiodef.h"     //added by kecx for fixing bug 13021 on 20090710
#include "fmradiotype.h"

#endif
#ifdef EARPHONE_DETECT_BY_INTERRUPT
#include "hal_gpio.h"
#include "sxr_tim.h"
#endif
#define STOP_ALL_TONE 1000

#define CARD_SRC_ID 0xFF
//convert mmi volumn to csw volumn
//#define SetAudioVolume(volume) DM_SetAudioVolume((volume) + 1)  //Modefied by Jinzh:20070617  原来#define SetAudioVolume(volume) DM_SetAudioVolume((volume) + 2),音量设为最大时没声
extern U16 gstartMiscToneId;
extern U16 gstartRingId;
extern U16 gstartGeneralToneId;
extern U16 gstartMidiId;
/*jinzh Add Start For NEW EARPHONE PLAN Ver: TBM780  on 2007-8-2 18:24 */
extern BOOL KeyDown;
/*jinzh Add End  For NEW EARPHONE PLAN Ver: TBM780  on 2007-8-2 18:24 */
extern UINT8 g_Flag0;
/*jinzh Add Start For NEW EARPHONE PLAN Ver: TBM780  on 2007-8-2 14:39 */
extern U8 isEarphonePlugged;

/*Jinzh Add Start For 6424 Ver: TBM780  on 2007-8-28 17:35 */
extern U8 isEarphonePlugged_1;  
/*Jinzh Add End  For 6424 Ver: TBM780  on 2007-8-28 17:35 */
/*jinzh Add End  For NEW EARPHONE PLAN Ver: TBM780  on 2007-8-2 14:39 */
#ifdef MMI_ON_HARDWARE_P
#ifdef __MMI_FM_RADIO__
extern mmi_fmrdo_struct g_fmrdo;     //added by kecx for fixing bug 13021 on 20090710
#endif
#endif
static UINT32 GetBuffer(UINT8 audioId , UINT32** pBuffer ,DM_PlayRingType* codec);
#if 0
static void CSDAudioMode2CswAudioMode(UINT8 CSDAudioMode , UINT8* cswAudioMode );
#endif
void PlayRingToneReq(l4aud_audio_play_req_struct *pAudioNode);
void PlayKeyPadToneReq(l4aud_audio_play_req_struct *pAudioNode);
void PlaySpecialToneReq(l4aud_audio_play_req_struct *pAudioNode);
void SetAudioVolumeReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI);
#if 0
static INT32 GetAudioFileBuffer(UINT8 *fileName, UINT32 **pAudioFileBuffer);
#endif
static void SendAudioEventToMMI(local_para_struct *pMessageStruct, msg_type_t messageID);
#if 0
static UINT32 GetFlashMp3Buffer(UINT8 srcID, UINT32 **pAudioBuffer);
#endif
BOOL AudioStartStream(UINT32 *buffer, UINT32 len, DM_PlayRingType codec, BOOL	loop_mode, DM_SpeakerGain volumn);
BOOL AudioStopStream();

#ifdef __MMI_ANALOGTV__    
#ifdef MMI_ON_HARDWARE_P
extern UINT32 mmc_AnalogTvAudioSetup(UINT8 volume);
#endif
#endif

extern U8 prevMode;             //Added by Jinzh:20070730 记录拔下耳机后应该从耳机恢复成什么模式    

#ifdef __FF_AUDIO_SET__
void GpioSpeakerModeStart();    //Added by Jinzh:20070616
void GpioSpeakerModeEnd();      //Added by Jinzh:20070616

void ToSetHandSetMode();        //Added by Jinzh:20070616
void ToSetEarpieceMode();        //Added by Jinzh:20070616
void ToSetSpeakerMode();        //Added by Jinzh:20070616
void MusicStartAudioMode(); //Added by Jinzh:20070730
void MusicEndAudioMode(); //Added by Jinzh:20070730
/*jinzh Modify Start For NEW EARPHONE PLAN Ver: TBM780  on 2007-8-2 14:18 */
void EarphoneDetect(UINT16 pLevel);		 //Added by Jinzh:20070616
/*jinzh Modify End  For NEW EARPHONE PLAN Ver: TBM780  on 2007-8-2 14:18 */
void InitHandSetMode();   //Added by Jinzh :20070617 耳机模式的初始化
#if 0
static UINT8 lastAudioMode=DM_AUDIO_MODE_EARPIECE;   //Added by Jinzh:20070617  记录耳机插入之前的audiomode, 以备耳机拔出时恢复插入耳机之前的audiomode状态，初始值必须是DM_AUDIO_MODE_EARPIECE
													 //由于以前的代码机制不明，故此变量无法判断某一时耳机和扬声器的当时工作状态
#endif													 
//static HANDSETSTATUS handsetInPhone=HANDSETNOTINPHONE;   //Added by Jinzh:20070617  判断耳机是否插在耳机上，0表示没插在上面，1表示插在上面
void SetHandsetInPhone(U8 h_status);     //Added by Jinzh:20070618
U8 GetHandsetInPhone();    				 //Added by Jinzh:20070618

//static SPEAKERWORKSTATUS speakerWorkStatus=SPEAKERNOTWORK;     //Added by Jinzh:20070618   判断耳机插上之前，speaker是否处于工作状态，一是放音乐，二是免提接听电话
void SetSpeakerWorkStatus(U8 status);      //Added by Jinzh:20070618
U8 GetSpeakerWorkStatus();			//Added by Jinzh:20070618
/*jinzh Add Start For NEW EARPHONE PLAN Ver: TBM780  on 2007-8-3 8:59 */
void SendMessageForEarphonePlugIn();
void SendMessageForEarphonePlugOut();
void SendMessageForEarphoneKey();
BOOL VmicOpenOrNot();
/*jinzh Add End  For NEW EARPHONE PLAN Ver: TBM780  on 2007-8-3 8:59 */
//chenhe +,070725
BOOL HA_audio_StartStream(UINT32*	buffer, UINT32	len, DM_PlayRingType codec, BOOL	loop_mode);
BOOL HA_audio_StopStream(VOID);
	
//chenhe -,070725
/*Jinzh Add Start For 6225 Ver: TBM780  on 2007-8-14 9:59 */
BOOL cameraAppOnOff = FALSE;
/*Jinzh Add End  For 6225 Ver: TBM780  on 2007-8-14 9:59 */
#endif

static BOOL isRingTone = FALSE;

#if 0
#define MULTIMESSAGE_END MSG_ID_MED_CODE_END+1
static UINT32 multimediaMessage = MULTIMESSAGE_END;
#endif

typedef enum
{
	AMR_TYPE,
	MP3_TYPE,
	MP4_TYPE,
	NO_TYPE
}audio_type;

typedef struct 
{
	UINT8 filename[2][20];
	UINT32 *pResourceBuf;
	UINT32 bufLen;
} 
mp3_resource_t;

//chenhe ++,070725
typedef enum
{
	AUDIO_DEVICE_STATE_ON,
	AUDIO_DEVICE_STATE_OFF,
	AUDIO_DEVICE_STATE_NONE	
}audio_device_state;

audio_device_state audioDeviceState=AUDIO_DEVICE_STATE_NONE;
//chenhe --,070725
#if 0


/**************************************************************
	FUNCTION NAME		: ADP_DispatchAudReqMsg
	PURPOSE			       : Message Adaption for Phonebook REQ.
	INPUT PARAMETERS	: COS_EVENT *pCosEvent
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
static BOOL ADP_DispatchAudReqMsg(COS_EVENT *pCosEvent)
{
	UINT32 nParam1;
	UINT32 nParam2;
	UINT8 nType;
	UINT16 nUTI;

	TRACE_SYS_FUNCTION();
	ASSERT(pCosEvent != NULL);
	
	nParam1 = pCosEvent->nParam1;
	nParam2 = pCosEvent->nParam2;
	nType = HIUINT8(pCosEvent->nParam3);
	nUTI = HIUINT16(pCosEvent->nParam3);

	switch (pCosEvent->nEventId)
	{
		case MSG_ID_L4AUD_AUDIO_PLAY_REQ:
			PlayAudioReq(nParam1, nParam2, nType, nUTI);
			break;

		case MSG_ID_L4AUD_AUDIO_STOP_REQ:
			StopAudioReq(nParam1, nParam2, nType, nUTI);
			break;
			
		case MSG_ID_L4AUD_AUDIO_SET_VOLUME_REQ:
			SetAudioVolumeReq(nParam1, nParam2, nType, nUTI);
			break;

		default:
			return FALSE;
	}
	TRACE_SYS_FUNCTION();
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchAudIndMsg

	PURPOSE				: Message Adaption for AUD

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

**************************************************************/
static BOOL ADP_DispatchAudIndMsg(COS_EVENT *pCosEvent)
{
	ASSERT(pCosEvent != NULL);
	
	TRACE_AUD_FUNC(EV_CSW_PRV_STREAM_END);
	
	switch (audioType)
	{
		case AMR_TYPE:
			StopPlayStreamInd();
			break;

		case MP3_TYPE:
			Mp3StopRsp();
			break;

		case MP4_TYPE:
		case NO_TYPE:
			mmi_trace(g_sw_ADP_AUD, "Invalid audio type! type=MP4_TYPE/NO_TYPE,=%d", audioType);
			break;
			
		default:
			mmi_trace(g_sw_ADP_AUD, "Unknown audio type! type=%d", audioType);
			break;
	}
	
	return TRUE;
}
#endif




#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)




#else
/******************************************************************
* FUNCTION
*    aud_media_Ring_play_ind_hdlr
* DESCRIPTION
*    This function is to handle the audio play indicator in case of CSW_EXTENDED_API
*    This is the callback function called by MCI_Audio in case of change of
*    state of the player
* PARAMETERS
*    MCI_STATE_T state : current state of the player
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/

VOID aud_media_Ring_play_ind_hdlr(MCI_ERR_T result ) //         MCI_MEDIA_PLAY_FINISH_IND,
{
        
    aud_media_play_event_ind_struct *ind_p;
    
   ind_p = ( aud_media_play_event_ind_struct*)
            construct_local_para(sizeof( aud_media_play_event_ind_struct), 2);  

       switch (result)
    {
        case MCI_ERR_END_OF_FILE:
		mmi_trace(1,"aud_media_Ring_play_ind_hdlr: State %d",result );

   		   ind_p->event = MEDIA_END;
        break;

        case MCI_ERR_INVALID_FORMAT:
		mmi_trace(1,"aud_media_Ring_play_ind_hdlr: State %d",result );
    		
               // Send indicator to the MEDIA task 
		ind_p->event = MEDIA_ERROR;
        break;

        default:
    		mmi_trace(1,"aud_media_Ring_play_ind_hdlr: Unknown State %d",result);
        break;
    }


  aud_send_ilm(MOD_MED, 
               MSG_ID_AUD_MEDIA_PLAY_EVENT_IND,
               ind_p, 
               NULL);
}

#endif



/**************************************************************
	FUNCTION NAME		: HA_audio_StartStream
	PURPOSE			       : play ring
	INPUT PARAMETERS	: UINT32*	buffer, UINT32	len, DM_PlayRingType codec, BOOL	loop_mode
	OUTPUT PARAMETERS	: nil
	RETURNS			       : BOOL
	REMARKS			:
**************************************************************/
BOOL HA_audio_StartStream(UINT32*	buffer, UINT32	len, DM_PlayRingType codec, BOOL	loop_mode)
{	
	
	BOOL result;
	if(AUDIO_DEVICE_STATE_ON == audioDeviceState)
	{
#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
        DM_Audio_StopStream();
#else
        MCI_AudioStopBuffer();
#endif
	}
	mmi_trace(g_sw_ADP_AUD,"HA_audio_StartStream,audioDeviceState is %d,codec:%d",audioDeviceState,codec);
	audioDeviceState = AUDIO_DEVICE_STATE_ON;	
#ifdef __FF_AUDIO_SET__
	/*JINZH Modify Start For 6296 Ver: TBM780  on 2007-8-17 13:42 */
//		UINT32 i;
	//for(i=0;i<3000000;i++); //jiashuo del for jasper
//	MusicStartAudioMode();	 //changed by jinzh:20070730

      SendAudioModeRequest( AUD_MODE_LOUDSPK );  //force ring play in loudspk
      SetSpeakerWorkStatus(AUD_MODE_LOUDSPK);   
    
	/*JINZH Modify End	For 6296 Ver: TBM780  on 2007-8-17 13:42 */
#endif
#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
    result = DM_Audio_StartStream(buffer, len,  codec, loop_mode);
#else

	result = MCI_AudioPlayBuffer(buffer, len, loop_mode, aud_media_Ring_play_ind_hdlr, (MCI_PLAY_MODE_T)codec, 0);
	aud_context_p->current_format = MED_TYPE_AMR;
#endif
	return result;
	
}

/**************************************************************
	FUNCTION NAME		: HA_audio_StopStream
	PURPOSE			       : Stop play ring
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : BOOL
	REMARKS			:
**************************************************************/
BOOL HA_audio_StopStream(VOID)
{
      if(AUDIO_DEVICE_STATE_ON == audioDeviceState)
      {
#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
          DM_Audio_StopStream();
#else
          MCI_AudioStopBuffer();
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */
		#ifdef __FF_AUDIO_SET__
		MusicEndAudioMode();   //Added by jinzh:20070730
		#endif
      }
  	mmi_trace(g_sw_ADP_AUD,"HA_audio_StopStream,audioDeviceState is %d",audioDeviceState);

	audioDeviceState = AUDIO_DEVICE_STATE_OFF;
  return TRUE;
}

/**************************************************************
	FUNCTION NAME		: PlayTone
	PURPOSE			       : Play the tone, like key pad tone and other special tones
	INPUT PARAMETERS	: UINT8 nToneType, UINT8 nToneAttenuation, UINT16 nDuration
	OUTPUT PARAMETERS	: nil
	RETURNS			       : BOOL
	REMARKS			:
**************************************************************/
BOOL PlayTone( UINT8 nToneType, UINT8 nToneAttenuation, UINT16 nDuration, DM_SpeakerGain volumn)
{
	return DM_PlayTone(nToneType, nToneAttenuation, nDuration, volumn);
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchAudMsg

	PURPOSE				: Message Adaption for AUD

	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

**************************************************************/

BOOL ADP_DispatchAudMsg(COS_EVENT *pCosEvent)
{
  //TRACE_EF(TRUE, "!!!!!!Excute func : ADP_DispatchAudMsg(msg come from lower layer is %d)", pCosEvent->nEventId);
 	TBM_ENTRY(0x29b2);
	ASSERT(pCosEvent != NULL);
	
  if (pCosEvent->nEventId == EV_DM_STREAM_END_IND)
  {
      ilm_struct ilm_ptr;

      aud_media_play_event_ind_struct* ind_p = (aud_media_play_event_ind_struct*) OslMalloc(sizeof(aud_media_play_event_ind_struct));
      ASSERT(ind_p != NULL);
	  memset(ind_p, 0, sizeof(aud_media_play_event_ind_struct));
      ind_p->event = MEDIA_END;
      
      ilm_ptr.dest_mod_id = MOD_MED;
      ilm_ptr.src_mod_id = MOD_L4C;    
      ilm_ptr.local_para_ptr= (local_para_struct*)ind_p;
      ilm_ptr.peer_buff_ptr = NULL;
      ilm_ptr.msg_id = MSG_ID_AUD_MEDIA_PLAY_EVENT_IND;

    TRACE_EF(TRUE, "Send msg %d to multitask ", ilm_ptr.msg_id);
      
      OslMsgSendExtQueue(&ilm_ptr);
        TBM_EXIT(0x29b2);
    return TRUE;
  }

  

/*Pan Xu Del Start For   Ver: TBM_780  on 2007-7-31 16:36 */
	#if 0
	if (pCosEvent->nEventId >= MSG_ID_MED_STARTUP_REQ && pCosEvent->nEventId <= MSG_ID_MED_CODE_END)
		 return ADP_DispatchAudReqMsg(pCosEvent);
  else
  			return FALSE;
	#endif
/*Pan Xu Del End  For   Ver: TBM_780  on 2007-7-31 16:36 */
	TBM_EXIT(0x29b2);
   	return FALSE;
}


/**************************************************************
	FUNCTION NAME		: AudioStartStream
	PURPOSE			       : Play the audio tone
	INPUT PARAMETERS	: UINT32*	buffer, UINT32 len, DM_PlayRingType codec, BOOL	loop_mode, DM_SpeakerGain volumn
	OUTPUT PARAMETERS	: nil
	RETURNS			       : BOOL
	REMARKS			:
**************************************************************/
BOOL AudioStartStream(UINT32 *buffer, UINT32 len, DM_PlayRingType codec, BOOL loop_mode, DM_SpeakerGain volumn)
{
	isRingTone = TRUE;

	if (DM_SetAudioVolume(volumn))
	{
		return HA_audio_StartStream(buffer, len, codec, loop_mode);
	}
	else
	{
		return FALSE;
	}
}

/**************************************************************
	FUNCTION NAME		: AudioStopStream
	PURPOSE			       : Stop the audio tone
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : BOOL
	REMARKS			:
**************************************************************/
BOOL AudioStopStream()
{
	isRingTone = FALSE;

	return HA_audio_StopStream();
}
#if 0
/**************************************************************
	FUNCTION NAME		: PlayAudioReq
	PURPOSE			       : Play the audio tone
	INPUT PARAMETERS	: UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void PlayAudioReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	aud_play_id_struct *pAudioNode;
	TRACE_AUD_FUNCTION();
	
	ASSERT(NULL != (ilm_struct*)nParam1);
	
	 pAudioNode = 
		(aud_play_id_struct *)(((ilm_struct*)nParam1)->local_para_ptr);
	ASSERT(NULL != pAudioNode);

	//TRACE_EF(g_sw_SYSTEM, "id = %d", pAudioNode->audio_id);
	
	if (pAudioNode->audio_id <= TONE_KEY_CLICK) //key pad tone
	{
		  PlayKeyPadToneReq(pAudioNode);
	}
	else if ((pAudioNode->audio_id > TONE_KEY_CLICK) && (pAudioNode->audio_id <= MAX_TONE_ID))
	{	
		PlaySpecialToneReq(pAudioNode);
	}
	else
	{
		audioType = AMR_TYPE;
		PlayRingToneReq(pAudioNode);
	}
	
}
#endif
/**************************************************************
	FUNCTION NAME		: PlayRingToneReq
	PURPOSE			       : Play the ring tone
	INPUT PARAMETERS	: aud_play_id_struct *pAudioNode
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void PlayRingToneReq(l4aud_audio_play_req_struct *pAudioNode)
{
	
	UINT32 len = 0;
	UINT32* pBuffer = NULL;
	BOOL loop = FALSE;
	UINT8 cswAudioMode = 0;
	DM_PlayRingType codec;
	BOOL result;
	TRACE_AUD_FUNCTION();
	ASSERT(NULL != pAudioNode);


   	TRACE_EF(TRUE, "PlayRingToneReq,   pAudioNode->output_path = %d",   pAudioNode->output_path);
    
/*Jinzh Del Start For 6293 Ver: TBM780  on 2007-8-20 16:41 */
	#if 0
	CSDAudioMode2CswAudioMode(pAudioNode->output_path, &cswAudioMode);
	DM_SetAudioMode_From_Calibration(DM_AUDIO_MODE_EARPIECE/*cswAudioMode*/);
	#endif
/*Jinzh Del End  For 6293 Ver: TBM780  on 2007-8-20 16:41 */

	if (pAudioNode->play_style == DEVICE_AUDIO_PLAY_INFINITE)
	{
		loop = TRUE;
	}
	else
	{
		loop = FALSE;
	}

	len = GetBuffer(pAudioNode->audio_id, &pBuffer, &codec);
	TRACE_EF(TRUE, "LEN = %d, loop = %d, cswAudioMode = %d", len, loop, cswAudioMode);
      #ifdef MMI_ON_HARDWARE_P
	aud_context_p->current_format = MED_TYPE_NONE; // play a internale music , we don't set any format
	#endif
	result = AudioStartStream(pBuffer, len, codec, loop, pAudioNode->volume);
	if (!result)
	{
		TRACE_AUD_FUNC("ok");
	}
	else
	{
		TRACE_AUD_FUNC("fail");
	}
}
#if 0
/**************************************************************
	FUNCTION NAME		: StopAudioReq
	PURPOSE			       : Stop playing the adudio
	INPUT PARAMETERS	: UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void StopAudioReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	l4aud_audio_stop_req_struct *pAudioNode;
	TRACE_SYS_FUNCTION();
	
	ASSERT(NULL != (ilm_struct*)nParam1);
	pAudioNode = 
		(l4aud_audio_stop_req_struct *)(((ilm_struct*)nParam1)->local_para_ptr);

	ASSERT(NULL != pAudioNode);
	
	TRACE_EF(g_sw_SYSTEM, "id = %d", pAudioNode->audio_id);

	

	if (pAudioNode->audio_id <= TONE_KEY_CLICK) //key pad tone
	{
		TRACE_AUD_FUNC("Stop key pad tone!");
	}
	else if (pAudioNode->audio_id == STOP_ALL_TONE)
	{
		AudioStopStream();
	}
	else
	{
		AudioStopStream();
	}
}
#endif
/**************************************************************
	FUNCTION NAME		: PlayKeyPadToneReq
	PURPOSE			       : Play the key pad tone
	INPUT PARAMETERS	: aud_play_id_struct *pAudioNode
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void PlayKeyPadToneReq(l4aud_audio_play_req_struct *pAudioNode)
{

	ASSERT(NULL != pAudioNode);

	switch (pAudioNode->audio_id)
	{
	case TONE_KEY_CLICK:
		/*Replace DM_TONE_RINGING with DM_TONE_DIAL because DM_TONE_DIAL is a higher frequency sound but DM_TONE_RINGING is lower frequency sound by liuxl on 20090608 */
		/*The frequency of DM_TONE_DIAL is double that of DM_TONE_RINGING*/
		//PlayTone(DM_TONE_RINGING, DM_TONE_m3dB, 200, pAudioNode->volume);
		PlayTone(DM_TONE_DIAL, DM_TONE_m3dB, 200, pAudioNode->volume);
		break;

	default:
		PlayTone(pAudioNode->audio_id, DM_TONE_m3dB, 200, pAudioNode->volume);
		break;
	}
}

/**************************************************************
	FUNCTION NAME		: PlaySpecialToneReq
	PURPOSE			       : Play the special tone
	INPUT PARAMETERS	: aud_play_id_struct *pAudioNode
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void PlaySpecialToneReq(l4aud_audio_play_req_struct *pAudioNode)
{
	UINT8 nToneAttenuation = 0;//jiashuo add for configurable tone attenuation,20090306
	TRACE_SYS_FUNCTION();
	ASSERT(NULL != pAudioNode);
	DM_StopTone(); //chenhe,081017,to fix bug 10082,10093
	//jiashuo add for configurable tone attenuation,start,20090306
	#ifdef __MMI_NOKIA_STYLE_N800__
	if(pAudioNode->volume <= LEVEL4)
	{
		nToneAttenuation = DM_TONE_m9d;
	}
	else if((pAudioNode->volume >= LEVEL5)&&(pAudioNode->volume <= LEVEL7))
	{
 		nToneAttenuation = DM_TONE_m3dB;
	}    
	#else
	if(pAudioNode->volume <= LEVEL4)
	{
		nToneAttenuation = DM_TONE_m15dB;
	}
	else if((pAudioNode->volume >= LEVEL5)&&(pAudioNode->volume <= LEVEL7))
	{
 		nToneAttenuation = DM_TONE_m9d;
	}
	#endif
	//jiashuo add for configurable tone attenuation,end,20090306
	PlayTone(DM_TONE_SPECIAL_INFORMATION, nToneAttenuation, 200, pAudioNode->volume);
}

/**************************************************************
	FUNCTION NAME		: SetAudioVolumeReq
	PURPOSE			       : Set the audio volume
	INPUT PARAMETERS	: UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void SetAudioVolumeReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	l4aud_audio_set_volume_req_struct *pAudioNode;
	TRACE_AUD_FUNCTION();
	
	ASSERT(NULL != (ilm_struct*)nParam1);
	pAudioNode = 
		(l4aud_audio_set_volume_req_struct *)(((ilm_struct*)nParam1)->local_para_ptr);
	ASSERT(NULL != pAudioNode);
	
	mmi_trace(g_sw_SYSTEM, "level = %d", pAudioNode->volume);
	if(pAudioNode->audio_type == AUD_VOLUME_FMR)
	{
		mmi_trace(1,"SetAudioVolumeReq->SetAudioVolumeReq:%d",pAudioNode->volume);
		#ifdef MMI_ON_HARDWARE_P
		#ifdef	__MMI_FM_RADIO__
		FMR_SetOutputVolume(15, pAudioNode->volume);
		#endif
		#endif
	}
	else if(pAudioNode->audio_type == AUD_VOLUME_MEDIA)
	{
#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
        MCI_AudioSetVolume(pAudioNode->volume);
#else
        DM_SetAudioVolume(pAudioNode->volume);
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */
		SET_CURRENT_VOLUME_LEVEL(AUD_VOLUME_MEDIA, pAudioNode->volume);//jiashuo add 20080908,save the volume level for resume operation
	}
	else if(pAudioNode->audio_type == AUD_VOLUME_VIDEO)
	{
#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
        MCI_AudioSetVolume(pAudioNode->volume);
#else
        DM_SetAudioVolume(pAudioNode->volume);
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */
		SET_CURRENT_VOLUME_LEVEL(AUD_VOLUME_VIDEO, pAudioNode->volume);//jiashuo add 20080908,save the volume level for resume operation
	}
#ifdef __MMI_ANALOGTV__    
	else if(pAudioNode->audio_type == AUD_VOLUME_TV)
	{
		mmi_trace(TRUE," Funcxxxxx: %s type: %d, volume:%d", __FUNCTION__, pAudioNode->audio_type, pAudioNode->volume);	

		#ifdef MMI_ON_HARDWARE_P
		mmc_AnalogTvAudioSetup(pAudioNode->volume);
		#endif
		SET_CURRENT_VOLUME_LEVEL(AUD_VOLUME_TV, pAudioNode->volume);//jiashuo add 20080908,save the volume level for resume operation
	}
#endif    
	else
	{
		mmi_trace(TRUE," Func: %s type: %d, volume:%d", __FUNCTION__, pAudioNode->audio_type, pAudioNode->volume);	
		DM_SetAudioVolume (pAudioNode->volume);
	}

#ifdef __MMI_BT_SUPPORT__
    mmi_bt_set_headset_speaker_volume(pAudioNode->volume);
#endif 
	
}


/**************************************************************
	FUNCTION NAME		: VibratePlayOnce
	PURPOSE			       : Start vibrate once
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
/*Jinzh Add Start For 6071 Ver: TBM780  on 2007-8-29 13:50 */
extern BOOL g_IsVibrate;
/*Jinzh Add End  For 6071 Ver: TBM780  on 2007-8-29 13:50 */
void VibratePlayOnce()
{	
// dingjian 20071224 update mmi
#ifdef __VIBRATOR__
	static UINT8 count = 1;
#ifdef __MMI_NOKIA_STYLE_N800__
	if (2 != count)
#else
	if ((VIBRATE_COUNT * 2 - 1) != count)
#endif
	{
		count++;
		
		if (vibrator_status())    //modify by dingjian 20070925
		{
			vibrator_close();
		}
		else
		{
			vibrator_open();
		}
		
		StopTimer(VIBRATE_TIME_ONCE_ID);
		StartTimer(VIBRATE_TIME_ONCE_ID, VIBRATE_TIME_DURATION, VibratePlayOnce);
	}
	else
	{
		count = 0;
		/*Jinzh Add Start For 6071 Ver: TBM780  on 2007-8-29 13:48 */
		g_IsVibrate=FALSE;
		/*Jinzh Add End  For 6071 Ver: TBM780  on 2007-8-29 13:48 */
		vibrator_close();//modify by dingjian 20070925
		StopTimer(VIBRATE_TIME_ONCE_ID);
	}
#endif // __VIBRATOR__		
}

/**************************************************************
	FUNCTION NAME		: VibratePlayRepeat
	PURPOSE			       : Start the vibrate repeat
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
void VibratePlayRepeat()
{	
// dingjian 20071224 update mmi
#ifdef __VIBRATOR__
	TRACE_AUD_FUNCTION();
	if (vibrator_status())//modify by dingj 20070926
	{
		vibrator_close();
	}
	else
	{
		vibrator_open();
	}

	StopTimer(VIBRATE_TIME_REPEAT_ID);
	StartTimer(VIBRATE_TIME_REPEAT_ID, VIBRATE_TIME_DURATION, VibratePlayRepeat);
#endif // __VIBRATOR__		
}
void VibratePlaydur()
{	
// dingjian 20071224 update mmi
#ifdef __VIBRATOR__
		g_IsVibrate=FALSE;
		vibrator_close();//modify by dingjian 20070925
		StopTimer(VIBRATE_TIME_DUR_ID);
#endif // __VIBRATOR__		
}

/**************************************************************
	FUNCTION NAME		: GetBuffer
	PURPOSE			       : Get the audio buffer
	INPUT PARAMETERS	: UINT8 audioId
	OUTPUT PARAMETERS	: UINT32** pBuffer 
	RETURNS			       : void
	REMARKS			:
**************************************************************/
//chenhe add for customer to add ring resource without taking care of ring ID ++
//#define GET_FORMAT_BY_HEADER
#ifdef GET_FORMAT_BY_HEADER
#define CSD_AMR_HEADER     "#!AMR\n"
#define CSD_MIDI_HEADER    "MThd"
typedef struct{
	UINT8	szRiff[4];			/* "RIFF" */
	UINT32	dwFileSize;			/* file size */
	UINT8	szWaveFmt[8];		/* "WAVEfmt " */
	UINT32	dwFmtSize;			/* 16 */
	UINT16	wFormatTag;		/* format type */
	UINT16	nChannels;			/* number of channels (i.e. mono, stereo...) */
	UINT32	nSamplesPerSec;	/* sample rate */
	UINT32	nAvgBytesPerSec;	/* for buffer estimation */
	UINT16	nBlockAlign;        	/* block size of data */
	UINT16	wBitsPerSample;		/* number of bits per sample of mono data */
}WAVHeader;
static MCI_PLAY_MODE_T GetBufferRingFormat(UINT32 *pBuffer)
{
	UINT8 headerBuffer[sizeof(WAVHeader)];
	WAVHeader *twavHeader = (WAVHeader*)headerBuffer;
	memcpy(headerBuffer,pBuffer,sizeof(headerBuffer));

		if((memcmp(twavHeader->szRiff,"RIFF",4)==0)
		&&(memcmp(twavHeader->szWaveFmt,"WAVEfmt ",8) == 0) )
		{
		mmi_trace(1,"chenhe,GetBufferRingFormat,MCI_PLAY_MODE_WAV");
			return MCI_PLAY_MODE_WAV;
		}

	
		memset(headerBuffer,0x0,sizeof(headerBuffer));
		memcpy(headerBuffer,pBuffer,strlen(CSD_AMR_HEADER));
		if(!memcmp(headerBuffer,CSD_AMR_HEADER,strlen(CSD_AMR_HEADER)))
		{
			mmi_trace(1,"chenhe,GetBufferRingFormat,MCI_PLAY_MODE_AMR_RING");

			return MCI_PLAY_MODE_AMR_RING;
		}
		else if(strstr(headerBuffer,CSD_MIDI_HEADER))
		{
			mmi_trace(1,"chenhe,GetBufferRingFormat,CSD_MIDI_HEADER");

			return MCI_PLAY_MODE_MID;
		}
		else
		{
			mmi_trace(1,"chenhe,GetBufferRingFormat,MCI_PLAY_MODE_AMR122");

			//return MCI_PLAY_MODE_MP3;
			return MCI_PLAY_MODE_AMR122;
		}

	


	
}
#endif //GET_FORMAT_BY_HEADER
//chenhe add for customer to add ring resource without taking care of ring ID --

static UINT32 GetBuffer(UINT8 audioId, UINT32** pBuffer, DM_PlayRingType* codec)
{
	UINT32 length = 0;
	TRACE_EF(g_sw_SYSTEM, "audioId = %d, audioId - gstartMiscToneId= %d", audioId, audioId - gstartMiscToneId + 1);
	 
	//sms
	if ((MIN_MSG_SND_ID <= audioId) && (MAX_MSG_SND_ID >= audioId))
	{
		TRACE_SYS_FUNC("((221 <= audioId) && (226 >= audioId))");

		*pBuffer =  (UINT32*)resource_message_sounds[audioId - gstartMiscToneId].data;
		length = resource_message_sounds[audioId - gstartMiscToneId].len;
		length /= 4;
		*codec = resource_message_sounds[audioId - gstartMiscToneId].format;;
		return  length;
	}
	//call /alarm
	else if ((MIN_RING_TONE_ID <= audioId) && (MAX_RING_TONE_ID >= audioId))
	{	
		TRACE_SYS_FUNC("((101 <= audioId) && (105 >= audioId))");

		*pBuffer =  (UINT32*)resource_imelodys[audioId - gstartRingId].data;
		length = resource_imelodys[audioId - gstartRingId].len;
		length /= 4;
		*codec =  resource_imelodys[audioId - gstartRingId].format;
		return  length;
	}
	//CALL / alarm
	else if ((MIN_MIDI_ID <= audioId) && (MAX_MIDI_ID >= audioId))
	{
		TRACE_SYS_FUNC("((151 <= audioId) && (162 >= audioId))");

		*pBuffer =  (UINT32*)resource_sounds[audioId - gstartMidiId].data;
		length = resource_sounds[audioId - gstartMidiId].len;
		length /= 4;
		*codec = resource_sounds[audioId - gstartMidiId].format;;
		return  length;
	}
#if defined(__MMI_GAME__)
	// Fun and Game
	else if ((MIN_FNG_SND_ID <= audioId) && (MAX_FNG_SND_ID >= audioId)) //Added by Jinzh start:20070724
	{TRACE_EF(g_sw_MIDI,"游戏音的buffer");
		TRACE_SYS_FUNC("((171 <= audioId) && (180 >= audioId))");

		*pBuffer =  (UINT32*)resource_fng_sounds[audioId - MIN_FNG_SND_ID].data;
		length = resource_fng_sounds[audioId - MIN_FNG_SND_ID].len;
		length /= 4;
		*codec = resource_fng_sounds[audioId - MIN_FNG_SND_ID].format;
		return  length;
	}						//Added by Jinzh end:20070724
#endif	
	//camera
	/*Jinzh Add Start For 5754 Ver: TBM780  on 2007-8-8 16:36 */
	else if ((MIN_CAMERA_SND_ID <= audioId) && (MAX_CAMERA_SND_ID >= audioId)) 
	{
		TRACE_SYS_FUNC("((181 <= audioId) && (190 >= audioId))");

		*pBuffer =  (UINT32*)resource_camera_sounds[audioId - MIN_CAMERA_SND_ID].data;
		length = resource_camera_sounds[audioId - MIN_CAMERA_SND_ID].len;
		length /= 4;

		*codec = resource_camera_sounds[audioId - MIN_CAMERA_SND_ID].format;
		return  length;
	}	
	/*Jinzh Add End  For 5754 Ver: TBM780  on 2007-8-8 16:36 */
	//power on / off
	else if ((MIN_SND_ID <= audioId) && (MAX_SND_ID >= audioId))
	{
		TRACE_SYS_FUNC("((201 <= audioId) && (206 >= audioId))");
		*pBuffer =  (UINT32*)resource_poweronoff_sounds[audioId - gstartGeneralToneId].data;
		length = resource_poweronoff_sounds[audioId - gstartGeneralToneId].len;
		*codec = resource_poweronoff_sounds[audioId - gstartGeneralToneId].format;


		length /= 4;
		return  length;
	}
#ifdef DIGIT_TONE_SUPPORT
	else if ((MIN_HUMAN_VOICE_ID <= audioId) && (MAX_HUMAN_VOICE_ID >= audioId))
	{
		TRACE_SYS_FUNC("((243 <= audioId) && (255 >= audioId))");
		*pBuffer =  (UINT32*)resource_human_voice_tones[audioId - MIN_HUMAN_VOICE_ID].data;
		length = resource_human_voice_tones[audioId - MIN_HUMAN_VOICE_ID].len;
		*codec = resource_human_voice_tones[audioId - MIN_HUMAN_VOICE_ID].format;

		length /= 4;
		return  length;
	}    
#endif	
        else //used for setting volume to play the music
	{
		*pBuffer =  (UINT32*)resource_imelodys[0].data;
		length = resource_imelodys[0].len;
		length /= 4;
		*codec = resource_imelodys[0].format;

		return  length;
	}
}
#if 0
/**************************************************************
	FUNCTION NAME		: StopPlayStreamInd
	PURPOSE			       : Deal when the stream has be finished
	INPUT PARAMETERS	: UINT8 audioId
	OUTPUT PARAMETERS	: UINT32** pBuffer 
	RETURNS			       : void
	REMARKS			:
**************************************************************/
static void StopPlayStreamInd()
{
	isRingTone = FALSE;
}
#endif
#if 0
/**************************************************************
	FUNCTION NAME		: CSDAudioMode2CswAudioMode
	PURPOSE			       : Convert the CSD mode to csw mode
	INPUT PARAMETERS	: UINT8 CSDAudioMode
	OUTPUT PARAMETERS	: UINT8* cswAudioMode
	RETURNS			       : void
	REMARKS			:
**************************************************************/
static void CSDAudioMode2CswAudioMode(UINT8 CSDAudioMode , UINT8 *cswAudioMode )
{
	switch (CSDAudioMode)
	{
		case  MDI_DEVICE_SPEAKER:
			*cswAudioMode = DM_AUDIO_MODE_HANDSET;
			break;

		case  MDI_DEVICE_MICROPHONE:
			*cswAudioMode = DM_AUDIO_MODE_EARPIECE;
			break;

		case  MDI_DEVICE_SPEAKER2:
			*cswAudioMode = DM_AUDIO_MODE_LOUDSPEAKER;
			break;

		case  MDI_DEVICE_LOUDSPEAKER:
			*cswAudioMode = DM_AUDIO_MODE_LOUDSPEAKER;
			break;

		case  MDI_DEVICE_SPEAKER_BOTH:
			*cswAudioMode = DM_AUDIO_MODE_HANDSET;
			break;

		default:	
			*cswAudioMode = DM_AUDIO_MODE_LOUDSPEAKER;
			break;
	}
}
#endif
/**************************************************************
	FUNCTION NAME		: SetHandsetInPhone
	PURPOSE			    : Set Handset InPhone
	INPUT PARAMETERS	: HANDSETSTATUS
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS				:
**************************************************************/
void SetHandsetInPhone(U8 h_status)
{
	//isEarphonePlugged = h_status;
	/*Jinzh Modify Start For 6424 Ver: TBM780  on 2007-8-28 17:35 */
	isEarphonePlugged_1 = h_status;
	/*Jinzh Modify End  For 6424 Ver: TBM780  on 2007-8-28 17:35 */
}
/**************************************************************
	FUNCTION NAME		: GetHandsetInPhone
	PURPOSE			    : Get HandsetInPhone
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			    : HANDSETSTATUS
	REMARKS				:
**************************************************************/
U8 GetHandsetInPhone()
{
	//return isEarphonePlugged;
	/*Jinzh Modify Start For 6424 Ver: TBM780  on 2007-8-28 17:35 */
	return isEarphonePlugged_1;
	/*Jinzh Modify End  For 6424 Ver: TBM780  on 2007-8-28 17:35 */
}

#ifdef __FF_AUDIO_SET__
/**************************************************************
	FUNCTION NAME		: GpioSpeakerStart
	PURPOSE			    : PlayRingTone for DM_AUDIO_MODE_LOUDSPEAKER
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS				:
**************************************************************/
static INT32 speakerWorkFlag =  FALSE ;
void GpioSpeakerModeStart()   //Added by Jinzh:20070616
{	
#ifdef MMI_ON_HARDWARE_P

   #if ((defined( __MED_BT_A2DP_MOD__))&&(defined(__BTCOOLSAND__)||defined(__BTRDA__)))
       if (!aud_bt_a2dp_is_mute_phone()) 	
   #endif   	
        {
        
       if   (speakerWorkFlag)		return;
	else  speakerWorkFlag = TRUE;

#ifndef AUD_EXT_PA_ON_WITH_LCD
	CONST PMD_CONFIG_T* pmdConfig = tgt_GetPmdConfig();
        uint32 DefaultGainLevel = pmdConfig->level[PMD_LEVEL_LOUD_SPEAKER].powerOnState;//get opal audio pa default gain from target
	pmd_SetLevel(PMD_LEVEL_LOUD_SPEAKER,0);//mute opal audio pa
	COS_Sleep(3);
	pmd_EnablePower(PMD_POWER_LOUD_SPEAKER, 1);//enable opal audio pa	
	COS_Sleep(3);
	pmd_SetLevel(PMD_LEVEL_LOUD_SPEAKER,1);
	COS_Sleep(3);
	pmd_SetLevel(PMD_LEVEL_LOUD_SPEAKER,2);
	COS_Sleep(3);
	pmd_SetLevel(PMD_LEVEL_LOUD_SPEAKER,DefaultGainLevel);
#endif // AUD_EXT_PA_ON_WITH_LCD
        } 
 	
#endif
}

/**************************************************************
	FUNCTION NAME		: GpioSpeakerEnd
	PURPOSE			    : PlayRingTone for DM_AUDIO_MODE_HANDSET 
	                                   and DM_AUDIO_MODE_EARPIECE
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS				:
**************************************************************/


void GpioSpeakerModeEnd()   //Added by Jinzh:20070616
{
	speakerWorkFlag = FALSE;
#ifdef MMI_ON_HARDWARE_P
#ifndef AUD_EXT_PA_ON_WITH_LCD
	pmd_EnablePower(PMD_POWER_LOUD_SPEAKER, 0);
#endif // AUD_EXT_PA_ON_WITH_LCD
#endif
}


/**************************************************************
	FUNCTION NAME		: HandSetOperate
	PURPOSE			    : operate Interrupt of HandSet
	INPUT PARAMETERS	: COS_EVENT *
	OUTPUT PARAMETERS	: nil
	RETURNS			    : PFN_DEVICE_HANDLER
	REMARKS				:
**************************************************************/

#if 0       // move to adp_events.c
void EarphoneDetect(UINT16 pLevel)   //Added by Jinzh:20070617
{
#ifndef EARPHONE_DETECT_BY_INTERRUPT
		if((MAXVOLT_EARPHONE_IN_PHONE >= pLevel 
			&& MINVOLT_EARPHONE_IN_PHONE <= pLevel) 
			&& GetHandsetInPhone() == 0
		  )//低电平为耳机插入
		{
			SetHandsetInPhone(1);
			SendMessageForEarphonePlugIn();
		}
        else if(MINVOLT_EARPHONE_NOT_IN_PHONE <= pLevel 
			&& GetHandsetInPhone() == 1
		  )  //拔下耳机
		{
            KeyDown = FALSE;
			SetHandsetInPhone(0);
			SendMessageForEarphonePlugOut();
		}
	    else 
#endif
        if((MAXVOLT_FOR_EARPHONE_PRESSKEY >= pLevel)	
			&& (GetHandsetInPhone() == 1))
		{
			KeyDown = TRUE;
		} 
        else if((KeyDown == TRUE) 
                &&(MAXVOLT_EARPHONE_IN_PHONE >= pLevel 
			    && MINVOLT_EARPHONE_IN_PHONE <= pLevel) 
                && GetHandsetInPhone() == 1)
		{
			KeyDown = FALSE;
			SendMessageForEarphoneKey();
		}
	
}
/*jinzh Modify End  For NEW EARPHONE PLAN Ver: TBM780  on 2007-8-2 14:17 */
#endif
/**************************************************************
	FUNCTION NAME		: MusicStartAudioMode
	PURPOSE			    : to set MusicStartAudioMode
	INPUT PARAMETERS	: UINT8 *
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS				:
**************************************************************/
void MusicStartAudioMode() //Added by Jinzh:20070616
{	
	/*Jinzh Modify Start For 6225 Ver: TBM780  on 2007-8-14 10:11 */
#ifdef TGT_GALLITE_G800              
	if (1 == GetHandsetInPhone() && cameraAppOnOff ==  FALSE
		#ifdef __MMI_A2DP_SUPPORT__
		||(aud_bt_a2dp_is_mute_phone()&&(cameraAppOnOff ==  FALSE))
		#endif
		#ifdef __MMI_ANALOGTV__
		&&(atvif_GetCurModule() != 1)
		#endif
		)
#else
		if( (1 == GetHandsetInPhone() && cameraAppOnOff ==  FALSE)
		#ifdef __MMI_A2DP_SUPPORT__
		||(aud_bt_a2dp_is_mute_phone()&&(cameraAppOnOff ==  FALSE))
		#endif
		)
#endif
	/*Jinzh Modify End  For 6225 Ver: TBM780  on 2007-8-14 10:11 */
	{
		SendAudioModeRequest( AUD_MODE_HEADSET );  //Changed by jinzh:20070730
	}
	else
	{	
		SendAudioModeRequest( AUD_MODE_LOUDSPK );  //Changed by jinzh:20070730
	}
	SetSpeakerWorkStatus(AUD_MODE_LOUDSPK);   //changed by jinzh:20070730
}

/**************************************************************
	FUNCTION NAME		: MusicEndAudioMode
	PURPOSE			    : to set MusicEndAudioMode
	INPUT PARAMETERS	: UINT8 *
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS				:
**************************************************************/
void MusicEndAudioMode() //Added by Jinzh:20070616
{	

        #ifdef __MMI_BT_AUDIO_VIA_SCO__
         if(mmi_profiles_bt_is_connected(MMI_PROFILES_BT_HFP_SCO))
        {
	    SetSpeakerWorkStatus(AUD_MODE_BLUETOOTH);
           SendAudioModeRequest( AUD_MODE_BLUETOOTH );

         return;
         }
        #endif

     	if(1 == GetHandsetInPhone()  )//Added by jinzh:20070710
	{
		SetSpeakerWorkStatus(AUD_MODE_HEADSET);
		SendAudioModeRequest( AUD_MODE_HEADSET );
	}
	else
	{
		SetSpeakerWorkStatus(AUD_MODE_NORMAL);
		SendAudioModeRequest( AUD_MODE_NORMAL );
	}
	
}


/**************************************************************
	FUNCTION NAME		: ToSetHandSetMode
	PURPOSE			    : To Set HandSetMode
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS				:
**************************************************************/
void ToSetHandSetMode()        //Added by Jinzh:20070616
{
	mmi_trace(g_sw_CC," Func: %s ", __FUNCTION__);
	#ifdef __MED_BT_HFP_MOD__
    if (aud_bt_hfp_is_audio_path_on())
    {
	aud_context_p->audio_mode = AUD_MODE_HEADSET;
    }
    else 
#endif	
    {
	GpioSpeakerModeEnd();
	/*Jinzh Modify Start For 6095 Ver: TBM780  on 2007-8-13 13:39 */
	DM_SetAudioMode(DM_AUDIO_MODE_HANDSET);
	/*Jinzh Modify End  For 6095 Ver: TBM780  on 2007-8-13 13:39 */
#ifdef MMI_ON_HARDWARE_P
	MCI_AudioSetOutputPath(MCI_PATH_HP,0);
	aud_context_p->audio_mode = AUD_MODE_HEADSET;
#endif
    }
}

/**************************************************************
	FUNCTION NAME		: ToSetSpeakerMode
	PURPOSE			    : To Set HandSetMode
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS				:
**************************************************************/
void ToSetSpeakerMode()        //Added by Jinzh:20070616
{
	mmi_trace(g_sw_CC," Func: %s ", __FUNCTION__);
#ifdef __MED_BT_HFP_MOD__
     if ( aud_bt_hfp_is_audio_path_on())
    {
	aud_context_p->audio_mode = AUD_MODE_LOUDSPK;
    }
    else 
#endif	
    {
	/*Jinzh Modify Start For 6095 Ver: TBM780  on 2007-8-13 13:39 */
	DM_SetAudioMode(DM_AUDIO_MODE_LOUDSPEAKER);
	/*Jinzh Modify End  For 6095 Ver: TBM780  on 2007-8-13 13:39 */
#ifdef MMI_ON_HARDWARE_P
	MCI_AudioSetOutputPath(MCI_PATH_LSP,0);      
	aud_context_p->audio_mode = AUD_MODE_LOUDSPK;
#endif
	GpioSpeakerModeStart();
    }
}

/**************************************************************
	FUNCTION NAME		: ToSetSpeakerMode
	PURPOSE			    : To Set HandSetMode
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS				:
**************************************************************/
void ToSetEarpieceMode()        //Added by Jinzh:20070616
{
//jzh used to tc9751 pop v start
	mmi_trace(g_sw_AUDIO," Func: %s ", __FUNCTION__);
#ifdef __MED_BT_HFP_MOD__
    if ( aud_bt_hfp_is_audio_path_on())
    {
	aud_context_p->audio_mode = AUD_MODE_NORMAL;
    }
    else 
#endif	

    {
	GpioSpeakerModeEnd();
//jzh used to tc9751 pop v end
	/*Jinzh Modify Start For 6095 Ver: TBM780  on 2007-8-13 13:39 */
	DM_SetAudioMode(DM_AUDIO_MODE_EARPIECE);
	/*Jinzh Modify End  For 6095 Ver: TBM780  on 2007-8-13 13:39 */
#ifdef MMI_ON_HARDWARE_P
	//jiashuo change start for jasper ff board audio,temporarily
	MCI_AudioSetOutputPath(MCI_PATH_NORMAL,0);
	//jiashuo change end for jasper ff board audio,temporarily
	      
	aud_context_p->audio_mode = AUD_MODE_NORMAL;
#endif
    	}

}


/**************************************************************
	FUNCTION NAME		: ToSetBluetoothMode
	PURPOSE			    : To Set BluetoothMode
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS				:
**************************************************************/
void ToSetBluetoothMode()        //Added by Cui:20090615
{

      DM_StopTone();
	mmi_trace(g_sw_CC," Func: %s ", __FUNCTION__);
	GpioSpeakerModeEnd();

	DM_SetAudioMode(DM_AUDIO_MODE_BLUETOOTH);
#ifdef MMI_ON_HARDWARE_P		
	MCI_AudioSetOutputPath(MCI_PATH_NORMAL,0);
	
	      

	//aud_context_p->audio_mode = AUD_MODE_BLUETOOTH;
		  
#endif

}


/**************************************************************
=======
	FUNCTION NAME		: SetSpeakerWorkStatus
	PURPOSE			    : Set Speaker Workstatus
	INPUT PARAMETERS	: SPEAKERWORKSTATUS
	OUTPUT PARAMETERS	: nil
	RETURNS			    : void
	REMARKS				:
**************************************************************/
void SetSpeakerWorkStatus(U8 status) //Added by jinzh
{
	mmi_trace(g_sw_CC," Func: %s prevMode=%d", __FUNCTION__,prevMode);
	prevMode = status;
}
/**************************************************************
	FUNCTION NAME		: GetSpeakerWorkStatus
	PURPOSE			    : Get Speaker Workstatus
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: nil
	RETURNS			    : SPEAKERWORKSTATUS
	REMARKS				:
**************************************************************/
U8 GetSpeakerWorkStatus()
{
	mmi_trace(g_sw_CC," Func: %s prevMode=%d", __FUNCTION__,prevMode);
	return prevMode;
}

void SendMessageForEarphonePlugIn()
{
	ilm_struct ptr_HandsetKey;

	mmi_eq_gpio_detect_ind_struct *pHandsetKey=(mmi_eq_gpio_detect_ind_struct *)OslMalloc(sizeof(mmi_eq_gpio_detect_ind_struct));
	ASSERT(NULL != pHandsetKey);
	memset(pHandsetKey,0,sizeof(mmi_eq_gpio_detect_ind_struct));
	
	pHandsetKey->gpio_device=EXT_DEV_EARPHONE;
	pHandsetKey->on_off=1;


	memset(&ptr_HandsetKey,0,sizeof(ilm_struct));

	ptr_HandsetKey.src_mod_id=MOD_L4C;
	ptr_HandsetKey.dest_mod_id=MOD_MMI;
	ptr_HandsetKey.msg_id=MSG_ID_MMI_EQ_GPIO_DETECT_IND;
	ptr_HandsetKey.local_para_ptr=(local_para_struct *)pHandsetKey;
	OslMsgSendExtQueue(&ptr_HandsetKey);
}

void SendMessageForEarphonePlugOut()
{	
	ilm_struct ptr_HandsetKey;

	mmi_eq_gpio_detect_ind_struct *pHandsetKey=(mmi_eq_gpio_detect_ind_struct *)OslMalloc(sizeof(mmi_eq_gpio_detect_ind_struct));
	ASSERT(NULL != pHandsetKey);
	memset(pHandsetKey,0,sizeof(mmi_eq_gpio_detect_ind_struct));
	pHandsetKey->gpio_device=EXT_DEV_EARPHONE;
	pHandsetKey->on_off=0;


	memset(&ptr_HandsetKey,0,sizeof(ilm_struct));

	ptr_HandsetKey.src_mod_id=MOD_L4C;
	ptr_HandsetKey.dest_mod_id=MOD_MMI;
	ptr_HandsetKey.msg_id=MSG_ID_MMI_EQ_GPIO_DETECT_IND;
	ptr_HandsetKey.local_para_ptr=(local_para_struct *)pHandsetKey;
	OslMsgSendExtQueue(&ptr_HandsetKey);
}

void SendMessageForEarphoneKey()
{	
	ilm_struct ptr_HandsetKey;

	mmi_eq_gpio_detect_ind_struct *pHandsetKey=(mmi_eq_gpio_detect_ind_struct *)OslMalloc(sizeof(mmi_eq_gpio_detect_ind_struct));
	ASSERT(NULL != pHandsetKey);
	memset(pHandsetKey,0,sizeof(mmi_eq_gpio_detect_ind_struct));
	pHandsetKey->gpio_device=EXT_DEV_KEY_1;
	pHandsetKey->on_off=1;


	memset(&ptr_HandsetKey,0,sizeof(ilm_struct));

	ptr_HandsetKey.src_mod_id=MOD_L4C;
	ptr_HandsetKey.dest_mod_id=MOD_MMI;
	ptr_HandsetKey.msg_id=MSG_ID_MMI_EQ_GPIO_DETECT_IND;
	ptr_HandsetKey.local_para_ptr=(local_para_struct *)pHandsetKey;
	OslMsgSendExtQueue(&ptr_HandsetKey);
}
#endif


#if 0
/**************************************************************
	FUNCTION NAME		: Mp3PlayReq
	PURPOSE			       : Mp3 play message is processed in this function
	INPUT PARAMETERS	: UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI
	OUTPUT PARAMETERS	: nil
	RETURNS			       : void
	REMARKS			:
**************************************************************/
static void Mp3PlayReq(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
	INT32 len = 0;
	UINT32 *pBuffer = NULL;
	BOOL loop = FALSE;
	aud_play_file_struct *pAudioNode;
	TRACE_AUD_FUNCTION();
	
	ASSERT(NULL != (ilm_struct*)nParam1);
	
	pAudioNode = 
		(aud_play_file_struct *)(((ilm_struct*)nParam1)->local_para_ptr);
	ASSERT(NULL != pAudioNode);
	

/*Jinzh Del Start For 6293 Ver: TBM780  on 2007-8-20 16:41 */
	#if 0
	CSDAudioMode2CswAudioMode(pAudioNode->output_path, &cswAudioMode);
	DM_SetAudioMode_From_Calibration(cswAudioMode); 
	#endif
/*Jinzh Del End  For 6293 Ver: TBM780  on 2007-8-20 16:41 */

	if (pAudioNode->play_style == DEVICE_AUDIO_PLAY_INFINITE)
	{
		loop = TRUE;
	}
	else
	{
		loop = FALSE;
	}

	if (pAudioNode->src_id == CARD_SRC_ID)
	{
		len = GetAudioFileBuffer((UINT8 *)pAudioNode->file_name_p, &pBuffer); 
		AudioStartStream(pBuffer, len, DM_DICTAPHONE_MR122, loop, pAudioNode->volume);
		OslMfree(pBuffer);
	}
	else
	{
		len = GetFlashMp3Buffer(pAudioNode->src_id, &pBuffer);
		AudioStartStream(pBuffer, len, DM_DICTAPHONE_MR122, loop, pAudioNode->volume);		
	}
}

/**************************************************************
	FUNCTION NAME		:GetAudioFileBuffer 
	PURPOSE		       : Get the buffer from the file system
	INPUT PARAMETERS	: UINT8 *fileName
	OUTPUT PARAMETERS	: pAudioFileBuffer
	RETURNS			       : the audio file buffer length
	REMARKS			:
**************************************************************/
static INT32 GetAudioFileBuffer(UINT8 *fileName, UINT32 **pAudioFileBuffer)
{
	HANDLE fileHandle = MMI_FS_Open(fileName, FS_READ_ONLY);

	INT32 fileSize = 0;
	INT32 bufferSize = 0;
	
	if (NIL != fileHandle)
	{
		pfgetfilesize(fileHandle, (S32 *)&fileSize); 

		*pAudioFileBuffer = (UINT32*)OslMalloc(fileSize);
		ASSERT(NULL != *pAudioFileBuffer);
		bufferSize = pfread(fileHandle, (UINT8 *)*pAudioFileBuffer, fileSize); //just use 64k as the largest
	}
	else
	{
		return -1;
	}

	pfclose(fileHandle);

	return bufferSize;
}

/**************************************************************
	FUNCTION NAME		:GetFlashMp3Buffer 
	PURPOSE		       : Get the MP3 buffer via the srcID, becuase the flash mp3 buffer is defined by it's id
	INPUT PARAMETERS	: UINT8 *srcID
	OUTPUT PARAMETERS	:  UINT32 **pAudioBuffer
	RETURNS			       : The audio buffer length
	REMARKS			:
**************************************************************/
static UINT32 GetFlashMp3Buffer(UINT8 srcID, UINT32 **pAudioBuffer)
{
	if (srcID >= 4)
	{
		return 0;
	}

	*pAudioBuffer = mp3Resource[srcID].pResourceBuf;

	return mp3Resource[srcID].bufLen;
}

/**************************************************************
	FUNCTION NAME		:Mp3StopReq
	PURPOSE		       : Stop the playing mp3
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void
	RETURNS			       : 
	REMARKS			:
**************************************************************/
void Mp3StopReq()
{
	AudioStopStream();
}
/**************************************************************
	FUNCTION NAME		:Mp3StopRsp 
	PURPOSE		       : The function will send the mp3 stop response to mmi
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void 
	RETURNS			       : 
	REMARKS			:
**************************************************************/
static void Mp3StopRsp()
{
	l4aud_media_play_finish_ind_struct *pMediaPlayFinishRsp;
	TRACE_AUD_FUNCTION();
	isRingTone = FALSE;

	pMediaPlayFinishRsp =
		(l4aud_media_play_finish_ind_struct*)OslMalloc(sizeof(l4aud_media_play_finish_ind_struct));
	ASSERT(NULL != pMediaPlayFinishRsp);
	memset(pMediaPlayFinishRsp, 0, sizeof(l4aud_media_play_finish_ind_struct));

	SendAudioEventToMMI((local_para_struct*) pMediaPlayFinishRsp, MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND);	 
}
/**************************************************************
	FUNCTION NAME		:Mp3Pause 
	PURPOSE		       : The function is used to pause the playing mp3
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void 
	RETURNS			       : 
	REMARKS			:
**************************************************************/
static void Mp3Pause()
{
	//CSW should give the interface for this
	TRACE_AUD_FUNCTION();
}
#endif

/**************************************************************

	FUNCTION NAME		: SendAudioEventToMMI

  	PURPOSE				: Send the event to mmi queue to be processed.

	INPUT PARAMETERS	: pMessageStruct, messageID

	OUTPUT PARAMETERS	: void

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static void SendAudioEventToMMI(local_para_struct *pMessageStruct, msg_type_t messageID)
{
	ilm_struct  ilm_ptr;

	ilm_ptr.local_para_ptr = pMessageStruct;
	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = messageID;
	ilm_ptr.sap_id = MNSMS_SAP;
	ilm_ptr.peer_buff_ptr = NULL;

	OslMsgSendExtQueue(&ilm_ptr);
}

/*Wangzl Add Start For 6348 Ver: TBM780  on 2007-8-22 9:10 */
//此函数是为处理消息MSG_ID_MMI_EQ_GET_AUDIO_PROFILE_REQ 所打的桩，尚未完善
void GetAudioProfile(void)
{
	mmi_eq_get_audio_profile_rsp_struct * AudioProRsp;

	AudioProRsp = (mmi_eq_get_audio_profile_rsp_struct *)OslMalloc(sizeof(mmi_eq_get_audio_profile_rsp_struct));
	ASSERT(AudioProRsp != NULL);
	memset(AudioProRsp, 0, sizeof(mmi_eq_get_audio_profile_rsp_struct));
	//添加实际获得函数

	//添加实际获得函数
	AudioProRsp->result = TRUE;
	SendAudioEventToMMI((local_para_struct *)AudioProRsp,MSG_ID_MMI_EQ_GET_AUDIO_PROFILE_RSP);
}

//此函数是为处理消息MSG_ID_MMI_EQ_GET_AUDIO_PARAM_REQ 所打的桩，尚未完善

void GetAudioParam(void)
{
	mmi_eq_get_audio_param_rsp_struct * AudioParamRsp;
	
	AudioParamRsp = (mmi_eq_get_audio_param_rsp_struct *)OslMalloc(sizeof(mmi_eq_get_audio_param_rsp_struct));
	ASSERT(AudioParamRsp != NULL);
	memset(AudioParamRsp, 0, sizeof(mmi_eq_get_audio_param_rsp_struct));
	//添加实际获得函数

	//添加实际获得函数
	AudioParamRsp->result = TRUE;
	SendAudioEventToMMI((local_para_struct *)AudioParamRsp,MSG_ID_MMI_EQ_GET_AUDIO_PARAM_RSP);
}
/*Wangzl Add End  For 6348 Ver: TBM780  on 2007-8-22 9:10 */


#ifdef __TFLASH_MOUNT_DYNAMIC__
/**************************************************************
	FUNCTION NAME		: SendMessageForTFlashPlugIn
	PURPOSE			    :  
	INPUT PARAMETERS	: COS_EVENT *
	OUTPUT PARAMETERS	: nil
	RETURNS			    : 
	REMARKS				:
**************************************************************/
void SendMessageForTFlashPlugIn()
{
	ilm_struct ptr_HandsetKey;

	mmi_eq_gpio_detect_ind_struct *pHandsetKey=(mmi_eq_gpio_detect_ind_struct *)OslMalloc(sizeof(mmi_eq_gpio_detect_ind_struct));
	ASSERT(NULL != pHandsetKey);
	memset(pHandsetKey,0,sizeof(mmi_eq_gpio_detect_ind_struct));
	
	pHandsetKey->gpio_device=EXT_DEV_TFLASH_DETECT;
	pHandsetKey->on_off=1;


	memset(&ptr_HandsetKey,0,sizeof(ilm_struct));

	ptr_HandsetKey.src_mod_id=MOD_L4C;
	ptr_HandsetKey.dest_mod_id=MOD_MMI;
	ptr_HandsetKey.msg_id=MSG_ID_MMI_EQ_GPIO_DETECT_IND;
	ptr_HandsetKey.local_para_ptr=(local_para_struct *)pHandsetKey;
	OslMsgSendExtQueue(&ptr_HandsetKey);
}
/**************************************************************
	FUNCTION NAME		: SendMessageForTFlashPlugOut
	PURPOSE			    : 
	INPUT PARAMETERS	: COS_EVENT *
	OUTPUT PARAMETERS	: nil
	RETURNS			    : 
	REMARKS				:
**************************************************************/
void SendMessageForTFlashPlugOut()
{
	ilm_struct ptr_HandsetKey;

	mmi_eq_gpio_detect_ind_struct *pHandsetKey=(mmi_eq_gpio_detect_ind_struct *)OslMalloc(sizeof(mmi_eq_gpio_detect_ind_struct));
	ASSERT(NULL != pHandsetKey);
	memset(pHandsetKey,0,sizeof(mmi_eq_gpio_detect_ind_struct));
	
	pHandsetKey->gpio_device=EXT_DEV_TFLASH_DETECT;
	pHandsetKey->on_off=0;


	memset(&ptr_HandsetKey,0,sizeof(ilm_struct));

	ptr_HandsetKey.src_mod_id=MOD_L4C;
	ptr_HandsetKey.dest_mod_id=MOD_MMI;
	ptr_HandsetKey.msg_id=MSG_ID_MMI_EQ_GPIO_DETECT_IND;
	ptr_HandsetKey.local_para_ptr=(local_para_struct *)pHandsetKey;
	OslMsgSendExtQueue(&ptr_HandsetKey);
}

/**************************************************************
	FUNCTION NAME		: T_FlashDetect
	PURPOSE			    : operate Interrupt of TFlash plugin/out
	INPUT PARAMETERS	: COS_EVENT *
	OUTPUT PARAMETERS	: nil
	RETURNS			    : 
	REMARKS				:
**************************************************************/
void ADP_FlashDetect(UINT16 is_on)  
{
    if(is_on)
    {
        StopTimer( TFLASH_ACTIVE_DELAY_TIMER);
        StartTimer(TFLASH_ACTIVE_DELAY_TIMER, 500 , SendMessageForTFlashPlugIn );
    }
    else
    {
        StopTimer( TFLASH_ACTIVE_DELAY_TIMER);
        StartTimer(TFLASH_ACTIVE_DELAY_TIMER, 500 , SendMessageForTFlashPlugOut );
    }
	
}
#endif

