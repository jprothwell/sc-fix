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
* SoundEffect.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   This file is the header file of sound effect setup.
*
* Author:
* -------
 
*
*============================================================================
*             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/Setting/include/SoundEffect.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:03:10   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:43:32   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:28:28   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Nov 06 2006 17:48:02   zhangxb
 * Initial revision.
 * Revision 1.1.1.1  2006/06/08 12:43:25  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
*============================================================================
****************************************************************************/
#ifndef _SOUND_EFFECT_H_
#define _SOUND_EFFECT_H_

#define MAX_EQUALIZER_NUM	8 // double check if NVRAM_EF_AUDIO_EQUALIZER_COUNT == MAX_EQUALIZER_NUM

typedef enum
{
	AUDIO_MIC_IN_CALL,
	AUDIO_MIC_APP,
	AUDIO_MIC_SET_NUM
} AUDIO_MIC_SET_ENUM;

typedef struct
{
	S16 count;
	S16 index;
	signed char magnitude[MAX_EQUALIZER_NUM][8];
} audio_equalizer_struct;

typedef struct
{
	unsigned char levels[AUDIO_MIC_SET_NUM];
} audio_mic_volume_struct;

extern audio_equalizer_struct g_settings_audio_equalizer;

void InitSoundEffect( void );
void InitEQ( void );
void mmi_settings_read_audio_equalizer_from_nvram( audio_equalizer_struct* audio_equalizer_p );
void mmi_settings_write_audio_equalizer_to_nvram( audio_equalizer_struct* audio_equalizer_p );
void mmi_settings_select_audio_equalizer( S16 index );
void mmi_settings_sound_effect_highlight_hdlr( void );
void mmi_settings_entry_sound_effect_list( void );
void mmi_settings_audio_equalizer_highlight_hdlr( void );
void mmi_settings_audio_equalizer_hint_hdlr( U16 index );
void mmi_settings_entry_audio_equalizer_list( void );
void mmi_settings_audio_equalizer_list_highlight_hdlr( S32 index );
void mmi_settings_audio_equalizer_list_LSK_hdlr( void );

void mmi_settings_read_audio_mic_volume_from_nvram( audio_mic_volume_struct* audio_mic_volume_p );
void mmi_settings_write_audio_mic_volume_to_nvram( audio_mic_volume_struct* audio_mic_volume_p );

#endif /* _SOUND_EFFECT_H_ */

