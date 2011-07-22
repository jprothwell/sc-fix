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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/debug_port.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _DEBUG_PORT_H_
#define _DEBUG_PORT_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'debug_port'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

typedef enum {
    DBG_SEL_SYS_NOGATE                          = 0x00000000,
    DBG_SEL_SYSD_SCI                            = 0x00000001,
    DBG_SEL_SYSD_SPI                            = 0x00000002,
    DBG_SEL_SYSD_SPI2                           = 0x00000003,
    DBG_SEL_SYSD_RF_SPI                         = 0x00000004,
    DBG_SEL_SYSD_OSC                            = 0x00000005,
    DBG_SEL_SYSD_PWM                            = 0x00000006,
    DBG_SEL_BBD_DAIS_208K                       = 0x00000007,
    DBG_SEL_HOST                                = 0x00000008,
    DBG_SEL_AUDIO_PLL                           = 0x00000009,
    DBG_SEL_AUDIO_BCK                           = 0x0000000A,
    DBG_SEL_UART                                = 0x0000000B,
    DBG_SEL_UART2                               = 0x0000000C,
    DBG_SEL_RF_RX                               = 0x0000000D,
    DBG_SEL_RF_TX                               = 0x0000000E,
    DBG_SEL_MEM_BRIDGE                          = 0x0000000F,
    DBG_SEL_VOC                                 = 0x00000010,
    DBG_SEL_32K                                 = 0x00000011,
    DBG_SEL_USBPHY                              = 0x00000012,
    DBG_SEL_CAMERA                              = 0x00000013,
/// this clock is taken before clock root
    DBG_SEL_OSC                                 = 0x00000014,
/// this clock is taken before clock root
    DBG_SEL_REF                                 = 0x00000015,
/// this clock is taken before clock root
    DBG_SEL_SLOW                                = 0x00000016,
/// this clock is taken before clock root
    DBG_SEL_FAST_PLL                            = 0x00000017,
/// this clock is taken before clock root
    DBG_SEL_FAST_ACCO_PLL                       = 0x00000018,
/// this clock is taken before clock root
    DBG_SEL_FAST_EXT_PLL                        = 0x00000019,
/// this clock is taken before clock root
    DBG_SEL_VOC_PLL                             = 0x0000001A,
/// this clock is taken before clock root
    DBG_SEL_UART_PLL                            = 0x0000001B,
    DBG_SEL_AU_CLK_520K                         = 0x0000001C,
    DBG_SEL_AU_CLK_40K                          = 0x0000001D,
    DBG_SEL_PADAC_CLK                           = 0x0000001E,
    DBG_SEL_AFCDAC_CLK                          = 0x0000001F,
    DBG_SEL_GPADC_CLK                           = 0x00000020,
    DBG_SEL_TX_CLK_FS                           = 0x00000021,
    DBG_SEL_RX_CLK_FS                           = 0x00000022,
/// this clock is taken outside granite ip
    DBG_SEL_EXT_DBG                             = 0x00000023
} DEBUG_MUX_CLOCKS_T;

#define NB_CLK_DBG_SEL                           (36)

typedef enum {
    DBG_SYS_IRQ_TCU0                            = 0x00000000,
    DBG_SYS_IRQ_TCU1                            = 0x00000001,
    DBG_SYS_IRQ_FRAME                           = 0x00000002,
    DBG_SYS_IRQ_DMA_TX_SDMMC                    = 0x00000003,
    DBG_SYS_IRQ_DMA_RX_SDMMC                    = 0x00000004,
    DBG_SYS_IRQ_DMA_TX_LCDC                     = 0x00000005,
    DBG_SYS_IRQ_DMA_RX_CAMERA                   = 0x00000006,
    DBG_SYS_IRQ_COM0                            = 0x00000007,
    DBG_SYS_IRQ_COM1                            = 0x00000008,
    DBG_SYS_IRQ_VOC                             = 0x00000009,
    DBG_SYS_IRQ_DMA                             = 0x0000000A,
    DBG_SYS_IRQ_GPIO                            = 0x0000000B,
    DBG_SYS_IRQ_KEYPAD                          = 0x0000000C,
    DBG_SYS_IRQ_TIMERS                          = 0x0000000D,
    DBG_SYS_IRQ_OS_TIMER                        = 0x0000000E,
    DBG_SYS_IRQ_CALENDAR                        = 0x0000000F,
    DBG_SYS_IRQ_SPI                             = 0x00000010,
    DBG_SYS_IRQ_SPI2                            = 0x00000011,
    DBG_SYS_IRQ_DEBUG_UART                      = 0x00000012,
    DBG_SYS_IRQ_UART                            = 0x00000013,
    DBG_SYS_IRQ_UART2                           = 0x00000014,
    DBG_SYS_IRQ_I2C                             = 0x00000015,
    DBG_SYS_IRQ_SCI                             = 0x00000016,
    DBG_SYS_IRQ_RF_SPI                          = 0x00000017,
    DBG_SYS_IRQ_LPS                             = 0x00000018,
    DBG_SYS_IRQ_BBIFC0                          = 0x00000019,
    DBG_SYS_IRQ_BBIFC1                          = 0x0000001A,
    DBG_SYS_IRQ_USBC                            = 0x0000001B,
    DBG_SYS_IRQ_LCDC                            = 0x0000001C,
    DBG_SYS_IRQ_SDMMC                           = 0x0000001D,
    DBG_SYS_IRQ_CAMERA                          = 0x0000001E,
    DBG_SYS_IRQ_EXTAPB                          = 0x0000001F,
    DBG_SYS_HREADY                              = 0x00000020,
    DBG_XCPU_I_FETCH                            = 0x00000021,
    DBG_XCPU_I_MISS                             = 0x00000022,
    DBG_XCPU_D_FETCH                            = 0x00000023,
    DBG_XCPU_D_MISS                             = 0x00000024,
    DBG_XCPU_MAIN_IRQ                           = 0x00000025,
    DBG_BB_IRQ_TCU0                             = 0x00000040,
    DBG_BB_IRQ_TCU1                             = 0x00000041,
    DBG_BB_IRQ_FRAME                            = 0x00000042,
    DBG_BB_IRQ_COM0                             = 0x00000043,
    DBG_BB_IRQ_COM1                             = 0x00000044,
    DBG_BB_IRQ_VOC                              = 0x00000045,
    DBG_BB_IRQ_IFC0                             = 0x00000046,
    DBG_BB_IRQ_IFC1                             = 0x00000047,
    DBG_BB_IRQ_IFC2                             = 0x00000048,
    DBG_BB_IRQ_RF_IF                            = 0x00000049,
    DBG_BB_IRQ_ITLV                             = 0x0000004A,
    DBG_BB_IRQ_VITAC                            = 0x0000004B,
    DBG_BB_IRQ_XCOR                             = 0x0000004C,
    DBG_SYS_IRQ_PAGE_SPY                        = 0x00000053,
    DBG_EXT_PLL_LOCKED                          = 0x00000054,
    DBG_ACCO_PLL_LOCKED                         = 0x00000055,
    DBG_WATCH0                                  = 0x00000056,
    DBG_WATCH1                                  = 0x00000057,
    DBG_WATCH2                                  = 0x00000058,
    DBG_WATCH3                                  = 0x00000059,
    DBG_DIGRF_STROBE                            = 0x0000005A,
    DBG_RX_SOC                                  = 0x0000005B,
    DBG_VOC_DMA_BUSY                            = 0x0000005C,
    DBG_VOC_BUSY                                = 0x0000005D,
    DBG_SYS_EBC_BUSY                            = 0x0000005E,
    DBG_BB_EBC_BUSY                             = 0x0000005F,
    DBG_BB_HREADY                               = 0x00000060,
    DBG_BCPU_I_FETCH                            = 0x00000061,
    DBG_BCPU_I_MISS                             = 0x00000062,
    DBG_BCPU_D_FETCH                            = 0x00000063,
    DBG_BCPU_D_MISS                             = 0x00000064,
    DBG_BCPU_MAIN_IRQ                           = 0x00000065
} DEBUG_SIGNAL_SELECT_T;


// =============================================================================
//  TYPES
// =============================================================================
// ============================================================================
// DEBUG_PORT_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_DEBUG_PORT_BASE         0x00B8D000


typedef volatile struct
{
    /// All this register is locked by the host_lock_reg signal
    REG32                          Debug_Port_Mode;              //0x00000000
    /// All this register is locked by the host_lock_reg signal
    REG32                          Pxts_Exl_Cfg;                 //0x00000004
    REG32                          WD_Addr_cfg;                  //0x00000008
    /// <table border="1" > <tr> <th>Pin</th> <th>MODE_SYS</th> <th>MODE_BB</th>
    /// <th>MODE_PA</th> <th>MODE_others</th> </tr> <tr> <td>DBG_BUS_STB</td> <td
    /// colspan="4" >FInt_H</td> </tr> <tr> <td>DBG_BUS_0</td> <td colspan="4" >GMSK_ON
    /// / RX_ON</td> </tr> <tr> <td>DBG_BUS_1</td> <td colspan="4" >RX_DEC_ON / TX_ON</td>
    /// </tr> <tr> <td>DBG_BUS_2</td> <td>Xcpu_Main_irq_L</td> <td>Vitac_Stall or
    /// ITLV stall or Xcor Stall</td> <td>PA_RAMP(0)</td> <td>Next Fc_h</td> </tr>
    /// <tr> <td>DBG_BUS_3</td> <td>Bcpu_Main_irq_L</td> <td>VITAC Busy</td> <td>PA_RAMP(1)</td>
    /// <td>Send_Spi_CMD</td> </tr> <tr> <td>DBG_BUS_4</td> <td>Sys_IntEnabled_H</td>
    /// <td>XCOR Busy</td> <td>PA_RAMP(2)</td> <td>First Gain_H</td> </tr> <tr> <td>DBG_BUS_5</td>
    /// <td>BB_IntEnabled_H</td> <td>ITLV Busy</td> <td>PA_NEW_RAMP</td> <td>Next
    /// Gain_H</td> </tr> <tr> <td>DBG_BUS_6</td> <td colspan="4" >Signal_Select0</td>
    /// </tr> <tr> <td>DBG_BUS_7</td> <td colspan="4" >Signal_Select1</td> </tr>
    /// </table>
    REG32                          Signal_Spy_Cfg;               //0x0000000C
    REG32                          Dbg_Clk_Cfg;                  //0x00000010
    REG32                          Dbg_Watch_Set;                //0x00000014
    REG32                          Dbg_Watch_Clr;                //0x00000018
    REG32 Reserved_0000001C[9];                 //0x0000001C
    REG32                          Pxts_tag[16];                 //0x00000040
} HWP_DEBUG_PORT_T;

#define hwp_debugPort               ((HWP_DEBUG_PORT_T*) KSEG1(REG_DEBUG_PORT_BASE))


//Debug_Port_Mode
#define DEBUG_PORT_MODE_CONFIG(n)   (((n)&7)<<0)
#define DEBUG_PORT_MODE_CONFIG_MASK (7<<0)
#define DEBUG_PORT_MODE_CONFIG_SHIFT (0)
#define DEBUG_PORT_MODE_CONFIG_MODE_PXTS_EXL_OFF (0<<0)
#define DEBUG_PORT_MODE_CONFIG_MODE_EXL (1<<0)
#define DEBUG_PORT_MODE_CONFIG_MODE_PXTS (2<<0)
#define DEBUG_PORT_MODE_CONFIG_MODE_PXTS_EXL (3<<0)
#define DEBUG_PORT_MODE_CONFIG_MODE_SIGNAL_SPY (4<<0)
#define DEBUG_PORT_PXTS_EXL_CLOCK_POL (1<<3)
#define DEBUG_PORT_PXTS_FIFO_EMPTY  (1<<4)
#define DEBUG_PORT_PXTS_FIFO_OVF    (1<<5)
#define DEBUG_PORT_EXL_FIFO_EMPTY   (1<<8)
#define DEBUG_PORT_EXL_FIFO_OVF     (1<<9)

//Pxts_Exl_Cfg
#define DEBUG_PORT_RECORD_TRIGGER_TYPE(n) (((n)&15)<<0)
#define DEBUG_PORT_RECORD_TRIGGER_TYPE_MASK (15<<0)
#define DEBUG_PORT_RECORD_TRIGGER_TYPE_SHIFT (0)
#define DEBUG_PORT_RECORD_TRIGGER_TYPE_RECORD_JUMANDBRANCH (0<<0)
#define DEBUG_PORT_RECORD_TRIGGER_TYPE_RECORD_BRANCH (1<<0)
#define DEBUG_PORT_RECORD_TRIGGER_TYPE_RECORD_BRANCHNOTTAKEN (2<<0)
#define DEBUG_PORT_RECORD_TRIGGER_TYPE_EXL_RECORD_BRANCHTAKEN (3<<0)
#define DEBUG_PORT_RECORD_TRIGGER_TYPE_EXL_RECORD_NOJUMNOBRANCH (4<<0)
#define DEBUG_PORT_ANY_BRANCH       (1<<4)
#define DEBUG_PORT_INT_EN           (1<<5)
#define DEBUG_PORT_CPU_SELECT_BCPU  (0<<6)
#define DEBUG_PORT_CPU_SELECT_XCPU  (1<<6)
#define DEBUG_PORT_BRANCH_TYPE(n)   (((n)&31)<<8)
#define DEBUG_PORT_BRANCH_TYPE_MASK (31<<8)
#define DEBUG_PORT_BRANCH_TYPE_SHIFT (8)
#define DEBUG_PORT_BRANCH_TYPE_BLTZ (0<<8)
#define DEBUG_PORT_BRANCH_TYPE_BGEZ (1<<8)
#define DEBUG_PORT_BRANCH_TYPE_BEQ  (2<<8)
#define DEBUG_PORT_BRANCH_TYPE_BNE  (3<<8)
#define DEBUG_PORT_BRANCH_TYPE_BLEZ (4<<8)
#define DEBUG_PORT_BRANCH_TYPE_BGTZ (5<<8)
#define DEBUG_PORT_BRANCH_TYPE_BC1F (6<<8)
#define DEBUG_PORT_BRANCH_TYPE_BC1T (7<<8)
#define DEBUG_PORT_BRANCH_TYPE_BC2F (8<<8)
#define DEBUG_PORT_BRANCH_TYPE_BC2T (9<<8)
#define DEBUG_PORT_BRANCH_TYPE_NO_BRANCH (10<<8)
#define DEBUG_PORT_BRANCH_TYPE_BR   (11<<8)
#define DEBUG_PORT_BRANCH_TYPE_BEQZ (12<<8)
#define DEBUG_PORT_BRANCH_TYPE_BNEZ (13<<8)
#define DEBUG_PORT_ENABLE_PXTS_TAG_0 (1<<16)
#define DEBUG_PORT_ENABLE_PXTS_TAG_1 (1<<17)
#define DEBUG_PORT_ENABLE_PXTS_TAG_2 (1<<18)
#define DEBUG_PORT_ENABLE_PXTS_TAG_3 (1<<19)
#define DEBUG_PORT_ENABLE_PXTS_TAG_4 (1<<20)
#define DEBUG_PORT_ENABLE_PXTS_TAG_5 (1<<21)
#define DEBUG_PORT_ENABLE_PXTS_TAG_6 (1<<22)
#define DEBUG_PORT_ENABLE_PXTS_TAG_7 (1<<23)
#define DEBUG_PORT_ENABLE_PXTS_TAG_8 (1<<24)
#define DEBUG_PORT_ENABLE_PXTS_TAG_9 (1<<25)
#define DEBUG_PORT_ENABLE_PXTS_TAG_10 (1<<26)
#define DEBUG_PORT_ENABLE_PXTS_TAG_11 (1<<27)
#define DEBUG_PORT_ENABLE_PXTS_TAG_12 (1<<28)
#define DEBUG_PORT_ENABLE_PXTS_TAG_13 (1<<29)
#define DEBUG_PORT_ENABLE_PXTS_TAG_14 (1<<30)
#define DEBUG_PORT_ENABLE_PXTS_TAG_15 (1<<31)
#define DEBUG_PORT_ENABLE_PXTS_TAG(n) (((n)&0xFFFF)<<16)
#define DEBUG_PORT_ENABLE_PXTS_TAG_MASK (0xFFFF<<16)
#define DEBUG_PORT_ENABLE_PXTS_TAG_SHIFT (16)

//WD_Addr_cfg
#define DEBUG_PORT_WD_ADDRESS(n)    (((n)&0x1FFFFFF)<<0)

//Signal_Spy_Cfg
#define DEBUG_PORT_DBG_BUS_RXTX_SEL0_RX_ON (1<<0)
#define DEBUG_PORT_DBG_BUS_RXTX_SEL0_GMSK_ON (0<<0)
#define DEBUG_PORT_DBG_BUS_RXTX_SEL1_TX_ON (1<<4)
#define DEBUG_PORT_DBG_BUS_RXTX_SEL1_RX_DEC_ON (0<<4)
#define DEBUG_PORT_DBG_BUS_MODE(n)  (((n)&3)<<8)
#define DEBUG_PORT_DBG_BUS_MODE_MASK (3<<8)
#define DEBUG_PORT_DBG_BUS_MODE_SHIFT (8)
#define DEBUG_PORT_DBG_BUS_MODE_DEBUG_MODE_SYS (0<<8)
#define DEBUG_PORT_DBG_BUS_MODE_DEBUG_MODE_BB (1<<8)
#define DEBUG_PORT_DBG_BUS_MODE_DEBUG_MODE_PA (2<<8)
#define DEBUG_PORT_DBG_BUS_MODE_DEBUG_MODE_OTHERS (3<<8)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0(n) (((n)&0x7F)<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_MASK (0x7F<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_SHIFT (16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_TCU0 (0x0<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_TCU1 (0x1<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_FRAME (0x2<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_DMA_TX_SDMMC (0x3<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_DMA_RX_SDMMC (0x4<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_DMA_TX_LCDC (0x5<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_DMA_RX_CAMERA (0x6<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_COM0 (0x7<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_COM1 (0x8<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_VOC (0x9<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_DMA (0xA<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_GPIO (0xB<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_KEYPAD (0xC<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_TIMERS (0xD<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_OS_TIMER (0xE<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_CALENDAR (0xF<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_SPI (0x10<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_SPI2 (0x11<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_DEBUG_UART (0x12<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_UART (0x13<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_UART2 (0x14<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_I2C (0x15<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_SCI (0x16<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_RF_SPI (0x17<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_LPS (0x18<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_BBIFC0 (0x19<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_BBIFC1 (0x1A<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_USBC (0x1B<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_LCDC (0x1C<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_SDMMC (0x1D<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_CAMERA (0x1E<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_EXTAPB (0x1F<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_HREADY (0x20<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_XCPU_I_FETCH (0x21<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_XCPU_I_MISS (0x22<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_XCPU_D_FETCH (0x23<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_XCPU_D_MISS (0x24<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_XCPU_MAIN_IRQ (0x25<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_TCU0 (0x40<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_TCU1 (0x41<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_FRAME (0x42<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_COM0 (0x43<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_COM1 (0x44<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_VOC (0x45<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_IFC0 (0x46<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_IFC1 (0x47<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_IFC2 (0x48<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_RF_IF (0x49<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_ITLV (0x4A<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_VITAC (0x4B<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_IRQ_XCOR (0x4C<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_IRQ_PAGE_SPY (0x53<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_EXT_PLL_LOCKED (0x54<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_ACCO_PLL_LOCKED (0x55<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_WATCH0 (0x56<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_WATCH1 (0x57<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_WATCH2 (0x58<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_WATCH3 (0x59<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_DIGRF_STROBE (0x5A<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_RX_SOC (0x5B<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_VOC_DMA_BUSY (0x5C<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_VOC_BUSY (0x5D<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_SYS_EBC_BUSY (0x5E<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_EBC_BUSY (0x5F<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BB_HREADY (0x60<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BCPU_I_FETCH (0x61<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BCPU_I_MISS (0x62<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BCPU_D_FETCH (0x63<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BCPU_D_MISS (0x64<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL0_DBG_BCPU_MAIN_IRQ (0x65<<16)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1(n) (((n)&0x7F)<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_MASK (0x7F<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_SHIFT (24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_TCU0 (0x0<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_TCU1 (0x1<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_FRAME (0x2<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_DMA_TX_SDMMC (0x3<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_DMA_RX_SDMMC (0x4<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_DMA_TX_LCDC (0x5<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_DMA_RX_CAMERA (0x6<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_COM0 (0x7<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_COM1 (0x8<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_VOC (0x9<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_DMA (0xA<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_GPIO (0xB<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_KEYPAD (0xC<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_TIMERS (0xD<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_OS_TIMER (0xE<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_CALENDAR (0xF<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_SPI (0x10<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_SPI2 (0x11<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_DEBUG_UART (0x12<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_UART (0x13<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_UART2 (0x14<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_I2C (0x15<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_SCI (0x16<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_RF_SPI (0x17<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_LPS (0x18<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_BBIFC0 (0x19<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_BBIFC1 (0x1A<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_USBC (0x1B<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_LCDC (0x1C<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_SDMMC (0x1D<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_CAMERA (0x1E<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_EXTAPB (0x1F<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_HREADY (0x20<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_XCPU_I_FETCH (0x21<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_XCPU_I_MISS (0x22<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_XCPU_D_FETCH (0x23<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_XCPU_D_MISS (0x24<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_XCPU_MAIN_IRQ (0x25<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_TCU0 (0x40<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_TCU1 (0x41<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_FRAME (0x42<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_COM0 (0x43<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_COM1 (0x44<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_VOC (0x45<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_IFC0 (0x46<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_IFC1 (0x47<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_IFC2 (0x48<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_RF_IF (0x49<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_ITLV (0x4A<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_VITAC (0x4B<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_IRQ_XCOR (0x4C<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_IRQ_PAGE_SPY (0x53<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_EXT_PLL_LOCKED (0x54<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_ACCO_PLL_LOCKED (0x55<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_WATCH0 (0x56<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_WATCH1 (0x57<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_WATCH2 (0x58<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_WATCH3 (0x59<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_DIGRF_STROBE (0x5A<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_RX_SOC (0x5B<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_VOC_DMA_BUSY (0x5C<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_VOC_BUSY (0x5D<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_SYS_EBC_BUSY (0x5E<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_EBC_BUSY (0x5F<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BB_HREADY (0x60<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BCPU_I_FETCH (0x61<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BCPU_I_MISS (0x62<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BCPU_D_FETCH (0x63<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BCPU_D_MISS (0x64<<24)
#define DEBUG_PORT_DBG_BUS_SIG_SEL1_DBG_BCPU_MAIN_IRQ (0x65<<24)
#define DEBUG_PORT_DBG_BUS_RXTX_SEL(n) (((n)&17)<<0)
#define DEBUG_PORT_DBG_BUS_RXTX_SEL_MASK (17<<0)
#define DEBUG_PORT_DBG_BUS_RXTX_SEL_SHIFT (0)

//Dbg_Clk_Cfg
#define DEBUG_PORT_SEL_SOURCE(n)    (((n)&0x3F)<<0)
#define DEBUG_PORT_SEL_SOURCE_MASK  (0x3F<<0)
#define DEBUG_PORT_SEL_SOURCE_SHIFT (0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_SYS_NOGATE (0x0<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_SYSD_SCI (0x1<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_SYSD_SPI (0x2<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_SYSD_SPI2 (0x3<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_SYSD_RF_SPI (0x4<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_SYSD_OSC (0x5<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_SYSD_PWM (0x6<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_BBD_DAIS_208K (0x7<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_HOST (0x8<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_AUDIO_PLL (0x9<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_AUDIO_BCK (0xA<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_UART (0xB<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_UART2 (0xC<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_RF_RX (0xD<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_RF_TX (0xE<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_MEM_BRIDGE (0xF<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_VOC (0x10<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_32K (0x11<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_USBPHY (0x12<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_CAMERA (0x13<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_OSC (0x14<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_REF (0x15<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_SLOW (0x16<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_FAST_PLL (0x17<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_FAST_ACCO_PLL (0x18<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_FAST_EXT_PLL (0x19<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_VOC_PLL (0x1A<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_UART_PLL (0x1B<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_AU_CLK_520K (0x1C<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_AU_CLK_40K (0x1D<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_PADAC_CLK (0x1E<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_AFCDAC_CLK (0x1F<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_GPADC_CLK (0x20<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_TX_CLK_FS (0x21<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_RX_CLK_FS (0x22<<0)
#define DEBUG_PORT_SEL_SOURCE_DBG_SEL_EXT_DBG (0x23<<0)
#define DEBUG_PORT_ENABLE_DBG_MASKED (0<<20)
#define DEBUG_PORT_ENABLE_DBG_ENABLED (1<<20)

//Dbg_Watch_Set
#define DEBUG_PORT_DBG_WATCH_S_C(n) (((n)&15)<<0)

//Dbg_Watch_Clr
//#define DEBUG_PORT_DBG_WATCH_S_C(n) (((n)&15)<<0)

//Pxts_tag[0]
#define DEBUG_PORT_TAG_PXTS(n)      (((n)&0xFFFF)<<0)





#endif
