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
//  $Name:$                                                                   //
//    $Author: admin $                                                                 //
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                   //
//    $Revision: 269 $                                                               //
//                                                                            //
//------------------------------------------------------------------------------
//                                                                            //
/// @file spal_rfif.h
/// rfif functions for SPC and SPP.
//                                                                            //
//------------------------------------------------------------------------------

#ifndef __SPAL_RFIF_H__
#define __SPAL_RFIF_H__

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"
#include "global_macros.h"
#include "spp_gsm.h"


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------



//======================================================================
// spal_RfIfTxFifoReset
//----------------------------------------------------------------------
/// reset the Tx fifo.
//======================================================================

INLINE VOID spal_RfIfTxFifoReset(VOID)
{
    ;
}

//======================================================================
// spal_RfIfWriteCtrl
//----------------------------------------------------------------------
/// write the RfIf Ctrl register.
/// @param ctrl
//======================================================================

INLINE VOID spal_RfIfWriteCtrl(UINT32 ctrl)
{
    ;
}

//======================================================================
// spal_RfIfTxFifoWrite
//----------------------------------------------------------------------
/// Write to the Rfif Tx fifo. Depending on implementation, this function will
/// directly write to the fifo iof the Rfif interface on APB or write in a
/// software fifo emptied by a dedicated IFC channel.
//
/// @param pFifo UINT32*. Pointer to the software Tx fifo (if used).
/// @param data UINT3*. Data to be written.
//======================================================================
INLINE VOID spal_RfIfTxFifoWrite(UINT32* pFifo, UINT32 data)
{
    ;
}




//======================================================================
// spal_RfIfTxFifoWrite
//----------------------------------------------------------------------
//
/// @param bstIdx
/// @param nbSymbols
/// @param BstMod
/// @param endOfWin
//======================================================================
INLINE VOID spal_RfIfTxFifoSetBurst(UINT8 bstIdx,
                                    UINT16 nbSymbols,
                                    SPP_MODULATION_TYPE_T BstMod,
                                    BOOL endOfWin)
{
    ;
}


#endif // __SPAL_RFIF_H__

