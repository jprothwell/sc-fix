/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2006, Coolsand Technologies, Inc.          */
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
/*      sw_dummy_calib.h                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      This file contains the calibration default values and accessors. */
/*                                                                       */
/*************************************************************************/

#ifndef SW_DUMMY_CALIB_H
#define SW_DUMMY_CALIB_H

#include "calib_m.h"
#include "rf_names.h"



// Calibration parameter accessors
// ------------------------------------------------------------------------
#define SW_TIME(n)                  (g_rfdCalibPtr->sw->times[(n)])

// Default calibration times.
// ------------------------------------------------------------------------
#define DEFAULT_CALIB_SW_TIMES          {}

// Default calibration parameters.
// ------------------------------------------------------------------------
#define DEFAULT_CALIB_SW_PARAM          {}

// Default calibration PAL custom parameters.
// ------------------------------------------------------------------------
#define DEFAULT_CALIB_SW_SPARE          {0, 0, 0, 0, 0, 0, 0, 0}

#define DEFAULT_CALIB_SW_PALCUST        {DEFAULT_CALIB_SW_DUMMY, \
                                         DEFAULT_CALIB_SW_SPARE}

#endif // SW_DUMMY_CALIB_H
