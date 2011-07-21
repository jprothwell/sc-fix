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
 *	Unitconvertor.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file defines struct, enum, and function prototypes of Unit Convertor application.
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
	Filename:		unitconvertor.h
 
	Date Created:	July-14-2003
	Contains:		Unitconvertor
**********************************************************************************/
#ifndef __MMI_UNITCONVERTOR_H
#define __MMI_UNITCONVERTOR_H

#include "mmi_features.h"
#ifdef __MMI_UNIT_CONVERTER__
#include "mmi_data_types.h"
#include "operator.h"
/* 
** Define
*/


/* 
** Typedef 
*/
typedef enum
{	STR_UNITCONVERTOR_WEIGHT=UNIT_CONVERTER +1 ,
	STR_UNITCONVERTOR_LENGTH,  //16252
	STR_UNITCONVERTOR_CAPTION,  //16253
	STR_UNITCONVERTOR_CAPTION1,  //16254
	STR_UNITCONVERTOR_KGPOUND,  //16255
	STR_UNITCONVERTOR_KG,   //16256
	STR_UNITCONVERTOR_POUND,  //16257
	STR_UCONV_OUNCE,  //16258
	STR_UCONV_KGOUNCE,  //16259
	STR_UNITCONVERTOR_KMMILE,   //16260
	STR_UNITCONVERTOR_MYARD,  //16261
	STR_UNITCONVERTOR_MFOOT,  //16262
	STR_UNITCONVERTOR_CMINCH,  //16263
	STR_UNITCONVERTOR_KM,  //16264
	STR_UNITCONVERTOR_METER,	 //16265
	STR_UNITCONVERTOR_CM, //16266
	STR_UNITCONVERTOR_MILE, //16267
	STR_UNITCONVERTOR_YARD, //16268
	STR_UNITCONVERTOR_FOOT, //16269
	STR_UNITCONVERTOR_INCH, //16270
	UC_ERROR_EMPTY_STRING, //16271
	ORGANIZER_MENU_UNITSCONVERTOR_STRINGID
}STR_ID_UCONV_ENUM;

typedef enum
{
	SCR_ID_UCONV_MENU = UNIT_CONVERTER +1,	/* do not change order */
	SCR_ID_UCONV_WEIGHT,						/* do not change order */
	SCR_ID_UCONV_LENGTH							/* do not change order */
}SCR_ID_UCONV_ENUM;

typedef enum
{
	ORGANIZER_MENU_UNITSCONVERTOR_IMAGEID = UNIT_CONVERTER +1 
}IMG_ID_UCONV_ENUM;

typedef enum
{
	UCONV_UNIT		=	0,
	UCONV_METRIC_CAP,			/* metric caption */
	UCONV_METIRC,				/* metric */
	UCONV_ENG_CAP,				/* English caption */
	UCONV_ENG,					/* English */
	UCONV_INLINE_TOTAL
}UCONV_INLINE_ITEM_ENUM;

typedef enum
{
	UCONV_WEIGHT	= 0,
	UCONV_LENGTH
}UCONV_UNIT_TYPE;

/* 
** Extern Global Variable
*/

/* 
** Extern Global Function
*/
extern void UconvInit(void);
extern void UconvDeInit(void);

extern void EntryUConvMenu(void);
extern void HighlightUConvWeightInlineItem(S32 index); 
extern void HighlightUConvLengthInlineItem(S32 index); 
extern void HighlightUConvInlineLenghSelect(S32 index); 
extern void ComputeLengthResult(void);
extern void UConvGoToComputeScreen(void);
extern void EntryUConvCompute(void);
extern void UConvCustomInlineFunction(void);
extern void HighlightUconvMenu(void);
extern void HighlightUConvInlineItem(S32 nIndex);
extern void HighlightUConvInlineWeightSelect(S32 index); 

#endif // __MMI_UNITCONVERTOR_H
#endif


