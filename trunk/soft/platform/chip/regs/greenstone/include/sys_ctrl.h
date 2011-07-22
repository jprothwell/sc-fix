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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/sys_ctrl.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _SYS_CTRL_H_
#define _SYS_CTRL_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'sys_ctrl'."
#endif


#include "globals.h"
#include "ip_version.h"

// =============================================================================
//  MACROS
// =============================================================================

typedef enum {
/// CPU IDs
    XCPU                                        = 0x00000000,
    BCPU                                        = 0x00000001
} CPU_ID_T;


typedef enum {
/// System side System clocks
    SYS_XCPU                                    = 0x00000000,
    SYS_XCPU_INT                                = 0x00000001,
    SYS_AMBA                                    = 0x00000002,
    SYS_PCLK_CONF                               = 0x00000003,
    SYS_PCLK_DATA                               = 0x00000004,
    SYS_DMA                                     = 0x00000005,
    SYS_EBC                                     = 0x00000006,
    SYS_IFC_CH0                                 = 0x00000007,
    SYS_IFC_CH1                                 = 0x00000008,
    SYS_IFC_CH2                                 = 0x00000009,
    SYS_IFC_CH3                                 = 0x0000000A,
    SYS_IFC_CH4                                 = 0x0000000B,
    SYS_IFC_CH5                                 = 0x0000000C,
    SYS_IFC_CH6                                 = 0x0000000D,
    SYS_IFC_CH7                                 = 0x0000000E,
    SYS_IFC_DBG                                 = 0x0000000F,
    SYS_A2A                                     = 0x00000010,
    SYS_COM_REGS                                = 0x00000011,
    SYS_DEBUG_UART                              = 0x00000012,
    SYS_DBGHST                                  = 0x00000013,
    SYS_GPADC                                   = 0x00000014,
/// System side divided clock (either divided by module or by sys_ctrl)
    SYSD_SCI                                    = 0x00000015,
    SYSD_RF_SPI                                 = 0x00000016,
    SYSD_OSC                                    = 0x00000017,
    SYSD_PWM                                    = 0x00000018,
/// the following don't have an auto enable
    SYS_GPIO                                    = 0x00000019,
    SYS_IRQ                                     = 0x0000001A,
    SYS_TCU                                     = 0x0000001B,
    SYS_TIMER                                   = 0x0000001C,
/// the following are sharing their enable
    SYS_SCI                                     = 0x0000001D,
/// keep last
    SYS_NOGATE                                  = 0x0000001E
} SYS_CLKS_T;

#define NB_SYS_CLK_AEN                           (25)
#define NB_SYS_CLK_EN                            (29)
#define NB_SYS_CLK                               (31)

typedef enum {
/// System side System clocks for Peripherals
    PER_UART                                    = 0x00000000,
    PER_UART2                                   = 0x00000001,
    PER_I2C                                     = 0x00000002,
    PER_LCDC                                    = 0x00000003,
    PER_SDMMC                                   = 0x00000004,
/// System side divided clock (either divided by module or by sys_ctrl)
    PERD_SPI                                    = 0x00000005,
    PERD_SPI2                                   = 0x00000006,
    PER_USBC                                    = 0x00000007,
    PER_CAMERA                                  = 0x00000008,
    PER_SPY                                     = 0x00000009,
    PER_TEST                                    = 0x0000000A
} PER_CLKS_T;

#define NB_PER_CLK_AEN                           (7)
#define NB_PER_CLK_EN                            (11)
#define NB_PER_CLK                               (11)

typedef enum {
/// Baseband side System clocks
    BB_BCPU                                     = 0x00000000,
    BB_BCPU_INT                                 = 0x00000001,
    BB_AMBA                                     = 0x00000002,
    BB_PCLK_CONF                                = 0x00000003,
    BB_PCLK_DATA                                = 0x00000004,
    BB_VOC_AHB                                  = 0x00000005,
    BB_IFC_CH0                                  = 0x00000006,
    BB_IFC_CH1                                  = 0x00000007,
    BB_IFC_CH2                                  = 0x00000008,
    BB_SRAM                                     = 0x00000009,
    BB_A2A                                      = 0x0000000A,
    BB_ITLV                                     = 0x0000000B,
    BB_VITERBI                                  = 0x0000000C,
    BB_CIPHER                                   = 0x0000000D,
    BB_RF_IF                                    = 0x0000000E,
    BB_COPRO                                    = 0x0000000F,
    BB_CP2_REG                                  = 0x00000010,
    BB_XCOR                                     = 0x00000011,
    BB_AIF                                      = 0x00000012,
/// Baseband side divided clock
/// divided BCK clock for DAI
    BBD_DAIS_208K                               = 0x00000013,
/// the following don't have an auto enable
    BB_IRQ                                      = 0x00000014
} BB_CLKS_T;

#define NB_BB_CLK_AEN                            (20)
#define NB_BB_CLK_EN                             (21)
#define NB_BB_CLK                                (21)
/// Other clocks
/// clocks with auto enble
/// the debug host clock auto enable is not used in host mode, only in uart mode
#define OC_HOST_UART                             (0)

typedef enum {
    OC_DEBUG_UART                               = 0x00000000,
    OC_BCK                                      = 0x00000001,
    OC_UART                                     = 0x00000002,
    OC_UART2                                    = 0x00000003,
    OC_RF_RX                                    = 0x00000004,
    OC_RF_TX                                    = 0x00000005,
    OC_MEM_BRIDGE                               = 0x00000006,
    OC_VOC_CORE                                 = 0x00000007,
    OC_VOC_AHB                                  = 0x00000008,
    OC_VOC_DAI                                  = 0x00000009,
/// the following don't have an auto enable
    OC_VOC                                      = 0x0000000A,
    OC_LPS                                      = 0x0000000B,
    OC_GPIO                                     = 0x0000000C,
    OC_USBPHY                                   = 0x0000000D,
    OC_CAMERA                                   = 0x0000000E,
    OC_CLK_OUT                                  = 0x0000000F,
    OC_MEM_CLK_OUT                              = 0x00000010
} OTHER_CLKS_T;

#define NB_OTHER_CLK_VOC_AEN_SYNC                (7)
#define NB_OTHER_CLK_AEN                         (10)
#define NB_OTHER_CLK_EN                          (17)
#define NB_OTHER_CLK                             (17)

typedef enum {
/// System side resets
    RST_XCPU                                    = 0x00000000,
    RST_SYS_IRQ                                 = 0x00000001,
    RST_SYS_A2A                                 = 0x00000002,
    RST_COM_REGS                                = 0x00000003,
    RST_DMA                                     = 0x00000004,
    RST_TIMER                                   = 0x00000005,
    RST_TCU                                     = 0x00000006,
    RST_GPIO                                    = 0x00000007,
    RST_KEYPAD                                  = 0x00000008,
    RST_PWM                                     = 0x00000009,
    RST_UART                                    = 0x0000000A,
    RST_UART2                                   = 0x0000000B,
    RST_SPI                                     = 0x0000000C,
    RST_SPI2                                    = 0x0000000D,
    RST_RF_SPI                                  = 0x0000000E,
    RST_SCI                                     = 0x0000000F,
    RST_SPY                                     = 0x00000010,
    RST_TEST                                    = 0x00000011,
    RST_I2C                                     = 0x00000012,
    RST_MEM_BRIDGE                              = 0x00000013,
    RST_PA_AFC                                  = 0x00000014,
    RST_GPADC                                   = 0x00000015,
    RST_ANA_ACCO                                = 0x00000016,
    RST_EXT_APB                                 = 0x00000017,
    RST_LCDC                                    = 0x00000018,
    RST_SDMMC                                   = 0x00000019,
    RST_CAMERA                                  = 0x0000001A,
    RST_USBC                                    = 0x0000001B,
    RST_USBPHY                                  = 0x0000001C,
    RST_BCPU                                    = 0x0000001D,
    RST_BB_IRQ                                  = 0x0000001E,
    RST_BB_A2A                                  = 0x0000001F,
    RST_BB_IFC                                  = 0x00000020,
    RST_BB_SRAM                                 = 0x00000021,
    RST_ITLV                                    = 0x00000022,
    RST_VITERBI                                 = 0x00000023,
    RST_CIPHER                                  = 0x00000024,
    RST_XCOR                                    = 0x00000025,
    RST_COPRO                                   = 0x00000026,
    RST_RF_IF                                   = 0x00000027,
    RST_VOC                                     = 0x00000028,
    RST_AIF                                     = 0x00000029,
    RST_DAI                                     = 0x0000002A,
    RST_BB_FULL                                 = 0x0000002B,
    RST_SYS_FULL                                = 0x0000002C
} RESETS_T;

#define NB_SRST                                  (29)
/// Baseband side resets
#define BOUND_BRST_FIRST                         (29)
#define BOUND_BRST_AFTER                         (43)
/// The following reset does not have register
#define NR_RST_REG                               (44)
#define NB_RST                                   (45)
#define NB_BRST                                  (BOUND_BRST_AFTER-BOUND_BRST_FIRST)

typedef enum {
/// Reset Other : resync on corresponding clock other
    RSTO_DBG_HOST                               = 0x00000000,
    RSTO_BCK                                    = 0x00000001,
    RSTO_UART                                   = 0x00000002,
    RSTO_UART2                                  = 0x00000003,
    RSTO_RF_RX                                  = 0x00000004,
    RSTO_RF_TX                                  = 0x00000005,
    RSTO_MEM_BRIDGE                             = 0x00000006,
    RSTO_VOC_DAI                                = 0x00000007,
    RSTO_VOC                                    = 0x00000008,
    RSTO_LPS                                    = 0x00000009,
    RSTO_GPIO                                   = 0x0000000A,
    RSTO_WDTIMER                                = 0x0000000B,
    RSTO_USBPHY                                 = 0x0000000C
} RESET_OTHERS_T;

#define BOUND_RSTO_RF_FIRST                      (4)
#define BOUND_RSTO_RF_AFTER                      (6)
#define NB_RSTO                                  (13)
/// For REG_DBG protect lock/unlock value
#define SYS_CTRL_PROTECT_LOCK                    (0XA50000)
#define SYS_CTRL_PROTECT_UNLOCK                  (0XA50001)

// =============================================================================
//  TYPES
// =============================================================================
// ============================================================================
// SYS_CTRL_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_SYS_CTRL_BASE           0x00B80000


typedef volatile struct
{
    /// <strong>This register is used to Lock and Unlock the protected registers.</strong>
    REG32                          REG_DBG;                      //0x00000000
    IP_VERSION_T                   IP_Version;                   //0x0x00000004
    /// This register is protected.
    REG32                          Sys_Rst_Set;                  //0x00000008
    REG32                          Sys_Rst_Clr;                  //0x0000000C
    /// This register is protected.
    REG32                          BB_Rst_Set;                   //0x00000010
    REG32                          BB_Rst_Clr;                   //0x00000014
    REG32                          Clk_Sys_Mode;                 //0x00000018
    REG32                          Clk_Sys_Enable;               //0x0000001C
    /// This register is protected.
    REG32                          Clk_Sys_Disable;              //0x00000020
    REG32                          Clk_Per_Mode;                 //0x00000024
    REG32                          Clk_Per_Enable;               //0x00000028
    /// This register is protected.
    REG32                          Clk_Per_Disable;              //0x0000002C
    REG32                          Clk_BB_Mode;                  //0x00000030
    REG32                          Clk_BB_Enable;                //0x00000034
    /// This register is protected.
    REG32                          Clk_BB_Disable;               //0x00000038
    REG32                          Clk_Other_Mode;               //0x0000003C
    REG32                          Clk_Other_Enable;             //0x00000040
    /// This register is protected.
    REG32                          Clk_Other_Disable;            //0x00000044
    /// This register is protected.
    REG32                          Sel_Clock;                    //0x00000048
    /// Bit force_div_update in this register is protected.
    REG32                          Cfg_Clk_Sys;                  //0x0000004C
    REG32                          Cfg_Clk_Mem_Bridge;           //0x00000050
    REG32                          Cfg_Clk_Voc;                  //0x00000054
    REG32                          Cfg_Clk_AudioBCK_Div;         //0x00000058
    REG32                          Cfg_Clk_Uart[3];              //0x0000005C
    REG32                          Cfg_Clk_PWM;                  //0x00000068
    /// This register is protected.
    REG32                          Cfg_AHB;                      //0x0000006C
    REG32                          XCpu_Dbg_BKP;                 //0x00000070
    REG32                          XCpu_Dbg_Addr;                //0x00000074
    REG32                          BCpu_Dbg_BKP;                 //0x00000078
    REG32                          BCpu_Dbg_Addr;                //0x0000007C
    REG32                          Cfg_Cpus_Cache_Ram_Disable;   //0x00000080
    REG32                          Reset_Cause;                  //0x00000084
    /// This register is protected.
    REG32                          WakeUp;                       //0x00000088
} HWP_SYS_CTRL_T;

#define hwp_sysCtrl                 ((HWP_SYS_CTRL_T*) KSEG1(REG_SYS_CTRL_BASE))


//REG_DBG
#define SYS_CTRL_SCRATCH(n)         (((n)&0xFFFF)<<0)
#define SYS_CTRL_WRITE_UNLOCK_STATUS (1<<30)
#define SYS_CTRL_WRITE_UNLOCK       (1<<31)

//IP_Version
#define SYS_CTRL_MAJOR(n)           (((n)&15)<<28)
#define SYS_CTRL_MINOR(n)           (((n)&15)<<24)
#define SYS_CTRL_YEAR(n)            (((n)&15)<<16)
#define SYS_CTRL_MONTH(n)           (((n)&0xFF)<<8)
#define SYS_CTRL_DAY(n)             (((n)&0xFF)<<0)

//Sys_Rst_Set
#define SYS_CTRL_SET_RST_XCPU       (1<<0)
#define SYS_CTRL_SET_RST_SYS_IRQ    (1<<1)
#define SYS_CTRL_SET_RST_SYS_A2A    (1<<2)
#define SYS_CTRL_SET_RST_COM_REGS   (1<<3)
#define SYS_CTRL_SET_RST_DMA        (1<<4)
#define SYS_CTRL_SET_RST_TIMER      (1<<5)
#define SYS_CTRL_SET_RST_TCU        (1<<6)
#define SYS_CTRL_SET_RST_GPIO       (1<<7)
#define SYS_CTRL_SET_RST_KEYPAD     (1<<8)
#define SYS_CTRL_SET_RST_PWM        (1<<9)
#define SYS_CTRL_SET_RST_UART       (1<<10)
#define SYS_CTRL_SET_RST_UART2      (1<<11)
#define SYS_CTRL_SET_RST_SPI        (1<<12)
#define SYS_CTRL_SET_RST_SPI2       (1<<13)
#define SYS_CTRL_SET_RST_RF_SPI     (1<<14)
#define SYS_CTRL_SET_RST_SCI        (1<<15)
#define SYS_CTRL_SET_RST_SPY        (1<<16)
#define SYS_CTRL_SET_RST_TEST       (1<<17)
#define SYS_CTRL_SET_RST_I2C        (1<<18)
#define SYS_CTRL_SET_RST_MEM_BRIDGE (1<<19)
#define SYS_CTRL_SET_RST_PA_AFC     (1<<20)
#define SYS_CTRL_SET_RST_GPADC      (1<<21)
#define SYS_CTRL_SET_RST_ANA_ACCO   (1<<22)
#define SYS_CTRL_SET_RST_EXT_APB    (1<<23)
#define SYS_CTRL_SET_RST_LCDC       (1<<24)
#define SYS_CTRL_SET_RST_SDMMC      (1<<25)
#define SYS_CTRL_SET_RST_CAMERA     (1<<26)
#define SYS_CTRL_SET_RST_USBC       (1<<27)
#define SYS_CTRL_SET_RST_USBPHY     (1<<28)
#define SYS_CTRL_SET_RST_OUT        (1<<30)
#define SYS_CTRL_SOFT_RST           (1<<31)
#define SYS_CTRL_SET_SYS_RST(n)     (((n)&0x1FFFFFFF)<<0)
#define SYS_CTRL_SET_SYS_RST_MASK   (0x1FFFFFFF<<0)
#define SYS_CTRL_SET_SYS_RST_SHIFT  (0)

//Sys_Rst_Clr
#define SYS_CTRL_CLR_RST_XCPU       (1<<0)
#define SYS_CTRL_CLR_RST_SYS_IRQ    (1<<1)
#define SYS_CTRL_CLR_RST_SYS_A2A    (1<<2)
#define SYS_CTRL_CLR_RST_COM_REGS   (1<<3)
#define SYS_CTRL_CLR_RST_DMA        (1<<4)
#define SYS_CTRL_CLR_RST_TIMER      (1<<5)
#define SYS_CTRL_CLR_RST_TCU        (1<<6)
#define SYS_CTRL_CLR_RST_GPIO       (1<<7)
#define SYS_CTRL_CLR_RST_KEYPAD     (1<<8)
#define SYS_CTRL_CLR_RST_PWM        (1<<9)
#define SYS_CTRL_CLR_RST_UART       (1<<10)
#define SYS_CTRL_CLR_RST_UART2      (1<<11)
#define SYS_CTRL_CLR_RST_SPI        (1<<12)
#define SYS_CTRL_CLR_RST_SPI2       (1<<13)
#define SYS_CTRL_CLR_RST_RF_SPI     (1<<14)
#define SYS_CTRL_CLR_RST_SCI        (1<<15)
#define SYS_CTRL_CLR_RST_SPY        (1<<16)
#define SYS_CTRL_CLR_RST_TEST       (1<<17)
#define SYS_CTRL_CLR_RST_I2C        (1<<18)
#define SYS_CTRL_CLR_RST_MEM_BRIDGE (1<<19)
#define SYS_CTRL_CLR_RST_PA_AFC     (1<<20)
#define SYS_CTRL_CLR_RST_GPADC      (1<<21)
#define SYS_CTRL_CLR_RST_ANA_ACCO   (1<<22)
#define SYS_CTRL_CLR_RST_EXT_APB    (1<<23)
#define SYS_CTRL_CLR_RST_LCDC       (1<<24)
#define SYS_CTRL_CLR_RST_SDMMC      (1<<25)
#define SYS_CTRL_CLR_RST_CAMERA     (1<<26)
#define SYS_CTRL_CLR_RST_USBC       (1<<27)
#define SYS_CTRL_CLR_RST_USBPHY     (1<<28)
#define SYS_CTRL_CLR_RST_OUT        (1<<30)
#define SYS_CTRL_CLR_SYS_RST(n)     (((n)&0x1FFFFFFF)<<0)
#define SYS_CTRL_CLR_SYS_RST_MASK   (0x1FFFFFFF<<0)
#define SYS_CTRL_CLR_SYS_RST_SHIFT  (0)

//BB_Rst_Set
#define SYS_CTRL_SET_RST_BCPU       (1<<0)
#define SYS_CTRL_SET_RST_BB_IRQ     (1<<1)
#define SYS_CTRL_SET_RST_BB_A2A     (1<<2)
#define SYS_CTRL_SET_RST_BB_IFC     (1<<3)
#define SYS_CTRL_SET_RST_BB_SRAM    (1<<4)
#define SYS_CTRL_SET_RST_ITLV       (1<<5)
#define SYS_CTRL_SET_RST_VITERBI    (1<<6)
#define SYS_CTRL_SET_RST_CIPHER     (1<<7)
#define SYS_CTRL_SET_RST_XCOR       (1<<8)
#define SYS_CTRL_SET_RST_COPRO      (1<<9)
#define SYS_CTRL_SET_RST_RF_IF      (1<<10)
#define SYS_CTRL_SET_RST_VOC        (1<<11)
#define SYS_CTRL_SET_RST_AIF        (1<<12)
#define SYS_CTRL_SET_RST_DAI        (1<<13)
#define SYS_CTRL_SET_RST_BB_FULL    (1<<31)
#define SYS_CTRL_SET_BB_RST(n)      (((n)&0x3FFF)<<0)
#define SYS_CTRL_SET_BB_RST_MASK    (0x3FFF<<0)
#define SYS_CTRL_SET_BB_RST_SHIFT   (0)

//BB_Rst_Clr
#define SYS_CTRL_CLR_RST_BCPU       (1<<0)
#define SYS_CTRL_CLR_RST_BB_IRQ     (1<<1)
#define SYS_CTRL_CLR_RST_BB_A2A     (1<<2)
#define SYS_CTRL_CLR_RST_BB_IFC     (1<<3)
#define SYS_CTRL_CLR_RST_BB_SRAM    (1<<4)
#define SYS_CTRL_CLR_RST_ITLV       (1<<5)
#define SYS_CTRL_CLR_RST_VITERBI    (1<<6)
#define SYS_CTRL_CLR_RST_CIPHER     (1<<7)
#define SYS_CTRL_CLR_RST_XCOR       (1<<8)
#define SYS_CTRL_CLR_RST_COPRO      (1<<9)
#define SYS_CTRL_CLR_RST_RF_IF      (1<<10)
#define SYS_CTRL_CLR_RST_VOC        (1<<11)
#define SYS_CTRL_CLR_RST_AIF        (1<<12)
#define SYS_CTRL_CLR_RST_DAI        (1<<13)
#define SYS_CTRL_CLR_RST_BB_FULL    (1<<31)
#define SYS_CTRL_CLR_BB_RST(n)      (((n)&0x3FFF)<<0)
#define SYS_CTRL_CLR_BB_RST_MASK    (0x3FFF<<0)
#define SYS_CTRL_CLR_BB_RST_SHIFT   (0)

//Clk_Sys_Mode
#define SYS_CTRL_MODE_SYS_XCPU      (1<<0)
#define SYS_CTRL_MODE_SYS_XCPU_INT_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_SYS_XCPU_INT_MANUAL (1<<1)
#define SYS_CTRL_MODE_SYS_AMBA_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_SYS_AMBA_MANUAL (1<<2)
#define SYS_CTRL_MODE_SYS_PCLK_CONF_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_SYS_PCLK_CONF_MANUAL (1<<3)
#define SYS_CTRL_MODE_SYS_PCLK_DATA_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_SYS_PCLK_DATA_MANUAL (1<<4)
#define SYS_CTRL_MODE_SYS_DMA_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_SYS_DMA_MANUAL (1<<5)
#define SYS_CTRL_MODE_SYS_EBC_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_SYS_EBC_MANUAL (1<<6)
#define SYS_CTRL_MODE_SYS_IFC_CH0_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_SYS_IFC_CH0_MANUAL (1<<7)
#define SYS_CTRL_MODE_SYS_IFC_CH1_AUTOMATIC (0<<8)
#define SYS_CTRL_MODE_SYS_IFC_CH1_MANUAL (1<<8)
#define SYS_CTRL_MODE_SYS_IFC_CH2_AUTOMATIC (0<<9)
#define SYS_CTRL_MODE_SYS_IFC_CH2_MANUAL (1<<9)
#define SYS_CTRL_MODE_SYS_IFC_CH3_AUTOMATIC (0<<10)
#define SYS_CTRL_MODE_SYS_IFC_CH3_MANUAL (1<<10)
#define SYS_CTRL_MODE_SYS_IFC_CH4_AUTOMATIC (0<<11)
#define SYS_CTRL_MODE_SYS_IFC_CH4_MANUAL (1<<11)
#define SYS_CTRL_MODE_SYS_IFC_CH5_AUTOMATIC (0<<12)
#define SYS_CTRL_MODE_SYS_IFC_CH5_MANUAL (1<<12)
#define SYS_CTRL_MODE_SYS_IFC_CH6_AUTOMATIC (0<<13)
#define SYS_CTRL_MODE_SYS_IFC_CH6_MANUAL (1<<13)
#define SYS_CTRL_MODE_SYS_IFC_CH7_AUTOMATIC (0<<14)
#define SYS_CTRL_MODE_SYS_IFC_CH7_MANUAL (1<<14)
#define SYS_CTRL_MODE_SYS_IFC_DBG_AUTOMATIC (0<<15)
#define SYS_CTRL_MODE_SYS_IFC_DBG_MANUAL (1<<15)
#define SYS_CTRL_MODE_SYS_A2A_AUTOMATIC (0<<16)
#define SYS_CTRL_MODE_SYS_A2A_MANUAL (1<<16)
#define SYS_CTRL_MODE_SYS_COM_REGS_AUTOMATIC (0<<17)
#define SYS_CTRL_MODE_SYS_COM_REGS_MANUAL (1<<17)
#define SYS_CTRL_MODE_SYS_DEBUG_UART_AUTOMATIC (0<<18)
#define SYS_CTRL_MODE_SYS_DEBUG_UART_MANUAL (1<<18)
#define SYS_CTRL_MODE_SYS_DBGHST_AUTOMATIC (0<<19)
#define SYS_CTRL_MODE_SYS_DBGHST_MANUAL (1<<19)
#define SYS_CTRL_MODE_SYS_GPADC_AUTOMATIC (0<<20)
#define SYS_CTRL_MODE_SYS_GPADC_MANUAL (1<<20)
#define SYS_CTRL_MODE_SYSD_SCI_AUTOMATIC (0<<21)
#define SYS_CTRL_MODE_SYSD_SCI_MANUAL (1<<21)
#define SYS_CTRL_MODE_SYSD_RF_SPI_AUTOMATIC (0<<22)
#define SYS_CTRL_MODE_SYSD_RF_SPI_MANUAL (1<<22)
#define SYS_CTRL_MODE_SYSD_OSC_AUTOMATIC (0<<23)
#define SYS_CTRL_MODE_SYSD_OSC_MANUAL (1<<23)
#define SYS_CTRL_MODE_SYSD_PWM_AUTOMATIC (0<<24)
#define SYS_CTRL_MODE_SYSD_PWM_MANUAL (1<<24)
#define SYS_CTRL_MODE_CLK_SYS(n)    (((n)&0xFFFFFF)<<1)
#define SYS_CTRL_MODE_CLK_SYS_MASK  (0xFFFFFF<<1)
#define SYS_CTRL_MODE_CLK_SYS_SHIFT (1)

//Clk_Sys_Enable
#define SYS_CTRL_ENABLE_SYS_XCPU    (1<<0)
#define SYS_CTRL_ENABLE_SYS_XCPU_INT (1<<1)
#define SYS_CTRL_ENABLE_SYS_AMBA    (1<<2)
#define SYS_CTRL_ENABLE_SYS_PCLK_CONF (1<<3)
#define SYS_CTRL_ENABLE_SYS_PCLK_DATA (1<<4)
#define SYS_CTRL_ENABLE_SYS_DMA     (1<<5)
#define SYS_CTRL_ENABLE_SYS_EBC     (1<<6)
#define SYS_CTRL_ENABLE_SYS_IFC_CH0 (1<<7)
#define SYS_CTRL_ENABLE_SYS_IFC_CH1 (1<<8)
#define SYS_CTRL_ENABLE_SYS_IFC_CH2 (1<<9)
#define SYS_CTRL_ENABLE_SYS_IFC_CH3 (1<<10)
#define SYS_CTRL_ENABLE_SYS_IFC_CH4 (1<<11)
#define SYS_CTRL_ENABLE_SYS_IFC_CH5 (1<<12)
#define SYS_CTRL_ENABLE_SYS_IFC_CH6 (1<<13)
#define SYS_CTRL_ENABLE_SYS_IFC_CH7 (1<<14)
#define SYS_CTRL_ENABLE_SYS_IFC_DBG (1<<15)
#define SYS_CTRL_ENABLE_SYS_A2A     (1<<16)
#define SYS_CTRL_ENABLE_SYS_COM_REGS (1<<17)
#define SYS_CTRL_ENABLE_SYS_DEBUG_UART (1<<18)
#define SYS_CTRL_ENABLE_SYS_DBGHST  (1<<19)
#define SYS_CTRL_ENABLE_SYS_GPADC   (1<<20)
#define SYS_CTRL_ENABLE_SYSD_SCI    (1<<21)
#define SYS_CTRL_ENABLE_SYSD_RF_SPI (1<<22)
#define SYS_CTRL_ENABLE_SYSD_OSC    (1<<23)
#define SYS_CTRL_ENABLE_SYSD_PWM    (1<<24)
#define SYS_CTRL_ENABLE_SYS_GPIO    (1<<25)
#define SYS_CTRL_ENABLE_SYS_IRQ     (1<<26)
#define SYS_CTRL_ENABLE_SYS_TCU     (1<<27)
#define SYS_CTRL_ENABLE_SYS_TIMER   (1<<28)
#define SYS_CTRL_ENABLE_CLK_SYS(n)  (((n)&0x1FFFFFFF)<<0)
#define SYS_CTRL_ENABLE_CLK_SYS_MASK (0x1FFFFFFF<<0)
#define SYS_CTRL_ENABLE_CLK_SYS_SHIFT (0)

//Clk_Sys_Disable
#define SYS_CTRL_DISABLE_SYS_XCPU   (1<<0)
#define SYS_CTRL_DISABLE_SYS_XCPU_INT (1<<1)
#define SYS_CTRL_DISABLE_SYS_AMBA   (1<<2)
#define SYS_CTRL_DISABLE_SYS_PCLK_CONF (1<<3)
#define SYS_CTRL_DISABLE_SYS_PCLK_DATA (1<<4)
#define SYS_CTRL_DISABLE_SYS_DMA    (1<<5)
#define SYS_CTRL_DISABLE_SYS_EBC    (1<<6)
#define SYS_CTRL_DISABLE_SYS_IFC_CH0 (1<<7)
#define SYS_CTRL_DISABLE_SYS_IFC_CH1 (1<<8)
#define SYS_CTRL_DISABLE_SYS_IFC_CH2 (1<<9)
#define SYS_CTRL_DISABLE_SYS_IFC_CH3 (1<<10)
#define SYS_CTRL_DISABLE_SYS_IFC_CH4 (1<<11)
#define SYS_CTRL_DISABLE_SYS_IFC_CH5 (1<<12)
#define SYS_CTRL_DISABLE_SYS_IFC_CH6 (1<<13)
#define SYS_CTRL_DISABLE_SYS_IFC_CH7 (1<<14)
#define SYS_CTRL_DISABLE_SYS_IFC_DBG (1<<15)
#define SYS_CTRL_DISABLE_SYS_A2A    (1<<16)
#define SYS_CTRL_DISABLE_SYS_COM_REGS (1<<17)
#define SYS_CTRL_DISABLE_SYS_DEBUG_UART (1<<18)
#define SYS_CTRL_DISABLE_SYS_DBGHST (1<<19)
#define SYS_CTRL_DISABLE_SYS_GPADC  (1<<20)
#define SYS_CTRL_DISABLE_SYSD_SCI   (1<<21)
#define SYS_CTRL_DISABLE_SYSD_RF_SPI (1<<22)
#define SYS_CTRL_DISABLE_SYSD_OSC   (1<<23)
#define SYS_CTRL_DISABLE_SYSD_PWM   (1<<24)
#define SYS_CTRL_DISABLE_SYS_GPIO   (1<<25)
#define SYS_CTRL_DISABLE_SYS_IRQ    (1<<26)
#define SYS_CTRL_DISABLE_SYS_TCU    (1<<27)
#define SYS_CTRL_DISABLE_SYS_TIMER  (1<<28)
#define SYS_CTRL_DISABLE_CLK_SYS(n) (((n)&0x1FFFFFFF)<<0)
#define SYS_CTRL_DISABLE_CLK_SYS_MASK (0x1FFFFFFF<<0)
#define SYS_CTRL_DISABLE_CLK_SYS_SHIFT (0)

//Clk_Per_Mode
#define SYS_CTRL_MODE_PER_UART_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_PER_UART_MANUAL (1<<0)
#define SYS_CTRL_MODE_PER_UART2_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_PER_UART2_MANUAL (1<<1)
#define SYS_CTRL_MODE_PER_I2C_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_PER_I2C_MANUAL (1<<2)
#define SYS_CTRL_MODE_PER_LCDC_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_PER_LCDC_MANUAL (1<<3)
#define SYS_CTRL_MODE_PER_SDMMC_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_PER_SDMMC_MANUAL (1<<4)
#define SYS_CTRL_MODE_PERD_SPI_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_PERD_SPI_MANUAL (1<<5)
#define SYS_CTRL_MODE_PERD_SPI2_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_PERD_SPI2_MANUAL (1<<6)
#define SYS_CTRL_MODE_CLK_PER(n)    (((n)&0x7F)<<0)
#define SYS_CTRL_MODE_CLK_PER_MASK  (0x7F<<0)
#define SYS_CTRL_MODE_CLK_PER_SHIFT (0)

//Clk_Per_Enable
#define SYS_CTRL_ENABLE_PER_UART    (1<<0)
#define SYS_CTRL_ENABLE_PER_UART2   (1<<1)
#define SYS_CTRL_ENABLE_PER_I2C     (1<<2)
#define SYS_CTRL_ENABLE_PER_LCDC    (1<<3)
#define SYS_CTRL_ENABLE_PER_SDMMC   (1<<4)
#define SYS_CTRL_ENABLE_PERD_SPI    (1<<5)
#define SYS_CTRL_ENABLE_PERD_SPI2   (1<<6)
#define SYS_CTRL_ENABLE_PER_USBC    (1<<7)
#define SYS_CTRL_ENABLE_PER_CAMERA  (1<<8)
#define SYS_CTRL_ENABLE_PER_SPY     (1<<9)
#define SYS_CTRL_ENABLE_PER_TEST    (1<<10)
#define SYS_CTRL_ENABLE_CLK_PER(n)  (((n)&0x7FF)<<0)
#define SYS_CTRL_ENABLE_CLK_PER_MASK (0x7FF<<0)
#define SYS_CTRL_ENABLE_CLK_PER_SHIFT (0)

//Clk_Per_Disable
#define SYS_CTRL_DISABLE_PER_UART   (1<<0)
#define SYS_CTRL_DISABLE_PER_UART2  (1<<1)
#define SYS_CTRL_DISABLE_PER_I2C    (1<<2)
#define SYS_CTRL_DISABLE_PER_LCDC   (1<<3)
#define SYS_CTRL_DISABLE_PER_SDMMC  (1<<4)
#define SYS_CTRL_DISABLE_PERD_SPI   (1<<5)
#define SYS_CTRL_DISABLE_PERD_SPI2  (1<<6)
#define SYS_CTRL_DISABLE_PER_USBC   (1<<7)
#define SYS_CTRL_DISABLE_PER_CAMERA (1<<8)
#define SYS_CTRL_DISABLE_PER_SPY    (1<<9)
#define SYS_CTRL_DISABLE_PER_TEST   (1<<10)
#define SYS_CTRL_DISABLE_CLK_PER(n) (((n)&0x7FF)<<0)
#define SYS_CTRL_DISABLE_CLK_PER_MASK (0x7FF<<0)
#define SYS_CTRL_DISABLE_CLK_PER_SHIFT (0)

//Clk_BB_Mode
#define SYS_CTRL_MODE_BB_BCPU       (1<<0)
#define SYS_CTRL_MODE_BB_BCPU_INT_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_BB_BCPU_INT_MANUAL (1<<1)
#define SYS_CTRL_MODE_BB_AMBA_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_BB_AMBA_MANUAL (1<<2)
#define SYS_CTRL_MODE_BB_PCLK_CONF_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_BB_PCLK_CONF_MANUAL (1<<3)
#define SYS_CTRL_MODE_BB_PCLK_DATA_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_BB_PCLK_DATA_MANUAL (1<<4)
#define SYS_CTRL_MODE_BB_VOC_AHB_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_BB_VOC_AHB_MANUAL (1<<5)
#define SYS_CTRL_MODE_BB_IFC_CH0_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_BB_IFC_CH0_MANUAL (1<<6)
#define SYS_CTRL_MODE_BB_IFC_CH1_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_BB_IFC_CH1_MANUAL (1<<7)
#define SYS_CTRL_MODE_BB_IFC_CH2_AUTOMATIC (0<<8)
#define SYS_CTRL_MODE_BB_IFC_CH2_MANUAL (1<<8)
#define SYS_CTRL_MODE_BB_SRAM_AUTOMATIC (0<<9)
#define SYS_CTRL_MODE_BB_SRAM_MANUAL (1<<9)
#define SYS_CTRL_MODE_BB_A2A_AUTOMATIC (0<<10)
#define SYS_CTRL_MODE_BB_A2A_MANUAL (1<<10)
#define SYS_CTRL_MODE_BB_ITLV_AUTOMATIC (0<<11)
#define SYS_CTRL_MODE_BB_ITLV_MANUAL (1<<11)
#define SYS_CTRL_MODE_BB_VITERBI_AUTOMATIC (0<<12)
#define SYS_CTRL_MODE_BB_VITERBI_MANUAL (1<<12)
#define SYS_CTRL_MODE_BB_CIPHER_AUTOMATIC (0<<13)
#define SYS_CTRL_MODE_BB_CIPHER_MANUAL (1<<13)
#define SYS_CTRL_MODE_BB_RF_IF_AUTOMATIC (0<<14)
#define SYS_CTRL_MODE_BB_RF_IF_MANUAL (1<<14)
#define SYS_CTRL_MODE_BB_COPRO_AUTOMATIC (0<<15)
#define SYS_CTRL_MODE_BB_COPRO_MANUAL (1<<15)
#define SYS_CTRL_MODE_BB_CP2_REG_AUTOMATIC (0<<16)
#define SYS_CTRL_MODE_BB_CP2_REG_MANUAL (1<<16)
#define SYS_CTRL_MODE_BB_XCOR_AUTOMATIC (0<<17)
#define SYS_CTRL_MODE_BB_XCOR_MANUAL (1<<17)
#define SYS_CTRL_MODE_BB_AIF_AUTOMATIC (0<<18)
#define SYS_CTRL_MODE_BB_AIF_MANUAL (1<<18)
#define SYS_CTRL_MODE_BBD_DAIS_208K_AUTOMATIC (0<<19)
#define SYS_CTRL_MODE_BBD_DAIS_208K_MANUAL (1<<19)
#define SYS_CTRL_MODE_CLK_BB(n)     (((n)&0x7FFFF)<<1)
#define SYS_CTRL_MODE_CLK_BB_MASK   (0x7FFFF<<1)
#define SYS_CTRL_MODE_CLK_BB_SHIFT  (1)

//Clk_BB_Enable
#define SYS_CTRL_ENABLE_BB_BCPU     (1<<0)
#define SYS_CTRL_ENABLE_BB_BCPU_INT (1<<1)
#define SYS_CTRL_ENABLE_BB_AMBA     (1<<2)
#define SYS_CTRL_ENABLE_BB_PCLK_CONF (1<<3)
#define SYS_CTRL_ENABLE_BB_PCLK_DATA (1<<4)
#define SYS_CTRL_ENABLE_BB_VOC_AHB  (1<<5)
#define SYS_CTRL_ENABLE_BB_IFC_CH0  (1<<6)
#define SYS_CTRL_ENABLE_BB_IFC_CH1  (1<<7)
#define SYS_CTRL_ENABLE_BB_IFC_CH2  (1<<8)
#define SYS_CTRL_ENABLE_BB_SRAM     (1<<9)
#define SYS_CTRL_ENABLE_BB_A2A      (1<<10)
#define SYS_CTRL_ENABLE_BB_ITLV     (1<<11)
#define SYS_CTRL_ENABLE_BB_VITERBI  (1<<12)
#define SYS_CTRL_ENABLE_BB_CIPHER   (1<<13)
#define SYS_CTRL_ENABLE_BB_RF_IF    (1<<14)
#define SYS_CTRL_ENABLE_BB_COPRO    (1<<15)
#define SYS_CTRL_ENABLE_BB_CP2_REG  (1<<16)
#define SYS_CTRL_ENABLE_BB_XCOR     (1<<17)
#define SYS_CTRL_ENABLE_BB_AIF      (1<<18)
#define SYS_CTRL_ENABLE_BBD_DAIS_208K (1<<19)
#define SYS_CTRL_ENABLE_BB_IRQ      (1<<20)
#define SYS_CTRL_ENABLE_CLK_BB(n)   (((n)&0x1FFFFF)<<0)
#define SYS_CTRL_ENABLE_CLK_BB_MASK (0x1FFFFF<<0)
#define SYS_CTRL_ENABLE_CLK_BB_SHIFT (0)

//Clk_BB_Disable
#define SYS_CTRL_DISABLE_BB_BCPU    (1<<0)
#define SYS_CTRL_DISABLE_BB_BCPU_INT (1<<1)
#define SYS_CTRL_DISABLE_BB_AMBA    (1<<2)
#define SYS_CTRL_DISABLE_BB_PCLK_CONF (1<<3)
#define SYS_CTRL_DISABLE_BB_PCLK_DATA (1<<4)
#define SYS_CTRL_DISABLE_BB_VOC_AHB (1<<5)
#define SYS_CTRL_DISABLE_BB_IFC_CH0 (1<<6)
#define SYS_CTRL_DISABLE_BB_IFC_CH1 (1<<7)
#define SYS_CTRL_DISABLE_BB_IFC_CH2 (1<<8)
#define SYS_CTRL_DISABLE_BB_SRAM    (1<<9)
#define SYS_CTRL_DISABLE_BB_A2A     (1<<10)
#define SYS_CTRL_DISABLE_BB_ITLV    (1<<11)
#define SYS_CTRL_DISABLE_BB_VITERBI (1<<12)
#define SYS_CTRL_DISABLE_BB_CIPHER  (1<<13)
#define SYS_CTRL_DISABLE_BB_RF_IF   (1<<14)
#define SYS_CTRL_DISABLE_BB_COPRO   (1<<15)
#define SYS_CTRL_DISABLE_BB_CP2_REG (1<<16)
#define SYS_CTRL_DISABLE_BB_XCOR    (1<<17)
#define SYS_CTRL_DISABLE_BB_AIF     (1<<18)
#define SYS_CTRL_DISABLE_BBD_DAIS_208K (1<<19)
#define SYS_CTRL_DISABLE_BB_IRQ     (1<<20)
#define SYS_CTRL_DISABLE_CLK_BB(n)  (((n)&0x1FFFFF)<<0)
#define SYS_CTRL_DISABLE_CLK_BB_MASK (0x1FFFFF<<0)
#define SYS_CTRL_DISABLE_CLK_BB_SHIFT (0)

//Clk_Other_Mode
#define SYS_CTRL_MODE_OC_DEBUG_UART_AUTOMATIC (0<<0)
#define SYS_CTRL_MODE_OC_DEBUG_UART_MANUAL (1<<0)
#define SYS_CTRL_MODE_OC_BCK_AUTOMATIC (0<<1)
#define SYS_CTRL_MODE_OC_BCK_MANUAL (1<<1)
#define SYS_CTRL_MODE_OC_UART_AUTOMATIC (0<<2)
#define SYS_CTRL_MODE_OC_UART_MANUAL (1<<2)
#define SYS_CTRL_MODE_OC_UART2_AUTOMATIC (0<<3)
#define SYS_CTRL_MODE_OC_UART2_MANUAL (1<<3)
#define SYS_CTRL_MODE_OC_RF_RX_AUTOMATIC (0<<4)
#define SYS_CTRL_MODE_OC_RF_RX_MANUAL (1<<4)
#define SYS_CTRL_MODE_OC_RF_TX_AUTOMATIC (0<<5)
#define SYS_CTRL_MODE_OC_RF_TX_MANUAL (1<<5)
#define SYS_CTRL_MODE_OC_MEM_BRIDGE_AUTOMATIC (0<<6)
#define SYS_CTRL_MODE_OC_MEM_BRIDGE_MANUAL (1<<6)
#define SYS_CTRL_MODE_OC_VOC_CORE_AUTOMATIC (0<<7)
#define SYS_CTRL_MODE_OC_VOC_CORE_MANUAL (1<<7)
#define SYS_CTRL_MODE_OC_VOC_AHB_AUTOMATIC (0<<8)
#define SYS_CTRL_MODE_OC_VOC_AHB_MANUAL (1<<8)
#define SYS_CTRL_MODE_OC_VOC_DAI_AUTOMATIC (0<<9)
#define SYS_CTRL_MODE_OC_VOC_DAI_MANUAL (1<<9)
#define SYS_CTRL_MODE_CLK_OTHER(n)  (((n)&0x3FF)<<0)
#define SYS_CTRL_MODE_CLK_OTHER_MASK (0x3FF<<0)
#define SYS_CTRL_MODE_CLK_OTHER_SHIFT (0)

//Clk_Other_Enable
#define SYS_CTRL_ENABLE_OC_DEBUG_UART (1<<0)
#define SYS_CTRL_ENABLE_OC_BCK      (1<<1)
#define SYS_CTRL_ENABLE_OC_UART     (1<<2)
#define SYS_CTRL_ENABLE_OC_UART2    (1<<3)
#define SYS_CTRL_ENABLE_OC_RF_RX    (1<<4)
#define SYS_CTRL_ENABLE_OC_RF_TX    (1<<5)
#define SYS_CTRL_ENABLE_OC_MEM_BRIDGE (1<<6)
#define SYS_CTRL_ENABLE_OC_VOC_CORE (1<<7)
#define SYS_CTRL_ENABLE_OC_VOC_AHB  (1<<8)
#define SYS_CTRL_ENABLE_OC_VOC_DAI  (1<<9)
#define SYS_CTRL_ENABLE_OC_VOC      (1<<10)
#define SYS_CTRL_ENABLE_OC_LPS      (1<<11)
#define SYS_CTRL_ENABLE_OC_GPIO     (1<<12)
#define SYS_CTRL_ENABLE_OC_USBPHY   (1<<13)
#define SYS_CTRL_ENABLE_OC_CAMERA   (1<<14)
#define SYS_CTRL_ENABLE_OC_CLK_OUT  (1<<15)
#define SYS_CTRL_ENABLE_OC_MEM_CLK_OUT (1<<16)
#define SYS_CTRL_ENABLE_CLK_OTHER(n) (((n)&0x1FFFF)<<0)
#define SYS_CTRL_ENABLE_CLK_OTHER_MASK (0x1FFFF<<0)
#define SYS_CTRL_ENABLE_CLK_OTHER_SHIFT (0)

//Clk_Other_Disable
#define SYS_CTRL_DISABLE_OC_DEBUG_UART (1<<0)
#define SYS_CTRL_DISABLE_OC_BCK     (1<<1)
#define SYS_CTRL_DISABLE_OC_UART    (1<<2)
#define SYS_CTRL_DISABLE_OC_UART2   (1<<3)
#define SYS_CTRL_DISABLE_OC_RF_RX   (1<<4)
#define SYS_CTRL_DISABLE_OC_RF_TX   (1<<5)
#define SYS_CTRL_DISABLE_OC_MEM_BRIDGE (1<<6)
#define SYS_CTRL_DISABLE_OC_VOC_CORE (1<<7)
#define SYS_CTRL_DISABLE_OC_VOC_AHB (1<<8)
#define SYS_CTRL_DISABLE_OC_VOC_DAI (1<<9)
#define SYS_CTRL_DISABLE_OC_VOC     (1<<10)
#define SYS_CTRL_DISABLE_OC_LPS     (1<<11)
#define SYS_CTRL_DISABLE_OC_GPIO    (1<<12)
#define SYS_CTRL_DISABLE_OC_USBPHY  (1<<13)
#define SYS_CTRL_DISABLE_OC_CAMERA  (1<<14)
#define SYS_CTRL_DISABLE_OC_CLK_OUT (1<<15)
#define SYS_CTRL_DISABLE_OC_MEM_CLK_OUT (1<<16)
#define SYS_CTRL_DISABLE_CLK_OTHER(n) (((n)&0x1FFFF)<<0)
#define SYS_CTRL_DISABLE_CLK_OTHER_MASK (0x1FFFF<<0)
#define SYS_CTRL_DISABLE_CLK_OTHER_SHIFT (0)

//Sel_Clock
#define SYS_CTRL_SLOW_SEL_RF_OSCILLATOR (1<<0)
#define SYS_CTRL_SLOW_SEL_RF_RF     (0<<0)
#define SYS_CTRL_SYS_SEL_FAST_SLOW  (1<<4)
#define SYS_CTRL_SYS_SEL_FAST_FAST  (0<<4)
#define SYS_CTRL_SYS_SEL_PLL_ACCO   (1<<8)
#define SYS_CTRL_SYS_SEL_PLL_EXT    (0<<8)
#define SYS_CTRL_CLOCKOUT_SEL_RF_MASK (1<<12)
#define SYS_CTRL_CLOCKOUT_SEL_RF_OSCILLATOR (1<<12)
#define SYS_CTRL_CLOCKOUT_SEL_RF_RF (0<<12)
#define SYS_CTRL_CLOCK_PLL_UART_SEL_104MHZ (1<<16)
#define SYS_CTRL_CLOCK_PLL_UART_SEL_52MHZ (0<<16)
#define SYS_CTRL_RF_DETECTED_OK     (1<<20)
#define SYS_CTRL_RF_DETECTED_NO     (0<<20)
#define SYS_CTRL_RF_DETECT_BYPASS   (1<<21)
#define SYS_CTRL_RF_DETECT_RESET    (1<<22)
#define SYS_CTRL_RF_SELECTED_L      (1<<23)
#define SYS_CTRL_EXT_PLL_LOCKED     (1<<24)
#define SYS_CTRL_ACCO_PLL_LOCKED    (1<<25)
#define SYS_CTRL_PLL_BYPASS_LOCK    (1<<27)
#define SYS_CTRL_SYS_PLL_SELECTED_ACCO (1<<30)
#define SYS_CTRL_SYS_PLL_SELECTED_EXT (0<<30)
#define SYS_CTRL_FAST_SELECTED_L    (1<<31)
#define SYS_CTRL_CLOCK_SELECT(n)    (((n)&273)<<0)
#define SYS_CTRL_CLOCK_SELECT_MASK  (273<<0)
#define SYS_CTRL_CLOCK_SELECT_SHIFT (0)

//Cfg_Clk_Sys
#define SYS_CTRL_FREQ(n)            (((n)&7)<<0)
#define SYS_CTRL_FREQ_MASK          (7<<0)
#define SYS_CTRL_FREQ_SHIFT         (0)
#define SYS_CTRL_FREQ_13M           (0<<0)
#define SYS_CTRL_FREQ_26M           (1<<0)
#define SYS_CTRL_FREQ_39M           (2<<0)
#define SYS_CTRL_FREQ_52M           (3<<0)
#define SYS_CTRL_FREQ_78M           (4<<0)
#define SYS_CTRL_FREQ_104M          (5<<0)
#define SYS_CTRL_FORCE_DIV_UPDATE   (1<<4)
#define SYS_CTRL_REQ_DIV_UPDATE     (1<<8)

//Cfg_Clk_Mem_Bridge
//#define SYS_CTRL_FREQ(n)          (((n)&7)<<0)
//#define SYS_CTRL_FREQ_MASK        (7<<0)
//#define SYS_CTRL_FREQ_SHIFT       (0)
//#define SYS_CTRL_FREQ_13M         (0<<0)
//#define SYS_CTRL_FREQ_26M         (1<<0)
//#define SYS_CTRL_FREQ_39M         (2<<0)
//#define SYS_CTRL_FREQ_52M         (3<<0)
//#define SYS_CTRL_FREQ_78M         (4<<0)
//#define SYS_CTRL_FREQ_104M        (5<<0)

//Cfg_Clk_Voc
//#define SYS_CTRL_FREQ(n)          (((n)&7)<<0)
//#define SYS_CTRL_FREQ_MASK        (7<<0)
//#define SYS_CTRL_FREQ_SHIFT       (0)
//#define SYS_CTRL_FREQ_13M         (0<<0)
//#define SYS_CTRL_FREQ_26M         (1<<0)
//#define SYS_CTRL_FREQ_39M         (2<<0)
//#define SYS_CTRL_FREQ_52M         (3<<0)
#define SYS_CTRL_VOC_SEL_PLL_SLOW   (1<<3)
#define SYS_CTRL_VOC_SEL_PLL_PLL    (0<<3)

//Cfg_Clk_AudioBCK_Div
#define SYS_CTRL_AUDIOBCK_DIVIDER(n) (((n)&0x7FF)<<0)

//Cfg_Clk_Uart[0]
#define SYS_CTRL_UART_DIVIDER(n)    (((n)&0x3FF)<<0)
#define SYS_CTRL_UART_DIVIDER_MASK  (0x3FF<<0)
#define SYS_CTRL_UART_DIVIDER_SHIFT (0)
#define SYS_CTRL_UART_SEL_PLL_SLOW  (0<<12)
#define SYS_CTRL_UART_SEL_PLL_PLL   (1<<12)

//Cfg_Clk_PWM
#define SYS_CTRL_PWM_DIVIDER(n)     (((n)&0xFF)<<0)

//Cfg_AHB
#define SYS_CTRL_ENABLE_SYS_MID_BIST_ENABLE (1<<0)
#define SYS_CTRL_ENABLE_SYS_MID_BIST_DISABLE (0<<0)
#define SYS_CTRL_ENABLE_SYS_MID_DMA_ENABLE (1<<1)
#define SYS_CTRL_ENABLE_SYS_MID_DMA_DISABLE (0<<1)
#define SYS_CTRL_ENABLE_SYS_MID_XCPU_ENABLE (1<<2)
#define SYS_CTRL_ENABLE_SYS_MID_XCPU_DISABLE (0<<2)
#define SYS_CTRL_ENABLE_SYS_MID_AHB2AHB_ENABLE (1<<3)
#define SYS_CTRL_ENABLE_SYS_MID_AHB2AHB_DISABLE (0<<3)
#define SYS_CTRL_ENABLE_SYS_MID_IFC_ENABLE (1<<4)
#define SYS_CTRL_ENABLE_SYS_MID_IFC_DISABLE (0<<4)
#define SYS_CTRL_ENABLE_SYS_MID_USB_ENABLE (1<<5)
#define SYS_CTRL_ENABLE_SYS_MID_USB_DISABLE (0<<5)
#define SYS_CTRL_ENABLE_BB_MID_BIST_ENABLE (1<<16)
#define SYS_CTRL_ENABLE_BB_MID_BIST_DISABLE (0<<16)
#define SYS_CTRL_ENABLE_BB_MID_VOC_ENABLE (1<<17)
#define SYS_CTRL_ENABLE_BB_MID_VOC_DISABLE (0<<17)
#define SYS_CTRL_ENABLE_BB_MID_BCPU_ENABLE (1<<18)
#define SYS_CTRL_ENABLE_BB_MID_BCPU_DISABLE (0<<18)
#define SYS_CTRL_ENABLE_BB_MID_AHB2AHB_ENABLE (1<<19)
#define SYS_CTRL_ENABLE_BB_MID_AHB2AHB_DISABLE (0<<19)
#define SYS_CTRL_ENABLE_BB_MID_IFC_ENABLE (1<<20)
#define SYS_CTRL_ENABLE_BB_MID_IFC_DISABLE (0<<20)
#define SYS_CTRL_USB_RAM_TEST_MODE_ENABLE (1<<31)
#define SYS_CTRL_USB_RAM_TEST_MODE_DISABLE (0<<31)
#define SYS_CTRL_SYS_ENABLE(n)      (((n)&0x3F)<<0)
#define SYS_CTRL_SYS_ENABLE_MASK    (0x3F<<0)
#define SYS_CTRL_SYS_ENABLE_SHIFT   (0)
#define SYS_CTRL_BB_ENABLE(n)       (((n)&31)<<16)
#define SYS_CTRL_BB_ENABLE_MASK     (31<<16)
#define SYS_CTRL_BB_ENABLE_SHIFT    (16)

//XCpu_Dbg_BKP
#define SYS_CTRL_BKPT_EN            (1<<0)
#define SYS_CTRL_BKPT_MODE(n)       (((n)&3)<<4)
#define SYS_CTRL_BKPT_MODE_I        (0<<4)
#define SYS_CTRL_BKPT_MODE_R        (1<<4)
#define SYS_CTRL_BKPT_MODE_W        (2<<4)
#define SYS_CTRL_BKPT_MODE_RW       (3<<4)
#define SYS_CTRL_STALLED            (1<<8)

//XCpu_Dbg_Addr
#define SYS_CTRL_BREAKPOINT_ADDRESS(n) (((n)&0x1FFFFFF)<<0)

//BCpu_Dbg_BKP
//#define SYS_CTRL_BKPT_EN          (1<<0)
//#define SYS_CTRL_BKPT_MODE(n)     (((n)&3)<<4)
//#define SYS_CTRL_BKPT_MODE_I      (0<<4)
//#define SYS_CTRL_BKPT_MODE_R      (1<<4)
//#define SYS_CTRL_BKPT_MODE_W      (2<<4)
//#define SYS_CTRL_BKPT_MODE_RW     (3<<4)
//#define SYS_CTRL_STALLED          (1<<8)

//BCpu_Dbg_Addr
//#define SYS_CTRL_BREAKPOINT_ADDRESS(n) (((n)&0x1FFFFFF)<<0)

//Cfg_Cpus_Cache_Ram_Disable
#define SYS_CTRL_XCPU_USE_MODE      (1<<0)
#define SYS_CTRL_XCPU_CLK_OFF_MODE  (1<<1)
#define SYS_CTRL_BCPU_USE_MODE      (1<<16)
#define SYS_CTRL_BCPU_CLK_OFF_MODE  (1<<17)
#define SYS_CTRL_XCPU_CACHE_RAM_DISABLE(n) (((n)&3)<<0)
#define SYS_CTRL_XCPU_CACHE_RAM_DISABLE_MASK (3<<0)
#define SYS_CTRL_XCPU_CACHE_RAM_DISABLE_SHIFT (0)
#define SYS_CTRL_BCPU_CACHE_RAM_DISABLE(n) (((n)&3)<<16)
#define SYS_CTRL_BCPU_CACHE_RAM_DISABLE_MASK (3<<16)
#define SYS_CTRL_BCPU_CACHE_RAM_DISABLE_SHIFT (16)

//Reset_Cause
#define SYS_CTRL_WATCHDOG_RESET_HAPPENED (1<<0)
#define SYS_CTRL_WATCHDOG_RESET_NO  (0<<0)
#define SYS_CTRL_GLOBALSOFT_RESET_HAPPENED (1<<4)
#define SYS_CTRL_GLOBALSOFT_RESET_NO (0<<4)
#define SYS_CTRL_HOSTDEBUG_RESET_HAPPENED (1<<8)
#define SYS_CTRL_HOSTDEBUG_RESET_NO (0<<8)
#define SYS_CTRL_ALARMCAUSE_HAPPENED (1<<12)
#define SYS_CTRL_ALARMCAUSE_NO      (0<<12)
#define SYS_CTRL_BOOT_MODE(n)       (((n)&0x3F)<<16)
#define SYS_CTRL_BOOT_MODE_MASK     (0x3F<<16)
#define SYS_CTRL_BOOT_MODE_SHIFT    (16)
#define SYS_CTRL_FONCTIONAL_TEST_MODE (1<<31)

//WakeUp
#define SYS_CTRL_FORCE_WAKEUP       (1<<0)





#endif
