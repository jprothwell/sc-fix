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
 *
 * Filename:
 * ---------
 *	AudioSrc.c
 *
 * Project:
 * --------
 *   MediaTeck GSM
 *
 * Description:
 * ------------
 
 *
 * Author:
 * -------
 * -------
 * 				
 *
 * HISTORY
 * -------
 * Created On 12 March
*******************************************************************************/
#ifndef _AUDIOINC_H
#define _AUDIOINC_H

#include "mmi_data_types.h"
#include "queuegprot.h"

typedef enum {
	MMI_ALERT_NONE,
	MMI_RING,
	MMI_VIBRATION_ONLY,
	MMI_VIBRATION_AND_RING,
	MMI_VIBRATION_THEN_RING,
	MMI_SILENT
} MMI_ALERT_TYPE;

typedef enum {
	MMI_INTELLIGENT_CALL_ALERT_OFF,
	MMI_INTELLIGENT_CALL_ALERT_ON
} MMI_INTELLIGENT_CALL_ALERT_TYPE;

#ifdef __MMI_CONNECT_NOTICE__
typedef enum {
	MMI_NOTICE_NONE,
	MMI_NOTICE_TONE_ONLY,
	MMI_NOTICE_VIB_ONLY,
	MMI_NOTICE_TONE_AND_VIB
} MMI_CONNECT_NOTICE_TYPE;
#endif

typedef enum{
	AUDIO_COMPOSER_PLAY_ID	=	1
}AUDIO_PLAY_ID;
#ifdef MMI_ON_WIN32
	#include "stdio.h"
	#include "windows.h"
	#include "mmsystem.h"		
	#include "windowsx.h"
	#include "stdafx.h"
	#include "resource.h"
	#define MAXRINGS    5
	void WindowHandle(HWND inPWHandle);
	HWND PWHandle;
	extern BOOL RepeatFactor;
	/* [CSD00612] Bearw */
#ifndef DEVELOPER_BUILD_FIRST_PASS
	typedef unsigned char   kal_uint8;
	typedef unsigned short int      kal_uint16;
	typedef unsigned int            kal_uint32;
#endif
#endif

#include "kal_non_specific_general_types.h"
#include "device.h"
#include "resource_audio.h"


void AudioPlayReqWithVolPath(U16 soundId, U8 style, U8 volume, U8 path);
void AudioPlayReq(U16 soundId, U8 style);
void AudioStopReq(U16 soundId);
void AudioPlayIMelodyString(U8 *string, U16 len, U8 style);
void AudioPlayIMelodyFile(U8 *file, U8 style);
void AudioPlayIMelodyFileWithId(U8 *file, U8 style, U16 nId);
void AudioStopIMelodyFile(U8 *file);
void AudioPlayToneWithCallBack(U16 soundId, U8 style, U8 tone_type );
void AudioPlayToneWithCallBackVolPath(U16 soundId, U8 style, U8 tone_type, U8 volume, U8 path );


void StopAudioMidiStream(void);
void StopAudioWavStream(void);
void StopAudioDviStream(void);
void StopAudioStream(void);
void PlayAudioMidiStream( U8* audioBuffer, U16 length, U8 style);
void PlayAudioWavStream(  U8* audioBuffer, U16 length, U8 style);
void PlayAudioStream(U8* audioBuffer, U16 length, U8 format, U8 style );
void PlayAudioDviStream(  U8* audioBuffer, U16 length, U8 style);

#endif	




