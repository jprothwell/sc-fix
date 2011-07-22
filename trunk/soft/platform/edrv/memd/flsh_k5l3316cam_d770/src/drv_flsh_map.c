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
//
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/memd/flsh_k5l3316cam_d770/src/drv_flsh_map.c $
//	$Author: huazeng $
//	$Date: 2010-10-20 16:24:07 +0800 (Wed, 20 Oct 2010) $
//	$Revision: 3006 $
//
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
#define NUMBER_OF_BANKS 8
#define BANK0_BASE 0
#define BANK1_BASE (512*KBYTES)
#define BANK2_BASE (1024*KBYTES)
#define BANK3_BASE (1536*KBYTES)
#define BANK4_BASE (2048*KBYTES)
#define BANK5_BASE (2560*KBYTES)
#define BANK6_BASE (3072*KBYTES)
#define BANK7_BASE (3584*KBYTES)


#define FLASH_SIZE      (4*MBYTES)

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PUBLIC CONST MEMD_BANK_LAYOUT_T g_memdFlashBankLayout [NUMBER_OF_BANKS] =
{
    { BANK0_BASE, { {8 , 8*KBYTES}, {7 , 64*KBYTES}, {0 , 0*KBYTES} } },
    { BANK1_BASE, { {8 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK2_BASE, { {8 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK3_BASE, { {8 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK4_BASE, { {8 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK5_BASE, { {8 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK6_BASE, { {8 , 64*KBYTES}, {0 , 0*KBYTES}, {0 , 0*KBYTES} } },
    { BANK7_BASE, { {7 , 64*KBYTES}, {8 , 8*KBYTES}, {0 , 0*KBYTES} } }
};

PUBLIC CONST MEMD_FLASH_LAYOUT_T g_memdFlashLayout =
{
    NUMBER_OF_BANKS, g_memdFlashBankLayout, FLASH_SIZE, USER_DATA_BASE, USER_DATA_SIZE, USER_DATA_BLOCK_SIZE
};


