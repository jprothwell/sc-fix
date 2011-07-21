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
 *	CallSetUp.h
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
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/**************************************************************

	FILENAME	: Callorwarding.h

  	PURPOSE		: Call application 

 

	 

	DATE		: 29th April 2003 

**************************************************************/

#ifndef _COOLSAND_CALLFORWARDINGSTRUCT_H
#define _COOLSAND_CALLFORWARDINGSTRUCT_H

//micha0727
#include "phonebooktypes.h"

//#define CS_NOTIFYDURATION					UI_POPUP_NOTIFYDURATION_TIME

#define BARR_SIM_PASS_LEN	4
#define MAX_CTR_NUM_DIGITS	6
#define MAX_DISP_UCS2		200*ENCODING_LENGTH

typedef enum
{
	LINE1,
	LINE2,
	MAX_LINE_NUM
} E_LINE_TPYE;

typedef struct {
	U8 CallerID;
	U8 CallerIDInput;
	U8 CTRStatus;
	U8 CTRStatusInput;
	U8 CTRNumber[MAX_CTR_NUM_DIGITS* ENCODING_LENGTH];
	U8 CTRNumberInput[MAX_CTR_NUM_DIGITS* ENCODING_LENGTH];
	U8 CTDStatus;
	U8 CTDInput;
	U8 AutoRedialStatus;
	U8 AutoRedialInput;
	U8 LineID;
	U8 LineIDInput;
#ifdef __MMI_LINE_SWITCH_SHOW_NUMBER__
	U8 LineInfo[(MAX_PB_NUMBER_LENGTH+25)*ENCODING_LENGTH];	
#else
	U8 LineInfo[25*ENCODING_LENGTH];	
#endif
	U8 SSDisplayBuffer[MAX_DISP_UCS2];
	U8 SSBarPassStep;
	U8 SSBarType;
	U8 SSBarAction;
	U8 SSBarPassOld[BARR_SIM_PASS_LEN* ENCODING_LENGTH + 4];
	U8 SSBarPassNew[BARR_SIM_PASS_LEN*ENCODING_LENGTH + 4];
	U8 SSBarPassCfrm[BARR_SIM_PASS_LEN*ENCODING_LENGTH + 4];	
	U8 SSCFType;
	U8 SSCFNum[(MAX_CC_ADDR_LEN * ENCODING_LENGTH) + 2];
	U8 SSCFNumFromPHB;
	U8 SSCFNoAnsTimeIndex;
	U8 nSimId;
  	U8 SoundWarnningIndex;
#ifdef MAGIC_AE_SUPPORT
  	U8 MagicSoundState;
  	U8 MagicSoundIndex;
#endif
#ifdef __MMI_AUTO_PLUS_ENABLE__
	U8 AutoPlusState;
#endif
} CallSetupContext;


extern CallSetupContext g_callset_context[];

extern CallSetupContext *g_callset_cntx_p;

#ifdef __MMI_MULTI_SIM__
#define MMI_CALLSET_DEST_MOD   (MOD_L4C + g_callset_cntx_p->nSimId)
#else
#define MMI_CALLSET_DEST_MOD   MOD_L4C
#endif

void CALLSETWaitingRsp(void *);
void CALLSETForwardRsp(void *);
void CALLSETBarringRsp(void *);
void CALLSETBarringPassRsp(void *);
void AppendBScodeString( U8, PS8);
void CALLSETLineInUseRsp(void*);
void CALLSETClipRsp(void * info);
void CALLSETClirRsp(void * info);
void CALLSETColpRsp(void * info);
void CALLSETColrRsp(void * info);
void CALLSETCnapRsp(void * info);
void CALLSETCcbsQueryRsp(void * info);
//micha1228
void CALLSETEmlppQueryRsp(void * info);
void EntryCALLSETGeneralSSResut(void);


void HighlightCallsetSoundWarnning(void);
void CallsetSoundWarnningHiliteHandler(S32 index);
void EntryCallsetSoundWarnning(void);
void CallsetSoundWarnningLSKHandler(void);

#ifdef  __MMI_SS_SHOW_CAUSE__
void ShowCauseString( U16, PS8);
#endif


//IP Number begin

#define MAX_IP_NUMBER      3
#define MAX_IP_NUMBER_LEN  10

 
#define MAX_BLACK_LIST_NUMBER	5
#define MAX_BLACK_LIST_NUMBER_LEN	40
 

#endif


