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
 *  wgui_categories_CM.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Call Management related categories.
 *  Author:
 * -------
 
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef WGUI_CATEGORIES_CM_H
#define WGUI_CATEGORIES_CM_H

/***************************************************************************** 
 * Include
 *****************************************************************************/
#include "gui_tab_bars.h"

/***************************************************************************** 
 * Define
 *****************************************************************************/
#define DURATION_CM_HEIGHT          15
#define SHOW_DURATION               1
#define NO_DURATION                 0
#define UI_MAX_TAB_BAR_ITEMS                    20
extern tab_bar_item_type MMI_tab_bar_items[UI_MAX_TAB_BAR_ITEMS];



/***************************************************************************** 
 * Typedef
 *****************************************************************************/
typedef enum
{
    WGUI_CATE_MOMT_RES_TYPE_IMAGE_ID = 0,
    WGUI_CATE_MOMT_RES_TYPE_VIDEO_ID,
    WGUI_CATE_MOMT_RES_TYPE_SWFLASH_ID,
    WGUI_CATE_MOMT_RES_TYPE_IMAGE_FILE,
    WGUI_CATE_MOMT_RES_TYPE_VIDEO_FILE,
    WGUI_CATE_MOMT_RES_TYPE_SWFLASH_FILE
} wgui_cate_momt_res_type_enum;


/***************************************************************************** 
 * Global Function
 *****************************************************************************/

/* Category 16 */
extern void RedrawCategory16Screen(void);
extern void ShowCategory16Screen(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *Buffer,
                U32 BufferLength,
                U8 *history_buffer);
extern void ExitCategory16Screen(void);
extern U8 *GetCategory16History(U8 *history_buffer);
extern S32 GetCategory16HistorySize(void);
void SetCategory16RightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);
extern void RegisterCategory16NavigationKeys(void);

/* Category 17 */
extern void ShowCategory17Screen(
                U16 title_id,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U16 NotificationStringId,
                PU8 NameOrNumber,
                PU8 IP_Number,
                U16 default_image_id,
                U16 resource_id,
                PS8 resource_filename,
                wgui_cate_momt_res_type_enum resource_type,
                U16 repeat_count,
                BOOL is_visaul_update,
                BOOL is_aud_enable,
                BOOL is_play_aud_when_start,
                PU8 history_buffer);

extern void RedrawCategory17Screen(void);
extern void ExitCategory17Screen(void);

extern void StopCategory17Video(void);
extern void DisableCategory17Audio(void);
extern void EnableCategory17Audio(void);
extern void DisableCategory17VideoUpdate(void);
extern void EnableCategory17VideoUpdate(void);
extern void MOMTHideAnimation(void);
extern MMI_BOOL IsCategory17VideoValid(void);   /* call this function after ShowCategory17Screen */
extern MMI_BOOL IsCategory17SWFlashValid(void);  /* call this function after ShowCategory17Screen */

/* Category 18 */
extern void ShowCategory18Screen(
                U16 title_id,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U16 NotificationStringId,
                PU8 NameOrNumber,
                PU8 IP_Number,
                U16 image_id,
                PS8 image_filename,
                MMI_BOOL isDisconnecting,
                PU8 history_buffer);

/* Category 19 */
extern void ShowCategory19Screen(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                S32 number_of_calls,
                U8 **CallList,
                U16 *CallStates,
                MYTIME *CurrCallTime,
                S32 highlighted_item,
                U8 *history_buffer);
extern void ExitCategory19Screen(void);
extern S32 GetCategory19HistorySize(void);
extern U8 *GetCategory19History(U8 *history_buffer);
extern void RedrawCategory19Screen(void);

/* Category 20 */
extern void ShowCategory20Screen(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *NameOrNumber,
                MYTIME *CurrCallTime,
                U16 CallState,
                MMI_BOOL isDisconnecting,
                U8 *history_buffer);

extern void ExitCategory20Screen(void);
extern void RedrawCategory20Screen(void);
extern S32 GetCategory20HistorySize(void);
extern U8 *GetCategory20History(U8 *history_buffer);

/* Category 308 */
extern void ShowCategory308Screen(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *NameOrNumber,
                U16 bkg_image,
                U16 call_image,
                MYTIME *CurrCallTime,
                U8 *history_buffer,
                U8 show_duration);

extern void ExitCategory308Screen(void);
extern U8 *GetCategory308History(U8 *history_buffer);
extern S32 GetCategory308HistorySize(void);
extern void RedrawCategory308Screen(void);

/* Category 309 */
extern void ExitCategory309Screen(void);
extern U8 *GetCategory309History(U8 *history_buffer);
extern S32 GetCategory309HistorySize(void);
extern void RedrawCategory309Screen(void);
extern void ChangeCategory309Status(U8 index, U8 value);
extern void Show_MainImages_Category309(void);
extern void ShowCategory309Screen(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                U8 *NumberorName,
                U16 bkg_image,
                MYTIME *CurrCallTime,
                U8 *status_calls,
                U8 *history_buffer);

#ifdef __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__
void SetDialingKeypadCallHandler(void (*handler) (void));
void SetDialingKeypadPhonebookHandler(void (*handler) (void));
#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
void SetDialingKeypadCall2Handler(void (*handler) (void));
void mmi_idle_sim1_dialpad_call(void);
void mmi_idle_sim2_dialpad_call(void);
void mmi_idle_sim3_dialpad_call(void);
void mmi_idle_sim4_dialpad_call(void);
#endif /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */
#endif /* __MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__ */ 

extern void ShowCategory403Screen(
                U8 *title,
                U16 title_icon,
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                S8 n_tabs,
                S8 highlighted_tab,
                tab_bar_item_type *tab_items,
                MYTIME *CallDuration,
                U8 **list_of_items,
                U16 *list_of_icons,
                S32 number_of_items,
                U16 highlighted_item,
                U8 *history_buffer);

extern void UpdateCategory403TabBar(
                UI_string_type title,
                S8 n_tabs,
                S8 highlighted_tab,
                tab_bar_item_type *tab_items);

extern void Category403UnBlinkTab(S8 tab);

extern void Category403BlinkTab(S8 tab);

void SetCategory403TabSelectCallback(void (*select_callback) (int index));

extern void ShowCategory404Screen(
            U16 left_softkey,
            U16 left_softkey_icon,
            U16 right_softkey,
            U16 right_softkey_icon,
            U16 bg_image,
            U8 *Buffer,
            U32 BufferLength,
            U8 *history_buffer);

extern void wgui_set_RSK(void);

#if ( !(defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
extern void DrawSimulateCenterKey(void);
extern void SimulateCenterKeyDailFunction(void (*f)(void),MMI_key_event_type k);
extern void ClearSimulateCenterKeyDailhandlers(void);
extern void SetNeedCenterKeyFlag(MMI_BOOL flag);
#endif

#endif /* WGUI_CATEGORIES_CM_H */ 

