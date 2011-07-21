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
 * PersonalizationDef.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for defines of Personalization module
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

	FILENAME	: PersonalizationDef.h

  	PURPOSE		: defines of Personalization module

 

	AUTHOR		: Deepali

	DATE		: 12/03/03

**************************************************************/

#ifndef _PERSONALIZATION_DEF_H
#define _PERSONALIZATION_DEF_H

#include "mmi_data_types.h"
/*
#define MAX_NP_LENGTH_MMI					9 
#define MAX_SIMP_LENGTH_MMI					9 
#define MAX_SP_LENGTH_MMI					9 
#define MAX_CP_LENGTH_MMI					9 
#define MAX_NSP_LENGTH_MMI					9 
*/

#define MAX_ALLOWED_SP_TIMEOUT				2048

enum STR_PERSONALIZATION_LIST
{
	STR_ENTER_NP_MSG	=		SIM_DETECTION_BASE+1400,
	STR_ENTER_NSP_MSG,
	STR_ENTER_CP_MSG,
	STR_ENTER_SP_MSG,
	STR_ENTER_SIMP_MSG,
	STR_WRONG_SP_MSG,
	STR_WRONG_CP_MSG,
	STR_WRONG_NSP_MSG,
	STR_WRONG_NP_MSG,
	STR_WRONG_SIMP_MSG,
	STR_SP_NOT_ALLOWED_MSG,
	STR_PERS_RSK_SOS
};

enum SCR_PERSONALIZATION_LIST
{
	SCR_ENTER_NP		=		SIM_DETECTION_BASE+1400,						
	SCR_ENTER_NSP,
	SCR_ENTER_CP,
	SCR_ENTER_SP,
	SCR_ENTER_SIMP
};


/********************************************************/


#endif  //_PERSONALIZATION_DEF_H


