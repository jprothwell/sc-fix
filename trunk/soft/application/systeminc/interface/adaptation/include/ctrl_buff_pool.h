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
 *   ctrl_buff_pool.h
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
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/*********************************************************************
   (C) _____ (year of first publication) Sasken communication
   Technologies Limited, All rights reserved.
*   This file provides a template for .c files. This space
*   should be used to describe the file contents
*   Component-specific prefix : xxxx
*********************************************************************/

#ifndef _CTRL_BUFF_POOL_H
#define _CTRL_BUFF_POOL_H

/*************************************************************************
 * Macros Without Parameters
 *************************************************************************/
#define RPS_CREATED_CTRL_BUFF_POOLS       14

/* Buffer thresholds expressed in percentage */
#define FLC_CTRL_BUFF_UPPER_THRSHLD1 ( 90 )
#define FLC_CTRL_BUFF_UPPER_THRSHLD2 ( 95 )

/*************************************************************************
 * Type Definitions
 *************************************************************************/
/* For performance reason, general buffer pool is allocated for 2^n size */
typedef enum {
   CTRL_BUFF_POOL_SIZE08      = 8,
   CTRL_BUFF_POOL_SIZE16      = 16,
   CTRL_BUFF_POOL_SIZE32      = 32,
   CTRL_BUFF_POOL_SIZE64      = 64,
   CTRL_BUFF_POOL_SIZE128     = 128,
   CTRL_BUFF_POOL_SIZE256     = 256,
   CTRL_BUFF_POOL_SIZE512     = 512,
   CTRL_BUFF_POOL_SIZE1024    = 1024,
   CTRL_BUFF_POOL_SIZE2048    = 2048,
   CTRL_BUFF_POOL_SIZE4096    = 4096,
   CTRL_BUFF_POOL_SIZE8192    = 8192,
   CTRL_BUFF_POOL_SIZE16384   = 16384,
   CTRL_BUFF_POOL_SIZE32768   = 32768,
   CTRL_BUFF_POOL_SIZE65536   = 65536,
   CTRL_BUFF_POOL_MAX_BUFF_SIZE = CTRL_BUFF_POOL_SIZE65536
} ctrl_buff_pool_size;

#if defined(__GSM_MODE__) && defined(__GPRS_MODE__)
/* GPRS Class B Solution */
typedef enum {
   NUM_CTRL_BUFF_POOL_SIZE08   = 85,
   NUM_CTRL_BUFF_POOL_SIZE16   = 85,
   NUM_CTRL_BUFF_POOL_SIZE32   = 85,
   NUM_CTRL_BUFF_POOL_SIZE64   = 85,
   NUM_CTRL_BUFF_POOL_SIZE128  = 61,
   NUM_CTRL_BUFF_POOL_SIZE256  = 50,
   NUM_CTRL_BUFF_POOL_SIZE512  = 22,
   NUM_CTRL_BUFF_POOL_SIZE1024 = 16,
   NUM_CTRL_BUFF_POOL_SIZE2048 = 8,
   NUM_CTRL_BUFF_POOL_SIZE4096 = 0,
   NUM_CTRL_BUFF_POOL_SIZE8192 = 0,
   NUM_CTRL_BUFF_POOL_SIZE16384 =0,
   NUM_CTRL_BUFF_POOL_SIZE32768 =0,
   NUM_CTRL_BUFF_POOL_SIZE60000 =0
} ctrl_num_buff_pool_size;
#elif defined(__GSM_MODE__)
/* XXX!!!. GSM Only Solution will be tuned after integration. */
typedef enum {
   NUM_CTRL_BUFF_POOL_SIZE08   = 50,
   NUM_CTRL_BUFF_POOL_SIZE16   = 50,
   NUM_CTRL_BUFF_POOL_SIZE32   = 50,
   NUM_CTRL_BUFF_POOL_SIZE64   = 30,
   NUM_CTRL_BUFF_POOL_SIZE128  = 41,
   NUM_CTRL_BUFF_POOL_SIZE256  = 30,
   NUM_CTRL_BUFF_POOL_SIZE512  = 22,
   NUM_CTRL_BUFF_POOL_SIZE1024 = 16,
   NUM_CTRL_BUFF_POOL_SIZE2048 = 8,
   NUM_CTRL_BUFF_POOL_SIZE4096 = 0,
   NUM_CTRL_BUFF_POOL_SIZE8192 = 0,
   NUM_CTRL_BUFF_POOL_SIZE16384 =0,
   NUM_CTRL_BUFF_POOL_SIZE32768 =0,
   NUM_CTRL_BUFF_POOL_SIZE60000 =0
} ctrl_num_buff_pool_size;
#endif

/*************************************************************************
 * Exported Global Variables
 *************************************************************************/
extern buff_pool_info_struct ctrl_buff_pool_info_g[ RPS_CREATED_CTRL_BUFF_POOLS];

/*************************************************************************
 * Exported Function Prototypes
 *************************************************************************/
extern void stack_init_ctrl_buff_pool_info( void );
extern kal_bool stack_create_ctrl_buff_pools(void);
extern kal_poolid get_ctrl_pool_id_from_buff_size(kal_uint16 buff_size);
#endif /* _CTRL_BUFF_POOL_H */


