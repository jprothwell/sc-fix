#ifndef _SYS_CTRL_ASM_H_
#define _SYS_CTRL_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'sys_ctrl'."
#endif

#include "globals_asm.h"
#include "ip_version_asm.h"

/// CPU IDs
#define XCPU                                     (0)
#define BCPU                                     (1)
/// System side System clocks
#define SYS_XCPU                                 (0)
#define SYS_XCPU_INT                             (1)
#define SYS_AMBA                                 (2)
#define SYS_PCLK_CONF                            (3)
#define SYS_PCLK_DATA                            (4)
#define SYS_DMA                                  (5)
#define SYS_EBC                                  (6)
#define SYS_IFC_CH0                              (7)
#define SYS_IFC_CH1                              (8)
#define SYS_IFC_CH2                              (9)
#define SYS_IFC_CH3                              (10)
#define SYS_IFC_CH4                              (11)
#define SYS_IFC_CH5                              (12)
#define SYS_IFC_CH6                              (13)
#define SYS_IFC_CH7                              (14)
#define SYS_IFC_DBG                              (15)
#define SYS_A2A                                  (16)
#define SYS_COM_REGS                             (17)
#define SYS_DEBUG_UART                           (18)
#define SYS_DBGHST                               (19)
#define SYS_GPADC                                (20)
/// System side divided clock (either divided by module or by sys_ctrl)
#define SYSD_SCI                                 (21)
#define SYSD_RF_SPI                              (22)
#define SYSD_OSC                                 (23)
#define SYSD_PWM                                 (24)
#define NB_SYS_CLK_AEN                           (25)
/// the following don't have an auto enable
#define SYS_GPIO                                 (25)
#define SYS_IRQ                                  (26)
#define SYS_TCU                                  (27)
#define SYS_TIMER                                (28)
#define NB_SYS_CLK_EN                            (29)
/// the following are sharing their enable
#define SYS_SCI                                  (29)
/// keep last
#define SYS_NOGATE                               (30)
#define NB_SYS_CLK                               (31)
/// System side System clocks for Peripherals
#define PER_UART                                 (0)
#define PER_UART2                                (1)
#define PER_I2C                                  (2)
#define PER_LCDC                                 (3)
#define PER_SDMMC                                (4)
/// System side divided clock (either divided by module or by sys_ctrl)
#define PERD_SPI                                 (5)
#define PERD_SPI2                                (6)
#define NB_PER_CLK_AEN                           (7)
#define PER_USBC                                 (7)
#define PER_CAMERA                               (8)
#define PER_SPY                                  (9)
#define PER_TEST                                 (10)
#define NB_PER_CLK_EN                            (11)
#define NB_PER_CLK                               (11)
/// Baseband side System clocks
#define BB_BCPU                                  (0)
#define BB_BCPU_INT                              (1)
#define BB_AMBA                                  (2)
#define BB_PCLK_CONF                             (3)
#define BB_PCLK_DATA                             (4)
#define BB_VOC_AHB                               (5)
#define BB_IFC_CH0                               (6)
#define BB_IFC_CH1                               (7)
#define BB_IFC_CH2                               (8)
#define BB_SRAM                                  (9)
#define BB_A2A                                   (10)
#define BB_ITLV                                  (11)
#define BB_VITERBI                               (12)
#define BB_CIPHER                                (13)
#define BB_RF_IF                                 (14)
#define BB_COPRO                                 (15)
#define BB_CP2_REG                               (16)
#define BB_XCOR                                  (17)
#define BB_AIF                                   (18)
/// Baseband side divided clock
/// divided BCK clock for DAI
#define BBD_DAIS_208K                            (19)
#define NB_BB_CLK_AEN                            (20)
/// the following don't have an auto enable
#define BB_IRQ                                   (20)
#define NB_BB_CLK_EN                             (21)
#define NB_BB_CLK                                (21)
/// Other clocks
/// clocks with auto enble
/// the debug host clock auto enable is not used in host mode, only in uart mode
#define OC_HOST_UART                             (0)
#define OC_DEBUG_UART                            (0)
#define OC_BCK                                   (1)
#define OC_UART                                  (2)
#define OC_UART2                                 (3)
#define OC_RF_RX                                 (4)
#define OC_RF_TX                                 (5)
#define OC_MEM_BRIDGE                            (6)
#define NB_OTHER_CLK_VOC_AEN_SYNC                (7)
#define OC_VOC_CORE                              (7)
#define OC_VOC_AHB                               (8)
#define OC_VOC_DAI                               (9)
#define NB_OTHER_CLK_AEN                         (10)
/// the following don't have an auto enable
#define OC_VOC                                   (10)
#define OC_LPS                                   (11)
#define OC_GPIO                                  (12)
#define OC_USBPHY                                (13)
#define OC_CAMERA                                (14)
#define OC_CLK_OUT                               (15)
#define OC_MEM_CLK_OUT                           (16)
#define NB_OTHER_CLK_EN                          (17)
#define NB_OTHER_CLK                             (17)
/// System side resets
#define RST_XCPU                                 (0)
#define RST_SYS_IRQ                              (1)
#define RST_SYS_A2A                              (2)
#define RST_COM_REGS                             (3)
#define RST_DMA                                  (4)
#define RST_TIMER                                (5)
#define RST_TCU                                  (6)
#define RST_GPIO                                 (7)
#define RST_KEYPAD                               (8)
#define RST_PWM                                  (9)
#define RST_UART                                 (10)
#define RST_UART2                                (11)
#define RST_SPI                                  (12)
#define RST_SPI2                                 (13)
#define RST_RF_SPI                               (14)
#define RST_SCI                                  (15)
#define RST_SPY                                  (16)
#define RST_TEST                                 (17)
#define RST_I2C                                  (18)
#define RST_MEM_BRIDGE                           (19)
#define RST_PA_AFC                               (20)
#define RST_GPADC                                (21)
#define RST_ANA_ACCO                             (22)
#define RST_EXT_APB                              (23)
#define RST_LCDC                                 (24)
#define RST_SDMMC                                (25)
#define RST_CAMERA                               (26)
#define RST_USBC                                 (27)
#define RST_USBPHY                               (28)
#define NB_SRST                                  (29)
/// Baseband side resets
#define BOUND_BRST_FIRST                         (29)
#define RST_BCPU                                 (29)
#define RST_BB_IRQ                               (30)
#define RST_BB_A2A                               (31)
#define RST_BB_IFC                               (32)
#define RST_BB_SRAM                              (33)
#define RST_ITLV                                 (34)
#define RST_VITERBI                              (35)
#define RST_CIPHER                               (36)
#define RST_XCOR                                 (37)
#define RST_COPRO                                (38)
#define RST_RF_IF                                (39)
#define RST_VOC                                  (40)
#define RST_AIF                                  (41)
#define RST_DAI                                  (42)
#define BOUND_BRST_AFTER                         (43)
#define RST_BB_FULL                              (43)
/// The following reset does not have register
#define NR_RST_REG                               (44)
#define RST_SYS_FULL                             (44)
#define NB_RST                                   (45)
#define NB_BRST                                  (BOUND_BRST_AFTER-BOUND_BRST_FIRST)
/// Reset Other : resync on corresponding clock other
#define RSTO_DBG_HOST                            (0)
#define RSTO_BCK                                 (1)
#define RSTO_UART                                (2)
#define RSTO_UART2                               (3)
#define BOUND_RSTO_RF_FIRST                      (4)
#define RSTO_RF_RX                               (4)
#define RSTO_RF_TX                               (5)
#define BOUND_RSTO_RF_AFTER                      (6)
#define RSTO_MEM_BRIDGE                          (6)
#define RSTO_VOC_DAI                             (7)
#define RSTO_VOC                                 (8)
#define RSTO_LPS                                 (9)
#define RSTO_GPIO                                (10)
#define RSTO_WDTIMER                             (11)
#define RSTO_USBPHY                              (12)
#define NB_RSTO                                  (13)
/// For REG_DBG protect lock/unlock value
#define SYS_CTRL_PROTECT_LOCK                    (0XA50000)
#define SYS_CTRL_PROTECT_UNLOCK                  (0XA50001)

//==============================================================================
// Cpu_Id
//------------------------------------------------------------------------------
/// 
//==============================================================================



//==============================================================================
// Sys_Clks
//------------------------------------------------------------------------------
/// 
//==============================================================================



//==============================================================================
// Per_Clks
//------------------------------------------------------------------------------
/// 
//==============================================================================



//==============================================================================
// BB_Clks
//------------------------------------------------------------------------------
/// 
//==============================================================================



//==============================================================================
// Other_Clks
//------------------------------------------------------------------------------
/// 
//==============================================================================



//==============================================================================
// Resets
//------------------------------------------------------------------------------
/// 
//==============================================================================



//==============================================================================
// Reset_Others
//------------------------------------------------------------------------------
/// 
//==============================================================================



//==============================================================================
// sys_ctrl
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_SYS_CTRL_BASE           0x00B80000

#define REG_SYS_CTRL_BASE_HI        BASE_HI(REG_SYS_CTRL_BASE)
#define REG_SYS_CTRL_BASE_LO        BASE_LO(REG_SYS_CTRL_BASE)

#define REG_SYS_CTRL_REG_DBG        REG_SYS_CTRL_BASE_LO + 0x00000000
#define REG_SYS_CTRLIP_VERSION_IP_VERSION REG_SYS_CTRL_BASE_LO + 0x00000004
#define REG_SYS_CTRL_SYS_RST_SET    REG_SYS_CTRL_BASE_LO + 0x00000008
#define REG_SYS_CTRL_SYS_RST_CLR    REG_SYS_CTRL_BASE_LO + 0x0000000C
#define REG_SYS_CTRL_BB_RST_SET     REG_SYS_CTRL_BASE_LO + 0x00000010
#define REG_SYS_CTRL_BB_RST_CLR     REG_SYS_CTRL_BASE_LO + 0x00000014
#define REG_SYS_CTRL_CLK_SYS_MODE   REG_SYS_CTRL_BASE_LO + 0x00000018
#define REG_SYS_CTRL_CLK_SYS_ENABLE REG_SYS_CTRL_BASE_LO + 0x0000001C
#define REG_SYS_CTRL_CLK_SYS_DISABLE REG_SYS_CTRL_BASE_LO + 0x00000020
#define REG_SYS_CTRL_CLK_PER_MODE   REG_SYS_CTRL_BASE_LO + 0x00000024
#define REG_SYS_CTRL_CLK_PER_ENABLE REG_SYS_CTRL_BASE_LO + 0x00000028
#define REG_SYS_CTRL_CLK_PER_DISABLE REG_SYS_CTRL_BASE_LO + 0x0000002C
#define REG_SYS_CTRL_CLK_BB_MODE    REG_SYS_CTRL_BASE_LO + 0x00000030
#define REG_SYS_CTRL_CLK_BB_ENABLE  REG_SYS_CTRL_BASE_LO + 0x00000034
#define REG_SYS_CTRL_CLK_BB_DISABLE REG_SYS_CTRL_BASE_LO + 0x00000038
#define REG_SYS_CTRL_CLK_OTHER_MODE REG_SYS_CTRL_BASE_LO + 0x0000003C
#define REG_SYS_CTRL_CLK_OTHER_ENABLE REG_SYS_CTRL_BASE_LO + 0x00000040
#define REG_SYS_CTRL_CLK_OTHER_DISABLE REG_SYS_CTRL_BASE_LO + 0x00000044
#define REG_SYS_CTRL_SEL_CLOCK      REG_SYS_CTRL_BASE_LO + 0x00000048
#define REG_SYS_CTRL_CFG_CLK_SYS    REG_SYS_CTRL_BASE_LO + 0x0000004C
#define REG_SYS_CTRL_CFG_CLK_MEM_BRIDGE REG_SYS_CTRL_BASE_LO + 0x00000050
#define REG_SYS_CTRL_CFG_CLK_VOC    REG_SYS_CTRL_BASE_LO + 0x00000054
#define REG_SYS_CTRL_CFG_CLK_AUDIOBCK_DIV REG_SYS_CTRL_BASE_LO + 0x00000058
#define REG_SYS_CTRL_CFG_CLK_UART_0 REG_SYS_CTRL_BASE_LO + 0x0000005C
#define REG_SYS_CTRL_CFG_CLK_UART_1 REG_SYS_CTRL_BASE_LO + 0x00000060
#define REG_SYS_CTRL_CFG_CLK_UART_2 REG_SYS_CTRL_BASE_LO + 0x00000064
#define REG_SYS_CTRL_CFG_CLK_PWM    REG_SYS_CTRL_BASE_LO + 0x00000068
#define REG_SYS_CTRL_CFG_AHB        REG_SYS_CTRL_BASE_LO + 0x0000006C
#define REG_SYS_CTRL_XCPU_DBG_BKP   REG_SYS_CTRL_BASE_LO + 0x00000070
#define REG_SYS_CTRL_XCPU_DBG_ADDR  REG_SYS_CTRL_BASE_LO + 0x00000074
#define REG_SYS_CTRL_BCPU_DBG_BKP   REG_SYS_CTRL_BASE_LO + 0x00000078
#define REG_SYS_CTRL_BCPU_DBG_ADDR  REG_SYS_CTRL_BASE_LO + 0x0000007C
#define REG_SYS_CTRL_CFG_CPUS_CACHE_RAM_DISABLE REG_SYS_CTRL_BASE_LO + 0x00000080
#define REG_SYS_CTRL_RESET_CAUSE    REG_SYS_CTRL_BASE_LO + 0x00000084
#define REG_SYS_CTRL_WAKEUP         REG_SYS_CTRL_BASE_LO + 0x00000088

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
