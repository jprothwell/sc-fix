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
 *  Modification Notice:
 *  --------------------------
 *  This software is modified by COOLSAND Inc. and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of COOLSAND Inc. (C) 2001
 *
 *******************************************************************************/

 /*******************************************************************************
 * Filename:
 * ---------
 *   AutoRedialAttempt.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  .......
 *
 * Author:
 * -------
 * -------
 *
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/CallManagement/src/AutoRedialAttempt.c-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:56:58   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:37:24   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:23:44   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Nov 06 2006 17:32:52   zhangxb
 * Initial revision.
 * 
 *    Rev 1.1   Sep 19 2006 19:10:48   zhoumn
 * No change.
 * 
 *    Rev 1.0   Jul 06 2006 17:42:36   zhangxb
 *
 *******************************************************************************/
 
/**************************************************************

	FILENAME	: AutoRedialAttempt.c

  	PURPOSE		: Redial attempts api

 

	AUTHOR		: Ashish

	DATE		: Nov 8,02

**************************************************************/
#define __NEWSIMULATOR
#include "stdc.h"
#include "mmi_features.h"
#include "l4dr.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globaldefs.h"
#include "globalconstants.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "debuginitdef.h"
#include "wgui_categories.h"
#include "timerevents.h"

#include "satgprots.h"
#include "settinggprots.h"
#include "phonebooktypes.h"

#include "callmanagementiddef.h"
#include "historyhelperprot.h"
#include "callmanagementstruct.h"
#include "callstructuremanagementprot.h"
#include "commonstubsprot.h"
#include "cmerrortypes.h"
#include "cmerrorhandlersprot.h"
#include "pscallhelpergprot.h"
#include "callmanagementgprot.h"
#include "outgoingcallhelperprot.h"
#include "outgoingcallmanagementprot.h"
#undef __NEWSIMULATOR

#include "redialhelpergprot.h"

static U8 gRedialMsgTemp[100];	/* temp array for display redial string */

/**************************************************************

	FUNCTION NAME		: EntryScrShowUserTimeToRedial(void)

  	PURPOSE				: Entry Screen for user notification on time left to redial

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrShowUserTimeToRedial(void)
{
   	EntryNewScreen(CM_SCR_RDL_TIME, ExitScrShowUserTimeToRedial,
                   NULL, NULL);

	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_EntryScrShowUserTimeToRedial));

	if (GetCurrScrnId() != CM_SCR_RDL_TIME)
   	DeleteScreenIfPresent(CM_SCR_RDL_TIME);
	
	ShowCategory141Screen(0,0,0,0,STR_GLOBAL_ABORT, 0,
						       gRedialMsgTemp, 0, NULL);

	SetRightSoftkeyFunction (ResetRedialAndGoBack, KEY_EVENT_UP);
	SetKeyHandler(ResetRedialAndGoBack,KEY_END,KEY_EVENT_DOWN);
	return;
}

/**************************************************************

	FUNCTION NAME		: ExitScrShowUserTimeToRedial

  	PURPOSE				: Exit Screen for blacklist full

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrShowUserTimeToRedial(void)
{
	if(GetCurrentState()==CM_INCOMING_STATE)
	{
		return;
	}

	if(cm_p->redial_info.RedialTimer == TRUE)
	{
		history_t   Scr1009;
		S16		nHistory	=	0;

		Scr1009.scrnID = CM_SCR_RDL_TIME;
		Scr1009.entryFuncPtr = EntryScrShowUserTimeToRedial;
		pfnUnicodeStrcpy((S8*)Scr1009.inputBuffer, (S8*)&nHistory);
		GetCategoryHistory(Scr1009.guiBuffer);
		AddHistory(Scr1009);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: ResetRedialAndGoBack(void)

  	PURPOSE				: Entry Screen for user notification on time left to redial

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern void mdi_audio_resume_background_play(void);
void ResetRedialAndGoBack(void)
{
	ResetRedialAttempts();
	GoBackfromNotifyScr();
    mdi_audio_resume_background_play();
	return;
}

/**************************************************************

	FUNCTION NAME		: ResetRedialAttempts

  	PURPOSE				: This function resets redial counter for call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void ResetRedialAttempts(void)
{
	cm_p->redial_info.MaxAttempts= 0;
	cm_p->redial_info.CurrentAttempt=0;
	cm_p->redial_info.RedialTimer = FALSE;
	cm_p->redial_info.ErrRedialScreen = FALSE;
	cm_p->redial_info.DialFromPhb = MMI_PHB_NONE;
	memset (cm_p->redial_info.RedialNum,0,MAX_NUMBER);
	StopTimer(CM_REDIAL_TIMER);
	return;
}

/**************************************************************

	FUNCTION NAME		: SetMaxAttempts

  	PURPOSE				: Sets the maximum number of attempts for this call

	INPUT PARAMETERS	: attempts number

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetMaxAttempts (U8 attempts)
{
	if(0==cm_p->redial_info.CurrentAttempt)
	{
		cm_p->redial_info.MaxAttempts = attempts;
   }
   return;
}

/**************************************************************

	FUNCTION NAME		: SetRedialNumber

  	PURPOSE				: To store number to redial

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetRedialNumber(void)
{
	memset((PS8)cm_p->redial_info.RedialNum,0,MAX_NUMBER);
	pfnUnicodeStrcpy((PS8)cm_p->redial_info.RedialNum,(PS8)GetOutgoingNumber());
}

/**************************************************************

	FUNCTION NAME		: IsRedialSet

  	PURPOSE				: This function checks if the redial flag is set

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE/FALSE

	RETURNS				: pBOOL

 
**************************************************************/
pBOOL IsRedialSet(void)
{
	if (GetOutgoingCallOrigin() == CM_CALL_MO_STK)
	{
		return SATCallRedialStatus();
	}
	return AutoRedialOnOffStatus();

}

/**************************************************************

	FUNCTION NAME		: IsRedialNeeded

  	PURPOSE				: This function checks if the error cause needs
  							  to redial

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: TRUE/FALSE

	RETURNS				: pBOOL

 
**************************************************************/
pBOOL IsRedialNeeded (U16 response)
{
   pBOOL result = FALSE;

	MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_IsRedialNeeded));

	if((GetTotalCallCount() == 0) ||
	   ((GetTotalCallCount() == 1) && (GetCurrentState() == CM_OUTGOING_STATE)))
	{
      switch (response)
	   {
		   case ERR_USER_BUSY:
		   case ERR_NORMAL_UNSPECIFIED:
		   case ERR_NO_USER_RESPONDING:
		   case ERR_DEST_OUT_OF_ORDER:
		   case ERR_NO_ANSWER_ON_ALERT:
		   case ERR_NO_CIRCUIT_CHANNEL_AVAIL:
		   case ERR_TEMPORARY_FAILURE:
		   case ERR_SWITCH_EQUIPMENT_CONGESTION:
		   case ERR_REQUESTED_CKT_CHANEL_NOT_AVIL:
		   case ERR_RESOURCE_UNAVAIL_UNSPECIFIED:
			   SetMaxAttempts(10);
			   result = TRUE;
			   break;

		   case ERR_UNASSIGNED_NUM:
		   case ERR_NO_ROUTE_TO_DESTINATION:
		   case ERR_NUMBER_CHANGED:
		   case ERR_INVALID_NUMBER_FORMAT:
		   case ERR_NETWORK_OUT_OF_ORDER:
			   SetMaxAttempts(1);
			   result = TRUE;
			   break;

		   default:
			   break;
	   }
	}
	if (result == FALSE)
	{
	   ResetRedialAttempts();
      return FALSE;
	}
	return TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  CheckRedialOrShowError
 * DESCRIPTION
 *  Checks if number must be redialed or error
 *  must be shown
 * PARAMETERS
 *  response        [IN]        
 *  Number(?)       [IN]        To show on screen
 * RETURNS
 *  void
 *****************************************************************************/
void CheckRedialOrShowError(U16 response)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    pBOOL result = TRUE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_CheckRedialOrShowError));

    mmi_trace(g_sw_CC,"MMI_Call: Func: %s response=%d", __FUNCTION__,response);
    result = CalculateSetBackOff();
    ShowCallManagementErrorMessage(response);
    if (result == TRUE)
        SetCurrentState(GetPreviousState());
    return;
}

/**************************************************************

	FUNCTION NAME		: ShowRedialScreen(U16 cause)

  	PURPOSE				: Timer callback after notification time expires

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShowRedialScreen(void)
{
	StopTimer(CM_NOTIFYDURATION_TIMER);
	cm_p->redial_info.ErrRedialScreen = FALSE;
	SetCurrentState(CM_IDLE_STATE);
	EntryScrShowUserTimeToRedial();
}


/*****************************************************************************
 * FUNCTION
 *  CalculateSetBackOff
 * DESCRIPTION
 *  Calculates attempt value and resets timer for redial
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
pBOOL CalculateSetBackOff(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    pBOOL minFlag = FALSE;
    U8 timer, dispTimer;
    S8 temp[10], temp2[20];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_TRACE((MMI_TRACE_G5_CM, MMI_CM_FUNC_CalculateSetBackOff));

    cm_p->redial_info.CurrentAttempt++;

    /* exceed redial attempt */
    if (cm_p->redial_info.CurrentAttempt > cm_p->redial_info.MaxAttempts)
    {
        ResetRedialAttempts();
        return FALSE;
    }
    switch (cm_p->redial_info.CurrentAttempt)
    {
        case 1:
            timer = 7;
            dispTimer = 5;
            break;

        case 2:
        case 3:
        case 4:
            timer = 65;
            dispTimer = 1;
            minFlag = TRUE;
            break;
        default:
            timer = 190;
            dispTimer = 3;
            minFlag = TRUE;
            break;
    }

    memset(gRedialMsgTemp, 0, sizeof(gRedialMsgTemp));
    pfnUnicodeStrcpy((PS8) gRedialMsgTemp, GetString(STR_CM_REDIAL));

	sprintf ((PS8)temp, "\n%d ", cm_p->redial_info.CurrentAttempt);
    AnsiiToUnicodeString((PS8) temp2, (PS8) temp);
    pfnUnicodeStrcat((PS8) gRedialMsgTemp, (PS8) temp2);

    pfnUnicodeStrcat((PS8) gRedialMsgTemp, GetString(STR_CM_REDIAL_TIMES));
    pfnUnicodeStrcat((PS8) gRedialMsgTemp, GetString(STR_CM_REDIAL_NEXT_ATTEMPT));

	sprintf ((PS8)temp, "\n%d ", dispTimer);
    AnsiiToUnicodeString((PS8) temp2, (PS8) temp);
    pfnUnicodeStrcat((PS8) gRedialMsgTemp, (PS8) temp2);

    if (minFlag == FALSE)
    {
        pfnUnicodeStrcat((PS8) gRedialMsgTemp, GetString(STR_CM_REDIAL_SEC));
    }
    else
    {
        pfnUnicodeStrcat((PS8) gRedialMsgTemp, GetString(STR_CM_REDIAL_MIN));
    }

    cm_p->redial_info.RedialTimer = TRUE;
    StartTimer(CM_REDIAL_TIMER, (timer * 1000), (FuncPtr) RedailAttemptNoArg);
    return TRUE;
}

/**************************************************************

	FUNCTION NAME		: RedailAttemptNoArg

  	PURPOSE				: Call back handler for timer expiry

	INPUT PARAMETERS	: flag

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
#ifdef __IP_NUMBER__
BOOL bUseIPForRedial = FALSE;
#endif
void RedailAttemptNoArg(void* info)
{
	U16 index;
	S8	TempMsgStruct[MAX_NUMBER];
	PHB_CM_INTERFACE *temp;

	StopTimer(CM_REDIAL_TIMER);
	cm_p->redial_info.RedialTimer = FALSE;	

	if (IsScreenPresent(SCR1001_CM_OUTGOINGCALLSCREEN) == TRUE)
	{
		DeleteNScrId(SCR1001_CM_OUTGOINGCALLSCREEN);
	}
	DeleteScreenIfPresent(CM_SCR_RDL_TIME);
		if (IsScreenPresent(CM_SCR_MARKER) == FALSE)
		{
			AddMarkerToHistory();
		}
	
	SetPreviousState(CM_IDLE_STATE);
	SetCurrentState(CM_OUTGOING_STATE);

#ifdef __IP_NUMBER__	
	if (bUseIPForRedial)
		SetTempUseIPNumber(TRUE);
#endif

   temp = GetCMPhoneBookStruct();
   if (temp->dialInList != MMI_PHB_NONE)
      cm_p->redial_info.DialFromPhb = temp->dialInList;

	index=GetFirstFreeIndex();

	AddNewCallInfo((U8*)cm_p->redial_info.RedialNum,(CALL_STATE)GetCurrentState(),CM_IDLE_STATE,
	               CM_CALL_MO, (CM_CALL_HANDLE)(index+1), CM_VOICE_CALL);

	UnicodeToAnsii(TempMsgStruct,(PS8)cm_p->redial_info.RedialNum);
	MakePsInitiateCall((PU8)TempMsgStruct,(void*)OutgoingCallConnected);

   return;
}

/**************************************************************

	FUNCTION NAME		: GetCurrentAttempt

  	PURPOSE				: To get flag to show redial screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
U8 GetCurrentAttempt(void)
{
	return cm_p->redial_info.CurrentAttempt;
}


