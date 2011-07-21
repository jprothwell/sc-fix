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
 *	Bmi.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file implements BMI application
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 /*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2003
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *	Bmi.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements BMI application.
 *
 * Author:
 * -------
 * -------
 *
 *****************************************************************************/
 /*  Include: MMI header file */
 #ifndef _MMI_BMI_C
 #define _MMI_BMI_C
#include "mmi_features.h"
#ifdef __MMI_BMI__

#include "historygprot.h"
#include "globaldefs.h" 
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "unicodexdcl.h"
#include "bmi.h"
#include "settingprofile.h"
#include "organizerdef.h"
#include "commonscreens.h"
#include "operator.h"

/*  Include: PS header file */

/* 
** Define
*/
#define	BMI_MALE_LB					20	/* Lower bound of standard BMI index of a man */
#define	BMI_MALE_UB					25	/* Upper bound of standard BMI index of a man */
#define	BMI_FEMALE_LB				18	/* Lower bound of standard BMI index of a woman */
#define	BMI_FEMALE_UB				23	/* Upper bound of standard BMI index of a woman */
//add by zough 061121 start
#define	BMI_MAX_INPUT				5
//add by zough 061121 end
#define	BMI_DIGIT_LEN				9*ENCODING_LENGTH
#define	BMI_LB_UB_LEN				4*ENCODING_LENGTH	/* buffer length to store lower and upper bound */
#define	BMI_RESULT_LEN				7*ENCODING_LENGTH	/* buffer length to store resut xxx.xx */
#define	BMI_MAX_HEIGHT				300
#define	BMI_MIN_HEIGHT				55
#define	BMI_MAX_WEIGHT				300
#define	BMI_MIN_WEIGHT				1
/* 
** Typedef 
*/
//add by zough 061121 start
#ifndef __MOD_ORGANIZER__	
typedef struct
{
	DOUBLE	Height;
	DOUBLE	Weight;
	S32		Gender;
	U8 		*GenderList[2];
	S8 		UBBuf[BMI_LB_UB_LEN];
	S8 		LBBuf[BMI_LB_UB_LEN];
	S8 		ResultBuf[BMI_RESULT_LEN];
	S8		HeightBuf[BMI_DIGIT_LEN];
	S8		WeightBuf[BMI_DIGIT_LEN];
	U8		CurrHilite;
}bmi_context_struct;
#else
typedef struct
{
//	DOUBLE	Height;
//	DOUBLE	Weight;
	CALOPERATOR_STRUCT	Height;
	CALOPERATOR_STRUCT	Weight;
	S32		Gender;
	U8 		*GenderList[2];
	S8 		UBBuf[BMI_LB_UB_LEN];
	S8 		LBBuf[BMI_LB_UB_LEN];
	S8 		ResultBuf[BMI_RESULT_LEN];
	S8		HeightBuf[BMI_DIGIT_LEN];
	S8		WeightBuf[BMI_DIGIT_LEN];
	U8		CurrHilite;
}bmi_context_struct;
extern void StrToRate(CALOPERATOR_STRUCT *Rate, char  *str);
extern void RateToStr(char *str, CALOPERATOR_STRUCT *Rate);
//add by zough 061121 end
#endif

/* 
** Local Variable
*/


/* 
** Local Function
*/

/* 
** Global Variable
*/

bmi_context_struct *g_bmi_cntx = NULL;
extern wgui_inline_item wgui_inline_items[];

/* 
** Global Function
*/
extern void  coolsand_UI_float_string(DOUBLE dob_val,S32 prec_val, S8* out_buffer);
extern void set_leading_zero(U8 u8type);

void EntryBMIInputScreen(void) ;
void BmiFreeMemAndGoBack(void);

/*****************************************************************************
* FUNCTION
*  HighlightBMIMenu
* DESCRIPTION
*   Highlight handler of BMI menu item. 
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightBMIMenu(void) 
{
#if 0
	SetLeftSoftkeyFunction(EntryBMIWelcomeScreen,KEY_EVENT_UP);
	SetKeyHandler(EntryBMIWelcomeScreen,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(EntryBMIInputScreen ,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(EntryBMIInputScreen,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif
}


/*****************************************************************************
* FUNCTION
*	BmiDeInit
* DESCRIPTION
*   Free allocated memory.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void BmiDeInit(void)
{
	if(g_bmi_cntx)
	{
		OslMfree(g_bmi_cntx);
		g_bmi_cntx = NULL;
	}
}

/*****************************************************************************
* FUNCTION
*	BmiExit
* DESCRIPTION
*   Free allocated memory.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void BmiExit(void *param_p)
{
	if(g_bmi_cntx)
	{
		OslMfree(g_bmi_cntx);
		g_bmi_cntx = NULL;
	}
}
/*****************************************************************************
* FUNCTION
*	BmiFreeMemAndGoBack
* DESCRIPTION
*   Free allocated memory and go back to previous screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void BmiFreeMemAndGoBack(void)
{
	BmiDeInit();
	GoBackHistory();
}

//add by zough 061121 start
/*****************************************************************************
* FUNCTION
*	CompFloat
* DESCRIPTION
*   	compute two float data, when operator1>operator2 return value >0, operator1=operator2, return 0, else value <0
* PARAMETERS
*	operator1  IN
*    	operator2  IN
* RETURNS
*	S32.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
S32 CompFloat(CALOPERATOR_STRUCT *operator1, CALOPERATOR_STRUCT *operator2 )
{
	S8 i,  j, retval = 0;	

//	ASSERT(operator1 != NULL);
//	ASSERT(operator2 != NULL);
	
	if ((operator1->Symbol - operator2->Symbol)<0)
	{
		return -1;
	}
	else if ((operator1->Symbol - operator2->Symbol)>0)
	{
		return 1;
	}
	
		
	if (operator1->Exponent < operator2->Exponent)
	{
		return -1;	
	}
	else if (operator1->Exponent > operator2->Exponent)
	{
		return 1;
	}

	for (i = 0; i < operator1->Exponent; i++)
	{
		retval = *(operator1->operBuff + i) - *(operator2->operBuff + i);
		if (retval != 0)
		{
			return retval;
		}
	}	

	if(operator1->Fraction > operator2->Fraction)
	{
		j = operator1->Fraction;
	}
	else 
	{
		j = operator2->Fraction;
	}

	for(i = 0; i < j; i++)
	{
		retval = *(operator1->operBuff +  i)-*(operator2->operBuff + i);
		if(retval != 0)
		{
			return retval;
		}
	}	

	return retval;
	
}
//add by zough 061121 end

/*****************************************************************************
* FUNCTION
*	EntryBMIWelcomeScreen
* DESCRIPTION
*   Display welcome screen of BMI application
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryBMIWelcomeScreen(void) 
{
	U8 *guiBuffer;

	EntryNewScreen(SCR_BMI_WELCOME, NULL, EntryBMIWelcomeScreen, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_BMI_WELCOME);

	if(g_bmi_cntx == NULL)
		g_bmi_cntx = OslMalloc(sizeof(bmi_context_struct));
	
	memset(g_bmi_cntx, 0, sizeof(bmi_context_struct));

	ShowCategory129Screen((U8*)get_string(STR_BMI_WELCOME_CAPTION), GetRootTitleIcon(ORGANIZER_HEALTH_MENU), 
					STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
					IMG_BMI_WELCOME_SCREEN, guiBuffer);
	
	SetLeftSoftkeyFunction(EntryBMIInputScreen ,KEY_EVENT_UP);
	SetRightSoftkeyFunction(BmiFreeMemAndGoBack,KEY_EVENT_UP);
	
	#ifdef __MMI_NOKIA_STYLE_N800__
        ClearKeyHandler( KEY_LSK,  KEY_EVENT_DOWN);
        ClearKeyHandler( KEY_RSK,  KEY_EVENT_DOWN);
        SetKeyHandler(EntryBMIInputScreen , KEY_LSK,  KEY_EVENT_UP);
        SetKeyHandler(BmiFreeMemAndGoBack , KEY_RSK,  KEY_EVENT_UP);
	#endif
}


/*****************************************************************************
* FUNCTION
*	EntryBMIInputScreen
* DESCRIPTION
*   Display inline editor screen for entering gender, height, and weight
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryBMIInputScreen(void) 
{
	U8 *guiBuffer;
	U8* inputBuffer;                       //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history
	U16 BMIImageList[6] = {0,0,0,0,0,0};

	//to malloc memory
	if(g_bmi_cntx == NULL)
	{
		g_bmi_cntx = OslMalloc(sizeof(bmi_context_struct));
		memset(g_bmi_cntx, 0, sizeof(bmi_context_struct));
	}		
	//
	
	EntryNewScreen(SCR_BMI_INPUT, ExitBMIInputScreen, NULL, NULL);
	SetDelScrnIDCallbackHandler(SCR_BMI_INPUT, (HistoryDelCBPtr)BmiExit);

	InitializeCategory57Screen();							

	guiBuffer = GetCurrGuiBuffer(SCR_BMI_INPUT);

	BMIFillInlineStruct();

	RegisterHighlightHandler(HighlightBMIInlineEditor);

	if(g_bmi_cntx->Gender == 0)
		BMIImageList[ENUM_GENDER] = IMG_BMI_BLUE_FLOWER;
	else
		BMIImageList[ENUM_GENDER] = IMG_BMI_RED_FLOWER;

	inputBuffer = GetCurrNInputBuffer(SCR_BMI_INPUT, &inputBufferSize);    //added for inline edit history
	
	if(inputBuffer!=NULL)                                            //added for inline edit history
		SetCategory57Data(wgui_inline_items, ENUM_INLINE_TOTAL, inputBuffer);//sets the data 

 	DisableCategory57ScreenDone();
 	
 	#ifdef __MMI_SLIM_LISTMENU_NUMBER__ 
 		wgui_override_list_menu_slim_style(WGUI_LIST_MENU_SLIM_STYLE_DRAW_ICON);
 	#endif
 
	 
 	//Category57AppendTopImage(IMG_BMI_WELCOME_SCREEN, TRUE);
 	 
	ShowCategory57Screen(STR_BMI_INPUT_CAPTION,GetRootTitleIcon(ORGANIZER_HEALTH_MENU), 
	                     STR_GLOBAL_OK, IMG_GLOBAL_OK, 
	                     STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
						 ENUM_INLINE_TOTAL, 
						 BMIImageList,
						 wgui_inline_items, 
						 g_bmi_cntx->CurrHilite, 
						 guiBuffer);
 
	SetCategory57RightSoftkeyFunctions(BmiComputeResult, GoBackHistory);

}


/*****************************************************************************
* FUNCTION
*	ExitBMIInputScreen
* DESCRIPTION
*   Exit handler of EntryBMIInputScreen
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitBMIInputScreen(void) 
{
	history_t h;
	U16 inputBufferSize;                			//added for inline edit history
	U16 nHistory = 0;
	set_leading_zero(TRUE);
	CloseCategory57Screen();       
	h.scrnID = SCR_BMI_INPUT;
	h.entryFuncPtr = EntryBMIInputScreen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(h.guiBuffer);

	inputBufferSize = (U16)GetCategory57DataSize();	//added for inline edit history
	GetCategory57Data ((U8*)h.inputBuffer);			//added for inline edit history
	AddNHistory(h, inputBufferSize);				//added for inline edit history
	
	#ifdef __MMI_SLIM_LISTMENU_NUMBER__ 
 		wgui_restore_list_menu_slim_style();
 	#endif
}

/*****************************************************************************
* FUNCTION
*	BMIFillInlineStruct
* DESCRIPTION
*   Initialize inline structure of input screen.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void BMIFillInlineStruct(void)
{
	set_leading_zero(FALSE);

	g_bmi_cntx->GenderList[0] = (U8*) GetString(STR_BMI_MALE_TEXT);
	g_bmi_cntx->GenderList[1] = (U8*) GetString(STR_BMI_FEMALE_TEXT);
	
	/* gender */
	SetInlineItemActivation(&wgui_inline_items[ENUM_GENDER], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemSelect(&wgui_inline_items[ENUM_GENDER], 2, (U8 **)g_bmi_cntx->GenderList,&g_bmi_cntx->Gender);
	RegisterInlineSelectHighlightHandler(&wgui_inline_items[ENUM_GENDER],HighlightBMIGender);

	/* height caption */
	SetInlineItemActivation(&wgui_inline_items[ENUM_HEIGHT_CAPTION], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[ENUM_HEIGHT_CAPTION], (U8*) GetString(STR_BMI_HEIGHT_TEXT));
	SetInlineItemFullWidth(&wgui_inline_items[ENUM_HEIGHT_CAPTION]);

	/* height */
	SetInlineItemActivation(&wgui_inline_items[ENUM_HEIGHT], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&wgui_inline_items[ENUM_HEIGHT], (U8*)g_bmi_cntx->HeightBuf, (BMI_MAX_INPUT+1), INPUT_TYPE_DECIMAL_NUMERIC);
	RightJustifyInlineItem(&wgui_inline_items[ENUM_HEIGHT]);
	SetInlineItemFullWidth(&wgui_inline_items[ENUM_HEIGHT]);
	EnableInlineItemBoundary(&wgui_inline_items[ENUM_HEIGHT]);

	/* weight caption */
	SetInlineItemActivation(&wgui_inline_items[ENUM_WEIGHT_CAPTION], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[ENUM_WEIGHT_CAPTION], (U8*)GetString(STR_BMI_WEIGHT_TEXT));
	SetInlineItemFullWidth(&wgui_inline_items[ENUM_WEIGHT_CAPTION]);

	/* weight */
	SetInlineItemActivation(&wgui_inline_items[ENUM_WEIGHT], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemTextEdit(&wgui_inline_items[ENUM_WEIGHT], (U8*)g_bmi_cntx->WeightBuf, (BMI_MAX_INPUT+1), INPUT_TYPE_DECIMAL_NUMERIC);
	RightJustifyInlineItem(&wgui_inline_items[ENUM_WEIGHT]);
	SetInlineItemFullWidth(&wgui_inline_items[ENUM_WEIGHT]);
	EnableInlineItemBoundary(&wgui_inline_items[ENUM_WEIGHT]);
}


/*****************************************************************************
* FUNCTION
*	EntryBMIResultScreen
* DESCRIPTION
*   Display result screen of calculation
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616

void EntryBMIResultScreen(void) 
{
	U8 *guiBuffer;
	DOUBLE bmiValue;
	U8 * bmiBuf;
	U8 percentageValue;
	U16 FolwerIndex;
	U8 LB,UB;

	memset(g_bmi_cntx->ResultBuf,0,sizeof(g_bmi_cntx->ResultBuf));

	guiBuffer = GetCurrGuiBuffer(SCR_BMI_RESULT);

	EntryNewScreen(SCR_BMI_RESULT, NULL, EntryBMIResultScreen, NULL);

	bmiValue = g_bmi_cntx->Weight/(g_bmi_cntx->Height*g_bmi_cntx->Height)*10000;

	coolsand_UI_float_string(bmiValue, 2, g_bmi_cntx->ResultBuf);

	if(g_bmi_cntx->Gender == 0)
	{
		LB = BMI_MALE_LB;
		UB = BMI_MALE_UB;
		FolwerIndex = IMG_BMI_BLUE_FLOWER;
	}
	else
	{
		LB = BMI_FEMALE_LB;
		UB = BMI_FEMALE_UB;
		FolwerIndex = IMG_BMI_RED_FLOWER;
	}

	coolsand_UI_itoa(LB, (U16*)g_bmi_cntx->LBBuf, 10);
	coolsand_UI_itoa(UB, (U16*)g_bmi_cntx->UBBuf, 10);
	
	if(bmiValue > UB)		
	{
		bmiBuf = (U8*) GetString(STR_BMI_FAT);
		percentageValue = 2;
	}
	else if(bmiValue < LB)
	{
		bmiBuf = (U8*) GetString(STR_BMI_THIN);
		percentageValue = 0;
	}
	else
	{
		bmiBuf = (U8*) GetString(STR_BMI_NORMAL);
		percentageValue = 1;
	}

	ShowCategory122Screen(STR_BMI_RESULT_CAPTION, GetRootTitleIcon(ORGANIZER_HEALTH_MENU), 
					0, 0, 
					STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
					STR_BMI_BMI_TEXT,percentageValue,
					guiBuffer, g_bmi_cntx->ResultBuf,bmiBuf,
					FolwerIndex,(U8*)g_bmi_cntx->LBBuf,(U8*)g_bmi_cntx->UBBuf);
	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
//add by liuxn 060616 start
#else
void EntryBMIResultScreen(void) 
{
	//add by zough 061121 start
	U8 *guiBuffer;
	S8 calculatestr[OPERATOR_LEN+1];
	S8 resultstr[OPERATOR_LEN+1];
	S8 LBstr[OPERATOR_LEN+1];
	S8 UBstr[OPERATOR_LEN+1];
	CALOPERATOR_STRUCT LBValue;
	CALOPERATOR_STRUCT UBValue;
	CALOPERATOR_STRUCT calculate;
	CALOPERATOR_STRUCT result;
	U8 *bmiBuf;
	U8 percentageValue;
	U16 FolwerIndex;
	U8 LB, UB;
    S8 UnDispLen;

	memset(calculatestr, 0, OPERATOR_LEN+1);
	memset(resultstr, 0, OPERATOR_LEN+1);
	
	RateToStr(resultstr, &g_bmi_cntx->Weight);
	StrToRate(&result, resultstr);

	memset(&calculatestr, 0, OPERATOR_LEN+1);
	*calculatestr = '1';
	*(calculatestr + 1) = '0';
	*(calculatestr + 2) = '0';
	*(calculatestr + 3) = '0';
	*(calculatestr + 4) = '0';
	StrToRate(&calculate, calculatestr);
	CalculatorMult(&result, &calculate);

	memset(g_bmi_cntx->ResultBuf, 0, sizeof(g_bmi_cntx->ResultBuf));

	guiBuffer = GetCurrGuiBuffer(SCR_BMI_RESULT);

	EntryNewScreen(SCR_BMI_RESULT, NULL, EntryBMIResultScreen, NULL);
	memset(&calculatestr, 0, OPERATOR_LEN+1);
	RateToStr(calculatestr, &g_bmi_cntx->Height);
	StrToRate(&calculate, calculatestr);
	CalculatorDivi(&result, &calculate);

	memset(&calculatestr, 0, OPERATOR_LEN+1);
	RateToStr(calculatestr, &g_bmi_cntx->Height);
	StrToRate(&calculate, calculatestr);
	CalculatorDivi(&result, &calculate);

	RateToStr(resultstr, &result);
        //add by zough 061207 start
        if ((OPERATOR_LEN - 6) > 0)
        {
            for (UnDispLen = 6; UnDispLen <= OPERATOR_LEN; UnDispLen++)
            {
                *(resultstr + UnDispLen) = '\0';
            }
            
        }
         //add by zough 061207 end
            
//	*(resultstr + OPERATOR_LEN) = '\0';
//	*(resultstr + OPERATOR_LEN-1) = '\0';
	AnsiiToUnicodeString((S8*)&g_bmi_cntx->ResultBuf, resultstr);

	if (g_bmi_cntx->Gender == 0)
	{
		LB = BMI_MALE_LB;
		UB = BMI_MALE_UB;
		FolwerIndex = IMG_BMI_BLUE_FLOWER;
	}
	else
	{
		LB = BMI_FEMALE_LB;
		UB = BMI_FEMALE_UB;
		FolwerIndex = IMG_BMI_RED_FLOWER;
	}

	coolsand_UI_itoa(LB, (U16*)g_bmi_cntx->LBBuf, 10);
	coolsand_UI_itoa(UB, (U16*)g_bmi_cntx->UBBuf, 10);

	UnicodeToAnsii(LBstr, g_bmi_cntx->LBBuf);
	UnicodeToAnsii(UBstr, g_bmi_cntx->UBBuf);

	StrToRate(&UBValue, UBstr);
	StrToRate(&LBValue, LBstr);

	if (CompFloat(&result, &UBValue) > 0)		
	{
		bmiBuf = (U8*) GetString(STR_BMI_FAT);
		percentageValue = 2;
	}
	else if (CompFloat(&result, &LBValue) < 0)
	{
		bmiBuf = (U8*) GetString(STR_BMI_THIN);
		percentageValue = 0;
	}
	else
	{
		bmiBuf = (U8*) GetString(STR_BMI_NORMAL);
		percentageValue = 1;
	}

	ShowCategory122Screen(STR_BMI_RESULT_CAPTION, GetRootTitleIcon(ORGANIZER_HEALTH_MENU), 
					0, 0, 
					STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
					STR_BMI_BMI_TEXT, percentageValue,
					guiBuffer, g_bmi_cntx->ResultBuf, bmiBuf,
					FolwerIndex, (U8*)g_bmi_cntx->LBBuf, (U8*)g_bmi_cntx->UBBuf);
	
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	//add by zough 061121 end
}
#endif
//add by liuxn 060616 end


/*****************************************************************************
* FUNCTION
*	HighlightBMIInlineEditor
* DESCRIPTION
*   Highlight handler of inline editor in input screen.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightBMIInlineEditor(S32 index) 
{
	g_bmi_cntx->CurrHilite = (U8)index;
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(BmiComputeResult,KEY_EVENT_UP);
	return;
}


/*****************************************************************************
* FUNCTION
*	HighlightBMIGender
* DESCRIPTION
*   Highlight handler of Gender item in input screen.
*	Change display folwer when gender changed.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightBMIGender(S32 index) 
{
	if( g_bmi_cntx->Gender == 0)
		Category57ChangeItemIcon(0,IMG_BMI_BLUE_FLOWER);
	else
		Category57ChangeItemIcon(0,IMG_BMI_RED_FLOWER);
	
	RedrawCategory57Screen();
}


/*****************************************************************************
* FUNCTION
*	BmiComputeResult
* DESCRIPTION
*   Validate correctness of input data and calcalute result.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616

void BmiComputeResult(void)
{
	S8	temp_buf[100];

	CloseCategory57Screen();     
	
	UnicodeToAnsii(temp_buf,(S8*)g_bmi_cntx->HeightBuf);
	g_bmi_cntx->Height = atof(temp_buf);
	
	UnicodeToAnsii(temp_buf, (S8*)g_bmi_cntx->WeightBuf);
	g_bmi_cntx->Weight = atof(temp_buf);

	if(((g_bmi_cntx->Height < BMI_MIN_HEIGHT) || (g_bmi_cntx->Height > BMI_MAX_HEIGHT)) &&
		((g_bmi_cntx->Weight < BMI_MIN_WEIGHT)  || (g_bmi_cntx->Weight > BMI_MAX_WEIGHT)))
	{
		DisplayPopup((U8*)GetString(STR_BMI_HEIGHT_WEIGHT_ERROR) ,IMG_GLOBAL_WARNING, 0,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
		g_bmi_cntx->CurrHilite = 2;
		return;
	}
	else if((g_bmi_cntx->Height < BMI_MIN_HEIGHT) || (g_bmi_cntx->Height > BMI_MAX_HEIGHT))
	{
		DisplayPopup((U8*)GetString(STR_BMI_HEIGHT_ERROR_TEXT) ,IMG_GLOBAL_WARNING, 0,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
		g_bmi_cntx->CurrHilite = 2;
		return;
	}
	else if((g_bmi_cntx->Weight < BMI_MIN_WEIGHT)  || (g_bmi_cntx->Weight > BMI_MAX_WEIGHT))
	{
		DisplayPopup((U8*)GetString(STR_BMI_WEIGHT_ERROR_TEXT) ,IMG_GLOBAL_WARNING, 0,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
		g_bmi_cntx->CurrHilite = 4;
		return;
	}
	
	g_bmi_cntx->CurrHilite = 0;
	EntryBMIResultScreen();
}
//add by liuxn 060616 start
#else
void BmiComputeResult(void)
{
	//add by zough 061121 start
	CALOPERATOR_STRUCT MaxHeight, MinHeight, MaxWeight, MinWeight;
	
	S8	temp_buf[OPERATOR_LEN + 1];
	S8	RateTemp_buf[OPERATOR_LEN + 1];
	S8   temp_unicode[OPERATOR_LEN * 2];

	CloseCategory57Screen();     
	memset(RateTemp_buf, 0 , OPERATOR_LEN + 1);

	memset(temp_buf, 0, OPERATOR_LEN + 1);
	UnicodeToAnsii(temp_buf, (S8*)g_bmi_cntx->HeightBuf);
	StrToRate(&g_bmi_cntx->Height, temp_buf);

	memset(temp_buf, 0, OPERATOR_LEN + 1);
	UnicodeToAnsii(temp_buf, (S8*)g_bmi_cntx->WeightBuf);
	StrToRate(&g_bmi_cntx->Weight, temp_buf);

	memset(temp_buf, 0, OPERATOR_LEN + 1);
	memset(temp_unicode, 0, OPERATOR_LEN * 2);
	coolsand_UI_itoa(BMI_MIN_HEIGHT, (U16*)temp_unicode, 10);
	UnicodeToAnsii(temp_buf, (S8*)temp_unicode);
	StrToRate(&MinHeight, temp_buf);

	memset(temp_buf, 0, OPERATOR_LEN + 1);
	memset(temp_unicode, 0, OPERATOR_LEN * 2);
	coolsand_UI_itoa(BMI_MAX_HEIGHT, (U16*)temp_unicode, 10);
	UnicodeToAnsii(temp_buf, (S8*)temp_unicode);
	StrToRate(&MaxHeight, temp_buf);

	memset(temp_buf, 0, OPERATOR_LEN + 1);
	memset(temp_unicode, 0, OPERATOR_LEN * 2);
	coolsand_UI_itoa(BMI_MIN_WEIGHT, (U16*)temp_unicode, 10);
	UnicodeToAnsii(temp_buf, (S8*)temp_unicode);
	StrToRate(&MinWeight, temp_buf);

	memset(temp_buf, 0, OPERATOR_LEN + 1);
	memset(temp_unicode, 0, OPERATOR_LEN * 2);
	coolsand_UI_itoa(BMI_MIN_HEIGHT, (U16*)temp_unicode, 10);
	UnicodeToAnsii(temp_buf, (S8*)temp_unicode);
	StrToRate(&MaxWeight, temp_buf);

	if (((memcmp(RateTemp_buf, g_bmi_cntx->Weight.operBuff, OPERATOR_LEN + 1) == 0 ) &&
		(memcmp(RateTemp_buf, g_bmi_cntx->Height.operBuff, OPERATOR_LEN + 1) == 0) ) ||
		(((CompFloat(&g_bmi_cntx->Height, &MinHeight) < 0) || (CompFloat(&g_bmi_cntx->Height, &MaxHeight) > 0)) &&
		((CompFloat(&g_bmi_cntx->Weight, &MinWeight) < 0)  || (CompFloat(&g_bmi_cntx->Weight, &MaxHeight) > 0))))
	{
		DisplayPopup((U8*)GetString(STR_BMI_HEIGHT_WEIGHT_ERROR), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		g_bmi_cntx->CurrHilite = 2;
		return;
	}
	else if (((memcmp(RateTemp_buf, g_bmi_cntx->Weight.operBuff, OPERATOR_LEN + 1) != 0 ) && (memcmp(RateTemp_buf, g_bmi_cntx->Height.operBuff, OPERATOR_LEN + 1) == 0) ) ||(CompFloat(&g_bmi_cntx->Height, &MinHeight) < 0) || (CompFloat(&g_bmi_cntx->Height, &MaxHeight) > 0))
	{
		DisplayPopup((U8*)GetString(STR_BMI_HEIGHT_ERROR_TEXT), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		g_bmi_cntx->CurrHilite = 2;
		return;
	}
	else if (((memcmp(RateTemp_buf, g_bmi_cntx->Weight.operBuff, OPERATOR_LEN + 1) == 0 ) && (memcmp(RateTemp_buf, g_bmi_cntx->Height.operBuff, OPERATOR_LEN + 1) != 0) ) ||(CompFloat(&g_bmi_cntx->Weight, &MinWeight) < 0)  || (CompFloat(&g_bmi_cntx->Weight, &MaxHeight) > 0))
	{
		DisplayPopup((U8*)GetString(STR_BMI_WEIGHT_ERROR_TEXT), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		g_bmi_cntx->CurrHilite = 4;
		return;
	}
	
	g_bmi_cntx->CurrHilite = 0;
	EntryBMIResultScreen();
	//add by zough  061121 end
}
#endif
//add by liuxn 060616 end


#endif // #ifdef __MMI_BMI__
#endif  //#ifndef _MMI_BMI_C


