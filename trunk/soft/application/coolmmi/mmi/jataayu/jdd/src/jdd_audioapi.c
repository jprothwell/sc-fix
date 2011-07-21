/**
 * @file    jdd_audioapi.c
 * @ingroup JDD
 * @brief   Defines the device dependent Audio API for ELIPS platform.
 */

/***************************************************************************
* IMPORTANT NOTICE
*
* Please note that any and all title and/or intellectual property rights
* in and to this Software or any part of this (including without limitation
* any images, photographs, animations, video, audio, music, text and/or
* "applets," incorporated into the Software), herein mentioned to as
* "Software", the accompanying printed materials, and any copies of the
* Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu")
* or Jataayu's suppliers as the case may be. The Software is protected by
* copyright, including without limitation by applicable copyright laws,
* international treaty provisions, other intellectual property laws and
* applicable laws in the country in which the Software is being used.
* You shall not modify, adapt or translate the Software, without prior
* express written consent from Jataayu. You shall not reverse engineer,
* decompile, disassemble or otherwise alter the Software, except and
* only to the extent that such activity is expressly permitted by
* applicable law notwithstanding this limitation. Unauthorized reproduction
* or redistribution of this program or any portion of it may result in severe
* civil and criminal penalties and will be prosecuted to the maximum extent
* possible under the law. Jataayu reserves all rights not expressly granted.
*
* THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND
* AND ANY AND ALL REPRESENTATION
*
***************************************************************************/

/* CVS ID (Auto-updated during CVS commit):
 * @(#) $Id: jdd_audioapi.c,v 1.10.2.3 2006/08/16 05:27:57 sureshdevop_cellon Exp $
 */

//#define __MODULE_ID__       COMMON_APPLICATION

/***************************************************************************
 * System Include Files
 **************************************************************************/
#include "dm.h"
//#include "device.h"
#include "mmi_features.h"
#include "mdi_audio.h"
//#include "audioplayerprot.h"



/* ===========================================================================
 *  HEADER FILES
 * ===========================================================================
 */

#include "ddl.h"
#include "jcal.h"



//#define TEMP_FILE_NAME      WORK_DIR "jddaudio."



/* ===========================================================================
 *  TYPE DEFINITIONS
 * ===========================================================================
 */
/*
typedef struct MPlayerInfo
{
    MediaPlayer *mPlayer;
    char        asciiFileName[64];
} MPlayerInfo_t;
*/

/* ===========================================================================
 *  PRIVATE FUNCTIONS
 * ===========================================================================
 */




/* ===========================================================================
 *  PUBLIC FUNCTIONS
 * ===========================================================================
 */

JC_RETCODE jdd_AudioInitialize(JDD_AUDIO *pAudioHandle)
{
    JC_RETCODE rCode = JC_OK;
    
    if(NULL == pAudioHandle)
    {
       return JC_ERR_INVALID_PARAMETER;
	}
	else
	{
		rCode = JC_OK;
    }
	
    return rCode;
}
JC_RETCODE jdd_AudioDeInitialize(JDD_AUDIO audioHandle)
{
    if(NULL == audioHandle)
    {
    }    
    return JC_OK;
}

JC_RETCODE jdd_AudioPlay(JDD_AUDIO audioHandle, JC_AUDIO_INFO *pAudioInfo)
{
	JC_RETCODE rCode = JC_OK;
	
	if (audioHandle == NULL || pAudioInfo == NULL)
    {        
        return JC_ERR_INVALID_PARAMETER;
    }
    rCode = mdi_audio_play_file_with_vol_path(pAudioInfo->psAudioFileName,2,JC_NULL,Jmms_audply_play_callback_hdlr,7,MDI_DEVICE_SPEAKER,0);   

	//play style = DEVICE_AUDIO_PLAY_ONCE
	//handler = mmi_audply_play_callback_hdlr
	//volume LEVEL7
 
    return rCode;
}

JC_RETCODE jdd_AudioStop(JDD_AUDIO audioHandle)
{
	JC_RETCODE rCode = JC_OK;
    if (audioHandle)
    {
        rCode =  mdi_audio_stop_file();
    }    
    return rCode;
}

JC_RETCODE jdd_AudioPause(JDD_AUDIO audioHandle)
{
    JC_RETCODE rCode = JC_OK;   

    if (audioHandle)
	{
       // rCode = mdi_audio_pause(JC_NULL, mmi_audply_pause_callback_hdlr); //TODO  mdi_callback handler  check (mdi_h, mmi_audply_pause_callback_hdlr)
    }    
    return rCode;
}

JC_RETCODE jdd_AudioResume(JDD_AUDIO audioHandle)
{
    JC_RETCODE rCode = JC_OK;
    if (audioHandle)
    {
        //rCode = mdi_audio_resume(JC_NULL, mmi_audply_resume_callback_hdlr); //TODO mdi_h, mmi_audply_resume_callback_hdlr
    }
    return rCode;
}

void Jmms_audply_play_callback_hdlr()
{
/*
	if( g_audply.state != STATE_PLAY )
		return;

	// handle result
	g_audply.need_restore = FALSE;
	
	switch( result )
	{
	case MDI_AUDIO_END_OF_FILE:
		g_audply.need_replay = FALSE;
		mmi_audply_play_completed();
		break;
	case MDI_AUDIO_TERMINATED:
		g_audply.need_replay = TRUE;//chenhe modify the value to TRUE for paly resume
		mmi_audply_stopped();
		if( g_audply.restorable )
		{
			//if( mdi_audio_store_file( mdi_h ) == MDI_AUDIO_SUCCESS )//chenhe comment it temparary for audio play reusme
				g_audply.need_restore = TRUE;
		}
		break;
	default:
		g_audply.need_replay = FALSE;
		mmi_audply_stopped();
		DisplayPopup( (PU8)GetString( mmi_audply_err_str( (S32)result ) ), IMG_GLOBAL_WARNING, 0, 1000, (U8)ERROR_TONE );//modify by lin @20070830
		break;
		*/
}
/* ===========================================================================
 *  END OF FILE
 * ===========================================================================
 */
