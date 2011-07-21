/*
zhangruixian add 20060612
*/
// zrx add beign 20060612
#define __NEWSIMULATOR
#include "mmi_features.h"
 
#include "globalmenuitems.h"
 
#include "commonscreens.h"
#include "timerevents.h"
#include "keybrd.h"
#include "profilegprots.h"
#include "globaldefs.h"

#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "eventsgprot.h"

#include "gui_ems.h"
#include "hf_rules.h"
#include "cswtype.h"
#include "kal_release.h"
#include "nvram_defs.h"
#include "nvram_user_defs.h"
#include "fmt_struct.h"
#include "fmt_def.h"
//#include "fmt_main.h"
//#include "custom_mmi_default_value.h"
#include "kal_non_specific_general_types.h"
#include "app_ltlcom.h"
#include "mmi_trace.h"

#undef __NEWSIMULATOR
 
#include "sscstringhandle.h"
#include "custresdef.h"
#include "engineermodepatch.h"
#include "factorymodeprot.h"
#include "simdetectiondef.h"
#include "mmi_msg_struct.h"
#include "engineermodeprot.h"
#include "simdetectionhwmapfile.h"
#include "filesystemgprot.h"
#ifdef MMI_ON_HARDWARE_P		
#include "uctls_m.h"
#endif
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
#include "settinggprots.h"

extern UINT8 ssc_disable_flag;
extern void RstStartRestore_ClearuserData(void);
extern void EntrySWFactoryModeMenu(void);

typedef struct __rtc 
{
	kal_uint8		rtc_sec;    /* seconds after the minute   - [0,59]  */
	kal_uint8		rtc_min;    /* minutes after the hour     - [0,59]  */
	kal_uint8		rtc_hour;   /* hours after the midnight   - [0,23]  */
	kal_uint8		rtc_day;    /* day of the month           - [1,31]  */
	kal_uint8		rtc_mon;    /* months 		               - [1,12] */
	kal_uint8		rtc_wday;   /* days in a week 		      - [1,7] */
	kal_uint8		rtc_year;   /* years                      - [0,127] */
} t_rtc;
#define MAX_IMAGE_NAME_EXTENSION_WIDTH		(5*ENCODING_LENGTH)
#define MAX_IMAGE_NAME_WIDTH				((13*ENCODING_LENGTH)	+	MAX_IMAGE_NAME_EXTENSION_WIDTH + ENCODING_LENGTH)

typedef struct
{
	kal_uint8 ImageName[MAX_IMAGE_NAME_WIDTH];
	kal_int16 ImagesId;
}DYNIMAGEINFO;
#ifdef __MMI_MULTI_SIM__
S8    gstrIMEI[MMI_SIM_NUMBER][(MAX_IMEI_LEN+1) * ENCODING_LENGTH];
#else 
S8    gstrIMEI[(MAX_IMEI_LEN+1) * ENCODING_LENGTH];
#endif /* __MMI_MULTI_SIM__ */
//added by panxu on 20061222
#if 0
void AudioPlayReq(U16 soundId, U8 style)
{
}
#endif

 S32 AccessRecordInt
(
	U16 nFileId,
	U16 nRecordId, 
	void *pBuffer, 
	U16 nBufferSize,
	S16 *pError, 
	pBOOL isWrite, 
	S8* filename, 
	S32 lineNumber
);
 	
 typedef struct sscstringtblstruct 
{
	const S8 *ssc_string;
	const U8 ssc_disable_ctrl;
	FuncPtr func_ptr;
} SSCSTRINGTBL;

 void MMIProcessActionMsg(void)
 	{
 	}


// 获取当前时间.
// rtctime: 通过此参数返回系统时间.
void RTC_GetTime(t_rtc *rtctime)
{
	BOOL bRet = FALSE;		
	TM_SYSTEMTIME systemTime;

	memset(&systemTime, 0, sizeof(TM_SYSTEMTIME));
	bRet = TM_GetSystemTime(&systemTime);
	
	if (bRet == FALSE)
	{
		mmi_trace(g_sw_CC, "Fatal Error : Call TM_GetLocalTime(), Return FALSE!");
		return;
	}

	rtctime->rtc_year = systemTime.uYear - 2000;
	rtctime->rtc_mon  = systemTime.uMonth;
	rtctime->rtc_day  = systemTime.uDay;
	rtctime->rtc_wday = systemTime.uDayOfWeek;
	rtctime->rtc_hour = systemTime.uHour;
	rtctime->rtc_min  = systemTime.uMinute;
	rtctime->rtc_sec  = systemTime.uSecond;
}

 
/*
void SSCStringParsing1(void)
{
}
void SSCStringParsing2(void)
{
}
void SSCStringParsing3(void)
{
}
int FS_SetDiskFlag(void)
{
}
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------SSCHandleEngineerMode()
/**************************************************************

	FUNCTION NAME		:SSCHandleEngineerMode

  	PURPOSE				: EngineerMode Menu Operate

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void SSCHandleEngineerMode(void)
{
   //PRINT_INFORMATION(("This is to handle Engineering Mode"));
   
#ifdef __MMI_ENGINEER_MODE__  
   EntryEngineerModeMenu();
#else
   DisplayIdleScreen();
   DisplayPopup((U8*)GetString(SSC_SCR_ENGINEER_MODE_STR_ID),IMG_GLOBAL_WARNING,0,SSC_POP_UP_TIME,(U8)ERROR_TONE);
#endif
}
void SSCHandleIMEI(void)// man for imei
{
	MYQUEUE Message;
	ClearAllKeyHandler();
   
	Message.oslMsgId = MSG_ID_MMI_NW_GET_IMEI_REQ;
	Message.oslDataPtr = NULL;
    Message.oslPeerBuffPtr= NULL;
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	OslMsgSendExtQueue(&Message);
	SetProtocolEventHandler( get_imeirsp, MSG_ID_MMI_NW_GET_IMEI_RSP);

}
extern void SSCHandleSwitchPwrNokia(void);
/**************************************************************

	FUNCTION NAME		: ShowIMEIScreen

  	PURPOSE				: Display IMEI Number

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowIMEIScreen()// man for imei
{
	U8 disp_str[200];
	
#ifdef __MMI_MULTI_SIM__
	U8 ansii_tmp[10];
	U8 tmpstr[20];
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(i == 0)
		{
			memset(disp_str, 0, sizeof(disp_str));
			memset(tmpstr, 0, sizeof(tmpstr));
			memset(ansii_tmp, 0, sizeof(ansii_tmp));
			sprintf(ansii_tmp, "IMEI%d:\n", i);
			AnsiiToUnicodeString((S8 *)tmpstr, (S8 *)ansii_tmp);
			UCS2Strcat(disp_str, tmpstr);
			UCS2Strcat(disp_str, gstrIMEI[i]);
		}
		else
		{
			memset(tmpstr, 0, sizeof(tmpstr));
			memset(ansii_tmp, 0, sizeof(ansii_tmp));
			sprintf(ansii_tmp, "\nIMEI%d:\n", i);
			AnsiiToUnicodeString((S8 *)tmpstr, (S8 *)ansii_tmp);
			UCS2Strcat(disp_str, tmpstr);
			UCS2Strcat(disp_str, gstrIMEI[i]);
		}
	}
#else 
	memcpy(disp_str, gstrIMEI, sizeof(gstrIMEI));
#endif /* __MMI_MULTI_SIM__ */

	EntryNewScreen(SCR_ENTER_SSC, NULL, ShowIMEIScreen, NULL);
	
	ShowCategory152Screen(SSC_SCR_IMEI_CAPTION_ID, 0,
					  STR_GLOBAL_OK, 0,
					  0, 0,
					  (U8*)disp_str, NULL); 

	/* If user press SSC to see IMEI in a call, not display dial pad again if he press OK to leave IMEI screen */
	//if(isInCall())
		//DeleteScreenIfPresent(DTMF_SCR_DISPLAY_ID);

	SetLeftSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	//SetLeftSoftkeyFunction(GoBack2TheHistory, KEY_EVENT_UP);
}
/**************************************************************

	FUNCTION NAME		: get_imeirsp 

  	PURPOSE				: Response handler fn of request to get IMEI number.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void get_imeirsp(void * buf)
{
	mmi_nw_get_imei_rsp_struct  * imeiresponse = (mmi_nw_get_imei_rsp_struct *) buf;
#ifdef __MMI_MULTI_SIM__
	U8 i;
	memset(gstrIMEI,0,sizeof(gstrIMEI));
	
//	if( imeiresponse->result == 1)
	{
		for( i = 0; i < MMI_SIM_NUMBER; i++)
		{
		#ifdef __ASCII
			memcpy(gstrIMEI[i], (S8*)imeiresponse->imei[i],MAX_IMEI_LEN);
		#endif

		#ifdef __UCS2_ENCODING
			AnsiiNToUnicodeString(gstrIMEI[i],(S8*)imeiresponse->imei[i],MAX_IMEI_LEN);	
		#endif
		}
	}
#else 
	memset(gstrIMEI,0,(MAX_IMEI_LEN+1) * ENCODING_LENGTH);
	
//	if( imeiresponse->result == 1)
	{
	#ifdef __ASCII
		memcpy(gstrIMEI, (S8*)imeiresponse->imei,MAX_IMEI_LEN);
	#endif

	#ifdef __UCS2_ENCODING
		AnsiiNToUnicodeString(gstrIMEI,(S8*)imeiresponse->imei,MAX_IMEI_LEN);	
	#endif
	}
#endif /* __MMI_MULTI_SIM__ */
	ShowIMEIScreen();

}

//-----------------------------------------------SSCHandleFactoryMode()
/**************************************************************

	FUNCTION NAME		:SSCHandleFactoryMode

  	PURPOSE				: FactoryMode Menu Operate

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void SSCHandleFactoryMode(void)
{
   //PRINT_INFORMATION(("this is to handle Factory Mode"));
   
#ifdef __MMI_FACTORY_MODE__  
//Lisen 0630
   EntryFMMenu();
#else
   DisplayIdleScreen();
   DisplayPopup((U8*)GetString(SSC_SCR_FACTORY_MODE_STR_ID),IMG_GLOBAL_WARNING,0,SSC_POP_UP_TIME,(U8)ERROR_TONE);
#endif
}

void SSCHandleSWFactoryMode(void)
{
   EntrySWFactoryModeMenu();
}

/**************************************************************

	FUNCTION NAME		: SSCHandleFactoryModeAutoTest

  	PURPOSE				: Set Factory Mode 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
#include"settingdefs.h"
#include"settingsgdcl.h"

void SendReqToChangePwd(void);
void SendReqPIN1Change(void)
{
   /*wangrui modify 2008.11.02 start for fixbug 10192*/
	S16 *tempp ;
	S8 length = 0;
	S16 i = 0;
	S8 alertLength=0;
	
	mmi_trace(1," SendReqPIN1Change");
	memset(g_SecuritySetupContext.NewPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);
	memset(g_SecuritySetupContext.ConfirmPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);
	memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);

	tempp =  (S16 *)(g_idle_context.DialPadCallBuffer + strlen("**04*")*ENCODING_LENGTH);
	alertLength = strlen("**04*");
	

	for(i=0; *(tempp + i)!= 0x0 && *(tempp + i)!= '*';i++)
	{
		length =	length + 1;
		if(alertLength > MAX_DIAL_PAD-1) 
		{
		     length = MAX_DIAL_PAD;
			 break;
		}
		alertLength = alertLength +1;
	}
	 
	if(*(tempp + length)== 0x0||length < 4||length > 8 )
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}
	
 	memcpy(g_SecuritySetupContext.Password,tempp,length*ENCODING_LENGTH);
 	tempp = tempp + length + 1;
	length = 0;
	for(i=0; *(tempp + i) != 0x0 && *(tempp + i)!= '*'; i++)
	{
		length =	length + 1;
		if(alertLength > MAX_DIAL_PAD-1) 
		{
		     length = MAX_DIAL_PAD;
			 break;
		}
		alertLength = alertLength +1;
	}
	if((tempp + length)== 0x0 ||length < 4||length > 8)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}


  	memcpy(g_SecuritySetupContext.NewPassword,tempp,length*ENCODING_LENGTH);
	 tempp = tempp + length + 1;
	length = 0;
	for(i=0;*(tempp + i) != 0x0 && *(tempp + i) != '#';i++)
	{
		length =	length + 1;
		if(alertLength > MAX_DIAL_PAD-1) 
		{
		     length = MAX_DIAL_PAD;
			 break;
		}
		alertLength = alertLength +1;
	}

	if(length > 8)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}
     /*wangrui modify 2008.11.02 end*/	
   	memcpy(g_SecuritySetupContext.ConfirmPassword,tempp,length*ENCODING_LENGTH);
	g_SecuritySetupContext.CurrentPassword = 0;

	SendReqToChangePwd();
}

/*wangrui modify 2008.11.20 start for fixbug 10309*/
void SendReqPIN2Change(void)
{

	S8 length = 0;
	S16 i = 0;
	S8 alertLength=0;
	S16 *tempp;
	
	memset(g_SecuritySetupContext.NewPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);
	memset(g_SecuritySetupContext.ConfirmPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);
	memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);

	tempp =  (S16 *)(g_idle_context.DialPadCallBuffer + strlen("**042*")*ENCODING_LENGTH);
	alertLength = strlen("**042*");
	

	for(i=0; *(tempp + i)!= 0x0 && *(tempp + i)!= '*';i++)
	{
		length =	length + 1;
		if(alertLength > MAX_DIAL_PAD-1) 
		{
		     length = MAX_DIAL_PAD;
			 break;
		}
		alertLength = alertLength +1;
	}
	 
	if(*(tempp + length)== 0x0||length < 4||length > 8 )
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}
	
 	memcpy(g_SecuritySetupContext.Password,tempp,length*ENCODING_LENGTH);
 	tempp = tempp + length + 1;
	length = 0;
	for(i=0; *(tempp + i) != 0x0 && *(tempp + i)!= '*'; i++)
	{
		length =	length + 1;
		if(alertLength > MAX_DIAL_PAD-1) 
		{
		     length = MAX_DIAL_PAD;
			 break;
		}
		alertLength = alertLength +1;
	}
	if((tempp + length)== 0x0 ||length < 4||length > 8)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}


  	memcpy(g_SecuritySetupContext.NewPassword,tempp,length*ENCODING_LENGTH);
	 tempp = tempp + length + 1;
	length = 0;
	for(i=0;*(tempp + i) != 0x0 && *(tempp + i) != '#';i++)
	{
		length =	length + 1;
		if(alertLength > MAX_DIAL_PAD-1) 
		{
		     length = MAX_DIAL_PAD;
			 break;
		}
		alertLength = alertLength +1;
	}

	if(length > 8)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}
   	memcpy(g_SecuritySetupContext.ConfirmPassword,tempp,length*ENCODING_LENGTH);
	g_SecuritySetupContext.CurrentPassword = 1;

	SendReqToChangePwd();
}
        /*wangrui modify 2008.11.20 end*/	
/*****************************************************************************
 * FUNCTION
 *  SSCEntryPUK1
 * DESCRIPTION
 *  Enter PUK1, if verify ok PIN1 is set to a new PIN1
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SSCEntryPUK1(void)
{
	S8 length = 0;
	S16 i = 0;
	S8 alertLength=0;
	S16 *tempp;
	
	memset(g_SecuritySetupContext.NewPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);
	memset(g_SecuritySetupContext.ConfirmPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);
	memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);

	tempp =  (S16 *)(g_idle_context.DialPadCallBuffer + strlen("**05*")*ENCODING_LENGTH);
	alertLength = strlen("**05*");

	for(i=0; *(tempp + i)!= 0x0 && *(tempp + i)!= '*';i++)
	{
		length =	length + 1;
		if(alertLength > MAX_DIAL_PAD-1) 
		{
		     length = MAX_DIAL_PAD;
			 break;
		}
		alertLength = alertLength +1;
	}
	 
	if(*(tempp + length)== 0x0||length < 8)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}
	
 	memcpy(g_SecuritySetupContext.Password,tempp,length*ENCODING_LENGTH);
 	tempp = tempp + length + 1;
	length = 0;
	for(i=0; *(tempp + i) != 0x0 && *(tempp + i)!= '*'; i++)
	{
		length =	length + 1;
		if(alertLength > MAX_DIAL_PAD-1) 
		{
		     length = MAX_DIAL_PAD;
			 break;
		}
		alertLength = alertLength +1;
	}
	if((tempp + length)== 0x0 ||length < 4||length > 8)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}


  	memcpy(g_SecuritySetupContext.NewPassword,tempp,length*ENCODING_LENGTH);
	 tempp = tempp + length + 1;
	length = 0;
	for(i=0;*(tempp + i) != 0x0 && *(tempp + i) != '#';i++)
	{
		length =	length + 1;
		if(alertLength > MAX_DIAL_PAD-1) 
		{
		     length = MAX_DIAL_PAD;
			 break;
		}
		alertLength = alertLength +1;
	}

	if(length > 8)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}
   	memcpy(g_SecuritySetupContext.ConfirmPassword,tempp,length*ENCODING_LENGTH);
	g_SecuritySetupContext.CurrentPassword = 3;

	SendReqToChangePwd();
}

/*****************************************************************************
 * FUNCTION
 *  SSCEntryPUK2
 * DESCRIPTION
 *  Enter PUK2, if verify ok PIN2 is set to a new PIN2
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SSCEntryPUK2(void)
{
	S8 length = 0;
	S16 i = 0;
	S8 alertLength=0;
	S16 *tempp;
	
	memset(g_SecuritySetupContext.NewPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);
	memset(g_SecuritySetupContext.ConfirmPassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);
	memset(g_SecuritySetupContext.Password,0,(MAX_SIM_SETTING_PASSWD_LEN+1)*ENCODING_LENGTH);

	tempp =  (S16 *)(g_idle_context.DialPadCallBuffer + strlen("**052*")*ENCODING_LENGTH);
	alertLength = strlen("**052*");

	for(i=0; *(tempp + i)!= 0x0 && *(tempp + i)!= '*';i++)
	{
		length =	length + 1;
		if(alertLength > MAX_DIAL_PAD-1) 
		{
		     length = MAX_DIAL_PAD;
			 break;
		}
		alertLength = alertLength +1;
	}
	 
	if(*(tempp + length)== 0x0||length < 8)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}
	
 	memcpy(g_SecuritySetupContext.Password,tempp,length*ENCODING_LENGTH);
 	tempp = tempp + length + 1;
	length = 0;
	for(i=0; *(tempp + i) != 0x0 && *(tempp + i)!= '*'; i++)
	{
		length =	length + 1;
		if(alertLength > MAX_DIAL_PAD-1) 
		{
		     length = MAX_DIAL_PAD;
			 break;
		}
		alertLength = alertLength +1;
	}
	if((tempp + length)== 0x0 ||length < 4||length > 8)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}


  	memcpy(g_SecuritySetupContext.NewPassword,tempp,length*ENCODING_LENGTH);
	 tempp = tempp + length + 1;
	length = 0;
	for(i=0;*(tempp + i) != 0x0 && *(tempp + i) != '#';i++)
	{
		length =	length + 1;
		if(alertLength > MAX_DIAL_PAD-1) 
		{
		     length = MAX_DIAL_PAD;
			 break;
		}
		alertLength = alertLength +1;
	}

	if(length > 8)
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_ERROR, 0,ST_NOTIFYDURATION,ERROR_TONE);
		DeleteUptoScrID(IDLE_SCREEN_ID);
		return;
	}
   	memcpy(g_SecuritySetupContext.ConfirmPassword,tempp,length*ENCODING_LENGTH);
	g_SecuritySetupContext.CurrentPassword = 4;

	SendReqToChangePwd();
}
void EntryFMMenuAutoTest(void);
void SSCHandleFactoryModeAutoTest(void)
{
   //PRINT_INFORMATION(("this is to handle Factory Mode Auto Test"));
//CSD: Lisen added, for factory auto test mode
#ifdef __MMI_FACTORY_MODE__  
//Lisen 0630
   EntryFMMenuAutoTest();
#else
   DisplayIdleScreen();
   DisplayPopup((U8*)GetString(SSC_SCR_FACTORY_MODE_STR_ID),IMG_GLOBAL_WARNING,0,SSC_POP_UP_TIME,(U8)ERROR_TONE);
#endif
//CSD: Lisen end
}

void SSCHandleAutoTest(void)
{
	EntryAutoTest();
}

BOOL is_phb_init_done(void);
void EntryAutoTestCamera(void);
void SSCHandleAutoTestCamera(void)
{
	if(is_phb_init_done())
	{
		EntryAutoTestCamera();
	}
	else
	{//stringid=25803  开机初始化 modified by renyh  2008/8/1
		DisplayPopup((PU8)GetString(STR_GLOBAL_POWER_ON_PROCESSING) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		return;
	}
}

//wangrui add the function for set usb  download model
#ifdef MMI_ON_HARDWARE_P		
void HandleUSBDownloadModel(void)
{
    UCTLS_ID_T uctlsMode;

    uctlsMode = uctls_GetMode();
    if(UCTLS_ID_TRACE != uctlsMode)
    {
        uctls_SetMode(UCTLS_ID_TRACE);
        DisplayPopup((PU8)GetString(STR_GLOBAL_OPEN_USBDOWNLOAD ),
                     IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
        DeleteScreenIfPresent( IDLE_SCREEN_DIGIT_HANDLER_ID);
        DeleteScreenIfPresent(SCR_DIAL_SOS_NUM);

    }
    else
    {
        uctls_SetMode(UCTLS_ID_1);
        DisplayPopup((PU8)GetString(STR_GLOBAL_CLOSE_USBDOWNLOAD ),
                     IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
        DeleteScreenIfPresent( IDLE_SCREEN_DIGIT_HANDLER_ID);
        DeleteScreenIfPresent(SCR_DIAL_SOS_NUM);
    }
}
#else
void HandleUSBDownloadModel(void)
{
}
#endif

/**************************************************************

	FUNCTION NAME		: SSCHandleSoftwareVersionSummary

  	PURPOSE				: Display sofeware version.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void EntryFMVersionSummaryMenu(void);
void SSCHandleSoftwareVersionSummary(void)
{
#if !defined(__PROJECT_GALLITE_C01__)
#ifdef __MMI_FACTORY_MODE__  
   EntryFMVersionSummaryMenu();
#else
   DisplayIdleScreen();
   DisplayPopup((U8*)GetString(SSC_SCR_FACTORY_MODE_STR_ID),IMG_GLOBAL_WARNING,0,SSC_POP_UP_TIME,(U8)ERROR_TONE);
#endif   
#else
   EntryFMVersionSwMenu();
#endif   
}
#ifdef TGT_AUTOGEN_MODULE_NAME
extern void EntryFMHWVersionSummaryMenu();
void SSCHandleHardwareModuleSummary(void)
{
#ifdef __MMI_FACTORY_MODE__  
   EntryFMHWVersionSummaryMenu();
#else
   DisplayIdleScreen();
   DisplayPopup((U8*)GetString(SSC_SCR_FACTORY_MODE_STR_ID),IMG_GLOBAL_WARNING,0,SSC_POP_UP_TIME,(U8)ERROR_TONE);
#endif  
}
#endif
typedef struct simstringtblstruct 
{
	const S8 *sim_string;
	S8 sim_length;
	FuncPtr func_ptr;
} SIMSTRINGTBL;
#ifdef __MMI_BT_PROFILE__
U8 g_ssc_bt_a2dp_mode = 0;  /* 0: INT mode, 1: ACP mode, 2: ACP mode for PTS only */
#endif 
/*********************************************************
chenhe,the secret code should '*#00*' is deserved for our develop tests
others as before
**********************************************************/
#ifdef __MMI_ENGINEER_MODE__
#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)
extern void StartRunAutoTestCasesInTime(void);
extern void OpenRecordAutoTest(void);
extern void StartRunAutoTestCasesWaitEvents(void);
extern void StartRunAutoTestCasesWaitEventsWithTimerOut(void);
extern void mmi_autotest_entry_list();
extern void mmi_autotest_play_all_start();
#endif
#endif
extern void TestLCD(void);
#if defined(MMI_ON_HARDWARE_P)
#include "bal_fstrace.h" 
#endif
#include "vdoplystrenum.h"

extern BOOL checkTCardExist(void);
#ifdef UPDATE_SW
#include"filemanagerdef.h"

extern INT32 bal_updateSoftware(void(*updateCb)(UINT32 percents));
void showUpdateProcess(void)
{	U8* guiBuffer;

		EntryNewScreen(POPUP_SCREENID, NULL, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer (POPUP_SCREENID) ;
		ShowCategory72Screen(STR_GLOBAL_UPDATE_SOFTWARE,0,
						0,0,
						0, 0,
						(U8*)GetString(STR_GLOBAL_UPDATE_SOFTWARE), 0, guiBuffer);	


}
void refreshUpdateProcess(UINT32 precent)
{
	U8* guiBuffer;

	mmi_trace(1,"chenhe,refreshUpdateProcess,precent is %d",precent);
	#if 1
	guiBuffer = GetCurrGuiBuffer (POPUP_SCREENID) ;

	

	ShowCategory72Screen(STR_GLOBAL_UPDATE_SOFTWARE,0,
						0,0,
						0, 0,
						(U8*)GetString(STR_GLOBAL_UPDATE_SOFTWARE), precent, guiBuffer);	
	#endif
}
#ifdef MMI_ON_HARDWARE_P		
#include "mcd_m.h"

typedef struct _tst_drv_tflash
{   
    UINT16                         is_open;                                                  // device status,0:deactive,1,active.
    UINT8                           dev_name[16];           // device name.
    MCD_CSD_T                 mcd_csd;                                                 // MCD_CSD.

}TST_DRV_TFLASH;

#ifdef DUAL_TFLASH_SUPPORT
extern MCD_ERR_T mcd_Close(MCD_CARD_ID mcdId);
extern MCD_ERR_T mcd_Open(MCD_CARD_ID mcdId,MCD_CSD_T* mcdCsd, MCD_CARD_VER mcdVer);
extern MCD_ERR_T mcd_SwitchOperationTo(MCD_CARD_ID mcdId);
#else
extern MCD_ERR_T mcd_Close(VOID);
extern MCD_ERR_T mcd_Open(MCD_CSD_T* mcdCsd, MCD_CARD_VER mcdVer);
#endif

extern MCD_ERR_T mcd_Write(UINT32 startAddr, CONST UINT8* blockWr, UINT32 blockLength);
extern MCD_ERR_T mcd_Read(UINT32 startAddr, UINT8* rdDataBlock, UINT32 blockLength);
extern MCD_ERR_T mcd_GetCardSize(MCD_CARD_SIZE_T* size);
extern BOOL hal_HstSendEvent(UINT32 ch);

#define MCD_BLOCK_SIZE 512
MCD_CARD_ID g_tst_dev_id = 0;
TST_DRV_TFLASH g_tst_tflash[] = {{0,"mmc0",{0,}},{0,"mmc1",{0,}}};
UINT8 g_tst_mcd_buff[MCD_BLOCK_SIZE] = {0,};

void tst_mcd(UINT32 dev_id)
{
        MCD_ERR_T mcd_error = 0;
        TST_DRV_TFLASH* ptst_tflash = NULL;
        MCD_CSD_T  mcd_csd = {0,}; 
        MCD_CARD_SIZE_T mcd_size = {0,0};
        
        ptst_tflash = &g_tst_tflash[dev_id];
        UINT32 block_nr = 0;
        UINT32 i = 0;
        MCD_CARD_ID card_id;

        card_id = dev_id == 0 ? MCD_CARD_ID_0:MCD_CARD_ID_1;
        
        hal_HstSendEvent(0xD00000A0); 
        if(dev_id != g_tst_dev_id)
        {
#ifdef DUAL_TFLASH_SUPPORT
            mcd_error = mcd_SwitchOperationTo(card_id);
            if(MCD_ERR_NO == mcd_error)
            {
                hal_HstSendEvent(0xD0000A1);
                g_tst_dev_id = dev_id;
            }
            else
            {
                hal_HstSendEvent(0xD0000A2);  
                goto func_end;
            }
#endif    
        }
        
       if(1 == ptst_tflash->is_open)
       {
#ifdef DUAL_TFLASH_SUPPORT              
            mcd_error = mcd_Close(card_id);
#else
            mcd_error = mcd_Close();
#endif
            if(MCD_ERR_NO == mcd_error)
            {
                ptst_tflash->is_open = 0;
                ptst_tflash->mcd_csd = mcd_csd;
                hal_HstSendEvent(0xD0000a3);
            }
            else
            {
                hal_HstSendEvent(0xD0000a4);  
            }
        }
	   
#ifdef DUAL_TFLASH_SUPPORT       
        mcd_error = mcd_Open(card_id,&mcd_csd,MCD_CARD_V1);
#else
        mcd_error = mcd_Open(&mcd_csd,MCD_CARD_V1);
#endif

        if(MCD_ERR_NO == mcd_error)
        {
            ptst_tflash->is_open = 1;
            ptst_tflash->mcd_csd = mcd_csd;
            hal_HstSendEvent(0xD0000a5);
        }
        else
        {
            hal_HstSendEvent(0xD0000a6);              
        }

        mcd_error = mcd_GetCardSize(&mcd_size);
        if(MCD_ERR_NO == mcd_error)      
        {
            hal_HstSendEvent(0xD0000a7);
            hal_HstSendEvent(mcd_size.nbBlock);
            hal_HstSendEvent(mcd_size.blockLen);
        }
        else
        {
            hal_HstSendEvent(0xD0000a8);
        }

        block_nr = mcd_size.nbBlock*(mcd_size.blockLen/MCD_BLOCK_SIZE);
        hal_HstSendEvent(0xD0000a9);
        hal_HstSendEvent(block_nr);
        
        for(i = 0; i < 1024; i++);
        {
            mcd_error = mcd_Read(i,g_tst_mcd_buff,MCD_BLOCK_SIZE);
            if(MCD_ERR_NO == mcd_error)      
            {
            
            }
            else
            {
                hal_HstSendEvent(0xD0000aa);
                hal_HstSendEvent(i);
                goto func_end;
            }
            mcd_error = mcd_Write(i,g_tst_mcd_buff,MCD_BLOCK_SIZE);
            if(MCD_ERR_NO == mcd_error)      
            {
                
            }
            else
            {
                hal_HstSendEvent(0xD0000ab);
                hal_HstSendEvent(i);
                goto func_end;
            }
        }
        hal_HstSendEvent(0xD0000ac);
#ifdef DUAL_TFLASH_SUPPORT                     
        mcd_error = mcd_Close(card_id);
#else
        mcd_error = mcd_Close();
#endif
        if(MCD_ERR_NO == mcd_error)
        {
            ptst_tflash->is_open = 0;
            ptst_tflash->mcd_csd = mcd_csd;
            hal_HstSendEvent(0xD0000ad);
        }
        else
        {
            hal_HstSendEvent(0xD0000ae);  
            goto func_end; 
        }
        hal_HstSendEvent(0xD00000AF); 
        return;
func_end:
    hal_HstSendEvent(0xD00000AE);
    return;
}
#endif

void tst_mcd_mmc0(void)
{
#ifdef MMI_ON_HARDWARE_P		

     tst_mcd(0);
#endif
}

void tst_mcd_mmc1(void)
{
#ifdef MMI_ON_HARDWARE_P		

     tst_mcd(1);
#endif
}

void updateSoftware(void)
{
	INT32 result =  0;
	result=bal_updateSoftware(refreshUpdateProcess);
	if(result == 0)
	{
		showUpdateProcess();

	}
	else
	{
		switch(result)
			{
				case -1:
					DisplayPopup((PU8)GetString(FMGR_FS_FAIL_GET_MEM_TEXT) ,
					IMG_GLOBAL_PROGRESS, 5,
					UI_POPUP_NOTIFYDURATION_TIME,
					WARNING_TONE);
				break;
				case -2:
					DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR) ,
					IMG_GLOBAL_PROGRESS, 5,
					UI_POPUP_NOTIFYDURATION_TIME,
					WARNING_TONE);
					break;
				case -3:
					DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR) ,
					IMG_GLOBAL_PROGRESS, 5,
					UI_POPUP_NOTIFYDURATION_TIME,
					WARNING_TONE);
					break;
				default:
					DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR) ,
					IMG_GLOBAL_PROGRESS, 5,
					UI_POPUP_NOTIFYDURATION_TIME,
					WARNING_TONE);
					break;
			}
	}
	mmi_trace(1,"chenhe,updateSoftware,result is %d",result);

		
}
#endif

void tst_mmi_read_hide_sector(void)
{
 #ifndef MMI_ON_WIN32
       FS_Dump("mmc0",0,99,NULL);
 #endif      
}
void tst_mmi_mount_t(void)
{
       UINT8 folderName[48];
	INT32 result=-1;
	AnsiiToUnicodeString(folderName,"/t");
	result=FS_MakeDir(folderName,0);
      if(result == 0 || ERR_FS_FILE_EXIST ==result)
       {
           mmi_trace(1,"tst_mmi_mount_t mkdir ok.");	
       }
       else
       {
           mmi_trace(1,"tst_mmi_mount_t mkdir fail,err_code = %d.",result);	
       }
	result = FS_HasFormatted("mmc0", FS_TYPE_FAT) ;
	if(result !=ERR_FS_HAS_FORMATED)
	{
		mmi_trace(1,"tst_mmi_mount_t,mmc0 is not formated.err_code = %d.",result);
	}
      else
       {
           mmi_trace(1,"tst_mmi_mount_t mmc0 has formated.");	
       }
	result = FS_Mount("mmc0", folderName,0, FS_TYPE_FAT );
       if(result == 0)
       {
           mmi_trace(1,"tst_mmi_mount_t ok.");	
       }
       else
       {
           mmi_trace(1,"tst_mmi_mount_t failed,err_code = %d.",result);	
       }
}
void mmi_fstraceStart(void)
{
	if(checkTCardExist())
	{
		bal_fstraceStart();
		DisplayPopup((PU8)GetString(STR_GLOBAL_TFLASH_TRACE_START) ,
			IMG_GLOBAL_PROGRESS, 5,
			UI_POPUP_NOTIFYDURATION_TIME,
			WARNING_TONE);
		
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD) ,
			IMG_GLOBAL_PROGRESS, 5,
			UI_POPUP_NOTIFYDURATION_TIME,
			WARNING_TONE);
	}
}
void mmi_fstraceStop(void)
{
	bal_fstraceStop();
	DisplayPopup((PU8)GetString(STR_GLOBAL_TFLASH_TRACE_STOP) ,
	IMG_GLOBAL_PROGRESS, 5,
	UI_POPUP_NOTIFYDURATION_TIME,
	WARNING_TONE);
}

const SSCSTRINGTBL  ssc_table1[]=
{
	{"*#06#",			MMI_FALSE, SSCHandleIMEI},//man for imei
	{SSC_ENGINEERING_MODE,		MMI_TRUE,  SSCHandleEngineerMode},
	{SSC_FACTORY_MODE,		MMI_TRUE,  SSCHandleFactoryMode},
#if defined(__PROJECT_GALLITE_C01__)
	{SSC_SW_FACTORY_MODE,		MMI_TRUE,  SSCHandleSWFactoryMode},
#else
	{SSC_FACTORY_MODE_AUTOTEST,MMI_TRUE,SSCHandleFactoryModeAutoTest},
#endif
	{SSC_SW_VERSION,MMI_FALSE,SSCHandleSoftwareVersionSummary},
     #ifdef TGT_AUTOGEN_MODULE_NAME
	{SSC_HW_MODULE,MMI_FALSE,SSCHandleHardwareModuleSummary},
     #endif
	{"*#88#",MMI_TRUE,SSCHandleAutoTest},
	{"*#5678#",MMI_TRUE,SSCHandleAutoTestCamera},
	{"*#3696#",MMI_TRUE,HandleUSBDownloadModel},//wangrui add for set usb  download model
#ifdef MMI_MEMLEAK_CHK	
	{"*#001#",MMI_TRUE,mmi_memleakCHK},
#endif
	{"*#002#",MMI_TRUE,RstStartRestore_ClearuserData},
	{"*#110#",MMI_TRUE,TestLCD},
#ifdef __MMI_ENGINEER_MODE__
#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)
	{"*#11#",MMI_TRUE,OpenRecordAutoTest},
	{"*#12#",MMI_TRUE,StartRunAutoTestCasesInTime},
	{"*#13#",MMI_TRUE,StartRunAutoTestCasesWaitEvents},	
	{"*#14#",MMI_TRUE,StartRunAutoTestCasesWaitEventsWithTimerOut},
	{"*#21#",MMI_TRUE,mmi_autotest_entry_list},
	{"*#22#",MMI_TRUE,mmi_autotest_play_all_start},
#endif
#endif
	{"*#003#",MMI_TRUE,enableSeeMMSFile},
	{"*#004#",MMI_TRUE,disableSeeMMSFile},
	#if 1
	//belowing is to start print trace to tflash card
	{"*#005#",MMI_TRUE,mmi_fstraceStart},
	{"*#006#",MMI_TRUE,mmi_fstraceStop},
#endif//__FS_TRACE_SUPPORT__
#ifdef UPDATE_SW	
	{"*#007#",MMI_TRUE,updateSoftware},
#endif
       {"*#008#",MMI_TRUE,tst_mmi_mount_t},
       {"*#009#",MMI_TRUE,tst_mmi_read_hide_sector},
       {"*#228#",MMI_TRUE,tst_mcd_mmc0},
       {"*#229#",MMI_TRUE,tst_mcd_mmc1},
       
#if defined(__MMI_NOKIA_STYLE_N800__)
        {"*#2010*#",MMI_TRUE,SSCHandleSwitchPwrNokia}
#endif
};

#ifdef __MMI_MULTI_SIM__
extern void 	mmi_entry_gsm_change_pin1_select_sim(void);
extern void 	mmi_entry_gsm_change_pin2_select_sim(void);
extern void 	mmi_ssc_entry_puk1_select_sim(void);
extern void 	mmi_ssc_entry_puk2_select_sim(void);
#endif

const SIMSTRINGTBL  sim_table[]=
{
#ifdef __MMI_MULTI_SIM__
	{"**04*", 5, mmi_entry_gsm_change_pin1_select_sim},
	{"**042*", 6, mmi_entry_gsm_change_pin2_select_sim},
	{"**05*", 5, mmi_ssc_entry_puk1_select_sim},
	{"**052*", 6, mmi_ssc_entry_puk2_select_sim}
#else
	{"**04*", 5, SendReqPIN1Change},
	{"**042*", 6, SendReqPIN2Change},
	{"**05*", 5, SSCEntryPUK1},
	{"**052*", 6, SSCEntryPUK2}
#endif
};




//-----------------------------------------------
/**************************************************************

	FUNCTION NAME		:SSCStringParsing1

  	PURPOSE				: Parsing Parameter Strring

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void SSCStringParsing1(VOID )
{
	UINT8 index=0;

#ifdef __UCS2_ENCODING
	UINT8 temp_str[MAX_SSC_STRING];
#endif

	UINT8 *pTemp;

	for (index = 0; index < (sizeof(ssc_table1) / sizeof(ssc_table1[0])); index++)
	{

#ifdef __UCS2_ENCODING
		AnsiiToUnicodeString((S8*)temp_str, (S8*)ssc_table1[index].ssc_string);
		pTemp = (UINT8 *)temp_str;
#endif
#ifdef __ASCII
		pTemp = (UINT8 *)ssc_table1[index].ssc_string;
#endif

		if (pfnUnicodeStrcmp((S8*)pTemp, (S8*)g_idle_context.DialPadCallBuffer) == 0)
			if (ssc_table1[index].func_ptr != NULL)
			{
				if (ssc_disable_flag&&ssc_table1[index].ssc_disable_ctrl) 
				{
					return;
				}
				ssc_table1[index].func_ptr();
				return;
			}
	}
 
/*
#ifdef MMI_ON_HARDWARE_P

#ifdef DEBUG_BUF
	AnsiiToUnicodeString((S8*)temp_str, (S8*)MEM_CTRL_BUF_DBG);
	if(pfnUnicodeStrncmp((const S8*)temp_str, (const S8*)g_idle_context.DialPadCallBuffer, (UINT32 )pfnUnicodeStrlen((const S8*)temp_str))==0)
	{
		UINT8 temp_str2[100], temp_str3[100];
		UINT32 value;
		extern VOID kal_monitor_buffer(kal_uint32 index);

		memset(temp_str2, 0, sizeof(temp_str2));
		pfnUnicodeStrncpy((S8*)temp_str2, 
			(const S8*)&g_idle_context.DialPadCallBuffer
				[ENCODING_LENGTH*(pfnUnicodeStrlen(MEM_CTRL_BUF_DBG)-1)],
			(UINT32 )(pfnUnicodeStrlen(g_idle_context.DialPadCallBuffer)
				-pfnUnicodeStrlen(MEM_CTRL_BUF_DBG)));
#ifdef __UCS2_ENCODING
		UnicodeToAnsii((S8*)temp_str3, (S8*)temp_str2);
		value=atoi((S8*)temp_str3);
#else
		value=atoi((S8*)temp_str2);
#endif
		DisplayPopup((UINT8 *)temp_str2, IMG_GLOBAL_WARNING, 0, SSC_POP_UP_TIME, (UINT8 )ERROR_TONE);
		kal_monitor_buffer(value);

		return ;
	}
	AnsiiToUnicodeString((S8*)temp_str, (S8*)MEM_CTRL_BUF_QRY);
	if(pfnUnicodeStrcmp((const S8*)temp_str, (const S8*)g_idle_context.DialPadCallBuffer)==0)
	{
		UINT8 temp_str[100], temp_str2[100];
		extern kal_uint32 buff_pool_debug_mask_g[];

		sprintf((S8*)temp_str, "CtrlBuffer=%d", buff_pool_debug_mask_g[0]);
#ifdef __UCS2_ENCODING
		AnsiiToUnicodeString((S8*)temp_str2, (S8*)temp_str);
		DisplayPopup((UINT8 *)temp_str2, IMG_GLOBAL_WARNING, 0, SSC_POP_UP_TIME, (UINT8 )ERROR_TONE);
#else
		DisplayPopup((UINT8 *)temp_str, IMG_GLOBAL_WARNING, 0, SSC_POP_UP_TIME, (UINT8 )ERROR_TONE);
#endif
		SetLeftSoftkeyFunction(DisplayIdleScreen, KEY_EVENT_UP);
		return ;
	}
#endif

	AnsiiToUnicodeString((S8*)temp_str, (S8*)TASK_CTRL_BUF_DBG);
	if(pfnUnicodeStrncmp((const S8*)temp_str, 
		(const S8*)g_idle_context.DialPadCallBuffer,
		(UINT32 )pfnUnicodeStrlen((const S8*)temp_str))==0)
	{
		UINT8 temp_str2[100], temp_str3[100];
		UINT32 value;
		//extern kal_uint32 task_debug_mask_g[];
		 
		kal_uint32 task_debug_mask_g[5];

		memset(temp_str2, 0, sizeof(temp_str2));
		pfnUnicodeStrncpy((S8*)temp_str2,
			(const S8*)&g_idle_context.DialPadCallBuffer
				[ENCODING_LENGTH*(pfnUnicodeStrlen(TASK_CTRL_BUF_DBG)-1)],
			(UINT32 )(pfnUnicodeStrlen(g_idle_context.DialPadCallBuffer)
				-pfnUnicodeStrlen(TASK_CTRL_BUF_DBG)));
#ifdef __UCS2_ENCODING
		UnicodeToAnsii((S8*)temp_str3, (S8*)temp_str2);
		value=atoi((S8*)temp_str3);
#else
		value=atoi((S8*)temp_str2);
#endif
		DisplayPopup((UINT8 *)temp_str2,
			IMG_GLOBAL_WARNING,
			0,
			SSC_POP_UP_TIME,
			(UINT8 )ERROR_TONE);
		task_debug_mask_g[0]=value;
		return ;
	}
	AnsiiToUnicodeString((S8*)temp_str, (S8*)TASK_CTRL_BUF_QRY);
	if(pfnUnicodeStrcmp((const S8*)temp_str, (const S8*)g_idle_context.DialPadCallBuffer)==0)
	{
		UINT8 temp_str[100], temp_str2[100];
		//extern kal_uint32 task_debug_mask_g[];
		 
		kal_uint32 task_debug_mask_g[5];

		sprintf((S8*)temp_str, "Taskdbgmask=%d", task_debug_mask_g[0]);
#ifdef __UCS2_ENCODING
		AnsiiToUnicodeString((S8*)temp_str2, (S8*)temp_str);
		DisplayPopup((UINT8 *)temp_str2,
			IMG_GLOBAL_WARNING,
			0,
			SSC_POP_UP_TIME,
			(UINT8 )ERROR_TONE);
#else
		DisplayPopup((UINT8 *)temp_str,
			IMG_GLOBAL_WARNING,
			0,
			SSC_POP_UP_TIME,
			(UINT8 )ERROR_TONE);
#endif
		SetLeftSoftkeyFunction(DisplayIdleScreen, KEY_EVENT_UP);
		return ;
	}
#endif
*/
}
//-----------------------------------------------------------------------------

#include "fontres.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "simdetectiongexdcl.h"
extern const sLanguageDetails CSD_gLanguageArray[];
extern const U16 CSD_gMaxDeployedLangs;
extern U16 gCurrLangIndex;
extern void RefreshNwProviderName(U8 *UpdatePlmn, U8 simID);
BOOL SSCStringChangeLanguage(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 index = 0;
	
#ifdef __UCS2_ENCODING
	U8 temp_str[MAX_SSC_STRING];
#endif 
	U8 *pTemp;
	U8 i;
	S16 error;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	for (index = 0; index < CSD_gMaxDeployedLangs; index++)
	{
	#ifdef __UCS2_ENCODING
		AnsiiToUnicodeString((S8*) temp_str, (S8*) CSD_gLanguageArray[index].aLangSSC);
		pTemp = (U8*) temp_str;
	#endif /* __UCS2_ENCODING */ 

	#ifdef __ASCII
		pTemp = (U8*) CSD_gLanguageArray[index].aLangSSC;
	#endif 

		if (pfnUnicodeStrcmp((S8*) pTemp, (S8*) g_idle_context.DialPadCallBuffer) == 0)
		{
			if (gCurrLangIndex != index)
			{
				SetCurrentLanguage(index);
				WriteValue(NVRAM_SETTING_LANG, &index, DS_BYTE, &error);
				
				for(i = 0; i < MMI_SIM_NUMBER; i++)
				{
					RefreshNwProviderName(gPLMN[i], i);
				}

				//SendLangChange2AT(index);

			#ifdef __MMI_SUBLCD__
				switch (g_idle_context.IsSubLcdOnIdleScreen)
				{
					case SUBLCD_ON_IDLE_NORMAL:
						ShowIdleAppSubLcd();
						break;
					case SUBLCD_ON_IDLE_INSERT_SIM:
						ShowSubLCDSimError();
						break;
					case SUBLCD_ON_IDLE_SIM_BLOCKED:
						ShowSubLCDBlockedSIM();
						break;
					case SUBLCD_NOT_ON_IDLE:
						break;
					default:
						ASSERT(0);
				}
			#endif /* __MMI_SUBLCD__ */ 
			}
			DisplayPopup(
						(U8*) GetString(STR_GLOBAL_DONE),
						IMG_GLOBAL_ACTIVATED,
						1,
						SSC_POP_UP_TIME,
						(U8) SUCCESS_TONE);
			mmi_trace(1, "SSCStringChangeLanguage, current langua set to index %d", index);
			return MMI_TRUE;
		}
	}
	return MMI_FALSE;
}
BOOL SSCStringParsing2(void)//man
{
	if(SSCStringChangeLanguage())
		return TRUE;
	return FALSE;
}
 BOOL SSCStringParsing3(void)
{
   U8    index=0;
#ifdef __UCS2_ENCODING
   U8    temp_str[MAX_SSC_STRING];
#endif
   U8    *pTemp;

   for (index=0; index< (sizeof(sim_table)/sizeof(sim_table[0])); index++)
   {
#ifdef __UCS2_ENCODING
      AnsiiToUnicodeString((S8*)temp_str, (S8*)sim_table[index].sim_string);
      pTemp = (U8 *) temp_str;
#endif
#ifdef __ASCII
      pTemp = (U8 *) sim_table[index].sim_string;
#endif

      if (pfnUnicodeStrncmp((S8*) pTemp, (S8*) g_idle_context.DialPadCallBuffer, sim_table[index].sim_length)==0)
	  {
         if (sim_table[index].func_ptr!=NULL)
            sim_table[index].func_ptr();
		  return MMI_TRUE;
	  }
   }

     return MMI_FALSE;

}

int FS_SetDiskFlag(void)
{
	return 0;
}
//---------------------------------------------------------

/*WUZC Del Start For MMIFS Ver:    on 2007-1-12 15:51 */
#if 0
void mmi_fmgr_select_path_and_enter (U8 app_id, U8 sel_type, FMGR_FILTER filter, PS8 start_path, PsExtFuncPtr callback)
{
}
#endif
/*WUZC Del End  For MMIFS Ver:    on 2007-1-12 15:51 */

// TODO: ...此函数的实现未被确认...
kal_bool is_test_sim(void)
{
	return FALSE;
}

U8 GetFullScreenFactoryImagesInfo(DYNIMAGEINFO ** factoryFileInfo)
{
	return 0;
}

// zrx add end
