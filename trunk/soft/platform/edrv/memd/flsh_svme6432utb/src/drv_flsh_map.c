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
//  $HeadURL: http://10.10.100.14/svn/developing/modem2G/trunk/platform/edrv/memd/flsh_s71pl064ja0/src/drv_flsh_map.c $ //
//	$Author: ytt $                                                        // 
//	$Date: 2011-3-21 20:00:04 +0800$                     //   
//	$Revision:  $                                                          //   
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
#define NUMBER_OF_BANKS 1
#define BANK0_BASE      0
#if 0
#define BANK1_BASE      (1*MBYTES)
#define BANK2_BASE      (4*MBYTES)
#define BANK3_BASE      (7*MBYTES)
#endif
#define FLASH_SIZE      (8*MBYTES)

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
#if 0
PUBLIC CONST MEMD_BANK_LAYOUT_T g_memdFlashBankLayout [NUMBER_OF_BANKS]
     =
{
    { BANK0_BASE, { {8 , 8*KBYTES}, {15 , 64*KBYTES}, {0 , 0*KBYTES} } },
    { BANK1_BASE, { {48 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK2_BASE, { {48 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK3_BASE, { {15 , 64*KBYTES}, {8 , 8*KBYTES}, {0 , 0*KBYTES} } }
};
#endif
PUBLIC CONST MEMD_BANK_LAYOUT_T g_memdFlashBankLayout [NUMBER_OF_BANKS]
     =
{
    { BANK0_BASE, { {128 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } }
};

PUBLIC CONST MEMD_FLASH_LAYOUT_T g_memdFlashLayout
     =
{
    NUMBER_OF_BANKS, g_memdFlashBankLayout, FLASH_SIZE, USER_DATA_BASE, USER_DATA_SIZE
};


