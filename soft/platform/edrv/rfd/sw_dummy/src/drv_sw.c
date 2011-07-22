/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2005, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*  This source code is the property of Coolsand Technologies and is     */
/*  confidential. Any modification, distribution, reproduction or        */
/*  exploitation of any content of this file is totally forbidden,       */
/*  except with the written permission of Coolsand Technologies.         */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      sw_dummy.c                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      "driver" for dummy component                                     */
/*                                                                       */
/*************************************************************************/

#include "cs_types.h"
#include "calib_m.h"

#include "drv_sw.h"
#include "drv_sw_calib.h"

#include "rfd_cfg.h"
#include "rfd_sw.h"
#include "rfd_defs.h"

// FIXME TODO WARNING do that properly and check which one is
// the first to initialise ...
// Pointer attributes in paOpen just in case ...
EXPORT  CALIB_CALIBRATION_T* g_rfdCalibPtr
     ;

const RFD_SW_PARAMS_T g_swParams=
{
    .dtxTcoSettings = 0,
    .rxOnTcoSettings = 0,
    // Window bounds
    // NOTE : these timings will be wrong if changed by a calibration
    // file, they should be updated at execution time
    .winBounds.rxSetup = 0,
    .winBounds.rxHold  = 0,
    .winBounds.txSetup = 0,
    .winBounds.txHold  = 0
};

const RFD_SW_PARAMS_T* rfd_SwGetParameters(VOID)
{
    return &g_swParams;
}

VOID rfd_SwOpen(CONST SW_CONFIG_T* swConfig)
{
    return;
}

VOID rfd_SwClose(VOID)
{
    return;
}

VOID rfd_SwWakeUp(VOID)
{
    return;
}

VOID rfd_SwSetPosition (RFD_RFDIR_T direction, GSM_RFBAND_T band, INT16 date)
{
    return;
}

VOID rfd_SwSleep(RFD_LP_MODE_T lp_mode)
{
    return;
}

//----------------------//
// Calibration Related  //
//----------------------//


const CALIB_SW_T sw_default_calib =
{
    DEFAULT_CALIB_SW_TIMES,
    DEFAULT_CALIB_SW_PARAM,
    DEFAULT_CALIB_SW_PALCUST
};

const CALIB_SW_T * rfd_SwCalibGetDefault(VOID)
{
    return &sw_default_calib;
}

VOID rfd_SwCalibUpdateValues(VOID)
{
    return;
}

VOID rfd_SwCalibResetValues(VOID)
{
    return;
}

