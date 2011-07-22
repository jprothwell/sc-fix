/******************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                    */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    cfw_task.h                                                    */
/*                                                                            */
/* Description:                                                               */
/*   Communication Framework Process entry.                                   */
/******************************************************************************/
#include <cfw.h>
#include <event.h>
#include <cfw_prv.h>
#include "api_msg.h"
#include <event_prv.h>
#include "sxs_cfg.h"
#include "cmn_defs.h"

//martin add(should remove to correct H file later)
#define HVY_TIMER_IN             ( 0xFFFFF000          )
//UINT32 CSW_SAT_Init(void* pEvt);
UINT32 CSW_SAT_InitEX(CFW_EV* pEV);
//
// CFW Task entry.
//
#ifdef CFW_MULTI_SIM
UINT8 nQueueid[CFW_SIM_COUNT];
#else
UINT8 nQueueid;
#endif
extern UINT32 CFW_Init(VOID);
extern BOOL g_SAT_INIT_COMPLETE;
extern BOOL g_SAT_IND;
extern BOOL Stack_MsgFree (PVOID pMemBlock);
UINT32 COS_SendRMCEvent(COS_EVENT* pEV)
{
  COS_EVENT nEv;
  SUL_MemCopy8(&nEv,pEV,SIZEOF(COS_EVENT));
  if( EV_RMT_APP_RSP == pEV->nEventId )
  {
    nEv.nEventId = EV_RMT_STTPC_RSP;
    sxr_Send( pEV, CMN_STTPC, SXR_SEND_EVT );
  }
}
static void cfw_ParseEvt ( void* pEvt )
{
    UINT32 *p   = (UINT32*)pEvt;
     
    if(p[0] == EV_CFW_INIT_REQ )
    {
        CSW_TRACE(BASE_BAL_TS_ID, " CFW_Init invoked\n");
        CFW_Init();
        p[0] = 0;
    }

    if(p[0] == API_PS_QUEUECREATE_IND)
    {
        //CFW_CfgNwSetPsQueueId(psInMsg->QueueId);

#ifdef CFW_MULTI_SIM
        api_PSQueueCreateInd_t *psInMsg  = (api_PSQueueCreateInd_t *)(&p[2]);

  	CFW_SIM_ID nSimID = p[1];
	nQueueid[nSimID] = psInMsg->QueueId;
	        CSW_TRACE(BASE_BAL_TS_ID, "API_PS_QUEUECREATE_IND = 0x%x\n", nQueueid[nSimID] );
#else
        api_PSQueueCreateInd_t *psInMsg  = (api_PSQueueCreateInd_t *)(&p[1]);

        nQueueid = psInMsg->QueueId;
        CSW_TRACE(BASE_BAL_TS_ID, "API_PS_QUEUECREATE_IND = 0x%x\n", nQueueid);
#endif
    }
    else if(p[0] == EV_RMT_STTPC_REQ)
    {
      COS_EVENT ev;
      ev.nEventId = EV_RMT_APP_REQ;
      ev.nParam1  = p[1];
      ev.nParam2  = p[2];
      ev.nParam3  = p[3];
      COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);                
    }
    else
    {
           
        CFW_AoScheduler(pEvt);
    }
    
    if(p[0] == API_SIM_FETCH_IND)
    {      
      if(!g_SAT_INIT_COMPLETE && g_SAT_IND)            //to wait the fetchind message
      {
          CSW_TRACE(BASE_BAL_TS_ID, "Func CSW_SAT_InitEX() "); 

          UINT32 *pE   = (UINT32*)pEvt;
          CFW_EV   Event;
          Event.nEventId = *pE++;
          if( EV_TIMER != Event.nEventId)
          {
#ifdef CFW_MULTI_SIM
            Event.nFlag = (UINT8)*pE++;
#endif
            Event.nParam1 = (UINT32)pE;
            CSW_SAT_InitEX(&Event);          
          }
      }
    }

}
static void cfw_ParseMsg ( void * pMsg )
{
    cfw_ParseEvt(pMsg);  

    Stack_MsgFree(pMsg);
}

static void cfw_ParseSignal ( UINT32 * Signal )
{
    UINT32 *p   = Signal;
    	CSW_TRACE(BASE_BAL_TS_ID, "cfw_ParseSignal 1 Signal[0]  = 0x%x, signal[1] = 0x%x\n", Signal[0], Signal[1]);
        if(p[0] >= HVY_TIMER_IN )
        {
            p[1] = p[0] -HVY_TIMER_IN;
            p[0] = EV_TIMER;
        }
    	CSW_TRACE(BASE_BAL_TS_ID, "cfw_ParseSignal 2 Signal[0]  = 0x%x, signal[1] = 0x%x\n", Signal[0], Signal[1]);
        CFW_AoScheduler(Signal);
        
}

static void cfw_Init (void)
{
   
}


const sxs_TaskParse_t cfw_TaskParse =
{
  cfw_Init,
  cfw_ParseMsg,
  cfw_ParseSignal,
  MMI_MBX
};

