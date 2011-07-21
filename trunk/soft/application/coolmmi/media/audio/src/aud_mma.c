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

/*******************************************************************************
 * Filename:
 * ---------
 *   aud_mma.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is to handle all messages for Java MMAPI. 
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT


/*==== INCLUDES =========*/

/* system includes */
#include "kal_release.h" 
#include "stack_common.h"  
#include "stack_msgs.h"
#include "app_ltlcom.h"          /* task message communiction */
#include "stacklib.h"           /* basic type for dll, evshed, stacktimer */
#include "event_shed.h"          /* event scheduler */
#include "stack_timer.h"         /* stack timer */

/* global includes */
#include "l1audio.h"
#include "device.h"
#include "custom_equipment.h"
#include "resource_audio.h"
#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"


/* local includes */
#include "med_global.h"
#include "med_main.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "aud_main.h"
#include "med_main.h"
#include "med_utility.h"

#include "media_others.h"
#include "mmi_trace.h"       //added by elisa for mem_cpy

//#include "mmf_vid_avi.h"
#ifdef __MED_MMA_MOD__

typedef  void (*media_handler)( Media_Handle *handle, Media_Event event );


/*==== FUNCTIONS ===========*/


/*************************************************************************
* FUNCTION
*	aud_mma_startup
*
* DESCRIPTION
*	This function is to start mma module. 
*
* PARAMETERS
*	None
*  
* RETURNS
*	kal_bool
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_bool aud_mma_startup(void)
{
   kal_mem_set(aud_context_p->player, 0, sizeof(aud_player_struct)*MMA_NO_OF_PLAYER);
   return KAL_TRUE;
}

/*************************************************************************
* FUNCTION
*	aud_mma_close_all_single_player
*
* DESCRIPTION
*	This function is to close all mma single player. 
*
* PARAMETERS
*	None
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_close_all_single_player(void)
{
#ifdef MMA_AMR_SUPPORT
   aud_player_struct *player_p;
   /* close AMR player */
   player_p=&(aud_context_p->player[MMA_PLAYER_AMR]);
   if(player_p->used)
   {
      JAmr_Stop((Media_Handle*)player_p->handle);
      JAmr_Close((Media_Handle*)player_p->handle);
      player_p->used=KAL_FALSE;
      player_p->handle = NULL;
   }
#ifdef DAF_DECODE
   player_p=&(aud_context_p->player[MMA_PLAYER_DAF]);
   if(player_p->used)
   {
      Media_Stop();
      player_p->used=KAL_FALSE;
      player_p->handle = NULL;
   }
#endif
#ifdef AAC_DECODE
   player_p=&(aud_context_p->player[MMA_PLAYER_AAC]);
   if(player_p->used)
   {
      Media_Stop();
      player_p->used=KAL_FALSE;
      player_p->handle = NULL;
   }
#endif
   player_p=&(aud_context_p->player[MMA_PLAYER_AU]);
   if(player_p->used)
   {
      Media_Stop();
      player_p->used=KAL_FALSE;
      player_p->handle = NULL;
   }
#endif
}

/*************************************************************************
* FUNCTION
*	aud_mma_close_all_multi_player
*
* DESCRIPTION
*	This function is to close all multi-players. 
*
* PARAMETERS
*	None
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_close_all_multi_player(void)
{
   kal_uint8 i;
   aud_player_struct *player_p;
   /* close IMY player */
   player_p=&(aud_context_p->player[MMA_PLAYER_IMY]);
   {
      if(player_p->used)
      {
         JImy_Stop((Media_Handle*)player_p->handle);
         aud_melody_stop_driver();
         JImy_Close((Media_Handle*)player_p->handle);
         player_p->used=KAL_FALSE;
         player_p->handle = -1;
      }
   }
   /* close SMF player */
   for(i=MMA_PLAYER_SMF_1;i<=MMA_PLAYER_SMF_4;i++)
   {
      player_p=&(aud_context_p->player[i]);
      if(player_p->used)
      {
         JSmf_Stop((Media_Handle*)player_p->handle);
         JSmf_Close((Media_Handle*)player_p->handle);
         player_p->used=KAL_FALSE;
         player_p->handle = -1;
      }
   }
   /* close TONE player */
   for(i=MMA_PLAYER_TONE_S1;i<=MMA_PLAYER_TONE_S4;i++)
   {
      player_p=&(aud_context_p->player[i]);
      if(player_p->used)
      {
         JTone_Stop((Media_Handle*)player_p->handle);
         JTone_Close((Media_Handle*)player_p->handle);
         player_p->used=KAL_FALSE;
		   player_p->handle = -1;
      }
   }
   /* close WAV player */
   for(i=MMA_PLAYER_WAV_1;i<=MMA_PLAYER_WAV_3;i++)
   {
      player_p=&(aud_context_p->player[i]);
      if(player_p->used)
      {
         JWav_Stop((Media_Handle*)player_p->handle);
         JWav_Close((Media_Handle*)player_p->handle);
         med_free_ext_mem((void**)&(player_p->buf_p));
         player_p->used=KAL_FALSE;
		   player_p->handle = -1;
      }
   }
   /* close XWAV player */
   player_p=&(aud_context_p->player[MMA_PLAYER_XWAV]);
   {
      if(player_p->used)
      {
         JXWav_Stop((Media_Handle*)player_p->handle);
         JXWav_Close((Media_Handle*)player_p->handle);
         player_p->used=KAL_FALSE;
   	   player_p->handle = -1;
      }
   }
}


/*************************************************************************
* FUNCTION
*	aud_mma_close_all
*
* DESCRIPTION
*	This function is to close all mma tasks. 
*
* PARAMETERS
*	None
*  
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_close_all(void)
{
   aud_mma_close_all_multi_player();
   aud_mma_close_all_single_player();
}

/*************************************************************************
* FUNCTION
*	aud_mma_event_callback
*
* DESCRIPTION
*	This function is to handle play event callback. 
*
* PARAMETERS
*	Media_Handle *handle
*  Media_Event event
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_event_callback(Media_Handle *handle, Media_Event event)
{
   media_mma_play_event_ind_struct* ind_p = NULL;

   switch(event)
   {
      case MEDIA_LED_ON:
         aud_melody_imy_led_hdlr(KAL_TRUE);
         return;
      case MEDIA_LED_OFF:
         aud_melody_imy_led_hdlr(KAL_FALSE);
         return;
      case MEDIA_VIBRATOR_ON:
         aud_melody_imy_vibrator_hdlr(KAL_TRUE);
         return;
      case MEDIA_VIBRATOR_OFF:
         aud_melody_imy_vibrator_hdlr(KAL_FALSE);
         return;
      case MEDIA_BACKLIGHT_ON:
         aud_melody_imy_backlight_hdlr(KAL_TRUE);
         return;
      case MEDIA_BACKLIGHT_OFF:
         aud_melody_imy_backlight_hdlr(KAL_FALSE);
         return;
      case MEDIA_REPEATED:
         aud_melody_stop_driver();
         return;
      case MEDIA_TERMINATED:
         return;
      case MEDIA_ERROR:
      case MEDIA_END:
         aud_melody_stop_driver();
         break;
      default:
         break;
   }
    
   ind_p = (media_mma_play_event_ind_struct*)
            construct_local_para(sizeof(media_mma_play_event_ind_struct), TD_CTRL);  

   ind_p->handle = (kal_int32)handle;
   ind_p->event = (kal_uint8)event;

   aud_send_ilm(MOD_MED, 
               MSG_ID_MEDIA_MMA_PLAY_EVENT_IND, 
               ind_p, 
               NULL);

}

/*************************************************************************
* FUNCTION
*	aud_mma_event_callback
*
* DESCRIPTION
*	This function is to handle play event callback. 
*
* PARAMETERS
*	Media_Handle *handle
*  Media_Event event
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_event_callback_1(Media_Event event)
{
   media_mma_play_event_ind_struct* ind_p = NULL;

   switch(event)
   {
      case MEDIA_LED_ON:
      case MEDIA_LED_OFF:
      case MEDIA_VIBRATOR_ON:
      case MEDIA_VIBRATOR_OFF:
      case MEDIA_BACKLIGHT_ON:
      case MEDIA_BACKLIGHT_OFF:
      case MEDIA_REPEATED:
      case MEDIA_TERMINATED:
         return;
      case MEDIA_ERROR:
      case MEDIA_END:
         aud_melody_stop_driver();
         break;
      default:
         break;
   }
    
   ind_p = (media_mma_play_event_ind_struct*)
            construct_local_para(sizeof(media_mma_play_event_ind_struct), TD_CTRL);  

   ind_p->handle = (kal_int32)aud_context_p->current_file_handle;
   ind_p->event = (kal_uint8)event;

   aud_send_ilm(MOD_MED, 
               MSG_ID_MEDIA_MMA_PLAY_EVENT_IND, 
               ind_p, 
               NULL);

}



/*************************************************************************
* FUNCTION
*	aud_mma_play_event_ind_hdlr
*
* DESCRIPTION
*	This function is to handle play event indication. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_play_event_ind_hdlr(ilm_struct *ilm_ptr)
{
   kal_uint8 i;
   kal_int32 handle;
   media_mma_play_event_ind_struct *ind_p;
   med_handler handler;
   
   ind_p = (media_mma_play_event_ind_struct*)ilm_ptr->local_para_ptr;
   handle=ind_p->handle;

   switch(ind_p->event)
   {
      case MEDIA_END:
      case MEDIA_ERROR:
      case MEDIA_TERMINATED:
      case MEDIA_DATA_REQUEST:
      case MEDIA_DATA_NOTIFICATION:
         if(handle!=-1)
         {
            aud_player_struct *player_p;
            for(i=0;i<MMA_NO_OF_PLAYER;i++)
            {
               player_p=&(aud_context_p->player[i]);
               if(player_p->used && player_p->handle==handle)
               {
                  handler=(med_handler)player_p->handler_func;
                  if(handler)
                     handler(handle, ind_p->event);
                  break;
               }
            }
         }
         break;
      default:
         break;         
   }   
}


/*************************************************************************
* FUNCTION
*	aud_mma_open_req_hdlr
*
* DESCRIPTION
*	This function is to open media handle. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_open_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_open_req_struct *req_p;
   Media_Handle* handle=NULL;
   aud_player_struct *player_p;

   req_p = (media_mma_open_req_struct*)ilm_ptr->local_para_ptr;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_handle(-1);
      MMA_SET_EVENT(MMA_EVT_OPEN);
      return;
   }

   if(aud_context_p->speech_on)
   {
      mma_set_handle(-1);
      MMA_SET_EVENT(MMA_EVT_OPEN);
      return;
   }   

   /* stop playing tasks before open any multiple play task */
   if (AUD_IN_STATE(AUD_MEDIA_PLAY) ||
         AUD_IN_STATE(AUD_MEDIA_PLAY_PAUSED) ||
         AUD_IN_STATE(AUD_MEDIA_PLAY_FINISH))
   {
      aud_stop_unfinished_process();
   }
   
   ASSERT(aud_context_p->state==AUD_MEDIA_IDLE);
   
   switch(req_p->media_type)
   {
      case MMA_TYPE_MIDI:
      {
         kal_uint8 i;
         for(i=MMA_PLAYER_SMF_1;i<=MMA_PLAYER_SMF_4;i++)
         {
            player_p=&(aud_context_p->player[i]);
            if(!player_p->used)
            {
               Media_SMF_Param *smf_param_p;

               aud_mma_close_all_single_player();
               player_p->handler_func=req_p->handler;
               if((handle=JSmf_Open(aud_mma_event_callback, (Media_SMF_Param **)&smf_param_p))!=NULL)
               {
                  player_p->handle=(kal_int32)handle;
                  smf_param_p->smffile=(const kal_uint8*)req_p->data;
                  smf_param_p->filesize=(kal_int32)req_p->size;
                  smf_param_p->repeats=(kal_int16)req_p->repeats;
                  player_p->used=KAL_TRUE;
               }
               break;
            }
         }   
         break;
      }   
      case MMA_TYPE_TONE:
      {
         kal_uint8 i;
         for(i=MMA_PLAYER_TONE_S1;i<=MMA_PLAYER_TONE_S4;i++)
         {
            player_p=&(aud_context_p->player[i]);
            if(!player_p->used)
            {
               aud_mma_close_all_single_player();
               player_p->handler_func=req_p->handler;
               if((handle=JTone_Open(aud_mma_event_callback, (Media_ToneSeq_Param**)&player_p->param_p))!=NULL)
               {
                  player_p->handle=(kal_int32)handle;
                  player_p->used=KAL_TRUE;
               }
               break;
            }   
         }   
         break;
      }   
      case MMA_TYPE_WAV:
      {
         kal_uint8 i;
         for(i=MMA_PLAYER_WAV_1;i<=MMA_PLAYER_WAV_3;i++)
         {
            player_p=&(aud_context_p->player[i]);
            if(!player_p->used)
            {
               kal_uint32 buf_len;
               Media_Status status=MEDIA_FAIL;
               Media_Wav_Param *wav_param_p;

               aud_mma_close_all_single_player();
               player_p->handler_func=req_p->handler;

               if((handle=JWav_Open(aud_mma_event_callback, (Media_Wav_Param **)&wav_param_p))!=NULL)
               {
                  player_p->handle=(kal_int32)handle;
                  wav_param_p->wavefile=(const kal_uint8*)req_p->data;
                  wav_param_p->filesize=req_p->size;
                  wav_param_p->repeats=req_p->repeats;
                  
                  if((status=JWav_Load(handle,&buf_len))!=MEDIA_SUCCESS)
                  {
                     JWav_Close(handle);
                     handle=NULL;
                     break;
                  }
                  if((player_p->buf_p=(kal_int16*)med_alloc_ext_mem(buf_len*sizeof(kal_uint16)))!=NULL)
                  {
                     JWav_SetBuffer(handle, player_p->buf_p, buf_len);
                  }
                  else
                  {
                     JWav_Close(handle);
                     handle=NULL;
                     ASSERT(player_p->buf_p!=NULL);
                     break;
                  }

                  player_p->used=KAL_TRUE;
    
               }
               break;
            }   
         }   
         break;
      }
      case MMA_TYPE_IMY:
      {
         Media_iMelody_Param *imy_param_p;
         player_p=&(aud_context_p->player[MMA_PLAYER_IMY]);

         if(!player_p->used)
         {
            aud_mma_close_all_single_player();
            player_p->handler_func=req_p->handler;
            if((handle=JImy_Open(aud_mma_event_callback, (Media_iMelody_Param **)&imy_param_p))!=NULL)
            {
               player_p->handle=(kal_int32)handle;
               imy_param_p->imyfile=(const kal_uint8*)req_p->data;
               imy_param_p->filesize=(kal_int32)req_p->size;
               imy_param_p->repeats=(kal_int16)req_p->repeats;
               imy_param_p->default_instrument=IMY_DEFAULT_INSTRUMENT;
               player_p->used=KAL_TRUE;
            }
         }
         break;
      }  
      /* single player, can't play with other players simultaneously */
#ifdef MMA_AMR_SUPPORT
      case MMA_TYPE_AMR:
      {
         Media_AMR_Param *amr_param_p;
         player_p=&(aud_context_p->player[MMA_PLAYER_AMR]);

         if(!player_p->used)
         {
            aud_mma_close_all_multi_player();
            player_p->handler_func=req_p->handler;
            if((handle=JAmr_Open(aud_mma_event_callback, (Media_AMR_Param **)&amr_param_p))!=NULL)
            {
               player_p->handle=(kal_int32)handle;
               amr_param_p->amrfile=(const kal_uint8*)req_p->data;
               amr_param_p->filesize=(kal_int32)req_p->size;
               amr_param_p->repeats=(kal_int16)req_p->repeats;
               player_p->used=KAL_TRUE;
            }
         }
         break;
      }   
#endif
#ifdef DAF_DECODE
      case MMA_TYPE_DAF:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_DAF]);

         if(!player_p->used)
         {
            aud_mma_close_all_multi_player();
            player_p->handler_func=req_p->handler;
            aud_context_p->current_file_handle=MMA_PLAYER_DAF;
            handle=(Media_Handle*)MMA_PLAYER_DAF;
            player_p->handle=(kal_int32)aud_context_p->current_file_handle;
            Media_SetBuffer((kal_uint16*)req_p->data, (kal_uint32)((req_p->size>>1)+1));
            Media_WriteDataDone(req_p->size>>1);
            Media_DataFinished();
            player_p->used=KAL_TRUE;
         }
         break;
      }   
#endif
#ifdef AAC_DECODE
      case MMA_TYPE_AAC:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AAC]);

         if(!player_p->used)
         {
            aud_mma_close_all_multi_player();
            player_p->handler_func=req_p->handler;
            aud_context_p->current_file_handle=MMA_PLAYER_AAC;
            handle=(Media_Handle*)MMA_PLAYER_AAC;
            player_p->handle=(kal_int32)aud_context_p->current_file_handle;
            Media_SetBuffer((kal_uint16*)req_p->data, (kal_uint32)((req_p->size>>1)+1));
            Media_WriteDataDone(req_p->size>>1);
            Media_DataFinished();
            player_p->used=KAL_TRUE;
         }
         break;
      }   
#endif
      case MMA_TYPE_AU:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AU]);

         if(!player_p->used)
         {
            aud_mma_close_all_multi_player();
            player_p->handler_func=req_p->handler;
            aud_context_p->current_file_handle=MMA_PLAYER_AU;
            handle=(Media_Handle*)MMA_PLAYER_AU;
            player_p->handle=(kal_int32)aud_context_p->current_file_handle;
            Media_SetBuffer((kal_uint16*)req_p->data, (kal_uint32)((req_p->size>>1)+1));
            Media_WriteDataDone(req_p->size>>1);
            Media_DataFinished();
            player_p->used=KAL_TRUE;
         }
         break;
      }   
   }
   mma_set_handle((kal_int32)handle);
   MMA_SET_EVENT(MMA_EVT_OPEN);
}

/*************************************************************************
* FUNCTION
*	aud_mma_close_req_hdlr
*
* DESCRIPTION
*	This function is to close media handle. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_close_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_close_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;
   med_result_enum result;
   aud_player_struct *player_p;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_CLOSE);
      return;
   }

   req_p = (media_mma_close_req_struct*)ilm_ptr->local_para_ptr;
   result=MED_RES_INVALID_HANDLE;

   ASSERT(req_p->handle!=NULL);
   switch(req_p->media_type)
   {
      case MMA_TYPE_MIDI:
      {
         kal_uint8 i;
         for(i=MMA_PLAYER_SMF_1;i<=MMA_PLAYER_SMF_4;i++)
         {
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==req_p->handle) 
            {
               status=JSmf_Close((Media_Handle*)req_p->handle);
               player_p->used=KAL_FALSE;
               player_p->handle = -1;
               result=(med_result_enum)aud_get_res(status);
               break;
            }
         }
         break;
      }   
      case MMA_TYPE_TONE:
      {
         kal_uint8 i;
         for(i=MMA_PLAYER_TONE_S1;i<=MMA_PLAYER_TONE_S4;i++)
         {
            player_p=&(aud_context_p->player[i]);
            if(player_p->used && player_p->handle==req_p->handle)
            {
               status=JTone_Close((Media_Handle*)req_p->handle);
               player_p->handle = -1;
               player_p->used=KAL_FALSE;
               result=(med_result_enum)aud_get_res(status);
               break;
            }
         }
         break;
      }
      case MMA_TYPE_WAV:
      {
         kal_uint8 i;
         for(i=MMA_PLAYER_WAV_1;i<=MMA_PLAYER_WAV_3;i++)
         {
            player_p=&(aud_context_p->player[i]);
            if(player_p->used && player_p->handle==req_p->handle)
            {
               status=JWav_Close((Media_Handle*)req_p->handle);
               player_p->used=KAL_FALSE;
               player_p->handle = -1;
               med_free_ext_mem((void**)&(player_p->buf_p));
               result=(med_result_enum)aud_get_res(status);
               break;
            }
         }
         break;
      }
      case MMA_TYPE_IMY:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_IMY]);
         if(player_p->used && 
            player_p->handle==req_p->handle)
         {
            status=JImy_Close((Media_Handle*)req_p->handle);
            player_p->used=KAL_FALSE;
            player_p->handle = -1;
            result=(med_result_enum)aud_get_res(status);
         }
         break;
      }   
#ifdef MMA_AMR_SUPPORT
      case MMA_TYPE_AMR:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AMR]);
         if(player_p->used && 
            player_p->handle==req_p->handle)
         {
            status=JAmr_Close((Media_Handle*)req_p->handle);
            player_p->used=KAL_FALSE;
            player_p->handle = NULL;
            result=(med_result_enum)aud_get_res(status);
         }
         break;
      }   
#endif
#ifdef DAF_DECODE
      case MMA_TYPE_DAF:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_DAF]);
         if(player_p->used && 
            player_p->handle==req_p->handle)
         {
            player_p->used=KAL_FALSE;
            player_p->handle = -1;
            result=MED_RES_OK;
         }
         break;
      }   
#endif
#ifdef AAC_DECODE
      case MMA_TYPE_AAC:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AAC]);
         if(player_p->used && 
            player_p->handle==req_p->handle)
         {
            player_p->used=KAL_FALSE;
            player_p->handle = -1;
            result=MED_RES_OK;
         }
         break;
      }   
#endif
      case MMA_TYPE_AU:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AU]);
         if(player_p->used && 
            player_p->handle==req_p->handle)
         {
            player_p->used=KAL_FALSE;
            player_p->handle = -1;
            result=MED_RES_OK;
         }
         break;
      }   
      default:
         ASSERT(0);
         result=MED_RES_ERROR;
         break;
   }
   mma_set_status(result);
   MMA_SET_EVENT(MMA_EVT_CLOSE);
}

/*************************************************************************
* FUNCTION
*	aud_mma_play_req_hdlr
*
* DESCRIPTION
*	This function is to play the media. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_play_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_play_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;
   kal_int32 handle;
   med_result_enum result;
   aud_player_struct *player_p;

   req_p = (media_mma_play_req_struct*)ilm_ptr->local_para_ptr;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_PLAY);
      return;
   }

   if(aud_context_p->speech_on)
   {
      mma_set_status(MED_RES_BUSY);
      MMA_SET_EVENT(MMA_EVT_PLAY);
      return;
   }
   
   if(aud_context_p->audio_mode==AUD_MODE_HEADSET)
     /* set the output device to headset speaker in HEADSET mode */
     aud_melody_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER2));
   else
     aud_melody_set_output_device(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER));
   
   result=MED_RES_INVALID_HANDLE;
   handle = req_p->handle;
   switch(req_p->media_type)
   {
      case MMA_TYPE_MIDI:
      {
         kal_uint8 i;
         for(i=MMA_PLAYER_SMF_1;i<=MMA_PLAYER_SMF_4;i++)
         {
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==handle) 
            {
               status=JSmf_Play((Media_Handle*)handle);
               result=(med_result_enum)aud_get_res(status);
               break;
            }
         }   
         break;
      }   
      case MMA_TYPE_TONE:
	   {
			kal_uint8 i;
			for(i=MMA_PLAYER_TONE_S1;i<=MMA_PLAYER_TONE_S4;i++) 
			{
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==handle) 
            {
               status=JTone_Play((Media_Handle*)handle);
               result=(med_result_enum)aud_get_res(status);
               break;
            }  
         }   
         break;
	   }
      case MMA_TYPE_WAV:
		{
			kal_uint8 i;
			for(i=MMA_PLAYER_WAV_1;i<=MMA_PLAYER_WAV_3;i++)
			{
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==handle) 
            {
               status=JWav_Play((Media_Handle*)handle);
               result=(med_result_enum)aud_get_res(status);
               break;
            }
         }   
         break;
		}
      case MMA_TYPE_IMY:
         player_p=&(aud_context_p->player[MMA_PLAYER_IMY]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=JImy_Play((Media_Handle*)handle);
            result=(med_result_enum)aud_get_res(status);
         }
         break;
#ifdef MMA_AMR_SUPPORT
      case MMA_TYPE_AMR:
         player_p=&(aud_context_p->player[MMA_PLAYER_AMR]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=JAmr_Play((Media_Handle*)handle);
            result=(med_result_enum)aud_get_res(status);
         }
         break;
#endif
#ifdef DAF_DECODE
      case MMA_TYPE_DAF:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_DAF]);
         if (player_p->used && player_p->handle==handle) 
         {
            status = Media_Play((Media_Format)MED_TYPE_DAF,
                              aud_mma_event_callback_1,
                              MEDIA_VMP_AS_RINGTONE);
            result=(med_result_enum)aud_get_res(status);                  
         }
         break;
      }   
#endif
#ifdef AAC_DECODE
      case MMA_TYPE_AAC:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AAC]);
         if (player_p->used && player_p->handle==handle) 
         {
            status = Media_Play((Media_Format)MED_TYPE_AAC,
                              aud_mma_event_callback_1,
                              MEDIA_VMP_AS_RINGTONE);
            result=(med_result_enum)aud_get_res(status);                  
         }
         break;
      }   
#endif
      case MMA_TYPE_AU:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AU]);
         if (player_p->used && player_p->handle==handle) 
         {
            status = Media_Play((Media_Format)MED_TYPE_AU,
                              aud_mma_event_callback_1,
                              MEDIA_VMP_AS_RINGTONE);
            result=(med_result_enum)aud_get_res(status);                  
         }
         break;
      }   
      default:
         ASSERT(0);
         result=MED_RES_ERROR;
         break;
   }
   mma_set_status(result);
   MMA_SET_EVENT(MMA_EVT_PLAY);

}

/*************************************************************************
* FUNCTION
*	aud_mma_stop_req_hdlr
*
* DESCRIPTION
*	This function is to stop the media. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_stop_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_stop_req_struct *req_p;
   kal_int32 handle;
   Media_Status status=MEDIA_FAIL;
   med_result_enum result;
   aud_player_struct *player_p;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_STOP);
      return;
   }

   if(aud_context_p->speech_on)
   {
      mma_set_status(MED_RES_BUSY);
      MMA_SET_EVENT(MMA_EVT_STOP);
      return;
   }   

   req_p = (media_mma_stop_req_struct*)ilm_ptr->local_para_ptr;
   handle = req_p->handle;
   result=MED_RES_INVALID_HANDLE;

   switch(req_p->media_type)
   {
      case MMA_TYPE_MIDI:
      {
         kal_uint8 i;
         for(i=MMA_PLAYER_SMF_1;i<=MMA_PLAYER_SMF_4;i++)
         {
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==handle) 
            {
               status=JSmf_Stop((Media_Handle*)handle);
               result=(med_result_enum)aud_get_res(status);
               break;
            }
         }   
         break;
      }   
      case MMA_TYPE_TONE:
	   {
			kal_uint8 i;
			for(i=MMA_PLAYER_TONE_S1;i<=MMA_PLAYER_TONE_S4;i++) 
			{
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==handle) 
            {
               status=JTone_Stop((Media_Handle*)handle);
               result=(med_result_enum)aud_get_res(status);
               break;
            }  
         }   
         break;
	   }
      case MMA_TYPE_WAV:
		{
			kal_uint8 i;
			for(i=MMA_PLAYER_WAV_1;i<=MMA_PLAYER_WAV_3;i++)
			{
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==handle) 
            {
               status=JWav_Stop((Media_Handle*)handle);
               result=(med_result_enum)aud_get_res(status);
               break;
            }
         }   
         break;
		}
      case MMA_TYPE_IMY:
         player_p=&(aud_context_p->player[MMA_PLAYER_IMY]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=JImy_Stop((Media_Handle*)handle);
            aud_melody_stop_driver();
            result=(med_result_enum)aud_get_res(status);
         }
         break;
#ifdef MMA_AMR_SUPPORT
      case MMA_TYPE_AMR:
         player_p=&(aud_context_p->player[MMA_PLAYER_AMR]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=JAmr_Stop((Media_Handle*)handle);
            result=(med_result_enum)aud_get_res(status);
         }
         break;
#endif
#ifdef DAF_DECODE
      case MMA_TYPE_DAF:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_DAF]);
         if (player_p->used && player_p->handle==handle) 
         {
            Media_Stop();
            result=MED_RES_OK;
         }
         break;
      }   
#endif
#ifdef AAC_DECODE
      case MMA_TYPE_AAC:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AAC]);
         if (player_p->used && player_p->handle==handle) 
         {
            Media_Stop();
            result=MED_RES_OK;
         }
         break;
      }   
#endif
      case MMA_TYPE_AU:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AU]);
         if (player_p->used && player_p->handle==handle) 
         {
            Media_Stop();
            result=MED_RES_OK;
         }
         break;
      }   
      default:
         ASSERT(0);
         result=MED_RES_ERROR;
         break;
   }
   mma_set_status(result);
   MMA_SET_EVENT(MMA_EVT_STOP);

}

/*************************************************************************
* FUNCTION
*	aud_mma_pause_req_hdlr
*
* DESCRIPTION
*	This function is to pause the media. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_pause_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_pause_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;
   kal_int32 handle;
   med_result_enum result;
   aud_player_struct *player_p;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_PAUSE);
      return;
   }

   if(aud_context_p->speech_on)
   {
      mma_set_status(MED_RES_BUSY);
      MMA_SET_EVENT(MMA_EVT_PAUSE);
      return;
   }   

   req_p = (media_mma_pause_req_struct*)ilm_ptr->local_para_ptr;
   handle = req_p->handle;
   result=MED_RES_INVALID_HANDLE;

   switch(req_p->media_type)
   {
      case MMA_TYPE_MIDI:
      {
         kal_uint8 i;
         for(i=MMA_PLAYER_SMF_1;i<=MMA_PLAYER_SMF_4;i++)
         {
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==handle) 
            {
               status=JSmf_Pause((Media_Handle*)handle);
               result=(med_result_enum)aud_get_res(status);
               break;
            }
         }   
         break;
      }   
      case MMA_TYPE_TONE:
	   {
			kal_uint8 i;
			for(i=MMA_PLAYER_TONE_S1;i<=MMA_PLAYER_TONE_S4;i++) 
			{
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==handle) 
            {
               status=JTone_Pause((Media_Handle*)handle);
               result=(med_result_enum)aud_get_res(status);
               break;
            }  
         }   
         break;
	   }
      case MMA_TYPE_WAV:
		{
			kal_uint8 i;
			for(i=MMA_PLAYER_WAV_1;i<=MMA_PLAYER_WAV_3;i++)
			{
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==handle) 
            {
               status=JWav_Pause((Media_Handle*)handle);
               result=(med_result_enum)aud_get_res(status);
               break;
            }
         }   
         break;
		}
      case MMA_TYPE_IMY:
         player_p=&(aud_context_p->player[MMA_PLAYER_IMY]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=JImy_Pause((Media_Handle*)handle);
            aud_melody_stop_driver();
            result=(med_result_enum)aud_get_res(status);
         }
         break;
#ifdef MMA_AMR_SUPPORT
      case MMA_TYPE_AMR:
         player_p=&(aud_context_p->player[MMA_PLAYER_AMR]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=JAmr_Pause((Media_Handle*)handle);
            result=(med_result_enum)aud_get_res(status);
         }
         break;
#endif
#ifdef DAF_DECODE
      case MMA_TYPE_DAF:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_DAF]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=Media_Pause();
            result=(med_result_enum)aud_get_res(status);
         }
         break;
      }   
#endif
#ifdef AAC_DECODE
      case MMA_TYPE_AAC:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AAC]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=Media_Pause();
            result=(med_result_enum)aud_get_res(status);
         }
         break;
      }   
#endif
      case MMA_TYPE_AU:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AU]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=Media_Pause();
            result=(med_result_enum)aud_get_res(status);
         }
         break;
      }   
      default:
         ASSERT(0);
         result=MED_RES_ERROR;
         break;
   }
   mma_set_status(result);
   MMA_SET_EVENT(MMA_EVT_PAUSE);

}

/*************************************************************************
* FUNCTION
*	aud_mma_resume_req_hdlr
*
* DESCRIPTION
*	This function is to resume the media. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_resume_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_resume_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;
   kal_int32 handle;
   med_result_enum result;
   aud_player_struct *player_p;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_RESUME);
      return;
   }

   if(aud_context_p->speech_on)
   {
      mma_set_status(MED_RES_BUSY);
      MMA_SET_EVENT(MMA_EVT_RESUME);
      return;
   }   
   
   req_p = (media_mma_resume_req_struct*)ilm_ptr->local_para_ptr;
   handle = req_p->handle;
   result=MED_RES_INVALID_HANDLE;

   switch(req_p->media_type)
   {
      case MMA_TYPE_MIDI:
      {
         kal_uint8 i;
         for(i=MMA_PLAYER_SMF_1;i<=MMA_PLAYER_SMF_4;i++)
         {
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==handle) 
            {
               status=JSmf_Resume((Media_Handle*)handle);
               result=(med_result_enum)aud_get_res(status);
               break;
            }
         }
         break;
      }   
      case MMA_TYPE_TONE:
	   {
			kal_uint8 i;
			for(i=MMA_PLAYER_TONE_S1;i<=MMA_PLAYER_TONE_S4;i++) 
			{
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==handle) 
            {
               status=JTone_Resume((Media_Handle*)handle);
               result=(med_result_enum)aud_get_res(status);
               break;
            }  
         }   
         break;
	   }
      case MMA_TYPE_WAV:
		{
			kal_uint8 i;
			for(i=MMA_PLAYER_WAV_1;i<=MMA_PLAYER_WAV_3;i++)
			{
            player_p=&(aud_context_p->player[i]);
            if (player_p->used && player_p->handle==handle) 
            {
               status=JWav_Resume((Media_Handle*)handle);
               result=(med_result_enum)aud_get_res(status);
               break;
            }
         }   
         break;
		}
      case MMA_TYPE_IMY:
         player_p=&(aud_context_p->player[MMA_PLAYER_IMY]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=JImy_Resume((Media_Handle*)handle);
            result=(med_result_enum)aud_get_res(status);
         }
         break;
#ifdef MMA_AMR_SUPPORT
      case MMA_TYPE_AMR:
         player_p=&(aud_context_p->player[MMA_PLAYER_AMR]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=JAmr_Resume((Media_Handle*)handle);
            result=(med_result_enum)aud_get_res(status);
         }
         break;
#endif
#ifdef DAF_DECODE
      case MMA_TYPE_DAF:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_DAF]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=Media_Resume();
            result=(med_result_enum)aud_get_res(status);
         }
         break;
      }   
#endif
#ifdef AAC_DECODE
      case MMA_TYPE_AAC:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AAC]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=Media_Resume();
            result=(med_result_enum)aud_get_res(status);
         }
         break;
      }   
#endif
      case MMA_TYPE_AU:
      {
         player_p=&(aud_context_p->player[MMA_PLAYER_AU]);
         if (player_p->used && player_p->handle==handle) 
         {
            status=Media_Resume();
            result=(med_result_enum)aud_get_res(status);
         }
         break;
      }   
      default:
         ASSERT(0);
         result=MED_RES_ERROR;
         break;
   }
   mma_set_status(result);
   MMA_SET_EVENT(MMA_EVT_RESUME);


}

/*************************************************************************
* FUNCTION
*	aud_mma_set_start_time_req_hdlr
*
* DESCRIPTION
*	This function is to set the start time of the media. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_set_start_time_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_set_start_time_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_SET_START_TIME);
      return;
   }

   req_p = (media_mma_set_start_time_req_struct*)ilm_ptr->local_para_ptr;

   switch(req_p->media_type)
   {
      case MMA_TYPE_MIDI:
         status=JSmf_SetStartTime((Media_Handle*)req_p->handle, req_p->start_time);
         break;
      case MMA_TYPE_TONE:
         status=JTone_SetStartTime((Media_Handle*)req_p->handle, req_p->start_time);
         break;
      case MMA_TYPE_WAV:
         status=JWav_SetStartTime((Media_Handle*)req_p->handle, req_p->start_time);
         break;
      case MMA_TYPE_IMY:
         status=JImy_SetStartTime((Media_Handle*)req_p->handle, req_p->start_time);
         break;
#ifdef MMA_AMR_SUPPORT
      case MMA_TYPE_AMR:
         status=JAmr_SetStartTime((Media_Handle*)req_p->handle, req_p->start_time);
         break;
#endif
   }
   mma_set_status((kal_int32)aud_get_res(status));
   MMA_SET_EVENT(MMA_EVT_SET_START_TIME);

}

/*************************************************************************
* FUNCTION
*	aud_mma_set_stop_time_req_hdlr
*
* DESCRIPTION
*	This function is to set the stop time of the media. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_set_stop_time_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_set_stop_time_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_SET_STOP_TIME);
      return;
   }

   req_p = (media_mma_set_stop_time_req_struct*)ilm_ptr->local_para_ptr;

   switch(req_p->media_type)
   {
      case MMA_TYPE_MIDI:
         status=JSmf_SetStopTime((Media_Handle*)req_p->handle, req_p->stop_time);
         break;
      case MMA_TYPE_TONE:
         status=JTone_SetStopTime((Media_Handle*)req_p->handle, req_p->stop_time);
         break;
      case MMA_TYPE_WAV:
         status=JWav_SetStopTime((Media_Handle*)req_p->handle, req_p->stop_time);
         break;
      case MMA_TYPE_IMY:
         status=JImy_SetStopTime((Media_Handle*)req_p->handle, req_p->stop_time);
         break;
#ifdef MMA_AMR_SUPPORT
      case MMA_TYPE_AMR:
         status=JAmr_SetStopTime((Media_Handle*)req_p->handle, req_p->stop_time);
         break;
#endif
   }
   mma_set_status((kal_int32)aud_get_res(status));
   MMA_SET_EVENT(MMA_EVT_SET_STOP_TIME);
}

/*************************************************************************
* FUNCTION
*	aud_mma_set_rate_req_hdlr
*
* DESCRIPTION
*	This function is to set the rate of the media. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_set_rate_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_set_rate_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_SET_RATE);
      return;
   }

   req_p = (media_mma_set_rate_req_struct*)ilm_ptr->local_para_ptr;

   switch(req_p->media_type)
   {
      case MMA_TYPE_MIDI:
         status=JMidi_SetRate((Media_Handle*)req_p->handle, req_p->rate);
         break;
      case MMA_TYPE_TONE:
      case MMA_TYPE_WAV:
         break;

   }
   mma_set_status((kal_int32)aud_get_res(status));
   MMA_SET_EVENT(MMA_EVT_SET_RATE);
}

/*************************************************************************
* FUNCTION
*	aud_mma_set_tempo_req_hdlr
*
* DESCRIPTION
*	This function is to set the tempo of the media. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_set_tempo_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_set_tempo_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_SET_TEMPO);
      return;
   }

   req_p = (media_mma_set_tempo_req_struct*)ilm_ptr->local_para_ptr;

   status=JMidi_SetTempo((Media_Handle*)req_p->handle, req_p->tempo);
   mma_set_status((kal_int32)aud_get_res(status));
   MMA_SET_EVENT(MMA_EVT_SET_TEMPO);
}

/*************************************************************************
* FUNCTION
*	aud_mma_set_pitch_req_hdlr
*
* DESCRIPTION
*	This function is to set the pitch of the media. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_set_pitch_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_set_pitch_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_SET_PITCH);
      return;
   }

   req_p = (media_mma_set_pitch_req_struct*)ilm_ptr->local_para_ptr;

   switch(req_p->media_type)
   {
      case MMA_TYPE_MIDI:
         status=JMidi_SetPitchTranspose((Media_Handle*)req_p->handle, req_p->pitch);
         break;
      case MMA_TYPE_TONE:
         status=JTone_SetPitchShift((Media_Handle*)req_p->handle, (kal_int8)req_p->pitch);
         break;
      case MMA_TYPE_WAV:
         break;

   }
   mma_set_status((kal_int32)aud_get_res(status));
   MMA_SET_EVENT(MMA_EVT_SET_PITCH);

}

/*************************************************************************
* FUNCTION
*	aud_mma_set_volume_req_hdlr
*
* DESCRIPTION
*	This function is to set the volume of the media. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_set_volume_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_set_volume_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_SET_VOLUME);
      return;
   }

   req_p = (media_mma_set_volume_req_struct*)ilm_ptr->local_para_ptr;

   switch(req_p->media_type)
   {
      case MMA_TYPE_MIDI:
         status=JMidi_SetLevel((Media_Handle*)req_p->handle, req_p->volume);
         break;
      case MMA_TYPE_TONE:
         status=JTone_SetLevel((Media_Handle*)req_p->handle, req_p->volume);
         break;
      case MMA_TYPE_WAV:
         status=JWav_SetLevel((Media_Handle*)req_p->handle, req_p->volume);
         break;
      case MMA_TYPE_IMY:
         status=JImy_SetLevel((Media_Handle*)req_p->handle, req_p->volume);
         break;
#ifdef MMA_AMR_SUPPORT
      case MMA_TYPE_AMR:
         status=JAmr_SetLevel((Media_Handle*)req_p->handle, req_p->volume);
         break;
#endif
   }
   mma_set_status((kal_int32)aud_get_res(status));
   MMA_SET_EVENT(MMA_EVT_SET_VOLUME);

}

/*************************************************************************
* FUNCTION
*	aud_mma_send_long_msg_req_hdlr
*
* DESCRIPTION
*	This function is to send long msg of the media. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_send_long_msg_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_send_long_msg_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;


   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_SEND_LONG_MSG);
      return;
   }

   if(aud_context_p->speech_on)
   {
      mma_set_status(MED_RES_BUSY);
      MMA_SET_EVENT(MMA_EVT_SEND_LONG_MSG);
      return;
   }   

   req_p = (media_mma_send_long_msg_req_struct*)ilm_ptr->local_para_ptr;

   status=JMidi_SendLongMsg((Media_Handle*)req_p->handle, req_p->buf, req_p->len);
   mma_set_status((kal_int32)aud_get_res(status));
   MMA_SET_EVENT(MMA_EVT_SEND_LONG_MSG);

}

/*************************************************************************
* FUNCTION
*	aud_mma_send_short_msg_req_hdlr
*
* DESCRIPTION
*	This function is to send short msg of the media. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_send_short_msg_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_send_short_msg_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_SEND_SHORT_MSG);
      return;
   }

   if(aud_context_p->speech_on)
   {
      mma_set_status(MED_RES_BUSY);
      MMA_SET_EVENT(MMA_EVT_SEND_SHORT_MSG);
      return;
   }   
   
   req_p = (media_mma_send_short_msg_req_struct*)ilm_ptr->local_para_ptr;

   status=JMidi_SendShortMsg((Media_Handle*)req_p->handle, req_p->type, req_p->data1, req_p->data2);
   mma_set_status((kal_int32)aud_get_res(status));
   MMA_SET_EVENT(MMA_EVT_SEND_SHORT_MSG);

}

/*************************************************************************
* FUNCTION
*	aud_mma_open_device_req_hdlr
*
* DESCRIPTION
*	This function is to open device. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_open_device_req_hdlr(ilm_struct *ilm_ptr)
{
   Media_Handle *handle;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_OPEN_DEVICE);
      return;
   }

   if(aud_context_p->speech_on)
   {
      mma_set_status(MED_RES_BUSY);
      MMA_SET_EVENT(MMA_EVT_OPEN_DEVICE);
      return;
   }   

   handle=JMidi_OpenDevice();
   mma_set_handle((kal_int32)handle);
   aud_context_p->midi_device_handle = (kal_int32)handle;
   aud_context_p->midi_device_opening = KAL_TRUE;
   MMA_SET_EVENT(MMA_EVT_OPEN_DEVICE);

}

/*************************************************************************
* FUNCTION
*	aud_mma_close_device_req_hdlr
*
* DESCRIPTION
*	This function is to close device. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_close_device_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_close_device_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_CLOSE_DEVICE);
      return;
   }

   req_p = (media_mma_close_device_req_struct*)ilm_ptr->local_para_ptr;

   status=JMidi_CloseDevice((Media_Handle*)req_p->handle);
   mma_set_status((kal_int32)aud_get_res(status));
   aud_context_p->midi_device_handle = -1;
   aud_context_p->midi_device_opening = KAL_FALSE;
   MMA_SET_EVENT(MMA_EVT_CLOSE_DEVICE);

}

/*************************************************************************
* FUNCTION
*	aud_mma_set_channel_volume_req_hdlr
*
* DESCRIPTION
*	This function is to set channel volume. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_set_channel_volume_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_set_channel_volume_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_SET_CHANNEL_VOLUME);
      return;
   }

   req_p = (media_mma_set_channel_volume_req_struct*)ilm_ptr->local_para_ptr;

   status=JMidi_SetChannelVolume((Media_Handle*)req_p->handle, req_p->channel, req_p->level);
   mma_set_status((kal_int32)aud_get_res(status));
   MMA_SET_EVENT(MMA_EVT_SET_CHANNEL_VOLUME);

}

/*************************************************************************
* FUNCTION
*	aud_mma_set_program_req_hdlr
*
* DESCRIPTION
*	This function is to set program. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_set_program_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_set_program_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_SET_PROGRAM);
      return;
   }

   req_p = (media_mma_set_program_req_struct*)ilm_ptr->local_para_ptr;

   status=JMidi_SetProgram((Media_Handle*)req_p->handle, req_p->channel, req_p->bank, req_p->program);
   mma_set_status((kal_int32)aud_get_res(status));
   MMA_SET_EVENT(MMA_EVT_SET_PROGRAM);

}

/*************************************************************************
* FUNCTION
*	aud_mma_set_data_req_hdlr
*
* DESCRIPTION
*	This function is to set data. 
*
* PARAMETERS
*	ilm_struct *ilm_ptr
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void aud_mma_set_data_req_hdlr(ilm_struct *ilm_ptr)
{
   media_mma_set_data_req_struct *req_p;
   Media_Status status=MEDIA_FAIL;

   if(aud_context_p->access_mod!=ilm_ptr->src_mod_id)
   {
      mma_set_status(MED_RES_BLOCKED);
      MMA_SET_EVENT(MMA_EVT_SET_DATA);
      return;
   }

   req_p = (media_mma_set_data_req_struct*)ilm_ptr->local_para_ptr;

   ASSERT(req_p->handle!=NULL);
   switch(req_p->media_type)
   {
      case MMA_TYPE_MIDI:
      {
         break;
      }   
      case MMA_TYPE_TONE:
      {
         kal_uint8 i;
         aud_player_struct *player_p;
         for(i=MMA_PLAYER_TONE_S1;i<=MMA_PLAYER_TONE_S4;i++)
         {
            player_p=&(aud_context_p->player[i]);
            if(player_p->handle==req_p->handle)
            {
               Media_ToneSeq_Param* toneseq_param_p;   
               
               toneseq_param_p=(Media_ToneSeq_Param*)player_p->param_p;
               toneseq_param_p->toneSeq=req_p->data;
               toneseq_param_p->toneSeqLen=req_p->size;
               toneseq_param_p->repeats=req_p->repeats;
               
               break;
            }   
         }   
         break;
      }   
      case MMA_TYPE_WAV:
      {
         break;
      }
   }

   mma_set_status((kal_int32)aud_get_res(status));
   MMA_SET_EVENT(MMA_EVT_SET_DATA);

}

void aud_mma_free_midi_context(Media_Handle *handle)
{

   kal_uint8 i = 0;
   aud_player_struct *player_p = NULL;
   
   for(i=MMA_PLAYER_SMF_1;i<=MMA_PLAYER_SMF_4;i++)
   {
      if(player_p->used && player_p->handle==(kal_int32)handle)
      {
         player_p->used=KAL_FALSE;
         player_p->handle=-1;
      }
   }   
}

 
void aud_mma_free_tone_context(Media_Handle *handle) 
{
   kal_uint8 i;
   aud_player_struct *player_p;

   for(i=MMA_PLAYER_TONE_S1;i<=MMA_PLAYER_TONE_S4;i++)
   {
		player_p=&(aud_context_p->player[i]);
      if(player_p->used && player_p->handle==(kal_int32)handle)
      {
         player_p->used=KAL_FALSE;
         player_p->handle=-1;
      }
   }
}

kal_bool aud_mma_opentone_with_handle(kal_int32 handle, kal_int32 handler_func)
{
	kal_uint8 i;
   aud_player_struct *player_p;

	for(i=MMA_PLAYER_TONE_S1;i<=MMA_PLAYER_TONE_S4;i++)
	{
		player_p=&(aud_context_p->player[i]);
		if(!player_p->used)
		{
			player_p->handler_func = handler_func;
			player_p->handle = handle;
			player_p->used = KAL_TRUE;
			return KAL_TRUE;
		}   
	}   
	return KAL_FALSE;
}

kal_bool aud_mma_openwav_with_handle(kal_int32 handle, kal_int32 handler_func)
{
	kal_uint8 i;
	kal_uint32 buf_len;
   aud_player_struct *player_p;

	for(i=MMA_PLAYER_WAV_1;i<=MMA_PLAYER_XWAV;i++)
	{
		player_p=&(aud_context_p->player[i]);
		if(!player_p->used)
		{
			player_p->handler_func = handler_func;
			player_p->handle = handle;
			player_p->used = KAL_TRUE;

			if(JWav_Load((Media_Handle *)handle, &buf_len)!=MEDIA_SUCCESS)
			{
				return KAL_FALSE;
			}
			if((player_p->buf_p=(kal_int16*)med_alloc_ext_mem(buf_len*sizeof(kal_uint16)))!=NULL)
			{
			   JWav_SetBuffer((Media_Handle *)handle, player_p->buf_p, buf_len);
			}

			return KAL_TRUE;
		}   
	}   
	return KAL_FALSE;
}

kal_bool aud_mma_openmidi_with_handle(kal_int32 handle, kal_int32 handler_func)
{
	kal_uint8 i;
   aud_player_struct *player_p;

	for(i=MMA_PLAYER_SMF_1;i<=MMA_PLAYER_SMF_4;i++)
	{
		player_p=&(aud_context_p->player[i]);
   	if(!player_p->used)
   	{
   		player_p->handler_func = handler_func;
   		player_p->handle = handle;
   		player_p->used = KAL_TRUE;
   		return KAL_TRUE;
   	}   
	}
	return KAL_FALSE;
}

kal_bool aud_mma_openimy_with_handle(kal_int32 handle, kal_int32 handler_func)
{
   aud_player_struct *player_p;
   
	player_p=&(aud_context_p->player[MMA_PLAYER_IMY]);
	if(!player_p->used)
	{
		player_p->handler_func = handler_func;
		player_p->handle = handle;
		player_p->used = KAL_TRUE;
		return KAL_TRUE;
	}   
	return KAL_FALSE;
}

kal_bool aud_mma_openamr_with_handle(kal_int32 handle, kal_int32 handler_func)
{
   aud_player_struct *player_p;
   
	player_p=&(aud_context_p->player[MMA_PLAYER_AMR]);
	if(!player_p->used)
	{
		player_p->handler_func = handler_func;
		player_p->handle = handle;
		player_p->used = KAL_TRUE;
		return KAL_TRUE;
	}   
	return KAL_FALSE;
}

kal_bool aud_mma_opendaf_with_handle(kal_int32 handle, kal_int32 handler_func, kal_uint8 * data, kal_int32 size)
{
	aud_player_struct *player_p;

	player_p=&(aud_context_p->player[MMA_PLAYER_DAF]);
	if(!player_p->used)
	{
		player_p->handler_func = handler_func;
		player_p->handle = handle;
		player_p->used = KAL_TRUE;

		Media_SetBuffer((kal_uint16*)data, (kal_uint32)((size>>1)+1));
		Media_WriteDataDone(size>>1);
		Media_DataFinished();

		return KAL_TRUE;
	}   

	return KAL_FALSE;
}

kal_bool aud_mma_openaac_with_handle(kal_int32 handle, kal_int32 handler_func, kal_uint8 * data, kal_int32 size)
{
	aud_player_struct *player_p;

	player_p=&(aud_context_p->player[MMA_PLAYER_AAC]);
	if(!player_p->used)
	{
		player_p->handler_func = handler_func;
		player_p->handle = handle;
		player_p->used = KAL_TRUE;

		Media_SetBuffer((kal_uint16*)data, (kal_uint32)((size>>1)+1));
		Media_WriteDataDone(size>>1);
		Media_DataFinished();

		return KAL_TRUE;
	}   

	return KAL_FALSE;
}

kal_bool aud_mma_openau_with_handle(kal_int32 handle, kal_int32 handler_func, kal_uint8 * data, kal_int32 size)
{
	aud_player_struct *player_p;

	player_p=&(aud_context_p->player[MMA_PLAYER_AU]);
	if(!player_p->used)
	{
		player_p->handler_func = handler_func;
		player_p->handle = handle;
		player_p->used = KAL_TRUE;

		Media_SetBuffer((kal_uint16*)data, (kal_uint32)((size>>1)+1));
		Media_WriteDataDone(size>>1);
		Media_DataFinished();

		return KAL_TRUE;
	}   

	return KAL_FALSE;
}

#endif /* __MED_MMA_MOD__ */

#endif /* MED_NOT_PRESENT */



