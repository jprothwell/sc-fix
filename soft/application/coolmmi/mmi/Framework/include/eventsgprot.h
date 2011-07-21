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
 *	EventsGprot.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Events gprot.
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

#ifndef _EVENTSGPROT_H
#define _EVENTSGPROT_H

#include "frameworkstruct.h"
#include "unicodexdcl.h"
#include "globalconstants.h"

/***************************************************************************** 
* Define
*****************************************************************************/
#define IsBitSet(Word, bit) 		((Word) & (1<<(bit)))
#define IsBitReset(Word, bit)		(!((Word) & (1<<(bit))))
#define SetBit(Word, bit) 			(Word)|=(1<<(bit))
#define ResetBit(Word, bit) 		(Word) &= ~(1<<(bit))

#define MMI_DEVICE_KEY 0x0001
#define MMI_DEVICE_PEN 0x0002
#define MMI_DEVICE_ALL (MMI_DEVICE_KEY | MMI_DEVICE_PEN)

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/
extern U8 hintData[MAX_SUB_MENUS][MAX_SUB_MENU_HINT_SIZE];
extern U8 subMenuData[MAX_SUB_MENUS][MAX_SUB_MENU_SIZE];
extern PU8 subMenuDataPtrs[MAX_SUB_MENUS];
extern PU8 hintDataPtrs[MAX_SUB_MENUS];

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
extern void InitEvents(void);															/* initilize events func */ 
extern void ExecuteCurrKeyHandler(S16 keyCode, S16 keyType);		/* execute current key func handler */
extern void ExecuteCurrProtocolHandler(U32 eventID,void* MsgStruct,int mod_src, void *peerBuf);	/* execute current protocol func handler */
extern U8 EntryNewScreen(U16 newscrnID, FuncPtr newExitHandler, FuncPtr newEntryHandler, void  *peerBuf); /* execute current entry func handler */
extern U8 EntryNewScreen_Ext(U16 newscrnID, FuncPtr newExitHandler, FuncPtr newEntryHandler, void  *peerBuf); /* execute current entry func handler without clear all keyhandler */
extern void ReplaceNewScreenHandler(U16 scrnID, FuncPtr exitFuncPtr, FuncPtr entryFuncPtr);		/* execute replace current screen screen id, exit and entry func handler */	
extern U16 GetActiveScreenId (void);												/*  Get current active screen ID */
extern void ExecuteCurrSubLcdExitHandler(void);
extern void ExecuteCurrHiliteHandler(S32 hiliteid);							/*  execute current hilite handler with arrow key clear */
extern void ExecuteCurrHiliteHandler_Ext(S32 hiliteid);					/*  execute current hilite handler without any key clear */
extern void SetKeyHandler(FuncPtr funcPtr, U16 keyCode, U16 keyType);	/* key event handlers */
extern FuncPtr GetKeyHandler(U16 keyCode, U16 keyType);
extern void SetGroupKeyHandler(FuncPtr funcPtr, PU16 keyCodes, U8 len,U16 keyType);	/* helper to register the same key handler for a group of key codes */
extern void ClearKeyHandler(U16 keyCode, U16 keyType);
extern void ClearOneKeyAllHandler(U16 keyCode);
extern void ClearAllKeyHandler(void);
extern void ClearInputEventHandler(U16 device);
extern void PowerAndEndKeyHandler(void);									/* power key & end key handler */
extern void SetProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID);
extern void ClearProtocolEventHandler(U32 eventID);
extern void ClearAllProtocolEventHandler(void);

#ifdef __MMI_MULTI_SIM__
extern void SetSlaveProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID);
extern void ClearSlaveProtocolEventHandler(U32 eventID);
extern void ClearSlaveAllProtocolEventHandler(void);
#endif

extern void SetProtocolEventHandlerExt(PsFuncPtr funcPtr, U32 eventID, U8 nSimID);
extern void ClearProtocolEventHandlerExt(U32 eventID, U8 nSimID);
extern void ClearAllProtocolEventHandlerExt(void);

extern void SetEntryHandler(U16 scrnID, FuncPtr entryFuncPtr);				/* entry function handler */
extern void ClearEntryHandler(void);
extern U16 GetExitScrnID(void);
extern void SetSubLcdExitHandler(FuncPtr exitSubLcdFuncPtr);
extern void ClearExitHandler(void);
extern void ClearSubLcdExitHandler(void);
extern void SetHiliteHandler(U16 itemid , FuncPtr hiliteFuncPtr );				/*  set hilite handler & parent id */
extern void ClearHiliteHandler(U16 itemid);
extern void ClearAllHiliteHandler(void);
extern void SetParentHandler(U16 parentid);
//CSD Brian added for AT+CVIB, 2003/11/23
extern U16 GetParentHandler(void);
extern void GetkeyInfo(U16 *keyCode, U16 *keyType);							/* gives key code & key type of last key event */
extern void SetkeyInfo(U16 keyCode, U16 keyType);
extern void StartTimer(U16 timerid, U32 delay, FuncPtr funcPtr);				/* this framework timer func for application */
extern void StopTimer(U16 timerid);
// lisen 03/27/2004
extern void DeInitFramework_KeyAndEventHandler(void);
extern void DeInitFramework_History(void);
extern void DeInitFramework(void);
extern U8 MaskItems(U16 *maskList, U8 noOfItems, U32 maskingWord);
extern U8 MaskItemsU8(U8 *maskList, U8 noOfItems, U32 maskingWord);
extern U8 MaskStringItems(U8 **maskList, U8 noOfItems, U32 maskingWord);
extern U8 MaskHiliteItems(U16 currParentID, U32 maskingWord);
extern U16 GetCurrHiliteID(void);
extern void ConstructHintsList(U16 currParentID, U8 **hintArray);
extern void SetHintHandler(U16 itemid, FuncPtrShort hintFuncPtr);
extern void ClearHintHandler(U16 itemid);
extern void GenericExitInlineScreen(U16 scrnID,  FuncPtr entryFuncPtr);
extern void GenericExitScreen(U16 scrnID,  FuncPtr entryFuncPtr);
extern void SetInterruptEventHandler(PsIntFuncPtr funcPtr, PsIntFuncPtr postfuncPtr, U32 eventID);
extern void SetGroupInterruptEventHandler(PsIntFuncPtr funcPtr, PsIntFuncPtr postfuncPtr, PU16 eventsID, U8 len);
extern void ClearInterruptEventHandler(U32 eventID);
extern void ClearAllInterruptEventHandler(void);
#ifdef __MMI_UI_TAB_PANE__
extern void SetCurrExitFuncPtr(FuncPtr ptr);
extern FuncPtr GetCurrExitFuncPtr(void);
extern void SetCurrEntryFuncPtr(FuncPtr ptr);
extern FuncPtr GetCurrEntryFuncPtr(void);
#endif

extern void ExecuteItemHiliteHandler(U16 hiliteItemID);

extern void MTPNP_AD_set_protocol_event_src_mode(U8 nIndex);
extern U8 MTPNP_AD_get_protocol_event_src_mode(void);


#endif /* _EVENTSGPROT_H */


