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

/*****************************************************************************
 *
 * Filename:
 * ---------
 * Personalization.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Personalization check
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: Personalization.c

  	PURPOSE		: Does Personalization check

 

 

	DATE		: 12/03/03

**************************************************************/
#define __NEWSIMULATOR 

#include "stdc.h"
#include "l4dr1.h"

#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "commonscreens.h"
#include "custdatares.h"
#include "protocolevents.h"
#include "timerevents.h"
#include "debuginitdef.h"
#include "ucs2prot.h" 
#include "taskinit.h"
#include "wgui_status_icons.h"
#include "custmenures.h"
#include "globaldefs.h"
#include "unicodexdcl.h"
#include "simdetectionhwmapfile.h"
//#include "simdetectiontype.h"
#include "simdetectiondef.h"

#include "simdetectiongprot.h"
#include "simdetectiongexdcl.h"
#include "audioinc.h"
#include "simdetectiondef.h"
#include "personalizationdef.h"
//#include "personalizationgdcl.h"
#include "personalizationprot.h"
#include "settingprofile.h"
#include "profilegprots.h"

#include "l4dr.h"
#include "custom_util.h"

//micha1230
#include "settingdefs.h"

#undef __NEWSIMULATOR 

static U32 gSpNotAllowedMultiplier=0;
static U8 gDisableSPCodeFlag=0;

//micha1230
extern U8 gCPHSRetryCount;

//extern U8 idleScreenFirst;
//extern U8 gPowerOnPeriod;

/**************************************************************

	FUNCTION NAME		: EntryScrEnterNP()

  	PURPOSE				: Function inputs NP from the user 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrEnterNP(void)
{
//micha1230
	S8 temp1[MAX_PASS_COUNTDOWN_DISP];
	S8 temp2[MAX_PASS_COUNTDOWN_DISP];

	sprintf(temp1, "%d", gCPHSRetryCount);
	AnsiiToUnicodeString(temp2, temp1 );
	pfnUnicodeStrcpy((S8*)temp1, ( S8*)GetString(STR_PASS_TRIALS_REMAIN));
	pfnUnicodeStrcat( (S8*)temp1, (S8*) temp2);


	EntryNewScreen(SCR_ENTER_NP, NULL, EntryScrEnterNP, NULL);
	DinitHistory();
	g_pwr_context[0].PrevScreenIndicator = ENTER_NP;

	if(!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 0;	

	ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_RSK_SOS, 
							0,
							STR_ENTER_NP_MSG,
							INPUT_TYPE_NUMERIC_PASSWORD,
							(U8*)gInputPwdBuffer,
//micha1230
							MAX_PWD_LENGTH_MMI ,(U8*)temp1, NULL);

	SetLeftSoftkeyFunction(ValidateNP, KEY_EVENT_UP);
    SetKeyHandler(ValidateNP,KEY_POUND,KEY_EVENT_UP);
	SetKeyHandler(GoToSOSDialScr,KEY_SEND,KEY_EVENT_DOWN);
	SetCategory111RightSoftkeyFunction(ShowMakeSOSCallScr, KEY_EVENT_UP);

		
}

/**************************************************************

	FUNCTION NAME		: EntryScrEnterNSP()

  	PURPOSE				: Function inputs NSP from the user 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrEnterNSP(void)
{
//micha1230
	S8 temp1[MAX_PASS_COUNTDOWN_DISP];
	S8 temp2[MAX_PASS_COUNTDOWN_DISP];

	sprintf(temp1, "%d", gCPHSRetryCount);
	AnsiiToUnicodeString((S8*)temp2, (S8*)temp1 );
	pfnUnicodeStrcpy((S8*)temp1, ( S8*)GetString(STR_PASS_TRIALS_REMAIN));
	pfnUnicodeStrcat( (S8*)temp1, (S8*) temp2);
	
	EntryNewScreen(SCR_ENTER_NP, NULL, EntryScrEnterNSP, NULL);
	DinitHistory();
	g_pwr_context[0].PrevScreenIndicator = ENTER_NSP;

	if(!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 0;	

	ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_RSK_SOS, 
							0,
							STR_ENTER_NSP_MSG,
							INPUT_TYPE_NUMERIC_PASSWORD,
							(U8*)gInputPwdBuffer,
//micha1230
							MAX_PWD_LENGTH_MMI ,(U8*)temp1, NULL);

	SetLeftSoftkeyFunction(ValidateNSP, KEY_EVENT_UP);
    SetKeyHandler(ValidateNSP,KEY_POUND,KEY_EVENT_UP);
	SetKeyHandler(GoToSOSDialScr,KEY_SEND,KEY_EVENT_DOWN);
	SetCategory111RightSoftkeyFunction(ShowMakeSOSCallScr, KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: EntryScrEnterSP()

  	PURPOSE				: Function inputs SP from the user 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrEnterSP(void)
{
	U32 SPNotAllowedTimeout;

//micha1230
	S8 temp1[MAX_PASS_COUNTDOWN_DISP];
	S8 temp2[MAX_PASS_COUNTDOWN_DISP];

	sprintf(temp1, "%d", gCPHSRetryCount);
	AnsiiToUnicodeString((S8*)temp2, (S8*)temp1 );
	pfnUnicodeStrcpy((S8*)temp1, ( S8*)GetString(STR_PASS_TRIALS_REMAIN));
	pfnUnicodeStrcat( (S8*)temp1, (S8*) temp2);

	EntryNewScreen(SCR_ENTER_SP, NULL, EntryScrEnterSP, NULL);
	DinitHistory();
	g_pwr_context[0].PrevScreenIndicator = ENTER_SP;

	if(!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 0;

	ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_RSK_SOS, 
							0,
							STR_ENTER_SP_MSG,
							INPUT_TYPE_NUMERIC_PASSWORD,
							(U8*)gInputPwdBuffer,
//micha1230
							MAX_PWD_LENGTH_MMI ,(U8*)temp1, NULL);

	SetLeftSoftkeyFunction(ValidateSP, KEY_EVENT_UP);
    SetKeyHandler(ValidateSP,KEY_POUND,KEY_EVENT_UP);
	SetKeyHandler(GoToSOSDialScr,KEY_SEND,KEY_EVENT_DOWN);
	SetCategory111RightSoftkeyFunction(ShowMakeSOSCallScr, KEY_EVENT_UP);

	if(gDisableSPCodeFlag)
	{
		SPNotAllowedTimeout = (gSpNotAllowedMultiplier * 30)*1000;
		StartTimer(SP_FAIL_TIMER,SPNotAllowedTimeout,EnableSPCode);
	}
}

/**************************************************************

	FUNCTION NAME		: EntryScrEnterCP()

  	PURPOSE				: Function inputs CP from the user 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrEnterCP(void)
{
//micha1230
	S8 temp1[MAX_PASS_COUNTDOWN_DISP];
	S8 temp2[MAX_PASS_COUNTDOWN_DISP];

	sprintf(temp1, "%d", gCPHSRetryCount);
	AnsiiToUnicodeString((S8*)temp2, (S8*)temp1 );
	pfnUnicodeStrcpy((S8*)temp1, ( S8*)GetString(STR_PASS_TRIALS_REMAIN));
	pfnUnicodeStrcat( (S8*)temp1, (S8*) temp2);
	
	EntryNewScreen(SCR_ENTER_CP, NULL, EntryScrEnterCP, NULL);
	DinitHistory();
	g_pwr_context[0].PrevScreenIndicator = ENTER_CP;

	if(!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 0;

	ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_RSK_SOS, 
							0,
							STR_ENTER_CP_MSG,
							INPUT_TYPE_NUMERIC_PASSWORD,
							(U8*)gInputPwdBuffer,
//micha1230							
							MAX_PWD_LENGTH_MMI ,(U8*)temp1, NULL);

	SetLeftSoftkeyFunction(ValidateCP, KEY_EVENT_UP);
    SetKeyHandler(ValidateCP,KEY_POUND,KEY_EVENT_UP);
	SetKeyHandler(GoToSOSDialScr,KEY_SEND,KEY_EVENT_DOWN);
	SetCategory111RightSoftkeyFunction(ShowMakeSOSCallScr, KEY_EVENT_UP);
		
}

/**************************************************************

	FUNCTION NAME		: EntryScrEnterSIMP()

  	PURPOSE				: Function inputs SIMP from the user 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrEnterSIMP(void)
{
//micha1230
	S8 temp1[MAX_PASS_COUNTDOWN_DISP];
	S8 temp2[MAX_PASS_COUNTDOWN_DISP];

	sprintf(temp1, "%d", gCPHSRetryCount);
	AnsiiToUnicodeString((S8*)temp2, (S8*)temp1 );
	pfnUnicodeStrcpy((S8*)temp1, ( S8*)GetString(STR_PASS_TRIALS_REMAIN));
	pfnUnicodeStrcat( (S8*)temp1, (S8*) temp2);
	
	EntryNewScreen(SCR_ENTER_SIMP, NULL, EntryScrEnterSIMP, NULL);
	DinitHistory();
	g_pwr_context[0].PrevScreenIndicator = ENTER_SIMP;

	if(!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 0;

	ShowCategory111Screen(	STR_GLOBAL_OK , 
							IMG_GLOBAL_OK ,
							STR_RSK_SOS, 
							0,
							STR_ENTER_SIMP_MSG,
							INPUT_TYPE_NUMERIC_PASSWORD,
							(U8*)gInputPwdBuffer,
//micha1230
							MAX_PWD_LENGTH_MMI ,(U8*)temp1, NULL);

	SetLeftSoftkeyFunction(ValidateSIMP, KEY_EVENT_UP);
    SetKeyHandler(ValidateSIMP,KEY_POUND,KEY_EVENT_UP);
	SetKeyHandler(GoToSOSDialScr,KEY_SEND,KEY_EVENT_DOWN);
	SetCategory111RightSoftkeyFunction(ShowMakeSOSCallScr, KEY_EVENT_UP);
		
}

/**************************************************************

	FUNCTION NAME		: EntryScrIncorrectNP

  	PURPOSE				: Flash Message Incorrect NP try again 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						 	
**************************************************************/
void EntryScrIncorrectNP(void)
{
	DisplayPopup((U8*)GetString(STR_WRONG_NP_MSG),IMG_GLOBAL_ERROR,0,2000,(U8)ERROR_TONE);
}

/**************************************************************

	FUNCTION NAME		: EntryScrIncorrectSP

  	PURPOSE				: Flash Message Incorrect SP try again 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						 	
**************************************************************/
void EntryScrIncorrectSP(void)
{
	DisplayPopup((U8*)GetString(STR_WRONG_SP_MSG),IMG_GLOBAL_ERROR,0,2000,(U8)ERROR_TONE);
	
	if(gSpNotAllowedMultiplier<=MAX_ALLOWED_SP_TIMEOUT)
	{
		if(gSpNotAllowedMultiplier>0)
			gSpNotAllowedMultiplier=gSpNotAllowedMultiplier*2;
		else
			gSpNotAllowedMultiplier=1;
	}
	gDisableSPCodeFlag=1;

}

/**************************************************************

	FUNCTION NAME		: EntryScrIncorrectCP

  	PURPOSE				: Flash Message Incorrect CP try again 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						 	
**************************************************************/
void EntryScrIncorrectCP(void)
{
	DisplayPopup((U8*)GetString(STR_WRONG_CP_MSG),IMG_GLOBAL_ERROR,0,2000,(U8)ERROR_TONE);
}


/**************************************************************

	FUNCTION NAME		: EntryScrIncorrectNSP

  	PURPOSE				: Flash Message Incorrect NSP try again 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						 	
**************************************************************/
void EntryScrIncorrectNSP(void)
{
	DisplayPopup((U8*)GetString(STR_WRONG_NSP_MSG),IMG_GLOBAL_ERROR,0,2000,(U8)ERROR_TONE);
}

/**************************************************************

	FUNCTION NAME		: EntryScrIncorrectSIMP

  	PURPOSE				: Flash Message Incorrect SIMP try again 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						 	
**************************************************************/
void EntryScrIncorrectSIMP(void)
{
	DisplayPopup((U8*)GetString(STR_WRONG_SIMP_MSG),IMG_GLOBAL_ERROR,0,2000,(U8)ERROR_TONE);
}

/**************************************************************

	FUNCTION NAME		: ValidateNP()

  	PURPOSE				: Function to validate password 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ValidateNP(void)
{
 
	 
	 
	MYQUEUE	Message; 
	MMISMUVERIFYPINREQ* local_data;

	if(!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 1;

	if(pfnUnicodeStrlen((S8*)gInputPwdBuffer) < MMI_MIN_PASSWORD_LENGTH)
	{
		memset((S8*)gInputPwdBuffer,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
		EntryScrInvalidPINPUKPLOCKLength();
		return;
	}

	PRINT_INFORMATION(("@@@@@ Sending request for verifying NP  @@@@@"));
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
//micha0420
	Message.oslMsgId = PRT_MMI_SMU_VERIFY_PIN_REQ;  
	local_data = (MMISMUVERIFYPINREQ*) OslConstructDataPtr(sizeof 
					(MMISMUVERIFYPINREQ));
	local_data->pin_type = MMI_TYPE_NP;
	
/* 1. Change gInputPwdBuffer to ASCII then copy local_data->pin*/
		UnicodeToAnsii((S8*)local_data->pin,(S8*)gInputPwdBuffer );		
	
	Message.oslDataPtr = (oslParaType *)local_data;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);

}
/**************************************************************

	FUNCTION NAME		: ValidateSIMP()

  	PURPOSE				: Function to validate password 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ValidateSIMP(void)
{
 
	 
	MYQUEUE	Message; 
	MMISMUVERIFYPINREQ* local_data;

	if(!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 1;

	if(pfnUnicodeStrlen((S8*)gInputPwdBuffer) < MMI_MIN_PASSWORD_LENGTH)
	{
		memset((S8*)gInputPwdBuffer,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
		EntryScrInvalidPINPUKPLOCKLength();
		return;
	}

	PRINT_INFORMATION(("@@@@@ Sending request for verifying SIMP  @@@@@"));
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
//micha0420
	Message.oslMsgId = PRT_MMI_SMU_VERIFY_PIN_REQ;  
	local_data = (MMISMUVERIFYPINREQ*) OslConstructDataPtr(sizeof 
					(MMISMUVERIFYPINREQ));
	local_data->pin_type = MMI_TYPE_IMSI_LOCK;
	
/* 1. Change gInputSIMPBuffer to ASCII then copy local_data->pin*/
		UnicodeToAnsii((S8*)local_data->pin,(S8*)gInputPwdBuffer );		
	
	Message.oslDataPtr = (oslParaType *)local_data;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);

}
/**************************************************************

	FUNCTION NAME		: ValidateSP()

  	PURPOSE				: Function to validate password 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ValidateSP(void)
{
 
	 
	MYQUEUE	Message; 
	MMISMUVERIFYPINREQ* local_data;

	if(!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 1;

	if(!gDisableSPCodeFlag)
	{
		if(pfnUnicodeStrlen((S8*)gInputPwdBuffer) < MMI_MIN_PASSWORD_LENGTH)
		{
			memset((S8*)gInputPwdBuffer,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
			EntryScrInvalidPINPUKPLOCKLength();
			return;
		}

		PRINT_INFORMATION(("@@@@@ Sending request for verifying SP  @@@@@"));
		Message.oslSrcId = MOD_MMI;
		Message.oslDestId = MOD_L4C;
//micha0420
		Message.oslMsgId = PRT_MMI_SMU_VERIFY_PIN_REQ;  
		local_data = (MMISMUVERIFYPINREQ*) OslConstructDataPtr(sizeof 
						(MMISMUVERIFYPINREQ));
		local_data->pin_type = MMI_TYPE_SP;
		
	/* 1. Change gInputPwdBuffer to ASCII then copy local_data->pin*/
			UnicodeToAnsii((S8*)local_data->pin,(S8*)gInputPwdBuffer );		
		
		Message.oslDataPtr = (oslParaType *)local_data;
		Message.oslPeerBuffPtr= NULL;
		OslMsgSendExtQueue(&Message);
	}
	else 
	{
		EntryScrSPNotAllowed();
	}
}
/**************************************************************

	FUNCTION NAME		: ValidateCP()

  	PURPOSE				: Function to validate password 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ValidateCP(void)
{
 
  
	MYQUEUE	Message; 
	MMISMUVERIFYPINREQ* local_data;

	if(!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 1;

	if(pfnUnicodeStrlen((S8*)gInputPwdBuffer) < MMI_MIN_PASSWORD_LENGTH)
	{
		memset((S8*)gInputPwdBuffer,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
		EntryScrInvalidPINPUKPLOCKLength();
		return;
	}

	PRINT_INFORMATION(("@@@@@ Sending request for verifying CP  @@@@@"));
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
//micha0420
	Message.oslMsgId = PRT_MMI_SMU_VERIFY_PIN_REQ;  
	local_data = (MMISMUVERIFYPINREQ*) OslConstructDataPtr(sizeof 
					(MMISMUVERIFYPINREQ));
	local_data->pin_type = MMI_TYPE_CP;
	
/* 1. Change gInputPwdBuffer to ASCII then copy local_data->pin*/
		UnicodeToAnsii((S8*)local_data->pin,(S8*)gInputPwdBuffer );		
	
	Message.oslDataPtr = (oslParaType *)local_data;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);

}
/**************************************************************

	FUNCTION NAME		: ValidateNSP()

  	PURPOSE				: Function to validate password 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ValidateNSP(void)
{
 
	 
	 
	MYQUEUE	Message; 
	MMISMUVERIFYPINREQ* local_data;

	if(!IS_IDLE_ARRIVED)
		g_pwr_context[0].PowerOnPeriod = 1;
	
	if(pfnUnicodeStrlen((S8*)gInputPwdBuffer) < MMI_MIN_PASSWORD_LENGTH)
	{
		memset((S8*)gInputPwdBuffer,0, MAX_PWD_LENGTH_MMI * ENCODING_LENGTH);
		EntryScrInvalidPINPUKPLOCKLength();
		return;
	}

	PRINT_INFORMATION(("@@@@@ Sending request for verifying NSP  @@@@@"));
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
//micha0420
	Message.oslMsgId = PRT_MMI_SMU_VERIFY_PIN_REQ;  
	local_data = (MMISMUVERIFYPINREQ*) OslConstructDataPtr(sizeof 
					(MMISMUVERIFYPINREQ));
	local_data->pin_type = MMI_TYPE_NSP;
	
/* 1. Change gInputNSPBuffer to ASCII then copy local_data->pin*/
		UnicodeToAnsii((S8*)local_data->pin,(S8*)gInputPwdBuffer );		
	
	Message.oslDataPtr = (oslParaType *)local_data;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);

}

/**************************************************************

	FUNCTION NAME		: EnableSPCode

  	PURPOSE				: Flash Message SP code not allowed 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						 	
**************************************************************/
void EnableSPCode(void)
{
	gDisableSPCodeFlag=0;
	StopTimer(SP_FAIL_TIMER);
}

/**************************************************************

	FUNCTION NAME		: EntryScrSPNotAllowed

  	PURPOSE				: Flash Message SP code not allowed 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						 	
**************************************************************/
void EntryScrSPNotAllowed(void)
{
	DisplayPopup((U8*)GetString(STR_SP_NOT_ALLOWED_MSG),IMG_GLOBAL_WARNING,0,2000,(U8)ERROR_TONE);
}


