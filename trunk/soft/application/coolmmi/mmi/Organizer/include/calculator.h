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
 *	Calculator.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines constant, enum value, structure, and function prototypes for calculator application.
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
	Filename:		Calculator.h
	Date Created:	June-21-2003
	Contains:		Calculator
**********************************************************************************/
#ifndef _MMI_CALCULATOR_H
#define _MMI_CALCULATOR_H
#include "mmi_features.h"
#include "mmi_data_types.h"
#include "operator.h"
#include "mmi_trace.h"
#ifdef __MMI_CALCULATOR__
#include "gdi_include.h"

#if defined( __MMI_TOUCH_CALCULATOR__ )
#include "kal_non_specific_general_types.h"
#include	"touchscreengprot.h"
#endif

/* 
** Define
*/
#define SCR_ID_CALC_APP	CALCULATOR_BASE+1
#if defined( __MMI_TOUCH_CALCULATOR__ )
#if defined (__MMI_UI_STYLE_4__) 
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined (__MMI_MAINLCD_320X240__)
#define  TOUCH_CALC_LSK_X_OFFSET    (17)
#define  TOUCH_CALC_LSK_Y_OFFSET    (5)
#define  TOUCH_CALC_SCREEN_X        (0)
#define  TOUCH_CALC_SCREEN_Y        (0)
#define  TOUCH_CALC_SCREEN_WIDTH    (320)
#define  TOUCH_CALC_SCREEN_HEIGHT   (212)
#define  CALC_TOUCH_RESULT_AREA_X   (0)//(1)// (15)
#define  CALC_TOUCH_RESULT_AREA_Y   (7)
#define  CALC_TOUCH_KEYPAD_WIDTH    (320)
#define  CALC_TOUCH_KEYPAD_HEIGHT   (145)
#define  CALC_TOUCH_KEY_WIDTH    (80)
#define  CALC_TOUCH_KEY_HEIGHT      (29)
#define  CALC_TOUCH_RESULT_KEYPAD_GAP     (4)
#define  CALC_TOUCH_VERTICAL_GAP    (0)
#define  CALC_TOUCH_HORIZONTAL_GAP  (0)
#define  CALC_TOUCH_NCOLUMNS        (4)
#define  CALC_TOUCH_NROWS        (5)
#define  CALC_TOUCH_RESULT_AREA_WIDTH (300)
#define  CALC_TOUCH_RESULT_AREA_HEIGHT (54)
#define CALC_TOUCH_OPERAND1_Y    (20)//(15)
#define CALC_TOUCH_OPERAND2_Y    (35)
#define CALC_TOUCH_OPERATOR_X    (9)//(1)
#define CALC_TOUCH_OPERATOR_Y    (CALC_TOUCH_OPERAND2_Y)
#define TOUCH_CALC_MEM_Y1        (5)
#define TOUCH_CALC_MEM_X1        (CALC_TOUCH_RESULT_AREA_WIDTH )
#define  MMI_CALC_KEY_START      (0)
#define  MMI_CALC_KEY_END     (20)
#else
#define  TOUCH_CALC_LSK_X_OFFSET    (17)
#define  TOUCH_CALC_LSK_Y_OFFSET    (5)
#define  TOUCH_CALC_SCREEN_X        (0)

#define  TOUCH_CALC_SCREEN_Y        (MMI_TITLE_HEIGHT+MMI_STATUS_BAR_HEIGHT)
#define  TOUCH_CALC_SCREEN_HEIGHT   (UI_DEVICE_HEIGHT-MMI_STATUS_BAR_HEIGHT-MMI_TITLE_HEIGHT-MMI_BUTTON_BAR_HEIGHT)
#define  CALC_TOUCH_RESULT_AREA_Y   (4)
#define  CALC_TOUCH_VERTICAL_GAP    (3)
#define  CALC_TOUCH_HORIZONTAL_GAP  (5)

#define  TOUCH_CALC_SCREEN_WIDTH    (UI_DEVICE_WIDTH)
#define  CALC_TOUCH_RESULT_AREA_X   (8)
#define  CALC_TOUCH_KEYPAD_WIDTH    (223)
#define  CALC_TOUCH_KEYPAD_HEIGHT   (157)
#define  CALC_TOUCH_KEY_WIDTH    (52)
#define  CALC_TOUCH_KEY_HEIGHT      (29)
#define  CALC_TOUCH_RESULT_KEYPAD_GAP     (5)

#define  CALC_TOUCH_NCOLUMNS        (4)
#define  CALC_TOUCH_NROWS        (5)
#define  CALC_TOUCH_RESULT_AREA_WIDTH (223)
#define  CALC_TOUCH_RESULT_AREA_HEIGHT (72)
#define CALC_TOUCH_OPERAND1_Y    (22)
#define CALC_TOUCH_OPERAND2_Y    (44)
#define CALC_TOUCH_OPERATOR_X    (5)
#define CALC_TOUCH_OPERATOR_Y    (CALC_TOUCH_OPERAND2_Y)
#define TOUCH_CALC_MEM_Y1        (1)
#define TOUCH_CALC_MEM_X1        (CALC_TOUCH_RESULT_AREA_WIDTH - 2 )

#define  MMI_CALC_KEY_START      (0)
#define  MMI_CALC_KEY_END     (20)
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */
  #elif defined (__MMI_UI_STYLE_3__)
  #define	TOUCH_CALC_LSK_X_OFFSET		(17)
#define	TOUCH_CALC_LSK_Y_OFFSET		(5)
#define	TOUCH_CALC_SCREEN_X			(0)
#if defined( __MMI_MAINLCD_220X176__  )
#define	TOUCH_CALC_SCREEN_Y			(0)//songhui 20101026
#else
#define	TOUCH_CALC_SCREEN_Y			(MMI_TITLE_HEIGHT+MMI_STATUS_BAR_HEIGHT)//lin modify for cs621
#endif
#define	TOUCH_CALC_SCREEN_WIDTH		UI_DEVICE_WIDTH//lin modify for cs621
#if defined( __MMI_MAINLCD_220X176__  )

#define	TOUCH_CALC_SCREEN_HEIGHT	(UI_DEVICE_HEIGHT-MMI_BUTTON_BAR_HEIGHT)//songhui 20101026
#define	CALC_TOUCH_RESULT_AREA_X	(4)//4  //songhui 20101026

#define	CALC_TOUCH_RESULT_AREA_Y	(10)  //songhui 20101026 Æ«ÒÆÁ¿

#else
#define	TOUCH_CALC_SCREEN_HEIGHT	(UI_DEVICE_HEIGHT-MMI_STATUS_BAR_HEIGHT-MMI_TITLE_HEIGHT-MMI_BUTTON_BAR_HEIGHT)//lin modify for cs621
#define	CALC_TOUCH_RESULT_AREA_X	(9)//4 lin modify for cs621
#define	CALC_TOUCH_RESULT_AREA_Y	(2)  //lin modify for cs621
#endif
  //luanyy modify for cs621
#if defined( __MMI_MAINLCD_220X176__  )
#define	CALC_TOUCH_KEYPAD_WIDTH		(214)
#define	CALC_TOUCH_KEYPAD_HEIGHT	(94)

#define	CALC_TOUCH_KEY_WIDTH		(46)
#define	CALC_TOUCH_KEY_HEIGHT		(18)
#define	CALC_TOUCH_VERTICAL_GAP		(1)
#define	CALC_TOUCH_HORIZONTAL_GAP	(10)
#define	CALC_TOUCH_RESULT_KEYPAD_GAP		(2) 


#else
#define	CALC_TOUCH_KEYPAD_WIDTH		(155)
#define	CALC_TOUCH_KEYPAD_HEIGHT	(104)
#define	CALC_TOUCH_KEY_WIDTH		(38)
#define	CALC_TOUCH_KEY_HEIGHT		(21)
	#define	CALC_TOUCH_RESULT_KEYPAD_GAP		(2) 
#define	CALC_TOUCH_VERTICAL_GAP		(1)
#define	CALC_TOUCH_HORIZONTAL_GAP	(2)
#endif
#define	CALC_TOUCH_NCOLUMNS			(4)
#define	CALC_TOUCH_NROWS			(5)

#if defined( __MMI_MAINLCD_220X176__  )
#define	CALC_TOUCH_RESULT_AREA_WIDTH (214)//songhui 20101026
#define	CALC_TOUCH_RESULT_AREA_HEIGHT (46)//songhui 20101026

#else
#define	CALC_TOUCH_RESULT_AREA_WIDTH (158)//120 lin modify for cs621
#define	CALC_TOUCH_RESULT_AREA_HEIGHT (45)//40 lin modify for cs621
#endif
#define CALC_TOUCH_OPERAND1_Y		(14)
#define CALC_TOUCH_OPERAND2_Y		(27)
#define CALC_TOUCH_OPERATOR_X		(5)
#define CALC_TOUCH_OPERATOR_Y		(CALC_TOUCH_OPERAND2_Y)
#define TOUCH_CALC_MEM_Y1			(1)
#define TOUCH_CALC_MEM_X1			(CALC_TOUCH_RESULT_AREA_WIDTH - 2 )
  //modify hansl tbm780 2007/07/12 e
#define	MMI_CALC_KEY_START		(0)
#define	MMI_CALC_KEY_END		(20)

	#else
  //modify hansl tbm780 2007/07/12 b
#define	TOUCH_CALC_LSK_X_OFFSET		(17)
#define	TOUCH_CALC_LSK_Y_OFFSET		(5)
#define	TOUCH_CALC_SCREEN_X			(0)
#define	TOUCH_CALC_SCREEN_Y			(MMI_TITLE_HEIGHT)
#define	TOUCH_CALC_SCREEN_WIDTH		(128)
#define	TOUCH_CALC_SCREEN_HEIGHT	(124)
#define	CALC_TOUCH_RESULT_AREA_X	(4)
#define	CALC_TOUCH_RESULT_AREA_Y	(1)   
#define	CALC_TOUCH_KEYPAD_WIDTH		(120)
#define	CALC_TOUCH_KEYPAD_HEIGHT	(78)
#define	CALC_TOUCH_KEY_WIDTH		(30)
#define	CALC_TOUCH_KEY_HEIGHT		(15)
	#define	CALC_TOUCH_RESULT_KEYPAD_GAP		(1) 
#define	CALC_TOUCH_VERTICAL_GAP		(1)
#define	CALC_TOUCH_HORIZONTAL_GAP	(2)
#define	CALC_TOUCH_NCOLUMNS			(4)
#define	CALC_TOUCH_NROWS			(5)
#define	CALC_TOUCH_RESULT_AREA_WIDTH (120)
#define	CALC_TOUCH_RESULT_AREA_HEIGHT (40)
#define CALC_TOUCH_OPERAND1_Y		(14)
#define CALC_TOUCH_OPERAND2_Y		(27)
#define CALC_TOUCH_OPERATOR_X		(5)
#define CALC_TOUCH_OPERATOR_Y		(CALC_TOUCH_OPERAND2_Y)
#define TOUCH_CALC_MEM_Y1			(1)
#define TOUCH_CALC_MEM_X1			(CALC_TOUCH_RESULT_AREA_WIDTH - 2 )
  //modify hansl tbm780 2007/07/12 e
#define	MMI_CALC_KEY_START		(0)
#define	MMI_CALC_KEY_END		(20)
	#endif
#endif
/* 
** Typedef 
*/
typedef enum 
{	
	STR_ID_CALC_MENU = CALCULATOR_BASE +1
}STR_ID_CALC_ENUM;

typedef enum 
{	
	IMG_ID_CALC_PLUS = CALCULATOR_BASE +1 ,
	IMG_ID_CALC_MINUS,
	IMG_ID_CALC_MULTIPLY,
	IMG_ID_CALC_DIVIDE,
	IMG_ID_CALC_EQUAL,  // modified by Sophie 050906
	IMG_ID_CALC_MP,
	IMG_ID_CALC_MM,
	IMG_ID_CALC_MC,
	IMG_ID_CALC_MR,
	IMG_ID_CALC_MAIN,
	IMG_ID_CALC_MENU_ICON
}IMG_ID_CALC_ENUM;
#if defined( __MMI_TOUCH_CALCULATOR__ )
typedef enum 
{	
	IMG_ID_TOUCH_CALC_KEY0= IMG_ID_CALC_MENU_ICON+1 ,
	IMG_ID_TOUCH_CALC_KEY1,
	IMG_ID_TOUCH_CALC_KEY2,
	IMG_ID_TOUCH_CALC_KEY3,
	IMG_ID_TOUCH_CALC_KEY4,
	IMG_ID_TOUCH_CALC_KEY5,
	IMG_ID_TOUCH_CALC_KEY6,
	IMG_ID_TOUCH_CALC_KEY7,
	IMG_ID_TOUCH_CALC_KEY8,
	IMG_ID_TOUCH_CALC_KEY9,
	IMG_ID_TOUCH_CALC_KEY_DOT,
	IMG_ID_TOUCH_CALC_KEY_EQUAL,
	IMG_ID_TOUCH_CALC_PLUS, 
	IMG_ID_TOUCH_CALC_MINUS,
	IMG_ID_TOUCH_CALC_MULTIPLY,
	IMG_ID_TOUCH_CALC_DIVIDE,
	IMG_ID_TOUCH_CALC_MP,
	IMG_ID_TOUCH_CALC_MM,
	IMG_ID_TOUCH_CALC_MC,
	IMG_ID_TOUCH_CALC_MR,
	IMG_ID_TOUCH_CALC_EQUAL,
	IMG_ID_TOUCH_CALC_MAIN
}IMG_ID_TOUCH_CALC_ENUM;

typedef enum 
{	
	MMI_CALC_KEY_NONE,
	MMI_CALC_NUM_KEY,
	MMI_CALC_SYM_KEY
}MMI_CALC_KEY_TYPE;

typedef enum
{
	GUI_CALC_PEN_NONE,
	GUI_CALC_PEN_HIGHLIGHT_CHANGED
}mmi_gui_calc_pen_enum;

typedef struct _calculator_keypad_struct
{
	S32 result_area_x,result_area_y;
	S32 keypad_x,keypad_y;
    S32 keypad_width,keypad_height;
	S32 key_width,key_height;
	S32 horizontal_gap,vertical_gap;
	S32 selected_key_x,selected_key_y;
	S32 n_rows,n_column;
	S32 key_type;
	S32 operand1_y,operand2_y,operator_x,operator_y;
	S32 mem_x1,mem_y1;
	gdi_image_cache_bmp_struct selected_key_bitmap;
	word flag;
}calculator_keypad_struct;
#endif

/* 
** Extern Global Variable
*/

/* 
** Extern Global Function
*/
#ifdef  __MMI_TOUCH_CALCULATOR__
extern calculator_keypad_struct	g_calculator_keypad;
#endif

extern void CalcInit(void);
extern void CalcDeInit(void);

extern void EntryCalcApp(void);
extern void CalcPreEntryApp(void);
extern void HighlightCalcMenu(void);
extern void CalcHandleClearKey(void);
extern void CalcShowResultScreen(void);
extern void CalcComputeResult(void);
extern void CalcShowKeypad(void);
extern void CalcClearAll(void);
extern void CalcResetContext(void);
extern void CalcHandleRSKLongPress(void);

#endif // #ifdef __MMI_CALCULATOR__
#ifdef _MMI_OPERATOR_	//add by hansl 061108
extern void CalcDouble2String(S8 *num, S8 *result);
extern void  coolsand_UI_float_string(S8 *dob_val,S8 prec_val,S8 *out_buffer);
extern void coolsand_UI_atof(CALOPERATOR_STRUCT *result,U8 *stringCnt,UI_string_type  s);
extern S8 CalcComputePrecision(S8 *result, S16 max_digits);
extern void CalcGetPreOperand(CALOPERATOR_STRUCT *result,UI_string_type Operand);
#else
extern void CalcDouble2String(DOUBLE num, S8* result);
extern void  coolsand_UI_float_string(DOUBLE dob_val,S32 prec_val,S8* out_buffer);
extern DOUBLE coolsand_UI_atof(UI_string_type  s);
extern S32 CalcComputePrecision(DOUBLE result, S16 max_digits);
#endif

#if defined(__MMI_TOUCH_CALCULATOR__ )
void setup_calculator_keypad(calculator_keypad_struct *calculator_keypad, S16 x, S16 y);
void show_touch_calculator_screen(S32 x1, S32 y1, S32 width, S32 height);
void coolsand_UI_calculator_translate_pen_position(calculator_keypad_struct *calculator_keypad,S32 x, S32 y,S32 *item_index, S32 *key_variable);
void coolsand_UI_calculator_key_select(calculator_keypad_struct *calculator_keypad, S32 item_index);
void coolsand_UI_calculator_key_unselect(calculator_keypad_struct *calculator_keypad);
BOOL coolsand_UI_calculator_translate_pen_event( calculator_keypad_struct *calculator_keypad, S32 x, S32 y, mmi_gui_calc_pen_enum *menu_event, mmi_pen_event_type_enum  pen_event );
#endif

#endif	// #ifndef __CALCULATOR_H__

