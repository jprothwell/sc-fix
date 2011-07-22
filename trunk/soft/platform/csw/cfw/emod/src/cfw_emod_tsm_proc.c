#include <cswtype.h>
#include <errorcode.h>
#include <itf_msgc.h>
#include <itf_api.h>
#include <cfw.h>
#include <base.h>
#include <cfw_prv.h>
#include <event.h>
#include <sul.h>
#include <cos.h>
#include <ts.h>


#include "cfw_emod_tsm.h"




// 说明1、整个TSM外场测试部分启动一个AO
//2、每一个SCL的API对应一个FUNCTON选择，并且每一个FUNCTON都是以5s自动上报的方式上报。
//3、AO给MMI的事件MMI不能释放此指针，MMI可以根据提取自己需要的信息。
UINT32 CFW_EmodOutfieldTestProc (HAO hAo,CFW_EV* pEvent)
{
    UINT32 nUTI        =0x00000000;    
    UINT32 nEvtId    =0x00000000;
	BOOL   nStartAoMark=FALSE;
    UINT32 result=ERR_CME_UNKNOWN;
    VOID*  nEvParam 										=NULL;
    CFW_EV ev;
	CFW_TSM_GET_INFO*		pGetInfo						=NULL;
	api_CurCellInfoReq_t*  pCurCellInfoReq 					= NULL; 
	api_AdjCellInfoReq_t*  pAdjCellInfoReq 					= NULL;
	api_CurCellInfoInd_t*			pCurrCellInfoInd		=NULL;
	api_AdjCellInfoInd_t*			pAdjCellInfoInd			=NULL;
	CFW_TSM_CURR_CELL_INFO*				pCurrCellInfo			=NULL;
	CFW_TSM_ALL_NEBCELL_INFO*			pNeighborCellInfo		=NULL;
	UINT32 	nPointer=0x000000000;
#ifdef CFW_MULTI_SIM
    CFW_SIM_ID nSimID;
    nSimID = CFW_GetSimCardID(hAo);
#endif
	
    pGetInfo = CFW_GetAoUserData(hAo);
    if((UINT32)pEvent == 0xffffffff)
    {
        SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
        ev.nParam1 = 0xff;
        pEvent = &ev;
        pGetInfo->nState.nNextState=CFW_TSM_IDLE;
    }
    else
    {
        nEvtId = pEvent->nEventId;
        nEvParam = (VOID*)pEvent->nParam1;
    }
    CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_TSMProc()\r\n") );
	if (!(pGetInfo->bClose))
	{
    switch(pGetInfo->nState.nNextState)
    {
    	
        case CFW_TSM_IDLE:			
			if(pGetInfo->SelecNum.nServingCell==1)
			{

				pCurCellInfoReq = (api_CurCellInfoReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_CurCellInfoReq_t));
				if (pCurCellInfoReq == NULL)
					result= ERR_NO_MORE_MEMORY;
				if(result==ERR_NO_MORE_MEMORY)
				{
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("MALLOC failure!!!\r\n") );
					CFW_UnRegisterAO(CFW_SIM_SRV_ID,hAo);
					hAo = HNULL; 
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent (EV_CFW_TSM_INFO_IND,(UINT32)result,0,nUTI|(nSimID<<24), 0xF0);
#else
					CFW_PostNotifyEvent (EV_CFW_TSM_INFO_IND,(UINT32)result,0,nUTI, 0xF0);
#endif
					return result;
				}
				pCurCellInfoReq->StartReport= pGetInfo->SelecNum.nServingCell;
				
#ifdef CFW_MULTI_SIM
				result=CFW_SendSclMessage (API_CURCELL_INFO_REQ, pCurCellInfoReq,nSimID);
#else
				result=CFW_SendSclMessage (API_CURCELL_INFO_REQ, pCurCellInfoReq);
#endif
				if(ERR_SUCCESS!=result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SendSclMessage() failure error code 0x%x\r\n") ,result);
					CFW_UnRegisterAO(CFW_EMOD_SRV_ID,hAo);
					hAo = HNULL; 
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent (EV_CFW_TSM_INFO_IND,(UINT32)result,0,nUTI|(nSimID<<24), 0xF0);
#else
					CFW_PostNotifyEvent (EV_CFW_TSM_INFO_IND,(UINT32)result,0,nUTI, 0xF0);
#endif
				}
				nStartAoMark=TRUE;
			}
			if(pGetInfo->SelecNum.nNeighborCell==1)
		    {
				pAdjCellInfoReq = (api_AdjCellInfoReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_AdjCellInfoReq_t));
				if(pAdjCellInfoReq == NULL)
				    result= ERR_NO_MORE_MEMORY;
				if(result==ERR_NO_MORE_MEMORY)
				{
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("MALLOC failure!!!\r\n") );
					CFW_UnRegisterAO(CFW_EMOD_SRV_ID,hAo);
				    hAo = HNULL; 
					return result;
				}
				pAdjCellInfoReq->StartReport= pGetInfo->SelecNum.nNeighborCell;
#ifdef CFW_MULTI_SIM
				    result=CFW_SendSclMessage (API_ADJCELL_INFO_REQ, pAdjCellInfoReq,nSimID);
#else
				    result=CFW_SendSclMessage (API_ADJCELL_INFO_REQ, pAdjCellInfoReq);
#endif
				if(ERR_SUCCESS!=result)
				{
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_SendSclMessage() failure error code 0x%x\r\n") ,result);
					CFW_UnRegisterAO(CFW_EMOD_SRV_ID,hAo);
					hAo = HNULL; 

#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent (EV_CFW_TSM_INFO_IND,(UINT32)result,0,nUTI|(nSimID<<24), 0xF0);
#else
					CFW_PostNotifyEvent (EV_CFW_TSM_INFO_IND,(UINT32)result,0,nUTI, 0xF0);
#endif	
				}
				nStartAoMark=TRUE;
			}
			if(nStartAoMark==TRUE)
			{
				
	            pGetInfo->nState.nCurrState=CFW_TSM_IDLE;
	            pGetInfo->nState.nNextState=CFW_TSM_REC_IND;
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
			}
			else
			{
				CSW_TRACE(CFW_SIM_TS_ID, TSTXT("No require start tsm function,the ao unregister!!!\n"));
				CFW_UnRegisterAO(CFW_EMOD_SRV_ID,hAo);
				hAo = HNULL; 
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent (EV_CFW_TSM_INFO_IND,(UINT32)result,0,nUTI|(nSimID<<24), 0xF0);
#else
				CFW_PostNotifyEvent (EV_CFW_TSM_INFO_IND,(UINT32)result,0,nUTI, 0xF0);
#endif
			}
    
            break;
            
        case CFW_TSM_REC_IND:
		CSW_TRACE(CFW_SIM_TS_ID, "CFW_TSMProc()--CFW_TSM_REC_IND pEvent->nEventId=0x%x\r\n", pEvent->nEventId);
		nPointer=(UINT32)((UINT8*)pGetInfo+SIZEOF(CFW_TSM_GET_INFO));
		if(pGetInfo->SelecNum.nServingCell==1)
		{
				pCurrCellInfo=(CFW_TSM_CURR_CELL_INFO*)nPointer;
		    	UINT32 nSize=SIZEOF(CFW_TSM_CURR_CELL_INFO);
		
				nPointer=nPointer+nSize;	
	/*		CSW_TRACE(CFW_SIM_TS_ID, "CFW_TSMProc()--nPointer2=0x%x\r\n", nPointer);
			CSW_TRACE(CFW_SIM_TS_ID, "CFW_TSMProc()--pGetInfo=0x%x\r\n", pGetInfo);
			CSW_TRACE(CFW_SIM_TS_ID, "CFW_TSMProc()--CFW_TSM_CURR_CELL_INFO nSize=0x%x\r\n", nSize);
			CSW_TRACE(CFW_SIM_TS_ID, "CFW_TSMProc()--pCurrCellInfo=0x%x\r\n", pCurrCellInfo);
			CSW_TRACE(CFW_SIM_TS_ID, "CFW_TSMProc()--CFW_TSM_CURR_CELL_INFO first add=0x%x\r\n", &pCurrCellInfo->nTSM_Arfcn);
			*/
		}
		if(pGetInfo->SelecNum.nNeighborCell==1)
		{
			pNeighborCellInfo=(CFW_TSM_ALL_NEBCELL_INFO*)nPointer;
			
			UINT32 nSize=SIZEOF(CFW_TSM_ALL_NEBCELL_INFO);
			
			nPointer=nPointer+nSize;
		/*	CSW_TRACE(CFW_SIM_TS_ID, "CFW_TSMProc()--pNeighborCellInfo=0x%x\r\n", pNeighborCellInfo);
			CSW_TRACE(CFW_SIM_TS_ID, "CFW_TSMProc()--CFW_TSM_ALL_NEBCELL_INFO nSize=0x%x\r\n", nSize);
			CSW_TRACE(CFW_SIM_TS_ID, "CFW_TSMProc()--CFW_TSM_ALL_NEBCELL_INFO first add=0x%x\r\n",&pNeighborCellInfo->nTSM_NebCellNUM);
			CSW_TRACE(CFW_SIM_TS_ID, "CFW_TSMProc()--nPointer=0x%x\r\n", nPointer);
			*/
		}
		
        CFW_GetUTI(hAo, &nUTI);
		switch(pEvent->nEventId)
		{

			case API_CURCELL_INFO_IND:

				pCurrCellInfoInd = (api_CurCellInfoInd_t *)nEvParam; 				
				if(CurrCellDataParse(pCurrCellInfo,pCurrCellInfoInd))
				{
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("API_CURCELLINFO_IND   post!\r\n") );
//					UINT32 pTemp=&pCurrCellInfo->nTSM_Arfcn;
					CSW_TRACE(CFW_SIM_TS_ID, "CFW_TSMProc()--pCurrCellInfo---------=0x%x\r\n", &pCurrCellInfo->nTSM_Arfcn);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent (EV_CFW_TSM_INFO_IND,(UINT32)0,CFW_TSM_CURRENT_CELL,nUTI|(nSimID<<24), 0x00);
#else
					CFW_PostNotifyEvent (EV_CFW_TSM_INFO_IND,(UINT32)0,CFW_TSM_CURRENT_CELL,nUTI, 0x00);
#endif
				}
				break;
			case API_ADJCELL_INFO_IND:
				
				pAdjCellInfoInd = (api_AdjCellInfoInd_t*)nEvParam;
				if(NeighborCellDataParse(pNeighborCellInfo, pAdjCellInfoInd))
				{
					CSW_TRACE(CFW_SIM_TS_ID, TSTXT("API_ADJCELLSINFO_IND    post!\r\n") );
					
		//			UINT32 pTemp=&pNeighborCellInfo->nTSM_NebCellNUM;
					CSW_TRACE(CFW_SIM_TS_ID, "CFW_TSMProc()--pNeighborCellInfo----------=0x%x\r\n", &pNeighborCellInfo->nTSM_NebCellNUM);
#ifdef CFW_MULTI_SIM
					CFW_PostNotifyEvent (EV_CFW_TSM_INFO_IND,(UINT32)0,CFW_TSM_NEIGHBOR_CELL,nUTI|(nSimID<<24), 0x00);
#else
					CFW_PostNotifyEvent (EV_CFW_TSM_INFO_IND,(UINT32)0,CFW_TSM_NEIGHBOR_CELL,nUTI, 0x00);
#endif
				}
				break;
			default:
				break;
		}			
			pGetInfo->nState.nCurrState=CFW_TSM_REC_IND;
	        pGetInfo->nState.nNextState=CFW_TSM_REC_IND;
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
            break;
        default:
            CFW_UnRegisterAO(CFW_EMOD_SRV_ID,hAo);
            hAo = HNULL;
            break;
    }
	}
	else
	{
		CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
		CSW_TRACE(CFW_SIM_TS_ID, "CFW_AO_PROC_CODE_CONSUMED tsm close!!!\r\n");
	}
    return ERR_SUCCESS;
}

