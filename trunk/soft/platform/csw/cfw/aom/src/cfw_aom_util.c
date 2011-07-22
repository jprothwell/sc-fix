#include "cswtype.h"
#include "cfw.h"
#include "sul.h"
#include "cfw_prv.h"
#include "rfm.h"
#include "ts.h"
#include "csw_debug.h"
   
#define GET_0TO7_BITS(x)        (x & 0x000000FF)        // TI from Stack or other device
#define GET_8TO23_BITS(x)       (((x)&0x00FFFF00)>>8)   // For Internal CFW or APP

#define GET_STACK_UTI(x)    GET_0TO7_BITS(x)
#define GET_APP_CFW_UTI(x)  GET_8TO23_BITS(x)

static PFN_CFW_EV_PORC pfn_NotifyProc[CFW_SRV_EV_MAX + 4];
static void *pNotifyProcUserData[CFW_SRV_EV_MAX + 4];

UINT32 CallNotifyFun( CFW_EV *pEvent );

typedef struct _CB_FUN_FOR_UTI
{
    UINT32                  UTI;
    void                    *UserData;
    PFN_CFW_EV_PORC         pfnEvProc;
    struct _CB_FUN_FOR_UTI  *Next;
    struct _CB_FUN_FOR_UTI  *Prev;

} CB_FUN_FOR_UTI;


typedef struct _QUEUE_INSTANCE
{
    UINT8 _QUEUE_DATA[8];
}QUEUE_INSTANCE;

typedef struct _QUEUE_MSG_INTERNAL
{
    UINT8  msg_num;
    UINT8  in_index;
    UINT8  out_index;
    UINT8  queue_max;
    UINT8* pMsgBuffer;
}QUEUE_MSG_INTERNAL;

#define MAX_MSG_NUM   50
PRIVATE char message_buffer[sizeof(CFW_EV) * MAX_MSG_NUM];

PRIVATE QUEUE_INSTANCE g_hCfwQueue;

static CB_FUN_FOR_UTI *UTI_CB_HEAD;
static CB_FUN_FOR_UTI *UTI_CB_TAIL;

UINT32 CFW_Init(VOID)
{
    QUEUE_MSG_INTERNAL* pQ = (QUEUE_MSG_INTERNAL*)&g_hCfwQueue;

	// Initialize System-related routines such as initialize DDI-related information when need.
    CFW_BaseInit();
//    TM_PowerOn();
    
    SUL_ZeroMemory8(message_buffer, SIZEOF(CFW_EV)*MAX_MSG_NUM);
    SUL_ZeroMemory8(pQ, SIZEOF(QUEUE_MSG_INTERNAL));
    pQ->queue_max    = MAX_MSG_NUM;
    pQ->pMsgBuffer   = message_buffer;

    /*Initialize notify event proc*/
    SUL_ZeroMemory32(pfn_NotifyProc, SIZEOF(pfn_NotifyProc) );
    SUL_ZeroMemory32(pNotifyProcUserData, SIZEOF(pNotifyProcUserData) );
    
    // Initialize AOM.
	CFW_AoInit();
    
    CFW_RegSysDefaultProc(CFW_ShellProc);
    
    // Initialize variable UTI_CB_HEAD & UTI_CB_TAIL.
    UTI_CB_HEAD = 0; 
    UTI_CB_TAIL = 0;

	// Init Each CFW Service.
	CFW_ServicesInit();
	CSW_TRACE(CFW_SHELL_TS_ID, "CFW_Init invoked\n");

	return ERR_SUCCESS;
}

UINT32 CFW_Exit(VOID)
{
#ifdef CFW_MULTI_SIM
	UINT8 sim_index = 0;

	for(sim_index = 0;sim_index < CFW_SIM_COUNT;sim_index++)
	{	
		CFW_NwDeRegister(255-sim_index,(CFW_SIM_ID)sim_index) ;	
	}
#else
	CFW_NwDeRegister(255) ;
#endif
//	CFW_ServicesExit();
//	CFW_AoExit();

	return ERR_SUCCESS;
}

BOOL CFW_GetEventQueue(CFW_EV* msg)
{
	QUEUE_MSG_INTERNAL* pQIns = (QUEUE_MSG_INTERNAL*)&g_hCfwQueue;
	CFW_EV* queue          = NULL;

	if( pQIns->msg_num == 0 )
	{
		//CSW_TRACE(CFW_AOM_TS_ID, "CFW_GetEventQueue Null\n");
		return FALSE;
	}

	queue = (CFW_EV*)&pQIns->pMsgBuffer[sizeof(CFW_EV)*pQIns->out_index];

	SUL_MemCopy32(msg, queue, sizeof(CFW_EV));

	pQIns->msg_num--;
	pQIns->out_index++;
	if( pQIns->out_index >= pQIns->queue_max ) /*Wrap*/
	    pQIns->out_index = 0;

	return TRUE;
}

BOOL CFW_ResetEventQueue(VOID)
{
    QUEUE_MSG_INTERNAL* pQIns = (QUEUE_MSG_INTERNAL*)&g_hCfwQueue;
    pQIns->msg_num   = 0;
    pQIns->in_index  = 0;
    pQIns->out_index = 0;

    return TRUE;
}

//
//
//
static UINT8 WhichNotifyService(UINT32 ID)
{
   CSW_TRACE(CFW_AOM_TS_ID, "WhichNotifyService ID : 0x%x \n",ID);

    if ( ID > EV_CFW_NW_RSP_BASE && ID < EV_CFW_NW_RSP_END_ )
    {
        // NW 
        return CFW_SRV_NW_EV_RSP;
    }
    else if ( ID > EV_CFW_CC_RSP_BASE && ID < EV_CFW_CC_RSP_END_ )
    {
        // CM 
        return CFW_SRV_CC_EV_RSP;    
    }
    else if ( ID > EV_CFW_SS_RSP_BASE && ID < EV_CFW_SS_RSP_END_ )
    {
        // SS 
        return CFW_SRV_SS_EV_RSP;
    
    }
    else if ( ID > EV_CFW_SIM_RSP_BASE && ID < EV_CFW_SIM_RSP_END_ )
    {
        // SIM 
        return CFW_SRV_SIM_EV_RSP;
    }
    else if ( ID > EV_CFW_SMS_RSP_BASE && ID < EV_CFW_SMS_RSP_END_ )
    {
        // SMS 
        return CFW_SRV_SMS_EV_RSP;
    }
    else if (( ID == EV_CFW_GPRS_DATA_IND )||( ID == EV_CFW_GPRS_ACT_RSP )||( ID == EV_CFW_GPRS_ATT_RSP )||(ID== EV_CFW_GPRS_CXT_DEACTIVE_IND))
    {
        // GPRS 
        return CFW_SRV_GPRS_EV_RSP;
    
    }
    else if ( ID > EV_CFW_IND_BASE && ID < EV_CFW_IND_END_ )
    {
        // IND 
        return CFW_SRV_EV_IND;
    
    }
    else
    {
        return CFW_SRV_EV_UNKNOW;
    }


}

//
//
//
UINT32 SRVAPI CFW_SetEventProc (
                                UINT8 nServiceId,
                                PVOID pUserData,
                                PFN_CFW_EV_PORC pfnEvProc)
{
	if( nServiceId > CFW_SRV_EV_MAX )
	{
		CSW_TRACE(CFW_AOM_TS_ID, "ERROR ! CFW_SetEventProc nServiceId out off range \n");
		return -1;
	}

	pfn_NotifyProc[nServiceId] = pfnEvProc;

	pNotifyProcUserData[nServiceId] = pUserData;

	return ERR_SUCCESS;
}

//=====================================================================
//
//
//=====================================================================
static CB_FUN_FOR_UTI* MatchExistingUTI(UINT16 nUTI)
{
    CB_FUN_FOR_UTI *pTmp = 0;

    if( UTI_CB_HEAD == 0 )
        return (CB_FUN_FOR_UTI *)0;
    
    pTmp = UTI_CB_HEAD;
    
    do 
    {
        if(pTmp->UTI == nUTI)
            return pTmp;
        else
        {
            pTmp = pTmp->Next;
        }
        
    } while( pTmp );
    
    return (CB_FUN_FOR_UTI *)0;

}



//=====================================================================
//
//
//=====================================================================
UINT32 SRVAPI CFW_SetAoEventNotifyProcEx ( 
                                  UINT16             nUTI,	
                                  PVOID              pUserData, 
                                  PFN_CFW_EV_PORC pfnEvProc)
{
    CB_FUN_FOR_UTI *pNew = 0;
    CB_FUN_FOR_UTI *pTmp = 0;
    
	if((nUTI < CFW_UTI_MIN) || (nUTI > CFW_UTI_MAX))
	{
		CSW_TRACE(CFW_AOM_TS_ID, "ERROR ! CFW_SetAoEventNotifyProcEx nUTI out off range \n");

		return ERR_CFW_UTI_INVALID;
	}

    //Check if there is a same nUTI number in existing CB table.
    pTmp = MatchExistingUTI(nUTI);

    if ( (pTmp) && (!pfnEvProc) )
    {
        //Delete this node
        if(pTmp->Prev)
            (pTmp->Prev)->Next = pTmp->Next;
        else
            UTI_CB_HEAD = pTmp->Next;
        
        if(pTmp->Next)
            (pTmp->Next)->Prev = pTmp->Prev;
        else
            UTI_CB_TAIL = pTmp->Prev;
        
        CFW_FREE(pTmp);

        CSW_TRACE(CFW_AOM_TS_ID, TSTXT("FREE the UTI_CB NODE  %x\n"), (UINT32)pTmp);
    }
    else if( (pTmp) && (pfnEvProc) )
    {
        //Update this node
        pTmp->UTI = nUTI;
        pTmp->UserData = pUserData;
        pTmp->pfnEvProc = pfnEvProc;
    }
    else if( (!pTmp) && (pfnEvProc) )
    {
        //Must be a new one
        pNew = (CB_FUN_FOR_UTI*)CFW_MALLOC( sizeof(CB_FUN_FOR_UTI) );
        
        CSW_TRACE(CFW_AOM_TS_ID, TSTXT("MALLOC the UTI_CB NODE  %x\n"), (UINT32)pNew);

        if(!pNew)
            return ERR_NO_MORE_MEMORY;

        pNew->UTI = nUTI;
        pNew->UserData = pUserData;
        pNew->pfnEvProc = pfnEvProc;
    
        pNew->Prev = UTI_CB_TAIL;
        pNew->Next = 0;
    
        if(UTI_CB_TAIL)
            UTI_CB_TAIL->Next = pNew;
        else
            UTI_CB_HEAD = pNew;
           
        UTI_CB_TAIL = pNew;    
    }   

    return ERR_SUCCESS;
}

UINT32 CFW_PostNotifyEvent (UINT32 nEventId,UINT32 nParam1,UINT32 nParam2,UINT32 nTransId,UINT8 nType)
{
    UINT32 ret = ERR_SUCCESS;
    QUEUE_MSG_INTERNAL* pQIns = (QUEUE_MSG_INTERNAL*)&g_hCfwQueue;
    CFW_EV* queue          = NULL;
    
    CFW_EV Event;
    CFW_EVENT   ExterEv;

    UINT32 APP_CFW_UTI   = GET_APP_CFW_UTI(nTransId);
    CSW_PROFILE_FUNCTION_ENTER(CFW_PostNotifyEvent);
    
    // Form event structure for function call...
    Event.nEventId = nEventId;
    Event.nParam1  = nParam1;
    Event.nParam2  = nParam2;
    Event.nTransId = nTransId;
    Event.nType    = nType;
#ifdef CFW_MULTI_SIM
    Event.nFlag    = (UINT8)(nTransId>>24);
#else
    Event.nFlag    = 0;
#endif


    //************************************************************************
    //Notify Event ==> Function Call                                        
    //************************************************************************    
    Event.nTransId = (Event.nTransId & 0x00FFFF00) >> 8;

    ExterEv.nEventId = Event.nEventId;
    ExterEv.nParam1  = Event.nParam1;
    ExterEv.nParam2  = Event.nParam2;
    ExterEv.nType    = Event.nType;
    ExterEv.nUTI     = (UINT16)Event.nTransId;
    ExterEv.nFlag    = Event.nFlag;
    CSW_TRACE(CFW_AOM_TS_ID, TSTXT("CFW_PostNotifyEvent  APP_CFW_UTI=%x\n"), APP_CFW_UTI);
    if((APP_CFW_UTI >= CFW_UTI_MIN) && (APP_CFW_UTI <= CFW_UTI_MAX))
    {
    // Post to CFW. 
        //Check if this UTI value has been registered in CB function table
        CB_FUN_FOR_UTI *pFunCBUTI = 0;
        pFunCBUTI = MatchExistingUTI((UINT16)APP_CFW_UTI);

        if(pFunCBUTI)
        {
            //Implement Event Proc(Call Back function)
            pFunCBUTI->pfnEvProc(&ExterEv, pFunCBUTI->UserData);

            //Free memory
            CFW_FREE(Event.nParam1); 

            CSW_TRACE(CFW_AOM_TS_ID, TSTXT("FREE the Param1 of EVENT  %x\n"), (UINT32)Event.nParam1);
            
            CSW_PROFILE_FUNCTION_EXIT(CFW_PostNotifyEvent);
            return ERR_SUCCESS;

        }

        //Check if this AO has been registered in CB function table
        Event.nTransId = nTransId;
        ret = CallNotifyFun(&Event);
        if(ret == ERR_SUCCESS)
        {
            CFW_FREE(Event.nParam1); 

            CSW_TRACE(CFW_AOM_TS_ID, TSTXT("FREE the Param1 of EVENT  %x\n"), (UINT32)Event.nParam1);
            
            CSW_PROFILE_FUNCTION_EXIT(CFW_PostNotifyEvent);
            return ERR_SUCCESS;
        }
        
    }
    else if((APP_CFW_UTI >= APP_UTI_MIN) && (APP_CFW_UTI <= APP_UTI_MAX))   
    {
        // Post to MMI/APP. 
        UINT8 nServiceId = 0;
        nServiceId = WhichNotifyService(nEventId);        
        
        if(nServiceId != CFW_SRV_EV_UNKNOW)    
        {
            if(pfn_NotifyProc[nServiceId])
            {
                ret = pfn_NotifyProc[nServiceId] (&ExterEv, pNotifyProcUserData[nServiceId]);  
               
              #if 0 
               if( ( Event.nParam1 > 0x80000000 ) && 
                   ( !(ret & BAL_PROC_DONT_FREE_MEM) ) )
               {
                   CFW_FREE(Event.nParam1);
                                      
                   CSW_TRACE(CFW_AOM_TS_ID, TSTXT("FREE the Param1 of EVENT  %x\n"), (UINT32)Event.nParam1);
               }
               #endif
               CSW_PROFILE_FUNCTION_EXIT(CFW_PostNotifyEvent);
               return ERR_SUCCESS;
            }

        }

    }
    else if((APP_CFW_UTI >= APP_EX_UTI_MIN) && (APP_CFW_UTI <= APP_EX_UTI_MAX))
    {
        ///
        //Dispatch the Notify Event to an extended APP.
        //  
        CFW_FREE(Event.nParam1); 

        CSW_TRACE(CFW_AOM_TS_ID, TSTXT("FREE the Param1 of EVENT  %x\n"), (UINT32)Event.nParam1);

        CSW_PROFILE_FUNCTION_EXIT(CFW_PostNotifyEvent);
        return ERR_SUCCESS;
    }

    
    //************************************************************************
    // Otherwise, manage to process the Queue                               
    //************************************************************************    

    if( pQIns->msg_num >= pQIns->queue_max)
    {
	CSW_TRACE(CFW_AOM_TS_ID, "ERROR ! CFW_PostNotifyEvent queue full \n");
	CSW_PROFILE_FUNCTION_EXIT(CFW_PostNotifyEvent);
	return ERR_CFW_QUEUE_FULL;
    }

    queue = (CFW_EV*)&pQIns->pMsgBuffer[sizeof(CFW_EV) * pQIns->in_index];

    queue->nEventId = nEventId;
    queue->nParam1  = nParam1;
    queue->nParam2  = nParam2;
    queue->nTransId = nTransId;
    queue->nType    = nType;

#ifdef CFW_MULTI_SIM
    queue->nFlag =Event.nFlag ;
	TS_OutPutEvId(Event.nFlag, nEventId);//Hameina[+] 2009.4.8 , output trace
#else
    queue->nFlag =Event.nFlag ;
	TS_OutPutEvId(nEventId);//Hameina[+] 2009.4.8 , output trace
#endif
    
    (pQIns->msg_num)++;
    (pQIns->in_index)++;
    if(pQIns->in_index >= pQIns->queue_max) //Wrap
        pQIns->in_index = 0;
    CSW_PROFILE_FUNCTION_EXIT(CFW_PostNotifyEvent);
    
    return ERR_SUCCESS;
}
