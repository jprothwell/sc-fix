#include "stddef.h"
#include "stdarg.h"
#include "cos.h"
#include "cswtype.h"
#include "fs_type.h"
#include "tm.h"
#include "mmi_trace.h"
#include "sxr_mem.h"
#include "internal.h"
unsigned char set_InputDone=0;
unicode input_mode_sign[10];


UINT16 inputType_index;

void GoBackHistory(void);


unsigned char inputSemaphore=0;
extern int checkforinputwindow;
unsigned char return_from_native_input=0;
extern unsigned char  inMenu ;

void gcj_StubDisplaySetITextEditer(unsigned char isTextEditer)
{
	if (isTextEditer)
		set_InputDone=1;
	else
		set_InputDone=0;
	
}

void gcj_StubDisplaySetInputMode(INPUT_MODE_T mode)
{
	UINT16 inputMode_index;
	static unsigned int count=0;
	switch (mode)
	{
		case IM_NONE:
			inputMode_index = INPUT_MODE_SM_MULTITAP_PINYIN;
			inputType_index = INPUT_TYPE_SM_PINYIN;
#if defined(__NATIVE_JAVA_NO_TP_IME_SUPPORT__)			
			AnsiiToUnicodeString(input_mode_sign,"PinYin");
#else
			AnsiiToUnicodeString(input_mode_sign,"System");
#endif			
			break;
		case IM_ROMAN_CAPS:
			inputMode_index = INPUT_MODE_MULTITAP_UPPERCASE_ABC;
			inputType_index = INPUT_TYPE_ALPHANUMERIC_UPPERCASE | INPUT_TYPE_USE_ONLY_ENGLISH_MODES;
			AnsiiToUnicodeString(input_mode_sign,"ABC");
			break;
		case IM_ROMAN_SMALL:
			inputMode_index = INPUT_MODE_MULTITAP_LOWERCASE_ABC;
			AnsiiToUnicodeString(input_mode_sign,"abc");
			inputType_index = INPUT_TYPE_ALPHANUMERIC_LOWERCASE | INPUT_TYPE_USE_ONLY_ENGLISH_MODES;
			break;
		case IM_NUMERIC:
			inputMode_index = INPUT_MODE_123;
			AnsiiToUnicodeString(input_mode_sign,"123");
			inputType_index = INPUT_TYPE_NUMERIC;
			break;
		case IM_SYMBOL:
			inputMode_index = INPUT_MODE_123_SYMBOLS;
			AnsiiToUnicodeString(input_mode_sign,"SYM");
			inputType_index = INPUT_TYPE_NONE;
			break;
#if defined(__NATIVE_JAVA_NO_TP_IME_SUPPORT__)						
		case IM_SYM_SM:
			inputMode_index = INPUT_MODE_SM_STROKE;
			AnsiiToUnicodeString(input_mode_sign,"BiHua");
			inputType_index = INPUT_TYPE_SM_STROKE;
			break;
#endif			
		default :
			abort();
	}
	SetInputMethod(inputMode_index);
	//set_InputDone =1;
	/*
	if (inMenu)
	{
		checkforinputwindow=0;
		return_from_native_input=0;
	}
	gcj_TraceOut(0,"gcj_StubDisplaySetInputMode mode=%d,inMenu=%d,checkforinputwindow=%d,return_from_native_input=%d\ ",mode,inMenu,checkforinputwindow,return_from_native_input);
	*/
	gcj_StubForceRefreshDisplay();
}


void GoBackN()
{
    GoBacknHistory(1);
}

void GoBackToJava()
{
	//gcj_StubReleaseSemphone(inputSemaphore);
	GoBackHistory();
	//javanotify_resume();
	//gcj_StubForceRefreshDisplay();
}

int currentIndex=0;
void gcj_get_native_input_options()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (currentIndex)
    {
        case 0: /* Done */
            //ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
            GoBackHistory();
            GoBackJavaEditer();
            break;
        case 1: /* Input Methods */
            EntryInputMethodScreen();
            RegisterInputMethodScreenCloseFunction(GoBackN);
            break;
        default:
            break;

    }
}

void gcj_set_native_input_options(S32 nIndex)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	currentIndex = nIndex;
	SetLeftSoftkeyFunction(gcj_get_native_input_options, KEY_EVENT_UP);
#if 0
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (nIndex)
    {
        case 0: /* Done */
            //ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	    SetLeftSoftkeyFunction(GoBackToJava, KEY_EVENT_UP);
            break;
        case 1: /* Input Methods */
	    EntryInputMethodScreen();
            RegisterInputMethodScreenCloseFunction(GoBackN);
            break;
        default:
            break;

    }
	#endif
}

pBOOL gcj_get_list_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
    U16 nStrItemList[2] = {STR_GLOBAL_DONE,STR_GLOBAL_INPUT_METHOD};
    pfnUnicodeStrcpy( (S8*)str_buff, GetString( nStrItemList[item_index] ) );
    *img_buff_p = (UI_image_type) GetImage( (U16)(IMG_GLOBAL_L1 + item_index) );

    return TRUE;
}


void gcj_entry_native_input_option (void)
{
	gcj_TraceOut(0,"gcj_entry_native_input_option-----------------------------------\n");	

    U8* guiBuffer;

    EntryNewScreen(SCR_JAVA + 2, NULL,
    gcj_entry_native_input_option, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_JAVA + 2);
    

    RegisterHighlightHandler(gcj_set_native_input_options);

	ShowCategory184Screen( STR_GLOBAL_OPTIONS, 0,	// title text and icon
		STR_GLOBAL_OK, 0,							// LSK text and icon
		STR_GLOBAL_BACK, 0, 								// RSK text and icon
		2, gcj_get_list_item, 
		NULL,
		0, guiBuffer );


    //RegisterInputMethodScreenCloseFunction(fmgr_return_to_editor);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_UP);
    ClearKeyHandler(KEY_END,KEY_EVENT_UP);
    ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_END,KEY_EVENT_UP);
    return;

}

unsigned short gcj_InputBuff[256];
unsigned int gcj_InputBuffLen;
extern S16 MMI_prefered_input_type;

extern unsigned char before_dm_redraw_category_screen;
extern unsigned char after_dm_redraw_category_screen;
void GoBackJavaEditer();
#if defined(__NATIVE_JAVA_NO_TP_IME_SUPPORT__)			
void gcj_buffer_insert_char( unsigned short gbchar)
{
	gcj_InputBuff[0]=gbchar;
}
extern FuncPtr currKeyFuncPtrs[MAX_KEYS][MAX_KEY_TYPE];		/* available key func ptrs to current screen  */
void gcj_exit_native_input(void)
{
	//mmi_pen_editor_exit_input_box_nativeqq();
}
void gcj_entry_native_input(int len)
{
	gcj_TraceOut(0,"gcj_entry_native_input---------inputType_index=%d--------------------------\n",inputType_index);	
	U8 *guiBuffer = NULL;
	U16 keyCode, keyType;
	GetkeyInfo(&keyCode, &keyType);
	EntryNewScreen(SCR_JAVA+1,gcj_exit_native_input,NULL,NULL);
	#if 0
	guiBuffer = GetCurrGuiBuffer( SCR_JAVA+1 );
	RegisterInputBoxEmptyFunction(NULL);
	//RegisterInputBoxNotEmptyFunction(jmetoc_not_empty);
	ShowCategory5Screen(0, 0, STR_GLOBAL_OPTIONS, 
	IMG_GLOBAL_OPTIONS, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	inputType_index, (U8*)gcj_InputBuff, len, guiBuffer);
	#endif
	//hide status bar
	entry_full_screen();
	//init ime
	Gb_Ime_init_sm(inputType_index);
	//init tp
	//mmi_pen_editor_setup_input_box_nativeqq();
#if defined(__MMI_GB__)
	GBSetInputboxLSKFunction(GoBackJavaEditer);
#endif
	SetLeftSoftkeyFunction(GoBackJavaEditer,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackJavaEditer, KEY_EVENT_UP);

	//ShowCategory5Screen(0, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, (U8*)jmetoc_input_buf, 100, guiBuffer);
	//SetLeftSoftkeyFunction(jmetoc_confirm_inputForm, KEY_EVENT_UP);
	//SetCategory5RightSoftkeyFunction(GoBackJavaEditer, KEY_EVENT_UP);
	//RegisterInputMethodScreenCloseFunction(GoBackHistory);
	ClearKeyHandler(KEY_END,KEY_EVENT_UP);
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackJavaEditer, KEY_END,KEY_EVENT_UP);
//clear keypound ,no switch when qq,the local only provide sm-pinyin input method
	ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	ClearKeyHandler(KEY_POUND,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackJavaEditer, KEY_POUND,  KEY_EVENT_DOWN);
	//execute the last key handler in input method
	if(currKeyFuncPtrs[keyCode][KEY_EVENT_DOWN])
		currKeyFuncPtrs[keyCode][KEY_EVENT_DOWN]();	//gcj_StubTakeSemphone(inputSemaphore);
}
#else
void gcj_entry_native_input(int len)
{
	gcj_TraceOut(0,"gcj_entry_native_input---------inputType_index=%d--------------------------\n",inputType_index);	
	U8 *guiBuffer = NULL;
	EntryNewScreen(SCR_JAVA+1,NULL,gcj_entry_native_input,NULL);
	guiBuffer = GetCurrGuiBuffer( SCR_JAVA+1 );
	RegisterInputBoxEmptyFunction(NULL);
	//RegisterInputBoxNotEmptyFunction(jmetoc_not_empty);
	ShowCategory5Screen(0, 0, STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, inputType_index, (U8*)gcj_InputBuff, len, guiBuffer);
#if defined(__MMI_GB__)
	GBSetInputboxLSKFunction(gcj_entry_native_input_option);
#endif
	SetLeftSoftkeyFunction(gcj_entry_native_input_option,KEY_EVENT_UP);

	//ShowCategory5Screen(0, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, (U8*)jmetoc_input_buf, 100, guiBuffer);
	//SetLeftSoftkeyFunction(jmetoc_confirm_inputForm, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackJavaEditer, KEY_EVENT_UP);
	//RegisterInputMethodScreenCloseFunction(GoBackHistory);
	ClearKeyHandler(KEY_END,KEY_EVENT_UP);
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackJavaEditer, KEY_END,KEY_EVENT_UP);
	//gcj_StubTakeSemphone(inputSemaphore);
}
#endif
void do_nothing()
{

}
extern multi_line_input_box MMI_multiline_inputbox;
unsigned char entry_native_input=0;


void gcj_StubNativeEditer(int len)
{
	gcj_TraceOut(0,"task id=%d,gcj_StubNativeEditer org len =%d\n",sxr_GetCurrentTaskId(),len);
	if (len >256)
		len = 256;
	COS_EVENT evt;
	_NATIVE_EDITER_T *data = (_NATIVE_EDITER_T *)OslMalloc(sizeof (_NATIVE_EDITER_T));
	if (!data)
		abort();
	data->maxLen = len;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_ENTRY_NATIVE_EDITER_REQ;
	evt.nParam1 = data;
	entry_native_input = 1;
	memset(gcj_InputBuff,0,sizeof(gcj_InputBuff));
	gcj_StubSendEvent(&evt,MOD_MMI);
}

int gcj_StubReadUTF(unsigned short ** buff)
{
	int len;
#if defined(__NATIVE_JAVA_NO_TP_IME_SUPPORT__)				
	len = pfnUnicodeStrlen((S8*)gcj_InputBuff);
#else
	len = (MMI_multiline_inputbox.text_length >> 1)-1;
#endif	
	*buff = gcj_InputBuff;
	entry_native_input = 0;
	//return_from_native_input=1;
	gcj_TraceOut(0,"gcj_StubReadUTF native len=%d\n",len);
	return len;
}
void UI_set_font(UI_font_type f);
extern icontext_button MMI_softkeys[WGUI_MAX_SOFTKEYS];

void GoBackJavaEditer()
{
	int len;
	EVENT_HANDLER_T data;
	data.java_event_type = KEY_EVENT;
	data.java_event_data.keyEventData.keytype = IME;
	data.java_event_data.keyEventData.keycode = 0;
	GoBackHistory();
	UI_set_font(MMI_softkeys[0].text_font);   
	event_action_push(data);
}

unsigned char gcj_IsFormJavaNativeEditer()
{
	return entry_native_input;
}

#include "key_defs.h"
unsigned char isHavaDelKey=0;
unsigned char isHavaCenterKey=0;

unsigned char gcj_IsHavaDelKey()
{
	return isHavaDelKey;
}

void gcj_CheckDelKey()
{
	isHavaDelKey = 0;
	unsigned char keyCode = adp_KeyMapGetKey(KEY_CLEAR);
	if (keyCode != KP_NB)
		isHavaDelKey = tgt_IsHaveKeyCode(keyCode);
	gcj_TraceOut(0,"gcj_CheckDelKey isHavaDelKey=%d,keyCode = %c",isHavaDelKey,keyCode);
}

unsigned char gcj_IsHavaCenterKey()
{
	return isHavaCenterKey;
}

void gcj_CheckCenterKey()
{
	isHavaCenterKey = 0;
	unsigned char keyCode = adp_KeyMapGetKey(KEY_CSK);
	if (keyCode != KP_NB)
		isHavaCenterKey = tgt_IsHaveKeyCode(keyCode);
	gcj_TraceOut(0,"gcj_CheckCenterKey isHavaCenterKey=%d,keyCode= %c",isHavaCenterKey,keyCode);
}


