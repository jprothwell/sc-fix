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
 *   This files defines macros to get/release data buffer from pool
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

#ifndef _DATA_BUFF_POOL_H
#define _DATA_BUFF_POOL_H

/*******************************************************************************
 * Macros With Parameters
 *******************************************************************************/
#define DATA_BUFF_POOL_SIZE_RLCBLK     		(56 + 8)    /* add rlc dl header length */
/* #define NUM_DATA_BUFF_UPLINK_SIZE_RLCBLK 	72 */
#define NUM_DATA_BUFF_DOWNLINK_SIZE_RLCBLK 	127

typedef enum {
   RLC_DATA_UP,
   RLC_DATA_DOWN,
   RLC_DATA_INVALID_DIR
} rlc_data_dir;

extern void kal_release_data_buffer_r(void * buff_pt);
   
#if defined(DEBUG_KAL) && defined(DEBUG_BUF)

#define get_data_buffer(size, dir) get_int_data_buffer(size, dir, __FILE__, __LINE__)

#ifdef CSD_KAL_MNT
#define free_data_buffer(ptr) FREE_BUFFER(ptr, __FILE__, __LINE__)   
#else   /* CSD_KAL_MNT */
#define free_data_buffer(ptr) kal_release_data_buffer_r(ptr)
#endif   /* CSD_KAL_MNT */

#else   /* DEBUG_KAL && DEBUG_BUF */

#define get_data_buffer(size, dir) get_int_data_buffer(size, dir, NULL, 0)

#ifdef CSD_KAL_MNT
#define free_data_buffer(ptr) FREE_BUFFER(ptr, __FILE__, __LINE__)
#else   /* CSD_KAL_MNT */
#define free_data_buffer(ptr) kal_release_data_buffer_r(ptr)   
#endif   /* CSD_KAL_MNT */

#endif   /* DEBUG_KAL && DEBUG_BUF */

/*******************************************************************************
 * Function Declaration
 *******************************************************************************/
void stack_init_data_buff_pool_info(void);

extern kal_bool stack_create_data_buff_pools(void);

void* get_int_data_buffer(kal_uint16 buff_size, rlc_data_dir dir, kal_char* file_name, kal_uint32 line);

#endif /* _DATA_BUFF_POOL_H */


