//------------------------------------------------------------------------------
/*
================================================================================

  This source code is property of Coolsand. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or
  transmisison of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright Coolsand Technologies (c) 2003

================================================================================
*/

/*
================================================================================
  File       : sap_cfg.h
--------------------------------------------------------------------------------

  Scope      : System configuration.

  History    :
--------------------------------------------------------------------------------
================================================================================
*/

#ifndef __SAP_CFG_H__
#define __SAP_CFG_H__

#include "sxs_type.h"
#include "sxs_cfg.h"
#include "sxr_ops.h"
#include "sxs_io.h"
#include "sxs_lib.h"
#include "cmn_defs.h"
#include "stt.h"

/******************************************************************************
       OS TASKS CONFIGURATION
******************************************************************************/
extern UINT8 testTaskId;
extern UINT8 testMbx;

extern void testTaskInit (void);
extern void sxs_TaskIdle (void);
//extern stt_Param_t SttParam;
//extern void stt_Task (stt_Param_t * Param);

#define STT_TASK_STACK_SIZE  300
#define STT_TASK_PRIORITY    0

#define TEST_TASK_STACK_SIZE   (4*1024)
#define TEST_TASK_PRIORITY     200


#define IDLE_TASK_STACK_SIZE (4*1024)
#define IDLE_TASK_PRIORITY   255

#ifdef __SAP_CFG_VAR__
const sxr_TaskDesc_t sxs_TaskIdleDesc =
{
    (void(*)(void *))sxs_TaskIdle,
    0,
    "Task Idle.",
    IDLE_TASK_STACK_SIZE,
    IDLE_TASK_PRIORITY
};

const sxr_TaskDesc_t sxs_testTaskDesc =
{
    (void(*)(void *))testTaskInit, 
    NIL, 
    "Test Task", 
    TEST_TASK_STACK_SIZE,
    TEST_TASK_PRIORITY
};



#if 0
const sxr_TaskDesc_t stt_TaskDesc =
{
    (void(*)(void *))stt_Task,
    0,
    "STT Task.",
    STT_TASK_STACK_SIZE,
    STT_TASK_PRIORITY
};
#endif
#else
extern const sxr_TaskDesc_t sxs_TaskIdleDesc;
extern const sxr_TaskDesc_t sxs_testTaskDesc;
extern const sxr_TaskDesc_t stt_TaskDesc;
#endif


#endif
