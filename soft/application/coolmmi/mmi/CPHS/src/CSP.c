/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *  CSP.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: CSP.c

  	PURPOSE		: Customer Service Profile

 

 

	DATE		: Aug 6,03

**************************************************************/
#define __NEWSIMULATOR
#include "stdc.h"
#include "mmi_features.h"
#include "l4dr.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

 
#include "globalmenuitems.h"
 
#include "csp.h"
#include "eventsgprot.h"
#include "debuginitdef.h"
#include "custmenures.h"
#include "l4dr.h"
#ifdef __MOD_SMSAL__
#include "messagesexdcl.h"
#endif
#undef __NEWSIMULATOR


U8 cpsInfoArray[CSP_TOTAL_SERVICE_GROUP];
U8 isCSPPresent=0;
U8 bootupOver=0;

/****************************************************************************
	FUNCTION NAME		: SetCSPGroupService

  	PURPOSE				: Sets Bit in CSP Array about CSP Service as per 
							service group & sevice

	INPUT PARAMETERS	: serviceGroup: Service Group of CSP
							service: Service in the group

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/

void SetCSPGroupService(U8 serviceGroup, U8 service)
{
	SetBit(cpsInfoArray[serviceGroup],service);
}

/****************************************************************************
	FUNCTION NAME		: ResetCSPGroupService

  	PURPOSE				: Resets Bit in CSP Array about CSP Service as per 
							service group & sevice

	INPUT PARAMETERS	: serviceGroup: Service Group of CSP
							service: Service in the group

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
void ResetCSPGroupService(U8 serviceGroup, U8 service)
{
	ResetBit(cpsInfoArray[serviceGroup],service);
}

/****************************************************************************
	FUNCTION NAME		: IsSetCSPGroupService

  	PURPOSE				: Checks if Bit ise set in CSP Array about CSP Service 
							as per service group & sevice

	INPUT PARAMETERS	: serviceGroup: Service Group of CSP
							service: Service in the group

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
U8 IsSetCSPGroupService(U8 serviceGroup, U8 service)
{
	return(IsBitSet(cpsInfoArray[serviceGroup],service));

}

/****************************************************************************
	FUNCTION NAME		: IsResetCSPGroupService

  	PURPOSE				: Checks if Bit is reset in CSP Array about CSP Service 
							as per service group & sevice

	INPUT PARAMETERS	: serviceGroup: Service Group of CSP
							service: Service in the group

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/

U8 IsResetCSPGroupService(U8 serviceGroup, U8 service)
{
	return(IsBitReset(cpsInfoArray[serviceGroup],service));
}

/****************************************************************************
	FUNCTION NAME		: IsInformationNumberAlowed

  	PURPOSE				: Checks if Information Number is allowed

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/

U8 IsInformationNumberAlowed(void)
{
	if(cpsInfoArray[CSP_INFORMATION_NO_SERVICE_GROUP]==0xFF)
		return 1;
	else
		return 0;
}

/****************************************************************************
	FUNCTION NAME		: ClearCSPArray

  	PURPOSE				: Clears Information in CSP Array

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/
void ClearCSPArray(void)
{
	memset((void*)cpsInfoArray,0xFF,CSP_TOTAL_SERVICE_GROUP);
}

/****************************************************************************
	FUNCTION NAME		: InitialiseCSPInfoArray

  	PURPOSE				: Initialises CSP Infor

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/
void InitialiseCSPInfoArray(U8 *cspInfo)
{
	U8 i;
	PRINT_INFORMATION(("<<CPHS>>In InitialiseCSPInfoArray"));
	for(i=0;i<22;i++)
	{
		switch(cspInfo[i])
		{
			case 0x01:
				cpsInfoArray[CSP_CALL_OFFERING_SERVICE_GROUP]=cspInfo[++i];
				break;
			case 0x02:
				cpsInfoArray[CSP_CALL_RESTRICTION_SERVICE_GROUP]=cspInfo[++i];
				break;
			case 0x03:
				cpsInfoArray[CSP_OTHER_SS_SERVICE_GROUP]=cspInfo[++i];
				break;
			case 0x04:
				cpsInfoArray[CSP_CALL_COMPLETION_SERVICE_GROUP]=cspInfo[++i];
				break;
			case 0x05:
				cpsInfoArray[CSP_TELESERVICES_SERVICE_GROUP]=cspInfo[++i];
				break;
			case 0x06:
				cpsInfoArray[CSP_CPHS_TELESERVICES_SERVICE_GROUP]=cspInfo[++i];
				break;
			case 0x07:
				cpsInfoArray[CSP_CPHS_FEATURES_SERVICE_GROUP]=cspInfo[++i];
				break;
			case 0x08:
				cpsInfoArray[CSP_NUMBER_IDENTIFICATION_SERVICE_GROUP]=cspInfo[++i];
				break;
			case 0x09:
				cpsInfoArray[CSP_PHASE_2_SERVICE_GROUP]=cspInfo[++i];
				break;
			case 0xC0:
				cpsInfoArray[CSP_VALUE_ADDED_SERVICES_SERVICE_GROUP]=cspInfo[++i];
//micha1217
#ifdef __MMI_CSP_LANG_SEL_MENU_ALWAYS_ON__
//Always turn on language selection for Chicago
				cpsInfoArray[CSP_VALUE_ADDED_SERVICES_SERVICE_GROUP]|=0x01;
#endif	
				break;
			case 0xD5:
				cpsInfoArray[CSP_INFORMATION_NO_SERVICE_GROUP]=cspInfo[++i];
				break;
		}
	}
	isCSPPresent=1;
	if(bootupOver) CSPModifyHiliteHandlers();
}

/****************************************************************************
	FUNCTION NAME		: CSPCallMain

  	PURPOSE				: Changes Hilite Handlers of CM in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/

void CSPCallMain(void)
{

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	 
	if(IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BOAC) &&
		IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BOIC) &&
		IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BOIC_exHC) &&
		IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BAIC) &&		
		IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BIC_Roam) )
		mmi_frm_hide_menu_item(MENU8237_SCR8093_MNGCALL_MENU_BARRING);

	if(IsResetCSPGroupService(CSP_OTHER_SS_SERVICE_GROUP,CSP_CUG))
		mmi_frm_hide_menu_item(MENU_SETTING_CALL_CLOSED_GP);

	if(IsResetCSPGroupService(CSP_CALL_COMPLETION_SERVICE_GROUP,CSP_CW))
		mmi_frm_hide_menu_item(MENU8237_SCR8093_MNGCALL_MENU_WAIT);

	if(IsResetCSPGroupService(CSP_CPHS_TELESERVICES_SERVICE_GROUP,ALS))
		mmi_frm_hide_menu_item(MENU_LINE_SWCH);
}

/****************************************************************************
	FUNCTION NAME		: CSPPhoneSetup

  	PURPOSE				: Changes Hilite Handlers of Phone Setup in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/

void CSPPhoneSetup(void)
{

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	 
	if(IsResetCSPGroupService(CSP_VALUE_ADDED_SERVICES_SERVICE_GROUP,CSP_Language))
		mmi_frm_hide_menu_item(MENU_SETTING_LANGUAGE);
}

/****************************************************************************
	
	FUNCTION NAME		: CSPCallBarringMainMenu

  	PURPOSE				: Changes Hilite Handlers of Call Barring Main Screen
							in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/

void CSPCallBarringMainMenu(void)
{

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	 
	if(IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BOAC) &&
		IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BOIC) &&
		IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BOIC_exHC) )
		mmi_frm_hide_menu_item(MENU_CALL_BARRING_OUTGOING);

	if(IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BAIC) &&		
		IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BIC_Roam) )
		mmi_frm_hide_menu_item(MENU_CALL_BARRING_INCOMING);
}

/****************************************************************************
	
	FUNCTION NAME		: CSPCallBarringOutgoingMenu

  	PURPOSE				: Changes Hilite Handlers of Call Barring Outgoing Screen
							in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/
void CSPCallBarringOutgoingMenu(void)	
{

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	 
	if(IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BOAC))
		mmi_frm_hide_menu_item(MENU_BARRING_OUTGOING_ALLCALLS);

	if(IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BOIC))
		mmi_frm_hide_menu_item(MENU_BARRING_OUTGOING_INTERCALL);

	if(IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BOIC_exHC))
		mmi_frm_hide_menu_item(MENU_BARRING_OUTGOING_INTEREXCEPTHOME);
}

/****************************************************************************
	
	FUNCTION NAME		: CSPCallBarringIncomingMenu

  	PURPOSE				: Changes Hilite Handlers of Call Barring Incoming Screen
							in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/

void CSPCallBarringIncomingMenu(void)	
{

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	 
	if(IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BAIC))
		mmi_frm_hide_menu_item(MENU_BARRING_INCOMING_ALLCALLS);

	if(IsResetCSPGroupService(CSP_CALL_RESTRICTION_SERVICE_GROUP,CSP_BIC_Roam))
		mmi_frm_hide_menu_item(MENU_BARRING_INCOMING_ROAMING);

}

/****************************************************************************
	
	FUNCTION NAME		: CSPCallForwardingMenu

  	PURPOSE				: Changes Hilite Handlers of Call Fwd. Screen
							in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/
void CSPCallForwardingMenu(void)
{

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	 
	if(IsResetCSPGroupService(CSP_CALL_OFFERING_SERVICE_GROUP,CSP_CFU))
		mmi_frm_hide_menu_item(MENU8238_MNGCALL_CALFWD_VOICECALL);

	if(IsResetCSPGroupService(CSP_CALL_OFFERING_SERVICE_GROUP,CSP_CFNRc))
		mmi_frm_hide_menu_item(MENU8239_CALFWD_VOICECALL_UNREACHABLE);

	if(IsResetCSPGroupService(CSP_CALL_OFFERING_SERVICE_GROUP,CSP_CFNRy))
		mmi_frm_hide_menu_item(MENU8239_CALFWD_VOICECALL_NOANSWER);

	if(IsResetCSPGroupService(CSP_CALL_OFFERING_SERVICE_GROUP,CSP_CFB))
		mmi_frm_hide_menu_item(MENU8239_CALFWD_VOICECALL_BUSY);
}

/****************************************************************************
	
	FUNCTION NAME		: CSPCallerIdMenu

  	PURPOSE				: Changes Hilite Handlers of Caller ID Screen
							in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/
void CSPCallerIdMenu(void)
{

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	 
	if(IsResetCSPGroupService(CSP_NUMBER_IDENTIFICATION_SERVICE_GROUP,CSP_CLI_block))
		mmi_frm_hide_menu_item(MENU_CID_HIDE_ID);

	if(IsResetCSPGroupService(CSP_NUMBER_IDENTIFICATION_SERVICE_GROUP,CSP_CLI_send))
		mmi_frm_hide_menu_item(MENU_CID_SHOW_ID);
}

/****************************************************************************
	
	FUNCTION NAME		: CSPCallHistoryMenu

  	PURPOSE				: Changes Hilite Handlers of Call History Screen
							in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/

void CSPCallHistoryMenu(void)
{

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	 
	if(IsResetCSPGroupService(CSP_OTHER_SS_SERVICE_GROUP,CSP_AoC))
		mmi_frm_hide_menu_item(MENU_CALL_HISTORY_CALLCOST);
}

/****************************************************************************
	
	FUNCTION NAME		: CSPNetworkSelectionMenu

  	PURPOSE				: Changes Hilite Handlers of Network Selection Screen
							in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/

void CSPNetworkSelectionMenu(void)
{

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	 
	if(IsResetCSPGroupService(CSP_VALUE_ADDED_SERVICES_SERVICE_GROUP,CSP_PLMN_MODE))
		mmi_frm_hide_menu_item(MENU9186_SELECTION_MODE);

	if(IsResetCSPGroupService(CSP_PHASE_2_SERVICE_GROUP,CSP_Multiple_Band))
		mmi_frm_hide_menu_item(MENU9188_BAND_SELECTION);
}

/****************************************************************************
	
	FUNCTION NAME		: CSPMesagesMainMenu

  	PURPOSE				: Changes Hilite Handlers of Messages Main Menu Screen
							in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/
void CSPMesagesMainMenu(void)
{

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	 
	if(IsResetCSPGroupService(CSP_TELESERVICES_SERVICE_GROUP,CSP_SM_MT))
		mmi_frm_hide_menu_item(MESSAGES_MENU_INBOX_MENUID);

	if(IsResetCSPGroupService(CSP_TELESERVICES_SERVICE_GROUP,CSP_SM_MO))
	{
		mmi_frm_hide_menu_item(MESSAGES_MENU_WMESSAGE_MENU_ID);
		mmi_frm_hide_menu_item(MESSAGES_MENU_OUTBOX_MENUID);
	}

	if(IsResetCSPGroupService(CSP_TELESERVICES_SERVICE_GROUP,CSP_SM_CB))
		mmi_frm_hide_menu_item(MESSAGES_MENU_CB_MENUID);

	if(IsResetCSPGroupService(CSP_VALUE_ADDED_SERVICES_SERVICE_GROUP,CSP_VPS))
		mmi_frm_hide_menu_item(MESSAGES_MENU_VMAIL_MENUID);
}

/****************************************************************************
	
	FUNCTION NAME		: CSPMessageSettingMenu

  	PURPOSE				: Changes Hilite Handlers of Message Setting Menu Screen
							in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/

void CSPMessageSettingMenu(void)
{

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	 
	if(IsResetCSPGroupService(CSP_TELESERVICES_SERVICE_GROUP,CSP_Del_Conf) &&
		IsResetCSPGroupService(CSP_TELESERVICES_SERVICE_GROUP,CSP_Reply_path) )
		mmi_frm_hide_menu_item(SETUP_MENU_COMMONSETTING_MENUID);
}

 
/****************************************************************************
	
	FUNCTION NAME		: CSPOutgoingCallManagement

  	PURPOSE				: Changes Hilite Handlers of Outgoing Call Management Menu Screen
							in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/

void CSPOutgoingCallManagement(void)
{
	if(IsResetCSPGroupService(CSP_OTHER_SS_SERVICE_GROUP,CSP_MPTY))
		mmi_frm_hide_menu_item(MITEM2015_CM_CALL_CONFER);

	if(IsResetCSPGroupService(CSP_CALL_COMPLETION_SERVICE_GROUP,CSP_HOLD))
		mmi_frm_hide_menu_item(MITEM2010_CM_ACTIVE_HOLD);
}
 

/****************************************************************************
	
	FUNCTION NAME		: CSPModifyHiliteHandlers

  	PURPOSE				: Modifies Hilite Handlers of all applications
							in Framework as per CSP Info

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/

void CSPModifyHiliteHandlers(void)
{
	bootupOver=1;
	if(isCSPPresent)
	{
		PRINT_INFORMATION(("<<CPHS>>CSP Present and Modifying Hilite Handlers"));
		CSPCallBarringMainMenu();
		CSPCallBarringIncomingMenu();
		CSPCallBarringOutgoingMenu();
		CSPCallForwardingMenu();
		CSPCallerIdMenu();
		CSPCallMain();
		CSPPhoneSetup();
		CSPCallHistoryMenu();
		CSPNetworkSelectionMenu();
		CSPMesagesMainMenu();
		CSPMessageSettingMenu();
		CSPOutgoingCallManagement();  
	}
//tangjie del begin 20060731
#if 0
#ifdef MMI_ON_HARDWARE_P
	 
	if(sim_service_table_query(SERVICE_CHV_DISABLE_FUNCTION) == SERVICE_NOT_SUPPORT)
		mmi_frm_hide_menu_item(MENU9169_SIM_LOCK);
#endif
#endif
//tangjie del end 20060731
}

static FuncPtr CMHiliteHandlersArray[32];
static U16 CMHiliteHandlersId[32];
static U8 itemsToBeRestored;
extern hiliteInfo maxHiliteInfo[MAX_HILITE_HANDLER];			/* available hilite func ptrs  */
/****************************************************************************
	
	FUNCTION NAME		: RestorePreviousHiliteHandlers

  	PURPOSE				: Restores Previous Hilite Handlers as per the parent,
							used only in CM

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: TRUE/FALSE

 

******************************************************************************/

void RestorePreviousHiliteHandlers(U16 parentId)
{
	static U8 count=0;
	U8 i;
	if(count)
	{
		for(i=0;i<itemsToBeRestored;i++)
			maxHiliteInfo[CMHiliteHandlersId[i]].entryFuncPtr=CMHiliteHandlersArray[i];	
	}
	if(count==0)count=1;
		
	itemsToBeRestored=(U8)GetNumOfChild(parentId);
	for(i=0;i<itemsToBeRestored;i++)
	{
		CMHiliteHandlersId[i]=GetSeqItemId((U16)parentId, (S16)i);
		CMHiliteHandlersArray[i]=maxHiliteInfo[CMHiliteHandlersId[i]].entryFuncPtr;
	}
	
}


