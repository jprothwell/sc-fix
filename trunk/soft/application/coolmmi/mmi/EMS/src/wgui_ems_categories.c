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
 * wgui_ems_categories.c
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
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 
/**********************************************************************************
	Filename:		wgui_ems_categories.c
	Date Created:	March-22-2003
	Contains:		UI wrapper routines for EMS and category screens
**********************************************************************************/

#define EMS_USE_INTERNAL_MENU					0
#define ENABLE_EMS_TEST_DEMO					0
#define EMS_USE_STUB_MENU						0
#define ENABLE_EMS_TEST_OBJECTS					0
#define ENABLE_EMS_TEST_WORD_HIGHLIGHT			0
#define EMS_INPUTBOX_DEBUG						0
#define EMS_USE_GSM_EXTENDED					0

// start - added by magesh
#define ENABLE_EMS_TEST_SAVE_OBJECTS			1	// 0- enable, 1-disable
#define ENABLE_EMS_TEST_SAVE_PIC_OBJECTS		0	// 1- enable, 0-disable
#define ENABLE_EMS_TEST_SAVE_ANIA_OBJECTS		1	// 1- enable, 0-disable
#define ENABLE_EMS_TEST_SAVE_MELO_OBJECTS		0	// 1- enable, 0-disable
// end - added by magesh

/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-17 15:0 */
#if defined(__MMI_GB__)
#define GBLasttotalRemainingOctet                                  472    //add by yaosq for  remainingOctect 20061127 ,当我们有中文输入时，最大可输入70个中文
											   	    //由于我们不支持长短信，故初始化程序提示的612不能减少到0
#define NotGBLasttotalRemainingOctet                             452    //原因同上，当没有中文输入时最大可输入160个字符
#define LargestByteNumberInSms                                   612 //wangzl:modify from 612  //guojian restore for bug 10360   //当支持长短信时，最大可输入612个英文字符，当我们还没有进入
#endif
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-17 15:0 */

#include "mmi_data_types.h"
#include "mmi_features.h"
#define __NEWSIMULATOR
#include "timerevents.h"
#include "filesystemdef.h"
#include "filesystemgprot.h"
#include "conversions.h"
#undef __NEWSIMULATOR
#include "custdataprots.h"
#include "historygprot.h"
#include "custmenures.h"
#include "debuginitdef.h"
#include "globaldefs.h"
#include "settingprofile.h"
#include "wgui.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_defs.h"
#include "wgui_softkeys.h"
#include "gdi_include.h"
#include "wgui_ems_categories.h"
#include "messagesresourcedata.h"
#include "commonscreens.h"
#include "settingdefs.h"
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
#include "filemgr.h"
#include "filemanagerdef.h"
#include "filemanagergprot.h"
#endif
#ifdef __MMI_DRAW_MANAGER__
#include "wgui_draw_manager.h"
#endif
 
#include "t9def.h"

#ifdef __MMI_HINDI_ALG__
#include "hf_rules.h"
#endif
#include "wgui_virtual_keyboard.h"
#if defined(__MMI_TOUCH_SCREEN__)
#include "editorpen.h"
#endif

#include "mmi_trace.h"
/*zhoumn Add Start For 6052 Ver: TBM_780  on 2007-8-7 15:1 */
#include "gbhmi.h" 
/*zhoumn Add End  For 6052 Ver: TBM_780  on 2007-8-7 15:1 */

 
extern BOOL r2lMMIFlag;
//CSD end
#include "gblt.h"



 
/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-17 15:2 */
#ifdef __MMI_GB__
#include"gbhmi.h"
//Fix Bug#12338 for cs2324c by cong.li on 2009.05.20.
#if 0
#ifdef __MMI_TOUCH_SCREEN__
const char Pinyin[] = {0xFC, 0x62, 0xF3, 0x97, 0x00, 0x00}; //modified by zhoumn@2007/07/17 pinyin->拼音
const char Bihua[] = {0x14, 0x7B, 0x12, 0x52, 0x00, 0x00} ; //modified by zhoumn@2007/07/17 bihua->笔划
#else
const char Pinyin[] = {0x50, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x79, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x00};
const char Bihua[] = {0x42, 0x00, 0x69, 0x00, 0x68, 0x00, 0x75, 0x00, 0x61, 0x00, 0x00} ;
#endif
#endif
const char BPMF[] = {0x42, 0x00, 0x50, 0x00, 0x4D, 0x00, 0x46, 0x00, 0x00};
#if !defined(__PROJECT_GALLITE_C01__)
const char Pinyin[] = {0x50, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x79, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x00};
const char Bihua[] = {0x42, 0x00, 0x69, 0x00, 0x68, 0x00, 0x75, 0x00, 0x61, 0x00, 0x00} ;
#else
const char Pinyin[] = {0xFC, 0x62, 0xF3, 0x97, 0x00, 0x00}; //modified by zhoumn@2007/07/17 pinyin->拼音
const char Bihua[] = {0x14, 0x7B, 0x12, 0x52, 0x00, 0x00} ; //modified by zhoumn@2007/07/17 bihua->笔划
#endif
const char SmartABC[] = {0x53, 0x00, 0x41, 0x00, 0x42, 0x00, 0x43, 0x00, 0x00} ;
const char Smartabc[] = {0x53, 0x00, 0x61, 0x00, 0x62, 0x00, 0x63, 0x00, 0x00} ;
extern mmi_editor_inoutbox_enum MMI_current_input_box_type;
extern UI_character_type EMS_input_box_new_line_character;
extern pBOOL bidi_get_char_type(U8* input_char,U8* charType);		//guoyt added for resolving warning info when compiling
extern void GbKey0PressHandler(void);
extern void GbKey1PressHandler(void);
extern void GbKey2PressHandler(void);
extern void GbKey3PressHandler(void);
extern void GbKey4PressHandler(void);
extern void GbKey5PressHandler(void);
extern void GbKey6PressHandler(void);
extern void GbKey7PressHandler(void);
extern void GbKey8PressHandler(void);
extern void GbKey9PressHandler(void);
extern void GbKeyArrowUPHandler(void);
extern void GbKeyArrowRightHandler(void);
extern void GbKeyArrowDownHandler(void);
extern void GbKeyArrowLeftHandler(void);
extern void  GbClearKeyNavigationHandler(void);
extern void  GbClearKey0To9Handler(void);
extern void  GbClearKeyStarAndPoundHandler(void);
extern void GbChangeInputMethod(U16 inputMode);
extern void GbInputSetRSK(void);
extern void GbDisplayForEmsEditor(void);
extern void InitGbInputMethod(void);
extern void GbInuptMethodEnterCategory28(void);
extern void GbInuptMethodExitCategory28(void);
extern void GbKeyStarPressHandler(void);
extern void mmi_msg_entry_option_write(void);
#ifdef __MMI_BLACKBERRY_QWERTY__
extern void wgui_inputbox_handle_symbol_picker_star_key_down(void);
#endif
extern BOOL EnterSymboFlag ;
PUBLIC UINT16 InsertEnter = 0;        //记录插入的回车数
extern GBIMESTATE gbstate;  //indication whether input methode work well
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-17 15:2 */
#elif defined __MMI_T9__
#include "t9definition.h"
#include "t9api.h"

extern T9U16 curInputMethod;
extern void (*t9_inputbox_LSK_function)(void);
extern void T9KeyArrowUPHandlerForEMSInputBox(void);
extern void T9KeyArrowRightHandlerForEMSInputBox(void);
extern void T9KeyArrowDownHandlerForEMSInputBox(void);
extern void T9KeyArrowLeftHandlerForEMSInputBox(void);
extern void T9KeyStarPressHandlerForEMSInputBox(void);
extern void T9InputBoxDisplayForEMSInputBox(void);
extern void T9ScreenStateChnageForEMSInputBox(void);
extern void T9SynT9WithInputBoxForEMSInputBox(void);
extern void T9Key0To9HandlerForEMSInputBox(T9MINOR mmiKey, T9MINOR keyType);
extern void T9ClearKeyNavigationHandler(void);
extern void T9ClearKeyStarAndPoundHandler(void);
extern void T9ChangeLanguageForEMSInputBox(T9U8 inputMethod);
extern void T9Key0PressHandlerForEMSInputBox(void);
extern void T9Key1PressHandlerForEMSInputBox(void);
extern void T9Key2PressHandlerForEMSInputBox(void);
extern void T9Key3PressHandlerForEMSInputBox(void);
extern void T9Key4PressHandlerForEMSInputBox(void);
extern void T9Key5PressHandlerForEMSInputBox(void);
extern void T9Key6PressHandlerForEMSInputBox(void);
extern void T9Key7PressHandlerForEMSInputBox(void);
extern void T9Key8PressHandlerForEMSInputBox(void);
extern void T9Key9PressHandlerForEMSInputBox(void);
extern void T9KeyPoundPressHandlerForEMSInputBox(void);
extern void InuptMethodEnterCategory28(void);
extern void InuptMethodExitCategory28(void);
extern void T9ClearKey0To9Handler(void);
#elif defined __MMI_ZI__

#include "zi8hmi.h"
extern void ZiKey0PressHandler(void);
extern void ZiKey1PressHandler(void);
extern void ZiKey2PressHandler(void);
extern void ZiKey3PressHandler(void);
extern void ZiKey4PressHandler(void);
extern void ZiKey5PressHandler(void);
extern void ZiKey6PressHandler(void);
extern void ZiKey7PressHandler(void);
extern void ZiKey8PressHandler(void);
extern void ZiKey9PressHandler(void);

extern void ZiKeyArrowUPHandler(void);
extern void ZiKeyArrowRightHandler(void);
extern void ZiKeyArrowDownHandler(void);
extern void ZiKeyArrowLeftHandler(void);

extern void ZiKeyStarPressHandler(void);
extern void ZiKeyStarLongPressHandler(void);
extern void ZiKeyHashPressHandler(void);
extern void ZiKeyHashLongPressHandler(void);

extern void ZiKeyLSKPressHandler(void);
extern void ZiKeyLSKLongPressHandler(void);
extern void ZiKeyRSKPressHandler(void);
extern void ZiKeyRSKLongPressHandler(void);
extern void ZiChangeInputMethod(INPUT_MODE inputMode);
extern void ZiClearKeyStarAndPoundHandler(void);
extern void ZiClearKeyNavigationHandler(void);
extern void ZiClearKey0To9Handler(void);

#if defined(__MMI_MESSAGES_EMS__)
extern void ZiInuptMethodEnterCategory28(void);
extern void ZiInuptMethodExitCategory28(void);
#endif

extern void ZiDisplay(void);
extern void ZiInputBoxScreenStateChange(void);
#elif defined __MMI_KA__

#include "konkahmi.h"
extern void KonkaKey0PressHandler(void);
extern void KonkaKey1PressHandler(void);
extern void KonkaKey2PressHandler(void);
extern void KonkaKey3PressHandler(void);
extern void KonkaKey4PressHandler(void);
extern void KonkaKey5PressHandler(void);
extern void KonkaKey6PressHandler(void);
extern void KonkaKey7PressHandler(void);
extern void KonkaKey8PressHandler(void);
extern void KonkaKey9PressHandler(void);

extern void KonkaKeyArrowUPHandler(void);
extern void KonkaKeyArrowRightHandler(void);
extern void KonkaKeyArrowDownHandler(void);
extern void KonkaKeyArrowLeftHandler(void);

extern void KonkaKeyStarPressHandler(void);
extern void KonkaKeyStarLongPressHandler(void);
extern void KonkaKeyHashPressHandler(void);
extern void KonkaKeyHashLongPressHandler(void);

extern void KonkaKeyLSKPressHandler(void);
extern void KonkaKeyLSKLongPressHandler(void);
extern void KonkaKeyRSKPressHandler(void);
extern void KonkaKeyRSKLongPressHandler(void);
extern void KonkaChangeInputMethod(INPUT_MODE inputMode);
extern void KonkaClearKeyStarAndPoundHandler();
extern void KonkaClearKeyNavigationHandler();
extern void KonkaClearKey0To9Handler();

extern void KonkaInuptMethodEnterCategory28(void);
extern void KonkaInuptMethodExitCategory28(void);
extern void KonkaDisplay(void);
extern void KonkaInputBoxScreenStateChange(void);
#elif defined __MMI_ITAP__
#include "itaphmidef.h"
#include "itaphmistruct.h"
#include "itaphmigprot.h"

#endif
#if defined(__MMI_SMART_FRENCH__)
extern void InitGbInputMethod_fr(void);
#endif
#if defined(__MMI_SMART_URDU__)
extern void InitGbInputMethod_Urdu(void);
#endif
#if defined(__MMI_SMART_PUNJABI__)
extern void InitGbInputMethod_Punjabi(void);
#endif
#if defined(__MMI_SMART_BENGALI__)
extern void InitGbInputMethod_Bengali(void);
#endif
#if defined(__MMI_SMART_TAMIL__)
extern void InitGbInputMethod_Tamil(void);
#endif
#if defined(__MMI_SMART_TURKISH__)
extern void InitGbInputMethod_turkish(void);
#endif
#if defined(__MMI_SMART_INDONESIAN__)
extern void InitGbInputMethod_indonesian(void);
#endif
#if defined(__MMI_SMART_MALAY__)
extern void InitGbInputMethod_malay(void);
#endif
// beginning--added by kecx for adding italian on 20081223
#if defined(__MMI_MULTITAP_ITALIAN__)||defined(__MMI_SMART_ITALIAN__)
extern void InitGbInputMethod_ita(void);
#endif
// end--added by kecx for adding italian on 20081223
#if defined(__MMI_SMART_RUSSIAN__)
extern void InitGbInputMethod_Ru(void);
#endif
#if defined(__MMI_SMART_THAI__)
extern void InitGbInputMethod_Thai(void);
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
extern void InitGbInputMethod_Vi(void);
#endif
#if defined(__MMI_GB_SM_CHINESE__)
void InitGbSMInputMethod(void);
#endif
#if defined(__MMI_GB_TR_CHINESE__)
void InitGbTRInputMethod(void);
#endif

//beginning:	guoyt added for adding hindi,german,spanish ime.
#if defined(__MMI_SMART_HINDI__)
extern void InitGbInputMethod_Hin(void);
#endif
#if defined(__MMI_SMART_GERMAN__)
extern void InitGbInputMethod_Ger(void);
#endif
#if defined(__MMI_SMART_SPANISH__)
extern void InitGbInputMethod_Spa(void);
#endif
//end: 	guoyt added for adding hindi,german,spanish ime.
#if defined(__MMI_SMART_TELUGU__)
extern void InitGbInputMethod_Tel(void);
#endif
#if defined(__MMI_GB_SMART_PORTUGUESE__)
extern void InitGbInputMethod_PT(void);
#endif
 
#ifdef __MMI_MULTITAP_THAI__
void handle_category28_change_input_mode_bymultitapthai(void);
#endif

byte EMS_inputbox_check_multitap_thai_input(UI_character_type inputchar,byte *updatef);

extern void handle_category28_change_input_mode(void);

extern void wgui_EMS_redraw_input_information_bar(void);
extern void coolsand_UI_EMS_change_DCS(UI_EMS_input_box *b);
#ifdef __MMI_MESSAGES_EMS__
extern void EMS_cancel_object_focus(void);
#endif
//CSD end

 
extern U16 StopMyTimer(U16 nTimerId);
extern U16 StartMyTimerInt(U16 nTimerId,U32 nTimeDuration,oslTimerFuncPtr TimerExpiry, U8 alignment);
//CSD end

 
extern void set_current_input_box_type(mmi_editor_inoutbox_enum input_box_type);
 
extern s16 MMI_prefered_input_type;
//CSD end

extern void dm_set_scr_bg_image_no_draw(U16 image_id, S8* file_name, S32 x, S32 y, U8 opacity); 

extern U8 PhnsetGetDefEncodingType(void);
extern void mmi_msg_entry_option_write (void);


/*	Functions for 7-bit GSM character set encoding	*/

#define EMS_ESCAPE_CHARACTER	(27)
#define EMS_SPACE_CHARACTER		(32)
#define EMS_CR_CHARACTER		(13)
#define EMS_EURO_CHARACTER_UCS2		(0x20AC)
#define EMS_EURO_CHARACTER			(0xA2)

extern const U8 AsciiToDefaultArray[];
extern const U8 ExtendedAsciiToDefaultArray[];
extern const U8 DefaultToExtendedAsciiArray[];
extern const U8 DefaultToAsciiArray[];
/*JINZH Add Start For 英语-法语-阿拉伯切换 Ver: 多国语言  on 2007-12-29 17:38 */
extern s16 GBIMEmethod;
/*JINZH Add End  For 英语-法语-阿拉伯切换 Ver: 多国语言  on 2007-12-29 17:38 */
 
#if defined(__MMI_WALLPAPER_ON_BOTTOM__)
static UI_image_ID_type editor_scr_bg_ID;
static S8* editor_scr_bg_filename;
static S32 editor_scr_bg_x, editor_scr_bg_y;
static U8 editor_scr_bg_opacity; 
#endif
 

 
void handle_category28_input(void);

#if(!UI_DISABLE_EMS_CATEGORY_SCREENS)

#if(!EMS_USE_GSM_EXTENDED)

/*	Call after EMSUnpack	*/
void EMS_ConvertGSM7BitDefaultEncodingToAscii(EMSData *data)
{	U16 c,OffsetToText;
	U8*	textBuffer=data->textBuffer;
	U16 textBufferLength=data->textLength;
	if(textBuffer==NULL) return;
	ResetCurrentPosition(data);
	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
	{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
		c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
		c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
		if((c&0xff00)!=0)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
		}
		else if(c==EMS_ESCAPE_CHARACTER)
		{	c=EMS_SPACE_CHARACTER;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
		}
		else
		{	if(c==EMS_CR_CHARACTER)
			{	ForwardCurrentPosition(data,1);
				CancelCurrentPosition(data,1);
				BackwardCurrentPosition(data,1);
			}
			else
			{
				c=DefaultToAsciiArray[c];
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
				textBuffer[OffsetToText+1]=(U8)(c&0xff);
				textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
				textBuffer[OffsetToText]=(U8)(c&0xff);
				textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
			}
		}
		if(ForwardCurrentPosition(data,1)!=1) break;
	}
	ResetCurrentPosition(data);
}

/*	Call before EMSPack	*/
void EMS_ConvertAsciiEncodingToGSM7BitDefault(EMSData *data)
{	U16 c,OffsetToText;
	U8* textBuffer=data->textBuffer;
	U16 textBufferLength=data->textLength;
	if(textBuffer==NULL) return;
	ResetCurrentPosition(data);
	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
	{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
		c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
		c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
		if((c&0xff00)==0)
		{
			c=AsciiToDefaultArray[c];
		}
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
		textBuffer[OffsetToText+1]=(U8)(c&0xff);
		textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
		textBuffer[OffsetToText]=(U8)(c&0xff);
		textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
		if(ForwardCurrentPosition(data,1)!=1) break;
	}
	ResetCurrentPosition(data);
}

#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
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
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
/* under construction !*/
#else
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
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
#else
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
/* under construction !*/
/* under construction !*/
#else
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
/* under construction !*/
#else
/* under construction !*/
#endif
/* under construction !*/
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
/* under construction !*/
/* under construction !*/
#else
/* under construction !*/
/* under construction !*/
#endif
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
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
/* under construction !*/
#else
/* under construction !*/
#endif
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
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
/* under construction !*/
#else
/* under construction !*/
#endif
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
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
/* under construction !*/
/* under construction !*/
#else
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
/* under construction !*/
#else
/* under construction !*/
#endif
/* under construction !*/
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
/* under construction !*/
/* under construction !*/
#else
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else

/*	Call after EMSUnpack	*/
void EMS_ConvertGSM7BitDefaultEncodingToAsciiWithExtended(EMSData *data)
{	U16 c,OffsetToText;
	U8*	textBuffer=data->textBuffer;
	U16 textBufferLength=data->textLength;
	if(textBuffer==NULL) return;
	ResetCurrentPosition(data);
	if(data->dcs==SMSAL_DEFAULT_DCS)
	{
	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
	{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
		c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
		c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
		if((c&0xff00)!=0)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
		}
		else if(c==EMS_ESCAPE_CHARACTER)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);

//			if(ForwardCurrentPosition(data,1)!=1) break;
			data->CurrentPosition.OffsetToText += 2;
			if((OffsetToText=data->CurrentPosition.OffsetToText)>=textBufferLength) break;

			OffsetToText=data->CurrentPosition.OffsetToText;

			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
			c=DefaultToExtendedAsciiArray[c];
			if(c==0x00)
			{
			CancelCurrentPosition(data, 1);
			data->CurrentPosition.OffsetToText -= 2;
			}
			else
			{
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
			}
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);

//			if(ForwardCurrentPosition(data,1)!=1) break;
			data->CurrentPosition.OffsetToText += 2;

			OffsetToText=data->CurrentPosition.OffsetToText;

			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
			c=DefaultToExtendedAsciiArray[c];
			if(c==0x00)
			{
			CancelCurrentPosition(data, 1);
			data->CurrentPosition.OffsetToText -= 2;
			}
			else
			{
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
			}
#endif
		}
		else
		{	c=DefaultToAsciiArray[c];
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
		}
//		if(ForwardCurrentPosition(data,1)!=1) break;
		data->CurrentPosition.OffsetToText += 2;

	}
	}
	else
	{	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
			// if(c==EMS_EURO_CHARACTER_UCS2) c=EMS_EURO_CHARACTER;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
//			if(ForwardCurrentPosition(data,1)!=1) break;
			data->CurrentPosition.OffsetToText += 2;

		}
	}
	ResetCurrentPosition(data);
}

/*	Call before EMSPack	*/
void EMS_ConvertAsciiEncodingToGSM7BitDefaultWithExtended(EMSData *data)
{	U16 c,OffsetToText;
	U8* textBuffer=data->textBuffer;
	U16 textBufferLength=data->textLength;
	if(textBuffer==NULL) return;
	ResetCurrentPosition(data);
	if(data->dcs==SMSAL_DEFAULT_DCS)
	{
	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
	{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
		c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
		c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
		if((c&0xff00)==0)
		{
			if(c==EMS_ESCAPE_CHARACTER)
			{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
				textBuffer[OffsetToText+1]=(U8)(c&0xff);
				textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
				textBuffer[OffsetToText]=(U8)(c&0xff);
				textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
				//if(ForwardCurrentPosition(data,1)!=1) break;
				data->CurrentPosition.OffsetToText += 2;
				if((OffsetToText=data->CurrentPosition.OffsetToText)>=textBufferLength) break;
				OffsetToText=data->CurrentPosition.OffsetToText;

#if(EMS_BYTE_ORDER_BIG_ENDIAN)
				c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
				c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
				c=ExtendedAsciiToDefaultArray[c];
			}
			else
			{
				c=AsciiToDefaultArray[c];
			}
		}
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
		textBuffer[OffsetToText+1]=(U8)(c&0xff);
		textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
		textBuffer[OffsetToText]=(U8)(c&0xff);
		textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
//		if(ForwardCurrentPosition(data,1)!=1) break;
		data->CurrentPosition.OffsetToText += 2;
	}
	}
	else
	{	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
			// if(c==EMS_EURO_CHARACTER) c=EMS_EURO_CHARACTER_UCS2;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
//			if(ForwardCurrentPosition(data,1)!=1) break;
			data->CurrentPosition.OffsetToText += 2;
		}
	}
	ResetCurrentPosition(data);
}

#endif	/* defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__) */
/* End : JP */

void EMS_ConvertGSM7BitDefaultEncodingToAsciiWithoutEMSlib(EMSData *data)
{	U16 c,OffsetToText;
	U8*	textBuffer=data->textBuffer;
	U16 textBufferLength=data->textLength;
	if(textBuffer==NULL) return;
       data->CurrentPosition.OffsetToText = 0;
	if(data->dcs==SMSAL_DEFAULT_DCS)
	{
	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
	{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
		c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
		c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
		if((c&0xff00)!=0)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
		}
		else if(c==EMS_ESCAPE_CHARACTER)
		{

			U16 tempOffsetToText = data->CurrentPosition.OffsetToText;

			while((tempOffsetToText+2) < textBufferLength)
			{
				textBuffer[tempOffsetToText] = textBuffer[tempOffsetToText+2];
				textBuffer[tempOffsetToText+1] = textBuffer[tempOffsetToText+3];
				tempOffsetToText = tempOffsetToText + 2;
			}
			textBuffer[tempOffsetToText] = 0;
			textBuffer[tempOffsetToText+1] = 0;
			textBufferLength = textBufferLength -2;
			data->textLength = data->textLength -2;


#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			//textBuffer[OffsetToText+1]=(U8)(c&0xff);
			//textBuffer[OffsetToText+0]=(U8)(c>>8);

			if((data->CurrentPosition.OffsetToText+2)>textBufferLength) break;
			OffsetToText=data->CurrentPosition.OffsetToText;

			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
			c=DefaultToExtendedAsciiArray[c];

			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			//textBuffer[OffsetToText]=(U8)(c&0xff);
			//textBuffer[OffsetToText+1]=(U8)(c>>8);

			if((data->CurrentPosition.OffsetToText+2)>textBufferLength) break;
			OffsetToText=data->CurrentPosition.OffsetToText;

			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
			c=DefaultToExtendedAsciiArray[c];

			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
		}
		else
		{	c=DefaultToAsciiArray[c];
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
		}
		data->CurrentPosition.OffsetToText += 2;
	}
	}
	else
	{	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
			// if(c==EMS_EURO_CHARACTER_UCS2) c=EMS_EURO_CHARACTER;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
			data->CurrentPosition.OffsetToText += 2;
		}
	}
       data->CurrentPosition.OffsetToText = 0;
}


#else

/*	Call after EMSUnpack	*/
void EMS_ConvertGSM7BitDefaultEncodingToAscii(EMSData *data)
{	U16 c,OffsetToText;
	U8*	textBuffer=data->textBuffer;
	U16 textBufferLength=data->textLength;
	if(textBuffer==NULL) return;
	ResetCurrentPosition(data);
	if(data->dcs==SMSAL_DEFAULT_DCS)
	{
		while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
			if(c==EMS_ESCAPE_CHARACTER)
			{	c=EMS_SPACE_CHARACTER;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
				textBuffer[OffsetToText+1]=(U8)(c&0xff);
				textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
				textBuffer[OffsetToText]=(U8)(c&0xff);
				textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
			}
			else
			{	c=DefaultToAsciiArray[c];
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
				textBuffer[OffsetToText+1]=(U8)(c&0xff);
				textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
				textBuffer[OffsetToText]=(U8)(c&0xff);
				textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
			}
			if(ForwardCurrentPosition(data,1)!=1) break;
		}
	}
	else
	{	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
			// if(c==EMS_EURO_CHARACTER_UCS2) c=EMS_EURO_CHARACTER;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
			if(ForwardCurrentPosition(data,1)!=1) break;
		}
	}
	ResetCurrentPosition(data);
}

/*	Call before EMSPack	*/
void EMS_ConvertAsciiEncodingToGSM7BitDefault(EMSData *data)
{	U16 c,OffsetToText;
	U8* textBuffer=data->textBuffer;
	U16 textBufferLength=data->textLength;
	if(textBuffer==NULL) return;
	ResetCurrentPosition(data);
	if(data->dcs==SMSAL_DEFAULT_DCS)
	{	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
			c=AsciiToDefaultArray[c];
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
			if(ForwardCurrentPosition(data,1)!=1) break;
		}
	}
	else
	{	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
			// if(c==EMS_EURO_CHARACTER) c=EMS_EURO_CHARACTER_UCS2;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
			if(ForwardCurrentPosition(data,1)!=1) break;
		}
	}
	ResetCurrentPosition(data);
}

/*	Call after EMSUnpack	*/
void EMS_ConvertGSM7BitDefaultEncodingToAsciiWithExtended(EMSData *data)
{	U16 c,OffsetToText;
	U8*	textBuffer=data->textBuffer;
	U16 textBufferLength=data->textLength;
	EMSObject* object;
	if(textBuffer==NULL) return;
	ResetCurrentPosition(data);
	if(data->dcs==SMSAL_DEFAULT_DCS)
	{	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
		{
			if(coolsand_UI_EMS_input_box_get_next_object(data,&data->CurrentPosition,&c,&object)!=1)
			{
				continue;
			}
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
			if(c==EMS_ESCAPE_CHARACTER)
			{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
				textBuffer[OffsetToText+1]=(U8)(c&0xff);
				textBuffer[OffsetToText+0]=(U8)(c>>8);

				if(ForwardCurrentPosition(data,1)!=1) break;
				CancelCurrentPosition(data,1);
				OffsetToText=data->CurrentPosition.OffsetToText;

				c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
				c=DefaultToExtendedAsciiArray[c];

				textBuffer[OffsetToText+1]=(U8)(c&0xff);
				textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
				textBuffer[OffsetToText]=(U8)(c&0xff);
				textBuffer[OffsetToText+1]=(U8)(c>>8);

				if(ForwardCurrentPosition(data,1)!=1) break;
				CancelCurrentPosition(data,1);
				OffsetToText=data->CurrentPosition.OffsetToText;

				c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
				c=DefaultToExtendedAsciiArray[c];

				textBuffer[OffsetToText]=(U8)(c&0xff);
				textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
			}
			else
			{	c=DefaultToAsciiArray[c];
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
				textBuffer[OffsetToText+1]=(U8)(c&0xff);
				textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
				textBuffer[OffsetToText]=(U8)(c&0xff);
				textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
			}
			if(ForwardCurrentPosition(data,1)!=1) break;
		}
	}
	else
	{	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
			// if(c==EMS_EURO_CHARACTER_UCS2) c=EMS_EURO_CHARACTER;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
			if(ForwardCurrentPosition(data,1)!=1) break;
		}
	}
	ResetCurrentPosition(data);
}

/*	Call before EMSPack	*/
void EMS_ConvertAsciiEncodingToGSM7BitDefaultWithExtended(EMSData *data)
{	U16 c,OffsetToText;
	U8* textBuffer=data->textBuffer;
	U16 textBufferLength=data->textLength;
	EMSObject* object;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
	U8	ESC_string[]={0x00,0x1B};
#else
	U8	ESC_string[]={0x1B,0x00};
#endif
	if(textBuffer==NULL) return;
	ResetCurrentPosition(data);

	if(data->dcs==SMSAL_DEFAULT_DCS)
	{	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
		{
			if(coolsand_UI_EMS_input_box_get_next_object(data,&data->CurrentPosition,&c,&object)!=1)
			{
				continue;
			}
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
			if(UI_TEST_GSM_EXTENDED(c))
			{	AddString(data,ESC_string,1,NULL);
				OffsetToText=data->CurrentPosition.OffsetToText;
				 
				textBufferLength += ENCODING_LENGTH;
				//CSD end
				c=ExtendedAsciiToDefaultArray[c];
			}
			else
			{
				c=AsciiToDefaultArray[c];
			}
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
			if(ForwardCurrentPosition(data,1)!=1) break;
		}
	}
	else
	{	while((OffsetToText=data->CurrentPosition.OffsetToText)<textBufferLength)
		{
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			c=(U16)(textBuffer[OffsetToText+1]|(textBuffer[OffsetToText]<<8));
#else
			c=(U16)(textBuffer[OffsetToText]|(textBuffer[OffsetToText+1]<<8));
#endif
			// if(c==EMS_EURO_CHARACTER) c=EMS_EURO_CHARACTER_UCS2;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
			textBuffer[OffsetToText+1]=(U8)(c&0xff);
			textBuffer[OffsetToText+0]=(U8)(c>>8);
#else
			textBuffer[OffsetToText]=(U8)(c&0xff);
			textBuffer[OffsetToText+1]=(U8)(c>>8);
#endif
			if(ForwardCurrentPosition(data,1)!=1) break;
		}
	}
	ResetCurrentPosition(data);
}

#endif

/*	Temporary: Will be removed later	*/
UI_string_type		temp_EMS_buffer;
EMSData				*temp_EMS_data;

/*	EMS input box: Low level wrappers	*/

void (*EMS_inputbox_input_callback)(void);

void register_EMS_inputbox_input_callback(void (*f)(void))
{	EMS_inputbox_input_callback=f;
}

#if(!UI_DISABLE_EMS_INPUT_BOX)

 
//#define ENABLE_EMS_INPUTBOX_FRAME_SKIPPING			1
 

byte				EMS_inputbox_has_changed=0;
UI_EMS_input_box	MMI_EMS_inputbox;
EMSTextFormat		wgui_EMS_text_format;

extern void wgui_EMS_redraw_remaining_characters_display(void);

#if((UI_ENABLE_FRAME_SKIPPING)&&(ENABLE_EMS_INPUTBOX_FRAME_SKIPPING))
S32 wgui_EMS_inputbox_frame_counter=0;
#endif

void show_EMS_inputbox(void)
{
#if(0)
	PRINT_INFORMATION(("\n\n"));
	if(MMI_EMS_inputbox.data->CurrentPosition.Object!=NULL)
	{
		if(MMI_EMS_inputbox.data->CurrentPosition.Object->Type==EMS_TYPE_TEXT_FORMAT)
		{	PRINT_INFORMATION(("O: *"));
		}
		else
		{	PRINT_INFORMATION(("O: %d",MMI_EMS_inputbox.data->CurrentPosition.Object->data.picture.pdu[0]));
		}
	}
	PRINT_INFORMATION((" TL: %d, TO: %d, T: %c",MMI_EMS_inputbox.data->textLength,MMI_EMS_inputbox.data->CurrentPosition.OffsetToText,MMI_EMS_inputbox.data->textBuffer[MMI_EMS_inputbox.data->CurrentPosition.OffsetToText+1]));
#endif
	coolsand_UI_set_EMS_input_box_current_theme(&MMI_EMS_inputbox);

	coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox);
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_BLT_double_buffer(MMI_EMS_inputbox.x,MMI_EMS_inputbox.y,MMI_EMS_inputbox.x+MMI_EMS_inputbox.width-1,MMI_EMS_inputbox.y+MMI_EMS_inputbox.height-1);
#endif
}

#if((UI_ENABLE_FRAME_SKIPPING)&&(ENABLE_EMS_INPUTBOX_FRAME_SKIPPING))

void wgui_EMS_inputbox_end_frame(void)
{	if(wgui_EMS_inputbox_frame_counter>=1)
	{	S32 BLT_y2,y2;
#if(UI_DOUBLE_BUFFER_SUPPORT)
		coolsand_UI_lock_double_buffer();
#endif
		coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox);
		if(EMS_inputbox_has_changed)
		{	wgui_EMS_redraw_remaining_characters_display();
			EMS_inputbox_has_changed=0;
		}
#if(UI_DOUBLE_BUFFER_SUPPORT)
		coolsand_UI_unlock_double_buffer();
		BLT_y2=MMI_EMS_inputbox.y+MMI_EMS_inputbox.height;
		y2=MMI_multitap_y+MMI_multitap_height+1;
		if(BLT_y2<y2) BLT_y2=y2;
		coolsand_UI_BLT_double_buffer(MMI_EMS_inputbox.x,MMI_EMS_inputbox.y,MMI_EMS_inputbox.x+MMI_EMS_inputbox.width-1,BLT_y2);
#endif
		wgui_EMS_inputbox_frame_counter=0;
		coolsand_UI_start_timer(UI_FRAME_SKIP_TIMEOUT,wgui_EMS_inputbox_end_frame);
	}
}

#endif

#if(EMS_INPUTBOX_DEBUG)

#define _DEBUG_PRINT(x)	_debug_print x

extern void DisplayToDebugwindow(S8 *m_nPrintableStr);

void _debug_print(S8 *fmt,...)
{
	S8		display_string[2048];
	va_list args;
	va_start(args,fmt);
	vsprintf(display_string,fmt,args);
	va_end(args);
	DisplayToDebugwindow(display_string);
}

void _DUMP_EMS_TEXT_FORMAT_OBJECT(EMSTextFormat *x)
{	if(x!=NULL)
	{	_DEBUG_PRINT(("L=%03d ",x->textLength));
		_DEBUG_PRINT(("AL="));
		switch(x->Alignment)
		{	case	EMS_ALIGN_LEFT:
					_DEBUG_PRINT(("L"));
					break;
			case	EMS_ALIGN_RIGHT:
					_DEBUG_PRINT(("R"));
					break;
			case	EMS_ALIGN_CENTER:
					_DEBUG_PRINT(("C"));
					break;
			case	EMS_ALIGN_LANG_DEP:
					_DEBUG_PRINT(("D"));
					break;
		}
		_DEBUG_PRINT((" SZ="));
		switch(x->FontSize)
		{	case	EMS_FONTSIZE_NORMAL:
					_DEBUG_PRINT(("N"));
					break;
			case	EMS_FONTSIZE_LARGE:
					_DEBUG_PRINT(("L"));
					break;
			case	EMS_FONTSIZE_SMALL:
					_DEBUG_PRINT(("S"));
					break;
		}
		_DEBUG_PRINT((" ST="));
		if(x->isBold)			_DEBUG_PRINT(("B"));
		if(x->isItalic)			_DEBUG_PRINT(("I"));
		if(x->isUnderline)		_DEBUG_PRINT(("U"));
		if(x->isStrikethrough)	_DEBUG_PRINT(("S"));
	}
}

void _DUMP_EMS_OBJECT(EMSObject *x)
{	if(x==NULL)	_DEBUG_PRINT(("NULL"));
	else
	{	_DEBUG_PRINT(("%8X OFF=%03d ",x,x->OffsetToText));
		switch(x->Type)
		{	case	EMS_TYPE_TEXT_FORMAT:
					_DEBUG_PRINT(("TXT FMT "));
					_DUMP_EMS_TEXT_FORMAT_OBJECT(&x->data->text_format);
					break;
			case	EMS_TYPE_PREDEF_SND:
					_DEBUG_PRINT(("PRE SND %d",x->PredefNo));
					break;
			case	EMS_TYPE_USERDEF_SND:
					_DEBUG_PRINT(("USR SND"));
					break;
			case	EMS_TYPE_PREDEF_ANM:
					_DEBUG_PRINT(("PRE ANM %d",x->PredefNo));
					break;
			case	EMS_TYPE_USERDEF_ANM:
					_DEBUG_PRINT(("USR ANM"));
					break;
			case	EMS_TYPE_PIC:
					_DEBUG_PRINT(("EMS PIC"));
					break;
		}
	}
}

#else

#define _DEBUG_PRINT(x)

void _DUMP_EMS_OBJECT(EMSObject *x)
{	UI_UNUSED_PARAMETER(x);
}

#endif

extern U16 GetExitScrnID(void);
#include "idleappdef.h"

void redraw_EMS_inputbox(void)
{
 if((SCR_POPUP_SCREEN != GetExitScrnID())&&(POPUP_SCREENID !=GetExitScrnID()))
{

#if(EMS_INPUTBOX_DEBUG)
	S32					i=0;
	UI_character_type	c='-';
	EMSObject			*object=MMI_EMS_inputbox.data->listHead;
	EMSPosition			mapped_position;

	EMS_map_position(MMI_EMS_inputbox.data,&MMI_EMS_inputbox.data->CurrentPosition,&mapped_position);
	if((MMI_EMS_inputbox.data->textBuffer!=NULL)&&(MMI_EMS_inputbox.data->textLength>0)&&(MMI_EMS_inputbox.data->CurrentPosition.OffsetToText<MMI_EMS_inputbox.data->textLength))
		c=MMI_EMS_inputbox.data->textBuffer[MMI_EMS_inputbox.data->CurrentPosition.OffsetToText];
	if((c==0x0A)||(c==0x0D)) c='.';
	_DEBUG_PRINT(("\n\nCurrent position: %c OFF=%03d, Object=",c,MMI_EMS_inputbox.data->CurrentPosition.OffsetToText));
	_DUMP_EMS_OBJECT(MMI_EMS_inputbox.data->CurrentPosition.Object);
	_DEBUG_PRINT(("\nMapped position:  %c OFF=%03d, Object=",c,mapped_position.OffsetToText,mapped_position.Object));
	_DUMP_EMS_OBJECT(mapped_position.Object);
	_DEBUG_PRINT(("\nInputbox text format: "));
	_DUMP_EMS_TEXT_FORMAT_OBJECT(&MMI_EMS_inputbox.text_format);
	_DEBUG_PRINT(("\nWGUI text format:     "));
	_DUMP_EMS_TEXT_FORMAT_OBJECT(&wgui_EMS_text_format);
	_DEBUG_PRINT(("\nCurrent text format:  "));
	_DUMP_EMS_OBJECT(MMI_EMS_inputbox.data->CurrentTextFormatObj);
	while(object!=NULL)
	{	_DEBUG_PRINT(("\nO%d=",i));
		_DUMP_EMS_OBJECT(object);
		object=object->next;
		i++;
	}
	for(i=0;i<MMI_EMS_inputbox.n_lines;i++)
	{	_DEBUG_PRINT(("\nL%d OFF=%03d, Count=%02d, x=%02d, Object=",i,MMI_EMS_inputbox.line_positions[i].OffsetToText,MMI_EMS_inputbox.line_object_counts[i],MMI_EMS_inputbox.line_x_positions[i]));
		_DUMP_EMS_OBJECT(MMI_EMS_inputbox.line_positions[i].Object);
	}
	_DEBUG_PRINT(("\n"));
#endif
#if(0)
	PRINT_INFORMATION(("\n\n"));
	if(MMI_EMS_inputbox.data->CurrentPosition.Object!=NULL)
	{
		if(MMI_EMS_inputbox.data->CurrentPosition.Object->Type==EMS_TYPE_TEXT_FORMAT)
		{	PRINT_INFORMATION(("O: *"));
		}
		else
		{	PRINT_INFORMATION(("O: %d",MMI_EMS_inputbox.data->CurrentPosition.Object->data.picture.pdu[0]));
		}
	}
	PRINT_INFORMATION((" TL: %d, TO: %d, T: %c",MMI_EMS_inputbox.data->textLength,MMI_EMS_inputbox.data->CurrentPosition.OffsetToText,MMI_EMS_inputbox.data->textBuffer[MMI_EMS_inputbox.data->CurrentPosition.OffsetToText+1]));
#endif

#if((UI_ENABLE_FRAME_SKIPPING)&&(ENABLE_EMS_INPUTBOX_FRAME_SKIPPING))
	if(wgui_EMS_inputbox_frame_counter>0)
	{	wgui_EMS_inputbox_frame_counter++;
	}
	else
	{	wgui_EMS_inputbox_frame_counter=1;
		coolsand_UI_start_timer(UI_FRAME_START_TIMEOUT,wgui_EMS_inputbox_end_frame);
	}
#else

	coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox);
 
	if(!(MMI_EMS_inputbox.flags&UI_EMS_INPUT_BOX_VIEW_MODE))
	{
	   wgui_EMS_redraw_remaining_characters_display();
//		EMS_inputbox_has_changed=0;
	}
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_BLT_double_buffer(MMI_EMS_inputbox.x,MMI_EMS_inputbox.y,MMI_EMS_inputbox.x+MMI_EMS_inputbox.width-1,MMI_EMS_inputbox.y+MMI_EMS_inputbox.height-1);
#endif

#endif
}
}

void show_EMS_inputbox_no_draw(void)
{
	MMI_EMS_inputbox.flags|=(UI_EMS_INPUT_BOX_DISABLE_DRAW);
	coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox);
	MMI_EMS_inputbox.flags&=~(UI_EMS_INPUT_BOX_DISABLE_DRAW);
}

/*zhoumn Add Start For Bug 5738 Ver: TBM_780  on 2007-8-17 17:41 */
#ifdef __MMI_TOUCH_SCREEN__
void EMS_inputbox_previous_TP(void)
{
    
   if (!(MMI_EMS_inputbox.flags&UI_EMS_INPUT_BOX_VIEW_MODE))
   {
      disable_active_multitap();
   }
   coolsand_UI_EMS_input_box_previous(&MMI_EMS_inputbox);
   wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
#if defined(__MMI_GB__)
	GbDisplayForEmsEditor();
#else
	redraw_EMS_inputbox();
#endif
}

void EMS_inputbox_next_TP(void)
{
    
   if (!(MMI_EMS_inputbox.flags&UI_EMS_INPUT_BOX_VIEW_MODE))
   {
      disable_active_multitap();
   }
   coolsand_UI_EMS_input_box_next(&MMI_EMS_inputbox);
   wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
#if defined(__MMI_GB__)
	GbDisplayForEmsEditor();
#else
	redraw_EMS_inputbox();
#endif
}
#endif
/*zhoumn Add End  For Bug 5738 Ver: TBM_780  on 2007-8-17 17:41 */

void EMS_inputbox_previous(void)
{
    
   if (!(MMI_EMS_inputbox.flags&UI_EMS_INPUT_BOX_VIEW_MODE))
   {
      disable_active_multitap();
   }
   coolsand_UI_EMS_input_box_previous(&MMI_EMS_inputbox);
   wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
   redraw_EMS_inputbox();
}

void EMS_inputbox_next(void)
{
    
   if (!(MMI_EMS_inputbox.flags&UI_EMS_INPUT_BOX_VIEW_MODE))
   {
      disable_active_multitap();
   }
   coolsand_UI_EMS_input_box_next(&MMI_EMS_inputbox);
   wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
   redraw_EMS_inputbox();
}

#ifdef __MMI_MESSAGES_EMS__
void EMS_inputbox_previous_object(void)
{
   coolsand_UI_EMS_input_box_view_previous_object(&MMI_EMS_inputbox);
   wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
   redraw_EMS_inputbox();
}

void EMS_inputbox_next_object(void)
{
   coolsand_UI_EMS_input_box_view_next_object(&MMI_EMS_inputbox);
   wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
   redraw_EMS_inputbox();
}
#endif

/*zhoumn Add Start For Bug 5738 Ver: TBM_780  on 2007-8-17 17:41 */
#ifdef __MMI_TOUCH_SCREEN__
void EMS_inputbox_previous_line_TP(void)
{
    
   if (!(MMI_EMS_inputbox.flags&UI_EMS_INPUT_BOX_VIEW_MODE))
   {
      disable_active_multitap();
   }
   coolsand_UI_EMS_input_box_previous_line(&MMI_EMS_inputbox);
   wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
#if defined(__MMI_GB__)
	GbDisplayForEmsEditor();
#else
	redraw_EMS_inputbox();
#endif
}

void EMS_inputbox_next_line_TP(void)
{
    
   if (!(MMI_EMS_inputbox.flags&UI_EMS_INPUT_BOX_VIEW_MODE))
   {
      disable_active_multitap();
   }
   coolsand_UI_EMS_input_box_next_line(&MMI_EMS_inputbox);
   wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
#if defined(__MMI_GB__)
	GbDisplayForEmsEditor();
#else
	redraw_EMS_inputbox();
#endif
}
#endif
/*zhoumn Add End  For Bug 5738 Ver: TBM_780  on 2007-8-17 17:41 */
void EMS_inputbox_previous_line(void)
{
    
   if (!(MMI_EMS_inputbox.flags&UI_EMS_INPUT_BOX_VIEW_MODE))
   {
      disable_active_multitap();
   }
   coolsand_UI_EMS_input_box_previous_line(&MMI_EMS_inputbox);
   wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
   redraw_EMS_inputbox();
}

void EMS_inputbox_next_line(void)
{
    
   if (!(MMI_EMS_inputbox.flags&UI_EMS_INPUT_BOX_VIEW_MODE))
   {
      disable_active_multitap();
   }
   coolsand_UI_EMS_input_box_next_line(&MMI_EMS_inputbox);
   wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
   redraw_EMS_inputbox();
}

UI_character_type EMS_GSM_extended_lookup[]=
{	/*'[',']','{','}','~','\\','^','|',*/0xA2,	/*	Euro character is mapped to 0xA2, Deleted by panxu not support EMS 2008-12-11*/
};

byte EMS_GSM_extended_map(UI_character_type c)
{	S32	i,n;

	if(c&0xff00) return(0);
	n=sizeof(EMS_GSM_extended_lookup)/sizeof(UI_character_type);
	for(i=0;i<n;i++)
	{	if(c==EMS_GSM_extended_lookup[i]) return(1);
	}
	return(0);
}

void EMS_inputbox_direct_input(UI_character_type c)
{	
	U8 r = 0;
	#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_editor_do_confirm();
	#endif
	if ((c == 10) && (MMI_EMS_inputbox.data->textLength < 534))
	{   
	#ifdef __MMI_GB__
		InsertEnter++; //此时插入的是回车符
	#endif	
	}
	if (UI_TEST_LF_CHARACTER(c) 
	#ifdef __MMI_GB__
        && EnterSymboFlag == FALSE
	#endif	
        )
		coolsand_UI_EMS_input_box_insert_new_line(&MMI_EMS_inputbox);
	else
	{
	#ifdef __MMI_GB__	
		EnterSymboFlag = FALSE ;
	#endif	
#if(!EMS_USE_GSM_EXTENDED)
	    if(MMI_EMS_inputbox.UCS2_count==0)
	    {
		if(EMS_GSM_extended_map(c)) coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,0x1b);
	    }
#endif
		r = coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,c);
	}

	if (r != EMS_NO_SPACE)
	{
		if (MMI_EMS_inputbox.UCS2_count > 0)
			MMI_EMS_inputbox.data->Reminder.totalRemainingOctet -= 2;   
		else
			MMI_EMS_inputbox.data->Reminder.totalRemainingOctet -= 1;
	}	
	
	redraw_EMS_inputbox();
	EMS_inputbox_input_callback();
}

void EMS_inputbox_direct_input_no_LF_check(UI_character_type c)
{	coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,c);
	redraw_EMS_inputbox();
	EMS_inputbox_input_callback();
}

void EMS_inputbox_backspace(void)
{		
	coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
	wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
/*zhoumn Modify Start For 5951 Ver: TBM_780  on 2007-8-9 14:58 */
#if defined(__MMI_GB__)
	GbDisplayForEmsEditor();
	wgui_EMS_redraw_remaining_characters_display();
#else
	redraw_EMS_inputbox();
#endif
/*zhoumn Modify End  For 5951 Ver: TBM_780  on 2007-8-9 14:58 */
	EMS_inputbox_input_callback();
}

/**************************************************************

	FUNCTION NAME		: EMS_inputbox_delete_all

  	PURPOSE				: Delete all charater

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: freeIndex

	RETURNS				: void

 

**************************************************************/
void EMS_inputbox_delete_all(void)
{	
	coolsand_UI_EMS_input_box_delete_all(&MMI_EMS_inputbox);
	#if defined(__MMI_GB__)
	MMI_EMS_inputbox.data->Reminder.totalRemainingOctet = LargestByteNumberInSms;    // add by yaosq 20061222 for init
	#endif
	MMI_EMS_inputbox.data->textLength = 0;
	/*Guojian Add Start For 8814 Ver: Jassmine  on 2008-6-23 11:7 */
	memset(MMI_EMS_inputbox.data->textBuffer, 0, MMI_EMS_inputbox.data->textBufferSize);
	/*Guojian Add End  For 8814 Ver: Jassmine  on 2008-6-23 11:7 */
	wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
	redraw_EMS_inputbox();
	EMS_inputbox_input_callback();
}

void EMS_inputbox_toggle_insert_mode(void)
{	coolsand_UI_EMS_input_box_toggle_insert_mode(&MMI_EMS_inputbox);
	redraw_EMS_inputbox();
}

void EMS_inputbox_delete(void)
{	coolsand_UI_EMS_input_box_delete(&MMI_EMS_inputbox);
	wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
	redraw_EMS_inputbox();
	EMS_inputbox_input_callback();
}

void EMS_inputbox_insert_new_line(void)
{
#if defined(__MMI_GB__)
	coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,EMS_input_box_new_line_character);
#else
	coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,'\n');
#endif
	EMS_inputbox_input_callback();
}

void EMS_inputbox_alphanumeric_keyboard_input_handler(S32 keyc)
{	if(keyc==0xc0)		EMS_inputbox_direct_input_no_LF_check((UI_character_type)'\n');
	else if(keyc==0xbb) EMS_inputbox_direct_input((UI_character_type)0x0d);
	else if(keyc==0x08) EMS_inputbox_backspace();
	else if(keyc==0x1b) EMS_inputbox_delete_all();
	else if(keyc==0x0d) EMS_inputbox_direct_input((UI_character_type)'\n');
	else if(keyc==0xbd) EMS_inputbox_direct_input((UI_character_type)0x1b);
	else if(keyc>0x1f)	EMS_inputbox_direct_input((UI_character_type)keyc);
}

void EMS_inputbox_key_handler(S32 vkey_code,S32 key_state)
{
#if(MMI_BUILD_TYPE==BUILD_TYPE_X86WIN32)
	if(key_state)
	{	switch(vkey_code)
		{	case 37:
#if(ENABLE_EMS_TEST_WORD_HIGHLIGHT)
						MMI_EMS_inputbox.flags&=~UI_EMS_INPUT_BOX_WORD_HIGHLIGHT;
#endif
						EMS_inputbox_previous();
						break;
			case 38:
#if(ENABLE_EMS_TEST_WORD_HIGHLIGHT)
						coolsand_UI_EMS_input_box_highlight_cursor_start(&MMI_EMS_inputbox);
						show_EMS_inputbox();
#else
						EMS_inputbox_previous_line();
#endif
						break;
			case 39:
#if(ENABLE_EMS_TEST_WORD_HIGHLIGHT)
						MMI_EMS_inputbox.flags&=~UI_EMS_INPUT_BOX_WORD_HIGHLIGHT;
#endif
						EMS_inputbox_next();
						break;
			case 40:
#if(ENABLE_EMS_TEST_WORD_HIGHLIGHT)
						coolsand_UI_EMS_input_box_highlight_cursor_end(&MMI_EMS_inputbox);
						show_EMS_inputbox();
#else
						EMS_inputbox_next_line();
#endif
						break;
			case 36:	//home
						break;
			case 35:	//end
						break;
			case 33:	//page up
						break;
			case 34:	//page down
						break;
			case 45:	EMS_inputbox_toggle_insert_mode();
						break;
			case 46:	EMS_inputbox_delete();
						break;
		}
	}
#else
	UI_UNUSED_PARAMETER(vkey_code);
	UI_UNUSED_PARAMETER(key_state);
#endif
}

void EMS_inputbox_viewer_key_handler(S32 vkey_code,S32 key_state)
{
#if(MMI_BUILD_TYPE==BUILD_TYPE_X86WIN32)
	if(key_state)
	{	switch(vkey_code)
		{	case 37:
					#ifdef __MMI_MESSAGES_EMS__
						EMS_inputbox_previous_object();
					#else
						EMS_inputbox_previous();
					#endif

						break;
			case 38:	EMS_inputbox_previous_line();
						break;

			case 39:
					#ifdef __MMI_MESSAGES_EMS__
						EMS_inputbox_next_object();
					#else
						EMS_inputbox_next();
					#endif
						break;
			case 40:	EMS_inputbox_next_line();
						break;
			case 36:	//home
						break;
			case 35:	//end
						break;
			case 33:	//page up
						break;
			case 34:	//page down
						break;
			case 45:	//insert
						break;
			case 46:	//delete
						break;
		}
	}
#else
	UI_UNUSED_PARAMETER(vkey_code);
	UI_UNUSED_PARAMETER(key_state);
#endif
}
void EMS_inputbox_multitap_input(UI_character_type c)
{	
	U8 r = 0;
	#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_editor_do_confirm();
	#endif
	r = coolsand_UI_EMS_input_box_insert_multitap_character(&MMI_EMS_inputbox,c);
	if (r != EMS_NO_SPACE)
	{
		if (MMI_EMS_inputbox.UCS2_count > 0)	
			MMI_EMS_inputbox.data->Reminder.totalRemainingOctet -= 2;  
		else
			MMI_EMS_inputbox.data->Reminder.totalRemainingOctet -= 1;
	}
	redraw_EMS_inputbox();
	EMS_inputbox_input_callback();
}

void EMS_inputbox_multitap_input_complete(void)
{	coolsand_UI_EMS_input_box_confirm_multitap_character(&MMI_EMS_inputbox);
	redraw_EMS_inputbox();
}

void EMS_inputbox_numeric_keyboard_input_handler(S32 keyc)
{	if(keyc>='0' && keyc<='9') 	EMS_inputbox_direct_input((byte)keyc);
	else if(keyc==0x08)			EMS_inputbox_backspace();
	else if(keyc==0x1b)			EMS_inputbox_delete_all();
}
#ifdef __MMI_FULL_KEYPAD__
extern void EMS_inputbox_direct_input_key_handler(S32 key);
#endif
void EMS_inputbox_handle_key_down(MMI_key_code_type k)
{	
	if(k >= KEY_0 &&  k <= KEY_9)
		EMS_inputbox_direct_input((UI_character_type)('0'+k));
	#ifdef __MMI_FULL_KEYPAD__
	else if(k >= KEY_COMMA)
		EMS_inputbox_direct_input_key_handler(k);
	#endif
}

/*zhoumn Modify Start For Bug 5738 Ver: TBM_780  on 2007-8-17 17:47 */
void register_EMS_inputbox_keys(void)
{	
#if defined(__MMI_TOUCH_SCREEN__)
	SetKeyHandler(EMS_inputbox_previous_TP,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EMS_inputbox_next_TP,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EMS_inputbox_previous_line_TP,KEY_UP_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EMS_inputbox_next_line_TP,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
	//SetKeyHandler(EMS_inputbox_backspace,KEY_CLEAR,KEY_EVENT_DOWN);
	register_keyboard_key_handler(EMS_inputbox_key_handler);
#else
	SetKeyHandler(EMS_inputbox_previous,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EMS_inputbox_next,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EMS_inputbox_previous_line,KEY_UP_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EMS_inputbox_next_line,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
	//SetKeyHandler(EMS_inputbox_backspace,KEY_CLEAR,KEY_EVENT_DOWN);
	register_keyboard_key_handler(EMS_inputbox_key_handler);
#endif
/*zhoumn Modify End  For Bug 5738 Ver: TBM_780  on 2007-8-17 17:47 */
}

void wgui_EMS_inputbox_handle_symbol_picker_star_key(void)
{	disable_active_multitap();
	EntrySymbolPickerScreen();
}
 
#ifdef __MMI_MULTITAP_THAI__
void wgui_EMS_inputbox_handle_symbol_picker_star_key_bymultitapthai(void)
{
	if(Get_inputbox_update_states() == 1)
	{
		coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
		wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
	}
	wgui_EMS_inputbox_handle_symbol_picker_star_key();
}
#endif
 
#if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
#else
extern FuncPtr currKeyFuncPtrs[MAX_KEYS][MAX_KEY_TYPE];
#endif
 
#ifdef __MMI_MULTITAP_THAI__
//return 0 not accept input
//return 1 accept input
//return 2 normal
//return 3 means reset the MMI_active_multitap. not use
byte EMS_inputbox_check_multitap_thai_input(UI_character_type inputchar,byte *updatef)
{
	EMSPosition			p=MMI_EMS_inputbox.data->CurrentPosition;
	EMSObject*			current_object;
	UI_character_type	c;
	byte				r;
	UI_character_type   cc;

	r = coolsand_UI_EMS_input_box_get_previous_object(MMI_EMS_inputbox.data,&p,&c,&current_object);

	if(r == 1)//means that the previous is character
	{
		if((inputchar >= 0x0e34 && inputchar <= 0x0e39) || inputchar == 0x0e31)	//Above Vowels and Below Vowels
		{
			if(c == 0x0e30 || c == 0x0e32 || c == 0x0e33 || c == 0x0e46 || c == 0x0e2f || c == 0x0e3f || (c >= 0x0e40 && c <= 0x0e44) || c == 0x0030)
			{
				if(c ==  0x0030 && *updatef == 1)
				{
					MMI_EMS_inputbox.flags&=~UI_EMS_INPUT_BOX_STATE_MULTITAP;
					coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
					coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,inputchar);
					redraw_EMS_inputbox();
					wgui_EMS_redraw_remaining_characters_display();
					return 1;
				}
				coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,0x0030);
				redraw_EMS_inputbox();
				wgui_EMS_redraw_remaining_characters_display();
				return 0;	//not accept input
			}
			else if((c == 0x0e31 || (c >= 0x0e34 && c <= 0x0e39)))
			{//Above Vowels and Below Vowels
				if(*updatef == 0)
				{
					coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,0x0030);
					redraw_EMS_inputbox();
					wgui_EMS_redraw_remaining_characters_display();
					return 0;
				}
				coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
				coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,inputchar);
				redraw_EMS_inputbox();
				wgui_EMS_redraw_remaining_characters_display();
				return 1;
			}
			else if(c >= 0x0e47 && c <= 0x0e4c)
			{//tone
				if(*updatef == 0)
				{
					coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,0x0030);
					redraw_EMS_inputbox();
					wgui_EMS_redraw_remaining_characters_display();
					return 0;
				}
				coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
				if(c == 0x0e47)
				{
					coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,inputchar);
					redraw_EMS_inputbox();
					wgui_EMS_redraw_remaining_characters_display();
					return 1;
				}
				r = coolsand_UI_EMS_input_box_get_previous_object(MMI_EMS_inputbox.data,&p,&cc,&current_object);
				if (r == 1 && ((cc >= 0x0e34 && cc <= 0x0e39) || cc == 0x0e31))
				{
					coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
					coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,inputchar);
					coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,c);
					redraw_EMS_inputbox();
					wgui_EMS_redraw_remaining_characters_display();
				}
				else
				{
					coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,inputchar);
					coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,c);
					redraw_EMS_inputbox();
					wgui_EMS_redraw_remaining_characters_display();
				}
				return 1;
			}
			else if(c >= 0x0e01 && c <= 0x0e2e)
			{
				//msz00065_20051010 peng hua add for multitap thai begin
				if(coolsand_UI_EMS_check_input_test_overflow(&MMI_EMS_inputbox,inputchar))
				//msz00065_20051010 peng hua add for multitap thai end
					*updatef = 1;
				coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,inputchar);
				redraw_EMS_inputbox();
				wgui_EMS_redraw_remaining_characters_display();
				return 1;
			}
			else
			{
				if (*updatef == 0)
				{
					coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,0x0030);
					*updatef = 0;
					redraw_EMS_inputbox();
					wgui_EMS_redraw_remaining_characters_display();
				}
				return 0;
			}
		}
		else if(inputchar == 0x0030)
		{
			if (*updatef == 1)
			{
				coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
//				redraw_EMS_inputbox();
				return 2;
			}
		}
		else if(inputchar >= 0x0e47 && inputchar <= 0x0e4c)
		{
			if(c == 0x0e30 || c == 0x0e32 || c == 0x0e33 || c == 0x0e46 || c == 0x0e2f || c == 0x0e3f || (c >= 0x0e40 && c <= 0x0e44) || c == 0x0030)
			{
				if((c == 0x0e46 || c == 0x0e2f || c == 0x0e3f) && *updatef == 1)
				{
					MMI_EMS_inputbox.flags&=~UI_EMS_INPUT_BOX_STATE_MULTITAP;
					redraw_EMS_inputbox();
					coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
					coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,inputchar);
					redraw_EMS_inputbox();
					wgui_EMS_redraw_remaining_characters_display();
				}
				return 1;	//not accept input
			}
			else if((c >= 0x0e47 && c <= 0x0e4c))
			{//tone
				if(*updatef == 0)
					return 1;

				coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
				if(inputchar == 0x0e47)	//
				{
					r = coolsand_UI_EMS_input_box_get_previous_object(MMI_EMS_inputbox.data,&p,&cc,&current_object);
					if (r == 1 && ((cc >= 0x0e34 && cc <= 0x0e39) || cc == 0x0e31))
					{
						coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
						coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,inputchar);
						redraw_EMS_inputbox();
						wgui_EMS_redraw_remaining_characters_display();
						return 1;
					}
				}
				coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,inputchar);
				redraw_EMS_inputbox();
				wgui_EMS_redraw_remaining_characters_display();
				return 1;
			}
			else if(c >= 0x0e01 && c <= 0x0e2e)
			{
				//msz00065_20051010 peng hua add for multitap thai begin
				if(coolsand_UI_EMS_check_input_test_overflow(&MMI_EMS_inputbox,inputchar))
				//msz00065_20051010 peng hua add for multitap thai end
				*updatef = 1;
				coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,inputchar);
				redraw_EMS_inputbox();
				wgui_EMS_redraw_remaining_characters_display();
				return 1;
			}
			else if((c >= 0x0e34 && c <= 0x0e39) || c == 0x0e31)
			{
				//msz00065_20051010 peng hua add for multitap thai begin
				if(coolsand_UI_EMS_check_input_test_overflow(&MMI_EMS_inputbox,inputchar))
				//msz00065_20051010 peng hua add for multitap thai end
				*updatef = 1;
				if(inputchar == 0x0e47)	//
				{
					coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
				}
				coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,inputchar);
				redraw_EMS_inputbox();
				wgui_EMS_redraw_remaining_characters_display();
				return 1;
			}
			else
			{
				if(*updatef == 1)
					coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
				return 1;
			}
		}
		if((inputchar == 0x0e46 || inputchar == 0x0e2f || inputchar == 0x0e3f) && *updatef == 1)
		{
			coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
			MMI_EMS_inputbox.flags&=~UI_EMS_INPUT_BOX_STATE_MULTITAP;
//			coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox,inputchar);
//			redraw_EMS_inputbox();
			return 2;
		}
	}
	else	//object
	{
		if((inputchar >= 0x0e34 && inputchar <= 0x0e39) || inputchar == 0x0e31)
			return 0;
		if(inputchar >= 0x0e47 && inputchar <= 0x0e4c)
			return 0;
	}
	return 2;
}
#endif

void register_EMS_multitap_function(void)
{
#if 1// defined(__MMI_FULL_SCREEN_EDITOR__)
   register_multitap_no_draw_key_handlers();
#else
   register_multitap_key_handlers();
#endif
   register_keyboard_input_handler(EMS_inputbox_alphanumeric_keyboard_input_handler);
   set_multitap_functions(EMS_inputbox_multitap_input,EMS_inputbox_multitap_input_complete);
   wgui_set_EMS_inputbox_RSK();
   register_EMS_inputbox_keys();
#if !defined(__MMI_FULL_KEYPAD__)   
   SetKeyHandler(wgui_EMS_inputbox_handle_symbol_picker_star_key,KEY_STAR,KEY_EVENT_DOWN);
#endif
}
#ifdef __MMI_FULL_KEYPAD__
void EMS_inputbox_direct_input_key_handler(S32 key)
{
	UI_character_type c = 0;
	c = MMI_get_char_by_key_code(key, getCharSet());
	if(c!=0)
		EMS_inputbox_direct_input(c);
}
void register_EMS_direct_input_function(void)
{
	clear_MMI_key_input_handler();
	register_MMI_key_input_handler();
	register_key_up_handler(UI_dummy_function_S32);
	register_key_down_handler(EMS_inputbox_direct_input_key_handler);
	wgui_set_EMS_inputbox_RSK();
	register_EMS_inputbox_keys();
#ifdef __MMI_BLACKBERRY_QWERTY__
	SetKeyHandler(wgui_inputbox_handle_symbol_picker_star_key_down, KEY_SYM, KEY_EVENT_UP);
#else
	SetKeyHandler(wgui_EMS_inputbox_handle_symbol_picker_star_key,KEY_CTRL1,KEY_EVENT_DOWN);
#endif
}
#endif
void change_EMS_inputbox_mode(byte mode)
{
	TRACE_EF(g_sw_IM, "change_EMS_inputbox_mode mode = %d", mode);
    
#if defined(__MMI_MULTITAP_THAI__)
   SetKeyHandler(handle_category28_change_input_mode,KEY_POUND,KEY_EVENT_DOWN);
#endif
#if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
#else
   ClearKeyHandler(KEY_POUND,KEY_EVENT_DOWN);
#endif

   switch(mode)
   {
      case INPUT_MODE_MULTITAP_UPPERCASE_ABC:
         change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_ABC);
	  #ifndef __MMI_FULL_KEYPAD__
         register_EMS_multitap_function();
	  #else
	  setCharSet(INPUT_MODE_MULTITAP_UPPERCASE_ABC);
	  register_EMS_direct_input_function();
	  #endif
         break;

      case INPUT_MODE_MULTITAP_LOWERCASE_ABC:
         change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_ABC);
	  #ifndef __MMI_FULL_KEYPAD__
         register_EMS_multitap_function();
	#else
	setCharSet(INPUT_MODE_MULTITAP_LOWERCASE_ABC);
	register_EMS_direct_input_function();
	#endif
         break;
	case INPUT_MODE_123:
         change_multitap_mode(INPUT_MODE_123);
         clear_multitap_key_handlers();
         register_MMI_key_input_handler();
         register_key_down_handler(EMS_inputbox_handle_key_down);
         register_keyboard_input_handler(EMS_inputbox_numeric_keyboard_input_handler);
         wgui_set_EMS_inputbox_RSK();
         register_EMS_inputbox_keys();
	
         if((MMI_current_input_type & INPUT_TYPE_MASK) == INPUT_TYPE_NUMERIC_CHANGEABLE)
         {
         	#ifndef __MMI_FULL_KEYPAD__
         	SetKeyHandler(wgui_EMS_inputbox_handle_symbol_picker_star_key,KEY_STAR,KEY_EVENT_DOWN);
         	#else
         	SetKeyHandler(wgui_EMS_inputbox_handle_symbol_picker_star_key,KEY_CTRL1,KEY_EVENT_DOWN);
         	#endif
         }
         break;
#if defined(__MMI_MULTITAP_ARABIC__)		 
	case INPUT_MODE_MULTITAP_ARABIC:
		change_multitap_mode(INPUT_MODE_MULTITAP_ARABIC);
		register_EMS_multitap_function();
		break;
#endif		
       
#if defined(__MMI_MULTITAP_THAI__)
      case INPUT_MODE_MULTITAP_THAI:
         change_multitap_mode(INPUT_MODE_MULTITAP_THAI);
#if defined(__MMI_FULL_SCREEN_EDITOR__)
         register_multitap_no_draw_key_handlers();
#else
         register_multitap_key_handlers();
#endif
         //register_MMI_key_input_extend_handler();
         //change_inputbox_type_multitapthai(EMS_INPUTBOX);		//msz065_20050420
         register_keyboard_input_handler(EMS_inputbox_alphanumeric_keyboard_input_handler);
         set_multitap_functions(EMS_inputbox_multitap_input,EMS_inputbox_multitap_input_complete);
         wgui_set_EMS_inputbox_RSK();
         register_EMS_inputbox_keys();
         SetKeyHandler(handle_category28_change_input_mode_bymultitapthai,KEY_POUND,KEY_LONG_PRESS);
         SetKeyHandler(wgui_EMS_inputbox_handle_symbol_picker_star_key_bymultitapthai,KEY_STAR,KEY_LONG_PRESS);
         SetKeyHandler(wgui_EMS_inputbox_handle_symbol_picker_star_key_bymultitapthai,KEY_STAR,KEY_EVENT_DOWN);
	
	 SetKeyHandler(handle_category28_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);

         break;
#endif

#if defined(__MMI_MULTITAP_SPANISH__)
      case INPUT_MODE_MULTITAP_UPPERCASE_SPANISH:
         change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_SPANISH);
         register_EMS_multitap_function();
         break;

      case INPUT_MODE_MULTITAP_LOWERCASE_SPANISH:
         change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_SPANISH);
         register_EMS_multitap_function();
         break;
#endif
 
#if defined(__MMI_MULTITAP_DANISH__)
    	case INPUT_MODE_MULTITAP_UPPERCASE_DANISH:
                                change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_DANISH);
                                register_EMS_multitap_function();
				break;

        case INPUT_MODE_MULTITAP_LOWERCASE_DANISH:
                                change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_DANISH);
                                register_EMS_multitap_function();
				break;
#endif

                //qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
                case INPUT_MODE_MMI_MULTITAP_TAMIL:
                change_multitap_mode(INPUT_MODE_MMI_MULTITAP_TAMIL);
                register_EMS_multitap_function();
                break;
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
                case INPUT_MODE_MMI_MULTITAP_BENGALI:
                change_multitap_mode(INPUT_MODE_MMI_MULTITAP_BENGALI);
                register_EMS_multitap_function();
                break;
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
                case INPUT_MODE_MMI_MULTITAP_PUNJABI:
                change_multitap_mode(INPUT_MODE_MMI_MULTITAP_PUNJABI);
                register_EMS_multitap_function();
                break;
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
                case INPUT_MODE_MMI_MULTITAP_TELUGU:
                change_multitap_mode(INPUT_MODE_MMI_MULTITAP_TELUGU);
                register_EMS_multitap_function();
                break;
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
                case INPUT_MODE_MMI_MULTITAP_KANNADA:
                change_multitap_mode(INPUT_MODE_MMI_MULTITAP_KANNADA);
                register_EMS_multitap_function();
                break;
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
                case INPUT_MODE_MMI_MULTITAP_MALAYALAM:
                change_multitap_mode(INPUT_MODE_MMI_MULTITAP_MALAYALAM);
                register_EMS_multitap_function();
                break;
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
                case INPUT_MODE_MMI_MULTITAP_ORIYA:
                change_multitap_mode(INPUT_MODE_MMI_MULTITAP_ORIYA);
                register_EMS_multitap_function();
                break;
#endif
                //qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
                case INPUT_MODE_MMI_MULTITAP_MARATHI:
                change_multitap_mode(INPUT_MODE_MMI_MULTITAP_MARATHI);
                register_EMS_multitap_function();
                break;
#endif
#if defined(__MMI_MULTITAP_URDU__)
                case INPUT_MODE_MMI_MULTITAP_URDU:
                change_multitap_mode(INPUT_MODE_MMI_MULTITAP_URDU);
                register_EMS_multitap_function();
                break;
#endif
 
#if defined(__MMI_SMART_ARABIC__)
		case INPUT_MODE_SMART_ARABIC:
#if defined(__MMI_GB__)
		GbIMEEnShutdown();
	 	InitGbInputMethod_Ara();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
	//	GBSetInputboxLSKFunction(mmi_msg_entry_option_write);    //qiff 2008/10/20 del for greenstone bug:10039                 //add by yaosq 20060901
		GbInputSetRSK();
		GbDisplayForEmsEditor();
#endif        
		break;
		
#endif
#if defined(__MMI_SMART_PERSIAN__)
		case INPUT_MODE_SMART_PERSIAN:

		GbIMEEnShutdown();
	 	InitGbInputMethod_Per();
		GbChangeInputMethod(mode);
		GbClearKeyStarAndPoundHandler();
		GbClearKeyNavigationHandler();
		GbClearKey0To9Handler();
		SetKeyHandler(GbKeyArrowUPHandler,   KEY_UP_ARROW,   KEY_EVENT_DOWN);
		SetKeyHandler(GbKeyArrowLeftHandler,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(GbKeyArrowDownHandler, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(GbKeyArrowRightHandler, KEY_LEFT_ARROW, KEY_EVENT_DOWN);		
		SetKeyHandler(GbKey1PressHandler,KEY_1,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey2PressHandler,KEY_2,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey3PressHandler,KEY_3,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey4PressHandler,KEY_4,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey5PressHandler,KEY_5,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey6PressHandler,KEY_6,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey7PressHandler,KEY_7,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey8PressHandler,KEY_8,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey9PressHandler,KEY_9,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey0PressHandler,KEY_0,KEY_EVENT_DOWN);
	//	GBSetInputboxLSKFunction(mmi_msg_entry_option_write);    //qiff 2008/10/20 del for greenstone bug:10039                 //add by yaosq 20060901
		GbInuptMethodEnterCategory28();                                                         //add  by yaoshq 20060901
		GbInputSetRSK();
		SetKeyHandler(GbKeyStarPressHandler, KEY_STAR, KEY_EVENT_DOWN);
		SetKeyHandler(handle_category28_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
		GbDisplayForEmsEditor();
		break;
		
#endif
#if defined(__MMI_MULTITAP_FRENCH__)
	  case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
	         change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_FRENCH);
	         register_EMS_multitap_function();
	         break;
         case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
	         change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_FRENCH);
	         register_EMS_multitap_function();
	         break;
#endif
#if defined(__MMI_SMART_FRENCH__)
	case INPUT_MODE_SMART_LOWERCASE_FRENCH:
	case INPUT_MODE_SMART_UPPERCASE_FRENCH:
       
		TRACE_EF(g_sw_IM,"初始化法语输入");
	 	GbIMEEnShutdown();
		InitGbInputMethod_fr();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
	//	GBSetInputboxLSKFunction(mmi_msg_entry_option_write);   //qiff 2008/10/20 del for greenstone bug:10039                  //add by yaosq 20060901
		GbInputSetRSK();
		GbDisplayForEmsEditor();
		break;

#endif
#if defined(__MMI_MULTITAP_MYANMAR__)
	case INPUT_MODE_MULTITAP_MYANMAR:
		change_multitap_mode(INPUT_MODE_MULTITAP_MYANMAR);
		register_EMS_multitap_function();
		break;
#endif
#if defined(__MMI_SMART_URDU__)
	case INPUT_MODE_SMART_URDU:
      	 	GbIMEEnShutdown();
		InitGbInputMethod_Urdu();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
		GbInputSetRSK();
		GbDisplayForEmsEditor();
		break;

#endif

#if defined(__MMI_SMART_PUNJABI__)
	case INPUT_MODE_SMART_PUNJABI:
      	 	GbIMEEnShutdown();
		InitGbInputMethod_Punjabi();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
		GbInputSetRSK();
		GbDisplayForEmsEditor();
		break;

#endif

#if defined(__MMI_SMART_BENGALI__)
	case INPUT_MODE_SMART_BENGALI:
      	 	GbIMEEnShutdown();
		InitGbInputMethod_Bengali();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
		GbInputSetRSK();
		GbDisplayForEmsEditor();
		break;

#endif
#if defined(__MMI_SMART_TAMIL__)
	case INPUT_MODE_SMART_TAMIL:
		GbIMEEnShutdown();
		InitGbInputMethod_Tamil();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
		GbInputSetRSK();
		GbDisplayForEmsEditor();
		break;

#endif
#if defined(__MMI_SMART_TURKISH__)
	case INPUT_MODE_SMART_LOWERCASE_TURKISH:
	case INPUT_MODE_SMART_UPPERCASE_TURKISH:
       
		mmi_trace(g_sw_IM,"init input method for turkish");
	 	GbIMEEnShutdown();
		InitGbInputMethod_turkish();
		GbChangeInputMethod(mode);
		GbClearKeyStarAndPoundHandler();
		GbClearKeyNavigationHandler();
		GbClearKey0To9Handler();
		SetKeyHandler(GbKeyArrowUPHandler,   KEY_UP_ARROW,   KEY_EVENT_DOWN);
		SetKeyHandler(GbKeyArrowRightHandler,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(GbKeyArrowDownHandler, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(GbKeyArrowLeftHandler, KEY_LEFT_ARROW, KEY_EVENT_DOWN);		
		SetKeyHandler(GbKey2PressHandler,KEY_2,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey3PressHandler,KEY_3,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey4PressHandler,KEY_4,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey5PressHandler,KEY_5,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey6PressHandler,KEY_6,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey7PressHandler,KEY_7,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey8PressHandler,KEY_8,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey9PressHandler,KEY_9,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey0PressHandler,KEY_0,KEY_EVENT_DOWN);
	//	GBSetInputboxLSKFunction(mmi_msg_entry_option_write);   //qiff 2008/10/20 del for greenstone bug:10039                  //add by yaosq 20060901
		GbInuptMethodEnterCategory28();                                                         //add  by yaoshq 20060901
		GbInputSetRSK();
		SetKeyHandler(GbKeyStarPressHandler, KEY_STAR, KEY_EVENT_DOWN);
		SetKeyHandler(handle_category28_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
		GbDisplayForEmsEditor();
		break;

#endif
#if defined(__MMI_SMART_INDONESIAN__)
	case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
	case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
       
		mmi_trace(g_sw_IM,"init input method for indonesian");
	 	GbIMEEnShutdown();
		InitGbInputMethod_indonesian();
		GbChangeInputMethod(mode);
		GbClearKeyStarAndPoundHandler();
		GbClearKeyNavigationHandler();
		GbClearKey0To9Handler();
		SetKeyHandler(GbKeyArrowUPHandler,   KEY_UP_ARROW,   KEY_EVENT_DOWN);
		SetKeyHandler(GbKeyArrowRightHandler,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(GbKeyArrowDownHandler, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(GbKeyArrowLeftHandler, KEY_LEFT_ARROW, KEY_EVENT_DOWN);		
#ifdef __MMI_FULL_KEYPAD__
	 	GbRegisterKeyHandler(mode);
#else
		SetKeyHandler(GbKey2PressHandler,KEY_2,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey3PressHandler,KEY_3,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey4PressHandler,KEY_4,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey5PressHandler,KEY_5,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey6PressHandler,KEY_6,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey7PressHandler,KEY_7,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey8PressHandler,KEY_8,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey9PressHandler,KEY_9,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey0PressHandler,KEY_0,KEY_EVENT_DOWN);
#endif
		GbInuptMethodEnterCategory28();
		GbInputSetRSK();
		SetKeyHandler(GbKeyStarPressHandler, KEY_STAR, KEY_EVENT_DOWN);
		SetKeyHandler(handle_category28_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
		GbDisplayForEmsEditor();
		break;

#endif
#if defined(__MMI_SMART_MALAY__)
	case INPUT_MODE_SMART_LOWERCASE_MALAY:
	case INPUT_MODE_SMART_UPPERCASE_MALAY:
       
		mmi_trace(g_sw_IM,"init input method for malay");
	 	GbIMEEnShutdown();
		InitGbInputMethod_malay();
		GbChangeInputMethod(mode);
		GbClearKeyStarAndPoundHandler();
		GbClearKeyNavigationHandler();
		GbClearKey0To9Handler();
		SetKeyHandler(GbKeyArrowUPHandler,   KEY_UP_ARROW,   KEY_EVENT_DOWN);
		SetKeyHandler(GbKeyArrowRightHandler,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(GbKeyArrowDownHandler, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(GbKeyArrowLeftHandler, KEY_LEFT_ARROW, KEY_EVENT_DOWN);		
		SetKeyHandler(GbKey2PressHandler,KEY_2,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey3PressHandler,KEY_3,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey4PressHandler,KEY_4,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey5PressHandler,KEY_5,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey6PressHandler,KEY_6,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey7PressHandler,KEY_7,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey8PressHandler,KEY_8,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey9PressHandler,KEY_9,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey0PressHandler,KEY_0,KEY_EVENT_DOWN);
		GbInuptMethodEnterCategory28();
		GbInputSetRSK();
		SetKeyHandler(GbKeyStarPressHandler, KEY_STAR, KEY_EVENT_DOWN);
		SetKeyHandler(handle_category28_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
		GbDisplayForEmsEditor();
		break;

#endif
// beginning--added by kecx for adding italian on 20081223
#if defined(__MMI_SMART_ITALIAN__)
	case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
	case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
		TRACE_EF(g_sw_IM,"初始化意大利语输入");
	 	GbIMEEnShutdown();
		InitGbInputMethod_ita();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
	//	GBSetInputboxLSKFunction(mmi_msg_entry_option_write);   //qiff 2008/10/20 del for greenstone bug:10039                  //add by yaosq 20060901
		GbInputSetRSK();
		GbDisplayForEmsEditor();
		break;
#endif
// end--added by kecx for adding italian on 20081223

//beginning:guoyt added for adding Hindi,German and Spanish IME
#if defined(__MMI_SMART_HINDI__)				
		case INPUT_MODE_SMART_HINDI:

		GbIMEEnShutdown();
	 	InitGbInputMethod_Hin();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
		GbInputSetRSK();
		GbDisplayForEmsEditor();
		break;
		
#endif

#if defined(__MMI_SMART_GERMAN__)
	case INPUT_MODE_SMART_UPPERCASE_GERMAN:
	case INPUT_MODE_SMART_LOWERCASE_GERMAN:
		
	 	GbIMEEnShutdown();
		InitGbInputMethod_Ger();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
		GbInputSetRSK();
		GbDisplayForEmsEditor();
		break;
#endif
#if defined(__MMI_SMART_SPANISH__)
	case INPUT_MODE_SMART_LOWERCASE_SPANISH:
	case INPUT_MODE_SMART_UPPERCASE_SPANISH:
		GbIMEEnShutdown();
		InitGbInputMethod_Spa();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
		GbInputSetRSK();
		GbDisplayForEmsEditor();
		break;
#endif
//end:guoyt added for adding Hindi,German and Spanish IME
#if defined(__MMI_SMART_TELUGU__)				
		case INPUT_MODE_SMART_TELUGU:

		GbIMEEnShutdown();
	 	InitGbInputMethod_Tel();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
		GbInputSetRSK();
		GbDisplayForEmsEditor();
		break;
		
#endif

#if defined(__MMI_MULTITAP_GERMAN__)
      case INPUT_MODE_MULTITAP_UPPERCASE_GERMAN:
         change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_GERMAN);
         register_EMS_multitap_function();
     #ifdef __MMI_GB__
		GbDisplayForEmsEditor();
	#endif
         break;

      case INPUT_MODE_MULTITAP_LOWERCASE_GERMAN:
         change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_GERMAN);
         register_EMS_multitap_function();
     #ifdef __MMI_GB__
		GbDisplayForEmsEditor();
	#endif
         break;
#endif
#if defined(__MMI_MULTITAP_ITALIAN__)
      case INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN:
         change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN);
         register_EMS_multitap_function();
     #ifdef __MMI_GB__
		GbDisplayForEmsEditor();
	#endif
         break;

      case INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN:
         change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN);
         register_EMS_multitap_function();
         break;
#endif
#if defined(__MMI_MULTITAP_RUSSIAN__)
      case INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN:
         change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN);
         register_EMS_multitap_function();
     #ifdef __MMI_GB__
		GbDisplayForEmsEditor();
	#endif
         break;

      case INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN:
         change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN);
         register_EMS_multitap_function();
     #ifdef __MMI_GB__
		GbDisplayForEmsEditor();
	#endif
         break;
#endif
#if defined(__MMI_SMART_RUSSIAN__)
     case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
     case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:	
	GbIMEEnShutdown();
	InitGbInputMethod_Ru();
	GbChangeInputMethod(mode);
	GbRegisterKeyHandler(mode);
	//GBSetInputboxLSKFunction(mmi_msg_entry_option_write);     //qiff 2008/10/20 del for greenstone bug:10039                //add by yaosq 20060901
	GbInputSetRSK();
	GbDisplayForEmsEditor();
	break;
#endif
#if defined(__MMI_GB_SMART_PORTUGUESE__)
	case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE :
	case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
	GbIMEEnShutdown();
	InitGbInputMethod_PT();
	GbChangeInputMethod(mode);
	GbRegisterKeyHandler(mode);
	//GBSetInputboxLSKFunction(mmi_msg_entry_option_write);     //qiff 2008/10/20 del for greenstone bug:10039                //add by yaosq 20060901
	GbInputSetRSK();
	GbDisplayForEmsEditor();
		break;
#endif

#if defined(__MMI_SMART_THAI__)
	case INPUT_MODE_SMART_THAI:
	GbIMEEnShutdown();
	InitGbInputMethod_Thai();
	GbChangeInputMethod(mode);
	GbRegisterKeyHandler(mode);
//	GBSetInputboxLSKFunction(mmi_msg_entry_option_write);      //qiff 2008/10/20 del for greenstone bug:10039               //add by yaosq 20060901
	GbInputSetRSK();
	GbDisplayForEmsEditor();
	break;
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
	case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
	case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
	GbIMEEnShutdown();
	InitGbInputMethod_Vi();
	GbChangeInputMethod(mode);
	GbRegisterKeyHandler(mode);
//	GBSetInputboxLSKFunction(mmi_msg_entry_option_write);      //qiff 2008/10/20 del for greenstone bug:10039               //add by yaosq 20060901
	GbInputSetRSK();
	GbDisplayForEmsEditor();
	break;
#endif
#if defined(__MMI_MULTITAP_TURKISH__)
      case INPUT_MODE_MULTITAP_UPPERCASE_TURKISH:
         change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_TURKISH);
         register_EMS_multitap_function();
         break;

      case INPUT_MODE_MULTITAP_LOWERCASE_TURKISH:
         change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_TURKISH);
         register_EMS_multitap_function();
         break;
#endif
#if defined(__MMI_MULTITAP_PORTUGUESE__)
      case INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE:
         change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE);
         register_EMS_multitap_function();
         break;

      case INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE:
         change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE);
         register_EMS_multitap_function();
         break;
#endif
#if defined(__MMI_MULTITAP_HINDI__)
      case INPUT_MODE_MULTITAP_HINDI:
         change_multitap_mode(INPUT_MODE_MULTITAP_HINDI);
         register_EMS_multitap_function();
         break;
#endif
#if defined(__MMI_MULTITAP_INDONESIAN__)
      case INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN:
         change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN);
         register_EMS_multitap_function();
         break;

      case INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN:
         change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN);
         register_EMS_multitap_function();
         break;
#endif
#if defined(__MMI_MULTITAP_MALAY__)
      case INPUT_MODE_MULTITAP_UPPERCASE_MALAY:
         change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_MALAY);
         register_EMS_multitap_function();
         break;

      case INPUT_MODE_MULTITAP_LOWERCASE_MALAY:
         change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_MALAY);
         register_EMS_multitap_function();
         break;
#endif
#if defined(__MMI_MULTITAP_VIETNAMESE__)
      case INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE:
         change_multitap_mode(INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE);
         register_EMS_multitap_function();
         break;

      case INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE:
         change_multitap_mode(INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE);
         register_EMS_multitap_function();
         break;
#endif

       
#ifdef __MMI_T9__
      case INPUT_MODE_TR_BOPOMO:
      case INPUT_MODE_SM_PINYIN:
      case INPUT_MODE_TR_STROKE:
      case INPUT_MODE_SM_STROKE:
      case INPUT_MODE_SMART_UPPERCASE_ABC:
      case INPUT_MODE_SMART_LOWERCASE_ABC:
      case INPUT_MODE_TR_MULTITAP_BOPOMO:
      case INPUT_MODE_SM_MULTITAP_PINYIN:
      case INPUT_MODE_SMART_THAI:
      case INPUT_MODE_SMART_UPPERCASE_SPANISH:
      case INPUT_MODE_SMART_LOWERCASE_SPANISH:
 
#if defined(__MMI_T9_DANISH__)
        case INPUT_MODE_SMART_UPPERCASE_DANISH:
        case INPUT_MODE_SMART_LOWERCASE_DANISH:
#endif
 
      case INPUT_MODE_SMART_UPPERCASE_FRENCH:
      case INPUT_MODE_SMART_LOWERCASE_FRENCH:
      case INPUT_MODE_SMART_UPPERCASE_GERMAN:
      case INPUT_MODE_SMART_LOWERCASE_GERMAN:
      case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
      case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
 

#if defined(__MMI_T9_VIETNAMESE__)
        case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
        case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
#endif

 

 
#if defined(__MMI_T9_INDONESIAN__)
        case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
        case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
#endif
 
      case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
      case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
 
#if defined(__MMI_T9_PORTUGUESE__)
         case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE:
         case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
#endif
 
         T9ChangeLanguageForEMSInputBox(mode);
         T9ClearKeyStarAndPoundHandler();
         T9ClearKeyNavigationHandler();
         T9ClearKey0To9Handler();
         //clear_right_softkey();
         //register_MMI_key_input_handler();
#if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
          
         if(mode==INPUT_MODE_SMART_THAI)
         {
         SetKeyHandler(handle_category28_change_input_mode,KEY_POUND,KEY_LONG_PRESS);
         SetKeyHandler(T9KeyPoundPressHandlerForEMSInputBox,KEY_POUND,KEY_EVENT_DOWN);
         }
         else
         {
         SetKeyHandler(handle_category28_change_input_mode,KEY_POUND,KEY_EVENT_DOWN);
         }
#else
         SetKeyHandler(T9KeyPoundPressHandlerForEMSInputBox,KEY_POUND,KEY_EVENT_DOWN);
          
         SetKeyHandler(wgui_EMS_inputbox_handle_symbol_picker_star_key,KEY_STAR,KEY_LONG_PRESS);
#endif
         SetKeyHandler(T9KeyStarPressHandlerForEMSInputBox,KEY_STAR,KEY_EVENT_DOWN);
         //set key arrow handler
         SetKeyHandler(T9KeyArrowUPHandlerForEMSInputBox,   KEY_UP_ARROW,   KEY_EVENT_DOWN);
         SetKeyHandler(T9KeyArrowRightHandlerForEMSInputBox,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
         SetKeyHandler(T9KeyArrowDownHandlerForEMSInputBox, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
         SetKeyHandler(T9KeyArrowLeftHandlerForEMSInputBox, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
         //register_key_down_handler(T9Key0To9HandlerForEMSInputBox);
         SetKeyHandler(T9Key0PressHandlerForEMSInputBox,KEY_0,KEY_EVENT_DOWN);
         SetKeyHandler(T9Key1PressHandlerForEMSInputBox,KEY_1,KEY_EVENT_DOWN);
         SetKeyHandler(T9Key2PressHandlerForEMSInputBox,KEY_2,KEY_EVENT_DOWN);
         SetKeyHandler(T9Key3PressHandlerForEMSInputBox,KEY_3,KEY_EVENT_DOWN);
         SetKeyHandler(T9Key4PressHandlerForEMSInputBox,KEY_4,KEY_EVENT_DOWN);
         SetKeyHandler(T9Key5PressHandlerForEMSInputBox,KEY_5,KEY_EVENT_DOWN);
         SetKeyHandler(T9Key6PressHandlerForEMSInputBox,KEY_6,KEY_EVENT_DOWN);
         SetKeyHandler(T9Key7PressHandlerForEMSInputBox,KEY_7,KEY_EVENT_DOWN);
         SetKeyHandler(T9Key8PressHandlerForEMSInputBox,KEY_8,KEY_EVENT_DOWN);
         SetKeyHandler(T9Key9PressHandlerForEMSInputBox,KEY_9,KEY_EVENT_DOWN);
         wgui_set_EMS_inputbox_RSK();
         set_right_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
         set_right_softkey_function(UI_dummy_function, KEY_EVENT_LONG_PRESS);
#if !defined(__MMI_TOUCH_SCREEN__)
         T9SynT9WithInputBoxForEMSInputBox();
         T9ScreenStateChnageForEMSInputBox();
         T9InputBoxDisplayForEMSInputBox();
#else
         register_EMS_inputbox_keys();
         wgui_set_EMS_inputbox_RSK();
#endif

#elif defined __MMI_ZI__
      case INPUT_MODE_TR_BOPOMO:
      case INPUT_MODE_SM_PINYIN:
      case INPUT_MODE_TR_STROKE:
      case INPUT_MODE_SM_STROKE:
      case INPUT_MODE_SMART_UPPERCASE_ABC:
      case INPUT_MODE_SMART_LOWERCASE_ABC:
      case INPUT_MODE_TR_MULTITAP_BOPOMO:
      case INPUT_MODE_SM_MULTITAP_PINYIN:
      case INPUT_MODE_SMART_THAI:
       
      case INPUT_MODE_SMART_ARABIC:
      //CSD end
      case INPUT_MODE_SMART_UPPERCASE_SPANISH:
      case INPUT_MODE_SMART_LOWERCASE_SPANISH:
 
#if defined(__MMI_T9_DANISH__)||defined(__MMI_ZI_DANISH__) 
        case INPUT_MODE_SMART_UPPERCASE_DANISH:
        case INPUT_MODE_SMART_LOWERCASE_DANISH:
#endif
 
      case INPUT_MODE_SMART_UPPERCASE_FRENCH:
      case INPUT_MODE_SMART_LOWERCASE_FRENCH:
      case INPUT_MODE_SMART_UPPERCASE_GERMAN:
      case INPUT_MODE_SMART_LOWERCASE_GERMAN:
      case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
      case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
      case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
      case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
      case INPUT_MODE_SMART_UPPERCASE_TURKISH:
      case INPUT_MODE_SMART_LOWERCASE_TURKISH:
      case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE:
      case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
      case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
      case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
      case INPUT_MODE_SMART_UPPERCASE_MALAY:
      case INPUT_MODE_SMART_LOWERCASE_MALAY:
      case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
      case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
         ZiChangeInputMethod((INPUT_MODE)mode);
         ZiClearKeyStarAndPoundHandler();
         ZiClearKeyNavigationHandler();
         ZiClearKey0To9Handler();
         //set key arrow handler
         SetKeyHandler(ZiKeyArrowUPHandler,   KEY_UP_ARROW,   KEY_EVENT_DOWN);
         SetKeyHandler(ZiKeyArrowRightHandler,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
         SetKeyHandler(ZiKeyArrowDownHandler, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
         SetKeyHandler(ZiKeyArrowLeftHandler, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
         SetKeyHandler(ZiKey0PressHandler,KEY_0,KEY_EVENT_DOWN);
         SetKeyHandler(ZiKey1PressHandler,KEY_1,KEY_EVENT_DOWN);
         SetKeyHandler(ZiKey2PressHandler,KEY_2,KEY_EVENT_DOWN);
         SetKeyHandler(ZiKey3PressHandler,KEY_3,KEY_EVENT_DOWN);
         SetKeyHandler(ZiKey4PressHandler,KEY_4,KEY_EVENT_DOWN);
         SetKeyHandler(ZiKey5PressHandler,KEY_5,KEY_EVENT_DOWN);
         SetKeyHandler(ZiKey6PressHandler,KEY_6,KEY_EVENT_DOWN);
         SetKeyHandler(ZiKey7PressHandler,KEY_7,KEY_EVENT_DOWN);
         SetKeyHandler(ZiKey8PressHandler,KEY_8,KEY_EVENT_DOWN);
         SetKeyHandler(ZiKey9PressHandler,KEY_9,KEY_EVENT_DOWN);
          
         handle_category28_input();
         // register the star and hash key handkers
         SetKeyHandler(EntrySymbolPickerScreen,KEY_STAR,KEY_EVENT_DOWN);
         SetKeyHandler(handle_category28_change_input_mode,KEY_POUND,KEY_EVENT_DOWN);
#if !defined(__MMI_TOUCH_SCREEN__)
         ZiDisplay();
#else
         register_EMS_inputbox_keys();
         wgui_set_EMS_inputbox_RSK();
#endif

#elif defined __MMI_KA__
      case INPUT_MODE_SM_PINYIN:
      case INPUT_MODE_SM_STROKE:
         KonkaChangeInputMethod(mode);
         KonkaClearKeyStarAndPoundHandler();
         KonkaClearKeyNavigationHandler();
         KonkaClearKey0To9Handler();
         clear_right_softkey();
         clear_left_softkey();
         //set key arrow handler
         SetKeyHandler(KonkaKeyArrowUPHandler,   KEY_UP_ARROW,   KEY_EVENT_DOWN);
         SetKeyHandler(KonkaKeyArrowRightHandler,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
         SetKeyHandler(KonkaKeyArrowDownHandler, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
         SetKeyHandler(KonkaKeyArrowLeftHandler, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
         //register_key_down_handler(T9Key0To9HandlerForEMSInputBox);
         SetKeyHandler(KonkaKey0PressHandler,KEY_0,KEY_EVENT_DOWN);
         SetKeyHandler(KonkaKey1PressHandler,KEY_1,KEY_EVENT_DOWN);
         SetKeyHandler(KonkaKey2PressHandler,KEY_2,KEY_EVENT_DOWN);
         SetKeyHandler(KonkaKey3PressHandler,KEY_3,KEY_EVENT_DOWN);
         SetKeyHandler(KonkaKey4PressHandler,KEY_4,KEY_EVENT_DOWN);
         SetKeyHandler(KonkaKey5PressHandler,KEY_5,KEY_EVENT_DOWN);
         SetKeyHandler(KonkaKey6PressHandler,KEY_6,KEY_EVENT_DOWN);
         SetKeyHandler(KonkaKey7PressHandler,KEY_7,KEY_EVENT_DOWN);
         SetKeyHandler(KonkaKey8PressHandler,KEY_8,KEY_EVENT_DOWN);
         SetKeyHandler(KonkaKey9PressHandler,KEY_9,KEY_EVENT_DOWN);
         // register the star and hash key handkers
         SetKeyHandler(EntrySymbolPickerScreen,KEY_STAR,KEY_EVENT_DOWN);
         SetKeyHandler(handle_category28_change_input_mode,KEY_POUND,KEY_EVENT_DOWN);
         // register the L and R softkey handlers
         set_right_softkey_function(KonkaKeyRSKPressHandler, KEY_EVENT_DOWN);
         set_right_softkey_function(KonkaKeyRSKLongPressHandler, KEY_EVENT_LONG_PRESS);
         set_left_softkey_function(KonkaKeyLSKPressHandler, KEY_EVENT_UP);
         set_left_softkey_function(KonkaKeyLSKLongPressHandler, KEY_EVENT_LONG_PRESS);
         KonkaDisplay();

#elif defined __MMI_ITAP__
      case INPUT_MODE_TR_BOPOMO:
      case INPUT_MODE_SM_PINYIN:
      case INPUT_MODE_TR_STROKE:
      case INPUT_MODE_SM_STROKE:
      case INPUT_MODE_SMART_UPPERCASE_ABC:
      case INPUT_MODE_SMART_LOWERCASE_ABC:
      case INPUT_MODE_TR_MULTITAP_BOPOMO:
      case INPUT_MODE_SM_MULTITAP_PINYIN:
         ItapChangeInputMethod(mode);
         ItapClearKeyStarAndPoundHandler();
         ItapClearKeyNavigationHandler();
         ItapClearKey0To9Handler();
         //set key arrow handler
         SetKeyHandler(ItapKeyArrowUPHandler,   KEY_UP_ARROW,   KEY_EVENT_DOWN);
         SetKeyHandler(ItapKeyArrowRightHandler,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
         SetKeyHandler(ItapKeyArrowDownHandler, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
         SetKeyHandler(ItapKeyArrowLeftHandler, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
         SetKeyHandler(ItapKey0PressHandler,KEY_0,KEY_EVENT_DOWN);
         SetKeyHandler(ItapKey1PressHandler,KEY_1,KEY_EVENT_DOWN);
         SetKeyHandler(ItapKey2PressHandler,KEY_2,KEY_EVENT_DOWN);
         SetKeyHandler(ItapKey3PressHandler,KEY_3,KEY_EVENT_DOWN);
         SetKeyHandler(ItapKey4PressHandler,KEY_4,KEY_EVENT_DOWN);
         SetKeyHandler(ItapKey5PressHandler,KEY_5,KEY_EVENT_DOWN);
         SetKeyHandler(ItapKey6PressHandler,KEY_6,KEY_EVENT_DOWN);
         SetKeyHandler(ItapKey7PressHandler,KEY_7,KEY_EVENT_DOWN);
         SetKeyHandler(ItapKey8PressHandler,KEY_8,KEY_EVENT_DOWN);
         SetKeyHandler(ItapKey9PressHandler,KEY_9,KEY_EVENT_DOWN);
         // register the star and hash key handkers
         SetKeyHandler(EntrySymbolPickerScreen,KEY_STAR,KEY_EVENT_DOWN);
         SetKeyHandler(handle_category28_change_input_mode,KEY_POUND,KEY_EVENT_DOWN);
         ItapDisplay();
#endif
//tangjie add begin 20060718
     #if defined(__MMI_GB__)
#if defined(__MMI_GB_SM_CHINESE__)
     case INPUT_MODE_SM_STROKE:
	 	InitGbSMInputMethod();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
/*Guojian Del Start For 9682 Ver: GreenStone  on 2008-9-19 17:26 */
		#if 0
		GBSetInputboxLSKFunction(mmi_msg_entry_option_write);                   //add by yaosq 20060901
		#endif
/*Guojian Del End  For 9682 Ver: GreenStone  on 2008-9-19 17:26 */
		GbInputSetRSK();
		GbDisplayForEmsEditor();
		break;
		
     case INPUT_MODE_SM_PINYIN:
	 	TRACE_EF(1, "INPUT_MODE_SM_PINYIN");
		GbIMEEnShutdown();
		InitGbSMInputMethod();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
//		GBSetInputboxLSKFunction(mmi_msg_entry_option_write);     //qiff 2008/10/20 del for greenstone bug:10039                //add by yaosq 20060901
		GbInputSetRSK();
		GbDisplayForEmsEditor();
		break;
#endif
#if defined(__MMI_GB_TR_CHINESE__)
     case INPUT_MODE_TR_BOPOMO:
	 	TRACE_EF(1, "INPUT_MODE_TR_BOPOMO");
		GbIMEEnShutdown();
		InitGbTRInputMethod();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
		GbInputSetRSK();
		break;
#endif

     case INPUT_MODE_SMART_LOWERCASE_ABC:
     case INPUT_MODE_SMART_UPPERCASE_ABC:
	 	GbIMEEnShutdown();
		InitGbInputMethod();
		GbChangeInputMethod(mode);
		GbRegisterKeyHandler(mode);
//		GBSetInputboxLSKFunction(mmi_msg_entry_option_write);         //qiff 2008/10/20 del for greenstone bug:10039          //add by yaosq 20060901
		GbInputSetRSK();
		GbDisplayForEmsEditor();

     #endif
      //tangjie add end 20060718
         break;
   }

#if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
#else
   if (wgui_EMS_inputbox_handle_symbol_picker_star_key==currKeyFuncPtrs[KEY_STAR][KEY_EVENT_DOWN])
   {
      ClearKeyHandler(KEY_STAR,KEY_EVENT_DOWN);
		 
      SetKeyHandler(wgui_EMS_inputbox_handle_symbol_picker_star_key,KEY_STAR,KEY_LONG_PRESS);
   }
   SetKeyHandler(handle_category28_change_input_mode,KEY_POUND,KEY_REPEAT);

#endif
}

void change_EMS_inputbox_input_mode(void)
{
   change_EMS_inputbox_mode(MMI_current_input_mode);
}

void register_EMS_inputbox_viewer_keys(void)
{
#ifdef __MMI_MESSAGES_EMS__
	SetKeyHandler(EMS_inputbox_previous_object,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EMS_inputbox_next_object,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#else
	SetKeyHandler(EMS_inputbox_previous,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EMS_inputbox_next,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif
	SetKeyHandler(EMS_inputbox_previous_line,KEY_UP_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EMS_inputbox_next_line,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EMS_inputbox_previous_line,KEY_VOL_UP,KEY_EVENT_DOWN);
	SetKeyHandler(EMS_inputbox_next_line,KEY_VOL_DOWN,KEY_EVENT_DOWN);
	register_keyboard_key_handler(EMS_inputbox_viewer_key_handler);
}

void reset_EMS_inputbox(void)
{
#if(UI_BLINKING_CURSOR_SUPPORT)
	//coolsand_UI_cancel_timer(UI_inputbox_blink_cursor);
	 
	StopMyTimer(BLINKING_CURSOR);
        //CSD end

#endif
#ifdef __MMI_MESSAGES_EMS__
	EMS_cancel_object_focus();
#endif
	reset_multitaps();
	clear_keyboard_key_handler();
	clear_keyboard_input_handler();
	clear_multitap_key_handlers();
	ClearKeyHandler(KEY_CLEAR,KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_CLEAR,KEY_EVENT_UP);
	ClearKeyHandler(KEY_CLEAR,KEY_EVENT_LONG_PRESS);
#if((UI_ENABLE_FRAME_SKIPPING)&&(ENABLE_EMS_INPUTBOX_FRAME_SKIPPING))
	coolsand_UI_cancel_timer(wgui_EMS_inputbox_end_frame);
	wgui_EMS_inputbox_frame_counter=0;
#endif
	EMS_inputbox_input_callback=UI_dummy_function;
}

byte test_EMS_inputbox_change_mode(void)
{
	//modified by zhoumn@2007/07/31<<<start
	if (MMI_EMS_inputbox.UCS2_count == 0)
	{
		if (EMSSetDCS(MMI_EMS_inputbox.data,SMSAL_UCS2_DCS) == EMS_OK &&
			MMI_EMS_inputbox.data->textLength < EMSEditor_Chinese_Length)
		{
			EMSSetDCS(MMI_EMS_inputbox.data,SMSAL_DEFAULT_DCS);
			return 1;
		}
		else
		{
			EMSSetDCS(MMI_EMS_inputbox.data,SMSAL_DEFAULT_DCS);
			return 0;
		}
	}
	else
	{
		return 1;
	}
	//modified by zhoumn@2007/07/31>>>end
}

/*	Temporary data: for testing			*/
#ifdef __MMI_MESSAGES_EMS__

const U8	EMS_sample_melody_object_PDU_1[]=
{
#if(0)
	EMS_PREDEFINED_MELODY, 0x69, 0x00,
	'B','E','G','I','N',':','I','M','E','L','O','D','Y','\n',
	'V','E','R','S','I','O','N',':','1','.','2','\n',
	'F','O','R','M','A','T',':','C','L','A','S','S','1','.','0','\n',
	'M','E','L','O','D','Y',':','*','4','#','f','2','*','4','a','3','.','*','4','#','f','3','*','4','#','f','4','*','4','#','a','3','*','4','#','f','3','*','4','e','3','*','4','#','f','2','*','5','c','3','\n',
	'E','N','D',':','I','M','E','L','O','D','Y','\n',
#else
	EMS_PREDEFINED_MELODY, 0x74, 0x00,
	'B','E','G','I','N',':','I','M','E','L','O','D','Y','\n',
	'V','E','R','S','I','O','N',':','1','.','2','\n',
	'F','O','R','M','A','T',':','C','L','A','S','S','1','.','0','\n',
	'B','E','A','T',':','1','2','0','\n',
	'C','O','M','P','O','S','E','R',':','M','T','K','(','2',')','\n',
	'M','E','L','O','D','Y',':','c','2','d','2','e','2','f','2','g','2','a','2','b','2','\n',
	'E','N','D',':','I','M','E','L','O','D','Y','\n',
#endif
};

const U8	EMS_sample_melody_object_PDU_2[]=
{	EMS_PREDEFINED_MELODY, 0x6c, 0x00,
	'B','E','G','I','N',':','I','M','E','L','O','D','Y','\n',
	'V','E','R','S','I','O','N',':','1','.','2','\n',
	'F','O','R','M','A','T',':','C','L','A','S','S','1','.','0','\n',
	'M','E','L','O','D','Y',':','*','5','f','0','*','5','#','g','2','*','5','g','3','*','5','#','d','0','*','5','#','g','2','*','5','g','3','*','5','c','0','*','5','#','g','2','*','5','g','3','*','4','#','g','0','\n',
	'E','N','D',':','I','M','E','L','O','D','Y','\n',
};

const U8	EMS_sample_picture_object_PDU_1[]=
{	EMS_USERDEFINED_PICTURE, 0x01, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00,
	0x07, 0xE0, 0x08, 0x10, 0x30, 0x0C, 0x20, 0x04, 0x4E, 0x72, 0x9A, 0x59, 0x86, 0x61, 0x80, 0x01,
	0x80, 0x01, 0x90, 0x09, 0x88, 0x11, 0x47, 0xE2, 0x23, 0xC4, 0x30, 0x0C, 0x08, 0x10, 0x07, 0xE0,
};

#if(!ENABLE_EMS_TEST_OBJECTS)

const U8	EMS_sample_picture_object_PDU_2[]=
{	EMS_PREDEFINED_PICTURE, 0x01, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x03, 0xA0, 0x05, 0x90, 0x09, 0x88, 0x11, 0x8C, 0x31,
	0x93, 0xC9, 0xA0, 0x05, 0xC0, 0x03, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#else

const U8	EMS_sample_picture_object_PDU_2[]=
{	EMS_PREDEFINED_MELODY, 0x6a, 0x00,
	'B','E','G','I','N',':','I','M','E','L','O','D','Y','\n',
	'V','E','R','S','I','O','N',':','1','.','2','\n',
	'F','O','R','M','A','T',':','C','L','A','S','S','1','.','0','\n',
	'M','E','L','O','D','Y',':','*','4','#','f','2','*','4','a','3','.','*','4','#','f','3','*','4','#','f','4','*','4','#','a','3','*','4','#','f','3','*','4','e','3','*','4','#','f','2','*','5','c','3','\n',
	'E','N','D',':','I','M','E','L','O','D','Y','\n',
};

#endif

#if(!ENABLE_EMS_TEST_OBJECTS)

const U8	EMS_sample_picture_object_PDU_3[]=
{	EMS_PREDEFINED_PICTURE, 0x01, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00,
	0x01, 0x80, 0x03, 0xC0, 0x03, 0xC0, 0x07, 0xE0, 0x0F, 0xF0, 0x1F, 0xF8, 0x3F, 0xFC, 0x3F, 0xFC,
	0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x3D, 0xBC, 0x19, 0x98, 0x01, 0x80, 0x01, 0x80, 0x07, 0xE0,
};

#else

const U8	EMS_sample_picture_object_PDU_3[]=
{	EMS_PREDEFINED_ANIMATION, 0x01,
};

#endif

#if(!ENABLE_EMS_TEST_OBJECTS)

const U8	EMS_sample_picture_object_PDU_4[]=
{	EMS_PREDEFINED_PICTURE, 0x01, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00,
	0x07, 0xE0, 0x08, 0x10, 0x30, 0x0C, 0x20, 0x04, 0x5C, 0x3A, 0x86, 0x61, 0x86, 0x61, 0x80, 0x01,
	0x80, 0x01, 0x81, 0xC1, 0x82, 0x21, 0x44, 0x12, 0x20, 0x04, 0x30, 0x0C, 0x08, 0x10, 0x07, 0xE0,
};

#else

const U8	EMS_sample_picture_object_PDU_4[]=
{	EMS_PREDEFINED_MELODY, 0x6c, 0x00,
	'B','E','G','I','N',':','I','M','E','L','O','D','Y','\n',
	'V','E','R','S','I','O','N',':','1','.','2','\n',
	'F','O','R','M','A','T',':','C','L','A','S','S','1','.','0','\n',
	'M','E','L','O','D','Y',':','*','5','f','0','*','5','#','g','2','*','5','g','3','*','5','#','d','0','*','5','#','g','2','*','5','g','3','*','5','c','0','*','5','#','g','2','*','5','g','3','*','4','#','g','0','\n',
	'E','N','D',':','I','M','E','L','O','D','Y','\n',
};

#endif

const U8	EMS_sample_picture_object_PDU_5[]=
{	EMS_PREDEFINED_PICTURE, 0x01, 0x18, 0x00, 0x18, 0x00, 0x48, 0x00,
	0x05, 0x80, 0x00, 0x10, 0x60, 0x00, 0x42, 0x1D, 0xC0, 0x80, 0x06, 0x30, 0x00, 0x29, 0x8C, 0x98,
	0x02, 0xA2, 0x9E, 0x00, 0xB5, 0x9F, 0x82, 0x77, 0x1F, 0xE1, 0x5B, 0x9F, 0xF9, 0x75, 0x9F, 0xF9,
	0x5B, 0x1F, 0xF9, 0x75, 0x9F, 0xF9, 0x5B, 0x9F, 0xF9, 0x77, 0x1F, 0xF9, 0x5B, 0xC7, 0xF9, 0x75,
	0xA1, 0xF9, 0xBA, 0x38, 0x78, 0xDC, 0x0E, 0x1A, 0xB8, 0x03, 0x81, 0x70, 0x00, 0xF1, 0x50, 0x00,
	0x3D, 0x70, 0x00, 0x0E, 0xE0, 0x00, 0x03, 0x80,
};

#if(!ENABLE_EMS_TEST_OBJECTS)

const U8	EMS_sample_picture_object_PDU_6[]=
{	EMS_PREDEFINED_PICTURE, 0x01, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00,
	0x07, 0xE0, 0x08, 0x10, 0x30, 0x0C, 0x26, 0x64, 0x48, 0x12, 0x90, 0x09, 0x86, 0x61, 0x86, 0x61,
	0x80, 0x01, 0x83, 0xC1, 0x87, 0xE1, 0x47, 0xE2, 0x23, 0xC4, 0x30, 0x0C, 0x08, 0x10, 0x07, 0xE0,
};

#else

const U8	EMS_sample_picture_object_PDU_6[]=
{	EMS_PREDEFINED_SOUND, 0x01,
};

#endif

#if(!ENABLE_EMS_TEST_OBJECTS)

const U8	EMS_sample_picture_object_PDU_7[]=
{	EMS_PREDEFINED_PICTURE, 0x01, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00,
	0x07, 0xE0, 0x08, 0x10, 0x30, 0x0C, 0x2C, 0x64, 0x52, 0x92, 0x96, 0xB1, 0x8C, 0x61, 0x80, 0x01,
	0xBF, 0xFD, 0xA9, 0x25, 0x99, 0x29, 0x59, 0x2A, 0x29, 0x34, 0x37, 0xEC, 0x08, 0x10, 0x07, 0xE0,
};

#else

const U8	EMS_sample_picture_object_PDU_7[]=
{	EMS_PREDEFINED_ANIMATION, 0x02,
};

#endif

#if(!ENABLE_EMS_TEST_OBJECTS)

const U8	EMS_sample_picture_object_PDU_8[]=
{	0x01, 0x01, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00,
	0x03, 0xC0, 0x07, 0xE0, 0x07, 0xE0, 0x07, 0xE0, 0x03, 0xC0, 0x01, 0x80, 0x19, 0x98, 0x3D, 0xBC,
	0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x3D, 0xBC, 0x19, 0x98, 0x01, 0x80, 0x01, 0x80, 0x07, 0xE0,
};

#else

const U8	EMS_sample_picture_object_PDU_8[]=
{	EMS_USERDEFINED_ANIMATION, 0x01, 0x08, 0x00, 0x08, 0x00, 0x20, 0x00,
	0xFF, 0x81, 0x81, 0x81, 0xF1, 0xD1, 0xF1, 0xFF,

	0xFF, 0x81, 0x81, 0x81, 0x8F, 0x8B, 0x8F, 0xFF,

	0xFF, 0x8F, 0x8B, 0x8F, 0x81, 0x81, 0x81, 0xFF,

	0xFF, 0xF1, 0xD1, 0xF1, 0x81, 0x81, 0x81, 0xFF,
};

#endif

#if(!ENABLE_EMS_TEST_OBJECTS)

const U8	EMS_sample_picture_object_PDU_9[]=
{	0x01, 0x01, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00,
	0x07, 0xE0, 0x08, 0x10, 0x30, 0x0C, 0x20, 0x04, 0x79, 0xCE, 0xBF, 0xF9, 0xB9, 0xC1, 0x80, 0x01,
	0x80, 0x01, 0x9F, 0x01, 0x9F, 0x81, 0x4F, 0xC2, 0x27, 0xC4, 0x30, 0x0C, 0x08, 0x10, 0x07, 0xE0,
};

#else

const U8	EMS_sample_picture_object_PDU_9[]=
{	EMS_USERDEFINED_ANIMATION, 0x01, 0x10, 0x00, 0x10, 0x00, 0x80, 0x00,
	0x0F, 0xF8, 0x0F, 0xF8, 0x04, 0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0x10, 0x02, 0x20, 0x01, 0x40,
	0x01, 0x40, 0x02, 0x20, 0x04, 0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0x10, 0x0F, 0xF8, 0x0F, 0xF8,

	0x06, 0x00, 0x0E, 0x00, 0x1F, 0x00, 0x38, 0x80, 0x70, 0x40, 0xE0, 0x40, 0x60, 0x40, 0x10, 0x40,
	0x0F, 0x3C, 0x00, 0x82, 0x00, 0x81, 0x00, 0x83, 0x00, 0x87, 0x00, 0x4E, 0x00, 0x3C, 0x00, 0x38,

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0xFC, 0x3F, 0xC2, 0x43, 0xC1, 0x83, 0xC0, 0x03,
	0xC1, 0x83, 0xC2, 0x43, 0xFC, 0x3F, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	0x00, 0x38, 0x00, 0x3C, 0x00, 0x4E, 0x00, 0x87, 0x00, 0x83, 0x00, 0x81, 0x00, 0x82, 0x0F, 0x3C,
	0x10, 0x40, 0x60, 0x40, 0xE0, 0x40, 0x70, 0x40, 0x38, 0x80, 0x1F, 0x00, 0x0E, 0x00, 0x06, 0x00,
};

#endif

const U8	EMS_sample_picture_object_PDU_10[]=
{	EMS_PREDEFINED_PICTURE, 0x01, 0x20, 0x00, 0x20, 0x00, 0x80, 0x00,
	0x00, 0xFE, 0x00, 0x00, 0x07, 0xFF, 0x80, 0x00, 0x3F, 0xFF, 0x70, 0x00, 0x7F, 0xFF, 0x8E, 0x00,
	0x47, 0xFF, 0xC1, 0xC0, 0x40, 0xFF, 0xE0, 0x38, 0x40, 0x1F, 0xF0, 0x06, 0x40, 0x03, 0xF8, 0x02,
	0x40, 0x00, 0x7C, 0x0E, 0x40, 0x00, 0x0E, 0x3E, 0x40, 0x00, 0x00, 0xFE, 0x20, 0x78, 0x03, 0xFC,
	0x22, 0x1D, 0x03, 0xFC, 0x27, 0x0F, 0x03, 0xFC, 0x2F, 0x87, 0x03, 0xFC, 0x23, 0xCF, 0x07, 0xF8,
	0x23, 0x03, 0x07, 0xF8, 0x13, 0x00, 0x07, 0xF8, 0x11, 0x00, 0x07, 0xF8, 0x11, 0x08, 0x8F, 0xF0,
	0x10, 0x88, 0x8F, 0xF0, 0x10, 0x59, 0x8F, 0xF0, 0x08, 0x3F, 0x8F, 0xE0, 0x08, 0x1F, 0x1F, 0xE0,
	0x08, 0x0C, 0x1F, 0xE0, 0x04, 0x02, 0x1F, 0xE0, 0x03, 0x00, 0x1F, 0xC0, 0x00, 0xC0, 0x1F, 0xC0,
	0x00, 0x30, 0x3F, 0xC0, 0x00, 0x0C, 0x3F, 0x00, 0x00, 0x03, 0x3C, 0x00, 0x00, 0x00, 0xF0, 0x00,
};

const U8	EMS_sample_picture_object_PDU_11[]=
{	0x01, 0x01, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00,
	0x00, 0x00, 0x1F, 0xF8, 0x20, 0x06, 0x26, 0x66, 0x23, 0xC6, 0x21, 0x86, 0x23, 0xC6, 0x26, 0x66,
	0x20, 0x06, 0x20, 0x06, 0x2A, 0xAE, 0x20, 0x06, 0x35, 0x56, 0x2A, 0xAE, 0x3F, 0xFE, 0x1F, 0xFC,
};

const U8	EMS_sample_animation_object_PDU_11[]=
{	EMS_USERDEFINED_ANIMATION, 0x01, 0x08, 0x00, 0x08, 0x00, 0x20, 0x00,
	0xFF, 0x81, 0x81, 0x81, 0xF1, 0xD1, 0xF1, 0xFF,

	0xFF, 0x81, 0x81, 0x81, 0x8F, 0x8B, 0x8F, 0xFF,

	0xFF, 0x8F, 0x8B, 0x8F, 0x81, 0x81, 0x81, 0xFF,

	0xFF, 0xF1, 0xD1, 0xF1, 0x81, 0x81, 0x81, 0xFF,
};

#define MAX_EMS_PREDEFINED_PICTURES		10

#if(ENABLE_EMS_TEST_OBJECTS)

/*	Temporary: Needs to populated with actual picture data	*/
U8* EMS_predefined_pictures[MAX_EMS_PREDEFINED_PICTURES]=
{	(U8*)EMS_sample_picture_object_PDU_1,
	(U8*)EMS_sample_picture_object_PDU_2,
	(U8*)EMS_sample_picture_object_PDU_3,
	(U8*)EMS_sample_picture_object_PDU_4,
	(U8*)EMS_sample_picture_object_PDU_5,
	(U8*)EMS_sample_picture_object_PDU_6,
	(U8*)EMS_sample_picture_object_PDU_7,
	(U8*)EMS_sample_picture_object_PDU_8,
	(U8*)EMS_sample_picture_object_PDU_9,
	(U8*)EMS_sample_picture_object_PDU_10,
};

#else

/*	This gets populated at run time with picture data		*/
U8* EMS_predefined_pictures[MAX_EMS_PREDEFINED_PICTURES]=
{	(U8*)EMS_sample_picture_object_PDU_11,
	(U8*)EMS_sample_picture_object_PDU_11,
	(U8*)EMS_sample_picture_object_PDU_11,
	(U8*)EMS_sample_picture_object_PDU_11,
	(U8*)EMS_sample_picture_object_PDU_11,
	(U8*)EMS_sample_picture_object_PDU_11,
	(U8*)EMS_sample_picture_object_PDU_11,
	(U8*)EMS_sample_picture_object_PDU_11,
	(U8*)EMS_sample_picture_object_PDU_11,
	(U8*)EMS_sample_picture_object_PDU_11,
};

#endif

U16 EMS_predefined_images[MAX_EMS_PREDEFINED_PICTURES]=
{	IMG_EMS01_PIC,
	IMG_EMS02_PIC,
	IMG_EMS03_PIC,
	IMG_EMS04_PIC,
	IMG_EMS05_PIC,
	IMG_EMS06_PIC,
	IMG_EMS07_PIC,
	IMG_EMS08_PIC,
	IMG_EMS09_PIC,
	IMG_EMS010_PIC
};



#define MAX_EMS_PREDEFINED_ANIMATIONS	15

U16	EMS_predefined_animations[MAX_EMS_PREDEFINED_ANIMATIONS]=
{	EMS_BI_ANIMATION_0_ID,
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
};

#define MAX_EMS_PREDEFINED_SOUNDS		10

/*	Temporary: Needs to be populated with the actual sound IDs	*/
U16 EMS_predefined_sounds[MAX_EMS_PREDEFINED_SOUNDS]=
{
	EMS_CHIMES_HIGH,
	EMS_CHIMES_LOW,
	EMS_DING,
	EMS_TADA,
	EMS_NOTIFY,
	EMS_DRUM,
	EMS_CLAPS,
	EMS_FANFAR,
	EMS_CHORE_HGH,
	EMS_CHORE_LOW,
};

#define MAX_EMS_PREDEFINED_MELODIES		10

/*	Temporary: Needs to be populated with the actual melody data	*/
U8* EMS_predefined_melodies[MAX_EMS_PREDEFINED_MELODIES]=
{
	(U8*)EMS_sample_melody_object_PDU_1,
	(U8*)EMS_sample_melody_object_PDU_2,
	(U8*)EMS_sample_melody_object_PDU_1,
	(U8*)EMS_sample_melody_object_PDU_2,
	(U8*)EMS_sample_melody_object_PDU_1,
	(U8*)EMS_sample_melody_object_PDU_2,
	(U8*)EMS_sample_melody_object_PDU_1,
	(U8*)EMS_sample_melody_object_PDU_2,
	(U8*)EMS_sample_melody_object_PDU_1,
	(U8*)EMS_sample_melody_object_PDU_2,
};

/*	data used for EMS_USERDEFINED_PICTURE, EMS_USERDEFINED_ANIMATION, EMS_USER_DEFINED_MELODY
	index used for EMS_PREDEFINED_PICTURE, EMS_PREDEFINED_MELODY.
	If index==-1, data is used for EMS_PREDEFINED_PICTURE, EMS_PREDEFINED_MELODY.	*/

EMSTATUS EMS_load_object(dword type,U8* data,S32 index,EMSObjData *object)
{	U8 ID;

	switch(type & 0xff)
	{	case EMS_PREDEFINED_PICTURE:
		{	if((index>=MAX_EMS_PREDEFINED_PICTURES)) return(EMS_INVALID_OBJECT);
			if(index>=0) data=EMS_predefined_pictures[index];
			if(data==NULL) return(EMS_NULL_POINTER);
			ID=data[0];
			if(ID==EMS_PREDEFINED_PICTURE)
			{	object->picture.bitsPerPixel=data[1];
				object->picture.attribute=data[2];
				object->picture.hDim=data[3]|(data[4]<<8);
				object->picture.vDim=data[5]|(data[6]<<8);
				object->picture.pdu_length=data[7]|(data[8]<<8);
				data+=9;
				object->picture.pdu=data;
			#ifdef __EMS_REL5__
				if((object->picture.hDim==0)||(object->picture.vDim==0)
				||(object->picture.pdu_length==0)
				||(object->picture.attribute==1))
			#else
				if((object->picture.hDim==0)||(object->picture.vDim==0)
				||(object->picture.pdu_length==0)||(object->picture.pdu_length>EMS_MAX_IMELODY_SIZE))
			#endif
					return(EMS_INVALID_OBJECT);
			}
			else return(EMS_INVALID_OBJECT);
		}
		break;

		case EMS_USERDEFINED_PICTURE:
		{	if(data==NULL) return(EMS_NULL_POINTER);
			ID=data[0];
			if(ID==EMS_USERDEFINED_PICTURE)
			{	object->picture.bitsPerPixel=data[1];
				object->picture.attribute=data[2];
				object->picture.hDim=data[3]|(data[4]<<8);
				object->picture.vDim=data[5]|(data[6]<<8);
				object->picture.pdu_length=data[7]|(data[8]<<8);
				data+=9;
				object->picture.pdu=data;
			#ifdef __EMS_REL5__
				if((object->picture.hDim==0)||(object->picture.vDim==0)
				||(object->picture.pdu_length==0)
				||(object->picture.attribute==1))
			#else
				if((object->picture.hDim==0)||(object->picture.vDim==0)
				||(object->picture.pdu_length==0)||(object->picture.pdu_length>EMS_MAX_IMELODY_SIZE))
			#endif
					return(EMS_INVALID_OBJECT);
			}
			else return(EMS_INVALID_OBJECT);
		}
		break;

		case EMS_USERDEFINED_ANIMATION:
		{	if(data==NULL) return(EMS_NULL_POINTER);
			ID=data[0];
			if(ID==EMS_USERDEFINED_ANIMATION)
			{	object->animation.bitsPerPixel=data[1];
				object->animation.attribute=data[2];
				object->animation.cFrame=data[3];
				object->animation.cRepeat=data[4];
				object->animation.Duration=data[5];
				object->animation.hDim=data[6]|(data[7]<<8);
				object->animation.vDim=data[8]|(data[9]<<8);
				object->animation.pdu_length=data[10]|(data[11]<<8);
				data+=12;
				object->animation.pdu=data;
			#ifdef __EMS_REL5__
				if((object->animation.hDim==0)||(object->animation.vDim==0)
				||(object->animation.pdu_length==0)
				||(object->animation.attribute==1)||(object->animation.cFrame==0))
			#else
				if((object->animation.hDim==0)||(object->animation.vDim==0)
				||(object->animation.pdu_length==0)||(object->animation.pdu_length>EMS_MAX_IMELODY_SIZE))
			#endif
					return(EMS_INVALID_OBJECT);
			}
			else return(EMS_INVALID_OBJECT);
		}
		break;
	//CSD Joy added for customization, 1014
	#if defined(__MMI_IMELODY_SUPPORT__)
		case EMS_PREDEFINED_MELODY:
		{	if((index>=MAX_EMS_PREDEFINED_MELODIES)) return(EMS_INVALID_OBJECT);
			if(index>=0) data=EMS_predefined_melodies[index];
			if(data==NULL) return(EMS_NULL_POINTER);
			ID=data[0];
			if(ID==EMS_PREDEFINED_MELODY)
			{	object->sound.attribute=data[1];
				object->sound.pdu_length=data[2]|(data[3]<<8);
				data+=4;
				object->sound.pdu=data;
			#ifdef __EMS_REL5__
				if((object->sound.pdu_length==0) || (object->sound.attribute==1))
			#else
				if((object->sound.pdu_length==0)||(object->sound.pdu_length>EMS_MAX_IMELODY_SIZE))
			#endif
					return(EMS_INVALID_OBJECT);
			}
			else return(EMS_INVALID_OBJECT);
		}
		break;

		case EMS_USERDEFINED_MELODY:
		{	if(data==NULL) return(EMS_NULL_POINTER);
			ID=data[0];
			if(ID==EMS_USERDEFINED_MELODY)
			{	object->sound.attribute=data[1];
				object->sound.pdu_length=data[2]|(data[3]<<8);
				data+=4;
				object->sound.pdu=data;
			#ifdef __EMS_REL5__
				if((object->sound.pdu_length==0) || (object->sound.attribute==1))
			#else
				if((object->sound.pdu_length==0)||(object->sound.pdu_length>EMS_MAX_IMELODY_SIZE))
			#endif
					return(EMS_INVALID_OBJECT);
			}
			else return(EMS_INVALID_OBJECT);
		}
		break;
	#endif
	//CSD end
	}
	return(EMS_OK);
}

#if(ENABLE_EMS_TEST_DEMO || ENABLE_EMS_TEST_OBJECTS)	/* test_block	*/

#define MAX_EMS_TEST_OBJECTS	10

U8	EMS_test_object_count=0;
U8* EMS_test_objects[MAX_EMS_TEST_OBJECTS]=
{	(U8*)EMS_sample_picture_object_PDU_1,
	(U8*)EMS_sample_picture_object_PDU_2,
	(U8*)EMS_sample_picture_object_PDU_3,
	(U8*)EMS_sample_picture_object_PDU_4,
	(U8*)EMS_sample_picture_object_PDU_5,
	(U8*)EMS_sample_picture_object_PDU_6,
	(U8*)EMS_sample_picture_object_PDU_7,
	(U8*)EMS_sample_picture_object_PDU_8,
	(U8*)EMS_sample_picture_object_PDU_9,
	(U8*)EMS_sample_picture_object_PDU_10,
};

#endif

#if(ENABLE_EMS_TEST_DEMO)	/* test_block	*/

void EMS_inputbox_insert_object(void)
{	EMSObjData object;
	EMSTATUS ret;
	U8*	data;
	U8 ID;

	S32 index=EMS_test_object_count++;
	if(EMS_test_object_count>9) EMS_test_object_count=0;
	data=EMS_test_objects[index];
	ID=data[0];
	switch(ID)
	{	case	EMS_PREDEFINED_PICTURE:
		case	EMS_USERDEFINED_PICTURE:
				ret=EMS_load_object(EMS_PREDEFINED_PICTURE,data,index,&object);
				if(ret==EMS_OK)
					ret=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_PIC,&object,0);
				break;

		case 	EMS_USERDEFINED_ANIMATION:
				ret=EMS_load_object(EMS_USERDEFINED_ANIMATION,data,index,&object);
				if(ret==EMS_OK)
					ret=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_USERDEF_ANM,&object,0);
				break;

		case	EMS_PREDEFINED_ANIMATION:
				ret=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_PREDEF_ANM,NULL,data[1]);
				break;

		case 	EMS_PREDEFINED_SOUND:
				ret=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_PREDEF_SND,NULL,data[1]);
				break;

		case 	EMS_PREDEFINED_MELODY:
				ret=EMS_load_object(EMS_PREDEFINED_MELODY,data,index,&object);
				if(ret==EMS_OK)
					ret=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_USERDEF_SND,&object,0);
				break;
	}
	//show_EMS_inputbox();
}

#endif

#if(ENABLE_EMS_TEST_OBJECTS)

EMSTATUS EMS_inputbox_insert_predefined_picture(S32 index)
{	EMSObjData object;
    EMSTATUS ret;
	U8*	data;
	U8 ID;

	if(index<0) index=0;
	if(index>(MAX_EMS_TEST_OBJECTS-1)) index=MAX_EMS_TEST_OBJECTS-1;
	data=EMS_test_objects[index];
	ID=data[0];
	switch(ID)
	{	case	EMS_PREDEFINED_PICTURE:
		case	EMS_USERDEFINED_PICTURE:
				ret=EMS_load_object(EMS_PREDEFINED_PICTURE,data,index,&object);
				if(ret==EMS_OK)
					ret=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_PIC,&object,0);
				break;

		case 	EMS_USERDEFINED_ANIMATION:
				ret=EMS_load_object(EMS_USERDEFINED_ANIMATION,data,index,&object);
				if(ret==EMS_OK)
					ret=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_USERDEF_ANM,&object,0);
				break;

		case	EMS_PREDEFINED_ANIMATION:
				ret=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_PREDEF_ANM,NULL,data[1]);
				break;

		case 	EMS_PREDEFINED_SOUND:
				ret=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_PREDEF_SND,NULL,data[1]);
				break;

		case 	EMS_PREDEFINED_MELODY:
				ret=EMS_load_object(EMS_PREDEFINED_MELODY,data,index,&object);
				if(ret==EMS_OK)
					ret=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_USERDEF_SND,&object,0);
				break;
	}
	return(ret);
}

#else

EMSTATUS EMS_inputbox_insert_predefined_picture(S32 index)
{	EMSObjData object;
	U8* data=NULL;
	EMSTATUS r=EMS_load_object(EMS_PREDEFINED_PICTURE,data,index,&object);
	if(r==EMS_OK)
		r=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_PIC,&object,0);
	return(r);
}

#endif

S32 EMS_get_object_size(U8 type,EMSObjData *objdata)
{	return(EMSGetObjectSize(type,objdata));
}

S32 EMS_check_object_size(U8 type,EMSObjData *objdata,byte *allowed)
{	kal_bool flag;
	S32 size=EMSGetObjectSizeWithCheck(type,objdata,&flag);
	if(flag==KAL_TRUE) *allowed=1;
	else *allowed=0;
	return(size);
}

void EMS_load_predefined_picture(S32 index,U8 *data)
{	if((index>=0) && (index<MAX_EMS_PREDEFINED_PICTURES))
		EMS_predefined_pictures[index]=data;
}

S32 EMS_get_predefined_picture_size(S32 index)
{	EMSObjData object;
	U8* data=NULL;
	EMSTATUS r=EMS_load_object(EMS_PREDEFINED_PICTURE,data,index,&object);
	if(r==EMS_OK)
		return(EMS_get_object_size(EMS_TYPE_PIC,&object));
	return(0);
}

S32 EMS_get_predefined_picture_data_size(U8* data)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_PREDEFINED_PICTURE,data,-1,&object);
	if(r==EMS_OK)
		return(EMS_get_object_size(EMS_TYPE_PIC,&object));
	return(0);
}

S32 EMS_check_predefined_picture_data_size(U8* data,U8* allowed)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_PREDEFINED_PICTURE,data,-1,&object);
	if(r==EMS_OK)
		return(EMS_check_object_size(EMS_TYPE_PIC,&object,allowed));
	return(0);
}

S32 EMS_get_userdefined_picture_size(U8* data)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_USERDEFINED_PICTURE,data,0,&object);
	if(r==EMS_OK)
		return(EMS_get_object_size(EMS_TYPE_PIC,&object));
	return(0);
}

S32 EMS_check_userdefined_picture_size(U8* data,U8* allowed)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_USERDEFINED_PICTURE,data,0,&object);
	if(r==EMS_OK)
		return(EMS_check_object_size(EMS_TYPE_PIC,&object,allowed));
	return(0);
}

EMSTATUS EMS_inputbox_insert_userdefined_picture(U8* data)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_USERDEFINED_PICTURE,data,0,&object);
	if(r==EMS_OK)
		r=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_PIC,&object,0);
	return(r);
}

S32 EMS_get_predefined_animation_size(S32 index)
{	EMSObjData object;
	if((index<0)||(index>=MAX_EMS_PREDEFINED_ANIMATIONS)) return(0);
	return(EMS_get_object_size(EMS_TYPE_PREDEF_ANM,&object));
}

EMSTATUS EMS_inputbox_insert_predefined_animation(S32 index)
{	if((index<0)||(index>=MAX_EMS_PREDEFINED_ANIMATIONS)) return(EMS_INVALID_OBJECT);
	return(coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_PREDEF_ANM,NULL,(U8)index));
}

S32 EMS_get_userdefined_animation_size(U8* data)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_USERDEFINED_ANIMATION,data,0,&object);
	if(r==EMS_OK)
		return(EMS_get_object_size(EMS_TYPE_USERDEF_ANM,&object));
	return(0);
}

S32 EMS_check_userdefined_animation_size(U8* data,U8* allowed)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_USERDEFINED_ANIMATION,data,0,&object);
	if(r==EMS_OK)
		return(EMS_check_object_size(EMS_TYPE_USERDEF_ANM,&object,allowed));
	return(0);
}

EMSTATUS EMS_inputbox_insert_userdefined_animation(U8* data)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_USERDEFINED_ANIMATION,data,0,&object);
	if(r==EMS_OK)
		r=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_USERDEF_ANM,&object,0);
	return(r);
}

S32 EMS_get_predefined_sound_size(S32 index)
{	EMSObjData object;
	if((index<0)||(index>=MAX_EMS_PREDEFINED_SOUNDS)) return(0);
	return(EMS_get_object_size(EMS_TYPE_PREDEF_SND,&object));
}

EMSTATUS EMS_inputbox_insert_predefined_sound(S32 index)
{	if((index<0)||(index>=MAX_EMS_PREDEFINED_SOUNDS)) return(EMS_INVALID_OBJECT);
	return(coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_PREDEF_SND,NULL,(U8)index));
}

//CSD Joy added for customization, 1014
#if defined(__MMI_IMELODY_SUPPORT__)

void EMS_load_predefined_melody(S32 index,U8 *data)
{	if((index>=0) && (index<MAX_EMS_PREDEFINED_MELODIES))
		EMS_predefined_melodies[index]=data;
}

S32 EMS_get_predefined_melody_size(S32 index)
{	EMSObjData object;
	U8* data=NULL;
	EMSTATUS r=EMS_load_object(EMS_PREDEFINED_MELODY,data,index,&object);
	if(r==EMS_OK)
		return(EMS_get_object_size(EMS_TYPE_USERDEF_SND,&object));
	return(0);
}

S32 EMS_get_predefined_melody_data_size(U8 *data)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_PREDEFINED_MELODY,data,-1,&object);
	if(r==EMS_OK)
		return(EMS_get_object_size(EMS_TYPE_USERDEF_SND,&object));
	return(0);
}

S32 EMS_check_predefined_melody_data_size(U8 *data,U8* allowed)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_PREDEFINED_MELODY,data,-1,&object);
	if(r==EMS_OK)
		return(EMS_check_object_size(EMS_TYPE_USERDEF_SND,&object,allowed));
	return(0);
}

EMSTATUS EMS_inputbox_insert_predefined_melody(S32 index)
{	EMSObjData object;
	U8* data=NULL;
	EMSTATUS r=EMS_load_object(EMS_PREDEFINED_MELODY,data,index,&object);
	if(r==EMS_OK)
		r=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_USERDEF_SND,&object,0);
	return(r);
}

S32 EMS_get_userdefined_melody_size(U8* data)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_USERDEFINED_MELODY,data,0,&object);
	if(r==EMS_OK)
		return(EMS_get_object_size(EMS_TYPE_USERDEF_SND,&object));
	return(0);
}

S32 EMS_check_userdefined_melody_size(U8* data,U8* allowed)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_USERDEFINED_MELODY,data,0,&object);
	if(r==EMS_OK)
		return(EMS_check_object_size(EMS_TYPE_USERDEF_SND,&object,allowed));
	return(0);
}

EMSTATUS EMS_inputbox_insert_userdefined_melody(U8* data)
{	EMSObjData object;
	EMSTATUS r=EMS_load_object(EMS_USERDEFINED_MELODY,data,0,&object);
	if(r==EMS_OK)
		r=coolsand_UI_EMS_inputbox_add_object(&MMI_EMS_inputbox,EMS_TYPE_USERDEF_SND,&object,0);
	return(r);
}

void EMS_userdefined_melody_attach_header(U8 *src,U8 *dst,S32 *size)
{
        coolsand_UI_memcpy(dst+4,src,*size);
	dst[0]=EMS_USERDEFINED_MELODY;
	dst[1]=0;
	dst[2]=(U8)((*size)&0xff);
	dst[3]=(U8)((*size)>>8);
	*size=*size+4;
}

void EMS_userdefined_melody_detach_header(U8 *src,U8 *dst,S32 *size)
{
        *size=*size-4;
	coolsand_UI_memcpy(dst,src+4,*size);
}

void EMS_predefined_melody_attach_header(U8 *src,U8 *dst,S32 *size)
{
        coolsand_UI_memcpy(dst+4,src,*size);
	dst[0]=EMS_PREDEFINED_MELODY;
	dst[1]=0;
	dst[2]=(U8)((*size)&0xff);
	dst[3]=(U8)((*size)>>8);
	*size=*size+4;
}

void EMS_predefined_melody_detach_header(U8 *src,U8 *dst,S32 *size)
{
        *size=*size-4;
	coolsand_UI_memcpy(dst,src+4,*size);
}

#endif
//CSD end

void EMS_inputbox_set_text_size(EMSFONTSIZE s)
{	wgui_EMS_text_format.FontSize=s;
//	AddString(MMI_EMS_inputbox.data,NULL,0,&wgui_EMS_text_format);
}

EMSFONTSIZE EMS_inputbox_get_text_size(void)
{
#ifdef __MMI_DIALLING_FONT__
	if(wgui_EMS_text_format.FontSize==EMS_FONTSIZE_LARGE) return(EMS_FONTSIZE_NORMAL);
	else return(wgui_EMS_text_format.FontSize);
#else
	return(wgui_EMS_text_format.FontSize);
#endif
}

void EMS_inputbox_set_text_style(U8 isBold,U8 isItalic,U8 isUnderline,U8 isStrikethrough)
{	if(isBold) wgui_EMS_text_format.isBold=KAL_TRUE;
	else wgui_EMS_text_format.isBold=KAL_FALSE;
	if(isItalic) wgui_EMS_text_format.isItalic=KAL_TRUE;
	else wgui_EMS_text_format.isItalic=KAL_FALSE;
	if(isUnderline) wgui_EMS_text_format.isUnderline=KAL_TRUE;
	else wgui_EMS_text_format.isUnderline=KAL_FALSE;
	if(isStrikethrough) wgui_EMS_text_format.isStrikethrough=KAL_TRUE;
	else wgui_EMS_text_format.isStrikethrough=KAL_FALSE;
//	AddString(MMI_EMS_inputbox.data,NULL,0,&wgui_EMS_text_format);
}

void EMS_inputbox_get_text_style(U8 *isBold,U8 *isItalic,U8 *isUnderline,U8 *isStrikethrough)
{	if(wgui_EMS_text_format.isBold==KAL_TRUE) *isBold=1;
	else *isBold=0;
	if(wgui_EMS_text_format.isItalic==KAL_TRUE) *isItalic=1;
	else *isItalic=0;
	if(wgui_EMS_text_format.isUnderline==KAL_TRUE) *isUnderline=1;
	else *isUnderline=0;
	if(wgui_EMS_text_format.isStrikethrough==KAL_TRUE) *isStrikethrough=1;
	else *isStrikethrough=0;
}
#endif //endif __MMI_MESSAGES_EMS__
byte EMS_inputbox_alignment_added_flag=0;
EMSALIGNMENT EMS_inputbox_alignment=EMS_ALIGN_LANG_DEP;
#ifdef __MMI_MESSAGES_EMS__
EMSTATUS EMS_inputbox_set_text_alignment(EMSALIGNMENT a)
{
	EMSTATUS emsObjectStatus;
	EMSTextFormat saved_wgui_EMS_text_format=wgui_EMS_text_format;
/*	Old code: Used for testing purpose.	Leave commented	*/
//	ResetTextFormat(&wgui_EMS_text_format);
//	wgui_EMS_text_format.Alignment=a;
/*	Old code: ends	*/
	saved_wgui_EMS_text_format.Alignment=a;
	emsObjectStatus=coolsand_UI_EMS_input_box_insert_text_alignment(&MMI_EMS_inputbox,&saved_wgui_EMS_text_format);
/*	Old code: Used for testing purpose.	Leave commented	*/
//	AddString(MMI_EMS_inputbox.data,NULL,0,&wgui_EMS_text_format);
//	wgui_EMS_text_format=saved_wgui_EMS_text_format;
/*	Old code: ends	*/
	if(emsObjectStatus!=EMS_OK)
		return emsObjectStatus;
	if(MMI_EMS_inputbox.data->CurrentTextFormatObj!=NULL)
	{	wgui_EMS_text_format.Alignment=MMI_EMS_inputbox.data->CurrentTextFormatObj->data->text_format.Alignment;
	}
/*	HACK: Set a flag to indicate that text alignment has been added	*/
	EMS_inputbox_alignment_added_flag=1;
	EMS_inputbox_alignment=a;
	return emsObjectStatus;
}

EMSALIGNMENT EMS_inputbox_get_text_alignment(void)
{	return(wgui_EMS_text_format.Alignment);
}

EMSTATUS EMS_inputbox_create_new_paragraph(EMSALIGNMENT a)
{
	EMSTATUS emsObjectStatus;
#if(0)
	wgui_EMS_text_format.Alignment=a;
	MMI_EMS_inputbox.text_format.Alignment=a;
	EMS_inputbox_insert_new_line();
#else
	wgui_EMS_text_format.Alignment=a;
	emsObjectStatus=coolsand_UI_EMS_input_box_insert_new_paragraph(&MMI_EMS_inputbox,&wgui_EMS_text_format);
//	AddString(MMI_EMS_inputbox.data,NULL,0,&wgui_EMS_text_format);
	if(emsObjectStatus!=EMS_OK)
	{
		wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
	}
	EMS_inputbox_input_callback();
	return emsObjectStatus;
#endif
}

#if(0)

byte EMS_bold=1;

void EMS_inputbox_insert_text_format(void)
{	EMSObjData	object;

	ResetTextFormat(&object.text_format);
	object.text_format.bgColor=EMS_BLACK;
	object.text_format.fgColor=EMS_BLACK;
	object.text_format.Alignment=EMS_ALIGN_LEFT;
	if(EMS_bold) object.text_format.FontSize=EMS_FONTSIZE_SMALL;
	else object.text_format.FontSize=EMS_FONTSIZE_NORMAL;
	object.text_format.isBold=0;
	object.text_format.isItalic=0;
	object.text_format.isUnderline=0;
	object.text_format.isStrikethrough=0;
	object.text_format.textLength=1;
	coolsand_toggle(EMS_bold);
	AddString(MMI_EMS_inputbox.data,NULL,0,&object.text_format);
}

#endif

extern UI_image_type	EMS_editor_sound_icon;
extern UI_image_type	EMS_editor_melody_icon;

void initialize_EMS_objects(void)
{	EMS_editor_sound_icon=get_image(EMS_EDITOR_SOUND_OBJECT_ID);
	EMS_editor_melody_icon=get_image(EMS_EDITOR_MELODY_OBJECT_ID);
}

void close_EMS_objects(void)
{
}

/*	EMS category screens: Application menus	*/

#define EMS_APPLICATION_MENUS_BASE							(DEMO_APP_BASE+400)
#define EMS_APPLICATION_MENUS_SCREEN_ID_BASE				(EMS_APPLICATION_MENUS_BASE+0)
#define EMS_MAIN_MENU_SCREEN_ID								(EMS_APPLICATION_MENUS_SCREEN_ID_BASE+1)
#define EMS_OBJECTS_MENU_SCREEN_ID							(EMS_APPLICATION_MENUS_SCREEN_ID_BASE+2)
#define EMS_TEXT_FORMATTING_MENU_SCREEN_ID					(EMS_APPLICATION_MENUS_SCREEN_ID_BASE+3)
#define EMS_TEXT_ALIGNMENT_SELECTION_MENU_SCREEN_ID			(EMS_APPLICATION_MENUS_SCREEN_ID_BASE+4)
#define EMS_TEXT_SIZE_SELECTION_MENU_SCREEN_ID				(EMS_APPLICATION_MENUS_SCREEN_ID_BASE+5)
#define EMS_TEXT_STYLE_SELECTION_MENU_SCREEN_ID				(EMS_APPLICATION_MENUS_SCREEN_ID_BASE+6)
#define EMS_TEXT_NEW_PARAGRAPH_ALIGNMENT_MENU_SCREEN_ID		(EMS_APPLICATION_MENUS_SCREEN_ID_BASE+7)

#define MAX_EMS_MAIN_MENU_ITEMS	5

#if(EMS_USE_INTERNAL_MENU)

#ifdef __UCS2_ENCODING

U8* EMS_main_menu_items[]=
{	(U8*)"A\0d\0d\0 \0s\0y\0m\0b\0o\0l\0\0",
	(U8*)"I\0n\0s\0e\0r\0t\0 \0o\0b\0j\0e\0c\0t\0\0",
	(U8*)"F\0o\0r\0m\0a\0t\0 \0t\0e\0x\0t\0\0",
	(U8*)"I\0n\0p\0u\0t\0 \0m\0e\0t\0h\0o\0d\0\0",
	(U8*)"H\0e\0l\0p\0\0",
};

#else

U8* EMS_main_menu_items[]=
{	(U8*)"Add symbol",
	(U8*)"Insert object",
	(U8*)"Formart text",
	(U8*)"Input method",
	(U8*)"Help",
};

#endif

void HandleEMSMainMenuLSK(void)
{	switch(GetHighlightedItem())
	{	case 0:	break;
		case 1:	/*	Temporary: For testing	*/
				EMS_inputbox_insert_object();
				GoBackHistory();
				/*EntryEMSObjectsMenu();*/
				break;
		case 2:	GotoEMSTextFormattingMenu();
				break;
		case 3:	break;
		case 4:	break;
	}
}

void EntryEMSMainMenu(void)
{
	U8* guiBuffer;
	ExecuteCurrExitHandler();
	guiBuffer = GetCurrGuiBuffer(EMS_MAIN_MENU_SCREEN_ID);
	ShowCategory6Screen(  0, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_EMS_MAIN_MENU_ITEMS, EMS_main_menu_items, 0, guiBuffer);
	SetLeftSoftkeyFunction(HandleEMSMainMenuLSK,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetExitHandler(EMS_MAIN_MENU_SCREEN_ID, ExitEMSMainMenu);
}

void ExitEMSMainMenu(void)
{
	history_t   h;

	h.scrnID = EMS_MAIN_MENU_SCREEN_ID;
	h.entryFuncPtr = EntryEMSMainMenu;
	h.inputBuffer[0]='\0';
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

#define MAX_EMS_OBJECTS_MENU_ITEMS	4

#ifdef __UCS2_ENCODING

U8* EMS_objects_menu_items[]=
{	(U8*)"P\0i\0c\0t\0u\0r\0e\0\0",
	(U8*)"A\0n\0i\0m\0a\0t\0i\0o\0n\0\0",
	(U8*)"S\0o\0u\0n\0d\0\0",
	(U8*)"M\0e\0l\0o\0d\0y\0\0",
};

#else

U8* EMS_objects_menu_items[]=
{	(U8*)"Picture",
	(U8*)"Animation",
	(U8*)"Sound",
	(U8*)"Melody",
};

#endif

void EntryEMSObjectsMenu(void)
{
	U8* guiBuffer;
	ExecuteCurrExitHandler();
	guiBuffer = GetCurrGuiBuffer(EMS_OBJECTS_MENU_SCREEN_ID);
	ShowCategory6Screen(  0, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_EMS_OBJECTS_MENU_ITEMS, EMS_objects_menu_items, 0, guiBuffer);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetExitHandler(EMS_OBJECTS_MENU_SCREEN_ID, ExitEMSObjectsMenu);
}

void ExitEMSObjectsMenu(void)
{
	history_t   h;

	h.scrnID = EMS_OBJECTS_MENU_SCREEN_ID;
	h.entryFuncPtr = EntryEMSObjectsMenu;
	h.inputBuffer[0]='\0';
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

#define MAX_EMS_TEXT_FORMATTING_MENU_ITEMS	4

#ifdef __UCS2_ENCODING

U8* EMS_text_formatting_menu_items[]=
{	(U8*)"T\0e\0x\0t\0 \0s\0i\0z\0e\0\0",
	(U8*)"T\0e\0x\0t\0 \0s\0t\0y\0l\0e\0\0",
	(U8*)"A\0l\0i\0g\0n\0m\0e\0n\0t\0\0",
	(U8*)"N\0e\0w\0 \0p\0a\0r\0a\0g\0r\0a\0p\0h\0\0",
};

#else

U8* EMS_text_formatting_menu_items[]=
{	(U8*)"Text size",
	(U8*)"Text style",
	(U8*)"Alignment",
	(U8*)"New paragraph",
};

#endif

void HandleEMSTextFormattingMenuLSK(void)
{	switch(GetHighlightedItem())
	{	case 0:	EntryEMSTextSizeSelectionMenu();
				break;
		case 1:	EntryEMSTextStyleSelectionMenu();
				break;
		case 2:	EntryEMSTextAlignmentSelectionMenu();
				break;
		case 3:	EntryEMSNewParagraphAlignmentMenu();
				break;
	}
}

/*	0: false, 1: true	*/
byte EMSTextFormattingMenu_bold=0;
byte EMSTextFormattingMenu_italic=0;
byte EMSTextFormattingMenu_underline=0;
byte EMSTextFormattingMenu_strikethrough=0;

/*	1: left, 2: right, 3: center, 0: automatic	*/
byte EMSTextFormattingMenu_alignment=0;

/*	0: small, 1: medium, 2: large	*/
byte EMSTextFormattingMenu_font_size=1;

void LoadEMSTextFormattingMenu(void)
{	EMSTextFormat *o;

	if((MMI_EMS_inputbox.data==NULL) || (MMI_EMS_inputbox.data->CurrentTextFormatObj==NULL))
	{
		EMSTextFormattingMenu_bold=0;
		EMSTextFormattingMenu_italic=0;
		EMSTextFormattingMenu_strikethrough=0;
		EMSTextFormattingMenu_underline=0;
		EMSTextFormattingMenu_alignment=0;
		EMSTextFormattingMenu_font_size=1;
	}
	else
	{
		o=&MMI_EMS_inputbox.data->CurrentTextFormatObj->data->text_format;
		if(o->isBold) EMSTextFormattingMenu_bold=1;
		else EMSTextFormattingMenu_bold=0;
		if(o->isItalic) EMSTextFormattingMenu_italic=1;
		else EMSTextFormattingMenu_italic=0;
		if(o->isStrikethrough) EMSTextFormattingMenu_strikethrough=1;
		else EMSTextFormattingMenu_strikethrough=0;
		if(o->isUnderline) EMSTextFormattingMenu_underline=1;
		else EMSTextFormattingMenu_underline=0;

		switch(o->Alignment)
		{
			case EMS_ALIGN_LEFT:		EMSTextFormattingMenu_alignment=1;
										break;
			case EMS_ALIGN_RIGHT:		EMSTextFormattingMenu_alignment=2;
										break;
			case EMS_ALIGN_CENTER:		EMSTextFormattingMenu_alignment=3;
										break;
			case EMS_ALIGN_LANG_DEP:	EMSTextFormattingMenu_alignment=0;
										break;
		}
		switch(o->FontSize)
		{
			case EMS_FONTSIZE_NORMAL:	EMSTextFormattingMenu_font_size=1;
										break;
			case EMS_FONTSIZE_LARGE:	EMSTextFormattingMenu_font_size=2;
										break;
			case EMS_FONTSIZE_SMALL:	EMSTextFormattingMenu_font_size=0;
										break;
		}
	}
}

void StoreEMSTextFormattingMenu(void)
{	EMSObjData	object;

	ResetTextFormat(&object.text_format);
	object.text_format.bgColor=EMS_BLACK;
	object.text_format.fgColor=EMS_BLACK;
	switch(EMSTextFormattingMenu_alignment)
	{	case 0:	object.text_format.Alignment=EMS_ALIGN_LEFT;
				break;
		case 1:	object.text_format.Alignment=EMS_ALIGN_LEFT;
				break;
		case 2:	object.text_format.Alignment=EMS_ALIGN_RIGHT;
				break;
		case 3:	object.text_format.Alignment=EMS_ALIGN_CENTER;
				break;
	}
	switch(EMSTextFormattingMenu_font_size)
	{	case 0:	object.text_format.FontSize=EMS_FONTSIZE_SMALL;
				break;
		case 1:	object.text_format.FontSize=EMS_FONTSIZE_NORMAL;
				break;
		case 2:	object.text_format.FontSize=EMS_FONTSIZE_LARGE;
				break;
	}
	if(EMSTextFormattingMenu_bold) object.text_format.isBold=1;
	else object.text_format.isBold=0;
	if(EMSTextFormattingMenu_italic) object.text_format.isItalic=1;
	else object.text_format.isItalic=0;
	if(EMSTextFormattingMenu_underline) object.text_format.isUnderline=1;
	else object.text_format.isUnderline=0;
	if(EMSTextFormattingMenu_strikethrough) object.text_format.isStrikethrough=1;
	else object.text_format.isStrikethrough=0;
	object.text_format.textLength=1;
	wgui_EMS_text_format=object.text_format;
}

void HandleEMSTextFormattingMenuRSK(void)
{	StoreEMSTextFormattingMenu();
	GoBackHistory();
	GoBackHistory();
}

void GotoEMSTextFormattingMenu(void)
{	LoadEMSTextFormattingMenu();
	EntryEMSTextFormattingMenu();
}

void EntryEMSTextFormattingMenu(void)
{
	U8* guiBuffer;
	ExecuteCurrExitHandler();
	guiBuffer = GetCurrGuiBuffer(EMS_TEXT_FORMATTING_MENU_SCREEN_ID);
	ShowCategory6Screen(  0, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_EMS_TEXT_FORMATTING_MENU_ITEMS, EMS_text_formatting_menu_items, 0, guiBuffer);
	SetLeftSoftkeyFunction(HandleEMSTextFormattingMenuLSK,KEY_EVENT_UP);
	SetRightSoftkeyFunction(HandleEMSTextFormattingMenuRSK,KEY_EVENT_UP);
	SetExitHandler(EMS_TEXT_FORMATTING_MENU_SCREEN_ID, ExitEMSTextFormattingMenu);
}

void ExitEMSTextFormattingMenu(void)
{
	history_t   h;

	h.scrnID = EMS_TEXT_FORMATTING_MENU_SCREEN_ID;
	h.entryFuncPtr = EntryEMSTextFormattingMenu;
	h.inputBuffer[0]='\0';
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

#define MAX_EMS_TEXT_SIZE_MENU_ITEMS	3

#ifdef __UCS2_ENCODING

U8* EMS_text_size_menu_items[]=
{	(U8*)"S\0m\0a\0l\0l\0\0",
	(U8*)"M\0e\0d\0i\0u\0m\0\0",
	(U8*)"L\0a\0r\0g\0e\0\0",
};

#else

U8* EMS_text_size_menu_items[]=
{	(U8*)"Small",
	(U8*)"Medium",
	(U8*)"Large",
};

#endif

void HandleEMSTextSizeSelectionMenuLSK(void)
{	EMSTextFormattingMenu_font_size=(byte)GetHighlightedItem();
	GoBackHistory();
}

void EntryEMSTextSizeSelectionMenu(void)
{
	U8* guiBuffer;
	U16	h;

	ExecuteCurrExitHandler();
	guiBuffer = GetCurrGuiBuffer(EMS_TEXT_SIZE_SELECTION_MENU_SCREEN_ID);
	h=EMSTextFormattingMenu_font_size;
	ShowCategory36Screen( 0, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_EMS_TEXT_SIZE_MENU_ITEMS, EMS_text_size_menu_items, h,guiBuffer);
	SetLeftSoftkeyFunction(HandleEMSTextSizeSelectionMenuLSK,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetExitHandler(EMS_TEXT_SIZE_SELECTION_MENU_SCREEN_ID, ExitEMSTextSizeSelectionMenu);
}

void ExitEMSTextSizeSelectionMenu(void)
{
	history_t   h;

	h.scrnID = EMS_TEXT_SIZE_SELECTION_MENU_SCREEN_ID;
	h.entryFuncPtr = EntryEMSTextSizeSelectionMenu;
	h.inputBuffer[0]='\0';
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

#define MAX_EMS_TEXT_STYLE_MENU_ITEMS	4

#ifdef __UCS2_ENCODING

U8* EMS_text_style_menu_items[]=
{	(U8*)"B\0o\0l\0d\0\0",
	(U8*)"I\0t\0a\0l\0i\0c\0\0",
	(U8*)"U\0n\0d\0e\0r\0l\0i\0n\0e\0\0",
	(U8*)"S\0t\0r\0i\0k\0e\0t\0h\0r\0o\0u\0g\0h\0\0",
};

#else

U8* EMS_text_style_menu_items[]=
{	(U8*)"Bold",
	(U8*)"Italic",
	(U8*)"Underline",
	(U8*)"Strikethrough",
};

#endif

byte	EMS_text_style_menu_states[]=
{	0,0,0,0,
};

void HandleEMSTextStyleSelectionMenuRSK(void)
{
	if(EMS_text_style_menu_states[0])	EMSTextFormattingMenu_bold=1;
	else								EMSTextFormattingMenu_bold=0;
	if(EMS_text_style_menu_states[1])	EMSTextFormattingMenu_italic=1;
	else								EMSTextFormattingMenu_italic=0;
	if(EMS_text_style_menu_states[2])	EMSTextFormattingMenu_underline=1;
	else								EMSTextFormattingMenu_underline=0;
	if(EMS_text_style_menu_states[3])	EMSTextFormattingMenu_strikethrough=1;
	else								EMSTextFormattingMenu_strikethrough=0;
	GoBackHistory();
}

void EntryEMSTextStyleSelectionMenu(void)
{
	U8* guiBuffer;
	ExecuteCurrExitHandler();
	guiBuffer = GetCurrGuiBuffer(EMS_TEXT_STYLE_SELECTION_MENU_SCREEN_ID);

	if(EMSTextFormattingMenu_bold)				EMS_text_style_menu_states[0]=1;
	else										EMS_text_style_menu_states[0]=0;
	if(EMSTextFormattingMenu_italic)			EMS_text_style_menu_states[1]=1;
	else										EMS_text_style_menu_states[1]=0;
	if(EMSTextFormattingMenu_underline)			EMS_text_style_menu_states[2]=1;
	else										EMS_text_style_menu_states[2]=0;
	if(EMSTextFormattingMenu_strikethrough)		EMS_text_style_menu_states[3]=1;
	else										EMS_text_style_menu_states[3]=0;

	ShowCategory12Screen( 0, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_EMS_TEXT_STYLE_MENU_ITEMS, EMS_text_style_menu_items, EMS_text_style_menu_states, 0, guiBuffer);
	SetRightSoftkeyFunction(HandleEMSTextStyleSelectionMenuRSK,KEY_EVENT_UP);
	SetExitHandler(EMS_TEXT_STYLE_SELECTION_MENU_SCREEN_ID, ExitEMSTextStyleSelectionMenu);
}

void ExitEMSTextStyleSelectionMenu(void)
{
	history_t   h;

	h.scrnID = EMS_TEXT_STYLE_SELECTION_MENU_SCREEN_ID;
	h.entryFuncPtr = EntryEMSTextStyleSelectionMenu;
	h.inputBuffer[0]='\0';
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

#define MAX_EMS_TEXT_ALIGNMENT_MENU_ITEMS	4

#ifdef __UCS2_ENCODING

U8*	EMS_text_alignment_menu_items[]=
{	(U8*)"A\0u\0t\0o\0m\0a\0t\0i\0c\0\0",
	(U8*)"L\0e\0f\0t\0\0",
	(U8*)"R\0i\0g\0h\0t\0\0",
	(U8*)"C\0e\0n\0t\0e\0r\0\0",
};

#else

U8*	EMS_text_alignment_menu_items[]=
{	(U8*)"Automatic",
	(U8*)"Left",
	(U8*)"Right",
	(U8*)"Center",
};

#endif

void HandleEMSTextAlignmentSelectionMenuLSK(void)
{	EMSTextFormattingMenu_alignment=(byte)GetHighlightedItem();
	GoBackHistory();
}

void EntryEMSTextAlignmentSelectionMenu(void)
{
	U8* guiBuffer;
	U16	h;

	ExecuteCurrExitHandler();
	guiBuffer = GetCurrGuiBuffer(EMS_TEXT_ALIGNMENT_SELECTION_MENU_SCREEN_ID);
	h=EMSTextFormattingMenu_alignment;
	ShowCategory36Screen( 0, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_EMS_TEXT_ALIGNMENT_MENU_ITEMS, EMS_text_alignment_menu_items, h,guiBuffer);
	SetLeftSoftkeyFunction(HandleEMSTextAlignmentSelectionMenuLSK,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetExitHandler(EMS_TEXT_ALIGNMENT_SELECTION_MENU_SCREEN_ID, ExitEMSTextAlignmentSelectionMenu);
}

void ExitEMSTextAlignmentSelectionMenu(void)
{
	history_t   h;

	h.scrnID = EMS_TEXT_ALIGNMENT_SELECTION_MENU_SCREEN_ID;
	h.entryFuncPtr = EntryEMSTextAlignmentSelectionMenu;
	h.inputBuffer[0]='\0';
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

void HandleEMSNewParagraphAlignmentMenuLSK(void)
{	EMSTextFormattingMenu_alignment=(byte)GetHighlightedItem();
	GoBackHistory();
	StoreEMSTextFormattingMenu();
	EMS_inputbox_insert_new_line();
	GoBackHistory();
	GoBackHistory();
}

void EntryEMSNewParagraphAlignmentMenu(void)
{
	U8* guiBuffer;
	U16	h;

	ExecuteCurrExitHandler();
	guiBuffer = GetCurrGuiBuffer(EMS_TEXT_ALIGNMENT_SELECTION_MENU_SCREEN_ID);
	h=EMSTextFormattingMenu_alignment;
	ShowCategory36Screen( 0, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  MAX_EMS_TEXT_ALIGNMENT_MENU_ITEMS, EMS_text_alignment_menu_items, h,guiBuffer);
	SetLeftSoftkeyFunction(HandleEMSNewParagraphAlignmentMenuLSK,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetExitHandler(EMS_TEXT_ALIGNMENT_SELECTION_MENU_SCREEN_ID, ExitEMSTextAlignmentSelectionMenu);
}

void ExitEMSNewParagraphAlignmentMenu(void)
{
	history_t   h;

	h.scrnID = EMS_TEXT_ALIGNMENT_SELECTION_MENU_SCREEN_ID;
	h.entryFuncPtr = EntryEMSTextAlignmentSelectionMenu;
	h.inputBuffer[0]='\0';
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}
#endif// __MMI_MESSAGES_EMS__
#endif	/*	UI_DISABLE_EMS_INPUT_BOX */

/*	EMS input box history related functions	*/

typedef struct _EMS_inputbox_category_history
{	word	history_ID;
	s16		cursor_ax,cursor_ay;
	s16		cursor_line_height;
	s16		display_y;
	s16		input_type;
	s16		UCS2_count;
#if defined(__MMI_TOUCH_SCREEN__)
	s16		vk_lang_type;
#endif
} EMS_inputbox_category_history;

extern byte history_valid(word history_ID,word category_screen_ID);

void get_EMS_inputbox_category_history(word history_ID,byte *history_buffer,s16 input_type)
{
   if(history_buffer!=NULL)
   {
      word hID=(word)(history_ID|0x8000);
      EMS_inputbox_category_history *h=(EMS_inputbox_category_history*)history_buffer;
      h->history_ID=hID;
      h->cursor_ax=(s16)MMI_EMS_inputbox.cursor_ax;
      h->cursor_ay=(s16)MMI_EMS_inputbox.cursor_ay;
      h->cursor_line_height=(s16)MMI_EMS_inputbox.cursor_line_height;
      h->display_y=(s16)MMI_EMS_inputbox.display_y;
      h->input_type=(s16)input_type;
      h->UCS2_count=(s16)MMI_EMS_inputbox.UCS2_count;
#if defined(__MMI_TOUCH_SCREEN__)
       
       
      if (!(MMI_EMS_inputbox.flags & UI_EMS_INPUT_BOX_VIEW_MODE))
      {
         if ( MMI_GUI_VIRTUAL_KEYBOARD_MAX_LANG == mmi_pen_editor_history_vk_lang_type() )
         {
            h->vk_lang_type = MMI_virtual_keyboard.lang_type;
         }
         else
         {
            h->vk_lang_type = mmi_pen_editor_history_vk_lang_type();
         }
      }
      else
      {
         h->vk_lang_type = MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY;
      }
#endif
   }
}

byte set_EMS_inputbox_category_history(word history_ID,byte *history_buffer,s16 *input_type)
{	if(history_buffer!=NULL)
	{	EMS_inputbox_category_history *h=(EMS_inputbox_category_history*)history_buffer;
		if(history_valid(h->history_ID,history_ID))
		{	MMI_EMS_inputbox.cursor_ax=h->cursor_ax;
			MMI_EMS_inputbox.cursor_ay=h->cursor_ay;
			MMI_EMS_inputbox.cursor_line_height=h->cursor_line_height;
			MMI_EMS_inputbox.display_y=h->display_y;
			MMI_EMS_inputbox.UCS2_count=h->UCS2_count;
			*input_type=h->input_type;
#if defined(__MMI_TOUCH_DIAL_SCREEN__)
    
			wgui_setup_virtual_keyboard(h->vk_lang_type);
#endif
			return(1);
		}
	}
	return(0);
}

#endif	/*	UI_DISABLE_EMS_CATEGORY_SCREENS	*/

extern void set_MMI_current_input_type(void);

S32 wgui_EMS_inputbox_information_bar_x;
S32 wgui_EMS_inputbox_information_bar_y;
S32 wgui_EMS_inputbox_information_bar_width;
S32 wgui_EMS_inputbox_information_bar_height;
byte wgui_EMS_inputbox_information_flag=0;
UI_image_type wgui_EMS_inputbox_information_icon=NULL;

void wgui_EMS_setup_input_information(S32 x,S32 y,S32 width,S32 height)
{	wgui_EMS_inputbox_information_bar_x=x;
	wgui_EMS_inputbox_information_bar_y=y;
	wgui_EMS_inputbox_information_bar_width=width;
	wgui_EMS_inputbox_information_bar_height=height;
}

void wgui_EMS_input_information_background(S32 x1,S32 y1,S32 x2,S32 y2)
{
   /* Now reads color from the theme structure	*/
   color_t c=*current_MMI_theme->information_bar_color;

   coolsand_UI_reset_clip();
    
   if ( wgui_is_wallpaper_on_bottom()==MMI_TRUE)
   {
      gdi_draw_solid_rect(x1,y1,x2,y2, GDI_COLOR_TRANSPARENT); 
   }
   else
   {
      coolsand_UI_fill_rectangle(x1,y1,x2,y2,c);
   }
}

 
 
 
#if defined __MMI_LANG_HINDI__  && defined __MMI_LANG_ARABIC__
extern S32 HINDI_PRESENT;
extern S32 ARABIC_PRESENT;
S32 IS_ARABIC = 0;
S32 IS_HINDI = 0;
#endif
 
#if defined(__MMI_TOUCH_SCREEN__)
extern UI_filled_area information_bar_BG_filler;
#endif
void wgui_EMS_show_input_mode(void)
{
   	color_t c=*current_MMI_theme->input_method_text_color;
#if !defined(__MMI_TOUCH_SCREEN__)
	stFontAttribute *f=&MMI_small_font;
#else
	stFontAttribute *f=&MMI_medium_font;
	S32 rect_height;
#endif
      S32 x;
      UI_string_type inputmode_string=(UI_string_type)"";
      S32 width, height;

#ifdef __MMI_MULTITAP_HINDI__
	U16 multitap_hindi_name[]={HINDI_IV_A,HINDI_IV_AA,HINDI_IV_I,0x0};
	//U16 name[] = {0x905,0x906,0x907,0x0};
#endif
	switch(MMI_current_input_mode)
	{
	 	case INPUT_MODE_MULTITAP_UPPERCASE_ABC:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_ABC_STRING_ID);
			  break;
		case INPUT_MODE_MULTITAP_LOWERCASE_ABC:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_abc_STRING_ID);
			  break;
#if defined(__MMI_GB__)
#if 0       //kecx modify on 20080701
		case INPUT_MODE_SM_PINYIN:
			inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_PINYIN_STRING_ID);
			break;
		case INPUT_MODE_SM_STROKE:
			inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_BIHUA_STRING_ID);
			break;
		case INPUT_MODE_SMART_UPPERCASE_ABC:
			inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_SABC_STRING_ID);
			break;
		case INPUT_MODE_SMART_LOWERCASE_ABC:
			inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_sabc_STRING_ID);
			break;
#else
		case INPUT_MODE_TR_BOPOMO:
			inputmode_string=(UI_string_type)(BPMF);
			break;
		case INPUT_MODE_SM_PINYIN:
			inputmode_string=(UI_string_type)(Pinyin);
			break;
		case INPUT_MODE_SM_STROKE:
			inputmode_string=(UI_string_type)(Bihua);
			break;
		case INPUT_MODE_SMART_UPPERCASE_ABC:
			inputmode_string = (UI_string_type)(SmartABC);
			break;
		case INPUT_MODE_SMART_LOWERCASE_ABC:
			inputmode_string = (UI_string_type)(Smartabc);
			break;
//beginning:guoyt added for adding Hindi,German and Spanish IME			
#if defined(__MMI_SMART_HINDI__)
		case INPUT_MODE_SMART_HINDI:
			inputmode_string = (UI_string_type)get_string(STR_INPUT_METHOD_SMART_HINDI);	
			break;
#endif			
/*Fix Bug#12417 for cs2324c by cong.li on 2009.05.21.
  *The smart input mode should match the smart ID string.
  */
#if defined(__MMI_SMART_GERMAN__)
		case INPUT_MODE_SMART_UPPERCASE_GERMAN:
			inputmode_string = (UI_string_type)get_string(STR_INPUT_METHOD_SMART_U_GERMAN);
			break;
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
			inputmode_string = (UI_string_type)get_string(STR_INPUT_METHOD_SMART_L_GERMAN);
			break;

#endif
#if defined(__MMI_SMART_SPANISH__)
		case INPUT_MODE_SMART_UPPERCASE_SPANISH:
			inputmode_string = (UI_string_type)get_string(STR_INPUT_METHOD_SMART_U_SPANISH);
			break;
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:
			inputmode_string = (UI_string_type)get_string(STR_INPUT_METHOD_SMART_L_SPANISH);
			break;
#endif	
//end:guoyt added for adding Hindi,German and Spanish IME
#if defined(__MMI_SMART_TELUGU__)
		case INPUT_MODE_SMART_TELUGU:
			inputmode_string = (UI_string_type)get_string(STR_INPUT_METHOD_SMART_TELUGU);	
			break;
#endif
#endif    //kecx modify on 20080701
#endif
 
#if defined (__MMI_MULTITAP_THAI__)
	 	case INPUT_MODE_MULTITAP_THAI:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_thai_STRING_ID);
			  break;
#endif
 
#if defined(__MMI_MULTITAP_SPANISH__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_SPANISH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_SPANISH_STRING_ID);
			  break;
		case INPUT_MODE_MULTITAP_LOWERCASE_SPANISH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_spanish_STRING_ID);
			  break;
#endif
                //qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
                case INPUT_MODE_MMI_MULTITAP_TAMIL:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_MENU_MULTITAP_TAMIL);
                break;
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
                case INPUT_MODE_MMI_MULTITAP_BENGALI:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_MENU_MULTITAP_BENGALI);
                break;
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
                case INPUT_MODE_MMI_MULTITAP_PUNJABI:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_MENU_MULTITAP_PUNJABI);
                break;
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
                case INPUT_MODE_MMI_MULTITAP_TELUGU:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_MULTITAP_TELUGU);
			  break;
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
                case INPUT_MODE_MMI_MULTITAP_KANNADA:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_MENU_MULTITAP_KANNADA);
                break;
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
                case INPUT_MODE_MMI_MULTITAP_MALAYALAM:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_MENU_MULTITAP_MALAYALAM);
                break;
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
                case INPUT_MODE_MMI_MULTITAP_ORIYA:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_MENU_MULTITAP_ORIYA);
                break;
#endif
                //qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
                case INPUT_MODE_MMI_MULTITAP_MARATHI:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_MENU_MULTITAP_MARATHI);
                break;
#endif
#if defined(__MMI_MULTITAP_URDU__)
                case INPUT_MODE_MMI_MULTITAP_URDU:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_MENU_MULTITAP_URDU);
                break;
#endif
 
#if defined(__MMI_MULTITAP_DANISH__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_DANISH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_DANISH_STRING_ID);
			  break;
		case INPUT_MODE_MULTITAP_LOWERCASE_DANISH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_danish_STRING_ID);
			  break;
#endif
 
#if defined(__MMI_MULTITAP_FRENCH__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_FRENCH_STRING_ID);
			  break;
		case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_french_STRING_ID);
			  break;
#endif
#if defined(__MMI_MULTITAP_MYANMAR__)
	 	case INPUT_MODE_MULTITAP_MYANMAR:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_MYANMAR_STRING_ID);
			  break;
#endif
#if defined(__MMI_SMART_FRENCH__)
	 	case INPUT_MODE_SMART_UPPERCASE_FRENCH:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_U_FRENCH);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_L_FRENCH);
			  break;
#endif
#if defined(__MMI_SMART_URDU__)
	 	case INPUT_MODE_SMART_URDU:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_URDU);
			  break;
#endif
#if defined(__MMI_SMART_PUNJABI__)
	 	case INPUT_MODE_SMART_PUNJABI:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_PUNJABI);
			  break;
#endif
#if defined(__MMI_SMART_BENGALI__)
	 	case INPUT_MODE_SMART_BENGALI:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_BENGALI);
			  break;
#endif
#if defined(__MMI_SMART_TAMIL__)
	 	case INPUT_MODE_SMART_TAMIL:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_TAMIL);
			  break;
#endif
#if defined(__MMI_SMART_TURKISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_TURKISH:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_U_TURKISH);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_TURKISH:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_L_TURKISH);
			  break;
#endif

#if defined(__MMI_SMART_INDONESIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_U_INDONESIAN);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_L_INDONESIAN);
			  break;
#endif

#if defined(__MMI_SMART_MALAY__)
	 	case INPUT_MODE_SMART_UPPERCASE_MALAY:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_U_MALAY);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_MALAY:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_L_MALAY);
			  break;
#endif

// beginning--added by kecx for adding italian on 20081223
#if defined(__MMI_SMART_ITALIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_U_ITALIAN);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
			  inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_L_ITALIAN);
			  break;
#endif
// end--added by kecx for adding italian on 20081223
#if defined(__MMI_MULTITAP_GERMAN__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_GERMAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_GERMAN_STRING_ID);
			  break;
		case INPUT_MODE_MULTITAP_LOWERCASE_GERMAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_german_STRING_ID);
			  break;
#endif
#if defined(__MMI_MULTITAP_ITALIAN__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_ITALIAN_STRING_ID);
			  break;
		case INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_italian_STRING_ID);
			  break;
#endif
#if defined(__MMI_MULTITAP_RUSSIAN__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_RUSSIAN_STRING_ID);
			  break;
		case INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_russian_STRING_ID);
			  break;
#endif
/*Wangzl Add Start For Ver: Multi_Language  on 2008-1-21 19:31 */
#if defined(__MMI_SMART_RUSSIAN__)
		case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
			inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_U_RUSSIAN);
			break;
		case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
			inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_L_RUSSIAN);
			break;
#endif

#if defined(__MMI_GB_SMART_PORTUGUESE__)
	case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE :
			inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_U_PORTUGUESE);
			break;
	case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
			inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_L_PORTUGUESE);
			break;
#endif

/*Wangzl Add End  For  Ver: Multi_Language  on 2008-1-21 19:31 */
#if defined(__MMI_MULTITAP_TURKISH__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_TURKISH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_TURKISH_STRING_ID);
			  break;
		case INPUT_MODE_MULTITAP_LOWERCASE_TURKISH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_turkish_STRING_ID);
			  break;
#endif
#if defined(__MMI_MULTITAP_PORTUGUESE__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_PORTUGUESE_STRING_ID);
			  break;
		case INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_portuguese_STRING_ID);
			  break;
#endif
#if defined(__MMI_MULTITAP_HINDI__)
		case INPUT_MODE_MULTITAP_HINDI:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_HINDI_STRING_ID);
			  break;
#endif

#if defined(__MMI_MULTITAP_INDONESIAN__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_INDONESIAN_STRING_ID);
			  break;
		case INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_indonesian_STRING_ID);
			  break;
#endif
#if defined(__MMI_MULTITAP_MALAY__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_MALAY:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_MALAY_STRING_ID);
			  break;
		case INPUT_MODE_MULTITAP_LOWERCASE_MALAY:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_malay_STRING_ID);
			  break;
#endif
#if defined(__MMI_MULTITAP_VIETNAMESE__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_VIETNAMESE_STRING_ID);
			  break;
		case INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_vietnamese_STRING_ID);
			  break;
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
		case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
			inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_U_VIETNAMESE);
			break;
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
			inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_L_VIETNAMESE);
			break;
#endif
//CSD end

		case INPUT_MODE_123:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_123_STRING_ID);
			  break;
#if !defined(__MMI_GB__)
	        case INPUT_MODE_SMART_UPPERCASE_ABC:
	                inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_ABC_STRING_ID);
	                break;
	        case INPUT_MODE_SMART_LOWERCASE_ABC:
	                inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_abc_STRING_ID);
	                break;
#endif
                case INPUT_MODE_SMART_THAI:
                        inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_THAI);
                        break;
                 
#if defined(__MMI_SMART_ARABIC__)
                case INPUT_MODE_SMART_ARABIC:
                        inputmode_string=(UI_string_type)get_string(STR_INPUT_METHOD_SMART_ARABIC);
                        //inputmode_string=(UI_string_type)SmartArabi;
                        break;
#endif
#if defined(__MMI_MULTITAP_ARABIC__)
				case INPUT_MODE_MULTITAP_ARABIC:
					inputmode_string = (UI_string_type)get_string(STR_INPUT_METHOD_MULTITAP_ARABIC);
					break;
#endif
#if defined(__MMI_SMART_PERSIAN__)
		case INPUT_MODE_SMART_PERSIAN:
			inputmode_string = (UI_string_type)get_string(STR_INPUT_METHOD_SMART_PERSIAN);
			break;
#endif
                //CSD end
 
#if defined(__MMI_T9_SPANISH__)||defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_SPANISH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_SPANISH_STRING_ID);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_spanish_STRING_ID);
			  break;
#endif
 
#if defined(__MMI_T9_DANISH__)||defined(__MMI_ZI_DANISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_DANISH: 
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_DANISH_STRING_ID);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_DANISH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_danish_STRING_ID);
			  break;
#endif
 
#if defined(__MMI_T9_FRENCH__)||defined(__MMI_ZI_CA_FRENCH__)||defined(__MMI_ZI_EU_FRENCH__)
	 	case INPUT_MODE_SMART_UPPERCASE_FRENCH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_FRENCH_STRING_ID);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_french_STRING_ID);
			  break;
#endif
#if defined(__MMI_T9_GERMAN__)||defined(__MMI_ZI_GERMAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_GERMAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_GERMAN_STRING_ID);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_german_STRING_ID);
			  break;
#endif
#if defined(__MMI_T9_ITALIAN__)||defined(__MMI_ZI_ITALIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_ITALIAN_STRING_ID);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_italian_STRING_ID);
			  break;
#endif
 
#if defined (__MMI_T9_RUSSIAN__)||defined(__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
//END 
	 	case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_RUSSIAN_STRING_ID);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_russian_STRING_ID);
			  break;
#endif
#if defined(__MMI_ZI_TURKISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_TURKISH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_TURKISH_STRING_ID);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_TURKISH:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_turkish_STRING_ID);
			  break;
#endif
 
#if defined(__MMI_T9_PORTUGUESE__)||defined(__MMI_ZI_EU_PORTUGUESE__)||defined(__MMI_ZI_SA_PORTUGUESE__)||defined(__MMI_ZI_BZ_PORTUGUESE__)
 
	 	case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_PORTUGUESE_STRING_ID);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_portuguese_STRING_ID);
			  break;
#endif
 
#if defined(__MMI_ZI_INDONESIAN__)||defined(__MMI_T9_INDONESIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_INDONESIAN_STRING_ID);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_indonesian_STRING_ID);
			  break;
#endif
 
#if defined(__MMI_ZI_MALAY__)
	 	case INPUT_MODE_SMART_UPPERCASE_MALAY:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_MALAY_STRING_ID);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_MALAY:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_malay_STRING_ID);
			  break;
#endif
 
#if defined(__MMI_ZI_VIETNAMESE__)||defined(__MMI_T9_VIETNAMESE__)
 
	 	case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_VIETNAMESE_STRING_ID);
			  break;
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
			  inputmode_string=(UI_string_type)get_string(WGUI_CATEGORY_vietnamese_STRING_ID);
			  break;
#endif
//CSD end
	}

	coolsand_UI_set_font(f);
	coolsand_UI_set_text_color(c);
	coolsand_UI_reset_text_clip();
	x=1;
 #if defined(__MMI_T9__) || defined(__MMI_ITAP__) || defined(__MMI_KA__)
    if(wgui_EMS_inputbox_information_icon==NULL)
    {
    	 
    	if(!r2lMMIFlag)
        x=2;
    else
	       x =  UI_device_width -2;
       //CSD end
    }
    else
    {
         
        coolsand_UI_measure_image(wgui_EMS_inputbox_information_icon, &width, &height );
        if(!r2lMMIFlag)
        {
#if 0//!defined(__MMI_TOUCH_SCREEN__)
	       coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+1,wgui_EMS_inputbox_information_icon,0);
#else
	       coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2,wgui_EMS_inputbox_information_icon,0);
#endif
        x=width+2;
    }
        else
        {
#if 0//!defined(__MMI_TOUCH_SCREEN__)
	        coolsand_UI_show_transparent_image(UI_device_width -1-width,wgui_EMS_inputbox_information_bar_y+1,wgui_EMS_inputbox_information_icon,0);
#else
	       coolsand_UI_show_transparent_image(UI_device_width -1-width,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2,wgui_EMS_inputbox_information_icon,0);
#endif
	        x =  UI_device_width -2-width;
        }
        //CSD end


    }
 #elif defined(__MMI_ZI__)
    if(wgui_EMS_inputbox_information_icon==NULL)
    {
    	 
    	if(!r2lMMIFlag)
        x=2;
    else
	       x =  UI_device_width -2;
       //CSD end
    }
    else
    {

         
        coolsand_UI_measure_image(get_image(WGUI_INPUT_INFORMATION_ICON1), &width, &height );
        if(!r2lMMIFlag)
        {
#if 0//!defined(__MMI_TOUCH_SCREEN__)
        	coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+1,get_image(WGUI_INPUT_INFORMATION_ICON1),0);
#else
        	coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2,get_image(WGUI_INPUT_INFORMATION_ICON1),0);
#endif
        x=width+2;

        }
        else
        {
#if 0//!defined(__MMI_TOUCH_SCREEN__)
        	coolsand_UI_show_transparent_image(UI_device_width -1-width,wgui_EMS_inputbox_information_bar_y+1,get_image(WGUI_INPUT_INFORMATION_ICON1),0);
#else
        	coolsand_UI_show_transparent_image(UI_device_width -1-width,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2,get_image(WGUI_INPUT_INFORMATION_ICON1),0);
#endif
	        x =  UI_device_width -2-width;

        }
        //CSD end


    }
 #else
    if(wgui_EMS_inputbox_information_icon==NULL)
    {
    	 
    	if(!r2lMMIFlag)
        x=2;
    else
	       x =  UI_device_width -2;
       //CSD end
    }
    else
    {
         
        coolsand_UI_measure_image(wgui_EMS_inputbox_information_icon, &width, &height );
        if(!r2lMMIFlag)
        {
#if 0//!defined(__MMI_TOUCH_SCREEN__)
 	       coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+1,wgui_EMS_inputbox_information_icon,0);
#else
        	 coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2,wgui_EMS_inputbox_information_icon,0);
#endif
        x=width+2;

        }
        else
        {
#if 0//!defined(__MMI_TOUCH_SCREEN__)
	        coolsand_UI_show_transparent_image(UI_device_width -1-width,wgui_EMS_inputbox_information_bar_y+1,wgui_EMS_inputbox_information_icon,0);
#else
        	  coolsand_UI_show_transparent_image(UI_device_width -1-width,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2,wgui_EMS_inputbox_information_icon,0);
#endif
	        x =  UI_device_width -2-width;

	}
        //CSD end

    }
 #endif
#if defined(__MMI_TOUCH_SCREEN__)
   coolsand_UI_measure_string(inputmode_string,&width, &height);
   rect_height = coolsand_UI_get_character_height();
#endif

	switch(MMI_current_input_mode)
	{
	 	case INPUT_MODE_MULTITAP_UPPERCASE_ABC:
		case INPUT_MODE_MULTITAP_LOWERCASE_ABC:
 
#if defined(__MMI_MULTITAP_THAI__)
		case INPUT_MODE_MULTITAP_THAI:
#endif
 
#if defined(__MMI_MULTITAP_SPANISH__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_SPANISH:
		case INPUT_MODE_MULTITAP_LOWERCASE_SPANISH:
#endif
                //qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
                case INPUT_MODE_MMI_MULTITAP_TAMIL:
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
                case INPUT_MODE_MMI_MULTITAP_BENGALI:
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
                case INPUT_MODE_MMI_MULTITAP_PUNJABI:
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
                case INPUT_MODE_MMI_MULTITAP_TELUGU:
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
                case INPUT_MODE_MMI_MULTITAP_KANNADA:
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
                case INPUT_MODE_MMI_MULTITAP_MALAYALAM:
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
                case INPUT_MODE_MMI_MULTITAP_ORIYA:
#endif
                //qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
                case INPUT_MODE_MMI_MULTITAP_MARATHI:
#endif
#if defined(__MMI_MULTITAP_URDU__)
                case INPUT_MODE_MMI_MULTITAP_URDU:
#endif
 
#if defined(__MMI_MULTITAP_DANISH__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_DANISH:
		case INPUT_MODE_MULTITAP_LOWERCASE_DANISH:
#endif
 
#if defined(__MMI_MULTITAP_FRENCH__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
		case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
#endif
#if defined(__MMI_MULTITAP_MYANMAR__)
	 	case INPUT_MODE_MULTITAP_MYANMAR:
#endif
#if defined(__MMI_SMART_FRENCH__)
	 	case INPUT_MODE_SMART_UPPERCASE_FRENCH:
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
#endif
#if defined(__MMI_SMART_URDU__)
	 	case INPUT_MODE_SMART_URDU:
#endif
#if defined(__MMI_SMART_PUNJABI__)
	 	case INPUT_MODE_SMART_PUNJABI:
#endif
#if defined(__MMI_SMART_BENGALI__)
	 	case INPUT_MODE_SMART_BENGALI:
#endif
#if defined(__MMI_SMART_TAMIL__)
	 	case INPUT_MODE_SMART_TAMIL:
#endif
#if defined(__MMI_SMART_TURKISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_TURKISH:
		case INPUT_MODE_SMART_LOWERCASE_TURKISH:
#endif
#if defined(__MMI_SMART_INDONESIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
		case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
#endif
#if defined(__MMI_SMART_MALAY__)
	 	case INPUT_MODE_SMART_UPPERCASE_MALAY:
		case INPUT_MODE_SMART_LOWERCASE_MALAY:
#endif
// beginning--added by kecx for adding italian on 20081223
#if defined(__MMI_SMART_ITALIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
#endif
// end--added by kecx for adding italian on 20081223
#if defined(__MMI_MULTITAP_GERMAN__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_GERMAN:
		case INPUT_MODE_MULTITAP_LOWERCASE_GERMAN:
#endif
#if defined(__MMI_MULTITAP_ITALIAN__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN:
		case INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN:
#endif
#if defined(__MMI_MULTITAP_RUSSIAN__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN:
		case INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN:
#endif
/*Wangzl Add Start For Ver: Multi_Language  on 2008-1-21 19:32 */
#if defined(__MMI_SMART_RUSSIAN__)
		case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
		case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
#endif
/*Wangzl Add End  For Ver: Multi_Language  on 2008-1-21 19:32 */
#if defined(__MMI_GB_SMART_PORTUGUESE__)
		case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE :
		case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
#endif

/*guoyt added for adding hindi,german,spanish lang ime,begin.....*/
#if defined(__MMI_SMART_HINDI__)
		case INPUT_MODE_SMART_HINDI:
#endif
#if defined(__MMI_SMART_GERMAN__)
		case INPUT_MODE_SMART_UPPERCASE_GERMAN:
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
#endif
#if defined(__MMI_SMART_SPANISH__)
		case INPUT_MODE_SMART_UPPERCASE_SPANISH:
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:
#endif
/*end:guoyt added for adding hindi,german,spanish lang ime*/
#if defined(__MMI_SMART_TELUGU__)
		case INPUT_MODE_SMART_TELUGU:
#endif
#if defined(__MMI_SMART_ARABIC__)
		case INPUT_MODE_SMART_ARABIC:
#endif
#if defined(__MMI_MULTITAP_ARABIC__)
		case INPUT_MODE_MULTITAP_ARABIC:
#endif
#if defined(__MMI_SMART_PERSIAN__)
		case INPUT_MODE_SMART_PERSIAN:
#endif
#if defined(__MMI_MULTITAP_TURKISH__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_TURKISH:
		case INPUT_MODE_MULTITAP_LOWERCASE_TURKISH:
#endif
#if defined(__MMI_MULTITAP_PORTUGUESE__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE:
		case INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE:
#endif
#if defined(__MMI_MULTITAP_HINDI__)
		case INPUT_MODE_MULTITAP_HINDI:
#endif

#if defined(__MMI_MULTITAP_INDONESIAN__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN:
		case INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN:
#endif
#if defined(__MMI_MULTITAP_MALAY__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_MALAY:
		case INPUT_MODE_MULTITAP_LOWERCASE_MALAY:
#endif
#if defined(__MMI_MULTITAP_VIETNAMESE__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE:
		case INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE:
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
		case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
#endif
//CSD end
#if defined(__MMI_GB__)
		case INPUT_MODE_TR_BOPOMO:
		case INPUT_MODE_SM_PINYIN:
		case INPUT_MODE_SM_STROKE:
		case INPUT_MODE_SMART_UPPERCASE_ABC:
		case INPUT_MODE_SMART_LOWERCASE_ABC:
#if defined(__MMI_SMART_THAI__)
		case INPUT_MODE_SMART_THAI:
#endif
#endif
		
		case INPUT_MODE_123:
//Fix Bug#12338 for cs2324c by cong.li on 2009.05.20.
#if 0			
 
			if(MMI_current_input_mode==INPUT_MODE_SMART_HINDI)//INPUT_MODE_MULTITAP_HINDI)		//guoyt added for adding smart hindi ime
				coolsand_UI_move_text_cursor(wgui_EMS_inputbox_information_bar_x+x,wgui_EMS_inputbox_information_bar_y);
			else
#endif				
#if !defined(__MMI_TOUCH_SCREEN__)
	coolsand_UI_measure_string(inputmode_string,&width, &height);
	 coolsand_UI_move_text_cursor(wgui_EMS_inputbox_information_bar_x+x,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2);
#else
{
		coolsand_UI_draw_filled_area(wgui_EMS_inputbox_information_bar_x+x+2, wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-rect_height)/2-1, wgui_EMS_inputbox_information_bar_x+x+4+width, wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height+rect_height)/2, &information_bar_BG_filler);
                coolsand_UI_move_text_cursor(wgui_EMS_inputbox_information_bar_x+x+3,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2);
}
#endif
 
	                coolsand_UI_print_text(inputmode_string);
	                break;
        #if defined (__MMI_T9__) || defined(__MMI_ITAP__) || defined(__MMI_KA__)
	        case INPUT_MODE_SMART_UPPERCASE_ABC:
	        case INPUT_MODE_SMART_LOWERCASE_ABC:
 
#if defined(__MMI_T9_SPANISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_SPANISH:
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:
#endif
 
#if defined(__MMI_T9_DANISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_DANISH:
		case INPUT_MODE_SMART_LOWERCASE_DANISH:
#endif
 
#if defined(__MMI_T9_FRENCH__)
	 	case INPUT_MODE_SMART_UPPERCASE_FRENCH:
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
#endif
#if defined(__MMI_T9_GERMAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_GERMAN:
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
#endif
#if defined(__MMI_T9_ITALIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
#endif
 

#if defined(__MMI_T9_VIETNAMESE__)
	 	case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
#endif

 
 
#if defined(__MMI_T9_INDONESIAN__)
		case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
		case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
#endif
 
//CSD end
 
#if defined(__MMI_T9_THAI__)
	 	case INPUT_MODE_SMART_THAI:
#endif
 
 
#if defined(__MMI_T9_RUSSIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
		case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
#endif
//END 

 
#if defined(__MMI_T9_PORTUGUESE__)
         case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE:
         case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
#endif
 
#if !defined(__MMI_TOUCH_SCREEN__)
	                 coolsand_UI_move_text_cursor(wgui_EMS_inputbox_information_bar_x+x,wgui_EMS_inputbox_information_bar_y+3);
#else
               coolsand_UI_move_text_cursor(wgui_EMS_inputbox_information_bar_x+x,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2);
#endif
	       	   coolsand_UI_print_text(inputmode_string);
	                 break;
        #elif defined(__MMI_ZI__)
	        case INPUT_MODE_SMART_UPPERCASE_ABC:
	        case INPUT_MODE_SMART_LOWERCASE_ABC:
 
#if defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_SPANISH:
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:
#endif
 
#if defined(__MMI_ZI_DANISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_DANISH:
		case INPUT_MODE_SMART_LOWERCASE_DANISH:
#endif
 
#if defined(__MMI_ZI_CA_FRENCH__)||defined(__MMI_ZI_EU_FRENCH__)
	 	case INPUT_MODE_SMART_UPPERCASE_FRENCH:
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
#endif
#if defined(__MMI_ZI_GERMAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_GERMAN:
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
#endif
#if defined(__MMI_ZI_ITALIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
#endif
#if defined(__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
		case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
#endif
#if defined(__MMI_ZI_TURKISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_TURKISH:
		case INPUT_MODE_SMART_LOWERCASE_TURKISH:
#endif
#if defined(__MMI_ZI_EU_PORTUGUESE__)||defined(__MMI_ZI_SA_PORTUGUESE__)||defined(__MMI_ZI_BZ_PORTUGUESE__)
	 	case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE:
		case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
#endif
#if defined(__MMI_ZI_INDONESIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
		case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
#endif
#if defined(__MMI_ZI_MALAY__)
	 	case INPUT_MODE_SMART_UPPERCASE_MALAY:
		case INPUT_MODE_SMART_LOWERCASE_MALAY:
#endif
#if defined(__MMI_ZI_VIETNAMESE__)
	 	case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
#endif
//CSD end
                case INPUT_MODE_SMART_THAI:
                 
#if defined(__MMI_SMART_ARABIC__)
                case INPUT_MODE_SMART_ARABIC:
                //CSD end

	               if(wgui_EMS_inputbox_information_icon==NULL)
	               {
	                    
	                   if(!r2lMMIFlag)
	                   x+=width+2;
	               else
	                   	   x -=  (width+2);
			     //CSD end
	               }
	               else
	               {

	                    
	                   coolsand_UI_measure_image(wgui_EMS_inputbox_information_icon, &width, &height );
	                   if(!r2lMMIFlag)
	                   {
#if !defined(__MMI_TOUCH_SCREEN__)
		                   coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+1,wgui_EMS_inputbox_information_icon,0);
#else
                   	       coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2,wgui_EMS_inputbox_information_icon,0);
#endif
		        	     x+=width+2;
	                   }
	                   else
	                   {
#if !defined(__MMI_TOUCH_SCREEN__)
		                   coolsand_UI_show_transparent_image(x-width,wgui_EMS_inputbox_information_bar_y+1,wgui_EMS_inputbox_information_icon,0);
#else
                   	       coolsand_UI_show_transparent_image(x-width,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2,wgui_EMS_inputbox_information_icon,0);
#endif
				     x -=  (width+2);
	                   }
			     //CSD end

	               }
	               coolsand_UI_move_text_cursor(wgui_EMS_inputbox_information_bar_x+x,wgui_EMS_inputbox_information_bar_y+3);
	      	        coolsand_UI_print_text(inputmode_string);
	               break;
#endif
#if defined(__MMI_SMART_PERSIAN__)
                case INPUT_MODE_SMART_PERSIAN:
                //CSD end

	               if(wgui_EMS_inputbox_information_icon==NULL)
	               {
	                    
	                   if(!r2lMMIFlag)
	                   x+=width+2;
	               else
	                   	   x -=  (width+2);
			     //CSD end
	               }
	               else
	               {

	                    
	                   coolsand_UI_measure_image(wgui_EMS_inputbox_information_icon, &width, &height );
	                   if(!r2lMMIFlag)
	                   {
#if !defined(__MMI_TOUCH_SCREEN__)
		                   coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+1,wgui_EMS_inputbox_information_icon,0);
#else
                   	       coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2,wgui_EMS_inputbox_information_icon,0);
#endif
		        	     x+=width+2;
	                   }
	                   else
	                   {
#if !defined(__MMI_TOUCH_SCREEN__)
		                   coolsand_UI_show_transparent_image(x-width,wgui_EMS_inputbox_information_bar_y+1,wgui_EMS_inputbox_information_icon,0);
#else
                   	       coolsand_UI_show_transparent_image(x-width,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2,wgui_EMS_inputbox_information_icon,0);
#endif
				     x -=  (width+2);
	                   }
			     //CSD end

	               }
	               coolsand_UI_move_text_cursor(wgui_EMS_inputbox_information_bar_x+x,wgui_EMS_inputbox_information_bar_y+3);
	      	        coolsand_UI_print_text(inputmode_string);
	               break;
#endif
	        case INPUT_MODE_TR_MULTITAP_BOPOMO:
	        case INPUT_MODE_SM_MULTITAP_PINYIN:
	        case INPUT_MODE_SM_STROKE:
	        case INPUT_MODE_TR_STROKE:
	               if(wgui_EMS_inputbox_information_icon==NULL)
	               {
	                    
	                   if(!r2lMMIFlag)
	                   x+=width+2;
	               else
	                   	   x -=  (width+2);
			     //CSD end
	               }
	               else
	               {
	                    
	                   coolsand_UI_measure_image(wgui_EMS_inputbox_information_icon, &width, &height );
	                   if(!r2lMMIFlag)
	               {
#if !defined(__MMI_TOUCH_SCREEN__)
	                   coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+1,wgui_EMS_inputbox_information_icon,0);
#else
               	 	coolsand_UI_draw_filled_area(x, wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-rect_height)/2-1, x+4+width, wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height+rect_height)/2, &information_bar_BG_filler);
                   	coolsand_UI_show_transparent_image(x+2,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2+1,wgui_EMS_inputbox_information_icon,0);
#endif
			     	     x+=width+2;
	                   }
	                   else
	                   {
#if !defined(__MMI_TOUCH_SCREEN__)
	          	            coolsand_UI_show_transparent_image(x-width,wgui_EMS_inputbox_information_bar_y+1,wgui_EMS_inputbox_information_icon,0);
#else
               	 	coolsand_UI_draw_filled_area(x-width, wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-rect_height)/2-1, x+4+width, wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height+rect_height)/2, &information_bar_BG_filler);
                   	coolsand_UI_show_transparent_image(x-width+2,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2+1,wgui_EMS_inputbox_information_icon,0);
#endif
				     x -=  (width+2);
	                   }
			     //CSD end

	               }
	               break;
	        case INPUT_MODE_TR_BOPOMO:
	        case INPUT_MODE_SM_PINYIN:
	               coolsand_UI_measure_image(get_image(WGUI_ZI_EZITEXT_LOGO), &width, &height );
                       
                     if(!r2lMMIFlag)
                     {
#if !defined(__MMI_TOUCH_SCREEN__)
	                 coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+1,get_image(WGUI_ZI_EZITEXT_LOGO),0);
#else
              	     coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2,get_image(WGUI_ZI_EZITEXT_LOGO),0);
#endif
                        x+=width+2;
                     }
                     else
                     {
#if !defined(__MMI_TOUCH_SCREEN__)
	                 coolsand_UI_show_transparent_image(x-width,wgui_EMS_inputbox_information_bar_y+1,get_image(WGUI_ZI_EZITEXT_LOGO),0);
#else
              	     coolsand_UI_show_transparent_image(x-width,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2,get_image(WGUI_ZI_EZITEXT_LOGO),0);
#endif
                    	   x -=  (width+2);
                     }
		       //CSD end

	               if(wgui_EMS_inputbox_information_icon==NULL)
	               {
	                    
	                   if(!r2lMMIFlag)
	                   x+=width+2;
	               else
	                   	   x -=  (width+2);
			     //CSD end
	               }
	               else
	               {
	                    
	                   coolsand_UI_measure_image(wgui_EMS_inputbox_information_icon, &width, &height );
	                   if(!r2lMMIFlag)
	               {
#if !defined(__MMI_TOUCH_SCREEN__)
	                   coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+1,wgui_EMS_inputbox_information_icon,0);
#else
               	 coolsand_UI_draw_filled_area(x, wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-rect_height)/2-1, x+4+width, wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height+rect_height)/2, &information_bar_BG_filler);
                      coolsand_UI_show_transparent_image(x,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2+1,wgui_EMS_inputbox_information_icon,0);
#endif
			     	     x+=width+2;
	                   }
	                   else
	                   {
#if !defined(__MMI_TOUCH_SCREEN__)
		                   coolsand_UI_show_transparent_image(x-width,wgui_EMS_inputbox_information_bar_y+1,wgui_EMS_inputbox_information_icon,0);
#else
               	 coolsand_UI_draw_filled_area(x-width, wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-rect_height)/2-1, x+4+width, wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height+rect_height)/2, &information_bar_BG_filler);
                      coolsand_UI_show_transparent_image(x-width,wgui_EMS_inputbox_information_bar_y+(wgui_inputbox_information_bar_height-height)/2+1,wgui_EMS_inputbox_information_icon,0);
#endif
				     x -=  (width+2);
	                   }
			     //CSD end

	               }
	               break;
         #endif
	}

}
//CSD end
void wgui_EMS_redraw_remaining_characters_display(void)
{
#ifndef __MMI_HIDE_EMS_REMAINING_CHAR__

	color_t				c;
#ifdef __MMI_MAINLCD_220X176__

       color_t temp_c = {255,255,41,100};
#endif

    
   S32                 x1;
	S32			        x2;
	S32					y1=wgui_EMS_inputbox_information_bar_y;
	S32					y2=wgui_EMS_inputbox_information_bar_y+wgui_EMS_inputbox_information_bar_height-1;
	//CSD end
#if !defined(__MMI_TOUCH_SCREEN__)
	stFontAttribute		*f=&MMI_small_font;
	S32					w,h,n,max_n;
#else
	stFontAttribute		*f=&MMI_medium_font;
	S32					w,h,n,max_n;
#endif
	UI_character_type	str[32];
	TBM_ENTRY(0x2860);


	  
	 if(!r2lMMIFlag)
	 {
#ifdef __MMI_MAINLCD_220X176__
	       x1=wgui_EMS_inputbox_information_bar_x+(wgui_EMS_inputbox_information_bar_width>>1)+ 40;
	       x2=wgui_EMS_inputbox_information_bar_x+wgui_EMS_inputbox_information_bar_width-1;
#else

	       x1=wgui_EMS_inputbox_information_bar_x+(wgui_EMS_inputbox_information_bar_width>>1)+ 10;
	       x2=wgui_EMS_inputbox_information_bar_x+wgui_EMS_inputbox_information_bar_width-1;
#endif

	 }
	 else
	 {
	  	x1=wgui_EMS_inputbox_information_bar_x;
	  	x2=wgui_EMS_inputbox_information_bar_x+(wgui_EMS_inputbox_information_bar_width/2)-1;

	 }
	  //CSD end


#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

#if(UI_DISABLE_EMS_INPUT_BOX)
#ifdef __UCS2_ENCODING
	n=(MMI_multiline_inputbox.text_length>>1)-1;
	if(MMI_multiline_inputbox.UCS2_count>0) max_n=268;
	else max_n=612;
#endif
#ifdef __ASCII
	n=MMI_multiline_inputbox.text_length-1;
	if(MMI_multiline_inputbox.UCS2_count>0) max_n=268;
	else max_n=612;
#endif
#else
	/* reassign MMI_EMS_inputbox.available_length because it might be one byte difference per segment for EMS 5.1 */
	
//qiff add for bug 8957
	gui_EMS_change_DCS(&MMI_EMS_inputbox);
/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-17 15:22 */
#if defined(__MMI_GB__)
	if (MMI_EMS_inputbox.UCS2_count > 0)
	{
		MMI_EMS_inputbox.data->Reminder.totalRemainingOctet = LargestByteNumberInSms - MMI_EMS_inputbox.data->textLength;// - 2 * InsertEnter;
	}
	else if (MMI_EMS_inputbox.UCS2_count == 0)
	{
		MMI_EMS_inputbox.data->Reminder.totalRemainingOctet = LargestByteNumberInSms - MMI_EMS_inputbox.data->textLength / 2 ;//- InsertEnter;
	}
#endif

#if 0 
	n=MMI_EMS_inputbox.available_length-EMS_get_remaining_length(MMI_EMS_inputbox.data); // 输入的字符长度
#else
	n = MMI_EMS_inputbox.available_length - (EMS_get_remaining_length(MMI_EMS_inputbox.data) >> 1); // 输入的字符长度
#endif	
/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-17 15:22 */
	if(MMI_EMS_inputbox.UCS2_count>0)
	{	
		n = MMI_EMS_inputbox.available_length - (EMS_get_remaining_length(MMI_EMS_inputbox.data) >> 1);
	}
	else if(MMI_EMS_inputbox.GSM_ext_count>0)
	{	
	/*zhoumn Modify Start For 5565 Ver: TBM_780  on 2007-8-9 15:6 */
	#if 0
		n = MMI_EMS_inputbox.available_length - EMS_get_remaining_length(MMI_EMS_inputbox.data);
	#else
		n = MMI_EMS_inputbox.available_length - (EMS_get_remaining_length(MMI_EMS_inputbox.data) >> 1);
	#endif
	/*zhoumn Modify End  For 5565 Ver: TBM_780  on 2007-8-9 15:6 */
	}
	 max_n = MMI_EMS_inputbox.available_length; //这个变量应该是根据纯英文和非纯英文两种情况来分别更新和维护的.
#endif
	wgui_EMS_input_information_background(x1,y1,x2,y2);
	coolsand_UI_reset_text_clip();
	coolsand_UI_sprintf(str,"%d/%d",n,max_n);
#ifdef __MMI_MAINLCD_220X176__
       c = temp_c;
#else
	c=*current_MMI_theme->remaining_length_text_color;
#endif
	coolsand_UI_set_font(f);
	coolsand_UI_set_text_color(c);
	coolsand_UI_set_text_clip(0,0,UI_device_width-1,UI_device_height-1);
#if 0//!defined(__MMI_TOUCH_SCREEN__)
	w=coolsand_UI_get_string_width(str);
#else
	coolsand_UI_measure_string(str,&w,&h);
#endif

	 
	if(!r2lMMIFlag)
#if 0//!defined(__MMI_TOUCH_SCREEN__)
	coolsand_UI_move_text_cursor(x2-2-w,y1+2);
#else
	coolsand_UI_move_text_cursor(x2-2-w,y1+(wgui_inputbox_information_bar_height-h)/2);
#endif
	else
#if 0//!defined(__MMI_TOUCH_SCREEN__)
		coolsand_UI_move_text_cursor(x1+2+w,y1+2);
#else
       	coolsand_UI_move_text_cursor(x1+2+w,y1+(wgui_inputbox_information_bar_height-h)/2);
#endif
       //CSD end
	coolsand_UI_print_text(str);
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(x1,y1,x2,y2);
#endif
#endif
	TBM_EXIT(0x2860);
}

void wgui_EMS_redraw_input_information_bar(void)
{
	S32 x1=wgui_EMS_inputbox_information_bar_x;
	S32 y1=wgui_EMS_inputbox_information_bar_y;
	S32 x2=wgui_EMS_inputbox_information_bar_x+wgui_EMS_inputbox_information_bar_width-1;
	S32 y2=wgui_EMS_inputbox_information_bar_y+wgui_EMS_inputbox_information_bar_height-1;
	coolsand_UI_lock_double_buffer();
	wgui_EMS_input_information_background(x1,y1,x2,y2);
	wgui_EMS_show_input_mode();
	wgui_EMS_redraw_remaining_characters_display();
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(x1,y1,x2,y2);
}

void (*category28_RSK_function)(void)=NULL;
byte			category28_RSK_label_clear=0;
UI_string_type	category28_RSK_label_string;
UI_image_type	category28_RSK_label_icon;
extern s16		MMI_current_input_type;

void SetCategory28RightSoftkeyFunction(void (*f)(void),MMI_key_event_type k)
{	UI_UNUSED_PARAMETER(k);
	category28_RSK_function=f;
}

void handle_category28_right_softkey_down(void)
{
   disable_active_multitap();
#if(UI_DISABLE_EMS_INPUT_BOX)
   if(MMI_multiline_inputbox.text_length>1)
   {
      multiline_inputbox_delete_character();
   }
#else
   if(!coolsand_UI_EMS_inputbox_empty(&MMI_EMS_inputbox))
   {
      EMS_inputbox_backspace();
   }
#endif
}

void handle_category28_right_softkey_up(void)
{
#if(UI_DISABLE_EMS_INPUT_BOX)
	if((MMI_multiline_inputbox.text_length<=1) && (!category28_RSK_label_clear))
	{	if(category28_RSK_function!=NULL) category28_RSK_function();
	}
#else
	if(category28_RSK_function!=NULL) category28_RSK_function();
#endif
}

void register_category28_handle_right_softkey_up(void)
{	set_right_softkey_function(handle_category28_right_softkey_up,KEY_EVENT_UP);
}

void register_category28_handle_clear_key_up(void)
{	SetKeyHandler(handle_category28_right_softkey_up, KEY_CLEAR, KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: handle_EditGoBackHistory

  	PURPOSE				: Go back screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: freeIndex

	RETURNS				: void

 

**************************************************************/
void handle_EditGoBackHistory(void)
{
	GoBacknHistory(0);
}
void handle_category28_right_softkey_long_press(void)
{
#if defined(__MMI_GB__)
	InsertEnter = 0;
	if (gbstate == GBIMESTATE_INPUT)
	{
		GbKeyRSKLongPressHandler();
	}
	else
#endif        
	{
#if(UI_DISABLE_EMS_INPUT_BOX)
		multiline_inputbox_delete_all_characters();
#else
		EMS_inputbox_delete_all();
#endif
		set_right_softkey_function(handle_EditGoBackHistory,KEY_EVENT_DOWN);
	}
	
}

void handle_category28_input(void)
{
	EMS_inputbox_has_changed=1;
#if(UI_DISABLE_EMS_INPUT_BOX)
	if(MMI_multiline_inputbox.text_length>1)
	{
	    #if defined(__MMI_WITH_C_KEY__)

           #else
	       if(category28_RSK_label_clear==0)
		{
			set_right_softkey_label((UI_string_type)get_string(WGUI_CATEGORY_CLEAR_STRING_ID));
			set_right_softkey_icon(NULL);
			redraw_right_softkey();
			category28_RSK_label_clear=1;
			set_right_softkey_function(UI_dummy_function,KEY_EVENT_UP);
		}
	    #endif
	}
	else
	{
            #if defined(__MMI_WITH_C_KEY__)
            #else
	       if(category28_RSK_label_clear==1)
		{
			set_right_softkey_label(category28_RSK_label_string);
			set_right_softkey_icon(category28_RSK_label_icon);
			redraw_right_softkey();
			category28_RSK_label_clear=0;
			set_right_softkey_function(register_category28_handle_right_softkey_up,KEY_EVENT_UP);
		}
	     #endif
	}
#else
	if(MMI_EMS_inputbox.data==NULL) return;
	if(!coolsand_UI_EMS_inputbox_empty(&MMI_EMS_inputbox))
	{
	    #if defined(__MMI_WITH_C_KEY__)
	       if(category28_RSK_label_clear==0)
		{
		       category28_RSK_label_clear=1;
		      	SetKeyHandler(handle_category28_right_softkey_down,KEY_CLEAR,KEY_EVENT_DOWN);
		      	SetKeyHandler(UI_dummy_function,KEY_CLEAR,KEY_EVENT_UP);
		       SetKeyHandler(handle_category28_right_softkey_long_press,KEY_CLEAR,KEY_LONG_PRESS);
		}
           #else
	       if(category28_RSK_label_clear==0)
		{
			set_right_softkey_label((UI_string_type)get_string(WGUI_CATEGORY_CLEAR_STRING_ID));
			set_right_softkey_icon(NULL);
			redraw_right_softkey();
			category28_RSK_label_clear=1;
			set_right_softkey_function(UI_dummy_function,KEY_EVENT_UP);
			set_right_softkey_function(handle_category28_right_softkey_down,KEY_EVENT_DOWN);
#ifdef __MMI_FULL_KEYPAD__
			SetKeyHandler(handle_category28_right_softkey_down, KEY_BACKSPACE1, KEY_EVENT_DOWN);
			SetKeyHandler(UI_dummy_function, KEY_BACKSPACE1, KEY_EVENT_UP);
			SetKeyHandler(handle_category28_right_softkey_long_press, KEY_BACKSPACE1, KEY_EVENT_LONG_PRESS);
#endif
			
		}
	    #endif

	}
	else
	{
	    #if defined(__MMI_WITH_C_KEY__)
	    	if(category28_RSK_label_clear==1)
		{
		       category28_RSK_label_clear=0;
		      	SetKeyHandler(UI_dummy_function,KEY_CLEAR,KEY_EVENT_DOWN);
		      	SetKeyHandler(register_category28_handle_clear_key_up,KEY_CLEAR,KEY_EVENT_UP);
		       SetKeyHandler(UI_dummy_function,KEY_CLEAR,KEY_LONG_PRESS);
		}
           #else
	    	if(category28_RSK_label_clear==1)
		{
			set_right_softkey_label(category28_RSK_label_string);
			set_right_softkey_icon(category28_RSK_label_icon);
			redraw_right_softkey();
			category28_RSK_label_clear=0;
			set_right_softkey_function(register_category28_handle_right_softkey_up,KEY_EVENT_UP);
#ifdef __MMI_FULL_KEYPAD__
			SetKeyHandler(UI_dummy_function, KEY_BACKSPACE1, KEY_EVENT_DOWN);
			SetKeyHandler(register_category28_handle_clear_key_up, KEY_BACKSPACE1, KEY_EVENT_UP);
			SetKeyHandler(UI_dummy_function, KEY_BACKSPACE1, KEY_EVENT_LONG_PRESS);
#endif
		}
	    #endif

	}
#endif
}

 
#ifdef __MMI_MULTITAP_THAI__
void handle_category28_change_input_mode_bymultitapthai(void)
{
	if(Get_inputbox_update_states() == 1)
	{
		coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
		wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
	}
	handle_category28_change_input_mode();
}
#endif

//CSD modify for Thai
 
//extern GBIMESTATE gbstate; //deleted by zhoumn@2007/07/31
void handle_category28_change_input_mode(void)
{	
	disable_active_multitap();
	change_input_mode();
	set_MMI_current_input_type();
	switch(MMI_current_input_mode)
	{
	case INPUT_MODE_MULTITAP_UPPERCASE_ABC:
	case INPUT_MODE_MULTITAP_LOWERCASE_ABC:
 
#if defined(__MMI_MULTITAP_THAI__)
		case INPUT_MODE_MULTITAP_THAI:
#endif
 
#if defined(__MMI_MULTITAP_SPANISH__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_SPANISH:
		case INPUT_MODE_MULTITAP_LOWERCASE_SPANISH:
#endif
                //qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
                case INPUT_MODE_MMI_MULTITAP_TAMIL:
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
                case INPUT_MODE_MMI_MULTITAP_BENGALI:
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
                case INPUT_MODE_MMI_MULTITAP_PUNJABI:
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
                case INPUT_MODE_MMI_MULTITAP_TELUGU:
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
                case INPUT_MODE_MMI_MULTITAP_KANNADA:
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
                case INPUT_MODE_MMI_MULTITAP_MALAYALAM:
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
                case INPUT_MODE_MMI_MULTITAP_ORIYA:
#endif
                //qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
                case INPUT_MODE_MMI_MULTITAP_MARATHI:
#endif
#if defined(__MMI_MULTITAP_URDU__)
                case INPUT_MODE_MMI_MULTITAP_URDU:
#endif
 
#if defined(__MMI_MULTITAP_DANISH__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_DANISH:
		case INPUT_MODE_MULTITAP_LOWERCASE_DANISH:
#endif
 
#if defined(__MMI_MULTITAP_FRENCH__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
		case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
#endif
#if defined(__MMI_MULTITAP_MYANMAR__)
	 	case INPUT_MODE_MULTITAP_MYANMAR:
#endif
#if defined(__MMI_SMART_FRENCH__)
	 	case INPUT_MODE_SMART_UPPERCASE_FRENCH:
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
#endif
#if defined(__MMI_SMART_URDU__)
	 	case INPUT_MODE_SMART_URDU:
#endif
#if defined(__MMI_SMART_PUNJABI__)
	 	case INPUT_MODE_SMART_PUNJABI:
#endif
#if defined(__MMI_SMART_BENGALI__)
	 	case INPUT_MODE_SMART_BENGALI:
#endif
#if defined(__MMI_SMART_TAMIL__)
	 	case INPUT_MODE_SMART_TAMIL:
#endif
#if defined(__MMI_SMART_TURKISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_TURKISH:
		case INPUT_MODE_SMART_LOWERCASE_TURKISH:
#endif
#if defined(__MMI_SMART_INDONESIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
		case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
#endif
#if defined(__MMI_SMART_MALAY__)
	 	case INPUT_MODE_SMART_UPPERCASE_MALAY:
		case INPUT_MODE_SMART_LOWERCASE_MALAY:
#endif
// beginning--added by kecx for adding italian on 20081223
#if defined(__MMI_SMART_ITALIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
#endif
// end--added by kecx for adding italian on 20081223
#if defined(__MMI_MULTITAP_GERMAN__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_GERMAN:
		case INPUT_MODE_MULTITAP_LOWERCASE_GERMAN:
#endif
#if defined(__MMI_MULTITAP_ITALIAN__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN:
		case INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN:
#endif
#if defined(__MMI_MULTITAP_RUSSIAN__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN:
		case INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN:
#endif
/*Wangzl Add Start For Ver: Multi_Language  on 2008-1-21 19:32 */
#if defined(__MMI_SMART_RUSSIAN__)
		case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
		case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
#endif
/*Wangzl Add End  For Ver: Multi_Language  on 2008-1-21 19:32 */
#if defined(__MMI_GB_SMART_PORTUGUESE__)
		case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE :
		case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
#endif

#if defined(__MMI_SMART_ARABIC__)
		case INPUT_MODE_SMART_ARABIC:
#endif
#if defined(__MMI_SMART_PERSIAN__)
		case INPUT_MODE_SMART_PERSIAN:
#endif
#if defined(__MMI_MULTITAP_TURKISH__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_TURKISH:
		case INPUT_MODE_MULTITAP_LOWERCASE_TURKISH:
#endif
#if defined(__MMI_MULTITAP_PORTUGUESE__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE:
		case INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE:
#endif
#if defined(__MMI_MULTITAP_HINDI__)
	case INPUT_MODE_MULTITAP_HINDI:
#endif

#if defined(__MMI_MULTITAP_INDONESIAN__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN:
		case INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN:
#endif
#if defined(__MMI_MULTITAP_MALAY__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_MALAY:
		case INPUT_MODE_MULTITAP_LOWERCASE_MALAY:
#endif
#if defined(__MMI_MULTITAP_VIETNAMESE__)
	 	case INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE:
		case INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE:
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
		case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
#endif
//CSD end
#if defined(__MMI_GB__)
//beginning:guoyt added for adding Hindi,German and Spanish IME
#if defined(__MMI_SMART_HINDI__)
		case INPUT_MODE_SMART_HINDI:			
#endif
#if defined(__MMI_SMART_GERMAN__)
		case INPUT_MODE_SMART_UPPERCASE_GERMAN:			
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:			
#endif
#if defined(__MMI_SMART_SPANISH__)								
		case INPUT_MODE_SMART_UPPERCASE_SPANISH:
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:
#endif
//end:guoyt added for adding Hindi,German and Spanish IME
#if defined(__MMI_SMART_TELUGU__)
		case INPUT_MODE_SMART_TELUGU:			
#endif
		case INPUT_MODE_SM_PINYIN:
              case INPUT_MODE_TR_BOPOMO :
		case INPUT_MODE_SM_STROKE:
		case INPUT_MODE_SMART_UPPERCASE_ABC:
		case INPUT_MODE_SMART_LOWERCASE_ABC:
#if defined(__MMI_SMART_THAI__)
		case INPUT_MODE_SMART_THAI:
#endif
#endif
        case INPUT_MODE_123:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
        #if defined(__MMI_T9__) || defined(__MMI_ITAP__) || defined(__MMI_KA__)
         case INPUT_MODE_TR_MULTITAP_BOPOMO :
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_MULTITAP_BPMF_IMG);
               break;
        case INPUT_MODE_SM_MULTITAP_PINYIN :
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_MULTITAP_PINYIN_IMG);
               break;

        case INPUT_MODE_TR_BOPOMO :
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_BPMF_IMG);
               break;
        case INPUT_MODE_SM_PINYIN :
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_PINYIN_IMG);
               break;
        case INPUT_MODE_TR_STROKE :
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_TR_STROKE_IMG);
               break;
        case INPUT_MODE_SM_STROKE :
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SM_STROKE_IMG);
               break;
        case INPUT_MODE_SMART_UPPERCASE_ABC:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
               break;
        case INPUT_MODE_SMART_LOWERCASE_ABC:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
               break;
 
#if defined(__MMI_T9_SPANISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_SPANISH:
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
               break;
#endif
 
#if defined(__MMI_T9_DANISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_DANISH:
		case INPUT_MODE_SMART_LOWERCASE_DANISH:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
               break;
#endif
 
#if defined(__MMI_T9_FRENCH__)
	 	case INPUT_MODE_SMART_UPPERCASE_FRENCH:
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
               break;
#endif
#if defined(__MMI_T9_GERMAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_GERMAN:
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
               break;
#endif
#if defined(__MMI_T9_ITALIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
               break;
#endif
 

#if defined(__MMI_T9_VIETNAMESE__)
	 	case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
               break;
#endif

 

 
#if defined(__MMI_T9_INDONESIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
		case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
               break;
#endif
 
//CSD end
 
#if defined (__MMI_T9_RUSSIAN__)
        case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
        case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
               break;
#endif
//END 
 
#if defined (__MMI_T9_THAI__)
	 	case INPUT_MODE_SMART_THAI:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
               break;
#endif

 
#if defined(__MMI_T9_PORTUGUESE__)
        case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE:
        case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
               break;
#endif
 

        #elif defined(__MMI_ZI__)
        case INPUT_MODE_TR_MULTITAP_BOPOMO :
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_BPMF_LOGO);
               break;
        case INPUT_MODE_SM_MULTITAP_PINYIN :
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_PINYIN_LOGO);
               break;
        case INPUT_MODE_TR_BOPOMO :
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_BPMF_LOGO);
               break;
        case INPUT_MODE_SM_PINYIN :
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_PINYIN_LOGO);
               break;
        case INPUT_MODE_TR_STROKE :
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_TR_STROKE_LOGO);
               break;
        case INPUT_MODE_SM_STROKE :
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_SM_STROKE_LOGO);
               break;
        case INPUT_MODE_SMART_UPPERCASE_ABC:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
        case INPUT_MODE_SMART_LOWERCASE_ABC:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
        case INPUT_MODE_SMART_THAI:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
         
        case INPUT_MODE_SMART_ARABIC:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
        //CSD end

 
#if defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_SPANISH:
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
#endif
 
#if defined(__MMI_ZI_DANISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_DANISH:
		case INPUT_MODE_SMART_LOWERCASE_DANISH:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
#endif
 
#if defined(__MMI_ZI_CA_FRENCH__)||defined(__MMI_ZI_EU_FRENCH__)
	 	case INPUT_MODE_SMART_UPPERCASE_FRENCH:
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
#endif
#if defined(__MMI_ZI_GERMAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_GERMAN:
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
#endif
#if defined(__MMI_ZI_ITALIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
#endif
#if defined(__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
        case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
        case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
#endif
#if defined(__MMI_ZI_TURKISH__)
	 	case INPUT_MODE_SMART_UPPERCASE_TURKISH:
		case INPUT_MODE_SMART_LOWERCASE_TURKISH:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
#endif
#if defined(__MMI_ZI_EU_PORTUGUESE__)||defined(__MMI_ZI_SA_PORTUGUESE__)||defined(__MMI_ZI_BZ_PORTUGUESE__)
        case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE:
        case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
#endif
#if defined(__MMI_ZI_INDONESIAN__)
	 	case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
		case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
#endif
#if defined(__MMI_ZI_MALAY__)
	 	case INPUT_MODE_SMART_UPPERCASE_MALAY:
		case INPUT_MODE_SMART_LOWERCASE_MALAY:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
#endif
#if defined(__MMI_ZI_VIETNAMESE__)
	 	case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
               break;
#endif
//CSD end
        #endif

		default:
				wgui_EMS_inputbox_information_icon=NULL;
				break;
	}
	wgui_EMS_redraw_input_information_bar();
}
//CSD end
void RedrawCategory28Screen(void)
{

	coolsand_UI_lock_double_buffer();

	clear_screen();

	show_title_status_icon();
	draw_title();
#if(UI_DISABLE_EMS_INPUT_BOX)
	show_multiline_inputbox();
#else
	show_EMS_inputbox();
#endif
	wgui_EMS_redraw_input_information_bar();
#if defined(__MMI_TOUCH_SCREEN__)
	wgui_show_virtual_keyboard();
#endif
	show_softkey_background();
	show_left_softkey();
	show_right_softkey();
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
	 
	if( RedrawSpellingOrCandidateBoxesFunction)
		   RedrawSpellingOrCandidateBoxesFunction();
	//CSD end


}

#if(EMS_USE_STUB_MENU)

void EntryEMSEditOptionsMenu(void)
{
}

#endif

byte wgui_EMS_inputbox_input_mode_changed=0;
byte wgui_EMS_inputbox_input_type=0;

void wgui_EMS_inputbox_change_input_mode_multitap_ABC(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_UPPERCASE;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_abc(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_LOWERCASE;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_numeric(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_NUMERIC_CHANGEABLE;
	wgui_EMS_inputbox_input_mode_changed=1;
}

// add by yaosq  20061011
#if defined(__MMI_GB__)
void wgui_EMS_inputbox_change_input_mode_multitap_GBStroke(void)
{	wgui_EMS_inputbox_input_type = INPUT_TYPE_SM_STROKE;
	wgui_EMS_inputbox_input_mode_changed = 1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_GBPinYin(void)
{	wgui_EMS_inputbox_input_type = INPUT_TYPE_SM_PINYIN;
	wgui_EMS_inputbox_input_mode_changed = 1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_GB_TR_BPMF(void)
{	wgui_EMS_inputbox_input_type = INPUT_TYPE_TR_BOPOMO;
	wgui_EMS_inputbox_input_mode_changed = 1;
}
void wgui_EMS_inputbox_change_input_mode_multitap_GB_TR_Stroke(void)
{	wgui_EMS_inputbox_input_type = INPUT_TYPE_TR_STROKE;
	wgui_EMS_inputbox_input_mode_changed = 1;
}

/**************************************************************

	FUNCTION NAME		: wgui_EMS_inputbox_change_input_mode_multitap_GB_SABC

  	PURPOSE				: add a flag of change IME

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: Null

	RETURNS				: void

 

**************************************************************/
void wgui_EMS_inputbox_change_input_mode_multitap_GB_SABC(void)
{	wgui_EMS_inputbox_input_type = INPUT_TYPE_SMART_UPPERCASE_ABC;
	wgui_EMS_inputbox_input_mode_changed = 1;
}
/**************************************************************

	FUNCTION NAME		: wgui_EMS_inputbox_change_input_mode_multitap_GBSabc

  	PURPOSE				: add a flag of change IME

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: Null

	RETURNS				: void

 

**************************************************************/

void wgui_EMS_inputbox_change_input_mode_multitap_GBSabc(void)
{	wgui_EMS_inputbox_input_type = INPUT_TYPE_SMART_LOWERCASE_ABC;
	wgui_EMS_inputbox_input_mode_changed = 1;
}

//beginning:guoyt added for adding Hindi,German,Spanish IME.
#ifdef __MMI_SMART_HINDI__			
void (*wgui_change_inputbox_mode_smart_hindi) (void) = UI_dummy_function;
void wgui_EMS_inputbox_change_input_mode_smart_hindi(void)	
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_HINDI;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_SMART_GERMAN__)
void (*wgui_change_inputbox_mode_smart_GERMAN) (void) = UI_dummy_function;
void (*wgui_change_inputbox_mode_smart_german) (void) = UI_dummy_function;
void wgui_EMS_inputbox_change_input_mode_smart_GERMAN(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_GERMAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
void wgui_EMS_inputbox_change_input_mode_smart_german(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_GERMAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_SMART_SPANISH__)
extern void(*wgui_change_inputbox_mode_smart_SPANISH)(void);
extern void(*wgui_change_inputbox_mode_smart_spanish)(void);
void wgui_EMS_inputbox_change_input_mode_smart_SPANISH(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_SPANISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}
void wgui_EMS_inputbox_change_input_mode_smart_spanish(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_SPANISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
//end:guoyt added for adding German,Spanish IME.

#ifdef __MMI_SMART_TELUGU__
void (*wgui_change_inputbox_mode_smart_telugu) (void) = UI_dummy_function;
void wgui_EMS_inputbox_change_input_mode_smart_telugu(void)	
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_TELUGU;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif


#endif
 
#if defined(__MMI_MULTITAP_THAI__)
void wgui_EMS_inputbox_change_input_mode_multitap_thai(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_MULTITAP_THAI;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif

 
#if defined(__MMI_MULTITAP_SPANISH__)
void wgui_EMS_inputbox_change_input_mode_multitap_SPANISH(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_UPPERCASE_SPANISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_spanish(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_LOWERCASE_SPANISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
                //qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
void wgui_EMS_inputbox_change_input_mode_multitap_tamil(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_MMI_MULTITAP_TAMIL;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
void wgui_EMS_inputbox_change_input_mode_multitap_bengali(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_MMI_MULTITAP_BENGALI;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
void wgui_EMS_inputbox_change_input_mode_multitap_punjabi(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_MMI_MULTITAP_PUNJABI;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
void wgui_EMS_inputbox_change_input_mode_multitap_telugu(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_MMI_MULTITAP_TELUGU;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
void wgui_EMS_inputbox_change_input_mode_multitap_kannada(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_MMI_MULTITAP_KANNADA;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
void wgui_EMS_inputbox_change_input_mode_multitap_malayalam(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_MMI_MULTITAP_MALAYALAM;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
void wgui_EMS_inputbox_change_input_mode_multitap_oriya(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_MMI_MULTITAP_ORIYA;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
                //qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
void wgui_EMS_inputbox_change_input_mode_multitap_marathi(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_MMI_MULTITAP_MARATHI;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif

#if defined(__MMI_MULTITAP_URDU__)
void wgui_EMS_inputbox_change_input_mode_multitap_urdu(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_MMI_MULTITAP_URDU;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif

 
#if defined(__MMI_MULTITAP_DANISH__)
void wgui_EMS_inputbox_change_input_mode_multitap_DANISH(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_UPPERCASE_DANISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_danish(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_LOWERCASE_DANISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
 
#if defined(__MMI_MULTITAP_FRENCH__)
void wgui_EMS_inputbox_change_input_mode_multitap_FRENCH(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_UPPERCASE_FRENCH;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_french(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_LOWERCASE_FRENCH;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_MYANMAR__)
void wgui_EMS_inputbox_change_input_mode_multitap_myanmar(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_MMI_MULTITAP_MYANMAR;
	wgui_EMS_inputbox_input_mode_changed=1;
}

#endif
#if defined(__MMI_SMART_FRENCH__)
void wgui_EMS_inputbox_change_input_mode_smart_FRENCH(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_FRENCH;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_french(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_FRENCH;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_SMART_URDU__)
void wgui_EMS_inputbox_change_input_mode_smart_urdu(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_URDU;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_SMART_PUNJABI__)
void wgui_EMS_inputbox_change_input_mode_smart_punjabi(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_PUNJABI;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_SMART_BENGALI__)
void wgui_EMS_inputbox_change_input_mode_smart_bengali(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_BENGALI;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_SMART_TAMIL__)
void wgui_EMS_inputbox_change_input_mode_smart_tamil(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_TAMIL;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_SMART_TURKISH__)
void wgui_EMS_inputbox_change_input_mode_smart_TURKISH(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_TURKISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_turkish(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_TURKISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_SMART_INDONESIAN__)
void wgui_EMS_inputbox_change_input_mode_smart_INDONESIAN(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_INDONESIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_indonesian(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_INDONESIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
// beginning--added by kecx for adding italian on 20081223
#if defined(__MMI_SMART_ITALIAN__)
void wgui_EMS_inputbox_change_input_mode_smart_ITALIAN(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_ITALIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_italian(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_ITALIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
// end--added by kecx for adding italian on 20081223
#if defined(__MMI_MULTITAP_GERMAN__)
void wgui_EMS_inputbox_change_input_mode_multitap_GERMAN(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_UPPERCASE_GERMAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_german(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_LOWERCASE_GERMAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_ITALIAN__)
void wgui_EMS_inputbox_change_input_mode_multitap_ITALIAN(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_UPPERCASE_ITALIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_italian(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_LOWERCASE_ITALIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_RUSSIAN__)
void wgui_EMS_inputbox_change_input_mode_multitap_RUSSIAN(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_UPPERCASE_RUSSIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_russian(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_LOWERCASE_RUSSIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
/*Wangzl Add Start For Ver: Multi_Language  on 2008-1-21 19:33 */
#if defined(__MMI_SMART_RUSSIAN__)
void wgui_EMS_inputbox_change_input_mode_smart_RUSSIAN(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_RUSSIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_russian(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_RUSSIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
/*Wangzl Add End  For Ver: Multi_Language  on 2008-1-21 19:33 */
#if defined(__MMI_SMART_ARABIC__)
void wgui_EMS_inputbox_change_input_mode_smart_arabic(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_ARABIC;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif

#if defined(__MMI_MULTITAP_ARABIC__)
void wgui_EMS_inputbox_change_input_mode_multitap_arabic(void)
{
	wgui_EMS_inputbox_input_type=INPUT_TYPE_MMI_MULTITAP_ARABIC;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif

#if defined(__MMI_SMART_PERSIAN__)
void wgui_EMS_inputbox_change_input_mode_smart_persian(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_PERSIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif

#if defined(__MMI_SMART_THAI__)
void wgui_EMS_inputbox_change_input_mode_smart_thai(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_THAI;
	wgui_EMS_inputbox_input_mode_changed=1;
}	
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
void wgui_EMS_inputbox_change_input_mode_smart_VIETNAMESE(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_vietnamese(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_TURKISH__)
void wgui_EMS_inputbox_change_input_mode_multitap_TURKISH(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_UPPERCASE_TURKISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_turkish(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_LOWERCASE_TURKISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_PORTUGUESE__)
void wgui_EMS_inputbox_change_input_mode_multitap_PORTUGUESE(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_UPPERCASE_PORTUGUESE;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_portuguese(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_LOWERCASE_PORTUGUESE;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_HINDI__)
void wgui_EMS_inputbox_change_input_mode_multitap_HINDI(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_MULTITAP_HINDI;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif

#if defined(__MMI_MULTITAP_INDONESIAN__)
void wgui_EMS_inputbox_change_input_mode_multitap_INDONESIAN(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_UPPERCASE_INDONESIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_indonesian(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_LOWERCASE_INDONESIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_MALAY__)
void wgui_EMS_inputbox_change_input_mode_multitap_MALAY(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_UPPERCASE_MALAY;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_malay(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_LOWERCASE_MALAY;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_SMART_MALAY__)
void wgui_EMS_inputbox_change_input_mode_smart_MALAY(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_MALAY;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_malay(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_MALAY;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_MULTITAP_VIETNAMESE__)
void wgui_EMS_inputbox_change_input_mode_multitap_VIETNAMESE(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_UPPERCASE_VIETNAMESE;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_multitap_vietnamese(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_ALPHANUMERIC_LOWERCASE_VIETNAMESE;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
 
#if defined(__MMI_T9__) || defined(__MMI_ZI__) || defined(__MMI_KA__) || defined(__MMI_ITAP__)
void wgui_EMS_inputbox_change_input_mode_smart_ABC(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_ABC;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_abc(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_ABC;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_tr_bpmf(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_TR_BOPOMO;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_tr_stroke(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_TR_STROKE;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_sm_pinyin(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SM_PINYIN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_sm_stroke(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SM_STROKE;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_tr_multitap_bpmf(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_TR_MULTITAP_BOPOMO;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_sm_multitap_pinyin(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SM_MULTITAP_PINYIN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_arabic(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_ARABIC;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_thai(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_THAI;
	wgui_EMS_inputbox_input_mode_changed=1;
}

 
#if defined(__MMI_T9_SPANISH__)||defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)
void wgui_EMS_inputbox_change_input_mode_smart_SPANISH(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_SPANISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_spanish(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_SPANISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
 
#if defined(__MMI_T9_DANISH__)||defined(__MMI_ZI_DANISH__)
void wgui_EMS_inputbox_change_input_mode_smart_DANISH(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_DANISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_danish(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_DANISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
 
#if defined(__MMI_T9_FRENCH__)||defined(__MMI_ZI_CA_FRENCH__)||defined(__MMI_ZI_EU_FRENCH__)
void wgui_EMS_inputbox_change_input_mode_smart_FRENCH(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_FRENCH;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_french(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_FRENCH;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_T9_GERMAN__)||defined(__MMI_ZI_GERMAN__)
void wgui_EMS_inputbox_change_input_mode_smart_GERMAN(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_GERMAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_german(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_GERMAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
#if defined(__MMI_T9_ITALIAN__)||defined(__MMI_ZI_ITALIAN__)
void wgui_EMS_inputbox_change_input_mode_smart_ITALIAN(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_ITALIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_italian(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_ITALIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
 
#if defined (__MMI_T9_RUSSIAN__)|| defined(__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
//END 
void wgui_EMS_inputbox_change_input_mode_smart_RUSSIAN(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_RUSSIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_russian(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_RUSSIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif

#if defined(__MMI_ZI_TURKISH__)

void wgui_EMS_inputbox_change_input_mode_smart_TURKISH(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_TURKISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_turkish(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_TURKISH;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
 
#if defined(__MMI_T9_PORTUGUESE__)||defined(__MMI_ZI_EU_PORTUGUESE__)||defined(__MMI_ZI_SA_PORTUGUESE__)||defined(__MMI_ZI_BZ_PORTUGUESE__)
 
void wgui_EMS_inputbox_change_input_mode_smart_PORTUGUESE(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_portuguese(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
 
#if defined(__MMI_ZI_INDONESIAN__)||defined(__MMI_T9_INDONESIAN__)
void wgui_EMS_inputbox_change_input_mode_smart_INDONESIAN(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_INDONESIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_indonesian(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_INDONESIAN;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
 
#if defined(__MMI_ZI_MALAY__)
void wgui_EMS_inputbox_change_input_mode_smart_MALAY(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_MALAY;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_malay(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_MALAY;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
 
#if defined(__MMI_ZI_VIETNAMESE__)||defined(__MMI_T9_VIETNAMESE__)
 
void wgui_EMS_inputbox_change_input_mode_smart_VIETNAMESE(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE;
	wgui_EMS_inputbox_input_mode_changed=1;
}

void wgui_EMS_inputbox_change_input_mode_smart_vietnamese(void)
{	wgui_EMS_inputbox_input_type=INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE;
	wgui_EMS_inputbox_input_mode_changed=1;
}
#endif
//CSD end
//CSD end
#endif
void wgui_EMS_inputbox_insert_symbol(UI_character_type symbol)
{
#if(!UI_DISABLE_EMS_INPUT_BOX)
	EMS_inputbox_direct_input(symbol);
#endif
}

void temp_ems_insert_character(void)
{	UI_character_type symbol=0xabc;
#if(!UI_DISABLE_EMS_INPUT_BOX)
	EMS_inputbox_direct_input(symbol);
#endif
}

#if(UI_DISABLE_EMS_INPUT_BOX)

EMSTATUS EMS_inputbox_insert_predefined_picture(S32 index)
{	UI_UNUSED_PARAMETER(index);
	return(0);
}

#endif

/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-17 17:16 */

/**************************************************************

	FUNCTION NAME		: JudgeStringChOrEn

  	PURPOSE				: 判断字符串s 中长度为length内是否函数汉字
	INPUT PARAMETERS	: s ; length

	OUTPUT PARAMETERS	: ChPlace

	RETURNS				: BOOL

 
	NOTE				:length的长度不要大于strlen(s)

**************************************************************/
BOOL  JudgeStringChOrEn(U8 *s, kal_uint16 length, kal_uint16 *ChPlace)
{
	kal_uint16 i = 1;
	ASSERT(s != NULL);
	*ChPlace = length;
	while (i < length)
	{
		if (s[i] != 0)
		{
			*ChPlace = i - 1;
			return TRUE;
		}
		i += 2;
	}	
	return FALSE;
}
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-17 17:16 */


/**************************************************************

	FUNCTION NAME		: AppendCategory28String

  	PURPOSE				: 在短信编辑框中插入新的字符
	INPUT PARAMETERS	: input_type,data,history_buffer

	OUTPUT PARAMETERS	: NULL

	RETURNS				: BOOL

 

**************************************************************/
byte AppendCategory28String(s16 input_type,EMSData *data,U8* s,U8* history_buffer)
{
#if(!UI_DISABLE_EMS_INPUT_BOX)
	S32					i=0;
#ifdef __MMI_MENUTREE_2__
	S32					j=0;
#endif
	S32					l,box_height;
	U8					ts[2]; // Temporary String, 临时字符串.  用于缓存选中的常用短语的每一个字符(中文or英文).
	byte				h;
	UI_character_type	c;
//Manju added for template, 1213
	S32					remaining_length; // 编辑器剩余的字符数,即还可以录入的字符个数.
#if defined(__MMI_GB__)
	kal_uint16 ChPlace = 0; // 字符串第一个中文字符的位置.
#endif	
//Mamju end
 
#if defined(__MMI_GB__)
	// 如果s超长则截短.
	if (data->textLength > 0)
	{
		/*当短信编辑框内有输入内容时,
		再插入常用短语,即使超出长度也会
		插入,这样是不对的,所以我们要考虑
		各种情况下,做出边界判断再决定是否插入,以及
		应该全部插入还是只插入一部分,使之符合短信编辑框的插入条件*/
		if (MMI_EMS_inputbox.UCS2_count > 0)                  //当在短信编辑框中插入常用短语超过编辑框限制时,不能全部插入
		{
			if ((data->textLength + pfnUnicodeStrlen(s) * 2) >= EMSEditor_Chinese_Length)
			{
				memset(s + EMSEditor_Chinese_Length - data->textLength, 0, pfnUnicodeStrlen(s) * 2 - (EMSEditor_Chinese_Length - data->textLength));
				s[EMSEditor_Chinese_Length - data->textLength] = '\0';
			}
		}
		else
		{
			if ((data->textLength + pfnUnicodeStrlen(s) * 2) >= EMSEditor_Enghish_Length)                  //控制总和长度
			{
				memset(s + EMSEditor_Enghish_Length - data->textLength, 0, pfnUnicodeStrlen(s) * 2 -(EMSEditor_Enghish_Length - data->textLength));
				s[EMSEditor_Enghish_Length - data->textLength] = '\0';
			}

			if (data->textLength > EMSEditor_Chinese_Length)
			{
				if (JudgeStringChOrEn(s,pfnUnicodeStrlen(s) * 2, &ChPlace) == TRUE)   //当s串中含有中文时
				{
					memset(s+ChPlace, 0, pfnUnicodeStrlen(s) * 2 - ChPlace);
					s[ChPlace] = '\0';
				}
			}
			else
			{
				if (JudgeStringChOrEn(s, pfnUnicodeStrlen(s) * 2, &ChPlace) == TRUE)
				{
					if ((data->textLength + ChPlace) < EMSEditor_Chinese_Length)
					{
						if ((data->textLength + pfnUnicodeStrlen(s) * 2) > EMSEditor_Chinese_Length)
						{
							memset(s + (EMSEditor_Chinese_Length - data->textLength), 0, pfnUnicodeStrlen(s) * 2 - (EMSEditor_Chinese_Length - data->textLength));
							s[EMSEditor_Chinese_Length - data->textLength] = '\0';
						}
					}
					else
					{
						memset(s + ChPlace, 0, pfnUnicodeStrlen(s) * 2 - ChPlace);
						s[ChPlace] = '\0';
					}
				}
			}
		}
	}
#endif
//wangzl: why there are these code? can't hindi and arabic exist at same time? 090520 remove these code for bug12106
#if 0 //defined __MMI_LANG_HINDI__  && defined __MMI_LANG_ARABIC__
	if(ARABIC_PRESENT || MMI_current_input_mode==INPUT_MODE_SMART_ARABIC)
	{
	U16 unicode=0;
	U16 clFill=0;
	IS_HINDI=0;
	while((s[clFill]!='\0' || s[clFill+1]!='\0')&& !IS_HINDI)
	{
		unicode = s[clFill];
		unicode|=(s[clFill+1]<<8);
		if(UI_HINDI_CHARACTER_UCS2_RANGE(unicode))
		{
			IS_HINDI=1;
		}
		clFill+=2;
	}
	if(IS_HINDI && ARABIC_PRESENT)
		return(0);
	else if(IS_HINDI)
	{
		test_change_input_mode=test_EMS_inputbox_change_mode;
#if defined(__MMI_SMART_HINDI__)		//guoyt modify for adding smart hindi
		wgui_change_inputbox_mode_smart_hindi();				
#elif defined(__MMI_MULTITAP_HINDI__)
		wgui_change_inputbox_mode_multitap_HINDI();
#endif		
	}
	}
	else if(HINDI_PRESENT || MMI_current_input_mode==INPUT_MODE_MULTITAP_HINDI)
	{
		U16 default_direction=1;
		U16 Count = 0;
		U16 length= pfnUnicodeStrlen((const char *)s)*2;
		IS_ARABIC=0;
		for(Count = 0; Count<length;Count+=2)
		{
            bidi_get_char_type(s+Count, (U8 *)&default_direction);
			if ( (default_direction==5) || (default_direction==3) || (default_direction ==2))
			IS_ARABIC = 1;
		}
		if(IS_ARABIC && HINDI_PRESENT)
			return(0);
		else if(IS_ARABIC)
		{
			test_change_input_mode=test_EMS_inputbox_change_mode;
			wgui_change_inputbox_mode_smart_arabic();
		}
	}
	#endif
 
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif
	l=coolsand_UI_strlen((UI_string_type)s);
	box_height=MMI_content_height-MMI_multitap_height-10; // MMI_content_height: LCD高度-标题栏高度-左右软件栏高度; MMI_multitap_height: 在这里就是标题栏下面的提示栏的高度; 10: 就是手写符号栏的高度.
	coolsand_UI_create_EMS_input_box(&MMI_EMS_inputbox,0,(MMI_title_y+MMI_title_height)+12,UI_device_width,box_height); // 创建一个ems editor.
	coolsand_UI_set_EMS_input_box_data(&MMI_EMS_inputbox,data); //把data指给这个新的ems editor.
	h=set_EMS_inputbox_category_history(MMI_CATEGORY28_ID,history_buffer,(s16*)&input_type);
	if(h)
	{	coolsand_UI_EMS_input_box_locate_cursor(&MMI_EMS_inputbox);
	#ifdef __MMI_MESSAGES_EMS__
		coolsand_UI_EMS_input_box_set_text_format_object(&MMI_EMS_inputbox,&wgui_EMS_text_format);
	#endif
	}
	MMI_EMS_inputbox.flags|=UI_EMS_INPUT_BOX_DISABLE_DRAW;
	coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox);
	MMI_EMS_inputbox.flags&=~UI_EMS_INPUT_BOX_DISABLE_DRAW;
	for(i=0;i<l;i++)
	{	UI_STRING_GET_NEXT_CHARACTER(s,c); // 逐个读取s中的字符到c中,以备进一步处理.
#if 0 // 两种不同的unicode编码方式: 大头在先/小头在先. 例如汉字'我'的unicode码为6211,则如果把'我'的unicode码存在ts[]中则大头的存放情况是ts[0]=62,ts[1]=11; 小头的情况是ts[0]=11,ts[1]=62.
		ts[0]=c>>8;
		ts[1]=c&0xff;
#else // 大头在先encoding.
		ts[1]=c>>8;
		ts[0]=c&0xff;
#endif
//Manju added for template, 1213
		if(MMI_EMS_inputbox.UCS2_count>0)
		{
			remaining_length=(EMS_get_remaining_length(MMI_EMS_inputbox.data)>>1);
		}
		else
		{	remaining_length=EMS_get_remaining_length(MMI_EMS_inputbox.data);
		}
		if(remaining_length<=0)
		{	break;
		}
		if(MMI_EMS_inputbox.UCS2_count==0)
		{	if(UI_TEST_UCS2_CHARACTER(c))
			{	if(EMSSetDCS(MMI_EMS_inputbox.data,SMSAL_UCS2_DCS) != EMS_OK)
				{   EMSSetDCS(MMI_EMS_inputbox.data,SMSAL_DEFAULT_DCS);
					break;
				}
				else
				{   if(EMS_get_remaining_length(MMI_EMS_inputbox.data)<2)
					{	EMSSetDCS(MMI_EMS_inputbox.data,SMSAL_DEFAULT_DCS);
						break;
					}
				}
			}
			else if(UI_TEST_GSM_EXTENDED(c))
			{	if(EMS_get_remaining_length(MMI_EMS_inputbox.data)<2)
				{	break;
				}
			}
		}
//Manju end
		if(coolsand_UI_EMS_input_box_add_string(&MMI_EMS_inputbox,ts,&MMI_EMS_inputbox.text_format)!=EMS_OK) break; // 把ts中的单个字符添加到编辑器中.
		coolsand_UI_EMS_input_box_locate_cursor(&MMI_EMS_inputbox);
	}
	get_EMS_inputbox_category_history(MMI_CATEGORY28_ID,history_buffer,input_type); // 把一些信息再读取出来存放到history_buffer中.
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
#endif
	if(i==l) return(1);
	else
	{
#ifdef __MMI_MENUTREE_2__
		for (j=0; j<i; j++)
			coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
#endif
		return(0);
	}
#else
	UI_UNUSED_PARAMETER(input_type);
	UI_UNUSED_PARAMETER(data);
	UI_UNUSED_PARAMETER(s);
	UI_UNUSED_PARAMETER(history_buffer);
	return(0);
#endif
}


byte AppendEMSString(s16 input_type,EMSData *data,U8* s,U8 dcs,U8* history_buffer)
{
	S32					l,box_height;

	l=coolsand_UI_strlen((UI_string_type)s);
	box_height=MMI_content_height-MMI_multitap_height-10;
	coolsand_UI_create_EMS_input_box(&MMI_EMS_inputbox,0,(MMI_title_y+MMI_title_height)+12,UI_device_width,box_height);
	coolsand_UI_set_EMS_input_box_data(&MMI_EMS_inputbox,data);
	EMSSetDCS(MMI_EMS_inputbox.data,dcs);

	if(AddString(MMI_EMS_inputbox.data, s, (U16)l, NULL)==EMS_OK)
		return 1;
	else
		return 0;
}
#ifdef __SP__
byte AppendEMSString_Ext(s16 input_type,EMSData *data,U8* s,U8 dcs,U8* history_buffer)
{
	S32					l,box_height;

	l=coolsand_UI_strlen((UI_string_type)s);
	box_height=MMI_content_height-MMI_multitap_height-10;
	coolsand_UI_create_EMS_input_box(&MMI_EMS_inputbox,0,(MMI_title_y+MMI_title_height)+12,UI_device_width,box_height);
	coolsand_UI_set_EMS_input_box_data(&MMI_EMS_inputbox,data);
	EMSSetDCS(MMI_EMS_inputbox.data,dcs);
  
	if(AddString_Ext(MMI_EMS_inputbox.data, s, (U16)UCS2Strlen(s), NULL)==EMS_OK)
		return 1;
	else
		return 0;
}
#endif
#ifdef __MMI_DRAW_MANAGER__
void category28callback(dm_coordinates * coordinate)
{
	 
	if( RedrawSpellingOrCandidateBoxesFunction)
		   RedrawSpellingOrCandidateBoxesFunction();
}
void move_EMS_inputbox(S32 x, S32 y)
{
	coolsand_UI_move_EMS_input_box(&MMI_EMS_inputbox, x,  y);
}
void resize_EMS_inputbox(S32 width, S32 height)
{
	coolsand_UI_resize_EMS_input_box(&MMI_EMS_inputbox,  width,  height);
}
#endif
void cat28_virtual_keypad_callback(void)
{
#if defined(__MMI_TOUCH_SCREEN__)
    
   //We could use this function for EMS input box
   mmi_pen_editor_resize_multiline_input_box_for_vk();
#endif
}

/*----------------------------------------------------------------------------
Function:			ShowCategory28Screen
Description:		Displays the MO EMS/SMS screen
Input Parameters:	[1]	STRING_ID	title,				Title for the screen
					[2]	IMAGE_ID	title_icon,			Icon shown with the title
					[3]	STRING_ID	left_softkey,		Left softkey label
					[4]	IMAGE_ID	left_softkey_icon,	Icon for the left softkey
					[5]	STRING_ID	right_softkey,		Right softkey label
					[6]	IMAGE_ID	right_softkey_icon,	Icon for the right softkey
					[7]	EMSData*	data				EMSData (Pre-allocated and initialized)
					[8]	BYTE*		history_buffer		history buffer
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/
//CSD modify for Thai
extern UINT16 TitlePlaceFlag;
#ifdef __MMI_NOKIA_STYLE_N800__
extern color_t green_color;
#endif

void ShowCategory28Screen(U16 title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,EMSData* data,U8* history_buffer)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
#ifdef __MMI_DRAW_MANAGER__
	dm_data_struct dm_data;
#endif

	U16 input_type=(U16)INPUT_TYPE_ALPHANUMERIC_LOWERCASE;
#if(UI_DISABLE_EMS_INPUT_BOX)
	S32				length;
	S32				i,j,h,fh;
	UI_UNUSED_PARAMETER(history_buffer);

#else
	byte			h;
	S32				box_height;
#endif

#if defined(__MMI_TOUCH_SCREEN__)
	mmi_pen_handwriting_area_struct stroke_area;
	mmi_pen_handwriting_area_struct ext_stroke;
#endif

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	TitlePlaceFlag = 1;

	gdi_layer_lock_frame_buffer();
 
#if defined __MMI_LANG_HINDI__  && defined __MMI_LANG_ARABIC__
	ARABIC_PRESENT=0;
	HINDI_PRESENT=0;
#endif

       
      set_current_input_box_type(EDITOR_EMS_INPUT_BOX);

	test_change_input_mode=test_EMS_inputbox_change_mode;
	wgui_set_default_input_type(&input_type);


      wgui_inputbox_information_flag=1;
#if defined(__PROJECT_GALLITE_C01__)
      wgui_inputbox_information_bar_height=INFORMATION_BAR_HEIGHT + 1;
#else
      wgui_inputbox_information_bar_height=INFORMATION_BAR_HEIGHT;
#endif
      wgui_EMS_setup_input_information(0,(MMI_title_y+MMI_title_height),UI_device_width,wgui_inputbox_information_bar_height);
	MMI_menu_shortcut_number=-1;
	change_left_softkey(left_softkey,left_softkey_icon);
	change_right_softkey(right_softkey,right_softkey_icon);
	SetupCategoryKeyHandlers();

	MMI_title_string=(UI_string_type)get_string(title);
	MMI_title_icon=(UI_image_type)get_image(title_icon);
    
   #ifdef __MMI_T9__
   InuptMethodEnterCategory28();
    #elif defined(__MMI_ZI__) && defined(__MMI_MESSAGES_EMS__)
   ZiInuptMethodEnterCategory28();
   #elif defined(__MMI_KA__)
   KonkaInuptMethodEnterCategory28();
   #elif defined(__MMI_ITAP__)
   ItapInuptMethodEnterCategory28();
   #elif defined(__MMI_GB__)
   GbInuptMethodEnterCategory28();
   #endif

#if(UI_DISABLE_EMS_INPUT_BOX) // wangbei dle 20060622
	length=(data->textBufferSize>>1);
	if(length==0)
	{	UI_string_type	s=(UI_string_type)"EMS Data not initialized";
		S32				l=coolsand_UI_strlen((UI_string_type)s);
		temp_EMS_buffer=(UI_string_type)coolsand_UI_malloc(l*sizeof(UI_character_type));
		coolsand_UI_strcpy((UI_string_type)temp_EMS_buffer,(UI_string_type)s);
		length=l+1;
		i=j=l;
	}
	else
	{
		temp_EMS_buffer=(UI_string_type)coolsand_UI_malloc(length*sizeof(UI_character_type));
		i=j=0;
		while(j<data->textLength)
		{
			if((data->textBuffer[j]=='\0') && (data->textBuffer[j+1]=='\0'))
			{	temp_EMS_buffer[i]='\0';
				break;
			}
			else temp_EMS_buffer[i]=data->textBuffer[j+1];
			i++;
			j+=2;
		}
	}
	temp_EMS_data=data;
	create_multiline_inputbox_set_buffer(temp_EMS_buffer,length,i,0);
	register_multiline_inputbox_keys();
	register_hide_multitap(category5_hide_multitap);
#ifndef __MMI_DRAW_MANAGER__
	move_multiline_inputbox(0,(MMI_title_y+MMI_title_height)+2);
#endif

#ifdef __MMI_NOKIA_STYLE_N800__
      MMI_multiline_inputbox.cursor_color=green_color;
#endif

	h=MMI_content_height-MMI_multitap_height-8;
	coolsand_UI_set_font(MMI_multiline_inputbox.text_font);
	fh=coolsand_UI_get_character_height();
	h=((h/fh)*fh)+8;
	resize_multiline_inputbox(MMI_multiline_inputbox.width,h);
	set_multiline_inputbox_mask(0);
	change_multiline_inputbox_mode(1);
	SetKeyHandler(change_input_mode,KEY_STAR,KEY_EVENT_DOWN);
	inputbox_change_mode=change_multiline_inputbox_input_mode;
	MMI_current_input_mode=1;

	set_right_softkey_function(handle_category28_right_softkey_down,KEY_EVENT_DOWN);
	set_right_softkey_function(handle_category28_right_softkey_long_press,KEY_LONG_PRESS);
	register_multiline_inputbox_input_callback(handle_category28_input);
	category28_RSK_label_icon=get_image(right_softkey_icon);
	category28_RSK_label_string=get_string(right_softkey);
	handle_category28_input();
	if(MMI_multiline_inputbox.text_length<=1)
		set_right_softkey_function(handle_category28_right_softkey_up,KEY_EVENT_UP);

 
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
	MMI_EMS_inputbox.flags |= UI_EMS_INPUT_BOX_TRANSPARENT_BACKGROUND;
	dm_get_scr_bg_image(&editor_scr_bg_ID, &editor_scr_bg_filename, &editor_scr_bg_x, &editor_scr_bg_y, &editor_scr_bg_opacity);
	dm_set_scr_bg_image(editor_scr_bg_ID, editor_scr_bg_filename, editor_scr_bg_x, editor_scr_bg_y, MMI_EDITOR_BG_OPACITY); 
#endif
 
#else /* UI_DISABLE_EMS_INPUT_BOX */

	#if((UI_ENABLE_FRAME_SKIPPING)&&(ENABLE_EMS_INPUTBOX_FRAME_SKIPPING))
		wgui_EMS_inputbox_frame_counter=0;
	#endif
	#ifdef __MMI_MESSAGES_EMS__
		initialize_EMS_objects();
	#endif
         
#if defined(__MMI_FULL_SCREEN_EDITOR__)
	box_height=MMI_content_height-13;
#else
	box_height=MMI_content_height-MMI_multitap_height-13;
#endif
        //CSD end
	coolsand_UI_create_EMS_input_box(&MMI_EMS_inputbox,0,(MMI_title_y+MMI_title_height)+wgui_EMS_inputbox_information_bar_height,UI_device_width,box_height);
	coolsand_UI_set_EMS_input_box_data(&MMI_EMS_inputbox,data);
	h=set_EMS_inputbox_category_history(MMI_CATEGORY28_ID,history_buffer,(s16*)&input_type);
	if(h)
	{	/*	HACK: Current alignment is not saved in history, so test it using a flag	*/
		if(EMS_inputbox_alignment_added_flag)
		{	wgui_EMS_text_format.Alignment=EMS_inputbox_alignment;
			MMI_EMS_inputbox.text_format.Alignment=EMS_inputbox_alignment;
			EMS_inputbox_alignment_added_flag=0;
		}
	#ifdef __MMI_MESSAGES_EMS__
		coolsand_UI_EMS_input_box_set_text_format_object(&MMI_EMS_inputbox,&wgui_EMS_text_format);
	#endif
	}
	else
	{

	       coolsand_UI_EMS_input_box_set_text_format_cursor_position(&MMI_EMS_inputbox);
		wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
	}
	coolsand_UI_EMS_input_box_locate_cursor(&MMI_EMS_inputbox);
	register_hide_multitap(category5_hide_multitap);
	SetKeyHandler(handle_category28_change_input_mode,KEY_POUND,KEY_EVENT_DOWN);
	 
#if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
#else
         SetKeyHandler(handle_category28_change_input_mode,KEY_POUND,KEY_LONG_PRESS);
#endif
	register_EMS_inputbox_keys();

 
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
	MMI_EMS_inputbox.flags |= UI_EMS_INPUT_BOX_TRANSPARENT_BACKGROUND;
	dm_get_scr_bg_image(&editor_scr_bg_ID, &editor_scr_bg_filename, &editor_scr_bg_x, &editor_scr_bg_y, &editor_scr_bg_opacity);
#if defined(__MMI_BLACKBERRY_QWERTY__) && defined(__MMI_MAINLCD_160X128__) 
	dm_set_scr_bg_image(editor_scr_bg_ID, editor_scr_bg_filename, editor_scr_bg_x, editor_scr_bg_y, 255);
#else
	dm_set_scr_bg_image(editor_scr_bg_ID, editor_scr_bg_filename, editor_scr_bg_x, editor_scr_bg_y, MMI_EDITOR_BG_OPACITY); 
#endif
#endif
 
	inputbox_change_mode=change_EMS_inputbox_input_mode;

	/*	Get input mode from "Input Methods" list selection	*/
	wgui_change_inputbox_mode_multitap_ABC=wgui_EMS_inputbox_change_input_mode_multitap_ABC;
	wgui_change_inputbox_mode_multitap_abc=wgui_EMS_inputbox_change_input_mode_multitap_abc;
	wgui_change_inputbox_mode_mode_numeric=wgui_EMS_inputbox_change_input_mode_multitap_numeric;

//add by yaosq  20061011
#if defined(__MMI_GB__)
	wgui_change_inputbox_mode_multitap_GBStroke = wgui_EMS_inputbox_change_input_mode_multitap_GBStroke;
	wgui_change_inputbox_mode_multitap_GBPinYin = wgui_EMS_inputbox_change_input_mode_multitap_GBPinYin;
	wgui_change_inputbox_mode_multitap_GB_SABC = wgui_EMS_inputbox_change_input_mode_multitap_GB_SABC;
	wgui_change_inputbox_mode_multitap_GBSabc = wgui_EMS_inputbox_change_input_mode_multitap_GBSabc;
	wgui_change_inputbox_mode_multitap_GB_TR_BPMF = wgui_EMS_inputbox_change_input_mode_multitap_GB_TR_BPMF;
	wgui_change_inputbox_mode_multitap_GB_TR_STROKE = wgui_EMS_inputbox_change_input_mode_multitap_GB_TR_Stroke;

#if defined(__MMI_SMART_THAI__)
	wgui_change_inputbox_mode_smart_thai = wgui_EMS_inputbox_change_input_mode_smart_thai;
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
	wgui_change_inputbox_mode_smart_VIETNAMESE = wgui_EMS_inputbox_change_input_mode_smart_VIETNAMESE;
	wgui_change_inputbox_mode_smart_vietnamese= wgui_EMS_inputbox_change_input_mode_smart_vietnamese;
#endif

#endif
#if defined(__MMI_MULTITAP_ARABIC__)
	wgui_change_inputbox_mode_multitap_arabic = wgui_EMS_inputbox_change_input_mode_multitap_arabic;
#endif
#endif
 
#if defined(__MMI_MULTITAP_THAI__)
	wgui_change_inputbox_mode_multitap_thai=wgui_EMS_inputbox_change_input_mode_multitap_thai;
#endif
	 
	#if defined(__MMI_MULTITAP_SPANISH__)
		wgui_change_inputbox_mode_multitap_SPANISH=wgui_EMS_inputbox_change_input_mode_multitap_SPANISH;
		wgui_change_inputbox_mode_multitap_spanish=wgui_EMS_inputbox_change_input_mode_multitap_spanish;
	#endif
//qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
             wgui_change_inputbox_mode_multitap_tamil = wgui_EMS_inputbox_change_input_mode_multitap_tamil;
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
             wgui_change_inputbox_mode_multitap_bengali = wgui_EMS_inputbox_change_input_mode_multitap_bengali;
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
             wgui_change_inputbox_mode_multitap_punjabi = wgui_EMS_inputbox_change_input_mode_multitap_punjabi;
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
             wgui_change_inputbox_mode_multitap_telugu = wgui_EMS_inputbox_change_input_mode_multitap_telugu;
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
             wgui_change_inputbox_mode_multitap_kannada = wgui_EMS_inputbox_change_input_mode_multitap_kannada;
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
             wgui_change_inputbox_mode_multitap_malayalam = wgui_EMS_inputbox_change_input_mode_multitap_malayalam;
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
             wgui_change_inputbox_mode_multitap_oriya = wgui_EMS_inputbox_change_input_mode_multitap_oriya;
#endif
//qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
             wgui_change_inputbox_mode_multitap_marathi = wgui_EMS_inputbox_change_input_mode_multitap_marathi;
#endif

#if defined(__MMI_MULTITAP_URDU__)
             wgui_change_inputbox_mode_multitap_urdu = wgui_EMS_inputbox_change_input_mode_multitap_urdu;
#endif

	 
	#if defined(__MMI_MULTITAP_DANISH__)
		wgui_change_inputbox_mode_multitap_DANISH=wgui_EMS_inputbox_change_input_mode_multitap_DANISH;
		wgui_change_inputbox_mode_multitap_danish=wgui_EMS_inputbox_change_input_mode_multitap_danish;
	#endif
 
	#if defined(__MMI_MULTITAP_FRENCH__)
		wgui_change_inputbox_mode_multitap_FRENCH=wgui_EMS_inputbox_change_input_mode_multitap_FRENCH;
		wgui_change_inputbox_mode_multitap_french=wgui_EMS_inputbox_change_input_mode_multitap_french;
	#endif
	#if defined(__MMI_MULTITAP_MYANMAR__)
		wgui_change_inputbox_mode_multitap_myanmar=wgui_EMS_inputbox_change_input_mode_multitap_myanmar;
	#endif
	#if defined(__MMI_SMART_FRENCH__)
		wgui_change_inputbox_mode_smart_FRENCH=wgui_EMS_inputbox_change_input_mode_smart_FRENCH;
		wgui_change_inputbox_mode_smart_french=wgui_EMS_inputbox_change_input_mode_smart_french;
	#endif
	#if defined(__MMI_SMART_URDU__)
		wgui_change_inputbox_mode_smart_urdu=wgui_EMS_inputbox_change_input_mode_smart_urdu;
	#endif
	#if defined(__MMI_SMART_PUNJABI__)
		wgui_change_inputbox_mode_smart_punjabi=wgui_EMS_inputbox_change_input_mode_smart_punjabi;
	#endif
	#if defined(__MMI_SMART_BENGALI__)
		wgui_change_inputbox_mode_smart_bengali=wgui_EMS_inputbox_change_input_mode_smart_bengali;
	#endif
	#if defined(__MMI_SMART_TAMIL__)
		wgui_change_inputbox_mode_smart_tamil=wgui_EMS_inputbox_change_input_mode_smart_tamil;
	#endif
	#if defined(__MMI_SMART_TURKISH__)
		wgui_change_inputbox_mode_smart_TURKISH=wgui_EMS_inputbox_change_input_mode_smart_TURKISH;
		wgui_change_inputbox_mode_smart_turkish=wgui_EMS_inputbox_change_input_mode_smart_turkish;
	#endif
	#if defined(__MMI_SMART_INDONESIAN__)
		wgui_change_inputbox_mode_smart_INDONESIAN=wgui_EMS_inputbox_change_input_mode_smart_INDONESIAN;
		wgui_change_inputbox_mode_smart_indonesian=wgui_EMS_inputbox_change_input_mode_smart_indonesian;
	#endif
	#if defined(__MMI_SMART_MALAY__)
		wgui_change_inputbox_mode_smart_MALAY=wgui_EMS_inputbox_change_input_mode_smart_MALAY;
		wgui_change_inputbox_mode_smart_malay=wgui_EMS_inputbox_change_input_mode_smart_malay;
	#endif
// beginning--added by kecx for adding italian on 20081223
	#if defined(__MMI_SMART_ITALIAN__)
		wgui_change_inputbox_mode_smart_ITALIAN=wgui_EMS_inputbox_change_input_mode_smart_ITALIAN;
		wgui_change_inputbox_mode_smart_italian=wgui_EMS_inputbox_change_input_mode_smart_italian;
	#endif
// end--added by kecx for adding italian on 20081223

//beginning:guoyt added for adding smart german ime
	#if defined(__MMI_SMART_HINDI__)
		wgui_change_inputbox_mode_smart_hindi=wgui_EMS_inputbox_change_input_mode_smart_hindi;
	#endif
	#if defined(__MMI_SMART_SPANISH__)
		wgui_change_inputbox_mode_smart_SPANISH = wgui_EMS_inputbox_change_input_mode_smart_SPANISH;
		wgui_change_inputbox_mode_smart_spanish = wgui_EMS_inputbox_change_input_mode_smart_spanish;
	#endif
	#if defined(__MMI_SMART_GERMAN__)		
		wgui_change_inputbox_mode_smart_GERMAN=wgui_EMS_inputbox_change_input_mode_smart_GERMAN;
		wgui_change_inputbox_mode_smart_german=wgui_EMS_inputbox_change_input_mode_smart_german;
	#endif				
//end:guoyt added for adding smart german ime
	#if defined(__MMI_SMART_TELUGU__)
		wgui_change_inputbox_mode_smart_telugu=wgui_EMS_inputbox_change_input_mode_smart_telugu;
	#endif
	#if defined(__MMI_MULTITAP_GERMAN__)
		wgui_change_inputbox_mode_multitap_GERMAN=wgui_EMS_inputbox_change_input_mode_multitap_GERMAN;
		wgui_change_inputbox_mode_multitap_german=wgui_EMS_inputbox_change_input_mode_multitap_german;
	#endif
	#if defined(__MMI_MULTITAP_ITALIAN__)
		wgui_change_inputbox_mode_multitap_ITALIAN=wgui_EMS_inputbox_change_input_mode_multitap_ITALIAN;
		wgui_change_inputbox_mode_multitap_italian=wgui_EMS_inputbox_change_input_mode_multitap_italian;
	#endif
	#if defined(__MMI_MULTITAP_RUSSIAN__)
		wgui_change_inputbox_mode_multitap_RUSSIAN=wgui_EMS_inputbox_change_input_mode_multitap_RUSSIAN;
		wgui_change_inputbox_mode_multitap_russian=wgui_EMS_inputbox_change_input_mode_multitap_russian;
	#endif
	/*Wangzl Add Start For Ver: Multi_Language  on 2008-1-21 19:32 */
	#if defined(__MMI_SMART_RUSSIAN__)
		wgui_change_inputbox_mode_smart_RUSSIAN=wgui_EMS_inputbox_change_input_mode_smart_RUSSIAN;
		wgui_change_inputbox_mode_smart_russian=wgui_EMS_inputbox_change_input_mode_smart_russian;
	#endif
	/*Wangzl Add End  For Ver: Multi_Language  on 2008-1-21 19:32 */
	#if defined(__MMI_SMART_ARABIC__) && defined(__MMI_GB__)
		wgui_change_inputbox_mode_smart_arabic=wgui_EMS_inputbox_change_input_mode_smart_arabic;
	#endif
	#if defined(__MMI_MULTITAP_ARABIC__)
		wgui_change_inputbox_mode_multitap_arabic = wgui_EMS_inputbox_change_input_mode_multitap_arabic;
	#endif
	#if defined(__MMI_SMART_PERSIAN__)
		wgui_change_inputbox_mode_smart_persian=wgui_EMS_inputbox_change_input_mode_smart_persian;
	#endif

	#if defined(__MMI_SMART_THAI__)
		wgui_change_inputbox_mode_smart_thai = wgui_EMS_inputbox_change_input_mode_smart_thai;
	#endif
	#if defined(__MMI_SMART_VIETNAMESE__)
		wgui_change_inputbox_mode_smart_VIETNAMESE = wgui_EMS_inputbox_change_input_mode_smart_VIETNAMESE;
		wgui_change_inputbox_mode_smart_vietnamese= wgui_EMS_inputbox_change_input_mode_smart_vietnamese;
	#endif
	
	#if defined(__MMI_MULTITAP_TURKISH__)
		wgui_change_inputbox_mode_multitap_TURKISH=wgui_EMS_inputbox_change_input_mode_multitap_TURKISH;
		wgui_change_inputbox_mode_multitap_turkish=wgui_EMS_inputbox_change_input_mode_multitap_turkish;
	#endif
	#if defined(__MMI_MULTITAP_PORTUGUESE__)
		wgui_change_inputbox_mode_multitap_PORTUGUESE=wgui_EMS_inputbox_change_input_mode_multitap_PORTUGUESE;
		wgui_change_inputbox_mode_multitap_portuguese=wgui_EMS_inputbox_change_input_mode_multitap_portuguese;
	#endif
	#if defined(__MMI_MULTITAP_HINDI__)
		wgui_change_inputbox_mode_multitap_HINDI=wgui_EMS_inputbox_change_input_mode_multitap_HINDI;
	#endif

	#if defined(__MMI_MULTITAP_INDONESIAN__)
		wgui_change_inputbox_mode_multitap_INDONESIAN=wgui_EMS_inputbox_change_input_mode_multitap_INDONESIAN;
		wgui_change_inputbox_mode_multitap_indonesian=wgui_EMS_inputbox_change_input_mode_multitap_indonesian;
	#endif
	#if defined(__MMI_MULTITAP_MALAY__)
		wgui_change_inputbox_mode_multitap_MALAY=wgui_EMS_inputbox_change_input_mode_multitap_MALAY;
		wgui_change_inputbox_mode_multitap_malay=wgui_EMS_inputbox_change_input_mode_multitap_malay;
	#endif
	#if defined(__MMI_MULTITAP_VIETNAMESE__)
		wgui_change_inputbox_mode_multitap_VIETNAMESE=wgui_EMS_inputbox_change_input_mode_multitap_VIETNAMESE;
		wgui_change_inputbox_mode_multitap_vietnamese=wgui_EMS_inputbox_change_input_mode_multitap_vietnamese;
	#endif
	//CSD end

	#if defined(__MMI_T9__) || defined(__MMI_ZI__) || defined(__MMI_KA__) || defined(__MMI_ITAP__)
		 wgui_change_inputbox_mode_smart_ABC=wgui_EMS_inputbox_change_input_mode_smart_ABC;
		 wgui_change_inputbox_mode_smart_abc=wgui_EMS_inputbox_change_input_mode_smart_abc;
		 wgui_change_inputbox_mode_tr_bpmf=wgui_EMS_inputbox_change_input_mode_tr_bpmf;
		 wgui_change_inputbox_mode_tr_stroke=wgui_EMS_inputbox_change_input_mode_tr_stroke;
		 wgui_change_inputbox_mode_sm_pinyin=wgui_EMS_inputbox_change_input_mode_sm_pinyin;
		 wgui_change_inputbox_mode_sm_stroke=wgui_EMS_inputbox_change_input_mode_sm_stroke;
		 wgui_change_inputbox_mode_tr_multitap_bpmf=wgui_EMS_inputbox_change_input_mode_tr_multitap_bpmf;
		 wgui_change_inputbox_mode_sm_multitap_pinyin=wgui_EMS_inputbox_change_input_mode_sm_multitap_pinyin;
		 wgui_change_inputbox_mode_smart_thai=wgui_EMS_inputbox_change_input_mode_smart_thai;
		 wgui_change_inputbox_mode_smart_arabic=wgui_EMS_inputbox_change_input_mode_smart_arabic;

		 
		#if defined(__MMI_T9_SPANISH__)||defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)
			wgui_change_inputbox_mode_smart_SPANISH=wgui_EMS_inputbox_change_input_mode_smart_SPANISH;
			wgui_change_inputbox_mode_smart_spanish=wgui_EMS_inputbox_change_input_mode_smart_spanish;
		#endif
		 
		#if defined(__MMI_T9_DANISH__)||defined(__MMI_ZI_DANISH__)
			wgui_change_inputbox_mode_smart_DANISH=wgui_EMS_inputbox_change_input_mode_smart_DANISH;
			wgui_change_inputbox_mode_smart_danish=wgui_EMS_inputbox_change_input_mode_smart_danish;
		#endif
		 
		#if defined(__MMI_T9_FRENCH__)||defined(__MMI_ZI_CA_FRENCH__)||defined(__MMI_ZI_EU_FRENCH__)
			wgui_change_inputbox_mode_smart_FRENCH=wgui_EMS_inputbox_change_input_mode_smart_FRENCH;
			wgui_change_inputbox_mode_smart_french=wgui_EMS_inputbox_change_input_mode_smart_french;
		#endif
		#if defined(__MMI_T9_GERMAN__)||defined(__MMI_ZI_GERMAN__)
			wgui_change_inputbox_mode_smart_GERMAN=wgui_EMS_inputbox_change_input_mode_smart_GERMAN;
			wgui_change_inputbox_mode_smart_german=wgui_EMS_inputbox_change_input_mode_smart_german;
		#endif
		#if defined(__MMI_T9_ITALIAN__)||defined(__MMI_ZI_ITALIAN__)
			wgui_change_inputbox_mode_smart_ITALIAN=wgui_EMS_inputbox_change_input_mode_smart_ITALIAN;
			wgui_change_inputbox_mode_smart_italian=wgui_EMS_inputbox_change_input_mode_smart_italian;
		#endif
 
		#if defined (__MMI_T9_RUSSIAN__)|| defined(__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
//END 
			wgui_change_inputbox_mode_smart_RUSSIAN=wgui_EMS_inputbox_change_input_mode_smart_RUSSIAN;
			wgui_change_inputbox_mode_smart_russian=wgui_EMS_inputbox_change_input_mode_smart_russian;
		#endif

		#if defined(__MMI_ZI_TURKISH__)
			wgui_change_inputbox_mode_smart_TURKISH=wgui_EMS_inputbox_change_input_mode_smart_TURKISH;
			wgui_change_inputbox_mode_smart_turkish=wgui_EMS_inputbox_change_input_mode_smart_turkish;
		#endif
		 
		#if defined(__MMI_T9_PORTUGUESE__)||defined(__MMI_ZI_EU_PORTUGUESE__)||defined(__MMI_ZI_SA_PORTUGUESE__)||defined(__MMI_ZI_BZ_PORTUGUESE__)
		 
			wgui_change_inputbox_mode_smart_PORTUGUESE=wgui_EMS_inputbox_change_input_mode_smart_PORTUGUESE;
			wgui_change_inputbox_mode_smart_portuguese=wgui_EMS_inputbox_change_input_mode_smart_portuguese;
		#endif
		 
		#if defined(__MMI_ZI_INDONESIAN__) ||defined(__MMI_T9_INDONESIAN__)
			wgui_change_inputbox_mode_smart_INDONESIAN=wgui_EMS_inputbox_change_input_mode_smart_INDONESIAN;
			wgui_change_inputbox_mode_smart_indonesian=wgui_EMS_inputbox_change_input_mode_smart_indonesian;
		#endif
		 
		#if defined(__MMI_ZI_MALAY__)
			wgui_change_inputbox_mode_smart_MALAY=wgui_EMS_inputbox_change_input_mode_smart_MALAY;
			wgui_change_inputbox_mode_smart_malay=wgui_EMS_inputbox_change_input_mode_smart_malay;
		#endif
 
		#if defined(__MMI_ZI_VIETNAMESE__)||defined(__MMI_T9_VIETNAMESE__)
 
			wgui_change_inputbox_mode_smart_VIETNAMESE=wgui_EMS_inputbox_change_input_mode_smart_VIETNAMESE;
			wgui_change_inputbox_mode_smart_vietnamese=wgui_EMS_inputbox_change_input_mode_smart_vietnamese;
		#endif
	//CSD end
	#endif

	wgui_inputbox_insert_symbol=wgui_EMS_inputbox_insert_symbol;
	 
	if (0==test_change_input_mode())
	{
		switch (input_type & INPUT_TYPE_MASK)
		{
			case INPUT_TYPE_SM_STROKE:
			case INPUT_TYPE_SM_PINYIN:
			case INPUT_TYPE_SM_MULTITAP_PINYIN:
			case INPUT_TYPE_TR_STROKE:
			case INPUT_TYPE_TR_BOPOMO:
			case INPUT_TYPE_TR_MULTITAP_BOPOMO:
			case INPUT_TYPE_SMART_THAI:
			case INPUT_TYPE_SMART_ARABIC:
#if defined(__MMI_MULTITAP_RUSSIAN__)
			case INPUT_TYPE_ALPHANUMERIC_UPPERCASE_RUSSIAN:
			case INPUT_TYPE_ALPHANUMERIC_LOWERCASE_RUSSIAN:
			case INPUT_TYPE_MMI_LOWERCASE_RUSSIAN:
#endif
#if defined(__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
			case INPUT_TYPE_SMART_UPPERCASE_RUSSIAN:
			case INPUT_TYPE_SMART_LOWERCASE_RUSSIAN:
#endif
#if defined(__MMI_ZI_VIETNAMESE__)
			case INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
			case INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
#endif
#if defined(__MMI_MULTITAP_HINDI__)
			case INPUT_TYPE_MULTITAP_HINDI:
#endif

				input_type = INPUT_TYPE_ALPHANUMERIC_LOWERCASE;
				break;
			default :
				break;
		}
	}
	if(wgui_EMS_inputbox_input_mode_changed)
	{	input_type=wgui_EMS_inputbox_input_type;
		wgui_EMS_inputbox_input_mode_changed=0;
	}
	MMI_current_input_type=input_type;
	wgui_EMS_inputbox_information_icon=NULL;
	 
	 
	mmi_trace(g_sw_IM, "ShowCategory28Screen input_type = %d",input_type);
	switch(input_type)
	{	case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE:
				mmi_trace(g_sw_IM, "ShowCategory28Screen INPUT_TYPE_ALPHANUMERIC_LOWERCASE");
				MMI_current_input_mode=1;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE:
			mmi_trace(g_sw_IM, "ShowCategory28Screen INPUT_TYPE_ALPHANUMERIC_UPPERCASE");
				MMI_current_input_mode=0;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_NUMERIC:
		case	INPUT_TYPE_NUMERIC_CHANGEABLE:
				MMI_current_input_mode=2;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#if defined(__MMI_SMART_RUSSIAN__)
		case	INPUT_TYPE_SMART_UPPERCASE_RUSSIAN:
				MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_RUSSIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_SMART_LOWERCASE_RUSSIAN:
				MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_RUSSIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif

#if defined(__MMI_GB_TR_CHINESE__)
		case INPUT_TYPE_TR_BOPOMO :
				MMI_current_input_mode = INPUT_MODE_TR_BOPOMO ;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_BPMF_IMG);
				break;
#endif

#if defined(__MMI_GB_SMART_PORTUGUESE__)
		case INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE :
				MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_PORTUGUESE;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
				MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_PORTUGUESE;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif

#if defined(__MMI_SMART_ARABIC__)
		case INPUT_TYPE_SMART_ARABIC:
                            MMI_current_input_mode=INPUT_MODE_SMART_ARABIC;
                            wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
                            break;
#endif
#if defined(__MMI_MULTITAP_ARABIC__)
		case INPUT_TYPE_MMI_MULTITAP_ARABIC:
                            MMI_current_input_mode=INPUT_MODE_MULTITAP_ARABIC;
                            wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
                            break;
#endif
#if defined(__MMI_SMART_PERSIAN__)
		case INPUT_TYPE_SMART_PERSIAN:
                            MMI_current_input_mode=INPUT_MODE_SMART_PERSIAN;
                            wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
                            break;
#endif
#if defined(__MMI_GB__)
		case INPUT_TYPE_SM_STROKE:
			MMI_current_input_mode = INPUT_MODE_SM_STROKE;
			wgui_EMS_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);           //add a picture for IME
			break;
		case INPUT_TYPE_SM_PINYIN:
			MMI_current_input_mode = INPUT_MODE_SM_PINYIN;
			wgui_EMS_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);       //add a picture for IME
			break;
		case INPUT_TYPE_SMART_UPPERCASE_ABC:
			MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_ABC;
			wgui_EMS_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);  
			break;
		case INPUT_TYPE_SMART_LOWERCASE_ABC:
			MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_ABC;
			wgui_EMS_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);  
			break;	
#endif
 
#if defined(__MMI_MULTITAP_THAI__)
		case	INPUT_TYPE_ALPHANUMERIC_MULTITAP_THAI:
				SetKeyHandler(handle_category28_change_input_mode_bymultitapthai,KEY_POUND,KEY_LONG_PRESS);
				MMI_current_input_mode=INPUT_MODE_MULTITAP_THAI;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif

#if defined(__MMI_SMART_THAI__)
		case INPUT_TYPE_SMART_THAI:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_THAI;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
                                                    break;
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
		case INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
				MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_VIETNAMESE;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
				MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_VIETNAMESE;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
					
#endif
 
#if defined(__MMI_MULTITAP_SPANISH__)
		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_SPANISH:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_LOWERCASE_SPANISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_SPANISH:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_UPPERCASE_SPANISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif

//qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
		case	INPUT_TYPE_MMI_MULTITAP_TAMIL:
				MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_TAMIL;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
		case	INPUT_TYPE_MMI_MULTITAP_BENGALI:
				MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_BENGALI;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_PUNJABI__)
		case	INPUT_TYPE_MMI_MULTITAP_PUNJABI:
				MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_PUNJABI;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_TELUGU__)
		case	INPUT_TYPE_MMI_MULTITAP_TELUGU:
				MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_TELUGU;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
		case	INPUT_TYPE_MMI_MULTITAP_KANNADA:
				MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_KANNADA;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
		case	INPUT_TYPE_MMI_MULTITAP_MALAYALAM:
				MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_MALAYALAM;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
		case	INPUT_TYPE_MMI_MULTITAP_ORIYA:
				MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_ORIYA;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
//qiff add end
#if defined(__MMI_MULTITAP_MARATHI__)
		case	INPUT_TYPE_MMI_MULTITAP_MARATHI:
				MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_MARATHI;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif

#if defined(__MMI_MULTITAP_URDU__)
		case	INPUT_TYPE_MMI_MULTITAP_URDU:
				MMI_current_input_mode=INPUT_MODE_MMI_MULTITAP_URDU;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif

 
#if defined(__MMI_MULTITAP_DANISH__)
		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_DANISH:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_LOWERCASE_DANISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_DANISH:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_UPPERCASE_DANISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
 
#if defined(__MMI_MULTITAP_FRENCH__)
		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_FRENCH:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_LOWERCASE_FRENCH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_FRENCH:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_UPPERCASE_FRENCH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_MYANMAR__)
		case	INPUT_TYPE_MMI_MULTITAP_MYANMAR:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_MYANMAR;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_SMART_FRENCH__)
		case	INPUT_TYPE_SMART_LOWERCASE_FRENCH:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_FRENCH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_FRENCH:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_FRENCH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_SMART_URDU__)
		case	INPUT_TYPE_SMART_URDU:
				MMI_current_input_mode=INPUT_MODE_SMART_URDU;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_SMART_PUNJABI__)
		case	INPUT_TYPE_SMART_PUNJABI:
				MMI_current_input_mode=INPUT_MODE_SMART_PUNJABI;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_SMART_BENGALI__)
		case	INPUT_TYPE_SMART_BENGALI:
				MMI_current_input_mode=INPUT_MODE_SMART_BENGALI;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_SMART_TAMIL__)
		case	INPUT_TYPE_SMART_TAMIL:
				MMI_current_input_mode=INPUT_MODE_SMART_TAMIL;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_SMART_TURKISH__)
		case	INPUT_TYPE_SMART_LOWERCASE_TURKISH:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_TURKISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_TURKISH:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_TURKISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_SMART_INDONESIAN__)
		case	INPUT_TYPE_SMART_LOWERCASE_INDONESIAN:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_INDONESIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_INDONESIAN:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_INDONESIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_SMART_MALAY__)
		case	INPUT_TYPE_SMART_LOWERCASE_MALAY:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_MALAY;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_MALAY:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_MALAY;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
// beginning--added by kecx for adding italian on 20081223
#if defined(__MMI_SMART_ITALIAN__)
		case	INPUT_TYPE_SMART_LOWERCASE_ITALIAN:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_ITALIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_ITALIAN:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_ITALIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
// end--added by kecx for adding italian on 20081223

//beginning:guoyt added for adding German,Spanish and Hindi IME
#if defined(__MMI_SMART_GERMAN__)		
		case	INPUT_TYPE_SMART_LOWERCASE_GERMAN:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_GERMAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_GERMAN:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_GERMAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_SMART_SPANISH__)
		case INPUT_TYPE_SMART_UPPERCASE_SPANISH:
			MMI_current_input_mode = INPUT_MODE_SMART_UPPERCASE_SPANISH;
			wgui_EMS_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
			break;
		case INPUT_TYPE_SMART_LOWERCASE_SPANISH:
			MMI_current_input_mode = INPUT_MODE_SMART_LOWERCASE_SPANISH;
			wgui_EMS_inputbox_information_icon = get_image(WGUI_INPUT_INFORMATION_ICON1);
			break;
#endif
#if defined(__MMI_SMART_HINDI__)				
		case  INPUT_TYPE_SMART_HINDI:			
            MMI_current_input_mode=INPUT_MODE_SMART_HINDI;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
//end:guoyt added for adding German,Spanish and Hindi IME

#if defined(__MMI_SMART_TELUGU__)				
		case  INPUT_TYPE_SMART_TELUGU:			
				MMI_current_input_mode=INPUT_MODE_SMART_TELUGU;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif

#if defined(__MMI_MULTITAP_GERMAN__)
		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_GERMAN:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_LOWERCASE_GERMAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_GERMAN:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_UPPERCASE_GERMAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_ITALIAN__)
		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_ITALIAN:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_LOWERCASE_ITALIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_ITALIAN:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_UPPERCASE_ITALIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_RUSSIAN__)
		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_RUSSIAN:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_LOWERCASE_RUSSIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_RUSSIAN:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_UPPERCASE_RUSSIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_HINDI__)
		case  INPUT_TYPE_MULTITAP_HINDI:
            MMI_current_input_mode=INPUT_MODE_MULTITAP_HINDI;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif

#if defined(__MMI_MULTITAP_PORTUGUESE__)
		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_PORTUGUESE:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_LOWERCASE_PORTUGUESE;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_PORTUGUESE:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_UPPERCASE_PORTUGUESE;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_TURKISH__)
		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_TURKISH:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_LOWERCASE_TURKISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_TURKISH:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_UPPERCASE_TURKISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_INDONESIAN__)
		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_INDONESIAN:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_LOWERCASE_INDONESIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_INDONESIAN:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_UPPERCASE_INDONESIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_MALAY__)
		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_MALAY:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_LOWERCASE_MALAY;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_MALAY:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_UPPERCASE_MALAY;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
#if defined(__MMI_MULTITAP_VIETNAMESE__)
		case	INPUT_TYPE_ALPHANUMERIC_LOWERCASE_VIETNAMESE:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_LOWERCASE_VIETNAMESE;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
		case	INPUT_TYPE_ALPHANUMERIC_UPPERCASE_VIETNAMESE:
				MMI_current_input_mode=INPUT_MODE_MULTITAP_UPPERCASE_VIETNAMESE;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
				break;
#endif
//CSD end
        #if defined(__MMI_T9__) || defined(__MMI_ITAP__)
        case INPUT_TYPE_TR_MULTITAP_BOPOMO:
                                                    MMI_current_input_mode=INPUT_MODE_TR_MULTITAP_BOPOMO;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_MULTITAP_BPMF_IMG);
                                                    break;

        case INPUT_TYPE_SM_MULTITAP_PINYIN:
                                                    MMI_current_input_mode=INPUT_MODE_SM_MULTITAP_PINYIN;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_MULTITAP_PINYIN_IMG);
                                                    break;

        case INPUT_TYPE_TR_STROKE:
                                                    MMI_current_input_mode=INPUT_MODE_TR_STROKE;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_TR_STROKE_IMG);
                                                    break;

        case INPUT_TYPE_SM_STROKE:
                                                    MMI_current_input_mode=INPUT_MODE_SM_STROKE;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SM_STROKE_IMG);
                                                    break;
        case INPUT_TYPE_SMART_UPPERCASE_ABC:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_ABC;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
                                                    break;

        case INPUT_TYPE_SMART_LOWERCASE_ABC:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_ABC;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
                                                    break;
        case INPUT_TYPE_TR_BOPOMO:
                                                    MMI_current_input_mode=INPUT_MODE_TR_BOPOMO;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_BPMF_IMG);
                                                    break;

        case INPUT_TYPE_SM_PINYIN:
                                                    MMI_current_input_mode=INPUT_MODE_SM_PINYIN;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_PINYIN_IMG);
                                                    break;
 
#if defined (__MMI_T9_THAI__)
        case INPUT_TYPE_SMART_THAI:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_THAI;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
                                                    break;
#endif
 
#if defined(__MMI_T9_SPANISH__)
		case	INPUT_TYPE_SMART_LOWERCASE_SPANISH:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_SPANISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_SPANISH:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_SPANISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
#endif

 
#if defined(__MMI_T9_DANISH__)
		case	INPUT_TYPE_SMART_LOWERCASE_DANISH:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_DANISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_DANISH:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_DANISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
#endif
 
#if defined(__MMI_T9_FRENCH__)
		case	INPUT_TYPE_SMART_LOWERCASE_FRENCH:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_FRENCH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_FRENCH:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_FRENCH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
#endif
#if defined(__MMI_T9_GERMAN__)
		case	INPUT_TYPE_SMART_LOWERCASE_GERMAN:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_GERMAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_GERMAN:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_GERMAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
#endif
#if defined(__MMI_T9_ITALIAN__)
		case	INPUT_TYPE_SMART_LOWERCASE_ITALIAN:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_ITALIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_ITALIAN:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_ITALIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
#endif

 

#if defined(__MMI_T9_VIETNAMESE__)
		case	INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_VIETNAMESE;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_VIETNAMESE;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
#endif

 

//CSD end
 
#if defined(__MMI_T9_RUSSIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_RUSSIAN:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_RUSSIAN;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
                                                    break;

        case INPUT_TYPE_SMART_LOWERCASE_RUSSIAN:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_RUSSIAN;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
                                                    break;
#endif
 
#if defined(__MMI_T9_INDONESIAN__)
		case	INPUT_TYPE_SMART_LOWERCASE_INDONESIAN:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_INDONESIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_INDONESIAN:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_INDONESIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
				break;
#endif
 

 
#if defined(__MMI_T9_PORTUGUESE__)
        case INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE:
             MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_PORTUGUESE;
             wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
             break;

        case INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
             MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_PORTUGUESE;
             wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_ENGLISH_IMG);
             break;
#endif
 
//END 
        #elif defined(__MMI_ZI__)
        case INPUT_TYPE_TR_MULTITAP_BOPOMO:
                                                    MMI_current_input_mode=INPUT_MODE_TR_MULTITAP_BOPOMO;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_BPMF_LOGO);
                                                    break;

        case INPUT_TYPE_SM_MULTITAP_PINYIN:
                                                    MMI_current_input_mode=INPUT_MODE_SM_MULTITAP_PINYIN;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_PINYIN_LOGO);
                                                    break;

        case INPUT_TYPE_TR_BOPOMO:
                                                    MMI_current_input_mode=INPUT_MODE_TR_BOPOMO;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_BPMF_LOGO);
                                                    break;

        case INPUT_TYPE_SM_PINYIN:
                                                    MMI_current_input_mode=INPUT_MODE_SM_PINYIN;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_PINYIN_LOGO);
                                                    break;

        case INPUT_TYPE_TR_STROKE:
                                                    MMI_current_input_mode=INPUT_MODE_TR_STROKE;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_TR_STROKE_LOGO);
                                                    break;

        case INPUT_TYPE_SM_STROKE:
                                                    MMI_current_input_mode=INPUT_MODE_SM_STROKE;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_SM_STROKE_LOGO);
                                                    break;
        case INPUT_TYPE_SMART_UPPERCASE_ABC:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_ABC;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
                                                    break;

        case INPUT_TYPE_SMART_LOWERCASE_ABC:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_ABC;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
                                                    break;
        case INPUT_TYPE_SMART_THAI:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_THAI;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
                                                    break;
#if defined(__MMI_SMART_ARABIC__)
		case INPUT_TYPE_SMART_ARABIC:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_ARABIC;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
                                                    break;
#endif
#if defined(__MMI_SMART_PERSIAN__)
		case INPUT_TYPE_SMART_PERSIAN:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_PERSIAN;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
                                                    break;
#endif

 
#if defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)
		case	INPUT_TYPE_SMART_LOWERCASE_SPANISH:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_SPANISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_SPANISH:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_SPANISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
#endif
 
#if defined(__MMI_ZI_EU_DANISH__)||defined(__MMI_ZI_SA_DANISH__)
		case	INPUT_TYPE_SMART_LOWERCASE_DANISH:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_DANISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_DANISH:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_DANISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
#endif
 
#if defined(__MMI_ZI_CA_FRENCH__)||defined(__MMI_ZI_EU_FRENCH__)
		case	INPUT_TYPE_SMART_LOWERCASE_FRENCH:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_FRENCH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_FRENCH:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_FRENCH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
#endif
#if defined(__MMI_ZI_GERMAN__)
		case	INPUT_TYPE_SMART_LOWERCASE_GERMAN:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_GERMAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_GERMAN:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_GERMAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
#endif
#if defined(__MMI_ZI_ITALIAN__)
		case	INPUT_TYPE_SMART_LOWERCASE_ITALIAN:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_ITALIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_ITALIAN:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_ITALIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
#endif
#if defined(__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
        case INPUT_TYPE_SMART_UPPERCASE_RUSSIAN:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_RUSSIAN;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
                                                    break;

        case INPUT_TYPE_SMART_LOWERCASE_RUSSIAN:
                                                    MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_RUSSIAN;
                                                    wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
                                                    break;
#endif
#if defined(__MMI_ZI_TURKISH__)
		case	INPUT_TYPE_SMART_LOWERCASE_TURKISH:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_TURKISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_TURKISH:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_TURKISH;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
#endif
#if defined(__MMI_ZI_EU_PORTUGUESE__)||defined(__MMI_ZI_SA_PORTUGUESE__)||defined(__MMI_ZI_BZ_PORTUGUESE__)
        case INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE:
             MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_PORTUGUESE;
             wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
             break;

        case INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
             MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_PORTUGUESE;
             wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
             break;
#endif
#if defined(__MMI_ZI_INDONESIAN__)
		case	INPUT_TYPE_SMART_LOWERCASE_INDONESIAN:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_INDONESIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_INDONESIAN:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_INDONESIAN;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
#endif
#if defined(__MMI_ZI_MALAY__)
		case	INPUT_TYPE_SMART_LOWERCASE_MALAY:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_MALAY;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_MALAY:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_MALAY;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
#endif
#if defined(__MMI_ZI_VIETNAMESE__)
		case	INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
				MMI_current_input_mode=INPUT_MODE_SMART_LOWERCASE_VIETNAMESE;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
		case	INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
				MMI_current_input_mode=INPUT_MODE_SMART_UPPERCASE_VIETNAMESE;
				wgui_EMS_inputbox_information_icon=get_image(WGUI_ZI_EZITEXT_LOGO);
				break;
#endif
//CSD end

        #elif defined(__MMI_KA__)
    case INPUT_TYPE_SM_PINYIN:
        MMI_current_input_mode=INPUT_MODE_SM_PINYIN;
        wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SMART_PINYIN_IMG);
        break;

    case INPUT_TYPE_SM_STROKE:
	      MMI_current_input_mode=INPUT_MODE_SM_STROKE;
	      wgui_EMS_inputbox_information_icon=get_image(WGUI_IME_SM_STROKE_IMG);
	      break;
        #endif
	/*#if defined(__MMI_GB__)
    case INPUT_TYPE_SM_PINYIN:
		MMI_current_input_mode=11;
		wgui_EMS_inputbox_information_icon=get_image(WGUI_INPUT_INFORMATION_ICON1);
        break;
	#endif*/
		default:
				MMI_current_input_mode=0;
				break;
	}
	category28_RSK_label_icon=get_image(right_softkey_icon);
	category28_RSK_label_string=get_string(right_softkey);
	change_EMS_inputbox_input_mode();
#if defined(__MMI_TOUCH_SCREEN__)
	ext_stroke.x1 = 0;
	ext_stroke.y1 = 0;
	ext_stroke.x2 = UI_device_width - 1;
	ext_stroke.y2 = UI_device_height - 1;
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else
	stroke_area.x1 = MMI_content_x;
	stroke_area.x2 = MMI_content_x + MMI_content_width -MMI_EMS_inputbox.vbar.width;
	stroke_area.y1 = MMI_title_y;
	stroke_area.y2 = MMI_title_y + MMI_content_height + MMI_title_height;
#endif
	mmi_pen_editor_setup_input_box(&stroke_area,&ext_stroke,MMI_current_input_type,MMI_TRUE,PEN_EDITOR_EMS_INPUT_BOX);
#endif

#if(EMS_USE_INTERNAL_MENU)
	SetKeyHandler(EntryEMSMainMenu,KEY_SEND,KEY_EVENT_UP);
#endif
//	test_change_input_mode=test_EMS_inputbox_change_mode;
//	SetKeyHandler(temp_ems_insert_character,KEY_STAR,KEY_EVENT_DOWN);
#endif /* UI_DISABLE_EMS_INPUT_BOX */
	coolsand_UI_unlock_double_buffer();

	ExitCategoryFunction=ExitCategory28Screen;
#ifdef __MMI_DRAW_MANAGER__
		dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
		dm_register_category_controlled_callback(category28callback);
		dm_data.S32ScrId = (S32)GetActiveScreenId();
		dm_data.S32CatId = MMI_CATEGORY28_ID;
		dm_data.S32flags=DM_CLEAR_SCREEN_BACKGROUND;
		dm_data.S32flags|=DM_SHOW_VKPAD;
		dm_setup_data(&dm_data);
 
      dm_register_vkpad_callback(cat28_virtual_keypad_callback);
 
		dm_redraw_category_screen();
#else
	RedrawCategoryFunction=RedrawCategory28Screen;
	GetCategoryHistory=GetCategory28History;
	GetCategoryHistorySize=GetCategory28HistorySize;
	RedrawCategory28Screen();
#endif
}

void wgui_set_EMS_inputbox_RSK(void)
{
	TBM_ENTRY(0x2872);
	SetKeyHandler(handle_category28_right_softkey_long_press, KEY_RSK, KEY_LONG_PRESS);
	category28_RSK_label_clear=0;
   #if defined(__MMI_WITH_C_KEY__)
	SetKeyHandler(handle_category28_right_softkey_down, KEY_CLEAR, KEY_EVENT_DOWN);
	SetKeyHandler(handle_category28_right_softkey_long_press, KEY_CLEAR, KEY_LONG_PRESS);
	set_right_softkey_function(handle_category28_right_softkey_up,KEY_EVENT_UP);
   #else
	set_right_softkey_function(handle_category28_right_softkey_down,KEY_EVENT_DOWN);
	set_right_softkey_function(handle_category28_right_softkey_long_press,KEY_LONG_PRESS);
   #endif
   
#ifdef __MMI_FULL_KEYPAD__
	SetKeyHandler(handle_category28_right_softkey_down, KEY_BACKSPACE1, KEY_EVENT_DOWN);
	SetKeyHandler(handle_category28_right_softkey_long_press, KEY_BACKSPACE1, KEY_LONG_PRESS);
#endif
   
	register_EMS_inputbox_input_callback(handle_category28_input);
	handle_category28_input();
	if(coolsand_UI_EMS_inputbox_empty(&MMI_EMS_inputbox))
	{
	   #if defined(__MMI_WITH_C_KEY__)
		SetKeyHandler(handle_category28_right_softkey_up, KEY_CLEAR, KEY_EVENT_UP);
	   #else
		set_right_softkey_function(handle_category28_right_softkey_up,KEY_EVENT_UP);
	   #endif
	}
	TBM_EXIT(0x2872);

}
/*----------------------------------------------------------------------------
Function:			CloseCategory28Screen
Description:		Releases data used by the MO EMS/SMS screen
Input Parameters:	none
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

void CloseCategory28Screen(void)
{
#if(UI_DISABLE_EMS_INPUT_BOX)
	S32				length;
	S32				i,j;
	U8*				buffer;

	length=MMI_multiline_inputbox.text_length;
	buffer=(U8*)coolsand_UI_malloc((length<<1)+2);
	i=j=0;
	ResetCurrentPosition(temp_EMS_data);
	while(i<length)
	{	buffer[j++]=0;
		buffer[j++]=(U8)temp_EMS_buffer[i++];
	}
	buffer[j++]=0;
	buffer[j++]='\0';
	AddString(temp_EMS_data,buffer,(U16)length,NULL);
	coolsand_UI_free(buffer);
	coolsand_UI_free(temp_EMS_buffer);
#endif
}

/*----------------------------------------------------------------------------
Function:			ExitCategory28Screen
Description:		Exits the MO EMS/SMS screen
Input Parameters:	none
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

void ExitCategory28Screen(void)
{
   EMS_inputbox_has_changed=0;
   //	set_MMI_current_input_type();
   category28_RSK_label_clear=0;
   category28_RSK_function=NULL;
   wgui_inputbox_information_flag=0;
#ifdef __MMI_MESSAGES_EMS__
   close_EMS_objects();
#endif
#if(!UI_DISABLE_EMS_INPUT_BOX)
   {
      reset_EMS_inputbox();
   }
#endif

    
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
	 
	dm_set_scr_bg_image_no_draw(editor_scr_bg_ID, editor_scr_bg_filename, editor_scr_bg_x, editor_scr_bg_y, editor_scr_bg_opacity);
#endif
    

   ExitCategoryFunction=MMI_dummy_function;
   RedrawCategoryFunction=MMI_dummy_function;
   GetCategoryHistory=dummy_get_history;
   GetCategoryHistorySize=dummy_get_history_size;
#ifdef __MMI_T9__
   InuptMethodExitCategory28();
#elif defined (__MMI_ZI__) && defined(__MMI_MESSAGES_EMS__)
   ZiInuptMethodExitCategory28();
#elif defined __MMI_KA__
   KonkaInuptMethodExitCategory28();
#elif defined __MMI_ITAP__
   ItapInuptMethodExitCategory28();
#elif defined __MMI_GB__
   GbInuptMethodExitCategory28();
#endif
#if defined(__MMI_TOUCH_SCREEN__)
   mmi_pen_editor_close_input_box();
#endif
   if(test_change_input_mode!=NULL)
   {
      if(test_change_input_mode())
      {
         MMI_english_input_mode_flag=0;
      }
      else
      {
         MMI_english_input_mode_flag=1;
      }
   }
   else
   {
      MMI_english_input_mode_flag=0;
   }
   test_change_input_mode=NULL;
	 
	set_current_input_box_type(EDITOR_NONE_INPUT_BOX);
}

/*----------------------------------------------------------------------------
Function:			GetCategory28HistorySize
Description:		Gets the history buffer size for the MO EMS/SMS screen
Input Parameters:	none
Output Parameters:	none
Returns:			size in U8s of the history buffer
----------------------------------------------------------------------------*/

S32 GetCategory28HistorySize(void)
{
#if(UI_DISABLE_EMS_INPUT_BOX)
	return(0);
#else
	return(sizeof(EMS_inputbox_category_history));
#endif
}

/*----------------------------------------------------------------------------
Function:			GetCategory28History
Description:		Gets the history buffer for the MO EMS/SMS screen
Input Parameters:	history_buffer	is the buffer into which the history data is stored (pre-allocated)
Output Parameters:	none
Returns:			pointer to the history buffer
----------------------------------------------------------------------------*/

U8* GetCategory28History(U8* history_buffer)
{
#if(UI_DISABLE_EMS_INPUT_BOX)
	return(history_buffer);
#else
	get_EMS_inputbox_category_history(MMI_CATEGORY28_ID,history_buffer,MMI_current_input_type);
	return(history_buffer);
#endif
}

#if(ENABLE_EMS_TEST_DEMO)
 
/*----------------------------------------------------------------------------
Function:			RedrawCategory29Screen
Description:		Redraws the MT EMS/SMS screen
Input Parameters:	none
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

void RedrawCategory29Screen(void)
{
	gdi_layer_lock_frame_buffer();

	clear_screen();
	show_title_status_icon();
	draw_title();
	show_softkey_background();
	show_left_softkey();
	show_right_softkey();
#if(UI_DISABLE_EMS_INPUT_BOX)
	show_multiline_inputbox();
#else
	show_EMS_inputbox();
#endif
	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0,0,UI_device_width-1,UI_device_height-1);
}

/*----------------------------------------------------------------------------
Function:			ShowCategory29Screen
Description:		Displays the MT EMS/SMS screen
Input Parameters:	[1]	STRING_ID	title,				Title for the screen
					[2]	IMAGE_ID	title_icon,			Icon shown with the title
					[3]	STRING_ID	left_softkey,		Left softkey label
					[4]	IMAGE_ID	left_softkey_icon,	Icon for the left softkey
					[5]	STRING_ID	right_softkey,		Right softkey label
					[6]	IMAGE_ID	right_softkey_icon,	Icon for the right softkey
					[7]	EMSData*	data				EMSData (Pre-allocated and initialized)
					[8]	BYTE*		history_buffer		history buffer
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

void ShowCategory29Screen(U16 title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,EMSData* data,U8* history_buffer)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
#ifdef __MMI_DRAW_MANAGER__
	dm_data_struct dm_data;
#endif
	s16		input_type=0;
#if(UI_DISABLE_EMS_INPUT_BOX)
	S32		length;
	S32		i,j,h,fh;

	UI_UNUSED_PARAMETER(history_buffer);
#else
	byte	h;
#endif

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	EMS_inputbox_has_changed=0;
#ifdef __MMI_MESSAGES_EMS__
	initialize_EMS_objects();
#endif
	MMI_menu_shortcut_number=-1;
	change_left_softkey(left_softkey,left_softkey_icon);
	change_right_softkey(right_softkey,right_softkey_icon);
	SetupCategoryKeyHandlers();

	MMI_title_string=(UI_string_type)get_string(title);
	MMI_title_icon=(UI_image_type)get_image(title_icon);

#if(UI_DISABLE_EMS_INPUT_BOX)
	length=(data->textBufferSize>>1);
	if(length==0)
	{	UI_string_type	s=(UI_string_type)"EMS Data not initialized";
		S32				l=coolsand_UI_strlen((UI_string_type)s);
		temp_EMS_buffer=(UI_string_type)coolsand_UI_malloc(l*sizeof(UI_character_type));
		coolsand_UI_strcpy((UI_string_type)temp_EMS_buffer,(UI_string_type)s);
		length=l+1;
		i=l;
	}
	else
	{
		temp_EMS_buffer=(UI_string_type)coolsand_UI_malloc(length*sizeof(UI_character_type));
		i=j=0;
		while(j<data->textLength)
		{
			if((data->textBuffer[j]=='\0') && (data->textBuffer[j+1]=='\0'))
			{	temp_EMS_buffer[i]='\0';
				break;
			}
			else temp_EMS_buffer[i]=data->textBuffer[j+1];
			i++;
			j+=2;
		}
	}
	temp_EMS_data=data;
	create_multiline_inputbox_set_buffer(temp_EMS_buffer,length,i,0);
	register_multiline_inputbox_keys();
	register_hide_multitap(category5_hide_multitap);
	move_multiline_inputbox(0,(MMI_title_y+MMI_title_height)+2);
	h=MMI_content_height-MMI_multitap_height-8;
	coolsand_UI_set_font(MMI_multiline_inputbox.text_font);
	fh=coolsand_UI_get_character_height();
	h=((h/fh)*fh)+8;
	resize_multiline_inputbox(MMI_multiline_inputbox.width,h);
	set_multiline_inputbox_mask(0);
#else
#if((UI_ENABLE_FRAME_SKIPPING)&&(ENABLE_EMS_INPUTBOX_FRAME_SKIPPING))
	wgui_EMS_inputbox_frame_counter=0;
#endif
	h=set_EMS_inputbox_category_history(MMI_CATEGORY29_ID,history_buffer,(s16*)&input_type);
	coolsand_UI_create_EMS_input_box(&MMI_EMS_inputbox,0,(MMI_title_y+MMI_title_height),MMI_content_width,MMI_content_height+2);
	MMI_EMS_inputbox.flags|=UI_EMS_INPUT_BOX_VIEW_MODE;
	if(!h)
	{	coolsand_UI_set_EMS_input_box_data(&MMI_EMS_inputbox,data);
		ResetCurrentPosition(MMI_EMS_inputbox.data);
	}
	register_EMS_inputbox_viewer_keys();
#endif
	ExitCategoryFunction=ExitCategory29Screen;
#ifdef __MMI_DRAW_MANAGER__
	dm_setup_category_functions(dm_redraw_category_screen, GetCategory29History, GetCategory29HistorySize);
	dm_data.S32ScrId = (S32)GetActiveScreenId();
	dm_data.S32CatId = MMI_CATEGORY29_ID;
	dm_data.S32flags=DM_CLEAR_SCREEN_BACKGROUND;
	dm_setup_data(&dm_data);
	dm_redraw_category_screen();
#else
	RedrawCategoryFunction=RedrawCategory29Screen;
	GetCategoryHistory=GetCategory29History;
	GetCategoryHistorySize=GetCategory29HistorySize;
	RedrawCategory29Screen();
#endif
}

/*----------------------------------------------------------------------------
Function:			CloseCategory29Screen
Description:		Releases data used by the MT EMS/SMS screen
Input Parameters:	none
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

void CloseCategory29Screen(void)
{
#if(UI_DISABLE_EMS_INPUT_BOX)
	S32				length;
	S32				i,j;
	U8*				buffer;

	length=MMI_multiline_inputbox.text_length;
	buffer=(U8*)coolsand_UI_malloc((length<<1)+2);
	i=j=0;
	ResetCurrentPosition(temp_EMS_data);
	while(i<length)
	{	buffer[j++]=0;
		buffer[j++]=(U8)temp_EMS_buffer[i++];
	}
	buffer[j++]=0;
	buffer[j++]='\0';
	AddString(temp_EMS_data,buffer,(U16)length,NULL);
	coolsand_UI_free(buffer);
	coolsand_UI_free(temp_EMS_buffer);
#endif
}

/*----------------------------------------------------------------------------
Function:			ExitCategory29Screen
Description:		Exits the MT EMS/SMS screen
Input Parameters:	none
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

void ExitCategory29Screen(void)
{
#if(!UI_DISABLE_EMS_INPUT_BOX)
	reset_EMS_inputbox();
#endif
#ifdef __MMI_MESSAGES_EMS__
	close_EMS_objects();
#endif
	ExitCategoryFunction=MMI_dummy_function;
	RedrawCategoryFunction=MMI_dummy_function;
	GetCategoryHistory=dummy_get_history;
	GetCategoryHistorySize=dummy_get_history_size;
}

/*----------------------------------------------------------------------------
Function:			GetCategory29HistorySize
Description:		Gets the history buffer size for the MT EMS/SMS screen
Input Parameters:	none
Output Parameters:	none
Returns:			size in U8s of the history buffer
----------------------------------------------------------------------------*/

S32 GetCategory29HistorySize(void)
{
#if(UI_DISABLE_EMS_INPUT_BOX)
	return(0);
#else
	return(sizeof(EMS_inputbox_category_history));
#endif

}

/*----------------------------------------------------------------------------
Function:			GetCategory29History
Description:		Gets the history buffer for the MT EMS/SMS screen
Input Parameters:	history_buffer	is the buffer into which the history data is stored (pre-allocated)
Output Parameters:	none
Returns:			pointer to the history buffer
----------------------------------------------------------------------------*/

U8* GetCategory29History(U8* history_buffer)
{
#if(UI_DISABLE_EMS_INPUT_BOX)
	return(history_buffer);
#else
	get_EMS_inputbox_category_history(MMI_CATEGORY28_ID,history_buffer,MMI_current_input_type);
	return(history_buffer);
#endif
}
#endif

/*	If EMS_VIEWER_HEADER_HEIGHT_AUTO is 1, then EMS_VIEWER_HEADER_HEIGHT defines
	the maximum possible height for the header. Otherwise, it defines the fixed
	height of the header	*/

#define EMS_VIEWER_HEADER_HEIGHT				48
#define EMS_VIEWER_HEADER_HEIGHT_AUTO			1

/*	If 1, uses multiline input box to display the header (word wrapping, no truncation),
	else uses normal text output (no word wrapping, truncation for lines longer than screen width)		*/

#define EMS_VIEWER_HEADER_MULTILINE_INPUTBOX	0

UI_filled_area EMS_viewer_header_background_filler=
{	UI_FILLED_AREA_TYPE_COLOR,
	UI_NULL_IMAGE,
	NULL,
	{	180,220,220,100	},
	{	0,0,0,0			},
	{	0,0,0,0			},
	{	0,0,0,0			},
	0
};

#if(EMS_VIEWER_HEADER_MULTILINE_INPUTBOX)

multi_line_input_box EMS_viewer_header_inputbox;

UI_multi_line_input_box_theme EMS_viewer_header_theme=
{	&EMS_viewer_header_background_filler,
	&EMS_viewer_header_background_filler,
	&EMS_viewer_header_background_filler,
	{	0,0,0,100		},
	{	0,0,0,100		},
	{	128,128,128,100	},
	{	0,0,0,100		},
	{	255,255,255,100	},
	{	51,88,171,100	},
	{	255,0,0,100		},
	&MMI_default_font,
	1,
	0,
	'*',
	1
};

/*	EMS header implementation using multi-line input box	*/

void create_EMS_viewer_header(U8* message)
{	S32 l,ll;
#if(EMS_VIEWER_HEADER_HEIGHT_AUTO)
	S32 fh,n_lines,h;
#endif
	l=coolsand_UI_strlen((UI_string_type)message);
	ll=(l+1)*ENCODING_LENGTH;
	coolsand_UI_create_multi_line_input_box_set_buffer(&EMS_viewer_header_inputbox,0,0,MMI_EMS_inputbox.width-MMI_EMS_inputbox.vbar.width,EMS_VIEWER_HEADER_HEIGHT,(UI_string_type)message,ll,ll,0);
	coolsand_UI_set_multi_line_input_box_theme(&EMS_viewer_header_inputbox,&EMS_viewer_header_theme);
	EMS_viewer_header_inputbox.flags|=(UI_MULTI_LINE_INPUT_BOX_USE_PRESET_CLIPPING|UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR|UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW);
#if(EMS_VIEWER_HEADER_HEIGHT_AUTO)
	coolsand_UI_set_font(EMS_viewer_header_inputbox.text_font);
	fh=coolsand_UI_get_character_height();
	EMS_viewer_header_inputbox.flags|=UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
	coolsand_UI_show_multi_line_input_box(&EMS_viewer_header_inputbox);
	EMS_viewer_header_inputbox.flags&=~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
	n_lines=EMS_viewer_header_inputbox.n_lines;
	if(n_lines>3) n_lines=3;
	h=n_lines*fh+6;
	if(h>EMS_VIEWER_HEADER_HEIGHT) h=EMS_VIEWER_HEADER_HEIGHT;
	coolsand_UI_resize_multi_line_input_box(&EMS_viewer_header_inputbox,EMS_viewer_header_inputbox.width,h);
#endif
}

void EMS_viewer_show_header(S32 x1,S32 y1,S32 x2,S32 y2)
{	S32 ex1,ey1,ex2,ey2;
	ex1=MMI_EMS_inputbox.x+1;
	ex2=ex1+MMI_EMS_inputbox.width-2;
	ey1=MMI_EMS_inputbox.y+1;
	ey2=ey1+MMI_EMS_inputbox.height-2;
	coolsand_UI_set_text_clip(ex1,ey1,ex2,ey2);
	coolsand_UI_set_clip(ex1,ey1,ex2,ey2);
	coolsand_UI_move_multi_line_input_box(&EMS_viewer_header_inputbox,x1,y1);
	coolsand_UI_show_multi_line_input_box(&EMS_viewer_header_inputbox);
}

S32 EMS_viewer_measure_header(void)
{	return(EMS_viewer_header_inputbox.height);
}

#else

/*	EMS header implementation using normal text output				*/
/*	Requires message to contain line breaks where ever necessary	*/

#define EMS_VIEWER_HEADER_MAX_LINES				4
#define EMS_VIEWER_HEADER_MAX_LINE_LENGTH		32*ENCODING_LENGTH

U8	EMS_header_text_strings[EMS_VIEWER_HEADER_MAX_LINES][EMS_VIEWER_HEADER_MAX_LINE_LENGTH];
S32	EMS_header_text_string_heights[EMS_VIEWER_HEADER_MAX_LINES];
S32	EMS_header_height=0;
S32	EMS_header_n_lines=0;

#if(0)

/*	Assumes the following format:
	<line1>\n
	<line2>\n
	....
	Long lines are truncated by ...	*/

void create_EMS_viewer_header(U8* message)
{	byte				done=0;
	byte				inner_done;
	S32					i,j;
	UI_buffer_type		text=message;
	UI_buffer_type		out_text;
	S32					sw,sh;
	UI_character_type	c;

	coolsand_UI_set_font(&MMI_default_font);
	EMS_header_n_lines=0;
	EMS_header_height=0;
	i=0;
	while(!done)
	{	if(EMS_header_n_lines>=EMS_VIEWER_HEADER_MAX_LINES) break;
		inner_done=0;
		j=0;
		out_text=(UI_buffer_type)EMS_header_text_strings[EMS_header_n_lines];
		EMS_header_text_strings[EMS_header_n_lines][0]='\0';
		EMS_header_text_strings[EMS_header_n_lines][1]=0;
		while(!inner_done)
		{	if(j>=(EMS_VIEWER_HEADER_MAX_LINE_LENGTH-ENCODING_LENGTH)) break;
			UI_STRING_GET_NEXT_CHARACTER(text,c);
			if(UI_STRING_LINE_BREAK_CHARACTER(c))
			{	inner_done=1;
				break;
			}
			if(UI_STRING_END_OF_STRING_CHARACTER(c))
			{	inner_done=1;
				done=1;
				break;
			}
			UI_STRING_INSERT_CHARACTER(out_text,c);
			j+=ENCODING_LENGTH;
		}
		UI_STRING_INSERT_CHARACTER(out_text,'\0');
		while(!inner_done)
		{	UI_STRING_GET_NEXT_CHARACTER(text,c);
			if(UI_STRING_LINE_BREAK_CHARACTER(c))
			{	inner_done=1;
				break;
			}
			if(UI_STRING_END_OF_STRING_CHARACTER(c))
			{	inner_done=1;
				done=1;
				break;
			}
		}
		coolsand_UI_measure_string((UI_string_type)EMS_header_text_strings[EMS_header_n_lines],&sw,&sh);
		sh+=1;
		EMS_header_text_string_heights[EMS_header_n_lines]=sh;
		EMS_header_height+=sh;
		EMS_header_n_lines++;
	}
	if(EMS_header_height>0) EMS_header_height+=2;
}

#else

/*	Returns true when end of string is reached	*/
/*	loads \n delimited strings					*/

byte load_EMS_viewer_header_string(U8 **src,U8 **dst,S32 max_dst_length)
{	UI_buffer_type		text=*src;
	UI_buffer_type		out_text=*dst;
	byte				inner_done=0;
	S32					j=0;
	byte				done=0;
	UI_character_type	c;

	while(!inner_done)
	{	if(j>=(max_dst_length-ENCODING_LENGTH)) break;
		UI_STRING_GET_NEXT_CHARACTER(text,c);
		if(UI_STRING_LINE_BREAK_CHARACTER(c))
		{	inner_done=1;
			break;
		}
		if(UI_STRING_END_OF_STRING_CHARACTER(c))
		{	inner_done=1;
			done=1;
			break;
		}
		UI_STRING_INSERT_CHARACTER(out_text,c);
		j+=ENCODING_LENGTH;
	}
	UI_STRING_INSERT_CHARACTER(out_text,'\0');
	while(!inner_done)
	{	UI_STRING_GET_NEXT_CHARACTER(text,c);
		if(UI_STRING_LINE_BREAK_CHARACTER(c))
		{	inner_done=1;
			break;
		}
		if(UI_STRING_END_OF_STRING_CHARACTER(c))
		{	inner_done=1;
			done=1;
			break;
		}
	}
	*src=text;
	*dst=out_text;
	return(done);
}

#define UI_EMS_HEADER_NUMBER_OF_BREAK_CHARACTERS		(1)

UI_character_type EMS_header_break_characters[]=
{	' '
};

#define UI_EMS_HEADER_BREAK_CHARACTER(c,flag)														\
{	S32 _ii_;																						\
	flag=0;																							\
	for(_ii_=0;_ii_<UI_EMS_HEADER_NUMBER_OF_BREAK_CHARACTERS;_ii_++)								\
	{	if((UI_character_type)c==(UI_character_type)EMS_header_break_characters[_ii_])				\
		{	flag=1;																					\
			break;																					\
		}																							\
	}																								\
}

/*	Assumes the following format:
	From:\n
	Name/number\n
	Date\n
	Time\n
	Attempts to combine first two lines	*/

void create_EMS_viewer_header(U8* message)
{	byte				done=0;
	byte				inner_done;
	S32					j;
	UI_buffer_type		text=message;
	UI_buffer_type		out_text;
	S32					sw,sh,break_count;
	UI_character_type	c;
	S32					l1_w,l1_h,l2_w,l2_h;
	U8					temp_string[EMS_VIEWER_HEADER_MAX_LINE_LENGTH];
/*	U8					space_string[]={' ',0,'\0',0};	*/
/*	U8					blank_string[]={'\0',0};		*/
	S32					character_width,character_height;
	S32					box_width=(MMI_EMS_inputbox.width-MMI_EMS_inputbox.vbar.width-2);
	UI_buffer_type		temp_text1,temp_text2;
	byte				break_flag;
	U16				from_len = 0;
	U16				name_number_len =0;

	coolsand_UI_set_font(&MMI_default_font);
	EMS_header_n_lines=0;
	EMS_header_height=0;
	out_text=(UI_buffer_type)EMS_header_text_strings[EMS_header_n_lines];
	/*	"From:"	*/
	done=load_EMS_viewer_header_string(&text,&out_text,EMS_VIEWER_HEADER_MAX_LINE_LENGTH);
	coolsand_UI_measure_string((UI_string_type)EMS_header_text_strings[EMS_header_n_lines],&l1_w,&l1_h);
	l1_h++;
	EMS_header_height+=l1_h;
	EMS_header_text_string_heights[EMS_header_n_lines]=l1_h;
	from_len = pfnUnicodeStrlen((S8*)EMS_header_text_strings[EMS_header_n_lines]);
	/*	"Name/number"	*/
	if(!done)
	{	out_text=(UI_buffer_type)temp_string;
		done=load_EMS_viewer_header_string(&text,&out_text,EMS_VIEWER_HEADER_MAX_LINE_LENGTH);
		coolsand_UI_measure_string((UI_string_type)temp_string,&l2_w,&l2_h);
		name_number_len = pfnUnicodeStrlen((S8*)temp_string);
		if(((l1_w+l2_w)<box_width) && ((from_len +name_number_len)<EMS_VIEWER_HEADER_MAX_LINE_LENGTH /ENCODING_LENGTH  ))
		{	coolsand_UI_strcat((UI_string_type)EMS_header_text_strings[EMS_header_n_lines],(UI_string_type)temp_string);
			coolsand_UI_measure_string((UI_string_type)EMS_header_text_strings[EMS_header_n_lines],&sw,&sh);
			sh++;
			EMS_header_height-=l1_h;
			EMS_header_height+=sh;
			EMS_header_text_string_heights[EMS_header_n_lines]=sh;
			EMS_header_n_lines++;
		}
		else if((l2_w<=box_width) && (name_number_len<EMS_VIEWER_HEADER_MAX_LINE_LENGTH /ENCODING_LENGTH  ))
		{	EMS_header_n_lines++;
			coolsand_UI_strcpy((UI_string_type)EMS_header_text_strings[EMS_header_n_lines],(UI_string_type)temp_string);
			coolsand_UI_measure_string((UI_string_type)EMS_header_text_strings[EMS_header_n_lines],&sw,&sh);
			sh++;
			EMS_header_height+=sh;
			EMS_header_text_string_heights[EMS_header_n_lines]=sh;
			EMS_header_n_lines++;
		}
		else	/*	break "Name/number" into two lines	*/
		{	out_text=(UI_buffer_type)temp_string;
			break_count=0;
			inner_done=0;
			sw=l1_w;
			j=0;
			while(!inner_done)
			{	UI_STRING_GET_NEXT_CHARACTER(out_text,c);
				if(UI_STRING_END_OF_STRING_CHARACTER(c))
				{	break_count=j;
					inner_done=1;
					done=1;
					break;
				}
				UI_EMS_HEADER_BREAK_CHARACTER(c,break_flag);
				if(break_flag) break_count=j+1;
				coolsand_UI_measure_character(c,&character_width,&character_height);
				character_width+=1;
				if((sw+character_width)>box_width)
				{	inner_done=1;
					break;
				}
				sw+=character_width;
				j++;
			}
			if(break_count==0) break_count=j;
			temp_text1=(UI_buffer_type)EMS_header_text_strings[EMS_header_n_lines+1];
			temp_text2=(UI_buffer_type)temp_string;
			for(j=0;j<break_count;j++)
			{	UI_STRING_GET_NEXT_CHARACTER(temp_text2,c);
				UI_STRING_INSERT_CHARACTER(temp_text1,c);
			}
			UI_STRING_INSERT_CHARACTER(temp_text1,'\0');
			coolsand_UI_strcat((UI_string_type)EMS_header_text_strings[EMS_header_n_lines],(UI_string_type)EMS_header_text_strings[EMS_header_n_lines+1]);
			inner_done=0;
			temp_text1=(UI_buffer_type)EMS_header_text_strings[EMS_header_n_lines+1];
			while(!inner_done)
			{	UI_STRING_GET_NEXT_CHARACTER(temp_text2,c);
				if(UI_STRING_END_OF_STRING_CHARACTER(c))
				{	inner_done=1;
					break;
				}
				if(UI_STRING_LINE_BREAK_CHARACTER(c))
				{	inner_done=1;
					break;
				}
				UI_STRING_INSERT_CHARACTER(temp_text1,c);
			}
			UI_STRING_INSERT_CHARACTER(temp_text1,'\0');
			coolsand_UI_measure_string((UI_string_type)EMS_header_text_strings[EMS_header_n_lines],&sw,&sh);
			sh++;
			EMS_header_text_string_heights[EMS_header_n_lines]=sh;
			EMS_header_height-=l1_h;
			EMS_header_height+=sh;
			coolsand_UI_measure_string((UI_string_type)EMS_header_text_strings[EMS_header_n_lines+1],&sw,&sh);
			sh++;
			EMS_header_text_string_heights[EMS_header_n_lines+1]=sh;
			EMS_header_height+=sh;
			EMS_header_n_lines+=2;
		}
	}
	else EMS_header_n_lines++;
	if(!done)
	{	out_text=(UI_buffer_type)EMS_header_text_strings[EMS_header_n_lines];
		done=load_EMS_viewer_header_string(&text,&out_text,EMS_VIEWER_HEADER_MAX_LINE_LENGTH);
		coolsand_UI_measure_string((UI_string_type)EMS_header_text_strings[EMS_header_n_lines],&sw,&sh);
		sh++;
		EMS_header_height+=sh;
		EMS_header_text_string_heights[EMS_header_n_lines]=sh;
		EMS_header_n_lines++;
	}
	if(!done)
	{	out_text=(UI_buffer_type)EMS_header_text_strings[EMS_header_n_lines];
		done=load_EMS_viewer_header_string(&text,&out_text,EMS_VIEWER_HEADER_MAX_LINE_LENGTH);
		coolsand_UI_measure_string((UI_string_type)EMS_header_text_strings[EMS_header_n_lines],&sw,&sh);
		sh++;
		EMS_header_height+=sh;
		EMS_header_text_string_heights[EMS_header_n_lines]=sh;
		EMS_header_n_lines++;
	}
	if(EMS_header_height>0) EMS_header_height+=2;
}

#endif

void EMS_viewer_show_header(S32 x1,S32 y1,S32 x2,S32 y2)
{	S32 ex1,ey1,ex2,ey2,x,y,xwidth,i;
	if(EMS_header_height<=0) return;
	ex1=MMI_EMS_inputbox.x+1;
	ex2=ex1+MMI_EMS_inputbox.width-5;
	ey1=MMI_EMS_inputbox.y+1;
	ey2=ey1+MMI_EMS_inputbox.height-2;
	coolsand_UI_set_text_clip(ex1,ey1,ex2,ey2);
	coolsand_UI_set_clip(ex1,ey1,ex2,ey2);
	coolsand_UI_set_font(&MMI_default_font);
	coolsand_UI_set_text_color(*current_MMI_theme->UI_black_color);
	coolsand_UI_draw_filled_area(ex1,y1,ex2,y2,&EMS_viewer_header_background_filler);
	 
	if(r2lMMIFlag)
		x=UI_device_width-3;
	else
		x=ex1+1;
	//CSD end
	y=y1+2;
	xwidth=(ex2-ex1-3);
	for(i=0;i<EMS_header_n_lines;i++)
	{
		coolsand_UI_print_truncated_text(x,y,xwidth,(UI_string_type)EMS_header_text_strings[i]);
		y+=EMS_header_text_string_heights[i];
	}
}

S32 EMS_viewer_measure_header(void)
{	return(EMS_header_height);
}

#endif

/*----------------------------------------------------------------------------
Function:			RedrawCategory39Screen
Description:		Redraws the MT EMS/SMS screen
Input Parameters:	none
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

void RedrawCategory39Screen(void)
{
	gdi_layer_lock_frame_buffer();
	clear_screen();
	show_title_status_icon();
	draw_title();
	show_softkey_background();
	show_left_softkey();
	show_right_softkey();
#if(UI_DISABLE_EMS_INPUT_BOX)
	show_multiline_inputbox();
#else
	show_EMS_inputbox();
#endif
	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0,0,UI_device_width-1,UI_device_height-1);
}

/*****************************************************************************
* FUNCTION
*  ShowCategory39Screen
*
* DESCRIPTION
*  Displays the MT EMS/SMS screen
*
* PARAMETERS
*   title						IN	Title for the screen
*   title_icon					IN	Icon displayed with the title
*   left_softkey				IN	Left softkey label
*   left_softkey_icon		IN	Left softkey icon
*   right_softkey				IN	Right softkey label
*   right_softkey_icon		IN	Right softkey icon
*   data						IN	EMSData (Pre-allocated and initialized)
*   header					IN	Header to be displayed (Must include '\n' for line seperation)
*   history_buffer			IN	history buffer
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*  ExitCategoryFunction, RedrawCategoryFunction, GetCategoryHistory
*  GetCategoryHistorySize, g_dm_data_struct
* *****************************************************************************/
void ShowCategory39Screen(U16 title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,EMSData* data,U8 *header,U8* history_buffer)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
#ifdef __MMI_DRAW_MANAGER__
	dm_data_struct dm_data;
#endif
	s16		input_type=0;
#if(UI_DISABLE_EMS_INPUT_BOX)
	S32		length;
	S32		i,j,h,fh;

	UI_UNUSED_PARAMETER(history_buffer);
#else
	byte	h;
#endif

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	EMS_inputbox_has_changed=0;
#ifdef __MMI_MESSAGES_EMS__
	initialize_EMS_objects();
#endif
	MMI_menu_shortcut_number=-1;
	change_left_softkey(left_softkey,left_softkey_icon);
	change_right_softkey(right_softkey,right_softkey_icon);
	SetupCategoryKeyHandlers();

	MMI_title_string=(UI_string_type)get_string(title);
	MMI_title_icon=(UI_image_type)get_image(title_icon);

#if(UI_DISABLE_EMS_INPUT_BOX)
	length=(data->textBufferSize>>1);
	if(length==0)
	{	UI_string_type	s=(UI_string_type)"EMS Data not initialized";
		S32				l=coolsand_UI_strlen((UI_string_type)s);
		temp_EMS_buffer=(UI_string_type)coolsand_UI_malloc(l*sizeof(UI_character_type));
		coolsand_UI_strcpy((UI_string_type)temp_EMS_buffer,(UI_string_type)s);
		length=l+1;
		i=l;
	}
	else
	{
		temp_EMS_buffer=(UI_string_type)coolsand_UI_malloc(length*sizeof(UI_character_type));
		i=j=0;
		while(j<data->textLength)
		{
			if((data->textBuffer[j]=='\0') && (data->textBuffer[j+1]=='\0'))
			{	temp_EMS_buffer[i]='\0';
				break;
			}
			else temp_EMS_buffer[i]=data->textBuffer[j+1];
			i++;
			j+=2;
		}
	}
	temp_EMS_data=data;

	create_multiline_inputbox_set_buffer(temp_EMS_buffer,length,i,0);
	register_multiline_inputbox_keys();
	register_hide_multitap(category5_hide_multitap);
	move_multiline_inputbox(0,(MMI_title_y+MMI_title_height));
	h=MMI_content_height-MMI_multitap_height-8;
	coolsand_UI_set_font(MMI_multiline_inputbox.text_font);
	fh=coolsand_UI_get_character_height();
	h=((h/fh)*fh)+8;
	resize_multiline_inputbox(MMI_multiline_inputbox.width,h);
	set_multiline_inputbox_mask(0);
#else
#if((UI_ENABLE_FRAME_SKIPPING)&&(ENABLE_EMS_INPUTBOX_FRAME_SKIPPING))
	wgui_EMS_inputbox_frame_counter=0;
#endif
	coolsand_UI_create_EMS_input_box(&MMI_EMS_inputbox,0,(MMI_title_y+MMI_title_height),MMI_content_width,MMI_content_height);
	coolsand_UI_set_EMS_input_box_data(&MMI_EMS_inputbox,data);
	h=set_EMS_inputbox_category_history(MMI_CATEGORY39_ID,history_buffer,(s16*)&input_type);
	MMI_EMS_inputbox.flags|=UI_EMS_INPUT_BOX_VIEW_MODE;
	MMI_EMS_inputbox.flags|=UI_EMS_INPUT_BOX_DISPLAY_HEADER;
#ifdef __MMI_WALLPAPER_ON_BOTTOM__
	MMI_EMS_inputbox.flags |= UI_EMS_INPUT_BOX_TRANSPARENT_BACKGROUND;
//	dm_get_scr_bg_image(&editor_scr_bg_ID, &editor_scr_bg_filename, &editor_scr_bg_x, &editor_scr_bg_y, &editor_scr_bg_opacity);
 
#endif
	if(!h)
		ResetCurrentPosition(MMI_EMS_inputbox.data);
	MMI_EMS_inputbox.flags|=UI_EMS_INPUT_BOX_DISABLE_CURSOR_DRAW;
	register_EMS_inputbox_viewer_keys();
	create_EMS_viewer_header(header);
	MMI_EMS_inputbox.display_header=EMS_viewer_show_header;
	MMI_EMS_inputbox.measure_header=EMS_viewer_measure_header;
#endif
	ExitCategoryFunction=ExitCategory39Screen;
#ifdef __MMI_DRAW_MANAGER__
	dm_setup_category_functions(dm_redraw_category_screen, GetCategory39History, GetCategory39HistorySize);
	dm_data.S32ScrId = (S32)GetActiveScreenId();
	dm_data.S32CatId = MMI_CATEGORY39_ID;
	dm_data.S32flags=DM_CLEAR_SCREEN_BACKGROUND;
	dm_setup_data(&dm_data);
	dm_redraw_category_screen();
#else
	RedrawCategoryFunction=RedrawCategory39Screen;
	GetCategoryHistory=GetCategory39History;
	GetCategoryHistorySize=GetCategory39HistorySize;
	RedrawCategory39Screen();
#endif
}

/*****************************************************************************
* FUNCTION
*  CloseCategory39Screen
*
* DESCRIPTION
*   Releases data used by the MT EMS/SMS screen
*
* PARAMETERS
*  void
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void CloseCategory39Screen(void)
{
#if(UI_DISABLE_EMS_INPUT_BOX)
	S32				length;
	S32				i,j;
	U8*				buffer;

	length=MMI_multiline_inputbox.text_length;
	buffer=(U8*)coolsand_UI_malloc((length<<1)+2);
	i=j=0;
	ResetCurrentPosition(temp_EMS_data);
	while(i<length)
	{	buffer[j++]=0;
		buffer[j++]=(U8)temp_EMS_buffer[i++];
	}
	buffer[j++]=0;
	buffer[j++]='\0';
	AddString(temp_EMS_data,buffer,(U16)length,NULL);
	coolsand_UI_free(buffer);
	coolsand_UI_free(temp_EMS_buffer);
#endif
}

/*****************************************************************************
* FUNCTION
*  ExitCategory39Screen
*
* DESCRIPTION
*   Exits the MT EMS/SMS screen
*
* PARAMETERS
*  void
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void ExitCategory39Screen(void)
{
#if(!UI_DISABLE_EMS_INPUT_BOX)
	reset_EMS_inputbox();
#endif
#ifdef __MMI_MESSAGES_EMS__
	close_EMS_objects();
#endif
	ExitCategoryFunction=MMI_dummy_function;
	RedrawCategoryFunction=MMI_dummy_function;
	GetCategoryHistory=dummy_get_history;
	GetCategoryHistorySize=dummy_get_history_size;
}

/*****************************************************************************
* FUNCTION
*  GetCategory39HistorySize
*
* DESCRIPTION
*   Gets the history buffer size for the MT EMS/SMS screen
*
* PARAMETERS
*  void
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
S32 GetCategory39HistorySize(void)
{
#if(UI_DISABLE_EMS_INPUT_BOX)
	return(0);
#else
	return(sizeof(EMS_inputbox_category_history));
#endif

}

/*****************************************************************************
* FUNCTION
*  GetCategory39History
*
* DESCRIPTION
*   Gets the history buffer for the MT EMS/SMS screen
*
* PARAMETERS
*  history				IN/OUT	the buffer into which the history data is stored (pre-allocated)
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
U8* GetCategory39History(U8* history_buffer)
{
#if(UI_DISABLE_EMS_INPUT_BOX)
	return(history_buffer);
#else
	get_EMS_inputbox_category_history(MMI_CATEGORY39_ID,history_buffer,MMI_current_input_type);
	return(history_buffer);
#endif
}
#ifdef __MMI_MESSAGES_EMS__
EMSObjData temp_EMS_obj_data;

void CreateCategory143ObjectList(EMSData *data,EMSObject **object_list)
{	EMSObject *p=data->listHead;
	EMSObject *q;
	S32 type;
	*object_list=q=NULL;
#if(ENABLE_EMS_TEST_SAVE_OBJECTS)
	if(data==NULL) return;
	if(data->listHead==NULL) return;
	while(p!=NULL)
	{	type=p->Type;
		/* reused object will not be put in category_143_object_list */
		if(
#ifdef __EMS_REL5__
		(!EMSIsReusedObj(data,p)) &&
#endif
		((type==EMS_TYPE_PIC) || (type==EMS_TYPE_USERDEF_ANM)
#if defined(__MMI_IMELODY_SUPPORT__)
		||(type==EMS_TYPE_USERDEF_SND)
#endif
		))
		{	if(q==NULL)
			{	*object_list=q=(EMSObject*)coolsand_UI_malloc(sizeof(EMSObject));
				q->next=NULL;
				q->prev=NULL;
				q->data=p->data;
				q->OffsetToText=p->OffsetToText;
				q->PredefNo=p->PredefNo;
				q->Type=p->Type;
			}
			else
			{	EMSObject *temp;
				temp=(EMSObject*)coolsand_UI_malloc(sizeof(EMSObject));
				temp->next=NULL;
				temp->data=p->data;
				temp->OffsetToText=p->OffsetToText;
				temp->PredefNo=p->PredefNo;
				temp->Type=p->Type;
				temp->prev=q;
				q->next=temp;
				q=temp;
			}
		}
		p=p->next;
	}
#else
#if(ENABLE_EMS_TEST_SAVE_PIC_OBJECTS)  // picture
	temp_EMS_obj_data.picture.bitsPerPixel=1;
	temp_EMS_obj_data.picture.hDim=16;
	temp_EMS_obj_data.picture.vDim=16;
	temp_EMS_obj_data.picture.pdu_length=32;
	temp_EMS_obj_data.picture.pdu=(U8*)(EMS_sample_picture_object_PDU_11+8);
	*object_list=q=(EMSObject*)coolsand_UI_malloc(sizeof(EMSObject));
	q->next=NULL;
	q->prev=NULL;
	q->data=&temp_EMS_obj_data;
	q->OffsetToText=0;
	q->PredefNo=0;
	q->Type=EMS_TYPE_PIC;
#endif // ENABLE_EMS_TEST_SAVE_PIC_OBJECTS
#if(ENABLE_EMS_TEST_SAVE_ANIA_OBJECTS)  // animation
	temp_EMS_obj_data.animation.bitsPerPixel=1;
	temp_EMS_obj_data.animation.hDim=8;
	temp_EMS_obj_data.animation.vDim=8;
	temp_EMS_obj_data.animation.pdu_length=32;
	temp_EMS_obj_data.animation.pdu=(U8*)(EMS_sample_animation_object_PDU_11+8);
	*object_list=q=(EMSObject*)coolsand_UI_malloc(sizeof(EMSObject));
	q->next=NULL;
	q->prev=NULL;
	q->data=&temp_EMS_obj_data;
	q->OffsetToText=0;
	q->PredefNo=0;
	q->Type=EMS_TYPE_USERDEF_ANM;
#endif	// ENABLE_EMS_TEST_SAVE_ANIA_OBJECTS
#if(ENABLE_EMS_TEST_SAVE_MELO_OBJECTS)  // melodies
//CSD Joy added for customization, 1014
#if defined(__MMI_IMELODY_SUPPORT__)
	temp_EMS_obj_data.sound.pdu_length=(U16)(EMS_sample_melody_object_PDU_1[1]|(EMS_sample_melody_object_PDU_1[2]<<8));
	temp_EMS_obj_data.sound.pdu=(U8*)(EMS_sample_melody_object_PDU_1+3);
	*object_list=q=(EMSObject*)coolsand_UI_malloc(sizeof(EMSObject));
	q->next=NULL;
	q->prev=NULL;
	q->data=&temp_EMS_obj_data;
	q->OffsetToText=0;
	q->PredefNo=0;
	q->Type=EMS_TYPE_USERDEF_SND;
#endif	//  __MMI_IMELODY_SUPPORT__
//CSD end
#endif // ENABLE_EMS_TEST_SAVE_MELO_OBJECTS
#endif // ENABLE_EMS_TEST_SAVE_OBJECTS
}

void FreeCategory143ObjectList(EMSObject **object_list)
{	EMSObject *q,*temp;
	if(object_list==NULL) return;
	q=*object_list;
	while(q!=NULL)
	{	temp=q;
		q=q->next;
		coolsand_UI_free(temp);
	}
	*object_list=NULL;
}

/*----------------------------------------------------------------------------
Function:			RedrawCategory143Screen
Description:		Redraws the object list screen
Input Parameters:	none
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

S32 *category_143_object_list_index=NULL;
S32 category_143_object_list_n_items=0;
vertical_scrollbar category143_vbar;
EMSObject *category_143_object_list;
EMSObject *category_143_current_object;
extern byte	EMS_view_mode;

void category_143_redraw_highlighted_item(void)
{	S32 x,y,w,h;
	color_t c;
	S32 vbar_x=0, vbar_button_x=0;

	EMS_cancel_object_focus();
	coolsand_UI_reset_clip();
	c=coolsand_UI_color(255,255,255);
	coolsand_UI_fill_rectangle(0,(MMI_title_y+MMI_title_height),UI_device_width-1,UI_device_height-MMI_button_bar_height-1,c);
	coolsand_UI_set_clip(0,(MMI_title_y+MMI_title_height),UI_device_width-current_MMI_theme->scrollbar_size,MMI_content_height+2);
	if(category_143_current_object!=NULL)
	{	coolsand_UI_EMS_input_box_measure_object(category_143_current_object,&w,&h);
		x=(UI_device_width>>1)-(w>>1);
		y=(UI_device_height>>1)-(h>>1);
		coolsand_UI_EMS_input_box_display_object(x,y,category_143_current_object,1);
	}
/*	Uncomment below line for auto-disable scrollbar		*/
/*	if(category_143_image_list_n_items>1)				*/
	{	coolsand_UI_set_vertical_scrollbar_range(&category143_vbar,category_143_object_list_n_items);
		coolsand_UI_set_vertical_scrollbar_scale(&category143_vbar,1);
		coolsand_UI_set_vertical_scrollbar_value(&category143_vbar,*category_143_object_list_index);
		 
		if(r2lMMIFlag)
		{
			vbar_x = category143_vbar.x;
			vbar_button_x = category143_vbar.scroll_button.x;
			category143_vbar.x = category143_vbar.x + category143_vbar.width - (UI_device_width-1);
			category143_vbar.scroll_button.x = category143_vbar.scroll_button.x + category143_vbar.scroll_button.width-(UI_device_width-1);
			coolsand_UI_show_vertical_scrollbar( &category143_vbar );
		}
		else
			coolsand_UI_show_vertical_scrollbar( &category143_vbar );
		//CSD end
		if(r2lMMIFlag)
		{
			category143_vbar.x = vbar_x;
			category143_vbar.scroll_button.x = vbar_button_x;
		}
	}
	coolsand_UI_BLT_double_buffer(0,(MMI_title_y+MMI_title_height),UI_device_width-1,UI_device_height-MMI_button_bar_height-1);
}

void category_143_highlight_handler(S32 index)
{	MMI_menu_shortcut_number=index+1;
	if(!menu_shortcut_handler_display) draw_title();
	MMI_list_highlight_handler(index);
}

void category_143_goto_item(S32 index)
{	if(category_143_object_list!=NULL)
	{	if(index<0)	return;	/*	index=0;	*/
		if(index>(category_143_object_list_n_items-1)) return;	/*	index=category_143_image_list_n_items-1;	*/
		*category_143_object_list_index=index;
		category_143_current_object=category_143_object_list;
		if(category_143_current_object!=NULL)
		{	S32 i;
			for(i=0;i<index;i++)
				category_143_current_object=category_143_current_object->next;
		}
	}
	category_143_redraw_highlighted_item();
	category_143_highlight_handler(index);
}

void category_143_goto_previous_item(void)
{	S32 index=*category_143_object_list_index;
	index--;
	if(index<0) index=category_143_object_list_n_items-1;
	*category_143_object_list_index=index;
	if(category_143_current_object!=NULL)
	{	if(category_143_current_object->prev==NULL)
		{	category_143_current_object=category_143_object_list;
			while(category_143_current_object->next!=NULL)
			{	category_143_current_object=category_143_current_object->next;
			}
		}
		else category_143_current_object=category_143_current_object->prev;
	}
	category_143_redraw_highlighted_item();
	category_143_highlight_handler(index);
}

void category_143_goto_next_item(void)
{	S32 index=*category_143_object_list_index;
	index++;
	if(index>(category_143_object_list_n_items-1)) index=0;
	*category_143_object_list_index=index;
	if(category_143_current_object!=NULL)
	{	if(category_143_current_object->next==NULL) category_143_current_object=category_143_object_list;
		else category_143_current_object=category_143_current_object->next;
	}
	category_143_redraw_highlighted_item();
	category_143_highlight_handler(index);
}

void category_143_key_handler(S32 vkey_code,S32 key_state)
{
#if(MMI_BUILD_TYPE==BUILD_TYPE_X86WIN32)
	if(key_state)
	{	switch(vkey_code)
		{	case 37:	// left
						break;
			case 39:	// right
						break;
			case 38:	category_143_goto_previous_item();
						break;
			case 40:	category_143_goto_next_item();
						break;
		}
	}
#else
	UI_UNUSED_PARAMETER(vkey_code);
	UI_UNUSED_PARAMETER(key_state);
#endif
}

#ifdef __MMI_TOUCH_SCREEN__
MMI_BOOL category_143_pen_down_hdlr(mmi_pen_point_struct pos)
{
	mmi_gui_scrollbar_pen_enum scrollbar_event;
	mmi_gui_pen_event_param_struct scrollbar_param;

	if ( PEN_CHECK_BOUND (pos.x, pos.y, category143_vbar.x, category143_vbar.y, category143_vbar.width, category143_vbar.height) )
	{
		coolsand_UI_vertical_scrollbar_translate_pen_event(&category143_vbar, MMI_PEN_EVENT_DOWN, pos.x, pos.y, &scrollbar_event, &scrollbar_param);
		if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
		{
			category_143_goto_item( (S32) scrollbar_param._u.i );
		}
		return MMI_TRUE;
	}
	return MMI_FALSE;
}

/*
MMI_BOOL category_143_pen_up_hdlr(mmi_pen_point_struct pos)
{
	category_143_translate_pen_event(MMI_PEN_EVENT_UP ,pos.x, pos.y);
}
*/

MMI_BOOL category_143_pen_move_hdlr(mmi_pen_point_struct pos)
{
	mmi_gui_scrollbar_pen_enum scrollbar_event;
	mmi_gui_pen_event_param_struct scrollbar_param;

	if ( PEN_CHECK_BOUND (pos.x, pos.y, category143_vbar.x, category143_vbar.y, category143_vbar.width, category143_vbar.height) )
	{
		coolsand_UI_vertical_scrollbar_translate_pen_event(&category143_vbar, MMI_PEN_EVENT_MOVE, pos.x, pos.y, &scrollbar_event, &scrollbar_param);
		if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
		{
			category_143_goto_item( (S32) scrollbar_param._u.i );
		}
		return MMI_TRUE;
	}
	return MMI_FALSE;
}

MMI_BOOL category_143_pen_repeat_hdlr(mmi_pen_point_struct pos)
{
	mmi_gui_scrollbar_pen_enum scrollbar_event;
	mmi_gui_pen_event_param_struct scrollbar_param;

	if ( PEN_CHECK_BOUND (pos.x, pos.y, category143_vbar.x, category143_vbar.y, category143_vbar.width, category143_vbar.height) )
	{
		coolsand_UI_vertical_scrollbar_translate_pen_event(&category143_vbar, MMI_PEN_EVENT_REPEAT, pos.x, pos.y, &scrollbar_event, &scrollbar_param);
		if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
		{
			category_143_goto_item( (S32) scrollbar_param._u.i );
		}
		return MMI_TRUE;
	}
	return MMI_FALSE;
}

#endif
#ifdef __MMI_DRAW_MANAGER__
/*****************************************************************************
* FUNCTION
*  DrawCate143CategoryControlArea
*
* DESCRIPTION
*   Draws the category143 category control area
*
* PARAMETERS
*  coordinate	IN/OUT	coordinates of category controlled area.
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void DrawCate143CategoryControlArea(dm_coordinates * coordinate)
{
	category_143_redraw_highlighted_item();
}
#else
/*****************************************************************************
* FUNCTION
*  RedrawCategory143Screen
*
* DESCRIPTION
*   Redraw the category143 screen
*
* PARAMETERS
*  void
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void RedrawCategory143Screen(void)
{
	gdi_layer_lock_frame_buffer();
	coolsand_UI_hide_animations();
	clear_screen();
	show_title_status_icon();
	draw_title();
	show_softkey_background();
	show_left_softkey();
	show_right_softkey();
	category_143_redraw_highlighted_item();
	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0,0,UI_device_width-1,UI_device_height-1);
}
#endif
/*****************************************************************************
* FUNCTION
*  ShowCategory143Screen
*
* DESCRIPTION
*  Displays the object list screen
*
* PARAMETERS
*   title						IN	Title for the screen
*   title_icon					IN	Icon displayed with the title
*   left_softkey				IN	Left softkey label
*   left_softkey_icon			IN	Left softkey icon
*   right_softkey				IN	Right softkey label
*   right_softkey_icon			IN	Right softkey icon
*   n_items						IN	Number of items in the menu
*   object_list					IN	Array of items
*   index						IN	Default item to be highlighted (if there is no history)
*   history_buffer				IN	history buffer
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*  ExitCategoryFunction, RedrawCategoryFunction, GetCategoryHistory
*  GetCategoryHistorySize, g_dm_data_struct
* *****************************************************************************/
void ShowCategory143Screen(U16 title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,EMSObject *object_list,S32 *n_items,S32 *index,U8* history_buffer)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
#ifdef __MMI_DRAW_MANAGER__
	dm_data_struct	dm_data;
#endif
	S32 shortcut_width,n=0;
	byte h_flag;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	gdi_layer_lock_frame_buffer();

	EMS_view_mode=1;
	initialize_EMS_objects();
	MMI_menu_shortcut_number=-1;
	MMI_disable_title_shortcut_display=0;

	clear_left_softkey();
	clear_right_softkey();
	change_left_softkey(left_softkey,left_softkey_icon);
	change_right_softkey(right_softkey,right_softkey_icon);
	SetupCategoryKeyHandlers();

	MMI_title_icon=get_image(title_icon);
	MMI_title_string=get_string(title);
	category_143_object_list=object_list;
	category_143_object_list_index=index;

	h_flag=set_object_list_category_history(MMI_CATEGORY143_ID,history_buffer,category_143_object_list_index);
	if(h_flag)
	{	*index=*category_143_object_list_index;
	}
	if(category_143_object_list==NULL)
	{	n=0;
		category_143_current_object=NULL;
	}
	else
	{	S32 i;
		EMSObject *p=object_list->next;
		++n;
		while(p!=NULL)
		{	p=p->next;
			++n;
		}
		if(*index<0) *index=0;
		if(*index>=n) *index=n-1;
		category_143_current_object=category_143_object_list;
		for(i=0;i<*index;i++)
			category_143_current_object=category_143_current_object->next;
	}
	*n_items=n;
	MMI_menu_shortcut_number=*index+1;
	category_143_object_list_n_items=n;
	category_143_object_list_index=index;
	if(*category_143_object_list_index<0) *category_143_object_list_index=0;
	if(*category_143_object_list_index>=category_143_object_list_n_items) *category_143_object_list_index=category_143_object_list_n_items-1;
	coolsand_UI_create_vertical_scrollbar(&category143_vbar,UI_device_width-current_MMI_theme->scrollbar_size,(MMI_title_y+MMI_title_height),current_MMI_theme->scrollbar_size,MMI_content_height);
/* ShortCut */
	register_fixed_list_shortcut_handler();
	shortcut_width=set_menu_item_count(*n_items)+7;
	resize_menu_shortcut_handler(shortcut_width,(MMI_title_height));
	move_menu_shortcut_handler(UI_device_width-MMI_menu_shortcut_box.width,MMI_title_y);
	register_hide_menu_shortcut(draw_title);

	register_keyboard_key_handler(category_143_key_handler);
	register_menu_shortcut_keys();
	register_menu_shortcut_handler(category_143_goto_item);
	SetKeyHandler(category_143_goto_previous_item,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(category_143_goto_next_item,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(category_143_goto_previous_item,KEY_VOL_UP,KEY_EVENT_UP);
	SetKeyHandler(category_143_goto_next_item,KEY_VOL_DOWN,KEY_EVENT_DOWN);

	gdi_layer_unlock_frame_buffer();

	ExitCategoryFunction=ExitCategory143Screen;
#ifdef __MMI_DRAW_MANAGER__
	dm_setup_category_functions(dm_redraw_category_screen, GetCategory143History, GetCategory143HistorySize);
	dm_register_category_controlled_callback(DrawCate143CategoryControlArea);
#ifdef __MMI_TOUCH_SCREEN__
	mmi_wgui_register_category_screen_control_area_pen_handlers(category_143_pen_down_hdlr, MMI_PEN_EVENT_DOWN);
	mmi_wgui_register_category_screen_control_area_pen_handlers(category_143_pen_move_hdlr, MMI_PEN_EVENT_MOVE);
	mmi_wgui_register_category_screen_control_area_pen_handlers(category_143_pen_repeat_hdlr, MMI_PEN_EVENT_REPEAT);
#endif
	dm_data.S32ScrId = (S32)GetActiveScreenId();
	dm_data.S32CatId = MMI_CATEGORY143_ID;
	dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
	dm_setup_data(&dm_data);
	dm_redraw_category_screen();
#else
	RedrawCategoryFunction=RedrawCategory143Screen;
	GetCategoryHistory=GetCategory143History;
	GetCategoryHistorySize=GetCategory143HistorySize;
	RedrawCategory143Screen();
#endif
}

/*----------------------------------------------------------------------------
Function:			ExitCategory143Screen
Description:		Exits the object list screen
Input Parameters:	none
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

void ExitCategory143Screen(void)
{	EMS_view_mode=0;
	EMS_cancel_object_focus();
	close_EMS_objects();
	reset_menu_shortcut_handler();
	category_143_object_list=NULL;
	category_143_object_list_index=NULL;
	category_143_object_list_n_items=0;
	ExitCategoryFunction=MMI_dummy_function;
	RedrawCategoryFunction=MMI_dummy_function;
	GetCategoryHistory=dummy_get_history;
	GetCategoryHistorySize=dummy_get_history_size;
}

/*----------------------------------------------------------------------------
Function:			GetCategory143HistorySize
Description:		Gets the history buffer size for the object list screen
Input Parameters:	none
Output Parameters:	none
Returns:			size in U8s of the history buffer
----------------------------------------------------------------------------*/

S32 GetCategory143HistorySize(void)
{	return(sizeof(object_list_category_history));
}

/*----------------------------------------------------------------------------
Function:			GetCategory143History
Description:		Gets the history buffer for the object list screen
Input Parameters:	history_buffer	is the buffer into which the history data is stored
Output Parameters:	none
Returns:			pointer to the history buffer
----------------------------------------------------------------------------*/

U8* GetCategory143History(U8* history_buffer)
{	get_object_list_category_history(MMI_CATEGORY143_ID,history_buffer,category_143_object_list_index);
	return(history_buffer);
}

/*	Supports only user defined objects	*/
byte GetCategory143DataType(void)
{	switch(category_143_current_object->Type)
	{	case	EMS_TYPE_PIC:
				return(EMS_USERDEFINED_PICTURE);
		case	EMS_TYPE_USERDEF_ANM:
				return(EMS_USERDEFINED_ANIMATION);
	//CSD Joy added for customization, 1014
	#if defined(__MMI_IMELODY_SUPPORT__)
		case	EMS_TYPE_USERDEF_SND:
				return(EMS_USERDEFINED_MELODY);
	#endif
	//CSD end
	}
	return(0);
}

/*	Supports only user defined objects	*/
S32 GetCategory143DataSize(void)
{	S32 size=0;
	switch(category_143_current_object->Type)
	{
                case	EMS_TYPE_PIC:
				size=9+category_143_current_object->data->picture.pdu_length;
				break;
		case	EMS_TYPE_USERDEF_ANM:
				size=12+category_143_current_object->data->animation.pdu_length;
				break;
	//CSD Joy added for customization, 1014
	#if defined(__MMI_IMELODY_SUPPORT__)
		case	EMS_TYPE_USERDEF_SND:
				size=4+category_143_current_object->data->sound.pdu_length;
				break;
	#endif
	//CSD end
	}
	return(size);
}

/*	Supports only user defined objects	*/
U8*	GetCategory143Data(U8* data)
{	if(data!=NULL)
	{	switch(category_143_current_object->Type)
		{
                        case	EMS_TYPE_PIC:
					data[0]=(U8)EMS_USERDEFINED_PICTURE;
					data[1]=(U8)category_143_current_object->data->picture.bitsPerPixel;
					data[2]=(U8)category_143_current_object->data->picture.attribute;
					data[3]=(U8)category_143_current_object->data->picture.hDim;
					data[4]=(U8)(category_143_current_object->data->picture.hDim>>8);
					data[5]=(U8)category_143_current_object->data->picture.vDim;
					data[6]=(U8)(category_143_current_object->data->picture.vDim>>8);
					data[7]=(U8)category_143_current_object->data->picture.pdu_length;
					data[8]=(U8)(category_143_current_object->data->picture.pdu_length>>8);
					coolsand_UI_memcpy(data+9,category_143_current_object->data->picture.pdu,category_143_current_object->data->picture.pdu_length);
					break;

			case	EMS_TYPE_USERDEF_ANM:
					data[0]=(U8)EMS_USERDEFINED_ANIMATION;
					data[1]=(U8)category_143_current_object->data->animation.bitsPerPixel;
					data[2]=(U8)category_143_current_object->data->animation.attribute;
					data[3]=(U8)category_143_current_object->data->animation.cFrame;
					data[4]=(U8)category_143_current_object->data->animation.cRepeat;
					data[5]=(U8)category_143_current_object->data->animation.Duration;
					data[6]=(U8)category_143_current_object->data->animation.hDim;
					data[7]=(U8)(category_143_current_object->data->animation.hDim>>8);
					data[8]=(U8)category_143_current_object->data->animation.vDim;
					data[9]=(U8)(category_143_current_object->data->animation.vDim>>8);
					data[10]=(U8)category_143_current_object->data->animation.pdu_length;
					data[11]=(U8)(category_143_current_object->data->animation.pdu_length>>8);
					coolsand_UI_memcpy(data+12,category_143_current_object->data->animation.pdu,category_143_current_object->data->animation.pdu_length);
					break;
		//CSD Joy added for customization, 1014
		#if defined(__MMI_IMELODY_SUPPORT__)
			case	EMS_TYPE_USERDEF_SND:
					data[0]=(U8)EMS_USERDEFINED_MELODY;
					data[1]=(U8)category_143_current_object->data->sound.attribute;
					data[2]=(U8)category_143_current_object->data->sound.pdu_length;
					data[3]=(U8)(category_143_current_object->data->sound.pdu_length>>8);
					coolsand_UI_memcpy(data+4,category_143_current_object->data->sound.pdu,category_143_current_object->data->sound.pdu_length);
					break;
		#endif
		//CSD end
		}
	}
	return(data);
}

#define EMS_PREDEFINED_PICTURE_HEADER_SIZE			3
#define EMS_USERDEFINED_PICTURE_HEADER_SIZE			3
#define EMS_PREDEFINED_ANIMATION_HEADER_SIZE		1
#define EMS_USERDEFINED_ANIMATION_HEADER_SIZE		3
#define EMS_PREDEFINED_SOUND_HEADER_SIZE			1
#define EMS_PREDEFINED_MELODY_HEADER_SIZE			1
#define EMS_USERDEFINED_MELODY_HEADER_SIZE			1

void InsertEMSObjectUIHeader(U32 object_type,S32 object_size,U8 *source,U8 *destination)
{	switch(object_type & 0xff)
	{	case	EMS_USERDEFINED_MELODY:
				coolsand_UI_memcpy((destination+3),source,object_size);
				destination[0]=(U8)EMS_USERDEFINED_MELODY;
				destination[1]=(U8)object_size;
				destination[2]=(U8)object_size>>8;
				break;
	}
}

void RemoveEMSObjectUIHeader(U32 object_type,U8 *source,U8 *destination)
{	S32 object_size;
	switch(object_type & 0xff)
	{	case	EMS_USERDEFINED_MELODY:
				object_size=destination[1];
				object_size|=destination[2]<<8;
				coolsand_UI_memcpy(destination,(source+3),object_size);
				break;
	}
}

void GetEMSObjectPDUSize(U8* data,S32 *size)
{	if(data!=NULL)
	{	byte ID=data[0];
		switch(ID)
		{	case	EMS_PREDEFINED_PICTURE:
					*size=*size-8;
					*size=*size+EMS_PREDEFINED_PICTURE_HEADER_SIZE;
					break;
			case	EMS_USERDEFINED_PICTURE:
					*size=*size-8;
					*size=*size+EMS_USERDEFINED_PICTURE_HEADER_SIZE;
					break;
			case	EMS_PREDEFINED_ANIMATION:
					*size=*size+EMS_PREDEFINED_ANIMATION_HEADER_SIZE;
					break;
			case	EMS_USERDEFINED_ANIMATION:
					*size=*size-8;
					*size=*size+EMS_USERDEFINED_ANIMATION_HEADER_SIZE;
					break;
			case	EMS_PREDEFINED_SOUND:
					*size=*size+EMS_PREDEFINED_SOUND_HEADER_SIZE;
					break;
			case	EMS_USERDEFINED_MELODY:
					*size=*size-3;
					*size=*size+EMS_USERDEFINED_MELODY_HEADER_SIZE;
					break;
			case	EMS_PREDEFINED_MELODY:
					*size=*size-3;
					*size=*size+EMS_PREDEFINED_MELODY_HEADER_SIZE;
					break;
		}
	}
}

S32 GetEMSPredefinedObjectPDUSize(U32 object_type)
{	S32 size=0;
	switch(object_type & 0xff)
	{	case	EMS_PREDEFINED_ANIMATION:
				size=(1+EMS_PREDEFINED_ANIMATION_HEADER_SIZE);
		case	EMS_PREDEFINED_SOUND:
				size=(1+EMS_PREDEFINED_SOUND_HEADER_SIZE);
	}
	return(size);
}

const U8 EMS_invalid_object_symbol[]=
{	EMS_PREDEFINED_PICTURE, 0x01, 0x18, 0x00, 0x18, 0x00, 0x48, 0x00,
	0x00, 0xFF, 0x00, 0x03, 0x00, 0xC0, 0x04, 0x00, 0x20, 0x18, 0x7E, 0x18, 0x11, 0xFF, 0x88, 0x23,
	0xFF, 0xC4, 0x47, 0xC3, 0xE2, 0x4F, 0x81, 0xF2, 0x8F, 0x3C, 0xF1, 0x9F, 0x3C, 0xF9, 0x9F, 0xFC,
	0xF9, 0x9F, 0xF9, 0xF9, 0x9F, 0xF3, 0xF9, 0x9F, 0xE7, 0xF9, 0x9F, 0xE7, 0xF9, 0x8F, 0xFF, 0xF1,
	0x4F, 0xFF, 0xF2, 0x47, 0xE7, 0xE2, 0x23, 0xE7, 0xC4, 0x11, 0xFF, 0x88, 0x18, 0x7E, 0x18, 0x04,
	0x00, 0x20, 0x03, 0x00, 0xC0, 0x00, 0xFF, 0x00,
};

/*----------------------------------------------------------------------------
Function:			RedrawCategory144Screen
Description:		Redraws the object list screen
Input Parameters:	none
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

S32 *category_144_object_list_index=NULL;
S32 category_144_object_list_n_items=0;
S32 category_144_object_list_default_index=0;
vertical_scrollbar category144_vbar;
EMSObject category_144_current_object;
EMSObjData category_144_current_object_data;
U8** category_144_userdefined_objects=NULL;
U16* category_144_predefined_objects=NULL;
U8** category_144_list_of_titles=NULL;
dword category_144_object_list_flags=0;
/*	image pre-loading for objects stored on a file system	*/
byte category_144_object_file_flag=0;
U8*	(*load_EMS_object_file)(S32 index)=NULL;
byte category_144_autodetect_object_type;
U8* category_144_data_ptr[1];
U8 category_144_file_data[1600]={0,0};

void category_144_load_invalid_object_symbol(EMSObject *obj)
{	EMSObjData *object=obj->data;
	U8* data=(U8*)EMS_invalid_object_symbol;
	obj->Type=EMS_TYPE_PIC;
	object->picture.bitsPerPixel=data[1];
	object->picture.hDim=data[2]|(data[3]<<8);
	object->picture.vDim=data[4]|(data[5]<<8);
	object->picture.pdu_length=data[6]|(data[7]<<8);
	data+=8;
	object->picture.pdu=data;
}

void category_144_load_object(S32 index)
{	byte object_type;

	if((index<0)||(index>=category_144_object_list_n_items)) return;
	object_type=(U8)(category_144_object_list_flags & 0xff);
	/*	image pre-loading for objects stored on a file system	*/
	if((category_144_object_file_flag)||(category_144_autodetect_object_type))
	{	byte *object_data;
		if(load_EMS_object_file!=NULL)
		{	object_data=(byte*)load_EMS_object_file(index);
			if(object_data!=NULL)
			{	if(category_144_autodetect_object_type) object_type=(U8)object_data[0];
				switch(object_type)
				{	case	EMS_PREDEFINED_PICTURE:
							category_144_current_object.Type=EMS_TYPE_PIC;
							EMS_predefined_pictures[index]=object_data;
							break;

					case	EMS_USERDEFINED_PICTURE:
							category_144_current_object.Type=EMS_TYPE_PIC;
							category_144_userdefined_objects[index]=object_data;
							break;

					case	EMS_USERDEFINED_ANIMATION:
							category_144_current_object.Type=EMS_TYPE_USERDEF_ANM;
							category_144_userdefined_objects[index]=object_data;
							break;

//CSD Joy added for customization, 1014
#if defined(__MMI_IMELODY_SUPPORT__)

					case	EMS_PREDEFINED_MELODY:
							category_144_current_object.Type=EMS_TYPE_USERDEF_SND;
							EMS_predefined_melodies[index]=object_data;
							break;

					case	EMS_USERDEFINED_MELODY:
							category_144_current_object.Type=EMS_TYPE_USERDEF_SND;
							category_144_userdefined_objects[index]=object_data;
							break;
#endif
//CSD end
				}
			}
		}
	}
	switch(object_type)
	{	case	EMS_PREDEFINED_PICTURE:
			#ifdef __EMS_REL5__
				if(EMS_load_object(object_type,NULL,category_144_predefined_objects[index],category_144_current_object.data)==EMS_INVALID_OBJECT && category_144_current_object.data->picture.attribute!=1)
			#else
				if(EMS_load_object(object_type,NULL,category_144_predefined_objects[index],category_144_current_object.data)==EMS_INVALID_OBJECT)
			#endif
					category_144_load_invalid_object_symbol(&category_144_current_object);
				break;

		case	EMS_USERDEFINED_PICTURE:
			#ifdef __EMS_REL5__
				if(EMS_load_object(object_type,category_144_userdefined_objects[index],0,category_144_current_object.data)==EMS_INVALID_OBJECT && category_144_current_object.data->picture.attribute!=1)
			#else
				if(EMS_load_object(object_type,category_144_userdefined_objects[index],0,category_144_current_object.data)==EMS_INVALID_OBJECT)
			#endif
					category_144_load_invalid_object_symbol(&category_144_current_object);
				break;

		case	EMS_USERDEFINED_ANIMATION:
			#ifdef __EMS_REL5__
				if(EMS_load_object(object_type,category_144_userdefined_objects[index],0,category_144_current_object.data)==EMS_INVALID_OBJECT && category_144_current_object.data->animation.attribute!=1)
			#else
				if(EMS_load_object(object_type,category_144_userdefined_objects[index],0,category_144_current_object.data)==EMS_INVALID_OBJECT)
			#endif
					category_144_load_invalid_object_symbol(&category_144_current_object);
				break;

		case	EMS_PREDEFINED_ANIMATION:
				category_144_current_object.Type=EMS_TYPE_PREDEF_ANM;
				category_144_current_object.PredefNo=(U8)category_144_predefined_objects[index];
				break;

		case	EMS_PREDEFINED_SOUND:
				category_144_current_object.Type=EMS_TYPE_PREDEF_SND;
				category_144_current_object.PredefNo=(U8)category_144_predefined_objects[index];
				break;

//CSD Joy added for customization, 1014
#if defined(__MMI_IMELODY_SUPPORT__)

		case	EMS_PREDEFINED_MELODY:
			#ifdef __EMS_REL5__
				if(EMS_load_object(EMS_PREDEFINED_MELODY,NULL,category_144_predefined_objects[index],category_144_current_object.data)==EMS_INVALID_OBJECT && category_144_current_object.data->sound.attribute!=1)
			#else
				if(EMS_load_object(EMS_PREDEFINED_MELODY,NULL,category_144_predefined_objects[index],category_144_current_object.data)==EMS_INVALID_OBJECT)
			#endif
					category_144_load_invalid_object_symbol(&category_144_current_object);
				break;

		case	EMS_USERDEFINED_MELODY:
			#ifdef __EMS_REL5__
				if(EMS_load_object(EMS_USERDEFINED_MELODY,category_144_userdefined_objects[index],0,category_144_current_object.data)==EMS_INVALID_OBJECT && category_144_current_object.data->sound.attribute!=1)
			#else
				if(EMS_load_object(EMS_USERDEFINED_MELODY,category_144_userdefined_objects[index],0,category_144_current_object.data)==EMS_INVALID_OBJECT)
			#endif
					category_144_load_invalid_object_symbol(&category_144_current_object);
				break;
#endif
//CSD end
	}
}

void category_144_redraw_highlighted_item(void)
{	S32 x,y,w,h;
	color_t c;
	S32 index=0;
	S32 vbar_x=0, vbar_button_x=0;

	if(category_144_object_list_index!=NULL) index=*category_144_object_list_index;
	EMS_cancel_object_focus();
	coolsand_UI_reset_clip();
	c=coolsand_UI_color(255,255,255);
	coolsand_UI_fill_rectangle(0,(MMI_title_y+MMI_title_height),UI_device_width-1,UI_device_height-MMI_button_bar_height-1,c);
	coolsand_UI_set_clip(0,(MMI_title_y+MMI_title_height),UI_device_width-current_MMI_theme->scrollbar_size,MMI_content_height+2);
	coolsand_UI_EMS_input_box_measure_object(&category_144_current_object,&w,&h);
	x=(UI_device_width>>1)-(w>>1);
	y=(UI_device_height>>1)-(h>>1);
	coolsand_UI_EMS_input_box_display_object(x,y,&category_144_current_object,1);
	if(category_144_list_of_titles!=NULL)
	{	MMI_title_string=(UI_string_type)category_144_list_of_titles[index];
		draw_title();
	}
/*	Uncomment below line for auto-disable scrollbar		*/
/*	if(category_144_image_list_n_items>1)				*/
	{	coolsand_UI_set_vertical_scrollbar_range(&category144_vbar,category_144_object_list_n_items);
		coolsand_UI_set_vertical_scrollbar_scale(&category144_vbar,1);
		coolsand_UI_set_vertical_scrollbar_value(&category144_vbar,*category_144_object_list_index);
		 
		if(r2lMMIFlag)
		{
			vbar_x = category144_vbar.x;
			vbar_button_x = category144_vbar.scroll_button.x;
			category144_vbar.x = category144_vbar.x + category144_vbar.width - (UI_device_width-1);
			category144_vbar.scroll_button.x = category144_vbar.scroll_button.x + category144_vbar.scroll_button.width-(UI_device_width-1);
			coolsand_UI_show_vertical_scrollbar( &category144_vbar );
		}
		else
			coolsand_UI_show_vertical_scrollbar( &category144_vbar );
		//CSD end
		if(r2lMMIFlag)
		{
			category144_vbar.x = vbar_x;
			category144_vbar.scroll_button.x = vbar_button_x;
		}
	}
	coolsand_UI_BLT_double_buffer(0,(MMI_title_y+MMI_title_height),UI_device_width-1,UI_device_height-MMI_button_bar_height-1);
}

void category_144_highlight_handler(S32 index)
{	MMI_menu_shortcut_number=index+1;
	if(!menu_shortcut_handler_display) draw_title();
	MMI_list_highlight_handler(index);
}

void category_144_goto_item(S32 index)
{	if(index<0)	return;
	if(index>(category_144_object_list_n_items-1)) return;
	*category_144_object_list_index=index;
	category_144_load_object(index);
	category_144_redraw_highlighted_item();
	category_144_highlight_handler(index);
}

void category_144_goto_previous_item(void)
{	S32 index=*category_144_object_list_index;
	index--;
	if(index<0) index=category_144_object_list_n_items-1;
	*category_144_object_list_index=index;
	category_144_load_object(index);
	category_144_redraw_highlighted_item();
	category_144_highlight_handler(index);
}

void category_144_goto_next_item(void)
{	S32 index=*category_144_object_list_index;
	index++;
	if(index>(category_144_object_list_n_items-1)) index=0;
	*category_144_object_list_index=index;
	category_144_load_object(index);
	category_144_redraw_highlighted_item();
	category_144_highlight_handler(index);
}

void category_144_key_handler(S32 vkey_code,S32 key_state)
{
#if(MMI_BUILD_TYPE==BUILD_TYPE_X86WIN32)
	if(key_state)
	{	switch(vkey_code)
		{	case 37:	// left
						break;
			case 39:	// right
						break;
			case 38:	category_144_goto_previous_item();
						break;
			case 40:	category_144_goto_next_item();
						break;
		}
	}
#else
	UI_UNUSED_PARAMETER(vkey_code);
	UI_UNUSED_PARAMETER(key_state);
#endif
}

void RegisterCategory144ObjectLoader(U8* (*f)(S32 index))
{	load_EMS_object_file=f;
}
#ifdef __MMI_TOUCH_SCREEN__
MMI_BOOL category_144_pen_down_hdlr(mmi_pen_point_struct pos)
{
	mmi_gui_scrollbar_pen_enum scrollbar_event;
	mmi_gui_pen_event_param_struct scrollbar_param;

	if ( PEN_CHECK_BOUND (pos.x, pos.y, category144_vbar.x, category144_vbar.y, category144_vbar.width, category144_vbar.height) )
	{
		coolsand_UI_vertical_scrollbar_translate_pen_event(&category144_vbar, MMI_PEN_EVENT_DOWN, pos.x, pos.y, &scrollbar_event, &scrollbar_param);
		if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
		{
			category_144_goto_item( (S32) scrollbar_param._u.i );
		}
		return MMI_TRUE;
	}
	return MMI_FALSE;
}


MMI_BOOL category_144_pen_up_hdlr(mmi_pen_point_struct pos)
{
	mmi_gui_scrollbar_pen_enum scrollbar_event;
	mmi_gui_pen_event_param_struct scrollbar_param;

	if ( PEN_CHECK_BOUND (pos.x, pos.y, category144_vbar.x, category144_vbar.y, category144_vbar.width, category144_vbar.height) )
	{
		coolsand_UI_vertical_scrollbar_translate_pen_event(&category144_vbar, MMI_PEN_EVENT_UP, pos.x, pos.y, &scrollbar_event, &scrollbar_param);
		if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
		{
			category_144_goto_item( (S32) scrollbar_param._u.i );
		}
		return MMI_TRUE;
	}
	return MMI_FALSE;
}


MMI_BOOL category_144_pen_move_hdlr(mmi_pen_point_struct pos)
{
	mmi_gui_scrollbar_pen_enum scrollbar_event;
	mmi_gui_pen_event_param_struct scrollbar_param;

	if ( PEN_CHECK_BOUND (pos.x, pos.y, category144_vbar.x, category144_vbar.y, category144_vbar.width, category144_vbar.height) )
	{
		coolsand_UI_vertical_scrollbar_translate_pen_event(&category144_vbar, MMI_PEN_EVENT_MOVE, pos.x, pos.y, &scrollbar_event, &scrollbar_param);
		if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
		{
			category_144_goto_item( (S32) scrollbar_param._u.i );
		}
		return MMI_TRUE;
	}
	return MMI_FALSE;
}

MMI_BOOL category_144_pen_repeat_hdlr(mmi_pen_point_struct pos)
{
	mmi_gui_scrollbar_pen_enum scrollbar_event;
	mmi_gui_pen_event_param_struct scrollbar_param;

	if ( PEN_CHECK_BOUND (pos.x, pos.y, category144_vbar.x, category144_vbar.y, category144_vbar.width, category144_vbar.height) )
	{
		coolsand_UI_vertical_scrollbar_translate_pen_event(&category144_vbar, MMI_PEN_EVENT_REPEAT, pos.x, pos.y, &scrollbar_event, &scrollbar_param);
		if (scrollbar_event == GUI_SCROLLBAR_PEN_JUMP_TO_I)
		{
			category_144_goto_item( (S32) scrollbar_param._u.i );
		}
		return MMI_TRUE;
	}
	return MMI_FALSE;
}

#endif

#ifdef __MMI_DRAW_MANAGER__
/*****************************************************************************
* FUNCTION
*  DrawCate144CategoryControlArea
*
* DESCRIPTION
*   Draws the category144 category control area
*
* PARAMETERS
*  coordinate	IN/OUT	coordinates of category controlled area.
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void DrawCate144CategoryControlArea(dm_coordinates * coordinate)
{
	category_144_redraw_highlighted_item();
}
#else
/*****************************************************************************
* FUNCTION
*  RedrawCategory144Screen
*
* DESCRIPTION
*   Redraw the category144 screen
*
* PARAMETERS
*  void
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void RedrawCategory144Screen(void)
{
	gdi_layer_lock_frame_buffer();
	coolsand_UI_hide_animations();
	clear_screen();
	show_title_status_icon();
	draw_title();
	show_softkey_background();
	show_left_softkey();
	show_right_softkey();
	category_144_redraw_highlighted_item();
	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0,0,UI_device_width-1,UI_device_height-1);
}
#endif


/*----------------------------------------------------------------------------
Function:			ShowCategory144Screen
Description:		Displays the object list screen
Input Parameters:	[1]	STRING		title,				Title for the screen
					[2]	IMAGE_ID	title_icon			Icon shown with the title
					[3]	STRING_ID	left_softkey,		Left softkey label
					[4]	IMAGE_ID	left_softkey_icon,	Icon for the left softkey
					[5]	STRING_ID	right_softkey,		Right softkey label
					[6]	IMAGE_ID	right_softkey_icon,	Icon for the right softkey
					[7]	INT			object type flags
					[8]	INT			number of objects in the list
					[9] STRING*		list of object titles
					[10] U16*		list of predefined objects (EMS specific IDs)
					[11] U8**		list of user defined objects (list of EMS data)
					[12] INT*		highlighted item index
					[13] BYTE*		history_buffer		history buffer
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

void ShowCategory144Screen(U8* title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,U32 object_type,S32 n_items,U8** object_titles,U16 *predefined_objects,MMI_BOOL is_short_name,U8** userdefined_objects,S32 *index,U8* history_buffer)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
#ifdef __MMI_DRAW_MANAGER__
	dm_data_struct	dm_data;
#endif
	S32 shortcut_width;
	byte h_flag;
	MMI_menu_shortcut_number=-1;
	MMI_disable_title_shortcut_display=0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
	if(object_type & CATEGORY144_FMGR_PREVIEW)
	{
		S32				str_len;
		S32				i;
		S8				buf_filename_no_ext[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];

		MMI_disable_title_shortcut_display=1;
		index=&category_144_object_list_default_index;

		if(predefined_objects==NULL)
		{
			GoBackToHistory(SCR_ID_MSG_EMS_INSERT_OBJECT);
			return;
		}
		else
		{
			INT	errorCode=0;
			FILE_HANDLE	emsObjectFilePtr=NULL;
			U32 filesize=0;
			S8* filename=(S8*)predefined_objects;

			str_len = pfnUnicodeStrlen((S8*)filename);

			/* find file name - by finding "\\"*/
			for(i = str_len*2-2; i >= 2; i -= 2)
			{
				if( pfnUnicodeStrncmp((PS8)&filename[i], (PS8)L"\\", 1) == 0)
				{
					i+=2;
					break;
				}
			}

		#ifdef MMI_ON_HARDWARE_P
			if (is_short_name)
			   mmi_chset_mixed_text_to_ucs2_str( (U8*)buf_filename_no_ext, (MAX_SUBMENU_CHARACTERS-1) * ENCODING_LENGTH, (U8*)&filename[i], PhnsetGetDefEncodingType() );
			else
		#endif
				pfnUnicodeStrcpy((PS8)buf_filename_no_ext, (PS8)(&(filename[i])));

			ChangeTitle(title_icon, (PU8)(buf_filename_no_ext));

			userdefined_objects=category_144_data_ptr;
			memset (&category_144_file_data, 0, 1600);
			emsObjectFilePtr = MMI_FS_Open ((S8*)predefined_objects, PFS_READ);
			if(!pfOpenError(emsObjectFilePtr))
			{
				filesize = GetFSFileSize (emsObjectFilePtr);
				if(filesize>1600/* bytes*/)
				{
					pfclose (emsObjectFilePtr);
					userdefined_objects[0]=NULL;
				}
				//pfread (category_144_file_data, 1, filesize, emsObjectFilePtr, &errorCode);
				pfread ( emsObjectFilePtr, category_144_file_data, filesize);
				pfclose (emsObjectFilePtr);
				if(pfError (errorCode))
					userdefined_objects[0]=NULL;
				else
					userdefined_objects[0]=category_144_file_data;
			}
			else
				userdefined_objects[0]=NULL;
		}
	}
#endif

	gdi_layer_lock_frame_buffer();
	initialize_EMS_objects();
	EMS_view_mode=1;
	category_144_object_file_flag=0;
	category_144_object_list_flags=object_type;
	category_144_current_object.data=&category_144_current_object_data;
	category_144_autodetect_object_type=0;

	switch(object_type & 0xff)
	{	case	EMS_PREDEFINED_PICTURE:
				category_144_current_object.Type=EMS_TYPE_PIC;
				category_144_object_file_flag=1;
				break;
		case	EMS_USERDEFINED_PICTURE:
				category_144_current_object.Type=EMS_TYPE_PIC;
				category_144_object_file_flag=1;
				break;
		case	EMS_PREDEFINED_ANIMATION:
				category_144_current_object.Type=EMS_TYPE_PREDEF_ANM;
				break;
		case	EMS_USERDEFINED_ANIMATION:
				category_144_current_object.Type=EMS_TYPE_USERDEF_ANM;
				category_144_object_file_flag=1;
				break;
		case	EMS_PREDEFINED_SOUND:
				category_144_current_object.Type=EMS_TYPE_PREDEF_SND;
				break;

//CSD Joy added for customization, 1014
#if defined(__MMI_IMELODY_SUPPORT__)

		case	EMS_PREDEFINED_MELODY:
				category_144_current_object.Type=EMS_TYPE_USERDEF_SND;
				category_144_object_file_flag=1;
				break;
		case	EMS_USERDEFINED_MELODY:
				category_144_current_object.Type=EMS_TYPE_USERDEF_SND;
				category_144_object_file_flag=1;
				break;
#endif
//CSD end

		default:	category_144_autodetect_object_type=1;
					break;
	}

	clear_left_softkey();
	clear_right_softkey();
	change_left_softkey(left_softkey,left_softkey_icon);
	change_right_softkey(right_softkey,right_softkey_icon);
	SetupCategoryKeyHandlers();

	MMI_title_icon=get_image(title_icon);
	if(!(object_type & CATEGORY144_FMGR_PREVIEW))
	MMI_title_string=(UI_string_type)title;
	category_144_object_list_index=index;
	category_144_userdefined_objects=userdefined_objects;
	category_144_predefined_objects=predefined_objects;

	h_flag=set_object_list_category_history(MMI_CATEGORY144_ID,history_buffer,category_144_object_list_index);
	if(h_flag)
		*index=*category_144_object_list_index;

	category_144_object_list_n_items=n_items;
	if(*index<0) *index=0;
	if(*index>=n_items) *index=n_items-1;
	category_144_load_object(*index);
	MMI_menu_shortcut_number=*index+1;
	category_144_object_list_index=index;
	if(!(object_type & CATEGORY144_DISABLE_PREVIEW))
	{	coolsand_UI_create_vertical_scrollbar(&category144_vbar,UI_device_width-current_MMI_theme->scrollbar_size,(MMI_title_y+MMI_title_height),current_MMI_theme->scrollbar_size,MMI_content_height);
		register_fixed_list_shortcut_handler();
		shortcut_width=set_menu_item_count(n_items)+7;
		resize_menu_shortcut_handler(shortcut_width,(MMI_title_height));
		move_menu_shortcut_handler(UI_device_width-MMI_menu_shortcut_box.width,MMI_title_y);
		register_hide_menu_shortcut(draw_title);
		register_keyboard_key_handler(category_144_key_handler);
		register_menu_shortcut_keys();
		register_menu_shortcut_handler(category_144_goto_item);
		SetKeyHandler(category_144_goto_previous_item,KEY_UP_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(category_144_goto_next_item,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(category_144_goto_previous_item,KEY_VOL_UP,KEY_EVENT_DOWN);
		SetKeyHandler(category_144_goto_next_item,KEY_VOL_DOWN,KEY_EVENT_DOWN);
	}
	gdi_layer_unlock_frame_buffer();

	if(!(object_type & CATEGORY144_DISABLE_PREVIEW))
	{
		ExitCategoryFunction=ExitCategory144Screen;
#ifdef __MMI_DRAW_MANAGER__
		dm_setup_category_functions(dm_redraw_category_screen, GetCategory144History, GetCategory144HistorySize);
		dm_register_category_controlled_callback(DrawCate144CategoryControlArea);
#ifdef __MMI_TOUCH_SCREEN__
	mmi_wgui_register_category_screen_control_area_pen_handlers(category_144_pen_down_hdlr, MMI_PEN_EVENT_DOWN);
	mmi_wgui_register_category_screen_control_area_pen_handlers(category_144_pen_up_hdlr, MMI_PEN_EVENT_UP);
	mmi_wgui_register_category_screen_control_area_pen_handlers(category_144_pen_move_hdlr, MMI_PEN_EVENT_MOVE);
	mmi_wgui_register_category_screen_control_area_pen_handlers(category_144_pen_repeat_hdlr, MMI_PEN_EVENT_REPEAT);
#endif
		dm_data.S32ScrId = (S32)GetActiveScreenId();
		dm_data.S32CatId = MMI_CATEGORY144_ID;
		dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
		dm_setup_data(&dm_data);
		dm_redraw_category_screen();
#else
		RedrawCategoryFunction=RedrawCategory144Screen;
		GetCategoryHistory=GetCategory144History;
		GetCategoryHistorySize=GetCategory144HistorySize;
		RedrawCategory144Screen();
#endif
	}
}

/*----------------------------------------------------------------------------
Function:			ExitCategory144Screen
Description:		Exits the object list screen
Input Parameters:	none
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

void ExitCategory144Screen(void)
{	EMS_cancel_object_focus();
	close_EMS_objects();
	reset_menu_shortcut_handler();
	EMS_view_mode=0;
	category_144_object_list_index=NULL;
	category_144_object_list_n_items=0;
	category_144_list_of_titles=NULL;
	category_144_object_list_flags=0;
	ExitCategoryFunction=MMI_dummy_function;
	RedrawCategoryFunction=MMI_dummy_function;
	GetCategoryHistory=dummy_get_history;
	GetCategoryHistorySize=dummy_get_history_size;
	load_EMS_object_file=NULL;
}

/*----------------------------------------------------------------------------
Function:			GetCategory144HistorySize
Description:		Gets the history buffer size for the object list screen
Input Parameters:	none
Output Parameters:	none
Returns:			size in U8s of the history buffer
----------------------------------------------------------------------------*/

S32 GetCategory144HistorySize(void)
{	return(sizeof(object_list_category_history));
}

/*----------------------------------------------------------------------------
Function:			GetCategory144History
Description:		Gets the history buffer for the object list screen
Input Parameters:	history_buffer	is the buffer into which the history data is stored
Output Parameters:	none
Returns:			pointer to the history buffer
----------------------------------------------------------------------------*/

U8* GetCategory144History(U8* history_buffer)
{	get_object_list_category_history(MMI_CATEGORY144_ID,history_buffer,category_144_object_list_index);
	return(history_buffer);
}

EMSObjData EMS_playback_object_data;
EMSObject EMS_playback_object;

EMSTATUS EMSObjectPlayback(U32 object_type,U8* data,S32 index)
{	EMSTATUS r=EMS_load_object(object_type,data,index,&EMS_playback_object_data);
	EMS_cancel_object_focus();
	if(r!=EMS_OK) return(r);
	EMS_playback_object.data=&EMS_playback_object_data;
	EMS_playback_object.prev=NULL;
	EMS_playback_object.next=NULL;
	EMS_playback_object.OffsetToText=0;
	EMS_playback_object.PredefNo=(U8)index;
	switch(object_type & 0xff)
	{	case	EMS_PREDEFINED_SOUND:
				EMS_playback_object.Type=EMS_TYPE_PREDEF_SND;
				coolsand_UI_EMS_playback_object(&EMS_playback_object);
				break;
		//CSD Joy added for customization, 1014
		#if defined(__MMI_IMELODY_SUPPORT__)
		case	EMS_PREDEFINED_MELODY:
				EMS_playback_object.Type=EMS_TYPE_USERDEF_SND;
				coolsand_UI_EMS_playback_object(&EMS_playback_object);
				break;
		case	EMS_USERDEFINED_MELODY:
				EMS_playback_object.Type=EMS_TYPE_USERDEF_SND;
				coolsand_UI_EMS_playback_object(&EMS_playback_object);
				break;
		#endif
		//CSD end
	}
	return(EMS_OK);
}

void EMSObjectStopPlayBack(void)
{
	EMS_cancel_object_focus();
}
#endif //__MMI_MESSAGES_EMS__

#define CAT145_SCROLL_DY	20

color_t cat145_message_title_color;
vertical_scrollbar cat145_vbar;
S32 cat145_h1,cat145_h2,cat145_h3,cat145_total_height,cat145_available_height;
S32 cat145_y=0,cat145_image_x,cat145_message_title_x;
S32 cat145_clip_x1,cat145_clip_y1,cat145_clip_x2,cat145_clip_y2;
dword cat145_flags;
byte cat145_scrollbar_flag=0;

void cat145_display(void)
{
	S32 y=cat145_y+(MMI_title_y+MMI_title_height)+2;
	color_t text_color;

	gdi_layer_push_clip();
	gdi_layer_set_clip(cat145_clip_x1,cat145_clip_y1,cat145_clip_x2,cat145_clip_y2);
	gdi_image_draw(cat145_image_x,y,(U8*)MMI_message_icon);
	y+=cat145_h1+2;
	switch(cat145_flags & 0xff)
	{	case	CAT145_BLACK_TITLE:
				text_color=coolsand_UI_color(0,0,0);
				break;

		case	CAT145_BLUE_TITLE:
				text_color=coolsand_UI_color(0,0,196);
				break;

		case	CAT145_RED_TITLE:
				text_color=coolsand_UI_color(196,0,0);
				break;

		case	CAT145_GREEN_TITLE:
				text_color=coolsand_UI_color(0,196,0);
				break;
	}
	coolsand_UI_set_font(&MMI_default_font);
	coolsand_UI_set_text_color(text_color);
	 
	if(r2lMMIFlag)
		coolsand_UI_move_text_cursor(cat145_message_title_x + coolsand_UI_get_string_width((UI_string_type)MMI_message_string),y);
	else
		coolsand_UI_move_text_cursor(cat145_message_title_x,y);
	//CSD end
	coolsand_UI_print_text(MMI_message_string);
	y+=cat145_h2;
	coolsand_UI_move_multi_line_input_box(&MMI_multiline_inputbox,MMI_multiline_inputbox.x,y);
	coolsand_UI_show_multi_line_input_box(&MMI_multiline_inputbox);
	gdi_layer_pop_clip();
}

void cat145_previous_line(void)
{
	S32 vbar_x=0, vbar_button_x=0;

	coolsand_UI_set_clip(0,cat145_clip_y1,UI_device_width-1,cat145_clip_y2);
	coolsand_UI_draw_filled_area(0,0,UI_device_width-1,UI_device_height-1,current_MMI_theme->general_background_filler);
	cat145_y+=CAT145_SCROLL_DY;
	if(cat145_y>0) cat145_y=0;
	coolsand_UI_set_vertical_scrollbar_value(&cat145_vbar,-cat145_y);
	 
	if(r2lMMIFlag)
	{
		vbar_x = cat145_vbar.x;
		vbar_button_x = cat145_vbar.scroll_button.x;
		cat145_vbar.x = cat145_vbar.x + cat145_vbar.width - (UI_device_width-1);
		cat145_vbar.scroll_button.x = cat145_vbar.scroll_button.x + cat145_vbar.scroll_button.width-(UI_device_width-1);
		coolsand_UI_show_vertical_scrollbar( &cat145_vbar );
	}
	else
		coolsand_UI_show_vertical_scrollbar( &cat145_vbar );
	//CSD end
	if(r2lMMIFlag)
	{
		cat145_vbar.x = vbar_x;
		cat145_vbar.scroll_button.x = vbar_button_x;
	}
	cat145_display();

	gdi_layer_blt_previous(0,cat145_clip_y1,UI_device_width-1,cat145_clip_y2);

}

void cat145_next_line(void)
{
	S32 vbar_x=0, vbar_button_x=0;

	coolsand_UI_set_clip(0,cat145_clip_y1,UI_device_width-1,cat145_clip_y2);
	coolsand_UI_draw_filled_area(0,0,UI_device_width-1,UI_device_height-1,current_MMI_theme->general_background_filler);
	cat145_y-=CAT145_SCROLL_DY;
	if(cat145_y<-(cat145_total_height-cat145_available_height)) cat145_y=-(cat145_total_height-cat145_available_height);
	coolsand_UI_set_vertical_scrollbar_value(&cat145_vbar,-cat145_y);
	 
	if(r2lMMIFlag)
	{
		vbar_x = cat145_vbar.x;
		vbar_button_x = cat145_vbar.scroll_button.x;
		cat145_vbar.x = cat145_vbar.x + cat145_vbar.width - (UI_device_width-1);
		cat145_vbar.scroll_button.x = cat145_vbar.scroll_button.x + cat145_vbar.scroll_button.width-(UI_device_width-1);
		coolsand_UI_show_vertical_scrollbar( &cat145_vbar );
	}
	else
		coolsand_UI_show_vertical_scrollbar( &cat145_vbar );
	//CSD end
	if(r2lMMIFlag)
	{
		cat145_vbar.x = vbar_x;
		cat145_vbar.scroll_button.x = vbar_button_x;
	}
	cat145_display();

	gdi_layer_blt_previous(0,cat145_clip_y1,UI_device_width-1,cat145_clip_y2);

}

void cat145_keyboard_key_handler(S32 vkey_code,S32 key_state)
{
#if(MMI_BUILD_TYPE==BUILD_TYPE_X86WIN32)
	if(key_state)
	{	switch(vkey_code)
		{	case 38:	// up
						cat145_previous_line();
						break;
			case 40:	// down
						cat145_next_line();
						break;
		}
	}
#else
	UI_UNUSED_PARAMETER(vkey_code);
	UI_UNUSED_PARAMETER(key_state);
#endif
}

#ifdef __MMI_DRAW_MANAGER__
/*****************************************************************************
* FUNCTION
*  DrawCate145CategoryControlArea
*
* DESCRIPTION
*   Draws the category145 category control area
*
* PARAMETERS
*  coordinate	IN/OUT	coordinates of category controlled area.
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void DrawCate145CategoryControlArea(dm_coordinates * coordinate)
{
	RedrawCategory145Screen();
}
#endif
/*****************************************************************************
* FUNCTION
*  RedrawCategory145Screen
*
* DESCRIPTION
*   Redraws the Delivery report screen
*
* PARAMETERS
*  void
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*
*****************************************************************************/
void RedrawCategory145Screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 vbar_x=0, vbar_button_x=0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifndef __MMI_DRAW_MANAGER__
	gdi_layer_lock_frame_buffer();
	clear_screen();
	show_title_status_icon();
	draw_title();
#endif
	if(cat145_scrollbar_flag)
	{
		 
		if(r2lMMIFlag)
		{
			vbar_x = cat145_vbar.x;
			vbar_button_x = cat145_vbar.scroll_button.x;
			cat145_vbar.x = cat145_vbar.x + cat145_vbar.width - (UI_device_width-1);
			cat145_vbar.scroll_button.x = cat145_vbar.scroll_button.x + cat145_vbar.scroll_button.width-(UI_device_width-1);
			coolsand_UI_show_vertical_scrollbar( &cat145_vbar );
		}
		else
			coolsand_UI_show_vertical_scrollbar( &cat145_vbar );
		//CSD end
		if(r2lMMIFlag)
		{
			cat145_vbar.x = vbar_x;
			cat145_vbar.scroll_button.x = vbar_button_x;
		}
	}
	cat145_display();
#ifndef __MMI_DRAW_MANAGER__
	show_softkey_background();
	show_left_softkey();
	show_right_softkey();

	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0,0,UI_device_width-1,UI_device_height-1);
#endif
}

void ChangeCategory145MessageString(U8 *new_string)
{
	MMI_multiline_inputbox.text=(UI_buffer_type)new_string;
	MMI_multiline_inputbox.cursor_p=new_string;
	MMI_multiline_inputbox.flags|=UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
	coolsand_UI_show_multi_line_input_box(&MMI_multiline_inputbox);
	MMI_multiline_inputbox.flags&=~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
}

/*****************************************************************************
* FUNCTION
*  ShowCategory145Screen
*
* DESCRIPTION
*  Displays the Delivery report screen
*
* PARAMETERS
*   title						IN	Title for the screen
*   title_icon					IN	Icon displayed with the title
*   left_softkey				IN	Left softkey label
*   left_softkey_icon		IN	Left softkey icon
*   right_softkey				IN	Right softkey label
*   right_softkey_icon		IN	Right softkey icon
*   message_icon				IN	icon displayed with the message
*   message_title				IN	colored text that appears above the message
*   message					IN	message
*   flags						IN	flags
*   history_buffer			IN	history buffer
*
* RETURNS
*  void
*
* GLOBALS AFFECTED
*  ExitCategoryFunction, RedrawCategoryFunction, GetCategoryHistory
*  GetCategoryHistorySize, g_dm_data_struct
* *****************************************************************************/
void ShowCategory145Screen(U8* title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,U16 message_icon,U8 *message_title,U8 *message,U32 flags,U8 *history_buffer)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
#ifdef __MMI_DRAW_MANAGER__
	dm_data_struct	dm_data;
#endif
	S32 l,width,height,content_width;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	gdi_layer_lock_frame_buffer();

	MMI_menu_shortcut_number=-1;
	MMI_disable_title_shortcut_display=1;
	change_left_softkey(left_softkey,left_softkey_icon);
	change_right_softkey(right_softkey,right_softkey_icon);
	change_left_softkey(left_softkey,left_softkey_icon);
	change_right_softkey(right_softkey,right_softkey_icon);
	SetupCategoryKeyHandlers();

	cat145_scrollbar_flag=0;
	coolsand_UI_create_vertical_scrollbar(&cat145_vbar,UI_device_width-current_MMI_theme->scrollbar_size,(MMI_title_y+MMI_title_height),current_MMI_theme->scrollbar_size,(UI_device_height-(MMI_title_y+MMI_title_height)-MMI_button_bar_height+1));
	content_width=UI_device_width;
	l=coolsand_UI_strlen((UI_string_type)message);
	create_multiline_inputbox_set_buffer((UI_string_type)message,l,l,0);
	resize_multiline_inputbox(content_width,MMI_content_height);
	show_multiline_inputbox_no_draw();
	resize_multiline_inputbox(MMI_multiline_inputbox.width,MMI_multiline_inputbox.text_height+6);
	cat145_h3=MMI_multiline_inputbox.height;
	MMI_multiline_inputbox.flags|=UI_MULTI_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW
								|UI_MULTI_LINE_INPUT_BOX_VIEW_MODE
								|UI_MULTI_LINE_INPUT_BOX_DISABLE_BACKGROUND
								|UI_MULTI_LINE_INPUT_BOX_DISABLE_SCROLLBAR
								|UI_MULTI_LINE_INPUT_BOX_USE_PRESET_CLIPPING
								|UI_MULTI_LINE_INPUT_BOX_CENTER_JUSTIFY;

	MMI_message_string=(UI_string_type)message_title;
	coolsand_UI_set_font(&MMI_default_font);
	coolsand_UI_measure_string(MMI_message_string,&width,&height);
	cat145_h2=height;
	cat145_message_title_x=(content_width>>1)-(width>>1);
	MMI_title_string=(UI_string_type)title;
	MMI_title_icon=(UI_image_type)get_image(title_icon);

	MMI_message_icon=(UI_image_type)get_image(message_icon);
	coolsand_UI_measure_image(MMI_message_icon,&width,&height);
	cat145_h1=height+4;
	cat145_image_x=(content_width>>1)-(width>>1);

	cat145_flags=flags;
	cat145_y=0;
	cat145_clip_x1=0;
	cat145_clip_x2=content_width;
	cat145_clip_y1=(MMI_title_y+MMI_title_height);
	cat145_clip_y2=UI_device_height-MMI_button_bar_height-1;
	cat145_total_height=(cat145_h1+cat145_h2+cat145_h3);
	cat145_available_height=cat145_clip_y2-cat145_clip_y1+1;
	if(cat145_total_height>cat145_available_height)
	{	cat145_scrollbar_flag=1;
		register_keyboard_key_handler(cat145_keyboard_key_handler);
		SetKeyHandler(cat145_previous_line,KEY_UP_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(cat145_next_line,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
		SetKeyHandler(cat145_previous_line,KEY_VOL_UP,KEY_EVENT_DOWN);
		SetKeyHandler(cat145_next_line,KEY_VOL_DOWN,KEY_EVENT_DOWN);
		coolsand_UI_set_vertical_scrollbar_range(&cat145_vbar,cat145_total_height);
		coolsand_UI_set_vertical_scrollbar_scale(&cat145_vbar,cat145_available_height);
		coolsand_UI_set_vertical_scrollbar_value(&cat145_vbar,-cat145_y);
	}
	else
	{	cat145_y=(cat145_available_height>>1)-(cat145_total_height>>1);
	}


	gdi_layer_unlock_frame_buffer();

	ExitCategoryFunction=ExitCategory145Screen;
#ifdef __MMI_DRAW_MANAGER__
	dm_setup_category_functions(dm_redraw_category_screen, dummy_get_history,dummy_get_history_size);
	dm_register_category_controlled_callback(DrawCate145CategoryControlArea);
	dm_data.S32ScrId = (S32)GetActiveScreenId();
	dm_data.S32CatId = MMI_CATEGORY145_ID;
	dm_data.S32flags = DM_CLEAR_SCREEN_BACKGROUND;
	dm_setup_data(&dm_data);
	dm_redraw_category_screen();
#else
	RedrawCategoryFunction=RedrawCategory145Screen;
	GetCategoryHistory=GetCategory145History;
	GetCategoryHistorySize=GetCategory145HistorySize;
	RedrawCategory145Screen();
#endif
}

/*----------------------------------------------------------------------------
Function:			ExitCategory145Screen
Description:		Exits the Delivery report screen
Input Parameters:	none
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

void ExitCategory145Screen(void)
{	cat145_scrollbar_flag=0;
	reset_softkeys();
	reset_multiline_inputbox();
	ExitCategoryFunction=MMI_dummy_function;
	RedrawCategoryFunction=MMI_dummy_function;
	GetCategoryHistory=dummy_get_history;
	GetCategoryHistorySize=dummy_get_history_size;
}

/*----------------------------------------------------------------------------
Function:			GetCategory145HistorySize
Description:		Gets the size of the history buffer for the Delivery report screen
Input Parameters:	none
Output Parameters:	none
Returns:			size in U8s of the history buffer
----------------------------------------------------------------------------*/

S32 GetCategory145HistorySize(void)
{	return(0);
}

/*----------------------------------------------------------------------------
Function:			GetCategory145History
Description:		Gets the history buffer for the Delivery report screen
Input Parameters:	history_buffer	is the buffer into which the history data is stored (pre-allocated)
Output Parameters:	none
Returns:			Pointer to the history buffer
----------------------------------------------------------------------------*/

U8* GetCategory145History(U8* history_buffer)
{	return(history_buffer);
}

#if(ENABLE_EMS_TEST_DEMO)

#define EMS_TEST_DEMO_SCREEN_ID				(MAIN_MENU_BASE+80)
#define EMS_OBJECT_LIST_TEST_SCREEN_ID		(MAIN_MENU_BASE+81)

EMSData		EMS_demo_EMS_data;
S32			EMS_demo_object_index;
S32			EMS_demo_n_objects;

void EMS_demo_exit_category28(void);
void EMS_demo_exit_category29(void);
void EMS_demo_exit_object_list(void);
void EMS_demo_enter_object_list(void);
void EMS_demo_exit_object_list2(void);
void EMS_demo_enter_object_list2(void);

void EMS_demo_close_category28(void)
{	CloseCategory28Screen();
	GoBackHistory();
}

#define EMS_GSM_ASCII_TEST		0
#define EMS_OBJECT_VIEWER_TEST	0

void EMS_demo_category28(void)
{
#if(EMS_GSM_ASCII_TEST)
	U8 s[2]={	0,0	};
	S32 i;
#endif

	U8* guiBuffer;
	ExecuteCurrExitHandler();
	guiBuffer = GetCurrGuiBuffer(EMS_TEST_DEMO_SCREEN_ID);
	if(guiBuffer==NULL)
		InitializeEMSData(&EMS_demo_EMS_data, SMSAL_DEFAULT_DCS);
#if(EMS_GSM_ASCII_TEST)
	for(i=0;i<128;i++)
	{	s[0]=(U8)i;
		AddString(&EMS_demo_EMS_data,s,1,NULL);
	}
	ResetCurrentPosition(&EMS_demo_EMS_data);
	EMS_ConvertGSM7BitDefaultEncodingToAscii(&EMS_demo_EMS_data);
	EMS_ConvertAsciiEncodingToGSM7BitDefault(&EMS_demo_EMS_data);
	EMS_ConvertGSM7BitDefaultEncodingToAscii(&EMS_demo_EMS_data);
#endif

	ShowCategory28Screen(0,0,
						 STR_GLOBAL_OK,0,
						 STR_GLOBAL_BACK,0,
						 &EMS_demo_EMS_data,guiBuffer);
	SetCategory28RightSoftkeyFunction(EMS_demo_close_category28,KEY_EVENT_UP);
//	SetKeyHandler(EntryInputMethodScreen,KEY_STAR,KEY_EVENT_UP);
//	RegisterInputMethodScreenCloseFunction(GoBackHistory);
	SetExitHandler(EMS_TEST_DEMO_SCREEN_ID, EMS_demo_exit_category28);
#if(EMS_OBJECT_VIEWER_TEST)
	SetLeftSoftkeyFunction(EMS_demo_enter_object_list2,KEY_EVENT_UP);
#endif
}

void EMS_demo_exit_category28(void)
{
	history_t   h;
	h.scrnID = EMS_TEST_DEMO_SCREEN_ID;
	h.entryFuncPtr = EMS_demo_category28;
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

#define EMS_DEMO_HIDE_PREDEFINED_OBJECTS	1

EMSObject *EMS_demo_object_list;

void EMS_demo_enter_object_list(void)
{
	U8* guiBuffer;
	ExecuteCurrExitHandler();
	guiBuffer = GetCurrGuiBuffer(EMS_OBJECT_LIST_TEST_SCREEN_ID);
	EMS_demo_object_index=0;
	EMS_demo_n_objects=0;
#if(EMS_DEMO_HIDE_PREDEFINED_OBJECTS)
	CreateCategory143ObjectList(&EMS_demo_EMS_data,&EMS_demo_object_list);
#else
	EMS_demo_object_list=EMS_demo_EMS_data.listHead;
#endif
	ShowCategory143Screen(0,0,
						 0,0,
						 STR_GLOBAL_BACK,0,
						 EMS_demo_object_list,&EMS_demo_n_objects,&EMS_demo_object_index,guiBuffer);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetExitHandler(EMS_OBJECT_LIST_TEST_SCREEN_ID, EMS_demo_exit_object_list);
}

void EMS_demo_exit_object_list(void)
{
	history_t   h;
#if(EMS_DEMO_HIDE_PREDEFINED_OBJECTS)
	FreeCategory143ObjectList(&EMS_demo_object_list);
#endif
	h.scrnID = EMS_OBJECT_LIST_TEST_SCREEN_ID;
	h.entryFuncPtr = EMS_demo_enter_object_list;
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

U16 EMS_demo_object_list2[]={	0,1,2,3,4,5,6,7,8,9,	};
U8* EMS_demo_object_list3[10];

U8* EMS_demo_object_list2_loader(S32 index)
{	return(EMS_predefined_pictures[index]);
}

void EMS_demo_enter_object_list2(void)
{
	U8* guiBuffer;
	ExecuteCurrExitHandler();
	guiBuffer = GetCurrGuiBuffer(EMS_OBJECT_LIST_TEST_SCREEN_ID);
	EMS_demo_object_index=0;
	EMS_demo_n_objects=10;

	RegisterCategory144ObjectLoader(EMS_demo_object_list2_loader);
	ShowCategory144Screen(0,0,STR_GLOBAL_OK,0,STR_GLOBAL_BACK,0,
						/*EMS_PREDEFINED_PICTURE*/0,EMS_demo_n_objects,
						NULL,EMS_demo_object_list2,EMS_demo_object_list3,&EMS_demo_object_index,guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetExitHandler(EMS_OBJECT_LIST_TEST_SCREEN_ID, EMS_demo_exit_object_list2);
}

void EMS_demo_exit_object_list2(void)
{
	history_t   h;
	h.scrnID = EMS_OBJECT_LIST_TEST_SCREEN_ID;
	h.entryFuncPtr = EMS_demo_enter_object_list2;
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

/*	Cat29 Demo Entry and Exit functions	*/

S8	EMS_demo_test_string[]=
{
	'S',0,'a',0,'m',0,'p',0,'l',0,'e',0,' ',0,'S',0,'M',0,'S',0,' ',0,'t',0,'e',0,'s',0,'t',0,' ',0,'m',0,'e',0,'s',0,'s',0,'a',0,'g',0,'e',0,'.',0,' ',0,'1',0,'2',0,'3',0,'4',0,' ',0,'5',0,'6',0,'7',0,'8',0,' ',0,'9',0,'0',0,'1',0,'2',0,' ',0,'A',0,'B',0,'C',0,'D',0,' ',0,'e',0,'f',0,'g',0,'h',0,' ',0,'I',0,'J',0,'K',0,'L',0,' ',0,'m',0,'n',0,'o',0,'p',0,' ',0,'Q',0,'R',0,'S',0,'T',0,' ',0,'u',0,'v',0,'w',0,'x',0,' ',0,'Y',0,'Z',0,'\0',0
};

void EMS_demo_close_category29(void)
{	CloseCategory29Screen();
	GoBackHistory();
}

void EMS_demo_category29(void)
{
	U8* guiBuffer;

	ExecuteCurrExitHandler();
	guiBuffer = GetCurrGuiBuffer(EMS_TEST_DEMO_SCREEN_ID);
	InitializeEMSData(&EMS_demo_EMS_data, SMSAL_DEFAULT_DCS);
	AddString(&EMS_demo_EMS_data,(U8*)EMS_demo_test_string,sizeof(EMS_demo_test_string)/2,NULL);
	ShowCategory29Screen(0,0,
						 0,0,
						 STR_GLOBAL_BACK,0,
						 &EMS_demo_EMS_data,guiBuffer);
	SetRightSoftkeyFunction(EMS_demo_close_category29,KEY_EVENT_UP);
	SetExitHandler(EMS_TEST_DEMO_SCREEN_ID, EMS_demo_exit_category29);
}

void EMS_demo_exit_category29(void)
{
	history_t   h;

	h.scrnID = EMS_TEST_DEMO_SCREEN_ID;
	h.entryFuncPtr = EMS_demo_category29;
	h.inputBuffer[0]='\0';
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

S8 EMS_demo_header_string[]=
{	'S',0,'a',0,'m',0,'p',0,'l',0,'e',0,' ',0,'h',0,'e',0,'a',0,'d',0,'e',0,'r',0,'\n',0,'m',0,'e',0,'s',0,'s',0,'a',0,'g',0,'e',0,' ',0,'s',0,'t',0,'r',0,'i',0,'n',0,'g',0,'\n',0,'1',0,'2',0,'3',0,'4',0,'5',0,'6',0,'\0',0
};

S8 EMS_demo_header_ASCII_string[]=
"From: \nABCDE FGHIJKLMNOP\n03/04/2003\n08:24:35\n";

U8 EMS_demo_header_temp_string[1024];

void EMS_demo_exit_category39(void);

void EMS_demo_close_category39(void)
{	CloseCategory39Screen();
	GoBackHistory();
}

void EMS_demo_category39(void)
{
	U8* guiBuffer;

	ExecuteCurrExitHandler();
	guiBuffer = GetCurrGuiBuffer(EMS_TEST_DEMO_SCREEN_ID);
	InitializeEMSData(&EMS_demo_EMS_data, SMSAL_DEFAULT_DCS);
	AddString(&EMS_demo_EMS_data,(U8*)EMS_demo_test_string,sizeof(EMS_demo_test_string)/2,NULL);
	AnsiiToUnicodeString(EMS_demo_header_temp_string,EMS_demo_header_ASCII_string);
	ShowCategory39Screen(0,0,
						 0,0,
						 STR_GLOBAL_BACK,0,
						 &EMS_demo_EMS_data,EMS_demo_header_temp_string,guiBuffer);
	SetRightSoftkeyFunction(EMS_demo_close_category39,KEY_EVENT_UP);
	SetExitHandler(EMS_TEST_DEMO_SCREEN_ID, EMS_demo_exit_category39);
}

void EMS_demo_exit_category39(void)
{
	history_t   h;

	h.scrnID = EMS_TEST_DEMO_SCREEN_ID;
	h.entryFuncPtr = EMS_demo_category39;
	h.inputBuffer[0]='\0';
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}

#endif

MMI_BOOL is_EMS_inputbox_in_view_mode(void)
{
	if ( MMI_EMS_inputbox.flags & UI_EMS_INPUT_BOX_VIEW_MODE )
		return MMI_TRUE;
	else
		return MMI_FALSE;
}
//wangzl:add B
S32 get_ems_input_character_number(void)
{
	/*zhoumn Modify Start For 6052 Ver: TBM_780  on 2007-8-8 13:55 */
	S32	n = 0; 
#if 0
	n=MMI_EMS_inputbox.available_length-EMS_get_remaining_length(MMI_EMS_inputbox.data); // 输入的字符长度
#else
	n = MMI_EMS_inputbox.available_length - (EMS_get_remaining_length(MMI_EMS_inputbox.data) >> 1); // 输入的字符长度
#endif
	/*zhoumn Modify End  For 6052 Ver: TBM_780  on 2007-8-8 13:55 */

	if(MMI_EMS_inputbox.UCS2_count>0)
	{	
		n=MMI_EMS_inputbox.available_length-(EMS_get_remaining_length(MMI_EMS_inputbox.data)>>1);	
	}
	else if(MMI_EMS_inputbox.GSM_ext_count>0)
	{	
	/*zhoumn Modify Start For 5565 Ver: TBM_780  on 2007-8-9 15:12 */
	#if 0
		n=MMI_EMS_inputbox.available_length-EMS_get_remaining_length(MMI_EMS_inputbox.data);	
	#else
		n=MMI_EMS_inputbox.available_length-(EMS_get_remaining_length(MMI_EMS_inputbox.data)>>1);	
	#endif
	/*zhoumn Modify End  For 5565 Ver: TBM_780  on 2007-8-9 15:12 */
	}
	
	return n;
}

S32 get_max_editor_height(S32 editor_top) {
	/*----------------------------------------------------------------*/
   	/* Code Body                                                      */
   	/*----------------------------------------------------------------*/
	S32 bottom; 

	// get most bottom y-ordinate in content area, 
	bottom = MMI_content_y + MMI_content_height;
	
	return ((bottom < 0) ? 0 : (bottom - editor_top));
}
//wangzl:add E
