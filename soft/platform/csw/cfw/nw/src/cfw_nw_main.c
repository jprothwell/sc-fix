#include <cswtype.h>
#include <errorcode.h>
//#include <scl.h>
#include "api_msg.h"
#include "cmn_defs.h"
#include <cfw.h>
#include <base.h>
#include <cfw_prv.h>
#include <event.h>
#include <sul.h>
#include <ts.h>
#include "csw_debug.h"
//#include "cfw_cfg.h"

#if (CFW_NW_DUMP_ENABLE==0) && (CFW_NW_SRV_ENABLE==1)

#include "cfw_nw_data.h"
#include "cfw_nw_tool.h"

#ifdef CFW_MULTI_SIM

PRIVATE UINT32 CFW_StackInit(CFW_SIM_ID nSimID);
PRIVATE UINT32 CFW_NwStop(BOOL bPowerOff, UINT16 nUTI, CFW_SIM_ID nSimID);
PRIVATE UINT32 CFW_NwImeiConf(api_ImeiInd_t * pImei, NW_SM_INFO * pSmInfo, UINT32 nUTI, CFW_SIM_ID nSimID);
PRIVATE UINT32 CFW_NwQualReportConf(api_QualReportCnf_t *pQualReport, NW_SM_INFO * pSmInfo, CFW_SIM_ID nSimID);
PRIVATE UINT32 CFW_NwListConf(api_NwListInd_t *pListInd, NW_SM_INFO * pSmInfo, UINT32 nUTI, CFW_SIM_ID nSimID);
PRIVATE UINT32 CFW_NwCsStatusConf(api_NwCsRegStatusInd_t *pCsRegStatus, NW_SM_INFO *pSmInfo, CFW_SIM_ID nSimID);
PRIVATE UINT32 CFW_NwPsStatusConf(api_NwPsRegStatusInd_t *pPsRegStatus, NW_SM_INFO *pSmInfo, UINT32 nUTI,CFW_SIM_ID nSimID);
PRIVATE UINT32 CFW_NwStartStack(UINT8 nBand, CFW_SIM_ID nSimID);
#ifdef CFW_GPRS_SUPPORT

PRIVATE VOID CFW_GprsDeactiveAll(CFW_SIM_ID nSimID);
#endif
#else

PRIVATE UINT32 CFW_StackInit();
PRIVATE UINT32 CFW_NwStop(BOOL bPowerOff, UINT16 nUTI);
PRIVATE UINT32 CFW_NwImeiConf(api_ImeiInd_t * pImei, NW_SM_INFO * pSmInfo, UINT32 nUTI);
PRIVATE UINT32 CFW_NwQualReportConf(api_QualReportCnf_t *pQualReport, NW_SM_INFO * pSmInfo);
PRIVATE UINT32 CFW_NwListConf(api_NwListInd_t *pListInd, NW_SM_INFO * pSmInfo, UINT32 nUTI);
PRIVATE UINT32 CFW_NwCsStatusConf(api_NwCsRegStatusInd_t *pCsRegStatus, NW_SM_INFO *pSmInfo);
PRIVATE UINT32 CFW_NwPsStatusConf(api_NwPsRegStatusInd_t *pPsRegStatus, NW_SM_INFO *pSmInfo,UINT32 nUTI);
PRIVATE UINT32 CFW_NwStartStack(UINT8 nBand);
#ifdef CFW_GPRS_SUPPORT

PRIVATE VOID CFW_GprsDeactiveAll();
#endif
#endif

extern void CFW_SmarchAttStatus(void); // add by wuys for patching QQ, 2010-06-09
PRIVATE UINT32 CFW_NwAoProc (HAO hAo,CFW_EV* pEvent);
PRIVATE HAO NetWork_timer_proc(CFW_EV* pEvent);
#define NW_UTI_SIM NW_UTI_MIN + 0
#ifdef CFW_MULTI_SIM
UINT32 CFW_SetQualReport(BOOL bStart,CFW_SIM_ID nSimID)
#else
UINT32 CFW_SetQualReport(BOOL bStart)
#endif
{
    api_QualReportReq_t *pOutMsg = NULL;
	
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetQualReport %d\n"),bStart);  
    
    pOutMsg = (api_QualReportReq_t *)CFW_MemAllocMessageStructure(SIZEOF(api_QualReportReq_t));
    pOutMsg->StartReport = bStart;

#ifdef CFW_MULTI_SIM
    CFW_SendSclMessage( API_QUAL_REPORT_REQ, pOutMsg, nSimID);
#else
    CFW_SendSclMessage(API_QUAL_REPORT_REQ, pOutMsg);
#endif

	return ERR_SUCCESS;
}
PRIVATE UINT32 CFW_SetFMSIMOpen(HAO hAo, CFW_EV* pEvent)
{
	NW_FM_INFO* pFMInfo;
	UINT32 nUTI = 0x00;
	
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFMSIMOpen \n"));

	if( (hAo == 0) || (pEvent == NULL))
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFMSIMOpen  ao or pEvent ERROR!!! \n"));
		CSW_PROFILE_FUNCTION_EXIT(CFW_SetFlightModeProc);
		return ERR_CFW_INVALID_PARAMETER;
	}
		
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif

	CFW_GetUTI(hAo, &nUTI);

	pFMInfo = CFW_GetAoUserData(hAo);

	switch (pFMInfo->n_CurrStatus)
	{
	case CFW_SM_NW_STATE_IDLE:
		{	
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFMSIMOpen CFW_SM_NW_STATE_IDLE \n"));

			if ((UINT32)pEvent == 0xFFFFFFFF)
			{

				if (pFMInfo->nTryCount >= 0x01)
				{
#ifdef CFW_MULTI_SIM
					CFW_SIM_STATUS sSimStatus= CFW_SIM_STATUS_END;
					sSimStatus = CFW_GetSimStatus(nSimID);

					if(CFW_SIM_ABSENT == sSimStatus)
					{
						CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFlightModeProc NO SIM  simid %d\n"),nSimID);

						//CFW_PostNotifyEvent(EV_CFW_SET_COMM_RSP, ERR_CME_SIM_NOT_INSERTED, 0, nUTI|(nSimID<<24), 0xF0);
						//CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
						//hAo = HNULL;
						//CSW_PROFILE_FUNCTION_EXIT(CFW_SetFlightModeProc);
						//return ERR_SUCCESS;
					}
					else
					{
            CFW_STORELIST_INFO* pStoreList = CFW_MemAllocMessageStructure(sizeof(CFW_STORELIST_INFO));

            CFW_SIM_ID s = CFW_SIM_0;

            for( ; s < CFW_SIM_COUNT; s++ )
            {
              CFW_CfgGetStoredPlmnList(&pStoreList->p_StoreList[s], s);
            }

						if (ERR_SUCCESS !=  CFW_SendSclMessage(API_SIM_OPEN_IND, pStoreList,nSimID))
#else
						if (ERR_SUCCESS !=  CFW_SendSclMessage(API_SIM_OPEN_IND, NULL))
#endif
						{
							CSW_TRACE(CFW_NW_TS_ID, TSTXT("FM send SIM Open ERROR!!!! \n"));
						}
#ifdef CFW_MULTI_SIM
					}
#endif
				}
			}
			CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
		}
		break;

	case CFW_SM_NW_STATE_SEARCH:
		{
			CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFMSIMOpen ERROR!!!! \n"));
		}
	break;
	default:
			CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFMSIMOpen state ERROR!!! \n"));
		break;
	}
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFMSIMOpen end \n"));

	return ERR_SUCCESS;
}

PRIVATE UINT32 CFW_SetCommSIMOpen (
#ifdef CFW_MULTI_SIM
  CFW_SIM_ID 		nSimID
#endif
)
{
	HAO hAo	= 0;
	NW_FM_INFO *pFMBinary = NULL;
	UINT32 ret=ERR_SUCCESS;
	
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetCommSIMOpen() start\r\n"));	
#ifdef CFW_MULTI_SIM	
	if( nSimID >=CFW_SIM_COUNT)
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR ! SIM id error CFW_SetCommSIMOpen()\r\n"));	
		CSW_PROFILE_FUNCTION_EXIT(CFW_SetCommSIMOpen);
		return ERR_CFW_INVALID_PARAMETER;
	}
#endif		
	pFMBinary = (NW_FM_INFO*)CSW_SIM_MALLOC(SIZEOF(NW_FM_INFO));
	if(pFMBinary == NULL)
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !CFW_SetCommSIMOpen() data error\r\n"));	
		CSW_PROFILE_FUNCTION_EXIT(CFW_SetCommSIMOpen);
		return ERR_NO_MORE_MEMORY;
	}  
#ifdef CFW_MULTI_SIM	
	hAo=CFW_RegisterAo(CFW_APP_SRV_ID, pFMBinary, CFW_SetFMSIMOpen,nSimID);
	CFW_SetUTI(hAo, 0x03+nSimID, 1);
#else
	hAo=CFW_RegisterAo(CFW_APP_SRV_ID, pFMBinary, CFW_SetFMSIMOpen);
	CFW_SetUTI(hAo, 0x03, 1);
#endif
	SUL_ZeroMemory32(pFMBinary, SIZEOF(NW_FM_INFO));
	CFW_SetServiceId(CFW_APP_SRV_ID);

	pFMBinary->n_CurrStatus		= CFW_SM_NW_STATE_IDLE;
	pFMBinary->n_PrevStatus		= CFW_SM_NW_STATE_IDLE;
	pFMBinary->nTryCount		= 0x01;
	pFMBinary->nMode			= 0;

	CFW_SetAoProcCode(hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetCommSIMOpen() end\r\n"));
	
	CSW_PROFILE_FUNCTION_EXIT(CFW_SetCommSIMOpen);
	return ret;  
}



UINT32 CFW_SetFlightModeProc(HAO hAo, CFW_EV* pEvent)
{
	NW_FM_INFO* pFMInfo;
	UINT32 nRet = 0;
	CFW_EV ev;
	UINT32 nUTI = 0x00;

	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFlightModeProc()\r\n"));
	CSW_PROFILE_FUNCTION_ENTER(CFW_SetFlightModeProc);

	if( (hAo == 0) || (pEvent == NULL))
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFlightModeProc  ao or pEvent ERROR!!! \n"));
		CSW_PROFILE_FUNCTION_EXIT(CFW_SetFlightModeProc);
		return ERR_CFW_INVALID_PARAMETER;
	}
		
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif

	CFW_GetUTI(hAo, &nUTI);

	pFMInfo = CFW_GetAoUserData(hAo);

	switch (pFMInfo->n_CurrStatus)
	{
	case CFW_SM_NW_STATE_IDLE:
	{	
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFlightModeProc CFW_SM_NW_STATE_IDLE \n"));

		if ((UINT32)pEvent == 0xFFFFFFFF)
		{
#ifdef CFW_MULTI_SIM
			CFW_SIM_STATUS sSimStatus= CFW_SIM_STATUS_END;
			sSimStatus = CFW_GetSimStatus(nSimID);
#endif
			SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
			ev.nParam1 = 0xff;
			pEvent = &ev;
#if 0
#ifdef CFW_MULTI_SIM		
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFlightModeProc if ((UINT32)pEvent == 0xFFFFFFFF) sSimStatus: %d,nSimID : %d,pFMInfo->nTryCount:%d\n"),sSimStatus,nSimID,pFMInfo->nTryCount);
			if(CFW_SIM_ABSENT == sSimStatus)
			{
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFlightModeProc 3333  \n"));

				CFW_PostNotifyEvent(EV_CFW_SET_COMM_RSP, ERR_CME_SIM_NOT_INSERTED, 0, nUTI|(nSimID<<24), 0xF0);
				CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
				hAo = HNULL;
				CSW_PROFILE_FUNCTION_EXIT(CFW_SetFlightModeProc);
				return ERR_SUCCESS;
			}	
#endif
#endif
			if (pFMInfo->nTryCount >= 0x01)
			{
#ifdef CFW_MULTI_SIM					
				if( CFW_ENABLE_COMM ==  pFMInfo->nMode )
#else
				if( 0 ==  pFMInfo->nMode )
#endif
				
				{
				
#ifdef CFW_MULTI_SIM
					nRet = CFW_StackInit(nSimID);
#else
					nRet = CFW_StackInit();
#endif
					CSW_TRACE(CFW_NW_TS_ID, TSTXT("in CFW_SetFlightModePro ccall CFW_StackInit, ret=0x%x\n"), nRet);

					if( ERR_SUCCESS != nRet )
					{
					#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SET_COMM_RSP, nRet, pFMInfo->nStorageFlag, nUTI|(nSimID<<24), 0xF0);
					#else
						CFW_PostNotifyEvent(EV_CFW_NW_SET_FM_RSP, pEvent->nParam1, pEvent->nParam2, nUTI, 0xF0);
					#endif
						CSW_TRACE(CFW_NW_TS_ID, TSTXT("FM Set  Failure! \n"));
						CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SetFlightModeProc);
						return ERR_SUCCESS;
					}
					else
					{
						//if success, just wait stack to restart, unregister AO here
						CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFlightModePro, post success event to MMI!pFMInfo->nMode=%d \n"), pFMInfo->nMode);
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SET_COMM_RSP, pFMInfo->nMode, pFMInfo->nStorageFlag, nUTI|(nSimID<<24), 0x00);
						CFW_PostNotifyEvent(EV_CFW_NW_REG_STATUS_IND, 2, 0, CFW_NW_IND_UTI|(nSimID<<24), 2 );
						CFW_SetCommSIMOpen(nSimID);
						if( 0 == pFMInfo->nStorageFlag)
						{
							CFW_CfgNwSetFM( pFMInfo->nMode, nSimID);//only use dual sim
						}
#else
						CFW_PostNotifyEvent(EV_CFW_NW_REG_STATUS_IND, 2, 0, CFW_NW_IND_UTI, 2 );
						CFW_SetCommSIMOpen();
#endif
						CSW_TRACE(CFW_NW_TS_ID, TSTXT("FM Set  Success \n"));
						CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
						hAo = HNULL;
						CSW_PROFILE_FUNCTION_EXIT(CFW_SetFlightModeProc);
						return ERR_SUCCESS;

					}
					
				}
				else
				{   
					CSW_TRACE(CFW_NW_TS_ID, TSTXT("FM send API_SIM_CLOSE_IND \n"));
					NW_SM_INFO* s_proc = NULL;    // static sm info
    				HAO s_hAo = 0;              // static handle

#ifdef CFW_MULTI_SIM
					if(CFW_SIM_ABSENT == sSimStatus)
					{
						CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFlightModeProc NO SIM  \n"));

						//CFW_PostNotifyEvent(EV_CFW_SET_COMM_RSP, ERR_CME_SIM_NOT_INSERTED, 0, nUTI|(nSimID<<24), 0xF0);
						//CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
						//hAo = HNULL;
						//CSW_PROFILE_FUNCTION_EXIT(CFW_SetFlightModeProc);
						//return ERR_SUCCESS;
					}
					else
					{
						//if (ERR_SUCCESS !=  CFW_SendSclMessage(API_SIM_CLOSE_IND, NULL,nSimID))
#else
						//if (ERR_SUCCESS !=  CFW_SendSclMessage(API_SIM_CLOSE_IND, NULL))
#endif
						//{
						//	CSW_TRACE(CFW_NW_TS_ID, TSTXT("FM send SIM Close ERROR!!!! \n"));
						//	CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
						//	hAo = HNULL;
						//}
#ifdef CFW_MULTI_SIM
					}
					nRet = CFW_NwDeRegister(  CFW_APP_UTI_MIN + nUTI,nSimID);
#else
					nRet = CFW_NwDeRegister(  CFW_APP_UTI_MIN + nUTI);
#endif
#ifdef CFW_MULTI_SIM
				    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
					CFW_SetServiceId(CFW_NW_SRV_ID);
				    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif
					s_proc = CFW_GetAoUserData(s_hAo);
					s_proc->nStpCause = STOP_CAUSE_FM;	
					CSW_TRACE(CFW_NW_TS_ID, TSTXT("in CFW_SetFlightModePro ccall CFW_NwDeRegister, ret=0x%x\n"), nRet);
					if( ERR_SUCCESS != nRet )
					{
#ifdef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SET_COMM_RSP, nRet, pFMInfo->nStorageFlag, nUTI|(nSimID<<24), 0xF0);
#else
						CFW_PostNotifyEvent(EV_CFW_NW_SET_FM_RSP, pEvent->nParam1, pEvent->nParam2, nUTI, 0xF0);
#endif
					
						CSW_TRACE(CFW_NW_TS_ID, TSTXT("FM Cancel  Failure! \n"));
						CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
						hAo = HNULL;
					}
				}

				pFMInfo->n_PrevStatus = pFMInfo->n_CurrStatus;
				pFMInfo->n_CurrStatus = CFW_SM_NW_STATE_SEARCH;

			}
			else
			{
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SET_COMM_RSP, pEvent->nParam1, pFMInfo->nStorageFlag, nUTI|(nSimID<<24), 0xF0);
#else
				CFW_PostNotifyEvent(EV_CFW_NW_SET_FM_RSP, pEvent->nParam1, pEvent->nParam2, nUTI, 0xF0);
#endif
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("FM Option  Failure! \n"));
				// Time out and TODO...
				CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
				hAo = HNULL;
			}

			// CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
		}
		else
		{
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("==> !!! if ((UINT32)pEvent == 0xFFFFFFFF)\r\n"));
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
		}
	}
	break;

	case CFW_SM_NW_STATE_SEARCH:
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SM_NW_STATE_SEARCH! \n"));

		if (pEvent->nEventId == EV_CFW_NW_DEREGISTER_RSP)// used to cancel FM
		{
			if(0x00 == pEvent->nType)
			{    
				HAO s_hAo = 0;              // staitc ao
				NW_SM_INFO *s_proc = NULL;  // static sm
#ifdef CFW_MULTI_SIM
				s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
				CFW_SetServiceId(CFW_NW_SRV_ID);
				s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif	
				s_proc = CFW_GetAoUserData(s_hAo);
				s_proc->bDetached = FALSE;
				s_proc->nAttach = 0xFF;	
				s_proc->sGprsStatusInfo.nCause = 0;
				s_proc->nGprsAttState= CFW_GPRS_DETACHED;//0xFF;
          #ifdef CFW_TCPIP_SUPPORT
          CFW_SmarchAttStatus();
	   #endif
#ifdef CFW_MULTI_SIM
				if( 0 == pFMInfo->nStorageFlag)
				{
					CFW_CfgNwSetFM( pFMInfo->nMode, nSimID);//only use dual sim
				}
				CFW_PostNotifyEvent(EV_CFW_SET_COMM_RSP, pEvent->nParam1, pFMInfo->nStorageFlag, nUTI|(nSimID<<24), 0x00);
#else
				CFW_PostNotifyEvent(EV_CFW_NW_SET_FM_RSP, pEvent->nParam1, pEvent->nParam2, nUTI, 0x00);
#endif
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("Cancel FM post EV_CFW_NW_SET_FM_RSP Success! \n"));
			}
			else
			{
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SET_COMM_RSP, pEvent->nParam1, pFMInfo->nStorageFlag, nUTI|(nSimID<<24), 0xF0);
#else
				CFW_PostNotifyEvent(EV_CFW_NW_SET_FM_RSP, pEvent->nParam1, pEvent->nParam2, nUTI, 0xF0);
#endif
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("Cancel FM EV_CFW_NW_SET_FM_RSP Success! \n"));
			}
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
			CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
			hAo = HNULL;
		}
		else// used to set FM
		{
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("in CFW_SetFlightModePro!pEvent->nType : 0x%x, eventID = 0x%x \n"),pEvent->nType, pEvent->nEventId);
			if(0x00 == pEvent->nType)
			{      
#ifdef CFW_MULTI_SIM
				//CFW_PostNotifyEvent(EV_CFW_SET_COMM_RSP, pEvent->nParam1, pEvent->nParam2, nUTI|(nSimID<<24), 0x00);
#else
				CFW_PostNotifyEvent(EV_CFW_NW_SET_FM_RSP, pEvent->nParam1, pEvent->nParam2, nUTI, 0x00);
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("Set FM post EV_CFW_NW_SET_FM_RSP Success! \n"));
				CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
				hAo = HNULL;
#endif
			}
			else
			{
#ifdef CFW_MULTI_SIM
				//CFW_PostNotifyEvent(EV_CFW_SET_COMM_RSP, pEvent->nParam1, pEvent->nParam2, nUTI|(nSimID<<24), 0xF0);
#else
				CFW_PostNotifyEvent(EV_CFW_NW_SET_FM_RSP, pEvent->nParam1, pEvent->nParam2, nUTI, 0xF0);
				CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
				hAo = HNULL;
#endif
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("Set FM EV_CFW_NW_SET_FM_RSP Success! \n"));
			}
		}
	}
	break;
	default:
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFlightModeProc error! \n"));
		break;
		
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_SetFlightModeProc);
	
	return ERR_SUCCESS;
}

#ifdef CFW_MULTI_SIM
UINT32 CFW_SetComm (
  CFW_COMM_MODE 	nMode,
  UINT8                       nStorageFlag,
  UINT16  		nUTI, 
  CFW_SIM_ID 		nSimID
)
{
	HAO hAo	= 0;
	NW_FM_INFO *pFMBinary = NULL;
	//CFW_SIM_STATUS sSimStatus= CFW_SIM_STATUS_END;
	UINT32 ret=ERR_SUCCESS;
	//UINT32 nAoStatus=0;
	
	
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetComm() start, nMode=%d,nStorageFlag = %d nSimID=%d\r\n"), nMode,nStorageFlag, nSimID);	
	CSW_PROFILE_FUNCTION_ENTER(CFW_SetComm);
	
	if( nSimID >=CFW_SIM_COUNT)
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR ! SIM id error CFW_SetComm()\r\n"));	

		CSW_PROFILE_FUNCTION_EXIT(CFW_SetComm);
		return ERR_CFW_INVALID_PARAMETER;
	}
	
	if(nMode > CFW_ENABLE_COMM)
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR ! nMode error CFW_SetComm()\r\n"));	

		CSW_PROFILE_FUNCTION_EXIT(CFW_SetComm);
		return ERR_CFW_INVALID_PARAMETER;
	}
	
#if 0	
	hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID,  nSimID);
	nAoStatus = CFW_GetAoState(hAo);
	sSimStatus = CFW_GetSimStatus(nSimID);
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetComm() nAoStatuse=0x%x,sSimStatus = 0x%x\r\n"), nAoStatus,sSimStatus);	
	
	if(nAoStatus == CFW_SM_NW_STATE_STOP)
	{
		if(nMode == CFW_DISABLE_COMM)
		{
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetComm()11, return ERR_CME_OPERATION_NOT_ALLOWED\n"));			
			CSW_PROFILE_FUNCTION_EXIT(CFW_SetComm);
			return ERR_CME_OPERATION_NOT_ALLOWED;
		}
	}
	else
	{
		if(nMode == CFW_ENABLE_COMM)
		{
			if(!((CFW_SIM_ABSENT == sSimStatus)&&(nAoStatus == CFW_SM_NW_STATE_IDLE)))
			{
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetComm()22, return ERR_CME_OPERATION_NOT_ALLOWED\n"));			
				CSW_PROFILE_FUNCTION_EXIT(CFW_SetComm);
				return ERR_CME_OPERATION_NOT_ALLOWED;			
			}

		}

	}
		
	hAo = 0;
#endif
	pFMBinary = (NW_FM_INFO*)CSW_SIM_MALLOC(SIZEOF(NW_FM_INFO));
	if(pFMBinary == NULL)
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR ! data error CFW_SetComm()\r\n"));	

		CSW_PROFILE_FUNCTION_EXIT(CFW_SetComm);
		return ERR_NO_MORE_MEMORY;
	}
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetComm() www , simid: 0x%x , pFMBinary: 0x%x\n"),nSimID,pFMBinary);	

	hAo=CFW_RegisterAo(CFW_APP_SRV_ID, pFMBinary, CFW_SetFlightModeProc,nSimID);
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetComm(), hAo: 0x%x \n"),hAo);	

	CFW_SetUTI(hAo, nUTI, 1);

	SUL_ZeroMemory32(pFMBinary, SIZEOF(NW_FM_INFO));
	CFW_SetServiceId(CFW_APP_SRV_ID);

	pFMBinary->n_CurrStatus		= CFW_SM_NW_STATE_IDLE;
	pFMBinary->n_PrevStatus		= CFW_SM_NW_STATE_IDLE;
	pFMBinary->nTryCount		= 0x01;
	pFMBinary->nMode			= nMode;
	pFMBinary->nStorageFlag     = nStorageFlag;

	CFW_SetAoProcCode(hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetComm() end\r\n"));
	
	CSW_PROFILE_FUNCTION_EXIT(CFW_SetComm);
	return ret;  
}


#else
UINT32 CFW_SetFlightMode(
  UINT8 bFM, UINT8 nUTI
)
{
	HAO hAo	= 0;
	NW_FM_INFO *pFMBinary = NULL;

	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFlightMode() start \r\n"));	
	CSW_PROFILE_FUNCTION_ENTER(CFW_SetFlightMode);
	pFMBinary = (NW_FM_INFO*)CSW_SIM_MALLOC(SIZEOF(NW_FM_INFO));
	if(pFMBinary == NULL)
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR ! data error CFW_SetFlightMode()\r\n"));	

		CSW_PROFILE_FUNCTION_EXIT(CFW_SetFlightMode);
		return ERR_NO_MORE_MEMORY;
	}
	
	hAo=CFW_RegisterAo(CFW_APP_SRV_ID, pFMBinary, CFW_SetFlightModeProc);
	CFW_SetUTI(hAo, nUTI, 1);

	SUL_ZeroMemory32(pFMBinary, SIZEOF(NW_FM_INFO));
	CFW_SetServiceId(CFW_APP_SRV_ID);//CFW_NW_SRV_ID

	pFMBinary->n_CurrStatus		= CFW_SM_NW_STATE_IDLE;
	pFMBinary->n_PrevStatus		= CFW_SM_NW_STATE_IDLE;
	pFMBinary->nTryCount		= 0x01;
	pFMBinary->nMode			= bFM;

	CFW_SetAoProcCode(hAo,CFW_AO_PROC_CODE_CONTINUE);

	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_SetFlightMode() end\r\n"));
	CSW_PROFILE_FUNCTION_EXIT(CFW_SetFlightMode);
	
	return(ERR_SUCCESS);  
}

#endif


#ifdef CFW_MULTI_SIM
UINT32 CFW_GetComm (
  CFW_COMM_MODE* pMode, 
  CFW_SIM_ID 	nSimID
)
{
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GetComm() nSimID : %d\r\n"),nSimID);
  	return CFW_CfgNwGetFM( pMode ,nSimID);
}
#else
UINT32 CFW_WriteFlightMode(UINT8 fMode)
{
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_WriteFlightMode() fMode : %d\r\n"),fMode);

	return CFW_CfgNwSetFM( fMode );
}

UINT32 CFW_GetFlightMode( UINT8 *pFM )
{
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GetFlightMode() \r\n"));

  	return CFW_CfgNwGetFM( pFM );
}
#endif

typedef struct _NW_CS_INFO{   
	BOOL  nFrstNCvFlag;
	BOOL  nTimeFlag;
	UINT32 nDelayTime;
	api_NwCsRegStatusInd_t nCSInfo;
}NW_CS_DELAY_INFO;
#ifdef CFW_MULTI_SIM
NW_CS_DELAY_INFO gCSDelayInfo[CFW_SIM_COUNT];
PUBLIC UINT32 CFW_NW_SetCSDelayTime(UINT32 ntime, CFW_SIM_ID nSimID)
{
	gCSDelayInfo[nSimID].nDelayTime = ntime;
	return ERR_SUCCESS; 
}

PRIVATE UINT32 CFW_NW_GetCSDelayTime(CFW_SIM_ID nSimID)
{
	return gCSDelayInfo[nSimID].nDelayTime; 
}

#else
NW_CS_DELAY_INFO gCSDelayInfo;

PUBLIC UINT32 CFW_NW_SetCSDelayTime(UINT32 ntime)
{
	gCSDelayInfo.nDelayTime = ntime;
	return ERR_SUCCESS; 
}

PRIVATE UINT32 CFW_NW_GetCSDelayTime()
{
	return gCSDelayInfo.nDelayTime; 
}
#endif
// When System start, this function will be called one time.
// Typically, used this function to register AO proc,
// don't take a long time to process in this function.
// If you don't care this function, return ERR_SUCCESS.
//
UINT32 CFW_NwInit(VOID) 
{
	UINT32 ret = ERR_SUCCESS;
	NW_SM_INFO * s_proc = NULL;     // static sm
	HAO s_hAo = 0;                  // static ao
	UINT8 bFM;

	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwInit start\n"));
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	for (nSimID = CFW_SIM_0; nSimID < CFW_SIM_COUNT; nSimID ++)
#endif
	{
		s_proc = (NW_SM_INFO*)CFW_MALLOC(SIZEOF(NW_SM_INFO));

		// Clear the proc structure, ensure that there is not existing un-intialized member.
		SUL_ZeroMemory8(s_proc, SIZEOF(NW_SM_INFO));

#ifdef CFW_MULTI_SIM
		CFW_CfgNwGetNetWorkMode(&s_proc->nNetMode, nSimID);
#else
		CFW_CfgNwGetNetWorkMode(&s_proc->nNetMode);
#endif

		s_proc->nNetModeEx = CFW_NW_AUTOMATIC_MODE;
		s_proc->bStaticAO = TRUE;
		s_proc->bReselNW = FALSE;
		s_proc->bDetached = FALSE;
		s_proc->bRoaming = TRUE;
		s_proc->bNetAvailable = FALSE;
		s_proc->nAttach = 0xFF;	
		s_proc->nCsStatusIndCount = 0;
		s_proc->bTimerneed = FALSE;
		s_proc->bTimeDeleted = FALSE;
		s_proc->sGprsStatusInfo.nCause = 0;
		s_proc->nGprsAttState= CFW_GPRS_DETACHED;//0xFF;
#ifdef CFW_TCPIP_SUPPORT
		CFW_SmarchAttStatus();
#endif

#ifdef CFW_MULTI_SIM
		CFW_CfgNwGetFrequencyBand(&s_proc->nBand, nSimID);
		s_hAo = CFW_RegisterAo(CFW_NW_SRV_ID, s_proc, CFW_NwAoProc, nSimID);
#else
		CFW_CfgNwGetFrequencyBand(&s_proc->nBand);
		s_hAo = CFW_RegisterAo(CFW_NW_SRV_ID, s_proc, CFW_NwAoProc);
#endif

		CFW_SetAoState(s_hAo, CFW_SM_NW_STATE_STOP);

		//Add by lixp at 20070703
		//For the function Flight Mode

#ifdef CFW_MULTI_SIM
		CFW_GetComm((CFW_COMM_MODE*)&bFM, nSimID);
		if( CFW_ENABLE_COMM == bFM )
		{
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("bFM = CFW_ENABLE_COMM, start stack, nSimID=%d\n"), nSimID);
			CFW_StackInit(nSimID);
		}
		else if((CFW_DISABLE_COMM==bFM)||(CFW_CHECK_COMM==bFM))
		{
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("bFM = CFW_DISABLE_COMM, don't start stack, nSimID=%d\n"), nSimID);
		}
#else
		CFW_GetFlightMode(&bFM);
		if( 0 == bFM )
		{
			CFW_StackInit();
		}
		else if((1==bFM)||(2==bFM))
		{
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("bFM = CFW_DISABLE_COMM, don't start stack\n"));
		}
#endif
		else
		{
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("FlightMode State Error!, bfm=%d\n"), bFM);
		}
#ifdef CFW_MULTI_SIM
		gCSDelayInfo[CFW_SIM_0+nSimID].nTimeFlag = TRUE;
		gCSDelayInfo[CFW_SIM_0+nSimID].nFrstNCvFlag = TRUE;
		CFW_NW_SetCSDelayTime(3 * 1000 MILLI_SECOND,CFW_SIM_0+nSimID);
#else
		gCSDelayInfo.nFrstNCvFlag = TRUE;
		gCSDelayInfo.nTimeFlag = TRUE;
		CFW_NW_SetCSDelayTime(3 * 1000 MILLI_SECOND );
#endif

	}

	CFW_RegisterCreateAoProc(EV_TIMER, NetWork_timer_proc); // timer
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwInit end\n"));

	return ret;
}


// used to register to the plmn fail, then auto register
PRIVATE HAO NetWork_timer_proc(CFW_EV* pEvent)
{
	// 1000 ms will get prefer plmn from sim and register to the plmn , manual.
	NW_SM_INFO* s_proc = NULL;    // sm info
	HAO s_hAo = 0;
	CSW_PROFILE_FUNCTION_ENTER(NetWork_timer_proc);
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("NetWork_timer_proc  start\n"));

	UINT32 nTimerID = *(UINT32*)(pEvent->nParam1);
    UINT8 uti;
#ifdef CFW_MULTI_SIM
	switch(nTimerID)
	{
		case PRV_CFW_NW_SIM0_TIMER_ID:
			s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, CFW_SIM_0);
			if (s_hAo == 0)
			{
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("s_hAo == 0  ERROR CFW_SIM_0\n"));
				CSW_PROFILE_FUNCTION_EXIT(NetWork_timer_proc);
				return -1;
			}
			s_proc = CFW_GetAoUserData(s_hAo);    
			s_proc->bTimerneed = FALSE;

			// get pref plmn from sim 
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("get pref plmn from sim CFW_SIM_0 \n")); 

			CFW_SimGetPrefOperatorList(NW_UTI_SIM, CFW_SIM_0);
			break;
			
		case PRV_CFW_NW_SIM1_TIMER_ID:
			s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, CFW_SIM_1);
			if (s_hAo == 0)
			{
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("s_hAo == 0  ERROR CFW_SIM_1\n"));
				CSW_PROFILE_FUNCTION_EXIT(NetWork_timer_proc);
				return -1; 
			}
			s_proc = CFW_GetAoUserData(s_hAo);    
			s_proc->bTimerneed = FALSE;

			// get pref plmn from sim 
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("get pref plmn from sim  CFW_SIM_1\n")); 

			CFW_SimGetPrefOperatorList(NW_UTI_SIM, CFW_SIM_1);			
			break;
		case PRV_CFW_NW_SIM2_TIMER_ID:
			s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, CFW_SIM_1+1);
			if (s_hAo == 0)
			{
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("s_hAo == 0  ERROR CFW_SIM_1\n"));
				CSW_PROFILE_FUNCTION_EXIT(NetWork_timer_proc);
				return -1; 
			}
			s_proc = CFW_GetAoUserData(s_hAo);    
			s_proc->bTimerneed = FALSE;

			// get pref plmn from sim 
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("get pref plmn from sim  CFW_SIM_1\n")); 

			CFW_SimGetPrefOperatorList(NW_UTI_SIM, CFW_SIM_1+1);			
			break;
		case PRV_CFW_NW_SIM3_TIMER_ID:
			s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, CFW_SIM_1+2);
			if (s_hAo == 0)
			{
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("s_hAo == 0  ERROR CFW_SIM_1\n"));
				CSW_PROFILE_FUNCTION_EXIT(NetWork_timer_proc);
				return -1; 
			}
			s_proc = CFW_GetAoUserData(s_hAo);    
			s_proc->bTimerneed = FALSE;

			// get pref plmn from sim 
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("get pref plmn from sim  CFW_SIM_1\n")); 

			CFW_SimGetPrefOperatorList(NW_UTI_SIM, CFW_SIM_1+2);			
			break;
		case PRV_CFW_NW_CS_DELAY_SIM0_TIMER_ID:
			CSW_TRACE(CFW_NW_TS_ID, "   DELAY TIME  SIM 0  \n");
			gCSDelayInfo[CFW_SIM_0].nTimeFlag = FALSE;
			CFW_BalSendMessage(11, API_NW_CSREGSTATUS_IND, (VOID*)&gCSDelayInfo[CFW_SIM_0].nCSInfo, sizeof(api_NwCsRegStatusInd_t),CFW_SIM_0);
			break;
			
		case PRV_CFW_NW_CS_DELAY_SIM1_TIMER_ID:
			CSW_TRACE(CFW_NW_TS_ID, "DELAY TIME  SIM 1  \n");
			gCSDelayInfo[CFW_SIM_1].nTimeFlag = FALSE;
			CFW_BalSendMessage(11, API_NW_CSREGSTATUS_IND, (VOID*)&gCSDelayInfo[CFW_SIM_1].nCSInfo, sizeof(api_NwCsRegStatusInd_t),CFW_SIM_1);
			break;
		case PRV_CFW_NW_CS_DELAY_SIM2_TIMER_ID:
			CSW_TRACE(CFW_NW_TS_ID, "DELAY TIME  SIM 1  \n");
			gCSDelayInfo[CFW_SIM_1+1].nTimeFlag = FALSE;
			CFW_BalSendMessage(11, API_NW_CSREGSTATUS_IND, (VOID*)&gCSDelayInfo[CFW_SIM_1+1].nCSInfo, sizeof(api_NwCsRegStatusInd_t),CFW_SIM_1+1);
			break;
		case PRV_CFW_NW_CS_DELAY_SIM3_TIMER_ID:
			CSW_TRACE(CFW_NW_TS_ID, "DELAY TIME  SIM 1  \n");
			gCSDelayInfo[CFW_SIM_1+2].nTimeFlag = FALSE;
			CFW_BalSendMessage(11, API_NW_CSREGSTATUS_IND, (VOID*)&gCSDelayInfo[CFW_SIM_1+2].nCSInfo, sizeof(api_NwCsRegStatusInd_t),CFW_SIM_1+2);
			break;
#ifdef CFW_GPRS_SUPPORT

			 // add by wuys 2010-05-17
		case CFW_START_GPRSDETACH_TIMER_ID:     	 
		    
	             CFW_GetFreeUTI(0, &uti);
		      CFW_GprsAtt(CFW_GPRS_DETACHED, uti, CFW_SIM_0);
		      break;
			   // add by wuys 2010-05-17
             case CFW_START_GPRSDETACH_TIMER_ID_SIM1:     	
		
	             CFW_GetFreeUTI(0, &uti);
		      CFW_GprsAtt(CFW_GPRS_DETACHED, uti, CFW_SIM_1);

			 break;
		case CFW_START_GPRSDETACH_TIMER_ID_SIM2:     	
		
	             CFW_GetFreeUTI(0, &uti);
		      CFW_GprsAtt(CFW_GPRS_DETACHED, uti, (CFW_SIM_1+1));

			 break;
		case CFW_START_GPRSDETACH_TIMER_ID_SIM3:     	
		
	             CFW_GetFreeUTI(0, &uti);
		      CFW_GprsAtt(CFW_GPRS_DETACHED, uti, (CFW_SIM_1+2));

			 break;
#endif
		default:
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("NetWork_timer_proc pEvent->nParam1 EEROR!!! pEvent->nParam1: %d\n"),pEvent->nParam1);
			break;
	}
#else
	switch(nTimerID)
	{
		case PRV_CFW_NW_TIMER_ID:
			CFW_SetServiceId(CFW_NW_SRV_ID);
			s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);

			if (s_hAo == 0)
			{
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("s_hAo == 0  ERROR\n"));
				CSW_PROFILE_FUNCTION_EXIT(NetWork_timer_proc);
				return -1;
			}

			s_proc = CFW_GetAoUserData(s_hAo);    
			s_proc->bTimerneed = FALSE;

			// get pref plmn from sim 
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("get pref plmn from sim  \n")); 

			CFW_SimGetPrefOperatorList(NW_UTI_SIM);
			break;

		case PRV_CFW_NW_CS_DELAY_TIMER_ID:
			gCSDelayInfo.nTimeFlag = FALSE;
			CFW_BalSendMessage(11, API_NW_CSREGSTATUS_IND, (VOID*)&gCSDelayInfo.nCSInfo, sizeof(api_NwCsRegStatusInd_t));
			break;
#ifdef CFW_GPRS_SUPPORT

			 // add by wuys 2010-05-17
		case CFW_START_GPRSDETACH_TIMER_ID:     	 
		  
	             CFW_GetFreeUTI(0, &uti);
		      CFW_GprsAtt(CFW_GPRS_DETACHED, uti);
		      break;
#endif
		default:
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("NetWork_timer_proc pEvent->nParam1 EEROR!!!\n"));
			break;
	}
#endif

	CSW_TRACE(CFW_NW_TS_ID, TSTXT("NetWork_timer_proc end \n")); 
	CSW_PROFILE_FUNCTION_EXIT(NetWork_timer_proc);

	return 1;
}

// start the stack 

PRIVATE UINT32 CFW_StackInit(
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID
#endif
	)
{
    UINT8 nBand = 0;
    // get 1 Full functionality. 
    // 2 Disable transmit RF circuits only. 
    // 3 Disable receive RF circuits only. 
    // 4 Disable both transmit and receive RF circuits 

    // and 
    // get band of the stack
    // then start the stack if needed.
    // for test
   // nBand = API_GSM_900P | API_DCS_1800 | API_GSM_900E;
   
#ifdef CFW_MULTI_SIM

    // get the frequency band from auto call setting in ram
    if(CFW_GetAutoCallFreqBand(&nBand))
    {
      CSW_TRACE(CFW_NW_TS_ID, TSTXT("Currently status is producing Band 0x%x\n") , nBand); 
    }
    else
    {
      UINT32 nRet = CFW_CfgNwGetFrequencyBand(&nBand, nSimID);
      if(ERR_SUCCESS != nRet )
      {
          CSW_TRACE(CFW_NW_TS_ID, TSTXT("Error CFW_CfgNwGetFrequencyBand Failed Ret:0x%x \n") , nRet); 
      }
    }
    CFW_NwStartStack(nBand, nSimID);
#else
    // get the frequency band from auto call setting in ram
    if(CFW_GetAutoCallFreqBand(&nBand))
    {
      CSW_TRACE(CFW_NW_TS_ID, TSTXT("Currently status is producing Band 0x%x\n") , nBand); 
    }
    else
    {
      UINT32 nRet = CFW_CfgNwGetFrequencyBand(&nBand);
      if(ERR_SUCCESS != nRet )
      {
          CSW_TRACE(CFW_NW_TS_ID, TSTXT("Error CFW_CfgNwGetFrequencyBand Failed Ret:0x%x \n") , nRet); 
      }
    }
    
    CFW_NwStartStack(nBand);
#endif

    CSW_TRACE(CFW_NW_TS_ID, TSTXT("start stack Band:0x%x \n") , nBand); 

    // FIXME Start stack ?

    //pSimReset = (api_SimResetReq_t*)CFW_MemAllocMessageStructure(sizeof(api_SimResetReq_t));
    //*pSimReset = 0xbb;
   // CSW_TRACE(0, TSTXT("start qual report \n")); 
   // CFW_SendSclMessage(API_QUAL_REPORT_REQ, pReportMsg);   
    //CFW_NwStartStack(FW_GSM_450 | FW_GSM_900E | FW_DCS_1800);
    //CFW_SimInit();
    // and need send the quality report to stack
    //CFW_NwQualReort(TRUE);
    //////////////////////////////////////////////////////////////////////////
    
    // for test
    //CFW_SendSclMessage(API_SIM_RESET_REQ, NULL);
    return ERR_SUCCESS;
}

//add a timer, if cs state is full service, we will delay some seconds when we receive no service. by Frank start.
#ifdef CFW_MULTI_SIM
BOOL ispdpactive(CFW_SIM_ID nSimID);
#endif
UINT32 CFW_NWDelay(HAO hAo,CFW_EV* pEvent)
{
  VOID* nEvParam = (VOID*)pEvent->nParam1;
  UINT32 nEventId = pEvent->nEventId; 
	UINT32 nCSTime = 0;
  NW_SM_INFO * proc = NULL;
  
  proc = CFW_GetAoUserData(hAo);

#ifdef CFW_MULTI_SIM
    CFW_SIM_ID nSimID =CFW_SIM_END;
    nSimID = CFW_GetSimCardID(hAo);
#endif


#ifdef CFW_MULTI_SIM
	if(API_NW_CSREGSTATUS_IND == nEventId)
	{
		CSW_TRACE(CFW_NW_TS_ID, " CSREGSTATUS  status: %d; SIMID : %d\n",((api_NwCsRegStatusInd_t *)nEvParam)->Status,nSimID);
	}
	nCSTime = CFW_NW_GetCSDelayTime(nSimID);
	   
	if((nCSTime > 0)&&(API_NW_CSREGSTATUS_IND == nEventId)&&gCSDelayInfo[nSimID].nTimeFlag)
	{
		api_NwCsRegStatusInd_t *p = (api_NwCsRegStatusInd_t *)nEvParam;
		switch(p->Status)
		{
			case API_NW_NO_SVCE:
				CSW_TRACE(CFW_NW_TS_ID, "CSREGSTATUS NO_SVCE  \n");
				SUL_MemCopy8((void *)&(gCSDelayInfo[nSimID].nCSInfo), (const void *)nEvParam, SIZEOF(api_NwCsRegStatusInd_t));
#ifdef CFW_GPRS_SUPPORT
        if((API_NW_FULL_SVCE == proc->sStatusInfo.nStatus )&&gCSDelayInfo[nSimID].nFrstNCvFlag && (ispdpactive(nSimID)))
#else
        if((API_NW_FULL_SVCE == proc->sStatusInfo.nStatus )&&gCSDelayInfo[nSimID].nFrstNCvFlag )
#endif
        {

					gCSDelayInfo[nSimID].nFrstNCvFlag = FALSE;
					BOOL bRet = COS_SetTimerEX(0xFFFFFFFF, PRV_CFW_NW_CS_DELAY_SIM0_TIMER_ID + nSimID, COS_TIMER_MODE_SINGLE,nCSTime);	
					CSW_TRACE(CFW_NW_TS_ID, "CSREGSTATUS NO_SVCE FrstNCvFlag bRet %d  \n",bRet);
					return ERR_SUCCESS;
				}
				//return ERR_SUCCESS;
			break;
			case API_NW_LTD_SVCE:
				SUL_MemCopy8((void *)&(gCSDelayInfo[nSimID].nCSInfo), (const void *)nEvParam, SIZEOF(api_NwCsRegStatusInd_t));
			break;
			case API_NW_FULL_SVCE:
				CSW_TRACE(CFW_NW_TS_ID, "CSREGSTATUS FULL_SVCE  \n");
				if( FALSE ==  gCSDelayInfo[nSimID].nFrstNCvFlag )
				{
					CSW_TRACE(CFW_NW_TS_ID, "CSREGSTATUS  Kill Timer   \n");
					COS_KillTimerEX(0xFFFFFFFF, PRV_CFW_NW_CS_DELAY_SIM0_TIMER_ID + nSimID);
				}
				gCSDelayInfo[nSimID].nFrstNCvFlag = TRUE;
			break;
			default: 
			break;
		}
	}
	else if((nCSTime > 0)&&(API_NW_CSREGSTATUS_IND == nEventId)&&(!gCSDelayInfo[nSimID].nTimeFlag))
	{
		CSW_TRACE(CFW_NW_TS_ID, "CSREGSTATUS !gCSDelayInfo[nSimID].nTimeFlag  \n");

		gCSDelayInfo[nSimID].nFrstNCvFlag= TRUE;
		gCSDelayInfo[nSimID].nTimeFlag = TRUE;
	}
	if(API_NW_CSREGSTATUS_IND == nEventId)
	{
		CSW_TRACE(CFW_NW_TS_ID, "CSREGSTATUS  22222  status: %d; SIMID : %d\n",((api_NwCsRegStatusInd_t *)nEvParam)->Status,nSimID);
	}
#else
	nCSTime = CFW_NW_GetCSDelayTime();
	if((nCSTime > 0)&&(API_NW_CSREGSTATUS_IND == nEventId)&&gCSDelayInfo.nTimeFlag)
	{
		api_NwCsRegStatusInd_t *p = (api_NwCsRegStatusInd_t *)nEvParam;
		switch(p->Status)
		{
			case API_NW_NO_SVCE:
				if((API_NW_FULL_SVCE == proc->sStatusInfo.nStatus)&&gCSDelayInfo.nFrstNCvFlag)
				{
					gCSDelayInfo.nFrstNCvFlag = FALSE;

					COS_SetTimerEX(0xFFFFFFFF, PRV_CFW_NW_CS_DELAY_TIMER_ID , COS_TIMER_MODE_SINGLE, nCSTime);	
					SUL_MemCopy8(&gCSDelayInfo.nCSInfo, nEvParam, SIZEOF(api_NwCsRegStatusInd_t));
					return ERR_SUCCESS;
				}
				//return ERR_SUCCESS;
			break;
			case API_NW_FULL_SVCE:
				if( FALSE ==  gCSDelayInfo.nFrstNCvFlag )
				{
					COS_KillTimerEX(0xFFFFFFFF, PRV_CFW_NW_CS_DELAY_TIMER_ID);
				}
				gCSDelayInfo.nFrstNCvFlag = TRUE;
			break;
			default: 
			break;
		}
	}
	else if((nCSTime > 0)&&(API_NW_CSREGSTATUS_IND == nEventId)&&(!gCSDelayInfo.nTimeFlag))
	{
		gCSDelayInfo.nTimeFlag = TRUE;
		gCSDelayInfo.nFrstNCvFlag= TRUE;
	}
#endif
  return ERR_CONTINUE;

}
//
// NW AO process entry.
// There are some action will done in this function:
// 1> ??
// 2> ?? 
//
extern void * memcpy(void *,const void *,size_t);
PRIVATE UINT32 CFW_NwAoProc (HAO hAo,CFW_EV* pEvent)
{
    UINT32 ret = ERR_SUCCESS;
    UINT32 nState    = 0;       // sm state
    NW_SM_INFO * proc = NULL;
    UINT32 nEventId = 0;       // message id from stack
    VOID* nEvParam = NULL;
    UINT32 nUTI = 0;

    HAO s_hAo = 0;              // staitc ao
    NW_SM_INFO *s_proc = NULL;  // static sm
    UINT32 s_nState = 0;        // static state
    CFW_EV ev;

    CSW_PROFILE_FUNCTION_ENTER(CFW_NwAoProc);
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("In CFW_NwAoProc start\n"));

    	
#ifdef CFW_MULTI_SIM
    CFW_SIM_ID nSimID =CFW_SIM_END;
    nSimID = CFW_GetSimCardID(hAo);
#endif
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("In CFW_NwAoProc after get sim id\n"));
	// verify the parameter
    if((UINT32)pEvent == 0xffffffff)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("In CFW_NwAoProc get message from AT \n"));
        SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
        ev.nParam1 = 0xff;
        pEvent = &ev;
    }

    if( (hAo == 0) || (pEvent == NULL))
    {
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc (hAo == 0) || (pEvent == NULL) ERROR!!\n"));
	CSW_PROFILE_FUNCTION_EXIT(CFW_NwAoProc);
        return ERR_CFW_INVALID_PARAMETER;
    }
    else 
    {
        nEvParam = (VOID*)pEvent->nParam1;
        //CFW_SetAoState(hAo)
        proc = CFW_GetAoUserData(hAo);
        nState = CFW_GetAoState(hAo);
        nEventId = pEvent->nEventId; 
        CFW_GetUTI(hAo, &nUTI);
        // get static information
		
#ifdef CFW_MULTI_SIM
        s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
        CFW_SetServiceId(CFW_NW_SRV_ID);
        s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif
	
        s_proc = CFW_GetAoUserData(s_hAo);
        s_nState = CFW_GetAoState(s_hAo);
    }
#ifdef CFW_MULTI_SIM  
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc nSIMID = 0x%x state = 0x%x, s_state = 0x%x eventId = 0x%x nUTI = 0x%x\n"),
    	 nSimID,
        nState, 
        s_nState, 
        nEventId,
        nUTI);
#else
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc state = 0x%x, s_state = 0x%x eventId = 0x%x nUTI = 0x%x\n"),
        nState, 
        s_nState, 
        nEventId,
        nUTI);
#endif
//for sim proc
     if(pEvent->nEventId == EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP)
     {
         //nEvParam the data
         if(pEvent->nType == 0)
         {
            UINT8 plmn[6];
             // success
            CSW_TRACE(CFW_NW_TS_ID, "EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP    PLMNs value=%x!, plmn num = 0x%x\n",pEvent->nParam1, pEvent->nParam2);
			SUL_MemCopy8(plmn, (VOID *)pEvent->nParam1, 6);
            CSW_TRACE(CFW_NW_TS_ID, "auto register to:  \n");
		//CFW_Dump(CFW_NW_TS_ID, (UINT8 *)pEvent->nParam1, 6 );	

#ifdef CFW_MULTI_SIM
		CFW_NwSetRegistration(plmn, CFW_NW_MANUAL_MODE, 1, nSimID);
#else
		CFW_NwSetRegistration(plmn, CFW_NW_MANUAL_MODE, 1);
#endif
	
		s_proc->bReselNW = FALSE;
		// free the param1
		CFW_FREE(pEvent->nParam1);
         }
	  else
	  {
	     // fail 
            CSW_TRACE(CFW_NW_TS_ID, "EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP    fail \n");

	  } 
         CSW_PROFILE_FUNCTION_EXIT(CFW_NwAoProc);
	  return ERR_SUCCESS;
     }
#ifdef CFW_MULTI_SIM
  if (pEvent->nEventId == API_NW_STORELIST_IND)
  {
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("Store Monitor List for Sim %d\n"), nSimID);
    CFW_StoredPlmnList* pStoreList = (CFW_StoredPlmnList*)nEvParam;
    CFW_CfgSetStoredPlmnList(pStoreList, nSimID);
    return ERR_SUCCESS;
  }
#else
//TODO...
#endif  
     ret = CFW_NWDelay(hAo,pEvent);
     if( ERR_CONTINUE != ret )
     	return ret;
//
    // network sm state 
    if (nState == CFW_SM_STATE_IDLE)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("In CFW_SM_STATE_IDLE\n"));
        if (pEvent->nParam1 == 0xff)    // req to stack
        {
            if (proc->nMsgId == API_NW_LIST_REQ)
            {
                if (s_nState == CFW_SM_NW_STATE_IDLE)
                {
                    CSW_TRACE(CFW_NW_TS_ID, TSTXT("send API_NW_LIST_REQ to stack\n"));
#ifdef CFW_MULTI_SIM
                    CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody, nSimID);
#else
                    CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody);
#endif
	
                    CFW_SetAoState(s_hAo, CFW_SM_NW_STATE_IDLE);
                    CFW_SetAoState(hAo, CFW_SM_NW_STATE_IDLE);
                }
                else
                {
                    CSW_TRACE(CFW_NW_TS_ID, TSTXT("list req should be called in idle mode\n"));
                    CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_NW_GET_AVAIL_OPERATOR_RSP, ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED, 0 ,
                                        nUTI|(nSimID<<24), 0xF0 );				
#else
                    CFW_PostNotifyEvent(EV_CFW_NW_GET_AVAIL_OPERATOR_RSP, ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED, 0 ,
                                        nUTI, 0xF0 );
#endif
				
                }
                
            }
            else if (proc->nMsgId == API_QUAL_REPORT_REQ)
            {
#ifdef CFW_MULTI_SIM
                CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody, nSimID);
#else
                CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody);
#endif
			
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("send API_QUAL_REPORT_REQ to stack\n"));
                //s_proc->nCSQUTI = nUTI;
                CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);

                //CFW_SetAoState(hAo, s_nState);
            }
            else if (proc->nMsgId == API_IMEI_REQ)
            {
#ifdef CFW_MULTI_SIM
                CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody, nSimID);
#else
                CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody);
#endif

                CSW_TRACE(CFW_NW_TS_ID, TSTXT("send API_IMEI_REQ to stack\n"));
                
                CFW_SetAoState(hAo, s_nState);
            }
            else if ((proc->nMsgId == API_NW_SEL_REQ) || (proc->nMsgId == API_NW_RESEL_REQ))
            {
                if (s_nState == CFW_SM_NW_STATE_IDLE)
                {      
                    if (s_proc->nNetMode != s_proc->nNetModeEx)
                    {
                        api_NwSelModeReq_t * pSelMode = (api_NwSelModeReq_t *)CFW_MemAllocMessageStructure(SIZEOF(api_NwSelModeReq_t));
                        pSelMode->Mode = s_proc->nNetMode == API_NW_AUTOMATIC_MODE ? API_NW_AUTOMATIC_MODE : API_NW_MANUAL_MODE;

#ifdef CFW_MULTI_SIM
                        CFW_SendSclMessage( API_NW_SELMODE_REQ, pSelMode, nSimID);
#else
			   CFW_SendSclMessage(API_NW_SELMODE_REQ, pSelMode); // add later
#endif

                        CSW_TRACE(CFW_NW_TS_ID, TSTXT("send API_NW_SELMODE_REQ to stack first mode = 0x%x\n"), pSelMode->Mode);   
                        s_proc->nNetModeEx = s_proc->nNetMode;

#ifdef CFW_MULTI_SIM
			            CFW_CfgNwSetNetWorkMode(s_proc->nNetMode, nSimID);
#else
			            CFW_CfgNwSetNetWorkMode(s_proc->nNetMode);
#endif
                    }
                    if (proc->nMsgId == API_NW_SEL_REQ)
                    {
                        api_NwSelReq_t *pSelPlmn = NULL;                        
                        pSelPlmn = (api_NwSelReq_t *)CFW_MemAllocMessageStructure(SIZEOF(api_NwSelReq_t));
                        plmnArray2Api(s_proc->nAttOperatorId, pSelPlmn->PlmnId); // covert to api format
                        //CSW_TRACE(CFW_NW_TS_ID, TSTXT(" stack manual mode plmnid = %x%x%x\n"),
                        //    pSelPlmn->PlmnId[0],
                         //   pSelPlmn->PlmnId[1],
                        //    pSelPlmn->PlmnId[2]);    
			//CSW_TC_MEMBLOCK(CFW_NW_TS_ID, s_proc->nAttOperatorId, 6, 16);
			//CSW_TC_MEMBLOCK(CFW_NW_TS_ID, pSelPlmn->PlmnId, 3, 16);
                        proc->pMsgBody = pSelPlmn;

#ifdef CFW_MULTI_SIM
                        CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody, nSimID);
#else
                        CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody);
#endif
						
                        CSW_TRACE(CFW_NW_TS_ID, TSTXT("send API_NW_SEL_REQ to stack\n"));  
                    }                    
                    else
                    {
#ifdef CFW_MULTI_SIM
                        CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody, nSimID);
#else
                        CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody);
#endif

                        CSW_TRACE(CFW_NW_TS_ID, TSTXT("send API_NW_RESEL_REQ to stack\n"));
                    }                                            
                    
                    CFW_SetAoState(s_hAo, CFW_SM_NW_STATE_SEARCH); // set the static state
                    
                    s_proc->nCOPSUTI = nUTI;  // used for sel or resel

                    CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);       // un register the AO         
                    
                    //CFW_SetAoState(hAo, CFW_SM_NW_STATE_SEARCH);
                    
                }
                else
                {
                    CSW_TRACE(CFW_NW_TS_ID, TSTXT(" sel or resel should be called in idle mode\n"));
                    CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);

#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_NW_SET_REGISTRATION_RSP, ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED, 0,
                                        nUTI|(nSimID<<24), 0xF0 );				
#else
					CFW_PostNotifyEvent(EV_CFW_NW_SET_REGISTRATION_RSP, ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED, 0,
                                        nUTI, 0xF0 );
#endif
                }
            }
            else if (proc->nMsgId == API_START_REQ)
            {
                // send the req to stack       
                if (s_nState == CFW_SM_NW_STATE_STOP)
                {
                    UINT8 *pBand = NULL;
                    pBand = (UINT8 *)CFW_MemAllocMessageStructure(SIZEOF(UINT8));
                    *pBand = proc->nBand;
                    proc->pMsgBody = pBand;

#ifdef CFW_MULTI_SIM
                    CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody, nSimID);
#else
                    CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody);
#endif
					
                    CSW_TRACE(CFW_NW_TS_ID, TSTXT("send API_START_REQ to Stack\n"));
                    if (s_proc->bDetached || s_proc->bChangeBand)  //whether detached first
                    {
                        CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc if (s_proc->bDetached || s_proc->bChangeBand)\n"));  

                        //s_proc->bDetached = FALSE;
                        //CSW_TRACE(CFW_NW_TS_ID, TSTXT("send API_SIM_OPEN_IND to Stack\n"));
                        //CFW_SendSclMessage(API_SIM_OPEN_IND, 0);    // need sim open for stack
                        
                    }
                    else // and need send quality report req to stack too                    
                    {
                        api_QualReportReq_t *pOutMsg = NULL;
                        
                        pOutMsg = (api_QualReportReq_t *)CFW_MemAllocMessageStructure(SIZEOF(api_QualReportReq_t));
                        pOutMsg->StartReport = TRUE;
                        CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc !!!!! if (s_proc->bDetached || s_proc->bChangeBand)\n"));  

#ifdef CFW_MULTI_SIM
                        CFW_SendSclMessage( API_QUAL_REPORT_REQ, pOutMsg, nSimID);
#else
                        CFW_SendSclMessage(API_QUAL_REPORT_REQ, pOutMsg);
#endif
						
                    }

                    CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);                                        
                    CFW_SetAoState(s_hAo, CFW_SM_NW_STATE_SEARCH);
                }
                else
                {
                    CSW_TRACE(CFW_NW_TS_ID, TSTXT(" start stack req should be called in stop state\n"));
                    CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
                    //CFW_PostNotifyEvent(EV_CFW_NW_ERROR_NOTIFY, ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED, 0 ,nUTI, 0);
                }
                
            }
            else if (proc->nMsgId == API_STOP_REQ)
            {
#ifdef CFW_MULTI_SIM
                CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody, nSimID);
#else
                CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody);
#endif
			
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("send API_STOP_REQ to stack\n"));
                if (!s_proc->bChangeBand)   // just detach not change band
                {
                    s_proc->bDetached = TRUE;   // flag for detach
                    s_proc->bNetAvailable = FALSE;//HAMEINA SETCOMM
                }
                else
                {
                    s_proc->nBandUTI = nUTI;
                }
                
                // set the static ao state
                CFW_SetAoState(s_hAo, CFW_SM_NW_STATE_STOP);                
                
                CFW_SetAoState(hAo, CFW_SM_NW_STATE_STOP);
                
            }
            else if (proc->nMsgId == API_NW_PSATTACH_REQ)
            {
                // ps attach
                // should check the state of the gprs att state
                if (s_nState == CFW_SM_NW_STATE_IDLE)
                {
                	UINT32 gprs_ret = ERR_SUCCESS;
                    CSW_TRACE(CFW_NW_TS_ID, TSTXT("send API_NW_PSATTACH_REQ to stack\n"));

#ifdef CFW_MULTI_SIM
                    gprs_ret = CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody, nSimID);
#else
                    gprs_ret = CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody);
#endif
			if (gprs_ret ==ERR_SUCCESS)
			{
				s_proc->nGprsAttState = CFW_GPRS_ATTACHED;		 
          #ifdef CFW_TCPIP_SUPPORT
          CFW_SmarchAttStatus();
	   #endif
			}
			CFW_GetUTI(hAo, &nUTI);

                    CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);  
			      s_proc->nAttach = 1;
				  s_proc->nAttachUTI = nUTI;

                }
                else
                {
                    CSW_TRACE(CFW_NW_TS_ID, TSTXT("ps attach should be called the attach in idle mode\n"));
                    CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
			nUTI = s_proc->nAttachUTI;
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_GPRS_ATT_RSP, ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED, 0,
                                        nUTI|(nSimID<<24), 0xF1 );				
#else
                    CFW_PostNotifyEvent(EV_CFW_GPRS_ATT_RSP, ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED, 0,
                                        nUTI, 0xF1 );
#endif
                }
                
            }
            else if (proc->nMsgId == API_NW_PSDETACH_REQ)
            {
                // ps detach
                // should check the state of the gprs att state
                if (s_nState == CFW_SM_NW_STATE_IDLE)
                {
                	UINT32 gprs_ret = ERR_SUCCESS;
                    CSW_TRACE(CFW_NW_TS_ID, TSTXT("send API_NW_PSDETACH_REQ to stack\n"));

#ifdef CFW_MULTI_SIM
                    gprs_ret =CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody, nSimID);
#else
                    gprs_ret = CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody);
#endif
			if (gprs_ret ==ERR_SUCCESS)
			{
				s_proc->nGprsAttState = CFW_GPRS_DETACHED;		 
                      #ifdef CFW_TCPIP_SUPPORT
          CFW_SmarchAttStatus();
	   #endif
			}

                     CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);  
		      s_proc->nAttach = 0;					
                }
                else
                {
                    CSW_TRACE(CFW_NW_TS_ID, TSTXT("pa detach should be called the detach in idle mode\n"));
                    CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);

#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_GPRS_ATT_RSP, ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED, 0, 
                                        nUTI|(nSimID<<24), 0xF0 );				
#else
                    CFW_PostNotifyEvent(EV_CFW_GPRS_ATT_RSP, ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED, 0, 
                                        nUTI, 0xF0 );
#endif
                }                
            }
            else
            {
                CSW_TRACE(CFW_NW_TS_ID, TSTXT(" The req not supported proc->nMsgId = 0x%x\n"), proc->nMsgId);
                CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
                //CFW_PostNotifyEvent(EV_CFW_NW_ERROR_NOTIFY, ERR_CME_OPERATION_NOT_SUPPORTED, 0, nUTI, 0);
            }

        }
        else
        {   
            CSW_TRACE(CFW_NW_TS_ID, TSTXT(" should not Receive the pEvent->nParam1 = 0x%x\n"), pEvent->nParam1);
            //CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
            // not req
        }
        //CFW_StackInit();
    }
    else if (nState == CFW_SM_NW_STATE_STOP)    // Stop
    {
        // static AO
        if (proc->bStaticAO)
        {
            if (nEventId == API_NW_PSREGSTATUS_IND)
            {
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("receive API_NW_PSREGSTATUS_IND at stop state\n"));
            }
            else if (nEventId == API_NW_CSREGSTATUS_IND)
            {
                api_NwCsRegStatusInd_t *p = (api_NwCsRegStatusInd_t *)nEvParam;
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("receive API_NW_CSREGSTATUS_IND at stop state\n"));

#ifdef CFW_MULTI_SIM
        				CFW_NwCsStatusConf(p, proc, nSimID);
#else
        				CFW_NwCsStatusConf(p, proc);
#endif

                //Fix bug 16947 and 16888

               	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc p->Status %d\n"),p->Status);  

                if ((p->Status == API_NW_NO_SVCE) || (p->Status == API_NW_LTD_SVCE))
                {
                	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc if (p->Status == API_NW_NO_SVCE)\n"));  

                    CFW_SetAoState(hAo, CFW_SM_NW_STATE_STOP); 
                }
                else
                {
                	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc !!!!   if (p->Status == API_NW_NO_SVCE)\n"));  

                  CFW_SetAoState(hAo, CFW_SM_NW_STATE_IDLE);                
                }
            }
            else if (nEventId == API_QUAL_REPORT_CNF)
            {
                // Added by Qiyan
                api_QualReportCnf_t *p = (api_QualReportCnf_t *)nEvParam;
                CSW_TRACE(32, TSTXT("get API_QUAL_REPORT_CNF from stack\n"));

#ifdef CFW_MULTI_SIM
				CFW_NwQualReportConf(p, proc, nSimID); 
#else
				CFW_NwQualReportConf(p, proc); 
#endif
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("receive API_QUAL_REPORT_CNF at stop state\n"));
            }
            else
            {
            	   CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc warring unknow eventid \n"));  

                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }
            
        }
        else
        {
            if (nEventId == API_STOP_CNF)
            {   // confirm the stop of the stack

                CSW_TRACE(CFW_NW_TS_ID, TSTXT("get API_STOP_CNF from stack\n"));
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);   
                CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
                s_proc->sStatusInfo.nStatus = CFW_NW_STATUS_NOTREGISTERED_NOTSEARCHING;
		      s_proc->nNetModeEx = CFW_NW_AUTOMATIC_MODE;
	          s_proc->bStaticAO = TRUE;
	          s_proc->bReselNW = FALSE;
	          s_proc->bDetached = FALSE;
	          s_proc->bRoaming = TRUE;
	          s_proc->bNetAvailable = FALSE;
	          s_proc->nAttach = 0xFF;	
	          s_proc->nCsStatusIndCount = 0;
	          s_proc->bTimerneed = FALSE;
	          s_proc->bTimeDeleted = FALSE;
	          s_proc->sGprsStatusInfo.nCause = 0;
	          s_proc->nGprsAttState= CFW_GPRS_DETACHED;//0xFF;
                 #ifdef CFW_TCPIP_SUPPORT
          CFW_SmarchAttStatus();
	   #endif

                //lai, cellid =0
                SUL_ZeroMemory8(s_proc->sStatusInfo.nAreaCode, 3);
                SUL_ZeroMemory8(s_proc->sStatusInfo.nCellId, 2);
                SUL_ZeroMemory8(s_proc->nOperatorId, 6);
                s_proc->Rssi = 99;
                s_proc->Ber  = 99;
				
		CFW_SetAoState(s_hAo, CFW_SM_NW_STATE_STOP);
				
		CFW_SetAoState(hAo, CFW_SM_NW_STATE_STOP);
                // any others???
                if (STOP_CAUSE_FB == s_proc->nStpCause)
                {
                     CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc  nStpCause is Frequency Band\n"));  
					 s_proc->nStpCause = STOP_CAUSE_NOMAL;
#ifdef CFW_MULTI_SIM
                    CFW_NwStartStack(s_proc->nBand, nSimID);
#else
                    CFW_NwStartStack(s_proc->nBand);
#endif
                }
                else if(STOP_CAUSE_FM == s_proc->nStpCause)
                {
                	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc  nStpCause is Flight Mode\n"));  
					s_proc->nStpCause = STOP_CAUSE_NOMAL;
#ifdef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_NW_DEREGISTER_RSP, 0, 0, 
                                        nUTI|(nSimID<<24), 0 );
                    CFW_PostNotifyEvent(EV_CFW_NW_DEREGISTER_RSP, 0, 0, 
                    GENERATE_SHELL_UTI()|(nSimID<<24), 0 );
#else
                    CFW_PostNotifyEvent(EV_CFW_NW_DEREGISTER_RSP, 0, 0, 
                                        nUTI, 0 );
                    // Added by Qi
                    CFW_PostNotifyEvent(EV_CFW_NW_DEREGISTER_RSP, 0, 0, 
                    GENERATE_SHELL_UTI(), 0 );
#endif
                }
				else
				{
					  CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc nStpCause is ERROR\n"));  
				}
                
                
            }
            else if (nEventId == API_IMEI_IND)
            {   // state not change
                api_ImeiInd_t *p = (api_ImeiInd_t *)nEvParam;
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("get API_IMEI_IND from stack\n"));

#ifdef CFW_MULTI_SIM
				CFW_NwImeiConf(p, proc, nUTI, nSimID);
#else
				CFW_NwImeiConf(p, proc, nUTI);
#endif
                CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
                // un register ao
            }              
            else // error 
            {
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("not support event = 0x%x\n"), nEventId);
                CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
                
            }
        }   
    }
    else if (nState == CFW_SM_NW_STATE_SEARCH) // Search
    {
        // static AO
        if (proc->bStaticAO)
        {
            if (nEventId == API_NW_CSREGSTATUS_IND)
            {                
                api_NwCsRegStatusInd_t *p = (api_NwCsRegStatusInd_t *)nEvParam;
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("get API_NW_CSREGSTATUS_IND from stack\n"));

#ifdef CFW_MULTI_SIM
				CFW_NwCsStatusConf(p, proc, nSimID);
#else
				CFW_NwCsStatusConf(p, proc);
#endif
                if (p->Status == API_NW_NO_SVCE)
                {
                    CFW_SetAoState(hAo, CFW_SM_NW_STATE_STOP); 
                }
                else
                {
                    CFW_SetAoState(hAo, CFW_SM_NW_STATE_IDLE);                
                }
            }
            else if (nEventId == API_NW_PSREGSTATUS_IND)
            {   // used for furture
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("get API_NW_PSREGSTATUS_IND from stack\n"));
            }
            else if (nEventId == API_QUAL_REPORT_CNF)
            {   // state not change
                api_QualReportCnf_t *p = (api_QualReportCnf_t *)nEvParam;
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("get API_QUAL_REPORT_CNF from stack\n"));

#ifdef CFW_MULTI_SIM
				CFW_NwQualReportConf(p, proc, nSimID); 
#else
				CFW_NwQualReportConf(p, proc); 
#endif
            }  
            else
            {
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("not support event = 0x%x\n"), nEventId);
                CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
            }
        }
        else
        {
            if (nEventId == API_IMEI_IND)
            {   // state not change
                api_ImeiInd_t *p = (api_ImeiInd_t *)nEvParam;
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("get API_IMEI_IND from stack\n"));

#ifdef CFW_MULTI_SIM
				CFW_NwImeiConf(p, proc, nUTI, nSimID);
#else			
				CFW_NwImeiConf(p, proc, nUTI);
#endif
                CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
                // un register ao
            }                  
            else // error
            {
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("not support event = 0x%x\n"), nEventId);
                CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
            }
        }
        
    }
    else if (nState == CFW_SM_NW_STATE_IDLE) // Idle
    {
        if (pEvent->nParam1 == 0xff)    // req to stack
        {
            if (proc->nMsgId == API_NW_LISTABORT_REQ)
            {
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("send API_NW_LISTABORT_REQ to stack\n")); 

#ifdef CFW_MULTI_SIM
                CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody, nSimID);
#else
                CFW_SendSclMessage( proc->nMsgId, proc->pMsgBody);
#endif
        
                CFW_SetAoState(s_hAo, CFW_SM_NW_STATE_IDLE);
                CFW_SetAoState(hAo, CFW_SM_NW_STATE_IDLE);                    
            }
        }
        // static AO
        if (proc->bStaticAO)
        {
            if (nEventId == API_NW_CSREGSTATUS_IND)
            {   // store reg status
                // if the URC is on, response to AT, and store the message
                // else, store the message , not response to AT
                // 
                api_NwCsRegStatusInd_t *p = (api_NwCsRegStatusInd_t *)nEvParam;
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("get API_NW_CSREGSTATUS_IND from stack\n"));

#ifdef CFW_MULTI_SIM
				CFW_NwCsStatusConf(p, proc, nSimID);
#else
				CFW_NwCsStatusConf(p, proc);
#endif
                if (p->Status == API_NW_NO_SVCE)
                {
                    CFW_SetAoState(hAo, CFW_SM_NW_STATE_STOP); 
                }
                else
                {
                    CFW_SetAoState(hAo, CFW_SM_NW_STATE_IDLE);                
                }
            }
            else if (nEventId == API_NW_PSREGSTATUS_IND)
            {   // used for future
                api_NwPsRegStatusInd_t *psInMsg = (api_NwPsRegStatusInd_t *)nEvParam;

                CSW_TRACE(CFW_NW_TS_ID, TSTXT("get API_NW_PSREGSTATUS_IND from stack psInMsg->Status: 0x%x ,psInMsg->Cause: 0x%x \n"),psInMsg->Status,psInMsg->Cause);

                //Add by lixp for UTI zero bug at 20090723.

				nUTI = s_proc->nAttachUTI;
				s_proc->sGprsStatusInfo.nCause = psInMsg->Cause;
#ifdef CFW_MULTI_SIM
				CFW_NwPsStatusConf(psInMsg, proc, nUTI,nSimID);
#else
				CFW_NwPsStatusConf(psInMsg, proc,nUTI);
#endif
            }
            else if (nEventId == API_QUAL_REPORT_CNF)
            {   // state not change
                // used by system, and store the message , and the message will be used by AT.
                api_QualReportCnf_t *p = (api_QualReportCnf_t *)nEvParam;
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("get API_QUAL_REPORT_CNF from stack\n"));

#ifdef CFW_MULTI_SIM
				CFW_NwQualReportConf(p, proc, nSimID);
#else
				CFW_NwQualReportConf(p, proc);
#endif
            }
            else if (nEventId == API_NWKINFO_IND)
            {
                CFW_NW_NETWORK_INFO *pNetWorkInfo = NULL;
                api_NwkInfoInd_t *psInMsg = (api_NwkInfoInd_t *)nEvParam;
                pNetWorkInfo = (CFW_NW_NETWORK_INFO *)CFW_MALLOC(SIZEOF(CFW_NW_NETWORK_INFO));
                SUL_MemCopy8(pNetWorkInfo, psInMsg, SIZEOF(api_NwkInfoInd_t));
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("API_NWKINFO_IND  data:....\n") );
                    

                // send the network info to up layer
                //

#ifdef CFW_MULTI_SIM
                CFW_PostNotifyEvent(EV_CFW_NW_NETWORKINFO_IND, (UINT32)pNetWorkInfo, 0,
                                    CFW_NW_IND_UTI|(nSimID<<24), 0x00 );				
#else
                CFW_PostNotifyEvent(EV_CFW_NW_NETWORKINFO_IND, (UINT32)pNetWorkInfo, 0, 
                                    CFW_NW_IND_UTI, 0 );
#endif				
            }            
            else
            {
            	  CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc warring unknow eventid  11111\n"));
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }
        }
        else
        {
            if (nEventId == API_NW_LIST_IND)
            {   // state not change
                // notify AT: EV_CFW_NW_GET_AVAIL_OPERATOR_RSP 
                // with parameter: CFW_NW_OPERATOR_INFO list, and the number of operators
                api_NwListInd_t *p = (api_NwListInd_t *)nEvParam;
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("get API_NW_LIST_IND from stack\n"));
                SUL_MemCopy8(proc->nOperatorId, s_proc->nOperatorId, 6);

#ifdef CFW_MULTI_SIM
				CFW_NwListConf(p, proc, nUTI, nSimID);
#else
				CFW_NwListConf(p, proc, nUTI);
#endif
                CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
            }
            else if (nEventId == API_NW_LISTABORT_IND)
            {
            		api_NwListAbortInd_t *p = (api_NwListAbortInd_t *)nEvParam;
                     CSW_TRACE(CFW_NW_TS_ID, TSTXT("get API_NW_LISTABORT_IND from stack cause: 0x%x\n"),p->Cause); 
			if(API_NORMAL_LIST_ABORT == p->Cause)
			{
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_NW_ABORT_LIST_OPERATORS_RSP, 0, 0 ,
				                    nUTI|(nSimID<<24), 0x00 );				
#else
				CFW_PostNotifyEvent(EV_CFW_NW_ABORT_LIST_OPERATORS_RSP, 0, 0 ,
				                    nUTI, 0 );
#endif				
			}
			else if(API_ABORM_LIST_ABORT == p->Cause)//we receive this event when we have send list REQ and have a call to come in.we will don't receive list IND,but we will receive abort IND and cause is 1. 
			{
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_NW_GET_AVAIL_OPERATOR_RSP, (UINT32)NULL, 0, 
				                    nUTI|(nSimID<<24), 0xF0 );				
#else
				CFW_PostNotifyEvent(EV_CFW_NW_GET_AVAIL_OPERATOR_RSP, (UINT32)NULL, 0, 
				                    nUTI, 0xF0 );
#endif		
			}
			else
			{
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_NW_ABORT_LIST_OPERATORS_RSP, 0, 0 ,
				                    nUTI|(nSimID<<24), 0xF0 );				
#else
				CFW_PostNotifyEvent(EV_CFW_NW_ABORT_LIST_OPERATORS_RSP, 0, 0 ,
				                    nUTI, 0xF0 );
#endif
			}

                // response to the list abort
                CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
                
            }
            else if (nEventId == API_IMEI_IND)
            {   // state not change
                // notify AT: EV_CFW_NW_GET_IMEI_ID_RSP
                // with parameter: Pointer to the buffer to hold the IMEI number in ASCII string format
                // and the buffer length
                api_ImeiInd_t *p = (api_ImeiInd_t *)nEvParam;
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("get API_IMEI_IND from stack\n"));

#ifdef CFW_MULTI_SIM
				CFW_NwImeiConf(p, proc, nUTI, nSimID);
#else
				CFW_NwImeiConf(p, proc, nUTI);
#endif
                CFW_UnRegisterAO(CFW_NW_SRV_ID, hAo);
            }            
            else // error
            {
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAoProc warring unknow eventid  22222\n"));

                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
            }
        }
               
    }
    else // state error
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("In CFW_NwAoProc state error\n"));
    }

    CSW_PROFILE_FUNCTION_EXIT(CFW_NwAoProc);
    return ret;
}

//////////////////////////////////////////////////////////////////////////
//
// CM Interface by MMI
//


//
// get available operators, send API_NW_LIST to stack
// and the response : API_NW_LIST_IND
//
UINT32 CFW_NwGetAvailableOperators (
									UINT16 nUTI
#ifdef CFW_MULTI_SIM
									,CFW_SIM_ID nSimID
#endif
									)
{
    // should not be called in stop state
    // state not change
    NW_SM_INFO *proc = NULL;
    //UINT32 nState    = 0;       // sm state
    HAO hAo = 0;               // Handle
    HAO s_hAo = 0;              // static handle
    NW_SM_INFO* s_proc = NULL;    // static sm info
        CSW_PROFILE_FUNCTION_ENTER(CFW_NwGetAvailableOperators);
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetAvailableOperators start nUTI = 0x%x\n"), nUTI);

#ifdef CFW_MULTI_SIM
	UINT8 ret;
	ret = CFW_CheckSimId(nSimID);	
	if( ERR_SUCCESS != ret)
	{		
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !! get sim id error!\n"));
		CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetAvailableOperators);
		return ret;
	}
#endif

	if (CFW_IsFreeUTI(nUTI, CFW_NW_SRV_ID) != ERR_SUCCESS)
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !! UTI BUSY!\n"));
		CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetAvailableOperators);
		return ERR_CFW_UTI_IS_BUSY;
	}

	
    proc = (NW_SM_INFO*)CFW_MALLOC(SIZEOF(NW_SM_INFO));
    if (proc == NULL)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("malloc exception\n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetAvailableOperators);
        return ERR_CME_MEMORY_FULL;  // should be updated by macro
    }

    // Clear the proc structure, ensure that there is not existing un-intialized member.
    SUL_ZeroMemory8(proc, SIZEOF(NW_SM_INFO));
	
    // save the selected mode to Ao (static AO)
#ifdef CFW_MULTI_SIM
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
    CFW_SetServiceId(CFW_NW_SRV_ID);
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif

    if (s_hAo == 0)
    {
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR ! s_hAo == 0 \n"));
		CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetAvailableOperators);
		return -1; // AOM error
    }

    s_proc = CFW_GetAoUserData(s_hAo);
	
    if(s_proc->bTimerneed)
    {
         CSW_TRACE(CFW_NW_TS_ID, TSTXT("kill the network timer\n"));   

        s_proc->bTimerneed = FALSE;
        proc->bTimeDeleted = TRUE;
        // kill the timer
#ifndef CFW_MULTI_SIM
        COS_KillTimerEX(0xFFFFFFFF, PRV_CFW_NW_TIMER_ID);
#else
	 UINT32 nTimerId=0;
	if(CFW_SIM_0 == nSimID)
		nTimerId = PRV_CFW_NW_SIM0_TIMER_ID;
	else if(CFW_SIM_1 == nSimID)
		nTimerId = PRV_CFW_NW_SIM1_TIMER_ID;
	else if(CFW_SIM_1+1 == nSimID)
		nTimerId = PRV_CFW_NW_SIM2_TIMER_ID;
	else if(CFW_SIM_1+2 == nSimID)
		nTimerId = PRV_CFW_NW_SIM3_TIMER_ID;
        COS_KillTimerEX(0xFFFFFFFF, nTimerId);
#endif
    }
    
    proc->nMsgId = API_NW_LIST_REQ;
    proc->pMsgBody = NULL;
    proc->bStaticAO = FALSE;
    
#ifdef CFW_MULTI_SIM
    hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc, nSimID);
#else
    hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc);
#endif

    CFW_SetUTI(hAo, nUTI, 0);
    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE);

    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetAvailableOperators end\n"));
    CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetAvailableOperators);
    return ERR_SUCCESS;
}

UINT32 CFW_NwGetCurrentOperator (
								 UINT8 OperatorId[6],
								 UINT8* pMode
#ifdef CFW_MULTI_SIM
								 , CFW_SIM_ID nSimID
#endif
								 )
{
    // state not change
    NW_SM_INFO * s_proc = NULL;
    HAO s_hAo = 0;               // Handle
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetCurrentOperator start\n"));

    CSW_PROFILE_FUNCTION_ENTER(CFW_NwGetCurrentOperator);

#ifdef CFW_MULTI_SIM
    UINT8 ret;
    ret = CFW_CheckSimId(nSimID);	
	if( ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetCurrentOperator SIM ERROR!\n"));

		CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetCurrentOperator);
		return ret;
	}
#endif

    if ((pMode == NULL) || (OperatorId == NULL))
    {
    	 CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetCurrentOperator input ERROR!\n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetCurrentOperator);
        return ERR_CFW_INVALID_PARAMETER;
    }

#ifdef CFW_MULTI_SIM
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
    CFW_SetServiceId(CFW_NW_SRV_ID);
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif

    if (s_hAo == 0)
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetCurrentOperator);
        return -1; // AOM error
    }

    s_proc = CFW_GetAoUserData(s_hAo); // get info from AO
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetCurrentOperator PLMN = %x%x%x%x%x%x mode = 0x%x\n"), 
        s_proc->nOperatorId[0],
        s_proc->nOperatorId[1],
        s_proc->nOperatorId[2],
        s_proc->nOperatorId[3],
        s_proc->nOperatorId[4],
        s_proc->nOperatorId[5],
        s_proc->nNetMode == API_NW_AUTOMATIC_MODE ? API_NW_AUTOMATIC_MODE : API_NW_MANUAL_MODE);

	SUL_MemCopy8(OperatorId, s_proc->nOperatorId, 6);
	*pMode = s_proc->nNetMode == API_NW_AUTOMATIC_MODE ? API_NW_AUTOMATIC_MODE : API_NW_MANUAL_MODE;    
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetCurrentOperator end\n"));

    CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetCurrentOperator);
    return ERR_SUCCESS;
}

//
//  get Imei , send API_IMEI_REQ to stack with no parameter
// response API_IMEI_CNF with parameter : IMEI
UINT32 CFW_NwGetImei (                 
						   UINT16 nUTI
#ifdef CFW_MULTI_SIM
						   ,CFW_SIM_ID nSimID
#endif
		 				   )
{
    // state not change
    NW_SM_INFO *proc = NULL;
    HAO hAo = 0;

    CSW_PROFILE_FUNCTION_ENTER(CFW_NwGetImei);
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetImei nUTI = 0x%x\n"), nUTI);

#ifdef CFW_MULTI_SIM
    UINT8 ret;
    ret = CFW_CheckSimId(nSimID);	
    if( ERR_SUCCESS != ret)
    {
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetImei SIM ERROR!\n"));

	    CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetImei);
		return ret;
	}
#endif

    if (CFW_IsFreeUTI(nUTI, CFW_NW_SRV_ID) != ERR_SUCCESS)
    {
    	 CSW_TRACE(CFW_NW_TS_ID, TSTXT("UTI busy\n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetImei);
        return ERR_CFW_UTI_IS_BUSY;
    }

    proc = (NW_SM_INFO*)CFW_MALLOC(SIZEOF(NW_SM_INFO));
    if (proc == NULL)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("malloc exception\n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetImei);
        return ERR_CME_MEMORY_FULL;  // should be updated by macro
    }
    // Clear the proc structure, ensure that there is not existing un-intialized member.
    SUL_ZeroMemory32(proc, SIZEOF(NW_SM_INFO));
    
    proc->nMsgId = API_IMEI_REQ;
    proc->pMsgBody = NULL;
    proc->bStaticAO = FALSE;

#ifdef CFW_MULTI_SIM
    hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc, nSimID);
#else    
    hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc);
#endif

    CFW_SetUTI(hAo, nUTI, 0);
    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom

	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetImei end\n"));
	//CFW_SendSclMessage( API_IMEI_REQ, NULL);
	CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetImei);
    return ERR_SUCCESS;
}

//
// get signal quality
// notify AT: EV_CFW_NW_GET_SIGNAL_QUALITY_RSP with parameter: rssi , ber
UINT32 SRVAPI CFW_NwGetSignalQuality(
		UINT8* pSignalLevel,
		UINT8* pBitError
#ifdef CFW_MULTI_SIM
		, CFW_SIM_ID nSimID
#endif
		)
{
    // state not change
    NW_SM_INFO* s_proc = NULL;    // sm info
    HAO s_hAo = 0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_NwGetSignalQuality);

	CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR! CFW_CheckSimId start\n"));
#ifdef CFW_MULTI_SIM
    UINT8 ret;
    ret = CFW_CheckSimId(nSimID);	
	if( ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !CFW_CheckSimId SIM ERROR!\n"));
		CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetSignalQuality);
		return ret;
	}
#endif
	
    if ((pSignalLevel == NULL) || (pBitError == NULL))
    {
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetSignalQuality);
        return ERR_CFW_INVALID_PARAMETER;
    }

#ifdef CFW_MULTI_SIM
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
    CFW_SetServiceId(CFW_NW_SRV_ID);
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif

    if (s_hAo == 0)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR! s_hAo == 0 \n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetSignalQuality);
        return -1; // AOM error
    }

    s_proc = CFW_GetAoUserData(s_hAo);
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetSignalQuality end rssi = %d, ber = %d \n"), s_proc->Rssi, s_proc->Ber);
    *pSignalLevel = s_proc->Rssi;
    *pBitError = s_proc->Ber;
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_NwGetSignalQuality);
    return ERR_SUCCESS;
}

//
// get the reg status from the stored message
//
UINT32 SRVAPI CFW_NwGetStatus (
		CFW_NW_STATUS_INFO* pStatusInfo
#ifdef CFW_MULTI_SIM
		, CFW_SIM_ID nSimID
#endif
		)
{
    NW_SM_INFO* s_proc = NULL;    // sm info
    HAO s_hAo = 0;

	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetStatus start\n"));

#ifdef CFW_MULTI_SIM
    UINT8 ret;
    ret = CFW_CheckSimId(nSimID);	
	if( ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !CFW_NwGetStatus SIM ERROR!\n"));
		return ret;
	}
#endif
	
    if (pStatusInfo == NULL)
    {
    	 CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !CFW_NwGetStatus SIM input error!\n"));
        return ERR_CFW_INVALID_PARAMETER;
    }

#ifdef CFW_MULTI_SIM
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
    CFW_SetServiceId(CFW_NW_SRV_ID);
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif

    if (s_hAo == 0)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("s_hAo == 0 \n"));
        return -1; // AOM error
    }

    s_proc = CFW_GetAoUserData(s_hAo);    

    SUL_MemCopy8(pStatusInfo, &s_proc->sStatusInfo, SIZEOF(CFW_NW_STATUS_INFO));

    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetStatus status = 0x%x\n"), pStatusInfo->nStatus);
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwGetStatus end LAC = 0x%x, 0x%x, cell id = 0x%x, 0x%x\n"),
        pStatusInfo->nAreaCode[0], pStatusInfo->nAreaCode[1], 
        pStatusInfo->nCellId[0], pStatusInfo->nCellId[1] );

    return ERR_SUCCESS;

}

//
// set the select mode and attempt to register the selected operator.
// if the mode=0, the operatorId should be ignored.
// if the mode=1, the operatorId should be valid.
// if the mode=4, the operatorId should be valid, and if the attempt failed, as if mode=0 will be entered.
UINT32 CFW_NwSetRegistration (
							  UINT8 nOperatorId[6],    
							  UINT8 nMode,      
							  UINT16 nUTI
#ifdef CFW_MULTI_SIM
							  ,CFW_SIM_ID nSimID
#endif							  
							  )

{
    UINT32 s_nState    = 0;       // static sm state
    HAO hAo = 0;               // Handle
    HAO s_hAo = 0;              // static handle
    NW_SM_INFO* proc = NULL;    // sm info
    NW_SM_INFO* s_proc = NULL;    // static sm info

    CSW_PROFILE_FUNCTION_ENTER(CFW_NwSetRegistration);
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwSetRegistration start\n"));

#ifdef CFW_MULTI_SIM
    UINT8 ret;
    ret = CFW_CheckSimId(nSimID);
    if( ERR_SUCCESS != ret)
    {
    		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !CFW_NwSetRegistration SIM ERROR!\n"));
	    CSW_PROFILE_FUNCTION_EXIT(CFW_NwSetRegistration);
		return ret;
	}
#endif

    // validate parameters
    if ((nMode != CFW_NW_AUTOMATIC_MODE) 
        && (nMode != CFW_NW_MANUAL_MODE) 
        && (nMode != CFW_NW_MANUAL_AUTOMATIC_MODE))
    {        
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !nMode is not correct \n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwSetRegistration);
        return ERR_CFW_INVALID_PARAMETER;        
    }
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwSetRegistration mode = %d nUTI = 0x%x\n"), nMode, nUTI);
    if (CFW_IsFreeUTI(nUTI, CFW_NW_SRV_ID) != ERR_SUCCESS)
    {
    	  CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !UTI_IS_BUSY \n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwSetRegistration);
        return ERR_CFW_UTI_IS_BUSY;
    }

    proc = (NW_SM_INFO*)CFW_MALLOC(SIZEOF(NW_SM_INFO));
    if (proc == NULL)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !malloc exception\n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwSetRegistration);
        return ERR_CME_MEMORY_FULL;  // should be updated by macro
    }

    // Clear the proc structure, ensure that there is not existing un-intialized member.
    SUL_ZeroMemory32(proc, SIZEOF(NW_SM_INFO));
    
    // save the selected mode to Ao (static AO)

#ifdef CFW_MULTI_SIM
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else	
    CFW_SetServiceId(CFW_NW_SRV_ID);
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif
    if (s_hAo == 0)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !s_hAo == 0 \n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwSetRegistration);
        return -1; // AOM error
    }

    s_proc = CFW_GetAoUserData(s_hAo);
    s_nState = CFW_GetAoState(s_hAo);

    s_proc->nNetMode = nMode;
    s_proc->bReselNW = TRUE;
    //s_proc->nCOPSUTI = nUTI;
    s_proc->nReselUTI = nUTI;
    s_proc->nCsStatusIndCount = 0;

    //if time need, delete the timer, else do nothing
    if(s_proc->bTimerneed)
    {
         CSW_TRACE(CFW_NW_TS_ID, TSTXT("kill the network timer\n"));   
        s_proc->bTimerneed = FALSE;
        // kill the timer
#ifndef CFW_MULTI_SIM
        COS_KillTimerEX(0xFFFFFFFF, PRV_CFW_NW_TIMER_ID);
#else
	 UINT32 nTimerId=0;
	if(CFW_SIM_0 == nSimID)
		nTimerId = PRV_CFW_NW_SIM0_TIMER_ID;
	else if(CFW_SIM_1 == nSimID)
		nTimerId = PRV_CFW_NW_SIM1_TIMER_ID;
	else if(CFW_SIM_1+1 == nSimID)
		nTimerId = PRV_CFW_NW_SIM2_TIMER_ID;
	else if(CFW_SIM_1+2 == nSimID)
		nTimerId = PRV_CFW_NW_SIM3_TIMER_ID;
        COS_KillTimerEX(0xFFFFFFFF, nTimerId);
#endif
    }
    if (nMode != CFW_NW_AUTOMATIC_MODE)
    {
        SUL_MemCopy8(s_proc->nAttOperatorId, nOperatorId, 6);
    }
    
    if (s_nState == CFW_SM_NW_STATE_STOP)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("in stop state\n"));  

	#ifdef CFW_MULTI_SIM
        CFW_NwStartStack(s_proc->nBand, nSimID);
	#else
        CFW_NwStartStack(s_proc->nBand);
	#endif

        CSW_PROFILE_FUNCTION_EXIT(CFW_NwSetRegistration);
        return ERR_SUCCESS;
    }
    
    if (nMode == CFW_NW_AUTOMATIC_MODE)       // automatic mode
    {
           CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwSetRegistration automatic mode\n"));   

        // should use AOM
        proc->nMsgId = API_NW_RESEL_REQ;
        proc->pMsgBody = NULL;        
        proc->bStaticAO = FALSE;

#ifdef CFW_MULTI_SIM
        hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc, nSimID);
#else        
        hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc);
#endif

        CFW_SetUTI(hAo, nUTI, 0);
              
        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom

        //CFW_SendSclMessage(API_NW_RESEL_REQ, NULL);       // resel the network
    }
    else  // manual mode or automatic/manual mode
    {    
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwSetRegistration manual mode PLMN = %x%x%x%x%x%x \n"),
            nOperatorId[0],
            nOperatorId[1],
            nOperatorId[2],
            nOperatorId[3],
            nOperatorId[4],
            nOperatorId[5]);
        // should use AOM
        proc->nMsgId = API_NW_SEL_REQ;
        proc->pMsgBody = NULL;        
        proc->bStaticAO = FALSE;     

#ifdef CFW_MULTI_SIM
        hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc, nSimID);
#else        
        hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc);
#endif

        CFW_SetUTI(hAo, nUTI, 0);
        
        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom
        
        //CFW_SendSclMessage(API_NW_SEL_REQ, pSelPlmn); // try to attach the selected plmn
    }
    	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwSetRegistration end\n"));
    // store the mode in flash
    // Todo use Mode;
   CSW_PROFILE_FUNCTION_EXIT(CFW_NwSetRegistration);
    return ERR_SUCCESS;
}

//
//  used by AT, cops=2, 
// send API_STOP_REQ to stack with parameter api_StopReq: False
// response API_STOP_CNF
UINT32 CFW_NwDeRegister (
						 UINT16 nUTI
#ifdef CFW_MULTI_SIM
						 ,CFW_SIM_ID nSimID
#endif
						 )
{
    UINT32 ret = 0;
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwDeRegister start\n"));
#ifdef CFW_MULTI_SIM
	if(nSimID >= CFW_SIM_COUNT)
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("DeRegister INVALID PARMETER!\n"));
		return ERR_CFW_INVALID_PARAMETER;
	}
#endif
#if 0
#ifdef CFW_MULTI_SIM
    ret = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !CFW_NwDeRegister SIM ERROR!\n"));
		return ret;
	}
#endif
#endif
    if (CFW_IsFreeUTI(nUTI, CFW_NW_SRV_ID) != ERR_SUCCESS)
    {		
    	 CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwDeRegister UTI_IS_BUSY\n"));
        return ERR_CFW_UTI_IS_BUSY;
    }

#ifdef CFW_MULTI_SIM

    ret = CFW_NwStop(FALSE, nUTI,nSimID);
#else
    ret = CFW_NwStop(FALSE, nUTI);
#endif
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwDeRegister end\n"));
    return ret;
}

//used by system

//
// start the stack
//
PRIVATE UINT32 CFW_NwStartStack(
		UINT8 nBand
#ifdef CFW_MULTI_SIM
		, CFW_SIM_ID nSimID
#endif
		)
{
    // state change to search
    HAO hAo = 0;               // Handle
    NW_SM_INFO* proc = NULL;    // sm info

    CSW_PROFILE_FUNCTION_ENTER(CFW_NwStartStack);
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwStartStack start \n") );

    proc = (NW_SM_INFO*)CFW_MALLOC(SIZEOF(NW_SM_INFO));
    if (proc == NULL)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !malloc exception\n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwStartStack);
        return ERR_CME_MEMORY_FULL;  // should be updated by macro
    }

    // Clear the proc structure, ensure that there is not existing un-intialized member.
    SUL_ZeroMemory32(proc, SIZEOF(NW_SM_INFO));
    proc->nBand = nBand;

    // save the nBand in sm information;;

    proc->nMsgId = API_START_REQ;
    proc->pMsgBody = NULL;
    proc->bStaticAO = FALSE;

#ifdef CFW_MULTI_SIM
    hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc, nSimID);
#else
    hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc);
#endif

    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom
    
    //    CFW_SendSclMessage(API_START_REQ, nTempBand);
    // need to send network mode to stack
    // api_selmode_req ---> api
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwStartStack end \n") );
    //CFW_SetAoNextState(hAo, CFW_SM_NW_STATE_SEARCH);   //set the state to search
    CSW_PROFILE_FUNCTION_EXIT(CFW_NwStartStack);
    return ERR_SUCCESS;
}

//
// stop the stack , power Off
//
UINT32 CFW_NwStopStack(
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID
#endif
	)
{

    UINT32 ret = 0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_NwStopStack);
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwStopStack start \n") );
#ifdef CFW_MULTI_SIM

    ret = CFW_CheckSimId(nSimID);
    if( ERR_SUCCESS != ret)
    {
    	   CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !CFW_NwStopStack SIM ERROR!\n"));
	    CSW_PROFILE_FUNCTION_EXIT(CFW_NwStopStack);
		return ret;
	}

    ret = CFW_NwStop(FALSE, 0, nSimID);
#else
    ret = CFW_NwStop(FALSE, 0);
#endif
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwStopStack end \n") );
    CSW_PROFILE_FUNCTION_EXIT(CFW_NwStopStack);
    return ret;
}

// used by stop and detach
PRIVATE UINT32 CFW_NwStop(
		BOOL bPowerOff, 
		UINT16 nUTI
#ifdef CFW_MULTI_SIM
		, CFW_SIM_ID nSimID
#endif
		)
{
    // state  change to stop
    HAO hAo = 0;               // Handle
    api_StopReq_t *pOutMsg = NULL;

    NW_SM_INFO *proc = NULL;
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwStop nUTI = 0x%x\n"), nUTI);

    pOutMsg = (api_StopReq_t *)CFW_MemAllocMessageStructure(SIZEOF(api_StopReq_t));
    pOutMsg->PowerOff = bPowerOff;   //  power off or Not

    proc = (NW_SM_INFO*)CFW_MALLOC(SIZEOF(NW_SM_INFO));
    if (proc == NULL)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR malloc exception\n"));
        return ERR_CME_MEMORY_FULL;  // should be updated by macro
    }

    // Clear the proc structure, ensure that there is not existing un-intialized member.
    SUL_ZeroMemory32(proc, SIZEOF(NW_SM_INFO));
    
    proc->nMsgId = API_STOP_REQ;
    proc->pMsgBody = pOutMsg;    
    proc->bStaticAO = FALSE;

#ifdef CFW_MULTI_SIM    
    hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc, nSimID);
#else
    hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc);
#endif

    CFW_SetUTI(hAo, nUTI, 0);
    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom
    // use AOM
    //    CFW_SendSclMessage( API_STOP_REQ, pOutMsg);
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwStop end \n") );

    //chage the state to stop
    return ERR_SUCCESS;
}

PRIVATE UINT8  getsum(UINT8 input)
{
    UINT8 temp = input % 10;
    if (input/10)   
    {
        temp += input / 10; 
    }
    return temp;
}

PRIVATE UINT8 getCD(UINT8 *pData)
{
    UINT8 i = 0;
    UINT8 j = 0;
    UINT8 sum = 0;  
    UINT8 IMEI[14];
    SUL_MemCopy8(IMEI, pData, 14);
    for (i = 0; i < 14/2; i++) 
    {
        IMEI[13 - 2 * i] = IMEI[13- 2 * i] * 2;
    }
    
    for (j = 0; j < 14; j++)
    {
        sum += getsum(IMEI[j]);
    }   
//    SXS_TRACE(CFW_NW_TS_ID_SXS, "sum = %d, CD = 0xd\n", sum, (10 - sum%10));
    
    return (10 - sum%10);  
    
}
//
PRIVATE UINT32 CFW_NwImeiConf(
			api_ImeiInd_t * pImei, 
			NW_SM_INFO * pSmInfo,
			UINT32 nUTI
#ifdef CFW_MULTI_SIM
			, CFW_SIM_ID nSimID
#endif
			)
{
    UINT8 *pOutMsg = (UINT8 *)CFW_MALLOC(15);
    SUL_MemCopy8(pOutMsg, pImei->Imei, API_IMEI_SIZE);
    pOutMsg[14] = getCD(pOutMsg);
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwImeiConf \n") );
     //CFW_Dump(CFW_NW_TS_ID, pOutMsg, 15);

#ifdef CFW_MULTI_SIM
    CFW_PostNotifyEvent(EV_CFW_NW_GET_IMEI_RSP, (UINT32)pOutMsg, 15,
                        nUTI|(nSimID<<24), 0 );				
#else
    CFW_PostNotifyEvent(EV_CFW_NW_GET_IMEI_RSP, (UINT32)pOutMsg, 15, 
                        nUTI, 0 );
#endif				
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwImeiConf end\n") );
    return ERR_SUCCESS;
}

//
// transform the rssi and rxqual from stack to AT+CGSN standard
//
// <rssi>:
// 0	-113 dBm or less
// 1	-111 dBm
// 2...30	-109... -53 dBm
// 31	-51 dBm or greater
// 99	not known or not detectable
// <ber> (in percent):
// 0...7	as RXQUAL values in the table in GSM 05.08 [20] subclause 8.2.4
// 99	not known or not detectable

//
PRIVATE UINT32 CFW_NwQualReportConf(
		api_QualReportCnf_t *pQualReport, 
		NW_SM_INFO * pSmInfo
#ifdef CFW_MULTI_SIM
		, CFW_SIM_ID nSimID
#endif
		)
{
    NW_SM_INFO * proc = pSmInfo;
    UINT8 nRssi = pQualReport->Rssi;
    CFW_IND_EVENT_INFO sIndEventInfo;
    UINT16 nInd = 0;
    
    UINT8 nRssiEx = 0;
    UINT8 nBerEx = 0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_NwQualReportConf);
    //BOOL bChanged = FALSE;
	    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwQualReportConf start \n") );

    nRssiEx = proc->Rssi;
    nBerEx = proc->Ber;
#ifdef CFW_MULTI_SIM
    CFW_CfgGetIndicatorEventInfo(&sIndEventInfo, nSimID);
    CFW_CfgGetIndicatorEvent(&nInd, nSimID);
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwQualReportConf, nSimID=%d\n"), nSimID);
#else
    CFW_CfgGetIndicatorEventInfo(&sIndEventInfo);
    CFW_CfgGetIndicatorEvent(&nInd);
#endif
   // nInd = CFW_CFG_IND_SQ_EVENT|CFW_CFG_IND_BIT_ERR_EVENT;  //just for test

    CSW_TRACE(CFW_NW_TS_ID, TSTXT("From stack : Rssi = %d RxQual = %d nInd = 0x%x\n"), pQualReport->Rssi, pQualReport->RxQual, nInd); 
    
    if (nRssi > 113)
    {
        proc->Rssi = 0;
    }
    else if ((nRssi <= 113) && (nRssi >= 51))
    {
        proc->Rssi = (UINT8)(31 - ((nRssi - 51)/2));
    }
    else if (nRssi < 51)
    {
        proc->Rssi = 31;
    }
    else
    {
        proc->Rssi = 99;
    }

    // need some transfer
    //proc->Rssi = pQualReport->Rssi;
    // if cc not working ber = 99, else = real value

    if (sIndEventInfo.call == 0) 
    {
        proc->Ber = 99;
    }
    else
    {
        proc->Ber  = pQualReport->RxQual;  
    }

    CSW_TRACE(CFW_NW_TS_ID, TSTXT("After transform Rssi = %d RxQual = %d\n"), proc->Rssi, proc->Ber); 
    if ((nInd & CFW_CFG_IND_SQ_EVENT) || (nInd & CFW_CFG_IND_BIT_ERR_EVENT))
    {
        if ((nInd & CFW_CFG_IND_SQ_EVENT) && (nInd & CFW_CFG_IND_BIT_ERR_EVENT))
    {
            if ((proc->Rssi != nRssiEx) || (proc->Ber != nBerEx))
            {
                
                UINT8 uSignal;
                if(!proc->Rssi)
                    uSignal = 0;
                else if(proc->Rssi == 1)
                    uSignal = 1;
                else if(proc->Rssi == 31)
                    uSignal = 5;
                else
                    uSignal = (UINT8)(2 + (proc->Rssi - 1) / 10);
                sIndEventInfo.sq = uSignal;
                
                sIndEventInfo.bit_err = proc->Ber;
                nRssiEx = proc->Rssi;
                nBerEx = proc->Ber;

                CSW_TRACE(CFW_NW_TS_ID, TSTXT("the signal quality change 11 and bit error change\n"));

#ifdef CFW_MULTI_SIM
			CFW_CfgSetIndicatorEventInfo(&sIndEventInfo, nSimID); // for ind
		            CFW_PostNotifyEvent(EV_CFW_NW_SIGNAL_QUALITY_IND, (UINT32)nRssiEx, (UINT32)nBerEx,
		                                CFW_NW_IND_UTI|(nSimID<<24), 0 );	
#else
			CFW_CfgSetIndicatorEventInfo(&sIndEventInfo); // for ind
				CFW_PostNotifyEvent(EV_CFW_NW_SIGNAL_QUALITY_IND, (UINT32)nRssiEx, (UINT32)nBerEx, 
                                    CFW_NW_IND_UTI, 0 );
#endif				

                CSW_TRACE(CFW_NW_TS_ID, TSTXT("rssi = %d ber = %d\n"), nRssiEx, nBerEx); 

            }
        }
        else if (nInd & CFW_CFG_IND_SQ_EVENT)
        {
            if (proc->Rssi != nRssiEx)
            {
                UINT8 uSignal;
                if(!proc->Rssi)
                    uSignal = 0;
                else if(proc->Rssi == 1)
                    uSignal = 1;
                else if(proc->Rssi == 31)
                    uSignal = 5;
                else
                    uSignal = (UINT8)(2 + (proc->Rssi - 1) / 10);
                sIndEventInfo.sq = uSignal;

                nRssiEx = proc->Rssi;
                nBerEx = 0;
                //CFW_CfgSetIndicatorEvent(CFW_CFG_IND_SQ_EVENT);
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("the signal quality change22\n")); 

#ifdef CFW_MULTI_SIM
                 CFW_CfgSetIndicatorEventInfo(&sIndEventInfo, nSimID);   // for ind
	                CFW_PostNotifyEvent(EV_CFW_NW_SIGNAL_QUALITY_IND, (UINT32)nRssiEx, (UINT32)nBerEx,
	                                    CFW_NW_IND_UTI|(nSimID<<24), 0 );
				
#else
			CFW_CfgSetIndicatorEventInfo(&sIndEventInfo); // for ind
                CFW_PostNotifyEvent(EV_CFW_NW_SIGNAL_QUALITY_IND, (UINT32)nRssiEx, (UINT32)nBerEx, 
                                    CFW_NW_IND_UTI, 0 );
#endif				

                CSW_TRACE(CFW_NW_TS_ID, TSTXT("rssi = %d ber = %d\n"), nRssiEx, nBerEx); 
                
            }
        }
        else if (nInd & CFW_CFG_IND_BIT_ERR_EVENT)
        {
            if (proc->Ber != nBerEx)
            {
                sIndEventInfo.bit_err = proc->Ber;

                nRssiEx = 0;
                nBerEx = proc->Ber;
                //CFW_CfgSetIndicatorEvent(CFW_CFG_IND_BIT_ERR_EVENT);
                CSW_TRACE(CFW_NW_TS_ID, TSTXT("the signal bit error change in 33 call progress \n")); 

#ifdef CFW_MULTI_SIM
                 CFW_CfgSetIndicatorEventInfo(&sIndEventInfo, nSimID); // for ind
	                CFW_PostNotifyEvent(EV_CFW_NW_SIGNAL_QUALITY_IND, (UINT32)nRssiEx, (UINT32)nBerEx,
	                                    CFW_NW_IND_UTI|(nSimID<<24), 0 );	
#else
                CFW_CfgSetIndicatorEventInfo(&sIndEventInfo); // for ind
                CFW_PostNotifyEvent(EV_CFW_NW_SIGNAL_QUALITY_IND, (UINT32)nRssiEx, (UINT32)nBerEx, 
                                    CFW_NW_IND_UTI, 0 );
#endif				

                CSW_TRACE(CFW_NW_TS_ID, TSTXT("rssi = %d ber = %d\n"), nRssiEx, nBerEx); 
            }
        }
        else // should not come here
        {
        }
    }
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwQualReportConf, return success\n"));
    //EV_CFW_NW_SIGNAL_LEVEL_IND
    CSW_PROFILE_FUNCTION_EXIT(CFW_NwQualReportConf);
    return ERR_SUCCESS;
}

//
PRIVATE UINT32 CFW_NwListConf(
			api_NwListInd_t *pListInd, 
			NW_SM_INFO * pSmInfo, 
			UINT32 nUTI
#ifdef CFW_MULTI_SIM
			, CFW_SIM_ID nSimID
#endif
			)
{
    UINT8 nListNum = 0;
    CFW_NW_OPERATOR_INFO *pOperatorList = NULL;
    CFW_NW_OPERATOR_INFO *pOperatorListTemp = NULL;
    UINT8 nIndex = 0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_NwListConf);
	    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwListConf start \n") );

    nListNum = pListInd->PlmnNb;    
    pOperatorList = (CFW_NW_OPERATOR_INFO *)CFW_MALLOC((UINT16)(SIZEOF(CFW_NW_OPERATOR_INFO) * nListNum));
    pOperatorListTemp = pOperatorList;
        
    for (nIndex = 0; nIndex < nListNum; nIndex++)
    {
        PlmnApi2Array(&pListInd->PlmnL[nIndex * 3], pOperatorList->nOperatorId); 
        if (pListInd->Restr[nIndex] == API_PLMN_FBD)   // forbidden
        {
            pOperatorList->nStatus = CFW_NW_OPERATOR_STATUS_FORBIDDEN;
        }
        else if (pListInd->Restr[nIndex] == API_PLMN_OK)
        {
            if (SUL_MemCompare(pOperatorList->nOperatorId, pSmInfo->nOperatorId, 6) == 0)  // the current selected
            {
                pOperatorList->nStatus = CFW_NW_OPERATOR_STATUS_CURRENT;
            }
            else  // available
            {
                pOperatorList->nStatus = CFW_NW_OPERATOR_STATUS_AVAILABLE;
            }                        
        }
        else  // unknow
        {
            pOperatorList->nStatus = CFW_NW_OPERATOR_STATUS_UNKNOW;
        }        
        //CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwListConf \n") );
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwListConf index = 0x%x, satus = 0x%x ,PLMN = %x%x%x%x%x%x\n"), 
            nIndex, 
            pOperatorList->nStatus,
            pOperatorList->nOperatorId[0],
            pOperatorList->nOperatorId[1],
            pOperatorList->nOperatorId[2],
            pOperatorList->nOperatorId[3],
            pOperatorList->nOperatorId[4],            
            pOperatorList->nOperatorId[5]);
        pOperatorList++;
    }

#ifdef CFW_MULTI_SIM
    CFW_PostNotifyEvent(EV_CFW_NW_GET_AVAIL_OPERATOR_RSP, (UINT32)pOperatorListTemp, nListNum, 
                        nUTI|(nSimID<<24), 0 );				
#else
    CFW_PostNotifyEvent(EV_CFW_NW_GET_AVAIL_OPERATOR_RSP, (UINT32)pOperatorListTemp, nListNum, 
                        nUTI, 0 );
#endif				
	
    if(pSmInfo->bTimeDeleted)   // if register fail and list the plmn, delete the timer, then restart the timer
    {
	BOOL ret;
#ifndef CFW_MULTI_SIM	
	ret = COS_SetTimerEX(0xFFFFFFFF, PRV_CFW_NW_TIMER_ID, COS_TIMER_MODE_SINGLE, 5 * 1000 MILLI_SECOND);	
#else
	UINT32 nTimerId=0;
	if(CFW_SIM_0 == nSimID)
		nTimerId = PRV_CFW_NW_SIM0_TIMER_ID;
	else if(CFW_SIM_1 == nSimID)
		nTimerId = PRV_CFW_NW_SIM1_TIMER_ID;
	else if(CFW_SIM_1+1 == nSimID)
		nTimerId = PRV_CFW_NW_SIM2_TIMER_ID;
	else if(CFW_SIM_1+2 == nSimID)
		nTimerId = PRV_CFW_NW_SIM3_TIMER_ID;
	ret = COS_SetTimerEX(0xFFFFFFFF, nTimerId, COS_TIMER_MODE_SINGLE, 5 * 1000 MILLI_SECOND);		
#endif
	pSmInfo->bTimerneed = TRUE;
					
	if(ret)
	{
	    CSW_TRACE(CFW_NW_TS_ID, TSTXT("list conf , nw timer start success\n"));
	}
	else
	{
	     CSW_TRACE(CFW_NW_TS_ID, TSTXT("nw timer start fail\n"));
	}	    
    }
	
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwListConf end \n") );
    CSW_PROFILE_FUNCTION_EXIT(CFW_NwListConf);
    return ERR_SUCCESS;
}

PRIVATE UINT32 CFW_NwPsStatusConf(
			api_NwPsRegStatusInd_t *pPsRegStatus, 
			NW_SM_INFO *pSmInfo,
			UINT32 nUTI
#ifdef CFW_MULTI_SIM
			, CFW_SIM_ID nSimID
#endif
			)
{
    UINT32 nAreaCellId = 0;
    UINT8 nStatusURC = 0xff;	
    CSW_PROFILE_FUNCTION_ENTER(CFW_NwPsStatusConf);
	
#ifdef CFW_MULTI_SIM
    CFW_CfgGetNwStatus(&nStatusURC, nSimID);	
#else
    CFW_CfgGetNwStatus(&nStatusURC);	
#endif
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("PS status = 0x%x, RAI = %x%x%x%x%x%x, CI = 0x%x%x\n"), 
        pPsRegStatus->Status, 
        pPsRegStatus->RAI[0],
        pPsRegStatus->RAI[1],
        pPsRegStatus->RAI[2],
        pPsRegStatus->RAI[3],
        pPsRegStatus->RAI[4],            
        pPsRegStatus->RAI[5],
        pPsRegStatus->CellId[0],
        pPsRegStatus->CellId[1]);   
    // should record the status in the static ao : nGprsAttState
    /*
    if(pPsRegStatus->Status == API_NW_FULL_SVCE)
    {
        pSmInfo->nGprsAttState = CFW_GPRS_ATTACHED;
    }
    else
    {
        pSmInfo->nGprsAttState = CFW_GPRS_DETACHED;    
    }
    */
    //add by wuys 2010-03-16 for handling detach originated by network
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwPsStatusConf , cause: %x \n") ,pPsRegStatus->Cause);

    if((pPsRegStatus->Status == API_NW_NO_SVCE)&&(pPsRegStatus->Cause ==API_P_NWK_DET ))
    {
            CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwPsStatusConf  no service, cause: %x \n") ,pPsRegStatus->Cause);

        pSmInfo->nGprsAttState = CFW_GPRS_DETACHED;
          #ifdef CFW_TCPIP_SUPPORT
          CFW_SmarchAttStatus();
	   #endif
    }
  
	//add end
    GprsStatusApi2At(pPsRegStatus->Status, pPsRegStatus->Roaming, &pSmInfo->sGprsStatusInfo.nStatus);
    SUL_MemCopy8(pSmInfo->sGprsStatusInfo.nAreaCode, &pPsRegStatus->RAI[3], 2);
    SUL_MemCopy8(pSmInfo->sGprsStatusInfo.nCellId, pPsRegStatus->CellId, 2);
    nAreaCellId = (((pSmInfo->sGprsStatusInfo.nAreaCode[0] << 8) | pSmInfo->sGprsStatusInfo.nAreaCode[1]) << 16) | ((pSmInfo->sGprsStatusInfo.nCellId[0] << 8) | pSmInfo->sGprsStatusInfo.nCellId[1]);
    //....need to do more...
    // if doing the attach post the rsp to uplayer
    if(pSmInfo->nAttach == 1)    // doing the attach
    {
        pSmInfo->nAttach = 0xFF;
	if(pPsRegStatus->Status == API_NW_FULL_SVCE)  // attach success
	{
#ifdef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_GPRS_ATT_RSP, 0, 0, nUTI|(nSimID<<24), 1 );				
#else
	    CFW_PostNotifyEvent(EV_CFW_GPRS_ATT_RSP, 0, 0, nUTI, 1);
#endif				
	}
	else   // attach failure
	{
#ifdef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_GPRS_ATT_RSP, ERR_CME_UNKNOWN, 0, nUTI|(nSimID<<24), 0xF1 );				
#else
	    CFW_PostNotifyEvent(EV_CFW_GPRS_ATT_RSP, ERR_CME_UNKNOWN, 0, nUTI, 0xF1);
#endif				
	}	 
    }
    else if(pSmInfo->nAttach == 0)   // doing the detach
    {
        pSmInfo->nAttach = 0xFF;
	if(pPsRegStatus->Status == API_NW_NO_SVCE)  // dettach success
	{
	    // need deactive the actived pdp context
	    // need to do un register gprs AO s
#ifdef CFW_GPRS_SUPPORT

#ifdef CFW_MULTI_SIM

	    CFW_GprsDeactiveAll(nSimID);
        CFW_PostNotifyEvent(EV_CFW_GPRS_ATT_RSP, 0, 0, nUTI|(nSimID<<24), 0);				
#else
	    CFW_GprsDeactiveAll();
	    CFW_PostNotifyEvent(EV_CFW_GPRS_ATT_RSP, 0, 0, nUTI, 0);
#endif	
#endif
	}
	else   // dettach failure
	{
#ifdef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_GPRS_ATT_RSP, ERR_CME_UNKNOWN, 0, nUTI|(nSimID<<24), 0xF0);				
#else
	    CFW_PostNotifyEvent(EV_CFW_GPRS_ATT_RSP, ERR_CME_UNKNOWN, 0, nUTI, 0xF0);
#endif				
	}	 
    } 
    else   // nothing action
    {
        // post the status ind to uplayer according to the URC, need to do
        if (nStatusURC == 1 || nStatusURC == 2)
        {
#ifdef CFW_MULTI_SIM
            CFW_PostNotifyEvent(EV_CFW_GPRS_STATUS_IND, (UINT32)pSmInfo->sGprsStatusInfo.nStatus, nAreaCellId, nUTI|(nSimID<<24), 2);				
#else
            CFW_PostNotifyEvent(EV_CFW_GPRS_STATUS_IND, (UINT32)pSmInfo->sGprsStatusInfo.nStatus, nAreaCellId, nUTI, 2);    
#endif				
		}
        
    }
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwPsStatusConf end \n") );
    CSW_PROFILE_FUNCTION_EXIT(CFW_NwPsStatusConf);
    return ERR_SUCCESS;
}

// store the status info and send the info to at according to the URC
PRIVATE UINT32 CFW_NwCsStatusConf(
		api_NwCsRegStatusInd_t *pCsRegStatus, 
		NW_SM_INFO *pSmInfo
#ifdef CFW_MULTI_SIM
		, CFW_SIM_ID nSimID
#endif
		)
{
	//CFW_NW_STATUS_INFO *pOutMsg = NULL;
	CFW_IND_EVENT_INFO sIndEventInfo;
	UINT8 nStatusURC = 0xff;
	//UINT32 nAreaCellId = 0;
	BOOL bNetAvailableEx;   // net work available
	BOOL bRoamingEx;        // roaming ?

	CSW_PROFILE_FUNCTION_ENTER(CFW_NwCsStatusConf);
#ifdef CFW_MULTI_SIM
	CFW_CfgGetIndicatorEventInfo(&sIndEventInfo, nSimID);
#else
	CFW_CfgGetIndicatorEventInfo(&sIndEventInfo);
#endif

	NetStatusApi2At(pCsRegStatus->Status, pCsRegStatus->Roaming, &pSmInfo->sStatusInfo.nStatus);
	//SUL_MemCopy8(pSmInfo->sStatusInfo.nAreaCode, &pCsRegStatus->LAI[0], 3);
	SUL_MemCopy8(pSmInfo->sStatusInfo.nCellId, pCsRegStatus->CellId, 2);
	PlmnApi2Array(pCsRegStatus->LAI, pSmInfo->nOperatorId);
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("cs status = 0x%x, cs roaming = 0x%x ,PLMN = %x%x%x%x%x%x\n"), 
	pCsRegStatus->Status, 
	pCsRegStatus->Roaming,
	pSmInfo->nOperatorId[0],
	pSmInfo->nOperatorId[1],
	pSmInfo->nOperatorId[2],
	pSmInfo->nOperatorId[3],
	pSmInfo->nOperatorId[4],            
	pSmInfo->nOperatorId[5]);   
	bRoamingEx = pSmInfo->bRoaming;
	bNetAvailableEx = pSmInfo->bNetAvailable;

#ifdef CFW_MULTI_SIM
	CFW_CfgGetNwStatus(&nStatusURC, nSimID);
#else
	CFW_CfgGetNwStatus(&nStatusURC);
#endif
	if (pCsRegStatus->Roaming)
	{
		sIndEventInfo.roam = 1;
		pSmInfo->bRoaming = TRUE;
	}
	else
	{
		sIndEventInfo.roam = 0;
		pSmInfo->bRoaming = FALSE;
	}

	if (pCsRegStatus->Status == API_NW_FULL_SVCE)
	{
		pSmInfo->bNetAvailable = TRUE;
	}
	else
	{
		pSmInfo->bNetAvailable = FALSE;
	}
	//nAreaCellId = (((pSmInfo->sStatusInfo.nAreaCode[0] << 8) | pSmInfo->sStatusInfo.nAreaCode[1]) << 16) | ((pSmInfo->sStatusInfo.nCellId[0] << 8) | pSmInfo->sStatusInfo.nCellId[1]);
#ifdef CFW_MULTI_SIM    
	CFW_CfgSetIndicatorEventInfo(&sIndEventInfo, nSimID);
#else
	CFW_CfgSetIndicatorEventInfo(&sIndEventInfo);
#endif
	//nStatusURC = 1;
	if (nStatusURC == 1 || nStatusURC == 2)
	{
		UINT16 nIndEvent = 0;
#ifdef CFW_MULTI_SIM		
		CFW_CfgGetIndicatorEvent(&nIndEvent, nSimID);
#else
		CFW_CfgGetIndicatorEvent(&nIndEvent);
#endif
		// has a problem : if roam and avail both; after post the ind event will be confused
		if(nIndEvent & CFW_CFG_IND_ROAM_EVENT)
		{
			if (pSmInfo->bRoaming != bRoamingEx)
			{
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("roam change 11\n") );
#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_NW_REG_STATUS_IND, pSmInfo->bRoaming, 0,
				        CFW_NW_IND_UTI|(nSimID<<24), 1);				
#else
				CFW_PostNotifyEvent(EV_CFW_NW_REG_STATUS_IND, pSmInfo->bRoaming, 0, 
				        CFW_NW_IND_UTI, 1 );
#endif				
			}
		}
		if (nIndEvent & CFW_CFG_IND_NW_EVENT) 
		{
			if ( (pSmInfo->bNetAvailable != bNetAvailableEx) || (pCsRegStatus->Status == API_NW_NO_SVCE) )
			{
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("net avail change 22\n") );

#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_NW_REG_STATUS_IND, pSmInfo->bNetAvailable, 0,
				        CFW_NW_IND_UTI|(nSimID<<24), 0);				
#else
				CFW_PostNotifyEvent(EV_CFW_NW_REG_STATUS_IND, pSmInfo->bNetAvailable, 0, 
				        CFW_NW_IND_UTI, 0 );
#endif				
			}
		}
		//CSW_TRACE(CFW_NW_TS_ID, TSTXT("before send  EV_CFW_NW_REG_STATUS_IND, pSmInfo->nAreaCellId=0x%x, nAreaCellId=0x%x\n"), pSmInfo->nAreaCellId, nAreaCellId);
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("before send  EV_CFW_NW_REG_STATUS_IND, pSmInfo->bNetAvailable=0x%x, bNetAvailableEx=0x%x\n"), pSmInfo->bNetAvailable, bNetAvailableEx);
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("before send  EV_CFW_NW_REG_STATUS_IND, pCsRegStatus->Status=0x%x, API_NW_NO_SVCE=0x%x\n"), pCsRegStatus->Status, API_NW_NO_SVCE);
		//frank modify this line for MMI,because MMI don't process CS status when other card is calling. If our phone is calling,we will lose CS Event.
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId; 
	UINT8 nAOCount = 0;
	INT16 nCmp         = 0;
	for (nSimId = CFW_SIM_0; nSimId < CFW_SIM_COUNT; nSimId ++)
	{
		if(nSimId!=nSimID )
		{
			 nAOCount += CFW_GetAoCount(CFW_CC_SRV_ID, nSimId);
		}
	}
	
	nCmp = SUL_MemCompare(&pCsRegStatus->LAI[0], pSmInfo->sStatusInfo.nAreaCode, 3);
       //if nAOCount > 0 ,have other sim have calling.if nCmp ==0,have sent event to mmi before.
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("before send  EV_CFW_NW_REG_STATUS_IND, nAOCount=0x%x, nCmp=0x%x\n"), nAOCount,nCmp);
	if (((nAOCount == 0)&&(nCmp != 0) //pSmInfo->nAreaCellId != nAreaCellId
#else
	if ((SUL_MemCompare(&pCsRegStatus->LAI[0], pSmInfo->sStatusInfo.nAreaCode, 3) 
#endif
			|| (pSmInfo->bNetAvailable != bNetAvailableEx) 
			|| (pCsRegStatus->Status == API_NW_NO_SVCE) 
			|| (pCsRegStatus->Status == API_NW_LTD_SVCE) ))
	{
		SUL_MemCopy8(pSmInfo->sStatusInfo.nAreaCode, &pCsRegStatus->LAI[0], 3);
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("send  EV_CFW_NW_REG_STATUS_IND to ATL 33\n") );

#ifdef CFW_MULTI_SIM
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("send  EV_CFW_NW_REG_STATUS_IND nSimID=%d, status to MMI is: %d\n"), nSimID, pSmInfo->sStatusInfo.nStatus);
	      if ((SOS_NO_SEND_SIM_OPEN == (UINT8)CFW_GetSimOpenStatus(nSimID)) && (pCsRegStatus->Status == API_NW_LTD_SVCE))
	      {
	        	CFW_PostNotifyEvent(EV_CFW_NW_REG_STATUS_IND, (UINT32)CFW_NW_STATUS_NOTREGISTERED_SEARCHING, 0x00,
	                            CFW_NW_IND_UTI | (nSimID << 24), 2);
	      }
	      else
	      {
			CFW_PostNotifyEvent(EV_CFW_NW_REG_STATUS_IND, (UINT32)pSmInfo->sStatusInfo.nStatus, 0x00, 
			CFW_NW_IND_UTI|(nSimID<<24), 2 );
	      }
		CFW_PostNotifyEvent(EV_CFW_NW_REG_STATUS_IND, (UINT32)pSmInfo->sStatusInfo.nStatus, 0x00, 
		GENERATE_SHELL_UTI()|(nSimID<<24), 2 );			
#else
	      if ((SOS_NO_SEND_SIM_OPEN == (UINT8)CFW_GetSimOpenStatus()) && (pCsRegStatus->Status == API_NW_LTD_SVCE))
	      {
	        	CFW_PostNotifyEvent(EV_CFW_NW_REG_STATUS_IND, (UINT32)CFW_NW_STATUS_NOTREGISTERED_SEARCHING, 0x00,
	                            CFW_NW_IND_UTI, 2);
	      }
	      else
	      {
	        	CFW_PostNotifyEvent(EV_CFW_NW_REG_STATUS_IND, (UINT32)pSmInfo->sStatusInfo.nStatus, 0x00, CFW_NW_IND_UTI, 2);
	      }
		CFW_PostNotifyEvent(EV_CFW_NW_REG_STATUS_IND, (UINT32)pSmInfo->sStatusInfo.nStatus, 0x00, 
		GENERATE_SHELL_UTI(), 2 );			
#endif				
		}
	}
	if ((pSmInfo->bDetached) && ( pSmInfo->nCsStatusIndCount == 0)) // just for omit the first cs status ind message
	{
		CSW_TRACE(CFW_NW_TS_ID, TSTXT("get cs status first \n") );    
		pSmInfo->nCsStatusIndCount = 1;
		CSW_PROFILE_FUNCTION_EXIT(CFW_NwCsStatusConf);
		return ERR_SUCCESS;
	}

	// if rel or sel... need do something
	if (pSmInfo->bChangeBand)
	{
	
		pSmInfo->bChangeBand = FALSE;
		pSmInfo->bDetached = FALSE;
		pSmInfo->bReselNW = FALSE;		
		if (pCsRegStatus->Status == API_NW_FULL_SVCE)
		{    
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("change the band success\n") );

#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_NW_SET_BAND_RSP, pSmInfo->nBand, 0, 
			    pSmInfo->nBandUTI|(nSimID<<24), 0);				
#else
			CFW_PostNotifyEvent(EV_CFW_NW_SET_BAND_RSP, pSmInfo->nBand, 0, 
			    pSmInfo->nBandUTI, 0 );
#endif							
		}
		else if (pCsRegStatus->Status == API_NW_LTD_SVCE)
		{
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("change the band limited service\n") );

#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_NW_SET_BAND_RSP, ERR_CME_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY, 0, 
			    pSmInfo->nBandUTI|(nSimID<<24), 0xF0);				
#else
			CFW_PostNotifyEvent(EV_CFW_NW_SET_BAND_RSP, ERR_CME_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY, 0, 
			    pSmInfo->nBandUTI, 0xF0 );
#endif							
		}
		else
		{
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("change the band fail\n") );

#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_NW_SET_BAND_RSP, ERR_CME_NO_NETWORK_SERVICE, 0, 
			    pSmInfo->nBandUTI|(nSimID<<24), 0xF0);				
#else
			CFW_PostNotifyEvent(EV_CFW_NW_SET_BAND_RSP, ERR_CME_NO_NETWORK_SERVICE, 0, 
			    pSmInfo->nBandUTI, 0xF0 );
#endif							
		}
	}
	else if (pSmInfo->bDetached)
	{
		if (pCsRegStatus->Status == API_NW_FULL_SVCE)
		{
			pSmInfo->bDetached = FALSE;
			pSmInfo->bReselNW = FALSE;
			if (pSmInfo->nNetMode != CFW_NW_AUTOMATIC_MODE)
			{
#ifdef CFW_MULTI_SIM
				CFW_NwSetRegistration(pSmInfo->nAttOperatorId, pSmInfo->nNetMode, pSmInfo->nReselUTI, nSimID);
#else
				CFW_NwSetRegistration(pSmInfo->nAttOperatorId, pSmInfo->nNetMode, pSmInfo->nReselUTI);
#endif
			}    
			else
			{

				UINT8 *pPLMN = (UINT8 *)CFW_MALLOC(6);
				SUL_MemCopy8(pPLMN, pSmInfo->nOperatorId, 6);
				CSW_TRACE(CFW_NW_TS_ID, TSTXT("send  EV_CFW_NW_SET_REGISTRATION_RSP to ATL\n") );

#ifdef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_NW_SET_REGISTRATION_RSP, 
					(UINT32)pPLMN, 
					(UINT32)(pSmInfo->nNetMode == API_NW_AUTOMATIC_MODE ? API_NW_AUTOMATIC_MODE : API_NW_MANUAL_MODE),      
					pSmInfo->nBandUTI|(nSimID<<24), 
					0);				
#else
				CFW_PostNotifyEvent(EV_CFW_NW_SET_REGISTRATION_RSP, 
					(UINT32)pPLMN, 
					(UINT32)(pSmInfo->nNetMode == API_NW_AUTOMATIC_MODE ? API_NW_AUTOMATIC_MODE : API_NW_MANUAL_MODE), 
					pSmInfo->nCOPSUTI,
					0);
#endif							
			}
		}
		else if (pCsRegStatus->Status == API_NW_NO_SVCE)
		{
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("No service start stack s_proc->nBand = 0x%x\n"), pSmInfo->nBand);   

#ifdef CFW_MULTI_SIM
			CFW_NwStartStack(pSmInfo->nBand, nSimID);
#else
			CFW_NwStartStack(pSmInfo->nBand);
#endif
		}
		else
		{
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("status is not full service, send API_SIM_OPEN_IND to Stack\n"));

#ifdef CFW_MULTI_SIM
			//CFW_SendSclMessage(API_SIM_OPEN_IND, 0, nSimID);
#else
			CFW_SendSclMessage(API_SIM_OPEN_IND, 0);    // need sim open for stack
#endif

		}
	}
	else if(pSmInfo->bReselNW) // attach to another plmn
	{
		pSmInfo->bReselNW = FALSE;
		if (pCsRegStatus->Status == API_NW_FULL_SVCE)
		{  
			UINT8 *pPLMN = (UINT8 *)CFW_MALLOC(6);
			SUL_MemCopy8(pPLMN, pSmInfo->nOperatorId, 6);
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("send  EV_CFW_NW_SET_REGISTRATION_RSP to ATL\n") );

#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_NW_SET_REGISTRATION_RSP, 
				(UINT32)pPLMN, 
				(UINT32)(pSmInfo->nNetMode == API_NW_AUTOMATIC_MODE ? API_NW_AUTOMATIC_MODE : API_NW_MANUAL_MODE), 
				pSmInfo->nCOPSUTI|(nSimID<<24), 
				0);				
#else
			CFW_PostNotifyEvent(EV_CFW_NW_SET_REGISTRATION_RSP, 
				(UINT32)pPLMN, 
				(UINT32)(pSmInfo->nNetMode == API_NW_AUTOMATIC_MODE ? API_NW_AUTOMATIC_MODE : API_NW_MANUAL_MODE), 
				pSmInfo->nCOPSUTI,
				0);
#endif							
		}
		else if (pCsRegStatus->Status == API_NW_LTD_SVCE)
		{

			if(pSmInfo->nNetMode == CFW_NW_MANUAL_AUTOMATIC_MODE)
			{
				BOOL ret;
#ifndef CFW_MULTI_SIM
				ret = COS_SetTimerEX(
				0xFFFFFFFF, PRV_CFW_NW_TIMER_ID, 
				COS_TIMER_MODE_SINGLE, 5 * 1000 MILLI_SECOND);		
#else
				UINT32 nTimerId = 0;
				if(CFW_SIM_0 == nSimID)
					nTimerId = PRV_CFW_NW_SIM0_TIMER_ID;
				else if(CFW_SIM_1 == nSimID)
					nTimerId = PRV_CFW_NW_SIM1_TIMER_ID;
				else if((CFW_SIM_1+1) == nSimID)
					nTimerId = PRV_CFW_NW_SIM2_TIMER_ID;
				else if((CFW_SIM_1+2) == nSimID)
					nTimerId = PRV_CFW_NW_SIM3_TIMER_ID;
				ret = COS_SetTimerEX(
					0xFFFFFFFF, nTimerId, 
					COS_TIMER_MODE_SINGLE, 5 * 1000 MILLI_SECOND);		
#endif
				pSmInfo->bTimerneed = TRUE;

				if(ret)
				{
					CSW_TRACE(CFW_NW_TS_ID, TSTXT("register fail,  CFW_NW_MANUAL_AUTOMATIC_MODE mode, nw timer start success\n"));
				}
				else
				{
					CSW_TRACE(CFW_NW_TS_ID, TSTXT("nw timer start fail\n"));
				}	

				}            	
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("try to attach to another plmn fail limited service\n") );

#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_NW_SET_REGISTRATION_RSP, ERR_CME_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY, 0,
			        pSmInfo->nCOPSUTI|(nSimID<<24), 0xF0);				
#else
			CFW_PostNotifyEvent(EV_CFW_NW_SET_REGISTRATION_RSP, ERR_CME_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY, 0,
			        pSmInfo->nCOPSUTI, 0xF0 );
#endif							

		}
		else
		{
			CSW_TRACE(CFW_NW_TS_ID, TSTXT("try to attach to another plmn fail no service\n") );

#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_NW_SET_REGISTRATION_RSP, ERR_CME_NO_NETWORK_SERVICE, 0, 
			        pSmInfo->nCOPSUTI|(nSimID<<24), 0xF0);				
#else
			CFW_PostNotifyEvent(EV_CFW_NW_SET_REGISTRATION_RSP, ERR_CME_NO_NETWORK_SERVICE, 0, 
			        pSmInfo->nCOPSUTI, 0xF0 );
#endif							
		}
	}        
	else
	{
		  CSW_TRACE(CFW_NW_TS_ID, TSTXT("warning !CFW_NwCsStatusConf attach to another plmn  \n") );
	}
	
	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwCsStatusConf end \n") );

	//..........................
	//CFW_PostNotifyEvent
	CSW_PROFILE_FUNCTION_EXIT(CFW_NwCsStatusConf);
	return ERR_SUCCESS;
}

UINT32 CFW_NwSetFrequencyBand (
			UINT8 nBand,
			UINT16 nUTI
#ifdef CFW_MULTI_SIM
			,CFW_SIM_ID nSimID
#endif
)
{
    NW_SM_INFO* s_proc = NULL;    // static sm info
    HAO s_hAo = 0;              // static handle
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwSetFrequencyBand nBand = 0x%x, nUTI = 0x%x\n"), nBand, nUTI);

#ifdef CFW_MULTI_SIM
    UINT8 ret;
    ret = CFW_CheckSimId(nSimID);
    if( ERR_SUCCESS != ret)
    {
    		CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwSetFrequencyBand SIM ERROR!\n"));

		return ret;
	}
#endif


	if((nBand == 0) || (nBand & API_GSM_450) || (nBand & API_GSM_480) || (nBand & API_GSM_900R))
    {
    	  CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !CFW_NwSetFrequencyBand input error! \n") );

        return ERR_CFW_INVALID_PARAMETER;
    }
    if (CFW_IsFreeUTI(nUTI, CFW_NW_SRV_ID) != ERR_SUCCESS)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !CFW_NwSetFrequencyBand UTI_IS_BUSY \n") );
        return ERR_CFW_UTI_IS_BUSY;
    }

#ifdef CFW_MULTI_SIM
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
	CFW_SetServiceId(CFW_NW_SRV_ID);
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif
	
    if (s_hAo == 0)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !s_hAo == 0 \n"));
        return -1; // AOM error
    }

    s_proc = CFW_GetAoUserData(s_hAo);
    s_proc->nCsStatusIndCount = 0;

    s_proc->nBand = nBand;
    s_proc->bChangeBand = TRUE;
	s_proc->nStpCause   = STOP_CAUSE_FB;
#ifdef CFW_MULTI_SIM
    CFW_CfgNwSetFrequencyBand(nBand, nSimID);
#else
    CFW_CfgNwSetFrequencyBand(nBand);
#endif

    //CFW_NwStopStack();

#ifdef CFW_MULTI_SIM
    CFW_NwStop(FALSE, nUTI, nSimID);
#else
    CFW_NwStop(FALSE, nUTI);
#endif

	CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwSetFrequencyBand end \n") );

    return ERR_SUCCESS;
}

// AT+CGATT  ps_ind should be move to gprs modle
// ps attach and detach
// nState: 
// CFW_GPRS_DETACHED 0 detached 
// CFW_GPRS_ATTACHED 1 attached 
// event:EV_CFW_GPRS_ATT_NOTIFY
// parameter1: 0
// parameter2: 0
// type: 0 or 1 to distinguish the attach or detach
#ifdef CFW_GPRS_SUPPORT
UINT32 CFW_GprsAtt(
			UINT8 nState, 
			UINT16 nUTI
#ifdef CFW_MULTI_SIM
			, CFW_SIM_ID nSimID
#endif
			)
{
    NW_SM_INFO * proc = NULL;     //  sm info
    HAO hAo = 0;                  //  ao
    UINT8 nAttstate = 0xFF;
    HAO s_hAo = 0;              // staitc ao
    NW_SM_INFO *s_proc = NULL;  // static sm
    CSW_PROFILE_FUNCTION_ENTER(CFW_GprsAtt);
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GprsAtt state = 0x%x, nUTI = 0x%x\n"), nState, nUTI);

#ifdef CFW_MULTI_SIM
    UINT8 ret;
    ret = CFW_CheckSimId(nSimID);
    if( ERR_SUCCESS != ret)
    {
    		CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GprsAtt SIM ERROR!\n"));

	       CSW_PROFILE_FUNCTION_EXIT(CFW_GprsAtt);
		return ret;
    }
#endif

	if((nState != CFW_GPRS_ATTACHED) && (nState != CFW_GPRS_DETACHED))
    {
    	    CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR CFW_GprsAtt input error\n") );

        CSW_PROFILE_FUNCTION_EXIT(CFW_GprsAtt);
        return ERR_CFW_INVALID_PARAMETER;
    }
    if (CFW_IsFreeUTI(nUTI, CFW_NW_SRV_ID) != ERR_SUCCESS)
    {
    	    CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !CFW_GprsAtt ERR_CFW_UTI_IS_BUSY \n") );

        CSW_PROFILE_FUNCTION_EXIT(CFW_GprsAtt);
        return ERR_CFW_UTI_IS_BUSY;
    }
#ifdef CFW_MULTI_SIM
	CFW_GetGprsAttState (&nAttstate, nSimID);
#else
	CFW_GetGprsAttState(&nAttstate);
#endif
    
    if(((nAttstate == CFW_GPRS_ATTACHED) && (nState == CFW_GPRS_ATTACHED))
       ||( (nAttstate == CFW_GPRS_DETACHED) && (nState == CFW_GPRS_DETACHED)))
    {
    	   CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !CFW_GprsAtt get data error \n") );

        CSW_PROFILE_FUNCTION_EXIT(CFW_GprsAtt);
        return ERR_CME_OPERATION_NOT_ALLOWED;
    }
    proc = (NW_SM_INFO*)CFW_MALLOC(SIZEOF(NW_SM_INFO));
    if (proc == NULL)   // malloc error
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("malloc exception\n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_GprsAtt);
        return ERR_CME_MEMORY_FULL;  // should be updated by macro
    }

		
#ifdef CFW_MULTI_SIM
        s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
	CFW_SetServiceId(CFW_NW_SRV_ID);
        s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif
        s_proc = CFW_GetAoUserData(s_hAo);
/*
	 if((CFW_NW_STATUS_NOTREGISTERED_NOTSEARCHING  == s_proc->sGprsStatusInfo.nStatus)&&(
	 	(API_P_ILLEGAL_USR == s_proc->sGprsStatusInfo.nCause)||
	 	(API_P_SRVC_NOT_ALLOWD == s_proc->sGprsStatusInfo.nCause)))
	 {
	 	 CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !API_NW_NO_SVCE  == s_proc->sGprsStatusInfo.nStatus \n") );
	 	 return ERR_CME_OPERATION_NOT_ALLOWED;
	 }
        */
    // Clear the proc structure, ensure that there is not existing un-intialized member.
    SUL_ZeroMemory32(proc, SIZEOF(NW_SM_INFO));
    if (nState == CFW_GPRS_ATTACHED) // attach
    {
        proc->nMsgId = API_NW_PSATTACH_REQ;
        proc->pMsgBody = NULL;
    }
    else   // detach
    {
        proc->nMsgId = API_NW_PSDETACH_REQ;
        proc->pMsgBody = NULL;
    }

#ifdef CFW_MULTI_SIM
    hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc, nSimID);
#else
    hAo = CFW_RegisterAo(CFW_NW_SRV_ID, proc, CFW_NwAoProc);
#endif

    CFW_SetUTI(hAo, nUTI, 0);
    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom
    
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GprsAtt end \n") );
    CSW_PROFILE_FUNCTION_EXIT(CFW_GprsAtt);
    
    return ERR_SUCCESS;
}

// get the att state attached or not?
//[out]pState

UINT32 CFW_GetGprsAttState (
		UINT8* pState
#ifdef CFW_MULTI_SIM
		, CFW_SIM_ID nSimID
#endif
		)
{
    NW_SM_INFO* s_proc = NULL;    // sm info
    HAO s_hAo = 0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_GetGprsAttState);
	    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GetGprsAttState start \n") );

#ifdef CFW_MULTI_SIM
    UINT8 ret;
    ret = CFW_CheckSimId(nSimID);
    if( ERR_SUCCESS != ret)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GprsAtt SIM ERROR!\n"));

	    CSW_PROFILE_FUNCTION_EXIT(CFW_GetGprsAttState);
		return ret;
	}
#endif

    if (pState == NULL)
    {
    	  CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR CFW_GprsAtt SIM input error!\n"));

        CSW_PROFILE_FUNCTION_EXIT(CFW_GetGprsAttState);
        return ERR_CFW_INVALID_PARAMETER;
    }

#ifdef CFW_MULTI_SIM
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
	CFW_SetServiceId(CFW_NW_SRV_ID);
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif

    if (s_hAo == 0)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !s_hAo == 0 \n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_GetGprsAttState);
        return -1; // AOM error
    }

    s_proc = CFW_GetAoUserData(s_hAo);    
    *pState = s_proc->nGprsAttState;
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GetGprsAttState end state = 0x%x\n"), *pState);
    CSW_PROFILE_FUNCTION_EXIT(CFW_GetGprsAttState);
    return ERR_SUCCESS;
}

// get the gprs status
// [out]pStatus

UINT32 CFW_GprsGetstatus(
		CFW_NW_STATUS_INFO *pStatus
#ifdef CFW_MULTI_SIM
		, CFW_SIM_ID nSimID
#endif
		)  
{
    NW_SM_INFO* s_proc = NULL;    // sm info
    HAO s_hAo = 0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_GprsGetstatus);

	    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GprsGetstatus start \n") );

#ifdef CFW_MULTI_SIM
    UINT8 ret;
    ret = CFW_CheckSimId(nSimID);
    if( ERR_SUCCESS != ret)
    {
    	    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GprsGetstatus SIM ERROR!\n"));
	    CSW_PROFILE_FUNCTION_EXIT(CFW_GprsGetstatus);
		return ret;
	}
#endif

    if (pStatus == NULL)
    {
    	  CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR CFW_GprsGetstatus SIM input error!\n"));

        CSW_PROFILE_FUNCTION_EXIT(CFW_GprsGetstatus);
        return ERR_CFW_INVALID_PARAMETER;
    }

#ifdef CFW_MULTI_SIM
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
	CFW_SetServiceId(CFW_NW_SRV_ID);
    s_hAo = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW);
#endif

    if (s_hAo == 0)
    {
        CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR !s_hAo == 0 \n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_GprsGetstatus);
        return -1; // AOM error
    }
    s_proc = CFW_GetAoUserData(s_hAo);  
    SUL_MemCopy8(pStatus, &s_proc->sGprsStatusInfo, SIZEOF(CFW_NW_STATUS_INFO));
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GprsGetstatus status = 0x%x LAC = 0x%x%x, CI = 0x%x%x\n"), 
        pStatus->nStatus,
        pStatus->nAreaCode[0],
        pStatus->nAreaCode[1],
        pStatus->nCellId[0],
        pStatus->nCellId[1]);
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_GprsGetstatus);
    return ERR_SUCCESS;
}
#endif

// This message is used by the MMI to abort the construction of the list of surrounding PLMNs.
// If the CFW_NwGetAvailableOperators does not sent before, the CFW_NwListAbort should not be called.
// return 

UINT32 CFW_NwAbortListOperators(
		UINT16 nUTI
#ifdef CFW_MULTI_SIM
		, CFW_SIM_ID nSimID
#endif
		)
{
   NW_SM_INFO *proc = NULL;
    HAO hAo = 0;
    UINT8 nAoCount = 0;
    UINT8 nAoIndex = 0;

    CSW_PROFILE_FUNCTION_ENTER(CFW_NwAbortListOperators);
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAbortListOperators start\n"));

#ifdef CFW_MULTI_SIM
    UINT8 ret;
    ret = CFW_CheckSimId(nSimID);
	if( ERR_SUCCESS != ret)
	{
    	    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GprsGetstatus SIM ERROR!\n"));
		CSW_PROFILE_FUNCTION_EXIT(CFW_NwAbortListOperators);
		return ret;
	}
#endif
	
    if (CFW_IsFreeUTI(nUTI, CFW_NW_SRV_ID) != ERR_SUCCESS)
    {
    	 CSW_TRACE(CFW_NW_TS_ID, TSTXT("ERROR! CFW_GprsGetstatus SIM ERROR!UTI_IS_BUSY\n"));
        CSW_PROFILE_FUNCTION_EXIT(CFW_NwAbortListOperators);
        return ERR_CFW_UTI_IS_BUSY;
    }
#ifdef CFW_MULTI_SIM
    nAoCount = CFW_GetAoCount(CFW_NW_SRV_ID, nSimID);
#else
    CFW_SetServiceId(CFW_NW_SRV_ID);
    nAoCount = CFW_GetAoCount();
#endif
    for (nAoIndex = 0; nAoIndex < nAoCount; nAoIndex++)
    {
#ifdef CFW_MULTI_SIM
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
    	CFW_SetServiceId(CFW_NW_SRV_ID);
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW);
#endif
	
        proc = CFW_GetAoUserData(hAo);

        if (proc->nMsgId == API_NW_LIST_REQ)
        {
            proc->nMsgId = API_NW_LISTABORT_REQ;
            proc->pMsgBody = NULL;
            proc->bStaticAO = FALSE;
            CFW_SetUTI(hAo, nUTI, 0);
            CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONTINUE); // trigger the aom
            CSW_PROFILE_FUNCTION_EXIT(CFW_NwAbortListOperators);
            return ERR_SUCCESS;            
        }
    }
     CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_NwAbortListOperators end\n"));

    CSW_PROFILE_FUNCTION_EXIT(CFW_NwAbortListOperators);
    
    return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
}
#ifdef CFW_GPRS_SUPPORT
VOID CFW_GprsDeactiveAll(
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID
#endif
	)
{
    HAO hAo = 0;                  //  ao
    UINT8 nAoCount = 0;
    UINT8 nAoIndex = 0;	
    CSW_PROFILE_FUNCTION_ENTER(CFW_GprsDeactiveAll);
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GprsDeactiveAll start\n"));

#ifdef CFW_MULTI_SIM
    nAoCount = CFW_GetAoCount(CFW_GPRS_SRV_ID,nSimID);
#else	
    CFW_SetServiceId(CFW_GPRS_SRV_ID);
    nAoCount = CFW_GetAoCount();
#endif
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GprsDeactiveAll nAoCount: %d\n"),nAoCount);

    for (nAoIndex = 0; nAoIndex < nAoCount; nAoIndex++)
    {
#ifdef CFW_MULTI_SIM
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW, CFW_NW_SRV_ID, nSimID);
#else
    	CFW_SetServiceId(CFW_NW_SRV_ID);
        hAo = CFW_GetAoHandle(nAoIndex, CFW_SM_STATE_UNKONW);
#endif
	
        CFW_UnRegisterAO(CFW_GPRS_SRV_ID, hAo);	
    }
    CSW_TRACE(CFW_NW_TS_ID, TSTXT("CFW_GprsDeactiveAll end\n"));

    CSW_PROFILE_FUNCTION_EXIT(CFW_GprsDeactiveAll);
}
#endif
#endif // ENALB

