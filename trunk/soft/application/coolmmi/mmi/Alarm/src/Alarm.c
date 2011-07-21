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
 *	Alarm.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements Alarm application related screens.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: Alarm.c

  	PURPOSE		: Handling of Alarm Application

 

	 

	DATE		: June 9,03

**************************************************************/
#ifndef _MMI_ALARM_C
#define _MMI_ALARM_C
	#define __NEWSIMULATOR 
#include "mmi_features.h"

/*  Include: MMI header file */
#include "mmi_data_types.h"
#include "globaldefs.h"
#include "protocolevents.h"

#include "timerevents.h"
#include "historygprot.h"
#include "sublcdhistorygprot.h"

#include "wgui_categories.h"
#include "wgui_status_icons.h"
#include "wgui_categories_inputs.h"

#include "gpioinc.h"

#include "alarmprot.h"
#include "alarmdef.h"
#include "alarmgprot.h"
#include "alarmframeworkprot.h"
#include "datetimegprot.h"

#include "filemgr.h"
#include "profilegprots.h"
#include "profilemgrgprot.h"
#include "profiles_prot.h"

#include "keybrd.h"		// for clearkeyevents

#include "unicodexdcl.h"
#include "custdatares.h"

#include "debuginitdef.h"
#include "settingprofile.h"

#include "nvramenum.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "audioinc.h"
#include "timerevents.h"
#include "organizerdef.h"
#include "profilesdefs.h"
#include "wrappergprot.h"
#include "simdetectiongexdcl.h"
#include "commonscreens.h"
#include "wgui_categories_popup.h"
#include "wgui_status_icons.h"

	#undef __NEWSIMULATOR 
#ifdef __MMI_FM_RADIO__
#include "fmradiodef.h"
#include "fmradiotype.h"
#include "fmradioprot.h"
#include "mainmenudef.h"
#include "aud_defs.h"
#include "fs_type.h"
#include "fs_func.h"

extern mmi_fmrdo_struct g_fmrdo; // defined in FMRadioSrc.c
extern pBOOL mmi_fmrdo_channel_list_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );
extern S32 mmi_fmrdo_channel_list_item_hint( S32 item_index, UI_string_type *hint_array );
extern void kal_wsprintf(unsigned short *outstr, char *fmt,...);

#ifdef __MMI_ALM_AUDIO_OPTIONS__

BOOL g_alm_aud_opt_selected = FALSE;
extern U8 isEarphonePlugged;

/*Jinzh Add Start For 6424 Ver: TBM780  on 2007-8-28 17:36 */
extern U8 isEarphonePlugged_1;  
/*Jinzh Add End  For 6424 Ver: TBM780  on 2007-8-28 17:36 */
BOOL g_alm_audio_options_alarm_edited_manually = FALSE;

S8 * g_alm_audio_options_string[NUM_AUDIO_OPTIONS];     /*array to store the audio option strings*/    
U16 g_alm_audio_options_curselitem[MAX_NAME_INPUT_LEN];
S8 g_alm_aud_opt_curselitem_inline_index;
S32 g_alm_audio_option_previndex;                       //the index of the selected audio option i.e. FM or tone.
U8 g_alm_aud_opt_fmrdo_channel_index;
U16 g_alm_aud_opt_fmrdo_channel_opt_index;
U16 g_alm_fmrdo_man_inp_opt_index;
U16 g_alm_fmrdo_prev_freq;
static UI_character_type channel_manual_input_buffer[MAX_MANUAL_INPUT_LEN];
static BOOL g_alm_fmrdo_read_channel_name = FALSE;

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
static PRFSET_MELODY_CONF_TYPE g_alm_tone_conf_type = 0;
static U8 g_ext_melody_idx=0;
static U8 extMldHlitItem = 0;
#endif

static S32 g_alm_tone_index;
extern U8 mmi_profiles_get_melody_list(U16 CurMelodyID, U16 *selecteditem); 
extern U16 gstartRingId;
extern U16 gtotalRingId;
extern U16 gstartMidiId;
extern U16 gtotalMidiId;
extern U16 gcurrentaudioId;
extern PROFILE gcurrentprofile;
#endif /*__MMI_ALM_AUDIO_OPTIONS__*/

#endif /*__MMI_FM_RADIO__*/

/* 
** Define
*/
/* 
** Typedef 
*/

/* 
** Local Variable
*/


/* 
** Local Function
*/

/* 
** Global Variable
*/
alm_context_struct g_alm_cntx;


S8 *gAlmOnOffString[NUM_STATE_ACTINACT_ALARM];		/* array to store on/off state of an alarm */
S8 *gAlmFreqList[NUM_FREQ_ALARM];		/* array to store frequency type of alarm */
U8 gAlmDaysStates[NUM_DAYS];			/*signifies the days alarm has been set for in case of freq weekdays*/

#ifdef __MMI_ALM_CUST_VOLUME__
S8* gAlmVolumeList[LEVEL7+1];
const S8 gAlmVolumeString[LEVEL7+1][4] =
{
	{'1',0,0,0},
	{'2',0,0,0},
	{'3',0,0,0},
	{'4',0,0,0},
	{'5',0,0,0},
	{'6',0,0,0},
	{'7',0,0,0}
};
#endif
#ifdef __MMI_ALM_SNOOZE_SETTING__ //qiff modify:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__
S8 *gAlmSettingSnooze[ALM_SNOOZE_INTERVAL_SETTING_NUM];
#endif 

extern wgui_inline_item wgui_inline_items[];

/* 
** Global Function
*/
extern void AlarmDisplayCharging(void);
extern void AudioPlayToneWithCallBackVolPath(U16 soundId, U8 style, U8 tone_type, U8 volume, U8 path);



/*****************************************************************************
* FUNCTION
*	HighlightAlmMenu
* DESCRIPTION
*   Highlight handler of alarm menu item. 
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightAlmMenu(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(EntryAlmMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(EntryAlmMenu,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif
	/*- zhouqin 2011-02-16 modify for q6 320X240 */	
	g_alm_cntx.CurrHiliteAlarm = 0;
}


/*****************************************************************************
* FUNCTION
*	AlmInit
* DESCRIPTION
*   Initailization function. Set the highlight handlers and get required strings
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmInit(void) 
{
	SetHiliteHandler(ORGANIZER_ALARM_MENU,HighlightAlmMenu);
}


/*****************************************************************************
* FUNCTION
*	AlarmDeInit
* DESCRIPTION
*   Deinitailization function of alarm application.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlarmDeInit()
{
	
}

/*****************************************************************************
* FUNCTION
*	AlmInitNvram
* DESCRIPTION
*   Gets the information from NVRAM
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmInitNvram(void) 
{
	S16 error;
	U8 nvram_buf[NVRAM_ALM_SPOF_DATA_SIZE];
#ifdef __MMI_ALM_AUDIO_OPTIONS__
	int index;
	alm_nvram_struct_extended temp_storage_for_nvram_data;
#else
	int index;
#endif

	ReadRecord(NVRAM_EF_ALM_SPOF_DATA_LID, 1, (void*)nvram_buf, NVRAM_ALM_SPOF_DATA_SIZE, &error);

#ifdef __MMI_ALM_AUDIO_OPTIONS__
	memcpy(&temp_storage_for_nvram_data, nvram_buf, sizeof(alm_nvram_struct_extended));
	for(index = 0; index < NUM_OF_ALM; index++)
	{
		g_alm_cntx.AlmList[index] = temp_storage_for_nvram_data.AlmList[index];
	}
#else
	memcpy(g_alm_cntx.AlmList, nvram_buf, sizeof(g_alm_cntx.AlmList));
#endif

	for(index = 0; index < NUM_OF_ALM; index++)//chenhe to adjust the hour and minute
	{
		g_alm_cntx.AlmList[index]. Hour= g_alm_cntx.AlmList[index]. Hour>24?0:g_alm_cntx.AlmList[index]. Hour;
		g_alm_cntx.AlmList[index]. Min=g_alm_cntx.AlmList[index]. Min>=60?0:g_alm_cntx.AlmList[index]. Min;
		COS_Sleep(100);
		mmi_trace(g_sw_ALARM, "gdm: into AlmInitNvram(): g_alm_cntx.AlmList[%d].Snooze=%d", index, g_alm_cntx.AlmList[index].Snooze);
	}
		
  	if(AlmIsAnyAlarmActivated())
		AlmActivateIndicator();
	else
		AlmDeactivateIndicator();
}


/*****************************************************************************
* FUNCTION
*	HighlightAlmIndex
* DESCRIPTION
*   Highlight handler of alarm item in alarm screen.
*	Store current highlight index.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightAlmIndex(S32 nIndex) 
{
	g_alm_cntx.CurrHiliteAlarm = (U8)nIndex;
}


/*****************************************************************************
* FUNCTION
*	EntryAlmMenu
* DESCRIPTION
*   Entry function of Alarm application
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryAlmMenu(void) 
{
	U8* guiBuffer;
	S32 i;
	U16 alarm_states_icon[NUM_OF_ALM];
	MYTIME almTime;

	EntryNewScreen(SCR_ID_ALM_LIST, NULL, EntryAlmMenu, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_ALM_LIST);

	if(guiBuffer == NULL)
		g_alm_cntx.CurrHiliteAlarm = 0;

	g_alm_cntx.IsDOWSet = 0;

	/* construct displayed string and icon */
	for(i=0; i<NUM_OF_ALM; i++) 
	{
		almTime.nHour = g_alm_cntx.AlmList[i].Hour;
		almTime.nMin = g_alm_cntx.AlmList[i].Min;
		time_string(&almTime,(UI_string_type)subMenuDataPtrs[i],DT_IDLE_SCREEN);

		if(g_alm_cntx.AlmList[i].State == SET_STATE) 
			alarm_states_icon[i] = IMG_VICON;
		else
			alarm_states_icon[i] = 0;
	}
	
	wgui_override_list_menu_slim_style(WGUI_LIST_MENU_SLIM_STYLE_DRAW_ICON);
#ifdef __MMI_MAINLCD_220X176__
	ShowCategory53Screen(ORGANIZER_MENU_ALARMS_STRINGID,
		0,
		ALARMS_EDIT_SOFTKEY_STRINGID, ALARMS_EDIT_SOFTKEY_IMAGEID,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,NUM_OF_ALM,
		subMenuDataPtrs,alarm_states_icon,0,0,(S32)g_alm_cntx.CurrHiliteAlarm,guiBuffer);
#else
	ShowCategory53Screen(ORGANIZER_MENU_ALARMS_STRINGID, GetRootTitleIcon(ORGANIZER_ALARM_MENU),
		ALARMS_EDIT_SOFTKEY_STRINGID, ALARMS_EDIT_SOFTKEY_IMAGEID,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,NUM_OF_ALM,
		subMenuDataPtrs,alarm_states_icon,0,0,(S32)g_alm_cntx.CurrHiliteAlarm,guiBuffer);
#endif
	wgui_restore_list_menu_slim_style();

	SetLeftSoftkeyFunction(AlmPrepareInlineScreen,KEY_EVENT_UP);	
	SetKeyHandler(AlmPrepareInlineScreen,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);	
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	

	RegisterHighlightHandler(HighlightAlmIndex);
}


/*****************************************************************************
* FUNCTION
*	AlmGoBackAlarmMenu
* DESCRIPTION
*   Go back to alarm menu list.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmGoBackAlarmMenu(void) 
{
	GoBackToHistory(SCR_ID_ALM_LIST);
}


/*****************************************************************************
* FUNCTION
*	EntryAlmSaveConfirmation
* DESCRIPTION
*   Display confirmation screen for setting alarm.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryAlmSaveConfirmation(void) 
{
#ifdef __MMI_MAINLCD_220X176__
    int i = 0;
#endif
#ifdef __MMI_ALM_AUDIO_OPTIONS__
	if( (!((g_alm_cntx.CurrHiliteAudioOption && g_alm_audio_option_previndex) ||(!g_alm_cntx.CurrHiliteAudioOption && !g_alm_audio_option_previndex))) ||
			(g_alm_cntx.CurrHiliteAudioOptionValue == 0))
	{
		DisplayConfirm(STR_GLOBAL_OK,
							IMG_GLOBAL_OK,
							0,
							0,
							get_string(STR_ID_ALM_FMRDO_AUD_OPT_INCOMPLETE_NOTIFY),
							0,
							WARNING_TONE);
		SetLeftSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		return;
	}
	
	/*check if the currently selected option is ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL and some channel is selected for it*/
	if(g_alm_cntx.CurrHiliteAudioOption == ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL)
	{
		if(g_alm_cntx.CurrHiliteAudioOptionValue < MIN_FM_FREQUENCY || g_alm_cntx.CurrHiliteAudioOptionValue > MAX_FM_FREQUENCY ) //if the freq is not correct
		{
			DisplayConfirm(STR_GLOBAL_OK, 
								IMG_GLOBAL_OK,
								0,
								0,
								get_string(STR_ID_ALM_FMRDO_INCOMPLETE_CHANNEL_INFO),
								0,
								WARNING_TONE);
			SetLeftSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
			return;
		}
	}
#endif

#ifdef __MMI_MAINLCD_220X176__
    for (i=0; i<NUM_OF_ALM; i++)
    {
        int index = g_alm_cntx.CurrHiliteAlarm;
        if (i == index) 
	     continue;

        if ( (g_alm_cntx.AlmList[i].Hour == ((g_alm_cntx.HourBuf[0]-'0')* 10 + (g_alm_cntx.HourBuf[ENCODING_LENGTH]-'0'))) &&
		(g_alm_cntx.AlmList[i].Min == ((g_alm_cntx.MinBuf[0]-'0')* 10 + (g_alm_cntx.MinBuf[ENCODING_LENGTH]-'0'))) &&
		(g_alm_cntx.AlmList[i].Freq ==  (U8)g_alm_cntx.CurrHiliteFreq) &&
		(g_alm_cntx.AlmList[i].State == (U8)g_alm_cntx.CurrHiliteState)
	    )
    	    {
    	    
    	    	DisplayPopup((U8*)GetString(STR_GLOBAL_ERROR) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		DeleteUptoScrID(SCR_ID_ALM_LIST);
		return;
	    }
    }
#endif
	DisplayConfirm(STR_GLOBAL_YES,
						IMG_GLOBAL_YES,
						STR_GLOBAL_NO,
						IMG_GLOBAL_NO,
						get_string(STR_GLOBAL_SAVE),
						IMG_GLOBAL_QUESTION,
						WARNING_TONE);

	SetLeftSoftkeyFunction(AlmSaveInlineEdit,KEY_EVENT_UP);
	SetRightSoftkeyFunction(AlmGoBackAlarmMenu,KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*	AlmPrepareInlineScreen
* DESCRIPTION
*   This fuction builds the display information of the selected alarm, and then enter
*	alarm screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmPrepareInlineScreen(void)
{
	S32 i,j;

	i = g_alm_cntx.CurrHiliteAlarm;
	/* fill inline selection strings */
	gAlmOnOffString[0] = GetString(ALARM_OFF_STRING);
	gAlmOnOffString[1] = GetString(ALARM_ON_STRING);
	gAlmFreqList[0] = GetString(ALARM_ONCE_STRING);
	gAlmFreqList[1] = GetString(ALARM_EVREYDAY_STRING);
	gAlmFreqList[2] = GetString(ALARM_DAYS_STRING);

#ifdef __MMI_ALM_AUDIO_OPTIONS__
	g_alm_audio_options_string[0] = GetString(STR_ID_ALM_AUDIO_OPTIONS_TONE);    
	g_alm_audio_options_string[1] = GetString(STR_ID_ALM_AUDIO_OPTIONS_FMRADIO);
#endif

	/* time string */
	AlmGetStringValue(g_alm_cntx.HourBuf,g_alm_cntx.AlmList[i].Hour);
	AlmGetStringValue(g_alm_cntx.MinBuf,g_alm_cntx.AlmList[i].Min);	

	g_alm_cntx.CurrHiliteState = g_alm_cntx.AlmList[i].State;
	g_alm_cntx.CurrHiliteFreq = g_alm_cntx.AlmList[i].Freq;

#ifdef __MMI_ALM_CUST_VOLUME__
	g_alm_cntx.CurrHiliteVol = g_alm_cntx.AlmList[i].Volume;
#endif

	if(g_alm_cntx.CurrHiliteFreq==DAYS)	/* not Days */
	{
		for(j=0; j<NUM_DAYS; j++) 
			gAlmDaysStates[j] = ((((g_alm_cntx.AlmList[i].Days) & (0x1 << j)) == 0) ?0:1);	
	}
	else
	{
		memset(gAlmDaysStates,0,7);
	}

	/*Audio options in alarm*/
#ifdef __MMI_ALM_AUDIO_OPTIONS__
	g_alm_cntx.CurrHiliteAudioOption = g_alm_cntx.AlmList[i].AudioOption;

	if(g_alm_cntx.CurrHiliteAudioOption == ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL)
	{
		S16 error;
		alm_nvram_struct_extended temp_storage_for_nvram_data;
		U8 nvram_buf[NVRAM_ALM_SPOF_DATA_SIZE];

		g_alm_cntx.CurrHiliteAudioOptionValue = g_alm_cntx.AlmList[i].AudioOptionValue;
		ReadRecord(NVRAM_EF_ALM_SPOF_DATA_LID, 1, (void*)nvram_buf, NVRAM_ALM_SPOF_DATA_SIZE, &error);
		memcpy(&temp_storage_for_nvram_data, nvram_buf, sizeof(alm_nvram_struct_extended));
		pfnUnicodeStrcpy((S8*)g_alm_cntx.CurrHiliteAlmFMChannelName,(S8*)temp_storage_for_nvram_data.alm_fm_selected_channel_names[i]);
	}
	else if(g_alm_cntx.CurrHiliteAudioOption == ALM_AUDIO_OPTIONS_FMRADIO_MANUAL)
	{
		
		g_alm_cntx.CurrHiliteAudioOptionValue = g_alm_cntx.AlmList[i].AudioOptionValue;
	}
	else /*ALM_AUDIO_OPTIONS_TONE*/
	{
#ifdef __MMI_ALM_CUSTOMIZE_TONE__
		if(g_alm_cntx.AlmList[i].AudioOptionValue > 0)
		{
			g_alm_cntx.CurrHiliteAudioOptionValue = g_alm_cntx.AlmList[i].AudioOptionValue;		
		}
		else
		{
			g_alm_cntx.CurrHiliteAudioOptionValue = gstartRingId;
		}
#else
		g_alm_cntx.CurrHiliteAudioOptionValue = gcurrentprofile.toneSetup.alarmTone;
#endif
	}

#endif /* __MMI_ALM_AUDIO_OPTIONS__ */

#ifdef __MMI_ALM_SNOOZE_SETTING__ //qiff modify:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__
    g_alm_cntx.CurrHilightSnoozeInterval = g_alm_cntx.AlmList[i].SnoozeInterval;
#endif 

	EntryAlmEditScreen();
}

#ifdef __MMI_ALM_CUST_VOLUME__
void AlmPlayAlarmToneWithVol(U8 volume)
{
	if(g_alm_frm_cntx.IsAlmTonePlaying == FALSE)
	{
#ifdef __MMI_PROFILE_ALERT_TYPE_DOMINANT__
		U8 alert_type = GetMtCallAlertTypeEnum();
#endif
		VibratorOn();

#ifdef __MMI_PROFILE_ALERT_TYPE_DOMINANT__		
		if( alert_type != MMI_VIBRATION_ONLY && alert_type != MMI_SILENT )
#endif
		{
#ifdef __MMI_ALM_AUDIO_OPTIONS__
			if(g_alm_frm_cntx.CurrAlmIndex >= ALM_ALM_START && g_alm_frm_cntx.CurrAlmIndex < ALM_SPOF_START)
			{
				if((g_alm_cntx.AlmList[g_alm_frm_cntx.CurrAlmIndex - ALM_ALM_START].AudioOption == ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL)||(g_alm_cntx.AlmList[g_alm_frm_cntx.CurrAlmIndex - ALM_ALM_START].AudioOption == ALM_AUDIO_OPTIONS_FMRADIO_MANUAL))
				{
					mdi_audio_set_volume( AUD_VOLUME_FMR, volume );
					mdi_fmr_power_on_with_path( MDI_DEVICE_SPEAKER_BOTH, (mdi_callback) mmi_fmrdo_preempted );
					mdi_fmr_set_freq( g_alm_cntx.AlmList[g_alm_frm_cntx.CurrAlmIndex - ALM_ALM_START].AudioOptionValue);
				}
				else //this is for the case when tone is the selected audio option for alarm
				{
	#ifdef __MMI_ALM_CUSTOMIZE_TONE__
					AudioPlayToneWithCallBackVolPath(
						g_alm_cntx.AlmList[g_alm_frm_cntx.CurrAlmIndex - ALM_ALM_START].AudioOptionValue, // current profile alarm tone id
						DEVICE_AUDIO_PLAY_INFINITE,
						ALARM_TONE,
						volume, // your volume level 0~6
						MDI_DEVICE_SPEAKER_BOTH );
	#else
					AudioPlayToneWithCallBackVolPath(
						GetAlarmToneType(), // current profile alarm tone id
						DEVICE_AUDIO_PLAY_INFINITE,
						ALARM_TONE,
						volume, // your volume level 0~6
						MDI_DEVICE_SPEAKER_BOTH );
	#endif
				}
			}
			else
#endif
			{
				AudioPlayToneWithCallBackVolPath(
					GetAlarmToneType(), // current profile alarm tone id
					DEVICE_AUDIO_PLAY_INFINITE,
					ALARM_TONE,
					volume, // your volume level 0~6
					MDI_DEVICE_SPEAKER_BOTH );
			}
		}
		
		g_alm_frm_cntx.IsAlmTonePlaying=TRUE;
	}
}
#endif

/*****************************************************************************
* FUNCTION
*	EntryAlmIndicationScreen
* DESCRIPTION
*   Display indication screen and play alarm tone when alarm expires.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryAlmIndicationScreen(void) 
{
	U8* guiBuffer;
	MYTIME currTime;
	U8 timeString[SIZE_OF_DATE_STRING];
	static U8 dateString[SIZE_OF_ALARM_STRING + SIZE_OF_DATE_STRING]; 	

	StopMyTimer(ALARM_TIMER_WAIT_FOR_CALL);
	/*dongwg Add Start For 6393 Ver: TBM780  on 2007-9-10 11:23 */
	StopTimer(IDLE_APP_AUTO_LOCK_TIMER_ID);
	/*dongwg Add End  For 6393 Ver: TBM780  on 2007-9-10 11:23 */
	TurnOnBacklight(1);//dongwg move 20070803
	
	EntryNewScreen(SCR_ID_ALM_INDICATION, ExitAlmIndicationScreen, NULL, NULL);

//zough add for getting exact time  20061130 begin
//	GetDateTime(&currTime);
	DTGetRTCTime(&currTime);
//zough add for getting exact time  20061130 end

	time_string(&currTime,(UI_string_type)timeString,DT_IDLE_SCREEN);
	/* prepare string to display */
	if(g_alm_frm_cntx.CurrAlmIndex == ALM_FACTORY)
	{
		memset(dateString, 0, ENCODING_LENGTH);
		pfnUnicodeStrcpy((S8*)dateString,(S8*)L"\n");
		pfnUnicodeStrcat((S8*)dateString,(S8*)GetString(ALARMS_POWER_ON));
		pfnUnicodeStrcat((S8*)dateString,(S8*)L"?");
	}
	else
	{
		date_string(&currTime,(UI_string_type)dateString,DT_IDLE_SCREEN);
		pfnUnicodeStrcat((S8*)dateString,(S8*)L"\n");
		pfnUnicodeStrcat((S8*)dateString,(S8*)timeString);
	}

	/* flush key because alarm if some key is not released, the alarm may be stopped */
	ClearKeyEvents();

	g_alm_cntx.IsSaveToHistory = 1;
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_ALM_INDICATION);


#ifdef __MMI_ALM_CUST_VOLUME__
	if(g_alm_frm_cntx.CurrAlmIndex != ALM_FACTORY)
		AlmPlayAlarmToneWithVol(g_alm_cntx.AlmList[g_alm_frm_cntx.CurrAlmIndex - ALM_ALM_START].Volume);
	else
		AlmPlayAlarmTone();	
#else
	AlmPlayAlarmTone();
#endif

	/*qiff modify:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__*/
	ShowCategory121Screen(ALARMS_STOP_STRINGID,ALARMS_STOP_IMAGEID,ALARMS_SNOOZE_STRINGID,
							ALARMS_SNOOZE_IMAGEID,dateString, ALARM_ANIMATION_INDICATION_IMAGEID,
							guiBuffer,1);
	/*dongwg Modify End  For 6159 Ver: TBM780  on 2007-8-9 16:5 */

	if(g_alm_frm_cntx.CurrAlmIndex == ALM_FACTORY)
	{
		ChangeLeftSoftkey(STR_GLOBAL_YES, IMG_GLOBAL_YES);
		ChangeRightSoftkey(STR_GLOBAL_NO, IMG_GLOBAL_NO);

		SetLeftSoftkeyFunction(AlmSendPowerOn, KEY_EVENT_UP);
		SetRightSoftkeyFunction(AlmSendPowerOff, KEY_EVENT_UP);
	}
	else
	{
		SetLeftSoftkeyFunction(AlmStopHandler,KEY_EVENT_UP);	
		/*dongwg Modify Start For 6159 Ver: TBM780  on 2007-8-9 10:31 */
		SetRightSoftkeyFunction(AlmSnoozeHandler,KEY_EVENT_UP);//qiff modify:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__
		/*dongwg Modify End  For 6159 Ver: TBM780  on 2007-8-9 10:31 */
		SetKeyHandler(AlmStopHandler,KEY_END,KEY_EVENT_DOWN);		//	SetKeyHandler(AlarmEndKeyPressed,KEY_END,KEY_EVENT_DOWN);

		#if defined(__MMI_ALARM_SIDE_KEY_CONTROL__)	
			SetKeyHandler(AlmStopHandler,KEY_VOL_UP, KEY_EVENT_DOWN);
			SetKeyHandler(AlmSnoozeHandler,KEY_VOL_DOWN, KEY_EVENT_DOWN);
		#endif
	}

	ForceSubLCDScreen(EntryAlmSubLcdScreen);

	/* register END key for power on alarm */
	if(g_alm_frm_cntx.IsPwronAlarm)
	{
		ClearKeyHandler(KEY_END,KEY_EVENT_UP);
		ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
		SetKeyHandler(AlmHandleEndKey,KEY_END,KEY_LONG_PRESS);		
		if(g_charbat_context.isChargerConnected && (!g_charbat_context.ChargingComplete))
		{
			AnimateStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
			AnimateStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
			UpdateStatusIcons();
		}		
	}
}


/*****************************************************************************
* FUNCTION
*	AlmHandleEndKey
* DESCRIPTION
*   End key handler of alarm indication screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmHandleEndKey(void)
{
	S32 i;
	i = g_alm_frm_cntx.CurrAlmIndex - ALM_ALM_START;

	AlmsStopAlarmTone();

	g_alm_cntx.AlmList[i].Snooze = 0;
		
	if(g_alm_cntx.AlmList[i].Freq == ONCE)	// reset the alarm state
	{
		g_alm_cntx.AlmList[i].State = UNSET_STATE;
		AlmWriteToNvram();
	}

	AlmSendPowerOn();
}


/*****************************************************************************
* FUNCTION
*	ExitAlmIndicationScreen
* DESCRIPTION
*   Exit function of alarm indication screen.
*	If exits screen because of interrupts, add screen to history.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitAlmIndicationScreen(void) 
{
	AlmsStopAlarmTone();
	mdi_audio_resume_background_play();//chenhe add for resume background audio player
	if(g_alm_cntx.IsSaveToHistory) /* add screen to history */
		GenericExitScreen(SCR_ID_ALM_INDICATION,  EntryAlmIndicationScreen);	
}


/*****************************************************************************
* FUNCTION
*	EntryAlmSubLcdScreen
* DESCRIPTION
*   Display sub-lcd screen when alarm expires.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryAlmSubLcdScreen(void)
{
	ExecSubLCDCurrExitHandler();
	ShowCategory304Screen(ALARM_ANIMATION_INDICATION_SUBLCD_IMAGEID,0);
//	SetSubLCDExitHandler(ExitAlmSubLcdScreen);
}


/*****************************************************************************
* FUNCTION
*	ExitAlmSubLcdScreen
* DESCRIPTION
*   Exit function of EntryAlmSubLcdScreen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitAlmSubLcdScreen(void)
{
	SubLCDHistoryNode SubLCDHistory;
	SubLCDHistory.entryFuncPtr=EntryAlmSubLcdScreen;
//	AddSubLCDHistory(&SubLCDHistory);
	AddSubLCDHistoryWithScrID(&SubLCDHistory,SCR_ID_ALM_SUB_INDICATION);

}

void AlmCommonExitProcedure(void)
{
	g_alm_cntx.IsSaveToHistory =0;

	//TurnOffBacklight();	//delete by liuxn 060616

	AlmsStopAlarmTone();
}


/*****************************************************************************
* FUNCTION
*	AlmExitIndScreen
* DESCRIPTION
*   Exit alarm indication screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmExitIndScreen(void)
{
	if(GetExitScrnID() == SCR_ID_ALM_INDICATION)
	{
		//GoBackBothLCDHistory();
		GoBackHistory();
	}
	else
	{
		DeleteScreenIfPresent(SCR_ID_ALM_INDICATION);
	/*	#ifdef __MMI_SUBLCD__
		   if(IsSubLCDScreenPresent(SCR_ID_ALM_SUB_INDICATION))
   			DeleteSubLCDScreen(SCR_ID_ALM_SUB_INDICATION);
		   else
		      GoBackSubLCDHistory();
		#endif
	*/
	}
}

/*****************************************************************************
* FUNCTION
*	AlmStopHandler
* DESCRIPTION
*   Handler for stopping alarm.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
extern void AlmstopVibratorThenRing(void)	;

void AlmStopHandler(void) 
{
	S32 i;

	/*dongwg Modify Start For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	//StopMyTimer(ALARM_ALERT_NOTIFYDURATION_TIMER);
	coolsand_UI_cancel_timer(AlmHandleAlarmTimeout);
	/*dongwg Modify End  For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	AlmCommonExitProcedure();

	coolsand_UI_cancel_timer(AlmstopVibratorThenRing);//chenhe,add 070809,to fix bug #6142
	
	/* factory alarm does not have to re-schedule */
	if(g_alm_frm_cntx.CurrAlmIndex != ALM_FACTORY)
	{
		i = g_alm_frm_cntx.CurrAlmIndex - ALM_ALM_START;

		g_alm_cntx.AlmList[i].Snooze = 0;
		
		if(g_alm_cntx.AlmList[i].Freq == ONCE)	// reset the alarm state
		{
			g_alm_cntx.AlmList[i].State = UNSET_STATE;

			if(!(AlmIsAnyAlarmActivated()))
				AlmDeactivateIndicator();
		}
		AlmWriteToNvram();
	}

	g_alm_frm_cntx.CurrAlmIndex = 0xff;


	if(g_alm_frm_cntx.IsPwronAlarm == FALSE) 	// normal power on
	{
		//GoBackBothLCDHistory();		
		AlmExitIndScreen();
		
		if(g_alm_frm_cntx.IsExitByTimer == FALSE)
			AlmExecPwrOnAlarm();
	}
	else 
	{
		EntryAlmPowerOnConfirmation();
	}
}


/*****************************************************************************
* FUNCTION
*	AlmStopHandler
* DESCRIPTION
*   Handler for snoozing alarm.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmSnoozeHandler(void) 
{
	S32 i;

#ifdef MMI_ON_WIN32
	if(g_alm_frm_cntx.IsPwronAlarm)
		AlmSendPowerOff();
#endif

	/*dongwg Modify Start For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	//StopMyTimer(ALARM_ALERT_NOTIFYDURATION_TIMER);
	coolsand_UI_cancel_timer(AlmHandleAlarmTimeout);
	/*dongwg Modify End  For 6041 Ver: TBM780  on 2007-8-6 14:49 */
	coolsand_UI_cancel_timer(AlmstopVibratorThenRing);
	COS_Sleep(100);
	mmi_trace(g_sw_ALARM, "gdm: come here!! AlmSnoozeHandler()");
	i = g_alm_frm_cntx.CurrAlmIndex - ALM_ALM_START;
	g_alm_cntx.AlmList[i].Snooze++;

	AlmCommonExitProcedure();

	if(g_alm_cntx.AlmList[i].Snooze <= MAX_SNOOZE_CHANCES)
	{	
		AlmWriteToNvram();
		AlmInsertIntoQueue(g_alm_frm_cntx.CurrAlmIndex, TRUE);

		g_alm_frm_cntx.CurrAlmIndex = 0xff;
		
		if(g_alm_frm_cntx.IsPwronAlarm == FALSE)
		{
			//GoBackBothLCDHistory();			
			AlmExitIndScreen();
		}
		else if(g_charbat_context.PowerOnCharger == TRUE)
		{
   			g_alm_frm_cntx.IsPwronAlarm=FALSE;  
   			
   			if(g_charbat_context.isChargerConnected)
				//GoBackBothLCDHistory();
				AlmExitIndScreen();
		}
		else if(g_alm_frm_cntx.IsPwronAlarm && g_charbat_context.isChargerConnected)
		{
   			g_alm_frm_cntx.IsPwronAlarm=FALSE;  
			AlarmDisplayCharging();
		}
		else if(g_alm_frm_cntx.IsPwronAlarm && g_alm_cntx.IsInPwronConfirm)
		{
			//GoBackHistory();
			AlmExitIndScreen();
		}

		if(g_alm_frm_cntx.IsExitByTimer == FALSE)
		{
			if(g_alm_frm_cntx.IsPwronAlarm)
			{
				AlmExecPwrOffAlarm();
			}
			else
			{
				AlmExecPwrOnAlarm();
			}
		}
	}
	else
	{
		AlmStopHandler();
	}
}


/*****************************************************************************
* FUNCTION
*	AlmWriteToNvram
* DESCRIPTION
*	Write alarm data to nvram
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmWriteToNvram(void)
{
	S16 error;
	U8 nvram_buf[NVRAM_ALM_SPOF_DATA_SIZE];
#ifdef __MMI_ALM_AUDIO_OPTIONS__
	alm_nvram_struct_extended temp_alm_nvram_struct;
#endif

	memset(nvram_buf, 0, NVRAM_ALM_SPOF_DATA_SIZE);

#ifdef __MMI_ALM_AUDIO_OPTIONS__
	ReadRecord(NVRAM_EF_ALM_SPOF_DATA_LID, 1, (void*)nvram_buf, NVRAM_ALM_SPOF_DATA_SIZE, &error);
	memcpy(&temp_alm_nvram_struct, nvram_buf, sizeof(alm_nvram_struct_extended));
	memcpy(temp_alm_nvram_struct.AlmList,g_alm_cntx.AlmList,sizeof(g_alm_cntx.AlmList));

	if(g_alm_audio_options_alarm_edited_manually)
	{
		if(g_alm_cntx.CurrHiliteAudioOption == ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL)
			pfnUnicodeStrcpy((S8*)temp_alm_nvram_struct.alm_fm_selected_channel_names[g_alm_cntx.CurrHiliteAlarm],(S8*)g_alm_cntx.CurrHiliteAlmFMChannelName);
		g_alm_audio_options_alarm_edited_manually = FALSE;
	}
    memcpy(nvram_buf,&temp_alm_nvram_struct, sizeof(alm_nvram_struct_extended));
#else
	memcpy(nvram_buf, g_alm_cntx.AlmList, sizeof(g_alm_cntx.AlmList));		
#endif
	
	WriteRecord(NVRAM_EF_ALM_SPOF_DATA_LID, 1, (void*)nvram_buf, NVRAM_ALM_SPOF_DATA_SIZE, &error);
}


/*****************************************************************************
* FUNCTION
*	AlmActivateIndicator
* DESCRIPTION
*   To check if there is any activated alarm.
* PARAMETERS
*	None.
* RETURNS
*	0 if no activated alarm; otherwise 0.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
BOOL AlmIsAnyAlarmActivated(void)
{
	S32 i;
  
	for (i=0; i<NUM_OF_ALM; i++) 
		if(g_alm_cntx.AlmList[i].State == SET_STATE)
			return TRUE;
	return FALSE;
}


/*****************************************************************************
* FUNCTION
*	AlmActivateIndicator
* DESCRIPTION
*   Show alarm status icon in IDLE screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmActivateIndicator(void) 
{
	ShowStatusIcon(STATUS_ICON_ALARM);
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__	
	ShowStatusIcon(STATUS_ICON_SUBLCD_ALARM);
#endif
	UpdateStatusIcons();
}


/*****************************************************************************
* FUNCTION
*	AlmDeactivateIndicator
* DESCRIPTION
*   Hide alarm status icon in IDLE screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmDeactivateIndicator(void) 
{
	HideStatusIcon(STATUS_ICON_ALARM);
#ifndef __MMI_SUBLCD_SHORT_STATUSICON_LIST__	
	HideStatusIcon(STATUS_ICON_SUBLCD_ALARM);
#endif
	UpdateStatusIcons();
}


/*****************************************************************************
* FUNCTION
*	mmi_alm_check_history_and_poweroff
* DESCRIPTION
*	Check history before sending power off request.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_alm_check_history_and_poweroff(void) 
{
	if (GetExitScrnID() == SCR_ID_ALM_PWRON_CONFIRM)
		g_alm_cntx.IsInPwronConfirm = MMI_FALSE;
		
	if (GetScreenCountInHistory() > 0)
	{
		GoBackHistory();
	}
	else
	{
		AlmSendPowerOff();			
	}
}

void ExitAlmPowerOnConfirmation(void) 
{
#if defined(__MMI_TOUCH_SCREEN__)
    mmi_pen_block( );
#endif
}
/*****************************************************************************
* FUNCTION
*	EntryAlmPowerOnConfirmation
* DESCRIPTION
*   Display confirmation screen to decide power on or not. 
*	This screen is used for power off alarm.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
/*dongwg Add Start For 6138 Ver: TBM780  on 2007-8-27 19:51 */
extern void AlmstopVibratorThenRing(void);
/*dongwg Add End  For 6138 Ver: TBM780  on 2007-8-27 19:51 */
void EntryAlmPowerOnConfirmation(void) 
{
	U8* guiBuffer;
	
	g_alm_cntx.IsInPwronConfirm = TRUE;
	/*dongwg Add Start For 6138 Ver: TBM780  on 2007-8-27 19:51 */
	coolsand_UI_cancel_timer(AlmstopVibratorThenRing);
	/*dongwg Add End  For 6138 Ver: TBM780  on 2007-8-27 19:51 */
	EntryNewScreen(SCR_ID_ALM_PWRON_CONFIRM, NULL, EntryAlmPowerOnConfirmation, NULL);

	/* to prevent power on confirmation screen from displaying twice */
	DeleteScreenIfPresent(SCR_ID_ALM_PWRON_CONFIRM);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_ALM_PWRON_CONFIRM);

	ShowCategory123Screen(ORGANIZER_MENU_ALARMS_STRINGID,ALARMS_MAIN_TITLE_IMAGEID,STR_GLOBAL_YES,IMG_GLOBAL_YES,
							STR_GLOBAL_NO,IMG_GLOBAL_NO,
							ALARMS_POWER_ON,IMG_GLOBAL_QUESTION,guiBuffer);

	SetLeftSoftkeyFunction(AlmSendPowerOn,KEY_EVENT_UP);	
	SetRightSoftkeyFunction(AlmSendPowerOff,KEY_EVENT_UP);
	ClearKeyHandler(KEY_END,KEY_EVENT_UP);
	ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
	SetKeyHandler(AlmSendPowerOn,KEY_END,KEY_LONG_PRESS);

	/* to prevent double displayed this screen */
	DeleteScreenIfPresent(SCR_ID_ALM_PWRON_CONFIRM);
}


/*****************************************************************************
* FUNCTION
*	HighlightAlmInlineRepeat
* DESCRIPTION
*   Highlight handler of Repeat inline selection item in editor screen.
*	Enable DOW component when selecting "Days"
* PARAMETERS
*	index	IN	index of inline selection
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void HighlightAlmInlineRepeat(S32 index)
{
	U8 j=0;

	if(index==2) 
	{
		if (g_alm_cntx.IsDOWSet == 0 )
		{
			/* if the user did not select DOW yet */
			if ( g_alm_cntx.AlmList[g_alm_cntx.CurrHiliteAlarm].Days == 0) 
			{
				memset(gAlmDaysStates,1,7);
				gAlmDaysStates[0] = 0;
				gAlmDaysStates[6] = 0;
			}
			else	/* fill user-set value to DOW */
			{
				for(j=0;j<NUM_DAYS;j++)
					gAlmDaysStates[j] = ((((g_alm_cntx.AlmList[g_alm_cntx.CurrHiliteAlarm].Days) & (0x1 << j)) == 0 ) ?0:1);
			}
				
			g_alm_cntx.IsDOWSet = 1;		
			EnableInlineItem(&wgui_inline_items[4],4);/* multi select days*/
		}
	}
	else
	{
		g_alm_cntx.IsDOWSet = 0;		
		memset(gAlmDaysStates,0,7);
		DisableInlineItem(&wgui_inline_items[4],4);/* multi select days*/
	}

	RedrawCategory57Screen();
}	

//added the support for alarm audio options.
//qiff add the function:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__
/*****************************************************************************
 * FUNCTION
 *  AlmGetSnoozeInterval
 * DESCRIPTION
 *  Get Snooze Interval
 * PARAMETERS
 *  void
 * RETURNS
 *  Snooze interval
 *****************************************************************************/
U8 AlmGetSnoozeInterval(U8 almIndex)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 snooze = 0;
#ifdef __MMI_ALM_SNOOZE_SETTING__

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    snooze = g_alm_cntx.AlmList[almIndex].SnoozeInterval;
#endif /* __MMI_ALM_SNOOZE_SETTING__ */ 

    if (snooze == 0)
    {
        snooze = ALM_SNOOZE_INTERVAL;
    }
    /* ReadValue(NVRAM_ALM_SNOOZE_TIME, &snooze, DS_BYTE, &error); */

    return snooze;
}

#ifdef __MMI_ALM_AUDIO_OPTIONS__   

#ifdef __MMI_ALM_CUSTOMIZE_TONE__
/*****************************************************************************
* FUNCTION
*	mmi_alm_entry_aud_opt_tone
* DESCRIPTION
*   The entry function for the screen displaying options for tone audio option.
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_entry_aud_opt_tone(void)
{
	U8* guiBuffer;
	U16 selecteditem =0;
	U8 totalItems=0;

	EntryNewScreen(SCR_ID_ALM_TONE_SETUP, mmi_alm_exit_aud_opt_tone, mmi_alm_entry_aud_opt_tone, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_ALM_TONE_SETUP);	
	SetParentHandler(MENU_ID_ALM_TONE_SETUP);
	RegisterHighlightHandler(mmi_alm_tone_setup_hilite_hdlr);

	/*Get the highlight index from profile*/
	if(g_alm_cntx.CurrHiliteAudioOption != ALM_AUDIO_OPTIONS_TONE ||
	   (g_alm_cntx.CurrHiliteAudioOption == ALM_AUDIO_OPTIONS_TONE && g_alm_cntx.CurrHiliteAudioOptionValue == 0))
	{
		totalItems = mmi_profiles_get_melody_list( gstartRingId, &selecteditem);
	}
	else
	{
		totalItems = mmi_profiles_get_melody_list(g_alm_cntx.CurrHiliteAudioOptionValue, &selecteditem);
	}

	ShowCategory89Screen(STR_ALARM_TONE_CAPTION,IMG_PROFILES_TITLEE,
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 totalItems,subMenuDataPtrs, hintDataPtrs, 0,selecteditem, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(mmi_alm_entry_tone_setup_set_alarm_tone,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	
}

/*****************************************************************************
* FUNCTION
*	mmi_alm_exit_aud_opt_tone
* DESCRIPTION
*   The exit function for the screen displaying options for tone audio option.
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_exit_aud_opt_tone(void)
{
	StopToneAudio();
}


/*****************************************************************************
* FUNCTION
*	mmi_alm_entry_tone_setup_set_alarm_tone
* DESCRIPTION
*   Setting Alarm tone ID.
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_entry_tone_setup_set_alarm_tone(void)
{
	TONE_SETUP tonesetup;
	U32 tempval1= 0,tempval2=gtotalRingId;
	U32 data=0;
	U8 ExtMelodyFlag=MMI_FALSE;

	if(((g_alm_tone_index) >=(U8)tempval1)&&((g_alm_tone_index)  <(U8)(tempval2)) )
	{
		tonesetup.ringTone = g_alm_tone_index + gstartRingId ;
	}


	tempval1= tempval2;
	tempval2 = gtotalMidiId+gtotalRingId;

	if(((g_alm_tone_index) >=(U8)tempval1)&&((g_alm_tone_index)  <(U8)(tempval2)) )
	{
		tonesetup.ringTone = g_alm_tone_index + gstartMidiId - gtotalRingId;
	}
	
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
	tempval1= tempval2;
	tempval2 = gtotalMidiId+gtotalRingId+MAXIMUM_EXT_MELODY_NUM;

	if( ((g_alm_tone_index)>=(U8)tempval1)&&((g_alm_tone_index)<(U8)(tempval2)) )	
	{
	     /*data is zero based*/
		data = g_alm_tone_index - (gtotalMidiId+gtotalRingId);
		g_ext_melody_idx = (U8)data;
		ExtMelodyFlag = MMI_TRUE;
	}

	if( ExtMelodyFlag == MMI_TRUE )
	{
		g_alm_tone_conf_type = PRFSET_MELODY_TYPE_ALARM;
		mmi_alm_entry_tone_setup_ext_melody();
	}
	else
#endif
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		g_alm_cntx.CurrHiliteAudioOptionValue = tonesetup.ringTone;
		g_alm_cntx.CurrHiliteAudioOption = ALM_AUDIO_OPTIONS_TONE;
		g_alm_aud_opt_selected = TRUE;
		StopToneAudio();
		DeleteScreenIfPresent( SCR_ID_ALM_TONE_SETUP );
	}
}



#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__

/*****************************************************************************
* FUNCTION
*	mmi_alm_entry_tone_setup_ext_melody
* DESCRIPTION
*	Entry function for the external melodies screen.
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_entry_tone_setup_ext_melody( void )
{
	U8 *nStrItemList[PRF_MENU_EXTMLD_MAX];
	U8 nNumofItem = 0; 
	U8* guiBuffer;
	U16 StrIDToneSet=0, StrIDToneDel = 0, StrIDToneAdd = 0;

	StrIDToneSet = STR_TONE_SETUP_ALARM_TONE_EXT_SET;
	StrIDToneDel = STR_TONE_SETUP_ALARM_TONE_EXT_DEL;
	StrIDToneAdd = STR_TONE_SETUP_ALARM_TONE_EXT_ADD;

	if(PmgIsEmptySlot(g_ext_melody_idx) == MMI_FALSE)	/* Not Empty Slot */
	{
		nStrItemList[PRF_MENU_EXTMLD_DONE] = (U8 *)GetString( StrIDToneSet );
		nStrItemList[PRF_MENU_EXTMLD_DEL] = (U8 *)GetString( StrIDToneDel );
		nStrItemList[PRF_MENU_EXTMLD_ADD] = (U8 *)GetString( StrIDToneAdd );
		nNumofItem = (U8)PRF_MENU_EXTMLD_MAX;
	}
	else /* Empty slot, show replace only */
	{
		nStrItemList[0] = (U8 *)GetString( StrIDToneAdd );
		nNumofItem = 1;
	}

	EntryNewScreen(SCR_ID_ALM_TONE_SETUP_EXT_MELODY, NULL, mmi_alm_entry_tone_setup_ext_melody, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_ALM_TONE_SETUP_EXT_MELODY);
	RegisterHighlightHandler (mmi_alm_tone_setup_ext_melody_hilight_hdlr);

	ShowCategory56Screen (STR_ALARM_TONE_CAPTION, IMG_PROFILES_TITLEE,
							 STR_GLOBAL_OK,IMG_GLOBAL_OK,
							 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, NULL, 0, guiBuffer);

	SetLeftSoftkeyFunction(mmi_alm_tone_setup_ext_melody_option,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*	mmi_alm_tone_setup_ext_melody_hilight_hdlr
* DESCRIPTION
*	Highlight handler for the external melodies options screen
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_tone_setup_ext_melody_hilight_hdlr(S32 index)
{
	extMldHlitItem = (U8)index;
}

/*****************************************************************************
* FUNCTION
*	mmi_alm_tone_setup_ext_melody_option
* DESCRIPTION
*	LSK fucntion for the external melodies options screen
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_tone_setup_ext_melody_option(void)
{
	U8 idx = mmi_alm_tone_setup_convert_empty_slot_menu_list( extMldHlitItem );
   
	if( idx == PRF_MENU_EXTMLD_DONE )
	{
		if(PmgIsEmptySlot(g_ext_melody_idx) == MMI_FALSE)
		{
			g_alm_cntx.CurrHiliteAudioOptionValue = PMG_EXT_MELODY_BEGIN + g_ext_melody_idx;
			g_alm_cntx.CurrHiliteAudioOption = ALM_AUDIO_OPTIONS_TONE;
			g_alm_aud_opt_selected = TRUE;

			if(g_alm_tone_conf_type != PRFSET_MELODY_TYPE_PHB_RING)
			{
				DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
				return;
			}
		}
	}
	else if( idx == PRF_MENU_EXTMLD_DEL )
	{
		if( PmgDelExtMelodyById( (U16)(PMG_EXT_MELODY_BEGIN + g_ext_melody_idx) ) == MMI_TRUE )
		{
			DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
			return;
		}
	}
	else if( idx == PRF_MENU_EXTMLD_ADD )
	{
		mmi_alm_entry_tone_setup_ext_melody_drv();
		return;
	}

	DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE); 
}

/*****************************************************************************
* FUNCTION
*	mmi_alm_tone_setup_convert_empty_slot_menu_list
* DESCRIPTION
*	
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
U8 mmi_alm_tone_setup_convert_empty_slot_menu_list( U8 idx )
{
	if(PmgIsEmptySlot(g_ext_melody_idx) == MMI_FALSE)  /* Not Empty slot */
	{
		return idx;
	}
	else
	{
		if( idx == 0 )
		{
			return PRF_MENU_EXTMLD_ADD; /* PRF_MENU_EXTMLD_DONE */
		}
#ifdef MMI_ON_HARDWARE_P
		else
		{
			MMI_ASSERT( MMI_FALSE );
		}
#endif
	}

	return PRF_MENU_EXTMLD_MAX;
}


/*****************************************************************************
* FUNCTION
*	mmi_alm_entry_tone_setup_ext_melody_drv
* DESCRIPTION
*   Fucntion for handling the selection of replace option of external melodies.
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_entry_tone_setup_ext_melody_drv(void)
{
	U8 *nStrItemList[PRF_MENU_EXTMLD_DRV_MAX];
	U8 nNumofItem = (U8)PRF_MENU_EXTMLD_DRV_MAX;
	U8* guiBuffer;
	S16 CardDrv = -1;

#if defined(__MMI_FILE_MANAGER__)
	CardDrv = MMI_FS_GetDrive( FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE );
#endif /* (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) ) */ 

	if( CardDrv > 0 ) /* Card Storage is present */
	{
		nStrItemList[PRF_MENU_EXTMLD_DRV_PHONE] = (U8 *)GetString( STR_PROFILES_RING_TONE_SETUP_DRV_PHONE );
		nStrItemList[PRF_MENU_EXTMLD_DRV_CARD] = (U8 *)GetString( STR_PROFILES_RING_TONE_SETUP_DRV_CARD );

		EntryNewScreen(SCR_ID_ALM_TONE_SETUP_EXT_MELODY_DRV, NULL, mmi_alm_entry_tone_setup_ext_melody_drv, NULL);
		guiBuffer = GetCurrGuiBuffer (SCR_ID_ALM_TONE_SETUP_EXT_MELODY_DRV);
		RegisterHighlightHandler (mmi_alm_tone_setup_ext_melody_hilight_hdlr);

		ShowCategory56Screen (STR_ALARM_TONE_CAPTION,IMG_PROFILES_TITLEE,
							 STR_GLOBAL_OK,IMG_GLOBAL_OK,
							 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							 nNumofItem, nStrItemList, NULL, 0, guiBuffer);

		SetLeftSoftkeyFunction(mmi_alm_tone_setup_ext_melody_drv_select,KEY_EVENT_UP);
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    }
    else  /* Card Storage not present */
    {
		extMldHlitItem = PRF_MENU_EXTMLD_DRV_PHONE;
		mmi_alm_tone_setup_ext_melody_drv_select();
    }
}


/*****************************************************************************
* FUNCTION
*	mmi_alm_tone_setup_ext_melody_drv_select
* DESCRIPTION
*   For displaying the the screen showing the drives.
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_tone_setup_ext_melody_drv_select(void)
{
#if defined(__MMI_FILE_MANAGER__)
	S16 phoneDrvLetter, cardDrvLetter;
	S8 temp[3];
	UI_character_type dir_phone_root[4];
	UI_character_type dir_card_root[4];
	FMGR_FILTER filter;

	FMGR_FILTER_INIT(&filter);
	FMGR_FILTER_SET_AUDIO(&filter);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER_DOT);

	phoneDrvLetter = MMI_FS_GetDrive( FS_DRIVE_V_NORMAL, 2, FS_DRIVE_I_SYSTEM | FS_DRIVE_V_NORMAL );
	cardDrvLetter = MMI_FS_GetDrive( FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE );
	sprintf( temp, "%c:\\", (S8) phoneDrvLetter );
	AnsiiToUnicodeString( (S8*)dir_phone_root, temp );
	sprintf( temp, "%c:\\", (S8) cardDrvLetter );
	AnsiiToUnicodeString( (S8*)dir_card_root, temp );

	if( extMldHlitItem == PRF_MENU_EXTMLD_DRV_CARD)
	{
		mmi_fmgr_select_path_and_enter (APP_PROFILES, FMGR_SELECT_FILE,  \
                                      filter,  \
                                      (PS8)dir_card_root, mmi_alm_tone_setup_ext_melody_drv_select_callback);
	}
	else /* if( extMldHlitItem == PRF_MENU_EXTMLD_DRV_PHONE) */
	{
		mmi_fmgr_select_path_and_enter (APP_PROFILES, FMGR_SELECT_FILE, \
                                      filter, \
                                      (PS8)dir_phone_root, mmi_alm_tone_setup_ext_melody_drv_select_callback);
	}
#endif /* (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__)  */      
}


/*****************************************************************************
* FUNCTION
*	mmi_alm_tone_setup_ext_melody_drv_select_callback
* DESCRIPTION
*	The call back function called when the user selects some file for replacing an external melody. 
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_tone_setup_ext_melody_drv_select_callback(void *path, int is_short)
{
   S8 *filepath = (S8*)path;
   PMG_ADDFILE_ERRNO err = PMG_ADDFILE_ERR_UNKNOWN;
   
   if(filepath)
   {

      if( PmgCheckImyMidFileSize(filepath) == PMG_ADDFILE_ERR_FILE_TOO_LARGE )
      {
         err = PMG_ADDFILE_ERR_FILE_TOO_LARGE;
      }
      else
      {
         PmgSetShortFileName( is_short );
         if(PmgIsEmptySlot(g_ext_melody_idx) == MMI_FALSE)
            err = PmgReplaceExtMelodyBySlotPathFileName( filepath , g_ext_melody_idx);
         else
            err = PmgAddExtMelodyBySlotPathFileName( filepath , g_ext_melody_idx);
      }

      switch( err )
      {
      case PMG_ADDFILE_ERR_SUCCESS:
         DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), 
                                                 IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE); 
           break;
           
      case PMG_ADDFILE_ERR_STR_TOO_LONG:
      case PMG_ADDFILE_ERR_STRFILE_TOO_LONG:
      case PMG_ADDFILE_ERR_STRPATH_TOO_LONG:
          DisplayPopup((PU8)GetString(STR_PROFILES_RING_TONE_SETUP_NAMETOOLONG), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE); 
          break;
          
      case PMG_ADDFILE_ERR_SLOT_FULL:
          DisplayPopup((PU8)GetString(STR_PROFILES_RING_TONE_SETUP_LISTFULL), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
          break;
          
      case PMG_ADDFILE_ERR_FILE_TOO_LARGE:
          DisplayPopup((PU8)GetString(STR_PROFILES_RING_TONE_SETUP_FILETOOLARGE), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE); 
          break;
          
      case PMG_ADDFILE_ERR_DUPLICATED:
          DisplayPopup((PU8)GetString(STR_PROFILES_RING_TONE_SETUP_DUPLICATED), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
          break;

      default:
          DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE); 
          break;
       }
      
       DeleteUptoScrID(SCR_ID_ALM_TONE_SETUP_EXT_MELODY);
       DeleteScreenIfPresent( SCR_ID_ALM_TONE_SETUP_EXT_MELODY );
      
   }
   else
   {
       /* DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), 
                                                IMG_GLOBAL_EMPTY, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE); */
       GoBackToHistory(SCR_ID_ALM_TONE_SETUP_EXT_MELODY);
   }

    
}

/*****************************************************************************
* FUNCTION
*	mmi_alm_tone_setup_delete_ext_melody
* DESCRIPTION
*	When the external file is deleted, Profile will call the function to update ring tone id.
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_tone_setup_delete_ext_melody(U16 ToneId)
{
	U16 i;
	BOOL dirty_flag = FALSE;
	S16 error;
	U8 nvram_buf[NVRAM_ALM_SPOF_DATA_SIZE];
	alm_nvram_struct_extended temp_alm_nvram_struct;

	/*Check if external melody ID is deleted*/
	for( i = 0 ; i < NUM_OF_ALM ; i++)
	{
		/*If the external ID saved in the alarm NVRAM*/
		if(	(g_alm_cntx.AlmList[i].AudioOption == ALM_AUDIO_OPTIONS_TONE) &&
			(g_alm_cntx.AlmList[i].AudioOptionValue == ToneId))
		{
			g_alm_cntx.AlmList[i].AudioOptionValue = gstartRingId;
			dirty_flag = TRUE;
		}

		/*If current highlight on the alarm*/
		if((g_alm_cntx.CurrHiliteAlarm == i) && (g_alm_cntx.CurrHiliteAudioOption == ALM_AUDIO_OPTIONS_TONE))
		{
			g_alm_cntx.CurrHiliteAudioOptionValue = gstartRingId;
		}
	}

	/*Write result back to NVRAM*/
	if(dirty_flag)
	{
		memset(nvram_buf, 0, NVRAM_ALM_SPOF_DATA_SIZE);
		ReadRecord(NVRAM_EF_ALM_SPOF_DATA_LID, 1, (void*)nvram_buf, NVRAM_ALM_SPOF_DATA_SIZE, &error);

		memcpy(&temp_alm_nvram_struct, nvram_buf, sizeof(alm_nvram_struct_extended));
		memcpy(temp_alm_nvram_struct.AlmList, g_alm_cntx.AlmList, sizeof(g_alm_cntx.AlmList));
		memcpy(nvram_buf, &temp_alm_nvram_struct, sizeof(alm_nvram_struct_extended));
	
		WriteRecord(NVRAM_EF_ALM_SPOF_DATA_LID, 1, (void*)nvram_buf, NVRAM_ALM_SPOF_DATA_SIZE, &error);
	}

}


#endif   //__MMI_PROFILE_EXTMELODY_SUPPORT__



/*****************************************************************************
* FUNCTION
*	mmi_alm_tone_setup_hilite_hdlr
* DESCRIPTION
*	Highlight handler for the tone list screen
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_tone_setup_hilite_hdlr(S32 index)
{
	U16 tempval1 = 0;
	U16 tempval2 = gtotalRingId;
	S32 data;

	g_alm_tone_index = (U16)index;

	StopToneAudio();
	
	if((g_alm_tone_index >= tempval1) && (g_alm_tone_index < tempval2))
	{
		gcurrentaudioId = g_alm_tone_index + gstartRingId ;
	}


	tempval1= tempval2;
	tempval2 = gtotalMidiId+gtotalRingId;

	if((g_alm_tone_index >= tempval1) && (g_alm_tone_index < tempval2))
	{
		gcurrentaudioId = g_alm_tone_index + gstartMidiId - gtotalRingId;
	}
	
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
	tempval1= tempval2;
	tempval2 = gtotalMidiId + gtotalRingId + MAXIMUM_EXT_MELODY_NUM;

	if( (g_alm_tone_index >= tempval1) && (g_alm_tone_index < tempval2) )	
	{
		data = g_alm_tone_index - (gtotalMidiId + gtotalRingId);
		gcurrentaudioId = PMG_EXT_MELODY_BEGIN + (U16)data;
	}
#endif

	StartTimer(TIMER_PROFILES_TONE_PLAY, 1000, PlayToneAudio);
}
#endif
/*****************************************************************************
* FUNCTION
*	mmi_alm_entry_aud_opt_fmrdo
* DESCRIPTION
*   The entry function for the screen displaying options for fmradio audio option.
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_entry_aud_opt_fmrdo(void)
{
	U8 *gui_buffer;
	U16 item_count;
	U16 item_texts[10];
	U8 *item_popups[10];
	S32 i;
	U16 item_icons[] = { IMG_GLOBAL_L1, IMG_GLOBAL_L2};

	EntryNewScreen(SCR_ID_ALM_AUDIO_OPT_FMRADIO, NULL, mmi_alm_entry_aud_opt_fmrdo,NULL);
	gui_buffer = GetCurrGuiBuffer(SCR_ID_ALM_AUDIO_OPT_FMRADIO);
	item_count = GetNumOfChild(MENU_ID_ALM_AUDIO_OPT_FMRADIO);
	GetSequenceStringIds(MENU_ID_ALM_AUDIO_OPT_FMRADIO,item_texts);

	SetParentHandler( MENU_ID_ALM_AUDIO_OPT_FMRADIO );
	RegisterHighlightHandler(mmi_alm_aud_opt_fmrdo_highlight_hdlr);

	for( i = 0; i < item_count; i++ )
		item_popups[i] = NULL;
	

	ShowCategory52Screen(STR_GLOBAL_OPTIONS,0/*MAIN_MENU_TITLE_MULTIMEDIA_ICON*/,STR_GLOBAL_OK,0,
		STR_GLOBAL_BACK,0, item_count,item_texts,item_icons,(U8**) item_popups, 0, 0, gui_buffer );

	//Set the right & left arrow key handlers.
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	//Set the LSK and RSK handlers.
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*	mmi_alm_aud_opt_fmrdo_highlight_hdlr
* DESCRIPTION
*   Highlight handler of Alarm Audio Options FM Radio options screen.
*
* PARAMETERS
*	index	IN	index of inline selection
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_aud_opt_fmrdo_highlight_hdlr(S32 index)
{
	if(index == 0)
	{
		SetLeftSoftkeyFunction(mmi_alm_entry_aud_opt_fmrdo_channel_list,KEY_EVENT_UP);
		SetKeyHandler(mmi_alm_entry_aud_opt_fmrdo_channel_list,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}
	else if(index == 1)
	{
		SetLeftSoftkeyFunction(mmi_alm_entry_fmrdo_manual_input,KEY_EVENT_UP);
		SetKeyHandler(mmi_alm_entry_fmrdo_manual_input,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_alm_entry_aud_opt_fmrdo_channel_list
* DESCRIPTION
*   The entry function for the screen displaying the FM channels.
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_entry_aud_opt_fmrdo_channel_list(void)
{
	U8* gui_buffer;
	S32 highlighted_item;
	S16 error;

	/*Use FM radio list before FM radio initialize*/
	if( !g_fmrdo.is_ready && !g_alm_fmrdo_read_channel_name)
	{
		ReadRecord( NVRAM_EF_FM_RADIO_LID, 1, (void*)&g_fmrdo.channel_list, NVRAM_EF_FM_RADIO_SIZE, &error );
		g_alm_fmrdo_read_channel_name = TRUE;
	}
	
	EntryNewScreen( SCR_ID_ALM_AUDIO_OPT_FMRADIO_CHANNEL_LIST, NULL, mmi_alm_entry_aud_opt_fmrdo_channel_list, NULL );
	
	// register highlight handler
	RegisterHighlightHandler( mmi_alm_aud_opt_fmrdo_channel_list_highlight_hdlr );
	
	gui_buffer = GetCurrGuiBuffer( SCR_ID_ALM_AUDIO_OPT_FMRADIO_CHANNEL_LIST );
	if( g_fmrdo.channel_index >= 0 )
		highlighted_item = g_fmrdo.channel_index;
	else
		highlighted_item = 0;

	ShowCategory184Screen( STR_ID_ALM_AUDIO_OPTIONS_FMRADIO_CHANNELLIST, 0,	// title text and icon
		0, 0,							// LSK text and icon
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 								// RSK text and icon
		9, mmi_fmrdo_channel_list_item, mmi_fmrdo_channel_list_item_hint,
		highlighted_item, gui_buffer );
	
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );

}

/*****************************************************************************
* FUNCTION
*	mmi_alm_aud_opt_fmrdo_channel_list_highlight_hdlr
* DESCRIPTION
*   Highlight handler for the FM Channels Screen.
*
* PARAMETERS
*	index	IN	index of inline selection
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_aud_opt_fmrdo_channel_list_highlight_hdlr(S32 index)
{
	g_alm_aud_opt_fmrdo_channel_index = index;
	
	if(g_fmrdo.channel_list.freq[g_alm_aud_opt_fmrdo_channel_index] == 0)
	{

		ClearKeyHandler(KEY_LSK,KEY_EVENT_UP);
		ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
		ChangeLeftSoftkey(0,0);
	}
	else
	{
		ChangeLeftSoftkey(STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS);
		SetLeftSoftkeyFunction( mmi_alm_entry_aud_opt_fmrdo_channel_options, KEY_EVENT_UP );
		SetKeyHandler( mmi_alm_entry_aud_opt_fmrdo_channel_options, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_alm_entry_aud_opt_fmrdo_channel_options
* DESCRIPTION
*   The entry function for the channel options screen
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_entry_aud_opt_fmrdo_channel_options(void)
{
	U8* gui_buffer_p;
	U16 item_texts[] = {STR_GLOBAL_SELECT,STR_ID_ALM_AUDIO_OPT_FMRDO_OPT_PREV};
	U16 item_icons[] = { IMG_GLOBAL_L1, IMG_GLOBAL_L2};
	U8* item_popups[] = { NULL,NULL};
	
	EntryNewScreen(SCR_ID_ALM_AUDIO_OPT_FMRDO_CHANNEL_OPT,NULL,mmi_alm_entry_aud_opt_fmrdo_channel_options,NULL);
	
	gui_buffer_p = GetCurrGuiBuffer( SCR_ID_ALM_AUDIO_OPT_FMRDO_CHANNEL_OPT );
	RegisterHighlightHandler(mmi_alm_fmrdo_channel_opt_highlight_hdlr);

	ShowCategory52Screen(STR_GLOBAL_OPTIONS,0,STR_GLOBAL_OK,0,STR_GLOBAL_BACK,0,
			2,item_texts,item_icons,(U8**) item_popups,0,0,gui_buffer_p);

	SetLeftSoftkeyFunction(mmi_alm_aud_opt_fmrdo_channel_opt_lsk_hdlr,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	//Set Key handlers
	SetKeyHandler( mmi_alm_aud_opt_fmrdo_channel_opt_lsk_hdlr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN );
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );


}

/*****************************************************************************
* FUNCTION
*	mmi_alm_fmrdo_channel_opt_highlight_hdlr
* DESCRIPTION
*   Highlight handler for the FM Radio channel options screen
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_fmrdo_channel_opt_highlight_hdlr(S32 index)
{
	U16 item_texts[] = {STR_GLOBAL_SELECT,STR_ID_ALM_AUDIO_OPT_FMRDO_OPT_PREV};

	g_alm_aud_opt_fmrdo_channel_opt_index = item_texts[index];
}

/*****************************************************************************
* FUNCTION
*	mmi_alm_aud_opt_fmrdo_channel_opt_lsk_hdlr
* DESCRIPTION
*   LSK function for the FM Radio channel options screen
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_aud_opt_fmrdo_channel_opt_lsk_hdlr(void)
{
	switch(g_alm_aud_opt_fmrdo_channel_opt_index)
	{
		case STR_GLOBAL_SELECT:			//for selecting a particular FM channel for alarm.
			g_alm_cntx.CurrHiliteAudioOption = ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL;
			g_alm_cntx.CurrHiliteAudioOptionValue = g_fmrdo.channel_list.freq[g_alm_aud_opt_fmrdo_channel_index]; 
			pfnUnicodeStrcpy((S8*)g_alm_cntx.CurrHiliteAlmFMChannelName,(S8*)g_fmrdo.channel_list.name[g_alm_aud_opt_fmrdo_channel_index]);
			g_alm_aud_opt_selected = TRUE; 
			DisplayPopup((U8*)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
			DeleteUptoScrID(SCR_ID_ALM_EDITOR);
			break;

		case STR_ID_ALM_AUDIO_OPT_FMRDO_OPT_PREV:
			g_alm_fmrdo_prev_freq = g_fmrdo.channel_list.freq[g_alm_aud_opt_fmrdo_channel_index];
#ifdef __MMI_FM_RADIO_BIND_EARPHONE__
			if(0 == GetHandsetInPhone())
			{
				//DisplayPopupCallBack( (PU8)GetString( STR_ID_ALM_FMRDO_PLEASE_PLUG_IN_EARPHONE ), IMG_GLOBAL_WARNING, 1, 2000, (U8)WARNING_TONE,mmi_alm_entry_fmrdo_prev_channel );
				DisplayPopup((U8*)GetString(STR_ID_ALM_FMRDO_PLEASE_PLUG_IN_EARPHONE) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME_2000,WARNING_TONE);
			}
			else
#endif
			mmi_alm_entry_fmrdo_prev_channel();
			break;
	}

}

/*****************************************************************************
* FUNCTION
*	mmi_alm_entry_aud_opt_fmrdo_manual_input
* DESCRIPTION
*   The entry function for the manual input screen
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_entry_fmrdo_manual_input(void)
{
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;

	EntryNewScreen( SCR_ID_ALM_FMRDO_MANUAL_INPUT, mmi_alm_exit_fmrdo_manual_input, NULL, NULL );
	
	InitializeCategory57Screen();
   
	guiBuffer = GetCurrGuiBuffer( SCR_ID_ALM_FMRDO_MANUAL_INPUT );
	inputBuffer = GetCurrNInputBuffer( SCR_ID_ALM_FMRDO_MANUAL_INPUT, &inputBufferSize );

	RegisterHighlightHandler( mmi_alm_fmrdo_manual_input_highlight_hdlr );

	if( guiBuffer == NULL )
	{
		//kal_wsprintf( channel_manual_input_buffer, "%d.%d", g_fmrdo.frequency / 10, g_fmrdo.frequency % 10 );
		if(g_alm_cntx.CurrHiliteAudioOption == ALM_AUDIO_OPTIONS_TONE)
			kal_wsprintf( channel_manual_input_buffer, "0.0");
		else
			kal_wsprintf( channel_manual_input_buffer, "%d.%d", g_alm_cntx.CurrHiliteAudioOptionValue / 10, g_alm_cntx.CurrHiliteAudioOptionValue % 10 );
	}

	SetInlineItemActivation( &wgui_inline_items[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0 );
	SetInlineItemTextEdit( &wgui_inline_items[0], (U8*)channel_manual_input_buffer, MAX_MANUAL_INPUT_LEN, INPUT_TYPE_DECIMAL_NUMERIC );
	LeftJustifyInlineItem( &wgui_inline_items[0] );
	SetInlineItemFullWidth( &wgui_inline_items[0] );
	EnableInlineItemBoundary( &wgui_inline_items[0] );

	DisableCategory57ScreenDone();
	
	// show category screen
	ShowCategory57Screen( STR_ID_ALM_AUDIO_OPTIONS_FMRADIO_MAN_INPUT, MAIN_MENU_TITLE_MULTIMEDIA_ICON,	// title text and icon
		STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,									// LSK text and icon
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,								// RSK text and icon
		1,
		0, wgui_inline_items,
		0, guiBuffer );

	SetCategory57LeftSoftkeyFunction( mmi_alm_fmrdo_manual_input_options );
	SetCategory57RightSoftkeyFunctions( GoBackHistory, GoBackHistory );
	
}


/*****************************************************************************
* FUNCTION
*	mmi_alm_exit_fmrdo_manual_input
* DESCRIPTION
*   The exit function for the manual input screen
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_exit_fmrdo_manual_input(void)
{
	history_t	h;
	U16 inputBufferSize;

	CloseCategory57Screen();
	h.scrnID = SCR_ID_ALM_FMRDO_MANUAL_INPUT;
	h.entryFuncPtr = mmi_alm_entry_fmrdo_manual_input;
	GetCategoryHistory( h.guiBuffer );
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data( (U8*) h.inputBuffer );
	AddNHistory( h, inputBufferSize );
}


/*****************************************************************************
* FUNCTION
*	mmi_alm_fmrdo_manual_input_highlight_hdlr
* DESCRIPTION
*   The highlight handler function for the manual input screen
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_fmrdo_manual_input_highlight_hdlr(S32 index)
{
}


/*****************************************************************************
* FUNCTION
*	mmi_alm_fmrdo_manual_input_LSK
* DESCRIPTION
*   The LSK function for manual input screen
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_fmrdo_manual_input_options()
{// calculate frequency number
	U16 freq = mmi_fmrdo_get_freq_from_str( channel_manual_input_buffer );
	
	// check frequency input
	if( freq < MIN_FM_FREQUENCY || freq > MAX_FM_FREQUENCY ) // warning invalid frequency
		DisplayPopup( (PU8)GetString( STR_ID_ALM_FMRDO_VALID_FM_BANDWIDTH ), IMG_GLOBAL_WARNING, 1, 2000, (U8)WARNING_TONE );
	else
	{
		mmi_alm_entry_fmrdo_manual_input_options();
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_alm_entry_fmrdo_manual_input_options
* DESCRIPTION
*   Entry function for the manual input options screen 
*
* PARAMETERS
*	void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_entry_fmrdo_manual_input_options(void)
{
	U8* gui_buffer_p;
	U16 item_texts[] = {STR_GLOBAL_DONE,STR_ID_ALM_AUDIO_OPT_FMRDO_OPT_PREV};
	U16 item_icons[] = { IMG_GLOBAL_L1, IMG_GLOBAL_L2};
	U8* item_popups[] = { NULL,NULL};
	EntryNewScreen(SCR_ID_ALM_FMRDO_MANUAL_INPUT_OPT,NULL,mmi_alm_entry_fmrdo_manual_input_options,NULL);
	
	gui_buffer_p = GetCurrGuiBuffer( SCR_ID_ALM_FMRDO_MANUAL_INPUT_OPT );
	RegisterHighlightHandler(mmi_alm_fmrdo_manual_input_opt_highlight_hdlr);

	ShowCategory52Screen(STR_GLOBAL_OPTIONS,0,STR_GLOBAL_OK,0,STR_GLOBAL_BACK,0,
			2,item_texts,item_icons,(U8**) item_popups,0,0,gui_buffer_p);
	
	SetLeftSoftkeyFunction(mmi_alm_fmrdo_manual_input_opt_lsk_hdlr,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
 
}

/*****************************************************************************
* FUNCTION
*	mmi_alm_fmrdo_manual_input_opt_lsk_hdlr
* DESCRIPTION
*   LSK handler for the manual input options screen
*
* PARAMETERS
*	index	IN	index of inline selection
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_fmrdo_manual_input_opt_lsk_hdlr(void)
{
	U16 freq = mmi_fmrdo_get_freq_from_str( channel_manual_input_buffer );
	
	switch (g_alm_fmrdo_man_inp_opt_index)
	{
		case STR_GLOBAL_DONE:
			g_alm_cntx.CurrHiliteAudioOptionValue = freq;
			g_alm_cntx.CurrHiliteAudioOption = ALM_AUDIO_OPTIONS_FMRADIO_MANUAL;
			g_alm_aud_opt_selected = TRUE;
			GoBackToHistory( SCR_ID_ALM_EDITOR );
			break;
			
		case STR_ID_ALM_AUDIO_OPT_FMRDO_OPT_PREV:
			g_alm_fmrdo_prev_freq = freq;
#ifdef __MMI_FM_RADIO_BIND_EARPHONE__
		if(0 == GetHandsetInPhone())
		{
			//DisplayPopupCallBack( (PU8)GetString( STR_ID_ALM_FMRDO_PLEASE_PLUG_IN_EARPHONE ), IMG_GLOBAL_WARNING, 1, 2000, (U8)WARNING_TONE,mmi_alm_entry_fmrdo_prev_channel );
			DisplayPopup((U8*)GetString(STR_ID_ALM_FMRDO_PLEASE_PLUG_IN_EARPHONE) ,IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME_2000,WARNING_TONE);
		}
		else
#endif
			mmi_alm_entry_fmrdo_prev_channel();
			
		break;
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_alm_entry_fmrdo_prev_channel
* DESCRIPTION
*   Entry function for the preview screen
*
* PARAMETERS
*	index	IN	index of inline selection
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_entry_fmrdo_prev_channel(void)
{
	U16 str[32];
	U8 vol;
		
	kal_wsprintf( str, "FM %d.%d", g_alm_fmrdo_prev_freq / 10, g_alm_fmrdo_prev_freq % 10 );
		
	//mdi function calls
	vol = mmi_fmrdo_get_volume();
	mdi_audio_set_volume( AUD_VOLUME_FMR, vol );
	mdi_fmr_power_on_with_path( MDI_DEVICE_SPEAKER2, (mdi_callback) mmi_fmrdo_preempted );
	mdi_fmr_set_freq( g_alm_fmrdo_prev_freq);
		
	EntryNewScreen( SCR_ID_ALM_FMRDO_PREV, mmi_alm_exit_fmrdo_prev_channel, NULL, NULL );
	// clear End key
	ClearKeyHandler( KEY_END, KEY_EVENT_DOWN );
	ClearKeyHandler( KEY_END, KEY_LONG_PRESS );
	// Show searching screen
	ShowCategory66Screen( STR_ID_ALM_AUDIO_OPT_FMRDO_OPT_PREV, 0,
		0, 0, STR_GLOBAL_BACK, 0, 
		(U8*)str, IMG_GLOBAL_PROGRESS, NULL );

	// Set RSK to abort searching
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

/*****************************************************************************
* FUNCTION
*	mmi_alm_exit_fmrdo_prev_channel
* DESCRIPTION
*   The interface for calling the mdi function for stopping the FM Radio.
*     
* PARAMETERS
*     void
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_exit_fmrdo_prev_channel(void)
{
	mdi_audio_stop_fmr();
}

/*****************************************************************************
* FUNCTION
*	mmi_alm_fmrdo_manual_input_opt_highlight_hdlr
* DESCRIPTION
*   Highlight handler for the manual input options screen
*
* PARAMETERS
*	index	IN	index of inline selection
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void mmi_alm_fmrdo_manual_input_opt_highlight_hdlr(S32 index)
{
	U16 item_texts[] = {STR_GLOBAL_DONE,STR_ID_ALM_AUDIO_OPT_FMRDO_OPT_PREV};
	g_alm_fmrdo_man_inp_opt_index = item_texts[index];
}
/*****************************************************************************
* FUNCTION
*	HighlightAlmInlineAudioOptions
* DESCRIPTION
*   Highlight handler of Alarm Audio Options inline selection item in editor screen.
*
* PARAMETERS
*	index	IN	index of inline selection
* RETURNS
*	None
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
void HighlightAlmInlineAudioOptions(S32 index)
{
	/*If selection item changed*/
	if(!((g_alm_cntx.CurrHiliteAudioOption && index) || (!g_alm_cntx.CurrHiliteAudioOption && !index)))
	{
		g_alm_cntx.CurrHiliteAudioOptionValue = 0;
		Category57ChangeItemText(g_alm_aud_opt_curselitem_inline_index,(U8*)GetString(STR_GLOBAL_EMPTY_LIST));

#ifndef __MMI_ALM_CUSTOMIZE_TONE__
		if(!index)
		{
			U16 sel_tone = gcurrentprofile.toneSetup.alarmTone;
			U8 tone_index;

			g_alm_cntx.CurrHiliteAudioOptionValue = gcurrentprofile.toneSetup.alarmTone;
			if( (sel_tone>= gstartRingId) && (sel_tone < (gstartRingId + gtotalRingId - 1)))
			{
				tone_index = sel_tone - gstartRingId;
				Category57ChangeItemText(g_alm_aud_opt_curselitem_inline_index,(U8*)GetString((U16)(STR_PROFILES_RING_1 + tone_index)));
			}
			else if((sel_tone)>=gstartMidiId && (sel_tone)<=(gstartMidiId + gtotalMidiId - 1))
			{
				tone_index = (sel_tone - (gstartMidiId) + gtotalRingId);
				Category57ChangeItemText(g_alm_aud_opt_curselitem_inline_index,(U8*)GetString((U16)(STR_PROFILES_RING_1 + tone_index)));
			}
#if defined( __MMI_PROFILE_EXTMELODY_SUPPORT__ )
			else if(sel_tone >= PMG_EXT_MELODY_BEGIN && sel_tone <= PMG_EXT_MELODY_END )
			{
				DYNEXTMELODYINFO *ExtMelodyInfo;
				tone_index = sel_tone - PMG_EXT_MELODY_BEGIN;
				PmgGetExtMelodyInfo(&ExtMelodyInfo);
				Category57ChangeItemText(g_alm_aud_opt_curselitem_inline_index,(ExtMelodyInfo->info[tone_index]).filename);
			}
#endif   //__MMI_PROFILE_EXTMELODY_SUPPORT__
		}
#endif	//	__MMI_ALM_CUSTOMIZE_TONE__
	}
	else /*if the user has selected the same audio option which is the CurrHiliteAudioOption*/
	{
		if(g_alm_cntx.CurrHiliteAudioOption != ALM_AUDIO_OPTIONS_TONE)
		{
			if(g_alm_cntx.CurrHiliteAudioOptionValue < MIN_FM_FREQUENCY || g_alm_cntx.CurrHiliteAudioOptionValue > MAX_FM_FREQUENCY ) 
			{
				Category57ChangeItemText(g_alm_aud_opt_curselitem_inline_index,(U8*)GetString((U16)(STR_GLOBAL_EMPTY_LIST)));
			}
			else
			{
				Category57ChangeItemText(g_alm_aud_opt_curselitem_inline_index,(U8*)g_alm_audio_options_curselitem);
			}
		}
		else
		{
	#ifdef __MMI_ALM_CUSTOMIZE_TONE__
			U16 sel_tone = g_alm_cntx.CurrHiliteAudioOptionValue;
	#else
			U16 sel_tone = gcurrentprofile.toneSetup.alarmTone;
	#endif
			U8 tone_index;
			
			if( (sel_tone>= gstartRingId) && (sel_tone < (gstartRingId + gtotalRingId - 1)))
			{
				tone_index = sel_tone - gstartRingId;
				Category57ChangeItemText(g_alm_aud_opt_curselitem_inline_index,(U8*)GetString((U16)(STR_PROFILES_RING_1 + tone_index)));
			}
			else if((sel_tone >= gstartMidiId) && (sel_tone <= (gstartMidiId + gtotalMidiId - 1)))
			{
				tone_index = (sel_tone - gstartMidiId + gtotalRingId);
				Category57ChangeItemText(g_alm_aud_opt_curselitem_inline_index,(U8*)GetString((U16)(STR_PROFILES_RING_1 + tone_index)));
			}
#if defined( __MMI_PROFILE_EXTMELODY_SUPPORT__ )
			else if(sel_tone >= PMG_EXT_MELODY_BEGIN && sel_tone <= PMG_EXT_MELODY_END )
			{
				DYNEXTMELODYINFO *ExtMelodyInfo;
				tone_index = sel_tone - PMG_EXT_MELODY_BEGIN;
				PmgGetExtMelodyInfo(&ExtMelodyInfo);
				Category57ChangeItemText(g_alm_aud_opt_curselitem_inline_index,(ExtMelodyInfo->info[tone_index]).filename);
			}
#endif   //__MMI_PROFILE_EXTMELODY_SUPPORT__
			else
			{
				g_alm_cntx.CurrHiliteAudioOptionValue = 0;
				Category57ChangeItemText(g_alm_aud_opt_curselitem_inline_index,(U8*)GetString((U16)(STR_GLOBAL_EMPTY_LIST)));
			}
			
		}
	}
	
	RedrawCategory57Screen();	
}

#endif   /* __MMI_ALM_AUDIO_OPTIONS__ */


/*****************************************************************************
* FUNCTION
*	AlmIsInlineEditorChanged
* DESCRIPTION
*   To check if alarm is changed in inline editor screen.
* PARAMETERS
*	index	IN	index of inline selection
* RETURNS
*	TURE if alarm is changed; otherwise, FLASE.
* GLOBALS AFFECTED
*	External global
*****************************************************************************/
BOOL AlmIsInlineEditorChanged(S32 index) 
{
	U8 tmpHour = (g_alm_cntx.HourBuf[0] -'0')*10 + (g_alm_cntx.HourBuf[ENCODING_LENGTH] -'0');
	U8 tmpMin =	(g_alm_cntx.MinBuf[0] -'0')*10 + (g_alm_cntx.MinBuf[ENCODING_LENGTH] -'0');
#ifdef __MMI_ALM_AUDIO_OPTIONS__
	S16 error;
	U8 nvram_buf[NVRAM_ALM_SPOF_DATA_SIZE];
	alm_nvram_struct_extended temp_storage_for_nvram_data;
	ReadRecord(NVRAM_EF_ALM_SPOF_DATA_LID, 1, (void*)nvram_buf, NVRAM_ALM_SPOF_DATA_SIZE, &error);
	memcpy(&temp_storage_for_nvram_data, nvram_buf, sizeof(alm_nvram_struct_extended));
#endif

	if(
#ifdef __MMI_ALM_CUST_VOLUME__
		(g_alm_cntx.AlmList[index].Volume!= g_alm_cntx.CurrHiliteVol) ||
#endif
		(g_alm_cntx.AlmList[index].State != g_alm_cntx.CurrHiliteState) ||
		(g_alm_cntx.AlmList[index].Freq !=  (U8)g_alm_cntx.CurrHiliteFreq) ||
		(g_alm_cntx.AlmList[index].Hour != tmpHour) ||
		(g_alm_cntx.AlmList[index].Min != tmpMin)) 	
	{
		return TRUE;
	}
#ifdef __MMI_ALM_AUDIO_OPTIONS__
	else if((g_alm_cntx.AlmList[index].AudioOption != g_alm_cntx.CurrHiliteAudioOption))
	{
		return TRUE;
	}
	else if((g_alm_cntx.AlmList[index].AudioOption == ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL) && (g_alm_cntx.AlmList[index].AudioOptionValue != g_alm_cntx.CurrHiliteAudioOptionValue))
	{
		return TRUE;
	}
	else if((g_alm_cntx.AlmList[index].AudioOption == ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL) &&(pfnUnicodeStrcmp((S8*)temp_storage_for_nvram_data.alm_fm_selected_channel_names[index],(S8*)g_alm_cntx.CurrHiliteAlmFMChannelName)))
	{
		return TRUE;
	}
	else if((g_alm_cntx.AlmList[index].AudioOption == ALM_AUDIO_OPTIONS_FMRADIO_MANUAL) && (g_alm_cntx.AlmList[index].AudioOptionValue != g_alm_cntx.CurrHiliteAudioOptionValue))
	{
		return TRUE;
	}
#ifdef __MMI_ALM_CUSTOMIZE_TONE__
	else if((g_alm_cntx.AlmList[index].AudioOption == ALM_AUDIO_OPTIONS_TONE) && g_alm_cntx.AlmList[index].AudioOptionValue != g_alm_cntx.CurrHiliteAudioOptionValue)
	{
		return TRUE;
	}
#endif

#endif
#ifdef __MMI_ALM_SNOOZE_SETTING__ //qiff modify:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__
    else if (g_alm_cntx.AlmList[index].SnoozeInterval != (U8) g_alm_cntx.CurrHilightSnoozeInterval)
    {
        return TRUE;
    }
#endif /* __MMI_ALM_SNOOZE_SETTING__ */ 

	else if(g_alm_cntx.AlmList[index].Freq == DAYS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************
* FUNCTION
*	AlmInlineEditHighlightHdlr
* DESCRIPTION
*   Alarm editor highlight handler
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmInlineEditHighlightHdlr(S32 index)
{
#ifdef __MMI_ALM_AUDIO_OPTIONS__   
	if(index == g_alm_aud_opt_curselitem_inline_index)
	{
		ChangeLeftSoftkey(STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS);
	
		if(g_alm_audio_option_previndex == ALM_AUDIO_OPTIONS_TONE)
		{
#ifdef __MMI_ALM_CUSTOMIZE_TONE__
			SetLeftSoftkeyFunction(mmi_alm_entry_aud_opt_tone,KEY_EVENT_UP);
#else
			ChangeLeftSoftkey(0,0);
			ClearKeyHandler(KEY_LSK,KEY_EVENT_UP);
			//g_alm_cntx.CurrHiliteAudioOption = ALM_AUDIO_OPTIONS_TONE;
#endif

		}
		else
		{
			SetLeftSoftkeyFunction(mmi_alm_entry_aud_opt_fmrdo,KEY_EVENT_UP);
		}
	}

#endif
}


/*****************************************************************************
* FUNCTION
*	EntryAlmEditScreen
* DESCRIPTION
*   Display inline editor screen of alarm.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryAlmEditScreen(void) 
{
	U8* guiBuffer;
	U8* inputBuffer;					//added for inline edit history
	U16 inputBufferSize;				//added for inline edit history
	U8 nMenuItems;

	U16 alarm_icon_list[] = { IMG_STATUS, IMG_TIME, IMG_REPEAT, 0, 0
#ifdef __MMI_ALM_CUST_VOLUME__
		, ALARM_VOLUME_IMAGEID, 0
#endif
#ifdef __MMI_ALM_AUDIO_OPTIONS__
		, IMG_ALM_AUD_OPT, 0,0
#endif
#ifdef __MMI_ALM_SNOOZE_SETTING__ //qiff modify:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__
        , ALARMS_SET_SNOOZE_PERIOD_IMAGEID, 0
#endif 

	};

	nMenuItems = sizeof(alarm_icon_list) / sizeof(U16);
	EntryNewScreen(SCR_ID_ALM_EDITOR, ExitAlmEditScreen, NULL, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_ALM_EDITOR);
	InitializeCategory57Screen();
	RegisterHighlightHandler(AlmInlineEditHighlightHdlr);
	AlmFillInlineEditStruct();
	
	inputBuffer = GetCurrNInputBuffer (SCR_ID_ALM_EDITOR, &inputBufferSize);    //added for inline edit history
	
	if(inputBuffer != NULL)
		SetCategory57Data(wgui_inline_items, nMenuItems, inputBuffer);
#ifdef __MMI_ALM_AUDIO_OPTIONS__
	if(g_alm_aud_opt_selected)
		SetInlineDoneFlag(guiBuffer);
#endif
#ifdef __MMI_MAINLCD_220X176__
	ShowCategory57Screen(ALARM_EDIT_TITLE_STRINGID,
		0,
		ALARM_STR_DONE, IMG_GLOBAL_OK,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
		nMenuItems,alarm_icon_list,wgui_inline_items,
		/*currItemAlarmEdit*/0,guiBuffer);
#else
	ShowCategory57Screen(ALARM_EDIT_TITLE_STRINGID,GetRootTitleIcon(ORGANIZER_ALARM_MENU),
		ALARM_STR_DONE, IMG_GLOBAL_OK,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
		nMenuItems,alarm_icon_list,wgui_inline_items,
		/*currItemAlarmEdit*/0,guiBuffer);
#endif

	SetCategory57RightSoftkeyFunctions(EntryAlmSaveConfirmation,GoBackHistory);
}


/*****************************************************************************
* FUNCTION
*	ExitAlmEditScreen
* DESCRIPTION
*   Exit function for inline editor screen of alarm.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitAlmEditScreen(void)
{
	history_t   Scr; 
	S16 nHistory = 0;
	U16 inputBufferSize;                //added for inline edit history
 	
	Scr.scrnID = SCR_ID_ALM_EDITOR;
	CloseCategory57Screen();
	Scr.entryFuncPtr = EntryAlmEditScreen;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();	//added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer);		//added for inline edit history
	AddNHistory(Scr, inputBufferSize);				//added for inline edit history

}

/*****************************************************************************
* FUNCTION
*	AlmTimeEditorCallBack
* DESCRIPTION
*   Callback function for inline time editor.
* PARAMETERS
*	string_buffer	IN/OUT	buffer to store the result of time
*	hours_buffer	IN		buffer to store hour string
*	min_buffer		IN		buffer to store minute string
*	AM_PM_flag		IN		flag to indicate the time format of time string
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmTimeEditorCallBack(U8 *string_buffer,U8* hours_buffer,U8* min_buffer,U8* AM_PM_flag)
{
	pfnUnicodeStrcpy((S8*)string_buffer,(S8*)hours_buffer);
	pfnUnicodeStrcat((S8*)string_buffer,(S8*)":");
	pfnUnicodeStrcat((S8*)string_buffer,(S8*)min_buffer);
}


/*****************************************************************************
* FUNCTION
*	AlmFillInlineEditStructs
* DESCRIPTION
*   Initialize inline structure of input screen.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmFillInlineEditStruct()
{
	S32 inline_item_index = 0; 
#if defined(__MMI_ALM_AUDIO_OPTIONS__) && defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
    DYNEXTMELODYINFO *ExtMelodyInfo;
#endif

#ifdef __MMI_ALM_CUST_VOLUME__
	gAlmVolumeList[0] = (S8*)gAlmVolumeString[0];
	gAlmVolumeList[1] = (S8*)gAlmVolumeString[1];
	gAlmVolumeList[2] = (S8*)gAlmVolumeString[2];
	gAlmVolumeList[3] = (S8*)gAlmVolumeString[3];
	gAlmVolumeList[4] = (S8*)gAlmVolumeString[4];
	gAlmVolumeList[5] = (S8*)gAlmVolumeString[5];
	gAlmVolumeList[6] = (S8*)gAlmVolumeString[6];
#endif
//qiff modify:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__
#ifdef __MMI_ALM_SNOOZE_SETTING__
    gAlmSettingSnooze[0] = GetString((U16) STR_ID_ALM_DEFAULT);
    gAlmSettingSnooze[1] = GetString((U16) STR_ID_ALM_1);
    gAlmSettingSnooze[2] = GetString((U16) STR_ID_ALM_2);
    gAlmSettingSnooze[3] = GetString((U16) STR_ID_ALM_3);
    gAlmSettingSnooze[4] = GetString((U16) STR_ID_ALM_4);
    gAlmSettingSnooze[5] = GetString((U16) STR_ID_ALM_5);
    gAlmSettingSnooze[6] = GetString((U16) STR_ID_ALM_6);
    gAlmSettingSnooze[7] = GetString((U16) STR_ID_ALM_7);
    gAlmSettingSnooze[8] = GetString((U16) STR_ID_ALM_8);
    gAlmSettingSnooze[9] = GetString((U16) STR_ID_ALM_9);
    gAlmSettingSnooze[10] = GetString((U16) STR_ID_ALM_10);
#endif /* __MMI_ALM_SNOOZE_SETTING__ */ 


	/* ON/OFF */
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);	
	SetInlineItemSelect(&(wgui_inline_items[inline_item_index]),NUM_STATE_ACTINACT_ALARM,(U8**)gAlmOnOffString,&g_alm_cntx.CurrHiliteState/*(gAlmCurrHiliteState[g_alm_cntx.CurrHiliteAlarm])*/);
	DisableInlineItemHighlight(&(wgui_inline_items[inline_item_index]));

	/* time */
	inline_item_index++;
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,KEY_EVENT_UP);
	SetInlineItemTime(&(wgui_inline_items[inline_item_index]),(U8*)g_alm_cntx.HourBuf,(U8*)g_alm_cntx.MinBuf,NULL,AlmTimeEditorCallBack);
	DisableInlineItemHighlight(&(wgui_inline_items[inline_item_index]));

	/* repeat caption */
	inline_item_index++;
	SetInlineItemCaption(&(wgui_inline_items[inline_item_index]),(U8*)GetString(ALARM_REPEAT_STRING));

	/* repeat */
	inline_item_index++;
	SetInlineItemSelect(&(wgui_inline_items[inline_item_index]),NUM_FREQ_ALARM - 2,(U8**)gAlmFreqList,&g_alm_cntx.CurrHiliteFreq);/* -2 forweekdays and monthly*/
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);	
	RegisterInlineSelectHighlightHandler(&wgui_inline_items[inline_item_index],HighlightAlmInlineRepeat);

	/* day of week */
	if(g_alm_cntx.CurrHiliteFreq!=DAYS)	/* not Days */
	{
		memset(gAlmDaysStates,0,7);
	}

	inline_item_index++;
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemDOWSelect(&(wgui_inline_items[inline_item_index]),0,gAlmDaysStates);
#ifdef __MMI_MAINLCD_220X176__	
	DisableInlineItemHighlight(&(wgui_inline_items[inline_item_index]));
#endif	
	/* day of week */
	if(g_alm_cntx.CurrHiliteFreq!=DAYS)	/* not Days */
	{
		DisableInlineItem(&wgui_inline_items[inline_item_index],4);/* multi select days*/		
	}

#ifdef __MMI_ALM_CUST_VOLUME__
	/* volume */
	inline_item_index++;
	SetInlineItemCaption(&(wgui_inline_items[inline_item_index]),(U8*)GetString(ALARMS_VOLUME));
	inline_item_index++;
	SetInlineItemSelect(&(wgui_inline_items[inline_item_index]),(LEVEL7+1),(U8**)gAlmVolumeList,&g_alm_cntx.CurrHiliteVol);/* -2 forweekdays and monthly*/
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);	
#endif

	/*Customize Ring Tone and FM alarm*/
#ifdef __MMI_ALM_AUDIO_OPTIONS__
	/*Ring Tone Caption*/
	inline_item_index++;
	SetInlineItemCaption(&(wgui_inline_items[inline_item_index]),(U8*)GetString(STR_ID_ALM_AUDIO_OPTIONS));
	g_alm_audio_option_previndex = (g_alm_cntx.CurrHiliteAudioOption > ALM_AUDIO_OPTIONS_TONE) ? ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL : ALM_AUDIO_OPTIONS_TONE;
	inline_item_index++;

	/*Ring Tone Inline Selectt*/
	SetInlineItemSelect(&(wgui_inline_items[inline_item_index]),NUM_AUDIO_OPTIONS,(U8**)g_alm_audio_options_string,&g_alm_audio_option_previndex);
	SetInlineItemActivation(&(wgui_inline_items[inline_item_index]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	RegisterInlineSelectHighlightHandler(&wgui_inline_items[inline_item_index],HighlightAlmInlineAudioOptions);
	inline_item_index++;
	
	g_alm_aud_opt_curselitem_inline_index = inline_item_index;
	
	if(g_alm_cntx.CurrHiliteAudioOption == ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL)
	{
		if(g_alm_cntx.CurrHiliteAudioOptionValue > 0)
		{
			if(pfnUnicodeStrlen((S8*)g_alm_cntx.CurrHiliteAlmFMChannelName))
			{
				pfnUnicodeStrcpy((S8*)g_alm_audio_options_curselitem, (S8*)g_alm_cntx.CurrHiliteAlmFMChannelName);
			}
			else
			{
				kal_wsprintf( g_alm_audio_options_curselitem, "FM %d.%d", g_alm_cntx.CurrHiliteAudioOptionValue / 10, g_alm_cntx.CurrHiliteAudioOptionValue % 10 );
			}
		}
		else
		{
			pfnUnicodeStrcpy((S8*)g_alm_audio_options_curselitem,GetString(STR_GLOBAL_EMPTY_LIST));
		}
		
		SetInlineItemActivation(&(wgui_inline_items[inline_item_index]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
		SetInlineItemDisplayOnly(&(wgui_inline_items[inline_item_index]),(U8*)g_alm_audio_options_curselitem);
	}
	else if(g_alm_cntx.CurrHiliteAudioOption == ALM_AUDIO_OPTIONS_FMRADIO_MANUAL)
	{
		if(g_alm_cntx.CurrHiliteAudioOptionValue > 0)
		{
			kal_wsprintf( g_alm_audio_options_curselitem, "FM %d.%d", g_alm_cntx.CurrHiliteAudioOptionValue / 10, g_alm_cntx.CurrHiliteAudioOptionValue % 10 );		
		}
		else
		{
			pfnUnicodeStrcpy((S8*)g_alm_audio_options_curselitem,GetString(STR_GLOBAL_EMPTY_LIST));
		}
		
		SetInlineItemActivation(&(wgui_inline_items[inline_item_index]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
		SetInlineItemDisplayOnly(&(wgui_inline_items[inline_item_index]),(U8*)g_alm_audio_options_curselitem);
	}
	else
	{
#ifdef __MMI_ALM_CUSTOMIZE_TONE__
		U16 sel_tone = g_alm_cntx.CurrHiliteAudioOptionValue;
#else
		U16 sel_tone = gcurrentprofile.toneSetup.alarmTone;
#endif
		U8 tone_index;

		SetInlineItemActivation(&(wgui_inline_items[inline_item_index]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
		
		/*Ring Tone as Alarm*/
		if( (sel_tone >= gstartRingId) && (sel_tone < (gstartRingId + gtotalRingId - 1)))
		{
			tone_index = sel_tone - gstartRingId;
			SetInlineItemDisplayOnly(&(wgui_inline_items[inline_item_index]),(U8*)GetString((U16)(STR_PROFILES_RING_1 + tone_index)));
		}
		/*Midi Tone as Alarm*/
		else if((sel_tone >= gstartMidiId) && (sel_tone <= (gstartMidiId + gtotalMidiId - 1)))
		{
			tone_index = (sel_tone - gstartMidiId + gtotalRingId);
			SetInlineItemDisplayOnly(&(wgui_inline_items[inline_item_index]),(U8*)GetString((U16)(STR_PROFILES_RING_1 + tone_index)));			
		}
		/*External Resource as Alarm*/
#if defined( __MMI_PROFILE_EXTMELODY_SUPPORT__ )
		else if(sel_tone >= PMG_EXT_MELODY_BEGIN && sel_tone <= PMG_EXT_MELODY_END )
		{
			tone_index = sel_tone - PMG_EXT_MELODY_BEGIN;
			PmgGetExtMelodyInfo(&ExtMelodyInfo);
			SetInlineItemDisplayOnly(&(wgui_inline_items[inline_item_index]),(ExtMelodyInfo->info[tone_index]).filename);
			return;
		}
#endif
		/*Ring tone range incorrect or not initialized*/
		else
		{
			pfnUnicodeStrcpy((S8*)g_alm_audio_options_curselitem,GetString(STR_GLOBAL_EMPTY_LIST));
			SetInlineItemDisplayOnly(&(wgui_inline_items[inline_item_index]),(U8*)g_alm_audio_options_curselitem);			
		}
	}
	
#endif //#ifdef __MMI_ALM_AUDIO_OPTIONS__
#ifdef __MMI_ALM_SNOOZE_SETTING__ //qiff modify:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__
    inline_item_index++;
    SetInlineItemActivation(&(wgui_inline_items[inline_item_index]), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(
        &(wgui_inline_items[inline_item_index]),
        (U8*) GetString((U16) ALARMS_SETTING_SNOOZE_INTERVAL));

    inline_item_index++;
    SetInlineItemActivation(&(wgui_inline_items[inline_item_index]), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(
        &(wgui_inline_items[inline_item_index]),
        (S32) ALM_SNOOZE_INTERVAL_SETTING_NUM,
        (U8 **) gAlmSettingSnooze,
        &(g_alm_cntx.CurrHilightSnoozeInterval));
    /* RegisterInlineSelectHighlightHandler(&wgui_inline_items[SNOOZE_INTERVAL],mmi_hightlight_snooze_selection); */
#endif /* __MMI_ALM_SNOOZE_SETTING__ */ 

}


/*****************************************************************************
* FUNCTION
*	AlmSaveInlineEdit
* DESCRIPTION
*	Save alarm to nvram and set/cancel alarm to driver accordingly.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void AlmSaveInlineEdit(void)
{
	if(AlmIsInlineEditorChanged(g_alm_cntx.CurrHiliteAlarm)) 
	{
		S32 i,j;		
		i = g_alm_cntx.CurrHiliteAlarm;
		
		g_alm_cntx.AlmList[i].Freq = (U8)g_alm_cntx.CurrHiliteFreq;
		g_alm_cntx.AlmList[i].Days = 0;
		
		if(g_alm_cntx.AlmList[i].Freq == DAYS)
		{
			for(j=0;j<NUM_DAYS;j++) 
				g_alm_cntx.AlmList[i].Days |= (gAlmDaysStates[j] << j);

			if(g_alm_cntx.AlmList[i].Days == 0)
				g_alm_cntx.AlmList[i].Freq = ONCE;
		}		

		g_alm_cntx.AlmList[i].State = (U8)g_alm_cntx.CurrHiliteState;
		g_alm_cntx.AlmList[i].Hour = (g_alm_cntx.HourBuf[0]-'0')* 10 + (g_alm_cntx.HourBuf[ENCODING_LENGTH]-'0');
		g_alm_cntx.AlmList[i].Min = (g_alm_cntx.MinBuf[0]-'0')* 10 + (g_alm_cntx.MinBuf[ENCODING_LENGTH]-'0');
		g_alm_cntx.AlmList[i].Snooze = 0;

#ifdef __MMI_ALM_CUST_VOLUME__
		g_alm_cntx.AlmList[i].Volume = (U8)g_alm_cntx.CurrHiliteVol;
#endif

		/*Alarm Audio Setting*/
#ifdef __MMI_ALM_AUDIO_OPTIONS__
		if(g_alm_cntx.CurrHiliteAudioOptionValue != 0)
		{
			g_alm_cntx.AlmList[i].AudioOption = (U8)g_alm_cntx.CurrHiliteAudioOption;
			g_alm_cntx.AlmList[i].AudioOptionValue = g_alm_cntx.CurrHiliteAudioOptionValue;
		}
		else /*Should not enter here because it already check before save*/
		{
			g_alm_cntx.AlmList[i].AudioOption = ALM_AUDIO_OPTIONS_TONE;

#ifdef __MMI_ALM_CUSTOMIZE_TONE__
			g_alm_cntx.AlmList[i].AudioOptionValue = gstartRingId;
#else
			g_alm_cntx.AlmList[i].AudioOptionValue = gcurrentprofile.toneSetup.alarmTone;
#endif			
		}

		/*
		*  flag for indicating that the alarm has been edited manually through the edit alarm screen, 
		*  would be reset once the alarm is written to NVRAM.
		*/
		g_alm_audio_options_alarm_edited_manually = TRUE;
#endif
    #ifdef __MMI_ALM_SNOOZE_SETTING__ //qiff modify:Bug 10667:add __MMI_ALM_SNOOZE_SETTING__
        g_alm_cntx.AlmList[i].SnoozeInterval = (U8) g_alm_cntx.CurrHilightSnoozeInterval;
    #endif 
		AlmWriteToNvram();
	
		if(g_alm_cntx.CurrHiliteState == SET_STATE) 
		{
			AlmSetAlarm((U8)(i+ALM_ALM_START));
			AlmActivateIndicator();
		}
		else 
		{
			AlmCancelAlarm((U8)(i+ALM_ALM_START));
			if(!(AlmIsAnyAlarmActivated()))
				AlmDeactivateIndicator();			
		}
	}
#ifdef __MMI_ALM_AUDIO_OPTIONS__
	
	if((g_alm_cntx.AlmList[g_alm_cntx.CurrHiliteAlarm].AudioOption == ALM_AUDIO_OPTIONS_FMRADIO_CHANNEL) || (g_alm_cntx.AlmList[g_alm_cntx.CurrHiliteAlarm].AudioOption == ALM_AUDIO_OPTIONS_FMRADIO_MANUAL))
	{
		DisplayConfirm(STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  0, 0,
					get_string(STR_ID_ALM_FMRDO_HEADFONE_REMINDER),0,WARNING_TONE);
		SetLeftSoftkeyFunction(mmi_alm_display_popup,KEY_EVENT_UP);
	}
	else
#endif
	{
 	DisplayPopup((U8*)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteUptoScrID(SCR_ID_ALM_LIST);
}
}

#ifdef __MMI_ALM_AUDIO_OPTIONS__
void mmi_alm_display_popup(void)
{
	DisplayPopup((U8*)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteUptoScrID(SCR_ID_ALM_LIST);
}
#endif

/*****************************************************************************
* FUNCTION
*	AlmCancelAlarmFromAT
* DESCRIPTION
*	Handler to cancel alarm from AT command.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void AlmCancelAlarmFromAT(U8 index)
{
	if(g_alm_cntx.AlmList[index].State == 1) /* alarm is currently on */
		AlmCancelAlarm((U8)(index+ALM_ALM_START));

	memset(&g_alm_cntx.AlmList[index], 0, sizeof(alm_nvram_struct));
	AlmWriteToNvram();

	if(!(AlmIsAnyAlarmActivated()))
		AlmDeactivateIndicator();	
	
}


/*****************************************************************************
* FUNCTION
*	AlmSetAlarmFromAT
* DESCRIPTION
*	Handler to process alarm request from AT command
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void AlmSetAlarmFromAT(U8 index, MYTIME mytime, U8 weekdays) 
{
	g_alm_cntx.AlmList[index].Hour = mytime.nHour;
	g_alm_cntx.AlmList[index].Min = mytime.nMin;
	g_alm_cntx.AlmList[index].Snooze = 0;
	g_alm_cntx.AlmList[index].State = 1;

	if(weekdays == 0)
		g_alm_cntx.AlmList[index].Freq = ONCE;
	else if(weekdays == 0x7F)
		g_alm_cntx.AlmList[index].Freq = EVERYDAY;
	else
	{
		g_alm_cntx.AlmList[index].Freq = DAYS;
		g_alm_cntx.AlmList[index].Days = weekdays;
	}
	
	AlmWriteToNvram();
	
	AlmSetAlarm((U8)(index+ALM_ALM_START));

	AlmActivateIndicator();

}


/*****************************************************************************
* FUNCTION
*	AlmQueryAlarmFromAT
* DESCRIPTION
*	Handler to process query alarm request from AT command
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	External global.
*****************************************************************************/
void AlmQueryAlarmFromAT(void)
{
#ifdef MMI_ON_HARDWARE_P
	mmi_at_alarm_query_res_req_struct* rsp;
	MYQUEUE	Message;
	S32 i,j;
	MYTIME	almTime;

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
 	Message.oslMsgId = MSG_ID_MMI_AT_ALARM_QUERY_RES_REQ;
	rsp =  (mmi_at_alarm_query_res_req_struct *) 
               OslConstructDataPtr(sizeof(mmi_at_alarm_query_res_req_struct));	

	rsp->totalAlarm = NUM_OF_ALM;
	rsp->alarmList = NULL;
	rsp->activeAlarm = 0;
	
	for(i=0; i<NUM_OF_ALM; i++)
		if(g_alm_cntx.AlmList[i].State == SET_STATE)
			rsp->activeAlarm++;

	if(rsp->activeAlarm)
	{
		rsp->alarmList =  (mmi_at_alarm_info_struct *) 
              	 OslConstructDataPtr(sizeof(mmi_at_alarm_info_struct)*rsp->activeAlarm);	
		
		for(i=0, j=0; i<NUM_OF_ALM; i++)
		{
			if(g_alm_cntx.AlmList[i].State == SET_STATE)
			{
				rsp->alarmList[j].alarm_index = i;

				almTime = AlmGetAlarmTime(i+ALM_ALM_START);

				rsp->alarmList[j].date_time.rtc_year = almTime.nYear-YEARFORMATE;
				rsp->alarmList[j].date_time.rtc_wday = almTime.DayIndex;
				rsp->alarmList[j].date_time.rtc_mon = almTime.nMonth;
				rsp->alarmList[j].date_time.rtc_day = almTime.nDay;
				rsp->alarmList[j].date_time.rtc_hour = almTime.nHour;
				rsp->alarmList[j].date_time.rtc_min = almTime.nMin;
				rsp->alarmList[j].date_time.rtc_sec = almTime.nSec;

				if(g_alm_cntx.AlmList[i].Freq == ONCE)
					rsp->alarmList[j].recurr = 0;
				else if(g_alm_cntx.AlmList[i].Freq == EVERYDAY)
					rsp->alarmList[j].recurr = ALM_ALL_DAYS;
				else
					rsp->alarmList[j].recurr = g_alm_cntx.AlmList[i].Days;
				
				j++;
			}
		}
	}

	Message.oslDataPtr = (oslParaType *)rsp;
	Message.oslPeerBuffPtr = NULL;
	OslMsgSendExtQueue(&Message);	
#endif
}

#endif // #ifndef _MMI_ALARM_C


