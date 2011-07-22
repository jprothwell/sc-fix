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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_lcdc.h                                                          //
///                                                                           //
/// This file decribes the lcd controller device driver API.                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#ifndef _HAL_SLCDC_H_
#define _HAL_SLCDC_H_

#include "cs_types.h"
#include "hal_error.h"



/// @defgroup lcdc HAL SLCDC Driver
/// This document describes the characteristics of the SLCDC module and how 
/// to use them via its Hardware Abstraction Layer API.
///
/// The SLCD Controller is a dedicated controller that is capable of driving
/// a serial LCD module 
///
/// @{


// =============================================================================
// MACROS
// =============================================================================


// =============================================================================
// TYPES
// =============================================================================


// =============================================================================
// HAL_SLCDC_IRQ_HANDLER_T
// -----------------------------------------------------------------------------
/// Type of the user handler function called when a LCDC related interrupt
/// is triggered.
// =============================================================================
typedef VOID (*HAL_SLCDC_IRQ_HANDLER_T)(VOID);


// =============================================================================
// HAL_SLCDC_BUS_KIND_T
// -----------------------------------------------------------------------------
/// Describe the bus kind used by the SLCD screen plugged to the controller.
// =============================================================================
typedef enum
{
    HAL_SLCDC_BUS_KIND_3,
    HAL_SLCDC_BUS_KIND_4,

    HAL_SLCDC_BUS_KIND_QTY
} HAL_SLCDC_BUS_KIND_T;

// =============================================================================
// HAL_SLCDC_CONFIG_T
// -----------------------------------------------------------------------------
/// Describes the configuration of the LCDC interface.
// =============================================================================
typedef struct
{
    // Requested Serial clock frequency in Hz
    UINT32 SclkFrequencyReq;
    // bus kind can be 3 or 4 wire bus
    HAL_SLCDC_BUS_KIND_T     busKind;
    /// \c FALSE is active low.
    /// \c TRUE is active high.
    BOOL                    csPolarity;
} HAL_SLCDC_CONFIG_T;


// =============================================================================
// FUNCTIONS
// =============================================================================
// =============================================================================
// hal_SlcdcOpen
// -----------------------------------------------------------------------------
/// Open the SLCDC controller.
/// Parameters are used to describe the screen connexion configuration and 
/// the controller timings to use depending on the system clock frequency.
/// @param config Pointer to the controller configuration.
/// @param timings Controller timing array..
// =============================================================================
PUBLIC VOID hal_SlcdcOpen(CONST HAL_SLCDC_CONFIG_T* config);


// =============================================================================
// hal_SlcdcClose
// -----------------------------------------------------------------------------
/// Close the LCDC controller.
// =============================================================================
PUBLIC VOID hal_SlcdcClose(VOID);


// =============================================================================
// hal_SlcdcWriteReg
// -----------------------------------------------------------------------------
/// Write a command and then a data to the LCD screen (Or a value at a register
/// address, depending on the point of view).
/// 
/// @param addr Address of the register to write, or command to send
/// to the LCD screen
/// @param data Corresponding data to write to the LCD.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_SlcdcWriteReg(UINT16 addr, UINT16 data);


// =============================================================================
// hal_SlcdcWriteCmd
// -----------------------------------------------------------------------------
/// Write a command to the LCD.
///
/// @param addr Command to write.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_SlcdcWriteCmd(UINT16 addr);


// =============================================================================
// hal_SlcdcWriteData
// -----------------------------------------------------------------------------
/// Write a data to the LCD screen.
///
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_SlcdcWriteData(UINT16 data);


// =============================================================================
// hal_SlcdcWriteDataBlock
// -----------------------------------------------------------------------------
/// Send a block of pixel data to the LCD screen.
/// WARNING: only even pixel number are supported, that is to say size must
/// be a mutiple of 4 bytes.
/// 
/// @param pData Pointer to the pixel buffer, to send to the screen.
/// @param size Size of the pixel buffer, in bytes. (Multiple of 4)
/// @param handler If not NULL, user function that is called when the transfer
/// is finished.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_SlcdcWriteDataBlock(CONST UINT16* pData, UINT32 size, HAL_SLCDC_IRQ_HANDLER_T handler);


#endif // _HAL_SLCDC_H_


