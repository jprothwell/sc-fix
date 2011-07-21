/*************************************************************************
*	
*	文件：Network.c
* 描述：创建网络	
* 日期：2005-8-30   	
* 作者：James    	    	
* 
*	Copyright (c) 2005 WEP All Rights Reserved.
************************************************************************/
#define _DEV_NET_	// module define, header include must after this
#ifdef MMI_ON_WIN32
//#include "windows.h"
#endif 

#include "devcfg.h"
#include "devdef.h"
#include "interface.h"
#include "devdecl.h"
#include "devnet.h"
#include "utility.h"
#include "extdecl.h"

#include "abm_soc_enums.h"	//wyl20100408

#include "dual_sim.h"
//#include "DebugInitDef.h"
//#include "stack_common.h"
//#include "soc_sap.h"
#ifdef _KK_SUPPORT_ 

//////////////////////////////////////////////////////////////////////////
#define NET_IS_INVALID(s)		((s) < 0)
#define WEP_CMWAP_ACCOUNT_ID 10
#define WEP_CMNET_ACCOUNT_ID 18      // 11->18 BN10011

#define MAX_READ_BUFFER_SIZE 20*1024


static NET * gNet = NULL;
static byte_t * gReadBuffer = NULL;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
static void NET_ConnectCBSet(INET *pINet, PFNNETCALLBACK fn, void * pUserData);
static void NET_SendCBSet(INET * pINet, PFNNETCALLBACK fn, void * pUserData);
static void NET_ReadCBSet(INET * pINet, PFNNETCALLBACK fn, void * pUserData);
static void NET_Connect(INET *pINet, const char_t * szURL, int32_t port);
static void NET_Send(INET * pINet, byte_t * pBuf, uint32_t nDataLen);
static void NET_Read(INET * pINet);
static void NET_Cancel(INET * pINet);
static void NET_Close(INET * pINet);

static void CSD_SocketEventHandler(void* inMsg);
static void CSD_DnsEventHandler(void* inMsg);
#ifdef SUPPORT_DUAL_SIM
#if (KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223) //bruce100113
#define kk_always_ask_encode_data_account_id(id, a, b, c) 		cbm_encode_data_account_id(id, a-1, b+1, c)
#else 
  #if(KK_MTKXXXX_SWSUBVER == KK_MTKXXXX_V07B12)
  #define kk_always_ask_encode_data_account_id(id, a, b, c)                 always_ask_encode_data_account_id(id, a, b, c) 
  #else
#define kk_always_ask_encode_data_account_id(id, a, b, c) 		cbm_encode_data_account_id(id, a-1, b, c)
  #endif/*KK_MTKXXXX_SWSUBVER == KK_MTKXXXX_V07B12*/
#endif /* KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223 */
#endif /* SUPPORT_DUAL_SIM */

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
INET * NET_Create(WEPAppData *pWep)
{
	NET * pNet;

	pNet = (NET *)DEV_Malloc(MALLOC_MODULE_NET, sizeof(NET));

	if (pNet != NULL)
	{
		gNet = pNet;

		DEV_MemSet(pNet, 0x00, sizeof(NET));

		pNet->Vtbl.Connect = NET_Connect;
		pNet->Vtbl.Read		 = NET_Read;
		pNet->Vtbl.Cancel	 = NET_Cancel;
		pNet->Vtbl.Send		 = NET_Send;
		pNet->Vtbl.Close	 = NET_Close;

		pNet->Vtbl.ConnectCBSet = NET_ConnectCBSet;
		pNet->Vtbl.ReadCBSet    = NET_ReadCBSet;
		pNet->Vtbl.SendCBSet    = NET_SendCBSet;

		pNet->pWep  = pWep;

		pNet->sockfd = SOC_INVALID_SOCKET;
		pNet->status = NET_STATUS_IDLE;
		pNet->socketConnected = FALSE;

	}

	gReadBuffer = (byte_t *)DEV_Malloc(MALLOC_MODULE_NET, MAX_READ_BUFFER_SIZE);
	if (gReadBuffer == NULL)
	{
		WEP_AppException(pWep, EXC_MEMORY_ALLOC_FAILED);
	}
	COS_PageProtectDisable(COS_PAGE_NUM_4);
         COS_PageProtectSetup(COS_PAGE_NUM_4, COS_PAGE_WRITE_TRIGGER, (UINT32)pNet, (UINT32)pNet+32);
	COS_PageProtectEnable(COS_PAGE_NUM_4);
	return (INET *)pNet;
}//end NET_Create()

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void NET_Destroy(INET * pINet)
{
//	NET * pNet = (NET *)pINet;

	pINet->Close(pINet);
	COS_PageProtectDisable(COS_PAGE_NUM_4);
	DEV_Free(pINet);
	DEV_Free(gReadBuffer);
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
static void NET_ConnectCBSet(INET * pINet, PFNNETCALLBACK fn, void * pUserData)
{
	NET * pNet = (NET *)pINet;

	pNet->connectCB = fn;
	pNet->pConnectCBData = pUserData;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
static void NET_ReadCBSet(INET *pINet, PFNNETCALLBACK fn, void * pUserData)
{
	NET * pNet = (NET *)pINet;

	pNet->readCB = fn;
	pNet->pReadCBData = pUserData;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
static void NET_SendCBSet(INET *pINet, PFNNETCALLBACK fn, void * pUserData)
{
	NET * pNet = (NET *)pINet;

	pNet->sendCB = fn;
	pNet->pSendCBData = pUserData;
}


extern kal_bool soc_ip_check(kal_char *asci_addr,
                      kal_uint8 *ip_addr,
                      kal_bool *ip_validity);


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// szURL MUST be a character stream
// NET_Connect can supports both DNS format and IP Address format
#include "adp_events.h"
extern BOOL app_SocketEventcb (COS_EVENT *pstCosEv);
extern UINT32 CFW_SetTCPIPCallBack ( PFN_TCPIP_CALLBACK pfnEv ) ;




void NET_Connect(INET * pINet, const char_t *szURL, int32_t port)
{
#if 1
	NET * pNet = (NET *)pINet;
	WEPAppData * pWep = pNet->pWep;

	kal_uint8 buf[4] , addLen;
	kal_uint8 val = KAL_TRUE;
	sockaddr_struct sock_addr;
	kal_int8 ret_val;
	kal_bool ip_validity = KAL_FALSE;
kal_int8 result = -1;
	pINet->Close(pINet);
	
#ifdef WEP_DEBUG
	WM_DebugPrint("NET_Connect: begin");
#endif



	if(port > 0)
	{
		pNet->port = port;
	}
	pNet->status =  NET_STATUS_CONNECTING;
	pNet->currentDataAcountID = 131090;//(WEP_CMNET_ACCOUNT_ID & 0x000000ff);

#ifdef _KK_SELECT_ACCOUNT_
#ifdef SUPPORT_DUAL_SIM
{
	U32 accId = 0;
	int accountSelected;
	
	pWep->pILd->IntDataGet(pWep->pILd, LOCAL_DATA_RECORD_WHICH_ACCOUNT, &accountSelected); 
	abm_bearer2nw_id(ABM_E_SOCKET_BEARER_GSM_GPRS, accountSelected + 1, &accId);
	
	if (pWep->simStatus == MTPNP_AD_DUALSIM_USABLE )
	{
		if (pWep->simCardIndex == 0)
		{
			pNet->currentDataAcountID = kk_always_ask_encode_data_account_id(accId,1, FALSE,0);
		}
		else 
		{   //card 2
			pNet->currentDataAcountID = kk_always_ask_encode_data_account_id(accId,2, FALSE,0);
		}
	}
	else if (pWep->simStatus == MTPNP_AD_SIMCARD1_USABLE)
	{
		pNet->currentDataAcountID = kk_always_ask_encode_data_account_id(accId,1, FALSE,0);
	}
	else if (pWep->simStatus == MTPNP_AD_SIMCARD2_USABLE)
	{
		pNet->currentDataAcountID = kk_always_ask_encode_data_account_id(accId,2, FALSE,0);
	}
}
#endif
#else	//!_KK_SELECT_ACCOUNT_
#ifdef SUPPORT_DUAL_SIM
{
//	U32 accId = 0;
//	abm_bearer2nw_id(ABM_E_SOCKET_BEARER_GSM_GPRS, GetAccountCount(), &accId);
	if (pWep->simStatus == MTPNP_AD_DUALSIM_USABLE )
	{
		if (pWep->simCardIndex == 0)
		{
			//card 1
			MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
			MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL,MTPNP_AD_CHANNEL_MASTER);
		//	pNet->currentDataAcountID = kk_always_ask_encode_data_account_id(accId,1, FALSE,0);
		}
		else 
		{   //card 2
			MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
			MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL,MTPNP_AD_CHANNEL_SLAVE);
		//	pNet->currentDataAcountID = kk_always_ask_encode_data_account_id(accId,2, FALSE,0);
		}
	}
	else if (pWep->simStatus == MTPNP_AD_SIMCARD1_USABLE)
	{
		MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
		MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL,MTPNP_AD_CHANNEL_MASTER);
	//	pNet->currentDataAcountID = kk_always_ask_encode_data_account_id(accId,1, FALSE,0);
	}
	else if (pWep->simStatus == MTPNP_AD_SIMCARD2_USABLE)
	{
		MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
		MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL,MTPNP_AD_CHANNEL_SLAVE);
	//	pNet->currentDataAcountID = kk_always_ask_encode_data_account_id(accId,2, FALSE,0);
	}
}
#endif
#endif	//_KK_SELECT_ACCOUNT_

#ifdef WEP_DEBUG
WM_DebugPrint("NET_Connect: current URL: %s:%d\n", szURL, port);
#endif

#ifdef SUPPORT_PROXY
	if(pWep->appUseProxy)
	{
		pNet->currentDataAcountID = WEP_CMWAP_ACCOUNT_ID;

		if(szURL != NULL)
		{
			int32_t len = 0;
	//		char_t tmpBuf[7];
			DEV_MemSet(pNet->currentRequestUrl,0x0,sizeof(pNet->currentRequestUrl));
			if(szURL[4] == ':')
			{
				len = 7;//discard "http://"
			}
			strcpy(pNet->currentRequestUrl,szURL+len);
		}
	}
#endif
	// Step 2: Open the socket. This shall be a TCP socket.
	// create socket
#if  1//(KK_MTKXXXX_SWVER < KK_MTKXXXX_V08A)
	pNet->sockfd = soc_create(2, 1, 0, MOD_MMI, pNet->currentDataAcountID);
#else
	pNet->sockfd = soc_create(SOC_PF_INET, SOC_SOCK_STREAM, 0, MOD_MMI, pNet->currentDataAcountID);	
#endif

#ifdef WEP_DEBUG
WM_DebugPrint("NET_Connect: soc_create pNet->sockfd = %d,pNet->currentDataAcountID is %d",pNet->sockfd,pNet->currentDataAcountID);
#endif

#if 0
{
	kal_uint8 local_ip[32];
	kal_uint8 i;
DEV_MemSet((void *)&local_ip, 0, 32);

	soc_getlocalip((kal_uint8 *)&local_ip);
	for(i=0;i<32;i++)
	{
		WM_DebugPrint("<<<<<<<<<<<<<< soc_getlocalip[%d]=%d >>>>>>>>>>>>>>>>>>", i, local_ip[i]);
	}
}
#endif

	if ( NET_IS_INVALID(pNet->sockfd) )
	{
		NET_Close(pINet);
		pNet->connectCB(pNet->pConnectCBData, NULL, NET_ERR_CONNECT);
		return;
       }

	// set socket to  non-blocking mode
	result =soc_setsockopt(pNet->sockfd, SOC_NBIO, &val, sizeof(val));
	WM_DebugPrint("NET_Connect: soc_setsockopt, ,result=%d",result);

  if( result< 0 )
  {
		NET_Close(pINet);
		pNet->connectCB(pNet->pConnectCBData, NULL, NET_ERR_CONNECT);
		return;
  }

	// set socket to Async mode
	val = SOC_READ | SOC_WRITE | SOC_CONNECT | SOC_CLOSE | SOC_ACCEPT;

	ret_val = soc_setsockopt(pNet->sockfd, SOC_ASYNC, &val, sizeof(val));
#ifdef WEP_DEBUG
WM_DebugPrint("NET_Connect: setsockopt SOC_ASYNC,re=%d",ret_val);
#endif

	if(ret_val < 0)
	{
		NET_Close(pINet);
		pNet->connectCB(pNet->pConnectCBData, NULL, NET_ERR_CONNECT);
		return;
	}

	val = soc_ip_check((kal_char *)szURL, buf, &ip_validity);
//	val = TRUE;
#ifdef SUPPORT_PROXY
	if(pWep->appUseProxy)
	{
	  val = TRUE;
	}
#endif

	if(val == FALSE)
	{
	  ret_val=soc_gethostbyname(KAL_FALSE, MOD_MMI, WEP_DNS_ID, (const kal_char *)szURL, buf, &addLen, 0, pNet->currentDataAcountID);
#ifdef WEP_DEBUG
WM_DebugPrint("soc_gethostbyname,re=%d",ret_val);
#endif
	}
	else 
	{
		addLen = 4;
		ret_val = SOC_SUCCESS;
	}

  if(ret_val == SOC_SUCCESS)
  {
		// Step 3: Connect the Socket
		memset(&sock_addr,0x00,sizeof(sockaddr_struct));

		sock_addr.addr[0] = buf[0];    
		sock_addr.addr[1] = buf[1];
		sock_addr.addr[2] = buf[2];
		sock_addr.addr[3] = buf[3];

		sock_addr.port = port;  //htons(25);
#ifdef SUPPORT_PROXY
	if(pWep->appUseProxy)
	{
		sock_addr.addr[0] = 10;    
		sock_addr.addr[1] = 0;
		sock_addr.addr[2] = 0;
		sock_addr.addr[3] = 172;
		sock_addr.port = 80;  //htons(25);
	}
#endif
	sock_addr.addr_len=addLen;
	#ifdef WEP_DEBUG
	WM_DebugPrint("NET_Connect: sock_addr,%d,%d,%d,%d,%d",sock_addr.addr[0],sock_addr.addr[1],sock_addr.addr[2],sock_addr.addr[3],sock_addr.port);
	#endif

	ret_val = soc_connect(pNet->sockfd, &sock_addr);

	#ifdef WEP_DEBUG
	WM_DebugPrint("NET_Connect: connect result: %d", ret_val);
	#endif

		if((ret_val < 0) && (ret_val != SOC_WOULDBLOCK))
		{
		    NET_Close(pINet);
				pNet->status = NET_STATUS_IDLE;
				pNet->connectCB(pNet->pConnectCBData, NULL, NET_ERR_CONNECT);
		}
		else
		{
			SetProtocolEventHandler(CSD_SocketEventHandler, MSG_ID_APP_SOC_NOTIFY_IND/*MSG_ID_APP_SOC_NOTIFY_IND*/);
		}
  }
  else if(ret_val == SOC_WOULDBLOCK)
  {
      SetProtocolEventHandler( CSD_DnsEventHandler, MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND/*MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND*/);
  }
  else 
  {
			NET_Close(pINet);

			pNet->status = NET_STATUS_IDLE;
			pNet->connectCB(pNet->pConnectCBData, NULL, NET_ERR_CONNECT);
  }	
#endif
	return;
}//end NET_Connect()


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
static void NET_Cancel(INET *pINet)
{
	NET  * pNet = (NET *)pINet;
// 	WEPAppData * pWep = pNet->pWep;

// 	if( pWep->bRequesting && pNet->bReqTimerRunning ) {
// 		return ;
// 	}

	pNet->status = NET_STATUS_IDLE;

}//end NET_Cancel()

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
static void NET_Read(INET * pINet)
{
	NET * pNet = (NET *)pINet;
//  	WEPAppData * pWep = pNet->pWep;

#ifdef WEP_DEBUG
	WM_DebugPrint("NET_Read");
#endif

  if ( NET_IS_INVALID(pNet->sockfd) ) {
		return ;
 	}

	pNet->status = NET_STATUS_READING;

}//end NET_Read()

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
static void NET_Send(INET * pINet, byte_t * pBuffer, uint32_t nDataLen)
{
	
	#if 1
	NET *pNet = (NET *)pINet;
 	WEPAppData *pWep = pNet->pWep;
 	int32_t numbytes = 0;

	if ( NET_IS_INVALID(pNet->sockfd) ) {
		return ;
 	}

//	if(pNet->status == NET_STATUS_WRITING) {
//		WM_DebugPrint(">>>>>NET_SEND: pNet->status == NET_STATUS_WRITING");
//		return;
//	}
	pNet->status = NET_STATUS_WRITING;
#ifdef SUPPORT_PROXY
	if(pWep->appUseProxy)
	{
		byte_t buf[2*1024];
		int32_t len;
		DEV_MemSet(buf,0x0,sizeof(buf));
		strcpy((char *)buf,"POST / HTTP/1.1\r\n");
		len = strlen((const char *)buf);
//#ifdef WEP_DEBUG 
		sprintf((char *)&buf[len],"X-Online-Host: %s:%d\r\n",pNet->currentRequestUrl,pNet->port);
//		sprintf((char *)&buf[len],"X-Online-Host: %s:%d\r\n",pNet->currentRequestUrl,(pNet->port == WEP_SERVER_PORT)?44168:pNet->port);
//#else
//		sprintf((char *)&buf[len],"X-Online-Host: %s:44168\r\n",WEP_SERVER_URL);
//#endif 
		len = strlen((const char *)buf);
		sprintf((char *)&buf[len],"Accept: */*\r\nUser-Agent: Profile/MIDP-2.0 Configuration/CLDC-1.0\r\nHost: 10.0.0.172:80\r\nUser-Agent: UNTRUSTED/1.0\r\nContent-Length: %d\r\n\r\n",nDataLen);
#ifdef WEP_DEBUG
	WM_DebugPrint("%s",&buf[100]);
#endif
		len = strlen((const char *)buf);
		DEV_MemCpy(&buf[len],pBuffer,nDataLen);

		numbytes = soc_send(pNet->sockfd, buf, nDataLen+len, 0);
		if(numbytes > 0)
		{
			numbytes = nDataLen;
		}
	}
	else
#endif
	{
		numbytes = soc_send(pNet->sockfd, pBuffer, nDataLen, 0);
	}

#ifdef WEP_DEBUG
WEP_DebugPrint("********************** NET_Send %d:%d bytes",numbytes,nDataLen);
#endif

	if (numbytes>0 && numbytes != nDataLen)
	{
		WM_DebugPrint("############  WARNING WARNING WARNING : numbytes != nDataLen ########### ");
	}
	
	if((numbytes != SOC_WOULDBLOCK)&&(numbytes != SOC_LIMIT_RESOURCE))
	{
		pWep->bSOC_WOULDBLOCK = FALSE;

#ifdef SUPPORT_PROXY
		if(pNet->pWep->appUseProxy)
		{
			pNet->newRead = TRUE;
		}
#endif
		pNet->sendCB(pNet->pSendCBData, NULL,numbytes > 0 ? numbytes : NET_ERR_SEND);
	}
	else
	{
#ifdef WEP_DEBUG
	WEP_DebugPrint("==========********** pWep->bSOC_WOULDBLOCK = TRUE");
#endif	
		
		pWep->bSOC_WOULDBLOCK = TRUE;
		SetProtocolEventHandler(CSD_SocketEventHandler, MSG_ID_APP_SOC_NOTIFY_IND);
		pNet->sendCB(pNet->pSendCBData, NULL,0);
	}
	#endif



}//end NET_Send()

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
static void NET_Close(INET * pINet)
{
	#if 1
	NET * pNet  = (NET *)pINet;
// 	WEPAppData * pWep = pNet->pWep;

#ifdef WEP_DEBUG
WM_DebugPrint("NET_Close:");
#endif

	ClearProtocolEventHandler(MSG_ID_APP_SOC_NOTIFY_IND/*MSG_ID_APP_SOC_NOTIFY_IND*/);
	ClearProtocolEventHandler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND/*MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND*/);

	if ( !NET_IS_INVALID(pNet->sockfd) )
	{
 		soc_close(pNet->sockfd);
 
	soc_close_nwk_account(MOD_MMI); //add for ttp10085
//#endif
	//	soc_StopGPRS(NULL);
		pNet->socketConnected = FALSE;
		pNet->sockfd = SOC_INVALID_SOCKET;
	}
	#endif
}//end NET_Close()

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#if 1
static void CSD_SocketEventHandler(void* inMsg)
{
	NET * pNet = gNet;

	int32_t status;
	kal_uint8  * buffer = gReadBuffer;




	app_soc_notify_ind_struct * soc_ind = (app_soc_notify_ind_struct *)inMsg;

WM_DebugPrint(" ::::::::::: CSD_SocketEventHandler: inMsg addr:0x%x" ,&inMsg);

	if (soc_ind->socket_id != pNet->sockfd)
		return ;

	switch( soc_ind->event_type )
	{
		case SOC_CONNECT:
			if(soc_ind->result == FALSE)
			{
#ifdef WEP_DEBUG
 				WM_DebugPrint("### CSD_SocketEventHandler: SOC_CONNECT  ERROR###");
#endif			

				NET_Close(pNet->pWep->pINet);

				pNet->status = NET_STATUS_IDLE;
				pNet->connectCB(pNet->pConnectCBData, NULL, NET_ERR_TIMEOUT);
			}
			else
			{
#ifdef WEP_DEBUG
			 	WM_DebugPrint("### CSD_SocketEventHandler: SOC_CONNECT###");
#endif			
				pNet->socketConnected = TRUE;
				pNet->connectCB(pNet->pConnectCBData, NULL, NET_ERR_SUCCESS);
			}
			break;

		case SOC_WRITE:
#ifdef WEP_DEBUG
	WEP_DebugPrint("### CSD_SocketEventHandler: SOC_WRITE###pWep->bSOC_WOULDBLOCK=FALSE");
#endif			
			pNet->pWep->bSOC_WOULDBLOCK=FALSE;
			WM_MsgNotify(pNet->pWep->pDeviceData, WEP_DATA_SEND, 0, 0); // 异步发送
//			pNet->sendCB(pNet->pSendCBData, NULL,
//					pNet->numberOfBytesSent > 0 ? pNet->numberOfBytesSent : NET_ERR_SEND);
			break;

		case SOC_READ:
#ifdef WEP_DEBUG
	WM_DebugPrint("### CSD_SocketEventHandler: SOC_READ###pNet->sockfd %d",pNet->sockfd);
#endif			
#if 0
			if (pNet->status != NET_STATUS_READING)
			{
 
			}
#endif			
//			if(pNet->readCB != NULL)
			{
				int32_t len = 0;
				do
				{
					len = 0;
					memset((kal_uint8 *)buffer,0,MAX_READ_BUFFER_SIZE);
					status = soc_recv(pNet->sockfd, (kal_uint8 *)buffer, MAX_READ_BUFFER_SIZE, 0);
					
//					if (status > 1500)
//					{
//						return;//error discard
//					}
#ifdef WEP_DEBUG
					WEP_DebugPrint("### SOC_READ: status = %d###",status);
// #if 0
// 					if (status >= 0)
// 					{
// 						pNet->pWep->numOfBytesRead += status;
// 						WM_DebugPrint(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> numOfBytesRead = %d <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<",pNet->pWep->numOfBytesRead);
// 						continue;
// 					} else
// 						return;
// 					
// #endif
					
#endif


#ifdef SUPPORT_PROXY
					if(pNet->pWep->appUseProxy)
					{
				//		int32_t i;
						byte_t tmpBuf[4];
#ifdef WEP_DEBUG
						WM_DebugPrint("pNet->newRead=%d",pNet->newRead);
#endif

						if(pNet->newRead)
						{
							pNet->newRead = FALSE;
							len = DEV_MemFind(buffer, 32, (byte_t *)"200 OK", 6);
							if(len <= 0)//error
							{
								len = 0;
								if(pNet->readCB != NULL)
								{
									pNet->readCB(pNet->pReadCBData, buffer,NET_ERR_READ);
								}
								return ;
							}
							len = 0;
						}

						tmpBuf[0] = 0x0d;
						tmpBuf[1] = 0x0a;
						tmpBuf[2] = 0x0d;
						tmpBuf[3] = 0x0a;

						if(status > 0)
						{
							len = DEV_MemFind(buffer, status, tmpBuf, 4);//find the end flag"\r\b\r\n"
						}
						if(len <= 0)
							len = 0;
						else
							len = len+4;
#ifdef WEP_DEBUG
						WM_DebugPrint("### http head=%d",len);
#endif
// 							if(status > 0)
// 							{
// 								int32_t j=0,k;
// 								UI_character_type buf[1024];
// 								DEV_MemSet(buf,0x0,1024);
// 								strncpy((char *)buf,(const char *)buffer,len);
// #ifdef WEP_DEBUG
// 								kal_prompt_trace(MOD_TP_TASK,"%s",buf);
// #endif
// 						
// 							}	
					}
#endif	

					if(status>0)
					{
						if(pNet->readCB != NULL)
						{
							pNet->readCB(pNet->pReadCBData, buffer+len,status-len);	
						}
						else 
							break;
					}
				}while(status>0);

				if(status == SOC_SUCCESS)
				{
						return;
				}
#ifdef SUPPORT_PROXY
				if (pNet->pWep->appUseProxy)
				{
					if(status == SOC_SUCCESS)
					{
						return;
					}
					else if(status!=SOC_WOULDBLOCK)
					{
						if(pNet->readCB != NULL)
						{
							pNet->readCB(pNet->pReadCBData, buffer,NET_ERR_READ);
						}
					}
				}
				else
#endif
				if(status < SOC_WOULDBLOCK)
				{
					if(pNet->readCB != NULL)
					{
						pNet->readCB(pNet->pReadCBData, buffer,NET_ERR_READ);
					}
				}
			}
			break;

		case SOC_CLOSE:
#ifdef WEP_DEBUG
	WEP_DebugPrint("### CSD_SocketEventHandler: SOC_CLOSE ###");
#endif

		pNet->pWep->pILnt->NetCB(pNet->pWep->pILnt, NULL, NET_ERR_CONNECT);
		break;
		case SOC_ACCEPT:
#ifdef WEP_DEBUG
	WM_DebugPrint("### CSD_SocketEventHandler: SOC_ACCEPT###");
#endif
			break;
		default:
#ifdef WEP_DEBUG
	WM_DebugPrint("########################################################### CSD_SocketEventHandler: default###");
#endif			
			break;
	}


}

void CSD_DnsEventHandler(void* inMsg)
{

#if 1
	NET * pNet = gNet;
	WEPAppData *pWep = pNet->pWep;



  app_soc_get_host_by_name_ind_struct *dns_ind=(app_soc_get_host_by_name_ind_struct *)inMsg;

	//kal_uint8 buf[4] , addLen;
	//kal_uint8 val = KAL_TRUE;
	sockaddr_struct sock_addr;
	kal_int8 ret_val;
	WM_DebugPrint("CSD_DnsEventHandler: dns_ind->request_id: %d", dns_ind->request_id);

  //if(dns_ind->request_id == WEP_DNS_ID) //chenhe for kk,as our request_id is 0
  {
  
		  if(dns_ind->result == FALSE)
		  {
				NET_Close(pWep->pINet);

				pNet->status = NET_STATUS_IDLE;
				pNet->connectCB(pNet->pConnectCBData, NULL, NET_ERR_TIMEOUT);
		    return;
		  }
  

		// Step 3: Connect the Socket
		memset(&sock_addr,0x00,sizeof(sockaddr_struct));

		sock_addr.addr[0] = dns_ind->addr[0];    
		sock_addr.addr[1] = dns_ind->addr[1];
		sock_addr.addr[2] = dns_ind->addr[2];
		sock_addr.addr[3] = dns_ind->addr[3];

		sock_addr.port = pNet->port;  //htons(25);
		sock_addr.addr_len=dns_ind->addr_len;
	#ifdef WEP_DEBUG
	WM_DebugPrint("CSD_DnsEventHandler:x-host %d,%d,%d,%d,%d",sock_addr.addr[0],sock_addr.addr[1],sock_addr.addr[2],sock_addr.addr[3],sock_addr.port);
	#endif

		ret_val = soc_connect(pNet->sockfd, &sock_addr);

	#ifdef WEP_DEBUG
	WM_DebugPrint("CSD_DnsEventHandler: connect result: %d", ret_val);
	#endif

		if((ret_val < 0) && (ret_val != SOC_WOULDBLOCK))
		{
				NET_Close(pWep->pINet);

				pNet->status = NET_STATUS_IDLE;
				pNet->connectCB(pNet->pConnectCBData, NULL, NET_ERR_CONNECT);
		}
		else
		{
			SetProtocolEventHandler(CSD_SocketEventHandler, MSG_ID_APP_SOC_NOTIFY_IND);
		}
  }
  return;

  #endif
}
#endif

#ifdef _KK_SELECT_ACCOUNT_
int NET_GetDefAccount(void)
{
	return WEP_CMNET_ACCOUNT_ID;
}
#endif

#endif
