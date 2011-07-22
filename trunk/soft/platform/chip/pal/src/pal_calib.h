/*************************************************************************/
/*                                                                       */
/*         Copyright (C) 2003-2006, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*  This source code is the property of Coolsand Technologies and is     */
/*  confidential. Any modification, distribution, reproduction or        */
/*  exploitation of any content of this file is totally forbidden,       */
/*  except with the written permission of Coolsand Technologies.         */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/**
    @file   pal_calib.h 
    
    @brief  This file contains the calibration default values and accessors.
*/
/*                                                                       */
/*************************************************************************/

#ifndef _PAL_CALIB_H_
#define _PAL_CALIB_H_

#include "calib_m.h"


/* -------------------------------------------------- */
/* ----- Window timing parameter accessors. */

#define PAL_TIME(n)                 (g_palCalibration->bb->times[(n)])

/* Tx Up */
#define PAL_TIME_GSMK_UP            0
#define PAL_TIME_DAC_UP             1
#define PAL_TIME_TX_OEN_UP          2

/* Tx Down */
#define PAL_TIME_GSMK_DN            3
#define PAL_TIME_DAC_DN             4
#define PAL_TIME_TX_OEN_DN          5
#define PAL_TIME_DTX_OFF_DN         6

/* Rx Up */
#define PAL_TIME_RF_IN_UP           7
#define PAL_TIME_ADC_UP             8
#define PAL_TIME_FIRST_GAIN         9
#define PAL_TIME_NEXT_GAIN          10

/* Rx Down */
#define PAL_TIME_RF_IN_DN           11
#define PAL_TIME_ADC_DN             12





#endif /* _PAL_CALIB_H_ */

