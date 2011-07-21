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
 * nvram_user_config.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *    This file is for customers to config/customize their parameters to NVRAM Layer and
 *    Driver Layer.
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
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifdef MMI_ON_HARDWARE_P
#include "kal_release.h"

#include "nvram_defs.h"
#include "nvram_user_defs.h"
#include "nvram_editor_data_item.h"  
#include "nvram_default_audio.h"
#include "custom_hw_default.h"
#include "custom_mmi_default_value.h"
#include "custom_nvram_editor_data_item.h"

//added by zhoumn @2006/11/28>>>start
#include "cswtype.h"
//added by zhoumn @2006/11/28>>>end
#include "phonebookdef.h"

#undef _FILE_CODE_
#define _FILE_CODE_ _NVRAM_USER_CONFIG_C_


/* DO NOT MODIFY THIS */
 
kal_uint8 const NVRAM_TOTAL_LID = NVRAM_EF_LAST_LID_CUST+(NVRAM_EF_LAST_LID_CUSTPACK-NVRAM_EF_CUSTPACK_BASE);
kal_uint8 const NVRAM_LAST_CUST_LID = NVRAM_EF_LAST_LID_CUST;
kal_uint8 const NVRAM_LAST_CUSTPACK_LID = NVRAM_EF_LAST_LID_CUSTPACK;
#if 0
ltable_entry_struct* logical_data_item_table[NVRAM_EF_LAST_LID_CUST+(NVRAM_EF_LAST_LID_CUSTPACK-NVRAM_EF_CUSTPACK_BASE)];
 

/* Below are customizable */

/* The pattern that defiines a LOCKED status. Choose one.*/
/* kal_uint8* CODED_LOCK_PATTERN = (kal_uint8*)NVRAM_LOCK_PATTERN_LOCKED; */
kal_uint8* CODED_LOCK_PATTERN = (kal_uint8*)NVRAM_LOCK_PATTERN_UNLOCKED;

/**
 * Hardcoded secret key. Always 8 bytes.
 * Note that this key will be used to decrypt LOCK_PATTERN.
 */
const kal_uint8* CODED_SECRET_KEY = (kal_uint8*)"12abcdef";

/**
 * Step 6:
 * Hardcoded data version of this version of source code. Always 16 bytes. 
 * MUST BE *CHANGED* as long as there is any modification to data items of 
 * `logical_data_item_table_vendor' and/or `logical_data_item_table_cust'.
 * (Of course the modification is to data items with category of NVRAM_CATEGORY_USER).
 */
const kal_uint8* CODED_DATA_VERSION = (kal_uint8*)"0000.0000.1A";
#endif

/**
 * Step 3:
 * Define necessary default values for each logical data item.
 */
 /*For hardware */
   
/*For Cache file*/

static kal_uint8 const NVRAM_SMS_DEFAULT [NVRAM_SMS_TEMPL_RECORD_SIZE] ;

/* NVRAM_SMS_CB_CHNL_DEFAULT = 0xff
static kal_uint8 const NVRAM_SMS_CB_CHNL_DEFAULT [NVRAM_SMS_CB_CHNL_RECORD_SIZE] =
{
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};


static kal_uint8 const NVRAM_SMS_CB_SMS_DEFAULT [NVRAM_SMS_CB_SMS_PAGE_SIZE]=
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
*/

static kal_uint8 const NVRAM_SMS_CB_SMS_INFO_DEFAULT [NVRAM_SMS_CB_SMS_INFO_SIZE]=
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
#if defined(CSD_NO_SLIM_NVRAM)	                   
static kal_uint8 const NVRAM_MSG_CLUB_DEFAULT [NVRAM_MSG_CLUB_RECORD_SIZE]=
{
	0x42, 0x4B, 0x4F, 0x4E, 0x4B, 0x00, 0x00, 0x00
};
#endif

#if defined(__MMI_SAVE_CURRENT_TIME__)
static kal_uint8 const NVRAM_EF_SAVE_CURRENT_TIME_DEFAULT[NVRAM_EF_SAVE_CURRENT_TIME_SIZE]=
{
	0, 0, 0, 1, 1, 11, 6
};
#endif
static kal_uint8 const NVRAM_MSG_CLUB_NUM_DEFAULT [NVRAM_MSG_CLUB_NUM_RECORD_SIZE]=
{
	0x33, 0x30, 0x32, 0x00
};

/*
static kal_uint8 const NVRAM_EMS_MY_PICTURE_NAME_DEFAULT [NVRAM_EMS_MY_PICTURE_NAME_RECORD_SIZE] = 
{
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static kal_uint8 const NVRAM_EMS_MY_ANIMATION_NAME_DEFAULT [NVRAM_EMS_MY_ANIMATION_NAME_RECORD_SIZE] = 
{
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static kal_uint8 const NVRAM_EMS_MY_MELODY_NAME_DEFAULT [NVRAM_EMS_MY_MELODY_NAME_RECORD_SIZE] = 
{
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

*/

/*
static kal_uint8 const NVRAM_SHORTCUTS_DEFAULT[NVRAM_SHORTCUTS_SIZE];
*/

static kal_uint8 const NVRAM_IP_NUMBER_DEFAULT[NVRAM_IP_NUMBER_SIZE] = { 
       '1', 0x00, '7', 0x00, '9', 0x00, '5', 0x00, '1', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
       	
       '1', 0x00, '7', 0x00, '9', 0x00, '1', 0x00, '1', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
       
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
static PHB_OPTIONAL_IDS_STRUCT const NVRAM_PHB_IDS_DEFAULT[NVRAM_PHB_FIELDS_TOTAL] = {
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
#if defined(__PHB_SIM255_NVRAM200__)
   ,
   {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
#elif defined(__PHB_SIM200_NVRAM250__)
	,
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
#elif defined(__PHB_SIM200_NVRAM300__)
	,
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
	{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
#endif
};
*/

#if (0)
#ifdef	__MMI_PHB_NO_OPTIONAL_FIELD__	/*Use very small space for no optional field */
/* under construction !*/
#else
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
#endif

static kal_uint8 const NVRAM_PHB_SETTINGS_DEFAULT[NVRAM_PHB_SETTINGS_SIZE] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

// MAXXXXX
//#include "todolist.h"
/*
static ToDoListNode const NVRAM_TODO_LIST_DEFAULT[MAX_TODO_LIST_TASK] =
{
	{
		{0,0,0,0,0,0,0},0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0
	},
	{
		{0,0,0,0,0,0,0},0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0
	},
	{
		{0,0,0,0,0,0,0},0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0
	},
	{
		{0,0,0,0,0,0,0},0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0
	},
	{
		{0,0,0,0,0,0,0},0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0
	},
	{
		{0,0,0,0,0,0,0},0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0
	},
	{
		{0,0,0,0,0,0,0},0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0
	},
	{
		{0,0,0,0,0,0,0},0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0
	},
	{
		{0,0,0,0,0,0,0},0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0
	},
	{
		{0,0,0,0,0,0,0},0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0,0
	}
};
*/
// MAXXXXX

static PHB_CALLER_GROUP_STRUCT const NVRAM_PHB_CALLER_GROUPS_DEFAULT[MAX_PB_CALLER_GROUPS] = {
	{{'F', 0, 'r', 0, 'i', 0, 'e', 0, 'n', 0, 'd', 0, 's', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, MIN_MIDI_ID, IMG_PHB_DEFAULT, 1, 0, 0, 0, MMI_RING,0},
	{{'F', 0, 'a', 0, 'm', 0, 'i', 0, 'l', 0, 'y', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  MIN_MIDI_ID, IMG_PHB_DEFAULT, 1, 0, 0, 0, MMI_RING,0},
	{{'V', 0, 'I', 0, 'P', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, MIN_MIDI_ID, IMG_PHB_DEFAULT, 1, 0, 0, 0, MMI_RING,0},
	{{'B', 0, 'u', 0, 's', 0, 'i', 0, 'n', 0, 'e', 0, 's', 0, 's', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, MIN_MIDI_ID, IMG_PHB_DEFAULT, 1, 0, 0, 0, MMI_RING,0},
	{{'O', 0, 't', 0, 'h', 0, 'e', 0, 'r', 0, 's', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, MIN_MIDI_ID, IMG_PHB_DEFAULT, 1, 0, 0, 0, MMI_RING,0}
};

/*
static ActivityDetails const NVRAM_CALORIE_DATA_DEFAULT[NUMBER_OF_WEEKDAYS] = {
	{{0,0,0,0,0,0,0,0,0,0}, {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, {0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0,0,0,0}, {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, {0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0,0,0,0}, {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, {0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0,0,0,0}, {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, {0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0,0,0,0}, {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, {0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0,0,0,0}, {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, {0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0,0,0,0}, {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, {0,0,0,0,0,0,0,0,0,0}}
};
*/


 PROFILE const NVRAM_PROFILES_DEFAULT [MAX_ELEMENTS] = 
{
/*
       {    ringVolumeLevel,   keypadVolumeLevel,  loudSpeakerVolumeLevel,  mtCallAlertTypeEnum,  
             light = {status, lcdBacklight, timer},  
             ringTypeEnum, 
             toneSetup = { ringTone, powerOffTone, powerOnTone, coverOpenTone, coverCloseTone, messageTone, keypadTone, alarmTone}, 
             answeringMode = {coverAnswer, anyKey, automatic},
             fontSizeEnum, 
             extraTone = {errorTone, connectTone, campOnTone, warningTone}
        }
*/

	{
		LEVEL7, LEVEL3, LEVEL4, MMI_RING, 
		{COLOR_PATTERN6, 1, {15}}, 
		REPEAT,
		{151,202, 201, COVER_OPEN_SILENT, COVER_CLOSE_SILENT, 221,KEYPAD_TONE,152,
		#ifdef __MMI_DUAL_PROFILE_SUPPORT__
		151,221
		#endif
		},
		{0, 0, 1}, 
		THEME_DEFAULT, 0,
		{0, 0, 0, 0}
	},
	{
		LEVEL7, LEVEL2, LEVEL4, MMI_VIBRATION_ONLY, 
		{COLOR_PATTERN6, 1, {15}}, 
		REPEAT,
		{151,POWEROFF_SILENT, POWERON_SILENT, COVER_OPEN_SILENT, COVER_CLOSE_SILENT, MESSAGE_SILENT,KEYPAD_SILENT,152,
		#ifdef __MMI_DUAL_PROFILE_SUPPORT__
		151,MESSAGE_SILENT
		#endif
		},
		{0, 0, 0}, 
		THEME_DEFAULT, 0,
		{0, 0, 0, 0}
	},
	{ 
		LEVEL7, LEVEL7, LEVEL4, MMI_VIBRATION_AND_RING, 
		{COLOR_PATTERN6, 1, {15}}, 
		REPEAT,
		{151,202, 201, COVER_OPEN_SILENT, COVER_CLOSE_SILENT, 221,KEYPAD_TONE,152,
		#ifdef __MMI_DUAL_PROFILE_SUPPORT__
		151,221
		#endif
		},
		{0, 0, 0}, 
		THEME_DEFAULT, 0,
		{1, 1, 1, 1}
	},
	{
		6/*LEVEL7*/, LEVEL2, LEVEL4, MMI_RING, 
		{COLOR_PATTERN6, 1, {15}}, 
		REPEAT,
		{151,202, 201, COVER_OPEN_SILENT, COVER_CLOSE_SILENT, 221,KEYPAD_TONE,152,
		#ifdef __MMI_DUAL_PROFILE_SUPPORT__
		151,221
		#endif
		},
		{0, 0, 0}, 
		THEME_DEFAULT, 0,
		{0, 0, 0, 0}
	},
	{
		6/*LEVEL7*/, LEVEL2, LEVEL4, MMI_VIBRATION_AND_RING, 
		{COLOR_PATTERN6, 1, {15}}, 
		REPEAT,
		{151,202, 201, COVER_OPEN_SILENT, COVER_CLOSE_SILENT, 221,KEYPAD_TONE,152,
		#ifdef __MMI_DUAL_PROFILE_SUPPORT__
		151,221
		#endif
		},
		/*Jinzh Modify Start For 6502 Ver: TBM780  on 2007-9-5 15:20 */
		{0, 0, 0}, 
		//{0, 0, 1}, 
		/*Jinzh Modify End  For 6502 Ver: TBM780  on 2007-9-5 15:20 */
		THEME_DEFAULT, 0,
		{0, 0, 0, 0}
	},
	{
		LEVEL3, LEVEL1, LEVEL4, MMI_SILENT, {COLOR_PATTERN1, 1, {15}}, REPEAT,{151,POWEROFF_SILENT, POWERON_SILENT, COVER_OPEN_SILENT, COVER_CLOSE_SILENT, MESSAGE_SILENT,KEYPAD_SILENT,152,
		#ifdef __MMI_DUAL_PROFILE_SUPPORT__
		151,MESSAGE_SILENT
		#endif
		},
		{1, 1, 1}, THEME_DEFAULT, 0, {0, 0, 0, 0}
	},
	{
		LEVEL3, LEVEL5, LEVEL4, MMI_RING, {COLOR_PATTERN1, 1, {15}}, REPEAT,{151,202, 201, 203, 204, 221,KEYPAD_TONE,152,
		#ifdef __MMI_DUAL_PROFILE_SUPPORT__
		151,221
		#endif
		},
		{1, 1, 1}, THEME_DEFAULT, 0, {1, 0, 1, 1}
	}
};

/*
static kal_uint8 const NVRAM_MAX_GROUP_NAME_DEFAULT[NVRAM_MAX_GROUP_NAME_SIZE] = 
{

    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
*/

/*
static kal_uint8 const NVRAM_DOWNLOAD_IMAGE_DEFAULT [NVRAM_DOWNLOAD_IMAGE_RECORD_SIZE * MAXIMUM_DOWNLOAD_IMAGES]=
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
*/

/*
static kal_uint8 const NVRAM_DOWNLOAD_TONE_DEFAULT [NVRAM_DOWNLOAD_TONE_RECORD_SIZE * MAXIMUM_DOWNLOAD_AUDIO]=
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
*/
/*
static kal_uint8 const NVRAM_DOWNLOAD_MELODY_DEFAULT [NVRAM_DOWNLOAD_MELODY_RECORD_SIZE * MAXIMUM_DOWNLOAD_AUDIO]=
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
*/

/*
static MYTIME const NVRAM_CALL_TIME_DEFAULT[NVRAM_CALL_TIME_RECORD_TOTAL];
*/

/*
static kal_uint8 const NVRAM_DEDICATED_SHORTCUT_DEFAULT[NVRAM_DEDICATED_SHORTCUT_SIZE*NVRAM_DEDICATED_SHORTCUT_TOTAL] =
{
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
*/

/*	
static kal_uint8 const NVRAM_COMPOSE_RINGTONE_DEFAULT [NVRAM_COMPOSE_RINGTONE_SIZE];
*/

static kal_uint8 const NVRAM_THEMES_VALUES_DEFAULT [NVRAM_THEMES_VALUES_SIZE] =
{
   0x05,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

static kal_uint8 const NVRAM_CHAT_VALUES_DEFAULT [NVRAM_CHAT_ROOM_INFO_RECORD_SIZE] =
{
    	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

static kal_uint8 const NVRAM_EF_ROOT_CA_DEFAULT[NVRAM_EF_ROOT_CA_SIZE] = {
#ifdef __PRODUCTION_RELEASE__
                  0x00
#else
                  0x30,0x00,0x12, /* 0x30 == '0', Root CA index file, size = 0x12 */
                  0x43,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x38,0x43,0x00,0x00,0x00,0x02,0x00,0x00,
                  0x01,0x99,
                  0x31,0x01,0x39, /* 0x31 == '1', 1st Root CA file, size = 0x0139 */
                  0x43,0x01,0x01,0x02,0x01,0x00,0x04,0x0d,0x54,0x65,0x6c,0x65,0x63,0x61,0x20,0x54,
                  0x65,0x73,0x74,0x20,0x32,0x3e,0xf8,0x15,0x46,0x40,0xd9,0x48,0xc6,0x01,0x00,0x04,
                  0x0d,0x54,0x65,0x6c,0x65,0x63,0x61,0x20,0x54,0x65,0x73,0x74,0x20,0x32,0x02,0x00,
                  0x00,0x03,0x01,0x00,0x01,0x00,0x80,0xc4,0x59,0x48,0x92,0x25,0xf7,0x40,0x14,0xc8,
                  0x02,0x80,0x54,0x69,0x02,0x1c,0x85,0x92,0xf2,0xc5,0xe0,0xdf,0xf2,0x72,0x0a,0xa9,
                  0xa3,0x5d,0x88,0x13,0x4b,0x27,0xbf,0xc8,0x21,0xf8,0xc6,0x6d,0x68,0x5d,0x7a,0xdc,
                  0x77,0x8e,0xbd,0x2a,0xfb,0x62,0xa0,0x33,0x0e,0x0a,0xa0,0xed,0x8f,0x48,0x64,0x52,
                  0x93,0xe7,0x2b,0xcb,0xb7,0xb5,0xa5,0x46,0xb5,0x3b,0x12,0x75,0x93,0xdc,0x94,0x6f,
                  0xd5,0x0b,0x6e,0x44,0x38,0xbf,0x9a,0xc4,0x07,0x18,0x56,0x4f,0x27,0x44,0x86,0xed,
                  0x7b,0x20,0x7b,0x0e,0x60,0xf0,0x40,0x80,0x1e,0xcc,0x11,0x04,0xdc,0xb4,0x50,0xf4,
                  0x40,0x0a,0x57,0x2a,0xc0,0x73,0xcb,0x6f,0x7b,0x34,0xf6,0x8a,0x66,0x4a,0xbd,0xee,
                  0x4b,0x39,0xbb,0x7e,0xf1,0x22,0xfd,0x00,0x80,0x55,0xb6,0xbe,0x89,0x25,0x59,0xf4,
                  0x9a,0x1b,0x7d,0x5d,0xc2,0x26,0x94,0x86,0xfb,0x18,0x3a,0xb4,0x27,0xa5,0x4a,0x9c,
                  0xbc,0x34,0x88,0x49,0x61,0x6c,0x85,0x9a,0x62,0x69,0xe8,0x3d,0x38,0x3b,0x72,0xcf,
                  0x29,0x9c,0xa6,0x73,0xca,0x2a,0xbc,0x76,0x93,0x34,0x91,0xa5,0xc8,0xdb,0x57,0x78,
                  0xea,0xbf,0xdd,0x9e,0x5d,0x94,0x90,0x6c,0x3a,0xce,0x49,0x3a,0xd6,0x7f,0xff,0x8c,
                  0xc6,0x5d,0x5b,0x21,0x64,0x04,0x01,0xac,0xb7,0xd1,0xab,0x55,0xd2,0xd5,0xe8,0x37,
                  0x9e,0x81,0x0e,0x69,0x55,0x94,0x79,0x12,0xe5,0x9b,0x5c,0xab,0x8a,0x27,0x0f,0x86,
                  0x6b,0x44,0x54,0x0d,0x62,0x12,0xce,0x26,0x30,0xd0,0x36,0xfa,0xd6,0x91,0x4b,0x7a,
                  0x67,0x55,0x46,0x3b,0x42,0xc9,0x37,0x2c,0x20,                  
                  0x32,0x01,0x9a, /* 0x32 == '2', 1st Root CA file, size = 0x019a */
                  0x43,0x02,0x30,0x82,0x01,0x94,0x30,0x82,0x01,0x3e,0xa0,0x03,0x02,0x01,0x02,0x02,
                  0x03,0x01,0x23,0x45,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,
                  0x05,0x05,0x00,0x30,0x47,0x31,0x0b,0x30,0x09,0x06,0x03,0x55,0x04,0x06,0x13,0x02,
                  0x53,0x45,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x0a,0x13,0x0b,0x54,0x65,0x6c,
                  0x65,0x63,0x61,0x52,0x6f,0x6f,0x74,0x33,0x31,0x0c,0x30,0x0a,0x06,0x03,0x55,0x04,
                  0x0b,0x13,0x03,0x54,0x4d,0x54,0x31,0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,
                  0x0b,0x54,0x65,0x6c,0x65,0x63,0x61,0x52,0x6f,0x6f,0x74,0x33,0x30,0x1e,0x17,0x0d,
                  0x30,0x32,0x31,0x31,0x31,0x39,0x31,0x31,0x33,0x35,0x35,0x39,0x5a,0x17,0x0d,0x31,
                  0x30,0x30,0x31,0x30,0x31,0x30,0x38,0x30,0x30,0x30,0x30,0x5a,0x30,0x47,0x31,0x0b,
                  0x30,0x09,0x06,0x03,0x55,0x04,0x06,0x13,0x02,0x53,0x45,0x31,0x14,0x30,0x12,0x06,
                  0x03,0x55,0x04,0x0a,0x13,0x0b,0x54,0x65,0x6c,0x65,0x63,0x61,0x52,0x6f,0x6f,0x74,
                  0x33,0x31,0x0c,0x30,0x0a,0x06,0x03,0x55,0x04,0x0b,0x13,0x03,0x54,0x4d,0x54,0x31,
                  0x14,0x30,0x12,0x06,0x03,0x55,0x04,0x03,0x13,0x0b,0x54,0x65,0x6c,0x65,0x63,0x61,
                  0x52,0x6f,0x6f,0x74,0x33,0x30,0x5c,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,
                  0x0d,0x01,0x01,0x01,0x05,0x00,0x03,0x4b,0x00,0x30,0x48,0x02,0x41,0x00,0xa6,0x02,
                  0xbb,0x55,0x41,0xe8,0x97,0x9c,0x3c,0x3f,0x72,0xba,0x5c,0xb4,0x22,0xcd,0x43,0xce,
                  0xff,0x3b,0x85,0xa1,0xf4,0x8f,0x16,0xbb,0x25,0xce,0x00,0x53,0x17,0xda,0xce,0x21,
                  0xc5,0xcc,0xd8,0x5f,0xee,0x09,0x3e,0x36,0xcb,0xf6,0x81,0xee,0xc4,0x60,0x95,0x98,
                  0x7e,0xb8,0x79,0x97,0x4a,0x93,0x86,0xe3,0x69,0xe5,0x6b,0xee,0xac,0xfd,0x02,0x03,
                  0x01,0x00,0x01,0xa3,0x13,0x30,0x11,0x30,0x0f,0x06,0x03,0x55,0x1d,0x13,0x01,0x01,
                  0xff,0x04,0x05,0x30,0x03,0x01,0x01,0xff,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,
                  0xf7,0x0d,0x01,0x01,0x05,0x05,0x00,0x03,0x41,0x00,0x0b,0x4e,0xb8,0xd8,0x42,0x6d,
                  0xc9,0xd9,0xcb,0x72,0x61,0x16,0xa7,0xb3,0x9b,0x2d,0xf0,0xf1,0xed,0x4f,0xb7,0x79,
                  0x92,0x16,0xe4,0x08,0xf5,0x2a,0xc8,0xd7,0x69,0xd3,0x70,0x93,0xfe,0xad,0xe4,0x12,
                  0xdc,0x37,0xde,0x0d,0xac,0xfe,0xbe,0xf0,0xe0,0xae,0xb0,0x95,0xae,0xa8,0xa4,0x29,
                  0x37,0x8b,0x82,0x7f,0x35,0x83,0x70,0x2c,0xa6,0xc4
#endif                  
                  };

static kal_uint8 const NVRAM_EF_CSD_PROFILE_DEFAULT[] = {
/* make CSD data account (1) begin ...*/

/* diaup-up: (+33660639639)*/
0x07,0x91,0x33,0x66,0x60,0x93,0x36,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,


/*ur : (9600)*/
0x02,
/*type : (ISDN)*/
0x01,
/*module : (NT)*/
0x00,
/*auth : (normal)*/
0x00,

/* username: (wap)*/
0x77,0x61,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/* password: (wapwap)*/
0x77,0x61,0x70,0x77,0x61,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


/* dns: (0.0.0.0)*/
0x00,0x00,0x00,0x00,

/* name: (Bouygues Telecom)*/
0x10,0x00,0x42,0x6f,0x75,0x79,0x67,0x75,0x65,0x73,0x20,0x54,0x65,0x6c,0x65,0x63,0x6f,0x6d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/* make profile (1) end ...*/
/* make CSD data account (2) begin ...*/

/* diaup-up: (+33674501100)*/
0x07,0x91,0x33,0x76,0x54,0x10,0x01,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,


/*ur : (9600)*/
0x02,
/*type : (ISDN)*/
0x02,
/*module : (NT)*/
0x00,
/*auth : (normal)*/
0x00,

 
0x6f,0x72,0x61,0x6e,0x67,0x65,0x2e,0x66,0x72,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

 
0x6f,0x72,0x61,0x6e,0x67,0x65,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


/* dns: (0.0.0.0)*/
0x00,0x00,0x00,0x00,

 
0x06,0x00,0x4f,0x72,0x61,0x6e,0x67,0x65,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/* make profile (2) end ...*/
/* make CSD data account (3) begin ...*/

/* diaup-up: (+33613000010)*/
0x07,0x91,0x33,0x16,0x03,0x00,0x10,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,


/*ur : (9600)*/
0x02,
/*type : (ISDN)*/
0x01,
/*module : (NT)*/
0x00,
/*auth : (normal)*/
0x00,

/* username: (0)*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/* password: (0)*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


/* dns: (0.0.0.0)*/
0x00,0x00,0x00,0x00,

/* name: (SFR)*/
0x03,0x00,0x53,0x46,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/* make profile (3) end ...*/
   
                  0x00, 0x81, 0x90, 0x13, 0x77, 0x77, 0x77, 0xff, 0xff, 0xff, 
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                   
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x41, 0x54, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x81, 0x71, 0x62, 0xf6, 0xff, 0xff, 0xff, 0xff, 0xff, 
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0x02, 0x02, 0x00, 0x00, 0x77, 0x61, 0x70, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x61,                   
                  0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x68, 0x69, 0x6e,
                  0x61, 0x20, 0x4D, 0x6f, 0x62, 0x69, 0x6c, 0x65, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x81, 0x90, 0x13, 0x77, 0x77, 0x77, 0xff, 0xff, 0xff, 
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                   
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x41, 0x54, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x81, 0x71, 0x62, 0xf6, 0xff, 0xff, 0xff, 0xff, 0xff, 
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0x02, 0x02, 0x00, 0x00, 0x77, 0x61, 0x70, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x61,                   
                  0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x68, 0x69, 0x6e,
                  0x61, 0x20, 0x4D, 0x6f, 0x62, 0x69, 0x6c, 0x65, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x81, 0x90, 0x13, 0x77, 0x77, 0x77, 0xff, 0xff, 0xff, 
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                   
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x41, 0x54, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x81, 0x71, 0x62, 0xf6, 0xff, 0xff, 0xff, 0xff, 0xff, 
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0x02, 0x02, 0x00, 0x00, 0x77, 0x61, 0x70, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x61,                   
                  0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x68, 0x69, 0x6e,
                  0x61, 0x20, 0x4D, 0x6f, 0x62, 0x69, 0x6c, 0x65, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x81, 0x71, 0x62, 0xf6, 0xff, 0xff, 0xff, 0xff, 0xff, 
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                  0xff, 0xff, 0x02, 0x02, 0x00, 0x00, 0x77, 0x61, 0x70, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x61,                   
                  0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x68, 0x69, 0x6e,
                  0x61, 0x20, 0x4D, 0x6f, 0x62, 0x69, 0x6c, 0x65, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                  
};

#if defined(CSD_NO_SLIM_NVRAM)	       
static kal_uint8 const NVRAM_EF_IMEI_IMEISV_DEFAULT[] = {
                  0x31, 0x75, 0x09, 0x42, 0x86, 0x11, 0x22, 0x00, /*The IMEI is 15 digit */
                  0x87,       /* Software version number */
                  0x01        /* Padding */
                  };
#endif

static kal_uint8 const NVRAM_EF_ABM_GPRS_PROFILE_DEFAULT[] ={
// CHINA MOBILE, cmwap
0x01, 0x06, 0x00, 0x00, 
0x43, 0x4D, 0X20, 0x57, 0x41, 0x50, 0x00, 0x00, 0X00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // user name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // password
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // dns
0x63, 0x6d, 0x77, 0x61, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, // apn
0x05, // apn_len 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, // qos 

// CHINA MOBILE, cmnet
0x02, 0x06, 0x00, 0x00, 
0x43, 0x4D, 0X20, 0x4E, 0x45, 0x54, 0x00, 0x00, 0X00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // user name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // password
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // dns
0x63, 0x6d, 0x6e, 0x65, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // apn 
0x05, // apn_len 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, // qos 

/* CHT, emome */
0x03, 0x08, 0x00, 0x00, 
0x43, 0x48, 0x54, 0x20, 0x47, 0x50, 0x52, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // user name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // password
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // dns
0x65, 0x6d, 0x6f, 0x6d, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, // apn
0x05, // apn_len 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, // qos 

/* TWN, mms */
0x04, 0x08, 0x00, 0x00,
0x54, 0x57, 0x4e, 0x20, 0x47, 0x50, 0x52, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // user name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // password
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // dns
0x6d, 0x6d, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, // apn
0x03, // apn_len 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, // qos 

// Vodafone_GPRS  portalnmms
0x05, 0x0d, 0x00, 0x00, 
0x56, 0x6f, 0X64, 0x61, 0x66, 0x6f, 0x6e, 0x65, 0X5f, 0x47, 0x50, 0x52, 0x53, 0x00, 0x00, 0x00,  // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // user name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // password
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // dns
0x70, 0x6f, 0x72, 0x74, 0x61, 0x6c, 0x6e, 0x6d, 0x6d, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, // apn
0x0a, // apn_len 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, // qos 

//Airtel Mobileoffice   airtelgprs.com	
0x06, 0x13, 0x00, 0x00, 
0x41, 0x69, 0X72, 0x74, 0x65, 0x6c, 0x20, 0x4d, 0X6f, 0x62, 0x69, 0x6c, 0x65, 0x6f, 0x66, 0x66, // Name
0x69, 0x63, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // user name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // dns
0x61, 0x69, 0x72, 0x74, 0x65, 0x6c, 0x67, 0x70, 0x72, 0x73, 0x2e, 0x63, 0x6f, 0x6d, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // apn 
0x0e, // apn_len 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, // qos 

// RELIANCE_GPRS  rcomwap
0x07, 0x0d, 0x00, 0x00, 
0x52, 0x45, 0x4c, 0x49, 0X41,0x4e,0x43, 0x45, 0x5f, 0x47, 0x50, 0x52, 0x53, 0x00, 0x00, 0x00,  // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // user name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // password
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // dns
0x72, 0x63, 0x6f, 0x6d, 0x77, 0x61, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, // apn
0x07, // apn_len 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, // qos 


/*add for MMS data account begin*/
// Vodafone_MMS  portalnmms
0x08, 0x0c, 0x00, 0x00, 
0x56, 0x6f, 0X64, 0x61, 0x66, 0x6f, 0x6e, 0x65, 0X5f, 0x4d, 0x4d, 0x53, 0x00, 0x00, 0x00, 0x00,  // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // user name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // password
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // dns
0x70, 0x6f, 0x72, 0x74, 0x61, 0x6c, 0x6e, 0x6d, 0x6d, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, // apn
0x0a, // apn_len 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, // qos 

// AIRTEL_MMS  airtelmms.com
0x09, 0x0a, 0x00, 0x00, 
0x41, 0x49, 0X52, 0x54, 0x45, 0x4c, 0x5f, 0x4d, 0X4d, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // user name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // dns
0x61, 0x69, 0x72, 0x74, 0x65, 0x6c, 0x6d, 0x6d, 0x73, 0x2e, 0x63, 0x6f, 0x6d, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // apn 
0x0d, // apn_len 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, // qos 

// RELIANCE_MMS  reliancemms
0x0a, 0x0c, 0x00, 0x00, 
0x52, 0x45, 0x4c, 0x49, 0X41 ,0x4e, 0x43, 0x45, 0x5f, 0x4d, 0x4d, 0x53, 0x00, 0x00, 0x00, 0x00,  // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // user name
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // password
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, // dns
0x72, 0x65, 0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65, 0x6d, 0x6d, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, // apn
0x0b, // apn_len 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, // qos 


/* make profile (10) end ...*/

};

// MAXXXXX
#if (0)
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
// MAXXXXX

#if (0)
#endif	
#if defined(CSD_NO_SLIM_NVRAM)	       
static kal_uint8 const NVRAM_EF_CLASSMARK_RACAP_DEFAULT[] = {
          
        
#if defined( __EGSM900__) && defined( __DCS1800__)
                  0x60,  /* MS supports E-GSM and DCS band */
                  0x14,  /* E-GSM power class 4, DCS power class 1 */  
#elif defined( __PGSM900__) && defined( __DCS1800__)
                  0x50,  /* MS supports P-GSM and DCS band */
                  0x14,  /* P-GSM power class 4, DCS power class 1 */  
#elif defined( __EGSM900__)
                  0x60,  /* MS supports E-GSM band only */
                  0x04,  /* E-GSM power class 4 */
#elif defined (__PGSM900__)
                  0x60,  /* MS supports P-GSM band only */
                  0x04,  /* P-GSM power class 4 */
#elif defined ( __DCS1800__ )
                  0x40,  /* MS supports DCS band only */
                  0x01,  /* DCS power class 1 */
#else 
                  0x00,  /* None of GSM900 and DCS1800 are supported */
                  0x00,       
#endif
                   
                  0x01,  /* HSCSD multislot cap not present, UCS2 =1 */   
                   

#if defined( __GSM450__) || defined( __GSM480__)
                  0x81,
#else
                  0x80,  /* Extended Measurement cap =1 */ 
#endif


#if !defined( __GSM450__) && !defined( __GSM480__) && !defined( __GSM850__)
                  0x00,  /* GSM 400 and GSM 850 are not suported */ 
#elif defined( __GSM450__) && !defined( __GSM480__) && !defined( __GSM850__)
                 0x90,  /* GSM 450 is supported with power class 4*/
#elif !defined( __GSM450__) && defined( __GSM480__) && !defined( __GSM850__)
                 0x50,  /* GSM 480 is supported with power class 4*/
#elif !defined( __GSM450__) && !defined( __GSM480__) && defined( __GSM850__)
                 0x02,  /* GSM 850 is supported*/
#elif defined( __GSM450__) && defined( __GSM480__) && !defined( __GSM850__)
                 0xd0,  /* GSM 450 and 480 are supported with power class 4*/ 
#elif defined( __GSM450__) && !defined( __GSM480__) && defined( __GSM850__)
                 0x92,  /* GSM 450 with power class 4 and GSM 850 are supported*/ 
#elif !defined( __GSM450__) && defined( __GSM480__) && defined( __GSM850__)
                 0x52,  /* GSM 450 with power class 4 and GSM 850 are supported*/ 
#else
                 0xd2,   /* GSM 450, 480 with power class 4 and 850 are supported*/
#endif


#if defined( __GSM850__) && defined( __PCS1900__)
                  0x91,  /* GSM 850 is power class 4 ,PCS 1900 is suportedwith power class 1*/
#elif  defined( __PCS1900__)
                  0x11,  /* PCS 1900 is suported , power class 1 */       
#elif  defined( __GSM850__)
                  0x80,  /* GSM 850 is power class 4 */ 
#else
                  0x00,  /* Both GSM 850 and DCS 1900 are not supported */
#endif

                  
          
                  0x30,  /* ES_IND =1, Revision level =01, A51 algo supported---*/
                  0x58,  /* PS cap =1, SS sreen indicator =01, SM cap =1 */
                  0x81,  /* CM3 =1, A52 algo supported */
                  
       /*---BYTE 10 to BYTE 12 : From MS RA capability and MS network capability --*/
                  0xb2,  /* Default GPRS multislot class 12 
                          * bit 8: multislot capability present, 
                          * bit 7 ~ 3: mutlislot capability */
                         /* bit 2: GPRS extended Dynamic Allocation cap =1 */ 
                         /* bit 1 : RL indicator =0 => Release 98 MS */ 
                  0x81,  /* GEA 1 supported, SM cap in GPRS =1 */
                  0x00   /* PFC feature mode =0 */
                  };
#endif
#if defined(CSD_NO_SLIM_NVRAM)	       
static kal_int32 const NVRAM_EF_ADC_DEFAULT[] = { 
#ifdef MT6205B
 #if ( defined(CHICAGO2003_BB)   )
 /* Typical value for  CHICAGO2003_BB  */
               5369,  5369, 5369, 5369, 5369,
               60428, 60428, 60428, 60428, 60428
 #elif ( defined(KLM2003_BB) )
 /*Typical value for KLM2003_BB */
               5369, 5369,  5369, 5369, 5369,
               (-25422),  (-25422),  (-25422), (-25422), (-25422)
 #else
               5524, 5524, 5524, 5524, 5524,
              23286, 23286, 23286, 23286, 23286,
 #endif
#else

 #if ( defined(MT6208_CEVB) || defined(MT6208_EVB) )
               5524, 5524, 5524, 5524, 5524,
              23286, 23286, 23286, 23286, 23286,
 #else
               5524, 5524, 5524, 5524, 5524, 5524, 5524,
              23286, 23286, 23286, 23286, 23286, 23286, 23286
#endif
#endif
};
#endif

static kal_uint8 const NVRAM_EF_BARCODE_NUM_DEFAULT[]={
                  'M', 'T', '0', '1', '2', '3', '4', '5', '6', '7',
                  '8', '9', '0', '1', '2', '3', '4', '5', '6', '7',
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00
};

static kal_uint8 const NVRAM_EF_CHIS_NUM_EFAULT[]={
				0xFF
};
#if defined(CSD_NO_SLIM_NVRAM)	           

/* 0-initialized value */ 
/*
static kal_uint8 const NVRAM_EF_SYS_CACHE_OCTET_DEFAULT[] = {
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/
static kal_uint8 const NVRAM_EF_UEM_MANUFACTURE_DATA_DEFAULT[] = {
                   'M',  'T',  'K',  '1', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                   'M',  'T',  'K',  '2', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                   'M',  'T',  'K',  '3', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                   'M',  'T',  'K',  '4', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                   'M',  'T',  'K',  '5', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

                   'M',  'T',  'K',  '6', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                   'M',  'T',  'K',  '7', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                  };
static kal_uint8 const NVRAM_EF_UEM_RMI_DATA_DEFAULT[] = {
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 'e'    , 'n'   , 0x01, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                  };

/*
static kal_uint8 const NVRAM_EF_SMSAL_MWIS_DEFAULT[] = {
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                  };
*/


static kal_uint8 const NVRAM_EF_SMSAL_SMS_DEFAULT[] = {
                  0xfe,                            /* status */
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                  };


static kal_uint8 const NVRAM_EF_TCM_PDP_PROFILE_DEFAULT[] = {
                  0x00, 0x00, 0x21, 0x01, 
                  0x00, 0x00, 0x00, 0x00, 
                  
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00,

                  0x08, 
                  
                  0x69, 0x6E, 0x74, 0x65, 0x72, 0x6E, 0x65, 0x74, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                  0x02, 0x02, 0x00, 0x00, 0x01, 
                  0xAC, 0x15, 0x78, 0x06, 
                  0x00, 
                  0x00, 0x00,

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00,

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

                  0x00
};
#endif

static kal_uint8 const NVRAM_EF_AUTOTEST_DEFAULT[NVRAM_EF_AUTOTEST_SIZE] = 
{
       /* [count, test0, test1, test2 ....... test229] */
		#ifdef __MMI_CAMERA__ 
		16,
		#else
		15,
		#endif
		3,	4,	5,	6,	7,	8,	9,	10,	11,	12,	13,	14,	17,	18,
		#ifdef __MMI_CAMERA__ 
		19,
		#endif
		20, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00,0x00,
#ifndef __MMI_CAMERA__ 
		0x00,                                                                                /* 230 */
#endif	
       /* Entry test time */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
	0x00,0x00,0x00,0x00                                                                                                  /* 250 */
};

/*
static kal_uint8 const NVRAM_EF_EXT_MELODY_INFO_DEFAULT[] = {

};
*/
#if defined(CSD_NO_SLIM_NVRAM)	                   
static kal_uint8 const NVRAM_EF_PORT_SETTING_DEFAULT[] = {
		    0x01, 0x00,   /* TST uses uart_port2(value is 1) */
		    0x00, 0x00,   /* APP uses uart_port1 */
                    0x00, 0xC2, 0x01, 0x00, /* tst default baud rate base = 115200 = 0x0001C200 */

#if defined (__GPRS_MODE__)                  
                    0x00, 0xC2, 0x01, 0x00, /* ps default baud rate base = 115200 = 0x0001C200 */
#else
                    0x00, 0xE1, 0x00, 0x00, /* ps default baud rate base =   57600 = 0x0000E100 */
#endif                  
                   0x00, 0x00, 0x00, 0x00   /* High SpeedSIM,  plus  3-byte padding */
};
#endif
static kal_uint8 const NVRAM_EF_PHB_COMPARE_DIGIT_DEFAULT[] = {
                  /*Guojian Modify Start For 9013 Ver: Jessamine  on 2008-7-3 18:32 */
                  0x09    //ox07
                  /*Guojian Modify End  For 9013 Ver: Jessamine  on 2008-7-3 18:32 */
};

//fengzb add begin
#ifdef JATAAYU_SUPPORT
nvram_wap_profile_content_struct const  NVRAM_EF_WAP_PROFILE_CONTENT_DEFAULT[NVRAM_WAP_PROFILE_MAX]=
{
#ifdef __OP01_WAP_DATACFG__
    {  /* wap profile 1 - Monwap(GPRS) */
        1, 0, 80, "http://wap.monternet.com", 10, 1, {10, 0, 0, 172}, "", ""
    },
    {  /* wap profile 2 - Internet(GPRS) */
        1, 0, 80, "http://wap.monternet.com", 11, 1, {10, 0, 0, 172}, "", ""
    },
    {  /* wap profile 3 - Monwap(Modem) */
        1, 0, 80, "http://wap.monternet.com", 0,  1, {10, 0, 0, 172}, "", ""
    },
    {  /* wap profile 4 - Internet(Modem) */
        1, 0, 80, "http://wap.monternet.com", 1,  1, {10, 0, 0, 172}, "", ""
    },
    {  /* wap profile 5 - Empty */
        1, 1,  0, "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* wap profile 6 - Empty */
        1, 1,  0, "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* wap profile 7 - Empty */
        1, 1,  0, "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* wap profile 8 - Empty */
        1, 1,  0, "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* wap profile 9 - Empty */
        1, 1,  0, "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* wap profile 10 - Empty */
        1, 1,  0, "", 10, 2, {0, 0, 0, 0}, "", ""
    }
#else /*__OP01_WAP_DATACFG__*/
   { /* wap profile 1 - CHINA MOBILE */
	  1,0,80,"http://wap.monternet.com",10,2,{10,0,0,172},"",""
   },   
   { /* wap profile 2 - Vodafone_GPRS */
      1,0,9401,"http://live.vodafone.in",14,1,{10,10,1,100},"",""
   },
   { /* wap profile 3 - airtel_GPRS */
      1,0,8799,"http://google.co.in",15,1,{100,1,201,172},"",""
   },     
   { /* wap profile 4 - reliance_GPRS */
      1,0,8080,"http://wap.rworld.co.in",16,1,{10,239,221,5},"",""
   },      
    {  /* wap profile 5 - Empty */
        1, 1,  0, "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* wap profile 6 - Empty */
        1, 1,  0, "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* wap profile 7 - Empty */
        1, 1,  0, "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* wap profile 8 - Empty */
        1, 1,  0, "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* wap profile 9 - Empty */
        1, 1,  0, "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* wap profile 10 - Empty */
        1, 1,  0, "", 10, 2, {0, 0, 0, 0}, "", ""
    }
#endif /*__OP01_WAP_DATACFG__*/
};


nvram_mms_profile_content_struct const NVRAM_EF_MMS_PROFILE_CONTENT_DEFAULT[NVRAM_MMS_PROFILE_MAX] =
{
#ifdef __OP01_MMS_DATACFG__
    {  /* wap profile 1 - MMS(GPRS) */
        1, 0, 80, "http://mmsc.monternet.com", 10, 1, {10, 0, 0, 172}, "", ""
    },
    {  /* wap profile 2 - MMS(GPRS) */
        1, 0, 80, "http://mmsc.monternet.com", 11, 1, {10, 0, 0, 172}, "", ""
    },
    {  /* wap profile 3 - Monwap(Modem) */
        1, 0, 80, "http://mmsc.monternet.com",  0, 1, {10, 0, 0, 172}, "", ""
    },
    {  /* wap profile 4 - Monwap(Modem) */
        1, 0, 80, "http://mmsc.monternet.com",  1, 1, {10, 0, 0, 172}, "", ""
    },
    {  /* mms profile 5 - Empty */
        1, 1,  0,  "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* mms profile 6 - Empty */
        1, 1,  0,  "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* mms profile 7 - Empty */
        1, 1,  0,  "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* mms profile 8 - Empty */
        1, 1,  0,  "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* mms profile 9 - Empty */
        1, 1,  0,  "", 10, 2, {0, 0, 0, 0}, "", ""
    },
    {  /* mms profile 10 - Empty */
        1, 1,  0,  "", 10, 2, {0, 0, 0, 0}, "", ""
    }
#else /*__OP01_MMS_DATACFG__*/
   { /* mms profile 1 - CHINA MOBILE */
      1,0,80,"http://mmsc.monternet.com",10,1,{10,0,0,172},"",""
   }, 
   { /* mms profile 2 - vodafone */
      1,0,9401,"http://mms1.hutchworld.co.in/mms/",17,1,{10,10,1,100},"",""
   },
   { /* mms profile 3 - airtel */
      1,0,8799,"http://100.1.201.171:10021/mmsc",18,1,{100,1,201,172},"",""
   },         
   { /* mms profile 4 - reliance */
      1,0,8080,"http://10.239.221.47/mms",19,1,{10,239,221,5},"",""
   },      
   { /* mms profile 5 - Empty */
      1,1,0,"http://",10,2,{0,0,0,0},"","" 
   },         
   { /* mms profile 6 - Empty */
      1,1,0,"http://",10,2,{0,0,0,0},"",""   
   },
   { /* mms profile 7 - Empty */
      1,1,0,"http://",10,2,{0,0,0,0},"","" 
   },
   { /* mms profile 8 - Empty */
      1,1,0,"http://",10,2,{0,0,0,0},"",""   
   },
   { /* mms profile 9 - Empty */
      1,1,0,"http://",10,2,{0,0,0,0},"","" 
   },         
   { /* mms profile 10 - Empty */
      1,1,0,"http://",10,2,{0,0,0,0},"",""   
   }
#endif /*__OP01_MMS_DATACFG__*/
 };

nvram_wap_profile_name_struct const NVRAM_EF_WAP_PROFILE_NAMES_DEFAULT[NVRAM_WAP_PROFILE_MAX]=
{
       {1,0x01,"\x43\x00\x68\x00\x69\x00\x6E\x00\x61\x00\x20\x00\x4D\x00\x6F\x00\x62\x00\x69\x00\x6C\x00\x65\x00"},
	{1,0x01,"\x56\x00\x6F\x00\x64\x00\x61\x00\x66\x00\x6F\x00\x6E\x00\x65\x00\x5F\x00\x47\x00\x50\x00\x52\x00\x53\x00"},
       {1,0x01,"\x61\x00\x69\x00\x72\x00\x74\x00\x65\x00\x6C\x00\x5F\x00\x47\x00\x50\x00\x52\x00\x53\x00"},
	{1,0x01,"\x72\x00\x65\x00\x6C\x00\x69\x00\x61\x00\x6E\x00\x63\x00\x65\x00\x5F\x00\x47\x00\x50\x00\x52\x00\x53\x00"},
	{1,0x01,"\x00\x00"},
	{1,0x01,"\x00\x00"},
	{1,0x01,"\x00\x00"},
	{1,0x01,"\x00\x00"},
	{1,0x01,"\x00\x00"},
	{1,0x01,"\x00\x00"}
};

nvram_mms_profile_name_struct const NVRAM_EF_MMS_PROFILE_NAMES_DEFAULT[NVRAM_MMS_PROFILE_MAX]=
{
       {1,0x01,"\x43\x00\x68\x00\x69\x00\x6E\x00\x61\x00\x20\x00\x4D\x00\x6F\x00\x62\x00\x69\x00\x6C\x00\x65\x00\x20\x00\x4D\x00\x4D\x00\x53\x00"},
	{1,0x01,"\x56\x00\x6F\x00\x64\x00\x61\x00\x66\x00\x6F\x00\x6E\x00\x65\x00\x5F\x00\x4D\x00\x4D\x00\x53\x00"},
       {1,0x01,"\x61\x00\x69\x00\x72\x00\x74\x00\x65\x00\x6C\x00\x5F\x00\x4D\x00\x4D\x00\x53\x00"},
	{1,0x01,"\x72\x00\x65\x00\x6C\x00\x69\x00\x61\x00\x6E\x00\x63\x00\x65\x00\x5F\x00\x4D\x00\x4D\x00\x53\x00"},
	{1,0x01,"\x00\x00"},
	{1,0x01,"\x00\x00"},
	{1,0x01,"\x00\x00"},
	{1,0x01,"\x00\x00"},
	{1,0x01,"\x00\x00"},
	{1,0x01,"\x00\x00"}
};

#endif/* JATAAYU_SUPPORT */
//fengzb add end

#ifdef __MMI_EBOOK_READER__ 
static kal_uint8 const NVRAM_EF_EBOOK_DEFAULT_VALUE [NVRAM_EF_EBOOK_SETTING_SIZE] =
{
   0x00,0x01,0x00,0x00,0x01,0x00,0x00,0x00,0x01
};
#endif /* __MMI_EBOOK_READER__ */

/*Don't remove this line: insert default value definition above*/
/**
 * Steps of defining logical data item:
 * 1> Define LID of new logical data item into `nvram_LID_cust_enum' in 
 *    nvram_user_defs.h
 * 2> Define two constants: SIZE and TOTAL, in nvram_user_defs.h
 * 3> Define default value of that new logical data item in nvram_user_config.c
 * 4> Define attributes of that new logical data item into 
 *    `logical_data_item_table_cust'
 * 5> Bypass.
 * 6> Change version number CODED_DATA_VERSION in nvram_user_config.c
 */
/**
 * The logical data item table.
 * Note:
 * Each logical data item must be:
 * 1> Size must be EVEN
 * 2> Size of default value must be equal to the logical data item's size.
 */
 /**
 * The rule for data item changes:
 * 1. To add a new data item, its name must be the next of the current last LID, for example, 
 *     the last LID in the CT reign now is CT1y, the new one must be CT1z. 
 * 2. Version must be increased if 
 *    (a) Size is changed, or 
 *    (b) Number of sections is changed, or 
 *    (c) Attribute is changed, or 
 *    (d) data structure is changed.
 */
  
 kal_uint8 const NVRAM_EF_ZERO_DEFAULT[NVRAM_EF_BT_INFO_SIZE] = { 0x00 };


 // added by zhoumn @2006/12/05 start>>>
 kal_uint8 const NVRAM_EF_ZERO_DEFAULT_FOR_MAXCALLCOST[NVRAM_CALL_COST_GET_MAX_RECORD_SIZE * NVRAM_CALL_COST_GET_MAX_RECORD_TOTAL] = {0};
 kal_uint8 const NVRAM_EF_ZERO_DEFAULT_FOR_LASTCALLCOST[NVRAM_CALL_COST_GET_LAST_RECORD_SIZE * NVRAM_CALL_COST_GET_LAST_RECORD_TOTAL] = {'0', 0, 0, 0, 0, 0, 0};
 kal_uint8 const NVRAM_EF_ZERO_DEFAULT_FOR_ALLCALLCOST[NVRAM_CALL_COST_GET_ALL_RECORD_SIZE * NVRAM_CALL_COST_GET_ALL_RECORD_TOTAL] = {'0', 0, 0, 0, 0, 0, 0, 0, 0};
 kal_uint8 const NVRAM_EF_ZERO_DEFAULT_FOR_CURRENCY[NVRAM_CALL_COST_GET_CURRENCY_RECORD_SIZE * NVRAM_CALL_COST_GET_CURRENCY_RECORD_TOTAL] = { '0' };
 kal_uint8 const NVRAM_EF_ZERO_DEFAULT_FOR_PPU[NVRAM_CALL_COST_GET_PPU_RECORD_SIZE * NVRAM_CALL_COST_GET_PPU_RECORD_TOTAL] = { '0' }; 
 // added by zhoumn @2006/12/05 end<<<

 kal_uint8 const NVRAM_EF_FF_DEFAULT[NVRAM_EF_FMSR_SETTING_SIZE] = { 0xFF };
  

 
#if defined(__MMI_ANALOGTV__)
#include "../../../../platform/edrv/atvd/include/atvd_m.h"

static kal_uint8 const NVRAM_EF_ATV_PLAYER_SETTINGS_DEFAULT[NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE] = {
  0x0,//0 first use ATV flag
  0x5,// 1 ATV volume 0~7
  0x0,// 2 total channel
  0x0,// 3 last channel
  0x0,// 4 storage
  0x0,// 5 mute
  0x0,// 6 
  #if defined(__MMI_TV169_CK_ML_C__) 
  TV_AREA_BRAZIL,// 7 
  #else
  TV_AREA_CHINA,
  #endif
  0x0,// 8 
  0x0,// 9 
  0x0,// 10 
  0x0,// 11
  0x0,// 12 
  0x0,// 13 
  0x0,// 14
  0x0,//15 
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
#if 0 
  0xFF,0xFF,//31
#endif
};
#endif

const ltable_entry_struct logical_data_item_table_cust[NVRAM_EF_LAST_LID_CUST-NVRAM_LID_CUST_BEGIN] = 
{
    {/*0*/
        NVRAM_EF_SMS_LID,                               /* LID */
        NVRAM_SMS_TEMPL_RECORD_SIZE,                       /* Size */
        NVRAM_SMS_RECORD_TOTAL,                      /* Number of records */
        NVRAM_SMS_DEFAULT,                    /* Default value */
        NVRAM_ATTR_NONE,                        /* Attributes */
        NVRAM_CATEGORY_USER,                        /* Category */
        NVRAM_DRV_FDM,                              /* Device */
        "CT0a", 
        VER(NVRAM_EF_SMS_LID), 
        "SMS\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,             /* Reserved. record ID is calculated by NVRAM */
    },

    {/*1*/
        NVRAM_EF_SMS_CB_CHNL_LID,                               /* LID */
        NVRAM_SMS_CB_CHNL_RECORD_SIZE,                       /* Size */
        NVRAM_SMS_CB_CHNL_RECORD_TOTAL,                      /* Number of records */
        NVRAM_EF_FF_DEFAULT,                    /* Default value */
        NVRAM_ATTR_NONE,                        /* Attributes */
        (NVRAM_CATEGORY_USER 
        | NVRAM_CATEGORY_FACTORY
        ),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */
        "CT0b", 
        VER(NVRAM_EF_SMS_CB_CHNL_LID), 
        "SMS CB Channel\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,             /* Reserved. record ID is calculated by NVRAM */
    },
    
    {/*2*/
        NVRAM_EF_SMS_CB_SMS_LID,                               /* LID */
        NVRAM_SMS_CB_SMS_PAGE_SIZE,                       /* Size */
        NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL,                      /* Number of records */
        NVRAM_EF_ZERO_DEFAULT,                    /* Default value */
        NVRAM_ATTR_NONE,                        /* Attributes */
        (NVRAM_CATEGORY_USER | NVRAM_CATEGORY_FACTORY),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */
        "CT0c", 
        VER(NVRAM_EF_SMS_CB_SMS_LID), 
        "SMS CB SMS\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,             /* Reserved. record ID is calculated by NVRAM */
    },

    {/*3*/
        NVRAM_EF_SMS_CB_SMS_INFO_LID,                               /* LID */
        NVRAM_SMS_CB_SMS_INFO_SIZE,                       /* Size */
        NVRAM_SMS_CB_SMS_INFO_TOTAL,                      /* Number of records */
        NVRAM_SMS_CB_SMS_INFO_DEFAULT,                    /* Default value */
        NVRAM_ATTR_NONE,                        /* Attributes */
        (NVRAM_CATEGORY_USER | NVRAM_CATEGORY_FACTORY),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */
        "CT11", 
        VER(NVRAM_EF_SMS_CB_SMS_INFO_LID), 
        "SMS CB SMS INFO\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,             /* Reserved. record ID is calculated by NVRAM */
    },
    
    {/*4*/
        NVRAM_EF_PHB_IDS_LID,						/* LID */                                       
        NVRAM_PHB_IDS_SIZE,					/* Size */                                      
        NVRAM_PHB_IDS_TOTAL,					/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,		/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        (NVRAM_CATEGORY_USER),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0g", 
        VER(NVRAM_EF_PHB_IDS_LID), 
        "Phonebook IDs\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },    
    {/*5*/
        NVRAM_EF_PHB_SETTINGS_LID,						/* LID */                                       
        NVRAM_PHB_SETTINGS_SIZE,					/* Size */                                      
        NVRAM_PHB_SETTINGS_TOTAL,					/* Number of records */                         
        NVRAM_PHB_SETTINGS_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        (NVRAM_CATEGORY_USER),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0i", 
        VER(NVRAM_EF_PHB_SETTINGS_LID), 
        "Phonebook Settings\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    {/*6*/
        NVRAM_EF_PHB_CALLER_GROUPS_LID,				/* LID */                                       
        NVRAM_PHB_CALLER_GROUPS_SIZE,				/* Size */                                      
        NVRAM_PHB_CALLER_GROUPS_TOTAL,				/* Number of records */                         
        (kal_uint8*)NVRAM_PHB_CALLER_GROUPS_DEFAULT,/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        (NVRAM_CATEGORY_USER | NVRAM_CATEGORY_FACTORY),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0j", 
        VER(NVRAM_EF_PHB_CALLER_GROUPS_LID), 
        "PHB Caller Group\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },

    {/*7*/
        NVRAM_EF_PROFILES_LID,						/* LID */                                       
        NVRAM_PROFILES_RECORD_SIZE,				/* Size */                                      
        NVRAM_PROFILES_RECORD_TOTAL,			/* Number of records */                         
        (kal_uint8 *)NVRAM_PROFILES_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        (NVRAM_CATEGORY_USER | NVRAM_CATEGORY_FACTORY),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0o", 
        VER(NVRAM_EF_PROFILES_LID), 
        "Profiles\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },      
    
    {/*8*/
        NVRAM_EF_GROUPLIST_LID,						/* LID */                                       
        NVRAM_MAX_GROUP_NAME_SIZE,				/* Size */                                      
        NVRAM_MAX_GROUP_NAME_TOTAL,			/* Number of records */                         
        NVRAM_EF_FF_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0s", 
        VER(NVRAM_EF_GROUPLIST_LID), 
        "Group Lists\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },    

    {/*9*/
        NVRAM_EF_CALL_TIME_LID,						/* LID */
        NVRAM_CALL_TIME_RECORD_SIZE ,				/* Size */                                      
        NVRAM_CALL_TIME_RECORD_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT, 	/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0w", 
        VER(NVRAM_EF_CALL_TIME_LID),
        "Call Time\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },

    {/*10*/
        NVRAM_EF_CALL_COST_GET_MAX_LID,						/* LID */
        NVRAM_CALL_COST_GET_MAX_RECORD_SIZE,				/* Size */                                      
        NVRAM_CALL_COST_GET_MAX_RECORD_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT_FOR_MAXCALLCOST, 	/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0w", 
        VER(NVRAM_EF_CALL_COST_GET_MAX_LID), 
        "Max Call Cost\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    
    {/*11*/
        NVRAM_EF_CALL_COST_GET_ALL_COST_LID,						/* LID */
        NVRAM_CALL_COST_GET_ALL_RECORD_SIZE,				/* Size */                                      
        NVRAM_CALL_COST_GET_ALL_RECORD_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT_FOR_ALLCALLCOST, 	/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0w", 
        VER(NVRAM_EF_CALL_COST_GET_ALL_COST_LID), 
        "All Call Cost\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    
    {/*12*/
        NVRAM_EF_CALL_COST_GET_LAST_COST_LID,						/* LID */
        NVRAM_CALL_COST_GET_LAST_RECORD_SIZE,				/* Size */                                      
        NVRAM_CALL_COST_GET_LAST_RECORD_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT_FOR_LASTCALLCOST, 	/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0w", 
        VER(NVRAM_EF_CALL_COST_GET_LAST_COST_LID), 
        "Last Call Cost\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    
    {/*13*/
        NVRAM_EF_CALL_COST_GET_CURRENCY_LID,						/* LID */
        NVRAM_CALL_COST_GET_CURRENCY_RECORD_SIZE,				/* Size */                                      
        NVRAM_CALL_COST_GET_CURRENCY_RECORD_TOTAL,			/* Number of records */                         
        //价格...
        NVRAM_EF_ZERO_DEFAULT_FOR_CURRENCY, 	/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0w", 
        VER(NVRAM_EF_CALL_COST_GET_CURRENCY_LID), 
        "Currency Call Cost\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    
    {/*14*/
        NVRAM_EF_CALL_COST_GET_PPU_LID,						/* LID */
        NVRAM_CALL_COST_GET_PPU_RECORD_SIZE,				/* Size */                                      
        NVRAM_CALL_COST_GET_PPU_RECORD_TOTAL,			/* Number of records */                         
        //费率...
        NVRAM_EF_ZERO_DEFAULT_FOR_PPU, 	/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0w", 
        VER(NVRAM_EF_CALL_COST_GET_PPU_LID), 
        "PPU Call Cost\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    
    {/*15*/
        NVRAM_EF_BT_INFO_LID,	       /** LID **/                                            
        NVRAM_EF_BT_INFO_SIZE,           /** Size **/                                           
        NVRAM_EF_BT_INFO_TOTAL,          /** Total **/                                          
        NVRAM_EF_ZERO_DEFAULT,        /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),    /** Attributes **/      
        NVRAM_CATEGORY_USER,                /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT0z", 
        VER(NVRAM_EF_BT_INFO_LID), 
        "Bluetooth info\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    
    {/*16*/
        NVRAM_EF_ALM_QUEUE_LID,						/* LID */
        NVRAM_ALM_QUEUE_SIZE ,				/* Size */                                      
        NVRAM_ALM_QUEUE_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,       /*dongwg modify 20070719*/
        /*NVRAM_EF_FF_DEFAULT,*/				/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_SYSTEM,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT17", 
        VER(NVRAM_EF_ALM_QUEUE_LID), 
        "Alarm queue\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },    

    {/*17*/
        NVRAM_EF_SYS_CACHE_OCTET_LID,			  /** LID **/                                            
        NVRAM_EF_SYS_CACHE_OCTET_SIZE,           /** Size **/                                           
        NVRAM_EF_SYS_CACHE_OCTET_TOTAL,          /** Total **/                                          
        NVRAM_EF_ZERO_DEFAULT,                                 /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),	/** Attributes **/      
        NVRAM_CATEGORY_USER,                /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1h", 
        VER(NVRAM_EF_SYS_CACHE_OCTET_LID), 
        //123456789012345
        "SYS_CACHE_OCTET\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },      

    {/*18*/
        NVRAM_EF_CAMERA_PARA_LID, 		  /** LID **/                                            
        NVRAM_EF_CAMERA_PARA_SIZE,        /** Size **/                                           
        NVRAM_EF_CAMERA_PARA_TOTAL,       /** Total **/                                          
        NVRAM_EF_ZERO_DEFAULT,     /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1t", 
        VER(NVRAM_EF_CAMERA_PARA_LID), 
        "Camera Para\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {/*19*/
        NVRAM_EF_WPSS_FILENAME_LID, 		  /** LID **/                                            
        NVRAM_EF_WPSS_FILENAME_SIZE,        /** Size **/                                           
        NVRAM_EF_WPSS_FILENAME_TOTAL,       /** Total **/                                          
        NVRAM_EF_ZERO_DEFAULT,     /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        (NVRAM_CATEGORY_USER 
        | NVRAM_CATEGORY_FACTORY
        ),                        /* Category */      
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1u", 
        VER(NVRAM_EF_WPSS_FILENAME_LID), 
        "WpSs filename\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {/*20*/
        NVRAM_EF_AUTOTEST_LID,						/* LID */
        NVRAM_EF_AUTOTEST_SIZE ,				/* Size */                                      
        NVRAM_EF_AUTOTEST_TOTAL,			/* Number of records */                         
        NVRAM_EF_AUTOTEST_DEFAULT,				/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT1v", 
        VER(NVRAM_EF_AUTOTEST_LID), 
        "FM AutoTest\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },        
    {	/*21*/
        NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_LID,		/* LID */
        NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_SIZE,		/* Size */                                      
        NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_TOTAL,	/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,							/* Default value */                             
        NVRAM_ATTR_NONE,									/* Attributes */                                
        NVRAM_CATEGORY_USER,                		/* Category */                                  
        NVRAM_DRV_FDM,                      		/* Device */                                    
        "CT22",
        VER(NVRAM_EF_FMGR_FILEPATH_POOL_HEAD_LID),
        "Fmgr Path Pool Head\0",
        NVRAM_AVERAGE_LID,
        (kal_uint8)NVRAM_INVALID_VALUE,        	/* Reserved. record ID is calculated by NVRAM */
    },
    {	/*22*/
        NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_LID,		/* LID */
        NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_SIZE,		/* Size */                                      
        NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_TOTAL,	/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,							/* Default value */                             
        NVRAM_ATTR_NONE,									/* Attributes */                                
        NVRAM_CATEGORY_USER,                		/* Category */                                  
        NVRAM_DRV_FDM,                      		/* Device */                                    
        "CT23",
        VER(NVRAM_EF_FMGR_FILEPATH_POOL_TAIL_LID),
        "Fmgr Path Pool Tail\0",
        NVRAM_AVERAGE_LID,
        (kal_uint8)NVRAM_INVALID_VALUE,        	/* Reserved. record ID is calculated by NVRAM */
    },
    {/*23*/
        NVRAM_EF_ALM_SPOF_DATA_LID,						/* LID */
        NVRAM_ALM_SPOF_DATA_SIZE ,				/* Size */                                      
        NVRAM_ALM_SPOF_DATA_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,	/*dongwg modify 2007_07_06*/
        /*NVRAM_EF_ZERO_DEFAULT,*/				/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        ( NVRAM_CATEGORY_SYSTEM| NVRAM_CATEGORY_FACTORY),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT18", 
        VER(NVRAM_EF_ALM_SPOF_DATA_LID), 
        "Alarm SPOF data\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    {/*24*/
        NVRAM_EF_PHB_COMPARE_DIGIT_LID, 					 /** LID **/                                            
        NVRAM_EF_PHB_COMPARE_DIGIT_SIZE,              /** Size **/                                           
        NVRAM_EF_PHB_COMPARE_DIGIT_TOTAL,             /** Total **/                                          
        NVRAM_EF_PHB_COMPARE_DIGIT_DEFAULT,           /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT2h", 
        VER(NVRAM_EF_PHB_COMPARE_DIGIT_LID), 
        "PHB Compare Digit\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },    
    {/*25*/
        NVRAM_EF_CAMERA_SETTING_LID, 		  /** LID **/                                            
        NVRAM_EF_CAMERA_SETTING_SIZE,       /** Size **/                                           
        NVRAM_EF_CAMERA_SETTING_TOTAL,      /** Total **/                                          
        NVRAM_EF_FF_DEFAULT,            			/** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),					/* Attributes */
        NVRAM_CATEGORY_USER,           	/* Category */
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT12", 
        VER(NVRAM_EF_CAMERA_SETTING_LID), 
        "CAMERA SETTING\0",					/* Description*/
        NVRAM_AVERAGE_LID,					/* Class */
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {/*26*/
        NVRAM_EF_VIDEO_SETTING_LID, 		  /** LID **/                                            
        NVRAM_EF_VIDEO_SETTING_SIZE,       /** Size **/                                           
        NVRAM_EF_VIDEO_SETTING_TOTAL,      /** Total **/                                          
        NVRAM_EF_FF_DEFAULT,            			/** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),					/* Attributes */
        NVRAM_CATEGORY_USER,           	/* Category */
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT12", 
        VER(NVRAM_EF_VIDEO_SETTING_LID), 
        "VIDEO  SETTING\0",					/* Description*/
        NVRAM_AVERAGE_LID,					/* Class */
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {/*27*/
        NVRAM_EF_CUST_ACOUSTIC_DATA_LID, 		  /** LID **/                                            
        NVRAM_EF_CUST_ACOUSTIC_DATA_SIZE,      /** Size **/                                           
        NVRAM_EF_CUST_ACOUSTIC_DATA_TOTAL,     /** Total **/                                          
        //NVRAM_EF_CUST_ACOUSTIC_DATA_DEFAULT,   /** Default value **/  
        NVRAM_EF_ZERO_DEFAULT,
        //lilei modify 20060615
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT01", 
        VER(NVRAM_EF_CUST_ACOUSTIC_DATA_LID), 
        "Accoustic\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {/*28*/
        NVRAM_EF_AUDIO_PARAM_LID, 		  /** LID **/                                            
        NVRAM_EF_AUDIO_PARAM_SIZE,        /** Size **/                                           
        NVRAM_EF_AUDIO_PARAM_TOTAL,       /** Total **/                                          
        NVRAM_EF_FF_DEFAULT,             /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_MULTIPLE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT02", 
        VER(NVRAM_EF_AUDIO_PARAM_LID), 
        "Audio Parameter\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },         
    {/*29*/                                                                   
        NVRAM_EF_BARCODE_NUM_LID,              /** LID **/                                                                                         
        NVRAM_EF_BARCODE_NUM_SIZE,             /** Size **/                                                                     
        NVRAM_EF_BARCODE_NUM_TOTAL,            /** Total **/                                                                    
        NVRAM_EF_BARCODE_NUM_DEFAULT,          /** Default value **/                                                            
        (kal_uint8)(NVRAM_ATTR_NONE | NVRAM_ATTR_CONFIDENTIAL), /** Attributes **/
        NVRAM_CATEGORY_USER,                /** Category **/                                                                 
        NVRAM_DRV_FDM,                        /** Device **/                                                                   
        "CT1e", 
        VER(NVRAM_EF_BARCODE_NUM_LID), 
        "BARCODE\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }   
#ifdef __MMI_NOTEPAD__
    ,{
        NVRAM_EF_NOTEPAD_LIST_LID,						/* LID */                                       
        NVRAM_NOTEPAD_LIST_SIZE,				/* Size */                                      
        NVRAM_NOTEPAD_LIST_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT2y", 
        VER(NVRAM_EF_NOTEPAD_LIST_LID), 
        "Notepad List\0",
        NVRAM_IMPORTANT_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    {
        NVRAM_EF_NOTEPAD_RECORD_LID,						/* LID */                                       
        NVRAM_NOTEPAD_RECORD_SIZE,				/* Size */                                      
        NVRAM_NOTEPAD_RECORD_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT2z", 
        VER(NVRAM_EF_NOTEPAD_RECORD_LID), 
        "Notepad Record\0",
        NVRAM_IMPORTANT_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    }
#endif
#ifdef __GPRS_MODE__
    ,{
        NVRAM_EF_CSD_PROFILE_LID, 		  /** LID **/                                            
        NVRAM_EF_CSD_PROFILE_SIZE,        /** Size **/                                           
        NVRAM_EF_CSD_PROFILE_TOTAL,       /** Total **/                                          
        NVRAM_EF_CSD_PROFILE_DEFAULT,             /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT10", 
        VER(NVRAM_EF_CSD_PROFILE_LID), 
        //123456789012345
        "CSD Profile\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {
        NVRAM_EF_ABM_GPRS_PROFILE_LID, 		  /** LID **/                                            
        NVRAM_EF_ABM_GPRS_PROFILE_SIZE,        /** Size **/                                           
        NVRAM_EF_ABM_GPRS_PROFILE_TOTAL,       /** Total **/                                          
        NVRAM_EF_ABM_GPRS_PROFILE_DEFAULT,             /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_MULTI_DEFAULT,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT14", 
        VER(NVRAM_EF_ABM_GPRS_PROFILE_LID), 
        "ABM GPRS PROFILE\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }
#endif   
#ifdef __IP_NUMBER__
    ,{
        NVRAM_EF_IP_NUMBER_LID, 		  /** LID **/                                            
        NVRAM_IP_NUMBER_SIZE,        /** Size **/                                           
        NVRAM_IP_NUMBER_TOTAL,       /** Total **/                                          
        NVRAM_IP_NUMBER_DEFAULT,             /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT13", 
        VER(NVRAM_EF_IP_NUMBER_LID), 
        "IP Number\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }
#endif    
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
    ,{
        NVRAM_EF_EXT_MELODY_INFO_LID, 		  /** LID **/                                            
        NVRAM_EF_EXT_MELODY_INFO_SIZE,        /** Size **/                                           
        NVRAM_EF_EXT_MELODY_INFO_TOTAL,       /** Total **/                                          
        NVRAM_EF_FF_DEFAULT,             /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT15", 
        VER(NVRAM_EF_EXT_MELODY_INFO_LID), 
        "External Melody\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }
#endif    
#if defined(__MMI_VCARD__)
    ,{
        NVRAM_EF_PHB_VCARD_LID,			  /** LID **/                                            
        NVRAM_PHB_VCARD_LID_SIZE,           /** Size **/                                           
        NVRAM_PHB_VCARD_LID_TOTAL,          /** Total **/                                          
        NVRAM_EF_ZERO_DEFAULT,                                 /** Default value **/                                  
        (kal_uint8)NVRAM_ATTR_NONE,	/** Attributes **/      
        NVRAM_CATEGORY_USER,                /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1f", 
        VER(NVRAM_EF_PHB_VCARD_LID), 
        "PHB vcard\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }
#endif    
#ifdef __MMI_STOPWATCH__
    ,{
        NVRAM_EF_STOPWATCH_LID,						/* LID */                                       
        NVRAM_EF_STOPWATCH_RECORD_SIZE,				/* Size */                                      
        NVRAM_EF_STOPWATCH_RECORD_TOTAL,			/* Number of records */                         
        NULL,
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT1x", 
        VER(NVRAM_EF_STOPWATCH_LID), 
        "Stopwatch\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    }
#endif    
#ifdef __MMI_MESSAGES_CHAT__
    ,{
        NVRAM_EF_CHAT_ROOM_INFO_LID,
        NVRAM_CHAT_ROOM_INFO_RECORD_SIZE,
        NVRAM_CHAT_ROOM_INFO_RECORD_TOTAL,
        NVRAM_CHAT_VALUES_DEFAULT,
        NVRAM_ATTR_NONE,
        NVRAM_CATEGORY_USER,
        NVRAM_DRV_FDM,
        "CT1z", 
        VER(NVRAM_EF_CHAT_ROOM_INFO_LID), 
        "ChatInfo\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_EF_ZERO_DEFAULT_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    }
#endif
#ifdef __MMI_MESSAGES_CLUB__
    ,{	
        NVRAM_EF_MSG_CLUB_NUM_LID,			/* LID */
        NVRAM_MSG_CLUB_NUM_RECORD_SIZE,				/* Size */                                      
        NVRAM_MSG_CLUB_NUM_RECORD_TOTAL,			/* Number of records */                         
        NVRAM_MSG_CLUB_NUM_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT21", 
        VER(NVRAM_EF_MSG_CLUB_NUM_LID), 		
        "Msg Club Number\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    }
#endif    
#ifdef __MMI_CM_BLACK_LIST__
    ,{
        NVRAM_EF_BLACK_LIST_LID,				/* LID */
        NVRAM_EF_BLACK_LIST_SIZE,				/* Size */
        NVRAM_EF_BLACK_LIST_TOTAL,			/* Number of records */
        NVRAM_EF_ZERO_DEFAULT,	/* Default value */
        (kal_uint8)(NVRAM_ATTR_NONE),					/* Attributes */
        NVRAM_CATEGORY_USER,           	/* Category */
        NVRAM_DRV_FDM,                      			/* Device */
        "CT24",						/* Prefix */
        VER(NVRAM_EF_BLACK_LIST_LID),			/* Verno */
        "Black List\0",					/* Description*/
        NVRAM_AVERAGE_LID,					/* Class */
        (kal_uint8)NVRAM_INVALID_VALUE,        	/* Reserved. record ID is calculated by NVRAM */
    }
#endif
#ifdef __MMI_SOUND_EFFECT__
    ,{
        NVRAM_EF_AUDIO_EQUALIZER_LID,				/* LID */
        NVRAM_EF_AUDIO_EQUALIZER_SIZE,				/* Size */
        NVRAM_EF_AUDIO_EQUALIZER_TOTAL,			/* Number of records */
        //(const kal_uint8 *) NVRAM_EF_AUDIO_EQUALIZER_DEFAULT,	/* Default value */
        NVRAM_EF_ZERO_DEFAULT,
        //lilei modify 20060615
        (kal_uint8)(NVRAM_ATTR_NONE),					/* Attributes */
        NVRAM_CATEGORY_USER,           	/* Category */
        NVRAM_DRV_FDM,                      			/* Device */
        "CT2f",						/* Prefix */
        VER(NVRAM_EF_AUDIO_EQUALIZER_LID),			/* Verno */
        "Audio EQ\0",					/* Description*/
        NVRAM_AVERAGE_LID,					/* Class */
        (kal_uint8)NVRAM_INVALID_VALUE,        	/* Reserved. record ID is calculated by NVRAM */
    }
#endif    
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
    ,{
        NVRAM_EF_PHB_BIRTHDAY_LID, 					 /** LID **/                                            
        NVRAM_EF_PHB_BIRTHDAY_SIZE,              /** Size **/                                           
        NVRAM_EF_PHB_BIRTHDAY_TOTAL,             /** Total **/                                          
        NVRAM_EF_ZERO_DEFAULT,           /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT2m", 
        VER(NVRAM_EF_PHB_BIRTHDAY_LID), 
        "PHB Birthday\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }
#endif
#ifdef __MMI_FM_RADIO__
    ,{	
        NVRAM_EF_FM_RADIO_LID,			/* LID */
        NVRAM_EF_FM_RADIO_SIZE,				/* Size */                                      
        NVRAM_EF_FM_RADIO_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT24", 
        VER(NVRAM_EF_FM_RADIO_LID), 		
        "FM Radio\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    }
#endif
#if defined(__MMI_FM_RADIO_SCHEDULE_REC__)
    ,{
        NVRAM_EF_FMSR_SETTING_LID,					   /** LID **/											  
        NVRAM_EF_FMSR_SETTING_SIZE, 			 /** Size **/											
        NVRAM_EF_FMSR_SETTING_TOTAL,			 /** Total **/											
        NVRAM_EF_FF_DEFAULT,		   /** Default value **/								  
        (kal_uint8)(NVRAM_ATTR_NONE),		  /** Attributes **/									 
        NVRAM_CATEGORY_USER|NVRAM_CATEGORY_SHADOW_MS,
        NVRAM_DRV_FDM,						  /** Device **/										 
        "CT2n", 
        VER(NVRAM_EF_FMSR_SETTING_LID), 
        "Schedule FM Radio Record\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE		  /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }
#endif
#ifdef __MMI_PHOTOEDITOR__
    ,{
        NVRAM_EF_PHOTO_EDITOR_SETTING_LID,					   /** LID **/											  
        NVRAM_EF_PHOTO_EDITOR_SETTING_SIZE, 			 /** Size **/											
        NVRAM_EF_PHOTO_EDITOR_SETTING_TOTAL,			 /** Total **/											
        NVRAM_EF_FF_DEFAULT,		   /** Default value **/								  
        (kal_uint8)(NVRAM_ATTR_NONE),		  /** Attributes **/									 
        NVRAM_CATEGORY_USER|NVRAM_CATEGORY_SHADOW_MS,
        NVRAM_DRV_FDM,						  /** Device **/										 
        "CT2n", 
        VER(NVRAM_EF_PHOTO_EDITOR_SETTING_LID), 
        "Photo Editor Settings\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE		  /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }
#endif
#ifdef __MMI_INCOMING_CALL_VIDEO__
    ,{
        NVRAM_EF_PHB_VIDEO_LID, 		  /** LID **/                                            
        NVRAM_EF_PHB_VIDEO_SIZE,       /** Size **/                                           
        NVRAM_EF_PHB_VIDEO_COUNT,      /** Total **/                                          
        NVRAM_EF_ZERO_DEFAULT,            			/** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),					/* Attributes */
        NVRAM_CATEGORY_USER,           	/* Category */
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT12", 
        VER(NVRAM_EF_PHB_VIDEO_LID), 
        "PHONEBOOK VIEW SETTING\0",					/* Description*/
        NVRAM_AVERAGE_LID,					/* Class */
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }
#endif  
#ifdef JATAAYU_SUPPORT
    ,{
        NVRAM_EF_WAP_PROFILE_CONTENT_LID,
        NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
        NVRAM_EF_WAP_PROFILE_CONTENT_TOTAL,
        (kal_uint8*)NVRAM_EF_WAP_PROFILE_CONTENT_DEFAULT,
        NVRAM_ATTR_AVERAGE ,
        NVRAM_CATEGORY_MULTI_DEFAULT,
        NVRAM_DRV_FDM,                        /** Device **/    
        "MP2u", 
        VER(NVRAM_EF_WAP_PROFILE_CONTENT_LID), 
        "wap profile\0",
        NVRAM_INVALID_VALUE
    }
#ifdef __MMI_MULTI_SIM__
    ,{
        NVRAM_EF_WAP_PROFILE_CONTENT_SIM2_LID,
        NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
        NVRAM_EF_WAP_PROFILE_CONTENT_TOTAL,
        (kal_uint8*)NVRAM_EF_WAP_PROFILE_CONTENT_DEFAULT,
        NVRAM_ATTR_AVERAGE ,
        NVRAM_CATEGORY_MULTI_DEFAULT,
        NVRAM_DRV_FDM,                        /** Device **/    
        "MP2u", 
        VER(NVRAM_EF_WAP_PROFILE_CONTENT_SIM2_LID), 
        "wap profile\0",
        NVRAM_INVALID_VALUE
    }
    ,{
        NVRAM_EF_WAP_PROFILE_CONTENT_SIM3_LID,
        NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
        NVRAM_EF_WAP_PROFILE_CONTENT_TOTAL,
        (kal_uint8*)NVRAM_EF_WAP_PROFILE_CONTENT_DEFAULT,
        NVRAM_ATTR_AVERAGE ,
        NVRAM_CATEGORY_MULTI_DEFAULT,
        NVRAM_DRV_FDM,                        /** Device **/    
        "MP2u", 
        VER(NVRAM_EF_WAP_PROFILE_CONTENT_SIM2_LID), 
        "wap profile\0",
        NVRAM_INVALID_VALUE
    }
    ,{
        NVRAM_EF_WAP_PROFILE_CONTENT_SIM4_LID,
        NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
        NVRAM_EF_WAP_PROFILE_CONTENT_TOTAL,
        (kal_uint8*)NVRAM_EF_WAP_PROFILE_CONTENT_DEFAULT,
        NVRAM_ATTR_AVERAGE ,
        NVRAM_CATEGORY_MULTI_DEFAULT,
        NVRAM_DRV_FDM,                        /** Device **/    
        "MP2u", 
        VER(NVRAM_EF_WAP_PROFILE_CONTENT_SIM2_LID), 
        "wap profile\0",
        NVRAM_INVALID_VALUE
    }
 #endif   
    ,{
        NVRAM_EF_MMS_PROFILE_CONTENT_LID,
        NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
        NVRAM_EF_MMS_PROFILE_CONTENT_TOTAL,
        (kal_uint8*)NVRAM_EF_MMS_PROFILE_CONTENT_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_MULTI_DEFAULT,
        NVRAM_DRV_FDM,                        /** Device **/    
        "MP2v",
        VER(NVRAM_EF_MMS_PROFILE_CONTENT_LID),
        "mms profile\0",
        NVRAM_INVALID_VALUE
    }

#ifdef __MMI_MULTI_SIM__
    ,{
        NVRAM_EF_MMS_PROFILE_CONTENT_SIM2_LID,
        NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
        NVRAM_EF_MMS_PROFILE_CONTENT_TOTAL,
        (kal_uint8*)NVRAM_EF_MMS_PROFILE_CONTENT_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_MULTI_DEFAULT,
        NVRAM_DRV_FDM,                        /** Device **/    
        "MP2v",
        VER(NVRAM_EF_MMS_PROFILE_CONTENT_SIM2_LID),
        "mms profile\0",
        NVRAM_INVALID_VALUE
    }	
    ,{
        NVRAM_EF_MMS_PROFILE_CONTENT_SIM3_LID,
        NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
        NVRAM_EF_MMS_PROFILE_CONTENT_TOTAL,
        (kal_uint8*)NVRAM_EF_MMS_PROFILE_CONTENT_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_MULTI_DEFAULT,
        NVRAM_DRV_FDM,                        /** Device **/    
        "MP2v",
        VER(NVRAM_EF_MMS_PROFILE_CONTENT_SIM2_LID),
        "mms profile\0",
        NVRAM_INVALID_VALUE
    }	
    ,{
        NVRAM_EF_MMS_PROFILE_CONTENT_SIM4_LID,
        NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
        NVRAM_EF_MMS_PROFILE_CONTENT_TOTAL,
        (kal_uint8*)NVRAM_EF_MMS_PROFILE_CONTENT_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_MULTI_DEFAULT,
        NVRAM_DRV_FDM,                        /** Device **/    
        "MP2v",
        VER(NVRAM_EF_MMS_PROFILE_CONTENT_SIM2_LID),
        "mms profile\0",
        NVRAM_INVALID_VALUE
    }
#endif
    ,{
        NVRAM_EF_WAP_PROFILE_NAMES_LID,
        NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
        NVRAM_EF_WAP_PROFILE_NAMES_TOTAL,
        (kal_uint8*)NVRAM_EF_WAP_PROFILE_NAMES_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_USER,
        NVRAM_DRV_FDM,                        /** Device **/            
        "MP2w",
        VER(NVRAM_EF_WAP_PROFILE_NAMES_LID),
        "wap profile name\0",
        NVRAM_INVALID_VALUE
    }
#ifdef __MMI_MULTI_SIM__
    ,{
        NVRAM_EF_WAP_PROFILE_NAMES_SIM2_LID,
        NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
        NVRAM_EF_WAP_PROFILE_NAMES_TOTAL,
        (kal_uint8*)NVRAM_EF_WAP_PROFILE_NAMES_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_USER,
        NVRAM_DRV_FDM,                        /** Device **/            
        "MP2w",
        VER(NVRAM_EF_WAP_PROFILE_NAMES_SIM2_LID),
        "wap sim2 profile name\0",
        NVRAM_INVALID_VALUE
    }
    ,{
        NVRAM_EF_WAP_PROFILE_NAMES_SIM3_LID,
        NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
        NVRAM_EF_WAP_PROFILE_NAMES_TOTAL,
        (kal_uint8*)NVRAM_EF_WAP_PROFILE_NAMES_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_USER,
        NVRAM_DRV_FDM,                        /** Device **/            
        "MP2w",
        VER(NVRAM_EF_WAP_PROFILE_NAMES_SIM2_LID),
        "wap sim2 profile name\0",
        NVRAM_INVALID_VALUE
    }
    ,{
        NVRAM_EF_WAP_PROFILE_NAMES_SIM4_LID,
        NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
        NVRAM_EF_WAP_PROFILE_NAMES_TOTAL,
        (kal_uint8*)NVRAM_EF_WAP_PROFILE_NAMES_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_USER,
        NVRAM_DRV_FDM,                        /** Device **/            
        "MP2w",
        VER(NVRAM_EF_WAP_PROFILE_NAMES_SIM2_LID),
        "wap sim2 profile name\0",
        NVRAM_INVALID_VALUE
    }
#endif
    ,{
        NVRAM_EF_MMS_PROFILE_NAMES_LID,
        NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
        NVRAM_EF_MMS_PROFILE_NAMES_TOTAL,
        (kal_uint8*)NVRAM_EF_MMS_PROFILE_NAMES_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_USER,
        NVRAM_DRV_FDM,                        /** Device **/            
        "MP2x", 
        VER(NVRAM_EF_MMS_PROFILE_NAMES_LID), 
        "mms profile name\0",
        NVRAM_INVALID_VALUE
    }
#ifdef __MMI_MULTI_SIM__
    ,{
        NVRAM_EF_MMS_PROFILE_NAMES_SIM2_LID,
        NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
        NVRAM_EF_MMS_PROFILE_NAMES_TOTAL,
        (kal_uint8*)NVRAM_EF_MMS_PROFILE_NAMES_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_USER,
        NVRAM_DRV_FDM,                        /** Device **/            
        "MP2x", 
        VER(NVRAM_EF_MMS_PROFILE_NAMES_SIM2_LID), 
        "mms sim2 profile name\0",
        NVRAM_INVALID_VALUE
    }	
    ,{
        NVRAM_EF_MMS_PROFILE_NAMES_SIM3_LID,
        NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
        NVRAM_EF_MMS_PROFILE_NAMES_TOTAL,
        (kal_uint8*)NVRAM_EF_MMS_PROFILE_NAMES_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_USER,
        NVRAM_DRV_FDM,                        /** Device **/            
        "MP2x", 
        VER(NVRAM_EF_MMS_PROFILE_NAMES_SIM3_LID), 
        "mms sim2 profile name\0",
        NVRAM_INVALID_VALUE
    }	
    ,{
        NVRAM_EF_MMS_PROFILE_NAMES_SIM4_LID,
        NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
        NVRAM_EF_MMS_PROFILE_NAMES_TOTAL,
        (kal_uint8*)NVRAM_EF_MMS_PROFILE_NAMES_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_USER,
        NVRAM_DRV_FDM,                        /** Device **/            
        "MP2x", 
        VER(NVRAM_EF_MMS_PROFILE_NAMES_SIM4_LID), 
        "mms sim2 profile name\0",
        NVRAM_INVALID_VALUE
    }
#endif
#endif

#ifdef __MMI_EBOOK_READER__ 
    ,{
        NVRAM_EF_EBOOK_SETTINGS_LID,
        NVRAM_EF_EBOOK_SETTING_SIZE,
        NVRAM_EF_EBOOK_SETTING_TOTAL,
        NVRAM_EF_EBOOK_DEFAULT_VALUE,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_USER| NVRAM_CATEGORY_FACTORY,
        NVRAM_DRV_FDM,                        /** Device **/                    
        "MP30",
        VER(NVRAM_EF_EBOOK_SETTINGS_LID),
        "Ebook Settings",
        NVRAM_INVALID_VALUE
    }
#endif /* __MMI_EBOOK_READER__ */

#if defined( __MMI_ANALOGTV__)	
    ,{
        NVRAM_EF_ATV_PLAYER_SETTINGS_LID,
        NVRAM_EF_ATV_PLAYER_SETTINGS_SIZE,
        NVRAM_EF_ATV_PLAYER_SETTINGS_TOTAL,
        NVRAM_EF_ATV_PLAYER_SETTINGS_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_USER,
        NVRAM_DRV_FDM,                        /** Device **/            

        "MP2y",
        "000",
        "AnalogTV settings\0",
        NVRAM_INVALID_VALUE
    }
    ,{
        NVRAM_EF_ATV_PLAYER_CHANNEL_LID,
        NVRAM_EF_ATV_PLAYER_CHANNEL_SIZE,
        NVRAM_EF_ATV_PLAYER_CHANNEL_TOTAL,
        NVRAM_EF_ZERO_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_USER,
        NVRAM_DRV_FDM,                        /** Device **/            

        "MP2z",
        "000",
        "AnalogTV channel\0",
        NVRAM_INVALID_VALUE
    }
#endif
#ifdef __MMI_NITZ__
    ,{
        NVRAM_EF_NITZ_NW_NAME_LID,
        NVRAM_NITZ_NW_NAME_SIZE,
        NVRAM_NITZ_NW_NAME_TOTAL,
        NVRAM_EF_ZERO_DEFAULT,
        NVRAM_ATTR_AVERAGE,
        NVRAM_CATEGORY_USER,
        NVRAM_DRV_FDM,                        /** Device **/            
        "MP31",
        "000",
        "NITZ PLMN\0",
        NVRAM_INVALID_VALUE
    }
#endif /* __MMI_NITZ__ */
#if defined(CSD_HW_LEVEL_NVRAM)
    ,{
        NVRAM_EF_CUST_HW_LEVEL_TBL_LID, 		  /** LID **/                                            
        NVRAM_EF_CUST_HW_LEVEL_TBL_SIZE,      /** Size **/                                           
        NVRAM_EF_CUST_HW_LEVEL_TBL_TOTAL,     /** Total **/                                          
        //(const kal_uint8 *)NVRAM_EF_CUST_HW_LEVEL_TBL_DEFAULT,   /** Default value **/     
        NVRAM_EF_ZERO_DEFAULT,
        //lilei modify 20060615
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_SYSTEM,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT03", 
        VER(NVRAM_EF_CUST_HW_LEVEL_TBL_LID), 
        "HW Level\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    } 
#endif
#if defined(CSD_PHB_EXT_NVRAM)
    ,{
        NVRAM_EF_PHB_LID, 					     /** LID **/                                            
        NVRAM_EF_PHB_SIZE,                    /** Size **/                                           
        NVRAM_EF_PHB_TOTAL,                   /** Total **/                                          
        NVRAM_EF_FF_DEFAULT,                                 /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        (NVRAM_CATEGORY_USER),                        /* Category */      
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1i", 
        VER(NVRAM_EF_PHB_LID), 
        "Phone Book\0",
        NVRAM_IMPORTANT_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {
        NVRAM_EF_PHB_LN_ENTRY_LID, 					     /** LID **/                                            
        NVRAM_EF_PHB_LN_ENTRY_SIZE,                    /** Size **/                                           
        NVRAM_EF_PHB_LN_ENTRY_TOTAL,                   /** Total **/                                          
        NVRAM_EF_ZERO_DEFAULT,                      /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1j", 
        VER(NVRAM_EF_PHB_LN_ENTRY_LID), 
        "Last Dial Num\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }
#endif    
#if defined(SHORTCUTS_APP)
    ,{
        NVRAM_EF_SHORTCUTS_LID,                        /* LID */
        NVRAM_SHORTCUTS_SIZE,                       /* Size */
        NVRAM_SHORTCUTS_TOTAL,                      /* Number of records */
        NVRAM_EF_FF_DEFAULT,                    /* Default value */
        NVRAM_ATTR_NONE,                        /* Attributes */
        NVRAM_CATEGORY_SYSTEM,                        /* Category */
        NVRAM_DRV_FDM,                              /* Device */
        "CT08", 
        VER(NVRAM_EF_SHORTCUTS_LID), 
        "Shortcuts\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,             /* Reserved. record ID is calculated by NVRAM */
    }
#endif
#if defined(__DOWNLOAD__)    
    ,{
        NVRAM_EF_EMS_MY_PICTURE_NAME_LID,                               /* LID */
        NVRAM_EMS_MY_PICTURE_NAME_RECORD_SIZE,                       /* Size */
        NVRAM_EMS_MY_PICTURE_NAME_RECORD_TOTAL,                      /* Number of records */
        NVRAM_EF_FF_DEFAULT,                    /* Default value */
        NVRAM_ATTR_NONE,                        /* Attributes */
        NVRAM_CATEGORY_USER,                        /* Category */
        NVRAM_DRV_FDM,                              /* Device */
        "CT0d", 
        VER(NVRAM_EF_EMS_MY_PICTURE_NAME_LID), 
        "EMS Picture\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,             /* Reserved. record ID is calculated by NVRAM */
    }
#endif
#if defined(__MMI_IMELODY_SUPPORT__)
    ,{
        NVRAM_EF_EMS_MY_ANIMATION_NAME_LID,                               /* LID */
        NVRAM_EMS_MY_ANIMATION_NAME_RECORD_SIZE,                       /* Size */
        NVRAM_EMS_MY_ANIMATION_NAME_RECORD_TOTAL,                      /* Number of records */
        NVRAM_EF_FF_DEFAULT,                    /* Default value */
        NVRAM_ATTR_NONE,                        /* Attributes */
        NVRAM_CATEGORY_USER,                        /* Category */
        NVRAM_DRV_FDM,                              /* Device */
        "CT0e", 
        VER(NVRAM_EF_EMS_MY_ANIMATION_NAME_LID), 
        "EMS Animation\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,             /* Reserved. record ID is calculated by NVRAM */
    },
    {
        NVRAM_EF_EMS_MY_MELODY_NAME_LID,                               /* LID */
        NVRAM_EMS_MY_MELODY_NAME_RECORD_SIZE,                       /* Size */
        NVRAM_EMS_MY_MELODY_NAME_RECORD_TOTAL,                      /* Number of records */
        NVRAM_EF_FF_DEFAULT,                    /* Default value */
        NVRAM_ATTR_NONE,                        /* Attributes */
        NVRAM_CATEGORY_USER,                        /* Category */
        NVRAM_DRV_FDM,                              /* Device */
        "CT0f", 
        VER(NVRAM_EF_EMS_MY_MELODY_NAME_LID), 
        "EMS Melody\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,             /* Reserved. record ID is calculated by NVRAM */
    }
#endif
#ifdef __MMI_TODOLIST__
   , {
        NVRAM_EF_TODO_LIST_LID,						/* LID */                                       
        NVRAM_TODO_LIST_RECORD_SIZE,				/* Size */                                      
        NVRAM_TODO_LIST_RECORD_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0r", 
        VER(NVRAM_EF_TODO_LIST_LID), 
        "Todo List\0",
        NVRAM_IMPORTANT_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    }
#endif
#ifndef __MMI_THEMES_V2_SUPPORT__     
    ,{
        NVRAM_EF_THEMES_VALUES,						/* LID */                                       
        NVRAM_THEMES_VALUES_SIZE,				/* Size */                                      
        NVRAM_THEMES_VALUES_RECORDS,			/* Number of records */                         
        NVRAM_THEMES_VALUES_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_MULTIPLE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0q", 
        VER(NVRAM_EF_THEMES_VALUES), 
        "Theme Values\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    }
#endif    
#if defined(__PHB_CALLER_IMAGE_SAVE_PATH_ONLY__)
   , {
      NVRAM_EF_PHB_PIC_LID, 					 /** LID **/                                            
      NVRAM_EF_PHB_PIC_SIZE,              /** Size **/                                           
      NVRAM_EF_PHB_PIC_TOTAL,             /** Total **/                                          
      NVRAM_EF_ZERO_DEFAULT,           /** Default value **/                                  
      (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
      NVRAM_CATEGORY_USER,                  /** Category **/                                       
      NVRAM_DRV_FDM,                        /** Device **/                                         
     "MP32", 
     "000", 
      "PHB PIC\0",
      NVRAM_AVERAGE_LID, 
      (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }
#endif
#ifdef __MMI_VIP_FUNCTION__
,{
	NVRAM_EF_VIP_FUNCTION_LID,			/* LID */                                       
	NVRAM_EF_VIP_FUNCTION_SIZE,				/* Size */                                      
	NVRAM_EF_VIP_FUNCTION_TOTAL,			/* Number of records */                         
	NVRAM_EF_ZERO_DEFAULT,					/* Default value */                             
	NVRAM_ATTR_NONE,						/* Attributes */                                
	NVRAM_CATEGORY_USER,                        /* Category */                                  
	NVRAM_DRV_FDM,                              /* Device */                                    
	"CT2y", 
	VER(NVRAM_EF_VIP_FUNCTION_LID), 
	"VIP Function\0",
	NVRAM_IMPORTANT_LID, 
	(kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
}
#endif
#if defined(CSD_NO_SLIM_NVRAM)	
    ,{
        NVRAM_EF_CALORIE_DATA_LID,				/* LID */                                       
        NVRAM_CALORIE_DATA_SIZE,				/* Size */                                      
        NVRAM_CALORIE_DATA_TOTAL,				/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,			/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0k", 
        VER(NVRAM_EF_CALORIE_DATA_LID), 
        "Calorie Data\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    {
        NVRAM_EF_DOWNLOAD_IMAGE_LID,						/* LID */                                       
        NVRAM_DOWNLOAD_IMAGE_RECORD_SIZE * MAXIMUM_DOWNLOAD_IMAGES,				/* Size */                                      
        NVRAM_DOWNLOAD_IMAGE_RECORD_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0p", 
        VER(NVRAM_EF_DOWNLOAD_IMAGE_LID), 
        "Download Image\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    {	
        NVRAM_EF_COMPOSE_RINGTONE_LID,			/* LID */
        NVRAM_COMPOSE_RINGTONE_SIZE,				/* Size */                                      
        NVRAM_COMPOSE_RINGTONE_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0t", 
        VER(NVRAM_EF_COMPOSE_RINGTONE_LID), 		
        "Comp Ringtone\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },    
    {
        NVRAM_EF_DOWNLOAD_TONE_LID,						/* LID */                                       
        NVRAM_DOWNLOAD_TONE_RECORD_SIZE * MAXIMUM_DOWNLOAD_AUDIO,				/* Size */                                      
        NVRAM_DOWNLOAD_TONE_RECORD_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0u", 
        VER(NVRAM_EF_DOWNLOAD_TONE_LID), 
        "Download Tone\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    {
        NVRAM_EF_DOWNLOAD_MELODY_LID,						/* LID */                                       
        NVRAM_DOWNLOAD_MELODY_RECORD_SIZE * MAXIMUM_DOWNLOAD_AUDIO,				/* Size */                                      
        NVRAM_DOWNLOAD_MELODY_RECORD_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT0v", 
        VER(NVRAM_EF_DOWNLOAD_MELODY_LID), 
        "Download Melody\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    {
        NVRAM_EF_ROOT_CA_LID,	       /** LID **/                                            
        NVRAM_EF_ROOT_CA_SIZE,           /** Size **/                                           
        NVRAM_EF_ROOT_CA_TOTAL,          /** Total **/                                          
        NVRAM_EF_ROOT_CA_DEFAULT,        /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),    /** Attributes **/      
        NVRAM_CATEGORY_USER,                /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT0z", 
        VER(NVRAM_EF_ROOT_CA_LID), 
        "WAP ROOT CA\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },   
    {
        NVRAM_EF_IMEI_IMEISV_LID,				  /** LID **/                                            
        NVRAM_EF_IMEI_IMEISV_SIZE,            /** Size **/                                           
        NVRAM_EF_IMEI_IMEISV_TOTAL,           /** Total **/                                          
        NVRAM_EF_IMEI_IMEISV_DEFAULT,         /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_WRITEPROTECT | NVRAM_ATTR_MULTIPLE | NVRAM_ATTR_CONFIDENTIAL), /** Attributes **/
        NVRAM_CATEGORY_USER,                /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT16", 
        VER(NVRAM_EF_IMEI_IMEISV_LID), 
        "IMEI IMEISV\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {
        NVRAM_EF_CLASSMARK_RACAP_LID, 				  /** LID **/                                            
        NVRAM_EF_CLASSMARK_RACAP_SIZE,             /** Size **/                                           
        NVRAM_EF_CLASSMARK_RACAP_TOTAL,            /** Total **/                                          
        NVRAM_EF_CLASSMARK_RACAP_DEFAULT,          /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_WRITEPROTECT | NVRAM_ATTR_MULTIPLE), /** Attributes **/
        NVRAM_CATEGORY_USER,                /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1b", 
        VER(NVRAM_EF_CLASSMARK_RACAP_LID), 
        "CLASSMARK RACAP\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {
        NVRAM_EF_ADC_LID,                     /** LID **/                                            
        NVRAM_EF_ADC_SIZE,                    /** Size **/                                           
        NVRAM_EF_ADC_TOTAL,                   /** Total **/                                          
        (const kal_uint8 *)NVRAM_EF_ADC_DEFAULT,                 /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_MULTIPLE), /** Attributes **/
        NVRAM_CATEGORY_USER,                /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1d", 
        VER(NVRAM_EF_ADC_LID), 
        "ADC\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {
        NVRAM_EF_UEM_MANUFACTURE_DATA_LID, 	  /** LID **/                                            
        NVRAM_EF_UEM_MANUFACTURE_DATA_SIZE,   /** Size **/                                           
        NVRAM_EF_UEM_MANUFACTURE_DATA_TOTAL,  /** Total **/                                          
        NVRAM_EF_UEM_MANUFACTURE_DATA_DEFAULT,/** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1k", 
        VER(NVRAM_EF_UEM_MANUFACTURE_DATA_LID), 
        "UEM Manufact\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }, 
    {
        NVRAM_EF_UEM_RMI_DATA_LID, 			  /** LID **/                                            
        NVRAM_EF_UEM_RMI_DATA_SIZE,           /** Size **/                                           
        NVRAM_EF_UEM_RMI_DATA_TOTAL,          /** Total **/                                          
        NVRAM_EF_UEM_RMI_DATA_DEFAULT,        /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1l", 
        VER(NVRAM_EF_UEM_RMI_DATA_LID), 
        "UEM RMI\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }, 
    {
        NVRAM_EF_SMSAL_MWIS_LID, 				  /** LID **/                                            
        NVRAM_EF_SMSAL_MWIS_SIZE,             /** Size **/                                           
        NVRAM_EF_SMSAL_MWIS_TOTAL,            /** Total **/                                          
        NVRAM_EF_ZERO_DEFAULT,          /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1n", 
        VER(NVRAM_EF_SMSAL_MWIS_LID), 
        "SMSAL MWIS\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }, 
    {
        NVRAM_EF_SMSAL_SMS_LID, 				  /** LID **/                                            
        NVRAM_EF_SMSAL_SMS_SIZE,              /** Size **/                                           
        NVRAM_EF_SMSAL_SMS_TOTAL,             /** Total **/                                          
        NVRAM_EF_SMSAL_SMS_DEFAULT,           /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1p", 
        VER(NVRAM_EF_SMSAL_SMS_LID), 
        "SMSAL SMS\0",
        NVRAM_IMPORTANT_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }, 
    {
        NVRAM_EF_TCM_PDP_PROFILE_LID, 		  /** LID **/                                            
        NVRAM_EF_TCM_PDP_PROFILE_SIZE,        /** Size **/                                           
        NVRAM_EF_TCM_PDP_PROFILE_TOTAL,       /** Total **/                                          
        NVRAM_EF_TCM_PDP_PROFILE_DEFAULT,     /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1s", 
        VER(NVRAM_EF_TCM_PDP_PROFILE_LID), 
        "TCM PDP Profile\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }, 
    {
        NVRAM_EF_PORT_SETTING_LID, 			     /** LID **/                                            
        NVRAM_EF_PORT_SETTING_SIZE,            /** Size **/                                           
        NVRAM_EF_PORT_SETTING_TOTAL,           /** Total **/                                          
        NVRAM_EF_PORT_SETTING_DEFAULT,         /** Default value **/                                  
        (kal_uint8) NVRAM_ATTR_NONE,          /** Attributes **/
        NVRAM_CATEGORY_USER,                /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT1w", 
        VER(NVRAM_EF_PORT_SETTING_LID), 
        "Port Settings\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {	
        NVRAM_PICTURE_EDITOR_LID,			/* LID */
        NVRAM_PICTURE_EDITOR_SIZE,				/* Size */                                      
        NVRAM_PICTURE_EDITOR_TOTAL,			/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT1y", 
        VER(NVRAM_PICTURE_EDITOR_LID), 		
        "Pic Editor\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    {	
        NVRAM_EF_MSG_CLUB_LID,			/* LID */
        NVRAM_MSG_CLUB_RECORD_SIZE,				/* Size */                                      
        NVRAM_MSG_CLUB_RECORD_TOTAL,			/* Number of records */                         
        NVRAM_MSG_CLUB_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT20", 
        VER(NVRAM_EF_MSG_CLUB_LID), 		
        "Msg Club\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    {	
        NVRAM_EF_IMPS_LID,		               /* LID */
        NVRAM_EF_IMPS_SIZE,            		/* Size */                                      
        NVRAM_EF_IMPS_TOTAL,              	/* Number of records */                         
        NVRAM_EF_ZERO_DEFAULT,						   	/* Default value */                             
        NVRAM_ATTR_NONE,							   	/* Attributes */                                
        NVRAM_CATEGORY_USER,                		/* Category */                                  
        NVRAM_DRV_FDM,                      		/* Device */                                    
        "CT2e",
        VER(NVRAM_EF_IMPS_LID),
        "IMPS profile setting\0",
        NVRAM_AVERAGE_LID,
        (kal_uint8)NVRAM_INVALID_VALUE        	/* Reserved. record ID is calculated by NVRAM */
    },
    {
        NVRAM_EF_PHB_LN_TYPE_SEQ_LID, 				/** LID **/                                            
        NVRAM_EF_PHB_LN_TYPE_SEQ_SIZE,            /** Size **/                                           
        NVRAM_EF_PHB_LN_TYPE_SEQ_TOTAL,           /** Total **/                                          
        NVRAM_EF_FF_DEFAULT,                      /** Default value **/                                  
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CT2g", 
        VER(NVRAM_EF_PHB_LN_TYPE_SEQ_LID), 
        //123456789012345
        "PHB LN Type Seq\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {
        NVRAM_EF_EONS_INFO_LID,						/* LID */                                       
        NVRAM_EF_EONS_INFO_SIZE,				/* Size */                                      
        NVRAM_EF_EONS_INFO_TOTAL,			/* Number of records */                         
        NVRAM_EF_FF_DEFAULT,					/* Default value */                             
        NVRAM_ATTR_NONE,						/* Attributes */                                
        NVRAM_CATEGORY_USER,                        /* Category */                                  
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CT2p", 
        VER(NVRAM_EF_EONS_INFO_LID), 
        "EONS Info\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    }
#endif

#ifdef __MMI_MESSAGES_BLACK_LIST__
    ,{
        NVRAM_EF_MESSAGES_BLACK_LIST_LID,				/* LID */
        NVRAM_EF_MESSAGES_BLACK_LIST_SIZE,				/* Size */
        NVRAM_EF_MESSAGES_BLACK_LIST_TOTAL,			/* Number of records */
        NVRAM_EF_ZERO_DEFAULT,	/* Default value */
        (kal_uint8)(NVRAM_ATTR_NONE),					/* Attributes */
        NVRAM_CATEGORY_USER,           	/* Category */
        NVRAM_DRV_FDM,                      			/* Device */
        "CT24",						/* Prefix */
        VER(NVRAM_EF_MESSAGES_BLACK_LIST_LID),			/* Verno */
        "Messages Black List\0",					/* Description*/
        NVRAM_AVERAGE_LID,					/* Class */
        (kal_uint8)NVRAM_INVALID_VALUE,        	/* Reserved. record ID is calculated by NVRAM */
    }
#endif

#if defined(__MMI_SAVE_CURRENT_TIME__)
    ,{
        NVRAM_EF_SAVE_CURRENT_TIME_LID,				/* LID */
        NVRAM_EF_SAVE_CURRENT_TIME_SIZE,				/* Size */
        NVRAM_EF_SAVE_CURRENT_TIME_TOTAL,			/* Number of records */
        NVRAM_EF_SAVE_CURRENT_TIME_DEFAULT,	/* Default value */
        (kal_uint8)(NVRAM_ATTR_NONE),					/* Attributes */
        NVRAM_CATEGORY_USER,           	/* Category */
        NVRAM_DRV_FDM,                      			/* Device */
        "CT25",						/* Prefix */
        VER(NVRAM_EF_SAVE_CURRENT_TIME_LID),			/* Verno */
        "Current system time\0",					/* Description*/
        NVRAM_AVERAGE_LID,					/* Class */
        (kal_uint8)NVRAM_INVALID_VALUE,        	/* Reserved. record ID is calculated by NVRAM */
    }
#endif
#ifdef __MMI_TTS_FEATURES__
    ,{
	  NVRAM_SETTING_TTS_LID,
	  NVRAM_SETTING_TTS_SIZE,
	  NVRAM_SETTING_TTS_TOTAL,
	  NVRAM_EF_FF_DEFAULT,              
	  NVRAM_ATTR_AVERAGE,
	  NVRAM_CATEGORY_USER | NVRAM_CATEGORY_CUSTPACK | NVRAM_CATEGORY_FACTORY,     
	  NVRAM_DRV_FDM,                        /** Device **/	  
	  "CCC",
	  VER(NVRAM_SETTING_TTS_LID),
	  "TTS\0",
	  NVRAM_AVERAGE_LID,					/* Class */
	  (kal_uint8)NVRAM_INVALID_VALUE,
	}	
#endif

};
  
extern	kal_uint8	NVRAM_SETTING_DEFAULT[NVRAM_SETTING_SIZE];
extern  kal_uint8   NVRAM_CACHE_BYTE_DEFAULT[NVRAM_CACHE_SIZE];
extern  kal_uint8   NVRAM_CACHE_SHORT_DEFAULT[NVRAM_CACHE_SIZE];
extern  kal_uint8   NVRAM_CACHE_DOUBLE_DEFAULT[NVRAM_CACHE_SIZE];
extern  kal_uint8   NVRAM_PHONEBOOK_SOS_DEFAULT [NVRAM_PHONEBOOK_SOS_RECORD_SIZE];
extern  kal_uint8   NVRAM_PHONEBOOK_SPEEDDIAL_DEFAULT [NVRAM_PHONEBOOK_SPEEDDIAL_RECORD_SIZE];
extern	kal_uint8	NVRAM_EF_SMSAL_MAILBOX_ADDR_DEFAULT[] ;
extern	kal_uint8	NVRAM_EF_MS_SECURITY_DEFAULT[];
extern	kal_uint8	NVRAM_EF_ALS_LINE_ID_DEFAULT[NVRAM_EF_ALS_LINE_ID_SIZE];
extern	kal_uint8	NVRAM_EF_MSCAP_DEFAULT[NVRAM_EF_MSCAP_SIZE];
extern	kal_uint8	NVRAM_EF_RAC_PREFERENCE_DEFAULT[];
//extern     NVRAM_EF_SMSAL_COMMON_PARAM_DEFAULT[];
//extern     NVRAM_EF_CB_DEFAULT_CH_DEFAULT[];
 
 
const ltable_entry_struct logical_data_item_table_custpack[NVRAM_EF_LAST_LID_CUSTPACK-NVRAM_EF_CUSTPACK_BASE] = 
{
    {
        NVRAM_EF_SETTING_LID,                        /* LID */
        NVRAM_SETTING_SIZE,                       /* Size */
        NVRAM_SETTING_TOTAL,                      /* Number of records */
        NVRAM_SETTING_DEFAULT,
        NVRAM_ATTR_NONE,                        /* Attributes */
        (NVRAM_CATEGORY_USER | NVRAM_CATEGORY_FACTORY),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */
        "CP01", 
        VER(NVRAM_EF_SETTING_LID), 
        "Settings\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,             /* Reserved. record ID is calculated by NVRAM */
    },
    {
        NVRAM_EF_CACHE_BYTE_LID,                           /* LID */
        NVRAM_CACHE_SIZE,                          /* Size */
        NVRAM_CACHE_TOTAL,                         /* Number of records */
        NVRAM_CACHE_BYTE_DEFAULT,          
        NVRAM_ATTR_NONE ,                         /* Attributes */
        NVRAM_CATEGORY_USER,                         /* Category */
        NVRAM_DRV_FDM,                               /* Device */
        "CP02", 
        VER(NVRAM_EF_CACHE_BYTE_LID), 
        "1-byte Setting\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    {
        NVRAM_EF_CACHE_SHORT_LID,                           /* LID */
        NVRAM_CACHE_SIZE,                          /* Size */
        NVRAM_CACHE_TOTAL,                         /* Number of records */
         
        NVRAM_CACHE_SHORT_DEFAULT,
        NVRAM_ATTR_NONE ,                         /* Attributes */
        NVRAM_CATEGORY_USER,                         /* Category */
        NVRAM_DRV_FDM,                               /* Device */
        "CP03", 
        VER(NVRAM_EF_CACHE_SHORT_LID), 
        "2-byte Setting\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
     },
    {
        NVRAM_EF_CACHE_DOUBLE_LID,                           /* LID */
        NVRAM_CACHE_SIZE,                          /* Size */
        NVRAM_CACHE_TOTAL,                         /* Number of records */
         
        NVRAM_CACHE_DOUBLE_DEFAULT,     
        NVRAM_ATTR_NONE ,                         /* Attributes */
        NVRAM_CATEGORY_USER,                         /* Category */
        NVRAM_DRV_FDM,                               /* Device */
        "CP04", 
        VER(NVRAM_EF_CACHE_DOUBLE_LID), 
        "8-byte Setting\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    {
        NVRAM_EF_PHONEBOOK_SOS_LID,						/* LID */                                       
        NVRAM_PHONEBOOK_SOS_RECORD_SIZE,				/* Size */                                      
        NVRAM_PHONEBOOK_SOS_RECORD_TOTAL,			/* Number of records */                         
         
        NVRAM_EF_FF_DEFAULT,       
        NVRAM_ATTR_NONE,						/* Attributes */                                
        (NVRAM_CATEGORY_USER),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CP05", 
        VER(NVRAM_EF_PHONEBOOK_SOS_LID), 
        "Phonebook SOS\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    } ,
#ifdef __MMI_MULTI_SIM__
    {
        NVRAM_EF_PHONEBOOK_SIM2_SOS_LID,						/* LID */                                       
        NVRAM_PHONEBOOK_SOS_RECORD_SIZE,				/* Size */                                      
        NVRAM_PHONEBOOK_SOS_RECORD_TOTAL,			/* Number of records */                         
         
        NVRAM_EF_FF_DEFAULT,       
        NVRAM_ATTR_NONE,						/* Attributes */                                
        (NVRAM_CATEGORY_USER),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CP0e", 
        VER(NVRAM_EF_PHONEBOOK_SIM2_SOS_LID), 
        "Phonebook SOS\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE             /* Reserved. record ID is calculated by NVRAM */
    },
    {
        NVRAM_EF_PHONEBOOK_SIM3_SOS_LID,						/* LID */                                       
        NVRAM_PHONEBOOK_SOS_RECORD_SIZE,				/* Size */                                      
        NVRAM_PHONEBOOK_SOS_RECORD_TOTAL,			/* Number of records */                         
         
        NVRAM_EF_FF_DEFAULT,       
        NVRAM_ATTR_NONE,						/* Attributes */                                
        (NVRAM_CATEGORY_USER),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CP0e", 
        VER(NVRAM_EF_PHONEBOOK_SIM3_SOS_LID), 
        "Phonebook SOS\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE             /* Reserved. record ID is calculated by NVRAM */
    },
    {
        NVRAM_EF_PHONEBOOK_SIM4_SOS_LID,						/* LID */                                       
        NVRAM_PHONEBOOK_SOS_RECORD_SIZE,				/* Size */                                      
        NVRAM_PHONEBOOK_SOS_RECORD_TOTAL,			/* Number of records */                         
         
        NVRAM_EF_FF_DEFAULT,       
        NVRAM_ATTR_NONE,						/* Attributes */                                
        (NVRAM_CATEGORY_USER),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CP0e", 
        VER(NVRAM_EF_PHONEBOOK_SIM3_SOS_LID), 
        "Phonebook SOS\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE             /* Reserved. record ID is calculated by NVRAM */
    },
#endif
    {
        NVRAM_EF_PHONEBOOK_SPEEDDIAL_LID,						/* LID */                                       
        NVRAM_PHONEBOOK_SPEEDDIAL_RECORD_SIZE,				/* Size */                                      
        NVRAM_PHONEBOOK_SPEEDDIAL_RECORD_TOTAL,			/* Number of records */                         
         
        NVRAM_PHONEBOOK_SPEEDDIAL_DEFAULT ,
        NVRAM_ATTR_NONE,						/* Attributes */                                
        (NVRAM_CATEGORY_USER 
        | NVRAM_CATEGORY_FACTORY
        ),                        /* Category */      
        NVRAM_DRV_FDM,                              /* Device */                                    
        "CP06", 
        VER(NVRAM_EF_PHONEBOOK_SPEEDDIAL_LID), 
        "Phonebook SpeedDial\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,              /* Reserved. record ID is calculated by NVRAM */
    },
    {
        NVRAM_EF_CHIS_NUM_LID, 	  /** LID **/                                            
        NVRAM_EF_CHIS_NUM_SIZE,     /** Size **/                                           
        NVRAM_EF_CHIS_NUM_TOTAL,    /** Total **/                                          
        NVRAM_EF_CHIS_NUM_EFAULT,                    /* Default value */    
        //NVRAM_EF_CB_DEFAULT_CH_DEFAULT,
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CP0d", 
        VER(NVRAM_EF_CB_DEFAULT_CH_LID), 
        "CB Default CH\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {
        NVRAM_EF_UNREAD_MISSED_NUM_LID, 	  /** LID **/                                            
        NVRAM_EF_UNREAD_MISSED_NUM_SIZE,     /** Size **/                                           
        NVRAM_EF_UNREAD_MISSED_NUM_TOTAL,    /** Total **/                                          
        NVRAM_EF_ZERO_DEFAULT,                    /* Default value */    
        //NVRAM_EF_CB_DEFAULT_CH_DEFAULT,
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CP0d", 
        VER(NVRAM_EF_CB_DEFAULT_CH_LID), 
        "CB Default CH\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
#if defined(CSD_NO_SLIM_NVRAM)	                   
    {
        NVRAM_EF_CUSTPACK_VERNO_LID,                        /* LID */
        NVRAM_EF_CUSTPACK_VERNO_SIZE,                       /* Size */
        NVRAM_EF_CUSTPACK_VERNO_TOTAL,                      /* Number of records */
        NVRAM_EF_ZERO_DEFAULT,                    /* Default value */
        (kal_uint8)( NVRAM_ATTR_MULTIPLE 
        | NVRAM_ATTR_WRITEPROTECT ), /** Attributes **/
        NVRAM_CATEGORY_USER,                        /* Category */
        NVRAM_DRV_FDM,                              /* Device */
        "CP00", 
        VER(NVRAM_EF_CUSTPACK_VERNO_LID), 
        "Cust Pack Bin Ver\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE,             /* Reserved. record ID is calculated by NVRAM */
    },	
    {
        NVRAM_EF_SMSAL_MAILBOX_ADDR_LID, 	  /** LID **/                                            
        NVRAM_EF_SMSAL_MAILBOX_ADDR_SIZE,     /** Size **/                                           
        NVRAM_EF_SMSAL_MAILBOX_ADDR_TOTAL,    /** Total **/                                          
         
        NVRAM_EF_SMSAL_MAILBOX_ADDR_DEFAULT,
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CP07", 
        VER(NVRAM_EF_SMSAL_MAILBOX_ADDR_LID), 
        "SMSAL MAILBOX\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }, 
    {
        NVRAM_EF_MS_SECURITY_LID, 			     /** LID **/                                            
        NVRAM_EF_MS_SECURITY_SIZE,            /** Size **/                                           
        NVRAM_EF_MS_SECURITY_TOTAL,           /** Total **/                                          
         
        NVRAM_EF_MS_SECURITY_DEFAULT,
        (kal_uint8)( NVRAM_ATTR_MULTIPLE 
        | NVRAM_ATTR_CONFIDENTIAL ), /** Attributes **/
        NVRAM_CATEGORY_USER,                /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CP08", 
        VER(NVRAM_EF_MS_SECURITY_LID), 
        "MS SECURITY\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    }, 
    {
        NVRAM_EF_ALS_LINE_ID_LID, 		  /** LID **/                                            
        NVRAM_EF_ALS_LINE_ID_SIZE,        /** Size **/                                           
        NVRAM_EF_ALS_LINE_ID_TOTAL,       /** Total **/                                          
         
        NVRAM_EF_ALS_LINE_ID_DEFAULT,
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CP09", 
        VER(NVRAM_EF_ALS_LINE_ID_LID), 
        "ALS Line\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {                                                                   
        NVRAM_EF_MSCAP_LID,					     /** LID **/                                                                                         
        NVRAM_EF_MSCAP_SIZE,                  /** Size **/                                                                     
        NVRAM_EF_MSCAP_TOTAL,                 /** Total **/                                                                    
         
        NVRAM_EF_MSCAP_DEFAULT,
        (kal_uint8)( NVRAM_ATTR_WRITEPROTECT 
        | NVRAM_ATTR_MULTIPLE),	/** Attributes **/      
        NVRAM_CATEGORY_USER,                /** Category **/                                                                 
        NVRAM_DRV_FDM,                        /** Device **/                                                                   
        "CP0a", 
        VER(NVRAM_EF_MSCAP_LID), 
        //123456789012345
        "MSCAP\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {
        NVRAM_EF_RAC_PREFERENCE_LID, 			  /** LID **/                                            
        NVRAM_EF_RAC_PREFERENCE_SIZE,         /** Size **/                                           
        NVRAM_EF_RAC_PREFERENCE_TOTAL,        /** Total **/                                          
         
        NVRAM_EF_RAC_PREFERENCE_DEFAULT,
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CP0b", 
        VER(NVRAM_EF_RAC_PREFERENCE_LID), 
        "RAC Preference\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {
        NVRAM_EF_SMSAL_COMMON_PARAM_LID, 	  /** LID **/                                            
        NVRAM_EF_SMSAL_COMMON_PARAM_SIZE,     /** Size **/                                           
        NVRAM_EF_SMSAL_COMMON_PARAM_TOTAL,    /** Total **/                                          
        NULL,                    /* Default value */     
        //NVRAM_EF_SMSAL_COMMON_PARAM_DEFAULT,
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        (NVRAM_CATEGORY_USER 
        | NVRAM_CATEGORY_FACTORY
        ),                                    /* Category */      
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CP0c", 
        VER(NVRAM_EF_SMSAL_COMMON_PARAM_LID), 
        "SMSAL COMMON\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
    {
        NVRAM_EF_CB_DEFAULT_CH_LID, 	  /** LID **/                                            
        NVRAM_EF_CB_DEFAULT_CH_SIZE,     /** Size **/                                           
        NVRAM_EF_CB_DEFAULT_CH_TOTAL,    /** Total **/                                          
        NULL,                    /* Default value */     
        //NVRAM_EF_CB_DEFAULT_CH_DEFAULT,
        (kal_uint8)(NVRAM_ATTR_NONE),         /** Attributes **/                                     
        NVRAM_CATEGORY_USER,                  /** Category **/                                       
        NVRAM_DRV_FDM,                        /** Device **/                                         
        "CP0d", 
        VER(NVRAM_EF_CB_DEFAULT_CH_LID), 
        "CB Default CH\0",
        NVRAM_AVERAGE_LID, 
        (kal_uint8)NVRAM_INVALID_VALUE        /** Record ID is calculated by NVRAM. DO NOT TOUCH. **/
    },
#endif        
};
 


//#ifdef __RESET_FACTORY__
#include "custom_mmi_default_value.h"
 
#if 0
__align(2) const kal_wchar CLEAN_FOLDER1[]= L"Z:\\@wap";
__align(2) const kal_wchar CLEAN_FOLDER2[]= L"Z:\\@mms";
 const kal_wchar * NVRAM_CLEAN_FOLDER_LIST[NVRAM_MAX_CLEAN_FOLDERS] =
{
	CLEAN_FOLDER1,
	CLEAN_FOLDER2
};
 #endif
  
void	nvram_restore_byte_cache(kal_uint8 *buffer)
{
	buffer[NVRAM_SCREENSAVER_STATUS] = buffer[RESTORE_DEFAULT_SCREENSAVER_STATUS];
	buffer[NVRAM_SCREENSAVER_WAITING] = buffer[RESTORE_DEFAULT_SCREENSAVER_WAITING];
	buffer[NVRAM_SETTING_SPEED_DIAL] = buffer[RESTORE_DEFAULT_SPEED_DIAL];
	buffer[NVRAM_SETTING_CONTRAST_LEVEL] = buffer[RESTORE_DEFAULT_CONTRAST_LEVEL];
	buffer[NVRAM_SETTING_CONTRAST_SUBLCD_LEVEL] = buffer[RESTORE_DEFAULT_CONTRAST_SUBLCD_LEVEL];
	buffer[NVRAM_SETTING_WELCOME_TEXT] = buffer[RESTORE_DEFAULT_WELCOME_TEXT];
	buffer[NVRAM_SETTING_APHORISM_TEXT] = buffer[RESTORE_DEFAULT_APHORISM_TEXT];
	buffer[NVRAM_SETTING_OWNER_NO] = buffer[RESTORE_DEFAULT_OWNER_NO];
	buffer[NVRAM_SETTING_STATUS_DT_TIME] = buffer[RESTORE_DEFAULT_STATUS_SHOW_DT_TIME];
	buffer[NVRAM_SETTING_AUTOUPDATE_DT_TIME] = buffer[RESTORE_DEFAULT_AUTOUPDATE_TIME];
	buffer[DATE_FORMAT] = buffer[RESTORE_DEFAULT_DATE_FORMAT];
	buffer[TIME_FORMAT] = buffer[RESTORE_DEFAULT_TIME_FORMAT];
	buffer[NVRAM_PROFILES_ACTIVATED] = 0;

}
void	nvram_restore_short_cache(kal_uint16 *buffer)
{

	buffer[NVRAM_FUNANDGAMES_SETWALLPAPER] = 	buffer[RESTORE_DEFAULT_FUNANDGAMES_SETWALLPAPER];
	buffer[NVRAM_CURRENT_SCREENSVER_ID] = 	buffer[RESTORE_DEFAULT_CURRENT_SCREENSVER_ID];
	buffer[NVRAM_SETTING_PREFER_INPUT_METHOD] = buffer[RESTORE_PREFER_INPUT_METHOD];
	buffer[NVRAM_CURRENT_POWER_ON_DISPLAY] = buffer[NVRAM_RESTORE_POWER_ON_DISPLAY];
	buffer[NVRAM_CURRENT_POWER_OFF_DISPLAY] = buffer[NVRAM_RESTORE_POWER_OFF_DISPLAY];
	buffer[NVRAM_SETWALLPAPER_SUB] = buffer[RESTORE_DEFAULT_SETWALLPAPER_SUB];  
}
void  nvram_restore_double_cache(kal_uint32 *buffer)
{
}
//#endif //__RESET_FACTORY__	


#endif /*MMI_ON_HARDWARE_P*/
