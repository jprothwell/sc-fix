/******************************************************************************/
/*              Copyright (C) 2006, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    aps_main.c                                                    */
/*                                                                            */
/* Description:                                                               */
/*                                                                            */
/******************************************************************************/

#ifdef CFW_TCPIP_SUPPORT

#include <csw.h>
#include <sxs_io.h>
//#include "sap_io.h"


#include <csw_ppp.h>
#include <aps.h>
#include <ppp_cp.h>

//#include "reg.h"
extern BOOL BAL_ApsTaskTcpipProc (COS_EVENT* pEvent);


extern CFW_GPRS_PDPCONT_INFO * PdpContList[];
//aps_contxt_t Aps_Contxt;
extern ppp_Data_t ppp_Data;
ppp_conf_t ppp_Gprs_state;

//extern UINT32 CFW_GprsAtt(UINT8 nState, UINT16 nUTI)	;

//extern void POP3_MsgDispatch( COS_EVENT *pEvent );                  
void BAL_ApsTaskTimerProc( COS_EVENT *pEvent );
void BAL_ApsTaskSmtpProc( COS_EVENT *pEvent );
void BAL_ApsTaskPop3Proc( COS_EVENT *pEvent );
//
// BAL_AdvTaskMain
//
// This function is a advanced protocol stack task enntry. 
// 
// Caution:
// Ensure that the following code does not loop indefinitely waiting for any event!
// 



UINT8 Aps_APP;
BOOL g_Pdp_Acting = FALSE;
UINT8 PPP_Cid;

HANDLE Pre_Task = 0;

//[[hameina[+] for clean warning 2008.9.8
extern VOID Gprs_SetAppCngDefault(VOID);
extern UINT32 Gprs_SetAppCng(BOOL BeModify, HANDLE TaskHandle);
extern HANDLE  Gprs_GetAppCng(VOID);

extern HANDLE CFW_bal_GetTaskHandle(UINT8 nTaskId);

//]]hameina[+] for clean warning 2008.9.8

//[[hameina[mod]2008.9.8 add return to this function
UINT32 Cfw_SetIpHeadPresent(BOOL flag)
{
	UINT32 nRet=0;
  if(flag)
    Gprs_SetAppCngDefault();
  else
    nRet = Gprs_SetAppCng(TRUE,  CFW_bal_GetTaskHandle(CSW_TASK_ID_APS));

	return nRet;
}
VOID Cfw_SetApsApp(UINT8 ApsApp)
{
        CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("!!!!!!!!!Cfw_SetApsApp. 0x%x\n"),ApsApp);

  Aps_APP = ApsApp;
}

typedef enum _ATE_CALLER_T
{
	TIMEOUT,
	DATA_FULL,
} ATE_CALLER_T;

/*
UINT16 uart_PsReadRxData(UINT8 *pBuff, UINT16 uBuffSize);

UINT8 recv_uart_Buf[3000] = {0,};
UINT16 PPP_ReadUartData(ATE_CALLER_T caller)
{
	//UINT8 *pBuff = NULL;
	UINT16 uSize = 0;

	// Get data size in Rx buffer first.
	uSize = uart_PsReadRxData(NULL, 0);

	// Allocate memory to store the data from Rx ring buffer.
	//pBuff = malloc(uSize);
	//AT_ASSERT(pBuff != NULL);
	memset(recv_uart_Buf, 0, 3000);
 CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS read uart data .\n"));
	// Read uplink data.
	uSize = uart_PsReadRxData(recv_uart_Buf, uSize);

	 CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS read uart data . data size: %d\n"),uSize);
	return uSize;
 
}
*/
/*****************************************************************/
/*the inteface for AT module to send ppp data to PPP module      */
/*****************************************************************/
INT32 CFW_PppSendData(UINT8* pData, UINT16 nDataSize)
{
   COS_EVENT sEvt = {0,};


  if(NULL == pData)
  {
    return ERR_CFW_INVALID_PARAMETER;
  }


	// Fill OS message structure, message ID is EV_CFW_GPRS_DATA_IND.
	sEvt.nEventId = EV_APS_PPP_DATA_REQ;
  
	sEvt.nParam1 = (UINT32)pData;
	sEvt.nParam2 = nDataSize;

	// Send test data (message) to APS task.
	BAL_SendTaskEvent(&sEvt, CSW_TASK_ID_APS);

  return ERR_SUCCESS;
	//COS_SendEvent(g_hPPPTask, &sEvt, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

extern HANDLE COS_GetDefaultMmiTaskHandle(VOID);
extern BOOL uart_PsSendData(UINT8 *pData, UINT16 uDataSize);
/*
INT32 CFW_SendPppData(UINT8* pData, UINT16 nDataSize)
{

   COS_EVENT sEvt = {{0,}};


   if(NULL == pData)
   {
      return ERR_CFW_INVALID_PARAMETER;
   }


	//Fill OS message structure, message ID is EV_CFW_GPRS_DATA_IND.
	sEvt.nEventId = EV_CFW_PPP_DATA_IND;
	sEvt.nParam1 = (UINT32)pData;
	sEvt.nParam2 = nDataSize;
  
    CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS send data to Uart.\n"));

	uart_PsSendData(pData, nDataSize);

	// Send test data (message) to APS task.
	//BAL_SendTaskEvent(&sEvt, CSW_TASK_ID_APS);
	//COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &sEvt, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}
*/
/*****************************************************************/
/*this function send ppp EV_CFW_PPP_TERMINATE_IND to AT module      */
/*****************************************************************/
VOID CFW_PppTermInd(VOID)
{
  COS_EVENT sEvt ;
  CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("   in CFW_PppTermInd "));	
  
	sEvt.nEventId = EV_CFW_PPP_TERMINATE_IND;
	sEvt.nParam1 = 0;
	sEvt.nParam2 = 0;
  
  Gprs_SetAppCng(TRUE,  Pre_Task);
	COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &sEvt, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

/*****************************************************************/
/*this function check the GPRS status, do deactivation and detach*/
/*****************************************************************/
VOID CFW_PppTermProcess(VOID)
{
  CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("   in CFW_PppTermProcess "));	
  if(ppp_Gprs_state.PPP_Act == TRUE)//active in ppp
  { 
    
    CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("   in CFW_PppTermProcess active TRUE "));	
    #if 1
#if 0 
    UINT8 nSimID= 0;
    CFW_GprsAct(CFW_GPRS_DEACTIVED, ppp_Gprs_state.PPP_Cid, 3,nSimID);

    CFW_GprsAct(CFW_GPRS_DEACTIVED, ppp_Gprs_state.PPP_Cid, 3);
#endif
    #else
    UINT8 uTi;
    if(ERR_SUCCESS == CFW_GetFreeUTI(CFW_GPRS_SRV_ID, &uTi))
       CFW_GprsAct(CFW_GPRS_DEACTIVED, ppp_Gprs_state.PPP_Cid, uTi);
    else
      CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("   no free uTi "));	
    #endif
 
  }else{ //no active in PPP,
    CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("   in CFW_PppTermProcess "));	
    CFW_PppTermInd();//send terminate to ATE
  }
}
/*****************************************************************/
/*this function is the PPP event process entry*/
/*****************************************************************/

VOID Aps_PppProc( COS_EVENT* pEvent )
{

 
  CFW_EVENT pCfwEvent;

  PPP_CP_ENTRY(CPID_Aps_PppProc);  
   
  pCfwEvent.nType = HIUINT8(pEvent->nParam3);
  
  if(pEvent->nEventId == EV_CFW_GPRS_DATA_IND)
  {
    CFW_GPRS_DATA * Msg;
    Msg = (CFW_GPRS_DATA *)pEvent->nParam2;
    CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS receive EV_CFW_GPRS_DATA_IND. data length: %d\n"),Msg->nDataLength);
    
    ppp_OnReceiveIpPacket(Msg->pData, Msg->nDataLength);
    
    if(Msg != NULL)
      CSW_TCPIP_FREE(Msg);
    
  }else if(pEvent->nEventId == EV_CFW_GPRS_ATT_RSP)
  {
       
    CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS receive EV_CFW_GPRS_ATT_RSP.pCfwEvent.nType  : %d\n"),pCfwEvent.nType);
    if(pCfwEvent.nType == CFW_GPRS_ATTACHED)//attach success
    {
      ppp_Gprs_state.PPP_Act = TRUE;
      ppp_Gprs_state.PPP_Cid = 1;
      
      // if not set the PDP context, do it
      
      if(PdpContList[0] == NULL)
      {
        CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS set PDP context.\n"));
       
        CFW_GPRS_QOS qos;
        qos.nDelay = 4;
        qos.nMean = 16;
        qos.nPeak = 4;
        qos.nPrecedence = 3;
        qos.nReliability = 3;
#if 0

        CFW_GprsSetReqQos(1, &qos);
#endif
       
        CFW_GPRS_PDPCONT_INFO pdp_cont;
        pdp_cont.nApnSize = 5;
        pdp_cont.pApn = "CMNET";
            
        pdp_cont.nApnUserSize = 3;
        pdp_cont.pApnUser = "wap";
        pdp_cont.nApnPwdSize = 3;
        pdp_cont.pApnPwd= "wap";

        
        pdp_cont.nPdpAddrSize = 0;
        pdp_cont.pPdpAddr = NULL;
        pdp_cont.nDComp = 0; 
        pdp_cont.nHComp = 0;
        pdp_cont.nPdpType = CFW_GPRS_PDP_TYPE_IP;
#if 0

        CFW_GprsSetPdpCxt(ppp_Gprs_state.PPP_Cid, &pdp_cont);
#endif
      }
     
      #if 1
#if 0

#endif


      #else
      UINT8 uTi;
      if(ERR_SUCCESS == CFW_GetFreeUTI(CFW_GPRS_SRV_ID, &uTi))
      {
        CFW_GprsAct(CFW_GPRS_ACTIVED, ppp_Gprs_state.PPP_Cid, uTi);
      }else
        CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("   no free uTi "));	
      #endif
    }else if(pCfwEvent.nType == CFW_GPRS_DETACHED)//if Detached ,sent ternminate to AT
    {
      CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS eceive CFW_GPRS_DETACHED,APS send terminate ppp.\n"));
      ppp_Gprs_state.PPP_Att = FALSE;

      CFW_PppTermInd();
      
    }else{
      CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS attach failure. error :0x%x\n"),pEvent->nParam1);
    }
  }else if(pEvent->nEventId == EV_CFW_GPRS_ACT_RSP)
  {
    CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS eceive EV_CFW_GPRS_ACT_RSP.\n"));
    
    //if PDP context actived, and  the ppp state is PPP_STATUS_NCP_START, begin handle NCP packet
    if(pCfwEvent.nType == CFW_GPRS_ACTIVED)// active success
    {
      g_Pdp_Acting = FALSE;
      ppp_Gprs_state.PPP_Cid = 1;
      
      if(ppp_Data.Status == PPP_STATUS_NCP_START)
      {
        ppp_ProcessNcpPacket();
        
      }else if(ppp_Data.Status ==PPP_STATUS_PPP_NULL)
      {
        //TODO
      }
    }else if(pCfwEvent.nType == CFW_GPRS_DEACTIVED)//deactive success
    {
      CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS eceive CFW_GPRS_DEACTIVED.\n"));
      ppp_Gprs_state.PPP_Cid = 0;
      ppp_Gprs_state.PPP_Act = FALSE;
      
      if(ppp_Gprs_state.PPP_Att == TRUE)
      {
        
        #if 1

        #else
          UINT8 uTi;
        if(ERR_SUCCESS == CFW_GetFreeUTI(CFW_NW_SRV_ID, &uTi))
        {
          CFW_GprsAtt(CFW_GPRS_DETACHED, uTi);
        }else
             CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("   no free uTi "));	
        #endif
        
        PPP_CP_EXIT(CPID_Aps_PppProc);        
        
        return;
      }else{
        CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS send terminate ppp.\n"));

        CFW_PppTermInd();
      }
    }else{
        CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS active failure.\n"));

    }
    
  
 }else if(pEvent->nEventId == EV_APS_PPP_DATA_REQ)
 {
    CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("Process ppp data from AT.\n"));

    if(Aps_APP == APS_TCP)
      Aps_APP = APS_PPP;
    
    UINT16 packet_head =0 ;
    UINT16 elem = 0;
    CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("Process ppp data from AT. data length: %d\n"),pEvent->nParam2);

    //SXS_DUMP(_MMI, 0, ppp_Buffer, pEvent->nParam2);

    UINT8* tmp = (UINT8*)(pEvent->nParam1);
   
    while (elem < pEvent->nParam2)
    {
      elem++;
      while(( tmp[elem] !=0x7E)&&(elem < pEvent->nParam2))
      {
        elem ++;
      }
      elem ++;
      CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("PPP handle a packet : %d\n"),elem - packet_head);

      ppp_ProcessPPP(tmp + packet_head, elem - packet_head);
      packet_head = elem;
    }
    
  }else{
     CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("PPP receive  unknow event : 0x%x\n"),pEvent->nEventId);

  }
      
  PPP_CP_EXIT(CPID_Aps_PppProc); 
}
//end add
UINT32 trace_id;
//add by wuys 2008-07-21 for other project except AT to compile correctly


BOOL (*PPP_SendFuc)(UINT8 *pData, UINT16 uDataSize) = NULL;

void CFW_SetPppSendFun(BOOL (* sendCallBack) (UINT8 *pData, UINT16 uDataSize))
{
  PPP_SendFuc = sendCallBack;
}
//add end
void APS_Init(void)
{
	//memset(Aps_Contxt.sockflag,0,MEMP_NUM_NETCONN);
	//Aps_Contxt.smtp_contxt.smtp_complete=TRUE;
	//PPP_SendFuc = NULL;

  Cfw_SetApsApp(APS_TCP);
  trace_id = _MMI;
  //ppp_Init();
}
/***************************************************************/
/*BAL_ApsEvtParse() added on 2006-10-31 for the optimization of APS task*/
/**/
/**************************************************************/
BOOL BAL_ApsEvtParse (COS_EVENT* pEvent)
{
  //          SXS_TRACE(_MMI, TSTXT("-----------APS parse ID:%d------\n"),pEvent->nEventId);
  //  CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("-----------APS parse ID:   0x%x, Aps_APP: 0x%x\n"),pEvent->nEventId, Aps_APP);

    switch (pEvent->nEventId)
    {
    	case EV_INTER_APS_TCPIP_REQ:
        if(Aps_APP != APS_TCP)
        {
          //ppp_Init();
          Cfw_SetApsApp(APS_TCP);
          Pre_Task = Gprs_GetAppCng();
          Gprs_SetAppCng(TRUE,  CFW_bal_GetTaskHandle(CSW_TASK_ID_APS));
        }
    		BAL_ApsTaskTcpipProc( pEvent );
    		break;
        
    	case EV_INTER_APS_SMTP_REQ:	
    		
    	//	SXS_TRACE(_MMI, TSTXT("!!!!!!!!EV_INTER_APS_SMTP_REQ\n"));
    		//BAL_ApsTaskSmtpProc( pEvent );
    	//	mime_parse_main();
    		break;
    
    	case EV_INTER_APS_POP3_REQ:	
    		BAL_ApsTaskPop3Proc( pEvent );
    		break;
        
    	case EV_CFW_GPRS_DATA_IND: 
      case EV_CFW_GPRS_ACT_RSP:
      case EV_CFW_GPRS_ATT_RSP:	
      //case EV_INTER_APS_ACT_REQ:
      case EV_CFW_GPRS_CXT_DEACTIVE_IND:
        if(Aps_APP == APS_TCP)
          BAL_ApsTaskTcpipProc( pEvent );
        else if(Aps_APP == APS_PPP)
          Aps_PppProc( pEvent );
        break;
        
      case EV_TIMER:	
    		BAL_ApsTaskTimerProc( pEvent );
        break;
        
      case EV_APS_PPP_DATA_REQ:
        if(Aps_APP != APS_PPP)
        {
          ppp_Init();
          Cfw_SetApsApp(APS_PPP);
          Pre_Task = Gprs_GetAppCng();
          Gprs_SetAppCng(TRUE,  CFW_bal_GetTaskHandle(CSW_TASK_ID_APS));
        }
        CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS receive ppp data from AT.\n"));
        Aps_PppProc( pEvent );
        break;
        
    	default:
    		break;
    }
//	COS_FREE(pEvent->nParam1);
	return TRUE;
}
void BAL_ApsTaskTimerProc( COS_EVENT *pEvent )
{
   	if( (pEvent->nParam1 == PRV_CFW_TCP_TIMER_ID) || (pEvent->nParam1 == PRV_CFW_IP_TIMER_ID)|| (pEvent->nParam1 == PRV_CFW_DNS_TIMER_ID)  )
    {

        BAL_ApsTaskTcpipProc( pEvent );
    }
}
void BAL_ApsTaskSmtpProc( COS_EVENT *pEvent )
{
  //	aps_smtp_req((aps_Msg_body_t *)pEvent->nParam1);
  	COS_FREE(pEvent->nParam1);
  	return;
}
void BAL_ApsTaskPop3Proc( COS_EVENT *pEvent )
{

   // POP3_MsgDispatch( pEvent );
    return;

}
#endif
