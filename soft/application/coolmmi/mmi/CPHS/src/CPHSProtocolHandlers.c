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
 *  CPHSProtocolHandlers.c
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: CPHSProtocolHandlers.c

  	PURPOSE		: COMMON PCN HANDSET SPECIFICATION

 

 

	DATE		: Aug 6,03

**************************************************************/
#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"
#include "l4dr.h"
#include "csp.h"
#include "infonum.h"
#include "eventsgprot.h"
#include "protocolevents.h"
#include "cphsprotocolhandlers.h"
#include "debuginitdef.h"
#include "simdetectiongprot.h"
#include "wgui_status_icons.h"
#include "mmi_data_types.h"
#include "settinggprots.h"
//micha0309
 
#include "callsetup.h"

#undef __NEWSIMULATOR

BOOL b_CFU_Eable[MMI_SIM_NUMBER] = {FALSE,};

extern void InitialiseInformationArray(U8 noInfoNum, l4csmu_info_num_struct *infoNum);
/*void testcsp(void)
{
	U8 csp[22];
	csp[0]=0x01;
	csp[1]=0xFF;
	csp[2]=0x02;
	csp[3]=0xFF;
	csp[4]=0x03;
	csp[5]=0xFF;
	csp[6]=0x04;
	csp[7]=0xFF;
	csp[8]=0x05;
	csp[9]=0x10;
	csp[10]=0x06;
	csp[11]=0x7F;
	csp[12]=0x07;
	csp[13]=0xFF;
	csp[14]=0x08;
	csp[15]=0xFF;
	csp[16]=0x09;
	csp[17]=0xF8;
	csp[18]=0xC0;
	csp[19]=0x0E;
	csp[20]=0xD5;
	csp[21]=0xFF;
	InitialiseCSPInfoArray(csp);
}*/
/*
void testInfoNum(void)
{
	l4csmu_info_num_struct info[12];
	memset(info,0,sizeof(info));
	info[0].alpha_id_size=strlen("TRAVEL");
	memcpy(info[0].alpha_id,"TRAVEL",info[0].alpha_id_size);
	info[0].ton_npi=0xFF;
	info[0].index_level=1;
	
	info[1].alpha_id_size=strlen("TAXIS");
	memcpy(info[1].alpha_id,"TAXIS",info[1].alpha_id_size);
	info[1].ton_npi=0xFF;
	info[1].index_level=2;

	info[2].alpha_id_size=strlen("Computercabs");
	memcpy(info[2].alpha_id,"Computercabs",info[2].alpha_id_size);
	info[2].ton_npi=1;
	info[2].digit_size=4;
	strcpy(info[2].digit,"111");
	info[2].index_level=3;
	info[2].network_specific=1;
	info[2].premium_rate=1;


	info[3].alpha_id_size=strlen("Dial-a-cab");
	memcpy(info[3].alpha_id,"Dial-a-cab",info[3].alpha_id_size);
	info[3].ton_npi=1;
	info[3].digit_size=4;
	strcpy(info[3].digit,"132");
	info[3].index_level=3;
	info[3].network_specific=1;

	info[4].alpha_id_size=strlen("AIRPORTS");
	memcpy(info[4].alpha_id,"AIRPORTS",info[4].alpha_id_size);
	info[4].ton_npi=0xFF;
	info[4].index_level=2;
	

	info[5].alpha_id_size=strlen("Heathrow");
	memcpy(info[5].alpha_id,"Heathrow",info[5].alpha_id_size);
	info[5].ton_npi=0x1;
	info[5].digit_size=4;
	strcpy(info[5].digit,"345");
	info[5].index_level=3;

	info[6].alpha_id_size=strlen("Gatwick");
	memcpy(info[6].alpha_id,"Gatwick",strlen("info[6].alpha_id_size"));
	info[6].ton_npi=0x1;
	info[6].digit_size=4;
	strcpy(info[6].digit,"651");
	info[6].index_level=3;
	
	info[7].alpha_id_size=strlen("WEATHER");
	memcpy(info[7].alpha_id,"WEATHER",info[7].alpha_id_size);
	info[7].ton_npi=0xFF;
	info[7].index_level=1;

	info[8].alpha_id_size=strlen("North");
	memcpy(info[8].alpha_id,"North",info[8].alpha_id_size);
	info[8].ton_npi=0x1;
	info[8].digit_size=4;
	strcpy(info[8].digit,"323");
	info[8].index_level=2;

	info[9].alpha_id_size=strlen("South");
	memcpy(info[9].alpha_id,"South",info[9].alpha_id_size);
	info[9].ton_npi=0x1;
	info[9].digit_size=4;
	strcpy(info[9].digit,"597");
	info[9].index_level=2;

	info[10].alpha_id_size=strlen("ENTERTAINMENT");
	memcpy(info[10].alpha_id,"ENTERTAINMENT",info[10].alpha_id_size);
	info[10].ton_npi=0xFF;
	info[10].index_level=1;

	info[11].alpha_id_size=strlen("Ticketmaster");
	memcpy(info[11].alpha_id,"Ticketmaster",info[11].alpha_id_size);
	info[11].ton_npi=0x1;
	info[11].digit_size=4;
	strcpy(info[11].digit,"562");
	info[11].index_level=2;

	InitialiseInformationArray(12, info);
}
*/

/****************************************************************************
	FUNCTION NAME		: InitCPHS

  	PURPOSE				: Initialises CPHS

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
void InitCPHS(void)
{
	U8 i;
	ClearCSPArray();
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetProtocolEventHandlerExt(CPHSDisplayCFUInd,PRT_MSG_ID_MMI_CPHS_DISPLAY_CFU_IND, i);
	}
	
	SetProtocolEventHandler(CPHSMMInfoInd,PRT_MSG_ID_MMI_CPHS_MMI_INFO_IND);
	 
//micha0309
	SetProtocolEventHandler(CALLSETLineInUseRsp, PRT_MMI_CPHS_DISPLAY_ALS_IND);

//	testInfoNum();
	
//	testcsp();
//	CSPModifyHiliteHandlers();
}

/****************************************************************************
	FUNCTION NAME		: CPHSDisplayCFUInd

  	PURPOSE				: Protocol Handler for CFU Ind

 

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
extern U8 MTPNP_AD_get_protocol_event_src_mode(void);

void CPHSDisplayCFUInd(void *msgPtr)
{
#ifdef __MMI_CFU_SEPARATE_LINE__  
	static U8 StatusCFULine1=0;
	static U8 StatusCFULine2=0;
#endif /* __MMI_CFU_SEPARATE_LINE__ */
	U8 nSimID;
	mmi_cphs_display_cfu_ind_struct *MsgCfuInd;
	MsgCfuInd=(mmi_cphs_display_cfu_ind_struct*)msgPtr;

	nSimID = MTPNP_AD_get_protocol_event_src_mode();
#ifdef __MMI_CFU_SEPARATE_LINE__  

		HideStatusIcon(STATUS_ICON_CALL_DIVERT_L1);
		HideStatusIcon(STATUS_ICON_CALL_DIVERT_L2);
		HideStatusIcon(STATUS_ICON_CALL_DIVERT_L1L2);


	if(MsgCfuInd->action==1)
	{
		if(MsgCfuInd->line_num==0)
			StatusCFULine1=1;
		else if(MsgCfuInd->line_num==1)
			StatusCFULine2=1;
	}
	else
	{
		if(MsgCfuInd->line_num==0)
			StatusCFULine1=0;
		else if(MsgCfuInd->line_num==1)
			StatusCFULine2=0;
	}

	if((StatusCFULine1)&&(!StatusCFULine2))
		{
			ShowStatusIcon(STATUS_ICON_CALL_DIVERT_L1);
		}
	else if((!StatusCFULine1)&&(StatusCFULine2))
		{
			ShowStatusIcon(STATUS_ICON_CALL_DIVERT_L2);
		}
	else if((StatusCFULine1)&&(StatusCFULine2))
		{
			ShowStatusIcon(STATUS_ICON_CALL_DIVERT_L1L2);
		}

#else  /* !__MMI_CFU_SEPARATE_LINE__ */

	if(MsgCfuInd->action==1)
	{
		b_CFU_Eable[nSimID] = TRUE;
		ShowStatusIcon(STATUS_ICON_CALL_DIVERT_L1L2 + nSimID);
	}
	else
	{
		b_CFU_Eable[nSimID] = FALSE;
	   	HideStatusIcon(STATUS_ICON_CALL_DIVERT_L1L2 + nSimID);
	}
	UpdateStatusIcons();
#endif /* __MMI_CFU_SEPARATE_LINE__ */
	
}

/****************************************************************************
	FUNCTION NAME		: CPHSDisplayCFUInd

  	PURPOSE				: Protocol Handler for MMI Info Ind

 

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
#ifdef __MMI_EONS_SUPPORT__
extern kal_bool is_sim_replaced(void);
extern void eons_replace_opn_by_pnn_for_hplmn_req(void);
#endif

void CPHSMMInfoInd(void *msgPtr)
{
	mmi_cphs_mmi_info_ind_struct *mmiIndoInd;
	mmiIndoInd=(mmi_cphs_mmi_info_ind_struct *)msgPtr;
	PRINT_INFORMATION(("<<CPHS>> Recieved CPHS MMI Info Ind"));
	if(mmiIndoInd->is_valid_csp)
		InitialiseCSPInfoArray(mmiIndoInd->csp);
	if(mmiIndoInd->no_info_num)
		InitialiseInformationArray(mmiIndoInd->no_info_num, mmiIndoInd->info_num);
/****************** Deepali*****************************/
	ServiceIndicationStringDisplay(mmiIndoInd);
/****************** Deepali*****************************/

#ifdef __MMI_EONS_SUPPORT__
	if( !is_sim_replaced() )
		eons_replace_opn_by_pnn_for_hplmn_req(); // replace opn string
#endif

	GetSettingOptionsStatus();
}


