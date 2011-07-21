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
 *   OutgoingCallHelper.c
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
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/CallManagement/src/OutgoingCallHelper.c-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:57:00   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:37:28   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:23:46   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Nov 06 2006 17:32:54   zhangxb
 * Initial revision.
 * 
 *    Rev 1.1   Sep 19 2006 19:10:52   zhoumn
 * No change.
 * 
 *    Rev 1.0   Jul 06 2006 17:42:14   zhangxb
 * Initial revision.
 * 
 *    Rev 1.0   Jul 04 2006 22:57:08   zhangx
 *
 *******************************************************************************/

/**************************************************************

	FILENAME	: OutgoingCallHelper.c

  	PURPOSE		: Call Management application

 

	AUTHOR		: Subramanya BG

	DATE		: Nov 12,02

**************************************************************/
#define __NEWSIMULATOR

#include "stdc.h"
#include "mmi_features.h"
#include "l4dr.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalconstants.h"
#include "globalscrenum.h"
#include "debuginitdef.h"
#include "protocolevents.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "commonscreens.h"
#include "wgui.h"
#include "wgui_status_icons.h"
#include "wgui_categories_cm.h"
#include "globaldefs.h"
#include "timerevents.h"

#include "bootup.h"
#include "poweronchargerprot.h"
#include "phonebookdef.h"
#include "phonebooktypes.h"
#include "phonebookgprot.h"
#include "callsetupenum.h"
#include "simdetectiongexdcl.h"
#include "alarmframeworkprot.h"

#include "callmanagementiddef.h"
#include "callmanagementstruct.h"
#include "callstructuremanagementprot.h"
#include "cmerrortypes.h"
#include "cmerrorhandlersprot.h"
#include "commonstubsprot.h"
#include "historyhelperprot.h"
#include "pscallhelperdef.h"
#include "pscallhelpergprot.h"
#include "redialhelpergprot.h"
#include "callmanagementgprot.h"
#include "incomingcallmanagementprot.h"
#include "outgoingcallmanagementprot.h"

#include "callsgprots.h"

#include "outgoingcallhelpergprot.h"
#include "outgoingcallhelperprot.h"
#include "gpioinc.h"

#include "settingdefs.h"
#include "settinggenum.h"//man
#include "securitysetup.h"
#include "settingsgdcl.h"//man
#include "mmi_trace.h"//man
#include "callsetup.h"//man
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
#endif
#include "simdetectiongprot.h"
#undef __NEWSIMULATOR

static CM_CALL_HANDLE gMyhandle;
// static U16 curKeyCode, curKeyType;
static pBOOL gCheckDialIndEndOutgoing =FALSE;

S8 DTMFPadCallBuffer[MAX_DIGITS_USSD];
S8 UnicodeDTMFPadCallBuffer[MAX_DIGITS_USSD* ENCODING_LENGTH];
PU8 gCallDeflectNumber;
extern SecuritySetupContext g_SecuritySetupContext;
extern FDLBDLContext gFDLBDLContext[];
extern FDLBDLContext *gpFDLBDLContext;
extern U16	gtmpOutgoingIndex;
#ifdef __MMI_SUPPORT_JME__
extern s8 vm_status;
#endif
extern void SSCHandleIMEI(void);
extern void  aud_speech_set_mode(kal_bool on);

const U16 CMIdleScreenDigits[MAX_CM_SCREEN_DIGITS] =
{
		KEY_0,KEY_1,KEY_2,KEY_3,
		KEY_4,KEY_5,KEY_6,KEY_7,
		KEY_8,KEY_9,KEY_STAR,
		KEY_POUND
};

#ifdef __MMI_MULTI_SIM__
BOOL  AllowFixedDial(PS8 strNumber, UINT8 nSimId)//man for fdn
#else
BOOL  AllowFixedDial(PS8 strNumber)//man for fdn
#endif
{
	int i = 0;
	BOOL result = FALSE;
	#ifndef __MMI_MULTI_SIM__
	UINT8 nSimId = 0;
	#endif
	
	if(g_SecuritySetupContext.FdlStatus[nSimId])
	{
	 	for(i = 0; i <  gFDLBDLContext[nSimId].FDNBDNCount; i++)
		{
			if(!pfnUnicodeStrcmp(gFDLBDLContext[nSimId].FDLBDLEntries[i].tel.number,strNumber))
			{
		 	       result = TRUE;
				break;
			}
		}
	}
	else
	{
		result = TRUE;
	}	

	return result ;
	
}

/**************************************************************

	FUNCTION NAME		: MakeCall(PS8 strNumber)

  	PURPOSE				: Wrapper which checks to see if the call to be made is
						  O/G or USSD

	INPUT PARAMETERS	: PS8 - Number which needs to be sent to network

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
#ifdef MAGIC_AE_SUPPORT
extern void CallsetSetMagicSound(U8 state);
#endif
#ifdef __MMI_AUTO_PLUS_ENABLE__
extern void CALLSETsetAutoPlus(U8 state);
#endif
void MakeCall(PS8 strNumber)
{

	if(pfnUnicodeStrlen((PS8)strNumber)== 0)
	{
	}


	if( !CHECK_MO_ALLOW() )
	{
		 
		ChgrPlayLowBatTone();
		BatteryIndicationPopup(STR_LOW_BATTERY_MT_PROHIBIT);
		return;
	}

	if( IS_LOW_BATTERY() )
	{
		 
		ChgrPlayLowBatTone();
		if ( g_charbat_context.LowBatteryTxProhibitFlag )
			SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_TALK_DURCNT_PROHIBIT );
		else
			SET_LOW_BATTERY_ALARM_THRESHOLD( LBAT_TALK_DURCNT_NORMAL );
	}

#ifdef __MMI_MULTI_SIM__
	if (MTPNP_AD_CHANNEL_NONE == MTPNP_AD_Get_Channel(MTPNP_AD_CALL_CHANNEL))
	{
		mmi_trace(g_sw_CC,"Call MO Func: %s Set CC channel default master", __FUNCTION__);
		
		MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_DEFAULT_MASTER);
	}
	else
	{
		mmi_trace(g_sw_CC,"MMI_Call: Func: %s CC channel: %d", __FUNCTION__, MTPNP_AD_Get_Channel(MTPNP_AD_CALL_CHANNEL));
	}
#endif

	SetNoSave();


	if(strNumber[0]=='*'&& (1 == pfnUnicodeStrlen(strNumber))) 
	{
		SetCallDeflectFlag(FALSE);
		ShowCallManagementErrorMessage(ERR_INVALID_NUMBER_FORMAT);	
		return;
	}

	if(NULL==strNumber)
	{
		SetCallDeflectFlag(FALSE);
		ShowCallManagementErrorMessage(ERR_INVALID_NUMBER_FORMAT);
		return;
	}
	else if(0==*strNumber)
	{
		SetCallDeflectFlag(FALSE);
		ShowCallManagementErrorMessage(ERR_INVALID_NUMBER_FORMAT);
		return;
	}

	if(UCS2StrFirstPositionSearch(strNumber)==FALSE)
	{
		SetCallDeflectFlag(FALSE);
		ShowCallManagementErrorMessage(ERR_INVALID_NUMBER_FORMAT);
		return;
	}
	if(UCS2StrNoFirstPositionSearch(strNumber,'p')==FALSE)
	{
		SetCallDeflectFlag(FALSE);
		ShowCallManagementErrorMessage(ERR_INVALID_NUMBER_FORMAT);
		return;
	}
	if(UCS2StrNoFirstPositionSearch(strNumber,'w')==FALSE)
	{
		SetCallDeflectFlag(FALSE);
		ShowCallManagementErrorMessage(ERR_INVALID_NUMBER_FORMAT);
		return;
	}

	#ifdef MAGIC_AE_SUPPORT
	CallsetSetMagicSound(g_callset_cntx_p->MagicSoundState);
	#endif

	#ifdef __MMI_AUTO_PLUS_ENABLE__
	CALLSETsetAutoPlus(g_callset_cntx_p->AutoPlusState);
	#endif
	
	FillDtmfBuff((PU8)strNumber);



	CheckShortCutOrCall();
}

/**************************************************************

	FUNCTION NAME		: CheckShortCutOrCall

  	PURPOSE				: Helper function to check if the key is shortcut or not
						  if not it makes a call

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void CheckShortCutOrCall(void)
{
	#ifdef __MMI_MULTI_SIM__
	U8 callSimID;
	#endif
	
	PU8 dtmfBuf;
	SetNoSave();
	dtmfBuf = GetDtmfBuffer();

	mmi_trace(1,"CheckShortCutOrCall");


	if(GetCallDeflectFlag()==TRUE)
	{
		PRINT_INFORMATION (("XXX Call Deflection\n"));
		SetCallDeflectFlag(FALSE);
		OutgoingProcessCMEvent(CM_KB_CALL_DEFLECT_REQ, dtmfBuf);
		return;
	}


    if (IsStandardGsmSS(dtmfBuf))
	{
	   return;
	}
	else if (IsSimControlProcedure(dtmfBuf) == TRUE)
	{
		PRINT_INFORMATION (("XXX Sim Specifc Control\n"));
		return;
	}
	else if (IsManufactuerSpecific(dtmfBuf) == TRUE)
	{
		PRINT_INFORMATION (("XXX Manufacture Specific Control \n"));
		return;
	}

	if (CheckValidEmergencyNo1(UnicodeDTMFPadCallBuffer))
	{
		MakeMyCall((PS8)dtmfBuf);
		return;
	}	
#ifdef __MMI_MULTI_SIM__
	callSimID = MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL);
	if( !AllowFixedDial(UnicodeDTMFPadCallBuffer, callSimID) )
	{
		SetCallDeflectFlag(FALSE);
		ShowCallManagementErrorMessage(ERR_L4C_FDN_FAIL);	
		return;
	}
#else
	if( !AllowFixedDial(UnicodeDTMFPadCallBuffer) )
	{
		SetCallDeflectFlag(FALSE);
		ShowCallManagementErrorMessage(ERR_L4C_FDN_FAIL);	
		return;
	}		
#endif

#ifdef __MMI_MULTI_SIM__
	if(GetCallSetLine(callSimID) == LINE2) 
#else
	if(g_callset_cntx_p->LineID == LINE2) 
#endif
	{
		SetCallDeflectFlag(FALSE);
		ShowCallManagementErrorMessage(ERR_PLMN_NOT_ALLOWED);	
		return;
	}

	if (GetCurrentState() == CM_INCOMING_STATE)// when in incoming state, barred dial call  bug 13948
	{
		DisplayPopup((PU8)GetString(STR_MENU9168_BARRED_DAIL), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		return;
	}

	if (isHashString(dtmfBuf))
	{
		MakeMyCall((PS8)dtmfBuf);
	}
	else if (isShortString(dtmfBuf))
	{
		MakeMyCall((PS8)dtmfBuf);
	}
	else// if (AllowMoreCalls())
	{
		MakeMyCall((PS8)dtmfBuf);
	}
	/*else
	{
		ShowCallManagementErrorMessage(ERR_CSMCC_CMD_NOT_ALLOW);
	}*/

//	ClearDtmfBuf();
}

/*****************************************************************************
 * FUNCTION
 *  MakeMyCall
 * DESCRIPTION
 *  Helper function to make a outgoingcall of a given number.
 *  
 *  This converts the number to OUTGOING_CALL structure which is necessary for State m/c.
 * PARAMETERS
 *  strNumber       [IN]        
 *  Number(?)       [IN]        Which needs to make the outgoing call.
 * RETURNS
 *  void
 *****************************************************************************/
void MakeMyCall(PS8 strNumber)
{
	OUTGOING_CALL outCall;
	mmi_trace(1,"MakeMyCall");
	memset (&outCall, 0, sizeof (OUTGOING_CALL));
	if((strlen((void*)strNumber) * ENCODING_LENGTH) > MAX_CM_NUMBER)
	{
		PRINT_INFORMATION (("XXX Number too long XXX \n"));
		ShowCallManagementErrorMessage(ERR_INVALID_NUMBER_FORMAT);
		return;
	}
	pfnUnicodeStrcpy((void*)outCall.Number,(void*)UnicodeDTMFPadCallBuffer);
	MakeOutgoingcall(outCall);
}

/**************************************************************

	FUNCTION NAME		: MakeOutgoingcall(OUTGOING_CALL MsgStruct)

  	PURPOSE				: Helper function to generate Keybrd Event CM_KB_OUTGOINGCALL with the OUTGOING_CALL structure.

	INPUT PARAMETERS	: OUTGOING_CALL structure contains the calling number and name if present corresponding to that number.

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakeOutgoingcall(OUTGOING_CALL MsgStruct)
{
	OutgoingProcessCMEvent(CM_KB_OUTGOINGCALL,&MsgStruct);
}


/*****************************************************************************
 * FUNCTION
 *  IsStandardGsmSS
 * DESCRIPTION
 *  Helper function for interpreting user code
 * PARAMETERS
 *  dtmfBuf         [IN]        
 *  true(?)         [OUT]       Or false on the basis of interepretaion of SS
 *  buffer(?)       [IN]        For interpretation
 * RETURNS
 *  pBOOL
 *****************************************************************************/
pBOOL IsStandardGsmSS(PU8 dtmfBuf)
{
	PU8 tmpBuf = dtmfBuf;
	CM_CALL_HANDLE callVal;
	S32 bufLen;
	pBOOL valid = FALSE;

	if (GetTotalCallCount() == 0)
	{
		return FALSE;
	}

	/* second digit must be NULL, *, or 0~9 */
	if ((*(tmpBuf+1) == 0) || (*(tmpBuf+1) == '*') ||
		((*(tmpBuf+1) >= '0') && (*(tmpBuf+1) <= '9')))
	   valid = TRUE;

	if (valid == TRUE)
	{
	switch (*tmpBuf)
	{
		case '0':
			if ('\0' == *(tmpBuf+1))
			{
				/* 0 send */
				if (-1!=GetIncomingCallHandle())
				{
					/* incoming call present => reject waiting call */
					if (GetTotalCallCount() > 1)
					{
						PRINT_INFORMATION (("\n *** Reject Waiting Call\n"));
						KbCBackCallIncomingRejected();
						return TRUE;
					}else
					{
						valid = TRUE;
						break;
					}
				}
				else
				{
					/* no incoming call, drop held call */
					if((GetTotalHoldCallCount() > 0) ||
					  ((GetTotalActiveCallCount() > 0) && (GetCurrentState()==CM_OUTGOING_STATE)))
					{
						PRINT_INFORMATION (("\n *** Hanging Up All Hold\n"));
						HangupallHeld();
						return TRUE;
					}
					else
					{
						valid = TRUE;
						break;
					}
				}
			}
			valid = FALSE;
			break;

		case '1':
			if ('\0' == *(tmpBuf + 1))
			{
				/* 1 send */
				if (GetCurrentState() == CM_INCOMING_STATE)
				{
					if(GetTotalCallCount() == 1)
					{
						valid = TRUE;
						break;
					}
					else
					{
						KbCBackEndActiveAcceptIncomingCall();
						return TRUE;
					}
				}
				else if (GetCurrentState() == CM_OUTGOING_STATE)
				{
					valid = TRUE;
					break;
				}
				else
				{
					if (GetTotalCallCount() > 0)
					{
						ProcessIncomingEvents (CM_KB_END_ALL_ACTIVE_RETRIEVE_HELD,NULL);
						return TRUE;
					}
					else
					{
						valid = TRUE;
						break;
					}
				}
			}
			else if ('\0' == *(tmpBuf + 2))
			{
				/* 1X send */
				PRINT_INFORMATION (("\n *** Hanging Up Active call \n"));

				callVal = (CM_CALL_HANDLE)atoi ((void*)(tmpBuf + 1));

            if (callVal != 0 && GetCallState(callVal) == CM_ACTIVE_STATE &&
               GetCurrentState() != CM_OUTGOING_STATE)
            {
            	HangupCall(callVal);
					return TRUE;
            }
            else
				{
					valid = TRUE;
					break;
				}
			}
			valid = FALSE;
			break;

		case '2':
			if ('\0' == *(tmpBuf + 1))
			{
				/* 2 send */
					if (GetWapCallPresent())
					{
						valid = TRUE;
						break;
					}
				if(CM_INCOMING_STATE==GetCurrentState())
					{
						KbCBackCallIncomingAccepted();
						return TRUE;
					}
				else if (GetCurrentState() == CM_OUTGOING_STATE)
				{
					valid = TRUE;
					break;
				}

				OutgoingProcessCMEvent(CM_KB_SWAPREQ, NULL);
				return TRUE;
			}
			else if ('\0' == *(tmpBuf + 2))
			{
				/* 2X send */
				callVal = (CM_CALL_HANDLE)atoi ((void*)(tmpBuf + 1));

				if (callVal==0 || (GetCallState(callVal)!=CM_ACTIVE_STATE))
				{
					valid = TRUE;
					break;
				}
				if ((GetTotalActiveCallCount() > 1) &&
				    (GetTotalHoldCallCount() == 0) &&
				    (GetCurrentState() != CM_OUTGOING_STATE))
				{
					PRINT_INFORMATION (("\n *** Splitting Call \n"));
					SplitCall(callVal);
					return TRUE;
				}
				else
				{
					valid = TRUE;
					break;
				}
			}
			valid = FALSE;
			break;

		case '3':
			if ('\0' == *(tmpBuf + 1))
			{
				/* 3 send */
				if ((GetTotalActiveCallCount() < MAX_HOLD) &&
				   (GetTotalHoldCallCount() < MAX_HOLD) &&
				   ((GetTotalActiveCallCount() > 0 && GetTotalHoldCallCount() > 0)))
				{
					PRINT_INFORMATION (("\n *** Conferencing Call \n"));
					ConferenceCall();
					return TRUE;
				}
				else
				{
					valid = TRUE;
					break;
				}
			}
			valid = FALSE;
			break;

		case '4':
			if ('*' == *(tmpBuf + 1))
			{
				/* 4* send */
				if((-1!=GetIncomingCallHandle()))
				{
					bufLen=strlen((PS8)tmpBuf);
					if(tmpBuf[bufLen-1]>='0' && tmpBuf[bufLen-1]<='9')
					{
						PRINT_INFORMATION (("\n *** Call Deflection\n"));
						gCallDeflectNumber=tmpBuf+2;
						OutgoingProcessCMEvent(CM_KB_CALL_DEFLECT_REQ, gCallDeflectNumber);
						return TRUE;
					}
				}
				else
				{
					PRINT_INFORMATION (("\n *** Not a valid action \n"));
					ShowCallManagementErrorMessage(NO_ACTIVE_CALL);
					return TRUE;
				}
			}
			else if ('\0' == *(tmpBuf + 1))
			{
				/* 4 send */
				if((GetTotalActiveCallCount()==1&&(-1 != GetOutgoingCallHandle())) ||
				   (GetTotalHoldCallCount()==1&&(-1!=GetOutgoingCallHandle())) ||
				   (GetTotalActiveCallCount() == 1 && GetTotalHoldCallCount() == 1))
				{
					PRINT_INFORMATION (("\n *** Explicit Call Transfer\n"));
					TransferCall();
					return TRUE;
				}
				else
				{
					valid = TRUE;
					break;
				}
			}
			valid = FALSE;
			break;

		case '5':
			if ('\0' == *(tmpBuf + 1))
			{
				/* 5 send */
				if(GetCCBSFlag() ==TRUE)
				{
					PRINT_INFORMATION (("\n *** CCBS Activation\n"));
					ActivateCCBS();
					return TRUE;
				}
				else
				{
					valid = TRUE;
					break;
				}
			}
			valid = FALSE;
			break;

		default:
			valid = FALSE;
			break;

	}
	}

	if (GetTotalCallCount() == 1 && GetCurrentState() == CM_INCOMING_STATE)
	{
		/* answer */
		KbCBackCallIncomingAccepted();
		//ProcessIncomingEvents (CM_KB_INCOMING_CALL_ACCEPT,NULL);
		return TRUE;
	}
	else if (valid == TRUE)
	{
		PRINT_INFORMATION (("\n Not a valid action\n"));
		ShowCallManagementErrorMessage(NO_ACTIVE_CALL);
		return TRUE;
	}
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: isHashString

  	PURPOSE				: Helper function to check is this is a # string

	INPUT PARAMETERS	: Buffer containing USSD string

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
pBOOL isHashString(PU8 buf)
{
	U16 count=0;

	while ('\0' != buf[count])
	{
		if ('#' == buf[count])
			return TRUE;
		if (count >= MAX_DIGIT)
			return FALSE;
		count++;

	}
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: isShortString

  	PURPOSE				: Helper function to check is this is a short string

	INPUT PARAMETERS	: Buffer containing USSD string

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
pBOOL isShortString(PU8 buf)
{
	U16 count=0;

	while ('\0' != buf[count++])
		if (count > 2)
			return FALSE;

	return TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  UCS2StrNoFirstPositionSearch
 * DESCRIPTION
 *  Search for a character in string for only first position
 * PARAMETERS
 *  strNumber           [IN]        
 *  charToSearch        [IN]        
 * RETURNS
 *  pBOOL
 *****************************************************************************/
pBOOL UCS2StrNoFirstPositionSearch(PS8 strNumber, S8 charToSearch)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 pos;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    PRINT_INFORMATION(("\nIn UCS2StrNoFirstPositionSearch\n"));

    if (strNumber[0] == charToSearch)
    {
        return FALSE;
    }
    else
    {
        pos = 2;
        while (strNumber[pos] != '\0')
        {
            if (strNumber[pos] == charToSearch)
            {
				if (strNumber[pos+2] != '*' && strNumber[pos+2] != '#')
                    return TRUE;
				else
				    return FALSE;
            }
            pos = pos + 2;
        }
        return TRUE;
    }
}

/**************************************************************

	FUNCTION NAME		: UCS2StrFirstPositionSearch(PS8 strNumber)

  	PURPOSE				: Search for a '+' character in string for only first position

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 
**************************************************************/
pBOOL UCS2StrFirstPositionSearch(PS8 strNumber)
{
	U16 pos;
	//pBOOL firstFlag=FALSE;
	pBOOL secondFlag=FALSE;
	S8 charToSearch='+';

	PRINT_INFORMATION(("\nIn UCS2StrFirstPositionSearch\n"));

	//if(strNumber[0]==charToSearch)
	//	firstFlag=TRUE;

    pos = 2;
    while (strNumber[pos] != '\0')
    {
        if ((strNumber[pos] == charToSearch) && 
            ((strNumber[pos - 2] != '#' && strNumber[pos - 2] != '*') ||
                strNumber[0] == charToSearch))
        {
            secondFlag = TRUE;
            break;
        }
        pos = pos + 2;
    }

	if(secondFlag==FALSE)
		return TRUE;
	else
		return FALSE;
}

/**************************************************************

	FUNCTION NAME		: ShowOutgoingIndicationScreen

  	PURPOSE				: Shows only outgoing call screen and takes control

	INPUT PARAMETERS	: Number to show on screen

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void ShowOutgoingIndicationScreen(PU8 number)
{
	/* for SAT MO only  */

	SetSATScrnSavedFlag(FALSE);

	SetPreviousState((CALL_STATE)GetCurrentState());
	SetCurrentState(CM_OUTGOING_STATE);

    gtmpOutgoingIndex = GetFirstFreeIndex();
    AddNewCallInfo(
        number,
        (CALL_STATE) GetCurrentState(),
        CM_IDLE_STATE,
        CM_CALL_MO_STK,
        (CM_CALL_HANDLE) (gtmpOutgoingIndex + 1),
        CSMCC_VOICE_CALL);

	OutgoingProcessCMEvent(CM_PS_SHOW_OUTGOING_CALL_SCREEN,NULL);
	SetSATScrnSavedFlag(TRUE);
}

/**************************************************************

	FUNCTION NAME		: RemoveOutgoingIndicationScreen

  	PURPOSE				: Removes outgoing call screen

	INPUT PARAMETERS	: Number to show on screen

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void RemoveOutgoingIndicationScreen(void)
{
	CM_CALL_HANDLE handle;

	/* SAT MO fail case */
	handle = GetOutgoingCallHandle();
	if (handle == -1)
    {
        return;
    }

	SetCallState(handle,CM_IDLE_STATE,TRUE);

	if(FALSE==GetSATScrnSavedFlag())
	{
		EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
		SetSATScrnSavedFlag(TRUE);
	}
	SetCallEndedBeforeConnFlag(FALSE);
	GoBackfromNotifyScr();
}

/**************************************************************

	FUNCTION NAME		: PsCSetCallFeatures(void*)

  	PURPOSE				: This Sets the features of the call as per
						  PARSING STR RSP

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackSetCallFeatures(void* info)
{
	MMI_PARSE_STR_RSP *rsp;
	CM_CALL_HANDLE handle = -1;


	rsp = (MMI_PARSE_STR_RSP*)info;
	mmi_trace(g_sw_CC,"Call MO In PsCBackSetCallFeatures rsp->info.opcode = %d\n",rsp->info.opcode );

	if(TRUE==DeriveAndSetCallFeatures(info))
	{
		if (GetCCFlag() == TRUE)
		{
			PHB_CM_INTERFACE PhoneNumberStruct;

			/* MO call */
			/* look up dialed number from phonebook first anyway */

			/* auto redial and dial from list */
			if (cm_p->redial_info.DialFromPhb != MMI_PHB_NONE)
				mmi_phb_call_set_dial_from_list(cm_p->redial_info.DialFromPhb);

#ifdef __MMI_MULTI_SIM__
			PhoneNumberStruct = mmi_phb_call_get_data_for_call_mgnt(GetOutgoingNumber(), MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL));
#else
			PhoneNumberStruct = mmi_phb_call_get_data_for_call_mgnt(GetOutgoingNumber());
#endif		

			SetCMPhoneBookStruct (&PhoneNumberStruct);
			//PhoneNumberStruct = GetCMPhoneBookStruct();

			if (CHISTGetDialFromCallLogFlag() == 0)
			{
				/* not dial from call log => use phonebook data and name */
				mmi_trace(g_sw_CC,"MMI_Call *** In PsCBackSetCallFeatures, not from call log, set MO name ***\n");
				SetOutgoingNamefromPhonebook();
			}
			else if (pfnUnicodeStrcmp((PS8)PhoneNumberStruct.name, (PS8)GetOutgoingName()) != 0)
			{
				/* dial from call log but phonebook name not match => don't use phonebook data */
				mmi_trace(g_sw_CC,"MMI_Call *** In PsCBackSetCallFeatures, dial from call log but phonebook name not match\n");
				memset (&PhoneNumberStruct, 0, sizeof (PHB_CM_INTERFACE));
				PhoneNumberStruct.pictureId = IMG_PHB_DEFAULT;
				SetCMPhoneBookStruct(&PhoneNumberStruct);
			}
			else
			{
				mmi_trace(g_sw_CC,"MMI_Call *** In PsCBackSetCallFeatures, dial from call log but still use phonebook data\n");
				/* dial from call log but still use phonebook data */
			}

			AlmDisableExpiryHandler();
			OutgoingProcessCMEvent(CM_PS_SHOW_OUTGOING_CALL_SCREEN,NULL);
			CHISTSetDialFromCallLogFlag (0);   
			return;
		}
		else if (rsp->info.opcode == SS_OPERATION)
		{
            /* SS string */
            if (GetCurrentState() == CM_INCOMING_STATE)
            {
                history_t ss_scr;
                S16 nHistory = 0;

                ss_scr.scrnID = SCR_CM_REQUESTINGUSSD;
                ss_scr.entryFuncPtr = EntryScrUSSDReq;
                pfnUnicodeStrcpy((S8*) ss_scr.inputBuffer, (S8*) & nHistory);
                InsertHistoryBeforeThisScrnReference(CM_SCR_MARKER, &ss_scr);
            }
            else
            {
                OutgoingProcessCMEvent(CM_PS_SHOW_REQUESTING_CALL_SCREEN, NULL);
            }
        }
		else 
		{
			/* must be SIM operation */
			//OutgoingProcessCMFailureEvent(CM_PS_CALLCONNECTED, ERR_INVALID_NUMBER_FORMAT);
		}
	}
	else if ((rsp->result.flag == 0) &&  (rsp->info.opcode == SIM_OPERATION))
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_DONE), IMG_GLOBAL_UNFINISHED, 0,
		             CS_NOTIFYDURATION, ERROR_TONE);
	}
	else if ((rsp->result.flag == 0) && (rsp->info.opcode == INVALID_OPERATION))
	{
        if (rsp->result.cause == PHB_L4C_ERROR)
        {
            OutgoingProcessCMFailureEvent(CM_PS_CALLCONNECTED, ERR_L4C_FDN_FAIL);
        }
        else
        {
            OutgoingProcessCMFailureEvent(CM_PS_CALLCONNECTED, ERR_INVALID_OPERATION);
        }
    }
	else
	{
	   OutgoingProcessCMFailureEvent(CM_PS_CALLCONNECTED, ERROR_FAIL);
 	   RemoveSSReqScr();
	}

#ifdef __IP_NUMBER__
    SetTempUseIPNumber(FALSE);
#endif 
	g_phb_cntx.dial_from_list = MMI_PHB_NONE;
	PurgeOutgoingCallsStructure();
	SetCallEndedBeforeConnFlag(FALSE);
	CHISTSetDialFromCallLogFlag (0);   

	SetCurrentState (GetPreviousState());
	if (GetPreviousState() == CM_INCOMING_STATE)
	{
        if (GetTotalCallCount() == 1)
        {
            SetPreviousState(CM_IDLE_STATE);
        }
        else if (GetTotalActiveCallCount() == GetTotalCallCount() - 1)
        {
            SetPreviousState(CM_ACTIVE_STATE);
        }
        else if (GetTotalHoldCallCount() == GetTotalCallCount() - 1)
        {
            SetPreviousState(CM_HOLD_STATE);
        }
    }

	// reset phb data
	if (GetCurrentState() == CM_INCOMING_STATE)
	{
		handle = GetIncomingCallHandle();		
	}
	else if (GetCurrentState() == CM_OUTGOING_STATE)
	{
		handle = GetOutgoingCallHandle();
	}
	if (handle != -1)
	{
		PHB_CM_INTERFACE phb_data;

#ifdef __MMI_MULTI_SIM__
	phb_data = mmi_phb_call_get_data_for_call_mgnt(GetCallNumber(handle), MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_CALL_CHANNEL));
#else
	phb_data = mmi_phb_call_get_data_for_call_mgnt(GetCallNumber(handle));
#endif	

#ifdef __MMI_INCOMING_CALL_VIDEO__
      /* waiting call and ccbs invoke can't use video for display */
#ifdef  __MMI_SUPPORT_JME__
      if ((GetTotalCallCount() > 0 || GetCCBSFlag() == TRUE)  || (vm_status))
#else
      if (GetTotalCallCount() > 0 || GetCCBSFlag() == TRUE)
#endif
      {
       	phb_data.videoId = 0;
      }
#endif
		SetCMPhoneBookStruct(&phb_data);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: PsCbackOutgoingCallIdSync(void* MsgStruct)

  	PURPOSE				: Helper function to get call list from PS to sync with MMI

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void PsCbackOutgoingCallIdSync(void* MsgStruct)
{
	CM_CALL_HANDLE handle;
	U16 response;

	PRINT_INFORMATION (("XXX - In PsCbackOutgoingCallIdSync XXX\n"));

	handle = DeriveOutgoingCallIdRsp(MsgStruct, &response);
	if (handle == -1 || handle > MAX_CALLS)
	{
		if (GetDialIndication() == TRUE)
		{
			ShowCallManagementErrorMessage(NO_ACTIVE_CALL);
			return;
		}

		PRINT_INFORMATION(("Error in making outgoing call %d", response));
		/* keep MO fail cause for later display if notify ss is ongoing */
		SetCallEndCause(response);

		/* MO fail */
		//SyncCallList();
		SetDialIndication(FALSE);

		LogCallInfoForCallHistory(GetOutgoingCallHandle());
		GetEndTimeAndLogUnconnectedMOCall();

		if (IsRedialSet())
        {
            SetRedialNumber();
        }

		OutgoingProcessCMFailureEvent(CM_PS_CALLCONNECTED, response);
		// reset call end before conn flag for next call
		SetCallEndedBeforeConnFlag(FALSE);
	}
	else
	{
		SetOutgoingCallHandle (handle);
		SetDialIndication(TRUE);
		if (GetTotalCallCount()>1)
        {
            SyncCallList();
        }
    }
}

/**************************************************************

	FUNCTION NAME		: OutgoingCallConnected(void* MsgStruct)

  	PURPOSE				: Helper function to Generate CM known Stack Event CM_PS_CALLCONNECTED
						  with the Structure containing CM_CALL_HANDLE.

	INPUT PARAMETERS	: void*(This contains CM_CALL_HANDLE passed by the stack when call is connected)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
							  state m/c. CONNECT_IND from PS
**************************************************************/
void OutgoingCallConnected(void* MsgStruct)
{
	MMI_CONNECT_IND *msg;
	CM_CALL_HANDLE handle;

	msg=(MMI_CONNECT_IND*)MsgStruct;

	PRINT_INFORMATION (("Call MO - In OutgoingCallConnected XXX\n"));

	if (GetInternalAlert() == TRUE)
	{
		StopInternalOutgoingTone();
	}

	SetAlertingFlag(FALSE);

	if(CM_CSD_CALL==(msg->call_type))
	{
		SetWapCallPresent(TRUE);
	}

   if (GetTotalCallCount() <= 1)
   {
#ifdef __MMI_CONNECT_NOTICE__
		/* play connect notice according to user defined style */
      PlayConnectNotice();
#else
	   playRequestedTone(CONNECT_TONE);
#endif
   }

	if(GetOutgoingCallOrigin() == CM_CALL_MO_STK)
	{
		/* STK call doesn't have DIAL_IND, so need to set call id in CONNECT_IND */
		SetOutgoingCallHandle (msg->call_id);
	}

	handle = GetOutgoingCallHandle();
	/* set MO number in case it's changed by PS */
	SetCalledNumWithType(GetMMIStructIndexof(handle),MsgStruct);
#ifdef __IP_NUMBER__
	SetTempUseIPNumber(FALSE);
#endif
	OutgoingProcessCMEvent(CM_PS_CALLCONNECTED,&handle);
}

/**************************************************************

	FUNCTION NAME		: PsCBackOutgoingCallEnded(void* MsgStruct)

  	PURPOSE				: Call Back function for ATH_SUCCESS to hangup outgoing call

	INPUT PARAMETERS	: expected NULL

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void PsCBackOutgoingCallEnded(void* MsgStruct)
{
	CM_CALL_HANDLE handle;

   SetCallAbortReqSentFlag(FALSE);

   if(GetCurrentState() == CM_IDLE_STATE)
   {
      PRINT_INFORMATION (("XXX - In PsCBackOutgoingCallEnded no call to drop XXX\n"));
      return;
   }

	if ((handle = GetOutgoingCallHandle()) == -1)
	{
		/* MO call already got connected before it's aborted */
      handle = GetHungUpHandle();
		if (handle == -1)
		{
			PRINT_INFORMATION (("XXX - In PsCBackOutgoingCallEnded call already dropped XXX\n"));
      		return;
		}
	}

	OutgoingProcessCMEvent(CM_PS_HANGUPSUC,(void*)&handle);
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
}

/**************************************************************

	FUNCTION NAME		: PsCBackUSSDResponse(void*)

  	PURPOSE				: This function is called back by the protocol stack when USSD data is recieved by n/w

	INPUT PARAMETERS	: void*

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PsCBackUSSDResponse(void* info)
{
    PRINT_INFORMATION (("XXX - In PsCBackUSSDResponse XXX\n"));
    OutgoingProcessCMEvent(CM_PS_USSD_RECVD, info);
    //ProcessIncomingEvents (CM_PS_USSD_RECVD, info);
}
 
/**************************************************************

	FUNCTION NAME		: ReleaseCall

  	PURPOSE				: Invokes the helper function to make the disconnecting call released complete

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ReleaseCall(void)
{
   	U8 count;
   	CM_CALL_HANDLE handle;
   	
   	for (count=0; count<MAX_CALLS; count++)
   	{
		if (cm_p->state_info.AllCalls[count].curr_state == CM_DISCONNECTING_STATE)
		{
			handle = cm_p->state_info.AllCalls[count].call_handle;
			MakePsReleaseCompleteRequest(handle);
			OutgoingProcessCMEvent(CM_PS_HANGUPSUC, (void *)&handle);
			if (GetAutoReleaseFlag() == FALSE)
				return;
		}
   	}
   	SetAutoReleaseFlag(FALSE);
	aud_speech_set_mode(KAL_FALSE); //chenhe,070808. for bug #6100,CM_KB_OUTGOINGCALL is been sent,so have to do so here
}

 
/**************************************************************

	FUNCTION NAME		: DropRequest(void)

  	PURPOSE				: Invokes the helper function to hangup or disconnect the call
						  which is yet to connect.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DropRequest(void)
{
   	CM_CALL_HANDLE handle;

	mmi_trace(g_sw_CC,("*** In Drop Request\n"));

   if (GetNotifySSFlag())
      StopTimer(CM_NOTIFYDURATION_TIMER);

	mmi_trace(g_sw_CC,"MMI_Call: Func: %s GetAbortCallRequestedFlag()=%d", __FUNCTION__,GetAbortCallRequestedFlag());
	if(FALSE==GetAbortCallRequestedFlag())
	{
	   	handle = GetOutgoingCallHandle();
	   	if ((GetCallflag(handle, TRUE) & CM_HANGUP_REQUESTED) == 0)
	   	{
	   		SetDropRequestFlag(TRUE);
		SetAbortCallRequestedFlag(TRUE);
			HangupCall(handle);
         if (GetExitScrnID() ==  SCR1001_CM_OUTGOINGCALLSCREEN)
            MOMTHideAnimation();
	   	}
	}
	else
	{
		mmi_trace(g_sw_CC,("*** In   DropRequest .. Not calling drop\n"));
	}
	aud_speech_set_mode(KAL_FALSE); //chenhe,070808. for bug #6100,CM_KB_OUTGOINGCALL is been sent,so have to do so here

}

/**************************************************************

	FUNCTION NAME		: HangupCall(CM_CALL_HANDLE CallHandle)

  	PURPOSE				: Helper function to generate Keybrd Event CM_KB_HANGUPREQ with the OUTGOING_CALL structure.

	INPUT PARAMETERS	: CM_CALL_HANDLE contains the call handle of the call that needs to be dropped.

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HangupCall(CM_CALL_HANDLE CallHandle)
{
	PRINT_INFORMATION (("*** In HangupCall\n"));
	OutgoingProcessCMEvent(CM_KB_HANGUPREQ,&CallHandle);
}

/**************************************************************

	FUNCTION NAME		: HangupReqSucess(void* MsgStruct)

  	PURPOSE				: Helper function to Generate CM known Stack Event CM_PS_HANGUPSUC
						  with the Structure containing CM_CALL_HANDLE.

	INPUT PARAMETERS	: void*(This contains CM_CALL_HANDLE passed by the stack when call drop is success)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  with the CM_CALL_HANDLE. CHLD RELEASE_SPECIFIC => RELEASE_IND from PS
**************************************************************/
void HangupReqSucess(void* MsgStruct)
{
	CM_CALL_HANDLE handle;

	PRINT_INFORMATION (("XXX - In HangupReqSucess XXX\n"));

	handle = DeriveCallHandle (MsgStruct);

	OutgoingProcessCMEvent(CM_PS_HANGUPSUC,(void*)&handle);
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
}

/**************************************************************

	FUNCTION NAME		: HangupAll(void)

  	PURPOSE				: Helper function to generate Keybrd Event CM_KB_HANGUPALLREQ with the NULL structure.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HangupAll(void)
{
   U16 scrId;
   U8 flag;

   flag = GetAllCallFlags();
#ifdef __MMI_MAINLCD_220X176__

   DeleteScreenIfPresent(SCREEN_DAIL_OPTION);
#endif

#ifdef __MMI_CLAMSHELL__
   if (GetCurrentState() == CM_INCOMING_STATE && GetTotalCallCount() > 1)
   	{
      KbCBackCallIncomingRejected();
      	return;
   	}   
#endif
      
	if (GetTotalActiveCallCount() > 0 && GetTotalHoldCallCount() > 0 &&
		((flag & CM_SWAP_REQUESTED) == 0) && ((flag & CM_CONF_REQUESTED) == 0))
	{
		/* swapping can't decide which call is active and held, 
		issue rel all req instead*/

		/* rel active and accept */
		ProcessIncomingEvents (CM_KB_END_ALL_ACTIVE_RETRIEVE_HELD,NULL);
	}
	else
	   OutgoingProcessCMEvent(CM_KB_HANGUPALLREQ,(void*)NULL);

   scrId = GetExitScrnID();
   if (scrId ==  SCR1001_CM_OUTGOINGCALLSCREEN || 
      (scrId == SCR_CM_ACTIVECALLSCREEN && GetTotalCallCount() == 1))
      MOMTHideAnimation();
   /*else
      EntryScr1002ActiveCall();*/
	  
}

/**************************************************************

	FUNCTION NAME		: HangupallReqSucess(void* MsgStruct)

  	PURPOSE				: Helper function to Generate CM known Stack Event CM_PS_HANGUPALLSUC
						  with the Structure containing NULL.

	INPUT PARAMETERS	: void*(This contains Null)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  without any structure.
**************************************************************/
void HangupallReqSucess(void* MsgStruct)
{
	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	gMyhandle = DeriveCallHandle (MsgStruct);
	PRINT_INFORMATION (("XXX - In HangupallReqSucess XXX\n"));
	OutgoingProcessCMEvent(CM_PS_HANGUPALLSUC,(void*)&gMyhandle);
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
}

/**************************************************************

	FUNCTION NAME		: HangupallActive(void)

  	PURPOSE				: Helper function to generate Keybrd Event CM_KB_HANGUPALLACTREQ with the NULL structure.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HangupallActive(void)
{
	OutgoingProcessCMEvent(CM_KB_HANGUPALLACTREQ,(void*)NULL);
}

/**************************************************************

	FUNCTION NAME		: HangupallActReqSucess(void* MsgStruct)

  	PURPOSE				: Helper function to Generate CM known Stack Event CM_PS_HANGUPALLACTSUC
						  with the Structure containing NULL.

	INPUT PARAMETERS	: void*(This contains Null)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  request without any structure.
**************************************************************/
void HangupallActReqSucess(void* MsgStruct)
{
	gMyhandle = DeriveCallHandle(MsgStruct);
	PRINT_INFORMATION (("XXX - In HangupallActReqSucess XXX\n"));
	OutgoingProcessCMEvent(CM_PS_HANGUPALLACTSUC,(void*)&gMyhandle);
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
}

/**************************************************************

	FUNCTION NAME		: HangupallHeld(void)

  	PURPOSE				: Helper function to generate Keybrd Event CM_KB_HANGUPALLHLDREQ with the NULL structure.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HangupallHeld(void)
{
	OutgoingProcessCMEvent(CM_KB_HANGUPALLHLDREQ,(void*)NULL);
}

/**************************************************************

	FUNCTION NAME		: HangupallHldReqSucess(void* MsgStruct)

  	PURPOSE				: Helper function to Generate CM known Stack Event CM_PS_HANGUPALLHLDSUC
						  with the Structure containing NULL.

	INPUT PARAMETERS	: void*(This contains Null)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  request without any structure.
**************************************************************/
void HangupallHldReqSucess(void* MsgStruct)
{
	gMyhandle = DeriveCallHandle (MsgStruct);
	PRINT_INFORMATION (("XXX - In HangupallHldReqSucess XXX\n"));
	OutgoingProcessCMEvent(CM_PS_HANGUPALLHLDSUC,(void*)&gMyhandle);
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
	SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
}

/**************************************************************

	FUNCTION NAME		: MakeActiveCallHold(void)

  	PURPOSE				: Helper function to generate Keybrd Event CM_KB_HOLDREQ with the NULL structure.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void MakeActiveCallHold(void)
{
	OutgoingProcessCMEvent(CM_KB_HOLDREQ,(void*)NULL);
}

/**************************************************************

	FUNCTION NAME		: HoldReqSucess(void* MsgStruct)

  	PURPOSE				: Helper function to Generate CM known Stack Event CM_PS_HOLDSUC
						  with the Structure containing NULL.

	INPUT PARAMETERS	: void*(This contains Null)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  without any structure.
**************************************************************/
void HoldReqSucess(void* MsgStruct)
{
	U16 result;

	if (CheckChldRequestSuccess(MsgStruct, &result))
	{
		PRINT_INFORMATION (("XXX - In HoldReqSucess XXX\n"));
		OutgoingProcessCMEvent(CM_PS_HOLDSUC,(void*)MsgStruct);
	}
	else
	{
		PRINT_INFORMATION(("Error in Making Hold call in protocol stack cause %d", result));
#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else 
		SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
		ResetHoldFlag();
		OutgoingProcessCMFailureEvent(CM_PS_HOLDSUC, result);
	}
}

/**************************************************************

	FUNCTION NAME		: RetrieveHeldCall(void)

  	PURPOSE				: Helper function to generate Keybrd Event CM_KB_RETRIEVEREQ with the NULL structure.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void RetrieveHeldCall(void)
{
	OutgoingProcessCMEvent(CM_KB_RETRIEVEREQ,(void*)NULL);
}

/**************************************************************

	FUNCTION NAME		: RetrieveReqSucess(void* MsgStruct)

  	PURPOSE				: Helper function to Generate CM known Stack Event CM_PS_RETRIEVESUC
						  with the Structure containing NULL.

	INPUT PARAMETERS	: void*(This contains Null)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  without any structure.
**************************************************************/
void RetrieveReqSucess(void* MsgStruct)
{
	U16 result;

	if (CheckChldRequestSuccess(MsgStruct, &result))
	{
		PRINT_INFORMATION (("XXX - In RetrieveReqSucess XXX\n"));
		OutgoingProcessCMEvent(CM_PS_RETRIEVESUC,(void*)MsgStruct);
	}
	else
	{
		PRINT_INFORMATION(("Error in Retriving Hold Call in protocol stack cause %d", result));
#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
		SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
		ResetRetrieveFlag();
		OutgoingProcessCMFailureEvent(CM_PS_RETRIEVESUC, result);
	}
}

/**************************************************************

	FUNCTION NAME		: SwapCall(void)

  	PURPOSE				: Helper function to generate Keybrd Event CM_KB_SWAPREQ with the NULL structure.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void SwapCall(void)
{
	OutgoingProcessCMEvent(CM_KB_SWAPREQ,(void*)NULL);
}

/**************************************************************

	FUNCTION NAME		: SwapReqSucess(void* MsgStruct)

  	PURPOSE				: Helper function to Generate CM known Stack Event CM_PS_SWAPSUC
						  with the Structure containing NULL.

	INPUT PARAMETERS	: void*(This contains Null)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  without any structure.
**************************************************************/
void SwapReqSucess(void* MsgStruct)
{
	U16 result;

	if (CheckChldRequestSuccess(MsgStruct, &result))
	{
		mmi_trace(1,"XXX - In SwapReqSucess sucess\n");
		OutgoingProcessCMEvent(CM_PS_SWAPSUC,(void*)MsgStruct);
	}
	else
	{
		mmi_trace(1,"Error in swapping resuest in PS cause: %d\n", result);
#ifdef __MMI_MULTI_SIM__
	MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
		SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
		ResetSwapFlag();
		OutgoingProcessCMFailureEvent(CM_PS_SWAPSUC, result);
	}
}

/**************************************************************

	FUNCTION NAME		: ConferenceCall(void)

  	PURPOSE				: Helper function to generate Keybrd Event CM_KB_CONFREQ with the NULL structure.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void ConferenceCall(void)
{
	if ((GetTotalActiveCallCount() < MAX_HOLD) &&
		(GetTotalHoldCallCount() < MAX_HOLD) &&
		((GetTotalActiveCallCount() > 0 && GetTotalHoldCallCount() > 0)))
	OutgoingProcessCMEvent(CM_KB_CONFREQ,(void*)NULL);
	else
		ShowCallManagementErrorMessage(NO_ACTIVE_CALL);
}

/**************************************************************

	FUNCTION NAME		: ConfReqSucess(void* MsgStruct)

  	PURPOSE				: Helper function to Generate CM known Stack Event CM_PS_CONFSUC
						  with the Structure containing NULL.

	INPUT PARAMETERS	: void*(This contains Null)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  without any structure.
**************************************************************/
void ConfReqSucess(void* MsgStruct)
{
	U16 result;

	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	if (CheckChldRequestSuccess(MsgStruct, &result))
	{
		PRINT_INFORMATION (("XXX - In ConfReqSucess XXX\n"));
		OutgoingProcessCMEvent(CM_PS_CONFSUC,(void*)MsgStruct);
	}
	else
	{
		PRINT_INFORMATION(("Error in Conferencing resquest in PS cause: %d\n", result));
		ResetConfFlag();
#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
		SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
		OutgoingProcessCMFailureEvent(CM_PS_CONFSUC, result);
 	}
}

/**************************************************************

	FUNCTION NAME		: SplitCall(void)

  	PURPOSE				: Helper function to generate Keybrd Event CM_KB_SPLITREQ with the NULL structure.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void SplitCall(CM_CALL_HANDLE CallHandle)
{
	OutgoingProcessCMEvent(CM_KB_SPLITREQ,&CallHandle);
}

/**************************************************************

	FUNCTION NAME		: SplitReqSucess(void* MsgStruct)

  	PURPOSE				: Helper function to Generate CM known Stack Event CM_PS_SPLITSUC
						  with the Structure containing NULL.

	INPUT PARAMETERS	: void*(This contains Null)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  request without any structure.
**************************************************************/
void SplitReqSucess(void* MsgStruct)
{
	U16 result;

	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	if (CheckChldRequestSuccess(MsgStruct, &result))
	{
		PRINT_INFORMATION (("XXX - In SplitReqSucess XXX\n"));
		OutgoingProcessCMEvent(CM_PS_SPLITSUC,(void*)MsgStruct);
	}
	else
	{
		PRINT_INFORMATION(("Error in Split in protocol stack cause %d", result));
#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
		SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
      ResetSplitFlag();
		OutgoingProcessCMFailureEvent(CM_PS_SPLITSUC, result);
  	}
}

/**************************************************************

	FUNCTION NAME		: TransferCall(void)

  	PURPOSE				: This function performs Explicit Call Transfer.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void TransferCall(void)
{
	OutgoingProcessCMEvent(CM_KB_ECT_REQ,(void*)NULL);
}

/**************************************************************

	FUNCTION NAME		: EctReqSucess(void* MsgStruct)

  	PURPOSE				: Helper function to Generate CM known Stack Event CM_PS_SPLITSUC
						  with the Structure containing NULL.

	INPUT PARAMETERS	: void*(This contains Null)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
						  request without any structure.
**************************************************************/
void EctReqSucess(void* MsgStruct)
{
   U16 result;

	mmi_trace(g_sw_CC,"-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	if (CheckChldRequestSuccess(MsgStruct, &result))
	{
		PRINT_INFORMATION (("XXX - In EctReqSucess XXX\n"));
		OutgoingProcessCMEvent(CM_PS_ECT_SUC,(void*)MsgStruct);
	}
	else
	{
		PRINT_INFORMATION(("Error in ECT in protocol stack cause %d", result));
#ifdef __MMI_MULTI_SIM__
		MTPNP_AD_SetProtocolEventHandler((PsFuncPtr)PsCBackNetworkCallDropped, PRT_NWRK_CALL_RELEASE, MTPNP_AD_CALL_CHANNEL);
#else
		SetProtocolEventHandler (PsCBackNetworkCallDropped,PRT_NWRK_CALL_RELEASE);
#endif
		OutgoingProcessCMFailureEvent(CM_PS_ECT_SUC, result);
    }
}

/**************************************************************

	FUNCTION NAME		: EnterScreenAndHandleDigit

  	PURPOSE				: Helper function to initiate sending DTMF tone and
						  display screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void EnterScreenAndHandleDigit(void)
{
//	ClearDtmfBuf();

	if ((GetTotalActiveCallCount() > 0 || GetCurrentState() == CM_OUTGOING_STATE)
		&&(GetDTMFFlag()))
	{
		EntryScrDisplayDialedDigits();
		HandleDTMFScreenDigits();
	}
	else
	{
		EntryScrDisplayDialedDigits();
		UpdateDialedDigitDisplay();
	}
}

/**************************************************************

	FUNCTION NAME		: EntryScrDisplayDialedDigits(void)

  	PURPOSE				: Shows the dialled digits in dtmf

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrDisplayDialedDigits(void)
{
	U8 *historyBuffer;
	U16 lskStrShow = STR_GLOBAL_SAVE;
	U8 *tmp=GetCurrInputBuffer(DTMF_SCR_DISPLAY_ID);

   EntryNewScreen(DTMF_SCR_DISPLAY_ID,
	               ExitScrDisplayDialedDigits,
	               NULL, NULL);

	cm_p->history_info.IsSave = TRUE;

	if(tmp)
	{
		pfnUnicodeStrcpy((S8*)UnicodeDTMFPadCallBuffer,(S8*)tmp);
	}
    else
    {
		  memset (UnicodeDTMFPadCallBuffer, 0, sizeof (UnicodeDTMFPadCallBuffer));
    }

	historyBuffer=GetCurrGuiBuffer(DTMF_SCR_DISPLAY_ID);

	// delete any previous intance of screen if present
	if(GetCurrScrnId()!=DTMF_SCR_DISPLAY_ID)
	{
		DeleteScreenIfPresent(DTMF_SCR_DISPLAY_ID);
	}

	if (GetCurrScrnId() == SCR1001_CM_OUTGOINGCALLSCREEN &&
		GetTotalCallCount() == 1)
	{
		lskStrShow = 0;
	}

#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) )
	#if defined  (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
		#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
    		mmi_idle_set_dual_sim_dialing_keypad_call_handler(
        	(MTPNP_AD_Get_UsableSide() > MTPNP_AD_DUALSIM_UNUSABLE) ? MakeSim1CallFromDTMFScr : UI_dummy_function,
        	(MTPNP_AD_Get_UsableSide() > MTPNP_AD_DUALSIM_UNUSABLE) ? MakeSim2CallFromDTMFScr : UI_dummy_function,NULL,NULL);
		#else
		SetDialingKeypadCallHandler(MakeCallFromDTMFScr);
		#endif
	#else
#ifdef __FLIGHT_MODE_SUPPORT__
	if (mmi_bootup_get_active_flight_mode() != 1)
#endif
		EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
	#endif
#endif

	ShowCategory16Screen(lskStrShow,0,STR_GLOBAL_BACK,
						 IMG_GLOBAL_BACK,(PU8)UnicodeDTMFPadCallBuffer, MAX_DIGIT,historyBuffer);
	#ifdef __MMI_WGUI_CSK_ENABLE__
		SetCenterSoftkeyFunction(MakeCallFromDTMFScr, KEY_EVENT_DOWN);
	#endif
	wgui_save_key_handlers();

	/* LSK, RSK */
	if (lskStrShow != 0)
	SetLeftSoftkeyFunction(CallMgmtSavePhoneBook,KEY_EVENT_UP);
	SetCategory16RightSoftkeyFunction(CheckBeforeGoBackHistory,KEY_EVENT_UP);

	/* SEND, END key */
	if (lskStrShow != 0)
	{
#ifdef __MMI_MULTI_SIM__
		MTPNP_PFAL_CC_HandleSendKeys(MakeCallFromDTMFScr, KEY_EVENT_DOWN);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
		ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
		SetKeyHandler(CallMgmtSavePhoneBook,KEY_ENTER,KEY_EVENT_UP );
#endif

#else 
		SetKeyHandler(MakeCallFromDTMFScr, KEY_SEND, KEY_EVENT_DOWN);
#endif

	}
	if(GetCurrentState()==CM_INCOMING_STATE)
	{
		if(GetTotalCallCount()==1)
		{
			SetKeyHandler(KbCBackCallIncomingRejected,KEY_END,KEY_EVENT_DOWN);
		}
		else
		{
#ifdef __MMI_CLAMSHELL__
			SetKeyHandler(KbCBackCallIncomingRejected,KEY_END,KEY_EVENT_DOWN);
#else
			SetKeyHandler(KbCBackEndAllCallsExceptIncoming,KEY_END,KEY_EVENT_DOWN);
#endif
		}

	}
	else if(CM_IDLE_STATE!=GetCurrentState())
	{
		SetKeyHandler(HangupAll,KEY_END,KEY_EVENT_DOWN);
	}

	/* group key */
	if ((GetTotalActiveCallCount() > 0 || GetCurrentState() == CM_OUTGOING_STATE)
		&&(GetDTMFFlag()))
	{
		SetGroupKeyHandler(HandleDTMFScreenDigits,(PU16)CMIdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);
		SetGroupKeyHandler(HandleDTMFScreenStopTone,(PU16)CMIdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_UP);
		SetGroupKeyHandler(MMI_dummy_function,(PU16)CMIdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_REPEAT);
	}
	else
	{
		SetGroupKeyHandler(UpdateDialedDigitDisplay,(PU16)CMIdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);
	}

	/* side key */
	SetSideVolumeKeysForCM();
}

/**************************************************************

	FUNCTION NAME		: ExitScrDisplayDialedDigits(void)

  	PURPOSE				: Shows the dialed digits in dtmf

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrDisplayDialedDigits(void)
{
	if (cm_p->history_info.IsSave)
	{
		history_t   Scr1001;

		Scr1001.scrnID = DTMF_SCR_DISPLAY_ID;
		Scr1001.entryFuncPtr = EntryScrDisplayDialedDigits;
		pfnUnicodeStrcpy((S8*)Scr1001.inputBuffer,(S8*)UnicodeDTMFPadCallBuffer);
		GetCategoryHistory(Scr1001.guiBuffer);
		AddHistory(Scr1001);
	}
	else
	{
		cm_p->history_info.IsSave = TRUE;
	}

	if(GetDTMFKeyUpFlag()==TRUE)
	{
		HandleDTMFScreenStopTone();
	}

#if ( (defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__) )
	#if defined  (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
		SetDialingKeypadCallHandler(UI_dummy_function);
	#else
		ResetCenterSoftkey();
	#endif
#endif

}

/**************************************************************

	FUNCTION NAME		: StoreAndKeepKeyCode

  	PURPOSE				: Helper to store current key press

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
/*
void StoreAndKeepKeyCode (U16 keycode, U16 keytype)
{
	curKeyCode = keycode;
	curKeyType = keytype;
}
*/
/**************************************************************

	FUNCTION NAME		: InCallSSCParsing

  	PURPOSE				: Helper function to check if the string is IMEI

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void InCallSSCParsing(void)
{
	S8 stringIMEI[20];
	S8 newstringIMEI[MAX_DIGITS_USSD * ENCODING_LENGTH];

	strcpy((S8*) stringIMEI, "*#06#");
	AnsiiToUnicodeString((PS8) newstringIMEI,(PS8) stringIMEI);

	if (pfnUnicodeStrcmp((S8*) newstringIMEI, (S8*) UnicodeDTMFPadCallBuffer)==0)
	{
		SSCHandleIMEI();
		return;
	}
}
/*****************************************************************************
 * FUNCTION
 *  is_valid_dtmf_key
 * DESCRIPTION
 *  Helper function to make an send dtmf tones over the network or
 *  dial a call in case of user clicking send
 * PARAMETERS
 *  keycode     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
pBOOL is_valid_dtmf_key(U16 keycode)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_CM_SCREEN_DIGITS; i++)
    {
        if (keycode == CMIdleScreenDigits[i])
        {
            return MMI_TRUE;
        }
    }
    return MMI_FALSE;
}
/**************************************************************

	FUNCTION NAME		: HandleDTMFScreenDigits

  	PURPOSE				: Helper function to make an send dtmf tones over the network or
						  dial a call in case of user clicking send

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HandleDTMFScreenDigits(void)
{
	U16 keycode,keytype;

	PRINT_INFORMATION (("\n **** HandleDTMFScreenDigits\n"));
	PRINT_INFORMATION (("\n **** Sending DTMF Tone\n"));
	//SetProtocolEventHandler(DtmfSendSuccess, PRT_DTMF_START_RES);

	SetDTMFKeyUpFlag(TRUE);

	GetkeyInfo(&keycode,&keytype);
	// StoreAndKeepKeyCode(keycode, keytype);

	// enter dial pad from New Call menu item
    if (!IsScreenPresent(SCR1003_CM_ACTIVECALLOPTIONS) && is_valid_dtmf_key(keycode))
	{
		//MuteOnMicrophone();
#ifdef __MMI_WGUI_CSK_ENABLE__
		if ( keycode != KEY_CSK )
#endif
		MakePsSendDTMFTone(&keycode);
	}
	wgui_execute_key_handler(keycode, keytype);
}

/**************************************************************

	FUNCTION NAME		: UpdateDialedDigitDisplay

  	PURPOSE				: Helper function to update dial screen with only digits

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void UpdateDialedDigitDisplay(void)
{
	U16 keycode, keytype;

	GetkeyInfo(&keycode,&keytype);
	// StoreAndKeepKeyCode(keycode, keytype);
	wgui_execute_key_handler(keycode, keytype);
	HandleDisplayDialedDigits();
	InCallSSCParsing();
}

/**************************************************************

	FUNCTION NAME		: HandleDTMFScreenStopTone

  	PURPOSE				: Helper function to make an send dtmf tones over the network or
						  dial a call in case of user clicking send

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
**************************************************************/
void HandleDTMFScreenStopTone(void)
{
	U16 keycode,keytype;

	PRINT_INFORMATION (("\n **** HandleDTMFScreenStopTone\n"));

	MakePsStopDTMFTone();
/*JINZH Del Start For 6444 Ver: TBM780  on 2007-8-24 17:50 */
	#if 0
	if (IsPhoneInMute() == MMI_FALSE)
	{TRACE_EF(g_sw_MIDI,"静音非静音");
		MuteOffMicrophone();
	}
	#endif
/*JINZH Del End  For 6444 Ver: TBM780  on 2007-8-24 17:50 */
	HandleDisplayDialedDigits();
	SetDTMFKeyUpFlag(FALSE);

	GetkeyInfo(&keycode,&keytype);
	// StoreAndKeepKeyCode(keycode, keytype);
	wgui_execute_key_handler(keycode, keytype);
	InCallSSCParsing();
}

/**************************************************************

	FUNCTION NAME		: HandleDisplayDialedDigits(void)

  	PURPOSE				: Handles drawing of category screen and display digits

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HandleDisplayDialedDigits(void)
{
	if ((GetTotalActiveCallCount() > 0 || GetCurrentState() == CM_OUTGOING_STATE)
		&&(GetDTMFFlag()))
	{
		SetGroupKeyHandler(HandleDTMFScreenDigits,(PU16)CMIdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);
		SetGroupKeyHandler(HandleDTMFScreenStopTone,(PU16)CMIdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_UP);
		SetGroupKeyHandler(MMI_dummy_function,(PU16)CMIdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_REPEAT);
	}
	else
	{
		//wgui_execute_key_handler(curKeyCode, curKeyType);
		SetGroupKeyHandler(UpdateDialedDigitDisplay,(PU16)CMIdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);
	}


	if(GetCurrentState()==CM_INCOMING_STATE)
	{
		if(GetTotalCallCount()==1)
		{
			SetKeyHandler(KbCBackCallIncomingRejected,KEY_END,KEY_EVENT_DOWN);
		}
		else
		{
#ifdef __MMI_CLAMSHELL__
			SetKeyHandler(KbCBackCallIncomingRejected,KEY_END,KEY_EVENT_DOWN);
#else
			SetKeyHandler(KbCBackEndAllCallsExceptIncoming,KEY_END,KEY_EVENT_DOWN);
#endif
		}

	}
	else if(CM_IDLE_STATE==GetCurrentState())
	{
		SetKeyHandler(CheckBeforeGoBackHistory,KEY_END,KEY_EVENT_DOWN);
	}
	else
	{
		SetKeyHandler(HangupAll,KEY_END,KEY_EVENT_DOWN);
	}

	RegisterCategory16NavigationKeys();

	SetLeftSoftkeyFunction(CallMgmtSavePhoneBook,KEY_EVENT_UP);
	SetCategory16RightSoftkeyFunction(CheckBeforeGoBackHistory,KEY_EVENT_UP);
#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_CC_HandleSendKeys(MakeCallFromDTMFScr, KEY_EVENT_DOWN);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
	ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
	SetKeyHandler(CallMgmtSavePhoneBook,KEY_ENTER,KEY_EVENT_UP );
#endif
#else
	SetKeyHandler(MakeCallFromDTMFScr, KEY_SEND, KEY_EVENT_DOWN);
#endif

	SetSideVolumeKeysForCM();
}

/**************************************************************

	FUNCTION NAME		: MakeCallFromDTMFScr

  	PURPOSE				: Clear the DTMF buffer

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MakeCallFromDTMFScr(void)
{
	MakeCall(UnicodeDTMFPadCallBuffer);
}
#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
/**************************************************************

	FUNCTION NAME		: MakeSim1CallFromDTMFScr

  	PURPOSE				: make call from sim1
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MakeSim1CallFromDTMFScr(void)
{
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
	MakeCall(UnicodeDTMFPadCallBuffer);
}
/**************************************************************

	FUNCTION NAME		: MakeSim2CallFromDTMFScr

  	PURPOSE				: make call from sim2

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void MakeSim2CallFromDTMFScr(void)
{
	MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);
	MakeCall(UnicodeDTMFPadCallBuffer);
}

#endif
/**************************************************************

	FUNCTION NAME		: CallMgmtSavePhoneBook()

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void CallMgmtSavePhoneBook(void)
{
	SetCMScrnFlag(FALSE);
	SaveNumberFromIdleScrn(UnicodeDTMFPadCallBuffer, 0);
}

/**************************************************************

	FUNCTION NAME		: ClearDtmfBuf

  	PURPOSE				: Clear the DTMF buffer

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ClearDtmfBuf(void)
{
   memset(UnicodeDTMFPadCallBuffer, 0, sizeof (UnicodeDTMFPadCallBuffer));
	memset(DTMFPadCallBuffer,0,sizeof (DTMFPadCallBuffer));
}

/**************************************************************

	FUNCTION NAME		: FillDtmfBuff

  	PURPOSE				: Fills up DTMF Buffer for access later

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void FillDtmfBuff(PU8 buf)
{
	if (buf == (PU8)UnicodeDTMFPadCallBuffer)
		return;
		
	memset(UnicodeDTMFPadCallBuffer, 0, sizeof (UnicodeDTMFPadCallBuffer));
	pfnUnicodeStrcpy(UnicodeDTMFPadCallBuffer,(void*)buf);
}

/**************************************************************

	FUNCTION NAME		: GetDtmfBuffer

  	PURPOSE				: Return the DTMF buffer

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
PU8 GetDtmfBuffer (void)
{
	memset(DTMFPadCallBuffer,0,sizeof (DTMFPadCallBuffer));
	UnicodeToAnsii(DTMFPadCallBuffer, UnicodeDTMFPadCallBuffer );
	return(PU8) DTMFPadCallBuffer;
}

/**************************************************************

	FUNCTION NAME		: SetDialIndication

  	PURPOSE				: to set the dial indication flag as per argument

	INPUT PARAMETERS	: TRUE/FALSE

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
void SetDialIndication(pBOOL truth)
{
	gCheckDialIndEndOutgoing = truth;
}

/**************************************************************

	FUNCTION NAME		: GetDialIndication

  	PURPOSE				: To Get the dial indication flag

	INPUT PARAMETERS	: TRUE/FALSE

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 

**************************************************************/
pBOOL GetDialIndication(void)
{
	return gCheckDialIndEndOutgoing;
}

void  PrintUnicodeNStr(U8* str, int n)
{
	int i, nstrlen;
	nstrlen = UCS2Strlen(str);
	mmi_trace(g_sw_DEBUG,"UNI_STR: n:%d, strlen:", n, nstrlen);
	n = (n > nstrlen? nstrlen : n);
	for (i=0; i<n; i++ )
	{
		mmi_trace(g_sw_DEBUG,"UNI_STR: [%d]:0x%x", i, str[i]);
	}
}

BOOL HandleLowBatteryProcess(void)
{
        mmi_trace(g_sw_DEBUG,"test if low battery!");
	if( !CHECK_MO_ALLOW() )
	{
		 
		ChgrPlayLowBatTone();
		BatteryIndicationPopup(STR_LOW_BATTERY);
		return TRUE;
	}
        return FALSE;
}
