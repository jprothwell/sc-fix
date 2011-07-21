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
 *	SchedulePowerOnOff.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file is defines constant, structrue, and function prototypes for SPOF application.
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

	FILENAME	:ScheduledPowerOnOff.h

  	PURPOSE		: nil

 

	AUTHOR		: Gautam Saluja

	DATE		: 

**************************************************************/
/* ScheduledPowerOnOff.h*/
#ifndef _MMI_SCHEDULEPOWEROFOFF_H
#define _MMI_SCHEDULEPOWEROFOFF_H

#include "alarmframeworkprot.h"
#include "alarmdef.h"
/* 
** Define
*/
#define SPOF_NUM_STATES_ONOFF 2
#define SPOF_NUM_STATES_ACTINACT 2
#define NUM_ITEM_EDIT_SCREEN 4
#define SPOF_DISABLE 0
#define SPOF_ENABLE 1
#define SPOF_POWERON 0
#define SPOF_POWEROFF 1
/* 
** Typedef 
*/

typedef struct
{
	U8	Type;
	U8	Status;
	U8	Hour;
	U8	Min;
}spof_nvram_struct;

typedef struct
{
	S32 CurrHiliteOnOff;
	S32 CurrHiliteActivation;
	S8	*ActStateList[SPOF_NUM_STATES_ACTINACT];
	S8	*OnOffStateList[SPOF_NUM_STATES_ONOFF];
	spof_nvram_struct SPOFList[NUM_OF_SPOF];
	S8  HourBuf[(HOUR_LENGTH + 1) * ENCODING_LENGTH];
	S8  MinBuf[(MINUTE_LENGTH + 1) * ENCODING_LENGTH];	
	U8	IsRestoreDefault;
	U8	CurrItem;
}spof_context_struct;

typedef enum
{
	SCR_ID_SPOF_APP	=	SPOF_BASE+1,
	SCR_ID_SPOF_EDITOR,
	SCR_ID_SPOF_PWROFF_IND
} SCR_ID_SPOF_ENUM;

typedef enum 
{	
	SPOF_MAIN_TITLE_STRINGID = SPOF_BASE+1,
	SPOF_EDIT_TITLE_STRINGID,
//	SPOF_EDIT_BUTTON_STRINGID,
	SPOF_STATUS_STRINGID,
	SPOF_DISABLE_STRINGID,
	SPOF_ENABLE_STRINGID,
	SPOF_TIME_CONFLICT,
	SPOF_POWER_OFF_CONFIRM

} STR_ID_SPOF_ENUM;

typedef enum 
{
	SPOF_EDIT_BUTTON_IMAGEID= SPOF_BASE+1,
	SPOF_TYPEPOWERON_IMAGEID,
	SPOF_TYPEPOWEROFF_IMAGEID,
	SPOF_SETTING_STATUS_IMAGEID,
	SPOF_BLANK_IMAGEID

} IMG_ID_SPOF_ENUM;


/* 
** Extern Global Variable
*/
extern spof_context_struct g_spof_cntx;

/* 
** Extern Global Function
*/
extern void EntrySpofApp(void);

extern void EntrySpofEdit(void);
extern void ExitSpofEdit(void);
extern void HighlightSpofList(S32 nIndex);
extern void SpofFillInlineEditStruct(void);
extern void SpofSaveData(void);
extern void SpofHandler(U8);
extern void SpofWritetoNvram(void);
extern BOOL SpofIsTimeClash(U8 hour, U8 min);


#endif	// _MMI_SCHEDULEPOWEROFOFF_H


