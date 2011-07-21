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
 *	nvram_defs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file contains definitions used by NVRAM task.
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef _NVRAM_DEFS_H
#define _NVRAM_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Header files should be include before this one */
#ifndef _KAL_RELEASE_H
#error "kal_release.h should be included"
#endif

//#define NVRAM_EF_SHORTREG_SIZE 5

#ifdef NVRAM_EF_SHORTREG_SIZE

typedef struct{
	kal_uint8	LID;
	kal_uint16	real_size;
	kal_uint8 const*	real_default;
}nvram_ef_shortreg_struct;
extern const nvram_ef_shortreg_struct  NVRAM_EF_SHORTREG_DEFAULT[NVRAM_EF_SHORTREG_SIZE];
#endif //NVRAM_SHORTREG
#define NVRAM_INVALID_VALUE      ~0
#define NVRAM_EF_ZERO_DEFAULT_VALUE  0x00
extern const kal_uint8 NVRAM_EF_ZERO_DEFAULT[];
extern const kal_uint8 NVRAM_EF_FF_DEFAULT[];


/* Length: 12 */
/**
 * If `lock' field of system record is NVRAM_LOCK_PATTERN_LOCKED, 
 * the global software lock status is LOCKED. (Its default value is assigned through
 * coded_version_lock in ltable.c)
 * Otherwise, the global software lock status is UNLOCKED.
 */
#define NVRAM_LOCK_PATTERN_LOCKED         "_NVRAM__YES_"
#define NVRAM_LOCK_PATTERN_UNLOCKED       " _NVRAM__NO_"



#define FILE_PREFIX_LEN 4
#define FILE_VERNO_LEN  3
#define NVRAM_FILE_LEN (FILE_PREFIX_LEN + 1 + FILE_VERNO_LEN)
#define NVRAM_MAX_DESC_LEN 20

#define NVRAM_MAKE_LID_FILENAME(buf, prefix, M, verno) {\
	((kal_char*) (buf))[0] = ((kal_char*) (prefix))[0];\
	((kal_char*) (buf))[1] = ((kal_char*) (prefix))[1];\
	((kal_char*) (buf))[2] = ((kal_char*) (prefix))[2];\
	((kal_char*) (buf))[3] = ((kal_char*) (prefix))[3];\
	((kal_char*) (buf))[4] = (kal_char) (M);\
	((kal_char*) (buf))[5] = ((kal_char*) (verno))[0];\
	((kal_char*) (buf))[6] = ((kal_char*) (verno))[1];\
	((kal_char*) (buf))[7] = ((kal_char*) (verno))[2];\
	((kal_char*) (buf))[8] = '\0';\
};

#define NVRAM_MAKE_LID_FILENAME_PREFIX(buf, prefix) {\
	((kal_char*) (buf))[0] = ((kal_char*) (prefix))[0];\
	((kal_char*) (buf))[1] = ((kal_char*) (prefix))[1];\
	((kal_char*) (buf))[2] = ((kal_char*) (prefix))[2];\
	((kal_char*) (buf))[3] = ((kal_char*) (prefix))[3];\
	((kal_char*) (buf))[4] = '\0';\
};


typedef kal_char NVRAM_FILE_NAME[9];

/**
 * Device types; predefined.
 * Note: default system device must be 0!!
 */
typedef enum 
{
   NVRAM_DRV_FDM    = 0,             /* Device: FLASH */
   NVRAM_DEV_EEPROM,
   NVRAM_DEV_MMC
} nvram_drv_type_enum;


/* Attributes of a Logical Data Item, predefined. */
typedef kal_uint16 nvram_attr_enum; 
#define NVRAM_ATTR_AVERAGE          0x00
#define NVRAM_ATTR_NONE				0x0
#define NVRAM_ATTR_WRITEPROTECT	0x1
#define NVRAM_ATTR_MULTIPLE		0x2
#define NVRAM_ATTR_CONFIDENTIAL	0x4

#define NVRAM_IS_ATTR_NONE(x)             (x & NVRAM_ATTR_NONE)
#define NVRAM_IS_ATTR_WRITEPROTECT(x)     (x & NVRAM_ATTR_WRITEPROTECT)
#define NVRAM_IS_ATTR_MULTIPLE(x)         (x & NVRAM_ATTR_MULTIPLE)
#define NVRAM_IS_ATTR_CONFIDENTIAL(x)     (x & NVRAM_ATTR_CONFIDENTIAL)

#define NVRAM_TURNOFF_ATTR_MULTIPLE(x)    (x &= ~(NVRAM_ATTR_MULTIPLE))


/**
 * Category of a Logical Data Item could belong to, predefined. 
 * Must be synchronous with nvram_reset_category_enum.
 */
typedef kal_uint32 nvram_category_enum;
#define NVRAM_CATEGORY_NONE				0x00
#define NVRAM_CATEGORY_USER				0x01
#define NVRAM_CATEGORY_SYSTEM				0x02
#define NVRAM_CATEGORY_COMPOSED			0x04
#define NVRAM_CATEGORY_FACTORY			0x08
#define NVRAM_CATEGORY_MULTI_DEFAULT	0x10
#define NVRAM_CATEGORY_CUSTPACK			0x20
#define NVRAM_CATEGORY_BRANCH				0x40
#define NVRAM_CATEGORY_SHADOW				0x80
#define NVRAM_CATEGORY_SHADOW_MS			0x0080

typedef enum 
{
   NVRAM_AVERAGE_LID, 
   NVRAM_IMPORTANT_LID,
   NVRAM_DIFFERENCE_LID 
} nvram_LID_CLASS;


typedef struct 
{
   kal_uint8					LID;             /* Logical Data Item ID */
   kal_uint16					size;            /* Size of one record. < sizeof control buffer */
   kal_uint16					total_records;   /* Number of records */
   kal_uint8 const*			default_value;		/* Hardcoded default */
   nvram_attr_enum			attr;
   nvram_category_enum		category;
   nvram_drv_type_enum		device;           /*  */
   kal_char						fileprefix[FILE_PREFIX_LEN+1];
   kal_char						fileverno[FILE_VERNO_LEN+1];
   kal_char*					description;
   nvram_LID_CLASS			LIDclass;
   kal_uint8					record_ID;        /* Reserved */
} ltable_entry_struct;

#ifdef _LOW_COST_SINGLE_BANK_FLASH_
#define __SHADOW_NVRAM__
#endif

#ifdef __SHADOW_NVRAM__

typedef struct
{
   kal_uint8	LID;             		/* Logical Data Item ID */
	kal_uint32	offset;					/* start address in Shadow buffer*/
	kal_uint32	dirty_records[4];		/* dirty bit map of Shadow records*/
}shadow_ltable_struct;


#endif

/** 
 * NVRAM Data Service System Level Variables, should be referenced
 * both by NVRAMTask Layer and Driver Layer.
 * Customers are required to define them.
 *
 * Convention:
 * logical_data_item_table[] should be able to be indexed by LID. That is, each LID is mapped to `LID'th
 * element in logical_data_item_table[]. Thus avoids linear seaching for LID and speeds up table lookup.
 */
#ifndef NVRAM_EF_CUSTPACK_BASE
#define NVRAM_EF_CUSTPACK_BASE 200
#endif

#if 0
#define nvram_get_data_item(ldi,LID) 	{ \
														kal_uint8 lid = LID; \
														if(lid<=NVRAM_TOTAL_LID) \
															ldi = nvram_ptr->ltable.table[lid]; \
														else \
															ldi = nvram_ptr->ltable.table[lid-NVRAM_EF_CUSTPACK_BASE+NVRAM_LAST_CUST_LID]; \
													}

extern ltable_entry_struct* logical_data_item_table[];
#endif
extern ltable_entry_struct logical_data_item_table_vendor[];
extern const ltable_entry_struct logical_data_item_table_cust[];
extern const  ltable_entry_struct logical_data_item_table_custpack[];
extern const kal_uint8 NVRAM_TOTAL_LID;
extern const kal_uint8 NVRAM_LAST_CUST_LID;
extern const kal_uint8 NVRAM_LAST_CUSTPACK_LID;
extern kal_uint8 coded_version_lock[];

#ifdef __cplusplus
}
#endif

#endif

