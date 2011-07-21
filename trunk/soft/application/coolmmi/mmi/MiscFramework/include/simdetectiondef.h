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
 * SimDetectionDef.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is for #defines of Boot Up module
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: SimDetectionDef.h

  	PURPOSE		: defines of Boot Up module

 

	AUTHOR		: Deepali

	DATE		: 12/03/03

**************************************************************/

#ifndef _SIM_DETECTION_DEF_H
#define _SIM_DETECTION_DEF_H

#include "mmi_data_types.h"

#define MAX_PWD_LENGTH_MMI			   		9 
#define MMI_MIN_PASSWORD_LENGTH				4
#define MMI_MIN_PUK_LENGTH					8

#define ENTER_PIN_SCR      					1
#define ENTER_PUK_SCR	   					2 
#define ENTER_SIM_SCR	   					3
#define ENTER_PHONELOCK	   					4
#define ENTER_SIM_INSERTION	  	  			5
#define ENTER_PUK_BLOCKED   				6
#define ENTER_SIM_BLOCK						7
#define ENTER_CONFIRM_PIN					8
#define ENTER_NEW_PIN						9
#define ENTER_NP							10
#define ENTER_SP							11
#define ENTER_CP							12
#define ENTER_NSP							13
#define ENTER_SIMP							14
#define ENTER_IDLE_SCR						15

/*Defines for Network detection */
/*Fix Bug#12330 for cs2324c by cong.li on 2009.06.24. 
   PLMN=MCC(3)+MNC(2), such as: 46000, 46001*/
#define MAX_PLMN_LEN_MMI             		6
#define MAX_LENGTH_DISPLAY_NAME				20

//micha0406, moved to customer folder
#if (0)
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

#define MAX_APHORISM_LIST_LENGTH			35
#define MAX_INSERT_SCR_DIGITS				13
#define MAX_ECC_NUMBER_LIST_LENGTH			5
#define MAX_LENGTH_SERVICE_PROVIDER			17
#define MAX_LENGTH_OPERATOR_NAME	 		21
//#define MAX_MSISDN_NUM						4
#define MAX_LENGTH_OWNER_NUMBER		 		41
#define MAX_LENGTH_SHORT_OPERATOR_NAME 		11


/* Battery Signal Strength Macros*/

 
//                0 means no signal strength, 99 means invaild pattern
// Because NetWorkSignalLevelIndication() use "less than", so SIGNAL_STRENGTH_LEVEL5 shall be 32
/*
	> -84 dBm : 4
	-85~-89dBm : 3
	-90~-94dBm : 2
	-100~ -95 dBm : 1
	< 100dBm : 0

Defined values

<rssi>:
0 -113 dBm or less
1 -111 dBm
2...30 -109... -53 dBm
31 -51 dBm or greater
99 not known or not detectable
	
*/

/*
0	-113    1	-111    2	-109    3	-107    4	-105
5	-103    6	-101    7	-99     8	-97     9	-95
10	-93     11	-91     12	-89     13	-87     14	-85
15	-83     16	-81     17	-79     18	-77     19	-75
20	-73     21	-71     22	-69     23	-67     24	-65
25	-63     26	-61     27	-59     28	-57     29	-55
30	-53     31	-51
*/
#define	SIGNAL_STRENGTH_NONE			0
#define	SIGNAL_STRENGTH_INVAILD			99
#define	SIGNAL_STRENGTH_LEVEL1			5 //5 : -103dBm
#define	SIGNAL_STRENGTH_LEVEL2			7 //7 : -99dBm
#define	SIGNAL_STRENGTH_LEVEL3			10//10: -93dBm
#define	SIGNAL_STRENGTH_LEVEL4			13//13: -87dBm
#define	SIGNAL_STRENGTH_LEVEL5			16//16: -81dBm
/* End Battery Signal Strength Macros*/
//CSD end


//#define BOOTUP_POPUP_TIMEOUT								2000
#define CODE_VERIFIED_TIMEOUT								2000

#define APHORISM_SCR_TIME									2000
#define WELCOME_SCR_TIME									2000		

 
/*
#ifdef MMI_ON_WIN32
	#define INITNVRAM_TIME										2000
#else
	#define INITNVRAM_TIME										200
#endif
*/
// CSD end

enum STR_SIM_DETECTION_LIST
{
	STR_ENTER_PIN_MSG	=	SIM_DETECTION_BASE+1,	
	STR_PIN_BLOCKED_MSG,					

	STR_ENTER_PUK_MSG,			
#ifdef __MMI_MULTI_SIM__
	STR_ENTER_SIM1_PUK_MSG,
	STR_ENTER_SIM2_PUK_MSG,
	STR_ENTER_SIM3_PUK_MSG,
	STR_ENTER_SIM4_PUK_MSG,
#endif

	STR_INCORRECT_PIN,					

	STR_PIN_BLOCKED_PUK,				
#ifdef __MMI_MULTI_SIM__
	STR_SIM1_PIN_BLOCKED_PUK,
	STR_SIM2_PIN_BLOCKED_PUK,
	STR_SIM3_PIN_BLOCKED_PUK,
	STR_SIM4_PIN_BLOCKED_PUK,
#endif

	STR_DIAL_EMERGENCY_NUMBER,

	STR_ENTER_NEW_PIN_MSG,
	STR_RSK_SOS,		

	STR_ENTER_PIN_AGAIN_MSG,

	STR_PIN_DONOT_MATCH,					

	//KPAD_SCR_EMERGENCY_CALL_PIN_PUK_CANCEL,	  

	STR_ENTER_PHONELOCK_MSG,			
	STR_SUBLCD_ENTER_PHONELOCK_MSG,
	STR_INCORRECT_PUK,							

	STR_INCORRECT_PHONELOCK	,							

	STR_CLEAR_FOR_CATEGORY5_DUMMY,				

	STR_NO_SERVICE_RUNNING_TEXT,					

	STR_INVALID_PINPUKPLOCK_LENGTH_RUNNING_TEXT,

	STR_SIM_OPTION,								

	STR_SIM_OPTION_CAPTION,						

//	STR_SIM_INSERTION_RSK,					
	STR_SIM_INSERTION_MSG,
	STR_SUBLCD_SIM_INSERTION_MSG,
	
	STR_SIM_ACCESS_ERROR_MSG,
	STR_SUBLCD_SIM_ACCESS_ERROR_MSG,

//micha0923, #169 begn
	STR_SIM_INVALID_SIM_TITLE,
	STR_SIM_INVALID_SIM_TXT,
	STR_SIM_INVALID_SIM_SELECT_NETWORK_TXT,
//micha0923, #169 end	

	STR_APHORISM_TEXT,						

	STR_WELCOME_SCR_TEXT,					

	STR_SUBLCD_ENTER_PIN_MSG,
	STR_SUBLCD_ENTER_PUK_MSG,

	STR_NOT_SOS_NUMBER,
    STR_CODE_VERIFIED,
	STR_INCORRECT_SSC,

	STR_LIMITED_SERVICE,
	STR_NO_SERVICE,

	STR_SIM_BLOCK_MSG,
	STR_SUBLCD_SIM_BLOCK_MSG,
	STR_APHORISM_TEXT_1,
	STR_APHORISM_TEXT_2,
	STR_APHORISM_TEXT_3,
	STR_APHORISM_TEXT_4,
	STR_APHORISM_TEXT_5,
	STR_APHORISM_TEXT_6,
	STR_APHORISM_TEXT_7,
	STR_APHORISM_TEXT_8,
	STR_APHORISM_TEXT_9,
	STR_APHORISM_TEXT_10,
	STR_APHORISM_TEXT_11,
	STR_APHORISM_TEXT_12,
	STR_APHORISM_TEXT_13,
	STR_APHORISM_TEXT_14,
	STR_APHORISM_TEXT_15,
	STR_APHORISM_TEXT_16,
	STR_APHORISM_TEXT_17,
	STR_APHORISM_TEXT_18,
	STR_APHORISM_TEXT_19,
	STR_APHORISM_TEXT_20,
	STR_APHORISM_TEXT_21,
	STR_APHORISM_TEXT_22,
	STR_APHORISM_TEXT_23,
	STR_APHORISM_TEXT_24,
	STR_APHORISM_TEXT_25,
	STR_APHORISM_TEXT_26,
	STR_APHORISM_TEXT_27,
	STR_APHORISM_TEXT_28,
	STR_APHORISM_TEXT_29,
	STR_APHORISM_TEXT_30,
	STR_APHORISM_TEXT_31,
	STR_APHORISM_TEXT_32,
	STR_APHORISM_TEXT_33,
	STR_APHORISM_TEXT_34,
//micha0406, STR_PLMN_* moved to customer folder
//	STR_APHORISM_TEXT_35,
	STR_APHORISM_TEXT_35,
	STR_NAND_CHECK_TITLE,	
	STR_NAND_CHECK,
	STR_NAND_CHECK_PROCESSING,
	STR_ENTER_CARD1_PIN_MSG,
	STR_ENTER_CARD2_PIN_MSG,
	STR_ENTER_CARD3_PIN_MSG,
	STR_ENTER_CARD4_PIN_MSG,
};



enum IMG_SIM_DETECTION_LIST
{
	IMG_ENTER_PIN_CAPTION	=		SIM_DETECTION_BASE+1,				
	IMG_ENTER_PIN_AGAIN_MSG,				

	IMG_ENTER_PUK_CAPTION,				

	IMG_ENTER_NEW_PIN_FIRST_TIME_CAPTION,	

	IMG_ENTER_NEW_PIN_SECOND_TIME_CAPTION,	

	IMG_ENTER_PHONE_LOCK_CAPTION,				

	IMG_APHORISM_SCR,						
	IMG_SUBLCD_POWER_ON_SCR,
	IMG_WELCOME_SCR						
	
};


enum SCR_SIM_DETECTION_LIST
{
	SCR_ENTER_PIN		=		SIM_DETECTION_BASE+1,						
	SCR_NO_SERVICE,								
	SCR_SIM_OPTION,								
	SCR_SIM_INSERTION,						
	SCR_PUK_BLOCKED,
	SCR_SIM_BLOCK,
	SCR_ENTER_PUK,								
	SCR_ENTER_CONFIRM_PIN,								
	SCR_ENTER_NEW_PIN,
	SCR_ENTER_PHONELOCK,
	SCR_DIAL_SOS_NUM,
	SCR_MAKE_SOS_CALL,
	SCR_BOOTUP_POPUP,
	SCR_SSC_ENTER_PUK,
	SCR_POP_UP_HISTORY,
	SCR_CODE_VERIFY,
	APHORISM_SCREEN_ID,
	WELCOME_SCREEN_ID,
	SCR_INVALID_SIM_INFO, 
	SCR_NAND_FLASH_CHECK,
	SCR_NAND_CHECK_PROCESSING
       
};

typedef enum {
NO_SERVICE,
LIMITED_SERVICE,
FULL_SERVICE
}SERVICE_TYPE ;

typedef enum
{
   MMI_TYPE_NO_REQUIRED = 100, //Becuase the values in MMISMUSECURITYTYPEENUM are same as the value in SMUSECURITYTYPE,
								// add 100 to distinguish the two values				
   MMI_TYPE_CHV_PASSED,		// CHV passed.
   MMI_TYPE_PHONELOCK,
   MMI_TYPE_CHV1,
   MMI_TYPE_CHV2,
   MMI_TYPE_UBCHV1,
   MMI_TYPE_UBCHV2,
   MMI_TYPE_NP,
   MMI_TYPE_NSP,
   MMI_TYPE_SP,
   MMI_TYPE_CP,
   MMI_TYPE_IMSI_LOCK,
   MMI_TYPE_PERSONALIZATION_KEY,
   MMI_TYPE_CHV1_BLOCKED,
   MMI_TYPE_CHV2_BLOCKED,
   MMI_TYPE_SIM_CARD_BLOCKED,
   MMI_TYPE_UBCHV2_BLOCKED,
   MMI_TYPE_UBCHV1_CHV1_NOT_LOCK,
   MMI_TYPE_UNSPECIFIED
}MMISMUSECURITYTYPEENUM;

typedef enum
{
   MMI_SIM_CARD_REMOVED,
   MMI_SIM_ACCESS_ERROR,
   MMI_SIM_REFRESH   

}MMISIMERRORCAUSEENUM;

typedef enum
{
   MMI_SIM_CMD_SUCCESS        = 2560,
   MMI_SIM_CMD_FAIL           = 2561,
   MMI_SIM_FATAL_ERROR        = 2562,
   MMI_SIM_NO_INSERTED        = 2563,
   MMI_SIM_CHV_NOT_INIT       = 2564,
   MMI_SIM_CHV_RETRY_ALLOW    = 2565,
   MMI_SIM_CHV_BLOCK          = 2566,
   MMI_SIM_ACCESS_NOT_ALLOW   = 2567,
   MMI_SIM_SAT_CMD_BUSY       = 2568,
   MMI_SIM_DL_ERROR           = 2569,
   MMI_SIM_MEMORY_PROBLEM     = 2570,
   MMI_SIM_TECHNICAL_PROBLEM  = 2571,
   MMI_SIM_EF_RECORD_FULL     = 2573 
}MMISMURESULTCMDRESULTENUM;

/* Enum for results of Network Attached Indecation */
typedef enum
{
	MMI_RAC_OK,
	MMI_RAC_NO_CELL,
	MMI_RAC_LIMITED_SERVICE,
//micha0923, #169	
	MMI_RAC_ERROR,
	MMI_RAC_INVALID_SIM,
//micha1119, vito's request	
	MMI_RAC_ATTEMPT_TO_UPDATE,
	MMI_RAC_SEARCHING /* 6 */
} MMIRACRESPONSEENUM;



/*Enum for Service Type */
typedef enum
{
   MMI_REG_STATE_NOT_REGISTERED=0,
   MMI_REG_STATE_REGISTERED,
   MMI_REG_STATE_SEARCHING,
   MMI_REG_STATE_REG_DENIED,
   MMI_REG_STATE_UNKNOWN,
   MMI_REG_STATE_NOT_ACTIVE

} REGSTATEENUM;     //reg_state_enum;



/********************************************************/
typedef struct 
{
   U8 	PowerOnPeriod;
   U8 	PowerOnTillIdleApp;
   U8 	IdleScreenArrived;
   U8 	PowerOnAnimationComplete;
   U8 	PasswdReqIndFlag;
   U8 	SimQueryFailIndFlag;
   U16 	NetWorkFlag; 				/* Flag indicating network is there or not */
   U8 	IsCampOnTonePlayed;
   U8 	testRoaming;			/*
									 1: Never send get IMEI request
									 2: Waiting get IMEI response
									 3: IMEI response has came  */

   U8 	CurrentServiceType;			/*Service types to be returned to other applications*/
   U8 	AbnormalReset; 				 
   U16 	NoOfPLMNSupported;
   U8 	PrevScreenIndicator;
   U8 	ForCMToIdentifySOSCall;   	/*For CM notification */
   U8 	InvalidSim;
   U8 	InvalidSimShowed;
   U8 	CurrentCampOnBand; 		 
   U8 	RequireRPLMN; 				 
   U8 	RoamingFlag;
   U8 	SSCFromEnterPukAtBootUp;

   U8 	SimRemoveFlag;
   U8 	SOSCallFlagTOAvoidHistorySave;

   U8 	IsSimValid; 					 
   U8   PowerOnMode;  //Power on Mode

#ifdef __MANUAL_MODE_NW_SEL__
   U8	PLMNSelectionMode;
#endif 

 
   U8	actFlightMode;	// Normal mode (0) or Filght mode (1) only
 

	/*WUZC Add Start For MMIFS Ver:    on 2007-1-12 18:43 */
	U8 PowerOnSIMStatus;
	/*WUZC Add End  For MMIFS Ver:    on 2007-1-12 18:43 */
		U8  PhoneLockProcesing;/*wangrui add 2008.11.27 for fixbug10388  */

} pwr_context_struct; 

typedef struct 
{
   U8 	PowerOnCharger;				// Tells if the power on is bcos of charger
   U8 	PowerOnChargingPeriod;			// Tells if charging is going on during power on charger
   U8 	ChargingAbnormal;				// If abnormal case of charging.
   U8 	ChargingComplete;				 
   U8 	LowBatteryFlag;					//Tels if Battery is low.
   U8 	LowBatteryTxProhibitFlag;
   U8 	chargerLevel;  
   U8 	batteryStatus; 
   U8 	isChargerConnected;				//Tells if charger is connected
   U8 	LowBatteryCutOffCallCnt;
} charbat_context_struct;

extern BOOL HasValidSIM(void);

typedef enum
{
    MMI_SIM_STATUS_INVALID              = 0x0001,   /* invalid SIM */
    MMI_SIM_STATUS_NOT_INSERT           = 0x0002,   /* SIM not inserted */
    MMI_SIM_STATUS_REFRESH              = 0x0004,   /* SIM refresh */
    MMI_SIM_STATUS_SECURITY_CHECK       = 0x0008,   /* SIM need to check security after power on animation */
    MMI_SIM_STATUS_FAILURE_RSP          = 0x0010,   /* SIM failure response received */
    MMI_SIM_STATUS_ROAMING              = 0x0020,   /* Roaming */
    MMI_SIM_STATUS_BT_ACCESS            = 0x0040,   /* SIM access profile */
    MMI_SIM_STATUS_3G                   = 0x0080,   /* 3G SIM (reserved) */
    MMI_SIM_STATUS_DUALSIM_DISCONNECTED = 0x0100,   /* dualsim (gsm+cdma) disconnected */
    MMI_SIM_STATUS_ACCESS_ERROR         = 0x0200    /* SIM access error */
} POWERONSIMSTATUS;


#endif  //_SIM_DETECTION_DEF_H


