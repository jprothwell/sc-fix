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
//  $Name: spal_irq.h $                                                       //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                   //   
//    $Revision: 269 $                                                         //   
//                                                                            //
//------------------------------------------------------------------------------
//                                                                            //
/// @file spal_irq_driver.S                                                   //
/// Not romed IRQ driver to choose to use the handler in INT SRAM instead of  //
/// the one in ROM.                                                           //
//                                                                            //
//------------------------------------------------------------------------------

#ifndef __SPAL_IFC_H__
#define __SPAL_IFC_H__


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"
#include "global_macros.h"

PUBLIC VOID spal_IfcTxFifoReset(VOID);

//======================================================================
// spal_IfcTxFifoSetup
//----------------------------------------------------------------------
/// Configure the Tx dedicate channel to output the data contained in the 
/// Tx fifo (if any) through the Rf Interface.
/// @param pFifo. Pointer to the fifo start address
/// @param sizeFifo. Size of the fifo
//======================================================================
PUBLIC VOID spal_IfcTxFifoSetup(UINT32* pFifo, UINT32 sizeFifo);

//======================================================================
// spal_IfcDisableRf
//----------------------------------------------------------------------
/// Disable the Ifc channel CH2 used for Rf samples 
//======================================================================
PUBLIC VOID spal_IfcDisableRf(VOID);

//======================================================================
// spal_IfcParamRf
//----------------------------------------------------------------------
/// Set the Channel2 parameters for Rf Ifc 
//======================================================================
PUBLIC VOID spal_IfcParamRf(UINT32 mask,
                              UINT32 startAddr,
                              UINT32 endAddr,
                              UINT32 tc,
                              UINT32 control);

#endif // __SPAL_IFC_H__


