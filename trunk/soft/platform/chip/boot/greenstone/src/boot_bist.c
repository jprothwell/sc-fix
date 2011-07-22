////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/boot/greenstone/src/boot_bist.c $ //
//  $Author: admin $                                                         //
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //
//  $Revision: 269 $                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file boot_bist.c                                                         //
///     Internal RAMs BIST Production test
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "global_macros.h"
#include "mem_bridge.h"
#include "bist.h"
#include "sys_ctrl.h"
#include "xcpu.h"
#include "bcpu_cache.h"
#include "bcpu_tag.h"
#include "bb_sram.h"
#include "xcpu_cache.h"
#include "xcpu_tag.h"
#include "gpio.h"

#if (VOC_OPTION == 1)
#include "voc_cfg.h"
#endif
#if (USB_OPTION == 1)
#include "usbc.h"
#endif


#include "cfg_regs.h"
#include "ana_acco.h"

extern UINT32 bcpu_main_entry;
extern UINT32 bcpu_stack_base;
extern void boot_FlushCache(void);

void boot_BistBCpu(void);
int boot_MarchCBBCp2 (void);
int boot_MarchCPa (void);
int boot_MarchCTcu (void);


typedef struct {
    INT32 errorCount;
    INT32 bistBBIdx;
    INT32 bistSysIdx;
    INT32 bistMemIdx;
} BOOT_BIST_STATE_T;

typedef struct {
    UINT32 addr;
    UINT32 size;
    UINT32 mask;
    UINT32 other;
} BOOT_BIST_CONF_T;

register BOOT_BIST_STATE_T* g_bootBistStatePtr asm("$17"); // $17 is s1

// fake global absolute uncached variable
#define b_bootBistBcpuEnd *((UINT32*)(0xa0000000+ REG_BB_SRAM_BASE))

#if 1
// to be redefined for real test to change a gpio or so
#define ERROR() {g_bootBistStatePtr->errorCount++; hwp_gpio->gpo_set=(1<<6);}
#define SUCCESS() {hwp_gpio->gpo_set=(1<<7);}

#define DEBUG_PRINT(a)

#else

#define ERROR() {g_bootBistStatePtr->errorCount++; CT_stop_sim(stop_on_error);}
#define SUCCESS() CT_stop_sim(stop_on_success)
#define DEBUG_PRINT CT_simu_print
#endif




#define BOOT_BIST_BB_NB_CONF 5
// assuming less of 4kByte caches of bcpu (no page change)
const BOOT_BIST_CONF_T g_bootBistBBConf[BOOT_BIST_BB_NB_CONF] = 
    { {REG_BCPU_DDATA_BASE, sizeof(HWP_BCPU_CACHE_T), ~0, 0}, // Data cache ram
      {REG_BCPU_IDATA_BASE, sizeof(HWP_BCPU_CACHE_T), ~0, 0}, // Instruction cache ram
      {REG_BCPU_TAG_BASE, sizeof(HWP_BCPU_TAG_T), BCPU_TAG_VALID | BCPU_TAG_TAG_MASK, XCPU_DEBUG_PAGE_ADDRESS_DTAG}, // Data tag ram
      {REG_BCPU_TAG_BASE, sizeof(HWP_BCPU_TAG_T), BCPU_TAG_VALID | BCPU_TAG_TAG_MASK, XCPU_DEBUG_PAGE_ADDRESS_ITAG}, // Instruction tag ram
      {REG_BB_SRAM_BASE, sizeof(HWP_BB_SRAM_T), ~0, 0} }; // BB_SRAM


int boot_BistBBScheduler(void)
{
    register UINT32 t;
    if (g_bootBistStatePtr->bistBBIdx < BOOT_BIST_BB_NB_CONF) {
        if (g_bootBistStatePtr->bistBBIdx == -1) {
            t = 0;
        } else {
            t=hwp_bbBist->status;
            if (t & BIST_FAIL) {
                DEBUG_PRINT("bb bist scheduler error");
                ERROR();
            }
        }
        if ((t & BIST_ENABLE) == 0) {
            g_bootBistStatePtr->bistBBIdx++;
            if (g_bootBistStatePtr->bistBBIdx < BOOT_BIST_BB_NB_CONF) {
                hwp_bcpu->Debug_Page_Address = g_bootBistBBConf[g_bootBistStatePtr->bistBBIdx].other;
                hwp_bbBist->mem_addr = g_bootBistBBConf[g_bootBistStatePtr->bistBBIdx].addr;
                hwp_bbBist->mem_size = g_bootBistBBConf[g_bootBistStatePtr->bistBBIdx].size;
                hwp_bbBist->data_mask = g_bootBistBBConf[g_bootBistStatePtr->bistBBIdx].mask;
                hwp_bbBist->control = BIST_START;
            } else if ( BOOT_BIST_BB_NB_CONF == g_bootBistStatePtr->bistBBIdx) {
                // start bcpu
#if 0
    // #error "TODO: update this for real chip production rom"
    UINT32* ptr = (UINT32*)0xa0aa3344;
    ptr[1] = 0xa1001000;
    ptr[0] = 0xa0000000+ REG_BB_SRAM_BASE + BB_SRAM_SIZE;
#endif
                bcpu_stack_base = 0xa0000000+ REG_BB_SRAM_BASE + BB_SRAM_SIZE;
                bcpu_main_entry = (UINT32)&boot_BistBCpu;
                b_bootBistBcpuEnd = 0;
                DEBUG_PRINT("bb bist scheduler start bcpu");
                hwp_sysCtrl->BCpu_Dbg_BKP = 0;
            }
        }
        return 1;
    } else if ( BOOT_BIST_BB_NB_CONF == g_bootBistStatePtr->bistBBIdx) {
        // bcpu tests
        if (b_bootBistBcpuEnd > 1) {
            if (b_bootBistBcpuEnd != 2) {
                DEBUG_PRINT("bb bist scheduler error: bcpu failed");
                ERROR();
            }
            g_bootBistStatePtr->bistBBIdx++;
        }
        return 1;
    }

    return 0;
}



#define BOOT_BIST_SYS_NB_CONF 6
// assuming less of 4kByte caches of xcpu (no page change)
const BOOT_BIST_CONF_T g_bootBistSysConf[BOOT_BIST_SYS_NB_CONF] = 
    { {REG_XCPU_DDATA_BASE, sizeof(HWP_XCPU_CACHE_T), ~0, 0}, // Data cache ram
      {REG_XCPU_IDATA_BASE, sizeof(HWP_XCPU_CACHE_T), ~0, 0}, // Instruction cache ram
      {REG_XCPU_TAG_BASE, sizeof(HWP_XCPU_TAG_T), XCPU_TAG_VALID | XCPU_TAG_TAG_MASK, XCPU_DEBUG_PAGE_ADDRESS_DTAG}, // Data tag ram
      {REG_XCPU_TAG_BASE, sizeof(HWP_XCPU_TAG_T), XCPU_TAG_VALID | XCPU_TAG_TAG_MASK, XCPU_DEBUG_PAGE_ADDRESS_ITAG},  // Instruction tag ram
      {(UINT32)hwp_usbc->SPFIFORAM, SPFIFORAM_SIZE*4, ~0, 0},
      {(UINT32)hwp_usbc->SPFIFORAM, SPFIFORAM_SIZE*4, ~0, SYS_CTRL_USB_RAM_TEST_MODE_ENABLE},
    };


int boot_BistSysScheduler(void)
{
    register UINT32 t;
    if (g_bootBistStatePtr->bistSysIdx < BOOT_BIST_SYS_NB_CONF) {
        if (g_bootBistStatePtr->bistSysIdx == -1) {
            t = 0;
        } else {
            t=hwp_sysBist->status;
            if (t & BIST_FAIL) {
                DEBUG_PRINT("sys bist scheduler error");
                ERROR();
            }
        }
        if ((t & BIST_ENABLE) == 0) {
            g_bootBistStatePtr->bistSysIdx++;
            if (g_bootBistStatePtr->bistSysIdx < BOOT_BIST_SYS_NB_CONF) {
                if (g_bootBistSysConf[g_bootBistStatePtr->bistSysIdx].addr == ((UINT32)hwp_usbc->SPFIFORAM)) {
                    DEBUG_PRINT("sys bist scheduler in usb if");
                    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

                    hwp_sysCtrl->Cfg_AHB = SYS_CTRL_SYS_ENABLE_MASK | SYS_CTRL_BB_ENABLE_MASK | g_bootBistSysConf[g_bootBistStatePtr->bistSysIdx].other;
                    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
                } else
                    hwp_xcpu->Debug_Page_Address = g_bootBistSysConf[g_bootBistStatePtr->bistSysIdx].other;

                hwp_sysBist->mem_addr = g_bootBistSysConf[g_bootBistStatePtr->bistSysIdx].addr;
                hwp_sysBist->mem_size = g_bootBistSysConf[g_bootBistStatePtr->bistSysIdx].size;
                hwp_sysBist->data_mask = g_bootBistSysConf[g_bootBistStatePtr->bistSysIdx].mask;
                hwp_sysBist->control = BIST_START;
            } 
            if ( 4 == g_bootBistStatePtr->bistSysIdx) {
                // flush the caches
                boot_FlushCache();
                // reenable all XCPU caches
                hwp_xcpu->Cache_Control = 0;
                {UINT32 unused __attribute__ ((unused)) = hwp_xcpu->Cache_Control;}
            }
        }
        return 1;
    } else if (g_bootBistStatePtr->bistSysIdx == BOOT_BIST_SYS_NB_CONF) {
        // only bcpu still runing:
        if (g_bootBistStatePtr->bistBBIdx >= BOOT_BIST_BB_NB_CONF) {
            // test TCU
            if(boot_MarchCTcu()) {
                DEBUG_PRINT("tcu march error");
                ERROR();
            }
            g_bootBistStatePtr->bistSysIdx++;
        }
        return 1;
    } else if (g_bootBistStatePtr->bistSysIdx == BOOT_BIST_SYS_NB_CONF +1 ) {
        {
            // test PA
            if(boot_MarchCPa()) {
                DEBUG_PRINT("pa march error");
                ERROR();
            }
            g_bootBistStatePtr->bistSysIdx++;
        }
        return 1;
    }
    return 0;
}

#define BOOT_BIST_MEM_NB_CONF 2
int boot_BistMemScheduler(void)
{
    register UINT32 t;
    if (g_bootBistStatePtr->bistMemIdx < BOOT_BIST_MEM_NB_CONF) {
        if (g_bootBistStatePtr->bistMemIdx == -1) {
            t = 0;
            // start membridge bist : rom
            hwp_memBridge->Rom_Bist = MEM_BRIDGE_CTRL_RUNNING;
        } else {
            t=hwp_memBridge->SRam_Bist;
            if ((t & MEM_BRIDGE_CTRL_RUNNING) == 0) {
                if (t != (MEM_BRIDGE_STATUS_OK | MEM_BRIDGE_BLOCK_SEL(g_bootBistStatePtr->bistMemIdx))) {
                    DEBUG_PRINT("mem_bridge RAM Bist error");
                    ERROR();
                }
            }
        }
        if ((t & MEM_BRIDGE_CTRL_RUNNING) == 0) {
            g_bootBistStatePtr->bistMemIdx++;
            if (g_bootBistStatePtr->bistMemIdx < BOOT_BIST_MEM_NB_CONF) {
                // start membridge bist : ram
                hwp_memBridge->SRam_Bist = MEM_BRIDGE_CTRL_RUNNING | MEM_BRIDGE_BLOCK_SEL(g_bootBistStatePtr->bistMemIdx);

            } else if ( BOOT_BIST_MEM_NB_CONF == g_bootBistStatePtr->bistMemIdx) {
                if( (MEM_BRIDGE_CTRL_DONE | MEM_BRIDGE_CRC(0xc001)) != hwp_memBridge->Rom_Bist) {
                    DEBUG_PRINT("mem_bridge ROM Bist error");
                    ERROR();
                }
            }
        }
        return 1;
    }
    return 0;
}

// check the membridge bist status of first block of ram for step 1
int boot_BistMemStep1(void)
{
    register UINT32 t;
    if (g_bootBistStatePtr->bistMemIdx < 1) {
        if (g_bootBistStatePtr->bistMemIdx == -1) {
            t = 0;
            // start membridge bist : rom
            hwp_memBridge->Rom_Bist = MEM_BRIDGE_CTRL_RUNNING;
        } else {
            t=hwp_memBridge->SRam_Bist;
            if ((t & MEM_BRIDGE_CTRL_RUNNING) == 0) {
                if (t != (MEM_BRIDGE_STATUS_OK | MEM_BRIDGE_BLOCK_SEL(g_bootBistStatePtr->bistMemIdx))) {
                    DEBUG_PRINT("mem_bridge RAM Bist error");
                    ERROR();
                }
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

void boot_BistStep1(void)
{
    int status;

    g_bootBistStatePtr = (BOOT_BIST_STATE_T*) 0x80B8801C; // dma pattern and nexts 
    g_bootBistStatePtr->errorCount= 0;
    g_bootBistStatePtr->bistBBIdx =-1;
    g_bootBistStatePtr->bistSysIdx=-1;
    g_bootBistStatePtr->bistMemIdx=-1;

    // start membridge bist : 1st block + rom
    boot_BistMemScheduler();

    // start voc Bist
    hwp_vocCfg->BIST_Ctrl = VOC_CFG_BIST_RUN;

    // LPG cleared and as GPO mode: GPO 6 and 7
    //asm volatile("nop;\nboot_Bist_zob: nop;");
    hwp_gpio->gpo_clr = (3<<6);
    hwp_extApb->Alt_mux_select |= REGS_PWL0_GPO_7 | REGS_PWL1_GPO_6;



    // stall bcpu, out of reset and start bist of it's caches
    hwp_sysCtrl->BCpu_Dbg_BKP = SYS_CTRL_STALLED;
    hwp_sysCtrl->BB_Rst_Clr = SYS_CTRL_CLR_RST_BCPU;

    // disable all XCPU caches : done in assembly before entering this function
    //hwp_xcpu->Cache_Control = XCPU_DCACHE_INHIBIT | XCPU_ICACHE_INHIBIT;
    //{u32 unused __attribute__ ((unused)) = hwp_xcpu->Cache_Control;}

    // give a clock to usb: acco pll in bypass
    hwp_anaAccoFe -> pll_ctrl = ANA_ACCO_PLL_PD_N_DISABLE | ANA_ACCO_PLL_BYPASS | ANA_ACCO_PLL_IN_SOURCE_CLOCKSQUARER | ANA_ACCO_PLL_EXT_EON_ENABLE;


    status=1;
    while (boot_BistMemStep1())
    {
        status = 0;
        // bcpu caches and tags + bbsram
        status += boot_BistBBScheduler();

        // xcpu caches and tags + usb +soft ( tcu + pa)
        status += boot_BistSysScheduler();

    }
    
}

void boot_BistStep2(void)
{
    int status;
    g_bootBistStatePtr = (BOOT_BIST_STATE_T*) 0x80B8801C; // dma pattern and nexts 

    status=1;
    while (status)
    {
        status = 0;
        // bcpu caches and tags + bbsram
        status += boot_BistBBScheduler();

        // xcpu caches and tags + usb +soft ( tcu + pa)
        status += boot_BistSysScheduler();

        // mem bridge ram & rom
        status += boot_BistMemScheduler();

    }
    
    // check voc bist status and crc value
    if( VOC_CFG_BIST_CRC(0xc001) != hwp_vocCfg->BIST_Ctrl) {
        DEBUG_PRINT("VoC Bist error");
        ERROR();
    }

    if (g_bootBistStatePtr->errorCount==0)
    {    
        SUCCESS();
    }
    else
    {
        ERROR();
    }    

    while (1);

}
/******************************************************************************/


void boot_BistBCpu(void)
{
    b_bootBistBcpuEnd = 1;
    if(boot_MarchCBBCp2()) {
        b_bootBistBcpuEnd = 10;
        ERROR();
    }
    b_bootBistBcpuEnd = 2;
    while (1);
}

/******************************************************************************/

#define MEM_READ_UNCACHE(addr)          ( *(volatile unsigned long *)((addr) | (0xa0000000)) )
#define MEM_WRITE_UNCACHE(addr, data)   ( *(volatile unsigned long *)((addr) | (0xa0000000)) ) = data

#define ZEROS       0xaaaaaaaa
#define ONES        0x55555555


#if 0 //debug

#define Stop(a,b,c) {\
        ct_setW0(a);\
        ct_setW0(b);\
        ct_setW1(c);\
        return 1;\
    }

#else
#define Stop(a,b,c) return 1
#endif

/******************************************************************************/
#include "tcu.h"
#define FORCE_ALL TCU_FORCELATCH_MASK

int boot_MarchCTcu(void)
{

    int i,k,Incr;
    unsigned int ReadVal_A, ReadVal_P, ReadExp, WriteVal, RdTime, WrTime;
    unsigned int ptr, EvId, Start;

    
    // setup gsm clocks:
    hwp_tcu->Cfg_Clk_Div= TCU_ENABLE_QBIT_ENABLED;

    // set both latch rams to 0
    hwp_tcu->Latch = TCU_CLEARPROGAREA;
    hwp_tcu->Latch = FORCE_ALL;
    hwp_tcu->Ctrl = TCU_LOAD_VAL(0x2aaa) | TCU_LOAD | TCU_ENABLE_ENABLED;
    
    for (k = 0 ; k<2; k++) {
        if ((k&0x1)==0) {
            /* ----------------------------------------------------- */
            // -- Going Down
            /* ----------------------------------------------------- */
            Incr = 4;
            Start = (unsigned int) (hwp_tcu->Event);
        } else {
            Incr = -4;
            Start = (unsigned int) (hwp_tcu->Event+NB_TCU_PROG_EVENTS-1);
        }
        ptr = Start;
        for (i=0; i<60; i++) {
            MEM_WRITE_UNCACHE(ptr,0x151555);
            ptr+=Incr;
        }
        hwp_tcu->Latch = FORCE_ALL;
    //######### one loop
        ptr = Start;
        ReadExp = 0x151555;
        WrTime = 0x2aaa;
        for (i=0; i<15; i++) {
            EvId = (2*i+1);
            WriteVal = (TCU_EVENT_TIME(WrTime) | TCU_EVENT_ID(EvId));
            ReadVal_P = MEM_READ_UNCACHE(ptr);
            MEM_WRITE_UNCACHE(ptr,WriteVal);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_DEBUG;
            ReadVal_A = MEM_READ_UNCACHE(ptr);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_NORMAL;
            hwp_tcu->Latch = FORCE_ALL;
            if (ReadVal_A != ReadExp) Stop(ptr, ReadVal_A, ReadExp);
            if (ReadVal_P != ReadExp) Stop(ptr, ReadVal_P, ReadExp);
            ptr+=Incr;
        }
        hwp_tcu->Ctrl = TCU_LOAD_VAL(WrTime-1) | TCU_LOAD | TCU_ENABLE_ENABLED;
        while (hwp_tcu->Cur_Val < WrTime + 1);
        WrTime = 0x2aab;
        for (i=15; i<30; i++) {
            EvId = 2*(i-15);
            WriteVal = (TCU_EVENT_TIME(WrTime) | TCU_EVENT_ID(EvId));
            ReadVal_P = MEM_READ_UNCACHE(ptr);
            MEM_WRITE_UNCACHE(ptr,WriteVal);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_DEBUG;
            ReadVal_A = MEM_READ_UNCACHE(ptr);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_NORMAL;
            hwp_tcu->Latch = FORCE_ALL;
            if (ReadVal_A != ReadExp) Stop(ptr, ReadVal_A, ReadExp);
            if (ReadVal_P != ReadExp) Stop(ptr, ReadVal_P, ReadExp);
            ptr+=Incr;
        }
        hwp_tcu->Ctrl = TCU_LOAD_VAL(WrTime-1) | TCU_LOAD | TCU_ENABLE_ENABLED;
        while (hwp_tcu->Cur_Val < WrTime + 1);
        WrTime = 0x2aac;
        for (i=30; i<45; i++) {
            EvId = 2*(i-30)+1;
            WriteVal = (TCU_EVENT_TIME(WrTime) | TCU_EVENT_ID(EvId));
            ReadVal_P = MEM_READ_UNCACHE(ptr);
            MEM_WRITE_UNCACHE(ptr,WriteVal);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_DEBUG;
            ReadVal_A = MEM_READ_UNCACHE(ptr);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_NORMAL;
            hwp_tcu->Latch = FORCE_ALL;
            if (ReadVal_A != ReadExp) Stop(ptr, ReadVal_A, ReadExp);
            if (ReadVal_P != ReadExp) Stop(ptr, ReadVal_P, ReadExp);
            ptr+=Incr;
        }
        hwp_tcu->Ctrl = TCU_LOAD_VAL(WrTime-1) | TCU_LOAD | TCU_ENABLE_ENABLED;
        while (hwp_tcu->Cur_Val < WrTime + 1);
        WrTime = 0x2aad;
        for (i=45; i<60; i++) {
            EvId      = 2*(i-45);
            WriteVal  = (TCU_EVENT_TIME(WrTime) | TCU_EVENT_ID(EvId));
            ReadVal_P = MEM_READ_UNCACHE(ptr);
            MEM_WRITE_UNCACHE(ptr,WriteVal);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_DEBUG;
            ReadVal_A = MEM_READ_UNCACHE(ptr);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_NORMAL;
            hwp_tcu->Latch = FORCE_ALL;
            if (ReadVal_A != ReadExp) Stop(ptr, ReadVal_A, ReadExp);
            if (ReadVal_P != ReadExp) Stop(ptr, ReadVal_P, ReadExp);
            ptr+=Incr;
        }
        hwp_tcu->Ctrl = TCU_LOAD_VAL(WrTime-1) | TCU_LOAD | TCU_ENABLE_ENABLED;
        while (hwp_tcu->Cur_Val < WrTime + 1);
    //#########
    //######### one loop
        ptr = Start;
        RdTime = 0x2aaa;
        WrTime = 0x1555;
        for (i=0; i<15; i++) {
            EvId      = 2*i+1;
            ReadExp   = (TCU_EVENT_TIME(RdTime) | TCU_EVENT_ID(EvId));
            WriteVal  = (TCU_EVENT_TIME(WrTime) | TCU_EVENT_ID(EvId));
            ReadVal_P = MEM_READ_UNCACHE(ptr);
            MEM_WRITE_UNCACHE(ptr,WriteVal);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_DEBUG;
            ReadVal_A = MEM_READ_UNCACHE(ptr);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_NORMAL;
            hwp_tcu->Latch = FORCE_ALL;
            if (ReadVal_A != ReadExp) Stop(ptr, ReadVal_A, ReadExp);
            if (ReadVal_P != ReadExp) Stop(ptr, ReadVal_P, ReadExp);
            ptr+=Incr;
        }
        hwp_tcu->Ctrl = TCU_LOAD_VAL(WrTime-1) | TCU_LOAD | TCU_ENABLE_ENABLED;
        while (hwp_tcu->Cur_Val < WrTime + 1);
        RdTime = 0x2aab;
        WrTime = 0x1556;
        for (i=15; i<30; i++) {
            EvId      = 2*(i-15);
            ReadExp   = (TCU_EVENT_TIME(RdTime) | TCU_EVENT_ID(EvId));
            WriteVal  = (TCU_EVENT_TIME(WrTime) | TCU_EVENT_ID(EvId));
            ReadVal_P = MEM_READ_UNCACHE(ptr);
            MEM_WRITE_UNCACHE(ptr,WriteVal);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_DEBUG;
            ReadVal_A = MEM_READ_UNCACHE(ptr);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_NORMAL;
            hwp_tcu->Latch = FORCE_ALL;
            if (ReadVal_A != ReadExp) Stop(ptr, ReadVal_A, ReadExp);
            if (ReadVal_P != ReadExp) Stop(ptr, ReadVal_P, ReadExp);
            ptr+=Incr;
        }
        hwp_tcu->Ctrl = TCU_LOAD_VAL(WrTime-1) | TCU_LOAD | TCU_ENABLE_ENABLED;
        while (hwp_tcu->Cur_Val < WrTime + 1);
        RdTime = 0x2aac;
        WrTime = 0x1557;
        for (i=30; i<45; i++) {
            EvId      = 2*(i-30)+1;
            ReadExp   = (TCU_EVENT_TIME(RdTime) | TCU_EVENT_ID(EvId));
            WriteVal  = (TCU_EVENT_TIME(WrTime) | TCU_EVENT_ID(EvId));
            ReadVal_P = MEM_READ_UNCACHE(ptr);
            MEM_WRITE_UNCACHE(ptr,WriteVal);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_DEBUG;
            ReadVal_A = MEM_READ_UNCACHE(ptr);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_NORMAL;
            hwp_tcu->Latch = FORCE_ALL;
            if (ReadVal_A != ReadExp) Stop(ptr, ReadVal_A, ReadExp);
            if (ReadVal_P != ReadExp) Stop(ptr, ReadVal_P, ReadExp);
            ptr+=Incr;
        }
        hwp_tcu->Ctrl = TCU_LOAD_VAL(WrTime-1) | TCU_LOAD | TCU_ENABLE_ENABLED;
        while (hwp_tcu->Cur_Val < WrTime + 1);
        RdTime = 0x2aad;
        WrTime = 0x1558;
        for (i=45; i<60; i++) {
            EvId = 2*(i-45);
            ReadExp   = (TCU_EVENT_TIME(RdTime) | TCU_EVENT_ID(EvId));
            WriteVal  = (TCU_EVENT_TIME(WrTime) | TCU_EVENT_ID(EvId));
            ReadVal_P = MEM_READ_UNCACHE(ptr);
            MEM_WRITE_UNCACHE(ptr,WriteVal);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_DEBUG;
            ReadVal_A = MEM_READ_UNCACHE(ptr);
            hwp_tcu->Setup=TCU_DEBUG_ACTIVE_NORMAL;
            hwp_tcu->Latch = FORCE_ALL;
            if (ReadVal_A != ReadExp) Stop(ptr, ReadVal_A, ReadExp);
            if (ReadVal_P != ReadExp) Stop(ptr, ReadVal_P, ReadExp);
            ptr+=Incr;
        }
        hwp_tcu->Ctrl = TCU_LOAD_VAL(WrTime-1) | TCU_LOAD | TCU_ENABLE_ENABLED;
        while (hwp_tcu->Cur_Val < WrTime + 1);
        //#########
    }

    return 0;
}


/******************************************************************************/
#include "pa_ctrl.h"

int boot_MarchCPa(void)
{

// March-C test 7 loops:
//  0: <-> W0
//  1:  -> R0; W1
//  2:  -> R1; W0
//  3: <-> R0
//  4: <-  R0; W1
//  5: <-  R1; W0
//  6: <-> R0
// Instead of using all 1's and all 0's, could modify to use a's and 5's.

    int Index;
    unsigned int ReadVal;
    unsigned int ptr;
    unsigned int Mask;

// Initialize RAM with zero's.  First loop
    ptr = REG_PA_CTRL_BASE + 0x4;
    for (Index = 0; Index < 30; Index++)
    {
        MEM_WRITE_UNCACHE(ptr, ZEROS);
        ptr += sizeof(unsigned int);
    }

//----------------------------------
//          BEGIN LOOP 1
    ptr = REG_PA_CTRL_BASE + 0x4;
    Mask = 0x3FFFFFFF ;
    for (Index = 0; Index < 30; Index++)
    {   
        if((Index == 5) || (Index == 11) || (Index == 17) || (Index == 23) || (Index == 29))
            Mask = 0x3FF ;
        else
            Mask = 0x3FFFFFFF ;

        ReadVal = MEM_READ_UNCACHE(ptr) & Mask;
        MEM_WRITE_UNCACHE(ptr, ONES);
        if (ReadVal != (ZEROS & Mask))
            Stop(ptr, ReadVal, (ZEROS & Mask));

            ptr += sizeof(unsigned int);
    }

//          END LOOP 1
//----------------------------------
//----------------------------------
//          BEGIN LOOP 2
    ptr = REG_PA_CTRL_BASE + 0x4;
    for (Index = 0; Index < 30; Index++)
    {
        if((Index == 5) || (Index == 11) || (Index == 17) || (Index == 23) || (Index == 29))
            Mask = 0x3FF ;
        else
            Mask = 0x3FFFFFFF ;

        ReadVal = MEM_READ_UNCACHE(ptr) & Mask;
        MEM_WRITE_UNCACHE(ptr, ZEROS);
        if (ReadVal != (ONES & Mask))
            Stop(ptr, ReadVal, (ONES & Mask));

            ptr += sizeof(unsigned int);
    }
//          END LOOP 2
//----------------------------------
//----------------------------------
//          BEGIN LOOP 3
    Mask = 0x3FFFFFFF ;
    ptr = REG_PA_CTRL_BASE + 0x4;
    for (Index = 0; Index < 30; Index++)
    {
        if((Index == 5) || (Index == 11) || (Index == 17) || (Index == 23) || (Index == 29))
            Mask = 0x3FF ;
        else
            Mask = 0x3FFFFFFF ;
        ReadVal = MEM_READ_UNCACHE(ptr) & Mask;
        if (ReadVal != (ZEROS & Mask))
            Stop(ptr, ReadVal, (ZEROS & Mask));

            ptr += sizeof(unsigned int);
    }
//          END LOOP 3
//----------------------------------
//----------------------------------
//          BEGIN LOOP 4
    ptr = REG_PA_CTRL_BASE + 0x4 + ((30-1)*sizeof(unsigned int));
    for (Index = 0; Index < 30; Index++)
    {
        if((Index == 0) || (Index == 6) || (Index == 12) || (Index == 18) || (Index == 24))
            Mask = 0x3FF ;
        else
            Mask = 0x3FFFFFFF ;
        ReadVal = MEM_READ_UNCACHE(ptr) & Mask;
        MEM_WRITE_UNCACHE(ptr, ONES);
        if (ReadVal != (ZEROS & Mask))
            Stop(ptr, ReadVal, (ZEROS & Mask));

            ptr -= sizeof(unsigned int);
    }
//          END LOOP 4
//----------------------------------
//----------------------------------
//          BEGIN LOOP 5
    ptr = REG_PA_CTRL_BASE + 0x4 + ((30-1)*sizeof(unsigned int));
    for (Index = 0; Index < 30; Index++)
    {
        if((Index == 0) || (Index == 6) || (Index == 12) || (Index == 18) || (Index == 24))
            Mask = 0x3FF ;
        else
            Mask = 0x3FFFFFFF ;
        ReadVal = MEM_READ_UNCACHE(ptr) & Mask;
        MEM_WRITE_UNCACHE(ptr, ZEROS);
        if (ReadVal != (ONES & Mask))
            Stop(ptr, ReadVal, (ONES & Mask));

            ptr -= sizeof(unsigned int);
    }

//          END LOOP 5
//----------------------------------
//----------------------------------
//          BEGIN LOOP 6
    ptr = REG_PA_CTRL_BASE + 0x4 + ((30-1)*sizeof(unsigned int));
    for (Index = 0; Index < 30; Index++)
    {
        if((Index == 0) || (Index == 6) || (Index == 12) || (Index == 18) || (Index == 24))
            Mask = 0x3FF ;
        else
            Mask = 0x3FFFFFFF ;
        ReadVal = MEM_READ_UNCACHE(ptr) & Mask;
        if (ReadVal != (ZEROS & Mask))
            Stop(ptr, ReadVal, (ZEROS & Mask));

            ptr -= sizeof(unsigned int);
    }

//          END LOOP 6
//----------------------------------


return 0;
}
//------------- MARCH PA

/******************************************************************************/
