#ifndef _COMREGS_ASM_H_
#define _COMREGS_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'comregs'."
#endif

#include "globals_asm.h"


//==============================================================================
// comregs
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_BB_COMREGS_BASE         0x0190F000

#define REG_BB_COMREGS_BASE_HI      BASE_HI(REG_BB_COMREGS_BASE)
#define REG_BB_COMREGS_BASE_LO      BASE_LO(REG_BB_COMREGS_BASE)

#define REG_BB_COMREGS_CAUSE        REG_BB_COMREGS_BASE_LO + 0x00000000
#define REG_BB_COMREGS_MASK_SET     REG_BB_COMREGS_BASE_LO + 0x00000004
#define REG_BB_COMREGS_MASK_CLR     REG_BB_COMREGS_BASE_LO + 0x00000008
#define REG_BB_COMREGS_ITREG_SET    REG_BB_COMREGS_BASE_LO + 0x0000000C
#define REG_BB_COMREGS_ITREG_CLR    REG_BB_COMREGS_BASE_LO + 0x00000010

//Cause
#define COMREGS_X_IRQ0_CAUSE(n)     (((n)&15)<<0)
#define COMREGS_X_IRQ0_CAUSE_MASK   (15<<0)
#define COMREGS_X_IRQ0_CAUSE_SHIFT  (0)
#define COMREGS_X_IRQ1_CAUSE(n)     (((n)&15)<<4)
#define COMREGS_X_IRQ1_CAUSE_MASK   (15<<4)
#define COMREGS_X_IRQ1_CAUSE_SHIFT  (4)
#define COMREGS_B_IRQ0_CAUSE(n)     (((n)&15)<<8)
#define COMREGS_B_IRQ0_CAUSE_MASK   (15<<8)
#define COMREGS_B_IRQ0_CAUSE_SHIFT  (8)
#define COMREGS_B_IRQ1_CAUSE(n)     (((n)&15)<<12)
#define COMREGS_B_IRQ1_CAUSE_MASK   (15<<12)
#define COMREGS_B_IRQ1_CAUSE_SHIFT  (12)

//Mask_Set
#define COMREGS_X_IRQ0_MASK_SET(n)  (((n)&15)<<0)
#define COMREGS_X_IRQ0_MASK_SET_MASK (15<<0)
#define COMREGS_X_IRQ0_MASK_SET_SHIFT (0)
#define COMREGS_X_IRQ1_MASK_SET(n)  (((n)&15)<<4)
#define COMREGS_X_IRQ1_MASK_SET_MASK (15<<4)
#define COMREGS_X_IRQ1_MASK_SET_SHIFT (4)
#define COMREGS_B_IRQ0_MASK_SET(n)  (((n)&15)<<8)
#define COMREGS_B_IRQ1_MASK_SET(n)  (((n)&15)<<12)

//Mask_Clr
#define COMREGS_X_IRQ0_MASK_CLR(n)  (((n)&15)<<0)
#define COMREGS_X_IRQ0_MASK_CLR_MASK (15<<0)
#define COMREGS_X_IRQ0_MASK_CLR_SHIFT (0)
#define COMREGS_X_IRQ1_MASK_CLR(n)  (((n)&15)<<4)
#define COMREGS_X_IRQ1_MASK_CLR_MASK (15<<4)
#define COMREGS_X_IRQ1_MASK_CLR_SHIFT (4)
#define COMREGS_B_IRQ0_MASK_CLR(n)  (((n)&15)<<8)
#define COMREGS_B_IRQ1_MASK_CLR(n)  (((n)&15)<<12)

//ItReg_Set
#define COMREGS_X_IRQ0_SET(n)       (((n)&15)<<0)
#define COMREGS_X_IRQ0_SET_MASK     (15<<0)
#define COMREGS_X_IRQ0_SET_SHIFT    (0)
#define COMREGS_X_IRQ1_SET(n)       (((n)&15)<<4)
#define COMREGS_X_IRQ1_SET_MASK     (15<<4)
#define COMREGS_X_IRQ1_SET_SHIFT    (4)
#define COMREGS_B_IRQ0_SET(n)       (((n)&15)<<8)
#define COMREGS_B_IRQ0_SET_MASK     (15<<8)
#define COMREGS_B_IRQ0_SET_SHIFT    (8)
#define COMREGS_B_IRQ1_SET(n)       (((n)&15)<<12)
#define COMREGS_B_IRQ1_SET_MASK     (15<<12)
#define COMREGS_B_IRQ1_SET_SHIFT    (12)
#define COMREGS_X_IRQ(n)            (((n)&0xFF)<<0)
#define COMREGS_X_IRQ_MASK          (0xFF<<0)
#define COMREGS_X_IRQ_SHIFT         (0)
#define COMREGS_B_IRQ(n)            (((n)&0xFF)<<8)
#define COMREGS_B_IRQ_MASK          (0xFF<<8)
#define COMREGS_B_IRQ_SHIFT         (8)

//ItReg_Clr
#define COMREGS_X_IRQ0_CLR(n)       (((n)&15)<<0)
#define COMREGS_X_IRQ1_CLR(n)       (((n)&15)<<4)
#define COMREGS_B_IRQ0_CLR(n)       (((n)&15)<<8)
#define COMREGS_B_IRQ1_CLR(n)       (((n)&15)<<12)




#endif
