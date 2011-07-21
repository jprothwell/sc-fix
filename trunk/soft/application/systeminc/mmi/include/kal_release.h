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
 *   kal_release.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file has to be included for accessing the target version
 *   functionalities of KAL.
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

#ifndef _KAL_RELEASE_H
#define _KAL_RELEASE_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */ 

/*************************************************************************
 * Include Statements
 *************************************************************************/
// Get types definition 
//#include "cswtype.h"
#include "mmi_data_types.h"
#ifdef KAL_ON_NUCLEUS
#include "nucleus.h"
#endif /* KAL_ON_NUCLEUS */

#include "stack_config.h"
#include "kal_non_specific_general_types.h"


#include "kal_common_defs.h"
//#include "kal_internal_macros.h"
#include "string.h"

/* To separate from l1 simuatlion environment */
#if defined(CSD_TARGET) || defined (CSD_KAL_MNT)
/*************************************************************************
 * Constant definition and check
 *************************************************************************/
#if 0
	#if (defined( DEBUG_KAL) && defined( RELEASE_KAL ) )
		#error "Only one of DEBUG_KAL and RELEASE_KAL can be defined"
	#endif

	#if (!defined( DEBUG_KAL) && !defined( RELEASE_KAL ))
		#error "At least one of DEBUG_KAL or RELEASE_KAL should be defined"
	#endif
#endif 

/*************************************************************************
 * Special definition applied on MNT only
 *************************************************************************/
#if defined( CSD_KAL_MNT )
#ifdef __GNUC__
#ifndef __int64
#define __int64 long long
#endif /* __int64 */
#ifndef __inline
#define __inline static
#endif /* __inline */
#endif /* __GNUC__ */
#endif /* CSD_KAL_MNT */

/*************************************************************************
 * Dat type Definitions
 *************************************************************************/
#if 0
	#if defined(KAL_ON_NUCLEUS)
typedef enum {
	KAL_PRIORITY_CLASS0        =     0,
	KAL_PRIORITY_CLASS1        =     10,
	KAL_PRIORITY_CLASS2        =     20,
	KAL_PRIORITY_CLASS3        =     30,
	KAL_PRIORITY_CLASS4        =     40,
	KAL_PRIORITY_CLASS5        =     50,
	KAL_PRIORITY_CLASS6        =     60,
	KAL_PRIORITY_CLASS7        =     70,
	KAL_PRIORITY_CLASS8        =     80,
	KAL_PRIORITY_CLASS9        =     90,
	KAL_PRIORITY_CLASS10       =     100,
	KAL_PRIORITY_CLASS11       =     110,
	KAL_PRIORITY_CLASS12       =     120,
	KAL_PRIORITY_CLASS13       =     130,
	KAL_PRIORITY_CLASS14       =     140,
	KAL_PRIORITY_CLASS15       =     150,
	KAL_PRIORITY_CLASS16       =     160,
	KAL_PRIORITY_CLASS17       =     170,
	KAL_PRIORITY_CLASS18       =     180,
	KAL_PRIORITY_CLASS19       =     190,
	KAL_PRIORITY_CLASS20       =     200,
	KAL_PRIORITY_CLASS21       =     210,
	KAL_PRIORITY_CLASS22       =     220,
	KAL_PRIORITY_CLASS23       =     230,
	KAL_PRIORITY_CLASS24       =     240,
	KAL_PRIORITY_CLASS25       =     250
} kal_priority_type;
	#endif /* KAL_ON_NUCLEUS */
#endif 

/* Types used by the task management */
typedef void* kal_taskid;

typedef void* kal_hisrid;

/* Types used by ITC */
typedef struct {
	kal_uint8   unused;
} *kal_semid;

typedef struct {
	kal_uint8   unused;
}* kal_msgqid;

typedef struct {
	kal_uint32 pending_msgs;	/* number of pending messages in the queue */
	kal_uint32 max_msgs;			/* number of maximum messages in the queue (queue length) */
} kal_msgq_info;

typedef struct {
	kal_uint8   unused;
} *kal_mutexid;

typedef struct {
	kal_uint8   unused;
} *kal_eventgrpid;

/* types used for buffer manager */
typedef struct {
	kal_uint8   unused;
} *kal_poolid;

/* Types used by the timer management */
typedef struct {
	kal_uint8 unused;
}* kal_timerid;

/* type used by application memory management */
typedef void* KAL_ADM_ID;

#if 0
/*************************************************************************
 * Constants
 *************************************************************************/
	#define KAL_NILTASK_ID        (kal_taskid)NULL
	#define KAL_NILHISR_ID        (kal_hisrid)NULL
	#define KAL_NILTIMER_ID       (kal_timerid)NULL
	#define KAL_NILPOOL_ID        (kal_poolid)NULL
	#define KAL_NILMUTEX_ID       (kal_mutexid)NULL
	#define KAL_NILEVENTGRP_ID    (kal_eventgrpid)NULL
	#define KAL_NILSEM_ID         (kal_semid)NULL
	#define KAL_NILQ_ID           (kal_msgqid)NULL

/* Impossible time: sec, and msec */
	#define KAL_NILSEC            (kal_uint32)0xffffffff
	#define KAL_NILMSEC           (kal_uint16)0xffff
	#define KAL_NILTICK           (kal_uint32)0xffffffff
#endif 

/*************************************************************************
 * Kal Initialize
 *    These routines are to be called during the initialization phase.
 * Call the following three routines in the very beginning.
 *************************************************************************/

/* Initializes the kal layer. It should be called during initialization phase */
void kal_initialize(void);

/*
 * Initializes kal for the Memory requirement. It should be called
 * during initialization phase
 */
void kal_initialize_mem(void* start_address_ptr);

/* Query system stage */
kal_bool kal_query_systemInit(void);

/*************************************************************************
 * Task Management
 *    This routine Creates a Task.
 *    This should be called during the creation phase.
 *************************************************************************/
#if 0
kal_taskid kal_create_task(kal_char* task_name_ptr, kal_uint8 priority,
									kal_uint32 stack_size, kal_task_func_ptr entry_func_ptr,
									void* entry_param_ptr, kal_uint8 options);

/* This routine deletes the specified task This should be called during termination phase */
void kal_delete_task(kal_taskid task_id);
#endif 

void kal_sleep_task(kal_uint32 time_in_ticks);

#if 0
kal_hisrid  kal_create_hisr (kal_char* hisr_name, kal_uint8 priority,
									  kal_uint32 stack_size, kal_hisr_func_ptr entry_func,
									  kal_uint8 options);

void kal_delete_hisr(kal_hisrid ext_hisr_id);

void kal_activate_hisr(kal_hisrid ext_hisr_id);

kal_uint32 kal_change_priority(kal_taskid taskid, kal_uint32 new_priority);

kal_uint32 stack_change_priority_by_module_ID(module_type mod_ID, kal_uint32 new_priority);

void *kal_get_task_stack_ptr(kal_taskid taskid);

kal_bool kal_if_hisr(void);

kal_taskid kal_get_task_self_id(void);
#endif 

void kal_get_my_task_index(kal_uint32 *index);


/*************************************************************************
 * ITC Management
 *    This routine creates a queue.
 *    This should be called during the Creation phase
 *************************************************************************/

/* This routine creates a message queue with variable length messages
 * or fixed length message depending on the value passed at q_type. if
 * q_type == KAL_FIXED_LEN_MSG_Q, msg_size will contain the
 * size of message passed to the queue. if q_type ==
 * KAL_VARIABLE_LEN_MSG_Q, msg_size will contain the maximum size
 * of messge that can be posted in the queue. */
kal_msgqid kal_create_msg_q(kal_char* queue_name_ptr, kal_uint16 msg_size,
									 kal_uint16 max_msgs);

/* This routine deletes the specified queue.
	This should be called during termination phase */
void kal_delete_msg_q( kal_msgqid queue_id);

/* To Enqueue message : Execution Phase */
kal_status kal_enque_msg(kal_msgqid ext_msg_q_id, void* buffer_ptr,
								 kal_uint16 msg_size, kal_wait_mode wait_mode, kal_bool message_to_head);

/* To De-queue message : Execution Phase */
kal_status kal_deque_msg(kal_msgqid ext_msg_q_id, void* buffer_ptr, kal_uint16 *msg_size,
								 kal_wait_mode wait_mode);

/* To get queue's information */
kal_bool kal_get_queue_info(kal_msgqid task_ext_qid, kal_msgq_info *msgq_info);

/* This routine creates a Mutex. This should be called during the Creation phase */
kal_mutexid kal_create_mutex( kal_char* mutex_name_ptr);

/* This routine deletes the specified Mutex. This should be called during the termination phase */
void kal_delete_mutex( kal_mutexid mutex_id);

/* This routine captures the created mutex. This should be called during the Execution phase */
void kal_take_mutex( kal_mutexid    mutex_id );

/* This routine releases the mutex.This should be called during the Execution phase */
void kal_give_mutex( kal_mutexid    mutex_id );

/* This routine creates a Semaphore. This should be called during the Creation phase */
kal_semid kal_create_sem( kal_char*    sem_name_ptr, kal_uint32 initial_count );

/* This routine deletes the specified Sem. This should be called during the termination phase */
void kal_delete_sem( kal_semid sem_id);

/* This routine captures the created sem. This should be called during the Execution phase */
kal_status kal_take_sem(kal_semid sem_id, kal_wait_mode wait_mode);

/* This routine releases the sem. This should be called during the Execution phase */
void kal_give_sem( kal_semid    sem_id );

/* This routine returns the current counting value of the given semaphore */
kal_uint32 kal_query_sem_value (kal_semid ext_sem_id_ptr);

/* This routine creates a Event Group. This should be called during the Creation phase */
kal_eventgrpid kal_create_event_group(kal_char* eventgrp_name);

/* This routine deletes a Event Group. This should be called during the Termination phase */
void kal_delete_eventgrp(kal_eventgrpid eg_id);

/* This routine sets Events to a Event Group. This should be called during the Execution phase */
kal_status kal_set_eg_events(kal_eventgrpid eg_id, kal_uint32 events,
									  kal_uint8 operation);

/* This routine retrieves Events to a Event Group. This should be called during the Execution phase */
kal_status kal_retrieve_eg_events(kal_eventgrpid eg_id, kal_uint32 requested_events,
											 kal_uint8 operation, kal_uint32 *retrieved_events,
											 kal_uint32 suspend);

/*************************************************************************
 * Memory Management
 *************************************************************************/

#if 0
#ifdef APPMEM_SUPPORT

/*
 * kal_app_mem_xxx are APIs for application shared memory.
 */
#define kal_app_mem_alloc(size, type, owner) kal_app_mem_alloc_w_info(size, type, owner, NULL);
void *kal_app_mem_alloc_w_info(kal_uint32 size, KAL_APPMEM_TYPE type, kal_uint16 owner, const KAL_APPMEM_APP_INFO* app_info);
void kal_app_mem_free(void *ptr);
KAL_APPMEM_APP_INFO* kal_app_mem_sync_alloc_clean_list(kal_uint32 size);

#endif   /* APPMEM_SUPPORT */
#endif 

/*
 * kal_evshed_xxx is special API which is optimized for event scheduler and
 * double-linked list.  Though its nameing is memory, actually, it's implemented
 * by partition memory instead dynamic memory to avoid fragmemtation problem.
 */
void kal_evshed_create_pool( void );
void* kal_evshed_get_mem( kal_uint32 size );
void kal_evshed_free_mem( void* buff_ptr );

#if 0
/*
 * kal_adm_xxx are APIs for application to manage memory.
 */
//extern KAL_ADM_ID kal_adm_create(void *mem_addr, kal_uint32 size, kal_uint32 *subpool_size, kal_bool islogging);   //chenqiang delete for touch panel
extern void *kal_adm_internal_alloc(KAL_ADM_ID adm_id, kal_uint32 size, char *filename, kal_uint32 line);
#define kal_adm_alloc(adm_id, size) kal_adm_internal_alloc(adm_id, size, __FILE__, __LINE__)
extern void kal_adm_free(KAL_ADM_ID adm_id, void *mem_addr);
extern kal_status kal_adm_delete(KAL_ADM_ID adm_id);
extern kal_uint32 kal_adm_get_max_alloc_size(KAL_ADM_ID adm_id);
extern kal_uint32 kal_adm_get_total_left_size(KAL_ADM_ID adm_id);
extern void kal_adm_print_log(KAL_ADM_ID adm_id);

/*
 * Free space query function for system memory and debug memory.
 */
extern kal_uint32 kal_sys_mem_query_freesize(void);
extern kal_uint32 kal_debug_mem_query_freesize(void);


/*************************************************************************
 * Buffer Management
 *************************************************************************/

/* types used for buffer manager */

/*
 * This routine Creates a buffer pool.
 * This should be called during the Creation phase
 */
kal_poolid kal_create_buff_pool(kal_uint16 buff_size, kal_uint16 num_buffs);

/*
 * This routine deletes the specified buffer pool.
 * This should called during the termination phase
 */
void kal_delete_buff_pool( kal_poolid pool_id );

void *kal_get_buffer(kal_poolid ext_pool_id, kal_uint32 alloc_id, kal_uint32 size,
                     kal_uint32 *num_free_buffs_p, kal_char *alloc_file, kal_uint32 alloc_line,
                     kal_bool alloc_check);
#endif 

//void kal_release_buffer(void *usr_buff, kal_uint32 dealloc_id, kal_char *dealloc_file, kal_uint32 dealloc_line);

#if 0
   kal_poolid kal_get_poolid_from_usr_buff( void* usr_buff_p );

kal_uint32 kal_get_num_free_buffs( kal_poolid pool_id );
kal_bool kal_query_ctrlbuf_max_consumption(kal_uint32 *ptr);


/*************************************************************************
 * Timer Management
 *************************************************************************/

/*
 * This routine Creates a timer.
 * This should be called during the Creation phase
 */
kal_timerid kal_create_timer(kal_char* timer_name_ptr);

/*
 * This routine deletes the specified timer.
 * This should be called during the termination phase
 */
void  kal_delete_timer(kal_timerid timer_id);

/*
 * This routine starts the timer with specified delay.
 * This should be called during the Execution phase
 */
void kal_set_timer(kal_timerid timer_id, kal_timer_func_ptr handler_func_ptr,
						 void* handler_param_ptr, kal_uint32 delay,
						 kal_uint32 reschedule_time);

/*
 * This routine cancles the created timer
 * This should be called during the Execution phase
 */
void kal_cancel_timer( kal_timerid    timer_id );

/*
 * This routine returns the time elaplsed from the system start time
 * This should be called during the Execution phase
 */
#endif 
void kal_get_time( kal_uint32* ticks_ptr);

#if 0
/*
 * This routine returns the remaining time for the expiration
 * of a given timer.
 */
kal_uint32 kal_get_time_remaining(kal_timerid    timer_id );

#define ALIGNED_TIMER_ID	0xcbcbcbcb
#define UNALIGNED_TIMER_ID	0xbcbcbcbc

/*
 * This routine return allowed sleep ticks
 */
kal_uint32 kal_check_allowed_sleep_clock( kal_uint32 period_ticks );

/*
 * This routine recover system clock
 */
void kal_recover_clock(kal_uint32 delayed_ticks);


/*
 * This routine invoke expired events during sleep period
 */
void kal_invoke_expired_events(kal_uint32 delayed_ticks);

/*************************************************************************
 * Error Handling and Debug Management
 *************************************************************************/

/*
 * This routine is to be called in the fatal error situation.
 * This should be called during the Execution phase
 */
void kal_fatal_error_handler(kal_char* error_message_ptr, kal_uint32 error_code,
									  kal_uint32 os_error_code);

/*
 * This routine stores the pointer to the fatal error handler.
 * This should be called during the Initialization phase
 */
void kal_register_fatal_error_handler(kal_error_func_ptr fatal_handler_ptr);

void kal_exit(void);
#endif
#ifdef CSD_KAL_MNT

#ifdef MMI_ON_HARDWARE_P
#define kal_print( string_to_be_printed ) NU_Printf( string_to_be_printed )
#define kal_printf( string_to_be_printed ) NU_Printf string_to_be_printed
#else
#define kal_print( string_to_be_printed )
#define kal_printf( string_to_be_printed )
#endif


#else   /* CSD_KAL_MNT */

#ifdef DEBUG_KAL

extern void kal_debug_print(kal_char* string_to_be_printed);
extern void dbg_print(char *fmt,...);

#define kal_print( string_to_be_printed ) kal_debug_print( string_to_be_printed )
#define kal_printf( string_to_be_printed ) dbg_print string_to_be_printed

#else   /* DEBUG_KAL */

#ifdef MMI_ON_HARDWARE_P
#define kal_print(fmt,...) \
	do{\
		mmi_trace(1,fmt,##__VA_ARGS__);\
	}while(0)
	
#define kal_pirntf(fmt,...) \
	do{\
		mmi_trace(1,fmt,##__VA_ARGS__);\
	}while(0)
#else
#define kal_print( string_to_be_printed )
#define kal_printf( string_to_be_printed )
#endif
#endif   /* DEBUG_KAL */

#endif   /* CSD_KAL_MNT */
#if 0
extern void stack_print(char* string);

#define system_print(s) stack_print(s)


/*************************************************************************
 * Misc
 *************************************************************************/

extern kal_char *kal_str_dup(kal_char *string);

extern kal_char *kal_strtok_r(kal_char *string, const kal_char *seperators, kal_char **ppLast);
#endif 

extern void kal_assert_fail(char* expr, char* file, kal_uint32 line, kal_bool isext, kal_uint32 ex1, kal_uint32 ex2, kal_uint32 ex3);

#define kal_sprintf sprintf

extern char *basename(char *file);

#if 0
#ifdef __ROMSA_SUPPORT__

#if defined(DEBUG_KAL)

#define ROM_ASSERT(expr) if(!(expr)) { ROMSA_FuncTable[ROMSA_PTR_kal_assert_fail]((void *)#expr, __FILE__, __LINE__, KAL_FALSE, 0, 0, 0); }
#define EXT_ROM_ASSERT(expr, e1, e2, e3) if(!(expr)) { ROMSA_FuncTable[ROMSA_PTR_kal_assert_fail]((void *)#expr, __FILE__, __LINE__, KAL_TRUE, e1, e2, e3); }

#else   /* DEBUG_KAL */

#define ROM_ASSERT(expr) do { } while (0);
#define EXT_ROM_ASSERT(expr, e1, e2, e3) if(!(expr)) { ROMSA_FuncTable[ROMSA_PTR_kal_assert_fail]((void *)NULL, NULL, 0, KAL_FALSE, 0, 0, 0); }

#endif  /* DEBUG_KAL */

#endif   /* __ROMSA_SUPPORT__ */
#endif 

#if defined(DEBUG_KAL)

/* For target platform, layer1 simulation's ASSERT() defined in l1_tool.h */

#if 0
#define EXT_ASSERT(expr, e1, e2, e3) if(!(expr)) { kal_assert_fail(#expr, __FILE__, __LINE__, KAL_TRUE, e1, e2, e3); }
#else
#define EXT_ASSERT(expr, e1, e2, e3) ASSERT((expr))
#endif

#else   /* DEBUG_KAL */

#if 0
#define EXT_ASSERT(expr, e1, e2, e3) if(!(expr)) { kal_assert_fail(NULL, NULL, 0, KAL_FALSE, 0, 0, 0); }
#else
#define EXT_ASSERT(expr, e1, e2, e3)
#endif

#endif /* DEBUG_KAL */

#endif   /* CSD_TARGET || CSD_KAL_MNT */

 
#if 0
/*************************************************************************
* FUNCTION
*  kal_mem_cpy
*
* DESCRIPTION
*  This routine copies size bytes from src memory location to dest memory
* location.
*
* CALLS
*
* PARAMETERS
*  dest     -  (Memory pointer of the destination)
*  src      -  (Memory pointer of the source)
*  size     -  (number of bytes to be copied)
*
* RETURNS
*  None
*
* GLOBALS AFFECTED
*
* HISTORY
 
*       Rex Luo         2002/07/09           Create initial version 1.0
*
*************************************************************************/
__inline void * kal_mem_cpy(void* dest, const void* src, kal_uint32 size)
{
	return memcpy ( dest, src, size );
}
 
/*************************************************************************
* FUNCTION
*  kal_mem_set
*
* DESCRIPTION
*  This routine sets the size number of bytes by value from dest memory
*  location.
*
* CALLS
*
* PARAMETERS
*  dest     -  (Memory pointer which is to be set)
*  set      -  (Value with which it is to be set)
*  size     -  (number of bytes to be set)*
*
* RETURNS
*  None
*
* GLOBALS AFFECTED
*
* HISTORY
 
*       Rex Luo         2002/07/09           Create initial version 1.0
*
*************************************************************************/
__inline void * kal_mem_set ( void*    dest,
										kal_uint8     value,
										kal_uint32     size
									 )
{
	return memset ( dest, value, size );
}

/*************************************************************************
* FUNCTION
*  kal_mem_cmp
*
* DESCRIPTION
*  This routine compares contents from two memory
* locations lexicographically.
* This should be called during the Execution phase
*
* CALLS
*
* PARAMETERS
*
* RETURNS
*  None
*
* GLOBALS AFFECTED
*
* HISTORY
 
*       Rex Luo         2002/07/09           Create initial version 1.0
*
*************************************************************************/
__inline kal_int32 kal_mem_cmp(const void* src1, const void* src2, kal_uint32 size)
{
	return memcmp ( src1, src2, size );
}

#endif
 
/*************************************************************************
* Execption Type
 *************************************************************************/
typedef enum {
	INVAID_EXCEPTION        =  0,
	UNDEF_EXCEPTION         =  1,
	SWI_EXCEPTION        =  2,
	PREFETCH_EXCEPTION      =  3,
	DATAABORT_EXCEPTION     =  4,
	ASSERT_FAIL_EXCEPTION   =  5,
	SYS_FATALERR_EXT_TASK_EXCEPTION  =  6,
	SYS_FATALERR_EXT_BUF_EXCEPTION = 7,
	SYS_LOCKUP           =  8,
	NUM_EXCEPTION
} exception_type;

#if 0
/*************************************************************************
* Boot mode
 *************************************************************************/
typedef enum {
	FACTORY_BOOT = 0,
	NORMAL_BOOT = 1,
	USBMS_BOOT =  2,
	NUM_OF_BOOT_MODE,
	UNKNOWN_BOOT_MODE = 0xff
} boot_mode_type;

 
#if 0
__inline boot_mode_type stack_query_boot_mode(void)
{
	extern boot_mode_type system_boot_mode;
	return system_boot_mode;
} 
#endif
 
/*************************************************************************
 * Include Internal Header Files
 *************************************************************************/
#ifdef KAL_ON_NUCLEUS

#include "kal_nucleus_common_defs.h"
#include "kal_nucleus.h"

#endif   /* KAL_ON_NUCLEUS */
#endif

#if defined(CSD_TARGET ) || defined(CSD_KAL_MNT)

//#include "kal_internal.h"
#include "lcd_ip_cqueue.h"

#ifdef DEBUG_KAL

 

#endif   /* DEBUG_KAL */

#include "kal_debug.h"

#endif /* CSD_TARGET || CSD_KAL_MNT */
#include "kal_active_module.h"
#if 1
/*************************************************************************
 * wide character support
 *************************************************************************/
// FIXME
// WCHAR is a type defined by cs_types.h
// and MUST NOT be redefined. If they are needed,
// include cs_types.h
//typedef unsigned short WCHAR;

// 16bits string operations
int kal_wstrlen(const WCHAR *wstr);
WCHAR *kal_wstrcpy(WCHAR *to, const WCHAR *from);
WCHAR *kal_wstrncpy(WCHAR *to, const WCHAR *from, int n);
int kal_wstrcmp(const WCHAR *s1, const WCHAR *s2);
int kal_wstrncmp(const WCHAR *s1, const WCHAR *s2, int n);
WCHAR *kal_wstrcat(WCHAR *s1, const WCHAR *s2);
WCHAR *kal_wstrncat(WCHAR *s1, const WCHAR *s2, int n);
WCHAR *kal_wstrchr(const WCHAR *s, int c);
WCHAR *kal_wstrrchr(const WCHAR *str, int ch);


// dual 8bits string operations
int kal_dchar_strlen(const char *wstr);
char *kal_dchar_strcpy(char *to, const char *from);
char *kal_dchar_strncpy(char *to, const char *from, int n);
int kal_dchar_strcmp(const char *s1, const char *s2);
int kal_dchar_strncmp(const char *s1, const char *s2, int n);
char *kal_dchar_strcat(char *s1, const char *s2);
char *kal_dchar_strncat(char *s1, const char *s2, int n);
char *kal_dchar_strchr(const char *s, int c);
char *kal_dchar_strrchr(const char *str, int ch);
void kal_dchar2char(WCHAR *outstr, char* tostr);

void kal_wsprintf(unsigned short *outstr, char *fmt,...);
#define kal_toWCHAR(asciiString) L##asciiString

#ifdef __cplusplus
};
#endif /* __cplusplus */ 
#endif 

#endif /* _KAL_RELEASE_H */


