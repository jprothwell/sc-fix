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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/halp_ebc.h $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file halp_ebc.h                                                          //
/// Prototypes for private EBC functions                                      //
//                                                                            //
//////////////////////////////////////////////////////////////////////////////// 


#ifndef  _HALP_EBC_H_
#define  _HALP_EBC_H_

#include "cs_types.h"
#include "global_macros.h"
#include "mem_bridge.h"
#include "hal_sys.h"
#include "hal_ebc.h"

// ============================================================================
// hal_EbcCsSetup
// ----------------------------------------------------------------------------
/// Configure the CS settings according to the system clock frequency
/// This function configures the CS time and mode settings.
///
///     @param sys_freq System frequency to which adjust 
///     the CS settings
// ============================================================================
PROTECTED VOID hal_EbcCsSetup(HAL_SYS_MEM_FREQ_T sys_freq);


#ifdef CHIP_HAS_EBC_CS2_BUG
/// Use to remember the CS0 timing configuration, as the register
/// cannot be read again.
extern PROTECTED UINT32 g_halCs0TimeReg ;
/// Use to remember the CS0 mode configuration, as the register
/// cannot be read again.
extern PROTECTED UINT32 g_halCs0ModeReg ;
#endif

/// 
///     Array of pointers to the different
///     CS configuration.
///     
///     \c NULL in the i-th place means
///     the CS i is not enabled/used @todo Write that properly.
/// 
extern HAL_EBC_CS_CFG_T* g_halEbcCsConfigArray [EBC_NB_CS];

#endif // HAL_EBC_H
