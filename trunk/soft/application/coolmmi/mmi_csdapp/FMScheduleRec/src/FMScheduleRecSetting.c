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
 *   FMScheduleRecSetting.c
 *
 * Project:
 * --------
 *   Maui_sw
 *
 * Description:
 * ------------
 *  .............
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MMI_FMSRSETTING_C
#define _MMI_FMSRSETTING_C

#include "mmi_features.h" 

#ifdef __MMI_FM_RADIO_SCHEDULE_REC__

//#undef __MMI_FM_RADIO__

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "debuginitdef.h"	/*debug info*/
#include "commonscreens.h"
#include "settingprofile.h"
#include "wgui_categories_inputsenum.h"
#include "wgui_categories_inputs.h"
#include "nvram_user_defs.h"
#include "nvramprot.h"
#include "datetimegprot.h"
//... Add More MMI header

#include "alarmframeworkprot.h"
#include "fs_type.h"
#include "fs_func.h"
#include "filemanagerdef.h"
#include "filemanagergprot.h"
#include "filemgr.h"
#include "fs_errcode.h"

#include "fmschedulerecdef.h"
#include "fmschedulerectype.h"
#include "fmschedulerecprot.h"

#ifdef MMI_ON_HARDWARE_P
#include "rtc_sw.h"
#endif

#if defined(__MMI_FM_RADIO__)
#include "fmradiodef.h"
#include "fmradiotype.h"
#include "fmradioprot.h"
#endif

#include "usbdevicegprot.h"
#undef __NEWSIMULATOR 

/*  Include: PS header file */
//... Add More PS header


/* 
** Typedef 
*/

typedef enum
{
	FMSR_SETTING_STATE,
	FMSR_SETTING_DATE,
	FMSR_SETTING_TIME,
	FMSR_SETTING_REPEAT_CAP,
	FMSR_SETTING_REPEAT,
	FMSR_SETTING_DAY,
	FMSR_SETTING_CHAN,
	FMSR_SETTING_REC,
	FMSR_SETTING_NUM
}fmsr_setting_item_enum;

typedef enum
{
	FMSR_REC_SETTING_FORMAT_CAP,
	FMSR_REC_SETTING_FORMAT,
	//FMSR_REC_SETTING_STORAGE_CAP,//jiashuo del for jessamine,20080513
	//FMSR_REC_SETTING_STORAGE,//jiashuo del for jessamine,20080513
	FMSR_REC_SETTING_FILE_CAP,
	FMSR_REC_SETTING_FILE_TYPE,
	FMSR_REC_SETTING_FILE_NAME,	
	FMSR_REC_SETTING_NUM
}fmsr_rec_setting_item_enum;

/* 
** Define
*/

/* 
** Local Variable
*/

/*Main List Display Buffer*/
static U16 fmsr_list_highlight;
static U16 fmsr_list_freq[NUM_OF_FMSR];
static U8 fmsr_list_state[NUM_OF_FMSR];

/*Inline Editor Display Buffer*/
static S8 *fmsr_on_off_str[2];		/* array to store on/off state of an alarm */
static S8 *fmsr_repeat_str[FMSR_REPEAT_NUM];		/* array to store frequency type of alarm */
static S8 *fmsr_rec_fomrat_str[FMSR_REC_FORMAT_NUM];
static S8 *fmsr_rec_type_str[FMSR_REC_TYPE_NUM];
static S8 *fmsr_rec_storage_str[FMSR_REC_STORAGE_NUM];
static U8 fmsr_weekday_state[FMSR_NUM_DAYS];			/*signifies the days alarm has been set for in case of freq weekdays*/

static S8 fmsr_day[3*ENCODING_LENGTH];
static S8 fmsr_mon[3*ENCODING_LENGTH];
static S8 fmsr_year[6*ENCODING_LENGTH];

static S8 fmsr_hour1[3*ENCODING_LENGTH];
static S8 fmsr_hour2[3*ENCODING_LENGTH];
static S8 fmsr_min1[3*ENCODING_LENGTH];
static S8 fmsr_min2[3*ENCODING_LENGTH];

static U8 fmsr_rec_format;
static U8 fmsr_rec_storage;
static U8 fmsr_rec_type;
static UI_character_type fmsr_freq_buffer[FMSR_FREQ_DISPLAY_LEN];
//static UI_character_type fmsr_editor_buff[MAX_FMSR_INPUT_FILE_LEN+2];

/* 
** Local Function
*/

/* 
** Global Variable
*/
#if defined(__MMI_FM_RADIO__)
extern mmi_fmrdo_struct g_fmrdo;
#endif
extern 	const U8 gAlmDayOfWeek[];

/* 
** Global Function
*/

#define MMI_FMSR_INIT
/*****************************************************************************
* FUNCTION
*  mmi_fmsr_init_setting
* DESCRIPTION
*   Init function for FM schedule record settings
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_init_setting( void )
{
	/*Set highlight Handler*/
	SetHiliteHandler( MENU_ID_FMSR_MAIN, mmi_fmsr_highlight_app );

	SetHiliteHandler( MENU_ID_FMSR_CHAN_LIST, mmi_fmsr_highlight_chan_list );
	SetHiliteHandler( MENU_ID_FMSR_CHAN_MANUAL, mmi_fmsr_highlight_chan_manual );	
	//SetHiliteHandler( MENU_ID_FMSR_EDIT_FILENAME_DONE, mmi_fmsr_highlight_rec_edit_done );
	//SetHiliteHandler( MENU_ID_FMSR_EDIT_FILENAME_INPUT, mmi_fmsr_highlight_rec_edit_input_method );	
}

#define MMI_FMSR_MAIN_LIST
/*****************************************************************************
* FUNCTION
*  mmi_fmsr_highlight_app
* DESCRIPTION
*   Highlight function for FM schedule record settings
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_highlight_app(void)
{
	SetLeftSoftkeyFunction(mmi_fmsr_pre_entry_main_list,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	
	SetKeyHandler(mmi_fmsr_pre_entry_main_list,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_pre_entry_main_list
* DESCRIPTION
*   Pre-entry function for FM schedule record settings
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_pre_entry_main_list(void)
{
	S16 pError;
	U16 i;
	S8 buffer[NVRAM_EF_FMSR_SETTING_SIZE];
	fmsr_nvram_struct* data_ptr;
	
	//jiashuo add for jessamine 20080514,only card disc can use
	if( MMI_CARD_DRV <= 0 )
	{
		DisplayPopup((PU8)GetString(STR_ID_FMSR_NO_CARD) ,IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
		return;
	}

	/*Make sure structure size is smaller than NVRAM storage size*/
	MMI_ASSERT(sizeof(fmsr_nvram_struct) < NVRAM_EF_FMSR_SETTING_SIZE);

	/*Get the frequency and state for the main list, default is '0xff' for each byte if empty*/
	for(i = 0; i < NUM_OF_FMSR; i++) 
	{
		ReadRecord(NVRAM_EF_FMSR_SETTING_LID, (U16)(i+1), buffer, NVRAM_EF_FMSR_SETTING_SIZE, &pError);
		data_ptr = (fmsr_nvram_struct*)buffer;

		fmsr_list_freq[i] = data_ptr->freq;
		fmsr_list_state[i] = (data_ptr->state == 0xff) ? 0 : data_ptr->state;
	}

	mmi_fmsr_entry_main_list();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_entry_main_list
* DESCRIPTION
*   Entry main screen of schedule FM record settings
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_entry_main_list(void)
{
	U8* guiBuffer;
	U16 i;
	UINT8 tmp[32];
	
	/*Check if in USB mode before enter setting*/
#if defined(__USB_IN_NORMAL_MODE__)
	if(mmi_usb_is_in_mass_storage_mode())
	{
		mmi_usb_app_unavailable_popup(0);
		return;
	}
#endif

	EntryNewScreen(SCR_ID_FMSR_MAIN_LIST, NULL, mmi_fmsr_entry_main_list, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_FMSR_MAIN_LIST);

	if(guiBuffer == NULL)
	{
		fmsr_list_highlight = 0;
	}

	/* construct displayed string and hint */
	for(i = 0; i < NUM_OF_FMSR; i++) 
	{
		if( (fmsr_list_freq[i] >= FMSR_MIN_FREQUENCY) && (fmsr_list_freq[i] <= FMSR_MAX_FREQUENCY))
		{
			//kal_wsprintf( (UI_string_type)subMenuData[i], "FM %d.%d", fmsr_list_freq[i]  / 10, fmsr_list_freq[i]  % 10 );
			sprintf( tmp, "FM %d.%d", fmsr_list_freq[i]  / 10, fmsr_list_freq[i]	% 10 );
			AnsiiToUnicodeString(subMenuData[i],tmp);
		}
		else
		{
			pfnUnicodeStrcpy((PS8)subMenuData[i], GetString(STR_GLOBAL_EMPTY_LIST));
		}

		if(fmsr_list_state[i] == 0)
		{
			pfnUnicodeStrcpy((PS8)hintData[i], GetString(STR_GLOBAL_OFF));
		}
		else
		{
			pfnUnicodeStrcpy((PS8)hintData[i], GetString(STR_GLOBAL_ON));
		}
		
		hintDataPtrs[i] = hintData[i];
	}

	RegisterHighlightHandler(mmi_fmsr_main_list_highlight_hdlr);
	ShowCategory53Screen(STR_ID_FMSR_APP, 
									GetRootTitleIcon(MENU_ID_FMSR_MAIN),
									STR_GLOBAL_EDIT, 
									IMG_GLOBAL_YES,
									STR_GLOBAL_BACK, 
									IMG_GLOBAL_BACK,
									NUM_OF_FMSR,
									subMenuDataPtrs,
									(U16*)gIndexIconsImageList,
									hintDataPtrs,
									0,
									(S32)fmsr_list_highlight,
									guiBuffer);

	SetLeftSoftkeyFunction(mmi_fmsr_main_setting_prepare_inline_data,KEY_EVENT_UP);	
	SetKeyHandler(mmi_fmsr_main_setting_prepare_inline_data,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);	
	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);	
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_list_highlight_hdlr
* DESCRIPTION
*   highlight handler for main schedule FM record screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_main_list_highlight_hdlr(S32 index)
{
	fmsr_list_highlight = (U16)index;
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_list_update_state
* DESCRIPTION
*   change main list state after recording.
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_main_list_update_state(U16 index, MMI_BOOL on_off)
{
    if(on_off)
    {
        fmsr_list_state[index] = 1;    
    }
    else
    {
        fmsr_list_state[index] = 0;        
    }
}

#define MMI_FMSR_MAIN_SETTING
/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_setting_prepare_inline_data
* DESCRIPTION
*  Prepare data for setting
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_main_setting_prepare_inline_data(void)
{
	S16 pError;
	S8 buffer[NVRAM_EF_FMSR_SETTING_SIZE];
	fmsr_nvram_struct* data_ptr;

	ReadRecord(NVRAM_EF_FMSR_SETTING_LID, (U16)(fmsr_list_highlight+1), buffer, NVRAM_EF_FMSR_SETTING_SIZE, &pError);
	data_ptr = (fmsr_nvram_struct*)buffer;

	g_fmsr_cntx.setting.onoff_state = (S32)(data_ptr->state == 0xff) ? 0 : data_ptr->state;

	mmi_fmsr_util_get_date_from_int(fmsr_year, fmsr_mon, fmsr_day, data_ptr->year, data_ptr->mon, data_ptr->day);
	mmi_fmsr_util_get_time_from_int(fmsr_hour1, fmsr_min1, fmsr_hour2, fmsr_min2, data_ptr->hour1, data_ptr->min1,data_ptr->hour2, data_ptr->min2);

	g_fmsr_cntx.setting.repeat = (S32)(data_ptr->repeat >= FMSR_REPEAT_NUM)? FMSR_REPEAT_ONCE : data_ptr->repeat;

	mmi_fmsr_util_get_weekday_from_int(fmsr_weekday_state, data_ptr->weekday);
	g_fmsr_cntx.setting.is_DOW_set = (g_fmsr_cntx.setting.repeat == FMSR_REPEAT_DAYS) ? 1 : 0;

	g_fmsr_cntx.setting.freqency = data_ptr->freq;
	g_fmsr_cntx.setting.format = (S32)(data_ptr->format >= FMSR_REC_FORMAT_NUM) ? FMSR_FORMAT_AMR : data_ptr->format;
	g_fmsr_cntx.setting.storage = (S32)(data_ptr->storage >= FMSR_REC_STORAGE_NUM) ? FMSR_STORAGE_PHONE : data_ptr->storage;
	g_fmsr_cntx.setting.type = (S32)(data_ptr->type >= FMSR_REC_TYPE_NUM) ? FMSR_TYPE_NEW : data_ptr->type;

	g_fmsr_cntx.setting.is_changed = 0;

	/*File path*/
	if((data_ptr->filename[0] != 0xffff) && (pfnUnicodeStrlen((S8*)data_ptr->filename) > 0))
	{	
		mmi_fmsr_util_get_full_path((UI_string_type)g_fmsr_cntx.setting.full_path, (UI_string_type)data_ptr->filename, (U8)g_fmsr_cntx.setting.storage, (U8)g_fmsr_cntx.setting.format);
	}
	else
	{
		memset(g_fmsr_cntx.setting.full_path, 0, 2);
	}

	mmi_fmsr_entry_main_setting();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_setting_get_string
* DESCRIPTION
*   Get string display data before enter screen. 
*   (Need to get every time because language may change)
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_main_setting_get_string(void)
{
	/*Inline Editor Selection Item*/
	fmsr_on_off_str[0] = GetString(STR_GLOBAL_OFF);
	fmsr_on_off_str[1] = GetString(STR_GLOBAL_ON);

	fmsr_repeat_str[FMSR_REPEAT_ONCE] = GetString(STR_ID_FMSR_ONCE);
	fmsr_repeat_str[FMSR_REPEAT_EVERYDAY] = GetString(STR_ID_FMSR_EVERYDAY);
	fmsr_repeat_str[FMSR_REPEAT_DAYS] = GetString(STR_ID_FMSR_DAYS);
	fmsr_repeat_str[FMSR_REPEAT_WEEKLY] = GetString(STR_ID_FMSR_WEEKLY);
	fmsr_repeat_str[FMSR_REPEAT_MONTHLY] = GetString(STR_ID_FMSR_MONTHLY);

	fmsr_rec_fomrat_str[FMSR_FORMAT_AMR] = GetString(STR_ID_FMSR_AMR);//change record type from amr to mp3 for jessamine
	fmsr_rec_fomrat_str[FMSR_FORMAT_WAV] = GetString(STR_ID_FMSR_WAV);

	fmsr_rec_type_str[FMSR_TYPE_NEW] = GetString(STR_ID_FMSR_NEW_FILE);
	fmsr_rec_type_str[FMSR_TYPE_APPEND] = GetString(STR_ID_FMSR_APPEND_FILE);

	fmsr_rec_storage_str[FMSR_STORAGE_PHONE] = GetString(STR_ID_FMSR_PHONE);
	fmsr_rec_storage_str[FMSR_STORAGE_CARD] = GetString(STR_ID_FMSR_CARD);		
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_entry_main_setting
* DESCRIPTION
*   enter main setting screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_entry_main_setting(void)
{
	U8* guiBuffer;
	U8* inputBuffer;					//added for inline edit history
	U16 inputBufferSize;				//added for inline edit history
	U16 inline_icon_list[FMSR_SETTING_NUM] = { IMG_GLOBAL_L1, IMG_GLOBAL_L2, 0, IMG_GLOBAL_L3, 0, 0, IMG_GLOBAL_L4, IMG_GLOBAL_L5 };

	EntryNewScreen(SCR_ID_FMSR_MAIN_SETTING, mmi_fmsr_exit_main_setting, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_FMSR_MAIN_SETTING);
	InitializeCategory57Screen();

	RegisterHighlightHandler(mmi_fmsr_main_setting_highlight_hdlr);

	/*Fill each inline editor item*/
	mmi_fmsr_main_setting_get_string();
	mmi_fmsr_main_setting_fill_inline_item();
	
	inputBuffer = GetCurrNInputBuffer (SCR_ID_FMSR_MAIN_SETTING, &inputBufferSize);    //added for inline edit history
	
	if(inputBuffer != NULL)
	{
		SetCategory57Data(wgui_inline_items, FMSR_SETTING_NUM, inputBuffer);
	}

	ShowCategory57Screen(STR_ID_FMSR_APP, 
									GetRootTitleIcon(MENU_ID_FMSR_MAIN),
									STR_GLOBAL_DONE,
									IMG_GLOBAL_OK,
									STR_GLOBAL_BACK,
									IMG_GLOBAL_BACK,
									FMSR_SETTING_NUM,
									inline_icon_list,
									wgui_inline_items,
									0,
									guiBuffer);

	if(g_fmsr_cntx.setting.is_changed)
	{
		g_fmsr_cntx.setting.is_changed = 0;
		set_wgui_inline_list_menu_changed();
	}

	SetCategory57RightSoftkeyFunctions(mmi_fmsr_main_setting_save_confirm,GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_exit_main_setting
* DESCRIPTION
*   exit main setting screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_exit_main_setting(void)
{
	history_t   Scr; 
	S16 nHistory = 0;
	U16 inputBufferSize;                //added for inline edit history
 	
	Scr.scrnID = SCR_ID_FMSR_MAIN_SETTING;
	CloseCategory57Screen();
	Scr.entryFuncPtr = mmi_fmsr_entry_main_setting;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();	//added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer);		//added for inline edit history
	AddNHistory(Scr, inputBufferSize);				//added for inline edit history

}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_setting_date_callback
* DESCRIPTION
*   show date string callback
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_main_setting_date_callback(U8 *DateStr, U8 *day,U8 *month,U8 *year)
{
	pfnUnicodeStrcpy((S8*)DateStr,(S8*)year);
	pfnUnicodeStrcat((S8*)DateStr,(S8*)"/");
	pfnUnicodeStrcat((S8*)DateStr,(S8*)month);
	pfnUnicodeStrcat((S8*)DateStr,(S8*)"/");
	pfnUnicodeStrcat((S8*)DateStr,(S8*)day);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_setting_time_callback
* DESCRIPTION
*   show time string callback
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_main_setting_time_callback(U8* string,U8* hour1,U8* min1,U8* AM_PM_flag1,U8* hour2,U8* min2,U8* AM_PM_flag2)
{
	pfnUnicodeStrcpy((S8*)string,(S8*)hour1);
	pfnUnicodeStrcat((S8*)string,(S8*)":");
	pfnUnicodeStrcat((S8*)string,(S8*)min1);
	pfnUnicodeStrcat((S8*)string,(S8*)" ");
	pfnUnicodeStrcat((S8*)string,(S8*)"-");
	pfnUnicodeStrcat((S8*)string,(S8*)" ");
	pfnUnicodeStrcat((S8*)string,(S8*)hour2);
	pfnUnicodeStrcat((S8*)string,(S8*)":");
	pfnUnicodeStrcat((S8*)string,(S8*)min2);

}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_setting_repeat_hdlr
* DESCRIPTION
*   repeat field highlight handler
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_main_setting_repeat_hdlr(S32 index)
{
	if(index == FMSR_REPEAT_DAYS) 
	{
		if (g_fmsr_cntx.setting.is_DOW_set == 0 )
		{
			memset( (fmsr_weekday_state+1), 1, 5);
				
			g_fmsr_cntx.setting.is_DOW_set = 1;		
			EnableInlineItem(&wgui_inline_items[FMSR_SETTING_DAY],FMSR_SETTING_DAY);/* multi select days*/
		}
	}
	else
	{
		g_fmsr_cntx.setting.is_DOW_set = 0;		
		memset(fmsr_weekday_state,0,FMSR_NUM_DAYS);
		DisableInlineItem(&wgui_inline_items[FMSR_SETTING_DAY],FMSR_SETTING_DAY);/* multi select days*/
	}

	RedrawCategory57Screen();
}	

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_setting_fill_inline_item
* DESCRIPTION
*   Fill the inline item for main setting screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_main_setting_fill_inline_item(void)
{
	UINT8 tmp[32];
	/*-- ON/OFF --*/
	SetInlineItemActivation(&(wgui_inline_items[FMSR_SETTING_STATE]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);	
	SetInlineItemSelect(&(wgui_inline_items[FMSR_SETTING_STATE]),2,(U8**)fmsr_on_off_str,&g_fmsr_cntx.setting.onoff_state);
	DisableInlineItemHighlight(&(wgui_inline_items[FMSR_SETTING_STATE]));

	/*-- Date --*/
	SetInlineItemActivation(&wgui_inline_items[FMSR_SETTING_DATE], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemDate(&wgui_inline_items[FMSR_SETTING_DATE],(U8*)fmsr_day,(U8*)fmsr_mon,(U8*)fmsr_year, mmi_fmsr_main_setting_date_callback);
	DisableInlineItemHighlight(&wgui_inline_items[FMSR_SETTING_DATE]);
	set_inline_date_boundary(FMSR_MAX_YEAR, FMSR_MIN_YEAR, 12, 1);

	/*-- Time --*/
	SetInlineItemActivation(&(wgui_inline_items[FMSR_SETTING_TIME]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemTimePeriod(&(wgui_inline_items[FMSR_SETTING_TIME]),(U8*)fmsr_hour1, (U8*)fmsr_min1, NULL,(U8*)fmsr_hour2, (U8*)fmsr_min2, NULL, mmi_fmsr_main_setting_time_callback);
	DisableInlineItemHighlight(&wgui_inline_items[FMSR_SETTING_TIME]);

	/*-- Repeat Caption --*/
	SetInlineItemActivation(&(wgui_inline_items[FMSR_SETTING_REPEAT_CAP]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemCaption(&(wgui_inline_items[FMSR_SETTING_REPEAT_CAP]),(U8*)GetString(STR_ID_FMSR_REPEAT));	

	/*-- Repeat --*/
	SetInlineItemActivation(&(wgui_inline_items[FMSR_SETTING_REPEAT]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemSelect(&(wgui_inline_items[FMSR_SETTING_REPEAT]),FMSR_REPEAT_NUM,(U8**)fmsr_repeat_str,&g_fmsr_cntx.setting.repeat);
	RegisterInlineSelectHighlightHandler(&wgui_inline_items[FMSR_SETTING_REPEAT], mmi_fmsr_main_setting_repeat_hdlr);

	/*-- Day of Week --*/
	SetInlineItemActivation(&(wgui_inline_items[FMSR_SETTING_DAY]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemDOWSelect(&(wgui_inline_items[FMSR_SETTING_DAY]),0,fmsr_weekday_state);

	if(g_fmsr_cntx.setting.repeat != FMSR_REPEAT_DAYS)	/* not Days */
	{
		//memset(fmsr_weekday_state,0,FMSR_NUM_DAYS);
		DisableInlineItem(&wgui_inline_items[FMSR_SETTING_DAY], FMSR_SETTING_DAY);/* multi select days*/		
	}
	#ifdef __MMI_MAINLCD_220X176__	
	DisableInlineItemHighlight(&(wgui_inline_items[FMSR_SETTING_DAY]));
	#endif	

	/*-- Channel Setting --*/
	SetInlineItemActivation(&wgui_inline_items[FMSR_SETTING_CHAN], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);

	if( (g_fmsr_cntx.setting.freqency >= FMSR_MIN_FREQUENCY) &&( g_fmsr_cntx.setting.freqency <= FMSR_MAX_FREQUENCY))
	{
		//kal_wsprintf( (UI_string_type)fmsr_freq_buffer, "FM %d.%d", g_fmsr_cntx.setting.freqency / 10, g_fmsr_cntx.setting.freqency  % 10 );
		sprintf( tmp, "FM %d.%d", g_fmsr_cntx.setting.freqency / 10, g_fmsr_cntx.setting.freqency  % 10 );
		AnsiiToUnicodeString((S8*)fmsr_freq_buffer,(S8*)tmp);
		SetInlineItemDisplayOnly(&wgui_inline_items[FMSR_SETTING_CHAN],(U8*)fmsr_freq_buffer);
	}
	else
	{
		SetInlineItemDisplayOnly(&wgui_inline_items[FMSR_SETTING_CHAN],(U8*)GetString(STR_ID_FMSR_CHAN_SETTING));
	}

	/*-- Record Setting --*/
	SetInlineItemActivation(&wgui_inline_items[FMSR_SETTING_REC], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemDisplayOnly(&wgui_inline_items[FMSR_SETTING_REC],(U8*)GetString(STR_ID_FMSR_REC_SETTING));
	

}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_setting_highlight_hdlr
* DESCRIPTION
*   main setting screen highlight handler
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_main_setting_highlight_hdlr(S32 index)
{
	if(index == FMSR_SETTING_CHAN)
	{
		ChangeLeftSoftkey(STR_GLOBAL_EDIT, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_fmsr_entry_chan_setting, KEY_EVENT_UP);
	}
	else if(index == FMSR_SETTING_REC)
	{
	 	ChangeLeftSoftkey(STR_GLOBAL_EDIT, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction(mmi_fmsr_rec_setting_prepare_inline_data, KEY_EVENT_UP);
	}
	else if(index == FMSR_SETTING_DAY)
	{
		return;
	}
	else
	{
		ChangeLeftSoftkey(0, 0);
		ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_setting_save_confirm
* DESCRIPTION
*   Save setting confirmation
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_main_setting_save_confirm(void) 
{

	DisplayConfirm(STR_GLOBAL_SAVE, IMG_GLOBAL_YES,
						  STR_GLOBAL_ABORT, IMG_GLOBAL_NO,
					get_string(STR_ID_FMSR_SAVE_CONFIRM),IMG_GLOBAL_INFO,WARNING_TONE);

	SetLeftSoftkeyFunction(mmi_fmsr_main_setting_save_and_set,KEY_EVENT_UP);
	SetRightSoftkeyFunction(mmi_fmsr_main_setting_abort,KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_setting_check_freq_and_file
* DESCRIPTION
*   Check the data correctness before save
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_fmsr_main_setting_check_freq_and_file(void)
{
	/*Check frequency*/
	if( g_fmsr_cntx.setting.freqency < FMSR_MIN_FREQUENCY || g_fmsr_cntx.setting.freqency > FMSR_MAX_FREQUENCY )
	{
		DisplayPopup( (PU8)GetString( STR_ID_FMSR_VALID_FREQUENCY ), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)ERROR_TONE );
		return FALSE;
	}

	/*Check file name, if append, need to select file in advance*/
	if((g_fmsr_cntx.setting.type == FMSR_TYPE_APPEND) &&  pfnUnicodeStrlen((S8*)g_fmsr_cntx.setting.full_path) == 0)
	{
		DisplayPopup( (PU8)GetString( STR_ID_FMSR_FILE_EMPTY ), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)ERROR_TONE );
		return FALSE;
	}
	
	/*Check Is SD card exists if storage is card*/
	if((g_fmsr_cntx.setting.storage == FMSR_STORAGE_CARD) && (!fmgr_is_msdc_present()))
	{
		DisplayPopup( (PU8)GetString( STR_ID_FMSR_NO_CARD), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)ERROR_TONE );
		return FALSE;
	}

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_setting_check_time
* DESCRIPTION
*   Check the data correctness before save
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_fmsr_main_setting_check_time(U16 year, U8 mon, U8 day, U8 hour1, U8 min1, U8 hour2, U8 min2, U8 weekday, U8 repeat)
{
	S8 buffer[NVRAM_EF_FMSR_SETTING_SIZE];
	MYTIME curr_time;
	U32 start_time, end_time;
	fmsr_nvram_struct* data_ptr;
	S16 pError;
	U8 i;

	/*Check Time*/
	start_time = hour1 * 60 + min1;
	end_time = hour2 * 60 + min2;

	if(start_time >= end_time)
	{
		DisplayPopup( (PU8)GetString( STR_ID_FMSR_END_TIME_INCORRECT ), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)ERROR_TONE );
		return FALSE;
	}

	/*Get if later than current time*/
	DTGetRTCTime(&curr_time);
	end_time = curr_time.nHour * 60 + curr_time.nMin;

	if((year < curr_time.nYear ) ||
	  (year == curr_time.nYear && mon < curr_time.nMonth) ||
	  (year == curr_time.nYear && mon == curr_time.nMonth && day < curr_time.nDay) ||
	  (year == curr_time.nYear && mon == curr_time.nMonth && day == curr_time.nDay && start_time <= end_time))
	{
		DisplayPopup( (PU8)GetString( STR_ID_FMSR_START_TIME_INCORRECT ), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)ERROR_TONE );
		return FALSE;
	}

	/*Check if overlap with other setting*/
	for(i = 0 ; i < NUM_OF_FMSR; i++)
	{
		if(i != fmsr_list_highlight)
		{	
			ReadRecord(NVRAM_EF_FMSR_SETTING_LID, (U16)(i+1), buffer, NVRAM_EF_FMSR_SETTING_SIZE, &pError);
			data_ptr = (fmsr_nvram_struct*)buffer;

			/*Enable recording and Time overlap*/
			if((data_ptr->state != 0) && (data_ptr->state != 0xff) && mmi_fmsr_util_check_time_overlap(hour1,min1,hour2,min2, data_ptr->hour1, data_ptr->min1, data_ptr->hour2, data_ptr->min2))
			{
				if(mmi_fmsr_util_check_repeat_overlap(repeat, year,mon , day, weekday, data_ptr->repeat, data_ptr->year, data_ptr->mon, data_ptr->day, data_ptr->weekday))
				{
				
					DisplayPopup( (PU8)GetString( STR_ID_FMSR_TIME_OVERLAP ), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)ERROR_TONE );
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_setting_save_and_set
* DESCRIPTION
*   Save setting data to NVRAM and set it to alarm framework
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_main_setting_save_and_set(void)
{
	S16 pError;
	S8 buffer[NVRAM_EF_FMSR_SETTING_SIZE];
	fmsr_nvram_struct* data_ptr;
	UI_string_type filename;

	/*Enable schedule record, need to check data correctness*/
	if(g_fmsr_cntx.setting.onoff_state)
	{
		/*Check Frequency and File Path*/
		if(!mmi_fmsr_main_setting_check_freq_and_file())
		{
			return;
		}
	}

	data_ptr = (fmsr_nvram_struct*)buffer;

	/*Get Record Time and Check Overlap*/
	mmi_fmsr_util_get_date_from_str(fmsr_year, fmsr_mon, fmsr_day, &data_ptr->year, &data_ptr->mon, &data_ptr->day);
	mmi_fmsr_util_get_time_from_str(fmsr_hour1, fmsr_min1, fmsr_hour2, fmsr_min2, &data_ptr->hour1, &data_ptr->min1,&data_ptr->hour2, &data_ptr->min2);
	data_ptr->repeat = g_fmsr_cntx.setting.repeat;
	mmi_fmsr_util_get_weekday_from_array(fmsr_weekday_state, &data_ptr->weekday);

	if((data_ptr->repeat == FMSR_REPEAT_DAYS) && (data_ptr->weekday == 0))
	{
		data_ptr->repeat = FMSR_REPEAT_ONCE;
	}

	/*Enable schedule record, need to check data correctness*/
	if(g_fmsr_cntx.setting.onoff_state)
	{
		if(!mmi_fmsr_main_setting_check_time(data_ptr->year, data_ptr->mon, data_ptr->day, data_ptr->hour1, data_ptr->min1,data_ptr->hour2, data_ptr->min2, data_ptr->weekday, data_ptr->repeat))
		{
			return;
		}
	}

	/*Get other settings*/
	data_ptr->state = g_fmsr_cntx.setting.onoff_state;
	fmsr_list_state[fmsr_list_highlight] = g_fmsr_cntx.setting.onoff_state;
	
	data_ptr->freq = g_fmsr_cntx.setting.freqency;
	fmsr_list_freq[fmsr_list_highlight]  = g_fmsr_cntx.setting.freqency;
	
	data_ptr->format = g_fmsr_cntx.setting.format;
	data_ptr->storage = 	g_fmsr_cntx.setting.storage;
	data_ptr->type =	g_fmsr_cntx.setting.type;

	/*Get file name*/
	if(g_fmsr_cntx.setting.type == FMSR_TYPE_APPEND)
	{
		filename=mmi_fmsr_util_get_file_name((UI_string_type)g_fmsr_cntx.setting.full_path);
		pfnUnicodeStrncpy((S8*)data_ptr->filename, (S8*)filename, MAX_FMSR_FILE_NAME_LEN*ENCODING_LENGTH);
		mmi_fmsr_util_remove_file_ext((UI_string_type)data_ptr->filename);
	}
	else
	{
		memset(data_ptr->filename, 0 ,2);
	}

	WriteRecord(NVRAM_EF_FMSR_SETTING_LID, (U16)(fmsr_list_highlight+1), buffer, NVRAM_EF_FMSR_SETTING_SIZE, &pError);

	if(data_ptr->state)
	{
		/*Set Alarm*/
		AlmSetAlarm((U8)(ALM_FMSR_START+fmsr_list_highlight));	
	}
	else
	{
		/*Cancel Alarm*/
		AlmCancelAlarm((U8)(ALM_FMSR_START+fmsr_list_highlight));
	}


	DisplayPopup( (PU8)GetString( STR_GLOBAL_DONE ), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)SUCCESS_TONE);
	DeleteScreenIfPresent(SCR_ID_FMSR_MAIN_SETTING);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_main_setting_abort
* DESCRIPTION
*   Abort setting
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_main_setting_abort(void)
{
	mmi_fmsr_go_back_2_history();
}

#define MMI_FMSR_CHANNEL_SETTING
/*****************************************************************************
* FUNCTION
*  mmi_fmsr_highlight_chan_list
* DESCRIPTION
*   highlight function for select channel list inline item
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_highlight_chan_list(void)
{
	SetLeftSoftkeyFunction(mmi_fmsr_entry_chan_list, KEY_EVENT_UP);
	SetKeyHandler(mmi_fmsr_entry_chan_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_highlight_chan_manual
* DESCRIPTION
*   highlight function for manual input inline item
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_highlight_chan_manual(void)
{
	SetLeftSoftkeyFunction(mmi_fmsr_entry_chan_manual, KEY_EVENT_UP);
	SetKeyHandler(mmi_fmsr_entry_chan_manual, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_entry_chan_setting
* DESCRIPTION
*   entry frequency setting screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_entry_chan_setting(void)
{
	U8* guiBuffer;
	U16 nStrItemList[5];	/*Need Expand when has more options.*/
	U16 nNumofItem;

	EntryNewScreen(SCR_ID_FMSR_CHAN_SETTNIG, NULL, mmi_fmsr_entry_chan_setting, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_FMSR_CHAN_SETTNIG);
	nNumofItem = GetNumOfChild(MENU_ID_FMSR_CHAN_SETTING);
	GetSequenceStringIds(MENU_ID_FMSR_CHAN_SETTING, nStrItemList);
	SetParentHandler(MENU_ID_FMSR_CHAN_SETTING);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	
	ShowCategory15Screen(STR_ID_FMSR_CHAN_SETTING, 
									GetRootTitleIcon(MENU_ID_FMSR_MAIN),
									STR_GLOBAL_OK,
									IMG_GLOBAL_OK,
									STR_GLOBAL_BACK,
									IMG_GLOBAL_BACK,
									nNumofItem,
									nStrItemList,
									(U16*)gIndexIconsImageList,
									LIST_MENU,
									0,
									guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);	
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_chan_list_highlight_hdlr
* DESCRIPTION
*   channel list highlight handler
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_chan_list_highlight_hdlr( S32 index )
{
	g_fmsr_cntx.setting.curr_channel = (U16)index;

	if(g_fmrdo.channel_list.freq[index] == 0)
	{
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_LSK,KEY_EVENT_UP);
		ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_SELECT, IMG_GLOBAL_OK);
		SetLeftSoftkeyFunction( mmi_fmsr_chan_list_select, KEY_EVENT_UP );
		SetKeyHandler( mmi_fmsr_chan_list_select, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	}	
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_entry_chan_list
* DESCRIPTION
*   channel list screen, use the chanel in FM radio
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_entry_chan_list(void)
{
#if defined(__MMI_FM_RADIO__)
	U8* gui_buffer;
	S16 error;
	
	EntryNewScreen( SCR_ID_FMSR_CHAN_LIST, NULL, mmi_fmsr_entry_chan_list, NULL );
	
	gui_buffer = GetCurrGuiBuffer( SCR_ID_FMSR_CHAN_LIST );
	if( gui_buffer == NULL )
	{
		/*Use FM radio list before FM radio initialize*/
		if( !g_fmrdo.is_ready )
		{
        #if defined(MMI_ON_HARDWARE_P)		
            ReadRecord( NVRAM_EF_FM_RADIO_LID, 1, (void*)&g_fmrdo.channel_list, NVRAM_EF_FM_RADIO_SIZE, &error );
        #else
            memset(&g_fmrdo.channel_list, 0, sizeof(mmi_fmrdo_channel_list_struct));
            error = 0;
        #endif
		}
	
		g_fmsr_cntx.setting.curr_channel = 0;
	}

	RegisterHighlightHandler( mmi_fmsr_chan_list_highlight_hdlr );
	
	ShowCategory184Screen( STR_ID_FMSR_CHAN_LIST,
									GetRootTitleIcon(MENU_ID_FMSR_MAIN),
									STR_GLOBAL_SELECT,
									IMG_GLOBAL_OK,
									STR_GLOBAL_BACK,
									IMG_GLOBAL_BACK,
									9,
									mmi_fmrdo_channel_list_item,
									mmi_fmrdo_channel_list_item_hint,
									g_fmsr_cntx.setting.curr_channel, 
									gui_buffer );
	
	// register LSK, RSK hanlders
	SetLeftSoftkeyFunction( mmi_fmsr_chan_list_select, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	
	// register left right arrow key handler
	SetKeyHandler( mmi_fmsr_chan_list_select, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );

#else
	DisplayPopup( (PU8)GetString( STR_GLOBAL_EMPTY ), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)WARNING_TONE );
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_chan_list_select
* DESCRIPTION
*   select a channel from channel to record
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_chan_list_select(void)
{
#if defined(__MMI_FM_RADIO__)
	U16 freq = g_fmrdo.channel_list.freq[g_fmsr_cntx.setting.curr_channel];

	if( freq == 0 )
	{
		DisplayPopup( (PU8)GetString( STR_GLOBAL_EMPTY ), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)WARNING_TONE );
	}
	else
	{
		g_fmsr_cntx.setting.freqency = freq;
		g_fmsr_cntx.setting.is_changed = 1;
		GoBackToHistory( SCR_ID_FMSR_MAIN_SETTING );
	}

#endif	
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_entry_chan_manual
* DESCRIPTION
*   Enter manual input screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_entry_chan_manual(void)
{
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
	UINT8 tmp[32];
	
	EntryNewScreen( SCR_ID_FMSR_CHAN_MANUAL, mmi_fmsr_exit_chan_manual, NULL, NULL );
	
	InitializeCategory57Screen();
   
	guiBuffer = GetCurrGuiBuffer( SCR_ID_FMSR_CHAN_MANUAL );
	inputBuffer = GetCurrNInputBuffer( SCR_ID_FMSR_CHAN_MANUAL, &inputBufferSize );

	if( guiBuffer == NULL )
	{
		if( (g_fmsr_cntx.setting.freqency >= FMSR_MIN_FREQUENCY) &&( g_fmsr_cntx.setting.freqency <= FMSR_MAX_FREQUENCY))
		{
			//kal_wsprintf( (UI_string_type)fmsr_freq_buffer, "%d.%d", g_fmsr_cntx.setting.freqency / 10, g_fmsr_cntx.setting.freqency  % 10 );
			sprintf( tmp, "%d.%d", g_fmsr_cntx.setting.freqency / 10, g_fmsr_cntx.setting.freqency  % 10 );	//shizhenghui, 2008-6-16		
			AnsiiToUnicodeString((S8*)fmsr_freq_buffer,(S8*)tmp);
		}
		else
		{
			memset(fmsr_freq_buffer,0,2);
		}
	}

	SetInlineItemActivation( &wgui_inline_items[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemTextEdit( &wgui_inline_items[0], (U8*)fmsr_freq_buffer, FMSR_FREQ_INPUT_LEN, INPUT_TYPE_DECIMAL_NUMERIC );
	LeftJustifyInlineItem( &wgui_inline_items[0] );
	SetInlineItemFullWidth( &wgui_inline_items[0] );
	EnableInlineItemBoundary( &wgui_inline_items[0] );

	DisableCategory57ScreenDone();
	
	// show category screen
	ShowCategory57Screen( STR_ID_FMSR_CHAN_MANUAL,
									GetRootTitleIcon(MENU_ID_FMSR_MAIN),
									STR_GLOBAL_OK,
									IMG_GLOBAL_OK,
									STR_GLOBAL_BACK,
									IMG_GLOBAL_BACK,
									1,
									0,
									wgui_inline_items,
									0,
									guiBuffer );

	SetCategory57LeftSoftkeyFunction( mmi_fmsr_chan_manual_input_LSK );
	SetCategory57RightSoftkeyFunctions( GoBackHistory, GoBackHistory );
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_exit_chan_manual
* DESCRIPTION
*   Exit manual input screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_exit_chan_manual( void )
{
	history_t	h;
	U16 inputBufferSize;

	CloseCategory57Screen();
	h.scrnID = SCR_ID_FMSR_CHAN_MANUAL;
	h.entryFuncPtr = mmi_fmsr_entry_chan_manual;
	GetCategoryHistory( h.guiBuffer );
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data( (U8*) h.inputBuffer );
	AddNHistory( h, inputBufferSize );
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_chan_manual_input_LSK
* DESCRIPTION
*   Check input frequency before exit screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_chan_manual_input_LSK( void )
{
	// calculate frequency number
	U16 freq = mmi_fmrdo_get_freq_from_str( fmsr_freq_buffer );	
	
	// check frequency input
	if(freq == 0xffff) /* invalid decimal digit */
	{
		DisplayPopup( (PU8)GetString( STR_ID_FMRDO_VALID_FM_DECIMAL ), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)WARNING_TONE );
	}	
	else if( freq < FMSR_MIN_FREQUENCY || freq > FMSR_MAX_FREQUENCY ) /* warning invalid frequency*/
	{	
		//memset(fmsr_freq_buffer,0,2);
		DisplayPopup( (PU8)GetString( STR_ID_FMSR_VALID_FREQUENCY ), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME_2000, (U8)WARNING_TONE );
	}
	else
	{
		g_fmsr_cntx.setting.freqency = freq;
		g_fmsr_cntx.setting.is_changed = 1;
		GoBackToHistory( SCR_ID_FMSR_MAIN_SETTING );
	}
}


#define MMI_FMSR_RECORD_SETTING
/*****************************************************************************
* FUNCTION
*  mmi_fmsr_rec_setting_prepare_inline_data
* DESCRIPTION
*   Prepare inline editor data item for record setting
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_rec_setting_prepare_inline_data(void)
{
	/*Check if user change select*/
	fmsr_rec_format = (U8) g_fmsr_cntx.setting.format;
	fmsr_rec_storage = (U8) g_fmsr_cntx.setting.storage;
	fmsr_rec_type = (U8) g_fmsr_cntx.setting.type;

	g_fmsr_cntx.setting.is_changed = 1;
	mmi_fmsr_entry_rec_setting();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_rec_setting_highlight_hdlr
* DESCRIPTION
*   Highlight handler for record setting
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_rec_setting_highlight_hdlr(S32 item_index)
{
	if(item_index == FMSR_REC_SETTING_FILE_NAME)
	{	
		if((g_fmsr_cntx.setting.type == FMSR_TYPE_APPEND) &&
		    (g_fmsr_cntx.setting.format == FMSR_FORMAT_AMR))
		{
			ChangeLeftSoftkey(STR_GLOBAL_SELECT, IMG_GLOBAL_OK);
			SetLeftSoftkeyFunction(mmi_fmsr_rec_select_file, KEY_EVENT_UP);	
		}
		else
		{
			ChangeLeftSoftkey(0, 0);
			ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);		
		}
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_fmsr_entry_rec_setting
* DESCRIPTION
*   Enter record setting screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_entry_rec_setting(void)
{
	U8* guiBuffer;
	U8* inputBuffer;					//added for inline edit history
	U16 inputBufferSize;				//added for inline edit history
	U16 inline_icon_list[FMSR_REC_SETTING_NUM] = { IMG_GLOBAL_L1, 0, IMG_GLOBAL_L2, 0, 0 };//jiashuo change for jessamine,20080513

	EntryNewScreen(SCR_ID_FMSR_REC_SETTING, mmi_fmsr_exit_rec_setting, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_FMSR_REC_SETTING);
	InitializeCategory57Screen();

	RegisterHighlightHandler(mmi_fmsr_rec_setting_highlight_hdlr);

	/*Fill each inline editor item*/
	mmi_fmsr_rec_setting_fill_inline_item();
	
	inputBuffer = GetCurrNInputBuffer (SCR_ID_FMSR_REC_SETTING, &inputBufferSize);    //added for inline edit history
	
	if(inputBuffer != NULL)
	{
		SetCategory57Data(wgui_inline_items, FMSR_REC_SETTING_NUM, inputBuffer);
	}

	ShowCategory57Screen(STR_ID_FMSR_REC_SETTING, 
									GetRootTitleIcon(MENU_ID_FMSR_MAIN),
									STR_GLOBAL_DONE,
									IMG_GLOBAL_OK,
									STR_GLOBAL_BACK,
									IMG_GLOBAL_BACK,
									FMSR_REC_SETTING_NUM,
									inline_icon_list,
									wgui_inline_items,
									0,
									guiBuffer);

	SetCategory57RightSoftkeyFunctions(mmi_fmsr_rec_setting_done,GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_exit_rec_setting
* DESCRIPTION
*   Exit the record setting screen
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_exit_rec_setting(void)
{
	history_t   Scr; 
	S16 nHistory = 0;
	U16 inputBufferSize;                //added for inline edit history
 	
	Scr.scrnID = SCR_ID_FMSR_REC_SETTING;
	CloseCategory57Screen();
	Scr.entryFuncPtr = mmi_fmsr_entry_rec_setting;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();	//added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer);		//added for inline edit history
	AddNHistory(Scr, inputBufferSize);				//added for inline edit history
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_rec_setting_format_hdlr
* DESCRIPTION
*   Inline selector highlight handler for record format
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_rec_setting_format_hdlr(S32 index)
{
	if(fmsr_rec_format != (U8)index)
	{
		fmsr_rec_type = (U8)index;

		if(pfnUnicodeStrlen((S8*)g_fmsr_cntx.setting.full_path))
		{
			memset(g_fmsr_cntx.setting.full_path,0,2);
			//ReConfigureInlineItemDisplayOnly(&(wgui_inline_items[FMSR_REC_SETTING_FILE_NAME]),(U8*)GetString(STR_GLOBAL_EMPTY_LIST));	
			//RedrawCategory57Screen();
			EntryNewScreen(SCR_ID_FMSR_BASE,NULL,NULL,NULL);
			GoBackHistory();
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_rec_setting_storage_hdlr
* DESCRIPTION
*   Inline selector highlight handler for storage
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_rec_setting_storage_hdlr(S32 index)
{
	if(fmsr_rec_storage != (U8)index)
	{
		fmsr_rec_type = (U8)index;

		if(pfnUnicodeStrlen((S8*)g_fmsr_cntx.setting.full_path))
		{
			memset(g_fmsr_cntx.setting.full_path,0,2);
			//ReConfigureInlineItemDisplayOnly(&(wgui_inline_items[FMSR_REC_SETTING_FILE_NAME]),(U8*)GetString(STR_GLOBAL_EMPTY_LIST));	
			//RedrawCategory57Screen();
			EntryNewScreen(SCR_ID_FMSR_BASE,NULL,NULL,NULL);
			GoBackHistory();
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_rec_setting_type_hdlr
* DESCRIPTION
*   Inline selector highlight handler for record type
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_rec_setting_type_hdlr(S32 index)
{
	if(fmsr_rec_type != (U8)index)
	{
		fmsr_rec_type = (U8)index;

		if(pfnUnicodeStrlen((S8*)g_fmsr_cntx.setting.full_path))
		{
			memset(g_fmsr_cntx.setting.full_path,0,2);
			//ReConfigureInlineItemDisplayOnly(&(wgui_inline_items[FMSR_REC_SETTING_FILE_NAME]),(U8*)GetString(STR_GLOBAL_EMPTY_LIST));	
			//RedrawCategory57Screen();
			EntryNewScreen(SCR_ID_FMSR_BASE,NULL,NULL,NULL);
			GoBackHistory();
		}
	}
}	

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_rec_setting_fill_inline_item
* DESCRIPTION
*   Fill the inline item for record setting
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_rec_setting_fill_inline_item(void)
{
	/*-- Format Caption --*/
	SetInlineItemActivation(&(wgui_inline_items[FMSR_REC_SETTING_FORMAT_CAP]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemCaption(&(wgui_inline_items[FMSR_REC_SETTING_FORMAT_CAP]),(U8*)GetString(STR_ID_FMSR_REC_FORMAT));	

	/*-- Format --*/
	SetInlineItemActivation(&(wgui_inline_items[FMSR_REC_SETTING_FORMAT]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);	
	SetInlineItemSelect(&(wgui_inline_items[FMSR_REC_SETTING_FORMAT]),FMSR_REC_FORMAT_NUM,(U8**)fmsr_rec_fomrat_str,&g_fmsr_cntx.setting.format);
	RegisterInlineSelectHighlightHandler(&wgui_inline_items[FMSR_REC_SETTING_FORMAT], mmi_fmsr_rec_setting_format_hdlr);

//jiashuo del for jessamine 
#if 0
	/*-- Storage Caption --*/
	SetInlineItemActivation(&(wgui_inline_items[FMSR_REC_SETTING_STORAGE_CAP]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemCaption(&(wgui_inline_items[FMSR_REC_SETTING_STORAGE_CAP]),(U8*)GetString(STR_ID_FMSR_REC_STORAGE));	

	/*-- Storage --*/
	SetInlineItemActivation(&(wgui_inline_items[FMSR_REC_SETTING_STORAGE]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	if(MMI_CARD_DRV > 0) /*NO SD card Slot*/
	{
		SetInlineItemSelect(&(wgui_inline_items[FMSR_REC_SETTING_STORAGE]),FMSR_REC_STORAGE_NUM,(U8**)fmsr_rec_storage_str,&g_fmsr_cntx.setting.storage);		
	}
	else
	{
		SetInlineItemSelect(&(wgui_inline_items[FMSR_REC_SETTING_STORAGE]),1,(U8**)fmsr_rec_storage_str,&g_fmsr_cntx.setting.storage);
	}
	RegisterInlineSelectHighlightHandler(&wgui_inline_items[FMSR_REC_SETTING_STORAGE], mmi_fmsr_rec_setting_storage_hdlr);
#endif 

	/*-- File Caption --*/
	SetInlineItemActivation(&(wgui_inline_items[FMSR_REC_SETTING_FILE_CAP]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemCaption(&(wgui_inline_items[FMSR_REC_SETTING_FILE_CAP]),(U8*)GetString(STR_ID_FMSR_REC_TYPE));	

	/*-- File Type --*/
	SetInlineItemActivation(&(wgui_inline_items[FMSR_REC_SETTING_FILE_TYPE]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);	
	SetInlineItemSelect(&(wgui_inline_items[FMSR_REC_SETTING_FILE_TYPE]),FMSR_REC_TYPE_NUM,(U8**)fmsr_rec_type_str,&g_fmsr_cntx.setting.type);
	RegisterInlineSelectHighlightHandler(&wgui_inline_items[FMSR_REC_SETTING_FILE_TYPE], mmi_fmsr_rec_setting_type_hdlr);

	/*-- File Name --*/
	SetInlineItemActivation(&(wgui_inline_items[FMSR_REC_SETTING_FILE_NAME]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);

	if(pfnUnicodeStrlen((S8*)g_fmsr_cntx.setting.full_path) > 0)
	{
		SetInlineItemDisplayOnly(&(wgui_inline_items[FMSR_REC_SETTING_FILE_NAME]),(U8*)g_fmsr_cntx.setting.full_path);
		//SetInlineItemCaption(&(wgui_inline_items[FMSR_REC_SETTING_FILE_NAME]),(U8*)g_fmsr_cntx.setting.full_path);	
	}
	else
	{
		SetInlineItemDisplayOnly(&(wgui_inline_items[FMSR_REC_SETTING_FILE_NAME]),(U8*)GetString(STR_GLOBAL_EMPTY_LIST));	
		//SetInlineItemCaption(&(wgui_inline_items[FMSR_REC_SETTING_FILE_NAME]),(U8*)GetString(STR_GLOBAL_EMPTY_LIST));	
	}
	
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_rec_setting_done
* DESCRIPTION
*   Done record setting and go back.
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_rec_setting_done(void)
{
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_rec_select_file
* DESCRIPTION
*  select a file to append from file manager
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_rec_select_file(void)
{
	UI_character_type path[MAX_FMSR_PATH_LEN];
	FMGR_FILTER	filter;

	mmi_fmsr_util_get_storage_path((UI_string_type)path, (U8)g_fmsr_cntx.setting.storage);

	FMGR_FILTER_INIT(&filter);
	//FMGR_FILTER_SET(&filter, FMGR_TYPE_WAV);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_AMR);

	mmi_fmgr_select_path_and_enter(APP_FMSCHEDULEREC, 
												FMGR_SELECT_FILE|FMGR_SELECT_REPEAT, 
												filter, 
												(S8*)path, 
												mmi_fmsr_rec_select_file_done);
   
	ClearKeyHandler( KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_rec_select_file_done
* DESCRIPTION
*   callback for select a file from file manager
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_rec_select_file_done(void* fullname, int is_short)
{
	if( fullname == NULL )
	{
		GoBackHistory();
	}
	else if(pfnUnicodeStrlen((S8*)fullname) > MAX_FM_FULL_NAME_LEN)
	{
		DisplayPopup( (PU8)GetString( STR_ID_FMSR_FILENAME_TOO_LONG ), IMG_GLOBAL_ERROR ,1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE );
	}
	else
	{
		pfnUnicodeStrncpy((S8*)g_fmsr_cntx.setting.full_path, (S8*)fullname, MAX_FM_FULL_NAME_LEN);
		fmgr_reset_app_select();	/*Use select_repeat, so clear by application itself.*/
		GoBackToHistory(SCR_ID_FMSR_REC_SETTING);
	}
}

#if 0
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

#define MMI_FMSR_UTIL
/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_get_date_from_int
* DESCRIPTION
*   convert date to string from integer
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_util_get_date_from_int(S8* str_year, S8* str_mon, S8* str_day, U16 year, U8 mon, U8 day)
{
	UINT8 tmp[32];

	if(! ((year >= FMSR_MIN_YEAR) && (year <= FMSR_MAX_YEAR)))
	{
		year = FMSR_MIN_YEAR;
	}

	if( (mon > 12) || (mon < 1) ) //shizhenghui, 2008-6-16
	{
		mon = 1;
	}

	if( (day > 31) || (day < 1) ) //shizhenghui, 2008-6-16
	{
		day = 1;
	}

	//kal_wsprintf( (UI_string_type)str_year, "%4d", year);
	//kal_wsprintf( (UI_string_type)str_mon, "%2d", mon);
	//kal_wsprintf( (UI_string_type)str_day, "%2d", day);

	sprintf( tmp, "%4d", year);
	AnsiiToUnicodeString(str_year,tmp);
	sprintf( tmp, "%02d", mon);
	AnsiiToUnicodeString(str_mon,tmp);
	sprintf( tmp, "%02d", day);
	AnsiiToUnicodeString(str_day,tmp);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_get_date_from_str
* DESCRIPTION
*   convert date to integer from string
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_util_get_date_from_str(S8* str_year, S8* str_mon, S8* str_day, U16 *year, U8 *mon, U8 *day)
{
	S8 ascii_buff[6];

	UnicodeToAnsii(ascii_buff, str_year);
	*year = (U16)atoi(ascii_buff);
	UnicodeToAnsii(ascii_buff, str_mon);
	*mon = (U8)atoi(ascii_buff);
	UnicodeToAnsii(ascii_buff, str_day);
	*day = (U8)atoi(ascii_buff);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_get_time_from_int
* DESCRIPTION
*   convert time to string from integer
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_util_get_time_from_int(S8* str_hour1, S8* str_min1, S8* str_hour2, S8* str_min2, U8 hour1, U8 min1, U8 hour2, U8 min2)
{
	UINT8 tmp[32];
	
	if(hour1 > 24)
	{
		hour1 = 0;
	}
	
	if(hour2 > 24)
	{
		hour2 = 0;
	}
	
	if(min1 > 60)
	{
		min1 = 0;
	}

	if(min2 > 60)
	{
		min2 = 0;
	}

	//kal_wsprintf( (UI_string_type)str_hour1, "%2d", hour1);
	//kal_wsprintf( (UI_string_type)str_hour2, "%2d", hour2);
	//kal_wsprintf( (UI_string_type)str_min1, "%2d", min1);
	//kal_wsprintf( (UI_string_type)str_min2, "%2d", min2);

	sprintf( tmp, "%02d", hour1);
	AnsiiToUnicodeString(str_hour1,tmp);
	sprintf( tmp, "%02d", hour2);
	AnsiiToUnicodeString(str_hour2,tmp);
	sprintf( tmp, "%02d", min1);
	AnsiiToUnicodeString(str_min1,tmp);
	sprintf( tmp, "%02d", min2);
	AnsiiToUnicodeString(str_min2,tmp);
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_get_time_from_str
* DESCRIPTION
*   convert time to integer from string
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_util_get_time_from_str(S8* str_hour1, S8* str_min1, S8* str_hour2, S8* str_min2, U8 *hour1, U8 *min1, U8 *hour2, U8 *min2)
{
	S8 ascii_buff[6];

	UnicodeToAnsii(ascii_buff, str_hour1);
	*hour1 = (U8)atoi(ascii_buff);
	UnicodeToAnsii(ascii_buff, str_min1);
	*min1 = (U8)atoi(ascii_buff);
	UnicodeToAnsii(ascii_buff, str_hour2);
	*hour2 = (U8)atoi(ascii_buff);
	UnicodeToAnsii(ascii_buff, str_min2);
	*min2 = (U8)atoi(ascii_buff);	
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_get_weekday_from_int
* DESCRIPTION
*   convert weekday to array from integer
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_util_get_weekday_from_int(U8* weekday_state, U8 weekday)
{
	U16 i;

	memset(weekday_state,0,FMSR_NUM_DAYS);
	if(weekday != 0xff)
	{
		for(i = 0 ; i < FMSR_NUM_DAYS ; i++)
		{
			if(weekday & gAlmDayOfWeek[i])
			{
				weekday_state[i] = 1;
			}
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_get_weekday_from_array
* DESCRIPTION
*   convert weekday to an integer from array
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_util_get_weekday_from_array(U8* weekday_state, U8* weekday)
{
	U16 i;

	*weekday = 0;

	for(i = 0 ; i < FMSR_NUM_DAYS ; i++)
	{
		if(weekday_state[i] == 1)
		{
			*weekday = *weekday | gAlmDayOfWeek[i];
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_get_storage_path
* DESCRIPTION
*  Get storage disc and folder
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_util_get_storage_path(UI_string_type path, U8 storage)
{
#ifdef MMI_ON_HARDWARE_P
	S16 drv;
	UINT8 tmp[64];
//jiashuo for jessamine ,20080514
#if 0	
	if(storage == FMSR_STORAGE_CARD)
	{
		drv = MMI_CARD_DRV;
	}
	else
	{
		drv = MMI_PUBLIC_DRV;
	}
#else
	drv = MMI_CARD_DRV;
#endif 

	//kal_wsprintf((kal_wchar*)path, "%c:\\%w", drv, FMGR_DEFAULT_FOLDER_AUDIO);
	sprintf(tmp,"%c:%c%s",drv,'/',FMGR_DEFAULT_FOLDER_AUDIO);
	AnsiiToUnicodeString((S8*)path,(S8*)tmp);
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_check_storage_folder
* DESCRIPTION
*   check if storage folder exists
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 mmi_fmsr_util_check_storage_folder(UI_string_type path)
{
#ifdef MMI_ON_HARDWARE_P
	FS_HANDLE file_handle;
	S32 result;

	file_handle = MMI_FS_Open((const UINT8 *)path, FS_OPEN_DIR | FS_READ_ONLY );
	if (file_handle < 0) 
	{
		result = MMI_FS_CreateDir((const UINT8 *)path);
		return result;
	}
	else
	{
		MMI_FS_Close(file_handle);
	}
#endif
	return FS_NO_ERROR;

}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_check_file_exist
* DESCRIPTION
*   check if record file exists
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 mmi_fmsr_util_check_file_exist(UI_string_type path)
{
#ifdef MMI_ON_HARDWARE_P
	FS_HANDLE file_handle;

	file_handle = MMI_FS_Open((const UINT8 *)path, FS_READ_ONLY);
	if (file_handle < 0) 
	{
		return file_handle;
	}
	else
	{
		MMI_FS_Close(file_handle);
	}
#endif
	return FS_NO_ERROR;

}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_append_file_ext
* DESCRIPTION
*   append the file extension according to type
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_util_append_file_ext(UI_string_type filename, U8 format)
{
	UINT8 tmpExt[12];
      switch(format)
      {
         case FMSR_FORMAT_AMR:
             AnsiiToUnicodeString(tmpExt,FMSR_FILE_EXT_AMR);
		pfnUnicodeStrcat((S8*)filename, (S8*)tmpExt);           
            break;
#if 0
         case FMSR_FORMAT_MP3:
            //pfnUnicodeStrncat((S8*)filename, (S8*)FMSR_FILE_EXT_MP3, MAX_FMSR_FILE_NAME_LEN);
            AnsiiToUnicodeString(tmpExt,FM_FILE_EXT_MP3);
		 	pfnUnicodeStrcat((S8*)filename, (S8*)tmpExt);
            break;
#endif 

         case FMSR_FORMAT_WAV:
            //pfnUnicodeStrncat((S8*)filename, (S8*)FMSR_FILE_EXT_WAV, MAX_FMSR_FILE_NAME_LEN);
            AnsiiToUnicodeString(tmpExt,FM_FILE_EXT_WAV);
			pfnUnicodeStrcat((S8*)filename, (S8*)tmpExt);
            break;
//#if defined( AMRWB_ENCODE ) && defined( AMRWB_DECODE )
//         case FM_FORMAT_AWB:
//            pfnUnicodeStrncat((S8*)filename, (S8*)FM_FILE_EXT_AWB, MAX_FM_FILE_NAME_LEN);
//            break;
//#endif
      }
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_get_full_path
* DESCRIPTION
*  get the full path for a record file
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_util_get_full_path(UI_string_type full_path, UI_string_type file_name, U8 storage, U8 format)
{
#ifdef MMI_ON_HARDWARE_P
#if 0//jiashuo fix bug for jessamine,20080627
	mmi_fmsr_util_get_storage_path(full_path, storage);
	mmi_fmsr_util_check_storage_folder(full_path);

	pfnUnicodeStrncat((S8*)full_path, (S8*)file_name, MAX_FMSR_FILE_NAME_LEN);
#else
	pfnUnicodeStrcpy((S8*)full_path, (S8*)file_name);
#endif
	mmi_fmsr_util_append_file_ext(full_path, format);
#endif         
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_get_file_name
* DESCRIPTION
*  get file name from full path
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
UI_string_type mmi_fmsr_util_get_file_name(UI_string_type fullname)
{
	UI_string_type p;

	p=fullname+pfnUnicodeStrlen((const S8*)fullname);

	while(p>fullname)
	{
		if (*p==(UI_character_type)_T_MMI'\\')
		{
			return (p+1);
		}
		p--;
	}
	return p;
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_remove_file_ext
* DESCRIPTION
*   remove file extension from file name
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_util_remove_file_ext(UI_string_type filename)
{
	UI_string_type p;

	p=filename+pfnUnicodeStrlen((const S8*)filename);

	while(p>filename)
	{
		if (*p==(UI_character_type)_T_MMI'.')
		{
			*p=0;
			break;
		}
		p--;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_get_new_file
* DESCRIPTION
*   get a new file according to the current time to record
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_util_get_new_file(UI_string_type fullname, U8 format)
{
#ifdef MMI_ON_HARDWARE_P
	UI_character_type filename[MAX_FMSR_FILE_NAME_LEN];
	t_rtc rtc_time;
	U8 count = 0;
	S8 *dest;
	UINT8 tmp[128];

	dest = (S8*)fullname + pfnUnicodeStrlen((S8*)fullname)* ENCODING_LENGTH;

	do 
	{
		RTC_GetTime(&rtc_time);

		//kal_wsprintf((kal_wchar*)filename, "FMR%02d%02d%02d%02d%02d%02d", rtc_time.rtc_mon, rtc_time.rtc_day, rtc_time.rtc_hour, rtc_time.rtc_min, rtc_time.rtc_sec, count);
		//memset(dest,0,2);
		sprintf(tmp, "FMR%02d%02d%02d%02d%02d%02d", rtc_time.rtc_mon, rtc_time.rtc_day, rtc_time.rtc_hour, rtc_time.rtc_min, rtc_time.rtc_sec, count);
		AnsiiToUnicodeString((S8*)filename,(S8*)tmp);
		pfnUnicodeStrncat((S8*)fullname, (S8*)filename, MAX_FMSR_FULL_NAME_LEN);
		
		mmi_fmsr_util_append_file_ext(fullname, format);
		count++;
   	}
	while(mmi_fmsr_util_check_file_exist(fullname) == FS_NO_ERROR);
   
#endif         
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_check_time_overlap
* DESCRIPTION
*   check if two shedule tome overlap
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_fmsr_util_check_time_overlap(U8 shour1, U8 smin1, U8 shour2, U8 smin2, U8 dhour1, U8 dmin1, U8 dhour2, U8 dmin2)
{
	U16 b1,e1,b2,e2;

	b1 = shour1*60 + smin1; /*Begin 1*/
	e1 = shour2*60 + smin2; /*End 1*/
	b2 = dhour1*60 + dmin1; /*Begin 2*/
	e2 = dhour2*60 + dmin2; /*End 2*/

	if(b1 == b2 || e1 == e2)
	{
		return TRUE;
	}
	else if(b1 < b2)
	{
		if(b2 <= e1)
		{
			return TRUE;
		}
	}
	else /*b1 > b2*/
	{
		if(b1 <= e2)
		{
			return TRUE;
		}
	}

	return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_check_repeat_overlap
* DESCRIPTION
*   check if two shedule tome overlap
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_fmsr_util_check_repeat_overlap(U8 repeat1, U16 year1, U8 mon1, U8 day1, U8 weekday1, U8 repeat2,U16 year2, U8 mon2, U8 day2, U8 weekday2)
{
	U32 begin_day1, begin_day2;
	U8 doa1, doa2;
	U8 weekday = 0;

	begin_day1 = year1*10000 + mon1*100 + day1;
	begin_day2 = year2*10000 + mon2*100 + day2;


	switch(repeat1)
	{
		case FMSR_REPEAT_MONTHLY:
			if(repeat2 != FMSR_REPEAT_ONCE)
			{
				return TRUE;
			}
			else if((begin_day1  <= begin_day2) && (day1 == day2)) /*Once*/
			{
				return TRUE;
			}
			break;
		case FMSR_REPEAT_EVERYDAY:
			if(repeat2 != FMSR_REPEAT_ONCE)
			{
				return TRUE;
			}
			else if(begin_day1  <= begin_day2) /*Once*/
			{
				return TRUE;
			}
			break;
		case FMSR_REPEAT_WEEKLY:
			if(repeat2 == FMSR_REPEAT_MONTHLY || repeat2 == FMSR_REPEAT_EVERYDAY)
			{
				return TRUE;
			}
			else if(repeat2 == FMSR_REPEAT_WEEKLY)
			{
				doa1 = DOW(year1,mon1,day1);
				doa2 = DOW(year2,mon2,day2);

				if(doa1 == doa2)
				{
					return TRUE;
				}
			}
			else if(repeat2 == FMSR_REPEAT_DAYS)
			{
				doa1 = DOW(year1,mon1,day1);
				weekday = gAlmDayOfWeek[doa1];

				if(weekday & weekday2)
				{
					return TRUE;
				}
			}
			else if(begin_day1 <= begin_day2) /*Once*/
			{
				doa1 = DOW(year1,mon1,day1);
				doa2 = DOW(year2,mon2,day2);

				if(doa1 == doa2)
				{
					return TRUE;
				}			
			}
			break;

		case FMSR_REPEAT_DAYS:
			if(repeat2 == FMSR_REPEAT_MONTHLY || repeat2 == FMSR_REPEAT_EVERYDAY)
			{
				return TRUE;
			}
			else if(repeat2 == FMSR_REPEAT_WEEKLY)
			{
				doa2 = DOW(year2,mon2,day2);
				weekday = gAlmDayOfWeek[doa2];
				
				if(weekday1 & weekday)
				{
					return TRUE;
				}
			}
			else if(repeat2 == FMSR_REPEAT_DAYS)
			{
				if(weekday1 & weekday2)
				{
					return TRUE;
				}
			}
			else if(begin_day1 <= begin_day2) /*Once*/
			{
				doa2 = DOW(year2,mon2,day2);
				weekday = gAlmDayOfWeek[doa2];
				
				if(weekday1 & weekday)
				{
					return TRUE;
				}
			}

			break;
		default: /*FMSR_REPEAT_ONCE*/
			if(repeat2 == FMSR_REPEAT_MONTHLY)
			{
				if((begin_day2  <= begin_day1) && (day1 == day2))
				{
					return TRUE;
				}				
			}
			else if(repeat2 == FMSR_REPEAT_EVERYDAY)
			{
				if(begin_day2  <= begin_day1)
				{
					return TRUE;
				}
			}
			else if(repeat2 == FMSR_REPEAT_WEEKLY)
			{
				 if(begin_day2 <= begin_day1) /*Once*/
				{
					doa1 = DOW(year1,mon1,day1);
					doa2 = DOW(year2,mon2,day2);

					if(doa1 == doa2)
					{
						return TRUE;
					}
				}
			}
			else if(repeat2 == FMSR_REPEAT_DAYS)
			{
				 if(begin_day2 <= begin_day1) /*Once*/
				{
					doa1 = DOW(year1,mon1,day1);
					weekday = gAlmDayOfWeek[doa1];
				
					if(weekday & weekday2)
					{
						return TRUE;
					}
				}			
			}
			else /*Once */
			{
				if(begin_day1 == begin_day2)
				{
					return TRUE;
				}
			}
			break;
	}

	return FALSE;

}


/*****************************************************************************
* FUNCTION
*  mmi_fmsr_go_back_2_history
* DESCRIPTION
*   Go back two level history when edit done
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_go_back_2_history(void) 
{
	GoBacknHistory(1);
}

#define MMI_FMSR_ALARM_UTIL
/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_get_setting_for_alarm
* DESCRIPTION
*   read setting from NVRAM
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_fmsr_util_get_setting_for_alarm(U16 index, MYTIME *alarmTime, U8 *repeat, U8 * weekday)
{
	S16 pError;
	S8 buffer[NVRAM_EF_FMSR_SETTING_SIZE];
	fmsr_nvram_struct* data_ptr;

	ReadRecord(NVRAM_EF_FMSR_SETTING_LID, (U16)(index+1), buffer, NVRAM_EF_FMSR_SETTING_SIZE, &pError);
	data_ptr = (fmsr_nvram_struct*)buffer;

	alarmTime->nYear = data_ptr->year;
	alarmTime->nMonth = data_ptr->mon;
	alarmTime->nDay = data_ptr->day;
	alarmTime->nHour = data_ptr->hour1;
	alarmTime->nMin = data_ptr->min1;
	alarmTime->nSec = 0;

	*repeat = data_ptr->repeat;
	*weekday = data_ptr->weekday;

}

/*****************************************************************************
* FUNCTION
*  mmi_fmsr_util_get_state_for_reinitialize_alarm
* DESCRIPTION
*   read setting from NVRAM
*
* PARAMETERS
*  void
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
BOOL mmi_fmsr_util_get_state_for_reinitialize_alarm(U8 index)
{
	S16 pError;
	S8 buffer[NVRAM_EF_FMSR_SETTING_SIZE];
	fmsr_nvram_struct* data_ptr;

	ReadRecord(NVRAM_EF_FMSR_SETTING_LID, (U16)(index+1), buffer, NVRAM_EF_FMSR_SETTING_SIZE, &pError);
	data_ptr = (fmsr_nvram_struct*)buffer;

	if(data_ptr->state != 0)
	{
		return TRUE;
	}
	
	return FALSE;
}


#endif /* #ifdef __MMI_FM_RADIO_SCHEDULE_REC__ */
#endif /* #ifndef _MMI_FMSRSETTING_C */
