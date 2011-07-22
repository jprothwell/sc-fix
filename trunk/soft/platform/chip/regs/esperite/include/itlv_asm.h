#ifndef _ITLV_ASM_H_
#define _ITLV_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'itlv'."
#endif

#include "globals_asm.h"


//==============================================================================
// itlv
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_ITLV_BASE               0x01904000

#define REG_ITLV_BASE_HI            BASE_HI(REG_ITLV_BASE)
#define REG_ITLV_BASE_LO            BASE_LO(REG_ITLV_BASE)

#define REG_ITLV_COMMAND            REG_ITLV_BASE_LO + 0x00000000
#define REG_ITLV_STATUS             REG_ITLV_BASE_LO + 0x00000004
#define REG_ITLV_BURST_BASE         REG_ITLV_BASE_LO + 0x00000008
#define REG_ITLV_FRAME_BASE         REG_ITLV_BASE_LO + 0x0000000C
#define REG_ITLV_INT_STATUS         REG_ITLV_BASE_LO + 0x00000010

//command
#define ITLV_NB_BITS(n)             (((n)&0x7FF)<<20)
#define ITLV_BURST_OFFSET(n)        (((n)&31)<<8)
#define ITLV_ITLV_TYPE(n)           (((n)&15)<<4)
#define ITLV_ITLV_TYPE_MASK         (15<<4)
#define ITLV_ITLV_TYPE_SHIFT        (4)
#define ITLV_ITLV_TYPE_TYPE_1A      (0<<4)
#define ITLV_ITLV_TYPE_TYPE_1B_HD   (1<<4)
#define ITLV_ITLV_TYPE_TYPE_1C      (2<<4)
#define ITLV_ITLV_TYPE_TYPE_2A      (3<<4)
#define ITLV_ITLV_TYPE_TYPE_2B      (4<<4)
#define ITLV_ITLV_TYPE_TYPE_3       (5<<4)
#define ITLV_ITLV_TYPE_TYPE_H1      (6<<4)
#define ITLV_ITLV_TYPE_TYPE_H2      (7<<4)
#define ITLV_ITLV_TYPE_TYPE_H3      (8<<4)
#define ITLV_ITLV_TYPE_TYPE_H4      (8<<4)
#define ITLV_ITLV_TYPE_TYPE_D1      (10<<4)
#define ITLV_ITLV_TYPE_TYPE_D2      (11<<4)
#define ITLV_ITLV_TYPE_TYPE_D3      (12<<4)
#define ITLV_INT_MASK               (1<<3)
#define ITLV_SV_SIZE_SV_4BIT        (0<<2)
#define ITLV_SV_SIZE_SV_8BIT        (1<<2)
#define ITLV_DITLV_START            (1<<1)
#define ITLV_ITLV_START             (1<<0)

//status
#define ITLV_BUSY                   (1<<0)

//burst_base
#define ITLV_BURST_BASE(n)          (((n)&0x7FF)<<4)

//frame_base
#define ITLV_FRAME_BASE(n)          (((n)&0x1FFF)<<2)

//int_status
#define ITLV_IT_STATUS              (1<<16)
#define ITLV_IT_CAUSE               (1<<0)




#endif
