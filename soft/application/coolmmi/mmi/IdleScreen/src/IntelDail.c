//#include "MMI_include.h"

// added by qinjiangwei begin
#ifdef __MMI_TOUCH_CALCULATOR__
/* start vijay 20050301 foe calculator */
#include "wgui.h"
/* end vijay */
#endif /* __MMI_TOUCH_CALCULATOR__ */ 

#include "gui.h"
#include "settingprofile.h"
#include "profilegprots.h"
//#include "med_utility.h"
#include "gui_themes.h"
#include "wgui_softkeys.h"
#include "phonebooktypes.h"
#include "idleappdef.h"

extern U16 g_phb_name_index[MAX_PB_ENTRIES];   /* Sorting Order by Name Field. */
extern MMI_PHB_ENTRY_BCD_STRUCT PhoneBook[MAX_PB_ENTRIES];

extern U16 PhoneBookEntryCount;
extern pBOOL phonebookReady;
#ifdef WIN32
#include <crtdbg.h>
#endif

#if	1//def __MMI_INTELIGENT_CALL_NUMBER__
///////
/// added by qinjiangwei 2008/5/23 拨号显示备选号码
/////////////////
///

#define DIALER_SCR_ID	SCR_ID_CALC_APP

static S32 total_items;
static UI_character_type *input_buffer;
static S32 input_buffer_len;

#define FIRST_USER_INPUT_INDEX 0


//6223C双卡双待项目上是用智能搜索后无法拨进电话，别的项目电话本较多时，数组溢出，发生任何事情皆有可能。
#define MAX_LIST_ITEM_COUNT (MAX_PB_ENTRIES + 1)                 //(MAX_PB_SIM_ENTRIES + 1)	//zhouhaitai modified 2008-09-09
static S8 *list_cell_text_array[MAX_LIST_ITEM_COUNT];
static S16 list_filter_index_array[MAX_LIST_ITEM_COUNT];
static S8 *pMemory;

extern void mmi_dailer_exit_screen(void);
void mmi_dailer_word_hdlr(S32 index);
static pBOOL mmi_dailer_list_get_item(S32 item_index, UI_string_type str_buff, PU8 *img_buff_p, U8 str_img_mask);
static S32 mmi_dailer_list_get_hint_item(S32 item_index, UI_string_type *hint_array);
//extern U8 mmi_phb_convert_to_digit_by_storage(U8 *dest, U8 *source, U8 max_dest_len, U8 storage);
extern U8 mmi_phb_convert_to_digit(U8 *dest, U8 *source, U8 max_dest_len);
static BOOL init_list_cell_text_array();
static BOOL deinit_list_cell_text_array();
static S32 my_search_func(U8 *keyword);
static void category658_highlight_handler(S32 item_index);

#ifdef __MMI_VOIP__
static S8 CallBuffer[65 * ENCODING_LENGTH];    /* This buffer stores all dialled digits from idle screen */
#else
static  S8 CallBuffer[MAX_DIAL_PAD * ENCODING_LENGTH];
#endif


  void ShowCategory658Screen(
	 U16 title,
	 U16 title_icon,
	 U16 left_softkey,
	 U16 left_softkey_icon,
	 U16 right_softkey,
	 U16 right_softkey_icon,
	 S32 number_of_items,
	 GetItemFuncPtr get_item_func,
	 GetHintFuncPtr get_hint_func,
	 S32 highlighted_item,
	 U16 search_image,
	 U8 *buffer,
	 S32 buffer_max_length,
	 U8 *history_buffer);

///
extern U8 g_dialer_focus_on_inputbox;

void mmi_dialer_entry_screen(U16 scr_id, U16 left_softkey_id, U16 right_softkey_id, U8 *buffer, S32 buff_len)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 *guiBuffer;
	S32 cnt;

	memset(CallBuffer, 0, sizeof(CallBuffer));

	if (g_phb_cntx.phb_ready)
	{
		total_items = PhoneBookEntryCount;
	}
	else
	{
		total_items = 0;
	}
	if (!init_list_cell_text_array())
	{
		return;
	}

	guiBuffer = GetCurrGuiBuffer(scr_id);

	RegisterCat200SearchFunction(my_search_func);
	RegisterHighlightHandler(mmi_dailer_word_hdlr);

	cnt = 0;
	
	input_buffer = (UI_string_type)buffer;
	input_buffer_len = buff_len;

#if 0		//liping 2008-06-11 del
	// 第一次进入该窗口
	if (guiBuffer == NULL)
	{	
		memset(input_buffer, 0, input_buffer_len);
	}
#endif	

//renchangming Bug #10121 待机界面输入号码进行保存时，左软键提示“保存”实际功能为拨号，提示与实际功能不符 	2009-03-30 
#ifdef __NEED_CHANGE_LEFT_KEY_TO_SENTER__
            entry_new_dis_id_left_key_to_senter_init();
#endif
menu_shortcut_handler_display = 1;
	ShowCategory658Screen(
			0,
			0,
			left_softkey_id,
			0,
			right_softkey_id,
			0,
			cnt,
			mmi_dailer_list_get_item,
			NULL,
			0,
			0,
			(U8*)input_buffer,
			input_buffer_len,
			guiBuffer);

	RegisterHighlightHandler(mmi_dailer_word_hdlr);
	
	SetCategory16RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

///
void mmi_dailer_exit_screen(void)
{
	deinit_list_cell_text_array();
}

///
S8 *GetCallBuffer()
{
	return &CallBuffer[0];
}

extern U8 g_dialer_focus_on_inputbox;
extern fixed_list_menu MMI_fixed_list_menu;

void chang_dialer_focus(void)
{
    if(MMI_fixed_list_menu.n_items <= 0)
        return;

    if(g_dialer_focus_on_inputbox)
    {
        g_dialer_focus_on_inputbox = 0;
        register_dynamic_list_keys();
    }
    else
    {
        g_dialer_focus_on_inputbox = 1;
		SetKeyHandler(NULL, KEY_UP_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(NULL, KEY_VOL_UP, KEY_EVENT_DOWN);
		SetKeyHandler(chang_dialer_focus, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
		SetKeyHandler(chang_dialer_focus, KEY_VOL_DOWN, KEY_EVENT_DOWN);
    }
    
    dynamic_list_goto_item_no_redraw(0);
    redraw_dynamic_list();       
}

extern void mmi_dailer_word_hdlr(S32 index);

void mmi_dailer_word_hdlr(S32 index)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	MMI_BOOL result;
	S32 dynamicItems;
    if(IsInDialerSrc() && !IsFocusOnInputBox())
        register_dynamic_list_keys();
    else
    {
        if(g_dialer_focus_on_inputbox)
        {
            SetKeyHandler(NULL, KEY_UP_ARROW, KEY_EVENT_DOWN);
            SetKeyHandler(NULL, KEY_VOL_UP, KEY_EVENT_DOWN);
            SetKeyHandler(chang_dialer_focus, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
            SetKeyHandler(chang_dialer_focus, KEY_VOL_DOWN, KEY_EVENT_DOWN);
        }
        else
        {
            SetKeyHandler(chang_dialer_focus, KEY_UP_ARROW, KEY_EVENT_DOWN);
            SetKeyHandler(chang_dialer_focus, KEY_VOL_UP, KEY_EVENT_DOWN);
            SetKeyHandler(NULL, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
            SetKeyHandler(NULL, KEY_VOL_DOWN, KEY_EVENT_DOWN);                
        }
        
    }
	category658_highlight_handler(index);
}



static S8 * get_num_ptr(S8 *str)
{
	S16 len;
	S16 i;
	S8 *p;

	return str;
}

static BOOL compare_num(S8 *in_str, S8 *key)
{
	S8 *p;

	p = strstr(in_str, key);
	
#if 0
	if (p != in_str)		// 匹配从头开始
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
#endif
	return p != NULL;
}

/// 根据输入的关键字，返回满足条件的所有记录条数 
static S32 my_search_func(U8 *keyword)
{
	S32 i;
	S8 *p;
	S32 ok_counts = 0;
	S8 tmp[256];
	S32 filter_index = 0;
	U8 num_ascii[MAX_PB_NUMBER_LENGTH + 1 + 1];
	S16 store_index;
	S32 item_index;


	mmi_ucs2_to_asc((S8 *)tmp, (S8 *) keyword);
        if(strlen(tmp) == 0)
            return 0;
#if 1
	for (i = 0;  i < /*PhoneBookEntryCount */ total_items; i++)
	{
#if 0//(NEW_DIAL_SCREEN)
		// 用户输入
		if (i == FIRST_USER_INPUT_INDEX)
		{
			list_filter_index_array[filter_index++] = -1;
			ok_counts++;
			
			continue;
		}
#endif
		// 电话本处理
		store_index =   g_phb_name_index[i];//[i - 1];//(NEW_DIAL_SCREEN)
                /* BEGIN: zouxuebin modify for Bug #13623 09-07-23 */
                if(((PhoneBook[store_index].tel.type & 0x90) == 0x90) && (PhoneBook[store_index].tel.type != 0xFF))
                {
                    num_ascii[0] = '+';
                    //mmi_phb_convert_to_digit_by_storage(num_ascii + 1, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1, MMI_NVRAM);
                    mmi_phb_convert_to_digit(num_ascii + 1, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1);

                }
                else
                {
                    //mmi_phb_convert_to_digit_by_storage(num_ascii, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1, MMI_NVRAM);
                    mmi_phb_convert_to_digit(num_ascii, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1);

				}
                /* END: zouxuebin modify */
		if (compare_num((S8 *)num_ascii, (S8 *)tmp))
		{
			ok_counts++;
			list_filter_index_array[filter_index++] = i;//i - 1;	// modified by qinjiangwei 2008/6/6//(NEW_DIAL_SCREEN)
		}
	}

#else
	
	for (i = 0; i < MAX_LIST_ITEM_COUNT; i++)
	{
		p = get_num_ptr(list_cell_text_array[i]);
		if (!p)
		{
			continue;
		}

		if (compare_num(p, tmp))
		{
			ok_counts++;
			list_filter_index_array[filter_index++] = i;
		}
	}
#endif	

#ifdef WIN32
	_RPT1( _CRT_WARN, "ok_counts = %d\n", ok_counts);
#endif
	memset(CallBuffer, 0, sizeof(CallBuffer));
	
	return ok_counts;
}



/// 第一个索引给到用户输入的数字
static BOOL init_list_cell_text_array()
{
	S32 i;
//(NEW_DIAL_SCREEN)
	//list_filter_index_array[FIRST_USER_INPUT_INDEX] = -1;

	// 初始化电话本列表
	for (i = 0/*1*/; i < total_items; i++)
	{
		list_filter_index_array[i] = i;//i - 1;
	}
	
	return MMI_TRUE;	
}


///
static BOOL deinit_list_cell_text_array()
{

	return MMI_TRUE;
}


///
static pBOOL mmi_dailer_list_get_item(S32 item_index, UI_string_type str_buff, PU8 *img_buff_p, U8 str_img_mask)
{
	unsigned short wtmp[50];
	S32 index;
	U8 num_ascii[MAX_PB_NUMBER_LENGTH + 1 + 1];
	U16 num_ascii_wchar[MAX_PB_NUMBER_LENGTH + 1 + 1];
	S16 store_index;


	if (item_index > total_items)
	{		
		return MMI_FALSE;	
	}
#if 0//(NEW_DIAL_SCREEN)
	// 第一个列表元素设为用户输入
	if (item_index == FIRST_USER_INPUT_INDEX)
	{
		mmi_ucs2cpy((S8 *)str_buff, (S8 *)input_buffer);
		*img_buff_p = NULL;

		return MMI_TRUE;
	}
#endif

	// 电话本处理
	index = list_filter_index_array[item_index];
	store_index = g_phb_name_index[index];
	
	if (mmi_ucs2strlen((S8*) (PhoneBook[store_index].alpha_id.name)))
	{	
		// name
		mmi_ucs2cpy((S8*) str_buff, (S8*) (PhoneBook[store_index].alpha_id.name));
		//stretch_eight_chars_width(str_buff);
	
		// phone number
	#if 1   //>>luchongguang mod 2009-07-10 Bug #12887 存储+86的号码，然后待机界面输入号码前几位快速选择呼叫，字符“+”会被去掉，拨打电话失败 
		mmi_phb_convert_get_ucs2_number((S8*) num_ascii_wchar, store_index);
	#else
		mmi_phb_convert_to_digit_by_storage(num_ascii, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1,MMI_NVRAM);
		mmi_asc_to_ucs2((S8 *)num_ascii_wchar, (S8 *)num_ascii);
	#endif  /* Bug #12994 */

		mmi_ucs2cat((S8*) str_buff, (S8 *)L"  ");
		mmi_ucs2cat((S8 *)str_buff, (S8 *)num_ascii_wchar);		
	}
	else
	{
		 mmi_phb_convert_get_ucs2_number((S8*) str_buff, store_index);
	}
	

	*img_buff_p = NULL;

	return MMI_TRUE;
}


///
void category658_highlight_handler(S32 item_index)
{
	S32 index;
	S16 store_index;
	U8 num_ascii[MAX_PB_NUMBER_LENGTH + 1 + 1];


	memset(CallBuffer, 0, sizeof(CallBuffer));
	
	if (item_index > total_items)
	{		
		return ;	
	}

#if 0//(NEW_DIAL_SCREEN)
	if (item_index == FIRST_USER_INPUT_INDEX)
	{
		mmi_ucs2cpy((S8 *)CallBuffer, (S8 *)input_buffer);
		return;
	}
#endif
	index = list_filter_index_array[item_index];
	store_index = g_phb_name_index[index];

        // Bug #13052, 电话簿存储+86的号码，待机界面输入手机号码前几位快速选择呼叫，字符“+”会被去掉，拨打电话失败 ouyangyi 090711
	if (((PhoneBook[store_index].tel.type & 0x90) == 0x90) && (PhoneBook[store_index].tel.type != 0xFF))
	{
		num_ascii[0] = '+';
		//mmi_phb_convert_to_digit_by_storage(num_ascii + 1, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1, MMI_NVRAM);
		mmi_phb_convert_to_digit(num_ascii + 1, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1);

	}
	else
	// End Bug #13052   Bug #12994
	//mmi_phb_convert_to_digit_by_storage(num_ascii, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1, MMI_NVRAM);
	mmi_phb_convert_to_digit(num_ascii, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1);

	mmi_asc_to_ucs2((S8 *)CallBuffer, (S8 *)num_ascii);
}

///
static S32 mmi_dailer_list_get_hint_item(S32 item_index, UI_string_type *hint_array)
{

#if 1
	return 0;
#else
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 i, count, store_index;
	S8 temp_number[30 * ENCODING_LENGTH];
	S8 tmp[30];
	S32 index;
	U8 num_ascii[MAX_PB_NUMBER_LENGTH + 1 + 1];

	if (item_index > total_items)
	{		
		return 0;	
	}

	index = list_filter_index_array[item_index];

#if 1
	mmi_phb_convert_to_digit(num_ascii, PhoneBook[index].tel.number, MAX_PB_NUMBER_LENGTH + 1);
	mmi_asc_to_ucs2((S8*) temp_number, (S8 *)num_ascii);
#else

	for (i = 0; i < 10; i++)
	{
		if (index >= 0 && index <= 9)
			tmp[i] = index + '0';
		else
		{
			S16 c;

			c = index + 'A';
			if (c > 'Z')
			{
				c = 'A';
			}
			
			tmp[i] = c;
		}
	}
	
	tmp[i] = '\0';
	mmi_asc_to_ucs2((S8*) temp_number, tmp);
#endif	
	

	mmi_ucs2cpy((S8*) hint_array[0], (S8*) temp_number);

	return 1;   /* One hint data only, can be more hints. */
#endif	
}


// added by qinjiangwei end
#endif
