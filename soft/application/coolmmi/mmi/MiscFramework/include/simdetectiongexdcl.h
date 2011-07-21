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
 * SimDetectionGexdcl.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Global variable definitions for SimDetection modules
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

	FILENAME	: SimDetectionGexdcl.h

  	PURPOSE		: Global variable definitions for SimDetection
					module.

 

	AUTHOR		: Deepali

	DATE		: 12/03/03

**************************************************************/


#ifndef __SIM_DETECTION_EXDCL_H__
#define __SIM_DETECTION_EXDCL_H__
//#include "simdetectiontype.h"
#include "simdetectiondef.h"

//extern U16 gNoOfPLMNSupported;
/*For CM notification */
//extern U8 gForCMToIdentifySOSCall;
extern S8 sPlmnsToDisplayName[][MAX_PLMN_LEN_MMI + 1];
//extern U8 gPowerOnTillIdleApp;
/* Flag indicating network is there or not */
//extern U16 gNetWorkFlag;
/*Flag to indicate emergency cll get connected*/
/*Service types to be returned to other applications*/
//extern U8 gNoService;
//extern U8 gLimitedService;
//extern U8 gFullService; 
extern U8 gNWProviderName[MMI_SIM_NUMBER][MAX_LENGTH_DISPLAY_NAME * ENCODING_LENGTH];

//Lisen 04112005
typedef enum
{
	__112__,
#if defined(__PCS1900__ )
	__911__,
#endif	
#if defined(__MEXICO_ECC__)
	__080__,
	__060__,
	__08__,
#endif	
	MAX_EMERGENCYNUMBER
}neum_emergencynumber;

extern U16 EmergencyNumber[MAX_EMERGENCYNUMBER][4];

extern S8 gInputPwdBuffer[];


//extern U8 idleScreenFirst;
/*var hold PLMN to be used later on for displaying Roaming Symbol
  by comparing this value returned in MSG_ID_MMI_SMU_GET_IMSI_RSP*/
extern U8 gPLMN[MMI_SIM_NUMBER][MAX_PLMN_LEN_MMI+1];

/* Flag to be set when limites service or full service is available used in SOS call
   making in SimDetection.c file*/
//extern U8 gIsNetworkServiceAvailable;

//extern U8 testRoaming;
//extern U8 gTypeNoReq;
//extern U8 gRoamingFlag;

extern S8 gServProvderName[MMI_SIM_NUMBER][MAX_LENGTH_SERVICE_PROVIDER*ENCODING_LENGTH];
extern S8 gOperatorName[];
extern S8 gShortOperatorName[];
extern S8 gOwnerName[MMI_SIM_NUMBER][MAX_LENGTH_OWNER_NUMBER*ENCODING_LENGTH];
extern U8 gOpNameValid;
extern U8 gShortOpnameValid;
extern U8 gSpnValid[];
//extern U8 gPowerOnPeriod;
//extern U8 gPrevScreenIndicator;
//extern U8 gSSCFromEnterPukAtBootUp;

 
//extern U8 gRequireRPLMN;
// CSD End

// CSD 04/18/2004 Lisen : code re-org
extern pwr_context_struct g_pwr_context[];
extern charbat_context_struct g_charbat_context;

#define IS_IDLE_ARRIVED g_pwr_context[0].IdleScreenArrived
// TODO: can add more shortcut name here...
#endif

