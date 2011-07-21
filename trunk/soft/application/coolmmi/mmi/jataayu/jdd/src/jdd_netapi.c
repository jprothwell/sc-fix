#define MODULE_NAME "JDD_NET"
#define FILE_NAME "jdd_netapi.c"
#ifdef JATAAYU_SUPPORT
/***************************************************************************
 *
 * File Name jdd_netapi.c
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
 * @file	jdd_netapi.c
 * @ingroup	jdd_netapi
 * @brief	Describes the net related API's.
 **/
 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
#include "cfw.h"
#include "adp_events.h"
#include "tcpip_inet.h"
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include <ddl.h>
#include <jcal.h>
#include <jcalnet.h>
#include <jdd_configapi.h>
#include <jdi_trfetch.h>
#include <jdi_cutils_str.h>
#include "jdi_cutils_tcsstr.h"
#include <jddex_dataconnection.h>


 /***************************************************************************
 * Macros
 **************************************************************************/
#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					NET_MODULE
#else
#define __MODULE_ID__					0
#endif

#if 0
#define     JDD_DUMP_SEND_BUFF
#define     JDD_DUMP_RECV_BUFF
#include "globaldefs.h"
#include "jwap_str_id.h"
#endif

/* 
 * If the socket is event based, then please ensure that 
 * TRANSPORT_SOCKET_EVENT is defined in ddldefines.h	
 */

#ifdef  JDD_DUMP_SEND_BUFF
#define JDD_SEND_FILE 			"/t/packetDump.txt"
#endif

#ifdef  JDD_DUMP_RECV_BUFF
#define JDD_RECV_FILE  			"/t/packetDump.txt"
#endif


 /***************************************************************************
 * Global Variable declarations and definitions
 **************************************************************************/
 
 /***************************************************************************
 * Local Function Prototypes
 **************************************************************************/
 //UINT32 CFW_SetTCPIPCallBack ( PFN_TCPIP_CALLBACK pfnEv ) ;

 /***************************************************************************
 * All Local Function Definitions
 **************************************************************************/

#if defined(JDD_DUMP_SEND_BUFF) || defined (JDD_DUMP_RECV_BUFF)

/**
 * @ingroup		jdd_netapi
 * @brief		Writes buffer to a specified file
 * @param[in]	pcBuffer recv/send buffer to be written to file
 * @param[in]	uiBufLen length of the recv/send buffer
 * @param[in]	pmDumpFileName File Name to which the buffer has to be dumped
 */
extern BOOL g_hasMountedTFlashFS ;
static void dumpBufferToFile (JC_INT8 *pcBuffer, JC_UINT32 uiBufLen, \
										const JC_INT8* pmDumpFileName)
{
	/* To be used for platforms where-in dump file size is restricted */
	/* JC_UINT32 uiFileSize ; */
	
	JDD_FILE *vfLogHandle = JC_NULL ;
	JDD_FSHANDLE    hFSHandle = JC_NULL ;
	JC_CHAR			amPath [255] ;

	jdd_AssertOnFail ((JC_NULL != pmDumpFileName), \
	"dumpBufferToFile: Filename is NULL ") ;
	if (FALSE == g_hasMountedTFlashFS)
	{
		DisplayPopup(GetString (STR_ID_WAP_NO_MEMORY_CARD),
							IMG_GLOBAL_ERROR, 1, 2000, 1) ;
		return;
	}
	jdi_CUtilsTcsStrCpy (pmDumpFileName, amPath) ;
	if (JC_OK == jdd_FSInitialize (&hFSHandle))
	{
		vfLogHandle = (JDD_FILE*)jdd_FSOpen(hFSHandle, amPath, \
								E_CREATE_APPEND_WRITE_MODE) ;
	}
	
	/* Below mentioned utility is for a platform where-in file
	size can be restricted
	*/
	
	/*
	jdd_FSGetSize(pLogHandle, &uiFileSize) ;

	if(LOG_MAX_FILE_SIZE < uiFileSize)
	{
		jdd_FSClose (pLogHandle) ;
		vfLogHandle  = (JDD_FILE*)jdd_FSOpen(JC_NULL, pmDumpFileName, \
									E_CREATE_WRITE_MODE) ;
	}
	*/
	if(vfLogHandle)
	{
		jdd_FSWrite ((void*)pcBuffer, sizeof( JC_UINT8 ), uiBufLen, \
						vfLogHandle) ;
	}
	
	jdd_FSClose (vfLogHandle) ;
	return ;
}

#endif

#if 0
/**
 * @ingroup		jdd_netapi
 * @brief 		This function is used to convert platform specific error to
 *				JCF error corresponding to net module
 * @param[in]	iErrVal PlatForm specific Error code
 * @retval		Error code of type JC_RETCODE
 */

static JC_RETCODE getNetJCFError (JC_UINT32	iErrVal)
{
	JC_RETCODE rCode ;

	switch (iErrVal)
	{
		/* Replace <platformErrVal> with Platform specific error values */
		/*
		case <platformErrVal>:
			rCode = JC_ERR_NET_INIT ;
			break ;
		*/
		case ERR_TCPIP_ECONNABORTED:
			rCode  = JC_ERR_NET_CONNECT ;
			break ;

		/*
		case <platformErrVal>:
			rCode = JC_ERR_NET_DNS_RESOLVE ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_NET_TIMEOUT ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_NET_SEND ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_NET_RECV ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_NET_SOCKET ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_NET_SOCKET_NOT_READY ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_NET_GET_SOCKOPT ;
			break ;
			
		case <platformErrVal>:
			rCode = JC_ERR_NET_SET_SOCKOPT ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_NET_WOULDBLOCK ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_NET_ACCEPT ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_NET_LISTEN ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_NET_CONN_ABORT ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_NET_DNS_WOULDBLOCK ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_NET_SOCKET_FULL ;
			break ;
		*/
		default :
			rCode = JC_ERR_UNKNOWN ;
			break ;
	}	

	return (rCode) ;
}
#endif



#define TRANSPORT_SOCKET_EVENT
#ifdef TRANSPORT_SOCKET_EVENT	

/**
 * @ingroup		jdd_netapi
 * @brief 		This function is used for sending socket events to transport
 * @param[in] 	iSocketID Specifies the socket ID which is responsible for the
 *				event
 * @param[in] 	eSocketEvent Type of event to be sent
 * @param[in] 	pucReadBuff Buffer read from the socket and 
 *				to be sent to transport
 *				(Should be present only for E_TR_SOCKET_READ event)
 * @param[in] 	uiBuffSize Size of buffer to be sent 
 *				(Should be greater than 0 only for E_TR_SOCKET_READ event)
 * @param[in] 	rStatusCode Return value corresponding to socket read 
 *				operation
 * @retval 		JC_OK on success
 * @retval		Specific Error code on failure
 * This function shall be invoked by the event based socket layer. 
 * Data from the socket should be read and this function should be invoked 
 * with the buffer received from the socket. pucReadBuff can be freed by 
 * the caller once the function returns. 
 * PSUEDOCODE for Using this API:
 *
 * [Function] 
 * sockRecvCallBack 
 * [Start]
 * pucReadBuff = alloc_memory ()
 * status = sock_read (iSocketID, pucReadBuff, &uiBuffSize)
 * rStatusCode = getNetJCFError (status)
 * rCode = sendSockEventToTransport (iSocketID, E_TR_SOCKET_READ, pucReadBuff, 
 *				uiBuffSize, rStatusCode)
 * free_memory (pucReadBuff)
 * [End]
 */

static JC_RETCODE sendSockEventToTransport(JC_INT32 iSocketID, \
		E_TR_SOCKET_EVENT_TYPE eSocketEvent, JC_UINT8 *pucReadBuff, \
		JC_UINT32 uiBuffSize, JC_RETCODE rStatusCode)
{
	JC_RETCODE      rCode = JC_OK ;
	JC_EVENT_DATA   stEventData ;
	ST_TR_SOCKET_EVENT *pstSockEvData = JC_NULL ;

	jc_memset(&stEventData, 0, sizeof (JC_EVENT_DATA)) ;
	stEventData.srcTaskInfo.iTaskID = E_TASK_TRANSPORT ;
	stEventData.srcTaskInfo.iAppID = E_TASK_TRANSPORT ;
	stEventData.destTaskInfo.iTaskID = E_TASK_TRANSPORT ;
	stEventData.destTaskInfo.iAppID = E_TASK_TRANSPORT ;
	stEventData.iEventType = (JC_INT32) E_TR_SOCKET_EVENT ;
	stEventData.uiEventInfoSize = sizeof (ST_TR_SOCKET_EVENT) ;
	
	if (E_TR_SOCKET_READ == eSocketEvent)
	{
		stEventData.uiEventInfoSize += uiBuffSize ;
	}
		
	rCode = jdd_QueueAlloc (&stEventData) ;
	
	pstSockEvData = (ST_TR_SOCKET_EVENT *) stEventData.pEventInfo ;
	if (JC_NULL != pstSockEvData)
	{
		pstSockEvData->stIPTaskInfo.iTaskID = E_TASK_TRANSPORT ;
		pstSockEvData->stIPTaskInfo.iAppID = E_TASK_TRANSPORT ;
		pstSockEvData->iSocketID = iSocketID ;
		
		if (E_TR_SOCKET_READ == eSocketEvent)
		{
			if ((JC_NULL != pucReadBuff) && (0 < uiBuffSize))
			{				
				pstSockEvData->uiBufferLen = uiBuffSize ;
				jc_memcpy (pstSockEvData->ucBuffer, pucReadBuff, 
							uiBuffSize) ;
			}
			else
			{
				pstSockEvData->uiBufferLen = 0 ;
			}
		}
		else
		{
			pstSockEvData->uiBufferLen = 0 ;
		}
		
		/* Event Sent even if rCode is not JC_OK */
		pstSockEvData->rCode = rStatusCode ;
		pstSockEvData->eSocketEvent = eSocketEvent ;
		rCode = jdd_QueueSend (&stEventData) ;
		if (JC_OK != rCode)
		{
			jdd_QueueFree (&stEventData) ;
			mmi_trace(1,"JDD_LOG: sendSockEventToTransport: Returns [%d]", rCode) ;
		}
	}
	else
	{
		jdd_QueueFree (&stEventData) ;
		mmi_trace(1,"JDD_LOG: sendSockEventToTransport: Returns [%d]", rCode) ;
	}
	
	return (rCode) ;
}

static void SendDNSEvent(UINT32 ipAddr, UINT8 *pcHostName,JC_RETCODE rVal)
{
	JC_EVENT_DATA pstEventData ;
	ST_TR_DNS_EVENT *pstDNSEvent = JC_NULL ;
	JC_RETCODE rCode ;

	mmi_trace (1, "JDD_LOG: SendDNSEvent :pcHostName[%s]",pcHostName);
	
	jc_memset(&pstEventData, 0, sizeof(JC_EVENT_DATA) ) ;
	pstEventData.srcTaskInfo.iTaskID = E_TASK_TRANSPORT;
	pstEventData.srcTaskInfo.iAppID= E_TASK_TRANSPORT;
	pstEventData.destTaskInfo.iTaskID = E_TASK_TRANSPORT;
	pstEventData.destTaskInfo.iAppID = E_TASK_TRANSPORT;
	
	pstEventData.uiEventInfoSize = sizeof (ST_TR_DNS_EVENT) ;
	pstEventData.iEventType = E_TR_DNS_EVENT ;


	rCode = jdd_QueueAlloc (&pstEventData) ;
	
	if(JC_OK == rCode)
	{
		pstDNSEvent = (ST_TR_DNS_EVENT *)pstEventData.pEventInfo ;
		if (JC_OK == rVal)
		{
			jc_sprintf(pstDNSEvent->aucIPAddress ,"%d.%d.%d.%d",(jdd_NetNtohl(ipAddr)>>24 &0xff),
				(jdd_NetNtohl(ipAddr)>>16 &0xff),
				(jdd_NetNtohl(ipAddr)>>8 &0xff),
				(jdd_NetNtohl(ipAddr) &0xff) ) ;
			if(JC_NULL != pcHostName)
			{
				jc_strcpy(pstDNSEvent->aucHostName, pcHostName) ;
			}
			
			mmi_trace (1, "@@@ DNS RESOLVED :%s",pstDNSEvent->aucIPAddress) ;
		}
		else
		{
			mmi_trace (1, "@@@ DNS failed") ;
		}
		pstDNSEvent->rCode = rVal ;
		jdd_QueueSend (&pstEventData) ;
	}
	
	mmi_trace(1,"JDD_LOG: SendDNSEvent Exit [%d]", rCode) ;

}

BOOL jdd_SocketEventcb (COS_EVENT *pstCosEv)
{
	JC_INT32 iSocketID ;
	E_TR_SOCKET_EVENT_TYPE eSocketEvent = 0 ;
	JC_UINT8 *pucReadBuff ;
	JC_UINT32 uiBuffSize ;
	JC_RETCODE rCode = JC_OK ;
	BOOL bIsProcessed = TRUE;

	mmi_trace(1,"JDD_LOG: jdd_SocketEventcb :nEventId [%d] nParam1[%d] nParam2[%d] nParam3[%d]",pstCosEv->nEventId,
		pstCosEv->nParam1,pstCosEv->nParam2,pstCosEv->nParam3) ;

	iSocketID = (pstCosEv->nParam1 + 1) ;
	pucReadBuff = JC_NULL ;
	uiBuffSize = 0 ;	
	
	switch (pstCosEv->nEventId)
	{
		case EV_CFW_TCPIP_SOCKET_CONNECT_RSP:
			mmi_trace(TRUE,"JDD_LOG: EV_CFW_TCPIP_SOCKET_CONNECT_RSP Func: %s ", __FUNCTION__);
			eSocketEvent = E_TR_SOCKET_CONNECT ;
			sendSockEventToTransport (iSocketID, eSocketEvent, pucReadBuff, uiBuffSize, rCode) ;
			break ;
	
    	case EV_CFW_TCPIP_SOCKET_CLOSE_RSP:
			mmi_trace(TRUE,"JDD_LOG: EV_CFW_TCPIP_SOCKET_CLOSE_RSP Func: %s ", __FUNCTION__);
			eSocketEvent = E_TR_SOCKET_CLOSE ;			
			//ignore close response
			//sendSockEventToTransport (iSocketID, eSocketEvent, pucReadBuff, uiBuffSize, rCode) ;
			break ;
			
    	case EV_CFW_TCPIP_SOCKET_SEND_RSP:
			mmi_trace(TRUE,"JDD_LOG: EV_CFW_TCPIP_SOCKET_SEND_RSP Func: %s ", __FUNCTION__);
			eSocketEvent = E_TR_SOCKET_WRITE ;			
			sendSockEventToTransport (iSocketID, eSocketEvent, pucReadBuff, uiBuffSize, rCode) ;
			break ;
		case EV_CFW_TCPIP_REV_DATA_IND:
				mmi_trace(TRUE,"JDD_LOG: EV_CFW_TCPIP_REV_DATA_IND Func: %s %d", __FUNCTION__,(JC_UINT32) pstCosEv->nParam2);
			eSocketEvent = E_TR_SOCKET_READ ;			
			sendSockEventToTransport (iSocketID, eSocketEvent, pucReadBuff, uiBuffSize, rCode) ;
			break ;
		case EV_CFW_TCPIP_ACCEPT_IND:
			mmi_trace(TRUE,"JDD_LOG: EV_CFW_TCPIP_ACCEPT_IND Func: %s ", __FUNCTION__);
			eSocketEvent = E_TR_SOCKET_ACCEPT ;			
			sendSockEventToTransport (iSocketID, eSocketEvent, pucReadBuff, uiBuffSize, rCode) ;
			break ;
		case EV_CFW_DNS_RESOLV_SUC_IND:
			mmi_trace(TRUE,"JDD_LOG: EV_CFW_DNS_RESOLV_SUC_IND Func: %s ", __FUNCTION__);
			SendDNSEvent(pstCosEv->nParam1, (UINT8*)pstCosEv->nParam2, JC_OK) ;
			break ;
		case EV_CFW_DNS_RESOLV_ERR_IND:				 
			mmi_trace(TRUE,"JDD_LOG: EV_CFW_DNS_RESOLV_ERR_IND Func: %s ", __FUNCTION__);
			SendDNSEvent(pstCosEv->nParam1, (UINT8*)pstCosEv->nParam2, JC_ERR_NET_DNS_RESOLVE) ;
			break ;

//		case EV_CF_TCPIP_ERR_IND:
		case EV_CFW_TCPIP_ERR_IND:
			{
				if( CFW_ERR_TCPIP_CONN_TIMEOUT == pstCosEv->nParam2 )
				{
					rCode = JC_ERR_NET_CONNECT ;
					eSocketEvent = E_TR_SOCKET_CONNECT ;
				}
				else
				{
					rCode = JC_ERR_NET_SOCKET ;
					eSocketEvent = E_TR_SOCKET_CLOSE;
				}
					
				mmi_trace(TRUE,"JDD_LOG: EV_CFW_TCPIP_ERR_IND Func: %s ", __FUNCTION__);
				
				sendSockEventToTransport (iSocketID, eSocketEvent, pucReadBuff, uiBuffSize, rCode) ;
			}
			break ;

		case EV_CFW_TCPIP_CLOSE_IND:
			mmi_trace(TRUE,"JDD_LOG: EV_CFW_TCPIP_CLOSE_IND Func: %s ", __FUNCTION__);
			eSocketEvent = E_TR_SOCKET_CLOSE ;
			rCode = JC_OK ;
			sendSockEventToTransport (iSocketID, eSocketEvent, pucReadBuff, uiBuffSize, rCode) ;
			break ;
			
		default:
			eSocketEvent = E_TR_SOCKET_CONNECT ;
			rCode = JC_ERR_NET_CONNECT ;			
			mmi_trace(1,"JDD_LOG: jdd_SocketEventcb : Unknown case") ;
			bIsProcessed = FALSE ;
			break ;
	}

	mmi_trace(1,"JDD_LOG: jdd_SocketEventcb SockID: %d, Event: %d @@@", iSocketID, eSocketEvent) ;
	return bIsProcessed ;
		
}
#endif


/***************************************************************************
 * All Global Function Definitions
 **************************************************************************/

/**
 * @ingroup jdd_netapi
 * @brief 	This function initializes the network layer, if required 
 * 			by the platform before using the network socket calls.
 * @retval	JC_OK on success
 * @retval	JC_ERR_NET_INIT on failure
 **/
 JC_RETCODE jdd_NetInitialize (void)
 {
	 JC_RETCODE      rCode = JC_OK ;
	 /*--- Platform Specific Variables ---*/
	 /*-----------------------------------*/

	 mmi_trace(1,"JDD_LOG: jdd_NetInitialize : Entry") ;

	 /*--- Platform Specific Implementation Start ---*/

	 /* Call for handling DNS Cacheing operation */
	 /*
	 * Incase of PlatForm Supporting event based Socket, Platform to create 
	 * TASK in initlization and send Socket events to transport 
	 */
	 /* PlatForm Specific Function for NetInitialize */				

#if 0 /*move to jdd_DataConnMakeConnection for multi gprs applications*/
	if(ERR_SUCCESS == CFW_SetTCPIPCallBack (jdd_SocketEventcb))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_NET_INIT ;
	}
#endif 

	 
	 /*
	 if (Platform API succeeds)
	 {
		rCode = JC_OK ;
	 }
	 else
	 {
		(PlatForm Specific call for DeInitlizing DNS Cacheing operation)
		rCode = getNetJCFError (<platformErrValue>)  ;
		JDD_LOG_INFO1 ("jdd_NetInitialize: Returns [%d]", rCode) ;
	 }
	 */		

	 /*--- Platform Specific Implementation End ---*/

	 mmi_trace(1,"JDD_LOG: jdd_NetInitialize : Exit [%d]",rCode) ;
	 return (rCode) ;
 }


 /**
 * @ingroup	jdd_netapi
 * @brief 	This call de-initializes the network layer.
 * @retval	JC_OK on success
 * @retval	JC_ERR_NET_INIT on failure
 */
 JC_RETCODE jdd_NetDeinitialize (void)
 {
	 JC_RETCODE rCode = JC_OK ;
	 /*--- Platform Specific Variables ---*/
	 /*-----------------------------------*/

	 mmi_trace(1,"JDD_LOG: jdd_NetDeinitialize : Entry") ;

	 /*--- Platform Specific Implementation Start ---*/

	/* Incase of PlatForm Supporting event based Socket, Platform to
	 * Destroy the TASK which was created during jdd_NetInitialize*/

	 /*
	 if (Platform API succeeds)
	 {
		rCode = JC_OK ;
	 }
	 else
	 {
		rCode = getNetJCFError (<platformErrValue>) ;
		JDD_LOG_INFO1 ("jdd_NetDeinitialize: Returns [%d]", rCode) ;
	 }
	 */
	 /* Call for DeInitlizing DNS Cacheing operation */

	 /*--- Platform Specific Implementation End ---*/

	 mmi_trace(1,"JDD_LOG: jdd_NetDeinitialize : Exit[%d]",rCode) ;
	 return (rCode) ;
 }

 /** 
 * @ingroup		jdd_netapi
 * @brief		This function creates an endpoint for communication.
 * @param[in]	eSocketDomain Specifies the socket domain for communication
 * @param[in]	eSocketType	Specify the type of the socket  
 *				whether its TCP(E_TCP_SOCKET) connection oriented   
 *				type of socket OR UDP(E_UDP_SOCKET) connectionless
 *				type of socket
 * @param[in]	eProtocol Specifies the protocol type. 
 *				This flag is currently not used by Jataayu core product engine.
 * @param[in]	uiConnID Specifies the data connection identifier for which 
 *				the created socket needs to be bounded with the specific 
 * 				transport service provided. 
 * @param[out]	piSocketID Returns the socket identifier associated with
 *				the created socket.
 * @retval		JC_OK on success
 * @retval		JC_ERR_NET_SOCKET when there is any error in creating the 
 * 				socket							 
 *
 * This function creates a socket identifier and any related resources to be  
 * allocated and bound to a specific transport service provider specified by 
 * the uiConnID. If the uiConnID has zero then it needs to bounded with 
 * the default service provider. Currently Jataayu core product engine
 * supports sockets of only IPv4 type. IPv6 is not supported.
 * @see 		#jdd_NetClose 
 */
UINT8 wap_mms_get_simid();

JC_RETCODE jdd_NetSocket (ESocketDomain eSocketDomain, \
							ESocketType eSocketType, \
							ESocketProtocol eProtocol, \
							JC_UINT32 uiConnID, \
							JC_INT32 *piSocketID)
{
	JC_RETCODE rCode = JC_ERR_NET_SOCKET ; 
	/*--- Platform Specific Variables ---*/
	UINT8		ucDomain, ucType, ucProto ;
	SOCKET		sockId ;
	JC_USER_INFO_CONNID * pstConIdInfo = (JC_USER_INFO_CONNID *)uiConnID;
	
	/*-----------------------------------*/
	mmi_trace(1,"JDD_LOG: jdd_NetSocket: Entry") ;
	
	PARAM_INTENTIONALLY_NOT_USED (eProtocol) ;
    
	jdd_AssertOnFail ((JC_NULL != piSocketID), \
						"jdd_NetSocket: piSocketID is NULL") ;
	jdd_AssertOnFail ((E_PF_INET_v4 == eSocketDomain), \
						"jdd_NetSocket: Invalid eSocketDomain") ;
	jdd_AssertOnFail ((E_SOCK_STREAM == eSocketType) ||  \
						(E_SOCK_DGRAM == eSocketType) , \
						"jdd_NetSocket: Invalid eSocketType") ;
	mmi_trace(1,"JDD_LOG: jdd_NetSocket: ESocketDomain[%d] ESocketType[%d]\
		eProtocol[%d] uiConnID[%d]",eSocketDomain,eSocketType,eProtocol,uiConnID) ;
    
	/*--- Platform Specific Implementation Start ---*/

	if (E_PF_INET_v4 == eSocketDomain)
    {
        /*--- Set platform specific socket family ---*/
		ucDomain = CFW_TCPIP_AF_INET ;
    }
	else
	{
		ucDomain = CFW_TCPIP_AF_UNSPEC ;
	}
	
    if (E_SOCK_STREAM == eSocketType)
    {
        /*--- Set platform specific socket type ---*/
		ucType = CFW_TCPIP_SOCK_STREAM ;
		ucProto = CFW_TCPIP_IPPROTO_TCP ;
    }
	else if (E_SOCK_DGRAM == eSocketType)
    {
        /*--- Set platform specific socket type ---*/
		ucType = CFW_TCPIP_SOCK_DGRAM ;
		ucProto = CFW_TCPIP_IPPROTO_UDP ;
    }   
	else
	{
		ucType = CFW_TCPIP_SOCK_RAW ;
		ucProto = CFW_TCPIP_IPPROTO_IP ;
	}
	/* PlatForm Specific Function for Net Socket */
	sockId = CFW_TcpipSocket (ucDomain, ucType, ucProto) ;

	mmi_trace(1,"JDD_LOG: jdd_NetSocket: sockId = %d ", sockId) ;
	if (-1 != sockId)
	 {
		 UINT8 uiLen =0 ;
		UINT8 acIPAddr[18]= {0,};
		CFW_TCPIP_SOCKET_ADDR stLocalAddr = {0, } ;
	 	
		CFW_SetTCPIPCallBackEx (jdd_SocketEventcb,sockId);

		*piSocketID = (sockId + 1) ;
		
#ifdef __ADP_MULTI_SIM__
		//nSimID=(CFW_SIM_ID)wap_mms_get_simid();
		//nSimID = wapmmsgetsimid(pstConIdInfo->nCid );
		//CFW_GprsGetPdpAddr(pstConIdInfo->nCid, &uiLen, acIPAddr,nSimID);
		CFW_GprsGetPdpAddr(pstConIdInfo->nCid, &uiLen, acIPAddr,pstConIdInfo->nSimID);
#else
		CFW_GprsGetPdpAddr(pstConIdInfo->nCid, &uiLen, acIPAddr);
#endif
		stLocalAddr.sin_len = 0;	
		stLocalAddr.sin_family = CFW_TCPIP_AF_INET;   
		stLocalAddr.sin_port = 0 ;
		stLocalAddr.sin_addr.s_addr = jdd_NetHtonl(acIPAddr[0]<<24 | acIPAddr[1]<<16 | acIPAddr[2]<<8 | acIPAddr[3]);
		
		CFW_TcpipSocketBind(*piSocketID-1, &stLocalAddr, sizeof(CFW_TCPIP_SOCKET_ADDR));
		rCode = JC_OK ;	
	 }
	 else
	 {
		   rCode = JC_ERR_NET_SOCKET ;
	 }

	/*--- Platform Specific Implementation End ---*/		
	
	mmi_trace(1,"JDD_LOG: jdd_NetSocket: sockid[%d] rCode[%x]",*piSocketID, -rCode) ;	
	return (rCode) ;
}

/**
 * @ingroup		jdd_netapi
 * @brief		This function binds the specfied socket with the associated 
 *				local address.
 * @param[in] 	uiSocketID Specifies the socket descriptor, which is returned
 *				by #jdd_NetSocket API.
 * @param[in] 	pcBindAddress Specifies the address to which the socket needs 
 *				to bind. If the value is NULL it needs to bind to the 
 *				default service provider or the provider to which it's bound
 *				during the socket creation.
 * @param[in]	uhPortNo	Specifies the local port number to which 
 *				the socket needs to bind.
 * @retval	   	JC_OK on sucess.
 * @retval	   	JC_ERR_NET_SOCKET, if bind is unsucessfull.
 * 
 * This function is used on an unconnected socket before subsequent calls to 
 * the connect or listen functions. It is used to bind to either 
 * connection-oriented (stream) or connectionless (datagram) sockets.   
 * The socket is binded to specific data connection when it is created, 
 * this function is used to bind to the specific port number. 
 **/
 
JC_RETCODE jdd_NetBind (JC_UINT32 uiSocketID, JC_INT8 *pcBindAddress, \
						JC_UINT16 uhPortNo)
{
	JC_RETCODE rCode ;
	
	/*--- Platform Specific Variables ---*/
	CFW_TCPIP_SOCKET_ADDR	stSockBind ;
	/*-----------------------------------*/
	
	mmi_trace(1,"JDD_LOG: jdd_NetBind: Entry") ;	
	jdd_AssertOnFail ((0 != uiSocketID), "jdd_NetBind: uiSocketId is NULL") ;
	jdd_AssertOnFail ((0 != uhPortNo), "jdd_NetBind: uhPortNo is invalid") ;
	
	if (JC_NULL == pcBindAddress)
	{
		/* 
		 * If pcBindAddress is NULL, then use platform equivalent 
		 * of INADDR_ANY 
		 * Example : uiIpAddr = INADDR_ANY
		 */
	}

	/* Platform Specific Function for bind */
	jc_memset (&stSockBind, 0, sizeof (CFW_TCPIP_SOCKET_ADDR)) ;
	stSockBind.sin_len = 0 ;
	stSockBind.sin_family = CFW_TCPIP_AF_INET ;
	stSockBind.sin_port = jdd_NetHtons(uhPortNo) ;
	stSockBind.sin_addr.s_addr = CFW_TcpipInetAddr(pcBindAddress) ;

	if (CFW_TcpipSocketBind ((uiSocketID-1), &stSockBind, sizeof (CFW_TCPIP_SOCKET_ADDR)) \
			== SOCKET_ERROR)
	{
		rCode = JC_ERR_NET_SOCKET ;
	}
	else
	{
		rCode = JC_OK ;
	}
	
	/*--- Platform Specific Implementation End ---*/

	mmi_trace(1,"JDD_LOG: jdd_NetBind: Exit[%x]",-rCode) ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_netapi
 * @brief	   	This function establishes connection to the specified address
 * @param[in] 	uiSocketID Specifies the socket descriptor which needs to be 
 *				connected
 * @param[in] 	pcIpAddress Specifies the IP Address of the socket to which the
 *				connection should be established.
 * @param[in] 	uhPortNo Specifies the destination port number to which the 
 *				socket should be connected.
 * @param[in] 	pvApplicationData This flag is not used.
 * @retval	   	JC_ERR_NET_CONNECT Returns when the connection request to 
 *				the peer was not successful.
 
 *				non-blocking and the send/receive operation would block.
 * @retval     	JC_OK on successful connection for blocking sockets.
 * 
 * In case of nonblocking sockets this function will return JC_ERR_WOULDBLOCK. 
 * It should call jdd_NetSelect function to determine the completion of the 
 * connection. In case of the blocking sockets, return value JC_OK indicates 
 * that socket is successfully connected. This returns only after the socket 
 * is accepted or the JC_ERR_NET_CONNECT is returned.
 **/
 
JC_RETCODE jdd_NetConnect (JC_UINT32 uiSocketID, JC_INT8 *pcIpAddress, \
						   JC_UINT16 uhPortNo, void *pvApplicationData)
{
	JC_RETCODE rCode ;

	/*--- Platform Specific Variables ---*/
	CFW_TCPIP_SOCKET_ADDR	stSockConnect ;
	/*-----------------------------------*/

	mmi_trace(1,"JDD_LOG: jdd_NetConnect: Entry") ;
	jdd_AssertOnFail ((0 != uiSocketID), \
			"jdd_NetConnect: uiSocketId  is invalid") ;
	jdd_AssertOnFail ((JC_NULL != pcIpAddress), \
		"jdd_NetConnect: pcIpAddress  is NULL") ;
		
	PARAM_INTENTIONALLY_NOT_USED(pvApplicationData) ;

	/*--- Platform Specific Implementation Start ---*/
	mmi_trace (1, "JDD_LOG: NetConnect: IP=%s Port=%d", pcIpAddress, uhPortNo) ;


	jc_memset (&stSockConnect, 0, sizeof (CFW_TCPIP_SOCKET_ADDR)) ;
	stSockConnect.sin_len = 0 ;
	stSockConnect.sin_family = CFW_TCPIP_AF_INET ;
	stSockConnect.sin_port = jdd_NetHtons(uhPortNo) ;
	//stSockConnect.sin_addr.s_addr = *pcIpAddress ;
	stSockConnect.sin_addr.s_addr = CFW_TcpipInetAddr(pcIpAddress) ;

	if (CFW_TcpipSocketConnect ((uiSocketID-1), &stSockConnect, sizeof (CFW_TCPIP_SOCKET_ADDR)) \
			== SOCKET_ERROR)
	{
		rCode = JC_ERR_NET_CONNECT ;
	}
	else
	{
		rCode = JC_ERR_NET_WOULDBLOCK ;
	}
	/*--- Platform Specific Implementation End ---*/

	mmi_trace(1,"JDD_LOG: jdd_NetConnect: Exit[%x]",-rCode) ;
	return (rCode) ;
}

/** 
 * @ingroup		jdd_netapi
 * @brief		This function accepts connection.
 * @param[in]	uiSocketID	Specifies the socket descriptor.
 * @param[out]	ppcAcceptAddress	Pointer to receive the address 
 *				of the connecting entity. 
 * @param[out]	puhPortNo	Pointer to receive the port number
 * @param[out]	puiNewSocketID	Pointer to receive the new socket descriptor
 * @retval		JC_OK on success
 * @retval		JC_ERR_NET_ACCEPT when not successful
 * @retval		JC_ERR_MEMORY_ALLOCATION when memory allocation for 
 *				ppcAcceptAddress fails
 *
 * This function accepts connection and creates a new socket descriptor
 **/

JC_RETCODE jdd_NetAccept (JC_UINT32 uiSocketID, JC_INT8 **ppcAcceptAddress, \
						  JC_UINT16 *puhPortNo, JC_UINT32   *puiNewSocketID)
{
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	CFW_TCPIP_SOCKET_ADDR	stSockAccept ;
	SOCKET					newSock ;
	UINT32					uiNewLen ;
	/*-----------------------------------*/

	mmi_trace(1,"JDD_LOG: jdd_NetAccept: Entry") ;    
    
    jdd_AssertOnFail (( JC_NULL != ppcAcceptAddress ), \
						"jdd_NetAccept: ppcAcceptAddress is NULL") ;
	jdd_AssertOnFail (( JC_NULL != puhPortNo ), \
						"jdd_NetAccept: puhPortNo is NULL") ;
	jdd_AssertOnFail (( JC_NULL != puiNewSocketID ), \
						"jdd_NetAccept: puiNewSocketID is NULL") ;
    

	/*--- Platform Specific Implementation Start ---*/
    
	/* PlatForm Specific Function for Net Accept */

	jc_memset (&stSockAccept, 0, sizeof (CFW_TCPIP_SOCKET_ADDR)) ;

	uiNewLen = sizeof (CFW_TCPIP_SOCKET_ADDR) ;
	newSock = CFW_TcpipSocketAccept ((uiSocketID-1), &stSockAccept, &uiNewLen) ;
	if (INVALID_SOCKET == newSock)
	{
		rCode = JC_ERR_NET_ACCEPT ;
		mmi_trace(1,"JDD_LOG: jdd_NetBind: Returns [%d]", rCode) ;
	}
	else
	{
		*puhPortNo = stSockAccept.sin_port ;
		*puiNewSocketID = newSock ;
		rCode = JC_OK ;
	}

	/*--- Platform Specific Implementation End ---*/		
	mmi_trace(1,"JDD_LOG: jdd_NetAccept: Exit[%d]",rCode) ;	
	return (rCode) ;
}

/** 
 * @ingroup		jdd_netapi
 * @brief		This function places a socket in a state where it is listening
 *				for an incoming connection.
 * @param[in]	uiSocketID	Specifies the socket descriptor.
 * @param[in]	uiTotalConnections	Specifies the maximum number of the
 *				incoming connections that are accepted.
 * @retval		JC_OK on success
 * @retval		JC_ERR_NET_LISTEN when not successful
 * @see			#jdd_NetBind
 */
 
JC_RETCODE jdd_NetListen (JC_UINT32 uiSocketID, JC_UINT32 uiTotalConnections)
{
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	mmi_trace(1,"JDD_LOG: jdd_NetListen: Entry") ;    
    
    jdd_AssertOnFail (( 0 != uiTotalConnections ) && (0 != uiSocketID), \
						"jdd_NetListen: NULL parameters") ;	
    
	/*--- Platform Specific Implementation Start ---*/  

	/* PlatForm Specific Function for Net Listen */
	if(CFW_TcpipSocketListen((uiSocketID-1), uiTotalConnections ) == SOCKET_ERROR)
	 {
		rCode = JC_ERR_NET_LISTEN ;
	 }
	 else
	 {
		rCode = JC_OK ;		
	 }

	/*--- Platform Specific Implementation End ---*/		
	mmi_trace(1,"JDD_LOG: jdd_NetListen: Returns [%d]", rCode) ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_netapi
 * @brief 		This function sends the data to the specified destination.
 * @param[in] 	uiSocketID Specifies the socket descriptor
 * @param[in] 	pucSendBuffer Specifies the byte stream which needs to 
 *				be transmitted.
 * @param[in] 	uiSendBufferLen Specifies the length of the content 
 *				'pucSendBuffer' that needs to be transmitted.
 * @param[in] 	eFlags Currently this flag is not used
 * @param[in] 	pToAddr Specifes the information about the destination 
 *				address
 * @param[in] 	uiToAddrLen Specifies the address of #JC_NET_ADDR.
 * @param[out] 	puiSentBytes Returns the actual number of bytes that has been 
 * 				transmitted from the given byte stream.
 *
 * @retval 		JC_ERR_NET_SEND Specifies that the data could not be sent 
 *				on the socket.
 * @retval 		JC_ERR_NET_SOCKET Specifies that there was an error in the 
 *				socket.
 
 *				as non-blocking and the send operation would blocked.
 *
 * This function shall be used to send a datagram to a specified peer socket 
 * identified by the destination address.
 * @see 		#jdd_NetRecvFrom
 */
JC_RETCODE jdd_NetSendTo (JC_UINT32 uiSocketID, JC_UINT8 *pucSendBuffer, \
						  JC_UINT32 uiSendBufferLen, ESendFlags eFlags, \
						  JC_NET_ADDR *pToAddr, JC_UINT32 uiToAddrLen, \
						  JC_UINT32 *puiSentBytes)
{
    JC_RETCODE  rCode ;
	/*--- Platform Specific Variables ---*/
	CFW_TCPIP_SOCKET_ADDR stDestAddr ;
	//UINT32 uiMaxAvailableBuffer = 0;
	/*-----------------------------------*/
    
	mmi_trace(1,"JDD_LOG: jdd_NetSendTo: Entry") ;
	jdd_AssertOnFail ((0 != uiSocketID ) && \
		(JC_NULL != pucSendBuffer) && \
		(0 != uiSendBufferLen) && \
		(JC_NULL != puiSentBytes) && \
		(JC_NULL != pToAddr), \
		"jdd_NetSendTo: Invalid Parameter") ;
	
	if ((jdd_CFGWSPGetSendPDUSize ()) < (JC_INT32)uiSendBufferLen)
	{		
		rCode = JC_ERR_NET_SEND ;
		mmi_trace(1,"JDD_LOG: SendBuffer size greater than the max \
			buffer size Returns [%d]", rCode) ;
	}
	else
	{
		/*--- Platform Specific Implementation Start ---*/
		jc_memset (&stDestAddr, 0, sizeof (CFW_TCPIP_SOCKET_ADDR)) ;
		stDestAddr.sin_len = 0 ;
		stDestAddr.sin_family = CFW_TCPIP_AF_INET ;
		stDestAddr.sin_port = jdd_NetHtons (pToAddr->hPortNo) ;
		stDestAddr.sin_addr.s_addr = pToAddr->iIP ;
		
		/* Platform Specific Function for Send To */
		*puiSentBytes = CFW_TcpipSocketSendto ((uiSocketID-1), (VOID *)pucSendBuffer, \
			uiSendBufferLen, 0, &stDestAddr, \
			sizeof (CFW_TCPIP_SOCKET_ADDR));
		mmi_trace(1,"JDD_LOG: jdd_NetSendTo SocketID = %d Size = %d", uiSocketID, *puiSentBytes) ;
		if (SOCKET_ERROR != *puiSentBytes)
		{
			rCode = JC_OK ;
#ifdef  JDD_DUMP_SEND_BUFF
			{
				JC_INT8 boundary [80] ;

				jc_memset (boundary, 0, 80) ;
				jc_sprintf (boundary, "\njdd_NetSendTo - Start Sock: %d Size: %d IP: %d Port: %d\n", \
					uiSocketID, *puiSentBytes, stDestAddr.sin_addr.s_addr, pToAddr->hPortNo) ;
				dumpBufferToFile (boundary, jc_strlen (boundary), JDD_RECV_FILE) ;
				
				dumpBufferToFile (pucSendBuffer, *puiSentBytes, JDD_RECV_FILE) ;
				
				jc_memset (boundary, 0, 80) ;
				jc_sprintf (boundary, "\njdd_NetSendTo - End\n\n") ;
				dumpBufferToFile (boundary, jc_strlen (boundary), JDD_RECV_FILE) ;
			}
#endif
		}
		else
		{
			rCode = JC_ERR_NET_SEND ;
		}
		/*--- Platform Specific Implementation End ---*/
	}
	
	mmi_trace(1,"JDD_LOG: jdd_NetSendTo: Exit [%X], port [%d], IP [%d], uiSendBufferLen [%d], *puiSentBytes [%d]",
		-rCode, pToAddr->hPortNo, stDestAddr.sin_addr.s_addr, uiSendBufferLen, *puiSentBytes) ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_netapi
 * @brief 		This function is invoked to send the data on connected socket
 * @param[in] 	uiSocketID Specifies the socket descriptor.
 * @param[in] 	pucSendBuffer Specifies the byte stream which needs to be 
 *				transmitted.
 * @param[in] 	uiSendBufferLen Specifies the length of the content 
 *				(pucSendBuffer) that needs to be transmitted.
 * @param[out] 	puiSentBytes Returns the number of bytes that got transmitted.
 *
 * @retval 		JC_OK	on Success
 * @retval 		JC_ERR_NET_CONNECT Specifies that the connection request to 
 *				peer was not successful.
 * @retval 		JC_ERR_NET_TIMEOUT Specifies that the time has expired before
 *				the network operation took place.
 * @retval 		JC_ERR_NET_SEND	Specifies that the data could not be sent on 
 *				the socket.
 * @retval 		JC_ERR_NET_SOCKET Specifies that there was an error in the 
 *				socket.
 
 *				non-blocking and the send operation would block.
 *
 * If this function returns JC_ERR_NET_WOULDBLOCK, it needs to resend the 
 * data for transmission.
 * @see 		#jdd_NetRecv
 */

JC_RETCODE jdd_NetSend (JC_UINT32 uiSocketID, JC_UINT8 *pucSendBuffer, \
				JC_UINT32 uiSendBufferLen, JC_UINT32 *puiSentBytes)
{
    JC_RETCODE  rCode ;
	/*--- Platform Specific Variables ---*/
	UINT32 uiMaxAvailableBuffer = 0;
	/*-----------------------------------*/
	
	mmi_trace(1,"JDD_LOG: jdd_NetSend: Entry") ;
	jdd_AssertOnFail ((0 != uiSocketID ) && \
		( JC_NULL != pucSendBuffer ) && \
		( 0 != uiSendBufferLen ) && \
		( JC_NULL != puiSentBytes )	, \
		"jdd_NetSend: Invalid Parameters") ;
	
	/*if ((jdd_CFGWSPGetSendPDUSize ()) < (JC_INT32)uiSendBufferLen)
	{	
		rCode = JC_ERR_NET_SEND ;
		mmi_trace(1,"JDD_LOG: jdd_NetSend: SendBuffer size greater than the \
 
	}
	else*/
	{
#ifdef  JDD_DUMP_SEND_BUFF                            
		dumpBufferToFile (pucSendBuffer, uiSendBufferLen, JDD_SEND_FILE) ;
#endif
		mmi_trace(1,"JDD_LOG: jdd_NetSend SocketID = %d  \
			SendBufferLen = %d, SentBytes = %d ",
			uiSocketID,	uiSendBufferLen, *puiSentBytes) ;
		
		/*--- Platform Specific Implementation Start ---*/
		/* Platform Specific Function for Send */
		uiMaxAvailableBuffer = CFW_TcpipAvailableBuffer(uiSocketID-1) ;
		if(uiMaxAvailableBuffer >= uiSendBufferLen)
		{
			*puiSentBytes = CFW_TcpipSocketSend (uiSocketID-1, pucSendBuffer, \
				uiSendBufferLen, 0) ;
			if (SOCKET_ERROR != *puiSentBytes)
			{
				rCode = JC_OK ;
			}
			else
			{
				rCode = JC_ERR_NET_SEND;
				//		rCode = getNetJCFError (CFW_TcpipGetLastError()) ;
			}
		}
		else
		{
			rCode = JC_ERR_NET_WOULDBLOCK ;
			mmi_trace(1,"JDD_LOG:jdd_NetSend Error :uiMaxAvailableBuffer[%d] \
				uiSendBufferLen[%d]",uiMaxAvailableBuffer, uiSendBufferLen) ;
		}
		/*--- Platform Specific Implementation End ---*/
	}
	
	mmi_trace(1,"JDD_LOG: jdd_NetSend: rCode[%x] bytessent[%d] tobesent[%d]", -rCode, *puiSentBytes,uiSendBufferLen) ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_netapi
 * @brief 		This function receives the data from the socket and stores
 *				the source address.
 * @param[in]	uiSocketID Specifies the socket descriptor
 * @param[in] 	eFlags This flag is not used.
 * @param[out]	pucRecvBuffer Specifies the buffer which will received 
 *				the data from the socket.
 * @param[in]  	uiBufferLength Specifies the size of the pucRecvBufer.
 * @param[out] 	puiReceivedBytes Returns the actual number of bytes received
 * @param[out] 	pstSourceAddress Returns the information about the source 
 *				address. This information is available only if this is not 
 *				JC_NULL.
 * @param[in] 	uiSourceAddressLen Specifies the size of the pstSourceAddress.
 * @retval 		JC_OK  on Success.	
 * @retval 		JC_ERR_NET_CONNECT	Connection request to peer was not 
 *				successful
 * @retval 		JC_ERR_NET_RECV		Data could not be received from the 
 *				socket.
 * @retval 		JC_ERR_NET_SOCKET	Error in the socket
 
 *				and the 
 *				receive operation would block
 * This function shall be used only for datagram sockets.
 * @see 		#jdd_NetSendTo
 */
JC_RETCODE jdd_NetRecvFrom (JC_UINT32 uiSocketID, ERecvFlags eFlags, \
							JC_UINT8 *pucRecvBuffer, JC_UINT32 uiBufferLength, \
							JC_UINT32 *puiReceivedBytes, \
							JC_NET_ADDR *pstSourceAddress, \
							JC_UINT32 uiSourceAddressLen)
{
	JC_RETCODE      rCode ;
	/*--- Platform Specific Variables ---*/
	
	INT32  ifromlen = uiSourceAddressLen ;
	CFW_TCPIP_SOCKET_ADDR stTcpIp_Socket_addr = {0,}; 
	/*-----------------------------------*/

	mmi_trace(1,"JDD_LOG: jdd_NetRecvFrom: Entry") ;

	jdd_AssertOnFail ((0 != uiSocketID), \
				"jdd_NetRecvFrom: Invalid Socket ID") ;
	jdd_AssertOnFail ((JC_NULL != pucRecvBuffer), \
				"jdd_NetRecvFrom: pucRecvBuffer is NULL") ;
	jdd_AssertOnFail ((0 != uiBufferLength), \
				"jdd_NetRecvFrom: uiBufferLength is 0") ;
	jdd_AssertOnFail ((JC_NULL != puiReceivedBytes), \
				"jdd_NetRecvFrom: puiReceivedBytes is NULL") ;
	
	PARAM_INTENTIONALLY_NOT_USED(eFlags) ; /*Currently not defined in core*/
	/*--- Platform Specific Implementation Start ---*/
	/* Platform Specific Function for RecvFrom */
	*puiReceivedBytes = CFW_TcpipSocketRecvfrom(uiSocketID-1, pucRecvBuffer, uiBufferLength, 
												0, &stTcpIp_Socket_addr, &ifromlen) ;
	if(SOCKET_ERROR == *puiReceivedBytes)
	{
		rCode = JC_ERR_NET_RECV ;
		*puiReceivedBytes = 0 ;
	}
	else if (*puiReceivedBytes == 0)
	{
		rCode = JC_ERR_NET_WOULDBLOCK ;
	}
	else
	{
		rCode = JC_OK ;
		/*				
					pstSourceAddress->hPortNo = stTcpIp_Socket_addr.sin_port ;
					pstSourceAddress->iIP = stTcpIp_Socket_addr.sin_addr.s_addr ;
		*/

#ifdef  JDD_DUMP_RECV_BUFF
		{
			JC_INT8 boundary [80] ;
			
			rCode = JC_OK ;

			jc_memset (boundary, 0, 80) ;
			jc_sprintf (boundary, "\njdd_NetRecvFrom - Start Sock: %d Size: %d IP: %d Port: %d\n", \
				uiSocketID, *puiReceivedBytes, stTcpIp_Socket_addr.sin_addr.s_addr, stTcpIp_Socket_addr.sin_port) ;
			dumpBufferToFile (boundary, jc_strlen (boundary), JDD_RECV_FILE) ;
			
			dumpBufferToFile (pucRecvBuffer, *puiReceivedBytes, JDD_RECV_FILE) ;
			
			jc_memset (boundary, 0, 80) ;
			jc_sprintf (boundary, "\njdd_NetRecvFrom - End\n\n") ;
			dumpBufferToFile (boundary, jc_strlen (boundary), JDD_RECV_FILE) ;
		}
#endif		
	}	
	/*	
		if ((Platform API succeeds))
		{
#ifdef TRANSPORT_SOCKET_EVENT
		 
			rCode = JC_ERR_WOULDBLOCK ;
#else
			rCode = JC_OK ;
#endif
		}
		else
		{
			rCode = getNetJCFError (<platform variable>) ;
			JDD_LOG_INFO1 ("jdd_NetRecvFrom: Returns [%d]", rCode) ;
		}
	*/


	/*--- Platform Specific Implementation End ---*/
	mmi_trace(1,"JDD_LOG: jdd_NetRecvFrom: Exit [%X], SocketID = %d uiBufferLength [%d], *puiReceivedBytes [%d]",
	-rCode, uiSocketID, uiBufferLength, *puiReceivedBytes) ;
	return (rCode) ;	
}

/**
 * @ingroup		jdd_netapi
 * @brief 		This function is invoked to receive the data on the connected
 *				socket.
 * @param[in] 	uiSocketID Specifies the socket descriptor
 * @param[out] 	pucRecvBuffer Specifies the buffer for the incoming data.
 * @param[in] 	uiBufferSize Specifies the size of the pucRecvBuffer. 
 *				The network layer should not copy more than this value 
 *				to pucRecvBuffer.
 * @param[out] 	puiReceivedBytes Returns the number of bytes that is copied
 *				to the pucRecvBuffer.
 * @param[in] 	eFlags This flag is not used
 * @retval 		JC_OK  on Success.	
 * @retval 		JC_ERR_NET_CONNECT	Connection request to peer was not 
 *				successful
 * @retval 		JC_ERR_NET_RECV		Data could not be received from the socket
 * @retval 		JC_ERR_NET_SOCKET	Error in the socket
 
 *				the receive operation would block
 * @see 		#jdd_NetSend
 */
JC_RETCODE jdd_NetRecv (JC_UINT32 uiSocketID, JC_UINT8 *pucRecvBuffer, \
						JC_UINT32 uiBufferSize,JC_UINT32 *puiReceivedBytes, \
						ERecvFlags eFlags)
{
	JC_RETCODE      rCode ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	mmi_trace(1,"JDD_LOG: jdd_NetRecv: Entry") ;
	
	jdd_AssertOnFail ((0 != uiSocketID), \
				"jdd_NetRecv: Invalid Socket ID") ;
	jdd_AssertOnFail ((JC_NULL != pucRecvBuffer), \
		"jdd_NetRecv: pucRecvBuffer is NULL") ;
	jdd_AssertOnFail ((0 != uiBufferSize), \
		"jdd_NetRecv: uiBufferSize is 0") ;
	jdd_AssertOnFail ((JC_NULL != puiReceivedBytes), \
		"jdd_NetRecv: puiReceivedBytes is NULL") ;

	PARAM_INTENTIONALLY_NOT_USED(eFlags) ;/*Currently not defined in core*/
	
	/*--- Platform Specific Implementation Start ---*/
	/* Platform Specific Function for Recv*/
	*puiReceivedBytes = CFW_TcpipSocketRecv (uiSocketID-1,pucRecvBuffer,uiBufferSize,0);
	if(SOCKET_ERROR == *puiReceivedBytes)
	{
		*puiReceivedBytes = 0 ;
		rCode = JC_ERR_NET_RECV ;
	}
	else if (0 == *puiReceivedBytes) //Ignore this as platform sends this for some reasons
	{
		rCode = JC_ERR_NET_WOULDBLOCK ;
	}
	else
	{
		rCode = JC_OK ;
	}


	/*	
		if ((Platform API succeeds))
		{
#ifdef TRANSPORT_SOCKET_EVENT
		 
			rCode = JC_ERR_WOULDBLOCK ;
#else
			rCode = JC_OK ;
#endif
		}
		else
		{
			rCode = getNetJCFError (<platform variable>) ;
		}
	*/
#ifdef  JDD_DUMP_RECV_BUFF
	if(*puiReceivedBytes)
	{
		JC_INT8 boundary [50] ;

		jc_memset (boundary, 0, 50) ;
		jc_sprintf (boundary, "\njdd_NetRecv - Start Sock: %d  Size: %d\n", uiSocketID, *puiReceivedBytes) ;
		dumpBufferToFile (boundary, jc_strlen (boundary), JDD_RECV_FILE) ;
		
		dumpBufferToFile (pucRecvBuffer, *puiReceivedBytes, JDD_RECV_FILE) ;
		
		jc_memset (boundary, 0, 50) ;
		jc_sprintf (boundary, "\njdd_NetRecv - End\n\n") ;
		dumpBufferToFile (boundary, jc_strlen (boundary), JDD_RECV_FILE) ;
	}
#endif
	
	mmi_trace(1,"JDD_LOG: jdd_NetRecv: Returns rCode[%x] retval[%d]", -rCode,*puiReceivedBytes) ;

	/*--- Platform Specific Implementation End ---*/		
	return (rCode) ;
}

/**
 * @ingroup		jdd_netapi
 * @brief 		This function gets the status of the socket waiting to perform 
 *				send/receive operations.
 * @param[in] 	uiSocketID Specifies the socket descriptor
 * @param[in] 	eSocketDescriptor Specifies the type of the socket descriptor.
 * @param[in] 	uiTimeInMilliSeconds Specifies the time period in 
 *				milliseconds to wait.
 * @retval		JC_OK on Success
 * @retval 		JC_ERR_NET_SOCKET_NOT_READY Specifies that the
 *				socket is not ready for the specified descriptor
 * @retval 		JC_ERR_NET_SOCKET Specifies that there was an error 
 *				in the socket
 */

JC_RETCODE jdd_NetSelect (JC_UINT32 uiSocketID, ESocketDesType \
			eSocketDescriptor, JC_UINT32 uiTimeInMilliSeconds)
{
	JC_RETCODE rCode = JC_ERR_NET_SOCKET_NOT_READY ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	mmi_trace(1,"JDD_LOG: jdd_NetSelect: Entry") ;

	jdd_AssertOnFail ((0 != uiSocketID), \
				"jdd_NetSelect: Invalid Socket ID") ;

	/*--- Platform Specific Implementation Start ---*/

	/* PlatForm Specific Function for NetSelect */
	/* 
	 * While invoking platform API for NetSelect, it might be necessary to 
	 * invoke it in a different way based on eSocketDescriptorType. For e.g:
	 * If eSocketDescriptor is E_READ_SOCKET:
	 * <platform_netSelect> (uiSocketID, NULL, uiTimeInMilliSeconds)
	 * If eSocketDescriptor is E_WRITE_SOCKET:
	 * <platform_netSelect> (uiSocketID, uiTimeInMilliSeconds, NULL)
	 */
	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = getNetJCFError (<platformErrValue>) ;
		JDD_LOG_INFO1 ("jdd_NetSelect: Returns [%d]", rCode) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/
	if (E_READ_SOCKET == eSocketDescriptor)
    {
        rCode = JC_OK;
    }
    else
    {   
        rCode = JC_ERR_NET_SOCKET_NOT_READY ;
    }

	mmi_trace(1,"JDD_LOG: jdd_NetSelect: Exit [%x]",-rCode) ;
	return (rCode) ;
}						  

/**
 * @ingroup		jdd_netapi
 * @brief		This function is invoked to get the local name for the socket.
 * @param [in] 	uiSocketID Specifies the socket descriptor.
 * @param [in] 	pcIpAddress Specifies the IP Address of the socket .
 * @param [in] 	puhPortNo Specifies the destination port number.
 * @retval     	JC_OK on sucess
 * @retval     	JC_ERR_NET_SOCKET on error.
 **/

JC_RETCODE jdd_NetGetSockName (JC_UINT32 uiSocketID, JC_INT8 *pcIpAddress, \
							   JC_UINT16 *puhPortNo)
{
	JC_RETCODE rCode = JC_ERR_NOT_IMPLEMENTED ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	mmi_trace(1,"JDD_LOG: jdd_NetGetSockName: Entry") ;
	jdd_AssertOnFail ((0 != uiSocketID), \
		"jdd_NetGetSockName: uiSocketId  is invalid") ;
	jdd_AssertOnFail ((JC_NULL != pcIpAddress), \
		"jdd_NetGetSockName: pcIpAddress  is NULL") ;
	jdd_AssertOnFail ((JC_NULL != puhPortNo), \
		"jdd_NetGetSockName: puhPortNo  is NULL") ;

	/*--- Platform Specific Implementation Start ---*/

	/* Platform Specific Function for GetSockName */
	/*
	if (Platform API Succeeds)
	{
		rCode = JC_OK ;
	}
	else
	{
	   rCode = getNetJCFError (<platformErrValue>) ;
	   JDD_LOG_INFO1 ("jdd_NetGetSockName: Returns [%d]", rCode) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/

	mmi_trace(1,"JDD_LOG: jdd_NetGetSockName: Exit [%d]",rCode) ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_netapi
 * @brief 		This function retrieves the host IP address from the host name.
 * @param[in] 	uiConnID	Specifies the data connection ID
 * @param[in] 	pcHostName Specifies the host name, where IP address 
 *				needs to be resolved.
 * @param[in] 	pApplicationData Specifies the application data
 * @param[out] 	ppucIpAddresses receives the list of resolved IP addresses
 * @retval  	JC_OK on success
 * @retval  	JC_ERR_NET_DNS_RESOLVE on failure
 * @retval		JC_ERR_INVALID_PARAMETER on invalid inputs.
 */
 
 JC_RETCODE jdd_NetGetHostByName ( JC_UINT32 uiConnID, \
						const JC_INT8 *pcHostName, void *pApplicationData, \
						JC_UINT8 **ppucIpAddresses )
 {
	JC_RETCODE      rCode ;
	struct ip_addr stIPAddress ;
	UINT32 rVal ;
	INT8 *pIPAddress = NULL ;	
#ifndef MMI_ON_WIN32 /* zhuoxz,2009-4-28 */
	in_addr stInAddr ={0,} ;
#else
  	IN_ADDR stInAddr ={0,} ;
#endif

//#ifdef __MMI_MULTI_SIM__
	JC_USER_INFO_CONNID * pstConIdInfo = (JC_USER_INFO_CONNID *)uiConnID;
//#endif

	/*-----------------------------------*/

	mmi_trace(1,"JDD_LOG: jdd_NetGetHostByName: Entry") ;
	jdd_AssertOnFail ((JC_NULL != pcHostName), \
		            "jdd_NetGetHostByName: pcHostName is NULL") ;
	jdd_AssertOnFail ((JC_NULL != ppucIpAddresses), \
		            "jdd_NetGetHostByName: ppucIpAddresses is NULL") ;

	/*--- Platform Specific Implementation Start ---*/
	mmi_trace (1, "JDD_LOG: GetHost called for %s.", pcHostName) ;
#ifdef __ADP_MULTI_SIM__
	mmi_trace(1,"JDD_LOG: CFW_Gethostbyname uiConnID[%d] Cid value [%d] SimID [%d]",uiConnID, pstConIdInfo->nCid, pstConIdInfo->nSimID );
	rVal = CFW_Gethostbyname((char*)pcHostName, &stIPAddress,pstConIdInfo->nCid,pstConIdInfo->nSimID) ;
#else
	rVal = CFW_Gethostbyname((char*)pcHostName, &stIPAddress,pstConIdInfo->nCid) ;
#endif
	mmi_trace (1, "JDD_LOG: GetHost called for %s. rVal: %x", pcHostName, rVal) ;
	if(RESOLV_COMPLETE == rVal)
	{	
		stInAddr.s_addr = stIPAddress.addr ;
#ifndef MMI_ON_WIN32 /* zhuoxz,2009-4-30 */
		pIPAddress = inet_ntoa(stInAddr);
#else
    	pIPAddress = inet_ntoa_win32(stInAddr);
#endif /* #ifndef MMI_ON_WIN32 */

		*ppucIpAddresses = jdi_CUtilsStrDuplicate(JC_NULL,pIPAddress) ;
		if (JC_NULL == *ppucIpAddresses)
		{
			rCode = JC_ERR_MEMORY_ALLOCATION ;
		}
		else
		{
			rCode = JC_OK ;
		}
	}
	else if(RESOLV_QUERY_QUEUED == rVal)
	{
		rCode = JC_ERR_NET_DNS_WOULDBLOCK ;
		 CFW_SetTCPIPCallBackEx(jdd_SocketEventcb,4);
	}
	else
	{
		rCode = JC_ERR_NET_DNS_RESOLVE ;
	}
	
	/* Query from native DNS cache if it is present */
	/*
	if(DNS resolved from Cache)
	{
		rCode = JC_OK ;
	}
	else	
	{
		<Platform Specific Function for GetHostByName >
		
		<Mapping should be done to EventType for event based platforms> 
		
		if ((Platform API succeeds))
		{
#ifdef TRANSPORT_SOCKET_EVENT
		 
			rCode = JC_ERR_WOULDBLOCK ;
#else
			rCode = JC_OK ;
#endif
		
		}
		else
		{
			rCode = getNetJCFError (<platformErrValue>) ;
			JDD_LOG_INFO1 ("jdd_NetGetHostByName: Returns [%d]", rCode) ;
		}
	}
	*/
	
	/* pcHostName might result in more than one address (aliases) 
	 * returned by the platform. ppucIpAddresses should be assigned all 
	 * the aliases.
	 */
	
	/*--- Platform Specific Implementation End ---*/

	mmi_trace(1,"JDD_LOG: jdd_NetGetHostByName: Exit [%d]",rCode) ;	
	return (rCode) ;
 }

/**
 * @ingroup		jdd_netapi
 * @brief		This function closes an existing socket.
 * @param[in]	uiSocketID Specifies the socket descriptor to be closed
 * @retval		JC_OK on success
 * @retval		JC_ERR_NET_SOCKET when not successful
 *
 * This function is used to release the socket descriptor so that  
 * further references to this socket will fail.
 * @see			#jdd_NetSocket
 */
JC_RETCODE jdd_NetClose (JC_UINT32 uiSocketID)
{
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	mmi_trace(1,"JDD_LOG: jdd_NetClose: Entry") ;    
    
    jdd_AssertOnFail (( 0 != uiSocketID ), \
						"jdd_NetClose: Invalid uiSocketID") ;	
    

	/*--- Platform Specific Implementation Start ---*/  
	mmi_trace (1, "JDD_LOG: NetClose: %d", uiSocketID) ;
	if(CFW_TcpipSocketClose (uiSocketID-1)== SOCKET_ERROR)
	{
		rCode = JC_ERR_NET_SOCKET ;
	}
	else
	{
		rCode = JC_OK ;
	}

	/* PlatForm Specific Function for Net Close */
	/*		
	if (Platform API Succeeds)
	 {
			rCode = JC_OK ;		
	 }
	 else
	 {
		   rCode = getNetJCFError (<platformErrValue>) ;
		   JDD_LOG_INFO1 ("jdd_NetClose: Returns [%d]", rCode) ;
	 }
	*/

	/*--- Platform Specific Implementation End ---*/		
	mmi_trace(1,"JDD_LOG: jdd_NetClose: Exit [%d]", rCode) ;	
	return (rCode) ;
}

/**
 * @ingroup		jdd_netapi
 * @brief 		This function sets the socket option.
 * @param[in]	uiSocketID Specifies the socket descriptor.
 * @param[in] 	eSocketLevel Specifies the socket level.
 * @param[in] 	eOptName Specifies the name of the option which needs to 
 *				be set.
 * @param[in] 	pvOptValue Specifies the value which needs to be set.
 * @param[in] 	uiOptLen Specifies the size of the pvOptValue.
 * @retval 		JC_OK  on Success.	
 * @retval 		JC_ERR_NET_SET_SOCKOPT On Failure.
 * 
 * Specifies the different socket options for different socket level.
 */
JC_RETCODE jdd_NetSetSockOpt (JC_UINT32 uiSocketID, \
							  ESocketLevel eSocketLevel, \
							  EOptionName  eOptName, void *pvOptValue, \
							  JC_UINT32 uiOptLen)
{
	JC_RETCODE      rCode = JC_OK ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	
	mmi_trace(1,"JDD_LOG: jdd_NetSetSockOpt: Entry") ;
	jdd_AssertOnFail (( 0 != uiSocketID ), \
						"jdd_NetSetSockOpt: Invalid uiSocketID") ;	
        	
	/*--- Platform Specific Implementation Start ---*/
	switch( eOptName )
	{
		/*Option to be passed to platform function*/
		case E_SO_DONTLINGER: 
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_LINGER:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_RCVBUF:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_REUSE_ADDR:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_SENDBUF:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_TCPNODELAY:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_SENDTIMEOUT:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_NONBLOCKING:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_RECV_TIMEOUT:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_BROADCAST:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_IP_ADD_MEMBERSHIP:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_IP_DROP_MEMBERSHIP:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_IP_MULTICAST_TTL:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_IP_HDRINCL:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_TCP_NODELAY:  
		/* Map to Platform Specific Option */
		break ;
		
		default :
			rCode = JC_ERR_NET_SET_SOCKOPT ;
			mmi_trace(1,"JDD_LOG: jdd_NetSetSockOpt: Returns [%d]", rCode) ;
		break ;
	}
	
	if(JC_ERR_NET_SET_SOCKOPT != rCode)
	{
		switch(eSocketLevel)
		{
			/*Option to be passed to platform function*/
			case E_SOL_SOCKET:  
			/* Map to Platform Specific Level */
			break ;
			
			case E_IPPROTO_TCP:   
			/* Map to Platform Specific Level */
			break ;
			
			case E_IPPROTO_IP:   
			/* Map to Platform Specific Level */
			break ;
			
			default:
				rCode = JC_ERR_NET_SET_SOCKOPT ;
				mmi_trace(1,"JDD_LOG: jdd_NetSetSockOpt: Returns [%d]", rCode) ;
			break ;
		}
		/*Platform specific API for Set Sock Opt*/
		/*		
		if (Platform API Succeeds)
		 {
			rCode = JC_OK ;		
		 }
		 else
		 {
		   rCode = getNetJCFError (<platformErrValue>) ;
		   JDD_LOG_INFO1 ("jdd_NetSetSockOpt: Returns [%d]", rCode) ;
		 }
		*/
	}
	/*--- Platform Specific Implementation End ---*/	
	mmi_trace(1,"JDD_LOG: jdd_NetSetSockOpt: Exit [%d]", rCode) ;
	return (rCode) ;
}


/**
 * @ingroup			jdd_netapi
 * @brief 			This function gets the socket option. 
 * @param[in] 		uiSocketID Specifies the socket descriptor.
 * @param[in] 		eSocketLevel Specifies the socket socket level.
 * @param[in] 		eOptName Name of the option to get.
 * @param[out] 		pvOptValue Option value which needs to be retrieved.
 * @param[in,out] 	puiOptLen Size of the pvOptValue.Returns the actual 
 * 					size that is used.
 * @retval 			JC_OK  on Success.	
 * @retval 			JC_ERR_NET_GET_SOCKOPT On Failure.
 **/
 
JC_RETCODE jdd_NetGetSockOpt (JC_UINT32 uiSocketID, ESocketLevel eSocketLevel, \
							  EOptionName  eOptName, void *pvOptValue, \
							  JC_UINT32 *puiOptLen) 
{
	JC_RETCODE      rCode = JC_ERR_NOT_IMPLEMENTED ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	
	mmi_trace(1,"JDD_LOG: jdd_NetGetSockOpt: Entry") ;
	jdd_AssertOnFail ((0 != uiSocketID ), \
		"jdd_NetGetSockOpt: Invalid Socket ID") ;
	jdd_AssertOnFail ((JC_NULL != pvOptValue), \
		"jdd_NetGetSockOpt: pvOptValue is NULL") ;
	jdd_AssertOnFail ((JC_NULL != puiOptLen), \
		"jdd_NetGetSockOpt: puiOptLen is NULL") ;
	
	/*--- Platform Specific Implementation Start ---*/
	switch( eOptName )
	{
		/*Option to be passed to platform function*/
		case E_SO_DONTLINGER: 
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_LINGER:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_RCVBUF:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_REUSE_ADDR:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_SENDBUF:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_TCPNODELAY:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_SENDTIMEOUT:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_NONBLOCKING:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_RECV_TIMEOUT:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_BROADCAST:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_IP_ADD_MEMBERSHIP:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_IP_DROP_MEMBERSHIP:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_IP_MULTICAST_TTL:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_IP_HDRINCL:  
		/* Map to Platform Specific Option */
		break ;
		
		case E_SO_TCP_NODELAY:  
		/* Map to Platform Specific Option */
		break ;
		
		default :
			rCode = JC_ERR_NET_SET_SOCKOPT ;
			mmi_trace(1,"JDD_LOG: jdd_NetGetSockOpt: Returns [%d]", rCode) ;
		break ;
	}
	
	if(JC_ERR_NET_SET_SOCKOPT != rCode)
	{
		switch(eSocketLevel)
		{
			/*Option to be passed to platform function*/
			case E_SOL_SOCKET:  
			/* Map to Platform Specific Level */
			break ;
			
			case E_IPPROTO_TCP:   
			/* Map to Platform Specific Level */
			break ;
			
			case E_IPPROTO_IP:   
			/* Map to Platform Specific Level */
			break ;
			
			default:
				rCode = JC_ERR_NET_SET_SOCKOPT ;
				mmi_trace(1,"JDD_LOG: jdd_NetGetSockOpt: Returns [%d]", rCode) ;
			break ;
		}
		/*Platform specific API for Get Sock Opt*/
		/*		
		if (Platform API Succeeds)
		 {
			rCode = JC_OK ;		
		 }
		 else
		 {
		   rCode = getNetJCFError (<platformErrValue>) ;
		   JDD_LOG_INFO1 ("jdd_NetGetSockOpt: Returns [%d]", rCode) ;
		 }
		*/
		
	}	
	/*--- Platform Specific Implementation End ---*/	
	mmi_trace(1,"JDD_LOG: jdd_NetGetSockOpt: Exit [%d]", rCode) ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_netapi
 * @brief		This function returns the binary representation of the 
 *				Internet address for the given "xxx.xxx.xxx.xxx" IP format.
 * @param[in] 	pcIPAddress Specifies the IP Address
 * @param[in] 	uiConnID Specifies the data connection identifier
 * @param[out]	puiIP contains the binary representation of Internet address.
 * @retval     	E_TRUE on sucess.
 * @retval     	E_FALSE on failure.
 **/
JC_BOOLEAN jdd_NetInetAddress (const JC_INT8 *pcIPAddress, \
							    JC_UINT32 uiConnID, JC_UINT32 *puiIP) 
{
	JC_BOOLEAN bRetVal = E_FALSE ;
	/* Variables Used for Workaround */
	/*
	  JC_UINT8 *pucAddrbyte ;
	  JC_UINT32 uiIndex[4] ;
	 */
	 
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	
	jdd_AssertOnFail ((JC_NULL != pcIPAddress) , \
		"jdd_NetInetAddress: pcIPAddress is NULL") ;
	PARAM_INTENTIONALLY_NOT_USED (uiConnID) ;
	
	/*--- Platform Specific Implementation Start ---*/
	*puiIP = CFW_TcpipInetAddr(pcIPAddress) ;
	if(0xFFFFFFFF != *puiIP)
	{
		bRetVal = E_TRUE ;
	}


	/* Platform Specific Function for net_int address */
	/* 
	if (Platform API Succeeds)
	{
		bRetVal = E_TRUE ;
	}
	else
	{
		bRetVal = E_FALSE ;
		JDD_LOG_INFO1 ("jdd_NetInetAddress: Returns [%d]", bRetVal) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/

	/*---If there is no platform call available, use the below workaround---*/
	/*---Start of workaround---*/
	/*
	
	if ((4 != jc_sscanf(pcIPAddress, "%u.%u.%u.%u", &uiIndex[0], &uiIndex[1], \
		&uiIndex[2], &uiIndex[3])) 	&& \
		(0x0ff < (uiIndex[0]  | uiIndex[1]  | uiIndex[2]  | uiIndex[3] )))
	{
		bRetVal = E_FALSE ;
		JDD_LOG_INFO1 ("jdd_NetInetAddress: Returns [%d]", bRetVal) ;
	}
	else
	{
		pucAddrbyte = (JC_UINT8 *) puiIP ;
		pucAddrbyte[0] = (JC_UINT8) uiIndex[0]  ;
		pucAddrbyte[1] = (JC_UINT8) uiIndex[1]  ;
		pucAddrbyte[2] = (JC_UINT8) uiIndex[2]  ;
		pucAddrbyte[3] = (JC_UINT8) uiIndex[3]  ;
		bRetVal = E_TRUE ;
		JDD_LOG_INFO1 ("jdd_NetInetAddress() returning %u", *puiIP) ;
		JDD_LOG_INFO1 ("jdd_NetInetAddress: Returns [%d]", bRetVal) ;
	}
	*/
	/*This returns "puiIP" on big-endian format*/
	/*---End of workaround---*/

	mmi_trace(1,"JDD_LOG: jdd_NetInetAddress: Exit [%d], pcIPAddress [%s], *puiIP [%d]",
	bRetVal, pcIPAddress, *puiIP) ;
	return (bRetVal) ;
}

/**
 * @ingroup		jdd_netapi
 * @brief		This function converts an unsigned short (16-bit) integer from 
 *				host byte order to internet network byte order	   
 * @param[in]	uhHostByteOrd host byte order
 * @retval		Internet network byte order
 *
 * The Internet network requires address and  port  reference
 * data  in  network byte  order. Use the htons() function to
 * convert address and port short  integers  from  host  byte
 * order to Internet network byte order. On machines which have a byte order 
 * which is the same as the network order, routines are defined as null macros
 */
JC_UINT16 jdd_NetHtons (JC_UINT16 uhHostByteOrd)
{
/*
	JC_UINT8 *pucNetByteOrd = (JC_UINT8 *) &uhHostByteOrd ;
*/
	JC_UINT16 uhNetByteOrd = 0 ;
	
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	
	mmi_trace(1,"JDD_LOG: jdd_NetHtons: Entry") ;
	/*--- Platform Specific Implementation Start ---*/
	uhNetByteOrd = htons(uhHostByteOrd);

	/* Platform API specific to HtoNs and assign to uhNetByteOrd */
	
	/*--- Platform Specific Implementation End ---*/

	/*If platform does not convert port no to short, then use the following
	work-around*/
	/*if (platform is big endian)
	{
		uhNetByteOrd = uhHostByteOrd ;
	}
	else
	{
		uhNetByteOrd = ((JC_UINT16)(pucNetByteOrd [0] << 8 | \
								pucNetByteOrd [1])) ;
	}
	*/
	mmi_trace(1,"JDD_LOG: jdd_NetHtons: Exit ") ;
	return (uhNetByteOrd) ;
}

/**
 * @ingroup		jdd_netapi
 * @brief		This function converts an unsigned short (16-bit) integer from 
 *				internet network byte order	to host byte order
 * @param[in]	uhNetByteOrd network byte order
 * @retval		Host byte order
 * 
 * These routines are most often used in conjunction with Internet
 * addresses and ports as returned by gethostbyname. On machines which have 
 * a byte order which is the same as the network order, routines are defined
 * as null macros
 */
 
JC_UINT16 jdd_NetNtohs (JC_UINT16 uhNetByteOrd)
{
/*
	JC_UINT8 *pucHostByteOrd = (JC_UINT8 *) &uhNetByteOrd ;
*/
	JC_UINT16 uhHostByteOrd = 0 ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	
	mmi_trace(1,"JDD_LOG: jdd_NetNtohs: Entry") ;
	/*--- Platform Specific Implementation Start ---*/
	uhHostByteOrd = ntohs(uhNetByteOrd);

	/* Platform API specific to NtoHs and assign to uhHostByteOrd */
	
	/*--- Platform Specific Implementation End ---*/

	/*
	 * If platform does not convert port no to host order, then use the 
	 * following work-around
	 */
	/*if (platform is big endian)
	{
		uhHostByteOrd = uhNetByteOrd ;
	}
	else
	{
		uhHostByteOrd = ((JC_UINT16)(pucHostByteOrd[0] << 8 | \
								pucHostByteOrd[1])) ;
	}
	*/
	mmi_trace(1,"JDD_LOG: jdd_NetNtohs: Exit") ;
	return (uhHostByteOrd) ;
}



/**
 * @ingroup		jdd_netapi
 * @brief		This function converts an unsigned long (32-bit) integer from 
 *				host byte order to internet network byte order	   
 * @param[in]	uiHostByteOrd host byte order
 * @retval		Internet network byte order
 *
 * These routines are often used in conjunction with Internet addresses
 * and ports as returned by gethostbyname. On machines which have a byte order
 * which is the same as the network order, routines are defined as null macros
 */
 
JC_UINT32 jdd_NetHtonl (JC_UINT32 uiHostByteOrd)
{
/*
	JC_UINT8 *pucNetByteOrd = (JC_UINT8 *) &uiHostByteOrd ;
*/
	JC_UINT32 uiNetByteOrd = 0 ;
	
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	
	mmi_trace(1,"JDD_LOG: jdd_NetHtonl: Entry") ;
	/*--- Platform Specific Implementation Start ---*/
	uiNetByteOrd = htonl(uiHostByteOrd);

	/* Platform API specific to HtoNl and assign to uiNetByteOrd */
	
	/*--- Platform Specific Implementation End ---*/

	/*If platform does not convert input to Network long, 
	* then use the following work-around */
	/*if (platform is big endian)
	{
		uiNetByteOrd = uiHostByteOrd ;
	}
	else
	{
		uiNetByteOrd = ((JC_UINT32)(pucNetByteOrd[0] << 24 | \
						pucNetByteOrd[1] << 16 | \
						pucNetByteOrd[2] << 8 | \
						pucNetByteOrd[3])) ;
	}
	*/
	mmi_trace(1,"JDD_LOG: jdd_NetHtonl: Exit") ;

	return (uiNetByteOrd) ;
}



/**
 * @ingroup		jdd_netapi
 * @brief		This function converts an unsigned long (32-bit) integer from 
 *				internet network byte order to host byte order
 * @param[in]	uiNetByteOrd network byte order
 * @retval		Host byte order
 * 
 * On machines which have a byte order which is the same as the network 
 * order, routines are defined as null macros. These routines are most 
 * often used in conjunction with Internet addresses and ports as returned 
 * by gethostbyname
 */
 
JC_UINT32 jdd_NetNtohl (JC_UINT32 uiNetByteOrd)
{
/*
	JC_UINT8 *pucHostByteOrd = (JC_UINT8 *) &uiNetByteOrd ;
*/
	JC_UINT32 uiHostByteOrd = 0 ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	
	mmi_trace(1,"JDD_LOG: jdd_NetNtohl: Entry") ;
	/*--- Platform Specific Implementation Start ---*/
	uiHostByteOrd = ntohl(uiNetByteOrd);

	/* Platform API specific to NtoHl and assign to uiHostByteOrd */
	
	/*--- Platform Specific Implementation End ---*/

	/* 
	 * If platform does not convert input to host order, then 
	 * use the following work-around
	 */
	/*if (platform is big endian)
	{
		uiHostByteOrd = uiNetByteOrd ;
	}
	else
	{
		uiHostByteOrd = ((JC_UINT32)(pucHostByteOrd[0] << 24 |  \
						pucHostByteOrd[1] << 16 | \
						pucHostByteOrd[2] << 8 | \
						pucHostByteOrd[3])) ;
	}
	*/
	mmi_trace(1,"JDD_LOG: jdd_NetNtohl: Exit") ;

	return (uiHostByteOrd) ;
}


/**
 * @ingroup		jdd_netapi
 * @brief		This function converts the internet address to ASCII string
 *				representing the address in '.' notation
 * @param[in]	uiAddr Value to be converted to ASCII format
 * @param[out]	puiSize Size of ASCII string
 * @param[out]	pcIPAddress ASCII string representing internet address
 * @return      JC_OK on success
 */
 
JC_RETCODE jdd_NetNtoA (JC_UINT32 uiAddr, JC_UINT32 *puiSize, \
						JC_INT8 *pcIPAddress)
{
	JC_RETCODE rCode ;
/*
	JC_UINT8	*pucAscii = (JC_UINT8 *) &uiAddr ;
*/
	/*--- Platform Specific Variables ---*/
	INT8 *pIPAddress;
#ifndef MMI_ON_WIN32 /* zhuoxz,2009-4-28 */
	in_addr stInAddr ;
#else
  IN_ADDR stInAddr;
#endif

	/*-----------------------------------*/
	
	mmi_trace(1,"JDD_LOG: jdd_NetNtoA: Entry") ;
	jdd_AssertOnFail ((JC_NULL != pcIPAddress && JC_NULL != puiSize), \
		            "jdd_NetNtoA: jdd_NetNtoA parameters are NULL") ;

	/*--- Platform Specific Implementation Start ---*/
	stInAddr.s_addr =  uiAddr ;
#ifndef MMI_ON_WIN32 /* zhuoxz,2009-4-30 */
	pIPAddress = inet_ntoa(stInAddr);
#else
  pIPAddress = inet_ntoa_win32(stInAddr);
#endif /* #ifndef MMI_ON_WIN32 */
	*puiSize = jc_strlen(pIPAddress) ;
	if(*puiSize)
	{
		jc_strncpy(pcIPAddress, pIPAddress, *puiSize);
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_INVALID_PARAMETER ;
	}

	/* Platform API specific to NtoA */
	
	/*--- Platform Specific Implementation End ---*/

	/* If platform does not convert input to ASCII, then use the following
	work-around*/
	/*
	jc_sprintf (pcIPAddress, "%d.%d.%d.%d", pucAscii [0] & 0xff, \
					pucAscii [1] & 0xff,\
					pucAscii [2] & 0xff, \
					pucAscii [3] & 0xff);
					
	*puiSize = jc_strlen (pcIPAddress) ;
	*/
	mmi_trace(1,"JDD_LOG: jdd_NetNtoA: Exit[%d]", rCode) ;
	return rCode ;
}
/*END OF FILE*/
#endif
