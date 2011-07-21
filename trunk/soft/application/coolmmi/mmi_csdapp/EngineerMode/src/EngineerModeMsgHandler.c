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
 * EngineerModeMsgHandler.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for engineer mode message handler
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

#ifdef __MMI_ENGINEER_MODE__  

/**********************/
/*****  Header Files *****/
/**********************/
#include "stdc.h"
#include "globalconstants.h"
#include "debuginitdef.h"
#include "wgui.h"
#include "globalstruct.h"
#include "mmi_data_types.h"
#include "wgui_categories.h"
#include "historygprot.h"
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
#include "taskinit.h"
#include "engineermodedef.h"
#include "engineermodetype.h"
#include "engineermodeprot.h"
 
#include "protocolevents.h"
#include "engineermodepatch.h"
//micha0202
#include "networksetupgprot.h"
#include "wgui_status_icons.h"

#include "mdi_datatype.h"
#include "mdi_audio.h"

#ifdef MMI_ON_HARDWARE_P
#include "uart_sw.h"    /* uart_port type */
#include "med_struct.h"
   #include "mmi_msg_struct.h"
	#include "l4dr.h"
	#include "custom_util.h"
	#include "lcd_sw.h"
   #include "resource_audio.h"
#ifndef WIN32
	#include "l1audio.h"
#endif
#endif

extern U8 EMBandIndex[EM_BANDSEL_MAX];
extern U8       currentHighlightIndex;
 
extern kal_uint8 SWDBG_Profile;
#ifdef MMI_ON_HARDWARE_P
extern UART_PORT TST_PORT;
extern UART_PORT PS_UART_PORT;
extern UART_baudrate TST_BAUDRATE;
extern UART_baudrate PS_BAUDRATE;
#endif

/* gprs */
#ifdef __MOD_TCM__
kal_uint8 g_gprs_pdp_idx=0;
kal_bool g_gprs_set_minqos_required=0;
static const kal_uint16 pdp_context_idx[16][2][6] = {
      {{1,2,4,5,5,16}, {0,0,0,0,0,0}}, //[PDP context1] 
      {{1,2,4,3,5,16}, {0,0,0,0,0,0}}, //[PDP context2] 
      {{1,2,4,5,5,16}, {0,0,4,0,2,0}}, //[PDP context3]
      {{1,2,4,3,6,16}, {0,0,0,0,0,0}}, //[PDP context4] 
      {{1,2,4,3,5,16}, {0,0,0,0,0,0}}, //[PDP context5] 
      {{1,2,4,5,6,16}, {0,0,0,0,0,0}}, //[PDP context6] 
      {{1,2,4,5,6,16}, {0,0,0,0,0,0}}, //[PDP context7] 
      {{1,2,4,3,5,16}, {0,0,0,0,0,0}}, //[PDP context8] 
      {{1,2,4,3,5,16}, {0,0,0,0,0,0}}, //[PDP context9] 
      {{1,2,4,5,5,16}, {0,0,0,0,0,0}}, //[PDP context10] 
      {{1,2,4,2,5,16}, {0,0,0,0,0,0}}, //[PDP context11] 
      {{1,2,4,2,5,16}, {0,0,0,0,0,0}}, //[PDP context12] 
      {{1,2,4,1,5,16}, {0,0,0,0,0,0}}, //[PDP context13] 
      {{1,2,4,3,5,16}, {0,0,0,0,0,0}}, //[PDP context14] 
      {{1,2,1,3,7,9 }, {0,0,0,0,0,0}}  //[PDP context15]
};

#endif /* __MOD_TCM__ */

extern U8 EmPortSetting;
extern U8 EmAppPortArray[EM_APP_MAX_NUM];
extern U8 EmAppBaudArray[EM_APP_MAX_NUM];
static U8 EmSetUartMsgDepth = 0;
kal_bool UART_setting = 0;

#ifdef MMI_ON_HARDWARE_P
extern UART_PORT TST_PORT;
extern UART_baudrate TST_BAUDRATE;
static UART_PORT   old_tst_port;
static UART_baudrate old_tst_baudrate;

#ifndef L4_NOT_PRESENT
extern UART_PORT PS_UART_PORT;
extern UART_baudrate PS_BAUDRATE;
static UART_PORT   old_ps_port;
static UART_baudrate old_ps_baudrate;
#endif
#endif

#ifdef MMI_ON_HARDWARE_P
extern kal_bool High_Speed_SIM;
#ifdef __TST_MODULE__
#include "tst.h"        /* tst extern functions */

extern UART_PORT TST_PORT;
extern UART_baudrate TST_BAUDRATE;

#ifndef L4_NOT_PRESENT
extern UART_PORT PS_UART_PORT;
extern UART_baudrate PS_BAUDRATE;
#endif

#endif
#endif

extern void mmi_em_display_init(void);
extern void mmi_em_trace_on_ind(void);
//extern void mmi_em_send_start_req(module_type mod_id, request_info_type info_id);
extern void mmi_em_trace_off_ind(void);
extern void mmi_em_tracer_end_req(void);

	
void EntryAtEngineerModeMenu(void *inMsg)
{
     DisplayIdleScreen();
     EntryEngineerModeMenu();
}

// Lisen 0906
#ifdef MMI_ON_HARDWARE_P
extern void mmi_em_send_start_req(module_type mod_id, request_info_type info_id);
#endif

void EngineerModeStartReq(U32 mod_id, U32 info_req)
{
/*
#ifdef MMI_ON_HARDWARE_P
   mmi_em_start_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_em_start_req_struct));
   msg_p->mod_id = mod_id;
   msg_p->info_request = info_req;
   EngineerModeSendMsg(MSG_ID_EM_START_REQ, msg_p, NULL);
#endif   
*/
#ifdef MMI_ON_HARDWARE_P
   mmi_em_display_init();
   mmi_em_trace_on_ind();
   //mmi_em_tracer_start_req();
   mmi_em_send_start_req(mod_id, info_req);
#endif
}

void EngineerModeStopReq(U32 mod_id, U32 info_req)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_em_trace_off_ind();
   mmi_em_tracer_end_req();
#endif		
/*
#ifdef MMI_ON_HARDWARE_P
   mmi_em_stop_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_em_stop_req_struct));
   msg_p->mod_id = mod_id;
   msg_p->info_request = info_req;
   EngineerModeSendMsg(MSG_ID_EM_STOP_REQ, msg_p, NULL);
#endif   
*/
}


void EngineerModeStartRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_em_start_rsp_struct* rsp_p = inMsg;
   if (rsp_p->result)
   {
      EngineerModeStatusUpdate(rsp_p->mod_id,rsp_p->info_request);
      EngineerModeSuccessNotice(EM_UPDATE_SUCCESS_TEXT);
   }
   else
      EngineerModeErrorNotice(EM_UPDATE_ERROR_TEXT);
#endif   
}

void EngineerModeStopRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_em_stop_rsp_struct* rsp_p = inMsg;
   if (rsp_p->result)
   {
      EngineerModeStatusUpdate(rsp_p->mod_id,rsp_p->info_request);
      //EngineerModeSuccessNotice(EM_UPDATE_SUCCESS_TEXT);
   }
   else
      EngineerModeErrorNotice(EM_UPDATE_ERROR_TEXT);
#endif   
}

void EngineerModePlayAudioReq(U8 index, U8 type)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_play_audio_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_play_audio_req_struct));
   switch(type)
   {
      case AUD_MIDI:
         msg_p->sound_id = index+MIN_MIDI_ID;
         break;
      case AUD_IMELODY:
         msg_p->sound_id = index+MIN_RING_TONE_ID;
         break;
      case AUD_SOUND:
         msg_p->sound_id = index+MIN_SND_ID;
         break;
      default:
         msg_p->sound_id = index+MIN_MIDI_ID;
         break;
   }
   msg_p->style = DEVICE_AUDIO_PLAY_INFINITE;
   EngineerModeSendMsg(MSG_ID_MMI_EQ_PLAY_AUDIO_REQ, msg_p, NULL);
#endif   
}


void EngineerModeStopAudioReq(U8 index, U8 type)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_stop_audio_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_stop_audio_req_struct));
   switch(type)
   {
      case AUD_MIDI:
         msg_p->sound_id = index+MIN_MIDI_ID;
         break;
      case AUD_IMELODY:
         msg_p->sound_id = index+MIN_RING_TONE_ID;
         break;
      case AUD_SOUND:
         msg_p->sound_id = index+MIN_SND_ID;
         break;
      default:
         msg_p->sound_id = index+MIN_MIDI_ID;
         break;
   }
   EngineerModeSendMsg(MSG_ID_MMI_EQ_STOP_AUDIO_REQ, msg_p, NULL);
#endif   
}


void EngineerModeStatusInd(void* inMsg)
{
   EngineerModeStatusIndHdlr(inMsg);
}

void EngineerModeSetVolumeReq(U8 volumeType, U8 volumeLevel)
{
#ifdef MMI_ON_HARDWARE_P

   switch(volumeType)
   {
      case MDI_VOLUME_CTN:
         TONE_SetOutputVolume(volumeLevel, volumeLevel);
         break;
      case MDI_VOLUME_KEY:
         KT_SetOutputVolume(volumeLevel, volumeLevel);
         break;
      case MDI_VOLUME_MIC:
         L1SP_SetMicrophoneVolume(volumeLevel);
         break;
      case MDI_VOLUME_FMR:
         Media_SetOutputVolume(volumeLevel, volumeLevel);
         break;
      case MDI_VOLUME_SPH:
         L1SP_SetOutputVolume(volumeLevel, volumeLevel);
         break;
      case MDI_VOLUME_SID:
         L1SP_SetSidetoneVolume(volumeLevel);
         break;
      case MDI_VOLUME_MEDIA:
         Media_SetOutputVolume(volumeLevel, volumeLevel);
         break;
      default:
         break;

   }

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
#endif   

   EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);   

}


void EngineerModeGetAudioProfileReq(U8 mode)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_get_audio_profile_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_get_audio_profile_req_struct));
   msg_p->audio_prof_in.mode = mode;
   EngineerModeSendMsg(MSG_ID_MMI_EQ_GET_AUDIO_PROFILE_REQ, msg_p, NULL);
#endif   
}

void EngineerModeGetAudioProfileRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_get_audio_profile_rsp_struct* rsp_p = inMsg;
   if (rsp_p->result)
   {
      EmUpdateAudioProfile(rsp_p->audio_prof_out.speech, rsp_p->audio_prof_out.keytone, 
                           rsp_p->audio_prof_out.melody, rsp_p->audio_prof_out.sound,
                        rsp_p->audio_prof_out.mic, rsp_p->audio_prof_out.sidetone);
   }
#endif   

}



void EngineerModeSetAudioProfileReq(void *audioProfile)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_set_audio_profile_req_struct* msg_p;
   EmAudioProfileStruct *p = (EmAudioProfileStruct*)audioProfile;
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_set_audio_profile_req_struct));
   msg_p->audio_prof.mode = p->mode;
   memcpy(msg_p->audio_prof.speech, p->speech, MAX_EM_VOLUME_LEVEL);
   memcpy(msg_p->audio_prof.keytone, p->keytone, MAX_EM_VOLUME_LEVEL);
   memcpy(msg_p->audio_prof.melody, p->melody, MAX_EM_VOLUME_LEVEL);
   memcpy(msg_p->audio_prof.sound, p->sound, MAX_EM_VOLUME_LEVEL);
   memcpy(msg_p->audio_prof.mic, p->mic, MAX_EM_VOLUME_LEVEL);
   msg_p->audio_prof.sidetone = p->sidetone;
   EngineerModeSendMsg(MSG_ID_MMI_EQ_SET_AUDIO_PROFILE_REQ, msg_p, NULL);
#else
   EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);   
#endif   
}

void EngineerModeSetAudioProfileRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_set_audio_profile_rsp_struct* rsp_p = inMsg;
   if (rsp_p->result)
   {
      EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
   }
   else
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
#endif

}

void EngineerModeSetAudioMode(U8 mode)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_set_audio_mode_req_struct* msg_p;
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_set_audio_mode_req_struct));
   msg_p->mode = mode;
   EngineerModeSendMsg(MSG_ID_MMI_EQ_SET_AUDIO_MODE_REQ, msg_p, NULL);
#endif  
}

void EngineerModeGetAudioParamReq()
{
#ifdef MMI_ON_HARDWARE_P
   EngineerModeSendMsg(MSG_ID_MMI_EQ_GET_AUDIO_PARAM_REQ, NULL, NULL);
#else
   extern EmAudioParamStruct AudioParameter;
   EmUpdateAudioParam((U8*) &AudioParameter);
#endif  
}

void EngineerModeGetAudioParamRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_get_audio_param_rsp_struct* rsp_p = (mmi_eq_get_audio_param_rsp_struct*)inMsg;
   if (rsp_p->result)
   {
      EmUpdateAudioParam( (U8 *)&(rsp_p->audio_para) );
   }
   else
      EngineerModeWarningNotice(EM_AUD_SET_ERROR_TEXT);   
#endif   

}

void EngineerModeSetAudioParamReq(void *audioParam)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_set_audio_param_req_struct* msg_p;
   EmAudioParamStruct *p = (EmAudioParamStruct*)audioParam;
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_set_audio_param_req_struct));
   msg_p->audio_para.selected_FIR_output_index = p->selected_FIR_output_index;
   msg_p->audio_para.es_time_const = p->es_time_const;
   msg_p->audio_para.es_time_const2 = p->es_time_const2;
   msg_p->audio_para.es_vol_const = p->es_vol_const;
   msg_p->audio_para.es_vol_const2 = p->es_vol_const2;
   msg_p->audio_para.max_swing = p->max_swing;

   EngineerModeSendMsg(MSG_ID_MMI_EQ_SET_AUDIO_PARAM_REQ, msg_p, NULL);
#else
   EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);   
#endif   


}

void EngineerModeSetAudioParamRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_set_audio_param_rsp_struct* rsp_p = inMsg;
   if (rsp_p->result)
   {
      EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
   }
   else
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
#endif

}



/* gprs */
#ifdef MMI_ON_HARDWARE_P
#ifdef __MOD_TCM__
static void EngineerModeGprsActFillstruct( l4c_qos_struct *l4c_qos_st,  kal_uint8 idx, kal_uint8 ismin )
{
      if( ismin != 0 )
      {
          ismin = 1;
          l4c_qos_st->qos_type = ATCI_MIN_QOS;
      }
      else
      {
          ismin = 0;
          l4c_qos_st->qos_type = ATCI_REQ_QOS;
      }
      
      idx-=1;
      // Lisen 0906
      if( idx > 14 /* || idx < 0 */)
         idx = 0;
	
      l4c_qos_st->context_id = 2;
      l4c_qos_st->delay_class = pdp_context_idx[idx][ismin][2];
      l4c_qos_st->reliability_class = pdp_context_idx[idx][ismin][3];
      l4c_qos_st->peak_throughput = pdp_context_idx[idx][ismin][4];
      l4c_qos_st->precedence_class = pdp_context_idx[idx][ismin][1];
      l4c_qos_st->mean_throughput = pdp_context_idx[idx][ismin][5];
      
}
#endif /* __MOD_TCM__ */
#endif

void EngineerModeGprsActivatePDPContext(kal_uint8 pdp_idx)
{
#ifdef MMI_ON_HARDWARE_P
#ifdef __MOD_TCM__
   mmi_ps_set_definition_req_struct* msg_p;

   msg_p = OslConstructDataPtr(sizeof(mmi_ps_set_definition_req_struct));
   msg_p->pdp.context_id = 2;
   msg_p->pdp.apn_length = 12;
   kal_sprintf( (kal_char *)msg_p->pdp.apn, "%s", "smg7.etsi.fr");
   msg_p->pdp.dcomp_algo = 0;
   msg_p->pdp.hcomp_algo = 0;
   msg_p->pdp.pdp_type = IPV4_ADDR_TYPE;
   msg_p->pdp.pdp_addr_len = IPV4_ADDR_LEN;
   msg_p->pdp.addr_val[0] = 192; msg_p->pdp.addr_val[1] = 168; msg_p->pdp.addr_val[2] =1; msg_p->pdp.addr_val[3] =1;

   EngineerModeSendMsg(MSG_ID_MMI_PS_SET_DEFINITION_REQ, msg_p, NULL);
#endif /* __MOD_TCM__ */
#endif  /* MMI_ON_HARDWARE_P */

#ifdef _WIN32
   EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);   
#endif

}

void EngineerModeGprsSetDefinitionRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
#ifdef __MOD_TCM__  
   mmi_ps_set_definition_rsp_struct* rsp_p = inMsg;

   if (rsp_p->result == KAL_FALSE )
   {
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
   }
   else
   {

      mmi_ps_set_qos_req_struct* msg_p;
      //kal_uint8 qos_type;
      msg_p = OslConstructDataPtr(sizeof(mmi_ps_set_qos_req_struct));
      msg_p->qos_type = ATCI_REQ_QOS;
      EngineerModeGprsActFillstruct( &msg_p->min, g_gprs_pdp_idx, 0 );
      
      EngineerModeSendMsg(MSG_ID_MMI_PS_SET_QOS_REQ, msg_p, NULL);
   }
#endif  /* __MOD_TCM__ */   
#endif  /* MMI_ON_HARDWARE_P */  
}

void EngineerModeGprsSetQosRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
#ifdef __MOD_TCM__  
   mmi_ps_set_qos_rsp_struct* rsp_p = inMsg;

   if (rsp_p->result == KAL_FALSE )
   {
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
      return;
   }

/* Joseph: Do not set min QoS here. Set Min QoS manually from CATCHER */   
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif    
   {
        /* Send ACTIVATE Request */
         mmi_ps_act_test_req_struct* msg_p;
         //kal_uint8 qos_type;

         msg_p = OslConstructDataPtr(sizeof(mmi_ps_act_test_req_struct));
         msg_p->opcode = TCM_PDP_ACTIVATE;
         msg_p->cid = 2;
         EngineerModeSendMsg(MSG_ID_MMI_PS_ACT_TEST_REQ, msg_p, NULL);
   }
#endif /* __MOD_TCM__  */
#endif  /* MMI_ON_HARDWARE_P */

}


void EngineerModeGprsActTestRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
#ifdef __MOD_TCM__  
   mmi_ps_act_test_rsp_struct* rsp_p = inMsg;
   if (rsp_p->result)
   {
      EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
   }
   else
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
#endif /* __MOD_TCM__  */
#endif  /* MMI_ON_HARDWARE_P */

}


void EngineerModeGprsAttachReq(void)
{
#ifdef MMI_ON_HARDWARE_P
#ifdef __MOD_TCM__  

   mmi_nw_set_attach_req_struct* msg_p;
   
//micha0120
   SetProtocolEventHandler (EngineerModeGprsAttachRes, MSG_ID_MMI_NW_SET_ATTACH_RSP);

   msg_p = OslConstructDataPtr(sizeof(mmi_nw_set_attach_req_struct));
   msg_p->type = RAC_PS;
   msg_p->opcode = RAC_ATTACH;
   msg_p->is_poweroff = KAL_FALSE;

   EngineerModeSendMsg(MSG_ID_MMI_NW_SET_ATTACH_REQ, msg_p, NULL);

#endif /* __MOD_TCM */
#endif /* MMI_ON_HARDWARE_P */
   
#ifdef _WIN32
   EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);   
#endif
}

void EngineerModeGprsAttachRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
#ifdef __MOD_TCM__  
//micha0120 begin
 
   mmi_nw_set_attach_rsp_struct* rsp_p = inMsg;
   if (rsp_p->result == MMI_TRUE)
   {
      EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
   }
   else
   {
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
   }
//micha0120 end
#endif /* __MOD_TCM */
#endif   

}

void  EngineerModeGprsDeactivatePDPReq(void)
{
#ifdef MMI_ON_HARDWARE_P
#ifdef __MOD_TCM__  
   mmi_ps_act_test_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_ps_act_test_req_struct));
   msg_p->opcode = TCM_PDP_DEACTIVATE;
   msg_p->cid = 2;

   EngineerModeSendMsg(MSG_ID_MMI_PS_ACT_TEST_REQ, msg_p, NULL);
#endif /* __MOD_TCM */
#endif   

#ifdef _WIN32
   EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);   
#endif
}

void  EngineerModeGprsDeactivatePDPRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
#ifdef __MOD_TCM__  
   mmi_ps_act_test_rsp_struct* rsp_p = inMsg;
   if (rsp_p->result)
   {
      EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
   }
   else
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
#endif /* __MOD_TCM */
#endif   
}
void  EngineerModeGprsSendDataReq(kal_uint32 size)
{
#ifdef MMI_ON_HARDWARE_P
#ifdef __MOD_TCM__  
   mmi_ps_send_data_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_ps_send_data_req_struct));
   msg_p->size = size;

   EngineerModeSendMsg(MSG_ID_MMI_PS_SEND_DATA_REQ, msg_p, NULL);
#endif /* __MOD_TCM */
#endif   

#ifdef _WIN32
   EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);   
#endif

}

void  EngineerModeGprsSendDataRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
#ifdef __MOD_TCM__  
   mmi_ps_send_data_rsp_struct* rsp_p = inMsg;
   if (rsp_p->result)
   {
      EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
   }
   else
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
#endif /* __MOD_TCM */
#endif   
}

void EngineerModeGetBandReq(void)
{
   EmAssignBandSelMenuItem(NULL, NULL);

#ifdef MMI_ON_HARDWARE_P
   EngineerModeSendMsg(MSG_ID_MMI_NW_GET_BAND_REQ, NULL, NULL);
#endif   

#ifdef _WIN32
   currentHighlightIndex = 0;
   EntryEmBandSelection();
#endif

}

void EngineerModeGetBandRsp(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_nw_get_band_rsp_struct* rsp_p = inMsg;
	 
   U8 index, MMI_Idx = 0;

   if (rsp_p->result)
   {
      switch(rsp_p->type)
      {
         case MMI_BAND_900:
			MMI_Idx = EM_BANDSEL_900;
            break;
         case MMI_BAND_1800:
			MMI_Idx = EM_BANDSEL_1800;
            break;
         case MMI_BAND_DUAL:
			MMI_Idx = EM_BANDSEL_DUAL;
            break;
         case MMI_BAND_1900:
			MMI_Idx = EM_BANDSEL_1900;
            break;
//Lisen 05312005	
//#if defined(__AUTO_BAND_SWITCH__)
#if (!defined(__GSM850__) && defined(__EGSM900__) && defined(__DCS1800__) && defined(__PCS1900__)) || \
    (defined(__GSM850__) && !defined(__EGSM900__) && defined(__DCS1800__) && defined(__PCS1900__)) || \
    (defined(__GSM850__) && defined(__EGSM900__) && !defined(__DCS1800__) && defined(__PCS1900__)) || \
    (defined(__GSM850__) && defined(__EGSM900__) && defined(__DCS1800__) && !defined(__PCS1900__)) || \
    (defined(__GSM850__) && defined(__EGSM900__) && defined(__DCS1800__) && defined(__PCS1900__))

         case MMI_BAND_AUTO:
                     MMI_Idx = EM_BANDSEL_AUTO;
            break;

#endif
//#endif         
            
         default:
			MMI_Idx = EM_BANDSEL_DUAL;
            break;
      }
   }

   for(index=0; index<EM_BANDSEL_MAX; index++)
   {
		if( EMBandIndex[index] == MMI_Idx )
			break;
   }

   if( index >= EM_BANDSEL_MAX )
	index = 0;

   currentHighlightIndex = index;
   EntryEmBandSelection();

#endif /* __MOD_TCM */


}


void EngineerModeSetBandReq(U8 index)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_nw_set_preferred_band_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_nw_set_preferred_band_req_struct));
   if( index < EM_BANDSEL_MAX )
   {
	   switch(EMBandIndex[index])
	   {
		  case EM_BANDSEL_900:
         msg_p->band = MMI_BAND_900;
         break;
		  case EM_BANDSEL_1800:
         msg_p->band = MMI_BAND_1800;
         break;
		  case EM_BANDSEL_DUAL:
         msg_p->band = MMI_BAND_DUAL;
         break;
		  case EM_BANDSEL_1900:
         msg_p->band = MMI_BAND_1900;
         break;
		  case EM_BANDSEL_850:
         msg_p->band = MMI_BAND_850;
         break;
//Lisen 05312005	
//#if defined(__AUTO_BAND_SWITCH__)
		  case EM_BANDSEL_AUTO:
         msg_p->band = MMI_BAND_AUTO;
         break;
//#endif         

      default:
         msg_p->band = MMI_BAND_DUAL;
         break;
   }
   }
   else
   {
	   msg_p->band = MMI_BAND_DUAL;
   }

   EngineerModeSendMsg(MSG_ID_MMI_NW_SET_PREFERRED_BAND_REQ, msg_p, NULL);

#endif   

   //EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);   

}

U32 EM_GetBaudRateByIdx(U8 idx)
{
   switch( idx )
      {
          case 0: return UART_BAUD_AUTO;
          case 1: return UART_BAUD_75; 
          case 2: return UART_BAUD_150; 
          case 3: return UART_BAUD_300; 
          case 4: return UART_BAUD_600; 
          case 5: return UART_BAUD_1200; 
          case 6: return UART_BAUD_2400; 
          case 7: return UART_BAUD_4800; 
          case 8: return UART_BAUD_7200; 
          case 9: return UART_BAUD_9600; 
          case 10: return UART_BAUD_14400;
          case 11: return UART_BAUD_19200;
          case 12: return UART_BAUD_28800; 
          case 13: return UART_BAUD_33900; 
          case 14: return UART_BAUD_38400; 
          case 15: return UART_BAUD_57600; 
          case 16: return UART_BAUD_115200; 
          case 17: return UART_BAUD_230400; 
          case 18: return UART_BAUD_460800; 
          case 19: return UART_BAUD_921600; 
          default:
            return UART_BAUD_115200; 
      }

}


U8 EM_GetIdxByBaudRate(U32 baud)
{
   switch( baud )
      {
          case UART_BAUD_AUTO: return 0;
          case UART_BAUD_75: return 1; 
          case UART_BAUD_150: return 2; 
          case UART_BAUD_300: return 3; 
          case UART_BAUD_600: return 4; 
          case UART_BAUD_1200: return 5; 
          case UART_BAUD_2400: return 6; 
          case UART_BAUD_4800: return 7; 
          case UART_BAUD_7200: return 8; 
          case UART_BAUD_9600: return 9; 
          case UART_BAUD_14400: return 10;
          case UART_BAUD_19200: return 11;
          case UART_BAUD_28800: return 12; 
          case UART_BAUD_33900: return 13; 
          case UART_BAUD_38400: return 14; 
          case UART_BAUD_57600: return 15; 
          case UART_BAUD_115200: return 16; 
          case UART_BAUD_230400: return 17; 
          case UART_BAUD_460800: return 18; 
          case UART_BAUD_921600: return 19; 
          default:
            return 0; 
      }

}


void EngineerModeGetUartReq(U8 ConfApp)
{
#ifdef MMI_ON_HARDWARE_P
   SetProtocolEventHandler (EngineerModeGetUartRes, MSG_ID_EM_GET_UART_RSP);
   EngineerModeSendMsg(MSG_ID_MMI_EQ_GET_UART_REQ, NULL, NULL);
#else
   EmAppPortArray[EM_APP_TST] = 0;
   EmAppPortArray[EM_APP_PS] = 1;
   EntryEmSetUartPort();
#endif
}


void  EngineerModeGetUartRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_get_uart_rsp_struct* rsp_p = inMsg;
   if (rsp_p->result)
   {
       EmAppPortArray[EM_APP_TST]= rsp_p->tst_uart_port;
       EmAppPortArray[EM_APP_PS]= rsp_p->ps_uart_port;
	       
       EmAppBaudArray[EM_APP_TST] = EM_GetIdxByBaudRate( rsp_p->tst_baud_rate );
       EmAppBaudArray[EM_APP_PS] = EM_GetIdxByBaudRate (rsp_p->ps_baud_rate );

       EntryEmSetUartPort();   
   }
#endif   
}

void EngineerModeSetHighSpeedSIMReq()
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_set_uart_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_set_uart_req_struct));
   msg_p->ps_port = PS_UART_PORT;
   msg_p->tst_port = TST_PORT;
   msg_p->ps_baud_rate = PS_BAUDRATE;
   msg_p->tst_baud_rate = TST_BAUDRATE;

   // Lisen 0813
   msg_p->High_Speed_SIM_Enabled = High_Speed_SIM;
   UART_setting = FALSE;         	
   EngineerModeSendMsg(MSG_ID_MMI_EQ_SET_UART_REQ, msg_p, NULL);
#else   
   EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);   
   
#endif
}

void EngineerModeSetUartReq()
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_set_uart_req_struct* msg_p;
//Lisen 05032005     Begin
   extern kal_uint8 SWDBG_Profile;
//Lisen 05032005     End
   
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_set_uart_req_struct));
   msg_p->ps_port = EmAppPortArray[EM_APP_PS];
   msg_p->tst_port = EmAppPortArray[EM_APP_TST];
   msg_p->ps_baud_rate = EM_GetBaudRateByIdx( EmAppBaudArray[EM_APP_PS] );
   msg_p->tst_baud_rate = EM_GetBaudRateByIdx( EmAppBaudArray[EM_APP_TST] );
   msg_p->High_Speed_SIM_Enabled = High_Speed_SIM;  	

//Lisen 05032005     Begin
   msg_p->swdbg = SWDBG_Profile;  	
//Lisen 05032005     End

   old_tst_port = TST_PORT;
   old_ps_port = PS_UART_PORT;
   old_tst_baudrate = TST_BAUDRATE;
   old_ps_baudrate = PS_BAUDRATE;

   //Lisen 07132005
   // UART_setting = TRUE;
   	
   EngineerModeSendMsg(MSG_ID_MMI_EQ_SET_UART_REQ, msg_p, NULL);

#else   
   EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);   
   
#endif
}

void  EngineerModeSetUartRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_set_uart_rsp_struct* rsp_p = inMsg;

   if (rsp_p->result)
   {
//Lisen 09062005
#ifndef __DYNAMIC_UART_PORT_SWITCHING__
      EngineerModeSuccessRebootNotice(EM_NOTICE_SUCCESS_REBOOT_TEXT);
#else
      if (UART_setting == TRUE  &&
      	   (old_tst_port != EmAppPortArray[EM_APP_TST] ||
           old_ps_port != EmAppPortArray[EM_APP_PS] ||
      	    old_tst_baudrate != EM_GetBaudRateByIdx(EmAppBaudArray[EM_APP_TST])  ||
      	    old_ps_baudrate != EM_GetBaudRateByIdx(EmAppBaudArray[EM_APP_PS])) )
      {
         EngineeringModeUartSendDetachReq();
         EmSetUartMsgDepth = 0;
      }
      else
      {
         DeleteScreenIfPresent( EM_DEV_INLINE_CONFIG_UART_MENU_SCR );
         EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
      }

      UART_setting = FALSE;      
#endif      //__SHUTDOWN_AFTER_UART_PORT_SETTING__
   }
   else
   {
   DeleteScreenIfPresent( EM_DEV_INLINE_CONFIG_UART_MENU_SCR );
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
   }
#endif   
}

void  EngineeringModeUartSendDetachReq()
{
#ifdef MMI_ON_HARDWARE_P
   mmi_detach_uart_port_req_struct* msp_p_detach_uart1;
   mmi_detach_uart_port_req_struct* msp_p_detach_uart2;
   
   EmSetUartMsgDepth = 0;

   // Detach for TST
   msp_p_detach_uart1 = OslConstructDataPtr(sizeof(mmi_detach_uart_port_req_struct));
   msp_p_detach_uart1->module_id = MOD_TST_READER;
   msp_p_detach_uart1->port = old_tst_port;

   // Detach for L4C
   msp_p_detach_uart2 = OslConstructDataPtr(sizeof(mmi_detach_uart_port_req_struct));
   msp_p_detach_uart2->module_id = MOD_L4C;
   msp_p_detach_uart2->port = old_ps_port;

   EngineerModeSendMsg(MSG_ID_MMI_DETACH_UART_PORT_REQ, msp_p_detach_uart1, NULL);
   EngineerModeSendMsg(MSG_ID_MMI_DETACH_UART_PORT_REQ, msp_p_detach_uart2, NULL);
#endif   
}

void  EngineerModeDetachUartRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_attach_uart_port_rsp_struct* rsp_p = inMsg;

   if (!rsp_p->result)
   {
      EmSetUartMsgDepth++;
      if (EmSetUartMsgDepth==2)
      {
         EngineeringModeUartSendAttachReq();
         EmSetUartMsgDepth = 0;
      }
   }
   else
   {
      DeleteScreenIfPresent( EM_DEV_INLINE_CONFIG_UART_MENU_SCR );
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
   }
#endif   
}

//Lisen 1111
void  EngineeringModeUartSendAttachReq()
{
#ifdef MMI_ON_HARDWARE_P
   mmi_attach_uart_port_req_struct* msp_p_uart1;
   mmi_attach_uart_port_req_struct* msp_p_uart2;

   // Attach for TST
   msp_p_uart1 = OslConstructDataPtr(sizeof(mmi_attach_uart_port_req_struct));
   msp_p_uart1->module_id = MOD_TST_READER;
   msp_p_uart1->port = EmAppPortArray[EM_APP_TST];
   msp_p_uart1->baud_rate = EM_GetBaudRateByIdx( EmAppBaudArray[EM_APP_TST] );
       
   // Attach for L4C
   msp_p_uart2 = OslConstructDataPtr(sizeof(mmi_attach_uart_port_req_struct));
   msp_p_uart2->module_id = MOD_L4C;
   msp_p_uart2->port = EmAppPortArray[EM_APP_PS];
   msp_p_uart2->baud_rate = EM_GetBaudRateByIdx( EmAppBaudArray[EM_APP_PS] );

   EmSetUartMsgDepth = 0;
   EngineerModeSendMsg(MSG_ID_MMI_ATTACH_UART_PORT_REQ, msp_p_uart1, NULL);
   EngineerModeSendMsg(MSG_ID_MMI_ATTACH_UART_PORT_REQ, msp_p_uart2, NULL);
#endif
}

void  EngineerModeAttachUartRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_attach_uart_port_rsp_struct* rsp_p = inMsg;

   if (!rsp_p->result)
   {
      EmSetUartMsgDepth++;
      if (EmSetUartMsgDepth==2)
      {
         DeleteScreenIfPresent( EM_DEV_INLINE_CONFIG_UART_MENU_SCR );
      EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
         EmSetUartMsgDepth = 0;
      }
   }
   else
   {
      DeleteScreenIfPresent( EM_DEV_INLINE_CONFIG_UART_MENU_SCR );
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
   }
#endif   
}

void EngineerModeSetSleepModeReq(U8 index)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_set_sleep_mode_req_struct* msg_p;
   
   msg_p = OslConstructDataPtr(sizeof(mmi_eq_set_sleep_mode_req_struct));
   if( index == 0 ) /* Enable */
      msg_p->on_off = KAL_TRUE;
   else /* Disable */
      msg_p->on_off = KAL_FALSE;
      
   EngineerModeSendMsg(MSG_ID_MMI_EQ_SET_SLEEP_MODE_REQ, msg_p, NULL);
#else   
   EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);   
#endif

}

void  EngineerModeSetSleepModeRes(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_set_sleep_mode_rsp_struct* rsp_p = inMsg;
   if (rsp_p->result)
   {
      EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
   }
   else
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
#endif   
}



void EngineerModeSendMsg(U32 msg_id, 
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

void EngineerModeSendMsg_Ext(U32 msg_id, U16 dest_mod_id, void *local_param_ptr, void *peer_buf_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *ilm_ptr = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ilm_ptr = allocate_ilm(MOD_MMI);
    ilm_ptr->src_mod_id = MOD_MMI;
    ilm_ptr->sap_id = BT_APP_SAP;
    ilm_ptr->dest_mod_id = dest_mod_id;

    ilm_ptr->msg_id = (kal_uint16) msg_id;
    ilm_ptr->local_para_ptr = (local_para_struct*) local_param_ptr;
    ilm_ptr->peer_buff_ptr = (peer_buff_struct*) peer_buf_ptr;

    msg_send_ext_queue(ilm_ptr);

}
#endif /* __MMI_ENGINEER_MODE__ */


