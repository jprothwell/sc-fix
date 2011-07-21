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
 *    commonscreens.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   for input method menu and symbol picker screen
 *
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: CommonScreens.c

  	PURPOSE		: Common screens used by applications

 


	DATE		: Aug-06-2003

**************************************************************/

#include "mmi_features.h"
 
#include "globalmenuitems.h"
 
#include "commonscreens.h"
#include "timerevents.h"
#include "keybrd.h"
#include "profilegprots.h"
#include "globaldefs.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "eventsgprot.h"
//START 
#include "gui_ems.h"
#include "hf_rules.h"
//END 
 
#include "imeres.h"
#include "debuginitdef.h"
extern const sIMEModeDetails *gIMEModeArray;
extern const sIMEModeDetails *gIMEQSearchModeArray;
 

//#include "wgui_categories_popup.h"
#include "sublcdhistorygprot.h"

extern void leave_full_screen(void);

//beginning:guoyt added for adding Hindi,German and Spanish IME.
#ifdef __MMI_SMART_HINDI__				
extern void (*wgui_change_inputbox_mode_smart_hindi)(void);
#endif
#if defined(__MMI_SMART_GERMAN__)		
extern void (*wgui_change_inputbox_mode_smart_GERMAN)(void);
extern void (*wgui_change_inputbox_mode_smart_german)(void);
#endif
#if defined(__MMI_SMART_SPANISH__)		
void (*wgui_change_inputbox_mode_smart_SPANISH)(void) = UI_dummy_function;
void (*wgui_change_inputbox_mode_smart_spanish)(void) = UI_dummy_function;
#endif
//end:guoyt added for adding Hindi,German and Spanish IME.
#ifdef __MMI_SMART_TELUGU__
extern void (*wgui_change_inputbox_mode_smart_telugu)(void);
#endif

extern UI_character_type gui_multi_line_current_character(multi_line_input_box *b);
extern BOOL IsMyTimerExist(U16 nTimerId);
void (*InputMethodScreenCloseFunction)(void)=UI_dummy_function;
void (*SavedInputMethodScreenCloseFunction)(void)=UI_dummy_function;
s16	 saved_MMI_current_input_type=0;
U16 gInputMethodAndDoneCaptionIcon=0;
#if defined(__MMI_GB__)
PUBLIC BOOL EnterSymboFlag = FALSE;
#endif
extern void goto_demo_under_construction(void);
extern s16	 MMI_current_input_type;
static U8 is_popup_allow = POPUP_ALL_ON; //it can be POPUP_ALL_OFF, POPUP_DSPL_ON, POPUP_TONE_ON, or POPUP_ALL_ON
 
#ifdef __MMI_T9__
extern void T9SynT9WithInputBox(void);
#elif defined __MMI_ZI__
extern void ZiDisplay(void);
#elif defined __MMI_KA__
extern void KonkaDisplay(void);
#elif defined __MMI_ITAP__
extern void ItapDisplay(void);
#endif

extern void TurnOnBacklight(U8 TimerFlag); 


//CSD end
U8 popupToneId = 0; 
 
#if 0//defined __MMI_LANG_HINDI__ && defined __MMI_LANG_ARABIC__
//guoyt modify:some bug arise from these codes controlled by "defined __MMI_LANG_HINDI__  && defined __MMI_LANG_ARABIC__". 
extern S32 HINDI_PRESENT;
extern S32 ARABIC_PRESENT;
#endif
 

//START 
#if defined (__MMI_MULTITAP_HINDI__)
extern UI_character_type coolsand_UI_EMS_current_character(UI_EMS_input_box *b);
extern UI_EMS_input_box	MMI_EMS_inputbox;
#endif
//END 
#define NUMBER_OF_INPUT_METHODS  INPUT_MODE_MAX_NUM

FuncPtr preHandleInputMethodLSKFuncPtrs[NUMBER_OF_INPUT_METHODS];
FuncPtr postHandleInputMethodLSKFuncPtrs[NUMBER_OF_INPUT_METHODS];

extern byte wgui_singleline_inputbox_input_mode_changed;
extern byte wgui_singleline_inputbox_input_type;
extern void wgui_change_inputbox_mode_quick_search_numeric(void);
extern void wgui_change_inputbox_mode_quick_search_abc(void);
 
#if defined(__MMI_MULTITAP_SPANISH__)
extern void wgui_change_inputbox_mode_quick_search_spanish(void);
#endif

//qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
extern void wgui_change_inputbox_mode_quick_search_tamil(void);
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
extern void wgui_change_inputbox_mode_quick_search_bengali(void);
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
extern void wgui_change_inputbox_mode_quick_search_punjabi(void);
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
extern void wgui_change_inputbox_mode_quick_search_telugu(void);
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
extern void wgui_change_inputbox_mode_quick_search_kannada(void);
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
extern void wgui_change_inputbox_mode_quick_search_malayalam(void);
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
extern void wgui_change_inputbox_mode_quick_search_oriya(void);
#endif
//qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
extern void wgui_change_inputbox_mode_quick_search_marathi(void);
#endif

#if defined(__MMI_MULTITAP_URDU__)
extern void wgui_change_inputbox_mode_quick_search_urdu(void);
#endif

#if defined(__MMI_MULTITAP_MYANMAR__)
extern void wgui_change_inputbox_mode_quick_search_myanmar(void);
#endif

 
#if defined(__MMI_MULTITAP_DANISH__)
extern void wgui_change_inputbox_mode_quick_search_danish(void);
#endif
 
#if defined(__MMI_MULTITAP_FRENCH__)
extern void wgui_change_inputbox_mode_quick_search_french(void);
#endif
#if defined(__MMI_MULTITAP_GERMAN__)
extern void wgui_change_inputbox_mode_quick_search_german(void);
#endif
#if defined(__MMI_MULTITAP_ITALIAN__)
extern void wgui_change_inputbox_mode_quick_search_italian(void);
#endif
#if defined(__MMI_MULTITAP_RUSSIAN__)
extern void wgui_change_inputbox_mode_quick_search_russian(void);
#endif
#if defined(__MMI_MULTITAP_TURKISH__)
extern void wgui_change_inputbox_mode_quick_search_turkish(void);
#endif
#if defined(__MMI_MULTITAP_PORTUGUESE__)
extern void wgui_change_inputbox_mode_quick_search_portuguese(void);
#endif
#if defined(__MMI_MULTITAP_HINDI__)
extern void wgui_change_inputbox_mode_quick_search_HINDI(void);
#endif

#if defined(__MMI_MULTITAP_INDONESIAN__)
extern void wgui_change_inputbox_mode_quick_search_indonesian(void);
#endif
#if defined(__MMI_MULTITAP_MALAY__)
extern void wgui_change_inputbox_mode_quick_search_malay(void);
#endif
#if defined(__MMI_MULTITAP_VIETNAMESE__)
extern void wgui_change_inputbox_mode_quick_search_vietnamese(void);
#endif
 
#if defined(__MMI_MULTITAP_THAI__)
extern void	wgui_change_inputbox_mode_quick_search_thai(void);
#endif
//CSD End


//CSD end
		 
#if defined (__MMI_T9__) || defined (__MMI_ZI__) || defined(__MMI_KA__) || defined(__MMI_ITAP__)
extern void wgui_change_inputbox_mode_quick_search_tr_stroke(void);
extern void wgui_change_inputbox_mode_quick_search_sm_stroke(void);
extern void wgui_change_inputbox_mode_tr_quick_search_multitap_bpmf(void);
extern void wgui_change_inputbox_mode_quick_search_sm_multitap_pinyin(void);
#endif

const U16 gIndexIconsImageList[]=
{
	IMG_GLOBAL_L1,
	IMG_GLOBAL_L2,
	IMG_GLOBAL_L3,
	IMG_GLOBAL_L4,
	IMG_GLOBAL_L5,
	IMG_GLOBAL_L6,
	IMG_GLOBAL_L7,
	IMG_GLOBAL_L8,
	IMG_GLOBAL_L9,
	IMG_GLOBAL_L10,
	IMG_GLOBAL_L11,
	IMG_GLOBAL_L12,
	IMG_GLOBAL_L13,
	IMG_GLOBAL_L14,
	IMG_GLOBAL_L15,
	IMG_GLOBAL_L16,
	IMG_GLOBAL_L17,
	IMG_GLOBAL_L18,
	IMG_GLOBAL_L19,
	IMG_GLOBAL_L20,
	IMG_GLOBAL_L21,
	IMG_GLOBAL_L22,
	IMG_GLOBAL_L23,
	IMG_GLOBAL_L24,
	IMG_GLOBAL_L25,
	IMG_GLOBAL_L26,
	IMG_GLOBAL_L27,
	IMG_GLOBAL_L28,
	IMG_GLOBAL_L29,
	IMG_GLOBAL_L30,
	IMG_GLOBAL_L31,//Fix Bug#12068 for M301WapMMS by cong.li on 2009.05.14.
	IMG_GLOBAL_L32,
	IMG_GLOBAL_L33,
	IMG_GLOBAL_L34,
	IMG_GLOBAL_L35,
	IMG_GLOBAL_L36,
	IMG_GLOBAL_L37,
	IMG_GLOBAL_L38,
	IMG_GLOBAL_L39	
};

 
void SetSavedQuickSearchInputType(void)
{
	switch(saved_MMI_current_input_type & INPUT_TYPE_MASK)
	{
		case	INPUT_TYPE_MMI_NUMERIC:
				wgui_change_inputbox_mode_quick_search_numeric();
				break;

		case	INPUT_TYPE_MMI_LOWERCASE_ABC:
				wgui_change_inputbox_mode_quick_search_abc();
				break;
 
#if defined(__MMI_MULTITAP_SPANISH__)
		case	INPUT_TYPE_MMI_LOWERCASE_SPANISH:
				wgui_change_inputbox_mode_quick_search_spanish();
				break;
#endif
 

//qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
		case	INPUT_TYPE_MMI_MULTITAP_TAMIL:
				wgui_change_inputbox_mode_quick_search_tamil();
				break;
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
		case	INPUT_TYPE_MMI_MULTITAP_BENGALI:
				wgui_change_inputbox_mode_quick_search_bengali();
				break;
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
		case	INPUT_TYPE_MMI_MULTITAP_PUNJABI:
				wgui_change_inputbox_mode_quick_search_punjabi();
				break;

#endif
#if defined(__MMI_MULTITAP_TELUGU__)
		case	INPUT_TYPE_MMI_MULTITAP_TELUGU:
				wgui_change_inputbox_mode_quick_search_telugu();
				break;
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
		case	INPUT_TYPE_MMI_MULTITAP_KANNADA:
				wgui_change_inputbox_mode_quick_search_kannada();
				break;
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
		case	INPUT_TYPE_MMI_MULTITAP_MALAYALAM:
				wgui_change_inputbox_mode_quick_search_malayalam();
				break;
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
		case	INPUT_TYPE_MMI_MULTITAP_ORIYA:
				wgui_change_inputbox_mode_quick_search_oriya();
				break;

#endif
//qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
		case	INPUT_TYPE_MMI_MULTITAP_MARATHI:
				wgui_change_inputbox_mode_quick_search_marathi();
				break;
#endif

#if defined(__MMI_MULTITAP_URDU__)
		case	INPUT_TYPE_MMI_MULTITAP_URDU:
				wgui_change_inputbox_mode_quick_search_urdu();
				break;
#endif

#if defined(__MMI_MULTITAP_DANISH__)
		case	INPUT_TYPE_MMI_LOWERCASE_DANISH:
				wgui_change_inputbox_mode_quick_search_danish();
				break;
#endif
 
#if defined(__MMI_MULTITAP_FRENCH__)
		case	INPUT_TYPE_MMI_LOWERCASE_FRENCH:
				wgui_change_inputbox_mode_quick_search_french();
				break;
#endif
#if defined(__MMI_MULTITAP_MYANMAR__)
		case 	INPUT_TYPE_MMI_MULTITAP_MYANMAR:
				wgui_change_inputbox_mode_quick_search_myanmar();
				break;
#endif
#if defined(__MMI_MULTITAP_GERMAN__)
		case	INPUT_TYPE_MMI_LOWERCASE_GERMAN:
				wgui_change_inputbox_mode_quick_search_german();
				break;
#endif
#if defined(__MMI_MULTITAP_ITALIAN__)
		case	INPUT_TYPE_MMI_LOWERCASE_ITALIAN:
				wgui_change_inputbox_mode_quick_search_italian();
				break;
#endif
#if defined(__MMI_MULTITAP_RUSSIAN__)
		case	INPUT_TYPE_MMI_LOWERCASE_RUSSIAN:
				wgui_change_inputbox_mode_quick_search_russian();
				break;
#endif
#if defined(__MMI_MULTITAP_TURKISH__)
		case	INPUT_TYPE_MMI_LOWERCASE_TURKISH:
				wgui_change_inputbox_mode_quick_search_turkish();
				break;
#endif
#if defined(__MMI_MULTITAP_PORTUGUESE__)
		case	INPUT_TYPE_MMI_LOWERCASE_PORTUGUESE:
				wgui_change_inputbox_mode_quick_search_portuguese();
				break;
#endif
#if defined(__MMI_MULTITAP_HINDI__)
		case	INPUT_TYPE_MULTITAP_HINDI:
				wgui_change_inputbox_mode_quick_search_HINDI();
				break;
#endif

#if defined(__MMI_MULTITAP_INDONESIAN__)
		case	INPUT_TYPE_MMI_LOWERCASE_INDONESIAN:
				wgui_change_inputbox_mode_quick_search_indonesian();
				break;
#endif
#if defined(__MMI_MULTITAP_MALAY__)
		case	INPUT_TYPE_MMI_LOWERCASE_MALAY:
				wgui_change_inputbox_mode_quick_search_malay();
				break;
#endif
#if defined(__MMI_MULTITAP_VIETNAMESE__)
		case	INPUT_TYPE_MMI_LOWERCASE_VIETNAMESE:
				wgui_change_inputbox_mode_quick_search_vietnamese();
				break;
#endif
 
#if defined(__MMI_MULTITAP_THAI__)
		case	INPUT_TYPE_MMI_MULTITAP_THAI:
				wgui_change_inputbox_mode_quick_search_thai();
				break;
#endif

//CSD end
		 
#if defined (__MMI_T9__) || defined (__MMI_ZI__) || defined(__MMI_KA__) || defined(__MMI_ITAP__)
		case INPUT_TYPE_MMI_TR_STROKE://tr stroke
			wgui_change_inputbox_mode_quick_search_tr_stroke();
			break;

		case INPUT_TYPE_MMI_SM_STROKE://sm stroke
			wgui_change_inputbox_mode_quick_search_sm_stroke();
			break;

		case INPUT_TYPE_MMI_MULTITAP_BPMF://mulitap bopomofo
			wgui_change_inputbox_mode_tr_quick_search_multitap_bpmf();
			break;

		case INPUT_TYPE_MMI_MULTITAP_PINYIN://multitap pinyin
			wgui_change_inputbox_mode_quick_search_sm_multitap_pinyin();
			break;
#endif
//CSD end

	}
}
extern void (*wgui_inline_edit_DONE_function)(void);
void (*wgui_common_screen_inline_edit_DONE_function)(void);
void mmi_frm_go_back_2_history (void);
void HighlightInLineMultilineEditorOption(S32 nIndex)
{
	switch (nIndex) 
	{
		case 0:
			ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
			ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
			SetLeftSoftkeyFunction(wgui_common_screen_inline_edit_DONE_function,KEY_EVENT_UP);
			SetKeyHandler(wgui_common_screen_inline_edit_DONE_function, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
			SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
			SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
			break;
		case 1:
			ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
			ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
			SetLeftSoftkeyFunction(EntryInputMethodScreen,KEY_EVENT_UP);
			SetKeyHandler(EntryInputMethodScreen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
			SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
			SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
			RegisterInputMethodScreenCloseFunction (mmi_frm_go_back_2_history);
			break;
		default:
			break;
	}
}

void EntryInLineMultilineEditorOption(void)
{
	U16 nStrItemList[2]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;		/* Stores no of children in the submenu*/
	U8* guiBuffer;		/* Buffer holding history data */

      if (wgui_inline_edit_DONE_function)
      {
	   wgui_common_screen_inline_edit_DONE_function = wgui_inline_edit_DONE_function;
	}
	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_IN_LINE_MULTI_LINE_INPUT_METHOD_AND_DONE, ExitInLineMultilineEditorOption, NULL, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_IN_LINE_MULTI_LINE_INPUT_METHOD_AND_DONE);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild (MENU_INPUT_METHOD_AND_DONE_OPTION_ID);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_INPUT_METHOD_AND_DONE_OPTION_ID, nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_INPUT_METHOD_AND_DONE_OPTION_ID);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(HighlightInLineMultilineEditorOption);

	/* 8 Display Category Screen */
	ShowCategory52Screen (STR_GLOBAL_OPTIONS, gInputMethodAndDoneCaptionIcon,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (PU16)gIndexIconsImageList,
						  NULL , 0, 0, guiBuffer );

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	/* 10. Register Exit handler */
}
void ExitInLineMultilineEditorOption(void)
{
	history_t currHistory;
	S16 ucs2InputBuffer=0;
	currHistory.scrnID = SCR_IN_LINE_MULTI_LINE_INPUT_METHOD_AND_DONE;
	currHistory.entryFuncPtr = EntryInLineMultilineEditorOption;
	pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&ucs2InputBuffer);
	GetCategoryHistory (currHistory.guiBuffer);
	AddHistory (currHistory);
}

U32 QuickSearchInputMethodSetKeyHandler(FuncPtr *preFuncPtrs,FuncPtr *postPtrs,const sIMEModeDetails* IMEQSearchModeArray)
{
	int i = 0;
	for (;;)
	{
		switch (IMEQSearchModeArray[i].IME_Mode_ID)
		{
			case INPUT_MODE_MMI_LOWERCASE_ABC:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_abc;
				break;
#if defined(__MMI_MULTITAP_SPANISH__)
			case INPUT_MODE_MMI_LOWERCASE_SPANISH:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_spanish;
				break;
#endif

//qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
			case INPUT_MODE_MMI_MULTITAP_TAMIL:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_tamil;
				break;
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
			case INPUT_MODE_MMI_MULTITAP_BENGALI:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_bengali;
				break;
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
			case INPUT_MODE_MMI_MULTITAP_PUNJABI:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_punjabi;
				break;
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
			case INPUT_MODE_MMI_MULTITAP_TELUGU:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_telugu;
				break;
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
			case INPUT_MODE_MMI_MULTITAP_KANNADA:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_kannada;
				break;
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
			case INPUT_MODE_MMI_MULTITAP_MALAYALAM:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_malayalam;
				break;
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
			case INPUT_MODE_MMI_MULTITAP_ORIYA:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_oriya;
				break;
#endif
//qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
			case INPUT_MODE_MMI_MULTITAP_MARATHI:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_marathi;
				break;
#endif
#if defined(__MMI_MULTITAP_URDU__)
			case INPUT_MODE_MMI_MULTITAP_URDU:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_urdu;
				break;
#endif
 
#if defined(__MMI_MULTITAP_DANISH__)
			case INPUT_MODE_MMI_LOWERCASE_DANISH:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_danish;
				break;
#endif
 
#if defined(__MMI_MULTITAP_FRENCH__)
			case INPUT_MODE_MMI_LOWERCASE_FRENCH:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_french;
				break;
#endif

#if defined(__MMI_MULTITAP_MYANMAR__)
			case INPUT_MODE_MMI_MULTITAP_MALAYALAM:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_myanmar;
				break;
#endif
#if defined(__MMI_MULTITAP_GERMAN__)
			case INPUT_MODE_MMI_LOWERCASE_GERMAN:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_german;
				break;
#endif
#if defined(__MMI_MULTITAP_ITALIAN__)
			case INPUT_MODE_MMI_LOWERCASE_ITALIAN:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_italian;
				break;
#endif
#if defined(__MMI_MULTITAP_RUSSIAN__)
			case INPUT_MODE_MMI_LOWERCASE_RUSSIAN:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_russian;
				break;
#endif
#if defined(__MMI_MULTITAP_TURKISH__)
			case INPUT_MODE_MMI_LOWERCASE_TURKISH:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_turkish;
				break;
#endif
#if defined(__MMI_MULTITAP_PORTUGUESE__)
			case INPUT_MODE_MMI_LOWERCASE_PORTUGUESE:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_portuguese;
				break;
#endif
#if defined(__MMI_MULTITAP_INDONESIAN__)
			case INPUT_MODE_MMI_LOWERCASE_INDONESIAN:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_indonesian;
				break;
#endif
#if defined(__MMI_MULTITAP_MALAY__)
			case INPUT_MODE_MMI_LOWERCASE_MALAY:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_malay;
				break;
#endif
#if defined(__MMI_MULTITAP_VIETNAMESE__)
			case INPUT_MODE_MMI_LOWERCASE_VIETNAMESE:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_vietnamese;
				break;
#endif
#if defined(__MMI_MULTITAP_HINDI__)
		case	INPUT_MODE_MULTITAP_HINDI:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_HINDI;
				break;
#endif
#if defined(__MMI_T9_TR_CHINESE__)||defined(__MMI_ZI_TR_CHINESE__)||defined(__MMI_ITAP_TR_CHINESE__)
			case INPUT_MODE_MMI_MULTITAP_BPMF:
				preFuncPtrs[i] = wgui_change_inputbox_mode_tr_quick_search_multitap_bpmf;
				break;
			case INPUT_MODE_MMI_TR_STROKE:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_tr_stroke;
				break;
#endif
#if defined(__MMI_T9_SM_CHINESE__)||defined(__MMI_ZI_SM_CHINESE__)||defined(__MMI_ITAP_SM_CHINESE__)
			case INPUT_MODE_MMI_MULTITAP_PINYIN:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_sm_multitap_pinyin;
				break;
			case INPUT_MODE_MMI_SM_STROKE:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_sm_stroke;
				break;
#endif
			case INPUT_MODE_MMI_NUMERIC:
				preFuncPtrs[i] = wgui_change_inputbox_mode_quick_search_numeric;
				break;
			case (0xff):
				break;
			default :
				MMI_ASSERT(0);
		}
		if ((0xff)!=IMEQSearchModeArray[i].IME_Mode_ID)
		{
			postPtrs[i]=InputMethodScreenCloseFunction;
			i++;
		}
		else
		{
			break;
		}
	}
	return i;
}

void QuickSearchInputMethodScreenHandleLSK(void)
{
       U32 i;
       FuncPtr currPreFuncPtr = NULL;
       FuncPtr currPostFuncPtr = NULL;
	S32 index=GetHighlightedItem();
       
       i=0;

	i = QuickSearchInputMethodSetKeyHandler(preHandleInputMethodLSKFuncPtrs,postHandleInputMethodLSKFuncPtrs,gIMEQSearchModeArray);
      postHandleInputMethodLSKFuncPtrs[i]=UI_dummy_function;      
      preHandleInputMethodLSKFuncPtrs[i++] = EntrySymbolPickerScreen;

      currPreFuncPtr = preHandleInputMethodLSKFuncPtrs[index];
      currPostFuncPtr = postHandleInputMethodLSKFuncPtrs[index];

      if(currPreFuncPtr!= NULL)
       (*currPreFuncPtr)();
      if(currPostFuncPtr!= NULL)
       (*currPostFuncPtr)();      
     //CSD end
}
void QuickSearchInputMethodScreenHandleRSK(void)
{
	SetSavedQuickSearchInputType();
	GoBackHistory();
}
void EntryQuickSearchInputMethodScreen(void)
{
	U8* guiBuffer;
	int nNumofItem=0,i=0;
	void (*LSK_handler)(void);
	U16 nStrItemList[INPUT_MODE_MAX_NUM];
	for (;;)
	{
		if (((0xff) != gIMEQSearchModeArray[i].IME_Mode_ID)&&(0 != gIMEQSearchModeArray[i].Common_Screen_StringID))
		{
			nStrItemList[nNumofItem] = gIMEQSearchModeArray[i].Common_Screen_StringID;
			nNumofItem++;
		}
		else
		{
			break;
		}
		i++;
	}

	EntryNewScreen(SCR_QUICK_SEARCH_INPUT_METHOD_LIST, ExitQuickSearchInputMethodScreen, NULL, NULL);
	LSK_handler=QuickSearchInputMethodScreenHandleLSK;
	saved_MMI_current_input_type=MMI_current_input_type;
	guiBuffer = GetCurrGuiBuffer(SCR_QUICK_SEARCH_INPUT_METHOD_LIST);
	if(guiBuffer!=NULL) InputMethodScreenCloseFunction=SavedInputMethodScreenCloseFunction;
      ShowCategory15Screen(STR_GLOBAL_INPUT_METHOD, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (PU16)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);
	SetLeftSoftkeyFunction(LSK_handler,KEY_EVENT_UP);
	SetRightSoftkeyFunction(QuickSearchInputMethodScreenHandleRSK,KEY_EVENT_UP);
	SetKeyHandler(LSK_handler, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(QuickSearchInputMethodScreenHandleRSK, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

void ExitQuickSearchInputMethodScreen(void)
{
	history_t		h;
	S16			nHistory=0;

	SavedInputMethodScreenCloseFunction=InputMethodScreenCloseFunction;
	h.scrnID = SCR_QUICK_SEARCH_INPUT_METHOD_LIST;
	h.entryFuncPtr = EntryQuickSearchInputMethodScreen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

void SetSavedInputType(void)
{	switch(saved_MMI_current_input_type & INPUT_TYPE_MASK)
	{
		case	INPUT_TYPE_NUMERIC:
		case	INPUT_TYPE_NUMERIC_CHANGEABLE:
				wgui_change_inputbox_mode_mode_numeric();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE:
				wgui_change_inputbox_mode_multitap_ABC();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE:
				wgui_change_inputbox_mode_multitap_abc();
				break;
 
#if defined(__MMI_MULTITAP_THAI__)
		case	INPUT_TYPE_ALPHANUMERIC_MULTITAP_THAI:
				wgui_change_inputbox_mode_multitap_thai();
				break;
#endif
#if defined(__MMI_MULTITAP_SPANISH__)
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_SPANISH:
				wgui_change_inputbox_mode_multitap_SPANISH();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_SPANISH:
				wgui_change_inputbox_mode_multitap_spanish();
				break;
#endif
//qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
			case INPUT_MODE_MMI_MULTITAP_TAMIL:
				 wgui_change_inputbox_mode_multitap_tamil();
				break;
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
			case INPUT_MODE_MMI_MULTITAP_BENGALI:
				 wgui_change_inputbox_mode_multitap_bengali();
				break;
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
			case INPUT_MODE_MMI_MULTITAP_PUNJABI:
				 wgui_change_inputbox_mode_multitap_punjabi();
				break;
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
			case INPUT_MODE_MMI_MULTITAP_TELUGU:
				 wgui_change_inputbox_mode_multitap_telugu();
				break;
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
			case INPUT_MODE_MMI_MULTITAP_KANNADA:
				 wgui_change_inputbox_mode_multitap_kannada();
				break;
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
			case INPUT_MODE_MMI_MULTITAP_MALAYALAM:
				 wgui_change_inputbox_mode_multitap_malayalam();
				break;
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
			case INPUT_MODE_MMI_MULTITAP_ORIYA:
				 wgui_change_inputbox_mode_multitap_oriya();
				break;
#endif
//qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
			case INPUT_MODE_MMI_MULTITAP_MARATHI:
				 wgui_change_inputbox_mode_multitap_marathi();
				break;
#endif
#if defined(__MMI_MULTITAP_URDU__)
			case INPUT_MODE_MMI_MULTITAP_URDU:
				 wgui_change_inputbox_mode_multitap_urdu();
				break;
#endif
 
#if defined(__MMI_MULTITAP_DANISH__)
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_DANISH:
				wgui_change_inputbox_mode_multitap_DANISH();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_DANISH:
				wgui_change_inputbox_mode_multitap_danish();
				break;
#endif
 
#if defined(__MMI_MULTITAP_FRENCH__)
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_FRENCH:
				wgui_change_inputbox_mode_multitap_FRENCH();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_FRENCH:
				wgui_change_inputbox_mode_multitap_french();
				break;
#endif

#if defined(__MMI_MULTITAP_MYANMAR__)
		case INPUT_TYPE_MMI_MULTITAP_MYANMAR:
			wgui_change_inputbox_mode_multitap_myanmar();
			break;
#endif

#if defined(__MMI_MULTITAP_GERMAN__)
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_GERMAN:
				wgui_change_inputbox_mode_multitap_GERMAN();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_GERMAN:
				wgui_change_inputbox_mode_multitap_german();
				break;
#endif
#if defined(__MMI_MULTITAP_ITALIAN__)
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_ITALIAN:
				wgui_change_inputbox_mode_multitap_ITALIAN();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_ITALIAN:
				wgui_change_inputbox_mode_multitap_italian();
				break;
#endif
#if defined(__MMI_MULTITAP_RUSSIAN__)
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_RUSSIAN:
				wgui_change_inputbox_mode_multitap_RUSSIAN();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_RUSSIAN:
				wgui_change_inputbox_mode_multitap_russian();
				break;
#endif
#if defined(__MMI_MULTITAP_TURKISH__)
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_TURKISH:
				wgui_change_inputbox_mode_multitap_TURKISH();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_TURKISH:
				wgui_change_inputbox_mode_multitap_turkish();
				break;
#endif
#if defined(__MMI_MULTITAP_PORTUGUESE__)
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_PORTUGUESE:
				wgui_change_inputbox_mode_multitap_PORTUGUESE();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_PORTUGUESE:
				wgui_change_inputbox_mode_multitap_portuguese();
				break;
#endif
#if defined(__MMI_GB_SMART_PORTUGUESE__)
		case 	INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE :
				wgui_change_inputbox_mode_smart_PORTUGUESE();
				break;
		case 	INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
				wgui_change_inputbox_mode_smart_portuguese();
				break;
#endif

#if defined(__MMI_MULTITAP_HINDI__)
		case	INPUT_TYPE_MULTITAP_HINDI:
				wgui_change_inputbox_mode_multitap_HINDI();
				break;
#endif

#if defined(__MMI_MULTITAP_INDONESIAN__)
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_INDONESIAN:
				wgui_change_inputbox_mode_multitap_INDONESIAN();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_INDONESIAN:
				wgui_change_inputbox_mode_multitap_indonesian();
				break;
#endif
#if defined(__MMI_MULTITAP_MALAY__)
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_MALAY:
				wgui_change_inputbox_mode_multitap_MALAY();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_MALAY:
				wgui_change_inputbox_mode_multitap_malay();
				break;
#endif
#if defined(__MMI_MULTITAP_VIETNAMESE__)
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_VIETNAMESE:
				wgui_change_inputbox_mode_multitap_VIETNAMESE();
				break;

		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_VIETNAMESE:
				wgui_change_inputbox_mode_multitap_vietnamese();
				break;
#endif
             #if defined (__MMI_T9__) || defined (__MMI_ZI__) || defined(__MMI_KA__) || defined(__MMI_ITAP__)
		case INPUT_TYPE_SMART_UPPERCASE_ABC:
                		wgui_change_inputbox_mode_smart_ABC();
				break;
		case INPUT_TYPE_SMART_LOWERCASE_ABC:
		          	wgui_change_inputbox_mode_smart_abc();
				break;

		case INPUT_TYPE_TR_BOPOMO:
		       	wgui_change_inputbox_mode_tr_bpmf();
				break;

		case INPUT_TYPE_TR_STROKE:
		          	wgui_change_inputbox_mode_tr_stroke();
				break;

		case INPUT_TYPE_SM_PINYIN:
		             	wgui_change_inputbox_mode_sm_pinyin();
				break;

		case INPUT_TYPE_SM_STROKE:
                		wgui_change_inputbox_mode_sm_stroke();
				break;
		case INPUT_TYPE_TR_MULTITAP_BOPOMO:
		       	wgui_change_inputbox_mode_tr_multitap_bpmf();
				break;

		case INPUT_TYPE_SM_MULTITAP_PINYIN:
		             	wgui_change_inputbox_mode_sm_multitap_pinyin();
				break;
#if defined(__MMI_T9_SPANISH__)||defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)
		case	INPUT_TYPE_SMART_UPPERCASE_SPANISH:
				wgui_change_inputbox_mode_smart_SPANISH();
				break;

		case	INPUT_TYPE_SMART_LOWERCASE_SPANISH:
				wgui_change_inputbox_mode_smart_spanish();
				break;
#endif
 
#if defined(__MMI_T9_DANISH__)||defined(__MMI_ZI_DANISH__)
		case	INPUT_TYPE_SMART_UPPERCASE_DANISH:
				wgui_change_inputbox_mode_smart_DANISH();
				break;

		case	INPUT_TYPE_SMART_LOWERCASE_DANISH:
				wgui_change_inputbox_mode_smart_danish();
				break;
#endif
 
#if defined(__MMI_T9_FRENCH__)||defined(__MMI_ZI_CA_FRENCH__)||defined(__MMI_ZI_EU_FRENCH__)
		case	INPUT_TYPE_SMART_UPPERCASE_FRENCH:
				wgui_change_inputbox_mode_smart_FRENCH();
				break;

		case	INPUT_TYPE_SMART_LOWERCASE_FRENCH:
				wgui_change_inputbox_mode_smart_french();
				break;
#endif
#if defined(__MMI_T9_GERMAN__)||defined(__MMI_ZI_GERMAN__)
		case	INPUT_TYPE_SMART_UPPERCASE_GERMAN:
				wgui_change_inputbox_mode_smart_GERMAN();
				break;

		case	INPUT_TYPE_SMART_LOWERCASE_GERMAN:
				wgui_change_inputbox_mode_smart_german();
				break;
#endif
#if defined(__MMI_T9_ITALIAN__)||defined(__MMI_ZI_ITALIAN__)
		case	INPUT_TYPE_SMART_UPPERCASE_ITALIAN:
				wgui_change_inputbox_mode_smart_ITALIAN();
				break;

		case	INPUT_TYPE_SMART_LOWERCASE_ITALIAN:
				wgui_change_inputbox_mode_smart_italian();
				break;
#endif
 
#if  defined(__MMI_T9_RUSSIAN__)|| defined(__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
//END 
		case	INPUT_TYPE_SMART_UPPERCASE_RUSSIAN:
				wgui_change_inputbox_mode_smart_RUSSIAN();
				break;

		case	INPUT_TYPE_SMART_LOWERCASE_RUSSIAN:
				wgui_change_inputbox_mode_smart_russian();
				break;
#endif
 
#if defined(__MMI_ZI_THAI__) || defined (__MMI_T9_THAI__)
		case	INPUT_TYPE_SMART_THAI:
				wgui_change_inputbox_mode_smart_thai();
				break;

		case	INPUT_TYPE_SMART_ARABIC:
				wgui_change_inputbox_mode_smart_arabic();
				break;
				
				
#endif
#if defined(__MMI_ZI_TURKISH__)
		case	INPUT_TYPE_SMART_UPPERCASE_TURKISH:
				wgui_change_inputbox_mode_smart_TURKISH();
				break;

		case	INPUT_TYPE_SMART_LOWERCASE_TURKISH:
				wgui_change_inputbox_mode_smart_turkish();
				break;
#endif
 
#if defined(__MMI_T9_PORTUGUESE__)||defined(__MMI_ZI_SA_PORTUGUESE__)||defined(__MMI_ZI_EU_PORTUGUESE__)||defined(__MMI_ZI_BZ_PORTUGUESE__)
 
		case	INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE:
				wgui_change_inputbox_mode_smart_PORTUGUESE();
				break;

		case	INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
				wgui_change_inputbox_mode_smart_portuguese();
				break;
#endif
 
#if defined(__MMI_ZI_INDONESIAN__)||defined(__MMI_T9_INDONESIAN__) 
		case	INPUT_TYPE_SMART_UPPERCASE_INDONESIAN:
				wgui_change_inputbox_mode_smart_INDONESIAN();
				break;

		case	INPUT_TYPE_SMART_LOWERCASE_INDONESIAN:
				wgui_change_inputbox_mode_smart_indonesian();
				break;
#endif
 
#if defined(__MMI_ZI_MALAY__)
		case	INPUT_TYPE_SMART_UPPERCASE_MALAY:
				wgui_change_inputbox_mode_smart_MALAY();
				break;

		case	INPUT_TYPE_SMART_LOWERCASE_MALAY:
				wgui_change_inputbox_mode_smart_malay();
				break;
		#endif
 
#if defined(__MMI_ZI_VIETNAMESE__)||defined(__MMI_T9_VIETNAMESE__)  
 
		case	INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
				wgui_change_inputbox_mode_smart_VIETNAMESE();
				break;

		case	INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
				wgui_change_inputbox_mode_smart_vietnamese();
				break;
#endif

		#endif

	}
}

 

U32 InputMethodSetKeyHandler(FuncPtr *preFuncPtrs,FuncPtr *postPtrs,const sIMEModeDetails* IMEModeArray,BOOL EnglishOnly)
{
	int i = 0,j = 0;
	for (;;)
	{
		switch (IMEModeArray[i].IME_Mode_ID)
		{
#if defined(__MMI_GB_SM_CHINESE__)                                                                                        
			case INPUT_MODE_SM_STROKE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_GBStroke;
				break;
			case INPUT_MODE_SM_PINYIN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_GBPinYin;
				break;
#endif
#if defined(__MMI_GB_TR_CHINESE__)                                                                                        
			case INPUT_MODE_TR_BOPOMO:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_GB_TR_BPMF;
				break;
			case INPUT_MODE_TR_STROKE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_GB_TR_STROKE;
				break;
#endif

#if defined(__MMI_GB_ENGLISH__)
			case INPUT_MODE_MULTITAP_UPPERCASE_ABC:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_ABC;
				break;
			case INPUT_MODE_MULTITAP_LOWERCASE_ABC:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_abc;
				break;


			case INPUT_MODE_SMART_UPPERCASE_ABC:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_GB_SABC;
				break;

			case INPUT_MODE_SMART_LOWERCASE_ABC:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_GBSabc;
				break;
#if defined(__MMI_SMART_FRENCH__)    
				case INPUT_MODE_SMART_UPPERCASE_FRENCH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_FRENCH;
				break;
			case INPUT_MODE_SMART_LOWERCASE_FRENCH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_french;
				break;
#endif
#if defined(__MMI_SMART_URDU__)    
				case INPUT_MODE_SMART_URDU:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_urdu;
				break;
#endif
#if defined(__MMI_SMART_PUNJABI__)
				case INPUT_MODE_SMART_PUNJABI:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_punjabi;
				break;
#endif
#if defined(__MMI_SMART_BENGALI__)
				case INPUT_MODE_SMART_BENGALI:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_bengali;
				break;
#endif
#if defined(__MMI_SMART_TAMIL__)
				case INPUT_MODE_SMART_TAMIL:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_tamil;
				break;
#endif
#if defined(__MMI_SMART_TURKISH__)
			case INPUT_MODE_SMART_UPPERCASE_TURKISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_TURKISH;
				break;
			case INPUT_MODE_SMART_LOWERCASE_TURKISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_turkish;
				break;
#endif
#if defined(__MMI_SMART_INDONESIAN__)
			case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_INDONESIAN;
				break;
			case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_INDONESIAN;
				break;
#endif
#if defined(__MMI_SMART_MALAY__)
			case INPUT_MODE_SMART_UPPERCASE_MALAY:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_MALAY;
				break;
			case INPUT_MODE_SMART_LOWERCASE_MALAY:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_MALAY;
				break;
#endif
// beginning--added by kecx for adding italian on 20081223
#if defined(__MMI_SMART_ITALIAN__)    
				case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_ITALIAN;
				break;
			case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_italian;
				break;
#endif
// end--added by kecx for adding italian on 20081223
#if defined(__MMI_SMART_RUSSIAN__)    
			case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_RUSSIAN;
				break;
			case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_russian;
				break;
#endif

#if defined(__MMI_GB_SMART_PORTUGUESE__)
			case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE :
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_PORTUGUESE;
				break;
			case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_portuguese;
				break;
#endif


#if defined(__MMI_SMART_THAI__)
			case INPUT_MODE_SMART_THAI:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_thai;
				break;
#endif

#if defined(__MMI_SMART_ARABIC__)        
			case  INPUT_MODE_SMART_ARABIC:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_arabic;
				break;
#endif
#if defined(__MMI_MULTITAP_ARABIC__)
			case INPUT_MODE_MULTITAP_ARABIC:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_arabic;
				break;
#endif
#if defined(__MMI_SMART_PERSIAN__)
			case INPUT_MODE_SMART_PERSIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_persian;
				break;
#endif

#if defined(__MMI_SMART_VIETNAMESE__)
			case  INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_vietnamese;
				break;
			case  INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_VIETNAMESE;
				break;
#endif


#endif
#if defined(__MMI_MULTITAP_SPANISH__)
			case INPUT_MODE_MULTITAP_UPPERCASE_SPANISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_SPANISH;
				break;
			case INPUT_MODE_MULTITAP_LOWERCASE_SPANISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_spanish;
				break;
#endif


//qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
			case INPUT_MODE_MMI_MULTITAP_TAMIL:
				preFuncPtrs[i] = wgui_change_inputbox_mode_multitap_tamil;
				break;
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
			case INPUT_MODE_MMI_MULTITAP_BENGALI:
				preFuncPtrs[i] = wgui_change_inputbox_mode_multitap_bengali;
				break;
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
			case INPUT_MODE_MMI_MULTITAP_PUNJABI:
				preFuncPtrs[i] = wgui_change_inputbox_mode_multitap_punjabi;
				break;
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
			case INPUT_MODE_MMI_MULTITAP_TELUGU:
				preFuncPtrs[i] = wgui_change_inputbox_mode_multitap_telugu;
				break;
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
			case INPUT_MODE_MMI_MULTITAP_KANNADA:
				preFuncPtrs[i] = wgui_change_inputbox_mode_multitap_kannada;
				break;
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
			case INPUT_MODE_MMI_MULTITAP_MALAYALAM:
				preFuncPtrs[i] = wgui_change_inputbox_mode_multitap_malayalam;
				break;
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
			case INPUT_MODE_MMI_MULTITAP_ORIYA:
				preFuncPtrs[i] = wgui_change_inputbox_mode_multitap_oriya;
				break;
#endif
//qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
			case INPUT_MODE_MMI_MULTITAP_MARATHI:
				preFuncPtrs[i] = wgui_change_inputbox_mode_multitap_marathi;
				break;
#endif

#if defined(__MMI_MULTITAP_URDU__)
			case INPUT_MODE_MMI_MULTITAP_URDU:
				preFuncPtrs[i] = wgui_change_inputbox_mode_multitap_urdu;
				break;
#endif

 
#if defined(__MMI_MULTITAP_DANISH__)
			case INPUT_MODE_MULTITAP_UPPERCASE_DANISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_DANISH;
				break;
			case INPUT_MODE_MULTITAP_LOWERCASE_DANISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_danish;
				break;
#endif
 
#if defined(__MMI_MULTITAP_FRENCH__)
			case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_FRENCH;
				break;
			case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_french;
				break;
#endif

#if defined(__MMI_MULTITAP_MALAYALAM__)
			case INPUT_MODE_MULTITAP_MYANMAR:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_myanmar;
				break;
#endif
#if defined(__MMI_MULTITAP_GERMAN__)
			case INPUT_MODE_MULTITAP_UPPERCASE_GERMAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_GERMAN;
				break;
			case INPUT_MODE_MULTITAP_LOWERCASE_GERMAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_german;
				break;
#endif
#if defined(__MMI_MULTITAP_ITALIAN__)
			case INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_ITALIAN;
				break;
			case INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_italian;
				break;
#endif
#if defined(__MMI_MULTITAP_RUSSIAN__)
			case INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_RUSSIAN;
				break;
			case INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_russian;
				break;
#endif
#if defined(__MMI_MULTITAP_TURKISH__)
			case INPUT_MODE_MULTITAP_UPPERCASE_TURKISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_TURKISH;
				break;
			case INPUT_MODE_MULTITAP_LOWERCASE_TURKISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_turkish;
				break;
#endif
#if defined(__MMI_MULTITAP_PORTUGUESE__)
			case INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_PORTUGUESE;
				break;
			case INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_portuguese;
				break;
#endif
#if defined(__MMI_MULTITAP_INDONESIAN__)
			case INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_INDONESIAN;
				break;
			case INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_indonesian;
				break;
#endif
#if defined(__MMI_MULTITAP_MALAY__)
			case INPUT_MODE_MULTITAP_UPPERCASE_MALAY:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_MALAY;
				break;
			case INPUT_MODE_MULTITAP_LOWERCASE_MALAY:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_malay;
				break;
#endif
#if defined(__MMI_MULTITAP_VIETNAMESE__)
			case INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_VIETNAMESE;
				break;
			case INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_vietnamese;
				break;
#endif
#if defined(__MMI_MULTITAP_HINDI__)
		case	INPUT_MODE_MULTITAP_HINDI:
				preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_HINDI;
				break;
#endif

//begining:guoyt added for adding Hindi,Spanish and German IME
#if defined(__MMI_SMART_HINDI__)
		case	INPUT_MODE_SMART_HINDI:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_hindi;	
				break;
#endif
#if defined(__MMI_SMART_SPANISH__)
		case INPUT_MODE_SMART_UPPERCASE_SPANISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_SPANISH;
				break;
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_spanish;
				break;
#endif
#if defined(__MMI_SMART_GERMAN__)
		case INPUT_MODE_SMART_UPPERCASE_GERMAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_GERMAN;
				break;
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_german;
				break;
#endif
//end:guoyt added for adding Hindi,Spanish and German IME	
#if defined(__MMI_SMART_TELUGU__)
		case	INPUT_MODE_SMART_TELUGU:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_telugu;	
				break;
#endif
 
#if defined(__MMI_MULTITAP_THAI__)
      case INPUT_MODE_MULTITAP_THAI:
            preFuncPtrs[j] = wgui_change_inputbox_mode_multitap_thai;
				break;
#endif
 
#if defined(__MMI_T9_ENGLISH__)||defined(__MMI_ZI_PRC_ENGLISH__)||defined(__MMI_ZI_UK_ENGLISH__)||defined(__MMI_ITAP_ENGLISH__)
			case INPUT_MODE_SMART_UPPERCASE_ABC:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_ABC;
				break;
			case INPUT_MODE_SMART_LOWERCASE_ABC:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_abc;
				break;
#endif
			case INPUT_MODE_123:
				preFuncPtrs[j] = wgui_change_inputbox_mode_mode_numeric;
				break;
#if defined(__MMI_T9_SPANISH__)||defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)
			case INPUT_MODE_SMART_UPPERCASE_SPANISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_SPANISH;
				break;
			case INPUT_MODE_SMART_LOWERCASE_SPANISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_spanish;
				break;
#endif
 
#if defined(__MMI_T9_DANISH__)||defined(__MMI_ZI_DANISH__)
			case INPUT_MODE_SMART_UPPERCASE_DANISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_DANISH;
				break;
			case INPUT_MODE_SMART_LOWERCASE_DANISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_danish;
				break;
#endif
 
#if defined(__MMI_T9_FRENCH__)||defined(__MMI_ZI_CA_FRENCH__)||defined(__MMI_ZI_EU_FRENCH__)
			case INPUT_MODE_SMART_UPPERCASE_FRENCH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_FRENCH;
				break;
			case INPUT_MODE_SMART_LOWERCASE_FRENCH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_french;
				break;
#endif
#if defined(__MMI_T9_GERMAN__)||defined(__MMI_ZI_GERMAN__)
			case INPUT_MODE_SMART_UPPERCASE_GERMAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_GERMAN;
				break;
			case INPUT_MODE_SMART_LOWERCASE_GERMAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_german;
				break;
#endif
#if defined(__MMI_T9_ITALIAN__)||defined(__MMI_ZI_ITALIAN__)
			case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_ITALIAN;
				break;
			case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_italian;
				break;
#endif
 
#if  defined(__MMI_T9_RUSSIAN__) || defined(__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
//END 
			case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_RUSSIAN;
				break;
			case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_russian;
				break;
#endif
#if defined(__MMI_ZI_THAI__) || defined(__MMI_T9_THAI__)
			case INPUT_MODE_SMART_THAI:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_thai;
				break;
#endif
#if defined(__MMI_ZI_ARABIC__)
			case INPUT_MODE_SMART_ARABIC:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_arabic;
				break;
#endif
#if defined(__MMI_ZI_TURKISH__)
			case INPUT_MODE_SMART_UPPERCASE_TURKISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_TURKISH;
				break;
			case INPUT_MODE_SMART_LOWERCASE_TURKISH:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_turkish;
				break;
#endif
 
#if defined(__MMI_T9_PORTUGUESE__)||defined(__MMI_ZI_EU_PORTUGUESE__)||defined(__MMI_ZI_SA_PORTUGUESE__)||defined(__MMI_ZI_BZ_PORTUGUESE__)
 
			case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_PORTUGUESE;
				break;
			case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_portuguese;
				break;
#endif
 
#if defined(__MMI_ZI_INDONESIAN__)||defined(__MMI_T9_INDONESIAN__) 
			case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_INDONESIAN;
				break;
			case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_indonesian;
				break;
#endif
 
#if defined(__MMI_ZI_MALAY__)
			case INPUT_MODE_SMART_UPPERCASE_MALAY:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_MALAY;
				break;
			case INPUT_MODE_SMART_LOWERCASE_MALAY:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_malay;
				break;
#endif
 
#if defined(__MMI_ZI_VIETNAMESE__)||defined(__MMI_T9_VIETNAMESE__)
 
			case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_VIETNAMESE;
				break;
			case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_smart_vietnamese;
				break;
#endif
#if defined(__MMI_T9_TR_CHINESE__)||defined(__MMI_ZI_TR_CHINESE__)||defined(__MMI_ITAP_TR_CHINESE__)
#if defined(__MMI_T9_MULTITAP_PHONETIC_INPUT__)||defined(__MMI_ZI_MULTITAP_PHONETIC_INPUT__)||defined(__MMI_ITAP_MULTITAP_PHONETIC_INPUT__)
			case INPUT_MODE_TR_MULTITAP_BOPOMO:
				preFuncPtrs[j] = wgui_change_inputbox_mode_tr_multitap_bpmf;
				break;
#endif
#if (defined(__MMI_T9_SMART_PHONETIC_INPUT__) && defined(__MMI_T9_AMB_BPMF_INPUT__))||defined(__MMI_ZI_SMART_PHONETIC_INPUT__)||defined(__MMI_ITAP_SMART_PHONETIC_INPUT__)
			case INPUT_MODE_TR_BOPOMO:
				preFuncPtrs[j] = wgui_change_inputbox_mode_tr_bpmf;
				break;
#endif
			case INPUT_MODE_TR_STROKE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_tr_stroke;
				break;
#endif
#if defined(__MMI_T9_SM_CHINESE__)||defined(__MMI_ZI_SM_CHINESE__)||defined(__MMI_ITAP_SM_CHINESE__)
#if defined(__MMI_T9_MULTITAP_PHONETIC_INPUT__)||defined(__MMI_ZI_MULTITAP_PHONETIC_INPUT__)||defined(__MMI_ITAP_MULTITAP_PHONETIC_INPUT__)
			case INPUT_MODE_SM_MULTITAP_PINYIN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_sm_multitap_pinyin;
				break;
#endif
#if defined(__MMI_T9_SMART_PHONETIC_INPUT__)||defined(__MMI_ZI_SMART_PHONETIC_INPUT__)||defined(__MMI_ITAP_SMART_PHONETIC_INPUT__)
			case INPUT_MODE_SM_PINYIN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_sm_pinyin;
				break;
#endif
			case INPUT_MODE_SM_STROKE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_sm_stroke;
				break;
#endif

#if defined(__MMI_KA__)
			case INPUT_MODE_SM_PINYIN:
				preFuncPtrs[j] = wgui_change_inputbox_mode_sm_pinyin;
				break;
			case INPUT_MODE_SM_STROKE:
				preFuncPtrs[j] = wgui_change_inputbox_mode_sm_stroke;
				break;
#endif
#if defined(__MMI_WCSS_INPUT_FORMAT_SUPPORT__)
			case INPUT_MODE_MULTITAP_UPPERCASE_ABC_NO_NUMERIC:
			case INPUT_MODE_MULTITAP_LOWERCASE_ABC_NO_NUMERIC:
			case INPUT_MODE_123_SYMBOLS:
#endif
			case (0xff):
				break;
			default :
				MMI_ASSERT(0);
		}
		if ((0xff)!=IMEModeArray[i].IME_Mode_ID
#if defined(__MMI_WCSS_INPUT_FORMAT_SUPPORT__)
			&&INPUT_MODE_MULTITAP_UPPERCASE_ABC_NO_NUMERIC!=IMEModeArray[i].IME_Mode_ID
			&&INPUT_MODE_MULTITAP_LOWERCASE_ABC_NO_NUMERIC!=IMEModeArray[i].IME_Mode_ID
			&&INPUT_MODE_123_SYMBOLS!=IMEModeArray[i].IME_Mode_ID
#endif
			)
		{
			if (MMI_TRUE==EnglishOnly)
			{
				if (IMEModeArray[i].English_Only_Flag)
				{
					postPtrs[j]=InputMethodScreenCloseFunction;
					j++;
				}
				i++;
			}
			else
			{
				postPtrs[j]=InputMethodScreenCloseFunction;
				i++;
				j++;
			}
		}
		else
		{
			break;
		}
	}
	return j;
}

void InputMethodScreenHandleLSK(void)
{
	U32 i;
	FuncPtr currPreFuncPtr = NULL;
	FuncPtr currPostFuncPtr = NULL;
	S32 index=GetHighlightedItem();
	 
	i=0;
	
	i = InputMethodSetKeyHandler(preHandleInputMethodLSKFuncPtrs,postHandleInputMethodLSKFuncPtrs,gIMEModeArray,MMI_FALSE);

#if !defined(__MMI_TOUCH_SCREEN__)
	postHandleInputMethodLSKFuncPtrs[i]=UI_dummy_function;
	preHandleInputMethodLSKFuncPtrs[i++] = EntrySymbolPickerScreenFromMenuList;
#endif

	currPreFuncPtr = preHandleInputMethodLSKFuncPtrs[index];
	currPostFuncPtr = postHandleInputMethodLSKFuncPtrs[index];
    
	if(currPreFuncPtr!= NULL)
	{
		(*currPreFuncPtr)();
	}
	if(currPostFuncPtr!= NULL)
	{
		(*currPostFuncPtr)();
	}
}



void InputMethodScreenEnglishOnlyHandleLSK(void)
{
       U32 i;
       FuncPtr currPreFuncPtr = NULL;
       FuncPtr currPostFuncPtr = NULL;
	S32 index=GetHighlightedItem();
       
	i=0;

	i = InputMethodSetKeyHandler(preHandleInputMethodLSKFuncPtrs,postHandleInputMethodLSKFuncPtrs,gIMEModeArray,MMI_TRUE);

#if !defined(__MMI_TOUCH_SCREEN__)
      postHandleInputMethodLSKFuncPtrs[i]=UI_dummy_function;
      preHandleInputMethodLSKFuncPtrs[i++] = EntrySymbolPickerScreenFromMenuList;
#endif

      currPreFuncPtr = preHandleInputMethodLSKFuncPtrs[index];
      currPostFuncPtr = postHandleInputMethodLSKFuncPtrs[index];

      if(currPreFuncPtr!= NULL)
       (*currPreFuncPtr)();
      if(currPostFuncPtr!= NULL)
       (*currPostFuncPtr)();
      //CSD end
}



void RegisterInputMethodScreenCloseFunction(void (*f)(void))
{
	if(f!=NULL) InputMethodScreenCloseFunction=f;
}

void InputMethodScreenHandleRSK(void)
{
	SetSavedInputType();
 
//Issue:
//if set them to UI_dummy_function, will cause following issue:
//user cannot select desired input mode while entering input method menu list, pressing RSK-Back,
//and entering input method menu again.
//pressing RSK will reset following function pointers to be UI_dummy_function.
//Solution:
//The xxxchange_inputbox_modexxxx will be re-initialized in different editing screen, so it
//not required to set them to UI_dummy_function while pressing RSK-Back
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
       #if defined(__MMI_T9__) || defined(__MMI_ZI__)
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
/* under construction !*/
#endif
	GoBackHistory();
}

extern void handle_inline_full_screen_edit_complete(void);
extern void handle_inline_full_screen_edit_cancel(void);
extern wgui_inline_item	*current_wgui_inline_item;
extern s32	wgui_n_inline_items;
extern s32 wgui_inline_item_highlighted_index;

void ConfirmInlineFullScreenEdit(void)
{

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
	if((current_wgui_inline_item==NULL)||(wgui_n_inline_items<=0))
		GoBackHistory();
	handle_inline_full_screen_edit_complete();
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
#endif
}

void CancelInlineFullScreenEdit(void)
{
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
	if((current_wgui_inline_item==NULL)||(wgui_n_inline_items<=0))
		GoBackHistory();
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
#endif
	handle_inline_full_screen_edit_cancel();
}

void EntryInputMethodScreen(void)
{
	U8* guiBuffer;
	U16 nNumofItem=0,i=0;
	U16 nStrItemList[INPUT_MODE_MAX_NUM];
	void (*LSK_handler)(void);

	//ExecuteCurrExitHandler();
	EntryNewScreen(SCR_INPUT_METHOD_LIST, ExitInputMethodScreen, NULL, NULL);
TRACE_EF(1,"----------kecx----MMI_english_input_mode_flag=%d",MMI_english_input_mode_flag);
	if(MMI_english_input_mode_flag)		/*	English only input methods menu		*/
	{
		for (;;)
		{
			if (((0xff) != gIMEModeArray[i].IME_Mode_ID)&&(0 != gIMEModeArray[i].Common_Screen_StringID))
			{
				if (gIMEModeArray[i].English_Only_Flag)
				{
					nStrItemList[nNumofItem] = gIMEModeArray[i].Common_Screen_StringID;
					nNumofItem++;
			TRACE_EF(1,"----------kecx----nNumofItem=%d",nNumofItem);
				}
			}
			else
			{
				break;
			}
			i++;
		}
#if !defined(__MMI_TOUCH_SCREEN__)
		nStrItemList[nNumofItem] = STR_INPUT_METHOD_MENU_INSERT_SYMBOLS;
		nNumofItem++;
#endif
		LSK_handler=InputMethodScreenEnglishOnlyHandleLSK;
	}
	else								/*	English-Chinese input methods menu	*/
	{
		for (;;)
		{
			if ((0xff) != gIMEModeArray[i].IME_Mode_ID&&(0 != gIMEModeArray[i].Common_Screen_StringID))
			{
				nStrItemList[nNumofItem] = gIMEModeArray[i].Common_Screen_StringID;
				nNumofItem++;
				TRACE_EF(1,"----------kecx----nNumofItem=%d",nNumofItem);
			}
			else
			{
				break;
			}
			i++;
		}
#if !defined(__MMI_TOUCH_SCREEN__)
		nStrItemList[nNumofItem] = STR_INPUT_METHOD_MENU_INSERT_SYMBOLS;
		nNumofItem++;
#endif
		LSK_handler=InputMethodScreenHandleLSK;
	}
        //CSD end
	 
#if 0//defined __MMI_LANG_HINDI__ && defined __MMI_LANG_ARABIC__
//guoyt modify:some bug arise from these codes controlled by "defined __MMI_LANG_HINDI__  && defined __MMI_LANG_ARABIC__". 
      if(HINDI_PRESENT || ARABIC_PRESENT)
      {
         int Item_counter=0;
         int string_found=0;
         if(ARABIC_PRESENT)
         {
            while(Item_counter!=nNumofItem)
            {
#if defined(__MMI_SMART_HINDI__)            												
               if(nStrItemList[Item_counter]==STR_INPUT_METHOD_MENU_SMART_HINDI)
#elif defined(__MMI_MULTITAP_HINDI__)
		if(nStrItemList[Item_counter]==STR_INPUT_METHOD_MENU_MULTITAP_HINDI)
#endif
			   string_found = 1;
               nStrItemList[Item_counter]=nStrItemList[Item_counter+string_found];
               Item_counter++;
            }
         }
         else
         {
            while(Item_counter!=nNumofItem)
            {
               if(nStrItemList[Item_counter]==STR_INPUT_METHOD_MENU_SMART_ARABIC)
                  string_found = 1;
               nStrItemList[Item_counter]=nStrItemList[Item_counter+string_found];
               Item_counter++;
            }
         }
      }
#endif
TRACE_EF(1,"----------kecx----nNumofItem=%d",nNumofItem);
	 
	saved_MMI_current_input_type=MMI_current_input_type;
	guiBuffer = GetCurrGuiBuffer(SCR_INPUT_METHOD_LIST);
	if(guiBuffer!=NULL) InputMethodScreenCloseFunction=SavedInputMethodScreenCloseFunction;
    ShowCategory15Screen(STR_GLOBAL_INPUT_METHOD, gInputMethodAndDoneCaptionIcon,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, (PU16)gIndexIconsImageList,
						  LIST_MENU, 0, guiBuffer);
	SetLeftSoftkeyFunction(LSK_handler,KEY_EVENT_UP);
	SetRightSoftkeyFunction(InputMethodScreenHandleRSK,KEY_EVENT_UP);
	SetKeyHandler(LSK_handler, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(InputMethodScreenHandleRSK, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	//SetExitHandler(SCR_INPUT_METHOD_LIST, ExitInputMethodScreen);
}

void ExitInputMethodScreen(void)
{
	history_t		h;
	S16			nHistory=0;

	SavedInputMethodScreenCloseFunction=InputMethodScreenCloseFunction;
 
//Issue:
//if set them to UI_dummy_function, will cause following issue:
//user cannot select desired input mode while entering input method menu list, pressing RSK-Back,
//and entering input method menu again.
//pressing RSK will reset following function pointers to be UI_dummy_function.
//Solution:
//The InputMethodScreenCloseFunction will be re-initialized in different editing screen, so it
//not required to set them to UI_dummy_function while pressing RSK-Back
#if 0
/* under construction !*/
#endif
	h.scrnID = SCR_INPUT_METHOD_LIST;
	h.entryFuncPtr = EntryInputMethodScreen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}
 

//START 
#if defined(__MMI_MULTITAP_HINDI__)
#define VOWEL_PICKER_MAX_HINDI_SYMBOLS			27
#define VOWEL_PICKER_LF_INDEX					(VOWEL_PICKER_MAX_HINDI_SYMBOLS-1)
#define VOWEL_PICKER_SPACE_INDEX				(VOWEL_PICKER_MAX_HINDI_SYMBOLS-2)
#define IND_VOWEL_PICKER_MAX_HINDI_SYMBOL		15
#define IND_VOWEL_PICKER_LF_INDEX				(IND_VOWEL_PICKER_MAX_HINDI_SYMBOL-1)
#define IND_VOWEL_PICKER_SPACE_INDEX			(IND_VOWEL_PICKER_MAX_HINDI_SYMBOL-2)
#endif
//END 
#define SYMBOL_PICKER_MAX_SYMBOLS				36
#define SYMBOL_PICKER_LF_INDEX					35
#define SYMBOL_PICKER_SPACE_INDEX				34


#define SYMBOL_PICKER_MAX_CHINESE_SYMBOLS		28
#define SYMBOL_PICKER_CHINESE_LF_INDEX			27
#define SYMBOL_PICKER_CHINESE_SPACE_INDEX		26


const U8* SymbolPickerSymbols[SYMBOL_PICKER_MAX_SYMBOLS]=
{
	 (U8*)"\x2e\0\0",
     (U8*)"\x2c\0\0",
     (U8*)"?\0\0",
     (U8*)"!\0\0",
     (U8*)"\'\0\0",
     (U8*)"\"\0\0",
     (U8*)"(\0\0",
     (U8*)")\0\0",
     (U8*)"@\0\0",
     (U8*)"\\\0\0",
     (U8*)"/\0\0",
     (U8*)":\0\0",
     (U8*)"_\0\0",
     (U8*)";\0\0",
     (U8*)"+\0\0",
     (U8*)"-\0\0",
     (U8*)"*\0\0",
     (U8*)"=\0\0",
     (U8*)"%\0\0",
     (U8*)"&\0\0",
     (U8*)"<\0\0",
     (U8*)">\0\0",
     (U8*)"\xa3\0\0",
     (U8*)"\x24\0\0",
     (U8*)"\xa5\0\0",
     (U8*)"\xa7\0\0",
     /*zhoumn Modify Start For 5565 Ver: TBM_780  on 2007-8-9 15:1 */
     #if 0
     //(U8*)"\xa2\0\0",
     (U8*)"\xac\x20\0\0",
     #else
     (U8*)"\xa2\0\0",
     //(U8*)"\xac\x20\0\0",
	 #endif
     /*zhoumn Modify End  For 5565 Ver: TBM_780  on 2007-8-9 15:1 */
     (U8*)"\xa4\0\0",
     (U8*)"[\0\0",
     (U8*)"]\0\0",
     (U8*)"{\0\0",
     (U8*)"}\0\0",
     (U8*)"~\0\0",
     (U8*)"#\0\0",
     (U8*)" \0\0",
     (U8*)"\n\0\0",
     //(U8*)" \0\0",
};

//START 
#if defined(__MMI_MULTITAP_HINDI__)
const U8* VowelPicker[VOWEL_PICKER_MAX_HINDI_SYMBOLS]=
{
	 (U8*)"\x05\x09\0\0",
     (U8*)"\x06\x09\0\0",
     (U8*)"\x07\x09\0\0",
     (U8*)"\x08\x09\0\0",
     (U8*)"\x09\x09\0\0",
     (U8*)"\x0A\x09\0\0",
     (U8*)"\x0B\x09\0\0",
     (U8*)"\x0D\x09\0\0",
     (U8*)"\x0F\x09\0\0",
     (U8*)"\x10\x09\0\0",
     (U8*)"\x11\x09\0\0",
     (U8*)"\x13\x09\0\0",
     (U8*)"\x14\x09\0\0",
     (U8*)"\x3E\x09\0\0",
     (U8*)"\x3F\x09\0\0",
     (U8*)"\x40\x09\0\0",
     (U8*)"\x41\x09\0\0",
     (U8*)"\x42\x09\0\0",
     (U8*)"\x43\x09\0\0",
     (U8*)"\x45\x09\0\0",
     (U8*)"\x47\x09\0\0",
     (U8*)"\x48\x09\0\0",
     (U8*)"\x49\x09\0\0",
     (U8*)"\x4B\x09\0\0",
     (U8*)"\x4C\x09\0\0",
	 (U8*)" \0\0",
     (U8*)"\n\0\0",
};

const U8* IndVowelPicker[IND_VOWEL_PICKER_MAX_HINDI_SYMBOL]=
{
	 (U8*)"\x05\x09\0\0",
     (U8*)"\x06\x09\0\0",
     (U8*)"\x07\x09\0\0",
     (U8*)"\x08\x09\0\0",
     (U8*)"\x09\x09\0\0",
     (U8*)"\x0A\x09\0\0",
     (U8*)"\x0B\x09\0\0",
     (U8*)"\x0D\x09\0\0",
     (U8*)"\x0F\x09\0\0",
     (U8*)"\x10\x09\0\0",
     (U8*)"\x11\x09\0\0",
     (U8*)"\x13\x09\0\0",
     (U8*)"\x14\x09\0\0",
     (U8*)" \0\0",
     (U8*)"\n\0\0",
};
#endif
//END 

//using full-width forms instead of CJK compatibility forms
//#if defined(__MMI_FULLWIDTH_FORMS__)
const U8* SymbolPickerChineseSymbols[SYMBOL_PICKER_MAX_CHINESE_SYMBOLS]=
{
     (U8*)"\x0c\xff\0\0",
     (U8*)"\x02\x30\0\0",//using CJK defned value instead of full-width defined value(U8*)"\x61\xff\0\0",
     (U8*)"\x01\x30\0\0",//using CJK defned value instead of full-width defined value(U8*)"\x64\xff\0\0",
     (U8*)"\x1a\xff\0\0",
     (U8*)"\x1b\xff\0\0",
     (U8*)"\x1f\xff\0\0",
     (U8*)"\x26\x20\0\0",
     (U8*)"\x01\xff\0\0",
     (U8*)"\x0a\x30\0\0",
     (U8*)"\x0b\x30\0\0",
     (U8*)"\x0e\x30\0\0",
     (U8*)"\x0f\x30\0\0",
     (U8*)"\x1c\x20\0\0",
     (U8*)"\x1d\x20\0\0",
     (U8*)"\x08\xff\0\0",
     (U8*)"\x09\xff\0\0",
     (U8*)"\x0c\x30\0\0",//using CJK defned value instead of full-width defined value(U8*)"\x62\xff\0\0",
     (U8*)"\x0d\x30\0\0",//using CJK defned value instead of full-width defined value(U8*)"\x63\xff\0\0",
     (U8*)"\x18\x20\0\0",
     (U8*)"\x19\x20\0\0",
     (U8*)"\x0e\xff\0\0",
     (U8*)"\x1c\xff\0\0",
     (U8*)"\x1e\xff\0\0",
     (U8*)"\x5e\xff\0\0",
     (U8*)"\x3b\xff\0\0",
     (U8*)"\x3d\xff\0\0",
     (U8*)"\x20\x00\0\0",
     (U8*)"\n\0\0",
};
//#else
#if 0  //using full-width forms instead of CJK compatibility forms
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
//#endif


const U8	SymbolPickerLFString[]={	0x85,0,'\0',0	};
const U8    SymbolPickerSpaceString[]={    0x86,0,'\0',0   };

const U8*	SymbolPickerDisplaySymbols[SYMBOL_PICKER_MAX_SYMBOLS];
//START 
#if defined(__MMI_MULTITAP_HINDI__)
const U8*	SymbolPickerDisplayHindiSymbols[VOWEL_PICKER_MAX_HINDI_SYMBOLS];
#endif
//END 
const U8*	SymbolPickerDisplayChineseSymbols[SYMBOL_PICKER_MAX_CHINESE_SYMBOLS];

extern void GbKeyRSKPressHandler(void);
void SymbolPickerHandleLSK(void)
{
	UI_character_type selected_symbol;
	S32 index = GetHighlightedItem();
	
#if 0//defined (__MMI_MULTITAP_HINDI__)
	UI_character_type previous_char;
#endif

	switch (MMI_current_input_type & INPUT_TYPE_MASK)
	{
		case INPUT_TYPE_TR_BOPOMO:
		case INPUT_TYPE_SM_PINYIN:
		case INPUT_TYPE_TR_MULTITAP_BOPOMO:
		case INPUT_TYPE_SM_MULTITAP_PINYIN:
		case INPUT_TYPE_TR_STROKE:
		case INPUT_TYPE_SM_STROKE:
			selected_symbol = ((UI_character_type)SymbolPickerChineseSymbols[index][0]) | (((UI_character_type)SymbolPickerChineseSymbols[index][1]) << 8);
#if defined(__MMI_GB__)
			if (index == SYMBOL_PICKER_CHINESE_LF_INDEX)  //此时为中文状态下回车标点符号
				EnterSymboFlag = TRUE;
#endif
		break;

#if 0//defined (__MMI_MULTITAP_HINDI__)		 
		case INPUT_TYPE_MULTITAP_HINDI:
			if (whether_use_multitap())
				previous_char = gui_multi_line_current_character(&MMI_multiline_inputbox);    
			else
				previous_char = coolsand_UI_EMS_current_character(&MMI_EMS_inputbox);
			if (HF_HINDI_RANGE(previous_char) && IdentifyCharacter(previous_char) == CON)
				selected_symbol = ((UI_character_type)VowelPicker[index][0]) | (((UI_character_type)VowelPicker[index][1]) << 8);
			else
				selected_symbol = ((UI_character_type)IndVowelPicker[index][0]) | (((UI_character_type)IndVowelPicker[index][1]) << 8);
		break;
#endif

		default:
			selected_symbol = ((UI_character_type)SymbolPickerSymbols[index][0]) | (((UI_character_type)SymbolPickerSymbols[index][1]) << 8);
#if defined(__MMI_GB__)
			if (index == SYMBOL_PICKER_LF_INDEX)  //此时为英文状态下回车标点符号
				EnterSymboFlag = TRUE;
#endif
		break;
	}

	TRACE_EF(1, "index = %d, slected_symbol = %x", index, selected_symbol);
	GoBackHistory();
	wgui_inputbox_insert_symbol(selected_symbol);
	switch (MMI_current_input_type & INPUT_TYPE_MASK)
	{
		case INPUT_TYPE_TR_BOPOMO:
		case INPUT_TYPE_SM_PINYIN:
		case INPUT_TYPE_TR_MULTITAP_BOPOMO:
		case INPUT_TYPE_SM_MULTITAP_PINYIN:
		case INPUT_TYPE_TR_STROKE:
		case INPUT_TYPE_SM_STROKE:
		case INPUT_TYPE_SMART_UPPERCASE_ABC:
		case INPUT_TYPE_SMART_LOWERCASE_ABC:
		 
#if  defined(__MMI_T9_SPANISH__)||defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)
		case INPUT_TYPE_SMART_UPPERCASE_SPANISH:
		case INPUT_TYPE_SMART_LOWERCASE_SPANISH:
#endif
		 
#if  defined(__MMI_T9_DANISH__)||defined(__MMI_ZI_DANISH__)
		case INPUT_TYPE_SMART_UPPERCASE_DANISH:
		case INPUT_TYPE_SMART_LOWERCASE_DANISH:
#endif
		 
#if  defined(__MMI_T9_FRENCH__)||defined(__MMI_ZI_CA_FRENCH__)||defined(__MMI_ZI_EU_FRENCH__)
		case INPUT_TYPE_SMART_UPPERCASE_FRENCH:
		case INPUT_TYPE_SMART_LOWERCASE_FRENCH:
#endif
#if  defined(__MMI_T9_GERMAN__)||defined(__MMI_ZI_GERMAN__)
		case INPUT_TYPE_SMART_UPPERCASE_GERMAN:
		case INPUT_TYPE_SMART_LOWERCASE_GERMAN:
#endif
#if  defined(__MMI_T9_ITALIAN__)||defined(__MMI_ZI_ITALIAN__)
		case INPUT_TYPE_SMART_UPPERCASE_ITALIAN:
		case INPUT_TYPE_SMART_LOWERCASE_ITALIAN:
#endif
		 
#if  defined (__MMI_T9_RUSSIAN__) || defined(__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
 
		case INPUT_TYPE_SMART_UPPERCASE_RUSSIAN:
		case INPUT_TYPE_SMART_LOWERCASE_RUSSIAN:
#endif
		 
#if  defined(__MMI_ZI_THAI__) || defined (__MMI_T9_THAI__)
		case INPUT_TYPE_SMART_THAI:
#endif
		case INPUT_TYPE_SMART_ARABIC:
#if  defined(__MMI_ZI_TURKISH__)
		case INPUT_TYPE_SMART_UPPERCASE_TURKISH:
		case INPUT_TYPE_SMART_LOWERCASE_TURKISH:
#endif
		 
#if defined(__MMI_T9_PORTUGUESE__)||defined(__MMI_ZI_EU_PORTUGUESE__)||defined(__MMI_ZI_SA_PORTUGUESE__)||defined(__MMI_ZI_BZ_PORTUGUESE__)
		 
		case INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE:
		case INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
#endif
		 
#if  defined(__MMI_ZI_INDONESIAN__)||defined(__MMI_T9_INDONESIAN__) 
		case INPUT_TYPE_SMART_UPPERCASE_INDONESIAN:
		case INPUT_TYPE_SMART_LOWERCASE_INDONESIAN:
#endif
		 
#if  defined(__MMI_ZI_MALAY__)
		case INPUT_TYPE_SMART_UPPERCASE_MALAY:
		case INPUT_TYPE_SMART_LOWERCASE_MALAY:
#endif
		 
#if  defined(__MMI_ZI_VIETNAMESE__)||defined(__MMI_T9_VIETNAMESE__) 
		 
		case INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
		case INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
#endif
		//CSD end
		 
#if !defined(__MMI_TOUCH_SCREEN__)
#ifdef __MMI_T9__
		T9SynT9WithInputBox();
#elif defined __MMI_ZI__
		ZiDisplay();
#elif defined __MMI_KA__
		KonkaDisplay();
#elif defined __MMI_ITAP__
		ItapDisplay();
#endif
#endif
		break;
	}
#if defined(__MMI_GB__)    
	//SetRightSoftkeyFunction(GbKeyRSKPressHandler, KEY_EVENT_DOWN);
#endif
}

void SymbolPickerHandleRSK(void)
{
	GoBackHistory();
}
extern s16 MMI_current_input_ext_type;
static s16 SymbolFilterFlag = 0;
void EntrySymbolPickerScreen(void)
{
	U8* guiBuffer;
	
	
   EntryNewScreen(SCR_SYMBOL_PICKER_SCREEN, ExitSymbolPickerScreen, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_SYMBOL_PICKER_SCREEN);
	if(guiBuffer != NULL) MMI_current_input_ext_type = SymbolFilterFlag;
	PrepareSymbolTableAndShowCategory(guiBuffer);
       
	SetLeftSoftkeyFunction(SymbolPickerHandleLSK,KEY_EVENT_UP);
	SetRightSoftkeyFunction(SymbolPickerHandleRSK,KEY_EVENT_UP);

}


void PrepareSymbolTableAndShowCategory(U8* guiBuffer)
{
	S32 i;
#if 0//defined (__MMI_MULTITAP_HINDI__)
	UI_character_type previous_char;
#endif

	    switch(MMI_current_input_type  & INPUT_TYPE_MASK)
       {

         case INPUT_TYPE_TR_BOPOMO:
         case INPUT_TYPE_SM_PINYIN:
         case INPUT_TYPE_TR_MULTITAP_BOPOMO:
         case INPUT_TYPE_SM_MULTITAP_PINYIN:
         case INPUT_TYPE_TR_STROKE:
         case INPUT_TYPE_SM_STROKE:
			for(i=0;i<SYMBOL_PICKER_MAX_CHINESE_SYMBOLS;i++)
			{
				
				if( (MMI_current_input_ext_type & INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL) && 
					(SymbolPickerChineseSymbols[i] == (U8*)"\n\0\0")
				  )
				{
					continue;
				}
				else SymbolPickerDisplayChineseSymbols[i]=SymbolPickerChineseSymbols[i];
			}
			
			SymbolPickerDisplayChineseSymbols[SYMBOL_PICKER_CHINESE_SPACE_INDEX]=SymbolPickerSpaceString;
			if( MMI_current_input_ext_type & INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL )
			{
				ShowCategory22Screen(	STR_INPUT_METHOD_MENU_SELECT_SYMBOL,
										0,
										STR_GLOBAL_OK,
										0,
										STR_GLOBAL_BACK,
										0,
										SYMBOL_PICKER_MAX_CHINESE_SYMBOLS-1,
										(U8**)SymbolPickerDisplayChineseSymbols,
										0,
										guiBuffer	);

			}
			else
			{
				SymbolPickerDisplayChineseSymbols[SYMBOL_PICKER_CHINESE_LF_INDEX]=SymbolPickerLFString;
				ShowCategory22Screen(	STR_INPUT_METHOD_MENU_SELECT_SYMBOL,
										0,
										STR_GLOBAL_OK,
										0,
										STR_GLOBAL_BACK,
										0,
										SYMBOL_PICKER_MAX_CHINESE_SYMBOLS,
										(U8**)SymbolPickerDisplayChineseSymbols,
										0,
										guiBuffer	);			
			}
	      break;
#if 0//defined(__MMI_MULTITAP_HINDI__)
		 case INPUT_TYPE_MULTITAP_HINDI:
			 if(whether_use_multitap())
				previous_char=gui_multi_line_current_character(&MMI_multiline_inputbox);
			else
				previous_char=coolsand_UI_EMS_current_character(&MMI_EMS_inputbox);
			if( HF_HINDI_RANGE(previous_char)&&IdentifyCharacter(previous_char)==CON)
			{
				for(i=0;i<VOWEL_PICKER_MAX_HINDI_SYMBOLS;i++)
				{
					
					if( (MMI_current_input_ext_type & INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL) && 
						(VowelPicker[i] == (U8*)"\n\0\0")
					  )
					{
						continue;
					}
					else SymbolPickerDisplayHindiSymbols[i]=VowelPicker[i];
				}

				SymbolPickerDisplayHindiSymbols[VOWEL_PICKER_SPACE_INDEX]=SymbolPickerSpaceString;
				if( MMI_current_input_ext_type & INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL )
				{
					ShowCategory22Screen(	STR_INPUT_METHOD_MENU_SELECT_VOWEL,
											0,
											STR_GLOBAL_OK,
											0,
											STR_GLOBAL_BACK,
											0,
											VOWEL_PICKER_MAX_HINDI_SYMBOLS-1,
											(U8**)SymbolPickerDisplayHindiSymbols,
											0,
											guiBuffer	);

				}
				else
				{
					SymbolPickerDisplayHindiSymbols[VOWEL_PICKER_LF_INDEX]=SymbolPickerLFString;
					ShowCategory22Screen(	STR_INPUT_METHOD_MENU_SELECT_VOWEL,
											0,
											STR_GLOBAL_OK,
											0,
											STR_GLOBAL_BACK,
											0,
											VOWEL_PICKER_MAX_HINDI_SYMBOLS,
											(U8**)SymbolPickerDisplayHindiSymbols,
											0,
											guiBuffer	);			
				}				
			}
			else
			{
				for(i=0;i<IND_VOWEL_PICKER_MAX_HINDI_SYMBOL;i++)
				{
					
					if( (MMI_current_input_ext_type & INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL) && 
						(IndVowelPicker[i] == (U8*)"\n\0\0")
					  )
					{
						continue;
					}
					else SymbolPickerDisplayHindiSymbols[i]=IndVowelPicker[i];
				}

				SymbolPickerDisplayHindiSymbols[IND_VOWEL_PICKER_SPACE_INDEX]=SymbolPickerSpaceString;
				if( MMI_current_input_ext_type & INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL )
				{
					ShowCategory22Screen(	STR_INPUT_METHOD_MENU_SELECT_VOWEL,
											0,
											STR_GLOBAL_OK,
											0,
											STR_GLOBAL_BACK,
											0,
											IND_VOWEL_PICKER_MAX_HINDI_SYMBOL-1,
											(U8**)SymbolPickerDisplayHindiSymbols,
											0,
											guiBuffer	);

				}
				else
				{
					SymbolPickerDisplayHindiSymbols[IND_VOWEL_PICKER_LF_INDEX]=SymbolPickerLFString;
					ShowCategory22Screen(	STR_INPUT_METHOD_MENU_SELECT_VOWEL,
											0,
											STR_GLOBAL_OK,
											0,
											STR_GLOBAL_BACK,
											0,
											IND_VOWEL_PICKER_MAX_HINDI_SYMBOL,
											(U8**)SymbolPickerDisplayHindiSymbols,
											0,
											guiBuffer	);			
				}							
			}
			break;
#endif

	  default:
			for(i=0;i<SYMBOL_PICKER_MAX_SYMBOLS;i++)
			{
				
				if( (MMI_current_input_ext_type & INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL) && 
					(SymbolPickerSymbols[i] == (U8*)"\n\0\0")
				  )
				{
					continue;
				}
				else SymbolPickerDisplaySymbols[i]=SymbolPickerSymbols[i];
			}
			SymbolPickerDisplaySymbols[SYMBOL_PICKER_SPACE_INDEX]=SymbolPickerSpaceString;
			if( MMI_current_input_ext_type & INPUT_TYPE_EXT_NO_SHOW_NEW_LINE_SYMBOL )
			{
				ShowCategory22Screen(	STR_INPUT_METHOD_MENU_SELECT_SYMBOL,
										0,
										STR_GLOBAL_OK,
										0,
										STR_GLOBAL_BACK,
										0,
										SYMBOL_PICKER_MAX_SYMBOLS-1,
										(U8**)SymbolPickerDisplaySymbols,
										0,
										guiBuffer	);

			}
			else
			{
				SymbolPickerDisplaySymbols[SYMBOL_PICKER_LF_INDEX]=SymbolPickerLFString;
				ShowCategory22Screen(	STR_INPUT_METHOD_MENU_SELECT_SYMBOL,
										0,
										STR_GLOBAL_OK,
										0,
										STR_GLOBAL_BACK,
										0,
										SYMBOL_PICKER_MAX_SYMBOLS,
										(U8**)SymbolPickerDisplaySymbols,
										0,
										guiBuffer	);			
			}
          break;
       }
}


void ExitSymbolPickerScreen(void)
{
	history_t		h;
	S16			nHistory=0;

	h.scrnID = SCR_SYMBOL_PICKER_SCREEN;
	h.entryFuncPtr = EntrySymbolPickerScreen;
	SymbolFilterFlag = MMI_current_input_ext_type;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

void SymbolPickerHandleLSKFromMenuList(void)
{
    UI_character_type   selected_symbol;
    S32 index = GetHighlightedItem();

#if 0//defined (__MMI_MULTITAP_HINDI__)
	UI_character_type previous_char;
#endif

	switch (MMI_current_input_type & INPUT_TYPE_MASK)
	{
		case INPUT_TYPE_TR_BOPOMO:
		case INPUT_TYPE_SM_PINYIN:
		case INPUT_TYPE_TR_MULTITAP_BOPOMO:
		case INPUT_TYPE_SM_MULTITAP_PINYIN:
		case INPUT_TYPE_TR_STROKE:
		case INPUT_TYPE_SM_STROKE:
			selected_symbol = ((UI_character_type)SymbolPickerChineseSymbols[index][0]) | (((UI_character_type)SymbolPickerChineseSymbols[index][1]) << 8);

#if defined(__MMI_GB__) //added by zhoumn 2007/06/14
			if (index == SYMBOL_PICKER_CHINESE_LF_INDEX) //此时为中文状态下回车标点符号
				EnterSymboFlag = TRUE;
#endif
		break;

#if 0//defined (__MMI_MULTITAP_HINDI__)		 
		case INPUT_TYPE_MULTITAP_HINDI:
			if (whether_use_multitap())
				previous_char = gui_multi_line_current_character(&MMI_multiline_inputbox);    
			else
				previous_char = coolsand_UI_EMS_current_character(&MMI_EMS_inputbox);
			
			if (HF_HINDI_RANGE(previous_char) && IdentifyCharacter(previous_char) == CON)
				selected_symbol = ((UI_character_type)VowelPicker[index][0]) | (((UI_character_type)VowelPicker[index][1]) << 8);
			else
				selected_symbol = ((UI_character_type)IndVowelPicker[index][0]) | (((UI_character_type)IndVowelPicker[index][1]) << 8);
		break;
#endif

		default:
			selected_symbol = ((UI_character_type)SymbolPickerSymbols[index][0]) | (((UI_character_type)SymbolPickerSymbols[index][1]) << 8);
#if defined(__MMI_GB__)  //added by zhoumn 2007/06/14
			if (index == SYMBOL_PICKER_LF_INDEX)  //此时为英文状态下回车标点符号
				EnterSymboFlag = TRUE;
#endif
		break;
	}
       
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
    GoBackHistory();
#if(UI_DOUBLE_BUFFER_SUPPORT)
    coolsand_UI_unlock_double_buffer();
#endif
    InputMethodScreenCloseFunction();
    wgui_inputbox_insert_symbol(selected_symbol);
	switch (MMI_current_input_type & INPUT_TYPE_MASK)
	{
		case INPUT_TYPE_TR_BOPOMO:
		case INPUT_TYPE_SM_PINYIN:
		case INPUT_TYPE_TR_MULTITAP_BOPOMO:
		case INPUT_TYPE_SM_MULTITAP_PINYIN:
		case INPUT_TYPE_TR_STROKE:
		case INPUT_TYPE_SM_STROKE:
		case INPUT_TYPE_SMART_UPPERCASE_ABC:
		case INPUT_TYPE_SMART_LOWERCASE_ABC:
		 
#if  defined(__MMI_T9_SPANISH__)||defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)
		case INPUT_TYPE_SMART_UPPERCASE_SPANISH:
		case INPUT_TYPE_SMART_LOWERCASE_SPANISH:
#endif
		 
#if  defined(__MMI_T9_DANISH__)||defined(__MMI_ZI_DANISH__)
		case INPUT_TYPE_SMART_UPPERCASE_DANISH:
		case INPUT_TYPE_SMART_LOWERCASE_DANISH:
#endif
		 
#if  defined(__MMI_T9_FRENCH__)||defined(__MMI_ZI_CA_FRENCH__)||defined(__MMI_ZI_EU_FRENCH__)
		case INPUT_TYPE_SMART_UPPERCASE_FRENCH:
		case INPUT_TYPE_SMART_LOWERCASE_FRENCH:
#endif
#if  defined(__MMI_T9_GERMAN__)||defined(__MMI_ZI_GERMAN__)
		case INPUT_TYPE_SMART_UPPERCASE_GERMAN:
		case INPUT_TYPE_SMART_LOWERCASE_GERMAN:
#endif
#if  defined(__MMI_T9_ITALIAN__)||defined(__MMI_ZI_ITALIAN__)
		case INPUT_TYPE_SMART_UPPERCASE_ITALIAN:
		case INPUT_TYPE_SMART_LOWERCASE_ITALIAN:
#endif
		 
#if  defined (__MMI_T9_RUSSIAN__) || defined(__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
 
		case INPUT_TYPE_SMART_UPPERCASE_RUSSIAN:
		case INPUT_TYPE_SMART_LOWERCASE_RUSSIAN:
#endif
		 
#if  defined(__MMI_ZI_THAI__) || defined (__MMI_T9_THAI__) 
		case INPUT_TYPE_SMART_THAI:
#endif
		case INPUT_TYPE_SMART_ARABIC:
#if  defined(__MMI_ZI_TURKISH__)
		case INPUT_TYPE_SMART_UPPERCASE_TURKISH:
		case INPUT_TYPE_SMART_LOWERCASE_TURKISH:
#endif
		 
#if defined(__MMI_T9_PORTUGUESE__)||defined(__MMI_ZI_EU_PORTUGUESE__)||defined(__MMI_ZI_SA_PORTUGUESE__)||defined(__MMI_ZI_BZ_PORTUGUESE__)
		 
		case INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE:
		case INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
#endif
		 
#if  defined(__MMI_ZI_INDONESIAN__)||defined(__MMI_T9_INDONESIAN__)
		case INPUT_TYPE_SMART_UPPERCASE_INDONESIAN:
		case INPUT_TYPE_SMART_LOWERCASE_INDONESIAN:
#endif
		 
#if  defined(__MMI_ZI_MALAY__)
		case INPUT_TYPE_SMART_UPPERCASE_MALAY:
		case INPUT_TYPE_SMART_LOWERCASE_MALAY:
#endif
		 
#if  defined(__MMI_ZI_VIETNAMESE__)||defined(__MMI_T9_VIETNAMESE__)  
		 
		case INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
		case INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
#endif
		//CSD end

		 
#if !defined(__MMI_TOUCH_SCREEN__)
#ifdef __MMI_T9__
		T9SynT9WithInputBox();
#elif defined __MMI_ZI__
		ZiDisplay();
#elif defined __MMI_KA__
		KonkaDisplay();
#elif defined __MMI_ITAP__
		ItapDisplay();
#endif
#endif
		break;
	}
#if defined(__MMI_GB__)
	SetRightSoftkeyFunction(GbKeyRSKPressHandler, KEY_EVENT_DOWN);//added by zhoumn for bug 181
#endif
}

void SymbolPickerHandleRSKFromMenuList(void)
{
    GoBackHistory();
}

void EntrySymbolPickerScreenFromMenuList(void)
{
	 U8* guiBuffer;
	 EntryNewScreen(SCR_SYMBOL_PICKER_SCREEN, ExitSymbolPickerScreenFromMenuList, NULL, NULL);
	 guiBuffer = GetCurrGuiBuffer(SCR_SYMBOL_PICKER_SCREEN);
	 
    PrepareSymbolTableAndShowCategory(guiBuffer);

    SetLeftSoftkeyFunction(SymbolPickerHandleLSKFromMenuList,KEY_EVENT_UP);
    SetRightSoftkeyFunction(SymbolPickerHandleRSKFromMenuList,KEY_EVENT_UP);

}

void ExitSymbolPickerScreenFromMenuList(void)
{
    history_t     h;
    S16         nHistory=0;

    h.scrnID = SCR_SYMBOL_PICKER_SCREEN;
    h.entryFuncPtr = EntrySymbolPickerScreenFromMenuList;
    pfnUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
    GetCategoryHistory(h.guiBuffer);
    AddHistory(h);
}



void SetInputMethodAndDoneCaptionIcon( U16 IconId )
{
	gInputMethodAndDoneCaptionIcon = IconId;
}

void EntryScrForInputMethodAndDone(void)
{
	U8* guiBuffer;
	U16 nStrItemList[10];
	U16 numItems;
	//ExecuteCurrExitHandler ();
	EntryNewScreen(SCR_INPUT_METHOD_AND_DONE, ExitScrForInputMethodAndDone, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_INPUT_METHOD_AND_DONE);
	SetParentHandler (MENU_INPUT_METHOD_AND_DONE_OPTION_ID);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	numItems = GetNumOfChild (MENU_INPUT_METHOD_AND_DONE_OPTION_ID);
	GetSequenceStringIds(MENU_INPUT_METHOD_AND_DONE_OPTION_ID, nStrItemList);
	ShowCategory52Screen (STR_GLOBAL_OPTIONS, gInputMethodAndDoneCaptionIcon,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, (PU16)gIndexIconsImageList,
						  NULL , 0, 0, guiBuffer );
      SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP); // wangbei add 20060619
	//SetExitHandler (SCR_INPUT_METHOD_AND_DONE, ExitScrForInputMethodAndDone);
}


void ExitScrForInputMethodAndDone(void)
{
	history_t currHistory;
	S16 ucs2InputBuffer=0;
	currHistory.scrnID = SCR_INPUT_METHOD_AND_DONE;
	currHistory.entryFuncPtr = EntryScrForInputMethodAndDone;
	pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&ucs2InputBuffer);
	GetCategoryHistory (currHistory.guiBuffer);
	AddHistory (currHistory);
//	gInputMethodAndDoneCaptionIcon = 0;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ime_delete_editor_common_scr_id
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_delete_editor_common_scr(U16 scr_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 next_id, old_id = 0xFFFF;
	MMI_BOOL commscr_active = MMI_FALSE;
    BOOL gdi_freeze = gdi_lcd_get_freeze();
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    gdi_lcd_freeze(TRUE);

    #if 0
    if (mmi_ime_is_editor_common_scr(GetActiveScreenId()))
    {
    	commscr_active = MMI_TRUE;
    	EntryNewScreen(0, NULL, NULL, NULL);
    }
    #endif
    
    while(GetNextScrnIdOf(scr_id, &next_id))
    {
        if (old_id == next_id)
        {
            scr_id = next_id;

            if (!GetNextScrnIdOf(scr_id, &next_id))
            {
                break;
            }
        }
    #if 0
        if (mmi_ime_is_editor_common_scr(next_id))
        {
            DeleteScreenIfPresent(next_id);
        }
        else
        {
            break;
        }
    #endif
        old_id = next_id;
    }

	if (commscr_active)
	{
		GoBackHistory();
	}

    gdi_lcd_freeze(gdi_freeze); /* Freeze lcd to avoid to blt temp screen in GoBackHistory() */

	/* 
	 * If GoBackHistory() was going to idle, we should blt, otherwise maybe no one
	 * will call blt function in the following process
	 */
	if (GetActiveScreenId() == IDLE_SCREEN_ID && commscr_active)
	{
		gdi_lcd_repaint_all(); 
	}
}

 
FuncPtr PopupFunc=NULL;
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
BOOL IsPopupSavedCharColor();
void PopupSaveCharColor();
void PopupChangeCharColor();
void PopupResetCharColor();
#endif
extern void ShowCategory65Screen(U8* message,U16 message_icon,U8* history_buffer);
//CSD added by JL 040523, move from events.c
/*****************************************************************
   FUNCTION NAME     : DisplayPopup()

   PURPOSE           : Displays the common pop-up screen used across all applications

   INPUT PARAMETERS  : U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId

   OUTPUT PARAMETERS : nil

   RETURNS           : nil

 

*******************************************************************/
#ifdef __MMI_SCREEN_ROTATE__
void DisplayPopupRotated(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId, mmi_frm_screen_rotate_enum rotation)
#else
void DisplayPopup(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId)
#endif
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	TurnOnBacklight(TRUE); 
   
   //ExecuteCurrExitHandler();
   EntryNewScreen(POPUP_SCREENID, ExitDisplayPopup, NULL, NULL);

#ifdef __MMI_SCREEN_ROTATE__
	mmi_frm_screen_rotate(rotation);
#endif

   /* to sync popup screen display, imageOnBottom flag is no more used */
   // if(imageOnBottom) ShowCategory63Screen(string,imageId,NULL);
   // else
   ShowCategory65Screen(string,imageId,NULL);
    
   SetGroupKeyHandler(PopupTimerFlagEnable,(PU16)PresentAllKeys,TOTAL_KEYS,KEY_EVENT_DOWN);
   //CSD end

   /***** Moved By Deepali*********/
    SetGroupKeyHandler(PopupCloseByPressAnyKey,(PU16)PresentAllKeys,TOTAL_KEYS,KEY_EVENT_UP);
   /*******************************/

   //SetExitHandler(POPUP_SCREENID, ExitDisplayPopup);
   StartTimer(POPUP_TIMER,popupDuration,PopupTimerOver);
   popupToneId=toneId;
   if (popupToneId) playRequestedTone(popupToneId);  //dyj del 20060514 //add by panxu 20061228
}

#ifdef __MMI_SCREEN_ROTATE__   // zrx del 20060612

void DisplayPopup(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId)
{
	DisplayPopupRotated(string, imageId, imageOnBottom, popupDuration, toneId, MMI_FRM_SCREEN_ROTATE_0);
}

#endif /* __MMI_SCREEN_ROTATE__ */   // zrx del 20060612

/*****************************************************************
   FUNCTION NAME     : ExitDisplayPopup()

   PURPOSE           : Exit function for DisplayPopup()

   INPUT PARAMETERS  : nil

   OUTPUT PARAMETERS : nil

   RETURNS           : void

 

*******************************************************************/
void ExitDisplayPopup(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   StopTimer(POPUP_TIMER);
    
   if(popupToneId) 
   {
   	stopRequestedTone(popupToneId);  //dyj del 20060514 //add by panxu 20061228
   	popupToneId = 0;
   }
    
   
}

void DisplayPopupCallBack(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId,FuncPtr f)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   //ExecuteCurrExitHandler();
   mmi_trace(TRUE,"Func: %s callback function=%p", __FUNCTION__,  f);
   EntryNewScreen(POPUP_SCREENID, ExitDisplayPopupCallBack, NULL, NULL);
   /* to sync popup screen display, imageOnBottom flag is no more used */
   // if(imageOnBottom) ShowCategory63Screen(string,imageId,NULL);
   // else
   ShowCategory65Screen(string,imageId,NULL);
    
   SetGroupKeyHandler(PopupTimerFlagEnable,(PU16)PresentAllKeys,TOTAL_KEYS,KEY_EVENT_DOWN);
   //CSD end

   /***** Moved By Deepali*********/
 
//    SetGroupKeyHandler(PopupCloseByPressAnyKey,(PU16)PresentAllKeys,TOTAL_KEYS,KEY_EVENT_UP);
   /*******************************/

 //  SetExitHandler(POPUP_SCREENID, ExitDisplayPopup);
   StartTimer(POPUP_TIMER,popupDuration,PopupTimerOverCallBack);
   PopupFunc = f;
   popupToneId=toneId;
   if(popupToneId) playRequestedTone(popupToneId); //dyj del 20060514 //add by panxu 20061228
}

/*****************************************************************
   FUNCTION NAME     : ExitDisplayPopupCallBack()

   PURPOSE           : Exit function for DisplayPopup()

   INPUT PARAMETERS  : nil

   OUTPUT PARAMETERS : nil

   RETURNS           : void

 

*******************************************************************/
void ExitDisplayPopupCallBack(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   StopTimer(POPUP_TIMER);
    
   if(popupToneId) 
   {
   	stopRequestedTone(popupToneId);  //dyj del 20060514 //add by panxu 20061228
   	popupToneId = 0;
   }
    
   PopupFunc=NULL;
}

/*****************************************************************
   FUNCTION NAME     : PopupTimerFlagEnable()

   PURPOSE           : turn on popup timer flag

   INPUT PARAMETERS  : nil

   OUTPUT PARAMETERS : nil

   RETURNS           : void

 

*******************************************************************/
BOOL popUpTimerFlag = FALSE;
void PopupTimerFlagEnable(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   popUpTimerFlag = TRUE;
}
/*****************************************************************
   FUNCTION NAME     : PopupTimerOver()

   PURPOSE           : Stops the timer and goes back after pop-up timer over

   INPUT PARAMETERS  : nil

   OUTPUT PARAMETERS : nil

   RETURNS           : void

 

*******************************************************************/
void PopupCloseByPressAnyKey(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if(popUpTimerFlag == TRUE)
   {
      popUpTimerFlag = FALSE;
    
   if(popupToneId) 
   {
   	stopRequestedTone(popupToneId);  //dyj del 20060514 //add by panxu 20061228
   	popupToneId = 0;
   }
    

#ifdef __MMI_SUBLCD__
      if( IsMyTimerExist( POPUP_SUBLCD ) )
      { 
         StopTimer(POPUP_SUBLCD);
	     GoBackSubLCDHistory();
      }
#endif

      StopTimer(POPUP_TIMER);
      GoBackHistory();
 
      if ( PopupFunc != NULL )
	   PopupFunc();
	  PopupFunc = NULL;
   }
}


/*****************************************************************
   FUNCTION NAME     : PopupTimerOver()

   PURPOSE           : Stops the timer and goes back after pop-up timer over

   INPUT PARAMETERS  : nil

   OUTPUT PARAMETERS : nil

   RETURNS           : void

 

*******************************************************************/
void PopupTimerOver(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   StopTimer(POPUP_TIMER);
    
   if(popupToneId) 
   {
   	stopRequestedTone(popupToneId);  //dyj del 20060514 //add by panxu 20061228
   	popupToneId = 0;
   }
    
   GoBackHistory();
}

/*****************************************************************
   FUNCTION NAME     : PopupTimerOverCallBack()

   PURPOSE           : Stops the timer and Call Callback function after pop-up timer over

   INPUT PARAMETERS  : nil

   OUTPUT PARAMETERS : nil

   RETURNS           : void

 

*******************************************************************/
void PopupTimerOverCallBack(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   StopTimer(POPUP_TIMER);
    
   mmi_trace(TRUE,"Func: %s PopupFunc=%p", __FUNCTION__, PopupFunc);
   if(popupToneId) 
   {
   	stopRequestedTone(popupToneId);  //dyj del 20060514 //add by panxu 20061228
   	popupToneId = 0;
   }
    
 
   if ( PopupFunc != NULL )
	   PopupFunc();
	PopupFunc=NULL;
}


//#ifndef MMI_ON_HARDWARE_P
/*****************************************************************
   FUNCTION NAME     : PopupNoSupport()

   PURPOSE           : Display the pop-up screen for PC Simulator limitation scenario

   INPUT PARAMETERS  : nil

   OUTPUT PARAMETERS : nil

   RETURNS           : nil

 

*******************************************************************/
void PopupNoSupport(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   DisplayPopup((PU8)GetString(STR_GLOBAL_UNSUPPORTED_FORMAT),
      IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE); 	
   return;
}
//#endif /* #ifndef MMI_ON_HARDWARE_P */


/*****************************************************************
   FUNCTION NAME     : DisplayConfirmCountDown()

   PURPOSE           : Display Popup Confirm Screen

   INPUT PARAMETERS  : nil

   OUTPUT PARAMETERS : nil

   RETURNS           : void

 

*******************************************************************/
void DisplayConfirmCountDown(U16 LSK_str, U16  LSK_img, U16  RSK_str, U16  RSK_img, UI_string_type message, UI_image_ID_type message_image, U8 toneId, U32 duration, FuncPtr callback)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   U8* guiBuffer;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   guiBuffer = GetCurrGuiBuffer(SCR_CONFIRM_SCREEN);
   EntryNewScreen(SCR_CONFIRM_SCREEN,ExitDisplayPopupConfirm,NULL,NULL);
   ClearAllKeyHandler();
   ShowCategory165Screen_ext(LSK_str, LSK_img, RSK_str, RSK_img,message,(U16)message_image,duration,guiBuffer);
   SetCat165TimerExpireFunc(callback);
   popupToneId=toneId;
  if(popupToneId)  //dyj del 20060514 //add by panxu 20061228
   	playRequestedTone(popupToneId); //add by panxu 20061228
}

void mmi_show_in_progress(U16 msg, U16 msgIcon)
{
	EntryNewScreen(SHOW_IN_PROGRESS_SCREEN, NULL, NULL,NULL);

	ShowCategory165Screen (0, 0, \
		0, 0, \
		(UI_string_type)GetString (msg), \
		msgIcon, NULL) ;

	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
}
/*****************************************************************
   FUNCTION NAME     : DisplayConfirm()

   PURPOSE           : Display Popup Confirm Screen

   INPUT PARAMETERS  : nil

   OUTPUT PARAMETERS : nil

   RETURNS           : void

 

*******************************************************************/
#ifdef __MMI_SCREEN_ROTATE__
void DisplayConfirmRotated(U16 LSK_str, U16  LSK_img, U16  RSK_str, U16  RSK_img, UI_string_type message, UI_image_ID_type message_image, U8 toneId, mmi_frm_screen_rotate_enum rotation)
#else
void DisplayConfirm(U16 LSK_str, U16  LSK_img, U16  RSK_str, U16  RSK_img, UI_string_type message, UI_image_ID_type message_image, U8 toneId)
#endif
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   U8* guiBuffer;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   guiBuffer = GetCurrGuiBuffer(SCR_CONFIRM_SCREEN);
   EntryNewScreen(SCR_CONFIRM_SCREEN,ExitDisplayPopupConfirm,NULL,NULL);
#ifdef __MMI_SCREEN_ROTATE__
	mmi_frm_screen_rotate(rotation);
#endif
   
   ClearAllKeyHandler();
   ShowCategory165Screen(LSK_str, LSK_img, RSK_str, RSK_img,message,(U16)message_image,guiBuffer);
   popupToneId=toneId;
   if(popupToneId)   //dyj del 20060514 //add by panxu 20061228
	playRequestedTone(popupToneId); //add by panxu 20061228
}

#ifdef __MMI_SCREEN_ROTATE__

void DisplayConfirm(U16 LSK_str, U16  LSK_img, U16  RSK_str, U16  RSK_img, UI_string_type message, UI_image_ID_type message_image, U8 toneId)
{
	DisplayConfirmRotated(LSK_str, LSK_img, RSK_str, RSK_img, message, message_image, toneId, MMI_FRM_SCREEN_ROTATE_0);
}

#endif /* __MMI_SCREEN_ROTATE__ */

void DisplayNewconfirm(U16 LSK_str, U16  LSK_img, U16  RSK_str, U16  RSK_img, UI_string_type message, UI_image_ID_type message_image, U8 toneId,FuncPtr func)
{
	   /*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8* guiBuffer;
	
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	guiBuffer = GetCurrGuiBuffer(SCR_CONFIRM_SCREEN);
	EntryNewScreen(SCR_CONFIRM_SCREEN,ExitDisplayPopupConfirm,func,NULL);
#ifdef __MMI_SCREEN_ROTATE__
	mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_0);
#endif
	ClearAllKeyHandler();
	ShowCategory119Screen(LSK_str, LSK_img, RSK_str, RSK_img,(U8*)message,(U16)message_image,guiBuffer);
	popupToneId=toneId;
	if(popupToneId)  
		playRequestedTone(popupToneId); 
	
}

#if (0)
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
void ExitDisplayPopupConfirm(void)
{
#if defined( __MMI_UI_STYLE_3__) || defined(__MMI_UI_STYLE_4__) 
	leave_full_screen();
#endif
    
   if(popupToneId) 
   {
   		stopRequestedTone(popupToneId);  //dyj del 20060514 //add by panxu 20061228
	   	popupToneId = 0;
   }
    
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_go_back_2_history
* DESCRIPTION
*   Go back 2 history
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_frm_go_back_2_history (void)
{
	GoBacknHistory (1);
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_inline_generic_done
* DESCRIPTION
*   Highlight inline done handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_frm_highlight_inline_generic_done (void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(ConfirmInlineFullScreenEdit,KEY_EVENT_UP);
	SetKeyHandler(ConfirmInlineFullScreenEdit, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_highlight_input_method_generic
* DESCRIPTION
*   Highlight input method generic handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_frm_highlight_input_method_generic (void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
	SetLeftSoftkeyFunction(EntryInputMethodScreen,KEY_EVENT_UP);
	SetKeyHandler(EntryInputMethodScreen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	RegisterInputMethodScreenCloseFunction (mmi_frm_go_back_2_history);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_set_highlight_handler
* DESCRIPTION
*   Register highlight generic handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_frm_set_highlight_handler (void)
{
	SetHiliteHandler (MENU_DONE_OPTION_ID, mmi_frm_highlight_inline_generic_done);
	SetHiliteHandler (MENU_INPUT_METHOD_OPTION_ID, mmi_frm_highlight_input_method_generic);
}

static void PopupSubLcdTimerOver(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   StopTimer(POPUP_SUBLCD);
   GoBackSubLCDHistory();

}

static void ExitSubLCDPopupScr(void)
{
	/* not to add into history */
   	StopTimer(POPUP_SUBLCD);
}

void DisplaySubLcdPopup(U16 imageId)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	ExecSubLCDCurrExitHandler();
	ShowCategory304Screen(imageId/*IMG_SUBLCD_CHARGER_POWER_ON*/, 0);
	SetSubLCDExitHandler(ExitSubLCDPopupScr);

   	StartTimer(POPUP_SUBLCD, UI_POPUP_NOTIFYDURATION_TIME, PopupSubLcdTimerOver);

}

/*****************************************************************************
* FUNCTION
*  SetInterruptPopupDisplay
* DESCRIPTION
*   Set Interrupt popup screen is allowed to display or not.
*
* PARAMETERS
*  flag: can be POPUP_ALL_OFF, POPUP_DSPL_ON, POPUP_TONE_ON
* RETURNS
*  none
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void SetInterruptPopupDisplay(U8 flag)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                             								   */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                    								   */
   /*----------------------------------------------------------------*/

   /* The value can be POPUP_ALL_OFF, POPUP_DSPL_ON, POPUP_TONE_ON or POPUP_ALL_ON */
   is_popup_allow = flag;
}

/*****************************************************************************
* FUNCTION
*  GetInterruptPopupDisplay
* DESCRIPTION
*   Get if the interrupt popup screen is allowed to display or not.
*
* PARAMETERS
*  none
* RETURNS
*  U8 : can be POPUP_ALL_OFF, POPUP_DSPL_ON, POPUP_TONE_ON
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
U8 GetInterruptPopupDisplay(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                             								   */
   /*----------------------------------------------------------------*/

   /*----------------------------------------------------------------*/
   /* Code Body                                                   								   */
   /*----------------------------------------------------------------*/

   /* The value can be POPUP_ALL_OFF, POPUP_DSPL_ON, POPUP_TONE_ON, or POPUP_ALL_ON */
   return is_popup_allow;
}
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined ( __MMI_MAINLCD_220X176__) ||defined ( __MMI_MAINLCD_320X240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
static BOOL qpad_is_pop_color_change = FALSE ;
static color_t qpad_saved_color;
extern color_t white_color;
BOOL IsPopupSavedCharColor()
{
	return qpad_is_pop_color_change;
}
void PopupSaveCharColor()
{
	qpad_is_pop_color_change = TRUE;
	qpad_saved_color	=  MMI_multiline_inputbox.normal_text_color;
}
void PopupChangeCharColor()
{
	MMI_multiline_inputbox.normal_text_color = white_color;
}
void PopupResetCharColor()
{
	qpad_is_pop_color_change = FALSE;
	MMI_multiline_inputbox.normal_text_color = qpad_saved_color;
}
#endif
