//==============================================================================
//                                                                              
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.              
//                            All Rights Reserved                               
//                                                                              
//      This source code is the property of Coolsand Technologies and is        
//      confidential.  Any  modification, distribution,  reproduction or        
//      exploitation  of  any content of this file is totally forbidden,        
//      except  with the  written permission  of  Coolsand Technologies.        
//                                                                              
//==============================================================================
//                                                                              
//    THIS FILE WAS GENERATED FROM ITS CORRESPONDING XML VERSION WITH COOLXML.  
//                                                                              
//                       !!! PLEASE DO NOT EDIT !!!                             
//                                                                              
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/regs.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _REGS_H_
#define _REGS_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'regs'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
#define GREENSTONE_GPIO_ONLY_NB                  (4)
#define GREENSTONE_GPIO_NB                       (32)
#define GREENSTONE_GPO_NB                        (11)
#define PROD_ID                                  (0X1208)

typedef enum {
    PIN_CAM_RST                                 = 0x00000004,
    PIN_CAM_PDN                                 = 0x00000005,
    PIN_I2S_DI_1                                = 0x00000006,
    PIN_SPI_CS_1                                = 0x00000007,
    PIN_SLCD_DA                                 = 0x00000008,
    PIN_SLCD_DC                                 = 0x00000009,
    PIN_I2C_SDA                                 = 0x0000000A,
    PIN_I2C_SCL                                 = 0x0000000B,
    PIN_SLCD_CL                                 = 0x0000000C,
    PIN_CAM_PCLK                                = 0x0000000D,
    PIN_CAM_HREF                                = 0x0000000E,
    PIN_CAM_VSYNC                               = 0x0000000F,
    PIN_UART2_RTS                               = 0x00000010,
    PIN_UART2_CTS                               = 0x00000011,
    PIN_UART2_TXD                               = 0x00000012,
    PIN_UART2_RXD                               = 0x00000013,
    PIN_UART1_CTS                               = 0x00000014,
    PIN_UART1_RTS                               = 0x00000015,
    PIN_SDMMC_CLK                               = 0x00000016,
    PIN_SDMMC_DATA0                             = 0x00000017,
    PIN_SDMMC_DATA1                             = 0x00000018,
    PIN_SDMMC_DATA2                             = 0x00000019,
    PIN_SDMMC_DATA3                             = 0x0000001A,
    PIN_SDMMC_CMD                               = 0x0000001B,
    PIN_LPSCO_1                                 = 0x0000001C,
    PIN_KEYOUT_5                                = 0x0000001D,
    PIN_KEYIN_5                                 = 0x0000001E,
    PIN_TCO6                                    = 0x0000001F
} GPIO_MAPPING_T;


// =============================================================================
//  TYPES
// =============================================================================
// ============================================================================
// REGS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_EXT_APB_BASE            0x00B97000


typedef volatile struct
{
    REG32                          CHIP_ID;                      //0x00000000
    /// This register contain the synthesis date and version
    REG32                          Build_Version;                //0x00000004
    /// Setting bit n to '1' selects GPIO Usage for PAD connected to GPIOn. Setting
    /// bit n to '0' selects Alt.
    REG32                          GPIO_Mode;                    //0x00000008
    REG32                          Alt_mux_select;               //0x0000000C
    REG32                          IO_Drive_Select1;             //0x00000010
    REG32                          IO_Drive_Select2;             //0x00000014
    /// Register protected by RiskyReg_Write_Unlocked_H (Cf. Sys_Ctrl).
    REG32                          ExtPll_Ctrl;                  //0x00000018
    /// PLL Output Clock Frequency Setting 
    /// FOUT = [FIN * (NF * 2)] / [(NR * 2) * NO] 
    /// Meanwhile, the following constraints must be followed : <ul> <li>800KHz &lt;
    /// Fref &lt; 8MHz</li> <li>200MHz &lt; FVCO &lt; 500MHz, FVCO > 250MHz is preferred</li>
    /// </ul> where 
    /// PLL's comparison frequency Fref = FIN/(NR*2). 
    /// PLL's VCO frequency FVCO = FIN*(NF*2)/(NR*2).
    REG32                          ExtPll_Config;                //0x0000001C
    REG32                          ExtPll_Dbg_Clk;               //0x00000020
    REG32                          CLK_CAMERA_OUT_CTRL;          //0x00000024
    REG32                          STEREO_DAC_CTRL_SET;          //0x00000028
    REG32                          STEREO_DAC_CTRL_CLR;          //0x0000002C
    REG32                          STEREO_DAC_DIG_CTRL;          //0x00000030
    REG32                          STEREO_DAC_ANA_CTRL;          //0x00000034
    REG32                          STEREO_DAC_MIXER_HP_GAIN;     //0x00000038
    REG32                          STEREO_DAC_HP_GAIN_L;         //0x0000003C
    REG32                          STEREO_DAC_HP_GAIN_R;         //0x00000040
    REG32                          STEREO_DAC_REF_GEN;           //0x00000044
    /// Selects I/O muxing for ACCO/STEREO DAC/USB tests and USB Backup port. 
    /// Note that theses settings can be overidden in TST_H mode by pad driving.
    REG32                          IO_TEST_MODE;                 //0x00000048
} HWP_REGS_T;

#define hwp_extApb                  ((HWP_REGS_T*) KSEG1(REG_EXT_APB_BASE))


//CHIP_ID
#define REGS_METAL_ID(n)            (((n)&0x3FF)<<0)
#define REGS_METAL_ID_MASK          (0x3FF<<0)
#define REGS_METAL_ID_SHIFT         (0)
#define REGS_BOND_ID(n)             (((n)&7)<<12)
#define REGS_BOND_ID_MASK           (7<<12)
#define REGS_BOND_ID_SHIFT          (12)
#define REGS_PROD_ID(n)             (((n)&0xFFFF)<<16)
#define REGS_PROD_ID_MASK           (0xFFFF<<16)
#define REGS_PROD_ID_SHIFT          (16)

//Build_Version
#define REGS_MAJOR(n)               (((n)&15)<<28)
#define REGS_YEAR(n)                (((n)&15)<<24)
#define REGS_MONTH(n)               (((n)&0xFF)<<16)
#define REGS_DAY(n)                 (((n)&0xFF)<<8)
#define REGS_BUILD_STYLE_FPGA       (0<<4)
#define REGS_BUILD_STYLE_CHIP       (1<<4)
#define REGS_BUILD_STYLE_FPGA_USB   (2<<4)
#define REGS_BUILD_STYLE_FPGA_GSM   (3<<4)
#define REGS_BUILD_REVISION(n)      (((n)&15)<<0)

//GPIO_Mode
#define REGS_MODE_PIN_CAM_RST       (1<<4)
#define REGS_MODE_PIN_CAM_RST_MASK  (1<<4)
#define REGS_MODE_PIN_CAM_RST_SHIFT (4)
#define REGS_MODE_PIN_CAM_RST_ALT   (0<<4)
#define REGS_MODE_PIN_CAM_RST_GPIO  (1<<4)
#define REGS_MODE_PIN_CAM_PDN       (1<<5)
#define REGS_MODE_PIN_CAM_PDN_MASK  (1<<5)
#define REGS_MODE_PIN_CAM_PDN_SHIFT (5)
#define REGS_MODE_PIN_CAM_PDN_ALT   (0<<5)
#define REGS_MODE_PIN_CAM_PDN_GPIO  (1<<5)
#define REGS_MODE_PIN_I2S_DI_1      (1<<6)
#define REGS_MODE_PIN_I2S_DI_1_MASK (1<<6)
#define REGS_MODE_PIN_I2S_DI_1_SHIFT (6)
#define REGS_MODE_PIN_I2S_DI_1_ALT  (0<<6)
#define REGS_MODE_PIN_I2S_DI_1_GPIO (1<<6)
#define REGS_MODE_PIN_SPI_CS_1      (1<<7)
#define REGS_MODE_PIN_SPI_CS_1_MASK (1<<7)
#define REGS_MODE_PIN_SPI_CS_1_SHIFT (7)
#define REGS_MODE_PIN_SPI_CS_1_ALT  (0<<7)
#define REGS_MODE_PIN_SPI_CS_1_GPIO (1<<7)
#define REGS_MODE_PIN_SLCD_DA       (1<<8)
#define REGS_MODE_PIN_SLCD_DA_MASK  (1<<8)
#define REGS_MODE_PIN_SLCD_DA_SHIFT (8)
#define REGS_MODE_PIN_SLCD_DA_ALT   (0<<8)
#define REGS_MODE_PIN_SLCD_DA_GPIO  (1<<8)
#define REGS_MODE_PIN_SLCD_DC       (1<<9)
#define REGS_MODE_PIN_SLCD_DC_MASK  (1<<9)
#define REGS_MODE_PIN_SLCD_DC_SHIFT (9)
#define REGS_MODE_PIN_SLCD_DC_ALT   (0<<9)
#define REGS_MODE_PIN_SLCD_DC_GPIO  (1<<9)
#define REGS_MODE_PIN_I2C_SDA       (1<<10)
#define REGS_MODE_PIN_I2C_SDA_MASK  (1<<10)
#define REGS_MODE_PIN_I2C_SDA_SHIFT (10)
#define REGS_MODE_PIN_I2C_SDA_ALT   (0<<10)
#define REGS_MODE_PIN_I2C_SDA_GPIO  (1<<10)
#define REGS_MODE_PIN_I2C_SCL       (1<<11)
#define REGS_MODE_PIN_I2C_SCL_MASK  (1<<11)
#define REGS_MODE_PIN_I2C_SCL_SHIFT (11)
#define REGS_MODE_PIN_I2C_SCL_ALT   (0<<11)
#define REGS_MODE_PIN_I2C_SCL_GPIO  (1<<11)
#define REGS_MODE_PIN_SLCD_CL       (1<<12)
#define REGS_MODE_PIN_SLCD_CL_MASK  (1<<12)
#define REGS_MODE_PIN_SLCD_CL_SHIFT (12)
#define REGS_MODE_PIN_SLCD_CL_ALT   (0<<12)
#define REGS_MODE_PIN_SLCD_CL_GPIO  (1<<12)
#define REGS_MODE_PIN_CAM_PCLK      (1<<13)
#define REGS_MODE_PIN_CAM_PCLK_MASK (1<<13)
#define REGS_MODE_PIN_CAM_PCLK_SHIFT (13)
#define REGS_MODE_PIN_CAM_PCLK_ALT  (0<<13)
#define REGS_MODE_PIN_CAM_PCLK_GPIO (1<<13)
#define REGS_MODE_PIN_CAM_HREF      (1<<14)
#define REGS_MODE_PIN_CAM_HREF_MASK (1<<14)
#define REGS_MODE_PIN_CAM_HREF_SHIFT (14)
#define REGS_MODE_PIN_CAM_HREF_ALT  (0<<14)
#define REGS_MODE_PIN_CAM_HREF_GPIO (1<<14)
#define REGS_MODE_PIN_CAM_VSYNC     (1<<15)
#define REGS_MODE_PIN_CAM_VSYNC_MASK (1<<15)
#define REGS_MODE_PIN_CAM_VSYNC_SHIFT (15)
#define REGS_MODE_PIN_CAM_VSYNC_ALT (0<<15)
#define REGS_MODE_PIN_CAM_VSYNC_GPIO (1<<15)
#define REGS_MODE_PIN_UART2_RTS     (1<<16)
#define REGS_MODE_PIN_UART2_RTS_MASK (1<<16)
#define REGS_MODE_PIN_UART2_RTS_SHIFT (16)
#define REGS_MODE_PIN_UART2_RTS_ALT (0<<16)
#define REGS_MODE_PIN_UART2_RTS_GPIO (1<<16)
#define REGS_MODE_PIN_UART2_CTS     (1<<17)
#define REGS_MODE_PIN_UART2_CTS_MASK (1<<17)
#define REGS_MODE_PIN_UART2_CTS_SHIFT (17)
#define REGS_MODE_PIN_UART2_CTS_ALT (0<<17)
#define REGS_MODE_PIN_UART2_CTS_GPIO (1<<17)
#define REGS_MODE_PIN_UART2_TXD     (1<<18)
#define REGS_MODE_PIN_UART2_TXD_MASK (1<<18)
#define REGS_MODE_PIN_UART2_TXD_SHIFT (18)
#define REGS_MODE_PIN_UART2_TXD_ALT (0<<18)
#define REGS_MODE_PIN_UART2_TXD_GPIO (1<<18)
#define REGS_MODE_PIN_UART2_RXD     (1<<19)
#define REGS_MODE_PIN_UART2_RXD_MASK (1<<19)
#define REGS_MODE_PIN_UART2_RXD_SHIFT (19)
#define REGS_MODE_PIN_UART2_RXD_ALT (0<<19)
#define REGS_MODE_PIN_UART2_RXD_GPIO (1<<19)
#define REGS_MODE_PIN_UART1_CTS     (1<<20)
#define REGS_MODE_PIN_UART1_CTS_MASK (1<<20)
#define REGS_MODE_PIN_UART1_CTS_SHIFT (20)
#define REGS_MODE_PIN_UART1_CTS_ALT (0<<20)
#define REGS_MODE_PIN_UART1_CTS_GPIO (1<<20)
#define REGS_MODE_PIN_UART1_RTS     (1<<21)
#define REGS_MODE_PIN_UART1_RTS_MASK (1<<21)
#define REGS_MODE_PIN_UART1_RTS_SHIFT (21)
#define REGS_MODE_PIN_UART1_RTS_ALT (0<<21)
#define REGS_MODE_PIN_UART1_RTS_GPIO (1<<21)
#define REGS_MODE_PIN_SDMMC_CLK     (1<<22)
#define REGS_MODE_PIN_SDMMC_CLK_MASK (1<<22)
#define REGS_MODE_PIN_SDMMC_CLK_SHIFT (22)
#define REGS_MODE_PIN_SDMMC_CLK_ALT (0<<22)
#define REGS_MODE_PIN_SDMMC_CLK_GPIO (1<<22)
#define REGS_MODE_PIN_SDMMC_DATA0   (1<<23)
#define REGS_MODE_PIN_SDMMC_DATA0_MASK (1<<23)
#define REGS_MODE_PIN_SDMMC_DATA0_SHIFT (23)
#define REGS_MODE_PIN_SDMMC_DATA0_ALT (0<<23)
#define REGS_MODE_PIN_SDMMC_DATA0_GPIO (1<<23)
#define REGS_MODE_PIN_SDMMC_DATA1   (1<<24)
#define REGS_MODE_PIN_SDMMC_DATA1_MASK (1<<24)
#define REGS_MODE_PIN_SDMMC_DATA1_SHIFT (24)
#define REGS_MODE_PIN_SDMMC_DATA1_ALT (0<<24)
#define REGS_MODE_PIN_SDMMC_DATA1_GPIO (1<<24)
#define REGS_MODE_PIN_SDMMC_DATA2   (1<<25)
#define REGS_MODE_PIN_SDMMC_DATA2_MASK (1<<25)
#define REGS_MODE_PIN_SDMMC_DATA2_SHIFT (25)
#define REGS_MODE_PIN_SDMMC_DATA2_ALT (0<<25)
#define REGS_MODE_PIN_SDMMC_DATA2_GPIO (1<<25)
#define REGS_MODE_PIN_SDMMC_DATA3   (1<<26)
#define REGS_MODE_PIN_SDMMC_DATA3_MASK (1<<26)
#define REGS_MODE_PIN_SDMMC_DATA3_SHIFT (26)
#define REGS_MODE_PIN_SDMMC_DATA3_ALT (0<<26)
#define REGS_MODE_PIN_SDMMC_DATA3_GPIO (1<<26)
#define REGS_MODE_PIN_SDMMC_CMD     (1<<27)
#define REGS_MODE_PIN_SDMMC_CMD_MASK (1<<27)
#define REGS_MODE_PIN_SDMMC_CMD_SHIFT (27)
#define REGS_MODE_PIN_SDMMC_CMD_ALT (0<<27)
#define REGS_MODE_PIN_SDMMC_CMD_GPIO (1<<27)
#define REGS_MODE_PIN_LPSCO_1       (1<<28)
#define REGS_MODE_PIN_LPSCO_1_MASK  (1<<28)
#define REGS_MODE_PIN_LPSCO_1_SHIFT (28)
#define REGS_MODE_PIN_LPSCO_1_ALT   (0<<28)
#define REGS_MODE_PIN_LPSCO_1_GPIO  (1<<28)
#define REGS_MODE_PIN_KEYOUT_5      (1<<29)
#define REGS_MODE_PIN_KEYOUT_5_MASK (1<<29)
#define REGS_MODE_PIN_KEYOUT_5_SHIFT (29)
#define REGS_MODE_PIN_KEYOUT_5_ALT  (0<<29)
#define REGS_MODE_PIN_KEYOUT_5_GPIO (1<<29)
#define REGS_MODE_PIN_KEYIN_5       (1<<30)
#define REGS_MODE_PIN_KEYIN_5_MASK  (1<<30)
#define REGS_MODE_PIN_KEYIN_5_SHIFT (30)
#define REGS_MODE_PIN_KEYIN_5_ALT   (0<<30)
#define REGS_MODE_PIN_KEYIN_5_GPIO  (1<<30)
#define REGS_MODE_PIN_TCO6          (1<<31)
#define REGS_MODE_PIN_TCO6_MASK     (1<<31)
#define REGS_MODE_PIN_TCO6_SHIFT    (31)
#define REGS_MODE_PIN_TCO6_ALT      (0<<31)
#define REGS_MODE_PIN_TCO6_GPIO     (1<<31)
#define REGS_GPIO_MODE(n)           (((n)&0xFFFFFFF)<<4)
#define REGS_GPIO_MODE_MASK         (0xFFFFFFF<<4)
#define REGS_GPIO_MODE_SHIFT        (4)

//Alt_mux_select
#define REGS_DEBUG_PORT_MASK        (3<<0)
#define REGS_DEBUG_PORT_DEBUG       (0<<0)
#define REGS_DEBUG_PORT_CAMERA      (1<<0)
#define REGS_DEBUG_PORT_DAI         (2<<0)
#define REGS_DEBUG_PORT_DAI_SIMPLE  (3<<0)
#define REGS_TCO_6_MASK             (1<<2)
#define REGS_TCO_6_TCO_6            (0<<2)
#define REGS_TCO_6_RF_SPI_DI        (1<<2)
#define REGS_XVR_SELECT_MASK        (1<<3)
#define REGS_XVR_SELECT_DIGRF       (0<<3)
#define REGS_XVR_SELECT_TCO         (1<<3)
#define REGS_STROBE_SELECT_MASK     (1<<4)
#define REGS_STROBE_SELECT_DIGRF_STROBE (0<<4)
#define REGS_STROBE_SELECT_TCO_8    (1<<4)
#define REGS_MEMORY_ADDESS_23_MASK  (1<<5)
#define REGS_MEMORY_ADDESS_23_MA_23 (0<<5)
#define REGS_MEMORY_ADDESS_23_M_CS4 (1<<5)
#define REGS_LSB_MEMORY_ADDR_BUS_MASK (1<<6)
#define REGS_LSB_MEMORY_ADDR_BUS_MEMORY (0<<6)
#define REGS_LSB_MEMORY_ADDR_BUS_PARALLEL_LCD (1<<6)
#define REGS_MEMORY_CS2_MASK        (3<<7)
#define REGS_MEMORY_CS2_M_CS_2      (0<<7)
#define REGS_MEMORY_CS2_LCD_CS_0    (1<<7)
#define REGS_MEMORY_CS2_SPI_CS_2    (2<<7)
#define REGS_SERIAL_LCD_CE_MASK     (3<<9)
#define REGS_SERIAL_LCD_CE_SLCD_CE  (0<<9)
#define REGS_SERIAL_LCD_CE_LCD_CS_1 (1<<9)
#define REGS_SERIAL_LCD_CE_M_CS_3   (2<<9)
#define REGS_UART2_MASK             (1<<11)
#define REGS_UART2_UART2            (0<<11)
#define REGS_UART2_UART1            (1<<11)
#define REGS_PWL0_MASK              (1<<12)
#define REGS_PWL0_PWL0              (0<<12)
#define REGS_PWL0_GPO_7             (1<<12)
#define REGS_PWL1_MASK              (1<<13)
#define REGS_PWL1_PWL1              (0<<13)
#define REGS_PWL1_GPO_6             (1<<13)
#define REGS_SDMMC_MASK             (1<<14)
#define REGS_SDMMC_SDMMC            (0<<14)
#define REGS_SDMMC_SPI2             (1<<14)
#define REGS_GPO_MASK               (1<<15)
#define REGS_GPO_GPO                (0<<15)
#define REGS_GPO_SPI2               (1<<15)
#define REGS_GPO_1_MASK             (1<<16)
#define REGS_GPO_1_GPO_1            (0<<16)
#define REGS_GPO_1_SPI2_CS_1        (1<<16)
#define REGS_TCO_5_MASK             (1<<17)
#define REGS_TCO_5_TCO_5            (0<<17)
#define REGS_TCO_5_GPO_10           (1<<17)
#define REGS_TCO_4_MASK             (1<<18)
#define REGS_TCO_4_TCO_4            (0<<18)
#define REGS_TCO_4_GPO_9            (1<<18)
#define REGS_TCO_3_MASK             (1<<19)
#define REGS_TCO_3_TCO_3            (0<<19)
#define REGS_TCO_3_GPO_8            (1<<19)
#define REGS_KEYOUT_5_MASK          (1<<20)
#define REGS_KEYOUT_5_KEYOUT_5      (0<<20)
#define REGS_KEYOUT_5_LPG           (1<<20)
#define REGS_KEYIN_5_MASK           (1<<21)
#define REGS_KEYIN_5_KEYIN_5        (0<<21)
#define REGS_KEYIN_5_PWT            (1<<21)

//IO_Drive_Select1
#define REGS_DATA(n)                (((n)&15)<<0)
#define REGS_LSB_MEMORY_ADDRESS(n)  (((n)&15)<<4)
#define REGS_MSB_MEMORY_ADDRESS(n)  (((n)&15)<<8)
#define REGS_MA_23(n)               (((n)&15)<<12)
#define REGS_M_CS0(n)               (((n)&15)<<16)
#define REGS_M_CS1(n)               (((n)&15)<<20)
#define REGS_M_CS2(n)               (((n)&15)<<24)
#define REGS_M_CTRL(n)              (((n)&15)<<28)

//IO_Drive_Select2
#define REGS_M_CTRL_WE(n)           (((n)&15)<<0)
#define REGS_M_CLK(n)               (((n)&15)<<4)
#define REGS_SERIAL_LCD(n)          (((n)&15)<<8)
#define REGS_SERIAL_LCD_SCE(n)      (((n)&15)<<12)
#define REGS_XVR_DOMAIN(n)          (((n)&15)<<16)

//ExtPll_Ctrl
#define REGS_EXTPLL_ENABLE_POWER_DOWN (1<<0)
#define REGS_EXTPLL_ENABLE_ENABLE   (0<<0)
#define REGS_EXTPLL_OUTPUT_ENABLE_ENABLE (0<<4)
#define REGS_EXTPLL_OUTPUT_ENABLE_DISABLE (1<<4)
#define REGS_EXTPLL_BYPASS_PASS     (0<<8)
#define REGS_EXTPLL_BYPASS_BYPASS   (1<<8)
#define REGS_EXTPLL_LOCKED_NOT_LOCKED (0<<12)
#define REGS_EXTPLL_LOCKED_LOCKED   (1<<12)
#define REGS_EXTPLL_CLK_FAST_ENABLE_ENABLE (1<<16)
#define REGS_EXTPLL_CLK_FAST_ENABLE_DISABLE (0<<16)
#define REGS_EXTPLL_LOCK_CTRL_RST_NO_RESET (0<<20)
#define REGS_EXTPLL_LOCK_CTRL_RST_RESET (1<<20)

//ExtPll_Config
#define REGS_EXTPLL_INPUT_DIVIDER(n) (((n)&31)<<0)
#define REGS_EXTPLL_FEEDBACK_DIVIDER(n) (((n)&0x1FF)<<8)
#define REGS_EXTPLL_OUTPUT_DIVIDER_DIV_BY_1 (0<<24)
#define REGS_EXTPLL_OUTPUT_DIVIDER_DIV_BY_2 (1<<24)
#define REGS_EXTPLL_OUTPUT_DIVIDER_DIV_BY_2_ALSO (2<<24)
#define REGS_EXTPLL_OUTPUT_DIVIDER_DIV_BY_4 (3<<24)

//ExtPll_Dbg_Clk
#define REGS_EXTPLL_DBG_CLK_SEL_CLK_TO_PLLACCO (0<<0)
#define REGS_EXTPLL_DBG_CLK_SEL_CLK_USB (1<<0)
#define REGS_EXTPLL_DBG_CLK_SEL_CLK_STEREO (2<<0)
#define REGS_EXTPLL_DBG_CLK_SEL_CLK_CAMERA (3<<0)
#define REGS_EXTPLL_DBG_CLK_SEL_AU_EXTTX_STB (4<<0)

//CLK_CAMERA_OUT_CTRL
#define REGS_CLK_CAMERA_OUT_EN_DISABLE (0<<0)
#define REGS_CLK_CAMERA_OUT_EN_ENABLE (1<<0)
#define REGS_CLK_CAMERA_DIV_SRC_SEL_156_MHZ (0<<4)
#define REGS_CLK_CAMERA_DIV_SRC_SEL_144_MHZ (1<<4)
#define REGS_CLK_CAMERA_DIV_SRC_SEL__26_MHZ (2<<4)
#define REGS_CLK_CAMERA_OUT_DIV(n)  (((n)&15)<<8)
#define REGS_CLK_CAMERA_OUT_DIV_MASK (15<<8)
#define REGS_CLK_CAMERA_OUT_DIV_SHIFT (8)
#define REGS_CLK_CAMERA_OUT_SRC_SEL_FROM_DIVIDER (0<<12)
#define REGS_CLK_CAMERA_OUT_SRC_SEL_FROM_32K (1<<12)
#define REGS_CLK_CAMERA_OUT_SRC_SEL_FROM_26M (2<<12)

//STEREO_DAC_CTRL_SET
#define REGS_AU_HPLPD_POWER_UP      (0<<0)
#define REGS_AU_HPLPD_POWER_DOWN    (1<<0)
#define REGS_AU_HPRPD_POWER_UP      (0<<1)
#define REGS_AU_HPRPD_POWER_DOWN    (1<<1)
#define REGS_AU_VREFPD_POWER_UP     (0<<2)
#define REGS_AU_VREFPD_POWER_DOWN   (1<<2)
#define REGS_AU_VMIDPD_POWER_UP     (0<<3)
#define REGS_AU_VMIDPD_POWER_DOWN   (1<<3)
#define REGS_AU_IBPD_POWER_UP       (0<<4)
#define REGS_AU_IBPD_POWER_DOWN     (1<<4)
#define REGS_AU_DACPDL_POWER_UP     (0<<5)
#define REGS_AU_DACPDL_POWER_DOWN   (1<<5)
#define REGS_AU_DACPDR_POWER_UP     (0<<6)
#define REGS_AU_DACPDR_POWER_DOWN   (1<<6)
#define REGS_AU_RST_H_NO_RESET      (0<<16)
#define REGS_AU_RST_H_RESET         (1<<16)
#define REGS_AU_CLKENB_L_ENABLE     (0<<31)
#define REGS_AU_CLKENB_L_DISABLE    (1<<31)

//STEREO_DAC_CTRL_CLR
//#define REGS_AU_HPLPD_POWER_UP    (0<<0)
//#define REGS_AU_HPLPD_POWER_DOWN  (1<<0)
//#define REGS_AU_HPRPD_POWER_UP    (0<<1)
//#define REGS_AU_HPRPD_POWER_DOWN  (1<<1)
//#define REGS_AU_VREFPD_POWER_UP   (0<<2)
//#define REGS_AU_VREFPD_POWER_DOWN (1<<2)
//#define REGS_AU_VMIDPD_POWER_UP   (0<<3)
//#define REGS_AU_VMIDPD_POWER_DOWN (1<<3)
//#define REGS_AU_IBPD_POWER_UP     (0<<4)
//#define REGS_AU_IBPD_POWER_DOWN   (1<<4)
//#define REGS_AU_DACPDL_POWER_UP   (0<<5)
//#define REGS_AU_DACPDL_POWER_DOWN (1<<5)
//#define REGS_AU_DACPDR_POWER_UP   (0<<6)
//#define REGS_AU_DACPDR_POWER_DOWN (1<<6)
//#define REGS_AU_RST_H_NO_RESET    (0<<16)
//#define REGS_AU_RST_H_RESET       (1<<16)
//#define REGS_AU_CLKENB_L_ENABLE   (0<<31)
//#define REGS_AU_CLKENB_L_DISABLE  (1<<31)

//STEREO_DAC_DIG_CTRL
#define REGS_AU_DACFLT(n)           (((n)&3)<<0)
#define REGS_AU_DACFLT_MASK         (3<<0)
#define REGS_AU_DACFLT_SHIFT        (0)
#define REGS_AU_DACFLT_OSR_125      (0<<0)
#define REGS_AU_DACFLT_OSR_135      (1<<0)
#define REGS_AU_DACFLT_OSR_145      (2<<0)
#define REGS_AU_DACFLT_RESERVED     (3<<0)
#define REGS_AU_AIF_STB_CFG         (1<<3)
#define REGS_AU_AIF_STB_CFG_MASK    (1<<3)
#define REGS_AU_AIF_STB_CFG_SHIFT   (3)
#define REGS_AU_AIF_STB_CFG_RISING  (0<<3)
#define REGS_AU_AIF_STB_CFG_FALLING (1<<3)
#define REGS_AU_DACMU_L             (1<<4)
#define REGS_AU_DACMU_L_MASK        (1<<4)
#define REGS_AU_DACMU_L_SHIFT       (4)
#define REGS_AU_DACMU_L_NO_MUTE     (0<<4)
#define REGS_AU_DACMU_L_MUTE        (1<<4)
#define REGS_AU_DACMU_R             (1<<5)
#define REGS_AU_DACMU_R_MASK        (1<<5)
#define REGS_AU_DACMU_R_SHIFT       (5)
#define REGS_AU_DACMU_R_NO_MUTE     (0<<5)
#define REGS_AU_DACMU_R_MUTE        (1<<5)
#define REGS_AU_DACINV_L            (1<<6)
#define REGS_AU_DACINV_R            (1<<7)
#define REGS_AU_DWAEN               (1<<8)
#define REGS_AU_DWAEN_MASK          (1<<8)
#define REGS_AU_DWAEN_SHIFT         (8)
#define REGS_AU_DWAEN_DISABLE       (0<<8)
#define REGS_AU_DWAEN_ENABLE        (1<<8)
#define REGS_AU_TSTEN               (1<<9)
#define REGS_AU_TSTEN_MASK          (1<<9)
#define REGS_AU_TSTEN_SHIFT         (9)
#define REGS_AU_TSTEN_DISABLE       (0<<9)
#define REGS_AU_TSTEN_ENABLE        (1<<9)
#define REGS_AU_LDAMUX              (1<<12)
#define REGS_AU_LDAMUX_MASK         (1<<12)
#define REGS_AU_LDAMUX_SHIFT        (12)
#define REGS_AU_LDAMUX_DIGIT_INPUT  (0<<12)
#define REGS_AU_LDAMUX_TEST_STIMU   (1<<12)
#define REGS_AU_RDAMUX              (1<<13)
#define REGS_AU_RDAMUX_MASK         (1<<13)
#define REGS_AU_RDAMUX_SHIFT        (13)
#define REGS_AU_RDAMUX_DIGIT_INPUT  (0<<13)
#define REGS_AU_RDAMUX_TEST_STIMU   (1<<13)
#define REGS_AU_DAC_SHIFT_SIGN      (1<<14)
#define REGS_AU_DAC_SHIFT_SIGN_MASK (1<<14)
#define REGS_AU_DAC_SHIFT_SIGN_SHIFT (14)
#define REGS_AU_DAC_SHIFT_SIGN_SIGNED (0<<14)
#define REGS_AU_DAC_SHIFT_SIGN_UNSIGNED (1<<14)
#define REGS_AU_DACIL_SHIFT(n)      (((n)&7)<<16)
#define REGS_AU_DACIL_SHIFT_MASK    (7<<16)
#define REGS_AU_DACIL_SHIFT_SHIFT   (16)
#define REGS_AU_DACIR_SHIFT(n)      (((n)&7)<<20)
#define REGS_AU_DACIR_SHIFT_MASK    (7<<20)
#define REGS_AU_DACIR_SHIFT_SHIFT   (20)
#define REGS_AU_CLK_DIV(n)          (((n)&15)<<24)
#define REGS_AU_CLK_DIV_MASK        (15<<24)
#define REGS_AU_CLK_DIV_SHIFT       (24)
#define REGS_AU_CLK_SRC_SEL         (1<<28)
#define REGS_AU_CLK_SRC_SEL_MASK    (1<<28)
#define REGS_AU_CLK_SRC_SEL_SHIFT   (28)
#define REGS_AU_CLK_SRC_SEL_12_MHZ  (0<<28)
#define REGS_AU_CLK_SRC_SEL_13_MHZ  (1<<28)

//STEREO_DAC_ANA_CTRL
#define REGS_AU_TOEN_DISABLE        (0<<0)
#define REGS_AU_TOEN_ENABLE         (1<<0)
#define REGS_AU_TOTM                (1<<1)
#define REGS_AU_CAPLESS_DISABLE     (0<<2)
#define REGS_AU_CAPLESS_ENABLE      (1<<2)
#define REGS_AU_HPLZC               (1<<4)
#define REGS_AU_HPRZC               (1<<5)
#define REGS_AU_LDA2HPL_DISABLE     (0<<16)
#define REGS_AU_LDA2HPL_ENABLE      (1<<16)
#define REGS_AU_RDA2HPL_DISABLE     (0<<17)
#define REGS_AU_RDA2HPL_ENABLE      (1<<17)
#define REGS_AU_LDA2HPR_DISABLE     (0<<20)
#define REGS_AU_LDA2HPR_ENABLE      (1<<20)
#define REGS_AU_RDA2HPR_DISABLE     (0<<21)
#define REGS_AU_RDA2HPR_ENABLE      (1<<21)
#define REGS_AU_VOI2HP_DISABLE      (0<<24)
#define REGS_AU_VOI2HP_ENABLE       (1<<24)
#define REGS_AU_TSDEN_DISABLE       (0<<28)
#define REGS_AU_TSDEN_ENABLE        (1<<28)
#define REGS_AU_DTEMP_NORMAL        (0<<31)
#define REGS_AU_DTEMP_OVER_TEMP     (1<<31)

//STEREO_DAC_MIXER_HP_GAIN
#define REGS_AU_VOI2HPVOL(n)        (((n)&7)<<0)

//STEREO_DAC_HP_GAIN_L
#define REGS_AU_HPLVU               (1<<0)
#define REGS_AU_HPLVOL(n)           (((n)&31)<<8)
#define REGS_AU_HPLVOL_MASK         (31<<8)
#define REGS_AU_HPLVOL_SHIFT        (8)
#define REGS_AU_HPLVOL_MUTE         (0<<8)

//STEREO_DAC_HP_GAIN_R
#define REGS_AU_HPRVU               (1<<0)
#define REGS_AU_HPRVOL(n)           (((n)&31)<<8)
#define REGS_AU_HPRVOL_MASK         (31<<8)
#define REGS_AU_HPRVOL_SHIFT        (8)
#define REGS_AU_HPRVOL_MUTE         (0<<8)

//STEREO_DAC_REF_GEN
#define REGS_AU_VROI                (1<<0)
#define REGS_AU_VROI_MASK           (1<<0)
#define REGS_AU_VROI_SHIFT          (0)
#define REGS_AU_VMIDSEL(n)          (((n)&3)<<4)
#define REGS_AU_VMIDSEL_MASK        (3<<4)
#define REGS_AU_VMIDSEL_SHIFT       (4)
#define REGS_AU_VSEL(n)             (((n)&7)<<8)
#define REGS_AU_VSEL_MASK           (7<<8)
#define REGS_AU_VSEL_SHIFT          (8)
#define REGS_AU_IMFMIX(n)           (((n)&3)<<12)
#define REGS_AU_IMFMIX_MASK         (3<<12)
#define REGS_AU_IMFMIX_SHIFT        (12)
#define REGS_AU_IMFDR(n)            (((n)&3)<<16)
#define REGS_AU_IMFDR_MASK          (3<<16)
#define REGS_AU_IMFDR_SHIFT         (16)
#define REGS_AU_IMFDAC(n)           (((n)&3)<<20)
#define REGS_AU_IMFDAC_MASK         (3<<20)
#define REGS_AU_IMFDAC_SHIFT        (20)
#define REGS_AU_DACIT1              (1<<24)
#define REGS_AU_DACIT1_MASK         (1<<24)
#define REGS_AU_DACIT1_SHIFT        (24)

//IO_TEST_MODE
#define REGS_TEST_MODE(n)           (((n)&15)<<0)
#define REGS_TEST_MODE_MASK         (15<<0)
#define REGS_TEST_MODE_SHIFT        (0)
#define REGS_TEST_MODE_DISABLED     (0<<0)
#define REGS_TEST_MODE_ACCO_TEST    (1<<0)
#define REGS_TEST_MODE_USB_TEST     (2<<0)
#define REGS_TEST_MODE_ST_DAC_TEST  (3<<0)
#define REGS_TEST_MODE_ST_DAC_BIST  (4<<0)
#define REGS_TEST_MODE_USB_BACKUP   (5<<0)





#endif
