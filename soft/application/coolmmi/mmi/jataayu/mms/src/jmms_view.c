
/***************************************************************************
*
* File Name : jmms_view.c
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
* THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
* KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
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
**************************************************************************
*
* File Description
* ----------------
*
* Purpose			: All the view & preview API's are present in this file
* Created By		: Ranjeet 
* Created Date		: 31st April 2009
*
***************************************************************************
* Revision Details
* ----------------
* 
* 1. Modified By, Modified Date, Purpose
*
* Ranjeet		 31st April 2009	Initial version
*
***************************************************************************/
#ifdef JATAAYU_SUPPORT
#include "jmms_inc.h"
#include "jmms_staticdefines.h"
#include "jmms_composedlg.h"
#include "jmms_composeutils.h"
#include "jmms_communicator.h"
#include "jmms_cb.h"
#include "datetimegprot.h"
#include "phonesetupgprots.h"
#include "messagesresourcedata.h"
#include "phonebookgprot.h"
#include "mdi_audio.h"
#include "wgui_touch_screen.h"
#include "jmms_mmi_cb.h"
#include "jmms_composedlg.h"
#include "jmms_addrdlg.h"
#include "filemanagergprot.h"

extern INT g_iSelectIndex ;
extern JC_UINT8	g_uiAttachListCount ;
extern JC_UINT8	**g_pListItems ;
extern JC_UINT8	g_uiMediaListCount ;
extern JC_UINT8	**g_pMediaListItems ;
#define		JMMS_FILENAME_MAX	41
#define  	JMMS_SAVE_ATTACH_PATH "/t/wap_download"
#define 	MAX_LEN_OF_EXT_NAME 4
JC_CHAR 	gTempExtName[MAX_LEN_OF_EXT_NAME + 1];

#ifdef __MMI_MAINLCD_176X220__
#define MMS_PROPERTIES_SEPARATOR "\n------------\n"
#else
#define MMS_PROPERTIES_SEPARATOR "\n-----------------\n"
#endif


#ifdef __cplusplus
extern "C" {
#endif
void DestroyVideoHandle();

#ifdef __cplusplus
}
#endif


typedef enum _jwap_mms_view_option_items
{
	E_MMS_OPTION_PLAY,
	E_MMS_OPTION_SAVE_MEDIA,
	E_MMS_OPTION_SAVE_ATTACHMENT,
	E_MMS_OPTION_VIEW_ITEMS,
	
}JWAP_MMS_VIEW_OPTION_ITEMS ;

extern void GBSetInputboxLSKFunction(FuncPtr f);
void jmms_SetFilename (void);
/* Explicitly declared here, but it should be declared in Worldclock.h */
S32 GetTimeZone (U8 cityIndex) ;

void jmms_ShowSubject(JC_CHAR *pSubject)
{
		jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_JMMS_SUBJECT)) ;
		jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
		if(pSubject)
		{
			jc_tcsncat(g_pstJMmsAppInfo->pszScreeBuff, pSubject, \
						jc_wstrlen(pSubject)) ;
		}
		else
		{
			jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
						(JC_CHAR*)GetString (STR_GLOBAL_EMPTY_LIST)) ;
		}
		jdi_CUtilsTcsStrCat(MMS_PROPERTIES_SEPARATOR, g_pstJMmsAppInfo->pszScreeBuff) ;		
}

void jmms_ShowVersion(JC_INT8 *pVersion)
{

	return;//don't display version information

	
	if(pVersion)//version
	{
		jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_MMS_MSG_VERSION)) ;
		jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
		jdi_CUtilsTcsStrCat(pVersion, g_pstJMmsAppInfo->pszScreeBuff) ;
		jdi_CUtilsTcsStrCat(MMS_PROPERTIES_SEPARATOR, g_pstJMmsAppInfo->pszScreeBuff) ;		
	}

}

void jmms_ShowFrom(JC_CHAR *pFrom)
{
	JC_CHAR *phName = NULL;
	if(pFrom)//from
	{
		jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_MMS_MSG_FROM)) ;
		jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
		
		phName = (JC_CHAR *)lookUpNumber((PS8)pFrom);
		if (phName != NULL)
		{			
			jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, phName) ;
		}
		else
		{
			jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, pFrom) ;
		}
		jdi_CUtilsTcsStrCat(MMS_PROPERTIES_SEPARATOR, g_pstJMmsAppInfo->pszScreeBuff) ;		
	}

}

void jmms_ShowCC(JC_CHAR *pCc)
{
	JC_CHAR *phName = NULL;
	JC_INT8 cCcCount  = 0;
	JC_CHAR	*addr_list_cc[10];
	JC_INT32 i;
	JC_UINT8 acTmpSemi[4];

	AnsiiToUnicodeString(acTmpSemi, ";");
	memset(addr_list_cc, 0, sizeof(addr_list_cc));
	
	if(pCc)//cc
	{
		jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_JMMS_CC)) ;
		jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;

		/*
		phName = (JC_CHAR *)lookUpNumber((PS8)pCc);

		if (phName != NULL)
		{			
			jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, phName) ;
		}
		else
		{
			jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, pCc) ;
		}
		*/

		cCcCount = jMms_addrFormListFromAddrBuff(pCc, addr_list_cc);
		MMI_ASSERT(cCcCount <= 10);
		for(i = 0; i < cCcCount; i++)
		{
			if(addr_list_cc[i] != JC_NULL)
			{
				phName = (JC_CHAR *)lookUpNumber((PS8)(addr_list_cc[i]));
				if (phName != NULL)
				{			
					jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, phName) ;
				}
				else
				{
					jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, addr_list_cc[i]) ;
				}
				jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)acTmpSemi);

				jdd_MemFree(addr_list_cc[i]);
			}
		}


		jdi_CUtilsTcsStrCat(MMS_PROPERTIES_SEPARATOR, g_pstJMmsAppInfo->pszScreeBuff) ;		
	}

}
void jmms_ShowTo(JC_CHAR *pTo)
{
	JC_CHAR *phName = NULL;
	JC_INT8 cToCount  = 0;
	JC_CHAR	*addr_list_to[10];
	JC_INT32 i;
	JC_UINT8 acTmpSemi[4];

	AnsiiToUnicodeString(acTmpSemi, ";");

	memset(addr_list_to, 0, sizeof(addr_list_to));
	if(pTo)//to
	{
		jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_JMMS_TO)) ;
		jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;

		/*
		phName = (JC_CHAR *)lookUpNumber((PS8)pTo);
		mmi_trace(TRUE,"guoj++ Func: %s phName =%p", __FUNCTION__, phName);
		if (phName != NULL)
		{			
			jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, phName) ;
		}
		else
		{
			jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, pTo) ;
		}
		*/
		
		cToCount = jMms_addrFormListFromAddrBuff(pTo, addr_list_to);
		MMI_ASSERT(cToCount <= 10);
		for(i = 0; i < cToCount; i++)
		{
			if(addr_list_to[i] != JC_NULL)
			{
				phName = (JC_CHAR *)lookUpNumber((PS8)(addr_list_to[i]));
				if (phName != NULL)
				{			
					jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, phName) ;
				}
				else
				{
					jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, addr_list_to[i]) ;
				}
				jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)acTmpSemi);

				jdd_MemFree(addr_list_to[i]);
			}
		}
		

		jdi_CUtilsTcsStrCat(MMS_PROPERTIES_SEPARATOR, g_pstJMmsAppInfo->pszScreeBuff) ;		
	}

}

void jmms_ShowBcc(JC_CHAR *pBcc)
{
	JC_CHAR *phName = NULL;
	JC_INT8 cbccCount  = 0;
	JC_CHAR	*addr_list_bcc[10];
	JC_INT32 i;
	JC_UINT8 acTmpSemi[4];

	AnsiiToUnicodeString(acTmpSemi, ";");
	memset(addr_list_bcc, 0, sizeof(addr_list_bcc));

	if(pBcc)//bcc
	{
		jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_JMMS_BCC)) ;
		jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
		/*
		phName = (JC_CHAR *)lookUpNumber((PS8)pBcc);

		if (phName != NULL)
		{			
			jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, phName) ;
		}
		else
		{
			jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, pBcc) ;
		}
		*/
		cbccCount = jMms_addrFormListFromAddrBuff(pBcc, addr_list_bcc);
		MMI_ASSERT(cbccCount <= 10);
		for(i = 0; i < cbccCount; i++)
		{
			if(addr_list_bcc[i] != JC_NULL)
			{
				phName = (JC_CHAR *)lookUpNumber((PS8)(addr_list_bcc[i]));
				if (phName != NULL)
				{			
					jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, phName) ;
				}
				else
				{
					jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, addr_list_bcc[i]) ;
				}
				jc_tcscat(g_pstJMmsAppInfo->pszScreeBuff, (JC_CHAR*)acTmpSemi);

				jdd_MemFree(addr_list_bcc[i]);
			}
		}
		jdi_CUtilsTcsStrCat(MMS_PROPERTIES_SEPARATOR, g_pstJMmsAppInfo->pszScreeBuff) ;		
	}

}
void jmms_ShowClass(EMessageClass eMsgClass)
{
	//class
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_JMMS_MSG_CLASS)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;	
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (MessageClassStr[eMsgClass])) ;
	jdi_CUtilsTcsStrCat(MMS_PROPERTIES_SEPARATOR, g_pstJMmsAppInfo->pszScreeBuff) ;		

}
void jmms_ShowPriority(EMessagePriority eMsgPriority)
{
	//priority
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_JMMS_PRIORITY)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;	
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (MessagePriorityStr[eMsgPriority])) ;
	jdi_CUtilsTcsStrCat(MMS_PROPERTIES_SEPARATOR, g_pstJMmsAppInfo->pszScreeBuff) ;		
}
void jmms_ShowDateTime(JC_UINT32 nDateTime, JC_BOOLEAN bRelative)
{
	//date & time
	mmi_trace (1, "JDD_LOG: jmms_ShowDateTime >>nDateTime is [%d]", nDateTime) ;
	if(nDateTime != 0)
	{
		if (bRelative)
		{
			jMMS_FormDateTimeStringFromSecs (nDateTime + (GetTimeZone(PhnsetGetHomeCity()) * 60 * 60), g_pstJMmsAppInfo->pszScreeBuff) ;
		}
		else
		{
			jMMS_FormDateTimeStringFromSecs (nDateTime, g_pstJMmsAppInfo->pszScreeBuff) ;
		}
		jdi_CUtilsTcsStrCat(MMS_PROPERTIES_SEPARATOR, g_pstJMmsAppInfo->pszScreeBuff) ;		
	}
}
void jmms_ShowSize(JC_UINT32 uiBufferLen)
{
	//size
	JC_INT8	TempString[14] = {0, };
		
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_JMMS_SIZE)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;	
	jc_itoa(uiBufferLen/1024, TempString, 10) ;
	jdi_CUtilsTcsStrCat(TempString, g_pstJMmsAppInfo->pszScreeBuff) ;
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_JMMS_KB)) ;
//	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;

}
void jmms_ShowDeliveryReport(JC_BOOLEAN bDeliveryReport)
{
	//delivery report
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_MMS_DELIVERY_REPORT)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
	if(bDeliveryReport == E_FALSE)
		jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_GLOBAL_NO)) ;
	else
		jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_GLOBAL_YES)) ;	
	jdi_CUtilsTcsStrCat(MMS_PROPERTIES_SEPARATOR, g_pstJMmsAppInfo->pszScreeBuff) ;		
}

void jmms_ShowReadReport(JC_BOOLEAN bReadReport)
{
	//read report
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_MMS_DELIVERY_REPORT)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;
	if(bReadReport == E_FALSE)
		jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_GLOBAL_NO)) ;
	else
		jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_GLOBAL_YES)) ;
	jdi_CUtilsTcsStrCat(MMS_PROPERTIES_SEPARATOR, g_pstJMmsAppInfo->pszScreeBuff) ;		
}
void jmms_ShowRetrieveStatus(JC_CHAR *pStatusText)
{
	if(pStatusText)//status
	{
		jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_MMS_MSG_STATUS)) ;
		jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;	
		jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, pStatusText) ;
		jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;		
		jdi_CUtilsTcsStrCat(MMS_PROPERTIES_SEPARATOR, g_pstJMmsAppInfo->pszScreeBuff) ;		
	}
}
void jmms_ShowDeliveryStatus(EMessageStatus eDeliveryStatus)
{
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_MMS_MSG_STATUS)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;	
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (MM1StatusText[eDeliveryStatus])) ;
}

void jmms_ShowReadReportStatus(EMessageReadStatus eReadStatus)
{
	//status
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (STR_ID_MMS_MSG_STATUS)) ;
	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;	
	jc_wstrcat (g_pstJMmsAppInfo->pszScreeBuff, \
					(JC_CHAR*)GetString (ReadReportStatus[eReadStatus])) ;
//	jdi_CUtilsTcsStrCat (JMMS_NEW_LINE, g_pstJMmsAppInfo->pszScreeBuff) ;	
}

void jmms_ViewMsgFromFolder ()
{
	JC_RETCODE			retCode ;
	CONTENT_DATA		stViewContent = {0, } ;	
	MESSAGE_LIST		*pstMsgDetail = JC_NULL ;
	
	/*retCode = jmms_GetMsgDetailsByIndex (g_pstJMmsAppInfo->hMsgStore,
					g_pstJMmsAppInfo->eCurrentFolderID, g_pstJMmsAppInfo->iFolderItemIndex, &pstMsgDetail) ;
	*/
	retCode = jmms_GetMsgDetailsByID(g_pstJMmsAppInfo->hMsgStore, g_pstJMmsAppInfo->eCurrentFolderID, 
						g_pstJMmsAppInfo->uiHighlightedMsgId, &pstMsgDetail) ;
	mmi_trace(1,"g_pstJMmsAppInfo->uiHighlightedMsgId [%d]",g_pstJMmsAppInfo->uiHighlightedMsgId);
					
	if (retCode == JC_OK && JC_NULL != pstMsgDetail)
	{
		if (pstMsgDetail->pMessageHeaders->eMsgType == E_MSG_TYPE_DELIVERY_REPORT)
		{
			mmi_trace(1, "JDD_LOG: unread/read : %d ", pstMsgDetail->bReadFlag);

			jmms_SetReadFlag(pstMsgDetail);
			jmms_MMIDisplayDeliveryReport(pstMsgDetail->pMessageHeaders);
		}
		else if (pstMsgDetail->pMessageHeaders->eMsgType == E_MSG_TYPE_READ_REPORT_RESPONSE)
		{			
			jmms_SetReadFlag(pstMsgDetail);			
			jmms_MMIDisplayReadResponse(pstMsgDetail->pMessageHeaders);
		}
		else if (pstMsgDetail->pMessageHeaders->eMsgType == E_MSG_TYPE_NOTIFICATION)
		{
			MESSAGE_NOTIFICATION	*pNotification = pstMsgDetail->pMessageHeaders->pHeaders ;
			JC_UINT32				uiMsgIndex =  pstMsgDetail->uiMessageID ;

			mmi_trace(1,"JDD_LOG: jmms_ViewMsgFromFolder MMS Download[%s] MsgID [%d]",pNotification->pContentLocation, uiMsgIndex) ;
			if (0 != uiMsgIndex)
			{
				ST_REQUEST_LIST *pNode = jMMS_GetReqRespNodeByMsgID (uiMsgIndex) ;
				if (pNode && (E_JMMS_NODE_AUTO_RETRIEVE == pNode->eNodeState))
				{
					retCode = JC_ERR_QUEUE_EXISTS ;
				}
				else
				{
					if(jMMS_IsReqNodeUnderProgress())
					{
						DisplayPopup((PU8)GetString(STR_ID_MMS_WAIT_AND_RETRY), IMG_GLOBAL_WARNING, 1,	2000, 1) ;
						return;
					}
//					show_progress_screen (E_JMMS_NODE_RETRIEVE, 0) ;
					mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
					retCode = jMMS_ProcessCommunicatorFetchRequest (uiMsgIndex, E_JMMS_NODE_RETRIEVE, E_JMMS_MSG_NIL) ;
					if (JC_OK != retCode)
					{
						jMMS_ErrorHandler (retCode, NULL) ;
						jmms_CallbackGoBackHistory();
					}
					/* This case is not view case. So the retCode should not be propagated further */
					retCode = JC_OK ;
				}
			}
		}
		else	
		{
			MESSAGE_RETRIEVAL	*pstRetrievHdr ;

			pstRetrievHdr = (MESSAGE_RETRIEVAL*)pstMsgDetail->pMessageHeaders->pHeaders ;
            // initliase readreport flag
			g_pstJMmsAppInfo->bSendReadReport = E_FALSE;
			mmi_trace(1, "JDD_LOG: unread/read : %d readreporttosend : %d", 
							pstMsgDetail->bReadFlag, pstRetrievHdr->bReadReport);
			if (pstMsgDetail->bReadFlag == E_FALSE )
			{
				/*
				if((g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport == E_MSG_SETTINGS_READ_REPORT_SEND)||
					( pstRetrievHdr->bReadReport&& g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport == \
					E_MSG_SETTINGS_READ_REPORT_ONREQUEST))
				*/	
			   if(pstRetrievHdr->bReadReport &&
			   		(g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport == E_MSG_SETTINGS_READ_REPORT_SEND|| 
			   		g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport == E_MSG_SETTINGS_READ_REPORT_ONREQUEST))
				{
					g_pstJMmsAppInfo->bSendReadReport = E_TRUE;
					mmi_trace(1, "JDD_LOG: read report to be sent %d:%d", 
					g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport,pstRetrievHdr->bReadReport );
				}
				else
				{
					mmi_trace(1, "JDD_LOG: read report NOT to be sent %d:%d", 
					g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport,pstRetrievHdr->bReadReport );
				}
				 
				jmms_SetReadFlag(pstMsgDetail);			
			}
			retCode = jdi_MsgInit (&g_pstJMmsAppInfo->hCurrMsg, NULL) ;
			if (retCode == JC_OK)
			{
				retCode = jdi_MsgSetMessagingType (g_pstJMmsAppInfo->hCurrMsg, E_TYPE_MMS) ;
				
				if (retCode == JC_OK)
				{
					stViewContent.bIsStream = E_FALSE;
					stViewContent.cbWriteCallback = jmms_FileStreamWriteCB;
					stViewContent.cbReadCallback = FileStreamReadCB;
					retCode = jMMS_GetMsgFromStoreByMsgID (g_pstJMmsAppInfo->hMsgStore,
						g_pstJMmsAppInfo->eCurrentFolderID, pstMsgDetail->uiMessageID, &stViewContent) ;
					if (retCode == JC_OK)
					{						
						retCode = jmms_ViewMms ( &stViewContent, E_TRUE) ;
						mmi_trace(1, "JDD_LOG: jmms_ViewMms Fail rcode %d:",retCode);
						if (stViewContent.pFileName != JC_NULL)
						{
							jdd_MemFree (stViewContent.pFileName) ;
						}
						
						if (stViewContent.pBuffer)
						{
							jdd_MemFree(stViewContent.pBuffer) ;
							stViewContent.pBuffer = JC_NULL ;
						}
					}
				}
			}	

			
		}	
		jmms_UpdateMmsIcon();
	}
	if (retCode != JC_OK)
	{
		if (FILE_MODULE == ((-retCode) & 0x00FFFF00) && JC_ERR_FILE_NOT_FOUND != retCode)
		{
			DisplayPopup (GetString (STR_ID_MMS_FILE_SYSTEM_FULL), \
						IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			jmms_CallbackGoBackHistory();
		}
		else if (JC_ERR_QUEUE_EXISTS == retCode)
		{
			DisplayPopup (GetString (STR_ID_MMS_DOWNLOADING), \
						IMG_GLOBAL_INFO, 1, 2000, 1) ;
			jmms_CallbackGoBackHistory();
		}
		else
		{
			DisplayPopup (GetString (STR_ID_MMS_INF_VIEW_FAIL), \
						IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			jmms_CallbackGoBackHistory();
		}
	}
}

void jmms_GoBackToHistoryAfterView()
{
	jmms_MMIViewDeInit();
	DeleteNHistory(1);
	GoBackHistory();
}

void jmms_MMISendReportDone()
{
	JC_RETCODE rCode = JC_OK;

	rCode = jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg,E_VIEW_CLOSE,JC_NULL);

	jMMS_SetPrimaryMMIStatus(E_FALSE);	

	jmms_MMISendReadreport();
	jmms_GoBackToHistoryAfterView();
}
void jmms_MMISendReadreport()
{	
	MESSAGE_RETRIEVAL	*pstRetrievHdr ;
	MESSAGE_LIST		*pstMsgDetail = NULL ;
	
	jmms_GetMsgDetailsByID (g_pstJMmsAppInfo->hMsgStore, g_pstJMmsAppInfo->eCurrentFolderID,
									g_pstJMmsAppInfo->uiHighlightedMsgId, &pstMsgDetail) ;
//temply solve bug 14941	
	if(!pstMsgDetail)
	{
		DisplayIdleScreen();
		return;
	}
	pstRetrievHdr = (MESSAGE_RETRIEVAL*)pstMsgDetail->pMessageHeaders->pHeaders ;
 	g_pstJMmsAppInfo->bSendReadReport = E_FALSE;	
	if (0 != pstMsgDetail->uiMessageID)
	{
		jMMS_ProcessCommunicatorFetchRequest (pstMsgDetail->uiMessageID, jMMS_SendReadReport (pstRetrievHdr->pVersion), E_JMMS_MSG_NIL) ;
	}
}

void jmms_MMIReadreportCancel()
{
	JC_RETCODE rCode = JC_OK;

	rCode = jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg,E_VIEW_CLOSE,JC_NULL);

	jMMS_SetPrimaryMMIStatus(E_FALSE);	

	g_pstJMmsAppInfo->bSendReadReport = E_FALSE;
	jmms_GoBackToHistoryAfterView();
}

JC_RETCODE	jmms_MMIDisplayDeliveryReport(MESSAGE_HEADERS *pMessageHdr)
{
    JC_RETCODE      eRet            = JC_OK ;
    MESSAGE_HEADERS *pTempMsgHdr    = NULL ; 
	
    pTempMsgHdr = (MESSAGE_HEADERS *)pMessageHdr ;
	g_pstJMmsAppInfo->pszScreeBuff = jdd_MemAlloc(JMMS_MAX_SCREEN_BUFFER + 1 , sizeof(JC_CHAR));
    if(pTempMsgHdr != NULL)
    {
        DELIVERY_REPORT *pDelRep = (DELIVERY_REPORT *)pTempMsgHdr->pHeaders ;

#if 0
        //jdi_CUtilsTcsStrCpy("Delivery report\nMsg-Id: ", g_pstJMmsAppInfo->pszScreeBuff) ;
        jc_wstrcpy (g_pstJMmsAppInfo->pszScreeBuff, GetString (STR_ID_MMS_DELIVERY_REPORT)) ;
		jdi_CUtilsTcsStrCat(" :\n\n", g_pstJMmsAppInfo->pszScreeBuff) ;
#endif /* Guoj delete. It's not used now at 2009-11-5 */
#if 0
		// STR_ID_MMS_MESSAGE_ID [Message-ID]		
		jc_wstrcpy (g_pstJMmsAppInfo->pszScreeBuff, GetString (STR_ID_MMS_MESSAGE_ID)) ;		
        jdi_CUtilsTcsStrCat(pDelRep->pMessageID, g_pstJMmsAppInfo->pszScreeBuff) ;
		jdi_CUtilsTcsStrCat("\n", g_pstJMmsAppInfo->pszScreeBuff) ;
#endif		

		jmms_ShowTo(pDelRep->pTo);
		jmms_ShowDateTime(pDelRep->uiDate, E_TRUE);
		jmms_ShowDeliveryStatus(pDelRep->eDeliveryStatus);

        jmms_MMIReportScreenEntryHandler();
    }
    return eRet ;
}

JC_RETCODE	jmms_MMIDisplayReadResponse(MESSAGE_HEADERS *pMessageHdr)
{
    JC_RETCODE      eRet            = JC_OK ;
    MESSAGE_HEADERS *pTempMsgHdr    = NULL ;

    pTempMsgHdr = (MESSAGE_HEADERS *)pMessageHdr ;
    if(pTempMsgHdr != NULL)
    {
		READ_REPORT_RESPONSE  *pReadRes = (READ_REPORT_RESPONSE *)pTempMsgHdr->pHeaders ;	
		g_pstJMmsAppInfo->pszScreeBuff = jdd_MemAlloc(JMMS_MAX_SCREEN_BUFFER + 1 , sizeof(JC_CHAR));
#if 0
        jc_wstrcpy (g_pstJMmsAppInfo->pszScreeBuff, GetString (STR_ID_MMS_READ_REPORT)) ;
        jdi_CUtilsTcsStrCpy(" :\n\n", g_pstJMmsAppInfo->pszScreeBuff) ;
#endif /* Guoj delete. It's not used now at 2009-11-5 */	

		jmms_ShowFrom(pReadRes->pFrom);
        jmms_ShowDateTime(pReadRes->uiReceivedTime, E_TRUE);
		jmms_ShowReadReportStatus(pReadRes->eReadStatus);

        jmms_MMIReportScreenEntryHandler();
    }
    return eRet ;
}

/**
 * @ingroup    MMS_MMI
 * @brief      Calls the view module apis to view the MMS message.
 * @param[in]  handle => message handle
 * @param[in]  eViewCtx => view context  (View or Preview)
 * @param[in]  pContent =>  (Optional) Pointer to the encapsulated content.
 * @param[out] None
 * @return     JC_OK on success.
 */
JC_RETCODE jmms_ViewMms (CONTENT_DATA *pstContent, JC_BOOLEAN bViewPageOnly)
{
	JC_RETCODE				eRet = JC_ERR_INVALID_PARAMETER;
	JC_UINT8				ucPageCount = 0;
	DeviceCharacteristics	deviceCharacteristics = {0, } ;

	eRet = jdd_MMIGetDeviceCharacteristics (&deviceCharacteristics) ;
    eRet = jmms_MMISetViewCallback (g_pstJMmsAppInfo->hCurrMsg);
	if (eRet == JC_OK)
	{
		eRet = jdi_MsgView (g_pstJMmsAppInfo->hCurrMsg, pstContent);
		if (eRet == JC_OK)
		{				
			ucPageCount    = 0;
			eRet = jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucPageCount);
			if (JC_OK == eRet)
			{
					EVENT_START_VIEW stEventStartView;
					jc_memset(&stEventStartView, 0, sizeof(EVENT_START_VIEW));
					
					stEventStartView.Rect.iLeft    = deviceCharacteristics.uiWindowX ;
					stEventStartView.Rect.iTop     = 0; //deviceCharacteristics.uiWindowY; //deviceCharacteristics.uiTopMargin ;
					stEventStartView.Rect.uiHeight = deviceCharacteristics.uiRenderingHeight; 
					stEventStartView.Rect.uiWidth  = deviceCharacteristics.uiRenderingWidth ; 
					stEventStartView.uiWindowID    = g_pstJMmsAppInfo->uiViewWindow ;
					if(ucPageCount >= 1)
						stEventStartView.ucPageIndex   = 1 ;
					else
						stEventStartView.ucPageIndex   = 0 ;
					stEventStartView.eViewMode     = E_PORTRAIT_MODE;
					
					if(pstContent == NULL)
					{
						g_pstJMmsAppInfo->eViewState = E_JMMS_PRVIEW_COMPOSED_MSG;
						g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_STARTED ;
					}
					else
					{
						g_pstJMmsAppInfo->eViewState = E_JMMS_VIEW_FOLDER_MSG;					
					}
					jmms_MMIViewScreenHandler();
					if(bViewPageOnly)
					{
						stEventStartView.bViewPageOnly = E_TRUE;
						stEventStartView.bStaticView = E_TRUE;
						g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_IDLE;
					}
					else
					{
						stEventStartView.bViewPageOnly = E_FALSE;
					}
					
					eRet = jdi_MsgViewHandleEvent (g_pstJMmsAppInfo->hCurrMsg, E_VIEW_START,	
									(void *) (&stEventStartView));
			}
		}
	}
	
	if(JC_OK != eRet)
	{
		// Display View Failed Msg
	}	
	return eRet;
}

extern void jMMS_GetUserConfirmation(JC_UINT8* pcMessage,FPOINTER fpLeft,FPOINTER fpRight,FPOINTER entryFun);
void jmms_HandleReadReportConfirm(void)
{
	jMMS_GetUserConfirmation(GetString(STR_ID_SEND_READ_REPORT),jmms_MMISendReportDone, 
					jmms_MMIReadreportCancel,jmms_HandleReadReportConfirm) ;
}

void jmms_HandleExitView()
{
	JC_RETCODE rCode = JC_OK;

	if((E_JMMS_VIEW_FOLDER_MSG == g_pstJMmsAppInfo->eViewState) && \
		(g_pstJMmsAppInfo->bSendReadReport))
	{
		if (g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport == E_MSG_SETTINGS_READ_REPORT_SEND)
		{
			rCode = jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg,E_VIEW_CLOSE,JC_NULL);

			mmi_trace(1,"JDD_LOG jdi_MsgViewHandleEvent rcode is [%d], sendflag [%d], \
						RetFlag[%d]", rCode, g_pstJMmsAppInfo->bSendReadReport, \
						g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport);
		
			jMMS_SetPrimaryMMIStatus(E_FALSE);	
			jmms_MMISendReadreport();
			jmms_MMIViewDeInit();
			GoBackHistory();
		}
		else
		{	
			jmms_HandleReadReportConfirm();
		}
	}
	else
	{
		rCode = jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg,E_VIEW_CLOSE,JC_NULL);

		mmi_trace(1,"JDD_LOG jdi_MsgViewHandleEvent rcode is [%d], sendflag [%d], \
					RetFlag[%d]", rCode, g_pstJMmsAppInfo->bSendReadReport, \
					g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport);
	
		jMMS_SetPrimaryMMIStatus(E_FALSE);	

		jmms_MMIViewDeInit();
		GoBackHistory();
	}
}
void jmms_HandleReplayView()
{
	JC_UINT8 PageIndex = 1;
	JC_RETCODE rCode = JC_OK;

	//eRet = jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &totalPages) ;
    rCode = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg,&PageIndex);
	if(JC_OK != rCode)
	{
		mmi_trace(1, "JDD_LOG: jmms_HandleReplayView return error %d",rCode);
		PageIndex = 1;	
	}
	mmi_trace(1,"JDD_LOG: jmms_HandleViewPresentationEnd called %d", PageIndex);
	jmms_MMIViewMmsPlaySlide (PageIndex, E_TRUE);

	//jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg, E_VIEW_REPLAY,NULL) ;
	ChangeRightSoftkey(STR_ID_STOP,0);
	ChangeLeftSoftkey(JC_NULL,0);
	SetRightSoftkeyFunction(jmms_HandleStopView,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(NULL,KEY_EVENT_UP);
	g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_STARTED ;
}

void jmms_HandleViewPresentationEnd()
{
	JC_RETCODE rCode = JC_OK;
	JC_UINT8 PageIndex = 1,ucPageCount = 0;
	
	//eRet = jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &totalPages) ;

	//if the presentation is completed and user has not selected stop
	if( E_JMMS_PREVIEW_STARTED_LEFTKEY == g_pstJMmsAppInfo->ePreviewState)
	{
	
		rCode = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg,&PageIndex);
		if( PageIndex > 1)
		{
			PageIndex -=1 ;
			mmi_trace(1," jmms_MMIViewMmsPlaySlide called %d", PageIndex);
			jmms_MMIViewMmsPlaySlide (PageIndex, E_TRUE);
		}
		g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_STARTED;
	}
	else if(E_JMMS_PREVIEW_STARTED_RIGHTKEY == g_pstJMmsAppInfo->ePreviewState)
	{
	
		rCode = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg,&PageIndex);
		rCode = jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucPageCount);
		if( PageIndex < ucPageCount)
		{
			PageIndex +=1 ;
			mmi_trace(1," jmms_MMIViewMmsPlaySlide called %d", PageIndex);
			jmms_MMIViewMmsPlaySlide (PageIndex, E_TRUE);
		}
		g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_STARTED;
	}
	else
	{		
		if( g_pstJMmsAppInfo->ePreviewState == E_JMMS_PREVIEW_STARTED)
		{
			rCode = jmms_MMIViewMmsPlaySlide (1, E_FALSE);
			mmi_trace(1," jmms_HandleViewPresentationEnd called with 1st slide");
		}
		g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_IDLE;
		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
		SetRightSoftkeyFunction(jmms_HandleExitView,KEY_EVENT_UP);
		jmms_ViewSetPlayRSK();
	}	
}

void jmms_ViewSetPlayRSK()
{
	JC_UINT8	uiAttachListCount, uiMediaCount;
	JC_UINT8 	ucPageIndex;
	JC_RETCODE	rCode = JC_OK;
	MEDIA_INFO	*pMediaList = JC_NULL ;			

    if(g_pstJMmsAppInfo->eViewState == E_JMMS_PRVIEW_COMPOSED_MSG)
	{
		ChangeLeftSoftkey(STR_ID_MMS_PREVIEW_PLAY,0);
		SetLeftSoftkeyFunction(jmms_MMIViewStartSlidePlay,KEY_EVENT_UP);
	}
	else
	{

		rCode = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg, &ucPageIndex);

		if(JC_OK == rCode)
		{
			rCode = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, ucPageIndex, &pMediaList, \
						&uiMediaCount, E_FALSE) ;
		}

		rCode = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, 0, &pMediaList, \
						&uiAttachListCount, E_TRUE) ;

		if((uiAttachListCount + uiMediaCount) > 0 )
		{
			ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
			SetLeftSoftkeyFunction(jmms_MMIViewSlideOptionsEntryHandler,KEY_EVENT_UP);
		}
		else
		{
			ChangeLeftSoftkey(STR_ID_MMS_PREVIEW_PLAY,0);
			SetLeftSoftkeyFunction(jmms_MMIViewStartSlidePlay,KEY_EVENT_UP);
		}
	}
}
void jmms_HandleStopView()
{	
	JC_RETCODE rCode = JC_OK;
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	
	mmi_trace(1," jmms_HandleStopView called ");
	rCode = jmms_ViewMmsStop (E_FALSE);	
	g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_STOPPED ;
	SetRightSoftkeyFunction(jmms_HandleExitView,KEY_EVENT_UP);	
	
	jmms_ViewSetPlayRSK();
}

void jmms_HandlePauseView ()
{
	JC_RETCODE rCode = JC_OK;	
	mmi_trace(1,"jmms_HandlePauseView start %d",g_pstJMmsAppInfo->ePreviewState );
	mmi_trace (1,"@@@ jmms_HandlePauseView () value is [%d %d]", g_pstJMmsAppInfo->hCurrMsg, g_pstJMmsAppInfo->ePreviewState) ;
	if (JC_NULL != g_pstJMmsAppInfo->hCurrMsg &&
		E_JMMS_PREVIEW_STARTED == g_pstJMmsAppInfo->ePreviewState)
	{
		rCode = jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg,E_VIEW_PAUSE, JC_NULL);
		g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_PAUSED ;
		mmi_trace(1, "jdd_log:jmms_HandlePauseView event rCode[%d]", rCode);
	}
	mmi_trace(1,"jmms_HandlePauseView end %d",g_pstJMmsAppInfo->ePreviewState );
}

void jmms_HandleResumeView ()
{
	JC_RETCODE rCode = JC_OK;
	mmi_trace (1,"@@@ jmms_HandleResumeView () value is [%d %d]", g_pstJMmsAppInfo->hCurrMsg, \
				g_pstJMmsAppInfo->ePreviewState) ;	
	if (JC_NULL != g_pstJMmsAppInfo->hCurrMsg)
	{
		if (E_JMMS_PREVIEW_PAUSED == g_pstJMmsAppInfo->ePreviewState)
		{
			rCode = jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg,E_VIEW_RESUME, JC_NULL);
			/* E_JMMS_PREVIEW_RESUMED is currently is of no use. It is just set inside this function */
			g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_RESUMED ;
		}
		else if (E_JMMS_PREVIEW_STOPPED == g_pstJMmsAppInfo->ePreviewState)
		{
			
		}		
	}
}

void jmms_MMIViewExitScreenHandler ()
{	
	jMMS_SetPrimaryMMIStatus (E_FALSE) ;
	jmms_HandlePauseView ();
	DestroyVideoHandle();
	mdi_audio_set_file_data_progress(0);
}

void jmms_MMIViewStartSlidePlay()
{
	JC_RETCODE eRet;
	JC_UINT8 ucPageCount=0;
	eRet = jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucPageCount);
	if( ucPageCount == 0)
	{
		DisplayPopup (GetString (STR_ID_JMMS_NO_PRESENTATION_AVAILABLE), \
						IMG_GLOBAL_INFO, 1, 2000, 1) ;
	}
	else
	{		
		jmms_HandleReplayView();
		mmi_trace(1,"JDD_LOG:jmms_MMIViewStartSlidePlay>>jdi_MsgViewHandleEvent return value %d",eRet);
	}
}

void jmms_MMIViewScreenHandler()
{
	U8* guiBuffer;
	EntryNewScreen (SCR_ID_MMS_VIEW, jmms_MMIViewExitScreenHandler, jmms_MMIViewScreenHandler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_VIEW) ;
	
	ShowCategory7Screen(JC_NULL, IMG_SMS_ENTRY_SCRN_CAPTION,
							STR_ID_MMS_PREVIEW_LSK_BLANK, 0, 
							STR_ID_MMS_PREVIEW_LSK_BLANK, 0, 
							NULL, 
							guiBuffer);

	mmi_trace(1,"jmms_MMIViewScreenHandler %d",g_pstJMmsAppInfo->ePreviewState );
	//Check wheter attachment is present in mms
	
	

    // if it is view and state is E_JMMS_PREVIEW_IDLE or E_JMMS_PREVIEW_NONE
    if(g_pstJMmsAppInfo->ePreviewState == E_JMMS_PREVIEW_IDLE ||
		g_pstJMmsAppInfo->ePreviewState == E_JMMS_PREVIEW_NONE)
    {
		JC_UINT8	uiAttachListCount ;
		JC_RETCODE	rCode = JC_OK;
		MEDIA_INFO	*pMediaList = JC_NULL ;
			

		rCode = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, 0, &pMediaList, \
						&uiAttachListCount, E_TRUE) ;

		ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
		SetRightSoftkeyFunction(jmms_HandleExitView,KEY_EVENT_UP);
		jmms_ViewSetPlayRSK();
    }
	else
	{
		ChangeRightSoftkey(STR_ID_STOP,0);
		//ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
		ChangeRightSoftkey(STR_ID_STOP,0);
		ChangeLeftSoftkey(JC_NULL,0);
		SetRightSoftkeyFunction(jmms_HandleStopView,KEY_EVENT_UP);

		//SetRightSoftkeyFunction(jmms_HandleExitView,KEY_EVENT_UP);
		SetRightSoftkeyFunction(jmms_HandleStopView,KEY_EVENT_UP);
		SetLeftSoftkeyFunction(NULL,KEY_EVENT_UP);

	}
	mmi_trace (1,"jdd_log: jmms_MMIViewScreenHandler value[%d %d]", \
				g_pstJMmsAppInfo->hCurrMsg, g_pstJMmsAppInfo->ePreviewState) ;
	jMMS_ViewRegisterKeyHandler ();
	jMMS_SetPrimaryMMIStatus(E_TRUE);	
		
	if (JC_NULL != g_pstJMmsAppInfo->hCurrMsg)
	{
		jMms_SetMmsViewTitle () ;
		if (E_JMMS_PREVIEW_PAUSED == g_pstJMmsAppInfo->ePreviewState ||
			E_JMMS_PREVIEW_IDLE == g_pstJMmsAppInfo->ePreviewState   ||
			E_JMMS_PREVIEW_STARTED_LEFTKEY == g_pstJMmsAppInfo->ePreviewState   ||
			E_JMMS_PREVIEW_STARTED_RIGHTKEY == g_pstJMmsAppInfo->ePreviewState)
		{
			jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg,E_VIEW_REFRESH, JC_NULL);
			if (E_JMMS_PREVIEW_PAUSED == g_pstJMmsAppInfo->ePreviewState)
			{
				jmms_HandleResumeView () ;
				g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_STARTED ;
			}
				
		}
	}
}

JC_RETCODE jmms_IsMediaPresent()
{
	JC_UINT8	uiMediaCount , uiPageIndex, PageCount =0;
	
	JC_RETCODE 	rCode = JC_OK;
       jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &PageCount);
	mmi_trace(1, "jmms_log: IsMediaPresent page PageCount[%d]", \
				PageCount);	   
	if(PageCount == 0)
		return E_FALSE;
	rCode = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg, &uiPageIndex);

	if(JC_OK == rCode)
	rCode = jdi_MsgGetMediaCount(g_pstJMmsAppInfo->hCurrMsg, uiPageIndex, &uiMediaCount, E_FALSE);

	mmi_trace(1, "jmms_log: IsMediaPresent page index[%d] rcode[%d] mediaCount[%d]", \
				uiPageIndex, rCode, uiMediaCount);
	
	if(JC_OK == rCode && (uiMediaCount > 0))
		return E_TRUE;
	else
		return E_FALSE;
}

JC_RETCODE jmms_IsAttachmentPresent()
{
	JC_UINT8       uiAttachListCount;
    JC_RETCODE     rCode = JC_OK;
    MEDIA_INFO     *pSaveMediaList = JC_NULL ;

	rCode = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, 0, &pSaveMediaList, \
	                            &uiAttachListCount, E_TRUE) ;
	if(JC_OK == rCode && (uiAttachListCount > 0))
		return E_TRUE;
	else
		return E_FALSE;
}
void jmms_MMIViewSlideOptionsEntryHandler()
{
	U16 item_texts[E_MMS_OPTION_VIEW_ITEMS] = {0, } ;
	INT iIndex = 0 ;
	
	item_texts [iIndex++] = STR_ID_MMS_PREVIEW_PLAY ;
	if(jmms_IsMediaPresent ())
	{
		item_texts [iIndex++] = STR_ID_MMS_SAVE_MEDIA ;
	}
	if (jmms_IsAttachmentPresent())
	{
		item_texts [iIndex++] = STR_ID_MMS_VIEW_ATTACHMENT ;
	}

	EntryNewScreen (SCR_ID_MMS_VIEW_SLIDE_OPTIONS, NULL, jmms_MMIViewSlideOptionsEntryHandler, NULL) ;
	RegisterHighlightHandler (jmms_view_option_highlight_handler) ;
	ShowCategory15Screen (STR_ID_MMS_TEXT, IMG_SMS_ENTRY_SCRN_CAPTION, 
		STR_GLOBAL_OK, 0, 
		STR_GLOBAL_BACK, 0,
		iIndex, item_texts,(U16 *)gIndexIconsImageList, 
		1, 0, NULL) ;

	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP) ;
}

void jmms_GoToViewScreen()
{
	GoBackHistory();
	jmms_MMIViewStartSlidePlay();
}

void jmms_view_option_highlight_handler (int iSelectIndex) 
{	
	mmi_trace(1,"jmms_log: jmms_view_option_highlight_handler Index:[%d]", iSelectIndex);
	
	switch (iSelectIndex)
	{
		case 0:
			SetLeftSoftkeyFunction (jmms_GoToViewScreen, KEY_EVENT_UP) ;
			break ;
		case 1:
			{
				if(jmms_IsMediaPresent ())
					SetLeftSoftkeyFunction (jmms_SaveMediaOptionEntryHandler, KEY_EVENT_UP) ;
				else
					SetLeftSoftkeyFunction (jmms_SaveAttachmentOptionEntryHandler, KEY_EVENT_UP) ;
			}
			break ;
		case 2:
			SetLeftSoftkeyFunction (jmms_SaveAttachmentOptionEntryHandler, KEY_EVENT_UP) ;
			break ;	
		default:			
			break ;		
	}
}

void jmms_SaveAttachmentExitHandler()
{
	jmms_FreeAttachmentListMemory();
}
void jMms_SaveAttachmentHighlightHandler (int iSelectIndex)
{
	g_iSelectIndex = iSelectIndex ;
}

void jmms_attachmentFinameCheck(void)
{
	JC_INT32 len = 0 ;
	
	JC_CHAR * fileName = (JC_CHAR *)jMms_getInputBuff();
	len = jc_wstrlen(fileName);
	
	if(jc_tcslen(fileName) == 0)
	{
		DisplayPopup(GetString (STR_ID_MMS_FILE_NAME_EMPTY),IMG_GLOBAL_ERROR, 1, 2000, ERROR_TONE);
		DeleteUptoScrID(SCR_ID_MMS_INPUT_NAME);
		return;
	}
	
	if(fileName[0] == '.' && len == 1)
	{
		DisplayPopup(GetString (STR_ID_MMS_FILE_NAME_INVALID),IMG_GLOBAL_ERROR, 1, 2000, ERROR_TONE);
		DeleteUptoScrID(SCR_ID_MMS_INPUT_NAME);
		return;
	}
	
	while ( len>0 )
	{
		len--;
		if (fileName[len] == '/'  || fileName[len] == '\\' )
		{
			DisplayPopup((PU8)GetString(STR_ID_MMS_FILE_NAME_INVALID), IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
			DeleteUptoScrID(SCR_ID_MMS_INPUT_NAME);
			return;
		}
	}
	
		
	jmms_SaveAttachment();
}

void jmms_inputBox_highlightHanlder (S32 index)
{
	switch(index)
	{
		case 0:
			SetLeftSoftkeyFunction (jmms_attachmentFinameCheck, KEY_EVENT_UP ) ;
			break;
		case 1:
			SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP ) ;
			RegisterInputMethodScreenCloseFunction (GoBackFromInputMethod);
			break ;
	}
	
}

void jmms_inputBoxLskHandler ()
{
	U16 item_texts[] = {STR_GLOBAL_DONE, \
						STR_GLOBAL_INPUT_METHOD} ;
	
	U16 item_icons[] = {IMG_GLOBAL_L1, \
						IMG_GLOBAL_L2, \
						} ;
	
	EntryNewScreen (SCR_ID_WIDGET_INPUTBOX_OPTION, 0, jmms_inputBoxLskHandler, 0) ;

	SetParentHandler (0);
	
	RegisterHighlightHandler (jmms_inputBox_highlightHanlder);

	ShowCategory52Screen (STR_GLOBAL_OPTIONS, 0, 
		STR_GLOBAL_OK, 0,
		STR_GLOBAL_BACK, 0, 
		2, item_texts, item_icons,
		0, 0, 0, 0);
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP ) ;
}

void jmms_getAndRemoveExternName(JC_CHAR *pFileName, JC_CHAR* pExtName)
{
	JC_UINT16 nStrLen;
	JC_UINT16 i= 0;
	ASSERT(NULL != pFileName);
	ASSERT(NULL != pExtName);
	nStrLen = jc_tcslen(pFileName);
	for(i = 0; i < nStrLen; i ++)
	{
		if(*(pFileName  + nStrLen - i  - 1) == '.')
			break;
	}
	if(i<nStrLen && i <= MAX_LEN_OF_EXT_NAME)
	{
		*(pFileName + nStrLen -i - 1) = 0;

		if(jc_tcslen(pFileName) > JMMS_FILENAME_MAX -1 )
		{
			*(pFileName + JMMS_FILENAME_MAX -1) = 0;
		}
		
		jc_tcscpy(pExtName, pFileName + nStrLen -i);
		
		mmi_trace(1, "jmms_log: ext name is: 0x%x, 0x%x, 0x%x", *(pExtName),*(pExtName+1),*(pExtName+2));
	}
	
	
}

void jmms_getFilenameFromMMS(U8 *pmDestFileName)
{
	JC_UINT8            uiAttachListCount ;
	JC_RETCODE      rCode = JC_OK;
	MEDIA_INFO     *pSaveMediaList = JC_NULL ;
	JC_INT32             iListIndex = 0;
	JC_CHAR          *pTempFileName = NULL ;
	MEDIA_INFO     *pTempMediaInfo = NULL;

	ASSERT(NULL != pmDestFileName);
	rCode = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, 0, &pSaveMediaList, \
	                            &uiAttachListCount, E_TRUE) ;
       pTempMediaInfo = pSaveMediaList;
	while(pTempMediaInfo != NULL)
	{
		if(g_iSelectIndex == iListIndex)
		{
			break;
		}
		iListIndex++;
		pTempMediaInfo = pTempMediaInfo->pNext;
	}
	if(pTempMediaInfo != NULL)
	{
		//need to check file system memory avalability
		if(pTempMediaInfo->pMimeInfo->MimeHeaders.pObjectName == NULL)
		{
			jdd_MsgConfigGetFileNameWithExt (&pTempFileName, pTempMediaInfo->pMimeInfo->MimeHeaders.pContentType, 0) ;
			if(NULL != pTempFileName)
			{
				memset(gTempExtName, 0, sizeof(gTempExtName));
				jmms_getAndRemoveExternName(pTempFileName, gTempExtName);
				jc_tcscpy(pmDestFileName, pTempFileName);
				jdd_MemFree(pTempFileName);
			}	
		}
		else
		{
			pTempFileName = jdd_MemAlloc(jc_tcslen(pTempMediaInfo->pMimeInfo->MimeHeaders.pObjectName)+1, sizeof(JC_CHAR));
			ASSERT(NULL != pTempFileName);
			memset(pTempFileName, 0, (jc_tcslen(pTempMediaInfo->pMimeInfo->MimeHeaders.pObjectName)+1)*sizeof(JC_CHAR));
			jc_tcscpy(pTempFileName, pTempMediaInfo->pMimeInfo->MimeHeaders.pObjectName);
			memset(gTempExtName, 0, sizeof(gTempExtName));
			jmms_getAndRemoveExternName(pTempFileName, gTempExtName);
			jc_tcscpy(pmDestFileName, pTempFileName);
			jdd_MemFree(pTempFileName);
			pTempFileName = NULL;
		}
	}

}

void jmms_EntryFilename (void)
{
	U8* guiBuffer = JC_NULL;
	
	EntryNewScreen (SCR_ID_MMS_INPUT_NAME, JC_NULL, jmms_EntryFilename, NULL);

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_INPUT_NAME);		
	SetParentHandler (0);
	ShowCategory5Screen (STR_ID_MMS_INPUT_NAME, 0,
		STR_GLOBAL_OPTIONS, 0,
		STR_GLOBAL_BACK, 0,
		INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
		(U8*)jMms_getInputBuff(), JMMS_FILENAME_MAX, 
		guiBuffer);
	
#ifdef __MMI_GB__
	GBSetInputboxLSKFunction (jmms_inputBoxLskHandler);
#endif

	SetLeftSoftkeyFunction (jmms_inputBoxLskHandler, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (GoBackHistory );
}

void jmms_getFilenameFromUser(void)
{
#ifndef MMI_ON_WIN32
	if(MMI_CARD_DRV <= 0)
	{
		DisplayPopup((PU8)GetString(STR_ID_MMS_NO_TCARD) ,IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
		return;
	}
#endif
	memset((U8*)jMms_getInputBuff(), 0 , jMMS_MAX_INPUT_SIZE);
	jmms_getFilenameFromMMS((U8*)jMms_getInputBuff());
	jmms_EntryFilename();
}

void jmms_SaveMediaExitHandler()
{
	if(g_pMediaListItems)
	{		
		jdd_MemFree(g_pMediaListItems);
		g_pMediaListItems = JC_NULL;
	}
}

void jMms_ComposeFormMediaList (JC_UINT8	**pMediaListItems, \
										MEDIA_INFO *pMediaInfoList, JC_UINT8 uiMediaListCount)
{
	JC_INT32 	iIndex ;	
	for (iIndex = 0; iIndex < uiMediaListCount; iIndex++)
	{		
		/*Getting File name ignoring bIsStream member */
		pMediaListItems [iIndex] = (JC_UINT8*) pMediaInfoList->pMimeInfo->MimeHeaders.pObjectName;
		pMediaInfoList = pMediaInfoList->pNext ;
	}	
}

void jmms_SaveMediaOptionEntryHandler()
{	
	U8			*guiBuffer ;	
	JC_UINT8	uiMediaListCount, ucPageIndex;
	JC_RETCODE	rCode = JC_OK;
	MEDIA_INFO	*pMediaInfoList = JC_NULL ;

	rCode = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg, &ucPageIndex);

	mmi_trace(1, "jmms_log: SaveMediaOptionEntryHandler GetMediaList[%d] [%d]", rCode, ucPageIndex);
	
	rCode = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, ucPageIndex, &pMediaInfoList, \
				&uiMediaListCount, E_FALSE) ;

	mmi_trace(1, "jmms_log: SaveMediaOptionEntryHandler GetMediaList[%d]", rCode);
	EntryNewScreen (SCR_ID_MMS_SAVE_MEDIA, jmms_SaveMediaExitHandler, \
					jmms_SaveMediaOptionEntryHandler, NULL) ;
	
	RegisterHighlightHandler (jMms_SaveAttachmentHighlightHandler) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_SAVE_MEDIA) ;			

	g_uiMediaListCount = uiMediaListCount;
	mmi_trace(1, "jmms_log: SaveMediaOptionEntryHandler count[%d]", uiMediaListCount);
	g_pMediaListItems = jdd_MemAlloc(uiMediaListCount, sizeof(JC_UINT8*));
	
	jMms_ComposeFormMediaList(g_pMediaListItems, pMediaInfoList, uiMediaListCount) ;	
	
	ShowCategory84Screen(STR_ID_MMS_VIEW_MSG, IMG_SMS_ENTRY_SCRN_CAPTION,
							 STR_GLOBAL_SAVE, IMG_GLOBAL_OPTIONS,
							 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							 uiMediaListCount, (U8**)g_pMediaListItems,
							 (PU16)gIndexIconsImageList, LIST_MENU,
							 0, guiBuffer);
		
	SetLeftSoftkeyFunction (jmms_getMediaFilenameFromUser, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}


void jmms_SaveAttachmentOptionEntryHandler (void)
{	
	U8 			*guiBuffer ;	
	JC_UINT8	uiAttachListCount ;
	JC_RETCODE	rCode = JC_OK;
	MEDIA_INFO	*pMediaList = JC_NULL ;
	
	rCode = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, 0, &pMediaList, \
				&uiAttachListCount, E_TRUE) ;

	mmi_trace(1, "jmms_log: jmms_SaveAttachmentOptionEntryHandler");
	EntryNewScreen (SCR_ID_MMS_ATTACHMENT, jmms_SaveAttachmentExitHandler, \
					jmms_SaveAttachmentOptionEntryHandler, NULL) ;
	
	RegisterHighlightHandler (jMms_SaveAttachmentHighlightHandler) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_ATTACHMENT) ;
			

	g_uiAttachListCount = uiAttachListCount;
	g_pListItems = jdd_MemAlloc(uiAttachListCount, sizeof(JC_UINT8*));
	
	jMms_ComposeFormAttachmentList (g_pListItems, pMediaList, uiAttachListCount) ;	
	
	ShowCategory84Screen(STR_ID_MMS_VIEW_ATTACHMENT, IMG_SMS_ENTRY_SCRN_CAPTION,
					         STR_GLOBAL_SAVE, IMG_GLOBAL_OPTIONS,
					         STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
					         uiAttachListCount, (U8**)g_pListItems,
							 (PU16)gIndexIconsImageList, LIST_MENU,
							 0, guiBuffer);
		
	SetLeftSoftkeyFunction (jmms_getFilenameFromUser, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}

void jmms_SaveAttachment()
{              
   	JC_UINT8            uiAttachListCount ;
    JC_RETCODE      rCode = JC_OK;
    MEDIA_INFO     *pSaveMediaList = JC_NULL ;
	JC_CHAR 		*pmFolder = JC_NULL ;
    rCode = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, 0, &pSaveMediaList, \
                                                    &uiAttachListCount, E_TRUE) ;
    mmi_trace(1, "jmms_log: jmms_SaveAttachment GetMediaList rCode[%d]", rCode);
    if(rCode == JC_OK)
    {
		JC_INT32       iListIndex = 0;
		JC_CHAR        *pFilePath= JC_NULL;
		MEDIA_INFO     *pTempMediaInfo = pSaveMediaList;               

		mmi_show_in_progress(STR_GLOBAL_SAVING, IMG_GLOBAL_PROGRESS);
		
		rCode = jdi_CUtilsCharToTchar (JC_NULL, JMMS_SAVE_ATTACH_PATH, &pmFolder) ;
		if (JC_OK == rCode)
		{
			if (E_FALSE == jdd_FSIsDirExist (JC_NULL, pmFolder))
			{
				rCode = jdd_FSMakeDir (JC_NULL, pmFolder) ;
				mmi_trace(1, "jmms_log: create foler[%s] return 0x%x", rCode);
			}
		}

		if(NULL != pmFolder)
			jdd_MemFree(pmFolder);   

        rCode = jdi_CUtilsCharToTchar (JC_NULL, JMMS_SAVE_ATTACH_PATH, &pFilePath) ;

        mmi_trace(1, "jmms_log: jdi_CUtilsCharToTchar rCode[%d] item[%d] info[%X]", \
                                                        rCode, g_iSelectIndex, pTempMediaInfo);
        while(pTempMediaInfo != NULL)
        {
            if(g_iSelectIndex == iListIndex)
             {
                break;
             }
             iListIndex++;
             pTempMediaInfo = pTempMediaInfo->pNext;
        }
        if(pTempMediaInfo != NULL)
        {
             //need to check file system memory avalability
             rCode = jmms_SaveAttachmentWitetoFile (pFilePath, pTempMediaInfo);
        }
		else
		{
		     rCode = JC_ERR_GENERAL_FAILURE;
		}

		if(rCode == JC_OK)
		{
			mmi_trace(1,"save attach sucessfully");
			//display success popup
			DisplayPopup (GetString (STR_GLOBAL_SAVED),IMG_GLOBAL_ACTIVATED, 1, 2000, SUCCESS_TONE) ;
			DeleteUptoScrID(SCR_ID_MMS_ATTACHMENT);
		}
		else
		{
			mmi_trace(1,"save attach fail[%x]", -rCode);
			// display fail
			if(JC_ERR_FILE_DIR_ALREADY_EXIST == rCode)
			{
				DisplayPopup (GetString (STR_ID_MMS_FILE_ALREADY_EXIST),IMG_GLOBAL_ERROR, 1, 3000, ERROR_TONE) ;
				DeleteUptoScrID(SCR_ID_MMS_INPUT_NAME);
				HistoryReplace(SCR_ID_MMS_INPUT_NAME, SCR_ID_MMS_INPUT_NAME, jmms_EntryFilename);
			}
			else if(JC_ERR_FILE_WRITE == rCode)
			{
				DisplayPopup (GetString (STR_ID_MMS_FILE_SYSTEM_FULL),IMG_GLOBAL_ERROR, 1, 2000, ERROR_TONE) ;
				DeleteUptoScrID(SCR_ID_MMS_ATTACHMENT);
			}
			else
			{
				DisplayPopup (GetString (STR_GLOBAL_ERROR),IMG_GLOBAL_ERROR, 1, 2000, ERROR_TONE) ;
				DeleteUptoScrID(SCR_ID_MMS_ATTACHMENT);
			}
		}
 
        if(pFilePath)
        {
        	jdd_MemFree(pFilePath);
        }
        mmi_trace(1, "jmms_log: WitetoFile rCode[%x]", -rCode);
    }
}

JC_RETCODE jmms_SaveAttachmentWitetoFile (JC_CHAR *pPath, MEDIA_INFO *pMedia)
{
	JC_RETCODE      eRet = JC_OK ;
	JC_CHAR         *pFullPath = NULL ;
	JC_UINT8         *pBuffer = NULL ;
	JDD_FSHANDLE      hFileSysHandle = NULL ;
	JDD_FILE          hFile = NULL ;
	JC_CHAR          *pTempFileName = NULL ;
	JC_UINT8 acTmpSuffix[4];


	mmi_trace(1, "jmms_log: jmms_SaveAttachmentWitetoFile entry");
	if(pPath != NULL && pMedia != NULL)
	{		
		pTempFileName = (JC_CHAR *)jMms_getInputBuff();
		pFullPath = (JC_CHAR *)jdd_MemAlloc (jc_tcslen(pPath)*sizeof(JC_CHAR)  + jc_tcslen(pTempFileName)*sizeof(JC_CHAR) + sizeof(acTmpSuffix)*2 +sizeof(gTempExtName)+ 1, 1) ;

		if(pFullPath != NULL)
		{
			eRet = jdd_FSInitialize (&hFileSysHandle);
			if(eRet == JC_OK)
			{
				jc_tcscpy(pFullPath, pPath) ;				

				memset(acTmpSuffix, 0, sizeof(acTmpSuffix));
				AnsiiToUnicodeString(acTmpSuffix,"/");
				jc_tcscat(pFullPath, (JC_CHAR*)acTmpSuffix);
				jc_tcscat(pFullPath, pTempFileName);
				AnsiiToUnicodeString(acTmpSuffix, ".");
				jc_tcscat(pFullPath, (JC_CHAR*)acTmpSuffix);
				jc_tcscat(pFullPath, gTempExtName);

				if(E_TRUE == jdd_FSIsFileExist(JC_NULL, pFullPath))
				{
					eRet = JC_ERR_FILE_DIR_ALREADY_EXIST;
				}
				if(JC_OK == eRet)
				{
					mmi_trace(1,"jmms_log: stream status[%d]",pMedia->pMimeInfo->ContentData.bIsStream);
					if(pMedia->pMimeInfo->ContentData.bIsStream == E_TRUE)
					{
						JC_INT32 iReadBytes = 0 ;
						hFile = jdd_FSOpen (NULL, pMedia->pMimeInfo->ContentData.pFileName, E_OPEN_READ_MODE) ;
						if(hFile != NULL)
						{

							pBuffer = jdd_MemAlloc(pMedia->pMimeInfo->ContentData.uiTotalSize, sizeof(JC_UINT8)) ;
							if(pBuffer != NULL)
							{
								iReadBytes = jdd_FSRead(pBuffer, pMedia->pMimeInfo->ContentData.uiTotalSize, 1, hFile) ;
								if(iReadBytes == 1)
								{
									if(E_MEDIA_TEXT == pMedia->eMediaType)
									{
										char* 	pTempBuff = JC_NULL;
										int 	iBufflength = 0;

										pTempBuff = jmms_UtilsUnicodeToUtf8((const JC_CHAR*)pMedia->pMimeInfo->ContentData.pBuffer, \
																	&iBufflength);
				
										eRet = jMMS_MMIWriteToFile (hFileSysHandle, pFullPath, \
															pTempBuff, 
															iBufflength);
										jdd_MemFree(pTempBuff);
										mmi_trace(1,"jmms_log: MMIWriteToFile in UTF8 rCode [%x] size[%d]", \
												-eRet, iBufflength);
									}
									else
									{
										eRet = jMMS_MMIWriteToFile (hFileSysHandle, pFullPath, pBuffer, pMedia->pMimeInfo->ContentData.uiTotalSize);
									}
								}
								else
								{
									eRet = JC_ERR_FILE_READ ;
								}
							}
							else
							{
								eRet = JC_ERR_MEMORY_ALLOCATION ;
							}
						}
						else
						{
							eRet = JC_ERR_FILE_OPEN ;
						}
					}
					else
					{
						if(E_MEDIA_TEXT == pMedia->eMediaType)
						{
							char* 	pTempBuff = JC_NULL;
							int 	iBufflength = 0;

							pTempBuff = jmms_UtilsUnicodeToUtf8((const JC_CHAR*)pMedia->pMimeInfo->ContentData.pBuffer, \
														&iBufflength);
	
							eRet = jMMS_MMIWriteToFile (hFileSysHandle, pFullPath, \
												pTempBuff, 
												iBufflength); 
							jdd_MemFree(pTempBuff);
							mmi_trace(1,"jmms_log: MMIWriteToFile in UTF8 rCode [%x] size[%d]", \
									-eRet, iBufflength);
						}
						else
						{
							eRet = jMMS_MMIWriteToFile (hFileSysHandle, pFullPath, \
													pMedia->pMimeInfo->ContentData.pBuffer, 
													pMedia->pMimeInfo->ContentData.uiTotalSize);                                                                             
							mmi_trace(1,"jmms_log: MMIWriteToFile rCode [%x] size[%d]", \
										-eRet, pMedia->pMimeInfo->ContentData.uiTotalSize);
						}
					}
				}
			}
		}
		else
		{
			eRet = JC_ERR_MEMORY_ALLOCATION ;
		}
	}
	if(pFullPath != NULL)
	{
		jdd_MemFree(pFullPath) ;
	}
	if(pBuffer != NULL)
	{
		jdd_MemFree(pBuffer) ;
	}
	if(hFile != NULL)
	{
		jdd_FSClose (hFile) ;
	}
	if(hFileSysHandle != NULL)
	{
		jdd_FSDeinitialize(hFileSysHandle) ;
	}
	//jdd_MemFree(pTempFileName) ;
	return eRet ;
}

JC_RETCODE jMMS_MMIWriteToFile (JDD_FSHANDLE hFileSysHandle, JC_CHAR *pPath, JC_UINT8 *pBuffer, JC_UINT32 uiBufLen)
{
	JC_RETCODE eRet = JC_OK;
	JC_UINT32 uiWritesize = 0;

	if (pPath != NULL && pBuffer != NULL)
	{
		JDD_FILE hFile = NULL;

		hFile = (JDD_FILE) jdd_FSOpen (hFileSysHandle, pPath, E_CREATE_WRITE_MODE) ;
		if (hFile != NULL)
		{
			uiWritesize = jdd_FSWrite ((void*)pBuffer, sizeof(JC_UINT8), uiBufLen, hFile) ;
			jdd_FSClose (hFile);
			if(uiWritesize == -1)
			{
				jdd_FSDelete(hFileSysHandle,pPath);
				eRet = JC_ERR_FILE_WRITE;
			}
		}
		else
		{
			eRet = JC_ERR_FILE_OPEN ;
		}
	}
	else
	{
		eRet = JC_ERR_INVALID_PARAMETER;
	}
	mmi_trace(1,"jmms_log: jMMS_MMIWriteToFile exit rCode[%x]", -eRet);
	return eRet ;
}


void jmms_MMIViewDeInit()
{
     //Destory MMS Main Window
	//jmms_MMIDestoryMMSMainWindow();
	mmi_trace (1,"@@@ jmms_MMIViewDeInit () value is [%d]", g_pstJMmsAppInfo->eViewState) ;

	if(E_JMMS_VIEW_FOLDER_MSG == g_pstJMmsAppInfo->eViewState)
	{
	 	if (g_pstJMmsAppInfo->hCurrMsg)
		{
			jdi_MsgDeInit(g_pstJMmsAppInfo->hCurrMsg) ;
			g_pstJMmsAppInfo->hCurrMsg = JC_NULL ;
		}
	}
	g_pstJMmsAppInfo->eViewState = E_JMMS_VIEW_NONE;
	g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_NONE ;

}

void jmms_HandleMsgViewScrolling (U16 keyCode)
{
	EVENT_SCROLL stScrollData = {0,} ;

	stScrollData.uiScrollOffset = jMMS_GetFontHeight() + 1 ;
	switch (keyCode)
	{
		case KEY_UP_ARROW:
			{
				stScrollData.eDirection = E_DIRECTION_UP ;
				jdi_MsgViewHandleEvent (g_pstJMmsAppInfo->hCurrMsg, E_VIEW_SCROLL, 
							(void*)(&stScrollData));
			}
			break ;
		case KEY_DOWN_ARROW:
			{
				stScrollData.eDirection = E_DIRECTION_DOWN ;
				jdi_MsgViewHandleEvent (g_pstJMmsAppInfo->hCurrMsg, E_VIEW_SCROLL, 
							(void*)(&stScrollData));
			}
			break ;
		case KEY_LEFT_ARROW:
			{
				EVENT_PREV_SLIDE stEventPrevSlide = {0,};
				JC_UINT8 PageIndex = 1, ucPageCount = 0;
				JC_RETCODE rCode = JC_OK;
				stEventPrevSlide.bIsTimerInvoked = E_FALSE ;
				mmi_trace(1,(PCSTR)"left key clicked");
				rCode = jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucPageCount);

				if(ucPageCount <= 1)
				{
				  break;
				}
				
				if( g_pstJMmsAppInfo->ePreviewState == E_JMMS_PREVIEW_IDLE)
				{
					jdi_MsgViewHandleEvent (g_pstJMmsAppInfo->hCurrMsg, E_VIEW_PLAY_PREV_SLIDE, 
								(void*)(&stEventPrevSlide));
				}
				else
				{
				    rCode = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg,&PageIndex);
					if( PageIndex > 1)
					{
						rCode = jmms_ViewMmsStop (E_FALSE);	
						g_pstJMmsAppInfo->ePreviewState= E_JMMS_PREVIEW_STARTED_LEFTKEY;
					}
					else
					{
						mmi_trace(1,(PCSTR)"left key ignored");
					}
				}
			}
			break ;
		case KEY_RIGHT_ARROW:
			{
				EVENT_NEXT_SLIDE stEventNextSlide = {0,};
				JC_UINT8 PageIndex = 1, ucPageCount = 0;
				JC_RETCODE rCode = JC_OK;
				  	
				stEventNextSlide.bIsTimerInvoked = E_FALSE ;
				mmi_trace(1,(PCSTR)"right key clicked");
				rCode = jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucPageCount);

				if(ucPageCount <= 1)
				{
				  break;
				}
				
				if( g_pstJMmsAppInfo->ePreviewState == E_JMMS_PREVIEW_IDLE)
				{
					jdi_MsgViewHandleEvent (g_pstJMmsAppInfo->hCurrMsg, E_VIEW_PLAY_NEXT_SLIDE, 
								(void*)(&stEventNextSlide));
				}
				else
				{
				    rCode = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg,&PageIndex);
					
					if( PageIndex < ucPageCount)
					{
						rCode = jmms_ViewMmsStop (E_FALSE);	
						g_pstJMmsAppInfo->ePreviewState = E_JMMS_PREVIEW_STARTED_RIGHTKEY;
					}
					else
					{
						mmi_trace(1,(PCSTR)"left key ignored");
					}
				}
			}
			break ;
		default:
			{
				//Invalid event
			}
	}
	return ;
}

void jMMS_ViewUpKeyHandler()
{
	jmms_HandleMsgViewScrolling (KEY_UP_ARROW) ;
}
void jMMS_ViewDownKeyHandler()
{
	jmms_HandleMsgViewScrolling (KEY_DOWN_ARROW) ;
}
void jMMS_ViewLeftKeyHandler()
{
	jmms_HandleMsgViewScrolling (KEY_LEFT_ARROW) ;
}
void jMMS_ViewRightKeyHandler()
{
	jmms_HandleMsgViewScrolling (KEY_RIGHT_ARROW) ;
}

void jMMS_ViewRegisterKeyHandler()
{
	SetKeyHandler(jMMS_ViewUpKeyHandler, KEY_UP_ARROW, KEY_EVENT_UP) ;
	SetKeyHandler(jMMS_ViewDownKeyHandler, KEY_DOWN_ARROW, KEY_EVENT_UP) ;
	SetKeyHandler(jMMS_ViewLeftKeyHandler, KEY_LEFT_ARROW, KEY_EVENT_UP) ;
	SetKeyHandler(jMMS_ViewRightKeyHandler, KEY_RIGHT_ARROW, KEY_EVENT_UP) ;
	//SetKeyHandler(jmms_ComposeSlideEndKeyHandler, KEY_END, KEY_EVENT_DOWN) ;
#ifdef __MMI_TOUCH_SCREEN__
	mmi_wgui_register_pen_down_handler (jMMS_PenDownHandler) ;
	//mmi_wgui_register_pen_up_handler (jMMS_PenUpHandler) ;
	mmi_wgui_register_pen_move_handler (jMMS_PenMoveHandler) ;
#endif
}

JC_RETCODE jmms_ViewMmsStop (JC_BOOLEAN bAudioVideoStatus)
{
	JC_RETCODE	eRet = JC_OK ;
    EVENT_STOP StopEvent = {0};
        
    StopEvent.bStopAudioVideo = bAudioVideoStatus;
    eRet = jdi_MsgViewHandleEvent(g_pstJMmsAppInfo->hCurrMsg,E_VIEW_STOP, &StopEvent);
        
	return eRet ;
}

void jmms_MMIReportScreenEntryHandler()
{
	EntryNewScreen(SCR_ID_MMS_DELIVERY_REPORT, jmms_MMIReportScreenExitHandler, jmms_ViewMsgFromFolder, NULL);
	ShowCategory78Screen(STR_ID_MMS_INBOX,IMG_SMS_ENTRY_SCRN_CAPTION,
						0, 0,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						NULL,
						(U8*) g_pstJMmsAppInfo->pszScreeBuff,
						jc_tcslen(g_pstJMmsAppInfo->pszScreeBuff),
						NULL);
	SetLeftSoftkeyFunction (0,KEY_EVENT_UP) ;
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);	
}

void jmms_MMIReportScreenExitHandler()
{
	mmi_trace(g_sw_MMS, "JDD_LOG: ReportScreenExitHandler is called");
	if(g_pstJMmsAppInfo && g_pstJMmsAppInfo->pszScreeBuff)
	{
		jdd_MemFree(g_pstJMmsAppInfo->pszScreeBuff);
		g_pstJMmsAppInfo->pszScreeBuff = NULL;
	}
}

JC_RETCODE jmms_MMISetViewCallback (MSG_HANDLE hMsgHandle)
{
	JC_RETCODE eRet = JC_OK ;

	eRet = jdi_MsgRegisterCallbacks(hMsgHandle, E_CB_ATTACHMENT_MISC_IND, (void *)jmms_AttachmentMiscIndicatorCB);
	if(eRet != JC_OK )
	{
		return eRet ;
	}

	//eRet = jdi_MsgRegisterCallbacks(hMsgHandle, E_CB_FOCUS_CHANGED_IND, (void *)jmms_FocusChangeCB);
	if(eRet != JC_OK )
	{
		return eRet ;
	}

	eRet = jdi_MsgRegisterCallbacks(hMsgHandle, E_CB_AUDIO_VIDEO_UNKNOWN_IND, (void *)jmms_AudioVideoUnknownIndicatorCB);
	if(eRet != JC_OK )
	{
		return eRet ;
	}

	eRet = jdi_MsgRegisterCallbacks(hMsgHandle, E_CB_PAGE_PRESENTATION_IND, (void *)jmms_PagePresentationIndicatorCB);

    if(eRet != JC_OK )
	{
		return eRet ;
	}

    eRet = jdi_MsgRegisterCallbacks(hMsgHandle, E_CB_TIMER_LISTENER, (void *)jmms_TimerListenerCB );
        
	if(eRet != JC_OK )
	{
	    return eRet ;
	}

	return eRet ;
}

void jmms_SetMediaFilename(U8 *pmDestFileName)
{
	JC_UINT8            uiMediaCount , uiPageIndex;
	JC_RETCODE      rCode = JC_OK;
	MEDIA_INFO     *pSaveMediaList = JC_NULL ;
	JC_INT32             iListIndex = 0;
	JC_CHAR          *pTempFileName = NULL ;
	MEDIA_INFO     *pTempMediaInfo = NULL;

	ASSERT(NULL != pmDestFileName);

	rCode = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg, &uiPageIndex);

	if(JC_OK == rCode)
	rCode = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, uiPageIndex, &pSaveMediaList, \
	                            &uiMediaCount, E_FALSE) ;
	
    pTempMediaInfo = pSaveMediaList;
	while(pTempMediaInfo != NULL)
	{
		if(g_iSelectIndex == iListIndex)
		{
			break;
		}
		iListIndex++;
		pTempMediaInfo = pTempMediaInfo->pNext;
	}
	if(pTempMediaInfo != NULL)
	{
		//need to check file system memory avalability
		if(pTempMediaInfo->pMimeInfo->MimeHeaders.pObjectName == NULL)
		{
			jdd_MsgConfigGetFileNameWithExt (&pTempFileName, pTempMediaInfo->pMimeInfo->MimeHeaders.pContentType, 0) ;
			if(NULL != pTempFileName)
			{
				memset(gTempExtName, 0, sizeof(gTempExtName));
				jmms_getAndRemoveExternName(pTempFileName, gTempExtName);
				jc_tcscpy(pmDestFileName, pTempFileName);
				jdd_MemFree(pTempFileName);
			}	
		}
		else
		{
			pTempFileName = jdd_MemAlloc(jc_tcslen(pTempMediaInfo->pMimeInfo->MimeHeaders.pObjectName)+1, sizeof(JC_CHAR));
			ASSERT(NULL != pTempFileName);
			memset(pTempFileName, 0, (jc_tcslen(pTempMediaInfo->pMimeInfo->MimeHeaders.pObjectName)+1)*sizeof(JC_CHAR));
			jc_tcscpy(pTempFileName, pTempMediaInfo->pMimeInfo->MimeHeaders.pObjectName);
			memset(gTempExtName, 0, sizeof(gTempExtName));
			jmms_getAndRemoveExternName(pTempFileName, gTempExtName);
			jc_tcscpy(pmDestFileName, pTempFileName);
			jdd_MemFree(pTempFileName);
			pTempFileName = NULL;
		}
	}

}

void jmms_SaveUserMediaName()
{              
   	JC_UINT8       uiAttachListCount, uiPageIndex;
    JC_RETCODE     rCode = JC_OK;
    MEDIA_INFO     *pSaveMediaList = JC_NULL ;
	JC_CHAR 	   *pmFolder = JC_NULL ;

	rCode = jdi_MsgGetCurrentPageIndex(g_pstJMmsAppInfo->hCurrMsg, &uiPageIndex);

	if(JC_OK == rCode)
    rCode = jdi_MsgGetMediaList(g_pstJMmsAppInfo->hCurrMsg, uiPageIndex, &pSaveMediaList, \
                                                    &uiAttachListCount, E_FALSE) ;
    mmi_trace(1, "jmms_log: SaveUserMediaName GetMediaList rCode[%d]", rCode);
    if(rCode == JC_OK)
    {
		JC_INT32       iListIndex = 0;
		JC_CHAR        *pFilePath= JC_NULL;
		MEDIA_INFO     *pTempMediaInfo = pSaveMediaList;              

		mmi_show_in_progress(STR_GLOBAL_SAVING, IMG_GLOBAL_PROGRESS);
		
		rCode = jdi_CUtilsCharToTchar (JC_NULL, JMMS_SAVE_ATTACH_PATH, &pmFolder) ;
		if (JC_OK == rCode)
		{
			if (E_FALSE == jdd_FSIsDirExist (JC_NULL, pmFolder))
			{
				rCode = jdd_FSMakeDir (JC_NULL, pmFolder) ;
				mmi_trace(1, "jmms_log: create foler[%s] return 0x%x", rCode);
			}
		}

		if(NULL != pmFolder)
			jdd_MemFree(pmFolder);
   

        rCode = jdi_CUtilsCharToTchar (JC_NULL, JMMS_SAVE_ATTACH_PATH, &pFilePath) ;

        mmi_trace(1, "jmms_log: jdi_CUtilsCharToTchar rCode[%d] item[%d] info[%X]", \
                                                        rCode, g_iSelectIndex, pTempMediaInfo);
        while(pTempMediaInfo != NULL)
        {
            if(g_iSelectIndex == iListIndex)
             {
                break;
             }
             iListIndex++;
             pTempMediaInfo = pTempMediaInfo->pNext;
        }
        if(pTempMediaInfo != NULL)
        {
             //need to check file system memory avalability
             rCode = jmms_SaveAttachmentWitetoFile (pFilePath, pTempMediaInfo);
        }
		else
		{
		     rCode = JC_ERR_GENERAL_FAILURE;
		}

		if(rCode == JC_OK)
		{
			mmi_trace(1,"jmms_log: save Media sucessfully");
			//display success popup
			DisplayPopup (GetString (STR_GLOBAL_SAVED),IMG_GLOBAL_ACTIVATED, 1, 2000, SUCCESS_TONE) ;
			DeleteUptoScrID(SCR_ID_MMS_SAVE_MEDIA);
		}
		else
		{
			mmi_trace(12,"jmms_log: save Media fail[%x]", -rCode);
			// display fail
			if(JC_ERR_FILE_DIR_ALREADY_EXIST == rCode)
			{
				DisplayPopup (GetString (STR_ID_MMS_FILE_ALREADY_EXIST),IMG_GLOBAL_ERROR, 1, 3000, ERROR_TONE) ;
				DeleteUptoScrID(SCR_ID_MMS_MEDIAINPUT_NAME);
				HistoryReplace(SCR_ID_MMS_MEDIAINPUT_NAME, SCR_ID_MMS_MEDIAINPUT_NAME, jmms_SetFilename);
			}
			else if(JC_ERR_FILE_WRITE == rCode)
			{
				DisplayPopup (GetString (STR_ID_MMS_FILE_SYSTEM_FULL),IMG_GLOBAL_ERROR, 1, 2000, ERROR_TONE) ;
				DeleteUptoScrID(SCR_ID_MMS_SAVE_MEDIA);
			}
			else
			{
				DisplayPopup (GetString (STR_GLOBAL_ERROR),IMG_GLOBAL_ERROR, 1, 2000, ERROR_TONE) ;
				DeleteUptoScrID(SCR_ID_MMS_SAVE_MEDIA);
			}
		}
 
        if(pFilePath)
        {
        	jdd_MemFree(pFilePath);
        }
        mmi_trace(1, "jmms_log: WitetoFile rCode[%x]", -rCode);
    }
}

void jmms_MediaFileNameCheck(void)
{
	JC_INT32 len = 0 ;
	
	JC_CHAR * fileName = (JC_CHAR *)jMms_getInputBuff();
	len = jc_wstrlen(fileName);
	
	if(jc_tcslen(fileName) == 0)
	{
		DisplayPopup(GetString (STR_ID_MMS_FILE_NAME_EMPTY),IMG_GLOBAL_ERROR, 1, 2000, ERROR_TONE);
		DeleteUptoScrID(SCR_ID_MMS_MEDIAINPUT_NAME);
		return;
	}
	
	if(fileName[0] == '.' && len == 1)
	{
		DisplayPopup(GetString (STR_ID_MMS_FILE_NAME_INVALID),IMG_GLOBAL_ERROR, 1, 2000, ERROR_TONE);
		DeleteUptoScrID(SCR_ID_MMS_MEDIAINPUT_NAME);
		return;
	}
	
	while ( len>0 )
	{
		len--;
		if (fileName[len] == '/'  || fileName[len] == '\\' )
		{
			DisplayPopup((PU8)GetString(STR_ID_MMS_FILE_NAME_INVALID), IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
			DeleteUptoScrID(SCR_ID_MMS_MEDIAINPUT_NAME);
			return;
		}
	}		
	jmms_SaveUserMediaName();
}

void jmms_MediainputBoxHighlightHanlder (S32 index)
{
	switch(index)
	{
		case 0:
			SetLeftSoftkeyFunction (jmms_MediaFileNameCheck, KEY_EVENT_UP ) ;
			break;
		case 1:
			SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP ) ;
			RegisterInputMethodScreenCloseFunction (GoBackFromInputMethod);
			break ;
	}
	
}

void jmms_MediainputBoxLskHandler ()
{
	U16 item_texts[] = {STR_GLOBAL_DONE, \
						STR_GLOBAL_INPUT_METHOD} ;
	
	U16 item_icons[] = {IMG_GLOBAL_L1, \
						IMG_GLOBAL_L2, \
						} ;
	
	EntryNewScreen (SCR_ID_MMS_MEDIA_OPTION_MENU, 0, jmms_MediainputBoxLskHandler, 0) ;
	SetParentHandler (0);	
	RegisterHighlightHandler (jmms_MediainputBoxHighlightHanlder);

	ShowCategory52Screen (STR_GLOBAL_OPTIONS, 0, 
							STR_GLOBAL_OK, 0,
							STR_GLOBAL_BACK, 0, 
							2, item_texts, item_icons,
							0, 0, 0, 0);
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP ) ;
}


void jmms_SetFilename (void)
{
	U8* guiBuffer = JC_NULL;
	
	EntryNewScreen (SCR_ID_MMS_MEDIAINPUT_NAME, JC_NULL, jmms_SetFilename, NULL);

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_MEDIAINPUT_NAME);		
	SetParentHandler (0);
	ShowCategory5Screen (STR_ID_MMS_INPUT_NAME, 0,
		STR_GLOBAL_OPTIONS, 0,
		STR_GLOBAL_BACK, 0,
		INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
		(U8*)jMms_getInputBuff(), JMMS_FILENAME_MAX, 
		guiBuffer);
	
#ifdef __MMI_GB__
	GBSetInputboxLSKFunction (jmms_MediainputBoxLskHandler);
#endif
	SetLeftSoftkeyFunction (jmms_MediainputBoxLskHandler, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (GoBackHistory );
}

void jmms_getMediaFilenameFromUser()
{
#ifndef MMI_ON_WIN32
	if(MMI_CARD_DRV <= 0)
	{
		DisplayPopup((PU8)GetString(STR_ID_MMS_NO_TCARD) ,IMG_GLOBAL_UNFINISHED, 1, UI_POPUP_NOTIFYDURATION_TIME, ERROR_TONE);
		return;
	}
#endif
	memset((U8*)jMms_getInputBuff(), 0 , jMMS_MAX_INPUT_SIZE);
	jmms_SetMediaFilename((U8*)jMms_getInputBuff());
	jmms_SetFilename();
}
#endif
