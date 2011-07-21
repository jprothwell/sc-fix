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
 * Filename:
 * ---------
 *   gui_ems.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *   EMS editor - UI component
 * Author:
 * -------
 * -------
 * -------
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**********************************************************************************
   Filename:      gui_ems.c
   Date Created:  March-25-2003
   Contains:      EMS input box
**********************************************************************************/

#include "settingprofile.h"
#include "keybrd.h"

#include "gui_ems.h"
#include "gui_inputs.h"
#include "wgui_categories_defs.h"
#include "debuginitdef.h"
#include "wgui_categories_inputs.h"

#ifdef __MMI_HINDI_ALG__
#include  "hf_rules.h"
#include  "gui.h"
#endif /* __MMI_HINDI_ALG__ */ 

#include "mmi_features.h"
#include "timerevents.h"

#ifdef __MMI_LANG_VIETNAMESE__
#include "gui_lang_viet.h"
#endif 


#ifdef __MMI_ZI_ARABIC__
#ifdef __MMI_ZI_V7__
#include "ezicmn.h"
#endif 
#include "zi8arshape.h"
#endif /* __MMI_ZI_ARABIC__ */ 

/* for R2L characters */
/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-7 15:26 */
#include "bididef.h"
#include "bidiprot.h"
/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-7 15:26 */

/* for R2L characters */
extern BOOL r2lMMIFlag;

#ifdef __MMI_BIDI_ALG__
extern U8 visual_str[];
extern U8 MMI_bidi_input_type;
#endif /* __MMI_BIDI_ALG__ */ 

#ifdef __MMI_LANG_THAI__
#define isThaiChar(ch)  (((ch >= 0x0E01) && (ch <= 0x0E7F))? (1):(0))
#endif 

extern U16 StopMyTimer(U16 nTimerId);
extern U16 StartMyTimerInt(U16 nTimerId, U32 nTimeDuration, oslTimerFuncPtr TimerExpiry, U8 alignment);
extern U8 mmi_msg_get_seg_num(void);
extern void UI_set_current_text_color(color_t c);
extern S32 UI_cursor_x1;    /* cursor x1 postioon */
extern S32 UI_cursor_y1;    /* cursor y1 position */
extern void UI_draw_horizontal_line(S32 x1, S32 x2, S32 y, color_t c);

#if defined(__MMI_T9__)
extern void T9SynT9WithInputBoxForEMSInputBox(void);
#endif 


#ifdef __MMI_ARSHAPING_ENGINE__
//extern void ArabicShapeEngine(PU16, PU16);
#include "arabicgprot.h"
#endif 

#if defined( __MMI_ARSHAPING_ENGINE__)
extern U16 shaping_str[];
extern S32 unshaped_text_len;
extern U16 gui_get_ems_text_actual_idx(UI_EMS_input_box *b, U16 OffsetToText);
extern U16 OrgToSha[];
#endif /* __MMI_ARSHAPING_ENGINE__ */ 

/*wuzc Add Start For GDI/GUI 短信不能删除字体Portting Ver:    on 2008-4-16 17:48 */
extern	UI_EMS_input_box MMI_EMS_inputbox;
extern     UINT16 InsertEnter ;      
extern     UINT16 CursorPlace ;
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-4-16 17:48 */

#if(0)
#include "debuginitdef.h"
#endif /* (0) */ 

#ifdef __MMI_HINDI_ALG__
#define UI_CHK_CONSONANT(ch)  (( (ch >= 0x15 && ch <= 0x39) || (ch >= 0x58 && ch <= 0x5f) || ch==0x50 )? (1):(0))
#endif 

#define EMS_SHOW_HORIZONTAL_RULE_FOR_NEWLINE       0
 
#define EMS_SHOW_HORIZONTAL_RULE_FOR_ALL_LINES        1
 
#define EMS_ENABLE_EDITOR_OBJECT_PLAYBACK          0
#define EMS_PREVENT_CRLF_DISPLAY                1
#define EMS_USE_CRLF_COMBINATION                1
#define EMS_USE_ESC_COMBINATION                    1
#define EMS_USE_GSM_EXTENDED                    0
#define EMS_DISPLAY_SPECIAL_CHARACTERS             0
#define EMS_ENABLE_GRAPHICAL_CURSOR_MOVEMENT       1
#define EMS_INPUTBOX_PARAGRAPH_BREAK_CHARACTER        0x0A
#define EMS_ENABLE_SPECIAL_FONT_DISPLAY               1
#define EMS_HEADER_SCROLL_SIZE                     20
#define EMS_ENABLE_TESTFIX_OBJECT_ORDERING            1

/*Wangzl Add Start For 6021 Ver: TBM_780  on 2007-8-9 15:29 */
#define EMS_DEFAULT_DCS_PER_SEG_LENGTH				(152)
/*Wangzl Add End  For 6021 Ver: TBM_780  on 2007-8-9 15:29 */
#define EMS_DEFAULT_DCS_ALLOCATED_LENGTH           mmi_msg_get_seg_num() * EMS_DEFAULT_DCS_PER_SEG_LENGTH
#define EMS_UCS2_DCS_ALLOCATED_LENGTH              mmi_msg_get_seg_num() * 67

#if defined(__MMI_MESSAGES_CONCAT_SMS__)
#define EMS_DEFAULT_DCS_AVAILABLE_LENGTH           mmi_msg_get_seg_num() * 153
#define EMS_UCS2_DCS_AVAILABLE_LENGTH              mmi_msg_get_seg_num() * 67
#else /* defined(__MMI_MESSAGES_CONCAT_SMS__) */ 
#define EMS_DEFAULT_DCS_AVAILABLE_LENGTH           160
#define EMS_UCS2_DCS_AVAILABLE_LENGTH              70
#endif /* defined(__MMI_MESSAGES_CONCAT_SMS__) */ 

UI_EMS_input_box_theme *current_EMS_input_box_theme = NULL;
stFontAttribute UI_EMS_inputbox_small_font = {0, 0, 0, SMALL_FONT, 0, 0};
stFontAttribute UI_EMS_inputbox_medium_font = {0, 0, 0, MEDIUM_FONT, 0, 0};

#ifdef __MMI_DIALLING_FONT__
stFontAttribute UI_EMS_inputbox_large_font = {0, 0, 0, MEDIUM_FONT, 0, 0};
#else 
stFontAttribute UI_EMS_inputbox_large_font = {0, 0, 0, LARGE_FONT, 0, 0};
#endif 

 
U8 check_for_halant = 0;

 
stFontAttribute UI_EMS_inputbox_current_font = {0, 0, 0, MEDIUM_FONT, 0, 0};
color_t UI_EMS_inputbox_text_color = {0, 0, 0, 100};

#if(EMS_DISPLAY_SPECIAL_CHARACTERS)

/* Temporary data for debugging  */
const U8 EMS_CR_display_character[] = 
{
    0x04, 0x00, 0xAF, 0x00, 0x00, 0x00,
    0x09, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0C, 0x12, 0x00, 0x00, 0x00,
    0x34, 0x12, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,
    0x34, 0x12, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,
    0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0x04, 0x20,
    0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0xFF, 0xFF, 0xFF, 0xFF,
    0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0xFF, 0xFF,
    0xFF, 0xFF, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,
    0x04, 0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,
    0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,
    0x34, 0x12, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,
    0x34, 0x12,
};

const U8 EMS_LF_display_character[] = 
{
    0x04, 0x00, 0xAF, 0x00, 0x00, 0x00,
    0x09, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0C, 0x12, 0x00, 0x00, 0x00,
    0x34, 0x12, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
    0x34, 0x12, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
    0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0xC0, 0x00,
    0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00,
    0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xFF, 0xFF,
    0xFF, 0xFF, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
    0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
    0x34, 0x12, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00,
    0x34, 0x12,
};

const U8 EMS_ESC_display_character[] = 
{
    0x04, 0x00, 0xAF, 0x00, 0x00, 0x00,
    0x09, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0C, 0x12, 0x00, 0x00, 0x00,
    0x34, 0x12, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F,
    0x34, 0x12, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F,
    0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0xFF, 0xFF, 0x42, 0x1F, 0x42, 0x1F,
    0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0xFF, 0xFF,
    0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F,
    0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F,
    0x34, 0x12, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F, 0x42, 0x1F,
    0x34, 0x12,
};

#endif /* (EMS_DISPLAY_SPECIAL_CHARACTERS) */ 

#if(!UI_DISABLE_EMS_INPUT_BOX)
EMSObjData UI_EMS_normal_text_format_object_data;
EMSObject UI_EMS_normal_text_format_object;

#ifdef __MMI_MESSAGES_EMS__
extern void EMS_cancel_object_focus(void);
extern U16 EMS_predefined_animations[];
#endif /* __MMI_MESSAGES_EMS__ */ 
#define EMS_DATA_END_OFFSET(x)   ((x))

/* Basic Latin,Latin 1,Latin Extended A, Latin Extended B */
/* Cyrillic */
/* Latin Extended Additional  */
 
/* Basic Arabic  */
/* Arabic Presentation Forms-A */
/* Arabic Presentation Forms-B */
#define UI_MULTI_LINE_INPUT_BOX_ALPHABETIC_CHARACTER(c)                                                              \
   ((((UI_character_type)(c)<=(UI_character_type)0x0240) ||                                                          \
   ( ((UI_character_type)(c)>=(UI_character_type)0x0400) && ((UI_character_type)(c)<=(UI_character_type)0x04FF) ) || \
   ( ((UI_character_type)(c)>=(UI_character_type)0x1E00) && ((UI_character_type)(c)<=(UI_character_type)0x1EFF) ) || \
   ( ((UI_character_type)(c)>=(UI_character_type)0x0300) && ((UI_character_type)(c)<=(UI_character_type)0x036F) ) || \
   ( ((UI_character_type)(c)>=(UI_character_type)0x0621) && ((UI_character_type)(c)<=(UI_character_type)0x0653) ) || \
   ( ((UI_character_type)(c)>=(UI_character_type)0xFB50) && ((UI_character_type)(c)<=(UI_character_type)0xFDFF) ) || \
   ( ((UI_character_type)(c)>=(UI_character_type)0xFE70) && ((UI_character_type)(c)<=(UI_character_type)0xFEFF) ) )?(1):(0))

/* Arabic Connectivity issue */
#if 0//defined( __MMI_LANG_ARABIC__)||defined(__MMI_LANG_PERSIAN__)
#define IS_ARABIC_CHARACTER(c)                                                                                          \
   (( ( ((UI_character_type)(c)>=(UI_character_type)0x0391) && ((UI_character_type)(c)<=(UI_character_type)0x03A1) ) || \
    ( ((UI_character_type)(c)>=(UI_character_type)0x3A3) && ((UI_character_type)(c)<=(UI_character_type)0x03A9) ) ||    \
   ( ( ((UI_character_type)(c)>=(UI_character_type)0x0621) && ((UI_character_type)(c)<=(UI_character_type)0x0653) ) || \
   ( ((UI_character_type)(c)>=(UI_character_type)0xFB50) && ((UI_character_type)(c)<=(UI_character_type)0xFDFF) ) ||    \
    ((UI_character_type)(c) == 0x60c ))|| \
   ( ((UI_character_type)(c)>=(UI_character_type)0xFE70) && ((UI_character_type)(c)<=(UI_character_type)0xFEFF) ) )?(1):(0))
#endif /* __MMI_LANG_ARABIC__ */ 




kal_uint16 BackwardCurrentPositionSkipCR(EMSData *data);
kal_uint16 ForwardCurrentPositionSkipCR(EMSData *data);
void MoveCurrentPosition(EMSData *data, EMSPosition *start, EMSPosition *end, kal_uint16 steps);
void EMS_map_position(EMSData *data, EMSPosition *src, EMSPosition *dst);
void MoveCursorToPosition(EMSData *data, EMSPosition *dst);
U8 TestLastPosition(EMSData *data, EMSPosition *p);
U8 TestFirstPosition(EMSData *data, EMSPosition *p);
U8 BackwardCurrentPositionObject(EMSData *data, EMSPosition *dst);
U8 ForwardCurrentPositionObject(EMSData *data, EMSPosition *dst);
void EMS_move_graphical_cursor(EMSData *data, S32 start_x, S32 cursor_x, S32 max_count);


/*****************************************************************************
 * FUNCTION
 *  gui_intialize_EMS
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_intialize_EMS(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_EMS_normal_text_format_object.Type = EMS_TYPE_TEXT_FORMAT;
    UI_EMS_normal_text_format_object.next = NULL;
    UI_EMS_normal_text_format_object.prev = NULL;
    UI_EMS_normal_text_format_object.OffsetToText = 0;
    UI_EMS_normal_text_format_object.data = &UI_EMS_normal_text_format_object_data;
    ResetTextFormat(&UI_EMS_normal_text_format_object.data->text_format);
#ifdef __MMI_MESSAGES_TEXT_FORMAT_ALIGN_LEFT__
    UI_EMS_normal_text_format_object.data->text_format.Alignment = EMS_ALIGN_LEFT;
#else 
    UI_EMS_normal_text_format_object.data->text_format.Alignment = EMS_ALIGN_LANG_DEP;
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_EMS_input_box_theme
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 *  t       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_EMS_input_box_theme(UI_EMS_input_box *b, UI_EMS_input_box_theme *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    b->normal_filler = t->normal_filler;
    b->disabled_filler = t->disabled_filler;
    b->selected_filler = t->selected_filler;
    b->normal_text_color = t->normal_text_color;
    b->selected_text_color = t->selected_text_color;
    b->selection_color = t->selection_color;
    b->cursor_color = t->cursor_color;
    b->boundary_color = t->boundary_color; 
    b->flags |= t->flags;
}


/*****************************************************************************
 * FUNCTION
 *  gui_set_EMS_input_box_current_theme
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_EMS_input_box_current_theme(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    b->normal_filler = current_EMS_input_box_theme->normal_filler;
    b->disabled_filler = current_EMS_input_box_theme->disabled_filler;
    b->selected_filler = current_EMS_input_box_theme->selected_filler;
    b->normal_text_color = current_EMS_input_box_theme->normal_text_color;
    b->selected_text_color = current_EMS_input_box_theme->selected_text_color;
    b->selection_color = current_EMS_input_box_theme->selection_color;
    b->cursor_color = current_EMS_input_box_theme->cursor_color;
    b->boundary_color = current_EMS_input_box_theme->boundary_color;   
    b->flags |= current_EMS_input_box_theme->flags;
}

extern S16 MMI_current_input_ext_type;


/*****************************************************************************
 * FUNCTION
 *  gui_create_EMS_input_box
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b           [?]         
 *  x           [IN]        
 *  y           [IN]        
 *  width       [IN]        
 *  height      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_create_EMS_input_box(UI_EMS_input_box *b, S32 x, S32 y, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    b->x = x;
    b->y = y;
    b->width = width;
    b->height = height;
    b->flags = 0;
    MMI_current_input_ext_type = 0;
    gui_set_EMS_input_box_current_theme(b);
    b->overflow_callback = UI_dummy_function;
    b->change_callback = UI_dummy_function;
    gui_create_vertical_scrollbar(
        &b->vbar,
        b->x + b->width - current_EMS_input_box_theme->vbar_width - 1,
        b->y + 1,
        current_EMS_input_box_theme->vbar_width,
        b->height - 2);
    b->data = NULL;
    b->start_position.Object = NULL;
    b->start_position.OffsetToText = 0;
    b->end_position.Object = NULL;
    b->end_position.OffsetToText = 0;
    b->display_y = 0;
    ResetTextFormat(&b->text_format);
#if defined(__MMI_HINDI_ALG__)
    reset_use_multitap();
#endif 
    b->text_format.Alignment = UI_EMS_normal_text_format_object.data->text_format.Alignment;
    b->saved_cursor_ax = -1;
    b->underline_color = gui_color(0, 0, 128);
    b->strikethrough_color = gui_color(192, 0, 0);
    b->paragraph_color = gui_color(0, 128, 0);
    
    b->locate_cursor_flag = 1;
    
}

#if(EMS_USE_DCS)


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_change_DCS
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_change_DCS(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (b->UCS2_count > 0)
    {
        EMSSetDCS(b->data, SMSAL_UCS2_DCS);
    #if defined(__MMI_MESSAGES_CONCAT_SMS__)
	// TODO: 暂时为了计数准确，而把b->data->Reminder.maxUsableLenPerSeg硬写成134
	b->available_length = /*b->data->Reminder.maxUsableLenPerSeg */134 * mmi_msg_get_seg_num() >> 1;
    #else 
        b->available_length = EMS_UCS2_DCS_AVAILABLE_LENGTH;
    #endif 
    }
    else
    {
        EMSSetDCS(b->data, SMSAL_DEFAULT_DCS);
    #if defined(__MMI_MESSAGES_CONCAT_SMS__)
        b->available_length = b->data->Reminder.maxUsableLenPerSeg * mmi_msg_get_seg_num();
    #else 
        b->available_length = EMS_DEFAULT_DCS_AVAILABLE_LENGTH;
    #endif 
    }
}

#endif /* (EMS_USE_DCS) */ 


/*****************************************************************************
 * FUNCTION
 *  gui_set_EMS_input_box_data
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 *  d       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_set_EMS_input_box_data(UI_EMS_input_box *b, EMSData *d)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSObject *text_format_object;
    U8 done = 0;
    EMSPosition current_position;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    b->data = d;
    if (d == NULL)
    {
        return;
    }
    /* ResetCurrentPosition(b->data); */
    b->UCS2_count = 0;
    b->object_count = 0;
    b->GSM_ext_count = 0;
#if(EMS_USE_DCS)
    if ((b->data->textLength > 0) || (b->data->listHead != NULL))
    {
        U8 r;
        EMSPosition p = b->data->CurrentPosition;
        EMSObject *current_object;
        UI_character_type c;

        p.OffsetToText = 0;
        p.Object = b->data->listHead;
        while (!done)
        {
            r = gui_EMS_input_box_get_next_object(d, &p, &c, &current_object);
            switch (r)
            {
                case 0:
                    done = 1;
                    break;
                case 1:
                    if (UI_TEST_UCS2_CHARACTER(c))
                    {
                        b->UCS2_count++;
                    }
                    if (UI_TEST_GSM_EXTENDED(c))
                    {
                        b->GSM_ext_count++;
                    }
                    break;
                case 2:
                    b->object_count++;
                    break;
            }
        }
    }
    gui_EMS_change_DCS(b);
#endif /* (EMS_USE_DCS) */ 
    current_position.OffsetToText = 0;
    current_position.Object = NULL;
    text_format_object = GetTextFormatObjectAtPosition(b->data, &current_position);
    if (text_format_object != NULL)
    {
        b->text_format = text_format_object->data->text_format;
    }
    b->saved_cursor_ax = -1;
}


/*zhoumn Modify Start For 6052 Ver: TBM_780  on 2007-8-7 13:52 */
#if 0
S32 EMS_get_remaining_length(EMSData *data)
{	
	if (data->dcs == SMSAL_DEFAULT_DCS) // 如果是7bits编码方式,也就是纯英文短信的编码方式...
	{
		#if defined(__MMI_MESSAGES_CONCAT_SMS__)
			return(data->Reminder.totalRemainingOctet-(data->Reminder.maxUsableLenPerSeg*mmi_msg_get_seg_num()-data->Reminder.maxUsableLenPerSeg*mmi_msg_get_seg_num()));
		#else
			return(data->Reminder.totalRemainingOctet-(EMS_DEFAULT_DCS_ALLOCATED_LENGTH-EMS_DEFAULT_DCS_AVAILABLE_LENGTH));
		#endif
	}
	else
	{
#if 0 //defined(__MMI_GB__)
		return(data->Reminder.totalRemainingOctet-((EMS_DEFAULT_DCS_ALLOCATED_LENGTH)-(EMS_UCS2_DCS_AVAILABLE_LENGTH<<1)));
#endif
	#if 1//defined(__MMI_MESSAGES_CONCAT_SMS__) //长短信支持
	// TODO: 暂时为了计数准确，而减去76
		// 剩余字符的计算方式不合理.
		return(data->Reminder.totalRemainingOctet-(76/*data->Reminder.maxUsableLenPerSeg*mmi_msg_get_seg_num()-data->Reminder.maxUsableLenPerSeg*mmi_msg_get_seg_num()*/));
	#else
		return(data->Reminder.totalRemainingOctet-((EMS_UCS2_DCS_ALLOCATED_LENGTH<<1)-(EMS_UCS2_DCS_AVAILABLE_LENGTH<<1)));
	#endif             
	}
}
#else // SMS support
#include "gbhmi.h"
S32 EMS_get_remaining_length(EMSData *data)
{	
	if (data->dcs == SMSAL_DEFAULT_DCS) // 如果是7bits编码方式,也就是纯英文短信的编码方式...
	{
#if defined(__MMI_MESSAGES_CONCAT_SMS__)
		return EMSEditor_Enghish_Length - data->textLength;
#else
		return EMSEditor_Enghish_Length - data->textLength;
#endif
	}
	else
	{
#if defined(__MMI_MESSAGES_CONCAT_SMS__)
		return EMSEditor_Chinese_Length - data->textLength;
#else
		return EMSEditor_Chinese_Length - data->textLength;
#endif             
	}
}

#endif
/*zhoumn Modify End  For 6052 Ver: TBM_780  on 2007-8-7 13:52 */

#ifdef __MMI_MESSAGES_EMS__


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_set_text_format_object
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 *  f       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_set_text_format_object(UI_EMS_input_box *b, EMSTextFormat *f)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (f != NULL)
    {
        b->text_format = *f;
    }
}
#endif /* __MMI_MESSAGES_EMS__ */ 


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_locate_cursor
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_locate_cursor(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 box_height = b->height - 4;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    b->flags |= UI_EMS_INPUT_BOX_DISABLE_DRAW;
    gui_show_EMS_input_box(b);
    b->flags &= ~UI_EMS_INPUT_BOX_DISABLE_DRAW;
    if (b->flags & UI_EMS_INPUT_BOX_DISPLAY_HEADER)
    {
        if ((b->cursor_ay + b->header_height) < (-b->display_y))
        {
            b->display_y = -b->cursor_ay - b->header_height;
        }
        else if ((b->cursor_ay + b->header_height + b->cursor_line_height) > ((-b->display_y) + (box_height)))
        {
            b->display_y -=
                (b->cursor_ay + b->cursor_line_height) - ((-b->display_y) + (box_height - b->header_height));
        }
        if (b->display_y > 0)
        {
            b->display_y = 0;
        }
    }
    else
    {
        if (b->cursor_ay < (-b->display_y))
        {
            b->display_y = -b->cursor_ay;
        }
        else if ((b->cursor_ay + b->cursor_line_height) > ((-b->display_y) + box_height))
        {
            b->display_y -= (b->cursor_ay + b->cursor_line_height) - ((-b->display_y) + box_height);
        }
        if ((b->display_y + b->display_height) < box_height)
        {
            b->display_y = (box_height) - b->display_height;
        }
        if (b->display_y > 0)
        {
            b->display_y = 0;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_reset_cursor_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_reset_cursor_position(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (b->data != NULL)
    {
        ResetCurrentPosition(b->data);
        gui_EMS_input_box_locate_cursor(b);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_get_previous_text_alignment
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 *  p       [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_input_box_get_previous_text_alignment(UI_EMS_input_box *b, EMSPosition *p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 done = 0;
    UI_character_type c;
    EMSObject *o;
    U8 r;
    EMSPosition current_position = *p;
    U8 alignment = UI_EMS_normal_text_format_object.data->text_format.Alignment;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (!done)
    {
        r = gui_EMS_input_box_get_previous_object(b->data, &current_position, &c, &o);
        switch (r)
        {
            case 0:
                done = 1;
                break;
            case 1:
                if (c == EMS_INPUTBOX_PARAGRAPH_BREAK_CHARACTER)
                {
                    done = 1;
                }
                break;
            case 2:
                if (o->Type == EMS_TYPE_TEXT_FORMAT)
                {
                    alignment = o->data->text_format.Alignment;
                }
                break;
        }
    }
    return (alignment);
}

/* Sets the alignment of the whole paragraph to the value specified.
   Starts from the given cursor position.
   The cursor position given must be at the first position on the paragraph   */


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_set_paragraph_text_alignment
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b               [?]         
 *  p               [?]         
 *  alignment       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_set_paragraph_text_alignment(UI_EMS_input_box *b, EMSPosition *p, U8 alignment)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 done = 0;
    UI_character_type c;
    EMSObject *o;
    U8 r;
    EMSPosition current_position = *p;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (!done)
    {
        r = gui_EMS_input_box_get_next_object(b->data, &current_position, &c, &o);
        switch (r)
        {
            case 0:
                done = 1;
                break;
            case 1:
                if (c == EMS_INPUTBOX_PARAGRAPH_BREAK_CHARACTER)
                {
                    done = 1;
                }
                break;
            case 2:
                if (o->Type == EMS_TYPE_TEXT_FORMAT)
                {
                    o->data->text_format.Alignment = (EMSALIGNMENT) alignment;
                }
                break;
        }
    }
}

/* Checks if the cursor is at the beginning of a paragraph  */


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_test_paragraph_start
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 *  p       [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_input_box_test_paragraph_start(UI_EMS_input_box *b, EMSPosition *p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 paragraph_flag = 0;
    UI_character_type c;
    EMSObject *o;
    U8 r;
    EMSPosition current_position = *p;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (TestFirstPosition(b->data, &current_position))
    {
        paragraph_flag = 1;
    }
    else
    {
        if (BackwardCurrentPosition(b->data, 1) == 1)
        {
            current_position = b->data->CurrentPosition;
            r = gui_EMS_input_box_get_next_object(b->data, &current_position, &c, &o);
            ForwardCurrentPosition(b->data, 1);
            if ((r == 1) && (c == EMS_INPUTBOX_PARAGRAPH_BREAK_CHARACTER))
            {
                paragraph_flag = 1;
            }
        }
    }
    return (paragraph_flag);
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_set_text_format_cursor_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_set_text_format_cursor_position(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 done = 0;
    EMSPosition current_position;
    EMSObject *current_text_format_object;
    UI_character_type c;
    EMSObject *o;
    U8 r;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    b->text_format = UI_EMS_normal_text_format_object.data->text_format;
    if (b->data == NULL)
    {
        return;
    }
    current_position = b->data->CurrentPosition;
    current_text_format_object = b->data->CurrentTextFormatObj;

    if ((current_text_format_object != NULL)
        && (current_position.OffsetToText >= current_text_format_object->OffsetToText)
        && (current_position.OffsetToText <=
            (current_text_format_object->OffsetToText + current_text_format_object->data->text_format.textLength)))
    {
        b->text_format = current_text_format_object->data->text_format;
    }

    /* At the beginning of a paragraph ? Then locate the text alignment to be used   */
    if (gui_EMS_input_box_test_paragraph_start(b, &current_position))
    {
        while (!done)
        {
            r = gui_EMS_input_box_get_next_object(b->data, &current_position, &c, &o);
            switch (r)
            {
                case 0:
                    b->text_format.Alignment = UI_EMS_normal_text_format_object.data->text_format.Alignment;
                    done = 1;
                    break;
                case 1:
                    if (c == EMS_INPUTBOX_PARAGRAPH_BREAK_CHARACTER)
                    {
                        done = 1;
                    }
                    break;
                case 2:
                    if (o->Type == EMS_TYPE_TEXT_FORMAT)
                    {
                        b->text_format.Alignment = o->data->text_format.Alignment;
                        done = 1;
                    }
                    break;
            }
        }
    }
     /*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-4-8 10:33 */
    #if 0
    else
    {
        while (!done)
        {
            r = gui_EMS_input_box_get_previous_object(b->data, &current_position, &c, &o);
            switch (r)
            {
                case 0:
                    done = 1;
                    break;
                case 1:
                    if (c == EMS_INPUTBOX_PARAGRAPH_BREAK_CHARACTER)
                    {
                        done = 1;
                    }
                    break;
                case 2:
                    if (o->Type == EMS_TYPE_TEXT_FORMAT)
                    {
                        b->text_format.Alignment = o->data->text_format.Alignment;
                        done = 1;
                    }
                    break;
            }
        }

    }
    #endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-4-8 10:33 */
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_set_cursor_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 *  p       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_set_cursor_position(UI_EMS_input_box *b, EMSPosition *p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (b->data != NULL)
    {
        ResetCurrentPosition(b->data);
        while (!
               ((b->data->CurrentPosition.OffsetToText == p->OffsetToText) &&
                (b->data->CurrentPosition.Object == p->Object)))
        {
            if (ForwardCurrentPosition(b->data, 1) != 1)
            {
                break;
            }
        }
        gui_EMS_input_box_locate_cursor(b);
    }
}

/* Macros for graphics functions */

extern stFontAttribute *UI_font;
extern U8 SetFont(stFontAttribute Font, U8 arrCount);

extern S32 UI_clip_x1, UI_clip_x2, UI_clip_y1, UI_clip_y2;
extern S32 UI_text_x, UI_text_y;
extern color_t UI_text_color;

#define EMS_SET_FONT(f)             \
      {  UI_font=(f);               \
         SetFont(*(f),0);           \
      }

#define EMS_MOVE_TEXT_CURSOR(x,y)      \
      {  UI_text_x=x;                  \
         UI_text_y=y;                  \
      }

#define EMS_SET_TEXT_COLOR(c)                     \
      {  UI_text_color=c;                         \
         UI_set_current_text_color(UI_text_color);\
      }


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_equal_text_format
 * DESCRIPTION
 *  
 * PARAMETERS
 *  t1      [?]     
 *  t2      [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_input_box_equal_text_format(EMSTextFormat *t1, EMSTextFormat *t2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (t1->Alignment != t2->Alignment)
    {
        return (0);
    }
    if (t1->bgColor != t2->bgColor)
    {
        return (0);
    }
    if (t1->fgColor != t2->fgColor)
    {
        return (0);
    }
    if (t1->FontSize != t2->FontSize)
    {
        return (0);
    }
    if (t1->isBold != t2->isBold)
    {
        return (0);
    }
    if (t1->isItalic != t2->isItalic)
    {
        return (0);
    }
    if (t1->isStrikethrough != t2->isStrikethrough)
    {
        return (0);
    }
    if (t1->isUnderline != t2->isUnderline)
    {
        return (0);
    }
    if (t1->textLength != t2->textLength)
    {
        return (0);
    }
    return (1);
}

#define EMS_EQUAL_TEXT_FORMAT(t1,t2) (~(memcmp((t1),(t2),sizeof(EMSTextFormat)))
#define EMS_ESCAPE_CHARACTER  (27)


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_remove_extension_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_remove_extension_character(EMSData *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 c, OffsetToText;
    U8 *textBuffer = data->textBuffer;
    U16 textBufferLength = data->textLength;
    EMSPosition currposition = data->CurrentPosition;

#if defined(__MMI_T9__)
     
    EMSPosition backupcurrposition = data->CurrentPosition;

     
#endif /* defined(__MMI_T9__) */ 
    kal_uint16 ret_char;
    EMSPosition dummyposition;
    EMSObject *ret_object;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (textBuffer == NULL)
    {
        return;
    }
    ResetCurrentPosition(data);
    if (data->dcs == SMSAL_DEFAULT_DCS)
    {
        while ((OffsetToText = data->CurrentPosition.OffsetToText) < textBufferLength)
        {
        #if(EMS_BYTE_ORDER_BIG_ENDIAN)
            c = (U16) (textBuffer[OffsetToText + 1] | (textBuffer[OffsetToText] << 8));
        #else 
            c = (U16) (textBuffer[OffsetToText] | (textBuffer[OffsetToText + 1] << 8));
        #endif 
            if (c == EMS_ESCAPE_CHARACTER)
            {
                dummyposition = data->CurrentPosition;
                while (EMSGetNextObject(data, &dummyposition, &ret_char, &ret_object) == 2)
                {
                    ForwardCurrentPosition(data, 1);
                    dummyposition = data->CurrentPosition;

                    if (ret_object->Type == EMS_TYPE_TEXT_FORMAT)
                    {
                        if (EMSGetPreviousObject(data, &dummyposition, &ret_char, &ret_object) == 1)
                        {
                            BackwardCurrentPosition(data, 1);
                            break;
                        }
                        dummyposition = data->CurrentPosition;
                    }
                }
            #if(EMS_BYTE_ORDER_BIG_ENDIAN)
                if (ForwardCurrentPosition(data, 1) != 1)
                {
                    break;
                }
                CancelCurrentPosition(data, 1);
                if (OffsetToText <= currposition.OffsetToText)
                {
                    currposition.OffsetToText--;
                }
                OffsetToText = data->CurrentPosition.OffsetToText;
                textBufferLength -= ENCODING_LENGTH;
            #else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
                if (ForwardCurrentPosition(data, 1) != 1)
                {
                    break;
                }
                CancelCurrentPosition(data, 1);
                if (OffsetToText <= currposition.OffsetToText)
                {
                    currposition.OffsetToText -= ENCODING_LENGTH;
                }
                OffsetToText = data->CurrentPosition.OffsetToText;
                textBufferLength -= ENCODING_LENGTH;
            #endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
            }
            if (ForwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        }
    }
    MoveCursorToPosition(data, &currposition);
#if defined(__MMI_T9__)
     
    if (memcmp((const void*)&backupcurrposition, (const void*)&currposition, sizeof(EMSPosition)) == 0)
    {
        return;
    }
     
    T9SynT9WithInputBoxForEMSInputBox();
#endif /* defined(__MMI_T9__) */ 
}

/* Call before EMSPack  */


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_insert_extension_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_insert_extension_character(EMSData *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 c, OffsetToText;
    U8 *textBuffer = data->textBuffer;
    U16 textBufferLength = data->textLength;
    EMSPosition currposition = data->CurrentPosition;

#if defined(__MMI_T9__)
     
    EMSPosition backupcurrposition = data->CurrentPosition;

     
#endif /* defined(__MMI_T9__) */ 
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
    U8 ESC_string[] = {0x00, 0x1B};
#else 
    U8 ESC_string[] = {0x1B, 0x00};
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (textBuffer == NULL)
    {
        return;
    }
    ResetCurrentPosition(data);

    if (data->dcs == SMSAL_DEFAULT_DCS)
    {
        while ((OffsetToText = data->CurrentPosition.OffsetToText) < textBufferLength)
        {
        #if(EMS_BYTE_ORDER_BIG_ENDIAN)
            c = (U16) (textBuffer[OffsetToText + 1] | (textBuffer[OffsetToText] << 8));
        #else 
            c = (U16) (textBuffer[OffsetToText] | (textBuffer[OffsetToText + 1] << 8));
        #endif 
            if (UI_TEST_GSM_EXTENDED(c))
            {
                AddString(data, ESC_string, 1, NULL);
                if (OffsetToText <= currposition.OffsetToText)
                {
                    currposition.OffsetToText += ENCODING_LENGTH;
                }
                OffsetToText = data->CurrentPosition.OffsetToText;
                 
                textBufferLength += ENCODING_LENGTH;
                /* CSD end */
            }
            if (ForwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        }
    }
    MoveCursorToPosition(data, &currposition);
#if defined(__MMI_T9__)
     
    if (memcmp((const void*)&backupcurrposition, (const void*)&currposition, sizeof(EMSPosition)) == 0)
    {
        return;
    }
     
    T9SynT9WithInputBoxForEMSInputBox();
#endif /* defined(__MMI_T9__) */ 
}

#if (EMS_USE_DCS)

#if(EMS_USE_CRLF_COMBINATION || EMS_USE_ESC_COMBINATION)

/* Deletes one object/character before cursor position and updates UCS2 and Object counters  */
/* Also handles CRLF combination                                              */


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_cancel_current_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_cancel_current_position(UI_EMS_input_box *b)
{
#ifdef __MMI_HINDI_ALG__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 lst_pos = 0;
    S32 cancel_two_pos = 0;
    UI_character_type current_character1 = 0;
    U8 object_type_check1 = 0;
    EMSObject *o1;
    EMSPosition p1 = b->data->CurrentPosition;
    EMSData *data_chk_type1 = b->data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (TestLastPosition(b->data, &(b->data->CurrentPosition)))
    {
        lst_pos = 1;
    }
#endif /* __MMI_HINDI_ALG__ */ 

    if (BackwardCurrentPosition(b->data, 1) == 1)
    {
        EMSPosition p = b->data->CurrentPosition;
        EMSObject *current_object;
        UI_character_type c;
        U8 r;
        S32 count;
        U8 dcs_change = 0;

        r = gui_EMS_input_box_get_next_object(b->data, &p, &c, &current_object);
    #ifdef __MMI_HINDI_ALG__
        if (lst_pos)
        {
            if (b->data->CurrentPosition.OffsetToText >= 4 &&
                UI_TEST_LF_CHARACTER(b->data->textBuffer[b->data->CurrentPosition.OffsetToText]) &&
                UI_TEST_CR_CHARACTER(b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 2]))
            {
                b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 2] = 0;
                b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 1] = 0;
                cancel_two_pos = 1;

            }
            b->data->textBuffer[b->data->CurrentPosition.OffsetToText] = 0;
            b->data->textBuffer[b->data->CurrentPosition.OffsetToText + 1] = 0;
        }
    #endif /* __MMI_HINDI_ALG__ */ 
        ForwardCurrentPosition(b->data, 1);
        switch (r)
        {
            case 0:
                break;
            case 1:
        #if(EMS_USE_CRLF_COMBINATION)
                if (UI_TEST_LF_CHARACTER(c))
                {
                    count = BackwardCurrentPosition(b->data, 2);
                    if (count == 2)
                    {
                        p = b->data->CurrentPosition;
                        r = gui_EMS_input_box_get_next_object(b->data, &p, &c, &current_object);
                        if ((r == 1) && UI_TEST_CR_CHARACTER(c))
                        {
                            ForwardCurrentPosition(b->data, 2);
                            CancelCurrentPosition(b->data, 1);
                            count = 0;
                        }
                    }
                    ForwardCurrentPosition(b->data, (U16) count);
                }
                else
        #endif /* (EMS_USE_CRLF_COMBINATION) */ 
                if (UI_TEST_UCS2_CHARACTER(c))
                {
                    if (b->UCS2_count > 0)
                    {
                        b->UCS2_count--;
                        if (b->UCS2_count == 0)
                        {
                            dcs_change = 1;
                            gui_EMS_change_DCS(b);
                        }
                    }
                }
                else if (UI_TEST_GSM_EXTENDED(c))
                {
                    if (b->GSM_ext_count > 0)
                    {
                        b->GSM_ext_count--;
                    }
                }
                break;
            case 2:
                /* CSD Joy added for text format case, get the character and set the parameters accordingly, 1230 */
                if (current_object->Type == 0)
                {
                    r = gui_EMS_input_box_get_next_object(b->data, &p, &c, &current_object);
                    if ((r == 1) && UI_TEST_UCS2_CHARACTER(c))
                    {
                        if (b->UCS2_count > 0)
                        {
                            b->UCS2_count--;
                            if (b->UCS2_count == 0)
                            {
                                dcs_change = 1;
                                gui_EMS_change_DCS(b);
                            }
                        }
                    }
                    else if (UI_TEST_GSM_EXTENDED(c))
                    {
                        if (b->GSM_ext_count > 0)
                        {
                            b->GSM_ext_count--;
                        }
                    }
                    break;
                }
                /* CSD end */
                if (b->object_count > 0)
                {
                    b->object_count--;
                    if (b->object_count == 0)
                    {
                        gui_EMS_change_DCS(b);
                    }
                }
                break;
        }
        CancelCurrentPosition(b->data, 1);
    #ifdef __MMI_HINDI_ALG__
        b->data->textBuffer[b->data->textLength] = 0;
        b->data->textBuffer[b->data->textLength + 1] = 0;
        if (cancel_two_pos)
        {
            CancelCurrentPosition(b->data, 1);
        }
        if (b->data->CurrentPosition.OffsetToText >= 4)
        {
            if ((UI_CHK_CONSONANT(b->data->textBuffer[b->data->CurrentPosition.OffsetToText]))
                && (UI_CHK_CONSONANT(b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 4]))
                && (b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 2] == 0x4d)
                && ((b->data->textBuffer[b->data->CurrentPosition.OffsetToText + 1]) == 0x09)
                && ((b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 3]) == 0x09))
            {
                EMSObject *bb;
                U8 fwd_positions = 0;
                U8 saved_fwd_positions = 0;
                PU8 dest_pos;
                PU8 src_pos = &b->data->textBuffer[b->data->CurrentPosition.OffsetToText];

                p1 = b->data->CurrentPosition;
                object_type_check1 = gui_EMS_input_box_get_next_object(data_chk_type1, &p1, &current_character1, &o1);

                /* dest_pos = hf_get_next_cluster_p(b->data->textBuffer,src_pos); */
                dest_pos = hf_get_next_cluster_p(src_pos, src_pos);
                fwd_positions = dest_pos - src_pos; /* Number of moves */
                fwd_positions = fwd_positions >> 1;
                saved_fwd_positions = fwd_positions;
                PRINT_INFORMATION_2((MMI_TRACE_INFO, "coolsand_UI_EMS_cancel_current_position[fwd_positions]:%d  ",
                                     fwd_positions));
                while (fwd_positions--)
                {
                    ForwardCurrentPosition(b->data, 1);
                }
                if (object_type_check1 == 2)
                {
                    if (saved_fwd_positions == 1)
                    {
                        /* To shift the images on cluster completion */
                        bb = b->data->listTail;
                        do
                        {
                            if (bb->OffsetToText == b->data->CurrentPosition.Object->OffsetToText)
                            {
                                bb->OffsetToText += (saved_fwd_positions) * 2;
                            }
                            bb = bb->prev;
                        } while (bb != NULL);

                        ForwardCurrentPosition(b->data, saved_fwd_positions);
                        BackwardCurrentPositionSkipCR(b->data);
                    }
                    else if (saved_fwd_positions > 1)
                    {
                        /* To shift the images on cluster completion */
                        bb = b->data->listHead;
                        do
                        {
                            if (bb->OffsetToText == b->data->CurrentPosition.Object->OffsetToText)
                            {
                                bb->OffsetToText += (saved_fwd_positions) * 2;
                            }
                            bb = bb->next;
                        } while (bb != NULL);
                        ForwardCurrentPosition(b->data, saved_fwd_positions);
                        if (b->data->listHead == b->data->listTail)
                        {
                            BackwardCurrentPositionSkipCR(b->data);
                        }
                        else
                        {
                            BackwardCurrentPositionSkipCR(b->data);
                            BackwardCurrentPositionSkipCR(b->data);
                        }

                    }

                }

            }
        }
    #endif /* __MMI_HINDI_ALG__ */ 

        if (dcs_change)
        {
            gui_EMS_input_box_insert_extension_character(b->data);
        }

    #if(EMS_USE_ESC_COMBINATION)
        if (b->UCS2_count == 0)
        {
            p = b->data->CurrentPosition;
            r = gui_EMS_input_box_get_previous_object(b->data, &p, &c, &current_object);
            if ((r == 1) && UI_TEST_ESC_CHARACTER(c))
            {
                CancelCurrentPosition(b->data, 1);
            }
        }
    #endif /* (EMS_USE_ESC_COMBINATION) */ 
        return (1);
    }
    return (0);
}

#else /* (EMS_USE_CRLF_COMBINATION || EMS_USE_ESC_COMBINATION) */ 

/* Deletes one object/character before cursor position and updates UCS2 and Object counters  */


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_cancel_current_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_cancel_current_position(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (BackwardCurrentPosition(b->data, 1) == 1)
    {
        EMSPosition p = b->data->CurrentPosition;
        EMSObject *current_object;
        UI_character_type c;
        U8 r;

        r = gui_EMS_input_box_get_next_object(b->data, &p, &c, &current_object);
        ForwardCurrentPosition(b->data, 1);
        switch (r)
        {
            case 0:
                break;
            case 1:
                if (UI_TEST_UCS2_CHARACTER(c))
                {
                    if (b->UCS2_count > 0)
                    {
                        b->UCS2_count--;
                        if (b->UCS2_count == 0)
                        {
                            gui_EMS_change_DCS(b);
                        }
                    }
                }
                break;
            case 2:
                if (b->object_count > 0)
                {
                    b->object_count--;
                    if (b->object_count == 0)
                    {
                        gui_EMS_change_DCS(b);
                    }
                }
                break;
        }
        CancelCurrentPosition(b->data, 1);
        return (1);
    }
    return (0);
}

#endif /* (EMS_USE_CRLF_COMBINATION || EMS_USE_ESC_COMBINATION) */ 

#define  EMSSWAP(t,a,b) \
{  t temp=a;            \
   a=b;                 \
   b=temp;              \
}

/* Temporary function: Will be removed later                            */
/* For testing purpose only: Using this will cause an assertion in the EMS library  */


/*****************************************************************************
 * FUNCTION
 *  ArrangeEMSObjects
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void ArrangeEMSObjects(EMSData *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSObject *current = data->listHead;
    EMSObject *start;
    U8 done;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (current != NULL)
    {
        done = 0;
        start = current;
        while (!done)
        {
            current = current->next;
            if (current == NULL)
            {
                done = 1;
                break;
            }
            if (current->OffsetToText != start->OffsetToText)
            {
                done = 1;
                break;
            }
            if (current->Type == EMS_TYPE_TEXT_FORMAT)
            {
                EMSSWAP(EMSObject, *current, *start);
                EMSSWAP(kal_uint16, current->OffsetToText, start->OffsetToText);
                EMSSWAP(struct _EMSObject *, current->prev, start->prev);
                EMSSWAP(struct _EMSObject *, current->next, start->next);

                break;
            }
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_add_string
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b               [?]     
 *  s               [?]     
 *  text_format     [?]     
 * RETURNS
 *  
 *****************************************************************************/
EMSTATUS gui_EMS_input_box_add_string(UI_EMS_input_box *b, U8 *s, EMSTextFormat *text_format)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UINT16	test_length = b->data->CurrentPosition.OffsetToText, test = 0;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
    UI_character_type c = ((UI_character_type) s[1]) | (((UI_character_type) s[0]) << 8);
#else 
    UI_character_type c = ((UI_character_type) s[0]) | (((UI_character_type) s[1]) << 8);
#endif 
    EMSTATUS r;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (text_format == NULL)
	{	//r=AddString(b->data,s,num_char,NULL);// wangbei del 20060623
	// wangbei add start 20060623
			b->data->textBuffer[b->data->textLength]= s[0];
			b->data->textLength++;
			b->data->textBuffer[b->data->textLength]=s[1];
			b->data->textLength++;
			b->data->CurrentPosition.OffsetToText +=2;
			r=EMS_OK;
        // wangbei add end
    }
    else if (b->data->CurrentTextFormatObj == NULL)
	{	if(gui_EMS_input_box_equal_text_format(&b->text_format,&UI_EMS_normal_text_format_object.data->text_format))
		{	//r=AddString(b->data,s,num_char,NULL);// wangbei del 20060623
		// wangbei add start 20060623
			if (b->data->textLength != b->data->CurrentPosition.OffsetToText)            //add by yaosq 20061220 for add character
			{
				if (CursorPlace == 0)
				{
					while (test_length<b->data->textLength)
					{
						b->data->textBuffer[b->data->textLength - test] = b->data->textBuffer[b->data->textLength - 2 - test];			
						b->data->textBuffer[b->data->textLength - test + 1] = b->data->textBuffer[b->data->textLength - 1 - test];
						test += 2;
						test_length += 2;
					}
					b->data->textBuffer[b->data->CurrentPosition.OffsetToText] = 0;			
					b->data->textBuffer[b->data->CurrentPosition.OffsetToText + 1] = 0;
					b->data->textBuffer[b->data->CurrentPosition.OffsetToText] = s[0];			
					b->data->textLength++;
					b->data->textBuffer[b->data->CurrentPosition.OffsetToText + 1] = s[1];
					b->data->textLength++;
				}
				else
				{
					b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 2] = 0;			
					b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 1] = 0;
					b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 2] = s[0];			
					b->data->textBuffer[b->data->CurrentPosition.OffsetToText -1 ] = s[1];
					b->data->CurrentPosition.OffsetToText -= 2;
					CursorPlace = 0;
				}
			}
    else
			{
				if (CursorPlace == 0)
				{
					b->data->textBuffer[b->data->textLength]= s[0];
					b->data->textLength++;
					b->data->textBuffer[b->data->textLength]=s[1];
					b->data->textLength++;
				}
				else
				{
					b->data->textBuffer[b->data->textLength - 2] = s[0];
					b->data->textBuffer[b->data->textLength - 1] = s[1];
					b->data->CurrentPosition.OffsetToText -= 2;
					CursorPlace = 0;
				}
			}
			b->data->CurrentPosition.OffsetToText +=2;
			r=EMS_OK;
              // wangbei add end
		}
	
		else
		{	
			//r=AddString(b->data,s,num_char,text_format);// wangbei del 20060623
			// wangbei add start 20060623
			b->data->textBuffer[b->data->textLength]= s[0];
			b->data->textLength++;
			b->data->textBuffer[b->data->textLength]=s[1];
			b->data->textLength++;
			b->data->CurrentPosition.OffsetToText +=2;
			r=EMS_OK;
               // wangbei add end
		}
	}
	else
	{	//r=AddString(b->data,s,num_char,text_format);// wangbei del 20060623
			// wangbei add start 20060623
			b->data->textBuffer[b->data->textLength]= s[0];
			b->data->textLength++;
			b->data->textBuffer[b->data->textLength]=s[1];
			b->data->textLength++;
			b->data->CurrentPosition.OffsetToText +=2;
			r=EMS_OK;
       // wangbei add end
	}
    if (r == EMS_OK)
    {
        if (UI_TEST_UCS2_CHARACTER(c))
        {
            if (b->UCS2_count == 0)
            {
                b->UCS2_count++;
                gui_EMS_change_DCS(b);
            }
            else
            {
                b->UCS2_count++;
            }
        }
        else if (UI_TEST_GSM_EXTENDED(c))
        {
            b->GSM_ext_count++;
        }
        if (b->UCS2_count > 0)
        {
            S32 remaining_length = (EMS_get_remaining_length(b->data) >> 1);

            if (remaining_length < 0)
            {
                gui_EMS_cancel_current_position(b);
                r = EMS_NO_SPACE;
            }
        }
    #if(EMS_USE_GSM_EXTENDED)
        else if (b->GSM_ext_count > 0)
        {
            S32 remaining_length = EMS_get_remaining_length(b->data);

            /* S32 available_length=b->available_length; */
            if (remaining_length < 0)
            {
                gui_EMS_cancel_current_position(b);
                r = EMS_NO_SPACE;
            }
        }
    #endif /* (EMS_USE_GSM_EXTENDED) */ 
    }
    else
    {
         
        gui_EMS_change_DCS(b);
    }

    /* Temporary   */
    /* ArrangeEMSObjects(b->data); */

    return (r);
}

#else /* (EMS_USE_DCS) */ 


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_add_string
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b               [?]     
 *  s               [?]     
 *  text_format     [?]     
 * RETURNS
 *  
 *****************************************************************************/
EMSTATUS gui_EMS_input_box_add_string(UI_EMS_input_box *b, U8 *s, EMSTextFormat *text_format)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 num_char = 1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (text_format == NULL)
    {
        return (AddString(b->data, s, num_char, NULL));
    }
    else if (b->data->CurrentTextFormatObj == NULL)
    {
        if (gui_EMS_input_box_equal_text_format(&b->text_format, &UI_EMS_normal_text_format_object.data.text_format))
        {
            return (AddString(b->data, s, num_char, NULL));
        }
        else
        {
            return (AddString(b->data, s, num_char, text_format));
        }
    }
    else
    {
        return (AddString(b->data, s, num_char, text_format));
    }
}

#endif /* (EMS_USE_DCS) */ 


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_inputbox_add_object
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b                               [?]         
 *  type                            [IN]        
 *  objData                         [?]         
 *  predefined_object_number        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
EMSTATUS gui_EMS_inputbox_add_object(UI_EMS_input_box *b, U8 type, EMSObjData *objData, U8 predefined_object_number)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSTATUS r;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    r = AddObject(b->data, type, objData, (U8) predefined_object_number);
#ifdef __MMI_HINDI_ALG__
     
    //if ((b->data->CurrentPosition.OffsetToText >= 4)  && (b->data->textBuffer[b->data->CurrentPosition.OffsetToText-1] == 0x09)
    //   && (b->data->textBuffer[b->data->CurrentPosition.OffsetToText-2] == 0x4d))
    //{
    //   b->data->textBuffer[b->data->CurrentPosition.OffsetToText]=0x20;
    //   b->data->textBuffer[b->data->CurrentPosition.OffsetToText+1]=0x00;
    //   ForwardCurrentPosition(b->data,1);
    //   b->data->listTail->OffsetToText += 2;
    //}

#endif /* __MMI_HINDI_ALG__ */ // #ifdef __MMI_HINDI_ALG__
#if(EMS_USE_DCS)
    if (r == EMS_OK)
    {
        S32 remaining_length;

        if (b->object_count == 0)
        {
            b->object_count++;
            gui_EMS_change_DCS(b);
        }
        else
        {
            b->object_count++;
        }
        if (b->UCS2_count > 0)
        {
            remaining_length = (EMS_get_remaining_length(b->data) >> 1);
        }
        else
        {
            remaining_length = EMS_get_remaining_length(b->data);
        }
        if (remaining_length < 0)
        {
            CancelCurrentPosition(b->data, 1);
            b->object_count--;
            if (b->object_count == 0)
            {
                gui_EMS_change_DCS(b);
            }
            UI_editor_play_tone_cannot_insert();
            return (EMS_NO_SPACE);
        }
    }
#endif /* (EMS_USE_DCS) */ 
    b->saved_cursor_ax = -1;
    return (r);
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_insert_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]         
 *  c       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_input_box_insert_character(UI_EMS_input_box *b, UI_character_type c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     
    //Return EMS_OK if we insert the character successfully
    U16 ch = (U16) c;
    U8 s[2] = {0, 0};
    S32 remaining_length;
    U8 ret = EMS_OK;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
    if (b->UCS2_count > 0)
    {
        remaining_length = (EMS_get_remaining_length(b->data) >> 1);
    }
    else
    {
        remaining_length = EMS_get_remaining_length(b->data);
    }
    if (remaining_length <= 0)
    {
        UI_editor_play_tone_cannot_insert();
        return EMS_NO_SPACE;
    }
     
    /* if((b->UCS2_count==0) && UI_TEST_UCS2_CHARACTER(c) && remaining_length<=344))
       {  UI_editor_play_tone_cannot_insert();
       return;
       } */
    if (b->UCS2_count == 0)
    {
        if (UI_TEST_UCS2_CHARACTER(c))
        {
            gui_EMS_input_box_remove_extension_character(b->data);
            if (EMSSetDCS(b->data, SMSAL_UCS2_DCS) != EMS_OK)
            {
                EMSSetDCS(b->data, SMSAL_DEFAULT_DCS);
                gui_EMS_input_box_insert_extension_character(b->data);
                UI_editor_play_tone_cannot_insert();
                return EMS_NO_SPACE;
            }
            else
            {
                if (EMS_get_remaining_length(b->data) < 2)
                {
                    EMSSetDCS(b->data, SMSAL_DEFAULT_DCS);
                    gui_EMS_input_box_insert_extension_character(b->data);
                    UI_editor_play_tone_cannot_insert();
                    return EMS_NO_SPACE;
                }
            }
        }
        else if (UI_TEST_GSM_EXTENDED(c))
        {
            if (EMS_get_remaining_length(b->data) < 2)
            {
                UI_editor_play_tone_cannot_insert();
                return EMS_NO_SPACE;
            }
        }
    }
    /* CSD end */

    /* If in Overwrite mode, delete the current character/object and insert the new one */
    if (b->flags & UI_EMS_INPUT_BOX_OVERWRITE_MODE)
    {
        if (gui_EMS_input_box_forward_cursor(b) == 1)
        {
            gui_EMS_cancel_current_position(b);
        }
    }
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
    s[0] = (ch & 0xff00) >> 8;
    s[1] = (ch & 0x00ff);
#else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
    s[1] = (U8) (ch >> 8);
    s[0] = (U8) ch;
#endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
    if (gui_EMS_input_box_add_string(b, s, &b->text_format) == EMS_NO_SPACE)
    {
        UI_editor_play_tone_cannot_insert();
        b->overflow_callback();
        ret = EMS_NO_SPACE;
    }
    else
    {
        b->change_callback();
    }
    
    if (b->locate_cursor_flag)
    {
        gui_EMS_input_box_locate_cursor(b);
    }
    
    b->saved_cursor_ax = -1;
    return ret;
}

#ifdef __MMI_HINDI_ALG__
U8 saved_last_skip_pos = 0;
#endif 
 
 
#if defined __MMI_LANG_HINDI__ && defined __MMI_LANG_ARABIC__
S32 HINDI_PRESENT = 0;
S32 ARABIC_PRESENT = 0;
#endif /* defined __MMI_LANG_HINDI__ && defined __MMI_LANG_ARABIC__ */ 
 


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_insert_multitap_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]         
 *  c       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
U8 gui_EMS_input_box_insert_multitap_character(UI_EMS_input_box *b, UI_character_type c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 ch = (U16) c;
    U8 s[2] = {0, 0};
    EMSTextFormat text_format;
    S32 remaining_length;

#ifdef __MMI_HINDI_ALG__
    EMSObject *bb;
    UI_character_type current_character1 = 0;
    U8 object_type_check1 = 0;
    EMSObject *o1;
    EMSPosition p1 = b->data->CurrentPosition;
    EMSData *data_chk_type1 = b->data;
    UI_character_type current_character2 = 0;
    U8 object_type_check2 = 0;

     
    U8 halant_after_object = 0;

     
    EMSObject *o2;
    EMSPosition p2 = b->data->CurrentPosition;
    EMSData *data_chk_type2 = b->data;

     
    UI_buffer_type middle_ems_position, first_cons_ems_position, second_cons_ems_position, halant_check_position;

     
    static U8 ems_cursor_pos_flag = 0;

     
    U8 total_no_of_objects = 0;

     
#endif /* __MMI_HINDI_ALG__ */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
    if (b->UCS2_count > 0)
    {
        remaining_length = (EMS_get_remaining_length(b->data) >> 1);
    }
    else
    {
        remaining_length = EMS_get_remaining_length(b->data);
    }
    if ((remaining_length <= 0) && !(b->flags & UI_EMS_INPUT_BOX_STATE_MULTITAP) &&
        !(b->flags & UI_EMS_INPUT_BOX_OVERWRITE_MODE))
    {
        UI_editor_play_tone_cannot_insert();
        return EMS_NO_SPACE;
    }
#if(0)
	if((b->UCS2_count==0) && UI_TEST_UCS2_CHARACTER(c) && (remaining_length<=344))
	{	UI_editor_play_tone_cannot_insert();
		return;
	}
#endif /* (0) */ 
    if (b->UCS2_count == 0)
    {
        if (UI_TEST_UCS2_CHARACTER(c))
        {
            if (EMSSetDCS(b->data, SMSAL_UCS2_DCS) != EMS_OK)
            {
                EMSSetDCS(b->data, SMSAL_DEFAULT_DCS);
                UI_editor_play_tone_cannot_insert();
                return EMS_NO_SPACE;
            }
            else
            {
                if (EMS_get_remaining_length(b->data) < 2)
                {
                    EMSSetDCS(b->data, SMSAL_DEFAULT_DCS);
                    UI_editor_play_tone_cannot_insert();
                    return EMS_NO_SPACE;
                }
            }
        }
        else if (UI_TEST_GSM_EXTENDED(c))
        {
            if (EMS_get_remaining_length(b->data) < 2)
            {
                UI_editor_play_tone_cannot_insert();
                return EMS_NO_SPACE;
            }
        }
    }
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
    s[0] = (ch & 0xff00) >> 8;
    s[1] = (ch & 0x00ff);
#else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
    s[1] = (U8) (ch >> 8);
    s[0] = (U8) ch;
#endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
    if (b->flags & UI_EMS_INPUT_BOX_STATE_MULTITAP)
    {
    #ifdef __MMI_HINDI_ALG__
        second_cons_ems_position = &(b->data->textBuffer[b->data->CurrentPosition.OffsetToText]);
         
        halant_check_position = &(b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 4]);
         
        if ((ems_cursor_pos_flag) && ((c == 49) || (c == 0x901) || (c == 0x902)))       /* Only when multitap input is 1 */
        {
            BackwardCurrentPosition(b->data, saved_last_skip_pos);
            ems_cursor_pos_flag = 0;
            saved_last_skip_pos = 0;
        }
         
        {

            EMSObject *object_and_halant_check;

            EMSPosition p = b->data->CurrentPosition;
			//UI_character_type c;					guoyt delete@09.11.02 for compiling warning:shadows a parameter

			EMSObject *current_object;
            //U8 r;								guoyt delete@09.11.02 for compiling warning:unused variable							

            object_and_halant_check = b->data->listHead;
            /* r=gui_EMS_input_box_get_previous_object(b->data,&p,&c,&current_object); */
            halant_after_object = gui_EMS_input_box_get_previous_object(b->data, &p, &c, &current_object);
            while (object_and_halant_check != NULL && halant_after_object == 2)
            {
                total_no_of_objects++;
                object_and_halant_check = object_and_halant_check->next;
            }
        }
        if (*halant_check_position == 0x4D && halant_after_object == 2)
        {
            check_for_halant = 1;
        }
         
    #endif /* __MMI_HINDI_ALG__ */ 
         
    #ifdef __MMI_HINDI_ALG__
        if (check_for_halant)
        {
            BackwardCurrentPosition(b->data, total_no_of_objects);
            gui_EMS_cancel_current_position(b);

        }
        else
    #endif /* __MMI_HINDI_ALG__ */ 
             
            gui_EMS_cancel_current_position(b);

      		/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-4-16 17:59 */
      		CursorPlace = 1;
		if (MMI_EMS_inputbox.UCS2_count > 0)
			MMI_EMS_inputbox.data->Reminder.totalRemainingOctet += 2;
		else
			MMI_EMS_inputbox.data->Reminder.totalRemainingOctet++;
      		/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-4-16 17:59 */
    #ifdef __MMI_HINDI_ALG__
        ems_cursor_pos_flag = 0;
        saved_last_skip_pos = 0;
        middle_ems_position = &(b->data->textBuffer[b->data->CurrentPosition.OffsetToText]);
        first_cons_ems_position = &(b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 2]);
    #endif /* __MMI_HINDI_ALG__ */ 
        text_format = b->text_format;
         
        if (gui_EMS_input_box_add_string(b, s, &text_format) != EMS_OK /*==EMS_NO_SPACE */ )
        {
            UI_editor_play_tone_cannot_insert();
            b->overflow_callback();
        }
        else
        {
        #ifdef __MMI_HINDI_ALG__
            if (!TestLastPosition(b->data, &(b->data->CurrentPosition)))
            {
                object_type_check1 = gui_EMS_input_box_get_next_object(data_chk_type1, &p1, &current_character1, &o1);
            }

            if ((UI_CHK_CONSONANT(*first_cons_ems_position)) && (UI_CHK_CONSONANT(*second_cons_ems_position))
                && (*middle_ems_position == 0x4d)
                && (*(first_cons_ems_position + 1) == 0x09) && (*(second_cons_ems_position + 1) == 0x09))
            {

                U8 skip_positions = 0;
                PU8 src_ems_pos = second_cons_ems_position;
                PU8 dest_ems_pos;

                /* dest_ems_pos = hf_get_next_cluster_p(b->data->textBuffer,src_ems_pos); */
                dest_ems_pos = hf_get_next_cluster_p(src_ems_pos, src_ems_pos);
                skip_positions = dest_ems_pos - src_ems_pos;    /* Number of moves */
                skip_positions = skip_positions >> 1;
                saved_last_skip_pos = skip_positions;
                PRINT_INFORMATION_2((MMI_TRACE_INFO,
                                     "coolsand_UI_EMS_input_box_insert_multitap_character[skip_positions]:%d  ",
                                     skip_positions));
                while (skip_positions--)
                {
                    ForwardCurrentPosition(b->data, 1);
                }
                ems_cursor_pos_flag = 1;

                if (object_type_check1 == 2)
                {
                     
                    if (saved_last_skip_pos == 1)
                    {
                        /* To shift the images on cluster completion */
                        bb = b->data->listTail;
                        do
                        {
                            if (bb->OffsetToText == b->data->CurrentPosition.Object->OffsetToText)
                            {
                                bb->OffsetToText += (saved_last_skip_pos) * 2;
                            }
                            bb = bb->prev;
                        } while (bb != NULL);

                        ForwardCurrentPosition(b->data, saved_last_skip_pos);
                        BackwardCurrentPositionSkipCR(b->data);
                    }
                    else if (saved_last_skip_pos > 1)
                    {
                        bb = b->data->listHead;
                        do
                        {
                            if (bb->OffsetToText == b->data->CurrentPosition.Object->OffsetToText)
                            {
                                bb->OffsetToText += (saved_last_skip_pos) * 2;
                            }
                            bb = bb->next;
                        } while (bb != NULL);

                        ForwardCurrentPosition(b->data, saved_last_skip_pos);
                        if (b->data->listHead == b->data->listTail)
                        {
                            BackwardCurrentPositionSkipCR(b->data);
                        }
                        else
                        {
                            BackwardCurrentPositionSkipCR(b->data);
                            BackwardCurrentPositionSkipCR(b->data);
                        }
                    }
                }

            }
             
            if (check_for_halant)
            {

                ForwardCurrentPosition(b->data, total_no_of_objects);
                check_for_halant = 0;
            }
             
        #endif /* __MMI_HINDI_ALG__ */ 
            b->change_callback();
        }
    }
    else
    {
    #ifdef __MMI_HINDI_ALG__
        ems_cursor_pos_flag = 0;
        saved_last_skip_pos = 0;
        second_cons_ems_position = &(b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 2]);

        object_type_check2 = gui_EMS_input_box_get_previous_object(data_chk_type2, &p2, &current_character2, &o2);
         
        if ((*second_cons_ems_position == 0x4d) && (object_type_check2 == 2))
        {
            check_for_halant = 1;
        }
         
         
        if ((object_type_check2 == 2) && (*second_cons_ems_position == 0x4d) && UI_CHK_CONSONANT(s[0]) && s[1] == 0x09)
             
        {
            {
                U8 skip_positions = 0;
                PU8 src_ems_pos = second_cons_ems_position;
                PU8 dest_ems_pos;

                dest_ems_pos = hf_get_next_cluster_p(src_ems_pos, src_ems_pos);
                skip_positions = dest_ems_pos - src_ems_pos;
                skip_positions = skip_positions >> 1;

                /* To shift the images on cluster completion */
                bb = b->data->listHead;
                do
                {
                    if (bb->OffsetToText == b->data->CurrentPosition.Object->OffsetToText)
                    {
                        bb->OffsetToText += (skip_positions) * 2;
                    }
                    bb = bb->next;
                } while (bb != NULL);
                 
                //ForwardCurrentPosition(b->data,1);
                 
            }
        }
    #endif /* __MMI_HINDI_ALG__ */ 
        if (b->flags & UI_EMS_INPUT_BOX_OVERWRITE_MODE)
        {
            if (gui_EMS_input_box_forward_cursor(b) == 1)
            {
                gui_EMS_cancel_current_position(b);
                text_format = b->text_format;
                 
                if (gui_EMS_input_box_add_string(b, s, &text_format) != EMS_OK /*==EMS_NO_SPACE */ )
                {
                    UI_editor_play_tone_cannot_insert();
                    b->overflow_callback();
                }
                else
                {
                    b->change_callback();
                }
            }
             
            b->flags |= UI_EMS_INPUT_BOX_STATE_MULTITAP;

        }
        else
        {
             
            if (gui_EMS_input_box_add_string(b, s, &b->text_format) != EMS_OK /*==EMS_NO_SPACE */ )
            {
                b->overflow_callback();
            }
            else
            {
                b->change_callback();
                 
                b->flags |= UI_EMS_INPUT_BOX_STATE_MULTITAP;
                /* CSD end */
            }
        }
         
        //b->flags|=UI_EMS_INPUT_BOX_STATE_MULTITAP;
        //CSD end
    }
#ifdef __MMI_HINDI_ALG__
    if (TestLastPosition(b->data, &(b->data->CurrentPosition)))
    {
        b->data->textBuffer[b->data->CurrentPosition.OffsetToText] = 0;
        b->data->textBuffer[b->data->CurrentPosition.OffsetToText + 1] = 0;
    }
#endif /* __MMI_HINDI_ALG__ */ 
    gui_EMS_input_box_locate_cursor(b);
    b->saved_cursor_ax = -1;

	return 0;
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_confirm_multitap_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_confirm_multitap_character(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    b->flags &= ~UI_EMS_INPUT_BOX_STATE_MULTITAP;
}

extern EMSTextFormat wgui_EMS_text_format;


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_delete_all
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_delete_all(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
    ResetCurrentPosition(b->data);
     
#if 0
#endif /* 0 */ 
    /* CSD end */
    /* Probably a better way of doing this is to have a EMS library API  */
    GoToEndPosition(b->data);
     
    memset(b->data->textBuffer, 0, b->data->CurrentPosition.OffsetToText);
     
#if defined(__MMI_MESSAGES_CONCAT_SMS__)
    CancelCurrentPosition(b->data, (U16) (b->data->Reminder.maxUsableLenPerSeg * mmi_msg_get_seg_num()));
#else 
    CancelCurrentPosition(b->data, (U16) (EMS_DEFAULT_DCS_ALLOCATED_LENGTH));
#endif 
    gui_EMS_input_box_set_text_format_cursor_position(b);
    gui_EMS_input_box_locate_cursor(b);
    b->UCS2_count = 0;
    b->object_count = 0;
    /* CSD Joy added for clear all, 1228 */
    b->GSM_ext_count = 0;
    /* CSD end */
    gui_EMS_change_DCS(b);
    b->saved_cursor_ax = -1;
	//ResetTextFormat(&wgui_EMS_text_format);  //dyj del 20060515
}

#if (EMS_USE_CRLF_COMBINATION || EMS_USE_ESC_COMBINATION)


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_delete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_delete(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   TRACE_GDI_FUNCTION();
	
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
    if (UI_TEST_CR_CHARACTER(b->data->textBuffer[b->data->CurrentPosition.OffsetToText]))
    {
        if (ForwardCurrentPosition(b->data, 1) == 1)
        {
            if (!UI_TEST_LF_CHARACTER(b->data->textBuffer[b->data->CurrentPosition.OffsetToText]))
            {
                BackwardCurrentPosition(b->data, 1);
            }
        }
    }
    if (gui_EMS_input_box_forward_cursor(b) == 1)
    {
        gui_EMS_cancel_current_position(b);
        gui_EMS_input_box_set_text_format_cursor_position(b);
        gui_EMS_input_box_locate_cursor(b);
    }
    else
    {
        UI_editor_play_tone_cannot_change();
    }
    b->saved_cursor_ax = -1;
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_previous
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_previous(UI_EMS_input_box *b)
{
#if defined(__MMI_LANG_VIETNAMESE__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition mapped_cursor_position;
    UI_character_type current_character, dummy_c = 0;
    EMSData *data = b->data;
    EMSObject *current_object;
    U8 object_type;
    viet_tone_mark tone_mark = VIET_TONE_NONE;
    viet_vowel_letter viet_vowel = VIET_VOWEL_NONE;
#endif /* defined(__MMI_LANG_VIETNAMESE__) */ 

#if defined(__MMI_LANG_THAI__)
    EMSPosition mapped_cursor_position_thai;
    UI_character_type current_character_thai, dummy_c_thai = 0;
    EMSData *data_thai = b->data;
    EMSObject *current_object_thai;
    U8 object_type_thai;
    UI_character_type pre_one_char = 0;
    UI_character_type pre_two_char = 0;
    UI_character_type pre_three_char = 0;
#endif /* defined(__MMI_LANG_THAI__) */ 


#if 0//defined(__MMI_LANG_ARABIC__)/*Delete by panxu. This is for the shape engine2.*/
    EMSPosition mapped_cursor_position_arabic;
    UI_character_type current_character_arabic;
    EMSData *data_arabic = b->data;
    EMSObject *current_object_arabic;
    U8 object_type_arabic;
#endif /* defined(__MMI_LANG_ARABIC__) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* CSD End */

#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
#if defined(__MMI_LANG_VIETNAMESE__)
    EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
    object_type = gui_EMS_input_box_get_previous_object(data, &mapped_cursor_position, &current_character, &current_object);
    if (1 == object_type)
    {
        if ((current_character >= 0x0300) && (current_character <= 0x0323))
        {
            object_type = gui_EMS_input_box_get_previous_object(data, &mapped_cursor_position, &dummy_c, &current_object);      /* get next character */
            if (1 == object_type)
            {
                viet_vowel = mmi_viet_vowel_letter(dummy_c);
                if (VIET_VOWEL_NONE != viet_vowel)
                {
                    tone_mark = mmi_viet_tone_mark(current_character);
                    if (VIET_TONE_NONE != tone_mark)
                    {
                        current_character = mmi_viet_combine_vowel_tone(viet_vowel, tone_mark);
                        BackwardCurrentPositionSkipCR(b->data);
                    }
                    else
                    {
                        object_type = gui_EMS_input_box_get_next_object(data, &mapped_cursor_position, &dummy_c, &current_object);
                    }
                }
                else
                {
                    object_type = gui_EMS_input_box_get_next_object(data, &mapped_cursor_position, &dummy_c, &current_object);
                }
            }
            else if (0 != object_type)
            {
                object_type = gui_EMS_input_box_get_next_object(data, &mapped_cursor_position, &dummy_c, &current_object);
            }
        }
    }
#endif /* defined(__MMI_LANG_VIETNAMESE__) */ 

#if defined(__MMI_LANG_THAI__)
    EMS_map_position(data_thai, &data_thai->CurrentPosition, &mapped_cursor_position_thai);
    object_type_thai = gui_EMS_input_box_get_previous_object(
                        data_thai,
                        &mapped_cursor_position_thai,
                        &current_character_thai,
                        &current_object_thai);
    if (object_type_thai == 1 && isThaiChar(current_character_thai))    /* object type == 1 --> character */
    {
        object_type_thai = gui_EMS_input_box_get_previous_object(
                            data_thai,
                            &mapped_cursor_position_thai,
                            &pre_one_char,
                            &current_object_thai);
        if (object_type_thai == 1 && isThaiChar(pre_one_char))
        {
            if (!RequireToMoveCursor(pre_one_char, current_character_thai))
            {
                BackwardCurrentPositionSkipCR(b->data);
                object_type_thai = gui_EMS_input_box_get_previous_object(
                                    data_thai,
                                    &mapped_cursor_position_thai,
                                    &pre_two_char,
                                    &current_object_thai);
                if (object_type_thai == 1 && isThaiChar(pre_two_char))
                {
                    if (!RequireToMoveCursor(pre_two_char, pre_one_char))
                    {
                        BackwardCurrentPositionSkipCR(b->data);
                        object_type_thai = gui_EMS_input_box_get_previous_object(
                                            data_thai,
                                            &mapped_cursor_position_thai,
                                            &pre_three_char,
                                            &current_object_thai);
                        if (object_type_thai == 1 && isThaiChar(pre_three_char))
                        {
                            if (!RequireToMoveCursor(pre_three_char, pre_two_char))
                            {
                                BackwardCurrentPositionSkipCR(b->data);
                            }
                            else
                            {
                                object_type_thai = gui_EMS_input_box_get_next_object(
                                                    data_thai,
                                                    &mapped_cursor_position_thai,
                                                    &dummy_c_thai,
                                                    &current_object_thai);
                            }
                        }
                        else
                        {
                            object_type_thai = gui_EMS_input_box_get_next_object(
                                                data_thai,
                                                &mapped_cursor_position_thai,
                                                &dummy_c_thai,
                                                &current_object_thai);
                        }
                    }
                    else
                    {
                        object_type_thai = gui_EMS_input_box_get_next_object(
                                            data_thai,
                                            &mapped_cursor_position_thai,
                                            &dummy_c_thai,
                                            &current_object_thai);
                    }
                }
                else
                {
                    object_type_thai = gui_EMS_input_box_get_next_object(
                                        data_thai,
                                        &mapped_cursor_position_thai,
                                        &dummy_c_thai,
                                        &current_object_thai);
                }
            }
            else
            {
                object_type_thai = gui_EMS_input_box_get_next_object(
                                    data_thai,
                                    &mapped_cursor_position_thai,
                                    &dummy_c_thai,
                                    &current_object_thai);
            }
        }
        else
        {
            object_type_thai = gui_EMS_input_box_get_next_object(
                                data_thai,
                                &mapped_cursor_position_thai,
                                &dummy_c_thai,
                                &current_object_thai);
        }
    }

#endif /* defined(__MMI_LANG_THAI__) */ 


#if 0 //defined(__MMI_LANG_ARABIC__) /*Delete by panxu. This is for the shape engine2.*/
    mapped_cursor_position_arabic.OffsetToText = data_arabic->CurrentPosition.OffsetToText;
    mapped_cursor_position_arabic.Object = data_arabic->CurrentPosition.Object;
    object_type_arabic = gui_EMS_input_box_get_previous_object(
                            data_arabic,
                            &mapped_cursor_position_arabic,
                            &current_character_arabic,
                            &current_object_arabic);
    if (1 == object_type_arabic)
    {
        if (IS_ARABIC_CHARACTER(current_character_arabic))
        {
            U16 idx;

            idx = gui_get_ems_text_actual_idx(b, data_arabic->CurrentPosition.OffsetToText) / 2 - 1;

            while (idx > 0)
            {
                if (OrgToSha[idx] == OrgToSha[idx - 1])
                {
                    BackwardCurrentPositionSkipCR(b->data);
                }
                else
                {
                    break;
                }
                idx--;
            }
        }
    }
#endif /* defined(__MMI_LANG_ARABIC__) */ 


    if (BackwardCurrentPositionSkipCR(b->data) != 1)
    {
        UI_editor_play_tone_cannot_navigate();
    }
    else
    {
        gui_EMS_input_box_set_text_format_cursor_position(b);
        gui_EMS_input_box_locate_cursor(b);
        b->saved_cursor_ax = -1;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_forward_cursor
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_input_box_forward_cursor(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TRACE_EF(1, "offset = %d, textLength = %d", b->data->CurrentPosition.OffsetToText, b->data->textLength);
    if (b->data->CurrentPosition.OffsetToText < (b->data->textLength))
    {    
        if (ForwardCurrentPositionSkipCR(b->data) == 1)
            //if(ForwardCurrentPositionSkipCR(b->data)>=1)
             
        {
            return (1);
        }
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_next
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_next(UI_EMS_input_box *b)
{
#if defined(__MMI_LANG_VIETNAMESE__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition mapped_cursor_position;
    UI_character_type current_character, dummy_c = 0;
    EMSData *data = b->data;
    EMSObject *current_object;
    U8 object_type;
    viet_tone_mark tone_mark = VIET_TONE_NONE;
    viet_vowel_letter viet_vowel = VIET_VOWEL_NONE;
#endif /* defined(__MMI_LANG_VIETNAMESE__) */ 

#if defined(__MMI_LANG_THAI__)
    EMSPosition mapped_cursor_position_thai;
    UI_character_type current_character_thai, dummy_c_thai = 0;
    EMSData *data_thai = b->data;
    EMSObject *current_object_thai;
    U8 object_type_thai;
    UI_character_type next_one_char = 0;
    UI_character_type next_two_char = 0;
    UI_character_type next_three_char = 0;
#endif /* defined(__MMI_LANG_THAI__) */ 


#if 0//defined(__MMI_LANG_ARABIC__)
    EMSPosition mapped_cursor_position_arabic;
    UI_character_type current_character_arabic;
    EMSData *data_arabic = b->data;
    EMSObject *current_object_arabic;
    U8 object_type_arabic;
#endif /* defined(__MMI_LANG_ARABIC__) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* CSD End */

#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
#if defined(__MMI_LANG_VIETNAMESE__)
    EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
    object_type = gui_EMS_input_box_get_next_object(data, &mapped_cursor_position, &current_character, &current_object);
    if (1 == object_type)
    {
        if ((current_character > 0x0040) && (current_character < 0x01B1))
        {
            object_type = gui_EMS_input_box_get_next_object(data, &mapped_cursor_position, &dummy_c, &current_object);  /* get next character */
            if (1 == object_type)
            {
                tone_mark = mmi_viet_tone_mark(dummy_c);
                if (VIET_TONE_NONE != tone_mark)
                {
                    viet_vowel = mmi_viet_vowel_letter(current_character);
                    if (VIET_VOWEL_NONE != viet_vowel)
                    {
                        current_character = mmi_viet_combine_vowel_tone(viet_vowel, tone_mark);
                        gui_EMS_input_box_forward_cursor(b);
                    }
                    else
                    {
                        object_type = gui_EMS_input_box_get_previous_object(
                                        data,
                                        &mapped_cursor_position,
                                        &dummy_c,
                                        &current_object);
                    }
                }
                else
                {
                    object_type = gui_EMS_input_box_get_previous_object(data, &mapped_cursor_position, &dummy_c, &current_object);
                }
            }
            else if (0 != object_type)
            {
                object_type = gui_EMS_input_box_get_next_object(data, &mapped_cursor_position, &dummy_c, &current_object);
            }
        }
    }

#endif /* defined(__MMI_LANG_VIETNAMESE__) */ 

#if defined(__MMI_LANG_THAI__)
    EMS_map_position(data_thai, &data_thai->CurrentPosition, &mapped_cursor_position_thai);
    object_type_thai = gui_EMS_input_box_get_next_object(
                        data_thai,
                        &mapped_cursor_position_thai,
                        &current_character_thai,
                        &current_object_thai);
    if (object_type_thai == 1 && isThaiChar(current_character_thai))    /* object type == 1 --> character */
    {
        object_type_thai = gui_EMS_input_box_get_next_object(
                            data_thai,
                            &mapped_cursor_position_thai,
                            &next_one_char,
                            &current_object_thai);
        if (object_type_thai == 1 && isThaiChar(next_one_char))
        {
            if (!RequireToMoveCursor(current_character_thai, next_one_char))
            {
                gui_EMS_input_box_forward_cursor(b);
                object_type_thai = gui_EMS_input_box_get_next_object(
                                    data_thai,
                                    &mapped_cursor_position_thai,
                                    &next_two_char,
                                    &current_object_thai);
                if (object_type_thai == 1 && isThaiChar(next_two_char))
                {
                    if (!RequireToMoveCursor(next_one_char, next_two_char))
                    {
                        gui_EMS_input_box_forward_cursor(b);
                        object_type_thai = gui_EMS_input_box_get_next_object(
                                            data_thai,
                                            &mapped_cursor_position_thai,
                                            &next_three_char,
                                            &current_object_thai);
                        if (object_type_thai == 1 && isThaiChar(next_three_char))
                        {
                            if (!RequireToMoveCursor(next_two_char, next_three_char))
                            {
                                gui_EMS_input_box_forward_cursor(b);
                            }
                            else
                            {
                                object_type_thai = gui_EMS_input_box_get_previous_object(
                                                    data_thai,
                                                    &mapped_cursor_position_thai,
                                                    &dummy_c_thai,
                                                    &current_object_thai);
                            }
                        }
                        else
                        {
                            object_type_thai = gui_EMS_input_box_get_previous_object(
                                                data_thai,
                                                &mapped_cursor_position_thai,
                                                &dummy_c_thai,
                                                &current_object_thai);
                        }
                    }
                    else
                    {
                        object_type_thai = gui_EMS_input_box_get_previous_object(
                                            data_thai,
                                            &mapped_cursor_position_thai,
                                            &dummy_c_thai,
                                            &current_object_thai);
                    }
                }
                else
                {
                    object_type_thai = gui_EMS_input_box_get_previous_object(
                                        data_thai,
                                        &mapped_cursor_position_thai,
                                        &dummy_c_thai,
                                        &current_object_thai);
                }
            }
            else
            {
                object_type_thai = gui_EMS_input_box_get_previous_object(
                                    data_thai,
                                    &mapped_cursor_position_thai,
                                    &dummy_c_thai,
                                    &current_object_thai);
            }
        }
        else
        {
            object_type_thai = gui_EMS_input_box_get_previous_object(
                                data_thai,
                                &mapped_cursor_position_thai,
                                &dummy_c_thai,
                                &current_object_thai);
        }
    }

#endif /* defined(__MMI_LANG_THAI__) */ 



#if 0//defined(__MMI_LANG_ARABIC__)
    mapped_cursor_position_arabic.OffsetToText = data_arabic->CurrentPosition.OffsetToText;
    mapped_cursor_position_arabic.Object = data_arabic->CurrentPosition.Object;
    object_type_arabic = gui_EMS_input_box_get_next_object(
                            data_arabic,
                            &mapped_cursor_position_arabic,
                            &current_character_arabic,
                            &current_object_arabic);
    if (1 == object_type_arabic)
    {
        if (IS_ARABIC_CHARACTER(current_character_arabic))
        {
            U16 idx;
            U16 offset = 0;

            if (mapped_cursor_position_arabic.Object != NULL)
            {
                offset += 2;
            }
            idx = gui_get_ems_text_actual_idx(b, data_arabic->CurrentPosition.OffsetToText + offset);
            if (idx != 0)
            {
                idx = idx / 2 - 1;
            }

            while (idx < unshaped_text_len)
            {
                if (OrgToSha[idx] == OrgToSha[idx + 1])
                {
                    gui_EMS_input_box_forward_cursor(b);
                }
                else
                {
                    break;
                }
                idx++;
            }
        }
    }
#endif /* defined(__MMI_LANG_ARABIC__) */ 


    if (gui_EMS_input_box_forward_cursor(b) != 1)
    {
        UI_editor_play_tone_cannot_navigate();
    }
    else
    {
        gui_EMS_input_box_set_text_format_cursor_position(b);
        gui_EMS_input_box_locate_cursor(b);
        b->saved_cursor_ax = -1;
    }
}

#else /* (EMS_USE_CRLF_COMBINATION || EMS_USE_ESC_COMBINATION) */ 


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_delete
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_delete(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
    if (gui_EMS_input_box_forward_cursor(b) == 1)
    {
        gui_EMS_cancel_current_position(b);
        gui_EMS_input_box_set_text_format_cursor_position(b);
        gui_EMS_input_box_locate_cursor(b);
    }
    else
    {
        UI_editor_play_tone_cannot_change();
    }
    b->saved_cursor_ax = -1;
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_previous
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_previous(UI_EMS_input_box *b)
{
#if defined(__MMI_LANG_VIETNAMESE__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition mapped_cursor_position;
    UI_character_type current_character, dummy_c = 0;
    EMSData *data = b->data;
    EMSObject *current_object;
    U8 object_type;
    viet_tone_mark tone_mark = VIET_TONE_NONE;
    viet_vowel_letter viet_vowel = VIET_VOWEL_NONE;
#endif /* defined(__MMI_LANG_VIETNAMESE__) */ 


#if defined(__MMI_LANG_ARABIC__)||defined(__MMI_LANG_PERSIAN__)
    EMSPosition mapped_cursor_position_arabic;
    UI_character_type current_character_arabic, dummy_c_arabic = 0;
    EMSData *data_arabic = b->data;
    EMSObject *current_object_arabic;
    U8 object_type_arabic;
    S32 saved_text_len = 0;
#endif /* defined(__MMI_LANG_ARABIC__) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
#if defined(__MMI_LANG_VIETNAMESE__)
    EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
    object_type = gui_EMS_input_box_get_previous_object(data, &mapped_cursor_position, &current_character, &current_object);
    if (1 == object_type)
    {
        if ((current_character >= 0x0300) && (current_character <= 0x0323))
        {
            object_type = gui_EMS_input_box_get_previous_object(data, &mapped_cursor_position, &dummy_c, &current_object);      /* get next character */
            if (1 == object_type)
            {
                viet_vowel = mmi_viet_vowel_letter(dummy_c);
                if (VIET_VOWEL_NONE != viet_vowel)
                {
                    tone_mark = mmi_viet_tone_mark(current_character);
                    if (VIET_TONE_NONE != tone_mark)
                    {
                        current_character = mmi_viet_combine_vowel_tone(viet_vowel, tone_mark);
                        BackwardCurrentPosition(b->data, 1);
                    }
                    else
                    {
                        object_type = gui_EMS_input_box_get_next_object(data, &mapped_cursor_position, &dummy_c, &current_object);
                    }
                }
                else
                {
                    object_type = gui_EMS_input_box_get_next_object(data, &mapped_cursor_position, &dummy_c, &current_object);
                }
            }
            else if (0 != object_type)
            {
                object_type = gui_EMS_input_box_get_next_object(data, &mapped_cursor_position, &dummy_c, &current_object);
            }
        }
    }
#endif /* defined(__MMI_LANG_VIETNAMESE__) */ 

#if defined(__MMI_LANG_ARABIC__)||defined(__MMI_LANG_PERSIAN__)
    mapped_cursor_position_arabic.OffsetToText = data_arabic->CurrentPosition.OffsetToText;
    mapped_cursor_position_arabic.Object = data_arabic->CurrentPosition.Object;
    object_type_arabic = gui_EMS_input_box_get_previous_object(
                            data_arabic,
                            &mapped_cursor_position_arabic,
                            &current_character_arabic,
                            &current_object_arabic);
    if (1 == object_type_arabic)
    {
        if (IS_ARABIC_CHARACTER(current_character_arabic))
        {
            U16 idx;

            idx = gui_get_ems_text_actual_idx(b, data_arabic->CurrentPosition.OffsetToText) / 2 - 1;

            while (idx > 0)
            {
                if (OrgToSha[idx] == OrgToSha[idx - 1])
                {
                    BackwardCurrentPositionSkipCR(b->data);
                }
                else
                {
                    break;
                }
                idx--;
            }
        }
    }
#endif /* defined(__MMI_LANG_ARABIC__) */ 


    if (BackwardCurrentPosition(b->data, 1) != 1)
    {
        UI_editor_play_tone_cannot_navigate();
    }
    gui_EMS_input_box_set_text_format_cursor_position(b);
    gui_EMS_input_box_locate_cursor(b);
    b->saved_cursor_ax = -1;
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_forward_cursor
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_input_box_forward_cursor(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (b->data->CurrentPosition.OffsetToText < (b->data->textLength))
    {
        if (ForwardCurrentPosition(b->data, 1) == 1)
        {
            return (1);
        }
    }
    return (0);
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_next
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_next(UI_EMS_input_box *b)
{
#if defined(__MMI_LANG_VIETNAMESE__)
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition mapped_cursor_position;
    UI_character_type current_character, dummy_c = 0;
    EMSData *data = b->data;
    EMSObject *current_object;
    U8 object_type;
    viet_tone_mark tone_mark = VIET_TONE_NONE;
    viet_vowel_letter viet_vowel = VIET_VOWEL_NONE;
#endif /* defined(__MMI_LANG_VIETNAMESE__) */ 


#if defined(__MMI_LANG_ARABIC__)||defined(__MMI_LANG_PERSIAN__)
    EMSPosition mapped_cursor_position_arabic;
    UI_character_type current_character_arabic, dummy_c_arabic = 0;
    EMSData *data_arabic = b->data;
    EMSObject *current_object_arabic;
    U8 object_type_arabic;
    S32 saved_text_len = 0;
#endif /* defined(__MMI_LANG_ARABIC__) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
#if defined(__MMI_LANG_VIETNAMESE__)
    EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
    object_type = gui_EMS_input_box_get_next_object(data, &mapped_cursor_position, &current_character, &current_object);
    if (1 == object_type)
    {
        if ((current_character > 0x0040) && (current_character < 0x01B1))
        {
            object_type = gui_EMS_input_box_get_next_object(data, &mapped_cursor_position, &dummy_c, &current_object);  /* get next character */
            if (1 == object_type)
            {
                tone_mark = mmi_viet_tone_mark(dummy_c);
                if (VIET_TONE_NONE != tone_mark)
                {
                    viet_vowel = mmi_viet_vowel_letter(current_character);
                    if (VIET_VOWEL_NONE != viet_vowel)
                    {
                        current_character = mmi_viet_combine_vowel_tone(viet_vowel, tone_mark);
                        gui_EMS_input_box_forward_cursor(b);
                    }
                    else
                    {
                        object_type = gui_EMS_input_box_get_previous_object(
                                        data,
                                        &mapped_cursor_position,
                                        &dummy_c,
                                        &current_object);
                    }
                }
                else
                {
                    object_type = gui_EMS_input_box_get_previous_object(data, &mapped_cursor_position, &dummy_c, &current_object);
                }
            }
            else if (0 != object_type)
            {
                object_type = gui_EMS_input_box_get_next_object(data, &mapped_cursor_position, &dummy_c, &current_object);
            }
        }
    }
#endif /* defined(__MMI_LANG_VIETNAMESE__) */ 


#if defined(__MMI_LANG_ARABIC__)||defined(__MMI_LANG_PERSIAN__)
    mapped_cursor_position_arabic.OffsetToText = data_arabic->CurrentPosition.OffsetToText;
    mapped_cursor_position_arabic.Object = data_arabic->CurrentPosition.Object;
    object_type_arabic = gui_EMS_input_box_get_next_object(
                            data_arabic,
                            &mapped_cursor_position_arabic,
                            &current_character_arabic,
                            &current_object_arabic);
    if (1 == object_type_arabic)
    {
        if (IS_ARABIC_CHARACTER(current_character_arabic))
        {
            U16 idx;
            U16 offset = 0;

            if (mapped_cursor_position_arabic.Object != NULL)
            {
                offset += 2;
            }
            idx = gui_get_ems_text_actual_idx(b, data_arabic->CurrentPosition.OffsetToText + offset);
            if (idx != 0)
            {
                idx = idx / 2 - 1;
            }

            while (idx < unshaped_text_len)
            {
                if (OrgToSha[idx] == OrgToSha[idx + 1])
                {
                    gui_EMS_input_box_forward_cursor(b);
                }
                else
                {
                    break;
                }
                idx++;
            }
        }
    }
#endif /* defined(__MMI_LANG_ARABIC__) */ 


    if (gui_EMS_input_box_forward_cursor(b) != 1)
    {
        UI_editor_play_tone_cannot_navigate();
    }
    gui_EMS_input_box_set_text_format_cursor_position(b);
    gui_EMS_input_box_locate_cursor(b);
    b->saved_cursor_ax = -1;
}

#endif /* (EMS_USE_CRLF_COMBINATION || EMS_USE_ESC_COMBINATION) */ 


/*****************************************************************************
 * FUNCTION
 *  gui_move_EMS_input_box
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]         
 *  x       [IN]        
 *  y       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_move_EMS_input_box(UI_EMS_input_box *b, S32 x, S32 y)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    b->x = x;
    b->y = y;
    gui_move_vertical_scrollbar(&b->vbar, b->x + b->width - b->vbar.width - 1, b->y + 1);
}


/*****************************************************************************
 * FUNCTION
 *  gui_resize_EMS_input_box
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b           [?]         
 *  width       [IN]        
 *  height      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_resize_EMS_input_box(UI_EMS_input_box *b, S32 width, S32 height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    b->width = width;
    b->height = height;
    gui_move_vertical_scrollbar(&b->vbar, b->x + b->width - b->vbar.width - 1, b->y + 1);
    gui_resize_vertical_scrollbar(&b->vbar, b->vbar.width, b->height - 2);
    gui_EMS_input_box_locate_cursor(b);
    b->saved_cursor_ax = -1;
}

/* Returns 0: none, 1: character, 2:object   */


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_get_next_object
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data                    [?]         
 *  current_position        [?]         
 *  c                       [?]         
 *  o                       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_input_box_get_next_object(
    EMSData *data,
    EMSPosition *current_position,
    UI_character_type *c,
    EMSObject **o)
{
#if(0)  /* Error in code: Do not enable this block   */
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	byte		return_type=0;

	if((current_position->Object!=NULL) && (current_position->Object->OffsetToText==current_position->OffsetToText))
	{
		if((current_position->Object->next!=NULL) && (current_position->Object->OffsetToText==current_position->Object->next->OffsetToText))
		{
			return_type=2;
			*o=current_position->Object;
			current_position->Object=current_position->Object->next;
		}
		else if(current_position->OffsetToText<=EMS_DATA_END_OFFSET(data->textLength))
		{
			return_type=2;
			*o=current_position->Object;
			current_position->Object=current_position->Object->next;
		}
	}
	else if(current_position->OffsetToText<EMS_DATA_END_OFFSET(data->textLength))
	{
		return_type=1;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
		*c=(data->textBuffer[current_position->OffsetToText+1]|(data->textBuffer[current_position->OffsetToText]<<8));
#else
		*c=(data->textBuffer[current_position->OffsetToText]|(data->textBuffer[current_position->OffsetToText+1]<<8));
#endif
		current_position->OffsetToText+=2;
	}
	return(return_type);
#else /* (0) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (EMSGetNextObject(data, current_position, c, o));
#endif /* (0) */ 
}

 
/* Returns 0: none, 1: character, 2:object   */


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_get_next_object_for_input_method
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data                    [?]         
 *  current_position        [?]         
 *  c                       [?]         
 *  o                       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_input_box_get_next_object_for_input_method(
    EMSData *data,
    EMSPosition *current_position,
    UI_character_type *c,
    EMSObject **o)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 return_type = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (current_position->Object != NULL)
    {
        if (current_position->Object->next != NULL &&
            current_position->Object->next->OffsetToText == current_position->OffsetToText)
        {
            return_type = 2;
            *o = current_position->Object->next;
            current_position->Object = current_position->Object->next;
        }
        else if (current_position->OffsetToText < EMS_DATA_END_OFFSET(data->textLength))
        {
            return_type = 1;
        #if(EMS_BYTE_ORDER_BIG_ENDIAN)
            *c = (data->
                  textBuffer[current_position->OffsetToText +
                             1] | (data->textBuffer[current_position->OffsetToText] << 8));
        #else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
            *c = (data->
                  textBuffer[current_position->OffsetToText] | (data->
                                                                textBuffer[current_position->OffsetToText + 1] << 8));
        #endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
            current_position->OffsetToText += 2;
        }
    }
    else if (data->listHead != NULL)
    {
        if (data->listHead->OffsetToText == current_position->OffsetToText)
        {
            return_type = 2;
            *o = data->listHead;
            current_position->Object = data->listHead;
        }
        else if (current_position->OffsetToText < EMS_DATA_END_OFFSET(data->textLength))
        {
            return_type = 1;
        #if(EMS_BYTE_ORDER_BIG_ENDIAN)
            *c = (data->
                  textBuffer[current_position->OffsetToText +
                             1] | (data->textBuffer[current_position->OffsetToText] << 8));
        #else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
            *c = (data->
                  textBuffer[current_position->OffsetToText] | (data->
                                                                textBuffer[current_position->OffsetToText + 1] << 8));
        #endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
            current_position->OffsetToText += 2;
        }

    }
    else if (current_position->OffsetToText < EMS_DATA_END_OFFSET(data->textLength))
    {
        return_type = 1;
    #if(EMS_BYTE_ORDER_BIG_ENDIAN)
        *c = (data->
              textBuffer[current_position->OffsetToText + 1] | (data->textBuffer[current_position->OffsetToText] << 8));
    #else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
        *c = (data->
              textBuffer[current_position->OffsetToText] | (data->textBuffer[current_position->OffsetToText + 1] << 8));
    #endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
        current_position->OffsetToText += 2;
    }
    return (return_type);
}

/* CSD end */

/* Returns 0: none, 1: character, 2:object   */


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_get_previous_object
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data                    [?]         
 *  current_position        [?]         
 *  c                       [?]         
 *  o                       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_input_box_get_previous_object(
    EMSData *data,
    EMSPosition *current_position,
    UI_character_type *c,
    EMSObject **o)
{
#if(1)  

	byte		return_type=0;

     
	if((current_position->Object!=NULL) && (current_position->Object->OffsetToText==current_position->OffsetToText))
	{
		if((current_position->Object->prev!=NULL) && (current_position->Object->OffsetToText==current_position->Object->prev->OffsetToText))
		{
			return_type=2;
			*o=current_position->Object;
			current_position->Object=current_position->Object->prev;
		}
		else
		{
			return_type=2;
			*o=current_position->Object;
			current_position->Object=current_position->Object->prev;
		}
	}
    else if(current_position->OffsetToText <= EMS_DATA_END_OFFSET(data->textLength) && current_position->OffsetToText >= 2)
	{
		return_type=1;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
        *c=(data->textBuffer[current_position->OffsetToText-1]|(data->textBuffer[current_position->OffsetToText-2]<<8));
#else
        *c=(data->textBuffer[current_position->OffsetToText-2]|(data->textBuffer[current_position->OffsetToText-1]<<8));
#endif
		current_position->OffsetToText-=2;
	}
    //CSD end
	return(return_type);
#else
	return(EMSGetPreviousObject(data,current_position,c,o));
#endif
}

 
#if 0
#if(UI_BLINKING_CURSOR_SUPPORT)
#else
#endif
#if(UI_BLINKING_CURSOR_SUPPORT)
#else
#endif
#if(UI_BLINKING_CURSOR_SUPPORT)
#endif
#endif /* 0 */ 


/*****************************************************************************
 * FUNCTION
 *  gui_show_EMS_input_box_cursor
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b           [?]         
 *  x           [IN]        
 *  y           [IN]        
 *  width       [IN]        
 *  height      [IN]        
 *  flags       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_EMS_input_box_cursor(UI_EMS_input_box *b, S32 x, S32 y, S32 width, S32 height, U32 flags)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 cursor_type = (U8) (flags & 0x0f);
    S32 x1, y1, x2, y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (cursor_type == 1)
    {
        x1 = x - 1;
        y1 = y;
        x2 = x;
        y2 = y + height - 1;
        if (x1 < (b->x + 1))
        {
            x1 = b->x + 1;
        }
        if (y1 < (b->y + 1))
        {
            y1 = b->y + 1;
        }
        if (x2 > (b->x + b->width - 2))
        {
            x2 = b->x + b->width - 2;
        }
        if (y2 > (b->y + b->height - 2))
        {
            y2 = b->y + b->height - 2;
        }
    #if(UI_BLINKING_CURSOR_SUPPORT)
        UI_inputbox_show_cursor(x1, y1, x2, y2, b->cursor_color);
    #else 
        gui_fill_rectangle(x1, y1, x2, y2, b->cursor_color);
    #endif 
    }
    else if (cursor_type == 2)
    {
        x1 = x - 2;
        y1 = y;
        x2 = x - 1;
        y2 = y + height - 1;
        if (x1 < (b->x + 1))
        {
            x1 = b->x + 1;
        }
        if (y1 < (b->y + 1))
        {
            y1 = b->y + 1;
        }
        if (x2 > (b->x + b->width - 2))
        {
            x2 = b->x + b->width - 2;
        }
        if (y2 > (b->y + b->height - 2))
        {
            y2 = b->y + b->height - 2;
        }
    #if(UI_BLINKING_CURSOR_SUPPORT)
        UI_inputbox_show_cursor(x1, y1, x2, y2, b->cursor_color);
    #else /* (UI_BLINKING_CURSOR_SUPPORT) */ 
        /* gui_draw_rectangle(x1,y1,x2,y2,b->cursor_color); */
        gui_fill_rectangle(x1, y1, x2, y2, b->cursor_color);
    #endif /* (UI_BLINKING_CURSOR_SUPPORT) */ 
    }
    else if (cursor_type == 3)
    {
        x1 = x - 1;
        y1 = y - 1;
        x2 = x + width - 1;
        y2 = y + height - 1;
        if (x1 < (b->x + 1))
        {
            x1 = b->x + 1;
        }
        if (y1 < (b->y + 1))
        {
            y1 = b->y + 1;
        }
        if (x2 > (b->x + b->width - 2))
        {
            x2 = b->x + b->width - 2;
        }
        if (y2 > (b->y + b->height - 2))
        {
            y2 = b->y + b->height - 2;
        }
    #if(UI_BLINKING_CURSOR_SUPPORT)
        //gui_cancel_timer(UI_inputbox_blink_cursor);
         
        StopMyTimer(BLINKING_CURSOR);
        /* CSD end */

    #endif /* (UI_BLINKING_CURSOR_SUPPORT) */ 
        gui_draw_rectangle(x1, y1, x2, y2, b->cursor_color);
    }
}

/* CSD end */

/* CRLF/Esc sequence handling functions:              */
/* These must be made available in the EMS library       */
/* Navigates CRLF/Esc sequence as a single object        */
/* Currently assumes Little endian byte ordering only    */
/* Supports only steps=1                           */


/*****************************************************************************
 * FUNCTION
 *  BackwardCurrentPositionSkipCR
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 * RETURNS
 *  
 *****************************************************************************/
kal_uint16 BackwardCurrentPositionSkipCR(EMSData *data)
{
#ifdef __MMI_HINDI_ALG__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_character_type current_character = 0;
    U8 object_type_check;
    EMSObject *o;
    EMSPosition p = data->CurrentPosition;
    EMSData *data_chk_type = data;
    UI_character_type dummy_c = 0;
    S32 prev_ems_moves = 0;
    UI_buffer_type check_ems_hindi_p;
    PU8 src_p;
    PU8 dest_p;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    check_ems_hindi_p = src_p = (data->textBuffer + data->CurrentPosition.OffsetToText);

    UI_STRING_GET_PREVIOUS_CHARACTER(check_ems_hindi_p, dummy_c);       /* get previous character */
    if (UI_HINDI_CHARACTER_UCS2_RANGE(dummy_c))
    {
        /* Move by a syllable rather than by a character as in English */

        dest_p = hf_get_prev_cluster_p(data->textBuffer, src_p);

        prev_ems_moves = src_p - dest_p;
        prev_ems_moves = prev_ems_moves >> 1;
        object_type_check = gui_EMS_input_box_get_previous_object(data_chk_type, &p, &current_character, &o);
        if (object_type_check == 2)
        {
            prev_ems_moves = 1;
        }
    }
    else
    {
        prev_ems_moves = 1;
    }
    /* To be checked why its returning 0 for prev_ems_moves>1 */
    PRINT_INFORMATION_2((MMI_TRACE_INFO, "BackwardCurrentPositionSkipCR[prev_ems_moves]:%d  ", prev_ems_moves));
    MMI_ASSERT(!(prev_ems_moves < 0));
    if (BackwardCurrentPosition(data, prev_ems_moves) >= 1)
#else /* __MMI_HINDI_ALG__ */ 
    if (BackwardCurrentPosition(data, 1) == 1)
#endif /* __MMI_HINDI_ALG__ */ 
    {
        if (data->textBuffer == NULL)
        {
            return (1);
        }
    #if(EMS_USE_CRLF_COMBINATION)
        if (UI_TEST_LF_CHARACTER(data->textBuffer[data->CurrentPosition.OffsetToText]))
        {
            if (BackwardCurrentPosition(data, 1) == 1)
            {
                if (!UI_TEST_CR_CHARACTER(data->textBuffer[data->CurrentPosition.OffsetToText]))
                {
                    ForwardCurrentPosition(data, 1);
                }
            }
        }
    #endif /* (EMS_USE_CRLF_COMBINATION) */ 
    #if(EMS_USE_ESC_COMBINATION)
        if (BackwardCurrentPosition(data, 1) == 1)
        {

            if (!UI_TEST_ESC_CHARACTER(data->textBuffer[data->CurrentPosition.OffsetToText]))
            {
                ForwardCurrentPosition(data, 1);
            }
        #ifdef __MMI_HINDI_ALG__
            else if ((UI_TEST_ESC_CHARACTER(data->textBuffer[data->CurrentPosition.OffsetToText])) &&
                     (data->textBuffer[data->CurrentPosition.OffsetToText + 1 == 0x09]))
            {
                ForwardCurrentPosition(data, 1);
            }
        #endif /* __MMI_HINDI_ALG__ */ 

        }
    #endif /* (EMS_USE_ESC_COMBINATION) */ 
        return (1);
    }
    return (0);
}

/* Navigates CRLF/Esc sequence as a single object        */
/* Currently assumes Little endian byte ordering only    */
/* Supports only steps=1                           */


/*****************************************************************************
 * FUNCTION
 *  ForwardCurrentPositionSkipCR
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 * RETURNS
 *  
 *****************************************************************************/
kal_uint16 ForwardCurrentPositionSkipCR(EMSData *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint16 count = 0;

#ifdef __MMI_HINDI_ALG__
    UI_character_type current_character = 0;
    U8 object_type_check;
    EMSObject *o;
    EMSPosition p = data->CurrentPosition;
    EMSData *data_chk_type = data;
    UI_character_type dummy_c = 0;
    S32 next_ems_moves = 0;
    UI_buffer_type check_ems_hindi_p;
    PU8 src_p;
    PU8 dest_p;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    check_ems_hindi_p = src_p = (data->textBuffer + data->CurrentPosition.OffsetToText);

    UI_STRING_GET_NEXT_CHARACTER(check_ems_hindi_p, dummy_c);   /* get previous character */
    if (UI_HINDI_CHARACTER_UCS2_RANGE(dummy_c))
    {
        /* Move by a syllable rather than by a character as in English */
        /* dest_p = hf_get_next_cluster_p(data->textBuffer,src_p); */
        dest_p = hf_get_next_cluster_p(src_p, src_p);
        next_ems_moves = dest_p - src_p;
        next_ems_moves = next_ems_moves >> 1;
        object_type_check = gui_EMS_input_box_get_next_object(data_chk_type, &p, &current_character, &o);
 
        if (object_type_check == 2 && (next_ems_moves >= 2) && p.Object->Type == EMS_TYPE_TEXT_FORMAT)
        {
            /* Do nothing */
        }

        else if (object_type_check == 2)
        {
            next_ems_moves = 1;
        }
 
    }
    else
    {
        next_ems_moves = 1;
    }
#endif /* __MMI_HINDI_ALG__ */ 
	
    if (data->textBuffer == NULL)
    {
        count = ForwardCurrentPosition(data, 1);
        return (count);
    }
#if(EMS_USE_ESC_COMBINATION)
    if ((data->dcs == SMSAL_DEFAULT_DCS) &&
        (UI_TEST_ESC_CHARACTER(data->textBuffer[data->CurrentPosition.OffsetToText])))
    {
        count = ForwardCurrentPosition(data, 1);
        if (count == 1)
        {
            ForwardCurrentPosition(data, 1);
        }
    }
    else
#endif /* (EMS_USE_ESC_COMBINATION) */ 
#if(EMS_USE_CRLF_COMBINATION)
    if (UI_TEST_CR_CHARACTER(data->textBuffer[data->CurrentPosition.OffsetToText]))
    {
        count = ForwardCurrentPosition(data, 1);
        if (UI_TEST_LF_CHARACTER(data->textBuffer[data->CurrentPosition.OffsetToText]))
        {
            ForwardCurrentPosition(data, 1);
        }
    }
    else
#endif /* (EMS_USE_CRLF_COMBINATION) */ 
    {
    #ifdef __MMI_HINDI_ALG__
        PRINT_INFORMATION_2((MMI_TRACE_INFO, "ForwardCurrentPositionSkipCR[next_ems_moves]:%d  ", next_ems_moves));
        MMI_ASSERT(!(next_ems_moves < 0));
        count = ForwardCurrentPosition(data, next_ems_moves);
 
        if (count >= 1)
        {
            count = 1;
        }
  
    #else /* __MMI_HINDI_ALG__ */ 
        count = ForwardCurrentPosition(data, 1);
    #endif /* __MMI_HINDI_ALG__ */ 

    }
    return (count);
}

/* Moves the cursor forwards by the number of steps specified.
   Cursor starts at start position. (If data->CursorPosition is not at
   the given start position, then the cursor will be moved to start position first)
   The cursor will not move beyond one step before the given end position  */


/*****************************************************************************
 * FUNCTION
 *  MoveCurrentPosition
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]         
 *  start       [?]         
 *  end         [?]         
 *  steps       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void MoveCurrentPosition(EMSData *data, EMSPosition *start, EMSPosition *end, kal_uint16 steps)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 diff;
    U8 object_flag;
    U8 done_flag = 0;
    S32 count = 0;
    U8 back_flag = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    object_flag = 0;
    diff = data->CurrentPosition.OffsetToText - start->OffsetToText;
    if ((start->Object != NULL) && (start->Object->OffsetToText == start->OffsetToText))
    {
        object_flag = 1;
    }
    if (diff < 0)
    {
        while (!done_flag)
        {
            if (data->CurrentPosition.OffsetToText == start->OffsetToText)
            {
                if (object_flag)
                {
                    if ((data->CurrentPosition.Object == start->Object) &&
                        (data->CurrentPosition.Object->OffsetToText == start->OffsetToText))
                    {
                        BackwardCurrentPosition(data, 1);
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            if (ForwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        }
    }
    else if (diff > 0)
    {
        while (!done_flag)
        {
            if (data->CurrentPosition.OffsetToText == start->OffsetToText)
            {
                if (object_flag)
                {
                    if ((data->CurrentPosition.Object == start->Object) &&
                        (data->CurrentPosition.Object->OffsetToText == start->OffsetToText))
                    {
                        BackwardCurrentPosition(data, 1);
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            if (BackwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        }
    }
    else
    {
        S32 old_OffsetToText = data->CurrentPosition.OffsetToText;

        while (data->CurrentPosition.OffsetToText == old_OffsetToText)
        {
            if (BackwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        }
        while (!done_flag)
        {
            if (data->CurrentPosition.OffsetToText == start->OffsetToText)
            {
                if (object_flag)
                {
                    if ((data->CurrentPosition.Object == start->Object) &&
                        (data->CurrentPosition.Object->OffsetToText == start->OffsetToText))
                    {
                        BackwardCurrentPosition(data, 1);
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            if (ForwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        }
    }
    if (steps == 0)
    {
        return;
    }
    object_flag = 0;
    if ((end->Object != NULL) && (end->Object->OffsetToText == end->OffsetToText))
    {
        object_flag = 1;
    }
    if (data->CurrentPosition.OffsetToText == end->OffsetToText)
    {
        S32 old_OffsetToText = data->CurrentPosition.OffsetToText;

        while (data->CurrentPosition.OffsetToText == old_OffsetToText)
        {
            if (BackwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
            count--;
        }
        while (!done_flag)
        {
            if (count >= steps)
            {
                break;
            }
            count++;
            if (data->CurrentPosition.OffsetToText == end->OffsetToText)
            {
                if (object_flag)
                {
                    if ((data->CurrentPosition.Object == end->Object) &&
                        (data->CurrentPosition.Object->OffsetToText == end->OffsetToText))
                    {
                        back_flag = 1;
                        break;
                    }
                }
                else
                {
                    back_flag = 1;
                    break;
                }
            }
            if (ForwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        }
        if (back_flag)
        {
            BackwardCurrentPosition(data, 1);
        }
        if (data->CurrentPosition.OffsetToText > end->OffsetToText)
        {
            BackwardCurrentPosition(data, 1);
        }
    }
    else
    {
        while (!done_flag)
        {
            if (count >= steps)
            {
                break;
            }
            count++;
            if (data->CurrentPosition.OffsetToText == end->OffsetToText)
            {
                if (object_flag)
                {
                    if ((data->CurrentPosition.Object == end->Object) &&
                        (data->CurrentPosition.Object->OffsetToText == end->OffsetToText))
                    {
                        back_flag = 1;
                        break;
                    }
                }
                else
                {
                    back_flag = 1;
                    break;
                }
            }
            if (ForwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        }
        if (back_flag)
        {
            BackwardCurrentPosition(data, 1);
        }
        if (data->CurrentPosition.OffsetToText >= end->OffsetToText)
        {
            if ((end->OffsetToText != data->textLength) && !((end->Object != NULL) && (end->Object != data->listHead)))
            {
                BackwardCurrentPosition(data, 1);
            }
            if ((data->CurrentPosition.Object != NULL) && (end->Object != NULL))
            {
                while (data->CurrentPosition.Object->OffsetToText > end->Object->OffsetToText)
                {
                    if (BackwardCurrentPosition(data, 1) != 1)
                    {
                        break;
                    }
                }
            }
        }
    }
}

/* Moves the cursor to a given position   */


/*****************************************************************************
 * FUNCTION
 *  MoveCursorToPosition
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 *  dst         [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void MoveCursorToPosition(EMSData *data, EMSPosition *dst)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 diff;
    U8 done_flag = 0;
    EMSPosition dst_copy = *dst;
    EMSPosition mapped_cursor_position;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EMS_map_position(data, &dst_copy, dst);
    EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
    diff = data->CurrentPosition.OffsetToText - dst->OffsetToText;

    if (diff < 0)
    {
        while (!done_flag)
        {
            EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
            if ((mapped_cursor_position.OffsetToText == dst->OffsetToText) &&
                (mapped_cursor_position.Object == dst->Object))
            {
                break;
            }
            if (ForwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        }
    }
    else if (diff > 0)
    {
        while (!done_flag)
        {
            EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
            if ((mapped_cursor_position.OffsetToText == dst->OffsetToText) &&
                (mapped_cursor_position.Object == dst->Object))
            {
                break;
            }
            if (BackwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        }
    }
    else
    {
        S32 old_OffsetToText = data->CurrentPosition.OffsetToText;

        while (data->CurrentPosition.OffsetToText == old_OffsetToText)
        {
            if (BackwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        }
        if (data->CurrentPosition.OffsetToText < old_OffsetToText)
        {
            ForwardCurrentPosition(data, 1);
        }
        while (!done_flag)
        {
            EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
            if (mapped_cursor_position.Object == dst->Object)
            {
                break;
            }
            if (ForwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        }
    }
}

/* Moves the cursor backwards to the given position. If an object is encountered, it breaks  */
/* Returns 1 if dst is reached, otherwise, returns 0                                */


/*****************************************************************************
 * FUNCTION
 *  BackwardCurrentPositionObject
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 *  dst         [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 BackwardCurrentPositionObject(EMSData *data, EMSPosition *dst)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition mapped_cursor_position;
    U8 done = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((data->CurrentPosition.OffsetToText == dst->OffsetToText) && (data->CurrentPosition.Object == dst->Object))
    {
        return (1);
    }
    while (!done)
    {
        if (BackwardCurrentPosition(data, 1) != 1)
        {
            done = 1;
            break;
        }
        EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
        if ((mapped_cursor_position.Object != NULL) && (mapped_cursor_position.Object->Type != EMS_TYPE_TEXT_FORMAT))
        {
            break;
        }
        if ((data->CurrentPosition.OffsetToText == dst->OffsetToText) && (data->CurrentPosition.Object == dst->Object))
        {
            done = 1;
            break;
        }
    }
    return (done);
}


/*****************************************************************************
 * FUNCTION
 *  ForwardCurrentPositionObject
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 *  dst         [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 ForwardCurrentPositionObject(EMSData *data, EMSPosition *dst)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition mapped_cursor_position;
    U8 done = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((data->CurrentPosition.OffsetToText == dst->OffsetToText) && (data->CurrentPosition.Object == dst->Object))
    {
        return (1);
    }
    while (!done)
    {
        if (ForwardCurrentPosition(data, 1) != 1)
        {
            done = 1;
            break;
        }
        EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
        if ((mapped_cursor_position.Object != NULL) && (mapped_cursor_position.Object->Type != EMS_TYPE_TEXT_FORMAT))
        {
            break;
        }
        if ((data->CurrentPosition.OffsetToText == dst->OffsetToText) && (data->CurrentPosition.Object == dst->Object))
        {
            done = 1;
            break;
        }
    }
    return (done);
}

/* Moves the cursor backwards to the given position. If an object is encountered, it breaks  */
/* Returns 1 if dst is reached, otherwise, returns 0                                */


/*****************************************************************************
 * FUNCTION
 *  BackwardCurrentPositionSeekObject
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 *  dst         [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 BackwardCurrentPositionSeekObject(EMSData *data, EMSPosition *dst)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition mapped_cursor_position;
    U8 done = 0;
    U8 object_flag = 0;
    EMSPosition last_object_position = data->CurrentPosition;
    S32 count = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((data->CurrentPosition.OffsetToText == dst->OffsetToText) && (data->CurrentPosition.Object == dst->Object))
    {
        return (1);
    }
    while (!done)
    {
        EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
        if ((mapped_cursor_position.Object != NULL) && (mapped_cursor_position.Object->Type != EMS_TYPE_TEXT_FORMAT))
        {
            last_object_position = data->CurrentPosition;
            object_flag = 1;
            if (count > 0)
            {
                break;
            }
        }
        if (BackwardCurrentPosition(data, 1) != 1)
        {
            done = 1;
            break;
        }
        if ((data->CurrentPosition.OffsetToText == dst->OffsetToText) && (data->CurrentPosition.Object == dst->Object))
        {
            done = 1;
            break;
        }
        count++;
    }
    EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
    if (object_flag && (count > 0) &&
        ((data->CurrentPosition.OffsetToText != dst->OffsetToText) || (data->CurrentPosition.Object != dst->Object)))
    {
        MoveCursorToPosition(data, &last_object_position);
    }
    else if (mapped_cursor_position.Object == NULL)
    {
        MoveCursorToPosition(data, &last_object_position);
    }
    return (done);
}


/*****************************************************************************
 * FUNCTION
 *  ForwardCurrentPositionSeekObject
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 *  dst         [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 ForwardCurrentPositionSeekObject(EMSData *data, EMSPosition *dst)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition mapped_cursor_position;
    U8 done = 0;
    EMSPosition last_object_position = data->CurrentPosition;
    S32 count = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((data->CurrentPosition.OffsetToText == dst->OffsetToText) && (data->CurrentPosition.Object == dst->Object))
    {
        return (1);
    }
    while (!done)
    {
        EMS_map_position(data, &data->CurrentPosition, &mapped_cursor_position);
        if ((mapped_cursor_position.Object != NULL) && (mapped_cursor_position.Object->Type != EMS_TYPE_TEXT_FORMAT))
        {
            last_object_position = data->CurrentPosition;
            if (count > 0)
            {
                break;
            }
        }
        if (ForwardCurrentPosition(data, 1) != 1)
        {
            done = 1;
            break;
        }
        if ((data->CurrentPosition.OffsetToText == dst->OffsetToText) && (data->CurrentPosition.Object == dst->Object))
        {
            done = 1;
            break;
        }
        count++;
    }
    MoveCursorToPosition(data, &last_object_position);
    return (done);
}

/* Returns true if the given position is the last position  */
/* p=un-mapped EMS position                        */


/*****************************************************************************
 * FUNCTION
 *  TestLastPosition
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 *  p           [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 TestLastPosition(EMSData *data, EMSPosition *p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition dst;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EMS_map_position(data, p, &dst);
    if ((dst.OffsetToText >= data->textLength) && (dst.Object == NULL))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

/* Returns true if the given position is the first position */
/* p=un-mapped EMS position                        */


/*****************************************************************************
 * FUNCTION
 *  TestFirstPosition
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 *  p           [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 TestFirstPosition(EMSData *data, EMSPosition *p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((p->OffsetToText == 0) && (p->Object == NULL))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  EMS_map_position
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 *  src         [?]     
 *  dst         [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_map_position(EMSData *data, EMSPosition *src, EMSPosition *dst)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (data->listHead == NULL)
    {
        *dst = *src;
    }
    else
    {
        EMSObject *object;

        if (src->Object == NULL)
        {
            object = data->listHead;
            if (object->OffsetToText == src->OffsetToText)
            {

            #if(EMS_ENABLE_TESTFIX_OBJECT_ORDERING)
                /* Nov-21-2003: TESTFIX: text format object followed by EMS object fix                 */
                /* Object ordering must be as text format object followed by other objects to use this fix   */
                if (object != NULL)
                {
                    if (object->Type == EMS_TYPE_TEXT_FORMAT)
                    {
                        if (object->next != NULL)
                        {
                            if (object->next->OffsetToText == object->OffsetToText)
                            {
                                dst->OffsetToText = src->OffsetToText;
                                dst->Object = object->next;
                            }
                            else
                            {
                                dst->OffsetToText = src->OffsetToText;
                                dst->Object = object;
                            }
                        }
                        else
                        {
                            dst->OffsetToText = src->OffsetToText;
                            dst->Object = object;
                        }
                    }
                    else
                    {
                        dst->OffsetToText = src->OffsetToText;
                        dst->Object = object;
                    }
                }
                else
                {
                    /* Nov-21-2003: TESTFIX: ends */
            #endif /* (EMS_ENABLE_TESTFIX_OBJECT_ORDERING) */ 

                    dst->OffsetToText = src->OffsetToText;
                    dst->Object = object;

                #if(EMS_ENABLE_TESTFIX_OBJECT_ORDERING)
                    /* Nov-21-2003: TESTFIX    */
                }
                /* Nov-21-2003: TESTFIX: ends */
                #endif /* (EMS_ENABLE_TESTFIX_OBJECT_ORDERING) */ 
            }
            else
            {
                dst->OffsetToText = src->OffsetToText;
                dst->Object = NULL;
            }
        }
        else
        {
            object = src->Object->next;
            if ((object != NULL) && (object->OffsetToText == src->OffsetToText))
            {
            #if(EMS_ENABLE_TESTFIX_OBJECT_ORDERING)
                /* Nov-21-2003: TESTFIX: text format object followed by EMS object fix                 */
                /* Object ordering must be as text format object followed by other objects to use this fix   */
                if (object->Type == EMS_TYPE_TEXT_FORMAT)
                {
                    if (object->next != NULL)
                    {
                        if (object->next->OffsetToText == object->OffsetToText)
                        {
                            dst->OffsetToText = src->OffsetToText;
                            dst->Object = object->next;
                        }
                        else
                        {
                            dst->OffsetToText = src->OffsetToText;
                            dst->Object = NULL;
                        }
                    }
                    else
                    {
                        dst->OffsetToText = src->OffsetToText;
                        dst->Object = NULL;
                    }
                }
                else
                {
                    /* Nov-21-2003: TESTFIX: ends */
            #endif /* (EMS_ENABLE_TESTFIX_OBJECT_ORDERING) */ 

                    dst->OffsetToText = src->OffsetToText;
                    dst->Object = object;

                #if(EMS_ENABLE_TESTFIX_OBJECT_ORDERING)
                    /* Nov-21-2003: TESTFIX    */
                }
                /* Nov-21-2003: TESTFIX: ends */
                #endif /* (EMS_ENABLE_TESTFIX_OBJECT_ORDERING) */ 
            }
            else
            {
                dst->OffsetToText = src->OffsetToText;
                dst->Object = NULL;
            }
        }
    }
}

U8 EMS_view_mode = 0;

#ifdef __MMI_MESSAGES_EMS__

#define EMS_ANIMATION_TIME    500

S32 EMS_current_predefined_sound_ID = -1;
U8 EMS_object_pre_defined_sound_flag = 0;
U8 EMS_object_user_defined_melody_flag = 0;
PU8 EMS_editor_sound_icon = NULL;
PU8 EMS_editor_melody_icon = NULL;
S32 EMS_object_x = 0;
S32 EMS_object_y = 0;
S32 EMS_object_width = 0;
S32 EMS_object_height = 0;
S32 EMS_object_frame_counter = 0;
S32 EMS_object_n_frames = 0;
S32 EMS_object_current_frame_size = 0;
S32 EMS_object_repeat_counter = 0;
U8 EMS_object_animation_timer_flag = 0;
U8 EMS_object_predefined_animation_timer_flag = 0;
EMSObject *EMS_current_object = NULL;
PU8 EMS_current_predefined_image;

extern PU8 get_image(U16 i);
extern void AudioPlayIMelodyString(U8 *string, U16 len, U8 style);
extern U16 EMS_predefined_sounds[];

#ifdef __MMI_AUDIO_PLAYER__
extern BOOL mmi_audply_is_play_activated(void);
#endif 


/*****************************************************************************
 * FUNCTION
 *  EMS_normal_play_pre_defined_sound
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ID      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_normal_play_pre_defined_sound(S32 ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ID < 0)
    {
        ID = 0;
    }
    if (ID > 9)
    {
        ID = 9;
    }
    EMS_current_predefined_sound_ID = EMS_predefined_sounds[ID];
    StopCurrentKeyPadTone();
    AudioPlayReq((U8) EMS_current_predefined_sound_ID, DEVICE_AUDIO_PLAY_ONCE);
}


/*****************************************************************************
 * FUNCTION
 *  EMS_normal_stop_pre_defined_sound
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_normal_stop_pre_defined_sound(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    AudioStopReq((U8) EMS_current_predefined_sound_ID);
}

#include "mdi_datatype.h"
#include "mdi_audio.h"


/*****************************************************************************
 * FUNCTION
 *  EMS_normal_play_user_defined_melody
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]         
 *  length      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_normal_play_user_defined_melody(U8 *data, S32 length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mdi_audio_play_string(data, (U16) length, MDI_FORMAT_IMELODY, DEVICE_AUDIO_PLAY_ONCE, NULL, NULL);
}


/*****************************************************************************
 * FUNCTION
 *  EMS_normal_stop_user_defined_melody
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_normal_stop_user_defined_melody(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mdi_audio_stop_string();
}


/*****************************************************************************
 * FUNCTION
 *  EMS_silent_play_pre_defined_sound
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ID      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_silent_play_pre_defined_sound(S32 ID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(ID);
}


/*****************************************************************************
 * FUNCTION
 *  EMS_silent_stop_pre_defined_sound
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_silent_stop_pre_defined_sound(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}


/*****************************************************************************
 * FUNCTION
 *  EMS_silent_play_user_defined_melody
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]         
 *  length      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_silent_play_user_defined_melody(U8 *data, S32 length)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UI_UNUSED_PARAMETER(data);
    UI_UNUSED_PARAMETER(length);
}


/*****************************************************************************
 * FUNCTION
 *  EMS_silent_stop_user_defined_melody
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_silent_stop_user_defined_melody(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}

void (*EMS_play_pre_defined_sound) (S32 ID) = EMS_normal_play_pre_defined_sound;
void (*EMS_stop_pre_defined_sound) (void) = EMS_normal_stop_pre_defined_sound;
void (*EMS_play_user_defined_melody) (U8 *data, S32 length) = EMS_normal_play_user_defined_melody;
void (*EMS_stop_user_defined_melody) (void) = EMS_normal_stop_user_defined_melody;


/*****************************************************************************
 * FUNCTION
 *  EMS_enable_audio_playback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_enable_audio_playback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EMS_play_pre_defined_sound = EMS_normal_play_pre_defined_sound;
    EMS_stop_pre_defined_sound = EMS_normal_stop_pre_defined_sound;
    EMS_play_user_defined_melody = EMS_normal_play_user_defined_melody;
    EMS_stop_user_defined_melody = EMS_normal_stop_user_defined_melody;
}


/*****************************************************************************
 * FUNCTION
 *  EMS_disable_audio_playback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_disable_audio_playback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EMS_play_pre_defined_sound = EMS_silent_play_pre_defined_sound;
    EMS_stop_pre_defined_sound = EMS_silent_stop_pre_defined_sound;
    EMS_play_user_defined_melody = EMS_silent_play_user_defined_melody;
    EMS_stop_user_defined_melody = EMS_silent_stop_user_defined_melody;
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_measure_object
 * DESCRIPTION
 *  
 * PARAMETERS
 *  o           [?]     
 *  width       [?]     
 *  height      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_measure_object(EMSObject *o, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    *width = 0;
    *height = 0;
    switch (o->Type)
    {
        case EMS_TYPE_PIC:
        {
            *width = o->data->picture.hDim;
            *height = o->data->picture.vDim;
        }
            break;
        case EMS_TYPE_USERDEF_ANM:
        {
            *width = o->data->animation.hDim;
            *height = o->data->animation.vDim;
        }
            break;
        case EMS_TYPE_PREDEF_ANM:
        {
            PU8 image = get_image(EMS_predefined_animations[o->PredefNo]);

            gui_measure_image(image, width, height);
        }
            break;
        case EMS_TYPE_PREDEF_SND:
        {
            gui_measure_image(EMS_editor_sound_icon, width, height);
        }
            break;
        case EMS_TYPE_USERDEF_SND:
        {
            gui_measure_image(EMS_editor_melody_icon, width, height);
        }
            break;
    }
}


void gui_EMS_input_box_display_object_pic(S32 x,S32 y,U8* src,S32 src_size,S32 w,S32 h,S32 bits_per_pixel)
{
    gdi_color palette[64]; // maximum is 64 colors
    switch(bits_per_pixel)
    {
    case 1:
        {
            palette[0] = gdi_act_color_from_rgb(255,255,255,255); // white
            palette[1] = gdi_act_color_from_rgb(255,0,0,0); // black
        }
        break;
    case 2:
        {
            palette[0] = gdi_act_color_from_rgb(255,0,0,0); // black
            palette[1] = gdi_act_color_from_rgb(255,85,85,85); // dark grey
            palette[2] = gdi_act_color_from_rgb(255,170,170,170); // light grey
            palette[3] = gdi_act_color_from_rgb(255,255,255,255); // white
        }
        break;
    case 6:
        {
            S32 R,G,B,index;
            const static S32 value[]={0,85,170,255};

            index = 0;
            for(R=0;R<4;R++)
                for(G=0;G<4;G++)
                    for(B=0;B<4;B++,index++)
                        palette[index] = gdi_act_color_from_rgb(255,value[R],value[G],value[B]);
        }
        break;
    default:
        MMI_ASSERT(0); // strange bits_per_pixel
        break;
    }
    gdi_bits_draw(
        x,y,
        src,
        src_size,
        w,h,
        bits_per_pixel,
        palette);

}


/*****************************************************************************
 * FUNCTION
 *  EMS_input_box_show_animation_frame
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/

void EMS_input_box_show_animation_frame(void)
{
    S32 width,height;
    EMS_object_frame_counter++;
    if (EMS_object_frame_counter >= EMS_current_object->data->animation.cFrame)
        EMS_object_frame_counter = 0;

    width = EMS_current_object->data->animation.hDim;
    height = EMS_current_object->data->animation.vDim;
    
    gui_EMS_input_box_display_object_pic(
                        EMS_object_x,EMS_object_y,
                        (U8*) (EMS_current_object->data->animation.pdu 
                             + EMS_object_frame_counter * EMS_object_current_frame_size),
                        EMS_object_current_frame_size,
                        width,
                        height,
                        EMS_current_object->data->animation.bitsPerPixel);
    gui_BLT_double_buffer(EMS_object_x, EMS_object_y, EMS_object_x + width - 1, EMS_object_y + height - 1);
    if (EMS_current_object->data->animation.cFrame != 0)
    {
        if (EMS_object_frame_counter % EMS_current_object->data->animation.cFrame == 0)
            EMS_object_repeat_counter++;
    }
    if (EMS_current_object->data->animation.cRepeat != 0 
    && EMS_object_repeat_counter >= EMS_current_object->data->animation.cRepeat)
    {
        gui_cancel_timer(NULL);
    }
    else
    {
        gui_start_timer((EMS_current_object->data->animation.Duration + 1) * 100, EMS_input_box_show_animation_frame);
    }
}


/*****************************************************************************
 * FUNCTION
 *  EMS_input_box_show_predefined_animation_frame
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_input_box_show_predefined_animation_frame(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EMS_object_frame_counter++;
    if (EMS_object_frame_counter >= EMS_object_n_frames)
    {
        EMS_object_frame_counter = 0;
    }
    gui_push_clip();
    gui_set_clip(0, MMI_title_y + MMI_title_height, UI_device_width - 1, UI_device_height - MMI_button_bar_height - 2);
    gui_show_animated_image_frame(
        EMS_object_x,
        EMS_object_y,
        EMS_current_predefined_image,
        (S16) EMS_object_frame_counter);
    gui_pop_clip();
#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_BLT_double_buffer(
        EMS_object_x,
        EMS_object_y,
        EMS_object_x + EMS_object_width - 1,
        EMS_object_y + EMS_object_height - 1);
#endif /* (UI_DOUBLE_BUFFER_SUPPORT) */ 
    gui_start_timer(EMS_ANIMATION_TIME, EMS_input_box_show_predefined_animation_frame);
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_display_object
 * DESCRIPTION
 *  
 * PARAMETERS
 *  x                   [IN]        
 *  y                   [IN]        
 *  o                   [?]         
 *  display_cursor      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/


void gui_EMS_input_box_display_object(S32 x, S32 y, EMSObject *o, U8 display_cursor)
{
#if(EMS_ENABLE_EDITOR_OBJECT_PLAYBACK)
    U8 playback_flag = 1;
#else
    U8 playback_flag = 0;
    if (EMS_view_mode)
        playback_flag = 1;
#endif

    switch (o->Type)
    {
        case EMS_TYPE_PIC:
            gui_EMS_input_box_display_object_pic(
                        x,y,
                        (U8*)o->data->picture.pdu,
                        o->data->picture.pdu_length,
                        o->data->picture.hDim,o->data->picture.vDim,
                        o->data->picture.bitsPerPixel);
            break;

        case EMS_TYPE_USERDEF_ANM:
            if (display_cursor)
            {
                EMS_object_x = x;
                EMS_object_y = y;
                EMS_object_frame_counter = 0;
                EMS_object_repeat_counter = 0;
                EMS_current_object = o;
            }
            
            EMS_object_current_frame_size = o->data->animation.pdu_length;
            if (o->data->animation.cFrame != 0)
                EMS_object_current_frame_size /= o->data->animation.cFrame;

            gui_EMS_input_box_display_object_pic(
                    x,y,
                    (U8*) o->data->animation.pdu,
                    EMS_object_current_frame_size,
                    o->data->animation.hDim,o->data->animation.vDim,
                    o->data->animation.bitsPerPixel);
            
            if (display_cursor && playback_flag)
            {
                EMS_object_animation_timer_flag = 1;
                gui_start_timer((o->data->animation.Duration + 1) * 100, EMS_input_box_show_animation_frame);
            }
            break;
        case EMS_TYPE_PREDEF_ANM:
        {
            PU8 image = get_image(EMS_predefined_animations[o->PredefNo]);

            gui_show_animated_image_frame(x, y, image, 0);
            if (display_cursor && playback_flag)
            {
                EMS_object_x = x;
                EMS_object_y = y;
                gui_measure_image(image, &EMS_object_width, &EMS_object_height);
                EMS_object_n_frames = gui_image_n_frames(image);
                EMS_object_frame_counter = 0;
                EMS_current_object = o;
                EMS_current_predefined_image = image;
                EMS_object_predefined_animation_timer_flag = 1;
                gui_start_timer(EMS_ANIMATION_TIME, EMS_input_box_show_predefined_animation_frame);
            }
        }
            break;
        case EMS_TYPE_PREDEF_SND:
        {
            gui_show_transparent_image(x, y, EMS_editor_sound_icon, 0);
            if (display_cursor && playback_flag)
            {
                EMS_object_pre_defined_sound_flag = 1;
                EMS_play_pre_defined_sound(o->PredefNo);
            }
        }
            break;
        case EMS_TYPE_USERDEF_SND:
        {
            gui_show_transparent_image(x, y, EMS_editor_melody_icon, 0);
            if (display_cursor && playback_flag)
            {
                EMS_object_user_defined_melody_flag = 1;
                /* CSD Joy added for customization, 1014 */
            #if defined(__MMI_IMELODY_SUPPORT__)
                EMS_play_user_defined_melody(o->data->sound.pdu, o->data->sound.pdu_length);
            #endif 
                /* CSD end */
            }
        }
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_playback_object
 * DESCRIPTION
 *  
 * PARAMETERS
 *  o       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_playback_object(EMSObject *o)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (o->Type)
    {
        case EMS_TYPE_PREDEF_SND:
        {
            EMS_object_pre_defined_sound_flag = 1;
            EMS_play_pre_defined_sound(o->PredefNo);
        }
            break;
            /* CSD Joy added for customization, 1014 */
    #if defined(__MMI_IMELODY_SUPPORT__)
        case EMS_TYPE_USERDEF_SND:
        {
            EMS_object_user_defined_melody_flag = 1;
            EMS_play_user_defined_melody(o->data->sound.pdu, o->data->sound.pdu_length);
        }
            break;
    #endif /* defined(__MMI_IMELODY_SUPPORT__) */ 
            /* CSD end */
    }
}


/*****************************************************************************
 * FUNCTION
 *  EMS_cancel_object_focus
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_cancel_object_focus(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (EMS_object_animation_timer_flag)
    {
        gui_cancel_timer(EMS_input_box_show_animation_frame);
        EMS_object_animation_timer_flag = 0;
    }
    else if (EMS_object_predefined_animation_timer_flag)
    {
        gui_cancel_timer(EMS_input_box_show_predefined_animation_frame);
        EMS_object_predefined_animation_timer_flag = 0;
    }
    else if (EMS_object_pre_defined_sound_flag)
    {
        EMS_stop_pre_defined_sound();
        EMS_object_pre_defined_sound_flag = 0;
    }
    else if (EMS_object_user_defined_melody_flag)
    {
        EMS_stop_user_defined_melody();
        EMS_object_user_defined_melody_flag = 0;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_set_text_format
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 *  o       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_set_text_format(UI_EMS_input_box *b, EMSObject *o)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 font_size = (U8) MEDIUM_FONT;
    U8 font_bold = 0;
    U8 font_italic = 0;
    U8 font_underline = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* color        c={0,0,0,100}; */

    /* UI_EMS_inputbox_text_color=c; */
    if (o == NULL)
    {
        EMS_SET_FONT(&UI_EMS_inputbox_medium_font);
        EMS_SET_TEXT_COLOR(UI_EMS_inputbox_text_color);
        b->flags &=
            ~(UI_EMS_INPUT_BOX_CURRENT_ALIGN_LEFT | UI_EMS_INPUT_BOX_CURRENT_ALIGN_RIGHT |
              UI_EMS_INPUT_BOX_CURRENT_ALIGN_CENTER);
    }
    else
    {
        if (o->Type == EMS_TYPE_TEXT_FORMAT)
        {
            EMSTextFormat *t = &(o->data->text_format);

            b->flags &=
                ~(UI_EMS_INPUT_BOX_CURRENT_ALIGN_LEFT | UI_EMS_INPUT_BOX_CURRENT_ALIGN_RIGHT |
                  UI_EMS_INPUT_BOX_CURRENT_ALIGN_CENTER);
            switch (t->Alignment)
            {
                case EMS_ALIGN_LEFT:
                    b->flags |= UI_EMS_INPUT_BOX_CURRENT_ALIGN_LEFT;
                    break;
                case EMS_ALIGN_RIGHT:
                    b->flags |= UI_EMS_INPUT_BOX_CURRENT_ALIGN_RIGHT;
                    break;
                case EMS_ALIGN_CENTER:
                    b->flags |= UI_EMS_INPUT_BOX_CURRENT_ALIGN_CENTER;
                    break;
                case EMS_ALIGN_LANG_DEP:
                    b->flags |= UI_EMS_INPUT_BOX_CURRENT_ALIGN_LEFT;
                    break;
            }
            switch (t->FontSize)
            {
                case EMS_FONTSIZE_NORMAL:
                    font_size = (U8) UI_EMS_inputbox_medium_font.size;
                    /* c.r=0;c.g=0;c.b=0; */
                    break;
                case EMS_FONTSIZE_LARGE:
                    font_size = (U8) UI_EMS_inputbox_large_font.size;
                    /* c.r=255;c.g=0;c.b=0; */
                    break;
                case EMS_FONTSIZE_SMALL:
                    font_size = (U8) UI_EMS_inputbox_small_font.size;
                    /* c.r=0;c.g=0;c.b=255; */
                    break;
            }
        #ifndef __MMI_TEXT_FORMAT_WITHOUT_BOLD__
            if (t->isBold)
            {
                font_bold = 1;
            }
        #endif /* __MMI_TEXT_FORMAT_WITHOUT_BOLD__ */ 
            if (t->isItalic)
            {
                font_italic = 1;
            }
            if (t->isUnderline)
            {
                font_underline = 1;
            }
            UI_EMS_inputbox_current_font.bold = font_bold;
            UI_EMS_inputbox_current_font.italic = font_italic;
            UI_EMS_inputbox_current_font.underline = font_underline;
            UI_EMS_inputbox_current_font.size = font_size;
            EMS_SET_FONT(&UI_EMS_inputbox_current_font);
            /* UI_EMS_inputbox_text_color=c; */
            EMS_SET_TEXT_COLOR(UI_EMS_inputbox_text_color);
        }
    }
}
#endif /* __MMI_MESSAGES_EMS__ */ 
#define UI_EMS_INPUTBOX_N_BREAK_CHARACTERS   2

UI_character_type EMS_input_box_break_characters[] = { ' ',
    /*
     * '.',
     * ',',
     * '\\',
     * '/',
     * '-',
     * ':',
     * ';',
     * '?',
     * '!',
     * '|',
     * '+',
     * '=',
     * '_',
     */
    '\n'
};

UI_character_type EMS_input_box_new_line_character = (UI_character_type) '\n';


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_breakable_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  c       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_breakable_character(UI_character_type c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < UI_EMS_INPUTBOX_N_BREAK_CHARACTERS; i++)
    {
        if ((UI_character_type) c == EMS_input_box_break_characters[i])
        {
            return (1);
        }
    }
    return (0);
}

#define UI_EMS_INPUT_BOX_BREAK_CHARACTER(c,flag)                                                      \
      {  S32 _ii_;                                                                                    \
         flag=0;                                                                                      \
         for(_ii_=0;_ii_<UI_EMS_INPUTBOX_N_BREAK_CHARACTERS;_ii_++)                                   \
         {  if((UI_character_type)c==(UI_character_type)EMS_input_box_break_characters[_ii_])         \
            {  flag=1;                                                                                \
               break;                                                                                 \
            }                                                                                         \
         }                                                                                            \
      }


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_new_line_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  c       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_new_line_character(UI_character_type c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((UI_character_type) c == EMS_input_box_new_line_character)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

#if(EMS_USE_CRLF_COMBINATION)


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_insert_new_line
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_input_box_insert_new_line(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    U8 ret = EMS_OK;

#if(EMS_BYTE_ORDER_BIG_ENDIAN)
    U8 s[4] = {0x00, 0x0D, 0x00, 0x0A};
#else 
    U8 s[4] = {0x0D, 0x00, 0x0A, 0x00};
#endif 
    S32 remaining_length;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
    if (b->UCS2_count > 0)
    {
        remaining_length = (EMS_get_remaining_length(b->data) >> 1);
    }
    else
    {
        remaining_length = EMS_get_remaining_length(b->data);
    }
    if (remaining_length < 2)
    {
        UI_editor_play_tone_cannot_insert();
        ret = EMS_NO_SPACE;
        return ret;
    }
    /* Overwrite mode is un-implemented */
    if (AddString(b->data, s, 2, NULL) == EMS_NO_SPACE)
    {
        UI_editor_play_tone_cannot_insert();
        b->overflow_callback();
        ret = EMS_NO_SPACE;
    }
    else
    {
        b->change_callback();
    }
	b->text_format.Alignment = EMS_ALIGN_LANG_DEP; // reset alignment when input 'enter' symbol
    gui_EMS_input_box_locate_cursor(b);
    return ret;
}

#else /* (EMS_USE_CRLF_COMBINATION) */ 


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_insert_new_line
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_input_box_insert_new_line(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_EMS_input_box_insert_character(b, EMS_input_box_new_line_character);
    return EMS_OK;
}

#endif /* (EMS_USE_CRLF_COMBINATION) */ 

#if(0)

UI_character_type TEMP_EMS_GET_OBJECT(EMSObject *o)
{	if((o!=NULL) && (o->Type==EMS_TYPE_PIC) && (o->data->picture.pdu!=NULL))
	{	return((UI_character_type)((UI_character_type)'0'+(UI_character_type)(o->data->picture.pdu[0])));
	}
	else return((UI_character_type)' ');
}

void temp_print_object(EMSObject *o)
{
	if(o==NULL) PRINT_INFORMATION(("-"));
	else if(o->Type==0) PRINT_INFORMATION(("*"));
	else if(o->Type==5) PRINT_INFORMATION(("%d",o->data->picture.pdu[0]));
}

void temp_print_next_object(EMSObject *o)
{
	if(o==NULL) PRINT_INFORMATION(("-"));
	else if(o->next==NULL) PRINT_INFORMATION(("-"));
	else if(o->next->Type==0) PRINT_INFORMATION(("*"));
	else if(o->next->Type==5) PRINT_INFORMATION(("%d",o->next->data->picture.pdu[0]));
}

#endif /* (0) */ 

#define EMS_SET_BREAK_POSITION()                                     \
{  break_line_width=line_width;                                      \
   break_line_height=line_height;                                    \
   break_line_object_count=line_object_count;                        \
   break_line_alignment=line_alignment;                              \
   break_line_text_object_count=line_text_object_count;              \
}

#define EMS_SET_NEXT_BREAK_POSITION()                                \
{  break_line_width=line_width;                                      \
   break_line_height=line_height;                                    \
   break_line_object_count=line_object_count+1;                      \
   break_line_alignment=line_alignment;                              \
   break_line_text_object_count=line_text_object_count;              \
}


/*****************************************************************************
 * FUNCTION
 *  EMS_handle_CRLF_navigation
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_handle_CRLF_navigation(EMSData *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 r;
    EMSPosition p;
    UI_character_type c;
    EMSObject *o;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if(EMS_USE_CRLF_COMBINATION)
    p = data->CurrentPosition;
    r = gui_EMS_input_box_get_next_object(data, &p, &c, &o);
    if ((r == 1) && UI_TEST_LF_CHARACTER(c))
    {
        p = data->CurrentPosition;
        r = gui_EMS_input_box_get_previous_object(data, &p, &c, &o);
        if ((r == 1) && UI_TEST_CR_CHARACTER(c))
        {
            BackwardCurrentPosition(data, 1);
            return;
        }
    }
#endif /* (EMS_USE_CRLF_COMBINATION) */ 
#if(EMS_USE_ESC_COMBINATION)
    if (data->dcs == SMSAL_DEFAULT_DCS)
    {
        p = data->CurrentPosition;
        r = gui_EMS_input_box_get_previous_object(data, &p, &c, &o);
        if ((r == 1) && UI_TEST_ESC_CHARACTER(c))
        {
            BackwardCurrentPosition(data, 1);
        }
    }
#endif /* (EMS_USE_ESC_COMBINATION) */ 
}

#define EMS_EDITOR_DRAW_DOTTED_LINE(x1,x2,y,c)                       \
{  S32 i,xx1=(x1),xx2=(x2),yy=(y);                                   \
   for(i=xx1;i<=xx2;i++)                                             \
   {  if(i&1) gui_putpixel(i,yy,c);                                  \
   }                                                                 \
}

#define EMS_EDITOR_DRAW_LINE(x1,x2,y,c)                              \
{  S32 i,xx1=(x1),xx2=(x2),yy=(y);                                   \
   for(i=xx1;i<=xx2;i++)                                             \
   {  gui_putpixel(i,yy,c);                                          \
   }                                                                 \
}

U8 EMS_object_width_cache[128];
U8 EMS_object_height_cache[128];

extern S32 wgui_EMS_inputbox_information_bar_height;

#define MAX_EMS_DISP_CLUSTER_LEN 50
 
#ifdef __MMI_HINDI_ALG__
extern BOOL cancelWidth;
#endif 
 

/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_back_cursor_for_ESC_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data            [?]     
 *  position        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_back_cursor_for_ESC_character(EMSData *data, EMSPosition *position)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSObject *current_object;
    UI_character_type current_character;
    U8 object_type;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    object_type = gui_EMS_input_box_get_previous_object(data, position, &current_character, &current_object);   /* get next character */
    if (1 == object_type)
    {
        if (UI_TEST_ESC_CHARACTER(current_character))
        {
            if (BackwardCurrentPositionSkipCR(data) != 1)
            {
                UI_editor_play_tone_cannot_navigate();
            }
        }
    }
}


#if defined (__MMI_LANG_ARABIC__)||defined(__MMI_LANG_PERSIAN__)


/*****************************************************************************
 * FUNCTION
 *  gui_is_ems_object_can_display_in_editor
 * DESCRIPTION
 *  
 * PARAMETERS
 *  type        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
MMI_BOOL gui_is_ems_object_can_display_in_editor(U8 type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (type == EMS_TYPE_PREDEF_SND ||
        type == EMS_TYPE_USERDEF_SND ||
        type == EMS_TYPE_PREDEF_ANM || type == EMS_TYPE_USERDEF_ANM || type == EMS_TYPE_PIC)
    {
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_insert_character_into_buffer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  buffer      [?]         
 *  offset      [IN]        
 *  c           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_insert_character_into_buffer(U16 *buffer, U16 offset, UI_character_type c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_buffer_type p1, p2;
    UI_character_type old_c;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    p1 = p2 = ((U8*) buffer) + offset;

    while (!UI_STRING_END_OF_STRING_CHARACTER(c))
    {
        UI_STRING_GET_NEXT_CHARACTER(p1, old_c);
        UI_STRING_INSERT_CHARACTER(p2, c);
        c = old_c;
    }
    UI_STRING_INSERT_CHARACTER(p2, c);  /* insert character */
}


/*****************************************************************************
 * FUNCTION
 *  gui_delete_character_from_buffer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  buffer      [?]         
 *  offset      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_delete_character_from_buffer(U16 *buffer, U16 offset)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_buffer_type p1, p2;
    UI_character_type c;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    p1 = p2 = ((U8*) buffer) + offset;
    c = *p1;
    p1 += 2;

    while (!UI_STRING_END_OF_STRING_CHARACTER(c))
    {
        UI_STRING_GET_NEXT_CHARACTER(p1, c);
        UI_STRING_INSERT_CHARACTER(p2, c);  /* insert character */
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_get_ems_text_actual_idx
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b                   [?]         
 *  OffsetToText        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U16 gui_get_ems_text_actual_idx(UI_EMS_input_box *b, U16 OffsetToText)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSData *data = b->data;
    EMSObject *cur_obj = data->listHead;
    U16 offset = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (cur_obj != NULL)
    {
        if (gui_is_ems_object_can_display_in_editor((U8) cur_obj->Type))
        {
            if (cur_obj->OffsetToText < OffsetToText)
            {
                offset += 2;
            }
            else
            {
                return (OffsetToText + offset);
            }
        }
        cur_obj = cur_obj->next;
    }
    return (OffsetToText + offset);

}

#define dummy_char 0x01
U16 object_position[128];
U8 object_no = 0;


/*****************************************************************************
 * FUNCTION
 *  gui_ems_data_apply_shaping
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b                       [?]     
 *  text_shaping_buffer     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
//extern void ArabicShapeEngine2(U16* len, U16* str, U16* Orgstr, U16* p);
void gui_ems_data_apply_shaping(UI_EMS_input_box *b, U16 *text_shaping_buffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSData *data = b->data;
    EMSObject *cur_obj = data->listHead;
    S32 text_len = (b->data->textLength / 2);
    MMI_BOOL has_ems_display_object = MMI_FALSE;
	U16 t;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    object_no = 0;
    while (cur_obj != NULL)
    {
        if (gui_is_ems_object_can_display_in_editor((U8) cur_obj->Type))
        {
            has_ems_display_object = MMI_TRUE;
            gui_insert_character_into_buffer(
                text_shaping_buffer,
                cur_obj->OffsetToText + (object_no * 2),
                (UI_character_type) dummy_char);
            text_len++;
            object_position[object_no] = cur_obj->OffsetToText / 2 + object_no + 1;
            object_no++;
        }
        cur_obj = cur_obj->next;
    }

    unshaped_text_len = text_len;
#ifdef __MMI_ZI__
#ifdef __MMI_ZI_V7__
    ZiStringShapeEx(ZI8_LANG_AR, (U16*) & text_len, (U16*) text_shaping_buffer, (U16*) OrgToSha, NULL);
#else /* __MMI_ZI_V7__ */ 
    ArabicStringShape2((U16*) & text_len, (U16*) text_shaping_buffer, (U16*) OrgToSha, NULL);
#endif /* __MMI_ZI_V7__ */ 
#else 
	t = text_len;
    //ArabicShapeEngine2(&t, text_shaping_buffer, OrgToSha, NULL);
    ArabicShapeEngine2_ext(&t, text_shaping_buffer, OrgToSha, NULL);
	text_len = t;
#endif 

}
#endif /* __MMI_LANG_ARABIC__ */ 



/*****************************************************************************
 * FUNCTION
 *  gui_show_EMS_input_box
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_EMS_input_box(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(__MMI_MAINLCD_220X176__)
	b->flags |= UI_EMS_INPUT_BOX_TRANSPARENT_BACKGROUND;
#endif	
    gui_show_EMS_input_box_ext(b, (-1), (-1));
}


/*****************************************************************************
 * FUNCTION
 *  gui_show_EMS_input_box_ext
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b               [?]         
 *  move_to_x       [IN]        
 *  move_to_y       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_show_EMS_input_box_ext(UI_EMS_input_box *b, S32 move_to_x, S32 move_to_y)
{	
   s32					x1,y1;	/* x1,y1 -> left-top corner of the editor windows */
   s32					x2,y2;	/* x2,y2 -> right-bottom of the editor windows */
	UI_filled_area		f;   
	byte					selection_flag=0;
	byte					overwrite_mode_flag=0;
	s32					tx=0,ty;/* specify the positon at which the text display starts */
	EMSData*				data=b->data;
   UI_character_type current_character = 0, pre2_character = 0, pre_character = 0;
	EMSPosition			current_position;
	EMSPosition			line_start_position;
	EMSPosition			previous_position;
	byte					object_type = 0;//guoyt added@09.11.02 for initializing.
	EMSObject*			current_object;
	s32					line_width=0,line_height=0;
	s32					break_line_width,break_line_height;
	S32					object_width,object_height;
	byte					done=0;
	byte					display_done;
	EMSObject*			current_text_format;
	EMSObject*			line_start_text_format;
	dword					flags=b->flags;
	s32					set_cursor_flag = 0;
	s32					center_x,box_width;
	s32					line_object_count = 0;
	s32					cursor_ax=0, cursor_ay=0;	/*	Absolute cursor co-ordinates	*/
	s32					break_line_object_count;
	s32					text_format_object_count,break_text_format_object_count;
	byte					disable_draw_flag		=	0;
	byte					*object_width_cache	=	EMS_object_width_cache;
	byte					*object_height_cache	=	EMS_object_height_cache;
	byte					word_highlight			=	0;
 
	s32					n_lines=0;
	byte					first_object_flag;
	EMSPosition			mapped_cursor_position;
	EMSPosition			mapped_current_position;
	EMSPosition			mapped_previous_position;
	byte					cursor_displayed_flag=0;
	byte					alignment,break_line_alignment=EMS_ALIGN_LANG_DEP,line_alignment;
	s32					line_text_object_count,break_line_text_object_count;
	color_t					normal_text_color		=	b->normal_text_color;
	color_t					selected_text_color	=	b->selected_text_color;
#if defined __MMI_LANG_HINDI__  && defined __MMI_LANG_ARABIC__	
	U16 unicode =	0;
	U16 clFill = 0;
#endif
 
#if(EMS_SHOW_HORIZONTAL_RULE_FOR_ALL_LINES)
	s32					minimum_line_height;
	s32					last_line_y	=	0;
#endif //EMS_SHOW_HORIZONTAL_RULE_FOR_ALL_LINES
 
#if(EMS_ENABLE_SPECIAL_FONT_DISPLAY)
	color_t				underline_color		=	b->underline_color;
	color_t				strikethrough_color	=	b->strikethrough_color;
	color_t				paragraph_color		=	b->paragraph_color;
	byte				underline_flag=0,strikethrough_flag=0;
#endif
#ifdef __MMI_HINDI_ALG__
	PU8				move_ems_cursor_prev_p			= 	data->textBuffer; 
	PU8				move_ems_cursor_curr_p			= 	data->textBuffer;
	PU8				temp_ems_cursor_p					=	data->textBuffer;
	PU8				move_ems_cursor_disp_prev_p	= 	data->textBuffer;
	PU8				move_ems_cursor_disp_curr_p	= 	data->textBuffer;
	U16				disp_ems_cluster[MAX_EMS_DISP_CLUSTER_LEN];
	s32				skip_ems_moves						=	1;/* for skipping by cluster */
	s32				saved_ems_skip_moves				=	1;
	s32				display_ems_skip_moves			=	1;/* for skipping by cluster while display */
	s32				saved_ems_display_skip_moves	=	0;
	s32				ems_cluster_width					=	0;
	s32				ems_cursor_width					=	0;
	U8					kdx									=	0;
	s32				hindi_ems_cluster_counter		=	0;
	
	//Performance related to store the cluster width
	s32				hindi_cluster_cache[50];
	s32				hindi_cache_counter=0;
	
	BOOL			set_hindi_text_format_flag = MMI_FALSE;
       EMSObject*		hindi_text_format = NULL;
	
#endif //__MMI_HINDI_ALG__

	PMT_BIDI_TYPES    default_direction= BIDI_L;
	s32					vbar_x = 0, vbar_button_x = 0;	

#ifdef __MMI_BIDI_ALG__
	U16 					logical_cur_pos; 
	U8 * 					temp_text_pt; 
	U16					temp_text_position = 0;	
	U8						temp_text_buffer[MAX_NUM_OF_CHAR_FOR_EACH_LINE];
	U16					temp_cursor_position = 0; 	
	U16					visual_str_len; 
	U16 					visual_cur_pos;
	U16 					logical_hilight_start;
   U16 					logical_hilight_end;
	U16 					visual_hilight_start;
	U16 					visual_hilight_end;
	U16					temp_hilight_start;
	U16					temp_hilight_end;
   PMT_BIDI_TYPES    default_line_direction= BIDI_L;
   pBOOL 				reverse_flag = MMI_FALSE;
	s32					num_char;
   U32					iCount;
   pBOOL 				arabic_char_flag= MMI_FALSE;
   U8						lastest_char_type;
   UI_character_type test_current_character;
 
   U16					text_len;
   PU8					ptr_text_buffer = NULL;
  
#endif
#if defined(__MMI_LANG_VIETNAMESE__)
	UI_character_type 	dummy_c 		= 0;
	viet_tone_mark 		tone_mark 	= VIET_TONE_NONE;
	viet_vowel_letter 	viet_vowel 	= VIET_VOWEL_NONE;
#endif	
	if (((-1)!=move_to_x)||((-1)!=move_to_y))
	{
		flags |= UI_EMS_INPUT_BOX_DISABLE_DRAW;
	}
/*Guojian Add Start For 10433  Ver: GreenStone  on 2008-11-21 16:57 */
#if defined(__MMI_ARSHAPING_ENGINE__)
		memset(shaping_str, 0, MAX_TEXT_LENGTH * BIDI_ENCODING_LENGTH);
#endif
/*Guojian Add End  For 10433  Ver: GreenStone  on 2008-11-21 16:57 */

 
#if defined( __MMI_ARSHAPING_ENGINE__ )
 
if((b->data->textLength/ENCODING_LENGTH)!=0)
{
   if(!(flags & UI_EMS_INPUT_BOX_VIEW_MODE))
   {
	   text_len = (b->data->textLength/2);
	   pfnUnicodeStrncpy((S8 *)shaping_str, (const S8 *)b->data->textBuffer,b->data->textLength/ENCODING_LENGTH);
		#ifdef __MMI_ZI_ARABIC__ 
		{
			ArabicStringShape((U16*)&text_len, (U16*)b->data->textBuffer);
		}
		#else
		{
			//ArabicShapeEngine((U16*)&text_len, (U16*)b->data->textBuffer); //function will reshaped the input string
			ArabicShapeEngine_ext((PU16)&text_len, (PU16)b->data->textBuffer);
		}
		#endif
   }
   else
   {
	   text_len = (b->data->textLength/2);
	   ptr_text_buffer = b->data->textBuffer;
	   pfnUnicodeStrncpy((S8 *)shaping_str, (const S8 *)b->data->textBuffer,b->data->textLength/ENCODING_LENGTH);
		#ifdef __MMI_ZI_ARABIC__ 
		{
			ArabicStringShape((U16*)&text_len, (U16*)shaping_str);
		}
		#else
		{
			//ArabicShapeEngine((U16*)&text_len, (U16*)shaping_str); //function will reshaped the input string
			ArabicShapeEngine_ext((PU16)&text_len, (PU16)shaping_str);
		}
		#endif   
	   b->data->textBuffer = (U8*)shaping_str;
#ifdef __MMI_HINDI_ALG__
	move_ems_cursor_prev_p			= 	data->textBuffer; 
	move_ems_cursor_curr_p			= 	data->textBuffer;
	temp_ems_cursor_p					=	data->textBuffer;
	move_ems_cursor_disp_prev_p	= 	data->textBuffer;
	move_ems_cursor_disp_curr_p	= 	data->textBuffer;
#endif
	}
 
}
 
#endif
 
 
#if defined __MMI_LANG_HINDI__  && defined __MMI_LANG_ARABIC__	
	HINDI_PRESENT		=	0;
	ARABIC_PRESENT	=	0;
	while((data->textBuffer[clFill]!='\0' || data->textBuffer[clFill+1]!='\0')&& !HINDI_PRESENT)
	{
		unicode 	= 	data->textBuffer[clFill];
		unicode	|=	(data->textBuffer[clFill+1]<<8);
		if(UI_HINDI_CHARACTER_UCS2_RANGE(unicode))
		{
			HINDI_PRESENT		=	1;
			ARABIC_PRESENT	=	0;
		}
		clFill+=2;
	}
#endif
 

#ifdef __MMI_BIDI_ALG__
	if(!(flags & UI_EMS_INPUT_BOX_DISABLE_DRAW) &&  (data->textLength !=0))
	{
		bidi_get_char_type(data->textBuffer, (U8 *)&default_direction);
		if(default_direction == BIDI_R || default_direction == AL)
	      default_direction = BIDI_R;
	   else
	      default_direction = BIDI_L;

		for(iCount=0;iCount<(U32)(data->textLength>>1);iCount++)
		{
			bidi_get_char_type((U8*)(data->textBuffer+iCount*ENCODING_LENGTH),(U8*)&lastest_char_type);
			if ( (lastest_char_type==AL) || (lastest_char_type==AN) || (lastest_char_type ==BIDI_R))
			{
				arabic_char_flag= MMI_TRUE;
				break;
			}
		}
 	}
#endif



	if(r2lMMIFlag)
		x1=	b->x +  b->vbar.width;
	else
		x1=	b->x;
	y1	=	b->y;
	x2	=	b->x/*x1*/+b->width-1;
	y2	=	y1+b->height-1;
	b->display_height=0;

#ifdef __MMI_HINDI_ALG__
	memset(disp_ems_cluster,0,sizeof(disp_ems_cluster));
#endif //__MMI_HINDI_ALG__

	if(flags & UI_EMS_INPUT_BOX_DISABLE_DRAW) disable_draw_flag=1;
	if(flags & UI_EMS_INPUT_BOX_VIEW_MODE) 	EMS_view_mode=1;
	else EMS_view_mode=0;
	if(!disable_draw_flag)
	{	
		if(flags & UI_EMS_INPUT_BOX_STATE_SELECTED)			f = *b->selected_filler;
		else if(flags & UI_EMS_INPUT_BOX_STATE_DISABLED) 	f = *b->disabled_filler;
		 
		else f = *b->normal_filler;

		if(r2lMMIFlag)
			coolsand_UI_set_clip(x1-b->vbar.width, y1, x2+2/*+b->vbar.width*/, y2+2);
		else
			coolsand_UI_set_clip(x1, y1, x2+2, y2+2);
		 
		if(!(flags	& UI_EMS_INPUT_BOX_DISABLE_BACKGROUND)) /* if not disable background then do ... */
		{
			if(!(flags & UI_EMS_INPUT_BOX_INCLUDE_INFO_AREA)) /* not include info area */
			{
				 
				if(r2lMMIFlag)
				{
					if ( (wgui_is_wallpaper_on_bottom()==MMI_TRUE) && (flags & UI_EMS_INPUT_BOX_TRANSPARENT_BACKGROUND))
					{
						gdi_draw_solid_rect(x1-b->vbar.width, y1, x2/*+b->vbar.width*/, y2, GDI_COLOR_TRANSPARENT);
					}
					else
					{
	 				coolsand_UI_draw_filled_area(x1-b->vbar.width, y1, x2/*+b->vbar.width*/, y2, &f);
					}
				}
				else
				{
					if ( (wgui_is_wallpaper_on_bottom()==MMI_TRUE) && (flags & UI_EMS_INPUT_BOX_TRANSPARENT_BACKGROUND))
					{
						gdi_draw_solid_rect(x1,y1,x2,y2, GDI_COLOR_TRANSPARENT);
					}
					else
					{
					coolsand_UI_draw_filled_area(x1, y1, x2, y2, &f);
					}
				}
				 

			}
			else /* include info area */
			{
				if(r2lMMIFlag)
	 				coolsand_UI_set_clip(	x1-b->vbar.width,
	 											y1-wgui_EMS_inputbox_information_bar_height,
	 											x2/*+b->vbar.width*/,
	 											y2	);
				else
					coolsand_UI_set_clip(	x1,
												y1-wgui_EMS_inputbox_information_bar_height,
												x2,
												y2	);

 				f.flags 	= 	UI_FILLED_AREA_TYPE_BITMAP;
 				f.b 		= 	get_image(WGUI_VOLUME_LEVEL1);
 				if(r2lMMIFlag)
 				{
					 
					if ( (wgui_is_wallpaper_on_bottom()==MMI_TRUE) && (flags & UI_EMS_INPUT_BOX_TRANSPARENT_BACKGROUND))
					{
						gdi_draw_solid_rect(x1-b->vbar.width,y1-wgui_EMS_inputbox_information_bar_height,x2/*+ b->vbar.width*/,y2, GDI_COLOR_TRANSPARENT);
					}
					else
					{
	 				coolsand_UI_draw_filled_area(	x1-b->vbar.width,
				 											y1-wgui_EMS_inputbox_information_bar_height,
				 											x2/*+ b->vbar.width*/,
				 											y2,
				 											&f	);
					}
					 
 	 				coolsand_UI_draw_rectangle(	x1-b->vbar.width,
			 	 										y1,
			 	 										x2/*+ b->vbar.width*/,
			 	 										y2,
			 	 										b->boundary_color	);
 				}
 				else
				{
					 
					if ( (wgui_is_wallpaper_on_bottom()==MMI_TRUE) && (flags & UI_EMS_INPUT_BOX_TRANSPARENT_BACKGROUND))
					{
						gdi_draw_solid_rect(x1,y1-wgui_EMS_inputbox_information_bar_height,x2,y2, GDI_COLOR_TRANSPARENT);
					}
					else
					{
	 				coolsand_UI_draw_filled_area(	x1,
				 											y1-wgui_EMS_inputbox_information_bar_height,
				 											x2,
				 											y2,
				 											&f	);
					}
					 
 					coolsand_UI_draw_rectangle(x1, y1, x2,y2, b->boundary_color);
				}

 				if(flags & UI_MULTI_LINE_INPUT_BOX_USE_PRESET_CLIPPING)
 					gui_set_clip_preset(x1, y1, x2+2, y2+2);
 				else
					coolsand_UI_set_clip(x1, y1, x2+2, y2+2);
			}
		}
	}
	if(!(flags & UI_EMS_INPUT_BOX_DISABLE_SCROLLBAR)) x2 -= b->vbar.width;
	box_width	=	(x2-x1-2);
	center_x		=	x1 + (box_width>>1)	+	1;
	ty				=	y1 + 2					+	b->display_y;
	if(flags & UI_EMS_INPUT_BOX_DISPLAY_HEADER)
	{	s32 h,ty1,ty2;
		h						=	b->measure_header();
		b->header_height	=	h;
		ty1					=	ty-2;
		ty2					=	ty+h-1;
		if(!disable_draw_flag && (ty2>y1)) b->display_header(x1,ty1,x2,ty2);
		ty 					+= h;
	}
	else
	{
		b->header_height	=	0;
	}
	if(!disable_draw_flag)
	{	

		if(flags & UI_EMS_INPUT_BOX_STATE_MULTITAP) 
		{
		#ifdef __MMI_BIDI_ALG__
			if(arabic_char_flag)
				selection_flag=0;
			else
		#endif	
				selection_flag=1;
		}
		if((flags & UI_EMS_INPUT_BOX_OVERWRITE_MODE) && (!selection_flag)) 	overwrite_mode_flag=1;
		coolsand_UI_set_clip(x1+1, y1+1, x2, y2-1);
		EMS_SET_FONT(&UI_EMS_inputbox_medium_font);
		//coolsand_UI_set_text_clip(x1+2,y1+2,x2-2,y2-2);
		EMS_SET_TEXT_COLOR(normal_text_color);
	}
	line_start_position.Object			=	NULL;
	line_start_position.OffsetToText	=	0;
	current_text_format					=	NULL;
	cursor_ay								=	0;
	text_format_object_count			=	0;
	b->first_displayed_line				=	-1;
	b->last_displayed_line				=	-1;
	EMS_map_position(data,&data->CurrentPosition,&mapped_cursor_position);
	line_alignment 						= 	EMS_ALIGN_LANG_DEP;

#if defined(__MMI_LANG_VIETNAMESE__)
       
      if((flags & UI_EMS_INPUT_BOX_DISABLE_DRAW) &&  (data->textLength !=0))
      {
         EMSPosition			temp_cursor_position_VI;
         EMS_map_position(data,&data->CurrentPosition,&temp_cursor_position_VI);
         object_type=coolsand_UI_EMS_input_box_get_next_object(data,&temp_cursor_position_VI,&current_character,&current_object);
         if (1==object_type)
         {
            if ((current_character >= 0x0300) && (current_character <= 0x0323))
            {
               EMS_map_position(data,&data->CurrentPosition,&temp_cursor_position_VI);
               object_type=coolsand_UI_EMS_input_box_get_previous_object(data,&temp_cursor_position_VI,&dummy_c,&current_object);//get next character
               if (1==object_type)
               {
                  viet_vowel = mmi_viet_vowel_letter(dummy_c);
                  if (VIET_VOWEL_NONE != viet_vowel)
                  {
                     tone_mark = mmi_viet_tone_mark(current_character);
                     if (VIET_TONE_NONE != tone_mark)
                     {
                        current_character = mmi_viet_combine_vowel_tone(viet_vowel,tone_mark);
                        coolsand_UI_EMS_input_box_forward_cursor(b);
                     }
                  }
               }
            }
         }
      }
#endif

	while(!done)
	{	/*	First Pass: Find the start and end of a line, line height and line width	*/
		line_start_text_format=current_text_format;
	#ifdef __MMI_MESSAGES_EMS__
		coolsand_UI_EMS_input_box_set_text_format(b,current_text_format);
	#else
	{	EMS_SET_FONT(&UI_EMS_inputbox_medium_font);
		EMS_SET_TEXT_COLOR(UI_EMS_inputbox_text_color);
		b->flags  &=  ~(	UI_EMS_INPUT_BOX_CURRENT_ALIGN_LEFT		|
								UI_EMS_INPUT_BOX_CURRENT_ALIGN_RIGHT	|
								UI_EMS_INPUT_BOX_CURRENT_ALIGN_CENTER	);
	}
	#endif
		break_line_height						=	line_height = coolsand_UI_get_character_height();
		current_position						=	line_start_position;
		break_text_format_object_count	=	text_format_object_count;
 
    #ifdef __MMI_HINDI_ALG__
        break_line_width = line_width = 1;
    #else 
        break_line_width = line_width = 0;
    #endif 
  
		break_line_object_count				=	line_object_count			=	0;
		break_line_text_object_count		=	line_text_object_count	=	0;
	#ifdef __MMI_HINDI_ALG__
		hindi_cache_counter		=	0;
		saved_ems_skip_moves 	= 	skip_ems_moves = 1;
		object_width 				= 	0;
		move_ems_cursor_prev_p 	=	(data->textBuffer + line_start_position.OffsetToText); 
	#endif //__MMI_HINDI_ALG__
		first_object_flag 		= 	1;
		while(!done)
		{
		   previous_position = current_position;
			if(TestLastPosition(data,&current_position))
			{
			   current_character = 0;
				EMS_SET_BREAK_POSITION();
				if (UI_EMS_inputbox_current_font.italic && pre_character != 0)
				{
					break_line_width += 3 ;
				}
				done = 1;
				break;
			}
		#ifdef __MMI_HINDI_ALG__
			//PRINT_INFORMATION_2((MMI_TRACE_INFO, "coolsand_UI_show_EMS_input_box[skip_ems_moves]:%d  ",skip_ems_moves));
			while(skip_ems_moves--)
			{
		#endif //__MMI_HINDI_ALG__
				object_type = coolsand_UI_EMS_input_box_get_next_object(	data,
																							&current_position,
																							&current_character,
																							&current_object	);
		#ifdef __MMI_HINDI_ALG__
				if( (skip_ems_moves >= 1) && (object_type == 2) && (current_object->Type == EMS_TYPE_TEXT_FORMAT))
				{
					object_type = coolsand_UI_EMS_input_box_get_next_object(	data,
																								&current_position,
																								&current_character,
																								&current_object );
				}


			}
			// Not when object is images 
			if( HF_HINDI_RANGE(current_character) && !(object_type == 2))
			{
				object_type = 1;
			}
		#endif//__MMI_HINDI_ALG__
			switch(object_type)
			{	case 0:
					done=1;
					break;
				case 1: /* character */
				#if(EMS_DISPLAY_SPECIAL_CHARACTERS)
					if(	UI_TEST_CR_CHARACTER(current_character)	||
							UI_TEST_LF_CHARACTER(current_character) 	|| 
							UI_TEST_ESC_CHARACTER(current_character) )
					{	object_width	=	10;
						object_height	=	10;
					}
					else
				#else
				#if(EMS_USE_CRLF_COMBINATION)
					if(	UI_TEST_CR_CHARACTER(current_character) 				&&
							(current_position.OffsetToText<data->textLength) 	&&
							UI_TEST_LF_CHARACTER(data->textBuffer[current_position.OffsetToText])	)
					{	
					#ifdef __MMI_HINDI_ALG__
						skip_ems_moves = 1;
					#endif
						object_width	=	0;
						object_height	=	line_height;
					}
					else if(UI_TEST_LF_CHARACTER(current_character))
					{	
					#ifdef __MMI_HINDI_ALG__
						skip_ems_moves = 1;
					#endif
						object_width	=	0;
						object_height	=	line_height;
					}
					else
				#endif	/*	EMS_USE_CRLF_COMBINATION */
				#if(EMS_USE_ESC_COMBINATION)
					if(UI_TEST_ESC_CHARACTER(current_character))
					{	
					#ifdef __MMI_HINDI_ALG__
						skip_ems_moves = 1;
					#endif
						object_width	=	0;
						object_height	=	line_height;
					}
					else
				#endif	/*	EMS_USE_ESC_COMBINATION	*/
				#endif	/*	EMS_DISPLAY_SPECIAL_CHARACTERS */
					if (UI_STRING_END_OF_STRING_CHARACTER(current_character))
					{
 
						#ifdef __MMI_HINDI_ALG__
							skip_ems_moves = 1;
						#endif
 
						object_width	=	0;
						object_height	=	line_height;
					}
					else
               {
					#if defined(__MMI_LANG_VIETNAMESE__)
	         		if ((current_character > 0x0040) && (current_character < 0x01B1))
	         		{
	         			object_type = coolsand_UI_EMS_input_box_get_next_object(data,&current_position,&dummy_c,&current_object);//get next character 
	         			if (1 == object_type)
	         			{
	         				tone_mark = mmi_viet_tone_mark(dummy_c);
	         				if (VIET_TONE_NONE != tone_mark)
	         				{
	         					viet_vowel = mmi_viet_vowel_letter(current_character);
	         					if (VIET_VOWEL_NONE != viet_vowel)
	         					{
	         						current_character = mmi_viet_combine_vowel_tone(viet_vowel,tone_mark);
	         					}
	         					else
	         					{
	         						object_type = coolsand_UI_EMS_input_box_get_previous_object(data,&current_position,&dummy_c,&current_object);
	         					}
	         				}
	         				else
	         				{
	         					object_type = coolsand_UI_EMS_input_box_get_previous_object(data,&current_position,&dummy_c,&current_object);
	         				}
	         			}
	         			else if (0 != object_type)
	         			{
	         				object_type = coolsand_UI_EMS_input_box_get_previous_object(data,&current_position,&dummy_c,&current_object);
	         			}
	         		}
					#endif

					#ifdef __MMI_HINDI_ALG__
                                 if (UI_HINDI_CHARACTER_UCS2_RANGE(current_character))
                                 {
 
                                    coolsand_UI_measure_character(current_character,&object_width,&object_height);
				    if(object_height>line_height) line_height	=	object_height;
				    object_height = coolsand_UI_get_character_height();
 
                                 }
                                 else
                                 {
                                    coolsand_UI_measure_character(current_character,&object_width,&object_height);
                                 }
                                 //PRINT_INFORMATION_2((MMI_TRACE_INFO, "coolsand_UI_show_EMS_input_box[object_height]:%d  ",object_height));
					#else
						coolsand_UI_measure_character(current_character,&object_width,&object_height);
					#endif//__MMI_HINDI_ALG__

							
						if(!RequireToMoveCursor(pre_character, current_character))
						{
							object_width 	= 0;
						#ifdef __MMI_HINDI_ALG__
							skip_ems_moves = 1;
                      
							move_ems_cursor_prev_p+=2;
                      
						#endif 
						}
						else
						{
						#ifdef __MMI_HINDI_ALG__

							/* Move pointer by the cluster width */
							//For performance issue b->text is also assumed as current cluster pos
							//move_ems_cursor_curr_p = hf_get_next_cluster_p(data->textBuffer,move_ems_cursor_prev_p);
							move_ems_cursor_curr_p 	=	 hf_get_next_cluster_p(move_ems_cursor_prev_p,move_ems_cursor_prev_p);
							skip_ems_moves 			= 	move_ems_cursor_curr_p - move_ems_cursor_prev_p;//Number of moves
							skip_ems_moves 			= 	skip_ems_moves >> 1;
							saved_ems_skip_moves 	= 	skip_ems_moves;/* Save moves to skip the current_text_p also*/
					
						

							/* Calculate the width of current cluster as well as english character*/
							//object_width = hf_size_of_current_cluster(data->textBuffer,move_ems_cursor_curr_p,0);
							object_width = hf_size_of_current_cluster(move_ems_cursor_prev_p,move_ems_cursor_curr_p,0);
                     //for bold italic in hindi
							if(UI_EMS_inputbox_current_font.italic || UI_EMS_inputbox_current_font.bold)
								object_width	+=	1;
						   	//PRINT_INFORMATION_2((MMI_TRACE_INFO, "coolsand_UI_show_EMS_input_box[object_width]:%d  ",object_width));
							if(UI_TEST_CR_CHARACTER(current_character))
								object_width 	= 	0;
							move_ems_cursor_prev_p = move_ems_cursor_curr_p;
						#else
						//Arabic Connectivity issue
						#ifdef __MMI_LANG_ARABIC__
							if(!IS_ARABIC_CHARACTER(current_character))
					   #endif
							object_width 	+= 1;
							

						#endif//__MMI_HINDI_ALG__
						}
					}

					if(	(line_width+object_width+1 )>box_width 		||
							(UI_EMS_inputbox_current_font.italic&&(line_width+object_width+3 ) > box_width)	)
					{
						line_width += object_width;
					}
					else
					{	byte bflag;
						UI_EMS_INPUT_BOX_BREAK_CHARACTER(current_character,bflag);
						if(bflag) 
							 EMS_SET_NEXT_BREAK_POSITION();
						line_width += object_width;
					#ifdef __MMI_HINDI_ALG__
						if(UI_HINDI_CHARACTER_UCS2_RANGE(current_character))
						{
							hindi_ems_cluster_counter = line_object_count;
							while(saved_ems_skip_moves--)
							{
								object_width_cache[hindi_ems_cluster_counter]	=	0;
								object_height_cache[hindi_ems_cluster_counter]	=	(byte)object_height;
								hindi_ems_cluster_counter++;
							}
							hindi_cluster_cache[hindi_cache_counter] = object_width;
						}
						else
					#endif //__MMI_HINDI_ALG__
						{
							object_width_cache[line_object_count]	=	(byte)object_width;
							/*
							#ifdef __MMI_BIDI_ALG__
							if(arabic_char_flag)
								object_height_cache[line_object_count]=(byte)coolsand_UI_get_character_height()+2;
							else
							#endif
							*/
								object_height_cache[line_object_count]=(byte)object_height;
						}
						
						if(object_height>line_height) line_height	=	object_height;
					#ifdef __MMI_HINDI_ALG__
						line_object_count			+=	skip_ems_moves;
						line_text_object_count	+=	skip_ems_moves;
						hindi_cache_counter++;
					#else
						line_object_count++;
						line_text_object_count++;
					#endif //__MMI_HINDI_ALG__
						
						
						if(UI_STRING_LINE_BREAK_CHARACTER(current_character))
						{	EMS_SET_BREAK_POSITION();
							if(text_format_object_count<=1) line_alignment = EMS_ALIGN_LANG_DEP;
						}
						if(text_format_object_count)
						{	text_format_object_count--;
							if(	(text_format_object_count==0) 	&& 
									(current_position.OffsetToText<data->textLength)	)
							{
							#ifdef __MMI_MESSAGES_EMS__
								coolsand_UI_EMS_input_box_set_text_format(b,&UI_EMS_normal_text_format_object);
							#else
							{	EMS_SET_FONT			(&UI_EMS_inputbox_medium_font	);
								EMS_SET_TEXT_COLOR	(UI_EMS_inputbox_text_color	);
								b->flags &= ~( UI_EMS_INPUT_BOX_CURRENT_ALIGN_LEFT		|
													UI_EMS_INPUT_BOX_CURRENT_ALIGN_RIGHT	|
													UI_EMS_INPUT_BOX_CURRENT_ALIGN_CENTER	);
							}
							#endif
								object_height	= coolsand_UI_get_character_height();
								if(object_height>line_height) line_height	=	object_height;
								if(line_object_count==0) 		line_height	=	object_height;
							}
						}
					#ifdef __MMI_HINDI_ALG__
						//Hindi condition also added for word wrap issue
      				if (	(!UI_MULTI_LINE_INPUT_BOX_ALPHABETIC_CHARACTER(current_character)) &&
      						(!UI_HINDI_CHARACTER_UCS2_RANGE(current_character))	)
					#else
						if ((!UI_MULTI_LINE_INPUT_BOX_ALPHABETIC_CHARACTER(current_character)))
					#endif //__MMI_HINDI_ALG__
		      			{
				      		break_line_object_count = 0;
			      		}
					}
					pre2_character = 	pre_character;
					pre_character 	= 	current_character;
					break;
			#ifdef __MMI_MESSAGES_EMS__
				case 2:/* object */	
					if(current_object->Type == EMS_TYPE_TEXT_FORMAT)
					{	line_alignment = current_object->data->text_format.Alignment;
						coolsand_UI_EMS_input_box_set_text_format(b,current_object);
						text_format_object_count	=	((current_object->data->text_format.textLength)>>1);
						object_height					=	coolsand_UI_get_character_height();
						if(object_height>line_height) 	line_height	=	object_height;
						if(line_object_count == 0) 		line_height	=	object_height;
					}
					else
					{	coolsand_UI_EMS_input_box_measure_object(current_object,&object_width,&object_height);
						object_width += 2;
						if(first_object_flag)
                        {
                            object_width += 1;
                        }
                        else
                        {
                            line_width+=3;
                        }
                            
						object_height += 1;		/*	1 pixel gap between lines	*/
						if((line_width+object_width+1) > box_width)
						{	line_width += object_width;
							/*	Removed line breaking rule for objects	*/
							//	EMS_SET_BREAK_POSITION();
						}
						else
						{	if(object_height>line_height) line_height = object_height;
							line_width += object_width;
							if(first_object_flag) object_width -= 2;
							object_width_cache[line_object_count]	=	(s8)object_width;
							object_height_cache[line_object_count]	=	(s8)object_height;
							line_object_count++;
							/*	Removed line breaking rule for objects	*/
							//	EMS_SET_BREAK_POSITION();
						}
					}
					current_character = 0;
				#ifdef __MMI_HINDI_ALG__
					skip_ems_moves = 1;
				#endif //__MMI_HINDI_ALG__
					break;
 
			#else
				case 2:/* object */
				current_character = 0;	
				#ifdef __MMI_HINDI_ALG__
					skip_ems_moves = 1;
				#endif //__MMI_HINDI_ALG__
				break;
 
			#endif
			}
			first_object_flag = 0;
			if(	(line_width+1  >box_width)||
					((UI_EMS_inputbox_current_font.italic&&(line_width+3 )>box_width)&&(1==object_type))	||
					UI_STRING_LINE_BREAK_CHARACTER(current_character)	)
			{
			   if((break_line_object_count == 0) && (line_object_count > 0))
				{
				   EMS_SET_BREAK_POSITION();
					break_line_width -= object_width;
				}
				if (	(UI_EMS_inputbox_current_font.italic&&(line_width+3 )>box_width) &&
						pre_character!=0 )
				{
					break_line_width += 3 ;
				}
				break;
			}
		}
		
		/*	Second pass: Display the line	*/
		pre2_character = 	0;
		pre_character 	= 	0;

	#ifdef __MMI_HINDI_ALG__
		ems_cluster_width = 	0;
		ems_cursor_width 	= 	0;
	#endif //__MMI_HINDI_ALG__

		line_width				=	break_line_width;
		line_height				=	break_line_height;
		current_text_format	=	line_start_text_format;
		current_position		=	line_start_position;
		alignment				=	break_line_alignment;
		if(	(alignment!=b->text_format.Alignment) 	&&
				(break_line_text_object_count==0) 		&&
				(b->current_line==n_lines)					&&
				(EMS_view_mode==0)	)
			alignment = b->text_format.Alignment;
/*			Old code: For testing						*/
/*			else if((break_line_text_object_count!=0) && (data->textLength==0))
				alignment=b->text_format.Alignment;		*/
		switch(alignment)
		{
			case EMS_ALIGN_LEFT:	
				if(default_direction == BIDI_L)
		         tx = 	x1 + 2;	
				else
					tx =  x1 + line_width + 2;
				break;
			case EMS_ALIGN_RIGHT:		
				if(default_direction == BIDI_L)
				   tx = x2 - line_width	-	2;
				else
					tx = x1 + box_width;
				break;
			case EMS_ALIGN_CENTER:		
				if(default_direction == BIDI_L)
				   tx = center_x 	- (line_width >> 1);
				else
				   tx = center_x  + (line_width >> 1);
				break;
			case EMS_ALIGN_LANG_DEP:	
				if(default_direction == BIDI_L)
		         tx = 	x1 + 2;	
				else
					tx	=	x1 + box_width;
				break;
			default:			
				tx = x1 + 2;
				break;
		}
		if(tx < (x1+2)) 	tx=x1+2;
		cursor_ax = tx-(x1+2);
		if((b->first_displayed_line == -1) && (ty >= (y1+2))) 	b->first_displayed_line = 	n_lines;
		if((ty+line_height) <= y2) 										b->last_displayed_line	=	n_lines;
		b->line_positions[n_lines]			=	line_start_position;
		b->line_object_counts[n_lines]	=	(byte)break_line_object_count;
		b->line_x_positions[n_lines]		=	(byte)cursor_ax;
		n_lines++;
	#ifdef __MMI_MESSAGES_EMS__
		coolsand_UI_EMS_input_box_set_text_format(b,current_text_format);
	#else
		{	EMS_SET_FONT		(&UI_EMS_inputbox_medium_font	);
			EMS_SET_TEXT_COLOR(UI_EMS_inputbox_text_color	);
			b->flags &= ~(	UI_EMS_INPUT_BOX_CURRENT_ALIGN_LEFT		|
								UI_EMS_INPUT_BOX_CURRENT_ALIGN_RIGHT	|
								UI_EMS_INPUT_BOX_CURRENT_ALIGN_CENTER	);
		}
	#endif
		object_height					=	coolsand_UI_get_character_height();
		text_format_object_count	=	break_text_format_object_count;
		display_done					=	0;
		line_object_count				=	0;
	#ifdef __MMI_HINDI_ALG__
		hindi_cache_counter			=	0;
	#endif
		first_object_flag				=	1;
	#if(EMS_ENABLE_SPECIAL_FONT_DISPLAY)
		if(current_text_format != NULL)
		{	if(current_text_format->data->text_format.isUnderline == KAL_TRUE) 		underline_flag=1;
			else 																							underline_flag=0;
			if(current_text_format->data->text_format.isStrikethrough==KAL_TRUE) 	strikethrough_flag=1;
			else 																							strikethrough_flag=0;
		}
	#endif

	#ifdef __MMI_BIDI_ALG__
		if(!(flags & UI_EMS_INPUT_BOX_DISABLE_DRAW) && arabic_char_flag)
		{

			//get last character unicode of previous line	
			if(n_lines==1)
			{
				if(break_line_text_object_count < data->textLength/2)
				{
				#if(EMS_BYTE_ORDER_BIG_ENDIAN)
					test_current_character = ( data->textBuffer [  	current_position.OffsetToText +
																				  	((1+break_line_text_object_count)*ENCODING_LENGTH)-1	] 	|
													   (data->textBuffer[	current_position.OffsetToText + 
													 							  	((1+break_line_text_object_count)*ENCODING_LENGTH)-2	]<<8) );
				#else
					test_current_character = ( data->textBuffer [	current_position.OffsetToText +
																					((1+break_line_text_object_count)*ENCODING_LENGTH)-2	]		|
														(data->textBuffer[   current_position.OffsetToText	+
																					((1+break_line_text_object_count)*ENCODING_LENGTH)-1	]<<8) );
				#endif
				}
				else
				{
				#if(EMS_BYTE_ORDER_BIG_ENDIAN)
					test_current_character = (	data->textBuffer	[	current_position.OffsetToText	+
																					((break_line_text_object_count)*ENCODING_LENGTH)-1	]		|
														(data->textBuffer	[	current_position.OffsetToText	+
																					((break_line_text_object_count)*ENCODING_LENGTH)-2	]<<8)	);
				#else
					test_current_character = (	data->textBuffer	[	current_position.OffsetToText	+
																					((break_line_text_object_count)*ENCODING_LENGTH)-2	]		|
														(data->textBuffer	[	current_position.OffsetToText	+
																					((break_line_text_object_count)*ENCODING_LENGTH)-1	]<<8)	);
				#endif
				}

			}
			else
			{
			#if(EMS_BYTE_ORDER_BIG_ENDIAN)
				test_current_character = (	data->textBuffer[current_position.OffsetToText-1]			|
													(data->textBuffer[current_position.OffsetToText-2]<<8)	);
			#else
				test_current_character = (	data->textBuffer[current_position.OffsetToText-2]			|
													(data->textBuffer[current_position.OffsetToText-1]<<8)	);
			#endif
			}
			 

			if(line_text_object_count > break_line_text_object_count)
				num_char = break_line_text_object_count+1;
			else
				num_char = break_line_text_object_count;//line_text_object_count;
		  	MMI_ASSERT(  !( num_char >  ((MAX_NUM_OF_CHAR_FOR_EACH_LINE>>1)-1)  ) );

		  	
	      if( ((mapped_cursor_position.OffsetToText)/ENCODING_LENGTH) < ((current_position.OffsetToText)/ENCODING_LENGTH))
	      	logical_cur_pos = 0xFFFF;
	      else if( ( ( (mapped_cursor_position.OffsetToText) - (current_position.OffsetToText) )/ENCODING_LENGTH) > (num_char))
	      	logical_cur_pos = 0xFFFF;
	      else
	      	logical_cur_pos = ( (mapped_cursor_position.OffsetToText) - (current_position.OffsetToText) )/ENCODING_LENGTH;

			if((n_lines > 1 && logical_cur_pos == 0) )
			{
				//	pfnEncodingSchemeToUnicode(&test_current_character, (data->textBuffer + current_position.OffsetToText - ENCODING_LENGTH));
				if(!UI_STRING_LINE_BREAK_CHARACTER(test_current_character))
					logical_cur_pos = 0xFFFF;
			}
			//if(UI_STRING_LINE_BREAK_CHARACTER(current_character))
			//	logical_cur_pos = 0xFFFF;

#if 0
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
#endif
	      {
				logical_hilight_start = 0;
				logical_hilight_end = 0;
	      }


	      bidi_get_char_type(	data->textBuffer + current_position.OffsetToText , 
	      							(U8 *)&default_line_direction);
			if(default_line_direction == BIDI_R || default_line_direction == AL)
		      default_line_direction = BIDI_R;
			else
		      default_line_direction = BIDI_L;
	      if(default_direction == BIDI_R && default_line_direction == BIDI_R)
	      	reverse_flag = MMI_TRUE;
	      else if(default_direction == BIDI_R && default_line_direction == BIDI_L)
	      	reverse_flag = MMI_TRUE;
	      else if(default_direction == BIDI_L && default_line_direction == BIDI_R)
      	   reverse_flag = MMI_FALSE;
	      else if(default_direction == BIDI_L && default_line_direction == BIDI_L)
	      	reverse_flag = MMI_FALSE;


	      bidi_main(	data->textBuffer + current_position.OffsetToText,
	       				(U16) num_char, 
	       				logical_cur_pos, 
	         			visual_str,
	         			&visual_str_len,
	         			&visual_cur_pos, 
	         			reverse_flag,
	         			MMI_bidi_input_type,
	    				   logical_hilight_start,
	    				   logical_hilight_end,
							&visual_hilight_start,
							&visual_hilight_end	);		
	      

			temp_text_position = current_position.OffsetToText;
			memcpy(	temp_text_buffer,
						data->textBuffer + current_position.OffsetToText,
						visual_str_len*ENCODING_LENGTH	);
			memcpy(	data->textBuffer + current_position.OffsetToText,
						visual_str,
						visual_str_len*ENCODING_LENGTH	);
			temp_cursor_position = mapped_cursor_position.OffsetToText;
			if(visual_cur_pos != 0xFFFF)
			{
				mapped_cursor_position.OffsetToText = line_start_position.OffsetToText + (visual_cur_pos* ENCODING_LENGTH);
			}

#if 0
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
#endif
		
		}
 	#endif // endif of __MMI_BIDI_ALG__

		while(!display_done)
		{	previous_position		=	current_position;
			if(line_object_count	>=	break_line_object_count) break;
			if(flags & UI_EMS_INPUT_BOX_WORD_HIGHLIGHT)
			{	if(current_position.OffsetToText == b->highlight_start_position.OffsetToText)		
				{
				#ifdef __MMI_BIDI_ALG__
					if(arabic_char_flag)
						word_highlight=0;
					else
				#endif	
					word_highlight=1;
				}
				if(current_position.OffsetToText == b->highlight_end_position.OffsetToText)		
					word_highlight=0;
			}
			object_type=coolsand_UI_EMS_input_box_get_next_object( 	data,
																					&current_position,
																					&current_character,
																					&current_object	);
			EMS_map_position(data,	&previous_position,	&mapped_previous_position	);
			EMS_map_position(data,	&current_position	,	&mapped_current_position	);
			switch(object_type)
			{  int no_show_char;
				case 0:	
					current_character	=	0;
					display_done		=	1;
					break;
				case 1:	/* character */
					no_show_char = 0;
					if(!(flags & UI_EMS_INPUT_BOX_DISABLE_DRAW))
					{
						if (((ty + line_height) < (y1 + 2))	||	((ty-line_height) > y2)) 
					   {
							no_show_char = 1;
						}
					}
				#ifdef __MMI_HINDI_ALG__
					if(set_hindi_text_format_flag == MMI_TRUE)
					{
#ifdef __MMI_MESSAGES_EMS__					
						coolsand_UI_EMS_input_box_set_text_format(b,hindi_text_format);
#endif
						set_hindi_text_format_flag = MMI_FALSE;
						current_text_format = hindi_text_format;
					}
				#endif

				#if defined(__MMI_LANG_VIETNAMESE__)
					if ((current_character > 0x0040) && (current_character < 0x01B1))
					{
						object_type = coolsand_UI_EMS_input_box_get_next_object(	data,
																									&current_position,
																									&dummy_c,
																									&current_object	);//get next character 
						if (1 == object_type)
						{
							tone_mark = mmi_viet_tone_mark(dummy_c);
							if (VIET_TONE_NONE != tone_mark)
							{
								viet_vowel = mmi_viet_vowel_letter(current_character);
								if (VIET_VOWEL_NONE != viet_vowel)
								{
									current_character = mmi_viet_combine_vowel_tone(viet_vowel,tone_mark);
								}
								else
								{
									object_type			=	coolsand_UI_EMS_input_box_get_previous_object(	data,
																															&current_position,
																															&dummy_c,
																															&current_object	);
								}
							}
							else
							{
								object_type = coolsand_UI_EMS_input_box_get_previous_object(	data,
																												&current_position,
																												&dummy_c,
																												&current_object	);
							}
						}
						else
						{
							object_type = coolsand_UI_EMS_input_box_get_previous_object(	data,
																											&current_position,
																											&dummy_c,
																											&current_object	);
						}
					}
				#endif

				#ifdef __MMI_BIDI_ALG__
					if(!(flags & UI_EMS_INPUT_BOX_DISABLE_DRAW) && arabic_char_flag)
					{
						coolsand_UI_measure_character(current_character,&object_width,&object_height);
						if(!RequireToMoveCursor(pre_character, current_character))
							object_width = 0;
						else{
						//Arabic Connectivity issue
						#ifdef __MMI_LANG_ARABIC__
							if(!IS_ARABIC_CHARACTER(current_character))
					   #endif
							object_width++;
						}
						
				   	object_width_cache[line_object_count] = object_width;
					}
				#endif				
               object_width	=	object_width_cache[line_object_count];
					object_height	=	object_height_cache[line_object_count];
				#ifdef __MMI_HINDI_ALG__
					if(UI_HINDI_CHARACTER_UCS2_RANGE(current_character))/*space shud have been added in the Hindi range */
					{
						
						if(	!( TestLastPosition(data, &previous_position) ) )
						{
							//For performance issues first argument= second srgument
							//move_ems_cursor_disp_curr_p = hf_get_next_cluster_p(data->textBuffer,move_ems_cursor_disp_prev_p);
							move_ems_cursor_disp_curr_p 	= 	hf_get_next_cluster_p(move_ems_cursor_disp_prev_p,move_ems_cursor_disp_prev_p);
							display_ems_skip_moves 			= 	move_ems_cursor_disp_curr_p - move_ems_cursor_disp_prev_p;
							display_ems_skip_moves 			= 	display_ems_skip_moves >> 1;
							
							
							//ems_cluster_width=ems_cursor_width=hf_size_of_current_cluster(data->textBuffer,move_ems_cursor_disp_curr_p,0);
						#if 0 
/* under construction !*/
						#else
							ems_cluster_width	=	ems_cursor_width	=	hindi_cluster_cache[hindi_cache_counter];
						#endif
							
							
							
							move_ems_cursor_disp_prev_p 	= move_ems_cursor_disp_curr_p;
							saved_ems_display_skip_moves 	= display_ems_skip_moves;
							display_ems_skip_moves--;
							for(kdx=0;kdx<display_ems_skip_moves;kdx++)
							{
								object_type = coolsand_UI_EMS_input_box_get_next_object(	data,
																											&current_position,
																											&current_character,
																											&current_object	);
								if(display_ems_skip_moves>=1 && object_type==2 && current_object->Type == EMS_TYPE_TEXT_FORMAT)	
								{
									    hindi_text_format = current_object;
										set_hindi_text_format_flag = MMI_TRUE;
										object_type = coolsand_UI_EMS_input_box_get_next_object(	data,
																											&current_position,
																											&current_character,
							    																			&current_object	);
																											
								}


							}
						}
					}
					else
					{
						move_ems_cursor_disp_prev_p	+=	2;
						saved_ems_display_skip_moves 	= 	1;
					}
				#endif //__MMI_HINDI_ALG__
					if(!disable_draw_flag)
					{	EMS_SET_TEXT_COLOR(normal_text_color);
						if(default_direction == BIDI_R)
						{
							 
						#ifdef __MMI_HINDI_ALG__
							if(UI_HINDI_CHARACTER_UCS2_RANGE(current_character))
							{
								tx 			-= 	ems_cluster_width;
								cursor_ax	-=		ems_cursor_width;
							}
							else
						#endif //__MMI_HINDI_ALG__
							{
      						tx 			-=	 	object_width;
								cursor_ax	-=		object_width;
							}
							 
						}
						if(	word_highlight 																						|| 
								(	overwrite_mode_flag 		&&
									!cursor_displayed_flag 	&&
									(mapped_previous_position.OffsetToText==mapped_cursor_position.OffsetToText) 
								) 																											||
								( 	selection_flag 			&&
									(mapped_current_position.OffsetToText==mapped_cursor_position.OffsetToText)
								)	
						  )
						{	s32 rx1,ry1,rx2,ry2;
							rx1	=	tx;
 
						#ifdef __MMI_HINDI_ALG__
							if(UI_HINDI_CHARACTER_UCS2_RANGE(current_character))
							{
								S32 h,w;
								coolsand_UI_measure_character(current_character, &w, &h);
 
								ry1	=	line_height-object_height + ty;
 
								rx2	=	rx1 + ems_cluster_width - 1;
								ry2	=	ry1 + h - 1;
								PRINT_INFORMATION_2((MMI_TRACE_INFO, "coolsand_UI_show_EMS_input_box[ems_cluster_width]:%d  ",ems_cluster_width));
							}
							else
						#endif //__MMI_HINDI_ALG__
							{
								ry1	=	line_height-object_height + ty;
								rx2	=	rx1 + object_width - 1;
								ry2	=	ry1 + object_height - 1;
							}
 
							EMS_SET_TEXT_COLOR(selected_text_color);
							coolsand_UI_fill_rectangle(rx1,ry1,rx2,ry2,b->selection_color);
						}
 
						#ifdef __MMI_HINDI_ALG__
							if(UI_HINDI_CHARACTER_UCS2_RANGE(current_character))
							{
 
								EMS_MOVE_TEXT_CURSOR(tx,line_height-object_height+ty);
 
							}
							else
						#endif //__MMI_HINDI_ALG__
 
						EMS_MOVE_TEXT_CURSOR(tx,line_height-object_height+ty);
					#if(EMS_DISPLAY_SPECIAL_CHARACTERS)
						if(UI_TEST_CR_CHARACTER(current_character))
							coolsand_UI_show_transparent_image(	tx,
																			line_height-object_height+ty,
																			(UI_image_type)EMS_CR_display_character,
																			0	);
						else if(UI_TEST_LF_CHARACTER(current_character))
							coolsand_UI_show_transparent_image(	tx,
																			line_height-object_height+ty,
																			(UI_image_type)EMS_LF_display_character,
																			0	);
						else if(UI_TEST_ESC_CHARACTER(current_character))
							coolsand_UI_show_transparent_image(	tx,
																			line_height-object_height+ty,
																			(UI_image_type)EMS_ESC_display_character,
																			0	);
						else
					#endif
						{
                                       if( !(
#if(EMS_USE_ESC_COMBINATION)
                                                UI_TEST_ESC_CHARACTER(current_character)||
#endif
#if(EMS_PREVENT_CRLF_DISPLAY)
                                                UI_TEST_LF_CHARACTER(current_character)||UI_TEST_CR_CHARACTER(current_character)||
#endif
                                                UI_STRING_END_OF_STRING_CHARACTER(current_character)))
                                       {
									 
							//coolsand_UI_print_character(current_character);
						#ifdef __MMI_HINDI_ALG__
							PU8  prev_temp_ems_cursor_p;
							if(TestLastPosition(data, &previous_position))
								goto end_ems_disp;
									
									
							//Performance issues 
							//temp_ems_cursor_p = hf_get_next_cluster_p(data->textBuffer,temp_ems_cursor_p);
							prev_temp_ems_cursor_p	=	temp_ems_cursor_p;
							temp_ems_cursor_p 		= 	hf_get_next_cluster_p(temp_ems_cursor_p,temp_ems_cursor_p);
							//if(hf_get_current_cluster(data->textBuffer,temp_ems_cursor_p,disp_ems_cluster))
							if(hf_get_current_cluster(prev_temp_ems_cursor_p, temp_ems_cursor_p, disp_ems_cluster))
							{
								S32 disp_len = pfnUnicodeStrlen((PS8)disp_ems_cluster);
								MMI_ASSERT(!(disp_len > MAX_EMS_DISP_CLUSTER_LEN));
								//To avoid rule appln in showstring
								hf_disable_hindi_rules_parsing();
								hf_padding_value(0);
                         
								if(!UI_HINDI_CHARACTER_UCS2_RANGE(current_character))
								{
									if(no_show_char!=1)
									{
									   cancelWidth = 1;
 
									   if(underline_flag) UI_font->underline=0;
 
								  	   ShowString(	UI_text_x,
									  				UI_text_y,
									  				*UI_font,
									  				0,
									  				(byte*)(disp_ems_cluster),
 
									  				object_height	);
									   if(underline_flag) UI_font->underline=1;
 
								  
								 	   cancelWidth = 0;
								   }
								 }
								 else
								 {
								 	if(no_show_char!=1)
									{
									   cancelWidth = 1;
 
									   if(underline_flag) UI_font->underline=0;
 
									   ShowString(	UI_text_x,
													UI_text_y,
													*UI_font,
													0,
													(byte*)(disp_ems_cluster),
													UI_text_height	);
 
									   if(underline_flag) UI_font->underline=1;	
 
										
									   cancelWidth = 0;
								    }
								 }
	                       
								 //Open it again
								 hf_enable_hindi_rules_parsing();
								 hf_padding_def_value();
							}
							else
						#endif //__MMI_HINDI_ALG__
							{
								if(no_show_char!=1)
								{
								  gui_print_stacked_character(current_character, pre2_character, pre_character);
							   }
							}
						#if(EMS_ENABLE_SPECIAL_FONT_DISPLAY)
							if(underline_flag)						
							{			
 
							#ifdef __MMI_HINDI_ALG__
								if (UI_HINDI_CHARACTER_UCS2_RANGE(current_character))
								{
									S32 h,w;
									coolsand_UI_measure_character(current_character, &w, &h);
									UI_draw_horizontal_line(	tx,
																tx+ems_cluster_width-1,
																ty+h,
																underline_color	);								
								}
								else
							#endif //__MMI_HINDI_ALG__
 
								UI_draw_horizontal_line(	tx,
																tx+object_width-1,
																ty+line_height-1,
																underline_color	);
							}
							if(strikethrough_flag)
							{	
							#ifdef __MMI_HINDI_ALG__
								if (UI_HINDI_CHARACTER_UCS2_RANGE(current_character))
								{
 
									S32 h,w;
									coolsand_UI_measure_character(current_character, &w, &h);
 
									UI_draw_horizontal_line(	tx,
																	tx+ems_cluster_width-1,
 
																	ty+(h>>1),
 
																	strikethrough_color		);
								}
								else
							#endif //__MMI_HINDI_ALG__
								{
								
								UI_draw_horizontal_line(	tx,
																tx+object_width-1,
																ty+line_height-(object_height>>1),
																strikethrough_color		);
								}
							}
						#ifdef __MMI_HINDI_ALG__
							end_ems_disp:
							memset(disp_ems_cluster, 0, sizeof(disp_ems_cluster));
						#endif //__MMI_HINDI_ALG__
						#endif // end of #if(EMS_ENABLE_SPECIAL_FONT_DISPLAY)
						}
					#ifdef __MMI_HINDI_ALG__
						else
						{
							temp_ems_cursor_p += 2;
						}
					#endif//__MMI_HINDI_ALG__
						}
					}
					if (!(((-1)!=move_to_x)||((-1)!=move_to_y)))
					{
						if( 	(!cursor_displayed_flag)	&&
								(mapped_previous_position.OffsetToText==mapped_cursor_position.OffsetToText)	)
						{	b->cursor_ax				=	cursor_ax;
							b->cursor_ay				=	cursor_ay;
							b->cursor_line_height	=	line_height;
							b->cursor_line_position	=	line_object_count;
							b->current_line			=	n_lines-1;
							if(!disable_draw_flag && !(flags & UI_EMS_INPUT_BOX_DISABLE_CURSOR_DRAW))
							{
								if(default_direction != BIDI_L )
								{
								#ifdef __MMI_HINDI_ALG__
									if( UI_HINDI_CHARACTER_UCS2_RANGE(pre_character))
									{
 
										S32 h,w;
										coolsand_UI_measure_character(pre_character, &w, &h);
										tx	-=	ems_cluster_width;
										tx	+=	ems_cursor_width;
										gui_show_EMS_input_box_cursor(	b,
																						tx+1+ems_cluster_width,
																						ty,
																						0,
																						h,
																						1	);
									}
									else
									{
										gui_show_EMS_input_box_cursor(	b,
																						tx+1+ems_cluster_width,
																						line_height-object_height+ty,
																						0,
																						object_height,
																						1	);
									}
 
								#else
									gui_show_EMS_input_box_cursor(	b,
																						tx+object_width,
																						line_height-object_height+ty,
																						0,
																						object_height,
																						1	);
								#endif //__MMI_HINDI_ALG__
								}
								else
								{
								#ifdef __MMI_HINDI_ALG__
									if( UI_HINDI_CHARACTER_UCS2_RANGE(pre_character))
									{
 
										S32 h,w;
										coolsand_UI_measure_character(pre_character, &w, &h);
										tx	-=	ems_cluster_width;
										tx	+=	ems_cursor_width;
										gui_show_EMS_input_box_cursor(	b,
																						tx+1,
																						ty,
																						0,
																						h,
																						1	);
									}
									else
									{
										gui_show_EMS_input_box_cursor(	b,
																						tx+1,
																						line_height-object_height+ty,
																						0,
																						object_height,
																						1	);
									}
 
								#else
									gui_show_EMS_input_box_cursor( 	b,
																						tx,
																						line_height-object_height+ty,
																						0,
																						object_height,
																						1	);
								#endif //__MMI_HINDI_ALG__
								}
							}
							cursor_displayed_flag=1;
						}
					}
					else if ((((-1)!=move_to_x)||((-1)!=move_to_y)))
					{
						if (((cursor_ax+b->x+object_width) > move_to_x)&&((cursor_ay+line_height+b->y+b->display_y+b->header_height)>move_to_y)&&(!set_cursor_flag))
						{
							b->cursor_ax				=	cursor_ax;
							b->cursor_ay				=	cursor_ay;
							b->cursor_line_height	=	line_height;
							b->cursor_line_position	=	line_object_count;
							b->current_line			=	n_lines-1;
							//EMS_map_position(data,&previous_position,&(data->CurrentPosition));
							MoveCursorToPosition(data,&previous_position);
							set_cursor_flag 			= 	1;
							#if(EMS_USE_ESC_COMBINATION)
							 
							coolsand_UI_EMS_input_box_back_cursor_for_ESC_character(data,&previous_position);
							#endif
						}
							else if (((cursor_ay+line_height+b->y+b->display_y+b->header_height)>move_to_y)&&(n_lines<(b->n_lines))&&(!set_cursor_flag)&&(((line_object_count+1)>=break_line_object_count)||(UI_TEST_CR_CHARACTER(current_character))))
						{
							b->cursor_ax				=	cursor_ax;
							b->cursor_ay				=	cursor_ay;
							b->cursor_line_height	=	line_height;
							b->cursor_line_position	=	line_object_count;
							b->current_line			=	n_lines-1;
							//EMS_map_position(data,&previous_position,&(data->CurrentPosition));
							MoveCursorToPosition(data,&previous_position);
							set_cursor_flag 			= 	1;
							#if(EMS_USE_ESC_COMBINATION)
							 
							coolsand_UI_EMS_input_box_back_cursor_for_ESC_character(data,&previous_position);
							#endif
						}
					}
					 
					if(default_direction == BIDI_L)
					{
					#ifdef __MMI_HINDI_ALG__
						if(UI_HINDI_CHARACTER_UCS2_RANGE(current_character))
						{
							tx 			+=	ems_cluster_width;
							cursor_ax	+=	ems_cursor_width;
						}
						else
					#endif //__MMI_HINDI_ALG__
						{
							tx				+=	object_width;
							cursor_ax	+=	object_width;
						}
					}
				#ifdef __MMI_HINDI_ALG__
					line_object_count += saved_ems_display_skip_moves;
					hindi_cache_counter++;
				#else
					line_object_count++;
				#endif //__MMI_HINDI_ALG__
					
					if(text_format_object_count)
					{	text_format_object_count--;
						if(	(text_format_object_count==0) &&
								(current_position.OffsetToText<=data->textLength)	)
						{
						#ifdef __MMI_MESSAGES_EMS__
							coolsand_UI_EMS_input_box_set_text_format(b,&UI_EMS_normal_text_format_object);
						#else
							{	EMS_SET_FONT(&UI_EMS_inputbox_medium_font);
								EMS_SET_TEXT_COLOR(UI_EMS_inputbox_text_color);
								b->flags &= ~(	UI_EMS_INPUT_BOX_CURRENT_ALIGN_LEFT		|
													UI_EMS_INPUT_BOX_CURRENT_ALIGN_RIGHT	|
													UI_EMS_INPUT_BOX_CURRENT_ALIGN_CENTER	);
							}
						#endif
							object_height	=	coolsand_UI_get_character_height();
							if(line_object_count == 0) line_height = object_height;
							current_text_format = &UI_EMS_normal_text_format_object;
						#if(EMS_ENABLE_SPECIAL_FONT_DISPLAY)
							if(current_text_format!=NULL)
							{	if(current_text_format->data->text_format.isUnderline == KAL_TRUE)
									underline_flag=1;
								else
									underline_flag=0;
								if(current_text_format->data->text_format.isStrikethrough == KAL_TRUE) 
									strikethrough_flag=1;
								else
									strikethrough_flag=0;
							}
						#endif
						}
					}
					pre2_character = 	pre_character;
					pre_character 	= 	current_character;
					break;
				#ifdef __MMI_MESSAGES_EMS__
				case 2:	/* object */
					if (((ty+line_height)<(y1+2)) && ((ty-line_height)>y2))
					{
						break;
					}
               if(current_object->Type == EMS_TYPE_TEXT_FORMAT)
					{	coolsand_UI_EMS_input_box_set_text_format(b,current_object);
						object_height			=	coolsand_UI_get_character_height();
						current_text_format	=	current_object;
					#if(EMS_ENABLE_SPECIAL_FONT_DISPLAY)
						if(current_text_format!=NULL)
						{	if(current_text_format->data->text_format.isUnderline == KAL_TRUE) 
								underline_flag=1;
							else 
								underline_flag=0;
							if(current_text_format->data->text_format.isStrikethrough == KAL_TRUE) 
								strikethrough_flag=1;
							else 
								strikethrough_flag=0;
						}
					#endif
						text_format_object_count = ((current_object->data->text_format.textLength)>>1);
						if(line_object_count == 0)
						{	switch(current_text_format->data->text_format.Alignment)
							{
								case EMS_ALIGN_LEFT:		
									if(default_direction == BIDI_L)
							         tx	= 	x1 + 2;	
									else
										tx =  x1 + line_width + 2;											
									break;
								case EMS_ALIGN_RIGHT:		
									if(default_direction == BIDI_L)
									   tx	=	x2 - line_width - 2;
									else
										tx =  x1 + box_width;
									break;
								case EMS_ALIGN_CENTER:		
									if(default_direction == BIDI_L)
									    tx = center_x - (line_width>>1);
									else
										 tx = center_x + (line_width>>1);
									break;
								case EMS_ALIGN_LANG_DEP:
									if(default_direction == BIDI_L)
							         tx = 	x1 + 2;	
									else
										tx	=	x1 + box_width;
									break;
							}
						}
					}
					else
					{	object_width	=	object_width_cache	[line_object_count];
						object_height	=	object_height_cache	[line_object_count];
						if(first_object_flag)
						{	
							if(default_direction == BIDI_L)
							{
							   object_width 	+= 	1;
								tx					+=		1;
							}
							else
							{
							   object_width	+=		1;
								tx					-=		1;
							}
							
						}
                        else
                        {
                        	if(default_direction == BIDI_L)
							{
								tx				+=		3;
							}                          
                        }
                        
						if(default_direction == BIDI_R)
						{
      					tx 			-= 	object_width;
							cursor_ax	-=		object_width;
						}
						if (!(((-1)!=move_to_x)||((-1)!=move_to_y)))
						{
							if(	(mapped_cursor_position.Object != NULL)													&&
									(mapped_previous_position.Object != NULL)													&&
									(mapped_previous_position.OffsetToText==mapped_cursor_position.OffsetToText)	&&
									(mapped_previous_position.Object==mapped_cursor_position.Object)	)
							{	b->cursor_ax				=	cursor_ax;
								b->cursor_ay				=	cursor_ay;
								b->cursor_line_height	=	line_height;
								b->cursor_line_position	=	line_object_count;
								b->current_line			=	n_lines-1;
								if(!disable_draw_flag)
								{	coolsand_UI_EMS_input_box_display_object(	tx,
																							line_height-object_height+ty,
																							current_object,
																							1	);
									if(!(flags & UI_EMS_INPUT_BOX_DISABLE_CURSOR_DRAW))
									{
		                        if(overwrite_mode_flag 	|| 
		                        	selection_flag			&&
		                        	!(flags & UI_EMS_INPUT_BOX_STATE_MULTITAP) )
										{	s32 rx1,ry1,rx2,ry2;
											rx1	=	tx;
											ry1	=	line_height 	- 	object_height 	+	 	ty;
											rx2	=	rx1				+	object_width	-		1;
											ry2	=	ry1				+	object_height	-		1;
											coolsand_UI_cross_hatch_fill_rectangle(rx1,ry1,rx2,ry2,b->selection_color);
										}

	                           if(default_direction != BIDI_L )
 
                              {
											#ifdef __MMI_HINDI_ALG__
                                 if( UI_HINDI_CHARACTER_UCS2_RANGE(pre_character))
									      {
											   S32 h,w;
										      coolsand_UI_measure_character(pre_character, &w, &h);
                                    gui_show_EMS_input_box_cursor(	b,
																								tx+object_width,
																								ty,
																								object_width,
																								h,
																								2	);                                 }
                                 else
                              #endif
                                 {
 
                                    gui_show_EMS_input_box_cursor(	b,
																								tx+object_width,
																								line_height-object_height+ty,
																								object_width,
																								object_height,
																								2	);
 
                                 }
                              }
										else
                              {
                              #ifdef __MMI_HINDI_ALG__
                                 if( UI_HINDI_CHARACTER_UCS2_RANGE(pre_character))
									      {
											   S32 h,w;
										      coolsand_UI_measure_character(pre_character, &w, &h);
										      gui_show_EMS_input_box_cursor(	b,
																								tx,
																								ty,
																								object_width,
																								h,
																								2	);
                                 }
                                 else
                              #endif
                                 {
 
											   gui_show_EMS_input_box_cursor(	b,
																								tx,
																								line_height-object_height+ty,
																								object_width,
																								object_height,
																								2	);
 
                                 }
                              }
										
									}
									else if(EMS_view_mode) 
                           {
                              #ifdef __MMI_HINDI_ALG__
                                 if( UI_HINDI_CHARACTER_UCS2_RANGE(pre_character))
									      {
											   S32 h,w;
										      coolsand_UI_measure_character(pre_character, &w, &h);
                                    gui_show_EMS_input_box_cursor(	b,
																														tx,
																														ty,
																														object_width,
																														h,
																														3	);
                                 }
                                 else
                              #endif
                                 {
                                    gui_show_EMS_input_box_cursor(	b,
																														tx,
																														line_height-object_height+ty,
																														object_width,
																														object_height,
																														3	);
                                 }
                           }
 
								}
								cursor_displayed_flag = 1;
							}
							else if(!disable_draw_flag) 
							coolsand_UI_EMS_input_box_display_object(tx,line_height-object_height+ty,current_object,0);
						}
						else if ((((-1)!=move_to_x)||((-1)!=move_to_y)))
						{
							if (	((cursor_ax+b->x+object_width) > move_to_x)	&&
									((cursor_ay+line_height+b->y+b->display_y+b->header_height)>move_to_y)		&&
									(!set_cursor_flag)	)
							{
								b->cursor_ax				=	cursor_ax;
								b->cursor_ay				=	cursor_ay;
								b->cursor_line_height	=	line_height;
								b->cursor_line_position	=	line_object_count;
								b->current_line			=	n_lines-1;
								//EMS_map_position(data,&previous_position,&(data->CurrentPosition));
								MoveCursorToPosition(data,&previous_position);
								set_cursor_flag			= 	1;
							}
						}

						if(default_direction == BIDI_L)
						{
				        tx			+=	object_width;
				        cursor_ax	+=	object_width;
						}
						line_object_count++;
					}
					current_character=0;
					break;
				#endif
			}
			first_object_flag = 0;
		}

	#ifdef __MMI_BIDI_ALG__
		if(!(flags & UI_EMS_INPUT_BOX_DISABLE_DRAW) && arabic_char_flag)
		{
			memcpy(	data->textBuffer + temp_text_position,
						temp_text_buffer,
						visual_str_len*ENCODING_LENGTH	);
			mapped_cursor_position.OffsetToText = temp_cursor_position;
 
		#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
		#endif
		
		}
 	#endif
		
		line_start_position = current_position;
		if(	(current_character == EMS_INPUTBOX_PARAGRAPH_BREAK_CHARACTER)
	#if(EMS_SHOW_HORIZONTAL_RULE_FOR_NEWLINE)
		       || ( UI_TEST_LF_CHARACTER(current_character) 
		       && (flags & UI_EMS_INPUT_BOX_NEWLINE_DRAW) )
	#endif
		)
		{	if(!disable_draw_flag)
			{
#if (EMS_SHOW_HORIZONTAL_RULE_FOR_ALL_LINES)						
				EMS_EDITOR_DRAW_DOTTED_LINE(x1+2,x2-1,ty+line_height+2,paragraph_color);
#endif
	}
			ty						+=	4;/*	4=height of line break horizontal rule display	*/
			cursor_ay			+=	4;
			b->display_height	+=	4;
		}
 
#if(EMS_SHOW_HORIZONTAL_RULE_FOR_ALL_LINES)
		else
		{
			if(flags & UI_EMS_INPUT_BOX_ALL_LINE_DRAW)
			{
				if(!disable_draw_flag)
				{
					UI_draw_horizontal_line(x1+2,x2-1,ty+line_height+2,coolsand_UI_color( 112, 125, 240));
					last_line_y=ty+line_height+2;
					line_height+=4;
				}
				else
				{
					line_height+=4;
				}
			}
		}
#endif //EMS_SHOW_HORIZONTAL_RULE_FOR_ALL_LINES
 
		line_height			+=	1;/*	+1 added for 1 pixel gap between lines	*/
		ty						+=	line_height;
		cursor_ay			+=	line_height;
		b->display_height	+=	line_height;
	}
	b->n_lines=n_lines;
 
#if(EMS_SHOW_HORIZONTAL_RULE_FOR_ALL_LINES)
	if(flags & UI_EMS_INPUT_BOX_ALL_LINE_DRAW)
	{
		minimum_line_height = object_height;
		if(!disable_draw_flag)
		{
			while(last_line_y<=y2)
			{
				last_line_y+=minimum_line_height+5;
				UI_draw_horizontal_line(x1+2,x2-1,last_line_y,coolsand_UI_color( 112, 125, 240));
			}
			if(EMS_view_mode)
			{
			#ifdef __MMI_MAINLCD_128X128__
				if(last_line_y-y2<=minimum_line_height-12 && b->display_y != 0)
			#else
				if(last_line_y-y2<=minimum_line_height-7 && b->display_y != 0)
         #endif	
			    {
//短消息不能上翻，如果中间有数字
//				   b->display_y -=3;	// Robert Modify 060811
			    }
         }
		}
	}
#endif //EMS_SHOW_HORIZONTAL_RULE_FOR_ALL_LINES
 
	 
	if(!cursor_displayed_flag&&((move_to_x == (-1))&&(move_to_y == (-1))))
	{
		cursor_ay					-=	(line_height);
		b->cursor_ax				=	cursor_ax;
		b->cursor_ay				=	cursor_ay;
		b->cursor_line_height	=	line_height-1;
		b->cursor_line_position	=	line_object_count;
		b->current_line			=	n_lines-1;
		if(!disable_draw_flag && !(flags & UI_EMS_INPUT_BOX_DISABLE_CURSOR_DRAW))
		{
 
#if(EMS_SHOW_HORIZONTAL_RULE_FOR_ALL_LINES)
		#ifdef __MMI_HINDI_ALG__
				if(flags & UI_EMS_INPUT_BOX_ALL_LINE_DRAW)
					gui_show_EMS_input_box_cursor(b,tx+1,ty-line_height,0,line_height-5,1);
				else
					gui_show_EMS_input_box_cursor(b,tx+1,ty-line_height,0,line_height-1,1);
		#else
				if(flags & UI_EMS_INPUT_BOX_ALL_LINE_DRAW)
					gui_show_EMS_input_box_cursor(b,tx,ty-line_height,0,line_height-5,1);
				else
					gui_show_EMS_input_box_cursor(b,tx,ty-line_height,0,line_height-1,1);
		#endif //__MMI_HINDI_ALG__
#else //EMS_SHOW_HORIZONTAL_RULE_FOR_ALL_LINES
 
		#ifdef __MMI_HINDI_ALG__
			//if( UI_HINDI_CHARACTER_UCS2_RANGE(pre_character))
				gui_show_EMS_input_box_cursor(b,tx+1,ty-line_height,0,line_height-1,1);
		#else
				gui_show_EMS_input_box_cursor(b,tx,ty-line_height,0,line_height-1,1);
		#endif //__MMI_HINDI_ALG__
 
#endif //EMS_SHOW_HORIZONTAL_RULE_FOR_ALL_LINES
 
		}
		cursor_ay+=line_height;
	}
	if ((((-1)!=move_to_x)||((-1)!=move_to_y))&&(!set_cursor_flag))
	{
		cursor_ay					-=	(line_height);
		b->cursor_ax				=	cursor_ax;
		b->cursor_ay				=	cursor_ay;
		b->cursor_line_height	=	line_height-1;
		b->cursor_line_position	=	line_object_count;
		b->current_line			=	n_lines-1;
		//EMS_map_position(data,&current_position,&(data->CurrentPosition));
		MoveCursorToPosition(data,&current_position);
		 
		//coolsand_UI_EMS_input_box_next(b);
		cursor_ay					+=	line_height;
	}
#if(EMS_SHOW_HORIZONTAL_RULE_FOR_ALL_LINES)
	 
	if ((flags & UI_EMS_INPUT_BOX_ALL_LINE_DRAW) && (b->current_line == (b->n_lines - 1)))
	{
		b->cursor_line_height	=	line_height;
	}
#endif
	if(b->saved_cursor_ax==-1) b->saved_cursor_ax=b->cursor_ax;
	 
	if ((!EMS_view_mode) && (((-1) != move_to_x)||((-1) != move_to_y)))
	{
		coolsand_UI_EMS_input_box_set_text_format_cursor_position(b);
	}
	if(!disable_draw_flag && !(flags & UI_EMS_INPUT_BOX_DISABLE_SCROLLBAR))
	{	if(flags & UI_EMS_INPUT_BOX_DISPLAY_HEADER)
		{	coolsand_UI_set_vertical_scrollbar_scale(	&b->vbar,	b->height-2						);
			coolsand_UI_set_vertical_scrollbar_range(	&b->vbar,	cursor_ay+b->header_height	);
			coolsand_UI_set_vertical_scrollbar_value(	&b->vbar,	-b->display_y					);
		}
		else
		{
			coolsand_UI_set_vertical_scrollbar_scale(	&b->vbar,	b->height-2		);
			/* Use b->display_height + 2 instead of cursor_ay 
			 * 1.cursor_ay is different when the cusrosr at the last line
			 * 2.(+2)because that the box_height is b->height - 4 in coolsand_UI_EMS_input_box_locate_cursor */
			coolsand_UI_set_vertical_scrollbar_range(	&b->vbar,	b->display_height + 2);
			//coolsand_UI_set_vertical_scrollbar_range(	&b->vbar,	cursor_ay		);
			coolsand_UI_set_vertical_scrollbar_value(	&b->vbar,	-b->display_y	);
		}
		if(flags & UI_EMS_INPUT_BOX_AUTO_DISABLE_SCROLLBAR)
		{	
			if(b->vbar.scale<b->vbar.range)	
			{
				if(r2lMMIFlag)
				{ 
					vbar_x 						= b->vbar.x;
					vbar_button_x 				= b->vbar.scroll_button.x;
					b->vbar.x 					= b->vbar.x + b->vbar.width - b->width;
					b->vbar.scroll_button.x = b->vbar.scroll_button.x + b->vbar.scroll_button.width-b->width;
					coolsand_UI_show_vertical_scrollbar( &b->vbar );
				}
				else
					coolsand_UI_show_vertical_scrollbar( &b->vbar );
				if(r2lMMIFlag)
				{ 
					b->vbar.x 					= 	vbar_x;
					b->vbar.scroll_button.x = 	vbar_button_x;
				}

				
			}

 		}
 		else
		{
			if(r2lMMIFlag)
			{ 
				vbar_x 						= b->vbar.x;
				vbar_button_x 				= b->vbar.scroll_button.x;
				b->vbar.x 					= b->vbar.x + b->vbar.width - b->width;
				b->vbar.scroll_button.x = b->vbar.scroll_button.x + b->vbar.scroll_button.width-b->width;
				coolsand_UI_show_vertical_scrollbar( &b->vbar );
			}
			else
				coolsand_UI_show_vertical_scrollbar( &b->vbar );
			if(r2lMMIFlag)
			{ 
				b->vbar.x 					=	vbar_x;
				b->vbar.scroll_button.x = 	vbar_button_x;
			}
				
		}
	
	}

	/*	Handle next line, previous line navigation	*/
	if(disable_draw_flag)
	{	if(flags & UI_EMS_INPUT_BOX_PREVIOUS_LINE)
		{	if(EMS_view_mode)
			{	s32				fdl = b->first_displayed_line;
				EMSPosition		new_position;

				if(data->listHead == NULL)
				{	b->current_line = fdl;
					if(b->current_line>0)
						b->current_line--;
					else
						b->cursor_ay = 0;
					new_position = b->line_positions[b->current_line];
					MoveCursorToPosition(data,&new_position);
				}
				else
				{	if((data->CurrentPosition.OffsetToText>0)||(data->CurrentPosition.Object!=NULL))
					{	EMSPosition		new_position1;
						byte				flag;
						byte				done1 = 0;

						while(!done1)
						{	flag=0;
							if(b->current_line > 0)
							{	flag = 1;
								b->current_line--;
							}
							else
								b->cursor_ay = 0;
							
							new_position1 = b->line_positions[b->current_line];
							if(BackwardCurrentPositionObject(data,&new_position1))
							{	if(flag) b->current_line++;
							}
							else break;
							if(b->current_line>0) b->current_line--;
							else break;
							if(b->current_line<=(fdl-1)) break;
						}
					}
				}
			}
			else
			{	if(b->current_line>0)
				{	EMSPosition		new_position;
					s32				object_count	=	b->cursor_line_position;
					s32				new_position_object_count;

					b->current_line--;
					new_position					=	b->line_positions[b->current_line];
					new_position_object_count	=	b->line_object_counts[b->current_line]-1;
					if(object_count>new_position_object_count) object_count = new_position_object_count;
					MoveCursorToPosition(data,&new_position);
				#if(EMS_ENABLE_GRAPHICAL_CURSOR_MOVEMENT)
					EMS_move_graphical_cursor(	data,
														b->line_x_positions[b->current_line],
														b->saved_cursor_ax,
														new_position_object_count
														);
				#else
					ForwardCurrentPosition(data,(U16)object_count);
				#endif
				}
				else
				{	if(flags & UI_EMS_INPUT_BOX_DISABLE_TONE)
						b->flags &= ~UI_EMS_INPUT_BOX_DISABLE_TONE;
					else
						UI_editor_play_tone_cannot_navigate();
				}
			}
         if (!EMS_view_mode)
         {
				EMS_handle_CRLF_navigation(data);
         }
			//if(!TestFirstPosition(data,&data->CurrentPosition))
				coolsand_UI_EMS_input_box_set_text_format_cursor_position(b);
		}
		else if(flags & UI_EMS_INPUT_BOX_PREVIOUS_OBJECT)
		{	if(EMS_view_mode && (data->listHead!=NULL))
			{	if((data->CurrentPosition.OffsetToText>0)||(data->CurrentPosition.Object!=NULL))
				{	EMSPosition	new_position;
					new_position = b->line_positions[b->current_line];
					BackwardCurrentPositionSeekObject(data,&new_position);
				}
			}
		}
		if(flags & UI_EMS_INPUT_BOX_NEXT_LINE)
		{	if(EMS_view_mode)
			{	s32				ldl = b->last_displayed_line;
				EMSPosition		new_position;

				if(data->listHead == NULL)
				{	b->current_line = ldl;
					if(b->current_line < (b->n_lines-1)) b->current_line++;
					new_position=b->line_positions[b->current_line];
					MoveCursorToPosition(data,&new_position);
				}
				else
				{	if(!TestLastPosition(data,&data->CurrentPosition))
					{	EMSPosition	new_position2,old_position;
						byte			flag;
						byte			done2=0;

						while(!done2)
						{	flag=0;
							if(b->current_line < (b->n_lines-1))
							{	flag = 1;
								b->current_line++;
							}
							new_position2 = b->line_positions[b->current_line];
							if(ForwardCurrentPositionObject(data,&new_position2))
							{	if(flag) b->current_line--;
							}
							else break;
							flag=0;
							if(b->current_line < (b->n_lines-1))
								new_position2 =b->line_positions[b->current_line+1];
							else
							{	new_position2.OffsetToText	=	data->textLength;
								new_position2.Object			=	data->listTail;
								flag								=	1;
							}
							old_position=data->CurrentPosition;
							if(ForwardCurrentPositionObject(data,&new_position2))
							{	if(!flag) MoveCursorToPosition(data,&old_position);
							}
							else break;
							if(b->current_line<(b->n_lines-1)) b->current_line++;
							else break;
							if(b->current_line>=(ldl+1)) break;
						}
					}
				}
			}
			else
			{	if(b->current_line<(b->n_lines-1))
				{	EMSPosition		new_position;
					s32				object_count = b->cursor_line_position;
					s32				new_position_object_count;

					b->current_line++;
					new_position					=	b->line_positions[b->current_line];
					new_position_object_count	=	b->line_object_counts[b->current_line];
					if(b->current_line!=(b->n_lines-1)) new_position_object_count-=1;
					if(object_count>new_position_object_count) object_count=new_position_object_count;
					MoveCursorToPosition(data,&new_position);
				#if(EMS_ENABLE_GRAPHICAL_CURSOR_MOVEMENT)
					EMS_move_graphical_cursor(	data,
														b->line_x_positions[b->current_line],
														b->saved_cursor_ax,
														new_position_object_count	);
				#else
					ForwardCurrentPosition(data,(U16)object_count);
				#endif
				}
				else
				{	if(flags & UI_EMS_INPUT_BOX_DISABLE_TONE)
						b->flags&=~UI_EMS_INPUT_BOX_DISABLE_TONE;
					else
						UI_editor_play_tone_cannot_navigate();
				}
			}
         if (!EMS_view_mode)
         {
				EMS_handle_CRLF_navigation(data);
         }
	//		if(!TestLastPosition(data,&data->CurrentPosition))
				coolsand_UI_EMS_input_box_set_text_format_cursor_position(b);
		}
		else if(flags & UI_EMS_INPUT_BOX_NEXT_OBJECT)
		{	if(EMS_view_mode && (data->listHead!=NULL))
			{	if(!TestLastPosition(data,&data->CurrentPosition))
				{	EMSPosition		new_position;
					if(b->current_line >= (b->n_lines-1))
					{	new_position.OffsetToText	=	data->textLength;
						new_position.Object			=	data->listTail;
					}
					else new_position	=	b->line_positions[b->current_line+1];
					ForwardCurrentPositionSeekObject(data,&new_position);
				}
			}
		}
	}

#if 0
	#ifdef __MMI_BIDI_ALG__
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
 	#endif
#endif
#ifdef __MMI_BIDI_ALG__
	UI_UNUSED_PARAMETER(temp_hilight_start);
	UI_UNUSED_PARAMETER(temp_hilight_end);
	UI_UNUSED_PARAMETER(temp_text_pt);
#endif	
 
#if defined(__MMI_ARSHAPING_ENGINE__)
if((b->data->textLength/ENCODING_LENGTH)!=0)
{

   
  if(!(b->flags & UI_EMS_INPUT_BOX_VIEW_MODE))
{
    pfnUnicodeStrncpy((S8 *)b->data->textBuffer, (const S8 *)shaping_str,b->data->textLength/ENCODING_LENGTH);
}
else
{
	b->data->textBuffer = ptr_text_buffer;
}
 
}
#endif
 
}
/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_toggle_insert_mode
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_toggle_insert_mode(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
    if (b->flags & UI_EMS_INPUT_BOX_OVERWRITE_MODE)
    {
        b->flags &= ~UI_EMS_INPUT_BOX_OVERWRITE_MODE;
    }
    else
    {
        b->flags |= UI_EMS_INPUT_BOX_OVERWRITE_MODE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_backspace
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_backspace(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	#if 0
    U8 paragraph_text_align_flag = 0;
    U8 previous_paragraph_text_alignment = EMS_ALIGN_LANG_DEP;
    EMSPosition current_position;
    UI_character_type c;
    EMSObject *o;
	#endif

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
/*wuzc Del Start For GDI/GUI Portting Ver:    on 2008-4-16 17:36 */
#if 0
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
    if ((b->data != NULL) && (b->data->listHead != NULL))
    {
        current_position = b->data->CurrentPosition;
        if (gui_EMS_input_box_test_paragraph_start(b, &current_position))
        {
            gui_EMS_input_box_get_previous_object(b->data, &current_position, &c, &o);
            previous_paragraph_text_alignment = gui_EMS_input_box_get_previous_text_alignment(b, &current_position);
            paragraph_text_align_flag = 1;
        }
    }
    if (gui_EMS_cancel_current_position(b) == 0)
    {
        UI_editor_play_tone_cannot_change();
    }
    else
    {
        
        if (b->locate_cursor_flag)
        {
            gui_EMS_input_box_locate_cursor(b);
        }
        
        gui_EMS_input_box_set_text_format_cursor_position(b);
        if (paragraph_text_align_flag)
        {
            current_position = b->data->CurrentPosition;
            gui_EMS_input_box_set_paragraph_text_alignment(b, &current_position, previous_paragraph_text_alignment);
        }
    }
    b->saved_cursor_ax = -1;
    #else
	UINT16 test_length = b->data->CurrentPosition.OffsetToText, test = 0;
	if (MMI_EMS_inputbox.data->CurrentPosition.OffsetToText != 0)
	{
		if (MMI_EMS_inputbox.UCS2_count > 0)
		{
			MMI_EMS_inputbox.data->Reminder.totalRemainingOctet += 2;
			if (b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 1] != 0)
			{
				MMI_EMS_inputbox.UCS2_count--;
				if (MMI_EMS_inputbox.UCS2_count == 0)
					MMI_EMS_inputbox.data->Reminder.totalRemainingOctet += (EMS_DEFAULT_DCS_ALLOCATED_LENGTH - MMI_EMS_inputbox.data->Reminder.totalRemainingOctet)>>1;
			}
		}
		else
			MMI_EMS_inputbox.data->Reminder.totalRemainingOctet++;
	}
	
       if ((b->data->textLength != b->data->CurrentPosition.OffsetToText) && (b->data->CurrentPosition.OffsetToText != 0))          // add  by yaosq 20061220 for delete character
	{
		b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 2] = 0;
		b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 1] = 0;
		while (test_length<b->data->textLength)
		{
			b->data->textBuffer[b->data->CurrentPosition.OffsetToText - 2 + test]= b->data->textBuffer[b->data->CurrentPosition.OffsetToText+test];			
			b->data->textBuffer[b->data->CurrentPosition.OffsetToText + test - 1]=b->data->textBuffer[b->data->CurrentPosition.OffsetToText + 1 + test];
			test += 2;
			test_length += 2;
		}
	}

	if (b->data->textBuffer[b->data->textLength - 1] == 0 && b->data->textBuffer[b->data->textLength - 2] == 10)  //删除回车键时按两个提示处理
	{
		if (MMI_EMS_inputbox.UCS2_count > 0)
			MMI_EMS_inputbox.data->Reminder.totalRemainingOctet += 2;
		else
			MMI_EMS_inputbox.data->Reminder.totalRemainingOctet++;
#ifdef __MMI_GB__
		InsertEnter--;
#endif
	}
	if (b->data->CurrentPosition.OffsetToText != 0)
	{
		b->data->textBuffer[b->data->textLength-1] = 0;
		b->data->textBuffer[b->data->textLength-2] = 0;
		b->data->textLength -=2;
		b->data->CurrentPosition.OffsetToText -=2;
		b->saved_cursor_ax=-1;
	}
      #endif
/*wuzc Del End  For GDI/GUI Portting Ver:    on 2008-4-16 17:36 */

}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_previous_line
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_previous_line(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 hflag = 0;
    EMSPosition current_position = b->data->CurrentPosition;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
    b->flags |= (UI_EMS_INPUT_BOX_PREVIOUS_LINE | UI_EMS_INPUT_BOX_DISABLE_DRAW);
    gui_show_EMS_input_box(b);
    b->flags &= ~(UI_EMS_INPUT_BOX_PREVIOUS_LINE | UI_EMS_INPUT_BOX_DISABLE_DRAW);
    if (b->cursor_ay == 0)
    {
        hflag = 1;
    }
    /* if(!TestFirstPosition(b->data,&current_position)) */
    gui_EMS_input_box_locate_cursor(b);
    if ((b->flags & UI_EMS_INPUT_BOX_DISPLAY_HEADER) && hflag
        && (b->cursor_ay == 0) && (b->display_y != 0) &&
        (current_position.Object == NULL) /* && TestFirstPosition(b->data,&current_position) */ )
    {
        b->flags |= UI_EMS_INPUT_BOX_DISABLE_TONE;
        b->display_y += EMS_HEADER_SCROLL_SIZE; /* Scroll through header   */
        if (b->display_y > 0)
        {
            b->display_y = 0;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_next_line
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_next_line(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
    b->flags |= (UI_EMS_INPUT_BOX_NEXT_LINE | UI_EMS_INPUT_BOX_DISABLE_DRAW);
    TRACE_EF(g_sw_IM, "b->save_cusor = %d, b->cusro_ax = %d", b->saved_cursor_ax, b->cursor_ax);
    gui_show_EMS_input_box(b);
	 TRACE_EF(g_sw_IM, "b->save_cusor = %d, b->cusro_ax = %d", b->saved_cursor_ax, b->cursor_ax);
    b->flags &= ~(UI_EMS_INPUT_BOX_NEXT_LINE | UI_EMS_INPUT_BOX_DISABLE_DRAW);
    /* if(!TestLastPosition(b->data,&b->data->CurrentPosition)) */
    gui_EMS_input_box_locate_cursor(b);
}

#ifdef __MMI_MESSAGES_EMS__


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_view_previous_object
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_view_previous_object(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EMS_cancel_object_focus();
    b->flags |= (UI_EMS_INPUT_BOX_PREVIOUS_OBJECT | UI_EMS_INPUT_BOX_DISABLE_DRAW);
    gui_show_EMS_input_box(b);
    b->flags &= ~(UI_EMS_INPUT_BOX_PREVIOUS_OBJECT | UI_EMS_INPUT_BOX_DISABLE_DRAW);
    gui_EMS_input_box_locate_cursor(b);
    b->saved_cursor_ax = -1;
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_view_next_object
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_view_next_object(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EMS_cancel_object_focus();
    b->flags |= (UI_EMS_INPUT_BOX_NEXT_OBJECT | UI_EMS_INPUT_BOX_DISABLE_DRAW);
    gui_show_EMS_input_box(b);
    b->flags &= ~(UI_EMS_INPUT_BOX_NEXT_OBJECT | UI_EMS_INPUT_BOX_DISABLE_DRAW);
    gui_EMS_input_box_locate_cursor(b);
    b->saved_cursor_ax = -1;
}
#endif /* __MMI_MESSAGES_EMS__ */ 


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_inputbox_empty
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_inputbox_empty(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((b->data->textLength == 0) && (b->data->listHead == NULL))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_highlight_cursor_end
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  
 *****************************************************************************/
S32 gui_EMS_input_box_highlight_cursor_end(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition *current_position = &b->data->CurrentPosition;
    EMSPosition previous_position;
    EMSPosition saved_current_position = b->data->CurrentPosition;
    U8 done = 0, bflag;
    UI_character_type current_character;
    S32 count = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Move cursor backwards until a break character is encountered   */
    while (!done)
    {
        previous_position = *current_position;
        if ((current_position->Object != NULL) &&
            (current_position->Object->OffsetToText == current_position->OffsetToText))
        {
            break;
        }
        if (current_position->OffsetToText == 0)
        {
            break;
        }
        if (BackwardCurrentPosition(b->data, 1) != 1)
        {
            break;
        }
    #if(EMS_BYTE_ORDER_BIG_ENDIAN)
        current_character =
            b->data->textBuffer[current_position->OffsetToText +
                                1] | (b->data->textBuffer[current_position->OffsetToText] << 8);
    #else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
        current_character =
            b->data->textBuffer[current_position->OffsetToText] | (b->data->
                                                                   textBuffer[current_position->OffsetToText + 1] << 8);
    #endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
        count++;
        UI_EMS_INPUT_BOX_BREAK_CHARACTER(current_character, bflag);
        if (bflag)
        {
            break;
        }
    }
    b->highlight_start_position = previous_position;
    /* *current_position=saved_current_position; */
    MoveCursorToPosition(b->data, &saved_current_position);

    /* Move cursor forwards until a break character is encountered */
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
    current_character =
        b->data->textBuffer[current_position->OffsetToText +
                            1] | (b->data->textBuffer[current_position->OffsetToText] << 8);
#else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
    current_character =
        b->data->textBuffer[current_position->OffsetToText] | (b->data->
                                                               textBuffer[current_position->OffsetToText + 1] << 8);
#endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
    UI_EMS_INPUT_BOX_BREAK_CHARACTER(current_character, bflag);
    if (b->data->CurrentPosition.OffsetToText >= b->data->textLength)
    {
        bflag = 1;
    }
    if (bflag)
    {
        done = 1;
        b->flags &= ~UI_EMS_INPUT_BOX_WORD_HIGHLIGHT;
    }
    else
    {
        done = 0;
        b->flags |= UI_EMS_INPUT_BOX_WORD_HIGHLIGHT;
    }
    while (!done)
    {
        if ((current_position->Object != NULL) &&
            (current_position->Object->OffsetToText == current_position->OffsetToText))
        {
            break;
        }
        if (current_position->OffsetToText >= b->data->textLength)
        {
            break;
        }
        if (ForwardCurrentPosition(b->data, 1) != 1)
        {
            break;
        }
    #if(EMS_BYTE_ORDER_BIG_ENDIAN)
        current_character =
            b->data->textBuffer[current_position->OffsetToText +
                                1] | (b->data->textBuffer[current_position->OffsetToText] << 8);
    #else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
        current_character =
            b->data->textBuffer[current_position->OffsetToText] | (b->data->
                                                                   textBuffer[current_position->OffsetToText + 1] << 8);
    #endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
        count++;
        UI_EMS_INPUT_BOX_BREAK_CHARACTER(current_character, bflag);
        if (bflag)
        {
            break;
        }
    }
    b->highlight_end_position = *current_position;
    gui_EMS_input_box_locate_cursor(b);
    return (count);
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_highlight_cursor_number
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b               [?]         
 *  numOfChar       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
S32 gui_EMS_input_box_highlight_cursor_number(UI_EMS_input_box *b, S32 numOfChar)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition *current_position = &b->data->CurrentPosition;
    EMSPosition saved_current_position = b->data->CurrentPosition;
    U8 bflag;
    UI_character_type current_character;
    S32 count = 0, i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < numOfChar; i++)
    {
        if ((current_position->Object != NULL) &&
            (current_position->Object->OffsetToText == current_position->OffsetToText))
        {
            break;
        }
        if (current_position->OffsetToText == 0)
        {
            break;
        }
        if (BackwardCurrentPosition(b->data, 1) != 1)
        {
            break;
        }
    #if(EMS_BYTE_ORDER_BIG_ENDIAN)
        current_character =
            b->data->textBuffer[current_position->OffsetToText +
                                1] | (b->data->textBuffer[current_position->OffsetToText] << 8);
    #else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
        current_character =
            b->data->textBuffer[current_position->OffsetToText] | (b->data->
                                                                   textBuffer[current_position->OffsetToText + 1] << 8);
    #endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
        count++;
        UI_EMS_INPUT_BOX_BREAK_CHARACTER(current_character, bflag);
        if (bflag)
        {
            break;
        }
    }

    b->highlight_start_position = *current_position;
    /* *current_position=saved_current_position; */
    MoveCursorToPosition(b->data, &saved_current_position);

#if(EMS_BYTE_ORDER_BIG_ENDIAN)
    current_character =
        b->data->textBuffer[current_position->OffsetToText +
                            1] | (b->data->textBuffer[current_position->OffsetToText] << 8);
#else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
    current_character =
        b->data->textBuffer[current_position->OffsetToText] | (b->data->
                                                               textBuffer[current_position->OffsetToText + 1] << 8);
#endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
    UI_EMS_INPUT_BOX_BREAK_CHARACTER(current_character, bflag);
    if(current_character == (UI_character_type)0x20) bflag =0;

    if (bflag)
    {
        b->flags &= ~UI_EMS_INPUT_BOX_WORD_HIGHLIGHT;
    }
    else
    {
        b->flags |= UI_EMS_INPUT_BOX_WORD_HIGHLIGHT;
    }
    b->highlight_end_position = *current_position;
    
    if (b->locate_cursor_flag)
    {
        gui_EMS_input_box_locate_cursor(b);
    }
    
    return (count);
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_highlight_cursor_start
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 * RETURNS
 *  
 *****************************************************************************/
S32 gui_EMS_input_box_highlight_cursor_start(UI_EMS_input_box *b)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition *current_position = &b->data->CurrentPosition;
    EMSPosition previous_position;
    EMSPosition saved_current_position = b->data->CurrentPosition;
    U8 done = 0, bflag;
    UI_character_type current_character;
    S32 count = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (BackwardCurrentPosition(b->data, 1) != 1)
    {
        b->flags &= ~UI_EMS_INPUT_BOX_WORD_HIGHLIGHT;
        return (0);
    }
    /* Move cursor forwards until a break character is encountered */
    while (!done)
    {
        if ((current_position->Object != NULL) &&
            (current_position->Object->OffsetToText == current_position->OffsetToText))
        {
            break;
        }
        if (current_position->OffsetToText >= b->data->textLength)
        {
            break;
        }
        if (ForwardCurrentPosition(b->data, 1) != 1)
        {
            break;
        }
    #if(EMS_BYTE_ORDER_BIG_ENDIAN)
        current_character =
            b->data->textBuffer[current_position->OffsetToText +
                                1] | (b->data->textBuffer[current_position->OffsetToText] << 8);
    #else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
        current_character =
            b->data->textBuffer[current_position->OffsetToText] | (b->data->
                                                                   textBuffer[current_position->OffsetToText + 1] << 8);
    #endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
        count++;
        UI_EMS_INPUT_BOX_BREAK_CHARACTER(current_character, bflag);
        if (bflag)
        {
            break;
        }
    }
    b->highlight_end_position = *current_position;
    /* *current_position=saved_current_position; */
    MoveCursorToPosition(b->data, &saved_current_position);

    /* Move cursor backwards until a break character is encountered   */
    if (BackwardCurrentPosition(b->data, 1) != 1)
    {
        b->flags &= ~UI_EMS_INPUT_BOX_WORD_HIGHLIGHT;
        return (0);
    }
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
    current_character =
        b->data->textBuffer[current_position->OffsetToText +
                            1] | (b->data->textBuffer[current_position->OffsetToText] << 8);
#else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
    current_character =
        b->data->textBuffer[current_position->OffsetToText] | (b->data->
                                                               textBuffer[current_position->OffsetToText + 1] << 8);
#endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
    UI_EMS_INPUT_BOX_BREAK_CHARACTER(current_character, bflag);
    if (bflag)
    {
        ForwardCurrentPosition(b->data, 1);
        done = 1;
        b->flags &= ~UI_EMS_INPUT_BOX_WORD_HIGHLIGHT;
        return (0);
    }
    else
    {
        done = 0;
        b->flags |= UI_EMS_INPUT_BOX_WORD_HIGHLIGHT;
    }
    while (!done)
    {
        previous_position = *current_position;
        if ((current_position->Object != NULL) &&
            (current_position->Object->OffsetToText == current_position->OffsetToText))
        {
            break;
        }
        if (current_position->OffsetToText == 0)
        {
            break;
        }
        if (BackwardCurrentPosition(b->data, 1) != 1)
        {
            break;
        }
    #if(EMS_BYTE_ORDER_BIG_ENDIAN)
        current_character =
            b->data->textBuffer[current_position->OffsetToText +
                                1] | (b->data->textBuffer[current_position->OffsetToText] << 8);
    #else /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
        current_character =
            b->data->textBuffer[current_position->OffsetToText] | (b->data->
                                                                   textBuffer[current_position->OffsetToText + 1] << 8);
    #endif /* (EMS_BYTE_ORDER_BIG_ENDIAN) */ 
        count++;
        UI_EMS_INPUT_BOX_BREAK_CHARACTER(current_character, bflag);
        if (bflag)
        {
            break;
        }
    }
    b->highlight_start_position = previous_position;
    /* *current_position=previous_position; */
    MoveCursorToPosition(b->data, &previous_position);
    gui_EMS_input_box_locate_cursor(b);
    return (count);
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_text_format_forward_seek
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b                               [?]         
 *  t                               [?]         
 *  length                          [?]         
 *  isModifyExistedTextformat       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_text_format_forward_seek(
        UI_EMS_input_box *b,
        EMSTextFormat *t,
        S32 *length,
        BOOL isModifyExistedTextformat)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition current_position = b->data->CurrentPosition;
    UI_character_type c;
    EMSObject *o;
    U8 r;
    U8 done = 0;
    S32 text_format_object_count = 0;
    S32 text_length = 0;
    BOOL isModify = isModifyExistedTextformat;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (!done)
    {
        if (current_position.OffsetToText >= b->data->textLength)
        {
            break;
        }
        r = gui_EMS_input_box_get_next_object(b->data, &current_position, &c, &o);
        switch (r)
        {
            case 0:
                done = 1;
                break;
            case 1:
                if (c == EMS_INPUTBOX_PARAGRAPH_BREAK_CHARACTER)
                {
                    done = 1;
                }
                if (text_format_object_count == 0)
                {
                    text_length++;
                }
                break;
            case 2:
                if (o->Type == EMS_TYPE_TEXT_FORMAT)
                {
                    text_format_object_count++;
                    if (isModify)
                    {
                        o->data->text_format.Alignment = t->Alignment;
                    }
                }
                break;
        }
    }
    *length = text_length;
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_text_format_backward_seek
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b                               [?]         
 *  t                               [?]         
 *  length                          [?]         
 *  isModifyExistedTextformat       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_text_format_backward_seek(
        UI_EMS_input_box *b,
        EMSTextFormat *t,
        S32 *length,
        BOOL isModifyExistedTextformat)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 text_length = *length;
    EMSPosition current_position = b->data->CurrentPosition;
    UI_character_type c;
    EMSObject *o;
    U8 r;
    U8 done = 0;
    U8 counter_reset_flag = 0;
    S32 saved_text_length = 0;
    S32 old_OffsetToText;
    BOOL isModify = isModifyExistedTextformat;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (!done)
    {
        if (BackwardCurrentPosition(b->data, 1) != 1)
        {
            if (counter_reset_flag)
            {
                text_length = saved_text_length;
            }
            break;
        }
        gui_EMS_input_box_set_text_format_cursor_position(b);
        current_position = b->data->CurrentPosition;
        r = gui_EMS_input_box_get_next_object(b->data, &current_position, &c, &o);
        switch (r)
        {
            case 0:
                if (counter_reset_flag)
                {
                    text_length = saved_text_length;
                }
                counter_reset_flag = 0;
                done = 1;
                break;
            case 1:
                if (c == EMS_INPUTBOX_PARAGRAPH_BREAK_CHARACTER)
                {
                    if (counter_reset_flag)
                    {
                        text_length = saved_text_length;
                    }
                    ForwardCurrentPosition(b->data, 1);
                    done = 1;
                }
                else
                {
                    text_length++;
                }
                counter_reset_flag = 0;
                break;
            case 2:
                if (o->Type == EMS_TYPE_TEXT_FORMAT)
                {
                    if (text_length > 0)
                    {
                        saved_text_length = text_length + 1;
                    }
                    else
                    {
                        saved_text_length = text_length;
                    }
                    text_length = 0;
                    if (isModify)
                    {
                        o->data->text_format.Alignment = t->Alignment;
                    }
                    counter_reset_flag = 1;
                }
                break;
        }
    }
    *length = text_length;

#if(EMS_ENABLE_TESTFIX_OBJECT_ORDERING)
    old_OffsetToText = b->data->CurrentPosition.OffsetToText;
    while (b->data->CurrentPosition.OffsetToText == old_OffsetToText)
    {
        if (BackwardCurrentPosition(b->data, 1) != 1)
        {
            break;
        }
    }
    if (b->data->CurrentPosition.OffsetToText != old_OffsetToText)
    {
        ForwardCurrentPosition(b->data, 1);
    }
#endif /* (EMS_ENABLE_TESTFIX_OBJECT_ORDERING) */ 
}


/*****************************************************************************
 * FUNCTION
 *  GetTextFormatObjectAtPosition
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [?]     
 *  p           [?]     
 * RETURNS
 *  
 *****************************************************************************/
EMSObject *GetTextFormatObjectAtPosition(EMSData *data, EMSPosition *p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSObject *current = data->listHead;
    U16 OffsetToText = (U16) p->OffsetToText;
    EMSObject *return_object = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (current != NULL)
    {
        if ((current->Type == EMS_TYPE_TEXT_FORMAT) && (current->OffsetToText == OffsetToText))
        {
            return_object = current;
            break;
        }
        if (current->OffsetToText > OffsetToText)
        {
            break;
        }
        current = current->next;
    }
    return (return_object);
}

#ifdef __MMI_MESSAGES_EMS__


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_find_paragraph_range
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b                   [?]     
 *  offsetToText        [?]     
 *  textLength          [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_find_paragraph_range(UI_EMS_input_box *b, kal_uint16 *offsetToText, kal_uint16 *textLength)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint16 *offset = offsetToText;
    kal_uint16 *textLen = textLength;
    EMSPosition saved_position = b->data->CurrentPosition;
    EMSPosition current_position = b->data->CurrentPosition;
    UI_character_type c;
    EMSObject *o;
    U8 r;
    U8 done = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Move to the beginning of paragraph */
    while (!done)
    {
        /* Reach the first position */
        if (BackwardCurrentPosition(b->data, 1) != 1)
        {
            break;
        }

        current_position = b->data->CurrentPosition;
        r = gui_EMS_input_box_get_next_object(b->data, &current_position, &c, &o);
        switch (r)
        {
                /* check if reach the start of paragraph */
            case 1:
                if (c == EMS_INPUTBOX_PARAGRAPH_BREAK_CHARACTER)
                {
                    ForwardCurrentPosition(b->data, 1);
                    done = 1;
                }
                break;

                /* ignore object */
            case 2:
                break;

                /* impossible case */
            case 0:
            default:
                MMI_ASSERT(0);
                break;
        }
    }

    *offset = b->data->CurrentPosition.OffsetToText;

    done = 0;
    current_position = b->data->CurrentPosition;

    /* count to the end of the paragraph */
    while (!done)
    {

        if (current_position.OffsetToText >= b->data->textLength)
        {
            break;
        }
        r = gui_EMS_input_box_get_next_object(b->data, &current_position, &c, &o);
        switch (r)
        {
                /* count the number of characters in the paragraph */
            case 1:
                if (c == EMS_INPUTBOX_PARAGRAPH_BREAK_CHARACTER)
                {
                    done = 1;
                    (*textLen)++;
                    /* Exclude 0x0d and 0x0a */
                    /* (*textLen)--; */
                }
                else
                {
                    (*textLen)++;
                }
                break;

                /* To the end of the message */
            case 0:
                done = 1;
                break;

                /* ignore object */
            case 2:
                break;

                /* impossible case */
            default:
                MMI_ASSERT(0);
                break;
        }
    }

    MoveCursorToPosition(b->data, &saved_position);

}

/* Warning: Function requires the parameter t to be modified in some cases.
   So it is necessary to pass a copy instead of the actual data            */
/* CSD Joy added for text format insert, 0112 */


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_insert_text_alignment
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 *  t       [?]     
 * RETURNS
 *  
 *****************************************************************************/
EMSTATUS gui_EMS_input_box_insert_text_alignment(UI_EMS_input_box *b, EMSTextFormat *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

#if (0)
#if(1)
#endif
#if(0)
#endif
#if(1)
#endif
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
#else
#endif
#endif /* (0) */ 
    kal_uint16 offsetToText = 0;
    kal_uint16 textLength = 0;
    EMSTATUS emsObjectStatus = EMS_OK;
    EMSTextFormat textFormat = *t;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (b->data->textLength == 0)
    {
        return EMS_OK;
    }

    gui_EMS_input_box_find_paragraph_range(b, &offsetToText, &textLength);

    if (textLength == 0)
    {
        return EMS_OK;
    }

    textFormat.textLength = textLength * 2;

    textFormat.FontSize = EMS_FONTSIZE_NORMAL;
    textFormat.isBold = KAL_FALSE;
    textFormat.isItalic = KAL_FALSE;
    textFormat.isStrikethrough = KAL_FALSE;
    textFormat.isUnderline = KAL_FALSE;

    emsObjectStatus = AddTextAlignment(b->data, offsetToText, &textFormat);

    gui_EMS_input_box_set_text_format_cursor_position(b);
    return emsObjectStatus;
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_insert_new_paragraph
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]     
 *  t       [?]     
 * RETURNS
 *  
 *****************************************************************************/
EMSTATUS gui_EMS_input_box_insert_new_paragraph(UI_EMS_input_box *b, EMSTextFormat *t)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSTATUS emsObjectStatus;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    emsObjectStatus = gui_EMS_input_box_insert_new_line(b);

    if (emsObjectStatus == EMS_OK)
    {
        emsObjectStatus = gui_EMS_input_box_insert_text_alignment(b, t);
    }

    return emsObjectStatus;
}

/* CSD end */
#endif /* __MMI_MESSAGES_EMS__ */ 


/*****************************************************************************
 * FUNCTION
 *  EMS_move_graphical_cursor
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data            [?]         
 *  start_x         [IN]        
 *  cursor_x        [IN]        
 *  max_count       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void EMS_move_graphical_cursor(EMSData *data, S32 start_x, S32 cursor_x, S32 max_count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    EMSPosition current_position = data->CurrentPosition;
    U8 done = 0;
    S32 x = start_x;
    S32 count = 0;
    U8 r;
    EMSObject *current_object;
    UI_character_type current_character;
    S32 width = 0, height;

 
#ifdef __MMI_HINDI_ALG__
    //EMSData *data_chk_type = data;		//guoyt delete@09.11.02 for compiling warning:unused variable.
    //UI_character_type dummy_c = 0;		//guoyt delete@09.11.02 for compiling warning:unused variable.
    S32 next_ems_moves = 0;
    PU8 src_p;
    PU8 dest_p;
#endif /* __MMI_HINDI_ALG__ */ 
 
#if defined(__MMI_LANG_VIETNAMESE__)
     
    U8 viet_tone_move = 0;
#endif /* defined(__MMI_LANG_VIETNAMESE__) */ 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (!done)
    {
        if (count >= max_count)
        {
            break;
        }
        r = gui_EMS_input_box_get_next_object(data, &current_position, &current_character, &current_object);
        switch (r)
        {
            case 0:
                done = 1;
                break;
            case 1:
        #if(EMS_USE_CRLF_COMBINATION)
                if (UI_TEST_CR_CHARACTER(current_character) && (current_position.OffsetToText < data->textLength) &&
                    UI_TEST_LF_CHARACTER(data->textBuffer[current_position.OffsetToText]))
                {
                    width = 0;
                }
                else if (UI_TEST_LF_CHARACTER(current_character))
                {
                    width = 0;
                }
                else
        #endif /* (EMS_USE_CRLF_COMBINATION) */ 
            #if(EMS_USE_ESC_COMBINATION)
                if (UI_TEST_ESC_CHARACTER(current_character))
                {
                    width = 0;
                }
                else
            #endif /* (EMS_USE_ESC_COMBINATION) */ 
                {
                #if defined(__MMI_LANG_VIETNAMESE__)
                     
                    UI_character_type dummy_c = 0;
                    viet_tone_mark tone_mark = VIET_TONE_NONE;
                    viet_vowel_letter viet_vowel = VIET_VOWEL_NONE;

                    if ((current_character > 0x0040) && (current_character < 0x01B1))
                    {
                        r = gui_EMS_input_box_get_next_object(data, &current_position, &dummy_c, &current_object);      /* get next character */
                        if (1 == r)
                        {
                            tone_mark = mmi_viet_tone_mark(dummy_c);
                            if (VIET_TONE_NONE != tone_mark)
                            {
                                viet_vowel = mmi_viet_vowel_letter(current_character);
                                if (VIET_VOWEL_NONE != viet_vowel)
                                {
                                    current_character = mmi_viet_combine_vowel_tone(viet_vowel, tone_mark);
                                     
                                    viet_tone_move = 1;
                                }
                                else
                                {
                                    r = gui_EMS_input_box_get_previous_object(
                                            data,
                                            &current_position,
                                            &dummy_c,
                                            &current_object);
                                }
                            }
                            else
                            {
                                r = gui_EMS_input_box_get_previous_object(
                                        data,
                                        &current_position,
                                        &dummy_c,
                                        &current_object);
                            }
                        }
                        else if (0 != r)
                        {
                            r = gui_EMS_input_box_get_previous_object(
                                    data,
                                    &current_position,
                                    &dummy_c,
                                    &current_object);
                        }
                    }
                #endif /* defined(__MMI_LANG_VIETNAMESE__) */ 
 
                #ifdef __MMI_HINDI_ALG__
                #ifdef __MMI_HINDI_ALG__
                    src_p = (data->textBuffer + data->CurrentPosition.OffsetToText) - 2;
                #endif 
                    if (UI_HINDI_CHARACTER_UCS2_RANGE(current_character))
                    {
                        dest_p = hf_get_next_cluster_p(src_p, src_p);
                        next_ems_moves = dest_p - src_p;
                        next_ems_moves = next_ems_moves >> 1;
                        if (next_ems_moves >= 2)    /* && r==2) //&& current_object->Type==EMS_TYPE_TEXT_FORMAT) */
                        {
                            ForwardCurrentPosition(data, next_ems_moves - 1);
                            count += next_ems_moves - 1;
                            next_ems_moves = 0;
                            width = hf_size_of_current_cluster(src_p, dest_p, 1);
                        }
                        else
                        {
                            gui_measure_character(current_character, &width, &height);
                        }
                        src_p = dest_p;
                    }
                    else
                #endif /* __MMI_HINDI_ALG__ */ 
 
                        gui_measure_character(current_character, &width, &height);
                    width += 1;
                }
                x += width;
                break;
            #ifdef __MMI_MESSAGES_EMS__
            case 2:
                if (current_object->Type == EMS_TYPE_TEXT_FORMAT)
                {
                    width = 0;
                }
                else
                {
                    gui_EMS_input_box_measure_object(current_object, &width, &height);
                    width += 2;
                }
                x += width;
                break;
            #endif /* __MMI_MESSAGES_EMS__ */ 
        }
        if (x <= (cursor_x + (width >> 1)))
        {
            count++;
            if (ForwardCurrentPosition(data, 1) != 1)
            {
                break;
            }
        #if defined(__MMI_LANG_VIETNAMESE__)
             
            if (1 == viet_tone_move)
            {
                if (ForwardCurrentPosition(data, 1) != 1)
                {
                }
                viet_tone_move = 0;
            }
        #endif /* defined(__MMI_LANG_VIETNAMESE__) */ 
        }
        else
        {
            break;
        }
    }
}

#endif /* (!UI_DISABLE_EMS_INPUT_BOX) */ 

/* msz00065_20051010 peng hua add for multitap thai begin */
#ifdef __MMI_MULTITAP_THAI__


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_check_input_test_overflow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b       [?]         
 *  c       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_EMS_check_input_test_overflow(UI_EMS_input_box *b, UI_character_type c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	#if 0
    U16 ch = (U16) c;
    U8 s[2] = {0, 0};
	#endif
    S32 remaining_length;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
    if (b->UCS2_count > 0)
    {
        remaining_length = (EMS_get_remaining_length(b->data) >> 1);
    }
    else
    {
        remaining_length = EMS_get_remaining_length(b->data);

    }
    if (remaining_length <= 0)
    {
        return FALSE;
    }
    if (b->UCS2_count == 0)
    {
        if (UI_TEST_UCS2_CHARACTER(c))
        {
            if (EMSSetDCS(b->data, SMSAL_UCS2_DCS) != EMS_OK)
            {
                EMSSetDCS(b->data, SMSAL_DEFAULT_DCS);
                return FALSE;
            }
            else
            {
                if (EMS_get_remaining_length(b->data) < 2)
                {
                    EMSSetDCS(b->data, SMSAL_DEFAULT_DCS);
                    return FALSE;
                }
                else
                {
                    EMSSetDCS(b->data, SMSAL_DEFAULT_DCS);
                    return TRUE;
                }
            }
        }
        else if (UI_TEST_GSM_EXTENDED(c))
        {
            if (EMS_get_remaining_length(b->data) < 1)  /* JP : 408 info bar issue */
            {
                return FALSE;
            }
        }
    }
    /* CSD end */

    /* If in Overwrite mode, delete the current character/object and insert the new one */
    if (b->flags & UI_EMS_INPUT_BOX_OVERWRITE_MODE)
    {
        return TRUE;
    }
    return TRUE;
}
#endif /* __MMI_MULTITAP_THAI__ */ 
/* msz00065_20051010 peng hua add for multitap thai end */

#ifdef __MMI_TOUCH_SCREEN__
static UI_EMS_input_box *gui_pen_scroll_EMS_input_box = NULL;


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_scroll_by_pen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b                       [?]         
 *  offset_y                [IN]        
 *  EMS_input_box_event     [?]         
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_EMS_input_box_scroll_by_pen(
                UI_EMS_input_box *b,
                S32 offset_y,
                gui_EMS_input_box_pen_enum *EMS_input_box_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int move_gap = b->display_y + offset_y;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_MESSAGES_EMS__
    EMS_cancel_object_focus();
#endif 
    b->display_y = -offset_y;
     
    if (!(b->flags & UI_EMS_INPUT_BOX_VIEW_MODE))
    {
        if (b->cursor_ay < -(b->display_y) || b->cursor_ay + b->cursor_line_height > b->height - b->display_y)
        {
            if (move_gap > 0)
            {
                gui_show_EMS_input_box_ext(b, UI_cursor_x1, UI_cursor_y1 + b->cursor_line_height);
            }
            else
            {
                if (0 == offset_y)
                {
                    gui_show_EMS_input_box_ext(b, UI_cursor_x1, UI_cursor_y1 + move_gap);
                }
                else
                {
                    gui_show_EMS_input_box_ext(b, UI_cursor_x1, UI_cursor_y1);
                }
            }
        }
    }
    b->display_y = -offset_y;
    if (!(b->flags & UI_EMS_INPUT_BOX_VIEW_MODE) && (0 != offset_y))
    {
        gui_EMS_input_box_locate_cursor(b);
    }
    gui_show_EMS_input_box(b);
    gui_BLT_double_buffer(b->x, b->y, b->x + b->width, b->y + b->height);
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_scroll_timer_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_EMS_input_box_scroll_timer_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_EMS_input_box *b = gui_pen_scroll_EMS_input_box;
    gui_EMS_input_box_pen_enum EMS_input_box_event;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_pen_scroll_EMS_input_box = NULL;

    if (!b)
    {
        return;
    }

    gui_EMS_input_box_scroll_by_pen(b, b->pen_scroll_after_delay, &EMS_input_box_event);
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_abort_scroll_timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_EMS_input_box_abort_scroll_timer(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_pen_scroll_EMS_input_box = NULL;
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_start_scroll_timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b           [?]         
 *  value       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_EMS_input_box_start_scroll_timer(UI_EMS_input_box *b, S32 value)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* assert that no two input box at the same time */
    MMI_DBG_ASSERT(!gui_pen_scroll_EMS_input_box || gui_pen_scroll_EMS_input_box == b);

    b->pen_scroll_after_delay = value;
    gui_pen_scroll_EMS_input_box = b;

    gui_cancel_timer(gui_EMS_input_box_scroll_timer_hdlr);
    gui_start_timer(b->pen_scroll_delay_time, gui_EMS_input_box_scroll_timer_hdlr);

    gui_add_cleanup_hook(gui_EMS_input_box_abort_scroll_timer);
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_set_pen_scroll_delay
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b               [?]         
 *  delay_time      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gui_EMS_input_box_set_pen_scroll_delay(UI_EMS_input_box *b, S32 delay_time)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_DBG_ASSERT(delay_time < (S32) 0x0000FFFF);
    b->pen_scroll_delay_time = (S16) delay_time;
}


/*****************************************************************************
 * FUNCTION
 *  gui_EMS_input_box_translate_pen_event
 * DESCRIPTION
 *  
 * PARAMETERS
 *  b                       [?]         
 *  pen_event               [IN]        
 *  x                       [IN]        
 *  y                       [IN]        
 *  EMS_input_box_event     [?]         
 * RETURNS
 *  
 *****************************************************************************/
BOOL gui_EMS_input_box_translate_pen_event(
        UI_EMS_input_box *b,
        mmi_pen_event_type_enum pen_event,
        S16 x,
        S16 y,
        gui_EMS_input_box_pen_enum *EMS_input_box_event)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL ret = MMI_TRUE;
    gui_EMS_input_box_pen_state_struct *pen_state;
    gui_scrollbar_pen_enum scrollbar_event;
    gui_pen_event_param_struct scrollbar_param;
    int scroll_bar_width;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (UI_EMS_INPUT_BOX_DISABLE_SCROLLBAR & b->flags)
    {
        scroll_bar_width = 0;
    }
    else
    {
        scroll_bar_width = b->vbar.width;
    }

    pen_state = &b->pen_state;
    ret = MMI_TRUE;

    if (pen_event != MMI_PEN_EVENT_DOWN && pen_state->pen_on_scroll_bar)
    {
        gui_vertical_scrollbar_translate_pen_event(&b->vbar, pen_event, x, y, &scrollbar_event, &scrollbar_param);
        if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
        {
            if (b->pen_scroll_delay_time > 0)   /* Delay time for scrollbar scrolling */
            {
                gui_EMS_input_box_start_scroll_timer(b, scrollbar_param._u.i);
            }
            else
            {
                gui_EMS_input_box_scroll_by_pen(b, scrollbar_param._u.i, EMS_input_box_event);
            }
        }
        *EMS_input_box_event = GUI_EMS_INPUT_BOX_PEN_SCROLL_BAR;
    }
    else
    {
        switch (pen_event)
        {
            case MMI_PEN_EVENT_DOWN:
                if (PEN_CHECK_BOUND(x, y, b->x, b->y, b->width, b->height))
                {
                    pen_state->pen_on_scroll_bar = 0;
                    gui_vertical_scrollbar_set_minimum_pen_offset(&b->vbar, b->cursor_line_height);
                    if (!(b->flags & UI_EMS_INPUT_BOX_DISABLE_SCROLLBAR) &&
                        gui_vertical_scrollbar_translate_pen_event(
                            &b->vbar,
                            MMI_PEN_EVENT_DOWN,
                            x,
                            y,
                            &scrollbar_event,
                            &scrollbar_param))
                    {
                        pen_state->pen_on_scroll_bar = 1;
                        if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
                        {
                            if (b->pen_scroll_delay_time > 0)   /* Delay time for scrollbar scrolling */
                            {
                                gui_EMS_input_box_start_scroll_timer(b, scrollbar_param._u.i);
                            }
                            else
                            {
                                gui_EMS_input_box_scroll_by_pen(b, scrollbar_param._u.i, EMS_input_box_event);
                            }
                        }
                        *EMS_input_box_event = GUI_EMS_INPUT_BOX_PEN_SCROLL_BAR;
                    }
                    else
                    {
                        *EMS_input_box_event = GUI_EMS_INPUT_BOX_PEN_TEXT_DOWN;
                    }
                }
                else
                {
                    ret = MMI_FALSE;
                }
                break;
            case MMI_PEN_EVENT_UP:
                if (PEN_CHECK_BOUND(x, y, b->x, b->y, b->width - scroll_bar_width, b->height))
                {
                    *EMS_input_box_event = GUI_EMS_INPUT_BOX_PEN_TEXT_UP;
                }
                else
                {
                    ret = MMI_FALSE;
                }
                break;
            case MMI_PEN_EVENT_MOVE:
            case MMI_PEN_EVENT_LONG_TAP:
            case MMI_PEN_EVENT_REPEAT:
            case MMI_PEN_EVENT_ABORT:
                if (PEN_CHECK_BOUND(x, y, b->x, b->y, b->width - scroll_bar_width, b->height))
                {
                }
                else
                {
                    ret = MMI_FALSE;
                }
                break;
            default:
                break;
        }
    }
    return ret;
}
#endif /* __MMI_TOUCH_SCREEN__ */ 

