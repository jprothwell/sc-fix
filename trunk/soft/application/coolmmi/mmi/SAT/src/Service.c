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

/*******************************************************************************
 * Filename:
 * ---------
 *	Service.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   
 *
 * Author:
 * -------
 * -------
 * * 修改记录：
 *       修改日期：20060512
 *       版 本 号：6205.v02
 *       修 改 人：苏世鹏
 *       修改内容：关闭include文件
 *------------------------------------------------------------------------------
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "l4dr.h"

#include "frameworkstruct.h"
#include "satdefs.h"
#include "satstruct.h"
#include "satgprots.h"
#ifdef __GPRS_MODE__
#include "dataaccountgprot.h"//jiashuo for wap/mms,20090212
#endif
#include "infonum.h"
#include "commonscreens.h"
#include "wapprot.h"
#include "waptype.h"
#include "servicedefs.h"
#include "service.h"
#include "settingprofile.h"

#ifdef __MMI_POC__
 
#include "pocgprot.h"
#endif
#include "mmi_trace.h"
#undef __NEWSIMULATOR

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif


//micha1229
//static U8 servicesIndex=0;
extern void goto_mobile_suite_screen(void);
extern SATSETUPMENU gSATMainMenu[];

/************************************************************

  	FUNCTION NAME		: GoToServicesMenu

  	PURPOSE				: Displays Services Menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
#ifdef __MMI_MULTI_SIM__
extern BOOL MTPNP_AD_Is_Flight_Mode(void);
#endif
#ifdef __FLIGHT_MODE_SUPPORT__ 
extern pBOOL	mmi_bootup_get_active_flight_mode(void);
#endif

extern U8 gInsertSimAppFlag ;

void GoToServicesMenu(void)
{
	U16 *nStrItemList;
	U16 *nIconItemList;

	PU8 *Strings = NULL;
	U16 *Icons = NULL;
	U16 nNumofItem;
	
	U16 i;
	U16 j;
	U16 parentId;
	U8* guiBuffer;

	int simIndex;
	
#ifdef __MMI_MULTI_SIM__
    mmi_trace(g_sw_SAT,"SAT Func: %s start up mode =0x%x", __FUNCTION__, MTPNP_AD_Is_Flight_Mode());
#endif

	if (
#ifdef __FLIGHT_MODE_SUPPORT__ 
	(mmi_bootup_get_active_flight_mode()==1)||
#endif
#ifdef __MMI_MULTI_SIM__
	(!MTPNP_PFAL_Has_CardValid())
#else
	(gInsertSimAppFlag ==1)
#endif
	)
    {
    
        DisplayPopup(
            (PU8) GetString(SERVICES_NOT_PROVIDED_STRING_ID),
            IMG_GLOBAL_WARNING,
            0,
            UI_POPUP_NOTIFYDURATION_TIME,
            (U8) WARNING_TONE);
        return;
    }

#ifdef __MMI_MERGE_SAT_TO_MAINMENU__
#ifndef JATAAYU_SUPPORT
#ifndef __MMI_MULTI_SIM__
	if (IsSATPresent(0)) {
		GoToSATMainMenu();
		return;
	}
#endif
#endif
#endif
#if 1// zrx del 20060612 // wangbei open 20060724

	if(isInfoNumPresent)
		parentId=MAIN_MENU_SERVICES_INFO_MENUID;
	else
		parentId=MAIN_MENU_SERVICES_MENUID;
	#endif

	nNumofItem = GetNumOfChild (parentId);
	nStrItemList=(U16 *)OslMalloc(nNumofItem*sizeof(U16));
	nIconItemList=(U16 *)OslMalloc(nNumofItem*sizeof(U16));
	GetSequenceStringIds (parentId, nStrItemList);
	GetSequenceImageIds(parentId, nIconItemList);
	SetParentHandler(parentId);
	guiBuffer=GetCurrGuiBuffer(SERVICES_MAIN_MENU_SCREEN_ID);

{
	U16 count=nNumofItem;

#ifndef WAP_SUPPORT
	for(i=j=0;i<nNumofItem;i++)
	if(nStrItemList[i] != SERVICES_WAP_STRING_ID)
	{
		nStrItemList[j] = nStrItemList[i];
		nIconItemList[j] = nIconItemList[i];
		j++;
	}
	else
		count--;
#endif

#if 0//ndef __TCPIP__
	for(i=j=0;i<count;i++)
#ifdef __MMI_GPRS_FEATURES__
	if(nStrItemList[i] != SERVICES_DATA_CONNECT_STRING_ID && nStrItemList[i] != SERVICES_DATA_CONNECT_MAIN_STRING_ID)
#else
	if(nStrItemList[i] != SERVICES_DATA_CONNECT_STRING_ID)
#endif
	{
		nStrItemList[j] = nStrItemList[i];
		nIconItemList[j] = nIconItemList[i];
		j++;
	}
	else
		count--;
#endif

	nNumofItem = count;
}

	Strings=(PU8 *)OslMalloc(MAX_SUB_MENUS*sizeof(PU8));
	Icons=(U16 *)OslMalloc((MAX_SUB_MENUS)*sizeof(U16));
 	 i = 0;

	 for(simIndex = 0; simIndex < MMI_SIM_NUMBER; simIndex++)
	 {
	 	if(IsSATPresent(simIndex))
	 	{
	 		Strings[i]=gSATMainMenu[simIndex].alphaId;
		#if defined( __MMI_MULTI_SIM__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
			Icons[i]=SAT_TITLE_IMAGE_ID_SIM1 + simIndex;
		#else
			Icons[i]=SAT_TITLE_IMAGE_ID;
		#endif
			i++;
	 	}
	 }

	for (j = 0; j < nNumofItem; j++)
	{
	    Strings[i] = (PU8) GetString(nStrItemList[j]);
	    Icons[i] = nIconItemList[j];
	    i++;
	}
	
	mmi_trace(g_sw_SAT,"SAT Func: %s nNumofItem = %d, i = %d", __FUNCTION__, nNumofItem, i);	
	nNumofItem = i;

	if(i == 0)
	{
		if (nNumofItem>0)
		{
			Strings=(PU8 *)OslMalloc(nNumofItem*sizeof(PU8));
			Icons=(U16 *)OslMalloc(nNumofItem*sizeof(U16));
			for(i=0;i<nNumofItem;i++)
			{
				Strings[i]=(PU8)GetString(nStrItemList[i]);
				Icons[i]=nIconItemList[i];	
			}
		}
		else
		{
			if(Strings != NULL)
			{
				OslMfree(Strings);
				Strings = NULL;
			}
			if(Icons != NULL)
			{
				OslMfree(Icons);
				Icons = NULL;
			}
			
			
			//yuxiang B 07.6.21
			DisplayPopup((U8*)GetString(SERVICES_NOT_PROVIDED_STRING_ID),IMG_GLOBAL_UNFINISHED,1,2000,(U8)ERROR_TONE);	
			return ;
			//yuxiang E 07.6.21
		}
	}

#ifdef __MMI_MERGE_SAT_TO_MAINMENU__
	if (nNumofItem) {

#ifdef __MMI_SERVICE_MENU_NUMERIC_LIST_ICON__
		EntryNewScreen(SERVICES_MAIN_MENU_SCREEN_ID, wgui_restore_list_menu_slim_style, GoToServicesMenu, NULL);
		wgui_override_list_menu_slim_style(WGUI_LIST_MENU_SLIM_STYLE_NUMBER);
#else
		EntryNewScreen(SERVICES_MAIN_MENU_SCREEN_ID, NULL, GoToServicesMenu, NULL);
#endif /* __MMI_SERVICE_MENU_NUMERIC_LIST_ICON__ */
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			gSATMainMenu[i].selectedMenuItem = 0;
		}
		RegisterHighlightHandler(ServicesMenuIndex);
//micha1002		
		ShowCategory84Screen(SERVICES_TITLE_STRING_ID,GetRootTitleIcon(MAIN_MENU_SERVICES_MENUID),
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								nNumofItem,Strings,Icons,LIST_MENU,0,guiBuffer);
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	}
	else {
		DisplayPopup((PU8)GetString(SERVICES_NOT_PROVIDED_STRING_ID) ,IMG_GLOBAL_ERROR, 0,1000,ERROR_TONE);		
	}
#else

#ifdef __MMI_SERVICE_MENU_NUMERIC_LIST_ICON__
	EntryNewScreen(SERVICES_MAIN_MENU_SCREEN_ID, wgui_restore_list_menu_slim_style, GoToServicesMenu, NULL);
	wgui_override_list_menu_slim_style(WGUI_LIST_MENU_SLIM_STYLE_NUMBER);
#else
	EntryNewScreen(SERVICES_MAIN_MENU_SCREEN_ID, NULL, GoToServicesMenu, NULL);	
#endif /* __MMI_SERVICE_MENU_NUMERIC_LIST_ICON__ */
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		gSATMainMenu[i].selectedMenuItem = 0;
	}
	RegisterHighlightHandler(ServicesMenuIndex);
//micha1002	
 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_enable_mini_tab_bar(parentId);
#endif
 

	ShowCategory84Screen(/*SERVICES_TITLE_STRING_ID*/ SERVICES_STK_STRING_ID,GetRootTitleIcon(MAIN_MENU_SERVICES_MENUID),
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nNumofItem,Strings,Icons,LIST_MENU,0,guiBuffer);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
#endif
	OslMfree(nStrItemList);
	OslMfree(nIconItemList);

	if (Strings!=NULL)
		OslMfree(Strings);
	if (Icons!=NULL)
		OslMfree(Icons);
}

/************************************************************

  	FUNCTION NAME		: ServicesMenuIndex

  	PURPOSE				: Hilite Handler for Services Menu

	INPUT PARAMETERS	: index: of highlighted item

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
extern void SATSetActiveSimID(U8 nSimID);
void ServicesMenuIndex(S32 index)
{

	int count = 0;

	U8 SimIndex[MMI_SIM_NUMBER];
	U8 i;
	mmi_trace(g_sw_SAT,"MMI_SAT: Func: %s index=%d", __FUNCTION__, index);
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(IsSATPresent(i))
		{
			SimIndex[count++] = i;
		}
	}
	if(index < count)
	{
		#ifdef __MMI_MULTI_SIM__
		SATSetActiveSimID(SimIndex[index]);
		#endif
		SATHiliteHandler();
	}
	mmi_trace(g_sw_SAT,"MMI_SAT: Func: %s  count =%d", __FUNCTION__, count);

	if (index >= count)
		ExecuteCurrHiliteHandler(index - count);

}

/************************************************************

  	FUNCTION NAME		: SATHiliteHandler

  	PURPOSE				: Hilite Handler for SAT Sub Menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/

void SATHiliteHandler(void)
{
	SetLeftSoftkeyFunction(GoToSATMainMenu,KEY_EVENT_UP);
	SetKeyHandler(GoToSATMainMenu,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

/************************************************************

  	FUNCTION NAME		: WapHiliteHandler

  	PURPOSE				: Hilite Handler for WAP Sub Menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/

void WapHiliteHandler(void)
{
#ifdef WAP_SUPPORT
	SetLeftSoftkeyFunction(goto_mobile_suite_screen,KEY_EVENT_UP);
	SetKeyHandler(goto_mobile_suite_screen,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif
}

/************************************************************

  	FUNCTION NAME		: DataConnectHiliteHandler

  	PURPOSE				: Hilite Handler for Data Connect Sub Menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/

void DataConnectHiliteHandler(void)
{
#ifdef __TCPIP__

#ifdef __MMI_GPRS_FEATURES__
	SetLeftSoftkeyFunction(EntryDTCNTMain,KEY_EVENT_UP);
	SetKeyHandler(EntryDTCNTMain,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(InitializeDataAccountRecords,KEY_EVENT_UP);
	SetKeyHandler(InitializeDataAccountRecords,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif	//  __MMI_GPRS_FEATURES__

#endif	// __TCPIP__

}

/************************************************************

  	FUNCTION NAME		: InfoNumHiliteHandler

  	PURPOSE				: Hilite Handler for Information Number Sub Menu

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/

void InfoNumHiliteHandler(void)
{
#if 0 // zrx del 20060612
	SetLeftSoftkeyFunction(EnterScrInfoNumber,KEY_EVENT_UP);
	SetKeyHandler(EnterScrInfoNumber,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif 
}


#ifdef __MMI_WAP__	 
#include "downloadprotos.h" 
/**************************************************************

	FUNCTION NAME		: WapAddDownloadResource(void *msgPtr)
	
  	PURPOSE				: add downloaded resource to MMI

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
static void WapAddDownloadResource(void *msgPtr)
{
	wap_mmi_add_download_ind_struct *req;
	req = (wap_mmi_add_download_ind_struct *)msgPtr;

	switch (req->type) {
		case WAP_DOWNLOAD_IMAGE:
		AddDownloadedImage(req->filename);
		break;
		
#if defined(__MMI_IMELODY_SUPPORT__)	
		case WAP_DOWNLOAD_IMELODY:
		AddDownloadedIMelody(req->filename);
		break;
#endif
		
		case WAP_DOWNLOAD_AUDIO:
		AddDownloadedTone(req->filename);		
		break;
		
		default:
		ASSERT(0);
	}

}

/************************************************************

  	FUNCTION NAME		: WapDeleteDownloadIndex

  	PURPOSE				: Protocol Handler for MSG_ID_WAP_MMI_DELETE_DOWNLOAD_INDEX_REQ

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/

static void WapDeleteDownloadIndex(void *msgPtr)
{
	wap_mmi_add_download_ind_struct *req;
	req = (wap_mmi_add_download_ind_struct *)msgPtr;

	switch (req->type) {
		case WAP_DOWNLOAD_IMAGE:
		DeleteDownloadImageIndexByFileName((U8 *)req->filename);
		break;
#if defined(__MMI_IMELODY_SUPPORT__)
		case WAP_DOWNLOAD_IMELODY:
		DeleteDownloadImelodyIndexByFileName((U8 *)req->filename);
		break;
#endif		
		case WAP_DOWNLOAD_AUDIO:
		DeleteDownloadAudioIndexByFileName((U8 *)req->filename);		
		break;
		
		default:
		ASSERT(0);
	}
}


#endif  


/**************************************************************

	FUNCTION NAME		: InitService

  	PURPOSE				: Initialises variables & Protocol Handlers for SAT

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/

void InitService(void)
{
	SetHiliteHandler(SERVICES_WAP_MENU_ID,WapHiliteHandler);
	SetHiliteHandler(SERVICES_INFO_NUM_MENU_ID,InfoNumHiliteHandler);

#ifdef __MMI_GPRS_FEATURES__
	SetHiliteHandler(SERVICES_DATA_CONNECT_MAIN_MENU_ID,DataConnectHiliteHandler);
	SetHiliteHandler(SERVICES_DATA_CONNECT_MENU_ID,DataConnectGSMHiliteHandler);
	SetHiliteHandler(SERVICES_DATA_CONNECT_GPRS_MENU_ID,DataConnectGPRSHiliteHandler);
#else
	SetHiliteHandler(SERVICES_DATA_CONNECT_MENU_ID,DataConnectHiliteHandler);
#endif //__MMI_GPRS_FEATURES__

#ifdef __MMI_WAP__
	SetProtocolEventHandler(WapAddDownloadResource, MSG_ID_WAP_MMI_ADD_DOWNLOAD_REQ);
	SetProtocolEventHandler(WapDeleteDownloadIndex, MSG_ID_WAP_MMI_DELETE_DOWNLOAD_INDEX_REQ);	
#endif

 
#ifdef __MMI_POC__
	SetHiliteHandler (MENU_ID_POC_MAIN, mmi_poc_highlight_main);
#endif

	InitSimAppToolkit();
}


#ifdef __MMI_GPRS_FEATURES__
void DataConnectGSMHiliteHandler(void)
{
//#ifdef __TCPIP__
#if 0/* JIASHUO FOR WAP/MMS,NOT SUPPORT GSM CSD NOW*/
	SetLeftSoftkeyFunction(InitializeDataAccountRecords,KEY_EVENT_UP);
	SetKeyHandler(InitializeDataAccountRecords,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(PopupNoSupport, KEY_EVENT_UP);
	SetKeyHandler(PopupNoSupport, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
#endif
//#endif	// __TCPIP__
}


void DataConnectGPRSHiliteHandler(void)
{
#ifdef __GPRS_MODE__
	SetLeftSoftkeyFunction(InitializeGPRSDataAccountRecords,KEY_EVENT_UP);
	SetKeyHandler(InitializeGPRSDataAccountRecords,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif	// __TCPIP__
}

#endif	//__MMI_GPRS_FEATURES__



