#ifndef _VITAC_ASM_H_
#define _VITAC_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'vitac'."
#endif

#include "globals_asm.h"


//==============================================================================
// vitac
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_VITAC_BASE              0x01906000

#define REG_VITAC_BASE_HI           BASE_HI(REG_VITAC_BASE)
#define REG_VITAC_BASE_LO           BASE_LO(REG_VITAC_BASE)

#define REG_VITAC_COMMAND           REG_VITAC_BASE_LO + 0x00000000
#define REG_VITAC_STATUS            REG_VITAC_BASE_LO + 0x00000004
#define REG_VITAC_CH_SYMB_ADDR      REG_VITAC_BASE_LO + 0x00000008
#define REG_VITAC_EXP_SYMB_ADDR     REG_VITAC_BASE_LO + 0x0000000C
#define REG_VITAC_PM_BASE_ADDR      REG_VITAC_BASE_LO + 0x00000010
#define REG_VITAC_OUT_BASE_ADDR     REG_VITAC_BASE_LO + 0x00000014
#define REG_VITAC_H0_PARAM          REG_VITAC_BASE_LO + 0x00000018
#define REG_VITAC_HL_PARAM          REG_VITAC_BASE_LO + 0x0000001C
#define REG_VITAC_RES_POLY_0        REG_VITAC_BASE_LO + 0x00000020
#define REG_VITAC_RES_POLY_1        REG_VITAC_BASE_LO + 0x00000024
#define REG_VITAC_RES_POLY_2        REG_VITAC_BASE_LO + 0x00000028
#define REG_VITAC_SHIST_0           REG_VITAC_BASE_LO + 0x0000002C
#define REG_VITAC_SHIST_1           REG_VITAC_BASE_LO + 0x00000030
#define REG_VITAC_SHIST_2           REG_VITAC_BASE_LO + 0x00000034
#define REG_VITAC_SHIST_3           REG_VITAC_BASE_LO + 0x00000038

//command
#define VITAC_START_GMSK_EQU        (1<<0)
#define VITAC_START_8PSK_EQU        (1<<1)
#define VITAC_START_DEC             (1<<2)
#define VITAC_START_TB              (1<<3)
#define VITAC_INT_MASK              (1<<4)
#define VITAC_PUNCTURED             (1<<5)
#define VITAC_NB_STATES(n)          (((n)&3)<<6)
#define VITAC_NB_STATES_MASK        (3<<6)
#define VITAC_NB_STATES_SHIFT       (6)
#define VITAC_NB_STATES_VITAC_16_STATES (0<<6)
#define VITAC_NB_STATES_VITAC_32_STATES (1<<6)
#define VITAC_NB_STATES_VITAC_64_STATES (2<<6)
#define VITAC_NB_STATES_VITAC_8PSK_STATES (3<<6)
#define VITAC_BKWD_TRELLIS          (1<<8)
#define VITAC_CODE_RATE(n)          (((n)&7)<<9)
#define VITAC_BM_SHIFT_LEV(n)       (((n)&15)<<12)
#define VITAC_SV_SHIFT_LEV(n)       (((n)&15)<<16)
#define VITAC_NB_SYMBOLS(n)         (((n)&0x3FF)<<20)

//status
#define VITAC_EQU_GMSK_PENDING      (1<<0)
#define VITAC_EQU_8PSK_PENDING      (1<<1)
#define VITAC_DEC_PENDING           (1<<2)
#define VITAC_TB_PENDING            (1<<3)
#define VITAC_IT_CAUSE              (1<<16)
#define VITAC_IT_STATUS             (1<<20)

//ch_symb_addr
#define VITAC_ADDR(n)               (((n)&0x1FFF)<<2)

//exp_symb_addr
//#define VITAC_ADDR(n)             (((n)&0x1FFF)<<2)

//pm_base_addr
//#define VITAC_ADDR(n)             (((n)&0x1FFF)<<2)

//out_base_addr
//#define VITAC_ADDR(n)             (((n)&0x1FFF)<<2)

//H0_param
#define VITAC_H0(n)                 (((n)&0xFFFFFFFF)<<0)

//HL_param
#define VITAC_HL_I_PART(n)          (((n)&0x1FFF)<<0)
#define VITAC_HL_Q_PART(n)          (((n)&0x1FFF)<<16)

//res_poly
#define VITAC_RES_POLY(n)           (((n)&0xFFFFFFFF)<<0)

//sHist
#define VITAC_STATE0_0(n)           (((n)&7)<<0)
#define VITAC_STATE0_1(n)           (((n)&7)<<4)
#define VITAC_STATE0_2(n)           (((n)&7)<<8)
#define VITAC_STATE0_3(n)           (((n)&7)<<12)
#define VITAC_STATE1_0(n)           (((n)&7)<<16)
#define VITAC_STATE1_1(n)           (((n)&7)<<20)
#define VITAC_STATE1_2(n)           (((n)&7)<<24)
#define VITAC_STATE1_3(n)           (((n)&7)<<28)




#endif
