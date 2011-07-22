////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2008, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/patch/greenstone/src/patch_open.c $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                        //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//       
/// @file patch_open.c
/// That file implement the patch_Open function
//
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "patch_m.h"

// =============================================================================
//  FUNCTIONS
// =============================================================================

// declare patched function as external and (CONST VOID (*)(VOID)) to avoid a cast
extern CONST VOID bb_prog_rx_win(VOID);
extern CONST VOID patch_bb_prog_rx_win(VOID);
extern CONST VOID spp_SearchFcch(VOID);
extern CONST VOID patch_spp_SearchFcch(VOID);

// =============================================================================
// patch_Open
// -----------------------------------------------------------------------------
/// This function install all the hardware patch available for the current chip
// =============================================================================
PUBLIC VOID patch_Open(VOID)
{
    patch_ReplaceFunction(bb_prog_rx_win, patch_bb_prog_rx_win);
#ifdef PATCH_SPP_USE_OPTIMIZED_FCCH_DETECTION
    patch_ReplaceFunction(spp_SearchFcch, patch_spp_SearchFcch);
#endif
}

