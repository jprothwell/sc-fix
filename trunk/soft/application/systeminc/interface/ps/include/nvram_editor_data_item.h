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
 *	nvram_editor_data_item.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for NVRAM editor.
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
  *------------------------------------------------------------------------------
 
 *==============================================================================
 ****************************************************************************/

#ifndef _NVRAM_EDTIOR_DATA_ITEM_SYSTEM_H
#define _NVRAM_EDTIOR_DATA_ITEM_SYSTEM_H

#ifndef __L1_STANDALONE__
/*****
  **
  ** This is structure for NVRAM_EF_MS_SECURITY_LID
  **
  *****/

#if 0  
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif  

typedef struct
{
   kal_uint8                cfu_flag[2];
}nvram_ef_cfu_flag_struct;


typedef struct _nvram_ef_cb_ch_info_struct 
{
		kal_uint16	cbmir_mask;
      kal_uint16	dcs_mask;
		kal_uint8	cbmi_mask[6];
		kal_uint16	cbmi[20];
}nvram_ef_cb_ch_info_struct ;




/*****
  **
  ** This is special case to include header files inside header files
  **
  **     The META tool should know the data structure using by each logical data item
  **   so that it is able to edit the data item. These data structures will be parsed
  **   by codegen and than generate database. The sturcture is originally maintained
  **   by each modules, include these header files so that data structures could be referred to. 
  **
  **   Notices:
  **     Include this "nvram_editor_data_item.h" will cause subsequently including of 
  **   following header files:
  **     
 
  **     ps/interfaces/local_inc
  **
  **   Make sure your include path is right or compile error will occur.
  **
  *****/
#include "l4c2uem_struct.h"
#include "device.h"
#include "mcd_l4_common.h"


/*****
  **
  ** This is structure for NVRAM_EF_UEM_RMI_DATA_LID
  **
  *****/

typedef struct 
{
   kal_uint32	total_rx_data;
   kal_uint32	total_tx_data;
   kal_uint32	last_rx_data;
   kal_uint32	last_tx_data;
} nvram_ef_tcm_statistics_struct;

#endif /* !__L1_STANDALONE__*/
#define VER_LID(lid)	lid##_VERNO lid
#define VER(lid)	lid##_VERNO

#define	NVRAM_EF_SYS_EXCEPTION_LID_VERNO		"000" 
#define	NVRAM_EF_SYS_STATISTICS_LID_VERNO               "000" 
#define	NVRAM_EF_L1_AGCPATHLOSS_LID_VERNO               "000" 
#define	NVRAM_EF_L1_RAMPTABLE_GSM850_LID_VERNO          "001" 
#define	NVRAM_EF_L1_RAMPTABLE_GSM900_LID_VERNO          "001" 
#define	NVRAM_EF_L1_RAMPTABLE_DCS1800_LID_VERNO         "001" 
#define	NVRAM_EF_L1_RAMPTABLE_PCS1900_LID_VERNO         "001" 
#define	NVRAM_EF_L1_AFCDATA_LID_VERNO                   "000" 
#define	NVRAM_EF_L1_TXIQ_LID_VERNO                      "002" 
#define	NVRAM_EF_L1_RFSPECIALCOEF_LID_VERNO             "000" 
#define	NVRAM_EF_L1_INTERSLOT_RAMP_GSM850_LID_VERNO     "000" 
#define	NVRAM_EF_L1_INTERSLOT_RAMP_GSM900_LID_VERNO     "000" 
#define	NVRAM_EF_L1_INTERSLOT_RAMP_DCS1800_LID_VERNO    "000" 
#define	NVRAM_EF_L1_INTERSLOT_RAMP_PCS1900_LID_VERNO    "000" 
#define	NVRAM_EF_L1_CRYSTAL_AFCDATA_LID_VERNO           "000" 
#define	NVRAM_EF_L1_CRYSTAL_CAPDATA_LID_VERNO           "000" 
#define	NVRAM_EF_EXTBCCH_LID_VERNO                     "000" 
#define	NVRAM_EF_EQ_PLMN_LID_VERNO                      "000" 
#define	NVRAM_EF_BAND_INFO_LID_VERNO                    "000" 
#define	NVRAM_EF_TST_FILTER_LID_VERNO                   "002" 
#define	NVRAM_EF_CB_CH_INFO_LID_VERNO                   "001" 
#define	NVRAM_EF_TCM_STATISTICS_LID_VERNO               "001" 

#define NVRAM_EF_BT_INFO_LID_VERNO "000"

#if 0 
/* under construction !*/
/* under construction !*/
#endif  

#define	NVRAM_EF_CFU_FLAG_LID_VERNO                     "000" 

 
#define NVRAM_EF_MM_LOCIGPRS_LID_VERNO "000"

#ifdef GEN_FOR_PC

//Lisen 0604 TST
typedef  struct { 
   kal_uint8 nvram_ef_tst_filter[NVRAM_EF_TST_FILTER_SIZE ];
} nvram_ef_tst_filter_struct;

#ifndef __L1_STANDALONE__
#if 0 
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif  

typedef  struct { 
 
/* Jinghan 20041211 merged EXT-BCCH to one file */
   kal_uint8 mcc1;
   kal_uint8 mcc2;
   kal_uint8 mcc3;
   kal_uint8 mnc1;
   kal_uint8 mnc2;
   kal_uint8 mnc3;               
   kal_uint8 arfcn_MSB;
   kal_uint8 arfcn_LSB;
   kal_uint8 byte[48];  // 16 for SI2bis, 16 for SI2ter, 16 for BA range
} nvram_ef_extbcch_struct;

typedef  struct { 
   kal_uint8 byte[NVRAM_EF_EQ_PLMN_SIZE];
} nvram_ef_eq_plmn_struct;

typedef  struct { 
   kal_uint8 byte[NVRAM_EF_BAND_INFO_SIZE];
} nvram_ef_band_info_struct;

typedef struct
{
		kal_uint16	cbmi_mask;
		kal_uint16	cbmir_mask;
      kal_uint16	dcs_mask;
}nvram_ef_cb_mask_struct;

 
typedef struct{
	kal_uint8 byte[NVRAM_EF_MM_LOCIGPRS_SIZE];
}nvram_ef_mm_locigprs_struct;

#endif /* !__L1_STANDALONE__ */


BEGIN_NVRAM_DATA

/************************************************
 ***  Section Name :  System Debugging information
 ************************************************/
/***********************************************************************
  *** This is a nvram data item bit level description for meta tools nvram editor
  ***
  *** Logical Data Item ID : NVRAM_EF_SYS_EXCEPTION_LID
  ***
  *** Module: L1
  ***
  *** Description:
  ***
  *** Maintainer:
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_SYS_EXCEPTION_LID) ex_exception_record * NVRAM_EF_SYS_EXCEPTION_TOTAL
{
};


 /***********************************************************************
  *** This is a nvram data item bit level description for meta tools nvram editor
  ***
  *** Logical Data Item ID : NVRAM_EF_SYS_STATISTICS_LID
  ***
  *** Module: L1
  ***
  *** Description:
  ***
  *** Maintainer:
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_SYS_STATISTICS_LID) stack_statistics_struct * NVRAM_EF_SYS_STATISTICS_TOTAL 
{
};




/******************************
 ***  Section:  L1 Calibration Data
 ******************************/

 /*****
   **
   ** Notes:  The defination of L1 calibration data structure is located at 
   **            "inc/l1cal.h". Here, provide decriptions only.
   **
   *****************
   **
   **             typedef struct
   **            {
   **                sAGCGAINOFFSET     agcPathLoss[FrequencyBandCount][PLTABLE_SIZE];
   **            }l1cal_agcPathLoss_T;
   ** 
   **            typedef struct
   **             {
   **               sRAMPDATA          rampData;
   **             }l1cal_rampTable_T;
   **
   **             typedef struct
   **            {
   **                kal_uint16         dacValue;
   **               kal_int32          slopeInv;
   **             }l1cal_afcData_T;
   **
   **             typedef struct
   **            {
   **                kal_int16 filter_coefficient[60];
   **            }l1spfc_T;
   ** 
   **            typedef struct
   **             {
   **               kal_int8     TxTrimI;
   **                kal_int8     TxTrimQ;
   **               kal_int8     TxOffsetI;
   **                kal_int8     TxOffsetQ;
   **            }l1cal_txiq_T;
   ** 
   ********/

 /***********************************************************************
  *** This is a nvram data item bit level description for meta tools nvram editor
  ***
  *** Logical Data Item ID : NVRAM_EF_L1_AGCPATHLOSS_LID
  ***
  *** Module: L1
  ***
  *** Description:
  ***
  *** Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_AGCPATHLOSS_LID) l1cal_agcPathLoss_T * NVRAM_EF_L1_AGCPATHLOSS_TOTAL
{

};



/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_RAMPTABLE_GSM850_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_RAMPTABLE_GSM850_LID) l1cal_rampTable_T  * NVRAM_EF_L1_RAMPTABLE_GSM850_TOTAL
{

};



/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_RAMPTABLE_GSM900_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_RAMPTABLE_GSM900_LID) l1cal_rampTable_T  * NVRAM_EF_L1_RAMPTABLE_GSM900_TOTAL
{

};




/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_RAMPTABLE_DCS1800_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_RAMPTABLE_DCS1800_LID) l1cal_rampTable_T * NVRAM_EF_L1_RAMPTABLE_DCS1800_TOTAL
{
};




/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_RAMPTABLE_PCS1900_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_RAMPTABLE_PCS1900_LID) l1cal_rampTable_T  * NVRAM_EF_L1_RAMPTABLE_DCS1800_TOTAL
{
};



/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_AFCDATA_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_AFCDATA_LID) l1cal_afcData_T  * NVRAM_EF_L1_AFCDATA_TOTAL
{
};


/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_TXIQ_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_TXIQ_LID) l1cal_txiq_T * NVRAM_EF_L1_TXIQ_TOTAL
{
};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_RFSPECIALCOEF_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_RFSPECIALCOEF_LID) l1cal_rfspecialcoef_T * NVRAM_EF_L1_RFSPECIALCOEF_TOTAL
{
};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_INTERSLOT_RAMP_GSM850_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_INTERSLOT_RAMP_GSM850_LID) l1cal_interRampData_T  * NVRAM_EF_L1_INTERSLOT_RAMP_GSM850_TOTAL
{
};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_INTERSLOT_RAMP_GSM900_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_INTERSLOT_RAMP_GSM900_LID) l1cal_interRampData_T  * NVRAM_EF_L1_INTERSLOT_RAMP_GSM900_TOTAL
{
};


/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_INTERSLOT_RAMP_DCS1800_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_INTERSLOT_RAMP_DCS1800_LID) l1cal_interRampData_T  * NVRAM_EF_L1_INTERSLOT_RAMP_DCS1800_TOTAL
{
};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_INTERSLOT_RAMP_PCS1900_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_INTERSLOT_RAMP_PCS1900_LID) l1cal_interRampData_T  * NVRAM_EF_L1_INTERSLOT_RAMP_PCS1900_TOTAL
{
};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_CRYSTAL_AFCDATA_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_CRYSTAL_AFCDATA_LID) l1cal_crystalAfcData_T  * NVRAM_EF_L1_CRYSTAL_AFCDATA_TOTAL
{
};

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_L1_CRYSTAL_CAPDATA_LID
  ***
  ***  Module: L1
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_L1_CRYSTAL_CAPDATA_LID) l1cal_crystalCapData_T  * NVRAM_EF_L1_CRYSTAL_CAPDATA_TOTAL
{
};




/***********************************
 ***  Section:  System Level Data Item
 ***********************************/

#ifndef __L1_STANDALONE__
#if 0  
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif 

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_EXTBCCH_LID
  ***
  ***  Module: 
  ***
  ***  Description:
  ***
 
  ***
  ***********************************************************************/
    
/* Jinghan 20041210 merged EXT-BCCH to a file */
LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_EXTBCCH_LID) nvram_ef_extbcch_struct * NVRAM_EF_EXTBCCH_TOTAL
{
   mcc1 : "mobile country code 1 " {};
   mcc2 : "mobile country code 2 " {};
   mcc3 : "mobile country code 3 " {};
   mnc1 : "mobile network code 1 " {};
   mnc2 : "mobile network code 2 "{};
   mnc3 : "mobile network code 3 "{};   
   arfcn_MSB: "last time serving cell arfcn MSB "{};
   arfcn_LSB: "last time serving cell arfcn LSB "{};
   byte: "Extended BCCH Frequency List (Refer to 04.18 10.5.2.22, Neighbour Cell Description" {};
};

LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_EQ_PLMN_LID) nvram_ef_eq_plmn_struct * NVRAM_EF_EQ_PLMN_TOTAL {};

LID_BIT MULTIPLE_LID VER_LID(NVRAM_EF_BAND_INFO_LID) nvram_ef_band_info_struct * NVRAM_EF_BAND_INFO_TOTAL {};


/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_TST_FILTER_LID
  ***
  ***  Module: TST
  ***
  ***  Description:
  ***
  ***  Maintainer:
  ***
  ***********************************************************************/
//Lisen 0604 TST
LID_BIT VER_LID(NVRAM_EF_TST_FILTER_LID) nvram_ef_tst_filter_struct * NVRAM_EF_TST_FILTER_TOTAL
{
    nvram_ef_tst_filter: "tst specific data" {};
};



/***********************************
 ***  Section:  User Level Data Item
 ***********************************/

/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_CB_CH_INFO_LID
  ***
  ***  Module: SMS
  ***
  ***  Description:  
  ***
 
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_CB_CH_INFO_LID) nvram_ef_cb_ch_info_struct * NVRAM_EF_CB_CH_INFO_TOTAL 
{
	cbmir_mask: "CBMIR MASK"
	{
	   cbmir_0: 1 "CBMIR [0]"
	   {
	      0x0: "Disable";
	      0x1: "Enable";
	   };

	   cbmir_1: 1 "CBMIR [1]"
	   {
	      0x0: "Disable";
	      0x1: "Enable";
	   };

	   cbmir_2: 1 "CBMIR [2]"
	   {
	      0x0: "Disable";
	      0x1: "Enable";
	   };

	   cbmir_3: 1 "CBMIR [3]"
	   {
	      0x0: "Disable";
	      0x1: "Enable";
	   };

	   cbmir_4: 1 "CBMIR [4]"
	   {
	      0x0: "Disable";
	      0x1: "Enable";
	   };

	   cbmir_5: 1 "CBMIR [5]"
	   {
	      0x0: "Disable";
	      0x1: "Enable";
	   };

	   cbmir_6: 1 "CBMIR [6]"
	   {
	      0x0: "Disable";
	      0x1: "Enable";
	   };

	   cbmir_7: 1 "CBMIR [7]"
	   {
	      0x0: "Disable";
	      0x1: "Enable";
	   };

      cbmir_8_15: 8 "CBMIR [8] ~ [15], Same as above"
      {
      };
	};
	

   dcs_mask: "DCS MASK" 
   {
      dcs_mask_0: 8 "DCS [0] ~ DCS [7]"
      {
      };

      dcs_mask_8: 8 "DCS [8] ~ DCS [15]"
      {
      };

   };
   
   cbmi_mask: "CBMI MASK" 
   {
      cbmi_mask: 24 "CBMI_MASK for ME"
      {
      };
      
      cbmi_mask: 24 "CBMI_MASK for SIM"
      {
      };
   };

   cbmi: "CBMI storage in ME";
};


/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_TCM_STATISTICS_LID
  ***
  ***  Module: 
  ***
  ***  Description:  
  ***
  ***  Maintainer: 
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_TCM_STATISTICS_LID) nvram_ef_tcm_statistics_struct * NVRAM_EF_TCM_STATISTICS_TOTAL
{
   total_rx_data: "Total received bytes" {};
   total_tx_data: "Total transmitted bytes" {};
   last_rx_data: "Received bytes of last" {};
   last_tx_data: "Transmitted bytes of last call" {};
};

#if 0  
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif  
/***********************************************************************
  ***  This is a nvram data item bit level description for meta tools nvram editor
  ***
  ***  Logical Data Item ID : NVRAM_EF_CFU_FLAG_LID
  ***
  ***  Module: 
  ***
  ***  Description:  
  ***
 
  ***
  ***********************************************************************/
LID_BIT VER_LID(NVRAM_EF_CFU_FLAG_LID) nvram_ef_cfu_flag_struct * NVRAM_EF_CFU_FLAG_TOTAL
{
   cfu_flag: "CFU flag" {};
};

 
LID_BIT VER_LID(NVRAM_EF_MM_LOCIGPRS_LID) nvram_ef_mm_locigprs_struct * NVRAM_EF_MM_LOCIGPRS_TOTAL{};

#endif /* !__L1_STANDALONE__ */

END_NVRAM_DATA

#endif /* GEN_FOR_PC */

#endif /* _NVRAM_EDTIOR_DATA_ITEM_SYSTEM_H */

