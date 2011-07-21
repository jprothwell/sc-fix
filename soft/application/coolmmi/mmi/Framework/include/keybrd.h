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
 *	MMI_features.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for MMI keypad config function.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef _KEYBRD_H_
#define _KEYBRD_H_
#include "cswtype.h"
#include "queuegprot.h"
#include "frameworkstruct.h"

/***************************************************************************** 
* Define
*****************************************************************************/
#define TOTAL_KEYS GetMaxPresentAllKeys()//JL 040223 for 3 extra keys. 22

#if defined(__MMI_BLACKBERRY_QWERTY__)
#define TOTAL_KEYS_QWERT GetMaxPresentAllKeys_QWERT()	
#endif
#define KEYTIMER_LONGPRESS			2000 //Key Long Pres Duration
#define KEYTIMER_REPEAT				1000 //Key Reapeat Duration

/***************************************************************************** 
* Typedef 
*****************************************************************************/

typedef struct
{
	S16 nKeyCode;//From Key Pad Driver
	S16 nMMIKeyCode; //Key code mapped from MMI perspective
	S16 nTimerId;
	//S16 nKeyState;//Pressed,Released or idle
	U8 KeyPadTone;
	U8 KeyPadDuration;
	#ifdef MMI_ON_WIN32
	S16 nHotKey; /* Mapping to keyboard in PC simulator */
	#endif
}KeyPadMap;

// zrx add begin 20060621
typedef struct
{
	S16 nKeyCode;//From Key Pad Driver
	S16 nMMIKeyCode; //Key code mapped from MMI perspective
}KeyPadMapSlim;
// zrx add end

typedef struct
{
	S16 nKeyState;
}KeyPadState;


/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/
extern const U16 PresentAllKeys[];

#ifdef __MMI_BLACKBERRY_QWERTY__
extern const U16 PresentAllKeys_qwert[];
#endif
/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
extern void KeyTimerExpiryProc(void  *idEvent);
extern void ProcessKeyEvent(U32 MsgType, U16 KeyMapIndex);
extern U8 ProcessPCKeyEvent(U32 MsgType, KEYBRD_MESSAGE *KeyBrdMsg);
 
extern void InitProcessKeyEvent(void);
extern void ProcessKeyBoardEvent(U32 MsgType, U16 KeyCode, U16 KeyMapIndex);
extern void KeyHandler(void *paraBuff);
extern void InitKeypadBeforePWRON(void);
extern void KeyHandleBeforePWRON(void *paraBuff);
extern void ClearKeyEvents(void);
extern S16 GetHumanVoiceToneID( S16 MMIKeyCode );
extern U16 GetCurKeypadTone(void);
extern U16 GetKeypadTone(S16 KeyCode);
extern U8 SetCurKeypadTone(U16 KeyTone);
extern U8 GetKeypadDuration(S16 KeyCode);
extern void StopCurrentKeyPadTone(void);
extern U8 GetMaxPresentAllKeys(void);
extern U8 GetMaxPresentDigitsKeys(void);

extern void mmi_kbd_reset_key_state(U32 MsgType, U16 KeyMapIndex);
extern void mmi_frm_kbd_set_tone_state(mmi_frm_kbd_tone_state_enum state);
extern mmi_frm_kbd_tone_state_enum mmi_frm_kbd_get_key_tone_state(void);
extern BOOL mmi_frm_kbd_is_tone_played(S16 key_code, S16 key_type);
extern void mmi_frm_kbd_reg_pre_key_hdlr(PsKeyProcessCBPtr func);
extern void mmi_frm_kbd_reg_post_key_hdlr(PsKeyProcessCBPtr func);
extern void mmi_frm_stop_keypad_tone(S16 key_code);
extern void mmi_frm_play_keypad_tone(S16 key_code);
extern U8 mmi_kbd_app_key_hdlr(KEYBRD_MESSAGE *eventKey);

extern void mmi_frm_set_key_handle_in_high_frequency(MMI_BOOL is_enable);

#endif


