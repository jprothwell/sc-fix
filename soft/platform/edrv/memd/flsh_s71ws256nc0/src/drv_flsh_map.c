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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/memd/flsh_s71ws256nc0/src/drv_flsh_map.c $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-08-23 13:47:34 +0800 (Mon, 23 Aug 2010) $                     //   
//	$Revision: 1532 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file drv_flsh_map.c                                                      //
/// That file provides the flah mapping structures.                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



// Bank and sector mapping of the flash chip.

#include "cs_types.h"
#include "memd_m.h"


// =============================================================================
//  MACROS
// =============================================================================
#define KBYTES          1024
#define MBYTES          (1024*KBYTES)

// -------------------------------
// FLASH Mapping
// -------------------------------
#define NUMBER_OF_BANKS 16
#define BANK0_BASE 0
#define BANK1_BASE (2*MBYTES)
#define BANK2_BASE (4*MBYTES)
#define BANK3_BASE (6*MBYTES)
#define BANK4_BASE (8*MBYTES)
#define BANK5_BASE (10*MBYTES)
#define BANK6_BASE (12*MBYTES)
#define BANK7_BASE (14*MBYTES)
#define BANK8_BASE (16*MBYTES)
#define BANK9_BASE (18*MBYTES)
#define BANK10_BASE (20*MBYTES)
#define BANK11_BASE (22*MBYTES)
#define BANK12_BASE (24*MBYTES)
#define BANK13_BASE (26*MBYTES)
#define BANK14_BASE (28*MBYTES)
#define BANK15_BASE (30*MBYTES)

#define FLASH_SIZE      (32*MBYTES)

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PUBLIC CONST MEMD_BANK_LAYOUT_T g_memdFlashBankLayout [NUMBER_OF_BANKS]
     =
{
    { BANK0_BASE,   { {4  , 32*KBYTES},  {15 , 128*KBYTES}, {0 , 0*KBYTES} } },
    { BANK1_BASE,   { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK2_BASE,   { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK3_BASE,   { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK4_BASE,   { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK5_BASE,   { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK6_BASE,   { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK7_BASE,   { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK8_BASE,   { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK9_BASE,   { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK10_BASE,  { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK11_BASE,  { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK12_BASE,  { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK13_BASE,  { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK14_BASE,  { {16 , 128*KBYTES}, {0 , 0*KBYTES},    {0 , 0*KBYTES} } },
    { BANK15_BASE,  { {15 , 128*KBYTES}, {4 , 32*KBYTES},   {0 , 0*KBYTES} } }
};

PUBLIC CONST MEMD_FLASH_LAYOUT_T g_memdFlashLayout
     =
{
    NUMBER_OF_BANKS, g_memdFlashBankLayout, FLASH_SIZE, USER_DATA_BASE, USER_DATA_SIZE, USER_DATA_BLOCK_SIZE
};


