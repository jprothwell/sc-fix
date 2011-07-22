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
  UINT32 arfcn;       // @ = set arfcn
  UINT32 band;        // @ = set band 
  UINT32 analogGain;  // @ = set analog  gain
  UINT32 digGain;     // @ = set digital gain 
  UINT32 digAfc;      // @ = set digital Afc
  UINT32 rampSet;     // @ = set ramp settings
  UINT32 dacBit;      // @ = set Dac value
}XCV_COMMANDS_T;


// RDA6220 Registers
#define RDA6220_ANALOG_GAIN_REG               0x01
#define RDA6220_DIG_GAIN_REG                  0x02
#define RDA6220_TX_GAIN_SET_REG               0X03 
#define RDA6220_FREQ_CTRL_REG                 0x08
#define RDA6220_AFC_REG                       0x1f
#define RDA6220_SYS_CTRL_REG                  0x30
#define RDA6220_CDAC_REG                      0x19
#define RDA6220_PAGE_REG                      0x3f

#define RDA6220_RAMP_DAC_MASK                 0x1f
#define RDA6220_RAMP_DAC_OFFSET                 13
#define RDA6220_RAMP_FACTOR_MASK              0x7f
#define RDA6220_RAMP_FACTOR_OFFSET               7


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

#define RDA6220_AFC_MID             0x2000
#define RDA6220_AFC_HRANGE          0xa00
#define RDA6220_AFC_MAX             0x3d00
#define RDA6220_AFC_MIN             0x200
//#define RDA6220_AFC_HRANGE          0x1fff
//#define RDA6220_AFC_MAX             0x3fff
//#define RDA6220_AFC_MIN             0x0

#define RDA6220_AFC_MASK            0X3fff
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
#define APC850_QTY 33
//#define APC900_QTY 32
#define APC900_QTY 29 //5 - 33 dB


typedef struct {
    UINT8    analogGain;
    UINT8    digGain;
    UINT16   totGain;
} XCV_RX_GAIN_T;


#define EGSM_AGC_DEFAULT {\
{0x7 , 0x0a ,   5},\
{0x7 , 0x10 ,   6},\
{0x7 , 0x12 ,   7},\
{0x7 , 0x14 ,   8},\
{0x7 , 0x16 ,   9},\
{0x7 , 0x18 ,  10},\
{0x7 , 0x1a ,  11},\
{0x7 , 0x20 ,  12},\
{0x7 , 0x22 ,  13},\
{0x7 , 0x24 ,  14},\
{0x7 , 0x26 ,  15},\
{0x7 , 0x28 ,  16},\
{0x7 , 0x2a ,  17},\
{0x7 , 0x30 ,  18},\
{0x6 , 0x10 ,  19},\
{0x6 , 0x12 ,  20},\
{0x6 , 0x14 ,  21},\
{0x6 , 0x16 ,  22},\
{0x6 , 0x18 ,  23},\
{0x6 , 0x1a ,  24},\
{0x6 , 0x20 ,  25},\
{0x6 , 0x22 ,  26},\
{0x5 , 0x10 ,  27},\
{0x5 , 0x12 ,  28},\
{0x5 , 0x14 ,  29},\
{0x5 , 0x16 ,  30},\
{0x5 , 0x18 ,  31},\
{0x5 , 0x1a ,  32},\
{0x5 , 0x20 ,  33},\
{0x5 , 0x22 ,  34},\
{0x5 , 0x24 ,  35},\
{0x5 , 0x26 ,  36},\
{0x5 , 0x28 ,  37},\
{0x5 , 0x2a ,  38},\
{0x4 , 0x10 ,  39},\
{0x4 , 0x12 ,  40},\
{0x4 , 0x14 ,  41},\
{0x4 , 0x16 ,  42},\
{0x4 , 0x18 ,  43},\
{0x4 , 0x1a ,  44},\
{0x4 , 0x20 ,  45},\
{0x4 , 0x22 ,  46},\
{0x4 , 0x24 ,  47},\
{0x4 , 0x26 ,  48},\
{0x4 , 0x28 ,  49},\
{0x4 , 0x2a ,  50},\
{0x4 , 0x30 ,  51},\
{0x3 , 0x10 ,  52},\
{0x3 , 0x12 ,  53},\
{0x3 , 0x14 ,  54},\
{0x3 , 0x16 ,  55},\
{0x3 , 0x18 ,  56},\
{0x3 , 0x1a ,  57},\
{0x3 , 0x20 ,  58},\
{0x3 , 0x22 ,  59},\
{0x3 , 0x24 ,  60},\
{0x3 , 0x26 ,  61},\
{0x3 , 0x28 ,  62},\
{0x2 , 0x10 ,  63},\
{0x2 , 0x12 ,  64},\
{0x2 , 0x14 ,  65},\
{0x2 , 0x16 ,  66},\
{0x2 , 0x18 ,  67},\
{0x2 , 0x1a ,  68},\
{0x1 , 0x10 ,  69},\
{0x1 , 0x12 ,  70},\
{0x1 , 0x14 ,  71},\
{0x1 , 0x16 ,  72},\
{0x1 , 0x18 ,  73},\
{0x0 , 0x10 ,  74},\
{0x0 , 0x12 ,  75},\
{0x0 , 0x14 ,  76},\
{0x0 , 0x16 ,  77},\
{0x0 , 0x18 ,  78},\
{0x0 , 0x1a ,  79},\
{0x0 , 0x20 ,  80},\
{0x0 , 0x22 ,  81},\
{0x0 , 0x24 ,  82},\
{0x0 , 0x26 ,  83},\
{0x0 , 0x28 ,  84},\
{0x0 , 0x2a ,  85},\
{0x0 , 0x30 ,  86},\
{0x0 , 0x32 ,  87},\
{0x0 , 0x34 ,  88},\
{0x0 , 0x36 ,  89},\
{0x0 , 0x38 ,  90},\
{0x0 , 0x3a ,  91},\
{0x0 , 0x40 ,  92},\
{0x0 , 0x42 ,  93},\
{0x0 , 0x44 ,  94},\
{0x0 , 0x46 ,  95},\
{0x0 , 0x48 ,  96},\
{0x0 , 0x4a ,  97},\
{0x0 , 0x50 ,  98},\
{0x0 , 0x52 ,  99}\
}

#define DCS_AGC_DEFAULT {\
{0x7 , 0x01 , 5 },\
{0x7 , 0x0a , 6 },\
{0x7 , 0x10 , 7 },\
{0x7 , 0x12 , 8 },\
{0x7 , 0x14 , 9 },\
{0x7 , 0x16 , 10},\
{0x7 , 0x18 , 11},\
{0x7 , 0x1a , 12},\
{0x7 , 0x20 , 13},\
{0x7 , 0x22 , 14},\
{0x7 , 0x24 , 15},\
{0x7 , 0x26 , 16},\
{0x7 , 0x28 , 17},\
{0x7 , 0x2a , 18},\
{0x7 , 0x30 , 19},\
{0x7 , 0x32 , 20},\
{0x7 , 0x34 , 21},\
{0x6 , 0x10 , 22},\
{0x6 , 0x12 , 23},\
{0x6 , 0x14 , 24},\
{0x6 , 0x16 , 25},\
{0x6 , 0x18 , 26},\
{0x6 , 0x1a , 27},\
{0x6 , 0x20 , 28},\
{0x5 , 0x10 , 29},\
{0x5 , 0x12 , 30},\
{0x5 , 0x14 , 31},\
{0x5 , 0x16 , 32},\
{0x5 , 0x18 , 33},\
{0x5 , 0x1a , 34},\
{0x5 , 0x20 , 35},\
{0x5 , 0x22 , 36},\
{0x5 , 0x24 , 37},\
{0x5 , 0x26 , 38},\
{0x5 , 0x28 , 39},\
{0x5 , 0x2a , 40},\
{0x4 , 0x10 , 41},\
{0x4 , 0x12 , 42},\
{0x4 , 0x14 , 43},\
{0x4 , 0x16 , 44},\
{0x4 , 0x18 , 45},\
{0x4 , 0x1a , 46},\
{0x4 , 0x20 , 47},\
{0x4 , 0x22 , 48},\
{0x3 , 0x10 , 49},\
{0x3 , 0x12 , 50},\
{0x3 , 0x14 , 51},\
{0x3 , 0x16 , 52},\
{0x3 , 0x18 , 53},\
{0x3 , 0x1a , 54},\
{0x3 , 0x20 , 55},\
{0x3 , 0x22 , 56},\
{0x3 , 0x24 , 57},\
{0x3 , 0x26 , 58},\
{0x3 , 0x28 , 59},\
{0x3 , 0x2a , 60},\
{0x3 , 0x30 , 61},\
{0x2 , 0x10 , 62},\
{0x2 , 0x12 , 63},\
{0x2 , 0x14 , 64},\
{0x2 , 0x16 , 65},\
{0x2 , 0x18 , 66},\
{0x2 , 0x1a , 67},\
{0x2 , 0x20 , 68},\
{0x1 , 0x10 , 69},\
{0x1 , 0x12 , 70},\
{0x1 , 0x14 , 71},\
{0x1 , 0x16 , 72},\
{0x1 , 0x18 , 73},\
{0x1 , 0x1a , 74},\
{0x1 , 0x20 , 75},\
{0x1 , 0x22 , 76},\
{0x0 , 0x10 , 77},\
{0x0 , 0x12 , 78},\
{0x0 , 0x14 , 79},\
{0x0 , 0x16 , 80},\
{0x0 , 0x18 , 81},\
{0x0 , 0x1a , 82},\
{0x0 , 0x20 , 83},\
{0x0 , 0x22 , 84},\
{0x0 , 0x24 , 85},\
{0x0 , 0x26 , 86},\
{0x0 , 0x28 , 87},\
{0x0 , 0x2a , 88},\
{0x0 , 0x30 , 89},\
{0x0 , 0x32 , 90},\
{0x0 , 0x34 , 91},\
{0x0 , 0x36 , 92},\
{0x0 , 0x38 , 93},\
{0x0 , 0x3a , 94},\
{0x0 , 0x40 , 95},\
{0x0 , 0x42 , 96},\
{0x0 , 0x44 , 97},\
{0x0 , 0x46 , 98},\
{0x0 , 0x48 , 99}\
}

typedef struct {
    UINT8    rampSet;
    UINT8    dacBit;
} XCV_APC_T;

#define GSM_850_APC {\
{0x53 , 0x0b},\
{0x54 , 0x0b},\
{0x55 , 0x0b},\
{0x56 , 0x0b},\
{0x58 , 0x0b},\
{0x5a , 0x0b},\
{0x5c , 0x0b},\
{0x5e , 0x0b},\
{0x60 , 0x0b},\
{0x62 , 0x0b},\
{0x65 , 0x0b},\
{0x67 , 0x0b},\
{0x6a , 0x0b},\
{0x6d , 0x0b},\
{0x71 , 0x0b},\
{0x74 , 0x0b},\
{0x28 , 0x11},\
{0x29 , 0x11},\
{0x2b , 0x11},\
{0x2c , 0x11},\
{0x2e , 0x11},\
{0x33 , 0x11},\
{0x36 , 0x11},\
{0x39 , 0x11},\
{0x3c , 0x11},\
{0x40 , 0x11},\
{0x44 , 0x11},\
{0x49 , 0x11},\
{0x4f , 0x11},\
{0x5a , 0x11},\
{0x66 , 0x11},\
{0x74 , 0x11},\
{0x78 , 0x11}\
}

/*#define GSM_900_APC {\
{0x55 , 0x0b},\
{0x56 , 0x0b},\
{0x58 , 0x0b},\
{0x5a , 0x0b},\
{0x5c , 0x0b},\
{0x5e , 0x0b},\
{0x60 , 0x0b},\
{0x62 , 0x0b},\
{0x65 , 0x0b},\
{0x67 , 0x0b},\
{0x6a , 0x0b},\
{0x6d , 0x0b},\
{0x71 , 0x0b},\
{0x74 , 0x0b},\
{0x28 , 0x11},\
{0x29 , 0x11},\
{0x2b , 0x11},\
{0x2c , 0x11},\
{0x2e , 0x11},\
{0x30 , 0x11},\
{0x33 , 0x11},\
{0x36 , 0x11},\
{0x39 , 0x11},\
{0x3c , 0x11},\
{0x40 , 0x11},\
{0x44 , 0x11},\
{0x49 , 0x11},\
{0x4f , 0x11},\
{0x5a , 0x11},\
{0x66 , 0x11},\
{0x64 , 0x11},\
{0x78 , 0x11}\
}*/

// New table for ramp factore & dac bit for 5 - 33 dBm
#define GSM_900_APC {\
{0x1c , 0x11},\
{0x1d , 0x11},\
{0x1e , 0x11},\
{0x1e , 0x11},\
{0x1f , 0x11},\
{0x20 , 0x11},\
{0x21 , 0x11},\
{0x22 , 0x11},\
{0x24 , 0x11},\
{0x25 , 0x11},\
{0x26 , 0x11},\
{0x28 , 0x11},\
{0x2a , 0x11},\
{0x2b , 0x11},\
{0x2e , 0x11},\
{0x30 , 0x11},\
{0x33 , 0x11},\
{0x36 , 0x11},\
{0x39 , 0x11},\
{0x3d , 0x11},\
{0x41 , 0x11},\
{0x46 , 0x11},\
{0x4b , 0x11},\
{0x51 , 0x11},\
{0x58 , 0x11},\
{0x5f , 0x11},\
{0x68 , 0x11},\
{0x73 , 0x11},\
{0x7f , 0x11},\
}












#endif // XCV_H
