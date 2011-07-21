#include "kal_non_specific_general_types.h"
#include "kal_release.h"
#include "stack_config.h"
#include "mmi_msg_struct.h"
#include "globaldefs.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "wgui_categories_util.h"
#include "commonscreens.h"
#define MMIAPI_USING_UI
#include "mmiapi_struct.h"
#include "mmiapi.h"
#include "mmiapi_func.h"
#include "mmiapi_res.h"
#include "wgui_categories_inputs.h"
#include "javadef.h"
#include "keybrd.h"
#include "javacall_native_editor.h"

#ifdef __MMI_GB__
extern void GBSetInputboxLSKFunction(FuncPtr f); 
#endif

unsigned short NativeEditorSemaphore;

/////////////////////////////////////////////////////////////////////////////////////
// variables related to fullscreen editor
/////////////////////////////////////////////////////////////////////////////////////
static kal_uint8 *mmiapi_fullscreen_editor_input_buffer;
static int mmiapi_fullscreen_editor_input_buffer_max_len;

/* remember current mmiapi_fullscreen_editor type */
static int mmiapi_fullscreen_editor_texttype;
static kal_bool mmiapi_fullscreen_editor_input_required;
static S32 mmiapi_fullscreen_editor_input_type;

/* disable switching input method */
static kal_bool mmiapi_fullscreen_editor_disable_switch_IM;
static int mmiapi_fullscreen_editor_return_value;

/////////////////////////////////////////////////////////////////////////////////////
// Fullscreen Editor 
/////////////////////////////////////////////////////////////////////////////////////
#define FULLSCREEN_EDITOR_RETURN_NONE  0
#define FULLSCREEN_EDITOR_RETURN_DONE  1
#define FULLSCREEN_EDITOR_RETURN_CANCEL   2

static void mmiapi_goto_fullscreen_editor_option(void);
static void mmiapi_enter_resume_fullscreen_editor(void);

static void javacall_launch_native_editor_callback(void)
{
    sxr_ReleaseSemaphore(NativeEditorSemaphore);
}

void CancelGotoJavaSreen(void)
{
    GoBackToHistory (SCR_JAVA);
    javacall_launch_native_editor_callback();
}

static void JavaEntryInputMethodScreen(void)
{
    EntryInputMethodScreen();
}

void mmiapi_inputBox_highlighthanlder (S32 index)
{
	switch(index)
	{
		case 0:
			SetLeftSoftkeyFunction (CancelGotoJavaSreen, KEY_EVENT_UP ) ;
			break;
			
		case 1:
			SetLeftSoftkeyFunction (CancelGotoJavaSreen, KEY_EVENT_UP ) ;
			break;

		case 2:
			SetLeftSoftkeyFunction (JavaEntryInputMethodScreen, KEY_EVENT_UP ) ;
			break ;
	}
	
}


static void mmiapi_goto_fullscreen_editor_option(void)
{
	U16 item_texts[] = {STR_GLOBAL_DONE, \
						STR_GLOBAL_CANCEL, \
						STR_GLOBAL_INPUT_METHOD} ;
	
	U16 item_icons[] = {IMG_GLOBAL_L1, \
						IMG_GLOBAL_L2, \
						IMG_GLOBAL_L3} ;
	
	EntryNewScreen (MMIAPI_SCREEN_FULLSCREEN_EDITOR_OPTION, 0, 0, 0) ;
    
	RegisterHighlightHandler (mmiapi_inputBox_highlighthanlder);

	ShowCategory52Screen (STR_GLOBAL_OPTIONS, 0, 
							STR_GLOBAL_OK, 0,
							STR_GLOBAL_BACK, 0, 
							3, item_texts, item_icons,
							0, 0, 0, 0);
	
	// register LSK, RSK handlers
	//SetLeftSoftkeyFunction (jddex_inputBoxOption_lskHandler, KEY_EVENT_UP ) ;
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP ) ;
}


static void mmiapi_enter_resume_fullscreen_editor(void)
{
    U8* guiBuffer = NULL;    
    
    EntryNewScreen (MMIAPI_SCREEN_FULLSCREEN_EDITOR, 0, mmiapi_enter_resume_fullscreen_editor, NULL);
    guiBuffer = GetCurrGuiBuffer (MMIAPI_SCREEN_FULLSCREEN_EDITOR);      
    
    ShowCategory5Screen (0, 0,
        STR_GLOBAL_OPTIONS, 0,
        STR_GLOBAL_BACK, 0,
        mmiapi_fullscreen_editor_input_type,
        (U8*) mmiapi_fullscreen_editor_input_buffer,
        mmiapi_fullscreen_editor_input_buffer_max_len,
        guiBuffer);
    
#ifdef __MMI_GB__
    GBSetInputboxLSKFunction (mmiapi_goto_fullscreen_editor_option);
#endif

    SetLeftSoftkeyFunction (mmiapi_goto_fullscreen_editor_option, KEY_EVENT_UP);
    SetCategory5RightSoftkeyFunction (CancelGotoJavaSreen, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (GoBackHistory );
}

void mmiapi_init_fullscreen_editor(kal_uint8 *src, int maxlen, int type, kal_bool input_required)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmiapi_fullscreen_editor_return_value = FULLSCREEN_EDITOR_RETURN_NONE;

    mmiapi_fullscreen_editor_input_buffer = src;
    mmiapi_fullscreen_editor_input_buffer_max_len = maxlen;
    mmiapi_fullscreen_editor_texttype = type;
    mmiapi_fullscreen_editor_input_required = input_required;
    mmiapi_fullscreen_editor_input_type = type;

    type &= ~INPUT_TYPE_OVERRIDE_DEFAULT;
    switch (type)
    {
        case INPUT_TYPE_ALPHANUMERIC_LOWERCASE:
        case INPUT_TYPE_ALPHANUMERIC_UPPERCASE:
        case INPUT_TYPE_ALPHANUMERIC_PASSWORD:
            mmiapi_fullscreen_editor_disable_switch_IM = KAL_FALSE;
            break;
        default:
            mmiapi_fullscreen_editor_disable_switch_IM = KAL_TRUE;
            break;
    }

}

void mmiapi_enter_fullscreen_editor(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* we only backup the buffer for the first time */
    /* mmiapi_backup_editor_buffer(mmiapi_fullscreen_editor_input_buffer,mmiapi_fullscreen_editor_input_buffer_max_len); */
    mmiapi_enter_resume_fullscreen_editor();
}

void mmiapi_ui_enter_fullscreen_editor_req(mmiapi_ui_enter_fullscreen_editor_req_struct *msg, module_type mod_src)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if 0//JiaShuo Delete
    BLOCKING_INIT(mod_src, msg);
#endif

    mmiapi_init_fullscreen_editor(msg->editor_buffer, msg->maxlen, msg->type, msg->is_must_input);
    ClearKeyEvents();   /* clear all keypad buffer */
    ClearAllKeyHandler();
    mmiapi_enter_fullscreen_editor();
}


int javacall_launch_native_editor(int Constraints, unsigned short* in, int inlen, unsigned short* title, int titlelen, unsigned short* out, int maxoutlen)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
      /************************************************
      IMPORTANT NOTICE:
         Before using this function, remember to pause Java VM.
      *************************************************/
    kal_int32 ime_type = 0;
    int retlen;
    mmiapi_ui_enter_fullscreen_editor_req_struct *msg = NEW_LOCAL_PTR(mmiapi_ui_enter_fullscreen_editor_req_struct);
    
	NativeEditorSemaphore = sxr_NewSemaphore(0);
    SetProtocolEventHandler(
        (PsFuncPtr) mmiapi_ui_enter_fullscreen_editor_req,
        (U16) MSG_ID_MMIAPI_UI_ENTER_FULLSCREEN_EDITOR_REQ);
    
	memset(out, 0, (maxoutlen+1)*2);
    memcpy(out, in, sizeof(unsigned short) * inlen);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (Constraints)
    {
        case EMAILADDR:
            ime_type = INPUT_TYPE_ALPHANUMERIC_LOWERCASE | INPUT_TYPE_USE_ONLY_ENGLISH_MODES;
            break;
            
        case NUMERIC:
            ime_type = INPUT_TYPE_DECIMAL_NUMERIC;
            break;

        case PHONENUMBER:
            ime_type = INPUT_TYPE_PHONE_NUMBER;
            break;

        case URL:
            ime_type = INPUT_TYPE_ALPHANUMERIC_LOWERCASE | INPUT_TYPE_USE_ONLY_ENGLISH_MODES;
            break;

        case PASSWORD:
            ime_type = INPUT_TYPE_ALPHANUMERIC_PASSWORD;
            break;

        case DECIMAL:
            ime_type = INPUT_TYPE_DECIMAL_NUMERIC;
            break;

    }
    ime_type |= INPUT_TYPE_OVERRIDE_DEFAULT;

    msg->blocking_data = NULL;
    msg->editor_buffer = (kal_uint8*) out;
    msg->is_must_input = KAL_FALSE;
    msg->maxlen = maxoutlen + 1;
    msg->type = ime_type;
    
    retlen = kal_wstrlen(( WCHAR * )out);

    mmi_trace(1,"javacall_launch_native_editor befor retlen=%d maxoutlen=%d",retlen,maxoutlen);  
 
    g_jam_vm_state = JVM_BG_BLOCKING_STATE;
    
    SEND_MESSAGE(MOD_J2ME, MOD_MMI, MMI_J2ME_SAP, MSG_ID_MMIAPI_UI_ENTER_FULLSCREEN_EDITOR_REQ, msg, NULL);
    
	sxr_TakeSemaphore(NativeEditorSemaphore);
    sxr_FreeSemaphore(NativeEditorSemaphore);
    
    retlen = kal_wstrlen(( WCHAR * )out);

    mmi_trace(1,"javacall_launch_native_editor after retlen=%d",retlen);

    return retlen;

}

