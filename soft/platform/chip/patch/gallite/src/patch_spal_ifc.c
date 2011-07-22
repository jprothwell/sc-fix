//------------------------------------------------------------------------------
//              Copyright (C) 2010, Coolsand Technologies, Inc.               
//                            All Rights Reserved                             
//------------------------------------------------------------------------------
/// @file patch_spal_ifc.c
///
/// This file contains BB Ifc Driver functions.
///
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/trunk/patch/gallite/src/patch_spal_ifc.c $ //
//    $Author: huazeng $                                                        // 
//    $Date: 2010-11-01 17:10:31 +0800 (星期一, 2010-11-01) $                     //   
//    $Revision: 3276 $                                                        //   
//------------------------------------------------------------------------------

#ifdef ENABLE_PATCH_SPAL_IFCDISABLERF

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"
#include "bb_ifc.h"
#include "global_macros.h"
#include "spal_mem.h"

//======================================================================
// patch_spal_IfcDisableRf
//----------------------------------------------------------------------
/// Disable the Ifc channel CH2 used for Rf samples 
//======================================================================
PUBLIC VOID SPAL_FUNC_INTERNAL patch_spal_IfcDisableRf(VOID)
{
    hwp_bbIfc->ch2_control = BB_IFC_DISABLE;
    hwp_bbIfc->ch2_int_clear = BB_IFC_END_TC | BB_IFC_HALF_TC;
}

#endif // ENABLE_PATCH_SPAL_IFCDISABLERF
