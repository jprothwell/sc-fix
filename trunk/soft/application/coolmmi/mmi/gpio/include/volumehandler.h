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
 * VolumeHandler.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Volume Key Handler
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

/**************************************************************

	FILENAME	: VolumeHandler.h

  	PURPOSE		: Volume Key Handler

 

 

	DATE		: July 22,03

**************************************************************/
#ifndef _VOLUME_HANDLER_H
#define _VOLUME_HANDLER_H
#include "frameworkstruct.h"
#include "settingprofile.h"

#ifdef MMI_ON_WIN32
//	#define PROFILES_TESTING_ON_WINDOWS
#endif

#define MAX_VOL_LEVEL_SCR_KEYS			19
#define KEY_VOLUME_TIMEOUT				2000
typedef enum
{
	VOLUMEHANDLER_RING_TONE,
	VOLUMEHANDLER_SPEECH_TONE,
	VOLUMEHANDLER_KEY_TONE
}VOLUMEHANDLER_TYPE;

void SetDefaultVolumeKeyHandlers(void);

void SetVolumeKeyHandlerFunctions(FuncPtr decreaseFunction,
							FuncPtr increaseFunction,
							FuncPtr minFunction,
							FuncPtr maxFunction,
							U16 caption, S32 *volume,
							U16 captionLSK, U16 captionRSK,
							FuncPtr callbackLSK, FuncPtr callbackRSK,
							U32 timerDuration, FuncPtr callbackExit, U8 updown,VOLUMEHANDLER_TYPE type);

void SetKeyPadVolMaxLongPress(void);
void SetKeyPadVolMinLongPress(void);
void SetKeyPadVolDown(void);
void SetKeyPadVolUp(void);
void EntryScrSetKeyPadVolumeLevel(void);
void ExitScrSetKeyPadVolumeLevel(void);
void StartKeyPadVolumeLevelTimer(void);
void StopKeyPadVolumeLevelTimer(void);
void ShowVerticalBarMin(void);
void ShowVerticalBarMax(void);
void ShowVerticalBarDown(void);
void ShowVerticalBarUp(void);

void SpeechVolumeFunction(void);
void SpeechVolumeExitFunction(void);
void RingVolumeExitFunction(void);
void RingToneVolumeFunction(void);

void KeyPadVolumeFunction(void);
void KeyPadVolumeExitFunction(void);


 
#if defined(__MMI_TOUCH_SCREEN__) //&& defined(__MMI_WALLPAPER_ON_BOTTOM__)

void CallShowVerticalBarMax(void);
void CallShowVerticalBarMin(void);
void ResetCallbackTimer(void);

#endif
 


#endif


