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
 *	Currencyconvertor.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements Currency Convertor application.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**********************************************************************************
	Filename:		Currencyconvertor.c
 
	Date Created:	June-29-2003
	Contains:		Currency Convertor
**********************************************************************************/		
#ifndef _MMI_CURRENCYCONVRETOR_C
#define _MMI_CURRENCYCONVRETOR_C

/*  Include: MMI header file */
#include "mmi_features.h"
#include "mmi_trace.h"


#include "mmi_data_types.h"
#include "wrappergprot.h"
#include "currencyconvertor.h"
#include "unicodexdcl.h"	
#include "historygprot.h"
#include "globaldefs.h"
#include "timerevents.h"
#include "debuginitdef.h"
#include "eventsgprot.h" 
#include "settingprofile.h"
#include "wgui_inputs.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "string.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "organizerdef.h"
#include "custdataprots.h"
#include "commonscreens.h"
#include "operator.h"
/*  Include: PS header file */
//... Add More PS header
#include "mmiappfnptrs.h"


/* 
** Define
*/
#define	CCONV_MAX_RATE				6
#define	CCONV_MAX_INPUT				8
#define	CCONV_MAX_RESULT			14
#define	CCONV_SEL_BUF_LEN			10*ENCODING_LENGTH		/* length of inline selection string */
#define	CCONV_RESULT_BUF_LEN		(14+1)*ENCODING_LENGTH

#ifdef __MMI_CURRENCY_CONVERTER__

/* 
** Typedef 
*/
//add by zough 061121 end
#ifndef __MOD_ORGANIZER__	
typedef struct
{
	DOUBLE Rate;
	S32 CurrSelRate;	/* current selecting rate */
	S8	*RateList[2];
	S8	LocalBuff[CCONV_RESULT_BUF_LEN];
	S8	ForeBuff[CCONV_RESULT_BUF_LEN];
	S8	RateBuff[CCONV_RESULT_BUF_LEN];
	S8	RateString[2][CCONV_SEL_BUF_LEN];
	U8	CurrHiliteItem;
	U8	ClearResultFlag;
	U8	IsFromHistory;
//	U8	IsComputingResult;
}cconv_context_struct;
#else
typedef struct
{
	CALOPERATOR_STRUCT Rate;
	S32 CurrSelRate;	/* current selecting rate */
	S8	*RateList[2];
	S8	LocalBuff[CCONV_RESULT_BUF_LEN];
	S8	ForeBuff[CCONV_RESULT_BUF_LEN];
	S8	RateBuff[CCONV_RESULT_BUF_LEN];
	S8	RateString[2][CCONV_SEL_BUF_LEN];
	U8	CurrHiliteItem;
	U8	ClearResultFlag;
	U8	IsFromHistory;
//	U8	IsComputingResult;
}cconv_context_struct;

void StrToRate( CALOPERATOR_STRUCT *Rate, char * str);
void  RateToStr(char * str, CALOPERATOR_STRUCT *Rate);
#endif
//add by zough 061121 end

/* 
** Local Variable
*/

/* 
** Local Function
*/

/* 
** Global Variable
*/
cconv_context_struct *g_cconv_cntx = NULL;
extern wgui_inline_item wgui_inline_items[];
#if defined(__MMI_TOUCH_SCREEN__)
void pre_process_vk_hw(void);
extern void (*complete_inline_item_edit)(void);
extern void mmi_pen_editor_set_vk_hw_pre_handler(fnPtr	vk_pre,fnPtr hw_pre);
#endif

/* 
** Global Function
*/
extern void set_leading_zero(U8 u8type);
extern DOUBLE coolsand_UI_atof(UI_string_type s);
extern void  coolsand_UI_float_string(DOUBLE dob_val,S32 prec_val,S8* out_buffer);
extern S32 CalcComputePrecision(DOUBLE result, S16 max_digits);


#define UCONV_RATE_INPUT
/*****************************************************************************
* FUNCTION
*	CconvInit
* DESCRIPTION
*   Initialize currency converter application.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CconvInit(void)
{
	SetHiliteHandler(ORGANIZER_CURRENCYCONVERTOR_MENU,HighlightCConvMenu);
}


/*****************************************************************************
* FUNCTION
*	CconvDeInit
* DESCRIPTION
*   Free allocated memory.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CconvDeInit(void)
{
	if(g_cconv_cntx)
	{
		OslMfree(g_cconv_cntx);
		g_cconv_cntx = NULL;
	}
}

/*****************************************************************************
* FUNCTION
*	CconvExit
* DESCRIPTION
*   Free allocated memory.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CconvExit(void *param_p)
{
	if(g_cconv_cntx)
	{
		OslMfree(g_cconv_cntx);
		g_cconv_cntx = NULL;
	}
}

/*****************************************************************************
* FUNCTION
*	CconvPreEntryApp
* DESCRIPTION
*   Application memory and entry application.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CconvPreEntryApp(void)
{
	if(g_cconv_cntx)
		OslMfree(g_cconv_cntx);
	
	g_cconv_cntx = OslMalloc(sizeof(cconv_context_struct));
	memset(g_cconv_cntx, 0, sizeof(cconv_context_struct));

	EntryCconvApp();
}

/*****************************************************************************
* FUNCTION
*  HighlightCConvMenu
* DESCRIPTION
*   Highlight handler of Currency Converter menu item. 
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightCConvMenu(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(CconvPreEntryApp,KEY_EVENT_UP);
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(CconvPreEntryApp,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */	
}


/*****************************************************************************
* FUNCTION
*	HighlightCconvRateItem
* DESCRIPTION
*	Highlight handler of rate input inline item.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightCconvRateItem(S32 index)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(CconvPreEntryExchange, KEY_EVENT_UP);
}



/*****************************************************************************
* FUNCTION
*	EntryCconvApp
* DESCRIPTION
*   Display rate input screen of currency convertor application
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/


void EntryCconvApp()
{	
	//S32 precision;
	U8* guiBuffer = NULL;
	U8 *inputBuffer = NULL;
	U16 inputBufferSize = 0;
	S16 error = 0;
	U16 RskImg = STR_GLOBAL_BACK;

	#ifndef __MOD_ORGANIZER__

	#else
	char tempstr[OPERATOR_LEN + 1];
	memset(tempstr, 0, OPERATOR_LEN + 1);
	#endif
	
	EntryNewScreen(SCR_ID_CCONV_RATE_INPUT, ExitCConvApp, NULL, NULL);
	SetDelScrnIDCallbackHandler( SCR_ID_CCONV_RATE_INPUT, (HistoryDelCBPtr)CconvExit);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_CCONV_RATE_INPUT);

	InitializeCategory57Screen();

	if(guiBuffer)
		memset((void*)guiBuffer, 0, 2);
	
	inputBuffer = GetCurrNInputBuffer(SCR_ID_CCONV_COMPUTE, &inputBufferSize);    //added for inline edit history
	
	if(inputBuffer)                                            //added for inline edit history
	{
		#ifdef __MMI_MAINLCD_220X176__	
			SetCategory57Data(wgui_inline_items, 1, inputBuffer);//sets the data 
		#else
		SetCategory57Data(wgui_inline_items, 2, inputBuffer);//sets the data 
		#endif
	}

	#ifndef __MOD_ORGANIZER__
	if(g_cconv_cntx->IsFromHistory)
	{

		g_cconv_cntx->Rate=coolsand_UI_atof((U16*)g_cconv_cntx->RateBuff);
		if(pfnUnicodeStrlen(g_cconv_cntx->RateBuff))
			RskImg = STR_GLOBAL_CLEAR;
		else
			RskImg = STR_GLOBAL_BACK;
	}
	else
	{
		ReadValue(CURRENCY_CONVERTOR_EXCHANGE_RATE,&g_cconv_cntx->Rate,DS_DOUBLE,&error);
		g_cconv_cntx->IsFromHistory = TRUE;
	}
	#else
	if (g_cconv_cntx->IsFromHistory)
	{		
		UnicodeToAnsii((S8*) tempstr, g_cconv_cntx->RateBuff);
		if (pfnUnicodeStrlen(g_cconv_cntx->RateBuff))
			RskImg = STR_GLOBAL_CLEAR;
		else
			RskImg = STR_GLOBAL_BACK;
	}
	else
	{
		ReadValue(CURRENCY_CONVERTOR_EXCHANGE_RATE, tempstr, DS_DOUBLE, &error);		
		g_cconv_cntx->IsFromHistory = TRUE;
	}
	#endif

//	StrToRate(&g_cconv_cntx->Rate,tempstr);
	#ifndef __MOD_ORGANIZER__	//add by liuxn 060616
	if(g_cconv_cntx->Rate<=999999 && g_cconv_cntx->Rate>0)//o data has been written set the text to blank
	{
		precision = CalcComputePrecision(g_cconv_cntx->Rate,CCONV_MAX_RATE);
		coolsand_UI_float_string(g_cconv_cntx->Rate,precision,(S8*)g_cconv_cntx->RateBuff);
	}
	#else 		//add by liuxn 060616
		{
//			precision = CalcComputePrecision(g_cconv_cntx->Rate,CCONV_MAX_RATE);
//			coolsand_UI_float_string(g_cconv_cntx->Rate,precision,(S8*)g_cconv_cntx->RateBuff);
	StrToRate(&g_cconv_cntx->Rate,tempstr);
	AnsiiToUnicodeString(g_cconv_cntx->RateBuff,tempstr);
		}
	#endif		//add by liuxn 060616

#ifdef __MMI_MAINLCD_220X176__	
	SetInlineItemActivation(&wgui_inline_items[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&wgui_inline_items[0], (U8*)g_cconv_cntx->RateBuff, (CCONV_MAX_RATE+1), INPUT_TYPE_DECIMAL_NUMERIC);
	EnableInlineItemBoundary(&wgui_inline_items[0]);
	RightJustifyInlineItem(&wgui_inline_items[0]);
#else
	SetInlineItemCaption(&wgui_inline_items[0], (U8*) get_string(STR_CURRENCYCONVERTOR_RATE));

	SetInlineItemActivation(&wgui_inline_items[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&wgui_inline_items[1], (U8*)g_cconv_cntx->RateBuff, (CCONV_MAX_RATE+1), INPUT_TYPE_DECIMAL_NUMERIC);
	EnableInlineItemBoundary(&wgui_inline_items[1]);
	RightJustifyInlineItem(&wgui_inline_items[1]);
#endif

	set_leading_zero(FALSE);
	RegisterHighlightHandler(HighlightCconvRateItem);

#ifdef __MMI_MAINLCD_220X176__	
	ShowCategory257Screen(STR_CURRENCYCONV_INPUT, GetRootTitleIcon(ORGANIZER_CURRENCYCONVERTOR_MENU),
			STR_GLOBAL_OK,IMG_GLOBAL_OK,RskImg,
			IMG_GLOBAL_BACK,CURRENCY_CONVERTOR_IMAGE, 
			1,NULL,wgui_inline_items,0,g_cconv_cntx->RateBuff);
#else
	ShowCategory257Screen(STR_CURRENCYCONV_INPUT, GetRootTitleIcon(ORGANIZER_CURRENCYCONVERTOR_MENU),
			STR_GLOBAL_OK,IMG_GLOBAL_OK,RskImg,
			IMG_GLOBAL_BACK,CURRENCY_CONVERTOR_IMAGE, 
			2,NULL,wgui_inline_items,0,g_cconv_cntx->RateBuff);
#endif

	SetCategory57RightSoftkeyFunctions(CconvPreEntryExchange, GoBackHistory);
	SetLeftSoftkeyFunction(CconvPreEntryExchange,KEY_EVENT_UP);
}


	
/*****************************************************************************
* FUNCTION
*  ExitCConvApp
* DESCRIPTION
*   Display rate input screen of currency convertor application
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitCConvApp(void)
{
	U16 inputBufferSize;                //added for inline edit history
	history_t HistoryCCComputeCurrency;
	S16	nHistory = 0;

	set_leading_zero(TRUE);
	CloseCategory57Screen();       
	HistoryCCComputeCurrency.scrnID=SCR_ID_CCONV_RATE_INPUT;
	HistoryCCComputeCurrency.entryFuncPtr=EntryCconvApp;
	pfnUnicodeStrcpy((S8*)HistoryCCComputeCurrency.inputBuffer, (S8*)&nHistory);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*)HistoryCCComputeCurrency.inputBuffer);         //added for inline edit history
	AddNHistory(HistoryCCComputeCurrency, inputBufferSize);                       //added for inline edit history
}


#define UCONV_CONVERTING
/*****************************************************************************
* FUNCTION
*  CconvComputeResult
* DESCRIPTION
*   Convert the result and display.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616

void CconvComputeResult(void)
{	
	S32 precision;
	DOUBLE result,conversion_factor;
#if 0
/* under construction !*/
#endif
	CloseCategory57Screen();

	if((coolsand_UI_strlen((UI_string_type)g_cconv_cntx->LocalBuff)<1))
	{	
		memset(g_cconv_cntx->ForeBuff,0,15);
		DisplayPopup((U8*)GetString(ERROR_CURRENCYCONVERTOR_RATE1),IMG_GLOBAL_WARNING,0, UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
		return;
	}

	/* calculate result */
	if(g_cconv_cntx->CurrSelRate)/*1:R*/ 
		conversion_factor=g_cconv_cntx->Rate;
	else
		conversion_factor=(1/g_cconv_cntx->Rate);

	result =(DOUBLE)(conversion_factor * coolsand_UI_atof((UI_string_type)g_cconv_cntx->LocalBuff));

	precision = CalcComputePrecision(result, CCONV_MAX_RESULT);

	coolsand_UI_float_string(result, precision, (S8*)g_cconv_cntx->ForeBuff);

	g_cconv_cntx->ClearResultFlag = 1;	

#ifdef __MMI_CONVERTER_KEEP_INLINE_FOCUS__
	RedrawCategoryFunction();
#if 0
/* under construction !*/
	#if(UI_DOUBLE_BUFFER_SUPPORT)
/* under construction !*/
	#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#if defined(UI_DOUBLE_BUFFER_SUPPORT)
/* under construction !*/
/* under construction !*/
	#endif
	#endif
#else

	SetHighlightedItem(0);
#endif

	SetLeftSoftkeyFunction(CconvComputeResult, KEY_EVENT_UP);

}
//add by liuxn 060616 start
#else
void CconvComputeResult(void)
{
	//S32 precision;
	CALOPERATOR_STRUCT result, conversion_factor;
	S8 tmpstr1[OPERATOR_LEN + 1], tmpstr2[OPERATOR_LEN + 1], tmpstr3[OPERATOR_LEN + 1];
	S8 tmp_proportion[2][OPERATOR_LEN + 1];

	memset(tmpstr1, 0, OPERATOR_LEN + 1);
	memset(tmpstr2, 0, OPERATOR_LEN + 1);
	memset(tmpstr3, 0, OPERATOR_LEN + 1);
	memset(tmp_proportion, 0, (OPERATOR_LEN + 1) * 2); // add by yaosq 20070403 设置一个临时变量记录汇率状态,使其不发生变化
	
	memcpy(tmp_proportion[0], g_cconv_cntx->RateList[0], 2 * pfnUnicodeStrlen(g_cconv_cntx->RateList[0]));
#if 0
/* under construction !*/
#endif
	CloseCategory57Screen();

	if ((coolsand_UI_strlen((UI_string_type)g_cconv_cntx->LocalBuff) < 1))
	{	
		memset(g_cconv_cntx->ForeBuff, 0, 15);
		DisplayPopup((U8*)GetString(ERROR_CURRENCYCONVERTOR_RATE1), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		return;
	}

	UnicodeToAnsii(tmpstr2, (S8*)g_cconv_cntx->LocalBuff); 
	StrToRate(&result, tmpstr2);
	RateToStr(tmpstr3, &g_cconv_cntx->Rate);
	StrToRate(&conversion_factor, tmpstr3);
	/* calculate result */
	if (g_cconv_cntx->CurrSelRate)/*1:R*/ 
	{	
		CalculatorMult(&result, &conversion_factor);
	}
	else
	{
		CalculatorDivi(&result, &conversion_factor);
	}
//qiff modify 2008/10/28 for GS  BUG:10215
	Calresulttoberound(&result); 
#if 0
	if (!Judgeifover(result) )
	{
		g_calc_cntx->ResultState = RESULT_EXCEED;
		playRequestedTone(ERROR_TONE);	
	}
#endif
//qiff modify end
	RateToStr(tmpstr1, &result);
	AnsiiToUnicodeString(g_cconv_cntx->ForeBuff, tmpstr1);
    // FIXME That case doesn't exist.
	//g_cconv_cntx->ForeBuff[CCONV_RESULT_BUF_LEN]=0x00;
	g_cconv_cntx->ForeBuff[CCONV_RESULT_BUF_LEN-1]=0x00;
	g_cconv_cntx->ForeBuff[CCONV_RESULT_BUF_LEN-2]=0x00;
	memcpy(g_cconv_cntx->RateList[0], tmp_proportion[0], 2 * pfnUnicodeStrlen(tmp_proportion[0]));
	g_cconv_cntx->ClearResultFlag = 1;	

#ifdef __MMI_CONVERTER_KEEP_INLINE_FOCUS__
	RedrawCategoryFunction();
#else

	SetHighlightedItem(0);
	
#endif
	
	SetLeftSoftkeyFunction(CconvComputeResult, KEY_EVENT_UP);

}
#endif
//add by liuxn 060616 end

/*****************************************************************************
* FUNCTION
*	CconvPreEntryExchange
* DESCRIPTION
*   Pre-entry function for exchange screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616

void CconvPreEntryExchange(void) 
{
	S16 error;

	CloseCategory57Screen();

	g_cconv_cntx->Rate=coolsand_UI_atof((U16*)g_cconv_cntx->RateBuff/*Currency_Convertor_rate.text*/);

	if(coolsand_UI_strlen((UI_string_type)g_cconv_cntx->RateBuff)<1 || g_cconv_cntx->Rate==0 )
	{
		DisplayPopup((U8*)GetString(ERROR_CURRENCYCONVERTOR_RATE1),IMG_GLOBAL_WARNING,0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		return;
	}
	else		
	{
		g_cconv_cntx->CurrSelRate = 0;
		g_cconv_cntx->CurrHiliteItem = 0;
	 
		memset(g_cconv_cntx->LocalBuff, 0, ENCODING_LENGTH);
		memset(g_cconv_cntx->ForeBuff, 0, ENCODING_LENGTH);	
		
		WriteValue(CURRENCY_CONVERTOR_EXCHANGE_RATE,&g_cconv_cntx->Rate,DS_DOUBLE,&error);
		g_cconv_cntx->IsFromHistory = FALSE;
		EntryCConvExchange();
	}
}
//add by liuxn 060616 start
#else
void CconvPreEntryExchange(void) 
{
	S16 error = 0;
	S8 	tmpstr[OPERATOR_LEN+1];
	S8 	RateBufTesultCmp[OPERATOR_LEN + 1];
	memset(tmpstr, 0, OPERATOR_LEN+1);
	memset(RateBufTesultCmp, 0, OPERATOR_LEN + 1);

	CloseCategory57Screen();

	UnicodeToAnsii(tmpstr,(S8*)g_cconv_cntx->RateBuff);

	StrToRate(&g_cconv_cntx->Rate, tmpstr);

	memset(tmpstr, 0, OPERATOR_LEN+1);
	RateToStr(tmpstr, &g_cconv_cntx->Rate);
	AnsiiToUnicodeString(g_cconv_cntx->RateBuff,tmpstr);
	// added by yaosq 20070326 (memcmp(RateBufTesultCmp,g_cconv_cntx->Rate.operBuff,OPERATOR_LEN+1) == 0)
	if ((memcmp(RateBufTesultCmp, g_cconv_cntx->Rate.operBuff, OPERATOR_LEN + 1) == 0) || coolsand_UI_strlen((UI_string_type)g_cconv_cntx->RateBuff) < 1 || (g_cconv_cntx->Rate.Exponent == 1&&
		g_cconv_cntx->Rate.Fraction==0&&*(g_cconv_cntx->Rate.operBuff)==0))
	{
		DisplayPopup((U8*)GetString(ERROR_CURRENCYCONVERTOR_RATE1),IMG_GLOBAL_WARNING,0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		return;
	}
	else		
	{
		g_cconv_cntx->CurrSelRate = 0;
		g_cconv_cntx->CurrHiliteItem = 0;
	 
		memset(g_cconv_cntx->LocalBuff, 0, ENCODING_LENGTH);
		memset(g_cconv_cntx->ForeBuff, 0, ENCODING_LENGTH);	
		
		WriteValue(CURRENCY_CONVERTOR_EXCHANGE_RATE,tmpstr,DS_DOUBLE,&error);
		g_cconv_cntx->IsFromHistory = FALSE;
		EntryCConvExchange();
	}
}
#endif
//add by liuxn 060616 end

 
/*****************************************************************************
* FUNCTION
*	CConvFillInlineStruct
* DESCRIPTION
*   Fill inline structure of converting screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CConvFillInlineStruct(void)
{
//	S32 precision;
	U8 strTemp[]={':','\0','1','\0','\0','\0'};
	U8 strTemp2[]={'1','\0',':','\0','\0','\0'};

#ifndef __MOD_ORGANIZER__
	precision = CalcComputePrecision(g_cconv_cntx->Rate,6);

	coolsand_UI_float_string(g_cconv_cntx->Rate,precision,g_cconv_cntx->RateString[0]);
#else
	S8 tmpstr[OPERATOR_LEN + 1];
	memset(tmpstr, 0, OPERATOR_LEN + 1);
	RateToStr(tmpstr, &g_cconv_cntx->Rate);
	AnsiiToUnicodeString(g_cconv_cntx->RateString[0],tmpstr);
#endif

//	precision = CalcComputePrecision(g_cconv_cntx->Rate,6);

//	coolsand_UI_float_string(g_cconv_cntx->Rate,precision,g_cconv_cntx->RateString[0]);
	

	pfnUnicodeStrcpy((S8*)g_cconv_cntx->RateString[1],(S8*)strTemp2);				/* 1:R */	
	pfnUnicodeStrcat((S8*)g_cconv_cntx->RateString[1],(S8*)g_cconv_cntx->RateString[0]);	/* R:1*/		
	pfnUnicodeStrcat((S8*)g_cconv_cntx->RateString[0],(S8*)strTemp);
	
	g_cconv_cntx->RateList[0] = g_cconv_cntx->RateString[0];
	g_cconv_cntx->RateList[1] = g_cconv_cntx->RateString[1];

	set_leading_zero(FALSE);
	DisableInlineInputMethodHints();

	SetInlineItemActivation(&wgui_inline_items[CCONV_INLINE_RATE], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemSelect(&wgui_inline_items[CCONV_INLINE_RATE], 2, (U8 **)g_cconv_cntx->RateList,&g_cconv_cntx->CurrSelRate);
	RegisterInlineSelectHighlightHandler(&wgui_inline_items[CCONV_INLINE_RATE],HighlightCconvInlineSelHandler);

	SetInlineItemActivation(&wgui_inline_items[CCONV_INLINE_LOCAL_CAP], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[CCONV_INLINE_LOCAL_CAP], (U8*)GetString(STR_CURRENCYCONVERTOR_LOCAL));

	SetInlineItemActivation(&wgui_inline_items[CCONV_INLINE_LOCAL], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&wgui_inline_items[CCONV_INLINE_LOCAL], (U8*)g_cconv_cntx->LocalBuff, (CCONV_MAX_INPUT+1), INPUT_TYPE_DECIMAL_NUMERIC);
	SetInlineTextEditCustomFunction(&wgui_inline_items[CCONV_INLINE_LOCAL],CconvCustomInlineFunction);
	EnableInlineItemBoundary(&wgui_inline_items[CCONV_INLINE_LOCAL]);
	RightJustifyInlineItem(&wgui_inline_items[CCONV_INLINE_LOCAL]);

	SetInlineItemActivation(&wgui_inline_items[CCONV_INLINE_FORE_CAP], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[CCONV_INLINE_FORE_CAP], (U8*)(U8 *) GetString(STR_CURRENCYCONVERTOR_FOREIGN));

	SetInlineItemActivation(&wgui_inline_items[CCONV_INLINE_FORE], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&wgui_inline_items[CCONV_INLINE_FORE], (U8*)g_cconv_cntx->ForeBuff, (CCONV_MAX_INPUT+1), INPUT_TYPE_DECIMAL_NUMERIC);
	EnableInlineItemBoundary(&wgui_inline_items[CCONV_INLINE_FORE]);
	RightJustifyInlineItem(&wgui_inline_items[CCONV_INLINE_FORE]);
	DisableInlineItem(&wgui_inline_items[CCONV_INLINE_FORE],CCONV_INLINE_FORE);
}


/*****************************************************************************
* FUNCTION
*  EntryCConvExchange
* DESCRIPTION
*   Entry Function of Compute currency exchange
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryCConvExchange(void)
{
	U8 *guiBuffer;
	U8* inputBuffer;                       //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history
	
	EntryNewScreen(SCR_ID_CCONV_COMPUTE, ExitCConvExchange, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_CCONV_COMPUTE);
	
	InitializeCategory57Screen();

	CConvFillInlineStruct();

	RegisterHighlightHandler(HighlightCconvInlineItemHandler);

	if(guiBuffer)
		memset((void*)guiBuffer, 0, 2);
	
	inputBuffer = GetCurrNInputBuffer(SCR_ID_CCONV_COMPUTE, &inputBufferSize );    //added for inline edit history
	
	if(inputBuffer)                                            //added for inline edit history
		SetCategory57Data(wgui_inline_items, CCONV_INLINE_TOTAL, inputBuffer);//sets the data 
	
	ShowCategory57Screen(STR_CURRENCYCONVERTOR_INPUT, GetRootTitleIcon(ORGANIZER_CURRENCYCONVERTOR_MENU), 
	                     STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
						 CCONV_INLINE_TOTAL, NULL,wgui_inline_items, g_cconv_cntx->CurrHiliteItem, guiBuffer );
 
	DisableCategory57ScreenDone();
	SetLeftSoftkeyFunction(CconvComputeResult,KEY_EVENT_UP);
	SetCategory57RightSoftkeyFunctions(CconvComputeResult, GoBackHistory);
}


/*****************************************************************************
* FUNCTION
*  ExitCConvExchange
* DESCRIPTION
*   Entry Function of Compute currency exchange
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitCConvExchange(void)
{
	U16 inputBufferSize;                //added for inline edit history
	history_t HistoryCCComputeCurrency;
	S16	nHistory = 0;

	set_leading_zero(TRUE);
	CloseCategory57Screen();       
	HistoryCCComputeCurrency.scrnID=SCR_ID_CCONV_COMPUTE;
	HistoryCCComputeCurrency.entryFuncPtr=EntryCConvExchange;
	pfnUnicodeStrcpy((S8*)HistoryCCComputeCurrency.inputBuffer, (S8*)&nHistory);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*)HistoryCCComputeCurrency.inputBuffer);         //added for inline edit history
	AddNHistory(HistoryCCComputeCurrency, inputBufferSize);                       //added for inline edit history
}


/*****************************************************************************
* FUNCTION
*  HighlightCconvInlineSelHandler
* DESCRIPTION
*   Highlight handler of inline selection.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightCconvInlineSelHandler(S32 index) 
{
	memset(g_cconv_cntx->LocalBuff, 0, ENCODING_LENGTH);
	memset(g_cconv_cntx->ForeBuff, 0, ENCODING_LENGTH);
	RedrawCategoryFunction();
}


/*****************************************************************************
* FUNCTION
*	HighlightCconvInlineItemHandler
* DESCRIPTION
*   Highlight handler of inline item in exchange screen.
* PARAMETERS
*	index	IN	index of item.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightCconvInlineItemHandler(S32 index) 
{
	if(index == 0)
	{
//		if(!g_cconv_cntx->IsComputingResult)
//		{
			ClearKeyHandler(KEY_LSK,KEY_EVENT_UP);
			ChangeLeftSoftkey(STRING_ID_NULL, IMAGE_ID_NULL);
			ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
			SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
//		}
	}
	else	/* 2 */
	{
#ifdef __MMI_CONVERTER_KEEP_INLINE_FOCUS__
		if(g_cconv_cntx->CurrHiliteItem!=index)
		{
			memset(g_cconv_cntx->LocalBuff, 0, 2);
			memset(g_cconv_cntx->ForeBuff, 0, 2);
		}
#if 0  //qiff 2008/10/20 del for greenstone bug:10112		
		else if(!g_cconv_cntx->ClearResultFlag)
		{
			g_cconv_cntx->ClearResultFlag = 1;		
		}
#endif
#if !defined(__MMI_WITH_C_KEY__)
		SetCategory57ScreenRSKClear();	 
#endif


		ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(CconvComputeResult,KEY_EVENT_UP);
#else
		if(g_cconv_cntx->CurrHiliteItem!=index)
		{
			memset(g_cconv_cntx->LocalBuff, 0, 2);
			memset(g_cconv_cntx->ForeBuff, 0, 2);
		}
		ChangeLeftSoftkey( STR_GLOBAL_OK, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(CconvComputeResult,KEY_EVENT_UP);
#endif
			
	}
	g_cconv_cntx->CurrHiliteItem = (U8)index;
}
		

/*****************************************************************************
* FUNCTION
*  CconvCustomInlineFunction
* DESCRIPTION
*   Set group key handlers of inline editor
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void CconvCustomInlineFunction(void)
{
#ifdef __MMI_CONVERTER_KEEP_INLINE_FOCUS__
	U16 keyCodes[]={KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_POUND};
	U8	nKeyCodes=(U8)11;

	if(g_cconv_cntx->ClearResultFlag)
	{	
		g_cconv_cntx->ClearResultFlag = 0;
		SetGroupKeyHandler(CconvClearResult,keyCodes,nKeyCodes,KEY_EVENT_DOWN);
#if !defined(__MMI_WITH_C_KEY__)
		SetRightSoftkeyFunction(CconvClearResult, KEY_LONG_PRESS);
		SetRightSoftkeyFunction(CconvClearResult, KEY_EVENT_UP);
		SetRightSoftkeyFunction(NULL, KEY_EVENT_DOWN);
#else
		SetKeyHandler(CconvClearResult, KEY_CLEAR, KEY_EVENT_UP);
		SetKeyHandler(NULL, KEY_CLEAR, KEY_EVENT_DOWN);
#endif

#if defined(__MMI_TOUCH_SCREEN__)
		mmi_pen_editor_set_vk_hw_pre_handler(pre_process_vk_hw,pre_process_vk_hw );
#endif
	}
#endif 
}

 void mmi_conv_reset_rsk(void)
{
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
/*****************************************************************************
* FUNCTION
*  CconvClearResult
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
void CconvClearResult(void)
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
void pre_process_vk_hw(void)
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
#endif
#ifndef __MOD_ORGANIZER__	

#else
/**************************************************************

	FUNCTION NAME		: StrToRate

  	PURPOSE				: convert string into CALOPERATOR_STRUCT *

	INPUT PARAMETERS	: CALOPERATOR_STRUCT *Rate, char * str
	
	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/
 void StrToRate( CALOPERATOR_STRUCT *Rate, char * str)
{
	S32 i , j = 0;	
	BOOL flag;
	ASSERT(Rate != NULL);
	ASSERT(str != NULL);
	Rate->Symbol = OPERATORSYMBOL_POST;
	flag = FALSE;
	Rate->Exponent = 0;
	Rate->Fraction = 0;
	memset(Rate->operBuff, 0, OPERATOR_LEN + 1);
	
	for (i = 0; i < OPERATOR_LEN; i++)
	{			
		if ((*(str + i)) == '.')
		{
			flag = TRUE;
			continue ;
		}	
	
		if (*(str + i) == '\0')
		{
			break ;
		}
		
		if (!flag)
		{
			Rate->Exponent++;
		}
		else
		{
			Rate->Fraction++;
		}
		
		Rate->operBuff[j] = str[i] - '0';
		j++;
	}
	
      for( ; Rate->Fraction>0; )
      	{
			if(Rate->operBuff[--j] == 0)
			{
				Rate->operBuff[j] = '\0' ;
				Rate->Fraction-- ;
			}
			else
			 break;
	  }
}

/**************************************************************

	FUNCTION NAME		: StrToRate

  	PURPOSE				: convert CALOPERATOR_STRUCT * into string

	INPUT PARAMETERS	: char * str, CALOPERATOR_STRUCT *Rate
	
	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

 

**************************************************************/

void RateToStr(char *str, CALOPERATOR_STRUCT *Rate)
{
  	S32 i = 0, j = 0;
 	char *pStr = NULL;
	
 	ASSERT(Rate != NULL);
	ASSERT(str != NULL); 	

	pStr = str;
	if(Rate->Exponent <= 0) //qiff modify 2008/10/28 for bug 10186
	{
		*pStr = '0';
  		pStr++;
	}
	else
	{
	  	for (i = 0; i < Rate->Exponent; i++)
	  	{
	  		*pStr = Rate->operBuff[i] + '0';
			pStr++;
	  		j++;
	  	}
	}

  	if (Rate->Fraction>0)
  	{
  		*pStr = '.';
		pStr++;	
  		j++;
  		
  		for (i = 0; (i < Rate->Fraction)&&(j < 14); i++) //qiff modify for protect
  		{
  			*pStr = Rate->operBuff[j - 1] + '0';
			pStr++;
  			j++;
	  	}
  	}		
}
#endif

#endif //#ifndef _MMI_CURRENCYCONVRETOR_C


