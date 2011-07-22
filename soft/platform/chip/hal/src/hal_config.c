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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_config.c $ //
//    $Author: huazeng $                                                        // 
//    $Date: 2011-05-06 20:51:11 +0800 (Fri, 06 May 2011) $                     //   
//    $Revision: 7454 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file   hal_config.c 
/// Implementation of HAL initialization related with the particular instance
/// of the IP.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////




#include "cs_types.h"

#include "chip_id.h"

#include "tgt_hal_cfg.h"

#include "global_macros.h"
#include "gpio.h"
#include "uart.h"
#include "cfg_regs.h"

#include "tcu.h"


#include "hal_pwm.h"
#include "halp_pwm.h"
#include "hal_gpio.h"
#include "halp_gpio.h"
#include "hal_config.h"
#include "hal_rda_audio.h"
#include "hal_gouda.h"

#include "halp_config.h"
#include "halp_sys.h"

#include "halp_debug.h"

#include "pmd_m.h"




// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

CONST HAL_CFG_CONFIG_T* g_halCfg;


// =============================================================================
//  FUNCTIONS
// =============================================================================



// =============================================================================
// hal_BoardSetup
// -----------------------------------------------------------------------------
/// Apply board dependent configuration to HAL
/// @param halCfg Pointer to HAL configuration structure (from the target
/// module).
// ============================================================================
PROTECTED VOID hal_BoardSetup(CONST HAL_CFG_CONFIG_T* halCfg)
{
    UINT32 availableGpio = 0;
    UINT32 availableGpo = 0;
    UINT32 altMux = 0;

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    // Set the IO Drive value first
    hwp_extApb->IO_Drive_Select1 = halCfg -> ioDrive.select1;
    hwp_extApb->IO_Drive_Select2 = halCfg -> ioDrive.select2;
#endif

    // Store the config pointer for later use in hal
    g_halCfg = halCfg;

    // TODO What about the chip version ???

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    // Greenstone

    // computed GPIO usage depending on the used functionality.
    // GPIO 0 to 3 are always GPIO
    availableGpio = HAL_GPIO_BIT(0)
                         | HAL_GPIO_BIT(1)
                         | HAL_GPIO_BIT(2)
                         | HAL_GPIO_BIT(3);
    // computed GPO usage depending on the used functionality.
    // GPO 0 is always GPO
    availableGpo = HAL_GPO_BIT(0);
    //
    // Other Mux configuration
    altMux = 0;

    if (halCfg->useDigrf)
    {
        altMux |= REGS_XVR_SELECT_DIGRF | REGS_STROBE_SELECT_DIGRF_STROBE;
        // check that TCO 7,8,9 are unused
        HAL_ASSERT(!(halCfg->usedTco & (0x7 << 7)), 
                "cannot use DigRf and TCO 7 8 or 9 at the same time (0x%x)",
                (halCfg->usedTco & (0x7 << 7)));
    }
    else
    {
        altMux |= REGS_XVR_SELECT_TCO | REGS_STROBE_SELECT_TCO_8;
    }

    if (halCfg->useRfSpiDi)
    {
        availableGpio |= REGS_MODE_PIN_TCO6_ALT;
        altMux |= REGS_TCO_6_RF_SPI_DI;
        HAL_ASSERT(!(halCfg->usedTco & (1 << 6)),
                "cannot use RF_SPI_DI and TCO_6 at the same time");
    }
    else if (halCfg->usedTco & (1 << 6))
    {
        // TCO 6
        availableGpio |= REGS_MODE_PIN_TCO6_ALT;
        altMux |= REGS_TCO_6_TCO_6;
    }
    else
    {
        // GPIO 31
        availableGpio |= REGS_MODE_PIN_TCO6_GPIO;
        altMux |= REGS_TCO_6_TCO_6;
    }

    if (halCfg->useLpsCo1)
    {
        // LPSCO_1 is used as LPSCO_1.
        availableGpio |= REGS_MODE_PIN_LPSCO_1_ALT;
    }
    else
    {
        // LPSCO_1 is used as GPIO28.
        availableGpio |= REGS_MODE_PIN_LPSCO_1_GPIO;
    }

    // 6th column and line of the keypad can be gpios, PWT or LPG
    if ((halCfg->keyInMask & 0x20) && (halCfg->pwmCfg.pwtUsed))
    {
        HAL_ASSERT(FALSE,
                "pin KEYIN_5 cannot be both used as Key In and PWT!");
    }
    else if (halCfg->keyInMask & 0x20)
    {
        availableGpio |= REGS_MODE_PIN_KEYIN_5_ALT;
        altMux |= REGS_KEYIN_5_KEYIN_5;
    }
    else if (halCfg->pwmCfg.pwtUsed)
    {
        availableGpio |= REGS_MODE_PIN_KEYIN_5_ALT;
        altMux |= REGS_KEYIN_5_PWT;
    }
    else
    {
        availableGpio |= REGS_MODE_PIN_KEYIN_5_GPIO;
    }

    if ((halCfg->keyOutMask & 0x20) && (halCfg->pwmCfg.lpgUsed))
    {
        HAL_ASSERT(FALSE,
                "pin KEYOUT_5 cannot be both used as Key Out and LPG!");
    }
    else if (halCfg->keyOutMask & 0x20)
    {
        availableGpio |= REGS_MODE_PIN_KEYOUT_5_ALT;
        altMux |= REGS_KEYOUT_5_KEYOUT_5;
    }
    else if (halCfg->pwmCfg.lpgUsed)
    {
        availableGpio |= REGS_MODE_PIN_KEYOUT_5_ALT;
        altMux |= REGS_KEYOUT_5_LPG;
    }
    else
    {
        availableGpio |= REGS_MODE_PIN_KEYOUT_5_GPIO;
    }

    if (halCfg->pwmCfg.pwl0Used)
    {
        altMux |= REGS_PWL0_PWL0;
    }
    else
    {
        altMux |= REGS_PWL0_GPO_7;
        availableGpo |= HAL_GPO_BIT(7);
    }

    if (halCfg->pwmCfg.pwl1Used)
    {
        altMux |= REGS_PWL1_PWL1;
    }
    else
    {
        altMux |= REGS_PWL1_GPO_6;
        availableGpo |= HAL_GPO_BIT(6);
    }

    if (halCfg->useI2C)
    {
        availableGpio |= REGS_MODE_PIN_I2C_SDA_ALT 
                      | REGS_MODE_PIN_I2C_SCL_ALT;
    }
    else
    {
        availableGpio |= REGS_MODE_PIN_I2C_SDA_GPIO 
                      | REGS_MODE_PIN_I2C_SCL_GPIO;
    }

    // note: other parts of i2sCfg config are ignores
    if (halCfg->i2sCfg.di1Used)
    {
        availableGpio |= REGS_MODE_PIN_I2S_DI_1_ALT;
    }
    else
    {
        availableGpio |= REGS_MODE_PIN_I2S_DI_1_GPIO;
    }

    // configure pin MA_23 : MA_23 / M_CS_4
    if (halCfg->ebcCfg.cs4Used)
    {
        altMux |= REGS_MEMORY_ADDESS_23_M_CS4;
    }
    else
    {
        altMux |= REGS_MEMORY_ADDESS_23_MA_23;
    }

    // configure pin SLCD_CE : M_CS_3 / SerialLCD_SCE / LCD_CS_1
    if ((halCfg->ebcCfg.cs3Used) && (!halCfg->useSLcdc) && (!halCfg->lcdcCfg.cs1Used))
    {
        altMux |= REGS_SERIAL_LCD_CE_M_CS_3;
    }
    else if ((!halCfg->ebcCfg.cs3Used) && (halCfg->useSLcdc) && (!halCfg->lcdcCfg.cs1Used))
    {
        altMux |= REGS_SERIAL_LCD_CE_SLCD_CE;
    }
    else if ((!halCfg->ebcCfg.cs3Used) && (!halCfg->useSLcdc) && (halCfg->lcdcCfg.cs1Used))
    {
        altMux |= REGS_SERIAL_LCD_CE_LCD_CS_1;
    }
    else
    {
        // none should be used then !
        HAL_ASSERT((!halCfg->ebcCfg.cs3Used) && (!halCfg->useSLcdc) && (!halCfg->lcdcCfg.cs1Used),
                "pin SLCD_CE can be used as only one function! (M_CS_3, SerialLCD_SCE or LCD_CS_1)");
    }

    // configure pin M_CS_2 : M_CS_2 / SPI_CS_2 / LCD_CS_1
    if ((halCfg->ebcCfg.cs2Used) && (!halCfg->spiCfg[0].cs2Used) && (!halCfg->lcdcCfg.cs0Used))
    {
        altMux |= REGS_MEMORY_CS2_M_CS_2;
    }
    else if ((!halCfg->ebcCfg.cs2Used) && (halCfg->spiCfg[0].cs2Used) && (!halCfg->lcdcCfg.cs0Used))
    {
        altMux |= REGS_MEMORY_CS2_SPI_CS_2;
    }
    else if ((!halCfg->ebcCfg.cs2Used) && (!halCfg->spiCfg[0].cs2Used) && (halCfg->lcdcCfg.cs0Used))
    {
        altMux |= REGS_MEMORY_CS2_LCD_CS_0;
    }
    else
    {
        // none should be used then !
        HAL_ASSERT((!halCfg->ebcCfg.cs2Used) && (!halCfg->spiCfg[0].cs2Used) && (!halCfg->lcdcCfg.cs0Used),
                "pin M_CS_2 can be used as only one function! (M_CS_2, SPI_CS_2 or LCD_CS_0)");
    }

    // UART 1 Pin configuration
    switch (halCfg->uartCfg[0])
    {
        case HAL_UART_CONFIG_NONE:
            // force TXD line to '0' to not drain power out
            hwp_uart->CMD_Set = UART_TX_BREAK_CONTROL;
            hwp_uart->ctrl = UART_ENABLE;
        case HAL_UART_CONFIG_DATA:
            availableGpio |= REGS_MODE_PIN_UART1_CTS_GPIO 
                          | REGS_MODE_PIN_UART1_RTS_GPIO;
            altMux |= REGS_UART2_UART2;
            break;
        case HAL_UART_CONFIG_FLOWCONTROL:
            availableGpio |= REGS_MODE_PIN_UART1_CTS_ALT 
                          | REGS_MODE_PIN_UART1_RTS_ALT;
            altMux |= REGS_UART2_UART2;
            break;
        case HAL_UART_CONFIG_MODEM:
            availableGpio |= REGS_MODE_PIN_UART1_CTS_ALT 
                           | REGS_MODE_PIN_UART1_RTS_ALT
                           | REGS_MODE_PIN_UART2_RTS_ALT
                           | REGS_MODE_PIN_UART2_CTS_ALT
                           | REGS_MODE_PIN_UART2_TXD_ALT
                           | REGS_MODE_PIN_UART2_RXD_ALT;
            altMux |= REGS_UART2_UART1;
            HAL_ASSERT(halCfg->uartCfg[1] == HAL_UART_CONFIG_NONE,
                    "UART 2 must be unused to use UART 1 Modem lines.");
            break;
        default:
            HAL_ASSERT(FALSE,
                    "Invalid Uart 1 Configuration (%d).",
                    halCfg->uartCfg[0]);
            break;
    }
    // UART 2 Pin configuration
    if (halCfg->uartCfg[0] != HAL_UART_CONFIG_MODEM)
    {
        switch (halCfg->uartCfg[1])
        {
            case HAL_UART_CONFIG_NONE:
                availableGpio |= REGS_MODE_PIN_UART2_RTS_GPIO
                              | REGS_MODE_PIN_UART2_CTS_GPIO
                              | REGS_MODE_PIN_UART2_TXD_GPIO
                              | REGS_MODE_PIN_UART2_RXD_GPIO;
                break;
            case HAL_UART_CONFIG_DATA:
                availableGpio |= REGS_MODE_PIN_UART2_RTS_GPIO 
                              | REGS_MODE_PIN_UART2_CTS_GPIO
                              | REGS_MODE_PIN_UART2_TXD_ALT
                              | REGS_MODE_PIN_UART2_RXD_ALT;
                break;
            case HAL_UART_CONFIG_FLOWCONTROL:
                availableGpio |= REGS_MODE_PIN_UART2_RTS_ALT
                              | REGS_MODE_PIN_UART2_CTS_ALT
                              | REGS_MODE_PIN_UART2_TXD_ALT
                              | REGS_MODE_PIN_UART2_RXD_ALT;
                break;
            default:
                HAL_ASSERT(FALSE,
                        "Invalid Uart 2 Configuration (%d).",
                        halCfg->uartCfg[0]);
                break;
        }
    }

    // SPI Pins Configuration
    // SPI_CS_2 already muxed with EBC_CS_2
    // SPI_CS_1 or GPIO7
    if (halCfg->spiCfg[0].cs1Used)
    {
        availableGpio |= REGS_MODE_PIN_SPI_CS_1_ALT;
    }
    else
    {
        availableGpio |= REGS_MODE_PIN_SPI_CS_1_GPIO;
    }
    
    // SPI2 Pins Configuration
    if (halCfg->spiCfg[1].cs0Used
      | halCfg->spiCfg[1].cs1Used)
    {
        // SPI 2 used
        if (halCfg->spi2UseSdPins)
        {   // SD Pins
            HAL_ASSERT(!halCfg->useSdmmc,
                    "Cannot use SPI2 on SDMMC pins when SDMMC Controller is used");
            altMux |= REGS_SDMMC_SPI2 | REGS_GPO_GPO | REGS_GPO_1_GPO_1;
            availableGpo |= HAL_GPO_BIT(1)
                         | HAL_GPO_BIT(2)
                         | HAL_GPO_BIT(3)
                         | HAL_GPO_BIT(4)
                         | HAL_GPO_BIT(5);
            //
            availableGpio |= REGS_MODE_PIN_SDMMC_CLK_ALT         // = SPI2_CLK
                          | REGS_MODE_PIN_SDMMC_DATA0_ALT        // = SPI2_DIO
                          | REGS_MODE_PIN_SDMMC_DATA2_GPIO;      // GPIO
            if (halCfg->spiCfg[1].cs0Used)
            {
                availableGpio |= REGS_MODE_PIN_SDMMC_CMD_ALT;    // = SPI2_CS0
            }
            else
            {
                availableGpio |= REGS_MODE_PIN_SDMMC_CMD_GPIO;   // = SPI2_CS0
            }
            if (halCfg->spiCfg[1].cs1Used)
            {
                availableGpio |= REGS_MODE_PIN_SDMMC_DATA3_ALT;  // = SPI2_CS1
            }
            else
            {
                availableGpio |= REGS_MODE_PIN_SDMMC_DATA3_GPIO; // = SPI2_CS1
            }
            if (halCfg->spiCfg[1].di1Used)
            {
                availableGpio |= REGS_MODE_PIN_SDMMC_DATA1_ALT;  // = SPI2_DI1
            }
            else
            {
                availableGpio |= REGS_MODE_PIN_SDMMC_DATA1_GPIO; // = SPI2_DI1
            }

        }
        else
        {   // GPO Pins
            altMux |= REGS_SDMMC_SDMMC | REGS_GPO_SPI2;
            if (halCfg->spiCfg[1].cs1Used)
            {
                altMux |= REGS_GPO_1_SPI2_CS_1;
            }
            else
            {
                altMux |= REGS_GPO_1_GPO_1;
                availableGpo |= HAL_GPO_BIT(1);
            }
        }
    }
    else
    {
        // The SPI2 is not used
        altMux |= REGS_GPO_GPO | REGS_GPO_1_GPO_1;
        availableGpo |= HAL_GPO_BIT(1)
                     | HAL_GPO_BIT(2)
                     | HAL_GPO_BIT(3)
                     | HAL_GPO_BIT(4)
                     | HAL_GPO_BIT(5);
    }

    // SD Card interface
    if (halCfg->useSdmmc)
    {
        altMux |= REGS_SDMMC_SDMMC;
        availableGpio |= REGS_MODE_PIN_SDMMC_CLK_ALT
                      | REGS_MODE_PIN_SDMMC_DATA0_ALT
                      | REGS_MODE_PIN_SDMMC_DATA1_ALT
                      | REGS_MODE_PIN_SDMMC_DATA2_ALT
                      | REGS_MODE_PIN_SDMMC_DATA3_ALT
                      | REGS_MODE_PIN_SDMMC_CMD_ALT;
    }
    else if ((!halCfg->spi2UseSdPins) || (!(halCfg->spiCfg[1].cs0Used
      | halCfg->spiCfg[1].cs1Used)))
    {   
        // SDMMC is not used, SPI 2 does not use SD pin (or is not used at all)
        // put all sdmmc pins as gpio
        availableGpio |= REGS_MODE_PIN_SDMMC_CLK_GPIO
                      | REGS_MODE_PIN_SDMMC_DATA0_GPIO
                      | REGS_MODE_PIN_SDMMC_DATA1_GPIO
                      | REGS_MODE_PIN_SDMMC_DATA2_GPIO
                      | REGS_MODE_PIN_SDMMC_DATA3_GPIO
                      | REGS_MODE_PIN_SDMMC_CMD_GPIO;
    }
    // else here whould be SPI2 used on SD Pins, 
    // this is already covered on the SPI configuration.

    // Camera interface
    if (halCfg->camCfg.camUsed)
    {
        // using camera? put in camera mode by default
        // software can later change it to use DEBUG PORT bus or DAIs
        altMux |= REGS_DEBUG_PORT_CAMERA;
        availableGpio |= REGS_MODE_PIN_CAM_PCLK_ALT
                      | REGS_MODE_PIN_CAM_HREF_ALT
                      | REGS_MODE_PIN_CAM_VSYNC_ALT;
        
        if (halCfg->camCfg.camPdnRemapFlag)
        {
            // REGS_MODE_PIN_CAM_PDN is remapped, this pin is now available as GPIO.
            availableGpio |= REGS_MODE_PIN_CAM_PDN_GPIO;
        }
        else
        {
            availableGpio |= REGS_MODE_PIN_CAM_PDN_ALT;
        }
        
        if (halCfg->camCfg.camRstRemapFlag)
        {
            // REGS_MODE_PIN_CAM_RST is remapped, this pin is now available as GPIO.
            availableGpio |= REGS_MODE_PIN_CAM_RST_GPIO;
        }
        else
        {
            availableGpio |= REGS_MODE_PIN_CAM_RST_ALT;
        }
    }
    else
    {
        availableGpio |= REGS_MODE_PIN_CAM_PCLK_GPIO
                      | REGS_MODE_PIN_CAM_HREF_GPIO
                      | REGS_MODE_PIN_CAM_VSYNC_GPIO
                      | REGS_MODE_PIN_CAM_RST_GPIO
                      | REGS_MODE_PIN_CAM_PDN_GPIO;
    }
    
    // LCD pin configuration
    // Serial LCD interface
    if (halCfg->useSLcdc)
    {
        HAL_ASSERT((!halCfg->lcdcCfg.cs0Used) && (!halCfg->lcdcCfg.cs1Used),
                "Serial LCD cannot be used when Parallel LCD is used");
        // Serial Lcd CE is already muxed with M_CS3 and selected earlyer
        // the altMux was already set at that time, so only configure
        // GPIO muxing here
        availableGpio |= REGS_MODE_PIN_SLCD_DA_ALT
                      | REGS_MODE_PIN_SLCD_DC_ALT
                      | REGS_MODE_PIN_SLCD_CL_ALT;
    }
    else
    {
        // Parallel LCD interface Configuration
        if ((halCfg->lcdcCfg.cs0Used) || (halCfg->lcdcCfg.cs1Used))
        {
            // MUST be in ADMux mode for the ram and flash access
            altMux |= REGS_LSB_MEMORY_ADDR_BUS_PARALLEL_LCD;
            // both CS_0 & SPI CS_2 & M_CS_2 muxing 
            // and CS_1 & Serial lcd CE & M_CS_3 muxing selected earlyer
            // only doing GPIO here
            availableGpio |= REGS_MODE_PIN_SLCD_DA_ALT
                          | REGS_MODE_PIN_SLCD_DC_ALT
                          | REGS_MODE_PIN_SLCD_CL_ALT;
        }
        else
        {
            altMux |= REGS_LSB_MEMORY_ADDR_BUS_MEMORY;
            availableGpio |= REGS_MODE_PIN_SLCD_DA_GPIO
                          | REGS_MODE_PIN_SLCD_DC_GPIO
                          | REGS_MODE_PIN_SLCD_CL_GPIO;
        }
    }

    // TCO mask
    if (halCfg->usedTco & (1 << 3))
    {
        // TCO 3
        altMux |= REGS_TCO_3_TCO_3;
    }
    else
    {
        // GPO 8
        availableGpo |= HAL_GPO_BIT(8);
        altMux |= REGS_TCO_3_GPO_8;
    }
    if (halCfg->usedTco & (1 << 4))
    {
        // TCO 4
        altMux |= REGS_TCO_4_TCO_4;
    }
    else
    {
        // GPO 9
        availableGpo |= HAL_GPO_BIT(9);
        altMux |= REGS_TCO_4_GPO_9;
    }
    if (halCfg->usedTco & (1 << 5))
    {
        // TCO 5
        altMux |= REGS_TCO_5_TCO_5;
    }
    else
    {
        // GPO 10
        availableGpo |= HAL_GPO_BIT(10);
        altMux |= REGS_TCO_5_GPO_10;
    }

    // GPIO mask check
    HAL_ASSERT((~availableGpio & halCfg->usedGpio) == 0,
            "Some used GPIO are not available (0x%x)",
            (~availableGpio & halCfg->usedGpio));
    HAL_ASSERT((~availableGpio & halCfg->noConnectGpio) == 0,
            "Some GPIO declared as not connected are not available (0x%x)",
            (~availableGpio & halCfg->noConnectGpio));

    HAL_ASSERT((halCfg->usedGpio & halCfg->noConnectGpio) == 0,
            "Some GPIO declared as not connected are used (0x%x)",
            (halCfg->usedGpio & halCfg->noConnectGpio));

    // GPO mask check
    HAL_ASSERT((~availableGpo & halCfg->usedGpo) == 0,
            "Some used GPO are not available (0x%x)",
            (~availableGpo & halCfg->usedGpo));


    // set GPIO mode:
    hwp_extApb->GPIO_Mode = availableGpio;
    // set Alt Mux configuration:
    hwp_extApb->Alt_mux_select = altMux;

#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
#ifdef FPGA
// no muxing, do nothing
#else
    // computed GPIO usage depending on the used functionality.
    // GPIO 0 to 1 are always GPIO
    availableGpio = HAL_GPIO_BIT(0) | HAL_GPIO_BIT(1);

    // computed GPO usage depending on the used functionality.
    availableGpo = 0;

    // Other Mux configuration
    altMux = 0;

    if (halCfg->camCfg.camUsed || halCfg->camCfg.cam1Used)
    {
        altMux |= CFG_REGS_DEBUG_PORT_CAMERA;
    }
    else
    {
        altMux |= CFG_REGS_DEBUG_PORT_DEBUG;
    }
    
    // Gouda Pin Configuration
    if (halCfg->goudaCfg.cs0Used)
    {
        altMux |= CFG_REGS_LCD_CS_0_LCD_CS_0;
    }
    else
    {
        altMux |= CFG_REGS_LCD_CS_0_SPI1_CS_2;
    }

    if (halCfg->goudaCfg.cs1Used)
    {
        altMux |= CFG_REGS_LCD_CS_1_LCD_CS_1;
    }
    else
    {
        altMux |= CFG_REGS_LCD_CS_1_GPO_11;
        availableGpo |= HAL_GPO_BIT(11);
    }
    
    // Memory Pin Configuration : MA_24 / M_CS_4
    if (halCfg->ebcCfg.cs4Used)
    {
        altMux |= CFG_REGS_MEMORY_ADDRESS_24_M_CS4;
    }
    else
    {
        altMux |= CFG_REGS_MEMORY_ADDRESS_24_MA_24;
    }

    // SPI1 Pin Configuration
    if (halCfg->spiCfg[0].cs1Used || halCfg->spiCfg[0].di1Used)
    {
        altMux |= CFG_REGS_SPI1_SELECT_SPI1;

        if (!halCfg->spiCfg[0].cs1Used)
        {
            availableGpio |= HAL_GPIO_BIT(14);
        }
        if (!halCfg->spiCfg[0].di1Used)
        {
            availableGpio |= HAL_GPIO_BIT(16);
        }
        
        HAL_ASSERT(!halCfg->i2cCfg.i2c3Used,
                "Cannot use I2C3 when SPI1 is used");
    }
    else
    {
        if (halCfg->i2cCfg.i2c3Used)
        {
            altMux |= CFG_REGS_SPI1_SELECT_I2C_3;
        }
        else
        {
            availableGpio |= HAL_GPIO_BIT(14)
                                    | HAL_GPIO_BIT(16);
        }
    }

    if (halCfg->spiCfg[0].cs0Used)
    {
        altMux |= CFG_REGS_SPI1_CS0_SELECT_SPI1_CS0;
    }
    else
    {
        altMux |= CFG_REGS_SPI1_CS0_SELECT_KEYIN_7;
        availableGpio |= HAL_GPIO_BIT(13);
    }

    // UART 1 Pin configuration
    switch (halCfg->uartCfg[0])
    {
        case HAL_UART_CONFIG_NONE:
            // force TXD line to '0' to not drain power out
            hwp_uart->CMD_Set = UART_TX_BREAK_CONTROL;
            hwp_uart->ctrl = UART_ENABLE;
            // continue with HAL_UART_CONFIG_DATA ...
        case HAL_UART_CONFIG_DATA:
             // use UART1 TXD, RXD
            availableGpio |= HAL_GPIO_BIT(20) 
                                    | HAL_GPIO_BIT(21);
            altMux |= CFG_REGS_UART2_UART2;
            break;
        case HAL_UART_CONFIG_FLOWCONTROL:
             // use UART1 TXD, RXD, CTS, RTS
            altMux |= CFG_REGS_UART2_UART2;
            break;
        case HAL_UART_CONFIG_MODEM:
            // use UART1 TXD, RXD, CTS, RTS, RI, DSR, DCD, DTR
            altMux |= CFG_REGS_UART2_UART1;
            HAL_ASSERT(halCfg->uartCfg[1] == HAL_UART_CONFIG_NONE,
                    "UART 2 must be unused to use UART 1 Modem lines.");
            break;
        default:
            HAL_ASSERT(FALSE,
                    "Invalid Uart 1 Configuration (%d).",
                    halCfg->uartCfg[0]);
            break;
    }
    // UART 2 Pin configuration
    if (halCfg->uartCfg[0] != HAL_UART_CONFIG_MODEM)
    {
        switch (halCfg->uartCfg[1])
        {
            case HAL_UART_CONFIG_NONE:
                availableGpio |= HAL_GPIO_BIT(5)
                                        | HAL_GPIO_BIT(6)
                                        | HAL_GPIO_BIT(7)
                                        | HAL_GPIO_BIT(8);
                break;
            case HAL_UART_CONFIG_DATA:
                // use UART2 TXD, RXD
                availableGpio |= HAL_GPIO_BIT(5) 
                                        | HAL_GPIO_BIT(7);
                break;
            case HAL_UART_CONFIG_FLOWCONTROL:
                // use UART2 TXD, RXD, CTS, RTS
                break;
            default:
                HAL_ASSERT(FALSE,
                        "Invalid Uart 2 Configuration (%d).",
                        halCfg->uartCfg[0]);
                break;
        }
    }

    // PWM Pin Configuration
    if (halCfg->pwmCfg.pwl1Used)
    {
        altMux |= CFG_REGS_PWL1_PWL1;
        HAL_ASSERT(halCfg->useClk32k == FALSE,
                "Cannot use CLK_32K with PWL1");
    }
    else
    {
        if (halCfg->useClk32k)
        {
            altMux |= CFG_REGS_PWL1_CLK_32K;
        }
        else
        {
            altMux |= CFG_REGS_PWL1_GPO_6;
            availableGpo |= HAL_GPO_BIT(6);
        }
    }

    // SD Card Interface
    if (halCfg->useSdmmc)
    {
        altMux |= CFG_REGS_SDMMC_SDMMC;
        // TCO
        if (halCfg->usedTco & (1<<0))
        {
            altMux |= CFG_REGS_TCO_0_TCO_0;
            HAL_ASSERT( !halCfg->spiCfg[1].cs1Used,
                    "Cannot use SPI2_CS_1 when TCO_0 is used");
        }
        else
        {
            if (halCfg->spiCfg[1].cs1Used)
            {
                altMux |= CFG_REGS_TCO_0_SP2_CS_1;
            }
        }
        
        if (halCfg->usedTco & (1<<1))
        {
            altMux |= CFG_REGS_TCO_1_TCO_1;
        }
        else
        {
            altMux |= CFG_REGS_TCO_1_GPO_7;
            availableGpo |= HAL_GPO_BIT(7);
        }
    }
    else
    {
        altMux |= CFG_REGS_SDMMC_DIGRF;
        HAL_ASSERT( (halCfg->usedTco & ((1<<0) | (1<<1))) == 0,
                "Cannot use TCO_0, 1 when DIGRF is used");
        HAL_ASSERT( !halCfg->spiCfg[1].cs1Used,
                "Cannot use SPI2_CS_1 when DIGRF is used");
    }

    // TCO
    if (halCfg->usedTco & (1<<2))
    {
        altMux |= CFG_REGS_TCO_2_TCO_2;
    }
    else
    {
        altMux |= CFG_REGS_TCO_2_GPO_10;
        availableGpo |= HAL_GPO_BIT(10);
    }
    
    if (halCfg->useUsbBackup)
    {
        altMux |= CFG_REGS_GPO_0_USB
                    | CFG_REGS_GPO_4_1_USB
                    | CFG_REGS_GPO_5_USB
                    | CFG_REGS_I2C2_USB;
        
        HAL_ASSERT( (halCfg->keyOutMask & 0xc) == 0,
                "Cannot use KEYOUT_6, 7 when USB backup is used");
        HAL_ASSERT( !halCfg->spiCfg[1].cs0Used && !halCfg->spiCfg[1].cs1Used,
                "Cannot use SPI2 when USB backup is used");
        HAL_ASSERT( !halCfg->i2cCfg.i2c2Used,
                "Cannot use I2C2 when USB backup is used");
    }
    else
    {
        if (halCfg->keyOutMask & 0x40)
        {
            altMux |= CFG_REGS_GPO_0_KEYOUT_6;
        }
        else
        {
            altMux |= CFG_REGS_GPO_0_GPO_0;
            availableGpo |= HAL_GPO_BIT(0);
        }

        // SPI2 Pins Configuration
        if (halCfg->spiCfg[1].cs0Used || halCfg->spiCfg[1].cs1Used)
        {
            altMux |= CFG_REGS_GPO_4_1_SPI2;

            if (!halCfg->spiCfg[1].cs0Used)
            {
                availableGpo |= HAL_GPO_BIT(3);
            }
            
            if (!halCfg->spiCfg[1].di1Used)
            {
                availableGpo |= HAL_GPO_BIT(4);
            }
            
            // SPI 2 used
            HAL_ASSERT(halCfg->spi2UseSdPins == FALSE,
                    "Cannot use SPI2 on SDMMC pins");
        }
        else
        {
            altMux |= CFG_REGS_GPO_4_1_GPO_4_1;
            availableGpo |= HAL_GPO_BIT(1)
                                | HAL_GPO_BIT(2)
                                | HAL_GPO_BIT(3)
                                | HAL_GPO_BIT(4);
        }

        if (halCfg->keyOutMask & 0x80)
        {
            altMux |= CFG_REGS_GPO_5_KEYOUT_7;
        }
        else
        {
            altMux |= CFG_REGS_GPO_5_GPO_5;
            availableGpo |= HAL_GPO_BIT(5);
        }

        if (halCfg->i2cCfg.i2c2Used)
        {
            altMux |= CFG_REGS_I2C2_I2C2;
        }
        else
        {
            availableGpio |= HAL_GPIO_BIT(24);
        }
    }

    // TCO again
    if (halCfg->usedTco & (1<<4))
    {
        altMux |= CFG_REGS_TCO_4_TCO_4;
    }
    else
    {
        altMux |= CFG_REGS_TCO_4_GPO_9;
        availableGpo |= HAL_GPO_BIT(9);
    }

    if (halCfg->usedTco & (1<<3))
    {
        altMux |= CFG_REGS_TCO_3_TCO_3;
    }
    else
    {
        altMux |= CFG_REGS_TCO_3_GPO_8;
        availableGpo |= HAL_GPO_BIT(8);
    }
    
    // 6th column and line of the keypad can be gpios, PWT or LPG
    if ((halCfg->keyOutMask & 0x20) && (halCfg->pwmCfg.lpgUsed))
    {
        HAL_ASSERT(FALSE,
                "pin KEYOUT_5 cannot be both used as Key Out and LPG!");
    }
    else if (halCfg->keyOutMask & 0x20)
    {
        altMux |= CFG_REGS_KEYOUT_5_KEYOUT_5;
    }
    else if (halCfg->pwmCfg.lpgUsed)
    {
        altMux |= CFG_REGS_KEYOUT_5_LPG;
    }
    else
    {
        availableGpio |= HAL_GPIO_BIT(9);
    }

    if ((halCfg->keyInMask & 0x20) && (halCfg->pwmCfg.pwtUsed))
    {
        HAL_ASSERT(FALSE,
                "pin KEYIN_5 cannot be both used as Key In and PWT!");
    }
    else if (halCfg->keyInMask & 0x20)
    {
        altMux |= CFG_REGS_KEYIN_5_KEYIN_5;
    }
    else if (halCfg->pwmCfg.pwtUsed)
    {
        altMux |= CFG_REGS_KEYIN_5_PWT;
    }
    else
    {
        availableGpio |= HAL_GPIO_BIT(10);
    }

    if (halCfg->i2sCfg.doUsed)
    {
        altMux |= CFG_REGS_I2S_DO_I2S_DO;
        HAL_ASSERT( (halCfg->usedTco & (1<<5)) == 0,
                "Cannot use TCO_5 when I2S_DO is used");
    }
    else
    {
        altMux |= CFG_REGS_I2S_DO_TCO_5;
    }

#if (CHIP_HAS_DBG_UART_SW_FLOW_CONTROL == 1)
    availableGpio |= HAL_GPIO_BIT(2)
                            |HAL_GPIO_BIT(3); 
#endif

    if (!halCfg->useLpsCo1)
    {
        availableGpio |= HAL_GPIO_BIT(4);
    }
    
    if (!halCfg->i2sCfg.di1Used)
    {
        availableGpio |= HAL_GPIO_BIT(11);

        if (!halCfg->i2sCfg.di0Used && !halCfg->i2sCfg.doUsed)
        {
            availableGpio |= HAL_GPIO_BIT(17);
        }
    }

    if (!halCfg->spiCfg[0].cs0Used)
    {
        availableGpio |= HAL_GPIO_BIT(13);

        if (!halCfg->spiCfg[0].cs1Used)
        {
            availableGpio |= HAL_GPIO_BIT(12);
            availableGpio |= HAL_GPIO_BIT(15);
        }
    }

    if (!halCfg->ebcCfg.cs2Used)
    {
        availableGpio |= HAL_GPIO_BIT(22);
    }
    
    if (!halCfg->ebcCfg.cs3Used)
    {
        availableGpio |= HAL_GPIO_BIT(23);
    }
    
    if (!halCfg->i2cCfg.i2c2Used)
    {
        availableGpio |= HAL_GPIO_BIT(25);
    }

    if (!halCfg->goudaCfg.lcdData16Bit)
    {
        availableGpio |= HAL_GPIO_BIT(18)
                                | HAL_GPIO_BIT(19)
                                | HAL_GPIO_BIT(26)
                                | HAL_GPIO_BIT(27)
                                | HAL_GPIO_BIT(28)
                                | HAL_GPIO_BIT(29)
                                | HAL_GPIO_BIT(30)
                                | HAL_GPIO_BIT(31);
    }

    if (!halCfg->i2cCfg.i2c2Used)
    {
        availableGpio |= HAL_GPIO_BIT(25);
    }

    // GPIO mask check
    HAL_ASSERT((~availableGpio & halCfg->usedGpio) == 0,
            "Some used GPIO are not available (0x%x)",
            (~availableGpio & halCfg->usedGpio));
    HAL_ASSERT((~availableGpio & halCfg->noConnectGpio) == 0,
            "Some GPIO declared as not connected are not available (0x%x)",
            (~availableGpio & halCfg->noConnectGpio));

    HAL_ASSERT((halCfg->usedGpio & halCfg->noConnectGpio) == 0,
            "Some GPIO declared as not connected are used (0x%x)",
            (halCfg->usedGpio & halCfg->noConnectGpio));

    // GPO mask check
    HAL_ASSERT((~availableGpo & halCfg->usedGpo) == 0,
            "Some used GPO are not available (0x%x)",
            (~availableGpo & halCfg->usedGpo));

    // set GPIO mode:
    hwp_configRegs->GPIO_Mode = availableGpio;
    // set Alt Mux configuration:
    hwp_configRegs->Alt_mux_select = altMux;

    if (halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_IO ||
        halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_O)
    {
        UINT32 lcdResetPin;
        if (halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_IO)
        {
            lcdResetPin = HAL_GPIO_BIT(halCfg->goudaCfg.lcdResetPin.id);
            if ((halCfg->usedGpio & lcdResetPin) == 0)
            {
                HAL_ASSERT(FALSE, "LCD reset pin (0x%08x) is not used as GPIO (0x%08x)",
                                        lcdResetPin, halCfg->usedGpio);
            }
        }
        else // if (halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_O)
        {
            lcdResetPin = HAL_GPO_BIT(halCfg->goudaCfg.lcdResetPin.id);
            if ((halCfg->usedGpo & lcdResetPin) == 0)
            {
                HAL_ASSERT(FALSE, "LCD reset pin (0x%08x) is not used as GPO (0x%08x)",
                                        lcdResetPin, halCfg->usedGpo);
            }
        }
		// Init GPIO/GPO pin after it is configured as usable
        hal_GoudaInitResetPin();
    }
    else if (halCfg->goudaCfg.lcdResetPin.type != HAL_GPIO_TYPE_NONE)
    {
        HAL_ASSERT(FALSE, "LCD reset pin must be GPIO or GPO or none");
    }

#endif

#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE)
// TODO : no I/Omux for now...
#else // CHIP_ASIC_ID
#error This CHIP_ASIC_ID is not supported by hal_config.c
#endif // CHIP_ASIC_ID

    // Common to all chips :
    
    // Set the not connected ones as output and drive 0
    UINT32 noConnectMask = halCfg->noConnectGpio;
    hwp_gpio->gpio_clr = noConnectMask;
    hwp_gpio->gpio_oen_set_out = noConnectMask;

    // --------------------------------------------------
    // As of now, all connected GPIOs, which are as GPIO 
    // or not used at all but connected are configured as 
    // GPIOs in input mode... (Out of reset config)
    // --------------------------------------------------

    // initial GPO values when power on
    hwp_gpio->gpo_clr = availableGpo;

    hal_PwlSetGlowing(halCfg->pwmCfg.pwlGlowPosition);

    // Set the LPSCOs polarity from PMD
    // this is assuming xvc needs V_RF, anyway pal will get the real info
    // from xcv and configure LPS properly.
    hwp_tcu->LPS_PU_Ctrl |= pmd_ConfigureRfLowPower(TRUE, FALSE);

    // Initialize Clock frequency global variables :
    // Out of boot, the system frequency is on the RF clock
    // that is (normally) 26 MHz, but this info is confirmed/given
    // in the configuration structure, so help ourselves !
    g_halSysSystemFreq = halCfg->rfClkFreq;
}

