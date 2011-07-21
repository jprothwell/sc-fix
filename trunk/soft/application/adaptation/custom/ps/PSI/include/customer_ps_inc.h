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
 * customer_ps_inc.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for include file of PS customization parameters. 
 *
 * Author:
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef _CUSTOMER_PS_INC_H
#define _CUSTOMER_PS_INC_H

/* For customer define SMS message box number. */
/* This parameter means total number of SMS stored in SIM and NVRAM can be read.  */


/* In later branch, each entry costs (46 + 2* MMI_SMS_MAX_MSG_SEG) bytes.  */
// 这个宏定义了收、发件箱存储短信总数。我们CSW目前支持的SIM短信存储最大数为250，
// ME存储最大数为50。因此原来的值太小了，需要修改成>=300。这里320打了个20的裕量。
//defined in mmi_config.h
//#define CM_SMS_MAX_MSG_NUM    470//wangzl 070914 modify from 320,当前ME存储最大数为200

/* This value is to define the maximun segment number of EMS short message. */
#ifdef __EMS_REL5__
#define MMI_SMS_MAX_MSG_SEG 8
#else
#define MMI_SMS_MAX_MSG_SEG 4
#endif


/* From 3GPP 23.040, if __EMS_REL5__ is NOT turned ON, segment number shall be  */
/* larger than 8 segments.                                                      */
#ifdef __EMS_REL5__
#if MMI_SMS_MAX_MSG_SEG < 8
#error segment number shall be larger than 8 for EMS 5.0
#endif
 
#endif

/* The value defines the buffer size for EMS objects, such as pre-defined animation. */
/* The more of buffer size would support more objects numbers.                       */
#define EMS_OBJ_BUFF_SIZE_PER_SEG  300

/* For EMS 5.1, the buffer for raw data of objects is separated to another memory pool. */
/* The pool size is (MMI_SMS_MAX_MSG_SEG * 140 * EMS_RATIO_OF_OBJ_PDU_BUFF).        */
/* 140 is the max size of a SMS                                                           */

/* The more of EMS_RATIO_OF_OBJ_PDU_BUFF value will support    */
/* the larger objects which is compressed in higher compression ratio.   */

/* The ratio can be a float value, for example, 1.5, 2.5 */

#ifdef __EMS_REL5__
#define EMS_RATIO_OF_OBJ_PDU_BUFF 2 
#endif


/* TOTAL memory cost in EMS library : */
/* If __EMS_REL5__ is NOT turned ON : */
/* memory costs = 2 * CM_SMS_MAX_MSG_NUM * (EMS_OBJ_BUFF_SIZE_PER_SEG + 310) bytes */

/* If __EMS_REL5__ is turned ON : */
/* memory costs = 2 * CM_SMS_MAX_MSG_NUM * (EMS_OBJ_BUFF_SIZE_PER_SEG + 310 +          */
/*                                          140 *EMS_RATIO_OF_OBJ_PDU_BUFF) bytes */

/* ps: 310 bytes per segment are for text string buffer */


/* display when concatenated  segment missed */
/* for example: a 3-segment concatenated message, but 2nd segment misses. */
/* MMI would show :   "content of seg1(.....)content of seg3 " */
/* Note :DO NOT use the UCS2 coding string . */
#define EMS_MISS_SEG_DELIMIT_STR   "(.....)"   


/* ********************************************************************************* */
/* For customer define E-Mail message box number. */
#define EMAIL_MAX_MSG_NUM (50)

#define MMI_EMAIL_MAX_MSG_SIZE              (100 * 1024)  /* Maximum message size */
#define MMI_EMAIL_MIN_MSG_SIZE              (0.5 * 1024)
#define MMI_EMAIL_MAX_ATTACH_SIZE           (30 * 1024) /* Maximum attachment size */
#define MMI_EMAIL_TOTAL_MAX_ATTACH_SIZE     (90 * 1024)

/* =========================================================================== */

#endif /* _CUSTOMER_PS_INC_H */

