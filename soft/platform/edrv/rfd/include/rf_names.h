/*************************************************************************/
/*                                                                       */
/*            Copyright (C) 2003-2006, Coolsand Technologies, Inc.       */
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
/*                                                                       */
/*      rf_names.h                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file defines the identifiers and the names of the RF chips  */
/*      that have a driver in phy_cfg: XCV, PA and SW.                   */
/*                                                                       */
/*************************************************************************/

#ifndef RF_NAMES_H
#define RF_NAMES_H

#include "calib_m.h"



/* -------------------------------------------------- */
/* ----- XCV identifiers. */

enum {
    DEFAULT_CALIB_XCV_AERO1PLUS = CALIB_XCV_MASK,
    DEFAULT_CALIB_XCV_AERO2,
    DEFAULT_CALIB_XCV_SKY74117,
    DEFAULT_CALIB_XCV_RDA6210B,
    DEFAULT_CALIB_XCV_SKY74200,
    DEFAULT_CALIB_XCV_RDA6220,
    DEFAULT_CALIB_XCV_RDA6220_GALLITE,
    DEFAULT_CALIB_XCV_QTY
};

/* -------------------------------------------------- */
/* ----- XCV names. */

#define DEFAULT_CALIB_XCV_NAMES_STR     "XCV SiLab Aero 1 Plus", \
                                        "XCV SiLab Aero 2", \
                                        "XCV Skyworks 74117", \
                                        "XCV RDA 6210B", \
                                        "XCV Skyworks 74200", \
                                        "XCV RDA 6220", \
                                        "XCV RDA 6220 Gallite"

/* -------------------------------------------------- */
/* ----- PA identifiers. */

enum {
    DEFAULT_CALIB_PA_AWT6146 = CALIB_PA_MASK,
    DEFAULT_CALIB_PA_TQM6M4002,
    DEFAULT_CALIB_PA_SKY77506,
    DEFAULT_CALIB_PA_SKY77318,
    DEFAULT_CALIB_PA_RDA6212,
    DEFAULT_CALIB_PA_SKY77518,
    DEFAULT_CALIB_PA_RDA6216,
    DEFAULT_CALIB_PA_RDA6218,
    DEFAULT_CALIB_PA_RFMD7163,
    DEFAULT_CALIB_PA_RDA6231,
    DEFAULT_CALIB_PA_DUMMY,
    DEFAULT_CALIB_PA_QTY
};

/* -------------------------------------------------- */
/* ----- PA names. */

#define DEFAULT_CALIB_PA_NAMES_STR      "PA Anadigics AWT6146", \
                                        "PA Triquint TQM6M4002", \
                                        "PA Skyworks 77506", \
                                        "PA Skyworks 77318", \
                                        "PA RDA 6212", \
                                        "PA Skyworks 77518", \
                                        "PA RDA 6216", \
                                        "PA RDA 6218", \
                                        "PA RFMD 7163", \
                                        "PA RDA 6231", \
                                        "Dummy PA"
                                        
/* -------------------------------------------------- */
/* ----- SW identifiers. */

enum {
    DEFAULT_CALIB_SW_LMSP7CHA = CALIB_SW_MASK,
    DEFAULT_CALIB_SW_TQM6M4002,
    DEFAULT_CALIB_SW_SKY77506,
    DEFAULT_CALIB_SW_ASM4518806T,
    DEFAULT_CALIB_SW_ASWMF46KAAACT,
    DEFAULT_CALIB_SW_LRT613S5,
    DEFAULT_CALIB_SW_SKY77518,
    DEFAULT_CALIB_SW_LMSP33QA323,
    DEFAULT_CALIB_SW_LMSP33QA382,
    DEFAULT_CALIB_SW_RDA6218,
    DEFAULT_CALIB_SW_RFMD7163,
    DEFAULT_CALIB_SW_RDA6231,
    DEFAULT_CALIB_SW_DUMMY,
    DEFAULT_CALIB_SW_QTY
};

/* -------------------------------------------------- */
/* ----- SW names. */

#define DEFAULT_CALIB_SW_NAMES_STR      "SW LMSP7CHA", \
                                        "SW Triquint TQM6M4002", \
                                        "SW Skyworks 77506", \
                                        "SW TDK ASM4518806t", \
                                        "SW SANYO ASWMF46KAAACT", \
                                        "SW KYOCERA LRT613S5", \
                                        "SW Skyworks 77518", \
                                        "SW RDA LMSP33QA323", \
                                        "SW MURATA LMSP33QA382", \
                                        "SW RDA 6218", \
                                        "SW RFMD 7163", \
                                        "SW RDA 6231", \
                                        "Dummy SW"


#endif /* RF_NAMES_H */

