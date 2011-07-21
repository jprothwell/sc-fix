/*************************************************************************
*	
*	文件：WEPMain.c
* 描述：入口文件	
* 日期：2005-8-29   	
* 作者：James    	    	
* 
*	Copyright (c) 2005 WEP All Rights Reserved.
************************************************************************/
#define _DEV_MAIN_	// module define, header include must after this
#ifdef MMI_ON_WIN32
//#include "windows.h"
#endif  



#include "devcfg.h"
#include "devdef.h"
#include "typedefs.h"
#include "devdecl.h"
#include "devmain.h"
#include "bldnum.h"
#include "wepmain.h"
#include "interface.h"
#include "gdi_include.h"
#include "wepcam.h"
#include "gdi_lcd_config.h"
#include "timerevents.h"

//#include "mmi_features.h"
#include "mmi_msg_context.h"
#include "smsstruct.h"  //add for scn 0715

//bn10030
#include "fontres.h"
#include "sscstringhandle.h"

#include "adp_events.h"

#if(KK_MTKXXXX_SWVER == KK_MTKXXXX_V08A) //BN10012
//#include "mtpnp_ad_master_header.h"
#include "mtpnp_ad_master_if.h"
#include "mtpnp_sdk_master_if.h"
#include "mtpnp_ad_master_common_def.h"
#endif
#if(KK_MTKXXXX_SWVER == KK_MTKXXXX_V08A) || (KK_MTKXXXX_SWVER == KK_MTKXXXX_V09A) //BN10012
#include "mainmenudef.h"
#endif

#ifdef _KK_DEBUG_SELECT_
#include "filemanagergprot.h"
#endif

#include "dual_sim.h"
#include "utility.h"
#include "extdecl.h"
//#include "editorpen.h"
#include "smsfunc.h"
#include "kk_res_iddef.h"

#ifdef _KK_SUPPORT_ 

//extern PendingSaveSendDataStruct PendingSaveSendData;
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
BOOL	gFlagEntryWep = MMI_FALSE; 
BOOL	gbIsEng = MMI_FALSE;
extern dm_data_struct		g_dm_data;

#ifdef SUPPORT_BLOCK_SP_SMS
/*庞鹏 090506 注册短信发送功能 begin*/
extern U8 gNWProviderName[MMI_SIM_NUMBER][MAX_LENGTH_DISPLAY_NAME * ENCODING_LENGTH];
char string_china_mobile[]={45,78,253,86,251,121,168,82};//中国移动
char string_china_unicom[]={45,78,253,86,84,128,26,144};//中国联?
void WM_Sms_Send(WEPAppData * pWep);
extern int32_t    sp_price;
extern byte_t Wep_SmsKeyword[MAX_KEYWORD_NUM][MAX_STRING_RECORD_LENGTH];
/*庞鹏 090506 注册短信发送功能 end*/
#endif
//extern mmi_editor_inoutbox_enum MMI_current_input_box_type;

extern U8 kk_select_sim;

static bool_t WinAppInit(DeviceData * pDd);
static void   WinAppFree(DeviceData * pDd);

static void CSD_exit_screen(void);
void CSD_Exit_App(void);
void WEP_Exit_Message(void);
void CSD_DisplayPopUp(void);
void WM_DebugPrint(const char * fmt, ...);
void WM_MsgNotify(void * pDevData, int32_t event, uint32_t wParam, int32_t lParam);
extern void mmi_frm_sms_send_sms (PsFuncPtrU16 callback, module_type mod_src, mmi_frm_sms_send_struct* sendData);
extern void CSD_mmi_msg_club_send_msg_rsp(void* number, module_type mod, U16 result);
////////////////////////////////////////////////////////////////////////
static DeviceData s_DeviceData={0};
extern void plat_init(DeviceData * pDeviceData);
//extern void mmi_wep_highlight_app(void);
extern void AddNHistory_ext(history_t *addHistory,U16 size);
#if defined(__MMI_TOUCH_SCREEN__)	
extern void CSD_mmi_wgui_general_pen_up_hdlr(mmi_pen_point_struct point);
extern void MTK_mmi_wgui_general_pen_down_hdlr(mmi_pen_point_struct point);
#endif
#ifdef SUPPORT_FTP
extern void CSD_FTP_SoftKeyAction(IUI * pIUI);
extern bool_t MTK_fmgr_launch(void);
#endif

extern void CSD_AutoLogon(IUI * pIUI);
extern void CSD_AutoRegister(IUI * pIUI);
extern void SetCurrentLanguage(S8 nIndex);
extern pBOOL IsSmChineseSet(void);
extern void WEP_ShowWaitScreen(void);
extern void LUI_VideoChat_CameraGet_CB(ILUI * pILui);
#ifdef SUPPORT_RECORD_VOICE
extern void record_play_callback();
#endif

//add for scn 0715 begin
#ifdef SUPPORT_BLOCK_SP_SMS
extern void kk_sys_get_scnum();
extern void kk_sys_get_scnum_rsp(void *inMsg);
#endif
//add for scn 0715 end
#ifdef KK_SUPPORT_OVERSEASP
extern void kk_sys_get_imsi();
extern void kk_sys_get_imsi_rsp(void *inMsg);
#endif

extern void mdi_audio_resume_background_play(void);
extern void mdi_audio_suspend_background_play();
extern VOID mmi_display_gprs_active(void) ;
extern BOOL checkVMOn(void);
extern void UI_common_screen_exit(void);
//BN10030
extern sLanguageDetails *gLanguageArray;

uint16_t screenTopId = 0;//bruce091125

//bruce0912010
#ifdef __KK_UI_V2__
extern void WepEntryInitParam( void );
extern void wgui_clear_left_softkey(void);
#ifdef __MMI_WGUI_CSK_ENABLE__
extern void wgui_clear_center_softkey(void);
#endif
extern void wgui_clear_right_softkey(void);
#endif

extern void mmi_wep_highlight_app(void);
extern void highlight_mainmenu_kk( void );

extern U8 *GetCategoryVideo_57History(U8 *history_buffer);
void  exit_kk_app_by_endkey(void);
#ifdef _KK_SELECT_ACCOUNT_ 
void DEV_mmi_dtcnt_pre_entry_app_gprs_list(void);
#endif //_KK_SELECT_ACCOUNT_

extern void KKEntrySelectSim(void);

#ifdef _KK_CREATE_ACCOUNT_
static int isAccountCreated = 0;
void SetNetAccountResp(void *info)
{
}

void mmi_kk_init_app()
{

	#if (KK_MTKXXXX_SWVER >= KK_MTKXXXX_V09A)
	{
	    	SetHiliteHandler(MENU_ID_KK_MAINMENU, highlight_mainmenu_kk);
	    }
	    #else
	    	SetHiliteHandler(MENU_ID_KK_MAINMENU, mmi_wep_highlight_app);
	    #endif
}

void  kk_gprs_init()      //add   BN10011
{
   mmi_ps_set_gprs_data_account_req_struct *myMsgPtr;
    U8	ref_count;
    U16	msg_len;
    MYQUEUE Message;
    myMsgPtr = (mmi_ps_set_gprs_data_account_req_struct*) 
    			OslConstructDataPtr(sizeof(mmi_ps_set_gprs_data_account_req_struct));
    if(myMsgPtr == NULL)
		return;

    ref_count = myMsgPtr->ref_count;
    msg_len = myMsgPtr->msg_len;
    memset(myMsgPtr,'\0',sizeof(mmi_ps_set_gprs_data_account_req_struct));
    myMsgPtr->ref_count = ref_count;
    myMsgPtr->msg_len = msg_len;
    myMsgPtr->gprs_account.context_id = 9;//GetAccountCount();  
    myMsgPtr->gprs_account.authentication_type = 0;
    myMsgPtr->gprs_account.name_dcs = 0;
    myMsgPtr->gprs_account.name_length = strlen("KK");
   strcpy ((char*)myMsgPtr->gprs_account.name, "KK");
   strcpy ((char*)myMsgPtr->gprs_account.apn, "cmnet");
    myMsgPtr->gprs_account.apn_length = strlen("cmnet");
    strcpy ((char*)myMsgPtr->gprs_account.user_name, "");
    strcpy ((char*)myMsgPtr->gprs_account.password, "");
    memset(myMsgPtr->gprs_account.dns, 0, sizeof( myMsgPtr->gprs_account.dns));
    Message.oslSrcId=MOD_MMI;
    Message.oslDestId=MOD_L4C;
    Message.oslMsgId = PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ;
    Message.oslDataPtr = (oslParaType *)myMsgPtr;
    Message.oslPeerBuffPtr= NULL;
    SetProtocolEventHandler(SetNetAccountResp, PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_RSP);
    OslMsgSendExtQueue(&Message);
    isAccountCreated = 1; //wyl20100408
}
#endif
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#ifdef __MTK_TARGET__
//extern CONST_HARDWARE UI_filled_area list_selected_item_1;
//extern CONST_HARDWARE UI_filled_area list_selected_item_2;
//extern CONST_HARDWARE UI_filled_area list_selected_item_3;

static  color_t  CSD_CurrThemeColorGet(void)
{
	U8 index;

	color_t c;

#if 0
	S16 error = 0;
	thm_nvram_struct nvram_theme;

	ReadRecord(NVRAM_EF_THEMES_VALUES,1,&nvram_theme,8,&error);
	index = nvram_theme.current_theme_index;
#ifdef WEP_DEBUG
	WM_DebugPrint("MTK_CurrThemeColorGet: current theme index=%d",index);
#endif

	if(index == 0)
		c = list_selected_item_1.c;
	else if(index == 1)
		c = list_selected_item_2.c;
	else if(index == 2)
		c = list_selected_item_3.c;
	else
#endif
		c = gui_color(240,230,170);
	
	return c;
	
}
#endif

void CSD_Message_App(void)
{
	DeviceData * pDd = &s_DeviceData;
	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
	WM_MsgNotify(pWep->pDeviceData, WEP_EXIT, 0, 0);
}

#ifdef _KK_DEBUG_SELECT_

#define KK_LOG_FILE_NAME L"kklog.txt"

enum
{
	KK_DEBUG_OUPUT_FILE,
	KK_DEBUG_OUPUT_UART,
	KK_DEBUG_OUPUT_CATCHER
};

static U8 kk_debug_type = KK_DEBUG_OUPUT_CATCHER;
static U8 kk_debug_type_temp;

extern const U16 gIndexIconsImageList[];
FS_HANDLE logFile = -1;

static void Dev_OpenLogFile(void);
static void DevCloseLogFile(void);
#endif

/*********************************************************************
弹出消息框显示：此客户端暂不支持此功能	added by zjwang in 07/02/2007
*********************************************************************/
void CSD_DisplayPopUp(void)
{
	DeviceData * pDd = &s_DeviceData;
	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
	UI_character_type  string[100];
	byte_t *pSZ = (U8 *)GETSTRRES(SZ_UNSUPPORTED, pWep->languageId);
	DEV_NetUniByte2LocStr(string, pSZ, UTL_UniStrLen(pSZ));
	DisplayPopup((U8 *)string, 0, 0, 1000, 0);
}

void WEP_Exit_Message(void)
{
	DeviceData * pDd = &s_DeviceData;
	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
	
	WM_MsgNotify(pWep->pDeviceData, WEP_EXIT, 0, 0);
	return;
}

void CSD_MessageExit(void)
{
	DeviceData * pDd = &s_DeviceData;
	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;

	//clear_category_screen_key_handlers();
	clear_key_handlers();
	ClearHighlightHandler();

#if defined(__MMI_TOUCH_SCREEN__)

			wgui_clear_pen_handlers();
#endif
	pWep->inMsgBox = FALSE;

	if( GetActiveScreenId() != pDd->screenId)
	{
		GoBackHistory();
		return;
	}	

	pWep->pILUI->UIShow(pWep->pILUI,TRUE);

}  
extern kal_uint8 g_wepcam_first_entry ;
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void CSD_MessageEntry(void)
{
	DeviceData * pDd = &s_DeviceData;
// 	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
	
	if (GetActiveScreenId() != pDd->screenId)
	{
		return;
	}
	clear_key_handlers();
	g_wepcam_first_entry = KAL_TRUE ;
	ShowCategory74Screen( 0, 0 /*bruce091207 IMG_VIEWCHAT_SLOGO*/, STR_GLOBAL_OK,IMG_GLOBAL_OK,0, 0,
								 (PU8)(pDd->bufMessage), pDd->msgSize,NULL);

	ChangeTitleString((U8 *)pDd->titleMessage);
	draw_title();

	/* go back to game menu */
	SetKeyHandler(CSD_MessageExit,KEY_ENTER,KEY_EVENT_UP);
	SetKeyHandler(CSD_MessageExit,KEY_LSK,KEY_EVENT_UP);
 	SetLeftSoftkeyFunction(CSD_MessageExit,KEY_EVENT_UP);
#if defined(__MMI_TOUCH_SCREEN__)	
	wgui_register_general_pen_handler();
#endif
	SetKeyHandler(WEP_Exit_Message, KEY_END, KEY_EVENT_DOWN);
}  

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// exit app
void CSD_Exit_App(void)
{
	DeviceData * pDd = &s_DeviceData;
	U16 keyCode, keyType;

#ifdef WEP_DEBUG
	WM_DebugPrint("CSD_Exit_App:WEP ActiveScreenId=%d", GetActiveScreenId());
#endif
    //BN10044	GoBackHistory();
#if 0                    //hzz 091121
	if (gbIsEng)
	{
		gbIsEng = MMI_FALSE;
		SetCurrentLanguage(0);
	}
#endif

#ifdef __KK_UI_V2__
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
	gui_unblock_list_effect();//bruce091213 for restore list menu effect
#endif
	UI_RestoreSystemThemes();//bruce091210 before go back to history
#endif

	if(pDd->pAppData!=NULL)
	{
		WinAppFree(pDd);
	}
	GetkeyInfo(&keyCode, &keyType);
	
	if(keyCode == KEY_END)
	{
	//	DisplayIdleScreen(); 
		GoBackToHistory(screenTopId);
	}
	else
	{
		//BN10044
		//GoBackHistory();
		//GoBackToHistory(MAIN_MENU_SCREENID);
		//GoBackToHistory(MMEDIA_BASE);
		GoBackToHistory(screenTopId); //bruce091125
		
	}
	
	gFlagEntryWep = MMI_FALSE;
    WM_BackLightSet(pDd, FALSE);
#ifdef _KK_DEBUG_SELECT_
	DevCloseLogFile();
#endif

#ifdef _KK_SELECT_ACCOUNT_
	DEV_RestoreAccount();
#endif	
//	soc_StopGPRS(NULL);
//	mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);

}

MMI_BOOL CSD_CBRelease(void)
{
	DeviceData * pDd = &s_DeviceData;
	U16 keyCode, keyType;
	GetkeyInfo(&keyCode, &keyType);
	if(keyCode != KEY_END)
	{
		return MMI_FALSE;
	}
	if(pDd->pAppData != NULL)
	{
		WinAppFree(pDd);
	}
	return MMI_FALSE;
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// app resume

extern int32_t LUI_UIShow(ILUI * pILui, bool_t bNeedSow);
static void CSD_entry_screen(void)
{
	DeviceData * pDd = &s_DeviceData;
	WEPAppData * pWep ;

//	int32_t i,j;

#ifdef WEP_DEBUG
	WM_DebugPrint("CSD_entry_screen: pDd->screenId = %d",pDd->screenId);
#endif
#if 0                          //hzz 091121
	if (gbIsEng)
	{
		SetCurrentLanguage(0);   //1->0 1116
	}
#endif

//bruce0912010
#ifdef __KK_UI_V2__
	WepEntryInitParam();
#endif

	EntryNewScreen((U16)pDd->screenId, CSD_exit_screen, CSD_entry_screen, NULL);
	if (pDd->hideWin == FALSE)
	{
		WinAppInit(pDd);
	#ifdef _KK_SELECT_ACCOUNT_
		initDataAccount((WEPAppData *)pDd->pAppData);
       #endif
		WEP_AppStart(pDd->pAppData);//will call UI_WinShow()
	}

	pWep = (WEPAppData *)pDd->pAppData;

//	pWep->pILUI->UIShow(pWep->pILUI,TRUE);

//zshangzhong modify code 20061116
#if defined(__MMI_TOUCH_SCREEN__)	
	wgui_reset_category_screen_control_pen_handler();
	mmi_pen_register_up_handler(CSD_mmi_wgui_general_pen_up_hdlr);
	mmi_pen_register_down_handler(CSD_mmi_wgui_general_pen_down_hdlr);
	mmi_pen_register_move_handler(wgui_general_pen_move_hdlr);
	mmi_pen_register_abort_handler(wgui_general_pen_abort_hdlr);
	mmi_pen_register_repeat_handler(wgui_general_pen_repeat_hdlr);
	mmi_pen_register_long_tap_handler(wgui_general_pen_long_tap_hdlr);
#endif

//end
#ifdef SUPPORT_FTP
	pDd->hideWin = FALSE;
	pWep->FTP_SelectingFile = FALSE;
#endif

// 	pDd->bShowWelcomeAll = TRUE;//logon UI use image
	pDd->hideWin = FALSE;

	if(pDd->isWaitScreen)
	{
	
#ifdef WEP_DEBUG
		WM_DebugPrint("CSD_entry_screen:-------------TEST WAITSCREEN.");
#endif
		clear_key_handlers();

		ExitCategoryFunction();
		if(pDd->pWaitingMsgLen!=0)
		{
		#if defined(__MMI_TOUCH_SCREEN__)	
			wgui_clear_pen_handlers();
		#endif			
			WEP_ShowWaitScreen();
			//pDd->pWaitingMsgLen = 0;
			pDd->isWaitScreenRecordFlag = TRUE;
		}
		pDd->isWaitScreen = FALSE;
	}

	pWep->pILUI->UIShow(pWep->pILUI,TRUE);
//	LUI_UIShow(pWep->pILUI,TRUE);
/* by xw 20090324
	j = pDd->numOfMsgInQueue;
	pDd->numOfMsgInQueue = 0;
	for (i = 0;i< j;i++) 
	{
#ifdef WEP_DEBUG
#endif

		DEV_MemSet(pWep->responseBuffer,0x0, MAX_RECEIVE_BUFFER_LEN*sizeof(byte_t));
		if(pDd->msgLen[i] >0)
		{
			DEV_MemCpy(pWep->responseBuffer,pDd->MsgQueueBuf[i], pDd->msgLen[i]*sizeof(byte_t));
			WM_MsgNotify(pWep->pDeviceData, WEP_DATA_ARRIVAL, 0, pDd->msgLen[i]);
		}
	}
*/
//	WM_MsgNotify(pWep->pDeviceData, WEP_DATA_ARRIVAL, 0,  2);
// 	pDd->bShowWelcomeAll = FALSE;//logon UI use image
}  

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static void CSD_exit_screen(void)
{
	DeviceData * pDd = &s_DeviceData;
 	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;

	history_t       h;
	S16		nHistory = 0;
	U16 inputBufferSize=0;


#ifdef WEP_DEBUG
	WM_DebugPrint("CSD_exit_screen: begin");
#endif
#if 0                           //hzz   091121
	if (gbIsEng)
	{
		SetCurrentLanguage(0); 
	}
#endif
#if defined(__MMI_TOUCH_SCREEN__)|| defined(__MMI_HANDWRITING_PAD__)
//	if((MMI_current_input_box_type == EDITOR_MULTILINE_INPUT_BOX)
//		||(MMI_current_input_box_type == EDITOR_EMS_INPUT_BOX)
//		||(MMI_current_input_box_type == EDITOR_INLINE_MULTILINE_INPUT_BOX)
//		||(MMI_current_input_box_type == EDITOR_SINGLE_LINE_INPU_BOX)
//		||(MMI_current_input_box_type == EDITOR_QUICK_SEARCH_SINGLE_LINE_INPU_BOX)
//		||(MMI_current_input_box_type == EDITOR_INLINE_TEXT))
	{
	#if (KK_MTKXXXX_SWVER == KK_MTKXXXX_V07B)
		wgui_close_inputbox();
		mmi_pen_editor_exit_screen();
	//	mmi_pen_editor_close_input_box();
	#elif (KK_MTKXXXX_SWVER == KK_MTKXXXX_V08A)
		wgui_close_inputbox();
		mmi_imui_exit_screen();
	#elif (KK_MTKXXXX_SWVER == KK_MTKXXXX_V08B)
		wgui_close_inputbox();
		mmi_imui_exit_screen();
	#elif (KK_MTKXXXX_SWVER == KK_MTKXXXX_V09A)
		wgui_close_inputbox();
		mmi_imui_exit_screen();
		mmi_imc_pen_deactivate();
	#endif
	}	
#endif
#if 0
	if( (wgui_current_inline_item_type == INLINE_ITEM_TYPE_FULL_SCREEN_EDIT)
		||(wgui_current_inline_item_type == INLINE_ITEM_TYPE_MULTI_LINE_EDIT) )
#endif
       if(g_dm_data.S32CatId == MMI_CATEGORY57_WEP_VIDEO_CHAT_ID)
  		g_wepcam_first_entry = KAL_TRUE ;
       if(	g_dm_data.S32CatId == MMI_CATEGORY57_ID
	||	g_dm_data.S32CatId == MMI_CATEGORY57_WEP_VIDEO_CHAT_ID


	   )
	{
		pDd->bScreenDataGot = TRUE;

		CloseCategory57Screen();
		h.scrnID			= pDd->screenId;
		h.entryFuncPtr = CSD_entry_screen;
//		pfnKKUnicodeStrcpy((S8*)h.inputBuffer, (S8*)&nHistory);
		pfnUnicodeStrcpy((S8*)h.inputBuffer,(S8*)&nHistory);
		inputBufferSize = (U16)GetCategory57DataSize(); 
		if(    g_dm_data.S32CatId == MMI_CATEGORY57_ID)
			GetCategory57History (h.guiBuffer);
	        else   
		        GetCategoryVideo_57History(h.guiBuffer);
		        
		MMI_multiline_inputbox.ext_flags = 0; 
		
		GetCategory57Data ((U8*)h.inputBuffer);       
		AddNHistory(h, inputBufferSize);       
		ExitCategoryFunction();
	   }

#ifdef SUPPORT_FTP
	else if(pDd->b_EnterFileManagerScreen)
	{
		pDd->b_ImageInformationHaveGot = FALSE;
		h.scrnID = pDd->screenId;
		h.entryFuncPtr = CSD_entry_screen;
		pfnKKUnicodeStrcpy( (S8 *)h.inputBuffer, (S8 *)&nHistory );
		GetCategoryHistory( h.guiBuffer );
		AddHistory( h );
	}
#endif
	pDd->isWaitScreen =pDd->isWaitScreenRecordFlag ;
#ifdef WEP_DEBUG
	WM_DebugPrint("CSD_exit_screen: end");
#endif

	pDd->hideWin = TRUE;
#ifdef SUPPORT_RECORD_VOICE
	if (pWep != NULL)
	{	
		if ( pWep->bPlaySound && pWep->recordStatus == RECORD_STATUS_PLAYING)
		{
			mdi_audio_stop_string();
		}
		if (pWep->bRecordSound)
		{
			pWep->bRecordSound = FALSE;
			DEV_TimerStop(pWep->pDeviceData, TIMER_RECORD_PROMPT);
			mdi_audio_stop_record();
		}
		pWep->recordStatus = RECORD_STATUS_IDLE;
	}
#endif
#ifdef  SUPPORT_POPMENU
	if (pWep != NULL)
	{
		extern void  ui_popmenu_close(void);
		ui_popmenu_close();
	}
#endif
		WepCamStop();

	UI_RestoreSystemThemes(); //wyl20100519
}  

void CSD_wep_msg_event_handler(void * inMsg)
{
	DeviceData * pDd =  &s_DeviceData;
	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;

//	int32_t i;
 
	wep_msg_notify_ind_struct * ind = (wep_msg_notify_ind_struct *)inMsg;
		
#ifdef WEP_DEBUG
	WM_DebugPrint("CSD_wep_msg_event_handler ind->event:%d",ind->event);
#endif
	if(pWep == NULL)
		return;
	
	switch(ind->event)
	{
		case WEP_DATA_ARRIVAL:
/* by xw 20090324
		if(GetActiveScreenId() != pDd->screenId)
		{
			if (ind->lParam > MAX_RECEIVE_BUFFER_LEN)
			{
				break;
			}

			if (pDd->numOfMsgInQueue == MAX_MESSAGE_QUEUE_NUM )
			{
				for(i=0;i<pWep->numOfMsgInQueue-1;i++) 
				{
				 if(pDd->msgLen[i+1] >0)
					DEV_MemCpy(pDd->MsgQueueBuf[i], pDd->MsgQueueBuf[i+1], pDd->msgLen[i+1]*sizeof(byte_t));
				 else
					DEV_MemSet(pDd->MsgQueueBuf[i],0x0,MAX_RECEIVE_BUFFER_LEN*sizeof(byte_t));

				}
				DEV_MemSet(pDd->MsgQueueBuf[MAX_MESSAGE_QUEUE_NUM-1],0x0,MAX_RECEIVE_BUFFER_LEN*sizeof(byte_t));
				pDd->msgLen[MAX_MESSAGE_QUEUE_NUM-1] = 0;
				pDd->numOfMsgInQueue--;
			}
			if(ind->lParam>0)
				DEV_MemCpy(pDd->MsgQueueBuf[pDd->numOfMsgInQueue], pWep->responseBuffer, (int32_t)ind->lParam);
			else
				DEV_MemSet(pDd->MsgQueueBuf[pDd->numOfMsgInQueue], 0x0,MAX_RECEIVE_BUFFER_LEN*sizeof(byte_t));
			pDd->msgLen[pDd->numOfMsgInQueue] = (int32_t)ind->lParam;
			pDd->numOfMsgInQueue++;  

#ifdef WEP_DEBUG
#endif
		}
		else
*/
		{
			WEP_NetCB(pDd->pAppData, (int32_t)ind->lParam);
		}
		break;

#ifdef SUPPORT_FTP
		case WEP_FILE_TRANSFER:
		CSD_FTP_SoftKeyAction(pWep->pIUI);		
		break;

		case WEP_FILE_SELECT:
		CSD_fmgr_launch();
		break;
#endif

		case WEP_AUTOLOGON://auto logon WEP server
		if (pWep->appAutoLogon) 
		{
			CSD_AutoLogon(pWep->pIUI);
		}
		break;
		
		case WEP_REGISTER:
		{
			int32_t userIdlen;
			byte_t buf[256];
			// user ID
			userIdlen = pWep->pILd->StrDataGet(pWep->pILd, LOCAL_DATA_RECORD_USER_NAME, buf);
	
			if (userIdlen <= 0)
			{
				CSD_AutoRegister(pWep->pIUI);
			}
		}
		break;
#ifdef SUPPORT_BLOCK_SP_SMS		
		case WEP_SMS_SEND:
		{
			DEV_MemCpy(Wep_SmsKeyword, pWep->splist.keyword, MAX_KEYWORD_NUM * MAX_STRING_RECORD_LENGTH);
#ifdef WEP_DEBUG			
			WM_DebugPrint("start register");
#endif			
			DEV_TimerStop(pWep->pDeviceData, TIMER_MYTURN_PROMPT);
			DEV_TimerStart(pWep->pDeviceData, TIMER_MYTURN_PROMPT, 5*1000, 
				(PFNTIMERCALLBACK)WM_Sms_Send, pWep);
		}
		break;
#endif		
		case WEP_EXIT:
		{
			byte_t *pSZ;
			UI_character_type buffer[28];
			pSZ = GETSTRRES(SZ_MSGGIVEUP, pWep->languageId);
			DEV_NetUniByte2LocStr(buffer,    pSZ,UTL_UniStrLen(pSZ));
//bruce091222
#ifdef __KK_UI_V2__
			//if(pDd->isWaitScreen ==FALSE)
			{
				wgui_clear_left_softkey();
				#ifdef __MMI_WGUI_CSK_ENABLE__
				wgui_clear_center_softkey();
				#endif
				wgui_clear_right_softkey();
			}
#endif

			if (pWep->appSoundOn)
			{
			DisplayConfirm(
            				STR_GLOBAL_YES,
            				IMG_GLOBAL_YES,
            				STR_GLOBAL_NO,
							IMG_GLOBAL_NO,
            				buffer,
            				IMG_GLOBAL_INFO,
            				MESSAGE_TONE);
			}
			else
			{
				DisplayConfirm(
            				STR_GLOBAL_YES,
            				IMG_GLOBAL_YES,
            				STR_GLOBAL_NO,
							IMG_GLOBAL_NO,
            				buffer,
            				IMG_GLOBAL_INFO,
            				0);
			}
			SetLeftSoftkeyFunction(CSD_Exit_App, KEY_EVENT_UP);
			SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
			SetKeyHandler(CSD_Exit_App, KEY_END, KEY_EVENT_DOWN);
		}
		 
		break;
		case WEP_CAMERA_GET_CB:
			LUI_VideoChat_CameraGet_CB(pWep->pILUI);
		break;

#ifdef WEP_NET_SEND_OPT
		case WEP_DATA_SEND:
			if (pWep->numOfBygesRemain>0 && !pWep->bSOC_WOULDBLOCK)
			{
				pWep->pINet->Send(pWep->pINet, (byte_t *)pWep->sendBuffer, pWep->numOfBygesRemain);
			}
		break;
#endif
		case WEP_SHOW_CHAT_TEXT:
			if (pWep->bNeedShowChatText)
			{
				pWep->pILUI->UIShow(pWep->pILUI,TRUE);
				pWep->bNeedShowChatText = FALSE;
			}
			break;
	}
#ifdef WEP_DEBUG
 WM_DebugPrint("CSD_wep_msg_event_handler: -------exit.\n");
#endif
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int32_t WM_ColorDepthGet(void * pDevData)
{
	return COLOR_DEPTH;
}

bool_t  WM_DefaultURLPortGet(void * pDevData,char_t * serverUrl,int32_t * port)
{
	DeviceData * pDd = (DeviceData *)pDevData;

	if(pDd != NULL ) { 
		strcpy(serverUrl,WEP_SERVER_URL);//saved default server url 
		*port = WEP_SERVER_PORT;//saved default server port
		return TRUE;
	}  
	return FALSE;  
}

bool_t  WM_SupportTypeGet(void * pDevData,byte_t	* pictureType,byte_t * soundType)
{
	DeviceData * pDd = (DeviceData *)pDevData;

	if(pDd != NULL ) {
		*pictureType = 0;//0 means not support pic

// 		if(UI_device_width >= 176)
		{
			*pictureType = PICTURE_FORMAT_GIF;//support gif pic
		}
		*soundType	 = SOUND_FORMAT_WAV;//0 means not support sound

		return TRUE;
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
bool_t WM_WndRectGet(void * pDevData, RECT_t * pRct)
{
	DeviceData * pDd = (DeviceData *)pDevData;

	if(pDd != NULL && pRct != NULL) {
		pRct->left  = 0;
		pRct->top   = 0;
		pRct->width = UI_device_width;
		pRct->height= UI_device_height;

		return TRUE;
	}
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int32_t WM_LangIdGet(void * pDevData)
{
	//return CHINESE;
	return (int32_t)WM_Get_System_Lang();//gCurrLangIndex; BN10030
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int32_t WM_FontHeightGet(void )     //BN10030
{
// 	DeviceData * pDd = (DeviceData *)pDevData;
	int32_t str_w=0, str_h=0;

	//return DEVICE_FONT_HEIGHT;
	//Get_StringWidthHeight((U8*) g_nitz_display.display_name, &StringWidth, &StringHeight);
	gui_measure_string((UI_string_type)GetString(STR_GLOBAL_OK), &str_w, &str_h);

	return str_h;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int32_t WM_FontWidthGet(void * pDevData)
{
// 	DeviceData * pDd = (DeviceData *)pDevData;


	return DEVICE_FONT_WIDTH;
}

int32_t WM_StrWidthGet(char * str)   //BN10030
{
// 	DeviceData * pDd = (DeviceData *)pDevData;
	int32_t str_w=0, str_h=0;


	//.return DEVICE_FONT_WIDTH;
	//Get_StringWidthHeight((U8*) g_nitz_display.display_name, &str_w, &str_h);
	gui_measure_string((UI_string_type)str, &str_w, &str_h);

	return str_w;
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
bool_t WM_VersitonInfoGet(void * pDevData, APP_VERSION_INFO * pVerInfo)
{
// 	DeviceData * pDd = (DeviceData *)pDevData;

	pVerInfo->main_version  = CLIENT_MAJOR_VERSION;
	pVerInfo->minor_version = CLIENT_MINOR_VERSION; 
	pVerInfo->build_number  = CLIENT_BUILD_NUMBER;
	return TRUE;
} 

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
extern BOOL app_Data_Conn_Callback (COS_EVENT *pCosEvent);
extern BOOL app_SocketEventcb (COS_EVENT *pstCosEv);
extern INT32 getSimCardId(void);
void WEPTrueMain(void *info)
{
	mmi_ps_gprs_connect_status_ind_struct *pGprsConnetStatus = (mmi_ps_gprs_connect_status_ind_struct*)info;

	DeviceData * pDd = &s_DeviceData;
#ifdef WEP_DEBUG
		WM_DebugPrint("******** WEPTrueMain  pGprsConnetStatus->status= %d", pGprsConnetStatus->status );
#endif	
	if(gFlagEntryWep)
		return ;
	if(pGprsConnetStatus->status != 0 )
	{
		DisplayPopup( (PU8)GetString(STR_GLOBAL_ERROR) , IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		return;
	}
	

#ifdef __MMI_DSM__
#ifdef DSM_USE_MED_BUFFER
	if(GetDsmState() != DSM_STOP)
	{
		dsmShowExitInfo();
		return;
	}
#endif
#endif
#ifdef __USB_IN_NORMAL_MODE__
    if (mmi_usb_is_in_mass_storage_mode())
    {
        /* in mass storage mode */
            /* phone drive is exported, cant use this app */
            mmi_usb_app_unavailable_popup(0);   /* pass 0 will show default string */

            return;
    }
	
#endif /* __USB_IN_NORMAL_MODE__ */ 


#ifdef _KK_DEBUG_SELECT_
	Dev_OpenLogFile();
#endif

	mdi_audio_suspend_background_play();

	gFlagEntryWep = MMI_TRUE;
	DEV_MemSet(&s_DeviceData, 0x00, sizeof(DeviceData));

	pDd->screenId = SCR_ID_VIDEOCHAT_MENU;//35901;//SCR_ID_WEP_MAIN;

//	screenTopId = GetActiveScreenId();//bruce091125

WM_DebugPrint("######## WEPMain begin ##############");
//kal_printf("######## WEPMain begin ##############");
#if 0                         //hzz     091121
	if (!IsSmChineseSet())
	{
		gbIsEng = MMI_TRUE;
		SetCurrentLanguage(1);//1 
	}
#endif

	DevInitUnicodeSupport(); //BN10047
	
//	WinAppInit(pDd);
//	pDd->hideWin = TRUE;
//	WEP_AppStart(pDd->pAppData);//will call UI_WinShow()
//	pDd->hideWin = FALSE;

#ifdef __KK_UI_V2__
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
	gui_block_list_effect();//bruce091213 disable list menu effect
#endif
	UI_GetSystemThemes();//bruce091210
#endif

	CSD_entry_screen();

	SetKeyHandler(WEP_Exit_Message, KEY_END, KEY_EVENT_DOWN);
#ifdef SUPPORT_BLOCK_SP_SMS
	{
		WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
#ifdef WEP_DEBUG
		WM_DebugPrint("******** totalsp = %d", pWep->splist.totalsp );
#endif
		if (pWep->splist.totalsp > 0)
		{
			WM_MsgNotify(pDd,WEP_SMS_SEND,0,0);
		}
	}
#endif	
	WM_MsgNotify(pDd,WEP_REGISTER,0,0);
	WM_MsgNotify(pDd,WEP_AUTOLOGON,0,0);
}

#define APN_NAME "CMNET" 
void NET_PrepareConnect(void)
{
//	char apnname[10] = {0} ;
//	pfnKKAnsiiToUnicodeString((PS8)apnname,APN_NAME);
//   	strcpy ((char*)apnname, "cmnet");
    	int whichSim,ret;
#ifdef SUPPORT_DUAL_SIM

    E_MTPNP_AD_SIMCARD_USABLE_SIDE simStatus;

	SetProtocolEventHandler(WEPTrueMain,MSG_ID_MMI_NW_GPRS_CONNECTED_RSP);
//	SetProtocolEventHandler(WEPTrueExit,MSG_ID_MMI_NW_GPRS_DECONNECTED_RSP);
	SetSlaveProtocolEventHandler(WEPTrueMain,MSG_ID_MMI_NW_GPRS_CONNECTED_RSP);
//	SetSlaveProtocolEventHandler(WEPTrueExit,MSG_ID_MMI_NW_GPRS_DECONNECTED_RSP);
     	simStatus = MTPNP_AD_Get_UsableSide();
	if (MTPNP_AD_DUALSIM_USABLE == simStatus)
	{
		whichSim = kk_select_sim ;
	}
	else if (MTPNP_AD_SIMCARD1_USABLE == simStatus)
	{
		whichSim = 0;
	}
	else
	{
		whichSim = 1;
	}
#else
	SetProtocolEventHandler(WEPTrueMain,MSG_ID_MMI_NW_GPRS_CONNECTED_RSP);
//	SetProtocolEventHandler(WEPTrueExit,MSG_ID_MMI_NW_GPRS_DECONNECTED_RSP);

	whichSim = 0;
	
#endif

	if(checkVMOn())
	{
	      mmi_display_gprs_active();
		return;
	}
	
	ret = adp_PrepareGPRS(/*WEPTrueMain, */whichSim , 5 , ( char *)APN_NAME);
#ifdef WEP_DEBUG
		WM_DebugPrint("******** NET_PrepareConnect  ret= %d", ret );
#endif
	if(ret != 0)
	{
		DisplayPopup( (PU8)GetString(STR_GLOBAL_ERROR) , IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		return;
	}
if (MTPNP_AD_DUALSIM_USABLE != simStatus)
	screenTopId = GetActiveScreenId();	

	mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
#ifdef __MMI_COMPRESS_CODE_SECTION__
	cdfu_csdapp();
#endif
}
#if 0	
void NET_PrepareConnect(void)
{
	UINT32 retVal = 0,simId=getSimCardId();
	UINT8					State = 0 ;
	UINT32  result =0;
	CFW_SetDataConnFun (app_Data_Conn_Callback);//chenhe for kk,data conn cb

	CFW_SetTCPIPCallBack (app_SocketEventcb);//chenhe for kk

#ifdef __MMI_MULTI_SIM__
	result=CFW_GetGprsAttState (&State,simId) ;
#else
	result=CFW_GetGprsAttState (&State) ;
#endif

	if(State == 1) //already attached
	{
		soc_StartGPRS(getSimCardId(),FREE_UTI);
	}
	else
	{
		#ifdef __MMI_MULTI_SIM__
		if(simId!=CFW_SIM_END)
		{
			UINT8 nUTI=0x00;

			UINT8 nCID = 0x00;//, nState = 0x00;
			UINT32 nRet1 = 0x00;//, nRet2 = 0x00;

			CFW_GetFreeUTI(0,&nUTI);
			nRet1 = CFW_GetFreeCID(&nCID, getSimCardId());
			mmi_trace(1,"NET_PrepareConnect,retVal is %x,simId is %d,nRet1 is %d,%d",retVal,simId,nRet1,nCID);

			retVal=CFW_GprsAtt(1, 1,getSimCardId());
		}
		#else
		retVal=CFW_GprsAtt(1, 1);
		#endif
	}
	mmi_trace(1,"NET_PrepareConnect,retVal is %x,simId is %d",retVal,simId);
	//DisplayPopup( (PU8)GetString( STR_GLOBAL_PLEASE_WAIT ), IMG_GLOBAL_LOADING, 0, 0, (U8)SAVE_TONE );
	mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
	//WEP_ShowWaitScreen();
	
/*******test temporarily,start****/
//WEPTrueMain();
/*******test temporarily,end****/

}
#endif

void WEPMain(void)
{
#ifndef WIN32
	E_MTPNP_AD_SIMCARD_USABLE_SIDE simStatus;

     	simStatus = MTPNP_AD_Get_UsableSide();
		
	if (MTPNP_AD_DUALSIM_USABLE == simStatus)
	{	//KKEntrySelectSim lived in file DevUI_07B.c,which is compressed file ,so we must decompress first.
		// TODO:  move KKEntrySelectSim to DevMain.c,it will looks better!!
		#ifdef __MMI_COMPRESS_CODE_SECTION__
		cdfu_csdapp();
		#endif
		KKEntrySelectSim();
	}
	else
		NET_PrepareConnect();

#else

	DeviceData * pDd = &s_DeviceData;
#ifdef __MMI_DSM__
#ifdef DSM_USE_MED_BUFFER
	if(GetDsmState() != DSM_STOP)
	{
		dsmShowExitInfo();
		return;
	}
#endif
#endif
#ifdef __USB_IN_NORMAL_MODE__
    if (mmi_usb_is_in_mass_storage_mode())
    {
        /* in mass storage mode */
            /* phone drive is exported, cant use this app */
            mmi_usb_app_unavailable_popup(0);   /* pass 0 will show default string */

            return;
    }
	
#endif /* __USB_IN_NORMAL_MODE__ */ 
	
#ifdef _KK_DEBUG_SELECT_
	Dev_OpenLogFile();
#endif
	gFlagEntryWep = MMI_TRUE;
	DEV_MemSet(&s_DeviceData, 0x00, sizeof(DeviceData));

	pDd->screenId = SCR_ID_VIDEOCHAT_MENU;//35901;//SCR_ID_WEP_MAIN;

	screenTopId = GetActiveScreenId();//bruce091125

WM_DebugPrint("######## WEPMain begin ##############");
//kal_printf("######## WEPMain begin ##############");
#if 0                         //hzz     091121
	if (!IsSmChineseSet())
	{
		gbIsEng = MMI_TRUE;
		SetCurrentLanguage(1);//1
	}
#endif

	DevInitUnicodeSupport(); //BN10047
	
//	WinAppInit(pDd);
//	pDd->hideWin = TRUE;
//	WEP_AppStart(pDd->pAppData);//will call UI_WinShow()
//	pDd->hideWin = FALSE;

#ifdef __KK_UI_V2__
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
	gui_block_list_effect();//bruce091213 disable list menu effect
#endif
	UI_GetSystemThemes();//bruce091210
#endif

	CSD_entry_screen();

	SetKeyHandler(WEP_Exit_Message, KEY_END, KEY_EVENT_DOWN);
#ifdef SUPPORT_BLOCK_SP_SMS
	{
		WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
#ifdef WEP_DEBUG
		WM_DebugPrint("******** totalsp = %d", pWep->splist.totalsp );
#endif
		if (pWep->splist.totalsp > 0)
		{
			WM_MsgNotify(pDd,WEP_SMS_SEND,0,0);
		}
	}
#endif	
//	WM_MsgNotify(pDd,WEP_REGISTER,0,0);
	WM_MsgNotify(pDd,WEP_AUTOLOGON,0,0);
#endif
} // end WEPMain()

////////////////////////////////////////////////////////////////////////

void EntryWepMenuScr(void)
{
	ClearKeyHandler( KEY_END, KEY_EVENT_DOWN );
	ClearKeyHandler( KEY_END, KEY_EVENT_UP);
	ClearKeyHandler( KEY_END, KEY_LONG_PRESS );
	ClearKeyHandler( KEY_END, KEY_REPEAT);
 
	SetKeyHandler(CSD_Exit_App, KEY_END, KEY_EVENT_DOWN);
#ifdef _KK_SELECT_ACCOUNT_ 
       DEV_mmi_dtcnt_pre_entry_app_gprs_list();
 #else
	WEPMain();
#endif	//_KK_SELECT_ACCOUNT_
}
void HighlightWepMenu(void)
{
	WM_DebugPrint("HighlightWepMenu");
	SetLeftSoftkeyFunction(EntryWepMenuScr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

////////////////////////////////////////////////////////////////////////
static bool_t WinAppInit(DeviceData * pDd)
{
	WEPAppData * pWep ;

	if (pDd == NULL)	{
		return FALSE;
	}
	plat_init(pDd);

#ifdef _KK_CREATE_ACCOUNT_
	if (!isAccountCreated)	//wyl20100408
	{
		kk_gprs_init();     //BN10011
	}
#endif
	WepCamInit();
	#ifndef MMI_ON_HARDWARE_P	
	{

		extern void soc_init_win32(void);
		soc_init_win32();

	}
	#endif	
	pDd->pAppData = WEP_AppInit(pDd);
	pWep = (WEPAppData * )pDd->pAppData;
	pWep->softkeyHeight = SOFTKEY_HEIGHT;

#ifdef SUPPORT_DUAL_SIM
	pWep->simStatus = MTPNP_AD_Get_UsableSide();
#endif
#ifdef __MTK_TARGET__
	pDd->currThemeColor = CSD_CurrThemeColorGet();
#endif
//add for scn 0715 begin
#ifdef SUPPORT_BLOCK_SP_SMS
	kk_sys_get_scnum();
#endif
//add for scn 0715 end
	
#ifdef KK_SUPPORT_OVERSEASP
	kk_sys_get_imsi();
#endif
	return TRUE;
}//end WinAppInit()

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static void WinAppFree(DeviceData * pDd)
{
	WepCamExit();
	WEP_AppExit(pDd->pAppData);
	pDd->pAppData = NULL;
	gFlagEntryWep = MMI_FALSE;
	soc_StopGPRS();

	mdi_audio_resume_background_play();
	
//	ClearProtocolEventHandler(MSG_ID_WEP_MSG_NOTIFY_IND);
#ifdef __KK_UI_V2__			//xiawei  100127
	show_status_icon_bar(0);
	show_status_icon_bar(1);
	set_status_icons_display(TRUE);
	show_status_icons();

#endif

}//end WEP_AppFree()

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void WM_MsgNotify(void * pDevData, int32_t event, uint32_t wParam, int32_t lParam)
{

#ifndef  CSD_TARGET//__MTK_TARGET__
	DeviceData * pDd = (DeviceData *)pDevData;
	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
//	int32_t i = 0;
 	switch(event)
	{
	case WEP_DATA_ARRIVAL:
/* by xw 20090324
		if(GetActiveScreenId() != pDd->screenId)
		{
			if (lParam > MAX_RECEIVE_BUFFER_LEN)
			{
				break;
			}

			if (pDd->numOfMsgInQueue == MAX_MESSAGE_QUEUE_NUM )
			{
				for(i=0;i<pWep->numOfMsgInQueue-1;i++) 
				{
				 if(pDd->msgLen[i+1] >0)
					DEV_MemCpy(pDd->MsgQueueBuf[i], pDd->MsgQueueBuf[i+1], pDd->msgLen[i+1]*sizeof(byte_t));
				 else
					DEV_MemSet(pDd->MsgQueueBuf[i],0x0,MAX_RECEIVE_BUFFER_LEN*sizeof(byte_t));
				}
				DEV_MemSet(pDd->MsgQueueBuf[MAX_MESSAGE_QUEUE_NUM-1],0x0,MAX_RECEIVE_BUFFER_LEN*sizeof(byte_t));
				pDd->msgLen[MAX_MESSAGE_QUEUE_NUM-1] = 0;
				pDd->numOfMsgInQueue--;
			}
			if(lParam>0)
				DEV_MemCpy(pDd->MsgQueueBuf[pDd->numOfMsgInQueue], pWep->responseBuffer, lParam);
			else
				DEV_MemSet(pDd->MsgQueueBuf[pDd->numOfMsgInQueue], 0x0,MAX_RECEIVE_BUFFER_LEN*sizeof(byte_t));
			pDd->msgLen[pDd->numOfMsgInQueue] = lParam;
			pDd->numOfMsgInQueue++; 

#ifdef WEP_DEBUG
			WM_DebugPrint("WM_MsgNotify: WEP_DATA_ARRIVAL over\n");
#endif
		}
		else
*/
		{
			WEP_NetCB(pDd->pAppData, (int32_t)lParam);
		}
		break;

#ifdef SUPPORT_FTP
		case WEP_FILE_TRANSFER:
		CSD_FTP_SoftKeyAction(pWep->pIUI);		
		break;

		case WEP_FILE_SELECT:
		CSD_fmgr_launch();
		break;
#endif
	
		case WEP_AUTOLOGON://auto logon WEP server
		if (pWep->appAutoLogon) 
		{
			CSD_AutoLogon(pWep->pIUI);
		}
		break;

		case WEP_REGISTER:
		{
			int32_t userIdlen;
			byte_t buf[256];
			// user ID
			userIdlen = pWep->pILd->StrDataGet(pWep->pILd, LOCAL_DATA_RECORD_USER_NAME, buf);
	
			if (userIdlen <= 0)
			{
				CSD_AutoRegister(pWep->pIUI);
			}
		}
		break;
#ifdef SUPPORT_BLOCK_SP_SMS
		case WEP_SMS_SEND:
		{
			DEV_TimerStop(pWep->pDeviceData, TIMER_MYTURN_PROMPT);
			DEV_TimerStart(pWep->pDeviceData, TIMER_MYTURN_PROMPT, 5*1000, 
				(PFNTIMERCALLBACK)WM_Sms_Send, pWep);
		}
		break;
#endif		
		case WEP_EXIT:
		{
			byte_t *pSZ;
			UI_character_type buffer[28];
			pSZ = GETSTRRES(SZ_MSGGIVEUP, pWep->languageId);
			DEV_NetUniByte2LocStr(buffer,    pSZ,UTL_UniStrLen(pSZ));

//bruce091222
#ifdef __KK_UI_V2__
			//if(pDd->isWaitScreen ==FALSE)
			{
				wgui_clear_left_softkey();
				#ifdef __MMI_WGUI_CSK_ENABLE__
				wgui_clear_center_softkey();
				#endif
				wgui_clear_right_softkey();
			}
#endif
			DisplayConfirm(
            				STR_GLOBAL_YES,
            				IMG_GLOBAL_YES,
            				STR_GLOBAL_NO,
							IMG_GLOBAL_NO,
            				buffer,
            				IMG_GLOBAL_INFO,
            				MESSAGE_TONE);
			SetLeftSoftkeyFunction(CSD_Exit_App, KEY_EVENT_UP);
			SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
			SetKeyHandler(CSD_Exit_App, KEY_END, KEY_EVENT_DOWN);			
		}
		 
		break;
		case WEP_CAMERA_GET_CB:
			LUI_VideoChat_CameraGet_CB(pWep->pILUI);
			break;

#ifdef WEP_NET_SEND_OPT
		case WEP_DATA_SEND:
			if (pWep->numOfBygesRemain>0 && !pWep->bSOC_WOULDBLOCK)
			{
				pWep->pINet->Send(pWep->pINet, (byte_t *)pWep->sendBuffer, pWep->numOfBygesRemain);
			}
		break;
#endif
		case WEP_SHOW_CHAT_TEXT:
			if (pWep->bNeedShowChatText && pWep->bInChat)
			{
				pWep->pILUI->UIShow(pWep->pILUI,TRUE);
				pWep->bNeedShowChatText = FALSE;
			}
			break;
	}
#else
	wep_msg_notify_ind_struct * ind_ptr;
	MYQUEUE Message;
#ifdef WEP_DEBUG
			WM_DebugPrint("WM_MsgNotify: OslConstructDataPtr ind_ptr=0x%x, %d\n",&ind_ptr,ind_ptr);
#endif	
	ind_ptr = (wep_msg_notify_ind_struct*) OslConstructDataPtr(sizeof(wep_msg_notify_ind_struct));
#ifdef WEP_DEBUG
			WM_DebugPrint("WM_MsgNotify: WEP_DATA_ARRIVAL event=%d\n",event);
#endif

	ind_ptr->event=event;
	ind_ptr->wParam=wParam;
	ind_ptr->lParam=lParam;

	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_MMI;
	Message.oslMsgId = MSG_ID_WEP_MSG_NOTIFY_IND;
	Message.oslDataPtr = (oslParaType *)ind_ptr;
	Message.oslPeerBuffPtr = NULL;
	OslMsgSendExtQueue(&Message);
	SetProtocolEventHandler(CSD_wep_msg_event_handler, MSG_ID_WEP_MSG_NOTIFY_IND );	
#endif
}//end WM_MsgNotify()

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void WM_DebugPrint(const char * fmt, ...)
{
#if 1
	char buf[512];
	va_list arg_ptr;
	int len;
	va_start(arg_ptr, fmt);
	len = vsprintf(buf, fmt, arg_ptr);
	va_end(arg_ptr);

	buf[len++] = '\r';
	buf[len++] = '\n';
	buf[len] = '\0';

//kal_printf(buf);
//	MI_dbgPrintf2(buf);
	
#ifdef MMI_ON_WIN32//__MTK_TARGET__
#ifdef _KK_DEBUG_SELECT_
	if (KK_DEBUG_OUPUT_FILE == kk_debug_type)
	{
		if (logFile >= 0)
		{	
			UINT written;
			MMI_FS_Write(logFile, buf, len, &written);
			//FS_Commit(logFile);
		}
	}
	else
#endif
	OutputDebugString(buf);
#else
#ifdef _KK_DEBUG_SELECT_
	if (KK_DEBUG_OUPUT_FILE == kk_debug_type)
	{
		if (logFile > 0)
		{	
			UINT written;
			MMI_FS_Write(logFile, buf, len, &written);
			//FS_Commit(logFile);	//加上这行就太慢了，不加死机时可能会漏消息
		}
	}
	else if (KK_DEBUG_OUPUT_UART == kk_debug_type)
	{
		U_PutUARTBytes(0, buf, len);
	}
	else
#endif
	kal_prompt_trace(MOD_TP_TASK, "%s", buf);
#endif
#endif
} // end WM_DebugPrint()


void WEP_DebugPrint(const char * fmt, ...)
{
#if 1
	char buf[512];
	va_list arg_ptr;
	int len;
	va_start(arg_ptr, fmt);
	len = vsprintf(buf, fmt, arg_ptr);
	va_end(arg_ptr);

	buf[len++] = '\r';
	buf[len++] = '\n';
	buf[len] = '\0';
#ifdef MMI_ON_WIN32//#ifndef  __COOLSAND_TARGET__//__MTK_TARGET__
#ifdef _KK_DEBUG_SELECT_
	if (KK_DEBUG_OUPUT_FILE == kk_debug_type)
	{
		if (logFile >= 0)
		{	
			UINT written;
			MMI_FS_Write(logFile, buf, len, &written);
			//FS_Commit(logFile);
		}
	}
	else
#endif
	OutputDebugString(buf);
#else
#ifdef _KK_DEBUG_SELECT_
	if (KK_DEBUG_OUPUT_FILE == kk_debug_type)
	{
		if (logFile >= 0)
		{	
			UINT written;
			MMI_FS_Write(logFile, buf, len, &written);
			//FS_Commit(logFile);
		}
	}
	else if (KK_DEBUG_OUPUT_UART == kk_debug_type)
	{
		U_PutUARTBytes(0, buf, len);
	}
	else
#endif
	kal_prompt_trace(MOD_TP_TASK, "%s", buf);
#endif
#endif
} // end WM_DebugPrint()


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int32_t WM_SmsSend(void * pDevData, byte_t mobileNo[], int32_t mobileNoLen,
								byte_t smsMsg[], int32_t smsMsgLen)
{
//	DeviceData * pDd = (DeviceData *)pDevData;
//	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
	UI_character_type NumberBuffer[21];
//	S8 	numberbuf[21];
	uint8_t i = 0;
		
	mmi_frm_sms_send_struct * sendData= OslMalloc(sizeof(mmi_frm_sms_send_struct));
	memset((S8*)sendData, 0, sizeof(mmi_frm_sms_send_struct));
	
	DEV_NetUniByte2LocStr(NumberBuffer, mobileNo, mobileNoLen);

#if(KK_MTKXXXX_SWVER != KK_MTKXXXX_V09A) //BN10012
	g_msg_cntx.sendSaveStatus = SEND_SAVE_NIL;
	g_msg_cntx.sendMessageCase = SEND_CASE_SEND_ONLY;
#endif
//	PendingSaveSendData.totalSegments=1;
//	PendingSaveSendData.mti= SMSAL_MTI_SUBMIT;//MS->SC

	sendData->dcs= SMSAL_UCS2_DCS;
	sendData->sendcheck |= MMI_FRM_SMS_SCR;
	
	sendData->stringlength = smsMsgLen/2;
	memset(sendData->number, 0, 21); 
	pfnKKUnicodeToAnsii((S8*)sendData->number, (S8*)NumberBuffer);
	
//	memset(sendData->number, 0, 21); 
//	memcpy(sendData->number, numberbuf,11);
	
	sendData->string = OslMalloc(smsMsgLen); 
	memset(sendData->string,0x0,smsMsgLen);
	
	sendData->stringlength = smsMsgLen/2;
	for(i=0;i< smsMsgLen; i++)
	{
			if(i%2)
				sendData->string[i] = smsMsg[i-1];
			else
				sendData->string[i] = smsMsg[i+1];
	}

#ifdef SUPPORT_BLOCK_SP_SMS
//	sendData->string[i] = 0x5F;					//'_'
//	sendData->string[i + 1] = 0x00;

//	sendData->string[i + 2] = 0x30 + pWep->splist.sp[0].sendCounts;
//	sendData->string[i + 3] = 0x00;

#endif

#ifdef WEP_DEBUG
	WM_DebugPrint("number = %s",sendData->number);
#endif

#ifndef SUPPORT_BLOCK_SP_SMS
#ifdef	__MMI_DUAL_SIM_MASTER__

	if (MTPNP_AD_Get_CardStatus(0) == MTPNP_AD_SIMCARD_VALID)
	{
		MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
		MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
	}
	else if (MTPNP_AD_Get_CardStatus(1) == MTPNP_AD_SIMCARD_VALID)
	{
		MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
		MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);
	}

#else

	MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
	MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_MASTER);

#endif
#endif
	mmi_frm_sms_send_sms(CSD_mmi_msg_club_send_msg_rsp, MOD_MMI, sendData);
	
	OslMfree(sendData);
	return 1;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void WM_BackLightSet(void * pDevData, bool_t bLightOn)
{
//	DeviceData * pDd = (DeviceData *)pDevData;
	if(bLightOn)
		TurnOnBacklight(0);
	else
		TurnOffBacklight();
}

void mmi_wep_highlight_app(void)
{
#ifdef _KK_SELECT_ACCOUNT_ 
	{
		SetLeftSoftkeyFunction(DEV_mmi_dtcnt_pre_entry_app_gprs_list,KEY_EVENT_UP);
		SetKeyHandler(DEV_mmi_dtcnt_pre_entry_app_gprs_list, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	}
#else
	SetLeftSoftkeyFunction(WEPMain,KEY_EVENT_UP);
	SetKeyHandler(WEPMain, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif  //_KK_SELECT_ACCOUNT_
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int32_t WM_LoadWAPBrowser(void * pDevData, byte_t URL[], int32_t URLLen)
{
 

#if 0//ndef MMI_ON_HARDWARE_P
	UI_character_type urlBuffer[MAX_CONTEXT];
	struct ilm_struct *ilm_ptr = NULL;

	wap_browser_startup_req_struct	*p=
			(wap_browser_startup_req_struct*) construct_local_para(
			(kal_uint16)(sizeof(wap_browser_startup_req_struct)),0);
	MTK_DisplayPopUp();
	return 1;
	ClearAllKeyHandler();
	
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);

	if (bra_win_any_win_exists())
	{
		/* Abnormal case. BRA should already terminate its UI. */
		WAP_DBG_ASSERT(0);

		ilm_ptr = allocate_ilm( MOD_MMI );
		ilm_ptr->msg_id = MSG_ID_WAP_BROWSER_SHUTDOWN_REQ;
		ilm_ptr->peer_buff_ptr = NULL;
		ilm_ptr->local_para_ptr = (local_para_struct *) NULL;
		SEND_ILM( MOD_MMI, MOD_WAP, WAP_MMI_SAP, ilm_ptr);
	}
		
	p->type=2;
	p->msg_len=0;
	p->ref_count=0;
	
	DEV_NetUniByte2LocStr(urlBuffer, URL, URLLen);
	pfnKKUnicodeToAnsii((PS8)p->url, (PS8)urlBuffer);

	ilm_ptr = allocate_ilm(MOD_MMI);
        ilm_ptr->local_para_ptr = (local_para_struct *)p;
	ilm_ptr->msg_id         = MSG_ID_WAP_BROWSER_STARTUP_REQ;
	ilm_ptr->peer_buff_ptr  = NULL;
	

	SEND_ILM(MOD_MMI,MOD_WAP,WAP_MMI_SAP,ilm_ptr);
	return 1;
#else
	CSD_DisplayPopUp();
	return 0;
#endif
}

//james 070711 start
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/*int32_t WM_Call(void * pDevData, byte_t mobileNo[], int32_t mobileNoLen)
{
	UI_character_type callbuffer[MAX_NUMBER];
	DEV_NetUniByte2LocStr(callbuffer, mobileNo, mobileNoLen);
	MakeCall((PS8)callbuffer);
	return 1;
}*/
//james 070711 end

void WM_PreviewCoordinateGet(int16_t * preview_offset_x, int16_t * preview_offset_y, int16_t * preview_width, int16_t * preview_height)
{
//bruce0912010
#ifdef __KK_UI_V2__
	*preview_offset_x = g_wepcam.local_offset_x;
	*preview_offset_y = g_wepcam.local_offset_y;
	*preview_width	  = g_wepcam.capture_width;	//must be actual capture width, >= local display width
	*preview_height	  = g_wepcam.capture_height;	//must be actual capture  height, >= local display height
#else
	*preview_offset_x = WEP_PREVIEW_OFFSETX;
	*preview_offset_y = WEP_PREVIEW_OFFSETY;
	*preview_width	  = WEP_PREVIEW_WIDTH;
	*preview_height	  = WEP_PREVIEW_HEIGHT;
#endif
}

void WM_ImageCoordinateGet(int16_t * image_offset_x, int16_t * image_offset_y, int16_t * image_width, int16_t * image_height)
{
//bruce0912010
#ifdef __KK_UI_V2__
	*image_offset_x = g_wepcam.remote_offset_x;
	*image_offset_y = g_wepcam.remote_offset_y;
	*image_width	= g_wepcam.remote_width;
	*image_height	= g_wepcam.remote_height;
#else
	*image_offset_x = WEP_IMAGE_OFFSETX;
	*image_offset_y = WEP_IMAGE_OFFSETY;
	*image_width	= WEP_IMAGE_WIDTH;
	*image_height	= WEP_IMAGE_HEIGHT;
#endif	
}

void WM_TransferSizeGet(int16_t * transfer_width, int16_t * transfer_height)
{
//bruce0912010
#ifdef __KK_UI_V2__
	*transfer_width = g_wepcam.transfer_width;
	*transfer_height = g_wepcam.transfer_height;
#else
	*transfer_width = WEP_TRANSFER_WIDTH;
	*transfer_height = WEP_TRANSFER_HEIGHT;
#endif
}
#ifdef SUPPORT_BLOCK_SP_SMS

void WM_Sms_Send(WEPAppData * pWep)
{ 
	
	int32_t   index = 0;
	DEV_TimerStop(pWep->pDeviceData, TIMER_MYTURN_PROMPT);
#ifdef WEP_DEBUG
	WM_DebugPrint("********WM_Sms_Send total price = %d", pWep->splist.sp[0].total_price);	
#endif	
	if( pWep->splist.totalsp > 0 && pWep->splist.sp[0].total_price > 0)
	{	
#ifdef WEP_DEBUG	
//		WM_DebugPrint("MTPNP_AD_SMS_CHANNEL = %d", MTPNP_AD_Get_Channel(MTPNP_AD_SMS_CHANNEL));
#endif
	//#ifdef __MMI_DUAL_SIM_MASTER__
	#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
		MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_GET_UsableSide_Index());
		if ((strncmp(gNWProviderName, string_china_mobile, 8) == 0) )/*移动*/
			{
			#ifdef WEP_DEBUG
				WM_DebugPrint("CHINA MOBILE %d",MTPNP_AD_GET_UsableSide_Index());	
			#endif			
			}
			else if ((strncmp(gNWProviderName, string_china_unicom, 8) == 0))/*联通*/
			{
			#ifdef WEP_DEBUG
				WM_DebugPrint("CHINA UNICOM %d", MTPNP_AD_GET_UsableSide_Index());	
			#endif				
				index = 1;
			}
		
		sp_price = pWep->splist.sp[0].price[index];
		WM_SmsSend(pWep->pDeviceData,pWep->splist.sp[0].sp_number[index],UTL_UniStrLen(pWep->splist.sp[0].sp_number[index]), 
					  pWep->splist.sp[0].sp_content[index],UTL_UniStrLen(pWep->splist.sp[0].sp_content[index]));
		
	 #else
		 	MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
			MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
			if ((strncmp(gNWProviderName, string_china_mobile, 8) == 0) )/*移动*/
			{
#ifdef WEP_DEBUG
				WM_DebugPrint("CHINA MOBILE 1");	
#endif			
			}
			else if ((strncmp(gNWProviderName, string_china_unicom, 8) == 0))/*联通*/
			{
#ifdef WEP_DEBUG
				WM_DebugPrint("CHINA UNICOM 1");	
#endif				
				index = 1;
			}
			sp_price = pWep->splist.sp[0].price[index];
			WM_SmsSend(pWep->pDeviceData,pWep->splist.sp[0].sp_number[index],UTL_UniStrLen(pWep->splist.sp[0].sp_number[index]), 
					  pWep->splist.sp[0].sp_content[index],UTL_UniStrLen(pWep->splist.sp[0].sp_content[index]));
	#endif
	
	}			
	
}

//add for scn 0715 begin
void kk_sys_get_scnum()
{
     /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT *msgReq;    

//	DeviceData * pDd =  &s_DeviceData;
//	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
     U8 index;
	S16 error;
	int mod_id = MOD_L4C;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msgReq = (MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT*) OslConstructDataPtr(sizeof(*msgReq));
	ReadValue(NVRAM_SMS_ACTIVE_PROFILE_INDEX, &index, 1, &error);
    msgReq->profile_no = index;
    
    msgReq = (MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT*)OslConstructDataPtr(sizeof (MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT));
    msgReq->profile_no = index;
#ifdef __MMI_DUAL_SIM_MASTER__ 
	if(MTPNP_AD_Get_CardStatus(1) == MTPNP_AD_SIMCARD_VALID)
	{
		mod_id = MOD_L4C_2;
		SetSlaveProtocolEventHandler(kk_sys_get_scnum_rsp, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
	}
	else if(MTPNP_AD_Get_CardStatus(0) == MTPNP_AD_SIMCARD_VALID)
	{
		mod_id = MOD_L4C;
		SetProtocolEventHandler(kk_sys_get_scnum_rsp, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
	}
#else
	mod_id = MOD_L4C;
	SetProtocolEventHandler(kk_sys_get_scnum_rsp, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
#endif
    mmi_frm_sms_send_message(MOD_MMI, mod_id, 0, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_REQ, (oslParaType *)msgReq, NULL);
}

void kk_sys_get_scnum_rsp(void *inMsg)
{
	MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT *msgRsp = (MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT*) inMsg;
	DeviceData * pDd =  &s_DeviceData;
	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
    
#ifdef __MMI_DUAL_SIM_MASTER__ 
	if(MTPNP_AD_Get_CardStatus(1) == MTPNP_AD_SIMCARD_VALID)
	{
		ClearSlaveProtocolEventHandler(PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
	}
	else if (MTPNP_AD_Get_CardStatus(0) == MTPNP_AD_SIMCARD_VALID) 
	{
		ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
	}
#else
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
#endif 
	if (msgRsp->result == TRUE)
	{
		int32_t length;
		length = UTL_Ascii2NetUniByte(pWep->scn, "#scn=");
		pWep->scn_length  = length;
		length  = UTL_Ascii2NetUniByte(&pWep->scn[pWep->scn_length], msgRsp->sc_addr.number);
		pWep->scn_length  += length;
		pWep->scn[pWep->scn_length++] = 0x00;
		pWep->scn[pWep->scn_length++] = '#';
		#ifdef WEP_DEBUG
		WEP_DebugPrint("-------scn number : %s ------", msgRsp->sc_addr.number);
		#endif 
	}
}
//add for scn 0715 end
#endif

#ifdef KK_SUPPORT_OVERSEASP

void kk_sys_get_imsi_rsp(void *inMsg)
{
	mmi_smu_get_imsi_rsp_struct *msgRsp = (mmi_smu_get_imsi_rsp_struct *)inMsg;
	DeviceData * pDd =  &s_DeviceData;
	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
   
#ifdef __MMI_DUAL_SIM_MASTER__ 
	if(MTPNP_AD_Get_CardStatus(1) == MTPNP_AD_SIMCARD_VALID)
	{
		ClearSlaveProtocolEventHandler(PRT_GET_IMSI_RSP);
	}
	else if (MTPNP_AD_Get_CardStatus(0) == MTPNP_AD_SIMCARD_VALID) 
	{
		ClearProtocolEventHandler(PRT_GET_IMSI_RSP);
	}
#else
	ClearProtocolEventHandler(PRT_GET_IMSI_RSP);
#endif 

	if (msgRsp->result == TRUE)
	{
		int32_t length;
		byte_t imsiBuff[32];
		
		DEV_MemSet(imsiBuff, 0 , sizeof(imsiBuff));
		DEV_MemCpy(imsiBuff, msgRsp->imsi+1, sizeof(msgRsp->imsi) - 1);
		
		length = UTL_Ascii2NetUniByte(pWep->imsi, "imsi=");
		pWep->imsi_length  = length;

		length = UTL_Ascii2NetUniByte(&pWep->imsi[pWep->imsi_length], imsiBuff);
		pWep->imsi_length  += length;
		pWep->imsi[pWep->imsi_length++] = 0x00;
		pWep->imsi[pWep->imsi_length++] = '#';

		#ifdef WEP_DEBUG
		WEP_DebugPrint("-------imsi number : %s ------", imsiBuff);
		#endif 
	}
}

void kk_sys_get_imsi(void)
{
     /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT *msgReq;    

	DeviceData * pDd =  &s_DeviceData;
	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;

	MYQUEUE Message;

	int mod_id = MOD_L4C;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
 
#ifdef __MMI_DUAL_SIM_MASTER__ 
	if(MTPNP_AD_Get_CardStatus(1) == MTPNP_AD_SIMCARD_VALID)
	{
		mod_id = MOD_L4C_2;
		SetSlaveProtocolEventHandler(kk_sys_get_imsi_rsp, PRT_GET_IMSI_RSP);
	}
	else if(MTPNP_AD_Get_CardStatus(0) == MTPNP_AD_SIMCARD_VALID)
	{
		mod_id = MOD_L4C;
		SetProtocolEventHandler(kk_sys_get_imsi_rsp, PRT_GET_IMSI_RSP);
	}
#else
	mod_id = MOD_L4C;
	SetProtocolEventHandler(kk_sys_get_imsi_rsp, PRT_GET_IMSI_RSP);
#endif
//    mmi_frm_sms_send_message(MOD_MMI, mod_id, 0, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_REQ, (oslParaType *)msgReq, NULL);

	Message.oslSrcId = MOD_MMI;
    Message.oslDestId = mod_id;
    Message.oslMsgId = PRT_GET_IMSI_REQ;
    Message.oslDataPtr = NULL;  /* (oslParaType *)local_data; */
    Message.oslPeerBuffPtr = NULL;
    OslMsgSendExtQueue(&Message);
}
#endif

int IsSupportOverSea(void)
{
#ifdef KK_SUPPORT_OVERSEASP
	return 1;
#else
	return 0;
#endif
}
U8 *GetCategoryVideo_57History(U8 *history_buffer)
{
	return (wgui_get_inline_history(MMI_CATEGORY57_WEP_VIDEO_CHAT_ID, history_buffer));
} 
void  exit_kk_app_by_endkey(void)
{
	DeviceData * pDd = &s_DeviceData;
	
#ifdef WEP_DEBUG
	WM_DebugPrint("exit_kk_app_by_endkey:WEP ActiveScreenId=%d", GetActiveScreenId());
#endif
	   	
   	if ( gFlagEntryWep == MMI_FALSE )
   		return;
	if (gbIsEng)
	{
		gbIsEng = MMI_FALSE;
		SetCurrentLanguage(0);
	}
	
	if(pDd->pAppData!=NULL)
	{
		WinAppFree(pDd);
	}
	
	//DisplayIdleScreen();
	
	gFlagEntryWep = MMI_FALSE;
    WM_BackLightSet(pDd, FALSE);
	   
}

uint8_t WM_Get_System_Lang()   //BN10030
{
	KK_LANG_ID curr_lang = KK_LANG_ENGLISH;
	U16 curr_sys_lang_index = gCurrLangIndex;

	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_ENGLISH) == 0)
		curr_lang = KK_LANG_ENGLISH;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_SCHINESE) == 0)
		curr_lang = KK_LANG_SCHINESE;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_TCHINESE) == 0)
		curr_lang = KK_LANG_SCHINESE;
#if 0		
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_SPANISH) == 0)
			curr_lang = KK_LANG_SPANISH;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_DANISH) == 0)
			curr_lang = KK_LANG_DANISH;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_POLISH) == 0)
			curr_lang = KK_LANG_POLISH;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_THAI) == 0)
			curr_lang = KK_LANG_THAI;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_FRENCH) == 0)
			curr_lang = KK_LANG_FRENCH;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_GERMAN) == 0)
			curr_lang = KK_LANG_GERMAN;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_ITALIAN) == 0)
			curr_lang = KK_LANG_ITALIAN;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_RUSSIAN) == 0)
			curr_lang = KK_LANG_RUSSIAN;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_BULGARIAN) == 0)
			curr_lang = KK_LANG_BULGARIAN;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_UKRAINIAN) == 0)
			curr_lang = KK_LANG_UKRAINIAN;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_PORTUGUESE) == 0)
			curr_lang = KK_LANG_PORTUGUESE;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_TURKISH) == 0)
			curr_lang = KK_LANG_TURKISH;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_VIETNAMESE) == 0)
			curr_lang = KK_LANG_VIETNAMESE;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_INDONESIAN) == 0)
			curr_lang = KK_LANG_INDONESIAN;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_CZECH) == 0)
			curr_lang = KK_LANG_CZECH;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_MALAY) == 0)
			curr_lang = KK_LANG_MALAY;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_NORWEGIAN) == 0)
			curr_lang = KK_LANG_NORWEGIAN;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_ARABIC) == 0)
			curr_lang = KK_LANG_ARABIC ;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_PERSIAN) == 0)
			curr_lang = KK_LANG_PERSIAN;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_URDU) == 0)
			curr_lang = KK_LANG_URDU;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_HINDI) == 0)
			curr_lang = KK_LANG_HINDI;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_TAMIL) == 0)
			curr_lang = KK_LANG_TAMIL;
	if (strcmp((PS8) gLanguageArray[curr_sys_lang_index].aLangSSC, (PS8) SSC_BENGALI) == 0)
			curr_lang = KK_LANG_BENGALI;
#endif

	//if(curr_lang >= KK_LANG_MAX)
	if(((curr_lang!=KK_LANG_SCHINESE) && (curr_lang!=KK_LANG_TCHINESE) && (curr_lang!=KK_LANG_ENGLISH) )|| (curr_lang >= KK_LANG_MAX))
		curr_lang = KK_LANG_ENGLISH;
		
	return (U8)curr_lang;
}

byte_t WM_GetLangIdForProt(void)  //wyl20100525
{
	if (KK_LANG_SCHINESE == WM_Get_System_Lang())
		return 1;
	else 
		return 2;
}

//bruce0912010
#ifdef __KK_UI_V2__
WEPAppData *WM_GetAppData(void)
{
	DeviceData * pDd = &s_DeviceData;
	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;

	return pWep;
}
#endif

int KK_IsInWepScreen(void)  //wyl20100519
{
	DeviceData * pDd = &s_DeviceData;
		
	return GetActiveScreenId() == pDd->screenId;
}

#ifdef _KK_SELECT_ACCOUNT_  
void DEV_mmi_dtcnt_pre_entry_app_gprs_list(void)
{
    DataAccountReadyCheck(WEPMain);
}
#endif

#ifdef _KK_DEBUG_SELECT_
void SSCHandleKKDebugHilitHdr(S32 index)
{
	kk_debug_type_temp = index;
}

void SSCHandleKKDebugLSKHdr(void)
{
	kk_debug_type = kk_debug_type_temp;
	DisplayPopup((U8*)L"Done", 0, 0, 2000, 0);
	DeleteScreenIfPresent(SCR_ID_VIDEOCHAT_MENU);
}

void SSCHandleKKDebug(void)
{
	U8 *guiBuffer;
	PU8 strItems[] = 
	{
		(PU8)L"File",
		(PU8)L"Uart",
		(PU8)L"catcher"
	};

	EntryNewScreen(SCR_ID_VIDEOCHAT_MENU, 0, SSCHandleKKDebug, 0);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_VIDEOCHAT_MENU);

	if (guiBuffer == NULL)
	{
		kk_debug_type_temp = kk_debug_type;
	}
	
	RegisterHighlightHandler(SSCHandleKKDebugHilitHdr);
	ShowCategory353Screen((U8*)L"KK Debug Ouput Option", 0, 
		STR_GLOBAL_OK, 0, 
		STR_GLOBAL_BACK, 0,
		sizeof(strItems)/sizeof(strItems[0]),
		strItems,
		(U16*)gIndexIconsImageList,
		0,
		0, 
		kk_debug_type_temp,
		guiBuffer);

	SetLeftSoftkeyFunction(SSCHandleKKDebugLSKHdr, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

static void DevCloseLogFile(void)
{	
	if (logFile >= 0)
	{	
		MMI_FS_Close(logFile);
		logFile =  -1;
	}
	
}

static void Dev_OpenLogFile(void)
{
	if (KK_DEBUG_OUPUT_FILE == kk_debug_type)
	{
		char drv;
		U16 path[128];
	
		DevCloseLogFile();
		
		if ((drv = MMI_CARD_DRV) > 0)
		{
			if (fmgr_is_msdc_present())
			{
#ifdef __USB_IN_NORMAL_MODE__
				if (!mmi_usb_is_in_mass_storage_mode())
#endif
    				{
    					memset(path, 0, sizeof(path));
    					path[0] = drv;
					mmi_ucs2cat((S8*)path, (S8*)L":\\");
					mmi_ucs2cat((S8*)path, (S8*)KK_LOG_FILE_NAME);
					logFile = MMI_FS_Open((PU8)path, FS_CREATE_ALWAYS | FS_READ_WRITE);
				}
			}
			else
			{
				DisplayPopup((U8*)L"Storage Card Absent!", 0, 0, 2000, 0);
			}
		}
	}
}
#else
void SSCHandleKKDebug(void)
{
}
#endif
int DEV_if_select_account(void)
{
#ifdef _KK_SELECT_ACCOUNT_  
	return 1;
#else
	return 0;
#endif
}

int32_t kk_is_support_horizon_cam(void) //BN10122
{
#ifdef HORIZONTAL_CAMERA   
	return 1;        
#endif
	return 0;
}

#ifdef _KK_SELECT_ACCOUNT_

extern S8 gHomePLMNNum[];
#ifdef SUPPORT_DUAL_SIM
extern S8 gHomePLMNNum_2[];
#endif

int initDataAccount(WEPAppData * pWep)
{
    int account = 0;

#ifdef SUPPORT_DUAL_SIM
    int whichSim;
    E_MTPNP_AD_SIMCARD_USABLE_SIDE simStatus;
#endif

#ifdef WEP_DEBUG
	WM_DebugPrint("initDataAccount0");
#endif

    pWep->pILd->IntDataGet(pWep->pILd, LOCAL_DATA_RECORD_WHICH_ACCOUNT, &account);

    if (account < 0 /*|| account > DEV_GetAccountCount()*/)
    {
        char *plmn =  gHomePLMNNum;
	 int preAccount = -1;
	 
 #ifdef SUPPORT_DUAL_SIM
     	simStatus = MTPNP_AD_Get_UsableSide();
	if (MTPNP_AD_DUALSIM_USABLE == simStatus || MTPNP_AD_SIMCARD1_USABLE == simStatus)
	{
		whichSim = 0;
	}
	else
	{
		whichSim = 1;
	}
	pWep->pILd->IntDataSet(pWep->pILd, LOCAL_DATA_RECORD_WHICH_SIM, whichSim); 	//wyl20100408
 
	if (whichSim != 0)
		plmn = gHomePLMNNum_2;
 #endif

 	 preAccount = GetPredefineAccountByPlmn(plmn);

#ifdef WEP_DEBUG
	WM_DebugPrint("initDataAccount1 %d, %s, %d", preAccount, plmn, DEV_GetAccountCount());
#endif

	 if (preAccount >= 0)
	 {
	 	account = SearchAccountByApn(preAccount);

#ifdef WEP_DEBUG
	WM_DebugPrint("initDataAccount2 %d", account);
#endif

		if (account >= 0)
		{
			account = DEV_GetActualAccount(account);
			pWep->pILd->IntDataSet(pWep->pILd, LOCAL_DATA_RECORD_WHICH_ACCOUNT, account);
			return 1;
		}
		else
		{
			account  = DEV_GetNewAccountIndex();
			pWep->pILd->IntDataSet(pWep->pILd, LOCAL_DATA_RECORD_WHICH_ACCOUNT, account);
			CreateAccount(preAccount);
			return 0;
		}
	 }
	 else
	 {
	 	pWep->pILd->IntDataSet(pWep->pILd, LOCAL_DATA_RECORD_WHICH_ACCOUNT, 0);
	 	return 1;
	 }
    }
    return 1;
}

#endif


#endif
