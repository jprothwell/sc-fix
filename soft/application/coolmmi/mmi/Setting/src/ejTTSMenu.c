#include "mmi_features.h"

#ifdef __MMI_TTS_FEATURES__ 

#include "stdc.h"
#include "l4dr1.h"
#include "l4dr.h"
#include "wgui_categories.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
#include "settingstructs.h"
#include "settinggprots.h"
#include "protocolevents.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "nvram_user_defs.h"
#include "commonscreens.h"
#include "settingprot.h"
#include "settingprofile.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "debuginitdef.h"
#include "mci.h"
#include "globaldefs.h"
#include "mainmenudef.h"

#include "ejTTSMenu.h"




nvram_tts_setting_struct g_tts_setting;

#define MAX_TTS_VOICE_ALARM_ITME    3
#define MAX_TTS_VOICE_EFFECT_ITEM   5


S32 g_curTTSAlertHourStatus;
S32 g_curTTSAlertWeekStatus;	
S32 g_curTTSAlertDateStatus;
	S32 g_curTTSVolumeStatus;
S32 g_curTTSSpeedStatus;
S32 g_curTTSEffectStatus;
S32 g_curTTSStyleStatus;
S32 g_curTTSTuneStatus;

UI_string_type g_tts_alarm_str[2];
  UI_string_type  g_tts_Effect_volume_str[5];

 UI_string_type  g_tts_Effect_speed_str[3];
UI_string_type  g_tts_Effect_effect_str[6];
 UI_string_type  g_tts_Effect_style_str[4];
 UI_string_type  g_tts_Effect_tune_str[3];

	 
extern	void mmi_settings_tts_read_from_nvram();
/**
* the init function
*
*/
void InitTTSPlayer( void )
{
	mmi_trace(1, "tts test, init tts player");
	
	SetHiliteHandler( MENU_SETTING_TTS_PLAYER, mmi_settings_sinovoice_highlight_hdlr );

	SetHiliteHandler( MENU_ID_TTS_READ_MSG, mmi_settings_tts_read_msg_highlight_hdlr );
	SetHintHandler( MENU_ID_TTS_READ_MSG, mmi_settings_tts_read_msg_hint_hdlr );

	SetHiliteHandler( MENU_ID_TTS_READ_MENU, mmi_settings_tts_read_menu_highlight_hdlr );
	SetHintHandler( MENU_ID_TTS_READ_MENU,mmi_settings_tts_read_menu_hint_hdlr);
	
	SetHiliteHandler( MENU_ID_TTS_READ_CALL, mmi_settings_tts_read_call_highlight_hdlr );
	SetHintHandler( MENU_ID_TTS_READ_CALL,	mmi_settings_tts_read_call_hint_hdlr);
	
	SetHiliteHandler( MENU_ID_TTS_SETTING, mmi_settings_tts_setting_highlight_hdlr );

	SetHiliteHandler( MENU_ID_TTS_ALARM_SETTING, mmi_settings_tts_voice_alarm_highlight_hdlr );

	SetHiliteHandler( MENU_ID_TTS_HELP, mmi_settings_tts_help_highlight_hdlr );

	mmi_settings_tts_read_from_nvram(&g_tts_setting);
	
}

/*
* end the init function
*
*/

/*
* add by liuwen
* begin sinovoice setting
*/

void mmi_settings_sinovoice_highlight_hdlr( void )
{
	mmi_trace(1, "tts test, the ejTTS hight light hdlr");
	ChangeLeftSoftkey( STR_GLOBAL_OK, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, 0 );
	SetLeftSoftkeyFunction( mmi_settings_entry_sinovoice_list, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

void mmi_settings_entry_sinovoice_list( void )
{
	U16 n_items;
	U8* guiBuffer;
	U16 item_strs[10];
	U16 item_icons[10];
	U8 *item_hints[10];
	int i;

	mmi_trace(1, "tts test, entry tts menu list");

	EntryNewScreen( SCR_ID_SETTING_TTS_PLAYER, NULL, mmi_settings_entry_sinovoice_list, NULL );

	guiBuffer = GetCurrGuiBuffer( SCR_ID_SETTING_TTS_PLAYER );	
	n_items = GetNumOfChild( MENU_SETTING_TTS_PLAYER);

	GetSequenceStringIds( MENU_SETTING_TTS_PLAYER, item_strs );
	GetSequenceImageIds( MENU_SETTING_TTS_PLAYER, item_icons );
	for(i=0;i<n_items;i++)
	{
		item_hints[i]=NULL;
		item_icons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);

	}

	SetParentHandler( MENU_SETTING_TTS_PLAYER );

	RegisterHighlightHandler( ExecuteCurrHiliteHandler );
	ConstructHintsList( MENU_SETTING_TTS_PLAYER, (U8**) item_hints );

	ShowCategory52Screen( STR_ID_SETTING_TTS_PLAYER, IMG_SCR_SETTING_CAPTION,
		STR_GLOBAL_OK, IMG_GLOBAL_OK,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
		n_items, item_strs, item_icons, (U8**)item_hints,
		LIST_MENU, 0, guiBuffer );

	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

/*
* end sinovoice setting
*
*/

/*
*  
* begin voice setting
*
*/

void mmi_settings_tts_setting_highlight_hdlr( void )
{
	ChangeLeftSoftkey( STR_GLOBAL_OK, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, 0 );
	SetLeftSoftkeyFunction( mmi_settings_tts_voice_Effect_Setting_Screen, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

void mmi_exit_TTS_Voice_Effect_Setting_screen(void)
{
	U16 		inputBufferSize;                
	history_t 	h;
	S16		nHistory = 0;

	h.scrnID = SCR_ID_TTS_EFFECT_SETTING;
	CloseCategory57Screen();
	h.entryFuncPtr = mmi_settings_tts_voice_Effect_Setting_Screen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer,(S8*)&nHistory);
	GetCategory57History (h.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data ((U8*)h.inputBuffer);
	AddNHistory(h, inputBufferSize);

}

void mmi_tts_voice_Effect_Setting_done_hdlr(void)
{

//	mmi_java_store_setting();
	g_tts_setting.curTTSVolumeStatus = g_curTTSVolumeStatus;
	g_tts_setting.curTTSSpeedStatus = g_curTTSSpeedStatus;
	g_tts_setting.curTTSEffectStatus = g_curTTSEffectStatus;
	g_tts_setting.curTTSStyleStatus  = g_curTTSStyleStatus;
	g_tts_setting.curTTSTuneStatus  = g_curTTSTuneStatus;

	mmi_settings_tts_write_to_nvram(&g_tts_setting);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	DeleteNHistory(1);
}


void mmi_tts_voice_Effect_Setting_highlight_hdlr(S32 index)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(mmi_tts_voice_Effect_Setting_done_hdlr);
}

void mmi_settings_tts_voice_Effect_Setting_Screen(void)
{

	U8		*guiBuffer;
	U8		*inputBuffer;
	U16	inputBufferSize;
	S32	item_offset = 0;

	InlineItem	inline_tts_items[MAX_TTS_VOICE_EFFECT_ITEM*2];  /* one for caption, one for selection */
	UI_image_ID_type item_icons[] =
	{
	IMG_GLOBAL_L1,
		0,
	IMG_GLOBAL_L2,
	0,
	IMG_GLOBAL_L3,
	0,
	IMG_GLOBAL_L4,
	0,
	IMG_GLOBAL_L5,
	0

	};

	
	/* load setting */
	//mmi_java_load_setting();

	EntryNewScreen(SCR_ID_TTS_EFFECT_SETTING, mmi_exit_TTS_Voice_Effect_Setting_screen, NULL, NULL);
	
	
   	g_tts_Effect_volume_str[0] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_VOLUME_1));
   	g_tts_Effect_volume_str[1] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_VOLUME_2));
	g_tts_Effect_volume_str[2] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_VOLUME_3));
	g_tts_Effect_volume_str[3] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_VOLUME_4));
	g_tts_Effect_volume_str[4] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_VOLUME_5));


   	g_tts_Effect_speed_str[0] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_SPEED_FAST));
   	g_tts_Effect_speed_str[1] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_SPEED_NOR));
	g_tts_Effect_speed_str[2] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_SPEED_SLOW));


   	g_tts_Effect_effect_str[0] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_EFFECT_BASE));
   	g_tts_Effect_effect_str[1] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_EFFECT_CHORUS));
	g_tts_Effect_effect_str[2] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_EFFECT_ECHO));
   	g_tts_Effect_effect_str[3] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_EFFECT_NEARFAR));
   	g_tts_Effect_effect_str[4] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_EFFECT_REVERB));
	g_tts_Effect_effect_str[5] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_EFFECT_ROBOT));

   	g_tts_Effect_style_str[0] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_STYLE_CLEAR));
   	g_tts_Effect_style_str[1] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_STYLE_NORMAL));
	g_tts_Effect_style_str[2] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_STYLE_PLAIN));
   	g_tts_Effect_style_str[3] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_STYLE_VIVID));

        	g_tts_Effect_tune_str[0] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_TUNE_DEEP));
   	g_tts_Effect_tune_str[1] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_TUNE_NORMAL));
	g_tts_Effect_tune_str[2] = (UI_string_type)GetString((U16)(STR_ID_TTS_SETTING_TUNE_STRIDENT));

	mmi_settings_tts_read_from_nvram(&g_tts_setting);
	g_curTTSVolumeStatus = g_tts_setting.curTTSVolumeStatus;
	g_curTTSSpeedStatus = g_tts_setting.curTTSSpeedStatus ;
	g_curTTSEffectStatus = g_tts_setting.curTTSEffectStatus;
	g_curTTSStyleStatus = g_tts_setting.curTTSStyleStatus;
	g_curTTSTuneStatus = g_tts_setting.curTTSTuneStatus;


	memset((void*)inline_tts_items, 0, sizeof(InlineItem)*MAX_TTS_VOICE_EFFECT_ITEM*2);

	/* tts effect volume */
	SetInlineItemCaption(&inline_tts_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_TTS_SETTING_VOLUME)));

	SetInlineItemActivation(&inline_tts_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&inline_tts_items[item_offset++],
								5,
								(U8**)g_tts_Effect_volume_str,
								&g_curTTSVolumeStatus);

	/* tts alarm weekday */
	SetInlineItemCaption(&inline_tts_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_TTS_SETTING_SPEED)));

	SetInlineItemActivation(&inline_tts_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&inline_tts_items[item_offset++],
								3,
								(U8**)g_tts_Effect_speed_str,
								&g_curTTSSpeedStatus);

        /*tts alarm Date*/
        SetInlineItemCaption( &inline_tts_items[item_offset++],
                                                (PU8) GetString((U16) (STR_ID_TTS_SETTING_EFFECT)));

        SetInlineItemActivation(   &inline_tts_items[item_offset],
                                                  INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
                                                     0);

        SetInlineItemSelect(     &inline_tts_items[item_offset++],
					            6,
					            (U8 **) g_tts_Effect_effect_str,
					            &g_curTTSEffectStatus);


	/* tts alarm weekday */
	SetInlineItemCaption(&inline_tts_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_TTS_SETTING_STYLE)));

	SetInlineItemActivation(&inline_tts_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&inline_tts_items[item_offset++],
								4,
								(U8**)g_tts_Effect_style_str,
								&g_curTTSStyleStatus);

	/* tts alarm weekday */
	SetInlineItemCaption(&inline_tts_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_TTS_SETTING_TUNE)));

	SetInlineItemActivation(&inline_tts_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&inline_tts_items[item_offset++],
								3,
								(U8**)g_tts_Effect_tune_str,
								&g_curTTSTuneStatus);



	guiBuffer	= GetCurrGuiBuffer(SCR_ID_TTS_EFFECT_SETTING);
	inputBuffer = GetCurrNInputBuffer(SCR_ID_TTS_EFFECT_SETTING, &inputBufferSize );

	InitializeCategory57Screen();

	if(inputBuffer != NULL)		
		SetCategory57Data(inline_tts_items ,MAX_TTS_VOICE_EFFECT_ITEM*2, inputBuffer);	

	RegisterHighlightHandler(mmi_tts_voice_Effect_Setting_highlight_hdlr);

	/* show category */
	ShowCategory57Screen(STR_ID_SETTING_TTS_SETTING, MAIN_MENU_TITLE_SETTINGS_ICON,		
								STR_GLOBAL_OK, IMG_GLOBAL_OK,											
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,									
								item_offset,
								item_icons, 
								inline_tts_items,
								0, guiBuffer);

	/* set key hdlr */
	SetCategory57LeftSoftkeyFunction(mmi_tts_voice_Effect_Setting_done_hdlr);
	SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
	DisableCategory57ScreenDone();	
}


/*
* end voice setting
*
*/

/*
* add by liuwen
* begin read msg setting
*/

void mmi_settings_tts_read_msg_highlight_hdlr( void )
{
	mmi_trace(1, "tts test, the read msg hight light hdlr");
	ChangeLeftSoftkey( STR_GLOBAL_OPEN, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, 0 );
	SetLeftSoftkeyFunction( mmi_settings_entry_tts_read_msg_list, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}
void mmi_settings_tts_read_msg_hint_hdlr( U16 index )
{
	memset( (S8 *)hintData[(index)], 0, MAX_SUB_MENU_SIZE );
	if(g_tts_setting.curTTSMsgReadStatus == 0) 
	{
		pfnUnicodeStrcpy( (S8 *)hintData[(index)], GetString( (U16)(STR_ID_TTS_READ_MSG_AUTO) ) );
	}
	else 	if(g_tts_setting.curTTSMsgReadStatus > 0) 
	{
		pfnUnicodeStrcpy( (S8 *)hintData[(index)], GetString( (U16)(STR_ID_TTS_READ_MSG_CONTROL) ) );
	}
}


void mmi_settings_entry_tts_read_msg_list( void )
{
	U8* guiBuffer;
	U16 n_items;
	U16 item_strs[10];
	U16 item_icons[10];
	int i;



	mmi_settings_tts_read_from_nvram(&g_tts_setting);
	mmi_trace(1, "tts test, entruy tts read msg list= %d", g_tts_setting.curTTSMsgReadStatus);
	
	EntryNewScreen( SCR_ID_TTS_READ_MSG, NULL, mmi_settings_entry_tts_read_msg_list, NULL );

	guiBuffer = GetCurrGuiBuffer( SCR_ID_TTS_READ_MSG );	

	n_items = GetNumOfChild( MENU_ID_TTS_READ_MSG);

	GetSequenceStringIds( MENU_ID_TTS_READ_MSG, item_strs );
	GetSequenceImageIds( MENU_ID_TTS_READ_MSG, item_icons );
	
	SetParentHandler( MENU_ID_TTS_READ_MSG);
//	RegisterHighlightHandler( mmi_settings_tts_read_msg_list_highlight_hdlr );
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory11Screen( STR_ID_SETTING_TTS_READ_MSG, IMG_SCR_SETTING_CAPTION,
		STR_GLOBAL_OK, IMG_GLOBAL_OK,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
		n_items, item_strs, g_tts_setting.curTTSMsgReadStatus, guiBuffer );

	SetLeftSoftkeyFunction( mmi_settings_tts_read_msg_list_LSK_hdlr, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}

void mmi_settings_tts_read_msg_list_highlight_hdlr( S32 index )
{
	mmi_trace(1, "tts test, entruy tts read msg list highlight hdlr");
}

void mmi_settings_tts_read_msg_list_LSK_hdlr( void )
{

	g_tts_setting.curTTSMsgReadStatus = GetCurrHiliteID();
	mmi_trace(1, "tts test, entruy tts read msg list lsk hdlr =%d",g_tts_setting.curTTSMsgReadStatus );
	 
	mmi_settings_tts_write_to_nvram(&g_tts_setting);
	DisplayPopup( (PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE );
	DeleteScreenIfPresent( SCR_ID_TTS_READ_MSG);
}

/*
* end read msg setting
*
*/

/*
* add by liuwen
* begin read menu setting
*/

void mmi_settings_tts_read_menu_highlight_hdlr( void )
{
	mmi_trace(1, "tts test, the read msg hight light hdlr");
	ChangeLeftSoftkey( STR_GLOBAL_OPEN, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, 0 );
	SetLeftSoftkeyFunction( mmi_settings_entry_tts_read_menu_list, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	//SetKeyHandler( mmi_settings_entry_sound_effect_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN) ;
	//SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}


void mmi_settings_tts_read_menu_hint_hdlr( U16 index )
{
	memset( (S8 *)hintData[(index)], 0, MAX_SUB_MENU_SIZE );
	if(g_tts_setting.curTTSMenuReadStatus == 0) 
	{
		pfnUnicodeStrcpy( (S8 *)hintData[(index)], GetString( (U16)(STR_ID_TTS_READ_CALL_OPEN) ) );
	}
	else 	if(g_tts_setting.curTTSMenuReadStatus >0) 
	{
		pfnUnicodeStrcpy( (S8 *)hintData[(index)], GetString( (U16)(STR_ID_TTS_READ_CALL_CLOSE) ) );
	}
}


void mmi_settings_entry_tts_read_menu_list( void )
{
	U8* guiBuffer;
	U16 n_items;
	U16 item_strs[10];
	U16 item_icons[10];
	int i;


	mmi_trace(1, "tts test, entruy tts read msg list");

	mmi_settings_tts_read_from_nvram(&g_tts_setting);
	
	EntryNewScreen( SCR_ID_TTS_READ_MENU, NULL, mmi_settings_entry_tts_read_menu_list, NULL );

	guiBuffer = GetCurrGuiBuffer( SCR_ID_TTS_READ_MENU );	

	n_items = GetNumOfChild( MENU_ID_TTS_READ_MENU);

	GetSequenceStringIds( MENU_ID_TTS_READ_MENU, item_strs );
	GetSequenceImageIds( MENU_ID_TTS_READ_MENU, item_icons );

	SetParentHandler( MENU_ID_TTS_READ_MENU);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory11Screen( STR_ID_SETTING_TTS_READ_MENU, IMG_SCR_SETTING_CAPTION,
		STR_GLOBAL_OK, IMG_GLOBAL_OK,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
		n_items, item_strs, g_tts_setting.curTTSMenuReadStatus, guiBuffer );

	SetLeftSoftkeyFunction( mmi_settings_tts_read_menu_list_LSK_hdlr, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	//SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

void mmi_settings_tts_read_menu_list_highlight_hdlr( S32 index )
{
	mmi_trace(1, "tts test, entruy tts read msg list highlight hdlr");
}

void mmi_settings_tts_read_menu_list_LSK_hdlr( void )
{

	mmi_trace(1, "tts test, entruy tts read msg list lsk hdlr");
	g_tts_setting.curTTSMenuReadStatus = GetCurrHiliteID();
	mmi_settings_tts_write_to_nvram(&g_tts_setting);
	DisplayPopup( (PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE );
	DeleteScreenIfPresent( SCR_ID_TTS_READ_MENU);
}

/*
* end read menu setting
*
*/

/*
* add by liuwen
* begin read msg setting
*/

void mmi_settings_tts_read_call_highlight_hdlr( void )
{
	mmi_trace(1, "tts test, the read msg hight light hdlr");
	ChangeLeftSoftkey( STR_GLOBAL_OPEN, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, 0 );
	SetLeftSoftkeyFunction( mmi_settings_entry_tts_read_call_list, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	//SetKeyHandler( mmi_settings_entry_sound_effect_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN) ;
	//SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

void mmi_settings_tts_read_call_hint_hdlr( U16 index )
{
	memset( (S8 *)hintData[(index)], 0, MAX_SUB_MENU_SIZE );
	if(g_tts_setting.curTTSRingIncommingCallStatus == 0) 
	{
		pfnUnicodeStrcpy( (S8 *)hintData[(index)], GetString( (U16)(STR_ID_TTS_READ_CALL_OPEN) ) );
	}
	else 	if(g_tts_setting.curTTSRingIncommingCallStatus >0) 
	{
		pfnUnicodeStrcpy( (S8 *)hintData[(index)], GetString( (U16)(STR_ID_TTS_READ_CALL_CLOSE) ) );
	}
}

void mmi_settings_entry_tts_read_call_list( void )
{
	U8* guiBuffer;
	U16 n_items;
	U16 item_strs[10];
	U16 item_icons[10];
	int i;

	mmi_trace(1, "tts test, entruy tts read msg list");

	mmi_settings_tts_read_from_nvram(&g_tts_setting);
	
	EntryNewScreen( SCR_ID_TTS_READ_CALL, NULL, mmi_settings_entry_tts_read_call_list, NULL );

	guiBuffer = GetCurrGuiBuffer( SCR_ID_TTS_READ_CALL );	
		n_items = GetNumOfChild( MENU_ID_TTS_READ_CALL);

	GetSequenceStringIds( MENU_ID_TTS_READ_CALL, item_strs );
	GetSequenceImageIds( MENU_ID_TTS_READ_CALL, item_icons );

	SetParentHandler( MENU_ID_TTS_READ_CALL);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory11Screen( STR_ID_SETTING_TTS_READ_CALL, IMG_SCR_SETTING_CAPTION,
		STR_GLOBAL_OK, IMG_GLOBAL_OK,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
		n_items, item_strs, g_tts_setting.curTTSRingIncommingCallStatus, guiBuffer );

	SetLeftSoftkeyFunction( mmi_settings_tts_read_call_list_LSK_hdlr, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	//SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

void mmi_settings_tts_read_call_list_highlight_hdlr( S32 index )
{
	mmi_trace(1, "tts test, entruy tts read msg list highlight hdlr");
	g_tts_setting.curTTSRingIncommingCallStatus = (U16) index;
}

void mmi_settings_tts_read_call_list_LSK_hdlr( void )
{

	mmi_trace(1, "tts test, entruy tts read msg list lsk hdlr");
	g_tts_setting.curTTSRingIncommingCallStatus = GetCurrHiliteID();
	mmi_settings_tts_write_to_nvram(&g_tts_setting);
	DisplayPopup( (PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE );
	DeleteScreenIfPresent( SCR_ID_TTS_READ_CALL);
}

/*
* end read msg setting
*
*/

/*
* add by liuwen
* begin voice alarm setting
*/

void mmi_settings_tts_voice_alarm_highlight_hdlr( void )
{
	mmi_trace(1, "tts test, the voice alarm hight light hdlr");
	ChangeLeftSoftkey( STR_GLOBAL_OPEN, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, 0 );
	SetLeftSoftkeyFunction( mmi_settings_tts_voice_alarm_Screen, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}
void mmi_exit_TTS_Voice_Alarm_screen(void)
{
	U16 		inputBufferSize;                
	history_t 	h;
	S16		nHistory = 0;

	h.scrnID = SCR_ID_TTS_ALARM_SETTING;
	CloseCategory57Screen();
	h.entryFuncPtr = mmi_settings_tts_voice_alarm_Screen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer,(S8*)&nHistory);
	GetCategory57History (h.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data ((U8*)h.inputBuffer);
	AddNHistory(h, inputBufferSize);

}

void mmi_tts_setting_Alarm_done_hdlr(void)
{

//	mmi_java_store_setting();
	g_tts_setting.curTTSAlertHourStatus = g_curTTSAlertHourStatus;
	g_tts_setting.curTTSAlertWeekStatus = g_curTTSAlertWeekStatus;	
	g_tts_setting.curTTSAlertDateStatus = g_curTTSAlertDateStatus;
	mmi_settings_tts_write_to_nvram(&g_tts_setting);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, ST_NOTIFYDURATION, SUCCESS_TONE);
	DeleteNHistory(1);
}


void mmi_tts_voice_alarm_highlight_hdlr(S32 index)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(mmi_tts_setting_Alarm_done_hdlr);
}

void mmi_settings_tts_voice_alarm_Screen(void)
{

	U8		*guiBuffer;
	U8		*inputBuffer;
	U16	inputBufferSize;
	S32	item_offset = 0;

	InlineItem	inline_tts_items[MAX_TTS_VOICE_ALARM_ITME*2];  /* one for caption, one for selection */
	UI_image_ID_type item_icons[] =
	{
	IMG_GLOBAL_L1,
		0,
	IMG_GLOBAL_L2,
	0,
	IMG_GLOBAL_L3,
	0

	};

	
	/* load setting */
	//mmi_java_load_setting();

	EntryNewScreen(SCR_ID_TTS_ALARM_SETTING, mmi_exit_TTS_Voice_Alarm_screen, NULL, NULL);
	
	



	g_tts_alarm_str[0] = (UI_string_type)GetString((U16)(STR_ID_TTS_AlARM_OPEN));
	g_tts_alarm_str[1] = (UI_string_type)GetString((U16)(STR_ID_TTS_AlARM_CLOSE));

	mmi_settings_tts_read_from_nvram(&g_tts_setting);

	g_curTTSAlertHourStatus = g_tts_setting.curTTSAlertHourStatus;
	g_curTTSAlertWeekStatus = g_tts_setting.curTTSAlertWeekStatus;	
	g_curTTSAlertDateStatus= g_tts_setting.curTTSAlertDateStatus ;



	memset((void*)inline_tts_items, 0, sizeof(InlineItem)*MAX_TTS_VOICE_ALARM_ITME*2);

	/* tts alarm hour */
	SetInlineItemCaption(&inline_tts_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_TTS_AlARM_EVERY_HOUR)));

	SetInlineItemActivation(&inline_tts_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&inline_tts_items[item_offset++],
								2,
								(U8**)g_tts_alarm_str,
								&g_curTTSAlertHourStatus);

	/* tts alarm weekday */
	SetInlineItemCaption(&inline_tts_items[item_offset++], 
								(PU8)GetString((U16)(STR_ID_TTS_ALARM_READ_WEEK)));

	SetInlineItemActivation(&inline_tts_items[item_offset],
									INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
									0 );

	SetInlineItemSelect(	&inline_tts_items[item_offset++],
								2,
								(U8**)g_tts_alarm_str,
								&g_curTTSAlertWeekStatus);

        /*tts alarm Date*/
        SetInlineItemCaption(
            &inline_tts_items[item_offset++],
            (PU8) GetString((U16) (STR_ID_TTS_ALARM_READ_DATE)));

        SetInlineItemActivation(
            &inline_tts_items[item_offset],
            INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
            0);

        SetInlineItemSelect(
            &inline_tts_items[item_offset++],
            2,
            (U8 **) g_tts_alarm_str,
            &g_curTTSAlertDateStatus);




	guiBuffer	= GetCurrGuiBuffer(SCR_ID_TTS_ALARM_SETTING);
	inputBuffer = GetCurrNInputBuffer(SCR_ID_TTS_ALARM_SETTING, &inputBufferSize );

	InitializeCategory57Screen();

	if(inputBuffer != NULL)		
		SetCategory57Data(inline_tts_items ,MAX_TTS_VOICE_ALARM_ITME*2, inputBuffer);	

	RegisterHighlightHandler(mmi_tts_voice_alarm_highlight_hdlr);

	/* show category */
	ShowCategory57Screen(STR_ID_SETTING_TTS_ALARM_SETTING, MAIN_MENU_TITLE_SETTINGS_ICON,		
								STR_GLOBAL_OK, IMG_GLOBAL_OK,											
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK,									
								item_offset,
								item_icons, 
								inline_tts_items,
								0, guiBuffer);

	/* set key hdlr */
	SetCategory57LeftSoftkeyFunction(mmi_tts_setting_Alarm_done_hdlr);
	SetCategory57RightSoftkeyFunctions(GoBackHistory, GoBackHistory);
	DisableCategory57ScreenDone();	
}


/*
* end voice alarm setting 
*
*/

/*
* add by liuwen
* begin help setting
*/

void mmi_settings_tts_help_highlight_hdlr( void )
{
	mmi_trace(1, "tts test, the voice alarm hight light hdlr");
	ChangeLeftSoftkey( STR_GLOBAL_OPEN, 0 );
	ChangeRightSoftkey( STR_GLOBAL_BACK, 0 );
	SetLeftSoftkeyFunction( mmi_settings_entry_tts_help, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	//SetKeyHandler( mmi_settings_entry_sound_effect_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN) ;
	//SetKeyHandler( GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN );
}

void mmi_settings_entry_tts_help( void )
{
	PS8 buffer;
	S32 bufferSize;

	EntryNewScreen(SCR_ID_TTS_HELP, NULL, mmi_settings_entry_tts_help, NULL);

	buffer		= GetString(STR_ID_SETTING_TTS_HELP_DESCRIPTION);
	bufferSize	= pfnUnicodeStrlen(buffer);

	ShowCategory74Screen( STR_ID_SETTING_TTS_HELP, 
								 MAIN_MENU_SETTINGS_ICON,
								 0, 0,
								 STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								 (PU8)buffer, bufferSize, NULL);

	/* go back to game menu */
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);


}

/*
* end help setting 
*
*/






void mmi_settings_tts_write_to_nvram( nvram_tts_setting_struct* pTtsInfo )
{	
	S16 error;
	WriteRecord( NVRAM_SETTING_TTS_LID, 1, (void*)pTtsInfo, NVRAM_SETTING_TTS_SIZE, &error );
	
}


void mmi_settings_tts_read_from_nvram( nvram_tts_setting_struct* pTtsInfo)
{
	S16 error;
	ReadRecord( NVRAM_SETTING_TTS_LID, 1, (void*)pTtsInfo, NVRAM_SETTING_TTS_SIZE, &error );

	if(error == NVRAM_READ_SUCCESS)
	{
	}
	else
	{

		pTtsInfo->curTTSMsgReadStatus = 1;
		pTtsInfo->curTTSMenuReadStatus = 1;
		pTtsInfo->curTTSRingIncommingCallStatus = 1;
		pTtsInfo->curTTSVolumeStatus = 2;
		pTtsInfo->curTTSSpeedStatus = 1;
		pTtsInfo->curTTSEffectStatus = 0;
		pTtsInfo->curTTSStyleStatus = 1;
		pTtsInfo->curTTSTuneStatus = 1;
		pTtsInfo->curTTSAlertAlarmAutoStatus = 1;
		pTtsInfo->curTTSAlertDateStatus = 1;
		pTtsInfo->curTTSAlertWeekStatus = 1;
		//memset( pReadMsgInfo->magnitude, MAX_EQUALIZER_NUM*8, 0 );

		mmi_settings_tts_write_to_nvram(pTtsInfo);
	}
}


/*****************************************************************************
 * FUNCTION
 *  ejTTSPlayTime
 * DESCRIPTION
 *  add by ejTTS, play the current time by ejTTS
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ejTTSPlayTime()
{
	TM_SYSTEMTIME time;
	U8 hour;
	U8 min;
	//char format[256];
	//char dateAndWeek[256];
	char readText[256];
	char temp[64];

	
	//add by ejTTS
	//获取当前时间进行播报
	memset(readText, 0, 256);
	TM_GetSystemTime(&time);

	//AnsiiToUnicodeString(dateAndWeek, "今天是");
	if(g_tts_setting.curTTSAlertDateStatus == 0
		|| g_tts_setting.curTTSAlertWeekStatus == 0)
	{
		UCS2Strcpy(readText, GetString(STR_ID_TTS_AlARM_TODAY));
	}
	
	if(g_tts_setting.curTTSAlertDateStatus == 0)
	{
		mmi_trace(1, "tts test, read the date");
		csd_wsprintf(temp, "%d", time.uYear);
		UCS2Strcat(readText, temp);
		//AnsiiToUnicodeString(temp, "年");
		UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_YEAR));
		csd_wsprintf(temp, "%d", time.uMonth);
		UCS2Strcat(readText, temp);
		//AnsiiToUnicodeString(temp, "月");
		UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_MONTH));
		csd_wsprintf(temp, "%d", time.uDay);
		UCS2Strcat(readText, temp);
		//AnsiiToUnicodeString(temp, "日");
		UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_DAY));
		
		//sprintf(format, dateAndWeek, time.uYear, time.uMonth, time.uDay);
	}
	
	if(g_tts_setting.curTTSAlertWeekStatus == 0)
	{
		mmi_trace(1, "tts test, read the week");
		//AnsiiToUnicodeString(temp, "星期");
		UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_WEEK));
		csd_wsprintf(temp, "%d", time.uDayOfWeek);
		UCS2Strcat(readText, temp);
	}

	UCS2Strcat(readText, L",");
	UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_NOW));
	
	if(time.uHour == 12)
	{
		hour = time.uHour;
		min = time.uMinute;
		//AnsiiToUnicodeString(temp, GetString(STR_ID_TTS_AlARM_PM));
		UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_PM));
		//sprintf(readText, format, hour);
	}
	else if(time.uHour == 0)
	{
		hour = time.uHour;
		min = time.uMinute;
		AnsiiToUnicodeString(temp, GetString(STR_ID_TTS_AlARM_DAY));
		UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_AM));
		//sprintf(readText, format, hour);
	}
	else if(time.uHour > 12)
	{
		hour = time.uHour-12;
		min = time.uMinute;
		//AnsiiToUnicodeString(temp, "。现在时刻下午");
		UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_PM));
		//sprintf(readText, format, hour);

	}else
	{
		hour = time.uHour;
		min = time.uMinute;
		//AnsiiToUnicodeString(temp, "。现在时刻上午");
		UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_AM));
		//sprintf(readText, format, hour);

	}

		csd_wsprintf(temp, "%d", hour);
		UCS2Strcat(readText, temp);
		//AnsiiToUnicodeString(temp, "时");
		UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_HOUR));
		csd_wsprintf(temp, "%d", min);
		UCS2Strcat(readText, temp);
		//AnsiiToUnicodeString(temp, "分");
		UCS2Strcat(readText, GetString(STR_ID_TTS_AlARM_MINTUE));

	//add by ejTTS 
	mmi_trace(1, "tts test, start play current time");
	ejTTS_PlayToPCM(readText, UCS2Strlen(readText)*2);
	
}



#endif


