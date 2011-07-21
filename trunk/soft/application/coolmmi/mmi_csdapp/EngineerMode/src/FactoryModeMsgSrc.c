/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 * FactoryModeMsgSrc.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Factory mode message handler
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

 
#include "mmi_features.h" 
#include "kal_non_specific_general_types.h"
//CSD End

#ifdef __MMI_FACTORY_MODE__  

#include "stdc.h"
#include "globalconstants.h"
#include "debuginitdef.h"
#include "wgui.h"
#include "globalstruct.h"
#include "mmi_data_types.h"
#include "wgui_categories.h"
#include "historygprot.h"
#include "globalscrenum.h"
#include "idleappprot.h"
#include "custdatares.h"
#include "protocolevents.h"
#include "eventsgprot.h"
#include "datetimetype.h"
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"
#include "custmenures.h"
#include "miscfunctions.h"
//jl #include "maintaskgexdcl.h"
#include "queuegprot.h"
//#include "taskinit.h"
#include "engineermodedef.h"
#include "engineermodetype.h"
#include "engineermodeprot.h"
//micha0202
#include "networksetupgprot.h"
#include "wgui_status_icons.h"
#include "factorymodeprot.h"

#ifdef MMI_ON_HARDWARE_P
   #include "mmi_msg_struct.h"
	#include "l4dr.h"
	#include "custom_util.h"
	#include "lcd_sw.h"
	#include "device.h"
   #include "resource_audio.h"
extern kal_int32 media_aud_stop_id(module_type src_mod_id, kal_uint8 audio_id);
#endif

/*External Funtions*/
extern void EntryIdleScreen(void);
extern void FM_LCDShowColor( S32 type );

 
#include "stack_ltlcom.h"
#include "mmiapi.h"
#include "engineermodepatch.h"

void FM_SetGpioReq(U8 type, U8 level)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_exe_gpio_level_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_exe_gpio_level_req_struct));
   msg_p->gpio_dev_type = type;
   msg_p->gpio_dev_level = level;
   FM_SendMsg(MSG_ID_MMI_EQ_EXE_GPIO_LEVEL_REQ, msg_p, NULL);
#endif   
}


void FM_SendADCStartReq(void)
{
#ifdef MMI_ON_HARDWARE_P
   FM_SendMsg(MSG_ID_MMI_EQ_START_ADC_ALL_CHANNEL_REQ, NULL, NULL);
#endif
}


void FM_SendADCStopReq(void)
{
#ifdef MMI_ON_HARDWARE_P
   FM_SendMsg(MSG_ID_MMI_EQ_STOP_ADC_ALL_CHANNEL_REQ, NULL, NULL);
#endif
}

void FM_SendKeyEventReq(U8 status, U8 code)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_em_keypad_event_output_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_em_keypad_event_output_req_struct));
   msg_p->keypad_status = status;
   msg_p->keypad_code = code;

   FM_SendMsg(MSG_ID_MMI_EM_KEYPAD_EVENT_OUTPUT_REQ, msg_p, NULL);
   #endif
}

void FM_ATEntryMenuHdlr(void *inMsg)
{
#ifdef MMI_ON_HARDWARE_P
     mmi_factory_test_ind_struct *factory_test_struct = (mmi_factory_test_ind_struct *)inMsg;
     DisplayIdleScreen();

     if( factory_test_struct->on_off == 1 )
     EntryFMMenu();
#endif
}

void FM_ATLCDTestMsgHdlr( void *inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_em_lcm_test_ind_struct* rsp_p = (mmi_em_lcm_test_ind_struct*)inMsg;

	switch(rsp_p->color_t)
	{

	/***********************/
	/** FM_LCDShowColor:  **/
	/**  0 : Auto         **/
	/**  1 : Red          **/
	/**  2 : Green        **/
	/**  3 : Blue         **/
	/**  4 : White        **/
	/***********************/
   
	case 0:  /* R */
      EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
      ClearAllKeyHandler();
      ShowCategory7Screen( 0, 0,
                           0, 0,
                           0, 0,
					            NULL, NULL); 
      FM_LCDShowColor(1); 
		break;

	case 1:  /* G */
      EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
      ClearAllKeyHandler();
      ShowCategory7Screen( 0, 0,
                           0, 0,
                           0, 0,
					            NULL, NULL); 
      FM_LCDShowColor(2); 
		break;

	case 2:  /* B */
      EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
      ClearAllKeyHandler();
      ShowCategory7Screen( 0, 0,
                           0, 0,
                           0, 0,
					            NULL, NULL); 
      FM_LCDShowColor(3); 
		break;

	case 3:  /* W */
      EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
      ClearAllKeyHandler();
      ShowCategory7Screen( 0, 0,
                           0, 0,
                           0, 0,
					            NULL, NULL); 
      FM_LCDShowColor(4);
		break;

	case 4:  /* Back to Idle screen */
         DisplayIdleScreen();
         EntryFMMenu();
		break;
		
	default:
		break;
	}
/*
   if (rsp_p->color_type == 1)
   {
      FM_EntryColorTest();
   }
   else
      GoBackHistory();
*/
#endif   
}


void FM_ATKeypadTestMsgHdlr(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P

   mmi_em_keypad_event_act_ind_struct* rsp_p = (mmi_em_keypad_event_act_ind_struct*)inMsg;

//Lisen 0630
   if (rsp_p->on_off == 1)
      EntryFMKeypadTest(); 
   else if( FM_IsInKeypadTest() == MMI_TRUE ) 
      GoBackHistory();

#endif
}

   extern void AudioPlayReq(U16, U8);
   void AudioStopReq(U16 soundId);

void FM_SendPlayAudioReq(U8 index)
{
#ifdef MMI_ON_HARDWARE_P
    #if 0 
   mmi_eq_play_audio_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_play_audio_req_struct));
   msg_p->sound_id = index+MIN_MIDI_ID;
   msg_p->style = DEVICE_AUDIO_PLAY_INFINITE;
   FM_SendMsg(MSG_ID_MMI_EQ_PLAY_AUDIO_REQ, msg_p, NULL);
   #else
   DM_SetAudioMode(DM_AUDIO_MODE_EARPIECE);

   AudioPlayReq(index+MIN_MIDI_ID, DEVICE_AUDIO_PLAY_INFINITE);
  #endif
  
#endif   
}

void FM_SendStopAudioReq(U8 index)
{
#ifdef MMI_ON_HARDWARE_P
/*
   mmi_eq_stop_audio_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_stop_audio_req_struct));
   msg_p->sound_id = index+MIN_MIDI_ID;
   FM_SendMsg(MSG_ID_MMI_EQ_STOP_AUDIO_REQ, msg_p, NULL);
*/
AudioStopReq(index+MIN_MIDI_ID);
//   media_aud_stop_id(MOD_MMI, (U8)index+MIN_MIDI_ID);

#endif   
}
/*wangrui modefy 20081219 begin for fix 10832*/
extern U8 GetHandsetInPhone(void);
extern void ToSetHandSetMode() ;
void FM_SendSetAudioModeReq(U8 mode)
{
#ifdef MMI_ON_HARDWARE_P
  #if 0
   mmi_eq_set_audio_mode_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_set_audio_mode_req_struct));
   msg_p->mode = mode;
   FM_SendMsg(MSG_ID_MMI_EQ_SET_AUDIO_MODE_REQ, msg_p, NULL);
#else
	if(GetHandsetInPhone())
	{
		mmi_trace(1, "GetHandsetInPhone == TRUE");
		DM_SetAudioMode(DM_AUDIO_MODE_HANDSET);
		ToSetHandSetMode();		 
	}
	else
	{

		switch(mode)
		{
			case AUD_MODE_NORMAL:
				mmi_trace(1,"FM_SendSetAudioModeReq::AUD_MODE_NORMAL");
				DM_SetAudioMode(DM_AUDIO_MODE_EARPIECE);
				break;
			case AUD_MODE_HEADSET:
				mmi_trace(1,"FM_SendSetAudioModeReq::AUD_MODE_HEADSET");
				DM_SetAudioMode(DM_AUDIO_MODE_HANDSET);
				break;
			case AUD_MODE_LOUDSPK:
				mmi_trace(1,"FM_SendSetAudioModeReq::AUD_MODE_LOUDSPK");
				DM_SetAudioMode(DM_AUDIO_MODE_LOUDSPEAKER);
				break;
			default:
				break;
		}
	}
#endif
#endif   
}
/*wangrui modefy 20081219 end*/

/*****************************************************************
 **  FM Mode Send Message
 *****************************************************************/
void FM_SendMsg(U32 msg_id, 
                     void *local_param_ptr, 
                     void *peer_buf_ptr)
{
#ifdef MMI_ON_HARDWARE_P
   ilm_struct       *ilm_ptr = NULL;

   ilm_ptr = allocate_ilm(MOD_MMI);
   ilm_ptr->src_mod_id = MOD_MMI;
   ilm_ptr->sap_id = MMI_L4C_SAP;
   ilm_ptr->dest_mod_id = MOD_L4C;
   
   ilm_ptr->msg_id = msg_id;
   ilm_ptr->local_para_ptr = (local_para_struct*)local_param_ptr;
   ilm_ptr->peer_buff_ptr = (peer_buff_struct*) peer_buf_ptr;


   msg_send_ext_queue(ilm_ptr);

#endif
} 

#endif /* __MMI_FACTORY_MODE__ */

