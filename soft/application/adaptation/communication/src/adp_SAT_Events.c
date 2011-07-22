/********************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               			*/
/*                            All Rights Reserved                             					*/
/********************************************************************************/
/* Filename:    adp_SAT_Events.c                                                   			*/
/*                                                                            						*/
/* Description:                                                               						*/
/*     change CSD message to coolsand message .                                  		*/
/*     Vivian 2006-10-10.                                                       					*/
/********************************************************************************/
#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"


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
#include "calllog_file_opration.h" 
#include "sat_def.h"
#include "satstruct.h"		
#include "pscallhelpergprot.h" 
#include "outgoingcallhelperprot.h" 
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

#ifdef __MMI_BT_SUPPORT__
#include "btmmiscrgprots.h"
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
#ifdef __ADP_MULTI_SIM__
#include "dual_sim.h"
#endif
#include "pscallhelperdef.h" 

#include "device.h"
#include "stack_msgs.h"
#include <cswtype.h>
#include "mmi_msg_struct.h"
#include "mmi_trace.h"

#include "unit_test.h"
#include "gsm7bitdefaultalphabet.h"

#define REFRESH     0x01 
#define MORE_TIME   0x02 
#define POLL_INTERVAL   0x03 
#define POLLING_OFF 0x04 
#define SET_UP_EVENT_LIST   0x05 
#define SET_UP_CALL     0x10 
#define SEND_SS     0x11 
#define SEND_USSD   0x12 
#define SEND_SMS  0x13 
#define SEND_DTMF   0x14 
#define PLAY_TONE   0x20 
#define DISPLAY_TEXT    0x21 
#define GET_INKEY   0x22 
#define GET_INPUT   0x23 
#define SELECT_ITEM     0x24 
#define SET_UP_MENU     0x25 
#define PROVIDE_LOCAL_INFORMATION   0x26 
#define TIMER_MANAGEMENT    0x27 
#define SET_UP_IDLE_MODEL_TEXT      0x28 
#define MENU_SELECT      0xd3

#define CMD_DETAILS_TAG 0x81
#define CMD_DETAILS_LEN  0X03
#define LAUNCH_BROWSER	    0x15	//nCmdType value of EV_CFW_SAT_CMDTYPE_IND 
#define MAX_TEXT_DISPLAY_LEN  256//240
static UINT8 *adp_SAT_get_Alpha_Id(UINT8 *pSrcString, UINT8 *pDec, UINT16 SrcStrlen, UINT8 *pDestStrlen);
typedef struct
	{	
		UINT8 nCmdType ;
		UINT8 nStatus ;
		UINT8 nItemId ;
	}ReSatRsp; 
ReSatRsp Rept2CSWstatMenuSel ;

#ifdef __ADP_MULTI_SIM__
adp_sat_context_t g_SATContext[ADP_SIM_NUMBER];
adp_sat_context_t *g_pSATContext = g_SATContext;
#else
adp_sat_context_t g_SATContext;
adp_sat_context_t *g_pSATContext = &g_SATContext;
#endif

BOOL bCellInfoForEM[ADP_SIM_NUMBER] = {FALSE,};

extern UINT8 gSSUTI;
extern SATCONTEXT gSATContext;
extern UINT8 gSSCode ;

VOID test_PLMNtoBCD(UINT8 *pIn, UINT8* pOut, UINT8* nInLength);
static BOOL ADP_DispatchSatReqMsg(COS_EVENT* pCosEvent);
static BOOL ADP_DispatchSatRspMsg(COS_EVENT* pCosEvent);
static BOOL ADP_DispatchSatIndMsg(COS_EVENT* pCosEvent);
void adp_SAT_SETUPMENU_ind(void);
static void adp_SAT_SETUPCALL_ind(void);
static void adp_SAT_SENDMESSAGE_rsp(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI);
static void adp_SAT_SET_CMD_Details(U8 *p_cmd_detail, U8 type_of_cmd, U8 cmd_qualifier);
static UINT8 *adp_SAT_get_Alpha_Id(UINT8 *pSrcString, UINT8 *pDec, UINT16 SrcStrlen, UINT8 *pDestStrlen);

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
extern UINT32 UT_CFW_SatActivation(UINT8 nMode,UINT16 nUTI ,UINT8 nSimId);
extern UINT32 UT_CFW_SatResponse(UINT8 nCmdType,UINT8 nStatus,UINT8 nItemId, VOID* pInputStr,UINT8 InputStrLen,UINT16 nUTI,UINT8 nSimId);
extern UINT32 UT_CFW_SatGetInformation( UINT8 nCmdType, VOID** CmdData ,UINT8 nSimId);
#endif
extern INT32  ADP_SAT_Text_Data_Decode(UINT8 *pDest, UINT8*pDestDec, UINT8 *pSrc,  UINT8 SrcDec, UINT16 nSrcLen);

/**************************************************************

	FUNCTION NAME		: ADP_DispatchSatMsg

  	PURPOSE				: Message Adaption for Sat 

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: NULL

	RETURNS				: TRUE->Message be processed
							  FALSE->Message not processed
	REMARKS				: 

**************************************************************/
BOOL ADP_DispatchSatMsg(COS_EVENT* pCosEvent)
{
	ASSERT(pCosEvent != NULL);
	
	if (ADP_DispatchSatReqMsg(pCosEvent))
		return TRUE;
	
	if (ADP_DispatchSatRspMsg(pCosEvent))
		return TRUE;
	
	if (ADP_DispatchSatIndMsg(pCosEvent))
		return TRUE;

	return FALSE;
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_SendMessageToMMI
	PURPOSE			    : Send message about SAT to CSD MMI task.
	INPUT PARAMETERS	: local_para_struct *msg_ptr
						: msg_type msg_id
	OUTPUT PARAMETERS   : NULL
	RETURN: NULL
**************************************************************/
static void adp_SAT_SendMessageToMMI(local_para_struct *msg_ptr, msg_type_t msg_id, module_type src_mod_id)
{
	//don't add ASSERT(msg_ptr != NULL) 
	
	ilm_struct  ilm_ptr;
	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.src_mod_id = src_mod_id;    
	ilm_ptr.local_para_ptr= (local_para_struct*)msg_ptr;
	ilm_ptr.peer_buff_ptr = NULL;
	ilm_ptr.msg_id = msg_id;               
	OslMsgSendExtQueue(&ilm_ptr);  	
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_SendNoFurtherCMDInd
	PURPOSE			    : Send message about SAT to CSD MMI task to main menu.
	INPUT PARAMETERS	: NULL
	OUTPUT PARAMETERS   : NULL
	RETURN: NULL
**************************************************************/
static void adp_SAT_SendNoFurtherCMDInd(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    adp_SAT_SendMessageToMMI((local_para_struct *)NULL, 
                                                        MSG_ID_MMI_SAT_NO_OTHER_CMD_IND, 
                                                        ADP_SAT_MSG_SRC_MOD);
    
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT adp_SAT_SendNoFurtherCMDInd <<<<MSG_ID_MMI_SAT_NO_OTHER_CMD_IND");
}

/*===========================================================
	FUNCTION NAME		: adp_SAT_INITATESPEECHCALL_rsp
	PURPOSE			    : process EV_CFW_CC_INITIATE_SPEECH_CALL_RSP event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN	            : NULL
===========================================================*/
static void adp_SAT_INITATESPEECHCALL_rsp(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(pCosEvent != NULL); 
#ifdef __ADP_MULTI_SIM__
    if ((gSATContext.isSATPresent[g_pSATContext->sim_id] != 0)
            && (g_pSATContext->gCurrSatReqId == PRT_MMI_SAT_CALL_SETUP_STAGE1_REQ))
#else
    if ((gSATContext.isSATPresent[0] != 0)
            && (g_pSATContext->gCurrSatReqId == PRT_MMI_SAT_CALL_SETUP_STAGE1_REQ))
#endif
    {
        mmi_sat_call_setup_stage1_rsp_struct *pMmiSatCallrsp1 =  
                    OslMalloc(sizeof(mmi_sat_call_setup_stage1_rsp_struct));

        ASSERT(pMmiSatCallrsp1 != NULL);
        memset(pMmiSatCallrsp1, 0, sizeof(mmi_sat_call_setup_stage1_rsp_struct));		

        pMmiSatCallrsp1->result.flag = 0x00;
        pMmiSatCallrsp1->result.cause = 0x00;
        pMmiSatCallrsp1->terminal_res = SAT_CMD_PERFORMED_SUCCESSFULLY;
        pMmiSatCallrsp1->additional_res = 0x00;
        
        adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSatCallrsp1, 
                                                            PRT_MMI_SAT_CALL_SETUP_STAGE1_RSP, ADP_SAT_MSG_SRC_MOD);
        
        g_pSATContext->gCurrSatReqId = 0;
    }
}

/*===========================================================
	FUNCTION NAME		: adp_SAT_INITATESPEECHCALL_rsp
	PURPOSE			    : process EV_CFW_CC_INITIATE_SPEECH_CALL_RSP event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN	            : NULL
===========================================================*/
static void adp_SAT_LOCAL_INFORMATION_rsp(COS_EVENT* pCosEvent)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	UINT8 nType  =  0;
	UINT8 nStatus = SAT_CMD_PERFORMED_SUCCESSFULLY;
	UINT16 nRet = 0;
	UINT8 i;
	BOOL bInfoForEm = FALSE;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	nType   = HIUINT8(pCosEvent->nParam3);
	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		if(bCellInfoForEM[i])
		{
			bInfoForEm = TRUE;
			break;
		}

	}
	if(bInfoForEm)
	{
		adp_SAT_SendMessageToMMI((local_para_struct *)NULL, PRT_MSG_ID_MMI_GET_CELL_INFO_RSP_IND, ADP_SAT_MSG_SRC_MOD);
		return;
	}
    
    if(nType == 0)
    {
        if(pCosEvent->nParam2 == CFW_TSM_CURRENT_CELL)
        {
            CFW_TSM_CURR_CELL_INFO pCurrCellInf = {0,};
            CFW_TSM_ALL_NEBCELL_INFO pNeighborCellInfo ={0,};
            SAT_LOCALINFO_RSP LocaInf;
            UINT8 nMNClen = 3;

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT	
            nRet = CFW_GetCellInfo(&pCurrCellInf,&pNeighborCellInfo);
#else
            nRet = CFW_GetCellInfo(&pCurrCellInf,&pNeighborCellInfo, g_pSATContext->sim_id);
#endif
#else
            nRet = CFW_GetCellInfo(&pCurrCellInf,&pNeighborCellInfo);
#endif

            if( ERR_SUCCESS != nRet)
            {
                mmi_trace(g_sw_ADP_SAT, "ADP_SAT LOCAL_INFO CFW_GetCellInfo ERROR!!!!!!!nRet=0x%x",nRet);
                return;
            }

            memset(&LocaInf, 0, sizeof(SAT_LOCALINFO_RSP));
            LocaInf.nCellIdentify = (pCurrCellInf.nTSM_CellID[0] << 8) | (pCurrCellInf.nTSM_CellID[1]);


            test_PLMNtoBCD(pCurrCellInf.nTSM_LAI, LocaInf.nMCC_MNC_Code, &nMNClen);
            LocaInf.nLocalAreaCode = (pCurrCellInf.nTSM_LAI[3]<<8 )|(pCurrCellInf.nTSM_LAI[4]);

            mmi_trace(g_sw_SAT, "ADP_SAT LOCAL_INFO RSP nCellIdentify = %x, nLocalAreaCode = %x"
                , LocaInf.nCellIdentify, LocaInf.nLocalAreaCode);

	#ifdef    __ADP_MULTI_SIM__
            if(g_pwr_context[g_pSATContext->sim_id].CurrentServiceType == NO_SERVICE)
	#else
		if(g_pwr_context[0].CurrentServiceType == NO_SERVICE)
	#endif
            {
                nStatus = SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD;
            }
	#ifdef    __ADP_MULTI_SIM__
            else if(g_pwr_context[g_pSATContext->sim_id].CurrentServiceType == LIMITED_SERVICE)
	#else
		else if(g_pwr_context[0].CurrentServiceType == LIMITED_SERVICE)
	#endif
            {
                nStatus = SAT_CMD_PERFORMED_LIMITED_SERVICE;
            }
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
            UT_CFW_SatResponse (PROVIDE_LOCAL_INFORMATION, nStatus, 0x0, &LocaInf, sizeof(SAT_LOCALINFO_RSP), FREE_UTI, g_pSATContext->sim_id);
#else
            CFW_SatResponse (PROVIDE_LOCAL_INFORMATION, nStatus, 0x0, &LocaInf, sizeof(SAT_LOCALINFO_RSP), FREE_UTI, g_pSATContext->sim_id);
#endif
#else
            CFW_SatResponse (PROVIDE_LOCAL_INFORMATION, nStatus, 0x0, &LocaInf, sizeof(SAT_LOCALINFO_RSP), FREE_UTI);
#endif								
            }
    }
    else if(nType == 0xF0)
    {
        if(pCosEvent->nParam1 == ERR_NO_MORE_MEMORY)
        {
            return;
        }
        else if(pCosEvent->nParam1 == ERR_CME_UNKNOWN)
        {
            return;
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
    
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT	
    nRet = CFW_EmodOutfieldTestEnd(FREE_UTI);
#else
    nRet = CFW_EmodOutfieldTestEnd(FREE_UTI, g_pSATContext->sim_id);
#endif
#else
    nRet = CFW_EmodOutfieldTestEnd(FREE_UTI);
#endif
    if( ERR_SUCCESS != nRet)
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT LOCAL_INFO CFW_EmodOutfieldTestEnd ERROR!!!!!!!nRet=0x%x",nRet);
        return;
    }
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_SENDMESSAGE_rsp
	PURPOSE			    : process EV_CFW_SMS_SEND_MESSAGE_RSP event
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
extern void CswError2CSDError(UINT16 *CSDError, UINT32 cswError);
static void adp_SAT_SENDMESSAGE_rsp(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{	
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __ADP_MULTI_SIM__
    if (g_pSATContext->gCurrSatReqId == PRT_MMI_SAT_SEND_SMS_STAGE1_REQ)
#else
    if (gSATContext.isSATPresent != 0
        && g_pSATContext->gCurrSatReqId == PRT_MMI_SAT_SEND_SMS_STAGE1_REQ)
#endif
    {
        mmi_sat_send_sms_stage1_rsp_struct* pMmiSendSmsRsp1 = 
            OslMalloc(sizeof(mmi_sat_send_sms_stage1_rsp_struct));

        ASSERT(pMmiSendSmsRsp1 != NULL);
        memset(pMmiSendSmsRsp1, 0, sizeof(mmi_sat_send_sms_stage1_rsp_struct));

        if (0 == nType)
        {
            pMmiSendSmsRsp1->res = 0x00;
            pMmiSendSmsRsp1->cause = 0x00;
        }
        else
        {
            pMmiSendSmsRsp1->res = 0xff;
            CswError2CSDError(&pMmiSendSmsRsp1->cause, nParam1);
        }

        mmi_trace(g_sw_ADP_SAT, "ADP_SAT SEND_SMS RSP  res = %d ,nParam1 = %d", pMmiSendSmsRsp1->res, nParam1);

        adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSendSmsRsp1, 
                                                                PRT_MMI_SAT_SEND_SMS_STAGE1_RSP, ADP_SAT_MSG_SRC_MOD);
        
        g_pSATContext->gCurrSatReqId = 0;

    }  
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_SENDUSSD_rsp
	PURPOSE			    : process EV_CFW_SMS_SEND_USSD_RSP event
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
mmi_sat_send_ussd_stage1_rsp_struct adp_sat_ussd_rsp;
void adp_SAT_SENDUSSD_rsp(UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT16 nUTI)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UINT8 nDec;
    UINT8 *pAdditioninfo = NULL;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_pSATContext->gCurrSatReqId == PRT_MMI_SAT_SEND_USSD_STAGE1_REQ)
    {
        mmi_sat_send_ussd_stage1_rsp_struct* pMmiSendUSSDRsp1 = 
        		OslMalloc(sizeof(mmi_sat_send_ussd_stage1_rsp_struct));
        CFW_SS_USSD_IND_INFO *pUSSDInd = NULL;

        pUSSDInd = (CFW_SS_USSD_IND_INFO *)nParam1;
        ASSERT(pMmiSendUSSDRsp1 != NULL);
        memset(pMmiSendUSSDRsp1, 0, sizeof(mmi_sat_send_ussd_stage1_rsp_struct));
        
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT SEND_USSD RSP, nType=%d", nType);	
        
        if (nType == 0 && pUSSDInd != NULL)//success
        {
            if(nParam2 == 2)
            {
                pMmiSendUSSDRsp1->res = SAT_USSD_SS_TRANSACTION_TERMINATION_BY_USER;		
            }
            else
            {
                mmi_trace(g_sw_ADP_SAT, "ADP_SAT SEND_USSD RSP, stringsize=%d, dec = %x", pUSSDInd->nStingSize, pUSSDInd->nDcs);	
                pMmiSendUSSDRsp1->res = SAT_CMD_PERFORMED_SUCCESSFULLY;
                pMmiSendUSSDRsp1->length = pUSSDInd->nStingSize + 2;
                
                pAdditioninfo = pMmiSendUSSDRsp1->addition_info;
                *pAdditioninfo = 0x11;
                pAdditioninfo++;

                ADP_USSDParseDCS(pUSSDInd->nDcs, &nDec);

                if (nDec == ADP_MSG_GSM_7 || nDec == ADP_MSG_RESERVED) 
                {
                    UINT8 nDecodeResultLen = 0x00;
                    *pAdditioninfo = MMI_8BIT_DCS;
                    pAdditioninfo++;
                    nDecodeResultLen = SUL_Decode7Bit(pUSSDInd->pUsdString, pAdditioninfo, pUSSDInd->nStingSize);
                    pMmiSendUSSDRsp1->length = 2 + nDecodeResultLen;
                }
                else if (nDec == ADP_MSG_8_BIT_DATA)
                {
                    *pAdditioninfo = MMI_8BIT_DCS;
                    pAdditioninfo++;
                    memcpy(pAdditioninfo, pUSSDInd->pUsdString, pUSSDInd->nStingSize);	
                }
                else if (nDec == ADP_MSG_UCS2)
                {
                    *pAdditioninfo = MMI_8BIT_DCS;  //ascii coding  

                    if(pUSSDInd->pUsdString[0] == 0x81)
                    {
                        *pAdditioninfo = MMI_UCS2_81;
                    }
                    else if(pUSSDInd->pUsdString[0] == 0x82)
                    {
                        *pAdditioninfo = MMI_UCS2_82;
                    }
                    pAdditioninfo++;
                    memcpy(pAdditioninfo, pUSSDInd->pUsdString, pUSSDInd->nStingSize);	
                }

            }
        }
	else if(nType != 0)
        {
            if(nType == 0xFB || nType == 0xFD)
            {
                pMmiSendUSSDRsp1->res = SAT_NET_CURRENTLY_UNABLE_TO_PROCESS_CMD;
            }
            else if(nType == 0xFC)
            {
                pMmiSendUSSDRsp1->res = SAT_USSD_RETURN_ERROR;
            }
        }
        else
        {
            pMmiSendUSSDRsp1->res = SAT_USSD_SS_TRANSACTION_TERMINATION_BY_USER;
        }

        memset(&adp_sat_ussd_rsp, 0x00, sizeof(mmi_sat_send_ussd_stage1_rsp_struct));

        if(nType == 0 && pUSSDInd != NULL)
        {
            adp_sat_ussd_rsp.length = pUSSDInd->nStingSize + 1;
            adp_sat_ussd_rsp.addition_info[0] = pUSSDInd->nDcs;
            memcpy(adp_sat_ussd_rsp.addition_info + 1, pUSSDInd->pUsdString, pUSSDInd->nStingSize);
        }
        
        adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSendUSSDRsp1, 
                                                            PRT_MMI_SAT_SEND_USSD_STAGE1_RSP, ADP_SAT_MSG_SRC_MOD);

        g_pSATContext->gCurrSatReqId = 0;
    }
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_proc_RESPONSE_rsp
	PURPOSE			    : process EV_CFW_SAT_RESPONSE_RSP event
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_proc_RESPONSE_rsp(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UINT8 nType  =  0;
    UINT32 nParam1 = 0;
    UINT32 nParam2 = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    ASSERT(pCosEvent != NULL);

    nParam1 = pCosEvent->nParam1;
    nParam2 = pCosEvent->nParam2;
    nType   = HIUINT8(pCosEvent->nParam3);

    TBM_ENTRY(0x29f0);

#ifdef __ADP_MULTI_SIM__
    mmi_trace(g_sw_SAT, "ADP_SAT gCurrSatReqId = %d",  g_pSATContext->gCurrSatReqId);
#endif /* __ADP_MULTI_SIM__ */
	
    switch (nParam1)
    {
        case MENU_SELECT:
        {
            mmi_sat_menu_select_rsp_struct *pmenu_select_rsp = 
                OslMalloc(sizeof(mmi_sat_menu_select_rsp_struct));

            ASSERT(pmenu_select_rsp != NULL);
            memset(pmenu_select_rsp, 0, sizeof(mmi_sat_menu_select_rsp_struct));	

            pmenu_select_rsp->result = nType;

            adp_SAT_SendMessageToMMI((local_para_struct *)pmenu_select_rsp, 
            				                                PRT_MMI_SAT_MENU_SELECT_RSP, ADP_SAT_MSG_SRC_MOD);
            
            g_pSATContext->gCurrSatReqId = 0;

            break;
        }
        case SELECT_ITEM:
        {
            mmi_sat_select_item_res_rsp_struct *psat_selectitem_rsp = 
                OslMalloc(sizeof(mmi_sat_select_item_res_rsp_struct));

            ASSERT(psat_selectitem_rsp != NULL);

            memset(psat_selectitem_rsp, 0, sizeof(mmi_sat_select_item_res_rsp_struct));
            psat_selectitem_rsp->result = nType;
		#ifdef __MMI_MULTI_SIM__
            psat_selectitem_rsp->sim_id = g_pSATContext->sim_id;
		#endif
            
            adp_SAT_SendMessageToMMI((local_para_struct *)psat_selectitem_rsp, 
                                                                PRT_MMI_SAT_SELECT_ITEM_RES_RSP, ADP_SAT_MSG_SRC_MOD);
            
            g_pSATContext->gCurrSatReqId = 0;

            break;
        }
        case SET_UP_MENU:
        {
            mmi_sat_setup_menu_res_rsp_struct *psat_menusetup_rsp = 
                OslMalloc(sizeof(mmi_sat_setup_menu_res_rsp_struct));

            ASSERT(psat_menusetup_rsp != NULL);
            memset(psat_menusetup_rsp, 0, sizeof(mmi_sat_setup_menu_res_rsp_struct));
            
            psat_menusetup_rsp->result = nType;

            adp_SAT_SendMessageToMMI((local_para_struct *)psat_menusetup_rsp, 
                                                                    PRT_MMI_SAT_SETUP_MENU_RES_RSP, ADP_SAT_MSG_SRC_MOD);
            
            g_pSATContext->gCurrSatReqId = 0;

            break;
        }
        case DISPLAY_TEXT:
        {
            mmi_sat_display_text_res_rsp_struct*pMmiDisplayTextRsp =  
                OslMalloc(sizeof(mmi_sat_display_text_res_rsp_struct));

            ASSERT(pMmiDisplayTextRsp != NULL);
            memset(pMmiDisplayTextRsp, 0, sizeof(mmi_sat_display_text_res_rsp_struct));

            pMmiDisplayTextRsp->result = nType;
            #ifdef __MMI_MULTI_SIM__
            pMmiDisplayTextRsp->sim_id = g_pSATContext->sim_id;
            #endif

            adp_SAT_SendMessageToMMI((local_para_struct *)pMmiDisplayTextRsp, 
                                                                    PRT_MMI_SAT_DISPLAY_TEXT_RES_RSP, ADP_SAT_MSG_SRC_MOD);
            
            g_pSATContext->gCurrSatReqId = 0;

            break;
        }
        case GET_INPUT:
        {
            mmi_sat_get_input_res_rsp_struct *pMmiGetInputRsp = 
                OslMalloc(sizeof(mmi_sat_get_input_res_rsp_struct));

            ASSERT(pMmiGetInputRsp != NULL);
            memset(pMmiGetInputRsp, 0, sizeof(mmi_sat_get_input_res_rsp_struct));

            pMmiGetInputRsp->result = nType;

            adp_SAT_SendMessageToMMI((local_para_struct *)pMmiGetInputRsp, 
                                                                        PRT_MMI_SAT_GET_INPUT_RES_RSP, ADP_SAT_MSG_SRC_MOD);
            
            g_pSATContext->gCurrSatReqId = 0;

            break;
        }
        case SET_UP_CALL:
        {
            mmi_sat_call_setup_stage2_rsp_struct *pMmiSatCallrsp2 = 
            OslMalloc(sizeof(mmi_sat_call_setup_stage2_rsp_struct));

            ASSERT(pMmiSatCallrsp2 != NULL);
            memset(pMmiSatCallrsp2, 0, sizeof(mmi_sat_call_setup_stage2_rsp_struct));	

            pMmiSatCallrsp2->result = nType;

            mmi_trace(g_sw_ADP_SAT, "ADP_SAT pMmiSatCallrsp2->res = %d", pMmiSatCallrsp2->result);
            adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSatCallrsp2, 
            PRT_MMI_SAT_CALL_SETUP_STAGE2_RSP, ADP_SAT_MSG_SRC_MOD);
            g_pSATContext->gCurrSatReqId = 0;

            break;
        }
        case SEND_SMS:
        {
            mmi_sat_send_sms_stage2_rsp_struct*pMmiSendSmsRsp2 = 
                OslMalloc(sizeof(mmi_sat_send_sms_stage2_rsp_struct));

            ASSERT(pMmiSendSmsRsp2 != NULL);
            memset(pMmiSendSmsRsp2, 0, sizeof(mmi_sat_send_sms_stage2_rsp_struct));	

            pMmiSendSmsRsp2->result = nType;

            mmi_trace(g_sw_ADP_SAT, "ADP_SAT pMmiSendSmsRsp2->res = %d", pMmiSendSmsRsp2->result);

            adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSendSmsRsp2, 
                                                                      PRT_MMI_SAT_SEND_SMS_STAGE2_RSP, ADP_SAT_MSG_SRC_MOD);
            g_pSATContext->gCurrSatReqId = 0;
            break;
        }
        case SEND_SS:
        {
            mmi_sat_send_ss_stage2_rsp_struct *pMmiSendSsRsp2 = 
                OslMalloc(sizeof(mmi_sat_send_ss_stage2_rsp_struct));

            pMmiSendSsRsp2->result = nType;
            adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSendSsRsp2, 
                                                                    PRT_MMI_SAT_SEND_SS_STAGE2_RSP, ADP_SAT_MSG_SRC_MOD);
            
            g_pSATContext->gCurrSatReqId = 0;

            break;
        }
        case SEND_USSD:
        {
            mmi_sat_send_ussd_stage2_rsp_struct *pMmiSendUSSDRsp2 = 
                OslMalloc(sizeof(mmi_sat_send_ussd_stage2_rsp_struct));

            pMmiSendUSSDRsp2->result = nType;
            
            adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSendUSSDRsp2, 
                                                                    PRT_MMI_SAT_SEND_USSD_STAGE2_RSP, ADP_SAT_MSG_SRC_MOD);
            g_pSATContext->gCurrSatReqId = 0;

            break;
        }
		case LAUNCH_BROWSER:
		{
			mmi_sat_launch_browser_res_rsp_struct *pMmiLaunchBrowserRsp =  OslMalloc(sizeof(mmi_sat_launch_browser_res_rsp_struct));

			mmi_trace(g_sw_SAT, "SAT <ADP RSP BEG>  PRT_MMI_SAT_LAUNCH_BROWSER_RES_REQ");
			ASSERT(pMmiLaunchBrowserRsp != NULL);
			
			memset(pMmiLaunchBrowserRsp, 0, sizeof(mmi_sat_launch_browser_res_rsp_struct));

			pMmiLaunchBrowserRsp->result = nType;
			
			adp_SAT_SendMessageToMMI((local_para_struct *)pMmiLaunchBrowserRsp, 
										PRT_MMI_SAT_LAUNCH_BROWSER_RES_RSP, ADP_SAT_MSG_SRC_MOD);
			g_pSATContext->gCurrSatReqId = 0;
			mmi_trace(g_sw_SAT, "SAT <ADP RSP END>  PRT_MMI_SAT_LAUNCH_BROWSER_RES_REQ");

			break;
		}
        default:
        {
            mmi_trace(g_sw_SAT, "ADP_SAT g_pSATContext->gCurrSatReqId = %d,", g_pSATContext->gCurrSatReqId);
            break;
        }
    }
    
    if (nParam2 == 0x90)
    {
        mmi_trace(g_sw_SAT, "ADP_SAT g_pSATContext->gCurrSatReqId = %d,", g_pSATContext->gCurrSatReqId);
        adp_SAT_SendNoFurtherCMDInd();
    }   
    
    TBM_EXIT(0x29f0);
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_CALLSETUPSTAGE2_req
	PURPOSE			    : process PRT_MMI_SAT_SEND_SMS_STAGE2_REQ event
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_CALLSETUPSTAGE2_req(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *oslMessage;
    UINT32 result;
    kal_uint8 nStatus;
    kal_uint8 comdnType;
    mmi_sat_call_setup_stage2_req_struct*pMmiCallSetUpReq2;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/	
    ASSERT(pCosEvent != NULL);
    oslMessage = (ilm_struct *)pCosEvent->nParam1;
    pMmiCallSetUpReq2 = (mmi_sat_call_setup_stage2_req_struct*)oslMessage->local_para_ptr;

    nStatus = pMmiCallSetUpReq2->res ;
    comdnType = pMmiCallSetUpReq2->cmd_detail[3] ;

    TRACE_EF(g_sw_SAT,"ADP_SAT comdnType=%x,nStatus=%x", comdnType, nStatus);

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    result = UT_CFW_SatResponse (comdnType, nStatus, 0, NULL, 0x00, FREE_UTI, g_pSATContext->sim_id);
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, NULL, 0x00, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, NULL, 0x00, FREE_UTI);
#endif

    if (result == ERR_SUCCESS)
    {
        g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_CALL_SETUP_STAGE2_REQ;
    }
    else
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT FUNC:%s CFW_SatResponse Error !!!!!!!!!!!!!!!", __FUNCTION__);    
    }
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_CALLSETUPSTAGE1_req
	PURPOSE			    : process PRT_MMI_SAT_CALL_SETUP_STAGE1_REQ event
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
extern UINT32 UT_CFW_CcInitiateSpeechCall (
  CFW_DIALNUMBER* pDialNumber,
  UINT16 nUTI,
  UINT8 nSimId
);
extern void Adp_SendCCDailIndMessage(UINT8 CallIndex, module_type DestMode);

static void adp_SAT_Proc_CALLSETUPSTAGE1_req(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *oslMessage;
    UINT8 CallIndex;
    UINT32 result = 0x00;			
    CFW_DIALNUMBER  DialNumber;
    mmi_sat_call_setup_stage1_req_struct* pMmiCallSetUpReq1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/		
    ASSERT(pCosEvent != NULL);
    oslMessage = (ilm_struct *)pCosEvent->nParam1;
    pMmiCallSetUpReq1 = (mmi_sat_call_setup_stage1_req_struct*)oslMessage->local_para_ptr;

    DialNumber.nDialNumberSize = pMmiCallSetUpReq1->no_addr;

    DialNumber.nType = CFW_TELNUMBER_TYPE_UNKNOWN; //pMmiCallSetUpReq1->sat_call_type;
    DialNumber.nClir = 0;
    
    DialNumber.pDialNumber = (UINT8*)OslMalloc(DialNumber.nDialNumberSize);
	
    ASSERT(DialNumber.pDialNumber != NULL);
    memset(DialNumber.pDialNumber, 0, DialNumber.nDialNumberSize);
    memcpy(DialNumber.pDialNumber, pMmiCallSetUpReq1->addr, pMmiCallSetUpReq1->no_addr);

    mmi_trace(g_sw_ADP_SAT, "ADP_SAT pMmiCallSetUpReq1->no_addr =0x%x", pMmiCallSetUpReq1->no_addr);
#ifdef SAT_TEST_SWITCH
    {
        UINT8 i;
        for (i = 0; i < pMmiCallSetUpReq1->no_addr; i++)
        {
            mmi_trace(g_sw_ADP_SAT, "ADP_SAT pMmiCallSetUpReq1->addr[%d] = 0x%x", i, pMmiCallSetUpReq1->addr[i]);
        }
    }
#endif

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    result = CFW_CcInitiateSpeechCallEx (&DialNumber, &CallIndex, FREE_UTI);
#else
    result = CFW_CcInitiateSpeechCallEx (&DialNumber, &CallIndex, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
    result = CFW_CcInitiateSpeechCallEx (&DialNumber, &CallIndex, FREE_UTI);
#endif

    if (result == ERR_SUCCESS)
    {
        g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_CALL_SETUP_STAGE1_REQ;
        Adp_SendCCDailIndMessage(CallIndex, ADP_SAT_MSG_SRC_MOD);
    }
    else
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT FUNC:%s CFW_CcInitiateSpeechCallEx Error !!!!!!!!!!!!!!!", __FUNCTION__);    
    }

    OslMfree(DialNumber.pDialNumber); 
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_SENDSMSSTAGE2_req
	PURPOSE			    : process PRT_MMI_SAT_SEND_SMS_STAGE2_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_SENDSMSSTAGE2_req(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *oslMessage;
    UINT32 result;
    kal_uint8 nStatus;
    kal_uint8 comdnType;
    mmi_sat_send_sms_stage2_req_struct* pMmiSendSmsReq2;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(pCosEvent != NULL);
    oslMessage = (ilm_struct *)pCosEvent->nParam1;
    pMmiSendSmsReq2 = (mmi_sat_send_sms_stage2_req_struct*)oslMessage->local_para_ptr;

    nStatus = pMmiSendSmsReq2->res ;
    comdnType = pMmiSendSmsReq2->cmd_detail[3] ;
    
    if(nStatus != 0)
    {
        nStatus = 0x35;
    }

    mmi_trace(g_sw_ADP_SAT, "ADP_SAT SEND_SMS REQ nStatus =%d, comdnType=0x%x", nStatus, comdnType);

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    result = UT_CFW_SatResponse (comdnType, nStatus, 0, NULL, 0x00, FREE_UTI, g_pSATContext->sim_id);
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, NULL, 0x00, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, NULL, 0x00, FREE_UTI);
#endif	

    if (result == ERR_SUCCESS)
    {
        g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_SEND_SMS_STAGE2_REQ;
    }
    else
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT FUNC:%s CFW_SatResponse Error !!!!!!!!!!!!!!!", __FUNCTION__);            
    }
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_SENDSSTAGE2_req
	PURPOSE			    : process PRT_MMI_SAT_SEND_SS_STAGE2_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_SENDSSTAGE2_req(COS_EVENT* pCosEvent)
{
    ilm_struct *oslMessage;
    UINT32 result;
    kal_uint8 nStatus;
    kal_uint8 comdnType;
    mmi_sat_send_ss_stage2_req_struct* pMmiSendSSReq2;

    ASSERT(pCosEvent != NULL);
    oslMessage = (ilm_struct *)pCosEvent->nParam1;
    pMmiSendSSReq2 = (mmi_sat_send_ss_stage2_req_struct*)oslMessage->local_para_ptr;

    nStatus = pMmiSendSSReq2->res ;
    comdnType = pMmiSendSSReq2->cmd_detail[3] ;
    if(nStatus != 0)
    {
    nStatus = 0x35;
    }

    mmi_trace(g_sw_ADP_SAT, "ADP_SAT nStatus =%d, comdnType=0x%x", nStatus, comdnType);

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    result = UT_CFW_SatResponse (comdnType, nStatus, 0, NULL, 0x00, FREE_UTI, g_pSATContext->sim_id);
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, pMmiSendSSReq2->additional_info, pMmiSendSSReq2->size_of_additional_info, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, NULL, 0x00, FREE_UTI);
#endif	

    if (result == ERR_SUCCESS)
    {
        g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_SEND_SS_STAGE2_REQ;
    }
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_SENDUSSDTAGE2_req
	PURPOSE			    : process PRT_MMI_SAT_SEND_USSD_STAGE2_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_SENDUSSDTAGE2_req(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *oslMessage;
    UINT32 result;
    kal_uint8 nStatus;
    kal_uint8 comdnType;
    mmi_sat_send_ussd_stage2_req_struct* pMmiSendUSSDReq2;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/	
    
    ASSERT(pCosEvent != NULL);
    oslMessage = (ilm_struct *)pCosEvent->nParam1;
    pMmiSendUSSDReq2 = (mmi_sat_send_ussd_stage2_req_struct*)oslMessage->local_para_ptr;

    nStatus = pMmiSendUSSDReq2->res ;
    comdnType = pMmiSendUSSDReq2->cmd_detail[3] ;

    mmi_trace(g_sw_ADP_SAT, "ADP_SAT nStatus =%d, comdnType=0x%x", nStatus, comdnType);

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    result = UT_CFW_SatResponse (comdnType, nStatus, 0, NULL, 0x00, FREE_UTI, g_pSATContext->sim_id);
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, adp_sat_ussd_rsp.addition_info, adp_sat_ussd_rsp.length, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, NULL, 0x00, FREE_UTI);
#endif	

    if (result == ERR_SUCCESS)
    {
        g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_SEND_USSD_STAGE2_REQ;
    }
    else
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT FUNC:%s CFW_SatResponse Error !!!!!!!!!!!!!!!", __FUNCTION__);            
    }	
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_SENDSMSSTAGE1_req
	PURPOSE			    : process PRT_MMI_SAT_SEND_SMS_STAGE1_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_SENDSMSSTAGE1_req(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *oslMessage;
    mmi_sat_send_sms_stage1_req_struct *pMmiSendSmsReq1;
    UINT32 result;
    kal_uint8 is_sms_packed_required;
    UINT8 *pPDUPacket = NULL;
    UINT8 *pPDUPacketTmp = NULL;
    UINT16 nPDUPacket = 0;
    UINT16 nPDUPacketTmp = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/	
    TBM_ENTRY(0x29f1);
    ASSERT(pCosEvent != NULL);
    
    oslMessage = (ilm_struct *)pCosEvent->nParam1;
    pMmiSendSmsReq1 = (mmi_sat_send_sms_stage1_req_struct*)oslMessage->local_para_ptr;
    is_sms_packed_required = pMmiSendSmsReq1->is_sms_packed_required;

    mmi_trace(g_sw_ADP_SAT, "ADP_SAT SEND_SMS REQ no_addr == %d", pMmiSendSmsReq1->no_addr);
	
    if (pMmiSendSmsReq1->no_addr == 0)   //如果没有传地址上来，就取得短信息中心号码
    {
        CFW_SMS_PARAMETER smsInfo;
        
        memset(&smsInfo, 0, sizeof(CFW_SMS_PARAMETER));
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
        CFW_CfgGetSmsParam(&smsInfo, 0);
#else
        CFW_CfgGetSmsParam(&smsInfo, 0, g_pSATContext->sim_id);
#endif
#else
        CFW_CfgGetSmsParam(&smsInfo, 0);
#endif

        pMmiSendSmsReq1->no_addr = smsInfo.nNumber[0];
        memcpy(pMmiSendSmsReq1->addr, &(smsInfo.nNumber[1]), smsInfo.nNumber[0]); 
        nPDUPacketTmp = pMmiSendSmsReq1->tpdu_length + pMmiSendSmsReq1->no_addr + 1;
        pPDUPacket = (UINT8*)OslMalloc(nPDUPacketTmp);
        if(pPDUPacket == NULL)
        {
            return;
        }
        memset(pPDUPacket, 0xFF, nPDUPacketTmp);
        pPDUPacketTmp = pPDUPacket;	
        *(pPDUPacketTmp++) = pMmiSendSmsReq1->no_addr;
    }
    else //如果传上来地址，在前面加91 代表加号
    {
        nPDUPacketTmp = pMmiSendSmsReq1->tpdu_length + pMmiSendSmsReq1->no_addr + 2;  //tpdu length ，加2是因为增加了2位: 地址长度和 91
        pPDUPacket = (UINT8*)OslMalloc(nPDUPacketTmp);
        if(pPDUPacket == NULL)
        {
            return;
        }
        memset(pPDUPacket, 0xFF, nPDUPacketTmp);
        pPDUPacketTmp = pPDUPacket;
        *(pPDUPacketTmp++) = pMmiSendSmsReq1->no_addr + 1;   //地址的长度，加一是因为多了一个91
        *(pPDUPacketTmp++) = 0x91;   // "+"
    }
	
    nPDUPacket  = nPDUPacketTmp; 

    if (pMmiSendSmsReq1->no_addr != 0)
    {
        memcpy(pPDUPacketTmp, pMmiSendSmsReq1->addr, pMmiSendSmsReq1->no_addr);
        pPDUPacketTmp += pMmiSendSmsReq1->no_addr;

    }
    if (pMmiSendSmsReq1->tpdu_length != 0)
    {
        memcpy(pPDUPacketTmp, pMmiSendSmsReq1->tpdu, pMmiSendSmsReq1->tpdu_length);
    }

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	CFW_CfgSetSmsFormat(0);
	result = CFW_SmsSendMessage(NULL, pPDUPacket, nPDUPacket, 11);
	CFW_CfgSetSmsFormat(1);
#else
	CFW_CfgSetSmsFormat(0, g_pSATContext->sim_id);
	result = CFW_SmsSendMessage(NULL, pPDUPacket, nPDUPacket, 11,  g_pSATContext->sim_id);
	CFW_CfgSetSmsFormat(1,  g_pSATContext->sim_id);
#endif
#else
	CFW_CfgSetSmsFormat(0);
	result = CFW_SmsSendMessage(NULL, pPDUPacket, nPDUPacket, 11);
	CFW_CfgSetSmsFormat(1);
#endif
	
	mmi_trace(g_sw_ADP_SAT, "ADP_SAT SEND_SMS REQ is_sms_packed_required = %d", pMmiSendSmsReq1->is_sms_packed_required);
	mmi_trace(g_sw_ADP_SAT, "ADP_SAT SEND_SMS no_addr = %d", pMmiSendSmsReq1->no_addr);
	mmi_trace(g_sw_ADP_SAT, "ADP_SAT SEND_SMS tpdu_length = %d", pMmiSendSmsReq1->tpdu_length);

#ifdef SAT_TEST_SWITCH
    {
        UINT8 i;
        
        for (i = 0; i< nPDUPacket; i++)
        {
            mmi_trace(g_sw_ADP_SAT, "pPDUPacket->tpdu[%d] = 0x%x", i, (UINT8)pPDUPacket[i]);
        }

        for (i = 0; i< pMmiSendSmsReq1->tpdu_length; i++)
        {
            mmi_trace(g_sw_ADP_SAT, "pMmiSendSmsReq1->tpdu[%d] = 0x%x", i, (UINT8)pMmiSendSmsReq1->tpdu[i]);
        }
    }
#endif
	
    if (result == ERR_SUCCESS)
    {
        g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_SEND_SMS_STAGE1_REQ;
    }
    else
    {
		mmi_trace(g_sw_ADP_SAT, "ADP_SAT SEND_SMS CFW_SmsSendMessage error!!!! = %x", result);    
    }

    OslMfree(pPDUPacket);
    TBM_EXIT(0x29f1);
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_SENDSSTAGE1_req
	PURPOSE			    : process PRT_MMI_SAT_SEND_SS_STAGE1_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_SENDSSTAGE1_req(COS_EVENT* pCosEvent)
{
	ilm_struct *ilm_ptr = NULL;
	mmi_sat_send_ss_stage1_req_struct *pReq = NULL;
	UINT32 ret = 0;
	ASSERT(pCosEvent != NULL);

	ilm_ptr = (ilm_struct *)pCosEvent->nParam1;
	pReq = (mmi_sat_send_ss_stage1_req_struct *)ilm_ptr->local_para_ptr;
	
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	ret = UT_CFW_SsSendUSSD(pReq->input, pReq->length, 3, pReq->dcs, gSSUTI, g_pSATContext->sim_id);	
#else
	ret = CFW_SsSendUSSD(pReq->input, pReq->length, 3, pReq->dcs, gSSUTI, g_pSATContext->sim_id);	
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
	ret = CFW_SsSendUSSD(pReq->input, pReq->length, 3, pReq->dcs, gSSUTI);	
#endif /* __ADP_MULTI_SIM__ */

	if (ret != ERR_SUCCESS)
	{
		mmi_trace(g_sw_ADP_SS, "adp_SAT_Proc_SENDSSTAGE1_req()   CFW_CmSendUSSD  ret = %d", ret);
	}
    
	if (ret == ERR_SUCCESS)
	{
		g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_SEND_SS_STAGE1_REQ;
	}    
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_SENDUSSDTAGE1_req
	PURPOSE			    : process PRT_MMI_SAT_SEND_USSD_STAGE1_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_SENDUSSDTAGE1_req(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *ilm_ptr = NULL;
    mmi_sat_send_ussd_stage1_req_struct *pReq = NULL;
    UINT32 ret = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(pCosEvent != NULL);

    ilm_ptr = (ilm_struct *)pCosEvent->nParam1;
    pReq = (mmi_sat_send_ussd_stage1_req_struct *)ilm_ptr->local_para_ptr;
    gSSCode = SS_OP_PROCESSUNSTRUCTUREDSS_DATA;
    
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    ret = UT_CFW_SsSendUSSD(pReq->input, pReq->length, 3, pReq->dcs, gSSUTI, g_pSATContext->sim_id);	
#else
    ret = CFW_SsSendUSSD(pReq->input, pReq->length, 3, pReq->dcs, gSSUTI, g_pSATContext->sim_id);	
#endif /* ADP_CFW_DUALSIM_SIMULATOR_SUPPORT */
#else
    ret = CFW_SsSendUSSD(pReq->input, pReq->length, 3, pReq->dcs, gSSUTI);	
#endif /* __ADP_MULTI_SIM__ */

    if (ret == ERR_SUCCESS)
    {
        g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_SEND_USSD_STAGE1_REQ;
    }  
    else
    {
        mmi_trace(g_sw_ADP_SS, "adp_SAT_Proc_SENDUSSDTAGE1_req()   CFW_CmSendUSSD  ret = %d", ret);
    }
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_GETINPUTRES_req
	PURPOSE			    : process PRT_MMI_SAT_GET_INPUT_RES_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_GETINPUTRES_req(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *oslMessage;
    UINT32 result;
    kal_uint8 nStatus;
    kal_uint8 comdnType;
    mmi_sat_get_input_res_req_struct* pMmiGetInputResReq;
    kal_uint8 dcs_of_input_text_string;
    kal_uint8 no_input_text_string;
    UINT8 *pStr = NULL;
    UINT8	i;    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(pCosEvent != NULL);
    oslMessage = (ilm_struct *)pCosEvent->nParam1;
    pMmiGetInputResReq = (mmi_sat_get_input_res_req_struct*)oslMessage->local_para_ptr;

    nStatus = pMmiGetInputResReq->res ;
    comdnType = pMmiGetInputResReq->cmd_detail[3];

    dcs_of_input_text_string = pMmiGetInputResReq->dcs_of_text_string;
    no_input_text_string = pMmiGetInputResReq->no_text_string;

    pStr = OslMalloc(no_input_text_string + 2);
    ASSERT(pStr != NULL);
    memset(pStr, 0, (no_input_text_string + 2));
    memcpy((pStr + 1), &pMmiGetInputResReq->text_string[0], no_input_text_string);
    *pStr = dcs_of_input_text_string;
    no_input_text_string += 1;

    mmi_trace(g_sw_ADP_SAT, "ADP_SAT REQ GET_INPUT nStatus = %x, comdnType =%x, dcs = %x, len = %x", 
        nStatus, comdnType, dcs_of_input_text_string, no_input_text_string);
		
    for (i = 0; i< no_input_text_string; i++ )
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT pStr[%d] = %c", i, pStr[i]);
    }

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    result = UT_CFW_SatResponse (comdnType, nStatus, 0, pStr, no_input_text_string, FREE_UTI, g_pSATContext->sim_id);
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, pStr, no_input_text_string, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, pStr, no_input_text_string, FREE_UTI);
#endif		

    if (result == ERR_SUCCESS)
    {
        g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_GET_INPUT_RES_REQ;
    }
    else
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT FUNC:%s CFW_SatResponse Error !!!!!!!!!!!!!!!", __FUNCTION__);    
    }
    
    if(pStr != NULL)
    {
        OslMfree(pStr);
    }
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_DISPLAYTEXT_req
	PURPOSE			    : process PRT_MMI_SAT_DISPLAY_TEXT_RES_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_DISPLAYTEXTRES_req(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *oslMessage;
    UINT32 result;
    kal_uint8 nStatus;
    kal_uint8 comdnType;
    mmi_sat_display_text_res_req_struct* pMmiDisplayTextResReq;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(pCosEvent != NULL);
    
    oslMessage = (ilm_struct *)pCosEvent->nParam1;
    pMmiDisplayTextResReq = (mmi_sat_display_text_res_req_struct*)oslMessage->local_para_ptr;

    nStatus = pMmiDisplayTextResReq->res;
    comdnType = pMmiDisplayTextResReq->cmd_detail[3];
#ifdef __ADP_MULTI_SIM__
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT REQ DISPLAY_TEXT comdnType = 0x%x, nStatus =%d, simid = %d", 
        comdnType, nStatus, g_pSATContext->sim_id);
#else
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT REQ DISPLAY_TEXT comdnType = 0x%x, nStatus =%d", 
        comdnType, nStatus);
#endif

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    result = UT_CFW_SatResponse (comdnType, nStatus, 0, NULL, 0, FREE_UTI, g_pSATContext->sim_id);
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, NULL, 0, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, NULL, 0, FREE_UTI);
#endif			

    if (result == ERR_SUCCESS)
    {
        g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_DISPLAY_TEXT_RES_REQ;
    }
    else
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT FUNC:%s CFW_SatResponse Error !!!!!!!!!!!!!!!", __FUNCTION__);    
    }
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_MENUSEL_req
	PURPOSE			    : process PRT_MMI_SAT_MENU_SELECT_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_MENUSEL_req(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *oslMessage;
    kal_uint8 itemId;
    UINT32 result;
    mmi_sat_menu_select_req_struct *menu_select_req;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(pCosEvent != NULL);

    oslMessage = (ilm_struct *) pCosEvent->nParam1;
    menu_select_req = (mmi_sat_menu_select_req_struct*)oslMessage->local_para_ptr;

    itemId = menu_select_req->item_id;
    #ifdef __ADP_MULTI_SIM__
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT REQ MENU_SELECT comdnType = 0x%x, nStatus =%d, simid = %d, item_id =0x%x", 
        MENU_SELECT, SAT_CMD_PERFORMED_SUCCESSFULLY, g_pSATContext->sim_id, itemId);
    #else
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT REQ MENU_SELECT comdnType = 0x%x, nStatus =%d, item_id =0x%x", 
        MENU_SELECT, SAT_CMD_PERFORMED_SUCCESSFULLY, itemId);
    #endif
    
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    result = UT_CFW_SatResponse (MENU_SELECT, SAT_CMD_PERFORMED_SUCCESSFULLY, itemId, NULL, 0, FREE_UTI, g_pSATContext->sim_id);
#else
    result = CFW_SatResponse (MENU_SELECT, SAT_CMD_PERFORMED_SUCCESSFULLY, itemId, NULL, 0, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
    result = CFW_SatResponse (MENU_SELECT, SAT_CMD_PERFORMED_SUCCESSFULLY, itemId, NULL, 0, FREE_UTI);
#endif		

    if (result == ERR_SUCCESS)
    {
        g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_MENU_SELECT_REQ;
    }
    else
    {
		mmi_sat_menu_select_rsp_struct *pmenu_select_rsp = OslMalloc(sizeof(mmi_sat_menu_select_rsp_struct));
        
		mmi_trace(g_sw_ADP_SAT, "ADP_SAT FUNC:%s CFW_SatResponse Error !!!!!!!!!!!!!!!", __FUNCTION__);    

        

        ASSERT(pmenu_select_rsp != NULL);
        memset(pmenu_select_rsp, 0, sizeof(mmi_sat_menu_select_rsp_struct));	

        pmenu_select_rsp->result = -1;

        adp_SAT_SendMessageToMMI((local_para_struct *)pmenu_select_rsp, 
                                                                PRT_MMI_SAT_MENU_SELECT_RSP, ADP_SAT_MSG_SRC_MOD);
    }
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_SELITEMRES_req
	PURPOSE			    : process PRT_MMI_SAT_SELECT_ITEM_RES_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_SELITEMRES_req(COS_EVENT* pCosEvent)
{
    ilm_struct *oslMessage;
    UINT32 result;
    kal_uint8 nStatus;
    kal_uint8 comdnType;
    kal_uint8 item_id;
    mmi_sat_select_item_res_req_struct* psat_selitemres_req;

    ASSERT(pCosEvent != NULL);

    oslMessage = (ilm_struct *) pCosEvent->nParam1;
    psat_selitemres_req = (mmi_sat_select_item_res_req_struct*)oslMessage->local_para_ptr;

    nStatus = psat_selitemres_req->res ;
    comdnType = psat_selitemres_req->cmd_detail[3];
    item_id = psat_selitemres_req->item_id;
#ifdef __ADP_MULTI_SIM__
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT REQ SELECT_ITEM comdnType = 0x%x, nStatus =%d, simid = %d, item_id =0x%x", 
        comdnType, nStatus, g_pSATContext->sim_id, item_id);
#else
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT REQ SELECT_ITEM comdnType = 0x%x, nStatus =%d, item_id =0x%x", 
        comdnType, nStatus,  item_id);
#endif

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    result = UT_CFW_SatResponse (comdnType, nStatus, item_id, NULL, 0, FREE_UTI, g_pSATContext->sim_id);
#else
    result = CFW_SatResponse (comdnType, nStatus, item_id, NULL, 0, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
    result = CFW_SatResponse (comdnType, nStatus, item_id, NULL, 0, FREE_UTI);
#endif		

    if (result == ERR_SUCCESS)
    {
        g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_SELECT_ITEM_RES_REQ;
    }
    else
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT FUNC:%s CFW_SatResponse Error !!!!!!!!!!!!!!!", __FUNCTION__);    
    }

}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_SETUPMENU_req
	PURPOSE			    : process PRT_MMI_SAT_SETUP_MENU_RES_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_SETUPMENURES_req(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *oslMessage;
    UINT32 result;
    kal_uint8 nStatus;
    kal_uint8 comdnType;
    mmi_sat_setup_menu_res_req_struct* psat_menusetup_req;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    ASSERT(pCosEvent != NULL);

    oslMessage = (ilm_struct *) pCosEvent->nParam1;
    psat_menusetup_req = (mmi_sat_setup_menu_res_req_struct*)oslMessage->local_para_ptr;

    nStatus = psat_menusetup_req->res ;
    comdnType = psat_menusetup_req->cmd_detail[3];
#ifdef __ADP_MULTI_SIM__
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT REQ SETUP_MENU comdnType = 0x%x, nStatus =%d, simid = %d", 
        comdnType, nStatus, g_pSATContext->sim_id);
#else
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT REQ SETUP_MENU comdnType = 0x%x, nStatus =%d", 
        comdnType, nStatus);
#endif

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    result = UT_CFW_SatResponse (comdnType, nStatus, 0, NULL, 0, FREE_UTI, g_pSATContext->sim_id);
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, NULL, 0, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
    result = CFW_SatResponse (comdnType, nStatus, 0, NULL, 0, FREE_UTI);
#endif		

    if (result == ERR_SUCCESS)
    {
        g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_SETUP_MENU_RES_REQ;
    }
    else
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT FUNC:%s CFW_SatResponse Error !!!!!!!!!!!!!!!", __FUNCTION__);
    }
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_Proc_LAUNCHBROWSER_req
	PURPOSE			    : process PRT_MMI_SAT_DISPLAY_TEXT_RES_REQ event.
	INPUT PARAMETERS	: COS_EVENT* pCosEvent
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_Proc_LAUNCHBROWSER_req(COS_EVENT* pCosEvent)
{
	ilm_struct *oslMessage;
	UINT32 result;
	kal_uint8 nStatus;
	kal_uint8 comdnType;
	mmi_sat_launch_browser_res_req_struct* pMmiLaunchBrowserResReq;

	ASSERT(pCosEvent != NULL);
	oslMessage = (ilm_struct *)pCosEvent->nParam1;
	pMmiLaunchBrowserResReq = (mmi_sat_launch_browser_res_req_struct*)oslMessage->local_para_ptr;

	nStatus = pMmiLaunchBrowserResReq->res;
	comdnType = pMmiLaunchBrowserResReq->cmd_detail[3];
	
	mmi_trace(g_sw_ADP_SAT, "SAT nStatus =%d, comdnType=0x%x", nStatus, pMmiLaunchBrowserResReq->cmd_detail[3]);

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	result = UT_CFW_SatResponse (
                        comdnType, 
                        nStatus, 
                        0, 
                        pMmiLaunchBrowserResReq->additional_info, 
                        pMmiLaunchBrowserResReq->size_of_additional_info, 
                        FREE_UTI, 
                        g_pSATContext->sim_id);
#else
	result = CFW_SatResponse (comdnType, nStatus, 0, NULL, 0, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
	result = CFW_SatResponse (comdnType, nStatus, 0, NULL, 0, FREE_UTI);
#endif			
	
	if (result == ERR_SUCCESS)
	{
		g_pSATContext->gCurrSatReqId = PRT_MMI_SAT_LAUNCH_BROWSER_RES_REQ;
	}

	mmi_trace(g_sw_ADP_SAT, "SAT <ADP REQ END> >>>>PRT_MMI_SAT_LAUNCH_BROWSER_RES_REQ CFW_SatResponse = %d", result);	
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_SENDSMS_ind
	PURPOSE			    : send PRT_MMI_SAT_SEND_SMS_IND event.
	INPUT PARAMETERS	: NULL
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_SENDSMS_ind(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    CFW_SAT_SMS_RSP *pCswSendSmsInd = NULL; 
    mmi_sat_send_sms_ind_struct *pMmiSendSmsInd;
    UINT32 nRet = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    UT_CFW_SatGetInformation(SEND_SMS, (void **)&pCswSendSmsInd, g_pSATContext->sim_id);
#else
    nRet = CFW_SatGetInformation(SEND_SMS, (void **)&pCswSendSmsInd, g_pSATContext->sim_id);
#endif
#else
    nRet = CFW_SatGetInformation(SEND_SMS, (void **)&pCswSendSmsInd);
#endif	
    if (nRet != ERR_SUCCESS)
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT SENDSMS CFW info return error = %x", nRet);
        return;
    }
    
    ASSERT(pCswSendSmsInd != NULL);
    TBM_ENTRY(0x29f3);
    
    pMmiSendSmsInd = OslMalloc(sizeof(mmi_sat_send_sms_ind_struct));

    ASSERT(pMmiSendSmsInd != NULL);
    memset(pMmiSendSmsInd, 0, sizeof(mmi_sat_send_sms_ind_struct));

    //get info
    adp_SAT_SET_CMD_Details(pMmiSendSmsInd->cmd_detail, SEND_SMS, pCswSendSmsInd->nComQualifier);

    if(pCswSendSmsInd->nAlphaLen > 0)
    {
        pMmiSendSmsInd->alpha_id = adp_SAT_get_Alpha_Id(
                                                            pCswSendSmsInd->pAlphaStr, 
                                                            &pMmiSendSmsInd->dcs_of_alpha_id, 
                                                            pCswSendSmsInd->nAlphaLen, 
                                                            &pMmiSendSmsInd->no_alpha_id);
    }

    if (pCswSendSmsInd->nAlphaLen)
    {
        pMmiSendSmsInd->is_alpha_id_present = 1;
    }

    pMmiSendSmsInd->is_sms_packed_required = pCswSendSmsInd->nComQualifier & 0x01;

    pMmiSendSmsInd->size_of_addr = pCswSendSmsInd->nAddrLen;
    memcpy(pMmiSendSmsInd->addr, pCswSendSmsInd->pAddrStr, pCswSendSmsInd->nAddrLen);
	
    pMmiSendSmsInd->no_sms_tpdu = pCswSendSmsInd->nPDULen;
    pMmiSendSmsInd->sms_tpdu = pCswSendSmsInd->pPDUStr;                                                                                                                                      

    mmi_trace(g_sw_ADP_SAT, "ADP_SAT no_alpha_id = %d", pMmiSendSmsInd->no_alpha_id);
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT dcs_of_alpha_id = 0x%x", pMmiSendSmsInd->dcs_of_alpha_id);
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT size_of_addr = %d", pMmiSendSmsInd->size_of_addr);
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT no_sms_tpdu = 0x%x", pMmiSendSmsInd->no_sms_tpdu);
#ifdef SAT_TEST_SWITCH
    {
        UINT8 i;
        for (i = 0; i < pMmiSendSmsInd->no_alpha_id; i++)
        {
            mmi_trace(g_sw_ADP_SAT, "pMmiSendSmsInd->alpha_id[%d] = 0x%x", i, pMmiSendSmsInd->alpha_id[i]);
        }
        for (i = 0; i < pMmiSendSmsInd->size_of_addr; i++)
        {      
            mmi_trace(g_sw_ADP_SAT, "pMmiSendSmsInd->addr[%d] = 0x%x", i, pMmiSendSmsInd->addr[i]);
        }
        for (i = 0; i < pMmiSendSmsInd->no_sms_tpdu; i++)
        {      
            mmi_trace(g_sw_ADP_SAT, "pMmiSendSmsInd->sms_tpdu[%d] = 0x%x", i, pMmiSendSmsInd->sms_tpdu[i]);
        }
    }
#endif
    /*lost info
    kal_uint8	is_icon_available;
    sat_icon_struct	icon_info;
    */
    adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSendSmsInd, 
                                                                PRT_MMI_SAT_SEND_SMS_IND, ADP_SAT_MSG_SRC_MOD);

    TBM_EXIT(0x29f3);
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_SENDSS_ind
	PURPOSE			    : send PRT_MMI_SAT_SEND_SS_IND event.
	INPUT PARAMETERS	: NULL
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_SENDSS_ind(void)
{
	CFW_SAT_SS_RSP *pCswSendSsInd = NULL; 
	mmi_sat_send_ss_ind_struct *pMmiSendSsInd;
	UINT32 nRet = 0;

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	UT_CFW_SatGetInformation(SEND_SS, (void **)&pCswSendSsInd, g_pSATContext->sim_id);
#else
	nRet = CFW_SatGetInformation(SEND_SS, (void **)&pCswSendSsInd, g_pSATContext->sim_id);
#endif
#else
	nRet = CFW_SatGetInformation(SEND_SS, (void **)&pCswSendSsInd);
#endif	
	if (nRet != ERR_SUCCESS)
	{
		mmi_trace(g_sw_ADP_SAT, "SAT SENDSS CFW info return error = %x", nRet);
		return;
	}
    
	ASSERT(pCswSendSsInd != NULL);
	TBM_ENTRY(0x29f3);
	pMmiSendSsInd = OslMalloc(sizeof(mmi_sat_send_ss_ind_struct));
	
	ASSERT(pMmiSendSsInd != NULL);
	memset(pMmiSendSsInd, 0, sizeof(mmi_sat_send_ss_ind_struct));
	pMmiSendSsInd->cmd_detail[0] = 0x81;
	pMmiSendSsInd->cmd_detail[1] = 0x03;
	pMmiSendSsInd->cmd_detail[2] = 0x01;
	pMmiSendSsInd->cmd_detail[3] = SEND_SS;
	pMmiSendSsInd->cmd_detail[4] = 0x00;

	if (pCswSendSsInd->nAlphaLen)
	{
		if(pCswSendSsInd->pAlphaStr[0] > 0x7F)
		{
			pMmiSendSsInd->dcs_of_alpha_id = 0x08;
			pMmiSendSsInd->no_alpha_id = pCswSendSsInd->nAlphaLen - 1;
			pMmiSendSsInd->alpha_id = (pCswSendSsInd->pAlphaStr + 1);
		}
		else
		{
			pMmiSendSsInd->dcs_of_alpha_id = 0x04;
			pMmiSendSsInd->no_alpha_id = pCswSendSsInd->nAlphaLen;
			pMmiSendSsInd->alpha_id = pCswSendSsInd->pAlphaStr;
		}
    
		pMmiSendSsInd->is_alpha_id_present = 1;
	}

	pMmiSendSsInd->no_ss_string = pCswSendSsInd->nSSLen;
	pMmiSendSsInd->ss_string = pCswSendSsInd->pSSStr;

	mmi_trace(g_sw_ADP_SAT, "nAlphaLen = %d, nSSLen = %d, ", pMmiSendSsInd->no_alpha_id, pMmiSendSsInd->no_ss_string);
	
	adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSendSsInd, 
							PRT_MMI_SAT_SEND_SS_IND, ADP_SAT_MSG_SRC_MOD);
    
	mmi_trace(g_sw_ADP_SAT, " <<<<PRT_MMI_SAT_SEND_SS_IND");
	TBM_EXIT(0x29f3);
}


/**************************************************************
	FUNCTION NAME		: adp_SAT_SENDUSSD_ind
	PURPOSE			    : send PRT_MMI_SAT_SEND_USSD_IND event.
	INPUT PARAMETERS	: NULL
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_SENDUSSD_ind(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    CFW_SAT_SS_RSP *pCswSendUSSDInd = NULL; 
    mmi_sat_send_ussd_ind_struct *pMmiSendUSSDInd;
    UINT32 nRet = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    UT_CFW_SatGetInformation(SEND_USSD, (void **)&pCswSendUSSDInd, g_pSATContext->sim_id);
#else
    nRet = CFW_SatGetInformation(SEND_USSD, (void **)&pCswSendUSSDInd, g_pSATContext->sim_id);
#endif
#else
    nRet = CFW_SatGetInformation(SEND_USSD, (void **)&pCswSendUSSDInd);
#endif	
    if (nRet != ERR_SUCCESS)
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT SENDUSSD CFW info return error = %x", nRet);
        return;
    }
    
    ASSERT(pCswSendUSSDInd != NULL);
    TBM_ENTRY(0x29f3);
    pMmiSendUSSDInd = OslMalloc(sizeof(mmi_sat_send_ussd_ind_struct));

    ASSERT(pMmiSendUSSDInd != NULL);
    memset(pMmiSendUSSDInd, 0, sizeof(mmi_sat_send_ussd_ind_struct));

     //get info
    adp_SAT_SET_CMD_Details(pMmiSendUSSDInd->cmd_detail, SEND_USSD, pCswSendUSSDInd->nComQualifier);
    
    if (pCswSendUSSDInd->nAlphaLen)
    {
        pMmiSendUSSDInd->alpha_id = adp_SAT_get_Alpha_Id(
                                                                pCswSendUSSDInd->pAlphaStr, 
                                                                &pMmiSendUSSDInd->dcs_of_alpha_id, 
                                                                pCswSendUSSDInd->nAlphaLen, 
                                                                &pMmiSendUSSDInd->no_alpha_id);
        
        pMmiSendUSSDInd->is_alpha_id_present = 1;
    }
    
    pMmiSendUSSDInd->is_icon_available = pCswSendUSSDInd->nIconIdentifer;
    pMmiSendUSSDInd->no_ussd_string = pCswSendUSSDInd->nSSLen;
    pMmiSendUSSDInd->ussd_string = pCswSendUSSDInd->pSSStr;
    pMmiSendUSSDInd->dcs_of_ussd_string = pCswSendUSSDInd->nSSType;
       
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT SEND_USSD IND nAlphaLen = %d, no_ussd_string = %d, dec = %d"
        , pMmiSendUSSDInd->no_alpha_id, pMmiSendUSSDInd->no_ussd_string, pMmiSendUSSDInd->dcs_of_ussd_string);

    /*lost info
    kal_uint8	is_changed_from_setupcall;
    */
    
    adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSendUSSDInd, 
                                                            PRT_MMI_SAT_SEND_USSD_IND, ADP_SAT_MSG_SRC_MOD);

    TBM_EXIT(0x29f3);
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_SETUPCALL_ind
	PURPOSE			    : send PRT_MMI_SAT_CALL_SETUP_IND event.
	INPUT PARAMETERS	: NULL
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_SETUPCALL_ind(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    CFW_SAT_CALL_RSP* pCswSetUpCallInd = NULL;
    UINT32 nRet = 0;
    mmi_sat_call_setup_ind_struct *pMmiSetUpCallInd = NULL;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/	
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    UT_CFW_SatGetInformation(SET_UP_CALL, (void **)&pCswSetUpCallInd, g_pSATContext->sim_id);
#else
    nRet = CFW_SatGetInformation(SET_UP_CALL, (void **)&pCswSetUpCallInd, g_pSATContext->sim_id);
#endif
#else
    nRet = CFW_SatGetInformation(SET_UP_CALL, (void **)&pCswSetUpCallInd);
#endif		

    if (nRet != ERR_SUCCESS)
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT SETUP_CALL CFW info return error = %x", nRet);
        return;
    }
    
    ASSERT(pCswSetUpCallInd != NULL);
    
    pMmiSetUpCallInd = OslMalloc(sizeof(mmi_sat_call_setup_ind_struct));
    ASSERT(pMmiSetUpCallInd != NULL);
    memset(pMmiSetUpCallInd, 0, sizeof(mmi_sat_call_setup_ind_struct));

     //get info
    adp_SAT_SET_CMD_Details(pMmiSetUpCallInd->cmd_detail, SET_UP_CALL, pCswSetUpCallInd->nComQualifier);

    mmi_trace(g_sw_ADP_SAT, "ADP_SAT  nComQualifier = %d", pCswSetUpCallInd->nComQualifier);

    switch (pCswSetUpCallInd->nComQualifier)
    {
        case 0:
        case 1:
        {
            pMmiSetUpCallInd->condition = CSMCC_SAT_SETUP_CALL;
            break;
        }
        case 2:
        case 3:
        {
            pMmiSetUpCallInd->condition = CSMCC_SAT_HOLD_AND_SETUP_CALL;
            break;
        }
        case 4:
        case 5:
        {
            pMmiSetUpCallInd->condition = CSMCC_SAT_REL_AND_SETUP_CALL;
            break;
        }
        default:
        {
            mmi_trace(g_sw_ADP_SAT, "ADP_SAT adp_SAT_SETUPCALL_ind error!!!");
            break;
        }
    }

    //address
    pMmiSetUpCallInd->no_addr = pCswSetUpCallInd->nAddrLen;
    pMmiSetUpCallInd->addr = pCswSetUpCallInd->pAddrStr;

    mmi_trace(g_sw_ADP_SAT, "ADP_SAT pMmiSetUpCallInd->no_addr = %d", pMmiSetUpCallInd->no_addr);
#ifdef SAT_TEST_SWITCH
    {
        UINT8 i;
        for (i = 0; i < pCswSetUpCallInd->nAddrLen; i++)
        {
            mmi_trace(g_sw_ADP_SAT, "ADP_SAT pCswSetUpCallInd->pAddrStr[%d] = 0x%x", i, pCswSetUpCallInd->pAddrStr[i]);
        }
    }
#endif

    //subaddr
    pMmiSetUpCallInd->size_of_subaddr = pCswSetUpCallInd->nSubAddrLen;
    memcpy(pMmiSetUpCallInd->subaddr, pCswSetUpCallInd->pSubAddrStr, pCswSetUpCallInd->nSubAddrLen);

    mmi_trace(g_sw_ADP_SAT, "ADP_SAT pCswSetUpCallInd->nSubAddrLen =%d", pCswSetUpCallInd->nSubAddrLen);
#ifdef SAT_TEST_SWITCH
    {
        UINT8 i;
        for (i = 0; i < pCswSetUpCallInd->nSubAddrLen; i++)
        {
            mmi_trace(g_sw_ADP_SAT, "ADP_SAT pCswSetUpCallInd->pSubAddrStr[%d] = 0x%x", i, pCswSetUpCallInd->pSubAddrStr[i]);
        }
    }
#endif

    //ccp
    pMmiSetUpCallInd->size_of_ccp = pCswSetUpCallInd->nCapabilityCfgLen;
    memcpy(pMmiSetUpCallInd->ccp, pCswSetUpCallInd->pCapabilityCfgStr, pCswSetUpCallInd->nCapabilityCfgLen);

    mmi_trace(g_sw_ADP_SAT, "ADP_SAT pCswSetUpCallInd->nCapabilityCfgLen =%d", pCswSetUpCallInd->nCapabilityCfgLen);
#ifdef SAT_TEST_SWITCH
    {
        UINT8 i;
        for (i = 0; i < pCswSetUpCallInd->nCapabilityCfgLen; i++)
        {
            mmi_trace(g_sw_ADP_SAT, "ADP_SAT pCswSetUpCallInd->pCapabilityCfgStr[%d] = 0x%x", i, pCswSetUpCallInd->pCapabilityCfgStr[i]);
        }
    }
#endif

    //alpha_id1
    pMmiSetUpCallInd->is_alpha_id1_present = 0x01;
    if(pCswSetUpCallInd->nAlphaConfirmLen > 0)
    {
        pMmiSetUpCallInd->alpha_id1 = adp_SAT_get_Alpha_Id(
                                                            pCswSetUpCallInd->pAlphaConfirmStr, 
                                                            &pMmiSetUpCallInd->dcs_of_alpha_id1, 
                                                            pCswSetUpCallInd->nAlphaConfirmLen, 
                                                            &pMmiSetUpCallInd->no_alpha_id1);
    }

    mmi_trace(g_sw_ADP_SAT, "ADP_SAT pMmiSetUpCallInd->nAlphaConfirmLen =%d", pCswSetUpCallInd->nAlphaConfirmLen);
#ifdef SAT_TEST_SWITCH
    {
        UINT8 i;
        for (i = 0; i < pCswSetUpCallInd->nAlphaConfirmLen; i++)
        {
            mmi_trace(g_sw_ADP_SAT, "ADP_SAT pCswSetUpCallInd->pAlphaConfirmStr[%d] = 0x%x", i, pCswSetUpCallInd->pAlphaConfirmStr[i]);
        }
    }
#endif

    //alpha_id2
    pMmiSetUpCallInd->is_alpha_id2_present = 0x01;
    if(pCswSetUpCallInd->nAlphaSetupLen > 0)
    {
        pMmiSetUpCallInd->alpha_id2 = adp_SAT_get_Alpha_Id(
                                                            pCswSetUpCallInd->pAlphaSetupStr, 
                                                            &pMmiSetUpCallInd->dcs_of_alpha_id2, 
                                                            pCswSetUpCallInd->nAlphaSetupLen, 
                                                            &pMmiSetUpCallInd->no_alpha_id2);
    }    
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT pMmiSetUpCallInd->nAlphaSetupLen =%d", pCswSetUpCallInd->nAlphaSetupLen);
#ifdef SAT_TEST_SWITCH
    {
        UINT8 i;
        for (i = 0; i < pCswSetUpCallInd->nAlphaSetupLen; i++)
        {
            mmi_trace(g_sw_ADP_SAT, "pCswSetUpCallInd->pAlphaSetupStr[%d] = 0x%x", i, pCswSetUpCallInd->pAlphaSetupStr[i]);
        }
    }
#endif

    //LOST INFO
    /*
    pCswSetUpCallInd->nTuint
    pCswSetUpCallInd->nTinterval
    
    kal_uint32	duration;
    kal_uint8	is_changed_from_ss;
    kal_uint8	is_icon1_available;
    sat_icon_struct	icon_info1;
    kal_uint8	is_icon2_available;
    sat_icon_struct	icon_info2;
    */
    
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT pCswSetUpCallInd->nTuint = 0x%x", pCswSetUpCallInd->nTuint);
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT pCswSetUpCallInd->nTinterval = 0x%x", pCswSetUpCallInd->nTinterval);

    adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSetUpCallInd, 
                                                            PRT_MMI_SAT_CALL_SETUP_IND, ADP_SAT_MSG_SRC_MOD);
    
    TBM_EXIT(0x29f2);
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_GETINKEY_ind
	PURPOSE			    : process EV_CFW_CC_RELEASE_CALL_IND event.
	INPUT PARAMETERS	: NULL
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
/*
#define GET_INKEY	0x22	//nCmdType value of EV_CFW_SAT_CMDTYPE_IND 
static void adp_SAT_GETINKEY_ind(void)
{
	//TODO...
	//CSW 不支持
}
*/

/**************************************************************
	FUNCTION NAME		: adp_SAT_GETINPUT_ind
	PURPOSE			    : send PRT_MMI_SAT_GET_INPUT_IND event.
	INPUT PARAMETERS	: NULL
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_GETINPUT_ind(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    CFW_SAT_INPUT_RSP *pCswGetInputInd = NULL;
    UINT32 nRet = 0;
    mmi_sat_get_input_ind_struct *pMmiGetInputInd = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    UT_CFW_SatGetInformation(GET_INPUT, (void **)&pCswGetInputInd, g_pSATContext->sim_id);
#else
    nRet = CFW_SatGetInformation(GET_INPUT, (void **)&pCswGetInputInd, g_pSATContext->sim_id);
#endif
#else
    nRet = CFW_SatGetInformation(GET_INPUT, (void **)&pCswGetInputInd);
#endif			
    if (nRet != ERR_SUCCESS)
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT GETINPUT CFW info return error = %x", nRet);
        return;
    }

    ASSERT(pCswGetInputInd != NULL);
    TBM_ENTRY(0x29f4);
	
    pMmiGetInputInd = OslMalloc(sizeof(mmi_sat_get_input_ind_struct));
    ASSERT(pMmiGetInputInd != NULL);
    memset(pMmiGetInputInd, 0, sizeof(mmi_sat_get_input_ind_struct));
    
    //get info
    adp_SAT_SET_CMD_Details(pMmiGetInputInd->cmd_detail, GET_INPUT, pCswGetInputInd->nComQualifier);

    if (0 == (pCswGetInputInd->nComQualifier & 0x01))
    {
        pMmiGetInputInd->type_of_input = TYPE_ONLY_DIGIT;
    }
    else if (0 == (pCswGetInputInd->nComQualifier & 0x02))
    {
        pMmiGetInputInd->type_of_input = TYPE_SMS_DEFAULT_SET;
    }
    else
    {
        pMmiGetInputInd->type_of_input = TYPE_UCS2;
    }

    if (0 == (pCswGetInputInd->nComQualifier & 0x04))
    {
        pMmiGetInputInd->is_input_revealed_to_user = 0x01;
    }

    pMmiGetInputInd->is_help_info_available = (pCswGetInputInd->nComQualifier & 0x80) >> 7;
    pMmiGetInputInd->len_of_max_input = pCswGetInputInd->nResMaxLen;
    pMmiGetInputInd->len_of_min_input = pCswGetInputInd->nResMinLen;

    pMmiGetInputInd->default_text = OslMalloc(MAX_TEXT_DISPLAY_LEN);
    ASSERT(pMmiGetInputInd->default_text != NULL);    
    memset(pMmiGetInputInd->default_text, 0x00, MAX_TEXT_DISPLAY_LEN);    
    pMmiGetInputInd->no_default_text = ADP_SAT_Text_Data_Decode(
                                                                pMmiGetInputInd->default_text, 
                                                                &pMmiGetInputInd->dcs_of_default_text, 
                                                                pCswGetInputInd->pDefTextStr, 
                                                                pCswGetInputInd->nDefTextSch, 
                                                                pCswGetInputInd->nDefTextLen);

    pMmiGetInputInd->text_string = OslMalloc(MAX_TEXT_DISPLAY_LEN);
    ASSERT(pMmiGetInputInd->text_string != NULL);    
    memset(pMmiGetInputInd->text_string, 0x00, MAX_TEXT_DISPLAY_LEN);    
    pMmiGetInputInd->no_text_string = ADP_SAT_Text_Data_Decode(
                                                                pMmiGetInputInd->text_string, 
                                                                &pMmiGetInputInd->dcs_of_text_string, 
                                                                pCswGetInputInd->pTextStr, 
                                                                pCswGetInputInd->nTextSch, 
                                                                pCswGetInputInd->nTextLen);

    
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT nComQualifier = 0x%x, dcs_default_text =0x%x, dcs_text =0x%x",\
        pCswGetInputInd->nComQualifier, pCswGetInputInd->nDefTextSch, pCswGetInputInd->nTextSch);

    adp_SAT_SendMessageToMMI((local_para_struct *)pMmiGetInputInd, 
                                                                       PRT_MMI_SAT_GET_INPUT_IND, ADP_SAT_MSG_SRC_MOD);
    
    TBM_EXIT(0x29f4);
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_DISPLAYTEXT_ind
	PURPOSE			    : send PRT_MMI_SAT_DISPLAY_TEXT_IND event.
	INPUT PARAMETERS	: NULL
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_DISPLAYTEXT_ind(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    CFW_SAT_DTEXT_RSP* pCFWDisplayText = NULL;
    UINT32 nRet = 0;
    mmi_sat_display_text_ind_struct *pSatDisplayTextInd = NULL;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    UT_CFW_SatGetInformation(DISPLAY_TEXT, (void **)&pCFWDisplayText, g_pSATContext->sim_id);
#else
    nRet = CFW_SatGetInformation(DISPLAY_TEXT, (void **)&pCFWDisplayText, g_pSATContext->sim_id);
#endif
#else
    nRet = CFW_SatGetInformation(DISPLAY_TEXT, (void **)&pCFWDisplayText);
#endif		
    if (nRet != ERR_SUCCESS)
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT DISPLAYTEXT CFW info return error = %x", nRet);
        return;
    }

    if(pCFWDisplayText == NULL)
    {
        mmi_trace(g_sw_ADP_SAT,"ADP_SAT Func: %s CFW_SatGetInformation = NULL !!!!!!!", __FUNCTION__);
        return;
    }

    //malloc
    pSatDisplayTextInd = OslMalloc(sizeof(mmi_sat_display_text_ind_struct));
    ASSERT(pSatDisplayTextInd != NULL);    
    memset(pSatDisplayTextInd, 0, sizeof(mmi_sat_display_text_ind_struct));

    pSatDisplayTextInd->text_string = OslMalloc(MAX_TEXT_DISPLAY_LEN);
    ASSERT(pSatDisplayTextInd->text_string != NULL);    
    memset(pSatDisplayTextInd->text_string, 0x00, MAX_TEXT_DISPLAY_LEN);

    //get info
    adp_SAT_SET_CMD_Details(pSatDisplayTextInd->cmd_detail, DISPLAY_TEXT, pCFWDisplayText->nComQualifier);
    if(pCFWDisplayText->nTextLen > 0 && pCFWDisplayText->nTextLen < 241)
    {
    	pSatDisplayTextInd->no_text_string = ADP_SAT_Text_Data_Decode(
		                                                    pSatDisplayTextInd->text_string, 
		                                                    &pSatDisplayTextInd->dcs_of_text_string, 
		                                                    pCFWDisplayText->pTextStr, 
		                                                    pCFWDisplayText->nTextSch, 
		                                                    pCFWDisplayText->nTextLen);
    }
    else
    {
        pSatDisplayTextInd->no_text_string = 0;
    }
    pSatDisplayTextInd->clear_text_type = (pCFWDisplayText->nComQualifier & 0x80) >> 7;
    pSatDisplayTextInd->priority = (pCFWDisplayText->nComQualifier & 0x01);

    //lost info
    //pSatDisplayTextInd->immediate_res;
    //pSatDisplayTextInd->is_icon_available;
    //pSatDisplayTextInd->icon_info;
    
    //send to mmi
    adp_SAT_SendMessageToMMI((local_para_struct *)pSatDisplayTextInd, 
    		PRT_MMI_SAT_DISPLAY_TEXT_IND, ADP_SAT_MSG_SRC_MOD);
    
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT Func:%s, nTextLen = %d, nTextSch = 0x%x, clear_text_type =%d", 
        __FUNCTION__, pCFWDisplayText->nTextLen, pCFWDisplayText->nTextSch, pSatDisplayTextInd->clear_text_type);
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_SELECTITEM_ind
	PURPOSE			    : send PRT_MMI_SAT_SELECT_ITEM_IND event.
	INPUT PARAMETERS	: NULL
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_SELECTITEM_ind(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint8 num_list = 0;
    kal_uint8 dcs_of_alpha_id=0x00;
    kal_uint16 no_item_data = 0;
    UINT8 *pU8;
    U8 *pSatMainItem;
    CFW_SAT_ITEM_LIST *pItemList;
    CFW_SAT_MENU_RSP_LIST *pSelectItemList = NULL;
    kal_uint8 *pAlphaText = NULL;
    UINT8 AlphaLen;
    UINT32 nRet = 0;
    UINT8 *pU8Tmp = NULL;
    mmi_sat_select_item_ind_struct *pMmiSetUpMenuInd = NULL;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TBM_ENTRY(0x29f9);
    
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    nRet = UT_CFW_SatGetInformation(SELECT_ITEM, (void **)&pSelectItemList, g_pSATContext->sim_id);
#else
    nRet = CFW_SatGetInformation(SELECT_ITEM, (void **)&pSelectItemList, g_pSATContext->sim_id);
#endif
#else
    nRet = CFW_SatGetInformation(SELECT_ITEM, (void **)&pSelectItemList);
#endif			
    if (nRet != ERR_SUCCESS)
    {
        mmi_trace(g_sw_ADP_SAT, "ADP_SAT SELECT_ITEM CFW info return error = %x", nRet);
        return;
    }
    
    ASSERT(pSelectItemList != NULL);

    mmi_trace(g_sw_ADP_SAT,"ADP_SAT Func: %s, nAlphaLen = %d", 
                                                        __FUNCTION__, pSelectItemList->nAlphaLen);

    //get alpha_id text
    AlphaLen = pSelectItemList->nAlphaLen;
    if(AlphaLen != 0)
    {
        pU8Tmp = adp_SAT_get_Alpha_Id(pSelectItemList->pAlphaStr, &dcs_of_alpha_id, pSelectItemList->nAlphaLen, &AlphaLen);

        pAlphaText = OslMalloc(AlphaLen+1);
        ASSERT(pAlphaText != NULL);
        memset(pAlphaText, 0, AlphaLen+1);

        memcpy(pAlphaText, pU8Tmp, AlphaLen);
    }

    //get ItemList
    pItemList = pSelectItemList->pItemList;		
    while (pItemList != NULL)
    {
        num_list ++;
        no_item_data += (kal_uint16)(5 + pItemList->nItemLen);
        mmi_trace(g_sw_ADP_SAT, "pItemList->nItemLen =%d", pItemList->nItemLen);
        pItemList = pItemList->pNextItem;     
    }

    mmi_trace(g_sw_ADP_SAT, " no_item_data = %d", no_item_data);
    mmi_trace(g_sw_ADP_SAT, " num_list = %d", num_list);
	
    pSatMainItem = OslMalloc(no_item_data + 1);
    ASSERT(pSatMainItem != NULL);
    memset(pSatMainItem, 0, no_item_data + 1);
    pU8 = pSatMainItem;
    pItemList = pSelectItemList->pItemList;

    while (pItemList != NULL)
    {
        UINT8* pU8ItemStr = pItemList->pItemStr;
        UINT8 Dcs_code;
        UINT8 nItemLen = 0;

        nItemLen = pItemList->nItemLen;
        if(pItemList->nItemLen > 0)
        {
            pU8ItemStr = adp_SAT_get_Alpha_Id(pItemList->pItemStr, &Dcs_code, pItemList->nItemLen, &nItemLen);
        }

        pU8++;
        *pU8 = nItemLen;
        pU8++;
        *pU8 = pItemList->nItemID;
        pU8++;
        *pU8 = Dcs_code ;
        pU8++;
        memcpy(pU8, pU8ItemStr, nItemLen);
        pU8 += nItemLen;
        pItemList = pItemList->pNextItem;  
    }

    //send to MMI
    pMmiSetUpMenuInd = OslMalloc(sizeof(mmi_sat_select_item_ind_struct));
    ASSERT(pMmiSetUpMenuInd != NULL);
    memset(pMmiSetUpMenuInd, 0, sizeof(mmi_sat_select_item_ind_struct));

    adp_SAT_SET_CMD_Details(pMmiSetUpMenuInd->cmd_detail, SELECT_ITEM, pSelectItemList->nComQualifier);
    
    pMmiSetUpMenuInd->is_help_info_available = (pSelectItemList->nComQualifier & 0x80) >> 7;
    pMmiSetUpMenuInd->is_softkey_preferred =  (pSelectItemList->nComQualifier & 0x04) >> 3;
    pMmiSetUpMenuInd->num_of_item = num_list ;
    pMmiSetUpMenuInd->no_alpha_id = (UINT8) AlphaLen;
    pMmiSetUpMenuInd->alpha_id = pAlphaText;
    pMmiSetUpMenuInd->dcs_of_alpha_id = dcs_of_alpha_id;
    pMmiSetUpMenuInd->no_item_data = no_item_data ;
    pMmiSetUpMenuInd->item_data = pSatMainItem;
    //LOST INFO
    /*
    kal_uint8	type_of_presentation;
    kal_uint8	is_item_id_available;    
    kal_uint8	is_icon_available;
    sat_icon_struct	icon_info;
    kal_uint8	item_icon_list_attr;
    kal_uint8	no_item_icon_id_list;
    kal_uint8*	item_icon_id_list;
    kal_uint8	no_next_action_ind_list;
    kal_uint8*	next_action_ind_list;		
    */
    
    adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSetUpMenuInd, 
                                                                PRT_MMI_SAT_SELECT_ITEM_IND, ADP_SAT_MSG_SRC_MOD);

    TBM_EXIT(0x29f9);
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_SETUPMENU_ind
	PURPOSE			    : send PRT_MMI_SAT_SETUP_MENU_IND event.
	INPUT PARAMETERS	: NULL
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
void adp_SAT_SETUPMENU_ind(void)
{
	kal_uint8 num_list = 0;
	kal_uint8 dcs_of_alpha_id=0x00;			//yuxiang 07.6.22
	kal_uint16 no_item_data = 0;
	UINT8* pU8;
	U8* pSatMainItem;
	CFW_SAT_ITEM_LIST * pItemList;
	CFW_SAT_MENU_RSP_LIST *pSetupmenuList = NULL;
	kal_uint8* pAlphaText;
	UINT16 AlphaLen;
	UINT32 nRet = 0;

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	UT_CFW_SatGetInformation(SET_UP_MENU, (void **)&pSetupmenuList, g_pSATContext->sim_id);
#else
	nRet = CFW_SatGetInformation(SET_UP_MENU, (void **)&pSetupmenuList, g_pSATContext->sim_id);
#endif
#else
	nRet = CFW_SatGetInformation(SET_UP_MENU, (void **)&pSetupmenuList);
#endif			
	if (nRet != ERR_SUCCESS)
	{
		mmi_trace(g_sw_ADP_SAT, "SAT SETUPMENU CFW info return error = %x", nRet);
		return;
	}
	ASSERT(pSetupmenuList != NULL);
	TBM_ENTRY(0x29f8);

	if(pSetupmenuList == NULL)
	{
		mmi_trace(g_sw_ADP_SAT,"SAT Func: %s pSetupmenuList = NULL!!!!!!####", __FUNCTION__);
		return;
	}
	pItemList = pSetupmenuList->pItemList;

	mmi_trace(g_sw_ADP_SAT, "SAT pSetupmenuList->nAlphaLen = %d", pSetupmenuList->nAlphaLen);

	//get alpha_id text
	pAlphaText = NULL;
	AlphaLen = pSetupmenuList->nAlphaLen;
	{
		UINT8* pU8Tmp = pSetupmenuList->pAlphaStr;

		mmi_trace(g_sw_ADP_SAT,"SAT Func: %s *pU8Tmp=0x%x", __FUNCTION__, *pU8Tmp);
		if (0 != AlphaLen)
		{
			if (*pU8Tmp == 0x80)
			{
				pU8Tmp++;
				AlphaLen -= 1;
				dcs_of_alpha_id = MMI_UCS2_DCS;
			}
			else if(*pU8Tmp == 0x81)
			{
				pU8Tmp++;
				AlphaLen -= 1;
				dcs_of_alpha_id = MMI_UCS2_81;
			}
			else if(*pU8Tmp == 0x82)
			{
				pU8Tmp++;
				AlphaLen -= 1;
				dcs_of_alpha_id = MMI_UCS2_82;
			}
			else
			{
				dcs_of_alpha_id = MMI_DEFAULT_DCS;
			}

			pAlphaText = OslMalloc(AlphaLen+1);
			ASSERT(pAlphaText != NULL);
			memset(pAlphaText, 0, AlphaLen+1);
			memcpy(pAlphaText, pU8Tmp, AlphaLen);

		}
		else
		{
			mmi_trace(g_sw_ADP_SAT,"SAT Func: %s nAlphaLen = 0!!!!!!####", __FUNCTION__);
			return;			
		}
	}
	
	while (pItemList != NULL)
	{
		num_list ++;
		no_item_data += (kal_uint16)(5 + pItemList->nItemLen);
		mmi_trace(g_sw_ADP_SAT, "SAT no_item_data = %d", no_item_data);
		pItemList = pItemList->pNextItem;     
	}
	
	mmi_trace(g_sw_ADP_SAT, "SAT num_list = %d", num_list);
	
	pSatMainItem = OslMalloc(no_item_data + 1);
	ASSERT(pSatMainItem != NULL);
	memset(pSatMainItem, 0, no_item_data + 1);
	pU8 = pSatMainItem;
	pItemList = pSetupmenuList->pItemList;

	while (pItemList != NULL)
	{
		UINT8* pU8ItemStr = pItemList->pItemStr;
		UINT8 code_dcs;
		UINT8 nItemLen = 0;

		nItemLen = pItemList->nItemLen;	
		if (*pU8ItemStr == 0x80)
		{
			pU8ItemStr++;
			nItemLen--;
			code_dcs = MMI_UCS2_DCS;
		}
		else if(*pU8ItemStr == 0x81)
		{
			pU8ItemStr++;
			nItemLen--;
			code_dcs = MMI_UCS2_81;
		}
		else if(*pU8ItemStr == 0x82)
		{
			pU8ItemStr++;
			nItemLen--;
			code_dcs = MMI_UCS2_82;
		}
		else
		{
			code_dcs = MMI_DEFAULT_DCS;
		}

		{
			mmi_trace(g_sw_ADP_SAT,"SAT Func: %s nItemLen=%d code_dcs=0x%x", __FUNCTION__, nItemLen, code_dcs);
			pU8++;
			*pU8 = nItemLen;
			pU8++;
			*pU8 = pItemList->nItemID;
			pU8++;
			*pU8 = code_dcs;
			pU8++;
			memcpy(pU8, pU8ItemStr, nItemLen);
			pU8 +=nItemLen;
			pItemList = pItemList->pNextItem;  
		}
	}
	
	
	{
		mmi_sat_setup_menu_ind_struct *pMmiSetUpMenuInd = OslMalloc(sizeof(mmi_sat_setup_menu_ind_struct));
		ASSERT(pMmiSetUpMenuInd != NULL);
		memset(pMmiSetUpMenuInd, 0, sizeof(mmi_sat_setup_menu_ind_struct));
		pMmiSetUpMenuInd->cmd_detail[0] = 0x81;
		pMmiSetUpMenuInd->cmd_detail[1] = 0x03;
		pMmiSetUpMenuInd->cmd_detail[2] = 0x01;
		pMmiSetUpMenuInd->cmd_detail[3] = SET_UP_MENU;
		pMmiSetUpMenuInd->cmd_detail[4] = 0x00;
		pMmiSetUpMenuInd->is_help_info_available = (pSetupmenuList->nComQualifier & 0x80)>>7;
		pMmiSetUpMenuInd->is_softkey_preferred =  (pSetupmenuList->nComQualifier & 0x01);
		pMmiSetUpMenuInd->num_of_item = num_list ;
		pMmiSetUpMenuInd->no_alpha_id = (UINT8) AlphaLen;
		pMmiSetUpMenuInd->alpha_id = pAlphaText;
		pMmiSetUpMenuInd->dcs_of_alpha_id = dcs_of_alpha_id;
		pMmiSetUpMenuInd->no_item_data = no_item_data ;
		pMmiSetUpMenuInd->item_data = pSatMainItem;
		
		adp_SAT_SendMessageToMMI((local_para_struct *)pMmiSetUpMenuInd, 
								PRT_MMI_SAT_SETUP_MENU_IND, ADP_SAT_MSG_SRC_MOD);

		mmi_trace(g_sw_ADP_SAT, "SAT <ADP IND END> <<<< PRT_MMI_SAT_SETUP_MENU_IND");
	}
	TBM_EXIT(0x29f8);
}

/**************************************************************
	FUNCTION NAME		: adp_SAT_LOCAL_INFORMATION_ind
	PURPOSE			    : get local info
	INPUT PARAMETERS	: NULL
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
#define Location_Information                00
#define IMEI_of_the_ME                          01
#define Network_Measurement_results      02
#define Date_time_and_time_zone	   
#define Language_setting                 04
#define Timing_Advance                   05

void adp_SAT_LOCAL_INFORMATION_ind(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef CSW_SAT_GET_LOCL_INFO    
    SAT_BASE_RSP *pCswLocalInfo = NULL;
#endif
    CFW_TSM_FUNCTION_SELECT SelecFUN;
    int nReturn;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef CSW_SAT_GET_LOCL_INFO
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
    nReturn = UT_CFW_SatGetInformation(SET_UP_MENU, (void **)&pCswLocalInfo, g_pSATContext->sim_id);
#else
    nReturn = CFW_SatGetInformation(SET_UP_MENU, (void **)&pCswLocalInfo, g_pSATContext->sim_id);
#endif
#else
    nReturn = CFW_SatGetInformation(SET_UP_MENU, (void **)&pCswLocalInfo);
#endif	
#endif

#ifdef CSW_SAT_GET_LOCL_INFO
    if(pCswLocalInfo->nComQualifier == Location_Information)
#endif       
    {
        SelecFUN.nNeighborCell = FALSE;
        SelecFUN.nServingCell = TRUE;

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT	
        nReturn = CFW_EmodOutfieldTestStart(&SelecFUN, FREE_UTI);
#else
        nReturn = CFW_EmodOutfieldTestStart(&SelecFUN, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
        nReturn = CFW_EmodOutfieldTestStart(&SelecFUN, FREE_UTI);
#endif

        if(nReturn == ERR_SUCCESS)
        {
            mmi_trace(g_sw_SAT, " Func: %s PROVIDE LOCAL INFO ERR_SUCCESS", __FUNCTION__);
        }
        else
        {
            mmi_trace(g_sw_SAT, " ADP_SAT Func: %s PROVIDE LOCAL INFO ERROR!!! = %d", __FUNCTION__, nReturn);    
        }
    }
#ifdef CSW_SAT_GET_LOCL_INFO
    else
    {
        mmi_trace(g_sw_SAT, " ADP_SAT PROVIDE LOCAL INFO NOT SURPPORT!!! = %d", __FUNCTION__, pCswLocalInfo->nComQualifier);  
        
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
        UT_CFW_SatResponse (PROVIDE_LOCAL_INFORMATION, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0x0, NULL, 0x0, FREE_UTI, g_pSATContext->sim_id);
#else
        CFW_SatResponse (PROVIDE_LOCAL_INFORMATION, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0x0, NULL, 0x0, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
        CFW_SatResponse (PROVIDE_LOCAL_INFORMATION, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0x0, NULL, 0x0, FREE_UTI);
#endif	        
    }
#endif

}

static void adp_SAT_LAUNCHBROWSER_ind(void)
{
	CFW_SAT_BROWSER_RSP* pCswLaunchBrowser = NULL;
	UINT32 nRet = 0;
    mmi_sat_launch_browser_ind_struct*pSatLaunchBrowserInd = OslMalloc(sizeof(mmi_sat_launch_browser_ind_struct));

#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	UT_CFW_SatGetInformation(LAUNCH_BROWSER, (void **)&pCswLaunchBrowser, g_pSATContext->sim_id);
#else
	nRet = CFW_SatGetInformation(LAUNCH_BROWSER, (void **)&pCswLaunchBrowser, g_pSATContext->sim_id);
#endif
#else
	nRet = CFW_SatGetInformation(LAUNCH_BROWSER, (void **)&pCswLaunchBrowser);
#endif		
	if (nRet != ERR_SUCCESS)
	{
		mmi_trace(g_sw_ADP_SAT, "SAT LAUNCHBROWSER CFW info return error = %x", nRet);
		return;
	}
    
	ASSERT(pCswLaunchBrowser != NULL);

	if(pCswLaunchBrowser == NULL)
	{
		mmi_trace(g_sw_ADP_SAT,"SAT Func: %s CFW_SatGetInformation = NULL !!!!!!!", __FUNCTION__);
		return;
	}
	{
    ASSERT(pSatLaunchBrowserInd != NULL);
    memset(pSatLaunchBrowserInd, 0, sizeof(mmi_sat_launch_browser_ind_struct));
    pSatLaunchBrowserInd->cmd_detail[0] = 0x81;
    pSatLaunchBrowserInd->cmd_detail[1] = 0x03;
    pSatLaunchBrowserInd->cmd_detail[2] = 0x01;
    pSatLaunchBrowserInd->cmd_detail[3] = LAUNCH_BROWSER;
    pSatLaunchBrowserInd->cmd_detail[4] = 0x00;

    if(pCswLaunchBrowser->nAlphaLenLength)
    {
        pSatLaunchBrowserInd->alpha_id= adp_SAT_get_Alpha_Id(
                                    pCswLaunchBrowser->pAlpha, 
                                    &pSatLaunchBrowserInd->dcs_of_alpha_id, 
                                    pCswLaunchBrowser->nAlphaLenLength, 
                                    &pSatLaunchBrowserInd->no_alpha_id);
        pSatLaunchBrowserInd->is_alpha_id_present = TRUE;
    }
    pSatLaunchBrowserInd->url = pCswLaunchBrowser->pURL;
    pSatLaunchBrowserInd->no_url = pCswLaunchBrowser->nURLLength;
    mmi_trace(g_sw_ADP_SAT, "ADP_SAT LaunchBrowser no_url = %d ", pSatLaunchBrowserInd->no_url);
    adp_SAT_SendMessageToMMI((local_para_struct *)pSatLaunchBrowserInd, 
        PRT_MMI_SAT_LAUNCH_BROWSER_IND, ADP_SAT_MSG_SRC_MOD);
	}
    mmi_trace(g_sw_ADP_SAT, "SAT <ADP IND END> <<<<PRT_MMI_SAT_LAUNCH_BROWSER_IND");
    
}
void adp_start_get_cell_info(void)
{
	UINT32 nReturn;
	CFW_TSM_FUNCTION_SELECT SelecFUN;
	U8 i;

	SelecFUN.nNeighborCell = TRUE;
	SelecFUN.nServingCell = TRUE;

	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		#ifdef __ADP_MULTI_SIM__
		nReturn = CFW_EmodOutfieldTestStart(&SelecFUN, FREE_UTI, g_SATContext[0].sim_id);
		#else
		nReturn = CFW_EmodOutfieldTestStart(&SelecFUN, FREE_UTI);
		#endif
		if(nReturn == ERR_SUCCESS)
		{
			bCellInfoForEM[i] = TRUE;
		}
	}

}
void adp_end_get_cell_info(void)
{
	U8 i;
	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		if(bCellInfoForEM[i])
		{
			#ifdef __ADP_MULTI_SIM__
			CFW_EmodOutfieldTestEnd(FREE_UTI, g_SATContext[i].sim_id);
			#else
			CFW_EmodOutfieldTestEnd(FREE_UTI);
			#endif
			bCellInfoForEM[i] = FALSE;
		}
	}
}
/**************************************************************
	FUNCTION NAME		: adp_CC_proc_SPEECHCALL_ind
	PURPOSE			    : process EV_CFW_SAT_CMDTYPE_IND event.
	INPUT PARAMETERS	: NULL
	OUTPUT PARAMETERS   : NULL
	RETURN              : NULL
**************************************************************/
static void adp_SAT_proc_CMDTYPE_ind(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(pCosEvent != NULL);

    switch (pCosEvent->nParam1)
    {
        case SET_UP_MENU:
        {
            adp_SAT_SETUPMENU_ind();
            break;
        }
        case SELECT_ITEM:
        {
            adp_SAT_SELECTITEM_ind();
            break;
        }
        case DISPLAY_TEXT:
        {
            adp_SAT_DISPLAYTEXT_ind();
            break;
        }
        case GET_INPUT:
        {
            adp_SAT_GETINPUT_ind();
            break;
        }
        case SET_UP_CALL:
        {
            adp_SAT_SETUPCALL_ind();
            break;
        }
        case SEND_SMS:
        {
            adp_SAT_SENDSMS_ind();
            break;
        }
        case SEND_SS:
        {
            adp_SAT_SENDSS_ind();
            break;
        }        
        case SEND_USSD:
        {
            adp_SAT_SENDUSSD_ind();
            break;
        }         
        case PROVIDE_LOCAL_INFORMATION:
        {
            adp_SAT_LOCAL_INFORMATION_ind();
            break;
        }
        case POLL_INTERVAL:
        case MORE_TIME:
        case POLLING_OFF:
        case PLAY_TONE:
        {            
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
            UT_CFW_SatResponse (pCosEvent->nParam1, SAT_CMD_PERFORMED_SUCCESSFULLY, 0, NULL, 0, FREE_UTI, g_pSATContext->sim_id);
#else
            CFW_SatResponse (pCosEvent->nParam1, SAT_CMD_PERFORMED_SUCCESSFULLY, 0, NULL, 0, FREE_UTI, g_pSATContext->sim_id);
#endif
#else
            CFW_SatResponse (pCosEvent->nParam1, SAT_CMD_PERFORMED_SUCCESSFULLY, 0, NULL, 0, FREE_UTI); 
#endif			
            break;
        }		
		case LAUNCH_BROWSER:
		{
			adp_SAT_LAUNCHBROWSER_ind();
			//CFW_SatResponse (pCosEvent->nParam1, SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD, 0, NULL, 0, FREE_UTI, g_pSATContext->sim_id);
			
			break;
		}
        default:
        {
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
            UT_CFW_SatResponse (pCosEvent->nParam1, SAT_BACKWARD_MOVE_IN_PROACTIVE_SESSION_BY_USER, 0, NULL, 0x00, FREE_UTI, g_pSATContext->sim_id);   //如果不支持此命令，就返回上一级菜单
#else
            CFW_SatResponse (pCosEvent->nParam1, SAT_BACKWARD_MOVE_IN_PROACTIVE_SESSION_BY_USER, 0, NULL, 0x00, FREE_UTI, g_pSATContext->sim_id);   //如果不支持此命令，就返回上一级菜单
#endif
#else
            CFW_SatResponse (pCosEvent->nParam1, SAT_BACKWARD_MOVE_IN_PROACTIVE_SESSION_BY_USER, 0, NULL, 0x00, FREE_UTI);   //如果不支持此命令，就返回上一级菜单
#endif					
            break;
        }
    }
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchSatReqMsg 

  	PURPOSE				: Message Adaption for SAT REQ

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: NULL

	RETURNS				: TRUE->message be processed
							  FALSE->message not processed
	REMARKS				: 

**************************************************************/
static BOOL ADP_DispatchSatIndMsg(COS_EVENT* pCosEvent)
{		
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(pCosEvent != NULL);
    TBM_ENTRY(0x29f7);

#ifdef __ADP_MULTI_SIM__
    adp_ConfigureContext(ADP_MODULE_SAT,adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif

    if (pCosEvent->nEventId == EV_CFW_SAT_CMDTYPE_IND)
    {
        mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
        mmi_trace(g_sw_ADP_SAT, " >>>>ADP_SAT EV_CFW_SAT_CMDTYPE_IND");
        mmi_trace(g_sw_ADP_SAT, " >>>>nParam1 = %x", pCosEvent->nParam1);
        mmi_trace(g_sw_ADP_SAT, " >>>>nParam2 = %x", pCosEvent->nParam2);
        mmi_trace(g_sw_ADP_SAT, " >>>>nParam3 = %x", pCosEvent->nParam3);
        mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

        adp_SAT_proc_CMDTYPE_ind(pCosEvent);
    }
    else
    {
        TBM_EXIT(0x29f7);
        return FALSE;
    }

    TBM_EXIT(0x29f7);
    return TRUE;
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchSatReqMsg 

  	PURPOSE				: Message Adaption for SAT REQ

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: NULL

	RETURNS				: TRUE->message be processed
							  FALSE->message not processed
	REMARKS				: 

**************************************************************/
static BOOL ADP_DispatchSatReqMsg(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(pCosEvent != NULL);
    TBM_ENTRY(0x29f6);

#ifdef __ADP_MULTI_SIM__
    adp_ConfigureContext(ADP_MODULE_SAT, adp_GetMMISimID(ADP_GET_MMI_DEST_MOD(pCosEvent->nParam3)));
#endif

    switch (pCosEvent->nEventId)
    {		
        case PRT_MMI_SAT_SETUP_MENU_RES_REQ:
        {            
            adp_SAT_Proc_SETUPMENURES_req(pCosEvent);
            break;
        }
        case PRT_MMI_SAT_SELECT_ITEM_RES_REQ:
        {
            adp_SAT_Proc_SELITEMRES_req(pCosEvent);
            break;
        }
        case PRT_MMI_SAT_MENU_SELECT_REQ:
        {
            adp_SAT_Proc_MENUSEL_req(pCosEvent);
            break;
        }
        case PRT_MMI_SAT_DISPLAY_TEXT_RES_REQ:
        {            
            adp_SAT_Proc_DISPLAYTEXTRES_req(pCosEvent);
            break;
        }
        case PRT_MMI_SAT_GET_INPUT_RES_REQ:
        {
            adp_SAT_Proc_GETINPUTRES_req(pCosEvent);
            break;
        }
        case PRT_MMI_SAT_SEND_SMS_STAGE1_REQ:
        {
            adp_SAT_Proc_SENDSMSSTAGE1_req(pCosEvent);
            break;
        }
        case PRT_MMI_SAT_SEND_SMS_STAGE2_REQ:
        {
            adp_SAT_Proc_SENDSMSSTAGE2_req(pCosEvent);
            break;
        }
        case PRT_MMI_SAT_SEND_SS_STAGE1_REQ:
        {
            adp_SAT_Proc_SENDSSTAGE1_req(pCosEvent);
            break;
        }
        case PRT_MMI_SAT_SEND_SS_STAGE2_REQ:
        {
            adp_SAT_Proc_SENDSSTAGE2_req(pCosEvent);
            break;
        }      
        case PRT_MMI_SAT_SEND_USSD_STAGE1_REQ:
        {
            adp_SAT_Proc_SENDUSSDTAGE1_req(pCosEvent);
            break;
        }        
        case PRT_MMI_SAT_SEND_USSD_STAGE2_REQ:
        {
            adp_SAT_Proc_SENDUSSDTAGE2_req(pCosEvent);
            break;
        }               
        case PRT_MMI_SAT_CALL_SETUP_STAGE1_REQ:
        {
            adp_SAT_Proc_CALLSETUPSTAGE1_req(pCosEvent);
            break;
        }
        case PRT_MMI_SAT_CALL_SETUP_STAGE2_REQ:
        {
            adp_SAT_Proc_CALLSETUPSTAGE2_req(pCosEvent);
            break;
        }
        case PRT_MSG_ID_MMI_GET_CELL_INFO_START:
        {
            adp_start_get_cell_info();
            break;
        }
        case PRT_MSG_ID_MMI_GET_CELL_INFO_END:
        {
            adp_end_get_cell_info();
            break;
        }
		case PRT_MMI_SAT_LAUNCH_BROWSER_RES_REQ:
		{
			mmi_trace(g_sw_ADP_SAT, "SAT <ADP REQ BEG >>>>>>>>>>>>>>>>>>>>>>>>>\n");
			mmi_trace(g_sw_ADP_SAT, "   >>>>PRT_MMI_SAT_LAUNCH_BROWSER_RES_REQ");
			mmi_trace(g_sw_ADP_SAT, "SAT nParam3 = %x", pCosEvent->nParam3);
#ifdef __ADP_MULTI_SIM__
			mmi_trace(g_sw_ADP_SAT, " simid = %d",  g_pSATContext->sim_id);
#endif /* __ADP_MULTI_SIM__ */

			mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			adp_SAT_Proc_LAUNCHBROWSER_req(pCosEvent);
			break;
		}
		        
        default:
        {
            TBM_EXIT(0x29f6);
            return FALSE;
        }
    }
    
    TBM_EXIT(0x29f6);
    return TRUE;
}

VOID test_PLMNtoBCD(UINT8 *pIn, UINT8* pOut, UINT8* nInLength)
{
    UINT8 i = 0;
    UINT8 j = 0;

    for (i = 0; i < *nInLength;i += 3)
    {
        if(0xFF != pIn[i])
        {
            j++;
            pOut[2*i]   = pIn[i] & 0x0F;    //MCC digit 1
            pOut[2*i+1] = pIn[i] >> 4;      //MCC digit 2
            pOut[2*i+2] = pIn[i+1] & 0x0F;  //MNC digit 3
            pOut[2*i+5] = pIn[i+1] >> 4;    //MCC digit 3
            pOut[2*i+3] = pIn[i+2] & 0x0F;  //MNC digit 2
            pOut[2*i+4] = pIn[i+2] >> 4;    //MNC digit 1
      	 }
    }
	*nInLength = j;

}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchSatRspMsg 

  	PURPOSE				: Message Adaption for SAT  RSP

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: TRUE->message be processed
							  FALSE->message not processed
	REMARKS				: 

**************************************************************/
static BOOL ADP_DispatchSatRspMsg(COS_EVENT* pCosEvent)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UINT8 nType  =  0;
    UINT32 nParam1 = 0;
    UINT32 nParam2 = 0;
    UINT16 nUTI = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(pCosEvent != NULL);

#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_SAT, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif
	
	nParam1 = pCosEvent->nParam1;
	nParam2 = pCosEvent->nParam2;
	nType   = HIUINT8(pCosEvent->nParam3);
	nUTI = HIUINT16(pCosEvent->nParam3);	
    
	TBM_ENTRY(0x29f5);
    
        switch (pCosEvent->nEventId)
        {
            case EV_CFW_SAT_ACTIVATION_RSP:
            {
                mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                mmi_trace(g_sw_ADP_SAT, " >>>>ADP_SAT EV_CFW_SAT_RESPONSE_RSP");
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam1 = %x", nParam1);
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam2 = %x", nParam2);
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam3 = %x", pCosEvent->nParam3);
                mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
                break;
            }
            case EV_CFW_SAT_GET_STATUS_RSP :
            {
                mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                mmi_trace(g_sw_ADP_SAT, " >>>>ADP_SAT EV_CFW_SAT_RESPONSE_RSP");
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam1 = %x", nParam1);
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam2 = %x", nParam2);
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam3 = %x", pCosEvent->nParam3);
                mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
                break;
            }
            case EV_CFW_SAT_RESPONSE_RSP :
            {
                mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                mmi_trace(g_sw_ADP_SAT, " >>>>ADP_SAT EV_CFW_SAT_RESPONSE_RSP");
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam1 = %x", nParam1);
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam2 = %x", nParam2);
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam3 = %x", pCosEvent->nParam3);
                mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

                adp_SAT_proc_RESPONSE_rsp(pCosEvent);
                break;
            }
            case EV_CFW_SMS_SEND_MESSAGE_RSP:
            {
                mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                mmi_trace(g_sw_ADP_SAT, " >>>>ADP_SAT EV_CFW_SMS_SEND_MESSAGE_RSP");
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam1 = %x", nParam1);
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam2 = %x", nParam2);
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam3 = %x", pCosEvent->nParam3);
                mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
                
                adp_SAT_SENDMESSAGE_rsp(nParam1, nParam2, nType, nUTI);
                TBM_EXIT(0x29f5);
                return FALSE;
                break;
            }
            case EV_CFW_TSM_INFO_IND:
            {
                mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                mmi_trace(g_sw_ADP_SAT, " >>>>ADP_SAT EV_CFW_TSM_INFO_IND");
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam1 = %x", nParam1);
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam2 = %x", nParam2);
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam3 = %x", pCosEvent->nParam3);
                mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");     
                
                adp_SAT_LOCAL_INFORMATION_rsp(pCosEvent);
                break;
            }
            case EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP:
            case EV_CFW_CC_RELEASE_CALL_RSP:
            {
                mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                mmi_trace(g_sw_ADP_SAT, " >>>>ADP_SAT EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP | EV_CFW_CC_RELEASE_CALL_RSP");
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam1 = %x", nParam1);
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam2 = %x", nParam2);
                mmi_trace(g_sw_ADP_SAT, " >>>>nParam3 = %x", pCosEvent->nParam3);
                mmi_trace(g_sw_ADP_SAT, " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");   

                adp_SAT_INITATESPEECHCALL_rsp(pCosEvent);
                TBM_EXIT(0x29f5);
                return FALSE;
            }
            default:
            {
                TBM_EXIT(0x29f5);
                return FALSE;
            }
	  }
        
        TBM_EXIT(0x29f5);
        return TRUE;
}


static void adp_SAT_SET_CMD_Details(U8 *p_cmd_detail, U8 type_of_cmd, U8 cmd_qualifier)
{
    ASSERT(p_cmd_detail != NULL);
    
    p_cmd_detail[0] = CMD_DETAILS_TAG;
    p_cmd_detail[1] = CMD_DETAILS_LEN;
    p_cmd_detail[2] = 0x01;
    p_cmd_detail[3] = type_of_cmd;
    p_cmd_detail[4] = cmd_qualifier;
}

static UINT8 *adp_SAT_get_Alpha_Id(UINT8 *pSrcString, UINT8 *pDec, UINT16 SrcStrlen, UINT8 *pDestStrlen)
{
    UINT8 *pRet = NULL;
    
    ASSERT(pSrcString != NULL);
    ASSERT(pDec != NULL);
    ASSERT(pDestStrlen != NULL);

    if(SrcStrlen ==0)
    {
        *pDestStrlen = 0;
        return pRet;
    }
    
    if (*pSrcString == 0x80)
    {
        pRet = pSrcString + 1;
        *pDestStrlen = SrcStrlen - 1;
        *pDec = MMI_UCS2_DCS;
    }
    else if(*pSrcString == 0x81)
    {
        pRet = pSrcString + 1;
        *pDestStrlen = SrcStrlen - 1;
        *pDec = MMI_UCS2_81;
    }
    else if(*pSrcString == 0x82)
    {
        pRet = pSrcString + 1;
        *pDestStrlen = SrcStrlen - 1;
        *pDec = MMI_UCS2_82;
    }
    else
    {
        pRet = pSrcString;
        *pDestStrlen = SrcStrlen;
        *pDec = MMI_DEFAULT_DCS;
    }

    return pRet;
}
