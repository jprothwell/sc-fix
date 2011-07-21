/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2002
*
*******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *	Bmi.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file intends for defining the data and functions structures used in BMI application
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
 /*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2003
*
*******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *	bmi.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file intends for define the data and functions structures used in 
 *  bmi.c
 *
 * Author:
 * -------
 * -------
 *
 *******************************************************************************/
#ifndef _MMI_BMI_H
#define _MMI_BMI_H

#include "mmi_features.h"
#ifdef __MMI_BMI__
#include "mmi_data_types.h"


/* 
** Define
*/

/* 
** Typedef 
*/

typedef enum
{
	ENUM_GENDER = 0,
	ENUM_HEIGHT_CAPTION,
	ENUM_HEIGHT,
	ENUM_WEIGHT_CAPTION,
	ENUM_WEIGHT,
	ENUM_INLINE_TOTAL
}BMI_INLINE_ITEM_ENUM;

typedef enum 
{
	STR_BMI_ACT1 = HEALTH_BMI +1,
	STR_BMI_WELCOME_CAPTION,
	STR_BMI_INPUT_CAPTION,
//	STR_BMI_RESULT_TEXT,
	STR_BMI_RESULT_CAPTION,
	STR_BMI_MALE_TEXT,
	STR_BMI_FEMALE_TEXT,
	STR_BMI_HEIGHT_TEXT,
	STR_BMI_WEIGHT_TEXT,
	STR_BMI_HEIGHT_ERROR_TEXT,
	STR_BMI_WEIGHT_ERROR_TEXT,
	STR_BMI_HEIGHT_WEIGHT_ERROR,
	STR_BMI_BMI_TEXT,
	STR_BMI_LEVEL,
	STR_BMI_NORMAL,
	STR_BMI_FAT,
	STR_BMI_THIN,
	EXTRA_HEALTH_MENU_BMI_STRINGID,
	STR_BMI_TOTAL
}STR_ID_BMI_ENUM;

typedef enum
{
	IMG_BMI_WELCOME_SCREEN = HEALTH_BMI +1 ,
	IMG_BMI_WELCOME_CAPTION,
	IMG_BMI_RED_FLOWER,
	IMG_BMI_BLUE_FLOWER
} IMG_ID_BMI_ENUM;

typedef enum
{
	SCR_BMI_WELCOME = HEALTH_BMI +1 ,
	SCR_BMI_INPUT,
	SCR_BMI_RESULT,
	SCR_POPUP_HEALTH
} SCR_ID_BMI_ENUM;

/* 
** Extern Global Variable
*/


/* 
** Extern Global Function
*/
extern void HighlightBMIMenu(void);
extern void BmiDeInit(void);
extern void BmiExit(void *param_p);
extern void EntryBMIWelcomeScreen(void);
extern void EntryBMIInputScreen(void) ;
extern void ExitBMIInputScreen(void) ;
extern void EntryBMIResultScreen(void); 
extern void HighlightBMIInlineEditor(S32 index) ;
extern void BmiComputeResult(void);
extern void HighlightBMIGender(S32 index);
extern void BMIFillInlineStruct(void);

#endif // #ifdef __MMI_BMI__

#endif // #ifndef _MMI_BMI_H




