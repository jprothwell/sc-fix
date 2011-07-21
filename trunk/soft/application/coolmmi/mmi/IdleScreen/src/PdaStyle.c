#ifdef APP_SOLARII_PRECOMPILE_SWITCH
#include "..\\..\\mmi\\SolarApp\\SolarAppInc\\SolarAppDef.h"
#endif


#include "mmi_features.h"

#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
#define __NEWSIMULATOR 

#include "GlobalMenuItems.h"
#include "pdastyle.h"
#include "gdi_lcd_config.h"
#include "Gdi_include.h"


#include "stdC.h"
#include "L4Dr1.h"
//#include "PixtelDataTypes.h"
#include "mmi_data_types.h"

//... Add More MMI header
#include "EventsGprot.h"
#include "HistoryGprot.h"
#include "GlobalMenuItems.h"
#include "GlobalConstants.h"
#include "Globaldefs.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "CustMenuRes.h"
#include "CommonScreens.h"
#include "Unicodexdcl.h"
#include "TimerEvents.h"
#include "DebugInitDef.h"
#include "protocolevents.h"
//#include "protocolevents.h"

//...Add MSG header
#include "customer_ps_inc.h"
#include "mmi_msg_context.h"
#include "MessagesL4Def.h"
#include "MessagesResourceData.h"
#include "MessagesMiscell.h"
#include "MessagesExDcl.h"
#include "SmsGuiInterfaceProt.h"
#include "SmsGuiInterfaceType.h"
#include "SmsPsHandler.h"
#include "CellBroadcastType.h"

#include "Gui_ems.h"
//#include "wgui_ems.h"
#include "wgui_ems_categories.h"
#include "GSM7BitDefaultAlphabet.h"
//
#include "smsal_l4c_enum.h"
//
#include "NVRAMEnum.h"
#include "NVRAMProt.h"
#include "NVRAMType.h"
//
#include "CSP.h"
#include "FrameworkStruct.h"
#include "MainMenuDef.h"
#include "CallHistoryGProts.h"
#include "CallManagementGprot.h"
#include "CallhistoryEnum.h"
#include "MiscFunctions.h"
#include "SettingProfile.h"
#include "ProfileGprots.h"
#include "PhoneBookGprot.h"
#include "IdleAppProt.h"
#include "AlarmFrameworkProt.h"

#include "wgui_status_icons.h"
#include "gpioInc.h"
#include "KeyBrd.h"
#include "SubLCDHistoryGprot.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"


#include "SMSApi.h"
#include "SMSStruct.h"
#include "SMSFunc.h"


#include "gui_data_types.h"
#include "kal_release.h"
#include "Conversions.h"
#include "FrameworkStruct.h"
#include "GlobalConstants.h"
#include "FileMgr.h"
#include "Audioplayerdef.h"
//#include "audioplayerdef.h"
#include "Audioplayertype.h"
//#include "audioplayertype.h"
#include "CallsStruct.h"
#include "TouchScreenGprot.h"
//#include "custom_mmi_default_value.h"
//#include "app_datetime.h"
#include "App_datetime.h"
//#include "app_mem.h" 

//#include "ToDoListResDef.h"
#include "Todolistenum.h"

//#include "mtpnp_ad_master_common_def.h"
#include "dual_sim.h"
//#include "SimDetectionResDef.h"
#include "simdetectiondef.h"
//#include "SettingResDef.h"
#include "settingdefs.h"
//#include "tcpip_mem.h"

#undef __NEWSIMULATOR 


S32 hilit_shortcut_id   = 0;

S8 g_MissedCall[MAX_SUB_MENUS];
S8 g_MissedMsg[MAX_SUB_MENUS];
S8 g_TodoList[MAX_SUB_MENUS];
S8 g_ActivedProfile[MAX_SUB_MENUS*2];
#define MMI_MAIN_BIT_PER_PIXEL	(16)
#define AL8(N) ((N+7)&~0x7)

#if defined (__MMI_WIN_STYLE_SUPPORT__)
#define OPTIMIZE_PDA_BUF_SIZE ((AL8(MAIN_LCD_DEVICE_WIDTH)*AL8(64)*MMI_MAIN_BIT_PER_PIXEL)>>3)
#else
#define OPTIMIZE_PDA_BUF_SIZE ((AL8(MAIN_LCD_DEVICE_WIDTH)*AL8(MMI_MENUITEM_HEIGHT)*MMI_MAIN_BIT_PER_PIXEL)>>3)
#endif

U8 opt_pda_mm_buf[OPTIMIZE_PDA_BUF_SIZE];

symbian_idle_pen_state_enum symbian_idle_object = SYMBIAN_IDLE_PEN_NONE;

extern msg_msgbox_info_struct msgbox_info;

extern UI_string_type MMI_highlighted_item_text;

extern UI_image_type MMI_mm_animation;

extern BOOL r2lMMIFlag;

extern U16 on_idle_screen;

extern gdi_handle wgui_layer_1;

extern gdi_handle wgui_base_layer;

extern scrolling_text* extra_scrolling_text ;

extern S32 GUI_current_fixed_icontext_menuitem_x;
extern S32 GUI_current_fixed_icontext_menuitem_y;
extern S32 UI_text_menuitem_scroll_gap;
extern byte GUI_current_fixed_icontext_menuitem_scroll;

extern void cat33_hide_time_display( S32 x1, S32 y1, S32 x2, S32 y2 );

extern void idle_screen_hide_status_icons_bar0( void );

extern void idle_screen_hide_status_icons_bar1( void );

extern void draw_wallpaper( void );

extern void cat33_animation_processor( void );

extern void idle_screen_show_network_details( void );

extern void mmi_phb_entry_main_menu(void);
extern void EntryScrProfiles(void);
extern void goto_home_page(void);
extern U8 PhnsetGetTimeFormat(void);
//extern E_MTPNP_AD_STATUS MTPNP_AD_Get_Status(void);
#ifdef __MMI_TODOLIST__
extern void Symbian_TodoListGetDetail( U16* yesterday, U16* today, U16* tomorrow );
extern void TDLShowAllList( void );
#endif //__MMI_TODOLIST__

extern void mmi_msg_entry_inbox_list( void );
extern void mmi_msg_entry_write_msg( void );

extern void mmi_fng_entry_screen( void );
extern U8 g_curr_thm_index;
#ifdef __MMI_FILE_MANAGER__
extern void fmgr_launch( void );
#endif

#ifdef __MMI_IMAGE_VIEWER__
extern void mmi_imgview_entry_app( void );
#endif

#ifdef __MMI_CALENDAR__
extern void ClndrPreEntryApp( void );
#endif 
#if defined(__MMI_AUDIO_PLAYER__)
extern void mmi_audply_entry_main( void );
#endif
#ifdef __MMI_CAMERA__

#ifdef __PRJ_WITH_SPICAM__
extern void mmi_camera_entry_option_menu_screen( void );
#else
extern void mmi_camera_entry_app_screen( void );
#endif

#endif
#if (defined(__FEATURE_DYNMENU__))
extern void dynmenu_enter_main(void);
#endif
#ifdef __tl_uumap__
extern void EntryUumap (void);
#endif
#ifdef __PALM_LEARN_MACHINE__
extern void dict_start_menu_win(void);
#endif
extern void GoToServicesMenu( void );
extern void EntryScrSetKeyPadVolumeLevel( void );
extern void EntrySECSETMain(void);
extern 	void cat33_hide_lsk(void);
extern 	void cat33_hide_rsk(void);
extern 	void cat33_hide_csk(void);
extern 	U8 g_slave_chist_missed_call_num ;
//extern idle_keylock_struct g_keylock_context;
extern U16   gKeyPadLockFlag;


extern U8 gactivatedprofile;

extern U8 g_chist_have_missed_call;
U16 gSymbian_icon_ani[SYMBIAN_TOTAL_ITEMS];


#define PDA_LINE_START_COLOR    gdi_act_color_from_rgb(255,87,87,87)
#define PDA_LINE_END_COLOR      gdi_act_color_from_rgb(255,215,215,215)
#define PDA_MATRIX_BACK_COLOR      gdi_act_color_from_rgb(255,0,0,0)
#define PDA_MATRIX_LINE_COLOR      gdi_act_color_from_rgb(255,255,0,0)

g_Symbian_idle_item pda_idle_shortcut[SYMBIAN_TOTAL_ITEMS]      =
{
#if defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
	{IDLE_SYMBIAN_MATRIX1,	  NULL, 	IMG_IDLE_SYMBIAN_MATRIX1,	  IMG_IDLE_SYMBIAN_MATRIX1_ANI,   EntrySymbianMatrix1}
  , {IDLE_SYMBIAN_MATRIX2,		  NULL, 	IMG_IDLE_SYMBIAN_MATRIX2,	  IMG_IDLE_SYMBIAN_MATRIX2_ANI,   EntrySymbianMatrix2}
  , {IDLE_SYMBIAN_MATRIX3,		  NULL, 	IMG_IDLE_SYMBIAN_MATRIX3,	  IMG_IDLE_SYMBIAN_MATRIX3_ANI,   EntrySymbianMatrix3}
  , {IDLE_SYMBIAN_MATRIX4,		  NULL, 	IMG_IDLE_SYMBIAN_MATRIX4,	  IMG_IDLE_SYMBIAN_MATRIX4_ANI,   EntrySymbianMatrix4}
  , {IDLE_SYMBIAN_MATRIX5,		  NULL, 	IMG_IDLE_SYMBIAN_MATRIX5,	  IMG_IDLE_SYMBIAN_MATRIX5_ANI,   EntrySymbianMatrix5}
  , {IDLE_SYMBIAN_MATRIX6,		  NULL, 	IMG_IDLE_SYMBIAN_MATRIX6,	  IMG_IDLE_SYMBIAN_MATRIX6_ANI,   EntrySymbianMatrix6}
  ,{IDLE_SYMBIAN_SETTING1,	  NULL, 	IMG_IDLE_SYMBIAN_SETTING1,	 IMG_IDLE_SYMBIAN_SETTING1_ANI,	EntrySymbianSetting1}
#elif defined (__MMI_WIN_STYLE_SUPPORT__)
  {IDLE_SYMBIAN_MATRIX1,        NULL,     IMG_IDLE_SYMBIAN_MATRIX1,	  IMG_IDLE_SYMBIAN_MATRIX1_ANI,   EntrySymbianMatrix1}
, {IDLE_SYMBIAN_MATRIX2,        NULL,     IMG_IDLE_SYMBIAN_MATRIX2,	  IMG_IDLE_SYMBIAN_MATRIX2_ANI,   EntrySymbianMatrix2}
, {IDLE_SYMBIAN_MATRIX3,        NULL,     IMG_IDLE_SYMBIAN_MATRIX3,	  IMG_IDLE_SYMBIAN_MATRIX3_ANI,   EntrySymbianMatrix3}
, {IDLE_SYMBIAN_MATRIX4,        NULL,     IMG_IDLE_SYMBIAN_MATRIX4,	  IMG_IDLE_SYMBIAN_MATRIX4_ANI,   EntrySymbianMatrix4}
, {IDLE_SYMBIAN_MATRIX5,        NULL,     IMG_IDLE_SYMBIAN_MATRIX5,	  IMG_IDLE_SYMBIAN_MATRIX5_ANI,   EntrySymbianMatrix5}
, {IDLE_SYMBIAN_MATRIX6,        NULL,     IMG_IDLE_SYMBIAN_MATRIX6,	  IMG_IDLE_SYMBIAN_MATRIX6_ANI,   EntrySymbianMatrix6}
, {IDLE_SYMBIAN_SETTING1,       NULL,     NULL,	 NULL,  EntrySymbianSetting2}
, {IDLE_SYMBIAN_SETTING2,       NULL,     NULL,    NULL,  EntrySymbianSetting3}
#else
  {IDLE_SYMBIAN_MATRIX1,        NULL,     IMG_IDLE_SYMBIAN_MATRIX1,     NULL,   EntrySymbianMatrix1}
, {IDLE_SYMBIAN_MATRIX2,        NULL,     IMG_IDLE_SYMBIAN_MATRIX2,     NULL,   EntrySymbianMatrix2}
, {IDLE_SYMBIAN_MATRIX3,        NULL,     IMG_IDLE_SYMBIAN_MATRIX3,     NULL,   EntrySymbianMatrix3}
, {IDLE_SYMBIAN_MATRIX4,        NULL,     IMG_IDLE_SYMBIAN_MATRIX4,     NULL,   EntrySymbianMatrix4}
, {IDLE_SYMBIAN_MATRIX5,        NULL,     IMG_IDLE_SYMBIAN_MATRIX5,     NULL,   EntrySymbianMatrix5}
, {IDLE_SYMBIAN_MATRIX6,        NULL,     IMG_IDLE_SYMBIAN_MATRIX6,     NULL,   EntrySymbianMatrix6}
, {IDLE_SYMBIAN_SETTING2,       NULL,     IMG_IDLE_SYMBIAN_SETTING2,    NULL,   EntrySymbianSetting2}
, {IDLE_SYMBIAN_SETTING3,       NULL,     IMG_IDLE_SYMBIAN_SETTING3,    NULL,   EntrySymbianSetting3}
, {IDLE_SYMBIAN_SETTING4,       NULL,     IMG_IDLE_SYMBIAN_SETTING4,    NULL,   EntrySymbianSetting4}
//, {IDLE_SYMBIAN_SETTING4,       NULL,     IMG_IDLE_SYMBIAN_SETTING4,    IMG_IDLE_SYMBIAN_SETTING4_ANI,  EntrySymbianSetting4}
//, {IDLE_SYMBIAN_SETTING5,       NULL,     IMG_IDLE_SYMBIAN_SETTING5,    IMG_IDLE_SYMBIAN_SETTING5_ANI,  EntrySymbianSetting5}
#endif
};

Symbian_idle_info g_Symbian_idle_info[SYMBIAN_TOTAL_LINES]=
{
#if defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
{LINE1_X  , 	LINE1_Y  ,	 SYMBIAN_MATRIX_WIDTH  ,	  SYMBIAN_MATRIX_HEIGHT  ,		LINE1_ITEM_NUM	,  0  , 				LINE1_TOTAL  ,		  FALSE}, //x,y,with,height,columns per row, ,highlight icon flag
{LINE2_X  , 	LINE2_Y  ,	 LINE2_ITEM_WIDTH  ,		  LINE2_ITEM_HEIGHT  ,			LINE2_ITEM_NUM	,  LINE1_TOTAL	,		LINE2_TOTAL  ,		  FALSE}, 
#elif defined (__MMI_WIN_STYLE_SUPPORT__)
  {LINE1_X  ,     LINE1_Y  ,   SYMBIAN_MATRIX_WIDTH  ,      SYMBIAN_MATRIX_HEIGHT  ,      LINE1_ITEM_NUM  ,  0  ,                 LINE1_TOTAL  ,        UI_MENUITEM_DISABLE_TEXT_DISPLAY}, //x,y,with,height,columns per row, ,highlight icon flag

  {LINE2_X  ,     LINE2_Y  ,   LINE2_ITEM_WIDTH  ,          LINE2_ITEM_HEIGHT  ,          LINE2_ITEM_NUM  ,  LINE1_TOTAL  ,       LINE2_TOTAL  ,        FALSE}, 
  {LINE3_X  ,     LINE3_Y  ,   LINE3_ITEM_WIDTH  ,          LINE3_ITEM_HEIGHT  ,          LINE3_ITEM_NUM  ,  LINE2_TOTAL  ,       LINE3_TOTAL  ,        FALSE}, 
//  {LINE4_X  ,     LINE4_Y  ,   LINE4_ITEM_WIDTH  ,          LINE4_ITEM_HEIGHT  ,          LINE4_ITEM_NUM  ,  LINE3_TOTAL  ,       LINE4_TOTAL  ,        FALSE}, 
#else
  {LINE1_X  ,     LINE1_Y  ,   SYMBIAN_MATRIX_WIDTH  ,      SYMBIAN_MATRIX_HEIGHT  ,      LINE1_ITEM_NUM  ,  0  ,                 LINE1_TOTAL  ,        UI_MENUITEM_DISABLE_TEXT_DISPLAY}, //x,y,with,height,columns per row, ,highlight icon flag
  {LINE2_X  ,     LINE2_Y  ,   LINE2_ITEM_WIDTH  ,          LINE2_ITEM_HEIGHT  ,          LINE2_ITEM_NUM  ,  LINE1_TOTAL  ,       LINE2_TOTAL  ,        FALSE}, 
  {LINE3_X  ,     LINE3_Y  ,   LINE3_ITEM_WIDTH  ,          LINE3_ITEM_HEIGHT  ,          LINE3_ITEM_NUM  ,  LINE2_TOTAL  ,       LINE3_TOTAL  ,        FALSE}, 
  {LINE4_X  ,     LINE4_Y  ,   LINE4_ITEM_WIDTH  ,          LINE4_ITEM_HEIGHT  ,          LINE4_ITEM_NUM  ,  LINE3_TOTAL  ,       LINE4_TOTAL  ,        FALSE}, 
  //{LINE5_X  ,     LINE5_Y  ,   LINE5_ITEM_WIDTH  ,          LINE5_ITEM_HEIGHT  ,          LINE5_ITEM_NUM  ,  LINE4_TOTAL  ,       LINE5_TOTAL  ,        FALSE}
 #endif
};

S32 g_symbian_idle_hilight_item = 0;

void resetSymbianIdleHilightItem(void)
{
	g_symbian_idle_hilight_item = 0;
}

S32 getSymbianIdleHilightItem(void)
{
	return g_symbian_idle_hilight_item;
}

void setSymbianIdleHilightItem(S32 index)
{
	if(index>=SYMBIAN_TOTAL_ITEMS || g_symbian_idle_hilight_item <=-1)
	{
		g_symbian_idle_hilight_item = 0;
	}
	else
	{
		g_symbian_idle_hilight_item = index;
	}
	
}
#ifdef WAP_SUPPORT 
#include "wap_ps_struct.h"
#include "widget.h"
extern char* bra_prof_get_key_str_value (int id, char *key);
void ConnectToWap(U8* url)
{
    ilm_struct *ilm_ptr = NULL;
    wap_browser_startup_req_struct *param_ptr;

    param_ptr = (wap_browser_startup_req_struct*)construct_local_para(sizeof(wap_browser_startup_req_struct), TD_CTRL);
    param_ptr->type = 2;
    strcpy ((char*)param_ptr->url, (char*)url);


	if (!MSF_wapIsReady())
	{
		WGUI_CTX->not_ready_for_MMS = 0;
		wap_entry_widget_not_ready();
		return;
	} 

	/* Disable key to prevent sending duplicated requests */
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);

    ilm_ptr = allocate_ilm(MOD_MMI);
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;
    ilm_ptr->msg_id         = MSG_ID_WAP_BROWSER_STARTUP_REQ;
    ilm_ptr->peer_buff_ptr  = NULL;

    SEND_ILM(MOD_MMI, MOD_WAP, 0, ilm_ptr);
	

}

void goto_home_page(void)
{
	char* homepage;
	
#ifdef __FT_MMI_OTH_ENTRY_MAINMENU_WHEN_NO_SIM__
#if defined(__MMI_DUAL_SIM_MASTER__)
		if(mmi_is_sim_error_exist())
#else
		if (mmi_bootup_is_sim_removed())
#endif
		{
			DisplayPopup((PU8) GetString(STR_SIM_INSERTION_MSG), IMG_GLOBAL_ERROR, 0, 1000, (U8) ERROR_TONE);
			return;
		}
		else if(mmi_bootup_get_active_flight_mode() != 0)
		{
			DisplayPopup((PU8) GetString(STR_MENU_FLIGHT_MODE), IMG_GLOBAL_ERROR, 0, 1000, (U8) ERROR_TONE);
			return; 	
		}	
#endif
	
	homepage = bra_prof_get_key_str_value(1, "homepage_url");
	//AnsiiToUnicodeString((S8*)aUnicodeString, (S8*)homepage);
	ConnectToWap((U8*)homepage); 
}

void goto_facebook_page(void)
{
	char  homepage[] = "http://www.facebook.com";
	
#ifdef __FT_MMI_OTH_ENTRY_MAINMENU_WHEN_NO_SIM__
#if defined(__MMI_DUAL_SIM_MASTER__)
		if(mmi_is_sim_error_exist())
#else
		if (mmi_bootup_is_sim_removed())
#endif
		{
			DisplayPopup((PU8) GetString(STR_SIM_INSERTION_MSG), IMG_GLOBAL_ERROR, 0, 1000, (U8) ERROR_TONE);
			return;
		}
		else if(mmi_bootup_get_active_flight_mode() != 0)
		{
			DisplayPopup((PU8) GetString(STR_MENU_FLIGHT_MODE), IMG_GLOBAL_ERROR, 0, 1000, (U8) ERROR_TONE);
			return; 	
		}	
#endif
	
	//homepage = bra_prof_get_key_str_value(1, "homepage_url");
	//AnsiiToUnicodeString((S8*)aUnicodeString, (S8*)homepage);
	ConnectToWap((U8*)homepage); 
}

void goto_taobao_page()
{
	char  homepage[] = "http://wap.taobao.com";
	
#ifdef __FT_MMI_OTH_ENTRY_MAINMENU_WHEN_NO_SIM__
#if defined(__MMI_DUAL_SIM_MASTER__)
		if(mmi_is_sim_error_exist())
#else
		if (mmi_bootup_is_sim_removed())
#endif
		{
			DisplayPopup((PU8) GetString(STR_SIM_INSERTION_MSG), IMG_GLOBAL_ERROR, 0, 1000, (U8) ERROR_TONE);
			return;
		}
		else if(mmi_bootup_get_active_flight_mode() != 0)
		{
			DisplayPopup((PU8) GetString(STR_MENU_FLIGHT_MODE), IMG_GLOBAL_ERROR, 0, 1000, (U8) ERROR_TONE);
			return; 	
		}	
#endif
	
	//homepage = bra_prof_get_key_str_value(1, "homepage_url");
	//AnsiiToUnicodeString((S8*)aUnicodeString, (S8*)homepage);
	ConnectToWap((U8*)homepage); 
}


void goto_yahoo_page(void)
{
	char  homepage[] = "http://www.yahoo.com";
	
#ifdef __FT_MMI_OTH_ENTRY_MAINMENU_WHEN_NO_SIM__
#if defined(__MMI_DUAL_SIM_MASTER__)
		if(mmi_is_sim_error_exist())
#else
		if (mmi_bootup_is_sim_removed())
#endif
		{
			DisplayPopup((PU8) GetString(STR_SIM_INSERTION_MSG), IMG_GLOBAL_ERROR, 0, 1000, (U8) ERROR_TONE);
			return;
		}
		else if(mmi_bootup_get_active_flight_mode() != 0)
		{
			DisplayPopup((PU8) GetString(STR_MENU_FLIGHT_MODE), IMG_GLOBAL_ERROR, 0, 1000, (U8) ERROR_TONE);
			return; 	
		}	
#endif
	
	//homepage = bra_prof_get_key_str_value(1, "homepage_url");
	//AnsiiToUnicodeString((S8*)aUnicodeString, (S8*)homepage);
	ConnectToWap((U8*)homepage); 
}

#endif

#ifdef __MEUU_SM_PACK__
extern void HightlightMeuuMain(void);
void initSpApp(void)
{
	SetHiliteHandler(MENU_MEUU_SP_MAIN ,HightlightMeuuMain);
//	SetHiliteHandler(MENU_ID_SP_CUSTOMER,HilightSpCustomer);
}
#endif
//extern void MTPNP_PFAL_chist_slave_req_read_missed_num_by_key(void);
void hilightPdaSim1MissedCall(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK, 0);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(ReqReadMissedNumByKey, KEY_EVENT_UP);
	SetKeyHandler(ReqReadMissedNumByKey, KEY_RIGHT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


void hilightPdaSim2MissedCall(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK, 0);

	/* Register function for left soft key */
//	SetLeftSoftkeyFunction(MTPNP_PFAL_chist_slave_req_read_missed_num_by_key, KEY_EVENT_UP);
	//SetKeyHandler(MTPNP_PFAL_chist_slave_req_read_missed_num_by_key, KEY_RIGHT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


void initPdaMissedCall(void)
{
	SetHiliteHandler(MENU_ITEM_MISSED_CALL_LIST_SIM1,hilightPdaSim1MissedCall);
	SetHiliteHandler(MENU_ITEM_MISSED_CALL_LIST_SIM2,hilightPdaSim2MissedCall);
}

void EntryCHISTMissedScr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer = NULL;
    U16 numItems = 0;
    U16 nStrItemList[MAX_SUB_MENUS];
    U16 itemIcons[MAX_SUB_MENUS];
    U8 *PopUpList[MAX_SUB_MENUS];

    E_MTPNP_AD_SIMCARD_STATUS sim1_status, sim2_status;


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	
#ifdef __FT_MMI_OTH_ENTRY_MAINMENU_WHEN_NO_SIM__
#if defined(__MMI_DUAL_SIM_MASTER__)
		if(mmi_is_sim_error_exist())
#else
		if (mmi_bootup_is_sim_removed())
#endif
		{
			DisplayPopup((PU8) GetString(STR_SIM_INSERTION_MSG), IMG_GLOBAL_ERROR, 0, 1000, (U8) ERROR_TONE);
			return;
		}
		else if(mmi_bootup_get_active_flight_mode() != 0)
		{
			DisplayPopup((PU8) GetString(STR_MENU_FLIGHT_MODE), IMG_GLOBAL_ERROR, 0, 1000, (U8) ERROR_TONE);
			return; 	
		}	
#endif

    sim1_status = MTPNP_AD_Get_CardStatus(0);
    sim2_status = MTPNP_AD_Get_CardStatus(1);
	//kal_prompt_trace(MOD_MMI, "leo,sim1_status=%d, sim2_status=%d ",sim1_status,sim2_status);
	if((sim1_status >=MTPNP_AD_SIMCARD_VALID) && (sim2_status >=MTPNP_AD_SIMCARD_VALID))
	{
	    EntryNewScreen(SCR_CALLLOG_MISSED_PDA, NULL, EntryCHISTMissedScr, NULL);
	   // PRINT_INFORMATION("\n[EntryCHISTTabPaneMissedPDA] \n");
	    
	    guiBuffer = GetCurrGuiBuffer(SCR_CALLLOG_MISSED_PDA);

	    numItems = GetNumOfChild_Ext(MENU_ITEM_MISSED_CALL_LIST_PDA);
	    GetSequenceStringIds_Ext(MENU_ITEM_MISSED_CALL_LIST_PDA, nStrItemList);
	    GetSequenceImageIds_Ext(MENU_ITEM_MISSED_CALL_LIST_PDA, itemIcons);
	    SetParentHandler(MENU_ITEM_MISSED_CALL_LIST_PDA);

	    /* construct hint popup strings for menu items */
	    ConstructHintsList(MENU_ITEM_MISSED_CALL_LIST_PDA, PopUpList);

	    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	    ShowCategory52Screen(
	        STR_MISSED_CALL,
	        0,
	        STR_GLOBAL_OK,
	        IMG_GLOBAL_OK,
	        STR_GLOBAL_BACK,
	        IMG_GLOBAL_BACK,
	        numItems,
	        nStrItemList,
	        (U16*) itemIcons,
	        (U8 **)PopUpList,
	        0,
	        0,
	        guiBuffer);

	    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    }
    else if((sim1_status <MTPNP_AD_SIMCARD_VALID) && (sim2_status >=MTPNP_AD_SIMCARD_VALID))
    {
	//	MTPNP_PFAL_chist_slave_req_read_missed_num_by_key();
    }
    else 
    {
		ReqReadMissedNumByKey();
    }
}

U32 AtiveScreenIsIdle(void)
{
	return (GetActiveScreenId()==IDLE_SCREEN_ID);
}
void EntrySymbianMatrix1( void )
{
#ifdef __FT_MMI_E400_UI_STYLE__
	mmi_msg_pre_entry_inbox_list();
	mmi_msg_entry_inbox_list();
#else 
	//mmi_msg_pre_entry_inbox_list();
    //mmi_msg_entry_inbox_list();
	//EntryScrMessagesMenuList();
	///mmi_msg_um_entry_write();
	EntryScrMessagesMenuList();
#endif 
}

void EntrySymbianMatrix2( void )
{
#ifdef __FT_MMI_E400_UI_STYLE__
	//goto_home_page();
	GoToServicesMenu();
#else 
	mmi_phb_entry_main_menu();
	//mmi_phb_quick_search_list_pre_entry();
	//goto_home_page();
	//GoToServicesMenu();
#endif
}
#ifdef _MAOL_APP_
extern void mtkMaolAppMSN(void);
#endif

//WW
#if 0//def __J2ME__
extern void mmi_java_luach_app_by_index(S32  nIndex);
#endif

#ifdef __POPO_MSN__
#ifndef WIN32 
extern void popo_msn_entryapp(void);
#endif 
#endif

void EntrySymbianMatrix3( void )
{
#ifdef __FT_MMI_E400_UI_STYLE__
	 mmi_audply_entry_main();
#else 
#ifdef __POPO_MSN__ //caijianbing 20091112
#ifndef WIN32 
popo_msn_entryapp();
#endif
#else	
#ifdef _MAOL_APP_
 mtkMaolAppMSN();
#else
//ClndrPreEntryApp();
TDLShowAllList(); //caijianbing modify 20091202
#endif
#endif
#endif 
}

void EntrySymbianMatrix4( void )
{
#ifdef __FT_MMI_E400_UI_STYLE__
#ifdef __PRJ_WITH_SPICAM__
    mmi_camera_entry_option_menu_screen();
#else
	mmi_camera_entry_app_screen();
#endif
#else 

#ifdef WAP_SUPPORT 
	goto_home_page();
#else
    //mmi_camera_entry_app_screen();
    GoToServicesMenu();
#endif
#endif 
}

void EntrySymbianMatrix6( void )
{
#ifdef __FT_MMI_E400_UI_STYLE__
	mmi_fng_entry_screen();
#else
	EntryAlmMenu();
//	CalcPreEntryApp();
#endif

}
void EntrySymbianMatrix5( void )
{
#ifdef __FT_MMI_E400_UI_STYLE__
	EntryAlmMenu();
#else 
	 mmi_audply_entry_main();
#endif 
}

#if defined (__MMI_WIN_STYLE_SUPPORT__)
void EntrySymbianSetting1( void )
{
    //ReqReadMissedNumByKey();
	//EntryCHISTMissedScr();
}

void EntrySymbianSetting2( void )
{
    TDLShowAllList();
}

void EntrySymbianSetting3( void )
{
	EntryScrProfiles();
}
#else
void EntrySymbianSetting1( void )
{
    //ReqReadMissedNumByKey();
	//EntryCHISTMissedScr();
	EntryScrProfiles();
}

void EntrySymbianSetting2( void )
{
#if 0//caijianbing solve the problem while only insert sim2 and have missed call;    20091222
    ReqReadMissedNumByKey();
#endif 
	EntryCHISTMissedScr();
}

void EntrySymbianSetting3( void )
{
	mmi_msg_pre_entry_inbox_list();
    mmi_msg_entry_inbox_list();
}
#endif

void EntrySymbianSetting4( void )
{
    TDLShowAllList();
}
/*
void EntrySymbianSetting5( void )
{
    TDLShowAllList();
}
*/

#ifdef __MMI_BLACK_BERRY_IDLE_STYLE__
U8 SymbianShowMenuFlag = 1;
U8 IsIdleShowMenuString(void)
{
	return SymbianShowMenuFlag;
}
void SetIdleShowMenuFlag(U8 flag)
{
	SymbianShowMenuFlag = flag;
}
void Symbian_idle_hide_menu_String(void)
{
	S32 x1, y1, x2, y2;
	x1 = SYMBIAN_IDLE_MENU_NAME_X;
	y1 = SYMBIAN_IDLE_MENU_NAME_Y;
	x2 = x1 +SYMBIAN_IDLE_MENU_NAME_W;
	y2 = y1 +SYMBIAN_IDLE_MENU_NAME_H;

	gui_push_clip();
	gui_set_clip( x1,y1,x2,y2);
	gdi_draw_solid_rect(x1,y1,x2,y2,GDI_COLOR_TRANSPARENT);
	gui_pop_clip();
}

void Symbian_idle_hide_menu_Strig_handle(void)
{
	//StopTimer(SYMBIAN_IDLE_HIDE_TIMER);
	SetIdleShowMenuFlag(0);
	Symbian_idle_hide_menu_String();
}
#endif
U16 Symbian_unread_msg_num( void )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i           = 0;
    U16 nUnRead     = 0;
    U16 totalinbox  = mmi_frm_sms_get_sms_list_size( MMI_FRM_SMS_APP_INBOX );

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/


    if ( totalinbox != MMI_FRM_SMS_INVALID_INDEX )
    {
        while ( i < totalinbox )
        {
            if ( ( mmi_frm_sms_get_status( MMI_FRM_SMS_APP_INBOX, i ) & 0x0f ) == MMI_FRM_SMS_APP_UNREAD )
            {
                nUnRead++;
            }
            i++;
        }
    }
    return nUnRead;
}


void Gui_Symbian_custom_highlight_handler( S32 index )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if ( gKeyPadLockFlag == 0 )
    {
        SetKeyHandler( pda_idle_shortcut[index].entryfrn, KEY_IP, KEY_EVENT_UP );
        SetKeyHandler( MMI_dummy_function, KEY_IP, KEY_EVENT_DOWN );
		#if defined(__MMI_WGUI_CSK_ENABLE__)
		SetCenterSoftkeyFunction(pda_idle_shortcut[index].entryfrn, KEY_EVENT_UP);
		#endif
    }
    else
    {
        SetKeyHandler( MMI_dummy_function, KEY_IP, KEY_EVENT_DOWN );
        SetKeyHandler( MMI_dummy_function, KEY_IP, KEY_EVENT_UP );
    }
}

void initial_Symbian_Info( g_Symbian_idle_item* pda_idle_shortcut )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	
    S8 LocalData1[10];
    S8 LocalData1_unicode[10];
    U16 unread_inbox;
    U16 yesterday_todo  = 0;
    U16 today_todo      = 0;
    U16 tomorrow_todo   = 0;

    U16 msg_id          = 0;
    U16 i               = 0;

	U16 missedCallNum =0;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

#if defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
	
   if ( gactivatedprofile == 0xff )
	{
		gactivatedprofile = 0;
	}

	switch ( gactivatedprofile )
	{
		case MMI_PROFILE_GENERAL:
			msg_id = STR_PROFILES_GENERAL;
			break;
		case MMI_PROFILE_MEETING:
			msg_id = STR_PROFILES_MEETING;
			break;
		case MMI_PROFILE_OUTDOOR:
			msg_id = STR_PROFILES_OUTDOOR;
			break;
		case MMI_PROFILE_INDOOR:
			msg_id = STR_PROFILES_INDOOR;
			break;
		case MMI_PROFILE_HEADSET:
			msg_id = STR_PROFILES_HEADSET;
			break;
		case MMI_PROFILE_SILENT:
			msg_id = STR_PROFILES_SILENT;
			break;
	#ifdef __BT_SUPPORT__       
		case MMI_PROFILE_BT:
			msg_id = STR_PROFILES_BT;
			break;
	#endif
		default:
	#ifdef MMI_ON_WIN32
			msg_id = STR_PROFILES_GENERAL;
	#else
			MMI_ASSERT( 0 );
	#endif 
	}
	
	memset( g_ActivedProfile, 0, sizeof( g_ActivedProfile ) );
	for ( i = 0; i < SYMBIAN_TOTAL_ITEMS; i++ )
    {
        memset( LocalData1, 0, sizeof( LocalData1 ) );
        memset( LocalData1_unicode, 0, sizeof( LocalData1_unicode ) );

        if ( pda_idle_shortcut[i].pdaMatrixMenu_ID == IDLE_SYMBIAN_SETTING1 )
        {
        	pda_idle_shortcut[i].pdaMatrixString = g_ActivedProfile;
            pfnUnicodeStrcpy( g_ActivedProfile, GetString( STR_IDLE_BLACK_BERRY_STRING8 ) );
            pfnUnicodeStrcat( ( PS8 ) g_ActivedProfile, ( S8 * ) L"(" );
            pfnUnicodeStrcat( ( PS8 ) g_ActivedProfile, GetString( msg_id ) );
			pfnUnicodeStrcat( ( PS8 ) g_ActivedProfile, ( S8 * ) L")" );

			pda_idle_shortcut[i].pda_Matrix_Icon_ID= IMG_IDLE_SYMBIAN_SETTING1+gactivatedprofile;
			pda_idle_shortcut[i].pda_Matrix_Icon_ani_ID= IMG_IDLE_SYMBIAN_SETTING1_ANI+gactivatedprofile;
        }
        else
        {        
			#ifdef __FT_CTA_VERSION__	
				if ( pda_idle_shortcut[i].pdaMatrixMenu_ID == IDLE_SYMBIAN_MATRIX3 )
				{				
	        		pda_idle_shortcut[i].pdaMatrixString = GetString( ( U16 ) ( STR_IDLE_SYMBIAN_STRING10 ));
				}
				else 
			#endif 
            	pda_idle_shortcut[i].pdaMatrixString = GetString( ( U16 ) ( STR_IDLE_BLACK_BERRY_STRING1 + i ) );
        }
    }
#elif defined (__MMI_WIN_STYLE_SUPPORT__)
    memset( g_MissedCall, 0, sizeof( g_MissedCall ) );
    memset( g_MissedMsg, 0, sizeof( g_MissedMsg ) );
    memset( g_TodoList, 0, sizeof( g_TodoList ) );
	initPdaMissedCall();
    for ( i = 0; i < SYMBIAN_TOTAL_ITEMS; i++ )
    {
        memset( LocalData1, 0, sizeof( LocalData1 ) );
        memset( LocalData1_unicode, 0, sizeof( LocalData1_unicode ) );

#ifdef __MMI_TODOLIST__     
        if ( pda_idle_shortcut[i].pdaMatrixMenu_ID == IDLE_SYMBIAN_SETTING1 )
        {
            Symbian_TodoListGetDetail( &yesterday_todo, &today_todo, &tomorrow_todo );
            pda_idle_shortcut[i].pdaMatrixString = g_TodoList;
            pfnUnicodeStrcpy( g_TodoList, GetString( STR_IDLE_SYMBIAN_STRING10 ) );
            pfnUnicodeStrcat( ( PS8 ) g_TodoList, ( S8 * ) L":  " );

            sprintf( LocalData1, "%d", today_todo ); 

            AnsiiToUnicodeString( LocalData1_unicode, LocalData1 );
            pfnUnicodeStrcat( ( PS8 ) g_TodoList, LocalData1_unicode );
        }
#endif //__MMI_TODOLIST__       
        else if ( pda_idle_shortcut[i].pdaMatrixMenu_ID == IDLE_SYMBIAN_SETTING2 )
        {
		if ( gactivatedprofile == 0xff )
		{
			gactivatedprofile = 0;
		}

		switch ( gactivatedprofile )
		{
			case MMI_PROFILE_GENERAL:
				msg_id = STR_PROFILES_GENERAL;
				break;
			case MMI_PROFILE_MEETING:
				msg_id = STR_PROFILES_MEETING;
				break;
			case MMI_PROFILE_OUTDOOR:
				msg_id = STR_PROFILES_OUTDOOR;
				break;
			case MMI_PROFILE_INDOOR:
				msg_id = STR_PROFILES_INDOOR;
				break;
			case MMI_PROFILE_HEADSET:
				msg_id = STR_PROFILES_HEADSET;
				break;
			case MMI_PROFILE_SILENT:
				msg_id = STR_PROFILES_SILENT;
				break;
#ifdef __BT_SUPPORT__       
			case MMI_PROFILE_BT:
				msg_id = STR_PROFILES_BT;
				break;
#endif
			default:
#ifdef MMI_ON_WIN32
				msg_id = STR_PROFILES_GENERAL;
#else
				MMI_ASSERT( 0 );
#endif 
		}

			
		memset( g_ActivedProfile, 0, sizeof( g_ActivedProfile ) );
		pda_idle_shortcut[i].pdaMatrixString = g_ActivedProfile;
		pfnUnicodeStrcpy( g_ActivedProfile, GetString( STR_IDLE_BLACK_BERRY_STRING7 ) );
		pfnUnicodeStrcat( ( PS8 ) g_ActivedProfile, ( S8 * ) L": " );
		pfnUnicodeStrcat( ( PS8 ) g_ActivedProfile, GetString( msg_id ) );

        }
        else
        {
            pda_idle_shortcut[i].pdaMatrixString = GetString( ( U16 ) ( STR_IDLE_BLACK_BERRY_STRING1 + i ) );
        }
    }
	
	#else
    memset( g_MissedCall, 0, sizeof( g_MissedCall ) );
    memset( g_MissedMsg, 0, sizeof( g_MissedMsg ) );
    memset( g_TodoList, 0, sizeof( g_TodoList ) );
	initPdaMissedCall();
    for ( i = 0; i < SYMBIAN_TOTAL_ITEMS; i++ )
    {
        memset( LocalData1, 0, sizeof( LocalData1 ) );
        memset( LocalData1_unicode, 0, sizeof( LocalData1_unicode ) );

        if ( pda_idle_shortcut[i].pdaMatrixMenu_ID == IDLE_SYMBIAN_SETTING2 )
        {
        	missedCallNum = g_chist_have_missed_call + g_slave_chist_missed_call_num;
        	if( MTPNP_AD_Get_CardStatus(1) == MTPNP_AD_SIMCARD_ABSENT)
        	{
        		missedCallNum = missedCallNum-g_slave_chist_missed_call_num;
        	}

			if( MTPNP_AD_Get_CardStatus(0) == MTPNP_AD_SIMCARD_ABSENT)
        	{
        		missedCallNum = missedCallNum-g_chist_have_missed_call;
        	}
            pda_idle_shortcut[i].pdaMatrixString = g_MissedCall;
            pfnUnicodeStrcpy( g_MissedCall, GetString( STR_IDLE_SYMBIAN_STRING2 ) );
            sprintf( LocalData1, ": (%d)",missedCallNum); 
            AnsiiToUnicodeString( LocalData1_unicode, LocalData1 );
            pfnUnicodeStrcat( ( PS8 ) g_MissedCall, LocalData1_unicode );
        }
        else if ( pda_idle_shortcut[i].pdaMatrixMenu_ID == IDLE_SYMBIAN_SETTING3 )
        {
            unread_inbox = Symbian_unread_msg_num(); 
            pda_idle_shortcut[i].pdaMatrixString = g_MissedMsg;
            pfnUnicodeStrcpy( g_MissedMsg, GetString( STR_IDLE_SYMBIAN_STRING3 ) );
            sprintf( LocalData1, ": (%d)", unread_inbox ); 
            AnsiiToUnicodeString( LocalData1_unicode, LocalData1 );
            pfnUnicodeStrcat( ( PS8 ) g_MissedMsg, LocalData1_unicode );
        }
#ifdef __MMI_TODOLIST__     
        else if ( pda_idle_shortcut[i].pdaMatrixMenu_ID == IDLE_SYMBIAN_SETTING4 )
        {
            Symbian_TodoListGetDetail( &yesterday_todo, &today_todo, &tomorrow_todo );
            pda_idle_shortcut[i].pdaMatrixString = g_TodoList;
            pfnUnicodeStrcpy( g_TodoList, GetString( STR_IDLE_SYMBIAN_STRING4 ) );
            pfnUnicodeStrcat( ( PS8 ) g_TodoList, ( S8 * ) L": " );

            sprintf( LocalData1, "(%d)", today_todo ); 

            AnsiiToUnicodeString( LocalData1_unicode, LocalData1 );
            pfnUnicodeStrcat( ( PS8 ) g_TodoList, LocalData1_unicode );
        }
#endif //__MMI_TODOLIST__       
        else
        {
            pda_idle_shortcut[i].pdaMatrixString = GetString( ( U16 ) ( STR_IDLE_SYMBIAN_STRING1 + i ) );
        }
    }
#endif
}


void Gui_Symbian_highlight_handler( S32 item_index )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	setSymbianIdleHilightItem(item_index);

    MMI_highlighted_item_text = get_item_text( item_index );

    MMI_mm_animation = get_image( gSymbian_icon_ani[item_index] );

    MMI_matrix_highlight_handler( item_index );
}

void Gui_Symbian_menu_locate_highlighted_item( fixed_matrix_menu* m )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i   = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if (  GetActiveScreenId() == IDLE_SCREEN_ID  )
    {
        for ( i = 0; i < SYMBIAN_TOTAL_LINES; i++ )
        {
            if ( ( m->highlighted_item ) < g_Symbian_idle_info[i].current_total_items )
            {
                m->highlighted_row = i;
                m->highlighted_column = ( m->highlighted_item - g_Symbian_idle_info[i].prev_columns ) %
                                        g_Symbian_idle_info[i].current_total_items;
                break;
            }
        }
    }
}

void Gui_Symbian_goto_next_item( fixed_matrix_menu* m )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item= m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/


    if ( m->highlighted_item >=
         ( m->g_Symbian_idle_info[m->highlighted_row].columns +
           m->g_Symbian_idle_info[m->highlighted_row].prev_columns -
           1 ) )
    {
        if ( m->flags & UI_MATRIX_MENU_LOOP )
        {
            m->highlighted_item = m->g_Symbian_idle_info[m->highlighted_row].prev_columns;
        }
        else
        {
            return;
        }
    }
    else
    {
        m->highlighted_item++;
    }
    Gui_Symbian_menu_locate_highlighted_item( m );

    if ( old_highlighted_item != m->highlighted_item )
    {
        if ( ( old_highlighted_item >= 0 ) && ( old_highlighted_item < m->n_items ) )
        {
            m->item_remove_highlight_function( m->items[old_highlighted_item], m->common_item_data );
        }   
        if ( ( m->highlighted_item >= 0 ) && ( m->highlighted_item < m->n_items ) )
        {
            m->item_highlight_function( m->items[m->highlighted_item], m->common_item_data );
        }
        m->item_unhighlighted( old_highlighted_item );
        m->item_highlighted( m->highlighted_item );
    }
}


void Gui_Symbian_goto_N_item( fixed_matrix_menu* m )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item= m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    m->highlighted_item = hilit_shortcut_id;

    Gui_Symbian_menu_locate_highlighted_item( m );

    if ( old_highlighted_item != m->highlighted_item )
    {
        if ( ( old_highlighted_item >= 0 ) && ( old_highlighted_item < m->n_items ) )
        {
            m->item_remove_highlight_function( m->items[old_highlighted_item], m->common_item_data );
        }
        if ( ( m->highlighted_item >= 0 ) && ( m->highlighted_item < m->n_items ) )
        {
            m->item_highlight_function( m->items[m->highlighted_item], m->common_item_data );
        }
        m->item_unhighlighted( old_highlighted_item );
        m->item_highlighted( m->highlighted_item );
    }
}


/*----------------------------------------------------------------------------
Function:           gui_fixed_matrix_menu_goto_previous_item
Description:        Highlights the previous item
Input Parameters:   m       is the fixed matrix menu object
Output Parameters:  none
Returns:            void
Remarks:            Does not redraw the fixed matrix menu
----------------------------------------------------------------------------*/

void Gui_Symbian_goto_previous_item( fixed_matrix_menu* m )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item= m->highlighted_item;


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if ( m->highlighted_item <= m->g_Symbian_idle_info[m->highlighted_row].prev_columns )
    {
        if ( m->flags & UI_MATRIX_MENU_LOOP )
        {
            m->highlighted_item = m->g_Symbian_idle_info[m->highlighted_row].prev_columns +
                                  ( m->g_Symbian_idle_info[m->highlighted_row].columns );
        }
        else
        {
            return;
        }
    }
    {
        m->highlighted_item--;
    }

    Gui_Symbian_menu_locate_highlighted_item( m );

    if ( old_highlighted_item != m->highlighted_item )
    {
        if ( ( old_highlighted_item >= 0 ) && ( old_highlighted_item < m->n_items ) )
        {
            m->item_remove_highlight_function( m->items[old_highlighted_item], m->common_item_data );
        }
        if ( ( m->highlighted_item >= 0 ) && ( m->highlighted_item < m->n_items ) )
        {
            m->item_highlight_function( m->items[m->highlighted_item], m->common_item_data );
        }
        m->item_unhighlighted( old_highlighted_item );
        m->item_highlighted( m->highlighted_item );
    }
}

/*----------------------------------------------------------------------------
Function:           gui_fixed_matrix_menu_goto_previous_row
Description:        Highlights an item in the previous row
Input Parameters:   m       is the fixed matrix menu object
Output Parameters:  none
Returns:            void
Remarks:            Does not redraw the fixed matrix menu
----------------------------------------------------------------------------*/

void Gui_Symbian_goto_previous_row( fixed_matrix_menu* m )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item;
    S32 old_highlighted_row;


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    old_highlighted_row = m->highlighted_row;
    old_highlighted_item = m->highlighted_item;


    m->highlighted_item = m->g_Symbian_idle_info[m->highlighted_row].prev_columns; 

    /* Menu rotation */
    if ( m->highlighted_row <= 0 )
    {
        m->highlighted_row = m->n_rows - 1;

        if ( m->highlighted_item > ( m->n_items - 1 ) )
        {
            m->highlighted_row -= 1;
            m->highlighted_item -= m->n_columns;
        }
        m->first_displayed_row = m->highlighted_row - m->displayed_rows + 1;

        if ( m->first_displayed_row < 0 )
        {
            m->first_displayed_row = 0;
        }
        m->highlighted_column = 0;
    }
    else
    {
        m->highlighted_row--;
        m->highlighted_column = 0;

        if ( m->highlighted_row < m->first_displayed_row )
        {
            m->first_displayed_row = m->highlighted_row;
        }
    }

    //must do two time, becoz of the totoal item can not be guarenteed to displayed in one screen.
    m->highlighted_item = m->g_Symbian_idle_info[m->highlighted_row].prev_columns; 


    if ( old_highlighted_item != m->highlighted_item )
    {
        if ( ( old_highlighted_item >= 0 ) && ( old_highlighted_item < m->n_items ) )
        {
            m->item_remove_highlight_function( m->items[old_highlighted_item], m->common_item_data );
        }
        if ( ( m->highlighted_item >= 0 ) && ( m->highlighted_item < m->n_items ) )
        {
            m->item_highlight_function( m->items[m->highlighted_item], m->common_item_data );
        }

        m->item_unhighlighted( old_highlighted_item );
        m->item_highlighted( m->highlighted_item );
    }
}

/*----------------------------------------------------------------------------
Function:           gui_fixed_matrix_menu_goto_next_row
Description:        Highlights an item in the next row
Input Parameters:   m       is the fixed matrix menu object
Output Parameters:  none
Returns:            void
Remarks:            Does not redraw the fixed matrix menu
----------------------------------------------------------------------------*/

void Gui_Symbian_goto_next_row( fixed_matrix_menu* m )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item;
    S32 old_highlighted_row;


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    old_highlighted_row = m->highlighted_row;
    old_highlighted_item = m->highlighted_item;

    /* Menu rotation */
    if ( m->highlighted_row >= ( m->n_rows - 1 ) )
    {
        m->highlighted_row = 0;
        m->first_displayed_row = 0;
        m->highlighted_column = 0;
    }
    else
    {
        m->highlighted_row++;
        m->highlighted_column = 0;
        if ( m->highlighted_row > ( m->first_displayed_row + m->displayed_rows - 1 ) )
        {
            m->first_displayed_row = ( m->highlighted_row - m->displayed_rows + 1 );
            if ( m->first_displayed_row > ( m->n_rows - m->displayed_rows ) )
            {
                m->first_displayed_row = ( m->n_rows - m->displayed_rows );
            }
        }
    }

    m->highlighted_item = m->g_Symbian_idle_info[( m->highlighted_row )].prev_columns;

    if ( m->highlighted_item < 0 )
    {
        m->highlighted_item = 0;
    }

    if ( m->highlighted_item > ( m->n_items - 1 ) ) /*  revert  */
    {
        m->highlighted_row = 0;

        m->highlighted_item = m->highlighted_column;
        m->first_displayed_row = 0;
    }
    /* Calvin end */
    if ( old_highlighted_item != m->highlighted_item )
    {
        if ( ( old_highlighted_item >= 0 ) && ( old_highlighted_item < m->n_items ) )
        {
            m->item_remove_highlight_function( m->items[old_highlighted_item], m->common_item_data );
        }
        if ( ( m->highlighted_item >= 0 ) && ( m->highlighted_item < m->n_items ) )
        {
            m->item_highlight_function( m->items[m->highlighted_item], m->common_item_data );
        }
        m->item_unhighlighted( old_highlighted_item );
        m->item_highlighted( m->highlighted_item );
    }
}

void Gui_Symbian_goto_item( fixed_matrix_menu* m, S32 i )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 old_highlighted_item= m->highlighted_item;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ( ( i < 0 ) || ( i > ( m->n_items - 1 ) ) )
    {
        return;
    }
    m->highlighted_item = i;

    Gui_Symbian_menu_locate_highlighted_item( m );

    if ( old_highlighted_item != m->highlighted_item )
    {
        if ( ( old_highlighted_item >= 0 ) && ( old_highlighted_item < m->n_items ) )
        {
            m->item_remove_highlight_function( m->items[old_highlighted_item], m->common_item_data );
        }
        if ( ( m->highlighted_item >= 0 ) && ( m->highlighted_item < m->n_items ) )
        {
            m->item_highlight_function( m->items[m->highlighted_item], m->common_item_data );
        }

        m->item_unhighlighted( old_highlighted_item );

        m->item_highlighted( m->highlighted_item );
    }
}

void Gui_Symbian_show_icontext_menuitem( void* item, void* common_item_data, S32 x, S32 y )
{
    S32 x1, y1, x2, y2;
	S32 icon_w,icon_h;
    /* relative x offset of text inside menu item. 
     * (which might be different from m->text_x in Arabic or scrolling)*/
    S32 text_x                      = 0;
    /* relative y offset of text inside menu item. */
    S32 text_y                      = 0;

    color_t text_color;
    UI_filled_area* f               = NULL;
    fixed_icontext_menuitem* m      = ( fixed_icontext_menuitem* ) common_item_data;
    fixed_icontext_menuitem_type* mi= ( fixed_icontext_menuitem_type* ) item;
    dword flags;
    UI_string_type _text;
    UI_image_type _icon;
    byte animation_flag             = 0;
    S32 sw = 0, sh = 0, max_text_width = 0;
    S32 is_text_full_width          = 0;

    flags = mi->flags;
    flags |= m->flags;
    _text = mi->item_text;
    _icon = mi->item_icon;
    x1 = x;
    y1 = y;
    x2 = x1 + m->width - 1;
    y2 = y1 + m->height - 1;

	icon_w = icon_h = 0;

    if ( flags & UI_MENUITEM_STATE_FOCUSSED )
    {
        GUI_current_fixed_icontext_menuitem_x = x;
        GUI_current_fixed_icontext_menuitem_y = y;
    }
	
	if ( _icon != UI_NULL_IMAGE )
	{
		gui_measure_image( _icon, &icon_w, &icon_h );
	}

    if ( !( flags & UI_MENUITEM_DISABLE_TEXT_DISPLAY ) )
    {
        /*  Autocalculate the text position based on font   */
        gui_set_font( m->text_font );
        gui_measure_string( _text, &sw, &sh );
        if ( flags & UI_MENUITEM_CENTER_TEXT_X )
        {
            m->text_x = ( m->width >> 1 ) - ( sw >> 1 );
        }
		else
		{
			m->text_x = x1 + icon_w +1;
		}
        if ( flags & UI_MENUITEM_CENTER_TEXT_Y )
        {
            m->text_y = ( m->height >> 1 ) - ( sh >> 1 );
        }
    }

    text_x = m->text_x;
    text_y = m->text_y;


    if ( r2lMMIFlag )
    {
        text_x = text_x + sw - 1;
    }

    /* Setup text scroll and background filler */
    if ( flags & UI_MENUITEM_STATE_FOCUSSED )
    {
        text_x += m->scroll_x;
    }

    if ( ( flags & UI_MENUITEM_STATE_FOCUSSED ) && ( flags & UI_MENUITEM_FOCUSSED_ANIMATE ) )
    {
        animation_flag = 1;
    }

    if ( ( flags & UI_MENUITEM_STATE_FOCUSSED ) && !( flags & UI_MENUITEM_DISABLE_HIGHLIGHT ) )
    {
        if ( !( flags & UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS ) )
        {
            f = m->focussed_filler;
        }
        text_color = m->focussed_text_color;
    }
    else if ( flags & UI_MENUITEM_STATE_DISABLED )
    {
        if ( !( flags & UI_MENUITEM_DISABLE_BACKGROUND ) && !( flags & UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS ) )
        {
            f = m->disabled_filler;
        }
        text_color = m->focussed_text_color;
    }
    else if ( flags & UI_MENUITEM_STATE_SELECTED )
    {
        f = m->selected_filler;
        text_color = m->selected_text_color;
        if ( flags & UI_MENUITEM_SELECTED_ANIMATE )
        {
            animation_flag = 1;
        }
    }
    else
    {
        if ( !( flags & UI_MENUITEM_DISABLE_BACKGROUND ) && !( flags & UI_MENUITEM_DISABLE_BACKGROUND_ALWAYS ) )
        {
            f = m->normal_filler;
        }

        text_color = gui_color( 255, 255, 255 );
    }
	
#if defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
	 Symbian_idle_hide_menu_String();
	#endif
    gui_push_clip();

    if ( MMI_current_menu_type == SYMBIAN_IDLE )
    {
        gui_set_clip( x1, y1, x2, y2 );
    }

    if ( m->flags & UI_MENUITEM_DISABLE_HIGHLIGHT_DISPLAY )
    {
        f = NULL;
    }

    if ( _icon != UI_NULL_IMAGE )
    {
    	S32 icon_y = 0;
		icon_y = y1+( m->height >> 1 ) - ( icon_h >> 1 );
        if ( mi->item_icon_handle != GDI_ERROR_HANDLE )
        {
            gdi_image_stop_animation( mi->item_icon_handle );

            mi->item_icon_handle = GDI_ERROR_HANDLE;
        }

        if ( animation_flag)
        {
			#if defined (__MMI_BLACK_BERRY_IDLE_STYLE__) || defined (__MMI_WIN_STYLE_SUPPORT__)
			gdi_image_draw_animation_once( x1, y1, MMI_mm_animation, ( gdi_handle * ) &mi->item_icon_handle );
                        //gdi_image_draw( x1, y1, MMI_mm_animation);//zhouqin modify
                        #else
        	if( flags & UI_MENUITEM_DISABLE_TEXT_DISPLAY )
        	{
     			//gdi_draw_solid_rect(x1,y1,x2,y2,PDA_MATRIX_BACK_COLOR);
        		//gdi_draw_rect(x1,y1,x2,y2,PDA_MATRIX_LINE_COLOR);
        		gui_show_transparent_animated_image_frame( x1, icon_y, 
        		(UI_image_type)GetImage(IMG_IDLE_SYMBIAN_MATRIX_HILIGHT+g_curr_thm_index), 0, 0 );
				gui_show_transparent_animated_image_frame( x1, icon_y, _icon, 0, 0 );
        	}
			else
			{
				//caijianbing modify for pda hilight
				gdi_draw_round_rect(x1+icon_w+1,y1,x2,y2,PDA_LINE_END_COLOR,PDA_LINE_START_COLOR,1);
				gui_show_transparent_animated_image_frame( x1, icon_y, _icon, 0, 0 );
				text_color = gui_color( 0, 0, 0 );
			}
        	#endif
        }
        else if ( m->flags & UI_MENUITEM_STATE_ANIMATE )
        {
            //gui_show_transparent_animated_image_frame(x1,y1,_icon,m->icon_transparent_color,1);
		#if !defined (__MMI_BLACK_BERRY_IDLE_STYLE__) ||defined (__MMI_WIN_STYLE_SUPPORT__)
			gdi_draw_solid_rect(x1,y1,x2,y2,GDI_COLOR_TRANSPARENT);
			#endif
            gui_show_transparent_animated_image_frame( x1, icon_y, _icon, 0, 1 );
            m->flags &= ~UI_MENUITEM_STATE_ANIMATE;
        }
        else
        {
            //gui_show_transparent_animated_image_frame(x1,y1,_icon,m->icon_transparent_color,0);
		#if !defined (__MMI_BLACK_BERRY_IDLE_STYLE__) ||defined (__MMI_WIN_STYLE_SUPPORT__)
			gdi_draw_solid_rect(x1,y1,x2,y2,GDI_COLOR_TRANSPARENT);
			#endif
            gui_show_transparent_animated_image_frame( x1, icon_y, _icon, 0, 0 );
        }
    }

    if ( !( flags & UI_MENUITEM_DISABLE_TEXT_DISPLAY ) )
    {
        gui_push_text_clip();
		#if defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
		if(( flags & UI_MENUITEM_STATE_FOCUSSED )&& IsIdleShowMenuString())
		{
			x1 = (MAIN_LCD_DEVICE_WIDTH - sw)>>1;
			y1 = SYMBIAN_IDLE_MENU_NAME_Y;
			gui_set_text_clip( x1, y1, x1+sw+1, y1+sh+1 );
			max_text_width = sw;
			
			if(r2lMMIFlag)
			{
				gui_move_text_cursor( x1+sw,  y1 );
			}
			else
			{
				gui_move_text_cursor( x1,  y1 );
			}
			
			gui_set_line_height( sh );
        	gui_set_text_color( text_color );
			gui_print_text( _text );		
			//WW
			#if 0
			StartTimer(SYMBIAN_IDLE_HIDE_TIMER, 3000, Symbian_idle_hide_menu_Strig_handle);
			#endif			
		}
		#else
        if ( !( flags & UI_MENUITEM_DISABLE_TEXT_CLIP ) )
        {
            if ( is_text_full_width )
            {
                /* FIXME. text_x might not be 0 */
                gui_set_text_clip( x1, y1, x2, y2 );
                max_text_width = m->width;
            }
            else
            {
                if ( m->icon_x > m->text_x )
                {
                    gui_set_text_clip( x1, y1, x1 + m->icon_x - 1, y2 );
                    max_text_width = m->icon_x - m->text_x;
                }
                else
                {
                    if ( !r2lMMIFlag )
                    {
                        gui_set_text_clip( x1 + m->text_x, y1, x2, y2 );
                    }
                    else
                    {
                        gui_set_text_clip( x1, y1, x2 - m->text_x, y2 );
                    }
                    max_text_width = x2 - ( x1 + m->text_x ) + 1;
                }
            }
        }
        else
        {
            max_text_width = m->width - m->text_x;
        }
        gui_move_text_cursor( x1 + text_x,  y1 + text_y );

        gui_set_line_height( sh );
        gui_set_text_color( text_color );

        if ( ( flags & UI_MENUITEM_STATE_FOCUSSED ) &&
             ( flags & UI_MENUITEM_MARQUEE_SCROLL ) &&
             GUI_current_fixed_icontext_menuitem_scroll )
        {
#ifdef __MMI_SUPPORT_DUMP_SCREEN_STRING__
            g_mmi_frm_cntx.dump_screen_info.hightlight_type = MMI_SCREEN_HIGHLIGHT_ICONTEXT_MARQUEE_SCROLL;
#endif  
            /* fix long text in inline-select display overlap issue */
            if ( !( mi->ext_flags & UI_MENUITEM_EXT_DISABLE_FOCUSSED_TEXT_DISPLAY ) )
            {
                gui_print_text( _text );
            }

            if ( !r2lMMIFlag )
            {
                gui_move_text_cursor( x1 + text_x + m->text_width + UI_text_menuitem_scroll_gap, y1 + text_y );
            }
            else
            {
                gui_move_text_cursor( x1 + text_x - ( m->text_width + UI_text_menuitem_scroll_gap ), y1 + text_y );
            }

            gui_set_line_height( sh );
            gui_print_text( _text );
        }
        else
        {
            gui_print_text( _text );
        }
		#endif
        gui_pop_text_clip();
    }

    gui_pop_clip();
}

void Gui_Symbian_icontext_menuitem_setting( fixed_icontext_menuitem* m, Symbian_idle_info* idleinfo )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    m->width = idleinfo->item_width;
    m->height = idleinfo->item_height;
    m->flags |= idleinfo->flags;
    m->ext_flags = 0;
    m->text_x = 2;
    m->text_y = 0;
    m->icon_x = idleinfo->x; //does not useful actually
    m->icon_y = idleinfo->y;//does not useful actually
    m->scroll_x = 0;//does not useful actually
    m->scroll_width = 0;//does not useful actually
}

void Gui_Symbian_icontext_menuitem_reset_flag( fixed_icontext_menuitem* m, Symbian_idle_info* idleinfo )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    m->flags &= ~idleinfo->flags;
}

void Gui_Symbian_Show_Idlescreen( fixed_matrix_menu* m )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 x1, y1, x2, y2, width, height;
    UI_filled_area* f;
    S32 i, j;
    S32 cx1, cy1, cx2, cy2;
    S32 tx1, ty1, tx2, ty2;
    S32 ix = 0, iy = 0, ix2 = 0, iy2 = 0;
    S32 highlite_x = 0, highlite_y = 0; //Show selected item after all other items displayed
    S32 xPos, xPos2, yPos, yPos2;//013105 Calvin added
    fixed_icontext_menuitem* mCommom= ( fixed_icontext_menuitem* ) m->common_item_data; 


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    gui_get_clip( &cx1, &cy1, &cx2, &cy2 );
    gui_get_text_clip( &tx1, &ty1, &tx2, &ty2 );


    if ( r2lMMIFlag )
    {
        x1 = m->x;
        x2 = UI_device_width;
    }
    else
    {
        x1 = m->x;
        x2 = x1 + m->width - 1;
    }

    y1 = m->y;
    y2 = y1 + m->height - 1;
    if ( m->flags & UI_LIST_MENU_STATE_FOCUSSED )
    {
        f = m->focussed_filler;
    }
    else
    {
        f = m->normal_filler;
    }

    gui_set_clip( x1, y1, x2, y2 );
    gui_draw_rectangle( x1, y1, x2, y2, gui_color( 0, 0, 255 ) );

    if ( m->n_items == 0 )
    {
        return;
    }

    width = x2 - x1 + 1;
    height = y2 - y1 + 1;

    gui_set_text_clip( x1, y1, x2, y2 );
    gui_set_clip( x1, y1, x2, y2 );


    for ( i = 0; i < m->displayed_rows; i++ )
    {
        ix = m->g_Symbian_idle_info[i].x;
        iy = m->g_Symbian_idle_info[i].y;

        Gui_Symbian_icontext_menuitem_setting( mCommom, &g_Symbian_idle_info[i] );
		
        for ( j = 0; j < m->g_Symbian_idle_info[i].columns; j++ )
        {
            ix2 = ix + m->g_Symbian_idle_info[i].item_width;
            iy2 = iy + m->g_Symbian_idle_info[i].item_height;
            gdi_layer_push_clip();
            gui_set_clip( ix, iy, ix2, iy2 );
            gui_set_text_clip( ix, iy, ix2, iy2 );

            if ( ( ( j + m->g_Symbian_idle_info[i].prev_columns ) == m->highlighted_item ) &&
                 ( gKeyPadLockFlag != 1 ) )
            {
                m->item_remove_highlight_function( m->items[j + ( m->g_Symbian_idle_info[i].prev_columns )], mCommom );
                m->item_display_function( m->items[j + ( m->g_Symbian_idle_info[i].prev_columns )], mCommom, ix, iy ); //pointer to Gui_Symbian_show_icontext_menuitem
                m->item_highlight_function( m->items[j + ( m->g_Symbian_idle_info[i].prev_columns )], mCommom ); //pointer to gui_highlight_fixed_icontext_menuitem
                highlite_x = ix;
                highlite_y = iy;
            }
            else
            {
                m->item_display_function( m->items[j + ( m->g_Symbian_idle_info[i].prev_columns )], mCommom, ix, iy );
                if ( ( j + m->g_Symbian_idle_info[i].prev_columns ) == m->highlighted_item )
                {
                    highlite_x = ix;
                    highlite_y = iy;
                }
            }

            gdi_layer_pop_clip();

            ix += m->g_Symbian_idle_info[i].item_width;
        }
        Gui_Symbian_icontext_menuitem_reset_flag( mCommom, &g_Symbian_idle_info[i] );       

        ix = 0;
        iy = 0;
    }

    xPos = highlite_x;
    xPos2 = xPos + g_Symbian_idle_info[m->highlighted_row].item_width - 1;
    yPos = highlite_y;
    yPos2 = yPos + g_Symbian_idle_info[m->highlighted_row].item_height - 1;

    gdi_layer_push_clip();
    gdi_layer_set_clip( xPos, yPos, xPos2, yPos2 );
    gdi_image_cache_bmp_get( xPos, yPos, xPos2, yPos2, &m->buffer );
    gdi_layer_pop_clip();
    m->cache_bmp_x1 = xPos;
    m->cache_bmp_x2 = xPos2;
    m->cache_bmp_y1 = yPos;
    m->cache_bmp_y2 = yPos2;
    m->last_hilited_x = highlite_x;
    m->last_hilited_y = highlite_y;

    Gui_Symbian_icontext_menuitem_setting( mCommom, &g_Symbian_idle_info[m->highlighted_row] );
    m->item_display_function( m->items[m->highlighted_item], mCommom, highlite_x, highlite_y );
    Gui_Symbian_icontext_menuitem_reset_flag( mCommom, &g_Symbian_idle_info[m->highlighted_row] );      

    gui_set_clip( cx1, cy1, cx2, cy2 );
    gui_set_text_clip( tx1, ty1, tx2, ty2 );
}

void Gui_Symbian_Show_Idlescreen_ENH( fixed_matrix_menu* m )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 hilited_x1, hilited_x2, hilited_y1, hilited_y2;
    S32 xPos, xPos2;
    S32 yPos, yPos2;
    fixed_icontext_menuitem* mCommom= ( fixed_icontext_menuitem* ) m->common_item_data; 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_push_clip();
    gdi_layer_set_clip( m->cache_bmp_x1, m->cache_bmp_y1, m->cache_bmp_x2, m->cache_bmp_y2 );

    gdi_image_cache_bmp_draw( m->cache_bmp_x1, m->cache_bmp_y1, &m->buffer );
    gdi_layer_pop_clip();

    hilited_x1 = g_Symbian_idle_info[m->highlighted_row].x +
                 g_Symbian_idle_info[m->highlighted_row].item_width * m->highlighted_column;

    hilited_y1 = g_Symbian_idle_info[m->highlighted_row].y;

    hilited_x2 = hilited_x1 + g_Symbian_idle_info[m->highlighted_row].item_width;

    hilited_y2 = hilited_y1 + g_Symbian_idle_info[m->highlighted_row].item_height;

    xPos = hilited_x1;
    xPos2 = xPos + g_Symbian_idle_info[m->highlighted_row].item_width - 1;
    yPos = hilited_y1;
    yPos2 = yPos + g_Symbian_idle_info[m->highlighted_row].item_height - 1;

    gdi_layer_push_clip();
    gdi_layer_set_clip( xPos, yPos, xPos2, yPos2 );

    gdi_image_cache_bmp_get( xPos, yPos, xPos2, yPos2, &m->buffer );
    gdi_layer_pop_clip();

    m->cache_bmp_x1 = xPos;
    m->cache_bmp_x2 = xPos2;
    m->cache_bmp_y1 = yPos;
    m->cache_bmp_y2 = yPos2;

    m->last_hilited_x = hilited_x1;
    m->last_hilited_y = hilited_y1;

    Gui_Symbian_icontext_menuitem_setting( mCommom, &g_Symbian_idle_info[m->highlighted_row] );

    m->item_display_function( m->items[m->highlighted_item], mCommom, hilited_x1, hilited_y1 );

    Gui_Symbian_icontext_menuitem_reset_flag( mCommom, &g_Symbian_idle_info[m->highlighted_row] );
}


void Gui_Symbian_Idlescreen( void )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#if defined (__MMI_WIN_STYLE_SUPPORT__)
	int ypos[3] = {0,LINE2_Y,LINE3_Y};
#endif
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    gui_lock_double_buffer();
	gdi_layer_push_and_set_active( wgui_base_layer );

    if ( MMI_fixed_matrix_menu.last_highlighted_item != -1 )
    {
        Gui_Symbian_Show_Idlescreen_ENH( &MMI_fixed_matrix_menu );
    }
    else
    {
        Gui_Symbian_Show_Idlescreen( &MMI_fixed_matrix_menu );
    }

#if defined (__MMI_WIN_STYLE_SUPPORT__)
    gdi_layer_push_and_set_active( wgui_layer_1 );

	switch(MMI_fixed_matrix_menu.highlighted_row)
	{
	case 0:
			draw_wallpaper();
		break;
	case 1:
	case 2:
			draw_wallpaper();
			gdi_image_draw_id(0,ypos[MMI_fixed_matrix_menu.highlighted_row],(U16)(IMG_IDLE_SYMBIAN_LINE1_HIGHLIGHT+MMI_fixed_matrix_menu.highlighted_row));
			break;
	default:
		break;
	}

	gdi_layer_pop_and_restore_active();
#endif
gdi_layer_pop_and_restore_active();
    gui_unlock_double_buffer();
    gui_BLT_double_buffer( MMI_fixed_matrix_menu.x
                         , MMI_fixed_matrix_menu.y
                         , MMI_fixed_matrix_menu.x + MMI_fixed_matrix_menu.width - 1
                         , MMI_fixed_matrix_menu.y + MMI_fixed_matrix_menu.height - 1 );
}

void configure_Symbian_Idlescreen( S32 item_width, S32 item_height, S32 n_columns, S32 n_rows )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    MMI_fixed_matrix_menu.n_columns = n_columns;
    MMI_fixed_matrix_menu.n_rows = n_rows;
    MMI_fixed_matrix_menu.displayed_rows = n_rows;
    MMI_fixed_matrix_menu.displayed_columns = 0;
}

void Gui_Symbian_goto_item_no_redraw( S32 item_index )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    MMI_fixed_matrix_menu.highlighted_item = -1;
    Gui_Symbian_goto_item( &MMI_fixed_matrix_menu, item_index );
}

void hide_Symbian_Idle_animation_frame( void )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    gui_hide_fixed_matrix_menu_highlighted_item( &MMI_fixed_matrix_menu );
}


void ShowCategorySymbianScreen( U16 left_softkey
                              , U16 left_softkey_icon
                              , U16 right_softkey
                              , U16 right_softkey_icon
                              , S32 number_of_items
                              , g_Symbian_idle_item* pda_idle_shortcut
                              , S32 highlighted_item
                              , U8* history_buffer )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S32 i;
#ifdef __MMI_IDLE_FULL_SCREEN__
    on_idle_screen = 1;
    disable_softkey_background();
#endif

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_lock_double_buffer();

#ifdef __GDI_MEMORY_PROFILE_2__
    //Enable multilayer
    gdi_layer_multi_layer_enable();

    gdi_layer_get_base_handle( &wgui_base_layer );

    if ( wgui_layer_1 == GDI_LAYER_EMPTY_HANDLE )
    {
        gdi_layer_create( 0, 0, UI_device_width, UI_device_height, &wgui_layer_1 );
    }

    gdi_layer_set_active( wgui_base_layer );

    gui_layer_transparent( wgui_base_layer, 255, 0, 0, UI_device_width - 1, UI_device_height );

#endif

    MMI_menu_shortcut_number = -1;
    change_left_softkey( left_softkey, left_softkey_icon );
    change_right_softkey( right_softkey, right_softkey_icon );
    SetupCategoryKeyHandlers();
#ifdef __MMI_TOUCH_SCREEN__ 
    mmi_PdaStyleRegisterPenHandle();
#endif  

    register_hide_left_softkey(cat33_hide_lsk);
    register_hide_right_softkey(cat33_hide_rsk);
    #ifdef __MMI_WGUI_CSK_ENABLE__
    register_hide_softkey(cat33_hide_csk, MMI_CENTER_SOFTKEY);
    #endif 
	
    register_hide_status_icon_bar( 0, idle_screen_hide_status_icons_bar0 );
    register_hide_status_icon_bar( 1, UI_dummy_function );
    set_status_icon_bar_clip( 0, 0, 0, UI_device_width - 1, MMI_status_bar_height - 1 );
    show_status_icon_bar( 0 );

    set_main_LCD_dt_date_hide_function( cat33_hide_time_display ); 
    set_main_LCD_dt_time_hide_function( cat33_hide_time_display );
    set_dt_display( DT_IDLE_SCREEN );
    create_fixed_icontext_menuitems();

    MMI_current_menu_type = SYMBIAN_IDLE;

    MMI_fixed_matrix_menu.flags |= UI_MATRIX_MENU_LOOP | UI_MATRIX_MENU_AUTO_DISABLE_SCROLLBAR;

    associate_fixed_icontext_matrix();
    for ( i = 0; i < number_of_items; i++ )
    {
        add_fixed_icontext_item( ( UI_string_type ) ( pda_idle_shortcut[i].pdaMatrixString )
                               , get_image( pda_idle_shortcut[i].pda_Matrix_Icon_ID ) );
        gSymbian_icon_ani[i] = pda_idle_shortcut[i].pda_Matrix_Icon_ani_ID;
    }


    if ( !gKeyPadLockFlag )
    {
        register_fixed_matrix_loop_keys();
    }

    set_status_icon_bar_clip( 0, 0, 0, UI_device_width - 1, MMI_status_bar_height - 1 );
    show_status_icon_bar( 0 );

    set_MMI_matrix_main_menu_theme();

    gui_set_fixed_matrix_menu_theme( &MMI_fixed_matrix_menu, current_UI_theme->fixed_matrix_menu_theme );

    gui_set_fixed_icontext_menuitem_theme( &MMI_fixed_icontext_menuitem
                                         , current_UI_theme->fixed_icontext_menuitem_theme );

    register_fixed_matrix_highlight_handler( Gui_Symbian_highlight_handler );

    move_fixed_matrix( 0, MMI_status_bar_height );

	#if defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
	resize_fixed_matrix( MMI_content_width, ( UI_device_height - MMI_status_bar_height ) );
	#else
    resize_fixed_matrix( MMI_content_width, ( UI_device_height - MMI_status_bar_height - MMI_softkey_height ) );
	#endif

	MMI_fixed_matrix_menu.last_highlighted_item = -1;

    memcpy( &MMI_fixed_matrix_menu.g_Symbian_idle_info
          , g_Symbian_idle_info
          , sizeof( Symbian_idle_info ) * SYMBIAN_TOTAL_LINES );     
    configure_Symbian_Idlescreen( 0, 0, 0, SYMBIAN_TOTAL_LINES );

    MMI_disable_title_shortcut_display = 0;

    /* 030705 Calvin added */

    if ( gKeyPadLockFlag != 1 )
    {
        //Gui_Symbian_goto_item_no_redraw( MMI_fixed_matrix_menu.highlighted_item );
       Gui_Symbian_goto_item_no_redraw( getSymbianIdleHilightItem());
    }
    MMI_fixed_icontext_menuitem.flags |= UI_MENUITEM_FOCUSSED_ANIMATE;

   // MMI_fixed_matrix_menu.buffer.buf_ptr = ( U8 * )OslMalloc( OPTIMIZE_PDA_BUF_SIZE );
	 MMI_fixed_matrix_menu.buffer.buf_ptr= opt_pda_mm_buf;

    gui_unlock_double_buffer();

    ExitCategoryFunction = ExitCategorySymbianScreen;
    UI_hide_animation_frame_function = hide_Symbian_Idle_animation_frame;

    RedrawCategoryFunction = RedrawCategorySymbianScreen;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
    RedrawCategoryFunction();
}


void RedrawCategorySymbianScreen( void )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    gui_lock_double_buffer();
    gui_reset_clip();
	
    in_idle_screen();

    //gdi_layer_set_active( wgui_layer_1 );
  gdi_layer_push_and_set_active(wgui_layer_1);
    draw_wallpaper();
    gdi_layer_pop_and_restore_active();
//caijianbing modify 20091211
#if !defined(__NOKIA_UI_STYLE_SUPPORT__)//!defined (__NOKIA_FULL_QWERTY_KEY_SUPPORT__) && !defined (__NOKIA_NORMAL_KEY_SUPPORT__)
	#if defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
	 //gdi_image_draw_id(0,0,(U16)(IMG_IDLE_SYMBIAN_STATUS_BACKGROUND));//zhouqin modify
	#endif
#endif
	arrange_status_icons();
	gdi_layer_push_and_set_active( wgui_base_layer );
	#if defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
  	gdi_layer_set_opacity(TRUE, 255);
	#endif
  	show_status_icons();	
	gdi_push_and_set_alpha_blending_source_layer(wgui_layer_1);
	
	Gui_Symbian_Idlescreen();

 	idle_screen_show_network_details(); /* EntryIdleScreen(); */
	update_mainlcd_dt_display();

   // MMI_fixed_matrix_menu.last_highlighted_item = -1;
	//Symbian_idle_screen_show_network_name();
#if !defined (__MMI_BLACK_BERRY_IDLE_STYLE__) ||defined (__MMI_WIN_STYLE_SUPPORT__)

#if !defined (__MMI_WIN_STYLE_SUPPORT__)
	#ifdef __MMI_WGUI_CSK_ENABLE__
	if ( gKeyPadLockFlag == 0 )
	{
		//change_softkey(STR_GLOBAL_OPEN,0,MMI_CENTER_SOFTKEY);
		//EnableCenterSoftkey(STR_GLOBAL_OPEN, 0);		
		//caijianbing modify 20091125
		EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
	}
	else
	{
		change_softkey(0,0,MMI_CENTER_SOFTKEY);
	}
	show_softkey(MMI_CENTER_SOFTKEY);
	#endif
#endif
    show_left_softkey();
    show_right_softkey();
#endif
    gdi_layer_pop_and_restore_active();
    gui_unlock_double_buffer();

    gdi_layer_blt( wgui_layer_1, wgui_base_layer, 0, 0, 0, 0, UI_device_width - 1, UI_device_height - 1 );

    register_hide_status_icon_bar( 0, idle_screen_hide_status_icons_bar0 );
    register_hide_status_icon_bar( 1, idle_screen_hide_status_icons_bar1 );
}

void ExitCategorySymbianScreen( void )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    leave_idle_screen();
    gdi_pop_and_restore_alpha_blending_source_layer();
#ifdef __GDI_MEMORY_PROFILE_2__

#if defined (ENABLE_ANIMATED_WALLPAPERS)
    animation_handle = GDI_ERROR_HANDLE;
#endif

    gui_free_multilayer_screen();

#endif  

#ifdef __MMI_IDLE_FULL_SCREEN__
    on_idle_screen = 0;
#endif

    if ( extra_scrolling_text )
    {
        gui_scrolling_text_stop( extra_scrolling_text );
        extra_scrolling_text = NULL;
    }

    gui_cancel_timer( cat33_animation_processor );
#if defined(ENABLE_ANIMATED_WALLPAPERS) && defined(__GDI_MEMORY_PROFILE_2__)
    if ( animation_handle != GDI_ERROR_HANDLE )
    {
        gdi_image_stop_animation( animation_handle );
        animation_handle = GDI_ERROR_HANDLE;
    }
#endif

    close_status_icons();
    register_hide_status_icon_bar( 1, MMI_dummy_function );

    ClearHighlightHandler();
    UI_hide_animation_frame_function = UI_dummy_function;
    gui_hide_animations();

    MMI_fixed_icontext_menuitem.flags &= ~UI_MENUITEM_FOCUSSED_ANIMATE;
    //  MMI_fixed_icontext_menuitem.flags&=~UI_MATRIX_MENU_FOR_PDA_STYLE;

    //  MMI_fixed_matrix_menu.flags&=~UI_MATRIX_MENU_FOR_PDA_STYLE;

    set_MMI_matrix_sub_menu_theme();
    gui_set_fixed_matrix_menu_theme( &MMI_fixed_matrix_menu, current_UI_theme->fixed_matrix_menu_theme );
    gui_set_fixed_icontext_menuitem_theme( &MMI_fixed_icontext_menuitem
                                         , current_UI_theme->fixed_icontext_menuitem_theme );

#if 1//def __MMI_MATRIX_MAIN_MENU_OPTIMIZE__
    /* free matrix highlight buffer */
   // applib_mem_screen_free( MMI_fixed_matrix_menu.buffer.buf_ptr ); //this is very important, 
   // OslMfree( MMI_fixed_matrix_menu.buffer.buf_ptr ); //this is very important, 
#endif /* __MMI_MATRIX_MAIN_MENU_OPTIMIZE__ */
    reset_softkeys();
    enable_softkey_background();
    reset_menu_shortcut_handler();
    reset_fixed_list();
    reset_fixed_matrix();
    close_main_LCD_dt_display();

    ExitCategoryFunction = MMI_dummy_function;
    RedrawCategoryFunction = MMI_dummy_function;
    GetCategoryHistory = dummy_get_history;
    GetCategoryHistorySize = dummy_get_history_size;
}



extern 	UI_string_type idle_screen_network_name[];
extern stFontAttribute idle_screen_network_name_font;
//extern UI_string_type g_slave_idle_screen_line1text;
//#define  PDA_IDLESCREEN_TEXT_GAP  5
extern scrolling_text *extra_scrolling_text ;
extern S16 extra_text_x1 ;
extern S16 extra_text_y1 ;
extern S16 extra_text_x2 ;
extern S16 extra_text_y2 ;
extern UI_string_type idle_screen_network_status[] ;
extern stFontAttribute idle_screen_network_status_font ;

void Symbian_idle_screen_show_network_name(void)
#if 1
{

	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	color_t c;
	S32 width, x, y = 0, h = 0, h2 = 0;
	S32 h_Total = 0, h2_Total = 0;
	BOOL show_extra = FALSE;
	UI_string_type extra_text = NULL;
	U8 i;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	void (*extra_scrolling_text_timer) (void) = NULL;

#if !defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) || !defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
	S32 w_inset = MMI_status_icon_bars[1].x2 - MMI_status_icon_bars[1].x1 + 1;
#else 
	S32 w_inset = X_INSERT;
#endif 

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined (__MMI_SYMBIAN_STYLE_IDLE__)  || defined (__MMI_WIN_STYLE_SUPPORT__)
	//Symbian_idle_screen_show_network_name();
	//return;
#endif
/*- zhouqin 2011-02-16 modify for q6 320X240 */


#ifdef __GDI_MEMORY_PROFILE_2__
    gdi_layer_push_clip();
    gdi_layer_set_clip(0, 0, UI_device_width - (MMI_status_icons[STATUS_ICON_BATTERY_STRENGTH].width) - 1, UI_device_height);
    if (extra_text_x1 >= 0)
    {
        gdi_draw_solid_rect(extra_text_x1, extra_text_y1-1, extra_text_x2, extra_text_y2+2, GDI_COLOR_TRANSPARENT);
    }
#endif /* __GDI_MEMORY_PROFILE_2__ */ 

	mmi_trace(g_sw_NW, "MMI_NW: idle_screen_show_network_details");
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if (idle_screen_network_name[i] != NULL)
		{
			mmi_trace(g_sw_NW, "MMI_NW: show idle_screen_network_name[%d]",i);
			/* set font according to string width */
		#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
			gdi_layer_push_clip();
			gdi_layer_set_clip(MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width,
								0,
								UI_device_width - (MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width) - 1,
								MMI_title_height);
			if (gui_get_string_width(idle_screen_network_name[0]) > UI_device_width - MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width - MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width - 1)
			{
				gui_set_font(&MMI_small_font);
			}
			else
			{
				gui_set_font(&MMI_medium_font);
			}
		#else /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 
			gui_set_font(&idle_screen_network_name_font);
		#endif /* __MMI_UI_DALMATIAN_STATUSBAR__ */ 

			/*
			* #if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
			* c = gui_color(255,255,255);
			* #elif defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
			* gui_set_font(&idle_screen_network_name_font);
			* c=gui_color(255,255,255);
			* #else
			* gui_set_font(&idle_screen_network_name_font);
			* c=gui_color(0,255,255);
			* #endif
			*/
			c = gui_color(255, 255, 255);
			gui_set_text_color(c);

		#if defined(__MMI_UI_DALMATIAN_STATUSBAR__) || defined(__MMI_UI_DALMATIAN_IDLESCREEN__)
			c = *current_MMI_theme->title_text_color;
		#elif defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) && defined(__MMI_MAINLCD_240X320__)
			c = gui_color(18, 114, 175);
		#else
			c = gui_color(0, 0, 0);
		#endif

			gui_set_text_border_color(c);
	width= gui_get_string_width(idle_screen_network_name[i]);
			gui_measure_string(idle_screen_network_name[i], &width, &h);

			h = UI_get_font_height(&MMI_large_font);

			if (!r2lMMIFlag)
			{
				x = (UI_device_width >> 1) - (width >> 1);
			}
			else
			{
				x = (UI_device_width >> 1) - (width >> 1) + width;
			}

		#if defined(__MMI_UI_DALMATIAN_STATUSBAR__)
			if (UI_font->type == MMI_small_font.type)
			{
				y = (MMI_title_height >> 1) - (h >> 1); /* for center in Title bar */
			}
			else
			{
				y = MMI_title_height - (h + 1); /* add 1 to create gap bottom of text */
			}
		#elif defined (__MMI_UI_TECHNO_IDLESCREEN_BAR__)
			y = (UI_device_height >> 1) + (UI_device_height >> 2) - PDA_IDLESCREEN_TEXT_GAP - h;
		#else 
			#ifdef __MMI_UI_STATUS_BAR_AT_BOTTOM__
			y = MMI_status_bar_height + 4;
			/*+ zhouqin 2011-02-16 modify for q6 320X240 */
			#elif defined(__MMI_SYMBIAN_STYLE_IDLE__)
			y = IDLE_NETWORK_GAP + 2;
			/*- zhouqin 2011-02-16 modify for q6 320X240 */
			#else 
			y = MMI_status_bar_height + 2;//zhouqin 
			#endif 
		#endif


			y += (h+PDA_IDLESCREEN_TEXT_GAP)*i;//each sim network name y postion

			h_Total += (h+PDA_IDLESCREEN_TEXT_GAP);

			gui_move_text_cursor(x, y + PDA_IDLESCREEN_TEXT_GAP);

			gui_print_bordered_text(idle_screen_network_name[i]);

		#if defined(__MMI_UI_DALMATIAN_STATUSBAR__)
			gdi_layer_pop_clip();
			gdi_layer_blt_previous(MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width,
									0,
									UI_device_width - (MMI_status_icons[STATUS_ICON_SIGNAL_STRENGTH].width) - 1,
									MMI_title_height);
		#endif /* defined(__MMI_UI_DALMATIAN_STATUSBAR__) */ 
		}   /* idle_screen_network_name!=NULL */
		else
		{
			mmi_trace(g_sw_NW, "MMI_NW: %s(%d) name is NULL", __FUNCTION__, i);
		}

		if (idle_screen_network_status[i] != NULL)
		{
			mmi_trace(g_sw_NW, "MMI_NW: show idle_screen_network_status[%d]",i);
			gui_set_font(&idle_screen_network_status_font);

		#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__)
			#ifdef __MMI_MAINLCD_240X320__
			c = gui_color(18, 114, 175);
			#else 
			c = gui_color(255, 255, 255);
			#endif 
		#else /* defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) */ 
			#ifdef __MMI_UI_DALMATIAN_IDLESCREEN__
			c = *current_MMI_theme->title_text_color;
			#else 
			c = gui_color(255, 255, 0);
			#endif 
		#endif /* defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) */ 
			gui_set_text_color(c);

		#if defined(__MMI_UI_TECHNO_IDLESCREEN_BAR__) && defined(__MMI_MAINLCD_240X320__)
			c = gui_color(255, 255, 255);
		#else 
			c = gui_color(0, 0, 0);
		#endif 
			gui_set_text_border_color(c);

			width = gui_get_string_width(idle_screen_network_status[i]);
			if (!r2lMMIFlag)
			{
				x = (UI_device_width >> 1) - (width >> 1);
			}
			else
			{
				x = (UI_device_width >> 1) - (width >> 1) + width;
			}

		#ifdef __MMI_UI_DALMATIAN_STATUSBAR__
			gui_measure_string(idle_screen_network_status[i], &width, &h2);
			if (PhnsetGetShowDateTimeStatus() != MMI_FALSE)
			{
				y = UI_device_height - (MMI_button_bar_height + (main_LCD_dt_object.time.height) * 3 + (BAR_CONTENT_GAP));
			}
			else
			{
				y = UI_device_height - (MMI_button_bar_height + (h2 + BAR_CONTENT_GAP) + (BAR_CONTENT_GAP));
			}
		#elif defined (__MMI_UI_TECHNO_IDLESCREEN_BAR__)
			y = y + h + PDA_IDLESCREEN_TEXT_GAP;
		/*+ zhouqin 2011-02-16 modify for q6 320X240 */
		#elif defined(__MMI_SYMBIAN_STYLE_IDLE__)
			y = IDLE_NETWORK_GAP +h+ 2;
		/*- zhouqin 2011-02-16 modify for q6 320X240 */
		#else 
			y = MMI_status_bar_height + h + 2;
		#endif 

			y += (h+PDA_IDLESCREEN_TEXT_GAP)*i;// which sim network detail y postion
			
			gui_move_text_cursor(x, y + PDA_IDLESCREEN_TEXT_GAP);

			gui_print_bordered_text(idle_screen_network_status[i]);

			gui_measure_string(idle_screen_network_status[i], &width, &h2);
			h2_Total += (h2+PDA_IDLESCREEN_TEXT_GAP);
		}   /* idle_screen_network_status!=NULL */
	}

#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_layer_pop_clip();
	gdi_layer_blt_previous(extra_text_x1, extra_text_y1-1, extra_text_x2, extra_text_y2+2);
#endif /* __GDI_MEMORY_PROFILE_2__ */ 

	if (GetSATIdleIcon())
	{
		y = MMI_status_bar_height + h + 7 + h2 + 20;
		gui_measure_image(GetSATIdleIcon(), &width, &h);
		x = (UI_device_width >> 1) - (width >> 1);
		gui_push_clip();
		gui_set_clip(x, y, x + width, y + h);
		gui_show_image(x, y, GetSATIdleIcon());
		gui_pop_clip();
	}

}

#else
{
	/*----------------------------------------------------------------*/
	/* Local Variables												  */
	/*----------------------------------------------------------------*/
	S32 x, y, imgWidth, imgHight, strWidth, strHeight,areaHeight;
	S32 w_inset = 5;
	#if defined (__MMI_BLACK_BERRY_IDLE_STYLE__) || defined (__MMI_WIN_STYLE_SUPPORT__)
	E_MTPNP_AD_STATUS status;
	E_MTPNP_AD_SIMCARD_STATUS sim1Status;
	E_MTPNP_AD_SIMCARD_STATUS Sim2Status;
	#endif
	color_t c;
	/*----------------------------------------------------------------*/
	/* Code Body													  */
	/*----------------------------------------------------------------*/
	
	//S32 max_str_h = Get_CharDisplayHeightOfAllLangAndType(idle_screen_network_name_font.size);	
	S32 max_str_h = Get_CharHeightOfAllLang(idle_screen_network_name_font.size) + 2;	
	
	gdi_layer_push_clip();
      if (!IsChineseSet()) //(IsEnglishSet())
	{
		gui_set_font(&MMI_small_font);	
	}
     else
	{
		gui_set_font(&idle_screen_network_name_font);	
	}
	

	c = gui_color(255, 255, 255);
	gui_set_text_color(c);
	c = gui_color(0, 0, 0);
	gui_set_text_border_color(c);
	//gui_set_text_color(*current_MMI_theme->idle_scr_network_name_color);
	//gui_set_text_border_color(*current_MMI_theme->idle_scr_network_name_border_color);
#if defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
	y = SYMBIAN_IDLE_NETWORK_NAME_Y ;
	
	/* clear background */
	gdi_layer_set_clip(
		w_inset, y,
		(UI_device_width - w_inset), (y + max_str_h));
	#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect(
		w_inset, y,
		(UI_device_width - w_inset), (y + max_str_h),
		GDI_COLOR_TRANSPARENT);
	#else
	draw_wallpaper();
	#endif /* __GDI_MEMORY_PROFILE_2__ */
	
	status = MTPNP_AD_Get_Status();
	sim1Status = MTPNP_AD_Get_Card1Status();
	Sim2Status = MTPNP_AD_Get_Card2Status();
#ifdef __FT_CTA_VERSION__	//cta control;
	if (status == MTPNP_AD_CARD1 || status == MTPNP_AD_DUALCARD || status == MTPNP_AD_FLIGHT)
	{
		if(idle_screen_network_name != NULL)
		{
			gui_measure_string(idle_screen_network_name,&strWidth,&strHeight);
			x = (MAIN_LCD_DEVICE_WIDTH - strWidth)>>1;
			if(r2lMMIFlag)
			{
				x = x + strWidth;
			}
			gui_move_text_cursor(x, y);
			gui_print_bordered_text(idle_screen_network_name);
		}	
	}
	else if (status == MTPNP_AD_CARD2)
	{
		
		/*if(g_slave_idle_screen_line1text != NULL)
		{
			gui_measure_string(g_slave_idle_screen_line1text,&strWidth,&strHeight);
			x = (MAIN_LCD_DEVICE_WIDTH - strWidth)>>1;
			if(r2lMMIFlag)
			{
				x = x + strWidth;
			}
			gui_move_text_cursor(x, y);
			gui_print_bordered_text(g_slave_idle_screen_line1text);
		}	*/
	}
#else 	
	if(status==MTPNP_AD_FLIGHT)/*flight mode*/
	{
		if(idle_screen_network_name != NULL)
		{
			gui_measure_string(idle_screen_network_name,&strWidth,&strHeight);
			x = (MAIN_LCD_DEVICE_WIDTH - strWidth)>>1;
			if(r2lMMIFlag)
			{
				x = x + strWidth;
			}
			gui_move_text_cursor(x, y);
			gui_print_bordered_text(idle_screen_network_name);
		}	
	}
	else
	{
		if(sim1Status != MTPNP_AD_SIMCARD_ABSENT &&
			Sim2Status == MTPNP_AD_SIMCARD_ABSENT)/*only sim 1*/
		{
			if(idle_screen_network_name != NULL)
			{
				gui_measure_string(idle_screen_network_name,&strWidth,&strHeight);
				x = (MAIN_LCD_DEVICE_WIDTH - strWidth)>>1;
				if(r2lMMIFlag)
				{
					x = x + strWidth;
				}
				gui_move_text_cursor(x, y);
				gui_print_bordered_text(idle_screen_network_name);
			}	
		}
		else if(sim1Status == MTPNP_AD_SIMCARD_ABSENT &&
			Sim2Status != MTPNP_AD_SIMCARD_ABSENT)/*only sim 2*/
		{
			/*if(g_slave_idle_screen_line1text != NULL)
			{
				gui_measure_string(g_slave_idle_screen_line1text,&strWidth,&strHeight);
				x = (MAIN_LCD_DEVICE_WIDTH - strWidth)>>1;
				if(r2lMMIFlag)
				{
					x = x + strWidth;
				}
				gui_move_text_cursor(x, y);
				gui_print_bordered_text(g_slave_idle_screen_line1text);
			}	*/
		}
		else if(sim1Status != MTPNP_AD_SIMCARD_ABSENT &&
			Sim2Status != MTPNP_AD_SIMCARD_ABSENT)/*sim1 & sim2*/
		{
			S32 strWidth2, strNetWorkGap;
			strNetWorkGap =10;
			
			if(idle_screen_network_name != NULL)
			{
				gui_measure_string(idle_screen_network_name,&strWidth,&strHeight);
			}	
			
			/*if(g_slave_idle_screen_line1text != NULL)
			{
				gui_measure_string(g_slave_idle_screen_line1text,&strWidth2,&strHeight);
			}	*/

			x = (MAIN_LCD_DEVICE_WIDTH - strWidth - strWidth2 -strNetWorkGap)>>1;
			if(r2lMMIFlag)
			{
				//x = x + strWidth +strWidth2 + strNetWorkGap;
				gui_move_text_cursor(x + strWidth +strWidth2 + strNetWorkGap, y);
			//gui_print_bordered_text(g_slave_idle_screen_line1text);

				gui_move_text_cursor(x + strWidth, y);
				gui_print_bordered_text(idle_screen_network_name);
			}
			else
			{
				gui_move_text_cursor(x, y);
				gui_print_bordered_text(idle_screen_network_name);
				gui_move_text_cursor(x + strWidth + strNetWorkGap, y);
				//gui_print_bordered_text(g_slave_idle_screen_line1text);
			}
		}
		else/*no sim card*/
		{
			gui_measure_string((UI_string_type)GetString(STR_SIM_INSERTION_MSG),&strWidth,&strHeight);
			x = (MAIN_LCD_DEVICE_WIDTH - strWidth)>>1;
			if(r2lMMIFlag)
			{
				x = x + strWidth;
			}
			gui_move_text_cursor(x, y);
			gui_print_bordered_text((UI_string_type)GetString(STR_SIM_INSERTION_MSG));	
		}
	}
#endif 
	#elif defined (__MMI_WIN_STYLE_SUPPORT__)
	y = SYMBIAN_IDLE_NETWORK_NAME_Y ;
	x = SYMBIAN_IDLE_NETWORK_NAME_X;
	
	/* clear background */
	gdi_layer_set_clip(
		0, y,
		UI_device_width, 128);
	#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect(
		0, y,
		UI_device_width, 128,
		GDI_COLOR_TRANSPARENT);
	#else
	draw_wallpaper();
	#endif /* __GDI_MEMORY_PROFILE_2__ */
	
	status = MTPNP_AD_Get_Status();
	sim1Status = MTPNP_AD_Get_Card1Status();
	Sim2Status = MTPNP_AD_Get_Card2Status();
	if(status==MTPNP_AD_FLIGHT)/*flight mode*/
	{
		if(idle_screen_network_name != NULL)
		{
			gui_measure_string(idle_screen_network_name,&strWidth,&strHeight);
//			x = (MAIN_LCD_DEVICE_WIDTH - strWidth)>>1;
			if(r2lMMIFlag)
			{
				x = x + strWidth;
			}
			gui_move_text_cursor(x, y);
			gui_print_text(idle_screen_network_name);
		}	
	}
	else
	{
		S32 strWidth2;
		
		if(idle_screen_network_name != NULL)
		{
			gui_measure_string(idle_screen_network_name,&strWidth,&strHeight);
		}	
		
		/*if(g_slave_idle_screen_line1text != NULL)
		{
			gui_measure_string(g_slave_idle_screen_line1text,&strWidth2,&strHeight);
		}	*/

		if(r2lMMIFlag)
		{
			gui_move_text_cursor(x + strWidth, y);
			gui_print_text(idle_screen_network_name);

			x = SYMBIAN_IDLE_SIM2_NETWORK_NAME_X;
			y = SYMBIAN_IDLE_SIM2_NETWORK_NAME_Y;
			
			gui_move_text_cursor(x +strWidth2, y);
			//gui_print_text(g_slave_idle_screen_line1text);

		}
		else
		{
			gui_move_text_cursor(x, y);
			gui_print_text(idle_screen_network_name);

			x = SYMBIAN_IDLE_SIM2_NETWORK_NAME_X;
			y = SYMBIAN_IDLE_SIM2_NETWORK_NAME_Y;
			
			gui_move_text_cursor(x, y);
			//gui_print_text(g_slave_idle_screen_line1text);
		}
	}
	
	#else
	gui_measure_image((PU8)get_image(IMG_IDLE_SYMBIAN_SETTING1), &imgWidth, &imgHight);

	y = SYMBIAN_IDLE_NETWORK_NAME_Y + (max_str_h>>1)-(imgWidth>>1);
	x =	SYMBIAN_IDLE_NETWORK_NAME_X;
	#ifdef __FT_MULTI_LANG__
	areaHeight = (max_str_h<<1)+2;
	#else
	areaHeight = max_str_h;
	#endif
	/* clear background */
	gdi_layer_set_clip(
		w_inset, y,
		(UI_device_width - w_inset), (y + areaHeight));
	#ifdef __GDI_MEMORY_PROFILE_2__
	gdi_draw_solid_rect(
		w_inset, y,
		(UI_device_width - w_inset), (y + areaHeight),
		GDI_COLOR_TRANSPARENT);
	#else
	draw_wallpaper();
	#endif /* __GDI_MEMORY_PROFILE_2__ */

	gui_show_image(x, y, (PU8)get_image(IMG_IDLE_SYMBIAN_SETTING1));
	if(idle_screen_network_name != NULL)
	{
		gui_measure_string(idle_screen_network_name,&strWidth,&strHeight);
		if(r2lMMIFlag)
		{
			x = x + strWidth;
		}
		gui_move_text_cursor(x +imgWidth +1, y);
		gui_print_text(idle_screen_network_name);
	}	
	
	if(mmi_bootup_get_active_flight_mode() != FLIGHTMODE_SILENT) 
	{
		x = SYMBIAN_IDLE_SIM2_NETWORK_NAME_X;
		y = SYMBIAN_IDLE_SIM2_NETWORK_NAME_Y+ (max_str_h>>1)-(imgWidth>>1);
		gui_show_image(x, y, (PU8)get_image(IMG_IDLE_SYMBIAN_SETTING1));
		/*if(g_slave_idle_screen_line1text != NULL)
		{
			gui_measure_string(g_slave_idle_screen_line1text,&strWidth,&strHeight);
			if(r2lMMIFlag)
			{
				x = x + strWidth;
			}
			gui_move_text_cursor(x+imgWidth+1, y);
			gui_print_text(g_slave_idle_screen_line1text);
		}*/
	}
	#endif
	gdi_layer_pop_clip();
}
#endif
void symbian_hide_main_LCD_idle_date_time(void)
{
	U32 x1,y1,x2,y2, x,y;
#if defined (__MMI_WIN_STYLE_SUPPORT__)
	U32 background_x = 220;	
	U32 background_y = 82;
	U32 background_w = 100;	
	U32 background_h = 52;
#else
	U32 background_x = 40;	
	U32 background_y = 25;
	U32 background_w = 278;	
	U32 background_h = 50;
#endif
	x1 = background_x;
	y1 = background_y;
	x2 = background_x+ background_w;
	y2 = background_y + background_h;

	gdi_layer_push_clip();
	gdi_layer_set_clip(x1,y1,x2,y2);
	gdi_draw_solid_rect(x1,y1,x2,y2, GDI_COLOR_TRANSPARENT);
	gdi_layer_pop_clip();
	
    gdi_layer_blt_previous(x1,y1,x2,y2);
	
}

extern stFontAttribute wgui_dialer_box_f1;
void Symbian_idle_screen_show_digital(void)
{
	UI_time dt;
	U8 day,month,week,hour,minute,len,i;
	S8 num;
	U16 year;
	U32 x1,y1,x2,y2, x,y;
	U8 displayAmPm;
	S32 imageWidth, imageHeight;
	U16 weekImgId;
	S32 sw,sh;
	color_t date_color={255,255,255,100};
	
#ifdef __MMI_MAINLCD_240X320__
	U32 background_x = 28;	
	U32 background_y = 18;
	U32 background_w = 170;	
	U32 background_h = 42;
	U32 week_x = 97;
	U32 week_y = 18;
	U32 date_x = 97;
	U32 date_y = 36;
	U32 time_x = 30;
	U32 time_y = 18;
	U8 timeString[12];
	U8 dateString[24]; 
#elif defined (__MMI_MAINLCD_320X240__)

#if defined (__MMI_WIN_STYLE_SUPPORT__)
	U32 background_x = 210;	
	U32 background_y = 82;
	U32 background_w = 110;	
	U32 background_h = 52;
	
	U32 week_x = 92;
	U32 week_y = 46;
	
	U32 date_x = 210;
	U32 date_y = 109;
	U32 time_x = 224;
	U32 time_y = 88;
#else
	U32 background_x = 40;	
	U32 background_y = 25;// 19;
	U32 background_w = 280;	
	U32 background_h = 50;//51;
	U32 week_x = 82;
	U32 week_y = 54;//46;
	U32 date_x = 160;
	U32 date_y = 56;//46;
	U32 time_x = 126;
	U32 time_y = 25;//20;
#endif	
	U8 timeString[12];
	U8 timeString_unicode[24];
	U8 dateString[24]; 
#endif
	GetDateTime(&dt);

	week = dt.DayIndex;
	day = dt.nDay;
	month = dt.nMonth;
	year = dt.nYear;
	hour = dt.nHour;
	minute = dt.nMin;

	x1 = background_x;
	y1 = background_y;
	x2 = background_x+ background_w;
	y2 = background_y + background_h;

	if(PhnsetGetTimeFormat())
	{
	 	displayAmPm =0;
	}
	else
	{
		displayAmPm =1;
	}
	gdi_layer_push_clip();
	gdi_layer_set_clip(x1,y1,x2,y2);
	#if !defined (__MMI_BLACK_BERRY_IDLE_STYLE__) ||defined (__MMI_WIN_STYLE_SUPPORT__)
	gdi_draw_solid_rect(x1,y1,x2,y2, GDI_COLOR_TRANSPARENT);
	#endif
	date_string(&dt, (UI_string_type) dateString, DT_IDLE_SCREEN);
#if defined (__MMI_BLACK_BERRY_IDLE_STYLE__) && !defined (__MMI_WIN_STYLE_SUPPORT__)
//caijianbing modify 20091211
#if !defined(__NOKIA_UI_STYLE_SUPPORT__)//!defined (__NOKIA_FULL_QWERTY_KEY_SUPPORT__) && !defined (__NOKIA_NORMAL_KEY_SUPPORT__)
	//gui_show_image(background_x, background_y, (PU8)get_image(IMG_IDLE_SYMBIAN_TITLE_BACKGROUND));
#endif
	//time_string(&dt, (UI_string_type) timeString, DT_IDLE_SCREEN);
	#ifdef __EPT_LARGE_DIALING_FONTS__
	gui_set_font(&MMI_sublcd_font);	
	#else 
	gui_set_font(&wgui_dialer_box_f1);
	#endif 


	if(displayAmPm ==1)
	{
		if (dt.nHour == 0)
		{
			sprintf((S8*)timeString, "%02d:%02d", 12, dt.nMin);
		}
		else if (dt.nHour < 12)
		{
			sprintf((S8*)timeString, "%02d:%02d", dt.nHour, dt.nMin);

		}
		else if (dt.nHour == 12)
		{
			sprintf((S8*)timeString, "%02d:%02d", dt.nHour, dt.nMin);
		}
		else
		{
			sprintf((S8*)timeString, "%02d:%02d", dt.nHour - 12, dt.nMin);
		}
	}
	else
	{
		sprintf((S8*)timeString,"%02d:%02d",hour,minute);	
	}
	
	//len = strlen((S8*)timeString) * 2;
	mmi_asc_to_ucs2((S8*) timeString_unicode, (S8*) timeString);
	
	x = time_x;
	gui_measure_string((UI_string_type)timeString_unicode,&sw,&sh);
	
	if(r2lMMIFlag)
	{
		gui_move_text_cursor(x+sw, time_y);
	}
	else
	{
		gui_move_text_cursor(x, time_y);
	}

	gui_set_text_color(date_color);
	gui_print_text((UI_string_type)timeString_unicode);
	x += sw +4; 
	if(displayAmPm)
	{
		if(dt.nHour < 12)//(timeString[len -2] == 'A')
		{
			gui_show_image(x, time_y - 10, (PU8)GetImage(IMG_IDLE_SYMBIAN_TIME_AM));
		}
		else
		{
			gui_show_image(x, time_y - 10 , (PU8)GetImage(IMG_IDLE_SYMBIAN_TIME_PM));
		}
		gui_measure_image((PU8)get_image(IMG_IDLE_SYMBIAN_TIME_NUM2), &imageWidth, &imageHeight);	
		week_x = week_x + imageWidth;
		date_x = date_x + imageWidth;
	}

	
#if 0	
	gui_measure_string((UI_string_type)timeString,&sw,&sh);
	if(r2lMMIFlag)
	{
		gui_move_text_cursor(time_x+sw, time_y);
	}
	else
	{
		gui_move_text_cursor(time_x, time_y);
	}
	
	gui_print_text((UI_string_type)timeString);
#endif 

	#else
	if(displayAmPm ==1)
	{
		if (dt.nHour == 0)
		{
			sprintf((S8*)timeString, "%02d:%02d", 12, dt.nMin);
		}
		else if (dt.nHour < 12)
		{
			sprintf((S8*)timeString, "%02d:%02d", dt.nHour, dt.nMin);

		}
		else if (dt.nHour == 12)
		{
			sprintf((S8*)timeString, "%02d:%02d", dt.nHour, dt.nMin);
		}
		else
		{
			sprintf((S8*)timeString, "%02d:%02d", dt.nHour - 12, dt.nMin);
		}
	}
	else
	{
		sprintf((S8*)timeString,"%02d:%02d",hour,minute);	
	}
	
	len = strlen((S8*)timeString);
	
	x = time_x;
	for(i = 0; i < len; i++)
	{
		num = timeString[i];
		if(num == ':')
		{
			gui_show_image(x, time_y, (PU8)GetImage(IMG_IDLE_SYMBIAN_TIME_DIAN));
			gui_measure_image((PU8)get_image(IMG_IDLE_SYMBIAN_TIME_DIAN), &imageWidth, &imageHeight);	
		}
		else if(num >= '0' && num <= '9')
		{
			U16 Offset;
			Offset = (U16)(num - '0');
			gui_show_image(x, time_y, (PU8)GetImage((U16)(IMG_IDLE_SYMBIAN_TIME_NUM0 + Offset)));
			gui_measure_image((PU8)get_image((U16)(IMG_IDLE_SYMBIAN_TIME_NUM0 + Offset)), &imageWidth, &imageHeight);	
		}

		x = x + imageWidth +1;
	}

	if(displayAmPm)
	{
		if(dt.nHour < 12)//(timeString[len -2] == 'A')
		{
			gui_show_image(x, time_y, (PU8)GetImage(IMG_IDLE_SYMBIAN_TIME_AM));
		}
		else
		{
			gui_show_image(x, time_y, (PU8)GetImage(IMG_IDLE_SYMBIAN_TIME_PM));
		}
		gui_measure_image((PU8)get_image(IMG_IDLE_SYMBIAN_TIME_NUM2), &imageWidth, &imageHeight);	
		week_x = week_x + imageWidth;
		date_x =  date_x + imageWidth;
	}
		#endif
	#if defined (__FT_MULTI_LANG__)
	#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
          	gui_set_text_color(*current_MMI_theme->idle_scr_date_color);
        #else 
              gui_set_text_color(date_color);
        #endif 

	gui_set_font(&MMI_medium_font);
	if(r2lMMIFlag)
	{
		gui_move_text_cursor(background_x + background_w - 2, week_y);
	}
	else
	{
		gui_move_text_cursor(week_x, week_y);
	}
	gui_set_text_color(date_color);
	gui_print_text((UI_string_type) GetString((U16)(STR_IDLESCREEN_SUNDAY + week)));
	#else
	if(IsChineseSet())
	{
		weekImgId = (U16)(IMG_IDLESCREEN_SUNDAY + week);
		
	}
	else
	{
		weekImgId = (U16)(IMG_IDLESCREEN_SUNDAY_EN+ week);
	}
	gui_measure_image((PU8)GetImage(weekImgId),&imageWidth,&imageHeight);
	//caijianbing masked 20091112
	#ifdef __MMI_BLACK_BERRY_SUPPORT__
		week_x = (MAIN_LCD_DEVICE_WIDTH>>1) - imageWidth -3;
	#else /*define __MMI_SYMBIAN_STYLE_SUPPORT__*/
		//week_x = (MAIN_LCD_DEVICE_WIDTH>>1) - imageWidth -3;
	#endif 
	gui_show_image(week_x, week_y, (PU8)GetImage(weekImgId));
#endif	

	#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
          	gui_set_text_color(*current_MMI_theme->idle_scr_date_color);
        #else 
              gui_set_text_color(date_color);
        #endif 
	
	gui_set_font(&MMI_medium_font);
	gui_measure_string((UI_string_type)dateString,&sw,&sh);
	
	if(r2lMMIFlag)
	{
//		gui_move_text_cursor(background_x + background_w - 2, date_y);
		gui_move_text_cursor(date_x+sw, date_y);
	}
	else
	{
		gui_move_text_cursor(date_x, date_y);
	}
	gui_set_text_color(date_color);
	gui_print_text((UI_string_type) dateString);
	#if defined(__MMI_BLACK_BERRY_IDLE_STYLE__)||defined(__MMI_SYMBIAN_STYLE_IDLE__ )//caijianbing 20091125
	if(MMI_fixed_matrix_menu.highlighted_item == 6)
	{
		gui_show_image(g_Symbian_idle_info[1].x,
						g_Symbian_idle_info[1].y,
						(PU8)get_image(pda_idle_shortcut[6].pda_Matrix_Icon_ani_ID));
		
	}
	else
	{
		gui_show_image(g_Symbian_idle_info[1].x,
						g_Symbian_idle_info[1].y,
						(PU8)get_image(pda_idle_shortcut[6].pda_Matrix_Icon_ID));
	}
	#endif
	
	gdi_layer_pop_clip();
	gdi_layer_blt_previous(x1,y1,x2,y2);
}

#ifdef __MMI_TOUCH_SCREEN__ 

MMI_BOOL mmi_pda_check_pen_shortcut( mmi_pen_point_struct point, mmi_pen_event_type_enum pen_event, S32* hilight )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, j;
    S32 x   = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if ( pen_event == MMI_PEN_EVENT_DOWN )
    {
        for ( i = 0; i < SYMBIAN_TOTAL_LINES; i++ )/*for each shortcut's coordinate*/
        {
            x = g_Symbian_idle_info[i].x;

            for ( j = 0; j < g_Symbian_idle_info[i].columns; j++ )
            {
                if ( PEN_CHECK_BOUND( point.x
                                    , point.y
                                    , x
                                    , g_Symbian_idle_info[i].y
                                    , g_Symbian_idle_info[i].item_width
                                    , g_Symbian_idle_info[i].item_height ) )
                {
                    *hilight = j + g_Symbian_idle_info[i].prev_columns;
                    return TRUE;
                }

                x += g_Symbian_idle_info[i].item_width;
            }
        }
    }
    else if ( pen_event == MMI_PEN_EVENT_UP )
    {
        for ( i = 0; i < SYMBIAN_TOTAL_LINES; i++ )/*for each shortcut's coordinate*/
        {
            x = g_Symbian_idle_info[i].x;

            for ( j = 0; j < g_Symbian_idle_info[i].columns; j++ )
            {
                if ( PEN_CHECK_BOUND( point.x
                                    , point.y
                                    , x
                                    , g_Symbian_idle_info[i].y
                                    , g_Symbian_idle_info[i].item_width
                                    , g_Symbian_idle_info[i].item_height ) )
                {
                    *hilight = j + g_Symbian_idle_info[i].prev_columns;
                    return TRUE;
                }

                x += g_Symbian_idle_info[i].item_width;
            }
        }
    }
    else if ( pen_event == MMI_PEN_EVENT_MOVE )
    {
        if ( symbian_idle_object == SYMBIAN_IDLE_PEN_LSK )
        {
            return TRUE;
        }
        else if ( symbian_idle_object == SYMBIAN_IDLE_PEN_RSK )
        {
            return TRUE;
        }
    }
    else
    {
        /*do nothing*/
    }

    return FALSE;
}

MMI_BOOL mmi_pda_check_pen_RSK( mmi_pen_point_struct point, mmi_pen_event_type_enum pen_event )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if ( pen_event == MMI_PEN_EVENT_DOWN )
    {
        /*check ponit*/
        if ( PEN_CHECK_BOUND( point.x
                            , point.y
                            , ( UI_device_width - MMI_softkey_width )
                            , ( UI_device_height - MMI_softkey_height )
                            , MMI_softkey_width
                            , MMI_softkey_height ) )
        {
            return TRUE;
        }
    }
    else if ( pen_event == MMI_PEN_EVENT_UP )
    {
        if ( PEN_CHECK_BOUND( point.x
                            , point.y
                            , ( UI_device_width - MMI_softkey_width )
                            , ( UI_device_height - MMI_softkey_height )
                            , MMI_softkey_width
                            , MMI_softkey_height ) )
        {
            return TRUE;
        }
    }
    else if ( pen_event == MMI_PEN_EVENT_MOVE )
    {
        return FALSE;
    }
    else
    {
        return FALSE;
    }
    return FALSE;
}

MMI_BOOL mmi_pda_check_pen_LSK( mmi_pen_point_struct point, mmi_pen_event_type_enum pen_event )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if ( pen_event == MMI_PEN_EVENT_DOWN )
    {
        /*check ponit*/
        if ( PEN_CHECK_BOUND( point.x
                            , point.y
                            , 0
                            , ( UI_device_height - MMI_softkey_height )
                            , MMI_softkey_width
                            , MMI_softkey_height ) )
        {
            return TRUE;
        }
    }
    else if ( pen_event == MMI_PEN_EVENT_UP )
    {
        if ( PEN_CHECK_BOUND( point.x
                            , point.y
                            , 0
                            , ( UI_device_height - MMI_softkey_height )
                            , MMI_softkey_width
                            , MMI_softkey_height ) )
        {
            return TRUE;
        }
    }
    else if ( pen_event == MMI_PEN_EVENT_MOVE )
    {
        return FALSE;
    }
    else
    {
        return FALSE;
    }
    return FALSE;
}

#ifdef __MMI_WGUI_CSK_ENABLE__
MMI_BOOL mmi_pda_check_pen_CSK( mmi_pen_point_struct point, mmi_pen_event_type_enum pen_event )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if ( pen_event == MMI_PEN_EVENT_DOWN )
    {
        /*check ponit*/
        if ( PEN_CHECK_BOUND( point.x
                            , point.y
                            , UI_device_height/3
                            , ( UI_device_height - MMI_softkey_height )
                            , UI_device_height/3
                            , MMI_softkey_height ) )
        {
            return TRUE;
        }
    }
    else if ( pen_event == MMI_PEN_EVENT_UP )
    {
        if ( PEN_CHECK_BOUND( point.x
                            , point.y
                            , UI_device_height/3
                            , ( UI_device_height - MMI_softkey_height )
                            , UI_device_height/3
                            , MMI_softkey_height ) )
        {
            return TRUE;
        }
    }
    else if ( pen_event == MMI_PEN_EVENT_MOVE )
    {
        return FALSE;
    }
    else
    {
        return FALSE;
    }
    return FALSE;
}

#endif

void mmi_pda_pen_down_handler( mmi_pen_point_struct point )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL ret= MMI_FALSE;
    S32 hilight = -1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    ret = mmi_pda_check_pen_shortcut( point, MMI_PEN_EVENT_DOWN, &hilight );
    if ( ret )
    {
      if(gKeyPadLockFlag !=0)
	  	UI_dummy_function();
	else	
	{
       	 	hilit_shortcut_id = hilight;
       	 	Gui_Symbian_goto_N_item( &MMI_fixed_matrix_menu );
        	Gui_Symbian_Idlescreen();
	}
        //...   
        return ;
    }

    if ( ( mmi_pda_check_pen_LSK( point, MMI_PEN_EVENT_DOWN ) ) )
    {
        symbian_idle_object = SYMBIAN_IDLE_PEN_LSK;

        ExecuteCurrKeyHandler( KEY_LSK, KEY_EVENT_DOWN );
    }
    else if ( mmi_pda_check_pen_RSK( point, MMI_PEN_EVENT_DOWN ) )
    {
        symbian_idle_object = SYMBIAN_IDLE_PEN_RSK;

        ExecuteCurrKeyHandler( KEY_RSK, KEY_EVENT_DOWN );
    }
	#ifdef __MMI_WGUI_CSK_ENABLE__
	 else if ( mmi_pda_check_pen_CSK( point, MMI_PEN_EVENT_DOWN ) )
    {
        symbian_idle_object = SYMBIAN_IDLE_PEN_CSK;
		
        //ExecuteCurrKeyHandler( KEY_IP, KEY_EVENT_DOWN );
    }
	#endif
}

void mmi_pda_pen_up_handler( mmi_pen_point_struct point )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL ret= MMI_FALSE;
    S32 hilight = -1;//=hilit_shortcut_id;/*should be the current hilight shortcut ID*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    ret = mmi_pda_check_pen_shortcut( point, MMI_PEN_EVENT_UP, &hilight );
    if ( ret && ( hilight != -1 ) && ( hilit_shortcut_id == hilight ) )
    {
        {
	if(gKeyPadLockFlag !=0)
           UI_dummy_function();
	else	
	   pda_idle_shortcut[hilight].entryfrn();
		
        }
    }
    if ( ( mmi_pda_check_pen_LSK( point, MMI_PEN_EVENT_UP ) ) )
    {
        ExecuteCurrKeyHandler( KEY_LSK, KEY_EVENT_UP );
    }
    else if ( mmi_pda_check_pen_RSK( point, MMI_PEN_EVENT_UP ) )
    {
        ExecuteCurrKeyHandler( KEY_RSK, KEY_EVENT_UP );
    }
	
#ifdef __MMI_WGUI_CSK_ENABLE__
	else if ( mmi_pda_check_pen_CSK( point, MMI_PEN_EVENT_UP ) )
    {
        //ExecuteCurrKeyHandler( KEY_IP, KEY_EVENT_UP );
         IdleScreenDigitHandler();
    }
#endif
    symbian_idle_object = SYMBIAN_IDLE_PEN_NONE;
}


void mmi_pda_pen_move_handler( mmi_pen_point_struct point )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL ret= MMI_FALSE;
    S32 hilight = hilit_shortcut_id;/*should be the current hilight shortcut ID*/
    gui_button_pen_enum button_event;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    ret = mmi_pda_check_pen_shortcut( point, MMI_PEN_EVENT_MOVE, &hilight );
    if ( ret )
    {
        if ( symbian_idle_object == SYMBIAN_IDLE_PEN_LSK )
        {
            translate_softkey_pen_event( MMI_LEFT_SOFTKEY, MMI_PEN_EVENT_MOVE, point.x, point.y, &button_event );
        }
        else if ( symbian_idle_object == SYMBIAN_IDLE_PEN_RSK )
        {
            translate_softkey_pen_event( MMI_RIGHT_SOFTKEY, MMI_PEN_EVENT_MOVE, point.x, point.y, &button_event );
        }
		#ifdef __MMI_WGUI_CSK_ENABLE__
		else if ( symbian_idle_object == SYMBIAN_IDLE_PEN_CSK )
        {
            translate_softkey_pen_event( MMI_CENTER_SOFTKEY, MMI_PEN_EVENT_MOVE, point.x, point.y, &button_event );
        }
		#endif
        return ;
    }

    return ;
}

void mmi_PdaStyleRegisterPenHandle( void )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    mmi_pen_register_down_handler( mmi_pda_pen_down_handler );
    mmi_pen_register_up_handler( mmi_pda_pen_up_handler );
    mmi_pen_register_move_handler( mmi_pda_pen_move_handler );
}

#endif

#endif


