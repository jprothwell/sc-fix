#define MODULE_NAME "JDD_DATACONNECTION"
#define FILE_NAME "jdd_dataconnection.c"
/***************************************************************************
 *
 * File Name jdd_dataconnection.c
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
 ***************************************************************************/
 /** 
 * @file    jdd_dataconnection.c
 * @ingroup jdd_dataconnection
 * @brief   Describes data connection related API's.
 *
 **/

#ifdef JATAAYU_SUPPORT 
/***************************************************************************
 * System Include Files
 **************************************************************************/
#include "cfw.h"
#include "mmi_features.h"
#include "adp_events.h"
#include "l4c_common_enum.h"

/***************************************************************************
 * User Include Files
 **************************************************************************/
# include <ddl.h>
# include <jcal.h>
# include <jdi_cutils.h>
# include <jdi_trfetch.h>
#ifdef __MMI_MULTI_SIM__
#include "commonscreens.h"
#include "dual_sim.h"
#endif
#include "timerevents.h"
#include <jddex_dataconnection.h>

/***************************************************************************
 * Macros
 **************************************************************************/
#ifdef	JDD_LOG_ENABLED
#define __MODULE_ID__					DATA_CONN_MODULE
#else
#define __MODULE_ID__					0
#endif

extern void Send_GPRS_Status_Ind(l4c_gprs_status_enum status) ;
/***************************************************************************
 * Global Variable declarations and definitions
 **************************************************************************/
 /**
 * @brief	This structure is used in #jdd_DataConnMakeConnection and 
 *			Same is received in #jdd_NetSocket function. Application may use
 *			this structure to maintain dataconnection details.
 *			All necessary platform specific information need to be included 
 *			in this structure.  
 */

JC_USER_INFO_CONNID *gpstConIdInfoHead ;
UINT8 nwap_uti=1 ;

static BOOL jdd_Data_Conn_Callback (COS_EVENT *pCosEvent);
extern BOOL jdd_SocketEventcb (COS_EVENT *pstCosEv);



#ifdef __MMI_MULTI_SIM__
UINT8 ATTACH_Status[MMI_SIM_NUMBER];//add by wuys 2011-03-16,  0: detached, 1:attaching, 2:attached
#else
UINT8 ATTACH_Status;//add by wuys 2011-03-16,  0: detached, 1:attaching, 2:attached
#endif
void init_attach_status(void)
{
#ifdef __MMI_MULTI_SIM__
UINT8 i;
for(i=0;i<MMI_SIM_NUMBER;i++)
    ATTACH_Status[i]=0x00;//add by wuys 2011-03-16, 0: detached, 1:attaching, 2:attached
#else
ATTACH_Status = 0x00;//add by wuys 2011-03-16, 0: detached, 1:attaching, 2:attached
#endif
}
/***************************************************************************
 * Local Function Prototypes
 **************************************************************************/

/***************************************************************************
 * All Local Function Definitions
 **************************************************************************/
JC_USER_INFO_CONNID* wapmmsAddToChildList(CFW_SIM_ID nSimID , UINT8  nCid, 
 		UINT16 nUTI, JC_INT8 *ApnAddr, JC_INT8 * acUserName,JC_INT8 * acPassword,E_DATACONN_STATE eConState )
{
	 JC_USER_INFO_CONNID* pstNewNode = NULL;
	 JC_USER_INFO_CONNID* pstTemp = NULL ;

	 pstNewNode = (JC_USER_INFO_CONNID*)jdd_MemAlloc (1, sizeof(JC_USER_INFO_CONNID)) ;
        memset(pstNewNode,0,sizeof(JC_USER_INFO_CONNID));
 	 pstNewNode->nSimID = nSimID;
	 pstNewNode->nCid = nCid;
	 pstNewNode->nUTI = nUTI;
	 pstNewNode->ApnAddr = jdi_CUtilsStrDuplicate(NULL, ApnAddr);
	 strcpy(pstNewNode->acUserName,acUserName);
	 strcpy(pstNewNode->acPassword,acPassword);
	 pstNewNode->eConState = eConState;
	 pstNewNode->pstNext = NULL;
	 pstNewNode->bIsCancelRequested = E_FALSE ;
	 
	 if(gpstConIdInfoHead == NULL)
	 {
		 gpstConIdInfoHead = pstNewNode;
	 }
	 else
	 {
		 pstTemp = gpstConIdInfoHead;
		 while(pstTemp->pstNext != NULL)
			 pstTemp = pstTemp->pstNext;
 	 
		 pstTemp->pstNext = pstNewNode;  
	 }
	 mmi_trace(1, "JDD_LOG: node[%d] added with simid[%d], nCid[%d], \
	 				nUTI[%d],ApnAddr[%s],eConState[%d]  ", \
	 				pstNewNode, nSimID , nCid, nUTI, ApnAddr, eConState);	
	 return pstNewNode;
 
}


JC_USER_INFO_CONNID*  GetNodeWithConID(UINT8  nCid,CFW_SIM_ID nSimID)
{
	JC_USER_INFO_CONNID* pstTempNode, * pstResNode = NULL;

	mmi_trace(1, "JDD_LOG: GetNodeWithConID with %d, sim id %d", nCid, nSimID);

	pstTempNode = gpstConIdInfoHead ;

	while ( pstTempNode )
	{
		if(pstTempNode->nCid == nCid && pstTempNode->nSimID == nSimID)
		{
			pstResNode = pstTempNode;
			break;
		}
		pstTempNode = pstTempNode->pstNext ;
	}
	mmi_trace(1, "JDD_LOG: GetNodeWithConID returns  %d", pstResNode);
	return pstResNode;
}

JC_USER_INFO_CONNID*  GetNodeWithSimID(CFW_SIM_ID nSimID)
{
	JC_USER_INFO_CONNID* pstTempNode, * pstResNode = NULL;

	mmi_trace(1, "JDD_LOG: GetNodeWith SIMID with %d", nSimID);

	pstTempNode = gpstConIdInfoHead ;

	while ( pstTempNode )
	{
		if(pstTempNode->nSimID == nSimID)
		{
			pstResNode = pstTempNode;
			break;
		}
		pstTempNode = pstTempNode->pstNext ;
	}
	mmi_trace(1, "JDD_LOG: GetNodeWithSimID returns  %d", pstResNode);
	return pstResNode;
}

void DeleteChild(JC_USER_INFO_CONNID *pstNode)
{
	JC_USER_INFO_CONNID *pstPrevNode = NULL, *pstTempNode ;  
	pstTempNode = gpstConIdInfoHead;  

	mmi_trace(1, "JDD_LOG: DeleteChild for node [%d]with UTI, CID, SIMID as %d %d %d",pstNode, pstNode->nUTI, pstNode->nCid, pstNode->nSimID);
	while ( pstTempNode != NULL )  
	{  
		if ( pstTempNode == pstNode)  
		{  
		#ifdef MMI_ON_HARDWARE_P
			#ifdef __ADP_MULTI_SIM__
			CFW_ReleaseCID(pstTempNode->nCid,pstTempNode->nSimID);
			#else
			CFW_ReleaseCID(pstTempNode->nCid);
			#endif
		#endif
			if ( pstTempNode == gpstConIdInfoHead )  
			{  
				gpstConIdInfoHead = pstTempNode->pstNext ; 
				jdd_MemFree(pstTempNode->ApnAddr);
				jdd_MemFree ( pstTempNode ) ;  
				mmi_trace(1, "JDD_LOG: DeleteChild node deleted", pstTempNode);
				return ;  
			}  
			else  
			{  
				pstPrevNode->pstNext = pstTempNode->pstNext ; 
				jdd_MemFree(pstTempNode->ApnAddr);
				jdd_MemFree ( pstTempNode ) ; 
				mmi_trace(1, "JDD_LOG: DeleteChild node deleted", pstTempNode);
				return ;  
			}  
		}  
		else   
		{  
			pstPrevNode = pstTempNode ;  
			pstTempNode = pstTempNode->pstNext ;  
		}  
	}
}
JC_BOOLEAN IsValidDataConnNode(JC_USER_INFO_CONNID *pstNode)
{
	JC_USER_INFO_CONNID *pstTempNode ;  
	JC_BOOLEAN bIsFound = E_FALSE;
	pstTempNode = gpstConIdInfoHead;  

	mmi_trace(1, "JDD_LOG: IsValidDataConnNode %d", pstNode);
	while ( pstTempNode != NULL )  
	{  
		if ( pstTempNode == pstNode)  
		{  
		 	bIsFound = E_TRUE;
			break;
		}  
		pstTempNode = pstTempNode->pstNext ;  
		 
	}
	return bIsFound;
}
#ifdef __MMI_MULTI_SIM__
CFW_SIM_ID wapmmsgetsimid(UINT32 uiConnID )
{
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER;i++)
	{
		if(uiConnID & (0x01<<i))
			return MMI_SIM_1+ i;
	}
	return MMI_SIM_1;
}
#endif
static JC_RETCODE StartGPRS(JC_USER_INFO_CONNID * pstConIdInfo)
{
	JC_RETCODE rCode = JC_OK ;
	CFW_GPRS_QOS stQos = {0,};
	CFW_GPRS_PDPCONT_INFO stPdp_cont = {0,};
	UINT32 rVal = 0 ;
	UINT8 State =0 ;

	mmi_trace (1,"JDD_LOG:  StartGPRS cid[%d], constate[%d], simid[%d], uti[%d], apn [%s]", pstConIdInfo->nCid, \
		pstConIdInfo->eConState, pstConIdInfo->nSimID, pstConIdInfo->nUTI,pstConIdInfo->ApnAddr );

#ifdef __ADP_MULTI_SIM__
	rVal = CFW_GetFreeCID(&pstConIdInfo->nCid, pstConIdInfo->nSimID);
#else
	rVal = CFW_GetFreeCID(&pstConIdInfo->nCid);
#endif 
	mmi_trace (1,"JDD_LOG: connection id given by platform[%d], rVal=0x%x", pstConIdInfo->nCid,rVal);

#ifdef __ADP_MULTI_SIM__
	CFW_GetGprsActState( pstConIdInfo->nCid, &State,pstConIdInfo->nSimID) ;
#else
	CFW_GetGprsActState( pstConIdInfo->nCid, &State) ;
#endif
	if(1 != State)
	{
		
		stQos.nDelay = 4;
		stQos.nMean = 16;
		stQos.nPeak = 4;
		stQos.nPrecedence = 3;
		stQos.nReliability = 3;
		
		
#ifdef __ADP_MULTI_SIM__
		CFW_GprsSetReqQos(pstConIdInfo->nCid, &stQos,pstConIdInfo->nSimID);
#else
		CFW_GprsSetReqQos(pstConIdInfo->nCid, &stQos);
#endif
		stPdp_cont.nApnSize = jc_strlen(pstConIdInfo->ApnAddr) ;
		stPdp_cont.pApn = pstConIdInfo->ApnAddr ;
		mmi_trace(1,"JDD_LOG:APN val [%s], size [%d]", stPdp_cont.pApn, stPdp_cont.nApnSize);

		stPdp_cont.nPdpAddrSize = 0;
		stPdp_cont.pPdpAddr = NULL ;
		stPdp_cont.nDComp = 0;
		stPdp_cont.nHComp = 0;
		stPdp_cont.nPdpType = CFW_GPRS_PDP_TYPE_IP;
		stPdp_cont.nApnUserSize = jc_strlen(pstConIdInfo->acUserName);
		stPdp_cont.pApnUser = pstConIdInfo->acUserName;
		stPdp_cont.nApnPwdSize = jc_strlen(pstConIdInfo->acPassword);
		stPdp_cont.pApnPwd = pstConIdInfo->acPassword;

		mmi_trace(1,"JDD_LOG:stPdp_cont.pApnUser is %s,stPdp_cont.pApnPwd is %s", stPdp_cont.pApnUser,stPdp_cont.pApnPwd);
#ifdef __ADP_MULTI_SIM__
		rVal = CFW_GprsSetPdpCxt(pstConIdInfo->nCid, &stPdp_cont,pstConIdInfo->nSimID);
#else
		rVal = CFW_GprsSetPdpCxt(pstConIdInfo->nCid, &stPdp_cont);
#endif
		if(ERR_SUCCESS != rVal)
		{
			mmi_trace (1, "JDD_LOG: jdd_DataConnection StartGPRS: Error CFW_GprsSet [0x%x]", rVal) ;
			rCode = JC_ERR_DATA_CONN_UNKNOWN;
		}
		else
		{	

				//incase attach response is success activate the pdp
#ifdef __ADP_MULTI_SIM__
				rVal = CFW_GprsActEx( CFW_GPRS_ACTIVED, pstConIdInfo->nCid, 
									pstConIdInfo->nUTI,pstConIdInfo->nSimID,TRUE);

//				rVal = CFW_GprsAct( CFW_GPRS_ACTIVED, pstConIdInfo->nCid, 
//									pstConIdInfo->nUTI,pstConIdInfo->nSimID);
   CFW_SetDataConnFunEx (jdd_Data_Conn_Callback,(UINT8)pstConIdInfo->nSimID,pstConIdInfo->nCid);
#else
			rVal = CFW_GprsActEx( CFW_GPRS_ACTIVED, pstConIdInfo->nCid, pstConIdInfo->nUTI,TRUE);

	//			rVal = CFW_GprsAct( CFW_GPRS_ACTIVED, pstConIdInfo->nCid, pstConIdInfo->nUTI);
                         CFW_SetDataConnFunEx (jdd_Data_Conn_Callback,pstConIdInfo->nCid);

#endif

				if(ERR_SUCCESS == rVal)
				{
					mmi_trace (1, "JDD_LOG:  StartGPRS :Success GPRS ACTIVATE rVal [0x%x]", rVal) ;
				}
				else
				{
					mmi_trace (1, "JDD_LOG:  StartGPRS :Error GPRS ACTIVATE rVal [0x%x]", rVal) ;
					rCode = JC_ERR_DATA_CONN_UNKNOWN ;
				}
		}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: Data Connection wrong state should not occur State[%d]",State) ;
	}
	
	mmi_trace (1, "JDD_LOG: StartGPRS retruns:[%d]", rCode) ;		
	return rCode ;
}


/**
 * @ingroup		jdd_dataconnection 
 * @brief 		This function is used to send DataConnection Events to 
 *				transport.
 * @param[in] 	iConnID Connection ID which is responsible for  
 *				the event to be sent
 * @param[in] 	rPlatRetVal JCF Return Value corresponding to platform
 *				error, if any. 
 * @param[in] 	eStatus Data Connection Status reported by platform
 * @retval		JC_OK on success
 *
 * When application receives a callback or an event, related to Data
 * Connection, this function can be used to send corresponding event to
 * transport task.
 */
static JC_RETCODE sendDCEventToTransport (JC_UINT32 uiConnID, \
JC_RETCODE rPlatRetVal, \
E_DATA_CONN_STATUS eStatus)

{
	JC_RETCODE rCode ;
	ST_TR_DC_EVENT *pstDCEvent ;
	JC_EVENT_DATA stEventData ;
	
	stEventData.uiEventInfoSize = sizeof(ST_TR_DC_EVENT) ;
	rCode = jdd_QueueAlloc (&stEventData) ;
	
	if ((JC_NULL != stEventData.pEventInfo) && (JC_OK == rCode))
	{
		stEventData.destTaskInfo.iAppID = (JC_INT32) E_TASK_TRANSPORT ;
		stEventData.destTaskInfo.iTaskID = (JC_INT32) E_TASK_TRANSPORT ;
		stEventData.srcTaskInfo.iAppID = (JC_INT32) E_TASK_TRANSPORT ;
		stEventData.srcTaskInfo.iTaskID = (JC_INT32) E_TASK_TRANSPORT ;
		stEventData.iEventType = (JC_INT32) E_TR_DC_EVENT ;
		/*jc_memcpy (stEventData.pEventInfo, &stDCEvent, \
		sizeof (ST_TR_DC_EVENT)) ;*/
		
		pstDCEvent = stEventData.pEventInfo ;
		pstDCEvent->rCode = rPlatRetVal ;
		pstDCEvent->eStatus = eStatus ;
		pstDCEvent->stDCTaskInfo.iAppID = (JC_INT32) E_TASK_TRANSPORT ;
		pstDCEvent->stDCTaskInfo.iTaskID = (JC_INT32) E_TASK_TRANSPORT ;
		pstDCEvent->uiConnectionID = uiConnID ;
		
		rCode = jdd_QueueSend (&stEventData) ;
	}
	else
	{
		rCode = JC_ERR_QUEUE_ALLOC ;
		mmi_trace(1,"JDD_LOG: sendDCEventToTransport: Returns [%d]", rCode) ;
	}
	
	return (rCode) ;
}

#ifdef __MMI_MULTI_SIM__
U8 gprs_attach_timer_index[MMI_SIM_NUMBER] = {0,}; // note: this index is simid+1, 0 means there is no timer start

void gprs_attach_timer_add_one(U8 nsimID)
{
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(gprs_attach_timer_index[i] == 0)
			break;
	}
	gprs_attach_timer_index[i] = nsimID + 1;
}
void gprs_attach_tiemr_remove_one(U8 nsimID)
{
	U8 i,j;
	memcpy(gprs_attach_timer_index, gprs_attach_timer_index + 1, MMI_SIM_NUMBER - 1);
	gprs_attach_timer_index[MMI_SIM_NUMBER - 1] = 0;
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(gprs_attach_timer_index[i] == nsimID + 1)
		{
			break;
		}
	}
	
	for(j = i; j < MMI_SIM_NUMBER - 1; j++)
	{
		gprs_attach_timer_index[j] = gprs_attach_timer_index[j + 1];
	}
	
	gprs_attach_timer_index[MMI_SIM_NUMBER - 1] = 0;
}

U8 gprs_attach_timer_get_simID(void)
{
	U8 nSimID;
	nSimID = gprs_attach_timer_index[0] - 1;
	gprs_attach_tiemr_remove_one(nSimID);
	return nSimID;
}

#endif

void gprs_attach_timer_callback()
{
		JC_USER_INFO_CONNID* pstConnInfo;
		U8 nSimID = 0;
		#ifdef __MMI_MULTI_SIM__
		nSimID = gprs_attach_timer_get_simID();
		#endif
		pstConnInfo = GetNodeWithSimID(nSimID);
		if (pstConnInfo != NULL)
		{
			sendDCEventToTransport ((JC_UINT32)pstConnInfo,0, E_DATA_CONN_FAIL_GPRS) ;
			DeleteChild(pstConnInfo);	
			mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: SIM0 attach failed time out") ;
		}
		else
		{
			mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: SIM0 ERROR NULL POINTER") ;
		}
}

void gprs_attach_start_timer(
#ifdef __MMI_MULTI_SIM__
				MMI_SIM_ID_T nSimID
#endif /* __MMI_MULTI_SIM__ */
				)
{
	U32 delay_time = 2*60*1000; // two minutes
#ifdef __MMI_MULTI_SIM__
	gprs_attach_timer_add_one(nSimID);
	StartTimer(GPRS_ATTACH_DELAY_TIMER + nSimID, delay_time, gprs_attach_timer_callback);
#else
	StartTimer(GPRS_ATTACH_DELAY_TIMER, delay_time, gprs_attach_timer_callback);
#endif
}

void gprs_attach_stop_timer(
#ifdef __MMI_MULTI_SIM__
		MMI_SIM_ID_T nSimID_attach
#endif /* __MMI_MULTI_SIM__ */
)
{
#ifdef __MMI_MULTI_SIM__
	mmi_trace(1, "JDD_LOG: Func: %s nSimID_attach=%d", __FUNCTION__, nSimID_attach);
	StopTimer(GPRS_ATTACH_DELAY_TIMER + nSimID_attach);
	gprs_attach_tiemr_remove_one(nSimID_attach);
#else
	StopTimer(GPRS_ATTACH_DELAY_TIMER);
#endif
}



static BOOL jdd_Data_Conn_Callback (COS_EVENT *pCosEvent)
{
	UINT8				nType = HIUINT8 (pCosEvent->nParam3) ;
	UINT8				ConnectionId = 0 ;
	JC_USER_INFO_CONNID *pstConnInfo ;
	JC_RETCODE			rCode = JC_OK ;
	UINT16				nUTI ;
	BOOL				bIsProcessed = TRUE;
	CFW_SIM_ID			nSimID  = 0;
	E_DATA_CONN_STATUS	eDataConnStatus = E_DATA_CONN_REGISTERING ;
#ifdef __MMI_MULTI_SIM__
	nSimID = LOUINT8( pCosEvent->nParam3 );
#endif
	nUTI = HIUINT16(pCosEvent->nParam3);

	mmi_trace (1,"JDD_LOG: jdd_Data_Conn_Callback nEventId [%d], nType [0x%x], simID [%d], uti [%d]",
		pCosEvent->nEventId, nType, nSimID, nUTI) ;

	switch (pCosEvent->nEventId)
    {
	case EV_CFW_GPRS_ATT_RSP: /* Attach Response */
		{
					#ifdef __MMI_MULTI_SIM__
ATTACH_Status[nSimID]=0x02;//add by wuys 2011-03-16, 0: detached, 1:attaching, 2:attached
#else
ATTACH_Status =0x02;//add by wuys 2011-03-16, 0: detached, 1:attaching, 2:attached
#endif

#ifdef __MMI_MULTI_SIM__
			gprs_attach_stop_timer(nSimID);
#else  /* No Defined __MMI_MULTI_SIM__ */
			gprs_attach_stop_timer();
#endif /* __MMI_MULTI_SIM__ */

			switch (nType)
			{
			case 0x00: /* Detach Success */
				{
					mmi_trace (1,"JDD_LOG: jdd_Data_Conn_Callback: Detach Success");
					Send_GPRS_Status_Ind (L4C_GPRS_DETACHED) ;
					eDataConnStatus = E_DATA_CONN_DISCONNECTED ;
					break ;
				}
			case 0x01: /* Attach Success */
				{
					mmi_trace (1,"JDD_LOG: jdd_Data_Conn_Callback: Attach Success") ;
					eDataConnStatus = E_DATA_CONN_CONNECTED ;
					break ;
				}
			case 0xF0: /* Detach Fail */
				{
					mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: Detach Failed") ;
					eDataConnStatus = E_DATA_CONN_FAIL_GPRS ;					
					break ;
				}
			case 0xF1: /* Attach Fail */
				{
					eDataConnStatus = E_DATA_CONN_FAIL_GPRS ;
					mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: Attach Failed") ;
					break ;
				}
			default:
				mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: EV_CFW_GPRS_ATT_RSP : Default Case") ;
				bIsProcessed = FALSE ;
				break;
			}		
		}
		break;		
	case EV_CFW_GPRS_ACT_RSP: /* Activation Response */
		{
			switch (nType)
			{
			case 0x00: /* Deactive Success */
				{
					mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: Dectivation Success") ;
					ConnectionId  = (pCosEvent->nParam1) ;
					eDataConnStatus = E_DATA_CONN_DISCONNECTED_GPRS ;
					break ;
				}
			case 0x01: /* Active Success */
				{
					mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: Activation Success[%d]") ;
					ConnectionId  = (pCosEvent->nParam1) ;
					eDataConnStatus = E_DATA_CONN_CONNECTED_GPRS ;
					break ;
				}
			case 0xF0: /* Deactive Fail */
				{
					mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: Deactivation Failed") ;
					ConnectionId  = (pCosEvent->nParam1) ;
					eDataConnStatus = E_DATA_CONN_FAIL_GPRS ;
					break ;
				}
			case 0xF1: /* Active Fail */
				{
					mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: Activation Failed") ;
					ConnectionId  = (pCosEvent->nParam1) ;
					eDataConnStatus = E_DATA_CONN_FAIL_GPRS ;
					break ;
				}
			default:
				{
					mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback, EV_CFW_GPRS_ACT_RSP : Default Case") ;
					ConnectionId  = (pCosEvent->nParam1) ;
					bIsProcessed = FALSE ;
					break ;
				}
			}
		}
		break ;		
	case EV_CFW_GPRS_CXT_DEACTIVE_IND: /* Deactivation Indication */
		{
			ConnectionId  = (pCosEvent->nParam1) ;
			eDataConnStatus = E_DATA_CONN_DISCONNECTED_GPRS ;
			mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback, EV_CFW_GPRS_CXT_DEACTIVE_IND") ;
		}
		break ;
	default:
		{
        	bIsProcessed = FALSE ;
			mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback : Default Not To be Processed Value \n");
			break ;
		}
    }

	if (TRUE == bIsProcessed)
	{
		pstConnInfo = GetNodeWithConID (ConnectionId, nSimID) ;
		
		if (JC_NULL != pstConnInfo)
		{
			mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: Connection ID [%d], SIM ID [%d], Connection Node [%d], eDataConnStatus [%d], State [%d] and bIsCancelRequested [%d]",
				ConnectionId, nSimID, pstConnInfo, eDataConnStatus, pstConnInfo->eConState, pstConnInfo->bIsCancelRequested) ;
			if (E_DATA_CONN_CONNECTED == eDataConnStatus)
			{
				if (E_DATACONN_ATTACHING == pstConnInfo->eConState)
				{
					if(E_FALSE == pstConnInfo->bIsCancelRequested)
					{
				   		pstConnInfo->eConState = E_DATACONN_ACTIVATING ;
						nwap_uti = pstConnInfo->nUTI = FREE_UTI ;
						rCode = StartGPRS (pstConnInfo) ;
						if (rCode != JC_OK)
						{
							sendDCEventToTransport ((JC_UINT32)pstConnInfo, 0, E_DATA_CONN_FAIL_GPRS) ;
							DeleteChild (pstConnInfo) ;
						}
						Send_GPRS_Status_Ind (L4C_GPRS_ATTACHED) ;
						mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: Starting Activation with rCode [%X] and UTI [%d]",
							rCode, pstConnInfo->nUTI) ;
					}
					else
					{
						mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: DANGER: Attach Response Came, but jdd_DataConnCancelConnection () has already been called.") ;
					}
				}
				else
				{
					mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: DANGER: Attach Response Came, but Connection State is Different [%d]",
						pstConnInfo->eConState) ;
				}
			}
			else if (E_DATA_CONN_CONNECTED_GPRS == eDataConnStatus)
			{
				if(E_FALSE == pstConnInfo->bIsCancelRequested)
				{
					pstConnInfo->eConState = E_DATACONN_ACTIVATED ;
					Send_GPRS_Status_Ind (L4C_PDP_ACTIVED) ;
					sendDCEventToTransport ((JC_UINT32)pstConnInfo,0, E_DATA_CONN_CONNECTED_GPRS) ;
				}
				else
				{
					nwap_uti = pstConnInfo->nUTI = FREE_UTI ;
#ifdef __ADP_MULTI_SIM__
					rCode = CFW_GprsActEx (CFW_GPRS_DEACTIVED, pstConnInfo->nCid, pstConnInfo->nUTI, pstConnInfo->nSimID,TRUE) ;

//					rCode = CFW_GprsAct (CFW_GPRS_DEACTIVED, pstConnInfo->nCid, pstConnInfo->nUTI, pstConnInfo->nSimID) ;
#else
					rCode = CFW_GprsActEx (CFW_GPRS_DEACTIVED, pstConnInfo->nCid, pstConnInfo->nUTI,TRUE) ;

//					rCode = CFW_GprsAct (CFW_GPRS_DEACTIVED, pstConnInfo->nCid, pstConnInfo->nUTI) ;
#endif
					mmi_trace (1,"JDD_LOG: jdd_Data_Conn_Callback: Starting De-activation with rCode [%X] and UTI [%d]",
									 rCode, pstConnInfo->nUTI) ;
				}
			}
			if (E_DATA_CONN_DISCONNECTED_GPRS == eDataConnStatus ||
					E_DATA_CONN_FAIL_GPRS == eDataConnStatus)
			{
				sendDCEventToTransport ((JC_UINT32)pstConnInfo,0, eDataConnStatus) ;
				DeleteChild (pstConnInfo) ;
				if (GetNodeWithSimID(nSimID)== NULL)
				{                    
					Send_GPRS_Status_Ind(L4C_PDP_DEACTIVED);
				}
			}
		}
		else
		{
			/*	If jdd_DataConnCancelConnection () is called with the state as E_DATACONN_ATTACHING,
				then this is valid case. Because the node is deleted inside jdd_DataConnCancelConnection ().*/
			mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: DANGER: Event To Be Handled But, Invalid Connection") ;
		}
	}
    return bIsProcessed ;
}

/***************************************************************************
 * All Global Function Definitions
 **************************************************************************/

/**
 * @ingroup		jdd_dataconnection
 * @brief      	Initializes the data connection module
 * @param[in]  	pvArg The argument that needs to be passed to the callback
 *					functions
 * @param[out] 	pvHandle Pointer to a location that receives a data 
 *					connection handle that is passed to the other data 
 *					connection APIs
 * @retval			JC_OK on success
 * @retval			JC_ERR_DATA_CONN_UNKNOWN on error
 */
JC_RETCODE jdd_DataConnInitialize (JDD_DATA_CONN_HANDLE	*pvHandle, void *pvArg)
{
	JC_RETCODE      rCode = JC_OK ;
#if 0 /*move to jdd_DataConnMakeConnection for multi gprs applications*/
	if (ERR_SUCCESS == CFW_SetDataConnFun (jdd_Data_Conn_Callback))
	{
		rCode = JC_OK ;
	}
#endif 
	mmi_trace (1,"JDD_LOG: jdd_DataConnInitialize : Exit [%X]", -rCode ) ;
	return rCode ;
} 
 

/**
 * @ingroup	jdd_dataconnection
 * @brief		Deinitializes the data connection module
 * @param[in]	vHandle The handle to the data connection
 * @retval    	JC_OK on success
 * @retval		JC_ERR_DATA_CONN_UNKNOWN on error
 */
JC_RETCODE jdd_DataConnDeInitialize (JDD_DATA_CONN_HANDLE vHandle) 
{
	JC_RETCODE      rCode ;
	/*-----------------------------------*/
#ifdef __MMI_MULTI_SIM__
//	CFW_SIM_ID nSimID= 0;
#endif
	mmi_trace (1,"JDD_LOG: jdd_DataConnDeInitialize : Entry") ;
	
	/*--- Platform Specific Implementation Start ---*/
#ifdef __ADP_MULTI_SIM__
     nwap_uti = FREE_UTI ;
	if (ERR_SUCCESS == CFW_GprsAtt( CFW_GPRS_DETACHED, nwap_uti,0))
#else
	if (ERR_SUCCESS == CFW_GprsAtt( CFW_GPRS_DETACHED, nwap_uti))
#endif
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_INVALID_PARAMETER ;
	}
	mmi_trace (1,"JDD_LOG: jdd_DataConnDeInitialize : Exit[%X]", -rCode) ;
	return rCode ;
}

/**
 * @ingroup		jdd_dataconnection
 * @brief		This function sets data connection progress callback.
 * @param[in]	vDCHandle Specifies the data connection handle, which is 
 *				returned by #jdd_DataConnInitialize.
 * @param[in]	cbProgressFnc Specifies the long function pointer which 
 *				is used by the data connection manager for updating the 
 * 				connection progress.
 * @retval 		JC_OK on sucesss.
 * Data Connection progress events can be sent to transport in two ways: 
 * as an event or using callback function. 
 * 
 * If events are used, jdd_DataConnSetProgressCB function can return JC_OK 
 * without any implementation. If DC callback for platform is defined in 
 * this file, sendDCEventToTransport () can be used to send DC events
 * to transport whenever there is a DC event from platform. 
 * 
 * If callback has to be used, cbProgressFnc needs to be registered/maintained
 * by application and cbProgressFnc needs to be invoked whenever there is a DC
 * event from platform. 
 **/	
JC_RETCODE jdd_DataConnSetProgressCB (JDD_DATA_CONN_HANDLE	vDCHandle, \
							JC_DATA_CONN_PROGRESS_CALLBACK	cbProgressFnc)
{
	JC_RETCODE rCode = JC_ERR_NOT_IMPLEMENTED ;
	mmi_trace (1,"JDD_LOG: jdd_DataConnSetProgressCB : Exit[%X]", rCode) ;
	return rCode ;
}

/**
 * @ingroup		jdd_dataconnection
 * @brief		This function sets data connection status callback.
 * @param[in]	vDCHandle Specifies the data connection handle, which is 
 *				returned by #jdd_DataConnInitialize.
 * @param[in]	cbStatusFnc Specifies the long function pointer, which is
 *				used by the data connection manager when the data connection
 * 				could not be established.
 * @retval		JC_OK on success.
 * Data Connection status events can be sent to transport in two ways: 
 * as an event or using callback function. 
 * 
 * If events are used, jdd_DataConnSetStatusCB function can return JC_OK 
 * without any implementation. If DC callback for platform is defined in 
 * this file, sendDCEventToTransport () can be used to send DC events
 * to transport whenever there is a DC event from platform. 
 * 
 * If callback has to be used, cbStatusFnc needs to be registered/maintained
 * by application and cbStatusFnc needs to be invoked whenever there is a DC
 * event from platform. 
 **/
JC_RETCODE jdd_DataConnSetStatusCB (JDD_DATA_CONN_HANDLE	vDCHandle, \
								JC_DATA_CONN_STATUS_CALLBACK	cbStatusFnc) 
{
	JC_RETCODE rCode = JC_ERR_NOT_IMPLEMENTED ;
	mmi_trace (1,"JDD_LOG: jdd_DataConnSetStatusCB : Exit[%X]", rCode) ;
	return rCode ;
}

/**
 * @ingroup		jdd_dataconnection
 * @brief		This function sets data connection disconnect callback.
 * @param[in]	vDCHandle Specifies the data connection handle, which is 
 *				returned by #jdd_DataConnInitialize.
 * @param[in]	cbDisconnectFnc Specifies the long function pointer, which
 *				is used by the data connection manager for updating when the
 *				specific connection is dropped.
 * @retval		JC_OK on success.	
 * Data Connection disconnect events can be sent to transport in two ways: 
 * as an event or using callback function. 
 * 
 * If events are used, jdd_DataConnSetDisconnectCB function can return JC_OK 
 * without any implementation. If DC callback for platform is defined in 
 * this file, sendDCEventToTransport () can be used to send DC events
 * to transport whenever there is a DC event from platform. 
 * 
 * If callback has to be used, cbDisconnectFnc needs to be 
 * registered/maintained by application and cbDisconnectFnc needs
 * to be invoked whenever there is a DC event from platform. 
 **/
JC_RETCODE jdd_DataConnSetDisconnectCB (JDD_DATA_CONN_HANDLE	vDCHandle, \
						JC_DATA_CONN_DISCONNECT_CALLBACK	cbDisconnectFnc)
{
	JC_RETCODE rCode = JC_ERR_NOT_IMPLEMENTED ;
	mmi_trace (1,"JDD_LOG: jdd_DataConnSetDisconnectCB : Exit[%X]", rCode) ;
	return rCode ;
}


/**
 * @ingroup		jdd_dataconnection
 * @brief		This function establishes the data connection.
 * @param[in]	vDCHandle Specifies the data connection handle, which 
 *				is returned by #jdd_DataConnInitialize.
 * @param[in]	pstDataConn Specifies the data connection information.
 * @param[out]	puiConnID Returns the unique connection identifier associated
 * 				with this specific data connection. 
 * @retval		JC_OK on Success.
 * @retval		JC_ERR_DATA_CONN_CONTINUE Specifies that the data connection 
 *				function call is non-blocking; data connection layer  
 *				shall call the callback function once the operation is 
 *				completed for progress (registered using 
 *				jdd_DataConnSetProgressCB) or disconnect (registered using
 *				jdd_DataConnSetDisconnectCB) or error (registered using 
 *				jdd_DataConnSetStatusCB) in making data connection.
 * @retval		JC_ERR_DATA_CONN_UNKNOWN on failure.
 *
 * This function makes the data connection. The handle obtained for the 
 * connection is then used to perform other data connection and socket
 * related operations.
 * The unique connection identifier returned from this function is passed 
 * while creating the socket (jdd_NetSocket) to bind to this specific
 * service provider.
 **/
#ifdef __MMI_SUPPORT_JME__
extern s8 vm_status;
#endif

 JC_RETCODE jdd_DataConnMakeConnection (JDD_DATA_CONN_HANDLE vDCHandle, \
										ST_GENERIC_DATA_CONN *pstDataConn, \
									    JC_UINT32 *puiConnID)
{
	JC_RETCODE				rCode ;
	JC_USER_INFO_CONNID	*pstConIdInfo ;
	UINT8					nCid = 0 ;
	UINT8					State = 0 ;
	CFW_SIM_ID				nSimID = 0 ;
	 
	jdd_AssertOnFail ((JC_NULL != pstDataConn) && (JC_NULL != puiConnID), \
		 "jdd_DataConnMakeCoOnnection: Invalid parameter") ;
	jdd_AssertOnFail ( (E_DATA_CONN_GPRS_BEARER_TYPE == pstDataConn->eBearerType),
		 "jdd_DataConnMakeConnection: Invalid Data Connection Type");
	//initialize Data connection callback function
//	CFW_SetDataConnFun (jdd_Data_Conn_Callback);
	// initialize TCPIP callback function
//	CFW_SetTCPIPCallBack (jdd_SocketEventcb);
      

#ifdef __MMI_MULTI_SIM__
	nSimID = wapmmsgetsimid (pstDataConn->uiConnID) ;
#endif
	mmi_trace (0, "JDD_LOG: MakeConnection  APN[%s], CONID [%d], SIMID[%d] username=%s,pass=%s",\
		 pstDataConn->uDataConnType.stGPRSDataConn.acAPN, nCid, nSimID,pstDataConn->acUserName,pstDataConn->acPassword) ;
	 
	 nwap_uti = FREE_UTI;
#ifdef __ADP_MULTI_SIM__
	CFW_GetGprsAttState (&State,nSimID) ;
#else
	CFW_GetGprsAttState (&State) ;
#endif

	if(1 == State)
	{
#ifdef  __MMI_SUPPORT_JME__
//		if (vm_status)
//			return JC_ERR_DATA_CONN_UNKNOWN;
#endif

      
#ifdef __MMI_MULTI_SIM__
if (ATTACH_Status[nSimID]==0x01)//add by wuys 2011-03-16, 0: detached, 1:attaching, 2:attached
#else
if(ATTACH_Status ==0x01)//add by wuys 2011-03-16, 0: detached, 1:attaching, 2:attached
#endif
{
   //  rVal = ERR_SUCCESS ;
   		mmi_trace (1, "JDD_LOG: attaching Status") ;
		pstConIdInfo = wapmmsAddToChildList(nSimID , nCid, nwap_uti,
			pstDataConn->uDataConnType.stGPRSDataConn.acAPN,pstDataConn->acUserName,pstDataConn->acPassword,E_DATACONN_ATTACHING) ;
		*puiConnID = (JC_UINT32)pstConIdInfo ;

	 rCode = JC_ERR_DATA_CONN_CONTINUE ;

}else{
		mmi_trace (1, "JDD_LOG: Already Attached Status") ;
		pstConIdInfo = wapmmsAddToChildList(nSimID , nCid, nwap_uti,
			pstDataConn->uDataConnType.stGPRSDataConn.acAPN,pstDataConn->acUserName,pstDataConn->acPassword,E_DATACONN_ACTIVATING) ;
		*puiConnID = (JC_UINT32)pstConIdInfo ;

		rCode = StartGPRS (pstConIdInfo) ;
		if (JC_OK == rCode)
		{	 
			rCode = JC_ERR_DATA_CONN_CONTINUE ;
		}
		else
		{
			mmi_trace (1, "JDD_LOG: Activation Req Failed") ;
			DeleteChild(pstConIdInfo) ; 
			rCode = JC_ERR_DATA_CONN_UNKNOWN ;
		}
}
	}
	else
	{
		mmi_trace (1, "JDD_LOG: Not Attached, Adding to the linklist") ; 
		pstConIdInfo = wapmmsAddToChildList(nSimID , nCid, nwap_uti,
			pstDataConn->uDataConnType.stGPRSDataConn.acAPN, pstDataConn->acUserName,pstDataConn->acPassword,E_DATACONN_ATTACHING) ;
		*puiConnID = (JC_UINT32)pstConIdInfo ;
#ifdef __ADP_MULTI_SIM__
		if (ERR_SUCCESS == CFW_GprsAtt( CFW_GPRS_ATTACHED, nwap_uti, nSimID))
#else
		if (ERR_SUCCESS == CFW_GprsAtt( CFW_GPRS_ATTACHED, nwap_uti))
#endif
		{
		#ifdef __MMI_MULTI_SIM__
ATTACH_Status[nSimID]=0x01;//add by wuys 2011-03-16, 0: detached, 1:attaching, 2:attached
#else
ATTACH_Status =0x01;//add by wuys 2011-03-16, 0: detached, 1:attaching, 2:attached
#endif

			mmi_trace (1, "JDD_LOG: Data Connection Attach req successfull") ;
#ifdef __ADP_MULTI_SIM__
	             CFW_SetDataConnFunEx (jdd_Data_Conn_Callback,(UINT8)nSimID,0);
			gprs_attach_start_timer(nSimID);

#else  /* No Defined __ADP_MULTI_SIM__ */
	             CFW_SetDataConnFunEx (jdd_Data_Conn_Callback,0);
			gprs_attach_start_timer();

#endif /* __ADP_MULTI_SIM__ */
			 rCode = JC_ERR_DATA_CONN_CONTINUE ;
		}
		else
		{
			mmi_trace (1, "JDD_LOG: Data Connection Attach req Error") ;
			DeleteChild(pstConIdInfo) ;
			rCode = JC_ERR_DATA_CONN_UNKNOWN ;
		}
	}
	return rCode ;
}


/**
 * @ingroup		jdd_dataconnection 
 * @brief 		This function cancels the connection which is in 
 *				progress but not yet successfully connected.
 * @param[in] 	vDCHandle Specifies the data connection handle, which is
 * 				returned from jdd_DataConnInitialize.
 * @param[in] 	uiConnID Specifies the connection identifier, which is 
 * 				returned from jdd_DataConnMakeConnection
 * @retval		JC_ERR_DATA_CONN_CANCEL on failure
 * @retval		JC_OK on success
 *
 * This function is to be called when "Connected" event is not 
 * received from data connection manager
 */
JC_RETCODE jdd_DataConnCancelConnection (JDD_DATA_CONN_HANDLE vDCHandle, \
										 JC_UINT32 uiConnID)
{
	JC_RETCODE			rCode = JC_OK ;
	JC_USER_INFO_CONNID *pstConIdInfo = (JC_USER_INFO_CONNID *)uiConnID ;

	mmi_trace (1, "JDD_LOG: jdd_DataConnCancelConnection called for node %d", uiConnID) ;
	if (E_FALSE == IsValidDataConnNode (pstConIdInfo))
	{
		mmi_trace(1,"JDD_LOG: jdd_DataConnCancelConnection called for deleted node");
		return JC_OK ;
	}
	if (pstConIdInfo->eConState == E_DATACONN_ATTACHING)
	{
#ifdef __MMI_MULTI_SIM__
		gprs_attach_stop_timer(pstConIdInfo->nSimID);
#else  /* No Defined __MMI_MULTI_SIM__ */
		gprs_attach_stop_timer();
#endif /* __MMI_MULTI_SIM__ */
		DeleteChild(pstConIdInfo);
		rCode = JC_OK ;
	}
	else if (pstConIdInfo->eConState == E_DATACONN_ACTIVATING)
	{
		pstConIdInfo->bIsCancelRequested = E_TRUE ;
		rCode = JC_ERR_DATA_CONN_CONTINUE ;
	}
	else
	{
		rCode = jdd_DataConnDisconnectConnection (vDCHandle, uiConnID) ;
	}
	mmi_trace (1, "JDD_LOG: jdd_DataConnCancelConnection : Exit conid [%d], eConState [%d], rCode: %X", \
				pstConIdInfo->nCid, pstConIdInfo->eConState, -rCode) ;
	return rCode ;
}


/**
 * @ingroup		jdd_dataconnection 
 * @brief 		This function disconnects the data connection.
 * @param[in] 	vDCHandle Specifies the data connection handle, which is 
 * 				returned by #jdd_DataConnInitialize.
 * @param[in] 	uiConnID Specifies the connection identifier, which is 
 *				returned by #jdd_DataConnMakeConnection
 * @retval		JC_OK on success
 * @retval		JC_ERR_DATA_CONN_UNKNOWN on failure
 *
 * This function is to be called after "Connected" event has been  
 * received from data connection manager.
 */
JC_RETCODE jdd_DataConnDisconnectConnection (JDD_DATA_CONN_HANDLE \
					vDCHandle, JC_UINT32 uiConnID)
{
	JC_RETCODE			rCode = JC_OK ;
	JC_USER_INFO_CONNID *pstConIdInfo = (JC_USER_INFO_CONNID *)uiConnID ;

	if (IsValidDataConnNode(pstConIdInfo) == E_FALSE)
	{
		mmi_trace (1,"JDD_LOG: jdd_DataConnDisconnectConnection called for deleted node");
		return JC_OK ;
	}

	nwap_uti = FREE_UTI ;
	pstConIdInfo->nUTI = nwap_uti ;

	mmi_trace (1,"JDD_LOG: jdd_DataConnDisconnectConnection Node [%d], connid [%d], state [%d], uti [%d], sim ID [%d]",\
   				uiConnID, pstConIdInfo->nCid, pstConIdInfo->eConState, pstConIdInfo->nUTI, pstConIdInfo->nSimID) ;
#ifdef __ADP_MULTI_SIM__	 
	   rCode = CFW_GprsActEx (CFW_GPRS_DEACTIVED, pstConIdInfo->nCid, nwap_uti, pstConIdInfo->nSimID,TRUE) ;

//	   rCode = CFW_GprsAct (CFW_GPRS_DEACTIVED, pstConIdInfo->nCid, nwap_uti, pstConIdInfo->nSimID) ;
#else
	   rCode = CFW_GprsActEx (CFW_GPRS_DEACTIVED,  pstConIdInfo->nCid,  nwap_uti,TRUE) ;

//	   rCode = CFW_GprsAct (CFW_GPRS_DEACTIVED,  pstConIdInfo->nCid,  nwap_uti) ;
#endif
	if (ERR_SUCCESS == rCode)
	{
		mmi_trace (1, "JDD_LOG: Disconnect Connection: rCode: 0x%X", -rCode) ;
		pstConIdInfo->eConState = E_DATACONN_DEACTIVATING ;
		rCode = JC_ERR_DATA_CONN_CONTINUE ;
	}
	else
	{
		mmi_trace (1, "JDD_LOG: Disconnect Connection: rCode: 0x%X", -rCode) ;
		rCode = JC_ERR_DATA_CONN_CANCEL ;

		MMI_ASSERT(0);

		if(pstConIdInfo)
		{
			DeleteChild (pstConIdInfo) ;
			if (GetNodeWithSimID (pstConIdInfo->nSimID)== NULL)
			{                    
				mmi_trace (1, "JDD_LOG: deactive success, Remove G icon");
				Send_GPRS_Status_Ind(L4C_PDP_DEACTIVED);
			}
		}
	}
	mmi_trace (1, "JDD_LOG: Disconnect Connection:Exit conn ID %d, rCode: %d", pstConIdInfo->nCid, rCode) ;
	/*--- Platform Specific Implementation End ---*/
	 
	 return (rCode) ;
}	


/**
 * @ingroup		jdd_dataconnection 
 * @brief     	Checks whether the data connection is available.
 * @param[in] 	vDCHandle Specifies the data connection handle, which is 
 * 				returned by #jdd_DataConnInitialize.
 * @param[in] 	uiConnID Specifies the connection identifier, which is 
				returned by #jdd_DataConnMakeConnection
 * @retval    	JC_OK If the specified data connection is still active.
 * @retval		JC_ERR_DATA_CONN_UNKNOWN on error.
 * Some platforms do not provide information when the data connection is 
 * dropped. This function is provided to cross check whether the data 
 * connection is available or not. 
 * If the platform can invoke JC_DATA_CONN_DISCONNECT_CALLBACK when the 
 * data connection is disconnected, then this function can return JC_OK 
 * without any check.
 */

JC_RETCODE jdd_DataConnIsAvailable (JDD_DATA_CONN_HANDLE	vDCHandle, \
									JC_UINT32 uiConnID)
{
	JC_RETCODE      rCode ;
	JC_USER_INFO_CONNID * pstConIdInfo = (JC_USER_INFO_CONNID *)uiConnID;
	
	jdd_AssertOnFail ((0 != uiConnID), \
				"jdd_DataConnIsAvailable: Invalid parameter") ;
	if (E_TRUE == IsValidDataConnNode(pstConIdInfo) &&
		E_DATACONN_ACTIVATED == pstConIdInfo->eConState)
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_DATA_CONN_UNKNOWN ;
	}
	mmi_trace (1,"JDD_LOG: jdd_DataConnIsAvailable returns %X", -rCode ) ;
	return rCode ;
}

/**
 * @ingroup		jdd_dataconnection
 * @brief		This function compares whether specified data connections
 *				are same.
 * @param[in]	vDCHandle Specifies the data connection handle, 
 *				which is returned by #jdd_DataConnInitialize.
 * @param[in] 	pstDataConn1 Specifies the data connection information, 
 *				which needs to be compared.
 * @param[in] 	pstDataConn2 Specifies the other data connection information, 
 *				which needs to be compared.
 * @retval		E_TRUE  if both the data connections are same
 * @retval		E_FALSE if the data connections are different
 * 
 * The application shall use this API to re-use the existing data connection
 * for the new session, if one data connection is already connected. 
 */
JC_BOOLEAN jdd_DataConnCompare (JDD_DATA_CONN_HANDLE		vDCHandle, \
								const ST_GENERIC_DATA_CONN	*pstDataConn1, \
								const ST_GENERIC_DATA_CONN	*pstDataConn2) 
{
	JC_BOOLEAN		bRetVal = E_FALSE;

	mmi_trace (1,"JDD_LOG: jdd_DataConnCompare : Entry") ;
	PARAM_INTENTIONALLY_NOT_USED (vDCHandle) ;

	jdd_AssertOnFail ((JC_NULL != pstDataConn1) && (JC_NULL != pstDataConn2), \
						"jdd_DataConnCompare: pstDataConn is NULL") ;		

	if ((pstDataConn1->eBearerType == pstDataConn2->eBearerType) && \
		(0 == jc_strcmp (pstDataConn1->uDataConnType.stGPRSDataConn.acAPN,
				pstDataConn2->uDataConnType.stGPRSDataConn.acAPN)) &&
		(pstDataConn1->uiConnID == pstDataConn2->uiConnID) &&
		(0 == jc_strcmp (pstDataConn1->acUserName, pstDataConn2->acUserName)) && \
		(0 == jc_strcmp (pstDataConn1->acPassword, pstDataConn2->acPassword)))
	{
		bRetVal = E_TRUE;
		
	}
	mmi_trace(12,"JDD_LOG: jdd_DataConnCompare: Returns [%d] id1[%d] id2[%d]", bRetVal, \
				pstDataConn1->uiConnID, pstDataConn2->uiConnID) ;
	/*
	else 
	{
		if (E_DATA_CONN_CSD_BEARER_TYPE == pstDataConn1->eBearerType)
		{
			if ((0 != jc_strcmp \
				(pstDataConn1->uDataConnType.stCSDDataConn.acPhoneNumber, \
				 pstDataConn2->uDataConnType.stCSDDataConn.acPhoneNumber)) || \
				(pstDataConn1->uDataConnType.stCSDDataConn.uiBaudRate != \
				pstDataConn2->uDataConnType.stCSDDataConn.uiBaudRate) || \
				(pstDataConn1->uDataConnType.stCSDDataConn.eCallType != \
				pstDataConn2->uDataConnType.stCSDDataConn.eCallType)) \
			{
				bRetVal = E_FALSE ;
				mmi_trace(1,"JDD_LOG: jdd_DataConnCompare: Returns [%d]", bRetVal) ;
			}
			else
			{
				bRetVal = E_TRUE ;
			}
		}
		else if (E_DATA_CONN_GPRS_BEARER_TYPE == pstDataConn1->eBearerType)
		{
			if (jc_strcmp (pstDataConn1->uDataConnType.stGPRSDataConn.acAPN,
				pstDataConn2->uDataConnType.stGPRSDataConn.acAPN))
			{
				bRetVal = E_FALSE ;
			}
			else
			{
				bRetVal = E_TRUE ;
			}
		}
		else if (E_DATA_CONN_OTHER_BEARER_TYPE == pstDataConn1->eBearerType)
		{
			bRetVal = E_TRUE ;
		}
		else
		{
			bRetVal = E_FALSE ;
		}
	}	
	*/
	mmi_trace (1, "JDD_LOG: jdd_DataConnCompare: Returns [%d]", bRetVal) ;
	return (bRetVal) ;
}
								

/**
 * @ingroup		jdd_dataconnection
 * @brief		This function provides IP address associated 
 *				with specified connection ID.
 * @param[in]	vDCHandle Specifies the data connection handle, 
 *				which is returned by #jdd_DataConnInitialize.
 * @param[in] 	uiConnID Specifies the connection identifier, 
 *				which is returned by #jdd_DataConnMakeConnection
 * @param[out]	pcIPAddress Returns the IP Address assigned to 
 *				the given connection identifier.
 * @warning		This function has been deprecated as most of the environment 
 *				is not able to provide the IP Address. 
 */
JC_RETCODE jdd_DataConnGetIPAddress (JDD_DATA_CONN_HANDLE	vDCHandle, \
									 JC_UINT32				uiConnID, \
									 JC_INT8				*pcIPAddress) 
{
	JC_RETCODE rCode = JC_ERR_NOT_IMPLEMENTED ;
	mmi_trace (1,"JDD_LOG: jdd_DataConnGetIPAddress : Exit[%X]", -rCode) ;
	return rCode ;
}
/*END OF FILE*/
#endif
