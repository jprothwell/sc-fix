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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/include/hal_config.h $ //
//    $Author: huazeng $                                                        // 
//    $Date: 2011-01-11 20:30:56 +0800 (Tue, 11 Jan 2011) $                     //   
//    $Revision: 5305 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_config.h                                                        //
/// That file describes the configuration structure used to set all 
/// configurable parameters in HAL.                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////





// svn propset svn:keywords "HeadURL Author Date Revision" dummy.h



// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================

#ifndef _HAL_CONFIG_H_
#define _HAL_CONFIG_H_

#include "chip_id.h"
#include "hal_pwm.h"
#include "hal_gpio.h"
#include "hal_i2c.h"

// =============================================================================
//  MACROS
// =============================================================================


// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// HAL_CFG_CAM_T
// -----------------------------------------------------------------------------
/// This structure describes the camera configuration for a given target.
/// The first field identify if camera is used.
/// The second and third field is the RemapFlag which identify if camera PDN/RST
/// need to be remapped to other GPIOs instead of default (GPIO_5 and GPIO_4),
/// for the first camera sensor.
/// The fourth and fifth field is the remapped pin when RemapFlag is set, for the
/// first camera sensor.
/// The sixth and seventh fields are used to describe which GPIOs are used for 
/// the PDN/RST lines of the optional second camera sensor. This second camera
/// sensor can only be used on with GPIOs to control its PDN and RST lines.
// =============================================================================
typedef struct
{
    /// \c TRUE if the rear camera is used
    BOOL            camUsed :1;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    /// \c TRUE if CAM_PDN is remapped to other GPIO instead of default(GPIO_5)
    BOOL            camPdnRemapFlag :1;
    /// \c TRUE if CAM_RST is remapped to other GPIO instead of default(GPIO_4)
    BOOL            camRstRemapFlag :1;
#endif // CHIP_ASIC_ID_GREENSTONE
    /// The polarity of the Power DowN line, for the rear sensor.
    BOOL            camPdnActiveH;
    /// The polarity of the Reset line, for the rear sensor.
    BOOL            camRstActiveH;
    /// The remapped GPIO controlling PDN (HAL_GPIO_NONE if not remapped), for the rear sensor.
    HAL_APO_ID_T    camPdnRemap;
    /// The remapped GPIO controlling RST (HAL_GPIO_NONE if not remapped), for the rear sensor.
    HAL_APO_ID_T    camRstRemap;
    /// \c TRUE if the front camera is used
    BOOL            cam1Used :1;
    /// The polarity of the Power DowN line, for the front sensor.    
    BOOL            cam1PdnActiveH;
    /// The polarity of the Reset line, for the front sensor.
    BOOL            cam1RstActiveH;
    /// The remapped GPIO controlling PDN (HAL_GPIO_NONE if not remapped), for the front sensor.
    HAL_APO_ID_T    cam1PdnRemap;
    /// The remapped GPIO controlling RST (HAL_GPIO_NONE if not remapped), for the front sensor.
    HAL_APO_ID_T    cam1RstRemap;
} HAL_CFG_CAM_T;

// =============================================================================
// HAL_CFG_PWM_T
// -----------------------------------------------------------------------------
/// This structure describes the PWM configuration for a given target. 
/// The first field identify which PWL is used for GLOW (if any).
/// The lasts fields tell wether the pin corresponding to PWM output 
/// is actually used as PWM output and not as something else (for 
/// instance as a GPIO). 
// =============================================================================
typedef struct
{
    /// the glowing PWL position
    HAL_PWL_ID_T pwlGlowPosition;
    /// \c TRUE if the PWL0 is used
    BOOL pwl0Used :1;
    /// \c TRUE if the PWL1 is used
    BOOL pwl1Used :1;
    /// \c TRUE if the PWT is used
    BOOL pwtUsed :1;
    /// \c TRUE if the LPG is used
    BOOL lpgUsed :1;
} HAL_CFG_PWM_T;

// =============================================================================
// HAL_CFG_I2C_T
// -----------------------------------------------------------------------------
/// This structure describes the I2C configuration for a given target. The
/// fields tell wether the corresponding I2C pins are actually used
/// for I2C and not as something else (for instance as a GPIO). 
// =============================================================================
typedef struct
{
    /// \c TRUE if the I2C pins are used
    BOOL i2cUsed :1;
    /// Bitrate for the I2C bus.
    HAL_I2C_BPS_T   i2cBps;
    /// \c TRUE if the I2C2 pins are used
    BOOL i2c2Used :1;
    /// Bitrate for the I2C2 bus.
    HAL_I2C_BPS_T   i2c2Bps;
    /// \c TRUE if the I2C3 pins are used
    BOOL i2c3Used :1;
    /// Bitrate for the I2C3 bus.
    HAL_I2C_BPS_T   i2c3Bps;
} HAL_CFG_I2C_T;


// =============================================================================
// HAL_CFG_I2S_T
// -----------------------------------------------------------------------------
/// This structure describes the I2S configuration for a given target. The
/// fields tell wether the corresponding I2S pin is actually used
/// for I2S and not as something else (for instance as a GPIO). 
// =============================================================================
typedef struct
{
    /// \c TRUE if the data out pin is used
    BOOL doUsed :1;
    BOOL :3;
    /// \c TRUE if corresponding input is used
    BOOL di0Used :1;
    BOOL di1Used :1;
} HAL_CFG_I2S_T;


// =============================================================================
// HAL_UART_CONFIG_T
// -----------------------------------------------------------------------------
/// Used to describes a configuration for used pin by an UART for a given target.
// =============================================================================
typedef enum
{
    /// invalid
    HAL_UART_CONFIG_INVALID = 0,

    /// UART is not used
    HAL_UART_CONFIG_NONE,

    /// UART use only data lines (TXD & RXD)
    HAL_UART_CONFIG_DATA,

    /// UART use data and flow control lines (TXD, RXD, RTS & CTS)
    HAL_UART_CONFIG_FLOWCONTROL,

    /// UART use all lines (TXD, RXD, RTS, CTS, RI, DSR, DCD & DTR)
    HAL_UART_CONFIG_MODEM,

    HAL_UART_CONFIG_QTY
} HAL_UART_CONFIG_T;


// =============================================================================
// HAL_CFG_SPI_T
// -----------------------------------------------------------------------------
/// This structure describes the SPI configuration for a given target. The first
/// fields tell wether the pin corresponding to chip select is actually used
/// as a chip select and not as something else (for instance as a GPIO). 
/// Then, the polarity of the Chip Select is given. It is only relevant
/// if the corresponding Chip Select is used as a Chip Select.
/// Finally which pin is used as input, Can be none, one or the other.
/// On most chip configuration the input 0 (di0) is on the output pin: SPI_DIO
// =============================================================================
typedef struct
{
    /// \c TRUE if the corresponding pin is used as a Chip Select.
    BOOL cs0Used :1;
    BOOL cs1Used :1;
    BOOL cs2Used :1;
    BOOL cs3Used :1;
    /// \c TRUE if the first edge is falling
    BOOL cs0ActiveLow :1;
    BOOL cs1ActiveLow :1;
    BOOL cs2ActiveLow :1;
    BOOL cs3ActiveLow :1;
    /// \c TRUE if corresponding input is used
    BOOL di0Used :1;
    BOOL di1Used :1;
} HAL_CFG_SPI_T;

// =============================================================================
// HAL_CFG_EBC_T
// -----------------------------------------------------------------------------
/// This structure describes the EBC configuration for a given target. The first
/// fields tell wether the pin corresponding to chip select is actually used
/// as a chip select and not as something else (for instance as a GPIO). 
// =============================================================================
typedef struct
{
    /// \c TRUE if the corresponding pin is used as a Chip Select.
    BOOL cs2Used :1;
    BOOL cs3Used :1;
    BOOL cs4Used :1;
} HAL_CFG_EBC_T;

#if (CHIP_HAS_GOUDA == 1)
// =============================================================================
// HAL_CFG_GOUDA_T
// -----------------------------------------------------------------------------
/// This structure describes the GOUDA configuration for a given target. 
/// The first fields tell wether the pin corresponding to chip select is 
/// actually used as a chip select and not as something else (for instance
/// as a GPIO). If none are used, the GOUDA is considered unused.
// =============================================================================
typedef struct
{
    /// \c TRUE if the corresponding pin is used as a Chip Select.
    BOOL cs0Used :1;
    BOOL cs1Used :1;
    /// \c TRUE if the LCD data width is 16 bits.
    BOOL lcdData16Bit :1;
    /// \c TRUE if the active LCD reset signal is low.
    BOOL lcdResetActiveLow :1;
    /// \c ID of the pin to reset LCD
    HAL_APO_ID_T lcdResetPin;
} HAL_CFG_GOUDA_T;
#else // (CHIP_HAS_GOUDA != 1)
#ifdef CHIP_HAS_LCDC
// =============================================================================
// HAL_CFG_LCDC_T
// -----------------------------------------------------------------------------
/// This structure describes the LCDC configuration for a given target. 
/// The first fields tell wether the pin corresponding to chip select is 
/// actually used as a chip select and not as something else (for instance
/// as a GPIO). If none are used, the LCDC is considered unused.
// =============================================================================
typedef struct
{
    /// \c TRUE if the corresponding pin is used as a Chip Select.
    BOOL cs0Used :1;
    BOOL cs1Used :1;
} HAL_CFG_LCDC_T;
#endif // CHIP_HAS_LCDC
#endif // (CHIP_HAS_GOUDA != 1)


// =============================================================================
// HAL_CFG_IO_DRIVE_T
// -----------------------------------------------------------------------------
/// This structure describes the IO Drive configuration for a given target. 
// =============================================================================
typedef union
{
    struct
    {
        UINT32 mData:4;
        UINT32 lsbMa:4;
        UINT32 msbMa:4;
        UINT32 ma23:4;
        UINT32 mCs0:4;
        UINT32 mCs1:4;
        UINT32 mCs2:4;
        UINT32 mCtrl:4;
        UINT32 mCtrlWe:4;
        UINT32 mClk:4;
        UINT32 serialLcd:4;
        UINT32 serialLcdSce:4;
        UINT32 xvrDomain:4;
    };
    struct
    {
        UINT32 select1;
        UINT32 select2;
    };
} HAL_CFG_IO_DRIVE_T;


// =============================================================================
// HAL_CFG_CONFIG_T
// -----------------------------------------------------------------------------
/// HAL configuration structure. 
/// This structure is used to setup HAL with regards to the specific 
/// configuration of the board.
/// Enabling a feature such as i2sCfg will set the corresponding pins which 
/// could also be a GPIO to their alternative use, that is I2S for this 
/// example.
/// GPIO Pins not connected must be set in the noConnectPins bitfield, for
/// power saving. They are configured as output and driven to '0'.
/// Pins which are not not connected (ie are connected) but are not used in
/// an alternative mode as I2S, are considered to be used as GPIOs.
// =============================================================================
typedef struct{
    /// Chip version
    UINT32  chipVersion;
    /// RF Clock Frequency
    UINT32  rfClkFreq;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    /// \c TRUE if the DIGRF is used
    BOOL    useDigrf;
    /// \c TRUE if the RF-SPI is not multiplexed (ie. DI and DO are not
    /// on the same wire.) AND input is used!
    BOOL    useRfSpiDi;
#endif // CHIP_ASIC_ID_GREENSTONE
    /// \c TRUE if LPS_CO_1 is used as LPS
    BOOL    useLpsCo1;
    /// Bitfield with '1' for keypad pins used as input on the keypad
    UINT8   keyInMask;
    /// Bitfield with '1' for keypad pins used as output on the keypad
    UINT8   keyOutMask;
    /// PWM Configuration
    HAL_CFG_PWM_T pwmCfg;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    /// \c TRUE if the I2C is used
    BOOL    useI2C;
#else // !CHIP_ASIC_ID_GREENSTONE
    BOOL useUsbBackup;
    BOOL useClk32k;
    HAL_CFG_I2C_T i2cCfg;
#endif // !CHIP_ASIC_ID_GREENSTONE
    /// I2S Configuration
    HAL_CFG_I2S_T i2sCfg;
    /// EBC Configuration
    HAL_CFG_EBC_T ebcCfg;
    /// UART Configuration
#if (CHIP_STD_UART_QTY != 0)
    HAL_UART_CONFIG_T uartCfg[CHIP_STD_UART_QTY];
#endif
    /// SPI Configuration
#if (CHIP_SPI2_CS_QTY == 0)
    HAL_CFG_SPI_T spiCfg[1];
#else
    HAL_CFG_SPI_T spiCfg[2];
    /// \c TRUE if the SPI2 is located on the SD Card Pins (SSD_CLK, SSD_CMD & SDAT_*)
    /// \c FALSE if the SPI2 is located on the GPO Pins (GPO_1 to GPO_5)
    BOOL    spi2UseSdPins;
#endif
#ifdef CHIP_HAS_SDMMC
    /// TRUE if the SD Card interface is used
    BOOL    useSdmmc;
#endif
    /// TRUE if the Camera interface is used
    HAL_CFG_CAM_T   camCfg;

#if (CHIP_HAS_GOUDA == 1)
    HAL_CFG_GOUDA_T goudaCfg;
#else
#ifdef CHIP_HAS_LCDC
    /// TRUE if the Serial LCD interface is used
    BOOL    useSLcdc;
    /// the Parallel LCD interface Configuration
    HAL_CFG_LCDC_T lcdcCfg;
#endif
#endif // (CHIP_HAS_GOUDA == 1)

    /// Bitfield with '1' for physically not connected GPIO pins
    UINT32  noConnectGpio;
    /// Bitfield with '1' for GPIO really used as GPIO
    UINT32  usedGpio;
    /// Bitfield with '1' for GPO really used as GPO
    UINT32  usedGpo;
    /// Bitfield with '1' for TCO really used as TCO
    UINT32  usedTco;

    /// IO Drive Value
    HAL_CFG_IO_DRIVE_T ioDrive;

} HAL_CFG_CONFIG_T;


///@todo
/// - add: ADMUX (check only, in target only ?)
/// - change pwlGlowPosition: new enum (add in hal_pwl): invalid, none, pwl0, pwl1
/// .





// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//  

// =============================================================================
//  FUNCTIONS
// =============================================================================


#endif // _HAL_CONFIG_H_

