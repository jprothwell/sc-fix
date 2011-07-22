//------------------------------------------------------------------------------
//  $Log: sap_main.c,v $
//  Revision 1.8  2006/06/07 12:53:19  fch
//  API mbx is remote when use of the StackSimulator (flag __STACKSIMU__)
//
//  Revision 1.7  2006/05/18 18:16:07  fch
//  Update with Bob\dev
//
//  Revision 1.6  2006/03/24 10:31:07  fch
//  Update with Bob
//
//  Revision 1.5  2006/03/09 13:27:02  fch
//  Inclusion of file pal_sxr.h instead of pal_sx.h
//
//  Revision 1.4  2006/03/09 11:02:48  fch
//  Update from bob
//
//  Revision 1.3  2006/02/13 02:37:23  oth
//  Finish to remove SXS from the IO modules
//
//  Revision 1.2  2005/12/29 17:39:16  pca
//  Log keyword added to all files
//
//------------------------------------------------------------------------------
/*
================================================================================

  This source code is property of StackCom. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or
  transmisison of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright StackCom (c) 2003

================================================================================
*/

/*
================================================================================
  File       : sap_main.c
--------------------------------------------------------------------------------

  Scope      : Main.

  History    :
--------------------------------------------------------------------------------
  Mar 23 06  |  MCE  | Removed call to pal_SimuIdleHook (en attendant mieux)
  Mar 23 06  |  MCE  | Replaced _SXS with _SXR
  Mar 03 06  |  ADA  | Timer initialization is generic.
  Mar 02 06  |  ADA  | sxs_StartAll: create Cs data queue.
  Dec 02 05  |  OTH  | Set number of cluster of pool 2 to 255 (nb max is 255)
  Aug 31 05  |  ADA  | Move cluster creation from sap_usr.c
  Jun 06 05  |  JFN  | Add init timer for PALB
  Aug 09 04  |  OTH  | Merge with sap_main.c from integration
  Aug 05 03  |  ADA  | Creation
================================================================================
*/

#include "chip_id.h"
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#else
#include "hal_sys.h"
#endif

#if !defined(__TARGET__) || defined (__SIMU__)
#include <malloc.h>
#ifdef __STKTRACE__
#include "sxs_udp.h"
#endif
#endif

#include "sxs_srl.h"
#include "sxs_type.h"
#include "sxs_lib.h"
#include "sxr_ops.h"
#include "sxr_csp.h"
#include "sxr_mutx.h"
#include "sxs_rmt.h"
#include "cmn_defs.h"

#define __SAP_CFG_VAR__
#include "sap_cfg.h"
#undef  __SAP_CFG_VAR__

#include "sxs_io.h"
#include "sxs_spy.h"
#include "pal_mem.h"
#include "pal_gsm.h"

#define PAL_DBG_PORT_IDX 0

typedef u32 MsgBody_t;

#include "itf_msg.h"

void  sap_InitUsr ( void );

//------------------------------------------------------------------
// Idle hook variables

void (*g_sxsFsIdleHookFuncPtr)(void) = NIL;

#define SXS_DEBUG_IDLE_HOOK_NUM 4
void (*g_sxsDebugIdleHookFuncPtr[SXS_DEBUG_IDLE_HOOK_NUM])(void) =
    { NIL, };

// To avoid linkage issue while compiling platform_test programs,
// we put the definition in sx_map.c instead of here
extern volatile u32 g_sxsDebugIdleHookEnable;

//------------------------------------------------------------------
// FS idle hook functions

void sxs_RegisterFsIdleHookFunc(void (*funcPtr)(void))
{
    g_sxsFsIdleHookFuncPtr = funcPtr;
}

void sxs_FsIdleHook (void)
{
    if (g_sxsFsIdleHookFuncPtr != NIL)
    {
        (*g_sxsFsIdleHookFuncPtr)();
    }
}

//------------------------------------------------------------------
// Debug idle hook functions

bool sxs_RegisterDebugIdleHookFunc(void (*funcPtr)(void))
{
    int i;
    bool result = FALSE;

    u32 status = sxr_EnterSc ();
    for (i=0; i<SXS_DEBUG_IDLE_HOOK_NUM; i++)
    {
        if (g_sxsDebugIdleHookFuncPtr[i] == NIL)
            break;
    }

    if (i < SXS_DEBUG_IDLE_HOOK_NUM)
    {
        g_sxsDebugIdleHookFuncPtr[i] = funcPtr;
        result = TRUE;
    }
    sxr_ExitSc (status);
    
    return result;
}

void sxs_DebugIdleHook (void)
{
    if (g_sxsDebugIdleHookEnable == 0)
    {
        return; // skip this debug hook
    }

    int i;
    for (i=0; i<SXS_DEBUG_IDLE_HOOK_NUM; i++)
    {
        if (g_sxsDebugIdleHookFuncPtr[i] != NIL)
        {
            (*g_sxsDebugIdleHookFuncPtr[i])();
        }
    }
}


/*
==============================================================================
   Function   :
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/
void sxs_Task (sxs_TaskParse_t *TaskParse)
{
 Msg_t *Msg;
 u32   Evt [SXR_EVT_MBX_SIZE];

 if (TaskParse -> TaskInit != NIL)
  TaskParse -> TaskInit ();

 while (TRUE)
 {
  Msg = (Msg_t *)sxr_Wait (Evt, TaskParse -> MailBox);

  if (Msg)
   TaskParse -> MsgParse (Msg);
  else
  {
   if (Evt [0] & SXS_TIMER_EVT_ID)
    TaskParse -> TimerParse (Evt);
   else
    ((void (*)(void *))Evt [0]) ((void *)Evt [1]);
  }
 }
}

/*
==============================================================================
   Function   :
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/
void sxs_TimerDelivery (u32 *Id)
{
 sxs_Raise (_SXR|TNB_ARG(1)|TDB, TSTR("Timer not handled %x\n", 0x070b0001), Id);
}


/*
==============================================================================
   Function   :
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/
void sxs_StartAll (void)
{
 u32 i;

 sxr_TkSdInit ();
 sxr_JbSdInit ();

 sxr_SbxInit ();
 sxr_MutexInit ();

 for (i=0; i<CMN_LAST; i++)
  sxr_NewMailBox ();

 sxr_SetRmtMbx (STTPC_MBX, TRUE ) ;

#ifdef __STACKSIMU__
 sxr_SetRmtMbx (API_MBX, TRUE ) ;
#endif

 sxr_InitTimer (sxs_TimerDelivery, pal_TimerSetCnt, pal_TimerGetCnt, SXR_REGULAR_TIMER);

 sxr_InitQueue ();

 sxr_NewQueue ("Cs Data");

 u16 SetUart = SXS_SET_NB_DATA (SXS_8_DATA)
   | SXS_SET_NB_STOP (SXS_1_STOP)
   | SXS_SET_PARITY (SXS_PARITY_DISABLE)
   | SXS_SET_RATE (SXS_R460800);

 sxs_InitSerial (SetUart, PAL_DBG_PORT_IDX, SXS_RMT_DBG_CPU_IDX);

 sxs_IoInit ();

#ifdef __PROTO_REMOTE__
 for (i=CMN_RR; i< CMN_LAST; i++)
  sxr_SetRmtMbx (i, TRUE);
#endif

 sxr_StartTask (sxr_NewTask (&sxs_TaskIdleDesc), NIL);

 for (i=0; i<sxs_NbStaticTasksDesc; i++)
 {
  u8 TaskId =
  sxs_CmnTaskId [sxs_TaskCfg [i].TaskParse -> MailBox] = sxr_NewTask (&sxs_TaskCfg [i].TaskDesc);
  sxr_StartTask (TaskId, (void *)sxs_TaskCfg [i].TaskParse);
 }
}


/*
==============================================================================
   Function   :
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/


void sxs_TaskIdle (void)
{
 u32 Cnt = 0;
 u8 SerialDataAvail = FALSE;

 while (TRUE)
 {
  sxs_FsIdleHook();
  SerialDataAvail = sxs_SerialFlush ();
  hal_SysProcessIdle();
  pal_WDogCntRst ();
  sxs_SpyData ();
  if(!SerialDataAvail)
  {
   pal_Sleep ();
  }
  sxs_RmtIdleHook ();
  sxs_DebugIdleHook();

  Cnt++;
 }
}


// Used by cfw_shell_main.c
/// This function modifies a variable which is never used,
/// the calls to this function should be removed from CSW!!!
void PM_ProLPMDisable(void)
{
}

// FIXME
BOOL BAL_TaskInit(VOID);

#include "hal_host.h"

/*
==============================================================================
   Function   :
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/
int main (int argc, char *argv[])
{
 u32 i;

 for (i=0; i<PAL_NB_RAM_AREA; i++)
 {
  if (i == 0)
   _sxr_NewHeap ( (void *)(((u8 *)PAL_HEAP_START(0)) + sizeof (sxs_Boot_t)), PAL_HEAP_SIZE(0) - sizeof (sxs_Boot_t), 0);
  else
   _sxr_NewHeap (PAL_HEAP_START(i), PAL_HEAP_SIZE(i), i);
 }

 sxs_SetFnStamp( (u32*)(((u32)_sxs_SetFnStamp) & ~3) );

 sxs_StartAll ();

  sxr_NewCluster (18, 100); 
  sxr_NewCluster (22, 220);
 sxr_NewCluster (50, 150);
  sxr_NewCluster (60, 200);
 sxr_NewCluster (66, 150);
  sxr_NewCluster (80, 200);
  sxr_NewCluster (580, 100);

 // User specific initialisation
 BAL_TaskInit();

 sap_InitUsr ();

 sxr_CheckTask();
 
 sxr_StartScheduling ();

 return 0;
}




