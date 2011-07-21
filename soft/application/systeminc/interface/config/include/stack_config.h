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
 *   stack_config.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file provides the task index and module index configuration.
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
 *
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

#ifndef _STACK_CONFIG_H
#define _STACK_CONFIG_H


/*************************************************************************
* Steps to add component task
*
* DESCRIPTION
*   The file describes steps to add component task.
*
*  1. add component task's index (Please add before system service)
*
*  2. add component task priority (If proirity not defined betweek KAL range, please
*     use Nucleus Plus 3~255, priority 0 is the highest priority, 255 is the lowest.
*     0~3 is reserved to HISR)
*
*  3. add component task's module id definition (Please add before system service)
*
*  4. add module to task transformation in syscomp_config.c
*
*  5. add and implement component task's create handler
*
* GLOBALS AFFECTED
*
*************************************************************************/

/*************************************************************************
 * 1. Component Task Index (Please add before system service)
 *************************************************************************/
typedef enum {
   INDEX_NIL =   0x00,
   INDEX_MMI,
   INDEX_ADP,
   INDEX_MED,
   INDEX_FMT,
   INDEX_REC,
   INDEX_TP,
   INDEX_TRANSPORT,
   TOTAL_TASKS
} task_indx_type;

/*************************************************************************
 * 2. Component task's priority and stack size[optional]
 *************************************************************************/

#define MMI_TASKS_PRIORITY_BASE         230


// MMI层几个Task的stack大小定义.
// TODO: 要根据SA给出的实际运行分析来调整一下具体的值.
#define MMI_TASK_STACK_SIZE		        4096	// 4k
#define ADP_TASK_STACK_SIZE		        2048	// 2k
#define MMI_FSTASK_SIZE                 2048	// 2k
#define MMI_USBTASK_SIZE                 2048	// 2k
#define MMI_MED_TASK_STACK_SIZE	        2048	// 2k
#define TP_TASK_STACK_SIZE		        2048	// 2k
#define TRANSPORT_TASK_STACK_SIZE       10240   // 10k
#define MMI_BTTASK_SIZE         4096	// 4k
#ifdef __J2ME__
#define JAVA_TASK_STACK_SIZE		2048	// 2k
#endif

//#define MMI_BTTASK_PRIORITY		179


// MMI层的几个Task的优先级定义.
#define TP_TASK_PRIORITY		        (MMI_TASKS_PRIORITY_BASE+0)
#define ADP_TASK_PRIORITY		        (MMI_TASKS_PRIORITY_BASE+1)

#define MMI_MED_TASK_PRIORITY	        (MMI_TASKS_PRIORITY_BASE+3)
#define MMI_TASK_PRIORITY		        (MMI_TASKS_PRIORITY_BASE+5)
#define MMI_BTTASK_PRIORITY		(MMI_TASKS_PRIORITY_BASE+4)

#define JAT_TRANSPORTTASK_PRIORITY		(MMI_TASKS_PRIORITY_BASE+6)
#define MMI_FSTASK_PRIORITY		(MMI_TASKS_PRIORITY_BASE+7)
#define MMI_USBTASK_PRIORITY		(MMI_TASKS_PRIORITY_BASE+8)
#ifdef __J2ME__
#define JAVA_TASK_PRIORITY		(MMI_TASKS_PRIORITY_BASE+9)
#endif



/* End of customizable tasks */

/*************************************************************************
 * 3. Component task's module id (Please add before system service)
 *************************************************************************/
typedef enum {
   MOD_NIL = 0,
   MOD_NVRAM,
   MOD_ADP ,
   MOD_L4C = MOD_ADP,
   MOD_L4C_2,
   MOD_L4C_3,
   MOD_L4C_4,
   MOD_MED,
   MOD_MED_V,
   MOD_MMI,
   MOD_FMT,
   MOD_TP_TASK,
   MOD_TRANSPORT,
   MOD_J2ME,
   MOD_L1SP,
   MOD_SOC,
   MOD_WAP,
   MOD_JASYN,
   MOD_UEM,
   MOD_AS,
   MOD_VISUAL_HISR,
   MOD_JPEG_HISR,
   MOD_TST_READER,
   TOTAL_MODULES,
   MOD_BT,
   MOD_USB,
   MOD_UART1_HISR,
   MOD_UART2_HISR,
   MOD_UART3_HISR

} module_type;

#endif /* _STACK_CONFIG_H */


