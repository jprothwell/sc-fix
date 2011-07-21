/*
 * @(#)jmetoc.c 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */
#ifdef __MMI_SUPPORT_JME__
 #ifdef MMI_ON_HARDWARE_P
//#include "stdafx.h"
#include "protocolevents.h"
#include "stack_config.h"
#include "datetimetype.h"
#include "nvram_user_defs.h"
#include "globalconstants.h"
#include "idleappdef.h"
#include "mmi_data_types.h"
#include "gdi_include.h"
#include "nvramprot.h"
#include "eventsgprot.h"
#include "fontres.h"
#include "frameworkstruct.h"
#include "gui_data_types.h"
#include "dual_sim.h"

#include "mmi_msg_struct.h"
#include "dataaccountcommon.h"
#include "tcpip_inet.h"
#include "wgui_draw_manager.h"
#include "wgui_categories.h"
#include "smsapi.h"
#include "smsstruct.h"
#include "jmetoc.h"
#include "adp_events.h"
#include "funandgamesdefs.h"

#define SRC_JMETOC_INPUTBOX                 ZYX_IPTV_BASE
#define SRC_JMETOC_INPUTBOX_OPTION  (ZYX_IPTV_BASE +1)
#define SRC_JMETOC_INPUTBOX_IP           (ZYX_IPTV_BASE +2)
#define SRC_JMETOC_INPUTBOX_NUM        (ZYX_IPTV_BASE +3)
#define SRC_JMETOC_INPUTBOX_PW          (ZYX_IPTV_BASE +4)
#define SRC_JMETOC_SHOW_TEXT              (ZYX_IPTV_BASE +5)
#define SMARCHBOX_NO_SDCARD_SCREEN_ID  5001
void MMI_Sleep(u32 um)
{
     COS_Sleep(um);
}
#ifndef  JATAAYU_SUPPORT
UINT32 jdd_NetHtonl (UINT32 uiHostByteOrd)
{

	UINT32 uiNetByteOrd = 0 ;
	uiNetByteOrd = htonl(uiHostByteOrd);
	return (uiNetByteOrd) ;
}

#endif
UINT32 jmetoc_getTimerBase(VOID)
{
	return JMETOC_TIMERS_BASE_BEGIN;
}


extern void mdi_audio_resume_background_play(void);
UINT32 CFW_SetDataConnFun ( PFN_DATA_CONN_CALLBACK pfnEv );
UINT32 CFW_SetTCPIPCallBack ( PFN_TCPIP_CALLBACK pfnEv );
extern VOID mmi_display_noSimCard(void) ;
extern VOID mmi_display_OneSimCard(void) ;
char jmetoc_sc_num[20];
int jmetoc_event_break = 0;
int mod_mmi = MOD_MMI;
int mod_l4c = MOD_L4C;
int mod_l4c2 = MOD_L4C_2;
int mod_trace = MOD_BT;
int msg_req = PRT_MSG_ID_MMI_SMS_SEND_MSG_REQ;
//update 2010 04 15
U8 jmetoc_mmi_sms_sending = 0;
U8 jmetoc_gprs_activing = 0;
//void mmi_msg_handle_deliver_report_ind (void* inMsg, module_type mod, U16 result)
//if (jmetoc_mmi_sms_sending) {
//  jmetoc_mmi_sms_sending = 0;
//  return;
//}
S8 jmetoc_input_lock = 0;
unsigned int jmetoc_mmi_sms_deliver = PRT_MSG_ID_MMI_SMS_DELIVER_MSG_IND;
unsigned int jmetoc_mmi_sms_sendrsp = PRT_MSG_ID_MMI_SMS_SEND_MSG_RSP;
unsigned int jmetoc_mmi_sms_sendrpt = PRT_MSG_ID_MMI_SMS_STATUS_REPORT_IND;
unsigned short idle_sid = IDLE_SCREEN_ID;
unsigned short jmetoc_dummy_sid = IDLE_SCREEN_DUMMY_ID;
int smarchbox_account_id = -1;//MAX_DATA_ACCOUNT_GSM_LIMIT + MAX_DATA_ACCOUNT_GPRS_LIMIT - 1;
U8 smarchbox_uti;
int smarchbox_attrsp = 0;
BOOL wait_time = FALSE;
MYQUEUE* jmetoc_message;
extern void jmetoc_appLink(unsigned short* name);
extern void ShowListCategoryScreen(
        UI_string_type title,
        PU8 title_icon,
        UI_string_type left_softkey,
        PU8 left_softkey_icon,
        UI_string_type right_softkey,
        PU8 right_softkey_icon,
        S32 number_of_items);
extern void CHISTIncRecvdSMS(U16 nSimid);
extern void mmi_frm_sms_convert_new_sms(
                MMI_FRM_SMS_DELIVER_MSG_IND_STRUCT *data,
                mmi_frm_sms_deliver_msg_struct *entry);
extern void mmi_frm_sms_add_new_sms(mmi_frm_sms_deliver_msg_struct *entry, U16 length, U8 *data);
extern void mmi_frm_sms_status_report_ind (void* inMsg);
void MTPNP_PFAL_sms_status_report_ind(void* inMsg)
{
    mmi_frm_sms_status_report_ind(inMsg);
}
///////////////////////////////////////////////////////////// 屏幕方法 /////////////////////////////////////////////////////////////
/**
 * 刷新屏幕。
 * <p>
 * 将显示缓存刷入实际显存，实现屏幕刷新。
 * </p>
 * @param x 屏幕刷新点左上 X 坐标
 * @param y 屏幕刷新点左上 Y 坐标
 * @param width 刷新区域宽度
 * @param height 刷新区域高度
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
void jmetoc_gdi_refresh_buffer(S32 x, S32 y, S32 width, S32 height) {
  gdi_layer_blt(GDI_LAYER_MAIN_BASE_LAYER_HANDLE,0,0,0,x,y,width - 1,height - 1);
}

/**
 * 获得字体高度。
 * @return 字体高度
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
S32 jmetoc_get_font_height() {
  S32 w,h;
  gui_measure_string( (UI_string_type) "\x2D\x4E",&w,&h);
  return h;
}
extern pBOOL mmi_bootup_is_sim_valid();
extern U8 vm_setSmsChannel(void);
U8 SetSimChannel(void) {
 #ifdef __MMI_MULTI_SIM__
  return vm_setSmsChannel();
#else
  if (mmi_bootup_is_sim_valid()) {
    return 0;
  } else {
    return 0xff;
  }
#endif
}

extern u16 current_chat_sms_index;
/**
 * 接收新短消息。
 * @param msgInd 短信数据
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
void jmetoc_rsvNewMsg(void* inMsg, s32* l) {
  s32 len;
  mmi_frm_sms_deliver_msg_struct *data = OslMalloc(sizeof(mmi_frm_sms_deliver_msg_struct));
  MMI_FRM_SMS_DELIVER_MSG_IND_STRUCT *msgInd = (MMI_FRM_SMS_DELIVER_MSG_IND_STRUCT*) inMsg;
  mmi_frm_sms_convert_new_sms(msgInd, data);
  current_chat_sms_index = data->index;
  mmi_trace(1,"current_chat_sms_index is: %d",current_chat_sms_index);
 #ifdef __MMI_MULTI_SIM__
	CHISTIncRecvdSMS(mmi_frm_msg_get_simid_by_l4_index(data->index));
#else  /* No Defined __MMI_MULTI_SIM__ */
	CHISTIncRecvdSMS(0);
#endif /* __MMI_MULTI_SIM__ */
  mmi_frm_sms_add_new_sms(data, msgInd->no_msg_data, msgInd->msg_data);
  mmi_trace(g_sw_SMS,"data index is: %d",data->index);

  if(msgInd->dcs == SMSAL_8BIT_DCS) { //ascii
    len = -1;
  } else if(msgInd->dcs == SMSAL_UCS2_DCS) { //unicode
    len = msgInd->no_msg_data;
  } else {
    len = -1;
  }
  *l = len;

  OslMfree(data);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * 获得本地通话时间。
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
s32 jmetoc_getCallTime() {
  S16 error;
  s32 time = 0;
  MYTIME t;
  int i;
  for(i = 0; i < MMI_SIM_NUMBER; i++)
  {
  	ReadRecord(NVRAM_EF_CALL_TIME_LID, 2+i*3, (void*)&t, sizeof(MYTIME), &error);
 	 time += (t.nYear + t.nMonth + t.nDay) * 1000 + t.nHour * 60 + t.nMin;
 	 mmi_trace(1, "ct1 %d,%d", i + 1,t.nHour, t.nMin);
  	ReadRecord(NVRAM_EF_CALL_TIME_LID, 3 + i*3, (void*)&t, sizeof(MYTIME), &error);
  	time += (t.nYear + t.nMonth + t.nDay) * 1000 + t.nHour * 60 + t.nMin;
  	mmi_trace(1, "ct%d %d,%d",i + 1, t.nHour, t.nMin);
  }
  mmi_trace(1, "ctime %d", time);
  return time;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ MSG_ID_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ
#define PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_RSP MSG_ID_MMI_PS_SET_GPRS_DATA_ACCOUNT_RSP
u8 jmetoc_SIM = 0xff;
u8 jmetoc_cid = 0;
char* jmetoc_apn;
char jmetoc_apn_set [100] = {0x00,};
char jmetoc_user_set [100] = {0x00,};
char jmetoc_pwd_set [100] = {0x00,};

void jmetocInitApn(u8 cid) {
  kal_prompt_trace(1, "jmetocInitApn entry");
  s32 ret;
  CFW_GPRS_PDPCONT_INFO pdp_cont;
  pdp_cont.nApnSize = strlen(jmetoc_apn);
  pdp_cont.pApn = (u8*)jmetoc_apn;
  pdp_cont.nPdpAddrSize = 0;
  pdp_cont.pPdpAddr = NULL;
  pdp_cont.nDComp = 0;
  pdp_cont.nHComp = 0;
  pdp_cont.nPdpType = CFW_GPRS_PDP_TYPE_IP;
  pdp_cont.nApnUserSize = strlen(jmetoc_user_set);
  pdp_cont.pApnUser =(u8*)jmetoc_user_set;
  pdp_cont.nApnPwdSize = strlen(jmetoc_pwd_set);
  pdp_cont.pApnPwd= (u8*)jmetoc_pwd_set;
  #ifdef __MMI_MULTI_SIM__
  ret = CFW_GprsSetPdpCxt(cid, &pdp_cont, jmetoc_SIM);
  kal_prompt_trace(mod_trace, "set pdp %d, %s", ret, jmetoc_apn);
  #else
  ret = CFW_GprsSetPdpCxt(cid, &pdp_cont);
  #endif
}

u16 jmetoc_htons(u16 p) {
  return htons(p);
}

//static void jmetoc_GPRSSetAccountRsp(void* info)
//{
//	kal_prompt_trace(mod_trace, "account ok");
//}
/**
 * 配置数据账户。
 * @param apnMode 数据账户名称
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
//UINT32 CFW_SetJmetocUser(const INT8 *user);
UINT32 CFW_GetJmetocUser(INT8 * user);
//UINT32 CFW_SetJmetocPwd(const INT8 *pwd);
UINT32 CFW_GetJmetocPwd(INT8 * pwd);

UINT32 CFW_GetJmetocApn(INT8 * apn);

      U16 jme_proxy_port =0x50;
    U32 jme_proxy_IP =0xac00000a;
void jmetocinitNetwork(const char* apnMode) {
  kal_prompt_trace(1, "jmetocinitNetwork entry. jmetoc_SIM:%d",jmetoc_SIM);
  if(jmetoc_SIM ==0xff)
        jmetoc_SIM = SetSimChannel();
  CFW_GetJmetocApn(jmetoc_apn_set);
  CFW_GetJmetocPwd(jmetoc_pwd_set);
  CFW_GetJmetocUser(jmetoc_user_set);
  VM_readProxyPort(&jme_proxy_port);
  VM_readProxyIp(&jme_proxy_IP);

	jmetoc_apn = (char*)jmetoc_apn_set;
  if (jmetoc_SIM == 0xff) return;
  kal_prompt_trace(mod_trace, "account ok apn: %s",jmetoc_apn);
}
void jmetocSetApn(const char* apn)
{
        strcpy(jmetoc_apn_set,apn);
	  jmetoc_apn = jmetoc_apn_set;
        kal_prompt_trace(1, "jmetocSetApn jmetoc_apn:%s",jmetoc_apn);

}
void jmetocSetPwd(const char* pwd)
{
        strcpy(jmetoc_pwd_set,pwd);
        kal_prompt_trace(1, "jmetocSetApn jmetoc_apn:%s",jmetoc_pwd_set);

}
void jmetocSetUser(const char* user)
{
        strcpy(jmetoc_user_set,user);
        kal_prompt_trace(1, "jmetocSetApn jmetoc_apn:%s",jmetoc_user_set);

}

extern void Send_GPRS_Status_Ind(l4c_gprs_status_enum status);
extern u8 ADP_GetFreeUti();

extern U8 jmetoc_cid;
extern s8 jmetoc_soc_id;

void jmetocResetNetwork() {
  kal_prompt_trace(1, "jmetocResetNetwork entry,jmetoc_cid:0x%x,jmetoc_soc_id:0x%x,jmetoc_SIM:0x%x",jmetoc_cid,jmetoc_soc_id,jmetoc_SIM);
  u32 ret = 0;
  smarchbox_uti = 0xff;
  if (jmetoc_SIM != 0xff) {

       if((jmetoc_soc_id>=0)&&(jmetoc_soc_id<4))
       {
              CFW_SetTCPIPCallBackEx(NULL,jmetoc_soc_id);      //modify by wuys for  background QQ, 2010-05-31
     
      }
	CFW_SetTCPIPCallBackEx(NULL,4);      //modify by wuys for  background QQ, 2010-05-31
	
	if((jmetoc_cid>0)&&(jmetoc_cid<=7))
	{
		smarchbox_uti = ADP_GetFreeUti();
      #ifdef __MMI_MULTI_SIM__
             ret = CFW_GprsAct(0, jmetoc_cid, smarchbox_uti, jmetoc_SIM);
             mmi_trace(1, "act cid %d,%d", jmetoc_cid, ret);
	       CFW_SetDataConnFunEx(NULL,jmetoc_SIM,jmetoc_cid);      //modify by wuys for  background QQ, 2010-05-31


      #else
             ret = CFW_GprsAct(0, jmetoc_cid, smarchbox_uti);
	  
             mmi_trace(1, "act cid %d,%d", jmetoc_cid, ret);
	       CFW_SetDataConnFunEx(NULL,jmetoc_cid);      //modify by wuys for  background QQ, 2010-05-31
      #endif
             Send_GPRS_Status_Ind(L4C_GPRS_DETACHED);
             Send_GPRS_Status_Ind(L4C_PDP_DEACTIVED);
    }
	#ifdef __MMI_MULTI_SIM__
	       CFW_SetDataConnFunEx(NULL,jmetoc_SIM,0);//add by wuys for background QQ, 2010-05-31
       #else
	       CFW_SetDataConnFunEx(NULL,0);//add by wuys for background QQ, 2010-05-31
	#endif

  }
   jmetoc_cid = 0;
   jmetoc_soc_id = -1;

}

typedef struct
{
   kal_uint8 ref_count;
   kal_uint16 msg_len;
   kal_uint8 type;
   kal_uint8 url[256];
} wap_browser_startup_req_struct;
#define MMI_FRM_SMS_WAP_BROWSER_STARTUP_REQ  wap_browser_startup_req_struct
/**
 * 打开wap浏览器。
 * @param url 浏览器地址
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
 void mmi_frm_sms_send_message(module_type MOD_SRC, module_type MOD_DEST, U8 MSG_SAP, const U32/*U16*/ MSG_ID, void* LOCAL1, void* PEER);

void jmetoc_openurl(const u8* url) {
  MMI_FRM_SMS_WAP_BROWSER_STARTUP_REQ  *open_url_req;
		
	open_url_req =	(MMI_FRM_SMS_WAP_BROWSER_STARTUP_REQ*) OslConstructDataPtr(sizeof(*open_url_req));
	open_url_req->type = 2;
	pfnUnicodeStrcpy((S8*)open_url_req->url, (S8*)url);
	mmi_frm_sms_send_message(MOD_MMI,  MOD_WAP, 0, MSG_ID_WAP_BROWSER_STARTUP_REQ, (void *)open_url_req, NULL);
}

///////////////////////////////////////////////////////////// 进入退出 /////////////////////////////////////////////////////////////
extern int jdd_DataConnInitialize (void*	*pvDCHandle,
								   void					*pvArg) ;
 int jdd_NetInitialize (void);
 
/**
 * 退出百宝箱。
 * <p>
 * 这个方法在系统完全退出后调用。留给外部扩展。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
void mmi_audply_do_resume_action(void);
void smarchbox_quit() {
	kal_prompt_trace(1, "QQ  smarchbox_quit  ");

      jmetocResetNetwork();
      //jdd_DataConnInitialize(0, 0);
      //jdd_NetInitialize();

      #if ( defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) ) && defined(MMI_ON_HARDWARE_P)
       mmi_pen_enable_quick_launch();
      #endif
      //add by wuys for presenting state bar 2010-0202
      leave_full_screen();
     //if(QQ_pausedAudio)
     //	 mdi_start_background_timer();
	  
	mdi_audio_resume_background_play();
     
}


/**
 * 进入百宝箱。
 * <p>
 * 这个方法在系统进入时，未做任何初始化操作时调用。留给外部扩展。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
void smarchbox_entry() {

mmi_trace(1, "QQ  smarchbox_entry  ");
  //u32 ret = 0;
  //if (jmetoc_SIM != 0xff) {
  //  if (jmetoc_cid > 0) {
  //    #ifdef __DUAL_SIM__
  //    ret = CFW_ReleaseCID(jmetoc_cid, jmetoc_SIM);
  //    #else
  //    ret = CFW_ReleaseCID(jmetoc_cid);
  //    #endif
  //  }
  //  mmi_trace(1, "free cid %d,%d", jmetoc_cid, ret);
  //}
  jmetoc_input_lock = 0;
  jmetoc_soc_id = -1;
  smarchbox_uti = 0xff;
#if ( defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) ) && defined(MMI_ON_HARDWARE_P)
  mmi_pen_disable_quick_launch();
#endif
  //add by wuys for hiding state bar 2010-0202
  entry_full_screen();
  jmetoc_gprs_activing = 0;
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 Get_CharAscent() {
  return Get_CharHeight();
}


extern void UI_set_font(UI_font_type f);
extern void UI_set_text_color(color_t c);
extern void UI_move_text_cursor(S32 x, S32 y);
extern void UI_print_character(UI_character_type c);
void jmetoc_SetAppStatus(BOOL bIsActive);

#define R_OF_RGB565(p)      ((p) >> 11)
#define G_OF_RGB565(p)      (((p) << 21) >> 26)
#define B_OF_RGB565(p)      (((p) << 27) >> 27)
void jmetoc_print_ch(U16 ch, S16 x, S16 y, U16 clr, stFontAttribute* font) {
  jmetoc_SetAppStatus(TRUE);
  color_t c;
  c.r = R_OF_RGB565(clr) << 3;
  c.g = G_OF_RGB565(clr) << 2;
  c.b = B_OF_RGB565(clr) << 3;
  UI_set_text_color(c);
  UI_set_font(font);
  //UI_print_character_at_xy(ch, x, y);
  UI_move_text_cursor(x, y);
  UI_print_character(ch);
  jmetoc_SetAppStatus(FALSE);

}

/**
 * 去掉 GPRS 图标。
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
void hideGIcon(void) {
  HideStatusIcon(STATUS_ICON_GPRS_SERVICE);
#ifdef __MMI_MULTI_SIM__
  HideStatusIcon(STATUS_ICON_GPRS_SERVICE_2);
#endif
  UpdateStatusIcons();
}


extern void (*input_callback)(u16* in);                    // inputbox 回调函数
extern char jmetoc_input_buf[202];                         // 输入缓存
/**
 * 确定输入返回。
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
void jmetoc_confirm_inputForm() {
  if (input_callback != NULL) {
    input_callback((u16*)jmetoc_input_buf);
  }
  jmetoc_input_lock = 0;
  

  GoBackHistory();
}



/**
 * 取消输入返回。
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
void jmetoc_cancel_inputForm() {
  if (input_callback != NULL) {
    input_callback(NULL);
  }
  jmetoc_input_lock = 0;
  GoBackHistory();
}

void GoBack2History(void)
{
    GoBacknHistory(1);
}
void jmetoc_confirm_inputForm2() {
  if (input_callback != NULL) {
    input_callback((u16*)jmetoc_input_buf);
  }
  jmetoc_input_lock = 0;
  GoBack2History();
}
void jmetoc_cancel_inputForm2() {
  if (input_callback != NULL) {
    input_callback(NULL);
  }
  jmetoc_input_lock = 0;
  GoBack2History();
}


void jmetoc_not_empty (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, 0);
	SetLeftSoftkeyFunction (jmetoc_confirm_inputForm, KEY_EVENT_UP);
}


pBOOL jmetoc_inputForm_get_list_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
    U16 nStrItemList[2] = {STR_GLOBAL_DONE,STR_GLOBAL_BACK};
    pfnUnicodeStrcpy( (S8*)str_buff, GetString( nStrItemList[item_index] ) );
    *img_buff_p = (UI_image_type) GetImage( (U16)(IMG_GLOBAL_L1 + item_index) );

    return TRUE;
}

void jmecoc_inputForm_option_highlight_hdlr( S32 index )
{
    switch(index)
    {
        case 0:
              SetLeftSoftkeyFunction(jmetoc_confirm_inputForm2, KEY_EVENT_UP);
              break;
        case 1:
        default:
              SetLeftSoftkeyFunction(jmetoc_cancel_inputForm2, KEY_EVENT_UP);
            break;
    }
}

void jmetoc_entry_inputForm_option (void)
{

    U8* guiBuffer;
    
    TRACE_FMGR_FUNCTION();
    TRACE_FMGR_FUNCTION();

    EntryNewScreen(SRC_JMETOC_INPUTBOX_OPTION, NULL,
    jmetoc_entry_inputForm_option, NULL);
     mmi_trace(1, "enter jme screen %d",SRC_JMETOC_INPUTBOX_OPTION);
    guiBuffer = GetCurrGuiBuffer(SRC_JMETOC_INPUTBOX_OPTION);
    

    RegisterHighlightHandler(jmecoc_inputForm_option_highlight_hdlr);

	ShowCategory184Screen( STR_GLOBAL_OPTIONS, 0,	// title text and icon
		STR_GLOBAL_OK, 0,							// LSK text and icon
		STR_GLOBAL_BACK, 0, 								// RSK text and icon
		2, jmetoc_inputForm_get_list_item, 
		NULL,
		0, guiBuffer );


    //RegisterInputMethodScreenCloseFunction(fmgr_return_to_editor);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_UP);
    ClearKeyHandler(KEY_END,KEY_EVENT_UP);
    ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
    SetKeyHandler(jmetoc_cancel_inputForm2, KEY_END,KEY_EVENT_UP);
    return;

}


extern void GBSetInputboxLSKFunction(FuncPtr f);

/**
 * 进入输入法窗口。
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
 void jmetoc_entry_inputForm();
 void jmetoc_exit_Form_input()
{
    jmetoc_input_lock = 0;
    GenericExitScreen(SRC_JMETOC_INPUTBOX,  jmetoc_entry_inputForm);
}
void jmetoc_entry_inputForm() {
  U8 *guiBuffer = NULL;
  if (jmetoc_input_lock) return;
  jmetoc_input_lock = 1;
  EntryNewScreen(SRC_JMETOC_INPUTBOX,jmetoc_exit_Form_input,NULL,NULL);
  guiBuffer = GetCurrGuiBuffer( SRC_JMETOC_INPUTBOX );
  if( guiBuffer== NULL)
  {
     memset(jmetoc_input_buf,0x0,202);
  }
  RegisterInputBoxEmptyFunction(NULL);
  //RegisterInputBoxNotEmptyFunction(jmetoc_not_empty);
  ShowCategory5Screen(0, 0, STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, (U8*)jmetoc_input_buf, 100, guiBuffer);
  #if defined(__MMI_GB__)
    GBSetInputboxLSKFunction(jmetoc_entry_inputForm_option);
	#endif
    SetLeftSoftkeyFunction(jmetoc_entry_inputForm_option,KEY_EVENT_UP);
  
  //ShowCategory5Screen(0, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, (U8*)jmetoc_input_buf, 100, guiBuffer);
  //SetLeftSoftkeyFunction(jmetoc_confirm_inputForm, KEY_EVENT_UP);
  SetCategory5RightSoftkeyFunction(jmetoc_cancel_inputForm, KEY_EVENT_UP);
  //RegisterInputMethodScreenCloseFunction(GoBackHistory);
  ClearKeyHandler(KEY_END,KEY_EVENT_UP);
  ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
  SetKeyHandler(jmetoc_cancel_inputForm, KEY_END,KEY_EVENT_UP);
}

void jmetoc_entry_inputFormPW() ;
void jmetoc_exit_PW_input() {
  jmetoc_input_lock = 0;
//GenericExitScreen(SRC_JMETOC_INPUTBOX,  jmetoc_entry_inputFormPW);
}

void jmetoc_entry_inputFormPW() {
 U8 *guiBuffer = NULL;
  if (jmetoc_input_lock) return;
  jmetoc_input_lock = 1;
  EntryNewScreen(SRC_JMETOC_INPUTBOX_PW,jmetoc_exit_PW_input,jmetoc_entry_inputFormPW,NULL);
  guiBuffer = GetCurrGuiBuffer( SRC_JMETOC_INPUTBOX_PW );
  if(guiBuffer == NULL )
  {
     memset(jmetoc_input_buf,0x0, 202);
  }
  mmi_trace(1, "jmetoc_input_buf: %s",jmetoc_input_buf);

  RegisterInputBoxEmptyFunction(NULL);
  //RegisterInputBoxNotEmptyFunction(jmetoc_not_empty);
  ShowCategory5Screen(0, 0, STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, INPUT_TYPE_ALPHANUMERIC_PASSWORD|INPUT_TYPE_USE_ONLY_ENGLISH_MODES, (U8*)jmetoc_input_buf, 100, guiBuffer);
  #if defined(__MMI_GB__)
    GBSetInputboxLSKFunction(jmetoc_entry_inputForm_option);
   #endif
    SetLeftSoftkeyFunction(jmetoc_entry_inputForm_option,KEY_EVENT_UP);
   
  //ShowCategory5Screen(0, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, (U8*)jmetoc_input_buf, 100, guiBuffer);
  //SetLeftSoftkeyFunction(jmetoc_confirm_inputForm, KEY_EVENT_UP);
  SetCategory5RightSoftkeyFunction(jmetoc_cancel_inputForm, KEY_EVENT_UP);
  //RegisterInputMethodScreenCloseFunction(GoBackHistory);
  ClearKeyHandler(KEY_END,KEY_EVENT_UP);
   ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
   SetKeyHandler(jmetoc_cancel_inputForm, KEY_END,KEY_EVENT_UP);
  /*U8 *guiBuffer = NULL;
  if (jmetoc_input_lock) return;
  jmetoc_input_lock = 1;
  EntryNewScreen(SRC_JMETOC_INPUTBOX,jmetoc_exit_input,jmetoc_entry_inputFormPW,NULL);
  guiBuffer = GetCurrGuiBuffer( SRC_JMETOC_INPUTBOX );
  RegisterInputBoxEmptyFunction(NULL);
  RegisterInputBoxNotEmptyFunction(jmetoc_not_empty);
  ShowCategory5Screen(0, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, INPUT_TYPE_ALPHANUMERIC_PASSWORD, (U8*)jmetoc_input_buf, 100, guiBuffer);
  SetLeftSoftkeyFunction(jmetoc_confirm_inputForm, KEY_EVENT_UP);
  SetCategory5RightSoftkeyFunction(jmetoc_cancel_inputForm, KEY_EVENT_UP);
  RegisterInputMethodScreenCloseFunction(GoBackHistory);*/
}

void jmetoc_entry_inputFormNum();
void jmetoc_exit_Num_input(void)
{
    jmetoc_input_lock = 0;
    GenericExitScreen(SRC_JMETOC_INPUTBOX,  jmetoc_entry_inputFormNum);
}
void jmetoc_entry_inputFormNum() {
  U8 *guiBuffer = NULL;
  if (jmetoc_input_lock) return;
  jmetoc_input_lock = 1;
  EntryNewScreen(SRC_JMETOC_INPUTBOX_NUM,jmetoc_exit_Num_input,NULL,NULL);
  guiBuffer = GetCurrGuiBuffer( SRC_JMETOC_INPUTBOX_NUM );
  if( guiBuffer== NULL)
  {
     memset(jmetoc_input_buf,0x0,202);
  }
  RegisterInputBoxEmptyFunction(NULL);
  RegisterInputBoxNotEmptyFunction(jmetoc_not_empty);
  ShowCategory5Screen(0, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, INPUT_TYPE_NUMERIC, (U8*)jmetoc_input_buf, 11, guiBuffer);
  	
  SetLeftSoftkeyFunction(jmetoc_confirm_inputForm, KEY_EVENT_UP);
  SetCategory5RightSoftkeyFunction(jmetoc_cancel_inputForm, KEY_EVENT_UP);
 // RegisterInputMethodScreenCloseFunction(GoBackHistory);
  ClearKeyHandler(KEY_END,KEY_EVENT_UP);
  ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
  SetKeyHandler(jmetoc_cancel_inputForm, KEY_END,KEY_EVENT_UP);
}

///////////////////////////////////////////////////////////// 键盘相关 /////////////////////////////////////////////////////////////
extern void keyPressedCallBack(void);
extern void keyReleasedCallBack(void);
extern void keyRepeatCallBack(void);
extern void exitSmarchBox(void);

/**
 * 空回调。
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
static void none() {
}

/**
 * 注册所有键盘的回调函数。
 * <p>
 * 虚拟机会将要除了 POWER 和 END 键以外的所有键释放给应用使用。
 * 而这两个键由虚拟机自己处理，作为强制退出键。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
void jmetoc_regkeyhandle() {
  s32 i;
  for (i = 0; i < MAX_KEYS; i++) {
	  if (i != KEY_CLEAR && i != KEY_END) {
      SetKeyHandler(keyPressedCallBack,i,KEY_EVENT_DOWN);
      SetKeyHandler(keyReleasedCallBack,i,KEY_EVENT_UP);
      SetKeyHandler(keyRepeatCallBack,i,KEY_REPEAT);
    } else {
      SetKeyHandler(exitSmarchBox,i,KEY_EVENT_DOWN);
      SetKeyHandler(none,i,KEY_EVENT_UP);
      SetKeyHandler(none,i,KEY_REPEAT);
    }
  }
}

/**
 * 键盘最初回调设置。
 * <p>
 * 在虚拟机启动过程中，会调用本方法来屏蔽所有按键。
 * 当虚拟机启动完毕后，将会调用 jmetoc_regkeyhandle 方法来设置按键处理回调。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
void jmetoc_initkey() {
  s32 i;
  for (i = 0; i < MAX_KEYS; i++) {
    SetKeyHandler((void (*)(void))none,i,KEY_EVENT_DOWN);
    SetKeyHandler((void (*)(void))none,i,KEY_EVENT_UP);
    SetKeyHandler((void (*)(void))none,i,KEY_REPEAT);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * 设备智能适配器机器人。
 * <p>
 * 配套平台移植人员，对具体设备所有接口进行检测。
 * </p>
 * @return 是否启动机器人，并且退出当前百宝箱
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
#ifdef ____JMETOC_SMART_ADAPTER____
#include "jmetocadpt.h"
#else
s8 jmetocSmartAdaptRobot() {
  return 0;
}
#endif

extern void ADP_DispatchMessage(COS_EVENT* pCosEvent);
extern BOOL MMI_FreeEvent(COS_EVENT* pEvent);
extern void MMI_Free_msg_buf(ilm_struct  *ilm_ptr);
extern void ExecCurrTimerHandler(U16 TimerId);
extern void ProtocolEventHandler(U32 eventID,void* MsgStruct,int mod_src, void *peerBuf);
extern void dm_dealedKey(void);
extern void COS_KeyEventHandler(ilm_struct* Message);
COS_EVENT jmetoc_ev;
u32 jmetoc_soc_rsv_buffer_size;

/**
 * 消息分发。
 * <p>
 * 在某些情况下，系统需要等待特定的消息(如联网消息)而引起堵塞，
 * 这个时候，需要分发消息以确保特定消息等待过程中不会影响其他消息。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-03-02
 */
void jmetoc_dispatchEvents() {
  ilm_struct* Message;
  U16 timerid;
  SUL_ZeroMemory32( &jmetoc_ev, sizeof(COS_EVENT) );
  for(;;) {
    if (jmetoc_event_break) {
      jmetoc_event_break = 0;
      return;
    }
    MMI_FreeEvent(&jmetoc_ev);
		GET_COSMSG(MOD_MMI, &jmetoc_ev);
		Message = (ilm_struct*)jmetoc_ev.nParam1;

		switch (jmetoc_ev.nEventId) {
		case EV_TIMER:
			timerid = (UINT16) jmetoc_ev.nParam1;

			if (timerid ==TIMER_REAL_TIME_CLOCK) {
		    TM_SYSTEMTIME pSystemTime;
		    ilm_struct  ilm_ptr_REAL_TIME; 
		    mmi_eq_clock_tick_ind_struct *pREAL_TIME=OslMalloc(sizeof(mmi_eq_clock_tick_ind_struct));
		    ilm_ptr_REAL_TIME.local_para_ptr = (local_para_struct *)pREAL_TIME;
		    TM_GetSystemTime(&pSystemTime);
		    pREAL_TIME->rtc_time.rtc_year = pSystemTime.uYear - 2000;
		    pREAL_TIME->rtc_time.rtc_mon =pSystemTime.uMonth;
		    pREAL_TIME->rtc_time.rtc_day =pSystemTime.uDay;
		    pREAL_TIME->rtc_time.rtc_hour =pSystemTime.uHour;
		    pREAL_TIME->rtc_time.rtc_min =pSystemTime.uMinute;
		    pREAL_TIME->rtc_time.rtc_sec =pSystemTime.uSecond;
		    pREAL_TIME->rtc_time.rtc_wday =pSystemTime.uDayOfWeek;
		    ilm_ptr_REAL_TIME.dest_mod_id = MOD_MMI;
		    ilm_ptr_REAL_TIME.src_mod_id = MOD_MMI;
		    ilm_ptr_REAL_TIME.msg_id = MSG_ID_MMI_EQ_CLOCK_TICK_IND;
		    OslMsgSendExtQueue(&ilm_ptr_REAL_TIME);
			}
			ExecCurrTimerHandler(timerid);
			jmetoc_ev.nParam1 = 0;
			continue;
		case MSG_ID_MMI_EQ_KEYPAD_DETECT_IND:
		  dm_dealedKey();
		  COS_KeyEventHandler(Message);
		  break;
		default:
		  kal_prompt_trace(mod_trace, "ev %d,%d", jmetoc_ev.nEventId, MSG_ID_MMI_EQ_KEYPAD_DETECT_IND);
      ProtocolEventHandler(Message->msg_id,(void*)Message->local_para_ptr,(int)Message->src_mod_id, (void*) Message);
      break;
		}
		MMI_Free_msg_buf(Message);
	  jmetoc_ev.nParam1 = 0;
	  return;
	}
}

///////////////////////////////////////////////////////////// 动态菜单 /////////////////////////////////////////////////////////////

extern void mmi_fng_entry_screen();
static S32 jmetoc_menu_item_index = 0;
extern s8 smarchbox_robot_booted;


#define ____MAX_MENU____ 20
extern u8 *smarchbox_apps_menu_icons[____MAX_MENU____];
extern U16* smarchbox_apps_menu_labels[____MAX_MENU____];
extern U16* smarchbox_apps_menu_name[____MAX_MENU____];
extern S32 jmetoc_menu_items_num;

#include "__jmetoc_static_menu__.h"

/**
 * 没有t卡的时候，对相应菜单项进行操作
 * <p>
 * 在没有t卡的时候，点击各菜单项，会调用此函数进行处理，
 * 此函数先进行扣费操作，然后提示用户插入t卡。
 * </p>
 * @author GX
 * @version 1.0.0.1, 2009-12-01
*/
void jmetoc_handle_no_SDCard(void)
{
mmi_trace(1, "!!!!!QQ  jmetoc_handle_no_SDCard, jmetoc_menu_item_index:0x%x  ",jmetoc_menu_item_index);
  #if 0
  if (2==jmetoc_menu_item_index) {
  
    DataAccountReadyCheck_QQApn();
	return;
  }
  else if(4==jmetoc_menu_item_index)
  {
   VM_select_sim_entry();
   return;
  }
#endif

if(! checksim())
{
	mmi_display_noSimCard();
	return;
}
  void (*callback)() = (void (*)())jmetoc_getstaticmenucallback(jmetoc_menu_item_index);
//	EntryNewScreen( SMARCHBOX_NO_SDCARD_SCREEN_ID + 1, NULL ,  NULL,  NULL);   //Bad code!随意使用screenid
//	         mmi_trace(1, "enter jme screen %d",SMARCHBOX_NO_SDCARD_SCREEN_ID+1);
	mmi_trace(1, "!!!!!QQ  jmetoc_handle_no_SDCard, 2 ,,callback: 0x%x ",callback);

	smarchbox_robot_booted = 0;
	if (callback) {
		  mdi_audio_suspend_background_play();
	  callback();
	}
}

/**
 * 没有t卡的时候，各菜单项的高亮函数
 * <p>
 * 在没有t卡的时候，选中某个菜单项，将使菜单高亮，此时系统会自动调用以下高亮函数，
 * 此高亮 函数主要设置了各按键的回调函数。
 * </p>
 * @param menu_item_index 高亮菜单项的索引，由系统指定其值
 * @author GX
 * @version 1.0.0.1, 2009-12-01
*/
void highlight_no_sdcard_menu_item(s32 menu_item_index)
{
      mmi_trace(1, "!!!!!QQ  highlight_no_sdcard_menu_item, menu_item_index:0x%x  ",menu_item_index);

	standard_list_highlight_handler(menu_item_index);	   //设置标准高亮函数
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetLeftSoftkeyFunction(jmetoc_handle_no_SDCard, KEY_EVENT_UP);
	jmetoc_menu_item_index = menu_item_index;	 //赋值给全局变量
}

extern void (*ExitCategoryFunction) (void);
extern void ExitListCategoryScreen(void);
extern void register_fixed_list_highlight_handler(void (*f) (S32 item_index));

/**
 * 屏幕模板函数，显示菜单列表
 * <p>
 * 显示菜单列表，包括背景、左右软键、标题、标题图片、菜单项、菜单项图片
 * </p>
 * @param title_lable         菜单标题的名称字符串
 * @param title_icon          菜单标题的图标
 * @param left_softkey_lable  左软键名称字符串
 * @param left_softkey_icon   左软键图标
 * @param right_softkey_lable 右软键名称字符串
 * @param menu_items_num      菜单的菜单项总数
 * @param menu_items_lable[]  包含了所有菜单项的名称字符串数组
 * @param menu_itmes_icon[]   包含了所有菜单项的图标数组
 * @author GX
 * @version 1.0.0.1, 2009-12-01
*/
void jmetoc_show_fixed_list_menu(
	const U16*    title_lable, 
	const U8*     title_icon, 
	const U16*    left_softkey_lable, 
	const U8 *    left_softkey_icon,
	const U16*    right_softkey_lable,
	const U8 *    right_softkey_icon,
	const S32     menu_items_num,
	U16*          menu_items_lable[],
	U8 *          menu_items_icon[] )
{
  dm_data_struct dm_data;
  S32 i;

      mmi_trace(1, "!!!!!QQ  jmetoc_show_fixed_list_menu  ");

  gdi_layer_lock_frame_buffer();
  //初始化菜单框架
  move_fixed_list(0, MMI_CONTENT_Y + 5);
  resize_fixed_list( MMI_CONTENT_WIDTH,  MMI_CONTENT_HEIGHT);

  //初始化菜单项公用数据
  create_fixed_icontext_menuitems();

  //联合菜单框架与菜单项
  associate_fixed_icontext_list();

  //初始化List的各项参数
  ShowListCategoryScreen(
    (UI_string_type)title_lable,
    (PU8)title_icon,
    (UI_string_type)left_softkey_lable,
    (PU8)left_softkey_icon,
    (UI_string_type)right_softkey_lable,
    (PU8)right_softkey_icon,
    (S32)menu_items_num);

  // 注册菜单的高亮函数，此处覆盖了上面ShowListCategoryScreen中所注册的高亮函数
  register_fixed_list_highlight_handler((void (*) (S32))highlight_no_sdcard_menu_item);

  //初始化各菜单项
  for(i = 0; i < menu_items_num; ++i)
    add_fixed_icontext_item(menu_items_lable[i],  menu_items_icon[i]);

  //初始化菜单高亮起始位置
  fixed_list_goto_item_no_redraw(0);

gdi_layer_unlock_frame_buffer();

  //刷新屏幕以显示菜单
  dm_setup_category_functions(dm_redraw_category_screen, dm_get_category_history, dm_get_category_history_size);
  dm_data.S32ScrId = (S32) GetActiveScreenId();
  dm_data.S32CatId = MMI_CATEGORY52_ID;
  dm_data.S32flags = 0;
  dm_setup_data(&dm_data);
  dm_redraw_category_screen();	
}

#ifdef __MMI_COMPRESS_CODE_SECTION__
extern void cdfu_jmetoc();
#endif

extern bool check_AppGprsStatus(void);
void fixed_list_goto_previous_item(void);
void fixed_list_goto_next_item(void);
/*
void handle_key(void)
{
      mmi_trace(1, "!!!!! jmetoc_smmu_clean_all ");

jmetoc_smmu_clean_all();
	SetKeyHandler(fixed_list_goto_previous_item, KEY_UP_ARROW, KEY_EVENT_DOWN);
       SetKeyHandler(fixed_list_goto_next_item, KEY_DOWN_ARROW, KEY_EVENT_DOWN);

}*/


	/*show jmetoc main menu screen*/
void jmetoc_showlist() {
//return;
//add by wuys 2010-0203
//if(FALSE==check_AppGprsStatus())
{
   //return;
}

//add end
  mmi_trace(1, "!!!!!QQ  jmetoc_showlist,ExitCategoryFunction : 0x%x",ExitCategoryFunction);
  int i;
  const U16* title_lable = (U16*)"\x94\x5E\x28\x75\x17\x52\x68\x88\x00";   //应用列表
  const U8   *title_icon = NULL;

  const U16* left_softkey_lable = (U16*)"\x6e\x78\x9a\x5b\x00";  // 确定
  const U8* left_softkey_icon = NULL;

  const U16* right_softkey_lable = (U16*)"\xD4\x8f\xDE\x56\x00";      //返回
  const U8* right_softkey_icon = NULL;

  jmetoc_menu_items_num = JMETOC_MENU_ITEM_NUM;
#ifdef __MMI_COMPRESS_CODE_SECTION__
  cdfu_jmetoc();
#endif
	//菜单项图标列表
	//for (i = 0; i < JMETOC_MENU_ITEM_NUM; i++) {
 //   smarchbox_apps_menu_icons[i] = wgui_get_list_menu_icon(i, IMG_GLOBAL_L1 + i);
 // }
  for (i = 0; i < JMETOC_MENU_ITEM_NUM; i++) {
    smarchbox_apps_menu_labels[i] = no_SDCard_menu_item_lables[i];
    smarchbox_apps_menu_icons[i] = jmetoc_getstaticmenuicon(i);
  }
  jmetoc_smmu_load_menu();
      mmi_trace(1, "!!!!!QQ  jmetoc_showlist,aftyer jmetoc_smmu_load_menu ");

  //设置退出屏幕的清理函数
 // ExitCategoryFunction = jmetoc_smmu_clean_all;
	//初始化屏幕
	//EntryNewScreen(SMARCHBOX_NO_SDCARD_SCREEN_ID,  ExitCategoryFunction,  jmetoc_showlist,  NULL);
	EntryNewScreen(SMARCHBOX_NO_SDCARD_SCREEN_ID,  jmetoc_smmu_clean_all,  jmetoc_showlist,  NULL);
	         mmi_trace(1, "enter jme screen %d",SMARCHBOX_NO_SDCARD_SCREEN_ID);
	//显示菜单
#if 1
	jmetoc_show_fixed_list_menu(
		title_lable,
		title_icon, 
		left_softkey_lable,
		left_softkey_icon, 
		right_softkey_lable,
		right_softkey_icon,
		jmetoc_menu_items_num,
		smarchbox_apps_menu_labels,
		smarchbox_apps_menu_icons
		);
#endif

}



u16* jmetoc_text_info;
void (*jmetoc_text_f)(void);

void jmetoc_show_ui_text_show() {
//			  kal_prompt_trace(mod_trace, "!!!!!QQ  jmetoc_show_ui_text_show  ");
      mmi_trace(1, "!!!!!QQ  jmetoc_show_ui_text_show  ");
  EntryNewScreen(SRC_JMETOC_SHOW_TEXT,NULL,jmetoc_show_ui_text_show,NULL);
        mmi_trace(1, "enter jme screen %d",SRC_JMETOC_SHOW_TEXT);

  ShowCategory74Screen(
    0,  //title
     0, //title icon
    STR_GLOBAL_OK, //left soft
    0, //left soft icon
    STR_GLOBAL_CANCEL,  //right soft
     0, //right soft icon
    (U8*)jmetoc_text_info,  //buf
    UCS2Strlen((const s8*)jmetoc_text_info) << 1, //buf size
    0 //histroy
    );
        mmi_trace(1, "!!!!!QQ  jmetoc_show_ui_text_show 2 ");

  SetLeftSoftkeyFunction(jmetoc_text_f, KEY_EVENT_UP);
        mmi_trace(1, "!!!!!QQ  jmetoc_show_ui_text_show  3");

  SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
        mmi_trace(1, "!!!!!QQ  jmetoc_show_ui_text_show exit  ");

}

/**
 * 显示文本框。
 */
void jmetoc_show_ui_text(void (*f) (void), U16* info) {
  jmetoc_text_info = info;
  jmetoc_text_f = f;

  jmetoc_show_ui_text_show();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef __MMI_DUAL_SIM_MASTER__
void jmetoc_frm_sms_get_card2_profile_detail_rsp(void *info) {
	MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT *msgRsp = (MMI_FRM_SMS_GET_PROFILE_PARAMS_RSP_STRUCT *) info;
	U16 result = MMI_FRM_SMS_ERROR;

	jmetocCard2Prof = OslMalloc(sizeof(msg_profile_edit_struct));
	ClearSlaveProtocolEventHandler(PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
	if (msgRsp->result == TRUE)
	{
		mmi_asc_n_to_ucs2((PS8) jmetocCard2Prof->scAddress,
                              (PS8) msgRsp->sc_addr.number, msgRsp->sc_addr.length);
		memset((void *) &jmetocCard2Prof->scAddress[msgRsp->sc_addr.length * ENCODING_LENGTH], 0,
               ENCODING_LENGTH);
		result = MMI_FRM_SMS_OK;
	}
	mmi_frm_sms_callback_action((void *) jmetocCard2Prof, result);
	OslMfree(jmetocCard2Prof);
}

void jmetoc_frm_sms_get_card2_profile_detail_req(void *inMsg) {
  MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT*	msgReq;
	MTPNP_UINT8 index = *(U8 *) inMsg;
	msgReq	=	(MMI_FRM_SMS_GET_PROFILE_PARAMS_REQ_STRUCT*)OslConstructDataPtr(sizeof (*msgReq));
	msgReq->profile_no	=	index;
	
	SetSlaveProtocolEventHandler(jmetoc_frm_sms_get_card2_profile_detail_rsp, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_RSP);
	mmi_frm_sms_send_message(MOD_MMI, MOD_L4C_2, 0, PRT_MSG_ID_MMI_SMS_GET_PROFILE_PARAMS_REQ, (oslParaType *)msgReq, NULL);
	OslMfree(inMsg);
}

void jmetoc_sms_get_profile_detail2(PsFuncPtrU16 callback) {
  U8 *data = OslMalloc(sizeof(U8));
  memset(data, 0, sizeof(U8));
  mmi_frm_sms_write_action(callback, MOD_MMI, data, jmetoc_frm_sms_get_card2_profile_detail_req);
}
#endif

void jmetoc_getSmsCenter_req(PsFuncPtrU16 callback) {
#ifdef __MMI_DUAL_SIM_MASTER__
  if (SetSimChannel() == 1) {
#endif
    mmi_frm_sms_get_profile_detail(callback, MOD_MMI, 0);
#ifdef __MMI_DUAL_SIM_MASTER__
  } else {
    jmetoc_sms_get_profile_detail2(callback);
  }
#endif
}

void jmetoc_copyAddressFromProfile(void *profiledetail) {
  msg_profile_edit_struct* p;
  U16* ad;
  S32 i;
  p = (msg_profile_edit_struct*)profiledetail;
  kal_prompt_trace(mod_trace, "sc addr %s", (char*)p->scAddress);
  ad = (U16*)p->scAddress;
  memset(jmetoc_sc_num, 0, 20);
  if (ad[0] == '+') ad++;
  for (i = 0; i < 19; i++) {
    jmetoc_sc_num[i] = ad[i];
    if (ad[i] == 0) {
      break;
    }
  }
}


S8 jmetoc_IMSI[40];
void jmetoc_getIMSI(void* buf) {
  mmi_smu_get_imsi_rsp_struct *local_data_p = (mmi_smu_get_imsi_rsp_struct*) buf;
  memset(jmetoc_IMSI,0,40);
  if( local_data_p->result) {
    memcpy(jmetoc_IMSI, (S8*)local_data_p->imsi+1, 16);
  }
}
void jmetoc_getIMSI_req(PsFuncPtr callback) {
	 MYQUEUE Message;
	 Message.oslSapId = MMI_L4C_SAP;
	 Message.oslMsgId = PRT_GET_IMSI_REQ;
	 Message.oslDataPtr = NULL;
	 Message.oslPeerBuffPtr= NULL;
	 Message.oslSrcId=MOD_MMI;
#ifdef __MMI_DUAL_SIM_MASTER__
  if (SetSimChannel() == 1) {
#endif
    Message.oslDestId = MOD_L4C;
    OslMsgSendExtQueue(&Message);
    SetProtocolEventHandler(callback, PRT_GET_IMSI_RSP);
#ifdef __MMI_DUAL_SIM_MASTER__
  } else {
    Message.oslDestId = MOD_L4C_2;
    OslMsgSendExtQueue(&Message);
    SetSlaveProtocolEventHandler(callback, PRT_GET_IMSI_RSP);
  }
#endif
}
void smarchbox_event_notify(s32 id) {
}


/**
 * 显示询问信息。
 * @param info 询问信息
 * @author runasea
 * @version 1.0.0.1, 2010-03-02
 */
 extern void ShowCategory165Screen(
                U16 left_softkey,
                U16 left_softkey_icon,
                U16 right_softkey,
                U16 right_softkey_icon,
                UI_string_type message,
                U16 message_icon,
                U8 *history_buffer);
void jmetoc_alert_ask(U16* info) {
  ShowCategory165Screen(STR_GLOBAL_YES,IMG_GLOBAL_YES,STR_GLOBAL_NO,IMG_GLOBAL_NO,info,IMG_GLOBAL_QUESTION,NULL);
}

/**
 * 短信重新发送。
 * @author runasea
 * @version 1.0.0.1, 2010-03-02
 */
void jmetoc_resend_sms() {
  OslMsgSendExtQueue(jmetoc_message);
}

void jmetoc_show_icon() {
  ShowStatusIcon(STATUS_ICON_JAVA_PAUSE);
  UpdateStatusIcons();
}

void jmetoc_hide_icon() {
  HideStatusIcon(STATUS_ICON_JAVA_PAUSE);
  UpdateStatusIcons();
}

void jmetoc_blink_icon() {
  BlinkStatusIcon(STATUS_ICON_JAVA_PAUSE);
  UpdateStatusIcons();
}

extern void VM_select_sim(void);
extern UINT8 checksim(void);

U32 VM_select_sim_entry(void)
{
	UINT8 sim_num =0;
	sim_num = checksim();
   	if(0==sim_num)
	{
		mmi_display_noSimCard();
		return 0;
	}else if(1==sim_num){
		mmi_display_OneSimCard();
		return 0;
	}

       VM_select_sim();
    return 0;
}



void VM_ReturnThreeDigitString(PU8 StringVal, U8 DigitVal)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 LocalBuffer[8];
    S8 StrVal[8];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (DigitVal == 0)
    {
        AnsiiToUnicodeString((PS8) StrVal, (PS8) "000");
    }
    else if (DigitVal < 10)
    {
        sprintf(LocalBuffer, "00%d", DigitVal);
        AnsiiToUnicodeString((PS8) StrVal, (PS8) LocalBuffer);
    }
    else if (DigitVal < 100)
    {
        sprintf(LocalBuffer, "0%d", DigitVal);
        AnsiiToUnicodeString((PS8) StrVal, (PS8) LocalBuffer);
    }
    else
    {
        sprintf(LocalBuffer, "%d", DigitVal);
        AnsiiToUnicodeString((PS8) StrVal, (PS8) LocalBuffer);
    }
    pfnUnicodeStrcpy((PS8) StringVal, (PS8) StrVal);
}

void VM_PreparedIPAddressString(U8 *string_buffer, U8 *IP1, U8 *IP2, U8 *IP3, U8 *IP4)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 Point[MAX_PART_IP_ADDRESS];
    S32 IPAddress[MAX_PART_IP_ADDRESS];
    U8 LocalBuffer[8];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    AnsiiToUnicodeString((S8*) Point, (S8*) ".");

    IPAddress[0] = (S32) gui_atoi((UI_string_type) IP1);
    IPAddress[1] = (S32) gui_atoi((UI_string_type) IP2);
    IPAddress[2] = (S32) gui_atoi((UI_string_type) IP3);
    IPAddress[3] = (S32) gui_atoi((UI_string_type) IP4);

    VM_ReturnThreeDigitString(LocalBuffer, (U8) IPAddress[0]);
    pfnUnicodeStrcpy((PS8) string_buffer, (PS8) LocalBuffer);
    pfnUnicodeStrcat((PS8) string_buffer, (PS8) Point);

    VM_ReturnThreeDigitString(LocalBuffer, (U8) IPAddress[1]);
    pfnUnicodeStrcat((PS8) string_buffer, (PS8) LocalBuffer);
    pfnUnicodeStrcat((PS8) string_buffer, (PS8) Point);

    VM_ReturnThreeDigitString(LocalBuffer, (U8) IPAddress[2]);
    pfnUnicodeStrcat((PS8) string_buffer, (PS8) LocalBuffer);
    pfnUnicodeStrcat((PS8) string_buffer, (PS8) Point);

    VM_ReturnThreeDigitString(LocalBuffer, (U8) IPAddress[3]);
    pfnUnicodeStrcat((PS8) string_buffer, (PS8) LocalBuffer);
}
UINT8 IP_address_desc[]="\x49\x00\x50\x00\x20\x00\x41\x00\x64\x00\x64\x00\x00\x00";
UINT8 Port_desc[] ="\x50\x00\x6f\x00\x72\x00\x74\x00\x00\x00";
    U8 proxy_port[12] = {0,};
    U8 ip1[8];
    U8 ip2[8];
    U8 ip3[8];
    U8 ip4[8];
U32 cfg_write_hex(U8 *uSubkey,U8 *name, void* data,U8 usize);
U32 cfg_read_hex(U8 *uSubkey,U8 *name, void* data,U8 usize);
void VM_readProxyIp(U32 *proxyIp)
{
      UINT32 ret = ERR_SUCCESS;
      ret = cfg_read_hex("JMETOC", "PROXY_ADR", (UINT8*)proxyIp, 4);

}
void VM_readProxyPort(U16 *proxyPort)
{
      UINT32 ret = ERR_SUCCESS;
      ret = cfg_read_hex("JMETOC", "PROXY_PORT", (UINT8*)proxyPort, 2);

}
void VM_writeProxyIp(U32 *proxyIp)
{
      UINT32 ret = ERR_SUCCESS;
      ret = cfg_write_hex("JMETOC", "PROXY_ADR", (UINT8*)proxyIp, 4);

}
void VM_writeProxyPort(U16 *proxyPort)
{
      UINT32 ret = ERR_SUCCESS;
      ret = cfg_write_hex("JMETOC", "PROXY_PORT", (UINT8*)proxyPort, 2);

}
 UINT8 VM_readSimNum(void)
{
      UINT32 ret = ERR_SUCCESS;
	   UINT8 val = 0x00;
      ret = cfg_read_hex("JMETOC", "SIM_APN_NUM", &val, 1);
	  return ((val & 0xF0)>>4);

}

void VM_writeSimNum(U8 simNum)
{
      UINT32 ret = ERR_SUCCESS;
	UINT8 val = 0x00;
      ret = cfg_read_hex("JMETOC", "SIM_APN_NUM", &val, 1);
	val = (val & 0x0F)|((simNum & 0x0F)<<4);
      ret = cfg_write_hex("JMETOC", "SIM_APN_NUM", &val, 1);

}
UINT8 VM_readApnNum(void)
{
      UINT32 ret = ERR_SUCCESS;
	   UINT8 val = 0x00;
      ret = cfg_read_hex("JMETOC", "SIM_APN_NUM", &val, 1);
	  return (val & 0x0F);

}

void VM_writeApnNum(U8 simNum)
{
      UINT32 ret = ERR_SUCCESS;
	UINT8 val = 0x00;
      ret = cfg_read_hex("JMETOC", "SIM_APN_NUM", &val, 1);
	val = (val & 0xF0)|(simNum & 0x0F);
      ret = cfg_write_hex("JMETOC", "SIM_APN_NUM", &val, 1);

}

void VM_mmi_ph_edit_conn_type_fill_inline_struct(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S32 buffer_size = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
  

 
        SetInlineItemActivation(
            (wgui_inline_items + 1),
            INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
            0);
        SetInlineItemCaption(
            (wgui_inline_items + 0),
            (U8*) IP_address_desc);
        SetInlineItemActivation((wgui_inline_items + 3), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
        SetInlineItemCaption((wgui_inline_items + 2), (U8*) Port_desc);

        buffer_size = 12;   //6
        SetInlineItemTextEdit(
            (wgui_inline_items + 3),
            (U8*) proxy_port,
            buffer_size,
            INPUT_TYPE_NUMERIC);
         VM_readProxyPort(&jme_proxy_port);
        gui_itoa((S32) jme_proxy_port, (U16*) proxy_port, 10);
        VM_readProxyIp(&jme_proxy_IP);
        VM_ReturnThreeDigitString(ip1, (U8)jme_proxy_IP);
        VM_ReturnThreeDigitString(ip2, (U8)(jme_proxy_IP>>8));
        VM_ReturnThreeDigitString(ip3, (U8)(jme_proxy_IP>>16));
        VM_ReturnThreeDigitString(ip4, (U8)(jme_proxy_IP>>24));

        SetInlineItemIP4(
            (wgui_inline_items + 1),
            (U8*) ip1,
            (U8*) ip2,
            (U8*) ip3,
            (U8*) ip4,
            VM_PreparedIPAddressString);

    
}
void VM_mmi_ph_save_connection_type_changes(void)
{

 U8 ip_value[4];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ip_value[0] = (U8) gui_atoi((UI_string_type) ip1);
    ip_value[1] = (U8) gui_atoi((UI_string_type) ip2);
    ip_value[2] = (U8) gui_atoi((UI_string_type) ip3);
    ip_value[3] = (U8) gui_atoi((UI_string_type) ip4);

    jme_proxy_IP = ip_value[0] |( ip_value[1]<<8)|( ip_value[2]<<16) | (ip_value[3]<<24);

//hal_HstSendEvent(jme_proxy_IP);
VM_writeProxyIp(&jme_proxy_IP);


        jme_proxy_port = (U32) gui_atoi((UI_string_type)proxy_port);
//hal_HstSendEvent(jme_proxy_port);

VM_writeProxyPort(&jme_proxy_port);
                //mmi_ph_update_connection_type();
//	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_OK, 5, 1000, NULL);
       GoBackHistory();
//	DeleteNHistory(2);
            
        
    
}

void jmetoc_exit_ProxyIp_input(void)
{

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    history_t h;
    U16 input_buffer_size = 0;
    S16 nHistory = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    h.scrnID = SRC_JMETOC_INPUTBOX_IP;
    CloseCategory57Screen();
    h.entryFuncPtr = VM_setProxyIp;
    pfnUnicodeStrcpy((S8*) h.inputBuffer, (S8*) & nHistory);
    GetCategory57History(h.guiBuffer);
    input_buffer_size = (U16) GetCategory57DataSize();
    GetCategory57Data((U8*) h.inputBuffer);
    AddNHistory(h, input_buffer_size);
  jmetoc_input_lock = 0;
}

void  VM_setProxyIp(void)
{
   if (jmetoc_input_lock) 
   	return ;
    jmetoc_input_lock = 1;
    U8 *input_buffer_p = NULL;  /* added for inline edit history */
    U8 *gui_buffer_p = NULL;    /* Buffer holding history data */
    U16 input_buffer_size = 0;
    U16 edit_conn_type_icon_list[4] = {IMG_GLOBAL_L1, 0, IMG_GLOBAL_L2, 0};
    EntryNewScreen(SRC_JMETOC_INPUTBOX_IP, jmetoc_exit_ProxyIp_input, NULL, NULL);
		         mmi_trace(1, "enter jme screen %d",SRC_JMETOC_INPUTBOX_IP);
mmi_trace(1,"SRC_JMETOC_INPUTBOX_IP = %d",SRC_JMETOC_INPUTBOX_IP);
mmi_trace(1,"SRC_JMETOC_INPUTBOX = %d",SRC_JMETOC_INPUTBOX);
mmi_trace(1,"SRC_JMETOC_INPUTBOX_NUM = %d",SRC_JMETOC_INPUTBOX_NUM);
mmi_trace(1,"SRC_JMETOC_INPUTBOX_PW = %d",SRC_JMETOC_INPUTBOX_PW);
mmi_trace(1,"SRC_JMETOC_SHOW_TEXT = %d",SRC_JMETOC_SHOW_TEXT);
mmi_trace(1,"SRC_JMETOC_INPUTBOX_OPTION = %d",SRC_JMETOC_INPUTBOX_OPTION);
mmi_trace(1,"SMARCHBOX_NO_SDCARD_SCREEN_ID = %d",SMARCHBOX_NO_SDCARD_SCREEN_ID);

    InitializeCategory57Screen();

    gui_buffer_p = GetCurrGuiBuffer(SRC_JMETOC_INPUTBOX_IP);
    input_buffer_p = GetCurrNInputBuffer(SRC_JMETOC_INPUTBOX_IP, &input_buffer_size);  /* added for inline edit history */

    VM_mmi_ph_edit_conn_type_fill_inline_struct();

    if (input_buffer_p != NULL)
    {
            SetCategory57Data(wgui_inline_items, 4, input_buffer_p);
    }
        ClearAllKeyHandler();
        ShowCategory57Screen(
            STR_GLOBAL_EDIT,
            0,/* GetRootTitleIcon(SERVICES_WAP_MENU_ID), */ //GetRootTitleIcon(g_ph_cntx.ph_title_img_id),
            STR_GLOBAL_EDIT,
            0,
            STR_GLOBAL_BACK,
            IMG_GLOBAL_BACK,
            4,
            edit_conn_type_icon_list,
            wgui_inline_items,
            0,
            gui_buffer_p);
       SetInlineListMenuChanged();
       SetCategory57RightSoftkeyFunctions(VM_mmi_ph_save_connection_type_changes, GoBackHistory);
     return ;
}
S8* apn_str="APN:";
S8* usr_str="User Name:";
S8* pwd_str="Password:";
S8 apn_a[10];
S8 usr_a[22];
S8 pwd_a[20];

U8 apn_input[100];
U8 usr_input[50];
U8 pwd_input[50];

void VM_edit_Apn_fill_inline_struct(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	AnsiiToUnicodeString(apn_a, apn_str);
	AnsiiToUnicodeString(usr_a, usr_str);
	AnsiiToUnicodeString(pwd_a, pwd_str);
	
	memset(jmetoc_apn_set,0,100);
	memset(jmetoc_user_set,0,100);
	memset(jmetoc_pwd_set,0,100);

      CFW_GetJmetocApn(jmetoc_apn_set);
      CFW_GetJmetocPwd(jmetoc_pwd_set);
      CFW_GetJmetocUser(jmetoc_user_set);

	AnsiiToUnicodeString(apn_input, jmetoc_apn_set);
	AnsiiToUnicodeString(usr_input, jmetoc_user_set);
	AnsiiToUnicodeString(pwd_input, jmetoc_pwd_set);

	
       SetInlineItemCaption((wgui_inline_items + 0), (U8*) apn_a);

       SetInlineItemActivation((wgui_inline_items + 1), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);

        SetInlineItemTextEdit(
            (wgui_inline_items + 1),
            (U8*) apn_input,
            100,
            INPUT_TYPE_ALPHANUMERIC_SENTENCECASE);

          SetInlineItemCaption((wgui_inline_items + 2), (U8*) usr_a);

    SetInlineItemActivation((wgui_inline_items + 3), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);

	
        SetInlineItemTextEdit(
            (wgui_inline_items + 3),
            (U8*) usr_input,
            50,
            INPUT_TYPE_ALPHANUMERIC_SENTENCECASE);
		
          SetInlineItemCaption((wgui_inline_items + 4), (U8*) pwd_a);

    SetInlineItemActivation((wgui_inline_items + 5), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);

        SetInlineItemTextEdit(
            (wgui_inline_items + 5),
            (U8*) pwd_input,
            50,
            INPUT_TYPE_ALPHANUMERIC_SENTENCECASE);

		
 /*
        SetInlineItemActivation(
            (wgui_inline_items + 1),
            INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,
            0);
        SetInlineItemCaption(
            (wgui_inline_items + 0),
            (U8*) IP_address_desc);
        SetInlineItemActivation((wgui_inline_items + 3), INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
        SetInlineItemCaption((wgui_inline_items + 2), (U8*) Port_desc);

        buffer_size = 12;   //6
        SetInlineItemTextEdit(
            (wgui_inline_items + 3),
            (U8*) proxy_port,
            buffer_size,
            INPUT_TYPE_NUMERIC);
      VM_readProxyPort(&jme_proxy_port);
        gui_itoa((S32) jme_proxy_port, (U16*) proxy_port, 10);
 VM_readProxyIp(&jme_proxy_IP);
        VM_ReturnThreeDigitString(ip1, (U8)jme_proxy_IP);
        VM_ReturnThreeDigitString(ip2, (U8)(jme_proxy_IP>>8));
        VM_ReturnThreeDigitString(ip3, (U8)(jme_proxy_IP>>16));
        VM_ReturnThreeDigitString(ip4, (U8)(jme_proxy_IP>>24));

        SetInlineItemIP4(
            (wgui_inline_items + 1),
            (U8*) ip1,
            (U8*) ip2,
            (U8*) ip3,
            (U8*) ip4,
            VM_PreparedIPAddressString);
*/
    
}

static BOOL vm_tone_enable = TRUE;
BOOL VM_mmi_is_enable_qq_tone(void)
{
    return  vm_tone_enable;
}
void VM_mmi_disable_qq_tone(void)
{
    vm_tone_enable = FALSE;
}
void VM_mmi_reset_qq_tone(void)
{
    vm_tone_enable = TRUE;
}

#else
char vm_status;
char jmetoc_SIM = 0xff;
char jmetoc_cid = 0;
char* jmetoc_apn;
char jmetoc_apn_set [100] = {0x00,};
char jmetoc_user_set [100] = {0x00,};
char jmetoc_pwd_set [100] = {0x00,};
void exitSmarchBox(void)
{
}
 int smarchbox_attrsp;

void jmetoc_showlist() 
{
	return;
}

char VM_readSimNum(void)
{
     return 0;
}

void VM_writeSimNum(char simNum)
{
    return ;
}
char VM_readApnNum(void)
{
    return 0;
}

void VM_writeApnNum(char simNum)
{
    return ;
}
#endif
extern U8 r2MMIFlagCached;
extern BOOL r2lMMIFlag  ;
void jmetoc_SetAppStatus(BOOL bIsActive)
{
    if(bIsActive )
    {
        r2MMIFlagCached = r2lMMIFlag;
        r2lMMIFlag = FALSE;
    }
    else
    {
        if(r2MMIFlagCached!=0xff)
        {
            r2lMMIFlag = r2MMIFlagCached;
            r2MMIFlagCached = 0xff;
        }
    }
}

#endif

