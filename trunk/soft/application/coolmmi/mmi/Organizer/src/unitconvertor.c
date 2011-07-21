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
 *	Unitconvertor.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements Unit Convertor application.
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
  * Nov 19 2005 CSD01215
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**********************************************************************************
	Filename:		Unitconvertor.c
 
	Date Created:	June-14-2003
	Contains:		Unit Convertor
**********************************************************************************/		
#ifndef _MMI_UNITCONVERTOR_C
#define _MMI_UNITCONVERTOR_C

/*  Include: MMI header file */
#include "mmi_features.h"
#ifdef __MMI_UNIT_CONVERTER__
#include "mmi_data_types.h"
#include "unitconvertor.h"
#include "unicodexdcl.h"
#include "historygprot.h"
#include "globaldefs.h"
#include "settingprofile.h"
#include "debuginitdef.h"
#include "commonscreens.h"
#include "eventsgprot.h" 
#include "wgui_inputs.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "organizerdef.h"
#include "currencyconvertor.h"
#include "mmi_trace.h"
#ifdef _MMI_OPERATOR_	//add by hansl 061108
#include "operator.h"
#endif
/*  Include: PS header file */

/* 
** Define
*/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
#define	UCONV_MAX_RESULT		28
#else
#define	UCONV_MAX_RESULT		14
#endif
#define	UCONV_MAX_INPUT			8
#define	UCONV_NUM_UNITS			4

#ifdef _MMI_OPERATOR_	//add by hansl 061108
const S8 KM2MILE[OPERATOR_LEN+1] = {'0','.','6','2','1','3','7','\0'};
const S8 M2YARD[OPERATOR_LEN+1] =  {'1','.','0','9','3','6','1','\0'};
const S8 M2FOOT[OPERATOR_LEN+1] =  {'3','.','2','8','0','8','4','\0'};
const S8 CM2INCH[OPERATOR_LEN+1] =  {'0','.','3','9','3','7','\0'};
const S8 KG2POUND[OPERATOR_LEN+1] =  {'2','.','2','0','4','6','2','\0'};
const S8 KG2OUNCE[OPERATOR_LEN+1] =  {'3','5','.','2','7','4','\0'};
#else
#define	KM2MILE					0.62137
#define	M2YARD					1.09361
#define	M2FOOT					3.28084
#define	CM2INCH					0.3937
#define	KG2POUND				2.20462
#define	KG2OUNCE				35.274
#endif

/* 
** Typedef 
*/
typedef struct
{
	S32 CurrInlineSelectUnit;
	S8	*UnitList[UCONV_NUM_UNITS];
	UI_character_type MetricBuf[UCONV_MAX_RESULT+2];
	UI_character_type EngBuf[UCONV_MAX_RESULT+2];	
	U8	CurrHiliteUnitType;		/* 0 == weight, 1 == length */
	U8	CurrSelectedItem;
	U8	ClearResultFlag;
//	U8	IsComputingResult;
}uconv_context_struct;

/* 
** Local Variable
*/


/* 
** Local Function
*/
//void UconvClearResult(void);


/* 
** Global Variable
*/
uconv_context_struct *g_uconv_cntx = NULL;
extern wgui_inline_item wgui_inline_items[];
//void (*complete_inline_item_edit)(void)=UI_dummy_function;

/* 
** Global Function
*/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
extern void coolsand_UI_atof(CALOPERATOR_STRUCT *result,U8 *stringCnt,UI_string_type  s);
extern void  coolsand_UI_float_string(S8* dob_val,S8 prec_val,S8* out_buffer);
extern void set_leading_zero(U8 u8type);
extern S8 CalcComputePrecision(S8* result, S16 max_digits);
extern void CconvClearResult(void);
#else
extern DOUBLE coolsand_UI_atof(UI_string_type s);
extern void coolsand_UI_float_string(DOUBLE dob_val,S32 prec_val,S8* out_buffer);
extern void set_leading_zero(U8 u8type);
extern S32 CalcComputePrecision(DOUBLE result, S16 max_digits);
extern void CconvClearResult(void);
#endif

#if defined(__MMI_TOUCH_SCREEN__)
typedef void (*fnPtr)(void);
extern void mmi_pen_editor_set_vk_hw_pre_handler(fnPtr	vk_pre,fnPtr hw_pre);
void	pre_unit_process_vk_hw(void );
#endif
extern void mmi_conv_reset_rsk(void);

/*****************************************************************************
* FUNCTION
*  HighlightUConvMenu
* DESCRIPTION
*   Highlight handler of Unit Conveter menu item. 
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightUConvMenu(void)
{	
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(EntryUConvMenu,KEY_EVENT_UP);
	SetKeyHandler(EntryUConvMenu,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	HighlightUConvLength
* DESCRIPTION
*   Highlight handler of Length menu in Unit Convertor
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightUConvLength(void)
{
	g_uconv_cntx->CurrHiliteUnitType = UCONV_LENGTH;
	SetLeftSoftkeyFunction(UConvGoToComputeScreen,KEY_EVENT_UP);
	SetKeyHandler(UConvGoToComputeScreen,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	HighlightUConvWeight
* DESCRIPTION
*   Highlight handler of Weight menu in Unit Convertor
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightUConvWeight(void)
{
	g_uconv_cntx->CurrHiliteUnitType = UCONV_WEIGHT;
	SetLeftSoftkeyFunction(UConvGoToComputeScreen,KEY_EVENT_UP);
	SetKeyHandler(UConvGoToComputeScreen,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*	UConvFillInlineStruct
* DESCRIPTION
*   Fill inline structure of inline items in Length convertor
* PARAMETERS
*	UnitType	IN	type of unit	0 == length, 1 == weight
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void UConvFillInlineStruct(void)
{
	UI_character_type	*Caption1,*Caption2;
	mmi_trace(g_sw_ORGANIZER, "begin------UConvFillInlineStruct---------end");

	DisableInlineInputMethodHints();

	if(g_uconv_cntx->CurrHiliteUnitType == UCONV_LENGTH)	/* length */
	{
		g_uconv_cntx->UnitList[0] = GetString(STR_UNITCONVERTOR_KMMILE);
		g_uconv_cntx->UnitList[1] = GetString(STR_UNITCONVERTOR_MYARD);
		g_uconv_cntx->UnitList[2] = GetString(STR_UNITCONVERTOR_MFOOT);
		g_uconv_cntx->UnitList[3] = GetString(STR_UNITCONVERTOR_CMINCH);

		/* caption strings */
		Caption2 = (UI_character_type*)(GetString((U16)(STR_UNITCONVERTOR_MILE+g_uconv_cntx->CurrInlineSelectUnit)));
		switch(g_uconv_cntx->CurrInlineSelectUnit)
		{
			case 0:
				Caption1=(UI_character_type*)(GetString(STR_UNITCONVERTOR_KM));
				break;
			case 3:
				Caption1=(UI_character_type*)(GetString(STR_UNITCONVERTOR_CM));
				break;
			default:
				Caption1=(UI_character_type*)(GetString(STR_UNITCONVERTOR_METER));
				break;
		}
		
		/* units */
		SetInlineItemSelect(&wgui_inline_items[UCONV_UNIT], 4, (U8**)g_uconv_cntx->UnitList,&g_uconv_cntx->CurrInlineSelectUnit);
		RegisterInlineSelectHighlightHandler(&wgui_inline_items[UCONV_UNIT],HighlightUConvInlineLenghSelect);
	}
	else	/* weight */
	{
		g_uconv_cntx->UnitList[0] = GetString(STR_UNITCONVERTOR_KGPOUND);
		Caption1=(UI_character_type*)(GetString(STR_UNITCONVERTOR_KG));
		
		g_uconv_cntx->UnitList[1] = GetString(STR_UCONV_KGOUNCE);

		if(g_uconv_cntx->CurrInlineSelectUnit == 0)	/* kg <-> pound */
			Caption2=(UI_character_type*)(GetString(STR_UNITCONVERTOR_POUND));
		else	/* Kg <-> Ounce */
			Caption2=(UI_character_type*)(GetString(STR_UCONV_OUNCE));

		/* unit */
		SetInlineItemSelect(&wgui_inline_items[UCONV_UNIT], 2, (U8 **)g_uconv_cntx->UnitList,&g_uconv_cntx->CurrInlineSelectUnit);
		RegisterInlineSelectHighlightHandler(&wgui_inline_items[UCONV_UNIT],HighlightUConvInlineWeightSelect);
		
	}

	
	SetInlineItemActivation(&wgui_inline_items[UCONV_UNIT], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);

	/* metric caption */
	SetInlineItemActivation(&wgui_inline_items[UCONV_METRIC_CAP], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[UCONV_METRIC_CAP], (U8*)Caption1);

	/* metric unit */
	SetInlineItemActivation(&wgui_inline_items[UCONV_METIRC], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&wgui_inline_items[UCONV_METIRC], (U8*)g_uconv_cntx->MetricBuf, (UCONV_MAX_INPUT+1), INPUT_TYPE_DECIMAL_NUMERIC);
	EnableInlineItemBoundary(&wgui_inline_items[UCONV_METIRC]);
	RightJustifyInlineItem(&wgui_inline_items[UCONV_METIRC]);
	SetInlineTextEditCustomFunction(&wgui_inline_items[UCONV_METIRC],UConvCustomInlineFunction);

	/* English caption */
	SetInlineItemActivation(&wgui_inline_items[UCONV_ENG_CAP], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[UCONV_ENG_CAP], (U8*)Caption2);

	/* English unit */
	SetInlineItemActivation(&wgui_inline_items[UCONV_ENG], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&wgui_inline_items[UCONV_ENG], (U8*)g_uconv_cntx->EngBuf, (UCONV_MAX_INPUT+1), INPUT_TYPE_DECIMAL_NUMERIC);
	EnableInlineItemBoundary(&wgui_inline_items[UCONV_ENG]);
	RightJustifyInlineItem(&wgui_inline_items[UCONV_ENG]);
	SetInlineTextEditCustomFunction(&wgui_inline_items[UCONV_ENG],UConvCustomInlineFunction);
}


/*****************************************************************************
* FUNCTION
*	UConvRedrawResult
* DESCRIPTION
*   Redraw result after computing
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void UConvRedrawResult(void)
{
#ifdef __MMI_CONVERTER_KEEP_INLINE_FOCUS__
	RedrawCategoryFunction();
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#if(UI_DOUBLE_BUFFER_SUPPORT)
/* under construction !*/
	#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#if(UI_DOUBLE_BUFFER_SUPPORT)
/* under construction !*/
/* under construction !*/
	#endif
	#endif
#else
	SetHighlightedItem(0);
#endif


}

/*****************************************************************************
* FUNCTION
*	ComputeLengthResult
* DESCRIPTION
*   To calculate the result of length conversion
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void ComputeLengthResult(void)
{	
	
	UI_character_type *ResultBuf = NULL, *SourceBuf = NULL;
	S32 len;
	U8 number1 = 0;
	U8 number2 = 0;
	S8 conversion_factor[OPERATOR_LEN + 1];
	
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
	
	CloseCategory57Screen();

	/* conversion rate */
    switch(g_uconv_cntx->CurrInlineSelectUnit)
	{	
		case 0: 	
		{
			memcpy(conversion_factor, KM2MILE, OPERATOR_LEN + 1);
			break ;
		}
		case 1:		
		{
			memcpy(conversion_factor, M2YARD, OPERATOR_LEN + 1);
			break;	
		}
		case 2: 	
		{
			memcpy(conversion_factor, M2FOOT, OPERATOR_LEN + 1);
			break;
		}
		default: /*case 3: */		
		{
			memcpy(conversion_factor, CM2INCH, OPERATOR_LEN + 1);
			break;			
		}
	}

	if (g_uconv_cntx->CurrSelectedItem == UCONV_METIRC)
	{
		SourceBuf = g_uconv_cntx->MetricBuf;
		ResultBuf = g_uconv_cntx->EngBuf;				
	    	coolsand_UI_atof(result1, &number1, SourceBuf);
		CalculatorGetOperator(conversion_factor, &number2, result2);	
		CalculatorMult(result1, result2);
		Calgetrealresult(result2, result1);		
		AnsiiToUnicodeString((S8*)ResultBuf, (S8*)result2->operBuff);
		OslMfree(result1);
		OslMfree(result2);
		result1 = NULL;
		result2 = NULL;		
	}
	else
	{
		SourceBuf = g_uconv_cntx->EngBuf;
		ResultBuf = g_uconv_cntx->MetricBuf;		
	    	coolsand_UI_atof(result1, &number1, SourceBuf);
		CalculatorGetOperator(conversion_factor, &number2, result2);
		CalculatorDivi(result1, result2);
		Calgetrealresult(result2, result1);		
		AnsiiToUnicodeString((S8*)ResultBuf, (S8*)result2->operBuff);
		OslMfree(result1);
		OslMfree(result2);
		result1 = NULL;
		result2 = NULL;		
	}
	
	len = coolsand_UI_strlen((UI_string_type)SourceBuf);
	if ((len == 0) || ((len == 1) && (SourceBuf[0] == 46)))
	{
		memset((void*)ResultBuf, 0, UCONV_MAX_RESULT+2);
		DisplayPopup((U8*)GetString(UC_ERROR_EMPTY_STRING), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
		return;
	}

	g_uconv_cntx->ClearResultFlag = TRUE;	
	UConvRedrawResult();
	SetLeftSoftkeyFunction(ComputeLengthResult, KEY_EVENT_UP);
	
}
#else
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
void ComputeLengthResult(void)
{
	DOUBLE conversion_factor;
	S32 precision=0;//,temp_int = 0;
	DOUBLE result;
	UI_character_type *ResultBuf=NULL, *SourceBuf=NULL;
	S32 len;
	
	CloseCategory57Screen();

	/* conversion rate */
    switch(g_uconv_cntx->CurrInlineSelectUnit)
	{	
		case 0: 
			conversion_factor = KM2MILE;  /* 0.62137 */ 
			break ;
		case 1:		
			conversion_factor = M2YARD;	/* 1.09361 */
			break;	
		case 2: 
			conversion_factor = M2FOOT;	/* 3.28084 */
			break;
		default: /*case 3: */
			conversion_factor = CM2INCH;  /* 0.3937 */
			break;			   
	}

	if(g_uconv_cntx->CurrSelectedItem == UCONV_METIRC)
	{
		SourceBuf = g_uconv_cntx->MetricBuf;
		ResultBuf = g_uconv_cntx->EngBuf;
	}
	else
	{
		SourceBuf = g_uconv_cntx->EngBuf;
		ResultBuf = g_uconv_cntx->MetricBuf;
		conversion_factor = 1/conversion_factor;
	}
	
	len = coolsand_UI_strlen((UI_string_type)SourceBuf);
	if((len == 0) || ((len==1) && (SourceBuf[0]== 46)))
	{
		DisplayPopup((U8*)GetString(UC_ERROR_EMPTY_STRING),IMG_GLOBAL_WARNING,0,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
		return;
	}

	g_uconv_cntx->ClearResultFlag = TRUE;
	
	result =(DOUBLE)(conversion_factor * coolsand_UI_atof(SourceBuf));

	precision = CalcComputePrecision(result, (S16)UCONV_MAX_RESULT);
	coolsand_UI_float_string(result,precision,(S8*)ResultBuf);

	UConvRedrawResult();
	SetLeftSoftkeyFunction(ComputeLengthResult, KEY_EVENT_UP);
}
#else
void ComputeLengthResult(void)
{
}
#endif
#endif


/*****************************************************************************
* FUNCTION
*	ComputeWeightResult
* DESCRIPTION
*   To calculate the result of weight conversion
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef _MMI_OPERATOR_	//add by hansl 061108
void ComputeWeightResult(void)
{ 
	
	UI_character_type *ResultBuf = NULL, *SourceBuf = NULL ;
	S8 len1;
	U8 number1 = 0;
	U8 number2 = 0;
	S8 conversion_factor[OPERATOR_LEN + 1];
	
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

	CloseCategory57Screen();
	
	/* determine rate */
       if (g_uconv_cntx->CurrInlineSelectUnit == 0)	// Kg to Pound
    	{
		memcpy(conversion_factor, KG2POUND, OPERATOR_LEN + 1);
    	}
	else
	{
		memcpy(conversion_factor, KG2OUNCE, OPERATOR_LEN + 1);
	}

	/* determine conversion direction */
	if (g_uconv_cntx->CurrSelectedItem == UCONV_METIRC)
	{
		SourceBuf = g_uconv_cntx->MetricBuf;
		ResultBuf = g_uconv_cntx->EngBuf;		
	    	coolsand_UI_atof(result1, &number1, SourceBuf);
		CalculatorGetOperator(conversion_factor, &number2, result2);
		CalculatorMult(result1, result2);
		Calgetrealresult(result2, result1);		
		AnsiiToUnicodeString((S8*)ResultBuf, (S8*)result2->operBuff);
		OslMfree(result1);
		OslMfree(result2);
		result1 = NULL;
		result2 = NULL;	
	}
	else	/* UCONV_ENGLISH */
	{
		SourceBuf = g_uconv_cntx->EngBuf;
		ResultBuf = g_uconv_cntx->MetricBuf;		
	    	coolsand_UI_atof(result1, &number1, SourceBuf);
		CalculatorGetOperator(conversion_factor, &number2, result2);
		CalculatorDivi(result1, result2);
		Calgetrealresult(result2, result1);		
		AnsiiToUnicodeString((S8*)ResultBuf, (S8*)result2->operBuff);
		OslMfree(result1);
		OslMfree(result2);
		result1 = NULL;
		result2 = NULL;			
	}
	
	len1 = coolsand_UI_strlen((UI_string_type)SourceBuf);

	if ((len1 == 0) || ((len1 == 1) && (ResultBuf[0] == 46)))
	{
		memset(ResultBuf, 0, UCONV_MAX_RESULT+2);
		DisplayPopup((U8*)GetString(UC_ERROR_EMPTY_STRING), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		return;
	}
	
	g_uconv_cntx->ClearResultFlag = TRUE;

	UConvRedrawResult();

	SetLeftSoftkeyFunction(ComputeWeightResult, KEY_EVENT_UP);
	
}
#else
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
void ComputeWeightResult(void)
{ 
	DOUBLE result=0;
	S32 prec_val = 0;
	DOUBLE conversion_factor = 0;
	UI_character_type *ResultBuf=NULL, *SourceBuf=NULL ;
	S32 len;
	
	CloseCategory57Screen();
	
	/* determine rate */
    if(g_uconv_cntx->CurrInlineSelectUnit == 0)	// Kg to Pound
		conversion_factor = KG2POUND; 
	else
		conversion_factor = KG2OUNCE; 			

	/* determine conversion direction */
	if(g_uconv_cntx->CurrSelectedItem == UCONV_METIRC)
	{
		SourceBuf = g_uconv_cntx->MetricBuf;
		ResultBuf = g_uconv_cntx->EngBuf;
	}
	else	/* UCONV_ENGLISH */
	{
		SourceBuf = g_uconv_cntx->EngBuf;
		ResultBuf = g_uconv_cntx->MetricBuf;
		conversion_factor = 1/conversion_factor;
	}
	
	len = coolsand_UI_strlen((UI_string_type)SourceBuf);
	
	if((len == 0) || ((len==1) && (ResultBuf[0]== 46)))
	{
		DisplayPopup((U8*)GetString(UC_ERROR_EMPTY_STRING),IMG_GLOBAL_WARNING,0,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
		return;
	}
	
	g_uconv_cntx->ClearResultFlag = TRUE;
	
	result =(DOUBLE)(conversion_factor * coolsand_UI_atof(SourceBuf));
    
	prec_val = CalcComputePrecision(result, UCONV_MAX_RESULT);
	coolsand_UI_float_string(result,prec_val,(S8*)ResultBuf);

	UConvRedrawResult();

	SetLeftSoftkeyFunction(ComputeWeightResult, KEY_EVENT_UP);

}
#else
void ComputeWeightResult(void)
{ 
}
#endif
#endif

/*****************************************************************************
* FUNCTION
*	UConvGoToComputeScreen
* DESCRIPTION
*   Intialize state variables and enter weight convertor
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void UConvGoToComputeScreen(void)
{
	g_uconv_cntx->CurrInlineSelectUnit=0;
	g_uconv_cntx->CurrSelectedItem=0;
	memset(g_uconv_cntx->EngBuf, 0, 2);
	memset(g_uconv_cntx->MetricBuf, 0, 2);
	EntryUConvCompute();
}


/*****************************************************************************
* FUNCTION
*	ExitUConvCompute
* DESCRIPTION
*   Exit function of conterting screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitUConvCompute(void)
{
	history_t   HistoryUCWeight;
	U16 inputBufferSize;                //added for inline edit history
	S16 nHistory = 0;
	set_leading_zero(TRUE);	
	CloseCategory57Screen();
	HistoryUCWeight.scrnID=SCR_ID_UCONV_WEIGHT+g_uconv_cntx->CurrHiliteUnitType;
	HistoryUCWeight.entryFuncPtr=EntryUConvCompute;
	pfnUnicodeStrcpy((S8*)HistoryUCWeight.inputBuffer, (S8*)&nHistory);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) HistoryUCWeight.inputBuffer );         //added for inline edit history
	AddNHistory(HistoryUCWeight, inputBufferSize);                       //added for inline edit history
}


/*****************************************************************************
* FUNCTION
*	EntryUConvCompute
* DESCRIPTION
*   Entry function of conterting screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryUConvCompute(void)
{
	U8 *guiBuffer;
	U8* inputBuffer;                       //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history
	U16	ScrID = (SCR_ID_UCONV_WEIGHT+g_uconv_cntx->CurrHiliteUnitType);

	EntryNewScreen(ScrID, ExitUConvCompute, NULL, NULL);

	set_leading_zero(FALSE);
	
	guiBuffer = GetCurrGuiBuffer(ScrID);

	InitializeCategory57Screen();

	UConvFillInlineStruct();
	RegisterHighlightHandler(HighlightUConvInlineItem);
	
	if(guiBuffer)
		memset((void*)guiBuffer, 0, 2);
	
	inputBuffer = GetCurrNInputBuffer(ScrID, &inputBufferSize);    //added for inline edit history
	
	if(inputBuffer)                                            //added for inline edit history
		SetCategory57Data(wgui_inline_items, UCONV_INLINE_TOTAL, inputBuffer);//sets the data 

	DisableCategory57ScreenDone();
	
	ShowCategory57Screen(STR_UNITCONVERTOR_CAPTION1,GetRootTitleIcon(ORGANIZER_UNITCONVERTOR_MENU), 
							STR_GLOBAL_OK, IMG_GLOBAL_OK, 
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
							UCONV_INLINE_TOTAL, NULL,
							wgui_inline_items, g_uconv_cntx->CurrSelectedItem, guiBuffer);


	if(g_uconv_cntx->CurrHiliteUnitType == UCONV_WEIGHT)	
		SetCategory57RightSoftkeyFunctions(ComputeWeightResult, GoBackHistory);
	else
		SetCategory57RightSoftkeyFunctions(ComputeLengthResult, GoBackHistory);
}


/*****************************************************************************
* FUNCTION
*  UConvCustomInlineFunction
* DESCRIPTION
*   Set group key handlers of inline editor
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void UConvCustomInlineFunction(void)
{
#ifdef __MMI_CONVERTER_KEEP_INLINE_FOCUS__
	U16 keyCodes[]={KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_POUND};
	U8	nKeyCodes=(U8)11;

	if(g_uconv_cntx->ClearResultFlag)
	{	
		g_uconv_cntx->ClearResultFlag = FALSE;

		SetGroupKeyHandler(CconvClearResult,keyCodes,nKeyCodes,KEY_EVENT_DOWN);
#if !defined(__MMI_WITH_C_KEY__)
	SetRightSoftkeyFunction(CconvClearResult, KEY_LONG_PRESS);//qiff 2008/10/20 add for greenstone bug: 10111
	SetRightSoftkeyFunction(CconvClearResult, KEY_EVENT_UP);
	SetRightSoftkeyFunction(NULL, KEY_EVENT_DOWN);
#else
	SetKeyHandler(CconvClearResult, KEY_CLEAR, KEY_EVENT_UP);
	SetKeyHandler(NULL, KEY_CLEAR, KEY_EVENT_DOWN);
#endif

#if defined(__MMI_TOUCH_SCREEN__)
	mmi_pen_editor_set_vk_hw_pre_handler(pre_unit_process_vk_hw,pre_unit_process_vk_hw );
#endif

	}
#endif
}

/*****************************************************************************
* FUNCTION
*  UconvClearResult
* DESCRIPTION
*   Clear buffer and input a character.
*	This function is used for new digits input after calculation.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
/*void UconvClearResult(void)
{	
	
#ifdef __MMI_CONVERTER_KEEP_INLINE_FOCUS__
	U16 keyCode,keyType;
	S32 hItem=GetHighlightedItem();

	GetkeyInfo((U16*)&keyCode,(U16*)&keyType);
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
#if defined(__MMI_TOUCH_SCREEN__)
	complete_inline_item_edit();
#endif
	coolsand_UI_fixed_list_menu_goto_item(&MMI_fixed_list_menu,0);
	coolsand_UI_fixed_list_menu_goto_item(&MMI_fixed_list_menu,hItem);
	show_fixed_list();

	if(keyCode != KEY_CLEAR) 
		ExecuteCurrKeyHandler(keyCode,keyType);
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
#endif
#endif
}*/




/*****************************************************************************
* FUNCTION
*  HighlightUConvInlineItem
* DESCRIPTION
*   Highlight handler of inline items
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightUConvInlineItem(S32 index) 
{
	if(index == 0)	/* inline selection */
	{	
//		if(!g_uconv_cntx->IsComputingResult)
//		{
			ChangeLeftSoftkey(STRING_ID_NULL, IMAGE_ID_NULL);
			ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
			SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
//		}
	}	
	else
	{
#ifdef __MMI_CONVERTER_KEEP_INLINE_FOCUS__

		if ((U8)index != g_uconv_cntx->CurrSelectedItem)
		{
			memset(g_uconv_cntx->MetricBuf, 0, 2);
			memset(g_uconv_cntx->EngBuf, 0, 2);
		}
#if 0  //qiff 2008/10/20 del for greenstone bug:10112		
		else if(!g_uconv_cntx->ClearResultFlag)
			g_uconv_cntx->ClearResultFlag=TRUE;
#endif
#if !defined(__MMI_WITH_C_KEY__)
		SetCategory57ScreenRSKClear();	 
#endif
		ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);

		if(g_uconv_cntx->CurrHiliteUnitType == UCONV_WEIGHT)
			SetLeftSoftkeyFunction(ComputeWeightResult,KEY_EVENT_UP);
		else
			SetLeftSoftkeyFunction(ComputeLengthResult,KEY_EVENT_UP);
		
#else
		if (index != g_uconv_cntx->CurrSelectedItem)
		{
			memset(g_uconv_cntx->MetricBuf, 0, 2);
			memset(g_uconv_cntx->EngBuf, 0, 2);
		}
		ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK);
		
		if(g_uconv_cntx->CurrHiliteUnitType == UCONV_WEIGHT)
			SetLeftSoftkeyFunction(ComputeWeightResult,KEY_EVENT_UP);
		else
			SetLeftSoftkeyFunction(ComputeLengthResult,KEY_EVENT_UP);

#endif
	}
	g_uconv_cntx->CurrSelectedItem = (U8)index;
	return;
}


/*****************************************************************************
* FUNCTION
*  HighlightUConvInlineWeightSelect
* DESCRIPTION
*   Highlight handler inline selection item in Weight convertor.
*	Redraw caption according to select.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightUConvInlineWeightSelect(S32 index) 
{
	UI_character_type	*Caption1,*Caption2;

	g_uconv_cntx->CurrInlineSelectUnit=(U8)index;
	
	Caption1=(UI_character_type*)(GetString(STR_UNITCONVERTOR_KG));
	if(index == 0)	/* Kg <-> Pound */	
		Caption2=(UI_character_type*)(GetString(STR_UNITCONVERTOR_POUND));
	else
		Caption2=(UI_character_type*)(GetString(STR_UCONV_OUNCE));

	memset(g_uconv_cntx->MetricBuf, 0, 2);
	memset(g_uconv_cntx->EngBuf, 0, 2);
	
	Category57ChangeItemText(1,(U8*)Caption1);
	Category57ChangeItemText(3,(U8*)Caption2);
	RedrawCategoryFunction();
}


/*****************************************************************************
* FUNCTION
*  HighlightUConvInlineLenghSelect
* DESCRIPTION
*   Highlight handler inline selection item in Length convertor.
*	Redraw caption according to select.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightUConvInlineLenghSelect(S32 index) 
{
	UI_character_type	*Caption1,*Caption2;

	g_uconv_cntx->CurrInlineSelectUnit=index;

	Caption2 = (UI_character_type*)(GetString((U16)(STR_UNITCONVERTOR_MILE+g_uconv_cntx->CurrInlineSelectUnit)));
	switch(g_uconv_cntx->CurrInlineSelectUnit)
	{
		case 0:	
			Caption1=(UI_character_type*)(GetString(STR_UNITCONVERTOR_KM));
			break;
		case 3:	
			Caption1=(UI_character_type*)(GetString(STR_UNITCONVERTOR_CM));
			break;
		default:
			Caption1=(UI_character_type*)(GetString(STR_UNITCONVERTOR_METER));
			break;
	}

	memset(g_uconv_cntx->MetricBuf, 0, 2);
	memset(g_uconv_cntx->EngBuf, 0, 2);

	Category57ChangeItemText(1,(U8*)Caption1);
	Category57ChangeItemText(3,(U8*)Caption2);
	RedrawCategoryFunction();
}


/*****************************************************************************
* FUNCTION
*  UconvFreeMemAndGoBack
* DESCRIPTION
*	Free memory and go back to previous screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void UconvDeInit(void)
{
	if(g_uconv_cntx)
	{
		OslMfree(g_uconv_cntx);
		g_uconv_cntx = NULL;
	}
}


/*****************************************************************************
* FUNCTION
*  UconvFreeMemAndGoBack
* DESCRIPTION
*	Free memory and go back to previous screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void UconvFreeMemAndGoBack(void)
{
	UconvDeInit();
	GoBackHistory();
}


/*****************************************************************************
* FUNCTION
*  EntryUConvMenu
* DESCRIPTION
*   Entry function of unit convertor
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryUConvMenu(void)
{
	U16 UC_list_of_items[10];
	U8 *guiBuffer;
	S32 num_of_items;

	if(g_uconv_cntx == NULL)
		g_uconv_cntx = OslMalloc(sizeof(uconv_context_struct));

	EntryNewScreen(SCR_ID_UCONV_MENU, NULL, EntryUConvMenu, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_UCONV_MENU);
	num_of_items = GetNumOfChild(ORGANIZER_UNITCONVERTOR_MENU);
	GetSequenceStringIds(ORGANIZER_UNITCONVERTOR_MENU,UC_list_of_items);
	SetParentHandler(ORGANIZER_UNITCONVERTOR_MENU);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);

	ShowCategory15Screen(ORGANIZER_MENU_UNITSCONVERTOR_STRINGID,
				GetRootTitleIcon(ORGANIZER_UNITCONVERTOR_MENU),STR_GLOBAL_OK,0,
				STR_GLOBAL_BACK,0,
				num_of_items,UC_list_of_items,(U16*)gIndexIconsImageList,
				 LIST_MENU,0,guiBuffer);

	SetRightSoftkeyFunction(UconvFreeMemAndGoBack, KEY_EVENT_UP);
	SetKeyHandler(UconvFreeMemAndGoBack,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	UconvInit
* DESCRIPTION
*   Set highlight handlers
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void UconvInit(void)
{
	SetHiliteHandler(ORGANIZER_UNITCONVERTOR_MENU,HighlightUConvMenu);
	SetHiliteHandler(ORGANIZER_UNITCONVERTOR_LENGTH,HighlightUConvLength);
	SetHiliteHandler(ORGANIZER_UNITCONVERTOR_WEIGHT,HighlightUConvWeight);
}

#if defined(__MMI_TOUCH_SCREEN__)

/*****************************************************************************
* FUNCTION
*  pre_process_vk_hw
* DESCRIPTION
*   Clear buffer and input a character.
*	This function is used for new digits input after calculation.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void pre_unit_process_vk_hw(void)
{	
#ifdef __MMI_CONVERTER_KEEP_INLINE_FOCUS__
	U16 keyCode,keyType;
	S32 hItem=GetHighlightedItem();
	GetkeyInfo((U16*)&keyCode,(U16*)&keyType);
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
	coolsand_UI_fixed_list_menu_goto_item(&MMI_fixed_list_menu,0);
	coolsand_UI_fixed_list_menu_goto_item(&MMI_fixed_list_menu,hItem);
	show_fixed_list();

	if (keyType == KEY_EVENT_LONG_PRESS && keyCode == KEY_RSK )
	{
	    SetRightSoftkeyFunction(mmi_conv_reset_rsk, KEY_EVENT_UP);
	}	
	
	if(keyCode != KEY_CLEAR) 
		ExecuteCurrKeyHandler(keyCode,keyType);
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
#endif
#endif
}
#endif


#endif	// __MMI_UNIT_CONVERTER__

#endif // #ifndef _MMI_UNITCONVERTOR_C


