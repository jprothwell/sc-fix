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
 *  CommonScreens.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
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

/**************************************************************

	FILENAME	: CommonScreens.h

  	PURPOSE		: Common screens used by applications

 


	DATE		: Aug-06-2003

**************************************************************/

 
#include "mmi_features.h" 
//CSD End
#include "globalconstants.h"
#include "globaldefs.h"
#include "custmenures.h"
#include "custdatares.h"
#include "frameworkstruct.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "unicodexdcl.h"
#include "idleappdef.h"
#include "commonscreensenum.h"
#include "screenrotationgprot.h"

#ifndef __COMMONSCREENS_H__
#define __COMMONSCREENS_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*  POPUP behavior defined by mask */
#define  POPUP_ALL_OFF	0x00
#define  POPUP_DSPL_ON	0x01
#define  POPUP_TONE_ON	0x02
#define  POPUP_ALL_ON	0xFF


/*  POPUP screen attribute  */
#define UI_POPUP_NOTIFYDURATION_TIME				1000
#define UI_POPUP_NOTIFYDURATION_TIME_2000			2000


 
void EntryQuickSearchInputMethodScreen(void);
void ExitQuickSearchInputMethodScreen(void);

void EntryInputMethodScreen(void);
void ExitInputMethodScreen(void);
void EntryInLineMultilineEditorOption(void);
void ExitInLineMultilineEditorOption(void);
void RegisterInputMethodScreenCloseFunction(void (*f)(void));
void ConfirmInlineFullScreenEdit(void);
void CancelInlineFullScreenEdit(void);

void EntrySymbolPickerScreen(void);
void ExitSymbolPickerScreen(void);
void SymbolPickerHandleLSKFromMenuList(void);
void SymbolPickerHandleRSKFromMenuList(void);
void EntrySymbolPickerScreenFromMenuList(void);
void ExitSymbolPickerScreenFromMenuList(void);
void SetInputMethodAndDoneCaptionIcon( U16 IconId );
void EntryScrForInputMethodAndDone(void);
void ExitScrForInputMethodAndDone(void);
void PrepareSymbolTableAndShowCategory(U8* guiBuffer);

extern const U16 gIndexIconsImageList[];  
void DisplayPopup(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId);
#ifdef __MMI_SCREEN_ROTATE__
void DisplayPopupRotated(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId, mmi_frm_screen_rotate_enum rotation);
#endif
void ExitDisplayPopup(void);
void DisplayPopupCallBack(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId,FuncPtr f);
void ExitDisplayPopupCallBack(void);
void PopupTimerFlagEnable(void);
void PopupCloseByPressAnyKey(void);
void PopupTimerOver(void);
void PopupTimerOverCallBack(void);
//#ifndef MMI_ON_HARDWARE_P
void PopupNoSupport(void);
//#endif /* #ifndef MMI_ON_HARDWARE_P */
void DisplayConfirm(U16 LSK_str, U16  LSK_img, U16  RSK_str, U16  RSK_img, UI_string_type message, UI_image_ID_type message_image, U8 toneId);
#ifdef __MMI_SCREEN_ROTATE__
void DisplayConfirmRotated(U16 LSK_str, U16  LSK_img, U16  RSK_str, U16  RSK_img, UI_string_type message, UI_image_ID_type message_image, U8 toneId, mmi_frm_screen_rotate_enum rotation);
#endif
void DisplayNewconfirm(U16 LSK_str, U16  LSK_img, U16  RSK_str, U16  RSK_img, UI_string_type message, UI_image_ID_type message_image, U8 toneId,FuncPtr func);
void DisplayConfirmCountDown(U16 LSK_str, U16  LSK_img, U16  RSK_str, U16  RSK_img, UI_string_type message, UI_image_ID_type message_image, U8 toneId, U32 duration, FuncPtr callback);
void ExitDisplayPopupConfirm(void);
void mmi_frm_set_highlight_handler (void);
void mmi_show_in_progress(U16 msg, U16 msgIcon);
extern void DisplaySubLcdPopup(U16 imageId);
extern void SetInterruptPopupDisplay(U8 flag); // To set the popup screen is allow to display or not.
extern U8 GetInterruptPopupDisplay(void); // To get the popup screen is allow to display or not.

#ifdef APPMEM_SUPPORT
extern void get_clean_list_str(S32 size, S32 string_id, U8** mem_full_string);
extern void mmi_asm_show_memory_full_dialog(S32 size, S32 string_id, int mod_type);
#endif
extern void mmi_ime_delete_editor_common_scr(U16 scr_id);

#ifdef __cplusplus
}
#endif

#endif


