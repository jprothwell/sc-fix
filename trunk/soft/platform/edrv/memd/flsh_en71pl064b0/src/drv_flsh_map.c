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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/memd/flsh_en71pl064b0/src/drv_flsh_map.c $ //
//	$Author: yanshengzhang $                                                        // 
//	$Date: 2011-01-13 17:23:18 +0800 (Thu, 13 Jan 2011) $                     //   
//	$Revision: 5374 $                                                          //   
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
#define KBYTES 1024
#define MBYTES (1024*KBYTES)

// -------------------------------
// FLASH Mapping
// -------------------------------
#define NUMBER_OF_BANKS 4
#define BANK0_BASE 0
#define BANK1_BASE (1*MBYTES)
#define BANK2_BASE (4*MBYTES)
#define BANK3_BASE (7*MBYTES)

#define FLASH_SIZE      (8*MBYTES)

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PUBLIC CONST MEMD_BANK_LAYOUT_T g_memdFlashBankLayout [NUMBER_OF_BANKS] =
{
    { BANK0_BASE, { {8 ,   8*KBYTES}, {15 , 64*KBYTES}, {0 , 0*KBYTES} } },
    { BANK1_BASE, { {48 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK2_BASE, { {48 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK3_BASE, { {15 , 64*KBYTES}, {8 , 8*KBYTES}, {0 , 0*KBYTES} } }
};

PUBLIC CONST MEMD_FLASH_LAYOUT_T g_memdFlashLayout =
{
    NUMBER_OF_BANKS, g_memdFlashBankLayout, FLASH_SIZE, USER_DATA_BASE, USER_DATA_SIZE, USER_DATA_BLOCK_SIZE
};


