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
//  $HeadURL: http://10.10.100.14/svn/developing/modem2G/trunk/platform/edrv/memd/flsh_k5l2731caa_d770/src/drv_flsh_map.c $ //
//	$Author: ytt $                                                        // 
//	$Date: 2010-08-22 11:00:00 +0800 (Tue, 22 Jul 2007) $                     //   
//	$Revision:   $                                                          //   
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
#include "mem_id.h"


// =============================================================================
//  MACROS
// =============================================================================
#define KBYTES 1024
#define MBYTES (1024*KBYTES)

// -------------------------------
// FLASH Mapping
// -------------------------------
#define NUMBER_OF_BANKS 1
#define BANK0_BASE 0

#define FLASH_SIZE      (8*MBYTES)

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

// ******************* following is flsh_en71gl032a0 macro *******************//

// =============================================================================
//  GLOBAL VARIABLES    flsh_en71gl032a0
// =============================================================================

// Bank and sector mapping of the flash chip.
#define NUMBER_OF_BANKS_EN71GL064B0 4
#define BANK0_BASE_1 0
#define BANK1_BASE_1 (1*MBYTES)
#define BANK2_BASE_1 (4*MBYTES)
#define BANK3_BASE_1 (7*MBYTES)

#define FLASH_SIZE_1      (8*MBYTES)


PUBLIC CONST MEMD_BANK_LAYOUT_T g_memdFlashBankLayout_en71pl064b0 [NUMBER_OF_BANKS_EN71GL064B0]=
{
    { BANK0_BASE_1, { {8 ,   8*KBYTES}, {15 , 64*KBYTES}, {0 , 0*KBYTES} } },
    { BANK1_BASE_1, { {48 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK2_BASE_1, { {48 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK3_BASE_1, { {15 , 64*KBYTES}, {8 , 8*KBYTES}, {0 , 0*KBYTES} } }
};

PUBLIC CONST MEMD_FLASH_LAYOUT_T g_memdFlashLayout_en71pl064b0=
{
    NUMBER_OF_BANKS_EN71GL064B0, g_memdFlashBankLayout_en71pl064b0, FLASH_SIZE_1, USER_DATA_BASE, USER_DATA_SIZE, USER_DATA_BLOCK_SIZE
};


// ******************* following is flsh_m36cow5040tozsp macro *******************//
#define NUMBER_OF_BANKS_M36COW6050TOZSP 1
#define BANK0_BASE_2 0

#define FLASH_SIZE_2      (8*MBYTES)

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PUBLIC CONST MEMD_BANK_LAYOUT_T g_memdFlashBankLayout_m36cow6050tozsp [NUMBER_OF_BANKS_M36COW6050TOZSP]=
{
    { BANK0_BASE_2, {{127 , 64*KBYTES},  {8 , 8*KBYTES},  {0 , 0*KBYTES} } }
};

PUBLIC CONST MEMD_FLASH_LAYOUT_T g_memdFlashLayout_m36cow6050tozsp=
{
    1, g_memdFlashBankLayout_m36cow6050tozsp, FLASH_SIZE_2, USER_DATA_BASE, USER_DATA_SIZE, USER_DATA_BLOCK_SIZE
};

// ******************* following is flsh_en71pl032a0   macro *******************//

// =============================================================================
//  MACROS
// =============================================================================
// -------------------------------
// FLASH Mapping
// -------------------------------


PUBLIC CONST FLASH_AUTOSELECTOR_ID g_flash_ID [FLASH_NAME_QTY]=
{
    {0x007f,0x001c,0x227e,0x220c,0x2201,4,FLASH_EN71PL064B0},
    {0x0020,0x0000,0x8848,0x0000,0x0000,2,FLASH_M36COW6050TOZSP},
    {0x0000,0x0000,0x0000,0x0000,0x0000,2,FLASH_S71PL064JA0}, 
    {0x0001,0x0000,0x227e,0x2202,0x2201,3,FLASH_S71PL064JB0},  
};







