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
 *	ShortcutsProts.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines function prototypes of Shortcut application.
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

/**************************************************************

	FILENAME	: ShortcutsProts.h

  	PURPOSE		: Shortcuts application 

 

 

	DATE		: May 12,03

**************************************************************/
#ifndef _MMI_SHORTCUTSPROTS_H
#define _MMI_SHORTCUTSPROTS_H
#include "mmi_data_types.h"
#include "frameworkstruct.h"

/* 
** Define
*/

/* 
** Typedef 
*/

/* 
** Extern Global Variable
*/

/* 
** Extern Global Function
*/
extern void ShctInit(void);				//Added by Jinzh:2007-5-14
extern void EntryShctInIdle(void);
extern void EntryShctInMainMenu(void);
BOOL ATSetShortcuts(U8 *index);

extern void ShctGetCandidateList(void);
extern void ShctReadFromNvram(void);
extern void ShctWriteToNvram(void);
extern void HighlightShctSelShortcut(S32 index);
extern void ShctExecSelectedShortcut(void);

extern void ShortcutsMenuIndex(S32);
extern void EntryShctEditScreen(void);
extern void ExitEditShortcuts(void);
extern void HighlightShctEdit(S32 index);
extern void ShctSaveChanges(void);
extern void ShctMenuShortcut(S32 index);
extern U16 ShctGetStringID(U16 menu_id);

#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
extern void ShctReadDedicatedListFromNVRAM(void);
extern void ShctWriteDedicatedListToNVRAM(void);
extern void EntryShctArrowKeyList(void);
extern void ExitDedicatedKeyScreen(void);
extern void HighlightDedicatedKeys(void);
extern void HighlightShctSelectedKey(S32 nIndex);
extern void HighlightDedicatedKeys(void);
extern void EntryShctEditDedicatedKey(void);
extern void ShctSaveDedicated(void);
extern void RegisterDedicatedKeyHandlers(void);
#endif



#endif /* _MMI_SHORTCUTSPROTS_H */


