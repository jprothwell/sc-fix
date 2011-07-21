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
 *	phb_defs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file contains common definitions for PHB module.
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
 *******************************************************************************/
#ifndef _PHB_DEFS_H
#define _PHB_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Header files should be include before this one */
#ifndef _KAL_RELEASE_H
#error "kal_release.h should be included"
#endif

#if (MAX_PHB_LN_ENTRY > 10) && (MAX_PHB_LN_ENTRY != 20) && \
	(MAX_PHB_LN_ENTRY != 30) && (MAX_PHB_LN_ENTRY != 40) && \
	(MAX_PHB_LN_ENTRY != 50) 
#error PHB_LN_ENTRY should be set to 20, 30, 40, 50 or less and equal than 10
#endif

#if (MAX_PHB_LN_ENTRY > 20)
#ifndef __PHB_RECORD_LN_TYPE_SEQ__
#define __PHB_RECORD_LN_TYPE_SEQ__
#endif
#ifndef __PHB_LN_SINGLE_COUNT__
#define __PHB_LN_SINGLE_COUNT__
#endif 
#endif

#define PHB_INVALID_VALUE              ~0
#define PHB_NULL_VALUE                 0

#define BYTE_SIZE                      8
#define BYTE_SIZE_SHIFT_ARITHMETIC     3

 
#define PHB_MAX_PHB_ENTRIES            15

/* Number of LN entries in one NVRAM LID */
#if (MAX_PHB_LN_ENTRY >= 10)
#define PHB_MAX_LN_ENTRIES             10
#else
#define PHB_MAX_LN_ENTRIES              MAX_PHB_LN_ENTRY
#endif /* MAX_PHB_LN_ENTRY >= 10 */

#ifdef __L4_MAX_NAME_60__
#define PHB_MAX_LND_ENTRIES            15
#define PHB_MAX_LNM_ENTRIES            15
#define PHB_MAX_LNR_ENTRIES            15
#else
#define PHB_MAX_LND_ENTRIES            MAX_PHB_LN_ENTRY
#define PHB_MAX_LNM_ENTRIES            MAX_PHB_LN_ENTRY
#define PHB_MAX_LNR_ENTRIES            MAX_PHB_LN_ENTRY
#endif

#ifdef __PHB_RECORD_LN_TYPE_SEQ__
#define PHB_TOTAL_LN_ENTRIES           (PHB_MAX_LND_ENTRIES+PHB_MAX_LNM_ENTRIES+PHB_MAX_LNR_ENTRIES)
#endif /* __PHB_RECORD_LN_TYPE_SEQ__ */

#define PHB_TOT_LND_LIDS      (PHB_MAX_LND_ENTRIES+PHB_MAX_LN_ENTRIES-1)/PHB_MAX_LN_ENTRIES
#define PHB_TOT_LNM_LIDS      (PHB_MAX_LNM_ENTRIES+PHB_MAX_LN_ENTRIES-1)/PHB_MAX_LN_ENTRIES
#define PHB_TOT_LNR_LIDS      (PHB_MAX_LNR_ENTRIES+PHB_MAX_LN_ENTRIES-1)/PHB_MAX_LN_ENTRIES

#define PHB_LND_LID_START     1
#define PHB_LNM_LID_START     PHB_TOT_LND_LIDS + PHB_LND_LID_START
#define PHB_LNR_LID_START     PHB_TOT_LNM_LIDS + PHB_LNM_LID_START
#define PHB_TOTAL_LN_LID      PHB_TOT_LND_LIDS+PHB_TOT_LNM_LIDS+PHB_TOT_LNR_LIDS

/* Concurrency degree is the number of control blocks. */
#define PHB_CONCURRENCY_DEGREE         2

/* In byte(s). Must not exceeds the capacity defined by l4_common_struct.h. */
 
#if defined(__MMI_FMI__)
#define PHB_ALPHA_ID_DEPTH             1
#else
#define PHB_ALPHA_ID_DEPTH             10
#endif

/* Number of bytes after ALPHA_ID field */
#define PHB_LENGTH_AFTER_ALPHA_ID      14

/* Byte size of TON/NPI allocated in NVRAM/SIM */
#define TON_NPI_SIZE 1

/**
 * Maximum bytes of TON/NPI and DIALLING_NUMBER/SSC_STRING 
 * fields allocated for ADN-like EF's
 */
#define PHB_EF_MAX_TON_NPI_TEL_NUMBER_LEN       11

/**
 * Maximum bytes of DIALLING_NUMBER/SSC_STRING field allocated
 * for EXT-like EF's.
 */
#define PHB_EF_EXT_MAX_TEL_NUMBER_LEN      10

/* TON and NPI constants */
#define NPI_UNKNOWN                    0
#define NPI_ISDN                       1
/** 
 * Most significant bit is always 1 for TON. ie, 1???. 
 * Hence, 
 * TON_UNKNOW is 1000, TON_NATIONAL is 1010, and 
 * TON_INTERNATIONAL is 1001
 */
#define TON_EXT                        0x08
#define TON_UNKNOWN                    0x00
#define TON_INTERNATIONAL              0x01
#define TON_NATIONAL                   0x02
#define TON_NETWORK_SPECIFIC           0x03
#define TON_DEDICATED                  0x04

/* Default TON/NPI */
#define TON_NPI_DEFAULT    (((TON_EXT | TON_UNKNOWN) << 4) | NPI_ISDN)

#define FETCH_TON(x)       ((x >> 4) & 0x07)

/* This is reserved for possible revision of l4_addr_bcd_struct */
#define L4_ADDR_BCD_TON_NPI_OFFSET     0
#define L4_ADDR_BCD_TON_NPI_SIZE       1
#define L4_ADDR_BCD_TEL_NUM_OFFSET     1

/* Valid digits are 0x00 ~ 0x0C; ie, before p; ranges from 0 ~ *, #. */
#define DN_VALID_DIGIT_LIMIT           0x0C

typedef enum
{
    IO_NONE,
    IO_WAIT,
    IO_COMPLETE,
    IO_SEGMENTATION,
    IO_REASSEMBLY
} IO_state_enum;

typedef enum 
{
    OP_READ,
    OP_WRITE,
    OP_DELETE
} op_enum;

typedef struct
{
   kal_uint8 length;
   kal_uint8 charset;
   kal_uint8 data[PHB_ALPHA_ID_DEPTH];
} alpha_id_type;


#define EF_TEL_NUMBER_SIZE 10
/* Stored phone book entries structure */
typedef struct 
{
   kal_uint8 len;
   kal_uint8 ton_npi;
   kal_uint8 tel_number[EF_TEL_NUMBER_SIZE];
   kal_uint8 cap_cnf_id;
   kal_uint8 ext_id;       /* Should not be used for nvram_access. */
   /* Note that BDN has 1 byte more: comparison method ID */
} phb_ef_mandatory_struct;

typedef struct
{
   kal_uint8* alpha_id;
   phb_ef_mandatory_struct* mandatory;
} phb_ef_struct;


#ifdef __cplusplus
}
#endif

#endif

