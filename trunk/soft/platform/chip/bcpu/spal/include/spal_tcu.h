//------------------------------------------------------------------------------
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
//------------------------------------------------------------------------------
//                                                                            //
//  $Name$                                                                    //
//    $Author: kongdq $                                                                  // 
//    $Revision: 6672 $                                                                //   
//                                                                            //
//------------------------------------------------------------------------------
//                                                                            //
/// @file spal_tcu.h                                                          
/// tcu register write and read functions  
//                                                                            //
//------------------------------------------------------------------------------

#ifndef __SPAL_TCU_H__
#define __SPAL_TCU_H__

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"
#include "tcu.h"
#include "global_macros.h"


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------



//======================================================================
// spal_TcuReadEnable()
//----------------------------------------------------------------------
/// this function ors the value of the Enable_Event register with the
/// value of event parameter.
//
/// @param event
//======================================================================

INLINE VOID spal_TcuEnableEvent(UINT32 event)
{
    hwp_tcu->Enable_Event |= event;
}

//======================================================================
// spal_TcuWriteDisableEvent()
//----------------------------------------------------------------------
/// this function writes the Disable_Event register.
//
/// @param event
//======================================================================

INLINE VOID spal_TcuDisableEvent(UINT32 event)
{
    hwp_tcu->Disable_Event = event;
}


// =============================================================================
// spal_TcuSetTco
// -----------------------------------------------------------------------------
/// This function sets the corresponding TCO line to its
/// active level (which depends on TCO's polarity)
/// @param tcoNumber : TCO bitmap to be set
// =============================================================================
INLINE VOID  spal_TcuSetTco(UINT32 BitMap)
{
    hwp_tcu->Set_TCO = BitMap;
}



// =============================================================================
// spal_TcuClrTco
// -----------------------------------------------------------------------------
/// This function sets the corresponding TCO line to its
/// inactive level (which depends on TCO's polarity)
/// @param tcoNumber : TCO bitmap to be reset
// =============================================================================
INLINE VOID spal_TcuClrTco(UINT32 BitMap)
{
    hwp_tcu->Clr_TCO = BitMap;
}



#endif // __SPAL_TCU_H__
