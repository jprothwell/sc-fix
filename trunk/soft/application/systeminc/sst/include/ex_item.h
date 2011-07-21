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
 *   ex_item.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	Header file for exception handling
 *
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef _EX_ITEM_H
#define _EX_ITEM_H

/*******************************************************************************
 * Constant Definition
 *******************************************************************************/
/*
 * Existed exception record count, current is 22 ()
 *    Current is valid word, lr, sp, and current thread pointer
 */
#define EXIST_EXPTR_COUNT        	25   /* Existed exception size count in long */
#define STACK_EXPTR_COUNT        	60   /* Code stack dump size in long */
#define SYS_EXPTR_COUNT				   2    /* System information */
#define EXTENSION_INFO              40   /* Extra 40 long for extension exception INFO */

/* 508B totally, (25+60+4+38 */
#define TOTAL_EXPTR_COUNT				(EXIST_EXPTR_COUNT+STACK_EXPTR_COUNT+SYS_EXPTR_COUNT+EXTENSION_INFO)

#define TOTAL_EXPTR_SIZE				(TOTAL_EXPTR_COUNT*sizeof(long)/sizeof(char))

#ifdef EXCEPTION_TEST
/* Unit Test */
   #define EXCPT_MAGIC					0x1234   	/* Exception record magic number */
   #define FDM_SECTOR_SIZE				(64*1024)	/* FDM sector size = 64K bytes */
   #define DEF_EXCPT_FLASH_ADR		0x00280000
   #define DEF_EXCPT_HDR_SIZE			3
   #define MAX_EXPTR_RECORD_COUNT	((FDM_SECTOR_SIZE-DEF_EXCPT_HDR_SIZE)/TOTAL_EXPTR_SIZE)
#endif /* EXCEPTION_TEST */

#define EX_EXT_INFO_POS             87
#define EX_TASK_INFO_POS(i)         (EX_EXT_INFO_POS + (i<<3))
#define EX_TASK_GP_POS              0
#define EX_TASK_STATUS_POS          2
#define EX_TASK_INT_Q_POS           4
#define EX_TASK_EXT_Q_POS           6

#define EX_MAX_TASK_DUMP            4
#define EX_PAD_1							2
#define EX_PAD_2							3

#define EX_FORCEMEMORYDUMP          0x26409001

extern kal_uint32 saved_irq_mask;

/*******************************************************************************
 * Constant Definition
 *******************************************************************************/
#if 0 /* Exception record string table - For explaination */
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

/* Duplicate structure definition to meet different platform requirement */
typedef struct ex_rtc_struct_t {
   kal_uint8      rtc_sec;    /* seconds after the minute   - [0,59]  */
   kal_uint8      rtc_min;    /* minutes after the hour     - [0,59]  */
   kal_uint8      rtc_hour;   /* hours after the midnight   - [0,23]  */
   kal_uint8      rtc_day;    /* day of the month           - [1,31]  */
   kal_uint8      rtc_mon;    /* months 		               - [1,12]  */
   kal_uint8      rtc_wday;   /* days in a week 		      - [1,7]   */
   kal_uint8      rtc_year;   /* years                      - [0,127] */
   kal_uint8      rtc_pad;    /* Padding */
} ex_rtc_struct;

typedef struct ex_frametime_sruct_t {
   kal_int32      frame;      /* @field Frame number */
   kal_int32      ebits;      /* @field Eighth bits  */
} ex_frametime_sruct;

typedef struct ex_ctrlbuf_his_t {
   kal_char       ex_his_owner[8];    /* control buffer owner */
   kal_char       ex_his_source[16];  /* source file */
   kal_uint32     ex_his_line;        /* line number */
   kal_uint32     ex_his_count;       /* number of identical entries */
} ex_ctrlbuf_his;

typedef struct ex_ctrlbuf_common_t {
   kal_uint32     ex_buf_NU_header1;  /* NUCLEUS overhead 1, 0: allocated, else next pointer */
   kal_uint32     ex_buf_NU_header2;  /* NUCLEUS overhead 2, pointer to its control block */
   kal_uint32     ex_buf_KAL_header1; /* KAL overhead 1, header (0xF1F1F1F1) */
   kal_uint32     ex_buf_KAL_header2; /* KAL overhead 2, task ID */
   kal_uint32     ex_buf_KAL_header3; /* KAL overhead 3, pointer to its control block) */
   kal_uint32     ex_buf_poolID;      /* Buffer pointer */
   kal_uint32     ex_buf_KAL_footer1; /* KAL footer: 0xF2F2F2F2 */
   kal_uint32     ex_buf_KAL_footer2; /* KAL footer appended after size requested */        
} ex_ctrlbuf_common;

typedef struct ex_ext_owner_t {
   kal_char       ex_buf_source[12];  /* Source file name */
   kal_uint32     ex_buf_line;        /* line number */
} ex_ext_owner;

typedef union {
   ex_ctrlbuf_his     history_t;
   ex_ctrlbuf_common  common;
} ex_ctrl_buf;

typedef struct ex_ctrlbuf_struct_t {
   kal_uint32      ex_ctrlbuf_size;    /* control buffer size per entry */
   kal_uint32      ex_ctrlbuf_num;     /* total number of entries */
   ex_ctrl_buf     ex_ctrlbuf_top;     /* top occupation history node */
   ex_ctrl_buf     ex_ctrlbuf_second;  /* second occupation history node */
   ex_ctrl_buf     ex_ctrlbuf_third;   /* third occupation history node */
   ex_ext_owner    ex_monitor[3];
   kal_uint32      ex_reserved[2];    /* reserved */
} ex_ctrlbuf_struct;

typedef struct ex_queue_stuct_t {
   kal_uint8      ex_q_src_mod;        /* source module ID */
   kal_uint8      ex_q_count;          /* total number of identical message */
   kal_uint16     ex_q_msg_id;         /* message ID */
   kal_uint16     ex_q_cur_mes_no;     /* tatal number of messages left in queue */
   kal_uint16     ex_q_config_entry;   /* total number of entries */
} ex_queue_struct;

typedef struct ex_task_struct_t {
   kal_char        ex_task_name[8];       /* task name */
   kal_char        ex_task_stack_gp[8];   /* guard pattern:STACK_END */
   kal_uint32      ex_task_cur_status;    /* task current status, eg. RUNNING, READY etc */
   ex_queue_struct ex_task_external_q;    /* task external queue */
   ex_queue_struct ex_task_internal_q;    /* task internal queue */
   kal_uint32      ex_reserved;           /* reserved */
} ex_task_struct;

typedef struct ex_exception_record_header_t {
   exception_type       ex_type;                /* 0: Exception Type */
   kal_uint8   ex_nvram;                        /* 0-1: NVRAM Reserved */
   kal_uint16  ex_serial_num;
} ex_exception_record_header;

/* Undefine exception(1), SWI(2), Prefetch exception(3), Data abort(4), System lockup(8) */
typedef struct ex_exception_record_t {
   ex_exception_record_header ex_header; 

   kal_uint32  ex_a1;                           /* 1: Argument 1 */
   kal_uint32  ex_a2;                           /* 2: Argument 2 */
   kal_uint32  ex_a3;                           /* 3: Argument 3 */
   kal_uint32  ex_a4;                           /* 4: Argument 4 */
   kal_uint32  ex_v1;                           /* 5: Register Variable 1 */
   kal_uint32  ex_v2;                           /* 6: Register Variable 2 */
   kal_uint32  ex_v3;                           /* 7: Register Variable 3 */
   kal_uint32  ex_v4;                           /* 8: Register Variable 4 */
   kal_uint32  ex_v5;                           /* 9: Register Variable 5 */
   kal_uint32  ex_sb;                           /* 10: Stack Base */
   kal_uint32  ex_sl;                           /* 11: Stack Limit */
   kal_uint32  ex_fp;                           /* 12: Frame Pointer */
   kal_uint32  ex_ip;                           /* 13: New SB in inter-link-unit calls */
   kal_uint32  ex_sp;                           /* 14: Lower end of current stack frame */
   kal_uint32  ex_lr;                           /* 15: Link address */
   kal_uint32  ex_cpsr;                         /* 16: CPSR */
   kal_uint32  ex_curthread_id;                 /* 17-20: Current thread's id(1), name(2), priority(1)*/
   kal_char    ex_cur_thread_name[8];
   kal_uint8   ex_cur_thread_status;
   kal_uint8   ex_pad2[EX_PAD_2];
   ex_rtc_struct        ex_systime;             /* 21-22: System time */
   ex_frametime_sruct   ex_frametime;           /* 23-24: Frame time */
   kal_uint32  ex_stackdump[60];                /* 25-84: Code Stack dump */
   kal_uint32  ex_proc_lisr;                    /* 85: Processing LISR */
   kal_uint32  ex_irqmask;                      /* 86: Interrupt Mask */
   ex_task_struct       ex_task_info[EX_MAX_TASK_DUMP];  /* 87-126: extension task info */
} ex_exception_record;


/* Assertion fail(5) */
typedef struct ex_assertfail_record_t {
   ex_exception_record_header ex_header; 

   kal_uint32  ex_a1;                           /* 1: Argument 1 */
   kal_uint32  ex_a2;                           /* 2: Argument 2 */
   kal_uint32  ex_a3;                           /* 3: Argument 3 */
   kal_uint32  ex_a4;                           /* 4: Argument 4 */
   kal_char    ex_filename[12];                 /* 5: Assert failed filename */
   kal_uint32  ex_line;                         /* 6: Assert failed line */
   kal_uint32  ex_extension;                    /* 9: Extension info (disable/enable)*/
   kal_uint32  ex_ext1;                         /* 10: Extension info 1 */
   kal_uint32  ex_ext2;                         /* 11: Extension info 2 */
   kal_uint32  ex_ext3;                         /* 12: Extension info 3 */
   kal_uint32  ex_ip;                           /* 13: New SB in inter-link-unit calls */
   kal_uint32  ex_sp;                           /* 14: Lower end of current stack frame */
   kal_uint32  ex_lr;                           /* 15: Link address */
   kal_uint32  ex_cpsr;                         /* 16: CPSR */
   kal_uint32  ex_curthread_id;                 /* 17-20: Current thread's id(1), name(2), priority(1)*/
   kal_char    ex_cur_thread_name[8];
   kal_uint8   ex_cur_thread_status;
   kal_uint8   ex_pad2[EX_PAD_2];
   ex_rtc_struct        ex_systime;             /* 21-22: System time */
   ex_frametime_sruct   ex_frametime;           /* 23-24: Frame time */
   kal_uint32  ex_stackdump[60];                /* 25-84: Code Stack dump */
   kal_uint32  ex_proc_lisr;                    /* 85: Processing LISR */
   kal_uint32  ex_irqmask;                      /* 86: Interrupt Mask */
   ex_task_struct       ex_task_info[EX_MAX_TASK_DUMP];  /* 87-126: extension task info */
} ex_assertfail_record;


/* Stack fatal error(6) - Tak Information */
typedef struct ex_stackfatalerr_task_record_t {
   ex_exception_record_header ex_header; 
   
   kal_uint32  ex_a1;                           /* 1: Argument 1 */
   kal_uint32  ex_a2;                           /* 2: Argument 2 */
   kal_uint32  ex_a3;                           /* 3: Argument 3 */
   kal_uint32  ex_a4;                           /* 4: Argument 4 */
   kal_uint32  ex_error_code1;                  /* 5: Fatal error code1 */
   kal_uint32  ex_error_code2;                  /* 6: Fatal error code2 */
   kal_uint32  ex_error_code3;                  /* 7: Fatal error code3 */
   kal_uint32  ex_error_code4;                  /* 8: Fatal error code4 */
   kal_uint32  ex_error_code5;                  /* 9: Fatal error code5 */
   kal_uint32  ex_sb;                           /* 10: Stack Base */
   kal_uint32  ex_sl;                           /* 11: Stack Limit */
   kal_uint32  ex_fp;                           /* 12: Frame Pointer */
   kal_uint32  ex_ip;                           /* 13: New SB in inter-link-unit calls */
   kal_uint32  ex_sp;                           /* 14: Lower end of current stack frame */
   kal_uint32  ex_lr;                           /* 15: Link address */
   kal_uint32  ex_cpsr;                         /* 16: CPSR */
   kal_uint32  ex_curthread_id;                 /* 17-20: Current thread's id(1), name(2), priority(1)*/
   kal_char    ex_cur_thread_name[8];
   kal_uint8   ex_cur_thread_status;
   kal_uint8   ex_pad2[EX_PAD_2];
   ex_rtc_struct        ex_systime;             /* 21-22: System time */
   ex_frametime_sruct   ex_frametime;           /* 23-24: Frame time */
   kal_uint32  ex_stackdump[60];                /* 25-84: Code Stack dump */
   kal_uint32  ex_proc_lisr;                    /* 85: Processing LISR */
   kal_uint32  ex_irqmask;                      /* 86: Interrupt Mask */
   ex_task_struct       ex_task_info[EX_MAX_TASK_DUMP];  /* 87-126: extension task info */
} ex_stackfatalerr_task_record;

/* Stack fatal error(7) - Control Buffer Pool */
typedef struct ex_stackfatalerr_ctrlbuf_record_t {
   ex_exception_record_header ex_header; 
   
   kal_uint32  ex_a1;                           /* 1: Argument 1 */
   kal_uint32  ex_a2;                           /* 2: Argument 2 */
   kal_uint32  ex_a3;                           /* 3: Argument 3 */
   kal_uint32  ex_a4;                           /* 4: Argument 4 */
   kal_uint32  ex_error_code1;                  /* 5: Fatal error code1 */
   kal_uint32  ex_error_code2;                  /* 6: Fatal error code2 */
   kal_uint32  ex_error_code3;                  /* 7: Fatal error code3 */
   kal_uint32  ex_error_code4;                  /* 8: Fatal error code4 */
   kal_uint32  ex_error_code5;                  /* 9: Fatal error code5 */
   kal_uint32  ex_sb;                           /* 10: Stack Base */
   kal_uint32  ex_sl;                           /* 11: Stack Limit */
   kal_uint32  ex_fp;                           /* 12: Frame Pointer */
   kal_uint32  ex_ip;                           /* 13: New SB in inter-link-unit calls */
   kal_uint32  ex_sp;                           /* 14: Lower end of current stack frame */
   kal_uint32  ex_lr;                           /* 15: Link address */
   kal_uint32  ex_cpsr;                         /* 16: CPSR */
   kal_uint32  ex_curthread_id;                 /* 17-20: Current thread's id(1), name(2), priority(1)*/
   kal_char    ex_cur_thread_name[8];
   kal_uint8   ex_cur_thread_status;
   kal_uint8   ex_pad2[EX_PAD_2];
   ex_rtc_struct        ex_systime;             /* 21-22: System time */
   ex_frametime_sruct   ex_frametime;           /* 23-24: Frame time */
   kal_uint32  ex_stackdump[60];                /* 25-84: Code Stack dump */
   kal_uint32  ex_proc_lisr;                    /* 85: Processing LISR */
   kal_uint32  ex_irqmask;                      /* 86: Interrupt Mask */
   ex_ctrlbuf_struct   ex_ctrl_buf;             /* 87-126: extension task info */
} ex_stackfatalerr_ctrlbuf_record;


extern ex_exception_record *saved_expr_record_ptr;
extern kal_uint32  INT_MemoryDumpFlag;

#endif /* _EX_ITEM_H */


