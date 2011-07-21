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
 * SpeedDial.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Speed dial applicaion
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

	FILENAME	: SpeedDial.h

  	PURPOSE		: Speeddial application 


 

	AUTHOR		: nil

	DATE		: 

**************************************************************/

#ifndef _SPEEDDIAL_H
#define _SPEEDDIAL_H

#define MAX_SPEED_DIAL_NUMBER  8

#ifdef __MMI_PHB_MAX_NAME_60__
#define NVRAM_SPEEDDIAL_RECORD_SIZE 1680
#else
#define NVRAM_SPEEDDIAL_RECORD_SIZE 1200
#endif

typedef struct {
	U8 type;
	U8 storage;
	U16  record_index  ;
	U8	number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
	U8	name[(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH];
}SpeedDialInfo;

void HighlightPhbSpeedDialStatus(void);
void HintPhbSpeedDial(U16);
void EntryPhbSpeedDialStatus(void);
void HighlightPhbSpeedDial(void);
void EntryPhbSpeedDial(void);
void ExitPhbSpeedDial(void);

void PhbInitSpeedDial(void);
void HighlightPhbSpeedDialSetNum(void);
void EntryPhbSpeedDialSetNum(void);
//void ExitScrSpeedDialMain(void);
void HighlightPhbSpeedDialSetNumOptionEdit(void);
void HighlightPhbSpeedDialSetNumOptionDel(void);
void EntryPhbSpeedDialSetNumOption(void);
void EntryPhbSpeedDialSetNumDelConfirm(void);
void PhbReadSpeedDialNVRAM(void);
void PhbSetSpeedDialFormList(void);
void PhbDelSpeedDialFromList(void);
void PhbSpeedDailEntryHiliteHandler(S32);
void PhbMakeCallFromSpeedDial(void);
PS8  PhbGetSpeedDialNum(U16 );
void ShowSpeedDial(void);
void PhbSetSpeedDialPhbIndex(U16);
void PhbGetSpeedDialInfo(void);
void PhbGetSpeedDialInfo_Ext(void);
void PhbGetSpeedDialPhbIndexReq(U8, U16, U8);
void PhbGetSpeedDialPhbIndexRsp(void*);
MMI_BOOL PhbGetSpeedDialStatus(void);
void PhbSpeedDialLookupNumberFromList(U16 speed_index);


#endif 


