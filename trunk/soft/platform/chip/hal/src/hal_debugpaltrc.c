////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_debugpaltrc.c $ //
//    $Author: huazeng $                                                        // 
//    $Date: 2010-09-30 15:30:29 +0800 (Thu, 30 Sep 2010) $                     //   
//    $Revision: 2650 $                                                         //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file   hal_debugpaltrc.c 
/// This file contains the PAL debug trace functions
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



// Common includes
#include "cs_types.h"
#include "hal_debugpaltrc.h"
#include "halp_debug.h"

#ifdef HAL_PAL_TRC_TCU_VERBOSE
#define HAL_DBGPALTRC_FIFO_DEPTH   64
#else // !HAL_PAL_TRC_TCU_VERBOSE
#define HAL_DBGPALTRC_FIFO_DEPTH   32
#endif // !HAL_PAL_TRC_TCU_VERBOSE

typedef struct {
    UINT32 id;
    UINT32 arg0;
    UINT32 arg1;
    UINT32 arg2;
    UINT32 arg3;
    UINT32 arg4;
    UINT32 arg5;
} HAL_DBGPALTRC_LINE_T;

typedef struct {
    HAL_DBGPALTRC_LINE_T Line[HAL_DBGPALTRC_FIFO_DEPTH];
    UINT8 wrPtr;
}HAL_PALDBGTRC_T;

HAL_PALDBGTRC_T g_halDbgPalTrc = { .wrPtr = 0 };


// =============================================================================
// hal_DbgPalFillTrace
// -----------------------------------------------------------------------------
/// Add one item to the trace buffer
/// The parameters are the Trace id, from the previous enum,
/// and eventual parameters.
// =============================================================================
PUBLIC VOID hal_DbgPalFillTrace(UINT32 TrcId, UINT32 arg0, UINT32 arg1, UINT32 arg2, UINT32 arg3, UINT32 arg4, UINT32 arg5)
{
    g_halDbgPalTrc.Line[g_halDbgPalTrc.wrPtr].id   = TrcId;
    g_halDbgPalTrc.Line[g_halDbgPalTrc.wrPtr].arg0 = arg0;
    g_halDbgPalTrc.Line[g_halDbgPalTrc.wrPtr].arg1 = arg1;
    g_halDbgPalTrc.Line[g_halDbgPalTrc.wrPtr].arg2 = arg2;
    g_halDbgPalTrc.Line[g_halDbgPalTrc.wrPtr].arg3 = arg3;
    g_halDbgPalTrc.Line[g_halDbgPalTrc.wrPtr].arg4 = arg4;
    g_halDbgPalTrc.Line[g_halDbgPalTrc.wrPtr].arg5 = arg5;
    g_halDbgPalTrc.wrPtr = (g_halDbgPalTrc.wrPtr + 1) % HAL_DBGPALTRC_FIFO_DEPTH;
}


// =============================================================================
// hal_DbgPalFlushTrace
// -----------------------------------------------------------------------------
/// Flush the trace
/// A call to this function prints the last items
/// recorded into the trace circular buffer.
// =============================================================================
PUBLIC VOID hal_DbgPalFlushTrace(VOID)
{
    static UINT8 already_flushed = 0;
    if (!already_flushed)
    {
        INT32 i;
        already_flushed = 1;
        HAL_TRACE(TSTDOUT|_PAL, 0, "##################");
        HAL_TRACE(TSTDOUT|_PAL, 0, "Last %d PAL calls:",HAL_DBGPALTRC_FIFO_DEPTH);
        for (i=0;i<HAL_DBGPALTRC_FIFO_DEPTH;i++)
        {
            HAL_DBGPALTRC_LINE_T *line;
            line = (HAL_DBGPALTRC_LINE_T *)&(g_halDbgPalTrc.Line[(g_halDbgPalTrc.wrPtr+i)% HAL_DBGPALTRC_FIFO_DEPTH]);

            switch (line->id)
            {
                case HAL_DBGPALTRC_FRAMESTART:
                {
                    // Recover old Fn
                    UINT32 Fn;
                    UINT16 T1 = line->arg0;
                    UINT8 T2 = line->arg1;
                    UINT8 T3 = line->arg2;
                    UINT8 Mod26 = (T3 < T2) ? (26 + T3 - T2) : (T3 - T2);
                    if (Mod26 >= 26) Mod26 -= 26;
                    Fn = ((51 * Mod26) + T3 + (51 * 26 * T1));
                    HAL_TRACE(TSTDOUT|_PAL, 0, "------------------------");
#ifdef HAL_PAL_TRC_TCU_VERBOSE
                    HAL_TRACE(TSTDOUT|_PAL, 0, "Frame Start Fn=%07d (Wrap=%04d Shift=%04d Setup=%04d)",
                        Fn,line->arg3,line->arg4,line->arg5);
#else // !HAL_PAL_TRC_TCU_VERBOSE
                    HAL_TRACE(TSTDOUT|_PAL, 0, "Frame Start Fn = %d",Fn);
#endif // !HAL_PAL_TRC_TCU_VERBOSE
                }
                break;

                case HAL_DBGPALTRC_FRAMEEND:
#ifdef HAL_PAL_TRC_TCU_VERBOSE
                    HAL_TRACE(TSTDOUT|_PAL, 0, "Frame End (LastStop=%04d)",line->arg0);
#else // !HAL_PAL_TRC_TCU_VERBOSE
                    HAL_TRACE(TSTDOUT|_PAL, 0, "Frame End");
#endif // !HAL_PAL_TRC_TCU_VERBOSE
                break;

                case HAL_DBGPALTRC_PALINIT:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "PalInitialization");
                break;

                case HAL_DBGPALTRC_TIMETUNING:
#ifdef HAL_PAL_TRC_TCU_VERBOSE
                    HAL_TRACE(TSTDOUT|_PAL, 0, "TimeTuning of %d Qb (Wrap=%04d)",line->arg0,line->arg1);
#else // !HAL_PAL_TRC_TCU_VERBOSE
                    HAL_TRACE(TSTDOUT|_PAL, 0, "TimeTuning of %d Qb",(INT8)line->arg0);
#endif // !HAL_PAL_TRC_TCU_VERBOSE
                break;

                case HAL_DBGPALTRC_SYNCCHANGE:
#ifdef HAL_PAL_TRC_TCU_VERBOSE
                    HAL_TRACE(TSTDOUT|_PAL, 0, "SynchChange of %d Qb (Wrap=%04d Shift=%04d Setup=%04d)",
                        line->arg0,line->arg1,line->arg2,line->arg3);
#else // !HAL_PAL_TRC_TCU_VERBOSE
                    HAL_TRACE(TSTDOUT|_PAL, 0, "SynchChange of %d Qb",line->arg0);
#endif // !HAL_PAL_TRC_TCU_VERBOSE
                break;

                case HAL_DBGPALTRC_FREQTUNING:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "FrequencyTuning :");
                    HAL_TRACE(TSTDOUT|_PAL, 0, "\tArfcn 0x%x\n\t%iHz [0x%03x]",line->arg0,line->arg1,line->arg2);
                break;

                case HAL_DBGPALTRC_SETCOUNTERS:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "SetGsmCounters T1=%d T2=%d T3=%d",line->arg0,line->arg1,line->arg2);
                break;

                case HAL_DBGPALTRC_RXBUFFCFG:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "RxConfig for Buffer %d Type:0x%x ItReq:0x%x ",line->arg0,line->arg1,line->arg2);
                break;

                case HAL_DBGPALTRC_SETMONWIN:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "SetMonitoring on win:%d\n\tArfcn:0x%x\n\tDate:%d Qb",line->arg0,line->arg1,line->arg2);
                break;

                case HAL_DBGPALTRC_GETMONRES:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "GetMonitoring on win:%d\n\tArfcn:0x%x\n\tPwr=-%ddBm",line->arg0,line->arg1,line->arg2);
                break;

                case HAL_DBGPALTRC_SETFCHWIN:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "SetFCH on win:%d\n\tArfcn:0x%x\n\tDate:%d Qb",line->arg0,line->arg1,line->arg2);
                break;

                case HAL_DBGPALTRC_CLOSEFCHWIN:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "Close FCH win Date:%d Qb",line->arg0);
                break;

                case HAL_DBGPALTRC_GETFCHOK:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "Found Fcch: fof=%d tof=%i Qb",line->arg0,line->arg1);
                break;

                case HAL_DBGPALTRC_GETFCHKO:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "GetFch: not found");
                break;

                case HAL_DBGPALTRC_SETSCHWIN:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "SetSch on win:%d\n\tArfcn:0x%x\n\tDate:%d Qb\n\tGain:%d",line->arg0,line->arg1,line->arg2,line->arg3);
                break;

                case HAL_DBGPALTRC_GETSCHOK:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "Found Sch:\n\tfof=%i\n\ttof=%d\n\tsnr=%d\n\tpow=%d",line->arg0,line->arg1,line->arg2,line->arg3);
                break;

                case HAL_DBGPALTRC_GETSCHKO:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "Sch not found:\n\tfof=%i\n\ttof=%d\n\tsnr=%d\n\tpow=%d",line->arg0,line->arg1,line->arg2,line->arg3);
                break;

                case HAL_DBGPALTRC_SETINTWIN:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "SetInterference on win %d\n\tArfcn:0x%x\n\tDate=%d Qb\n\tNb Ts=%d",line->arg0,line->arg1,line->arg2,line->arg3);
                break;

                case HAL_DBGPALTRC_GETINTRES:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "GetInterference on %d Ts: Ts0=%d Ts1=%d Ts2=%d Ts3=%d Ts4=%d",line->arg0,line->arg1,line->arg2,line->arg3,line->arg4,line->arg5);
                break;

                case HAL_DBGPALTRC_SETNBWIN:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "SetNBurst on win %d:\n\tBitmap:0x%01x\n\tBurstIdx:%d\n\tArfcn:0x%x\n\tDate=%d Qb\n\tGain=%d",line->arg0,line->arg1,line->arg2,line->arg3,line->arg4,line->arg5);
                break;

                case HAL_DBGPALTRC_GETNBSTRES:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "GetNBurst Result:\n\tBurstIdx:%d\n\tFOf=%i\n\tTOf=%d\n\tSnr=%d\n\tPwr=%d",line->arg0,line->arg1,line->arg2,line->arg3,line->arg4);
                break;

                case HAL_DBGPALTRC_GETNBLKRES:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "GetNBlock Result on Buffer %d:\n\tmode=%02x\n\tCS=%d\n\tBer=%d\n\tBfi=%d",line->arg0,line->arg1,line->arg2,line->arg3,line->arg4);
                break;

                case HAL_DBGPALTRC_GETUSF:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "GetUsf: USF0=0x%x USF1=0x%x USF2=0x%x USF3=0x%x",line->arg0,line->arg1,line->arg2,line->arg3);
                break;

                case HAL_DBGPALTRC_ABSTENC:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "Access Burst Encoding on Buffer %d:\n\tdata=0x%x\n\tBsic=%d\n\tSize=%d",line->arg0,line->arg1,line->arg2,line->arg3);
                break;

                case HAL_DBGPALTRC_NBLKENC:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "Block Encoding on Buffer%d:\n\tChType=%d\n\tCS=%d",line->arg0,line->arg1,line->arg2);
                break;

                case HAL_DBGPALTRC_SETTXWIN:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "Set Tx on win %d:\n\tBurstIdx:%d\n\tArfcn:0x%x\n\tDate=%d Qb\n\tBitMap=%d\n\tPCL=%d",line->arg0,line->arg1,line->arg2,line->arg3,line->arg4,line->arg5);
                break;

                case HAL_DBGPALTRC_SETRACH:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "TxType:Rach");
                break;

                case HAL_DBGPALTRC_WINRESIZE:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "WinResize of win %d: StopPos=%d",line->arg0,line->arg1);
                break;

                case HAL_DBGPALTRC_STARTCIPH:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "StartCiphering:\n\tAlgo=%d\n\tKeyLow=0x%8x\n\tKeyHigh=0x%8x",line->arg0,line->arg1,line->arg2,line->arg3);
                break;

                case HAL_DBGPALTRC_STOPCIPH:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "StopCiphering");
                break;

                case HAL_DBGPALTRC_GPRSCIPH:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "GprsCiphering:\n\tAlgo=%d\n\tKeyLow=0x%8x\n\tKeyHigh=0x%8x",line->arg0,line->arg1,line->arg2);
                break;

                case HAL_DBGPALTRC_STARTTCH:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "StartTch mode=0x%x",line->arg0);
                break;

                case HAL_DBGPALTRC_CHANGETCH:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "ChangeTchMode mode=0x%x",line->arg0);
                break;

                case HAL_DBGPALTRC_RETAMRCFG:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "RetrieveCurrentAMRCfg");
                break;

                case HAL_DBGPALTRC_SETDTX:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "SetDtx Allowed=%d",line->arg0);
                break;

                case HAL_DBGPALTRC_GETDTX:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "GetDtx =%d",line->arg0);
                break;

                case HAL_DBGPALTRC_SETLOOP:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "SetLoop mode=%d",line->arg0);
                break;

                case HAL_DBGPALTRC_STOPTCH:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "StopTch");
                break;

                case HAL_DBGPALTRC_FSUSPEND:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "FintSuspend FrameQty=%d",line->arg0);
                break;

                case HAL_DBGPALTRC_FRESUME:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "FintResume");
                break;

                case HAL_DBGPALTRC_GETELAPSED:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "GetElapsedTime : %d frames",line->arg0);
                break;

                case HAL_DBGPALTRC_RFWAKEUP:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "RfWakeUp");
                break;

                case HAL_DBGPALTRC_RFSLEEP:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "RfSleep");
                break;

                case HAL_DBGPALTRC_RESACTIVE:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "ResourceActive: %d",line->arg0);
                break;

                case HAL_DBGPALTRC_RESINACTIVE:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "ResourceInactive: %d",line->arg0);
                break;

                case HAL_DBGPALTRC_PALSLEEP:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "PAL Sleep %d",line->arg0);
                break;

                case HAL_DBGPALTRC_SETUSRVECT:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "IT Vector:%d / 0x%08x",line->arg0,line->arg1);
                break;

                case HAL_DBGPALTRC_SETUSRVECTMSK:
                    HAL_TRACE(TSTDOUT|_PAL, 0, "DSP VectorMask:0x%x",line->arg0);
                break;

                case HAL_DBGPALTRC_TEMP1:
#ifdef HAL_PAL_TRC_TCU_VERBOSE
                    HAL_TRACE(TSTDOUT|_PAL, 0, "FCH Rx Off: stop=%d rxIqTof=%d rxHold=%d lastStop=%d",
                        line->arg0,line->arg1,line->arg2,line->arg3);
#endif // HAL_PAL_TRC_TCU_VERBOSE
                break;

                case HAL_DBGPALTRC_TEMP2:
                break;
            }
        }
        HAL_TRACE(TSTDOUT|_PAL, 0, "##################");
    }
}
