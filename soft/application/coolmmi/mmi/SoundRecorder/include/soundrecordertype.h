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
 * SoundRecorderType.h
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
#ifndef _SOUND_RECORDER_TYPE_H
#define _SOUND_RECORDER_TYPE_H



#define SND_NOT_SUPPORT_WAV_ON_FLASH
typedef enum
{
   STATE_IDLE,
   STATE_PLAY,
   STATE_PLAY_PAUSED,
   STATE_RECORD,
   STATE_RECORD_PAUSED,
   NO_OF_STATE

}
SoundRecorderStateEnum;

typedef enum
{
   STORAGE_PHONE,
   STORAGE_CARD,
   /*+ zhouqin modify for dual t-flash card 20110503*/
    #if defined(DUAL_TFLASH_SUPPORT)
    STORAGE_SEC_CARD,
    #endif
    /*- zhouqin modify for dual t-flash card 20110503*/
   NO_OF_STORAGE
}
SoundRecorderStorageEnum;


typedef enum
{
#ifndef MT6205B
   FORMAT_AMR,
   FORMAT_WAV,
#endif
#if defined( AMRWB_ENCODE ) && defined( AMRWB_DECODE )
   FORMAT_AWB,
   FORMAT_VM,
#endif
   FORMAT_AMR,//chenhe move for above for jasperII
   #if 0
   FORMAT_MP3,// chenxia add for jasperII
   #else
   FORMAT_WAV,
   #endif
   NO_OF_FORMAT 
}


SoundRecorderFormatEnum;


typedef struct
{
   U16 noOfEntries;
   U16 noOfEntriesPhone;
   U16 noOfEntriesCard;
   U8 currentStorage;
   UI_character_type storagePath[NO_OF_STORAGE][MAX_STORAGE_PATH_LEN];
}
SoundFileListStruct;

typedef struct
{
   S32 dataRate[NO_OF_FORMAT];
   U8 storage;
   U8 format;
   BOOL cardAvailable;
   /*+ zhouqin modify for dual t-flash card 20110503*/
    #if defined(DUAL_TFLASH_SUPPORT)
    BOOL seccardAvailable;
    #endif
    /*- zhouqin modify for dual t-flash card 20110503*/
}
SoundRecorderSettingsStruct;


typedef struct
{
   SoundFileListStruct   SoundFileList;
   SoundRecorderSettingsStruct Settings;
   
   U8 state;
   BOOL inCall;
   BOOL cmdProcessing;
   BOOL inSoundRecorder;
   BOOL exitToStop;
   BOOL isAppend;
   BOOL isSlefStop;
   BOOL isFromCallExpress;
   U8 volume;
   UI_character_type fileName[MAX_FILE_NAME_LEN];
   UI_character_type fullName[MAX_FULL_NAME_LEN];
   U32 recTime;
   U32 constSeconds;
   U32 recSeconds;
}
SoundRecorderStruct;


#endif /* _SOUND_RECORDER_TYPE_H */


