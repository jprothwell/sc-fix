#ifndef _XCOR_ASM_H_
#define _XCOR_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'xcor'."
#endif

#include "globals_asm.h"

#define XCOR_SYMBOL_SIZE                         (VITAC_MULT_SIZE)
#define XCOR_SADDER_SIZE                         (XCOR_SYMBOL_SIZE+1 +6)

//==============================================================================
// xcor
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_XCOR_BASE               0x01901000

#define REG_XCOR_BASE_HI            BASE_HI(REG_XCOR_BASE)
#define REG_XCOR_BASE_LO            BASE_LO(REG_XCOR_BASE)

#define REG_XCOR_COMMAND            REG_XCOR_BASE_LO + 0x00000000
#define REG_XCOR_CONVOLUTION_0      REG_XCOR_BASE_LO + 0x00000004
#define REG_XCOR_CONVOLUTION_1      REG_XCOR_BASE_LO + 0x00000008
#define REG_XCOR_STATUS             REG_XCOR_BASE_LO + 0x0000000C
#define REG_XCOR_HV0                REG_XCOR_BASE_LO + 0x00000010
#define REG_XCOR_HV1                REG_XCOR_BASE_LO + 0x00000014
#define REG_XCOR_DATA_0             REG_XCOR_BASE_LO + 0x00000018
#define REG_XCOR_DATA_1             REG_XCOR_BASE_LO + 0x0000001C
#define REG_XCOR_DATA_2             REG_XCOR_BASE_LO + 0x00000020
#define REG_XCOR_DATA_3             REG_XCOR_BASE_LO + 0x00000024
#define REG_XCOR_DATA_4             REG_XCOR_BASE_LO + 0x00000028
#define REG_XCOR_DATA_5             REG_XCOR_BASE_LO + 0x0000002C
#define REG_XCOR_DATA_6             REG_XCOR_BASE_LO + 0x00000030
#define REG_XCOR_DATA_7             REG_XCOR_BASE_LO + 0x00000034
#define REG_XCOR_ACCUI              REG_XCOR_BASE_LO + 0x00000038
#define REG_XCOR_ACCUQ              REG_XCOR_BASE_LO + 0x0000003C
#define REG_XCOR_ADDR0              REG_XCOR_BASE_LO + 0x00000040

//command
#define XCOR_BITERR_EN              (1<<0)
#define XCOR_DCO1P_EN               (1<<1)
#define XCOR_DCO2P_EN               (1<<2)
#define XCOR_DCO3P_EN               (1<<3)
#define XCOR_TSCXC_EN               (1<<4)
#define XCOR_SREC_EN                (1<<5)
#define XCOR_BEXT_EN                (1<<6)
#define XCOR_SPROC_EN               (1<<7)
#define XCOR_CHEST_EN               (1<<8)
#define XCOR_FCHXC_EN               (1<<9)
#define XCOR_DEROT_EN               (1<<10)
#define XCOR_IQ_EN                  (1<<11)
#define XCOR_POWER_EN               (1<<12)
#define XCOR_BMML_EN                (1<<13)
#define XCOR_CCONV_EN               (1<<14)
#define XCOR_MMULT_EN               (1<<15)
#define XCOR_SLDWIN_EN              (1<<16)
#define XCOR_DEROTATION             (1<<17)
#define XCOR_MODE_8PSK              (1<<18)
#define XCOR_PACK_IQ                (1<<19)
#define XCOR_READ_SRAM              (1<<20)
#define XCOR_STORE_IN_BBSRAM        (1<<21)
#define XCOR_DEC_ADDR4              (1<<22)
#define XCOR_IT_MASK                (1<<23)
#define XCOR_CONV_SHIFT(n)          (((n)&15)<<24)

//convolution_0
#define XCOR_CONJUG_SEQ1            (1<<0)
#define XCOR_CONJUG_SEQ2            (1<<1)
#define XCOR_REVERSE                (1<<2)
#define XCOR_MAX_ITERATION(n)       (((n)&7)<<4)
#define XCOR_OFFSET_IDX_PTR(n)      (((n)&0xFF)<<8)
#define XCOR_NB_ILOOP(n)            (((n)&0xFF)<<16)
#define XCOR_NB_SYMB(n)             (((n)&0xFF)<<24)

//convolution_1
#define XCOR_MIN_LENGHT(n)          (((n)&0xFF)<<0)
#define XCOR_ALLENGHT(n)            (((n)&0x1FF)<<8)

//status
#define XCOR_OP_PENDING             (1<<0)
#define XCOR_IT_CAUSE               (1<<16)
#define XCOR_IT_STATUS              (1<<31)

//hv0
#define XCOR_HV0(n)                 (((n)&0xFFFFFFFF)<<0)

//hv1
#define XCOR_HV1(n)                 (((n)&0xFFFFFFFF)<<0)

//data
#define XCOR_DATA(n)                (((n)&0xFFFFFFFF)<<0)

//accuI
#define XCOR_ACCUI(n)               (((n)&0x1FFFFF)<<0)

//accuQ
#define XCOR_ACCUQ(n)               (((n)&0x1FFFFF)<<0)

//addr0
#define XCOR_ADDR0(n)               (((n)&0x1FFF)<<2)




#endif
