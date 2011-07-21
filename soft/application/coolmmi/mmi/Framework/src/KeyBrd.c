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
 * Keybrd.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for keypad processing
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: KeyBrd.c

  	PURPOSE		: Key Board Handler 

 

 

	DATE		: Aug 25, 2003

**************************************************************/
/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "keybrd.h"
#include "timerevents.h"

#include "audioinc.h"
#include "globalconstants.h"
#include "stdc.h"
#include "globalconstants.h"
#include "thrdosinterface.h"
#include "debuginitdef.h"
#include "frameworkstruct.h"
#include "settingprofile.h"
#include "idleappdef.h"
#include "queuegprot.h"
#include "wrappergprot.h"
#include "profilegprots.h"
#undef __NEWSIMULATOR 

#include "l4dr.h"
 
#include "eventsgprot.h"
#include "gpioinc.h"
#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif

#include "mmi_trace.h"


/***************************************************************************** 
* Define
*****************************************************************************/
#define MAX_PRESENTALLKEYS	sizeof(PresentAllKeys)/sizeof(PresentAllKeys[0])
#define MAX_PRESENTDIGITSKEYS sizeof(PresentDigitsKeys)/sizeof(PresentDigitsKeys[0])
#define MAX_KEYPADMAP	sizeof(nKeyPadMap)/sizeof(nKeyPadMap[0])

#if defined(__MMI_BLACKBERRY_QWERTY__)
#define MAX_PRESENTALLKEYS_QWERT	sizeof(PresentAllKeys_qwert)/sizeof(PresentAllKeys_qwert[0])
#endif
mmi_frm_context_struct g_mmi_frm_cntx;
mmi_frm_context_struct *frm_p = &g_mmi_frm_cntx;

/***************************************************************************** 
* Local Variable
*****************************************************************************/
static U16 currKeyPadTone=0xFFFF;
static mmi_key_types_enum nKeyPadStatus[MAX_KEYS];

/***************************************************************************** 
* Global Variable
*****************************************************************************/
const U16 PresentDigitsKeys[]=
{KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9};
const U16 PresentAllKeys[]= 
{												 
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_LSK,
		KEY_RSK,
		KEY_UP_ARROW,
		KEY_DOWN_ARROW,
		KEY_LEFT_ARROW,	
		KEY_RIGHT_ARROW,		
		KEY_SEND,
		KEY_END,
		KEY_STAR,
		KEY_POUND,
		KEY_VOL_UP,
		KEY_VOL_DOWN,
		KEY_CLEAR,
		KEY_CAMERA,
		KEY_WAP,
#ifdef __SENDKEY2_SUPPORT__
	    KEY_SEND1,
	    KEY_SEND2,
#endif	/* __SENDKEY2_SUPPORT__ */
		KEY_EXTRA_1,
		KEY_EXTRA_2,
#ifdef TGT_GALLITE_G800
#ifdef TGT_USING_POWER_KEY
		KEY_POWER,
#endif
		KEY_ENTER,
#else
		KEY_FM,
#if defined(__PROJECT_GALLITE_C01__)
    	KEY_QQ,
#endif
    	KEY_MUSIC,
    	KEY_MUSIC_PREV,
    	KEY_MUSIC_NEXT,
		KEY_MUSIC_PLAY,
		KEY_MUSIC_STOP,
#endif
};
#if defined(__MMI_BLACKBERRY_QWERTY__)

const U16 PresentAllKeys_qwert[]= 
{
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_STAR,
	KEY_POUND, //12
	KEY_ENTER,
	KEY_LSK,
	KEY_RSK,
	KEY_UP_ARROW,
	 KEY_DOWN_ARROW,
	KEY_LEFT_ARROW,
	KEY_RIGHT_ARROW,
	KEY_ENTER,
	KEY_SEND1,
	KEY_END,
	KEY_VOL_DOWN,
	KEY_VOL_UP,

#ifdef __MMI_FULL_KEYPAD__
	KEY_CLEAR,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_BACKSPACE1,
	KEY_SMS,
	KEY_CLOG,
	KEY_COMMA,
	KEY_AND,
	KEY_FULLSTOP,
	KEY_SHIFT_INPUTM,
	KEY_SHIFT1,
	KEY_SPACE1,
	KEY_INTEGRR,
	KEY_CALENDER,
	KEY_NEWLINE,
	KEY_CAMERA,
	KEY_SYM,
	KEY_DOLLAR,
	KEY_STOP,
	KEY_MUSIC,
	KEY_SEND2,
#endif  //endif __MMI_FULL_KEYPAD__
	KEY_FM
};
#endif
#ifdef MMI_ON_HARDWARE_P
#ifdef __MMI_BLACKBERRY_QWERTY__
static const KeyPadMap nKeyPadMap[] = {
{KEY_0,				KEY_0,					KEY_TIMER_ID0,				TONE_DTMF_0,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_1,				KEY_1,					KEY_TIMER_ID1,				TONE_DTMF_1,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_2,				KEY_2,					KEY_TIMER_ID2,				TONE_DTMF_2,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_3,				KEY_3,					KEY_TIMER_ID3,				TONE_DTMF_3,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_4,				KEY_4,					KEY_TIMER_ID4,				TONE_DTMF_4,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_5,				KEY_5,					KEY_TIMER_ID5,				TONE_DTMF_5,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_6,				KEY_6,					KEY_TIMER_ID6,				TONE_DTMF_6,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_7,				KEY_7,					KEY_TIMER_ID7,				TONE_DTMF_7,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_8,				KEY_8,					KEY_TIMER_ID8,				TONE_DTMF_8,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_9,				KEY_9,					KEY_TIMER_ID9,				TONE_DTMF_9,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_LSK,			KEY_LSK,				KEY_TIMER_ID10,				TONE_KEY_SK_LEFT,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_RSK,			KEY_RSK,				KEY_TIMER_ID11,				TONE_KEY_RIGHT,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_UP_ARROW,	   	KEY_UP_ARROW,			KEY_TIMER_ID12,				TONE_KEY_UP,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_DOWN_ARROW,   KEY_DOWN_ARROW,			KEY_TIMER_ID13,				TONE_KEY_DOWN,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_LEFT_ARROW,	KEY_LEFT_ARROW,			KEY_TIMER_ID14,				TONE_KEY_LEFT,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_RIGHT_ARROW,   KEY_RIGHT_ARROW,		KEY_TIMER_ID15,				TONE_KEY_RIGHT,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_SEND1,			KEY_SEND1,				KEY_TIMER_ID16,				TONE_KEY_SEND,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_END,			KEY_END,				KEY_TIMER_ID17,				TONE_KEY_END,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_CLEAR,			KEY_CLEAR,				KEY_TIMER_ID25,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_STAR,			KEY_STAR,				KEY_TIMER_ID19,				TONE_KEY_STAR,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_POUND,			KEY_POUND,				KEY_TIMER_ID20,				TONE_KEY_HASH,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_VOL_UP,		KEY_VOL_UP,				KEY_TIMER_ID21,				TONE_KEY_VOL_UP,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_VOL_DOWN,	       KEY_VOL_DOWN,		KEY_TIMER_ID22,				TONE_KEY_VOL_DOWN,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_INVALID,		KEY_INVALID,			KEY_TIMER_ID_NONE,			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_QUICK_ACS,		KEY_QUICK_ACS ,			KEY_TIMER_ID23,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_ENTER,			KEY_ENTER,				KEY_TIMER_ID24,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_MSN, 		KEY_MSN,				KEY_TIMER_ID26, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_DOLLAR, 		KEY_DOLLAR,				KEY_TIMER_ID58, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SYM, 		KEY_SYM,				KEY_TIMER_ID59, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_WAP, 		KEY_WAP,				KEY_TIMER_ID60, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_IP, 		KEY_IP,				KEY_TIMER_ID61, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_EXTRA_1,		KEY_EXTRA_1,			KEY_TIMER_ID27,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_EXTRA_2,		KEY_EXTRA_2,			KEY_TIMER_ID28,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SEND2,		KEY_SEND2,			KEY_TIMER_ID29,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SEND3,		KEY_SEND3,			KEY_TIMER_ID30,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SMS,				KEY_SMS,				KEY_TIMER_ID31,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_CLOG,				KEY_CLOG,				KEY_TIMER_ID32,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_B,					KEY_B,					KEY_TIMER_ID33,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_G, 				KEY_G,					KEY_TIMER_ID42, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_H,					KEY_H,					KEY_TIMER_ID34,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_I,					KEY_I,					KEY_TIMER_ID35,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_J,					KEY_J,					KEY_TIMER_ID36,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_K,					KEY_K,					KEY_TIMER_ID37,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_L,					KEY_L,					KEY_TIMER_ID38,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_M,					KEY_M,					KEY_TIMER_ID39,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_N, 				KEY_N,					KEY_TIMER_ID43, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_O,					KEY_O,					KEY_TIMER_ID40,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_P,					KEY_P,					KEY_TIMER_ID41,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_T, 				KEY_T,					KEY_TIMER_ID45, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_U,					KEY_U,					KEY_TIMER_ID44,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_V, 				KEY_V,					KEY_TIMER_ID47, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_Y,					KEY_Y,					KEY_TIMER_ID46,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_COMMA,			KEY_COMMA,				KEY_TIMER_ID48,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_FULLSTOP,			KEY_FULLSTOP,			KEY_TIMER_ID49,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SHIFT1,				KEY_SHIFT1,				KEY_TIMER_ID50,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SHIFT_INPUTM,		KEY_SHIFT_INPUTM,		KEY_TIMER_ID51,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SPACE1,			KEY_SPACE1,				KEY_TIMER_ID52,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_AND,				KEY_AND,				KEY_TIMER_ID53,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_INTEGRR,			KEY_INTEGRR,			KEY_TIMER_ID54,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_CTRL1,				KEY_CTRL1,				KEY_TIMER_ID55,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_NEWLINE,			KEY_NEWLINE,			KEY_TIMER_ID56,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_BACKSPACE1,		KEY_BACKSPACE1,		KEY_TIMER_ID57,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_QQ,				KEY_QQ, 			KEY_TIMER_ID58, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_QF,				KEY_QF, 			KEY_TIMER_ID59, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_MUSIC,			KEY_MUSIC, 			KEY_TIMER_ID59, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_FM,		        		KEY_FM,					KEY_TIMER_ID58,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_CALENDER,		        KEY_CALENDER,			KEY_TIMER_ID59,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SLASH,		        KEY_SLASH,					KEY_TIMER_ID60,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},

{MAX_KEYS,		MAX_KEYS,			KEY_TIMER_ID56,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE}
};

#else
static const KeyPadMap nKeyPadMap[] = {
{KEY_0,				KEY_0,					KEY_TIMER_ID0,				TONE_DTMF_0,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_1,				KEY_1,					KEY_TIMER_ID1,				TONE_DTMF_1,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_2,				KEY_2,					KEY_TIMER_ID2,				TONE_DTMF_2,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_3,				KEY_3,					KEY_TIMER_ID3,				TONE_DTMF_3,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_4,				KEY_4,					KEY_TIMER_ID4,				TONE_DTMF_4,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_5,				KEY_5,					KEY_TIMER_ID5,				TONE_DTMF_5,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_6,				KEY_6,					KEY_TIMER_ID6,				TONE_DTMF_6,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_7,				KEY_7,					KEY_TIMER_ID7,				TONE_DTMF_7,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_8,				KEY_8,					KEY_TIMER_ID8,				TONE_DTMF_8,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_9,				KEY_9,					KEY_TIMER_ID9,				TONE_DTMF_9,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_LSK,			KEY_LSK,					KEY_TIMER_ID10,				TONE_KEY_SK_LEFT,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_RSK,			KEY_RSK,				KEY_TIMER_ID11,				TONE_KEY_RIGHT,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_UP_ARROW,	   	KEY_UP_ARROW,			KEY_TIMER_ID12,				TONE_KEY_UP,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_DOWN_ARROW,   KEY_DOWN_ARROW,		KEY_TIMER_ID13,				TONE_KEY_DOWN,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_LEFT_ARROW,	KEY_LEFT_ARROW,			KEY_TIMER_ID14,				TONE_KEY_LEFT,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_RIGHT_ARROW,   KEY_RIGHT_ARROW,		KEY_TIMER_ID15,				TONE_KEY_RIGHT,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_SEND,			KEY_SEND,				KEY_TIMER_ID16,				TONE_KEY_SEND,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_END,			KEY_END,				KEY_TIMER_ID17,				TONE_KEY_END,			DEVICE_AUDIO_PLAY_ONCE},
{KEY_CLEAR,			KEY_CLEAR,				KEY_TIMER_ID25,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_STAR,			KEY_STAR,				KEY_TIMER_ID19,				TONE_KEY_STAR,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_POUND,			KEY_POUND,				KEY_TIMER_ID20,				TONE_KEY_HASH,			DEVICE_AUDIO_PLAY_INFINITE},
{KEY_VOL_UP,		KEY_VOL_UP,				KEY_TIMER_ID21,				TONE_KEY_VOL_UP,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_VOL_DOWN,	       KEY_VOL_DOWN,			KEY_TIMER_ID22,				TONE_KEY_VOL_DOWN,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_INVALID,		KEY_INVALID,				KEY_TIMER_ID_NONE,			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_QUICK_ACS,		KEY_QUICK_ACS ,			KEY_TIMER_ID19,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_ENTER,			KEY_ENTER,				KEY_TIMER_ID18,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_EXTRA_1,		KEY_EXTRA_1,			KEY_TIMER_ID27,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_EXTRA_2,		KEY_EXTRA_2,			KEY_TIMER_ID28,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
#ifdef __SENDKEY2_SUPPORT__
{KEY_SEND1,		KEY_SEND1,			KEY_TIMER_ID29,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SEND2,		KEY_SEND2,			KEY_TIMER_ID30,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
#endif
{KEY_SMS,				KEY_SMS,				KEY_TIMER_ID31,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_CLOG,				KEY_CLOG,				KEY_TIMER_ID32,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_A,					KEY_A,					KEY_TIMER_ID33,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_H,					KEY_H,					KEY_TIMER_ID34,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_I,					KEY_I,					KEY_TIMER_ID35,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_J,					KEY_J,					KEY_TIMER_ID36,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_K,					KEY_K,					KEY_TIMER_ID37,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_L,					KEY_L,					KEY_TIMER_ID38,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_M,					KEY_M,					KEY_TIMER_ID39,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_O,					KEY_O,					KEY_TIMER_ID40,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_P,					KEY_P,					KEY_TIMER_ID41,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_Q,					KEY_Q,					KEY_TIMER_ID42,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_S,					KEY_S,					KEY_TIMER_ID43,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_U,					KEY_U,					KEY_TIMER_ID44,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_W,					KEY_W,					KEY_TIMER_ID45,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_Y,					KEY_Y,					KEY_TIMER_ID46,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_Z,					KEY_Z,					KEY_TIMER_ID47,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_COMMA,				KEY_COMMA,				KEY_TIMER_ID48,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_FULLSTOP,			KEY_FULLSTOP,			KEY_TIMER_ID49,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SHIFT1,			KEY_SHIFT1,				KEY_TIMER_ID50,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SHIFT_INPUTM,		KEY_SHIFT_INPUTM,		KEY_TIMER_ID51,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SPACE1,			KEY_SPACE1,				KEY_TIMER_ID52,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_AND,				KEY_AND,				KEY_TIMER_ID53,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_INTEGRR,			KEY_INTEGRR,			KEY_TIMER_ID54,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_CTRL1,				KEY_CTRL1,				KEY_TIMER_ID55,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_NEWLINE,			KEY_NEWLINE,			KEY_TIMER_ID56,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_BACKSPACE1,		KEY_BACKSPACE1,			KEY_TIMER_ID57,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
#ifdef TGT_GALLITE_G800
{KEY_CAMERA,	KEY_CAMERA,		KEY_TIMER_ID58,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_POWER,		KEY_POWER,		KEY_TIMER_ID59,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{MAX_KEYS,		MAX_KEYS,		KEY_TIMER_ID60,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE}
#else
{KEY_FM,		KEY_FM,			KEY_TIMER_ID58,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_MUSIC,		KEY_MUSIC,		KEY_TIMER_ID59,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_MUSIC_PLAY,KEY_MUSIC_PLAY,	KEY_TIMER_ID60,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_MUSIC_STOP,KEY_MUSIC_STOP,	KEY_TIMER_ID61,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_MUSIC_PREV,KEY_MUSIC_PREV,	KEY_TIMER_ID62,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_MUSIC_NEXT,KEY_MUSIC_NEXT,	KEY_TIMER_ID63,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_CALENDER,	KEY_CALENDER,	KEY_TIMER_ID64,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_SLASH,	    KEY_SLASH,		KEY_TIMER_ID65,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
#if defined(__PROJECT_GALLITE_C01__)
{KEY_QQ,		KEY_QQ,			KEY_TIMER_ID66,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{MAX_KEYS,		MAX_KEYS,		KEY_TIMER_ID67,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE}
#else
{MAX_KEYS,		MAX_KEYS,		KEY_TIMER_ID66,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE}
#endif
#endif
};
#endif
 
#else /* !MMI_ON_HARDWARE_P */
/* 
 * virtual-Key Code is defined in "WINUSER.H", and "WINDOWS.H" includes "WINUSER.H" 
 *
 * Constant name 				Value (hexadecimal) 
 *-------------------------------------------------
 * #define VK_SPACE          0x20
 * #define VK_PRIOR          0x21
 * #define VK_NEXT           0x22
 * #define VK_END            0x23
 * #define VK_HOME           0x24
 * #define VK_LEFT           0x25
 * #define VK_UP             0x26
 * #define VK_RIGHT          0x27
 * #define VK_DOWN           0x28
 * #define VK_SELECT         0x29
 * #define VK_PRINT          0x2A
 * #define VK_EXECUTE        0x2B
 * #define VK_SNAPSHOT       0x2C
 * #define VK_INSERT         0x2D
 * #define VK_DELETE         0x2E
 * #define VK_HELP           0x2F
 *
 *	[ VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) ]
 *	[ VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) ]
 *
 * #define VK_LWIN           0x5B
 * #define VK_RWIN           0x5C
 * #define VK_APPS           0x5D
 * 
 * #define VK_NUMPAD0        0x60
 * #define VK_NUMPAD1        0x61
 * #define VK_NUMPAD2        0x62
 * #define VK_NUMPAD3        0x63
 * #define VK_NUMPAD4        0x64
 * #define VK_NUMPAD5        0x65
 * #define VK_NUMPAD6        0x66
 * #define VK_NUMPAD7        0x67
 * #define VK_NUMPAD8        0x68
 * #define VK_NUMPAD9        0x69
 * #define VK_MULTIPLY       0x6A
 * #define VK_ADD            0x6B
 * #define VK_SEPARATOR      0x6C
 * #define VK_SUBTRACT       0x6D
 * #define VK_DECIMAL        0x6E
 * #define VK_DIVIDE         0x6F
 * #define VK_F1             0x70
 * #define VK_F2             0x71
 * #define VK_F3             0x72
 * #define VK_F4             0x73
 * #define VK_F5             0x74
 * #define VK_F6             0x75
 * #define VK_F7             0x76
 * #define VK_F8             0x77
 * #define VK_F9             0x78
 * #define VK_F10            0x79
 * #define VK_F11            0x7A
 * #define VK_F12            0x7B
 * #define VK_F13            0x7C
 * #define VK_F14            0x7D
 * #define VK_F15            0x7E
 * #define VK_F16            0x7F
 * #define VK_F17            0x80
 * #define VK_F18            0x81
 * #define VK_F19            0x82
 * #define VK_F20            0x83
 * #define VK_F21            0x84
 * #define VK_F22            0x85
 * #define VK_F23            0x86
 * #define VK_F24            0x87
 *
 */

const KeyPadMap nKeyPadMap[] = {
{KEY_0,				KEY_0,					KEY_TIMER_ID0,				TONE_DTMF_0,				DEVICE_AUDIO_PLAY_INFINITE,     VK_NUMPAD0/* 0 */},
{KEY_1,				KEY_1,					KEY_TIMER_ID1,				TONE_DTMF_1,				DEVICE_AUDIO_PLAY_INFINITE,     VK_NUMPAD1/* 1 */},
{KEY_2,				KEY_2,					KEY_TIMER_ID2,				TONE_DTMF_2,				DEVICE_AUDIO_PLAY_INFINITE,     VK_NUMPAD2/* 2 */},
{KEY_3,				KEY_3,					KEY_TIMER_ID3,				TONE_DTMF_3,				DEVICE_AUDIO_PLAY_INFINITE,     VK_NUMPAD3/* 3 */},
{KEY_4,				KEY_4,					KEY_TIMER_ID4,				TONE_DTMF_4,				DEVICE_AUDIO_PLAY_INFINITE,     VK_NUMPAD4/* 4 */},
{KEY_5,				KEY_5,					KEY_TIMER_ID5,				TONE_DTMF_5,				DEVICE_AUDIO_PLAY_INFINITE,     VK_NUMPAD5/* 5 */},
{KEY_6,				KEY_6,					KEY_TIMER_ID6,				TONE_DTMF_6,				DEVICE_AUDIO_PLAY_INFINITE,     VK_NUMPAD6/* 6 */},
{KEY_7,				KEY_7,					KEY_TIMER_ID7,				TONE_DTMF_7,				DEVICE_AUDIO_PLAY_INFINITE,     VK_NUMPAD7/* 7 */},
{KEY_8,				KEY_8,					KEY_TIMER_ID8,				TONE_DTMF_8,				DEVICE_AUDIO_PLAY_INFINITE,     VK_NUMPAD8/* 8 */},
{KEY_9,				KEY_9,					KEY_TIMER_ID9,				TONE_DTMF_9,				DEVICE_AUDIO_PLAY_INFINITE,     VK_NUMPAD9/* 9 */},
{KEY_LSK,			KEY_LSK,					KEY_TIMER_ID10,			TONE_KEY_STAR,				DEVICE_AUDIO_PLAY_INFINITE,     VK_INSERT/* Ins */},
{KEY_RSK,			KEY_RSK,					KEY_TIMER_ID11,			TONE_KEY_HASH,				DEVICE_AUDIO_PLAY_INFINITE,     VK_PRIOR/* PgUp */},
{KEY_UP_ARROW,	   KEY_UP_ARROW,			KEY_TIMER_ID12,			TONE_KEY_VOL_UP,			DEVICE_AUDIO_PLAY_ONCE,         VK_UP/* Up */},
{KEY_DOWN_ARROW,  KEY_DOWN_ARROW,		KEY_TIMER_ID13,			TONE_KEY_VOL_DOWN,		DEVICE_AUDIO_PLAY_ONCE,         VK_DOWN/* Down */},
{KEY_LEFT_ARROW,	KEY_LEFT_ARROW,		KEY_TIMER_ID14,			TONE_KEY_LEFT,				DEVICE_AUDIO_PLAY_ONCE,         VK_LEFT/* Left */},
{KEY_RIGHT_ARROW, KEY_RIGHT_ARROW,		KEY_TIMER_ID15,			TONE_KEY_RIGHT,			DEVICE_AUDIO_PLAY_ONCE,         VK_RIGHT/* Right */},
{KEY_SEND,			KEY_SEND,				KEY_TIMER_ID16,			TONE_KEY_SEND,				DEVICE_AUDIO_PLAY_ONCE,         VK_DELETE/* Del */},
{KEY_END,			KEY_END,					KEY_TIMER_ID17,			TONE_KEY_END,				DEVICE_AUDIO_PLAY_ONCE,         VK_NEXT/* PgDn */},
{KEY_CLEAR,			KEY_CLEAR,				KEY_TIMER_ID25,			TONE_KEY_NORMAL,			DEVICE_AUDIO_PLAY_ONCE,         VK_END/* End */},
{KEY_STAR,			KEY_STAR,				KEY_TIMER_ID19,			TONE_KEY_STAR,				DEVICE_AUDIO_PLAY_INFINITE,     VK_DIVIDE/* / */},
{KEY_POUND,			KEY_POUND,				KEY_TIMER_ID20,			TONE_KEY_HASH,				DEVICE_AUDIO_PLAY_INFINITE,     VK_MULTIPLY/* * */},
{KEY_VOL_UP,		KEY_VOL_UP,				KEY_TIMER_ID21,			TONE_KEY_VOL_UP,			DEVICE_AUDIO_PLAY_ONCE,         VK_ADD/* + */},
{KEY_VOL_DOWN,	   KEY_VOL_DOWN,			KEY_TIMER_ID22,			TONE_KEY_VOL_DOWN,		DEVICE_AUDIO_PLAY_ONCE,         VK_SUBTRACT/* - */},
{KEY_INVALID,		KEY_INVALID,			KEY_TIMER_ID_NONE,		TONE_KEY_NORMAL,			DEVICE_AUDIO_PLAY_ONCE,         0/* None */},
{KEY_QUICK_ACS,	KEY_QUICK_ACS ,		KEY_TIMER_ID19,			TONE_KEY_NORMAL,			DEVICE_AUDIO_PLAY_ONCE,         VK_BACK/* Backspace */},
{KEY_ENTER,			KEY_ENTER,				KEY_TIMER_ID18,			TONE_KEY_NORMAL,			DEVICE_AUDIO_PLAY_ONCE,         VK_RETURN/* Enter */},
{KEY_EXTRA_1,		KEY_EXTRA_1,			KEY_TIMER_ID27,			TONE_KEY_NORMAL,			DEVICE_AUDIO_PLAY_ONCE,         VK_F1/* F1 */},
{KEY_EXTRA_2,		KEY_EXTRA_2,			KEY_TIMER_ID28,			TONE_KEY_NORMAL,			DEVICE_AUDIO_PLAY_ONCE,         VK_F2/* F2 */},
#ifdef __SENDKEY2_SUPPORT__
{KEY_SEND1,		KEY_SEND1,			KEY_TIMER_ID29,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_SEND2,		KEY_SEND2,			KEY_TIMER_ID30,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
#endif
{KEY_SMS,		KEY_SMS,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_CLOG,		KEY_CLOG,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_A,		KEY_A,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_B,		KEY_B,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_C, 	KEY_C,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE,0},
{KEY_D, 	KEY_D,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE,0},
{KEY_E, 	KEY_E,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE,0},
{KEY_F, 	KEY_F,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE,0},
{KEY_G, 	KEY_G,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE,0},
{KEY_H,		KEY_H,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_I,		KEY_I,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_J,		KEY_J,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_K,		KEY_K,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_L,		KEY_L,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_M,		KEY_M,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_N, 	KEY_N,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE,0},	
{KEY_O,		KEY_O,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_P,		KEY_P,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_Q,		KEY_Q,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_R, 	KEY_R,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE,0},
{KEY_S,		KEY_S,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_T, 	KEY_T,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE,0},
{KEY_U,		KEY_U,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_V, 	KEY_V,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE,0},
{KEY_W,		KEY_W,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_X, 	KEY_X,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE,0},
{KEY_Y,		KEY_Y,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
#ifdef __MMI_BLACKBERRY_QWERTY__
{KEY_SYM,		KEY_SYM,	KEY_TIMER_ID59, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
{KEY_DOLLAR,	KEY_DOLLAR,KEY_TIMER_ID59, 			TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE},
#endif
{KEY_Z,		KEY_Z,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_COMMA,		KEY_COMMA,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_FULLSTOP,		KEY_FULLSTOP,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_SHIFT1,		KEY_SHIFT1,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_SHIFT_INPUTM,		KEY_SHIFT_INPUTM,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_SPACE1,		KEY_SPACE1,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_AND,		KEY_AND,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_INTEGRR,		KEY_INTEGRR,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_CTRL1,		KEY_CTRL1,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
{KEY_NEWLINE,		KEY_NEWLINE,			KEY_TIMER_ID_NONE,				TONE_KEY_NORMAL,		DEVICE_AUDIO_PLAY_ONCE, 0},
};
#endif /* MMI_ON_HARDWARE_P */

extern void IdleHandleKeypadLockProcess(void);

#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
extern U8 CanPlayHumanVoiceKeyTone( void );
#endif

#if defined(__MMI_CUST_KEYPAD_TONE__)
extern U8 CanPlayCustKeyTone( void );
#endif

#if defined(MMI_ON_WIN32)
extern void MMI_system_keybord_handler(S32 key_code,S32 key_state);
#endif


/**************************************************************

	FUNCTION NAME		: InitProcessKeyEvent()

  	PURPOSE				: To init the process key handler for preprocessing and postprocessing.

	INPUT PARAMETERS	: U32 MsgType, U16 KeyMapIndex

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
void InitProcessKeyEvent(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   U16  i = 0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   for (i = 0; i < MAX_KEYS; i++)
   {
      nKeyPadStatus[i] = KEY_EVENT_UP;
   }
}


/**************************************************************

	FUNCTION NAME		: KeyEventHandler()

  	PURPOSE				: To handle the key event.

	INPUT PARAMETERS	: KEYBRD_MESSAGE *eventKey

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
static void KeyEventHandler(KEYBRD_MESSAGE *eventKey) 
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* Preprocess the keypad handler */
	mmi_kbd_app_key_hdlr(eventKey);

	if(g_mmi_frm_cntx.kbd_pre_func)
		g_mmi_frm_cntx.kbd_pre_func(eventKey); 

	/* Process the register App keypad handler */
	ExecuteCurrKeyHandler((S16)eventKey->nKeyCode,(S16)eventKey->nMsgType);

	if(g_mmi_frm_cntx.kbd_post_func)
	  g_mmi_frm_cntx.kbd_post_func(eventKey); 

}

/**************************************************************

	FUNCTION NAME		: ProcessKeyEvent()

  	PURPOSE				: To send the key events to MMI task

	INPUT PARAMETERS	: U32 MsgType, U16 KeyMapIndex

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
void ProcessKeyEvent(U32 MsgType, U16 KeyMapIndex)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_ASSERT(KeyMapIndex<MAX_KEYPADMAP);
	
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_PROC_KEYEVENT_HDLR, MsgType, nKeyPadMap[KeyMapIndex].nMMIKeyCode));
	
	if(MsgType == WM_KEYPRESS)
	{
		if (nKeyPadStatus[KeyMapIndex] == KEY_EVENT_UP)
		{
			
			KEYBRD_MESSAGE KeyBrdMsg;			

			nKeyPadStatus[KeyMapIndex] = KEY_EVENT_DOWN;
			KeyBrdMsg.nMsgType = KEY_EVENT_DOWN;
			KeyBrdMsg.nKeyCode = nKeyPadMap[KeyMapIndex].nMMIKeyCode;			
			KeyEventHandler((struct KEYBRD_MESSAGE *)&KeyBrdMsg);			
			
			#ifndef MMI_ON_HARDWARE_P
			StartMyTimer(nKeyPadMap[KeyMapIndex].nTimerId,KEYTIMER_LONGPRESS,(oslTimerFuncPtr)KeyTimerExpiryProc);
			#endif
		}
		else
		{
			//Ignore the event
		}
	}

	else if(MsgType == WM_KEYRELEASE)
	{
		if ( (nKeyPadStatus[KeyMapIndex] == KEY_EVENT_DOWN)
		     || (nKeyPadStatus[KeyMapIndex] == KEY_LONG_PRESS)
		     || (nKeyPadStatus[KeyMapIndex] == KEY_REPEAT) )
		{

			KEYBRD_MESSAGE KeyBrdMsg;			

			nKeyPadStatus[KeyMapIndex] = KEY_EVENT_UP;
			KeyBrdMsg.nMsgType = KEY_EVENT_UP;
			KeyBrdMsg.nKeyCode = nKeyPadMap[KeyMapIndex].nMMIKeyCode;			
			KeyEventHandler((struct KEYBRD_MESSAGE *)&KeyBrdMsg);			

			#ifndef MMI_ON_HARDWARE_P
			StopMyTimer(nKeyPadMap[KeyMapIndex].nTimerId);
			#endif
		}
		else
		{
			//Ignore the event
		}
	}
    
  #ifdef MMI_ON_HARDWARE_P
	else if(MsgType == DRV_WM_KEYLONGPRESS)
	{
		if (nKeyPadStatus[KeyMapIndex] == KEY_EVENT_DOWN)
		{
			KEYBRD_MESSAGE KeyBrdMsg;			
			
			nKeyPadStatus[KeyMapIndex] = KEY_LONG_PRESS;
			KeyBrdMsg.nMsgType = KEY_LONG_PRESS;
			KeyBrdMsg.nKeyCode = nKeyPadMap[KeyMapIndex].nMMIKeyCode;			
			KeyEventHandler((struct KEYBRD_MESSAGE *)&KeyBrdMsg);			
		}
		else
		{
			//Ignore the event
		}
	}
	else if(MsgType == DRV_WM_KEYREPEATED)
	{
		if ( (nKeyPadStatus[KeyMapIndex] == KEY_LONG_PRESS)
		     || (nKeyPadStatus[KeyMapIndex] == KEY_REPEAT) )
		{
			KEYBRD_MESSAGE KeyBrdMsg;			
			
			nKeyPadStatus[KeyMapIndex] = KEY_REPEAT;
			KeyBrdMsg.nMsgType = KEY_REPEAT;
			KeyBrdMsg.nKeyCode = nKeyPadMap[KeyMapIndex].nMMIKeyCode;			
			KeyEventHandler((struct KEYBRD_MESSAGE *)&KeyBrdMsg);			
		}
		else
		{
			//Ignore the event
		}
	}
	else if(MsgType == DRV_WM_KEYFULLPRESS)
	{
		if ( (nKeyPadStatus[KeyMapIndex] == KEY_EVENT_DOWN)
			|| (nKeyPadStatus[KeyMapIndex] == KEY_LONG_PRESS)
			|| (nKeyPadStatus[KeyMapIndex] == KEY_REPEAT) )
		{
			KEYBRD_MESSAGE KeyBrdMsg;			
			
			nKeyPadStatus[KeyMapIndex] = KEY_FULL_PRESS;
			KeyBrdMsg.nMsgType = KEY_FULL_PRESS;
			KeyBrdMsg.nKeyCode = nKeyPadMap[KeyMapIndex].nMMIKeyCode;			
			KeyEventHandler((struct KEYBRD_MESSAGE *)&KeyBrdMsg);			
		}
		else
		{
			//Ignore the event
		}
	}
  #endif /* MMI_ON_HARDWARE_P */
	else
	{
		MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_ERROR_PROC_KEYEVENT_HDLR));

		MMI_ASSERT(0);
	}

}

#ifdef MMI_ON_WIN32

#define CHECK_SIMPLY_HANDLE_MULTIPLEKEY(type, id)				\
	{															\
		static U16	lastId = 0xffff;							\
		if (type == WM_KEYPRESS)								\
		{														\
			if (lastId == 0xffff)								\
			{	/* update lastId */								\
				lastId = id;									\
			}													\
			else if (lastId != id)								\
			{	/* previous key isn't release but another key is pressed */	\
				/* release the previous key first*/				\
				/* then continue to press the current key */	\
				ProcessKeyEvent(WM_KEYRELEASE, lastId);			\
				lastKeyId = i;									\
			}													\
		}														\
		else	/* WM_KEYRELEASE */								\
		{	/* reset lastId */									\
			lastId = 0xffff;									\
		}														\
	}


/*****************************************************************************
* FUNCTION
*	ProcessPCKeyEvent
* DESCRIPTION
*	This function is to convert the W32 key to MMI keys.
* PARAMETERS
*	MsgType			IN		type of key event
*	KeyMsg			IN		key structure
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
U8 ProcessPCKeyEvent(U32 MsgType, KEYBRD_MESSAGE *KeyBrdMsg)
{

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 i;
	static U16 lastKeyId = 0xffff;	/* initial value (magic number) */
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if ((MsgType == WM_KEYBRD_PRESS) || (MsgType == WM_KEYBRD_RELEASE))
	{
        /* 
		 * handle the key event from the keyboard event
         * find keyCode in nHotKey of nKeyPadMap's table
         */
	        for (i=0; i < MAX_KEYPADMAP; i++)
		{
			if (KeyBrdMsg->nKeyCode == nKeyPadMap[i].nHotKey)
			{
				if (MsgType == WM_KEYBRD_PRESS)
				{
					MsgType = WM_KEYPRESS;
				}
				else
				{
					MsgType = WM_KEYRELEASE;
				}
				CHECK_SIMPLY_HANDLE_MULTIPLEKEY(MsgType, i)
				ProcessKeyEvent(MsgType, i);
				return MMI_TRUE;
			}
		}
	}
	else if ((MsgType == WM_KEYPRESS) || (MsgType == WM_KEYRELEASE))
	{
        /* 
		 * handle the key event from the mouse event
         * find keyCode in nKeyCode of nKeyPadMap's table
         */
		for (i = 0; i < MAX_KEYPADMAP; i++)
		{
			if (KeyBrdMsg->nKeyCode == nKeyPadMap[i].nKeyCode)
			{
				CHECK_SIMPLY_HANDLE_MULTIPLEKEY(MsgType, i);
				ProcessKeyEvent(MsgType,i); //Msg type press or release & keymap index
        		return MMI_TRUE;
			}
		}
	}
	//Not handle this message
	return MMI_FALSE;
}


#endif


/**************************************************************

	FUNCTION NAME		: KeyTimerExpiryProc

  	PURPOSE				: Timer expiry procedure 

	INPUT PARAMETERS	: void* handle to window

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
void KeyTimerExpiryProc(void *idEvent)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 i;
    MMI_BOOL    bNeedToDo = FALSE;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   /* Check the timer ID */
    if ( (KEY_TIMER_ID0 <= (U32)idEvent) && ((U32)idEvent < UI_TIMER_ID_BASE) )
    {
	   	for(i=0;i<MAX_KEYPADMAP;i++)
		{
        	if (((S32)idEvent) == nKeyPadMap[i].nTimerId)
        	{
        	    /* Check the keypad status. It should not be KEY_EVENT_UP */
           		if (nKeyPadStatus[i] == KEY_EVENT_DOWN)
           		{
           		    nKeyPadStatus[i] = KEY_LONG_PRESS;
           		    bNeedToDo = TRUE;
                }
           		else if ( (nKeyPadStatus[i] == KEY_LONG_PRESS) 
           		          || (nKeyPadStatus[i] == KEY_REPEAT) )
           		{
           		    nKeyPadStatus[i] = KEY_REPEAT;
           		    bNeedToDo = TRUE;
                }
           		/* stop the timer even the keypad status is KEY_EVENT_UP */
                 
                
                /* Only the */
           		if (bNeedToDo != FALSE)
       		    {
                    KEYBRD_MESSAGE KeyBrdMsg;

                    KeyBrdMsg.nMsgType = nKeyPadStatus[i];
                    KeyBrdMsg.nKeyCode = nKeyPadMap[i].nMMIKeyCode;

                    MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_PROC_TIMER_EVENT_HDLR, KeyBrdMsg.nMsgType, KeyBrdMsg.nKeyCode));

                    KeyEventHandler((struct KEYBRD_MESSAGE *)&KeyBrdMsg);
                     
       		    }
           		break;
           	}
		}
    }
    else
    {
        MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_ERROR_KEY_TIMER_EXPIRY_HDLR, (U32)idEvent));
        MMI_ASSERT(0);
    }
}


 
#ifdef MMI_ON_HARDWARE_P
/**************************************************************

	FUNCTION NAME		: InitKeypadBeforePWRON()

  	PURPOSE				: Initializes keypad before power on

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
void InitKeypadBeforePWRON(void) 
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	 
}

/**************************************************************

	FUNCTION NAME		: KeyHandleBeforePWRON()

  	PURPOSE				: This function detects key presses before power on

	INPUT PARAMETERS	: void *paraBuff

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
void KeyHandleBeforePWRON(void *paraBuff)
{
 
#if 0
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
 	mmi_eq_keypad_detect_ind_struct *p;
 	kbd_data k;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	p = (mmi_eq_keypad_detect_ind_struct *)paraBuff;	

   	while( (*(p->func))(&k) == KAL_TRUE );
#endif
 
}



#endif


extern U8 processUpRepeatKey[MAX_KEYS];

/**************************************************************

	FUNCTION NAME		: StopCurrentKeyPadTone

  	PURPOSE				: Stops Current keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/

void StopCurrentKeyPadTone(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(currKeyPadTone!=0xFFFF) 
	{
		AudioStopReq(currKeyPadTone);
		currKeyPadTone=0xFFFF;
	}
}

/**************************************************************

	FUNCTION NAME		: ClearKeyEvents

  	PURPOSE				: Clears Keyboard buffer

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
void ClearKeyEvents(void)
{
 
#if 0
#ifdef MMI_ON_HARDWARE_P
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	extern drv_get_key_func		keypad_ptr;
	kbd_data k;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_CLEAR_KEY_EVENTS_HDLR));
	
	if(keypad_ptr !=NULL)
	{
		while( (*(keypad_ptr))(&k) == KAL_TRUE)
		{
			/* restore state of keys */
			if (k.Keydata[0]!=KEY_INVALID)
				mmi_kbd_reset_key_state(k.Keyevent, k.Keydata[0]);
		}
	}
	processUpRepeatKey=0;
	StopCurrentKeyPadTone();

#ifdef __MMI_TOUCH_SCREEN__
	/* We disable pen on key down and enable pen on key up. 
	 * If key down and key up events are not paired, pen is not enabled again.
	 */
	mmi_pen_enable();
#endif

#endif
#endif
 
}

#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
S16 GetHumanVoiceToneID( S16 MMIKeyCode )
{
	S16 i;
	U8 keypad_tone_type = GetKeypadToneType();
	if( keypad_tone_type >= KEYPAD_TONE_HUMAN_VOICE_1 && keypad_tone_type <= KEYPAD_TONE_HUMAN_VOICE_6 )
	{
		if( !CanPlayHumanVoiceKeyTone() )
			return -1;
		if( MMIKeyCode >= KEY_0 && MMIKeyCode <= KEY_9 )
			i = MMIKeyCode - KEY_0;
		else if( MMIKeyCode == KEY_STAR )
			i = 10;
		else if( MMIKeyCode == KEY_POUND )
			i = 11;
		else
			i = -1;

		if( i >= 0 )
			return (S16) ((keypad_tone_type - KEYPAD_TONE_HUMAN_VOICE_1)*13 + i + MIN_HUMAN_VOICE_ID);
	}
	return -1;
}
#endif

#if defined(__MMI_CUST_KEYPAD_TONE__)
S16 GetCustKeyPadToneID( S16 MMIKeyCode )
{
	U8 keypad_tone_type = GetKeypadToneType();
	if( keypad_tone_type >= KEYPAD_TONE_CUST_1 && keypad_tone_type <= KEYPAD_TONE_CUST_6 )
	{
		if( !CanPlayCustKeyTone() )
			return -1;
		return (S16) (keypad_tone_type - KEYPAD_TONE_CUST_1 + MIN_KEYPAD_TONE_ID);
	}
	return -1;
}
#endif

U16 GetKeypadTone(S16 MMIKeyCode)
{
   U8 is_found = MMI_FALSE;
	S16	i;
#if defined(__MMI_CUST_KEYPAD_TONE__)
	if( (i = GetCustKeyPadToneID( MMIKeyCode )) >= 0 )
		return (U16) i;
#endif

#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
	 
	if( (i = GetHumanVoiceToneID( MMIKeyCode )) >= 0 )
		return (U16) i;
#endif

	for(i=0;i<MAX_KEYPADMAP;i++)
	{
	   	if (MMIKeyCode == nKeyPadMap[i].nMMIKeyCode)
	   	{
      		is_found = MMI_TRUE;
      		break;
      	}
	}
	
	MMI_ASSERT(is_found==MMI_TRUE);

	return (U16)nKeyPadMap[i].KeyPadTone;
}

U16 GetCurKeypadTone(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	return currKeyPadTone;
}

U8 SetCurKeypadTone(U16 KeyTone)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	currKeyPadTone = KeyTone;
	return MMI_TRUE;
}

U8 GetKeypadDuration(S16 MMIKeyCode)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   U8 is_found = MMI_FALSE;
   U16	i;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

#if defined(__MMI_CUST_KEYPAD_TONE__)
	if( GetCustKeyPadToneID( MMIKeyCode ) >= 0 )
		return DEVICE_AUDIO_PLAY_ONCE;
#endif

#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
	 
	if( GetHumanVoiceToneID( MMIKeyCode ) >= 0 )
		return DEVICE_AUDIO_PLAY_ONCE;
#endif
   
	for(i=0;i<MAX_KEYPADMAP;i++)
	{
	   	if (MMIKeyCode == nKeyPadMap[i].nMMIKeyCode)
	   	{
      		is_found = MMI_TRUE;
      		break;
      	}
	}
	MMI_ASSERT(is_found==MMI_TRUE);

	//return KeyToneMap[KeyCode].KeyPadDuration;
	return nKeyPadMap[i].KeyPadDuration;
}


/*****************************************************************************
* FUNCTION
*	mmi_kbd_reset_key_state
* DESCRIPTION
*	This function set the keypad state according corrent key event. 
*	But keypad handler is not executed.
* PARAMETERS
*	MsgType			IN		type of key event
*	KeyMapIndex		IN		key index
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void mmi_kbd_reset_key_state(U32 MsgType, U16 KeyMapIndex)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(MsgType == WM_KEYPRESS)
		nKeyPadStatus[KeyMapIndex] = KEY_EVENT_DOWN;
	else if(MsgType == WM_KEYRELEASE)
		nKeyPadStatus[KeyMapIndex] = KEY_EVENT_UP;
	/* else 
		others won't change key state.
	*/
}


/*****************************************************************************
* FUNCTION
*	GetMaxPresentAllKeys
* DESCRIPTION
*	This function is to get all MMI keys.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
U8 GetMaxPresentAllKeys(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   return MAX_PRESENTALLKEYS;
}

#if defined(__MMI_BLACKBERRY_QWERTY__)
U8 GetMaxPresentAllKeys_QWERT(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   return MAX_PRESENTALLKEYS_QWERT;
}
#endif

/*****************************************************************************
* FUNCTION
*	GetMaxPresentDigitsKeys
* DESCRIPTION
*	This function is to get all MMI digits keys.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
U8 GetMaxPresentDigitsKeys(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   return MAX_PRESENTDIGITSKEYS;
}


/*****************************************************************************
* FUNCTION
*	mmi_frm_kbd_set_tone_state
* DESCRIPTION
*   This function is to chage the state of keypad tone. 
*   To prevent abnormal cases, keypad tone will be stopped before state changed.
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*   None
*****************************************************************************/
void mmi_frm_kbd_set_tone_state(mmi_frm_kbd_tone_state_enum state)
{
   StopCurrentKeyPadTone();
   frm_p->key_tone_state = state;
}


/*****************************************************************************
* FUNCTION
*   mmi_frm_kbd_get_key_tone_state
* DESCRIPTION
*   Get the state to see if keypad tone is disabled
* PARAMETERS
*   None
* RETURNS
*   state of keypad tone.
* GLOBALS AFFECTED
*   None
*****************************************************************************/
mmi_frm_kbd_tone_state_enum mmi_frm_kbd_get_key_tone_state(void)
{
   return frm_p->key_tone_state;
}


/*****************************************************************************
* FUNCTION
*   mmi_frm_kbd_reg_pre_key_hdlr
* DESCRIPTION
*   Regiester function to execute before general keypad handler.
* PARAMETERS
*   None
* RETURNS
*   state of keypad tone.
* GLOBALS AFFECTED
*   None
*****************************************************************************/
void mmi_frm_kbd_reg_pre_key_hdlr(PsKeyProcessCBPtr func)
{
   frm_p->kbd_pre_func = func;
}


/*****************************************************************************
* FUNCTION
*   mmi_frm_kbd_reg_post_key_hdlr
* DESCRIPTION
*   Regiester function to execute afer general keypad handler.
* PARAMETERS
*   None
* RETURNS
*   state of keypad tone.
* GLOBALS AFFECTED
*   None
*****************************************************************************/
void mmi_frm_kbd_reg_post_key_hdlr(PsKeyProcessCBPtr func)
{
   frm_p->kbd_post_func = func;
}


/*****************************************************************************
* FUNCTION
*   mmi_frm_kbd_is_tone_played
* DESCRIPTION
*   Function to check if keypad tone can be played or not
* PARAMETERS
*   key_code		IN		key code
*   key_type		IN		key type
* RETURNS
*   TRUE if the keypad tone shall be played by framework; otherwise, FALSE.
* GLOBALS AFFECTED
*   None
*****************************************************************************/
extern U16   gKeyPadLockFlag;
BOOL mmi_frm_kbd_is_tone_played(S16 key_code, S16 key_type)
{
   U8 tone_type = GetKeypadToneType();

   if(tone_type == KEYPAD_SILENT  				/* silent profile */
   	|| frm_p->key_tone_state == MMI_KEY_TONE_DISABLED 
   	||(tone_type == KEYPAD_TONE_HUMAN_VOICE_1 && gKeyPadLockFlag))
   {
      return FALSE;
   }
   else if(frm_p->key_tone_state == MMI_KEY_VOL_TONE_DISABLED)	
   {
      /* disable volume keypad tone */
      if(key_code == KEY_VOL_UP || key_code == KEY_VOL_DOWN)
      {
   		return FALSE;
      }
   }

   return TRUE;
}

/*****************************************************************************
* FUNCTION
*   mmi_frm_play_keypad_tone
* DESCRIPTION
*   Function to play corresponding keypad tone
* PARAMETERS
*   key_code		IN		key code
* RETURNS
*   None
* GLOBALS AFFECTED
*   None
*****************************************************************************/
void mmi_frm_play_keypad_tone(S16 key_code)
{
	//TRACE_SYS_FUNCTION();
   U8 tone_type = GetKeypadToneType();

   TRACE_EF(g_sw_SYSTEM, "tone_type = %d", tone_type);
   if( tone_type == KEYPAD_CLICK )
	{
		AudioPlayReq(TONE_KEY_CLICK, DEVICE_AUDIO_PLAY_ONCE );
	}
	else
	{
		SetCurKeypadTone( GetKeypadTone( key_code ) );
		frm_p->key_tone_id = GetCurKeypadTone();
		frm_p->key_tone_duration = GetKeypadDuration( key_code );
		AudioPlayReq( frm_p->key_tone_id, frm_p->key_tone_duration );
	}
}

/*****************************************************************************
* FUNCTION
*   mmi_frm_stop_keypad_tone
* DESCRIPTION
*   Function to stop corresponding keypad tone
* PARAMETERS
*   key_code		IN		key code
* RETURNS
*   None
* GLOBALS AFFECTED
*   None
*****************************************************************************/
void mmi_frm_stop_keypad_tone(S16 key_code)
{
   U8 tone_type = GetKeypadToneType();
   if( tone_type == KEYPAD_TONE )
   {
      SetCurKeypadTone( 0xFFFF );
      if( GetKeypadDuration( key_code ) == DEVICE_AUDIO_PLAY_INFINITE )
      {
         AudioStopReq( GetKeypadTone( key_code ) );
      }
   }
#if defined( __MMI_CUST_KEYPAD_TONE__ )
   else if( tone_type >= KEYPAD_TONE_CUST_1 && tone_type <= KEYPAD_TONE_CUST_6
#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
		&& frm_p->key_tone_id < MIN_HUMAN_VOICE_ID ) // MIN_KEYPAD_TONE_ID > MIN_HUMAN_VOICE_ID
#else
		&& frm_p->key_tone_id < MIN_KEYPAD_TONE_ID )
#endif
			{
				SetCurKeypadTone( 0xFFFF );
				if( frm_p->key_tone_duration == DEVICE_AUDIO_PLAY_INFINITE )
				{
					AudioStopReq( frm_p->key_tone_id );
				}
			}
#endif
#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
			else if( tone_type >= KEYPAD_TONE_HUMAN_VOICE_1 && tone_type <= KEYPAD_TONE_HUMAN_VOICE_6
				&& frm_p->key_tone_id < MIN_HUMAN_VOICE_ID ) // MIN_KEYPAD_TONE_ID > MIN_HUMAN_VOICE_ID
			{
				SetCurKeypadTone( 0xFFFF );
				if( frm_p->key_tone_duration == DEVICE_AUDIO_PLAY_INFINITE )
				{
					AudioStopReq( frm_p->key_tone_id );
				}
			}
#endif
}

/*****************************************************************************
* FUNCTION
*   mmi_kbn_key_tone_hdlr
* DESCRIPTION
*   Function to handle general keypad tones
* PARAMETERS
*   eventKey		IN		key event
* RETURNS
*  TRUE if general key tone plays; otherwise, FALSE.
* GLOBALS AFFECTED
*   None
*****************************************************************************/
U8 mmi_kbn_key_tone_hdlr(KEYBRD_MESSAGE *eventKey)
{
	U8 keypad_tone_type ;
	//CSD added by JL 031220 for mute microphone to avoid the conflict with speaker and cause the DTMF frequency error
	PRINT_INFORMATION_2((MMI_TRACE_G1_FRM, "key event <<code %d, type %d>> \n",eventKey->nKeyCode, eventKey->nMsgType));
	if(eventKey->nKeyCode==KEY_INVALID) 
		return MMI_TRUE;

/* To Play Tones */

	keypad_tone_type = GetKeypadToneType();

	if(mmi_frm_kbd_is_tone_played(eventKey->nKeyCode, eventKey->nMsgType))
	{
		if(eventKey->nMsgType==KEY_EVENT_DOWN)
		{
		   mmi_frm_play_keypad_tone( eventKey->nKeyCode );

			StopTimer(TIMER_KEYPAD_BACKLIGHT);
		}		
		else if(eventKey->nMsgType==KEY_EVENT_UP)
		{
		   mmi_frm_stop_keypad_tone( eventKey->nKeyCode );
		}
#ifdef MMI_ON_WIN32
	{
		if(((S16)eventKey->nMsgType == WM_KEYBRD_PRESS ||
			(S16)eventKey->nMsgType == WM_KEYBRD_RELEASE))
		{
			MMI_system_keybord_handler((S16)eventKey->nKeyCode,(S16)eventKey->nMsgType);
			return MMI_TRUE;
		}
	}
#endif /* 0 */

	}
		//move to Framework ExecuteCurrKeyHandler((S16)eventKey->nKeyCode,(S16)eventKey->nMsgType);
	//To continue to handler framework process, if false.
	return MMI_FALSE;
}


/*****************************************************************************
* FUNCTION
*   mmi_kbd_app_key_hdlr
* DESCRIPTION
*   Function to handle general appplication events
* PARAMETERS
*   key_code		IN		key code
* RETURNS
*  U8, if return true, Framework will not control the key, otherwse.
* GLOBALS AFFECTED
*   None
*****************************************************************************/
U8 mmi_kbd_app_key_hdlr(KEYBRD_MESSAGE *eventKey)
{
#if 0   
	#ifdef MMI_ON_HARDWARE_P
	{

		extern U8 gKeyPadLockFlag;
		extern void HalfModeBacklight(void);

		 
		#if (0)
		#ifdef __MMI_CLAMSHELL__
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
/* under construction !*/
		#endif /* __MMI_CLAMSHELL__ */
		#endif
		 

		if( (gKeyPadLockFlag && g_idle_context.IsOnIdleScreen) \
			||(gKeyPadLockFlag && g_idle_context.ScreenSaverRunFlag) )
			HalfModeBacklight();
		else
		     TurnOnBacklight(1);  


	   if( IsMyTimerExist(KEYPAD_LOCK_TIMER) == MMI_TRUE )
	   {
	      #ifdef __MMI_KEYPAD_LOCK_PATTERN_1__

	      if( (S16)eventKey->nKeyCode != KEY_POUND  )
	      {
	        //StopMyTimer( KEYPAD_LOCK_TIMER );
	        IdleHandleKeypadLockProcess();
	      }

	      #endif
	      
	      #ifdef __MMI_KEYPAD_LOCK_PATTERN_2__

	      if( (S16)eventKey->nKeyCode != KEY_STAR  )
	      {
	         //StopMyTimer( KEYPAD_LOCK_TIMER );
	         IdleHandleKeypadLockProcess();
	      }

	      #endif
	   }
	}
	#else
	{
		if(g_idle_context.IsOnIdleScreen)
	     		TurnOnBacklight(1);
	}
	#endif


	return mmi_kbn_key_tone_hdlr(eventKey);
#endif
	return 1;
}


