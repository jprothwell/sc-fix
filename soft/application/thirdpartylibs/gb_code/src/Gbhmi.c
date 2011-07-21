#include "eb_funcs.h"
#include "eb_structures.h"
#include "eb_defines.h"
#include "eb.h"
#include "eb_types.h"
#include "gblt.h"
#include "gblt_def.h"
#include "gblt_func.h"
//#include "utils.h"

#include "frameworkstruct.h"//renwy move 2008/09/28
#include "gbar_tran.h"
#include "debuginitdef.h"
#include "gbhmi.h"
#include "gui_inputs.h"
#include "oslmemory.h"
#include "globalconstants.h"
#include "eventsdef.h"
#include "mmi_data_types.h"
//#include "frameworkstruct.h"
#include "eventsdef.h"
#include "wgui.h"
#include "timerevents.h"
#include "debuginitdef.h"
#include "custdatares.h"
#include "messagesresourcedata.h"
#include "globaldefs.h"
#include "gui_config.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_defs.h"
#include "gui_ems.h"
#include "t9def.h"
#include "commonscreens.h"
#include "gui.h"
#include "wgui_softkeys.h"
#include "wgui_ems_categories.h"

#include "mmi_trace.h"
#include "wgui_categories_cm.h"
#ifdef __MMI_TOUCH_SCREEN__
#include "editorpen.h"
#endif
#if defined(__MMI_LANG_ARABIC__)||defined(__MMI_LANG_PERSIAN__)
//GBT_SECTION gbTranSection;
#endif



#include "engine.h"

//solution for composition and candidate window display
#ifdef __GB_CANDIDATE_AREA_WITH_ARROW__
#if defined(__MMI_MAINLCD_220X176__)
#define GB_CANDIDATE_X MMI_multitap_x + GB_ARROW_WIDTH -2
#else
#define GB_CANDIDATE_X MMI_multitap_x +10
#endif
#else
#define GB_CANDIDATE_X MMI_multitap_x + GB_ARROW_WIDTH
#endif
#ifdef __GB_COMPOSITION_AREA_WITH_ARROW__
#if defined(__MMI_MAINLCD_220X176__)
#define GB_COMPOSITION_X MMI_multitap_x + GB_ARROW_WIDTH
#else
#define GB_COMPOSITION_X MMI_multitap_x + 10
#endif
#else
#define GB_COMPOSITION_X MMI_multitap_x
#endif
#if defined(__MMI_MAINLCD_220X176__)
#define GB_COMPOSITION_Y  UI_device_height-MMI_BUTTON_BAR_HEIGHT-1-MMI_multitap_height-1-MMI_multitap_height
#define GB_CANDIDATE_Y  UI_device_height-MMI_BUTTON_BAR_HEIGHT-1-MMI_multitap_height
#else
#if defined(__PROJECT_GALLITE_C01__)
#define GB_COMPOSITION_Y  UI_device_height-MMI_BUTTON_BAR_HEIGHT-MMI_multitap_height-1-MMI_multitap_height
#define GB_CANDIDATE_Y  UI_device_height-MMI_BUTTON_BAR_HEIGHT-MMI_multitap_height
#else
#define GB_COMPOSITION_Y  UI_device_height-MMI_BUTTON_BAR_HEIGHT-MMI_multitap_height-1-MMI_multitap_height+1 
#define GB_CANDIDATE_Y  UI_device_height-MMI_BUTTON_BAR_HEIGHT-MMI_multitap_height+1 
#endif
#endif

#define GbInputHandlerFun GbIMEprocessCharInput
#if defined (MONO_LCD)
UI_filled_area MMI_GB_COMPOSITION_WINDOW_filler=
{
    UI_FILLED_AREA_TYPE_COLOR,//|UI_FILLED_AREA_SINGLE_BORDER,
    UI_NULL_IMAGE,
    NULL,
	{    WHITE    },//{    206,206,206,100    },
	{    BLACK    	},
	{    BLACK    	},
	{WHITE},//{    128,128,128,100    },
    0,
};

#else
UI_filled_area MMI_GB_COMPOSITION_WINDOW_filler=
{
    UI_FILLED_AREA_TYPE_COLOR,//|UI_FILLED_AREA_SINGLE_BORDER,
    UI_NULL_IMAGE,
    NULL,
	{    182,189,210,100    },//{    206,206,206,100    },
	{    0,0,0,0    		},
	{    0,0,0,100    	},
	{10,36,106,100},//{    128,128,128,100    },
    0,
};
#endif

extern S32 MMI_multiline_inputbox_x;
extern S32 MMI_multiline_inputbox_y;
extern S32 MMI_multiline_inputbox_width;
extern S32 MMI_multiline_inputbox_height;
extern S32 UI_device_height;
extern S32 MMI_title_y;
extern S32 MMI_title_height;
extern S32 MMI_button_bar_height;
extern S32  MMI_multitap_x;
extern S32    MMI_multitap_y;
extern S32    MMI_multitap_width;
extern S32  MMI_multitap_height;
extern UI_EMS_input_box    MMI_EMS_inputbox;

extern s16     MMI_current_input_type;
s16 GBIMEmethod;

#if defined(__MMI_GB__)
extern mmi_editor_inoutbox_enum MMI_current_input_box_type;							 
#endif

//#ifdef __MMI_DEFAULT_THEME_3__
extern CONST_HARDWARE UI_filled_area MMI_multitap_filler;
extern CONST_HARDWARE color_t multitap_selector_txt_col_Default;
extern CONST_HARDWARE color_t multitap_selected_txt_col_Default;
//#endif
extern byte inline_full_screen_edit_changed;
extern byte wgui_inline_list_menu_disable_done;
extern EMSTextFormat        wgui_EMS_text_format;
extern byte    	    wgui_singleline_inputbox_RSK_label_clear;
extern UI_theme *current_UI_theme;
extern UI_multitap_input_theme* current_multitap_input_theme;
extern byte    	    category28_RSK_label_clear;
extern UI_string_type  category28_RSK_label_string;
extern UI_image_type   category28_RSK_label_icon;
extern byte    	    wgui_inputbox_RSK_label_clear;
extern UI_string_type  wgui_inputbox_RSK_label_string;
extern UI_image_type   wgui_inputbox_RSK_label_icon;

extern void (*wgui_inline_edit_DONE_function)(void);

extern void (*coolsand_UI_start_timer)(S32 count,void (*callback)(void));
extern void coolsand_UI_show_EMS_input_box(UI_EMS_input_box *b);
extern void coolsand_UI_show_single_line_input_box(single_line_input_box *b);
extern void (*coolsand_UI_BLT_double_buffer)(S32 x1,S32 y1,S32 x2,S32 y2);
extern void (*coolsand_UI_lock_double_buffer)(void);
extern void (*coolsand_UI_unlock_double_buffer)(void);
extern void coolsand_UI_create_single_line_input_box_use_buffer(single_line_input_box* b,S32 x,S32 y,S32 width,S32 height,UI_string_type buffer,S32 length);
#if 0
extern void redraw_left_softkey(void);
extern void redraw_right_softkey(void);
#endif
extern void wgui_EMS_redraw_remaining_characters_display(void);
//extern void coolsand_UI_EMS_input_box_insert_character(UI_EMS_input_box *b,UI_character_type c);		 //tangjie del 20060628
extern U8   coolsand_UI_EMS_input_box_insert_character(UI_EMS_input_box *b,UI_character_type c);   //tangjie add 20060628
extern void coolsand_UI_EMS_input_box_backspace(UI_EMS_input_box *b);
extern void coolsand_UI_single_line_input_box_next(single_line_input_box *b);
extern void coolsand_UI_single_line_input_box_previous(single_line_input_box *b);
extern void (*hide_multitap)(void);
extern void coolsand_UI_single_line_input_box_goto_first_character(single_line_input_box *b);
extern void coolsand_UI_EMS_input_box_delete_all(UI_EMS_input_box *b);
extern void coolsand_UI_EMS_input_box_previous(UI_EMS_input_box *b);
extern void coolsand_UI_EMS_input_box_next(UI_EMS_input_box *b);
extern void coolsand_UI_EMS_input_box_previous_line(UI_EMS_input_box *b);
extern void coolsand_UI_EMS_input_box_next_line(UI_EMS_input_box *b);
extern S32 coolsand_UI_EMS_input_box_highlight_cursor_end(UI_EMS_input_box *b);
extern S32 coolsand_UI_EMS_input_box_highlight_cursor_start(UI_EMS_input_box *b);
extern S32 coolsand_UI_EMS_input_box_highlight_cursor_number(UI_EMS_input_box *b, S32 numOfChar );
extern void register_category28_handle_right_softkey_up(void);
extern S8 *UCS2Strcat( S8 *strDestination, const S8 *strSource );
extern S8 *UCS2Strcpy(S8 *strDestination, const S8 *strSource );
extern S32 UCS2Strlen(const S8 *arrOut);
extern S32 coolsand_UI_single_line_input_box_highlight_cursor_start(single_line_input_box *b);
extern void    (*coolsand_UI_cancel_timer)(void (*callback)(void));
extern void coolsand_UI_multi_line_input_box_insert_character(multi_line_input_box *b,UI_character_type c);
extern void coolsand_UI_multi_line_input_box_delete_character(multi_line_input_box *b);
extern void coolsand_UI_multi_line_input_box_previous(multi_line_input_box *b);
extern void coolsand_UI_multi_line_input_box_next(multi_line_input_box *b);
extern void wgui_redraw_remaining_characters_display(void);
extern S32 coolsand_UI_multi_line_input_box_highlight_cursor_number(multi_line_input_box *b,S32 n);
extern void wgui_register_inputbox_handle_right_softkey_up(void);
extern void coolsand_UI_multi_line_input_box_delete_all(multi_line_input_box *b);
extern void wgui_handle_inputbox_input(void);
extern void handle_category28_input(void);
extern void show_EMS_inputbox(void);
extern void show_multiline_inputbox(void);
extern S32 coolsand_UI_single_line_input_box_highlight_cursor_number(single_line_input_box *b, S32 numOfChar);
extern S32 coolsand_UI_single_line_input_box_insert_string(single_line_input_box *b,UI_string_type s, S32 o);
extern void coolsand_UI_resize_EMS_input_box(UI_EMS_input_box *b,S32 width,S32 height);
extern void    (*coolsand_UI_reset_clip)(void);
extern color_t    (*coolsand_UI_color)(byte r,byte g,byte b);
extern void draw_resizeable_arrow(S32 x,S32 y,U8 arrow_type,color_t c, S32 width, S32 height);
extern void draw_arrow(S32 x,S32 y,U8 arrow_type,color_t c);
extern void    (*coolsand_UI_putpixel)(S32 x,S32 y,color_t c);
extern U16 StopMyTimer(U16 nTimerId);
extern U16 StartMyTimerInt(U16 nTimerId,U32 nTimeDuration,oslTimerFuncPtr TimerExpiry, U8 alignment);
extern void wgui_register_singleline_inputbox_handle_right_softkey_up(void);
extern void wgui_handle_singleline_inputbox_input(void);
extern void redraw_input_mode_only(void);
extern void redraw_fixed_list(void);
extern void coolsand_UI_show_fixed_list_menu(fixed_list_menu *m);
extern void PbookListSearchEntryReq(void);
extern void show_softkey_background(void);
extern void UI_editor_play_tone_cannot_insert(void);
extern void register_category28_handle_clear_key_up(void);
extern void wgui_register_inputbox_handle_clear_key_up(void);
extern void wgui_handle_inputbox_right_softkey_up(void);
extern void handle_category28_right_softkey_up(void);
extern void EntrySymbolPickerScreenForCurrentEMSScreen(void);
extern void wgui_handle_inputbox_right_softkey_long_press(void);
extern void wgui_redraw_input_information_bar(void);
extern void handle_category28_change_input_mode(void);
extern void wgui_handle_singleline_inputbox_change_input_mode(void);
extern void UI_fill_rectangle(S32 x1, S32 y1, S32 x2, S32 y2, color_t c);
void (*gb_inputbox_LSK_function)(void)=NULL;
void (*gb_inputbox_ENTER_function)(void) = NULL;
void GbKeyLSKPressHandler(void);
void GBSelectCharacters(void);
void GBDeleteKey(void);
void GB_Reset(void);
void GBDelectcharater(void);
void GbKeyRSKPressHandler(void);
void GBSetInputboxLSKFunction(FuncPtr f);
void GbKey1PressHandler(void);
void GbKey2PressHandler(void);
void GbKey3PressHandler(void);
void GbKey4PressHandler(void);
void GbKey5PressHandler(void);
void GbKey6PressHandler(void);
void GbKey7PressHandler(void);
void GbKey8PressHandler(void);
void GbKey9PressHandler(void);
void GbKey0PressHandler(void);
void GBClearSixToNineKEY( void );
GBRETURN GbIMEAddInputChar(U8 inChar);

#define GB_SELECTION_BUFFER_SIZE 64
#if defined(__MMI_GB__)
#define GbLargestByteNumberInSms    			    612    //wangzl:modify from 612  //guojian restore for bug 10360    		////当支持长短信时，初始化最大可输入612个英文字符，当我们还没有进入  
													//输入操作时，可提示最大可输入的个数
#define TextLengthInEmsFourLine    		    56    	//短信中文输入4行时的文本长度
#define TextLengthInEms    				  84    	//当短信的长度将要达到最下面一行时的短信长度
//wangzl:modify B
#ifdef __MMI_TOUCH_SCREEN__
#ifdef __MMI_MAINLCD_240X320__
#define GbEmsEditorHeightInIdle    				   MMI_content_height    		//90    	//当输入短信超过一屏时，编辑框的高度。此时缩小编辑框的高度是为了使输入的汉字不把编辑框的字体给屏蔽掉
#define GbMultilineEditorHeightInIdle    		    MMI_content_height    		//107    	//当输入的短信的长度还没达到一屏时，编辑框的高度。
#elif defined __MMI_MAINLCD_128X128__    			  //roger
#define GbEmsEditorHeightInIdle    				   100//90    	//当输入短信超过一屏时，编辑框的高度。此时缩小编辑框的高度是为了使输入的汉字不把编辑框的字体给屏蔽掉
#define GbMultilineEditorHeightInIdle    			   100//107    	//当输入的短信的长度还没达到一屏时，编辑框的高度。
#elif defined __MMI_MAINLCD_176X220__
#define GbEmsEditorHeightInIdle    				   122//90    	//当输入短信超过一屏时，编辑框的高度。此时缩小编辑框的高度是为了使输入的汉字不把编辑框的字体给屏蔽掉
#define GbMultilineEditorHeightInIdle    			   122//107    	//当输入的短信的长度还没达到一屏时，编辑框的高度。
#elif defined __MMI_MAINLCD_220X176__ /*cong.li added for product QPAD[220*176] on 2009.08.11. */
#define GbEmsEditorHeightInIdle    				   122//90    	//当输入短信超过一屏时，编辑框的高度。此时缩小编辑框的高度是为了使输入的汉字不把编辑框的字体给屏蔽掉
#define GbMultilineEditorHeightInIdle    			   122//107    	//当输入的短信的长度还没达到一屏时，编辑框的高度。
#else //__MMI_MAINLCD_128X160__
#define GbEmsEditorHeightInIdle    					 100//90    	//当输入短信超过一屏时，编辑框的高度。此时缩小编辑框的高度是为了使输入的汉字不把编辑框的字体给屏蔽掉
#define GbMultilineEditorHeightInIdle    			   100//107    	//当输入的短信的长度还没达到一屏时，编辑框的高度。
#endif
#else
#ifdef __MMI_MAINLCD_128X160__
#define GbEmsEditorHeightInIdle    				  (MMI_CONTENT_HEIGHT-32)		//当输入短信超过一屏时，编辑框的高度。此时缩小编辑框的高度是为了使输入的汉字不把编辑框的字体给屏蔽掉
#define GbMultilineEditorHeightInIdle    			   (MMI_CONTENT_HEIGHT-32)		//当输入的短信的长度还没达到一屏时，编辑框的高度。
#elif defined __MMI_MAINLCD_128X128__
#define GbEmsEditorHeightInIdle    					 (MMI_CONTENT_HEIGHT -32)		//当输入短信超过一屏时，编辑框的高度。此时缩小编辑框的高度是为了使输入的汉字不把编辑框的字体给屏蔽掉
#define GbMultilineEditorHeightInIdle    			   (MMI_CONTENT_HEIGHT - 32)		//当输入的短信的长度还没达到一屏时，编辑框的高度。
#elif defined(__MMI_MAINLCD_160X128__)
#define GbEmsEditorHeightInIdle    				  (MMI_CONTENT_HEIGHT-32)		//当输入短信超过一屏时，编辑框的高度。此时缩小编辑框的高度是为了使输入的汉字不把编辑框的字体给屏蔽掉
#define GbMultilineEditorHeightInIdle    			   (MMI_CONTENT_HEIGHT-32)		//当输入的短信的长度还没达到一屏时，编辑框的高度。
#elif defined __MMI_MAINLCD_176X220__
#define GbEmsEditorHeightInIdle    					 (MMI_content_height -42)		//当输入短信超过一屏时，编辑框的高度。此时缩小编辑框的高度是为了使输入的汉字不把编辑框的字体给屏蔽掉
#define GbMultilineEditorHeightInIdle    			   (MMI_content_height - 42)		//当输入的短信的长度还没达到一屏时，编辑框的高度。
#elif defined __MMI_MAINLCD_220X176__ /*cong.li added for product QPAD[220*176] on 2009.08.11. */
#define GbEmsEditorHeightInIdle    					 (MMI_CONTENT_HEIGHT -32)		//当输入短信超过一屏时，编辑框的高度。此时缩小编辑框的高度是为了使输入的汉字不把编辑框的字体给屏蔽掉
#define GbMultilineEditorHeightInIdle    			   (MMI_CONTENT_HEIGHT - 32)		//当输入的短信的长度还没达到一屏时，编辑框的高度。
#elif defined __MMI_MAINLCD_240X320__
#define GbEmsEditorHeightInIdle    					 MMI_CONTENT_HEIGHT    	//当输入短信超过一屏时，编辑框的高度。此时缩小编辑框的高度是为了使输入的汉字不把编辑框的字体给屏蔽掉
#define GbMultilineEditorHeightInIdle    			   MMI_CONTENT_HEIGHT    	//当输入的短信的长度还没达到一屏时，编辑框的高度。
#endif
#endif
//wangzl:modify E
#endif

extern const unsigned long prv_dataArray[];
extern const unsigned long prv_enDataArray[];
extern const unsigned long prv_ruDataArray[];
/*JINZH Add Start For 英语-法语-阿拉伯切换 Ver: 多国语言  on 2007-12-21 9:31 */
extern const unsigned long prv_frDataArray[];
/*JINZH Add End  For 英语-法语-阿拉伯切换 Ver: 多国语言  on 2007-12-21 9:31 */
extern const unsigned long prv_italianDataArray[];   //added by kecx for adding italian on 20081223
extern const unsigned long prv_enArabicDataArray[];
extern const unsigned long gbar_arabic_tranform_data[];
#ifdef __MMI_LANG_PERSIAN__
extern const unsigned long gbar_persian_tranform_data[];
extern const unsigned long prv_enPersianDataArray[];
#endif
#ifdef __MMI_SMART_TURKISH__
extern const unsigned long prv_turkishDataArray[];
#endif
#ifdef __MMI_SMART_INDONESIAN__
extern const unsigned long prv_indonesianDataArray[];
#endif
#ifdef __MMI_SMART_MALAY__
extern const unsigned long prv_malayDataArray[];
#endif
#ifdef __MMI_SMART_TAMIL__
extern const unsigned long prv_tamilDataArray[];
#endif
extern const unsigned long prv_trdataArray[];

extern const unsigned long prv_gerDataArray[];		
extern const unsigned long prv_hiDataArray[];		
extern const unsigned long prv_spaDataArray[];
extern const unsigned long prv_ptDataArray[];
extern const unsigned long prv_teDataArray[];
#ifndef __MMI_GB_V5__
EBInputStruct ebis;
ENInputStruct enis;
#endif
single_line_input_box gbCompositionArea;
single_line_input_box gbCandidateArea;
single_line_input_box gbStrokArea;
EBLong gbEMSEditorHeight;
EBLong gbMultilineEditorHeight;
EBWord  gbCompositionBuffer[GB_COMPOSITION_BUFFER_SIZE+1];
EBWord  gbCandidateBuffer[GB_CANDIDATE_MAX_BUFFER_SIZE+1];
EBWord  gbStrokCandidateBuffer[30];
EBLong idxCandidate=0;
GBIMESTATE gbstate=GBIMESTATE_IDLE;  //indication whether input methode work well
EBLong GbInputMethodCategoryFlag;
EBByte gbSelectWordFlag=0;
unsigned short SmartOption = 0;

#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
single_line_input_box gcj_native_input_box;
extern unsigned short gcj_InputBuff[];
void Gb_Ime_hide_input_area(void);
void Gb_Ime_show_input_box(void);
void Gb_Ime_delete_in_input_box(void);
#endif


//API for cool profile
#define u16 UINT16
#define u8   UINT8
extern void hal_PXTS_SendProfilingCode(u16 code);
#define CPMASK 0x3fff
#define CPEXITFLAG 0x8000
#define CP_ENTRY(id) hal_PXTS_SendProfilingCode((id) & CPMASK)
#define CP_EXIT(id) hal_PXTS_SendProfilingCode(((id) & CPMASK) | CPEXITFLAG)

extern icontext_button MMI_softkeys[];

static UI_string_type gb_LSK_string_hold ;

#ifdef __STROKE_24568__
typedef U8 key_t[10];

key_t INPUT_KEY_MAP[2] ={
{GBKEY0, GBKEY6, GBKEY1, GBKEY8, GBKEY3, GBKEY2, GBKEY4, GBKEY7, GBKEY5, GBKEY9},
{'0',		 '2',		  '5',		  '4',		 '6',		 '8',		 '1',		  '7',		 '3',		  '9'},
};

key_t *p_mmi_key = INPUT_KEY_MAP[0];
key_t *p_guobi_key = INPUT_KEY_MAP[1];

U8 ConvertMMIKeyToGuobiKey(U8 mmiKey)
{
	return (INPUT_KEY_MAP[0])[mmiKey];
	//return *(p_mmi_key + mmiKey);
}

U8 ConvertGuobiKeyToMmiKey(U8 gbKey)
{
	return (INPUT_KEY_MAP[1])[gbKey];
	//return *(p_guobi_key + gbKey);
}
#endif

#include "gbapi.h"
#ifndef _C166
#define _huge
#endif

#ifdef __MMI_GB_V5__
extern GBInputStruct _huge g_gbis;
#endif

#define PRINT_ENIS() Trace_Enis(__FUNCTION__, __FILE__, __LINE__)

void Trace_Enis(const U8 *funcName, const U8 *fileName, INT32 fileLine)
{
#ifdef __MMI_GB_V5__
	int i;
	int j;
	mmi_trace(g_sw_IM, "MMI_INPUT: %s,%d,", funcName, fileLine);
	mmi_trace(g_sw_IM, "MMI_INPUT: g_gbis.nInputMode is %d,g_gbis.nLang is %d,g_gbis.nCandType is %d, g_gbis.flags is %d",g_gbis.nInputMode, g_gbis.nLang, g_gbis.nCandType, g_gbis.flags);
	mmi_trace(g_sw_IM, "MMI_INPUT: g_gbis.inputStringLen is %d,",g_gbis.inputStringLen);
	for(i = 0; i < g_gbis.inputStringLen; i++)
	{
		mmi_trace(g_sw_IM, "MMI_INPUT: g_gbis.inputString[%d] is %d",i,g_gbis.inputString[i]);
	}
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		mmi_trace(g_sw_IM, "MMI_INPUT: g_gbis.pAuxInfo->nSyllableNum is %d",g_gbis.pAuxInfo->nSyllableNum);
		for(i = 0; i < g_gbis.pAuxInfo->nSyllableNum; i++)
		{
			GBPCCHAR pSyllable = g_gbis.pAuxInfo->pSyllables[i];
			for(j = 0; j < UCS2Strlen((const S8 *)pSyllable); j++)
			{
				mmi_trace(g_sw_IM, "MMI_INPUT: pSyllable[%d] is 0x%x", j, *(pSyllable+j));
			}
		}
	}
	mmi_trace(g_sw_IM, "MMI_INPUT: g_gbis.outputInfo.nCandNum is %d,idxCandidate is %d",g_gbis.outputInfo.nCandNum,idxCandidate);
	for(i = 0; i < g_gbis.outputInfo.nCandNum; i++)
	{
		GBPCCHAR pCandidate = g_gbis.outputInfo.pCandidates[i];
		mmi_trace(g_sw_IM, "MMI_INPUT: UCS2Strlen of candidate%d is %d",i, UCS2Strlen((const S8 *)pCandidate));
		for(j = 0; j < UCS2Strlen((const S8 *)pCandidate); j++)
		{
			mmi_trace(g_sw_IM, "MMI_INPUT: pCandidate[%d] is 0x%x", j, *(pCandidate+j));
		}
	}
#else
	INT32 i = 0;
	UINT32 j = 0;
	char buf[256];
	char temp[32];

	memset(buf, 0, sizeof(buf));

	mmi_trace(g_sw_IM, "Func: %s, line: %d :enis candiatecount = %d,  inputMethod = %d", funcName, fileLine, enis.candidateCount,enis.option);

	for(i = 0; i <  enis.inputLen; i++)
	{
		memset(temp, 0, sizeof(temp));	
		memset(buf, 0, sizeof(buf));	
		sprintf((S8*)temp, "inputString[%d]: %c,", (int)i, (S8)enis.inputString[i]);
		strcpy((S8*)buf, (S8*)temp);
		mmi_trace(g_sw_IM, "inputString: %s", buf);
	}

	memset(buf, 0, sizeof(buf));
	for(i = 0; i < enis.candidateCount; i++)
	{
		for (j = 0; j < UCS2Strlen((S8*)enis.candidates[i]); j++)
		{
			if (j < sizeof(temp) / sizeof(temp[0]))
			{
				memset(temp, 0, sizeof(temp));
				memset(buf, 0, sizeof(buf));
				sprintf((S8*)temp, "enis.candidate[%d][%d]: 0x%02x,", (int)i,(int)j,enis.candidates[i][j]);
				strcpy((S8*)buf, (S8*)temp);
				mmi_trace(g_sw_IM, " %s", buf);
			}
		}
	}
#endif
}

void GbIMEEnShutdown(void)
{
#ifdef __MMI_GB_V5__
	// TODO:
#else
	EnShutDown(&enis);
#endif
}
#ifdef __MMI_GB_V5__
U8 upper_lower_status = 0;
void GbIMESetEngineCurrentLang(GBINT nLangID, GBINT nInputMode, U8 upper_lower)
{

	GBSetCurLanguage(&g_gbis, nLangID, 0);
	GBSetInputMode(&g_gbis, nInputMode);
	UpdateConfig(upper_lower);
	upper_lower_status = upper_lower;
}

BOOL GbIMEIsUpperOutput(void)
{
	if(upper_lower_status)
		return TRUE;
	return FALSE;
}

GBRETURN GbIMEGetNextPageCandidate(void)
{
	GBRETURN ret = 0;
	
	if(!GBIS_Is_CandSelected(&g_gbis))
		GbIMEAddInputChar(GBKEY_DOWN);
	
	ret = GbIMEAddInputChar(GBKEY_DOWN);

	return ret;
		
}

GBRETURN GbIMEGetPrevPageCandidate(void)
{
	GBRETURN ret = GBOK;
	
	if(!GBIS_Is_CandSelected(&g_gbis))
		GbIMEAddInputChar(GBKEY_DOWN);
	
	ret = GbIMEAddInputChar(GBKEY_UP);

	return ret;
}
GBRETURN GbIMEGetNextCandidate(void)
{
	GBRETURN ret = GBOK;
	
	if(!GBIS_Is_CandSelected(&g_gbis))
		GbIMEAddInputChar(GBKEY_DOWN);
	
	ret = GbIMEAddInputChar(GBKEY_RIGHT);

	return ret;
}

GBRETURN GbIMEGetPrevCandidate(void)
{
	GBRETURN ret = GBOK;
	
	if(!GBIS_Is_CandSelected(&g_gbis))
		GbIMEAddInputChar(GBKEY_DOWN);
	
	ret = GbIMEAddInputChar(GBKEY_LEFT);

	return ret;
}

GBRETURN GbIMEGetNextSyllable(void)
{
	GBRETURN ret = GBOK;
	
	if(GBIS_Is_CandSelected(&g_gbis))
		GbIMEAddInputChar(GBKEY_UP);
	
	ret = GbIMEAddInputChar(GBKEY_RIGHT);

	return ret;
}

GBRETURN GbIMEGetPrevSyllable(void)
{
	GBRETURN ret = GBOK;
	
	if(GBIS_Is_CandSelected(&g_gbis))
		GbIMEAddInputChar(GBKEY_UP);
	
	ret = GbIMEAddInputChar(GBKEY_LEFT);

	return ret;
}

#endif
GBRETURN GbIMEAddInputChar(U8 inChar)
{
	GBRETURN ret = 0;
#ifdef __MMI_GB_V5__
	ret = GBMMIHandleKeyEvent(&g_gbis, (GBINT)inChar,GBEvent_None);
	mmi_trace(g_sw_IM, "MMI_INPUT: GbIMEAddInputChar, call GBMMIHandleKeyEvent return 0x%x",ret);
	if(GbIMEIsUpperOutput())
	{
		GBGetDesireOutput(&g_gbis, GBA_OUTPUT_CAPITAL); 
	}
	else
	{
		GBGetDesireOutput(&g_gbis, GBA_OUTPUT_LOWER); 
	}
#else
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		ret = EBHelperAddInputChar(&ebis, inChar);
	}
	else
	{
		enis.inputString[enis.inputLen++] = inChar;
	}
#endif
	return ret;
}


GBRETURN GbIMEDeleteInputChar(void)
{
	GBRETURN ret = 0;
#ifdef __MMI_GB_V5__
	ret = GBMMIHandleKeyEvent(&g_gbis, GBKEY_BACK,GBEvent_None);
	mmi_trace(g_sw_IM, "MMI_INPUT: GbIMEDeleteInputChar, call GBMMIHandleKeyEvent return 0x%x",ret);
#else
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		EBHelperDeleteInputChar(&ebis);
		ebis.inputString[ebis.inputStringLen] = 0;
	}
	else
	{
		enis.inputString[--enis.inputLen] = 0;
	}
#endif
	return ret;
}

BOOL GbIMEIsInvalidInput(void)
{
#ifdef __MMI_GB_V5__
	return TRUE;
#else
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		if(EBFindFirstPageC(&ebis) == EB_OK && ebis.inputStringLen <= GB_MAX_INPUT_LEN)
			return TRUE;
	}
	else
	{
		if (EnFirstHeader(&enis) == EBEN_OK)
			return TRUE;
	}
	
#endif
	return FALSE;
}

U8 GbIMEGetSyllableIndex(void)
{
#ifdef __MMI_GB_V5__
	return g_gbis.pAuxInfo->nSyllableIndex;
#else
	ebis.syllableIndex;
#endif
}
U8 GbIMEGetSyllableNumber(void)
{
#ifdef __MMI_GB_V5__
	return g_gbis.pAuxInfo->nSyllableNum;
#else
	return ebis.syllableLen;
#endif
}
BOOL GbIMEHasInput(void)
{
#ifdef __MMI_GB_V5__
	if(g_gbis.inputStringLen > 0)
		return TRUE;
#else
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		if(ebis.inputStringLen > 0)
			return TRUE;
	}
	else
	{
		if (enis.inputLen > 0)
			return TRUE;
	}
#endif
	return FALSE;	
}
BOOL GbIMEHasAssociate(void)
{
#ifdef __MMI_GB_V5__
	if(g_gbis.outputInfo.nCandNum != 0)
		return TRUE;
#else
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		if(ebis.associateISNCount > 0)
			return TRUE;
	}
	else
	{
		
	}
#endif
	return FALSE;	
}
U8 GbIMEGetInputStrLen(void)
{
#ifdef __MMI_GB_V5__
	return g_gbis.inputStringLen;
#else
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		return ebis.inputStringLen;
	}
	else
	{
		return enis.inputLen;
	}
#endif
}

U8 GbIMEGetCandidateCount(void)
{
#ifdef __MMI_GB_V5__
	return (U8)g_gbis.outputInfo.nCandNum;
#else
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		return ebis.candidateLen;
	}
	else
	{
		return enis.candidateCount;
	}
#endif
}

BOOL GbIMECanPageUp(void)
{
#ifdef __MMI_GB_V5__
	if(GBHavePrevPage(&g_gbis) == GBOK)
		return TRUE;
	else
		return FALSE;
#else
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		if(EBCanPageUp(&ebis) == EB_OK)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if(EnCanPageUp(&enis) == EBEN_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif
}

BOOL GbIMECanPageDown(void)
{
#ifdef __MMI_GB_V5__
	if(GBHaveNextPage(&g_gbis) == GBOK)
		return TRUE;
	else
		return FALSE;
#else
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		if(EBCanPageDown(&ebis) == EB_OK)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if(EnCanPageDown(&enis) == EBEN_OK)
			return TRUE;
		else
			return FALSE;
	}
#endif
}

void GbIMESetCandidateInfo(U8 worldLen, U8 CandidateNum)
{
#ifdef __MMI_GB_V5__
	GBGlobalPageOption pageOption;
	GBGetPageOption(&g_gbis, &pageOption);
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		pageOption.mChnCandPageOption.nMaxCandCountPerPage = CandidateNum;
	}
	else
	{
		pageOption.mPageOption.nMaxCandCountPerPage = CandidateNum;
		
	}
	mmi_trace(g_sw_IM, "MMI_INPUT: GbIMESetCandidateInfo set candidatenum to %d",CandidateNum);
	GBSetPageOption(&g_gbis, &pageOption);
#else
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		ebis.candidateDesireLen = CandidateNum;
		ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
	}
	else
	{
		enis.candidateDesireLen = worldLen;
		enis.candidateDesireCount = CandidateNum;
	}
#endif
	
}
void GbInuptMethodEnterCategory5(void)
{
	gb_LSK_string_hold = MMI_softkeys[MMI_LEFT_SOFTKEY].text;
	GbInputMethodCategoryFlag|= GB_IN_CATEGORY_5_FLAG;
}

void GbInuptMethodEnterSinglelineInputBox(void)
{
	gb_LSK_string_hold = MMI_softkeys[MMI_LEFT_SOFTKEY].text;
	GbInputMethodCategoryFlag |= GB_IN_CATEGORY_SINGLELINE_INPUTBOX_FLAG;
}

void GbInuptMethodEnterCategory28(void)
{
	gb_LSK_string_hold = MMI_softkeys[MMI_LEFT_SOFTKEY].text;
	GbInputMethodCategoryFlag|= GB_IN_CATEGORY_28_FLAG;
}

void GbInuptMethodExitCategory28(void)
{
	if(NULL != gb_inputbox_ENTER_function)
	{
		SetKeyHandler(gb_inputbox_ENTER_function, KEY_ENTER, KEY_EVENT_UP);
		gb_inputbox_ENTER_function = NULL;
	}
	GbInputMethodCategoryFlag&= ~GB_IN_CATEGORY_28_FLAG;
	gb_LSK_string_hold = 0;
}

void GbInuptMethodExitCategory5(void)
{
	GbInputMethodCategoryFlag&= ~GB_IN_CATEGORY_5_FLAG;
	gb_LSK_string_hold = 0;
}

void GbInuptMethodExitSinglelineInputBox(void)
{
	GbInputMethodCategoryFlag &= ~GB_IN_CATEGORY_SINGLELINE_INPUTBOX_FLAG;
	gb_LSK_string_hold = 0;
}

/**************************************************************

    FUNCTION NAME    	: GbChangeInputMethod

	  PURPOSE    			: 改变当前输入法
    INPUT PARAMETERS    : nil

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/
void GbChangeInputMethod(U16 inputMode)
{
#ifdef __MMI_GB_V5__
	GBINT CandType = 0; //0,小写; 1,大写
	GBINT nInputMode = GBIM_Predictive;
	GBINT nLangID = GBL_English;
	mmi_trace(g_sw_IM, "GbChangeInputMethod to %d",inputMode);
	GBIMEmethod = inputMode;
	switch(inputMode)
	{
		case INPUT_MODE_SMART_LOWERCASE_ABC:
			CandType = 0;
			nLangID = GBL_English;
			nInputMode = GBIM_Predictive;
			break;
		case INPUT_MODE_SMART_UPPERCASE_ABC:
			CandType = 1;
			nLangID = GBL_English;
			nInputMode = GBIM_Predictive;
			break;
		case INPUT_MODE_SM_PINYIN:
			CandType = 0;
			nLangID = GBL_Chinese;
			nInputMode = GBIM_Pinyin;
			GbIMESetCandidateInfo(GB_CANDIDATE_LEN, GB_CH_CANDIDATE_COUNT);
			break;
		case INPUT_MODE_SM_STROKE:
			CandType = 0;
			nLangID = GBL_Chinese;
			nInputMode = GBIM_Stroke;
			GbIMESetCandidateInfo(GB_CANDIDATE_LEN, GB_CH_CANDIDATE_COUNT);
			break;
	#ifdef __MMI_SMART_URDU__
		case INPUT_MODE_SMART_URDU:
			CandType = 0;
			nLangID = GBL_Urdu;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_PUNJABI__
		case INPUT_MODE_SMART_PUNJABI:
			CandType = 0;
			nLangID = GBL_Punjabi;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_HINDI__
		case INPUT_MODE_SMART_HINDI:
			CandType = 0;
			nLangID = GBL_Hindi;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_BENGALI__
		case INPUT_MODE_SMART_BENGALI:
			CandType = 0;
			nLangID = GBL_Bengali;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_ARABIC__
		case INPUT_MODE_SMART_ARABIC:
			CandType = 0;
			nLangID = GBL_Arabic;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_GERMAN__
		case INPUT_MODE_SMART_UPPERCASE_GERMAN:
			CandType = 1;
			nLangID = GBL_German;
			nInputMode = GBIM_Predictive;
			break;
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
			CandType = 0;
			nLangID = GBL_German;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_FRENCH__
		case INPUT_MODE_SMART_UPPERCASE_FRENCH:
			CandType = 1;
			nLangID = GBL_French;
			nInputMode = GBIM_Predictive;
			break;
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
			CandType = 0;
			nLangID = GBL_French;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_ITALIAN__
		case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
			CandType = 1;
			nLangID = GBL_Italian;
			nInputMode = GBIM_Predictive;
			break;
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
			CandType = 0;
			nLangID = GBL_Italian;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_MALAY__
		case INPUT_MODE_SMART_UPPERCASE_MALAY:
			CandType = 1;
			nLangID = GBL_Malaysian;
			nInputMode = GBIM_Predictive;
			break;
		case INPUT_MODE_SMART_LOWERCASE_MALAY:
			CandType = 0;
			nLangID = GBL_Malaysian;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_TAMIL__
		case INPUT_MODE_SMART_TAMIL:
			CandType = 0;
			nLangID = GBL_Tamil;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_SPANISH__
		case INPUT_MODE_SMART_UPPERCASE_SPANISH:
			CandType = 1;
			nLangID = GBL_Spanish;
			nInputMode = GBIM_Predictive;
			break;
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:
			CandType = 0;
			nLangID = GBL_Spanish;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_INDONESIAN__
		case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
			CandType = 1;
			nLangID = GBL_Indonesian;
			nInputMode = GBIM_Predictive;
			break;
		case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
			CandType = 0;
			nLangID = GBL_Indonesian;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_TELUGU__
		case INPUT_MODE_SMART_TELUGU:
			CandType = 0;
			nLangID = GBL_Telugu;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_VIETNAMESE__
		case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
			CandType = 1;
			nLangID = GBL_Vietnamese;
			nInputMode = GBIM_Predictive;
			break;
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
			CandType = 0;
			nLangID = GBL_Vietnamese;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_THAI__
		case INPUT_MODE_SMART_THAI:
			CandType = 0;
			nLangID = GBL_Thai;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_PERSIAN__
		case INPUT_MODE_SMART_PERSIAN:
			CandType = 0;
			nLangID = GBL_Farsi;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_GB_SMART_PORTUGUESE__
		case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE:
			CandType = 1;
			nLangID = GBL_Portuguese;
			nInputMode = GBIM_Predictive;
			break;
		case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
			CandType = 0;
			nLangID = GBL_Portuguese;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_RUSSIAN__
		case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
			CandType = 1;
			nLangID = GBL_Russian;
			nInputMode = GBIM_Predictive;
			break;
		case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
			CandType = 0;
			nLangID = GBL_Russian;
			nInputMode = GBIM_Predictive;
			break;
	#endif
	#ifdef __MMI_SMART_TURKISH__
		case INPUT_MODE_SMART_UPPERCASE_TURKISH:
			CandType = 1;
			nLangID = GBL_Turkish;
			nInputMode = GBIM_Predictive;
			break;
		case INPUT_MODE_SMART_LOWERCASE_TURKISH:
			CandType = 0;
			nLangID = GBL_Turkish;
			nInputMode = GBIM_Predictive;
			break;
	#endif
		default:
			//here means this input method need add
			MMI_ASSERT(0);
			break;
	}
	GbIMESetEngineCurrentLang(nLangID, nInputMode, CandType);
#else
	EBByte oldmod = ebis.inputMethod;
	EBLong res;
	TBM_ENTRY(0x2882);

	mmi_trace(g_sw_IM,"inputMode = %d",inputMode);
	switch (inputMode)
	{
	#if defined(__MMI_GB_SM_CHINESE__)
		case INPUT_MODE_SM_PINYIN:
			ebis.candidateDesireLen = GB_CH_CANDIDATE_COUNT;
			ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
			ebis.inputMethod = EB_INPUT_METHOD_PINYIN;
			GBIMEmethod = INPUT_MODE_SM_PINYIN;
			break;
		case INPUT_MODE_SM_STROKE:
			ebis.candidateDesireLen = GB_CH_CANDIDATE_COUNT;
			ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
			ebis.inputMethod = EB_INPUT_METHOD_STROKE;
			GBIMEmethod = INPUT_MODE_SM_STROKE;
			break;
	#endif

	#if defined(__MMI_GB_TR_CHINESE__)
		case INPUT_MODE_TR_BOPOMO:
			ebis.candidateDesireLen = GB_CH_CANDIDATE_COUNT;
			ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
			ebis.inputMethod = EB_INPUT_METHOD_BPMF;
			GBIMEmethod = INPUT_MODE_TR_BOPOMO;
			break;
	#endif

	#if  defined(__MMI_SMART_ARABIC__)
		case INPUT_MODE_SMART_ARABIC:
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_SMART_ARABIC;
			break; 
	#endif
	#if defined(__MMI_SMART_PERSIAN__)
		case INPUT_MODE_SMART_PERSIAN:
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_SMART_PERSIAN;
			break;
	#endif
	#if defined(__MMI_SMART_THAI__)
		case INPUT_MODE_SMART_THAI:
			enis.option = EN_OPTION_OUTPUT_NORMAL;
			GBIMEmethod = INPUT_MODE_SMART_THAI;
			break;
	#endif

	#if defined(__MMI_SMART_VIETNAMESE__)
		case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_SMART_UPPERCASE_VIETNAMESE;
			break;
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
			enis.option = EN_OPTION_OUTPUT_LOWER;
			GBIMEmethod = INPUT_MODE_SMART_LOWERCASE_VIETNAMESE;
			break;
	#endif

	#if  defined(__MMI_MULTITAP_FRENCH__)
		case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_MULTITAP_UPPERCASE_FRENCH;
			break;

		case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
			enis.option = EN_OPTION_OUTPUT_LOWER;
			GBIMEmethod = INPUT_MODE_MULTITAP_LOWERCASE_FRENCH;
			break;
	#endif
	#if defined(__MMI_SMART_FRENCH__)
		case INPUT_MODE_SMART_UPPERCASE_FRENCH:
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_SMART_UPPERCASE_FRENCH;
			break;

		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
			enis.option = EN_OPTION_OUTPUT_LOWER;
			GBIMEmethod = INPUT_MODE_SMART_LOWERCASE_FRENCH;
			break; 
	#endif
	#if defined(__MMI_SMART_TURKISH__)
		case INPUT_MODE_SMART_UPPERCASE_TURKISH:
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_SMART_UPPERCASE_TURKISH;
			break;

		case INPUT_MODE_SMART_LOWERCASE_TURKISH:
			enis.option = EN_OPTION_OUTPUT_LOWER;
			GBIMEmethod = INPUT_MODE_SMART_LOWERCASE_TURKISH;
			break;
	#endif
	#if defined(__MMI_SMART_INDONESIAN__)
		case INPUT_MODE_SMART_UPPERCASE_INDONESIAN:
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_SMART_UPPERCASE_INDONESIAN;
			break;

		case INPUT_MODE_SMART_LOWERCASE_INDONESIAN:
			enis.option = EN_OPTION_OUTPUT_LOWER;
			GBIMEmethod = INPUT_MODE_SMART_LOWERCASE_INDONESIAN;
			break;
	#endif
	#if defined(__MMI_SMART_MALAY__)
		case INPUT_MODE_SMART_UPPERCASE_MALAY:
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_SMART_UPPERCASE_MALAY;
			break;

		case INPUT_MODE_SMART_LOWERCASE_MALAY:
			enis.option = EN_OPTION_OUTPUT_LOWER;
			GBIMEmethod = INPUT_MODE_SMART_LOWERCASE_MALAY;
			break;
	#endif
	#if defined(__MMI_SMART_ITALIAN__)
		case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_SMART_UPPERCASE_ITALIAN;
			break;

		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
			enis.option = EN_OPTION_OUTPUT_LOWER;
			GBIMEmethod = INPUT_MODE_SMART_LOWERCASE_ITALIAN;
			break; 
	#endif
	#if defined(__MMI_SMART_GERMAN__)				
		case INPUT_MODE_SMART_UPPERCASE_GERMAN:
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_SMART_UPPERCASE_GERMAN;
			break;

		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
			enis.option = EN_OPTION_OUTPUT_LOWER;
			GBIMEmethod = INPUT_MODE_SMART_LOWERCASE_GERMAN;
			break; 
	#endif
	#if defined(__MMI_SMART_SPANISH__)
		case INPUT_MODE_SMART_UPPERCASE_SPANISH:
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_SMART_UPPERCASE_SPANISH;
			break;

		case INPUT_MODE_SMART_LOWERCASE_SPANISH:	
			enis.option = EN_OPTION_OUTPUT_LOWER;
			GBIMEmethod = INPUT_MODE_SMART_LOWERCASE_SPANISH;
			break;
	#endif
	#if  defined(__MMI_SMART_HINDI__)
		case INPUT_MODE_SMART_HINDI:
			enis.option = EN_OPTION_OUTPUT_NORMAL;
			GBIMEmethod = INPUT_MODE_SMART_HINDI;
			break; 
	#endif
	#if  defined(__MMI_SMART_TELUGU__)
		case INPUT_MODE_SMART_TELUGU:
			enis.option = EN_OPTION_OUTPUT_NORMAL;
			GBIMEmethod = INPUT_MODE_SMART_TELUGU;
			break; 
	#endif 
		case INPUT_MODE_SMART_LOWERCASE_ABC:
			GBIMEmethod = INPUT_MODE_SMART_LOWERCASE_ABC;
			enis.option = EN_OPTION_OUTPUT_LOWER;
			break;
		case INPUT_MODE_SMART_UPPERCASE_ABC:
			GBIMEmethod = INPUT_MODE_SMART_UPPERCASE_ABC;
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			break;
		case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:	
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_SMART_UPPERCASE_RUSSIAN;
			break;
		case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
			enis.option = EN_OPTION_OUTPUT_LOWER;
			GBIMEmethod = INPUT_MODE_SMART_LOWERCASE_RUSSIAN;
			break;
	#if defined(__MMI_GB_SMART_PORTUGUESE__)
		case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE :
			enis.option = EN_OPTION_OUTPUT_CAPITAL;
			GBIMEmethod = INPUT_MODE_SMART_UPPERCASE_PORTUGUESE;
			break;
		case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
			enis.option = EN_OPTION_OUTPUT_LOWER;
			GBIMEmethod = INPUT_MODE_SMART_LOWERCASE_PORTUGUESE;
			break;
	#endif
		default:
			break;
	}
	if (oldmod == 0)
		res = EBInputMethodInit(&ebis);
	else
		res = EBSwitchInputMethod(&ebis, ebis.inputMethod);

	TBM_EXIT(0x2882);
	mmi_trace(g_sw_IM, "GBIMEmethod = %d, result = %d", GBIMEmethod, res);
#endif
}

void GbInputSetInputBox(GBINT nLangID)
{
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
    {

    	coolsand_UI_create_single_line_input_box_use_buffer(&gcj_native_input_box,
														8,
														GB_COMPOSITION_Y - MMI_BUTTON_BAR_HEIGHT,
														UI_device_width-17,
														MMI_BUTTON_BAR_HEIGHT,
														gcj_InputBuff,
														(( GB_COMPOSITION_BUFFER_SIZE + 1) * sizeof(S16)));

    	coolsand_UI_create_single_line_input_box_use_buffer(&gbCompositionArea,
														8,
														GB_COMPOSITION_Y,
														UI_device_width-17,
														MMI_BUTTON_BAR_HEIGHT,
														gbCompositionBuffer,
														(( GB_COMPOSITION_BUFFER_SIZE + 1) * sizeof(S16)));

    	coolsand_UI_create_single_line_input_box_use_buffer(&gbCandidateArea,
														8,
														GB_COMPOSITION_Y+MMI_BUTTON_BAR_HEIGHT,
														UI_device_width-17,
														MMI_BUTTON_BAR_HEIGHT,
														gbCandidateBuffer,
														(( GB_SELECTION_BUFFER_SIZE + 1) * sizeof(S16)));
    }
    else
	{
#endif	
	coolsand_UI_create_single_line_input_box_use_buffer(&gbCompositionArea,
														GB_COMPOSITION_X,
														GB_COMPOSITION_Y,
														MMI_multitap_width -18,
														MMI_multitap_height,
														gbCompositionBuffer,
														(( GB_COMPOSITION_BUFFER_SIZE + 1) * sizeof(S16)));

	coolsand_UI_create_single_line_input_box_use_buffer(&gbCandidateArea,
														GB_CANDIDATE_X,
														GB_CANDIDATE_Y,
														MMI_multitap_width - 18,
														MMI_multitap_height,
														gbCandidateBuffer,
														(( GB_SELECTION_BUFFER_SIZE + 1) * sizeof(S16)));
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	}
#endif
	gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW;
	gbCompositionArea.cursor_color.r = 255;
	gbCompositionArea.cursor_color.g = 255;
	gbCompositionArea.cursor_color.b = 255;
	gbCompositionArea.cursor_color.alpha = 100;

	gbCompositionArea.normal_filler = &MMI_GB_COMPOSITION_WINDOW_filler;
	gbCompositionArea.selection_color = multitap_selector_txt_col_Default;
	gbCompositionArea.selection_text_color= multitap_selected_txt_col_Default;


	gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_SCROLLING;
	gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_VIEW_FULL_CHARACTERS;
	gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_CENTER_JUSTIFY;

	gbCandidateArea.cursor_color.r = 255;
	gbCandidateArea.cursor_color.g = 255;
	gbCandidateArea.cursor_color.b = 255;
	gbCandidateArea.cursor_color.alpha = 100;

	gbCandidateArea.normal_filler = &MMI_GB_COMPOSITION_WINDOW_filler;//(current_UI_theme->multitap_input_theme->f);
	gbCandidateArea.selection_color = multitap_selector_txt_col_Default;
	gbCandidateArea.selection_text_color = multitap_selected_txt_col_Default;
	//gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_CHARACTER_HIGHLIGHT;
}

void InitGbInputMethod(void)
{
	int ret;
	TBM_ENTRY(0x2884);
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_enDataArray);
	mmi_trace(g_sw_IM, "EnStartup() return ret = %d", ret);
	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = 20;
	enis.candidateDesireCount = 5;
	
	#ifdef __MMI_GB_SM_CHINESE__
	ret = EBStartup(prv_dataArray);
	mmi_trace(g_sw_IM, "EBStartup: ret=%d",ret );
	memset(&ebis, 0, sizeof(ebis));
	//设置候选字的个数
	ebis.candidateDesireLen = GB_CH_CANDIDATE_COUNT;
	//设置候选标点的个数
	ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
	#endif
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;

	TBM_EXIT(0x2884);
}
#if defined(__MMI_SMART_URDU__)
void InitGbInputMethod_Urdu(void)
{
	TBM_ENTRY(0x2884);
	newEngine();
	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	
	TBM_EXIT(0x2884);
}
#endif

#if defined(__MMI_SMART_PUNJABI__)
void InitGbInputMethod_Punjabi(void)
{
	newEngine();

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
}
#endif

#if defined(__MMI_SMART_BENGALI__)
void InitGbInputMethod_Bengali(void)
{
	newEngine();
	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	
}
#endif

#if defined(__MMI_GB_TR_CHINESE__)
void InitGbTRInputMethod(void)
{
    int ret;
    TBM_ENTRY(0x2884);
    ret = EBStartup(prv_trdataArray);
    mmi_trace(g_sw_IM, TSTXT("InitGbTRInputMethod: ret=%d.\n"),ret );

    memset(&enis, 0, sizeof(enis));

    enis.candidateDesireLen = GB_CANDIDATE_LEN;
    enis.candidateDesireCount = GB_CANDIDATE_COUNT;
	
	
    coolsand_UI_create_single_line_input_box_use_buffer(&gbCompositionArea,
										    GB_COMPOSITION_X/*MMI_multitap_x*/,
										    GB_COMPOSITION_Y,
										    MMI_multitap_width -18,
										    MMI_multitap_height,
										    gbCompositionBuffer,
											(( GB_COMPOSITION_BUFFER_SIZE + 1) * sizeof(EBWord)));

    coolsand_UI_create_single_line_input_box_use_buffer(&gbCandidateArea,
										    GB_CANDIDATE_X,
										    GB_CANDIDATE_Y,
										    MMI_multitap_width - 18,
										    MMI_multitap_height,
										    gbCandidateBuffer,
											(( GB_SELECTION_BUFFER_SIZE + 1) * sizeof(EBWord)));
    gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW;
    gbCompositionArea.cursor_color.r = 255;
    gbCompositionArea.cursor_color.g = 255;
    gbCompositionArea.cursor_color.b = 255;
    gbCompositionArea.cursor_color.alpha = 100;

    gbCompositionArea.normal_filler = &MMI_GB_COMPOSITION_WINDOW_filler;
    gbCompositionArea.selection_color = multitap_selector_txt_col_Default;
    gbCompositionArea.selection_text_color= multitap_selected_txt_col_Default;


    gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_SCROLLING;
    gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_VIEW_FULL_CHARACTERS;
    gbCandidateArea.cursor_color.r = 255;
    gbCandidateArea.cursor_color.g = 255;
    gbCandidateArea.cursor_color.b = 255;
    gbCandidateArea.cursor_color.alpha = 100;

    gbCandidateArea.normal_filler = &MMI_GB_COMPOSITION_WINDOW_filler;
    gbCandidateArea.selection_color = multitap_selector_txt_col_Default;
    gbCandidateArea.selection_text_color = multitap_selected_txt_col_Default;
	//gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_CHARACTER_HIGHLIGHT;

    gbstate = GBIMESTATE_IDLE;
    idxCandidate = 0;
	//gbEMSEditorHeight=0;
	//gbMultilineEditorHeight=0;
    TBM_EXIT(0x2884);
}
			
#endif

#if defined(__MMI_GB_SM_CHINESE__)
void InitGbSMInputMethod(void)
{
	int ret;
	TBM_ENTRY(0x2884);
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EBStartup(prv_dataArray);
	mmi_trace(g_sw_IM, TSTXT("GbIMEprocessKey: ret=%d.\n"),ret );
	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;

	TBM_EXIT(0x2884);
}
#endif

#if defined(__MMI_SMART_TAMIL__)
void InitGbInputMethod_Tamil(void)
{
	int ret;
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_tamilDataArray);
	mmi_trace(g_sw_IM, "InitGbInputMethod_Tamil ret is %d",ret);
	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;

}
#endif

#if defined(__MMI_SMART_SPANISH__)
void InitGbInputMethod_Spa()
{
	int ret;
	TBM_ENTRY(0x2884);
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_spaDataArray);
	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	TBM_EXIT(0x2884);
}
#endif


#if defined(__MMI_SMART_GERMAN__)
void InitGbInputMethod_Ger()
{
	int ret;
	TBM_ENTRY(0x2884);
	
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_gerDataArray);

	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	
	TBM_EXIT(0x2884);
}
#endif

#if defined(__MMI_SMART_HINDI__)
void InitGbInputMethod_Hin(void)
{
	int ret;
	TBM_ENTRY(0x2884);

#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_hiDataArray);
	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	TBM_EXIT(0x2884);
}
#endif
//ending--guoyt added for adding spanish,German and Hindi IME.

#if defined(__MMI_SMART_TELUGU__)
void InitGbInputMethod_Tel(void)
{
	int ret;
	TBM_ENTRY(0x2884);
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_teDataArray);

	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif
	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	TBM_EXIT(0x2884);
}
#endif
/*JINZH Add Start For 英语-法语-阿拉伯切换 Ver: 多国语言  on 2007-12-21 9:42 */
#if defined(__MMI_SMART_ARABIC__)
void InitGbInputMethod_Ara(void)
{
	int ret;
	TBM_ENTRY(0x2884);
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_enArabicDataArray);
	mmi_trace(g_sw_IM, "EnStartup() for arabic return ret = %d", ret);
	
	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	TBM_EXIT(0x2884);
}
#endif

#if defined(__MMI_SMART_PERSIAN__)
void InitGbInputMethod_Per(void)
{
#ifdef __MMI_GB_V5__
	newEngine();
#else
	int ret;
	ret = EnStartup(prv_enPersianDataArray);
	mmi_trace(g_sw_IM, "EnStartup() for Persian ret = %d", ret);
	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
}
#endif

#if defined(__MMI_MULTITAP_FRENCH__)||defined(__MMI_SMART_FRENCH__)
void InitGbInputMethod_fr(void)
{
	int ret;
	TBM_ENTRY(0x2884);
	
	//初始化法文引擎
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_frDataArray);
	mmi_trace(g_sw_IM, "InitGbInputMethod_fr: ret=%d", ret );
	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	
	TBM_EXIT(0x2884);
}
#endif
#if defined(__MMI_SMART_MALAY__)
void InitGbInputMethod_malay(void)
{
	int ret;

	//初始化马来西亚语引擎
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_malayDataArray);
	mmi_trace(g_sw_IM, "InitGbInputMethod_malay: ret=%d",ret );
	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif
	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
}
#endif
#if defined(__MMI_SMART_TURKISH__)
void InitGbInputMethod_turkish(void)
{
	int ret;

	//初始化土耳其语引擎
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_turkishDataArray);
	mmi_trace(g_sw_IM, "InitGbInputMethod_turkish: ret=%d",ret );
	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
}
#endif
#if defined(__MMI_SMART_INDONESIAN__)
void InitGbInputMethod_indonesian(void)
{
	int ret;

	//初始化印尼语引擎
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_indonesianDataArray);
	mmi_trace(g_sw_IM, "InitGbInputMethod_indonesian: ret=%d",ret );

	memset(&enis, 0, sizeof(enis));

	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif	
	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
}
#endif
// beginning--added by kecx for adding italian on 20081223
#if defined(__MMI_MULTITAP_ITALIAN__)||defined(__MMI_SMART_ITALIAN__)
void InitGbInputMethod_ita(void)
{
	int ret;
	TBM_ENTRY(0x2884);

	//初始化意大利文引擎
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_italianDataArray);
	mmi_trace(g_sw_IM, "InitGbInputMethod_ita: ret=%d",ret );
	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	
	TBM_EXIT(0x2884);
}
#endif
// end--added by kecx for adding italian on 20081223
#if defined(__MMI_SMART_RUSSIAN__)
void InitGbInputMethod_Ru(void)
{
	int ret;
	TBM_ENTRY(0x2884);
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_ruDataArray);
	mmi_trace(g_sw_IM, TSTXT("GbIMEprocessKey: ret=%d.\n"),ret );
	memset(&enis, 0, sizeof(enis));
	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	TBM_EXIT(0x2884);
}

/*JINZH Add End  For 英语-法语-阿拉伯切换 Ver: 多国语言  on 2007-12-21 9:42 */
#endif
#if defined(__MMI_SMART_THAI__)
extern const unsigned long prv_thDataArray[];
void InitGbInputMethod_Thai(void)
{
	int ret;
	TBM_ENTRY(0x2884);

#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_thDataArray);
	mmi_trace(g_sw_IM, TSTXT("GbIMEprocessKey: ret=%d.\n"),ret );

	memset(&enis, 0, sizeof(enis));

	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;
#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	TBM_EXIT(0x2884);
}

/*JINZH Add End  For 英语-法语-阿拉伯切换 Ver: 多国语言  on 2007-12-21 9:42 */
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
extern const unsigned long prv_viDataArray[];
void InitGbInputMethod_Vi(void)
{
	int ret;
	TBM_ENTRY(0x2884);

	//初始化越文引擎
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_viDataArray);
	mmi_trace(g_sw_IM, TSTXT("GbIMEprocessKey: ret=%d.\n"),ret );

	memset(&enis, 0, sizeof(enis));

	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;

#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	TBM_EXIT(0x2884);
}
#endif

#if defined(__MMI_GB_SMART_PORTUGUESE__)
void InitGbInputMethod_PT(void)
{
	int ret;
	TBM_ENTRY(0x2884);
#ifdef __MMI_GB_V5__
	newEngine();
#else
	ret = EnStartup(prv_ptDataArray);
	mmi_trace(g_sw_IM, TSTXT("GbIMEprocessKey: ret=%d.\n"),ret );

	memset(&enis, 0, sizeof(enis));

	enis.candidateDesireLen = GB_CANDIDATE_LEN;
	enis.candidateDesireCount = GB_CANDIDATE_COUNT;

#endif

	GbInputSetInputBox(0);

	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	TBM_EXIT(0x2884);
}

/*JINZH Add End  For 英语-法语-阿拉伯切换 Ver: 多国语言  on 2007-12-21 9:42 */
#endif

void GBHideCompositionArea(void)
{
	color_t c;
#ifdef __MMI_DEFAULT_THEME_3__
	c.r = 206;
	c.g = 206;
	c.b = 206;
#else
	c.r = 255;
	c.g = 255;
	c.b = 255;
#endif
	c.alpha = 100;
	coolsand_UI_reset_clip();
#if defined(__MMI_MAINLCD_220X176__)
	gdi_draw_solid_rect(0, gbCompositionArea.y - 5, UI_device_width,gbCompositionArea.y + gbCompositionArea.height + 5, GDI_COLOR_TRANSPARENT);
#else
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
    {
           	coolsand_UI_fill_rectangle(4, gbCompositionArea.y - 5, UI_device_width-5,gbCompositionArea.y + gbCompositionArea.height , c);
    }
    else
#endif	
	coolsand_UI_fill_rectangle(0, gbCompositionArea.y - 5, UI_device_width,gbCompositionArea.y + gbCompositionArea.height + 5, c);
#endif
}

void GBInsertPinyin(void)
{
#ifdef __MMI_GB_V5__
	int i, j = 0, k = 0, t = 0;
	GBPCCHAR pSyllable;
	coolsand_UI_single_line_input_box_delete_all(&gbCompositionArea);

	for (i = 0; i < g_gbis.pAuxInfo->nSyllableNum; i++)
	{
		pSyllable = g_gbis.pAuxInfo->pSyllables[i];
		for(j = 0; j < UCS2Strlen((const S8 *)pSyllable); j++)
		{
			coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, *(pSyllable+j));
		}
		coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, ' ');
	}
	
	coolsand_UI_single_line_input_box_goto_first_character(&gbCompositionArea);//move cursor to first character

	
	pSyllable = g_gbis.pAuxInfo->pSyllables[0];
	for (i = 0; i < g_gbis.pAuxInfo->nSyllableIndex + 1; i++)
	{
		pSyllable = g_gbis.pAuxInfo->pSyllables[i];
		for(j = 0; j < UCS2Strlen((UI_string_type)pSyllable); j++)
		{
			coolsand_UI_single_line_input_box_next(&gbCompositionArea);
		}
		coolsand_UI_single_line_input_box_next(&gbCompositionArea);
	}
	coolsand_UI_single_line_input_box_previous(&gbCompositionArea);
	gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_WORD_HIGHLIGHT;
	gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
	gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_CHARACTER_HIGHLIGHT;
	coolsand_UI_single_line_input_box_highlight_cursor_number(&gbCompositionArea, UCS2Strlen(pSyllable));	
#else
	int i, j = 0, k = 0, t = 0;
	coolsand_UI_single_line_input_box_delete_all(&gbCompositionArea);
	//添加fulloutputstring中的音节和汉字到显示框中
	for (i = 0; i < UCS2Strlen((char *)ebis.fullOutputString); i++)
	{
		coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, ebis.fullOutputString[i]);
	}
	t = UCS2Strlen((char *)ebis.fullOutputString);
	coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, ' ');
	//添加结束

	mmi_trace(g_sw_IM, TSTXT("ebis.syllableLen=%d:.\n"), ebis.syllableLen);


	// 添加每一个音节到显示后选框中，ebis.syllableLen表示候选的音节数。
	for (i = 0; i < ebis.syllableLen; i++)
	{
		while(ebis.syllables[j] != 0)
		{
			coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, ebis.syllables[j++]);
		}
		coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, ' ');
		j++;
	}
	//添加完成所有的音节
	coolsand_UI_single_line_input_box_goto_first_character(&gbCompositionArea);//move cursor to first character


	//移动光标到下一个音节的位置
	if (gbSelectWordFlag == 1)
	{
		for (i = 0; i < t; i++)
		{
			coolsand_UI_single_line_input_box_next(&gbCompositionArea);
		}
	}
	else
	{
		for (i = 0; i < ebis.inputStringLen; i++)
		{
			coolsand_UI_single_line_input_box_next(&gbCompositionArea);
		}
	}
	//移动结束
	j = 0;

	//其值在整个过程是不一致的，随着汉字的匹配而变，表示fullOutputString中的输入
	if (gbSelectWordFlag == 1)
	{
		k = t;
	}
	else
		k = ebis.inputStringLen;

	for (i = 0; i < ebis.syllableIndex; i++)
	{
		k = 0;
		while(ebis.syllables[j] != 0)
		{
			k++;
			coolsand_UI_single_line_input_box_next(&gbCompositionArea);
			j++;
		}
		coolsand_UI_single_line_input_box_next(&gbCompositionArea);
		j++;
	}
	coolsand_UI_single_line_input_box_highlight_cursor_number(&gbCompositionArea, k);
#endif
}
#if defined(__MMI_GB_TR_CHINESE__)
void GBInsertbpmf(void)
{
	UI_character_type bihua = 0;
	int i= 0;
	int j = 0;
	int k = 0, t = 0;
	coolsand_UI_single_line_input_box_delete_all(&gbCompositionArea);
	//添加fulloutputstring中的音节和汉字到显示框中
	for (i = 0; i < UCS2Strlen((char *)ebis.fullOutputString); i++)
	{
		coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, ebis.fullOutputString[i]);
	}
	coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea,' ');

	t = UCS2Strlen((char *)ebis.fullOutputString);
	//添加结束
	mmi_trace(g_sw_IM, TSTXT("ebis.syllableLen=%d:.\n"), ebis.syllableLen);
	// 添加每一个音节到显示后选框中，ebis.syllableLen表示候选的音节数。
	//05 31 00 06 31 00 07 31 00 08 31 00
	for (i = 0; i < 20; i++)
	{
		mmi_trace(g_sw_IM, TSTXT("gbCompositionArea[%d]=0x%x:.\n"), i,ebis.syllables[i]);
	}

	for (i = 0; i < ebis.syllableLen; i++)
	{
		while(ebis.syllables[j] == 0)
		{
			j = j+2;
		}
		while(ebis.syllables[j] != 0)
		{
			bihua = (ebis.syllables[j])|((ebis.syllables[j+1]<<8)&0xff00);
			mmi_trace(g_sw_IM, TSTXT("gbCompositionArea[%d]=0x%x:.\n"), j,bihua);
			coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, bihua);
			j = j+2;
		}
		coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, ' ');
	}

	//添加完成所有的音节
	mmi_trace(g_sw_IM, TSTXT("ebis.syllableIndex goto first:.\n"));
	coolsand_UI_single_line_input_box_goto_first_character(&gbCompositionArea);//move cursor to first character


	//移动光标到下一个音节的位置
	if (gbSelectWordFlag == 1)
	{
		for (i = 0; i < t; i++)
		{
			coolsand_UI_single_line_input_box_next(&gbCompositionArea);
			mmi_trace(g_sw_IM, TSTXT("ebis.syllableIndex goto next1:.\n"));
		}
	}
	else
	{
		for (i = 0; i < ebis.inputStringLen; i++)
		{
			coolsand_UI_single_line_input_box_next(&gbCompositionArea);
			mmi_trace(g_sw_IM, TSTXT("ebis.syllableIndex goto next2:.\n"));
		}
	}
	//其值在整个过程是不一致的，随着汉字的匹配而变，表示fullOutputString中的输入
	if (gbSelectWordFlag == 1)
	{
		k = t;
	}
	else
		k = ebis.inputStringLen;

	mmi_trace(g_sw_IM, TSTXT("ebis.syllableIndex %d:.\n"), ebis.syllableIndex);

	for (i = 0; i < ebis.syllableIndex; i++)
	{
		//移动结束
		j = 0;
		k = 0;
		while(ebis.syllables[j] == 0)
		{
			j=j+2;
		}
		while(ebis.syllables[j] != 0)
		{
			coolsand_UI_single_line_input_box_next(&gbCompositionArea);
			k++;
			j=j+2;
		}
		coolsand_UI_single_line_input_box_next(&gbCompositionArea);
	}
	coolsand_UI_single_line_input_box_highlight_cursor_number(&gbCompositionArea, k);
}

void GBDrawbpmf(void)
{
	GBInsertbpmf();
	gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_WORD_HIGHLIGHT;
	gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
	gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW;

#ifdef __GB_COMPOSITION_AREA_WITH_ARROW__
	coolsand_UI_reset_clip();
	coolsand_UI_lock_double_buffer();
	if(ebis.syllableIndex <= 0)
	{	
		draw_resizeable_arrow(gbCompositionArea.x - GB_ARROW_WIDTH - 2, gbCompositionArea.y, GB_LEFT_ARROW, coolsand_UI_color(192,192,192),GB_ARROW_WIDTH,gbCompositionArea.height);
	}
	else
	{
		draw_resizeable_arrow(gbCompositionArea.x - GB_ARROW_WIDTH - 2, gbCompositionArea.y, GB_LEFT_ARROW, coolsand_UI_color(0,0,0),GB_ARROW_WIDTH,gbCompositionArea.height);
	}
	if(ebis.syllableIndex >= ebis.syllableLen)
	{
		draw_resizeable_arrow(gbCompositionArea.x + gbCompositionArea.width + 1, gbCompositionArea.y, GB_RIGHT_ARROW, coolsand_UI_color(192,192,192),GB_ARROW_WIDTH,gbCompositionArea.height);
	}
	else
	{
		draw_resizeable_arrow(gbCompositionArea.x + gbCompositionArea.width + 1, gbCompositionArea.y, GB_RIGHT_ARROW, coolsand_UI_color(0,0,0),GB_ARROW_WIDTH,gbCompositionArea.height);
	}
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width -1,UI_device_height -1);
#endif
	coolsand_UI_show_single_line_input_box(&gbCompositionArea);
}
#endif

void GBDrawPinyin(void)
{
	GBInsertPinyin();
	gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_WORD_HIGHLIGHT;
	gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
	gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW;
#ifdef __GB_COMPOSITION_AREA_WITH_ARROW__
	coolsand_UI_reset_clip();
	coolsand_UI_lock_double_buffer();
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
    {
	if(GbIMEGetSyllableIndex() <= 0)
	{	
		draw_resizeable_arrow(gbCompositionArea.x - 3 - 2, gbCompositionArea.y, GB_LEFT_ARROW, coolsand_UI_color(192,192,192),3,gbCompositionArea.height);
	}
	else
	{
		draw_resizeable_arrow(gbCompositionArea.x - 3 - 2, gbCompositionArea.y, GB_LEFT_ARROW, coolsand_UI_color(0,0,0),3,gbCompositionArea.height);
	}
	if(GbIMEGetSyllableIndex >= GbIMEGetSyllableNumber())
	{
		draw_resizeable_arrow(gbCompositionArea.x + gbCompositionArea.width + 1, gbCompositionArea.y, GB_RIGHT_ARROW, coolsand_UI_color(192,192,192),3,gbCompositionArea.height);
	}
	else
	{
		draw_resizeable_arrow(gbCompositionArea.x + gbCompositionArea.width + 1, gbCompositionArea.y, GB_RIGHT_ARROW, coolsand_UI_color(0,0,0),3,gbCompositionArea.height);
	}

	}
    else
    	{
#endif	
	if(GbIMEGetSyllableIndex() <= 0)
	{	
		draw_resizeable_arrow(gbCompositionArea.x - 3 - 2, gbCompositionArea.y, GB_LEFT_ARROW, coolsand_UI_color(192,192,192),GB_ARROW_WIDTH,gbCompositionArea.height);
	}
	else
	{
		draw_resizeable_arrow(gbCompositionArea.x - GB_ARROW_WIDTH - 2, gbCompositionArea.y, GB_LEFT_ARROW, coolsand_UI_color(0,0,0),GB_ARROW_WIDTH,gbCompositionArea.height);
	}
	if(GbIMEGetSyllableIndex >= GbIMEGetSyllableNumber())
	{
		draw_resizeable_arrow(gbCompositionArea.x + gbCompositionArea.width + 1, gbCompositionArea.y, GB_RIGHT_ARROW, coolsand_UI_color(192,192,192),GB_ARROW_WIDTH,gbCompositionArea.height);
	}
	else
	{
		draw_resizeable_arrow(gbCompositionArea.x + gbCompositionArea.width + 1, gbCompositionArea.y, GB_RIGHT_ARROW, coolsand_UI_color(0,0,0),GB_ARROW_WIDTH,gbCompositionArea.height);
	}
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	}
#endif	
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width -1,UI_device_height -1);
#endif
	coolsand_UI_show_single_line_input_box(&gbCompositionArea);
}

void GBInsertChinese(void)
{
	int i, j, k;
	U8 count;
	#ifdef __MMI_GB_V5__
	GBPCCHAR pCandidate;
	#endif
	
	TBM_ENTRY(0x2886);
	k = 0;
	
	coolsand_UI_single_line_input_box_delete_all(&gbCandidateArea);
	count = GbIMEGetCandidateCount();
	for (i = 0; i <count; i++)
	{
	#ifdef __MMI_GB_V5__
		pCandidate = g_gbis.outputInfo.pCandidates[i];
		for(j = 0; j < UCS2Strlen(pCandidate); j++)
		{
			coolsand_UI_single_line_input_box_insert_character(&gbCandidateArea,*(pCandidate+j));
		}
	#else
		for (j = 0; j < ebis.cis[i].len; j++)
		{
			coolsand_UI_single_line_input_box_insert_character(&gbCandidateArea, ebis.candidates[k++]);
		}
	#endif
	}
	coolsand_UI_single_line_input_box_goto_first_character(&gbCandidateArea);
	#ifdef __MMI_GB_V5__
	pCandidate = g_gbis.outputInfo.pCandidates[0];
	#endif
	for (i = 0; i < idxCandidate + 1; i++)
	{
	#ifdef __MMI_GB_V5__
		pCandidate = g_gbis.outputInfo.pCandidates[i];
		for(j = 0; j < UCS2Strlen(pCandidate);j ++)
		{
			coolsand_UI_single_line_input_box_next(&gbCandidateArea);
		}
	#else
		for (j = 0; j < ebis.cis[i].len; j++)
		{
			coolsand_UI_single_line_input_box_next(&gbCandidateArea);
		}
	#endif
	}
	#ifdef __MMI_GB_V5__
	coolsand_UI_single_line_input_box_highlight_cursor_number(&gbCandidateArea, UCS2Strlen(pCandidate));
	#else
	coolsand_UI_single_line_input_box_highlight_cursor_number(&gbCandidateArea, ebis.cis[idxCandidate].len);
	#endif
	TBM_EXIT(0x2886);
}

void GBDrawChinese(void)
{
	GBInsertChinese();
	gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_WORD_HIGHLIGHT;
	gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
	gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_CENTER_JUSTIFY;
	gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW;
#ifdef __GB_CANDIDATE_AREA_WITH_ARROW__
	coolsand_UI_reset_clip();
	coolsand_UI_lock_double_buffer();
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
    {
        UI_fill_rectangle(4,gbCandidateArea.y,UI_device_width-5,gbCandidateArea.y+gbCandidateArea.height-1,coolsand_UI_color(255,255,255));
	if((idxCandidate<=0)&&(!GbIMECanPageUp()))
	{
		draw_resizeable_arrow(gbCandidateArea.x - 3, gbCandidateArea.y, GB_LEFT_ARROW, coolsand_UI_color(192,192,192),3,gbCandidateArea.height);
	}
	else
	{
		draw_resizeable_arrow(gbCandidateArea.x - 3, gbCandidateArea.y, GB_LEFT_ARROW, coolsand_UI_color(0,0,0),3,gbCandidateArea.height);
	}
	/*Fix Bug#12588 for cs2324c by cong.li on 2009.06.23. 
	idxCandidate from 0 to ebis.candidateLen-1.*/
	if((idxCandidate >= (GbIMEGetCandidateCount()-1))&&(!GbIMECanPageDown()))
	{
		draw_resizeable_arrow(gbCandidateArea.x + gbCandidateArea.width + 1, gbCandidateArea.y, GB_RIGHT_ARROW, coolsand_UI_color(192,192,192),3,gbCandidateArea.height);
	}
	else
	{
		draw_resizeable_arrow(gbCandidateArea.x + gbCandidateArea.width + 1, gbCandidateArea.y, GB_RIGHT_ARROW, coolsand_UI_color(0,0,0),3,gbCandidateArea.height);
	}

	}
    else
    	{
#endif	

	UI_fill_rectangle(0,gbCandidateArea.y,UI_device_width-1,gbCandidateArea.y+gbCandidateArea.height-1,coolsand_UI_color(255,255,255));

	if((idxCandidate<=0)&&(!GbIMECanPageUp()))
	{
		draw_resizeable_arrow(gbCandidateArea.x - GB_ARROW_WIDTH, gbCandidateArea.y, GB_LEFT_ARROW, coolsand_UI_color(192,192,192),GB_ARROW_WIDTH,gbCandidateArea.height);
	}
	else
	{
		draw_resizeable_arrow(gbCandidateArea.x - GB_ARROW_WIDTH, gbCandidateArea.y, GB_LEFT_ARROW, coolsand_UI_color(0,0,0),GB_ARROW_WIDTH,gbCandidateArea.height);
	}
	/*Fix Bug#12588 for cs2324c by cong.li on 2009.06.23. 
	idxCandidate from 0 to ebis.candidateLen-1.*/
	if((idxCandidate >= (GbIMEGetCandidateCount()-1))&&(!GbIMECanPageDown()))
	{
		draw_resizeable_arrow(gbCandidateArea.x + gbCandidateArea.width + 1, gbCandidateArea.y, GB_RIGHT_ARROW, coolsand_UI_color(192,192,192),GB_ARROW_WIDTH,gbCandidateArea.height);
	}
	else
	{
		draw_resizeable_arrow(gbCandidateArea.x + gbCandidateArea.width + 1, gbCandidateArea.y, GB_RIGHT_ARROW, coolsand_UI_color(0,0,0),GB_ARROW_WIDTH,gbCandidateArea.height);
	}
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	}
#endif	
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width -1,UI_device_height -1);
#endif
	coolsand_UI_show_single_line_input_box(&gbCandidateArea);
}
/**************************************************
Function Name: GbIMERevmoeCopyPortectChar
In: Null
Out: Null
Description: there is a char(0x3e) at first in GB V5 for copy protect,
		   we don't want show this, so remove it
**************************************************/
void GbIMERevmoeCopyProtectChar(void)
{
#ifdef __MMI_GB_V5__
	U8 i;
	U8 count;
	GBPCCHAR pCandidate;
	count = GbIMEGetCandidateCount();
	pCandidate = g_gbis.outputInfo.pCandidates[0];
	if(*pCandidate == 0x3e && UCS2Strlen((UI_string_type)pCandidate) == 1)
	{
		for(i = 0; i < count - 1; i++)
		{
			g_gbis.outputInfo.pCandidates[i] = g_gbis.outputInfo.pCandidates[i+1];
		}
		g_gbis.outputInfo.nCandNum--;
	}
#endif
}
void GBDrawEnglish(void)
{
	U8 i,j, len;
	U8 count;
	coolsand_UI_single_line_input_box_delete_all(&gbCandidateArea);
	GbIMERevmoeCopyProtectChar();
	count = GbIMEGetCandidateCount();
	for (i = 0; i < count; i++)
	{
	#ifdef __MMI_GB_V5__
		GBPCCHAR pCandidate = g_gbis.outputInfo.pCandidates[i];
		for(j = 0; j < UCS2Strlen((UI_string_type)pCandidate); j++)
		{
			coolsand_UI_single_line_input_box_insert_character(&gbCandidateArea, *(pCandidate+j));
		}
	#else
		len = UCS2Strlen((S8*)enis.candidates[i]);
		for (j = 0;j < len; j++)
		{
			coolsand_UI_single_line_input_box_insert_character(&gbCandidateArea, enis.candidates[i][j]);
		}
	#endif
		coolsand_UI_single_line_input_box_insert_character(&gbCandidateArea, ' ');
	}
	coolsand_UI_single_line_input_box_goto_first_character(&gbCandidateArea);
	for (i = 0; i < idxCandidate + 1; i++)
	{
	#ifdef __MMI_GB_V5__
		GBPCCHAR pCandidate = g_gbis.outputInfo.pCandidates[i];
		for(j = 0; j < UCS2Strlen((UI_string_type)pCandidate); j++)
		{
			coolsand_UI_single_line_input_box_next(&gbCandidateArea);
		}
	#else
		for (j = 0; j < UCS2Strlen((S8*)enis.candidates[i]); j++)
		{
			coolsand_UI_single_line_input_box_next(&gbCandidateArea);
		}
	#endif
		coolsand_UI_single_line_input_box_next(&gbCandidateArea);
	}
	coolsand_UI_single_line_input_box_previous(&gbCandidateArea);
#ifdef __MMI_GB_V5__
	coolsand_UI_single_line_input_box_highlight_cursor_number(&gbCandidateArea, UCS2Strlen((S8*)g_gbis.outputInfo.pCandidates[idxCandidate]));
#else
	coolsand_UI_single_line_input_box_highlight_cursor_number(&gbCandidateArea, UCS2Strlen((S8*)enis.candidates[idxCandidate]));
#endif

	gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_WORD_HIGHLIGHT;
	gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
	gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW;

	/*zhoumn Add Start For 6612 Ver: TBM  on 2007-9-7 14:39 */
#ifdef __GB_CANDIDATE_AREA_WITH_ARROW__
	coolsand_UI_reset_clip();
	coolsand_UI_lock_double_buffer();

	UI_fill_rectangle(0,gbCandidateArea.y,UI_device_width-1,gbCandidateArea.y+gbCandidateArea.height-1,coolsand_UI_color(255,255,255));

#if defined(__MMI_SMART_ARABIC__)
	if(MMI_current_input_mode == INPUT_MODE_SMART_ARABIC) //add by panxu for the candidates transform
	{
		if((idxCandidate<=0)&& (!GbIMECanPageUp()))
		{
			draw_resizeable_arrow(gbCandidateArea.x + gbCandidateArea.width + 1, gbCandidateArea.y, GB_RIGHT_ARROW, coolsand_UI_color(192,192,192),GB_ARROW_WIDTH,gbCandidateArea.height);
		}
		else
		{
			draw_resizeable_arrow(gbCandidateArea.x + gbCandidateArea.width + 1, gbCandidateArea.y, GB_RIGHT_ARROW, coolsand_UI_color(0,0,0),GB_ARROW_WIDTH,gbCandidateArea.height);
		}

		if((idxCandidate >= (GbIMEGetCandidateCount() - 1))&&(!GbIMECanPageDown()))
		{
			draw_resizeable_arrow(gbCandidateArea.x - GB_ARROW_WIDTH, gbCandidateArea.y, GB_LEFT_ARROW, coolsand_UI_color(192,192,192),GB_ARROW_WIDTH,gbCandidateArea.height);
		}
		else
		{
			draw_resizeable_arrow(gbCandidateArea.x - GB_ARROW_WIDTH, gbCandidateArea.y, GB_LEFT_ARROW, coolsand_UI_color(0,0,0),GB_ARROW_WIDTH,gbCandidateArea.height);
		}
	}
	else
#endif
#if defined(__MMI_SMART_PERSIAN__)
	if(MMI_current_input_mode == INPUT_MODE_SMART_PERSIAN)
	{
		if((idxCandidate<=0)&&(!GbIMECanPageUp()))
		{
			draw_resizeable_arrow(gbCandidateArea.x + gbCandidateArea.width + 1, gbCandidateArea.y, GB_RIGHT_ARROW, coolsand_UI_color(192,192,192),GB_ARROW_WIDTH,gbCandidateArea.height);
		}
		else
		{
			draw_resizeable_arrow(gbCandidateArea.x + gbCandidateArea.width + 1, gbCandidateArea.y, GB_RIGHT_ARROW, coolsand_UI_color(0,0,0),GB_ARROW_WIDTH,gbCandidateArea.height);
		}

		if((idxCandidate >= (GbIMEGetCandidateCount() - 1))&&(!GbIMECanPageDown()))
		{
			draw_resizeable_arrow(gbCandidateArea.x - GB_ARROW_WIDTH, gbCandidateArea.y, GB_LEFT_ARROW, coolsand_UI_color(192,192,192),GB_ARROW_WIDTH,gbCandidateArea.height);
		}
		else
		{
			draw_resizeable_arrow(gbCandidateArea.x - GB_ARROW_WIDTH, gbCandidateArea.y, GB_LEFT_ARROW, coolsand_UI_color(0,0,0),GB_ARROW_WIDTH,gbCandidateArea.height);
		}
	}
	else
#endif
	{
		if((idxCandidate<=0)&&(!GbIMECanPageUp()))
		{
			draw_resizeable_arrow(gbCandidateArea.x - GB_ARROW_WIDTH-3, gbCandidateArea.y, GB_LEFT_ARROW, coolsand_UI_color(192,192,192),GB_ARROW_WIDTH,gbCandidateArea.height);
		}
		else
		{
			draw_resizeable_arrow(gbCandidateArea.x - GB_ARROW_WIDTH-3, gbCandidateArea.y, GB_LEFT_ARROW, coolsand_UI_color(0,0,0),GB_ARROW_WIDTH,gbCandidateArea.height);
		}

		if((idxCandidate >= (GbIMEGetCandidateCount()- 1))&&(!GbIMECanPageDown()))
		{
			draw_resizeable_arrow(gbCandidateArea.x + gbCandidateArea.width +1, gbCandidateArea.y, GB_RIGHT_ARROW, coolsand_UI_color(192,192,192),GB_ARROW_WIDTH,gbCandidateArea.height);
		}
		else
		{
			draw_resizeable_arrow(gbCandidateArea.x + gbCandidateArea.width +1, gbCandidateArea.y, GB_RIGHT_ARROW, coolsand_UI_color(0,0,0),GB_ARROW_WIDTH,gbCandidateArea.height);
		}
	}
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width -1,UI_device_height -1);
#endif
	/*zhoumn Add End  For 6612 Ver: TBM  on 2007-9-7 14:39 */

	coolsand_UI_show_single_line_input_box(&gbCandidateArea);
}

void GBDrawEnglishInputNumber(void)
{
	U8 i, k = 0;
	U8 count;
	coolsand_UI_single_line_input_box_delete_all(&gbCompositionArea);

	count = GbIMEGetInputStrLen();
	if(count != 0)
	{
		for (i = 0; i < count; i++)
		{
		#ifdef __MMI_GB_V5__
			coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, g_gbis.inputString[i]);
		#else
			coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, enis.inputString[i]);
		#endif
		}
		
		coolsand_UI_single_line_input_box_highlight_cursor_number(&gbCompositionArea, k);
		gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_WORD_HIGHLIGHT;
		gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
		coolsand_UI_show_single_line_input_box(&gbCompositionArea);
	}

}

U16 TransStroke(U16 stroke)
{
	switch(stroke)
	{
		case 0x4e5b:
			return 0xF005;
			break;

		case 0xe836:
			return 0xf12a;
			break;

		case 0xe822:
			return 0xf07a;
			break;

		default:
			return stroke;
			break;
	}
}

void GBInsertStroke(void)
{
#ifdef __MMI_GB_V5__
	int i;

	coolsand_UI_single_line_input_box_delete_all(&gbCompositionArea);

	if(NULL != g_gbis.pAuxInfo->pOutputString)
	{
		i = 0;
		while(*(g_gbis.pAuxInfo->pOutputString+i))
		{
			coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, *(g_gbis.pAuxInfo->pOutputString+i));
			i++;
		}
	}
	
	i = 0;
	while(g_gbis.pAuxInfo->wStroke[i])
	{
		coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, TransStroke(g_gbis.pAuxInfo->wStroke[i]));
		i++;
	}
	
	coolsand_UI_single_line_input_box_goto_first_character(&gbCompositionArea);
	coolsand_UI_single_line_input_box_highlight_cursor_number(&gbCompositionArea, i);
#else
	int i;
	static EBWord BiHua[128];

	coolsand_UI_single_line_input_box_delete_all(&gbCompositionArea);
	EBStringToStroke(&ebis, (char *)BiHua, sizeof(BiHua));
	for (i = 0; i < ebis.inputStringLen; i++)
	{
	#ifdef __STROKE_24568__
		coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, ConvertGuobiKeyToMmiKey(ebis.inputString[i] - '0'));
	#else
		coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, ebis.inputString[i]);
	#endif
	}
	//coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, ' ') ;
	i = 0;
	while(BiHua[i])
	{
		coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, TransStroke(BiHua[i]));
		i++;
	}
#if 0//由于我们暂时在笔划输入中不支持部首输入的提示,暂时屏蔽这段代码,需要时打开即可
	for(i=0;i<ebis.componentLen; i++)
	{
		if(i==4)
		{
			coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, '0') ;
		}
		else
		{
			coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, '6'+i) ;
		}
		coolsand_UI_single_line_input_box_insert_character(&gbCompositionArea, TransStroke(ebis.components[i])) ;
	}
#endif
	coolsand_UI_single_line_input_box_goto_first_character(&gbCompositionArea);
	coolsand_UI_single_line_input_box_highlight_cursor_number(&gbCompositionArea, i);
#endif
}

void GBDrawStroke(void)
{
	GBInsertStroke();
	gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_WORD_HIGHLIGHT;
	gbCompositionArea.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
	coolsand_UI_show_single_line_input_box(&gbCompositionArea);
}


void GbInputSetRSK(void)
{
	if (GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG)
	{
		mmi_trace(g_sw_IM, "GbInputSetRSK fro category28");
		if (!coolsand_UI_EMS_inputbox_empty(&MMI_EMS_inputbox))
		{
			mmi_trace(g_sw_IM, "GbInputSetRSK not empty");
			set_right_softkey_label((UI_string_type)get_string(STR_GLOBAL_CLEAR));
			set_right_softkey_icon(NULL);
			redraw_right_softkey();
			wgui_set_EMS_inputbox_RSK(); //add by yaosq 20061218
			category28_RSK_label_clear = 1;
			set_right_softkey_function(UI_dummy_function, KEY_EVENT_UP);
			set_right_softkey_function(GbKeyRSKPressHandler, KEY_EVENT_DOWN);
		}
		else
		{
			mmi_trace(g_sw_IM, "GbInputSetRSK empty");
			set_right_softkey_label((UI_string_type)get_string(STR_GLOBAL_BACK));
			set_right_softkey_icon(NULL);
			redraw_right_softkey();
			wgui_set_EMS_inputbox_RSK();
			set_right_softkey_function(UI_dummy_function, KEY_EVENT_UP);//add by yaosq 20070123
			set_right_softkey_function(register_category28_handle_right_softkey_up, KEY_EVENT_DOWN);//open by yaosq 20070123
		}
	#ifdef __MMI_FULL_KEYPAD__
		SetKeyHandler(UI_dummy_function, KEY_BACKSPACE1, KEY_EVENT_UP);
		SetKeyHandler(GbKeyRSKPressHandler, KEY_BACKSPACE1, KEY_EVENT_DOWN);
	#endif    	
	}
	else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG))
	{
		if (coolsand_UI_multi_line_input_box_get_text_length(&MMI_multiline_inputbox) > 0)
		{
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
  			  {
           			wgui_inputbox_RSK_label_clear = 1;
				SetKeyHandler(UI_dummy_function, KEY_RSK,KEY_EVENT_UP);
				SetKeyHandler(GbKeyRSKPressHandler,KEY_RSK, KEY_EVENT_DOWN);

 			   }
 			   else
			  {
#endif
	
				set_right_softkey_label((UI_string_type)get_string(STR_GLOBAL_CLEAR));
				set_right_softkey_icon(NULL);
				redraw_right_softkey();
				wgui_set_RSK();//add by yaosq 20070105
		
				wgui_inputbox_RSK_label_clear = 1;
				set_right_softkey_function(UI_dummy_function, KEY_EVENT_UP);
				set_right_softkey_function(GbKeyRSKPressHandler, KEY_EVENT_DOWN);
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			}
#endif
		}
		else
		{
			wgui_inputbox_RSK_label_clear = 0;
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
			{
	
			}
			else
			{
#endif
				set_right_softkey_label((UI_string_type)get_string(STR_GLOBAL_BACK));
				set_right_softkey_icon(NULL);
				redraw_right_softkey();
				//add by chenq for bug 6708 200809191652 B
				set_right_softkey_function(wgui_handle_inputbox_right_softkey_long_press,KEY_LONG_PRESS);
				//add by chenq for bug 6708 200809191652 E
				set_right_softkey_function(wgui_register_inputbox_handle_right_softkey_up, KEY_EVENT_DOWN);  
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__			
			}
#endif		
		}
	#ifdef __MMI_FULL_KEYPAD__
		SetKeyHandler(UI_dummy_function, KEY_BACKSPACE1, KEY_EVENT_UP);
		SetKeyHandler(GbKeyRSKPressHandler, KEY_BACKSPACE1, KEY_EVENT_DOWN);
	#endif    	
	}
}

/**************************************************************

    FUNCTION NAME    	:GbInputBoxScreenStateChange

	  PURPOSE    			: 改变输入状态
    INPUT PARAMETERS    : nil

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/
//wangzl:add B
#ifdef __MMI_TOUCH_SCREEN__
extern void mmi_pen_editor_change_handwriting_area_gb(void);
extern mmi_pen_editor_virtual_keyboard_state_enum gPenEditorVKState;
#endif
//wangzl:add E
extern BOOL bUseLSKAsSendKey2;
void GbInputBoxScreenStateChange(void)
{
	U8 IsGBIMEStateAssationFirst = 0;
	TBM_ENTRY(0x2883);
	switch (gbstate)
	{
		case GBIMESTATE_IDLE:
			if (GbIMEHasInput())
				gbstate = GBIMESTATE_INPUT;
			break;

		case GBIMESTATE_INPUT:
			if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
			{
				if (!GbIMEHasInput() && !GbIMEHasAssociate()) 
					gbstate = GBIMESTATE_IDLE;
				if (!GbIMEHasInput() && GbIMEHasAssociate()) 
				{
					gbstate = GBIMESTATE_ASSATION;
					IsGBIMEStateAssationFirst = 1;
				}
			}
			else
			{
				if (!GbIMEHasInput())
					gbstate = GBIMESTATE_IDLE;
			}
			break;

		case GBIMESTATE_ASSATION:
			if (GbIMEHasInput())   
				gbstate = GBIMESTATE_INPUT;
			if (!GbIMEHasInput() && !GbIMEHasAssociate())  
				gbstate = GBIMESTATE_IDLE;
			
			break;

		default:
			break;
	}

	switch(gbstate)
	{
		case GBIMESTATE_IDLE:
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
		       {
		            GoBackJavaEditer();
					return;
		       }
		       else
		       {
#endif	
				if(gb_LSK_string_hold)
				{
					set_left_softkey_label((UI_string_type)(gb_LSK_string_hold));
				}
				else
				{
					set_left_softkey_label((UI_string_type)get_string(STR_GLOBAL_OPTIONS));
				}
				set_left_softkey_icon(NULL);
				redraw_left_softkey();
				if (gb_inputbox_LSK_function != NULL)
					set_left_softkey_function(gb_inputbox_LSK_function, KEY_EVENT_UP);
				GbInputSetRSK();
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			}
#endif			
		#ifdef __MMI_TOUCH_SCREEN__
			gPenEditorVKState = PEN_EDITOR_VK_ON;
		#endif
			if(gb_inputbox_ENTER_function != NULL)
			{
				SetKeyHandler(gb_inputbox_ENTER_function, KEY_ENTER, KEY_EVENT_UP);
				gb_inputbox_ENTER_function = NULL;
			#ifdef __MMI_USE_LSK_AS_SEND2__
				if (GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG)
					bUseLSKAsSendKey2  = TRUE;
			#endif
			}
			break;

		case GBIMESTATE_INPUT:
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
			{
			    
			    U8* act_layer_buf;
			    U8  lsk_str[]={0x09,0x90,0xe9,0x62,0x00,0x00};
			    U8  rsk_str[]={0x05,0x6e,0x64,0x96,0x00,0x00};
				LCDUIsetSoftButton(0,lsk_str,pfnUnicodeStrlen(lsk_str));
				LCDUIsetSoftButton(1,0,0);
				LCDUIsetSoftButton(2,rsk_str,pfnUnicodeStrlen(rsk_str));
			    gdi_layer_get_buffer_ptr(&act_layer_buf);
		           gcj_DrawCommandBar(act_layer_buf);
			//	gdi_layer_blt_previous(0, UI_device_height-MMI_BUTTON_BAR_HEIGHT, 
			//		UI_device_width-1, UI_device_height-1);
			    SetKeyHandler(UI_dummy_function ,  KEY_LSK,  KEY_EVENT_DOWN);
			    SetKeyHandler(GbKeyLSKPressHandler ,  KEY_LSK,  KEY_EVENT_UP);
				
			    SetKeyHandler(UI_dummy_function ,  KEY_RSK,  KEY_EVENT_DOWN);	
			    SetKeyHandler(GbKeyRSKPressHandler ,  KEY_RSK,  KEY_EVENT_UP);	
				
			}
			else
			{			
#endif		
				set_left_softkey_label((UI_string_type)get_string(STR_GLOBAL_SELECT));
				set_left_softkey_icon(NULL);
				redraw_left_softkey();
				set_left_softkey_function(GbKeyLSKPressHandler, KEY_EVENT_UP);
				set_right_softkey_label((UI_string_type)get_string(STR_GLOBAL_CLEAR));
				category28_RSK_label_clear = 1;
				set_right_softkey_icon(NULL);
				redraw_right_softkey();
				set_right_softkey_function(GbKeyRSKPressHandler, KEY_EVENT_DOWN);
				set_right_softkey_function(UI_dummy_function, KEY_EVENT_UP);
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			}
#endif
		#ifdef __MMI_TOUCH_SCREEN__
			gPenEditorVKState = PEN_EDITOR_VK_OFF;
		#endif
			if(NULL == gb_inputbox_ENTER_function)
			{
				gb_inputbox_ENTER_function = GetKeyHandler(KEY_ENTER, KEY_EVENT_UP);
				if(gb_inputbox_ENTER_function != NULL)
				{
				#ifdef __MMI_USE_LSK_AS_SEND2__
					bUseLSKAsSendKey2  = FALSE;
				#endif
					ClearKeyHandler(KEY_ENTER, KEY_EVENT_UP);
				}
			}
			break;

		case GBIMESTATE_ASSATION:
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
			{
			    
			    U8* act_layer_buf;
			    U8  lsk_str[]={0x09,0x90,0xe9,0x62,0x00,0x00};
			    U8  rsk_str[]={0x93,0x8f,0x65,0x51,0x00,0x00};
				LCDUIsetSoftButton(0,lsk_str,pfnUnicodeStrlen(lsk_str));
				LCDUIsetSoftButton(1,0,0);
				LCDUIsetSoftButton(2,rsk_str,pfnUnicodeStrlen(rsk_str));
			    gdi_layer_get_buffer_ptr(&act_layer_buf);
		           gcj_DrawCommandBar(act_layer_buf);
			//	gdi_layer_blt_previous(0, UI_device_height-MMI_BUTTON_BAR_HEIGHT, 
			//		UI_device_width-1, UI_device_height-1);
			    SetKeyHandler(UI_dummy_function ,  KEY_LSK,  KEY_EVENT_DOWN);
			    SetKeyHandler(GbKeyLSKPressHandler ,  KEY_LSK,  KEY_EVENT_UP);
				
			    SetKeyHandler(UI_dummy_function ,  KEY_RSK,  KEY_EVENT_DOWN);	
			    SetKeyHandler(GbKeyRSKPressHandler ,  KEY_RSK,  KEY_EVENT_UP);	
				
			}
			else
			{			
#endif		
			set_left_softkey_function(GbKeyLSKPressHandler,KEY_EVENT_UP);
			if (IsGBIMEStateAssationFirst == 1)
			{
				set_right_softkey_label((UI_string_type)get_string(STR_GLOBAL_BACK));
				set_right_softkey_icon(NULL);
				category28_RSK_label_clear = 0;
				redraw_right_softkey();
				IsGBIMEStateAssationFirst = 0;			
			}
			SetRightSoftkeyFunction(GbKeyRSKPressHandler, KEY_EVENT_DOWN);
			set_right_softkey_function(UI_dummy_function, KEY_EVENT_UP);
		#ifdef __MMI_TOUCH_SCREEN__
			gPenEditorVKState = PEN_EDITOR_VK_OFF;
		#endif
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
		}
#endif		
			break;
	}
	//wangzl:add B
#ifdef __MMI_HAND_WRITING__
	mmi_pen_editor_change_handwriting_area_gb();
#endif
	//wangzl:add E
	TBM_EXIT(0x2883);
}

#ifdef __MMI_TOUCH_SCREEN__
#include "gui_virtual_keyboard.h"
extern virtual_keyboard MMI_virtual_keyboard;
extern void mmi_pen_editor_clear_and_show_virtual_keyboard_area(void);
#endif
void GbDisplayForEmsEditor(void)
{
	EBLong h;
	TBM_ENTRY(0x2881);

	coolsand_UI_lock_double_buffer();

#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
	{
		Gb_Ime_hide_input_area();
	}
	else
	{
#endif			
		GBHideCompositionArea();
		category5_hide_multitap();
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	}
#endif			

	if (MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX)
	{
		GbInputMethodCategoryFlag = GB_IN_CATEGORY_28_FLAG;
	}
	else
	{
		GbInputMethodCategoryFlag = GB_IN_CATEGORY_5_FLAG;
	}

	TRACE_EF(g_sw_IM, TSTXT("GbIMEprocessKey: GbInputMethodCategoryFlag=%d gbstate=%d.\n"),GbInputMethodCategoryFlag,gbstate );
	gbEMSEditorHeight = GbEmsEditorHeightInIdle;
	gbMultilineEditorHeight = GbMultilineEditorHeightInIdle;
	switch(gbstate)
	{
		case GBIMESTATE_IDLE:
		{
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
			{
					Gb_Ime_show_input_box();
			}
			else
			{
#endif			
				if (GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG)
				{
				#if defined(__MMI_MAINLCD_220X176__)
					h = MMI_CONTENT_HEIGHT -INFORMATION_BAR_HEIGHT  ;  //gbEMSEditorHeight;
				#else
					h = MMI_content_height -wgui_inputbox_information_bar_height  ;  //gbEMSEditorHeight;
				#endif

				#ifdef __MMI_TOUCH_SCREEN__
					if (MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY != MMI_virtual_keyboard.lang_type)
						h -= (MMI_virtual_keyboard.height+1);
				#endif
					coolsand_UI_resize_EMS_input_box(&MMI_EMS_inputbox, MMI_EMS_inputbox.width, h);
					MMI_EMS_inputbox.flags|=(UI_EMS_INPUT_BOX_DISABLE_DRAW);
					MMI_multiline_inputbox.flags|=(UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW) ;
					coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox);
					MMI_multiline_inputbox.flags&=~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
					MMI_EMS_inputbox.flags&=~(UI_EMS_INPUT_BOX_DISABLE_DRAW);
					coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox);
				}
				else if (GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG)
				{
				#if defined(__MMI_MAINLCD_220X176__)
					h = MMI_CONTENT_HEIGHT -INFORMATION_BAR_HEIGHT  ;//gbMultilineEditorHeight;
				#else
					h = MMI_content_height -wgui_inputbox_information_bar_height  ;//gbMultilineEditorHeight;
				#endif

				#ifdef __MMI_TOUCH_SCREEN__
					if (MMI_GUI_VIRTUAL_KEYBOARD_EMPTY_TRAY != MMI_virtual_keyboard.lang_type)
						h -= (MMI_virtual_keyboard.height+1);
				#endif
					resize_multiline_inputbox(MMI_multiline_inputbox.width, h);
					MMI_multiline_inputbox.flags|=(UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW) ;
					coolsand_UI_show_multi_line_input_box(&MMI_multiline_inputbox);
					MMI_multiline_inputbox.flags&=~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
					coolsand_UI_show_multi_line_input_box(&MMI_multiline_inputbox);
				}
			#ifdef __MMI_TOUCH_SCREEN__
				mmi_pen_editor_clear_and_show_virtual_keyboard_area();
			#endif
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			}
#endif			
			break;
		}

		case GBIMESTATE_INPUT:
		{
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
			{
					Gb_Ime_show_input_box();
			}
			else
			{
#endif			
				if (GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG)
				{
				#ifdef __MMI_TOUCH_SCREEN__
					#if defined(__MMI_MAINLCD_220X176__)
					h = gbEMSEditorHeight - MMI_multitap_height*2 - 8;
					#else
					h = gbEMSEditorHeight - MMI_multitap_height - 1;
					#endif
				#else
					h = gbEMSEditorHeight - MMI_multitap_height ;
				#endif
					coolsand_UI_resize_EMS_input_box(&MMI_EMS_inputbox, MMI_EMS_inputbox.width, h);
					MMI_EMS_inputbox.flags|=(UI_EMS_INPUT_BOX_DISABLE_DRAW);
					coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox);
					MMI_EMS_inputbox.flags&=~(UI_EMS_INPUT_BOX_DISABLE_DRAW);
					coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox);
				}
				else if (GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG)
				{
				#ifdef __MMI_TOUCH_SCREEN__
					h = gbEMSEditorHeight - MMI_multitap_height - 1;
				#else
					h = gbEMSEditorHeight - MMI_multitap_height;
				#endif
					resize_multiline_inputbox(MMI_multiline_inputbox.width, h);
					MMI_multiline_inputbox.flags|=UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
					coolsand_UI_show_multi_line_input_box(&MMI_multiline_inputbox);
					MMI_multiline_inputbox.flags&=~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
					coolsand_UI_show_multi_line_input_box(&MMI_multiline_inputbox);
				}
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__				
			}
#endif
			switch (GBIMEmethod)
			{
				case INPUT_MODE_SM_PINYIN:
					GBDrawPinyin();
					GBDrawChinese();
					break;
			#if defined(__MMI_GB_TR_CHINESE__)
				case INPUT_MODE_TR_BOPOMO:
					GBDrawbpmf();
					GBDrawChinese();
					break;
			#endif
				case INPUT_MODE_SM_STROKE:
					GBDrawStroke();
					GBDrawChinese();
					break;
#if defined(__MMI_SMART_ARABIC__)
				case INPUT_MODE_SMART_ARABIC:
#endif

#if defined(__MMI_SMART_PERSIAN__)
				case INPUT_MODE_SMART_PERSIAN:
#endif

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

#if defined(__MMI_SMART_TELUGU__)
				case INPUT_MODE_SMART_TELUGU:			
#endif
#if defined(__MMI_SMART_THAI__)
				case INPUT_MODE_SMART_THAI:
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
				case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
				case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
#endif
#if defined(__MMI_MULTITAP_FRENCH__)
				case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
				case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
#endif

#if defined(__MMI_SMART_FRENCH__)
				case INPUT_MODE_SMART_UPPERCASE_FRENCH:
				case INPUT_MODE_SMART_LOWERCASE_FRENCH:
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

#if defined(__MMI_SMART_ITALIAN__)
				case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
				case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
#endif

#if defined(__MMI_SMART_RUSSIAN__)
				case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
				case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:	
#endif
#if defined(__MMI_GB_SMART_PORTUGUESE__)
				case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE :
				case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
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
				case INPUT_MODE_SMART_UPPERCASE_ABC:
				case INPUT_MODE_SMART_LOWERCASE_ABC:
					GBDrawEnglishInputNumber();
					GBDrawEnglish();
					break;
			}
			break;
		}

		case GBIMESTATE_ASSATION:
		{
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
			{
					Gb_Ime_show_input_box();
			}
			else
			{
#endif			
				if (GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG)
				{
				#ifdef __MMI_TOUCH_SCREEN__
					h = MMI_CONTENT_HEIGHT -INFORMATION_BAR_HEIGHT - 30;
				#else
					h = gbEMSEditorHeight;
				#endif
					coolsand_UI_resize_EMS_input_box(&MMI_EMS_inputbox, MMI_EMS_inputbox.width, h);
					MMI_EMS_inputbox.flags|=(UI_EMS_INPUT_BOX_DISABLE_DRAW);
					//coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox); //为了解决连续按左软键选字上屏而死机的问题
					MMI_EMS_inputbox.flags&=~(UI_EMS_INPUT_BOX_DISABLE_DRAW);
					coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox);
				}
				else if (GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG)
				{
				#ifdef __MMI_TOUCH_SCREEN__
					h = MMI_CONTENT_HEIGHT -INFORMATION_BAR_HEIGHT - 30;
				#else
					h = gbEMSEditorHeight;
				#endif
					resize_multiline_inputbox(MMI_multiline_inputbox.width, h);
					MMI_multiline_inputbox.flags|=UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
					coolsand_UI_show_multi_line_input_box(&MMI_multiline_inputbox);
					MMI_multiline_inputbox.flags&=~UI_MULTI_LINE_INPUT_BOX_DISABLE_DRAW;
					coolsand_UI_show_multi_line_input_box(&MMI_multiline_inputbox);
				}
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__				
			}
#endif			
			switch (GBIMEmethod)
			{
				case INPUT_MODE_SM_PINYIN:
				case INPUT_MODE_TR_BOPOMO:
					//GBDrawPinyin() ;
					GBDrawChinese();
					break;
				case INPUT_MODE_SM_STROKE:
					//GBDrawStroke();
					GBDrawChinese();
					break;
			}
			break;
		}
	}
	coolsand_UI_unlock_double_buffer();
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
	{
		coolsand_UI_BLT_double_buffer(0,
		0,
		UI_device_width-1,
		UI_device_height-1);// - MMI_title_height - MMI_button_bar_height );		
	}
	else
	{
#endif			
	
		coolsand_UI_BLT_double_buffer(MMI_multiline_inputbox_x,
		MMI_multiline_inputbox_y,
		UI_device_width,
		UI_device_height);// - MMI_title_height - MMI_button_bar_height );	
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__				
	}
#endif	
	TBM_EXIT(0x2881);
}

/**************************************************************

    FUNCTION NAME    	: GBClearSixToNineKEY

	  PURPOSE    			: 清除0键和6-9键
    INPUT PARAMETERS    	: void

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/
void GBClearSixToNineKEY( void )
{
	U16 keyCode = 0;
	U16 keyType = 0;
	 
	for (keyCode = KEY_6; keyCode <= KEY_9; keyCode++)
	{
		for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
		{
			ClearKeyHandler(KEY_0, keyType);
			ClearKeyHandler(keyCode, keyType);
		}
	}
}

/**************************************************************

    FUNCTION NAME    	: GbIMEprocessCharInput

	  PURPOSE    			: 接收按键处理
    INPUT PARAMETERS    : inChar

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/
#ifdef __MMI_TOUCH_SCREEN__
extern BOOL mmi_pen_in_handwriting_process(void);
extern void mmi_pen_editor_set_candidate_state(BOOL state);
extern void  mmi_pen_editor_reset_vk_inputs(void);
#endif
void GbIMEprocessCharInput(U8 inChar)
{
	U16 indexValue = 0;
	TBM_ENTRY(0x2880);

	mmi_trace(g_sw_IM, "MMI_IME: GbIMEprocessCharInput, inChar = %c, GBIMEmethod=%d,gbstate=%d.",inChar, GBIMEmethod,gbstate );
#ifdef __MMI_TOUCH_SCREEN__
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(!(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer()))
	{
#endif

		//no handle keys in handwriting process
		if(mmi_pen_in_handwriting_process())
			return ;
		//wangzl:add B
		gdi_layer_lock_frame_buffer();
		mmi_pen_editor_do_confirm();
		mmi_pen_editor_set_candidate_state(MMI_FALSE);
		mmi_pen_editor_reset_vk_inputs();
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	}
#endif
#else
#endif
	//wangzl:add E
	mmi_trace(g_sw_IM, "MMI_IME: GbIMEprocessCharInput,GBIMEmethod = %d", GBIMEmethod);

	if (
#if defined(__MMI_SMART_ARABIC__)
		GBIMEmethod == INPUT_MODE_SMART_ARABIC || 
#endif
#if defined(__MMI_SMART_PERSIAN__)
		GBIMEmethod == INPUT_MODE_SMART_PERSIAN ||
#endif
#if defined(__MMI_SMART_THAI__)
		GBIMEmethod == INPUT_MODE_SMART_THAI ||
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_VIETNAMESE||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_VIETNAMESE||
#endif
#if defined(__MMI_MULTITAP_FRENCH__)
		GBIMEmethod == INPUT_MODE_MULTITAP_UPPERCASE_FRENCH  || 
		GBIMEmethod == INPUT_MODE_MULTITAP_LOWERCASE_FRENCH || 
#endif
#if defined(__MMI_SMART_FRENCH__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_FRENCH ||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_FRENCH ||
#endif
#if defined(__MMI_SMART_TURKISH__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_TURKISH ||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_TURKISH ||
#endif
#if defined(__MMI_SMART_INDONESIAN__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_INDONESIAN||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_INDONESIAN||
#endif
#if defined(__MMI_SMART_MALAY__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_MALAY||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_MALAY||
#endif
#if defined(__MMI_SMART_ITALIAN__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_ITALIAN||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_ITALIAN||
#endif
#if defined(__MMI_SMART_HINDI__)
		GBIMEmethod == INPUT_MODE_SMART_HINDI||	
#endif
#if defined(__MMI_SMART_TELUGU__)
		GBIMEmethod == INPUT_MODE_SMART_TELUGU||
#endif
#if defined(__MMI_SMART_GERMAN__)								
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_GERMAN ||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_GERMAN ||
#endif
#if defined(__MMI_SMART_SPANISH__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_SPANISH||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_SPANISH||
#endif
#if defined(__MMI_SMART_RUSSIAN__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_RUSSIAN ||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_RUSSIAN||
#endif
#if defined(__MMI_GB_SMART_PORTUGUESE__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_PORTUGUESE ||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_PORTUGUESE||
#endif
#if defined(__MMI_SMART_URDU__)
		GBIMEmethod == INPUT_MODE_SMART_URDU||
#endif
#if defined(__MMI_SMART_PUNJABI__)
		GBIMEmethod == INPUT_MODE_SMART_PUNJABI||
#endif
#if defined(__MMI_SMART_BENGALI__)
		GBIMEmethod == INPUT_MODE_SMART_BENGALI||
#endif
#if defined(__MMI_SMART_TAMIL__)
		GBIMEmethod == INPUT_MODE_SMART_TAMIL||
#endif
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_ABC || 
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_ABC
	)
	{
		SmartOption = 0 ;
		if (!GbIMEHasInput())
		{
			GbIMESetCandidateInfo(GB_CANDIDATE_LEN, GB_CANDIDATE_COUNT);
		}
		else
		{
			GbIMESetCandidateInfo(18, 1);
		}
	}

	switch(GBIMEmethod)
	{
		case INPUT_MODE_SM_PINYIN:
		case INPUT_MODE_TR_BOPOMO:
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
	{
		#ifndef __MMI_GB_V5__
		ebis.syllableIndex = 0;
		ebis.associateISNCount = 0;
		#endif
				
		idxCandidate = 0;
		gbstate = GBIMESTATE_INPUT;

		if (GbIMEAddInputChar(inChar) == EB_OK)
		{
			if (!GbIMEIsInvalidInput())
			{
				GbIMEDeleteInputChar();
				#ifndef __MMI_GB_V5__
				EBFindFirstPageC(&ebis);
				#endif
			}
		}
	}
	else
	{
#endif		
			if (((MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX) && (MMI_EMS_inputbox.data->textLength < MMI_EMS_inputbox.available_length * 2))
				|| ((MMI_current_input_box_type == EDITOR_MULTILINE_INPUT_BOX) && (MMI_multiline_inputbox.text_length < MMI_multiline_inputbox.available_length)))
			{
				#ifndef __MMI_GB_V5__
				ebis.syllableIndex = 0;
				ebis.associateISNCount = 0;
				#endif
				
				idxCandidate = 0;
				gbstate = GBIMESTATE_INPUT;

				if (GbIMEAddInputChar(inChar) == EB_OK)
				{
					if (!GbIMEIsInvalidInput())
					{
						GbIMEDeleteInputChar();
						#ifndef __MMI_GB_V5__
						EBFindFirstPageC(&ebis);
						#endif
					}
				}
			}
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
		}
#endif					
			break;
		case INPUT_MODE_SM_STROKE:
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
	{
	        #ifndef __MMI_GB_V5__
				ebis.syllableIndex = 0;
				ebis.associateISNCount = 0;
				#endif
				idxCandidate = 0;
				gbstate = GBIMESTATE_INPUT;
			#ifdef __STROKE_24568__
				if (GbIMEAddInputChar(ConvertMMIKeyToGuobiKey(inChar - '0')+ '0') == EB_OK)
			#else
				if (GbIMEAddInputChar(inChar) == EB_OK)
			#endif
				{
					if(!GbIMEIsInvalidInput())
					{
						GbIMEDeleteInputChar();
						#ifndef __MMI_GB_V5__
						EBFindFirstPageC(&ebis);
						#endif
					}
				}
	}
	else
	{
#endif		
		
			if (((MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX) && (MMI_EMS_inputbox.data->textLength < MMI_EMS_inputbox.available_length * 2))
			|| ((MMI_current_input_box_type == EDITOR_MULTILINE_INPUT_BOX) && (MMI_multiline_inputbox.text_length < MMI_multiline_inputbox.available_length)))
			{
				#ifndef __MMI_GB_V5__
				ebis.syllableIndex = 0;
				ebis.associateISNCount = 0;
				#endif
				idxCandidate = 0;
				gbstate = GBIMESTATE_INPUT;
			#ifdef __STROKE_24568__
				if (GbIMEAddInputChar(ConvertMMIKeyToGuobiKey(inChar - '0')+ '0') == EB_OK)
			#else
				if (GbIMEAddInputChar(inChar) == EB_OK)
			#endif
				{
					if(!GbIMEIsInvalidInput())
					{
						GbIMEDeleteInputChar();
						#ifndef __MMI_GB_V5__
						EBFindFirstPageC(&ebis);
						#endif
					}
				}
			}
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	}
#endif		
			
			break;
	#if defined(__MMI_SMART_ARABIC__)	
		case INPUT_MODE_SMART_ARABIC:
	#endif
	#if defined(__MMI_SMART_PERSIAN__)
		case INPUT_MODE_SMART_PERSIAN:
	#endif
	#if defined(__MMI_SMART_THAI__)
		case INPUT_MODE_SMART_THAI:
	#endif
	#if defined(__MMI_SMART_VIETNAMESE__)
		case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
	#endif
	#if defined(__MMI_MULTITAP_FRENCH__)
		case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
		case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
	#endif
	#if defined(__MMI_SMART_FRENCH__)
		case INPUT_MODE_SMART_UPPERCASE_FRENCH:
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
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
	#if defined(__MMI_SMART_ITALIAN__)
		case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
	#endif
	#if defined(__MMI_SMART_SPANISH__)
		case INPUT_MODE_SMART_UPPERCASE_SPANISH:
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:	
	#endif
	#if defined(__MMI_SMART_GERMAN__)					
		case INPUT_MODE_SMART_UPPERCASE_GERMAN:
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:	
	#endif
	#if defined(__MMI_SMART_HINDI__)				
		case INPUT_MODE_SMART_HINDI:
	#endif
	#if defined(__MMI_SMART_TELUGU__)
		case INPUT_MODE_SMART_TELUGU:
	#endif
	#if defined(__MMI_SMART_RUSSIAN__)
		case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
		case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:	
	#endif
	#if defined(__MMI_GB_SMART_PORTUGUESE__)
		case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE :
		case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
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
		case INPUT_MODE_SMART_UPPERCASE_ABC:
		case INPUT_MODE_SMART_LOWERCASE_ABC:
			if (((MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX) && (MMI_EMS_inputbox.data->textLength < MMI_EMS_inputbox.available_length * 2))
				|| ((MMI_current_input_box_type == EDITOR_MULTILINE_INPUT_BOX) && (MMI_multiline_inputbox.text_length < MMI_multiline_inputbox.available_length)))
			{
				GbIMEAddInputChar(inChar);
				
			#ifdef __MMI_GB_V5__
			
			#else
				if (GbIMEIsInvalidInput())
				{
					TRACE_EF(g_sw_IM, "EnFirstHeader(&enis) == EBEN_OK");
					EnFirstHeader(&enis);
					EnFirstPage(&enis);
				}
				else
				{
					TRACE_EF(g_sw_IM, "EnFirstHeader(&enis) != EBEN_OK");
					GbIMEDeleteInputChar();
					EnFirstHeader(&enis);
					EnFirstPage(&enis);
				}
			#endif
			
				idxCandidate = 0;
				PRINT_ENIS();
				gbstate = GBIMESTATE_INPUT;
			}
			break;
	}
	GbInputBoxScreenStateChange();
	PRINT_ENIS();
	GbDisplayForEmsEditor();
#ifdef __MMI_TOUCH_SCREEN__
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(!(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer()))
	{
#endif
	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0 ,  0,  UI_DEVICE_WIDTH,  UI_DEVICE_HEIGHT);
	gb_register_arrow_handler();
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	}
#endif

#endif
	TBM_EXIT(0x2880);
}

#ifndef __MMI_FULL_KEYPAD__

void GbKey0PressHandler(void)
{
	if (MMI_current_input_mode == INPUT_MODE_SMART_THAI||MMI_current_input_mode == INPUT_MODE_SMART_PERSIAN)
		GbInputHandlerFun('0');
	else
	{	
		/*zhoumn Modify Start For Bug 6271 Ver: TBM_780  on 2007-8-17 17:44 */
		GBIMEinsertChar( 32 );
		gbstate = GBIMESTATE_INPUT;
		#ifndef __MMI_GB_V5__
		ebis.syllableIndex = 0;
		ebis.associateISNCount = 0;
		#endif
		idxCandidate = 0;
		/*zhoumn Modify End  For Bug 6271 Ver: TBM_780  on 2007-8-17 17:44 */
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
      {
           // GoBackJavaEditer(); that GB_Reset will call state change then will cause double goback
      }
      else
      {
#endif	
		GbInputBoxScreenStateChange();
		GbDisplayForEmsEditor();
		if (MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX)
			wgui_EMS_redraw_remaining_characters_display();
		else
			wgui_redraw_remaining_characters_display();
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	}
  #endif	
	}
}

void GbKey1PressHandler(void)
{
	GbInputHandlerFun('1');
}

void GbKey2PressHandler(void)
{
	GbInputHandlerFun('2');
}

void GbKey3PressHandler(void)
{
	GbInputHandlerFun('3');
}

void GbKey4PressHandler(void)
{
	GbInputHandlerFun('4');
}

void GbKey5PressHandler(void)
{
	GbInputHandlerFun('5');
}

void GbKey6PressHandler(void)
{
	GbInputHandlerFun( '6');
}


void GbKey7PressHandler(void)
{
	GbInputHandlerFun('7');
}


void GbKey8PressHandler(void)
{
	GbInputHandlerFun('8');
}


void GbKey9PressHandler(void)
{
	GbInputHandlerFun('9');
}
#else
BOOL isShiftKeyPressed = FALSE;
extern BOOL EnterSymboFlag;

void GbKeyAPressHandler(void)
{
	GbInputHandlerFun('a');
}

void GbKeyBPressHandler(void)
{
	GbInputHandlerFun('b');
}

void GbKeyCPressHandler(void)
{
	GbInputHandlerFun('c');
}


void GbKeyDPressHandler(void)
{
	GbInputHandlerFun('d');
}


void GbKeyEPressHandler(void)
{
	GbInputHandlerFun('e');
}

void GbKeyFPressHandler(void)
{
	GbInputHandlerFun('f');
}

void GbKeyGPressHandler(void)
{
	GbInputHandlerFun('g');
}


void GbKeyHPressHandler(void)
{
	GbInputHandlerFun('h');
}

void GbKeyIPressHandler(void)
{
	GbInputHandlerFun('i');
}

void GbKeyJPressHandler(void)
{
	GbInputHandlerFun('j');
}

void GbKeyKPressHandler(void)
{
	GbInputHandlerFun('k');
}

void GbKeyLPressHandler(void)
{
	GbInputHandlerFun('l');
}

void GbKeyMPressHandler(void)
{
	GbInputHandlerFun('m');
}

void GbKeyNPressHandler(void)
{
	GbInputHandlerFun('n');
}

void GbKeyOPressHandler(void)
{
	GbInputHandlerFun('o');
}

void GbKeyPPressHandler(void)
{
	GbInputHandlerFun('p');
}

void GbKeyQPressHandler(void)
{
	GbInputHandlerFun('q');
}

void GbKeyRPressHandler(void)
{
	GbInputHandlerFun('r');
}

void GbKeySPressHandler(void)
{
	GbInputHandlerFun('s');
}

void GbKeyTPressHandler(void)
{
	GbInputHandlerFun('t');
}

void GbKeyUPressHandler(void)
{
	GbInputHandlerFun('u');
}

void GbKeyVPressHandler(void)
{
	GbInputHandlerFun('v');
}

void GbKeyWPressHandler(void)
{
	GbInputHandlerFun('w');
}

void GbKeyXPressHandler(void)
{
	GbInputHandlerFun('x');
}

void GbKeyYPressHandler(void)
{
	GbInputHandlerFun('y');
}

void GbKeyZPressHandler(void)
{
	GbInputHandlerFun('z');
}

void GbKeyCommaPressHandler(void)
	{
	UI_character_type symbol = 0;
	switch(GBIMEmethod)
	{
		case INPUT_MODE_SM_PINYIN:
			if(isShiftKeyPressed)
			{
			symbol = 0xff1a; // ':'
			}
			else
			{
				symbol = 0xff0c; // '，'
			}
			break;
		default:
			break;
	}
	if(symbol != 0)
		wgui_inputbox_insert_symbol(symbol);	
}

void GbKeyFullstopPressHandler(void)
{
	UI_character_type symbol = 0x0000;
	switch(GBIMEmethod)
	{
	case INPUT_MODE_SM_PINYIN:
			if(isShiftKeyPressed)
			{
				symbol = 0xff1b; // '；'
			}
			else
			{
				symbol = 0x3002; // '。'
			}
			break;
		default:
			break;
	}
	if(symbol != 0)
		wgui_inputbox_insert_symbol(symbol);	
}
void GbKeySpacePressHandler(void)
{
	int j = 0;
	TBM_ENTRY(0x2888);
    if (
#if defined(__MMI_SMART_ARABIC__)
	    GBIMEmethod == INPUT_MODE_SMART_ARABIC || 
#endif
#if defined(__MMI_SMART_PERSIAN__)
	    GBIMEmethod == INPUT_MODE_SMART_PERSIAN ||
#endif
#if defined(__MMI_SMART_THAI__)
	    GBIMEmethod == INPUT_MODE_SMART_THAI ||
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
	    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_VIETNAMESE||
	    GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_VIETNAMESE||
#endif
#if defined(__MMI_MULTITAP_FRENCH__)
	    GBIMEmethod == INPUT_MODE_MULTITAP_UPPERCASE_FRENCH  || 
	    GBIMEmethod == INPUT_MODE_MULTITAP_LOWERCASE_FRENCH || 
#endif
#if defined(__MMI_SMART_FRENCH__)
	    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_FRENCH  || 
	    GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_FRENCH || 
#endif
#if defined(__MMI_SMART_TURKISH__)
	    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_TURKISH|| 
	    GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_TURKISH|| 
#endif
#if defined(__MMI_SMART_INDONESIAN__)
	    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_INDONESIAN|| 
	    GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_INDONESIAN|| 
#endif
#if defined(__MMI_SMART_MALAY__)
	    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_MALAY|| 
	    GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_MALAY|| 
#endif
#if defined(__MMI_SMART_ITALIAN__)
	    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_ITALIAN|| 
	    GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_ITALIAN|| 
#endif
#if defined(__MMI_SMART_GERMAN__)			
	    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_GERMAN||
	    GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_GERMAN||
#endif
#if defined(__MMI_SMART_HINDI__)
	    GBIMEmethod == INPUT_MODE_SMART_HINDI || 
#endif
#if defined(__MMI_SMART_TELUGU__)
	    GBIMEmethod == INPUT_MODE_SMART_TELUGU|| 
#endif
#if defined(__MMI_SMART_SPANISH__)
	    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_SPANISH||
	    GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_SPANISH||
#endif
#if defined(__MMI_SMART_RUSSIAN__)
	    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_RUSSIAN ||
	    GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_RUSSIAN||
#endif
#if defined(__MMI_GB_SMART_PORTUGUESE__)
		 GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_PORTUGUESE ||
		 GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_PORTUGUESE||
#endif
#if defined(__MMI_SMART_URDU__)
		GBIMEmethod == INPUT_MODE_SMART_URDU ||
#endif
#if defined(__MMI_SMART_PUNJABI__)
		GBIMEmethod == INPUT_MODE_SMART_PUNJABI||
#endif
#if defined(__MMI_SMART_BENGALI__)
		GBIMEmethod == INPUT_MODE_SMART_BENGALI||
#endif
#if defined(__MMI_SMART_TAMIL__)
		GBIMEmethod == INPUT_MODE_SMART_TAMIL||
#endif

	    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_ABC || 
	    GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_ABC)
	{
		for (j = 0; j < UCS2Strlen((S8*)enis.candidates[idxCandidate]); j++)
		{
			GBIMEinsertChar(enis.candidates[idxCandidate][j]);
		}//此处是把下面输入栏的内容放到上面显示的关键一步
	}
	else
	{
		GBSelectCharacters();
	}
	
	GB_Reset();
	GbInputBoxScreenStateChange();
	GbDisplayForEmsEditor();
	if (MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX)
	{
		wgui_EMS_redraw_input_information_bar();
	}
	else
	{
		wgui_redraw_input_information_bar();
	}
	TBM_EXIT(0x2888);
}
void GbKeySpace1PressHandler(void)
{
	//blankspace function:
	//SM or smart ABC&abc when inputing ->confirm
	//SM or smart ABC&abc when idle ->input space
	//multi-tap not use this function do space
	UI_character_type symbol = 0x0000;
	mmi_trace(g_sw_IM," GbKeySpace1PressHandler %d ",gbstate);
	switch(gbstate)
	{
		case GBIMESTATE_IDLE: //input space
			switch(GBIMEmethod)
			{
				case INPUT_MODE_SM_PINYIN:
					symbol = 0x20; // ' '
					break;
				default:
					break;
			}
			if(symbol != 0)
				wgui_inputbox_insert_symbol(symbol);
			else
				wgui_inputbox_insert_symbol(' ');
			break;

		case GBIMESTATE_INPUT: //do confirm
		case GBIMESTATE_ASSATION:
			GbKeySpacePressHandler();
			break;
	}
}

void GbKey0PressHandler(void)
{
	UI_character_type symbol = 0x0000;
	switch(GBIMEmethod)
	{
		case INPUT_MODE_SM_PINYIN:
			if(isShiftKeyPressed)
			{
				symbol = 0x2f; // '/'
			}
			else
			{
			#ifdef   __MMI_BLACKBERRY_QWERTY__
				symbol = 0;	
			#else	 
				symbol = 0x40; // '@'
			#endif    
			}
			break;
		default:
			break;
	}
	if(symbol != 0)
		wgui_inputbox_insert_symbol(symbol);
}

void GbKeyNewlinePressHandler(void)
{
	UI_character_type symbol = 0x0000;
	mmi_trace(1," GbKeyNewlinePressHandler %d  ",GBIMEmethod);
	switch(GBIMEmethod)
	{
		case INPUT_MODE_SM_PINYIN:
			symbol = 0xa; // new line
			break;
		default:
			break;
	}
	if(symbol != 0)
	{
		EnterSymboFlag = TRUE;
		wgui_inputbox_insert_symbol(symbol);
	}
}

void GbKeyAndPressHandler(void)
{
	UI_character_type symbol = 0x0000;
	switch(GBIMEmethod)
	{
		case INPUT_MODE_SM_PINYIN:
			symbol = 0x26; // '&'
			break;
		default:
			break;
	}
	if(symbol != 0)
		wgui_inputbox_insert_symbol(symbol);
}

void GbKeyIntergrrPressHandler(void)
{
	UI_character_type symbol = 0x0000;
	switch(GBIMEmethod)
	{
		case INPUT_MODE_SM_PINYIN:
			symbol = 0xff1f; // '?' in chinese
			break;
		default:
			break;
	}
	if(symbol != 0)
		wgui_inputbox_insert_symbol(symbol);
}

void GbKeyShiftPressHandler(void)
{
	isShiftKeyPressed = !isShiftKeyPressed;
}
#endif

#ifdef __MMI_BLACKBERRY_QWERTY__	
extern void wgui_inputbox_handle_symbol_picker_star_key_down();
#endif
void GbRegisterKeyHandler(byte mode)
{
	GbClearKeyStarAndPoundHandler();
	GbClearKeyNavigationHandler();
	GbClearAllInputKeyHandler();
	SetKeyHandler(GbKeyArrowUPHandler,   KEY_UP_ARROW,   KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyArrowRightHandler,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyArrowDownHandler, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyArrowLeftHandler, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	if(GbInputMethodCategoryFlag&GB_IN_CATEGORY_28_FLAG)
		SetKeyHandler(handle_category28_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
	else if(GbInputMethodCategoryFlag&GB_IN_CATEGORY_5_FLAG)
		SetKeyHandler(wgui_handle_inputbox_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
	else if(GbInputMethodCategoryFlag&GB_IN_CATEGORY_SINGLELINE_INPUTBOX_FLAG)
		SetKeyHandler(wgui_handle_singleline_inputbox_change_input_mode,KEY_POUND,KEY_EVENT_DOWN);

#ifdef __MMI_FULL_KEYPAD__
	SetKeyHandler(GbKeyAPressHandler,KEY_A,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyBPressHandler,KEY_B,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyCPressHandler,KEY_C,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyDPressHandler,KEY_D,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyEPressHandler,KEY_E,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyFPressHandler,KEY_F,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyGPressHandler,KEY_G,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyHPressHandler,KEY_H,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyIPressHandler,KEY_I,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyJPressHandler,KEY_J,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyKPressHandler,KEY_K,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyLPressHandler,KEY_L,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyMPressHandler,KEY_M,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyNPressHandler,KEY_N,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyOPressHandler,KEY_O,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyPPressHandler,KEY_P,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyQPressHandler,KEY_Q,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyRPressHandler,KEY_R,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeySPressHandler,KEY_S,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyTPressHandler,KEY_T,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyUPressHandler,KEY_U,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyVPressHandler,KEY_V,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyWPressHandler,KEY_W,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyXPressHandler,KEY_X,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyYPressHandler,KEY_Y,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyZPressHandler,KEY_Z,KEY_EVENT_DOWN);

	SetKeyHandler(GbKeyCommaPressHandler, KEY_COMMA, KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyFullstopPressHandler, KEY_FULLSTOP, KEY_EVENT_DOWN);
	SetKeyHandler(GbKeySpace1PressHandler, KEY_SPACE1, KEY_EVENT_DOWN);
	SetKeyHandler(GbKey0PressHandler, KEY_0, KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyNewlinePressHandler, KEY_NEWLINE, KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyAndPressHandler, KEY_AND, KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyIntergrrPressHandler, KEY_INTEGRR, KEY_EVENT_DOWN);

	SetKeyHandler(GbKeyShiftPressHandler, KEY_SHIFT1, KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyStarPressHandler, KEY_CTRL1, KEY_EVENT_DOWN);

	#ifdef __MMI_BLACKBERRY_QWERTY__	
	SetKeyHandler(wgui_inputbox_handle_symbol_picker_star_key_down, KEY_SYM, KEY_EVENT_UP);
	#endif
	//SetKeyHandler(GbKeyBackSpaceHandler, KEY_BACKSPACE1, KEY_EVENT_DOWN);

#else/*__MMI_FULL_KEYPAD__*/

	SetKeyHandler(GbKeyStarPressHandler, KEY_STAR, KEY_EVENT_DOWN);
	if (mode != INPUT_MODE_SM_STROKE)
	{
		SetKeyHandler(GbKey1PressHandler,KEY_1,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey2PressHandler,KEY_2,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey3PressHandler,KEY_3,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey4PressHandler,KEY_4,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey5PressHandler,KEY_5,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey6PressHandler,KEY_6,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey7PressHandler,KEY_7,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey8PressHandler,KEY_8,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey9PressHandler,KEY_9,KEY_EVENT_DOWN);

	}
	else
	{
	#ifdef __STROKE_24568__
		SetKeyHandler(GbKey2PressHandler,KEY_2,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey4PressHandler,KEY_4,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey5PressHandler,KEY_5,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey6PressHandler,KEY_6,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey8PressHandler,KEY_8,KEY_EVENT_DOWN);
	#else
		SetKeyHandler(GbKey1PressHandler,KEY_1,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey2PressHandler,KEY_2,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey3PressHandler,KEY_3,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey4PressHandler,KEY_4,KEY_EVENT_DOWN);
		SetKeyHandler(GbKey5PressHandler,KEY_5,KEY_EVENT_DOWN);
	#endif
	}
	SetKeyHandler(GbKey0PressHandler,KEY_0,KEY_EVENT_DOWN);
#endif/*__MMI_FULL_KEYPAD__*/
}
/**************************************************************

    FUNCTION NAME    	:GbKeyArrowUPHandler

	  PURPOSE    			: 上键处理
    INPUT PARAMETERS    : nil

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/
void GbKeyArrowUPHandler(void)
{
	int ret = 0;

	//add by chenq for Bug 6247 20080815 B
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_editor_do_confirm();
#endif
	//add by chenq for Bug 6247 20080815 E

	if (MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX)
		GbInputMethodCategoryFlag = 2;									 //add by yaosq 20061103 
	else
		GbInputMethodCategoryFlag = 1;

	switch (GBIMEmethod)
	{
		case INPUT_MODE_SM_PINYIN:
		case INPUT_MODE_TR_BOPOMO:
		case INPUT_MODE_SM_STROKE:
			switch (gbstate)
			{
				case GBIMESTATE_IDLE:
					if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG))								// add by yaosq 20061103 for move left
						coolsand_UI_EMS_input_box_previous_line(&MMI_EMS_inputbox);
					else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG))
						multiline_inputbox_previous_line();
					else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_INLINE_MULTILINE_INPUTBOX_FLAG))
						multiline_inputbox_previous_line();		
					break;

				case GBIMESTATE_INPUT:
					if (GbIMECanPageUp())
					{
					#ifdef __MMI_GB_V5__
						GbIMEGetPrevPageCandidate();
					#else
						EBPrevPageC(&ebis);
					#endif
						idxCandidate = 0;
					}
					break;

				case GBIMESTATE_ASSATION:
					if (GbIMECanPageUp())
					{
					#ifdef __MMI_GB_V5__
						GbIMEGetPrevPageCandidate();
					#else
						EBPrevPageC(&ebis);
					#endif
						idxCandidate = 0;
					}
					break;
			}
			break;

	#if defined(__MMI_SMART_ARABIC__)
		case INPUT_MODE_SMART_ARABIC:
	#endif
	#if defined(__MMI_SMART_PERSIAN__)
		case INPUT_MODE_SMART_PERSIAN:
	#endif
	#if defined (__MMI_SMART_THAI__)
		case INPUT_MODE_SMART_THAI:
	#endif
	#if defined(__MMI_SMART_VIETNAMESE__)
		case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
	#endif
	#if defined(__MMI_MULTITAP_FRENCH__)
		case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
		case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
	#endif
	#if defined(__MMI_SMART_FRENCH__)
		case INPUT_MODE_SMART_UPPERCASE_FRENCH:
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
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
	#if defined(__MMI_SMART_ITALIAN__)
		case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
	#endif
	#if defined(__MMI_SMART_GERMAN__)					
		case INPUT_MODE_SMART_UPPERCASE_GERMAN:
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
	#endif
	#if defined(__MMI_SMART_HINDI__)
		case INPUT_MODE_SMART_HINDI:
	#endif
	#if defined(__MMI_SMART_TELUGU__)
		case INPUT_MODE_SMART_TELUGU:
	#endif
	#if defined(__MMI_SMART_SPANISH__)
		case INPUT_MODE_SMART_UPPERCASE_SPANISH:
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:	
	#endif
		case INPUT_MODE_SMART_UPPERCASE_ABC:
		case INPUT_MODE_SMART_LOWERCASE_ABC:
	#if defined(__MMI_SMART_RUSSIAN__)
		case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
		case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:	
	#endif
	#if defined(__MMI_GB_SMART_PORTUGUESE__)
		case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE :
		case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
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
			switch (gbstate)
			{
				case GBIMESTATE_IDLE:
					if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG))								// add by yaosq 20061103 for move left
						coolsand_UI_EMS_input_box_previous_line(&MMI_EMS_inputbox);
					else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG))
						multiline_inputbox_previous_line();
					else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_INLINE_MULTILINE_INPUTBOX_FLAG))
						multiline_inputbox_previous_line();		
					break;

				case GBIMESTATE_INPUT:
					SmartOption = 0;
				#ifdef __MMI_GB_V5__
					if(GbIMECanPageUp())
					{
						GBPrevPageCandidate(&g_gbis, g_gbis.outputInfo.nFirstCandIndex - g_gbis.outputInfo.nCandNum);
					}
				#else
					if (EnCanPrevHeader(&enis) == EBEN_OK)
					{
						ret = EnPrevHeader(&enis);
						if (ret >= 0)
						{
							EnFirstPage(&enis);
							idxCandidate = 0;
						}
					}
				#endif
					break;
				case GBIMESTATE_ASSATION:
					break;
			}
		break;
	}
	GbInputBoxScreenStateChange();
	GbDisplayForEmsEditor();
}

/**************************************************************

    FUNCTION NAME    	:GbKeyArrowUPHandler

	  PURPOSE    			: 右键处理
    INPUT PARAMETERS    : nil

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/

void GbKeyArrowRightHandler(void)
{
	//add by chenq for Bug 6247 20080815 B
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_editor_do_confirm();
#endif
	//add by chenq for Bug 6247 20080815 E
	switch (gbstate)
	{
		case GBIMESTATE_IDLE:
			if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG))
				coolsand_UI_EMS_input_box_next(&MMI_EMS_inputbox);
			else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG))
				coolsand_UI_multi_line_input_box_next(&MMI_multiline_inputbox);
			else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_SINGLELINE_INPUTBOX_FLAG))
				coolsand_UI_single_line_input_box_next(&MMI_singleline_inputbox);
			else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_INLINE_MULTILINE_INPUTBOX_FLAG))
				coolsand_UI_multi_line_input_box_next(&MMI_multiline_inputbox);
			break;

		case GBIMESTATE_INPUT:
			idxCandidate++;
			if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
			{
				if (idxCandidate >= GbIMEGetCandidateCount())
				{
					if (GbIMECanPageDown())
					{
					#ifdef __MMI_GB_V5__
						GbIMEGetNextCandidate();
					#else
						EBNextPageC(&ebis);
					#endif
						idxCandidate = 0;
					}
					else
					{
						/*Fix Bug#12588 for cs2324c by cong.li on 2009.06.23. 
						Considering reference phone, we shouldn't circle candidate.*/
						//EBFindFirstPageC(&ebis);
						//idxCandidate = 0;
						idxCandidate = GbIMEGetCandidateCount()-1;
					}
				}
				else
				{
					#ifdef __MMI_GB_V5__
					GbIMEGetNextCandidate();
					#endif
					coolsand_UI_single_line_input_box_next(&gbCandidateArea);
				}
			}
			else
			{

				SmartOption = 0;
				if (idxCandidate >= GbIMEGetCandidateCount())
				{
					if (GbIMECanPageDown())
					{
					#ifdef __MMI_GB_V5__
						GBNextPageCandidate(&g_gbis, g_gbis.outputInfo.nCandNum + g_gbis.outputInfo.nFirstCandIndex);
					#else
						EnNextPage(&enis);
					#endif
					}
					idxCandidate = 0;
				}
				coolsand_UI_single_line_input_box_next(&gbCandidateArea);
			}
			break;

		case GBIMESTATE_ASSATION:
			idxCandidate++;
			if (idxCandidate >= GbIMEGetCandidateCount())
			{
				if (GbIMECanPageDown())
				{
				#ifdef __MMI_GB_V5__
					GbIMEGetNextCandidate();
				#else
					EBNextPageC(&ebis);
				#endif
					idxCandidate = 0;
				}
				else
				{
					//EBFindFirstPageC(&ebis);
					idxCandidate =GbIMEGetCandidateCount() -1;
				}
			}
			else
			{
				#ifdef __MMI_GB_V5__
				GbIMEGetNextCandidate();
				#endif
				coolsand_UI_single_line_input_box_next(&gbCandidateArea);
			}
			break;
	}
	GbInputBoxScreenStateChange();
	GbDisplayForEmsEditor();
}

void GbKeyArrowDownHandler(void)
{
	int ret;

	//add by chenq for Bug 6247 20080815 B
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_editor_do_confirm();
#endif
	//add by chenq for Bug 6247 20080815 E

	switch (GBIMEmethod)
	{
		case INPUT_MODE_TR_BOPOMO:
		case INPUT_MODE_SM_PINYIN:
		case INPUT_MODE_SM_STROKE:
			switch (gbstate)
			{
				case GBIMESTATE_IDLE:
					if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG))								// add by yaosq 20061103 for move left
						coolsand_UI_EMS_input_box_next_line(&MMI_EMS_inputbox);
					else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG))
						multiline_inputbox_next_line();
					else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_INLINE_MULTILINE_INPUTBOX_FLAG))
						multiline_inputbox_next_line();	
					break;

				case GBIMESTATE_INPUT:
					if (GbIMECanPageDown())
					{
					#ifdef __MMI_GB_V5__
						GbIMEGetNextPageCandidate();
					#else
						EBNextPageC(&ebis);
					#endif
						idxCandidate=0 ;
					}
					break;

				case GBIMESTATE_ASSATION:
					if (GbIMECanPageDown())
					{
					#ifdef __MMI_GB_V5__
						GbIMEGetNextPageCandidate();
					#else
						EBNextPageC(&ebis);
					#endif
						idxCandidate = 0;
					}
					break;
			}
			break;

	#if defined(__MMI_SMART_ARABIC__)
		case INPUT_MODE_SMART_ARABIC:
	#endif
	#if defined(__MMI_SMART_PERSIAN__)
		case INPUT_MODE_SMART_PERSIAN:
	#endif
	#if defined(__MMI_SMART_THAI__)
		case INPUT_MODE_SMART_THAI:
	#endif
	#if defined(__MMI_SMART_VIETNAMESE__)
		case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
	#endif
	#if defined(__MMI_MULTITAP_FRENCH__)
		case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
		case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
	#endif
	#if defined(__MMI_SMART_FRENCH__)
		case INPUT_MODE_SMART_UPPERCASE_FRENCH:
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
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
	#if defined(__MMI_SMART_ITALIAN__)
		case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
	#endif
	#if defined(__MMI_SMART_GERMAN__)			
		case INPUT_MODE_SMART_UPPERCASE_GERMAN:
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
	#endif
	#if defined(__MMI_SMART_SPANISH__)
		case INPUT_MODE_SMART_UPPERCASE_SPANISH:
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:	
	#endif
	#if defined(__MMI_SMART_HINDI__)
		case INPUT_MODE_SMART_HINDI:
	#endif
	#if defined(__MMI_SMART_TELUGU__)
		case INPUT_MODE_SMART_TELUGU:
	#endif
		case INPUT_MODE_SMART_UPPERCASE_ABC:
		case INPUT_MODE_SMART_LOWERCASE_ABC:
	#if defined(__MMI_SMART_RUSSIAN__)
		case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
		case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:	 
	#endif
	#if defined(__MMI_GB_SMART_PORTUGUESE__)
		case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE :
		case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
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

			switch (gbstate)
			{
				case GBIMESTATE_IDLE:
					if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG))								// add by yaosq 20061103 for move left
						coolsand_UI_EMS_input_box_next_line(&MMI_EMS_inputbox);
					else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG))
						multiline_inputbox_next_line();
					else if ((GbInputMethodCategoryFlag    & GB_IN_CATEGORY_INLINE_MULTILINE_INPUTBOX_FLAG))
						multiline_inputbox_next_line();	
					break;

				case GBIMESTATE_INPUT:
					SmartOption = 0;
				#ifdef __MMI_GB_V5__
					if(GbIMECanPageDown())
					{
						GBNextPageCandidate(&g_gbis,g_gbis.outputInfo.nCandNum + g_gbis.outputInfo.nFirstCandIndex);
					}
				#else
					if (EnCanNextHeader(&enis) == EBEN_OK)
					{
						ret = EnNextHeader(&enis);
						if (ret >= 0)
						{
							EnFirstPage(&enis);
							idxCandidate = 0;
						}
					}
					#endif
				case GBIMESTATE_ASSATION:
					break;
			}
		break;
	}
	GbInputBoxScreenStateChange();
	GbDisplayForEmsEditor();
}

/**************************************************************

    FUNCTION NAME    	:GbKeyArrowUPHandler

	  PURPOSE    			: 左键处理
    INPUT PARAMETERS    : nil

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/
void GbKeyArrowLeftHandler(void)
{

	//add by chenq for Bug 6247 20080815 B
#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_editor_do_confirm();
#endif
	//add by chenq for Bug 6247 20080815 E

	if (MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX)
		GbInputMethodCategoryFlag = 2;									 //add by yaosq 20061103 
	else
		GbInputMethodCategoryFlag = 1;
	switch (gbstate)
	{
		case GBIMESTATE_IDLE:
			if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG))								// add by yaosq 20061103 for move left
			{
				coolsand_UI_EMS_input_box_previous(&MMI_EMS_inputbox);
			}
			else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG))
				coolsand_UI_multi_line_input_box_previous(&MMI_multiline_inputbox);
			else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_SINGLELINE_INPUTBOX_FLAG))
				coolsand_UI_single_line_input_box_next(&MMI_singleline_inputbox);
			else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_INLINE_MULTILINE_INPUTBOX_FLAG))
				coolsand_UI_multi_line_input_box_previous(&MMI_multiline_inputbox);
			break;

		case GBIMESTATE_INPUT:
			idxCandidate--;
			if (GBIMEmethod == INPUT_MODE_TR_BOPOMO || GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
			{
				if (idxCandidate <= 0)
				{
					if (GbIMECanPageUp())
					{
					#ifdef __MMI_GB_V5__
						GbIMEGetPrevCandidate();
						idxCandidate = GbIMEGetCandidateCount()-1;
					#else
						EBPrevPageC(&ebis);
						idxCandidate=ebis.candidateLen - 1;
					#endif
					}
					else
					{	
						/*Fix Bug#12588 for cs2324c by cong.li on 2009.06.23. 
						Considering reference phone, we shouldn't circle candidate.*/
						//idxCandidate += ebis.candidateLen;
						//idxCandidate %= ebis.candidateLen;
						idxCandidate=0;
					}
				}
				else
				{
					#ifdef __MMI_GB_V5__
					GbIMEGetPrevCandidate();
					#endif
				}
			}
			else
			{
				SmartOption = 0;
				if (idxCandidate <= 0)			//按下"*"键在当前head 候选中进行循环切换
				{
					if (GbIMECanPageUp())
					{
					#ifdef __MMI_GB_V5__
						GBPrevPageCandidate(&g_gbis, g_gbis.outputInfo.nFirstCandIndex - g_gbis.outputInfo.nCandNum);
					#else
						EnPrevPage(&enis);
					#endif
					}
					idxCandidate = 0;
				}
				coolsand_UI_single_line_input_box_next(&gbCandidateArea);
			}
			break;

		case GBIMESTATE_ASSATION:
			idxCandidate--;
			if (idxCandidate <= 0)
			{
				if (GbIMECanPageUp())
				{
				#ifdef __MMI_GB_V5__
					GbIMEGetPrevCandidate();
					idxCandidate = GbIMEGetCandidateCount()-1;
				#else
					EBPrevPageC(&ebis);
					idxCandidate = ebis.candidateLen - 1;
				#endif
				}
				else
					idxCandidate = 0;
			}
			else
			{
				#ifdef __MMI_GB_V5__
				GbIMEGetPrevCandidate();
				#endif
			}
			break;

		default:
			break;
	}
	GbDisplayForEmsEditor();
}

/**************************************************************

    FUNCTION NAME    	:GbKeyStarPressHandler

	  PURPOSE    			: *号键处理
    INPUT PARAMETERS    : nil

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/
void GbKeyStarPressHandler(void)
{
	switch(gbstate)
	{
		case GBIMESTATE_INPUT:
			if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
			{
			#ifdef __MMI_GB_V5__
				if(g_gbis.pAuxInfo->nSyllableNum > 0)
				{
					GbIMEGetNextSyllable();
					idxCandidate = 0;
				}
			#else
				if(ebis.syllableLen > 0)
				{
					ebis.syllableIndex = (ebis.syllableIndex + 1) % (ebis.syllableLen + 1);
					EBFindFirstPageC(&ebis);
					idxCandidate = 0;
				}
			#endif
			}
			else
			{
				if (GbIMEGetInputStrLen() > 1)
				{
					SmartOption++;
					if (SmartOption > 2)
						SmartOption = 0;
					if (SmartOption == 0)
					{
						if (GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_ABC 
						#if defined(__MMI_MULTITAP_FRENCH__)				
							|| GBIMEmethod == INPUT_MODE_MULTITAP_LOWERCASE_FRENCH
						#endif
						#if defined(__MMI_SMART_FRENCH__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_FRENCH
						#endif
						#if defined(__MMI_SMART_TURKISH__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_TURKISH
						#endif
						#if defined(__MMI_SMART_INDONESIAN__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_INDONESIAN
						#endif
						#if defined(__MMI_SMART_MALAY__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_MALAY
						#endif
						#if defined(__MMI_SMART_ITALIAN__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_ITALIAN
						#endif
						#if defined(__MMI_SMART_GERMAN__)						
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_GERMAN
						#endif
						#if defined(__MMI_SMART_HINDI__)
							|| GBIMEmethod == INPUT_MODE_SMART_HINDI
						#endif
						#if defined(__MMI_SMART_TELUGU__)
							|| GBIMEmethod == INPUT_MODE_SMART_TELUGU
						#endif
						#if defined(__MMI_SMART_SPANISH__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_SPANISH
						#endif
						)
						{
						#ifdef __MMI_GB_V5__
							GBGetDesireOutput(&g_gbis, GBA_OUTPUT_LOWER); 
						#else
							EnGetDesireOutput(&enis, EN_OPTION_OUTPUT_LOWER);
						#endif
						}
						else if (GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_ABC  
						#if defined(__MMI_MULTITAP_FRENCH__)
							|| GBIMEmethod == INPUT_MODE_MULTITAP_UPPERCASE_FRENCH 
						#endif
						#if defined(__MMI_SMART_FRENCH__)
							|| GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_FRENCH
						#endif
						#if defined(__MMI_SMART_TURKISH__)
							||GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_TURKISH
						#endif
						#if defined(__MMI_SMART_INDONESIAN__)
							||GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_INDONESIAN
						#endif
						#if defined(__MMI_SMART_MALAY__)
							||GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_MALAY
						#endif
						#if defined(__MMI_SMART_ITALIAN__)
							|| GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_ITALIAN
						#endif
						#if defined(__MMI_SMART_ARABIC__)
							|| GBIMEmethod == INPUT_MODE_SMART_ARABIC
						#endif
						#if defined(__MMI_SMART_PERSIAN__)
							|| GBIMEmethod == INPUT_MODE_SMART_PERSIAN
						#endif
						#if defined(__MMI_SMART_HINDI__)
							|| GBIMEmethod == INPUT_MODE_SMART_HINDI
						#endif
						#if defined(__MMI_SMART_TELUGU__)
							|| GBIMEmethod == INPUT_MODE_SMART_TELUGU
						#endif
						#if defined(__MMI_SMART_GERMAN__)				
							|| GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_GERMAN
						#endif
						#if defined(__MMI_SMART_SPANISH__)
							||GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_SPANISH
						#endif
							|| GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_RUSSIAN
						#if defined(__MMI_GB_SMART_PORTUGUESE__)
							|| GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_PORTUGUESE
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_PORTUGUESE
						#endif
						)
						{
						#ifdef __MMI_GB_V5__
							GBGetDesireOutput(&g_gbis, GBA_OUTPUT_CAPITAL); 
						#else
							EnGetDesireOutput(&enis, EN_OPTION_OUTPUT_CAPITAL);
						#endif
						}
					}
					else if (SmartOption == 1)
					{
						if (GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_ABC 
						#if defined(__MMI_MULTITAP_FRENCH__)
							|| GBIMEmethod == INPUT_MODE_MULTITAP_LOWERCASE_FRENCH
						#endif
						#if defined(__MMI_SMART_FRENCH__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_FRENCH
						#endif
						#if defined(__MMI_SMART_TURKISH__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_TURKISH
						#endif
						#if defined(__MMI_SMART_INDONESIAN__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_INDONESIAN
						#endif
						#if defined(__MMI_SMART_MALAY__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_MALAY
						#endif
						#if defined(__MMI_SMART_ITALIAN__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_ITALIAN
						#endif
						#if defined(__MMI_SMART_HINDI__)
							||GBIMEmethod == INPUT_MODE_SMART_HINDI
						#endif
						#if defined(__MMI_SMART_TELUGU__)
							||GBIMEmethod == INPUT_MODE_SMART_TELUGU
						#endif
						#if defined(__MMI_SMART_GERMAN__)				
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_GERMAN
						#endif
						#if defined(__MMI_SMART_SPANISH__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_SPANISH
						#endif
							||    GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_RUSSIAN
						)
						{
						#ifdef __MMI_GB_V5__
							GBGetDesireOutput(&g_gbis, GBA_OUTPUT_FCC); 
						#else
							EnGetDesireOutput(&enis, EN_OPTION_OUTPUT_FCC);
						#endif
						}
						else if (GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_ABC 
						#if defined(__MMI_MULTITAP_FRENCH__)
							||GBIMEmethod == INPUT_MODE_MULTITAP_UPPERCASE_FRENCH 
						#endif
						#if defined(__MMI_SMART_FRENCH__)
							||    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_FRENCH
						#endif
						#if defined(__MMI_SMART_TURKISH__)
							||    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_TURKISH
						#endif
						#if defined(__MMI_SMART_INDONESIAN__)
							||    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_INDONESIAN
						#endif
						#if defined(__MMI_SMART_MALAY__)
							||GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_MALAY
						#endif
						#if defined(__MMI_SMART_ITALIAN__)
							||    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_ITALIAN
						#endif
						#if defined(__MMI_SMART_GERMAN__)				
							|| GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_GERMAN
						#endif
						#if defined(__MMI_SMART_HINDI__)
							||	 GBIMEmethod == INPUT_MODE_SMART_HINDI
						#endif
						#if defined(__MMI_SMART_TELUGU__)
							||	 GBIMEmethod == INPUT_MODE_SMART_TELUGU
						#endif
						#if defined(__MMI_SMART_SPANISH__)
							||    GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_SPANISH
						#endif
						#if defined(__MMI_SMART_ARABIC__)
							||	 GBIMEmethod == INPUT_MODE_SMART_ARABIC
						#endif
						#if defined(__MMI_SMART_PERSIAN__)
							||   GBIMEmethod == INPUT_MODE_SMART_PERSIAN
						#endif
							|| GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_RUSSIAN
						#if defined(__MMI_GB_SMART_PORTUGUESE__)
							||GBIMEmethod  ==  INPUT_MODE_SMART_UPPERCASE_PORTUGUESE 
							||GBIMEmethod  ==  INPUT_MODE_SMART_LOWERCASE_PORTUGUESE
						#endif
						)
						{
						#ifdef __MMI_GB_V5__
							GBGetDesireOutput(&g_gbis, GBA_OUTPUT_LOWER); 
						#else
							EnGetDesireOutput(&enis, EN_OPTION_OUTPUT_LOWER);
						#endif
						}
					}
					else if (SmartOption == 2)
					{
						if (GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_ABC 
						#if defined(__MMI_MULTITAP_FRENCH__)
							|| GBIMEmethod == INPUT_MODE_MULTITAP_LOWERCASE_FRENCH
						#endif
						#if defined(__MMI_SMART_FRENCH__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_FRENCH
						#endif
						#if defined(__MMI_SMART_TURKISH__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_TURKISH
						#endif
						#if defined(__MMI_SMART_INDONESIAN__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_INDONESIAN
						#endif
						#if defined(__MMI_SMART_MALAY__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_MALAY
						#endif
						#if defined(__MMI_SMART_ITALIAN__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_ITALIAN
						#endif
						#if defined(__MMI_SMART_HINDI__)
							||GBIMEmethod == INPUT_MODE_SMART_HINDI
						#endif
						#if defined(__MMI_SMART_TELUGU__)
							||GBIMEmethod == INPUT_MODE_SMART_TELUGU
						#endif
						#if defined(__MMI_SMART_GERMAN__)			
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_GERMAN
						#endif
						#if defined(__MMI_SMART_SPANISH__)
							|| GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_SPANISH
						#endif
							||    GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_RUSSIAN)
						{
						#ifdef __MMI_GB_V5__
							GBGetDesireOutput(&g_gbis, GBA_OUTPUT_CAPITAL); 
						#else
							EnGetDesireOutput(&enis, EN_OPTION_OUTPUT_CAPITAL);
						#endif
						}
						else
						{
						#ifdef __MMI_GB_V5__
							GBGetDesireOutput(&g_gbis, GBA_OUTPUT_FCC); 
						#else
							EnGetDesireOutput(&enis, EN_OPTION_OUTPUT_FCC);	 
						#endif
						}
					}
				}
			}
			//qiff add for disable navigate error tone, for starkey handle in GB inputmethod
			gbCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_TONES ;
			coolsand_UI_single_line_input_box_next(&gbCandidateArea);
			GbDisplayForEmsEditor();
			break;
		default:
			EntrySymbolPickerScreen();
			break;
	}
}

void  GbClearKeyNavigationHandler(void)
{
	U16 keyCode = 0;
	U16 keyType = 0;

	for (keyCode = KEY_UP_ARROW; keyCode <= KEY_RIGHT_ARROW; keyCode++)
	{
		switch (keyCode)
		{
			case KEY_UP_ARROW:
			case KEY_DOWN_ARROW:
			case KEY_RIGHT_ARROW:
			case KEY_LEFT_ARROW:
				for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
					ClearKeyHandler(keyCode,keyType);
				break;
		}
	}
}

void  GbClearKey0To9Handler(void)
{
	U16 keyCode = 0;
	U16 keyType = 0;
	 
	for (keyCode = KEY_0; keyCode <= KEY_9; keyCode++)
	{
		for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
		{
			ClearKeyHandler(keyCode,keyType);
		}
	}
}

void GbClearKeyStarAndPoundHandler(void)
{
	U16 keyCode = 0;
	U16 keyType = 0;

	for (keyCode = KEY_STAR; keyCode < KEY_POUND; keyCode++)
	{
		for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
		{
			ClearKeyHandler(keyCode,keyType);
		}
	}
}

void GbClearAllInputKeyHandler(void)
{
#ifdef __MMI_FULL_KEYPAD__
	U16 keyCode = 0;
	U16 keyType = 0;
	for (keyCode = KEY_A; keyCode <= KEY_Z; keyCode++)
	{
		for (keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
		{
			ClearKeyHandler(keyCode,keyType);
		}
	}
#endif
	GbClearKey0To9Handler();
}

/**************************************************************

    FUNCTION NAME    	: GbKeyLSKPressHandler

	  PURPOSE    			:左软键处理
    INPUT PARAMETERS    : nil

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/
void GbKeyLSKPressHandler(void)
{
	int j = 0;
	TBM_ENTRY(0x2888);

	if (
	#if defined(__MMI_SMART_ARABIC__)
		GBIMEmethod == INPUT_MODE_SMART_ARABIC || 
	#endif
	#if defined(__MMI_SMART_PERSIAN__)
		GBIMEmethod == INPUT_MODE_SMART_PERSIAN ||
	#endif
	#if defined(__MMI_SMART_THAI__)
		GBIMEmethod == INPUT_MODE_SMART_THAI ||
	#endif
	#if defined(__MMI_SMART_VIETNAMESE__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_VIETNAMESE||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_VIETNAMESE||
	#endif
	#if defined(__MMI_MULTITAP_FRENCH__)
		GBIMEmethod == INPUT_MODE_MULTITAP_UPPERCASE_FRENCH  || 
		GBIMEmethod == INPUT_MODE_MULTITAP_LOWERCASE_FRENCH || 
	#endif
	#if defined(__MMI_SMART_FRENCH__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_FRENCH  || 
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_FRENCH || 
	#endif
	#if defined(__MMI_SMART_TURKISH__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_TURKISH|| 
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_TURKISH|| 
	#endif
	#if defined(__MMI_SMART_INDONESIAN__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_INDONESIAN|| 
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_INDONESIAN|| 
	#endif
	#if defined(__MMI_SMART_MALAY__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_MALAY|| 
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_MALAY|| 
	#endif
	#if defined(__MMI_SMART_ITALIAN__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_ITALIAN|| 
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_ITALIAN|| 
	#endif
	#if defined(__MMI_SMART_GERMAN__)			
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_GERMAN||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_GERMAN||
	#endif
	#if defined(__MMI_SMART_HINDI__)
		GBIMEmethod == INPUT_MODE_SMART_HINDI || 
	#endif
	#if defined(__MMI_SMART_TELUGU__)
		GBIMEmethod == INPUT_MODE_SMART_TELUGU|| 
	#endif
	#if defined(__MMI_SMART_SPANISH__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_SPANISH||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_SPANISH||
	#endif
	#if defined(__MMI_SMART_RUSSIAN__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_RUSSIAN ||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_RUSSIAN||
	#endif
	#if defined(__MMI_GB_SMART_PORTUGUESE__)
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_PORTUGUESE ||
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_PORTUGUESE||
	#endif
	#if defined(__MMI_SMART_URDU__)
		GBIMEmethod == INPUT_MODE_SMART_URDU||
	#endif
	#if defined(__MMI_SMART_PUNJABI__)
		GBIMEmethod == INPUT_MODE_SMART_PUNJABI||
	#endif
	#if defined(__MMI_SMART_BENGALI__)
		GBIMEmethod == INPUT_MODE_SMART_BENGALI||
	#endif
	#if defined(__MMI_SMART_TAMIL__)
		GBIMEmethod == INPUT_MODE_SMART_TAMIL||
	#endif
		GBIMEmethod == INPUT_MODE_SMART_UPPERCASE_ABC || 
		GBIMEmethod == INPUT_MODE_SMART_LOWERCASE_ABC
	)
	{
	#ifdef __MMI_GB_V5__
		#if defined(__MMI_SMART_URDU__)
		//NOTE: in urdu, the last candidate is left to right and others is right ot left, so we add this section to handle this
		if(GBIMEmethod == INPUT_MODE_SMART_URDU && GbIMEGetInputStrLen() == 1
			&&(GbIMEGetCandidateCount() -2 == idxCandidate||GbIMEGetCandidateCount() - 1 == idxCandidate))
		{
			if(idxCandidate == GbIMEGetCandidateCount() - 2)
			{
				for(j = 0; j < UCS2Strlen((S8*)g_gbis.outputInfo.pCandidates[idxCandidate + 1]); j++)
				{
					GBIMEinsertChar(g_gbis.outputInfo.pCandidates[idxCandidate + 1][j]);
				}
			}
			if(idxCandidate == GbIMEGetCandidateCount() - 1)
			{
				for(j = 0; j < UCS2Strlen((S8*)g_gbis.outputInfo.pCandidates[idxCandidate - 1]); j++)
				{
					GBIMEinsertChar(g_gbis.outputInfo.pCandidates[idxCandidate - 1][j]);
				}
			}
		}
		else
		#endif
		for(j = 0; j < UCS2Strlen((S8*)g_gbis.outputInfo.pCandidates[idxCandidate]); j++)
		{
			GBIMEinsertChar(g_gbis.outputInfo.pCandidates[idxCandidate][j]);
		}
	#else
		for (j = 0; j < UCS2Strlen((S8*)enis.candidates[idxCandidate]); j++)
		{
			GBIMEinsertChar(enis.candidates[idxCandidate][j]);
		}
	#endif
		GB_Reset();
	}
	else
	{
		GBSelectCharacters();
	}

#if 0//def __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
      {
           // GoBackJavaEditer(); that GB_Reset will call state change then will cause double goback
      }
      else
      {
#endif	
	gdi_layer_lock_frame_buffer();
	GbInputBoxScreenStateChange();
	GbDisplayForEmsEditor();
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
      {
           // GoBackJavaEditer(); that GB_Reset will call state change then will cause double goback
      }
      else
      {
#endif	
	if (MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX)
	{
		wgui_EMS_redraw_input_information_bar();
	}
	else
	{
		wgui_redraw_input_information_bar();
	}
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	}
#endif		
	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0 ,  0,  UI_DEVICE_WIDTH,  UI_DEVICE_HEIGHT);
	TBM_EXIT(0x2888);
#if 0//def __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	}
#endif		
}

void GbKeyRSKLongPressHandler()
{
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{	
		mmi_trace(g_sw_IM, "GbKeyRSKLongPressHandler");
	#ifdef __MMI_GB_V5__
		//
	#else
		ebis.fullOutputString[0] = 0;
		ebis.syllableLen = 0;

		ebis.inputStringLen = 0;
		ebis.associateISNCount = 0;
	#endif
	}
	else
	{
		if (GbIMEHasInput())
		{	
		#ifdef __MMI_GB_V5__
			// 
		#else
			enis.inputLen = 0;
			enis.inputString[enis.inputLen] = 0;
			EnFirstHeader(&enis);
			EnFirstPage(&enis);
		#endif
		}
	}
	GB_Reset();
	GbInputBoxScreenStateChange();
	GbDisplayForEmsEditor();
}
void GbKeyRSKPressHandler(void)
{
	TBM_ENTRY(0x2889);
	switch(gbstate)
	{
		case GBIMESTATE_IDLE:
			GBDelectcharater();
			break;
		case GBIMESTATE_INPUT:
			GBDeleteKey();
			break;
		case GBIMESTATE_ASSATION:
			GB_Reset();
			break;
	}


#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
    {
	if(gbstate!=GBIMESTATE_IDLE)
	{
    		GbInputBoxScreenStateChange();
		GbDisplayForEmsEditor();
	}
    }
    else
	{
#endif	
		GbInputBoxScreenStateChange();
		GbDisplayForEmsEditor();
	if (MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX)
	{
		wgui_EMS_redraw_input_information_bar();
	}
	else
	{
		wgui_redraw_input_information_bar();
	}
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	}
#endif	
	TBM_EXIT(0x2889);
}

void GBIMEinsertChar( unsigned short gbchar)
{
	U8 r = 0;
	TBM_ENTRY(0x2887);
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
	if(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
    {
        //gcj_buffer_insert_char(gbchar);
        coolsand_UI_single_line_input_box_insert_character( &gcj_native_input_box ,gbchar);
	return ;
    }
#endif		
	
	if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG))
	{
		mmi_trace(g_sw_IM, "GBIMEinsertChar coolsand_UI_EMS_input_box_insert_character");
		r = coolsand_UI_EMS_input_box_insert_character(&MMI_EMS_inputbox, gbchar);
		if (r != EMS_NO_SPACE)
		{
			if (MMI_EMS_inputbox.UCS2_count > 0)
			{
				if ((MMI_EMS_inputbox.data->Reminder.totalRemainingOctet != GbLargestByteNumberInSms && MMI_EMS_inputbox.UCS2_count == 1) && gbchar != 32)								 //当有汉字出现时重新计算提示字符,空格此时本来就是占两个字节，不用重新计算
					MMI_EMS_inputbox.data->Reminder.totalRemainingOctet = GbLargestByteNumberInSms - (GbLargestByteNumberInSms - MMI_EMS_inputbox.data->Reminder.totalRemainingOctet) * 2; 
				MMI_EMS_inputbox.data->Reminder.totalRemainingOctet -= 2;
			}
			else
				MMI_EMS_inputbox.data->Reminder.totalRemainingOctet -= 1;
		}
	}
	else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG))
	{
		inline_full_screen_edit_changed = 1; //add by yaosq 20061110 for save
		coolsand_UI_multi_line_input_box_insert_character(&MMI_multiline_inputbox, gbchar);
	}
	else if((GbInputMethodCategoryFlag & GB_IN_CATEGORY_SINGLELINE_INPUTBOX_FLAG))
	{
		coolsand_UI_single_line_input_box_insert_character(&MMI_singleline_inputbox, gbchar);
	}
	else if((GbInputMethodCategoryFlag & GB_IN_CATEGORY_INLINE_MULTILINE_INPUTBOX_FLAG))
	{
		coolsand_UI_multi_line_input_box_insert_character(&MMI_multiline_inputbox, gbchar);
	}
	TBM_EXIT(0x2887);
}

void GBSelectCharacters(void)
{
#ifdef __MMI_GB_V5__
	INT32 i;
	GbIMEAddInputChar(GBKEY_OK);
	if(NULL != g_gbis.outputInfo.pUpScreenStr)
	{
		for(i = 0; i < UCS2Strlen(g_gbis.outputInfo.pUpScreenStr); i++)
		{
			GBIMEinsertChar(*(g_gbis.outputInfo.pUpScreenStr+i));
		}
		if (((MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX) && (MMI_EMS_inputbox.data->textLength < MMI_EMS_inputbox.available_length * 2))
			|| ((MMI_current_input_box_type == EDITOR_MULTILINE_INPUT_BOX) && (MMI_multiline_inputbox.text_length < MMI_multiline_inputbox.available_length)))
		{
			
		}
		else
		{
			GB_Reset();
		}
		
		idxCandidate = 0;
		gbSelectWordFlag = 0;
	}
	PRINT_ENIS();
#else
	INT32 i;  
	INT32 ret;
	TBM_ENTRY(0x2885);

	ret = EBHelperSelect(&ebis, idxCandidate);
	if (ret < 0)
	{
		return;
	}
	else
	{
		int fullOutputStringLen = UCS2Strlen((char *)ebis.fullOutputString);

		if(ret == 0)
		{
			for (i = 0; i < fullOutputStringLen; i++)
			{
				GBIMEinsertChar(ebis.fullOutputString[i]);
			}
		}
		else
		{
			if(ret < fullOutputStringLen)
			{
				for (i = 0; i < fullOutputStringLen - ret; i++)
				{
					GBIMEinsertChar(ebis.fullOutputString[i]);
				}
			}
			else
			{
				for (i = 0; i < fullOutputStringLen ; i++)
				{
					GBIMEinsertChar(ebis.fullOutputString[i]);
				}			
			}
		}

		if (((MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX) && (MMI_EMS_inputbox.data->textLength < MMI_EMS_inputbox.available_length * 2))
			|| ((MMI_current_input_box_type == EDITOR_MULTILINE_INPUT_BOX) && (MMI_multiline_inputbox.text_length < MMI_multiline_inputbox.available_length))
//here need dujge if exceed
#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
			||(gcj_IsJavaOn()&&gcj_IsFormJavaNativeEditer())
#endif	
			)
		{
			//用户每输入一个汉字都应该调用EBAssocAddHistoryISN, 以更新联想词
			EBAssocAddHistoryISN(&ebis, ebis.fullOutputString, fullOutputStringLen);
			//表示开始重新输入词语
			EBHelperEBISRestart(&ebis);
			//获得联想词
			EBAssocFirstPageC(&ebis);
			//wordselect=GBFLASE;
		}
		else
			GB_Reset();
		idxCandidate = 0;
		gbSelectWordFlag = 0;
	}
	TBM_EXIT(0x2885);
#endif
}

/**************************************************************

    FUNCTION NAME    	:GBDeleteKey

	  PURPOSE    			: 删除键处理
    INPUT PARAMETERS    : nil

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/

void GBDeleteKey(void)
{
	idxCandidate = 0;
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
	#ifdef __MMI_GB_V5__
		if (GbIMEHasInput())
		{
			GbIMEDeleteInputChar();
		}
		else
		{
			GB_Reset();
			idxCandidate = 0;
			gbstate = GBIMESTATE_IDLE;
		}
	#else
		if (EBHelperCancelSelect(&ebis) == EB_OK)
		{
			EBFindFirstPageC(&ebis);
		}
		else if(EBHelperDeleteInputChar(&ebis) == EB_OK)
		{
			ebis.syllableIndex = EB_NULL;
			if (ebis.inputStringLen > 0)
			{
				EBFindFirstPageC(&ebis);
				//bFirstKeyUpPad = 1 ;
			}
			else
			{
				//to idle state
				EBInputMethodInit(&ebis);
			}
		}
	#endif
	}
	else
	{
		if (GbIMEHasInput())
		{
			if (GbIMEGetInputStrLen()== 2) 
			{
				GbIMESetCandidateInfo(GB_CANDIDATE_LEN, GB_CANDIDATE_COUNT);
			}
			else
			{
				GbIMESetCandidateInfo(18, 1);
			}
			
			GbIMEDeleteInputChar();

		#ifdef __MMI_GB_V5__
			//V5 handle this in GbIMEDeleteInputChar automatically
		#else
			EnFirstHeader(&enis);
			EnFirstPage(&enis);
		#endif
		}
		else
		{
			GB_Reset();
			idxCandidate = 0;
			gbstate = GBIMESTATE_IDLE;
		}
	}
}


void GB_Reset(void)
{	
#ifdef __MMI_GB_V5__
	GBReset(&g_gbis);
#else
	EnReset(&enis);
	enis.header[0] = 0;
	EBInputReset(&ebis);
#endif
	gbstate = GBIMESTATE_IDLE;
	idxCandidate = 0;
	GbInputBoxScreenStateChange();

}

void GBDelectcharater(void)
{
	TBM_ENTRY(0x2870);
	if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG))
	{
		coolsand_UI_EMS_input_box_backspace(&MMI_EMS_inputbox);
		wgui_EMS_text_format=MMI_EMS_inputbox.text_format;
	}
	else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG))
	{
		coolsand_UI_multi_line_input_box_delete_character(&MMI_multiline_inputbox);
		//setup the inline edit changed flag
		inline_full_screen_edit_changed = 1;
	}
	else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_SINGLELINE_INPUTBOX_FLAG))
	{
		coolsand_UI_single_line_input_box_delete_character(&MMI_singleline_inputbox);
	}
	else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_INLINE_MULTILINE_INPUTBOX_FLAG))
	{
		coolsand_UI_multi_line_input_box_delete_character(&MMI_multiline_inputbox);
	}
	TBM_EXIT(0x2870);
}

void GBSetInputboxLSKFunction(FuncPtr f)
{
	gb_inputbox_LSK_function = f;
}

void GbInputMethodExit(void)
{

	EBLong h;
	mmi_trace(g_sw_HandW," ---------GbInputMethodExit------------   ");
#if defined(MEM_ULC_3216)
	GB_Reset();
#endif
	coolsand_UI_lock_double_buffer();
	if ((!(GbInputMethodCategoryFlag & GB_IN_CATEGORY_INLINE_MULTILINE_INPUTBOX_FLAG)))
	{
		set_left_softkey_label((UI_string_type)get_string(STR_GLOBAL_OPTIONS));
		if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG))
		{
			if (coolsand_UI_multi_line_input_box_get_text_length(&MMI_multiline_inputbox) > 0)
			{
				if (wgui_inputbox_not_empty_callback != NULL)
				{
					wgui_inputbox_not_empty_callback();
				}
			}
			else
			{
				if (wgui_inputbox_empty_callback != NULL)
				{
					wgui_inputbox_empty_callback();
				}
			}
		}
	}
	set_left_softkey_icon(NULL);
	redraw_left_softkey();
	register_left_softkey_handler();
	set_left_softkey_function(gb_inputbox_LSK_function, KEY_EVENT_UP);
	set_left_softkey_function(UI_dummy_function, KEY_EVENT_LONG_PRESS);

	//re-register the LSK and RSK functions and redraw the screen
	if (GbInputMethodCategoryFlag & GB_IN_CATEGORY_5_FLAG)
	{
		h = gbMultilineEditorHeight;
		/*zhoumn Add Start For Bug 6408 Ver: TBM_780  on 2007-8-24 14:5 */
#ifdef __MMI_TOUCH_SCREEN__
		if (MMI_GUI_VIRTUAL_KEYBOARD_TRAY != MMI_virtual_keyboard.lang_type)
		{
			h -= (MMI_virtual_keyboard.height -16); //-16 是因为本身高度高度设计上留出了一部分
		}
#endif
		/*zhoumn Add End  For Bug 6408 Ver: TBM_780  on 2007-8-24 14:5 */
		MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_WORD_HIGHLIGHT;
		//wgui_handle_inputbox_input();
		GBHideCompositionArea();
		category5_hide_multitap();
		set_left_softkey_label((UI_string_type)get_string(STR_GLOBAL_OPTIONS));
		set_left_softkey_icon(NULL);
		redraw_left_softkey();
		register_left_softkey_handler();
		set_left_softkey_function(gb_inputbox_LSK_function, KEY_EVENT_UP);
		/*zhoumn Add Start For Bug 5918 Ver: TBM_780  on 2007-8-14 15:23 */
		GbInputSetRSK();
		/*zhoumn Add End  For Bug 5918 Ver: TBM_780  on 2007-8-14 15:23 */
		resize_multiline_inputbox(MMI_multiline_inputbox.width, MMI_multiline_inputbox.height);
		show_multiline_inputbox();
	}
	else if (GbInputMethodCategoryFlag & GB_IN_CATEGORY_28_FLAG)
	{
		h = gbEMSEditorHeight;
	//wangzl:add B
	#ifdef __MMI_TOUCH_SCREEN__
		if (MMI_GUI_VIRTUAL_KEYBOARD_TRAY != MMI_virtual_keyboard.lang_type)
		{
			h -= (MMI_virtual_keyboard.height -16); //-16 是因为本身高度高度设计上留出了一部分
		}
	#endif
	//wangzl:add E
		GBHideCompositionArea();
		category5_hide_multitap();
		set_left_softkey_label((UI_string_type)get_string(STR_GLOBAL_OPTIONS));
		set_left_softkey_icon(NULL);
		redraw_left_softkey();
		register_left_softkey_handler();
		set_left_softkey_function(gb_inputbox_LSK_function, KEY_EVENT_UP);
		/*zhoumn Add Start For Bug 5918 Ver: TBM_780  on 2007-8-14 15:23 */
		GbInputSetRSK();//zhoumn
		/*zhoumn Add End  For Bug 5918 Ver: TBM_780  on 2007-8-14 15:23 */
		MMI_EMS_inputbox.flags|=(UI_EMS_INPUT_BOX_DISABLE_DRAW);

		coolsand_UI_resize_EMS_input_box(&MMI_EMS_inputbox, MMI_EMS_inputbox.width, MMI_EMS_inputbox.height);
		coolsand_UI_show_EMS_input_box(&MMI_EMS_inputbox);

	}
	else if ((GbInputMethodCategoryFlag & GB_IN_CATEGORY_SINGLELINE_INPUTBOX_FLAG))
	{
		MMI_singleline_inputbox.flags &= ~UI_SINGLE_LINE_INPUT_BOX_WORD_HIGHLIGHT;
		wgui_handle_singleline_inputbox_input();
		show_singleline_inputbox();
	}
	else if(GbInputMethodCategoryFlag & GB_IN_CATEGORY_INLINE_MULTILINE_INPUTBOX_FLAG)
	{
		MMI_multiline_inputbox.flags &= ~UI_MULTI_LINE_INPUT_BOX_WORD_HIGHLIGHT;
		wgui_handle_inputbox_input();
		show_multiline_inputbox();
	}
	if(NULL != gb_inputbox_ENTER_function)
	{
		SetKeyHandler(gb_inputbox_ENTER_function, KEY_ENTER, KEY_EVENT_UP);
		gb_inputbox_ENTER_function = NULL;
	}
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(MMI_multiline_inputbox_x,
	MMI_multiline_inputbox_y,
	UI_device_width,
	UI_device_height);
}



BOOL gbIsInput(void)
{
	return((gbstate == GBIMESTATE_INPUT)||(gbstate == GBIMESTATE_ASSATION));
}

//wangzl: add B
#ifdef __MMI_TOUCH_SCREEN__
void gb_register_arrow_handler(void)
{
	GbClearKeyNavigationHandler();

	SetKeyHandler(GbKeyArrowLeftHandler,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyArrowRightHandler,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyArrowUPHandler,KEY_UP_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GbKeyArrowDownHandler,KEY_DOWN_ARROW,KEY_EVENT_DOWN);
	
}
BOOL gb_is_pen_in_composition_area(mmi_pen_point_struct pos)
{
	if((GBIMESTATE_INPUT == gbstate)&&((pos.y > gbCompositionArea.y)&&(pos.y<=(gbCompositionArea.y+gbCompositionArea.height))))
	{
		return MMI_TRUE;
	}
	return MMI_FALSE;
}

BOOL gb_is_pen_in_candidate_area(mmi_pen_point_struct pos)
{
	mmi_trace(g_sw_HandW,"gb_is_pen_in_candidate_area pos.x is:%d,pos.y is:%d",pos.x,pos.y);
	if(((GBIMESTATE_INPUT == gbstate)||(GBIMESTATE_ASSATION == gbstate))&&((pos.y >= gbCandidateArea.y)&&(pos.y<(gbCandidateArea.y+gbCandidateArea.height))))
	{
		return MMI_TRUE;
	}
	return MMI_FALSE;

}

void gb_set_syllable_by_index(U8 index)
{
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO ||GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		if(ebis.syllableLen > 0)
		{
			ebis.syllableIndex = (index)% (ebis.syllableLen + 1);
			EBFindFirstPageC(&ebis);
			idxCandidate = 0 ;
		}
	}
	coolsand_UI_single_line_input_box_next(&gbCandidateArea);
	GbDisplayForEmsEditor();
}

void gb_input_character_by_candidate_index(U8 index)
{
	mmi_trace(g_sw_HandW,"gb_input_character_by_candidate_index  index is:%d",index);
	switch(gbstate)
	{
		case GBIMESTATE_INPUT:
		case GBIMESTATE_ASSATION:
			idxCandidate = index;
			break;
		default:
			break;
	}
	//add by chenq for bug 6660 B
	GbKeyLSKPressHandler();
	//add by chenq for bug 6660 E
}

void gb_set_candidate_by_index(U8 index)
{
	switch(gbstate)
	{
		case GBIMESTATE_INPUT:
		case GBIMESTATE_ASSATION:
			idxCandidate = index;
			break;
		default:
		break;
	}
	coolsand_UI_lock_double_buffer();
	switch (GBIMEmethod)
	{
		case INPUT_MODE_SM_PINYIN:
			if(GBIMESTATE_INPUT == gbstate)
			{
				GBDrawPinyin() ;
			}
			GBDrawChinese();
			break;
	#if defined(__MMI_GB_TR_CHINESE__)
		case INPUT_MODE_TR_BOPOMO:
			if(GBIMESTATE_INPUT == gbstate)
			{
				GBDrawbpmf() ;
			}
			GBDrawChinese();
			break;
	#endif
		case INPUT_MODE_SM_STROKE:
			if(GBIMESTATE_INPUT == gbstate)
			{
				GBDrawStroke();
			}
			GBDrawChinese();
			break;
		//add by chenq for bug 6660 B

	#if defined(__MMI_MULTITAP_FRENCH__)
		case INPUT_MODE_MULTITAP_UPPERCASE_FRENCH:
		case INPUT_MODE_MULTITAP_LOWERCASE_FRENCH:
	#endif
	#if defined(__MMI_SMART_FRENCH__)
		case INPUT_MODE_SMART_UPPERCASE_FRENCH:
		case INPUT_MODE_SMART_LOWERCASE_FRENCH:
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
	#if defined(__MMI_SMART_ITALIAN__)
		case INPUT_MODE_SMART_UPPERCASE_ITALIAN:
		case INPUT_MODE_SMART_LOWERCASE_ITALIAN:
	#endif
	#if defined(__MMI_SMART_GERMAN__)				
		case INPUT_MODE_SMART_UPPERCASE_GERMAN:
		case INPUT_MODE_SMART_LOWERCASE_GERMAN:
	#endif
	#if defined(__MMI_SMART_HINDI__)
		case INPUT_MODE_SMART_HINDI:	
	#endif
	#if defined(__MMI_SMART_TELUGU__)
		case INPUT_MODE_SMART_TELUGU:	
	#endif
	#if defined(__MMI_SMART_SPANISH__)				
		case INPUT_MODE_SMART_UPPERCASE_SPANISH:
		case INPUT_MODE_SMART_LOWERCASE_SPANISH:
	#endif
	#if defined(__MMI_SMART_ARABIC__)
		case INPUT_MODE_SMART_ARABIC:
	#endif
	#if defined(__MMI_SMART_PERSIAN__)
		case INPUT_MODE_SMART_PERSIAN:
	#endif
	#if defined(__MMI_SMART_THAI__)
		case INPUT_MODE_SMART_THAI:
	#endif
	#if defined(__MMI_SMART_VIETNAMESE__)
		case INPUT_MODE_SMART_UPPERCASE_VIETNAMESE:
		case INPUT_MODE_SMART_LOWERCASE_VIETNAMESE:
	#endif
	#if defined(__MMI_SMART_RUSSIAN__)
		case INPUT_MODE_SMART_UPPERCASE_RUSSIAN:
		case INPUT_MODE_SMART_LOWERCASE_RUSSIAN:
	#endif
	#if defined(__MMI_GB_SMART_PORTUGUESE__)
		case INPUT_MODE_SMART_UPPERCASE_PORTUGUESE :
		case INPUT_MODE_SMART_LOWERCASE_PORTUGUESE:
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
		case INPUT_MODE_SMART_UPPERCASE_ABC:
		case INPUT_MODE_SMART_LOWERCASE_ABC:
			if(GBIMESTATE_INPUT == gbstate)
			{
				GBDrawEnglishInputNumber();
			}
			GBDrawEnglish();
			//add by chenq for bug 6660 E
			break;
		default:
			break;
	}
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(MMI_multiline_inputbox_x,
						    MMI_multiline_inputbox_y,
						    UI_device_width,
						    UI_device_height);
}
void gb_display_syllable_by_index(U8 index)
{
	if (GBIMEmethod == INPUT_MODE_TR_BOPOMO || GBIMEmethod == INPUT_MODE_SM_PINYIN || GBIMEmethod == INPUT_MODE_SM_STROKE)
	{
		if(ebis.syllableLen > 0)
		{
			ebis.syllableIndex = (index)% (ebis.syllableLen + 1);
			EBFindFirstPageC(&ebis);
			idxCandidate = 0 ;
		}
	}
	coolsand_UI_lock_double_buffer();
#if defined(__MMI_GB_TR_CHINESE__)
	if(GBIMEmethod == INPUT_MODE_TR_BOPOMO)
		GBDrawbpmf();
	else
#endif
		GBDrawPinyin();
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(MMI_multiline_inputbox_x,
							MMI_multiline_inputbox_y,
							UI_device_width,
							UI_device_height);
}
static U8 current_page=0;
s32 mmi_ime_get_candidates_by_phrase(UI_character_type phrase,UI_character_type *candidatebuffer)
{
	s32 num = 0;
	U16 i, j;

	if(mmi_pen_editor_is_arraw_able())
	{
		ebis.candidateDesireLen = PEN_EDITOR_CANDIDATE_SIZE_WITH_ARROWS;
		ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
	}
	else
	{
		ebis.candidateDesireLen = GB_CH_CANDIDATE_COUNT;
		ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
	}

	EBAssocAddHistoryISN(&ebis, &phrase, 1);
	EBHelperEBISRestart(&ebis);
	EBAssocFirstPageC(&ebis);

	for (i = 0; i < ebis.candidateLen && num < PEN_EDITOR_MAX_CANDIDATE_SIZE; i++)
	{
		for (j = 0; j < ebis.cis[i].len; j++)
		{
			*(candidatebuffer++) = ebis.candidates[num++];
		}

	}

	mmi_trace(g_sw_IM, "MMI_IM: mmi_ime_get_candidates_by_phrase return num is:%d", num);
	current_page = 0;
	ebis.candidateDesireLen = GB_CH_CANDIDATE_COUNT;
	ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
	return num;
}

extern BOOL mmi_pen_editor_vk_delete_last_character(void);
s32 mmi_ime_get_candidates_by_composition(UI_character_type *composition,UI_character_type *candidatebuffer)
{
	s32 num = 0;
	U16 i=0, j;
	U16 indexValue = 0;
	EBAssocAddHistoryISN(&ebis, composition, 1);
	EBHelperEBISRestart(&ebis);

	idxCandidate = 0;
	ebis.candidateDesireLen = PEN_EDITOR_CANDIDATE_SIZE_WITH_ARROWS;
	ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
	for (i=0; '\0' != composition[i]; i++)
	{
		if (EBHelperAddInputChar(&ebis, composition[i] ) == EB_OK)
		{
			if (!GbIMEIsInvalidInput())
			{
				GbIMEDeleteInputChar();
				EBFindFirstPageC(&ebis);
				mmi_pen_editor_vk_delete_last_character();
			}
			mmi_trace(g_sw_IM, TSTXT("ebis.candidateLen=%d .\n"),ebis.candidateLen );
		}
	}

	for (i = 0; i < ebis.candidateLen && num < PEN_EDITOR_MAX_CANDIDATE_SIZE; i++)
	{
		for (j = 0; j < ebis.cis[i].len; j++)
		{
			*(candidatebuffer++) = ebis.candidates[num++];
		}
	}
	mmi_trace(g_sw_IM, "MMI_IM: mmi_ime_get_candidates_by_composition return num is:%d", num);
	current_page = 0;
	ebis.candidateDesireLen = GB_CH_CANDIDATE_COUNT;
	ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
	return num;
}

S32 mmi_ime_get_candidates_by_page(S32 page, UI_character_type *candidatebuffer)
{
	U16 i,j;
	s32 num = 0;
	if(mmi_pen_editor_is_arraw_able())
	{
		ebis.candidateDesireLen = PEN_EDITOR_CANDIDATE_SIZE_WITH_ARROWS;
		ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
	}
	else
	{
		ebis.candidateDesireLen = GB_CH_CANDIDATE_COUNT;
		ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
	}

	if(current_page > page)   
	{
		while(current_page!=page)   
		{
			if (GbIMECanPageUp())   
			{
			#ifdef __MMI_GB_V5__
				GbIMEGetPrevPageCandidate();
			#else
				EBPrevPageC(&ebis);
			#endif
				current_page--;
			} 
			else   
			{
				break;
			}
		}
		if(current_page > page)
			return 0;
	}

	if(current_page < page)   
	{
		while(current_page!=page)   
		{
			if (GbIMECanPageDown())   
			{
			#ifdef __MMI_GB_V5__
				GbIMEGetNextPageCandidate();
			#else
				EBNextPageC(&ebis);
			#endif
				current_page++;
			} 
			else   
			{
				break;
			}
		}
		if(current_page > page)
			return 0;
	}		

	for (i = 0; i < ebis.candidateLen && num < PEN_EDITOR_MAX_CANDIDATE_SIZE; i++)
	{
		for (j = 0; j < ebis.cis[i].len; j++)
		{
			*(candidatebuffer++) = ebis.candidates[num++];
		}
	}
	ebis.candidateDesireLen = GB_CH_CANDIDATE_COUNT;
	ebis.interpunctionDesireLen = GB_CH_SYMBOL_CANDIDATE_COUNT;
	return num;
}
#endif
//wangzl:add E

#ifdef __GB_CANDIDATE_AREA_WITH_ARROW__
/**************************************************************

    FUNCTION NAME    	: gb_candidate_area_left_arrow_pen_up_handler

	  PURPOSE    			:pen up on candidate area left arrow
    INPUT PARAMETERS    : nil

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/
void gb_candidate_area_left_arrow_pen_up_handler(void)
{
	//add by chenq for bug 6660 B
	if((idxCandidate<=0)&&(!GbIMECanPageUp()))
	{
		return;
	}
	//add by chenq for bug 6660 E
	GbKeyArrowLeftHandler();
}

/**************************************************************

    FUNCTION NAME    	: gb_candidate_area_right_arrow_pen_up_handler

	  PURPOSE    			:pen up on candidate area right arrow
    INPUT PARAMETERS    : nil

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/
void gb_candidate_area_right_arrow_pen_up_handler(void)
{
	//add by chenq for bug 6660 B
	if((idxCandidate >= GbIMEGetCandidateCount()- 1)&&(!GbIMECanPageDown()))
	{
		return;
	}
	//add by chenq for bug 6660 E
	GbKeyArrowRightHandler();
}
#endif

#ifdef __GB_COMPOSITION_AREA_WITH_ARROW__
/**************************************************************

    FUNCTION NAME    	: gb_candidate_area_left_arrow_pen_up_handler

	  PURPOSE    			:pen up on candidate area left arrow
    INPUT PARAMETERS    : nil

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/
void gb_composition_area_left_arrow_pen_up_handler(void)
{
	if(GbIMEGetSyllableIndex()<=0)
	{
		return;
	}
	else
	{
		if(GbIMEGetSyllableIndex() > 0 && GbIMEGetSyllableNumber()>0)
		{
		#ifdef __MMI_GB_V5__
			GbIMEGetPrevSyllable();
		#else
			ebis.syllableIndex = (ebis.syllableIndex - 1) % (ebis.syllableLen + 1);
			EBFindFirstPageC(&ebis);
		#endif
			idxCandidate = 0;
		}
		coolsand_UI_single_line_input_box_next(&gbCandidateArea);
		GbDisplayForEmsEditor();
	}
}

/**************************************************************

    FUNCTION NAME    	: gb_candidate_area_right_arrow_pen_up_handler

	  PURPOSE    			:pen up on candidate area right arrow
    INPUT PARAMETERS    : nil

    OUTPUT PARAMETERS    : nil

    RETURNS    			: void

 

**************************************************************/
void gb_composition_area_right_arrow_pen_up_handler(void)
{
	if(GbIMEGetSyllableIndex() >= GbIMEGetSyllableNumber())
	{
		return;
	}
	else
	{
		if((GbIMEGetSyllableIndex()< GbIMEGetSyllableNumber())&&(GbIMEGetSyllableNumber() > 0))
		{
		#ifdef __MMI_GB_V5__
			GbIMEGetPrevSyllable();
		#else
			ebis.syllableIndex = (ebis.syllableIndex + 1) % (ebis.syllableLen + 1);
			EBFindFirstPageC(&ebis);
		#endif
			idxCandidate = 0;
		}
		coolsand_UI_single_line_input_box_next(&gbCandidateArea);
		GbDisplayForEmsEditor();
	}
}
//add by chenq for bug 6660 B

U16 GetEnisCandidateCount(void)
{
#ifdef __MMI_GB_V5__
	return GbIMEGetCandidateCount();
#else
	return enis.candidateCount;
#endif
}
//add by chenq for bug 6660 E
#endif

#ifdef __NATIVE_JAVA_NO_TP_IME_SUPPORT__
void Gb_Ime_init_sm(U16 mode)
{

	switch(mode)
	{
		case INPUT_TYPE_SM_PINYIN:
		 	InitGbSMInputMethod();
			GbChangeInputMethod(INPUT_MODE_SM_PINYIN);
			GbRegisterKeyHandler(INPUT_MODE_SM_PINYIN);
			GbInputSetRSK();
			break;
			
			case INPUT_TYPE_SM_STROKE:
		 	InitGbSMInputMethod();
			GbChangeInputMethod(INPUT_MODE_SM_STROKE);
			GbRegisterKeyHandler(INPUT_MODE_SM_STROKE);
			GbInputSetRSK();
			//GbDisplayForEmsEditor();
			break;

		default:
			break;
		
	}
	
	GbClearKeyStarAndPoundHandler();

}
void Gb_Ime_hide_input_area(void)
{
    color_t c;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#define 	Y  (GB_COMPOSITION_Y-MMI_BUTTON_BAR_HEIGHT-1)
#define 	Y2  (GB_COMPOSITION_Y+ MMI_BUTTON_BAR_HEIGHT+1)
#define   X  (3)
#define   X2 (UI_device_width - 4)

#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_lock_double_buffer();
#endif 

    c.r = 255;
    c.g = 255;
    c.b = 255;
    c.alpha = 100;
    gui_reset_clip();
    gui_fill_rectangle(X, Y, X2, Y2, c);

#if(UI_DOUBLE_BUFFER_SUPPORT)
    gui_unlock_double_buffer();
    gui_BLT_double_buffer(X, Y, X2, Y2);
#endif 
}
void Gb_Ime_show_input_box(void)
{
#define 	Y  (GB_COMPOSITION_Y-MMI_BUTTON_BAR_HEIGHT-1)
#define 	Y2  (GB_COMPOSITION_Y+ MMI_BUTTON_BAR_HEIGHT+1)
#define   X  (8)
#define   X2 (UI_device_width - 4)

	gcj_native_input_box.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW;
	gcj_native_input_box.normal_filler = &MMI_GB_COMPOSITION_WINDOW_filler;
	if(gbstate == GBIMESTATE_ASSATION)
	{
		coolsand_UI_move_single_line_input_box(&gcj_native_input_box,X,Y+MMI_BUTTON_BAR_HEIGHT);
	}
	else if(gbstate == GBIMESTATE_INPUT)
	{
		coolsand_UI_move_single_line_input_box(&gcj_native_input_box,X,Y);
	}
	else
		return;
	coolsand_UI_show_single_line_input_box(&gcj_native_input_box);

}
void Gb_Ime_input_box_delete_char_and_show(void)
{
#define 	Y  (GB_COMPOSITION_Y-MMI_BUTTON_BAR_HEIGHT-1)
#define 	Y2  (GB_COMPOSITION_Y)
#define   X  (8)
#define   X2 (UI_device_width - 4)

    color_t c;

	c.r = 255;
	c.g = 255;
	c.b = 255;
	c.alpha = 100;
	gui_reset_clip();
	
	if(gbstate == GBIMESTATE_ASSATION)
	{
		gui_fill_rectangle(X-1,Y+MMI_BUTTON_BAR_HEIGHT-1,X2+1,Y+MMI_BUTTON_BAR_HEIGHT*2+1,c);
	}
	else if(gbstate == GBIMESTATE_INPUT)
	{
		gui_fill_rectangle(X-1, Y-1, X2+1, Y+MMI_BUTTON_BAR_HEIGHT+1, c);
	}
	

	//gcj_native_input_box.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_CURSOR_DRAW;
	gcj_native_input_box.normal_filler = &MMI_GB_COMPOSITION_WINDOW_filler;
	if(gbstate == GBIMESTATE_ASSATION)
	{
		coolsand_UI_single_line_input_box_delete_current_character(&gcj_native_input_box);
		coolsand_UI_show_single_line_input_box(&gcj_native_input_box);
	}

}

#endif
