/*
================================================================================

  This source code is property of Coolsand. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or
  transmisison of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright Coolsand Technologies (c) 2006

================================================================================
*/

/*
================================================================================
  File       : sap_main.c
--------------------------------------------------------------------------------

  Scope      : Main.
================================================================================
*/

#include "cs_types.h"
#include "hal_sys.h"

#include "sxs_srl.h"
#include "sxs_type.h"
#include "sxs_lib.h"
#include "sxr_ops.h"
#include "sxs_rmt.h"
#include "cmn_defs.h"

#include "sxs_io.h"
#include "sxs_spy.h"
#include "sxr_csp.h"
#include "sxr_mutx.h"
#include "calib_sap_cfg.h"
#include "pal_gsm.h"

#define PAL_DBG_PORT_IDX 0
typedef UINT32 MsgBody_t;

#include "itf_msg.h"
#include "pmd_m.h"

extern void sxs_DebugIdleHook (void);

/*
==============================================================================
   Function   :
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/
VOID calib_SxsTimerDelivery (UINT32 *Id)
{
 sxs_Raise (_SXR, "Timer not handled %x\n", Id);
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
VOID calib_SxsTaskIdle (VOID)
{
    UINT32 cnt = 0;

    while (TRUE)
    {
        sxs_SerialFlush ();
        hal_SysProcessIdle();
        //pal_WDogCntRst();
        //pal_Sleep ();
        sxs_RmtIdleHook ();
        sxs_DebugIdleHook();
        cnt++;
    }
}


/*
==============================================================================
   Function   : calib_SxsInit
 ----------------------------------------------------------------------------
   Scope      : OS initialization
   Parameters : None.
   Return     : None.
==============================================================================
*/

VOID calib_SxsInit(VOID)
{
  UINT32 i;
 // -----------------------
 // Create Heaps
 // -----------------------
 for (i=0; i<PAL_NB_RAM_AREA; i++)
 {
  if (i == 0)
   _sxr_NewHeap ( (VOID *)(((UINT8 *)PAL_HEAP_START(0)) + sizeof (sxs_Boot_t)), PAL_HEAP_SIZE(0) - sizeof (sxs_Boot_t), 0);
  else
   _sxr_NewHeap (PAL_HEAP_START(i), PAL_HEAP_SIZE(i), i);
 }

 if (sxs_Boot -> Pattern != SXS_BOOT_PATTERN)
 {
  memset (sxs_Boot, 0, sizeof (sxs_Boot_t));
  sxs_Boot -> Pattern = SXS_BOOT_PATTERN;
 }

 sxs_SetFnStamp( (UINT32*)(((UINT32)_sxs_SetFnStamp) & ~3) );

 //------------------------
 // Init OS timer
 //------------------------
 
 sxr_InitTimer (calib_SxsTimerDelivery, pal_TimerSetCnt, pal_TimerGetCnt, SXR_REGULAR_TIMER);
 //------------------------
 // Init OS Tasks
 //------------------------
 sxr_TkSdInit ();
 //------------------------
 // Init OS Jobs
 //------------------------
 sxr_JbSdInit ();
 //------------------------
 // Init OS Mailboxes
 //------------------------
 sxr_SbxInit ();
 //------------------------
 // Init OS Mutex
 //------------------------
 sxr_MutexInit ();
 //------------------------
 // Init OS Queues
 //------------------------
 sxr_InitQueue ();

 //------------------------
 // Init OS Trace
 //------------------------
 UINT16 SetUart = SXS_SET_NB_DATA (SXS_8_DATA)
   | SXS_SET_NB_STOP (SXS_1_STOP)
   | SXS_SET_PARITY (SXS_PARITY_DISABLE)
   | SXS_SET_RATE (SXS_R460800);

 sxs_InitSerial (SetUart, PAL_DBG_PORT_IDX, SXS_RMT_DBG_CPU_IDX);
 sxs_IoInit ();

 //-----------------------
 // Start Idle Task
 // ----------------------
 sxr_StartTask (sxr_NewTask (&g_calibSxsTaskIdleDesc), NIL);

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

int calib_StubMain (int argc, char *argv[])
{
    calib_SxsInit();

    pal_Initialisation(PAL_INIT_GLOBAL, PAL_GSM900 | PAL_DCS1800);

    calib_SapInitUsr();

    // All sxr traces 
    sxs_SetTraceLevel(_SXR, 0xff);

    SXS_TRACE(TSTDOUT,"start schedule ");
    sxr_StartScheduling ();
    return 0;
}




