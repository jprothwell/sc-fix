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
 *	ManageCallMain.h
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

	FILENAME	: Calls_Defs.h

  	PURPOSE		: Call application 

 

	 

	DATE		: April 25,03

**************************************************************/

#ifndef _COOLSAND_MANAGECALLSMAIN_H
#define _COOLSAND_MANAGECALLSMAIN_H


/*gaosh Add Start For rstdefault Ver: TBM780_R_V1.0.7_20071129_1740  on 2007-12-7 17:9 */
BOOL CallsetRstDefault(void);
/*gaosh Add End  For rstdefault Ver: TBM780_R_V1.0.7_20071129_1740  on 2007-12-7 17:9 */
void HighlightManageCalls(void);
void EntryCALLSETMain(void);
void EntryCALLSETCallTimeReminder(void);
void ExitCALLSETCallTimeReminder(void);
void EntryCALLSETCtrTime(void);
void ExitCALLSETCtrTime(void);
#if (0)
/* under construction !*/
#endif
void EntryCALLSETCUGMain(void);
void InitCallWaiting(void);
void InitCallForwarding(void);
void InitCallBarring(void);
void InitGroupListApp(void );
void InitLineSwitch(void);
void InitCallerId(void);
void HintCALLSETCallerID(U16 );
void HintCALLSETLineSwitch(U16 );
void HintCALLSETCallTimeReminder(U16 );
void HintCALLSETCallTimeDisplay(U16 );
void HintCALLSETAutoRedial(U16 );
void HintCALLSETSoundWarnningTime(U16 index);
void InitSoundWarnningTime(void);
void SetSoundWarnningTimer(void);
void ClearSoundWarnningTimer(void);
#ifdef  __MMI_SS_SHOW_CAUSE__
typedef struct {
	U16	cause_number;
	U8	cause_string[80];
} SSCauseDisplayStruct;
#endif



//IP number begin

#ifdef __IP_NUMBER__
void InitIPNumber(void);
void HighlightIPNumber(void);
void LoadIPNumbers(void);
void EntryScrViewIPNumbers(void);
void SetCurIPNumberIndex(S32 index);
void EntryScrIPNumberOptions (void);
void HighlightIPNumActivate(void);
void HighlightIPNumEdit(void);
void ActivateIPNumber(void);
void EntryScrIPNumberEdit (void);
void SetIPNumber(void);
void GetCurrIPNumber(U8* num_str, U8 nSimID);
S8 *GetCurrIPNumberUCS2(U8 nSimID);
void SetIPMenuOption(U32 menuID);
#endif
 
#ifdef __MMI_CM_BLACK_LIST__
void InitBlackList(void);
void HighlightBlackList(void);
void HighlightBlackListMode(void);
void HighlightBlackListNum(void);
void HintBlackList(U16 index);
void HintBlackListMode(U16 index);
void black_red_list_status(void);
void EntryScrBlackList(void);
void EntryScrBlackListMode(void);
void EntryScrBlackListNum(void);
void ExitScrBlackListNum(void);
void EntryScrSetBlackListNumConfirm(void);
void GoToBlackList(void);
void SetBlackListMode(pBOOL flag);
pBOOL GetBlackListMode(void);
void SetBlackListNum(void);
void LoadBlackListNum(void);
void SetCurrBlackListNumIndex(S32 index);
pBOOL IsBlackListNum(PU8 number);
#endif
 
#endif 


