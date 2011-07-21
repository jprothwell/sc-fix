/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename   : adp_GPRS_Events.c                                             */
/* Author     : JIASHUO                                                       */
/* Description:                                                               */
/* dispatch GPRS related message                                             */
/* Date       : 20090211                                                      */
/******************************************************************************/
#ifdef __GPRS_MODE__

#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_features.h"

 
#include "wgui_categories_inputs.h"
#include "imeres.h"
 
#include "globalconstants.h"
#include "taskinit.h"
#include "mmitaskprot.h"
#include "thrdosinterface.h"
#include "globalstruct.h"
#include "debuginitdef.h"
#include "protocolevents.h"
#include "asynceventsgprot.h"
#include "alarmframeworkprot.h"    
#include "callmanagementgprot.h"
#include "callmanagementstruct.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "service.h"
#include "satgprots.h"
#include "cphsprotocolhandlers.h"
#include "engineermodeprot.h"
#include "factorymodeprot.h"
#include "gpioinc.h"
#include "phonebookgprot.h"
#include "callhistorymain.h"
#include "funandgamesprots.h"
#include "messagesresourcedata.h"
#include "organizergprot.h"
#include "alarmgprot.h"
#include "scheduledpoweronoffgprot.h"
#include "unicodedcl.h"
#include "unicodedef.h"
#include "organizergprot.h"
#include "idleappprot.h"
#include "conversions.h"
#include "nvramprot.h"
 
#include <csw.h>
#include "mmi.h"
#include "uim.h"
#include "adp_events.h"
#include "adp_eventsredef.h"
#include "cfw.h"   
#include "phonebooktypes.h"  
#include "dataaccountdef.h" 

 
#ifdef __MMI_THEMES_V2_SUPPORT__
#include "thememanager.h"
#endif

#include "messagesexdcl.h"

#ifdef __MMI_USB_SUPPORT__
#include "usbdevicegprot.h"
   #ifdef MMI_ON_HARDWARE_P
   extern kal_bool INT_USBBoot(void);
   #endif
#endif

#ifdef __MMI_IRDA_SUPPORT__
#include "irdammigprots.h"
#endif


#ifdef __MMI_FILE_MANAGER__
#include "filemanagergprot.h"
#include "msdc_def.h"
#endif

#ifdef __MMI_WEBCAM__
#include "mdi_datatype.h"
#include "mdi_webcam.h"
#endif /* __MMI_WEBCAM__ */

#include "soundeffect.h"

#include "shortcutsprots.h"
#include "sublcdhistorygprot.h"
#include "todolistdef.h"

#include "wpssprotos.h"
#include "calleridgprot.h"

#if defined(__MMI_STOPWATCH__)
#include "stopwatchgprot.h"
#endif

#if defined(__MMI_EMAIL__)		
#include "emailappgprot.h"
#endif // def __MMI_EMAIL__	
#include "osthreadinterfacegprot.h"

#include "eventsgprot.h"
#include "allappgprot.h"
#include "wrappergprot.h"
#include "queuegprot.h"

#include "keybrd.h"
#include "nvramexdcl.h"
#include "simdetectiongprot.h"
#include "wgui_status_icons.h"
#include "poweronchargerprot.h"
#include "settinggprots.h"
#include "simdetectiongexdcl.h"
#include "phonesetupgprots.h" /* phnsetreadnvramcalibrationdata */
#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
#endif /*__MMI_SMART_MESSAGE_MO__ && !__MMI_MESSAGES_EMS__*/

 
#ifdef __MMI_POC__
#include "pocgprot.h"
#endif

#undef __NEWSIMULATOR

#ifdef MMI_ON_HARDWARE_P
#include "lcd_sw_inc.h"
#include "sublcd.h"
#else
#include "lcd_if.h"
#endif
#include "composeringtoneprots.h"

#ifdef __MMI_IMPS__
#include "mmi_imps_gprot.h"
#endif

#include "nvram_user_defs.h"
#include "stack_msgs.h"
#include "tcpip_inet.h"

UINT32 gprs_simId = 0;
UINT8   g_nSocCid  = 0;
 UINT8 g_apnsize =0;
 char    g_apnName[16];

	
adp_wap_context_t adp_gWAPContext[ADP_SIM_NUMBER];
adp_wap_context_t *adp_pWAPContext = adp_gWAPContext;
extern HostName_RequestID_struct g_struRequest[];
extern UINT32   g_nRequestCount;

BOOL app_Data_Conn_Callback (COS_EVENT *pCosEvent);


#if 1
#ifdef __ADP_MULTI_SIM__

PFN_DATA_CONN_CALLBACK      pfn_DataConn[ADP_SIM_NUMBER][8]  = {{NULL},};
UINT32 CFW_SetDataConnFunEx ( PFN_DATA_CONN_CALLBACK pfnEv, UINT8 SimId,  UINT8 Cid )
{
		mmi_trace (1, "JDD_LOG: CFW_SetDataConnFunEx, SimId: %d, Cid: %d, callback: 0x%x",SimId,Cid,pfnEv) ;

    if( Cid > 7)
          return ERR_INVALID_PARAMETER;
    pfn_DataConn[SimId][Cid] = pfnEv;
    
    return ERR_SUCCESS;
}

#else
PFN_DATA_CONN_CALLBACK      pfn_DataConn[8]  = {NULL,};
UINT32 CFW_SetDataConnFunEx ( PFN_DATA_CONN_CALLBACK pfnEv, UINT8 Cid )
{
    if( Cid > 7)
          return ERR_INVALID_PARAMETER;
    pfn_DataConn[Cid] = pfnEv;
    
    return ERR_SUCCESS;
}

#endif



UINT32 CFW_SetDataConnFun ( PFN_DATA_CONN_CALLBACK pfnEv )
{
#ifdef __ADP_MULTI_SIM__
   return CFW_SetDataConnFunEx(pfnEv,0,1);
#else
   return CFW_SetDataConnFunEx(pfnEv,1);

#endif
    
}

PFN_TCPIP_CALLBACK               pfn_TCPIPCB[5]     = {NULL,};
UINT32 CFW_SetTCPIPCallBackEx ( PFN_TCPIP_CALLBACK pfnEv, UINT8 SocketId )
{
		mmi_trace (1, "JDD_LOG: CFW_SetDataConnFunEx, socket ID: %d, callback: 0x%x",SocketId,pfnEv) ;

	if( SocketId > 4)
          return ERR_INVALID_PARAMETER;

	pfn_TCPIPCB[SocketId] = pfnEv;
    
    return ERR_SUCCESS;
}


UINT32 CFW_SetTCPIPCallBack ( PFN_TCPIP_CALLBACK pfnEv )
{
   return CFW_SetTCPIPCallBackEx(pfnEv,0);
    
}

#else
//Add by lixp for Jataayu issue at 20090120
PFN_DATA_CONN_CALLBACK      pfn_DataConn  = NULL;
PFN_TCPIP_CALLBACK               pfn_TCPIPCB     = NULL;

UINT32 CFW_SetDataConnFun ( PFN_DATA_CONN_CALLBACK pfnEv )
{
    pfn_DataConn = pfnEv;
    
    return ERR_SUCCESS;
}


UINT32 CFW_SetTCPIPCallBack ( PFN_TCPIP_CALLBACK pfnEv )
{
    pfn_TCPIPCB = pfnEv;
    
    return ERR_SUCCESS;
}
#endif

VOID Send_GPRS_Status_Ind(l4c_gprs_status_enum status)
{
	ilm_struct	ilm_ptr;

	mmi_ps_gprs_status_update_ind_struct *pGPRSStatusInd;
	
	pGPRSStatusInd = (mmi_ps_gprs_status_update_ind_struct*)OslMalloc(sizeof(mmi_ps_gprs_status_update_ind_struct));
	ASSERT(pGPRSStatusInd != NULL);
	memset(pGPRSStatusInd, 0, sizeof(mmi_ps_gprs_status_update_ind_struct));
	pGPRSStatusInd->status = status;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = ADP_WAP_MSG_SRC_MOD;	 
	ilm_ptr.local_para_ptr= (local_para_struct*)pGPRSStatusInd;
	ilm_ptr.peer_buff_ptr = NULL;
	ilm_ptr.msg_id = PRT_MMI_PS_GPRS_STATUS_UPDATE_IND;				 
	OslMsgSendExtQueue(&ilm_ptr); 

}

void soc_notify_GPRS_CONNECTED(UINT8 nResult)
{
	ilm_struct	ilm_ptr;
	mmi_ps_gprs_connect_status_ind_struct *pGprsConnetStatus;
		pGprsConnetStatus = (mmi_ps_gprs_connect_status_ind_struct*)OslMalloc(sizeof(mmi_ps_gprs_connect_status_ind_struct));
	ASSERT(pGprsConnetStatus != NULL);
	memset(pGprsConnetStatus, 0, sizeof(mmi_ps_gprs_connect_status_ind_struct));
	pGprsConnetStatus->status = nResult;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = ADP_WAP_MSG_SRC_MOD;	 
	ilm_ptr.local_para_ptr= (local_para_struct*)pGprsConnetStatus;
	ilm_ptr.peer_buff_ptr = NULL;
	ilm_ptr.msg_id = MSG_ID_MMI_NW_GPRS_CONNECTED_RSP;				 
	OslMsgSendExtQueue(&ilm_ptr); 
}

void soc_notify_GPRS_DECONNECTED(UINT8 nResult)
{
	ilm_struct	ilm_ptr;
	mmi_ps_gprs_deconnect_status_ind_struct *pGprsDeconnetStatus;
		pGprsDeconnetStatus = (mmi_ps_gprs_deconnect_status_ind_struct*)OslMalloc(sizeof(mmi_ps_gprs_deconnect_status_ind_struct));
	ASSERT(pGprsDeconnetStatus != NULL);
	memset(pGprsDeconnetStatus, 0, sizeof(mmi_ps_gprs_deconnect_status_ind_struct));
	pGprsDeconnetStatus->status = nResult;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = ADP_WAP_MSG_SRC_MOD;	 
	ilm_ptr.local_para_ptr= (local_para_struct*)pGprsDeconnetStatus;
	ilm_ptr.peer_buff_ptr = NULL;
	ilm_ptr.msg_id = MSG_ID_MMI_NW_GPRS_DECONNECTED_RSP;				 
	OslMsgSendExtQueue(&ilm_ptr); 


}
void soc_notify_APP(module_type module, int socketid, kal_uint8 event, kal_bool result, kal_int8 cause, kal_int32 detail_cause )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE Message;

    app_soc_notify_ind_struct *ind_ptr;

    ind_ptr = (app_soc_notify_ind_struct*) OslConstructDataPtr(sizeof(app_soc_notify_ind_struct));

    ind_ptr->socket_id = socketid;
    ind_ptr->event_type = event;
    ind_ptr->result = result;
    ind_ptr->error_cause = cause;
    ind_ptr->detail_cause = detail_cause;

    Message.oslSrcId = MOD_ADP;
    Message.oslDestId = module;
    Message.oslMsgId = MSG_ID_APP_SOC_NOTIFY_IND;

    Message.oslDataPtr = (oslParaType*) ind_ptr;
    Message.oslPeerBuffPtr = NULL;
    OslMsgSendExtQueue(&Message);

}

void soc_hostname_notify_ind(kal_bool result,
                 module_type mod_id,
                 kal_int32 request_id,
                 kal_uint8 access_id,
                 kal_uint8 *ip_addr,
                 kal_uint8 addr_len,
                 kal_int8 error_cause,
                 kal_int32 detail_cause)
{
    MYQUEUE Message;

    app_soc_get_host_by_name_ind_struct *ind_ptr;
  
    ind_ptr = (app_soc_get_host_by_name_ind_struct*)
               OslConstructDataPtr(sizeof(app_soc_get_host_by_name_ind_struct) );
    ind_ptr->result = result;

    if (result == KAL_TRUE)
    {
        ind_ptr->num_entry = addr_len/4;
        if (ind_ptr->num_entry > SOC_MAX_A_ENTRY)
        {
            ind_ptr->num_entry = SOC_MAX_A_ENTRY;
            addr_len = SOC_MAX_A_ENTRY * 4;
        }
        kal_mem_cpy(ind_ptr->entry, ip_addr, addr_len);
    }
    else    /* fail */
    {
        ind_ptr->num_entry = 0;
        kal_mem_set(ind_ptr->entry, 0, sizeof(ind_ptr->entry));
    }
    
    kal_mem_cpy(ind_ptr->addr, ip_addr, addr_len);
    ind_ptr->addr_len = addr_len;

    ind_ptr->request_id = request_id;
    ind_ptr->access_id = access_id;
    ind_ptr->error_cause = error_cause;
    ind_ptr->detail_cause = detail_cause;
	
    Message.oslSrcId = MOD_ADP;
    Message.oslDestId = mod_id;
    Message.oslMsgId = MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND;

    Message.oslDataPtr = (oslParaType*) ind_ptr;
    Message.oslPeerBuffPtr = NULL;
    OslMsgSendExtQueue(&Message);

}

void gprs_attach_stop_timer_soc()
{
	mmi_trace(g_sw_ADP_GPRS,"SOC: Func: gprs_attach_stop_timer_soc");
		StopTimer(GPRS_ATTACH_DELAY_TIMER);
}



UINT32  soc_StopGPRS()
{
	UINT32 rCode = 0;
	UINT8 nUTI = 0;

	CFW_GetFreeUTI(0,&nUTI);

#ifdef __ADP_MULTI_SIM__
	rCode = CFW_GprsAct (CFW_GPRS_DEACTIVED, g_nSocCid, nUTI, gprs_simId) ;
#else
	rCode = CFW_GprsAct (CFW_GPRS_DEACTIVED, g_nSocCid, nUTI) ;
#endif
	mmi_trace (g_sw_ADP_GPRS,"soc_StopGPRS:: nCid=%d,rCode=%d", g_nSocCid,rCode);
	
	return rCode;
}
#ifdef __ADP_MULTI_SIM__
extern void soc_setActiveSimAndCID(CFW_SIM_ID nSimID,UINT8 nCid );
#endif
INT32 soc_StartActiveGPRS( UINT8 apnsize, char *apnName)
{
	INT32 rCode = -1 ;
	CFW_GPRS_QOS stQos = {0,};
	CFW_GPRS_PDPCONT_INFO stPdp_cont = {0,};
	UINT32 rVal = 0 ;
	UINT8 State =0 ;
	UINT8 nUTI = 0;

	
#ifdef __ADP_MULTI_SIM__
	CFW_GetGprsActState( g_nSocCid, &State,gprs_simId) ;
#else
	CFW_GetGprsActState( g_nSocCid, &State) ;
#endif
	if(1 != State)
	{
		
		stQos.nDelay = 4;
		stQos.nMean = 16;
		stQos.nPeak = 4;
		stQos.nPrecedence = 3;
		stQos.nReliability = 3;
		
		
#ifdef __ADP_MULTI_SIM__
		CFW_GprsSetReqQos(g_nSocCid, &stQos,gprs_simId);
#else
		CFW_GprsSetReqQos(g_nSocCid, &stQos);
#endif

		stPdp_cont.nApnSize = apnsize ; //need change
		stPdp_cont.pApn = apnName; //"CMWAP" ;//need change
		mmi_trace(g_sw_ADP_GPRS,"SOC:APN val [%s], size [%d]", stPdp_cont.pApn, stPdp_cont.nApnSize);

		stPdp_cont.nPdpAddrSize = 0;
		stPdp_cont.pPdpAddr = NULL ;
		stPdp_cont.nDComp = 0;
		stPdp_cont.nHComp = 0;
		stPdp_cont.nPdpType = CFW_GPRS_PDP_TYPE_IP;
		stPdp_cont.nApnUserSize = 0;
		stPdp_cont.pApnUser = NULL;
		stPdp_cont.nApnPwdSize = 0;
		stPdp_cont.pApnPwd = NULL;	

		
#ifdef __ADP_MULTI_SIM__
		rVal = CFW_GprsSetPdpCxt(g_nSocCid, &stPdp_cont,gprs_simId);
#else
		rVal = CFW_GprsSetPdpCxt(g_nSocCid, &stPdp_cont);
#endif
		if(ERR_SUCCESS != rVal)
		{
			mmi_trace (g_sw_ADP_GPRS, "SOC: jdd_DataConnection StartGPRS: Error CFW_GprsSet [0x%x]", rVal) ;
			rCode = -1;
		}
		else
		{	
				CFW_GetFreeUTI(0,&nUTI);


				//incase attach response is success activate the pdp
#ifdef __ADP_MULTI_SIM__
				rVal = CFW_GprsAct( CFW_GPRS_ACTIVED, g_nSocCid, nUTI,gprs_simId);
				   CFW_SetDataConnFunEx (app_Data_Conn_Callback,gprs_simId,g_nSocCid);

#else
				rVal = CFW_GprsAct( CFW_GPRS_ACTIVED, g_nSocCid, nUTI);
				   CFW_SetDataConnFunEx (app_Data_Conn_Callback,g_nSocCid);

#endif
				rCode = rVal;

				mmi_trace (g_sw_ADP_GPRS, "SOC:  soc_StartActiveGPRS : GPRS ACTIVATE rVal [0x%x]", rVal) ;
		}
	}
	else
	{
		mmi_trace (g_sw_ADP_GPRS, "SOC:soc_StartActiveGPRS Data Connection wrong state should not occur State[%d]",State) ;
	}
	
	mmi_trace (g_sw_ADP_GPRS, "SOC: soc_StartActiveGPRS retruns:[%d]", rCode) ;		
	return rCode ;
}

BOOL app_Data_Conn_Callback (COS_EVENT *pCosEvent)
{
	UINT8				nType = HIUINT8 (pCosEvent->nParam3) ;
	UINT8				ConnectionId = 0 ;
	//JC_USER_INFO_CONNID *pstConnInfo ;
	//JC_RETCODE			rCode = JC_OK ;
	UINT8				nUTI ;
	UINT16				nreUTI ;
	BOOL				bIsProcessed = TRUE;
	UINT8			nSimID  = 0;
	//E_DATA_CONN_STATUS	eDataConnStatus = E_DATA_CONN_REGISTERING ;
#ifdef __ADP_MULTI_SIM__
	nSimID = LOUINT8( pCosEvent->nParam3 );
#endif
	nreUTI = HIUINT16(pCosEvent->nParam3);

	mmi_trace (g_sw_ADP_GPRS,"app_Data_Conn_Callback::  nEventId [%d], nType [0x%x], simID [%d], uti [%d]",
		pCosEvent->nEventId, nType, nSimID, nUTI) ;

	switch (pCosEvent->nEventId)
    {
	case EV_CFW_GPRS_ATT_RSP: /* Attach Response */
		{
			gprs_attach_stop_timer_soc();

			switch (nType)
			{
			case 0x00: /* Detach Success */
				{
					mmi_trace (g_sw_ADP_GPRS," app_Data_Conn_Callback:  Detach Success");
					Send_GPRS_Status_Ind (L4C_GPRS_DETACHED) ;
					break ;
				}
			case 0x01: /* Attach Success */
				{
						UINT32 rVal = 0;

						mmi_trace (g_sw_ADP_GPRS,"SOC: app_Data_Conn_Callback: Attach Success") ;
						#ifdef __ADP_MULTI_SIM__
						rVal = CFW_GetFreeCID(&g_nSocCid, gprs_simId);
						#else
						rVal = CFW_GetFreeCID(&g_nSocCid);
						#endif 
						mmi_trace (g_sw_ADP_GPRS,"SOC: app_Data_Conn_Callback cid given by platform[%d], rVal=0x%x", g_nSocCid,rVal);
						#if defined (MMI_ON_HARDWARE_P) &&defined(__ADP_MULTI_SIM__)
						soc_setActiveSimAndCID(gprs_simId,g_nSocCid);
						#endif
					      soc_StartActiveGPRS(g_apnsize,g_apnName);
						Send_GPRS_Status_Ind (L4C_GPRS_ATTACHED) ;


					break ;
				}
			
			case 0xF0: /* Detach Fail */
				{
					soc_notify_GPRS_DECONNECTED(1);
					mmi_trace (g_sw_ADP_GPRS, "app_Data_Conn_Callback:: Detach Failed") ;
					//eDataConnStatus = E_DATA_CONN_FAIL_GPRS ;					
					break ;
				}
			case 0xF1: /* Attach Fail */
				{
					//eDataConnStatus = E_DATA_CONN_FAIL_GPRS ;
					mmi_trace (g_sw_ADP_GPRS, "app_Data_Conn_Callback:: Attach Failed") ;
					soc_notify_GPRS_CONNECTED(2);

					break ;
				}
			default:
				mmi_trace (g_sw_ADP_GPRS, "app_Data_Conn_Callback:: EV_CFW_GPRS_ATT_RSP : Default Case") ;
				bIsProcessed = FALSE ;
				break;
			}		
		}
		break;		
	case EV_CFW_GPRS_ACT_RSP: /* Activation Response */
		{
			switch (nType)
			{
			case 0x00: /* Deactive Success */
				{
					mmi_trace (g_sw_ADP_GPRS, "app_Data_Conn_Callback:: Dectivation Success") ;
					ConnectionId  = (pCosEvent->nParam1) ;
					CFW_GetFreeUTI(0,&nUTI);
					Send_GPRS_Status_Ind(L4C_PDP_DEACTIVED);
					soc_notify_GPRS_DECONNECTED(0);
				 //     CFW_GprsAtt(CFW_GPRS_DETACHED, nUTI,gprs_simId);
					break ;
				}
			case 0x01: /* Active Success */
				{
					mmi_trace (g_sw_ADP_GPRS, "app_Data_Conn_Callback:: Activation Success[%d]") ;
					ConnectionId  = (pCosEvent->nParam1) ;
					//eDataConnStatus = E_DATA_CONN_CONNECTED_GPRS ;
					soc_notify_GPRS_CONNECTED(0);
					Send_GPRS_Status_Ind (L4C_PDP_ACTIVED) ;
					break ;
				}
			case 0xF0: /* Deactive Fail */
				{
					mmi_trace (g_sw_ADP_GPRS, "app_Data_Conn_Callback:: Deactivation Failed") ;
					ConnectionId  = (pCosEvent->nParam2) ;
					soc_notify_GPRS_DECONNECTED(2);
					//eDataConnStatus = E_DATA_CONN_FAIL_GPRS ;
					break ;
				}
			case 0xF1: /* Active Fail */
				{
					mmi_trace (g_sw_ADP_GPRS, " app_Data_Conn_Callback:: Activation Failed") ;
					ConnectionId  = (pCosEvent->nParam2) ;
					//eDataConnStatus = E_DATA_CONN_FAIL_GPRS ;
					soc_notify_GPRS_CONNECTED(1);
					break ;
				}
			default:
				{
					mmi_trace (g_sw_ADP_GPRS, " app_Data_Conn_Callback:: EV_CFW_GPRS_ACT_RSP : Default Case") ;
					ConnectionId  = (pCosEvent->nParam1) ;
					bIsProcessed = FALSE ;
				//	WEPTrueMain();
					break ;
				}
			}
		}
		break ;		
	case EV_CFW_GPRS_CXT_DEACTIVE_IND: /* Deactivation Indication */
		{
			ConnectionId  = (pCosEvent->nParam1) ;
			//eDataConnStatus = E_DATA_CONN_DISCONNECTED_GPRS ;
			mmi_trace (g_sw_ADP_GPRS, "app_Data_Conn_Callback, EV_CFW_GPRS_CXT_DEACTIVE_IND") ;
			Send_GPRS_Status_Ind(L4C_PDP_DEACTIVED);
			soc_notify_GPRS_CONNECTED(3);

		}
		break ;
	default:
		{
        	bIsProcessed = FALSE ;
			mmi_trace (g_sw_ADP_GPRS, "app_Data_Conn_Callback: : Default Not To be Processed Value \n");
			break ;
		}
    }
#if 0
	if (TRUE == bIsProcessed)
	{
		pstConnInfo = GetNodeWithConID (ConnectionId, nSimID) ;
		if (JC_NULL != pstConnInfo)
		{
			mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: Connection ID [%d], SIM ID [%d], Connection Node [%d], eDataConnStatus [%d], State [%d] and bIsCancelRequested [%d]",
				ConnectionId, nSimID, pstConnInfo, eDataConnStatus, pstConnInfo->eConState, pstConnInfo->bIsCancelRequested) ;
			if (E_DATA_CONN_CONNECTED == eDataConnStatus)
			{
				if (E_DATACONN_ATTACHING == pstConnInfo->eConState)
				{
					if(E_FALSE == pstConnInfo->bIsCancelRequested)
					{
				   		pstConnInfo->eConState = E_DATACONN_ACTIVATING ;
						nwap_uti = pstConnInfo->nUTI = FREE_UTI ;
						rCode = StartGPRS (pstConnInfo) ;
						if (rCode != JC_OK)
						{
							sendDCEventToTransport ((JC_UINT32)pstConnInfo, 0, E_DATA_CONN_FAIL_GPRS) ;
							DeleteChild (pstConnInfo) ;
						}
						Send_GPRS_Status_Ind (L4C_GPRS_ATTACHED) ;
						mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: Starting Activation with rCode [%X] and UTI [%d]",
							rCode, pstConnInfo->nUTI) ;
					}
					else
					{
						mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: DANGER: Attach Response Came, but jdd_DataConnCancelConnection () has already been called.") ;
					}
				}
				else
				{
					mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: DANGER: Attach Response Came, but Connection State is Different [%d]",
						pstConnInfo->eConState) ;
				}
			}
			else if (E_DATA_CONN_CONNECTED_GPRS == eDataConnStatus)
			{
				if(E_FALSE == pstConnInfo->bIsCancelRequested)
				{
					pstConnInfo->eConState = E_DATACONN_ACTIVATED ;
					Send_GPRS_Status_Ind (L4C_PDP_ACTIVED) ;
					sendDCEventToTransport ((JC_UINT32)pstConnInfo,0, E_DATA_CONN_CONNECTED_GPRS) ;
				}
				else
				{
					nwap_uti = pstConnInfo->nUTI = FREE_UTI ;
#ifdef __ADP_MULTI_SIM__
					rCode = CFW_GprsAct (CFW_GPRS_DEACTIVED, pstConnInfo->nCid, pstConnInfo->nUTI, pstConnInfo->nSimID) ;
#else
					rCode = CFW_GprsAct (CFW_GPRS_DEACTIVED, pstConnInfo->nCid, pstConnInfo->nUTI) ;
#endif
					mmi_trace (1,"JDD_LOG: jdd_Data_Conn_Callback: Starting De-activation with rCode [%X] and UTI [%d]",
									 rCode, pstConnInfo->nUTI) ;
				}
			}
			if (E_DATA_CONN_DISCONNECTED_GPRS == eDataConnStatus ||
					E_DATA_CONN_FAIL_GPRS == eDataConnStatus)
			{
				sendDCEventToTransport ((JC_UINT32)pstConnInfo,0, eDataConnStatus) ;
				DeleteChild (pstConnInfo) ;
				if (GetNodeWithSimID(nSimID)== NULL)
				{                    
					Send_GPRS_Status_Ind(L4C_PDP_DEACTIVED);
				}
			}
		}
		else
		{
			/*	If jdd_DataConnCancelConnection () is called with the state as E_DATACONN_ATTACHING,
				then this is valid case. Because the node is deleted inside jdd_DataConnCancelConnection ().*/
			mmi_trace (1, "JDD_LOG: jdd_Data_Conn_Callback: DANGER: Event To Be Handled But, Invalid Connection") ;
		}
	}
	#endif
    return bIsProcessed ;
}

BOOL app_SocketEventcb (COS_EVENT *pstCosEv)
{
            kal_uint8 addr[4];
            kal_uint8 addr_len;
	kal_char  *hostname;
       kal_int8      nsimID;
       kal_int8      ncid;

       int i;
	BOOL bIsProcessed = TRUE;

	mmi_trace(g_sw_ADP_GPRS," app_SocketEventcb :nEventId [%d] nParam1[%d] nParam2[%d] nParam3[%d]",pstCosEv->nEventId,
		pstCosEv->nParam1,pstCosEv->nParam2,pstCosEv->nParam3) ;

	
	switch (pstCosEv->nEventId)
	{
		case EV_CFW_TCPIP_SOCKET_CONNECT_RSP:
			mmi_trace(g_sw_ADP_GPRS," EV_CFW_TCPIP_SOCKET_CONNECT_RSP Func: %s ", __FUNCTION__);
			soc_notify_APP(MOD_MMI,pstCosEv->nParam1, SOC_CONNECT, TRUE, 0, 0);
			break ;
	
    	case EV_CFW_TCPIP_SOCKET_CLOSE_RSP:
			mmi_trace(g_sw_ADP_GPRS," EV_CFW_TCPIP_SOCKET_CLOSE_RSP Func: %s ", __FUNCTION__);
//			soc_notify_APP(MOD_MMI,pstCosEv->nParam1, SOC_CLOSE, TRUE, 0, 0);
			break ;
			
    	case EV_CFW_TCPIP_SOCKET_SEND_RSP:
			mmi_trace(g_sw_ADP_GPRS," EV_CFW_TCPIP_SOCKET_SEND_RSP Func: %s ", __FUNCTION__);
			soc_notify_APP(MOD_MMI,pstCosEv->nParam1, SOC_WRITE, TRUE, 0, 0);
			break ;
		case EV_CFW_TCPIP_REV_DATA_IND:
			mmi_trace(g_sw_ADP_GPRS," EV_CFW_TCPIP_REV_DATA_IND Func: %s %d", __FUNCTION__, pstCosEv->nParam2);
			soc_notify_APP(MOD_MMI,pstCosEv->nParam1, SOC_READ, TRUE, 0, 0);
			break ;
		case EV_CFW_TCPIP_ACCEPT_IND:
			mmi_trace(g_sw_ADP_GPRS," EV_CFW_TCPIP_ACCEPT_IND Func: %s ", __FUNCTION__);
			soc_notify_APP(MOD_MMI,pstCosEv->nParam1, SOC_ACCEPT, TRUE, 0, 0);
			break ;
		case EV_CFW_DNS_RESOLV_SUC_IND:
			mmi_trace(g_sw_ADP_GPRS," EV_CFW_DNS_RESOLV_SUC_IND Func: %s ", __FUNCTION__);
			addr[0] = ntohl(pstCosEv->nParam1) >>24&0xff;	
			addr[1] = ntohl(pstCosEv->nParam1) >>16&0xff;	
			addr[2] = ntohl(pstCosEv->nParam1) >>8&0xff;		
			addr[3] = ntohl(pstCosEv->nParam1)&0xff;	

			addr_len = 4;

			hostname =(kal_char *) (pstCosEv->nParam2);
			nsimID  = (pstCosEv->nParam2)>>24;
			ncid      = ((pstCosEv->nParam2)>>16)&0xff;
			
                      for(i=0; i<g_nRequestCount; i++)
                  	{
                  		if(memcmp(g_struRequest[g_nRequestCount].domain_name, hostname, strlen(hostname))) continue;
				if(g_struRequest[g_nRequestCount].nsimID != nsimID)	 continue;
				if(g_struRequest[g_nRequestCount].ncid == ncid)  break;
                  	}
			 if(i<g_nRequestCount)		  
			 {
				soc_hostname_notify_ind(TRUE, MOD_MMI,
			                 g_struRequest[i].request_id,g_struRequest[i].access_id, addr, addr_len, 0,0);

				if(i ==g_nRequestCount-1)
					g_nRequestCount--;
				else
				{
					memmove(&g_struRequest[i], &g_struRequest[i+1], (g_nRequestCount-i-1)*sizeof(HostName_RequestID_struct));
					g_nRequestCount--;	
				}

			 }
			 else
			soc_hostname_notify_ind(TRUE, MOD_MMI,0,0, addr, addr_len, 0,0);
			break ;
		case EV_CFW_DNS_RESOLV_ERR_IND:				 
			mmi_trace(g_sw_ADP_GPRS,"SOC: EV_CFW_DNS_RESOLV_ERR_IND Func: %s ", __FUNCTION__);
			hostname =(kal_char *) (pstCosEv->nParam2);
			nsimID  = (pstCosEv->nParam2)>>24;
			ncid      = ((pstCosEv->nParam2)>>16)&0xff;
			
                      for(i=0; i<g_nRequestCount; i++)
                  	{
                  		if(memcmp(g_struRequest[g_nRequestCount].domain_name, hostname, strlen(hostname))) continue;
				if(g_struRequest[g_nRequestCount].nsimID != nsimID)	 continue;
				if(g_struRequest[g_nRequestCount].ncid == ncid)  break;
                  	}
			 if(i<g_nRequestCount)		  
			 {
				soc_hostname_notify_ind(FALSE, MOD_MMI,
					g_struRequest[i].request_id,g_struRequest[i].access_id, 0, 0, ((pstCosEv->nParam3)>>8)&0xff,0);
				if(i ==g_nRequestCount-1)
					g_nRequestCount--;
				else
				{
					memmove(&g_struRequest[i], &g_struRequest[i+1], (g_nRequestCount-i-1)*sizeof(HostName_RequestID_struct));
					g_nRequestCount--;	
				}
			 }
			 else 
				soc_hostname_notify_ind(FALSE, MOD_MMI,	0,0, 0, 0, ((pstCosEv->nParam3)>>8)&0xff,0);
			break ;

//		case EV_CF_TCPIP_ERR_IND:
		case EV_CFW_TCPIP_ERR_IND:
			{
				mmi_trace(TRUE,"SOC: EV_CFW_TCPIP_ERR_IND Func: %s ", __FUNCTION__);
				if( CFW_ERR_TCPIP_CONN_TIMEOUT == pstCosEv->nParam2 )
				{
					soc_notify_APP(MOD_MMI,pstCosEv->nParam1, SOC_CONNECT, FALSE, 0, 0);
				}
				else
				{
					soc_notify_APP(MOD_MMI,pstCosEv->nParam1, SOC_ERROR_IND, 0, pstCosEv->nParam2, CFW_TcpipGetLastError());
				}
			}
			break ;

		case EV_CFW_TCPIP_CLOSE_IND:
			mmi_trace(g_sw_ADP_GPRS,"SOC: EV_CFW_TCPIP_CLOSE_IND Func: %s ", __FUNCTION__);
			soc_notify_APP(MOD_MMI,pstCosEv->nParam1, SOC_CLOSE, TRUE, 0, 0);
			break ;
			
		default:
			mmi_trace(g_sw_ADP_GPRS,"SOC: jdd_SocketEventcb : Unknown case") ;
			bIsProcessed = FALSE ;
			break ;
	}

	return bIsProcessed ;
		
}

void gprs_attach_TimerOut_callback()
{
	mmi_trace (g_sw_ADP_GPRS, "app_Data_Conn_Callback:: Attach Failed, Timer out") ;
	soc_notify_GPRS_CONNECTED(4);

}
UINT32 adp_PrepareGPRS(UINT32 arg_simid, UINT8 apnsize, char *apnName)
{
	#ifdef MMI_ON_HARDWARE_P
	
	UINT32 retVal = 0;
	UINT8   State = 0 ;
	UINT32  result =0;
	UINT8 nUTI=0x00;
	
	gprs_simId = arg_simid;
	CFW_SetDataConnFun (app_Data_Conn_Callback);//chenhe for kk,data conn cb

	CFW_SetTCPIPCallBack (app_SocketEventcb);//chenhe for kk

#ifdef __ADP_MULTI_SIM__
	result=CFW_GetGprsAttState (&State,gprs_simId) ;
#else
	result=CFW_GetGprsAttState (&State) ;
#endif

	CFW_GetFreeUTI(0,&nUTI);
	if(State == 1) //already attached
	{
			mmi_trace (g_sw_ADP_GPRS,"SOC: app_Data_Conn_Callback: aready attache") ;
		#ifdef __ADP_MULTI_SIM__
		 CFW_GetFreeCID(&g_nSocCid, gprs_simId);
		#else
		 CFW_GetFreeCID(&g_nSocCid);
		#endif 
#if defined(MMI_ON_HARDWARE_P) &&defined(__ADP_MULTI_SIM__)

		soc_setActiveSimAndCID(gprs_simId,g_nSocCid);
#endif
		retVal = soc_StartActiveGPRS(apnsize,apnName);
		mmi_trace(g_sw_ADP_GPRS,"adp_PrepareGPRS::  active gprs retVal is %x",retVal);

	}
	else
	{
		g_apnsize = apnsize;
		if(apnsize <16)
			memcpy(g_apnName, apnName,apnsize);
		else 
			return -1;

		#ifdef __ADP_MULTI_SIM__
	             CFW_SetDataConnFunEx (app_Data_Conn_Callback,(UINT8)gprs_simId,0);
		#else
	            CFW_SetDataConnFunEx (app_Data_Conn_Callback,0);
		#endif
	
		#ifdef __ADP_MULTI_SIM__
		if(gprs_simId!=CFW_SIM_END)
		{
			retVal=CFW_GprsAtt(CFW_GPRS_ATTACHED, nUTI,gprs_simId);
		}
		#else
			retVal=CFW_GprsAtt(CFW_GPRS_ATTACHED, nUTI);
		#endif
              StartTimer(GPRS_ATTACH_DELAY_TIMER, 2*60*1000, gprs_attach_TimerOut_callback);
			
	}
	mmi_trace(g_sw_ADP_GPRS,"adp_PrepareGPRS,retVal is %x,simId is %d",retVal,gprs_simId);
	//DisplayPopup( (PU8)GetString( STR_GLOBAL_PLEASE_WAIT ), IMG_GLOBAL_LOADING, 0, 0, (U8)SAVE_TONE );
//	mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
	//WEP_ShowWaitScreen();
	
/*******test temporarily,start****/
//WEPTrueMain();
/*******test temporarily,end****/
	return retVal;
#endif
}
//End for Jataayu issue at 20090120

static BOOL Get_CSD_DataAccount_Rsp(COS_EVENT *pCosEvent)
{
	ilm_struct *Message = NULL;
	ilm_struct	ilm_ptr;
	mmi_cc_get_csd_profile_req_struct *dataPtr = NULL;
	mmi_cc_get_csd_profile_rsp_struct *pCsdDataAccountRsp;

	INT32   ret = 0;
	UINT16 error = 0;
	nvram_ef_csd_profile_struct	*csd_account_total;

	csd_account_total = (nvram_ef_csd_profile_struct*)OslMalloc(sizeof(nvram_ef_csd_profile_struct)*MAX_DATA_ACCOUNT_GSM_LIMIT);
	MMI_ASSERT(csd_account_total != NULL);

	Message = (ilm_struct *)pCosEvent->nParam1;
	dataPtr = (mmi_cc_get_csd_profile_req_struct *)Message->local_para_ptr;

	pCsdDataAccountRsp = (mmi_cc_get_csd_profile_rsp_struct*)OslMalloc(sizeof(mmi_cc_get_csd_profile_rsp_struct));
	ASSERT(pCsdDataAccountRsp != NULL);
	memset(pCsdDataAccountRsp, 0, sizeof(mmi_cc_get_csd_profile_rsp_struct));
	
	ret = ReadRecord(NVRAM_EF_CSD_PROFILE_LID, 1, csd_account_total, sizeof(nvram_ef_csd_profile_struct) * MAX_DATA_ACCOUNT_GSM_LIMIT, &error);
	if (ret < 0)
	{ 
		OslMfree(csd_account_total);
		ASSERT(0);
		return FALSE;
	}
	
	pCsdDataAccountRsp->rate = CSMCC_ITC_3_1_K_AUDIO;
	pCsdDataAccountRsp->type = CSD_PROF_RATE_14400;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;	 
	ilm_ptr.local_para_ptr= (local_para_struct*)pCsdDataAccountRsp;
	ilm_ptr.peer_buff_ptr = NULL;
	ilm_ptr.msg_id = PRT_MMI_CC_GET_CSD_PROFILE_RSP;				 
	OslMsgSendExtQueue(&ilm_ptr); 

	OslMfree(csd_account_total);
	return TRUE;

}

static BOOL Set_CSD_DataAccount_Rsp(COS_EVENT *pCosEvent)
{
	ilm_struct *Message = NULL;
	ilm_struct	ilm_ptr;
	mmi_cc_set_csd_profile_req_struct *dataPtr = NULL;
	mmi_cc_set_csd_profile_rsp_struct *pCsdDataAccountRsp;
	
	INT32   ret = 0;
	UINT16 error = 0;
	nvram_ef_csd_profile_struct*	csd_account_total;
	
	csd_account_total = (nvram_ef_csd_profile_struct*)OslMalloc(sizeof(nvram_ef_csd_profile_struct)*MAX_DATA_ACCOUNT_GSM_LIMIT);
	MMI_ASSERT(csd_account_total != NULL);

	Message = (ilm_struct *)pCosEvent->nParam1;
	dataPtr = (mmi_cc_set_csd_profile_req_struct *)Message->local_para_ptr;

	ret = ReadRecord(NVRAM_EF_CSD_PROFILE_LID, 1, csd_account_total, sizeof(nvram_ef_csd_profile_struct) * MAX_DATA_ACCOUNT_GSM_LIMIT, &error);
	if (ret < 0)
	{ 
		OslMfree(csd_account_total);
		ASSERT(0);
		return FALSE;
	}

	ret = WriteRecord(NVRAM_EF_CSD_PROFILE_LID, 1, csd_account_total, sizeof(nvram_ef_csd_profile_struct) * MAX_DATA_ACCOUNT_GSM_LIMIT, &error);
	if (ret < 0)
	{ 
		OslMfree(csd_account_total);
		ASSERT(0);
		return FALSE;
	}
	
	pCsdDataAccountRsp = (mmi_cc_set_csd_profile_rsp_struct*)OslMalloc(sizeof(mmi_cc_set_csd_profile_rsp_struct));
	ASSERT(pCsdDataAccountRsp != NULL);
	memset(pCsdDataAccountRsp, 0, sizeof(mmi_cc_set_csd_profile_rsp_struct));	
	
	pCsdDataAccountRsp->result = TRUE;
	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;	 
	ilm_ptr.local_para_ptr= (local_para_struct*)pCsdDataAccountRsp;
	ilm_ptr.peer_buff_ptr = NULL;
	ilm_ptr.msg_id = PRT_MMI_CC_SET_CSD_PROFILE_RSP;				 
	OslMsgSendExtQueue(&ilm_ptr); 

	OslMfree(csd_account_total);
	return TRUE;

}


static BOOL Get_GPRS_DataAccount_Rsp(COS_EVENT *pCosEvent)
{
	ilm_struct *Message = NULL;
	ilm_struct	ilm_ptr;
	mmi_ps_get_gprs_data_account_req_struct *dataPtr = NULL;
	mmi_ps_get_gprs_data_account_rsp_struct *pGprsDataAccountRsp;

	INT32   ret = 0;
	UINT16 error = 0;
	l4c_gprs_account_info_struct	*gprs_account_total;

	gprs_account_total = (l4c_gprs_account_info_struct*)OslMalloc(sizeof(l4c_gprs_account_info_struct)*MAX_DATA_ACCOUNT_GPRS_LIMIT);
	MMI_ASSERT(gprs_account_total != NULL);

	Message = (ilm_struct *)pCosEvent->nParam1;
	dataPtr = (mmi_ps_get_gprs_data_account_req_struct *)Message->local_para_ptr;

	pGprsDataAccountRsp = (mmi_ps_get_gprs_data_account_rsp_struct*)OslMalloc(sizeof(mmi_ps_get_gprs_data_account_rsp_struct));
	ASSERT(pGprsDataAccountRsp != NULL);
	memset(pGprsDataAccountRsp, 0, sizeof(mmi_ps_get_gprs_data_account_rsp_struct));
	
	ret = ReadRecord(NVRAM_EF_ABM_GPRS_PROFILE_LID, 1, gprs_account_total, sizeof(l4c_gprs_account_info_struct) * MAX_DATA_ACCOUNT_GPRS_LIMIT, &error);
	if (ret < 0)
	{ 
		OslMfree(gprs_account_total);
		ASSERT(0);
		return FALSE;
	}
	
	memcpy(&pGprsDataAccountRsp->gprs_account,&gprs_account_total[dataPtr->profile_id - 1],sizeof(l4c_gprs_account_info_struct));

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;	 
	ilm_ptr.local_para_ptr= (local_para_struct*)pGprsDataAccountRsp;
	ilm_ptr.peer_buff_ptr = NULL;
	ilm_ptr.msg_id = PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_RSP;				 
	OslMsgSendExtQueue(&ilm_ptr); 

	OslMfree(gprs_account_total);
	return TRUE;

}

static BOOL Set_GPRS_DataAccount_Rsp(COS_EVENT *pCosEvent)
{
	ilm_struct *Message = NULL;
	ilm_struct	ilm_ptr;
	mmi_ps_set_gprs_data_account_req_struct *dataPtr = NULL;
	mmi_ps_set_gprs_data_account_rsp_struct *pGprsDataAccountRsp;
	
	INT32   ret = 0;
	UINT16 error = 0;
	l4c_gprs_account_info_struct*	gprs_account_total;

	gprs_account_total = (l4c_gprs_account_info_struct*)OslMalloc(sizeof(l4c_gprs_account_info_struct)*MAX_DATA_ACCOUNT_GPRS_LIMIT);
	MMI_ASSERT(gprs_account_total != NULL);

	Message = (ilm_struct *)pCosEvent->nParam1;
	dataPtr = (mmi_ps_set_gprs_data_account_req_struct *)Message->local_para_ptr;

	ret = ReadRecord(NVRAM_EF_ABM_GPRS_PROFILE_LID, 1, gprs_account_total, sizeof(l4c_gprs_account_info_struct) * MAX_DATA_ACCOUNT_GPRS_LIMIT, &error);
	if (ret < 0)
	{ 
		OslMfree(gprs_account_total);
		ASSERT(0);
		return FALSE;
	}

	memcpy(&gprs_account_total[dataPtr->gprs_account.context_id - 1],&dataPtr->gprs_account,sizeof(l4c_gprs_account_info_struct));
	
	ret = WriteRecord(NVRAM_EF_ABM_GPRS_PROFILE_LID, 1, gprs_account_total, sizeof(l4c_gprs_account_info_struct) * MAX_DATA_ACCOUNT_GPRS_LIMIT, &error);
	if (ret < 0)
	{ 
		OslMfree(gprs_account_total);
		ASSERT(0);
		return FALSE;
	}
	
	pGprsDataAccountRsp = (mmi_ps_set_gprs_data_account_rsp_struct*)OslMalloc(sizeof(mmi_ps_set_gprs_data_account_rsp_struct));
	ASSERT(pGprsDataAccountRsp != NULL);
	memset(pGprsDataAccountRsp, 0, sizeof(mmi_ps_set_gprs_data_account_rsp_struct));	
	
	pGprsDataAccountRsp->result = TRUE;
	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;	 
	ilm_ptr.local_para_ptr= (local_para_struct*)pGprsDataAccountRsp;
	ilm_ptr.peer_buff_ptr = NULL;
	ilm_ptr.msg_id = PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_RSP;				 
	OslMsgSendExtQueue(&ilm_ptr); 

	OslMfree(gprs_account_total);
	return TRUE;

}


static BOOL ADP_Get_GPRS_ConnectType_Rsp(COS_EVENT *pCosEvent)
{
	ilm_struct	ilm_ptr;
	mmi_nw_get_gprs_connect_type_rsp_struct *dataPtr = NULL;
	
	dataPtr = (mmi_nw_get_gprs_connect_type_rsp_struct*)OslMalloc(sizeof(mmi_nw_get_gprs_connect_type_rsp_struct));

	dataPtr->type = L4C_ALWAYS;
	dataPtr->result = MMI_TRUE;
	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;	 
	ilm_ptr.local_para_ptr= (local_para_struct*)dataPtr;
	ilm_ptr.peer_buff_ptr = NULL;
	ilm_ptr.msg_id = PRT_NW_GET_GPRS_CONNECT_TYPE_RSP;				 
	OslMsgSendExtQueue(&ilm_ptr); 

	return TRUE;

}

static BOOL ADP_Get_GPRS_Data_Counter_Rsp(COS_EVENT *pCosEvent)
{
	ilm_struct	ilm_ptr;
	mmi_ps_get_gprs_data_counter_rsp_struct *dataPtr = NULL;
	
	dataPtr = (mmi_ps_get_gprs_data_counter_rsp_struct*)OslMalloc(sizeof(mmi_ps_get_gprs_data_counter_rsp_struct));
#ifdef MMI_ON_HARDWARE_P
#ifdef __ADP_MULTI_SIM__
	U8 nSimID = 0;
	nSimID = adp_GetMMISimID(ADP_GET_MMI_DEST_MOD(pCosEvent->nParam3));
	CFW_GetGprsSum(
        (INT32*)&dataPtr->counter_info.total_tx_data, 
        (INT32*)&dataPtr->counter_info.total_rx_data, 
        nSimID);
#endif
    #endif
	dataPtr->result = MMI_TRUE;

	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;	 
	ilm_ptr.local_para_ptr= (local_para_struct*)dataPtr;
	ilm_ptr.peer_buff_ptr = NULL;
	ilm_ptr.msg_id = PRT_MMI_PS_GET_GPRS_DATA_COUNTER_RSP;				 
	OslMsgSendExtQueue(&ilm_ptr); 

	return TRUE;
}

static BOOL ADP_Get_GPRS_Reset_Data_Counter_Rsp(COS_EVENT *pCosEvent)
{
	ilm_struct	ilm_ptr;
	mmi_ps_reset_gprs_data_counter_rsp_struct *dataPtr = NULL;
	
	dataPtr = (mmi_ps_reset_gprs_data_counter_rsp_struct*)OslMalloc(sizeof(mmi_ps_reset_gprs_data_counter_rsp_struct));
    #ifdef MMI_ON_HARDWARE_P
	#ifdef __ADP_MULTI_SIM__
	U8 nSimID = 0;
	nSimID = adp_GetMMISimID(ADP_GET_MMI_DEST_MOD(pCosEvent->nParam3));
	CFW_ClearGprsSum(nSimID);
	#endif
    #endif

	dataPtr->result = MMI_TRUE;
	dataPtr->cause= MMI_TRUE;
	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;	 
	ilm_ptr.local_para_ptr= (local_para_struct*)dataPtr;
	ilm_ptr.peer_buff_ptr = NULL;
	ilm_ptr.msg_id = PRT_MMI_PS_RESET_GPRS_DATA_COUNTER_RSP;				 
	OslMsgSendExtQueue(&ilm_ptr); 

	return TRUE;
}

#if 0
static VOID SendGPRSEvent2MMI(COS_EVENT *pCosEvent)
{
	UINT8 nType = HIUINT8( pCosEvent->nParam3 );
#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_WAP, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif

	switch (pCosEvent->nEventId)
	{
		case EV_CFW_GPRS_ATT_RSP:
		{
			switch(nType)
			{
				case CFW_GPRS_DETACHED:
				{
					mmi_trace(g_sw_ADP_GPRS,"MMI_GPRS: EV_CFW_GPRS_ATT_RSP:detach success \n");
					Send_GPRS_Status_Ind(L4C_GPRS_DETACHED);
					break ;
				}
				case CFW_GPRS_ATTACHED:
				{
					mmi_trace(g_sw_ADP_GPRS,"MMI_GPRS: EV_CFW_GPRS_ATT_RSP: attach success \n");
					Send_GPRS_Status_Ind(L4C_GPRS_ATTACHED);
					break ;
				}
				case 0xF0:
				{
					mmi_trace(g_sw_ADP_GPRS,"MMI_GPRS: EV_CFW_GPRS_ATT_RSP: detach failed \n");
					break ;
				}
				case 0xF1:
				{
					mmi_trace (g_sw_ADP_GPRS,"MMI_GPRS: EV_CFW_GPRS_ATT_RSP: attach failed") ;
					break ;
				}
				default:
				{
					mmi_trace (g_sw_ADP_GPRS, "MMI_GPRS: EV_CFW_GPRS_ATT_RSP:Unknown value \n");
				}
			}		
		}
		break;
		
		case EV_CFW_GPRS_ACT_RSP:
		{
			if ( nType == 0xF0 )
			{
				mmi_trace (g_sw_ADP_GPRS, "MMI_GPRS: EV_CFW_GPRS_ACT_RSP dectived failed:  0x%x\n", pCosEvent->nParam1);
				Send_GPRS_Status_Ind(L4C_PDP_DEACTIVED);
			}
			if ( nType == 0xF1 )
			{
				mmi_trace (g_sw_ADP_GPRS, "MMI_GPRS: EV_CFW_GPRS_ACT_RSP  actived failed:  0x%x\n", pCosEvent->nParam1);
			}
			else if(nType == CFW_GPRS_ACTIVED)
			{
				mmi_trace (g_sw_ADP_GPRS, "MMI_GPRS: EV_CFW_GPRS_ACT_RSP actived success \n");
				Send_GPRS_Status_Ind(L4C_PDP_ACTIVED);
			}
			else if(nType == CFW_GPRS_DEACTIVED)
			{
				mmi_trace(g_sw_ADP_GPRS, "MMI_GPRS: EV_CFW_GPRS_ACT_RSP dectived success \n");
				Send_GPRS_Status_Ind(L4C_PDP_DEACTIVED);
			}
			else
			{
				mmi_trace (g_sw_ADP_GPRS, "MMI_GPRS: EV_CFW_GPRS_ACT_RSP Unknown value \n");
			}
		}
		break;
		case EV_CFW_GPRS_CXT_DEACTIVE_IND:
		{
				mmi_trace (g_sw_ADP_GPRS, "MMI_GPRS: EV_CFW_GPRS_CXT_DEACTIVE_IND  dectived success\n");
				Send_GPRS_Status_Ind(L4C_PDP_DEACTIVED);
		}
		break;
		case EV_CFW_GPRS_CXT_ACTIVE_RSP:
		{
			if(nType == CFW_GPRS_ACTIVED)
			{
				mmi_trace (g_sw_ADP_GPRS, "MMI_GPRS: EV_CFW_GPRS_CXT_ACTIVE_RSP actived success \n");
				Send_GPRS_Status_Ind(L4C_PDP_ACTIVED);
			}
			else if ( nType == 0xF1 )
			{
				mmi_trace (g_sw_ADP_GPRS, "MMI_GPRS: EV_CFW_GPRS_CXT_ACTIVE_RSP actived failed:  0x%x\n", pCosEvent->nParam1);
			}
			else
			{
				mmi_trace (g_sw_ADP_GPRS, "MMI_GPRS: EV_CFW_GPRS_CXT_ACTIVE_RSP Unknown value \n");
			}
		}
		break;

		default:
			break;
	}
		
}
#endif /* Guoj delete. It's not used now at 2009-11-17 */
#if 1
BOOL ADP_DispatchGPRSMsg(COS_EVENT *pCosEvent)
{
	UINT8 sid;
	ASSERT(pCosEvent != NULL);
	switch (pCosEvent->nEventId)
	{
		case PRT_MMI_CC_GET_CSD_PROFILE_REQ:
		{
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>PRT_MMI_CC_GET_CSD_PROFILE_REQ");
			Get_CSD_DataAccount_Rsp(pCosEvent);
			break;
		}
		
		case PRT_MMI_CC_SET_CSD_PROFILE_REQ:
		{
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>PRT_MMI_CC_SET_CSD_PROFILE_REQ");
			Set_CSD_DataAccount_Rsp(pCosEvent);
			break;
		}
		
		case PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_REQ:
		{
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_REQ");
			Get_GPRS_DataAccount_Rsp(pCosEvent);
			break;
		}

		case PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ:
		{
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ");
			Set_GPRS_DataAccount_Rsp(pCosEvent);
			break;
		}	
		case PRT_NW_GET_GPRS_CONNECT_TYPE_REQ:
		{
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>PRT_NW_GET_GPRS_CONNECT_TYPE_REQ");
			ADP_Get_GPRS_ConnectType_Rsp(pCosEvent);
			break;
		}
		case PRT_MMI_PS_GET_GPRS_DATA_COUNTER_REQ:
		{
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>PRT_MMI_PS_GET_GPRS_DATA_COUNTER_REQ");
			ADP_Get_GPRS_Data_Counter_Rsp(pCosEvent);
			break;
		}
 		case PRT_MMI_PS_RESET_GPRS_DATA_COUNTER_REQ:
		{
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>PRT_MMI_PS_RESET_GPRS_DATA_COUNTER_REQ");
			ADP_Get_GPRS_Reset_Data_Counter_Rsp(pCosEvent);
			break;
		}       
        
	      case EV_CFW_GPRS_ATT_RSP:
	      case EV_CFW_GPRS_STATUS_IND:
		  	#ifdef __ADP_MULTI_SIM__
			adp_ConfigureContext(ADP_MODULE_WAP, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));


			sid = adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3));
	            if( NULL != pfn_DataConn[sid][0] ) //jdd_Data_Conn_Callback
	            {
	                (*pfn_DataConn[sid][0])(pCosEvent);
	                break;
	            }  
	            else
	            {
	                return FALSE;
	            }
			#else
			if( NULL != pfn_DataConn[0] ) //jdd_Data_Conn_Callback
	            {
	                (*pfn_DataConn[0])(pCosEvent);
	                break;
	            }  
	            else
	            {
	                return FALSE;
	            }
			#endif

            //For GPRS message
            case EV_CFW_GPRS_CTX_MODIFY_ACC_RSP:
            case EV_CFW_GPRS_ACT_RSP:
            case EV_CFW_GPRS_CXT_ACTIVE_RSP:
            case EV_CFW_GPRS_MOD_RSP:
            case EV_CFW_GPRS_CXT_ACTIVE_IND:
            case EV_CFW_GPRS_CXT_DEACTIVE_IND:
            case EV_CFW_GPRS_MOD_IND:
            case EV_CFW_GPRS_DATA_IND:

//			SendGPRSEvent2MMI(pCosEvent);
#ifdef __ADP_MULTI_SIM__
		adp_ConfigureContext(ADP_MODULE_WAP, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
		sid = adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3));
		if( NULL != pfn_DataConn[sid][pCosEvent->nParam1] ) //jdd_Data_Conn_Callback
            {
                (*pfn_DataConn[sid][pCosEvent->nParam1])(pCosEvent);
                break;
            }  
            else
            {
                return FALSE;
            }
#else

            if( NULL != pfn_DataConn[pCosEvent->nParam1] ) //jdd_Data_Conn_Callback
            {
                (*pfn_DataConn[pCosEvent->nParam1])(pCosEvent);
                break;
            }  
            else
            {
                return FALSE;
            }
#endif			
            //For TCP message
            case EV_CFW_TCPIP_SOCKET_CONNECT_RSP:
            case EV_CFW_TCPIP_SOCKET_CLOSE_RSP:
            case EV_CFW_TCPIP_SOCKET_SEND_RSP:
            case EV_CFW_TCPIP_ERR_IND:
            case EV_CFW_TCPIP_ACCEPT_IND:
            case EV_CFW_TCPIP_CLOSE_IND:
            case EV_CFW_TCPIP_REV_DATA_IND:  
         
            if( NULL != pfn_TCPIPCB[pCosEvent->nParam1] ) //jdd_SocketEventcb
            {
                (*pfn_TCPIPCB[pCosEvent->nParam1])(pCosEvent);
                break;
            }    
            else
            {
                 return FALSE;
            }
		case EV_CFW_DNS_RESOLV_ERR_IND:
	      case EV_CFW_DNS_RESOLV_SUC_IND:
		if( NULL != pfn_TCPIPCB[4] ) //jdd_SocketEventcb
            {
                (*pfn_TCPIPCB[4])(pCosEvent);
                break;
            }    
            else
            {
                 return FALSE;
            }
		

            default:
                return FALSE;
	}
	
	return TRUE;

}




#else
BOOL ADP_DispatchGPRSMsg(COS_EVENT *pCosEvent)
{
	ASSERT(pCosEvent != NULL);

	switch (pCosEvent->nEventId)
	{
		case PRT_MMI_CC_GET_CSD_PROFILE_REQ:
		{
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>PRT_MMI_CC_GET_CSD_PROFILE_REQ");
			Get_CSD_DataAccount_Rsp(pCosEvent);
			break;
		}
		
		case PRT_MMI_CC_SET_CSD_PROFILE_REQ:
		{
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>PRT_MMI_CC_SET_CSD_PROFILE_REQ");
			Set_CSD_DataAccount_Rsp(pCosEvent);
			break;
		}
		
		case PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_REQ:
		{
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>PRT_MMI_PS_GET_GPRS_DATA_ACCOUNT_REQ");
			Get_GPRS_DataAccount_Rsp(pCosEvent);
			break;
		}

		case PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ:
		{
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>PRT_MMI_PS_SET_GPRS_DATA_ACCOUNT_REQ");
			Set_GPRS_DataAccount_Rsp(pCosEvent);
			break;
		}	
		case PRT_NW_GET_GPRS_CONNECT_TYPE_REQ:
		{
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>PRT_NW_GET_GPRS_CONNECT_TYPE_REQ");
			ADP_Get_GPRS_ConnectType_Rsp(pCosEvent);
			break;
		}
            //For GPRS message
            case EV_CFW_GPRS_CTX_MODIFY_ACC_RSP:
            case EV_CFW_GPRS_ATT_RSP:
            case EV_CFW_GPRS_ACT_RSP:
            case EV_CFW_GPRS_CXT_ACTIVE_RSP:
            case EV_CFW_GPRS_MOD_RSP:
            case EV_CFW_GPRS_CXT_ACTIVE_IND:
            case EV_CFW_GPRS_CXT_DEACTIVE_IND:
            case EV_CFW_GPRS_MOD_IND:
            case EV_CFW_GPRS_STATUS_IND:
            case EV_CFW_GPRS_DATA_IND:

//			SendGPRSEvent2MMI(pCosEvent);
#ifdef __ADP_MULTI_SIM__
			adp_ConfigureContext(ADP_MODULE_WAP, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif
			mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>ADP_DispatchGPRSMsg,ch,1111");

            if( NULL != pfn_DataConn ) //jdd_Data_Conn_Callback
            {
                (*pfn_DataConn)(pCosEvent);
                break;
            }  
            else
            {
                return FALSE;
            }
			
            //For TCP message
            case EV_CFW_TCPIP_SOCKET_CONNECT_RSP:
            case EV_CFW_TCPIP_SOCKET_CLOSE_RSP:
            case EV_CFW_TCPIP_SOCKET_SEND_RSP:
            case EV_CFW_TCPIP_ERR_IND:
            case EV_CFW_TCPIP_ACCEPT_IND:
            case EV_CFW_TCPIP_CLOSE_IND:
            case EV_CFW_TCPIP_REV_DATA_IND:  
            case EV_CFW_DNS_RESOLV_ERR_IND:
		    case EV_CFW_DNS_RESOLV_SUC_IND:
				mmi_trace(g_sw_ADP_GPRS,">>>>>>>>>>>>>>ADP_DispatchGPRSMsg,ch,2222");
		
            if( NULL != pfn_TCPIPCB ) //jdd_SocketEventcb
            {
                (*pfn_TCPIPCB)(pCosEvent);
                break;
            }    
            else
            {
                 return FALSE;
            }

            default:
                return FALSE;
	}
	
	return TRUE;

}
#endif
#endif//__GPRS_MODE__
