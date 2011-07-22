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



#ifndef _HAL_LCDC_H_
#define _HAL_LCDC_H_

#include "cs_types.h"
#include "hal_error.h"



/// @defgroup lcdc HAL LCDC Driver
/// This document describes the characteristics of the LCDC module and how 
/// to use them via its Hardware Abstraction Layer API.
///
/// The LCD Controller is a dedicated controller that is capable of driving
/// a parallel or a serial LCD module.
///
/// @{


// =============================================================================
// MACROS
// =============================================================================

/// Number of different set of timings available for the LCDC
/// configuration. This number should be equal to the number
/// of different system frequencies, so that the LCDC interface
/// can be configured optimaly for every system frequency.
#define HAL_LCDC_TIMINGS_QTY    8



// =============================================================================
// TYPES
// =============================================================================

// =============================================================================
// HAL_LCDC_TIMINGS_T
// -----------------------------------------------------------------------------
/// This types describes the timing used by the LCDC interface.
/// An array os those will be needed to describe the LCDC
/// timings for each system frequency.
// =============================================================================
typedef union
{
    struct
    {
        /// RS/RW setup time in clock number
        UINT32 tas:2;
        UINT32 :6;
        /// Control (E, WR/RD) pulse width high, in clock number
        UINT32 tch:6;
        UINT32 :2;
        /// Control (E, WR/RD) pulse width low, in clock number
        UINT32 tcl:6;
        UINT32 :10;
    };

    /// Use this 'reg' field of the union 
    /// to access this timing configuration as
    /// a 32 bits value than can be written to 
    /// a register.
    UINT32 reg;
} HAL_LCDC_TIMINGS_T;



// =============================================================================
// HAL_LCDC_IRQ_HANDLER_T
// -----------------------------------------------------------------------------
/// Type of the user handler function called when a LCDC related interrupt
/// is triggered.
// =============================================================================
typedef VOID (*HAL_LCDC_IRQ_HANDLER_T)(VOID);


// =============================================================================
// HAL_LCDC_BUS_WIDTH_T
// -----------------------------------------------------------------------------
/// Describes the width of the LCD bus.
// =============================================================================
typedef enum
{
    HAL_LCDC_BUS_WIDTH_8_BITS   = 0,
    HAL_LCDC_BUS_WIDTH_9_BITS   = 1,
    HAL_LCDC_BUS_WIDTH_16_BITS  = 2,
    HAL_LCDC_BUS_WIDTH_18_BITS  = 3,

    HAL_LCDC_BUS_WIDTH_QTY
} HAL_LCDC_BUS_WIDTH_T;


// =============================================================================
// HAL_LCDC_CS_T
// -----------------------------------------------------------------------------
/// Defines the Chip Select use to select the LCD screen connected to the 
/// LCDC controller.
// =============================================================================
typedef enum
{
    HAL_LCDC_CS_0 = (1<<0),
    HAL_LCDC_CS_1 = (1<<1),

    HAL_LCDC_CS_QTY
} HAL_LCDC_CS_T;

// =============================================================================
// HAL_LCDC_BUS_KIND_T
// -----------------------------------------------------------------------------
/// Describe the bus kind used by the LCD screen plugged to the controller.
// =============================================================================
typedef enum
{
    HAL_LCDC_BUS_KIND_80,
    HAL_LCDC_BUS_KIND_68,

    HAL_LCDC_BUS_KIND_QTY
} HAL_LCDC_BUS_KIND_T;

// =============================================================================
// HAL_LCDC_CONFIG_T
// -----------------------------------------------------------------------------
/// Describes the configuration of the LCDC interface.
// =============================================================================
typedef struct
{
    HAL_LCDC_BUS_WIDTH_T    busWidth;
    HAL_LCDC_CS_T           cs;
    /// \c FALSE is active low.
    /// \c TRUE is active high.
    BOOL                    csPolarity;
    HAL_LCDC_BUS_KIND_T     busKind;
} HAL_LCDC_CONFIG_T;


// =============================================================================
// FUNCTIONS
// =============================================================================
// =============================================================================
// hal_LcdcOpen
// -----------------------------------------------------------------------------
/// Open the LCDC controller.
/// Parameters are used to describe the screen connexion configuration and 
/// the controller timings to use depending on the system clock frequency.
/// @param config Pointer to the controller configuration.
/// @param timings Controller timing array..
// =============================================================================
PUBLIC VOID hal_LcdcOpen(CONST HAL_LCDC_CONFIG_T* config, CONST HAL_LCDC_TIMINGS_T* timings);


// =============================================================================
// hal_LcdcClose
// -----------------------------------------------------------------------------
/// Close the LCDC controller.
// =============================================================================
PUBLIC VOID hal_LcdcClose(VOID);


// =============================================================================
// hal_LcdcWriteReg
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
PUBLIC HAL_ERR_T hal_LcdcWriteReg(UINT16 addr, UINT16 data);


// =============================================================================
// hal_LcdcReadReg
// -----------------------------------------------------------------------------
/// Write the address and then read a data from the LCD register
/// 
/// @param addr Address of the register to read.
/// @param pData Pointer to where to store the read data.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_LcdcReadReg(UINT16 addr, UINT16* pData);


// =============================================================================
// hal_LcdcWriteCmd
// -----------------------------------------------------------------------------
/// Write a command to the LCD.
///
/// @param addr Command to write.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_LcdcWriteCmd(UINT16 addr);


// =============================================================================
// hal_LcdcWriteData
// -----------------------------------------------------------------------------
/// Write a data to the LCD screen.
///
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_LcdcWriteData(UINT16 data);


// =============================================================================
// hal_LcdcReadData
// -----------------------------------------------------------------------------
/// Read a data to the LCD screen.
///
/// @param pData Pointer to where to store the read data.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_LcdcReadData(UINT16* pData);


// =============================================================================
// hal_LcdcWriteDataBlock
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
PUBLIC HAL_ERR_T hal_LcdcWriteDataBlock(CONST UINT16* pData, UINT32 size, HAL_LCDC_IRQ_HANDLER_T handler);


#endif // _HAL_LCDC_H_


