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
//	$Author: Jixj $                                                        // 
//	$Date: 2008-07-22 11:00:00 +0800 (Tue, 22 Jul 2007) $                     //   
//	$Revision: 7769 $                                                          //   
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
#define KBYTES  1024
#define MBYTES  (1024*KBYTES)

// -------------------------------
// FLASH Mapping
// -------------------------------
#define NUMBER_OF_BANKS 1
#define BANK0_BASE      0

#ifdef DUAL_SPI_FLSH
#define FLASH_SIZE      (8*MBYTES)
#else
#define FLASH_SIZE      (4*MBYTES)
#endif
// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
PUBLIC CONST MEMD_BANK_LAYOUT_T g_memdFlashBankLayout [NUMBER_OF_BANKS]=
 //   __attribute__((section (".memd.globalvars.c"))) =
{
#ifdef DUAL_SPI_FLSH
    { BANK0_BASE, { {2048 ,   4*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } }
#else
    { BANK0_BASE, { {1024 ,   4*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } }
#endif
};

PUBLIC CONST MEMD_FLASH_LAYOUT_T g_memdFlashLayout=
//    __attribute__((section (".memd.globalvars.c"))) =
{
    NUMBER_OF_BANKS, g_memdFlashBankLayout, FLASH_SIZE, USER_DATA_BASE, USER_DATA_SIZE, USER_DATA_BLOCK_SIZE
};

