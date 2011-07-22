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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spal/include/spal_mem.h $ //
//  $Author: admin $                                                        // 
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//  $Revision: 269 $                                                         //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file spal_mem.h
/// This file memory related macros.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _SPAL_MEM_H_
#define _SPAL_MEM_H_

#include "cs_types.h"



// =============================================================================
// SPAL_FUNC_INTERNAL
// -----------------------------------------------------------------------------
/// Macro used to put a specific function in internal SRAM
// =============================================================================
#define SPAL_FUNC_INTERNAL   __attribute__((section(".sramtext")))


// =============================================================================
// SPAL_DATA_INTERNAL
// -----------------------------------------------------------------------------
/// Macro used to put some specific variables in internal SRAM
// =============================================================================
#define SPAL_DATA_INTERNAL   __attribute__((section(".sramdata")))


// =============================================================================
// SPAL_UNCACHED_DATA_INTERNAL
// -----------------------------------------------------------------------------
/// Macro used to put some specific variables in internal SRAM
// =============================================================================
#define SPAL_UNCACHED_DATA_INTERNAL   __attribute__((section(".sramucdata")))


// =============================================================================
// SPAL_BBSRAM_GLOBAL
// -----------------------------------------------------------------------------
/// Macro used to put some specific data in the ".bbsram_globals" section in 
/// BBSRAM.
// =============================================================================
#define  SPAL_BBSRAM_GLOBAL  __attribute__((section(".bbsram_globals")))




///  @} <- End of the mem_map group 


#endif

