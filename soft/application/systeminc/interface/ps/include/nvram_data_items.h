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
 *	nvram_data_items.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *    This file defines logical data items stored in NVRAM. 
 *    These logical data items are used in object code of Protocol Stack software.
 *
 *    As for customizable logical data items, they are defined in nvram_user_defs.h
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef _NVRAM_DATA_ITEMS_H
#define _NVRAM_DATA_ITEMS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Header files should be include before this one */
#ifndef _KAL_RELEASE_H
#error "kal_release.h should be included"
#endif

/* For system exception (in init\include)*/
#include "ex_item.h"
#include "stack_config.h"
#include "stack_buff_pool.h"
#include "ctrl_buff_pool.h"

#include "nvram_defs.h"
#if 0
#ifdef __SHADOW_NVRAM__
#define SHD(lid)	lid##_SHD

typedef enum {
	NVRAM_EF_SYS_STATISTICS_LID_SHD
	,NVRAM_EF_EXTBCCH_LID_SHD
	,NVRAM_EF_EQ_PLMN_LID_SHD
	,NVRAM_EF_BAND_INFO_LID_SHD
	,NVRAM_EF_TST_FILTER_LID_SHD
	,NVRAM_EF_CB_CH_INFO_LID_SHD
	,NVRAM_EF_TCM_STATISTICS_LID_SHD
	,NVRAM_EF_CFU_FLAG_LID_SHD
	/*Add before this*/
	,NVRAM_EF_LAST_VENDOR_SHD
}shadow_order_vendor_enum;

#define SHADOW_SIZE_VENDOR (GUARD_PATTERN_SIZE*2) \
/*NVRAM_EF_SYS_STATISTICS_LID    */+(NVRAM_EF_SYS_STATISTICS_SIZE*NVRAM_EF_SYS_STATISTICS_TOTAL)\
/*NVRAM_EF_EXTBCCH_LID           */+(NVRAM_EF_EXTBCCH_SIZE*NVRAM_EF_EXTBCCH_TOTAL)\
/*NVRAM_EF_EQ_PLMN_LID           */+(NVRAM_EF_EQ_PLMN_SIZE*NVRAM_EF_EQ_PLMN_TOTAL)\
/*NVRAM_EF_BAND_INFO_LID         */+(NVRAM_EF_BAND_INFO_SIZE*NVRAM_EF_BAND_INFO_TOTAL)\
/*NVRAM_EF_TST_FILTER_LID        */+(NVRAM_EF_TST_FILTER_SIZE*NVRAM_EF_TST_FILTER_TOTAL)\
/*NVRAM_EF_CB_CH_INFO_LID        */+(NVRAM_EF_CB_CH_INFO_SIZE*NVRAM_EF_CB_CH_INFO_TOTAL)\
/*NVRAM_EF_TCM_STATISTICS_LID    */+(NVRAM_EF_TCM_STATISTICS_SIZE*NVRAM_EF_TCM_STATISTICS_TOTAL)\
/*NVRAM_EF_CFU_FLAG_LID          */+(NVRAM_EF_CFU_FLAG_SIZE*NVRAM_EF_CFU_FLAG_TOTAL)

#else
#define SHD(lid)	(kal_uint8)NVRAM_INVALID_VALUE
#endif
#endif 

/** 
 * Step 1: (See comment of nvram_data_item.c for detail).
 
 * These logical data items are used in object code of Protocol Stack software.
 *
 * **** Lower boud is 1. ****
 */

typedef enum {
/* System record, keep the system version */
	NVRAM_EF_SYS_LID,
/* Branch record, keep the branch version */
	NVRAM_EF_BRANCH_VERNO_LID,
	NVRAM_EF_START,

	/*System Category starts form 1 */
	NVRAM_EF_SYS_EXCEPTION_LID = NVRAM_EF_START,
	NVRAM_EF_SYS_STATISTICS_LID, 
	NVRAM_EF_L1_START,
	NVRAM_EF_L1_AGCPATHLOSS_LID=NVRAM_EF_L1_START, 
	NVRAM_EF_L1_RAMPTABLE_GSM850_LID, 
	NVRAM_EF_L1_RAMPTABLE_GSM900_LID,		//5
	NVRAM_EF_L1_RAMPTABLE_DCS1800_LID,
	NVRAM_EF_L1_RAMPTABLE_PCS1900_LID,
	NVRAM_EF_L1_AFCDATA_LID,  
	NVRAM_EF_L1_TXIQ_LID, 
        NVRAM_EF_L1_RFSPECIALCOEF_LID,              //10 
	NVRAM_EF_L1_INTERSLOT_RAMP_GSM850_LID,	//10
	NVRAM_EF_L1_INTERSLOT_RAMP_GSM900_LID,
	NVRAM_EF_L1_INTERSLOT_RAMP_DCS1800_LID,
	NVRAM_EF_L1_INTERSLOT_RAMP_PCS1900_LID,
	NVRAM_EF_L1_CRYSTAL_AFCDATA_LID,
	NVRAM_EF_L1_CRYSTAL_CAPDATA_LID,		//15
	NVRAM_EF_L1_END = NVRAM_EF_L1_CRYSTAL_CAPDATA_LID,
	
    /*start form 16 */
 
	NVRAM_EF_EXTBCCH_LID, 
	NVRAM_EF_EQ_PLMN_LID,  
   NVRAM_EF_BAND_INFO_LID,

	NVRAM_EF_TST_FILTER_LID,
	NVRAM_EF_CB_CH_INFO_LID,    //20
	NVRAM_EF_TCM_STATISTICS_LID, 

 
	NVRAM_EF_CFU_FLAG_LID,					//23

     
    NVRAM_EF_MM_LOCIGPRS_LID,
    
	/* NVRAM_EF_LAST_LID_VENDOR = NVRAM_EF_CUST_LID  = NVRAM_LID_CUST_BEGIN = NVRAM_TOTAL_LID_VENDOR+1 = 33 */    
	NVRAM_EF_LAST_LID_VENDOR  
 } nvram_LID_vendor_enum;

/**
 * Step 2:
 * Defines constants of size and number of records for each logical data item.
 * For linear-fixed, TOTAL is greater than 1, and SIZE is size of each record;
 * for transparent, TOTAL must be exaclty 1, and SIZE is size of entire data item.
 *
 * Each logical data item must be:
 * 1> Size must be EVEN
 * 2> Size of default value must be equal to the logical data item's size.
 *
 * Notice:
 * If total number of L1 Calibration data items is changed, NVRAM_L1CAL_ELEMENT_TOTAL
 * must be modified properly.
 */



/** System record:
 * [  DATA_VERSION  ][ PADDING ][ LOCK_PATTERN ]
 *    18 bytes          4 bytes  12 bytes
 * Size must be 
 * strlen(CODED_DATA_VERSION) + strlen(CODED_PADDING_LENGTH) + strlen(NVRAM_LOCK_PATTERN) 
 * And must be even-byte aligned.
 *
 * System record is two-copied. It is controlled directly by NVRAM Layer itself.
 * Ie, their record ID's are: 1 and 2.
 */

#define NVRAM_EF_SYS_SIZE                 34
#define NVRAM_EF_SYS_TOTAL                1

#define CODED_DATA_VERSION_SIZE           18
#define CODED_PADDING_SIZE                4

#define CODED_LOCK_PATTERN_SIZE           12
#define CODED_LOCK_PATTERN_OFFSET         22




#define NVRAM_EF_BRANCH_VERNO_SIZE	NVRAM_EF_SYS_SIZE
#define NVRAM_EF_BRANCH_VERNO_TOTAL	NVRAM_EF_SYS_TOTAL

/**
 * TST Filter
 */
#define NVRAM_EF_TST_FILTER_SIZE         480
#define NVRAM_EF_TST_FILTER_TOTAL        1

/**
 * System Exception Dumping
 */
#define NVRAM_EF_SYS_EXCEPTION_SIZE       TOTAL_EXPTR_SIZE
#define NVRAM_EF_SYS_EXCEPTION_TOTAL      10

/**
 * System Statistics
 */
#define NVRAM_EF_SYS_STATISTICS_SIZE       sizeof(stack_statistics_struct)
#define NVRAM_EF_SYS_STATISTICS_TOTAL      1


/************************************************************
 * Start of L1 Calibration data
 ************************************************************/
/**
 * Total number of L1 Calibration data items
 */
#define NVRAM_L1CAL_ELEMENT_TOTAL NVRAM_EF_L1_END - NVRAM_EF_L1_START +1

/**
 * L1 AGC Path Loss
 */
#define NVRAM_EF_L1_AGCPATHLOSS_SIZE     sizeof(l1cal_agcPathLoss_T)
#define NVRAM_EF_L1_AGCPATHLOSS_TOTAL    1

/**
 * L1 Ramp Table for GSM850
 */
#define NVRAM_EF_L1_RAMPTABLE_GSM850_SIZE    sizeof(l1cal_rampTable_T)
#define NVRAM_EF_L1_RAMPTABLE_GSM850_TOTAL   1


/**
 * L1 Ramp Table for GSM900
 */
#define NVRAM_EF_L1_RAMPTABLE_GSM900_SIZE    sizeof(l1cal_rampTable_T)
#define NVRAM_EF_L1_RAMPTABLE_GSM900_TOTAL   1

/**
 * L1 Ramp Table for DCS1800
 */
#define NVRAM_EF_L1_RAMPTABLE_DCS1800_SIZE   sizeof(l1cal_rampTable_T)
#define NVRAM_EF_L1_RAMPTABLE_DCS1800_TOTAL  1

/**
 * L1 Ramp Table for PCS1900
 */
#define NVRAM_EF_L1_RAMPTABLE_PCS1900_SIZE   sizeof(l1cal_rampTable_T)
#define NVRAM_EF_L1_RAMPTABLE_PCS1900_TOTAL  1

/**
 * L1 AFC Data
 */
#define NVRAM_EF_L1_AFCDATA_SIZE         sizeof(l1cal_afcData_T)
#define NVRAM_EF_L1_AFCDATA_TOTAL        1

/**
 * L1 TXIQ Calibration data
 */
#define NVRAM_EF_L1_TXIQ_SIZE         sizeof(l1cal_txiq_T)
#define NVRAM_EF_L1_TXIQ_TOTAL        1
 
/**
 * L1 RF Special Coef Calibration data
 */
#define NVRAM_EF_L1_RFSPECIALCOEF_SIZE         sizeof(l1cal_rfspecialcoef_T)
#define NVRAM_EF_L1_RFSPECIALCOEF_TOTAL        1

/**
 * L1 Inter-slot ramp table Calibration data for GSM 850
 */
#define NVRAM_EF_L1_INTERSLOT_RAMP_GSM850_SIZE    sizeof(l1cal_interRampData_T)
#define NVRAM_EF_L1_INTERSLOT_RAMP_GSM850_TOTAL   1


/**
 * L1 Inter-slot ramp table Calibration data for GSM 900
 */
#define NVRAM_EF_L1_INTERSLOT_RAMP_GSM900_SIZE    sizeof(l1cal_interRampData_T)
#define NVRAM_EF_L1_INTERSLOT_RAMP_GSM900_TOTAL   1

/**
 * L1 Inter-slot ramp table Calibration data for DCS 1800
 */
#define NVRAM_EF_L1_INTERSLOT_RAMP_DCS1800_SIZE    sizeof(l1cal_interRampData_T)
#define NVRAM_EF_L1_INTERSLOT_RAMP_DCS1800_TOTAL   1

/**
 * L1 Inter-slot ramp table Calibration data for PCS 1900
 */
#define NVRAM_EF_L1_INTERSLOT_RAMP_PCS1900_SIZE    sizeof(l1cal_interRampData_T)
#define NVRAM_EF_L1_INTERSLOT_RAMP_PCS1900_TOTAL   1

/**
 * L1 Crystal AFC Data
 */
#define NVRAM_EF_L1_CRYSTAL_AFCDATA_SIZE         sizeof(l1cal_crystalAfcData_T)
#define NVRAM_EF_L1_CRYSTAL_AFCDATA_TOTAL        1

/**
 * L1 Crystal CAP Data
 */
#define NVRAM_EF_L1_CRYSTAL_CAPDATA_SIZE         sizeof(l1cal_crystalCapData_T)
#define NVRAM_EF_L1_CRYSTAL_CAPDATA_TOTAL        1


/************************************************************
 * End of L1 Calibration data
 ************************************************************/



/**
 * MSCAP
 */
#define NVRAM_EF_MSCAP_SIZE              4
#define NVRAM_EF_MSCAP_TOTAL             1

/**
 * EXTBCCH
 */
#define NVRAM_EF_EXTBCCH_SIZE            56
#define NVRAM_EF_EXTBCCH_TOTAL           1


/** EQ_PLMN
 * 
 */
#define NVRAM_EF_EQ_PLMN_SIZE            38
#define NVRAM_EF_EQ_PLMN_TOTAL           1


/** BAND_INFO
 * 
 */
#define NVRAM_EF_BAND_INFO_SIZE            6
#define NVRAM_EF_BAND_INFO_TOTAL           1

/**
 * Drx parameters
 */
#define NVRAM_EF_DRX_PARAM_SIZE           2
#define NVRAM_EF_DRX_PARAM_TOTAL          1


/**
 * CB Mask
 */
#define NVRAM_EF_CB_CH_INFO_SIZE              50
#define NVRAM_EF_CB_CH_INFO_TOTAL             1

/**
 * TCP statistics
 */
#define NVRAM_EF_TCM_STATISTICS_SIZE         16//22
#define NVRAM_EF_TCM_STATISTICS_TOTAL        1

/**
 * ALS LINE ID
 */
#define NVRAM_EF_ALS_LINE_ID_SIZE             2
#define NVRAM_EF_ALS_LINE_ID_TOTAL            1

/**
 * CFU FLAG
 */
#define NVRAM_EF_CFU_FLAG_SIZE             2
#define NVRAM_EF_CFU_FLAG_TOTAL            1

 
#define NVRAM_EF_MM_LOCIGPRS_SIZE 14
#define NVRAM_EF_MM_LOCIGPRS_TOTAL 1
////////////////////////////////////////////////////////////////////////////////////////////////
//changed by licheng for easily add new unit to NVRAM @20080229
#if 0
#define NVRAM_PHB_NVRAM_LID  300
#define NVRAM_DIALEDCALL_NVRAM_LID  301
#define NVRAM_RECVDCALL_NVRAM_LID  302
#define NVRAM_MISSEDCALL_NVRAM_LID  303
#define NVRAM_PHONELOCK_NVRAM_LID  304
#define NVRAM_SMSPROFNE_NVRAM_LID  305
#define NVRAM_SMSPROSC_NVRAM_LID  306
#define NVRAM_VOICEMAIL1_NVRAM_LID  307
#define NVRAM_VOICEMAIL2_NVRAM_LID  308

#define NVRAM_EF_PHB_SOMEFIELDS_LID   309

#define NVRAM_REJECTEDCALL_NVRAM_LID  310
#endif 

typedef enum 
{
  NVRAM_PHB_NVRAM_LID = 300,
  NVRAM_DIALEDCALL_NVRAM_LID,
  NVRAM_RECVDCALL_NVRAM_LID,
  NVRAM_MISSEDCALL_NVRAM_LID,
  NVRAM_REJECTEDCALL_NVRAM_LID, //added by licheng for rejected call
  NVRAM_PHONELOCK_NVRAM_LID,
  NVRAM_SMSPROFNE_NVRAM_LID,
  #ifdef __MMI_MULTI_SIM__
  NVRAM_SLAVE_SMSPROFNE_NVRAM_LID,
  NVRAM_SIM3_SMSPROFNE_NVRAM_LID,
  NVRAM_SIM4_SMSPROFNE_NVRAM_LID,
  #endif
  NVRAM_SMSPROSC_NVRAM_LID,
  NVRAM_VOICEMAIL1_NVRAM_LID,
  #ifdef __MMI_MULTI_SIM__
  NVRAM_SLAVE_VOICEMAIL1_NVRAM_LID,
  NVRAM_SIM3_VOICEMAIL1_NVRAM_LID,
  NVRAM_SIM4_VOICEMAIL1_NVRAM_LID,
  #endif
  NVRAM_VOICEMAIL2_NVRAM_LID,
  #ifdef __MMI_MULTI_SIM__
  NVRAM_SLAVE_VOICEMAIL2_NVRAM_LID,
  NVRAM_SIM3_VOICEMAIL2_NVRAM_LID,
  NVRAM_SIM4_VOICEMAIL2_NVRAM_LID,
  #endif
  NVRAM_EF_PHB_SOMEFIELDS_LID
}NVRAM_UNIT;
////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#endif /* define _NVRAM_DATA_ITEMS_H */



