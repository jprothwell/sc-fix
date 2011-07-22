////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2009, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: cfw_gprs_main.c
// 
// DESCRIPTION:
// TODO: ...
// 
// REVISION HISTORY:
//    NAME              DATE                REMAKS
// EP                2009-5-6       Created initial version 1.0
//
////////////////////////////////////////////////////////////////////////////////
 

#ifdef CFW_GPRS_SUPPORT
#include "sxs_io.h"

#include <csw.h>
#include "api_msg.h"
#include <cfw_prv.h>

#include "cfw_gprs.h"
#include "cfw_gprs_tool.h"
#include "cfw_gprs_data.h"
#include "string.h"



#include "dbg.h"
extern int sprintf(char *buf, const char *fmt, ...);
#define CSW_ASSERT(expr) \
				do { \
					char assertion_info[256] = {0}; \
					sprintf(assertion_info, "[TCP_ASSERT]FILE:%s FUN:%s LN:%d", __FILE__, __FUNCTION__, __LINE__); \
					DBG_ASSERT(expr, assertion_info); \
				} while (0)

// Re-route APS events to MMI for AT
#ifdef CFW_EXTENDED_API
// TODO: use a more explicit compile flag for this
#define GET_APS_TASK        CFW_bal_GetTaskHandle(CSW_TASK_ID_MMI_DEFAULT)
#else
#define GET_APS_TASK        CFW_bal_GetTaskHandle(CSW_TASK_ID_APS)
#endif
#define DURATION_BEFORE_DETACH  (120*1000 MILLI_SECOND)

#ifdef CFW_MULTI_SIM

CFW_GPRS_PDPCONT_INFO * PdpContList[CFW_SIM_COUNT][7];

CFW_GPRS_QOS pQosList[CFW_SIM_COUNT][7];
CFW_GPRS_QOS pMinQosList[CFW_SIM_COUNT][7];
struct CurrentAppCng CurAppCng[CFW_SIM_COUNT];
UINT32 CFW_GprsRemovePdpCxt(UINT8 nCid, CFW_SIM_ID nSimID);

UINT32 CFW_StopDetachTimer(CFW_SIM_ID nSimID);
UINT32 CFW_StartDetachTimer(CFW_SIM_ID nSimID);
BOOL ispdpactive(CFW_SIM_ID nSimID);
UINT32 gprs_SetGprsSum(INT32 upsum,INT32 downsum,CFW_SIM_ID nSimID );
UINT32 gprs_GetGprsSum(INT32* upsum,INT32* downsum,CFW_SIM_ID nSimID );

#else

CFW_GPRS_PDPCONT_INFO * PdpContList[7]={NULL,};

CFW_GPRS_QOS pQosList[7];
CFW_GPRS_QOS pMinQosList[7];
struct CurrentAppCng CurAppCng;
UINT32 CFW_GprsRemovePdpCxt(UINT8 nCid);
UINT32 CFW_StopDetachTimer(VOID);
UINT32 CFW_StartDetachTimer(VOID);

#endif


CONST UINT8 IPCP_OPTION[19]={0x80,0x21,0x10,0x01,0x00,0x00,0x10,0x81,0x06,0x00,0x00,0x00,0x00,0x83,0x06,0x00,0x00,0x00,0x00};//add by wuys 2006-02-26

PRIVATE UINT32 CFW_GprsAoProc (HAO hAo,CFW_EV* pEvent);


#ifdef CFW_MULTI_SIM
BOOL Gprs_SetAppCngInit(CFW_SIM_ID nSimID)
{
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error Gprs_SetAppCngInit()\r\n"));	

		return nRet;
	}
	
	CurAppCng[nSimID].CurAppSet = TRUE;
	CurAppCng[nSimID].CurrentAppTaskHandle =  GET_APS_TASK;

	return nRet;
}

BOOL Gprs_SetAppCngDefault(CFW_SIM_ID nSimID)
{
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error Gprs_SetAppCngDefault()\r\n"));	

		return nRet;
	}
	CurAppCng[nSimID].CurAppSet = TRUE;
	CurAppCng[nSimID].CurrentAppTaskHandle = GET_APS_TASK;
	return nRet;
}

UINT32 Gprs_SetAppCng(BOOL BeModify, HANDLE TaskHandle, CFW_SIM_ID nSimID)
{
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error Gprs_SetAppCng()\r\n"));	

		return nRet;
	}
	
    if((CurAppCng[nSimID].CurAppSet)||(TaskHandle == CurAppCng[nSimID].CurrentAppTaskHandle))
    {
		CurAppCng[nSimID].CurAppSet = BeModify;
		CurAppCng[nSimID].CurrentAppTaskHandle =  TaskHandle;
		return ERR_SUCCESS;
    }
	else
	{
		return ERR_INVALID_HANDLE;// this application don't modify the application aim now.
    }
}

HANDLE  Gprs_GetAppCng(CFW_SIM_ID nSimID)
{
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error Gprs_GetAppCng()\r\n"));	

		return nRet;
	}

	return CurAppCng[nSimID].CurrentAppTaskHandle;
}

#else
VOID Gprs_SetAppCngInit(VOID)
{
  CurAppCng.CurAppSet = TRUE;
  CurAppCng.CurrentAppTaskHandle =  GET_APS_TASK;
  //CurAppCng.CurrentAppTaskHandle = COS_GetDefaultMmiTaskHandle();
};

VOID Gprs_SetAppCngDefault(VOID)
{
      CurAppCng.CurAppSet = TRUE;
      CurAppCng.CurrentAppTaskHandle = GET_APS_TASK;
      //CurAppCng.CurrentAppTaskHandle = COS_GetDefaultMmiTaskHandle();

}
UINT32 Gprs_SetAppCng(BOOL BeModify, HANDLE TaskHandle)
{
    if((CurAppCng.CurAppSet)||(TaskHandle == CurAppCng.CurrentAppTaskHandle))
    {
      CurAppCng.CurAppSet = BeModify;
      CurAppCng.CurrentAppTaskHandle =  TaskHandle;
      return ERR_SUCCESS;

    }else{
      return ERR_INVALID_HANDLE;// this application don't modify the application aim now.
    }
}
HANDLE  Gprs_GetAppCng(VOID)
{
  return CurAppCng.CurrentAppTaskHandle;
}

#endif


// Only use to test 
UINT32 Gprs_MSG_Redirect(CFW_EVENT* pEvent, VOID* pUserData)
{
    COS_EVENT cos_ev;
	UINT32 ret = ERR_SUCCESS;
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID = pEvent->nFlag;
#endif

    cos_ev.nEventId = pEvent->nEventId;
    cos_ev.nParam1  = pEvent->nParam1;
    cos_ev.nParam2  = pEvent->nParam2;
    cos_ev.nParam3  = MAKEUINT32(MAKEUINT16(pEvent->nFlag, pEvent->nType), pEvent->nUTI);
#ifdef CFW_MULTI_SIM
    COS_SendEvent(CurAppCng[nSimID].CurrentAppTaskHandle, &cos_ev,COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);// modify by wuys 2008-04-14
#else
    COS_SendEvent(CurAppCng.CurrentAppTaskHandle, &cos_ev,COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);// modify by wuys 2008-04-14
#endif
	return ret;
}

#ifdef CFW_MULTI_SIM
UINT8 * DnsServerIP[2][7];//add wuys 2009-10-26
extern INT32 gprs_sum[CFW_SIM_COUNT][2];
UINT8 GprsCidSavedInApp[CFW_SIM_COUNT];
#else
UINT8 * DnsServerIP[7];//add wuys 2009-10-26
UINT8 GprsCidSavedInApp;

#endif


#ifdef CFW_MULTI_SIM
BOOL IsCidSavedInApp(UINT8 Cid, UINT8 nSimID)
{
		if(0x00==(GprsCidSavedInApp[nSimID] &(0x01 << Cid)))
			return FALSE;
		else
			return TRUE;
}
#else
BOOL IsCidSavedInApp(UINT8 Cid)
{
		if(0x00==(GprsCidSavedInApp &(0x01 << Cid)))
			return FALSE;
		else
			return TRUE;
}
#endif



UINT32 CFW_GprsInit(VOID) 
{

	UINT32 nRet = ERR_SUCCESS;
	
	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsInit !!!!!!!\n"));
	INT32 gprs_upsum_tmp =0;
	INT32 gprs_downsum_tmp =0;

#ifdef CFW_MULTI_SIM
      UINT8 nIndex = 0;
      for(nIndex = 0; nIndex < CFW_SIM_COUNT; nIndex++)
      	{
		nRet = CFW_CheckSimId(nIndex);
		if( ERR_SUCCESS!=nRet)
		{
			CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsInit()\r\n"));	

			return nRet;
		}
		UINT8 cidIdx =0;
		for(cidIdx=0; cidIdx<7; cidIdx++)
			DnsServerIP[nIndex][cidIdx]=NULL;
		gprs_GetGprsSum(&gprs_upsum_tmp,&gprs_downsum_tmp,nIndex);
		gprs_sum[nIndex][0] = gprs_upsum_tmp;
		gprs_sum[nIndex][1] = gprs_downsum_tmp;
		GprsCidSavedInApp[nIndex] = 0x00;
      	}
#else
     UINT8 cidIdx =0;
		for(cidIdx=0; cidIdx<7; cidIdx++)
			DnsServerIP[cidIdx]=NULL;
		GprsCidSavedInApp = 0x00;

#endif

    CFW_RegisterCreateAoProc(API_PDP_CTXACTIVATE_IND, GprsMTAoProc); 
#ifdef CFW_MULTI_SIM
      for(nIndex = 0; nIndex < CFW_SIM_COUNT; nIndex++)
      	{
		nRet=Gprs_SetAppCngInit(nIndex);
		if( ERR_SUCCESS!=nRet)
		{
			CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! Gprs_SetAppCngInit error CFW_GprsInit()\r\n"));	
			return nRet;
		}
      	}
#else
    Gprs_SetAppCngInit();
#endif
    CFW_SetEventProc( CFW_SRV_GPRS_EV_RSP, NULL, Gprs_MSG_Redirect );//add by wuys 2008-04-11 

    return nRet;
}




PRIVATE UINT32 CFW_GprsAoProc (HAO hAo,CFW_EV* pEvent)
{
    UINT32 nState    = 0;       // sm state
    CFW_EV ev;
    VOID* nEvParam = NULL;
    GPRS_SM_INFO * proc = NULL;
    UINT32 nEventId = 0;       // message id from stack
    UINT32 nUTI = 0;
    
    //
    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("In CFW_GprsAoProc\n"));
#ifdef CFW_MULTI_SIM
    CFW_SIM_ID nSimID =CFW_SIM_END;
    nSimID = CFW_GetSimCardID(hAo);
#endif
    // verify the parameter
    if((UINT32)pEvent == 0xffffffff)
    {
        CSW_TRACE(CFW_GPRS_TRACE, TSTXT("In CFW_GprsAoProc get message from AT \n"));
        SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
        ev.nParam1 = 0xff;
        pEvent = &ev;
    }    
    if( (hAo == 0) || (pEvent == NULL))
    {
        return ERR_CFW_INVALID_PARAMETER;
    }
    else
    {
        CFW_SetServiceId(CFW_GPRS_SRV_ID);

        nEvParam = (VOID*)pEvent->nParam1;
        //CFW_SetAoState(hAo)
        proc = CFW_GetAoUserData(hAo);
        nState = CFW_GetAoState(hAo);
        nEventId = pEvent->nEventId; 
    }
    if (nState == CFW_SM_STATE_IDLE)
    {
        if (pEvent->nParam1 == 0xff)    // req to stack
        {
            if (proc->nMsgId == API_PDP_CTXACTIVATE_REQ)  // activate  act = 1;
            {
                CFW_GPRS_PDPCONT_INFO sPdpCXT;
                api_PdpCtxActivateReq_t *pOutMsg = NULL;
                pOutMsg = (api_PdpCtxActivateReq_t *)CFW_MemAllocMessageStructure(SIZEOF(api_PdpCtxActivateReq_t));
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("send API_PDP_CTXACTIVATE_REQ\n"));
                SUL_MemSet8(pOutMsg, 0, SIZEOF(api_PdpCtxActivateReq_t));
				
                sPdpCXT.pApn = (UINT8 *)CFW_MALLOC(100);  // the max length of the APN is 100
                sPdpCXT.pPdpAddr = (UINT8 *)CFW_MALLOC(18);  // the max length of the PDPADDR is 18
                sPdpCXT.pApnUser= (UINT8 *)CFW_MALLOC(20);  // add by wuys 2008-02-25
                sPdpCXT.pApnPwd= (UINT8 *)CFW_MALLOC(20);  // add by wuys 2008-02-25
#ifdef CFW_MULTI_SIM
                if(CFW_GprsGetPdpCxt(proc->Cid2Index.nCid, &sPdpCXT,nSimID) != ERR_SUCCESS)
#else
                if(CFW_GprsGetPdpCxt(proc->Cid2Index.nCid, &sPdpCXT) != ERR_SUCCESS)
#endif
                {
//                    UINT8 nApnNet[5] = {'C', 'M', 'N', 'E', 'T'};         // for internet access // / france websfr
                    //UINT8 nApnNet[6] = {'w', 'e', 'b', 's', 'f', 'r'};         // for internet access // / french websfr
                    UINT8 nApnWap[5] = {'C', 'M', 'W', 'A', 'P'};         // for wap  access
                    UINT8 *nApn = NULL;
                    nApn = nApnWap;

                    
                    // get the context from flash error using the default values                
                    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("get PDP context error maybe flash error using the default settings\n"));
                    // type
                    sPdpCXT.nPdpType = 0;
                    
                    // apn
                    //sPdpCXT.nApnSize = Gprs_APN_Len;    // 5 for china  // 6 for franch
                    //SUL_MemCopy8(sPdpCXT.pApn, nApn, Gprs_APN_Len);    // 5 for china  // 6 for franch
                    
                    sPdpCXT.nApnSize = 5;    // 5 for china  // 6 for franch
                    SUL_MemCopy8(sPdpCXT.pApn, nApn, 5);    // 5 for china  // 6 for franch
                    

                    // pdp addr
                    sPdpCXT.nPdpAddrSize = 0;
                    SUL_MemSet8(sPdpCXT.pPdpAddr, 0, 4);
                    //sPdpCXT.pPdpAddr = (UINT8 *)CFW_MALLOC(4);
                    
                    // parameter
                    sPdpCXT.nDComp  = 0;
                    sPdpCXT.nHComp = 0;
                }
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT(" !!!!!!PDP context config, ApnUserSize: 0x%x; ApnPwdSize:  0x%x \n"),sPdpCXT.nApnUserSize, sPdpCXT.nApnPwdSize);
                SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, sPdpCXT.pApnUser, 20);
                SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, sPdpCXT.pApnPwd, 20);
                
                SUL_MemSet8(&proc->Qos, 0, SIZEOF(CFW_GPRS_QOS));
                #if 1 //TMP_GPRS_CONTEXT
#ifdef CFW_MULTI_SIM
                  CFW_GprsGetReqQos(proc->Cid2Index.nCid, &proc->Qos,nSimID);
#else
                  CFW_GprsGetReqQos(proc->Cid2Index.nCid, &proc->Qos);
#endif
                #endif
                // copy the qos ... to poutmsg
                // nsapi
                pOutMsg->NSapi = proc->Cid2Index.nNsapi;
                // smindex
                pOutMsg->SmIndex = proc->Cid2Index.nSmIndex;
                // qos  
                CFW_GprsQos2Api(proc->Qos, pOutMsg->QoS, &pOutMsg->QoSLen);
                
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("QOS: "));
                SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, pOutMsg->QoS, 11);

                // pdp address
                CFW_GprsPdpAddr2Api(&sPdpCXT, &pOutMsg->PDPAddLen, pOutMsg->PDPAddress);

                // apn   
                pOutMsg->APNLen = sPdpCXT.nApnSize + 1 ;
                pOutMsg->AccessPointName[0] = sPdpCXT.nApnSize;
               // modify by wuys for handle the APN containing ".", 2008-03-05
               //added by wuys 2008-03-05
                UINT8 i;
                UINT8 n =0;
                for(i = 0; i < sPdpCXT.nApnSize; i++)
                {
                  if( sPdpCXT.pApn[i] == '.')
                  {
                    pOutMsg->AccessPointName[n] = i - n;
                    SUL_MemCopy8(pOutMsg->AccessPointName  + n +1 , sPdpCXT.pApn + n, i - n);
                    n = i + 1;
                  }
                }
                if (n > 0)
                {
                  pOutMsg->AccessPointName[n] = i- n;
                  SUL_MemCopy8(pOutMsg->AccessPointName  + n +1 , sPdpCXT.pApn + n, i- n);

                }else{
                  SUL_MemCopy8(pOutMsg->AccessPointName + 1 , sPdpCXT.pApn, sPdpCXT.nApnSize);

                }
                //add end
              #if 0 //removed by wuys 2008-03-05
                
                SUL_MemCopy8(pOutMsg->AccessPointName + 1 , sPdpCXT.pApn, sPdpCXT.nApnSize);
              #endif
              //modify end
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("APN length: %d, APN: "),pOutMsg->APNLen);
                SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, pOutMsg->AccessPointName, sPdpCXT.nApnSize+1+4);

                // free the malloc value
                CFW_FREE(sPdpCXT.pApn);
                CFW_FREE(sPdpCXT.pPdpAddr);				
                // protocol option
               
              //modified by wuys for test APN anthentication, 2008-02-25
                
                pOutMsg->ProtocolCfgOption[0] = 0x80; 
                pOutMsg->ProtocolCfgOptionLen = 1;
                if((sPdpCXT.nApnUserSize > 0) && (sPdpCXT.nApnPwdSize > 0))
                {
                    
                  pOutMsg->ProtocolCfgOption[1] = 0xc0;
                  pOutMsg->ProtocolCfgOption[2] = 0x23; 
                  
                  pOutMsg->ProtocolCfgOption[3] = sPdpCXT.nApnUserSize +sPdpCXT.nApnPwdSize + 2 + 4; 
                  
                  pOutMsg->ProtocolCfgOption[4] = 0x01;
                  
                  pOutMsg->ProtocolCfgOption[5] = 0x00; 
                  
                  pOutMsg->ProtocolCfgOption[6] = 0x00;
                  pOutMsg->ProtocolCfgOption[7] = sPdpCXT.nApnUserSize +sPdpCXT.nApnPwdSize + 2 + 4; 
                  
                  pOutMsg->ProtocolCfgOption[8] = sPdpCXT.nApnUserSize; 
                  SUL_MemCopy8(pOutMsg->ProtocolCfgOption + 9 , sPdpCXT.pApnUser, sPdpCXT.nApnUserSize);
                  pOutMsg->ProtocolCfgOption[9 + sPdpCXT.nApnUserSize] = sPdpCXT.nApnPwdSize; 
                  SUL_MemCopy8(pOutMsg->ProtocolCfgOption + 9 + sPdpCXT.nApnUserSize +1 , sPdpCXT.pApnPwd, sPdpCXT.nApnPwdSize);
                  pOutMsg->ProtocolCfgOptionLen = sPdpCXT.nApnUserSize +sPdpCXT.nApnPwdSize + 4 + 4 + 2; 

                }
                SUL_MemCopy8(pOutMsg->ProtocolCfgOption + pOutMsg->ProtocolCfgOptionLen , IPCP_OPTION, 19);
                pOutMsg->ProtocolCfgOptionLen += 19;
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT(" !!!!!!pOutMsg->ProtocolCfgOptionLen: 0x%x  \n"),pOutMsg->ProtocolCfgOptionLen);

                SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, pOutMsg->ProtocolCfgOption, 100);

                CFW_FREE(sPdpCXT.pApnUser);
                CFW_FREE(sPdpCXT.pApnPwd);	
                
                proc->pMsgBody = pOutMsg;
#ifdef CFW_MULTI_SIM
				CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody,nSimID);
#else
				CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody);
#endif
                CFW_SetAoState(hAo, CFW_GPRS_STATE_CONNECTING);

            }
            else
            {
            }
        }
        else
        {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT(" should not Receive the pEvent->nParam1 = 0x%x\n"), pEvent->nParam1);
            CFW_UnRegisterAO(CFW_GPRS_SRV_ID, hAo);
        }
    }
    else if(nState == CFW_GPRS_STATE_CONNECTING)
    {
        if (nEventId == API_PDP_CTXACTIVATE_CNF) // actived
        {
            api_PdpCtxActivateCnf_t  *psInMsg  = (api_PdpCtxActivateCnf_t *)nEvParam; 
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("API_PDP_CTXACTIVATE_CNF data: \n"));                    
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, nEvParam, sizeof(api_PdpCtxActivateCnf_t));

            
            if (proc->Cid2Index.nSmIndex == psInMsg->SmIndex)
            {
                CFW_GPRS_PDPCONT_INFO sPdpCXT;
                
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_CONNECTING\nreceive API_PDP_CTXACTIVATE_CNF smindex = 0x%x\n"), psInMsg->SmIndex);
		            //CFW_Dump_SXS(CFW_GPRS_TRACE, (UINT8 *)psInMsg, SIZEOF(api_PdpCtxActivateCnf_t));
                //CFW_Dump_SXS(CFW_GPRS_TRACE, (UINT8 *)psInMsg, SIZEOF(api_PdpCtxActivateCnf_t));
                //store the return parmeters
                //act state
                proc->nActState = CFW_GPRS_ACTIVED;
                
                // qos
                CFW_GprsApi2Qos(&proc->Qos, psInMsg->QoS, psInMsg->QoSLen);
#ifdef CFW_MULTI_SIM
                CFW_GprsSetReqQos(proc->Cid2Index.nCid, &proc->Qos,nSimID);
#else
                CFW_GprsSetReqQos(proc->Cid2Index.nCid, &proc->Qos);
#endif
                
                sPdpCXT.pApn = (UINT8 *)CFW_MALLOC(100);  // the max length of the APN is 100
                sPdpCXT.pPdpAddr = (UINT8 *)CFW_MALLOC(18);  // the max length of the PDPADDR is 18
                sPdpCXT.pApnUser= (UINT8 *)CFW_MALLOC(20);  // add by wuys 2008-02-25
                sPdpCXT.pApnPwd= (UINT8 *)CFW_MALLOC(20);  // add by wuys 2008-02-25
               //add by wuys 2008-07-03
                memset(sPdpCXT.pApn, 0, 100);
                memset(sPdpCXT.pPdpAddr, 0, 18);
                memset(sPdpCXT.pApnUser, 0, 20);
                memset(sPdpCXT.pApnPwd, 0, 20);
                //add end
                if(sPdpCXT.pApn!=NULL && sPdpCXT.pPdpAddr!=NULL && sPdpCXT.pApnUser != NULL && sPdpCXT.pApnPwd != NULL)
                  CSW_TRACE(CFW_GPRS_TRACE, TSTXT("MALloc seccess! \n"));
#ifdef CFW_MULTI_SIM
                if(CFW_GprsGetPdpCxt(proc->Cid2Index.nCid, &sPdpCXT,nSimID) != ERR_SUCCESS)
#else
                if(CFW_GprsGetPdpCxt(proc->Cid2Index.nCid, &sPdpCXT) != ERR_SUCCESS)
#endif
                {
                    //get the values from flash error 
                    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsGetPdpCxt error, maybe the flash error \n"));                    
                }
                
                //pdp addr
                //sPdpCXT.pPdpAddr = (UINT8)CFW_MALLOC(psInMsg->PDPAddLen);  // need to do more
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("!!!!!API_PDP_CTXACTIVATE_CNF IP address len: 0x%x,address: \n"),psInMsg->PDPAddLen);                    
                SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, psInMsg->PDPAddress, psInMsg->PDPAddLen);
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("!!!!!API_PDP_CTXACTIVATE_CNF ProtocolCfgOption len: 0x%x,address: \n"),psInMsg->ProtocolCfgOptionLen);                    
                SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, psInMsg->ProtocolCfgOption, psInMsg->ProtocolCfgOptionLen);


                //Add by lixp for UTI zero bug at 20090723.
                CFW_GetUTI(hAo, &nUTI);

                if(psInMsg->PDPAddLen > 2) //add by wuys 2008-07-03, for avoiding error address length from stack
                {
                    CFW_GprsPdpApi2Addr(&sPdpCXT, psInMsg->PDPAddLen, psInMsg->PDPAddress);
                }
                else
                {
                   sPdpCXT.nPdpAddrSize = 0;
                   sPdpCXT.pPdpAddr = NULL;

                   
#ifdef CFW_MULTI_SIM
                   CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, proc->Cid2Index.nCid, 1, nUTI|(nSimID<<24), CFW_GPRS_ACTIVED);
#else
                   CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, proc->Cid2Index.nCid, 1, nUTI, CFW_GPRS_ACTIVED);
#endif
                }

               //add by wuys 2009-10-22

		  if(psInMsg->ProtocolCfgOptionLen > 0)
		  {
		      UINT8 tmp_i = 0 ;
		      UINT8 tmp_j = 0;
			  UINT8 dnsSize = 0;
		      for(tmp_i = 0; tmp_i < (psInMsg->ProtocolCfgOptionLen-1); tmp_i++) 
		      	{
		      	    if((psInMsg->ProtocolCfgOption[tmp_i] == 0x80) && (psInMsg->ProtocolCfgOption[tmp_i+1] == 0x21))
		      	    	{
		      	    	       for(tmp_j = 0; tmp_j <psInMsg->ProtocolCfgOption[tmp_i+2]; tmp_j++)
		      	    	 	{
		      	    	 	      if(psInMsg->ProtocolCfgOption[tmp_i+3+tmp_j] == 0x81)
		      	    	 	      {
						     dnsSize = psInMsg->ProtocolCfgOption[tmp_i+3+tmp_j+1] -2;
							CSW_TRACE(CFW_GPRS_TRACE, TSTXT("!!!!!API_PDP_CTXACTIVATE_CNFsPdpCXT.nDnsAddrSize: %d \n"),dnsSize);                    
#ifdef CFW_MULTI_SIM
						    DnsServerIP[nSimID][proc->Cid2Index.nCid-1]= (UINT8 *)CFW_MALLOC(dnsSize );
							 if(  DnsServerIP[nSimID][proc->Cid2Index.nCid-1] != NULL)
						            SUL_MemCopy8(DnsServerIP[nSimID][proc->Cid2Index.nCid-1], psInMsg->ProtocolCfgOption+tmp_i+3+tmp_j+2, dnsSize );
							 else
#else
                                              DnsServerIP[proc->Cid2Index.nCid-1]= (UINT8 *)CFW_MALLOC(dnsSize );
							 if( DnsServerIP[proc->Cid2Index.nCid-1] != NULL)
						            SUL_MemCopy8(DnsServerIP[proc->Cid2Index.nCid-1], psInMsg->ProtocolCfgOption+tmp_i+3+tmp_j+2, dnsSize );
							 else
#endif
							 	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("!!!!!API_PDP_CTXACTIVATE_CNFsPdpCXT.nDnsAddrSize: memory error \n"));                    
							tmp_j=tmp_j+2+dnsSize-1;
                                            break;
		      	    	 	      }
						  
		      	    	 	}

					tmp_i+=(tmp_j-1+3);
		      	    	}
		      	}
		  	
		  }
   //             CSW_TRACE(CFW_GPRS_TRACE, TSTXT("!!!!!API_PDP_CTXACTIVATE_CNF cid = %d, dns server,address: \n"),proc->Cid2Index.nCid);                    

    //            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, DnsServerIP[nSimID][proc->Cid2Index.nCid-1], 6);
#ifdef CFW_MULTI_SIM
                //SXS_DUMP(CFW_GPRS_TRACE, 0, sPdpCXT.pPdpAddr, sPdpCXT.nPdpAddrSize);
                if(CFW_GprsSetPdpCxt(proc->Cid2Index.nCid, &sPdpCXT,nSimID) != ERR_SUCCESS)
#else
                if(CFW_GprsSetPdpCxt(proc->Cid2Index.nCid, &sPdpCXT) != ERR_SUCCESS)
#endif
                {
                    // save the value into flash error
                    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsSetPdpCxt error, maybe the flash error \n"));                    
          		      proc->nPDPAddrSize = psInMsg->PDPAddLen - 2;
          		      SUL_MemCopy8(proc->nPDPAddr, psInMsg->PDPAddress + 2, psInMsg->PDPAddLen - 2);
          		      CFW_Dump_SXS(CFW_GPRS_TRACE_DATA, proc->nPDPAddr, proc->nPDPAddrSize);
                }
                //SXS_DUMP(CFW_GPRS_TRACE, 0, sPdpCXT.pPdpAddr, sPdpCXT.nPdpAddrSize);
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("!!!!! free    API_PDP_CTXACTIVATE_CNF : \n"));
                if(sPdpCXT.pApn != NULL)
                  CFW_FREE(sPdpCXT.pApn);
                else
                   CSW_TRACE(CFW_GPRS_TRACE, TSTXT("sPdpCXT.pApn is NULL \n"));

                
                if(sPdpCXT.pPdpAddr != NULL)
                  CFW_FREE(sPdpCXT.pPdpAddr);	
                else
                   CSW_TRACE(CFW_GPRS_TRACE, TSTXT("sPdpCXT.pPdpAddr is NULL \n"));

                
                if(sPdpCXT.pApnUser != NULL)
                   CFW_FREE(sPdpCXT.pApnUser);
                else
                   CSW_TRACE(CFW_GPRS_TRACE, TSTXT("sPdpCXT.pApnUser is NULL \n"));

                
                if(sPdpCXT.pApnPwd != NULL)
                   CFW_FREE(sPdpCXT.pApnPwd);	
                else
                   CSW_TRACE(CFW_GPRS_TRACE, TSTXT("sPdpCXT.pApnPwd is NULL \n"));

#ifdef CFW_MULTI_SIM
                //
                // should post the response
                if(proc->bAttManualAcc)  // MT manual acc
                {
                    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("post EV_CFW_GPRS_CXT_ACTIVE_RSP to up layer \n"));                                    
                    CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_ACTIVE_RSP, proc->Cid2Index.nCid, 0, nUTI|(nSimID<<24), CFW_GPRS_ACTIVED);
                }
                else
                {
                    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("post EV_CFW_GPRS_ACT_RSP to up layer active success\n"));                                    
		                CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, proc->Cid2Index.nCid, 0, nUTI|(nSimID<<24), CFW_GPRS_ACTIVED);

                   // CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, proc->Cid2Index.nCid, 0, nUTI, CFW_GPRS_ACTIVED);
                }
#else
                //
                // should post the response
                if(proc->bAttManualAcc)  // MT manual acc
                {
                    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("post EV_CFW_GPRS_CXT_ACTIVE_RSP to up layer \n"));                                    
                    CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_ACTIVE_RSP, proc->Cid2Index.nCid, 0, nUTI, CFW_GPRS_ACTIVED);
                }
                else
                {
                    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("post EV_CFW_GPRS_ACT_RSP to up layer active success\n"));                                    
                    CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, proc->Cid2Index.nCid, 0, nUTI, CFW_GPRS_ACTIVED);
                }
#endif
            		if (0)
            		{
                   UINT8 addr[4];				
      		         UINT32 Ip_Addr;
          #if 1 //TMP_GPRS_CONTEXT
                   UINT8 lenth;
#ifdef CFW_MULTI_SIM
                   CFW_GprsGetPdpAddr(proc->Cid2Index.nCid, &lenth, addr,nSimID);//modify by wuys 2008-01-21
#else
                   CFW_GprsGetPdpAddr(proc->Cid2Index.nCid, &lenth, addr);//modify by wuys 2008-01-21
#endif
          #else
                   SUL_MemCopy8(addr, proc->nPDPAddr, proc->nPDPAddrSize);
          #endif      
                   // g_ip_addr.addr = (addr[3]<<24) |(addr[2]<<16) | (addr[1]<< 8) |addr[0] ;
                   Ip_Addr = (addr[3]<<24) |(addr[2]<<16) | (addr[1]<< 8) |addr[0] ;
                      
                   CSW_TRACE(CFW_GPRS_TRACE, TSTXT("send IP address to APS task IP addr: 0x%x\n"),Ip_Addr);
            		   // send the IP addr to the APS task
            		   {
            		         COS_EVENT cos_ev;
                  			 cos_ev.nEventId = EV_INTER_APS_ACT_REQ;
                  			 cos_ev.nParam1 = proc->Cid2Index.nCid;
                  			 cos_ev.nParam2 = Ip_Addr;
                  			 cos_ev.nParam3 = CFW_GPRS_ACTIVED;
            		         BAL_SendTaskEvent(&cos_ev, CSW_TASK_ID_APS);

            		   }
            		  // CFW_SetEventProc( CFW_SRV_GPRS_EV_RSP, NULL, Gprs_MSG_Redirect );//removed by wuys 2008-04-11
                }
                CFW_SetAoState(hAo, CFW_GPRS_STATE_ACTIVE);  // change the state
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);               
                
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }
            
        }
        else if (nEventId == API_PDP_CTXACTIVATE_REJ)
        {            
            api_PdpCtxActivateRej_t  *psInMsg = (api_PdpCtxActivateRej_t *)nEvParam;
            if (proc->Cid2Index.nSmIndex == psInMsg->SmIndex)
            {
				CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_CONNECTING: receive API_PDP_CTXACTIVATE_REJ smindex = 0x%x, psInMsg->Cause = 0x%x\n"), psInMsg->SmIndex, psInMsg->Cause);
				CFW_Dump_SXS(CFW_GPRS_TRACE_DATA, (UINT8 *)psInMsg, SIZEOF(api_PdpCtxActivateRej_t));

				//Add by lixp for UTI zero bug at 20090723.
				CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
				//
				// should post the response
				if(proc->bAttManualAcc)
					CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_ACTIVE_RSP,proc->Cid2Index.nCid, psInMsg->Cause,  nUTI|(nSimID<<24), 0xF1);
				else
					CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP,proc->Cid2Index.nCid, psInMsg->Cause,  nUTI|(nSimID<<24), 0xF1);
#else
				//
				// should post the response
				if(proc->bAttManualAcc)
					CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_ACTIVE_RSP, proc->Cid2Index.nCid,psInMsg->Cause,  nUTI, 0xF1);
				else
					CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, proc->Cid2Index.nCid,psInMsg->Cause,  nUTI, 0xF1);
#endif
				CFW_SetAoState(hAo, CFW_GPRS_STATE_IDLE);
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
#ifdef CFW_MULTI_SIM

				if(!IsCidSavedInApp(proc->Cid2Index.nCid, nSimID))
				     CFW_ReleaseCID(proc->Cid2Index.nCid, nSimID);
#else
				if(!IsCidSavedInApp(proc->Cid2Index.nCid))
				     CFW_ReleaseCID(proc->Cid2Index.nCid);
#endif
				//if( ERR_SUCCESS != nRet )
				{
					//TODO..
				}
				proc->Cid2Index.nCid = 0xFF;
				// then destroy the ao
				CFW_UnRegisterAO(CFW_GPRS_SRV_ID, hAo);
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
        }
	  else if (nEventId == API_PDP_CTXDEACTIVATE_IND)//add by wuys for fix bug16960, 2010-05-24
        {            
             api_PdpCtxDeactivateInd_t   *psInMsg  = (api_PdpCtxDeactivateInd_t *)nEvParam;
            if (proc->Cid2Index.nSmIndex == psInMsg->SmIndex)
            {
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_ACTIVE: receive API_PDP_CTXDEACTIVATE_IND smindex = 0x%x, cause: 0x%x\n"), psInMsg->SmIndex,psInMsg->Cause);

                //Add by lixp for UTI zero bug at 20090723.
                CFW_GetUTI(hAo, &nUTI);

              
#ifdef CFW_MULTI_SIM
				//
				// should post the response
				if(proc->bAttManualAcc)
					CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_ACTIVE_RSP, proc->Cid2Index.nCid,psInMsg->Cause,  nUTI|(nSimID<<24), 0xF1);
				else
					CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, proc->Cid2Index.nCid,psInMsg->Cause,  nUTI|(nSimID<<24), 0xF1);
#else
				//
				// should post the response
				if(proc->bAttManualAcc)
					CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_ACTIVE_RSP, proc->Cid2Index.nCid,psInMsg->Cause,  nUTI, 0xF1);
				else
					CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, proc->Cid2Index.nCid,psInMsg->Cause,  nUTI, 0xF1);
#endif
				CFW_SetAoState(hAo, CFW_GPRS_STATE_IDLE);
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
#ifdef CFW_MULTI_SIM
				if(!IsCidSavedInApp(proc->Cid2Index.nCid, nSimID))
				     CFW_ReleaseCID(proc->Cid2Index.nCid, nSimID);
#else
				if(!IsCidSavedInApp(proc->Cid2Index.nCid))
				     CFW_ReleaseCID(proc->Cid2Index.nCid);
#endif
				//if( ERR_SUCCESS != nRet )
				{
					//TODO..
				}
				proc->Cid2Index.nCid = 0xFF;
				// then destroy the ao
				CFW_UnRegisterAO(CFW_GPRS_SRV_ID, hAo);
			}
			else
			{
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			}
        }
        else if (nEventId == API_PDP_CTXSMINDEX_IND)
        {
            api_PdpCtxSmIndexInd_t *psInMsg = (api_PdpCtxSmIndexInd_t *)nEvParam;
            if (proc->Cid2Index.nNsapi == psInMsg->NSapi)
            {
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_CONNECTING\nreceive API_PDP_CTXSMINDEX_IND = 0x%x, psInMsg->NSapi = 0x%x\n"), psInMsg->SmIndex, psInMsg->NSapi);
                proc->Cid2Index.nSmIndex = psInMsg->SmIndex;
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }            
        }
        else if (nEventId == 0xFFFFFFFF)
        {
            // in CFW_GPRS_STATE_CONNECTING send req to stack
            if (proc->nMsgId == API_PDP_CTXACTIVATE_REJ)
            {
                api_PdpCtxActivateRej_t *pOutMsg = NULL;
                pOutMsg = (api_PdpCtxActivateRej_t *)CFW_MemAllocMessageStructure(SIZEOF(api_PdpCtxActivateRej_t));
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("send api_PdpCtxActivateRej_t\n"));
                //
                pOutMsg->SmIndex = proc->Cid2Index.nSmIndex;
                pOutMsg->Cause = 0x00;
                pOutMsg->ProtocolCfgOptionLen = 0;

                //
                proc->pMsgBody = pOutMsg;
#ifdef CFW_MULTI_SIM
                CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody,nSimID);
#else
                CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody);
#endif
                CFW_SetAoState(hAo, CFW_GPRS_STATE_IDLE);// 
#ifdef CFW_MULTI_SIM
		if(!IsCidSavedInApp(proc->Cid2Index.nCid, nSimID))
		     CFW_ReleaseCID(proc->Cid2Index.nCid, nSimID);
#else
		if(!IsCidSavedInApp(proc->Cid2Index.nCid))
		   CFW_ReleaseCID(proc->Cid2Index.nCid);
#endif
	//	  if( ERR_SUCCESS != nRet )
		  {
			//TODO..
		  }		  
                // destroy the ao....
                CFW_UnRegisterAO(CFW_GPRS_SRV_ID, hAo);
            }
            else
            {
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_CONNECTING\n Error :receive proc->nMsgId = 0x%x\n"), proc->nMsgId);            
        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED); 
            }
        }
        else 
        {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_CONNECTING\n Error :receive eventid = 0x%x\n"), nEventId);            
      CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
        }
    }
    else if(nState == CFW_GPRS_STATE_DISCONNECTING)
    {
        if (nEventId == API_PDP_CTXDEACTIVATE_CNF)   // deactived
        {
        
            api_PdpCtxDeactivateCnf_t *psInMsg = (api_PdpCtxDeactivateCnf_t *)nEvParam;
            if(proc->Cid2Index.nSmIndex == psInMsg->SmIndex)
            {
                
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_DISCONNECTING: receive API_PDP_CTXDEACTIVATE_CNF smindex = 0x%x\n"), psInMsg->SmIndex);
#ifdef CFW_MULTI_SIM
                if(PdpContList[nSimID][proc->Cid2Index.nCid -1]->nPdpAddrSize >0)
                {
                  CSW_GPRS_FREE(PdpContList[nSimID][proc->Cid2Index.nCid -1]->pPdpAddr);
                }
                PdpContList[nSimID][proc->Cid2Index.nCid -1]->nPdpAddrSize = 0;
                PdpContList[nSimID][proc->Cid2Index.nCid -1]->pPdpAddr = NULL;
                if(PdpContList[nSimID][proc->Cid2Index.nCid -1]->nApnUserSize >0)
                {
                  CSW_GPRS_FREE(PdpContList[nSimID][proc->Cid2Index.nCid -1]->pApnUser);
                }
                PdpContList[nSimID][proc->Cid2Index.nCid -1]->nApnUserSize = 0;
                PdpContList[nSimID][proc->Cid2Index.nCid -1]->pApnUser = NULL;

                if(PdpContList[nSimID][proc->Cid2Index.nCid -1]->nApnPwdSize >0)
                {
                  CSW_GPRS_FREE(PdpContList[nSimID][proc->Cid2Index.nCid -1]->pApnPwd);
                }
                PdpContList[nSimID][proc->Cid2Index.nCid -1]->nApnPwdSize = 0;
                PdpContList[nSimID][proc->Cid2Index.nCid -1]->pApnPwd = NULL;

                if(PdpContList[nSimID][proc->Cid2Index.nCid -1]->nApnSize >0)
                {
                  CSW_GPRS_FREE(PdpContList[nSimID][proc->Cid2Index.nCid -1]->pApn);
                }
                PdpContList[nSimID][proc->Cid2Index.nCid -1]->nApnSize = 0;
                PdpContList[nSimID][proc->Cid2Index.nCid -1]->pApn = NULL;                
                CSW_GPRS_FREE(PdpContList[nSimID][proc->Cid2Index.nCid -1]);
                PdpContList[nSimID][proc->Cid2Index.nCid -1] = NULL;
#else
                if(PdpContList[proc->Cid2Index.nCid -1]->nPdpAddrSize >0)
                {
                  CSW_GPRS_FREE(PdpContList[proc->Cid2Index.nCid -1]->pPdpAddr);
                }
                PdpContList[proc->Cid2Index.nCid -1]->nPdpAddrSize = 0;
                PdpContList[proc->Cid2Index.nCid -1]->pPdpAddr = NULL;
                if(PdpContList[proc->Cid2Index.nCid -1]->nApnUserSize >0)
                {
                  CSW_GPRS_FREE(PdpContList[proc->Cid2Index.nCid -1]->pApnUser);
                }
                PdpContList[proc->Cid2Index.nCid -1]->nApnUserSize = 0;
                PdpContList[proc->Cid2Index.nCid -1]->pApnUser = NULL;

                if(PdpContList[proc->Cid2Index.nCid -1]->nApnPwdSize >0)
                {
                  CSW_GPRS_FREE(PdpContList[proc->Cid2Index.nCid -1]->pApnPwd);
                }
                PdpContList[proc->Cid2Index.nCid -1]->nApnPwdSize = 0;
                PdpContList[proc->Cid2Index.nCid -1]->pApnPwd = NULL;

                if(PdpContList[proc->Cid2Index.nCid -1]->nApnSize >0)
                {
                  CSW_GPRS_FREE(PdpContList[proc->Cid2Index.nCid -1]->pApn);
                }
                PdpContList[proc->Cid2Index.nCid -1]->nApnSize = 0;
                PdpContList[proc->Cid2Index.nCid -1]->pApn = NULL;    

                CSW_GPRS_FREE(PdpContList[proc->Cid2Index.nCid -1]);
                PdpContList[proc->Cid2Index.nCid -1] = NULL;
#endif
                CFW_SetAoState(hAo, CFW_GPRS_STATE_IDLE);
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
				//release nCid when the GPRS state is CFW_GPRS_DEACTIVED add by qidd@20090317

#ifdef CFW_MULTI_SIM
                if(DnsServerIP[nSimID][proc->Cid2Index.nCid -1])
                {
                  CSW_GPRS_FREE(DnsServerIP[nSimID][proc->Cid2Index.nCid -1]);
                  DnsServerIP[nSimID][proc->Cid2Index.nCid -1] = NULL;
                }

		if(!IsCidSavedInApp(proc->Cid2Index.nCid, nSimID))
		   CFW_ReleaseCID(proc->Cid2Index.nCid, nSimID);

//				        if (ERR_SUCCESS != CFW_ReleaseCID( proc->Cid2Index.nCid, nSimID))
#else
                if(DnsServerIP[proc->Cid2Index.nCid -1])
                {
                  CSW_GPRS_FREE(DnsServerIP[proc->Cid2Index.nCid -1]);
                  DnsServerIP[proc->Cid2Index.nCid -1] = NULL;
                }
		if(!IsCidSavedInApp(proc->Cid2Index.nCid))
		   CFW_ReleaseCID(proc->Cid2Index.nCid);

//				        if (ERR_SUCCESS != CFW_ReleaseCID( proc->Cid2Index.nCid))
#endif
        				{
        					//TODO...
        				}

                //Add by lixp for UTI zero bug at 20090723.
                CFW_GetUTI(hAo, &nUTI);
    			CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GprsAoProc DEACTIVED UTI: 0x%x \n") ,nUTI);

                // then destroy the ao
                CFW_UnRegisterAO(CFW_GPRS_SRV_ID, hAo);
				#ifdef CFW_MULTI_SIM
//    CFW_StartDetachTimer(nSimID);
#else
  //  CFW_StartDetachTimer();
#endif

                //
                // should post the response 
                //CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, 0, 0, nUTI, CFW_GPRS_DEACTIVED);
#ifdef CFW_MULTI_SIM
                CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, proc->Cid2Index.nCid, 0, nUTI|(nSimID<<24), CFW_GPRS_DEACTIVED); //modify by wuys 2008-06-04
#else
                CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, proc->Cid2Index.nCid, 0, nUTI, CFW_GPRS_DEACTIVED); //modify by wuys 2008-06-04
#endif
       #if 0   // remove by wuys 2008-06-04         // send the de active msg to aps task
		   {
		        COS_EVENT cos_ev;
			 cos_ev.nEventId = EV_INTER_APS_ACT_REQ;
			 cos_ev.nParam1 = proc->Cid2Index.nCid;
			 cos_ev.nParam2 = 0;
			 cos_ev.nParam3 = CFW_GPRS_DEACTIVED;
		        BAL_SendTaskEvent(&cos_ev, CSW_TASK_ID_APS);

		   }
		   #endif
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }

        }
        else if (nEventId == API_PDP_CTXDEACTIVATE_IND) // deactive ind event:
        {
            api_PdpCtxDeactivateInd_t   *psInMsg  = (api_PdpCtxDeactivateInd_t *)nEvParam;
            if (proc->Cid2Index.nSmIndex == psInMsg->SmIndex)
            {
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_DISCONNECTING: receive api_PdpCtxDeactivateInd_t smindex = 0x%x\n"), psInMsg->SmIndex);
                CFW_SetAoState(hAo, CFW_GPRS_STATE_IDLE);
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
				
#ifdef CFW_MULTI_SIM
		if(!IsCidSavedInApp(proc->Cid2Index.nCid, nSimID))
 		     CFW_ReleaseCID(proc->Cid2Index.nCid, nSimID);

 //       				if (ERR_SUCCESS != CFW_ReleaseCID( proc->Cid2Index.nCid, nSimID))
#else
		if(!IsCidSavedInApp(proc->Cid2Index.nCid))
		   CFW_ReleaseCID(proc->Cid2Index.nCid);

//        				if (ERR_SUCCESS != CFW_ReleaseCID( proc->Cid2Index.nCid))
#endif
        				{
        					//TODO...
        				}

                //Add by lixp for UTI zero bug at 20090723.
                CFW_GetUTI(hAo, &nUTI);

                CFW_UnRegisterAO(CFW_GPRS_SRV_ID, hAo);
		   				#ifdef CFW_MULTI_SIM
   // CFW_StartDetachTimer(nSimID);
#else
  //  CFW_StartDetachTimer();
#endif

                // post the ind event: 
                //
                //CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, 0, 0, nUTI, CFW_GPRS_DEACTIVED);
#ifdef CFW_MULTI_SIM
                CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_DEACTIVE_IND, proc->Cid2Index.nCid, 0, nUTI|(nSimID<<24), 0);
#else
                CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_DEACTIVE_IND, proc->Cid2Index.nCid, 0, nUTI, 0);
#endif
       #if 0   // remove by wuys 2008-06-04           // send the de active msg to aps task
		   {
		        COS_EVENT cos_ev;
			 cos_ev.nEventId = EV_INTER_APS_ACT_REQ;
			 cos_ev.nParam1 = proc->Cid2Index.nCid;
			 cos_ev.nParam2 = 0;
			 cos_ev.nParam3 = CFW_GPRS_DEACTIVED;
		        BAL_SendTaskEvent(&cos_ev, CSW_TASK_ID_APS);

		   }
       #endif
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }
            
        }
        else if (nEventId == 0xFFFFFFFF)
        {

            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_DISCONNECTING\n Error :receive proc->nMsgId = 0x%x\n"), proc->nMsgId);            
      CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
        }
        else
        {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_DISCONNECTING\n Error :receive eventid = 0x%x\n"), nEventId);            
      CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
        }
    }
    else if(nState == CFW_GPRS_STATE_ACTIVE)
    {
        if (nEventId == API_PDP_CTXDEACTIVATE_IND)  // deactive ind event
        {
            api_PdpCtxDeactivateInd_t   *psInMsg  = (api_PdpCtxDeactivateInd_t *)nEvParam;
            if (proc->Cid2Index.nSmIndex == psInMsg->SmIndex)
            {
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_ACTIVE: receive API_PDP_CTXDEACTIVATE_IND smindex = 0x%x, cause: 0x%x\n"), psInMsg->SmIndex,psInMsg->Cause);
                CFW_SetAoState(hAo, CFW_GPRS_STATE_IDLE);
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
#ifdef CFW_MULTI_SIM
		if(!IsCidSavedInApp(proc->Cid2Index.nCid, nSimID))
		   CFW_ReleaseCID(proc->Cid2Index.nCid, nSimID);

//				if (ERR_SUCCESS != CFW_ReleaseCID( proc->Cid2Index.nCid, nSimID))
#else

		if(!IsCidSavedInApp(proc->Cid2Index.nCid))
		   CFW_ReleaseCID(proc->Cid2Index.nCid);
//				if (ERR_SUCCESS != CFW_ReleaseCID( proc->Cid2Index.nCid))
#endif
				{
					//TODO...
				}

                //Add by lixp for UTI zero bug at 20090723.
                CFW_GetUTI(hAo, &nUTI);

                CFW_UnRegisterAO(CFW_GPRS_SRV_ID, hAo);
                // post the ind event: 
                //
                //CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, 0, 0, nUTI, CFW_GPRS_DEACTIVED);
  #ifdef CFW_MULTI_SIM
                CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_DEACTIVE_IND, proc->Cid2Index.nCid, 0, nUTI|(nSimID<<24), 0);
 #else
                CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_DEACTIVE_IND, proc->Cid2Index.nCid, 0, nUTI, 0);
 #endif
      #if 0   // remove by wuys 2008-06-04            // send the de active msg to aps task
		   {
		        COS_EVENT cos_ev;
			 cos_ev.nEventId = EV_INTER_APS_ACT_REQ;
			 cos_ev.nParam1 = proc->Cid2Index.nCid;
			 cos_ev.nParam2 = 0;
			 cos_ev.nParam3 = CFW_GPRS_DEACTIVED;
		        BAL_SendTaskEvent(&cos_ev, CSW_TASK_ID_APS);

		   }
      #endif
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }
            
        }
        else if (nEventId == API_PDP_CTXMODIFY_IND)
        {
            api_PdpCtxModifyInd_t   *psInMsg  = (api_PdpCtxModifyInd_t *)nEvParam;
            if (proc->Cid2Index.nSmIndex == psInMsg->SmIndex)
            {
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_ACTIVE: receive API_PDP_CTXMODIFY_IND smindex = 0x%x\n"), psInMsg->SmIndex);
                CFW_SetAoState(hAo, CFW_GPRS_STATE_MODIFYING);
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
                // record the modify info
                // post the ind event: 
                //

                //Add by lixp for UTI zero bug at 20090723.
                CFW_GetUTI(hAo, &nUTI);

#ifdef CFW_MULTI_SIM
                CFW_PostNotifyEvent(EV_CFW_GPRS_MOD_IND, proc->Cid2Index.nCid, 0, nUTI|(nSimID<<24), 0);
#else
                CFW_PostNotifyEvent(EV_CFW_GPRS_MOD_IND, proc->Cid2Index.nCid, 0, nUTI, 0);
#endif

            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            } 
        }
      	else if((nEventId == API_SYS_CTR_IND) || (nEventId == API_PS_ULCONGEST_IND))
      	{
      	    UINT8 nAoCount = 0;
            UINT8 nAoIndex = 0;
      	    // 1(1<<0) uplink congestion or low memory, 2(1<<1) uplink congestion or low memory disappear	
      	    api_FlowCtrInd_t *psInMsg = (api_FlowCtrInd_t *)nEvParam;
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("receive API_SYS_CTR_IND status = 0x%x\n"), psInMsg->status);		
            // sxr_CheckCluster();
            CFW_SetServiceId(CFW_GPRS_SRV_ID);
#ifdef CFW_MULTI_SIM	
		nAoCount = CFW_GetAoCount(CFW_GPRS_SRV_ID, nSimID);
#else
		nAoCount = CFW_GetAoCount();
#endif
      	
            for (nAoIndex = 0; nAoIndex < nAoCount; nAoIndex++)
            {
#ifdef CFW_MULTI_SIM
                hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW, CFW_GPRS_SRV_ID, nSimID);
#else
                hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW);
#endif
                proc = CFW_GetAoUserData(hAo);
                proc->nFlowCtr = psInMsg->status;
            }
      	    
      	}
        else if (nEventId == API_PSDATA_IND)
        {
            // in coming data 
            // post the data to up layer
            api_PSDataInd_t *psInMsg = (api_PSDataInd_t *)nEvParam;
            if (proc->Cid2Index.nNsapi == psInMsg->Ctrl.NSAPI)
            {
                CFW_GPRS_DATA *pData = NULL;
                UINT16 nDataLen = 0;
                nDataLen = (UINT16)(psInMsg->DataLen - psInMsg->DataOffset);
                //pData = (CFW_GPRS_DATA *)CFW_MALLOC(nDataLen + OFFSETOF(CFW_GPRS_DATA, pData));
                pData = (CFW_GPRS_DATA *)CSW_GPRS_MALLOC(nDataLen + OFFSETOF(CFW_GPRS_DATA, pData));
                if(pData == NULL)
                {
                   CSW_TRACE(CFW_GPRS_TRACE, TSTXT("received CSW_GPRS_MALLOC error\n"));
                   //COS_Sleep(20);//remove by wuys for gprs test 2008-9-03
                   return ERR_BUSY_DEVICE;
                }
                pData->nDataLength = nDataLen;
                SUL_MemCopy8(pData->pData, &(psInMsg->UserData[psInMsg->DataOffset]), psInMsg->DataLen - psInMsg->DataOffset);
                
                //dump the data for debug
                
                //CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_ACTIVE: receive API_PSDATA_IND data from stack: psInMsg->DataLen = 0x%x£¬ psInMsg->DataOffset = 0x%x\n"), psInMsg->DataLen, psInMsg->DataOffset);                
                //CFW_Dump_SXS(CFW_GPRS_TRACE, psInMsg->UserData,psInMsg->DataLen);                                  
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_ACTIVE: receive API_PSDATA_IND Nsapi = 0x%x nDataLen = 0x%x\n"), psInMsg->Ctrl.NSAPI, nDataLen);
                
                //  dump the data for debug
                
                //CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_ACTIVE: data send to uplayer: p = %p\n"), pData);                
                //CFW_Dump_SXS(CFW_GPRS_TRACE, (UINT8 *) pData, (nDataLen + (OFFSETOF(CFW_GPRS_DATA, pData))));
                //CFW_Dump_SXS(CFW_GPRS_TRACE_DATA, (UINT8 *) pData->pData, pData->nDataLength);
                
                // post the data to up layer
			
//modify by wuys 2008-09-04
       #if 0


                Gprs_TcpIp(pData,proc->Cid2Index.nCid);
                CSW_GPRS_FREE((PVOID)pData);
			 #else
       
                //Add by lixp for UTI zero bug at 20090723.
                CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
                gprs_sum[nSimID][1]+=pData->nDataLength ;
                CFW_PostNotifyEvent(EV_CFW_GPRS_DATA_IND, proc->Cid2Index.nCid,(UINT32)pData,  nUTI|(nSimID<<24), 0);                // comsumed the message 
#else
                CFW_PostNotifyEvent(EV_CFW_GPRS_DATA_IND, proc->Cid2Index.nCid,(UINT32)pData,  nUTI, 0);                // comsumed the message 
#endif
       #endif
                // comsumed the message 
                // state not changed
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }                        
        }
        else if(nEventId == 0xFFFFFFFF)
       {
            // in CFW_GPRS_STATE_ACTIVE send req to stack
            if (proc->nMsgId == API_PDP_CTXDEACTIVATE_REQ) //deactivate  act = 0;
            {
                api_PdpCtxDeactivateReq_t *pOutMsg = NULL;  
                pOutMsg = (api_PdpCtxDeactivateReq_t *)CFW_MemAllocMessageStructure(SIZEOF(api_PdpCtxDeactivateReq_t));
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("send API_PDP_CTXDEACTIVATE_REQ\n"));
                // smindex
                pOutMsg->SmIndex = proc->Cid2Index.nSmIndex;

                //cause
                pOutMsg->Cause = 0x00;  // the cause value
                

                // send the message to stack
                proc->pMsgBody = pOutMsg;
#ifdef CFW_MULTI_SIM
                CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody,nSimID);
#else
                CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody);
#endif
                CFW_SetAoState(hAo, CFW_GPRS_STATE_DISCONNECTING);

            }    
            else if (proc->nMsgId == API_PDP_CTXMODIFY_REQ)  // modify
            {
                api_PdpCtxModifyReq_t *pOutMsg = NULL;
                pOutMsg = (api_PdpCtxModifyReq_t *)CFW_MemAllocMessageStructure(SIZEOF(api_PdpCtxModifyReq_t));
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("send API_PDP_CTXMODIFY_REQ\n"));

                // // should be in active state!!!!!
                //     or modify state!!!!
                // copy the data to the msg body
                pOutMsg->SmIndex = proc->Cid2Index.nSmIndex;
                CFW_GprsQos2Api(proc->ModifyQos, pOutMsg->QoS, &pOutMsg->QoSLen);
                // send the message to stack
                proc->pMsgBody = pOutMsg;
#ifdef CFW_MULTI_SIM
                CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody,nSimID);
#else
                CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody);
#endif
                CFW_SetAoState(hAo, CFW_GPRS_STATE_MODIFYING);
            }
            else if (proc->nMsgId == API_PS_QUEUEDATA_REQ)
            {
                // send event to stack 
                // send the message to stack indicate there is one or more data in the queue .
            }
            else
            {
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_ACTIVE\n Error :receive proc->nMsgId = 0x%x\n"), proc->nMsgId);            
            }
            
        }
        else
        {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_ACTIVE: Error :receive eventid = 0x%x\n"), nEventId);            
      CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
        }
    }
    else if(nState == CFW_GPRS_STATE_MODIFYING)
    {
        // receive the modify conf
        if (nEventId == API_PDP_CTXMODIFY_REJ)
        {
            api_PdpCtxModifyRej_t *psInMsg = (api_PdpCtxModifyRej_t *)nEvParam;
            if (proc->Cid2Index.nSmIndex == psInMsg->SmIndex)
            {
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_MODIFYING: receive API_PDP_CTXMODIFY_REJ smindex = 0x%x\n"), psInMsg->SmIndex);

                CFW_SetAoState(hAo, CFW_GPRS_STATE_ACTIVE);
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
                
                //Add by lixp for UTI zero bug at 20090723.
                CFW_GetUTI(hAo, &nUTI);

                
#ifdef CFW_MULTI_SIM
                // should post to the uplayer : modify error
                CFW_PostNotifyEvent(EV_CFW_GPRS_MOD_RSP,proc->Cid2Index.nCid, ERR_CME_OPERATION_NOT_ALLOWED,  nUTI|(nSimID<<24), 0xF0);
#else
                CFW_PostNotifyEvent(EV_CFW_GPRS_MOD_RSP,proc->Cid2Index.nCid, ERR_CME_OPERATION_NOT_ALLOWED,  nUTI, 0xF0);

#endif

            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }
        }
        else if (nEventId == API_PDP_CTXMODIFY_CNF)
        {
            api_PdpCtxModifyCnf_t *psInMsg = (api_PdpCtxModifyCnf_t *)nEvParam;
            if (proc->Cid2Index.nSmIndex == psInMsg->SmIndex)
            {
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_MODIFYING: receive API_PDP_CTXMODIFY_CNF smindex = 0x%x\n"), psInMsg->SmIndex);
                CFW_SetAoState(hAo, CFW_GPRS_STATE_ACTIVE);
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
                
                //Add by lixp for UTI zero bug at 20090723.
                CFW_GetUTI(hAo, &nUTI);
#ifdef CFW_MULTI_SIM
                // should post to the uplayer : modify ok
                CFW_PostNotifyEvent(EV_CFW_GPRS_MOD_RSP, proc->Cid2Index.nCid, 0, nUTI|(nSimID<<24), 0 );
#else
                CFW_PostNotifyEvent(EV_CFW_GPRS_MOD_RSP, proc->Cid2Index.nCid, 0, nUTI, 0 );

#endif
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }
        }
        else if (nEventId == API_PDP_CTXDEACTIVATE_IND)
        {
            api_PdpCtxDeactivateInd_t   *psInMsg  = (api_PdpCtxDeactivateInd_t *)nEvParam;
            if (proc->Cid2Index.nSmIndex == psInMsg->SmIndex)
            {
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_MODIFYING: receive API_PDP_CTXDEACTIVATE_IND smindex  0x%x, Cause %x\n"), psInMsg->SmIndex, psInMsg->Cause);

                CFW_SetAoState(hAo, CFW_GPRS_STATE_IDLE);
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
#ifdef CFW_MULTI_SIM
		if(!IsCidSavedInApp(proc->Cid2Index.nCid, nSimID))
		   CFW_ReleaseCID(proc->Cid2Index.nCid, nSimID);

//        				if (ERR_SUCCESS != CFW_ReleaseCID( proc->Cid2Index.nCid, nSimID))
#else
		if(!IsCidSavedInApp(proc->Cid2Index.nCid))
		   CFW_ReleaseCID(proc->Cid2Index.nCid);

//        				if (ERR_SUCCESS != CFW_ReleaseCID( proc->Cid2Index.nCid))
#endif
        				{
        					//TODO...
        				}
                //Add by lixp for UTI zero bug at 20090723.
                CFW_GetUTI(hAo, &nUTI);
                
                CFW_UnRegisterAO(CFW_GPRS_SRV_ID, hAo);
                // post the ind event: 
                // add the ind 
                //CFW_PostNotifyEvent(EV_CFW_GPRS_ACT_RSP, 0, 0, nUTI, CFW_GPRS_DEACTIVED);
#ifdef CFW_MULTI_SIM
                CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_DEACTIVE_IND, proc->Cid2Index.nCid, 0, nUTI|(nSimID<<24), 0);
#else
                CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_DEACTIVE_IND, proc->Cid2Index.nCid, 0, nUTI, 0);

#endif
                #if 0   // remove by wuys 2008-06-04  // send the de active msg to aps task
        		    {
            		   COS_EVENT cos_ev;
            			 cos_ev.nEventId = EV_INTER_APS_ACT_REQ;
            			 cos_ev.nParam1 = proc->Cid2Index.nCid;
            			 cos_ev.nParam2 = 0;
            			 cos_ev.nParam3 = CFW_GPRS_DEACTIVED;
            		   BAL_SendTaskEvent(&cos_ev, CSW_TASK_ID_APS);

        		    }
                #endif
            }
            else
            {
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }
        }
        else if (nEventId == 0xFFFFFFFF)
        {
            // in CFW_GPRS_STATE_MODIFYING send deactive req to stack
            if (proc->nMsgId == API_PDP_CTXDEACTIVATE_REQ) //deactivate  act = 0;
            {
                api_PdpCtxDeactivateReq_t *pOutMsg = NULL;  
                pOutMsg = (api_PdpCtxDeactivateReq_t *)CFW_MemAllocMessageStructure(SIZEOF(api_PdpCtxDeactivateReq_t));
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("send API_PDP_CTXDEACTIVATE_REQ\n"));
                // smindex
                pOutMsg->SmIndex = proc->Cid2Index.nSmIndex;

                //cause
                pOutMsg->Cause = 0x00;  // the cause value

                // send the message to stack
                proc->pMsgBody = pOutMsg;
#ifdef CFW_MULTI_SIM
                CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody,nSimID);
#else
                CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody);

#endif
                CFW_SetAoState(hAo, CFW_GPRS_STATE_DISCONNECTING);

            }    
            else if (proc->nMsgId == API_PDP_CTXMODIFY_ACC)
            {
                api_PdpCtxModifyAcc_t *pOutMsg = NULL;                    
                pOutMsg = (api_PdpCtxModifyAcc_t *)CFW_MemAllocMessageStructure(SIZEOF(api_PdpCtxModifyAcc_t));

                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("send API_PDP_CTXMODIFY_ACC\n"));
                

                //
                // copy the data to the msg body
                pOutMsg->SmIndex = proc->Cid2Index.nSmIndex;
                // send the message to stack
                proc->pMsgBody = pOutMsg;
#ifdef CFW_MULTI_SIM
                CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody,nSimID);
#else
                CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody);

#endif
                CFW_SetAoState(hAo, CFW_GPRS_STATE_MODIFYING);// ????
            }
            else if (proc->nMsgId == API_PDP_CTXMODIFY_REJ)
            {
                api_PdpCtxModifyRej_t *pOutMsg = NULL;
                pOutMsg = (api_PdpCtxModifyRej_t *)CFW_MemAllocMessageStructure(SIZEOF(api_PdpCtxModifyRej_t));
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("send API_PDP_CTXMODIFY_REJ\n"));
                
                //
                // copy the data to the msg body
                pOutMsg->SmIndex = proc->Cid2Index.nSmIndex;
                pOutMsg->Cause = 0x00; // reject cause
                // send the message to stack
                proc->pMsgBody = pOutMsg;
#ifdef CFW_MULTI_SIM
                CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody,nSimID);
#else
                CFW_SendSclMessage(proc->nMsgId, proc->pMsgBody);
#endif
                CFW_SetAoState(hAo, CFW_GPRS_STATE_ACTIVE);// ????
            }
            else
            {
                CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_MODIFYING\n Error :receive proc->nMsgId = 0x%x\n"), proc->nMsgId);            
            }

        }
        else
        {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_MODIFYING\n Error :receive eventid = 0x%x\n"), nEventId);            
        }

    }
    else // error state
    {
    }
    return ERR_SUCCESS;
}
//by frank for no tcpip
#if 0
//add by wuys for test gprs 2008-09-05
void Gprs_TcpIp(CFW_GPRS_DATA *pData ,UINT32 cid);

void handle_TCP_data(VOID *nEvParam)
{
      CFW_GPRS_DATA *pData = NULL;
      UINT16 nDataLen = 0;
      api_PSDataInd_t *psInMsg = (api_PSDataInd_t *)nEvParam;

      
      nDataLen = (UINT16)(psInMsg->DataLen - psInMsg->DataOffset);
      pData = (CFW_GPRS_DATA *)CSW_GPRS_MALLOC(nDataLen + OFFSETOF(CFW_GPRS_DATA, pData));
      if(pData == NULL)
      {
         CSW_TRACE(CFW_GPRS_TRACE, TSTXT("received CSW_GPRS_MALLOC error\n"));
         //COS_Sleep(20);//remove by wuys for gprs test 2008-9-03
         //return ERR_BUSY_DEVICE;
      }
      pData->nDataLength = nDataLen;
      SUL_MemCopy8(pData->pData, &(psInMsg->UserData[psInMsg->DataOffset]), psInMsg->DataLen - psInMsg->DataOffset);
      
      //dump the data for debug
      
      //CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GPRS_STATE_ACTIVE: receive API_PSDATA_IND data from stack: psInMsg->DataLen = 0x%x£¬ psInMsg->DataOffset = 0x%x\n"), psInMsg->DataLen, psInMsg->DataOffset);                
      //CFW_Dump_SXS(CFW_GPRS_TRACE, psInMsg->UserData,psInMsg->DataLen);                                  
      CSW_TRACE(CFW_GPRS_TRACE, TSTXT("handle_TCP_data: receive API_PSDATA_IND Nsapi = 0x%x nDataLen = 0x%x\n"), psInMsg->Ctrl.NSAPI, nDataLen);

      Gprs_TcpIp(pData,1);
      CSW_GPRS_FREE((PVOID)pData);
   
}
#endif
//add end
// process the MT activation
// register a dynamic ao and post ind message to up layer
// Ind: EV_CFW_GPRS_CXT_ACTIVE_IND
// the parameter of the ind ??? should add more?
HAO GprsMTAoProc(CFW_EV* pEvent)
{
    UINT8  nCID = 0;
    HANDLE hSm;   
    GPRS_SM_INFO* proc=NULL;
    api_PdpCtxActivateInd_t  *psInMsg = NULL;
    CFW_GPRS_PDPCONT_INFO sPdpCXT;

#ifdef CFW_MULTI_SIM	
    CFW_SIM_ID nSimID = pEvent->nFlag;
#endif
    proc=(GPRS_SM_INFO*)CFW_MALLOC(SIZEOF(GPRS_SM_INFO)); //  + 80
    if(proc == NULL)
        return ERR_NO_MORE_MEMORY;
    
    SUL_ZeroMemory8(proc, SIZEOF(GPRS_SM_INFO)); // +80
    psInMsg = (api_PdpCtxActivateInd_t *)pEvent->nParam1;
    //
#ifdef CFW_MULTI_SIM	
     // get free cid from active ao modify by qidd @20090317
    if (ERR_SUCCESS != CFW_GetFreeCID(& nCID, nSimID))
#else
    if (ERR_SUCCESS != CFW_GetFreeCID(& nCID))
#endif
    {
    	CSW_GPRS_FREE( proc );
    	return ERR_CFW_NOT_OPERATOR_ID;
    }
    proc->Cid2Index.nCid = nCID;
    proc->Cid2Index.nNsapi = proc->Cid2Index.nCid + 5;
    proc->Cid2Index.nSmIndex = psInMsg->SmIndex;
    
    // APN
    sPdpCXT.nApnSize= psInMsg->APNLen;
    sPdpCXT.pApn = (UINT8 *)CFW_MALLOC(sPdpCXT.nApnSize);
    if(sPdpCXT.pApn == NULL)
	{
		CSW_GPRS_FREE( proc );
		return ERR_NO_MORE_MEMORY;
	}
    SUL_MemCopy8(sPdpCXT.pApn, psInMsg->AccessPointName, psInMsg->APNLen);
    
    // PDP Address
    sPdpCXT.pPdpAddr = (UINT8 *)CFW_MALLOC(psInMsg->PDPAddLen - 2);
    if(sPdpCXT.pPdpAddr == NULL)
    	{
		CSW_GPRS_FREE( proc );
		CSW_GPRS_FREE( sPdpCXT.pApn );
		 return ERR_NO_MORE_MEMORY;
	}
       
    
    CFW_GprsPdpApi2Addr(&sPdpCXT, psInMsg->PDPAddLen, psInMsg->PDPAddress);
#ifdef CFW_MULTI_SIM
    if(CFW_GprsSetPdpCxt(proc->Cid2Index.nCid, &sPdpCXT,nSimID) != ERR_SUCCESS)  // save the apn and pddress in flash
#else
    if(CFW_GprsSetPdpCxt(proc->Cid2Index.nCid, &sPdpCXT) != ERR_SUCCESS)  // save the apn and pddress in flash
#endif
	{
        CSW_TRACE(CFW_GPRS_TRACE, TSTXT("API_PDP_CTXACTIVATE_IND save the apn ,address in flash error \n"));
    }
    CFW_FREE(sPdpCXT.pApn); // free the apn pointer
    CFW_FREE(sPdpCXT.pPdpAddr);
    proc->nActState = CFW_GPRS_DEACTIVED;

    //set the nCid by qidd @20090317
#ifdef CFW_MULTI_SIM
    if (ERR_SUCCESS != CFW_SetCID(proc->Cid2Index.nCid, nSimID))
#else
    if (ERR_SUCCESS != CFW_SetCID(proc->Cid2Index.nCid))
#endif
    {
        return ERR_CFW_INVALID_PARAMETER;
    }
#ifdef CFW_MULTI_SIM
    hSm = CFW_RegisterAo(CFW_GPRS_SRV_ID, proc, CFW_GprsAoProc, nSimID);
    CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_ACTIVE_IND, proc->Cid2Index.nCid, 0, 0|(nSimID<<24), 0);
#else
    hSm = CFW_RegisterAo(CFW_GPRS_SRV_ID, proc, CFW_GprsAoProc);
    CFW_PostNotifyEvent(EV_CFW_GPRS_CXT_ACTIVE_IND, proc->Cid2Index.nCid, 0, 0, 0);
#endif
    CFW_SetAoState(hSm, CFW_GPRS_STATE_IDLE);
    //
    // post the ind message to up layer  ????  // include cid ,uti, and qos, ....
    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("GprsMTAoProc: receive API_PDP_CTXACTIVATE_IND \n"));
    return hSm;
    
}

//AT+CGPADDR
//get the address of the selected Pdp context of the gprs network.
//[in] nCid [1 - 7]
//[in/out] nLength: 
// [in] nLength should be the length that malloc the pdpAdd. 
// [out] nLength should be the length of the Pdp address. 
//[out]pPdpAdd: Point to the buffer to hold the PDP address 
// return: ERR_CFW_INVALID_PARAMETER, ERR_CFW_GPRS_INVALID_CID
// response: None
// remark:
#ifdef CFW_MULTI_SIM
UINT32 CFW_GprsGetPdpAddr(UINT8 nCid, UINT8 *nLength, UINT8 *pPdpAdd, CFW_SIM_ID nSimID)
#else
UINT32 CFW_GprsGetPdpAddr(UINT8 nCid, UINT8 *nLength, UINT8 *pPdpAdd)
#endif
{  
    GPRS_SM_INFO * proc = NULL;     //  sm info
    HAO hAo = 0;                  //  ao
    UINT8 nAoCount = 0;
    UINT8 nAoIndex = 0;
    CFW_GPRS_PDPCONT_INFO sPdpCXT;
    UINT32  ret = ERR_SUCCESS;	
    
    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsGetPdpAddr cid = 0x%x\n"), nCid);
    if((nLength == NULL) || (pPdpAdd == NULL))
        return ERR_CFW_INVALID_PARAMETER;
    if ((nCid > 7) || (nCid < 1))
    {
        return ERR_CFW_INVALID_PARAMETER; 
    }
#ifdef CFW_MULTI_SIM
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsGetPdpAddr()\r\n"));	

		return nRet;
	}
#endif
    sPdpCXT.pApn = (UINT8 *)CFW_MALLOC(100);  // the max length of the APN is 100
    sPdpCXT.pPdpAddr = (UINT8 *)CFW_MALLOC(18);  // the max length of the PDPADDR is 18
    sPdpCXT.pApnUser= (UINT8 *)CFW_MALLOC(20);  // add by wuys 2008-02-25
    sPdpCXT.pApnPwd= (UINT8 *)CFW_MALLOC(20);  // add by wuys 2008-02-25

  if((sPdpCXT.pApn == NULL) || (sPdpCXT.pPdpAddr == NULL)|| (sPdpCXT.pApnUser == NULL)|| (sPdpCXT.pApnPwd == NULL))
        return ERR_NO_MORE_MEMORY;
    
    CFW_SetServiceId(CFW_GPRS_SRV_ID);
#ifdef CFW_MULTI_SIM	
    nAoCount = CFW_GetAoCount(CFW_GPRS_SRV_ID, nSimID);
#else
    nAoCount = CFW_GetAoCount();
#endif
    for (nAoIndex = 0; nAoIndex < nAoCount; nAoIndex++)
    {
#ifdef CFW_MULTI_SIM	
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW,CFW_GPRS_SRV_ID, nSimID);
#else
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW);
#endif
        proc = CFW_GetAoUserData(hAo);
        if (proc->Cid2Index.nCid == nCid)
        {
          break;
        }
    }
    if(nAoIndex == nAoCount)
    {
    CFW_FREE(sPdpCXT.pApn);
    CFW_FREE(sPdpCXT.pPdpAddr);	
    CFW_FREE(sPdpCXT.pApnUser);
    CFW_FREE(sPdpCXT.pApnPwd);	
      
      return ERR_CFW_INVALID_PARAMETER;
    }
#ifdef CFW_MULTI_SIM
    ret = CFW_GprsGetPdpCxt(proc->Cid2Index.nCid, &sPdpCXT, nSimID);
#else
    ret = CFW_GprsGetPdpCxt(proc->Cid2Index.nCid, &sPdpCXT);
#endif
    if(ret != ERR_SUCCESS)
    {  
        CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsGetPdpCxt = 0x%x, maybe the flash error \n"), ret);
        *nLength = proc->nPDPAddrSize;
        SUL_MemCopy8(pPdpAdd, proc->nPDPAddr, *nLength);       
        CFW_Dump_SXS(CFW_GPRS_TRACE, proc->nPDPAddr, proc->nPDPAddrSize);				
      
    }
    else
    {
        *nLength = sPdpCXT.nPdpAddrSize;
        SUL_MemCopy8(pPdpAdd, sPdpCXT.pPdpAddr, *nLength);              
    }
    
    CFW_FREE(sPdpCXT.pApn);
    CFW_FREE(sPdpCXT.pPdpAddr);	
    CFW_FREE(sPdpCXT.pApnUser);
    CFW_FREE(sPdpCXT.pApnPwd);	
    return ret;  // there is no pdp address according to the cid
}

// get activate state according to the cid
// [in] nCid [1 - 7]
// [out] pState:
// CFW_GPRS_DEACTIVED 0 deactivated 
// CFW_GPRS_ACTIVED 1 activated 
// return: ERR_CFW_INVALID_PARAMETER, ERR_CFW_GPRS_INVALID_CID
#ifdef CFW_MULTI_SIM
UINT32 CFW_GetGprsActState (UINT8 nCid, UINT8* pState, CFW_SIM_ID nSimID)
#else
UINT32 CFW_GetGprsActState (UINT8 nCid, UINT8* pState)
#endif
{
    GPRS_SM_INFO * proc = NULL;     //  sm info
    HAO hAo = 0;                  //  ao
    UINT8 nAoCount = 0;
    UINT8 nAoIndex = 0;

//    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GetGprsActState cid = 0x%x\n"), nCid);
    if ((pState == NULL) || (nCid > 7) || (nCid < 1))
    {
        return ERR_CFW_INVALID_PARAMETER;
    }
#ifdef CFW_MULTI_SIM
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GetGprsActState()\r\n"));	

		return nRet;
	}
#endif
    // get ao handle according to tha cid
    //
    CFW_SetServiceId(CFW_GPRS_SRV_ID);
#ifdef CFW_MULTI_SIM	
    nAoCount = CFW_GetAoCount(CFW_GPRS_SRV_ID, nSimID);
#else
    nAoCount = CFW_GetAoCount();
#endif
    for (nAoIndex = 0; nAoIndex < nAoCount; nAoIndex++)
    {
#ifdef CFW_MULTI_SIM	
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW, CFW_GPRS_SRV_ID, nSimID);
#else
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW);
#endif
        proc = CFW_GetAoUserData(hAo);
        if (proc->Cid2Index.nCid == nCid)
        {
            *pState = proc->nActState;
            return ERR_SUCCESS;
        }
    }
    *pState = CFW_GPRS_DEACTIVED;
    return ERR_SUCCESS;  // there is no pdp context according to the cid
}


// AT+CGACT
// PDP context activate or deactivate
// nState:
// CFW_GPRS_DEACTIVED 0 deactivated 
// CFW_GPRS_ACTIVED 1 activated 
// event:EV_CFW_GPRS_ACT_NOTIFY
// parameter1:cid[1 - 7]
// parameter2:0
// type: 0 or 1 to distinguish the active or deactive or 0xF0 to distinguish the rejection.

// return : ERR_CFW_GPRS_HAVE_ACTIVED
// ERR_CFW_UTI_IS_BUSY
// ERR_CFW_GPRS_INVALID_CID
#ifdef CFW_MULTI_SIM
UINT32 CFW_GprsAct(UINT8 nState, UINT8 nCid, UINT16 nUTI, CFW_SIM_ID nSimID)     // [in] nCid  // notify: success or fail
#else
UINT32 CFW_GprsAct(UINT8 nState, UINT8 nCid, UINT16 nUTI)     // [in] nCid  // notify: success or fail
#endif
{
    #ifdef CFW_MULTI_SIM
	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsAct: GprsCidSavedInApp[0x%x]: 0x%x\r\n"),nSimID,GprsCidSavedInApp[nSimID]);
#else
	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsAct: GprsCidSavedInApp: 0x%x\r\n"),GprsCidSavedInApp);
#endif
    GPRS_SM_INFO * proc = NULL;     //  sm info
    HAO hAo = 0;                  //  ao
    UINT8 nAoCount = 0;
    UINT8 nAoIndex = 0;
    UINT8 nAttstate = 0xFF;
    gprs_SetGprsSum(gprs_sum[nSimID][0], gprs_sum[nSimID][1], nSimID );

    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsAct cid = 0x%x, state = 0x%x, nUTI = 0x%x\n"), nCid, nState, nUTI);
    if((nCid > 7) || (nCid < 1))
    {
    	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR !CID is INVALID\r\n"));	
        return ERR_CFW_INVALID_PARAMETER;
    }
    if ((nState != CFW_GPRS_DEACTIVED) && (nState != CFW_GPRS_ACTIVED))
    {
    	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR !ACTIVED OR DEACTIVE STATUS error\r\n"));	
        return ERR_CFW_INVALID_PARAMETER;
    }
    if (CFW_IsFreeUTI(nUTI, CFW_GPRS_SRV_ID) != ERR_SUCCESS)
    {
    	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! UTI IS BUSY\r\n"));	
        return ERR_CFW_UTI_IS_BUSY;
    }

     UINT32 nRet = ERR_SUCCESS;
#ifdef CFW_MULTI_SIM
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsAct()\r\n"));	

		return nRet;
	}
    CFW_GetGprsAttState(&nAttstate, nSimID);
#else
    CFW_GetGprsAttState(&nAttstate);
#endif

#if 0
//add for test
	HAO s_hAo = 0;              // staitc ao
    NW_SM_INFO *s_proc = NULL;  // static sm
//add end
#ifdef CFW_MULTI_SIM
        s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
	CFW_SetServiceId(CFW_NW_SRV_ID);
        s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif
        s_proc = CFW_GetAoUserData(s_hAo);
	 if((nAttstate == CFW_GPRS_DETACHED)&&(
	 	(API_P_ILLEGAL_USR == s_proc->sGprsStatusInfo.nCause)||
	 	(API_P_SRVC_NOT_ALLOWD == s_proc->sGprsStatusInfo.nCause)))
	 {
	 	 CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !API_NW_NO_SVCE  == s_proc->sGprsStatusInfo.nStatus \n") );
	 	 return ERR_CME_OPERATION_NOT_ALLOWED;
	 }else  if(nAttstate == 0xFF)
       {
	 	 CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !API_NW_NO_SVCE  == s_proc->sGprsStatusInfo.nStatus \n") );
	 	 return ERR_CME_OPERATION_NOT_ALLOWED;
	 }

#else



    if(nAttstate == CFW_GPRS_DETACHED)
    {
    	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! nAttstate == CFW_GPRS_DETACHED\r\n"));	
        return ERR_CME_OPERATION_NOT_ALLOWED;
    }
	
	//add ,do active need check PS status, do deactive need not //2009-11-02
	HAO s_hAo = 0;              // staitc ao
    NW_SM_INFO *s_proc = NULL;  // static sm

#ifdef CFW_MULTI_SIM
        s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
	CFW_SetServiceId(CFW_NW_SRV_ID);
        s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif
        s_proc = CFW_GetAoUserData(s_hAo);

	if( (nState == CFW_GPRS_ACTIVED)&&(s_proc->sGprsStatusInfo.nStatus ==CFW_NW_STATUS_NOTREGISTERED_NOTSEARCHING ))
    {
       	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! PS status is NO SERVICE\r\n"));	
        return ERR_CME_OPERATION_NOT_ALLOWED;
    }

//add end
	
	
#endif  
    // get ao handle according to tha cid
    //
    CFW_SetServiceId(CFW_GPRS_SRV_ID);

#ifdef CFW_MULTI_SIM	
    nAoCount = CFW_GetAoCount(CFW_GPRS_SRV_ID, nSimID);
#else
    nAoCount = CFW_GetAoCount();
#endif
    for (nAoIndex = 0; nAoIndex < nAoCount; nAoIndex++)
    {
#ifdef CFW_MULTI_SIM	
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW, CFW_GPRS_SRV_ID, nSimID);
#else
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW);
#endif
        proc = CFW_GetAoUserData(hAo);
        if (proc->Cid2Index.nCid == nCid)
        {
            if (proc->nActState == nState)
            {
                if(nState == CFW_GPRS_ACTIVED)
                {
                	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR !HAVE ACTIVED\r\n"));	
                    return ERR_CFW_GPRS_HAVE_ACTIVED; // already actived or deactived should be replaced by macro
                }
				if(nState == CFW_GPRS_DEACTIVED)
                {
                	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR !HAVE DEACTIVED\r\n"));	
                    return ERR_CFW_GPRS_HAVE_DEACTIVED; // should be ERR_CFW_GPRS_HAVE_DEACTIVED
                }
            }
            else
            {
                if (nState == CFW_GPRS_DEACTIVED)  // deactive
                {
                 UINT8  nActUTI = 0;    
                  // send deactive message to stack
                  proc->nMsgId = API_PDP_CTXDEACTIVATE_REQ;
                  proc->pMsgBody = NULL;                    
                  CSW_TRACE(CFW_GPRS_TRACE, TSTXT("send deactive req, ao proc cont : 0x%x\n"),nUTI);
 
		    CFW_GetUsingUTI(hAo,&nActUTI);
		    CFW_RelaseUsingUTI(0, nActUTI);
                 CFW_SetUTI(hAo, nUTI, 0);

                  nRet = CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom
                   CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_act  CFW_SetAoProcCode : 0x%x\n"),nRet);

                  CSW_ASSERT(nRet==ERR_SUCCESS);

                  return ERR_SUCCESS;
                }
                else // should not be CFW_GPRS_ACTIVED ? maybe the MT exist.
                {
                //remove by wuys 2010-04-29 for patching a QQ bug
                /*
                  proc->nMsgId = API_PDP_CTXACTIVATE_REQ;
                  proc->pMsgBody = NULL;
                  proc->bAttManualAcc = TRUE;
                  proc->nFlowCtr = FLOW_RELEASED; //  2 uplink congestion or low memory disappear
                  //         proc->nFlowCtr = 1; //  1 uplink congestion or low memory disappear
                  CSW_TRACE(CFW_GPRS_TRACE, TSTXT("should not be CFW_GPRS_ACTIVED,  maybe the MT exist\n"));
                  CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);
*/
                  return ERR_SUCCESS;
                }
                
            }
        }
    }
    // should be active here
    if (nState == CFW_GPRS_ACTIVED)   // attach
    {
        proc = (GPRS_SM_INFO*)CFW_MALLOC(SIZEOF(GPRS_SM_INFO));
        if (proc == NULL)   // malloc error
        {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("malloc exception\n"));
            return ERR_CME_MEMORY_FULL;  // should be updated by macro
        }
        
        // Clear the proc structure, ensure that there is not existing un-intialized member.
        SUL_ZeroMemory32(proc, SIZEOF(GPRS_SM_INFO));
        
        //get the context according to the ncid
        //should copy the context to msgbody
        //set nCid when the GPRS state is CFW_GPRS_ACTIVED add by qidd@20090317
#ifdef CFW_MULTI_SIM
		if (ERR_SUCCESS != CFW_SetCID(nCid, nSimID))
#else
		if (ERR_SUCCESS != CFW_SetCID(nCid))
#endif
		{
			CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR !SET CID INVALID\r\n"));	
			return ERR_CFW_INVALID_PARAMETER;
		}
        proc->nMsgId = API_PDP_CTXACTIVATE_REQ;
        proc->pMsgBody = NULL;
        proc->Cid2Index.nCid = nCid;
        proc->Cid2Index.nNsapi = nCid + 4;
        proc->Cid2Index.nSmIndex = 0x0;
        proc->nActState = CFW_GPRS_DEACTIVED;  // ???  // shoud find the ao according to the cid???
        proc->bAttManualAcc = FALSE;
	      proc->nFlowCtr = FLOW_RELEASED;    //   2 uplink congestion or low memory disappear
	//      	      proc->nFlowCtr = 1;    //   1 uplink congestion or low memory disappear

        CSW_TRACE(CFW_GPRS_TRACE, TSTXT("register ao and continue CFW_SetUTI : 0x%x\n"),nUTI);
#ifdef CFW_MULTI_SIM
        hAo = CFW_RegisterAo(CFW_GPRS_SRV_ID, proc, CFW_GprsAoProc,nSimID);
#else
        hAo = CFW_RegisterAo(CFW_GPRS_SRV_ID, proc, CFW_GprsAoProc);
#endif
        CFW_SetUTI(hAo, nUTI, 0);
        nRet = CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom
        CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_act  CFW_SetAoProcCode : 0x%x\n"),nRet);

        CSW_ASSERT(nRet==ERR_SUCCESS);
	  #ifdef CFW_MULTI_SIM
    CFW_StopDetachTimer(nSimID);
#else
    CFW_StopDetachTimer();
#endif

    }
    else   // detach
    {
    	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("warring! if(nState == CFW_GPRS_ACTIVED)\r\n"));
        // error; should not be here
    }    
    
    return ERR_SUCCESS;
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_GprsActEx(UINT8 nState, UINT8 nCid, UINT16 nUTI, CFW_SIM_ID nSimID,BOOL SavedInApp)     // [in] nCid  // notify: success or fail
{
	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("muti sim CFW_GprsActEx,SavedInApp: 0x%x \r\n"),SavedInApp);

	if(SavedInApp)
	      GprsCidSavedInApp[nSimID] |= (0x01 << nCid);

       return CFW_GprsAct( nState, nCid, nUTI,  nSimID);     // [in] nCid  // notify: success or fail
}
#else
UINT32 CFW_GprsActEx(UINT8 nState, UINT8 nCid, UINT16 nUTI,BOOL SavedInApp)     // [in] nCid  // notify: success or fail
{
	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsActEx,SavedInApp: 0x%x \r\n"),SavedInApp);

	if(SavedInApp)
       	GprsCidSavedInApp |= (0x01 << nCid);
	
       return CFW_GprsAct( nState,  nCid,  nUTI) ;    // [in] nCid  // notify: success or fail
}
#endif

//
// MT
// Manual acceptance of a network request for PDP context activation 'A'
// parameter1: 0
// parameter2: 0
// return: ERR_CFW_GPRS_INVALID_CID
// ERR_CFW_UTI_IS_BUSY
// event: EV_CFW_GPRS_CXT_ACTIVE_IND_NOTIFY
// parameter1: cid [1 - 7]
// parameter2: 0
// UTI:uti
// type: 0 or 0xF0 to distinguish the confirm or reject the request.
#ifdef CFW_MULTI_SIM
UINT32 CFW_GprsManualAcc(UINT16 nUTI, UINT8 nCid, CFW_SIM_ID nSimID)
#else
UINT32 CFW_GprsManualAcc(UINT16 nUTI, UINT8 nCid)
#endif
{
    GPRS_SM_INFO * proc = NULL;     //  sm info
    HAO hAo = 0;                  //  ao
    UINT8 nAoCount = 0;
    UINT8 nAoIndex = 0;
    UINT8 nAttstate = 0xFF;
          
    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsManualAcc cid = 0x%x, nUTI = 0x%x\n"), nCid, nUTI);
    if((nCid > 7) || (nCid < 1))
        return ERR_CFW_INVALID_PARAMETER;
    if (CFW_IsFreeUTI(nUTI, CFW_GPRS_SRV_ID) != ERR_SUCCESS)
    {
        return ERR_CFW_UTI_IS_BUSY;
    }
#ifdef CFW_MULTI_SIM
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsManualAcc()\r\n"));	

		return nRet;
	}
    CFW_GetGprsAttState(&nAttstate,nSimID);
#else
    CFW_GetGprsAttState(&nAttstate);
#endif
    if(nAttstate == CFW_GPRS_DETACHED)
    {
        return ERR_CME_OPERATION_NOT_ALLOWED;
    }
    CFW_SetServiceId(CFW_GPRS_SRV_ID);

#ifdef CFW_MULTI_SIM	
    nAoCount = CFW_GetAoCount(CFW_GPRS_SRV_ID, nSimID);
#else
    nAoCount = CFW_GetAoCount();
#endif

    for (nAoIndex = 0; nAoIndex < nAoCount; nAoIndex++)
    {
#ifdef CFW_MULTI_SIM	
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW, CFW_GPRS_SRV_ID, nSimID);
#else
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW);
#endif
        proc = CFW_GetAoUserData(hAo);
        if (proc->Cid2Index.nCid == nCid)
        {    
            if(proc->nActState == CFW_GPRS_ACTIVED)
            {
                return ERR_CFW_GPRS_HAVE_ACTIVED;
            }
            proc->nMsgId = API_PDP_CTXACTIVATE_REQ;
            proc->pMsgBody = NULL;
            
            proc->bAttManualAcc = TRUE;
            //SUL_MemCopy8(&proc->ModifyQos, Qos, SIZEOF(CFW_GPRS_QOS));

            CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom
            return ERR_SUCCESS;
        }
    }
    return ERR_CFW_GPRS_INVALID_CID;   // there is no context according to the cid;    
}

// ATH
// Manual rejection of a network request for PDP context activation 'H'
// parameter1: None 
// parameter2: None
// return: ERR_CFW_GPRS_INVALID_CID
// ERR_CFW_UTI_IS_BUSY
// response: None
#ifdef CFW_MULTI_SIM
UINT32 CFW_GprsManualRej(UINT16 nUTI, UINT8 nCid, CFW_SIM_ID nSimID)
#else
UINT32 CFW_GprsManualRej(UINT16 nUTI, UINT8 nCid)
#endif
{
    GPRS_SM_INFO * proc = NULL;     //  sm info
    HAO hAo = 0;                  //  ao
    UINT8 nAoCount = 0;
    UINT8 nAoIndex = 0;
          
    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsManualRej cid = 0x%x, nUTI = 0x%x\n"), nCid, nUTI);
    if((nCid > 7) || (nCid < 1))
        return ERR_CFW_INVALID_PARAMETER;
    if (CFW_IsFreeUTI(nUTI, CFW_GPRS_SRV_ID) != ERR_SUCCESS)
    {
        return ERR_CFW_UTI_IS_BUSY;
    }
#ifdef CFW_MULTI_SIM
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsManualRej()\r\n"));	

		return nRet;
	}
#endif
    CFW_SetServiceId(CFW_GPRS_SRV_ID);
#ifdef CFW_MULTI_SIM	
    nAoCount = CFW_GetAoCount(CFW_GPRS_SRV_ID, nSimID);
#else
    nAoCount = CFW_GetAoCount();
#endif
    for (nAoIndex = 0; nAoIndex < nAoCount; nAoIndex++)
    {
#ifdef CFW_MULTI_SIM	
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW, CFW_GPRS_SRV_ID, nSimID);
#else
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW);
#endif
        proc = CFW_GetAoUserData(hAo);
        if (proc->Cid2Index.nCid == nCid)
        {    
            if(proc->nActState == CFW_GPRS_ACTIVED)
            {
                return ERR_CFW_GPRS_HAVE_ACTIVED;
            }
            proc->nMsgId = API_PDP_CTXACTIVATE_REJ;
            proc->pMsgBody = NULL;
            
            //SUL_MemCopy8(&proc->ModifyQos, Qos, SIZEOF(CFW_GPRS_QOS));

            CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom
            return ERR_SUCCESS;
        }
    }
    return ERR_CFW_GPRS_INVALID_CID;   // there is no context according to the cid;   }

}

// modify needed
// [in]UTI
// [in]nCid
// [in]Qos: point the qos set the Qos
// return: ERR_CFW_GPRS_INVALID_CID
// ERR_CFW_UTI_IS_BUSY
// ERR_CFW_INVALID_PARAMETER
// response: EV_CFW_GPRS_MOD_NOTIFY
// parameter1: Cid
// parameter1: 0
// UTI: uti
// type: 0
#ifdef CFW_MULTI_SIM
UINT32 CFW_GprsCtxModify(UINT16 nUTI, UINT8 nCid, CFW_GPRS_QOS *Qos, CFW_SIM_ID nSimID)
#else
UINT32 CFW_GprsCtxModify(UINT16 nUTI, UINT8 nCid, CFW_GPRS_QOS *Qos)
#endif
{
    GPRS_SM_INFO * proc = NULL;   //  sm info
    HAO hAo = 0;                  //  ao
    UINT8 nAoCount = 0;
    UINT8 nAoIndex = 0;
	// add by wuys 2009-03-18, for fix the bug 11403
    UINT8 nActState = 0xFF;
#ifdef CFW_MULTI_SIM
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsCtxModify()\r\n"));	

		return nRet;
	}
    CFW_GetGprsActState(nCid,&nActState,nSimID);
#else
    CFW_GetGprsActState(nCid,&nActState);
#endif
 	if(nActState !=CFW_GPRS_ACTIVED)
		return ERR_CME_OPERATION_NOT_ALLOWED;
	//add end
    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsCtxModify cid = 0x%x, nUTI = 0x%x\n"), nCid, nUTI);
    if((nCid > 7) || (nCid < 1))
        return ERR_CFW_INVALID_PARAMETER;
    if (CFW_IsFreeUTI(nUTI, CFW_GPRS_SRV_ID) != ERR_SUCCESS)
    {
        return ERR_CFW_UTI_IS_BUSY;
    }
    if (Qos == NULL)
    {
        return ERR_CFW_INVALID_PARAMETER;
    }
#ifdef CFW_MULTI_SIM
	if( nSimID >=CFW_SIM_COUNT)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsCtxModify()\r\n"));	
		return ERR_CFW_INVALID_PARAMETER;
	}
#endif

    CFW_SetServiceId(CFW_GPRS_SRV_ID);
#ifdef CFW_MULTI_SIM	
    nAoCount = CFW_GetAoCount(CFW_GPRS_SRV_ID, nSimID);
#else
    nAoCount = CFW_GetAoCount();
#endif
    for (nAoIndex = 0; nAoIndex < nAoCount; nAoIndex++)
    {
#ifdef CFW_MULTI_SIM	
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW, CFW_GPRS_SRV_ID, nSimID);
#else
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW);
#endif
        proc = CFW_GetAoUserData(hAo);
        if (proc->Cid2Index.nCid == nCid)
        {    
            proc->nMsgId = API_PDP_CTXMODIFY_REQ;
            proc->pMsgBody = NULL;
            
            SUL_MemCopy8(&proc->ModifyQos, Qos, SIZEOF(CFW_GPRS_QOS));

            CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom
            return ERR_SUCCESS;
        }
    }           
    return ERR_CFW_GPRS_INVALID_CID;   // there is no context according to the cid;
}


// no response
// [in]UTI
// [in]nCid [1 - 7]
// return: ERR_CFW_GPRS_INVALID_CID
// ERR_CFW_UTI_IS_BUSY
#ifdef CFW_MULTI_SIM
UINT32 CFW_GprsCtxModifyAcc(UINT16 nUTI, UINT8 nCid, CFW_SIM_ID nSimID)
#else
UINT32 CFW_GprsCtxModifyAcc(UINT16 nUTI, UINT8 nCid)
#endif
{
    GPRS_SM_INFO * proc = NULL;   //  sm info
    HAO hAo = 0;                  //  ao
    UINT8 nAoCount = 0;
    UINT8 nAoIndex = 0;
          
    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsCtxModifyAcc cid = 0x%x, nUTI = 0x%x\n"), nCid, nUTI);
    if((nCid > 7) || (nCid < 1))
        return ERR_CFW_INVALID_PARAMETER;
    if (CFW_IsFreeUTI(nUTI, CFW_GPRS_SRV_ID) != ERR_SUCCESS)
    {
        return ERR_CFW_UTI_IS_BUSY;
    }
#ifdef CFW_MULTI_SIM
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsCtxModifyAcc()\r\n"));	

		return nRet;
	}
#endif
    CFW_SetServiceId(CFW_GPRS_SRV_ID);
#ifdef CFW_MULTI_SIM	
    nAoCount = CFW_GetAoCount(CFW_GPRS_SRV_ID, nSimID);
#else
    nAoCount = CFW_GetAoCount();
#endif
    for (nAoIndex = 0; nAoIndex < nAoCount; nAoIndex++)
    {
#ifdef CFW_MULTI_SIM	
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW, CFW_GPRS_SRV_ID, nSimID);
#else
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW);
#endif
        proc = CFW_GetAoUserData(hAo);
        if (proc->Cid2Index.nCid == nCid)
        {    
            proc->nMsgId = API_PDP_CTXMODIFY_ACC;
            proc->pMsgBody = NULL;

            CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom
            return ERR_SUCCESS;
        }
    }
    return ERR_CFW_GPRS_INVALID_CID;   // there is no context according to the cid;
}

// [in]UTI
// [in]nCid [1 - 7]
// return: ERR_CFW_GPRS_INVALID_CID
// ERR_CFW_UTI_IS_BUSY
// response: none
#ifdef CFW_MULTI_SIM
UINT32 CFW_GprsCtxModifyRej(UINT16 nUTI, UINT8 nCid, CFW_SIM_ID nSimID)
#else
UINT32 CFW_GprsCtxModifyRej(UINT16 nUTI, UINT8 nCid)
#endif
{
    GPRS_SM_INFO * proc = NULL;     //  sm info
    HAO hAo = 0;                  //  ao
    UINT8 nAoCount = 0;
    UINT8 nAoIndex = 0;
          
    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsCtxModifyRej cid = 0x%x, nUTI = 0x%x\n"), nCid, nUTI);
    if((nCid > 7) || (nCid < 1))
        return ERR_CFW_INVALID_PARAMETER;
    if (CFW_IsFreeUTI(nUTI, CFW_GPRS_SRV_ID) != ERR_SUCCESS)
    {
        return ERR_CFW_UTI_IS_BUSY;
    }
#ifdef CFW_MULTI_SIM
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsCtxModifyRej()\r\n"));	

		return nRet;
	}
#endif
    CFW_SetServiceId(CFW_GPRS_SRV_ID);
#ifdef CFW_MULTI_SIM	
    nAoCount = CFW_GetAoCount(CFW_GPRS_SRV_ID, nSimID);
#else
    nAoCount = CFW_GetAoCount();
#endif
    for (nAoIndex = 0; nAoIndex < nAoCount; nAoIndex++)
    {
#ifdef CFW_MULTI_SIM	
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW, CFW_GPRS_SRV_ID, nSimID);
#else
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW);
#endif
        proc = CFW_GetAoUserData(hAo);
        if (proc->Cid2Index.nCid == nCid)
        {    
            proc->nMsgId = API_PDP_CTXMODIFY_REJ;
            proc->pMsgBody = NULL;
            //SUL_MemCopy8(&proc->ModifyQos, Qos, SIZEOF(CFW_GPRS_QOS));

            CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom
            return ERR_SUCCESS;
        }
    }
    return ERR_CFW_GPRS_INVALID_CID;   // there is no context according to the cid;
}

#ifdef CFW_MULTI_SIM

extern  UINT8 nQueueid[];
#else
extern  UINT8 nQueueid;
#endif
// nCid [1 - 7]
// send data   //UINT8 nCid, UINT16 nDataLength, UINT8 *pData
// event: None
// return: ERR_CFW_GPRS_INVALID_CID
// ERR_CFW_INVALID_PARAMETER
#ifdef CFW_MULTI_SIM
UINT32 CFW_GprsSendData(UINT8 nCid, CFW_GPRS_DATA * pGprsData, CFW_SIM_ID nSimID)
#else
UINT32 CFW_GprsSendData(UINT8 nCid, CFW_GPRS_DATA * pGprsData)
#endif
{
    GPRS_SM_INFO * proc = NULL;     //  sm info
    HAO hAo = 0;                  //  ao
    UINT8 nAoCount = 0;
    UINT8 nAoIndex = 0;
    UINT8 nActState = CFW_GPRS_DEACTIVED;
	
    //UINT8 nQueueid;
    #ifdef CFW_MULTI_SIM
    		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsSendData() begin, SimID:%d, cid: %d \r\n"),nSimID, nCid );	
#endif
   //CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsSendData cid = 0x%x, \n"), nCid);
    if((nCid > 7) || (nCid < 1))
        return ERR_CFW_INVALID_PARAMETER;
    if ((pGprsData == NULL) || (pGprsData->pData == NULL))
    {
        return ERR_CFW_INVALID_PARAMETER;
    }
#ifdef CFW_MULTI_SIM
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if(ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsSendData()\r\n"));	

		return nRet;
	}
  	CFW_GetGprsActState(nCid, &nActState,nSimID);
#else
  	CFW_GetGprsActState(nCid, &nActState);
#endif
  	if(nActState != CFW_GPRS_ACTIVED)
  	{
        CSW_TRACE(CFW_GPRS_TRACE, TSTXT("The pdp is not actived\n"));
  	    return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
  	}

#ifdef CFW_MULTI_SIM
	if( nSimID >=CFW_SIM_COUNT)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsSendData()\r\n"));	

		return ERR_CFW_INVALID_PARAMETER;
	}
#endif
    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsSendData: send data to stack:\n"));                    
    CFW_Dump_SXS(CFW_GPRS_TRACE_DATA, pGprsData->pData, pGprsData->nDataLength);
    CFW_SetServiceId(CFW_GPRS_SRV_ID);
#ifdef CFW_MULTI_SIM	
    nAoCount = CFW_GetAoCount(CFW_GPRS_SRV_ID, nSimID);
#else
    nAoCount = CFW_GetAoCount();
#endif
    //CSW_TRACE(CFW_GPRS_TRACE, TSTXT("nAoCount = 0x%x:\n"), nAoCount);   	
    for (nAoIndex = 0; nAoIndex < nAoCount; nAoIndex++)
    {
#ifdef CFW_MULTI_SIM	
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW, CFW_GPRS_SRV_ID, nSimID);
#else
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW);
#endif
        proc = CFW_GetAoUserData(hAo);
        if (proc->Cid2Index.nCid == nCid)
        {
            if (proc->nActState == CFW_GPRS_ACTIVED)
            {
                if(proc->nFlowCtr == FLOW_RELEASED)
                {
#ifdef CFW_MULTI_SIM	
					CSW_TRACE(CFW_GPRS_TRACE,"CSW send data to SNDCP, len = %d; Reliability:%d \n",pGprsData->nDataLength,pQosList[nSimID][nCid - 1].nReliability);
				    if(pQosList[nSimID][nCid - 1].nReliability <3)
				    {
			       gprs_sum[nSimID][0]+= pGprsData->nDataLength;
			  CFW_SendSclData(proc->Cid2Index.nNsapi, nQueueid[nSimID], API_PDP_ACK , pGprsData->nDataLength, pGprsData->pData, nSimID);
            }            
     		    else
     		    {
			    	  gprs_sum[nSimID][0]+= pGprsData->nDataLength;
              CFW_SendSclData(proc->Cid2Index.nNsapi, nQueueid[nSimID], API_PDP_UNACK , pGprsData->nDataLength, pGprsData->pData, nSimID);
            }

#else
					CSW_TRACE(CFW_GPRS_TRACE,"CSW send data to SNDCP, len = %d; Reliability:%d \n",pGprsData->nDataLength,pQosList[nCid - 1].nReliability);
				    if(pQosList[nCid - 1].nReliability <3)
            {
              CFW_SendSclData(proc->Cid2Index.nNsapi, nQueueid, API_PDP_ACK , pGprsData->nDataLength, pGprsData->pData);
            }
     		    else
     		    {
              CFW_SendSclData(proc->Cid2Index.nNsapi, nQueueid, API_PDP_UNACK , pGprsData->nDataLength, pGprsData->pData);
            }

#endif


                }
				else
				{
				    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsSendData: receive the FlowCtr from stack, discard the data req\n"));       
				}
                return ERR_SUCCESS;
            }
            else
            {
                return ERR_CME_OPERATION_NOT_ALLOWED; // should not send the data in the deactived state
            }

        }
        
    }
    return ERR_CFW_GPRS_INVALID_CID; // the cid error ,there is no context according to the cid    
    
}


//=============================================================================================
//synchronization function
//AT+CGDCONT
//[in]nCid[1 - 7], [in]pPdpCont
//
//============================================================================================= 
#ifdef CFW_MULTI_SIM
UINT32 CFW_GprsSetPdpCxt(UINT8 nCid, CFW_GPRS_PDPCONT_INFO *pPdpCont, CFW_SIM_ID nSimID)
#else
UINT32 CFW_GprsSetPdpCxt(UINT8 nCid, CFW_GPRS_PDPCONT_INFO *pPdpCont)
#endif
{
   CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsSetPdpCxt, cid: 0x%x, pPdpCont: 0x%x, simid: 0x%x\n"),nCid,pPdpCont,nSimID);       


//#if TMP_GPRS_CONTEXT
    UINT32  ret = ERR_SUCCESS;
    if( (nCid > 7) || (nCid < 1) || (pPdpCont == NULL) )
        return ERR_CFW_INVALID_PARAMETER;
    
    if((pPdpCont->nPdpType > CFW_GPRS_PDP_TYPE_PPP) 
		|| (pPdpCont->nDComp > CFW_GPRS_PDP_D_COMP_V44) 
		|| (pPdpCont->nHComp > CFW_GPRS_PDP_H_COMP_RFC3095))
    {
        return ERR_CFW_INVALID_PARAMETER;
    }
#ifdef CFW_MULTI_SIM
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsSetPdpCxt()\r\n"));	

		return nRet;
	}
#endif
	CFW_GPRS_PDPCONT_INFO * new_pdp = CSW_GPRS_MALLOC(sizeof(CFW_GPRS_PDPCONT_INFO));
    memset(new_pdp, 0x00, sizeof(CFW_GPRS_PDPCONT_INFO));
    new_pdp->nApnSize = pPdpCont->nApnSize;
    if(new_pdp->nApnSize > 0)
    {
      new_pdp->pApn =CSW_GPRS_MALLOC(pPdpCont->nApnSize);
      memcpy(new_pdp->pApn, pPdpCont->pApn, pPdpCont->nApnSize);
    }
    new_pdp->nApnPwdSize= pPdpCont->nApnPwdSize;
    if(new_pdp->nApnPwdSize > 0)
    {
      new_pdp->pApnPwd =CSW_GPRS_MALLOC(pPdpCont->nApnPwdSize);
      memcpy(new_pdp->pApnPwd, pPdpCont->pApnPwd, pPdpCont->nApnPwdSize);
    }
    new_pdp->nApnUserSize= pPdpCont->nApnUserSize;
    if(new_pdp->nApnUserSize > 0)
    {
      new_pdp->pApnUser = CSW_GPRS_MALLOC(pPdpCont->nApnUserSize);
      memcpy(new_pdp->pApnUser, pPdpCont->pApnUser, pPdpCont->nApnUserSize);
    }
    new_pdp->nPdpAddrSize= pPdpCont->nPdpAddrSize;
    
    if(new_pdp->nPdpAddrSize > 0)
    {
        new_pdp->pPdpAddr=CSW_GPRS_MALLOC(18);
        memcpy(new_pdp->pPdpAddr, pPdpCont->pPdpAddr, pPdpCont->nPdpAddrSize);
    }else{
        new_pdp->pPdpAddr=CSW_GPRS_MALLOC(18);
        memset(new_pdp->pPdpAddr,0,18);
    }
    
    new_pdp->nDComp= pPdpCont->nDComp;
    new_pdp->nHComp= pPdpCont->nDComp;
    new_pdp->nPdpType= pPdpCont->nPdpType;
#ifdef CFW_MULTI_SIM   
    if(PdpContList[nSimID][nCid -1]!= NULL)
    {
      CFW_GprsRemovePdpCxt(nCid, nSimID);
    }
    PdpContList[nSimID][nCid -1] = new_pdp;
    UINT8 n = 0;
    for(; n<7; n++)
    {
      if(PdpContList[nSimID][n] != NULL)
      {
        if( PdpContList[nSimID][n]->nApnPwdSize> 0)
        {
          if(PdpContList[nSimID][n]->pApnPwd != NULL)
          {
             CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnPwd:\n"),n+1);
             SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[nSimID][n]->pApnPwd, PdpContList[nSimID][n]->nApnPwdSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnPwd: NULL\n"),n+1);
          }
        }
        
        if( PdpContList[nSimID][n]->nApnSize> 0)
        {
          if(PdpContList[nSimID][n]->pApn != NULL)
          {
             CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApn:\n"),n+1);
             SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[nSimID][n]->pApn, PdpContList[nSimID][n]->nApnSize);
          }else{
             CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApn: NULL\n"),n+1);
          }
        }

        if( PdpContList[nSimID][n]->nApnUserSize> 0)
        {
          if(PdpContList[nSimID][n]->pApnUser!= NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnUser:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[nSimID][n]->pApnUser, PdpContList[nSimID][n]->nApnUserSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnUser: NULL\n"),n+1);
          }
        }
        if( PdpContList[nSimID][n]->nPdpAddrSize > 0)
        {
          if(PdpContList[nSimID][n]->pPdpAddr!= NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pPdpaddr:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[nSimID][n]->pPdpAddr, PdpContList[nSimID][n]->nPdpAddrSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pPdpaddr: NULL\n"),n+1);
          }
        }

      }
    }
#else
   //add by wuys 2008-07-07
   
    if(PdpContList[nCid -1]!= NULL)
    {
      CFW_GprsRemovePdpCxt(nCid);
    }
   //end add
    PdpContList[nCid -1] = new_pdp;
    //add by wuys 2008-07-02, for debug 
    UINT8 n = 0;
    for(; n<7; n++)
    {
      if(PdpContList[n] != NULL)
      {
        if( PdpContList[n]->nApnPwdSize> 0)
        {
          if(PdpContList[n]->pApnPwd != NULL)
          {
             CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnPwd:\n"),n+1);
             SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[n]->pApnPwd, PdpContList[n]->nApnPwdSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnPwd: NULL\n"),n+1);
          }
        }
        
        if( PdpContList[n]->nApnSize> 0)
        {
          if(PdpContList[n]->pApn != NULL)
          {
             CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApn:\n"),n+1);
             SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[n]->pApn, PdpContList[n]->nApnSize);
          }else{
             CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApn: NULL\n"),n+1);
          }
        }

        if( PdpContList[n]->nApnUserSize> 0)
        {
          if(PdpContList[n]->pApnUser!= NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnUser:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[n]->pApnUser, PdpContList[n]->nApnUserSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnUser: NULL\n"),n+1);
          }
        }
        if( PdpContList[n]->nPdpAddrSize > 0)
        {
          if(PdpContList[n]->pPdpAddr!= NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pPdpaddr:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[n]->pPdpAddr, PdpContList[n]->nPdpAddrSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pPdpaddr: NULL\n"),n+1);
          }
        }

      }
    }
    //add end
#endif
    return ret;
    
}

//=============================================================================================
//[in]nCid[1 - 7], [out]pPdpCont
//=============================================================================================
#ifdef CFW_MULTI_SIM
UINT32 CFW_GprsGetPdpCxt(UINT8 nCid, CFW_GPRS_PDPCONT_INFO *pPdpCont, CFW_SIM_ID nSimID)
{
	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsGetPdpCxt\n"));       

    UINT8 n = 0;
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsGetPdpCxt()\r\n"));	

		return nRet;
	}
	
    for(; n<7; n++)
    {
      if(PdpContList[nSimID][n] != NULL)
      {
        if( PdpContList[nSimID][n]->nApnPwdSize> 0)
        {
          if(PdpContList[nSimID][n]->pApnPwd != NULL)
          {
             CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnPwd:\n"),n+1);
             SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[nSimID][n]->pApnPwd, PdpContList[nSimID][n]->nApnPwdSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnPwd: NULL\n"),n+1);
          }
        }
        
        if( PdpContList[nSimID][n]->nApnSize> 0)
        {
          if(PdpContList[nSimID][n]->pApn != NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApn:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[nSimID][n]->pApn, PdpContList[nSimID][n]->nApnSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApn: NULL\n"),n+1);
          }
        }

        if( PdpContList[nSimID][n]->nApnUserSize> 0)
        {
          if(PdpContList[nSimID][n]->pApnUser!= NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnUser:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[nSimID][n]->pApnUser, PdpContList[nSimID][n]->nApnUserSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnUser: NULL\n"),n+1);
          }
        }
        if( PdpContList[nSimID][n]->nPdpAddrSize > 0)
        {
          if(PdpContList[nSimID][n]->pPdpAddr!= NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pPdpaddr:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[nSimID][n]->pPdpAddr, PdpContList[nSimID][n]->nPdpAddrSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pPdpaddr: NULL\n"),n+1);
          }
        }

      }
    }
    //add end
   // #if TMP_GPRS_CONTEXT
      UINT32  ret = ERR_SUCCESS;
      if( (nCid > 7) || (nCid < 1) || (pPdpCont == NULL) )
        return ERR_CFW_INVALID_PARAMETER;
      if(NULL == PdpContList[nSimID][nCid - 1])
        return ERR_NOT_SUPPORT;
      pPdpCont->nApnSize =  PdpContList[nSimID][nCid - 1]->nApnSize;
      if(pPdpCont->nApnSize > 0)
        memcpy(pPdpCont->pApn,  PdpContList[nSimID][nCid - 1]->pApn, pPdpCont->nApnSize);

      pPdpCont->nApnUserSize =  PdpContList[nSimID][nCid - 1]->nApnUserSize;
      if(pPdpCont->nApnUserSize > 0)
        memcpy(pPdpCont->pApnUser,  PdpContList[nSimID][nCid - 1]->pApnUser, pPdpCont->nApnUserSize);
      
      pPdpCont->nApnPwdSize =  PdpContList[nSimID][nCid - 1]->nApnPwdSize;
      if(pPdpCont->pApnPwd > 0)
        memcpy(pPdpCont->pApnPwd,  PdpContList[nSimID][nCid - 1]->pApnPwd, pPdpCont->nApnPwdSize);
      
      pPdpCont->nPdpAddrSize =  PdpContList[nSimID][nCid - 1]->nPdpAddrSize;
      if( PdpContList[nSimID][nCid -1]->nPdpAddrSize > 0)
      {
          memcpy(pPdpCont->pPdpAddr,  PdpContList[nSimID][nCid - 1]->pPdpAddr, pPdpCont->nPdpAddrSize);
      }else{
          memset(pPdpCont->pPdpAddr,0, 18);
      }
      
      pPdpCont->nDComp =  PdpContList[nSimID][nCid - 1]->nDComp;
      pPdpCont->nHComp =  PdpContList[nSimID][nCid - 1]->nHComp;
      pPdpCont->nPdpType =  PdpContList[nSimID][nCid - 1]->nPdpType;

      return ret;
    
}

UINT32 CFW_GprsRemovePdpCxt(UINT8 nCid, CFW_SIM_ID nSimID)
{
    UINT8 n = 0;
	UINT32 nRet = ERR_SUCCESS;
	nRet = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=nRet)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsRemovePdpCxt()\r\n"));	

		return nRet;
	}
	
    for(; n<7; n++)
    {
      if(PdpContList[nSimID][n] != NULL)
      {
        if( PdpContList[nSimID][n]->nApnPwdSize> 0)
        {
          if(PdpContList[nSimID][n]->pApnPwd != NULL)
          {
             CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnPwd:\n"),n+1);
             SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[nSimID][n]->pApnPwd, PdpContList[nSimID][n]->nApnPwdSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnPwd: NULL\n"),n+1);
          }
        }
        
        if( PdpContList[nSimID][n]->nApnSize> 0)
        {
          if(PdpContList[nSimID][n]->pApn != NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApn:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[nSimID][n]->pApn, PdpContList[nSimID][n]->nApnSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApn: NULL\n"),n+1);
          }
        }

        if( PdpContList[nSimID][n]->nApnUserSize> 0)
        {
          if(PdpContList[nSimID][n]->pApnUser!= NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnUser:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[nSimID][n]->pApnUser, PdpContList[nSimID][n]->nApnUserSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnUser: NULL\n"),n+1);
          }
        }
        if( PdpContList[nSimID][n]->nPdpAddrSize > 0)
        {
          if(PdpContList[nSimID][n]->pPdpAddr!= NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pPdpaddr:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[nSimID][n]->pPdpAddr, PdpContList[nSimID][n]->nPdpAddrSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pPdpaddr: NULL\n"),n+1);
          }
        }

      }
    }
   
   // #if TMP_GPRS_CONTEXT
      UINT32  ret = ERR_SUCCESS;
      if((nCid > 7) || (nCid < 1)  )
        return ERR_CFW_INVALID_PARAMETER;
      if(NULL == PdpContList[nSimID][nCid - 1])
        return ERR_NOT_SUPPORT;
      
      if(PdpContList[nSimID][nCid - 1]->pApn != NULL)
        CSW_GPRS_FREE(PdpContList[nSimID][nCid - 1]->pApn);
      if(PdpContList[nSimID][nCid - 1]->pApnPwd!= NULL)
        CSW_GPRS_FREE(PdpContList[nSimID][nCid - 1]->pApnPwd);
      if(PdpContList[nSimID][nCid - 1]->pApnUser!= NULL)
        CSW_GPRS_FREE(PdpContList[nSimID][nCid - 1]->pApnUser);
      if(PdpContList[nSimID][nCid - 1]->pPdpAddr!= NULL)
        CSW_GPRS_FREE(PdpContList[nSimID][nCid - 1]->pPdpAddr);
/*
      if(DnsServerIP[nSimID][nCid - 1]!=NULL)
      	{
	  	  CSW_GPRS_FREE(DnsServerIP[nSimID][nCid - 1]);
		  DnsServerIP[nSimID][nCid - 1]=NULL;
      	}
*/	  
      CSW_GPRS_FREE(PdpContList[nSimID][nCid - 1]);
     
      PdpContList[nSimID][nCid - 1]= NULL;

      
      return ret;
    
   
}

UINT32 CFW_GprsSetReqQos(UINT8 nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID)
{
	UINT32  ret = ERR_SUCCESS;
	ret = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=ret)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsSetReqQos()\r\n"));	

		return ret;
	}
	
	if( (nCid > 7) || (nCid < 1) || (pQos == NULL) )
	return ERR_CFW_INVALID_PARAMETER;
	if((pQos->nDelay > 7) || (pQos->nMean > 31) || (pQos->nPeak > 15) 
		|| (pQos->nPrecedence > 7) || (pQos->nReliability > 7))
	{
	return ERR_CFW_INVALID_PARAMETER;
	}
	pQosList[nSimID][nCid - 1].nDelay = pQos->nDelay;
	pQosList[nSimID][nCid - 1].nMean = pQos->nMean;
	pQosList[nSimID][nCid - 1].nPeak = pQos->nPeak ;
	pQosList[nSimID][nCid - 1].nPrecedence = pQos->nPrecedence;
	pQosList[nSimID][nCid - 1].nReliability = pQos->nReliability;

	return ret;
}

UINT32 CFW_GprsGetReqQos(UINT8 nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID)
{     
	UINT32  ret = ERR_SUCCESS;
	ret = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=ret)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsGetReqQos()\r\n"));	

		return ret;
	}

	if( (nCid > 7) || (nCid < 1) || (pQos == NULL) )
	return ERR_CFW_INVALID_PARAMETER;

	pQos->nDelay =  pQosList[nSimID][nCid - 1].nDelay;
	pQos->nMean =  pQosList[nSimID][nCid - 1].nMean;
	pQos->nPeak =  pQosList[nSimID][nCid - 1].nPeak;
	pQos->nPrecedence =  pQosList[nSimID][nCid - 1].nPrecedence;
	pQos->nReliability =  pQosList[nSimID][nCid - 1].nReliability;

	return ret;
}

UINT32 CFW_GprsSetMinQos(UINT8 nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	ret = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=ret)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsSetMinQos()\r\n"));	

		return ret;
	}
	CSW_TRACE(CFW_GPRS_TRACE , TSTXT("CFW_GprsSetMinQos = 0x%x , 0x%x ,0x%x ,0x%x ,0x%x \n"), pQos->nDelay, pQos->nMean, pQos->nPeak,pQos->nPrecedence,pQos->nReliability);

	if ((nCid > 7) || (nCid < 1) || (pQos == NULL))
	return ERR_CFW_INVALID_PARAMETER;
	if ((pQos->nDelay > 4) || (pQos->nMean > 31) || (pQos->nPeak > 9)
	  || (pQos->nPrecedence > 3) || (pQos->nReliability > 5))
	{
	return ERR_CFW_INVALID_PARAMETER;
	}
	pMinQosList[nSimID][nCid - 1].nDelay        = pQos->nDelay;
	pMinQosList[nSimID][nCid - 1].nMean         = pQos->nMean;
	pMinQosList[nSimID][nCid - 1].nPeak         = pQos->nPeak;
	pMinQosList[nSimID][nCid - 1].nPrecedence   = pQos->nPrecedence;
	pMinQosList[nSimID][nCid - 1].nReliability  = pQos->nReliability;

	return ret;
}

UINT32 CFW_GprsGetMInQos(UINT8 nCid, CFW_GPRS_QOS* pQos, CFW_SIM_ID nSimID)
{
	UINT32 ret = ERR_SUCCESS;
	ret = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=ret)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GprsGetMInQos()\r\n"));	

		return ret;
	}
	
	if(pQos == NULL)
	{
	  return ERR_CFW_INVALID_PARAMETER;
	}

	if ((nCid > 7) || (nCid < 1) || (pQos == NULL))
	{
	  return ERR_CFW_INVALID_PARAMETER;
	}

	pQos->nPrecedence = pMinQosList[nSimID][nCid-1].nPrecedence;
	pQos->nDelay      = pMinQosList[nSimID][nCid-1].nDelay;
	pQos->nMean       = pMinQosList[nSimID][nCid-1].nMean;
	pQos->nPeak       = pMinQosList[nSimID][nCid-1].nPeak;
	pQos->nReliability= pMinQosList[nSimID][nCid-1].nReliability;

	CSW_TRACE(CFW_GPRS_TRACE , TSTXT("CFW_GprsSetMinQos = 0x%x , 0x%x ,0x%x ,0x%x ,0x%x \n"), pQos->nDelay, pQos->nMean, pQos->nPeak,pQos->nPrecedence,pQos->nReliability);


	return ret;
}

static UINT32 storecid[CFW_SIM_COUNT][8];
#define MAX_CID_NUM   0x07

//The CFW_GetFreeCID function find the empty position to save the CID.
// [out]pCID point to UINT8 type to retrieve the free CID.
// return: If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the error code ERR_CFW_NOT_OPERATOR_ID may be returned
UINT32 CFW_GetFreeCID(UINT8* pCID, CFW_SIM_ID nSimID)
{
	UINT8  n=0, i=0;
  	UINT32 temp=0x00;
	UINT32 ret = ERR_SUCCESS;
	ret = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=ret)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_GetFreeCID()\r\n"));	

		return ret;
	}
	
	for(i=0; i<8; i++)
	{
		for(n=1; n<33; n++)
		{
			temp = storecid[nSimID][i] & (1<<(n-1));
			if (0 == temp)
			{
				*pCID = (i*32 + n);
				CSW_TRACE(CFW_GPRS_TRACE, TSTXT("!!!!!!! CFW_GetFreeCID() 0x%d\r\n"),*pCID);	

				if( *pCID > MAX_CID_NUM )
					return ERR_CFW_NOT_OPERATOR_ID;
				else						
					return ERR_SUCCESS;
			}
		}
	}

	return ERR_CFW_NOT_OPERATOR_ID;
}


//The CFW_ReleaseCID function will release the CID when the state of GPRS attachment is CFW_GPRS_DEACTIVATED.
//[in] nCID
// return: If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the error code ERR_CFW_INVALID_PARAMETER may be returned
UINT32 CFW_ReleaseCID(UINT8 nCID, CFW_SIM_ID nSimID)
{
	UINT8 i=0, n=0;
	UINT32 ret = ERR_SUCCESS;
	ret = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=ret)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_ReleaseCID()\r\n"));	

		return ret;
	}
	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("!!!!!!! CFW_ReleaseCID() 0x%d\r\n"),nCID);	

	if((0 != nCID) && (MAX_CID_NUM >= nCID))
	{
		i = nCID/32;
		n = nCID - i*32;
		if (0 == (storecid[nSimID][i] & (1<<(n-1))))
		{
			return ERR_CFW_INVALID_PARAMETER;
		}
		else
		{
			storecid[nSimID][i] = storecid[nSimID][i] & (~(1<<(n-1))); 
			GprsCidSavedInApp[nSimID] &=~(0x01<<nCID);

			CFW_StartDetachTimer(nSimID);
			return ERR_SUCCESS;
		}
	}

	return ERR_CFW_INVALID_PARAMETER;
}


//The CFW_SetCID function will set the CID when the state of GPRS attachment is CFW_GPRS_ACTIVATED.
//[in] nCID
// return: If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the error code ERR_CFW_INVALID_PARAMETER may be returned
UINT32 CFW_SetCID(UINT8 nCID, CFW_SIM_ID nSimID)
{
	UINT8 i=0, n=0;
	UINT32 ret = ERR_SUCCESS;
	ret = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS!=ret)
	{
		CSW_TRACE(CFW_GPRS_TRACE, TSTXT("ERROR ! SIM id error CFW_SetCID()\r\n"));	

		return ret;
	}
	CSW_TRACE(CFW_GPRS_TRACE, TSTXT("!!!!!!! CFW_SetCID() 0x%d\r\n"),nCID);	

	if((0 != nCID) && (MAX_CID_NUM >= nCID))
	{
		i = nCID/32;
		n = nCID - i*32;
		if (0 != (storecid[nSimID][i]&(1<<(n-1))))
		{
			return ERR_CFW_INVALID_PARAMETER;
		}
		else
		{
			storecid[nSimID][i] = storecid[nSimID][i]|(1<<(n-1));
			return ERR_SUCCESS;
		}
	}
	return ERR_CFW_INVALID_PARAMETER;
}


   //add wuys 2009-10-22

  


   
   UINT32 CFW_getDnsServerbyPdp(UINT8 nCid, UINT8 nSimID )
   {
      CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_getDnsServerbyPdp, cid : %d, simid: %d\n"),nCid,nSimID);      
	  SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, DnsServerIP[nSimID][nCid-1], 6);
   	UINT8* tmp;
	UINT32 serip= 0x00;
	//CFW_GPRS_PDPCONT_INFO  *tmp_pdp;
	//tmp_pdp = PdpContList[nSimID][nCid-1];
	tmp = DnsServerIP[nSimID][nCid-1];
	if(tmp==NULL)
	   return serip;
	serip = tmp[3]<<24| tmp[2]<<16| tmp[1]<<8| tmp[0];
       CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_getDnsServerbyPdp, serip 0x%x\n"),serip);      

        // udp_connect(resolv_pcb, &serverIP, DNS_SERVER_PORT);
  	    return serip;
   }
 
   //add end
#else

UINT32 CFW_GprsGetPdpCxt(UINT8 nCid, CFW_GPRS_PDPCONT_INFO *pPdpCont)
{
     CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GprsGetPdpCxt\n"));       

    //add by wuys 2008-07-02, for debug 
    UINT8 n = 0;
    for(; n<7; n++)
    {
      if(PdpContList[n] != NULL)
      {
        if( PdpContList[n]->nApnPwdSize> 0)
        {
          if(PdpContList[n]->pApnPwd != NULL)
          {
             CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnPwd:\n"),n+1);
             SXS_DUMP(CFW_GPRS_TRACE, 0, PdpContList[n]->pApnPwd, PdpContList[n]->nApnPwdSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnPwd: NULL\n"),n+1);
          }
        }
        
        if( PdpContList[n]->nApnSize> 0)
        {
          if(PdpContList[n]->pApn != NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApn:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[n]->pApn, PdpContList[n]->nApnSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApn: NULL\n"),n+1);
          }
        }

        if( PdpContList[n]->nApnUserSize> 0)
        {
          if(PdpContList[n]->pApnUser!= NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnUser:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[n]->pApnUser, PdpContList[n]->nApnUserSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnUser: NULL\n"),n+1);
          }
        }
        if( PdpContList[n]->nPdpAddrSize > 0)
        {
          if(PdpContList[n]->pPdpAddr!= NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pPdpaddr:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[n]->pPdpAddr, PdpContList[n]->nPdpAddrSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pPdpaddr: NULL\n"),n+1);
          }
        }

      }
    }
    //add end
   // #if TMP_GPRS_CONTEXT
      UINT32  ret = ERR_SUCCESS;
      if( (nCid > 7) || (nCid < 1) || (pPdpCont == NULL) )
        return ERR_CFW_INVALID_PARAMETER;
      if(NULL == PdpContList[nCid - 1])
        return ERR_NOT_SUPPORT;
      pPdpCont->nApnSize =  PdpContList[nCid - 1]->nApnSize;
      if(pPdpCont->nApnSize > 0)
        memcpy(pPdpCont->pApn,  PdpContList[nCid - 1]->pApn, pPdpCont->nApnSize);

      pPdpCont->nApnUserSize =  PdpContList[nCid - 1]->nApnUserSize;
      if(pPdpCont->nApnUserSize > 0)
        memcpy(pPdpCont->pApnUser,  PdpContList[nCid - 1]->pApnUser, pPdpCont->nApnUserSize);
      
      pPdpCont->nApnPwdSize =  PdpContList[nCid - 1]->nApnPwdSize;
      if(pPdpCont->pApnPwd > 0)
        memcpy(pPdpCont->pApnPwd,  PdpContList[nCid - 1]->pApnPwd, pPdpCont->nApnPwdSize);
      
      pPdpCont->nPdpAddrSize =  PdpContList[nCid - 1]->nPdpAddrSize;
      if( PdpContList[nCid -1]->nPdpAddrSize > 0)
      {
          memcpy(pPdpCont->pPdpAddr,  PdpContList[nCid - 1]->pPdpAddr, pPdpCont->nPdpAddrSize);
      }else{
          memset(pPdpCont->pPdpAddr,0, 18);
      }
      
      pPdpCont->nDComp =  PdpContList[nCid - 1]->nDComp;
      pPdpCont->nHComp =  PdpContList[nCid - 1]->nHComp;
      pPdpCont->nPdpType =  PdpContList[nCid - 1]->nPdpType;

      return ret;
    
}

//add by wuys 2008-07-02
UINT32 CFW_GprsRemovePdpCxt(UINT8 nCid)
{
    UINT8 n = 0;
	
    for(; n<7; n++)
    {
      if(PdpContList[n] != NULL)
      {
        if( PdpContList[n]->nApnPwdSize> 0)
        {
          if(PdpContList[n]->pApnPwd != NULL)
          {
             CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnPwd:\n"),n+1);
             SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[n]->pApnPwd, PdpContList[n]->nApnPwdSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnPwd: NULL\n"),n+1);
          }
        }
        
        if( PdpContList[n]->nApnSize> 0)
        {
          if(PdpContList[n]->pApn != NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApn:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[n]->pApn, PdpContList[n]->nApnSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApn: NULL\n"),n+1);
          }
        }

        if( PdpContList[n]->nApnUserSize> 0)
        {
          if(PdpContList[n]->pApnUser!= NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnUser:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[n]->pApnUser, PdpContList[n]->nApnUserSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pApnUser: NULL\n"),n+1);
          }
        }
        if( PdpContList[n]->nPdpAddrSize > 0)
        {
          if(PdpContList[n]->pPdpAddr!= NULL)
          {
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pPdpaddr:\n"),n+1);
            SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, PdpContList[n]->pPdpAddr, PdpContList[n]->nPdpAddrSize);
          }else{
            CSW_TRACE(CFW_GPRS_TRACE, TSTXT("PdpCont %d: pPdpaddr: NULL\n"),n+1);
          }
        }

      }
    }
   
   // #if TMP_GPRS_CONTEXT
      UINT32  ret = ERR_SUCCESS;
      if((nCid > 7) || (nCid < 1)  )
        return ERR_CFW_INVALID_PARAMETER;
      if(NULL == PdpContList[nCid - 1])
        return ERR_NOT_SUPPORT;
      
      if(PdpContList[nCid - 1]->pApn != NULL)
        CSW_GPRS_FREE(PdpContList[nCid - 1]->pApn);
      if(PdpContList[nCid - 1]->pApnPwd!= NULL)
        CSW_GPRS_FREE(PdpContList[nCid - 1]->pApnPwd);
      if(PdpContList[nCid - 1]->pApnUser!= NULL)
        CSW_GPRS_FREE(PdpContList[nCid - 1]->pApnUser);
      if(PdpContList[nCid - 1]->pPdpAddr!= NULL)
        CSW_GPRS_FREE(PdpContList[nCid - 1]->pPdpAddr);

      CSW_GPRS_FREE(PdpContList[nCid - 1]);

      PdpContList[nCid - 1]= NULL;

      
      return ret;
    
   
}

//add end
//=============================================================================================
//AT+CGQREQ
//This command allows the TE to specify a Quality of Service Profile 
//that is used when the MT sends an Activate PDP Context Request message 
///to the network.

//[in]nCid [1 - 7], [in]pQos

//=============================================================================================
UINT32 CFW_GprsSetReqQos(UINT8 nCid, CFW_GPRS_QOS *pQos)
{
  //  #if TMP_GPRS_CONTEXT
      UINT32  ret = ERR_SUCCESS;
      if( (nCid > 7) || (nCid < 1) || (pQos == NULL) )
        return ERR_CFW_INVALID_PARAMETER;
      if((pQos->nDelay > 7) || (pQos->nMean > 31) || (pQos->nPeak > 15) 
  		|| (pQos->nPrecedence > 7) || (pQos->nReliability > 7))
      {
        return ERR_CFW_INVALID_PARAMETER;
      }
      pQosList[nCid - 1].nDelay = pQos->nDelay;
      pQosList[nCid - 1].nMean = pQos->nMean;
      pQosList[nCid - 1].nPeak = pQos->nPeak ;
      pQosList[nCid - 1].nPrecedence = pQos->nPrecedence;
      pQosList[nCid - 1].nReliability = pQos->nReliability;


      return ret;

}

//=============================================================================================
// [in]nCid[1 - 7], [out]pQos
//=============================================================================================
UINT32 CFW_GprsGetReqQos(UINT8 nCid, CFW_GPRS_QOS *pQos)
{
     //#if TMP_GPRS_CONTEXT
     
      UINT32  ret = ERR_SUCCESS;
      if( (nCid > 7) || (nCid < 1) || (pQos == NULL) )
        return ERR_CFW_INVALID_PARAMETER;
    
      pQos->nDelay =  pQosList[nCid - 1].nDelay;
      pQos->nMean =  pQosList[nCid - 1].nMean;
      pQos->nPeak =  pQosList[nCid - 1].nPeak;
      pQos->nPrecedence =  pQosList[nCid - 1].nPrecedence;
      pQos->nReliability =  pQosList[nCid - 1].nReliability;


      return ret;

}
//===========================================================================
//[in]nCid[1 - 7]
//===========================================================================
UINT32 CFW_GprsSetMinQos(UINT8 nCid, CFW_GPRS_QOS *pQos)
{
  UINT32 ret = ERR_SUCCESS;
  
  SXS_TRACE(CFW_GPRS_TRACE | TNB_ARG(3), TSTXT("CFW_GprsSetMinQos = 0x%x , 0x%x ,0x%x ,0x%x ,0x%x \n"), pQos->nDelay, pQos->nMean, pQos->nPeak,pQos->nPrecedence,pQos->nReliability);

  if ((nCid > 7) || (nCid < 1) || (pQos == NULL))
    return ERR_CFW_INVALID_PARAMETER;
  if ((pQos->nDelay > 4) || (pQos->nMean > 31) || (pQos->nPeak > 9)
      || (pQos->nPrecedence > 3) || (pQos->nReliability > 5))
  {
    return ERR_CFW_INVALID_PARAMETER;
  }
  pMinQosList[nCid - 1].nDelay        = pQos->nDelay;
  pMinQosList[nCid - 1].nMean         = pQos->nMean;
  pMinQosList[nCid - 1].nPeak         = pQos->nPeak;
  pMinQosList[nCid - 1].nPrecedence   = pQos->nPrecedence;
  pMinQosList[nCid - 1].nReliability  = pQos->nReliability;

  return ret;
  //return ERR_NOT_SUPPORT;
}

/*===========================================================================
//[in]nCid[1 - 7]
===========================================================================*/
UINT32 CFW_GprsGetMInQos(UINT8 nCid, CFW_GPRS_QOS* pQos)
{
  UINT32 ret = ERR_SUCCESS;

  if(pQos == NULL)
  {
      return ERR_CFW_INVALID_PARAMETER;
  }
  
  if ((nCid > 7) || (nCid < 1) || (pQos == NULL))
  {
      return ERR_CFW_INVALID_PARAMETER;
  }
  
  pQos->nPrecedence = pMinQosList[nCid-1].nPrecedence;
  pQos->nDelay      = pMinQosList[nCid-1].nDelay;
  pQos->nMean       = pMinQosList[nCid-1].nMean;
  pQos->nPeak       = pMinQosList[nCid-1].nPeak;
  pQos->nReliability= pMinQosList[nCid-1].nReliability;

  SXS_TRACE(CFW_GPRS_TRACE | TNB_ARG(3), TSTXT("CFW_GprsSetMinQos = 0x%x , 0x%x ,0x%x ,0x%x ,0x%x \n"), pQos->nDelay, pQos->nMean, pQos->nPeak,pQos->nPrecedence,pQos->nReliability);

  
  return ret;
}

// add by qidd for separate Cid from one another @20090317---begin
static UINT32 storecid[8]={0,};
#define MAX_CID_NUM   0x07

//The CFW_GetFreeCID function find the empty position to save the CID.
// [out]pCID point to UINT8 type to retrieve the free CID.
// return: If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the error code ERR_CFW_NOT_OPERATOR_ID may be returned
UINT32 CFW_GetFreeCID(UINT8* pCID)
{

	UINT8  n=0, i=0;
  	UINT32 temp=0x00;
	for(i=0; i<8; i++)
	{
		for(n=1; n<33; n++)
		{
			temp = storecid[i] & (1<<(n-1));
			if (0 == temp)
			{

				*pCID = (i*32 + n);
				CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_GetFreeCID, cid: 0x%d\r\n"),*pCID);	

				if( *pCID > MAX_CID_NUM )
					return ERR_CFW_NOT_OPERATOR_ID;
				else						
					return ERR_SUCCESS;
			}
		}
	}

	return ERR_CFW_NOT_OPERATOR_ID;
}


//The CFW_ReleaseCID function will release the CID when the state of GPRS attachment is CFW_GPRS_DEACTIVATED.
//[in] nCID
// return: If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the error code ERR_CFW_INVALID_PARAMETER may be returned
UINT32 CFW_ReleaseCID(UINT8 nCID)
{
      CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_ReleaseCID, cid : %d\n"),nCID);      

	UINT8 i=0, n=0;

	if((0 != nCID) && (MAX_CID_NUM >= nCID))
	{
		i = nCID/32;
		n = nCID - i*32;
		if (0 == (storecid[i] & (1<<(n-1))))
		{
			return ERR_CFW_INVALID_PARAMETER;
		}
		else
		{
			storecid[i] = storecid[i] & (~(1<<(n-1))); 
			GprsCidSavedInApp &=~(0x01<<nCID);
                   CFW_StartDetachTimer();
			return ERR_SUCCESS;
		}
	}

	return ERR_CFW_INVALID_PARAMETER;
}


//The CFW_SetCID function will set the CID when the state of GPRS attachment is CFW_GPRS_ACTIVATED.
//[in] nCID
// return: If the function succeeds, the return value is ERR_SUCCESS, if the function fails, the error code ERR_CFW_INVALID_PARAMETER may be returned
UINT32 CFW_SetCID(UINT8 nCID)
{
	UINT8 i=0, n=0;

	if((0 != nCID) && (MAX_CID_NUM >= nCID))
	{
		i = nCID/32;
		n = nCID - i*32;
		if (0 != (storecid[i]&(1<<(n-1))))
		{
			return ERR_CFW_INVALID_PARAMETER;
		}
		else
		{
			storecid[i] = storecid[i]|(1<<(n-1));
			return ERR_SUCCESS;
		}
	}
	return ERR_CFW_INVALID_PARAMETER;
}
// add by qidd for separate Cid from one another @20090317---end

 //add wuys 2009-10-22

 UINT32 CFW_getDnsServerbyPdp(UINT8 nCid )
   {
      CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_getDnsServerbyPdp, cid : %d, simid: %d\n"),nCid);      
	  SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, DnsServerIP[nCid-1], 6);
   	UINT8* tmp;
	UINT32 serip= 0x00;
	//CFW_GPRS_PDPCONT_INFO  *tmp_pdp;
	//tmp_pdp = PdpContList[nSimID][nCid-1];
	tmp = DnsServerIP[nCid-1];
	
	serip = tmp[3]<<24| tmp[2]<<16| tmp[1]<<8| tmp[0];
       CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_getDnsServerbyPdp, serip 0x%x\n"),serip);      

        // udp_connect(resolv_pcb, &serverIP, DNS_SERVER_PORT);
  	    return serip;
   }
//add end
#endif

//add wuys 2009-10-22
/*
extern INT8 *CFW_TcpipInet_ntoa(in_addr addr);
VOID CFW_GetDnsIP(UINT8 cid, INT8** DnsIP)
{
in_addr iip;
UINT8 tmp_ip[4]= {0x00,0x00,0x00,0x00};
			memcpy(tmp_ip, PdpContList[cid-1]->pDnsAddr, PdpContList[cid-1]->nDnsAddrSize);
			iip.s_addr = *((UINT32*) tmp_ip);
			*DnsIP=CFW_TcpipInet_ntoa(iip);

}
*/
//add end

//add by wuys 2010-05-17, remove detach process from MMI to CSW


#if 0

U32 gprs_Detach (
#ifdef __MMI_DUAL_SIM__
	CFW_SIM_ID nSimId
#endif
) 

{
	U32      rCode = 0;
	
     nwap_uti = FREE_UTI ;
#ifdef __MMI_DUAL_SIM__
	rCode == CFW_GprsAtt( CFW_GPRS_DETACHED, nwap_uti,nSimId);
#else
	rCode == CFW_GprsAtt( CFW_GPRS_DETACHED, nwap_uti);
#endif

	mmi_trace (1,"JDD_LOG: gprs_Detach : Exit[%X]", rCode) ;
	return rCode ;
}
void gprs_detach_stop_timer(
#ifdef __MMI_DUAL_SIM__
		CFW_SIM_ID nSimID_detach
#endif /* __MMI_DUAL_SIM__ */
)
{
#ifdef __MMI_DUAL_SIM__
	mmi_trace(TRUE,"JDD_LOG: Func: %s nSimID_detach=%d", __FUNCTION__, nSimID_detach);
	if (nSimID_detach == CFW_SIM_0)
	{
		StopTimer(GPRS_DETACH_DELAY_TIMER);
	}
	else if (nSimID_detach == CFW_SIM_1)
	{
		StopTimer(GPRS_DETACH_DELAY_TIMER_SIM1);
	}
	
#else  /* No Defined __MMI_DUAL_SIM__ */
	StopTimer(GPRS_DETACH_DELAY_TIMER);
#endif /* __MMI_DUAL_SIM__ */
}
void gprs_detach_timer_callback()
{
#ifdef __MMI_DUAL_SIM__
	gprs_Detach(CFW_SIM_0);
#else
	gprs_Detach();
#endif
}
#ifdef __MMI_DUAL_SIM__
void gprs_detach_timer_callback_sim1()
{
	gprs_Detach(CFW_SIM_1);
}
#endif
void gprs_detach_start_timer(
#ifdef __MMI_DUAL_SIM__
				CFW_SIM_ID nSimID
#endif /* __MMI_DUAL_SIM__ */
				)
{
	U32 delay_time = 2*60*1000; // 2m
#ifdef __MMI_DUAL_SIM__
	if (nSimID == CFW_SIM_0)
	{
		StartTimer(GPRS_DETACH_DELAY_TIMER, delay_time, gprs_detach_timer_callback);
	}
	else if (nSimID == CFW_SIM_1)
	{
		StartTimer(GPRS_DETACH_DELAY_TIMER_SIM1, delay_time, gprs_detach_timer_callback_sim1);
	}
#else  /* No Defined __MMI_DUAL_SIM__ */
	StartTimer(GPRS_DETACH_DELAY_TIMER, delay_time, gprs_detach_timer_callback);
#endif /* __MMI_DUAL_SIM__ */

}




#endif


#ifdef CFW_MULTI_SIM

BOOL start_gprsDetach_timer[CFW_SIM_COUNT] = {FALSE,};
BOOL CFW_isPdpActiveAvailable(CFW_SIM_ID nSimID)
{
   if(storecid[nSimID][0] == 0x00)
   	return FALSE;
   else
   	return TRUE;
}
UINT32 CFW_StartDetachTimer(CFW_SIM_ID nSimID)
{
	 CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_StartDetachTimer begin, active pdp: 0x%x,\n"),storecid[nSimID][0] );      

       if(storecid[nSimID][0] == 0x00)
    	{
    	   BOOL ret;
    	   ret = COS_SetTimerEX(0xFFFFFFFF, CFW_START_GPRSDETACH_TIMER_ID+((UINT8)nSimID), 
                COS_TIMER_MODE_SINGLE, DURATION_BEFORE_DETACH);	
	  if(ret)
	  	{
		   start_gprsDetach_timer[nSimID] = TRUE;
		   	 CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_StartDetachTimer success, nSimID: %d\n"),nSimID);      

	  }else
	  	{
	  	      CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_StartDetachTimer fail\n"));      

	  	}
    	}

	return storecid[nSimID][0];
    	
}


UINT32 CFW_StopDetachTimer(CFW_SIM_ID nSimID)
{
	 CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_StopDetachTimer begin, start detach timer: %d, simid: %d\n"),start_gprsDetach_timer[nSimID],nSimID);      

       if(start_gprsDetach_timer[nSimID])
    	{
    	    CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_StopDetachTimer COS_KillTimerEX\n"));      

    	   COS_KillTimerEX(0xFFFFFFFF, CFW_START_GPRSDETACH_TIMER_ID+((UINT8) nSimID));
		   start_gprsDetach_timer[nSimID] = FALSE;
    	}

	return ERR_SUCCESS;
    	
}
BOOL ispdpactive(CFW_SIM_ID nSimID)
{
     //   CFW_SIM_ID tmpnSimID;
	  bool ret =FALSE;

        UINT8 sim_num =0;
	for(sim_num =0; (CFW_SIM_0+sim_num) <CFW_SIM_COUNT;sim_num++)
		if(((CFW_SIM_0+sim_num)!=nSimID)&&(storecid[sim_num][0]!= 0x00))
			ret = TRUE;
	
	return ret;
}

#else

BOOL start_gprsDetach_timer = FALSE;
BOOL CFW_isPdpActiveAvailable(VOID)
{
   if(storecid[0] == 0x00)
   	return FALSE;
   else
   	return TRUE;
}
UINT32 CFW_StartDetachTimer(VOID)
{
	 CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_StartDetachTimer begin\n"));      

       if(storecid[0] == 0x00)
    	{
    	   BOOL ret;
    	   ret = COS_SetTimerEX(0xFFFFFFFF, CFW_START_GPRSDETACH_TIMER_ID, 
                COS_TIMER_MODE_SINGLE, DURATION_BEFORE_DETACH);	
	  if(ret)
	  	{
		   start_gprsDetach_timer = TRUE;
	  }else
	  	{
	  	      CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_StartDetachTimer fail\n"));      

	  	}
    	}

	return storecid[0];
    	
}


UINT32 CFW_StopDetachTimer(VOID)
{
	 CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_StopDetachTimer begin\n"));      

       if(start_gprsDetach_timer)
    	{
    		 CSW_TRACE(CFW_GPRS_TRACE, TSTXT("CFW_StopDetachTimer COS_KillTimerEX\n"));      

    	   COS_KillTimerEX(0xFFFFFFFF, CFW_START_GPRSDETACH_TIMER_ID);	
		   start_gprsDetach_timer = FALSE;
    	}

	return ERR_SUCCESS;
    	
}
#endif







#endif

