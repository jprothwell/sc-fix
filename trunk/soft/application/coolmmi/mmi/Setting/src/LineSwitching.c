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
 *	LineSwitching.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   <file description>.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/**************************************************************

	FILENAME	: LineSwitching.c


  	PURPOSE		: 

 

	 

	DATE		: 

**************************************************************/
#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"

#include "protocolevents.h"
#include "callsetup.h"
#include "phonebooktypes.h"
#include "simdetectiondef.h"
#include "simdetectiongexdcl.h"
#include "wgui_status_icons.h"
#include "idleappprot.h"
#include "idleappdef.h" 
#include "debuginitdef.h"
#include "callsetupenum.h"
#include "globaldefs.h"
#include "wgui_fixed_menus.h"
#include "lineswitchgprot.h"
#include "nvram_user_defs.h"
#include "nvramprot.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim_call.h"
#include "mtpnp_ad_resdef.h"
#endif
#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__  
#include "statusiconres.h"
#endif

#undef __NEWSIMULATOR 


/**************************************************************

	FUNCTION NAME		: InitLineSwitch(void)

  	PURPOSE				: initialization function for line switching 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InitLineSwitch(void)
{
	S16 error;
	static BOOL first_in = FALSE;
	U8 i;

#ifdef __MMI_MULTI_SIM__
	E_MTPNP_AD_STATUS status;
#else
	U8 status = 1;
#endif
	
	mmi_trace(TRUE,"MMI_CallSetting Func: %s ", __FUNCTION__);
	
	if (first_in)
		return;
	else
		first_in = TRUE;

	/* set hilite handlers for the individual menu items */
	SetHiliteHandler(MENU_LINE_SWCH, HighlightCALLSETLineSwitch);

#ifdef __MMI_MULTI_SIM__
	status = MTPNP_AD_Get_Startup_Mode();
#endif
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	for(i = 0; i < 1; i++)
#else
	for(i = 0; i < MMI_SIM_NUMBER; i++)
#endif
	{
		ReadRecord(NVRAM_EF_LINESWITCH_LID, 1 + i, (void *)&g_callset_context[i].LineID, sizeof(g_callset_context[i].LineID), &error);
	
		if(status&(0x01<<i))
		{
			 if(g_callset_context[i].LineID == LINE1)
		 	{
		 	
			#ifdef __MMI_MULTI_SIM__
				#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_SIM_ID__
		 		ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH + i, IMG_SI_MASTER_SIGNAL_STRENGTH_L1 + i);
				#else
				ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH + i, IMG_SI_SIGNAL_STRENGTH_L1);
				#endif/*__MMI_STATUS_ICON_SIGNAL_WITH_SIM_ID__*/
			#else
				ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH + i, IMG_SI_SIGNAL_STRENGTH_L1);
			#endif
		 	}
		 	else
		 	{
		 	#ifdef __MMI_MULTI_SIM__
				#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_SIM_ID__
		 		ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH + i, IMG_SI_MASTER_SIGNAL_STRENGTH_L2+i);
				#else
				ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH + i, IMG_SI_SIGNAL_STRENGTH_L2);
				#endif/*__MMI_STATUS_ICON_SIGNAL_WITH_SIM_ID__*/
			#else
				ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH + i, IMG_SI_SIGNAL_STRENGTH_L2);
			#endif
		 	}
		}
	}

	return;
}
/**************************************************************

	FUNCTION NAME		: HighlightCALLSETLineSwitch(void)

  	PURPOSE				: hilite handler for line selection option 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCALLSETLineSwitch(void)
{	
	g_callset_cntx_p->LineIDInput=(U8)GetCurrHiliteID();
	/* Change left soft key icon and label */
	ChangeLeftSoftkey(STR_SETTING_LINE_SEL, 0);
	/* Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	/* set the left/right soft key functions handlers */	

	SetLeftSoftkeyFunction(MakePsSelectLine,KEY_EVENT_UP);
	SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* set the left/right arrow key function handlers */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	return;
}
/**************************************************************

	FUNCTION NAME		: MakePsSelectLine(void)

  	PURPOSE				: Sends Clip request to stack

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakePsSelectLine(void)
{
	MYQUEUE   Message;
	mmi_cphs_update_als_req_struct *msg = NULL;
	msg = (mmi_cphs_update_als_req_struct*) 
			OslConstructDataPtr(sizeof(mmi_cphs_update_als_req_struct));
	ClearAllKeyHandler();
	SetKeyHandler(0,KEY_END,KEY_EVENT_DOWN);
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MMI_CALLSET_DEST_MOD;

	if (g_callset_cntx_p->LineID == LINE1) {
		msg->line_id = LINE2;
	}
	else {
		msg->line_id = LINE1;	
	}
	
#ifdef __MMI_MULTI_SIM__
	MTPNP_CallSetSetProtocolEventHandler(CALLSETLineSwitchRsp, PRT_MMI_CPHS_UPDATE_ALS_RSP);
#else
	SetProtocolEventHandler(CALLSETLineSwitchRsp, PRT_MMI_CPHS_UPDATE_ALS_RSP);
#endif
	Message.oslMsgId = PRT_LINE_SET_REQ;
	Message.oslDataPtr = (oslParaType *)msg;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
	return;
}
/**************************************************************

	FUNCTION NAME		: CALLSETLineSwitchRsp(void)

  	PURPOSE				: response handler for set line1 req

	INPUT PARAMETERS	: response buffer void *

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/


void CALLSETLineSwitchRsp(void *info)
{
	mmi_cphs_update_als_rsp_struct *local;
  	S16 error;
#ifdef __MMI_LINE_SWITCH_SHOW_NUMBER__
	S8 tempstr[10];
#endif
	local = (mmi_cphs_update_als_rsp_struct*)info;

	mmi_trace(TRUE," Func: %s local->res=%d  SIM: %d, LineID: %d", __FUNCTION__,local->res, g_callset_cntx_p->nSimId, g_callset_cntx_p->LineID);

	if (local->res == MMI_TRUE) 
	{
		if (g_callset_cntx_p->LineID == LINE1) {
		 
		/* ChangeStatusIconImage will not call UpdateStatusIcon */
		#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__  
		#ifdef __MMI_MULTI_SIM__
			#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_SIM_ID__
			ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH + g_callset_cntx_p->nSimId, IMG_SI_MASTER_SIGNAL_STRENGTH_L2 + g_callset_cntx_p->nSimId);
			#else
			ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH + g_callset_cntx_p->nSimId, IMG_SI_SIGNAL_STRENGTH_L2);
			#endif
		#else
			ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH + g_callset_cntx_p->nSimId, IMG_SI_SIGNAL_STRENGTH_L2);
		#endif
		#else
			HideStatusIcon(STATUS_ICON_ALTERNATE_LINE_L1);
			ShowStatusIcon(STATUS_ICON_ALTERNATE_LINE_L2);
		#endif
		 
			g_callset_cntx_p->LineID =LINE2;
			pfnUnicodeStrcpy((S8*)g_callset_cntx_p->LineInfo, ( S8*)GetString(STR_LSW_LINE2));
#ifdef __MMI_LINE_SWITCH_SHOW_NUMBER__
#ifdef __MMI_MULTI_SIM__
			g_pDispOwnerNumber = gDispOwnerNumber[g_callset_cntx_p->nSimId];	
#endif			
			if(pfnUnicodeStrlen((S8*)g_pDispOwnerNumber[1].name) > 0) {
				AnsiiToUnicodeString((S8*)tempstr, (S8*)",");
				pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) tempstr);
				pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) g_pDispOwnerNumber[1].name);
			}
			else if (pfnUnicodeStrlen((S8*)g_pDispOwnerNumber[1].number) > 0) {
				AnsiiToUnicodeString((S8*)tempstr, (S8*)",");
				pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) tempstr);
				pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) g_pDispOwnerNumber[1].number);
			}
#endif

		}
		else {
		 
		/* ChangeStatusIconImage will not call UpdateStatusIcon */
		#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__  
		#ifdef __MMI_MULTI_SIM__
			#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_SIM_ID__
			ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH + g_callset_cntx_p->nSimId, IMG_SI_MASTER_SIGNAL_STRENGTH_L1 + g_callset_cntx_p->nSimId);
			#else
			ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH + g_callset_cntx_p->nSimId, IMG_SI_SIGNAL_STRENGTH_L1);
			#endif
		#else
			ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH + g_callset_cntx_p->nSimId, IMG_SI_SIGNAL_STRENGTH_L1);
		#endif
		#else
			HideStatusIcon(STATUS_ICON_ALTERNATE_LINE_L2);
			ShowStatusIcon(STATUS_ICON_ALTERNATE_LINE_L1);
		#endif
		 
			g_callset_cntx_p->LineID =LINE1;
			pfnUnicodeStrcpy((S8*)g_callset_cntx_p->LineInfo, ( S8*)GetString(STR_LSW_LINE1));
#ifdef __MMI_LINE_SWITCH_SHOW_NUMBER__
#ifdef __MMI_MULTI_SIM__
			g_pDispOwnerNumber = gDispOwnerNumber[g_callset_cntx_p->nSimId];	
#endif			
			if(pfnUnicodeStrlen((S8*)g_pDispOwnerNumber[0].name) > 0) {
				AnsiiToUnicodeString((S8*)tempstr, (S8*)",");
				pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) tempstr);
				pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) g_pDispOwnerNumber[0].name);
			}
			else if (pfnUnicodeStrlen((S8*)g_pDispOwnerNumber[0].number) > 0) {
				AnsiiToUnicodeString((S8*)tempstr, (S8*)",");
				pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) tempstr);
				pfnUnicodeStrcat((S8*)g_callset_cntx_p->LineInfo, ( S8*) g_pDispOwnerNumber[0].number);
			}
#endif
		}

		RefreshDisplayOwnerNumber((U8)g_callset_cntx_p->LineID);
//micha0602
#ifdef __MMI_MULTI_SIM__
		if (GetActiveScreenId() == SCR_ID_UCM_GENERAL_CALL_SETTING ) 
#else
		if (GetActiveScreenId() == SCR_ID_CALLSET_MAIN) 
#endif 
		{
	 		pfnUnicodeStrcpy((S8*)hintData[g_callset_cntx_p->LineIDInput],(PS8)g_callset_cntx_p->LineInfo);
			Category52ChangeItemDescription(g_callset_cntx_p->LineIDInput,(U8*)g_callset_cntx_p->LineInfo);
			RedrawCategoryFunction();
			ClearAllKeyHandler();
			register_fixed_list_keys();
			HighlightCALLSETLineSwitch();
		}
		else 
		{
			UpdateStatusIcons();
		}
	#ifdef __MMI_MULTI_SIM__
		WriteRecord(NVRAM_EF_LINESWITCH_LID, 1+g_callset_cntx_p->nSimId, (void *)&g_callset_cntx_p->LineID, sizeof(g_callset_cntx_p->LineID), &error);
	#else 
		WriteRecord(NVRAM_EF_LINESWITCH_LID, 1, (void *)&g_callset_cntx_p->LineID, sizeof(g_callset_cntx_p->LineID), &error);//man
	#endif
	}
	else {
		ShowCategory2Screen (STR_LINE_SET_TITLE,IMG_LINE_SET_TITLE,
							STR_GLOBAL_OK,IMG_GLOBAL_OK, 
							0, 0,  STR_LINE1_SET_ERR, NULL);
		SetLeftSoftkeyFunction (GoBackHistory,KEY_EVENT_UP);
	}
	return;
}


/**************************************************************

	FUNCTION NAME		: RefreshDisplayOwnerNumber(U8 lineid)

  	PURPOSE				: Owner number associate with Line ID

	INPUT PARAMETERS	: U8 line id

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void UI_set_font(UI_font_type f);
void RefreshDisplayOwnerNumber(U8 lineid)
{
     U8 idx=0, iCnt=0;
     S32 StringWidth, StringHeight;
     stFontAttribute prev_f;
	U16 i;
     
     if( lineid == LINE1 )
       idx = 0;
     else
       idx = 1;

	for( i = 0; i < MMI_SIM_NUMBER; i++)
	{
		g_pDispOwnerNumber = gDispOwnerNumber[i];
		memset( gOwnerName[i], 0, sizeof( gOwnerName[i]) );
		if( g_pDispOwnerNumber[idx].name[0] != 0x0 || g_pDispOwnerNumber[idx].name[1] != 0x0 ) //not null string
		{
			iCnt = ( (MAX_PB_NAME_LENGTH) > MAX_DISPLAY_CHAR ) ? MAX_DISPLAY_CHAR : (MAX_PB_NAME_LENGTH);
			pfnUnicodeStrncpy(gOwnerName[i], (PS8)g_pDispOwnerNumber[idx].name, iCnt );
		}
		else if( g_pDispOwnerNumber[idx].number[0] != 0x0 ||  g_pDispOwnerNumber[idx].number[1] != 0x0 ) //not null string
		{
			iCnt = ( (MAX_PB_NUMBER_LENGTH+1) > MAX_DISPLAY_CHAR ) ? MAX_DISPLAY_CHAR : (MAX_PB_NUMBER_LENGTH+1) ;
			pfnUnicodeStrncpy(gOwnerName[i], (PS8)g_pDispOwnerNumber[idx].number, iCnt  );
		}

		memcpy( &prev_f, UI_font, sizeof(stFontAttribute) );
		//UI_set_font(&MMI_default_font); 
		//lisen: to calculate based on the current font size
		UI_set_font(&UI_DEFAULT_FONT);
		for( iCnt= (U8)pfnUnicodeStrlen((PS8) gOwnerName[i]);  iCnt>0;  iCnt-- )
		{ 
			Get_StringWidthHeight((U8*)gOwnerName[i],&StringWidth,&StringHeight);
			/* Add one pixel for border text */
			if( (StringWidth+1) >= MAIN_LCD_device_width -(ICON_WIDTH<<1)   )
			{
				gOwnerName[i][iCnt*ENCODING_LENGTH] = 0x0;
				//micha0305
				gOwnerName[i][iCnt*ENCODING_LENGTH+1] = 0x0;
			}
			else
				break;
		}
		UI_set_font(&prev_f);
	}
      

}
/**************************************************************

	FUNCTION NAME		: CALLSETLineInUseRsp(void)

  	PURPOSE				: response handler to set the line in use

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CALLSETLineInUseRsp(void *info)
{
#if 0   //dyj del 20060515
	mmi_cphs_display_als_ind_struct *localPtr;

	/* check the response struct for set line in use */
	localPtr = (mmi_cphs_display_als_ind_struct *)info;
	if(localPtr->line == LINE1)
	{
		/* update the status icon the menu items */
	#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__  
	#ifdef __FLIGHT_MODE_SUPPORT__
		if(mmi_bootup_get_active_flight_mode() == 0)
	#endif
			ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH, IMG_SI_SIGNAL_STRENGTH_L1); 
		UpdateStatusIcons(); 
	#else
		IdleResetStatusIcon(STATUS_ICON_ALTERNATE_LINE_L2);
		IdleSetStatusIcon(STATUS_ICON_ALTERNATE_LINE_L1);
	#endif
		g_callset_cntx_p->LineID =LINE1;
	}
	else
	{
		/* update the status icon the menu items */
	#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__  
	#ifdef __FLIGHT_MODE_SUPPORT__
		if(mmi_bootup_get_active_flight_mode() == 0)
	#endif
			ChangeStatusIconImage(STATUS_ICON_SIGNAL_STRENGTH, IMG_SI_SIGNAL_STRENGTH_L2);
		UpdateStatusIcons(); 
	#else
		IdleResetStatusIcon(STATUS_ICON_ALTERNATE_LINE_L1);
		IdleSetStatusIcon(STATUS_ICON_ALTERNATE_LINE_L2);
	#endif
		g_callset_cntx_p->LineID =LINE2;
	}
	/* Owner number associate with Line ID */
	RefreshDisplayOwnerNumber((U8)g_callset_cntx_p->LineID);
	return;
#endif
}

 
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
void set_vertical_signal_status_icons(void)
{
#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__	
	if(g_callset_cntx_p->LineID == LINE1)
		ChangeStatusIconImageWithoutReArrange(STATUS_ICON_SIGNAL_STRENGTH, IMG_SI_SIGNAL_STRENGTH_IDLE_L1);
	else if(g_callset_cntx_p->LineID == LINE2)
		ChangeStatusIconImageWithoutReArrange(STATUS_ICON_SIGNAL_STRENGTH, IMG_SI_SIGNAL_STRENGTH_IDLE_L2);
#endif
}

void set_horizontal_signal_status_icons(void)
{
#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__		
	if(g_callset_cntx_p->LineID == LINE1)
		ChangeStatusIconImageWithoutReArrange(STATUS_ICON_SIGNAL_STRENGTH, IMG_SI_SIGNAL_STRENGTH_L1);
	else if(g_callset_cntx_p->LineID == LINE2)
		ChangeStatusIconImageWithoutReArrange(STATUS_ICON_SIGNAL_STRENGTH, IMG_SI_SIGNAL_STRENGTH_L2);
#endif
}
#endif
 
