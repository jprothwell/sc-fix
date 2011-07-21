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
 *	FrameworkStruct.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is defines prototypes, struct, defined marco for general MMI framework usage.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef _FRAMEWORKSTRUCT_H
#define _FRAMEWORKSTRUCT_H

#include "mmi_data_types.h"
#include "queuegprot.h"

/***************************************************************************** 
* Define
*****************************************************************************/
/* maximum history data	*/
 #ifdef JATAAYU_SUPPORT
#define MAX_INPUT_BUFFER			1500
#else
#define MAX_INPUT_BUFFER			1000
#endif
#define MAX_GUI_BUFFER				128

/***************************************************************************** 
* Typedef 
*****************************************************************************/
/* enum type for framework */
typedef enum
{
   MMI_KEY_TONE_ENABLED = 0,
   MMI_KEY_VOL_TONE_DISABLED,
   MMI_KEY_TONE_DISABLED  
} mmi_frm_kbd_tone_state_enum;

typedef struct KEYBRD_MESSAGE 
{
#ifdef MMI_ON_HARDWARE_P
	LOCAL_PARA_HDR
#else
	U8 ref_count;
	U16 msg_len;
#endif
	S16 nMsgType;
	S16 nKeyCode;
}KEYBRD_MESSAGE;


/* FuncPtr is a pointer to a func returns void */
typedef void (*FuncPtr)(void);   
typedef void (*FuncPtrShort)(U16); 


/* FuncPtr is a pointer to a func that takes void* has a parameter &  returns void */
typedef void (*PsFuncPtr)(void*); 
typedef U8 (*PsIntFuncPtr)(void*); 
/* JL added for new history callback */
typedef U8 (*HistoryDelCBPtr)(void*); 
typedef U8 (*SubLcdHistoryCBPtr)(void*); 
typedef U8 (*MainLcdHistoryCBPtr)(void*); 

typedef void (*PsFuncPtrU32)(void*,U32); 

typedef void (*PsExtFuncPtr)(void*,int); 		/* add by hjf, to pass the mod_src */
typedef U8 (*PsExtIntFuncPtr)(void*,int);		/* add by hjf, to pass the mod_src */

typedef void (*PsExtPeerFuncPtr)(void*, int, void*); 
typedef U8 (*PsExtPeerIntFuncPtr)(void*, int, void*); 


 
typedef U8 (*PsKeyProcessCBPtr)(KEYBRD_MESSAGE *); 

/* event information stuct */
typedef struct _eventInfo
{
	U32 eventID;										/* for timer & hardware events		*/
	FuncPtr entryFuncPtr; 
}eventInfo;

/* Async PRT event information struct */
typedef struct _PseventInfo
{
	U32 eventID;										/* for timer & hardware events		*/
	PsFuncPtr entryFuncPtr; 
}PseventInfo;

typedef struct _PIntseventInfo
{
	U32 eventID;										/* for timer & hardware events		*/
	PsIntFuncPtr entryIntFuncPtr; 
	PsIntFuncPtr postIntFuncPtr; 
}PsInteventInfo;

/* history information stuct */
typedef struct _history
{
	U16 scrnID;
	FuncPtr entryFuncPtr; 
	U8 inputBuffer[MAX_INPUT_BUFFER];			/* running text data							*/
	U8 guiBuffer[MAX_GUI_BUFFER];				/* this includes hilite item, input mode & etc..*/
}history_t;

/* history information stuct */
typedef struct _historyNode
{
	U16 scrnID;
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	U16 isSmallScreen;
#endif
	FuncPtr entryFuncPtr; 
	U8 *inputBuffer;									/* running text data							*/
	U8 *guiBuffer;									/* this includes hilite item, input mode & etc..*/
}historyNode;

/* history information stuct */
typedef struct _historyCallback
{
	U16 scrnID;
	HistoryDelCBPtr historydelCBPtr; 
}historyCallback;

/* hilite information stuct	*/
typedef struct _hiliteInfo
{
	FuncPtrShort hintFuncPtr;
	FuncPtr entryFuncPtr; 
}hiliteInfo;


typedef struct _SubLCDhistoryNode
{
	U16 scrnID;
	FuncPtr entryFuncPtr; 
}SubLCDHistoryNode;


/* mmi framework contact struct */
typedef struct
{
   mmi_frm_kbd_tone_state_enum   key_tone_state;   /* state of keypad tone */
   PsKeyProcessCBPtr              kbd_pre_func;    /* function to be executed before general keypad handler */
   PsKeyProcessCBPtr              kbd_post_func;   /* function to be executed after general keypad handler */
   U16                            key_tone_id;
   U8                             key_tone_duration;
}mmi_frm_context_struct;


/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/
extern mmi_frm_context_struct g_mmi_frm_cntx;

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/

#endif  /* _FRAMEWORKSTRUCT_H */


