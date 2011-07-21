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
 *  PreferedInputMethodSetting.c
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/***************** Settings Application implementation *********************/

/**************************************************************

    FILENAME    : PreferedInputMethodSetting.c

    PURPOSE     : PreferedInputMethod application

 

 

    DATE        : 21 Oct 2003 (Taipei)

**************************************************************/

#include "mmi_features.h"

#ifdef __MMI_PREFER_INPUT_METHOD__

#include "mmi_data_types.h"
#include "settingprot.h"
#include "settingdefs.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "globaldefs.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "unicodexdcl.h"
#include "hardwareevents.h"
#include "thrdosinterface.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "osthreadinterfacegprot.h"
#include "mainmenudef.h"
#include "settinggprots.h"
#include "settingsgexdcl.h"
#include "settingprofile.h"
#include "preferedinputmethodprot.h"

#include "eventsgprot.h"
#include "commonscreens.h"
 
#include "imeres.h"
extern const sIMEModeDetails *gIMEModeArray;
U16 MMI_all_prefered_input_mode_set[INPUT_MODE_MAX_NUM] = {0xffff};
U8*  inputMethodList[INPUT_MODE_MAX_NUM];

 
static U16   gnInputMethods =   0;
U16   currInputMethodIndex  =   0;

extern s16     MMI_prefered_input_type;
 
extern PS8 (*pfnUnicodeStrcpy)(S8 *strDestination, const S8 *strSource );
//extern void DisplayPopup(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId);
 
/**************************************************************

	FUNCTION NAME		: InitPreferedInputMethod

  	PURPOSE				: initialization function for input method screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitPreferedInputMethod(void)
{
    U16 i=0;

	for (;;)
	{
		if (((0xff) != gIMEModeArray[i].IME_Mode_ID)&&(0!=gIMEModeArray[i].Prefered_IMEStringID))
		{
			inputMethodList[i] = (U8*)GetString(gIMEModeArray[i].Prefered_IMEStringID);
			i++;
		}
		else
		{
			break;
		}
	}
	gnInputMethods = i ;
	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightInputMehtodSetting

  	PURPOSE				: highlight handler for input method selection

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightInputMehtodSetting(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* set the left/right soft key functions handlers */
        SetLeftSoftkeyFunction(EntryScrPreferedInputMethodList,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* set the left/right arrow key function handlers */
	SetKeyHandler(EntryScrPreferedInputMethodList, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: EntryScrPreferedInputMethodList

  	PURPOSE				: Entry function for input method screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrPreferedInputMethodList(void)
{
    U8* pGuiBuffer;
    U8 i;
    InitPreferedInputMethod();

    i = 0;
    while(MMI_all_prefered_input_mode_set[i] != MMI_prefered_input_type)
    {
        if(MMI_all_prefered_input_mode_set[i] == 0xffff)
        {
        	i=0;
        	break;
        }
        else
              i++;

    }
    currInputMethodIndex = i;
	/* Call Exit Handler*/
	//ExecuteCurrExitHandler();
	EntryNewScreen(SCR_INPUT_METHOD_SELECTION, ExitInputMethodSelectScreen, NULL, NULL);
	/* Get current screen info into gui buffer  for history purposes*/
    pGuiBuffer = GetCurrGuiBuffer(SCR_INPUT_METHOD_SELECTION);
	/* Register highlight handler to be called in menu screen */
    RegisterHighlightHandler(HighlightCurrentInputMethod);
	/* Display Screen */
	ShowCategory36Screen(  STR_MENU_INPUT_METHOD, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
                          gnInputMethods, (U8**)inputMethodList, currInputMethodIndex,pGuiBuffer);
	/* Register function with left/right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    SetLeftSoftkeyFunction(SelectInputMethodHandler, KEY_EVENT_UP);
	/* Register function with left/right arrow key */
	SetKeyHandler(0, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	/* Register Exit handler */
    //SetExitHandler(SCR_INPUT_METHOD_SELECTION, ExitInputMethodSelectScreen);
	return;
}
 
/**************************************************************

	FUNCTION NAME		: HighlightCurrentInputMethod

  	PURPOSE				: upate selected index

	INPUT PARAMETERS	: S32 index

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCurrentInputMethod(S32 index)
{
	/* upate selected index */
    currInputMethodIndex  = (U8) index;
	return;
}
/**************************************************************

	FUNCTION NAME		: SelectInputMethodHandler

  	PURPOSE				: Exit function for input method screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SelectInputMethodHandler(void)
{
    U16 data;
	S16 error;
	U16 index;
	index = GetCurrHiliteID();
    SetInputMethod(currInputMethodIndex);
    data = MMI_prefered_input_type;
    WriteValue( NVRAM_SETTING_PREFER_INPUT_METHOD, &data, DS_SHORT , &error);
	pfnUnicodeStrcpy((PS8)hintData[index],(PS8)inputMethodList[currInputMethodIndex]);
    Category52ChangeItemDescription(index,hintData[index]);
    RedrawCategoryFunction();
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
	DeleteNHistory(1);
	return;
}
/**************************************************************

	FUNCTION NAME		: ExitInputMethodSelectScreen

  	PURPOSE				: Exit function for input method screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitInputMethodSelectScreen(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	S16 nHistory = 0;
	/* getting details of the current screen (to be added to history) */
    Scr.scrnID = SCR_INPUT_METHOD_SELECTION;
    Scr.entryFuncPtr = EntryScrPreferedInputMethodList;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategoryHistory(Scr.guiBuffer);
	/* add the screen to history */
	AddHistory(Scr);
	return;
}
 
/**************************************************************

	FUNCTION NAME		: SetInputMethod

  	PURPOSE				: Set input method

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetInputMethod(U16 index)
{


	if(index < INPUT_MODE_MAX_NUM)
     	MMI_prefered_input_type = MMI_all_prefered_input_mode_set[index];
     else
       MMI_prefered_input_type = INPUT_TYPE_ALPHANUMERIC_UPPERCASE;
	return;
}
/**************************************************************

	FUNCTION NAME		: GetPrefferedInputMethodByCurrentLang

  	PURPOSE				: Get Preffered Input Method By Current Lang

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern pBOOL IsSmChineseSet(void);
extern pBOOL IsTrChineseSet(void);
extern pBOOL IsThaiSet(void);
extern pBOOL IsFrenchSet(void);
extern pBOOL IsItalianSet(void);
extern pBOOL IsVietnameseSet(void);
extern pBOOL IsRussianSet(void);
extern pBOOL IsArabicSet(void);
extern pBOOL IsHindiSet(void);
extern pBOOL IsGermanSet(void);
extern pBOOL IsSpanishSet(void);
extern pBOOL IsPortugueseSet(void);
extern pBOOL IsPersianSet(void);
extern pBOOL IsTurkishSet(void);
extern pBOOL	IsIndonesianSet(void);
extern pBOOL IsMalaySet(void);
extern pBOOL IsTeluguSet(void);
extern pBOOL IsMyanmarSet(void);
extern pBOOL IsTamilSet(void);

U16 GetPrefferedInputMethodByCurrentLang(void)
{
	#ifdef __MMI_LANG_SM_CHINESE__
	if(IsSmChineseSet())
		return INPUT_TYPE_SM_PINYIN;
	else
	#endif
	#ifdef __MMI_LANG_TR_CHINESE__
	if(IsTrChineseSet())
		return INPUT_TYPE_TR_BOPOMO;
	else
	#endif
	#ifdef __MMI_LANG_THAI__
	if(IsThaiSet())
	{
		#ifdef __MMI_SMART_THAI__
			return INPUT_TYPE_SMART_THAI;
		#else
			return INPUT_TYPE_MULTITAP_THAI;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_FRENCH__
	if(IsFrenchSet())
	{
		#ifdef __MMI_SMART_FRENCH__
			return INPUT_TYPE_SMART_LOWERCASE_FRENCH;
		#else
			return INPUT_TYPE_ALPHANUMERIC_LOWERCASE_FRENCH;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_ITALIAN__
	if(IsItalianSet())
	{
		#ifdef __MMI_SMART_ITALIAN__
			return INPUT_TYPE_SMART_LOWERCASE_ITALIAN;
		#else
			return INPUT_TYPE_ALPHANUMERIC_LOWERCASE_ITALIAN;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_VIETNAMESE__
	if(IsVietnameseSet())
	{
		#ifdef __MMI_SMART_VIETNAMESE__
			return INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE;
		#else
			return INPUT_TYPE_ALPHANUMERIC_LOWERCASE_VIETNAMESE;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_RUSSIAN__
	if(IsRussianSet())
	{
		#ifdef __MMI_SMART_RUSSIAN__
			return INPUT_TYPE_SMART_LOWERCASE_RUSSIAN;
		#else
			return INPUT_TYPE_ALPHANUMERIC_LOWERCASE_RUSSIAN;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_ARABIC__
	if(IsArabicSet())
	{
		#ifdef __MMI_SMART_ARABIC__
			return INPUT_TYPE_SMART_ARABIC;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_HINDI__
	if(IsHindiSet())
	{
		#ifdef __MMI_SMART_HINDI__
			return INPUT_TYPE_SMART_HINDI;
		#else
			return INPUT_TYPE_MULTITAP_HINDI;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_TELUGU__
	if(IsTeluguSet())
	{
		#ifdef __MMI_SMART_TELUGU__
			return INPUT_TYPE_SMART_TELUGU;
		#else
			return INPUT_TYPE_MMI_MULTITAP_TELUGU;
		#endif
	}
	#endif
	#ifdef __MMI_LANG_GERMAN__
	if(IsGermanSet())
	{
		#ifdef __MMI_SMART_GERMAN__
			return INPUT_TYPE_SMART_LOWERCASE_GERMAN;
		#else
			return INPUT_TYPE_ALPHANUMERIC_LOWERCASE_GERMAN;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_SPANISH__
	if(IsSpanishSet())
	{
		#ifdef __MMI_SMART_SPANISH__
			return INPUT_TYPE_SMART_LOWERCASE_SPANISH;
		#else
			return INPUT_TYPE_ALPHANUMERIC_LOWERCASE_SPANISH;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_PORTUGUESE__
	if(IsPortugueseSet())
	{
		#ifdef __MMI_GB_SMART_PORTUGUESE__
			return INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE;
		#else
			return INPUT_TYPE_ALPHANUMERIC_LOWERCASE_PORTUGUESE;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_PERSIAN__
	if(IsPersianSet())
	{
		#ifdef __MMI_SMART_PERSIAN__
			return INPUT_TYPE_SMART_PERSIAN;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_TURKISH__
	if(IsTurkishSet())
	{
		#ifdef __MMI_SMART_TURKISH__
			return INPUT_TYPE_SMART_LOWERCASE_TURKISH;
		#else
			return INPUT_TYPE_ALPHANUMERIC_LOWERCASE_TURKISH;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_INDONESIAN__
	if(IsIndonesianSet())
	{
		#ifdef __MMI_SMART_INDONESIAN__
			return INPUT_TYPE_SMART_LOWERCASE_INDONESIAN;
		#else
			return INPUT_TYPE_ALPHANUMERIC_LOWERCASE_INDONESIAN;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_MALAY__
	if(IsMalaySet())
	{
		#ifdef __MMI_SMART_MALAY__
			return INPUT_TYPE_SMART_LOWERCASE_MALAY;
		#else
			return INPUT_TYPE_ALPHANUMERIC_LOWERCASE_MALAY;
		#endif
	}
	else
	#endif
	#ifdef __MMI_LANG_MYANMAR__
	if(IsMyanmarSet())
	{
		return INPUT_TYPE_MMI_MULTITAP_MYANMAR;
	}
	else
	#endif
	#ifdef __MMI_LANG_TAMIL__
	if(IsTamilSet())
	{
		#ifdef __MMI_SMART_TAMIL__
			return INPUT_TYPE_SMART_TAMIL;
		#else
			return INPUT_TYPE_MMI_MULTITAP_TAMIL;
		#endif
	}
	else
	#endif
		return INPUT_TYPE_SMART_LOWERCASE_ABC;
}
/**************************************************************

	FUNCTION NAME		: SetInputMethodFromNVRAM

  	PURPOSE				: Retrive input method type from nvram and set.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetInputMethodFromNVRAM(void)
{
	S16 error;
	U16 data=0;
	U8 i;
	/* retrive input method type from nvram */

	ReadValue( NVRAM_SETTING_PREFER_INPUT_METHOD, &data, DS_SHORT , &error);
	if(data == 0xFF || error != NVRAM_READ_SUCCESS )
	{
		data = GetPrefferedInputMethodByCurrentLang();//INPUT_TYPE_ALPHANUMERIC_LOWERCASE;
		//WriteValue( NVRAM_SETTING_PREFER_INPUT_METHOD, &data, DS_SHORT , &error);
	}
	/* check the input type */
	i = 0;
	while(MMI_all_prefered_input_mode_set[i] != data)
	{
		if(MMI_all_prefered_input_mode_set[i] == 0xffff)
		{
			i=0;
			break;
		}
		else
			i++;
	}
	currInputMethodIndex = i;

	/* set input method with current input method */
	SetInputMethod(currInputMethodIndex);
	return;
}
/**************************************************************

	FUNCTION NAME		: SetInputMethodType

  	PURPOSE				: Entry function for call barring result screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetInputMethodType(U16 inputType)
{
    U8 i;

    i = 0;
    while(MMI_all_prefered_input_mode_set[i] != inputType)
    {
        if(MMI_all_prefered_input_mode_set[i] == 0xffff)
        {
        	i=0;
        	break;
        }
        else
              i++;

    }
    currInputMethodIndex = i;

    SetInputMethod(currInputMethodIndex);
    return;
}
/**************************************************************

	FUNCTION NAME		: GetInputMethodIndex

  	PURPOSE				: function that return input method index

	INPUT PARAMETERS	: U16 input type

	OUTPUT PARAMETERS	: nil

	RETURNS				: U16

 

**************************************************************/
U16 GetInputMethodIndex(U16 inputType)
{
    U8 i;

    i = 0;
    while(MMI_all_prefered_input_mode_set[i] != inputType)
    {
        if(MMI_all_prefered_input_mode_set[i] == 0xffff)
        {
            i=0;
            break;
        }
        else
              i++;
        
    }
    return i;
}
 
#endif
/*********************************************END OF FILE *************************************/

