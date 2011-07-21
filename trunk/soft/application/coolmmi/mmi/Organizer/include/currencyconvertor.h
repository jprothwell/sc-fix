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
 *	CurrencyConvertor.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines constant, enum values, structure, and function prototypes for currency converter application.
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
	Filename:		currencyconvertor.h
 
	Date Created:	July-29-2003
	Contains:		CurrencyConvertor
**********************************************************************************/
#ifndef _MMI_CURRENCYCONVERTOR_H
#define _MMI_CURRENCYCONVERTOR_H
#include "mmi_features.h"
#ifdef __MMI_CURRENCY_CONVERTER__
#include "mmi_data_types.h"


/* 
** Define
*/

/* 
** Typedef 
*/
typedef enum
{
	CCONV_INLINE_RATE = 0,
	CCONV_INLINE_LOCAL_CAP,
	CCONV_INLINE_LOCAL,
	CCONV_INLINE_FORE_CAP,
	CCONV_INLINE_FORE,
	CCONV_INLINE_TOTAL
}CCONV_INLINE_ITEM_ENUM;

typedef enum
{
	STR_CURRENCYCONVERTOR_RATEINPUT= CURRENCY_CONVERTER + 1,
	STR_CURRENCYCONVERTOR_EXCHANGE,
	STR_CURRENCYCONVERTOR_RATE,
	STR_CURRENCYCONVERTOR_INPUT,
	STR_CURRENCYCONVERTOR_LOCAL,
	STR_CURRENCYCONVERTOR_FOREIGN,
	ERROR_CURRENCYCONVERTOR_RATE1,
	ORGANIZER_MENU_CURRENCYCONVERTOR_STRINGID,
	STR_CURRENCYCONV_INPUT
}STR_ID_CCONV_ENUM;

typedef enum
{
	ORGANIZER_MENU_CURRENCYCONVERTOR_IMAGEID = CURRENCY_CONVERTER + 1,
	CURRENCY_CONVERTOR_IMAGE
}IMG_ID_CCONV_ENUM;

typedef enum
{
	SCR_ID_CCONV_RATE_INPUT = CURRENCY_CONVERTER + 1,
	SCR_ID_CCONV_COMPUTE
}SCR_ID_CCONV_ENUM;


/* 
** Extern Global Variable
*/

/* 
** Extern Global Function
*/
extern void CconvInit(void);
extern void CconvDeInit(void);
extern void CconvExit(void* param_p);
extern void CconvClearResult(void);

extern void HighlightCConvMenu(void);
extern void EntryCconvApp(void);
extern void ExitCConvApp(void);
extern void CconvPreEntryExchange(void);
extern void EntryCConvExchange(void);
extern void ExitCConvExchange(void);
extern void HighlightCconvInlineItemHandler(S32 index); 
extern void HighlightCconvInlineSelHandler(S32 index); 
extern void CconvCustomInlineFunction(void);


#endif	// #ifdef __MMI_CURRENCY_CONVERTER__
#endif	// #ifndef _MMI_CURRENCYCONVERTOR_H


