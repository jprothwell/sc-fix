/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2007, Coolsand Technologies, Inc.          */
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
/*      xcv_calib.h                                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      This file contains the calibration default values and accessors. */
/*                                                                       */
/*************************************************************************/

#ifndef XCV_CALIB_H
#define XCV_CALIB_H

#include "calib_m.h"
#include "rf_names.h"
#include "gsm.h"



// Calibration parameter accessors
// ------------------------------------------------------------------------
#define XCV_PARAM(n)                (g_xcvCalibPtr->xcv->param[(n)])
#define HST_XCV_PARAM(n)            (g_xcvCalibPtr->hstXcv->param[(n)])
#define XCV_TIME(n)                 (g_xcvCalibPtr->xcv->times[(n)])

// Default Calibration Times
// ------------------------------------------------------------------------
#define DEFAULT_CALIB_PDN_TX_UP         -107
#define DEFAULT_CALIB_PDN_TX_DN           18

#define DEFAULT_CALIB_PDN_RX_UP         -108 
#define DEFAULT_CALIB_PDN_RX_DN           74 
#define DEFAULT_CALIB_SPI_CMD           -160
#define DEFAULT_CALIB_GMSK_ON            -10
#define DEFAULT_CALIB_GMSK_OFF             0
#define DEFAULT_CALIB_STROBE_UP            6
#define DEFAULT_CALIB_STROBE_DN            2  

#define DEFAULT_CALIB_TIMES             {DEFAULT_CALIB_PDN_TX_UP, \
                                         DEFAULT_CALIB_PDN_TX_DN, \
                                         DEFAULT_CALIB_PDN_RX_UP, \
                                         DEFAULT_CALIB_PDN_RX_DN, \
                                         DEFAULT_CALIB_SPI_CMD, \
                                         DEFAULT_CALIB_GMSK_ON, \
                                         DEFAULT_CALIB_GMSK_OFF, \
                                         DEFAULT_CALIB_STROBE_UP, \
                                         DEFAULT_CALIB_STROBE_DN}





// Calibration Times Accessor index      
#define PDN_TX_UP          0
#define PDN_TX_DN          1
#define PDN_RX_UP          2
#define PDN_RX_DN          3
#define SPI_CMD            4
#define TX_GMSK_ON         5
#define TX_GMSK_OFF        6
#define TX_STROBE_ON       7
#define TX_STROBE_OFF      8

// Default Calibration Parameters
// ------------------------------------------------------------------------
#define RFD_DEFAULT_CALIB_ILOSS_ARFCN_850    {128, 158, 188, 218, 251,  0}
#define RFD_DEFAULT_CALIB_ILOSS_ARFCN_GSM    {35,  65,  116,  999, 1010, 0}
#define RFD_DEFAULT_CALIB_ILOSS_ARFCN_DCS    {545, 575, 627, 677, 702, 722, 800, 860, 0}
#define RFD_DEFAULT_CALIB_ILOSS_ARFCN_PCS    {512, 558, 604, 650, 696, 743, 788, 810, 0}

// Default calibration parameters.
#define DEFAULT_CALIB_CDAC				  0xcc
#define DEFAULT_CALIB_ILOSS				  0x33333, 0x33333, 0x44444444, 0x55555555 /* GSM850 GSM900 DCS PCS */
#define DEFAULT_CALIB_AFC_GAIN			  18, 18, 36, 36 /* 850 GSM DCS PCS */  


#define DEFAULT_CALIB_PARAM              {DEFAULT_CALIB_CDAC,  \
                                          DEFAULT_CALIB_ILOSS, \
                                          DEFAULT_CALIB_AFC_GAIN }
// Calibration Parameters Accessor index
#define XTAL              (0)
#define ILOSS(band)       (1 + (band))
#define AFC_GAIN(band)    (1 + (band) + 1*GSM_BAND_QTY)

// Default calibration PAL custom parameters.
// ------------------------------------------------------------------------
#define DEFAULT_CALIB_RX_TX_FREQ_OFFSET 	{0, 0, 0, 0}   /* 850 GSM DCS PCS */
#define DEFAULT_CALIB_RX_TX_TIME_OFFSET		(-27)
#define DEFAULT_CALIB_RX_IQ_TIME_OFFSET		(0)
#define DEFAULT_CALIB_TX_IQ_TIME_OFFSET		(-12)
#define DEFAULT_CALIB_DCO_CAL_TIME        (-100)

#define DEFAULT_CALIB_XCV_SPARE         	{0, 0, 0, 0, 0, 0, 0}

#define DEFAULT_CALIB_PALCUST              {DEFAULT_CALIB_XCV_RDA6210B,     \
                                            DEFAULT_CALIB_RX_TX_FREQ_OFFSET,\
                                            DEFAULT_CALIB_RX_TX_TIME_OFFSET,\
                                            DEFAULT_CALIB_RX_IQ_TIME_OFFSET,\
                                            DEFAULT_CALIB_TX_IQ_TIME_OFFSET,\
                                            DEFAULT_CALIB_DCO_CAL_TIME,\
                                            DEFAULT_CALIB_XCV_SPARE}



#endif // XCV_CALIB_H

