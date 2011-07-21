 /***************************************************************************
 *
 * File Name : jmms_interface.c
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND 
 * AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************/

 /** 
 * @file		jmms_interface.c
 * @ingroup		jMMS
 * @brief	
 *
 **/

#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_interface.c"
 #ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include "jmms_inc.h"


#include "jmms_cb.h"
#include "jdd_netdatatype.h"
#include "jdd_netapi.h"
#include "jmms_composeutils.h"
#include "jddex_mmieventhandler.h"
#include "jmms_settings.h"
#include "jmms_communicator.h"

#include "settingprofile.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif

#define MMS_SENDRECV_TIMEOUT 20
#define MMS_RETRIES                   6
/***************************************************************************
 * Macros
 **************************************************************************/

 /***************************************************************************
 * Global variables declarations
 **************************************************************************/

JMMS_APP_INFO *g_pstJMmsAppInfo ;
JC_BOOLEAN g_bIsMMSPrimaryMMIActive;

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

JC_INT8 *jdd_CFGGetMMSTempFilePath (void) ;
/***************************************************************************
 * All Local Function Definitions
***************************************************************************/
extern U16 gCurrLangIndex;


void jMMS_SetPrimaryMMIStatus(JC_BOOLEAN bIsActive)
{
	mmi_trace(1, "#### jMMS_SetPrimaryMMIStatus value is [%d]",bIsActive) ;
	g_bIsMMSPrimaryMMIActive = bIsActive;
}

JC_BOOLEAN jMMS_GetPrimaryMMIStatus()
{
	return g_bIsMMSPrimaryMMIActive;
}

static JC_BOOLEAN jmms_IsFolderExist (char* pFolder)
{
	JC_BOOLEAN		bIsExist = E_FALSE;
	JC_RETCODE		rCode = JC_OK ;
	FOLDER_INFO 	*pstFolderInfo	= JC_NULL ;
	FOLDER_LIST 	*pstFolderList	= JC_NULL ;
	
	rCode = jdi_StoreGetFolders (g_pstJMmsAppInfo->hMsgStore, &pstFolderInfo) ;
	if (!rCode && pstFolderInfo)
	{
		pstFolderList = pstFolderInfo->pFolderList;		
		while(pstFolderList)
		{
			if (jdi_CUtilsTcsStrCaseCmp (pstFolderList->pFolderName, pFolder) == 0)
			{
				bIsExist = E_TRUE ;
				break ;
			}
			pstFolderList = pstFolderList->pNext ;		 
		}
	}	
	return bIsExist ;
}

 
INT jMMS_configMsgSmil (MSG_HANDLE hMsgHandle)
{
	JC_RETCODE		eRet		= JC_OK ;
	JC_UINT32		uiHeight	= 0;
	JC_UINT32		uiWidth		= 0 ;
	JC_INT8			*pBGColor	= JC_NULL ;
	REGION_LAYOUT	*pRegionLayout = JC_NULL, RegionLayout = {0};	
	MSG_CONFIG_SMIL_INFO	SMILInfo = {0 } ;

	eRet = jdi_MsgGetSMILRootLayout (hMsgHandle, &uiHeight, &uiWidth, &pBGColor);
	if(uiHeight == 0 || uiWidth == 0)
	{
		DeviceCharacteristics devChars ;
		
		jdd_MMIGetDeviceCharacteristics (&devChars) ;
		uiHeight = devChars.uiRenderingHeight ;
		uiWidth = devChars.uiRenderingWidth ;
		eRet = jdd_MsgConfigGetDefaultSMILInfo (&SMILInfo, uiHeight, uiWidth, 0) ;
		eRet = jdi_MsgSetSMILRootLayout (hMsgHandle, uiHeight, uiWidth, pBGColor) ;
	}
	eRet = jdi_MsgGetSMILRegionLayout (hMsgHandle, &pRegionLayout) ;
	if (pRegionLayout == JC_NULL)
	{
		RegionLayout.uiTop = SMILInfo.uiRegion_1_Top;
		RegionLayout.uiLeft = SMILInfo.uiRegion_1_Left;
		RegionLayout.uiWidth = SMILInfo.uiRegion_1_Width;
		RegionLayout.uiHeight = SMILInfo.uiRegion_1_Height;
		RegionLayout.pRegionName = SMILInfo.pRegion_1_Name;
		RegionLayout.eFit = SMILInfo.eRegion_1_Fit;
		RegionLayout.pNext = JC_NULL;
		eRet = jdi_MsgAddSMILRegionLayout(hMsgHandle,&RegionLayout);

		RegionLayout.uiTop = SMILInfo.uiRegion_2_Top;
		RegionLayout.uiLeft = SMILInfo.uiRegion_2_Left;
		RegionLayout.uiWidth = SMILInfo.uiRegion_2_Width;
		RegionLayout.uiHeight = SMILInfo.uiRegion_2_Height;
		RegionLayout.pRegionName = SMILInfo.pRegion_2_Name;
		RegionLayout.eFit = SMILInfo.eRegion_2_Fit;
		RegionLayout.pNext = JC_NULL;
		eRet = jdi_MsgAddSMILRegionLayout(hMsgHandle,&RegionLayout);

	}
	return JC_OK;
}

JC_HANDLE  jMms_getCommHandle ()
{
	return g_pstJMmsAppInfo->hCommunicator ;
}

JC_RETCODE jMms_createMmsViewWindow ()
{
	DeviceCharacteristics stDevChars ; 
	JC_RECT				stRect ;
	INT					iErr ;
	JC_WINDOW_PROPERTIES winProp = {0 } ;

    winProp.destTaskInfo.iAppID  = E_TASK_MMS_UI ;
    winProp.destTaskInfo.iTaskID = E_TASK_MMS_UI ;

    winProp.srcTaskInfo.iAppID  = E_TASK_MMS_UI ;
    winProp.srcTaskInfo.iTaskID = E_TASK_MMS_UI ;

    //winProp.pszCaptionValue
    winProp.uiReserved = 0 ; 
    winProp.eWindowStyle = E_APPLICATION_WINDOW ;
    winProp.pApplicationData = JC_NULL ;

 
    jdd_MMIGetDeviceCharacteristics (&stDevChars) ;
    stRect.iLeft = stDevChars.uiWindowX ;
    stRect.iTop = stDevChars.uiWindowY ;
    stRect.uiWidth = stDevChars.uiRenderingWidth  ;
    stRect.uiHeight = stDevChars.uiRenderingHeight ;
	
    iErr = jdd_MMIInitialize (JC_NULL) ;
    mmi_trace (1, "@@@ jdd_MMIInitialize: %x", JRET (iErr)) ;
    if (!iErr)
    {
        iErr = jdd_MMICreateWindow (&winProp, &stRect, &(g_pstJMmsAppInfo->uiViewWindow)) ; 
		mmi_trace (1, "@@@ jdd_MMICreateWindow: %x", JRET (iErr)) ;
    }
		
   return (iErr) ;
}

void jMms_DestroyMmsViewWindow ()
{	
	if(g_pstJMmsAppInfo->uiViewWindow)
	{
		jdd_MMIDestroyWindow(g_pstJMmsAppInfo->uiViewWindow) ; 
		g_pstJMmsAppInfo->uiViewWindow = JC_NULL;
	}
	mmi_trace(1, "JDD_LOG: jdd_MMIdestroyWindow called") ;
}

JC_RETCODE	jMMS_TIMER_LISTENER_CB(MSG_HANDLE			hMsg, 
											 void				*pCallbackArg, 
											 void				*pvTimerInfo)
{
	mmi_trace(1,"JDD_LOG: jMMS_TIMER_LISTENER_CB") ;
	jdi_MsgViewHandleEvent(hMsg, E_VIEW_TIMER, NULL);
	return JC_OK ;
}


JC_RETCODE	jMms_msgComposeInit ()
{
	INT		iErr = JC_OK ;	

	if (!iErr)
	{
		iErr = jdi_MsgInit (&g_pstJMmsAppInfo->hCurrMsg, JC_NULL) ;
	}

	if (!iErr)
	{
		iErr = jMms_setMsgProperties () ;
	}

	if (!iErr)
	{
		iErr = jMMS_configMsgSmil (g_pstJMmsAppInfo->hCurrMsg) ;
	}

	if (!iErr)
	{		
		iErr = jmms_MMISetViewCallback (g_pstJMmsAppInfo->hCurrMsg);
	}
	
	if (!iErr)
	{
		iErr = jmms_MMISetComposeCallback (g_pstJMmsAppInfo->hCurrMsg);
	}
	mmi_trace (1, "@@@ jMms_msgComposeInit: %x @@@", JRET (iErr)) ;
	return (iErr) ;
}

void jmms_CreateMMSDir()
{
	JC_RETCODE	rCode = JC_OK;
	STORE_CONFIG_SETTINGS	stStoreSettings = {0, } ;				
	JC_CHAR	*pmRootDirectory ;
	
	stStoreSettings.uiMsgType = 0 ;
	jdd_MsgConfigGetDefaultStoreSettings (&stStoreSettings) ;
	if (stStoreSettings.pRootPath)
	{
		pmRootDirectory = jdi_CUtilsTcsCopyIndexNum (JC_NULL,
				stStoreSettings.pRootPath, 0, jc_tcslen (stStoreSettings.pRootPath) - 1) ;
		if (pmRootDirectory)
		{
			if (E_FALSE == jdd_FSIsDirExist (JC_NULL, pmRootDirectory))
			{
				rCode = jdd_FSMakeDir (JC_NULL, pmRootDirectory) ;
			}
			jdd_MemFree (pmRootDirectory) ;
		}	
		jdd_MemFree (stStoreSettings.pRootPath) ;
		jdd_MemFree (stStoreSettings.pRootFile) ;
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jmms_CreateMMSDir() failed") ;
	}	
}

JC_RETCODE	jMms_init_engine ()
{
	JC_RETCODE	rCode ;
	JC_CHAR		szFolderName[16] ={0,};
	JC_CHAR		*pmMMSTempFilePath ;

	rCode = jdi_StoreInit (&g_pstJMmsAppInfo->hMsgStore, 0) ;
	mmi_trace (1, "@@@ jdi_StoreInit: %x @@@", JRET (rCode)) ;
	
	if (!rCode)
	{
		if(!jmms_IsFolderExist(INBOX_FOLDER))
        {			
        	jdi_CUtilsCharToTcharMem (INBOX_FOLDER, 16, szFolderName) ;
			rCode = jdi_StoreCreateFolder(g_pstJMmsAppInfo->hMsgStore, szFolderName, 0);
			mmi_trace (1, "@@@ jdi_StoreCreateFolder:%x @@@", JRET (rCode)) ;
        }
		if(!jmms_IsFolderExist(SENT_FOLDER))
        {			
        	jdi_CUtilsCharToTcharMem (SENT_FOLDER, 16, szFolderName) ;
            rCode = jdi_StoreCreateFolder(g_pstJMmsAppInfo->hMsgStore, szFolderName, 0);
			mmi_trace (1, "@@@ jdi_StoreCreateFolder:%x @@@", JRET (rCode)) ;
        }
		if(!jmms_IsFolderExist(OUTBOX_FOLDER))
        {			
        	jdi_CUtilsCharToTcharMem (OUTBOX_FOLDER, 16, szFolderName) ;
            rCode = jdi_StoreCreateFolder(g_pstJMmsAppInfo->hMsgStore, szFolderName, 0);
			mmi_trace (1, "@@@ jdi_StoreCreateFolder:%x @@@", JRET (rCode)) ;
        }
		if(!jmms_IsFolderExist(DRAFT_FOLDER))
        {			
        	jdi_CUtilsCharToTcharMem (DRAFT_FOLDER, 16, szFolderName) ;
            rCode = jdi_StoreCreateFolder(g_pstJMmsAppInfo->hMsgStore, szFolderName, 0);
			mmi_trace (1, "@@@ jdi_StoreCreateFolder:%x @@@", JRET (rCode)) ;
        }

        #ifdef __MMI_MULTI_SIM__
		if(MTPNP_AD_MMS_Is_Avail())
        #endif
		{
			jmms_UpdateMmsIcon () ;
		}
		if (g_pstJMmsAppInfo->hMsgStore)
		{
			rCode = jdi_StoreDeinit (g_pstJMmsAppInfo->hMsgStore) ;
			g_pstJMmsAppInfo->hMsgStore = JC_NULL ;
			mmi_trace (1, "JDD_LOG: jMms_init_engine::jdi_StoreDeinit:%X", -rCode) ;
		}
	}
	if (!rCode)
	{
		jMMS_CreateReservedMMSFile () ;
	}
	rCode = jdi_CUtilsCharToTchar (JC_NULL, jdd_CFGGetMMSTempFilePath (), &pmMMSTempFilePath) ;
	if (JC_OK == rCode)
	{
		if (E_TRUE == jdd_FSIsDirExist (JC_NULL, pmMMSTempFilePath))
		{
			rCode = jdd_FSDelDir (JC_NULL, pmMMSTempFilePath) ;
			if (JC_OK != rCode)
			{
				mmi_trace (1, "JDD_LOG: jMms_init_engine () failed in Deleting the temp DIR [%X]", -rCode) ;
			}
		}
		rCode = jdd_FSMakeDir (JC_NULL, pmMMSTempFilePath) ;
		if (JC_OK != rCode)
		{
			mmi_trace (1, "JDD_LOG: jMms_init_engine () failed in Creating the temp DIR [%X]", -rCode) ;
		}
		jdd_MemFree (pmMMSTempFilePath) ;
	}
	else
	{
		mmi_trace (1, "JDD_LOG: jMms_init_engine () failed in Memory Allocation") ;
	}
	return (rCode) ;
}

void jMMS_updateDataConnSettings (ST_COMM_PROFILE *pstProxySett, ST_GENERIC_DATA_CONN *pstGenDataConn)
{
	JC_RETCODE iErr ;
	ST_COMM_SET_OPTION stCommOption = {0, } ;

	{
//		pstGenDataConn->eBearerType = E_DATA_CONN_GPRS_BEARER_TYPE ;
//		stDataConn.uiConnID = 2 ;
//		jc_strcpy(stDataConn.uDataConnType.stGPRSDataConn.acAPN, "cmwap") ;

		mmi_trace (1, "@@@ jMMS_updateDataConnSettings APN: %s", pstGenDataConn->uDataConnType.stGPRSDataConn.acAPN) ;
		mmi_trace (1, "@@@ jMMS_updateDataConnSettings Proxy: %s", pstProxySett->acHostName) ;
		mmi_trace (1, "@@@ jMMS_updateDataConnSettings Port: %d", pstProxySett->uiPortNumber) ;
		mmi_trace(1,"jMMS_updateDataConnSettings [%d]",pstGenDataConn->eBearerType) ;
		
		iErr = jdi_CommunicationDataConnection(g_pstJMmsAppInfo->hCommunicator, pstGenDataConn);

		mmi_trace (1, "@@@ jdi_CommunicationDataConnection: %x @@@", JRET (iErr));
	}

	if (!iErr)
	{
		//stCommProf.eConnMode = E_COMM_WPHTTP_DIRECT ;
//		stCommProf.eConnMode = E_COMM_WPHTTP_PROXY ;
//		jc_strcpy ( stCommProf.acHostName, "10.0.0.172") ;
//		stCommProf.uiPortNumber = 80 ;
		if(E_COMM_WSP_CONNECTION_ORIENTED == pstProxySett->eConnMode )
		{
			stCommOption.iTimeOut = MMS_SENDRECV_TIMEOUT ;
        		stCommOption.iMaxRetries = MMS_RETRIES ;
		}
		else
		{
			stCommOption.iTimeOut = MMS_SENDRECV_TIMEOUT *MMS_RETRIES;
        		stCommOption.iMaxRetries = 0 ;
		}
		stCommOption.iMaxTcpRecvBuf = 5*1024;
		
        jdi_CommunicationSetOption (g_pstJMmsAppInfo->hCommunicator, &stCommOption) ;
		iErr = jdi_CommunicationProfile (g_pstJMmsAppInfo->hCommunicator, pstProxySett) ;
		mmi_trace (1, "@@@ jdi_CommunicationProfile: %x @@@", JRET (iErr));	 
	}
}
extern void jMMS_GetUpdatedSetting() ;
void jMms_comm_init ()
{
	INT						iErr ;
	JC_TASK_INFO			stTaskInfo ;
	JC_TASK_INFO    		stTrTaskInfo ;
	ST_COMM_PROFILE 		stCommProf ;
    ST_GENERIC_DATA_CONN 	stDataConn ;

	jc_memset (&stTaskInfo, 0, sizeof (JC_TASK_INFO)) ;
	jc_memset (&stTrTaskInfo, 0, sizeof (JC_TASK_INFO)) ;
	jc_memset (&stCommProf, 0, sizeof (ST_COMM_PROFILE)) ;
	jc_memset (&stDataConn, 0, sizeof (ST_GENERIC_DATA_CONN)) ;
	
	stTaskInfo.iTaskID		= (JC_INT32) E_TASK_MMS_CORE;
	stTaskInfo.iAppID		= (JC_INT32) E_TASK_MMS_UI ;
	stTrTaskInfo.iTaskID	= (JC_INT32) E_TASK_TRANSPORT ;
	stTrTaskInfo.iAppID		= (JC_INT32) E_TASK_TRANSPORT ;


	iErr = jdi_CommunicationInitialize (&stTaskInfo, &stTrTaskInfo, JC_NULL, jMMS_CommCB, 
										JC_NULL, &g_pstJMmsAppInfo->hCommunicator) ;
	
    mmi_trace (1, "@@@ jdi_CommunicationInitialize: %x @@@", JRET (iErr));
    
	
	// initlisation needed only for single SIM . as in dual SIM profile 
	// updation is done each time when we send/recv 
#ifndef __MMI_MULTI_SIM__
	jMMS_SettingreadUpdateDataConnInfo();
#endif 
	jdd_NetInitialize () ;
	jmms_ReadFromSettingsFile ();
}

void jMms_init_context ()
{
	jmms_CreateMMSDir();
	g_pstJMmsAppInfo = jdd_MemAlloc (1, sizeof (JMMS_APP_INFO)) ;

	if (g_pstJMmsAppInfo)
	{
		g_pstJMmsAppInfo->pcInputBuff = jdd_MemAlloc (jMMS_MAX_INPUT_SIZE + 1, sizeof(JC_CHAR)) ;
	}
	g_pstJMmsAppInfo->pActiveTextBuffer = (JC_CHAR *)jdd_MemAlloc(JMMS_MAX_TEXT_PER_SLIDE+1,sizeof(JC_CHAR) ) ;   
	g_pstJMmsAppInfo->bIsViolatingContentClass = E_FALSE ;
	g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_NONE ;
	g_pstJMmsAppInfo->eCurrentFolderID = E_JMMS_NONE_FOLDER ;
	//g_pstJMmsAppInfo->ePreviewState = E_JMMS_COMM_IDLE ;
}

JC_HANDLE  jmms_getWinHandle ()
{
	return (JC_HANDLE)g_pstJMmsAppInfo->uiViewWindow;
}

JC_BOOLEAN jMMS_IsMMSAppOrDownloadActive (void)
{
	if (g_pstJMmsAppInfo && JC_NULL == g_pstJMmsAppInfo->pReqRespList &&
			(E_JMMS_COMM_IDLE == g_pstJMmsAppInfo->eCommunicatorState ||
			E_JMMS_COMM_DISCONNECTED == g_pstJMmsAppInfo->eCommunicatorState) &&
		E_FALSE == jMMS_GetMMSAppStatus ())
	{
		return E_FALSE ;
	}
	else
	{
	//	mmi_trace (1, "jMMS_IsMMSAppOrDownloadActive >> [%d], [%d]",
	//		g_pstJMmsAppInfo->pReqRespList, g_pstJMmsAppInfo->eCommunicatorState);
		return E_TRUE ;
	}
}

JC_UINT32 jMMS_GetFontHeight(void)
{
	return Get_FontHeight(MMI_medium_font, (U8) gCurrLangIndex);
}


JC_BOOLEAN jMMS_IsMMSAutoDownloading(U8 nSIMId)
{
	if(nSIMId == 0xff 
#ifdef __MMI_MULTI_SIM__
		|| g_pstJMmsAppInfo->send_simid == nSIMId
#endif
)
	{
		if(jMMS_IsMMSAppOrDownloadActive())
		{
			DisplayPopup((PU8)GetString(STR_ID_MMS_DOWNLOADING) ,
				IMG_GLOBAL_PROGRESS, 1,
				UI_POPUP_NOTIFYDURATION_TIME,
				WARNING_TONE);
			return TRUE;
		}
	}
	return FALSE;
}

#ifdef MMI_ON_HARDWARE_P /* zhuoxz,2009-4-29 */
inline U8*jMms_getInputBuff () 
{
	return g_pstJMmsAppInfo->pcInputBuff ;
}
#else /* #ifdef MMI_ON_HARDWARE_P */
U8*jMms_getInputBuff () 
{
	return g_pstJMmsAppInfo->pcInputBuff ;
}
#endif /* #ifdef MMI_ON_HARDWARE_P */

#endif //__JATAAYU_APP__
