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
 *   FMScheduleRecMsgHdlr.c
 *
 * Project:
 * --------
 *   Maui_sw
 *
 * Description:
 * ------------
 *  .............
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
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MMI_FMSRMSGHDLR_C
#define _MMI_FMSRMSGHDLR_C

#include "mmi_features.h" 

#ifdef __MMI_FM_RADIO_SCHEDULE_REC__

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "commonscreens.h"
#include "keybrd.h"

#include "aud_defs.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "mdi_include.h"
#include "fs_type.h"
#include "fs_errcode.h"
#if defined(__DRM_SUPPORT__)
#include "fs_errcode.h"
#include "drm_gprot.h"
#endif
//... Add More MMI header

#include "fmschedulerecdef.h"
#include "fmschedulerectype.h"
#include "fmschedulerecprot.h"
#undef __NEWSIMULATOR 

/*  Include: PS header file */
//... Add More PS header


/* 
** Typedef 
*/

/* 
** Define
*/

/* 
** Local Variable
*/

/* 
** Local Function
*/

/* 
** Global Variable
*/

/* 
** Global Function
*/


/*****************************************************************************
* FUNCTION
*  mmi_fmsr_mdi_callback
* DESCRIPTION
*   The callback function for mdi.
*
* PARAMETERS
*  result	IN	the result of a mdi command
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_mdi_callback( S32 result )
{
	if( g_fmsr_cntx.rec.state == FMSR_STATE_IDLE )
		return;


	/*Stop FM radio recording, but not stop playing yet*/
	if( (g_fmsr_cntx.rec.state == FMSR_STATE_RECORD || g_fmsr_cntx.rec.state == FMSR_STATE_PAUSE ) && 
	     result == MDI_AUDIO_STOP_FM_RECORD)
	{
		if(g_fmsr_cntx.rec.state == FMSR_STATE_PAUSE)
		{
			g_fmsr_cntx.rec.is_screen_paused = TRUE;
		}
		
		g_fmsr_cntx.rec.state = FMSR_STATE_READY ;
		return;
	}
	/*Stop FM radio recording, then stop FM radio*/
	else if(g_fmsr_cntx.rec.state == FMSR_STATE_RECORD || g_fmsr_cntx.rec.state == FMSR_STATE_PAUSE)
	{
		if(g_fmsr_cntx.rec.state == FMSR_STATE_PAUSE)
		{
			g_fmsr_cntx.rec.is_screen_paused = TRUE;
		}
		
		g_fmsr_cntx.rec.state = FMSR_STATE_READY;

		/*record not finish correctly, show the error*/
		if(result != MDI_AUDIO_TERMINATED)
		{
			mmi_fmsr_record_error_hdlr(result);
			return;
		}
	}

	/*Stop FM Playing*/
	g_fmsr_cntx.rec.state = FMSR_STATE_IDLE;
	mmi_fmsr_record_error_hdlr(result);
		
//#ifdef __MMI_SUBLCD__
//	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
//		ShowIdleAppSubLcd();
//#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_power_on
* DESCRIPTION
*   Power on the FM radio and playing
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_power_on( void )
{
	if(g_fmsr_cntx.rec.state != FMSR_STATE_IDLE)
	{
		/*Enter error screen*/
		return;
	}
	
	//if( !mdi_check_device_ownership() )
	if( mdi_check_device_ownership() )//jiashuo jessamine
	{
		/*Enter error screen*/
		return;
	}
	else
	{
		/*Stop all aduo and begin to record*/
		mdi_audio_stop_all();
		
		mdi_fmr_power_on_with_path( MDI_DEVICE_SPEAKER2, (mdi_callback) mmi_fmsr_mdi_callback );
		mdi_audio_set_volume( AUD_VOLUME_FMR, g_fmsr_cntx.rec.volume );
		mdi_fmr_set_freq( g_fmsr_cntx.rec.freqency );
		g_fmsr_cntx.rec.state = FMSR_STATE_READY;
	}

//#ifdef __MMI_SUBLCD__
//	if( g_idle_context.IsSubLcdOnIdleScreen == SUBLCD_ON_IDLE_NORMAL )
//		ShowIdleAppSubLcd();
//#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_power_off
* DESCRIPTION
*   Power off the FM radio
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_power_off( void )
{
	if(g_fmsr_cntx.rec.state == FMSR_STATE_IDLE)
	{
		return;
	}
	
	mdi_audio_stop_fmr();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_start_record
* DESCRIPTION
*   Start to record FM radio
*
* PARAMETERS
*  filename	IN	the record file name
*  format		IN	the record file format
*
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_fmsr_start_record(UI_string_type filename, U8 format)
{
	U8 file_format;
	mdi_result result;
   
#ifdef MMI_ON_HARDWARE_P
#ifdef __DRM_SUPPORT__
	{
		FS_HANDLE handle;
		BOOL is_permitted = FALSE;

		if( (handle = DRM_open_file( (PU16)filename, FS_READ_ONLY, DRM_PERMISSION_NONE )) >= FS_NO_ERROR )
		{
			if( DRM_get_rights( handle ) == NULL )
				is_permitted = TRUE;
			DRM_close_file( handle );
		}
		else if( handle < -1 ) /* file open error. but not DRM related problem*/
		{
			is_permitted = TRUE;
		}

		if( !is_permitted )
		{
			mmi_fmsr_record_error_hdlr(DRM_NOT_PERMIT_TO_APPEND);
			return FALSE;
		}
	}
#endif
	switch(format)
	{
		case FMSR_FORMAT_AMR:
			file_format = MEDIA_AMR;
			break;
#if 0            
		case FMSR_FORMAT_MP3:
			file_format = MEDIA_DAF;
			break;
#endif
		case FMSR_FORMAT_WAV:
			file_format = MEDIA_WAV_DVI_ADPCM;
			break;
		default:
			file_format = MEDIA_AMR;
			break;
		
	}
#else
	file_format = 0;
#endif

      result=mdi_fmr_start_record(filename, file_format, NULL, mmi_fmsr_mdi_callback);
      if(result==MDI_AUDIO_SUCCESS)
      {
      	g_fmsr_cntx.rec.state = FMSR_STATE_RECORD;
      	return TRUE;
      }
	mmi_fmsr_record_error_hdlr(result);
	return FALSE;

}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_stop_record
* DESCRIPTION
*  Stop record FM radio
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_stop_record(void)
{
	S32 result;

	if(g_fmsr_cntx.rec.state == FMSR_STATE_READY ||
		g_fmsr_cntx.rec.state == FMSR_STATE_IDLE)
	{
		return;
	}
	
	if(g_fmsr_cntx.rec.state == FMSR_STATE_RECORD)
	{
		mmi_fmsr_update_duration();
	}

	/* State will update in mmi_fmrdo_preempted() when call back*/
	if((result = mdi_audio_stop_fmr_record_only()) != MDI_AUDIO_SUCCESS)
	{
		/*No reason not stop, error Handling Here.*/
		mmi_fmsr_record_error_hdlr(result);
	}

}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_pause_record
* DESCRIPTION
*   Pause FM radio record
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_pause_record(void)
{
	mdi_result result;
	
	if( g_fmsr_cntx.rec.state == FMSR_STATE_RECORD )
	{
		result = mdi_fmr_pause_record(mmi_fmsr_mdi_callback);

		mmi_fmsr_update_duration();
		if(result==MDI_AUDIO_SUCCESS)
		{
			g_fmsr_cntx.rec.state = FMSR_STATE_PAUSE;
			mmi_fmsr_change_pause_screen();
		}   
		else	/*Can't pause, stop recording*/
		{
			g_fmsr_cntx.rec.state = FMSR_STATE_READY;
			mmi_fmsr_record_error_hdlr(result);

			g_fmsr_cntx.rec.is_show_msg = FALSE;
			mmi_fmsr_power_off();				
	      }

		// clear key events to prevent external queue overflow
		ClearKeyEvents();
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_resume_record
* DESCRIPTION
*   Resume FM radio record
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_resume_record(void)
{
	mdi_result result;


	if( g_fmsr_cntx.rec.state == FMSR_STATE_PAUSE )
	{

		result=mdi_frm_resume_record(mmi_fmsr_mdi_callback);
		if(result == MDI_AUDIO_SUCCESS)
		{
			g_fmsr_cntx.rec.state = FMSR_STATE_RECORD;
			mmi_fmsr_start_duration();
			mmi_fmsr_restore_recording_screen();
		}   
		else
		{
			g_fmsr_cntx.rec.state = FMSR_STATE_READY;
			mmi_fmsr_record_error_hdlr(result);

			g_fmsr_cntx.rec.is_show_msg = FALSE;
			mmi_fmsr_power_off();				
			
		}

		// clear key events to prevent external queue overflow
		ClearKeyEvents();
	}

}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_increase_volume
* DESCRIPTION
*   Increase the playing volume
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_increase_volume( void )
{
	if( g_fmsr_cntx.rec.volume < MAX_VOL_LEVEL - 1 )
	{
		mdi_audio_set_volume( AUD_VOLUME_FMR, ++g_fmsr_cntx.rec.volume );
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_decrease_volume
* DESCRIPTION
*   Decrease the playing volume
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_decrease_volume( void )
{
	if( g_fmsr_cntx.rec.volume > 0 )
	{
		mdi_audio_set_volume( AUD_VOLUME_FMR, --g_fmsr_cntx.rec.volume );
	}
}

#endif /* #ifdef __MMI_FM_RADIO_SCHEDULE_REC__ */
#endif /* #ifndef _MMI_FMSRMSGHDLR_C */
