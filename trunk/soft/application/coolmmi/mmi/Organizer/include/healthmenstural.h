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
 *	HealthMenstural.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines constant, enum value, structure, and function prototypes for Menstrual application.
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
#ifndef _MMI_HEALTHMENSTRUAL_H
#define _MMI_HEALTHMENSTRUAL_H

#include "mmi_features.h"
#include "mmi_data_types.h"
#if defined(__MMI_MENSTRUAL__)
#include "cswtype.h"
#include "datetimetype.h"

/* 
** Define
*/

/* 
** Typedef 
*/
typedef enum
{
	MENS_INLINE_DATE_CAP = 0,
	MEMS_INLINE_DATE,
	MEMS_INLINE_PERIOD_CAP,
	MEMS_INLINE_PERIOD,
	MEMS_INLINE_NUM_DAYS,	
	MEMS_INLINE_TOTAL	
}MENS_INLINE_ENUM;

typedef enum 
{
	COLOR_NONE = 	0,
	RED_COLOR,
	ORANGE_COLOR,
	YELLOW_COLOR
}MENS_COLOR_EMUM;


typedef enum
{
	MENS_ERR_NONE	 = 0,
	MENS_ERR_DATE,
	MENS_ERR_PERIOD
}MENS_ERR_MSG_ENUM;


typedef enum
{
	SCR_ID_MENS_WELCOME = HEALTH_MENSTRUAL + 1,
	SCR_ID_MENS_INPUT,
	SCR_ID_MENS_RESULT,
	SCR_ID_MENS_PROB
}SCR_ID_MENS_ENUM;

typedef enum
{
	STR_SCR_HEALTH_MENSIS_CAPTION = HEALTH_MENSTRUAL + 1,
	STR_HEALTH_MENSIS_DATE_CAPTION,
	STR_LAST_DATE,
	STR_DATE_FORMAT_HEALTH_MENSIS,
	STR_AVERAGE_DAYS_HEALTH_MENSIS,
	STR_INVALID_AVERAGE_DAY_MESSAGE,
	STR_SCR_HEALTH_MENSIS_PROBABILITY_CAPTION,
	STR_AVERAGE_DAYS_HEALTH_NAVIGATE,
	STR_SCR_HEALTH_MENSIS_PROBABILITY_MESSAGE,
	STR_CANNOT_NAVIGATE_FURTHER_MESSAGE,
	STR_CALENDER_MENSES_CAPTION,
	STR_CALENDER_MESSAAGE1,
	STR_CALENDER_MESSAAGE2,
	STR_HEALTH_WRONG_DATE_MSG,
	EXTRA_HEALTH_MENU_MENSTRUAL_STRINGID
}STR_ID_MENS_ENUM;


typedef enum
{
	IMG_SCR_HEALTH_MENSIS = HEALTH_MENSTRUAL + 1
}IMG_ID_MENS_ENUM;

/* 
** Extern Global Variable
*/

/* 
** Extern Global Function
*/

extern void MensInit(void);
extern void MensDeInit(void);

extern void HighlightMensApp(void);
extern void EntryMensWelcome(void);
extern void EntryMensInput(void);
extern void ExitMensInput(void);
extern void MensPreEntryInput(void);


// inline editor
extern void HighlightMensInputItem(S32 item_index);
extern void MensDisplayResult(void);
extern U8 MensValidateInput(void);
extern void MensPreEntryResult(void);
extern void EntryMensResult(void);
extern void HighlightMensNevigeteDate(S32 index);
extern void MensGoToPreviousDay(void);
extern void MensGoToNextDay(void);
extern void MensGoToPreviousWeek(void);
extern void MensGoToNextWeek(void);
extern void MensGoToPreviousMonth(void);
extern void MensGoToNextMonth(void);
extern void MensDisplayBoundaryPopup(void);
extern S16 MensGetDaysDifference(MYTIME *t1, MYTIME *t2);

extern void EntryMensProbability(void);

//add by zough 061121 start
#ifndef __MOD_ORGANIZER__
extern FLOAT MensGetPregancyProb(S16 DiffDay, S16 AvgPeriod);
#else
extern S32 MensGetPregancyProb(S16 DiffDay, S16 AvgPeriod);
#endif
//add by zough 061121 end
extern MENS_COLOR_EMUM MensGetDisplayColor(S16 DiffDay, U8 AvgPeriod);


#endif //#if defined(__MMI_MENSTRUAL__)
#endif//#ifndef _MMI_HEALTHMENSTRUAL_H


