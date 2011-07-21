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
 * wgui_ems_categories.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for UI wrapper routines for EMS and category screens.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 
/**********************************************************************************
	Filename:		wgui_ems_categories.h
	Date Created:	March-22-2003
	Contains:		UI wrapper routines for EMS and category screens
**********************************************************************************/

#ifndef __EMSCATEGORYSCREENS_H__
#define __EMSCATEGORYSCREENS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "gui_config.h"
#include "mmi_data_types.h"

#if(!UI_DISABLE_EMS_CATEGORY_SCREENS)

typedef enum
{	
	EMS_BI_ANIMATION_0_ID=DEMO_APP_BASE+450,
	EMS_BI_ANIMATION_1_ID,
	EMS_BI_ANIMATION_2_ID,
	EMS_BI_ANIMATION_3_ID,
	EMS_BI_ANIMATION_4_ID,
	EMS_BI_ANIMATION_5_ID,
	EMS_BI_ANIMATION_6_ID,
	EMS_BI_ANIMATION_7_ID,
	EMS_BI_ANIMATION_8_ID,
	EMS_BI_ANIMATION_9_ID,
	EMS_BI_ANIMATION_10_ID,
	EMS_BI_ANIMATION_11_ID,
	EMS_BI_ANIMATION_12_ID,
	EMS_BI_ANIMATION_13_ID,
	EMS_BI_ANIMATION_14_ID,

	EMS_EDITOR_SOUND_OBJECT_ID,
	EMS_EDITOR_MELODY_OBJECT_ID,

	EMS_CAN_INSERT_OBJECT_ID,
	EMS_CANNOT_INSERT_OBJECT_ID

} EMS_BI_IMAGEID;

#include "gui_ems.h"
#include "wgui_categories.h"

/*	EMS Application menus					*/
void GotoEMSTextFormattingMenu(void);
void EntryEMSMainMenu(void);
void ExitEMSMainMenu(void);
void EntryEMSObjectsMenu(void);
void ExitEMSObjectsMenu(void);
void EntryEMSTextFormattingMenu(void);
void ExitEMSTextFormattingMenu(void);
void EntryEMSTextSizeSelectionMenu(void);
void ExitEMSTextSizeSelectionMenu(void);
void EntryEMSTextStyleSelectionMenu(void);
void ExitEMSTextStyleSelectionMenu(void);
void EntryEMSTextAlignmentSelectionMenu(void);
void ExitEMSTextAlignmentSelectionMenu(void);
void EntryEMSNewParagraphAlignmentMenu(void);
void ExitEMSNewParagraphAlignmentMenu(void);

/*	External functions from wgui_categories	*/
byte* dummy_get_history(byte *buffer);
S32 dummy_get_history_size(void);
void show_input_mode(void);
void register_hide_multitap(void (*f)(void));
void category5_hide_multitap(void);
void change_input_mode(void);
extern void (*inputbox_change_mode)(void);
void change_multiline_inputbox_input_mode(void);
extern U8 MMI_current_input_mode;

 
void wgui_EMS_setup_input_information(S32 x,S32 y,S32 width,S32 height);
void move_EMS_inputbox(S32 x, S32 y);
void resize_EMS_inputbox(S32 width, S32 height);
void wgui_EMS_redraw_input_information_bar(void);
void show_EMS_inputbox(void);
void get_EMS_inputbox_category_history(word history_ID,byte *history_buffer,s16 input_type);
byte set_EMS_inputbox_category_history(word history_ID,byte *history_buffer,s16 *input_type);
void wgui_set_EMS_inputbox_RSK(void);
 

#define EMS_PREDEFINED_PICTURE			1
#define EMS_USERDEFINED_PICTURE			2
#define EMS_PREDEFINED_ANIMATION		3
#define EMS_USERDEFINED_ANIMATION		4
#define EMS_PREDEFINED_SOUND			5
#define EMS_PREDEFINED_MELODY			6
#define EMS_USERDEFINED_MELODY			7
#ifndef CATEGORY144_FMGR_PREVIEW
#define CATEGORY144_FMGR_PREVIEW		0x1000
#endif
#ifdef __MMI_MESSAGES_EMS__
EMSTATUS EMS_load_object(dword type,U8* data,S32 index,EMSObjData *object);
#endif
EMSTATUS EMS_inputbox_insert_predefined_picture(S32 index);
EMSTATUS EMS_inputbox_insert_userdefined_picture(U8* data);
EMSTATUS EMS_inputbox_insert_predefined_animation(S32 index);
EMSTATUS EMS_inputbox_insert_userdefined_animation(U8* data);
EMSTATUS EMS_inputbox_insert_predefined_sound(S32 index);
EMSTATUS EMS_inputbox_insert_predefined_melody(S32 index);
EMSTATUS EMS_inputbox_insert_userdefined_melody(U8* data);

void EMS_load_predefined_picture(S32 index,U8 *data);
S32 EMS_get_predefined_picture_size(S32 index);
S32 EMS_get_predefined_picture_data_size(U8* data);
S32 EMS_get_userdefined_picture_size(U8* data);
S32 EMS_get_predefined_animation_size(S32 index);
S32 EMS_get_userdefined_animation_size(U8* data);
S32 EMS_get_predefined_sound_size(S32 index);
void EMS_load_predefined_melody(S32 index,U8 *data);
S32 EMS_get_predefined_melody_size(S32 index);
S32 EMS_get_predefined_melody_data_size(U8 *data);
S32 EMS_get_userdefined_melody_size(U8* data);

S32 EMS_check_userdefined_picture_size(U8* data,U8* allowed);
S32 EMS_check_predefined_picture_data_size(U8* data,U8* allowed);
S32 EMS_check_userdefined_animation_size(U8* data,U8* allowed);
S32 EMS_check_predefined_melody_data_size(U8 *data,U8* allowed);
S32 EMS_check_userdefined_melody_size(U8* data,U8* allowed);

void EMS_userdefined_melody_attach_header(U8 *src,U8 *dst,S32 *size);
void EMS_userdefined_melody_detach_header(U8 *src,U8 *dst,S32 *size);
void EMS_predefined_melody_attach_header(U8 *src,U8 *dst,S32 *size);
void EMS_predefined_melody_detach_header(U8 *src,U8 *dst,S32 *size);

void EMS_inputbox_set_text_size(EMSFONTSIZE s);
EMSFONTSIZE EMS_inputbox_get_text_size(void);
void EMS_inputbox_set_text_style(U8 isBold,U8 isItalic,U8 isUnderline,U8 isStrikethrough);
void EMS_inputbox_get_text_style(U8 *isBold,U8 *isItalic,U8 *isUnderline,U8 *isStrikethrough);
EMSTATUS EMS_inputbox_set_text_alignment(EMSALIGNMENT a);
EMSALIGNMENT EMS_inputbox_get_text_alignment(void);
EMSTATUS EMS_inputbox_create_new_paragraph(EMSALIGNMENT a);
extern void redraw_EMS_inputbox(void);
void RedrawCategory28Screen(void);
void ShowCategory28Screen(U16 title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,EMSData* data,U8* history_buffer);
void ExitCategory28Screen(void);
S32 GetCategory28HistorySize(void);
U8* GetCategory28History(U8* history_buffer);
void CloseCategory28Screen(void);
void SetCategory28RightSoftkeyFunction(void (*f)(void),MMI_key_event_type k);
byte AppendCategory28String(s16 input_type,EMSData *data,U8* s,U8* history_buffer);
byte AppendEMSString(s16 input_type,EMSData *data,U8* s,U8 dcs,U8* history_buffer);

void RedrawCategory29Screen(void);
void ShowCategory29Screen(U16 title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,EMSData* data,U8* history_buffer);
void ExitCategory29Screen(void);
S32 GetCategory29HistorySize(void);
U8* GetCategory29History(U8* history_buffer);
void CloseCategory29Screen(void);

void RedrawCategory39Screen(void);
void ShowCategory39Screen(U16 title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,EMSData* data,U8* header,U8* history_buffer);
void ExitCategory39Screen(void);
S32 GetCategory39HistorySize(void);
U8* GetCategory39History(U8* history_buffer);
void CloseCategory39Screen(void);

void RedrawCategory143Screen(void);
void ShowCategory143Screen(U16 title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,EMSObject *object_list,S32 *n_items,S32 *index,U8* history_buffer);
void ExitCategory143Screen(void);
S32 GetCategory143HistorySize(void);
U8* GetCategory143History(U8* history_buffer);
S32 GetCategory143DataSize(void);
U8*	GetCategory143Data(U8* data);
byte GetCategory143DataType(void);
void GetEMSObjectPDUSize(U8* data,S32 *size);
S32 GetEMSPredefinedObjectPDUSize(U32 object_type);
void InsertEMSObjectUIHeader(U32 object_type,S32 object_size,U8 *source,U8 *destination);
void RemoveEMSObjectUIHeader(U32 object_type,U8 *source,U8 *destination);
void CreateCategory143ObjectList(EMSData *data,EMSObject **object_list);
void FreeCategory143ObjectList(EMSObject **object_list);

#define CATEGORY144_DISABLE_PREVIEW	0x0100

void RedrawCategory144Screen(void);
void ShowCategory144Screen(U8* title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,U32 object_type,S32 n_items,U8** object_titles,U16 *predefined_objects,MMI_BOOL is_short_name,U8** userdefined_objects,S32 *index,U8* history_buffer);
void ExitCategory144Screen(void);
S32 GetCategory144HistorySize(void);
U8* GetCategory144History(U8* history_buffer);
void RegisterCategory144ObjectLoader(U8* (*f)(S32 index));

void EMSObjectStopPlayBack(void);
EMSTATUS EMSObjectPlayback(U32 object_type,U8* data,S32 index);

/*	Call before EMSPack	*/
void EMS_ConvertAsciiEncodingToGSM7BitDefault(EMSData *data);
void EMS_ConvertAsciiEncodingToGSM7BitDefaultWithExtended(EMSData *data);

/*	Call after EMSUnpack	*/
void EMS_ConvertGSM7BitDefaultEncodingToAscii(EMSData *data);
void EMS_ConvertGSM7BitDefaultEncodingToAsciiWithExtended(EMSData *data);

MMI_BOOL is_EMS_inputbox_in_view_mode(void);
#define CAT145_BLACK_TITLE	0x00000000
#define CAT145_BLUE_TITLE	0x00000001
#define CAT145_RED_TITLE	0x00000002
#define CAT145_GREEN_TITLE	0x00000003

void RedrawCategory145Screen(void);
void ShowCategory145Screen(U8* title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,U16 message_icon,U8 *message_title,U8 *message,U32 flags,U8 *history_buffer);
void ExitCategory145Screen(void);
S32 GetCategory145HistorySize(void);
U8* GetCategory145History(U8* history_buffer);
typedef struct {
   const U8 *data;
   kal_uint32  len;
}ems_resource_struct;

#ifdef __MMI_DRAW_MANAGER__
void move_EMS_inputbox(S32 x, S32 y);
void resize_EMS_inputbox(S32 width, S32 height);
#endif
S32 get_max_editor_height(S32 editor_top);
#endif	// (!UI_DISABLE_EMS_CATEGORY_SCREENS)

#ifdef __cplusplus
}
#endif

#endif


