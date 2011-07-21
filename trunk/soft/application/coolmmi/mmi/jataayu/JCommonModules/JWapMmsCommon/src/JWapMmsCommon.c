/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
 * JProfileUIHandler.C
 *
 * Project:
 * --------
 *
 * Description:
 * ------------
 *   This is source file for UI handling for the profile handler
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#define MODULE_NAME "JWAPMMSCOMMON"
#define FILE_NAME "JWapMmsCommon.c"

#ifdef JATAAYU_SUPPORT

#include "jwap_inc.h"


#include "jdi_browserengine.h"
#include "jwap_cb.h"
#include "jdi_cutils.h"
#include "jdi_arr.h"
#include "jdi_displayinfo.h"
#include "jddex_platform.h"
#include "jdi_history.h"

#include "jddex_mmieventhandler.h"
#include "jddex_wnddefaults.h"

#include "jdi_communicator.h"
#include "jdi_browserengine.h"
#include "jwap_bookmark.h"
#include "jwap_interface.h"
#include "jdd_netdatatype.h"
#include "jdd_netapi.h"
#include "jdd_dataconnection.h"
#include "jddex_dataconnection.h"

#include "custom_nvram_editor_data_item.h"
#include "nvram_user_defs.h"
#include "custom_mmi_default_value.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "ts.h"
#include "jmms_init.h"
#include "jdd_deconfig.h"
#include "custom_nvram_editor_data_item.h"
#include "jprofilehandlertypes.h"
#include "jprofilehandlerprots.h"
#include "settingprofile.h"
#include "jdi_downloadclient.h"

#ifdef JATAAYU_SUPPORT_WAP
	extern JWAP_APP_INFO *g_pstJWapAppInfo ;
#endif

UINT32 COS_SendEventToJataayu( COS_EVENT* pCosEvent )
{
	JC_EVENT_DATA *pstEventData = JC_NULL ;

	pstEventData = 	(JC_EVENT_DATA *)(pCosEvent->nParam1) ;

	mmi_trace(1,(PCSTR)"*****IN COS_SendEventToJataayu*****") ;

	mmi_trace (1, (PCSTR)"@@@ Src:%d  Dest:%d  Event: %d @@@", \
		pstEventData->srcTaskInfo.iTaskID, pstEventData->destTaskInfo.iTaskID, \
		pstEventData->iEventType) ;

	if (pstEventData)
	{
		if (E_TASK_BROWSER_CORE == pstEventData->destTaskInfo.iTaskID ||
			E_TASK_BROWSER_UI == pstEventData->destTaskInfo.iTaskID)
		{
			#ifdef JATAAYU_SUPPORT_WAP
			if (g_pstJWapAppInfo && g_pstJWapAppInfo->hBECore)
			{
				if (E_TASK_TRANSPORT == pstEventData->srcTaskInfo.iTaskID)
				{
					jdi_BESetEvent(g_pstJWapAppInfo->hBECore, E_BE_TRANSPORT_EVENT, pstEventData) ;
				}
				else if (E_TASK_BROWSER_CORE == pstEventData->destTaskInfo.iTaskID)
				{

					switch (pstEventData->iEventType)
					{
						case E_BE_CACHE_RETRIEVE:
						case E_BE_CACHE_HEADER_RETRIEVE:
						case E_BE_FILE_RETRIEVE:
						  jdi_BESetEvent (g_pstJWapAppInfo->hBECore, (E_BE_EVENT_TYPE)pstEventData->iEventType, pstEventData) ;
						  break ;

						case E_BE_TIMER:
						  jdi_BESetEvent (g_pstJWapAppInfo->hBECore, (E_BE_EVENT_TYPE)pstEventData->iEventType,pstEventData->pEventInfo) ;
						  break ;

						default :
						  break;
					}
					jdd_QueueFree (pstEventData) ;
				}
				else
				{
					/* Event from WAP UI to WAP UI */
					mmi_trace(1,"jwap else case for event" );
					switch (pstEventData->iEventType)
					{
						case E_WAP_APP_EXIT:
							jWap_processAppEvent (pstEventData) ;
							break ;
						default :
							mmi_trace (1, "DANGER Unhandled WAP UI event [%d] to WAP UI",
								pstEventData->iEventType) ;
						  break;
					}
					jdd_QueueFree (pstEventData) ;
				}
			}
			else
			{
				mmi_trace (1, "DANGER Unhandled transport event [%d] to browser",
					pstEventData->iEventType) ;
				/*if (E_TR_DISCONNECT_RESP_EVENT == pstEventData->iEventType ||
					E_TR_PROGRESS_IND_EVENT == pstEventData->iEventType)*/
				{
					jdd_QueueFree (pstEventData) ;
				}
			}
			#endif //end JATAAYU_SUPPORT_WAP
		}
		else if (E_TASK_MMS_CORE == pstEventData->destTaskInfo.iTaskID ||
			E_TASK_MMS_UI == pstEventData->destTaskInfo.iTaskID)
		{
		#ifdef JATAAYU_SUPPORT_MMS
			if (E_TASK_TRANSPORT == pstEventData->srcTaskInfo.iTaskID)
			{
				jdi_CommunicationEventHandler (jMms_getCommHandle (), pstEventData) ;
			}
			else
			{
				/* MMS APPLICATION TO HANDLE */
				jMMS_processAppEvent(pstEventData) ;
				jdd_QueueFree(pstEventData) ;
			}
		#endif	
		}
		#ifdef JOTA_PROVISION_APP_ENABLE
		else if (E_TASK_WAP_PROVISIONING_CORE == pstEventData->destTaskInfo.iTaskID ||
			E_TASK_WAP_PROVISIONING_UI == pstEventData->destTaskInfo.iTaskID)
		{
			extern JC_HANDLE  prv_getCommHandle ();
			if (E_TASK_TRANSPORT == pstEventData->srcTaskInfo.iTaskID)
			{
				jdi_CommunicationEventHandler (prv_getCommHandle (), pstEventData) ;
			}
			else
			{
				/* provosioning  application to handle */

			}
		}
		#endif
		else
		{
			mmi_trace (1, "DANGER Unhandled Non-WAP-MMS event [%d] Caught",
					pstEventData->iEventType) ;
			jdd_QueueFree (pstEventData) ;
		}
		jdd_MemFree (pstEventData) ;
	}
	return ERR_SUCCESS ;
}

JC_UINT32 jwap_GetLcdWidth()
{
	return MAIN_LCD_DEVICE_WIDTH;
}

JC_UINT32 jwap_GetLcdHeight()
{	
	return MAIN_LCD_DEVICE_HEIGHT;
}

JC_UINT32 jwap_GetSoftkeyHeight()
{	
#ifdef LCDSIZE_220_176
	return (MMI_BUTTON_BAR_HEIGHT+2);
#else

	return MMI_BUTTON_BAR_HEIGHT;
#endif

}

JC_UINT32 jwap_GetTitleHeight()
{
#ifdef LCDSIZE_240_320	
	return (MMI_TITLE_HEIGHT + MMI_STATUS_BAR_HEIGHT);
#elif LCDSIZE_176_220
	return (MMI_TITLE_HEIGHT + MMI_STATUS_BAR_HEIGHT);
#elif LCDSIZE_220_176
	return (/*MMI_TITLE_HEIGHT +*/ MMI_STATUS_BAR_HEIGHT);
#elif LCDSIZE_128_160
	return MMI_TITLE_HEIGHT ;
#elif LCDSIZE_160_128
	return MMI_TITLE_HEIGHT ;
#endif
}

JC_UINT32 jwap_GetScrollbarWidth()
{
	return (16);
}

JC_UINT32 jwap_GetScrollbarbarArrowHight()
{
	return (14);
}

JC_UINT32 jwap_GetScrollbarbarArrowWidth()
{
	return (14);
}

extern JC_BOOLEAN jMMS_GetPrimaryMMIStatus();

JC_BOOLEAN jApp_isSecMmiActive ()
{

#ifdef JATAAYU_SUPPORT_MMS
   // check whether the active application is MMS
	if(jMMS_GetMMSAppStatus())
	{
	 	  return (!jMMS_GetPrimaryMMIStatus());
	}
#endif
#ifdef JATAAYU_SUPPORT_WAP
	if(jWap_GetWapAppStatus())
	{
		mmi_trace (1, (PCSTR)"@@@ In jApp_isSecMmiActive:%d @@@", !g_pstJWapAppInfo->bIsPriMmiActive) ;
		return (!g_pstJWapAppInfo->bIsPriMmiActive) ;
	}
#endif
	return (0);
}

extern JC_HANDLE jmms_getWinHandle();
JC_UINT32 jwap_GetPrimaryWindow()
{
    JC_UINT32   hWin = 0;

#ifdef JATAAYU_SUPPORT_MMS	
	// check if the active application is MMS
    if (jMMS_GetMMSAppStatus()) 
	{
		hWin = (JC_UINT32)jmms_getWinHandle();
		return hWin;
	}
#endif
#ifdef JATAAYU_SUPPORT_WAP
	if (jWap_GetWapAppStatus()) 
		{   

		//if wap is active app
		hWin = g_pstJWapAppInfo->uiWinId;
		return hWin;
	}
#endif
	return hWin;
}

#endif /*JATAAYU_SUPPORT*/ 

