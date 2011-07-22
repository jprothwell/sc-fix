#include <cswtype.h>
#include <errorcode.h>
#include <cfw.h>
#include <base.h>
#include <cfw_prv.h>
#include <event.h>
#include <sul.h>
#include <cos.h>
#include <ts.h>
#include <itf_msgc.h>
#include <itf_api.h>

#include "cfw_emod_tsm.h"
#include "dbg.h"

#ifdef CFW_MULTI_SIM
UINT32 CFW_GetCellInfo(CFW_TSM_CURR_CELL_INFO *pCurrCellInfo, CFW_TSM_ALL_NEBCELL_INFO *pNeighborCellInfo,CFW_SIM_ID nSimID)
#else
UINT32 CFW_GetCellInfo(CFW_TSM_CURR_CELL_INFO *pCurrCellInfo, CFW_TSM_ALL_NEBCELL_INFO *pNeighborCellInfo)
#endif
{
	HAO hEm = 0;
	CFW_TSM_GET_INFO *pGetInfo=NULL;
	UINT32 	nPointer=0x000000000;
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_GetCellInfo!\n"));

#ifdef CFW_MULTI_SIM
	UINT32 ret;
	ret = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_GetCellInfo SIM ERROR!\n"));
		return ret;
	}
	hEm = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW,CFW_EMOD_SRV_ID,nSimID);
#else
	CFW_SetServiceId(CFW_EMOD_SRV_ID);
	hEm = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif
	if(0 == hEm)
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	
	 pGetInfo = CFW_GetAoUserData(hEm);
	 if(NULL == pGetInfo)
	{
		return ERR_CFW_INVALID_PARAMETER;
	}
	nPointer=(UINT32)((UINT8*)pGetInfo+SIZEOF(CFW_TSM_GET_INFO));

	if(pGetInfo->SelecNum.nServingCell == 1)
	{
		//modify by wulc for bug 13366
		//pCurrCellInfo=(CFW_TSM_CURR_CELL_INFO*)nPointer;
		SUL_MemCopy8(pCurrCellInfo, (void *)nPointer, sizeof(CFW_TSM_CURR_CELL_INFO));
		
    		UINT32 nSize=SIZEOF(CFW_TSM_CURR_CELL_INFO);
		nPointer=nPointer+nSize;	
	}
	if(pGetInfo->SelecNum.nNeighborCell==1)
	{

		//modify by wulc for bug 13366
		//pNeighborCellInfo=(CFW_TSM_ALL_NEBCELL_INFO*)nPointer;
		SUL_MemCopy8(pNeighborCellInfo, (void *)nPointer, sizeof(CFW_TSM_ALL_NEBCELL_INFO));
	}
	
	return ERR_SUCCESS;
}


#ifdef CFW_MULTI_SIM
UINT32 CFW_EmodOutfieldTestStart(CFW_TSM_FUNCTION_SELECT* pSelecFUN,UINT16 nUTI,CFW_SIM_ID nSimID)
#else
UINT32 CFW_EmodOutfieldTestStart(CFW_TSM_FUNCTION_SELECT* pSelecFUN,UINT16 nUTI)
#endif
{

	HAO hAo                             =0x00000000;    
	CFW_TSM_GET_INFO*    pGetInfo = NULL;
	UINT32 nExtraSize =0x00000000;
#ifdef CFW_MULTI_SIM
	UINT8 ret=ERR_SUCCESS;
	ret = CFW_CheckSimId(nSimID);	
	if( ERR_SUCCESS != ret)
	{
		return ret;
	}
#endif
	
	
    if (CFW_IsFreeUTI(nUTI, CFW_EMOD_SRV_ID) != ERR_SUCCESS)
        return ERR_CFW_UTI_IS_BUSY;
	if(pSelecFUN->nServingCell==1)
	{
		nExtraSize = nExtraSize+SIZEOF(CFW_TSM_CURR_CELL_INFO);
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_TSM_CURR_CELL_INFO    ---= 0x%x\r\n") ,SIZEOF(CFW_TSM_CURR_CELL_INFO));
	}
	if(pSelecFUN->nNeighborCell==1)
	{
		nExtraSize = nExtraSize+SIZEOF(CFW_TSM_ALL_NEBCELL_INFO);
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_TSM_NEIGHBOR_CELL_INFO    ---= 0x%x\r\n") ,SIZEOF(CFW_TSM_ALL_NEBCELL_INFO));
	}
	
    pGetInfo = (CFW_TSM_GET_INFO*)CSW_TMS_MALLOC(SIZEOF(CFW_TSM_GET_INFO)+nExtraSize);
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_TSM_GET_INFO    ---= 0x%x\r\n") ,SIZEOF(CFW_TSM_GET_INFO));
    if (pGetInfo == NULL)
        return ERR_NO_MORE_MEMORY;  // should be updated by macro
    SUL_ZeroMemory32(pGetInfo,SIZEOF(CFW_TSM_GET_INFO)+nExtraSize);
	
	pGetInfo->SelecNum.nServingCell=pSelecFUN->nServingCell;
	pGetInfo->SelecNum.nNeighborCell=pSelecFUN->nNeighborCell;
	pGetInfo->bClose=FALSE;
	
#ifdef CFW_MULTI_SIM
    hAo = CFW_RegisterAo(CFW_EMOD_SRV_ID, pGetInfo, CFW_EmodOutfieldTestProc,nSimID);
#else
    hAo = CFW_RegisterAo(CFW_EMOD_SRV_ID, pGetInfo, CFW_EmodOutfieldTestProc);
#endif
 
    CFW_SetUTI(hAo, nUTI, 0);
    CFW_SetAoProcCode(hAo,CFW_AO_PROC_CODE_CONTINUE);
    return(ERR_SUCCESS);
}
#ifdef CFW_MULTI_SIM
UINT32  CFW_EmodOutfieldTestEnd(UINT16 nUTI,CFW_SIM_ID nSimID)
#else
UINT32  CFW_EmodOutfieldTestEnd(UINT16 nUTI)
#endif
{
	//A TWO MODE:UnRegisterAO  OR NOT POST EVENT TO MMI.
	HAO tsm_hAo;
	UINT32 result;
	CFW_TSM_GET_INFO*		pTsm_procdata					=NULL;
	api_CurCellInfoReq_t*  pCurCellInfoReq 					= NULL; 
	api_AdjCellInfoReq_t*  pAdjCellInfoReq 					= NULL;
	
	
#ifdef CFW_MULTI_SIM
	tsm_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW,CFW_EMOD_SRV_ID,nSimID);
#else
	CFW_SetServiceId(CFW_EMOD_SRV_ID);
	tsm_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif
	if(0 == tsm_hAo)
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("[ERROR]tsm AO is NULL !!\n"));
		//DBG_ASSERT(FALSE, "Tsm AO is NULL!");
		return ERR_CFW_NOT_EXIST_AO;
	}
    pTsm_procdata= CFW_GetAoUserData(tsm_hAo);
	if( NULL == pTsm_procdata )
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("[ERROR]tsm AO-data is NULL !!\n"));
		DBG_ASSERT(FALSE, "Tsm AO-data is NULL!");
		return ERR_CFW_NOT_EXIST_AO;
	}
	pTsm_procdata->bClose=TRUE;
	if(pTsm_procdata->SelecNum.nServingCell==1)
	{
		pCurCellInfoReq = (api_CurCellInfoReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_CurCellInfoReq_t));
	    if (pCurCellInfoReq == NULL)
	      	return ERR_NO_MORE_MEMORY;

	    pCurCellInfoReq->StartReport=0;
#ifdef CFW_MULTI_SIM
	    result=CFW_SendSclMessage(API_CURCELL_INFO_REQ, pCurCellInfoReq,nSimID);
#else
	    result=CFW_SendSclMessage(API_CURCELL_INFO_REQ, pCurCellInfoReq);
#endif
		
	    if(ERR_SUCCESS!=result)
	    {
	    	return result;
	    }
	}
	if(pTsm_procdata->SelecNum.nNeighborCell==1)
	{
	    pAdjCellInfoReq = (api_AdjCellInfoReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_AdjCellInfoReq_t));
	    if(pAdjCellInfoReq == NULL)
	    	return ERR_NO_MORE_MEMORY;
	    pAdjCellInfoReq->StartReport= 0;
#ifdef CFW_MULTI_SIM
	    result=CFW_SendSclMessage(API_ADJCELL_INFO_REQ, pAdjCellInfoReq,nSimID);
#else
	    result=CFW_SendSclMessage(API_ADJCELL_INFO_REQ, pAdjCellInfoReq);
#endif
	    if(ERR_SUCCESS!=result)
	    {
	   		return result;
	    }
	}
	
	result=CFW_UnRegisterAO(CFW_EMOD_SRV_ID,tsm_hAo);
	
	if(result==ERR_SUCCESS)
	{
	
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Close tsm mode success!!!\n"));
		//CFW_UnRegisterAO(CFW_EMOD_SRV_ID,tsm_hAo);
		tsm_hAo = HNULL; 
	#ifdef CFW_MULTI_SIM
		CFW_PostNotifyEvent (EV_CFW_TSM_INFO_END_RSP,0,0, nUTI|(nSimID<<24), 0x00);
	#else
		CFW_PostNotifyEvent (EV_CFW_TSM_INFO_END_RSP,0,0,nUTI, 0x00);
	#endif
		
	}
	else
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("Close tsm mode failure!!!\n"));
	#ifdef CFW_MULTI_SIM
		CFW_PostNotifyEvent (EV_CFW_TSM_INFO_END_RSP,result,0, nUTI|(nSimID<<24), 0xF0);
	#else
		CFW_PostNotifyEvent (EV_CFW_TSM_INFO_END_RSP,result,0,nUTI, 0xF0);
	#endif
	}
	return result;//added by fengwei for warning reduce
}


