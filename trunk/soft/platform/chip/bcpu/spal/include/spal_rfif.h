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
#include "rf_if.h"
#include "global_macros.h"


//------------------------------------------------------------------------------
// Structure
//------------------------------------------------------------------------------

typedef enum {
    SPAL_UNKNOWN_MOD                             = 0x00000000,
    SPAL_GMSK_MOD                                = 0x00000001,
    SPAL_8PSK_MOD                                = 0x00000002
} SPAL_MODULATION_TYPE_T;

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
#if (CHIP_IFC_RF_TX_CHANNEL==1)
//   UINT32 bstIdx;
//   for (bstIdx=0;bstIdx<4;bstIdx++)
//       hwp_rfIf->Tx_Burst_descriptor[bstIdx] = 0;
#endif
    // TODO check the behavior in digRf when the 
    // following disable is commented
    //hwp_rfIf->Ctrl &= ~(RF_IF_ENABLE_ENABLE);
    hwp_rfIf->Ctrl |= (RF_IF_ENABLE_ENABLE);    
    hwp_rfIf->Ctrl |= RF_IF_TX_FIFO_RESET;    
}

//======================================================================
// spal_RfIfWriteCtrl
//----------------------------------------------------------------------
/// write the RfIf Ctrl register.
/// @param ctrl
//======================================================================

INLINE VOID spal_RfIfWriteCtrl(UINT32 ctrl)
{

    hwp_rfIf->Ctrl = ctrl;    
}

//======================================================================
// spal_RfIfTxFifoWrite
//----------------------------------------------------------------------
/// Write to the Rfif Tx fifo. Depending on implementation, this function will
/// directly write to the fifo of the Rfif interface on APB or write in a 
/// software fifo emptied by a dedicated IFC channel.
// 
/// @param pFifo UINT32*. Pointer to the software Tx fifo (if used).
/// @param data UINT3*. Data to be written.
//======================================================================
INLINE VOID spal_RfIfTxFifoWrite(UINT32* pFifo, UINT32 data)
{
#if (CHIP_IFC_RF_TX_CHANNEL==0)
    hwp_rfIf->Buffer  = data;
#else // (CHIP_IFC_RF_TX_CHANNEL)
    *pFifo = data;
#endif // (!CHIP_IFC_RF_TX_CHANNEL)       
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
                                    SPAL_MODULATION_TYPE_T BstMod,
                                    BOOL endOfWin)
{
#if (CHIP_IFC_RF_TX_CHANNEL==1)
    UINT32 bstDesc = (BstMod==SPAL_8PSK_MOD)?RF_IF_MODULATION:0;
    if (endOfWin) bstDesc |= RF_IF_END_BURST;
    bstDesc = bstDesc | RF_IF_NB_SYMBOLS(nbSymbols-1);
    hwp_rfIf->Tx_Burst_descriptor[bstIdx] = bstDesc;
#endif
}

#endif // __SPAL_RFIF_H__

