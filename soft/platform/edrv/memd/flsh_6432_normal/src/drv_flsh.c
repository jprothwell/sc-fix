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
//  $HeadURL: svn://atlas/SVN_MIRROR/dev_mirror/modem2G/trunk/platform/edrv/memd/flsh_k5l2731caa_d770/src/drv_flsh.c $ //
//	$Author: ytt $                                                        // 
//	$Date: 2011-01-22 11:00:00 +0800  $                     //   
//	$Revision: 7772 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file drv_flsh.c                                                          //
/// That file provides the driver implementation for the k5l2731caa_d770      //
/// flash.                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "hal_ebc.h"
#include "hal_sys.h"
//#include "sxs_io.h"

#include "string.h"

#include "memd_m.h"
#include "memdp_debug.h"
#include "memdp.h"
#include "mem_id.h"
#include "memd_tgt_params_greenstone_3216.h"
#include "hal_host.h"


//UINT32 dds[100]={1};
#define FLASH_DEBUG_HST(n)   //hal_HstSendEvent(n)
/* banks at flash bit 20 */
/* for eon 4M */
#define FLASHBANK_MASK      (0x3<<(22))     // Actually only need 1 bits, but have to use 2 due to shift

#define KBYTES 1024
#define MBYTES (1024*KBYTES)

#define FLASH_ADDR_MASK 0x00ffffff
#define MY_MASK 0x00FFFFFF      // FIX MY_MASK is only used for masking address to Block Lock function.  Do it properly?

#define KBYTES              1024
#define MBYTES              (1024*KBYTES)

#define CMD_READ_STATUS     0x70
#define CMD_READ_ESD        0x90
#define CMD_BLOCK_PROT      0x60
#define CMD_BLOCK_LOCK      0x01
#define CMD_BLOCK_UNLOCK    0xD0
#define CMD_READ_ARRAY      0xFF
#define CMD_BLOCK_ERASE_1   0x20
#define CMD_BLOCK_ERASE_2   0xD0
#define CMD_PROG            0x40
#define CMD_SET_CFG_REG_1   0x60
#define CMD_SET_CFG_REG_2   0x03
#define CMD_CLR_STATUS_REG  0x50
#define CMD_ERASE_SUSPEND   0xb0
#define CMD_EEASE_RESUME    0xd0
#define CMD_READ_CFI_QUERY  0x98

#define SR0                 (1<<0)
#define SR1                 (1<<1)
#define SR2                 (1<<2)
#define SR3                 (1<<3)
#define SR4                 (1<<4)
#define SR5                 (1<<5)
#define SR6                 (1<<6)
#define SR7                 (1<<7)

#define SECOND        * HAL_TICK1S
#define MILLI_SECOND  SECOND / 1000
#define MILLI_SECONDS MILLI_SECOND
// -------------------------------
// FLASH/RAM configuration
// -------------------------------
#define FLASH_SIZE  8*MBYTES 

extern CONST FLASH_AUTOSELECTOR_ID g_flash_ID[];

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
/// Base address
UINT32 g_memdFlashBaseAddress = 0xa2000000;

PUBLIC CONST MEMD_FLASH_LAYOUT_T * g_memdFlashLayout;
PUBLIC CONST MEMD_RAM_CONFIG_T * g_MemdRamCfg;
PUBLIC CONST MEMD_FLASH_CONFIG_T * g_MemdFlashCfg;

PROTECTED CONST MEMD_RAM_CONFIG_T g_tgtMemdRamCfg_en71pl064b0 = TGT_MEMD_RAM_CONFIG_EN71PL064B0;
PROTECTED CONST MEMD_RAM_CONFIG_T g_tgtMemdRamCfg_m36cow6050tozsp = TGT_MEMD_RAM_CONFIG_M36COW6050TOZSP;
PROTECTED CONST MEMD_RAM_CONFIG_T g_tgtMemdRamCfg_s71pl064ja0 = TGT_MEMD_RAM_CONFIG_S71PL064JA0;
PROTECTED CONST MEMD_RAM_CONFIG_T g_tgtMemdRamCfg_s71pl064jb0 = TGT_MEMD_RAM_CONFIG_S71PL064JB0;

// flash timing
PROTECTED CONST MEMD_FLASH_CONFIG_T g_tgtMemdFlashCfg_en71pl064b0 = TGT_MEMD_FLASH_CONFIG_EN71PL064B0;
PROTECTED CONST MEMD_FLASH_CONFIG_T g_tgtMemdFlashCfg_m36cow6050tozsp = TGT_MEMD_FLASH_CONFIG_M36COW6050TOZSP;
PROTECTED CONST MEMD_FLASH_CONFIG_T g_tgtMemdFlashCfg_s71pl064ja0 = TGT_MEMD_FLASH_CONFIG_S71PL064JA0;
PROTECTED CONST MEMD_FLASH_CONFIG_T g_tgtMemdFlashCfg_s71pl064jb0 = TGT_MEMD_FLASH_CONFIG_S71PL064JB0;





PRIVATE UINT32 g_flash_bank_maks = (0x3<<22);

PRIVATE FLASH_NAME_ID g_flashID = FLASH_NAME_QTY;

BOOL  __attribute__((section(".ram"))) memd_FlashID_match(FLASH_AUTOSELECTOR_ID flash_ID)
{

    UINT8 i = 0;
    for (i = 0;i<FLASH_NAME_QTY;i++)
    {
        FLASH_DEBUG_HST(0x4444);
        FLASH_DEBUG_HST((UINT32)i);
        if(flash_ID.manufacturerID0 != g_flash_ID[i].manufacturerID0) continue;
        if(flash_ID.manufacturerID1 != g_flash_ID[i].manufacturerID1) continue;
        if(flash_ID.deviceID0 != g_flash_ID[i].deviceID0) continue;
        if(flash_ID.deviceID1 != g_flash_ID[i].deviceID1) continue;
        if(flash_ID.deviceID2 != g_flash_ID[i].deviceID2) continue;
        if(flash_ID.cfiBankNum == g_flash_ID[i].cfiBankNum)
        {
             FLASH_DEBUG_HST(0x2223);
            g_flashID = g_flash_ID[i].flashName;
            if (g_flashID == FLASH_EN71PL064B0)
            {
                g_memdFlashLayout = &g_memdFlashLayout_en71pl064b0;
                g_MemdRamCfg = &g_tgtMemdRamCfg_en71pl064b0;
                g_MemdFlashCfg = &g_tgtMemdFlashCfg_en71pl064b0;
                g_flash_bank_maks = 0x3<<20;
                
               
            }
            else if (g_flashID == FLASH_M36COW6050TOZSP)
            {
                g_memdFlashLayout = &g_memdFlashLayout_m36cow6050tozsp;
                g_MemdRamCfg = &g_tgtMemdRamCfg_m36cow6050tozsp;
                g_MemdFlashCfg = &g_tgtMemdFlashCfg_m36cow6050tozsp;
                FLASH_DEBUG_HST(0x98765432);

            }
            else if (g_flashID == FLASH_S71PL064JA0)// ram 2M
            {
                g_memdFlashLayout = &g_memdFlashLayout_en71pl064b0;
                g_MemdRamCfg = &g_tgtMemdRamCfg_s71pl064ja0;
                g_MemdFlashCfg = &g_tgtMemdFlashCfg_s71pl064ja0;
                FLASH_DEBUG_HST(0x987000);

            }
            else if (g_flashID == FLASH_S71PL064JB0)
            {
                g_memdFlashLayout = &g_memdFlashLayout_en71pl064b0;
                g_MemdRamCfg = &g_tgtMemdRamCfg_s71pl064jb0;
                g_MemdFlashCfg = &g_tgtMemdFlashCfg_s71pl064jb0;
                FLASH_DEBUG_HST(0x987002);

            }    
            hal_HstSendEvent((UINT32)g_flashID);
            return TRUE;
        }

    }
          return FALSE;

}


//VOID  __attribute__((section(".ram"))) memd_FlashRead_ID(VOID)
VOID memd_FlashRead_ID(VOID)
{
    VOLATILE UINT16 * BankAddr;
    VOLATILE UINT16 * ptr;
    UINT32 phys_add;
    UINT16 data =0,data1=0,data2=0 ;
    UINT32 status;
    FLASH_AUTOSELECTOR_ID flash_ID;
    FLASH_DEBUG_HST(0x58); 
    FLASH_DEBUG_HST(g_memdFlashBaseAddress);
    //another flash class
    flash_ID.deviceID0 = 0;
    flash_ID.deviceID1 = 0;
    flash_ID.deviceID2 = 0;
    flash_ID.manufacturerID0 = 0;
    flash_ID.manufacturerID1 = 0;
    flash_ID.cfiBankNum = 0;
    flash_ID.flashName = FLASH_NAME_QTY;
    phys_add = 0x0;
    BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&g_flash_bank_maks));
    ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&~1));
    FLASH_DEBUG_HST(0x157); 
    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x90;
    data = *(ptr);
   *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status);      FLASH_DEBUG_HST(0x159); 
    flash_ID.manufacturerID0 = data;
    FLASH_DEBUG_HST(0x158); 
    FLASH_DEBUG_HST(data);
    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x90;
    data = *(ptr+0x01);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.deviceID0= data;    FLASH_DEBUG_HST(data);
    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x55) = 0x98;
    data = *(ptr+0x2c);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.cfiBankNum = data;
    FLASH_DEBUG_HST(data);
    if (TRUE == memd_FlashID_match(flash_ID))
      return;
    FLASH_DEBUG_HST(0x59); 
    //another flash class fm91l03216ba
    flash_ID.deviceID0 = 0;
    flash_ID.deviceID1 = 0;
    flash_ID.deviceID2 = 0;
    flash_ID.manufacturerID0 = 0;
    flash_ID.manufacturerID1 = 0;
    flash_ID.cfiBankNum = 0;
    phys_add = 0x0;
    BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&g_flash_bank_maks));
    ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&~1));
    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x90;
    data = *(ptr+0x0);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.manufacturerID0 = data;
    FLASH_DEBUG_HST(data);

    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x90;
    data = *(ptr+0x01);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.deviceID0= data;    FLASH_DEBUG_HST(data);
    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x90;
    data = *(ptr+0x0e);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.deviceID1= data;    FLASH_DEBUG_HST(data);
        status = hal_SysEnterCriticalSection();
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x90;
    data = *(ptr+0x0f);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.deviceID2= data;    FLASH_DEBUG_HST(data);
    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x55) = 0x98;
    data = *(ptr+0x2c);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.cfiBankNum = data;FLASH_DEBUG_HST(data);
    if (TRUE == memd_FlashID_match(flash_ID))
      return;

    FLASH_DEBUG_HST(0x5a); 

    // another flash class  en71pl064b0
    
    phys_add = 0x0;
    BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&g_flash_bank_maks));
    ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&~1));
    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x90;
    data = *(ptr);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.manufacturerID0 = data;
    FLASH_DEBUG_HST(data);

    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x90;
    data = *(ptr+0x100);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.manufacturerID1 = data;
    FLASH_DEBUG_HST(data);

    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x90;
    data = *(ptr+0x01);
    data1 = *(ptr+0x0e);
    data2 = *(ptr+0x0f);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.deviceID0= data;    FLASH_DEBUG_HST(data);
    flash_ID.deviceID1= data1;    FLASH_DEBUG_HST(data1);
    flash_ID.deviceID2= data2;    FLASH_DEBUG_HST(data2);

    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x55) = 0x98;
    data = *(ptr+0x57);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.cfiBankNum = data;FLASH_DEBUG_HST(data);


    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x55) = 0x98;
    data = *(ptr+0x58);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    FLASH_DEBUG_HST(data);
    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x55) = 0x98;
    data = *(ptr+0x59);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    FLASH_DEBUG_HST(data);
    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x55) = 0x98;
    data = *(ptr+0x5a);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    FLASH_DEBUG_HST(data);
    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x55) = 0x98;
    data = *(ptr+0x5b);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    FLASH_DEBUG_HST(data);
    if (TRUE == memd_FlashID_match(flash_ID))
      return;
    FLASH_DEBUG_HST(0x5b); 

    // anohter flash class m36cow
    flash_ID.deviceID0 = 0;
    flash_ID.deviceID1 = 0;
    flash_ID.deviceID2 = 0;
    flash_ID.manufacturerID0 = 0;
    flash_ID.manufacturerID1 = 0;
    flash_ID.cfiBankNum = 0;
    phys_add = 0x0;
    BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&FLASHBANK_MASK));
    ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&~1));
    status = hal_SysEnterCriticalSection();
    *(BankAddr) = 0x90;
    data = *(ptr+0x0);
    *BankAddr = CMD_READ_ARRAY;
    hal_SysExitCriticalSection(status); 
    flash_ID.manufacturerID0 = data;
    status = hal_SysEnterCriticalSection();
    *(BankAddr) = 0x90;
    data = *(ptr+0x01);
    *BankAddr = CMD_READ_ARRAY;
    hal_SysExitCriticalSection(status); 
    flash_ID.deviceID0 = data;
    status = hal_SysEnterCriticalSection();
    *(BankAddr) = CMD_READ_CFI_QUERY;
    data = *(ptr+0x2c);
    *BankAddr = CMD_READ_ARRAY;
    hal_SysExitCriticalSection(status); 
    flash_ID.cfiBankNum = data;

    if (TRUE == memd_FlashID_match(flash_ID))
      return;
    //another flash class fm91l03216ba
    flash_ID.deviceID0 = 0;
    flash_ID.deviceID1 = 0;
    flash_ID.deviceID2 = 0;
    flash_ID.manufacturerID0 = 0;
    flash_ID.manufacturerID1 = 0;
    flash_ID.cfiBankNum = 0;
    phys_add = 0x0;
    BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&g_flash_bank_maks));
    ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&~1));
    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x90;
    data = *(ptr+0x04);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.manufacturerID0 = data;
    FLASH_DEBUG_HST(data);

    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x90;
    data = *(ptr);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.manufacturerID1= data;    FLASH_DEBUG_HST(data);
    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x555) = 0xaa;
    *(BankAddr+0x2aa) = 0x55;
    *(BankAddr+0x555) = 0x90;
    data = *(ptr+0x01);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.deviceID0= data;    FLASH_DEBUG_HST(data);

    status = hal_SysEnterCriticalSection();
    *(BankAddr+0x55) = 0x98;
    data = *(ptr+0x2c);
    *(BankAddr) = 0xf0;
    hal_SysExitCriticalSection(status); 
    flash_ID.cfiBankNum = data;FLASH_DEBUG_HST(data);
    if (TRUE == memd_FlashID_match(flash_ID))
      return;

    FLASH_DEBUG_HST(0x5a); 
     hal_DbgAssert("error flash id");

}
FLASH_NAME_ID memd_FlashGet_ID(VOID)
{
    return g_flashID;
}

#ifdef MEMD_RAM_IS_BURST
#define PSRAM_BCR 0x08590f

PRIVATE VOID HAL_FUNC_INTERNAL 
memd_RamConfigureBurstMode(REG16* csBase, REG32* pCRE)
{
    UINT16 read;

    // Using the CR controlled method 
    // Configure BCR
    // PSRAM_BCR[15] : 0 = synch / 1 = asynch
    // PSRAM_BCR[14] : 0 = variable Latency / 1 = Fixed latency
    // PSRAM_BCR[13:11] : Latency count 011(3)-111(7) clock cycle could be used 
    // PSRAM_BCR[10] : Wait Polarity 0 = Active Low / 1 = Active High
    // PSRAM_BCR[9] : MUST be set to 0
    // PSRAM_BCR[8] : Wait Config 0 = Asserted during wait / 1 = Asserted 1 cycle before wait
    // PSRAM_BCR[7:6] : MUST be set to 0   
    // PSRAM_BCR[5:4] : driver strength bits 0 = Full Drive / 1 = 1/2 Drive / 2 = 1/4 Drive
    // PSRAM_BCR[3] : 0 = Wrap / 1 = No wrap
    // PSRAM_BCR[2:0] : Burst len 001 = 4 words / 010 = 8 words / 011 = 16 words / 100 = 32 words / 111 Continuous
    read=*(csBase+PSRAM_BCR); 
    *pCRE=1; // MEM_FIFO_CRE
    *(csBase+PSRAM_BCR)=read; // read burst with latency = 3 fixed (4 clocks)
    
    {
        UINT32 flush __attribute__((unused)) = *pCRE;
    }
    
    *pCRE=0;

}
#endif //!MEMD_RAM_IS_BURST


// =============================================================================
// memd_FlashOpen
// -----------------------------------------------------------------------------
/// That function configures the flash.
/// It must be called before using any other function of the MEMD API.
///
/// @param cfg Pointer to the configuration to use to open the flash.
/// This parameter is ignored by the romulator driver.
/// @return Pointer towards the structure describing the flash.
// =============================================================================
PUBLIC MEMD_FLASH_LAYOUT_T* memd_FlashOpen(CONST MEMD_FLASH_CONFIG_T* cfg)
{
    MEMD_ASSERT(cfg != NULL, "Opening flash with a NULL configuration");
    // Open Flash, and store base address.
  //  g_memdFlashBaseAddress = (UINT32) hal_EbcFlashOpen(HAL_SYS_FREQ_39M,&cfg->csConfig, NULL);
    memd_FlashRead_ID();

    g_memdFlashBaseAddress = (UINT32) hal_EbcFlashOpen(HAL_SYS_FREQ_39M,&(g_MemdFlashCfg->csConfig), NULL);
    FLASH_DEBUG_HST(0x8234);

    return (MEMD_FLASH_LAYOUT_T*)g_memdFlashLayout;
}


// =============================================================================
// memd_RamOpen
// -----------------------------------------------------------------------------
/// That function configures the RAM.
/// It must be called to configure the best settings for the RAM, thus enabling
/// the fatest access.
/// @param cfg Ram configuration used to open the RAM.
///
// =============================================================================
PUBLIC VOID memd_RamOpen(CONST MEMD_RAM_CONFIG_T* cfg)
{
    MEMD_ASSERT(cfg != NULL, "Opening RAM with a NULL configuration");
    memd_FlashRead_ID();
    // Open External SRAM
    //g_MemdRamCfg = &g_tgtMemdRamCfg_en71gl032a0;
    hal_EbcCsOpen(HAL_EBC_SRAM, &((*g_MemdRamCfg).csConfig));
    FLASH_DEBUG_HST(0x9234);

}




PRIVATE UINT32 memd_FlashGetSectorLimits(UINT32 sectorId, UINT32 *start, UINT32 *end)
{
    UINT32 aStart, aEnd;
    int bankNum = 0;
    int majSecIdx, minSecIdx;
    UINT32 sectSize;

    if (sectorId == g_memdFlashLayout->fullSize)
    {
        *start = g_memdFlashLayout->fullSize;
        *end = g_memdFlashLayout->fullSize;
        return 0;
    }
    else if (sectorId > g_memdFlashLayout->fullSize)
    {
        *start = 0;
        *end = 0;
        MEMD_ASSERT(FALSE, "Out of accessible flash space !!!!");
        return 0;
    }
    else
    {
        while ( (sectorId >= g_memdFlashLayout->bankLayout[bankNum+1].bankStartAddr) &&
                (bankNum < (g_memdFlashLayout->numberOfBank-1)) )
        {
            ++bankNum;
        }
        aStart = g_memdFlashLayout->bankLayout[bankNum].bankStartAddr;

        majSecIdx = 0;
        aEnd = aStart +
            (g_memdFlashLayout->bankLayout[bankNum].sectorLayoutTable[majSecIdx][0] *
            g_memdFlashLayout->bankLayout[bankNum].sectorLayoutTable[majSecIdx][1]);
        while (sectorId >= aEnd)
        {
            if (majSecIdx == 2)
            {
                *start = 0;
                *end = 0;
                MEMD_ASSERT(FALSE, "Out of accessible flash space !!!!");
                return 0;
            } else
            {
                ++majSecIdx;
                aStart = aEnd;
                aEnd = aStart +
                    (g_memdFlashLayout->bankLayout[bankNum].sectorLayoutTable[majSecIdx][0] *
                    g_memdFlashLayout->bankLayout[bankNum].sectorLayoutTable[majSecIdx][1]);
            }
        }

        minSecIdx = 0;
        sectSize = g_memdFlashLayout->bankLayout[bankNum].sectorLayoutTable[majSecIdx][1];
        aEnd = aStart + sectSize;
        while (sectorId >= aEnd)
        {
            aStart = aEnd;
            aEnd = aStart + sectSize;
            ++minSecIdx;
        }

        *start = aStart;
        *end = aEnd;
        return sectSize;
    }
}

PRIVATE MEMD_ERR_T memd_BlockLock(UINT8 *phy_startFlshAddr, UINT8 *phy_endFlshAddr, BOOL Lock)
{
    UINT32 phy_Start;
    UINT32 phy_End;
    UINT32 status;
    VOLATILE UINT16 * CurBankAddr;
    VOLATILE UINT16 * PrevBankAddr;

    // Check that start & end addresses are aligned
    if (phy_endFlshAddr == NULL)
    {
        memd_FlashGetSectorLimits((UINT32)phy_startFlshAddr, &phy_Start, &phy_End);
        phy_endFlshAddr = (UINT8*)phy_End;
    } else
    {
        if ((UINT32)phy_endFlshAddr != FLASH_SIZE) {
            memd_FlashGetSectorLimits((UINT32)phy_endFlshAddr , &phy_Start, &phy_End);
            if ((UINT32)phy_endFlshAddr != phy_Start) {
                return MEMD_ERR_ALIGN;
            }
        }
    }

    memd_FlashGetSectorLimits((UINT32)phy_startFlshAddr, &phy_Start, &phy_End);
    if ((UINT32)phy_startFlshAddr != phy_Start)
    {
        return MEMD_ERR_ALIGN;
    }

    PrevBankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phy_Start&FLASHBANK_MASK));

    while (phy_Start != (UINT32)phy_endFlshAddr)
    {
        VOLATILE UINT16 * SectAddr;
        SectAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + phy_Start);
        CurBankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phy_Start&FLASHBANK_MASK));
        status = hal_SysEnterCriticalSection();
        // If Bank Address is the same as previous Bank Address, continue
        // otherwise, need to put old Bank into Read Array mode and
        // open new Bank.
        if (CurBankAddr != PrevBankAddr)
        {
            // Close Prev Bank
            *PrevBankAddr = CMD_READ_ARRAY;
            PrevBankAddr = CurBankAddr;
        }

        // Lock Set/Clr
        if (Lock)
        {
            *SectAddr = CMD_BLOCK_PROT;
            *SectAddr = CMD_BLOCK_LOCK;
        }
        else
        {
            *SectAddr = CMD_BLOCK_PROT;
            *SectAddr = CMD_BLOCK_UNLOCK;
        }
        hal_SysExitCriticalSection(status);

        if (phy_End != FLASH_SIZE) {
            memd_FlashGetSectorLimits(phy_End, &phy_Start, &phy_End);
        } else
        {
            phy_Start = phy_End;
        }
    }

    // Return to Read Array mode
    status = hal_SysEnterCriticalSection();
    *PrevBankAddr = CMD_READ_ARRAY;
    hal_SysExitCriticalSection(status);

    return MEMD_ERR_NO;
}

PUBLIC HAL_FUNC_INTERNAL MEMD_ERR_T memd_FlashErase_First(UINT8 *startFlashAddress, UINT8 *endFlashAddress,UINT32 time)
{
    UINT8 rdstatus, rdstatus_old;
    UINT32 status;
    VOLATILE UINT8 * ptr;
    VOLATILE UINT16 *flashaddr,*ptr1;
    VOLATILE UINT16 *BankAddr;
    UINT32 Start, End;
    UINT32 phys_end_add, phys_start_add;
    UINT32 suspend_time = 0,max_time = 0;
    UINT32 phy_Start;
    UINT32 phy_End;
    BOOL isLocked;
    MEMD_ERR_T errorcode=MEMD_ERR_NO;    
    max_time = time MILLI_SECONDS;
    FLASH_DEBUG_HST(0x55558);
    UINT32 now = hal_TimGetUpTime();
    if ( g_flashID == FLASH_EN71PL064B0 ||g_flashID == FLASH_S71PL064JB0)
    {
        phys_start_add = (UINT32)startFlashAddress;
      //  FLASH_DEBUG_HST(phys_start_add);
        MEMD_ASSERT((phys_start_add & 0xe0000000) == 0, 
            "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");
    
        // Check that start & end addresses are aligned
        if (endFlashAddress == NULL)
        {
            memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
            phys_end_add = End;
        } else
        {
            phys_end_add = (UINT32)endFlashAddress;
            MEMD_ASSERT((phys_end_add & 0xe0000000) == 0, 
                "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")
            if (phys_end_add != FLASH_SIZE)
            {
                memd_FlashGetSectorLimits(phys_end_add , &Start, &End);
                if (phys_end_add != Start)
                {
                    return MEMD_ERR_ALIGN;
                }
            }
        }
    
        memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
        if (phys_start_add != Start)
        {
            return MEMD_ERR_ALIGN;
        }
    
        hal_EbcFlashWriteEnable(TRUE);
    
        while (Start != phys_end_add)
        {
            flashaddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (Start&FLASHBANK_MASK));
            ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + (Start&~0xfff));
            status = hal_SysEnterCriticalSection();
            // Sector Erase command
            *(flashaddr+0x555) = 0xaa;
            *(flashaddr+0x2aa) = 0x55;
            *(flashaddr+0x555) = 0x80;
            *(flashaddr+0x555) = 0xaa;
            *(flashaddr+0x2aa) = 0x55;
            *(ptr) = 0x30;
            hal_SysExitCriticalSection(status);
    
            // note the pooling could be done on data == 0xff also
        
            // pooling
            rdstatus = (*ptr);
            while(1){
    
                    suspend_time = hal_TimGetUpTime();
                     if (suspend_time - now > max_time) 
                      {
                          status = hal_SysEnterCriticalSection();
                          *(ptr) = 0xb0;
                          hal_SysExitCriticalSection(status);
                          do
                          {
                             rdstatus_old = (*ptr);
                             rdstatus = (*ptr);
                             rdstatus_old &= 0xc0;
                             rdstatus &= 0xc0;
                            
                          }while (!(rdstatus_old == 0xC0 && rdstatus == 0xC0));	// Wait for suspend active
                          (*flashaddr) = 0xf0;
                          hal_EbcFlashWriteEnable(FALSE);
                          return MEMD_ERR_SUSPEND;
                      }
             
                rdstatus_old = rdstatus;
                rdstatus = (*ptr);
                // DQ6 & DQ2 not toggling? => done
                if (((rdstatus ^ rdstatus_old) & 0x44) == 0)
                break;
                // DQ5 = 1
                if ((rdstatus & 0x20) == 0x20) {
                    rdstatus_old = (*ptr);
                    rdstatus = (*ptr);
                    // DQ6 & DQ2 not toggling? => done
                    if (((rdstatus ^ rdstatus_old) & 0x44) == 0)
                        break;
                    // reset
                    (*flashaddr) = 0xf0;
                    return MEMD_ERR_ERASE;
                }
            }
            if (End != FLASH_SIZE)
            {
                memd_FlashGetSectorLimits(End, &Start, &End);
            } else
            {
                Start = End;
            }
        }
        // erase done
        hal_EbcFlashWriteEnable(FALSE);
        return MEMD_ERR_NO;
     }
    else  if (g_flashID == FLASH_M36COW6050TOZSP)
    {
        phys_start_add = (UINT32)startFlashAddress;
        max_time = time MILLI_SECONDS;
        MEMD_ASSERT((phys_start_add & 0xe0000000) == 0, 
            "flash_address is expected to be a byte offset within the flash chip, not an absolute address");
        // Check that start & end addresses are aligned
        if (endFlashAddress == NULL)
        {
            memd_FlashGetSectorLimits(phys_start_add, &phy_Start, &phy_End);
            phys_end_add = phy_End;
        } else
        {
            phys_end_add = (UINT32)endFlashAddress;
            MEMD_ASSERT((phys_end_add & 0xe0000000) == 0, 
                "flash_address is expected to be a byte offset within the flash chip, not an absolute address")
            if (phys_end_add != FLASH_SIZE)
            {
                memd_FlashGetSectorLimits(phys_end_add , &phy_Start, &phy_End);
                if (phys_end_add != phy_Start)
                {
                    return MEMD_ERR_ALIGN;
                }
            }
        }
    
        memd_FlashGetSectorLimits(phys_start_add, &phy_Start, &phy_End);
        if (phys_start_add != phy_Start)
        {
            return MEMD_ERR_ALIGN;
        }
        hal_EbcFlashWriteEnable(TRUE);
    
        BankAddr = NULL;
        while (phy_Start != phys_end_add)
        {
            BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phy_Start&FLASHBANK_MASK));
            // phy_Start should already be aligned to sector boundary, so shouldn't need any more masking
            ptr1 = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + phy_Start);
          
            memd_FlashGetLockStatus((UINT8*)((UINT32)ptr1 & MY_MASK),&isLocked);
    	 // Re-enable EBC write
            hal_EbcFlashWriteEnable(TRUE);
    
            if(isLocked == TRUE)
            {
               memd_BlockLock((UINT8*)((UINT32)ptr1 & MY_MASK),NULL,FALSE);
            }
      
            status = hal_SysEnterCriticalSection();		
            // Sector Erase command
            *(BankAddr) = CMD_BLOCK_ERASE_1;
            *(ptr1) = CMD_BLOCK_ERASE_2;
    
            //*(BankAddr) = CMD_ERASE_SUSPEND;
            hal_SysExitCriticalSection(status);
    
    
            // pooling
            // Wait for Ready, then check status
            do{
    
                    suspend_time = hal_TimGetUpTime();
                     if (suspend_time - now > max_time) 
                     {
                                status = hal_SysEnterCriticalSection();		
                                *(BankAddr) = CMD_ERASE_SUSPEND;
                                *(BankAddr) = CMD_READ_STATUS;
                                do{
                                 rdstatus = *BankAddr;
               
                                }while((rdstatus & SR7) != SR7);
                               if((rdstatus & SR6) == SR6)
                                {
               
                                  *BankAddr = CMD_READ_ARRAY;
                                  hal_SysExitCriticalSection(status);
                                  return MEMD_ERR_NO;
                                }
                                else
                                {
                                  *BankAddr = CMD_READ_ARRAY;
                                   hal_SysExitCriticalSection(status);
    
                                  return MEMD_ERR_NO;
    
                                }
    
                     }
                     
    
    
              
                 rdstatus = *BankAddr;
            }while((rdstatus & SR7) != SR7);   // Any address in the bank may be used
    
            // Should probably fix this scheme for reporting errors more cleanly.
            // For now, just prioritize the errors with the most significant error returned
            // in errorcode (order this list from least to most significant)
            if ((rdstatus & SR3) != 0)
            {
              	 
                // Vpp Invalid Error
                errorcode=MEMD_ERR_ERASE;
            }
            else if (((rdstatus & SR5) != 0) && ((rdstatus & SR4) != 0))
            {
                // Command Sequence Error
                errorcode=MEMD_ERR_ERASE; while(1){;};
            }
            else if ((rdstatus & SR5) != 0)
            {
                // Erase Error
                errorcode=MEMD_ERR_ERASE; while(1){;};
            }
            else if ((rdstatus & SR1) != 0)
            {
                errorcode=MEMD_ERR_PROTECT; while(1){;};
            }
            
            // Reset to read array mode when every block erase operation is finished.
    	       status = hal_SysEnterCriticalSection();
            *BankAddr = CMD_READ_ARRAY;
             hal_SysExitCriticalSection(status);
    
    
    	// Clear status register if any error 
    	if(errorcode != MEMD_ERR_NO)
    	{
       // while(1){;};
    	            status = hal_SysEnterCriticalSection();
                *BankAddr = CMD_CLR_STATUS_REG;
                hal_SysExitCriticalSection(status);
    	           hal_EbcFlashWriteEnable(FALSE);
                return errorcode;
    	}
    
            if (phy_End != FLASH_SIZE)
            {
                memd_FlashGetSectorLimits(phy_End, &phy_Start, &phy_End);
            } else
            {
                phy_Start = phy_End;
            }
        }
        if (BankAddr != NULL)
        {
            // Return to Read Array mode
            status = hal_SysEnterCriticalSection();
            *BankAddr = CMD_READ_ARRAY;
            hal_SysExitCriticalSection(status);
        }
    
        // erase done
         hal_EbcFlashWriteEnable(FALSE);
    
        return errorcode;
    }
    return MEMD_ERR_NO;
}
PUBLIC HAL_FUNC_INTERNAL MEMD_ERR_T memd_FlashErase_Continue(UINT8 *startFlashAddress, UINT8 *endFlashAddress,UINT32 time)
{
    UINT8 rdstatus, rdstatus_old;
    UINT32 status;
    VOLATILE UINT8 * ptr;
    VOLATILE UINT16 *flashaddr,*ptr1;
    VOLATILE UINT16 *BankAddr;
    UINT32 Start, End;
    UINT32 phy_Start;
    UINT32 phy_End;
    UINT32 phys_end_add, phys_start_add;
    UINT32 suspend_time = 0,max_time = 0;
    BOOL isLocked;
    MEMD_ERR_T errorcode=MEMD_ERR_NO;
    max_time = time MILLI_SECONDS;
    FLASH_DEBUG_HST(0x55557);
    UINT32 now = hal_TimGetUpTime();
    if (g_flashID == FLASH_EN71PL064B0 ||g_flashID == FLASH_S71PL064JB0)
    {
        phys_start_add = (UINT32)startFlashAddress;
    
        MEMD_ASSERT((phys_start_add & 0xe0000000) == 0, 
            "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");
    
        // Check that start & end addresses are aligned
        if (endFlashAddress == NULL)
        {
            memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
            phys_end_add = End;
        } else
        {
            phys_end_add = (UINT32)endFlashAddress;
            MEMD_ASSERT((phys_end_add & 0xe0000000) == 0, 
                "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")
            if (phys_end_add != FLASH_SIZE)
            {
                memd_FlashGetSectorLimits(phys_end_add , &Start, &End);
                if (phys_end_add != Start)
                {
                    return MEMD_ERR_ALIGN;
                }
            }
        }
    
        memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
        if (phys_start_add != Start)
        {
            return MEMD_ERR_ALIGN;
        }
    
        hal_EbcFlashWriteEnable(TRUE);
    
        while (Start != phys_end_add)
        {
            flashaddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (Start&FLASHBANK_MASK));
            ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + (Start&~0xfff));
            status = hal_SysEnterCriticalSection();
            // Sector Erase command
    
            *(ptr) = 0x30;
            hal_SysExitCriticalSection(status);
            // note the pooling could be done on data == 0xff also
            // pooling
            rdstatus = (*ptr);
            while(1){
    
                    suspend_time = hal_TimGetUpTime();
                     if (suspend_time - now > max_time) 
                      {
                          status = hal_SysEnterCriticalSection();
                          *(ptr) = 0xb0;
                          hal_SysExitCriticalSection(status);
                          do
                          {
                             rdstatus_old = (*ptr);
                             rdstatus = (*ptr);
                             rdstatus_old &= 0xc0;
                             rdstatus &= 0xc0;
                            
                          }while (!(rdstatus_old == 0xc0 && rdstatus == 0xc0));	// Wait for suspend active
                          (*flashaddr) = 0xf0;
                          hal_EbcFlashWriteEnable(FALSE);
                          return MEMD_ERR_SUSPEND;
                      }
             
                rdstatus_old = rdstatus;
                rdstatus = (*ptr);
                // DQ6 & DQ2 not toggling? => done
                if (((rdstatus ^ rdstatus_old) & 0x44) == 0)
                break;
                // DQ5 = 1
                if ((rdstatus & 0x20) == 0x20) {
                    rdstatus_old = (*ptr);
                    rdstatus = (*ptr);
                    // DQ6 & DQ2 not toggling? => done
                    if (((rdstatus ^ rdstatus_old) & 0x44) == 0)
                        break;
                    // reset
                    (*flashaddr) = 0xf0;
                    hal_EbcFlashWriteEnable(FALSE);
                    return MEMD_ERR_ERASE;
                }
            }
            if (End != FLASH_SIZE)
            {
                memd_FlashGetSectorLimits(End, &Start, &End);
            } else
            {
                Start = End;
            }
        }
        // erase done
        hal_EbcFlashWriteEnable(FALSE);
        return MEMD_ERR_NO;
     }
    else  if (g_flashID == FLASH_M36COW6050TOZSP)
    {
        phys_start_add = (UINT32)startFlashAddress;
    
        MEMD_ASSERT((phys_start_add & 0xe0000000) == 0, 
            "flash_address is expected to be a byte offset within the flash chip, not an absolute address");
        // Check that start & end addresses are aligned
        if (endFlashAddress == NULL)
        {
            memd_FlashGetSectorLimits(phys_start_add, &phy_Start, &phy_End);
            phys_end_add = phy_End;
        } else
        {
            phys_end_add = (UINT32)endFlashAddress;
            MEMD_ASSERT((phys_end_add & 0xe0000000) == 0, 
                "flash_address is expected to be a byte offset within the flash chip, not an absolute address")
            if (phys_end_add != FLASH_SIZE)
            {
                memd_FlashGetSectorLimits(phys_end_add , &phy_Start, &phy_End);
                if (phys_end_add != phy_Start)
                {
                    return MEMD_ERR_ALIGN;
                }
            }
        }
    
        memd_FlashGetSectorLimits(phys_start_add, &phy_Start, &phy_End);
        if (phys_start_add != phy_Start)
        {
            return MEMD_ERR_ALIGN;
        }
    
        hal_EbcFlashWriteEnable(TRUE);
    
        BankAddr = NULL;
        while (phy_Start != phys_end_add)
        {
            BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phy_Start&FLASHBANK_MASK));
            // phy_Start should already be aligned to sector boundary, so shouldn't need any more masking
            ptr1 = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + phy_Start);
    
           
            memd_FlashGetLockStatus((UINT8*)((UINT32)ptr1 & MY_MASK),&isLocked);
    	 // Re-enable EBC write
            hal_EbcFlashWriteEnable(TRUE);
    
    
            if(isLocked == TRUE)
            {
               memd_BlockLock((UINT8*)((UINT32)ptr1 & MY_MASK),NULL,FALSE);
               	  
            }
      
            status = hal_SysEnterCriticalSection();		
            // Sector Erase command
            *(BankAddr) = CMD_EEASE_RESUME;
    
            //*(BankAddr) = CMD_ERASE_SUSPEND;
            hal_SysExitCriticalSection(status);
    
    
            // pooling
            // Wait for Ready, then check status
            do{
    
                     suspend_time =  hal_TimGetUpTime();
                     if (suspend_time - now > max_time) 
                     {
             
                                status = hal_SysEnterCriticalSection();		
                                *(BankAddr) = CMD_ERASE_SUSPEND;
                                *(BankAddr) = CMD_READ_STATUS;
                                do{
                                 rdstatus = *BankAddr;
               
                                }while((rdstatus & SR7) != SR7);
                                
                                 rdstatus = *BankAddr;
               
                                if((rdstatus & SR6) == SR6)
                                {
               
                                  *BankAddr = CMD_READ_ARRAY;
                                  hal_SysExitCriticalSection(status);
                                  hal_EbcFlashWriteEnable(FALSE);
                                  return MEMD_ERR_ERASE;
                                }
                                else
                                {
                                  *BankAddr = CMD_READ_ARRAY;
                                   hal_SysExitCriticalSection(status);
                                  hal_EbcFlashWriteEnable(FALSE);
                                  return MEMD_ERR_NO;
    
                                }
    
                     }
                     
         
                 rdstatus = *BankAddr;
            }while((rdstatus & SR7) != SR7);   // Any address in the bank may be used
    
            // Should probably fix this scheme for reporting errors more cleanly.
            // For now, just prioritize the errors with the most significant error returned
            // in errorcode (order this list from least to most significant)
            if ((rdstatus & SR3) != 0)
            {
              	 
                // Vpp Invalid Error
                errorcode=MEMD_ERR_ERASE;
            }
            else if (((rdstatus & SR5) != 0) && ((rdstatus & SR4) != 0))
            {
                // Command Sequence Error
                errorcode=MEMD_ERR_ERASE; while(1){;};
            }
            else if ((rdstatus & SR5) != 0)
            {
                // Erase Error
                errorcode=MEMD_ERR_ERASE; while(1){;};
            }
            else if ((rdstatus & SR1) != 0)
            {
                errorcode=MEMD_ERR_PROTECT; while(1){;};
            }
            
            // Reset to read array mode when every block erase operation is finished.
    	      status = hal_SysEnterCriticalSection();
            *BankAddr = CMD_READ_ARRAY;
            hal_SysExitCriticalSection(status);
    
          	// Clear status register if any error 
          	if(errorcode != MEMD_ERR_NO)
          	{
          	    status = hal_SysEnterCriticalSection();
                *BankAddr = CMD_CLR_STATUS_REG;
                hal_SysExitCriticalSection(status);
          	    hal_EbcFlashWriteEnable(FALSE);
                return errorcode;
          	}
          
            if (phy_End != FLASH_SIZE)
            {
                memd_FlashGetSectorLimits(phy_End, &phy_Start, &phy_End);
            } else
            {
                phy_Start = phy_End;
            }
        }
        if (BankAddr != NULL)
        {
            // Return to Read Array mode
            status = hal_SysEnterCriticalSection();
            *BankAddr = CMD_READ_ARRAY;
            hal_SysExitCriticalSection(status);
        }
    
        // erase done
        hal_EbcFlashWriteEnable(FALSE);
    
        return errorcode;
    }
    return MEMD_ERR_NO;
}

// =============================================================================
// memd_FlashWrite
// -----------------------------------------------------------------------------
/// This function writes data in the flash. It gets its data from \c buffer, copies 
/// \c byteSize bytes to the flash location designed by \c flashAddress. \c 
/// pWrittenbyteSize is filled with the actual number of written bytes (Equal 
/// @param  flashAddress The byte offset within the flash chip. (Take care not 
/// to overwrite the code present at the beginning of the flash)
/// @param  byteSize Number of bytes to write in flash
/// @param  pWrittenbyteSize Number of bytes actually written in flash
/// @param  buffer Buffer where to get the data to write in the flash
/// @return #MEMD_ERR_NO, #MEMD_ERR_WRITE or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed.
// =============================================================================

PUBLIC BOOL RamRun_FlashErase(UINT8 *startFlashAddress)
{
 
   #ifdef RAMRUN_BURN_FLASH
   INT16 bank_num = 0;
   UINT16 i = 0,k = 0,j =0;
   UINT32 add_flash = FLASH_SIZE;
   FLASH_DEBUG_HST(0x555560);
   for(bank_num = g_memdFlashLayout->numberOfBank -1; bank_num >=0  ;bank_num --)
   {
      FLASH_DEBUG_HST(0x4224);
      FLASH_DEBUG_HST((UINT32)bank_num);
       for (i = 3;i >0;i--)
       {   j=i-1;
           for (k = 0;k < g_memdFlashLayout->bankLayout[bank_num].sectorLayoutTable[j][0] ;k++)
           {
               add_flash -= g_memdFlashLayout->bankLayout[bank_num].sectorLayoutTable[j][1];
               if ((UINT8 *)add_flash == (startFlashAddress+1024*0))
               {
                 FLASH_DEBUG_HST(0x555577);
                 FLASH_DEBUG_HST(add_flash);
                 return TRUE;
               }
               
           }
  
       }
  
   }
   FLASH_DEBUG_HST(0x555588);
   return FALSE;
  #else
  FLASH_DEBUG_HST(0x555589);
  return TRUE;
 #endif


}

PUBLIC MEMD_ERR_T memd_FlashErase(UINT8 *startFlashAddress, UINT8 *endFlashAddress)
{
    UINT8 rdstatus, rdstatus_old;
    UINT32 status;
    VOLATILE UINT8 * ptr;
    VOLATILE UINT16 *BankAddr,*ptr1;
    UINT32 Start, End,phy_Start,phy_End;
    UINT32 phys_end_add, phys_start_add;
    MEMD_ERR_T errorcode=MEMD_ERR_NO;
    BOOL isLocked;
    FLASH_DEBUG_HST(0x422000);
    FLASH_DEBUG_HST((UINT32)startFlashAddress);;
    if ( FALSE == RamRun_FlashErase(startFlashAddress)) 
    {
      return MEMD_ERR_NO;
    }
    if (g_flashID == FLASH_EN71PL064B0 ||g_flashID == FLASH_S71PL064JB0)
    {
        FLASH_DEBUG_HST(0x423000);
        phys_start_add = (UINT32)startFlashAddress;
        MEMD_ASSERT((phys_start_add & 0xe0000000) == 0, 
            "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");
    
        // Check that start & end addresses are aligned
        if (endFlashAddress == NULL)
        {
            memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
            phys_end_add = End;
        } else
        {
            phys_end_add = (UINT32)endFlashAddress;
            MEMD_ASSERT((phys_end_add & 0xe0000000) == 0, 
                "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")
            if (phys_end_add != FLASH_SIZE)
            {
                memd_FlashGetSectorLimits(phys_end_add , &Start, &End);
                if (phys_end_add != Start)
                {
                    return MEMD_ERR_ALIGN;
                }
            }
        }
    
        memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
        if (phys_start_add != Start)
        {
            return MEMD_ERR_ALIGN;
        }
    
        hal_EbcFlashWriteEnable(TRUE);
    
        while (Start != phys_end_add)
        {
           
            BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (Start&g_flash_bank_maks));
                // Start should already be aligned to sector boundary, so shouldn't need any more masking
            ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + Start);
            status = hal_SysEnterCriticalSection();
            // Sector Erase command
            *(BankAddr+0x555) = 0xaa;
            *(BankAddr+0x2aa) = 0x55;
            *(BankAddr+0x555) = 0x80;
            *(BankAddr+0x555) = 0xaa;
            *(BankAddr+0x2aa) = 0x55;
            *(ptr) = 0x30;
            hal_SysExitCriticalSection(status);
            // note the pooling could be done on data == 0xff also
            // pooling
            rdstatus = (*ptr);
            while(1){
                rdstatus_old = rdstatus;
                rdstatus = (*ptr);
                // DQ6 & DQ2 not toggling? => done
                if (((rdstatus ^ rdstatus_old) & 0x44) == 0)
                break;
                // DQ5 = 1
                if ((rdstatus & 0x20) == 0x20) {
                    rdstatus_old = (*ptr);
                    rdstatus = (*ptr);
                    // DQ6 & DQ2 not toggling? => done
                    if (((rdstatus ^ rdstatus_old) & 0x44) == 0)
                        break;
                    // reset
                    (*BankAddr) = 0xf0;
                    hal_EbcFlashWriteEnable(FALSE);
                    return MEMD_ERR_ERASE;
                }
            }
            if (End != FLASH_SIZE)
            {
                memd_FlashGetSectorLimits(End, &Start, &End);
            } else
            {
                Start = End;
            }
        }
        // erase done
        hal_EbcFlashWriteEnable(FALSE);
        return MEMD_ERR_NO;
    
    }
    else  if (g_flashID == FLASH_M36COW6050TOZSP)
    {
        FLASH_DEBUG_HST(0x422008);
       //return MEMD_ERR_NO;
      
        phys_start_add = (UINT32)startFlashAddress;
        MEMD_ASSERT((phys_start_add & 0xe0000000) == 0, 
            "flash_address is expected to be a byte offset within the flash chip, not an absolute address");
        // Check that start & end addresses are aligned
        if (endFlashAddress == NULL)
        {
            memd_FlashGetSectorLimits(phys_start_add, &phy_Start, &phy_End);
            phys_end_add = phy_End;
        } else
        {
            phys_end_add = (UINT32)endFlashAddress;
            MEMD_ASSERT((phys_end_add & 0xe0000000) == 0, 
                "flash_address is expected to be a byte offset within the flash chip, not an absolute address")
            if (phys_end_add != FLASH_SIZE)
            {
                memd_FlashGetSectorLimits(phys_end_add , &phy_Start, &phy_End);
                if (phys_end_add != phy_Start)
                {
                    return MEMD_ERR_ALIGN;
                }
            }
        }
    
        memd_FlashGetSectorLimits(phys_start_add, &phy_Start, &phy_End);
        if (phys_start_add != phy_Start)
        {
            return MEMD_ERR_ALIGN;
        }
    
        hal_EbcFlashWriteEnable(TRUE);
    
        BankAddr = NULL;
        while (phy_Start != phys_end_add)
        {
            BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phy_Start&FLASHBANK_MASK));
            // phy_Start should already be aligned to sector boundary, so shouldn't need any more masking
            ptr1 = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + phy_Start);
    
           
            memd_FlashGetLockStatus((UINT8*)((UINT32)ptr1 & MY_MASK),&isLocked);
    	 // Re-enable EBC write
            hal_EbcFlashWriteEnable(TRUE);
    
    
            if(isLocked == TRUE)
            {
               memd_BlockLock((UINT8*)((UINT32)ptr1 & MY_MASK),NULL,FALSE);
               	 // while(1){;};
            }
      
            status = hal_SysEnterCriticalSection();		
            // Sector Erase command
            *(BankAddr) = CMD_BLOCK_ERASE_1;
            *(ptr1) = CMD_BLOCK_ERASE_2;
            hal_SysExitCriticalSection(status);
    
    
            // pooling
            // Wait for Ready, then check status
            do{
                 rdstatus = *BankAddr;
            }while((rdstatus & SR7) != SR7);   // Any address in the bank may be used
    
            // Should probably fix this scheme for reporting errors more cleanly.
            // For now, just prioritize the errors with the most significant error returned
            // in errorcode (order this list from least to most significant)
            if ((rdstatus & SR3) != 0)
            {
              	  while(1){;};
                // Vpp Invalid Error
                errorcode=MEMD_ERR_ERASE;
            }
            else if (((rdstatus & SR5) != 0) && ((rdstatus & SR4) != 0))
            {
                // Command Sequence Error
                errorcode=MEMD_ERR_ERASE; while(1){;};
            }
            else if ((rdstatus & SR5) != 0)
            {
                // Erase Error
                errorcode=MEMD_ERR_ERASE; while(1){;};
            }
            else if ((rdstatus & SR1) != 0)
            {
                errorcode=MEMD_ERR_PROTECT; while(1){;};
            }
            
            // Reset to read array mode when every block erase operation is finished.
          	status = hal_SysEnterCriticalSection();
            *BankAddr = CMD_READ_ARRAY;
            hal_SysExitCriticalSection(status);
    
    	// Clear status register if any error 
         	if(errorcode != MEMD_ERR_NO)
         	{
               status = hal_SysEnterCriticalSection();
               *BankAddr = CMD_CLR_STATUS_REG;
               hal_SysExitCriticalSection(status);
               hal_EbcFlashWriteEnable(FALSE);
               return errorcode;
         	}
         
           if (phy_End != FLASH_SIZE)
           {
               memd_FlashGetSectorLimits(phy_End, &phy_Start, &phy_End);
           } else
           {
               phy_Start = phy_End;
           }
        }
        if (BankAddr != NULL)
        {
            // Return to Read Array mode
            status = hal_SysEnterCriticalSection();
            *BankAddr = CMD_READ_ARRAY;
            hal_SysExitCriticalSection(status);
        }
    
        // erase done
        hal_EbcFlashWriteEnable(FALSE);
        return errorcode;
    }
    return MEMD_ERR_NO;
}


// =============================================================================
// memd_FlashWrite
// -----------------------------------------------------------------------------
/// This function writes data in the flash. It gets its data from \c buffer, copies 
/// \c byteSize bytes to the flash location designed by \c flashAddress. \c 
/// pWrittenbyteSize is filled with the actual number of written bytes (Equal 
/// to size, or less in case of error).
/// 
/// @param  flashAddress The byte offset within the flash chip. (Take care not 
/// to overwrite the code present at the beginning of the flash)
/// @param  byteSize Number of bytes to write in flash
/// @param  pWrittenbyteSize Number of bytes actually written in flash
/// @param  buffer Buffer where to get the data to write in the flash
/// @return #MEMD_ERR_NO, #MEMD_ERR_WRITE or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed.
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashWrite(UINT8 *flashAddress,
        UINT32 byteSize, 
        UINT32 * pWrittenbyteSize, 
        CONST UINT8* buffer)
{
    VOLATILE UINT16 * BankAddr;
    VOLATILE UINT16 * ptr;
    UINT16 rdstatus, data;
    UINT32 bsize=0, wsize=0, owsize=0;
    MEMD_ERR_T errorcode=MEMD_ERR_NO;
    UINT32 status;
    UINT32 phys_add,phy_SectAddr,phy_End;
    BOOL isLocked;

     FLASH_DEBUG_HST(0x2225);
     //return MEMD_ERR_NO;
 //     FLASH_DEBUG_HST((UINT32)byteSize);

    if (g_flashID == FLASH_EN71PL064B0 ||g_flashID == FLASH_S71PL064JB0)
    { 
      FLASH_DEBUG_HST(0x2226);
        *pWrittenbyteSize=0;
        if (byteSize==0)
        {
            return MEMD_ERR_NO;
        }
        hal_EbcFlashWriteEnable(TRUE);
    
        phys_add = (UINT32)flashAddress;
        MEMD_ASSERT((phys_add & 0xe0000000) == 0,
            "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")
    
        BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&g_flash_bank_maks));
        // Start should already be aligned to sector boundary, so shouldn't need any more masking
        ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&~1));

        // Unlock Bypass Entry
        status = hal_SysEnterCriticalSection();
        *(BankAddr+0x555) = 0xaa;
        *(BankAddr+0x2aa) = 0x55;
        *(BankAddr+0x555) = 0x20;
       // *(BankAddr+0x555) = 0xa0;
        hal_SysExitCriticalSection(status);
        //SXS_TRACE(TSTDOUT,"hal_flash: unlock\n");
           
        if ((phys_add&1) == 1)
        {
            data = *(ptr);
        } else {
            data = (*buffer) | ~0x00ff;
            buffer ++;
            bsize ++;
        }
        if (bsize<byteSize)
        {
            data = data & (((*buffer) << 8) | 0x00ff);
            buffer ++;
            bsize ++;
        }
    
        if (bsize==byteSize)
        {
            // last byte
            data = data & *(ptr);
        }
        // first byte is prepared
        /* 16b data ready write it to flash*/
        status = hal_SysEnterCriticalSection();
        *(BankAddr) = 0xa0;
        *(ptr)=data;
        hal_SysExitCriticalSection(status);
        while(bsize<byteSize) {
            UINT16 tdata;
            VOLATILE UINT16 * tptr;
        // FLASH_DEBUG_HST(0x2227);
        //FLASH_DEBUG_HST((UINT32)bsize);
       //FLASH_DEBUG_HST((UINT32)byteSize);

            // do the next data preparation before the pooling so we are ready to do a new programming just after pooling is OK.
            tdata = data;
            tptr = ptr;
    
            owsize = wsize;
    
            wsize = bsize;
            ptr+=1;
    
    
            if (bsize<byteSize) {
                data = (*buffer) | ~0x00ff;
                buffer ++;
                bsize ++;
            }
            if(bsize<byteSize) {
                data = data & (((*buffer) << 8) | 0x00ff);
                buffer ++;
                bsize ++;
            }
    
            // pooling
            while(1)
            {
                rdstatus = (*tptr);
                // DQ7 = prog value ? ok done
                if (((rdstatus ^ tdata) & 0x80) == 0)
                    break;
                // DQ5 = 1
                if ((rdstatus & 0x20) == 0x20)
                {
                    rdstatus = (*tptr);
                    // DQ7 = prog value ? ok done
                    if (((rdstatus ^ tdata) & 0x80) == 0)
                        break;
                    errorcode=MEMD_ERR_WRITE;
                    break;
                }
            }
            if (errorcode!=MEMD_ERR_NO) break;
            if(bsize==byteSize) {
                /* last byte */
                data = data & *(ptr);
            }
            /* 16b data ready write it to flash*/
            status = hal_SysEnterCriticalSection();
            *(BankAddr) = 0xa0;
            *(ptr)=data;
            hal_SysExitCriticalSection(status);
        }
        if (errorcode!=MEMD_ERR_NO)
        {
            wsize = owsize;
        } else
        {
            // last data pooling
            while(1)
            {
                rdstatus = (*ptr);
                // DQ7 = prog value ? ok done
                if (((rdstatus ^ data) & 0x80) == 0)
                    break;
                // DQ5 = 1
                if ((rdstatus & 0x20) == 0x20)
                {
                    rdstatus = (*ptr);
                    // DQ7 = prog value ? ok done
                    if (((rdstatus ^ data) & 0x80) == 0)
                        break;
                    errorcode=MEMD_ERR_WRITE;
                    break;
                }
            }
            wsize = bsize;
        }
        
        *pWrittenbyteSize = wsize;
        // Unlock Bypass Exit
        status = hal_SysEnterCriticalSection();
        *(BankAddr) = 0x90;
        *(BankAddr) = 0x00;
       *(BankAddr) = 0xf0;
        hal_SysExitCriticalSection(status);
        hal_EbcFlashWriteEnable(FALSE);
        FLASH_DEBUG_HST(0x2229);
        FLASH_DEBUG_HST((UINT32)errorcode);
        return errorcode;
    }
    else  if (g_flashID == FLASH_M36COW6050TOZSP)
    {
        FLASH_DEBUG_HST(0x422009);
      //  return MEMD_ERR_NO;
        *pWrittenbyteSize=0;
        if (byteSize==0)
        {
            return MEMD_ERR_NO;
        }
        hal_EbcFlashWriteEnable(TRUE);
        phys_add = (UINT32)flashAddress;
        MEMD_ASSERT((phys_add & 0xe0000000) == 0,
            "flash_address is expected to be a byte offset within the flash chip, not an absolute address")
        //    assert(0);
        memd_FlashGetSectorLimits((UINT32)flashAddress, &phy_SectAddr, &phy_End);
        BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&FLASHBANK_MASK));
        ptr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phys_add&~1));
        // Unlock block
        memd_FlashGetLockStatus((UINT8*)((UINT32)phy_SectAddr & MY_MASK),&isLocked);
    	  hal_EbcFlashWriteEnable(TRUE);
        if(isLocked == TRUE)
        {
            memd_BlockLock((UINT8*)((UINT32)phy_SectAddr & MY_MASK),NULL,FALSE);
          
        }
        if ((phys_add&1) == 1)
        {
            data = *(ptr);
        } else {
            data = (*buffer) | ~0x00ff;
            buffer ++;
            bsize ++;
        }
        if (bsize<byteSize)
        {
            data = data & (((*buffer) << 8) | 0x00ff);
            buffer ++;
            bsize ++;
        }
    
        if (bsize==byteSize)
        {
            // last byte
            data = data & *(ptr);
        }
        // first byte is prepared
        /* 16b data ready write it to flash*/
        status = hal_SysEnterCriticalSection();
        *(BankAddr) = CMD_PROG;
        *(ptr)=data;
        hal_SysExitCriticalSection(status);
    
        while(bsize<byteSize) {
            UINT16 tdata;
            VOLATILE UINT16 * tptr;
            // do the next data preparation before the pooling so we are ready to do a new programming just after pooling is OK.
            tdata = data;
            tptr = ptr;
            owsize = wsize;
            wsize = bsize;
            ptr+=1;
    
            if (bsize<byteSize) {
                data = (*buffer) | ~0x00ff;
                buffer ++;
                bsize ++;
            }
            if(bsize<byteSize) {
                data = data & (((*buffer) << 8) | 0x00ff);
                buffer ++;
                bsize ++;
            }
            // pooling
            // 
    	do{
               rdstatus = *BankAddr;
    	}while((rdstatus & SR7) != SR7);
            
            // Should probably fix this scheme for reporting errors more cleanly.
            // For now, just prioritize the errors with the most significant error returned
            // in errorcode (order this list from least to most significant)
            if ((rdstatus & SR4) != 0)
            {
                // Some other programming error, should be decoded, but maybe do it later
                errorcode=MEMD_ERR_WRITE;
            }
            else if ((rdstatus & SR1) != 0)
            {
                errorcode=MEMD_ERR_PROTECT;
            }
    
            if (errorcode!=MEMD_ERR_NO) break;
            if(bsize==byteSize) {
                /* last byte */
                *BankAddr = CMD_READ_ARRAY;
                data = data & *(ptr);
            }
            /* 16b data ready write it to flash*/
            status = hal_SysEnterCriticalSection();
            *(BankAddr) = CMD_PROG;
            *(ptr)=data;
            hal_SysExitCriticalSection(status);
        }
        if (errorcode!=MEMD_ERR_NO)
        {
            wsize = owsize;
        } else
        {
        	// last data pooling
            do{
               rdstatus = *BankAddr;
    	}while((rdstatus & SR7) != SR7);
         
            if ((rdstatus & SR4) != 0)
            {
                // Some other programming error, should be decoded, but maybe do it later
                errorcode=MEMD_ERR_WRITE;
            }
            else if ((rdstatus & SR1) != 0)
            {
                errorcode=MEMD_ERR_PROTECT;
            }
    
            wsize = bsize;
        }
    		
        *pWrittenbyteSize = wsize;
      
        // return to Read Array mode
        status = hal_SysEnterCriticalSection();
        *BankAddr = CMD_READ_ARRAY;
        hal_SysExitCriticalSection(status);
           
        if(errorcode != MEMD_ERR_NO)
        {
        	status = hal_SysEnterCriticalSection();
            *BankAddr = CMD_CLR_STATUS_REG;          
            hal_SysExitCriticalSection(status);
        }
    
        hal_EbcFlashWriteEnable(FALSE);
        return errorcode;
    }
         
         
    return errorcode;
}



// =============================================================================
// memd_FlashRead
// -----------------------------------------------------------------------------
/// This function reads data from the flash. It gets its data from \c flashAddress, 
/// copies \c byteSize bytes to \c buffer. \c pReadbyteSize is filled with the 
/// actual number of read bytes (Equal to \c byteSize, or less in case of error)
///
/// @param flashAddress The byte offset within the flash chip (Take care of 
/// the  positions of the code in the flash)
/// @param byteSize Number of bytes to read from the flash
/// @param pReadbyteSize Filled with the number of bytes actually read
/// @param buffer Buffer to fill with data from flash
/// @return #MEMD_ERR_NO
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashRead(UINT8 *flashAddress,
        UINT32  byteSize, 
        UINT32* pReadbyteSize,
        UINT8*  buffer)
{
    VOLATILE UINT8 * ptr;
    UINT32 phys_add;


    phys_add = (UINT32)flashAddress;
    MEMD_ASSERT((phys_add & 0xe0000000) == 0,
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")

    ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + phys_add);
   

    /* could do aligned read from flash to improve bus accesses as it is uncached */
    memcpy(buffer, (UINT8 *)ptr, byteSize);
    *pReadbyteSize=byteSize;
    return MEMD_ERR_NO;
}



// =============================================================================
// memd_FlashGetGlobalAddress
// -----------------------------------------------------------------------------
/// This function returns the global address in the system memory map 
/// from the flash relative address. 
/// Using the returned address, the user will be able to read directly 
/// from the flash, without using #memd_FlashRead
///
/// Note that the flash can be read directly, but it cannot be written without 
/// using the memd driver functions. That's why the returned address is declared 
/// as \c CONST. The content of the flash can be erased without the help of the 
/// CPU (by sending a command to the flash chip), that's why the returned 
/// address is declared as \c VOLATILE.
///
/// @param flashAddress Byte address within the flash chip to be converted 
/// into a global byte address (take care to the positions of the code in flash)
/// @return The byte global address
// =============================================================================
PUBLIC VOLATILE CONST UINT8* memd_FlashGetGlobalAddress(UINT8 *flashAddress)
{
    VOLATILE CONST UINT8* ptr;
    UINT32 phys_add;

    phys_add = (UINT32)flashAddress;
    MEMD_ASSERT((phys_add & 0xe0000000) == 0,
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");
    ptr = (VOLATILE CONST UINT8 *)(g_memdFlashBaseAddress + phys_add);
    return ptr;
}


PRIVATE MEMD_ERR_T memd_SetLock(UINT8 *phy_startFlshAddr, UINT8 *phy_endFlshAddr, BOOL Lock)
{  
    UINT32 phy_Start;
    UINT32 phy_End;
    UINT32 status;
    VOLATILE UINT16 * CurBankAddr;
    VOLATILE UINT16 * PrevBankAddr;
  
    if ( g_flashID == FLASH_EN71PL064B0 ||g_flashID == FLASH_S71PL064JB0)
    {
        return MEMD_ERR_NO;
    }
    else  if (g_flashID == FLASH_M36COW6050TOZSP)
    {
        if (phy_endFlshAddr == NULL)
        {
            memd_FlashGetSectorLimits((UINT32)phy_startFlshAddr, &phy_Start, &phy_End);
            phy_endFlshAddr = (UINT8*)phy_End;
        } else
        {
            if ((UINT32)phy_endFlshAddr != FLASH_SIZE) {
                memd_FlashGetSectorLimits((UINT32)phy_endFlshAddr , &phy_Start, &phy_End);
                if ((UINT32)phy_endFlshAddr != phy_Start) {
                    return MEMD_ERR_ALIGN;
                }
            }
        }
    
        memd_FlashGetSectorLimits((UINT32)phy_startFlshAddr, &phy_Start, &phy_End);
        if ((UINT32)phy_startFlshAddr != phy_Start)
        {
            return MEMD_ERR_ALIGN;
        }
    
        PrevBankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phy_Start&FLASHBANK_MASK));
    
        while (phy_Start != (UINT32)phy_endFlshAddr)
        {
            VOLATILE UINT16 * SectAddr;
            SectAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + phy_Start);
            CurBankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phy_Start&FLASHBANK_MASK));
            status = hal_SysEnterCriticalSection();
            // If Bank Address is the same as previous Bank Address, continue
            // otherwise, need to put old Bank into Read Array mode and
            // open new Bank.
            if (CurBankAddr != PrevBankAddr)
            {
                // Close Prev Bank
                *PrevBankAddr = CMD_READ_ARRAY;
                PrevBankAddr = CurBankAddr;
            }
    
            // Lock Set/Clr
            if (Lock)
            {
                *SectAddr = CMD_BLOCK_PROT;
                *SectAddr = CMD_BLOCK_LOCK;
            }
            else
            {
                *SectAddr = CMD_BLOCK_PROT;
                *SectAddr = CMD_BLOCK_UNLOCK;
            }
            hal_SysExitCriticalSection(status);
    
            if (phy_End != FLASH_SIZE) {
                memd_FlashGetSectorLimits(phy_End, &phy_Start, &phy_End);
            } else
            {
                phy_Start = phy_End;
            }
        }
    
        // Return to Read Array mode
        status = hal_SysEnterCriticalSection();
        *PrevBankAddr = CMD_READ_ARRAY;
        hal_SysExitCriticalSection(status);
    
        return MEMD_ERR_NO;
    }
    return MEMD_ERR_NO;  
}

PRIVATE UINT16 memd_LockStatus(UINT8 * SectorAddr, UINT16 *Lock)
{
    VOLATILE UINT16 * BankAddr;
    VOLATILE UINT16 * BlockAddr;
    UINT16 DeviceCode;
    UINT32 phy_Start;
    UINT32 phy_End;
    UINT32 status;
    if (g_flashID == FLASH_EN71PL064B0 ||g_flashID == FLASH_S71PL064JB0)
    {
        *Lock = 0;return MEMD_ERR_NO;
    }
    else  if (g_flashID == FLASH_M36COW6050TOZSP)
    {

        memd_FlashGetSectorLimits((UINT32)SectorAddr, &phy_Start, &phy_End);
        if ((UINT32)SectorAddr != phy_Start)
        {
            *Lock = 0xffff;
            return MEMD_ERR_ALIGN;
        }
        BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (phy_Start&FLASHBANK_MASK));
        BlockAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + phy_Start);
        // Block Protection Read
        hal_EbcFlashWriteEnable(TRUE);
        status = hal_SysEnterCriticalSection();
        *BankAddr = CMD_READ_ESD;
        DeviceCode = *(UINT16*)(BankAddr + 0x1);
        *Lock = *(UINT16*)(BlockAddr + 0x2);
        *BankAddr = CMD_READ_ARRAY;
        hal_SysExitCriticalSection(status);
        hal_EbcFlashWriteEnable(FALSE);
        // Write out Device Code for debug check
        return MEMD_ERR_NO;
    }
    return MEMD_ERR_NO;  
}


// =============================================================================
// memd_FlashLock
// -----------------------------------------------------------------------------
/// This function protects the sectors between \c start_flashAddress and 
/// \c end_flashAddress.  Addresses must be sector aligned.  The sector 
/// specified by \c end_flashAddress is @b _NOT_ included in the 
/// protect procedure 
///
/// @param startFlashAddress Address of the first sector to protect
/// @param end_LashAddress Address of the first sector not to protect
/// @return #MEMD_ERR_NO or #MEMD_ERR_ALIGN
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashLock(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress)
{
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    MEMD_ASSERT((((UINT32)endFlashAddress) & 0xe0000000) == 0,
        "endFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    return memd_SetLock(startFlashAddress, endFlashAddress, TRUE);
}


// =============================================================================
// memd_FlashUnlock
// -----------------------------------------------------------------------------
/// This function unlocks the protection of the flash sectors between 
/// \c start_flashAddress and \c end_flashAddress.  Addresses must be 
/// sector aligned.  The sector specified by \c end_flashAddress is 
/// _NOT_ included in the unprotect procedure 
///
/// @param start_flashAddress Address of the first sector to unprotect
/// @param end_flashAddress Address of the first sector not to unprotect
/// @return #MEMD_ERR_NO or #MEMD_ERR_ALIGN
// ============================================================================
PUBLIC MEMD_ERR_T memd_FlashUnlock(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress)
{
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    MEMD_ASSERT((((UINT32)endFlashAddress) & 0xe0000000) == 0,
        "endFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    return memd_SetLock(startFlashAddress, endFlashAddress, FALSE);
}

// =============================================================================
// memd_FlashGetLockStatus
// -----------------------------------------------------------------------------
/// This function gets the protections status of the sector starting at 
/// \c start_flashAddress.  This addresses must be sector 
/// aligned.
///
/// @param startFlashAddress Address of the beginning of the sector to check.
/// @param lockStatus Pointer to the boolean value retaining the result of the
/// get: \c TRUE if the sector is protected, \c FALSE otherwise. The value is 
/// only valid if the return value is MEMD_ERR_NO;
/// @return #MEMD_ERR_NO or #MEMD_ERR_ALIGN
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashGetLockStatus(UINT8* startFlashAddress, BOOL* lockStatus)
{
    UINT16 lock = 0xf;
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    MEMD_ERR_T status = memd_LockStatus(startFlashAddress, &lock);
    if (lock == 1)
    {
        *lockStatus = TRUE;
    }
    else
    {
        *lockStatus = FALSE;
    }
    return status;
}


// =============================================================================
// memd_FlashGetUserFieldInfo
// -----------------------------------------------------------------------------
/// That function get the user data info from memd.def.
///
/// @param userFieldBase The USER_DATA_BASE defined in memd.def.
/// @param userFieldSize The USER_DATA_SIZE defined in memd.def.
/// @param sectorSize The sector size of user data field, it is the large sector size.
///        Some small boot sectors can be regarded as one large sector.
/// @return VOID.
// =============================================================================
PUBLIC VOID memd_FlashGetUserFieldInfo(UINT32 *userFieldBase, UINT32 *userFieldSize, UINT32 *sectorSize, UINT32 *blockSize)
{
    UINT32 Start, End;
    
    *userFieldBase = g_memdFlashLayout->userDataBase;
    *userFieldSize = g_memdFlashLayout->userDataSize;
    *sectorSize = memd_FlashGetSectorLimits(*userFieldBase, &Start, &End);
    *blockSize = g_memdFlashLayout->userDataSectorLogicalBlockSize;
}

