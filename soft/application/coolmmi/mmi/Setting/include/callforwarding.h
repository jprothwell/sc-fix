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
 *	CallForwarding.h
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

#ifndef _COOLSAND_CALLFORWARDING_H
#define _COOLSAND_CALLFORWARDING_H


typedef enum {
	FORWARD_UNREACHABLE =0,
	FORWARD_NOANSWER,
	FORWARD_BUSY,
	FORWARD_ALLVOICE ,
	FORWARD_DATA,
	FORWARD_CANCEL_ALL
} SSForwardType;


void HighlightCALLSETCallFwdCancelAll(void);
void InitCallForwarding(void);
void HighlightCALLSETCallFwd(void);
void HighlightCALLSETCallFwdVoice(void);
void HighlightCALLSETCallFwdData(void);
void HighlightCALLSETCallFwdUnreach(void);
void HighlightCALLSETCallFwdBusy(void);
void HighlightCALLSETCallFwdNoAns(void);
void HighlightCALLSETCallFwdOn(void);
void HighlightCALLSETCallFwdOff(void);
void HighlightCALLSETCallFwdQuery(void);
void EntryCALLSETForwardMain(void);
void EntryCALLSETForwardOptions(void );
void StarKeyHandler(void);
void EntryCALLSETForwardNumber(void);
void EnterScrCallFwdOnReq(void);
void EnterScrCallFwdOffReq(void);
void EnterScrCallFwdQueryReq(void);
void ActivateVNoAnswer( void );
void GetCurrHandler( S32 item_index );
void EntryCALLSETForwardNoAnsTime(void);
void SendPhbEntryForDivert(U8 name[], U8 number[]);


#endif 


