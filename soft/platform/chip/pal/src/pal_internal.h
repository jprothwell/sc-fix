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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/pal/src/pal_internal.h $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file pal_internal.h                                                      //
///                                                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef PAL_INTERNAL_H
#define PAL_INTERNAL_H



#include "cs_types.h"
#include "string.h"
#include "gsm.h"

#include "hal_comregs.h"
#include "calib_m.h"
#include "hal_sim.h"

#include "pal.h"
#include "pal_gsm.h"
#include "pal_calib.h"
#include "pal_flsh_p.h"
#include "hal_debugpaltrc.h"

// Type to store stack handlers
// Global variables to store external handlers
typedef struct 
{
    void(*fintHandler)(UINT32);
    void(*simHandler)(UINT32);
    void(*dspHandler)(UINT32);
} PAL_HANDLERS_T;

// Structure where handlers are stored
EXPORT  PAL_HANDLERS_T g_palHandlers ;

// Structures declared in pal_gsm.c
extern PAL_CONTEXT_T g_palCtx;
extern PAL_FRAME_T g_palFrame;
extern PAL_DEBUG_T g_palDebug;
extern UINT32 _mbbsram;

// Structure declared in pal_gsms.c
extern CALIB_CALIBRATION_T* g_palCalibration;

//--------------------------------------------------------------------//
//                                                                    //
//                      PAL Internal Functions                        //
//                                                                    //
//--------------------------------------------------------------------//
void palCommonInit (void);
void palHwInit (void);
void palEnableRF (void);
void palWinInit (int WinIdx);
void palFrameInit(void);
void palSendRfFirstGain(UINT16 date);
void palSendRfNextGain(UINT16 date);
void palReorderWin (int WinIdx);
void palProgramRxWin (int WinIdx);
void palProgramTxWin (int WinIdx);
void palSetRamps (int WinIdx);
void palSetRamps_calib (UINT16 dacValueIdx, GSM_RFBAND_T band);
void pal_DspIrqHandler(HAL_COMREGS_IRQ_STATUS_T status);
void palStartBcpu(void);
void palTimersSkippedUpdate (UINT16 elapsedFrames);
void palSetUpperBound (int stop);
//void hal_LpsSkipFrame (UINT16 frameQty);
UINT8 palCalcAgcPower(UINT8 WinIdx, UINT16 Arfcn);
GSM_RFBAND_T palGetBand(UINT16 arfcn);
UINT8 palGetTcuPtr (void);
void palSetTcuPtr (UINT8 TcuPtr);
void palRxBstOn(INT16 start);
void palRxBstOff(INT16 stop);
void palSetAGC(INT16 date);
void palXcvNeedsAfcDac(UINT16 AfcDefault);
void palTurnRfIfOff(void);
void palTurnRfIfOn(void);
void pal_FintIrqHandler (void);
void palTcuIrq0Vector (UINT32 status);
void palTcuIrq1Vector (UINT32 status);
PROTECTED VOID pal_SimIrqHandler(HAL_SIM_IRQ_STATUS_T cause);



#endif // PAL_INTERNAL_H
