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
/*      xcv.h                                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      This file is header for the RDA6220  related functions.          */
/*                                                                       */
/*************************************************************************/

#ifndef XCV_H
#define XCV_H

#include "drv_xcv_calib.h"

//--------------------------------------------------------------------//
//                                                                    //
//                    GPIO and TCO Assignement                        //
//                                                                    //
//--------------------------------------------------------------------//

//--------------------------------------------------------------------//
//                                                                    //
//                   RF SPI Commands Related                          //
//                                                                    //
//--------------------------------------------------------------------//
// General bitfield access macros
#define RDA6220_MASK(field)         (RDA6220_ ## field ## _MASK)
#define RDA6220_OFFSET(field)       (RDA6220_ ## field ## _OFFSET)
#define RDA6220_FIELD(dword, field) (((dword) >> RDA6220_OFFSET(field)) & RDA6220_MASK(field))
#define RDA6220_SET_FIELD(dword, field, value)      \
                    dword = (((dword) & ~(RDA6220_MASK(field) << RDA6220_OFFSET(field)))    \
                    | (((value) & RDA6220_MASK(field)) << RDA6220_OFFSET(field)))
#define RDA6220_WRITE_FMT(addr, data) (((addr & RDA6220_ADDR_MASK) << RDA6220_ADDR_OFFSET) |\
                                       ((data & RDA6220_DATA_MASK) << RDA6220_DATA_OFFSET))
                                                                 
#define RDA6220_READ_FMT(addr, data)  ( ((addr & RDA6220_ADDR_MASK) << RDA6220_ADDR_OFFSET)|\
                                        ((data & RDA6220_DATA_MASK) << RDA6220_DATA_OFFSET)|\
                                                                                     1<<31);


// Rda6220 RF Transceiver:
//  1 write command is:
//  1 operand bit = "0" for write + 6 bits for address + 18 bits for data = 25 bits
//           0AAAAAAD | DDDDDDDD | DDDDDDDD | D0000000
//      MSB    Byte0     Byte 1     Byte 2      Byte 3  LSB
//      
//  1 read command is:
//  1 operand bit = "1" for read + 6 bits for address + 18 bits for data = 25 bits
//           1AAAAAAD | DDDDDDDD | DDDDDDDD | D0000000
//      MSB    Byte0     Byte 1       Byte 2     Byte 3  LSB
//

#define RDA6220_ONE_CMD_BYTE_QTY        4
#define RDA6220_ADDR_MASK               0x3F
#define RDA6220_ADDR_OFFSET             25
#define RDA6220_DATA_MASK               0x3FFFF
#define RDA6220_DATA_OFFSET             7

#define BYTE_SIZE                       8
#define BYTE_MASK                       0xff


// RF commands context structure
typedef struct {
  UINT32 freqCtrl;       // @ = set arfcn and band
  UINT32 gain;  // @ = set analog & digital gain
  UINT32 digAfc;      // @ = set digital Afc
}XCV_COMMANDS_T;


// RDA6220 Registers
#define RDA6220_RX_GAIN_REG                   0x01
#define RDA6220_FREQ_CTRL_REG                 0x08
#define RDA6220_CDAC_REG                      0x19
#define RDA6220_AFC_REG                       0x1f
#define RDA6220_SYS_CTRL_REG                  0x30

#define RDA6220_TX_RAMP_0_REG                 0x03
#define RDA6220_TX_RAMP_1_REG                 0x2a
#define RDA6220_TX_RAMP_2_REG                 0x2c
#define RDA6220_TX_RAMP_3_REG                 0x34

#define RDA6220_RAMP_CURVE_MASK               0x07
#define RDA6220_RAMP_CURVE_OFFSET               11

#define RDA6220_RAMP_FACTOR_MASK             0x3ff
#define RDA6220_RAMP_FACTOR_OFFSET               0


#define RDA6220_ARFCN_MASK                   0x3ff 
#define EGSM_ARFCN_QTY                         174
#define DCS_ARFCN_QTY                          374
#define PCS_ARFCN_QTY                          299

#define RDA6220_MIN_IN_POWER                   109 // in -dBm
#define RDA6220_MAX_IN_POWER                    15  // in -dBm
#define XCVER_AGC_TARGET                        75

// AFC Parameters
#define RDA6220_CDAC_DEFAULT       (XCV_PARAM(XTAL))
#define RDA6220_CDAC_MAX            0xff
#define RDA6220_CDAC_MIN            0x0
#define RDA6220_CDAC_OFFSET         0x18

// MID value is changed from 0x2000 to 0x1500 to allow more space for temperature increasing
#define RDA6220_AFC_MID             0x1500 // 0x2000
#define RDA6220_AFC_HRANGE          0xa00
#define RDA6220_AFC_MAX             0x3d00
#define RDA6220_AFC_MIN             0x200
//#define RDA6220_AFC_HRANGE          0x1fff
//#define RDA6220_AFC_MAX             0x3fff
//#define RDA6220_AFC_MIN             0x0

#define RDA6220_AFC_MASK            0x3fff
#define RDA6220_AFC_BOUND_HIGH     (RDA6220_AFC_MID+RDA6220_AFC_HRANGE)
#define RDA6220_AFC_BOUND_LOW      (RDA6220_AFC_MID-RDA6220_AFC_HRANGE)

/* -----------------------------------------------------------------
 | From  To(dbm) |  Tab[]  |  LNA + AGain  |Digital |TotalVoltGain |
 ------------------------------------------------
 |     -109      | Tab[0] |     xxxxxx     |  xxx   |  xxxxxxxxxx  |
 ------------------------------------------------
 |     -108      | Tab[1] |     xxxxxx     |  xxx   |  xxxxxxxxxx  |
 -------------------------------------------------------------------
 |     ......      ......       .....        .....     ..........  |
 |                                             |
 |     ......      ......       .....        .....     ..........  |
 -------------------------------------------------------------------
 |     -16       | Tab[93]|     xxxxxx     |  xxx   |  xxxxxxxxxx  |
 ------------------------------------------------
 |     -15       | Tab[94]|     xxxxxx     |  xxx   |  xxxxxxxxxx  |
 ------------------------------------------------ ------------------*/
// AGC table 
#define AGC_QTY 95
// APC table
#define APCLB_QTY 15 // GSM850 & GSM900: PCL 5 - 29 (5 - 33 dB)
#define APCHB_QTY 16 // DCS1800 & PCS1900: PCL 0 - 15 (0 - 30 dB)


typedef struct {
    UINT8    analogGain;
    UINT8    digGain;
    UINT16   totGain;
} XCV_RX_GAIN_T;


#define EGSM_AGC_DEFAULT {\
{0x0 , 0x10 ,   5},\
{0x0 , 0x12 ,   6},\
{0x0 , 0x14 ,   7},\
{0x0 , 0x16 ,   8},\
{0x0 , 0x18 ,   9},\
{0x0 , 0x1a ,  10},\
{0x0 , 0x20 ,  11},\
{0x0 , 0x22 ,  12},\
{0x0 , 0x24 ,  13},\
{0x0 , 0x26 ,  14},\
{0x0 , 0x28 ,  15},\
{0x0 , 0x2a ,  16},\
{0x0 , 0x30 ,  17},\
{0x0 , 0x32 ,  18},\
{0x1 , 0x12 ,  19},\
{0x1 , 0x14 ,  20},\
{0x1 , 0x16 ,  21},\
{0x1 , 0x18 ,  22},\
{0x1 , 0x1a ,  23},\
{0x1 , 0x20 ,  24},\
{0x1 , 0x22 ,  25},\
{0x1 , 0x24 ,  26},\
{0x2 , 0x12 ,  27},\
{0x2 , 0x14 ,  28},\
{0x2 , 0x16 ,  29},\
{0x2 , 0x18 ,  30},\
{0x2 , 0x1a ,  31},\
{0x2 , 0x20 ,  32},\
{0x2 , 0x22 ,  33},\
{0x2 , 0x24 ,  34},\
{0x2 , 0x26 ,  35},\
{0x2 , 0x28 ,  36},\
{0x2 , 0x2a ,  37},\
{0x2 , 0x30 ,  38},\
{0x3 , 0x14 ,  39},\
{0x3 , 0x16 ,  40},\
{0x3 , 0x18 ,  41},\
{0x3 , 0x1a ,  42},\
{0x3 , 0x20 ,  43},\
{0x3 , 0x22 ,  44},\
{0x3 , 0x24 ,  45},\
{0x3 , 0x26 ,  46},\
{0x3 , 0x28 ,  47},\
{0x3 , 0x2a ,  48},\
{0x3 , 0x30 ,  49},\
{0x3 , 0x32 ,  50},\
{0x3 , 0x34 ,  51},\
{0x4 , 0x14 ,  52},\
{0x4 , 0x16 ,  53},\
{0x4 , 0x18 ,  54},\
{0x4 , 0x1a ,  55},\
{0x4 , 0x20 ,  56},\
{0x4 , 0x22 ,  57},\
{0x4 , 0x24 ,  58},\
{0x4 , 0x26 ,  59},\
{0x4 , 0x28 ,  60},\
{0x4 , 0x2a ,  61},\
{0x4 , 0x30 ,  62},\
{0x5 , 0x12 ,  63},\
{0x5 , 0x14 ,  64},\
{0x5 , 0x16 ,  65},\
{0x5 , 0x18 ,  66},\
{0x5 , 0x1a ,  67},\
{0x5 , 0x20 ,  68},\
{0x6 , 0x12 ,  69},\
{0x6 , 0x14 ,  70},\
{0x6 , 0x16 ,  71},\
{0x6 , 0x18 ,  72},\
{0x6 , 0x1a ,  73},\
{0x7 , 0x10 ,  74},\
{0x7 , 0x12 ,  75},\
{0x7 , 0x14 ,  76},\
{0x7 , 0x16 ,  77},\
{0x7 , 0x18 ,  78},\
{0x7 , 0x1a ,  79},\
{0x7 , 0x20 ,  80},\
{0x7 , 0x22 ,  81},\
{0x7 , 0x24 ,  82},\
{0x7 , 0x26 ,  83},\
{0x7 , 0x28 ,  84},\
{0x7 , 0x2a ,  85},\
{0x7 , 0x30 ,  86},\
{0x7 , 0x32 ,  87},\
{0x7 , 0x34 ,  88},\
{0x7 , 0x36 ,  89},\
{0x7 , 0x38 ,  90},\
{0x7 , 0x3a ,  91},\
{0x7 , 0x40 ,  92},\
{0x7 , 0x42 ,  93},\
{0x7 , 0x44 ,  94},\
{0x7 , 0x46 ,  95},\
{0x7 , 0x48 ,  96},\
{0x7 , 0x4a ,  97},\
{0x7 , 0x50 ,  98},\
{0x7 , 0x52 ,  99}\
}

#define DCS_AGC_DEFAULT {\
{0x0 , 0x01 , 5 },\
{0x0 , 0x0a , 6 },\
{0x0 , 0x10 , 7 },\
{0x0 , 0x12 , 8 },\
{0x0 , 0x14 , 9 },\
{0x0 , 0x16 , 10},\
{0x0 , 0x18 , 11},\
{0x0 , 0x1a , 12},\
{0x0 , 0x20 , 13},\
{0x0 , 0x22 , 14},\
{0x0 , 0x24 , 15},\
{0x0 , 0x26 , 16},\
{0x0 , 0x28 , 17},\
{0x0 , 0x2a , 18},\
{0x0 , 0x30 , 19},\
{0x0 , 0x32 , 20},\
{0x0 , 0x34 , 21},\
{0x1 , 0x10 , 22},\
{0x1 , 0x12 , 23},\
{0x1 , 0x14 , 24},\
{0x1 , 0x16 , 25},\
{0x1 , 0x18 , 26},\
{0x1 , 0x1a , 27},\
{0x1 , 0x20 , 28},\
{0x2 , 0x10 , 29},\
{0x2 , 0x12 , 30},\
{0x2 , 0x14 , 31},\
{0x2 , 0x16 , 32},\
{0x2 , 0x18 , 33},\
{0x2 , 0x1a , 34},\
{0x2 , 0x20 , 35},\
{0x2 , 0x22 , 36},\
{0x2 , 0x24 , 37},\
{0x2 , 0x26 , 38},\
{0x2 , 0x28 , 39},\
{0x2 , 0x2a , 40},\
{0x3 , 0x12 , 41},\
{0x3 , 0x14 , 42},\
{0x3 , 0x16 , 43},\
{0x3 , 0x18 , 44},\
{0x3 , 0x1a , 45},\
{0x3 , 0x20 , 46},\
{0x3 , 0x22 , 47},\
{0x3 , 0x24 , 48},\
{0x4 , 0x10 , 49},\
{0x4 , 0x12 , 50},\
{0x4 , 0x14 , 51},\
{0x4 , 0x16 , 52},\
{0x4 , 0x18 , 53},\
{0x4 , 0x1a , 54},\
{0x4 , 0x20 , 55},\
{0x4 , 0x22 , 56},\
{0x4 , 0x24 , 57},\
{0x4 , 0x26 , 58},\
{0x4 , 0x28 , 59},\
{0x4 , 0x2a , 60},\
{0x4 , 0x30 , 61},\
{0x5 , 0x10 , 62},\
{0x5 , 0x12 , 63},\
{0x5 , 0x14 , 64},\
{0x5 , 0x16 , 65},\
{0x5 , 0x18 , 66},\
{0x5 , 0x1a , 67},\
{0x5 , 0x20 , 68},\
{0x6 , 0x10 , 69},\
{0x6 , 0x12 , 70},\
{0x6 , 0x14 , 71},\
{0x6 , 0x16 , 72},\
{0x6 , 0x18 , 73},\
{0x6 , 0x1a , 74},\
{0x6 , 0x20 , 75},\
{0x6 , 0x22 , 76},\
{0x7 , 0x0a , 77},\
{0x7 , 0x10 , 78},\
{0x7 , 0x12 , 79},\
{0x7 , 0x14 , 80},\
{0x7 , 0x16 , 81},\
{0x7 , 0x18 , 82},\
{0x7 , 0x1a , 83},\
{0x7 , 0x20 , 84},\
{0x7 , 0x22 , 85},\
{0x7 , 0x24 , 86},\
{0x7 , 0x26 , 87},\
{0x7 , 0x28 , 88},\
{0x7 , 0x2a , 89},\
{0x7 , 0x30 , 90},\
{0x7 , 0x32 , 91},\
{0x7 , 0x34 , 92},\
{0x7 , 0x36 , 93},\
{0x7 , 0x38 , 94},\
{0x7 , 0x3a , 95},\
{0x7 , 0x40 , 96},\
{0x7 , 0x42 , 97},\
{0x7 , 0x44 , 98},\
{0x7 , 0x46 , 99}\
}

typedef struct {
    UINT16    rampSet;
} XCV_APC_T;

// ramp factore for PCL 5 - 29 (33 - 5 dBm)
#define EGSM_APC_DEFAULT {\
{0x354},\
{0x2c8},\
{0x258},\
{0x200},\
{0x1bc},\
{0x185},\
{0x15c},\
{0x13a},\
{0x11f},\
{0x109},\
{0x0f6},\
{0x0e7},\
{0x0da},\
{0x0cf},\
{0x0c6},\
}

// ramp factore for PCL 0 - 15 (31 - 0 dBm)
#define DCS_APC_DEFAULT {\
{0x360},\
{0x290},\
{0x1f9},\
{0x1b3},\
{0x183},\
{0x158},\
{0x138},\
{0x11c},\
{0x107},\
{0x0f4},\
{0x0e6},\
{0x0da},\
{0x0d0},\
{0x0c8},\
{0x0c0},\
{0x0ba},\
}

#endif // XCV_H
