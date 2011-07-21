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
 *	CallBarring.h
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

 

	DATE		: Jan 15,02

**************************************************************/
#ifndef _COOLSAND_CALLBARRING_H
#define _COOLSAND_CALLBARRING_H


typedef enum{
BARRING_OUTGOING_ALLCALLS,
BARRING_OUTGOING_INTERCALL,
BARRING_OUTGOING_INTEREXCEPTHOME,
BARRING_INCOMING_ALLCALLS,
BARRING_INCOMING_ROAMING
}SSBarType ;


typedef enum{
BARRING_ON,
BARRING_OFF,
BARRING_QUERY,
BARRING_CHANGE_PSW,
BARRING_CANCEL_ALL
} SSBarAction ;


void MakeCall(PS8 );
void InitCallBarring(void);
void HighlightCALLSETCallBarring(void);
void EntryCALLSETBarringMain(void);
void HighlightCALLSETCallBarringOutgoing(void);
void HighlightCALLSETCallBarringIncoming(void);
void HighlightCALLSETCallBarringCancelAll(void);
void HighlightCALLSETCallBarringChangePwd(void);
void EntryCALLSETBarringOut(void);
void EntryCALLSETBarringIn(void);
void EntryScrCallBarringCancelAll(void);
void EntryScrCallBarringChnPass(void);
void HighlightCALLSETCallBarringAllOutCalls(void);
void HighlightCALLSETCallBarringAllOutInter(void);
void HighlightCALLSETCallBarringAllOutInterExcHome(void);
void HighlightCALLSETCallBarringAllInCalls(void);
void HighlightCALLSETCallBarringAllInRoaming(void);
void EntryCALLSETBarringOption(void);
void HighlightCALLSETCallBarringActive(void);
void HighlightCALLSETCallBarringDeactive(void);
void HighlightCALLSETCallBarringQuery(void);
void EntryCALLSETBarringPass(void);
void CallBarrOption(void);
void SendCallBarrActivate(void);
void SendCallBarrDeactivate(void);
void SendBarrQuery(void);
void SendCancelAllReq(void);
void CallBarringCancelAllRsp(void *);
void SendChangePswdReq(void);
void EntryCALLSETBarringOldPass(void);
void EntryCALLSETBarringNewPass(void);
void EntryCALLSETBarringCfrmPass(void);

void CheckOldpassword(void);
void CheckNewpassword(void);
void CheckConfirmpassword(void);
void CheckBarpassword(void);

 
#endif

