#ifndef _CAP_ASM_H_
#define _CAP_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'cap'."
#endif

#include "globals_asm.h"


//==============================================================================
// cap
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_CAP_BASE                0x01905000

#define REG_CAP_BASE_HI             BASE_HI(REG_CAP_BASE)
#define REG_CAP_BASE_LO             BASE_LO(REG_CAP_BASE)

#define REG_CAP_CTRL                REG_CAP_BASE_LO + 0x00000000
#define REG_CAP_OPERATION           REG_CAP_BASE_LO + 0x00000004
#define REG_CAP_STATUS              REG_CAP_BASE_LO + 0x00000008
#define REG_CAP_SRC_DATA_ADDR       REG_CAP_BASE_LO + 0x0000000C
#define REG_CAP_DST_DATA_ADDR       REG_CAP_BASE_LO + 0x00000010
#define REG_CAP_PUNCT_ADDR          REG_CAP_BASE_LO + 0x00000014
#define REG_CAP_CRC_CODE_LSB        REG_CAP_BASE_LO + 0x00000018
#define REG_CAP_CRC_CODE_MSB        REG_CAP_BASE_LO + 0x0000001C

//Ctrl
#define CAP_FIRST_POLY(n)           (((n)&15)<<0)
#define CAP_SECOND_POLY(n)          (((n)&15)<<4)
#define CAP_THIRD_POLY(n)           (((n)&7)<<8)
#define CAP_FOURTH_POLY(n)          (((n)&7)<<11)
#define CAP_FITH_POLY(n)            (((n)&7)<<14)
#define CAP_SIXTH_POLY(n)           (((n)&7)<<17)
#define CAP_RSC_POLY(n)             (((n)&7)<<20)
#define CAP_NB_POLY(n)              (((n)&7)<<23)
#define CAP_ENABLE_PUNCTURING       (1<<26)
#define CAP_ENABLE_CRC_PRELOADING   (1<<27)

//Operation
#define CAP_START_CRC_ENCODING      (1<<0)
#define CAP_START_CRC_DECODING      (1<<1)
#define CAP_START_CONV              (1<<2)
#define CAP_START_TRAPPING          (1<<3)
#define CAP_BIT_NUMBER(n)           (((n)&0x3FF)<<16)

//Status
#define CAP_ENABLE                  (1<<0)

//Src_Data_addr
#define CAP_SRC_DATA_ADDR(n)        (((n)&0x1FFF)<<2)

//Dst_Data_addr
#define CAP_DST_DATA_ADDR(n)        (((n)&0x1FFF)<<2)

//Punct_addr
#define CAP_PUNCT_ADDR(n)           (((n)&0x1FFF)<<2)

//CRC_code_LSB
#define CAP_CRC_CODE_LSB(n)         (((n)&0xFFFFFFFF)<<0)

//CRC_code_MSB
#define CAP_CRC_CODE_MSB(n)         (((n)&0xFF)<<0)




#endif
