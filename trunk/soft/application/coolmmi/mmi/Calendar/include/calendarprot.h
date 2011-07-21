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
 *	CalendarProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines constant, enum value, structure, and function prototypes for calendar application.
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

/**********************************************************************************
	Filename:		CalendarProt.h
	Author:			GAUTAM
	Date Created:	July-12-2003
	Contains:		Calendar Prot
**********************************************************************************/
#ifndef _MMI_CALENDARPROT_H
#define _MMI_CALENDARPROT_H
#include "mmi_features.h"
#ifdef __MMI_CALENDAR__
#include "wgui_calendar.h"
#include "mmi_data_types.h"


/* 
** Define
*/


/* 
** Typedef 
*/

/* struct DateInfo defines all data/result for calendar converting */
typedef struct 
{  	
	U16 SolarYear;						/* year in solar calendar system. */
	U16 LunarYear;						/* year in lunar calendar system. */
	U8 	SolarMonth;						/* month in solar calendar system. */
	U8 	SolarDay;						/* day in solar calendar system. */
	U8 	LunarMonth;						/* month in lunar calendar system. */
	U8	LunarDay;						/* day in lunar calendar system. */
	U8 	KanYear;						/* the Kan symbol of given year */
	U8 	ChiYear;						/* the Chin symbol of given year */
	U8 	Direction;						/* the unfortunate direction of selecting day */
	U8 	Age;							/* the unfortunate age of selecting day */
	U8 	Animal;							/* the unfortunate symbolizing animal today */
	U8 	SolarTerm;						/* solar term today, 25 == NONE */
	U8	SolarDate;						/* the nearest solar date from today */
	U8 	YearAnimal;						/* the symbolizing animal of this year */
	S8 TitleInfo1[CLNDR_TITLE_LEN];		/* the string shown in the first line of title */
	S8 TitleInfo2[CLNDR_TITLE_LEN];		/* the string shown in the second line of title */
	S8 MessageLine1[CLNDR_HINT_LEN];	/* the string shown solar term */
	S8 MessageLine2[CLNDR_HINT_LEN];	/* the string shown about unlucky direction */
	S8 MessageLine3[CLNDR_HINT_LEN];	/* the string shown about unfortunate age*/	
}clndr_lnuar_date_info_struct;


/* 
** Extern Global Variable
*/

/* 
** Extern Global Function
*/

extern void ClndrInit(void);
extern void ClndrDeInit(void);


extern void ClndrComputeLunarInfo(clndr_lnuar_date_info_struct *DI);
extern S16 ClndrComputeSolarTerm(S16 year, S16 month, S16 solar_index);
extern void ClndrPreEntryApp(void);
extern void EntryClndrScreen(void);
extern void ExitClndrScreen(void);
extern void HighlightClndrMenu(void);
extern void HighlightClndrDate(S32 date);
extern void ClndrGetScreenInfo(void);
extern void ClndrGoBackFromCalendar(void);
extern void ClndrGeneratePopupInfo(U8 type,U8 num_cols,U8 new_day,U8 old_day);

extern void ExitClndrOptions(void);
extern void EntryClndrOptions(void);
extern void HighlightClndrViewTask(void);
extern void HighlightClndrLunar(void);
extern void ClndrDisplayTask(void);
extern void ClndrLunarHintHandler(U16);


extern void HighlightJumpToDate(void);
extern void EntryClndrJumpToDate(void);
extern void ClndrDateEditorCallBack(U8 *par1, U8* par2,U8* par3,U8* par4);

extern void UpdateClndrContext(s32 day,s32 month,s32 year);
#endif
#endif // #ifndef _MMI_CALENDARPROT_H


