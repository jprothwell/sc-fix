/******************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                    */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    cfw_bal_init.h                                                */
/* Description:                                                               */
/*  BAL Initialization functions(CB or CFW init) implementation.              */
/*                                                                            */
/******************************************************************************/

#include "cswtype.h"
#include "ts.h"
#include "errorcode.h"
#include "event.h"
#include "cfw.h"
#include "drv_prv.h"
#include "api_msg.h"
#include "sxs_type.h"
#include "sxr_sbx.h"
#include "sxr_queu.h"
#include "sxr_mem.h"
#include "string.h"
#include "cfw_prv.h"

#ifdef _DEBUG
#define PRINT(fmt, ...)  do {} while(0)
#else
#define PRINT(fmt, ...) do {} while(0)
#endif 
#ifdef __221_UNIT_TEST__
//Lixp add for debug
extern CFW_SIM_ID gSimID;
#endif

#ifdef CFW_MULTI_SIM
CFW_SIM_OC_STATUS SIMOCStatus[CFW_SIM_COUNT] = {CFW_SIM_OC_IDLE,CFW_SIM_OC_IDLE};
#else
CFW_SIM_OC_STATUS SIMOCStatus = CFW_SIM_OC_IDLE;
#endif
//
// Hardware Related functions
//
PRIVATE PFN_AUD_START_SPK_MIC pfnAudStartSpkMic = NULL;
PRIVATE PFN_AUD_STOP_SPK_MIC  pfnAudStopSpkMic  = NULL;

extern PVOID Stack_MsgMalloc(UINT32 nSize);
extern VOID* SUL_MemCopy8 (VOID*  dest, CONST VOID* src, UINT32 count);
extern UINT32 CFW_NwInit();
extern UINT32 SRVAPI CFW_SmsMtInit();
extern UINT32 CFW_SsInit();
extern UINT32 CFW_CcInit();
extern UINT32 CFW_GprsInit();


#ifdef CFW_MULTI_SIM

static UINT32 _SendSclMessage(UINT32 nMsgId, VOID* pMessageBuf, CFW_SIM_ID nSimID)
#else
static UINT32 _SendSclMessage(UINT32 nMsgId, VOID* pMessageBuf)
#endif
{

   if((UINT32*)pMessageBuf != NULL)
   {   
       UINT32* p = (UINT32*)pMessageBuf;  
       
#ifdef CFW_MULTI_SIM   
       *--p = nSimID;
       *--p = nMsgId;
#else
       *--p = nMsgId;

#endif

       PRINT("Send Message 2: %x\n", p);
       sxr_Send (p, API_MBX, SXR_SEND_MSG);
   }
   else
   {
      UINT32  nCallerAdd = 0x00;
      COS_GET_RA(&nCallerAdd);    
#ifdef CFW_MULTI_SIM   
       UINT32* p = (UINT32*)Stack_MsgMalloc( sizeof(UINT32) + sizeof(UINT32) );
#else
       UINT32* p = (UINT32*)Stack_MsgMalloc( sizeof(UINT32) );
#endif
       if(p)
       {
        COS_UpdateMemCallAdd(p, nCallerAdd);
        
#ifdef CFW_MULTI_SIM   
         * p  = nMsgId;
         *++p = nSimID;
         PRINT("Send Message 1: %x\n", (p - 1));
         sxr_Send (--p, API_MBX, SXR_SEND_MSG);
         
#else
         * p = nMsgId;
         PRINT("Send Message 1: %x\n", p);
         sxr_Send (p, API_MBX, SXR_SEND_MSG);


#endif
        
       }
       else
        return ERR_CME_MEMORY_FULL;      
   }
   
	return ERR_SUCCESS;
}

#ifdef CFW_MULTI_SIM

static UINT32 _SendSIMOpenCloseMessage(UINT32 nMsgId, VOID* pMessageBuf, CFW_SIM_ID nSimID)
#else
static UINT32 _SendSIMOpenCloseMessage(UINT32 nMsgId, VOID* pMessageBuf)
#endif
{
#ifdef CFW_MULTI_SIM
   switch(SIMOCStatus[nSimID])
   	{
		case CFW_SIM_OC_IDLE:
			if ( API_SIM_OPEN_IND == nMsgId )
			{
				SIMOCStatus[nSimID] = CFW_SIM_OC_OPEN;
			}
			else
			{
				return ERR_SUCCESS;
			}
		break;
		
		case CFW_SIM_OC_OPEN:
			if ( API_SIM_CLOSE_IND == nMsgId )
			{
				SIMOCStatus[nSimID] = CFW_SIM_OC_CLOSE;
			}
			else
			{
				return ERR_SUCCESS;
			}
		break;
		
		case CFW_SIM_OC_CLOSE:
			if ( API_SIM_OPEN_IND == nMsgId )
			{
				SIMOCStatus[nSimID] = CFW_SIM_OC_OPEN;
			}
			else
			{
				return ERR_SUCCESS;
			}
		break;
		
		default:
			return ERR_INVALID_PARAMETER;
		break;
   	}
#else
   switch(SIMOCStatus)
   	{
		case CFW_SIM_OC_IDLE:
			if ( API_SIM_OPEN_IND == nMsgId )
			{
				SIMOCStatus = CFW_SIM_OC_OPEN;
			}
			else
			{
				return ERR_SUCCESS;
			}
		break;
		
		case CFW_SIM_OC_OPEN:
			if ( API_SIM_CLOSE_IND == nMsgId )
			{
				SIMOCStatus = CFW_SIM_OC_CLOSE;
			}
			else
			{
				return ERR_SUCCESS;
			}
		break;
		
		case CFW_SIM_OC_CLOSE:
			if ( API_SIM_OPEN_IND == nMsgId )
			{
				SIMOCStatus = CFW_SIM_OC_OPEN;
			}
			else
			{
				return ERR_SUCCESS;
			}
		break;
		
		default:
			return ERR_INVALID_PARAMETER;
		break;
   	}
#endif
#ifdef CFW_MULTI_SIM   
   	return _SendSclMessage(nMsgId,pMessageBuf,nSimID);
#else
	return _SendSclMessage(nMsgId,pMessageBuf);
#endif
}
#include "dbg.h"

#ifndef __221_UNIT_TEST__
#ifdef CFW_MULTI_SIM

UINT32 CFW_SendSclMessage(UINT32 nMsgId, VOID* pMessageBuf, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SendSclMessage(UINT32 nMsgId, VOID* pMessageBuf)
#endif
{
   {
      //CSW_TRACE(1, TSTXT("CFW_SendSclMessage nSimID:0x%x \n"), nSimID);
      // COS_Sleep(10);
      #ifdef CFW_MULTI_SIM
      CSW_TRACE(1, TSTXT("CFW_SendSclMessage nSimID:0x%x \n"), nSimID);

      if((CFW_SIM_COUNT < nSimID) || (CFW_SIM_0 > nSimID))
      {
            DBG_ASSERT(0, "CFW_SendSclMessage");
      }
	  #endif
   }
   if( ( API_SIM_OPEN_IND == nMsgId ) || ( API_SIM_CLOSE_IND == nMsgId ))
   {
#ifdef CFW_MULTI_SIM   
   	return _SendSIMOpenCloseMessage(nMsgId,pMessageBuf,nSimID);
#else
	return _SendSIMOpenCloseMessage(nMsgId,pMessageBuf);
#endif
   }
   else
   {
#ifdef CFW_MULTI_SIM   
   	return _SendSclMessage(nMsgId,pMessageBuf,nSimID);
#else
	return _SendSclMessage(nMsgId,pMessageBuf);
#endif
   }
   
}

//
// Allocate memory buffer used by CFW_SendSclMessage.
// Only use this function for CFW_SendSclMessage.
// Don't free this memory buffer, it will automatically free by received task(API Task).
//
PVOID CFW_MemAllocMessageStructure (
									UINT16 nSize // specified allocated block size bytes long
									)
{
    if(nSize == 0)
        return NULL;
    else 
    {
      UINT32  nCallerAdd = 0x00;
      COS_GET_RA(&nCallerAdd);    
      
#ifdef CFW_MULTI_SIM   
        PUINT32 p = (PUINT32)Stack_MsgMalloc(nSize + 4 + 4);
        COS_UpdateMemCallAdd(p, nCallerAdd);

        PRINT("Allocate Buffer: %x\n", p);
        p ++;
        p ++;

#else
        PUINT32 p = (PUINT32)Stack_MsgMalloc(nSize + 4);
        COS_UpdateMemCallAdd(p, nCallerAdd);

        PRINT("Allocate Buffer: %x\n", p);
        p ++;

#endif
       return p;
    }
}

//UINT8 nNsapi, the NSAPI of the data
//UINT8 nQueueId, the index of the queue that stack used
//BOOL bMode, the mode of the data: 
// Possible values for Mode
// API_PDP_ACK   0
//API_PDP_UNACK 1
//UINT16 nDataSize the data size
//UINT8 *pData point to the message data

#ifdef CFW_MULTI_SIM
//#define CMN_SET_SIM_ID(EventId, SimId)      ((EventId) | ((SimId) << 29))
UINT32 CFW_SendSclData(
    UINT8 nNsapi,
    UINT8 nQueueId,
    BOOL bMode, 
    UINT16 nDataSize, 
    UINT8 *pData,
    CFW_SIM_ID nSimID
)
#else
UINT32 CFW_SendSclData(
    UINT8 nNsapi,
    UINT8 nQueueId,
    BOOL bMode, 
    UINT16 nDataSize, 
    UINT8 *pData
)
#endif
{
    // For internal use
    //#define sxr_PopIn(Data, Queue) sxr_QueueLast(Data, Queue)
    //#define sxr_PopOut(Queue)      sxr_QueueRmvFirst(Queue)

    //Msg_t           *psMsg     = NULL;    
    UINT32* psMsg = NULL;
    
    api_PSDataReq_t *psDataMsg = NULL;
    UINT32 Signal[4] = {0, }; 
    	
#ifdef CFW_MULTI_SIM      
    Signal[0] = CMN_SET_SIM_ID(API_PS_QUEUEDATA_REQ, nSimID);    
#else
    Signal[0] = API_PS_QUEUEDATA_REQ;
#endif
    API_PSALLOC_DATAREQ(psMsg, nDataSize);

    // Prepare and send a packet to SNDCP
    *psMsg++ = SND_SEND_REQ;
#ifdef CFW_MULTI_SIM
    *psMsg++ = nSimID;
#endif
    
    //psDataMsg           = (api_PSDataReq_t*)&(psMsg->B);
    psDataMsg             = (api_PSDataReq_t*)psMsg;
    psDataMsg->DataLen    = API_PSDATA_LEN(nDataSize);
    psDataMsg->Ctrl.Mode  = bMode;
    psDataMsg->Ctrl.NSAPI = nNsapi;
    
    SUL_MemCopy8(&psDataMsg->UserData[psDataMsg->DataOffset], pData, nDataSize);

#ifdef CFW_MULTI_SIM
    psMsg -= 2;
#else
    psMsg --;
#endif
    
    sxr_QueueLast(psMsg, nQueueId);
    sxr_Send ((void *)Signal, API_MBX, SXR_SEND_EVT);
    
    return ERR_SUCCESS;
}

#ifdef CFW_MULTI_SIM   
UINT32 CFW_BalSendMessage(
    UINT32 nMailBoxId, 
    UINT32 nEventId, 
    VOID* pData,
    UINT32 nSize,
    CFW_SIM_ID nSimID
)
#else
UINT32 CFW_BalSendMessage(
    UINT32 nMailBoxId, 
    UINT32 nEventId, 
    VOID* pData,
    UINT32 nSize
)
#endif

{ 
    UINT32* p = NULL;
    UINT32  nCallerAdd = 0x00;
    COS_GET_RA(&nCallerAdd);
    
 #ifdef CFW_MULTI_SIM       
     p = CSW_BAL_MALLOC( 4 + 4 + nSize );
 #else
     p = CSW_BAL_MALLOC( 4 + nSize );
 #endif
     if(p)
     {
        COS_UpdateMemCallAdd(p, nCallerAdd);

        p[0] = nEventId;
 #ifdef CFW_MULTI_SIM   
        p[1] = nSimID;
 #endif
        if(pData && nSize)
        {
 #ifdef CFW_MULTI_SIM  
           memcpy((char*)&p[2], pData, nSize);
 #else
           memcpy((char*)&p[1], pData, nSize);
 #endif
        }
        sxr_Send ((void*)p, nMailBoxId, 0); //SXR_SEND_MSG
        return ERR_SUCCESS;
     }
     else
     {
        CSW_TRACE(CSW_TS_ID, "\n malloc fail: size=%d \n\n",4+nSize);  
        return ERR_NO_MORE_MEMORY;
     }
}
#else

#ifdef CFW_MULTI_SIM
UINT32 CFW_SendSclMessage(UINT32 nMsgId, VOID* pMessageBuf, CFW_SIM_ID nSimID)
#else
UINT32 CFW_SendSclMessage(UINT32 nMsgId, VOID* pMessageBuf)
#endif
{
    UINT32  nCallerAdd = 0x00;
    COS_GET_RA(&nCallerAdd);
  
	gSimID = nSimID;
   CSW_TRACE(30, TSTXT("CFW_SendSclMessage gSimID:0x%x \n"), gSimID);
   if((UINT32*)pMessageBuf != NULL)
   {   
       UINT32* p = (UINT32*)pMessageBuf;  
       
#ifdef CFW_MULTI_SIM
       *--p = nSimID;
       *--p = nMsgId;
#else
       *--p = nMsgId;
#endif

       PRINT("Send Message 2: %x\n", p);
       sxr_Send (p, API_MBX, SXR_SEND_MSG);
   }
   else
   {
#ifdef CFW_MULTI_SIM
       UINT32* p = (UINT32*)Stack_MsgMalloc( sizeof(UINT32) + sizeof(UINT32) );
#else
       UINT32* p = (UINT32*)Stack_MsgMalloc( sizeof(UINT32) );
#endif
       if(p)
       {
        COS_UpdateMemCallAdd(p, nCallerAdd);
        
#ifdef CFW_MULTI_SIM   
         * p  = nMsgId;
         *++p = nSimID;
         PRINT("Send Message 1: %x\n", (p - 1));
         sxr_Send (--p, API_MBX, SXR_SEND_MSG);
         
#else
         * p = nMsgId;
         PRINT("Send Message 1: %x\n", p);
         sxr_Send (p, API_MBX, SXR_SEND_MSG);


#endif
        
       }
       else
        return ERR_CME_MEMORY_FULL;

       
   }

   return ERR_SUCCESS;
}

//
// Allocate memory buffer used by CFW_SendSclMessage.
// Only use this function for CFW_SendSclMessage.
// Don't free this memory buffer, it will automatically free by received task(API Task).
//
PVOID CFW_MemAllocMessageStructure (
									UINT16 nSize // specified allocated block size bytes long
									)
{
    UINT32  nCallerAdd = 0x00;
    COS_GET_RA(&nCallerAdd);
    
    if(nSize == 0)
        return NULL;
    else 
    {
#ifdef CFW_MULTI_SIM
        PUINT32 p = (PUINT32)Stack_MsgMalloc(nSize + 4 + 4);

        COS_UpdateMemCallAdd(p, nCallerAdd);

        PRINT("Allocate Buffer: %x\n", p);
        p ++;
        p ++;

#else
        PUINT32 p = (PUINT32)Stack_MsgMalloc(nSize + 4);
        PRINT("Allocate Buffer: %x\n", p);
        p ++;

#endif
       return p;
    }
}

//UINT8 nNsapi, the NSAPI of the data
//UINT8 nQueueId, the index of the queue that stack used
//BOOL bMode, the mode of the data: 
// Possible values for Mode
// API_PDP_ACK   0
//API_PDP_UNACK 1
//UINT16 nDataSize the data size
//UINT8 *pData point to the message data


UINT32 CFW_SendSclData(
    UINT8 nNsapi,
    UINT8 nQueueId,
    BOOL bMode, 
    UINT16 nDataSize, 
    UINT8 *pData
)
{
    // For internal use
    #define sxr_PopIn(Data, Queue) sxr_QueueLast(Data, Queue)
    #define sxr_PopOut(Queue)      sxr_QueueRmvFirst(Queue)

    //Msg_t           *psMsg     = NULL;    
    UINT32* psMsg = NULL;
    
    api_PSDataReq_t *psDataMsg = NULL;
    UINT32 Signal[4] = {0, }; 
    	
    Signal[0] = API_PS_QUEUEDATA_REQ;

    API_PSALLOC_DATAREQ(psMsg, nDataSize);

    // Prepare and send a packet to SNDCP
    *psMsg++ = SND_SEND_REQ;
    
    //psDataMsg           = (api_PSDataReq_t*)&(psMsg->B);
    psDataMsg             = (api_PSDataReq_t*)psMsg;
    psDataMsg->DataLen    = API_PSDATA_LEN(nDataSize);
    psDataMsg->Ctrl.Mode  = bMode;
    psDataMsg->Ctrl.NSAPI = nNsapi;
    
    SUL_MemCopy8(&psDataMsg->UserData[psDataMsg->DataOffset], pData, nDataSize);

    psMsg --;
    
    sxr_PopIn(psMsg, nQueueId);
    sxr_Send ((void *)Signal, API_MBX, SXR_SEND_EVT);
    
    return ERR_SUCCESS;
}

#ifdef CFW_MULTI_SIM   
UINT32 CFW_BalSendMessage(
    UINT32 nMailBoxId, 
    UINT32 nEventId, 
    VOID* pData,
    UINT32 nSize,
    CFW_SIM_ID nSimID
)
#else
UINT32 CFW_BalSendMessage(
    UINT32 nMailBoxId, 
    UINT32 nEventId, 
    VOID* pData,
    UINT32 nSize
)
#endif

{ 
    UINT32  nCallerAdd = 0x00;
    COS_GET_RA(&nCallerAdd);
  
     gSimID = nSimID;
   CSW_TRACE(BASE_BAL_TS_ID, TSTXT("CFW_BalSendMessage gSimID:0x%x \n"), gSimID);
     UINT32* p = NULL;
     
 #ifdef CFW_MULTI_SIM   
     p = CSW_BAL_MALLOC( 4 + 4 + nSize );
 #else
     p = CSW_BAL_MALLOC( 4 + nSize );
 #endif
     if(p)
     {
        COS_UpdateMemCallAdd(p, nCallerAdd);
      
        p[0] = nEventId;
 #ifdef CFW_MULTI_SIM  
        p[1] = nSimID;
 #endif
        if(pData && nSize)
        {
 #ifdef CFW_MULTI_SIM
           memcpy((char*)&p[2], pData, nSize);
 #else
           memcpy((char*)&p[1], pData, nSize);
 #endif
        }
        sxr_Send ((void*)p, nMailBoxId, 0); //SXR_SEND_MSG
        return ERR_SUCCESS;
     }
     else
     {
        CSW_TRACE(BASE_BAL_TS_ID, "\n malloc fail: size=%d \n\n",4+nSize);  
        return ERR_NO_MORE_MEMORY;
     }
}
#endif
//
// pMessagBuf should be used via CFW_MemAllocMessageStructure(..).
// CAUTION: Don't used any others funtion to allocate memory for this parameter!
//


UINT32 CFW_BaseInit()
{   
#ifdef MF32_X86_PLATFORM
	BS32_Init();
#endif
	return 0;
}

BOOL CFW_BalAudCallStart()
{
    if(pfnAudStartSpkMic == NULL)
    {
        DM_StartAudio(); // Default Audio operations.
    }
    else if( (UINT32)pfnAudStartSpkMic == 0xFFFFFFFF) // Send Event to upper-layer APP.
    {
        return FALSE;
    }
    else
    {
        pfnAudStartSpkMic();
    }
    
    return TRUE;
}

BOOL CFW_BalAudCallStop()
{
    if(pfnAudStopSpkMic == NULL)
    {
        DM_StopAudio(); // Default Audio operations.
    }
    else if( (UINT32)pfnAudStopSpkMic == 0xFFFFFFFF) // Send Event to upper-layer APP.
    {
        return FALSE;
    }
    else
    {
        pfnAudStopSpkMic();
    }
    
    return TRUE;
}

//
// Initialize all Services of CFW.
// 
UINT32 CFW_ServicesInit()
{
//    COS_Sleep(1000) ;
    CFW_NwInit();   
    
//    COS_Sleep(4000) ;
#ifdef CFW_MULTI_SIM
CFW_SIM_ID nSimID;
	for (nSimID = CFW_SIM_0; nSimID < CFW_SIM_COUNT; nSimID ++)
	{
            CFW_SimInit(nSimID);
   
	}
#else
    CFW_SimInit();
#endif
//    COS_Sleep(4000) ;
    CFW_SmsMtInit();

//    COS_Sleep(1000) ;
    CFW_SsInit();

//    COS_Sleep(1000) ;
    CFW_CcInit();


#ifdef CFW_GPRS_SUPPORT

    CFW_GprsInit();//add by wuys 2008-04-14
#endif
	return ERR_SUCCESS;
}

//
// Exit all Services of CFW.
// 
UINT32 CFW_ServicesExit()
{
    pfnAudStartSpkMic = NULL;
    pfnAudStopSpkMic  = NULL;
	return TRUE;
}

//
// BAL Interface for CFW.
//

//
// Setup HW related Callback functions.
//
BOOL CFW_BalSetupHwCbs(CFW_PFN_AUD_CBS* pHwFun)
{
    if(pHwFun == (CFW_PFN_AUD_CBS*)0xFFFFFFFF)
    {
    
    }
    else if ( pHwFun == (CFW_PFN_AUD_CBS*)NULL )
    {
    
    }
    else
    {
        pfnAudStartSpkMic = pHwFun->m_pfnAudStartSpkMic; 
        pfnAudStopSpkMic  = pHwFun->m_pfnAudStopSpkMic; 
    }

    return TRUE;
}

