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
 * SoundRecorderMsgHandler.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is for sound recorder.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"
#ifdef __MMI_SOUND_RECORDER__

#define __NEWSIMULATOR 
#include "stdc.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "debuginitdef.h"
#include "wgui.h"
#include "globalstruct.h"
#include "mmi_data_types.h"
#include "wgui_categories.h"
#include "historygprot.h"
#include "idleappprot.h"
#include "custdatares.h"
#include "timerevents.h"
#include "protocolevents.h"
#include "eventsgprot.h"
#include "datetimetype.h"
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"
#include "custmenures.h"
#include "miscfunctions.h"
#include "queuegprot.h"
#include "taskinit.h"
#include "profilegprots.h"
#include "settingprofile.h"
#include "gpioinc.h"

#ifdef MMI_ON_HARDWARE_P
#include "mmi_msg_struct.h"
#include "l4dr.h"
#include "custom_util.h"
#include "aud_defs.h"
#include "med_struct.h"
#endif

#include "mdi_datatype.h"
#include "mdi_audio.h"

#include "filemanagerdef.h"

#include "soundrecorderdef.h"
#include "soundrecordertype.h"
#include "soundrecorderprot.h"

#ifdef __DRM_SUPPORT__
#include "fs_errcode.h"
#include "drm_gprot.h"
#define DRM_NOT_PERMIT_TO_PLAY	9999
#define DRM_NOT_PERMIT_TO_APPEND	10000
#endif
#include "kal_release.h"

#include "mmi_trace.h" //chenhe,20070615
#undef __NEWSIMULATOR 

#include "vdoplystrenum.h"
extern void ClearKeyEvents(void);
extern SoundRecorderStruct SoundRecorder;


BOOL Snd_getRecAmrRunState(VOID) //chenhe
{
	if( SoundRecorder.state==STATE_RECORD ||SoundRecorder.state==STATE_PLAY )
		return TRUE;
	else
		return FALSE;
}
UI_string_ID_type mmi_sndrec_get_res_string(U8 result)
{
#ifdef MMI_ON_HARDWARE_P
   switch(result)
   {
      case MDI_AUDIO_FAIL:
         return STR_ID_SNDREC_COMMAND_ERROR;
      case MDI_AUDIO_BUSY:
         return STR_ID_SNDREC_DEVICE_BUSY;
      case MDI_AUDIO_DISC_FULL:
//         return STR_ID_SNDREC_DISC_FULL;
         return FMGR_FS_DISK_FULL_TEXT; // use file manager disk full string id for unified error message notification
      case MDI_AUDIO_OPEN_FILE_FAIL:
      	  return STR_ID_SNDREC_OPEN_FILE_ERROR;
      case MDI_AUDIO_BAD_FORMAT:
      	  return STR_ID_SNDREC_BAD_FORMAT;
      case MDI_AUDIO_INVALID_FORMAT:
         return STR_ID_SNDREC_INVALID_FORMAT;
      case MDI_AUDIO_NO_DISC:
         return /*STR_ID_SNDREC_NO_CARD*/ STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD;
      case MDI_AUDIO_ERROR:
         return STR_ID_SNDREC_ERROR;
      default:
         return STR_ID_SNDREC_COMMAND_ERROR;
   }
#else
   return 0;
#endif
}
U32 global_var_uptime(BOOL bSet)
{
	static U32 up_time = 0;
	if(bSet)
		up_time++;

	return up_time;
}


U32 mmi_sndrec_get_duration_ticks()
{
	return SoundRecorder.constSeconds + SoundRecorder.recSeconds;
}

void mmi_sndrec_init_duration()
{
	SoundRecorder.recSeconds = 0;
	SoundRecorder.constSeconds = 0;
}

void mmi_sndrec_start_duration()
{
	SoundRecorder.constSeconds += SoundRecorder.recSeconds;
	SoundRecorder.recSeconds = 0;
	kal_get_time( (kal_uint32*)&SoundRecorder.recTime );
}

void mmi_sndrec_update_duration()
{
	kal_uint32 current_time;
	kal_get_time( &current_time );
	SoundRecorder.recSeconds= current_time - SoundRecorder.recTime;
}

void mmi_sndrec_record_event_hdlr(mdi_result result)
{
	TBM_ENTRY(Rec_stub_19);

mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_record_event_hdlr,result is %d,SoundRecorder.state is %d",result,SoundRecorder.state);
   SoundRecorder.cmdProcessing = FALSE;
   if(SoundRecorder.state==STATE_RECORD)
   {    
      if (result==MDI_AUDIO_TERMINATED)
      {
      mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_record_event_hdlr,result is %d,SoundRecorder.state is %d",result,SoundRecorder.state);

         SoundRecorder.state=STATE_IDLE;
	      mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_record_event_hdlr,!SoundRecorder.isFromCallExpress is %d",!SoundRecorder.isFromCallExpress);
	      mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_record_event_hdlr,!SoundRecorder.isAppend  is %d",!SoundRecorder.isAppend );
  	      mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_record_event_hdlr,mmi_sndrec_is_file_exist()   is %d",mmi_sndrec_is_file_exist()  );
	      mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_record_event_hdlr,SoundRecorder.isSlefStop  is %d",SoundRecorder.isSlefStop );

      //   if(!SoundRecorder.isFromCallExpress && !SoundRecorder.isAppend && mmi_sndrec_is_file_exist() && SoundRecorder.isSlefStop) //chenhe
               if(!SoundRecorder.isFromCallExpress && !SoundRecorder.isAppend && SoundRecorder.isSlefStop)

     	{
            mmi_sndrec_record_stopped_hdlr();
	      mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_record_event_hdlr,if 1111");

     	}
         else if( SoundRecorder.isAppend ) // this should never be in call so we can gobackhistory to file list screen
         {
            /*PANXU Modify Start For 6430 Ver: TBM780  on 2007-8-25 17:20 */
            //GoBackToHistory( SCR_FMGR_EXPLORER );
            GoBackHistory();  
            /*PANXU Modify End  For 6430 Ver: TBM780  on 2007-8-25 17:20 */
            SoundRecorder.isFromCallExpress = FALSE;
            SoundRecorder.isAppend = FALSE;
         }
         else if( SoundRecorder.isFromCallExpress || !mdi_audio_is_background_play_suspended() )
         {
            GoBackHistory();
            SoundRecorder.isFromCallExpress = FALSE;
         }
      }
      else
      {
         SoundRecorder.state=STATE_IDLE;   
         if (!SoundRecorder.isAppend && mmi_sndrec_is_file_exist())
            {
// Beginning--added by kecx for fixing bug 13268 on 20090817
            mmi_sndrec_error_hdlr((U8)result);
            COS_Sleep(1000);
// end--added by kecx for fixing bug 13268 on 20090817
            mmi_sndrec_record_stopped_hdlr();
            }
         else
            mmi_sndrec_error_hdlr((U8)result);
      }
   }   

   	TBM_EXIT(Rec_stub_19);

}

void mmi_sndrec_start_record(UI_string_type filename, U8 format)
{
   U8 file_format;
   mdi_result result;
	TBM_ENTRY(Rec_stub_10);
            mmi_trace(g_sw_SREC, "mmi_sndrec_start_record");

   if(!SoundRecorder.cmdProcessing)
   {
#ifdef MMI_ON_HARDWARE_P
#ifdef __DRM_SUPPORT__
      {
         FS_HANDLE handle= FS_ERROR_RESERVED;
	  BOOL is_permitted = FALSE;
	  if( (handle = DRM_open_file( (PU16)filename, FS_READ_ONLY, DRM_PERMISSION_NONE )) >= FS_NO_ERROR )
         {
         mmi_trace(g_sw_SREC, "mmi_sndrec_start_record,open success");

            if( DRM_get_rights( handle ) == NULL )
               is_permitted = TRUE;
            DRM_close_file( handle );
	  }
	  else if( handle < -1 ) // file open error. but not DRM related problem
         	is_permitted = TRUE;

         mmi_trace(g_sw_SREC, "mmi_sndrec_start_record,is_permitted is %d",is_permitted);

         if( !is_permitted )
         {
            mmi_sndrec_error_hdlr( DRM_NOT_PERMIT_TO_APPEND );
            SoundRecorder.isFromCallExpress = FALSE;
            mdi_audio_stop_all();
            return;
         }
      }
#endif
      switch(format)
      {
#ifndef MT6205B
         case FORMAT_AMR:
            file_format = MEDIA_AMR;
            break;
         case FORMAT_WAV:
            file_format = MEDIA_WAV_DVI_ADPCM;
            break;
#endif
#if defined( AMRWB_ENCODE ) && defined( AMRWB_DECODE )
         case FORMAT_AWB:
            file_format = MEDIA_AMR_WB;
            break;
		 case FORMAT_VM:						// chenxia move above for jasperII
            file_format = MEDIA_GSM_EFR;
            break;
#endif            

         default:
#ifndef MT6205B
            file_format = MEDIA_AMR;

#else
            file_format = MEDIA_GSM_EFR;

#endif
            break;
      }
#else
      file_format = 0;
#endif
      result=mdi_audio_start_record(filename, file_format, NULL, mmi_sndrec_record_event_hdlr);
   TRACE_EF(g_sw_SREC,"chenhe,mdi_audio_start_record,result is %d",result);
	//result=MDI_AUDIO_SUCCESS;//chenhe
      if(result==MDI_AUDIO_SUCCESS)
      {
         SoundRecorder.state=STATE_RECORD;
         SoundRecorder.isSlefStop=FALSE;
         mmi_sndrec_init_duration();
         mmi_sndrec_start_duration();
         EntryNewScreen( SCR_ID_SNDREC_RECORD, mmi_sndrec_exit_hdlr, NULL, NULL );
         if( SoundRecorder.isAppend )
            mmi_sndrec_entry_append();
         else
            mmi_sndrec_entry_record();
      }
      else
      {
         mmi_sndrec_error_hdlr(result);
         SoundRecorder.isFromCallExpress = FALSE;
      }
   }

   	TBM_EXIT(Rec_stub_10);

}

void mmi_sndrec_stop_record(void)
{
	TBM_ENTRY(Rec_stub_11);
	TurnOffBacklight(); //zhoumn
   TRACE_EF(g_sw_SREC,"chenhe,mmi_sndrec_stop_record,SoundRecorder.cmdProcessing is %d",SoundRecorder.cmdProcessing);
   if(!SoundRecorder.cmdProcessing)
   {    
      SoundRecorder.isSlefStop=TRUE;
      mdi_audio_stop_record();
   }

   	TBM_EXIT(Rec_stub_11);

}


void mmi_sndrec_play_event_hdlr(mdi_result result)
{
	TBM_ENTRY(Rec_stub_16);

   SoundRecorder.cmdProcessing = FALSE;
   if(SoundRecorder.state==STATE_PLAY)
   {
      if (result==MDI_AUDIO_END_OF_FILE)
      {
         SoundRecorder.state=STATE_IDLE;
         mmi_sndrec_error_hdlr((U8)result);   
      }
      else if (result==MDI_AUDIO_TERMINATED)
      {
         SoundRecorder.state=STATE_IDLE;
         mmi_sndrec_error_hdlr((U8)result);   
      }
      else
      {
         SoundRecorder.state=STATE_IDLE;
         mmi_sndrec_error_hdlr((U8)result);   
      }
   }   

   	TBM_EXIT(Rec_stub_16);

}


extern kal_uint8 aud_close_play_file(void);

void mmi_sndrec_start_play(UI_string_type filename)
{
	TBM_ENTRY(Rec_stub_3);

#ifdef MMI_ON_HARDWARE_P
   if(!SoundRecorder.cmdProcessing)
   {
      mdi_result result;   
#ifdef __DRM_SUPPORT__
      {
         FS_HANDLE handle= FS_ERROR_RESERVED;
         BOOL is_permitted = FALSE;
	
         if( (handle = DRM_open_file( (PU16)filename, FS_READ_ONLY, DRM_PERMISSION_NONE )) >= FS_NO_ERROR )
         {
            if( DRM_validate_permission( handle, DRM_PERMISSION_PLAY ) )
            {
               DRM_consume_rights( handle, DRM_PERMISSION_PLAY );
               is_permitted = TRUE;
	     }
	     DRM_close_file( handle );
	  }
         else if( handle < -1 ) // file open error, but not DRM related problem
         	is_permitted = TRUE;

         if( !is_permitted )
         {
            SoundRecorder.state=STATE_IDLE;
            mmi_sndrec_error_hdlr( DRM_NOT_PERMIT_TO_PLAY );
            mdi_audio_stop_all();
            return;
         }
      }
#endif
      result=mdi_audio_play_file(filename, DEVICE_AUDIO_PLAY_ONCE, NULL, mmi_sndrec_play_event_hdlr);
      if(result==MDI_AUDIO_SUCCESS)
      {
         SoundRecorder.state=STATE_PLAY;
         SoundRecorder.isSlefStop=FALSE;
		 //mdi_audio_set_volume( VOL_TYPE_MEDIA, GetRingVolumeLevel() );
         mmi_sndrec_init_duration();
         mmi_sndrec_start_duration();
         mmi_sndrec_entry_play();
		 SoundRecorder.volume=GetRingVolumeLevel();
      }   
      else
      {
         SoundRecorder.state=STATE_IDLE;
	 aud_close_play_file();
         mmi_sndrec_error_hdlr(result);   
      }
   }
#endif   

	TBM_EXIT(Rec_stub_3);

}

void mmi_sndrec_stop_play(void)
{
	TBM_ENTRY(Rec_stub_13);
	TurnOffBacklight(); //zhoumn


#ifdef MMI_ON_HARDWARE_P

   if(!SoundRecorder.cmdProcessing)
   {
      SoundRecorder.isSlefStop=TRUE;
      mdi_audio_stop_file();
      //GoBackHistory();
   }
#endif   

	TBM_EXIT(Rec_stub_13);

}

void mmi_sndrec_pause_event_hdlr(mdi_result result)
{
	TBM_ENTRY(Rec_stub_17);

   SoundRecorder.cmdProcessing = FALSE;
mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_pause_event_hdlr,result is %d,mmi_sndrec_is_file_exist()  is %d",result,mmi_sndrec_is_file_exist() );
   if (result==MDI_AUDIO_TERMINATED)
   {
      if((SoundRecorder.state==STATE_RECORD||SoundRecorder.state==STATE_RECORD_PAUSED))
      {   
         SoundRecorder.state=STATE_IDLE;
       //  if(!SoundRecorder.isFromCallExpress && !SoundRecorder.isAppend && mmi_sndrec_is_file_exist() && SoundRecorder.isSlefStop)//chenhe
            if(!SoundRecorder.isFromCallExpress && !SoundRecorder.isAppend && SoundRecorder.isSlefStop)
            	{
            	TRACE_EF(g_sw_SREC,"chenhe,mmi_sndrec_pause_event_hdlr,result is %d",result );

            mmi_sndrec_record_stopped_hdlr();
            	}
         else if( SoundRecorder.isFromCallExpress || !mdi_audio_is_background_play_suspended() )
         {
            GoBackHistory();
            SoundRecorder.isFromCallExpress = FALSE;
         }
      }
      else if(SoundRecorder.state==STATE_PLAY||SoundRecorder.state==STATE_PLAY_PAUSED)
      {
         SoundRecorder.state=STATE_IDLE;
         GoBackHistory();
      }   
   }
   else if (result==MDI_AUDIO_END_OF_FILE)
   {
      if((SoundRecorder.state==STATE_RECORD||SoundRecorder.state==STATE_RECORD_PAUSED))
      {
         SoundRecorder.state=STATE_IDLE;
         if(!SoundRecorder.isAppend && mmi_sndrec_is_file_exist())         
            mmi_sndrec_record_stopped_hdlr();

         else
            mmi_sndrec_error_hdlr((U8)result);
      }
      else if(SoundRecorder.state==STATE_PLAY||SoundRecorder.state==STATE_PLAY_PAUSED)
      {
         SoundRecorder.state=STATE_IDLE;
         mmi_sndrec_error_hdlr((U8)result);
      }
   }
   else
   {
      if(SoundRecorder.state!=STATE_IDLE)
      {
         BOOL isRecord = (SoundRecorder.state==STATE_RECORD||SoundRecorder.state==STATE_RECORD_PAUSED);
         if( isRecord && result == MDI_AUDIO_FAIL )
            result = MDI_AUDIO_DISC_FULL;
         if( isRecord && !SoundRecorder.isAppend && mmi_sndrec_is_file_exist() )
         {
            SoundRecorder.state=STATE_IDLE;

            mmi_sndrec_record_stopped_hdlr();
         }
         else
         {
            SoundRecorder.state=STATE_IDLE;
            mmi_sndrec_error_hdlr((U8)result);
         }
      }
   }

   	TBM_EXIT(Rec_stub_17);

}

void mmi_sndrec_pause(void)
{
	TBM_ENTRY(Rec_stub_12);

	  mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_pause,");
   if( SoundRecorder.state != STATE_IDLE && !SoundRecorder.cmdProcessing )
   {
      mdi_result result = mdi_audio_pause(5, mmi_sndrec_pause_event_hdlr);
      mmi_sndrec_update_duration();
	  mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_pause,result is %d",result);
      if(result==MDI_AUDIO_SUCCESS)
      {
         if (SoundRecorder.state==STATE_PLAY)
         {
            SoundRecorder.state=STATE_PLAY_PAUSED;
            mmi_sndrec_entry_play_paused();
         }
         else if(SoundRecorder.state==STATE_RECORD)
         {
            SoundRecorder.state=STATE_RECORD_PAUSED;
            mmi_sndrec_entry_record_paused();
         }  
      }   
      else
      {
         if( SoundRecorder.state==STATE_RECORD && result == MDI_AUDIO_FAIL )
            result = MDI_AUDIO_DISC_FULL;
         if (SoundRecorder.state==STATE_RECORD && !SoundRecorder.isAppend && mmi_sndrec_is_file_exist())
         {
            SoundRecorder.state=STATE_IDLE;

            mmi_sndrec_record_stopped_hdlr();
         }
         else
         {
            if( SoundRecorder.state == STATE_PLAY )
               result = MDI_AUDIO_END_OF_FILE;
            SoundRecorder.state=STATE_IDLE;
            mmi_sndrec_error_hdlr((U8)result);
         }
      }

      // clear key events to prevent external queue overflow
      ClearKeyEvents();
   }

   	TBM_EXIT(Rec_stub_12);

}

void mmi_sndrec_resume_event_hdlr(mdi_result result)
{
	TBM_ENTRY(Rec_stub_18);

   SoundRecorder.cmdProcessing = FALSE;
   mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_resume_event_hdlr,result is %d",result);

   if (result==MDI_AUDIO_TERMINATED)
   {
      if((SoundRecorder.state==STATE_RECORD||SoundRecorder.state==STATE_RECORD_PAUSED))
      {   
         SoundRecorder.state=STATE_IDLE;
         //if(!SoundRecorder.isFromCallExpress && !SoundRecorder.isAppend && mmi_sndrec_is_file_exist() && SoundRecorder.isSlefStop)//chenhe
          if(!SoundRecorder.isFromCallExpress && !SoundRecorder.isAppend && SoundRecorder.isSlefStop)

mmi_sndrec_record_stopped_hdlr();
          else if( SoundRecorder.isFromCallExpress || !mdi_audio_is_background_play_suspended() )
         {
            GoBackHistory();
            SoundRecorder.isFromCallExpress = FALSE;
         }
      }
      else if(SoundRecorder.state==STATE_PLAY||SoundRecorder.state==STATE_PLAY_PAUSED)
      {
         SoundRecorder.state=STATE_IDLE;
         GoBackHistory();
      }   
   }
   else if (result==MDI_AUDIO_END_OF_FILE)
   {
      if((SoundRecorder.state==STATE_RECORD||SoundRecorder.state==STATE_RECORD_PAUSED))
      {
         SoundRecorder.state=STATE_IDLE;
         if(!SoundRecorder.isAppend && mmi_sndrec_is_file_exist())         

            mmi_sndrec_record_stopped_hdlr();
         else
            mmi_sndrec_error_hdlr((U8)result);
      }
      else if(SoundRecorder.state==STATE_PLAY||SoundRecorder.state==STATE_PLAY_PAUSED)
      {
         SoundRecorder.state=STATE_IDLE;
         mmi_sndrec_error_hdlr((U8)result);
      }
   }
   else
   {
      if(SoundRecorder.state!=STATE_IDLE)
      {
         BOOL isRecord = (SoundRecorder.state==STATE_RECORD||SoundRecorder.state==STATE_RECORD_PAUSED);
         if( isRecord && result == MDI_AUDIO_FAIL )
            result = MDI_AUDIO_DISC_FULL;
         if( isRecord && !SoundRecorder.isAppend && mmi_sndrec_is_file_exist() )
         {
            SoundRecorder.state=STATE_IDLE;

            mmi_sndrec_record_stopped_hdlr();
         }
         else
         {
            SoundRecorder.state=STATE_IDLE;
            mmi_sndrec_error_hdlr((U8)result);
         }
      }   
   }

   	TBM_EXIT(Rec_stub_18);

}

void mmi_sndrec_resume(void)
{
	TBM_ENTRY(Rec_stub_14);

   mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_resume,SoundRecorder.state is %d,SoundRecorder.cmdProcessing is %d",SoundRecorder.state,SoundRecorder.cmdProcessing);
   if( SoundRecorder.state != STATE_IDLE && !SoundRecorder.cmdProcessing )
   {
      mdi_result result;
      result=mdi_audio_resume(5, mmi_sndrec_resume_event_hdlr);
     mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_resume,result is %d",result);

      if(result==MDI_AUDIO_SUCCESS)
      {
         if (SoundRecorder.state==STATE_PLAY_PAUSED)
         {
            SoundRecorder.state=STATE_PLAY;
            mmi_sndrec_start_duration();
            mmi_sndrec_entry_play();
         }
         else if(SoundRecorder.state==STATE_RECORD_PAUSED)
         {
            SoundRecorder.state=STATE_RECORD;
            mmi_sndrec_start_duration();
            if( SoundRecorder.isAppend )
               mmi_sndrec_entry_append();
            else
               mmi_sndrec_entry_record();
         }  
      }   
      else
      {
         if (SoundRecorder.state==STATE_RECORD_PAUSED && !SoundRecorder.isAppend && mmi_sndrec_is_file_exist())
         {
            SoundRecorder.state=STATE_IDLE;

            mmi_sndrec_record_stopped_hdlr();
         }
         else
         {
            SoundRecorder.state=STATE_IDLE;
            mmi_sndrec_error_hdlr((U8)result);
         } 
      }

      // clear key events to prevent external queue overflow
      ClearKeyEvents();
   }

   	TBM_EXIT(Rec_stub_14);

}

void mmi_sndrec_set_media_volume(U8 volume)
{
#ifdef MMI_ON_HARDWARE_P
   mdi_audio_set_volume(MDI_VOLUME_MEDIA, volume);
#endif   
}

void mmi_sndrec_set_speech_volume(U8 volume)
{
#ifdef MMI_ON_HARDWARE_P
   mdi_audio_set_volume(MDI_VOLUME_SPH, volume);
#endif   
}

#endif   /* __MMI_SOUND_RECORDER__ */


