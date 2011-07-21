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
* SoundEffect.c
*
* Project:
* --------
 
*
* Description:
* ------------
*   This file is intends to setup sound effect.
*
* Author:
* -------
 
*
*============================================================================
*             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/Setting/Src/SoundEffect.c-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:03:22   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:43:44   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:28:36   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Nov 06 2006 17:48:20   zhangxb
 * 
 *
 *------------------------------------------------------------------------------
 
*============================================================================
****************************************************************************/
#include "mmi_features.h"

#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"
#include "l4dr.h"
#include "wgui_categories.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
#include "settingstructs.h"
#include "settinggprots.h"
#include "protocolevents.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "commonscreens.h"
#include "settingprot.h"
#include "settingprofile.h"
#include "soundeffect.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "debuginitdef.h"
#include "mci.h"
#undef __NEWSIMULATOR

#ifdef __MMI_SOUND_EFFECT__
audio_equalizer_struct g_settings_audio_equalizer;
static U16 selected_audio_equalizer_index = 0;
#endif // __MMI_SOUND_EFFECT__

audio_mic_volume_struct g_settings_audio_mic_volume;

void InitSoundEffect( void )
{
#ifdef __MMI_SOUND_EFFECT__
	SetHiliteHandler( MENU_SETTING_SOUND_EFFECT, mmi_settings_sound_effect_highlight_hdlr );
	SetHiliteHandler( MENU_SETTING_AUDIO_EQUALIZIER, mmi_settings_audio_equalizer_highlight_hdlr );
	SetHintHandler( MENU_SETTING_AUDIO_EQUALIZIER, mmi_settings_audio_equalizer_hint_hdlr );
	mmi_settings_read_audio_equalizer_from_nvram( &g_settings_audio_equalizer );
//	mdi_audio_set_equalizer( g_settings_audio_equalizer.magnitude[g_settings_audio_equalizer.index] );
#endif // __MMI_SOUND_EFFECT__
	mmi_settings_read_audio_mic_volume_from_nvram( &g_settings_audio_mic_volume );
}

void InitEQ( void )
{
#ifdef __MMI_SOUND_EFFECT__
      #ifdef MMI_ON_HARDWARE_P
	MCI_AudioSetEQ(g_settings_audio_equalizer.index);
	  #endif
#endif // __MMI_SOUND_EFFECT__
}


#ifdef __MMI_SOUND_EFFECT__
void mmi_settings_read_audio_equalizer_from_nvram( audio_equalizer_struct* audio_equalizer_p )
{
#ifdef MMI_ON_HARDWARE_P
	S16 error;
	ReadRecord( NVRAM_EF_AUDIO_EQUALIZER_LID, 1, (void*)audio_equalizer_p, NVRAM_EF_AUDIO_EQUALIZER_SIZE, &error );
	if(audio_equalizer_p->count == 0x00)
	{
		audio_equalizer_p->count = MAX_EQUALIZER_NUM;
		audio_equalizer_p->index = 0;
		memset( audio_equalizer_p->magnitude, MAX_EQUALIZER_NUM*8, 0 );

		mmi_settings_write_audio_equalizer_to_nvram(audio_equalizer_p);
	}
#else
	audio_equalizer_p->count = MAX_EQUALIZER_NUM;
	audio_equalizer_p->index = 0;
	memset( audio_equalizer_p->magnitude, MAX_EQUALIZER_NUM*8, 0 );
#endif
	//MMI_ASSERT( audio_equalizer_p->count <= MAX_EQUALIZER_NUM );
	//MMI_ASSERT( audio_equalizer_p->index >= 0 && audio_equalizer_p->index < MAX_EQUALIZER_NUM );
}

void mmi_settings_write_audio_equalizer_to_nvram( audio_equalizer_struct* audio_equalizer_p )
{
#ifdef MMI_ON_HARDWARE_P
	S16 error;
	WriteRecord( NVRAM_EF_AUDIO_EQUALIZER_LID, 1, (void*)audio_equalizer_p, NVRAM_EF_AUDIO_EQUALIZER_SIZE, &error );
#endif
}

void mmi_settings_select_audio_equalizer( S16 index )
{
	if( g_settings_audio_equalizer.index != index )
	{
		g_settings_audio_equalizer.index = index;
//		mdi_audio_set_equalizer( g_settings_audio_equalizer.magnitude[index] );
      #ifdef MMI_ON_HARDWARE_P
		MCI_AudioSetEQ(index);
	 #endif 
		mmi_settings_write_audio_equalizer_to_nvram( &g_settings_audio_equalizer );
	}
}

void mmi_settings_sound_effect_highlight_hdlr( void )
{
	ChangeLeftSoftkey( STR_GLOBAL_OK, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, 0 );
	SetLeftSoftkeyFunction( mmi_settings_entry_sound_effect_list, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	SetKeyHandler( mmi_settings_entry_sound_effect_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN) ;
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

void mmi_settings_audio_equalizer_hint_hdlr( U16 index )
{
	memset( (S8 *)hintData[(index)], 0, MAX_SUB_MENU_SIZE );
	pfnUnicodeStrcpy( (S8 *)hintData[(index)], GetString( (U16)(STR_ID_SETTING_AUDIO_EQUALIZER_SET_1 + g_settings_audio_equalizer.index) ) );
}

void mmi_settings_entry_sound_effect_list( void )
{
	U16 n_items;
	U8* guiBuffer;
	U16 item_strs[10];
	U16 item_icons[10];
	U8 *item_hints[10];

	EntryNewScreen( SCR_ID_SETTING_SOUND_EFFECT , NULL, mmi_settings_entry_sound_effect_list, NULL );

	guiBuffer = GetCurrGuiBuffer( SCR_ID_SETTING_SOUND_EFFECT );	
	n_items = GetNumOfChild( MENU_SETTING_SOUND_EFFECT );

	GetSequenceStringIds( MENU_SETTING_SOUND_EFFECT, item_strs );
	GetSequenceImageIds( MENU_SETTING_SOUND_EFFECT, item_icons );

	SetParentHandler( MENU_SETTING_SOUND_EFFECT );

	RegisterHighlightHandler( ExecuteCurrHiliteHandler );
	ConstructHintsList( MENU_SETTING_SOUND_EFFECT, (U8**) item_hints );

	ShowCategory52Screen( STR_ID_SETTING_SOUND_EFFECT, IMG_SCR_SETTING_CAPTION,
		STR_GLOBAL_OK, IMG_GLOBAL_OK,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
		n_items, item_strs, item_icons, (U8**)item_hints,
		LIST_MENU, 0, guiBuffer );

	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

void mmi_settings_audio_equalizer_highlight_hdlr( void )
{
	ChangeLeftSoftkey( STR_GLOBAL_OK, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, 0 );
	SetLeftSoftkeyFunction( mmi_settings_entry_audio_equalizer_list, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	SetKeyHandler( mmi_settings_entry_audio_equalizer_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN) ;
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

void mmi_settings_entry_audio_equalizer_list( void )
{
	U8* guiBuffer;
	U16 audio_equalizer_list_strings[] = 
	{
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_1,
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_2,
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_3,
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_4,
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_5,
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_6,
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_7,
		STR_ID_SETTING_AUDIO_EQUALIZER_SET_8
	};
	
	EntryNewScreen( SCR_ID_SETTING_AUDIO_EQUALIZER, NULL, mmi_settings_entry_audio_equalizer_list, NULL );

	guiBuffer = GetCurrGuiBuffer( SCR_ID_SETTING_AUDIO_EQUALIZER );	
	
	SetParentHandler( MENU_SETTING_AUDIO_EQUALIZIER );
	RegisterHighlightHandler( mmi_settings_audio_equalizer_list_highlight_hdlr );

	ShowCategory11Screen( STR_ID_SETTING_AUDIO_EQUALIZER, IMG_SCR_SETTING_CAPTION,
		STR_GLOBAL_OK, IMG_GLOBAL_OK,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
		g_settings_audio_equalizer.count, audio_equalizer_list_strings, (U16) g_settings_audio_equalizer.index, guiBuffer );

	SetLeftSoftkeyFunction( mmi_settings_audio_equalizer_list_LSK_hdlr, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

void mmi_settings_audio_equalizer_list_highlight_hdlr( S32 index )
{
	selected_audio_equalizer_index = (U16) index;
}

void mmi_settings_audio_equalizer_list_LSK_hdlr( void )
{
	mmi_settings_select_audio_equalizer( (S16)selected_audio_equalizer_index );
	DisplayPopup( (PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE );
	DeleteScreenIfPresent( SCR_ID_SETTING_AUDIO_EQUALIZER );
}
#endif // __MMI_SOUND_EFFECT__

void mmi_settings_read_audio_mic_volume_from_nvram( audio_mic_volume_struct* audio_mic_volume_p )
{
#ifdef MMI_ON_HARDWARE_P
	S16 error;
	U16 mic_volume_set;
	U8 set_index;
	unsigned char *ptr = (unsigned char *) &mic_volume_set;
	
	ReadValue( NVRAM_AUDIO_MIC_VOLUME_SET, &mic_volume_set, DS_SHORT, &error );

	for( set_index = 0; set_index < AUDIO_MIC_SET_NUM; set_index++ )
	{
		// check if the value from NVRAM an invalid data
		if( *ptr == 0xFF )
		{
			ptr = (unsigned char *) &mic_volume_set;
			// reset in call mic volume level to 3
			audio_mic_volume_p->levels[AUDIO_MIC_IN_CALL] = *ptr++ = 3;
			// reset app mic volume level to 4
			audio_mic_volume_p->levels[AUDIO_MIC_APP] = *ptr++ = 4;
			WriteValue( NVRAM_AUDIO_MIC_VOLUME_SET, &mic_volume_set, DS_SHORT, &error );
			break;
		}
		else
			audio_mic_volume_p->levels[set_index] = *ptr++;
	}
#endif
}

void mmi_settings_write_audio_mic_volume_to_nvram( audio_mic_volume_struct* audio_mic_volume_p )
{
#ifdef MMI_ON_HARDWARE_P
	S16 error;
	U16 mic_volume_set;
	U8 set_index;
	unsigned char *ptr = (unsigned char *) &mic_volume_set;
	
	for( set_index = 0; set_index < AUDIO_MIC_SET_NUM; set_index++ )
		*ptr++ = audio_mic_volume_p->levels[set_index];
	WriteValue( NVRAM_AUDIO_MIC_VOLUME_SET, &mic_volume_set, DS_SHORT, &error );
#endif
}


