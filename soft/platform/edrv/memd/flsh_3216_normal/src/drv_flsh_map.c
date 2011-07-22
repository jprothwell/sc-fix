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

#define FLASH_SIZE      (4*MBYTES)

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

// ******************* following is flsh_en71gl032a0 macro *******************//

// =============================================================================
//  GLOBAL VARIABLES    flsh_en71gl032a0
// =============================================================================

// Bank and sector mapping of the flash chip.
#define NUMBER_OF_BANKS_EN71GL032A0 1
#define BANK0_BASE_1 0
#define FLASH_SIZE_1      (4*MBYTES)


PUBLIC CONST MEMD_BANK_LAYOUT_T g_memdFlashBankLayout_en71gl032a0 [NUMBER_OF_BANKS_EN71GL032A0]=
{
    { BANK0_BASE_1, { {8 ,   8*KBYTES}, {62 , 64*KBYTES}, {8 , 8*KBYTES} } }
};

PUBLIC CONST MEMD_FLASH_LAYOUT_T g_memdFlashLayout_en71gl032a0=
{
    NUMBER_OF_BANKS_EN71GL032A0, g_memdFlashBankLayout_en71gl032a0, FLASH_SIZE_1, USER_DATA_BASE, USER_DATA_SIZE, USER_DATA_BLOCK_SIZE
};


// ******************* following is flsh_m36cow5040tozsp macro *******************//
#define NUMBER_OF_BANKS_M36COW5040TOZSP 1
#define BANK0_BASE_2 0

#define FLASH_SIZE_2      (4*MBYTES)

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PUBLIC CONST MEMD_BANK_LAYOUT_T g_memdFlashBankLayout_m36cow5040tozsp [NUMBER_OF_BANKS_M36COW5040TOZSP]=
{
    { BANK0_BASE_2, {{63 , 64*KBYTES},  {8 , 8*KBYTES},  {0 , 0*KBYTES} } }
};

PUBLIC CONST MEMD_FLASH_LAYOUT_T g_memdFlashLayout_m36cow5040tozsp=
{
    1, g_memdFlashBankLayout_m36cow5040tozsp, FLASH_SIZE_2, USER_DATA_BASE, USER_DATA_SIZE, USER_DATA_BLOCK_SIZE
};

// ******************* following is flsh_en71pl032a0   macro *******************//

// =============================================================================
//  MACROS
// =============================================================================
// -------------------------------
// FLASH Mapping
// -------------------------------
#define NUMBER_OF_BANKS_EN71PL032A0 4
#define BANK0_BASE_3 0
#define BANK1_BASE_3 (512*KBYTES)
#define BANK2_BASE_3 (2048*KBYTES)
#define BANK3_BASE_3 (3584*KBYTES)

#define FLASH_SIZE_3      (4*MBYTES)

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PUBLIC CONST MEMD_BANK_LAYOUT_T g_memdFlashBankLayout_en71pl032a0 [NUMBER_OF_BANKS_EN71PL032A0]=
{
    { BANK0_BASE_3, { {8 ,   8*KBYTES}, {7 , 64*KBYTES}, {0 , 0*KBYTES} } },
    { BANK1_BASE_3, { {24 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK2_BASE_3, { {24 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK3_BASE_3, { {7 , 64*KBYTES}, {8 , 8*KBYTES}, {0 , 0*KBYTES} } }
};

PUBLIC CONST MEMD_FLASH_LAYOUT_T g_memdFlashLayout_en71pl032a0=
{
    NUMBER_OF_BANKS_EN71PL032A0, g_memdFlashBankLayout_en71pl032a0, FLASH_SIZE_3, USER_DATA_BASE, USER_DATA_SIZE, USER_DATA_BLOCK_SIZE
};


PUBLIC CONST FLASH_AUTOSELECTOR_ID g_flash_ID [FLASH_NAME_QTY]=
{
    {0x007f,0x001c,0x227e,0x220a,0x2201,4,FLASH_EN71GL032A0},
    {0x007f,0x001c,0x227e,0x220a,0x2201,4,FLASH_EN71PL032A0},
    {0x0020,0x0000,0x88ba,0x0000,0x0000,2,FLASH_M36COW5040TOZSP},
    {0x00c2,0x0000,0x22a7,0x0000,0x0000,2,FLASH_SVME3216UTA}, 
    {0x007f,0x008c,0x22f6,0x0000,0x0000,2,FLASH_FM91L03216UA},  
};







