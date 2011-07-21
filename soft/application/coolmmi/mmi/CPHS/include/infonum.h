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
 *  InfoNum.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: InfoNum.h

  	PURPOSE		: Information Number

 

 

	DATE		: Aug 13,03

**************************************************************/
#ifndef __INFO_NUM_H
#define __INFO_NUM_H

#include "mmi_data_types.h"
#include "l4dr.h"
extern U8 isInfoNumPresent;
void EnterScrInfoNumber(void);

#define MAX_INFO_NUM_LEVELS 16
#define MAX_INFO_NUM_ITEMS		20
typedef enum
{
	SERVICES_INFO_NUM_STRING_ID=INFO_NUM_BASE+1,
	SERVICES_INFO_DIAL_STRING_ID,
	SERVICES_INFO_PREMIUM_STRING_ID,
	SERVICES_INFO_ROAMING_STRING_ID
} INFO_NUM_STRING;

typedef enum
{
	SERVICES_INFO_NUM_IMAGE_ID=INFO_NUM_BASE+1
} INFO_NUM_IMAGE;

typedef enum
{
	SERVICES_INFO_NUM_SCREEN_ID=INFO_NUM_BASE+1,
	SERVICES_INFO_PREMIUM_SCREEN_ID,
	SERVICES_INFO_ROAMING_SCREEN_ID
} INFO_NUM_SCREEN_ID;



typedef struct InfoNum
{
	U8 noOfChildren;
	U8 alphaId[21*ENCODING_LENGTH+ENCODING_LENGTH];
	U8 nsi;
	U8 psi;
	U8 index;
	U8 number[4*ENCODING_LENGTH+ENCODING_LENGTH];
	struct InfoNum *parent;
	struct InfoNum *child;
	struct InfoNum *next;
	struct InfoNum *previous;
}INFONUM;
void DisplayInformationNumber(void);
void InfoNumberScrHiliteHandler(S32 index);
void GoAheadInfoNum(void);
void GoBackInfoNum(void);
void ExitInfoNumScreen(void);
void DisplayPremimumInfoNum(void);
void ExitDisplayPremimumInfoNum(void);
void DisplayPremimumInfoNumOK(void);
void DisplayRoamingInfoNum(void);
void ExitDisplayRoamingInfoNum(void);

#endif

