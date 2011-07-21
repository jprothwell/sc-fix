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
 *	Calculator.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements Calculator application.
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
 *
 *
 *
 *
 *
 *
 *
 *
  * Nov 13 2005 CSD01215
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**********************************************************************************
	Filename:		Calculator.c
	Date Created:	June-21-2003
	Contains:		Calculator
**********************************************************************************/

#ifndef _MMI_CALCULATOR_C
#define _MMI_CALCULATOR_C

#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "cswtype.h"
#include "unicodexdcl.h"
#ifdef __MMI_TOUCH_CALCULATOR__
//start 
#include "gdi_include.h"
#include "wgui.h"
//end 
#endif
#include "mmi_trace.h"
#include "gui.h"
#include "calculator.h"
#include "settingprofile.h"
#include "profilegprots.h"

#ifdef __MMI_CALCULATOR__
#include "globaldefs.h"
#include "custdatares.h"
#include "gui_themes.h"
#include "wgui_categories.h"
#include "wgui_softkeys.h"
#include "historygprot.h"
#include "organizerdef.h"
#undef __NEWSIMULATOR 
#endif
#include "csw.h"



 
extern BOOL r2lMMIFlag;
//CSD end


/* 
** Define
*/
/*	Images for the calculator: Currently hardcoded */
#ifdef _MMI_OPERATOR_
#define CALC_MAX_DIGITS				8
#define CALC_MAX_BUFF_LEN			16
#else
#define CALC_MAX_DIGITS				12
#define CALC_MAX_BUFF_LEN			30
#endif

#ifdef __MMI_CALCULATOR__
#define CALC_NUM_BUTTONS			9

#if defined(__MMI_MAINLCD_220X176__) && defined(__MMI_TOUCH_CALCULATOR__)
#define CALC_SCR_X					0
#define CALC_SCR_WIDTH				CALC_TOUCH_RESULT_AREA_WIDTH
#define CALC_SCR_HEIGHT				CALC_TOUCH_RESULT_AREA_HEIGHT
#else
#define CALC_SCR_X					0
#define CALC_SCR_WIDTH				UI_device_width
#if defined(__MMI_MAINLCD_160X128__)
#define CALC_SCR_HEIGHT				45
#else
#define CALC_SCR_HEIGHT				57
#endif
#endif
#ifdef __MMI_SLIM_CALCULATOR__ /*Slim Calculator*/

#if defined (__MMI_MAINLCD_128X128__) || defined (__MMI_MAINLCD_128X160__) 

#define CALC_KEYPAD_X				 19
#define CALC_SCR_Y					0  
#ifdef __MMI_MAINLCD_128X128__
#define CALC_KEYPAD_Y				45
#else
#define CALC_KEYPAD_Y				CALC_SCR_Y + CALC_SCR_HEIGHT
#endif
#else 
#ifdef __MMI_MAINLCD_176X220__ /*Style 3*/
#define CALC_KEYPAD_X				 24
#define CALC_SCR_Y					0 + MMI_status_bar_height + MMI_TITLE_HEIGHT 	
#define CALC_KEYPAD_Y				CALC_SCR_Y + CALC_SCR_HEIGHT + 10 
#endif
#endif

#ifdef __MMI_MAINLCD_160X128__ /*Style 2*/
#if defined(__MMI_MAINLCD_160X128__)
#define CALC_KEYPAD_X				 35
#else
#define CALC_KEYPAD_X				 19
#endif
#define CALC_SCR_Y					0  
#define CALC_KEYPAD_Y				CALC_SCR_Y + CALC_SCR_HEIGHT
#endif

/*cong.li added for product QPAD[220*176] on 2009.08.11. */
#ifdef __MMI_MAINLCD_220X176__ /*Style 3*/
#define CALC_KEYPAD_X				 24
#define CALC_SCR_Y					0 + MMI_status_bar_height + MMI_TITLE_HEIGHT 	
#define CALC_KEYPAD_Y				CALC_SCR_Y + CALC_SCR_HEIGHT + 10 
#endif

#else /* General Calculator*/

#if defined (__MMI_MAINLCD_128X128__) || defined (__MMI_MAINLCD_128X160__) /*Style 1 and Style 2*/
#ifdef __MMI_MAINLCD_128X128__
#define CALC_KEYPAD_X				21//21 modify by lin for cs368 @20070829			'21'guoyt modify for c128 @090402
#define CALC_SCR_Y					0  
#define CALC_KEYPAD_Y				45//45 modify by lin for cs368 @20070829			'45'guoyt modify for c128 @090402	
#else
#define CALC_KEYPAD_X				0//21 modify by lin for cs368 @20070829
#define CALC_SCR_Y					0  
#define CALC_KEYPAD_Y				CALC_SCR_Y + CALC_SCR_HEIGHT	- 39			//guoyt modify for f658@090407
#endif
#else 
#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) /*Style 3 and Style 4*/
#define CALC_KEYPAD_X				0
#define CALC_SCR_Y					0 + MMI_status_bar_height + MMI_TITLE_HEIGHT 	
#define CALC_KEYPAD_Y				CALC_SCR_Y + CALC_SCR_HEIGHT + 10 
#endif
#endif
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#ifdef __MMI_MAINLCD_320X240__ 
#define CALC_NUM_HEIGHT 22
#define MAP_IMG_WIDTH   320
#define CALC_SCR_Y               0
#define CALC_KEYPAD_X            ((CALC_SCR_WIDTH - MAP_IMG_WIDTH)/2)   
#define CALC_KEYPAD_Y            (67)
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */

#ifdef __MMI_MAINLCD_160X128__ /*Style 2*/
#define CALC_KEYPAD_X				20
#define CALC_SCR_Y					0  
#define CALC_KEYPAD_Y				CALC_SCR_Y + CALC_SCR_HEIGHT
#endif

/*cong.li added for product QPAD[220*176] on 2009.08.11. */
#if defined(__MMI_MAINLCD_220X176__) && defined(__MMI_TOUCH_CALCULATOR__) /*Style 3 and Style 4*/

#define CALC_KEYPAD_X				3
#define CALC_SCR_Y					(TOUCH_CALC_SCREEN_Y + CALC_TOUCH_RESULT_AREA_Y)  	
#define CALC_KEYPAD_Y				CALC_SCR_Y + CALC_SCR_HEIGHT -4//按键区域

#endif

#endif
#if defined(__MMI_MAINLCD_160X128__)
#define CALC_OPERAND1_Y				CALC_SCR_Y + 14
#define CALC_OPERAND2_Y				CALC_SCR_Y + 31
#else
#define CALC_OPERAND1_Y				CALC_SCR_Y + 19
#define CALC_OPERAND2_Y				CALC_SCR_Y + 36
#endif
#if defined(__MMI_MAINLCD_220X176__) && defined(__MMI_TOUCH_CALCULATOR__)
#define CALC_KEYPAD_WIDTH			CALC_TOUCH_KEYPAD_WIDTH
#define CALC_KEYPAD_HEIGHT			CALC_TOUCH_KEYPAD_HEIGHT

#else
#define CALC_KEYPAD_WIDTH			86
#define CALC_KEYPAD_HEIGHT			63
#endif

#define CALC_OPERATOR_X				5
#define CALC_OPERATOR_Y				CALC_OPERAND2_Y
#define CALC_MEM_X1					2
#define CALC_MEM_Y1					CALC_SCR_Y+1
#define CALC_MEM_X2					CALC_SCR_WIDTH - 2
#define CALC_MEM_Y2					CALC_OPERAND1_Y - 1

#define RESULT_NOT_EXCEED			0
#define RESULT_EXCEED				1

#define CALC_RSK_BACK				0
#define CALC_RSK_CLEAR				1

#if defined (WIN32) || defined (__WIN32__)   
#define CALC_MAX_VALUE				999999999999l
#else
#ifdef _MMI_OPERATOR_
const U8  CALC_MAX_LEFT[OPERATOR_LEN + 1] = "99999999";
const U8 CALC_MIN_LEFT[OPERATOR_LEN + 1] = "0.000001";
#else
#define CALC_MAX_VALUE				999999999999ll
#define CALC_MIN_VALUE				0.0000000001
#endif
#endif


#define CALC_ZEOR_LEN				1
#define CALC_ZEOR_DOT_LEN				3

#define CALC_SYM_ERROR		(UI_character_type)'E'
#define CALC_SYM_EQUAL		(UI_character_type)'='
#define CALC_SYM_PLUS		(UI_character_type)'+'
#define CALC_SYM_MINUS		(UI_character_type)'-'
#define CALC_SYM_MUL		(UI_character_type)'x'
#define CALC_SYM_DIV		(UI_character_type)'/'
#define CALC_SYM_DOT		(UI_character_type)'.'
#define CALC_SYM_ZERO		(UI_character_type)'0'
#define CALC_SYM_NULL		(UI_character_type)'\0'

#define CALC_RESERVED_SPACE_FOR_OPERATOR		20

/* 
** Typedef 
*/
typedef struct
{	
	S32					x;
	S32					y;
	UI_image_type		image;
}calc_img_struct;


typedef struct
{
#ifdef 	_MMI_OPERATOR_
	U8 Operand1[CALC_MAX_DIGITS + 1];
	U8 Operand2[CALC_MAX_DIGITS + 1];
#else
	DOUBLE Operand1;
	DOUBLE Operand2;
#endif
#ifndef __MMI_SLIM_CALCULATOR__
#ifdef _MMI_OPERATOR_
	U8 MemValue[CALC_MAX_DIGITS + 1];
	UI_character_type MemValueBuf[CALC_MAX_BUFF_LEN + 2];
	U8 MemValuelen;
#else
	DOUBLE MemValue;
#endif
#endif
	calc_img_struct OperatorImg[CALC_NUM_BUTTONS];
#ifdef _MMI_OPERATOR_
	UI_character_type Operand1Buf[CALC_MAX_BUFF_LEN + 2];
	UI_character_type Operand2Buf[CALC_MAX_BUFF_LEN + 2];
#else
	UI_character_type Operand1Buf[CALC_MAX_DIGITS + 2];
	UI_character_type Operand2Buf[CALC_MAX_DIGITS + 2];
#endif	
	U8 Operand1Len;	
	U8 Operand2Len;
	U8 Operator;
	U8 CurrHilite;
	U8 ResultState;
	U8 RSKState;
	U8 ExecFlag;		/* if just after execution */
	U8 OpToExecute;		/* operator to execute */
	U8 IsRSKPress;		
	U8 ComputeType;
}calc_context_struct;


typedef enum
{
	CALC_OP_PLUS = 0,
	CALC_OP_MINUS,
	CALC_OP_MULT,
	CALC_OP_DIV,
	CALC_OP_EQUAL,
	CALC_OP_MP,
	CALC_OP_MM,
	CALC_OP_MC,
	CALC_OP_MR,
	CALC_OP_NONE
}CALC_OPER_ENUM;

#ifdef  __MMI_TOUCH_CALCULATOR__ 

typedef enum 
{	
	TOUCH_CALC_KEY0=0,
	TOUCH_CALC_KEY1,
	TOUCH_CALC_KEY2,
	TOUCH_CALC_KEY3,
	TOUCH_CALC_KEY4,
	TOUCH_CALC_KEY5,
	TOUCH_CALC_KEY6,
	TOUCH_CALC_KEY7,
	TOUCH_CALC_KEY8,
	TOUCH_CALC_KEY9,
	TOUCH_CALC_KEY_DOT=20 //pound key or hash key
}TOUCH_CALC_NUM_KEY_ENUM;

typedef enum 
{	
	TOUCH_CALC_OP_PLUS=CALC_OP_PLUS,
	TOUCH_CALC_OP_MINUS=CALC_OP_MINUS,
	TOUCH_CALC_OP_MULT=CALC_OP_MULT,
	TOUCH_CALC_OP_DIV=CALC_OP_DIV,
	TOUCH_CALC_OP_MP=CALC_OP_MP,
	TOUCH_CALC_OP_MM=CALC_OP_MM,
	TOUCH_CALC_OP_MC=CALC_OP_MC,
	TOUCH_CALC_OP_MR=CALC_OP_MR,
	TOUCH_CALC_OP_EQUAL=CALC_OP_EQUAL
}TOUCH_CALC_SYM_KEY_ENUM;

typedef enum
{
	TOUCH_CALC_PEN_NONE,
	TOUCH_CALC_PEN_LSK,
	TOUCH_CALC_PEN_RSK,
	TOUCH_CALC_PEN_NUM_KEY
} touch_calc_pen_state_enum;

const S32 TouchCalcKeys[CALC_TOUCH_NROWS][CALC_TOUCH_NCOLUMNS]=
{
	{TOUCH_CALC_OP_MP,TOUCH_CALC_OP_MM,TOUCH_CALC_OP_MR,TOUCH_CALC_OP_MC},
	{TOUCH_CALC_KEY7,TOUCH_CALC_KEY8,TOUCH_CALC_KEY9,TOUCH_CALC_OP_DIV},
	{TOUCH_CALC_KEY4,TOUCH_CALC_KEY5,TOUCH_CALC_KEY6,TOUCH_CALC_OP_MULT},
	{TOUCH_CALC_KEY1,TOUCH_CALC_KEY2,TOUCH_CALC_KEY3,TOUCH_CALC_OP_MINUS},
	{TOUCH_CALC_KEY0,TOUCH_CALC_KEY_DOT,TOUCH_CALC_OP_EQUAL,TOUCH_CALC_OP_PLUS}
};

const UI_image_ID_type	calculator_key_image[]=
{
	IMG_ID_TOUCH_CALC_MP,
	IMG_ID_TOUCH_CALC_MM,
	IMG_ID_TOUCH_CALC_MR,
	IMG_ID_TOUCH_CALC_MC,
	IMG_ID_TOUCH_CALC_KEY7,
	IMG_ID_TOUCH_CALC_KEY8,
	IMG_ID_TOUCH_CALC_KEY9,
	IMG_ID_TOUCH_CALC_DIVIDE,
	IMG_ID_TOUCH_CALC_KEY4,
	IMG_ID_TOUCH_CALC_KEY5,
	IMG_ID_TOUCH_CALC_KEY6,
	IMG_ID_TOUCH_CALC_MULTIPLY,
	IMG_ID_TOUCH_CALC_KEY1,
	IMG_ID_TOUCH_CALC_KEY2,
	IMG_ID_TOUCH_CALC_KEY3,
	IMG_ID_TOUCH_CALC_MINUS,
	IMG_ID_TOUCH_CALC_KEY0,
	IMG_ID_TOUCH_CALC_KEY_DOT,
	IMG_ID_TOUCH_CALC_KEY_EQUAL,
	IMG_ID_TOUCH_CALC_PLUS
};
#endif

/* 
** Local Variable
*/

const color_t gCalcResultScrColor 	= {255,234,182,100};
const color_t gCalcResultBorderColor	= {255,186,64,100};
const color_t gCalcTextColor			= {0,0,0,100};
#ifdef __MMI_TOUCH_CALCULATOR__ 
//start 
const color_t gTouchCalcResultScrColor 	= {230,230,230,100};  //hansl tbm780 2007/07/12
//end 
#endif
extern const U8 gCalcImgCoor[][2];

extern byte	matrix_main_menu_highlight_bitmap_data[]; 

/* 
** Local Function
*/
#ifdef __MMI_TOUCH_CALCULATOR__
void mmi_touchcalc_pen_down_hdlr(mmi_pen_point_struct point );
void mmi_touchcalc_pen_up_hdlr(mmi_pen_point_struct point );
void mmi_touchcalc_pen_move_hdlr(mmi_pen_point_struct point );
void mmi_touchcalc_pen_repeat_hdlr(mmi_pen_point_struct point );
void mmi_touchcalc_pen_long_tap_hdlr(mmi_pen_point_struct point );
#endif

/* 
** Global Variable
*/
calc_context_struct *g_calc_cntx = NULL;
#ifdef __MMI_TOUCH_CALCULATOR__
calculator_keypad_struct  g_calculator_keypad;
touch_calc_pen_state_enum	calc_pen_on_object=TOUCH_CALC_PEN_NONE;
#define CALC_PEN_DOWN_ON_KEYPAD		0x0001
BOOL TOUCH_CALC_SELECT_KEY = TRUE;
#endif

/* 
** Global Function
*/

/* Button co-ordinates for the on screen calculator keypad */

#endif
#define CALC_UTILITY
#ifdef _MMI_OPERATOR_	//add by hasl 061108
void CalcDouble2String(S8 *num, S8 *result)
{
	S8 precision;
	/* if Operand1 value is negative, negaive sign should not be treated as one digits*/
	if (num[0] == '-') 			 
		precision = CalcComputePrecision(num, CALC_MAX_DIGITS + 1);  
	else 
		precision = CalcComputePrecision(num, CALC_MAX_DIGITS); 
	coolsand_UI_float_string(num, precision, result);
}
//add by liuxn 060616 start
#else
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
void CalcDouble2String(DOUBLE num, S8* result)
{
	S32 precision;
	/* if Operand1 value is negative, negaive sign should not be treated as one digits*/
	if(num <0) 			 
		precision = CalcComputePrecision(num, CALC_MAX_DIGITS+1);  
	else 
		precision = CalcComputePrecision(num, CALC_MAX_DIGITS); 
	coolsand_UI_float_string(num,precision,result);
}
#else
void CalcDouble2String(DOUBLE num, S8* result)
{
}
#endif
#endif
//add by liuxn 060616 end

/*****************************************************************************
* FUNCTION         :   Judgeifover
*
* DESCRIPTION      :   判断运算结果是否越界 
* PARAMETERS       :   pointer which is CALOPERATOR_STRUCT
* RETURNS          :   true or false
* GLOBALS AFFECTED :   None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hasl 061108
BOOL Judgeifover(CALOPERATOR_STRUCT *result)
{
/*dongwg Del Start For Ver: TBM780  on 2007-8-23 9:22 */
	#if 0
	U8 t, h;
	U8 temp[OPERATOR_LEN];
	#endif
/*dongwg Del End  For Ver: TBM780  on 2007-8-23 9:22 */

	ASSERT(result != NULL);
		
	if (result->Exponent > 8 || result->Fraction > 6)
	{
		return FALSE;
	}
		
	return TRUE;
}
#endif

/*****************************************************************************
* FUNCTION         :   CaladdpationMinus
*
* DESCRIPTION      :   防止除数为0
* PARAMETERS       :   pointer which is CALOPERATOR_STRUCT
* RETURNS          :   None
* GLOBALS AFFECTED :   None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061129
void  CalcChangeOperator(CALOPERATOR_STRUCT *operator)
{
	ASSERT(operator != 0);
	if (operator->Exponent != 0 && operator->Fraction != 0)
	{
		U8 i;
		//dongwg del for the bug n0nnn.n change to n0nnn start 
		#if 0 
		if (operator->Fraction == 1 )
		{
			if (operator->operBuff[1] == 0)
				operator->Fraction = 0;
		}
		else
		#endif
		//dongwg del for the bug n0nnn.n change to n0nnn end
		{
			for (i = 0; i <= (operator->Exponent + operator->Fraction); )
			{
				if (operator->operBuff[i] == 0)
				{
					i++;
				}
				else				
					break;
					
			}
			i--;				
			if (i == (operator->Exponent + operator->Fraction))
			{
				operator->Fraction = 0;
			}
		}												
	}
}
#endif

/*****************************************************************************
* FUNCTION          :   CaladdpationPlus
* DESCRIPTION       :   加法运算
* PARAMETERS        :   two pointers which are CALOPERATOR_STRUCT
* RETURNS           :   None
* GLOBALS AFFECTED  :   None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061201
void CaladdpationPlus(CALOPERATOR_STRUCT *op1, CALOPERATOR_STRUCT *op2)
{
	U8 i;
	U8 temp[OPERATOR_LEN];
			
	ASSERT(op1 != 0 && op2 != 0);
	mmi_trace(g_sw_ORGANIZER, "begin-------CALC_OP_PLUS-------end");
	CalculatorAligDot(op1, op2);

	if (op1->Symbol == OPERATORSYMBOL_NEGA)
	{
		if (CalculatorCmpOperator(op1->operBuff, op2->operBuff) >= 0)
				CalculatorSub(op1->operBuff, op2->operBuff);
		else
		{
			for (i = 0; i < OPERATOR_LEN + 1; i++)
			{
				temp[i] = op2->operBuff[i];
				op2->operBuff[i] = op1->operBuff[i];
				op1->operBuff[i] = temp[i];
			}
					
			if (op1->Symbol == OPERATORSYMBOL_NEGA)
				op1->Symbol = OPERATORSYMBOL_POST;
			else
			{
				op1->Symbol = OPERATORSYMBOL_NEGA;
			}
					
				CalculatorSub(op1->operBuff, op2->operBuff);					
		}				
	}
	else
	{
		CalculatorAdd(op1->operBuff, op2->operBuff);														
	}
			
	if (op1->Exponent < op2->Exponent)
	{
		op1->Exponent = op2->Exponent;
	}
	if (op1->Fraction < op2->Fraction)
	{
		op1->Fraction = op2->Fraction;	
	}
			
	CalcAdjustResult1(op1);	

	if (op1->Fraction != 0 && (op1->Exponent + op1->Fraction) > 7)
	{
		if (op1->Exponent < 7)
		{
			op1->Fraction = 7 - op1->Exponent;
		}
		/*dongwg Add Start For 6118 Ver: TBM780  on 2007-8-7 15:04 */
		else if(op1->Exponent == 7)
		{
			op1->Fraction =1;
		}
		/*dongwg Add End  For 6118 Ver: TBM780  on 2007-8-7 15:04 */
		else
		{
			op1->Fraction = 0;
		}
				
	}
}
#endif

/*****************************************************************************
* FUNCTION          :   CaladdpationEXMP
* DESCRIPTION       :   连续加法
* PARAMETERS        :   two pointers which are CALOPERATOR_STRUCT
* RETURNS           :   None
* GLOBALS AFFECTED  :   None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061201
void CaladdpationEXMP(CALOPERATOR_STRUCT *result1, CALOPERATOR_STRUCT *result2)
{
	U8 i;
	U8 temp[OPERATOR_LEN];
	
	ASSERT(result1 != 0 && result2 != 0);
	if (result1->Symbol == result2->Symbol )
		{
			CalculatorAdd(result1->operBuff, result2->operBuff);	
		}    
		else
		{			
			if (CalculatorCmpOperator(result1->operBuff, result2->operBuff) >= 0)
			{
				if (result1->Symbol == OPERATORSYMBOL_NEGA)						
						result1->Symbol = OPERATORSYMBOL_NEGA;
				else
				{
						result1->Symbol = OPERATORSYMBOL_POST;
				}
				
				CalculatorSub(result1->operBuff, result2->operBuff);
				
			}
			else
			{
				/*dongwg Modify Start Ver: TBM780  on 2007-9-20 
				小数最多6位,整数最多8位,小数点一位,此函数之前右移了一位,所以改成16*/
				//for (i = 0; i < 8; i++)
				for (i = 0; i < 16; i++)
				/*dongwg Modify End Ver: TBM780  on 2007-9-20*/
				{
					temp[i] = result2->operBuff[i];
					result2->operBuff[i] = result1->operBuff[i];
					result1->operBuff[i] = temp[i];
				}
				if (result1->Symbol == OPERATORSYMBOL_NEGA)
					result1->Symbol = OPERATORSYMBOL_POST;
				else
				{
					result1->Symbol = OPERATORSYMBOL_NEGA;
				}
				
				CalculatorSub(result1->operBuff, result2->operBuff);	
				
			}		
			
		}	
		
		if (result1->Exponent < result2->Exponent)
		{
			result1->Exponent = result2->Exponent;
		}
		if (result1->Fraction<result2->Fraction)
		{
			result1->Fraction = result2->Fraction;	
		}
		
		CalcAdjustResult1(result1);
		if (result1->Fraction != 0 && (result1->Exponent + result1->Fraction) > 7)
		{
			if (result1->Exponent < 7)
			{
				result1->Fraction = 7 - result1->Exponent;
			}
			/*dongwg Add Start For 6118 Ver: TBM780  on 2007-8-7 15:04 */
			else if(result1->Exponent == 7)
			{
				result1->Fraction =1;
			}
			/*dongwg Add End  For 6118 Ver: TBM780  on 2007-8-7 15:04 */
			else
			{
				result1->Fraction = 0;
			}
					
		}
}
#endif

/*****************************************************************************
* FUNCTION          :   CaladdpationMinus
* DESCRIPTION       :   减法运算
* PARAMETERS        :   two pointers which are CALOPERATOR_STRUCT
* RETURNS           :   None
* GLOBALS AFFECTED  :   None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061201
void CaladdpationMinus(CALOPERATOR_STRUCT *op1, CALOPERATOR_STRUCT *op2)
{
	U8 i;
	U8 temp[OPERATOR_LEN + 1];	
			
	ASSERT(op1 != 0 && op2 != 0);
	CalculatorAligDot(op1, op2);

	if (op1->Symbol == OPERATORSYMBOL_NEGA)
	{
		CalculatorAdd(op1->operBuff,op2->operBuff);
	}
	else
	{
		if (CalculatorCmpOperator(op1->operBuff, op2->operBuff) >= 0)
		{
			CalculatorSub(op1->operBuff, op2->operBuff);	
		}
		else
		{
			for (i = 0; i < OPERATOR_LEN + 1; i++)
			{
				temp[i] = op2->operBuff[i];
				op2->operBuff[i] = op1->operBuff[i];
				op1->operBuff[i] = temp[i];
			}	
					
			if (op1->Symbol == OPERATORSYMBOL_POST)
				op1->Symbol = OPERATORSYMBOL_NEGA;
			else
			{
				op1->Symbol = OPERATORSYMBOL_POST;
			}
					
				CalculatorSub(op1->operBuff, op2->operBuff);
		}	
	}
			
			
	if (op1->Exponent < op2->Exponent)
	{
		op1->Exponent = op2->Exponent;
	}
			
	if (op1->Fraction < op2->Fraction)
	{
		op1->Fraction = op2->Fraction;	
	}
			
	CalcAdjustResult1(op1);
	if (op1->Fraction != 0 && (op1->Exponent + op1->Fraction) > 7)
	{
		if (op1->Exponent < 7)
		{
			op1->Fraction = 7 - op1->Exponent;
		}
		/*dongwg Add Start For 6118 Ver: TBM780  on 2007-8-7 15:04 */
		else if(op1->Exponent == 7)
		{
			op1->Fraction = 1;
		}
		/*dongwg Add End  For 6118 Ver: TBM780  on 2007-8-7 15:04 */
		else
		{
			op1->Fraction = 0;
		}
				
	}
}
#endif

/*****************************************************************************
* FUNCTION          :   CaladdpationEXMM
* DESCRIPTION       :   连续减法
* PARAMETERS        :   two pointers which are CALOPERATOR_STRUCT
* RETURNS           :   None
* GLOBALS AFFECTED  :   None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061201
void CaladdpationEXMM(CALOPERATOR_STRUCT *result1, CALOPERATOR_STRUCT *result2)
{
	U8 i;
	U8 temp[OPERATOR_LEN];
	
	ASSERT(result1 != 0 && result2 != 0);
	if (result1->Symbol == result2->Symbol )
		{
			if (CalculatorCmpOperator(result1->operBuff, result2->operBuff) >= 0)
			{
				CalculatorSub(result1->operBuff, result2->operBuff);
			}
			else
			{
				/*dongwg Modify Start For 6632 Ver: TBM780  on 2007-9-12 13:3 
				小数最多6位,整数最多8位,小数点一位,此函数之前右移了一位,所以改成16*/
				//for (i = 0; i < 8; i++)
				for (i = 0; i < 16; i++)
				/*dongwg Modify End  For 6632 Ver: TBM780  on 2007-9-12 13:3 */
				{
						temp[i] = result2->operBuff[i];
						result2->operBuff[i] = result1->operBuff[i];
						result1->operBuff[i] = temp[i];
				}
				
				if (result1->Symbol == OPERATORSYMBOL_POST)
				{
					result1->Symbol = OPERATORSYMBOL_NEGA;
					/*dongwg Modify Start For 6603 Ver: TBM780  on 2007-9-6 10:5 */
					CalculatorSub(result1->operBuff, result2->operBuff);
					//CalculatorAdd(result1->operBuff, result2->operBuff);
					/*dongwg Modify End  For 6603 Ver: TBM780  on 2007-9-6 10:5 */
				}
			}			
		}
		else
		{			
				
			if (result1->Symbol == OPERATORSYMBOL_NEGA)
			{
				result1->Symbol = OPERATORSYMBOL_NEGA;
				CalculatorAdd(result1->operBuff, result2->operBuff);
			}
			else
			{
				result1->Symbol = OPERATORSYMBOL_POST;
				CalculatorAdd(result1->operBuff, result2->operBuff);
			}																														
		}	
		
		if (result1->Exponent < result2->Exponent)
		{
			result1->Exponent = result2->Exponent;
		}
		if (result1->Fraction < result2->Fraction)
		{
			result1->Fraction = result2->Fraction;	
		}
		
		CalcAdjustResult1(result1);
		if (result1->Fraction != 0 && (result1->Exponent + result1->Fraction) > 7)
		{
			if (result1->Exponent < 7)
			{
				result1->Fraction = 7 - result1->Exponent;
			}
			/*dongwg Add Start For 6118 Ver: TBM780  on 2007-8-7 15:04 */
			else if(result1->Exponent == 7)
			{
				result1->Fraction =1;
			}
			/*dongwg Add End  For 6118 Ver: TBM780  on 2007-8-7 15:04 */
			else
			{
				result1->Fraction = 0;
			}
					
		}
}
#endif

/*****************************************************************************
* FUNCTION
*	coolsand_UI_atof
* DESCRIPTION
*   Convert string to float
* PARAMETERS
*	s	IN	string to be converted
* RETURNS
*	float value of the string
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void coolsand_UI_atof(CALOPERATOR_STRUCT *result,  U8 *stringCnt, UI_string_type  s)
{
	S8 ansii_str[CALC_MAX_DIGITS + 1];		
	U8 count = 0;

	ASSERT(result != NULL && stringCnt != NULL);

	memset(ansii_str, 0, CALC_MAX_DIGITS + 1);
	count = UnicodeToAnsii(ansii_str, (S8*)s);		
	CalculatorGetOperator(ansii_str, stringCnt,result);	
}
#else
DOUBLE coolsand_UI_atof(UI_string_type  s)
{
	S8 ansii_str[CALC_MAX_BUFF_LEN];
	UnicodeToAnsii((S8*) ansii_str,(S8*)s);
	//	return atof(ansii_str);		//delete by liuxn 060616
	return 0.0;
}
#endif
/*****************************************************************************
* FUNCTION
*	coolsand_UI_float_string
* DESCRIPTION
*   Convert float to string
* PARAMETERS
*	dob_val		IN		value to be converted.
*	prec_val	IN		number of precision.
*	out_buffer	IN/OUT	pointer to the result string
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void  coolsand_UI_float_string(S8 *dob_val, S8 prec_val, S8 *out_buffer)
{
	S8 ansii_str[CALC_MAX_BUFF_LEN + 1];
	S8 fmt[8];

	ASSERT(dob_val != NULL);
	ASSERT(out_buffer != NULL);
	
	memset(ansii_str, 0, CALC_MAX_BUFF_LEN + 1);
	memset(fmt, 0, 8);

	sprintf(fmt,  "%%.%df",  prec_val);
	sprintf(ansii_str,  fmt,  *dob_val);
	AnsiiToUnicodeString(out_buffer,  ansii_str);
}
#else
void  coolsand_UI_float_string(DOUBLE dob_val,S32 prec_val,S8* out_buffer)
{
	S8 ansii_str[CALC_MAX_BUFF_LEN];
	S8 fmt[8];
	memset(ansii_str,0,CALC_MAX_BUFF_LEN);
	memset(fmt,0,8);

	sprintf(fmt, "%%.%df", prec_val);
	sprintf(ansii_str, fmt, dob_val);
	AnsiiToUnicodeString(out_buffer,ansii_str);
}
#endif


/*****************************************************************************
* FUNCTION
*	CalcComputePrecision
* DESCRIPTION
*   To retrive the length of precision part of a given number.
* PARAMETERS
*	result		IN	the number to be computed.
*	max_digits	IN	maximum digits of the number
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
S8 CalcComputePrecision(S8 *result, S16 max_digits)
{
	S32 prec_val = 0 ,j = 0;
	S32 index = 0;
	U8 str[CALC_MAX_BUFF_LEN];
	S8 fmt[8];
	memset(fmt, 0, 8);
	
	sprintf(fmt, "%%.%dg", max_digits);
	memset(str,'\0', CALC_MAX_BUFF_LEN);
	sprintf((S8*)str, fmt, *result);

	/* to find out the first '.' */
	for (index = 0; index<max_digits; index++)
	{
		if (str[index] == '.')	/* 46 */
			break;
	}

	j = strlen((S8*)str);

	if (j > max_digits)
	{
		 
		j = (max_digits) - 1;
	}
	else 
	{
		j--;
	}
	for ( ; j>index; j--)
	{
		if (str[j] != '0')		/* 48 */
		{
			prec_val = (j - index);
			break;
		}
	}

	return prec_val;
	
}
#else
S32 CalcComputePrecision(DOUBLE result, S16 max_digits)
{
	S32 prec_val =0 ,index= 0,j= 0;
	U8 str[CALC_MAX_BUFF_LEN];
	S8 fmt[8];
	memset(fmt,0,8);
	
	sprintf(fmt, "%%.%dg", max_digits);
	memset(str,'\0',CALC_MAX_BUFF_LEN);
	sprintf((S8*)str,fmt,result);

	/* to find out the first '.' */
	for(index= 0;index<max_digits;index++)
	{
		if(str[index] == '.')	/* 46 */
			break;
	}

	j = strlen((S8*)str);

	if(j>max_digits)
	{
		 
		j = (max_digits) - 1;
	}
	else 
	{
		j--;
	}
	for( ;j>index;j--)
	{
		if(str[j]!= '0')		/* 48 */
		{
			prec_val = (j - index);
			break;
		}
	}

	return prec_val;
	
}
#endif
#ifdef __MMI_CALCULATOR__

#define CALC_INITIALIZE
/*****************************************************************************
* FUNCTION
*  HighlightCalcMenu
* DESCRIPTION
*   Highlight handler of Calculator menu item. 
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightCalcMenu(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(CalcPreEntryApp,KEY_EVENT_UP);
    /*+ zhouqin 2011-02-16 modify for q6 320X240 */	
    #ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(CalcPreEntryApp,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
    #endif
    /*- zhouqin 2011-02-16 modify for q6 320X240 */	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    /*+ zhouqin 2011-02-16 modify for q6 320X240 */	
    #ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
    #endif
    /*- zhouqin 2011-02-16 modify for q6 320X240 */	
}


/*****************************************************************************
* FUNCTION
*  CalcInit
* DESCRIPTION
*   Initialize highlight handler and global vairables.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcInit(void)
{
#if defined(__T800_MAIN_MENU__)
	SetHiliteHandler(EXTRA_CALCULATOR_MENU,HighlightCalcMenu);
#else
	SetHiliteHandler(ORGANIZER_CALCULATOR_MENU,HighlightCalcMenu);
#endif
}


/*****************************************************************************
* FUNCTION
*  CalcDeInit
* DESCRIPTION
*   Free dynamic memory.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcDeInit(void)
{
	if(g_calc_cntx)
	{
		OslMfree(g_calc_cntx);
		g_calc_cntx = NULL;
	}
}


/*****************************************************************************
* FUNCTION
*	CalcFreeMemAndBack
* DESCRIPTION
*   Free memory and exit calculator application.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcFreeMemAndBack(void)
{
	if(g_calc_cntx->IsRSKPress)
	{
		g_calc_cntx->IsRSKPress = FALSE;
		SetRightSoftkeyFunction(NULL, KEY_EVENT_LONG_PRESS);
	}
	else
	{
		CalcDeInit();
		GoBackHistory();
	}
}


/*****************************************************************************
* FUNCTION
*	CalcSetRSKClear
* DESCRIPTION
*   Change RSK to "Clear"
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcSetRSKClear(void)
{
	ChangeRightSoftkey(STR_GLOBAL_CLEAR, IMG_GLOBAL_CLEAR);
	SetRightSoftkeyFunction(CalcHandleRSKLongPress,KEY_LONG_PRESS);
	SetRightSoftkeyFunction(CalcHandleClearKey,KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*	CalcSetRSKBack
* DESCRIPTION
*   Change RSK to "Back"
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcSetRSKBack(void)
{
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_OK);
	SetRightSoftkeyFunction(CalcFreeMemAndBack,KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*	CalcResetResultValue
* DESCRIPTION
*   Reset memory related variables
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcResetResultValue(void)
{
	g_calc_cntx->CurrHilite=CALC_OP_EQUAL;
	g_calc_cntx->Operator=CALC_OP_NONE;
#ifdef _MMI_OPERATOR_	//add by hansl 061108
	g_calc_cntx->Operand2[0] = '0';
#else
	g_calc_cntx->Operand1 = 0;
#endif
	memset(g_calc_cntx->Operand2Buf,0, sizeof(g_calc_cntx->Operand2Buf));
	memset(g_calc_cntx->Operand1Buf,0, sizeof(g_calc_cntx->Operand1Buf));
	g_calc_cntx->Operand2Buf[0] = CALC_SYM_ZERO;
	g_calc_cntx->Operand1Buf[0] = CALC_SYM_NULL;

	g_calc_cntx->Operand2Len=1;
	g_calc_cntx->Operand1Len=1;

	g_calc_cntx->OpToExecute=CALC_OP_NONE;
	g_calc_cntx->RSKState = CALC_RSK_BACK;
	g_calc_cntx->ExecFlag = FALSE;
	
	g_calc_cntx->ResultState = RESULT_NOT_EXCEED;

}


/*****************************************************************************
* FUNCTION
*	CalcResetMemValue
* DESCRIPTION
*   Reset memory related variables
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifndef __MMI_SLIM_CALCULATOR__
void CalcResetMemValue(void)
{
	g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
	g_calc_cntx->Operator=CALC_OP_NONE;
	g_calc_cntx->ResultState = RESULT_NOT_EXCEED;
	
#ifdef _MMI_OPERATOR_	//add by hansl 061108
	g_calc_cntx->MemValue[0] = '0';
	g_calc_cntx->MemValueBuf[0] = CALC_SYM_NULL;
	memset(g_calc_cntx->MemValueBuf, 0, sizeof(g_calc_cntx->MemValueBuf));
	g_calc_cntx->MemValuelen = 1;	
#else
	g_calc_cntx->MemValue = 0;
#endif
}
#endif

/*****************************************************************************
* FUNCTION
*	ExitCalcApp
* DESCRIPTION
*   Reset all values to default.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcResetContext()
{
	CalcResetResultValue();
#ifndef __MMI_SLIM_CALCULATOR__
	CalcResetMemValue();
#endif
}

#define CALC_DRAW_FUNC
/*****************************************************************************
* FUNCTION
*  HighlightCalcMenu
* DESCRIPTION
*   Draw keypad of calculator.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcShowKeypad(void)
{	
	coolsand_UI_reset_clip();

	coolsand_UI_show_image(CALC_KEYPAD_X,CALC_KEYPAD_Y,(UI_image_type)GetImage(IMG_ID_CALC_MAIN));
	
	coolsand_UI_show_transparent_image(CALC_KEYPAD_X+g_calc_cntx->OperatorImg[g_calc_cntx->CurrHilite].x,
						CALC_KEYPAD_Y+g_calc_cntx->OperatorImg[g_calc_cntx->CurrHilite].y,
						(UI_image_type)g_calc_cntx->OperatorImg[g_calc_cntx->CurrHilite].image,0);
#if(UI_DOUBLE_BUFFER_SUPPORT)
	/*commented the sentence by liuxl on 2009-5-14 to fix the bug-12086 of M301_BT_WAPMMS
	to ensure only to execute a time refresh screen in the function-CalcShowResultScreen()  at a time operation */
	//coolsand_UI_BLT_double_buffer(CALC_KEYPAD_X,CALC_KEYPAD_Y,UI_device_width-1,UI_device_height-1);
#endif
}


/*****************************************************************************
* FUNCTION
*	CalcShowResultScreen
* DESCRIPTION
*   Draw result screen area.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108

void CalcShowResultScreen()
{	
#ifdef _MMI_OPERATOR_	//add by hansl 061108	
	UI_character_type op = CALC_SYM_ERROR;
	S32	w;	
	//U8 minus_flag = 0;	
	//S32 temp_prec = 0;		
#if !defined( __MMI_TOUCH_CALCULATOR__) && !defined( __MMI_SLIM_CALCULATOR__ ) 
	s32 sw;
#endif
#else
	UI_character_type op = CALC_SYM_ERROR;
	S32	w;
	S8 uni_s[64];
	S32 temp_int;
	U8 minus_flag = 0;

	DOUBLE temp_res = 0;
	S32 temp_prec = 0;
	S8 ansii_str[CALC_MAX_BUFF_LEN];
	
#ifndef __MMI_SLIM_CALCULATOR__
	S8 uni_memory[64];
	s32 sw;
#endif
#endif
	coolsand_UI_lock_double_buffer();

#if defined( __MMI_TOUCH_CALCULATOR__)
	coolsand_UI_set_font(&MMI_medium_font); //hansl tbm780 2007/07/12
	coolsand_UI_set_text_color(gCalcTextColor);
	coolsand_UI_reset_clip();
	coolsand_UI_reset_text_clip();
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#ifdef __MMI_MAINLCD_320X240__ 
	coolsand_UI_fill_rectangle(g_calculator_keypad.result_area_x + 9,g_calculator_keypad.result_area_y,g_calculator_keypad.result_area_x+9+CALC_TOUCH_RESULT_AREA_WIDTH-1,g_calculator_keypad.result_area_y+CALC_TOUCH_RESULT_AREA_HEIGHT-1,gTouchCalcResultScrColor);
#else 
	coolsand_UI_fill_rectangle(g_calculator_keypad.result_area_x,g_calculator_keypad.result_area_y,g_calculator_keypad.result_area_x+CALC_TOUCH_RESULT_AREA_WIDTH-1,g_calculator_keypad.result_area_y+CALC_TOUCH_RESULT_AREA_HEIGHT-1,gTouchCalcResultScrColor);
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#else
	coolsand_UI_set_font(&MMI_medium_font);
	coolsand_UI_set_text_color(gCalcTextColor);

	/* screen background */
	coolsand_UI_reset_clip();
	coolsand_UI_reset_text_clip();
	coolsand_UI_fill_rectangle(CALC_SCR_X,CALC_SCR_Y,CALC_SCR_X+CALC_SCR_WIDTH-1,CALC_SCR_Y+CALC_SCR_HEIGHT-1,gCalcResultScrColor);
	coolsand_UI_draw_rectangle(CALC_SCR_X,CALC_SCR_Y,CALC_SCR_X+CALC_SCR_WIDTH-1,CALC_SCR_Y+CALC_SCR_HEIGHT-1,gCalcResultBorderColor);
	coolsand_UI_draw_rectangle(CALC_SCR_X+1,CALC_SCR_Y+1,CALC_SCR_X+CALC_SCR_WIDTH-2,CALC_SCR_Y+CALC_SCR_HEIGHT-2,gCalcResultBorderColor);
#endif /*end of __MMI_TOUCH_CALCULATOR__*/

#if !defined( __MMI_TOUCH_CALCULATOR__) && !defined( __MMI_SLIM_CALCULATOR__ ) // 050905 Sophie modified
	/* memory filed */
	if(r2lMMIFlag)
	{
		sw=coolsand_UI_get_character_width((UI_character_type)'[');
		coolsand_UI_move_text_cursor(CALC_MEM_X1+ sw,CALC_MEM_Y1);
	}
	else	
	{
		coolsand_UI_move_text_cursor(CALC_MEM_X1,CALC_MEM_Y1);
	}

	coolsand_UI_print_character((UI_character_type)'[');

	if(r2lMMIFlag)
	{
		sw=coolsand_UI_get_character_width((UI_character_type)']');
		coolsand_UI_move_text_cursor(CALC_MEM_X2-6+sw,CALC_MEM_Y1);
	}
	else	
	{
		coolsand_UI_move_text_cursor(CALC_MEM_X2 - 6,CALC_MEM_Y1);
	}

	coolsand_UI_print_character((UI_character_type)']');
#endif //__MMI_TOUCH_CALCULATOR__	&& __MMI_SLIM_CALCULATOR__
	if(g_calc_cntx->ResultState == RESULT_EXCEED)
	{
		op = CALC_SYM_ERROR;

		w=coolsand_UI_get_character_width(CALC_SYM_ZERO);

		if(r2lMMIFlag)
		{
			coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH - 4 - CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
		}
#if defined( __MMI_TOUCH_CALCULATOR__ )
		else
		{
			coolsand_UI_move_text_cursor(g_calculator_keypad.result_area_x + CALC_TOUCH_RESULT_AREA_WIDTH - w - 4,g_calculator_keypad.operand2_y);
		}
#else
		else	
		{
			coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH - w - 4,CALC_OPERAND2_Y);
		}
#endif	
		coolsand_UI_print_character(CALC_SYM_ZERO);
		playRequestedTone(ERROR_TONE);
	}
	else
	{
		if(g_calc_cntx->Operator==CALC_OP_EQUAL)
		{
	 		op = CALC_SYM_EQUAL;
#ifdef _MMI_OPERATOR_	//add by hansl 061108	
			w = coolsand_UI_get_string_width(g_calc_cntx->Operand1Buf);
#else

			temp_int = (S32)g_calc_cntx->Operand1;
			if(temp_int == g_calc_cntx->Operand1)
			{
				sprintf(ansii_str,"%d",temp_int);
				AnsiiToUnicodeString((S8*)uni_s,(S8*)ansii_str);
			}
			else
			{
				CalcDouble2String(g_calc_cntx->Operand1,uni_s);
			}

			w=coolsand_UI_get_string_width((UI_string_type)uni_s);
#endif			
			

			if(r2lMMIFlag)
			{
				coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH - 4 - CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
			}
#if defined( __MMI_TOUCH_CALCULATOR__ )
			else
			{
				coolsand_UI_move_text_cursor(
                        g_calculator_keypad.result_area_x + CALC_TOUCH_RESULT_AREA_WIDTH - w - 4,
                        g_calculator_keypad.operand2_y);
			}
#else
			else	
			{
				coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH - w - 4,CALC_OPERAND2_Y);
			}
#endif
			
#ifdef _MMI_OPERATOR_	//add by hansl 061108
			coolsand_UI_print_text(g_calc_cntx->Operand1Buf);
#else
			coolsand_UI_print_text((UI_string_type)uni_s);
#endif
		}
		else if(g_calc_cntx->Operator==CALC_OP_NONE)
		{
			if(g_calc_cntx->CurrHilite < CALC_OP_EQUAL) /* +,-,*,/ */
			{
				switch(g_calc_cntx->CurrHilite)
				{
					case CALC_OP_PLUS:	
						op=CALC_SYM_PLUS;
						break;
					case CALC_OP_MINUS:	
						op=CALC_SYM_MINUS;
						break;
					case CALC_OP_MULT:
						op=CALC_SYM_MUL;
						break;
					case CALC_OP_DIV:
						op=CALC_SYM_DIV;
						break;
					default:
						break;
				}
 
#ifdef _MMI_OPERATOR_	//add by hansl 061108
				w = coolsand_UI_get_string_width(g_calc_cntx->Operand2Buf);
#else
				temp_res = coolsand_UI_atof((UI_string_type) g_calc_cntx->Operand2Buf);
				CalcDouble2String(temp_res, uni_s);

				w = coolsand_UI_get_string_width((UI_string_type)uni_s);
#endif

				if(r2lMMIFlag)
				{
					coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH - 4 - CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND1_Y);
				}
#if defined( __MMI_TOUCH_CALCULATOR__ )
				else
				{
					coolsand_UI_move_text_cursor(
                            g_calculator_keypad.result_area_x + CALC_TOUCH_RESULT_AREA_WIDTH - w - 4,g_calculator_keypad.operand2_y);
				}
#else
				else	
				{
					coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH - w - 4,CALC_OPERAND1_Y);
				}
#endif
				
#ifdef _MMI_OPERATOR_	//add by hansl 061108
				coolsand_UI_print_text(g_calc_cntx->Operand2Buf);
#else
				coolsand_UI_print_text((UI_string_type)uni_s);
#endif
				w = coolsand_UI_get_character_width(CALC_SYM_ZERO);

				if(r2lMMIFlag)
				{
					coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH-4-CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
				}
#if defined( __MMI_TOUCH_CALCULATOR__ )
				else
				{
					coolsand_UI_move_text_cursor(g_calculator_keypad.result_area_x +CALC_TOUCH_RESULT_AREA_WIDTH - w - 4,g_calculator_keypad.operand2_y);
				}
#else
				else	
				{
					coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH - w - 4,CALC_OPERAND2_Y);
				}
#endif				

				coolsand_UI_print_character(CALC_SYM_ZERO);
			}
			else
			{
				op = CALC_SYM_NULL;
				if(g_calc_cntx->Operand2Buf[0]==CALC_SYM_NULL)
				{
					w=coolsand_UI_get_character_width(CALC_SYM_ZERO);

					if(r2lMMIFlag)
					{
						coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH-4-CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
					}
#if defined( __MMI_TOUCH_CALCULATOR__ )
					else
					{
						coolsand_UI_move_text_cursor(g_calculator_keypad.result_area_x + CALC_TOUCH_RESULT_AREA_WIDTH-w-4,g_calculator_keypad.operand2_y);
					}
#else
					else	
					{
						coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH-w-4,CALC_OPERAND2_Y);
					}
#endif					
					coolsand_UI_print_character(CALC_SYM_ZERO);
				}
				else
				{
					w=coolsand_UI_get_string_width(g_calc_cntx->Operand2Buf);

					if(r2lMMIFlag)
					{
						coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-4-CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
					}
#if defined( __MMI_TOUCH_CALCULATOR__ )
					else
					{
						coolsand_UI_move_text_cursor(g_calculator_keypad.result_area_x+CALC_TOUCH_RESULT_AREA_WIDTH-w-4,g_calculator_keypad.operand2_y);
					}
#else
					else	
					{
						coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-w-4,CALC_OPERAND2_Y);
					}
#endif
					coolsand_UI_print_text(g_calc_cntx->Operand2Buf);
				}
			}
		}
		else 
		{
			switch(g_calc_cntx->Operator)
			{	case CALC_OP_PLUS:
					op=CALC_SYM_PLUS;
					break;
				case CALC_OP_MINUS:
					op=CALC_SYM_MINUS;
					break;
				case CALC_OP_MULT:
					op=CALC_SYM_MUL;
					break;
				case CALC_OP_DIV:
					op=CALC_SYM_DIV;
					break;
				default:
					break;
			}			
#ifdef _MMI_OPERATOR_	//add by hansl 061108
			w = coolsand_UI_get_string_width(g_calc_cntx->Operand1Buf);
#else
			CalcDouble2String(g_calc_cntx->Operand1,uni_s);

			w = coolsand_UI_get_string_width((UI_string_type)uni_s);
#endif
			
			if(r2lMMIFlag)
			{
				coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH - 4 - CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND1_Y);
			}
#if defined( __MMI_TOUCH_CALCULATOR__ )
			else	
			{
				coolsand_UI_move_text_cursor(g_calculator_keypad.result_area_x + CALC_TOUCH_RESULT_AREA_WIDTH - w - 4,g_calculator_keypad.operand1_y);
			}
#else			
			else	
			{
				coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH - w - 4,CALC_OPERAND1_Y);
			}
#endif
			
#ifdef _MMI_OPERATOR_	//add by hansl 061108
			coolsand_UI_print_text(g_calc_cntx->Operand1Buf);
#else
			coolsand_UI_print_text((UI_string_type)uni_s);
#endif

			w=coolsand_UI_get_string_width(g_calc_cntx->Operand2Buf);
			if(g_calc_cntx->Operand2Buf[0]==CALC_SYM_NULL)
			{
				w=coolsand_UI_get_character_width(CALC_SYM_ZERO);
				if(r2lMMIFlag)
				{
					coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH - 4 -CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
				}
#if defined( __MMI_TOUCH_CALCULATOR__ )
				else	
				{
					coolsand_UI_move_text_cursor(g_calculator_keypad.result_area_x + CALC_TOUCH_RESULT_AREA_WIDTH - w - 4,g_calculator_keypad.operand2_y);
				}
#else			
				else	
				{
					coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH -w - 4, CALC_OPERAND2_Y);
				}
#endif

				coolsand_UI_print_character(CALC_SYM_ZERO);
			}
			else
			{
				if (r2lMMIFlag)
				{
					coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH - 4 - CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
				}
#if defined( __MMI_TOUCH_CALCULATOR__ )
				else	
				{
					coolsand_UI_move_text_cursor(g_calculator_keypad.result_area_x + CALC_TOUCH_RESULT_AREA_WIDTH - w - 4,g_calculator_keypad.operand2_y);
				}
#else
				else	
				{
					coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_SCR_WIDTH - w - 4, CALC_OPERAND2_Y);
				}
#endif
				coolsand_UI_print_text(g_calc_cntx->Operand2Buf);
			}
		}
#ifndef __MMI_SLIM_CALCULATOR__        
#ifdef _MMI_OPERATOR_	//add by hansl 061108
//add by hansl  to operate the memvalue 2006/11/8  begin
			if (g_calc_cntx->MemValueBuf[0] == CALC_SYM_NULL)
			{
				w = coolsand_UI_get_character_width(CALC_SYM_ZERO);

				if (r2lMMIFlag)
				{
					coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_MEM_X2 - 8, CALC_MEM_Y1);
				}
#if defined( __MMI_TOUCH_CALCULATOR__ )
				else
				{
					coolsand_UI_move_text_cursor(g_calculator_keypad.result_area_x + g_calculator_keypad.mem_x1 - w -8,g_calculator_keypad.mem_y1);
				}
#else
				else	
				{
					coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_MEM_X2 - w - 8, CALC_MEM_Y1);
				}
#endif
					coolsand_UI_print_character(CALC_SYM_ZERO);
			}
			else
			{	
//add by hansl  to operate the memvalue 2006/11/8  end
#endif
#endif

#ifndef __MMI_SLIM_CALCULATOR__ 
		/* memory field */		
#ifdef _MMI_OPERATOR_	//add by hansl 061108
				//pfnUnicodeStrcat((S8*)g_calc_cntx->MemValue, (S8*)(L" "));//modify by hansl 
				w = coolsand_UI_get_string_width(g_calc_cntx->MemValueBuf);					
#else
				CalcDouble2String(g_calc_cntx->MemValue, uni_memory);
				pfnUnicodeStrcat((S8*)uni_memory, (S8*)(L" "));
				w = coolsand_UI_get_string_width((UI_string_type)uni_memory);
#endif
				if (r2lMMIFlag)
				{
					coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_MEM_X2 - 4, CALC_MEM_Y1);
				}
#if defined( __MMI_TOUCH_CALCULATOR__ )
				else
				{
					coolsand_UI_move_text_cursor(g_calculator_keypad.result_area_x + g_calculator_keypad.mem_x1 - w - 8,g_calculator_keypad.mem_y1);
				}
#else
				else	
				{
					coolsand_UI_move_text_cursor(CALC_SCR_X + CALC_MEM_X2 - w - 8, CALC_MEM_Y1);
				}
#endif
		
#ifdef _MMI_OPERATOR_	//add by hansl 061108		
					coolsand_UI_print_text(g_calc_cntx->MemValueBuf);
#else
					coolsand_UI_print_text((UI_string_type)uni_memory);
#endif
#endif 
#ifndef __MMI_SLIM_CALCULATOR__ 
#ifdef _MMI_OPERATOR_	//add by hansl 061108
			}
#endif			
#endif
	} // if(g_calc_cntx->ResultState == RESULT_EXCEED)
	/* operator */
	if (r2lMMIFlag)
	{
		coolsand_UI_move_text_cursor(CALC_SCR_WIDTH - 4, CALC_OPERATOR_Y);
	}
#if defined( __MMI_TOUCH_CALCULATOR__ )
	else	
	{
		coolsand_UI_move_text_cursor(g_calculator_keypad.operator_x, g_calculator_keypad.operator_y);
	}
#else
	else	
	{
		coolsand_UI_move_text_cursor(CALC_OPERATOR_X, CALC_OPERATOR_Y);
	}
#endif
	coolsand_UI_print_character((UI_character_type)op);

	if (g_calc_cntx->RSKState == CALC_RSK_CLEAR)
	{
		CalcSetRSKClear();
	}
	else
	{
		CalcSetRSKBack();
	}

	show_softkey_background();
	show_left_softkey();
	show_right_softkey();

	coolsand_UI_unlock_double_buffer();
#if defined( __MMI_TOUCH_CALCULATOR__ )
	coolsand_UI_BLT_double_buffer(TOUCH_CALC_SCREEN_X,TOUCH_CALC_SCREEN_Y,TOUCH_CALC_SCREEN_X+TOUCH_CALC_SCREEN_WIDTH-1,TOUCH_CALC_SCREEN_Y+TOUCH_CALC_SCREEN_HEIGHT-1);
#else
	coolsand_UI_BLT_double_buffer(CALC_SCR_X,CALC_SCR_Y,CALC_SCR_X+CALC_SCR_WIDTH-1,CALC_SCR_Y+CALC_SCR_HEIGHT-1);
#endif //__MMI_TOUCH_CALCULATOR__
}

//add by liuxn 060616 start
#else
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616

void CalcShowResultScreen()
{	
	UI_character_type	op = CALC_SYM_ERROR;
	S32	w;
	S8 uni_s[64];
	S32 temp_int;
	U8 minus_flag = 0;

	DOUBLE temp_res = 0;
	S32 temp_prec = 0;
	S8 ansii_str[CALC_MAX_BUFF_LEN];
	
#ifndef __MMI_SLIM_CALCULATOR__
	S8 uni_memory[64];
	s32 sw;
#endif

	coolsand_UI_lock_double_buffer();

#if defined( __MMI_TOUCH_CALCULATOR__)
	coolsand_UI_set_font(&MMI_large_font);
	coolsand_UI_set_text_color(gCalcTextColor);
	coolsand_UI_reset_clip();
	coolsand_UI_reset_text_clip();
	coolsand_UI_fill_rectangle(calculator_keypad.result_area_x,calculator_keypad.result_area_y,calculator_keypad.result_area_x+CALC_TOUCH_RESULT_AREA_WIDTH-1,calculator_keypad.result_area_y+CALC_TOUCH_RESULT_AREA_HEIGHT-1,gTouchCalcResultScrColor);
#else
	coolsand_UI_set_font(&MMI_medium_font);
	coolsand_UI_set_text_color(gCalcTextColor);

	/* screen background */
	coolsand_UI_reset_clip();
	coolsand_UI_reset_text_clip();
	coolsand_UI_fill_rectangle(CALC_SCR_X,CALC_SCR_Y,CALC_SCR_X+CALC_SCR_WIDTH-1,CALC_SCR_Y+CALC_SCR_HEIGHT-1,gCalcResultScrColor);
	coolsand_UI_draw_rectangle(CALC_SCR_X,CALC_SCR_Y,CALC_SCR_X+CALC_SCR_WIDTH-1,CALC_SCR_Y+CALC_SCR_HEIGHT-1,gCalcResultBorderColor);
	coolsand_UI_draw_rectangle(CALC_SCR_X+1,CALC_SCR_Y+1,CALC_SCR_X+CALC_SCR_WIDTH-2,CALC_SCR_Y+CALC_SCR_HEIGHT-2,gCalcResultBorderColor);
#endif /*end of __MMI_TOUCH_CALCULATOR__*/

#if !defined( __MMI_TOUCH_CALCULATOR__) && !defined( __MMI_SLIM_CALCULATOR__ ) // 050905 Sophie modified
	/* memory filed */
	if(r2lMMIFlag)
	{
		sw=coolsand_UI_get_character_width((UI_character_type)'[');
		coolsand_UI_move_text_cursor(CALC_MEM_X1+ sw,CALC_MEM_Y1);
	}
	else	
		coolsand_UI_move_text_cursor(CALC_MEM_X1,CALC_MEM_Y1);

	coolsand_UI_print_character((UI_character_type)'[');

	if(r2lMMIFlag)
	{
		sw=coolsand_UI_get_character_width((UI_character_type)']');
		coolsand_UI_move_text_cursor(CALC_MEM_X2-6+sw,CALC_MEM_Y1);
	}
	else	
		coolsand_UI_move_text_cursor(CALC_MEM_X2-6,CALC_MEM_Y1);

	coolsand_UI_print_character((UI_character_type)']');
#endif //__MMI_TOUCH_CALCULATOR__	&& __MMI_SLIM_CALCULATOR__
	if(g_calc_cntx->ResultState == RESULT_EXCEED)
	{
		op = CALC_SYM_ERROR;

		w=coolsand_UI_get_character_width(CALC_SYM_ZERO);

		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-4-CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
#if defined( __MMI_TOUCH_CALCULATOR__ )
		else
			coolsand_UI_move_text_cursor(calculator_keypad.result_area_x+CALC_TOUCH_RESULT_AREA_WIDTH-w-4,calculator_keypad.operand2_y);
#else
		else	
			coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-w-4,CALC_OPERAND2_Y);
#endif	
		coolsand_UI_print_character(CALC_SYM_ZERO);
		playRequestedTone(ERROR_TONE);
	}
	else
	{
		if(g_calc_cntx->Operator==CALC_OP_EQUAL)
		{
	 		op = CALC_SYM_EQUAL;

			temp_int = (S32)g_calc_cntx->Operand1;
			if(temp_int == g_calc_cntx->Operand1)
			{
				sprintf(ansii_str,"%d",temp_int);
				AnsiiToUnicodeString((S8*)uni_s,(S8*)ansii_str);
			}
			else
			{
				CalcDouble2String(g_calc_cntx->Operand1,uni_s);
			}

			w=coolsand_UI_get_string_width((UI_string_type)uni_s);

			if(r2lMMIFlag)
				coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-4-CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
#if defined( __MMI_TOUCH_CALCULATOR__ )
			else
				coolsand_UI_move_text_cursor(calculator_keypad.result_area_x+CALC_TOUCH_RESULT_AREA_WIDTH-w-4,calculator_keypad.operand2_y);
#else
			else	
				coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-w-4,CALC_OPERAND2_Y);
#endif
			coolsand_UI_print_text((UI_string_type)uni_s);
		}
		else if(g_calc_cntx->Operator==CALC_OP_NONE)
		{
			if(g_calc_cntx->CurrHilite < CALC_OP_EQUAL) /* +,-,*,/ */
			{
				switch(g_calc_cntx->CurrHilite)
				{
					case CALC_OP_PLUS:	
						op=CALC_SYM_PLUS;
						break;
					case CALC_OP_MINUS:	
						op=CALC_SYM_MINUS;
						break;
					case CALC_OP_MULT:
						op=CALC_SYM_MUL;
						break;
					case CALC_OP_DIV:
						op=CALC_SYM_DIV;
						break;
					default:
						break;
				}
 
				temp_res = coolsand_UI_atof((UI_string_type) g_calc_cntx->Operand2Buf);
				CalcDouble2String(temp_res, uni_s);

				w=coolsand_UI_get_string_width((UI_string_type)uni_s);

				if(r2lMMIFlag)
					coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-4-CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND1_Y);
#if defined( __MMI_TOUCH_CALCULATOR__ )
				else
					coolsand_UI_move_text_cursor(calculator_keypad.result_area_x+CALC_TOUCH_RESULT_AREA_WIDTH-w-4,calculator_keypad.operand2_y);
#else
				else	
					coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-w-4,CALC_OPERAND1_Y);
#endif
				coolsand_UI_print_text((UI_string_type)uni_s);

				w=coolsand_UI_get_character_width(CALC_SYM_ZERO);

				if(r2lMMIFlag)
					coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-4-CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
#if defined( __MMI_TOUCH_CALCULATOR__ )
				else
					coolsand_UI_move_text_cursor(calculator_keypad.result_area_x+CALC_TOUCH_RESULT_AREA_WIDTH-w-4,calculator_keypad.operand2_y);
#else
				else	
					coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-w-4,CALC_OPERAND2_Y);
#endif				

				coolsand_UI_print_character(CALC_SYM_ZERO);
			}
			else
			{
				op = CALC_SYM_NULL;
				if(g_calc_cntx->Operand2Buf[0]==CALC_SYM_NULL)
				{
					w=coolsand_UI_get_character_width(CALC_SYM_ZERO);

					if(r2lMMIFlag)
						coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-4-CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
#if defined( __MMI_TOUCH_CALCULATOR__ )
					else
						coolsand_UI_move_text_cursor(calculator_keypad.result_area_x+CALC_TOUCH_RESULT_AREA_WIDTH-w-4,calculator_keypad.operand2_y);
#else
					else	
						coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-w-4,CALC_OPERAND2_Y);
#endif					
					coolsand_UI_print_character(CALC_SYM_ZERO);
				}
				else
				{
					w=coolsand_UI_get_string_width(g_calc_cntx->Operand2Buf);

					if(r2lMMIFlag)
						coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-4-CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
#if defined( __MMI_TOUCH_CALCULATOR__ )
					else
						coolsand_UI_move_text_cursor(calculator_keypad.result_area_x+CALC_TOUCH_RESULT_AREA_WIDTH-w-4,calculator_keypad.operand2_y);
#else
					else	
						coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-w-4,CALC_OPERAND2_Y);
#endif
					coolsand_UI_print_text(g_calc_cntx->Operand2Buf);
				}
			}
		}
		else 
		{
			switch(g_calc_cntx->Operator)
			{	case CALC_OP_PLUS:
					op=CALC_SYM_PLUS;
					break;
				case CALC_OP_MINUS:
					op=CALC_SYM_MINUS;
					break;
				case CALC_OP_MULT:
					op=CALC_SYM_MUL;
					break;
				case CALC_OP_DIV:
					op=CALC_SYM_DIV;
					break;
				default:
					break;
			}

			CalcDouble2String(g_calc_cntx->Operand1,uni_s);

			w=coolsand_UI_get_string_width((UI_string_type)uni_s);
			if(r2lMMIFlag)
				coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-4-CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND1_Y);
#if defined( __MMI_TOUCH_CALCULATOR__ )
			else	
				coolsand_UI_move_text_cursor(calculator_keypad.result_area_x+CALC_TOUCH_RESULT_AREA_WIDTH-w-4,calculator_keypad.operand1_y);
#else			
			else	
				coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-w-4,CALC_OPERAND1_Y);
#endif
			coolsand_UI_print_text((UI_string_type)uni_s);

			w=coolsand_UI_get_string_width(g_calc_cntx->Operand2Buf);
			if(g_calc_cntx->Operand2Buf[0]==CALC_SYM_NULL)
			{
				w=coolsand_UI_get_character_width(CALC_SYM_ZERO);
				if(r2lMMIFlag)
					coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-4-CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
#if defined( __MMI_TOUCH_CALCULATOR__ )
				else	
					coolsand_UI_move_text_cursor(calculator_keypad.result_area_x+CALC_TOUCH_RESULT_AREA_WIDTH-w-4,calculator_keypad.operand2_y);
#else			
				else	
					coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-w-4,CALC_OPERAND2_Y);
#endif

				coolsand_UI_print_character(CALC_SYM_ZERO);
			}
			else
			{
				if(r2lMMIFlag)
					coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-4-CALC_RESERVED_SPACE_FOR_OPERATOR,CALC_OPERAND2_Y);
#if defined( __MMI_TOUCH_CALCULATOR__ )
				else	
					coolsand_UI_move_text_cursor(calculator_keypad.result_area_x+CALC_TOUCH_RESULT_AREA_WIDTH-w-4,calculator_keypad.operand2_y);
#else
				else	
					coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_SCR_WIDTH-w-4,CALC_OPERAND2_Y);
#endif
				coolsand_UI_print_text(g_calc_cntx->Operand2Buf);
			}
		}
#ifndef __MMI_SLIM_CALCULATOR__ 
		/* memory field */
		CalcDouble2String(g_calc_cntx->MemValue, uni_memory);
		pfnUnicodeStrcat((S8*)uni_memory,(S8*)(L" "));
		w=coolsand_UI_get_string_width((UI_string_type)uni_memory);
		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_MEM_X2-4,CALC_MEM_Y1);
#if defined( __MMI_TOUCH_CALCULATOR__ )
		else
			coolsand_UI_move_text_cursor(calculator_keypad.result_area_x+calculator_keypad.mem_x1-w-4,calculator_keypad.mem_y1);
#else
		else	
			coolsand_UI_move_text_cursor(CALC_SCR_X+CALC_MEM_X2-w-4,CALC_MEM_Y1);
#endif
		coolsand_UI_print_text((UI_string_type)uni_memory);
#endif 
	} // if(g_calc_cntx->ResultState == RESULT_EXCEED)

	/* operator */
	if(r2lMMIFlag)
		coolsand_UI_move_text_cursor(CALC_SCR_WIDTH-4,CALC_OPERATOR_Y);
#if defined( __MMI_TOUCH_CALCULATOR__ )
	else	
		coolsand_UI_move_text_cursor(calculator_keypad.operator_x,calculator_keypad.operator_y);
#else
	else	
		coolsand_UI_move_text_cursor(CALC_OPERATOR_X,CALC_OPERATOR_Y);
#endif
	coolsand_UI_print_character((UI_character_type)op);

	if(g_calc_cntx->RSKState == CALC_RSK_CLEAR)
		CalcSetRSKClear();
	else
		CalcSetRSKBack();

	show_softkey_background();
	show_left_softkey();
	show_right_softkey();

	coolsand_UI_unlock_double_buffer();
#if defined( __MMI_TOUCH_CALCULATOR__ )
	coolsand_UI_BLT_double_buffer(TOUCH_CALC_SCREEN_X,TOUCH_CALC_SCREEN_Y,TOUCH_CALC_SCREEN_X+TOUCH_CALC_SCREEN_WIDTH-1,TOUCH_CALC_SCREEN_Y+TOUCH_CALC_SCREEN_HEIGHT-1);
#else
	coolsand_UI_BLT_double_buffer(CALC_SCR_X,CALC_SCR_Y,CALC_SCR_X+CALC_SCR_WIDTH-1,CALC_SCR_Y+CALC_SCR_HEIGHT-1);
#endif //__MMI_TOUCH_CALCULATOR__
}

//add by liuxn 060616 start
#else
void CalcShowResultScreen()
{
}
#endif
#endif
//add by liuxn 060616 end


 
#if defined( __MMI_TOUCH_CALCULATOR__) /* && defined(__MMI_UI_STYLE_4__) */  
extern UI_image_ID_type	idle_screen_wallpaper_ID;
extern gdi_handle wgui_base_layer;
extern gdi_handle wgui_layer_1;
#endif
 
/*****************************************************************************
* FUNCTION
*	CalcRedraw
* DESCRIPTION
*   Redraw calculator screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#if defined( __MMI_MAINLCD_220X176__  )
extern void hide_status_icon_bar(U8 status_bar_ID);
#endif
void CalcRedraw(void)
{	
	coolsand_UI_lock_double_buffer();
	coolsand_UI_reset_clip();

	clear_screen();
//start 
#if defined( __MMI_TOUCH_CALCULATOR__)
	 
	//if ( coolsand_UI_is_entry_new_screen()==MMI_TRUE )
	if ( coolsand_UI_is_entry_new_screen()==MMI_TRUE && wgui_is_wallpaper_on_bottom() == MMI_TRUE)
	{
		coolsand_UI_set_entry_new_screen(MMI_FALSE);
		
		gdi_layer_multi_layer_enable();
		gdi_layer_get_base_handle(&wgui_base_layer);
		coolsand_UI_layer_transparent(wgui_base_layer, UI_SUBMENU_OPACITY_BASED_LAYER, 0, 0, UI_DEVICE_WIDTH-1, UI_DEVICE_HEIGHT-1);
		if (wgui_layer_1 == GDI_LAYER_EMPTY_HANDLE)
		{
			gdi_layer_create(0, 0, UI_DEVICE_WIDTH, UI_DEVICE_HEIGHT, &wgui_layer_1);
		}
		
		gdi_layer_set_active(wgui_layer_1);
		gdi_layer_lock_frame_buffer();
		
		gdi_layer_push_clip();
		gdi_layer_reset_clip();

		gdi_layer_set_opacity(TRUE, 200);
		gdi_image_draw_animation(0,0, get_image(idle_screen_wallpaper_ID),NULL);
		//gdi_image_draw(0,0, get_image(idle_screen_wallpaper_ID));
		
		gdi_layer_pop_clip();
					
		gdi_layer_unlock_frame_buffer();
		gdi_layer_restore_base_active();

		gdi_layer_set_blt_layer(wgui_layer_1, wgui_base_layer, 0, 0);
		gdi_layer_blt_previous(0, 0, UI_DEVICE_WIDTH-1, UI_DEVICE_HEIGHT-1);

		gdi_layer_set_active(wgui_base_layer);
		gdi_draw_solid_rect(0, 0, UI_DEVICE_WIDTH-1, UI_DEVICE_HEIGHT-1,GDI_COLOR_TRANSPARENT);
	}
	else
	{
 
//		gdi_layer_get_base_handle(&wgui_base_layer);
//		coolsand_UI_layer_transparent(wgui_base_layer, UI_SUBMENU_OPACITY_BASED_LAYER, 0, 0, UI_DEVICE_WIDTH-1, UI_DEVICE_HEIGHT-1);
//		gdi_layer_set_active(wgui_base_layer);
//		gdi_draw_solid_rect(0, 0, UI_DEVICE_WIDTH-1, UI_DEVICE_HEIGHT-1,GDI_COLOR_TRANSPARENT);
		
	}
	 

	setup_calculator_keypad(&g_calculator_keypad,TOUCH_CALC_SCREEN_X,TOUCH_CALC_SCREEN_Y);
	MMI_menu_shortcut_number=-1;
	#if defined( __MMI_MAINLCD_220X176__  )
	hide_status_icon_bar(0);
	#else
	 
	 
	show_title_status_icon();
	//show_status_icons();
	 
	 
	ChangeTitleString((U8*)get_string(STR_ID_CALC_MENU));
	draw_title();	
	#endif
	show_touch_calculator_screen(TOUCH_CALC_SCREEN_X,TOUCH_CALC_SCREEN_Y,TOUCH_CALC_SCREEN_WIDTH,TOUCH_CALC_SCREEN_HEIGHT);
	CalcShowResultScreen();
#else
#if	defined (__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_240X320__)
	MMI_disable_title_shortcut_display=1;
	show_title_status_icon();
	ChangeTitleString((U8*)get_string(STR_ID_CALC_MENU));
	ChangeTitleIcon(GetRootTitleIcon(ORGANIZER_CALCULATOR_MENU));
	draw_title();	
#endif	
/*cong.li added for product QPAD[220*176] on 2009.08.11. */
#ifdef __MMI_MAINLCD_220X176__
hide_status_icon_bar(0);
#endif
	CalcShowKeypad();
	CalcShowResultScreen();
#endif //end __MMI_TOUCH_CALCULATOR__
//end 
	register_default_hide_softkeys(); //to register softkeys hide default function
	show_softkey_background();
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	show_left_softkey();
	show_right_softkey();
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}

/*****************************************************************************
* FUNCTION
*	CalcSearchDecimal
* DESCRIPTION
*   To search if there is a decimal point in current operand.
* PARAMETERS
*	None.
* RETURNS
*	TRUE if decimal exists; otherwise, FALSE.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
U8 CalcSearchDecimal(void)
{
	U8 index;
	if(g_calc_cntx->Operand2Len>1)
	{
		for(index=0; index<g_calc_cntx->Operand2Len; index++)
		{
			if(g_calc_cntx->Operand2Buf[index]== CALC_SYM_DOT) 
				return TRUE;
		}
	}
	return FALSE ;
}


/*****************************************************************************
* FUNCTION
*	CalcInsertChar
* DESCRIPTION
*   Insert a character to result screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcInsertChar(UI_character_type c)
{	
	g_calc_cntx->ComputeType = CALC_OP_NONE;
	/* if operand2 = -(12 digit) || (12 digit) */
	if((g_calc_cntx->Operand2Buf[0] == CALC_SYM_MINUS && g_calc_cntx->Operand2Len>=(CALC_MAX_DIGITS+2)) ||
		(g_calc_cntx->Operand2Buf[0] != CALC_SYM_MINUS &&g_calc_cntx->Operand2Len>=(CALC_MAX_DIGITS+1)))
	{
		playRequestedTone(ERROR_TONE);
		return;
	}
	
	if(g_calc_cntx->Operator==CALC_OP_EQUAL)
	{	
		g_calc_cntx->Operator=CALC_OP_NONE;
#ifdef _MMI_OPERATOR_	//add by hansl 061108
		g_calc_cntx->Operand1[0] = '0';
		g_calc_cntx->Operand2[0] = '0';
#else
		g_calc_cntx->Operand1=0;
		g_calc_cntx->Operand2=0;
#endif
		g_calc_cntx->Operand2Len=1;
		g_calc_cntx->Operand2Buf[0]=CALC_SYM_NULL;		
		g_calc_cntx->OpToExecute=CALC_OP_NONE;
	}

	if(c == CALC_SYM_DOT && g_calc_cntx->Operand2Len == 1) /* insert "." after zero */
	{
		g_calc_cntx->Operand2Buf[0]=CALC_SYM_ZERO;
		g_calc_cntx->Operand2Len++;
	}

	/*If input zero ,str is 0 + '\0' */
	if(!(g_calc_cntx->Operand2Len == 2 && c == CALC_SYM_ZERO && g_calc_cntx->Operand2Buf[0] == CALC_SYM_ZERO)	)
	{
		if(g_calc_cntx->Operand2Len == 2 && c != CALC_SYM_ZERO 
			&& g_calc_cntx->Operand2Buf[0] == CALC_SYM_ZERO && c != CALC_SYM_DOT)
		{
			g_calc_cntx->Operand2Buf[0]=(UI_character_type)c;
			g_calc_cntx->Operand2Buf[1]=CALC_SYM_NULL;	
		}
		else
		{
			g_calc_cntx->Operand2Buf[g_calc_cntx->Operand2Len-1]=(UI_character_type)c;
			g_calc_cntx->Operand2Buf[g_calc_cntx->Operand2Len]=CALC_SYM_NULL;
			g_calc_cntx->Operand2Len++; 	
		}
	}
	g_calc_cntx->RSKState = CALC_RSK_CLEAR;
	
	CalcShowResultScreen();
}


/*****************************************************************************
* FUNCTION
*	CalcExecMR
* DESCRIPTION
*   Execute MR operator.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifndef __MMI_SLIM_CALCULATOR__
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void CalcExecMR(void)
{
	memcpy(g_calc_cntx->Operand2Buf, (UI_string_type)g_calc_cntx->MemValueBuf, sizeof(g_calc_cntx->Operand2Buf));
	/*dongwg Modify Start For 6715 Ver: TBM780  on 2007-9-19 18:59 */
	//g_calc_cntx->MemValuelen = g_calc_cntx->Operand2Len;
	g_calc_cntx->Operand2Len = g_calc_cntx->MemValuelen;
	/*dongwg Modify End  For 6715 Ver: TBM780  on 2007-9-19 18:59 */
	g_calc_cntx->Operator = CALC_OP_NONE;	
}
#else
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
void CalcExecMR(void)
{
	CalcResetResultValue();

	CalcDouble2String(g_calc_cntx->MemValue, (S8*)g_calc_cntx->Operand2Buf);

	g_calc_cntx->Operand2Len = pfnUnicodeStrlen((S8*) g_calc_cntx->Operand2Buf) + 1;
//start 
#ifndef __MMI_TOUCH_CALCULATOR__
	if(g_calc_cntx->Operand2Len > 1)
		g_calc_cntx->RSKState = CALC_RSK_CLEAR;
	else
		g_calc_cntx->RSKState = CALC_RSK_BACK;
#endif
//end 
}
#else
void CalcExecMR(void)
{
}
#endif
#endif

/*****************************************************************************
* FUNCTION
*	CalcExecMP
* DESCRIPTION
*   Execute M+ operator.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void CalcExecMP(void)
{
	BOOL value = TRUE;
	
	CALOPERATOR_STRUCT *result1 = NULL;
	CALOPERATOR_STRUCT *result2 = NULL;

	result1 = (CALOPERATOR_STRUCT *)OslMalloc(sizeof(CALOPERATOR_STRUCT));
	result2 = (CALOPERATOR_STRUCT *)OslMalloc(sizeof(CALOPERATOR_STRUCT));	
	ASSERT(result1 != NULL);
	ASSERT(result2 != NULL);
	memset(result1->operBuff, 0, OPERATOR_LEN + 1);
	memset(result2->operBuff, 0, OPERATOR_LEN + 1);
	result1->Symbol = OPERATORSYMBOL_POST;
	result1->Exponent = 1;
	result1->Fraction = 0;		
	result2->Symbol = OPERATORSYMBOL_POST;
	result2->Exponent = 1;
	result2->Fraction = 0;

	if (g_calc_cntx->Operand2Buf[0] != CALC_SYM_NULL)
	{				
		
		CalcGetPreOperand(result2, g_calc_cntx->Operand2Buf);
		CalcGetPreOperand(result1, g_calc_cntx->MemValueBuf);
		CalculatorAligDot(result1, result2);	
		CaladdpationEXMP(result1, result2);
		Calresulttoberound(result1);//dongwg move it here 20070919
		value = Judgeifover(result1);
		if (!value )//judge the value  over
		{
			g_calc_cntx->ResultState = RESULT_EXCEED;
			playRequestedTone(ERROR_TONE);	
		}
		
		//Calresulttoberound(result1);//dongwg del 20070919
		Calgetrealresult(result2, result1);
		/*dongwg Modify Start For 6500 Ver: TBM780  on 2007-8-29 19:28 */
		AnsiiToUnicodeString((S8*)g_calc_cntx->MemValueBuf, (S8*)result2->operBuff);		
		#if 0
		AnsiiToUnicodeString((S8*)g_calc_cntx->Operand2Buf, (S8*)result2->operBuff);
		UnicodeToAnsii((S8*)g_calc_cntx->MemValue, (S8*)g_calc_cntx->Operand2Buf);
		memcpy(g_calc_cntx->MemValueBuf, g_calc_cntx->Operand2Buf, sizeof(g_calc_cntx->MemValueBuf));						
		#endif
		g_calc_cntx->MemValuelen = g_calc_cntx->Operand2Len;
		/*dongwg Modify End  For 6500 Ver: TBM780  on 2007-8-29 19:28 */
		OslMfree(result1);
		OslMfree(result2);
		result1 = NULL;
		result2 = NULL;		
	}
}
#else
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
void CalcExecMP(void)
{
	DOUBLE temp_res;

	if(g_calc_cntx->Operand2Buf[0]!=CALC_SYM_NULL)
	{
		temp_res = coolsand_UI_atof((UI_string_type)g_calc_cntx->Operand2Buf);
		
		g_calc_cntx->MemValue += temp_res;
		if((g_calc_cntx->MemValue > CALC_MAX_VALUE) || (g_calc_cntx->MemValue < (-CALC_MAX_VALUE)))
		{
			g_calc_cntx->ResultState = RESULT_EXCEED;
			g_calc_cntx->MemValue = 0;
			playRequestedTone(ERROR_TONE);
		}
	}
}
#else
void CalcExecMP(void)
{
}
#endif
#endif
/*****************************************************************************
* FUNCTION
*	CalcExecMM
* DESCRIPTION
*   Execute M- operator.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void CalcExecMM(void)
{
	BOOL value = TRUE;
	
	CALOPERATOR_STRUCT *result1 = NULL;
	CALOPERATOR_STRUCT *result2 = NULL;
	
	result1 = (CALOPERATOR_STRUCT*)OslMalloc(sizeof(CALOPERATOR_STRUCT));
	result2 = (CALOPERATOR_STRUCT*)OslMalloc(sizeof(CALOPERATOR_STRUCT));
	ASSERT(result1 != NULL);
	ASSERT(result2 != NULL);
	memset(result1->operBuff, 0, OPERATOR_LEN);
	memset(result2->operBuff, 0, OPERATOR_LEN);
	result1->Symbol = OPERATORSYMBOL_POST;
	result1->Exponent = 1;
	result1->Fraction = 0;		
	result2->Symbol = OPERATORSYMBOL_POST;
	result2->Exponent = 1;
	result2->Fraction = 0;
	
	if (g_calc_cntx->Operand2Buf[0] != CALC_SYM_NULL)
	{	
		CalcGetPreOperand(result2, g_calc_cntx->Operand2Buf);
		CalcGetPreOperand(result1, g_calc_cntx->MemValueBuf);
		CalculatorAligDot(result1, result2);	
		CaladdpationEXMM(result1, result2);
		Calresulttoberound(result1);//dongwg move it here 20070919
		value = Judgeifover(result1);
		if ( !value )//judge the value  over
		{
			g_calc_cntx->ResultState = RESULT_EXCEED;
			playRequestedTone(ERROR_TONE);	
		}
		
		//Calresulttoberound(result1);//dongwg del 20070919
		Calgetrealresult(result2, result1);
		/*dongwg Modify Start For 6500 Ver: TBM780  on 2007-8-29 19:28 */
		AnsiiToUnicodeString((S8*)g_calc_cntx->MemValueBuf, (S8*)result2->operBuff);		
		#if 0
		AnsiiToUnicodeString((S8*)g_calc_cntx->Operand2Buf, (S8*)result2->operBuff);
		UnicodeToAnsii((S8*)g_calc_cntx->MemValue, (S8*)g_calc_cntx->Operand2Buf);
		memcpy(g_calc_cntx->MemValueBuf, g_calc_cntx->Operand2Buf, sizeof(g_calc_cntx->MemValueBuf));					
		#endif
		g_calc_cntx->MemValuelen = g_calc_cntx->Operand2Len;
		/*dongwg Modify End  For 6500 Ver: TBM780  on 2007-8-29 19:28 */
		OslMfree(result1);
		OslMfree(result2);
		result1 = NULL;
		result2 = NULL;				
	}
}
#else
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
void CalcExecMM(void)
{
	DOUBLE temp_res;
	
	if(g_calc_cntx->Operand2Buf[0]!=CALC_SYM_NULL)
	{
		temp_res = coolsand_UI_atof((UI_string_type) g_calc_cntx->Operand2Buf);
		
		g_calc_cntx->MemValue -= temp_res ;
		if(g_calc_cntx->MemValue > CALC_MAX_VALUE || g_calc_cntx->MemValue < -CALC_MAX_VALUE)
		{
			g_calc_cntx->ResultState = RESULT_EXCEED;
			g_calc_cntx->MemValue = 0;
			playRequestedTone(ERROR_TONE);
		}
	}
}
#else
void CalcExecMM(void)
{
}
#endif
#endif
#endif

/*****************************************************************************
* FUNCTION
*	CalcGetPreOperand
* DESCRIPTION
*   Convert the string of operand 2 to number
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void CalcGetPreOperand(CALOPERATOR_STRUCT *result, UI_string_type Operand)
{		
	U8 stringCnt = 0;	

	ASSERT(result != NULL);
	
	if ((S8)Operand[0] == '-')
	{		
		stringCnt = 1;
		result->Symbol = OPERATORSYMBOL_NEGA;
	}
	
	coolsand_UI_atof(result,&stringCnt,Operand);		
}
#else
void CalcGetPreOperand(void)
{
	if (g_calc_cntx->Operand2Buf[0] == CALC_SYM_NULL)
		g_calc_cntx->Operand2 = 0;
	else
		g_calc_cntx->Operand2 = coolsand_UI_atof((UI_string_type) g_calc_cntx->Operand2Buf);
}
#endif

/*****************************************************************************
* FUNCTION
*	CalcGetCurrOperand
* DESCRIPTION
*   Convert the string of operand 1 to number
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void CalcGetCurrOperand(void)
{		
}
#else
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
void CalcGetCurrOperand(void)
{	
	if(g_calc_cntx->Operand2Buf[0]==CALC_SYM_NULL)
		g_calc_cntx->Operand1=0;
	else	
	{
		g_calc_cntx->Operand1 = coolsand_UI_atof((UI_string_type) g_calc_cntx->Operand2Buf);
		/* 06/01/11, for bug of displaying "-0" */
		if(g_calc_cntx->Operand1 == -0)
			g_calc_cntx->Operand1 = 0;
	}
}
#else
void CalcGetCurrOperand(void)
{	
}
#endif
#endif
/*****************************************************************************
* FUNCTION
*	CalcExecOperator
* DESCRIPTION
*   Execute current computation operation.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
S8 CalcExecOperator(CALOPERATOR_STRUCT *op1,CALOPERATOR_STRUCT *op2)
{	
	BOOL Returnvalue;
	BOOL value;

	CalcSetRSKClear();
	Returnvalue = TRUE;
	value = TRUE;

	ASSERT(op1 != NULL && op2 != NULL);
	
	switch (g_calc_cntx->OpToExecute)
	{	
		case CALC_OP_PLUS:	
			CaladdpationPlus(op1, op2);
			break;
		
		case CALC_OP_MINUS:
			CaladdpationMinus(op1, op2);		
			break;
		
		case CALC_OP_MULT:				
		{
			CalculatorMult(op1, op2);		
			if (op1->Fraction != 0 && (op1->Exponent + op1->Fraction) > 7)  //make the value be 8 digit added by hansl
			{
				if (op1->Exponent <= 0)
				{
					op1->Fraction = 6;
				}
				/*dongwg Add Start For 6118 Ver: TBM780  on 2007-8-7 19:51 */
				else if(op1->Exponent > 6)
				{
					op1->Fraction = 8 - op1->Exponent;
				}
				/*dongwg Add End  For 6118 Ver: TBM780  on 2007-8-7 19:51 */
				else
					op1->Fraction = 7 - op1->Exponent ;
			}
			break;
		}
		case CALC_OP_DIV:							
		{
			Returnvalue= CalculatorDivi(op1, op2);	
			if ( !Returnvalue )               //商为0  added by  hansl
			{
				g_calc_cntx->ResultState = RESULT_EXCEED;
				playRequestedTone(ERROR_TONE);
				return 0;
			}
			
			if (op1->Fraction != 0 && (op1->Exponent + op1->Fraction) > 7)
			{
				if (op1->Exponent <= 0)
				{
					op1->Fraction = 6;
				}
				/*dongwg Add Start For 6118 Ver: TBM780  on 2007-8-7 19:50 */
				else if(op1->Exponent > 6)
				{
					op1->Fraction = 8 - op1->Exponent;
				}
				/*dongwg Add End  For 6118 Ver: TBM780  on 2007-8-7 19:50 */
				else
					op1->Fraction = 7 - op1->Exponent ;
			}
			
			break;							
		}
		default: 
			return 0;
			break;
	}

	
/*dongwg Del Start For 5939 Ver: TBM780  on 2007-8-16 15:49 */
	#if 0
	value = Judgeifover(op1);
	if ( !value )//judge the value  over
	{
		g_calc_cntx->ResultState = RESULT_EXCEED;
		playRequestedTone(ERROR_TONE);	
	}
	#endif
/*dongwg Del End  For 5939 Ver: TBM780  on 2007-8-16 15:49 */
	return value;
}
#else
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
DOUBLE CalcExecOperator(DOUBLE op1,DOUBLE op2)
{
	DOUBLE  result = 0;

	CalcSetRSKClear();
	
	switch(g_calc_cntx->OpToExecute)
	{	
		case CALC_OP_PLUS:		
			result = op1+op2;	
			break;
		case CALC_OP_MINUS:		
			result = op1-op2;
			break;
		case CALC_OP_MULT:	
			result = op1*op2;
			break;
		case CALC_OP_DIV:	
			if(op2 >0)
				result = (DOUBLE) op1/op2;
			break;							
		default: 
			return 0;
	}

	if(result > CALC_MAX_VALUE || result < -CALC_MAX_VALUE || (result < CALC_MIN_VALUE && result > -CALC_MIN_VALUE))
	{
		g_calc_cntx->ResultState = RESULT_EXCEED;
		playRequestedTone(ERROR_TONE);
		return 0;
	}

	return(result);
}
#else
DOUBLE CalcExecOperator(DOUBLE op1,DOUBLE op2)
{
}
#endif
#endif
/**************************************************************

	FUNCTION NAME		: CalcComputeResult

  	PURPOSE				: 
	
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void CalcComputeResult(void)
{
	
	
	mmi_trace(g_sw_ORGANIZER, "begin-------CalcComputeResult-------end");
	switch(g_calc_cntx->CurrHilite)
	{
#ifndef __MMI_SLIM_CALCULATOR__
		case CALC_OP_MR:		
		{
			CalcExecMR();
			g_calc_cntx->Operator = CALC_OP_NONE;
			g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
			g_calc_cntx->ComputeType = CALC_OP_MR;
			break;			
		}
		case CALC_OP_MC:
		{
			CalcResetMemValue();
			g_calc_cntx->Operator = CALC_OP_NONE;
			g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
			g_calc_cntx->ComputeType = CALC_OP_MC;
			break;			
		}
		case CALC_OP_MP:
		{
			CalcExecMP();			
			g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
			g_calc_cntx->Operator = CALC_OP_NONE;
			g_calc_cntx->ComputeType = CALC_OP_MP;			
			break;
		}
		case CALC_OP_MM:
		{
			CalcExecMM();			
			g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
			g_calc_cntx->Operator = CALC_OP_NONE;
			g_calc_cntx->ComputeType = CALC_OP_MM;			
			break;	
		}
#endif
		default:
		{	
			
/*dongwg Del Start For 6244 Ver: TBM780  on 2007-8-22 19:48 */
			#if 0
			CALOPERATOR_STRUCT *result1 = NULL;
			CALOPERATOR_STRUCT *result2 = NULL;

			result1 = (U8*)OslMalloc(sizeof(CALOPERATOR_STRUCT));
			result2 = (U8*)OslMalloc(sizeof(CALOPERATOR_STRUCT));	
			ASSERT(result1 != NULL);
			ASSERT(result2 != NULL);
			memset(result1->operBuff, 0, OPERATOR_LEN + 1);
			memset(result2->operBuff, 0, OPERATOR_LEN + 1);
			#endif
/*dongwg Del End  For 6244 Ver: TBM780  on 2007-8-22 19:48 */
			/*dongwg Add Start For 6244 Ver: TBM780  on 2007-8-22 19:50 */
			CALOPERATOR_STRUCT result1;
			CALOPERATOR_STRUCT result2;

			memset(result1.operBuff, 0, OPERATOR_LEN + 1);
			memset(result2.operBuff, 0, OPERATOR_LEN + 1);
			
			result1.Symbol = OPERATORSYMBOL_POST;
			result1.Exponent = 1;
			result1.Fraction = 0;		
			result2.Symbol = OPERATORSYMBOL_POST;
			result2.Exponent = 1;
			result2.Fraction = 0;
			
			/*dongwg Add End  For 6244 Ver: TBM780  on 2007-8-22 19:50 */
			if(g_calc_cntx->Operator != CALC_OP_NONE)
			{
				g_calc_cntx->ComputeType = g_calc_cntx->Operator;
				g_calc_cntx->Operator = CALC_OP_EQUAL;
				CalcGetPreOperand(&result2,g_calc_cntx->Operand2Buf);
				//added by hansl  for modify the bug which 1/0.00 = dead  BEGIN
				CalcChangeOperator(&result2);
				//END	
				if (g_calc_cntx->OpToExecute != CALC_OP_NONE)
				{
					CalcGetPreOperand(&result1, g_calc_cntx->Operand1Buf);
					CalcExecOperator(&result1, &result2);	
					/*dongwg Add Start For 5958 Ver: TBM780  on 2007-8-1 13:35 */
					Calresulttoberound(&result1);
					if (!Judgeifover(&result1))
					{
						g_calc_cntx->ResultState = RESULT_EXCEED;
						playRequestedTone(ERROR_TONE);	
					}
					/*dongwg Add End  For 5958 Ver: TBM780  on 2007-8-1 13:35 */
					Calgetrealresult(&result2, &result1);
					/*dongwg Add Start For 6244 Ver: TBM780  on 2007-8-24 9:12 */
					if(g_calc_cntx->ResultState == RESULT_EXCEED)
					{
						memset(result2.operBuff, 0, OPERATOR_LEN + 1);
					}
					/*dongwg Add End  For 6244 Ver: TBM780  on 2007-8-24 9:12 */
					AnsiiToUnicodeString((S8*)g_calc_cntx->Operand2Buf, result2.operBuff);
					UnicodeToAnsii((S8*)g_calc_cntx->Operand1, (S8*)g_calc_cntx->Operand2Buf);
					AnsiiToUnicodeString((S8*)g_calc_cntx->Operand1Buf , (S8*)g_calc_cntx->Operand1);							
					g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
					g_calc_cntx->OpToExecute = CALC_OP_NONE;
				}
			}
			else
			{
				g_calc_cntx->ComputeType = CALC_OP_EQUAL; 
				g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
			}
/*dongwg Del Start For 6244 Ver: TBM780  on 2007-8-22 19:52 */
			#if 0
			OslMfree(result1);
			OslMfree(result2);
			result1 = NULL;
			result2 = NULL;
			#endif
/*dongwg Del End  For 6244 Ver: TBM780  on 2007-8-22 19:52 */
			break;
		}
	}

	g_calc_cntx->ExecFlag = TRUE;
	
#if !defined( __MMI_TOUCH_CALCULATOR__)
	CalcShowKeypad();
#endif
	CalcShowResultScreen();
}
#else
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
void CalcComputeResult(void)
{
	switch(g_calc_cntx->CurrHilite)
	{
#ifndef __MMI_SLIM_CALCULATOR__
		case CALC_OP_MR:		
		{
			CalcExecMR();
			g_calc_cntx->Operator = CALC_OP_NONE;
			g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
			g_calc_cntx->ComputeType = CALC_OP_MR;

			break;
		}
		case CALC_OP_MC:
		{
			CalcResetMemValue();
			g_calc_cntx->Operator = CALC_OP_NONE;
			g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
			g_calc_cntx->ComputeType = CALC_OP_MC;

			break;
		}
		case CALC_OP_MP:
		{
			CalcExecMP();
			
			g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
			g_calc_cntx->Operator = CALC_OP_NONE;
			g_calc_cntx->ComputeType = CALC_OP_MP;
			
			break;
		}
		case CALC_OP_MM:
		{
			CalcExecMM();
			
			g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
			g_calc_cntx->Operator = CALC_OP_NONE;
			g_calc_cntx->ComputeType = CALC_OP_MM;
			
			break;	
		}
#endif
		default:
		{
			if(g_calc_cntx->Operator != CALC_OP_NONE)
			{
				g_calc_cntx->ComputeType = g_calc_cntx->Operator;
				g_calc_cntx->Operator = CALC_OP_EQUAL;
				CalcGetPreOperand();
					
				if((g_calc_cntx->Operand2 == 0) && (g_calc_cntx->OpToExecute == CALC_OP_DIV))
				{
					g_calc_cntx->ResultState = RESULT_EXCEED;
					CalcSetRSKClear();
					g_calc_cntx->Operator=CALC_OP_NONE;
					playRequestedTone(ERROR_TONE);
				} 
				else
				{
					if(g_calc_cntx->OpToExecute!=CALC_OP_NONE)
					{
						g_calc_cntx->Operand1=CalcExecOperator(g_calc_cntx->Operand1,g_calc_cntx->Operand2);
						g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
						g_calc_cntx->OpToExecute=CALC_OP_NONE;
					}
				}
			}
			else
			{
				g_calc_cntx->ComputeType = CALC_OP_EQUAL; 
				g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
			}
			break;
		}
	}

	g_calc_cntx->ExecFlag = TRUE;
#if !defined( __MMI_TOUCH_CALCULATOR__)
	CalcShowKeypad();
#endif
	CalcShowResultScreen();

}
#else
void CalcComputeResult(void)
{
}
#endif
#endif

#define CALC_KEY_HILITE_HANDLER
/*****************************************************************************
* FUNCTION
*	CalcHandleHiliteMemoryButton
* DESCRIPTION
*   Highlight handler of memory button.
*	Move operand to result filed and clear all others.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void CalcHandleHiliteMemoryButton(void)
{
	
	memcpy(g_calc_cntx->Operand2Buf,  g_calc_cntx->Operand1Buf,  sizeof(g_calc_cntx->Operand2Buf));	
	g_calc_cntx->Operand2Len = g_calc_cntx->Operand1Len;	
	g_calc_cntx->Operand1[0] = '\0';
	g_calc_cntx->Operand1Len = 1;
	g_calc_cntx->Operand1Buf[0] = CALC_SYM_NULL;	
	g_calc_cntx->Operator = CALC_OP_NONE;		

}
#else
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
void CalcHandleHiliteMemoryButton(void)
{
	CalcDouble2String(g_calc_cntx->Operand1, (S8*)g_calc_cntx->Operand2Buf);

	g_calc_cntx->Operator = CALC_OP_NONE;
	
	if(g_calc_cntx->Operand1!=0)
		g_calc_cntx->Operand2Len = pfnUnicodeStrlen((S8*) g_calc_cntx->Operand2Buf) + 1;
	else
		g_calc_cntx->Operand2Len = CALC_ZEOR_LEN;

}
#else
void CalcHandleHiliteMemoryButton(void)
{
}
#endif
#endif
/*****************************************************************************
* FUNCTION
*	CalcHandleHiliteOperator
* DESCRIPTION
*   Execute correspoding operation when highlighting on opeartor.
*	Dislpay operator, calculator reslut, and add new operand.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void CalcHandleHiliteOperator(void)
{
	U8	PreOperator;
	U8    i;
	
	CALOPERATOR_STRUCT *result1 = NULL;
	CALOPERATOR_STRUCT *result2 = NULL;
	result1 = (CALOPERATOR_STRUCT*)OslMalloc(sizeof(CALOPERATOR_STRUCT));
	result2 = (CALOPERATOR_STRUCT*)OslMalloc(sizeof(CALOPERATOR_STRUCT));
	ASSERT(result1 != NULL);
	ASSERT(result2 != NULL);
	memset(result1->operBuff, 0, OPERATOR_LEN + 1);
	memset(result2->operBuff, 0, OPERATOR_LEN + 1);
	result1->Symbol = OPERATORSYMBOL_POST;
	result1->Exponent = 1;
	result1->Fraction = 0;		
	result2->Symbol = OPERATORSYMBOL_POST;
	result2->Exponent = 1;
	result2->Fraction = 0;	
	
	PreOperator = g_calc_cntx->Operator;
	g_calc_cntx->ExecFlag = FALSE;	
	g_calc_cntx->Operator = g_calc_cntx->CurrHilite;	
//start 
#ifdef __MMI_TOUCH_CALCULATOR__
	if ( !(g_calc_cntx->Operator == CALC_OP_MP || g_calc_cntx->Operator == CALC_OP_MM || 
		g_calc_cntx->Operator == CALC_OP_MR || g_calc_cntx->Operator == CALC_OP_MC) )
	g_calc_cntx->RSKState = CALC_RSK_CLEAR;
#else
	g_calc_cntx->RSKState = CALC_RSK_CLEAR;
#endif
//end 
	switch(PreOperator)
	{
		case CALC_OP_PLUS:
		case CALC_OP_MINUS:
		case CALC_OP_MULT:
		case CALC_OP_DIV:
		/*dongwg Add Start For 5959 Ver: TBM780  on 2007-9-5 14:18 */
		case CALC_OP_EQUAL:
		/*dongwg Add End  For 5959 Ver: TBM780  on 2007-9-5 14:18 */
			{
				
				if (g_calc_cntx->Operand2Buf[0] != CALC_SYM_NULL)
				{					
					
					g_calc_cntx->OpToExecute = PreOperator;										
					CalcGetPreOperand(result1, g_calc_cntx->Operand1Buf);						
					CalcGetPreOperand(result2, g_calc_cntx->Operand2Buf);					
					CalcChangeOperator(result2);
				
					
					CalcExecOperator(result1, result2);										
					Calresulttoberound(result1);	
					/*dongwg Add Start For 6120 Ver: TBM780  on 2007-8-7 11:44 */
					if (!Judgeifover(result1))
					{
						g_calc_cntx->ResultState = RESULT_EXCEED;
						playRequestedTone(ERROR_TONE);	
					}
					/*dongwg Add End  For 6120 Ver: TBM780  on 2007-8-7 11:44 */
					Calgetrealresult(result2, result1);	

					/*dongwg Add Start For 5959 Ver: TBM780  on 2007-8-28 18:0 */
					for(i = 0;i < 17; i++)
					{
						if(result2->operBuff[i] == CALC_SYM_NULL)
						{
							break;
						}
					}
					/*dongwg Add Start For 6700 Ver: TBM780  on 2007-9-18 10:54 */
					if(result2->operBuff[0] == CALC_SYM_ZERO && g_calc_cntx->Operand2Len <=2)
					{
						i = 0;
					}
					/*dongwg Add End  For 6700 Ver: TBM780  on 2007-9-18 10:54 */
					g_calc_cntx->Operand2Len = i+1;
					/*dongwg Add End  For 5959 Ver: TBM780  on 2007-8-28 18:0 */
					
					AnsiiToUnicodeString((S8*)g_calc_cntx->Operand2Buf , result2->operBuff);
					UnicodeToAnsii((S8*)g_calc_cntx->Operand1, (S8*)g_calc_cntx->Operand2Buf);
					/*make comment  the follwing four sentences and before exiting function to have a public memory free by liuxl on 20090917*/
					/*
					OslMfree(result1);
					OslMfree(result2);
					result1 = NULL;
					result2 = NULL;
					*/
					g_calc_cntx->OpToExecute = g_calc_cntx->Operator;										
					break;
				}
				else
				{
					g_calc_cntx->OpToExecute = g_calc_cntx->Operator;
					/*add to avoid memoy leak by liuxl on 20090917 */
					OslMfree(result1);
					OslMfree(result2);
					result1 = NULL;
					result2 = NULL;
					return;
				}
			}
#ifndef __MMI_SLIM_CALCULATOR__		
		case CALC_OP_MP:
		case CALC_OP_MM:
		case CALC_OP_MR:
		case CALC_OP_MC:
#endif
		case CALC_OP_NONE:
			{				
				if (g_calc_cntx->Operand2Buf[0] == CALC_SYM_NULL)
					g_calc_cntx->Operand1[0] = '\0';
				else	
				{													
					
					CalcGetPreOperand(result2, g_calc_cntx->Operand2Buf);										
					CalcChangeOperator(result2);					
					Calgetrealresult(result1, result2);
					AnsiiToUnicodeString((S8*)g_calc_cntx->Operand2Buf, (S8*)result1->operBuff);
					UnicodeToAnsii((S8*)g_calc_cntx->Operand1, (S8*)g_calc_cntx->Operand2Buf);
					/*make comment  the follwing four sentences and before exiting function to have a public memory free by liuxl on 20090917*/
					/*
					OslMfree(result1);
					OslMfree(result2);
					result1 = NULL;
					result2 = NULL;														
					*/
				}
			}
		default:
			break;
	}
	/*add to avoid memoy leak by liuxl on 20090917 */
	OslMfree(result1);
	OslMfree(result2);
	result1 = NULL;
	result2 = NULL;
	
	g_calc_cntx->OpToExecute = g_calc_cntx->Operator;
	memcpy(g_calc_cntx->Operand1Buf,g_calc_cntx->Operand2Buf,sizeof(g_calc_cntx->Operand1Buf));
	g_calc_cntx->Operand1Len = g_calc_cntx->Operand2Len;
	

	g_calc_cntx->Operand2[0]='\0';
	g_calc_cntx->Operand2Len=1;
	g_calc_cntx->Operand2Buf[0]=CALC_SYM_NULL;
}
#else
void CalcHandleHiliteOperator(void)
{
	U8	PreOperator = g_calc_cntx->Operator;

	g_calc_cntx->ExecFlag = FALSE;
	
	g_calc_cntx->Operator = g_calc_cntx->CurrHilite;
//start 
#ifdef __MMI_TOUCH_CALCULATOR__
	if( !(g_calc_cntx->Operator == CALC_OP_MP || g_calc_cntx->Operator == CALC_OP_MM || 
		g_calc_cntx->Operator == CALC_OP_MR || g_calc_cntx->Operator == CALC_OP_MC) )
	g_calc_cntx->RSKState = CALC_RSK_CLEAR;
#else
	g_calc_cntx->RSKState = CALC_RSK_CLEAR;
#endif
//end 
	switch(PreOperator)
	{
		case CALC_OP_PLUS:
		case CALC_OP_MINUS:
		case CALC_OP_MULT:
		case CALC_OP_DIV:
			if(g_calc_cntx->Operand2Buf[0] != CALC_SYM_NULL)
			{
				g_calc_cntx->OpToExecute = PreOperator;
				CalcGetPreOperand();
				g_calc_cntx->Operand1=CalcExecOperator(g_calc_cntx->Operand1,g_calc_cntx->Operand2);
				g_calc_cntx->OpToExecute = g_calc_cntx->Operator;
				break;
			}
			else
			{
				g_calc_cntx->OpToExecute = g_calc_cntx->Operator;
				return;
			}
#ifndef __MMI_SLIM_CALCULATOR__		
		case CALC_OP_MP:
		case CALC_OP_MM:
		case CALC_OP_MR:
		case CALC_OP_MC:
#endif
		case CALC_OP_NONE:
			CalcGetCurrOperand();
		default:
			break;
	}

	g_calc_cntx->OpToExecute = g_calc_cntx->Operator;
	memcpy(g_calc_cntx->Operand1Buf,g_calc_cntx->Operand2Buf,sizeof(g_calc_cntx->Operand1Buf));
	g_calc_cntx->Operand1Len = g_calc_cntx->Operand2Len;

	g_calc_cntx->Operand2=0;
	g_calc_cntx->Operand2Len=1;
	g_calc_cntx->Operand2Buf[0]=CALC_SYM_NULL;
}
#endif

/*****************************************************************************
* FUNCTION
*	CalcHandleLeftKey
* DESCRIPTION
*   Left arrow key handler.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcHandleLeftKey(void)
{
	mmi_trace(g_sw_ORGANIZER, "begin-------CalcHandleLeftKey-------end");
	if(g_calc_cntx->ResultState  == RESULT_NOT_EXCEED)
	{
		switch(g_calc_cntx->CurrHilite)
		{	
#ifdef __MMI_SLIM_CALCULATOR__
			case CALC_OP_MULT:
				return;				
#else 
			case CALC_OP_MC:		
				return;
			case CALC_OP_MR:		
			{
				g_calc_cntx->CurrHilite=CALC_OP_DIV;
				CalcHandleHiliteOperator();
				break;
			}
			case CALC_OP_MULT:		
			{
				g_calc_cntx->CurrHilite=CALC_OP_MC;
				CalcHandleHiliteMemoryButton();
				break;
			}
			case CALC_OP_MP:		
			case CALC_OP_MM:
#endif // end of ifdef __MMI_SLIM_CALCULATOR__ 
			case CALC_OP_PLUS:
			case CALC_OP_MINUS	:	
			case CALC_OP_EQUAL:
			case CALC_OP_DIV:		
			case CALC_OP_NONE://add by hansl 061108
			{
				g_calc_cntx->CurrHilite=CALC_OP_MULT; 
				CalcHandleHiliteOperator();
				break;									
			}
			default:
			{
				g_calc_cntx->CurrHilite=CALC_OP_EQUAL;
				break;
			}
		}

#if !defined( __MMI_TOUCH_CALCULATOR__)//dongwg add 2007/07/17
		CalcShowKeypad();
#endif
		CalcShowResultScreen();
	}
}


/*****************************************************************************
* FUNCTION
*	CalcHandleRightKey
* DESCRIPTION
*   Right arrow key handler.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcHandleRightKey(void)
{	
	if(g_calc_cntx->ResultState  == RESULT_NOT_EXCEED)
	{
		switch(g_calc_cntx->CurrHilite)
		{
#ifdef __MMI_SLIM_CALCULATOR__
			case CALC_OP_DIV:
				return;
#else
			case CALC_OP_MR:			
				return;
			case CALC_OP_MC:
			{
				g_calc_cntx->CurrHilite=CALC_OP_MULT;
				CalcHandleHiliteOperator();
				break;
			}
			case CALC_OP_DIV:
			{
				g_calc_cntx->CurrHilite=CALC_OP_MR; 
				CalcHandleHiliteMemoryButton();
				break;
			}
			case CALC_OP_MP:	
			case CALC_OP_MM:
#endif // end of ifdef __MMI_SLIM_CALCULATOR__ 
			case CALC_OP_PLUS:	
			case CALC_OP_MINUS:	
			case CALC_OP_MULT:
			case CALC_OP_EQUAL:
			case CALC_OP_NONE://add by hansl 061108
			{
				g_calc_cntx->CurrHilite=CALC_OP_DIV;
				CalcHandleHiliteOperator();
				break;
			}
			default:
			{
				g_calc_cntx->CurrHilite=CALC_OP_EQUAL;
				break;
			}
		}		

#if !defined( __MMI_TOUCH_CALCULATOR__)//dongwg add 2007/07/17
			CalcShowKeypad();
#endif		
			CalcShowResultScreen();
	}
}


/*****************************************************************************
* FUNCTION
*	CalcHandleUpKey
* DESCRIPTION
*   Up arrow key handler.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcHandleUpKey(void)
{
	mmi_trace(g_sw_ORGANIZER, "begin-------CalcHandleUpKey-------end");
	if(g_calc_cntx->ResultState  == RESULT_NOT_EXCEED)
	{
		switch(g_calc_cntx->CurrHilite)
		{
#ifdef __MMI_SLIM_CALCULATOR__
			case CALC_OP_PLUS:
				return;
#else
			case CALC_OP_MP:
				return;
			case CALC_OP_MM:
			{
				g_calc_cntx->CurrHilite=CALC_OP_MINUS;
				CalcHandleHiliteOperator();
				break;
			}
			case CALC_OP_PLUS:
			{
				g_calc_cntx->CurrHilite=CALC_OP_MP;
				CalcHandleHiliteMemoryButton();
				break;
			}
			case CALC_OP_MR:			
			case CALC_OP_MC:
#endif // end of ifdef __MMI_SLIM_CALCULATOR__ 
			case CALC_OP_DIV:
			case CALC_OP_MULT:
			case CALC_OP_EQUAL:
			case CALC_OP_MINUS:
			case CALC_OP_NONE://add by hansl 061108
			{
				g_calc_cntx->CurrHilite=CALC_OP_PLUS;
				CalcHandleHiliteOperator();		
				break;
			}
			default:
			{
				g_calc_cntx->CurrHilite=CALC_OP_PLUS;
				break;
			}
		}	

#if !defined( __MMI_TOUCH_CALCULATOR__)//dongwg add 2007/07/17
		CalcShowKeypad();
#endif
		CalcShowResultScreen();
	}
}


/*****************************************************************************
* FUNCTION
*	CalcHandleDownKey
* DESCRIPTION
*   Down arrow key handler.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcHandleDownKey(void)
{
	mmi_trace(g_sw_ORGANIZER, "begin-------CalcHandleDownKey-------end");
	if(g_calc_cntx->ResultState == RESULT_NOT_EXCEED)
	{
		switch(g_calc_cntx->CurrHilite)
		{
#ifdef __MMI_SLIM_CALCULATOR__
			case CALC_OP_MINUS:
				return;
#else
			case CALC_OP_MM:
				return;		
			case CALC_OP_MP:
			{
				g_calc_cntx->CurrHilite = CALC_OP_PLUS;
				CalcHandleHiliteOperator();
				break;
			}
			case CALC_OP_MINUS:
			{
				g_calc_cntx->CurrHilite = CALC_OP_MM; 
				CalcHandleHiliteMemoryButton();
				break;
			}
			case CALC_OP_MR:		
			case CALC_OP_MC:
#endif 
			case CALC_OP_DIV:
			case CALC_OP_MULT:
			case CALC_OP_EQUAL:
			case CALC_OP_PLUS:
			case CALC_OP_NONE://add by hansl 061108
			{
				g_calc_cntx->CurrHilite = CALC_OP_MINUS;
				CalcHandleHiliteOperator();
				break;
			}
			default:
			{
				g_calc_cntx->CurrHilite=CALC_OP_EQUAL;
				break;
			}
		}

#if !defined( __MMI_TOUCH_CALCULATOR__)//dongwg add 2007/07/17
			CalcShowKeypad();
#endif
			CalcShowResultScreen();
	}

}

/*****************************************************************************
* FUNCTION
*	CalWin32ArrowKeyHandler
* DESCRIPTION
*   Handle number key input
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcHandleNumKey(S32 key_code)
{
	if(g_calc_cntx->ResultState  == RESULT_NOT_EXCEED)
	{
		if(g_calc_cntx->ExecFlag)
			CalcResetResultValue();

		if(g_calc_cntx->CurrHilite < CALC_OP_MP) /* operators */
		{
			g_calc_cntx->CurrHilite = CALC_OP_EQUAL;
#if !defined( __MMI_TOUCH_CALCULATOR__ )			
			CalcShowKeypad();
#endif
		}

		CalcInsertChar((UI_character_type)('0'+key_code));	
	}
	else
		playRequestedTone(ERROR_TONE);
}


/*****************************************************************************
* FUNCTION
*	CalcHandlePoundKey
* DESCRIPTION
*   Pound key handler.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcHandlePoundKey(void)
{
	if(g_calc_cntx->ExecFlag)
		CalcResetResultValue();

	if(g_calc_cntx->Operand2Len == 1)
	{
		CalcInsertChar(CALC_SYM_DOT);
	}
	else if(!CalcSearchDecimal())
	{
		CalcInsertChar(CALC_SYM_DOT);
	}
}


/*****************************************************************************
* FUNCTION
*	CalcHandleEndKey
* DESCRIPTION
*   End key handler of calculator
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcHandleEndKey(void)
{
	CalcDeInit();
	DisplayIdleScreen();
}

/*****************************************************************************
* FUNCTION
*	CalcHandleClearKey
* DESCRIPTION
*   Clear key handler of calculator
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void CalcHandleClearKey(void)
{	
	if (g_calc_cntx->ResultState == RESULT_EXCEED)
	{
		CalcResetContext();
	}
	else if (g_calc_cntx->Operator != CALC_OP_NONE)
	{
		if (g_calc_cntx->Operator != CALC_OP_EQUAL)
		{
			if (g_calc_cntx->Operand2Len > CALC_ZEOR_LEN)
			{
				g_calc_cntx->Operand2Len--; 
				/* '0.' || '-x' */
				/*dongwg Modify For 6226 Ver: TBM780  on 2007-8-17 10:29 change '==' to '<='*/
				if ((g_calc_cntx->Operand2Len <= CALC_ZEOR_DOT_LEN && (g_calc_cntx->Operand2Buf[1]== CALC_SYM_DOT && g_calc_cntx->Operand2Buf[0]== CALC_SYM_ZERO)) 
					|| (g_calc_cntx->Operand2Buf[g_calc_cntx->Operand2Len-2] == CALC_SYM_MINUS))
				{
					g_calc_cntx->Operand2Len = CALC_ZEOR_LEN;
				}
				g_calc_cntx->Operand2Buf[g_calc_cntx->Operand2Len-1]=CALC_SYM_NULL;
				
			}
			else
			{
				g_calc_cntx->Operator = CALC_OP_NONE;
				g_calc_cntx->CurrHilite = CALC_OP_EQUAL;

				if (g_calc_cntx->Operand1[0] == '0')
				{
					CalcResetResultValue();
				}
				else
				{
#ifdef _MMI_OPERATOR_	//add by hansl 061108
					memcpy(g_calc_cntx->Operand2Buf,  g_calc_cntx->Operand1Buf,  sizeof(g_calc_cntx->Operand2Buf));	
					g_calc_cntx->Operand2Len = g_calc_cntx->Operand1Len;	
					g_calc_cntx->Operand1[0] = '\0';
					g_calc_cntx->Operand1Len = 1;
					g_calc_cntx->Operand1Buf[0] = CALC_SYM_NULL;	
					g_calc_cntx->Operator = CALC_OP_NONE;
#else
					CalcDouble2String((S8*)g_calc_cntx->Operand1, (S8*)g_calc_cntx->Operand2Buf);
					g_calc_cntx->Operand2Len = pfnUnicodeStrlen((S8*) g_calc_cntx->Operand2Buf) + 1;
#endif
				}
			}
		} 
		else	/* display result after calculator, operator == equal */
		{
			CalcResetResultValue();	
		}
	}
	else
	{
		if (g_calc_cntx->ComputeType >= CALC_OP_EQUAL && g_calc_cntx->ComputeType < CALC_OP_NONE) /* Move to Mx and press LSK */
			CalcResetResultValue();
		else /* Move to Mx */
		{
			g_calc_cntx->Operand2Len--;

			/* current operand == "0." || "-x" */
			/*dongwg Modify Start For 6226 Ver: TBM780  on 2007-8-17 10:28 change '==' to '<='*/
			if (((g_calc_cntx->Operand2Len <= CALC_ZEOR_DOT_LEN) &&
			((g_calc_cntx->Operand2Buf[1]== CALC_SYM_DOT) && 
			(g_calc_cntx->Operand2Buf[0]== CALC_SYM_ZERO )))|| 
			(g_calc_cntx->Operand2Buf[g_calc_cntx->Operand2Len-2] == CALC_SYM_MINUS))
			{
				g_calc_cntx->Operand2Len = CALC_ZEOR_LEN;
			}
			else 
			{
				g_calc_cntx->Operand2Buf[g_calc_cntx->Operand2Len-1]=CALC_SYM_NULL;
			}

			if (g_calc_cntx->Operand2Len<=1) 
				CalcResetResultValue();
		}
		
	}
//start 
#if !defined( __MMI_TOUCH_CALCULATOR__ )
	CalcShowKeypad();
#endif
//end 
	CalcShowResultScreen();
}

#else
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
void CalcHandleClearKey(void)
{	
	if(g_calc_cntx->ResultState == RESULT_EXCEED)
	{
		CalcResetContext();
	}
	else if(g_calc_cntx->Operator != CALC_OP_NONE)
	{
		if(g_calc_cntx->Operator != CALC_OP_EQUAL)
		{
			if(g_calc_cntx->Operand2Len > CALC_ZEOR_LEN)
			{
				g_calc_cntx->Operand2Len--; 
				/* '0.' || '-x' */
				if((g_calc_cntx->Operand2Len == CALC_ZEOR_DOT_LEN && (g_calc_cntx->Operand2Buf[1]== CALC_SYM_DOT && g_calc_cntx->Operand2Buf[0]== CALC_SYM_ZERO)) 
					|| (g_calc_cntx->Operand2Buf[g_calc_cntx->Operand2Len-2] == CALC_SYM_MINUS))
				{
					g_calc_cntx->Operand2Len = CALC_ZEOR_LEN;
				}
				g_calc_cntx->Operand2Buf[g_calc_cntx->Operand2Len-1]='\0';
				
			}
			else
			{
				g_calc_cntx->Operator = CALC_OP_NONE;
				g_calc_cntx->CurrHilite = CALC_OP_EQUAL;

				if(g_calc_cntx->Operand1 == 0)
				{
					CalcResetResultValue();
				}
				else
				{
					CalcDouble2String(g_calc_cntx->Operand1, (S8*)g_calc_cntx->Operand2Buf);
					g_calc_cntx->Operand2Len = pfnUnicodeStrlen((S8*) g_calc_cntx->Operand2Buf) + 1;
				}
			}
		} 
		else	/* display result after calculator, operator == equal */
		{
			CalcResetResultValue();	
		}
	}
	else
	{
		if(g_calc_cntx->ComputeType >= CALC_OP_EQUAL && g_calc_cntx->ComputeType < CALC_OP_NONE) /* Move to Mx and press LSK */
			CalcResetResultValue();
		else /* Move to Mx */
		{
			g_calc_cntx->Operand2Len--;

			/* current operand == "0." || "-x" */
			if((g_calc_cntx->Operand2Len == CALC_ZEOR_DOT_LEN) &&
				((g_calc_cntx->Operand2Buf[1]== CALC_SYM_DOT) && (g_calc_cntx->Operand2Buf[0]== CALC_SYM_ZERO ))
				|| (g_calc_cntx->Operand2Buf[g_calc_cntx->Operand2Len-2] == CALC_SYM_MINUS))
			
			{
				g_calc_cntx->Operand2Len = CALC_ZEOR_LEN;
			}
			else 
			{
				g_calc_cntx->Operand2Buf[g_calc_cntx->Operand2Len-1]='\0';
			}

			if(g_calc_cntx->Operand2Len<=1) 
				CalcResetResultValue();
		}
		
	}
//start 
#if !defined( __MMI_TOUCH_CALCULATOR__ )
	CalcShowKeypad();
#endif
//end 
	CalcShowResultScreen();
}
#else
void CalcHandleClearKey(void)
{
}
#endif
#endif
//add by liuxn 060616 end

/*****************************************************************************
* FUNCTION
*	CalcHandleRSKLongPress
* DESCRIPTION
*   Reset calculator to defualt state 
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcHandleRSKLongPress(void)
{
	g_calc_cntx->IsRSKPress = TRUE;
	CalcResetResultValue();
#ifndef __MMI_TOUCH_CALCULATOR__	
	CalcShowKeypad();
#endif
	CalcShowResultScreen();
}

#ifdef MMI_ON_WIN32
/*****************************************************************************
* FUNCTION
*	CalWin32NumKeyHandler
* DESCRIPTION
*   Key handler on Win 32
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalWin32NumKeyHandler(S32 keyc)
{
	if(keyc>='0' && keyc<='9')
		CalcInsertChar((UI_character_type)keyc);
	else if(keyc==0x08)
		CalcHandleClearKey();
	else if(keyc==0x1b);	/*	Esc	*/
	else if(keyc==0x0d);	/*	Tab	*/
}


/*****************************************************************************
* FUNCTION
*	CalWin32ArrowKeyHandler
* DESCRIPTION
*   Key handler on Win 32
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalWin32ArrowKeyHandler(S32 vkey_code,S32 key_state)
{	
#if(MMI_BUILD_TYPE==BUILD_TYPE_X86WIN32)
	if(key_state)
	{	
		switch(vkey_code)
		{
			case 37:	
				CalcHandleLeftKey();
				break;
			case 39:
				CalcHandleRightKey();
				break;
			case 38:
				CalcHandleUpKey();
				break;
			case 40:
				CalcHandleDownKey();
				break;
		}
	}
#else
	UI_UNUSED_PARAMETER(vkey_code);
	UI_UNUSED_PARAMETER(key_state);
#endif
}
#endif


/*****************************************************************************
* FUNCTION
*	CalcRegisterKeyHandler
* DESCRIPTION
*   Register key handler of calculator 
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CalcRegisterKeyHandler(void)
{

//#ifndef __MMI_TOUCH_CALCULATOR__//dongwg del 2007/07/17
//modify by lin for cs368 @20070829
//#ifdef __MMI_TOUCH_CALCULATOR__//dongwg add 2007/07/17
	SetKeyHandler(CalcHandleLeftKey,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(CalcHandleRightKey,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(CalcHandleUpKey,KEY_UP_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(CalcHandleDownKey,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(CalcHandlePoundKey,KEY_POUND,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(CalcComputeResult,KEY_EVENT_UP);
	SetKeyHandler(CalcHandleEndKey,KEY_END,KEY_EVENT_DOWN);
//#endif modify by lin for cs368 @20070829
/*dongwg Del Start For 6244 Ver: TBM780  on 2007-8-23 9:34 */
	#if 0
 
	SetLeftSoftkeyFunction(CalcComputeResult,KEY_EVENT_UP);	
	//end 
	#endif
/*dongwg Del End  For 6244 Ver: TBM780  on 2007-8-23 9:34 */
	if(g_calc_cntx->RSKState == CALC_RSK_BACK)
		CalcSetRSKBack();
	else
		CalcSetRSKClear();

#ifdef MMI_ON_WIN32
	register_keyboard_key_handler(CalWin32ArrowKeyHandler);
	register_keyboard_input_handler(CalWin32NumKeyHandler);
#endif

//#ifndef __MMI_TOUCH_CALCULATOR__//dongwg del 2007/07/17
//modify by lin for cs368 @20070829
//#ifdef __MMI_TOUCH_CALCULATOR__//dongwg add 2007/07/17
#ifdef __MMI_MAINLCD_160X128__
	register_MMI_key_number_input_handler();
#else	
	register_MMI_key_input_handler();
#endif
	register_key_down_handler(CalcHandleNumKey);
//#endif
//modify by lin for cs368 @20070829
}

/*****************************************************************************
* FUNCTION
*	ExitCalcApp
* DESCRIPTION
*   Exit function of calculator application 
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitCalcApp(void)
{
	MMI_disable_title_shortcut_display = 0;
}

/*****************************************************************************
* FUNCTION
*	EntryCalcApp
* DESCRIPTION
*   Entry function of calculator application 
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryCalcApp(void)
{	
	TBM_ENTRY(0x28D7);
	EntryNewScreen(SCR_ID_CALC_APP, ExitCalcApp, EntryCalcApp, NULL);
#ifndef  __MMI_TOUCH_CALCULATOR__
	/*cong.li added for product QPAD[220*176] on 2009.08.11. */
	#if	!defined (__MMI_MAINLCD_176X220__) && !defined (__MMI_MAINLCD_240X320__) && !defined(__MMI_MAINLCD_220X176__)
	entry_full_screen();
	#endif
#else 
	#if defined(__MMI_UI_STYLE_4__)
		//reset_title_status_icon(); 
		#ifdef __MMI_WALLPAPER_ON_BOTTOM__
		wgui_set_wallpaper_on_bottom(MMI_TRUE);
		 
		#ifdef __MMI_DRAW_MANAGER__
			dm_set_scr_bg_redraw(MMI_TRUE);
		#endif
		 
		#endif
	#endif
#endif
	CalcRedraw();
	CalcRegisterKeyHandler();
#ifdef __MMI_TOUCH_CALCULATOR__
	mmi_pen_register_down_handler(mmi_touchcalc_pen_down_hdlr);
	mmi_pen_register_up_handler(mmi_touchcalc_pen_up_hdlr);
	mmi_pen_register_move_handler(mmi_touchcalc_pen_move_hdlr);
	mmi_pen_register_repeat_handler(mmi_touchcalc_pen_repeat_hdlr);
	mmi_pen_register_long_tap_handler(mmi_touchcalc_pen_long_tap_hdlr);
#endif
	
	RedrawCategoryFunction=CalcRedraw;
	TBM_EXIT(0x28D7);
}


/*****************************************************************************
* FUNCTION
*	CalcPreEntryApp
* DESCRIPTION
*   Initialize data before entry calculator.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/

void CalcPreEntryApp(void)
{
	S32 i;
	if(g_calc_cntx == NULL)
		g_calc_cntx = OslMalloc(sizeof(calc_context_struct));

	memset(g_calc_cntx, 0, sizeof(calc_context_struct));

	/* initialize image */
#ifdef __MMI_SLIM_CALCULATOR__
	for(i=CALC_OP_PLUS; i<CALC_OP_MP; i++)		
#else		
	for(i=CALC_OP_PLUS; i<CALC_OP_NONE; i++)
#endif
	{
		g_calc_cntx->OperatorImg[i].x = gCalcImgCoor[i][0];
		g_calc_cntx->OperatorImg[i].y = gCalcImgCoor[i][1];
		g_calc_cntx->OperatorImg[i].image = (UI_image_type)GetImage((U16)(IMG_ID_CALC_PLUS+i));
	}

	/* initialize keys */
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);	
	CalcResetContext();
	EntryCalcApp();
}



#ifdef __MMI_TOUCH_CALCULATOR__
void mmi_touchcalc_pen_down_hdlr(mmi_pen_point_struct point )
{
	MMI_BOOL ret = MMI_FALSE;
	mmi_gui_calc_pen_enum calc_menu_event;
	mmi_gui_button_pen_enum button_event;

	ret = translate_softkey_pen_event(MMI_LEFT_SOFTKEY, MMI_PEN_EVENT_DOWN, 
	point.x, point.y, &button_event);
	if (ret)
	{
		calc_pen_on_object = TOUCH_CALC_PEN_LSK;
		if (button_event == GUI_BUTTON_PEN_DOWN)
		{
			execute_softkey_function(KEY_EVENT_DOWN, MMI_LEFT_SOFTKEY);
		}
	}
	ret = translate_softkey_pen_event(MMI_RIGHT_SOFTKEY, MMI_PEN_EVENT_DOWN, 
				point.x, point.y, &button_event);
	if (ret)
	{
		calc_pen_on_object = TOUCH_CALC_PEN_RSK;
		if (button_event == GUI_BUTTON_PEN_DOWN)
		{
			execute_softkey_function(KEY_EVENT_DOWN,MMI_RIGHT_SOFTKEY);
		}
	}
	ret = coolsand_UI_calculator_translate_pen_event(&g_calculator_keypad,point.x,point.y,&calc_menu_event, MMI_PEN_EVENT_DOWN);
	if(ret)
	{
		calc_pen_on_object = TOUCH_CALC_PEN_NUM_KEY;
		if(calc_menu_event == GUI_CALC_PEN_HIGHLIGHT_CHANGED)
		{
			coolsand_UI_BLT_double_buffer(TOUCH_CALC_SCREEN_X,TOUCH_CALC_SCREEN_Y,UI_device_width - 1,TOUCH_CALC_SCREEN_Y+TOUCH_CALC_SCREEN_HEIGHT-1 );
		}
	}
}
void mmi_touchcalc_pen_up_hdlr(mmi_pen_point_struct point )
{
	mmi_gui_button_pen_enum button_event;
	mmi_gui_calc_pen_enum calc_menu_event;

	switch(calc_pen_on_object)
	{
	case TOUCH_CALC_PEN_NUM_KEY:
		coolsand_UI_calculator_translate_pen_event(&g_calculator_keypad,point.x,point.y,&calc_menu_event, MMI_PEN_EVENT_UP);
		if( calc_menu_event==GUI_CALC_PEN_HIGHLIGHT_CHANGED)
		{
			coolsand_UI_BLT_double_buffer(TOUCH_CALC_SCREEN_X,TOUCH_CALC_SCREEN_Y,UI_device_width - 1,TOUCH_CALC_SCREEN_Y+TOUCH_CALC_SCREEN_HEIGHT-1 );
		}
		break;
	case TOUCH_CALC_PEN_LSK:
		translate_softkey_pen_event(MMI_LEFT_SOFTKEY, MMI_PEN_EVENT_UP, 
				point.x, point.y, &button_event);
		if (button_event == GUI_BUTTON_PEN_UP_INSIDE)
		{
			execute_softkey_function(KEY_EVENT_UP,MMI_LEFT_SOFTKEY);
		}
		break;
	case TOUCH_CALC_PEN_RSK:
		translate_softkey_pen_event(MMI_RIGHT_SOFTKEY, MMI_PEN_EVENT_UP, 
				point.x, point.y, &button_event);
		if (button_event == GUI_BUTTON_PEN_UP_INSIDE)
		{
			execute_softkey_function(KEY_EVENT_UP,MMI_RIGHT_SOFTKEY);
		}
        break;
    default:
        break;
	}
	calc_pen_on_object = TOUCH_CALC_PEN_NONE;
}

void mmi_touchcalc_pen_move_hdlr(mmi_pen_point_struct point )
{
	S16	x1,x2,y1,y2;
	mmi_gui_calc_pen_enum  calc_menu_event;
	mmi_gui_button_pen_enum button_event;

	switch(calc_pen_on_object)
	{
	case TOUCH_CALC_PEN_NUM_KEY:
		coolsand_UI_calculator_translate_pen_event(&g_calculator_keypad, point.x, point.y, &calc_menu_event,MMI_PEN_EVENT_MOVE);
		if (calc_menu_event == GUI_CALC_PEN_HIGHLIGHT_CHANGED )
		{
			TOUCH_CALC_SELECT_KEY = FALSE; 
			x1 = g_calculator_keypad.selected_key_x;
			y1 = g_calculator_keypad.selected_key_y;
			x2 = x1+g_calculator_keypad.key_width;
			y2 = y1+g_calculator_keypad.key_height;
			gdi_layer_push_clip();
			gdi_layer_set_clip(x1, y1,x2, y2);
			gdi_image_cache_bmp_draw(x1, y1, &g_calculator_keypad.selected_key_bitmap);
			gdi_layer_pop_clip();
			coolsand_UI_BLT_double_buffer(TOUCH_CALC_SCREEN_X,TOUCH_CALC_SCREEN_Y,UI_device_width - 1,TOUCH_CALC_SCREEN_Y+TOUCH_CALC_SCREEN_HEIGHT-1 );
		}
		coolsand_UI_BLT_double_buffer(TOUCH_CALC_SCREEN_X,TOUCH_CALC_SCREEN_Y,UI_device_width - 1,TOUCH_CALC_SCREEN_Y+TOUCH_CALC_SCREEN_HEIGHT-1 );
	break;
	case TOUCH_CALC_PEN_LSK:
		translate_softkey_pen_event(MMI_LEFT_SOFTKEY, MMI_PEN_EVENT_MOVE, 
				point.x, point.y, &button_event);
		break;
	case TOUCH_CALC_PEN_RSK:
		translate_softkey_pen_event(MMI_RIGHT_SOFTKEY, MMI_PEN_EVENT_MOVE, 
				point.x, point.y, &button_event);
		break;
    default:
        break;
	}
}

void mmi_touchcalc_pen_repeat_hdlr(mmi_pen_point_struct point)
{
	mmi_gui_calc_pen_enum  calc_menu_event;
	if(g_calculator_keypad.flag==CALC_PEN_DOWN_ON_KEYPAD)
	{
		coolsand_UI_calculator_translate_pen_event(
                &g_calculator_keypad, 
                point.x, point.y,
                &calc_menu_event,MMI_PEN_EVENT_REPEAT);
		if(calc_menu_event == GUI_CALC_PEN_HIGHLIGHT_CHANGED )
			coolsand_UI_BLT_double_buffer(TOUCH_CALC_SCREEN_X,TOUCH_CALC_SCREEN_Y,UI_device_width - 1,TOUCH_CALC_SCREEN_Y+TOUCH_CALC_SCREEN_HEIGHT-1 );
	}
}
void mmi_touchcalc_pen_long_tap_hdlr(mmi_pen_point_struct point)
{
	mmi_gui_button_pen_enum button_event;
	if(calc_pen_on_object == TOUCH_CALC_PEN_RSK)
	{
		translate_softkey_pen_event(MMI_RIGHT_SOFTKEY, MMI_PEN_EVENT_LONG_TAP, 
				point.x, point.y, &button_event);
		if (button_event == GUI_BUTTON_PEN_LONGTAP)
		{
			execute_softkey_function(KEY_LONG_PRESS,MMI_RIGHT_SOFTKEY);
		}
	}

}
void setup_calculator_keypad(calculator_keypad_struct *calculator_keypad, S16 x, S16 y)
{
//	calculator_keypad->result_area_x = x + CALC_TOUCH_RESULT_AREA_X;
	calculator_keypad->result_area_x = x + CALC_TOUCH_RESULT_AREA_X;// Kenneth modified for keypad for 2324
	calculator_keypad->result_area_y = y + CALC_TOUCH_RESULT_AREA_Y;

	calculator_keypad->keypad_x = calculator_keypad->result_area_x;
	calculator_keypad->keypad_y = calculator_keypad->result_area_y+CALC_TOUCH_RESULT_AREA_HEIGHT+CALC_TOUCH_RESULT_KEYPAD_GAP; 
	calculator_keypad->keypad_width = CALC_TOUCH_KEYPAD_WIDTH;
	calculator_keypad->keypad_height = CALC_TOUCH_KEYPAD_HEIGHT;
	calculator_keypad->key_width = CALC_TOUCH_KEY_WIDTH;
	calculator_keypad->key_height = CALC_TOUCH_KEY_HEIGHT;
	calculator_keypad->key_type = MMI_CALC_KEY_NONE;
	calculator_keypad->horizontal_gap = CALC_TOUCH_HORIZONTAL_GAP;
	calculator_keypad->vertical_gap = CALC_TOUCH_VERTICAL_GAP;
	calculator_keypad->n_column = CALC_TOUCH_NCOLUMNS;
	calculator_keypad->n_rows =CALC_TOUCH_NROWS;
	calculator_keypad->operand1_y = calculator_keypad->result_area_y+CALC_TOUCH_OPERAND1_Y;
	calculator_keypad->operand2_y = calculator_keypad->result_area_y + CALC_TOUCH_OPERAND2_Y;
	calculator_keypad->operator_x = calculator_keypad->result_area_x+ CALC_TOUCH_OPERATOR_X;
	calculator_keypad->operator_y = calculator_keypad->operand2_y;
	calculator_keypad->mem_x1 = calculator_keypad->result_area_x + TOUCH_CALC_MEM_X1;
	calculator_keypad->mem_y1 = calculator_keypad->result_area_y + TOUCH_CALC_MEM_Y1;
	calculator_keypad->flag = 0;
	
}
void show_touch_calculator_screen(S32 x1, S32 y1, S32 width, S32 height)
{
	
	S32 x2,y2;
	x2 = x1+width-1;
	y2 = y1+height-1;
	gdi_layer_push_clip();
	gdi_layer_set_clip(x1,y1,x2,y2);
	coolsand_UI_show_image(x1,y1,(UI_image_type)GetImage(IMG_ID_TOUCH_CALC_MAIN)); 
	gdi_layer_pop_clip();
	coolsand_UI_BLT_double_buffer(x1,y1,x2,y2);
}


void coolsand_UI_calculator_translate_pen_position(calculator_keypad_struct *calculator_keypad,S32 x, S32 y,S32 *item_index, S32 *key_variable)
{
	S32	total_height=0,total_width = 0;
	S32 i,j;
	total_height = 0;
	total_width = 0;
	
	x=x-calculator_keypad->keypad_x+1;
	y=y-calculator_keypad->keypad_y+1;

	for(i=0;i<calculator_keypad->n_rows;i++)
	{
		total_height+=calculator_keypad->key_height ;
		if(total_height>=y)
		{
			for(j=0;j<calculator_keypad->n_column;j++)
			{
				total_width+= calculator_keypad->key_width;
				if(total_width>=x)
				{
					*item_index = i*calculator_keypad->n_column + j +1 ;
					*key_variable = TouchCalcKeys[i][j];
					calculator_keypad->selected_key_x = calculator_keypad->keypad_x + (calculator_keypad->key_width)*j+(calculator_keypad->horizontal_gap)*j;
					calculator_keypad->selected_key_y = calculator_keypad->keypad_y + (calculator_keypad->key_height)*i+(calculator_keypad->vertical_gap)*i;
					if( i==0 || j==(calculator_keypad->n_column-1) || (*key_variable == TOUCH_CALC_OP_EQUAL && i==calculator_keypad->n_rows-1) )
						calculator_keypad->key_type = MMI_CALC_SYM_KEY;
					else
						calculator_keypad->key_type = MMI_CALC_NUM_KEY;
					break;
				}
				else
				{
					total_width+=calculator_keypad->horizontal_gap;
					if(total_width>=x)
					{
						*item_index = -1;
						return;
					}
				}
			
			}
		break;
		}
		else
		{
			total_height+=calculator_keypad->vertical_gap;
			if(total_height>=y)
			{
				*item_index = -1;
				return;
			}
		}
	}
}	

void coolsand_UI_calculator_key_select(calculator_keypad_struct *calculator_keypad, S32 item_index)
{
	S32 x1,y1,x2,y2;
	S32 width,height;
	UI_image_ID_type	image_id;
	x1 = calculator_keypad->selected_key_x;
	y1 = calculator_keypad->selected_key_y;
	
	x2 = x1 + calculator_keypad->key_width-1;
	y2 = y1 + calculator_keypad->key_height-1;
	
	calculator_keypad->selected_key_bitmap.buf_ptr = (U8*)matrix_main_menu_highlight_bitmap_data;

	memset(calculator_keypad->selected_key_bitmap.buf_ptr,0,(calculator_keypad->key_width)*(calculator_keypad->key_height)*2);
	gdi_layer_push_clip();
	gdi_layer_set_clip(x1,y1,x2,y2);
	gdi_image_cache_bmp_get(x1, y1, x2, y2, &calculator_keypad->selected_key_bitmap);
	gdi_layer_pop_clip();
	
	gdi_layer_push_clip();
	gdi_layer_set_clip(x1,y1,x2,y2);
	#ifdef __MMI_USE_DRAWING_HIDHLIGHT__
	{
	color_t UICOLOR_WHITE = {240, 240, 240, 80};
	gui_fill_transparent_color(x1, y1, x2, y2, UICOLOR_WHITE);
		}
       #else
	image_id = calculator_key_image[item_index-1];
	coolsand_UI_measure_image(get_image(image_id),&width,&height);
	x1 = x1+((calculator_keypad->key_width>>1)-(width>>1));//modify hansl tbm780 2007/07/12
	y1 = y1+((calculator_keypad->key_height>>1)-(height>>1));
	coolsand_UI_show_image(x1,y1,get_image(image_id));
	#endif
	gdi_layer_pop_clip();
}

void coolsand_UI_calculator_key_unselect(calculator_keypad_struct *calculator_keypad)
{
		S16	x1,x2,y1,y2;
		x1 = calculator_keypad->selected_key_x;
		y1 = calculator_keypad->selected_key_y;
		x2 = x1+calculator_keypad->key_width-1;
		y2 = y1+calculator_keypad->key_height-1;
		
		gdi_layer_push_clip();
		gdi_layer_set_clip(x1,y1,x2, y2);
		gdi_image_cache_bmp_draw(x1, y1, &calculator_keypad->selected_key_bitmap);
		calculator_keypad->selected_key_x = 0;
		calculator_keypad->selected_key_y = 0;
		gdi_layer_pop_clip();
}


BOOL coolsand_UI_calculator_translate_pen_event( calculator_keypad_struct *calculator_keypad, S32 x, S32 y, mmi_gui_calc_pen_enum *menu_event, mmi_pen_event_type_enum  pen_event )
{
	S32 x1,y1;
	BOOL ret;
	S32 item_index,key_variable;
	x1 = calculator_keypad->keypad_x;
	y1 = calculator_keypad->keypad_y;
	
	ret = TRUE;
	*menu_event = GUI_CALC_PEN_NONE;
	switch(pen_event)
	{
	case MMI_PEN_EVENT_DOWN:
		if(PEN_CHECK_BOUND(x, y, x1, y1,calculator_keypad->keypad_width-1 , calculator_keypad->keypad_height-1 ))
		{
			coolsand_UI_calculator_translate_pen_position(calculator_keypad,x,y,&item_index,&key_variable);
			if(item_index>MMI_CALC_KEY_START && item_index<=MMI_CALC_KEY_END )
			{
				calculator_keypad->flag|=CALC_PEN_DOWN_ON_KEYPAD;
				coolsand_UI_calculator_key_select(calculator_keypad,item_index);
				if( calculator_keypad->key_type == MMI_CALC_NUM_KEY )
				{
					*menu_event = GUI_CALC_PEN_HIGHLIGHT_CHANGED;
					if(key_variable == TOUCH_CALC_KEY_DOT && g_calc_cntx->ResultState == RESULT_NOT_EXCEED)//should not be called if result is exceded..no change on pressing Dot
						CalcHandlePoundKey();
					else
						CalcHandleNumKey(key_variable);
				}
				else if( calculator_keypad->key_type == MMI_CALC_SYM_KEY )
				{
					*menu_event = GUI_CALC_PEN_HIGHLIGHT_CHANGED;
					g_calc_cntx->CurrHilite = key_variable;
					if( key_variable==TOUCH_CALC_OP_EQUAL )
					{
						CalcComputeResult();
					}
					else if(key_variable>= TOUCH_CALC_OP_MP && key_variable<= TOUCH_CALC_OP_MR && g_calc_cntx->ResultState == RESULT_NOT_EXCEED)//should not be called if result is exceded..no change on pressing these buttons
					{
						CalcHandleHiliteOperator();
						CalcHandleHiliteMemoryButton();
						CalcComputeResult();
					}
					else
					{
						CalcHandleHiliteOperator();	
						CalcShowResultScreen();
					}
				}
			}
			else
				ret = FALSE;
		}
		else
		{
			ret = FALSE;
		}
	break;
	
	case MMI_PEN_EVENT_UP:
		calculator_keypad->flag&=~CALC_PEN_DOWN_ON_KEYPAD;
		TOUCH_CALC_SELECT_KEY = TRUE;
		if(PEN_CHECK_BOUND(x, y, calculator_keypad->selected_key_x, calculator_keypad->selected_key_y, calculator_keypad->key_width-1,calculator_keypad->key_height-1))
		{
			*menu_event = GUI_CALC_PEN_HIGHLIGHT_CHANGED;
			coolsand_UI_calculator_key_unselect(calculator_keypad);
		}
		else if(TOUCH_CALC_SELECT_KEY)
		{
			coolsand_UI_calculator_key_unselect(calculator_keypad);
		}
		break;
	case MMI_PEN_EVENT_MOVE:
		if( (!PEN_CHECK_BOUND(x, y, calculator_keypad->selected_key_x, calculator_keypad->selected_key_y, calculator_keypad->key_width-1,calculator_keypad->key_height-1)))
		{
		*menu_event = GUI_CALC_PEN_HIGHLIGHT_CHANGED;
			TOUCH_CALC_SELECT_KEY = FALSE;
		}
		else if(PEN_CHECK_BOUND(x, y, calculator_keypad->selected_key_x, calculator_keypad->selected_key_y, calculator_keypad->key_width-1,calculator_keypad->key_height-1))
		{
			coolsand_UI_calculator_translate_pen_position(calculator_keypad,x,y,&item_index,&key_variable);	
			if(!TOUCH_CALC_SELECT_KEY)
				coolsand_UI_calculator_key_select(calculator_keypad,item_index);
			TOUCH_CALC_SELECT_KEY = TRUE;
		}
		break;
	case MMI_PEN_EVENT_LONG_TAP:
		break;
	case MMI_PEN_EVENT_REPEAT:
		if(PEN_CHECK_BOUND(x, y, calculator_keypad->selected_key_x, calculator_keypad->selected_key_y, calculator_keypad->key_width-1,calculator_keypad->key_height-1))
		{
			coolsand_UI_calculator_translate_pen_position(calculator_keypad,x,y,&item_index,&key_variable);
			if( calculator_keypad->key_type == MMI_CALC_NUM_KEY )
			{
				*menu_event = GUI_CALC_PEN_HIGHLIGHT_CHANGED;
				if(key_variable == TOUCH_CALC_KEY_DOT )
					CalcHandlePoundKey();
				else
					CalcHandleNumKey(key_variable);
			}
		}
		break;
    default:
        break;
	}
return ret;
}
#endif //__MMI_TOUCH_SCREEN
#endif

#endif //#ifndef _MMI_CALCULATOR_C

