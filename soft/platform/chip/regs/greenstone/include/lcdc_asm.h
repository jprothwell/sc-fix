#ifndef _LCDC_ASM_H_
#define _LCDC_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'lcdc'."
#endif



//==============================================================================
// lcdc
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_LCDC_BASE               0x00B98000

#define REG_LCDC_BASE_HI            BASE_HI(REG_LCDC_BASE)
#define REG_LCDC_BASE_LO            BASE_LO(REG_LCDC_BASE)

#define REG_LCDC_APBI_CTRL_LCD      REG_LCDC_BASE_LO + 0x00000000
#define REG_LCDC_APBI_FIFO          REG_LCDC_BASE_LO + 0x00000008
#define REG_LCDC_LCD_CTRL           REG_LCDC_BASE_LO + 0x00000400
#define REG_LCDC_LCD_STATUS         REG_LCDC_BASE_LO + 0x00000404
#define REG_LCDC_LCD_DATA           REG_LCDC_BASE_LO + 0x00000408
#define REG_LCDC_LCD_CLEAR          REG_LCDC_BASE_LO + 0x0000040C
#define REG_LCDC_LCD_CFG            REG_LCDC_BASE_LO + 0x00000410
#define REG_LCDC_LCD_TIMING         REG_LCDC_BASE_LO + 0x00000414
#define REG_LCDC_SLCD_CTRL          REG_LCDC_BASE_LO + 0x00000800
#define REG_LCDC_SLCD_STATUS        REG_LCDC_BASE_LO + 0x00000804
#define REG_LCDC_SLCD_CFG           REG_LCDC_BASE_LO + 0x00000808
#define REG_LCDC_SLCD_INT           REG_LCDC_BASE_LO + 0x0000080C
#define REG_LCDC_SLCD_RW            REG_LCDC_BASE_LO + 0x00000810
#define REG_LCDC_SLCD_CDOUT         REG_LCDC_BASE_LO + 0x00000814
#define REG_LCDC_SLCD_DIN           REG_LCDC_BASE_LO + 0x00000818
#define REG_LCDC_SLCD_DIVCLK        REG_LCDC_BASE_LO + 0x0000081C

//apbi_ctrl_lcd
#define LCDC_L_ENDIAN(n)            (((n)&3)<<0)
#define LCDC_SOFT_RST_L             (1<<2)

//APBI_FIFO
#define LCDC_DATA_IN(n)             (((n)&0xFFFFFFFF)<<0)

//LCD_CTRL
#define LCDC_LCD_ENABLE             (1<<0)
#define LCDC_LCD_CTRL_RW            (1<<4)
#define LCDC_LCD_CTRL_RS            (1<<5)
#define LCDC_LCD_CTRL_AF            (1<<8)
#define LCDC_LCD_CTRL_CNT(n)        (((n)&0xFFFFF)<<12)

//LCD_STATUS
#define LCDC_LCD_RDY                (1<<0)
#define LCDC_LCD_INT_STATUS         (1<<16)
#define LCDC_LCD_INT_CAUSE          (1<<20)

//LCD_DATA
#define LCDC_LCD_DATA(n)            (((n)&0xFFFFFFFF)<<0)

//LCD_CLEAR
#define LCDC_LCD_INT_CLR            (1<<16)

//LCD_CFG
#define LCDC_LCD_CFG_DBS(n)         (((n)&7)<<0)
#define LCDC_LCD_CFG_CS(n)          (((n)&7)<<4)
#define LCDC_LCD_CFG_PIS            (1<<8)
#define LCDC_LCD_CS_POL(n)          (((n)&7)<<12)
#define LCDC_LCD_INT_MASK           (1<<16)

//LCD_TIMING
#define LCDC_LCD_TIMING_TAS(n)      (((n)&3)<<0)
#define LCDC_LCD_TIMING_TCH(n)      (((n)&0x3F)<<8)
#define LCDC_LCD_TIMING_TCL(n)      (((n)&0x3F)<<16)

//SLCD_CTRL
#define LCDC_ENABLE                 (1<<0)
#define LCDC_SCL_ENABLE             (1<<4)
#define LCDC_APB_FF                 (1<<8)
#define LCDC_CD                     (1<<12)
#define LCDC_DMA_CNT(n)             (((n)&0xFFFF)<<16)

//SLCD_STATUS
#define LCDC_READY                  (1<<0)
#define LCDC_SLCD_INT_STATUS        (1<<8)
#define LCDC_SLCD_INT_CAUSE         (1<<12)

//SLCD_CFG
#define LCDC_LINE                   (1<<0)
#define LCDC_CE                     (1<<4)
#define LCDC_SLCD_INT_MASK          (1<<8)

//SLCD_INT
#define LCDC_INT_CLEAR              (1<<8)

//SLCD_RW
#define LCDC_RW                     (1<<0)
#define LCDC_READ_BYTE(n)           (((n)&7)<<4)

//SLCD_CDOUT
#define LCDC_CMD_DATA(n)            (((n)&0xFFFFFFFF)<<0)

//SLCD_DIN
#define LCDC_DIN(n)                 (((n)&0xFFFFFFFF)<<0)

//SLCD_DIVCLK
#define LCDC_DIV_CLK(n)             (((n)&0xFFFF)<<0)




#endif
