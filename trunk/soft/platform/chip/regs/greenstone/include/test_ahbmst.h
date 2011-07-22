#ifndef TEST_AHBMST_H
#define TEST_AHBMST_H

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'test_ahbmst'."
#endif

#include "globals.h"



#define MEM_LEN (64)
#define MEM_LEN_BITS (6)

////////////////////////////////////////////////////////////////////////////////
// test_ahbmst
////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////
#define REG_SYS_TEST_AHBMST_BASE    0x00B18000
#define REG_BB_TEST_AHBMST_BASE     0x00BC9000


typedef volatile struct
{
    REG32 En;                                   //0x00000000
    REG32 Status;                               //0x00000004
    REG32 StartAddr;                            //0x00000008
    REG32 Length;                               //0x0000000C
    REG32 Busy;                                 //0x00000010
    REG32 Reserved_00000014;                    //0x00000014
    REG32 CurAddr;                              //0x00000018
    REG32 AccessCount;                          //0x0000001C
    REG32 Reserved_00000020[56];                //0x00000020
    REG32 data[MEM_LEN];                            //0x00000100
} HWP_TEST_AHBMST_T;

#define hwp_sysTestAhbmst           ((HWP_TEST_AHBMST_T*) KSEG1(REG_SYS_TEST_AHBMST_BASE))
#define hwp_bbTestAhbmst            ((HWP_TEST_AHBMST_T*) KSEG1(REG_BB_TEST_AHBMST_BASE))

//En
#define TEST_AHBMST_ENABLE          (1<<0)
#define TEST_AHBMST_WRITE           (1<<4)
#define TEST_AHBMST_HBURST(n)       (((n)&7)<<8)
#define TEST_AHBMST_HBURST_SINGLE   (0<<8)
#define TEST_AHBMST_HBURST_INCR     (1<<8)
#define TEST_AHBMST_HBURST_WRAP4    (2<<8)
#define TEST_AHBMST_HBURST_INCR4    (3<<8)
#define TEST_AHBMST_HBURST_WRAP8    (4<<8)
#define TEST_AHBMST_HBURST_INCR8    (5<<8)
#define TEST_AHBMST_HBURST_WRAP16   (6<<8)
#define TEST_AHBMST_HBURST_INCR16   (7<<8)
#define TEST_AHBMST_FULLBURST       (1<<12)
#define TEST_AHBMST_HSIZE(n)        (((n)&7)<<16)
#define TEST_AHBMST_HSIZE_BYTE      (0<<16)
#define TEST_AHBMST_HSIZE_HALFWORD  (1<<16)
#define TEST_AHBMST_HSIZE_WORD      (2<<16)

//Status
#define TEST_AHBMST_RUNNING         (1<<0)
#define TEST_AHBMST_GOTERR          (1<<4)

//StartAddr
#define TEST_AHBMST_STARTADDR(n)    (((n)&0x3FFFFFF)<<0)

//Length
#define TEST_AHBMST_LENGTH(n)       (((n)&0x3F)<<2)

//Busy
#define TEST_AHBMST_BUSYGENDELAY(n) (((n)&15)<<0)
#define TEST_AHBMST_BUSYGENRELOAD(n) (((n)&15)<<8)
#define TEST_AHBMST_BUSYLENGTH(n)   (((n)&15)<<16)
#define TEST_AHBMST_BUSYVARIABLELENGTH (1<<24)

//CurAddr
#define TEST_AHBMST_CURADDR(n)      (((n)&0x3FFFFFF)<<0)

//AccessCount
#define TEST_AHBMST_ACCESSCOUNT(n)  (((n)&0x3F)<<2)

//data
#define TEST_AHBMST_DATA(n)         (((n)&0xFFFFFFFF)<<0)





#endif
