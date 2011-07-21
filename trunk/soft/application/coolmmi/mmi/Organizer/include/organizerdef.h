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
 *	OrganizerDef.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is defines enum values of Organizer applications.
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

	FILENAME	: OrganizerDef.h

  	PURPOSE		: Organizer application 

 

	AUTHOR		: Gautam Saluja

	DATE		: June 17,03

**************************************************************/

#ifndef _MMI_ORGANIZERDEF_H
#define	_MMI_ORGANIZERDEF_H
#include "mmi_features.h"

#include "mmi_data_types.h"

#define __MMI_CONVERTER_KEEP_INLINE_FOCUS__
// CSD end

typedef enum
{
#if defined(__MMI_BMI__)
	HALETH_ENUM_BMI,
#endif

#if defined(__MMI_MENSTRUAL__)
	HALETH_ENUM_MENSTRUAL,
#endif

#if defined(__MMI_CALORIE__)
/* under construction !*/
#endif
	HALETH_ENUM_TOTAL
} HealthMenu;

#if defined(__MMI_VERSION_2__)
	typedef enum
	{
	#if defined (__MMI_CALCULATOR__)
		EXTRA_ENUM_CALCULATOR,	
	#endif
	
	#if defined (__MMI_UNIT_CONVERTER__)	
		EXTRA_ENUM_UNIT_CONVERTER,		
	#endif
	
	#if defined (__MMI_CURRENCY_CONVERTER__)
		EXTRA_ENUM_CURRENCY_CONVERTER,
	#endif

	#if defined(__MMI_BMI__) || defined(__MMI_MENSTRUAL__) || defined(__MMI_CALORIE__)
		EXTRA_ENUM_HEALTH,
	#endif

	#if defined(__MMI_APHORISM__)	
/* under construction !*/
	#endif

	#if defined(__MMI_IRDA_SUPPORT__)	
		EXTRA_ENUM_IRDA,
	#endif

	#if defined(__MMI_BT_SUPPORT__)	
		EXTRA_ENUM_BT,
	#endif

		EXTRA_ENUM_TOTAL	
	} ExtraMenu;

typedef enum
{
#if defined(__MMI_CALENDAR__)
	ORG_ENUM_CALRNDAR,
#endif

#if defined(__MMI_TODOLIST__)
	ORG_ENUM_TODOLIST,		
#endif

	ORG_ENUM_ALARM,	

	#if defined (__MMI_WORLD_CLOCK__) && !defined(__MMI_WORLD_CLOCK_TO_PHONESETUP__)
		ORG_ENUM_WORLDCLOCK,
	#endif
#ifdef __MMI_MESSAGES_CLUB__
		ORG_ENUM_SERVICE,
#endif
		ORG_ENUM_TOTAL	
	} OrganizerMenu;
#else
	typedef enum
	{
	#if defined(__MMI_CALENDAR__)
		ORG_ENUM_CALRNDAR,
	#endif
	
	#if defined(__MMI_TODOLIST__)
		ORG_ENUM_TODOLIST,		
	#endif
	
		ORG_ENUM_ALARM,	
	
#if defined (__MMI_CALCULATOR__)
	ORG_ENUM_CALCULATOR,	
#endif

#if defined (__MMI_UNIT_CONVERTER__)	
	ORG_ENUM_UNIT_CONVERTER,		
#endif

#if defined (__MMI_CURRENCY_CONVERTER__)
	ORG_ENUM_CURRENCY_CONVERTER,
#endif

#if defined (__MMI_WORLD_CLOCK__) && !defined(__MMI_WORLD_CLOCK_TO_PHONESETUP__)
	ORG_ENUM_WORLDCLOCK,
#endif

#if defined(__MMI_BMI__) || defined(__MMI_MENSTRUAL__) || defined(__MMI_CALORIE__)
	ORG_ENUM_HEALTH,
#endif

#if defined(__MMI_APHORISM__)	
/* under construction !*/
#endif
#ifdef __MMI_MESSAGES_CLUB__
	ORG_ENUM_SERVICE,
#endif
	ORG_ENUM_TOTAL	
} OrganizerMenu;
#endif


typedef enum 
{
	ORGANIZER_SCREENID=ORGANIZER_BASE+1,
	SCR_HEALTH_MENU,
// Beginning--added by kecx for flashlight function on 20081110
#ifdef __MMI_FLASHLIGHT__
	ORGANIZER_FLASHLIGHT_SCREENID,
#endif
// end--added by kecx for flashlight function on 20081110

	SCR9147_APHORISM_TEXT
} SCREENID_LIST_ORGANIZER;

typedef enum 
{	
	ORGANIZER_TITLE_STRINGID = ORGANIZER_BASE+1,
		
#if (defined(__MMI_BMI__) || defined(__MMI_CALORIE__) || defined(__MMI_MENSTRUAL__))
	ORGANIZER_MENU_HEALTH_STRINGID,
#endif

#if defined(__MMI_APHORISM__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

#if (defined(__MMI_BMI__) || defined(__MMI_CALORIE__) || defined(__MMI_MENSTRUAL__))
	STR_SCR_HEALTH_CAPTION,
#endif
	STR_STK,

// Beginning--added by kecx for flashlight function on 20081110
#ifdef __MMI_FLASHLIGHT__
       ORGANIZER_MENU_FLASHLIGHT_STRID,
	ORGANIZER_FLASHLIGHT_OPEN_STRID,
	ORGANIZER_FLASHLIGHT_CLOSE_STRID,
#endif
// end--added by kecx for flashlight function on 20081110

	STR_SCR_ORGANIZER_TOTAL

} STRINGID_LIST_ORGANIZER;

typedef enum
{
	ORGANIZER_TITLE_IMAGEID = ORGANIZER_BASE+1,
#if (defined(__MMI_BMI__) || defined(__MMI_CALORIE__) || defined(__MMI_MENSTRUAL__))
	ORGANIZER_MENU_HEALTH_IMAGEID,
#endif

#if defined(__MMI_APHORISM__)
/* under construction !*/
#endif

#if (defined(__MMI_BMI__) || defined(__MMI_CALORIE__) || defined(__MMI_MENSTRUAL__))
	IMG_SCR_HEALTH_CAPTION,
#endif

	IMG_SCR_ORGANIZER_TOTAL
} IMAGEID_LIST_ORGANIZER;
// Beginning--added by kecx for flashlight shortcut key on 20081120
#ifdef __MOD_FLASHLIGHT__
extern UINT8 iShortcutSwitch;
extern void SwitchFlashlightStatus(void);
#endif
// End--added by kecx for flashlight shortcut key on 20081120

// Beginning--added by kecx for flashlight function on 20081110
#ifdef __MMI_FLASHLIGHT__

extern UINT8 gFlashlightItemIdex;

typedef enum 
{
	FL_CLOSE,
	FL_OPEN
} FLASHLIGHT_STATUS;
extern void HighlightOrgFlashlightMenu(void);
extern void ExitFlashLightScreen(void);
extern void FlashlightGoOrganizer(void);
extern void Torch_light(UINT8 Torch_On);
extern void FlashlightOpen(void);
extern void FlashlightClose(void);
extern void DisplayPopup(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId);
#define FlashLight(flashLight_switch)          Torch_light(flashLight_switch)

#endif
// End--added by kecx for flashlight function on 20081110

#endif // _MMI_ORGANIZERDEF_H


