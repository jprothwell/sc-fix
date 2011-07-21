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
 *	HistoryGprot.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for history functions Prototypes.
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _PIXCOM_HISTORYGPROT_H
#define _PIXCOM_HISTORYGPROT_H

#include "frameworkstruct.h"

/***************************************************************************** 
* Define
*****************************************************************************/

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
extern void InitHistory(MainLcdHistoryCBPtr main_root_ptr, SubLcdHistoryCBPtr sub_root_ptr);
extern void HistoryDump(void);
extern pBOOL HistoryReplace(U16 out_scrn_id, U16 in_scrn_id, FuncPtr in_src_func);

#ifdef OSL_MEMORY_DUMP
	void AddHistoryReferenceMemoryRecord(history_t *addHistory, char* fileName, int lineNumber);
	#define AddHistory(addHistory) AddHistoryReferenceMemoryRecord(&(addHistory), __FILE__, __LINE__)
#else
	void AddHistoryReference(history_t *addHistory);
	#define AddHistory(addHistory) AddHistoryReference(&(addHistory))

	void InsertHistoryBeforeThisScrnReference (U16 scrnId, history_t *addHistory);
	#define InsertHistoryBeforeThisScrn(scrnId, addHistory) InsertHistoryBeforeThisScrnReference(scrnId, &(addHistory))
#endif

extern void DeleteHistory(U16 scrnID);
extern U8  GetHistory(U16 scrnID,history_t *ptrHistory);
extern U8  GetHistoryScrID(U16 scrnID, historyNode **ptrHistory);
extern pBOOL CheckIsBackHistory(void);
extern void GoBackHistory(void);
extern U8 GoBackToHistory(U16 scrnid);
extern void GoBacknHistory(U16 nHistory);
extern U8* GetCurrGuiBuffer(U16 scrnid);								/* get current screen history */
extern U8* GetCurrInputBuffer(U16 scrnid);								
extern void DeleteNHistory(U16 DeleteCount);
extern void ExecutecurrHisIndEntryFunc(void);
extern U8 GoBeyondMarkerScr(U16 scrnid);
extern U8 DeleteBeyondScrTillScr(U16 beyondScrnid,U16 tillScrnid);
extern U8 DeleteUptoScrID(U16 scrnid);
extern U8 DeleteNScrId(U16 scrnid);
extern U16 DeleteScreenIfPresent(U16 ScrId);
extern pBOOL IsScreenPresent(U16 scrnId);
extern pBOOL IsMainLCDHistoryBack(void);
extern U8 IsCurrMarkerScreen(U16 scrnid);
extern void DinitHistory(void);
extern void DisplayIdleScreen(void);
extern U8 ExecuteRootMainHistoryScreen(void *funcPtr);    /* execute main lcd history callback function and root history function */
extern U8  GetNHistory(U16 scrnID, history_t *ptrHistory);
 
extern void AddNHistory_ext(history_t *addHistory,U16 size);
#define AddNHistory(addHistory,size)	AddNHistory_ext(&(addHistory),size)
 
extern U8* GetCurrNInputBuffer(U16 scrnid,U16 *size);
extern S16 GetScreenCountInHistory(void);
extern pBOOL GetPreviousScrnIdOf (U16 scrnId, U16* previousScrnId); 
extern pBOOL GetNextScrnIdOf (U16 scrnId, U16* nextScrnId);

extern U16 GetCurrScrnId(void);

extern U16 DeleteBetweenScreen(U16 StartScrId, U16 EndScrId);
extern U16 DeleteBetweenScreens(U16 StartScrId, U16 EndScrId);
extern U8 SetDelScrnIDCallbackHandler(U16 ScrnID, HistoryDelCBPtr funcPtr);
void set_small_screen(void);//__MMI_UI_SMALL_SCREEN_SUPPORT__
void reset_small_screen(void);
 
void force_no_small_screen(void);
 
 
void enable_check_for_small_screen(void);
void disable_check_for_small_screen(void);
 
#ifdef __MMI_UI_TAB_PANE__
void disable_history_saving(void);
void enable_history_saving(void);
void change_curr_gui_buffer(U8 *gui_buffer);
#endif

 
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
void set_redrawing_old_screens(void);
void reset_redrawing_old_screens(void);
S32 is_redrawing_old_screens();
#endif

U8 FreeHistory(void *funcPtr);

#endif /* _PIXCOM_HISTORYGPROT_H */

