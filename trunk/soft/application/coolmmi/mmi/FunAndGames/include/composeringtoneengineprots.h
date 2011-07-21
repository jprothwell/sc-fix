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
 *	ComposeRingToneProts.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is defines prototypes for Ring Tone Composer application.
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

/**************************************************************

	FILENAME	: ComposeRingToneEngineProts.h

  	PURPOSE		: Prototypes for the Fun and Games application

 

	AUTHOR		: Vikram

	DATE		: 

**************************************************************/

#ifndef RING_COMPOSER_PROTS_H
#define RING_COMPOSER_PROTS_H

#include "mmi_features.h"
#if defined(__MMI_RING_COMPOSER__) && defined(__MMI_IMELODY_SUPPORT__)

#include "composeringtonedefs.h"
#include "filemgr.h"
//start 
#ifdef	__MMI_TOUCH_RING_COMPOSER__
#include	"wgui.h"
#include	"gdi_datatype.h"
#include	"gdi_image_bmp.h"
#endif
//end 
#include "gui_setting.h"


#define	MAX_NOTES				150
 
#if defined( __MMI_UI_STYLE_3__)  
#define	NUM_NOTES_IN_STAVE		7			/* maximal number of notes per stave */
#elif defined(__MMI_UI_STYLE_4__)	
#define	NUM_NOTES_IN_STAVE		10			/* maximal number of notes per stave */
#else
#define	NUM_NOTES_IN_STAVE		5			/* maximal number of notes per stave */
#endif
 

#define	INSTR_PIANO				2
#define	INSTR_MUSICBOX			11
#define	INSTR_XYLOPHONE			13
#define	INSTR_HARMONICA			23
#define	INSTR_GUITAR			26
#define	INSTR_VIOLIN			45
#define	INSTR_SPHONE			66
#define	INSTR_FLUTE				74
#define	INSTR_TRUMPET			57
#define	INSTR_SDRUMS			115

#define	SLOW_SPEED				60
#define	NORMAL_SPEED			120	
#define	FAST_SPEED				180

#define MELODY_FILE_EXTENSION ".imy"

#define	TIMEOUT_COMPOSER_RINGTONE	  2000

#if defined(__MMI_FILE_MANAGER__)
#define	RNGC_FILE_LEN			FMGR_MAX_FILE_LEN*ENCODING_LENGTH
#define	RNGC_INPUT_FILE_LEN	FMGR_MAX_INPUT_FILE_LEN
#else
#define	RNGC_FILE_LEN		MAX_AUDIO_NAME_WIDTH
#define	RNGC_INPUT_FILE_LEN	((RNGC_FILE_LEN)/2)-6
#endif
//start 
#ifdef __MMI_TOUCH_RING_COMPOSER__

//#define TOTAL_INSTR					(10) 
#define RNGC_MAX_ROW				(2)
#define	RNGC_MAX_COLUMN				(7)
#define RNGC_MAX_CUSTOM_KEYS		(3)

#if defined(__MMI_UI_STYLE_3__)		 
#define NOTE_SELECTION_IMAGE_X		(0)
#define NOTE_SELECTION_IMAGE_Y		(150)
#define NOTE_SELECTION_MATRIX_WIDTH	 (132)
#define NOTE_SELECTION_MATRIX_HEIGHT (47)
#define NOTE_SELECTION_WIDTH		(176)
#define NOTE_SELECTION_HEIGHT		(49)
#define RNGC_MATRIX_KEY_WIDTH		(18)
#define RNGC_MATRIX_KEY_HEIGHT		(23)

#define NORMAL_TOOLBAR_IMAGE_X		(0)
#define NORMAL_TOOLBAR_IMAGE_Y		(184)
#define NORMAL_TOOLBAR_HEIGHT		(16)
#define NORMAL_TOOLBAR_WIDTH		(176)

//#define SYMBOL_SELECTION_IMAGE_X	(1)
//#define SYMBOL_SELECTION_IMAGE_Y	(151)
//#define SYMBOL_SELECTION_HEIGHT		(49)
#define	RNGC_NOTE_SELECT_MATRIX_WIDTH		(132)/*(89)*/
#define RNGC_NOTE_SELECT_MATRIX_HEIGHT		(47)
//#define	RNGC_SYMBOL_SELECT_MATRIX_WIDTH		(89)
//#define RNGC_SYMBOL_SELECT_MATRIX_HEIGHT	(47)

//#define RNGC_SYMBOL_MATRIX_KEY_WIDTH	(29)
//#define RNGC_SYMBOL_MATRIX_KEY_HEIGHT	(23)
 
#elif defined(__MMI_UI_STYLE_4__)

#define NOTE_SELECTION_WIDTH		(UI_device_width)
#define NOTE_SELECTION_HEIGHT		(66)
#define NOTE_SELECTION_IMAGE_X		(0)
#define NOTE_SELECTION_IMAGE_Y		(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-NOTE_SELECTION_HEIGHT) 
#define NOTE_SELECTION_MATRIX_WIDTH	 (175)
#define NOTE_SELECTION_MATRIX_HEIGHT (65)
#define RNGC_MATRIX_KEY_WIDTH		(24)
#define RNGC_MATRIX_KEY_HEIGHT		(31)

#define NORMAL_TOOLBAR_HEIGHT		(23)
#define NORMAL_TOOLBAR_WIDTH		(UI_device_width)
#define NORMAL_TOOLBAR_IMAGE_X		(0)
#define NORMAL_TOOLBAR_IMAGE_Y		(MAIN_LCD_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT-NORMAL_TOOLBAR_HEIGHT)//(275) 

#define	RNGC_NOTE_SELECT_MATRIX_WIDTH		(175)
#define RNGC_NOTE_SELECT_MATRIX_HEIGHT		(65)
#define RNGC_INSTR_SELECT_MARGIN			(2)
#define RNGC_INSTR_SELECT_X_OFFSET		(6)
#else

#define NOTE_SELECTION_IMAGE_X		(0)
#define NOTE_SELECTION_IMAGE_Y		(150)
#define NOTE_SELECTION_MATRIX_WIDTH	 (132)
#define NOTE_SELECTION_MATRIX_HEIGHT (47)
#define NOTE_SELECTION_WIDTH		(176)
#define NOTE_SELECTION_HEIGHT		(49)
#define RNGC_MATRIX_KEY_WIDTH		(18)
#define RNGC_MATRIX_KEY_HEIGHT		(23)

#define NORMAL_TOOLBAR_IMAGE_X		(0)
#define NORMAL_TOOLBAR_IMAGE_Y		(184)
#define NORMAL_TOOLBAR_HEIGHT		(16)
#define NORMAL_TOOLBAR_WIDTH		(176)

//#define SYMBOL_SELECTION_IMAGE_X	(1)
//#define SYMBOL_SELECTION_IMAGE_Y	(151)
//#define SYMBOL_SELECTION_HEIGHT		(49)
#define	RNGC_NOTE_SELECT_MATRIX_WIDTH		(132)/*(89)*/
#define RNGC_NOTE_SELECT_MATRIX_HEIGHT		(47)
//#define	RNGC_SYMBOL_SELECT_MATRIX_WIDTH		(89)
//#define RNGC_SYMBOL_SELECT_MATRIX_HEIGHT	(47)

//#define RNGC_SYMBOL_MATRIX_KEY_WIDTH	(29)
//#define RNGC_SYMBOL_MATRIX_KEY_HEIGHT	(23)

#endif	/* __MMI_UI_STYLE_3__ */
/*~]*/

#define RNGC_INSTRUMENT_WIDTH			(24)
#define RNGC_INSTRUMENT_HEIGHT			(24)
#define RNGC_INSTRUMENT_X				(UI_device_width-RNGC_INSTRUMENT_WIDTH-2)
#define RNGC_INSTRUMENT_Y				(MMI_title_y)
#endif
//end 
/* 
** Typedef 
*/


typedef struct 
{
#if !defined(__MMI_FILE_MANAGER__)	
	DYNAUDIOINFO RingToneInfo[MAXIMUM_COMPOSED_RINGTONE];
#endif
	S8 FileName[RNGC_FILE_LEN];			/* string of filename */
	S8 PrevFileName[RNGC_FILE_LEN];		/* string of filename */
#if !defined(__MMI_FILE_MANAGER__)	
	U8 TotalTone;								/* total number of composed ring tone */
#endif
	U8 CurrTone;								/* index of current composed tone */
	U8 CurrInstr;								/* current instrument */
	U8 CurrSpeed;								/* current play speed */
	U8 IsTonePlaying;							/* if tone is playing */
	U8 CurrHiliteItem;							/* index of current selected option */
	U8 CurrEditState;								/* eidt state 0 == Add, 1 == Edit, 2 == Rename*/
	U8 IsSelfStop;
}rngc_context_struct;

typedef struct
{
	UI_image_type ImageList[NUM_NOTES_IN_STAVE];
	S16	TotalNotes;					/* total number of notes*/
	U16	CurrNoteIndex;				/* current position of cursor */
	U16	CurrUIPos;					/* current UI position */
	U16	NotesList[MAX_NOTES+1];	
	U8 YPos[NUM_NOTES_IN_STAVE];
	U8 IsRskLP;						/* if RSK is long-pressed */
	U8 CursorWidth;
}rngc_engine_context_struct;

typedef struct 
{
	U16	nNote;
	U8 nImageId;
}NoteInfo;

typedef struct 
{
	U16	nImageId;
	U8	nXPos;
	U8	nYPos;
}DisplayInfo;

 
#if defined(__MMI_TOUCH_RING_COMPOSER__) || defined(__MMI_UI_STYLE_4__)
typedef enum
{
	RNGC_PREVIOUS_INSTR=0,
	RNGC_NEXT_INSTR,
	RNGC_INSRT_NONE
}RNGC_SELECTED_INSTR;
#endif	/* __MMI_TOUCH_RING_COMPOSER__ || __MMI_UI_STYLE_4__ */
 

//start 
#ifdef __MMI_TOUCH_RING_COMPOSER__

typedef enum 
{
	GUI_RNGC_NORMAL_TOOLBAR=1,
	GUI_RNGC_NOTE_SELECTION
}touch_rngc_selection_area;

typedef enum 
{
	RNGC_PEN_NONE=1,
	RNGC_PEN_INSTRUMENT,
	RNGC_PEN_STAVE,
	RNGC_PEN_NORMAL_TOOLBAR,
	RNGC_PEN_NOTE_SELECTION,
	TOUCH_RNGC_PEN_LSK,
	TOUCH_RNGC_PEN_RSK
}touch_rngc_pen_state;
typedef enum
{
	GUI_RNGC_VIEW=1,
	GUI_RNGC_EDIT,
	GUI_RNGC_INSERT,
	GUI_RNGC_INSERT_EDIT,
	RNGC_PLAY,
	RNGC_PAUSE
}touch_rngc_mode;

typedef enum
{
	GUI_RNGC_KEY_SHARP =SHARP,
	GUI_RNGC_KEY_FLAT=FLAT,
	GUI_RNGC_KEY_DOT,
	GUI_RNGC_KEY_OK,
	GUI_RNGC_KEY_EDIT,
	GUI_RNGC_KEY_INSERT,
	GUI_RNGC_KEY_PLAY,
    GUI_RNGC_KEY_NEXT,
	GUI_RNGC_KEY_PREV,
	GUI_RNGC_OCTAVE_UP,
	GUI_RNGC_OCTAVE_DOWN,
	GUI_RNGC_KEY_NOTE1=(NOTE_C|OCTAVE_4|DURATION_1|NORMAL),
	GUI_RNGC_KEY_NOTE2=(NOTE_C|OCTAVE_4|DURATION_1BY2|NORMAL),
	GUI_RNGC_KEY_NOTE3=(NOTE_C|OCTAVE_4|DURATION_1BY4|NORMAL),
	GUI_RNGC_KEY_NOTE4=(NOTE_C|OCTAVE_4|DURATION_1BY8|NORMAL),
	GUI_RNGC_KEY_NOTE5=(NOTE_C|OCTAVE_4|DURATION_1BY16|NORMAL),
	GUI_RNGC_KEY_NOTE6=(NOTE_REST|OCTAVE_4|DURATION_1BY4),
	GUI_RNGC_KEY_NOTE7=(NOTE_REST|OCTAVE_4|DURATION_1BY8),
	GUI_RNGC_KEY_NOTE8=(NOTE_REST|OCTAVE_4|DURATION_1BY16)
}touch_rngc_key_note;


typedef struct
{
	S16 matrix_x,matrix_y;
	S16 n_matrix_rows,n_matrix_columns;
	S16 matrix_width,matrix_height;
	S16	key_width, key_height;
	UI_image_ID_type key_image[RNGC_MAX_ROW][RNGC_MAX_COLUMN];
	U16	key_note[RNGC_MAX_ROW][RNGC_MAX_COLUMN];
}rngc_matrix_key_layout_struct;

typedef struct
{
	S16 x,y;
	S16 key_width,key_height;
	UI_image_ID_type key_image;
	U16	key_note;
}rngc_custom_key_layout_struct;

typedef struct 
{
	S16	n_matrix_keys,n_custom_keys;
	S16 image_x,image_y;
	rngc_matrix_key_layout_struct	*matrix_layout;
	rngc_custom_key_layout_struct	*custom_keys;
	S16 horizontal_gap,vertical_gap;
	UI_image_ID_type imageid;
} rngc_selection_area_layout_struct;

typedef struct
{
	U16 original_key_note;
	touch_rngc_mode mode;
	S32  play_mode;
	S32 selected_key_x,selected_key_y;
	S32 key_width,key_height;
	S32 selected_key_image;
	gdi_image_cache_bmp_struct selected_key_bitmap;
	touch_rngc_selection_area area_type;
	U16	key_note_type;
	dword	flag;
}touch_rngc_struct;
#endif
//end 


//void PopulateRingComposer(void);

extern rngc_context_struct g_rngc_cntx;
extern rngc_engine_context_struct *g_rngc_eng_cntx;

void ExitRngcComposerEngine(void);

void HandleKey_0(void);
void HandleKey_1(void);
void ShowNote(U16 nNote);

void HandleKey_2(void);
void HandleKey_3(void);
void HandleKey_4(void);
void HandleKey_5(void);
void HandleKey_6(void);

void HandleKey_7(void);
void HandleKey_8(void);
void HandleKey_9(void);
void HandleKey_STAR(void);
void HandleKey_POUND(void);
void HandleKey_UP(void);
void HandleKey_DOWN(void);
void HandleKey_LEFT(void);
void HandleKey_RIGHT(void);
//void DisplayNote(U16 image_id,U8 x_pos,U8 y_pos1,S32 value);

U16 GetMelodyFromStruct(U8* pList, U16 nListSize);
U8* GetStringForNode(U8* pStr, U32 nNote);

void GetIndexInList(U16 nNote,U16* pImageId, U8* pYVal);
//void SetFocusOnNote(U16 nPos);
//void SetInstrument(U8 Instrument);

// UI Functions
void redraw_ringcomposer(void);
void SetRingComposer(U16 title_id,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon, U8 instru_type,S32 value,U8* history_buffer);
void RefreshNotesList(U16	nTotalNotes,DisplayInfo*	psDisplayInfo );
U16 GetInstrumentImage( U8 instru_type);
S32 GenerateMelodyFile(PS8 pFileName);
void RedrawNotes(void);
void InsertNote(U16	nNote);
void FreeMemory(void);
void DeleteCurNote(void);
void EditMelody(U8*	pFileName);

void RngcComposerRSKUpHandler(void);
void RngcComposerRSKDownHandler(void);


U8	GetComposerStructFromMelody(U8*	pMelody);
U8 CompareMelody( U8** pptr, void *ps);
U8 Tolower( U8 ch );
S8 StrnCmp( S8 *s1, S8 *s2, int maxlen);
void TrimStr(U8** pptr);

void ClearToEnd( U8 **pptr );
U8 GetInstrument(U8** pptr);
U16 GetBeat(U8** pptr);
void SkipSpace( U8 **pptr );
S32 GetInteger(U8 **pptr );
U8 SetNote(U8** pptr);
U8 SetOctaveFromStream(U8** pptr);
U8 SetLED(U8** pptr);
U8 SetVibration(U8** pptr);
U8 SetBackLight(U8** pptr);
U8 GetMelody(U8** pptr);
void HandleKey_END(void);
void InitializeComposer(void);
void HandleKey_CLEARLONG(void);
void HandleKey_CLEARLONG(void);
void PlayCurrentNode(void);
void GenerateCompleteComposeFileName(PS8	pFileName, PS8	pCompletePath);
S32 RngcCreateDir(void);
 
#if defined(__MMI_TOUCH_RING_COMPOSER__) || defined(__MMI_UI_STYLE_4__)
void mmi_rngc_change_instrument(S32 selected_instrument);
#endif	/* __MMI_TOUCH_RING_COMPOSER__ || __MMI_UI_STYLE_4__ */

#if defined(__MMI_UI_STYLE_4__)
void initialize_rngc_instrument_select_menu_items(s32 x,s32 y,s32 width,s32 height,U8** list_of_items,s32 *highlighted_item,void (*f)(s32 item_index));
void rngc_select_previous_instrument(void);
void rngc_select_next_instrument(void);
void register_rngc_instrument_select_keys(void);
#endif	/* __MMI_UI_STYLE_4__ */
/*~]*/
//start 
#ifdef	__MMI_TOUCH_RING_COMPOSER__
void mmi_rngc_note_selection(touch_rngc_struct *touch_rngc_context);
void show_touch_rngc_selection_area(void);
MMI_BOOL mmi_rngc_pen_down_selection_area(touch_rngc_struct *touch_rngc_context,mmi_pen_point_struct pos );
MMI_BOOL mmi_rngc_pen_down_on_stave( mmi_pen_point_struct pos,S32 *item_index );
MMI_BOOL mmi_rngc_pen_down_instrument(mmi_pen_point_struct pos);
void mmi_rngc_set_note_type(S32 note_type_to_set);
void mmi_rngc_set_note_beat(void);
void mmi_rngc_change_stave_notes(touch_rngc_struct *touch_rngc_context,S32 item_index);
void mmi_rngc_normal_toolbar(touch_rngc_struct *touch_rngc_context);
void mmi_rngc_note_selection(touch_rngc_struct *touch_rngc_context);
void mmi_rngc_pen_down_hdlr(mmi_pen_point_struct pos);
void mmi_rngc_pen_up_hdlr(mmi_pen_point_struct pos);
void mmi_rngc_pen_move_hdlr(mmi_pen_point_struct pos);
void mmi_rngc_pen_repeat_hdlr(mmi_pen_point_struct pos);
#endif
//end 
#endif //#if defined(__MMI_RING_COMPOSER__)
#endif


