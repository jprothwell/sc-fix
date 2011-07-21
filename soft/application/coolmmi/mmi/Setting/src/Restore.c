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
 * Restore.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for restore factory default application
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: Restore.c

  	PURPOSE		: nil

 

 

	DATE		: 11/14/2003

**************************************************************/
#ifndef _RESTORE_C
#define _RESTORE_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "l4dr.h"//__reset_factory__
#include "mmi_data_types.h"
#include "wgui_categories.h"
#include "commonscreens.h"
#include "miscfunctions.h"
#include "eventsgprot.h"
#include "custdatares.h"
#include "custmenures.h"
#include "debuginitdef.h"
#include "audioinc.h"
#include "historygprot.h"
#include "mainmenudef.h"
#include "unicodexdcl.h"
#include "protocolevents.h"
#include "queuegprot.h"
#include "globaldefs.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "idleappdef.h"
#include "settingprofile.h"
#include "settingsgexdcl.h"
#include "settinggprots.h"
#include "settingprot.h"
#include "screensaverdefs.h"
#include "wpssprotos.h"
#include "scheduledpoweronoffgprot.h"
#include "phonebookgprot.h"
#include "callsetupenum.h"
#include "simdetectiongexdcl.h"
#include "phonesetupgprots.h"
#include "mmi_trace.h"

#ifdef __MOD_SMSAL__
#include "messagesexdcl.h"
#endif

//micha0420
#include "settingdefs.h"
#include "settingsgdcl.h"

#undef __NEWSIMULATOR 
 
#include "wgui_categories_popup.h"
#include "wgui_categories_inputs.h"
#ifdef JATAAYU_SUPPORT
#include "jmms_str_id.h"
#endif


//#define __RESET_FACTORY__   // zrx del 20060612

#ifdef __RESET_FACTORY__
 
#include "alarmframeworkprot.h"
#endif

#include "mdi_datatype.h"
#include "adp_fs.h"
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
** Global Variable
*/
//micha0420
//extern U8 gpin2;
extern U8 CurrentDtType;
//micha0420
//extern U16 Isfdl,IsDelete,Isbdl;
extern U8 gCallCost;
//micha0420
//extern U8 gPINLocked;
extern U8 gIsSimValid;


//micha0420,  change all MAX_SIM_SETTING_PASSWD_LEN in this file
U8 gPhonePassword[(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH];



/* 
** Global Function
*/
extern pBOOL SetProfileDefault(void);
extern void EntryScrnSimBlockRsp(void);
#ifdef JATAAYU_SUPPORT
#ifdef JATAAYU_SUPPORT_MMS
	extern INT8 jMMS_IsMMSAppOrDownloadActive(void);
#endif
#ifdef JATAAYU_SUPPORT_WAP
	extern  INT8 jMMS_IsWapConnected (void);
#endif
#endif

//micha0420
//extern void DeleteFdlist(void);
//extern void NewFdlEntryReq(void);
//extern void EntryNewBdlReq(void);
//extern void DeleteBdlist(void);
//extern void FillCountDownNumber( sim_chv_info_struct	chv_info);
//extern void RspPin2ForCallCostSuccess(void);

#ifdef __MMI_PREFER_INPUT_METHOD__
extern void SetInputMethodType(U16 inputType);
#endif 

#ifdef __MMI_EBOOK_READER__
#include "ebookmanager.h"
#endif /* __MMI_EBOOK_READER__ */

mdi_result mdi_audio_reset_background_handler(U8 level);

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetMainMenuStyle
* DESCRIPTION
*   This function is Highlight handler for  "Resotre"
*   Follow functions: EntryRstScr
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightRstScr(void)
{
	TRACE_SYS_FUNCTION();
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	/* Register function for left/right soft key */
	SetLeftSoftkeyFunction(EntryRstScr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* Register function for left/right arrow keys */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(EntryRstScr/*0*/, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);//dongwg modify
	return;
}


/*****************************************************************************
* FUNCTION
*  EntryRstScr
* DESCRIPTION
*   This function is Entry function for "Restore"
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryRstScr(void)
{
#ifdef __HALF_SECURE_RESTORE__
     
	if(!IsPhoneLock())
	   EntryRstConfirm();
	else
	   EntryRstCheckPhoneLock();
	
#else  /* Check phone lock code before restore default */

     EntryRstCheckPhoneLock();

#endif

}



/*****************************************************************************
* FUNCTION
*  EntryRstCheckPhoneLock
* DESCRIPTION
*   This function is Entry function for Restore->Enter Phone Lock Code
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryRstCheckPhoneLock(void)
{
	
	U8* guiBuffer;       /* Buffer holding history data */

	TRACE_SYS_FUNCTION();
    PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== EntryRstCheckPhoneLock() ===\n"));

	/* Call Exit Handler*/
	EntryNewScreen(SCR_RESTORE, NULL, EntryRstScr, NULL);
	
	/* Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_RESTORE);	
	
	/* check the screen flow is from forward or backward */
	if ( guiBuffer == NULL)
		memset(gPhonePassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1));
	
	/* register the input validation function handler */
	RegisterInputBoxValidationFunction(RstrSettingPasswdScrSoftKeyFunc);
	
	/* display the corresponding catergory function */
	ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_GLOBAL_BACK, 
							IMG_GLOBAL_BACK,
							STR_RESTORE_PHONE,
							INPUT_TYPE_NUMERIC_PASSWORD,
							gPhonePassword,
							(MAX_SIM_SETTING_PASSWD_LEN+1) ,NULL, NULL);
	
	/* Register function with left/right softkey */
	SetCategory111RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(RstVerifyPhoneLockReq,KEY_EVENT_UP);
#ifdef TGT_GALLITE_G800
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
	ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
	/* register the key handler function for the pound key */
	SetKeyHandler(RstVerifyPhoneLockReq,KEY_POUND,KEY_EVENT_UP);
//micha0606
	g_SecuritySetupContext.PasswdLenCheckCallBack = RstVerifyPhoneLockReq;
	if(pfnUnicodeStrlen((S8*)gPhonePassword) < MIN_PASSWORD_LENGTH) {
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
	}
	

}



/*****************************************************************************
* FUNCTION
*  RstVerifyPhoneLockReq
* DESCRIPTION
 
*                                               for Restore->Enter Phone Lock Code
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void RstVerifyPhoneLockReq(void)
{
 	historyNode *History;
	U8 tempBuffer[(MAX_SIM_SETTING_PASSWD_LEN+1)];
	MYQUEUE	Message; 
	MMIPHONEVERIFYREQ* local_data;

   PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstVerifyPhoneLockReq() ===\n"));

	 
	if(pfnUnicodeStrlen((S8*)gPhonePassword) < MIN_PASSWORD_LENGTH)
	{
		/* if the password is invalid display error popup */
		DisplayPopup((U8*)GetString(STR_INVALID_PINPUKPLOCK_LENGTH),IMG_GLOBAL_WARNING,0,ST_NOTIFYDURATION,(U8)ERROR_TONE);
		if(GetHistoryScrID(SCR_RESTORE,&History)==ST_SUCCESS)
		{
			if(History->inputBuffer!=NULL)
				memset(History->inputBuffer,0,ENCODING_LENGTH);
			if(History->guiBuffer!=NULL)
				memset(History->guiBuffer,0,2);
		}
		memset(gPhonePassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1));
		DeleteUptoScrID(SCR_RESTORE);		
	}
	else{

		ClearAllKeyHandler();
		 
		Message.oslSrcId = MOD_MMI;
		Message.oslDestId = MOD_L4C;
//micha0420
		Message.oslMsgId = PRT_MMI_SMU_VERIFY_PIN_REQ;  
		local_data = (MMIPHONEVERIFYREQ*) OslConstructDataPtr(sizeof 
						(MMIPHONEVERIFYREQ));
		memset(local_data->pin,'\0',(MAX_SIM_SETTING_PASSWD_LEN+1));
		memset(local_data->new_pin,'\0',(MAX_SIM_SETTING_PASSWD_LEN+1));
		local_data->pin_type = MMI_SETTING_TYPE_PHONELOCK;
		/*  Change gInputPhoneLockBuffer to ASCII then copy local_data->pin*/
		memset(tempBuffer,0,(MAX_SIM_SETTING_PASSWD_LEN+1));
		UnicodeNToAnsii((S8*)tempBuffer,(S8*)gPhonePassword,(MAX_SIM_SETTING_PASSWD_LEN+1) );		
		memcpy(local_data->pin,tempBuffer,(MAX_SIM_SETTING_PASSWD_LEN+1)); 
		Message.oslDataPtr = (oslParaType *)local_data;
		Message.oslPeerBuffPtr= NULL;
		/* set the protocol handler to handle the response of this request */

//micha0420
		g_SecuritySetupContext.VerifyPassSuccessCallBack = EntryRstConfirm;
		SetProtocolEventHandler(SECSETVerifyPINRsp, PRT_MMI_SMU_VERIFY_PIN_RSP);
		OslMsgSendExtQueue(&Message);
	}
	return;
}


//micha0420  delete RstSmuVerifyPINRspHdlr
//

//


/*****************************************************************************
* FUNCTION
*  RstSendRestoreMsgToWap
* DESCRIPTION
*   This function is to 
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void RstSendRestoreMsgToWap(void)
{
#ifdef WAP_SUPPORT
	MYQUEUE Message;

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_WAP;
	Message.oslMsgId = MSG_ID_WAP_RESTORE_FACTORY_SETTING_REQ;

     Message.oslDataPtr = NULL;
	Message.oslPeerBuffPtr = NULL;
	OslMsgSendExtQueue(&Message);
#endif
}





/*****************************************************************************
* FUNCTION
*  EntryRstConfirm
* DESCRIPTION
*   This function is Entry function for Restore->PhoneLockCode->RuSure?
*   Following functions: RstStartRestore
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryRstConfirm(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						    STR_GLOBAL_NO, IMG_GLOBAL_NO,
						    get_string(STR_SETTING_RESTORE_CONFIRM),IMG_GLOBAL_QUESTION, WARNING_TONE);


	/* Register function with left/right softkey */
#ifdef __HALF_SECURE_RESTORE__
	if(!IsPhoneLock())
	  SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	else
	  SetRightSoftkeyFunction(GoBack2TheHistory,KEY_EVENT_UP);
#else
	SetRightSoftkeyFunction(GoBack2TheHistory,KEY_EVENT_UP);
#endif

	SetLeftSoftkeyFunction(RstStartRestore,KEY_EVENT_UP);

	/* Register Exit handler */
   //SetGenericExitHandler(SCR_SETTING_RESTORE_CONFIRM, NULL, EntryRstConfirm);

	return;
}


/*****************************************************************************
* FUNCTION
*  EntryRstRestoreProcessing
* DESCRIPTION
*   This function is Entry function to display restore processing screen
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryRstRestoreProcessing(void)
{
   PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== EntryRstRestoreProcessing() ===\n"));
   
// Beginning--modified by kecx for fixing bug 14995 on 20091030
   mdi_audio_reset_background_handler(0);
   mdi_audio_reset_background_handler(1);
   mdi_audio_reset_background_handler(2);
// end--modified by kecx for fixing bug 14995 on 20091030

   /* Call Exit Handler*/
   EntryNewScreen(SCR_RESTORE_PROCESSING, NULL, EntryRstRestoreProcessing, NULL);
   
   ShowCategory66Screen(STR_MENU_RESTORE,0, 0, 0, 0, 0, (PU8)GetString(STR_SETTING_RESTORE_PROCESSING), IMG_GLOBAL_PROGRESS, NULL);

   /* clear all key handlers so as to restrict duplicate reqs */		
   ClearAllKeyHandler();
   ClearKeyHandler(KEY_END,KEY_EVENT_UP);
   ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
   ClearKeyHandler(KEY_END,KEY_LONG_PRESS);
}

/*****************************************************************************
* FUNCTION
*  RstStartRestore
* DESCRIPTION
*   This function is start to restore settings to factore default
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
pBOOL bStatus = FALSE;
#ifdef __RESET_FACTORY__
#include "nvramtype.h"
#include "nvramhwmapfile.h"
void RstSyncHandler(void *msg)
{
#if 0 // zrx del 20060612
	extern alm_queue_node_struct gAlmQueue[ALM_NUM_OF_ALAMRS];
	extern alm_framework_context_struct g_alm_frm_cntx;

	memset(gAlmQueue, 0xff, sizeof(gAlmQueue));	

	g_alm_frm_cntx.EnabledAlarms = 0;

   #ifdef WAP_SUPPORT
	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstStartRestore(): Send Restore to WAP. ===\n"));
   SetProtocolEventHandler(RstWapRspHdlr, MSG_ID_WAP_RESTORE_FACTORY_SETTING_CNF);
	RstSendRestoreMsgToWap();
	#else
	AlmATPowerReset(MMI_FALSE, 10); // shutdown system and reboot in 3 seconds.
	#endif
#endif
	
}
void RstDetachHandler(void *msg)
{
#if 0  // zrx del 20060612
		MYQUEUE	Message;
		MMIEQNVRAMRESETREQ    *local_data;

		 
		Message.oslSrcId = MOD_MMI;
		Message.oslDestId = MOD_L4C;
		Message.oslMsgId = MSG_ID_MMI_EQ_NVRAM_RESET_REQ;
		local_data = (MMIEQNVRAMRESETREQ *) OslConstructDataPtr(sizeof(MMIEQNVRAMRESETREQ) );

		local_data-> lid = 0;

		local_data-> reset_category = NVRAM_RESET_FACTORY; 

	   
		Message.oslDataPtr = (oslParaType *)local_data;
		Message.oslPeerBuffPtr= NULL;
		SetProtocolEventHandler(RstSyncHandler, MSG_ID_MMI_EQ_NVRAM_RESET_RSP);
		OslMsgSendExtQueue(&Message);
		#endif

}
#endif
#ifdef JATAAYU_SUPPORT
extern void mmi_brw_restore_default_profile_settings(void);
extern void mmi_brw_mms_restore_default_profile_settings(void);
#endif

//#include "dsm_dev_driver.h"
#include "fmt_struct.h"
#include "filemgr.h"
void restorePhoneCB(void *msg)
{
	pBOOL status=0;
	mmi_fmt_format_rsp_strcut *resCB = (mmi_fmt_format_rsp_strcut*)msg;
	status = resCB->result;
	RstRestoreDisplayPopup(ERR_SUCCESS==status);
}
void RstStartRestore_ClearuserData(void)
{
#ifdef JATAAYU_SUPPORT
	if(
	#ifdef JATAAYU_SUPPORT_MMS 
		jMMS_IsMMSAppOrDownloadActive()
		#ifdef JATAAYU_SUPPORT_WAP
		||jMMS_IsWapConnected()
		#endif
	#else
		jMMS_IsWapConnected()
	#endif
	
		)
	{
		DisplayPopup((PU8)GetString(STR_ID_DELETE_FAIL_NODEBUSY) ,
			IMG_GLOBAL_PROGRESS, 1,
			UI_POPUP_NOTIFYDURATION_TIME,
			WARNING_TONE);
				return;
	}
#endif
	EntryRstRestoreProcessing();
	fmgr_send_format_req('a',0,restorePhoneCB);	
	return;
}
#ifndef MMI_ON_WIN32
//#define RESTOR_BY_CLEAR_USERDATA
#endif
#ifdef RESTOR_BY_CLEAR_USERDATA

//chenhe add for clear up data region in flash to restore 
void RstStartRestore(void)
{

	EntryRstRestoreProcessing();
	fmgr_send_format_req('a',0,restorePhoneCB);	
	return;
}
#else
void RstStartRestore(void)
{	
	TRACE_SYS_FUNCTION();
/*
#ifdef __HALF_SECURE_RESTORE__
	if(!IsPhoneLock())
	  GoBackHistory();
	else
	  GoBack2TheHistory();
#else
	  GoBack2TheHistory();
#endif
*/
#ifdef JATAAYU_SUPPORT
	if(
	#ifdef JATAAYU_SUPPORT_MMS
		jMMS_IsMMSAppOrDownloadActive()
		#ifdef JATAAYU_SUPPORT_WAP
		||jMMS_IsWapConnected()
		#endif
	#else
		jMMS_IsWapConnected()
	#endif
	
		)

	{
		DisplayPopup((PU8)GetString(STR_ID_DELETE_FAIL_NODEBUSY) ,
					IMG_GLOBAL_PROGRESS, 1,
					UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		return;
	}
#endif
	EntryRstRestoreProcessing();
#if defined(__RESET_FACTORY__) && defined (MMI_ON_HARDWARE_P)
	{
		TRACE_SYS_FUNC("defined(__RESET_FACTORY__) && defined (MMI_ON_HARDWARE_P)");
		MYQUEUE	Message;

		/* Lock Nvram Cache Access and never relase*/
		//SetFlashAllNVRAMCatchData(MMI_TRUE);  // zrx del 20060612

		SetProtocolEventHandler(RstDetachHandler,MSG_ID_MMI_NW_PWROFF_DETACH_RSP);
		 
		Message.oslSrcId=MOD_MMI;
		Message.oslDestId=MOD_L4C;
		Message.oslMsgId = MSG_ID_MMI_NW_PWROFF_DETACH_REQ;

		Message.oslDataPtr = (oslParaType *)NULL;
		Message.oslPeerBuffPtr= NULL;
		OslMsgSendExtQueue(&Message);
	}

#else // __RESET_FACTORY__
/* Lock Nvram Cache Access */
//SetFlashAllNVRAMCatchData(MMI_TRUE); // zrx del 20060612
	/* restore wallpaper */
	TRACE_SYS_FUNC("!!!defined(__RESET_FACTORY__) && defined (MMI_ON_HARDWARE_P)");

	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstStartRestore(): Start restore Wall Paper. ===\n"));
	if(!PhnsetRstWallpaperDefault())
	{
		RstRestoreDisplayPopup(bStatus);
		return;
	}

	/*WeiD Add Start For 6126 Ver: TBM780  on 2007-8-20 20:26 */
	if(!PhnsetRstPwrOffDisplayDefault())
	{
		RstRestoreDisplayPopup(bStatus);
		mmi_trace(1, "PhnsetRstWallpaperDefault is ERROR");
		return;
	}

	if(!PhnsetRstPwrOnDisplayDefault())
	{
		RstRestoreDisplayPopup(bStatus);
		mmi_trace(1, "PhnsetRstWallpaperDefault is ERROR");
		return;
	}
	/*WeiD Add End  For 6126 Ver: TBM780  on 2007-8-20 20:26 */
	
	/* restore screen saver */
	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstStartRestore(): Start restore Screen Saver. ===\n"));
	if(!PhnsetRstScrSvrDefault())
	{
		RstRestoreDisplayPopup(bStatus);
		return;
	}

	
	/* clear restore factory password */
	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstStartRestore(): Start restore Phonesetup. ===\n"));
	memset(gPhonePassword,0,(MAX_SIM_SETTING_PASSWD_LEN+1) * ENCODING_LENGTH);
	/* restore settings values */
	if(!RstRestorePhoneSetup())
	{
		RstRestoreDisplayPopup(bStatus);
		return;
	}

	
	/* restore profile default values */
	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstStartRestore(): Start restore User Profile. ===\n"));
	if(!SetProfileDefault())
	{
		RstRestoreDisplayPopup(bStatus);
		return;
	}
	
	/* restore caller group default values */
	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstStartRestore(): Start restore Caller group. ===\n"));
	if(!mmi_phb_callergroup_restore_default())
	{
		RstRestoreDisplayPopup(bStatus);
		return;
	}

	/* Flush Nvram Data */
	//WriteFlashAllNVRAMCatchData();  // zrx del 20060612

   #ifdef WAP_SUPPORT
   	TRACE_SYS_FUNC("WAP_SUPPORT");
	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstStartRestore(): Send Restore to WAP. ===\n"));
   SetProtocolEventHandler(RstWapRspHdlr, MSG_ID_WAP_RESTORE_FACTORY_SETTING_CNF);
	RstSendRestoreMsgToWap();
	#else
	TRACE_SYS_FUNC("!WAP_SUPPORT");
	RstWapRspHdlr((void *)NULL);
	#endif

#endif //__RESET_FACTORY__
#ifdef JATAAYU_SUPPORT
mmi_brw_restore_default_profile_settings();
mmi_brw_mms_restore_default_profile_settings();
#endif
	bStatus = MMI_TRUE; 
	RstRestoreDisplayPopup(bStatus);
	//GoBackHistory();
	return;
}
#endif
/*****************************************************************************
* FUNCTION
*  RstRestoreDisplayPopup
* DESCRIPTION
*   This function is to display success/fail popup for restore default
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void RstWapRspHdlr(void *msg)
{
	/* restore message setting values */
	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstStartRestore(): Start restore Message. ===\n"));

	#ifdef __RESET_FACTORY__
	AlmATPowerReset(MMI_FALSE, 10); // shutdown system and reboot in 3 seconds.
	#else
	//RestoreMessagesSettings();   // zrx del 20060612
	#endif
	
	return;
}


/*****************************************************************************
* FUNCTION
*  RstRestoreDisplayPopup
* DESCRIPTION
*   This function is to display success/fail popup for restore default
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
extern void QuitSystemOperation(void);//qiff add here for factory mode shutdown without power off animation
extern BOOL gSystemResetFlag ;
extern pBOOL isInCall(void);

static void restoretoshutdown(void)
{
	INT32 handle = -1,reason = TRUE;
	UINT8 tmpFilename[256];
	AnsiiToUnicodeString(tmpFilename,PHONE_RESTORE_FLAG_PATH);
	if(!isInCall())
	{
		handle =  FS_Open(tmpFilename,FS_O_CREAT|FS_O_RDWR,0);
		if(handle>=0)
		{
			FS_Write(handle,(UINT8*)&reason,sizeof(reason));
			
		}
		FS_Close(handle);
		gSystemResetFlag = MMI_TRUE;
		QuitSystemOperation();
	}
}
extern mdi_result mdi_audio_stop_all(void);

void RstRestoreDisplayPopup(pBOOL status)
{

	/* check the status is success or not */
	if(status)
	{
	     /* if the restore default values is success */
#ifndef __MMI_NOKIA_STYLE_N800__		 
	     SpofRestore();
#endif		

   	     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestoreDisplayPopup(): Restore Successful. ===\n"));
	     DisplayPopup((PU8)GetString( STR_GLOBAL_DONE /*STR_RESTORE_SUCCESS*/) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
		/* clear all key handlers so as to restrict duplicate reqs */		
		ClearAllKeyHandler();
		ClearKeyHandler(KEY_END,KEY_EVENT_UP);
		ClearKeyHandler(KEY_END,KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_END,KEY_LONG_PRESS);

//qiff modify for factory mode shutdown without power off animation
//		  coolsand_UI_start_timer(ST_NOTIFYDURATION,DisplayShutDownSystem);//wangzl:add for restart 
		mdi_audio_stop_all();
		coolsand_UI_start_timer(ST_NOTIFYDURATION,restoretoshutdown);//wangzl:add for restart 
	}
	else
	{
		/* if the restore default values is failure */
   	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestoreDisplayPopup(): Restore Fail. ===\n"));
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
	}

	/* delete screens for maitaining the history */
  	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestoreDisplayPopup(): Delete Screen. ===\n"));
	DeleteScreenIfPresent(SCR_RESTORE);
	DeleteScreenIfPresent(SCR_SETTING_RESTORE_CONFIRM);
	DeleteScreenIfPresent(SCR_RESTORE_PROCESSING);
    
}



/*****************************************************************************
* FUNCTION
*  RstRestorePhoneSetup
* DESCRIPTION
*   This function is to restore phone setup default values
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
extern void mmi_audply_restoreSetting(void);

pBOOL RstRestorePhoneSetup(void)
{
	U8 data = 0;
	S16 error;

#ifdef __MMI_PREFER_INPUT_METHOD__
        U16 data16=0;
#endif              	


#ifdef __MMI_PREFER_INPUT_METHOD__
 	   /* Restore Prefer input method */
  	  PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestorePhoneSetup(): Start restore InputMethod. ===\n"));
	  data16 = RstGetPreferedInputMethodDefault();
	  WriteValue( NVRAM_SETTING_PREFER_INPUT_METHOD, &data16, DS_SHORT , &error);
	  SetInputMethodType(data16);
#endif              	

	/* Restore speed dial status */
 	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestorePhoneSetup(): Start restore Speed dial status. ===\n"));
	data = RstGetSpeedDialStatusDefault();
	WriteValue( NVRAM_SETTING_SPEED_DIAL, &data, DS_BYTE , &error);

/*Wangzl Del Start For 5864 Ver: TBM_780  on 2007-7-31 16:3 */ 
	#if 0
	/* Restore contrast level for main lcd */
 	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestorePhoneSetup(): Start restore Contrast Level. ===\n"));
	data = RstGetContrastLevelMainDefault();
	PhnsetChangeMainLCDContrastValue( data );
	PhnsetMainLCDContrast(data);
	WriteValue( NVRAM_SETTING_CONTRAST_LEVEL, &data, DS_BYTE , &error);
	#endif
/*Wangzl Del End  For 5837 Ver: TBM_780  on 2007-7-31 16:3 */
#ifdef __MMI_SUBLCD__
	/* Restore contrast lever for sub lcd */
	data = RstGetContrastLevelSubDefault();
	PhnsetChangeSubLCDContrastValue( data );
	PhnsetSubLCDContrast(data);
	WriteValue( NVRAM_SETTING_CONTRAST_SUBLCD_LEVEL, &data, DS_BYTE , &error);
#endif

	/* Restore greeting text status */
 	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestorePhoneSetup(): Start restore greeting text. ===\n"));
	data = RstGetGreetingTextStatusDefault();
	WriteValue( NVRAM_SETTING_WELCOME_TEXT, &data, DS_BYTE , &error);
	memset(WelcomeText,0,MAX_SETTING_WELCOME_TEXT * ENCODING_LENGTH);
	AnsiiToUnicodeString((S8*)WelcomeText, (S8*)"Welcome");  
	WriteRecord(NVRAM_EF_SETTING_LID, 1,(void *) WelcomeText,(U16) (MAX_SETTING_WELCOME_TEXT * ENCODING_LENGTH), &error);

#if defined(__MMI_APHORISM__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif


	mmi_audply_restoreSetting();//chenhe add
	/* Restore show own number status */
 	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestorePhoneSetup(): Start restore OwnerNumber. ===\n"));
	data = RstGetOwnerNumStatusDefault();
	WriteValue( NVRAM_SETTING_OWNER_NO, &data, DS_BYTE , &error);
	//CurrentDtType = 1;//default on
	PhnsetSetShowDTStatus(1);

	/* Restore show date/time status */
 	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestorePhoneSetup(): Start restore DateTime Status. ===\n"));
	data = RstGetShowDTStatusDefault();
	WriteValue( NVRAM_SETTING_STATUS_DT_TIME, &data, DS_BYTE , &error);

	/* Restore auto update date/time status */
 	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestorePhoneSetup(): Start restore NITZ. ===\n"));
	data = RstGetNITZStatusDefault();
	WriteValue( NVRAM_SETTING_AUTOUPDATE_DT_TIME, &data, DS_BYTE , &error);

	/* Restore speed dial */
	/*
	data = PhbRestoreSpeedDial();		
	if(error == NVRAM_WRITE_SUCCESS)
		return TRUE;
	else
		return FALSE;*/


	/* Restore speed dial */
 	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestorePhoneSetup(): Start restore Speed dial. ===\n"));
	if( PhbRestoreSpeedDial() == MMI_FALSE )
	   return FALSE;


      /* Restore date format  */
	 #if 0  //modified by lin for restore factory setting @20070619
 	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestorePhoneSetup(): Start restore Date Format. ===\n"));
	data = RstGetDateFormatDefault();
	if (PhnsetSetDateFormat(data)!= NVRAM_WRITE_SUCCESS) /*dd-mmm-yyyy*/
	  return FALSE;

	/* Restore time format */
 	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstRestorePhoneSetup(): Start restore Time Format. ===\n"));
	data = RstGetTimeFormatDefault();
	if (PhnsetSetTimeFormat(data)!= NVRAM_WRITE_SUCCESS) /*12hr format as per the error*/
	  return FALSE;
     #endif
#ifdef __MMI_TOUCH_SCREEN__
	#ifdef MMI_ON_HARDWARE_P
	WriteNvramCalibrationSet(FALSE);
    mmi_pen_restore_default_calibration( );
    #endif
#endif

     return TRUE;	
     
}



/*****************************************************************************
* FUNCTION
*  RstrSettingPasswdScrSoftKeyFunc
* DESCRIPTION
*   This function is to change softkey function in screen according to 
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void RstrSettingPasswdScrSoftKeyFunc(U8* text,U8* cursor,S32 length)
{
//micha0606
	/* check the validity of the input password*/
	if((length) < MIN_PASSWORD_LENGTH)
	{
		/* if length is short then disable lsk */
		ChangeLeftSoftkey(0,0);
		ClearKeyHandler(KEY_POUND,KEY_EVENT_UP);
#ifdef TGT_GALLITE_G800
		ClearKeyHandler(KEY_ENTER,KEY_EVENT_UP);
		ClearKeyHandler(KEY_ENTER,KEY_EVENT_DOWN);
#endif
	}
	else
	{
		/* if its valid length then enable lsk */
		EnableLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
		SetKeyHandler(g_SecuritySetupContext.PasswdLenCheckCallBack,KEY_POUND,KEY_EVENT_UP);
#ifdef TGT_GALLITE_G800	
	SetKeyHandler(g_SecuritySetupContext.PasswdLenCheckCallBack,KEY_ENTER,KEY_EVENT_UP);
#endif
	}
}



#ifdef __MMI_PREFER_INPUT_METHOD__

/*****************************************************************************
* FUNCTION
*  RstGetPreferedInputMethodDefault
* DESCRIPTION
*   This function is to get prefered Input Method restore default value
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U16 RstGetPreferedInputMethodDefault(void)
{
	U16 data16 = 0;
	S16 error = 0;
	/* retrive preferred input method by reading default value from nvram */
	ReadValue( RESTORE_PREFER_INPUT_METHOD, &data16, DS_SHORT, &error);
	/* return the preferred input method */
	return data16;
}
#endif



/*****************************************************************************
* FUNCTION
*  RstGetSpeedDialStatusDefault
* DESCRIPTION
*   This function is to get speed dial status restore default value
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 RstGetSpeedDialStatusDefault(void)
{
	U8 gSpeedDialStatus = 0;
	S16 error = 0;

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetSpeedDialStatusDefault() ===\n"));

	/* retrive default speed dial status from nvram */
	ReadValue( RESTORE_DEFAULT_SPEED_DIAL, &gSpeedDialStatus, DS_BYTE, &error);
	/* return the default speed dial status */
	return gSpeedDialStatus;
}


/*****************************************************************************
* FUNCTION
*  RstGetDateFormatDefault
* DESCRIPTION
*   This function is to get date format restore default
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 RstGetDateFormatDefault(void)
{
	U8 gDateFormat = 0;
	S16 error = 0;

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetDateFormatDefault() ===\n"));

	/* retrive default date format from nvram */
	ReadValue( RESTORE_DEFAULT_DATE_FORMAT, &gDateFormat, DS_BYTE, &error);
	/* return the default date format */
	return gDateFormat;
}


/*****************************************************************************
* FUNCTION
*  RstGetTimeFormatDefault
* DESCRIPTION
*   This function is to get time format restore default
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 RstGetTimeFormatDefault(void)
{
	U8 gTimeFormat = 0;
	S16 error = 0;

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetTimeFormatDefault() ===\n"));

	/* retrive default time format from nvram */
	ReadValue( RESTORE_DEFAULT_TIME_FORMAT, &gTimeFormat, DS_BYTE, &error);
	/* return the default time format */
	return gTimeFormat;
}



/*****************************************************************************
* FUNCTION
*  RstGetContrastLevelSubDefault
* DESCRIPTION
*   This function is to get sub lcd constract level restore default value
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 RstGetContrastLevelSubDefault(void)
{
	U8 gContrastSubLcd = 0;
	S16 error = 0;

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetContrastLevelSubDefault() ===\n"));

	/* retrive the default contrast level for sub lcd from nvram */
	ReadValue(RESTORE_DEFAULT_CONTRAST_SUBLCD_LEVEL, &gContrastSubLcd, DS_BYTE, &error);
	/* return the default contrast level for sub lcd */
	return gContrastSubLcd;
}


/*****************************************************************************
* FUNCTION
*  RstGetContrastLevelMainDefault
* DESCRIPTION
*   This function is to get main lcd constract level restore default value
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 RstGetContrastLevelMainDefault(void)
{
	U8 gContrastLcd = 0;
	S16 error = 0;

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetContrastLevelMainDefault() ===\n"));

	/* retrive the default contrast level for main lcd from nvram */
	ReadValue(RESTORE_DEFAULT_CONTRAST_LEVEL, &gContrastLcd, DS_BYTE, &error);
	/* return the default contrast level for main lcd */
	return gContrastLcd;
}



/*****************************************************************************
* FUNCTION
*  RstGetGreetingTextStatusDefault
* DESCRIPTION
*   This function is to get greeting text on/off status default value
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 RstGetGreetingTextStatusDefault(void)
{	
	U8 gWelcomeText = 0;
	S16 error = 0;

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetGreetingTextStatusDefault() ===\n"));

	/* retrive the default greeting text status from nvram */
	ReadValue(RESTORE_DEFAULT_WELCOME_TEXT, &gWelcomeText, DS_BYTE, &error);
	/* return the default greeting text status */
	return gWelcomeText;
}

/*****************************************************************************
* FUNCTION
*  RetGetAphorismTextStatusDefault
* DESCRIPTION
*   This function is to get Aphorism on/off status default value
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
#if defined(__MMI_APHORISM__)
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



/*****************************************************************************
* FUNCTION
*  RstGetOwnerNumStatusDefault
* DESCRIPTION
*   This function is to 
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 RstGetOwnerNumStatusDefault(void)
{
	U8 gOwnerNo = 0;
	S16 error = 0;

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetOwnerNumStatusDefault() ===\n"));

	/* retrive the default value for owner number status from nvram */
	ReadValue(RESTORE_DEFAULT_OWNER_NO, &gOwnerNo, DS_BYTE, &error);
	/* return the default owner numbers status */
	return gOwnerNo;
}



/*****************************************************************************
* FUNCTION
*  RstGetShowDTStatusDefault
* DESCRIPTION
*   This function is to get show date and time status default value
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 RstGetShowDTStatusDefault(void)
{
	U8 gStatusShowDtTime = 0;
	S16 error = 0;

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetShowDTStatusDefault() ===\n"));

	/* retrive default value for display date time status from nvram */
	ReadValue(RESTORE_DEFAULT_STATUS_SHOW_DT_TIME, &gStatusShowDtTime, DS_BYTE, &error);
	/* return default value for display date time status */
	return gStatusShowDtTime;
}



/*****************************************************************************
 * FUNCTION
 *  RstGetNITZStatusDefault
 * DESCRIPTION
 *  This function is to get NITZ status default
 * PARAMETERS
 *  void
 * RETURNS
 *  On/Off
 *****************************************************************************/
U8 RstGetNITZStatusDefault(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 gAutoUpdateTime = 0;
    S16 error = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetNITZStatusDefault() ===\n"));

    /* retrive default value for auto update date time status from nvram */
    ReadValue(RESTORE_DEFAULT_AUTOUPDATE_TIME, &gAutoUpdateTime, DS_BYTE, &error);
    /* return default value for auto update date time status */
    return gAutoUpdateTime;
}


/*****************************************************************************
* FUNCTION
*  RstGetScrSaverStatusDefault
* DESCRIPTION
*   This function is to get screen saver status default value
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 RstGetScrSaverStatusDefault(void)
{
	U8 ScrSvrStatus = 0;
	S16 error = 0;

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetScrSaverStatusDefault() ===\n"));

	/* retrive default value for screen saver status  from nvram */
#if defined(__MMI_LCD_PARTIAL_ON__) && defined(__MMI_LCD_PARTIAL_ON_ENABLED__)
	ScrSvrStatus = (U8) SET_SS_STATUS_POWERSAVING;
#else
	ReadValue(RESTORE_DEFAULT_SCREENSAVER_STATUS, &ScrSvrStatus, DS_BYTE, &error);
#endif 
	/* return default value for screen saver status */
	return ScrSvrStatus;
}




/*****************************************************************************
* FUNCTION
*  RstGetScrSaverWaitingDefault
* DESCRIPTION
*   This function is to get screen saver waiting time default value
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 RstGetScrSaverWaitingDefault(void)
{
	U8 gRestorScrWaitingVal = 0;
	S16 error = 0;

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetScrSaverWaitingDefault() ===\n"));

	/* retrive default value for screen saver waiting value from nvram */
	ReadValue(RESTORE_DEFAULT_SCREENSAVER_WAITING, &gRestorScrWaitingVal, DS_BYTE, &error);
	/* return default value for screen saver waiting value*/
	return gRestorScrWaitingVal;
}




/*****************************************************************************
* FUNCTION
*  RstGetScrSaverImgIdDefault
* DESCRIPTION
*   This function is to get screen saver image id default value
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U16 RstGetScrSaverImgIdDefault(void)
{
	S16 error = 0;
	U16 gScreenSaverId = 0;	

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetScrSaverImgIdDefault() ===\n"));

	/* retrive default value for screen saver id from nvram */
	ReadValue(RESTORE_DEFAULT_CURRENT_SCREENSVER_ID, &gScreenSaverId, DS_SHORT, &error);	
	/* return default value for screen saver id */
	return gScreenSaverId;
}





/*****************************************************************************
* FUNCTION
*  RstGetWallpaperImgIdDefault
* DESCRIPTION
*   This function is to get wallpaper image id default vaule
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U16 RstGetWallpaperImgIdDefault(void)
{
	S16 error = 0;
	U16 gScreenSaverId = 0;	

     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetScrSaverImgIdDefault() ===\n"));

	/* retrive value for wall paper id from nvram */
	ReadValue(RESTORE_DEFAULT_FUNANDGAMES_SETWALLPAPER, &gScreenSaverId, DS_SHORT, &error);	
	if(gScreenSaverId == 0)
		gScreenSaverId = FACTORY_RESTORE_DEFAULT_FUNANDGAMES_SETWALLPAPER_ID;
	/* return default value for wall paper id*/
	return gScreenSaverId;
}

 
#ifdef __MMI_SUB_WALLPAPER__
U16 RstGetWallpaperSubImgIdDefault(void)
{
	S16 error = 0;
	U16 gScreenSaverId = 0;	

	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== RstGetWallpaperSubImgIdDefault() ===\n"));

	/* retrive value for wall paper id from nvram */
	ReadValue(RESTORE_DEFAULT_SETWALLPAPER_SUB, &gScreenSaverId, DS_SHORT, &error);	
	/* return default value for wall paper id*/
	return gScreenSaverId;
}
#endif
 

#endif /* _RESTORE_C */


