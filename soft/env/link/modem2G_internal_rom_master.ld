/* ======================================================================= */
/*                  TARGET CONFIGURATION SECTION                           */
/* ======================================================================= */
/* The following values are defined in ff_target.def and exported by the Makefile */
/*  FLASH_SIZE              */
/*  RAM_SIZE                */
/*  CALIB_BASE              */
/*  FACT_SETTINGS_BASE      */
/*  CODE_BASE               */
/*  USER_DATA_BASE          */
/*  USER_DATA_SIZE          */
/* ======================================================================= */
/*                  END CONFIGURATION SECTION                              */
/* ======================================================================= */
/* The linker script depends on the following defines */
/*  - CONST_IN_RAM : all constants are placed in RAM, this is useful for ram_run */
/*  - CODE_IN_RAM : All code is executed from RAM, this is useful for ram_run or code running from RAM on romulator */
/*  - PRELOADED_INT_SRAM : The internal SRAM are already preloaded, useful for ram_run */
/*  - PRELOADED_RAM : The external RAm is already preloaded, useful for ram_run or simu */

/* Take into account assert bug in binutils-2.15*/
#if (USE_BINUTILS_2_19 == 1)
#define LD_ASSERT(condition, small, full)   ASSERT(condition, full)
#else
#define LD_ASSERT(condition, small, full)   ASSERT(condition, small)
#endif

EXTERN (__xcpu_start __bcpu_start)

EXTERN (boot_romCsString boot_Monitor g_bootRomVersionNumber)

EXTERN (_boot_IrqHandler)

EXTERN (spp_SearchAmrMarker  spp_CipherCountInit spp_FcchDetectAsm spp_FcchDetect
        spal_IrqSetMask spp_DecodeBlock spp_Open
        spp_EqualizeNBurst spp_SearchFcch spp_EncodeBlock
        spal_ItlvItlv spp_10log10 g_sppNbFwequStartSt
        __bb_int_handler 
        )

EXTERN (spal_InvalidateDcache 
        spal_Open
        spal_VitacBusy
        spal_XcorTscxcNb
        spal_DbgHstSendEvent
        )

EXTERN (spc_AmrDecode spc_UpdateAmrCfg g_mailbox 
        spc_RxProcess g_spcCtxStruct AFSfilterCI
        spc_MboxToSppSpeechMode spc_IrqHandler)

EXTERN (memcpy)

EXTERN (atoi
        _ctype
        memchr
        memcmp
        memmove
        memscan
        strcat
        strchr
        strcmp
        strcpy
        strlen
        strncat
        strncmp
        strncpy
        strnicmp
        strsep
        strstr
        vsscanf)

/* Chip Specific Grabber. */
/* TODO Improve readibility */
#if (CHIP_ASIC_ID == 3)        
/*		#include "greenstone_internal_rom_content" */
        EXTERN (vsprintf)
#elif (CHIP_ASIC_ID == 4)
/*        #include "nephrite_internal_rom_content" */
#elif (CHIP_ASIC_ID == 5)
/*        #include "emerald_internal_rom_content" */
#elif (CHIP_ASIC_ID == 6)
/*        #include "gallite_internal_rom_content" */
#else
#error "Unsupported Chip Id"
#endif  

EXTERN (boot_UsbOpen
        boot_UsbClose
        boot_UsbReset
        boot_UsbRecv
        boot_UsbSend
        boot_UsbEpStall
        boot_UsbCompletedCommand
        boot_UsbEpEnableInterrupt
        boot_UsbEpRxTransferedSize
        boot_UsbEpTransfertDone
        boot_UsbIrqHandler)

EXTERN (boot_HostUsbOpen
        boot_HostUsbClose
        boot_HostUsbSend
        boot_HostUsbRecv)

EXTERN (g_spalMapVersion g_spcMapVersion g_sppMapVersion)

#ifndef ROM_RM_VOCODER
EXTERN (g_vppAmrCode)
#endif

SECTIONS
{

    /* ============================= */
    /* boot monitor in flash         */
    /* ============================= */

    /* ========================================================= */
    /* INT ROM                                                   */
    /* ========================================================= */
  	.internal_rom  INT_ROM_BASE : AT(ADDR(.internal_rom))
    {
        /* Specifies a section for the functions to put
           in ROM. */
        __int_rom_main = .;
        _int_rom_start = .;
 
/* TODO Improve readibility */
#if (CHIP_ASIC_ID == 3)        
		#include "greenstone_internal_rom_content"
#elif (CHIP_ASIC_ID == 4)
        #include "nephrite_internal_rom_content"
#elif (CHIP_ASIC_ID == 5)
        #include "emerald_internal_rom_content"
#elif (CHIP_ASIC_ID == 6)
        #include "gallite_internal_rom_content"
#else
#error "Unsupported Chip Id"
#endif       
        . = ALIGN(0x10);
    }
        
        __int_rom_end = .;

    .rom_entries_cached ROM_MISC_BASE:AT(ADDR(.rom_entries_cached))
    {
#if (CHIP_ASIC_ID == 3)        
        #include "greenstone_internal_rom_reserved_ram"
#elif (CHIP_ASIC_ID == 4)
        #include "nephrite_internal_rom_reserved_ram"
#elif (CHIP_ASIC_ID == 5)
        #include "emerald_internal_rom_reserved_ram"
#elif (CHIP_ASIC_ID == 6)
        #include "gallite_internal_rom_reserved_ram"
#else
#error "Unsupported Chip Id"
#endif
    }

    __int_rom_reserved_end = . ;
    
    /* Reserved Ram for Rom overflow */
    LD_ASSERT(ABSOLUTE(__int_rom_reserved_end) <= SYS_SRAM_AVAILABLE_BASE, "RRRO", "Reserved Ram Reserved for Rom Overflowed")

    /* ========================================================= */
    /* BB SRAM                                                   */
    /* ========================================================= */
    .bbsram BB_SRAM_BASE : AT(ADDR(.bbsram))
    {
        _bb_sram_romed_start = .;
        _bb_sram_romed_cached_start = .;
        *(BB_ITLV_SECTION)     
        . = ALIGN(0x4);
        _bb_sram_romed_itlv_buf_end = .;
        *(BB_SPC_CTX_SECTION)
        . = ALIGN(0x10);
    } 
    _bb_sram_romed_cached_end = .;
    _bb_sram_romed_cached_size = _bb_sram_romed_cached_end - _bb_sram_romed_cached_start;
    
    . = . + 0x20000000;
    
    .bbsramu : AT(ADDR(.bbsramu))
    {
        _bb_sram_romed_uncached_start = .;
        . = ALIGN(0x10);
        _mbbsramu = .;
        *(BB_STATIC_SECTION)
        . = ALIGN(0x10);
    } 
    _bb_sram_romed_uncached_end = .;
    _bb_sram_romed_uncached_size = _bb_sram_romed_uncached_end - _bb_sram_romed_uncached_start;
    
    _bb_sram_romed_size = _bb_sram_romed_cached_size + _bb_sram_romed_uncached_size;
    _bb_sram_romed_end = _bb_sram_romed_start + _bb_sram_romed_size;
    
    .bbsram_globals BB_SRAM_BASE + BB_SRAM_SIZE - BB_SRAM_GLOBALS_SIZE : AT(ADDR(.bbsram_globals))
    {
        _bb_sram_romed_globals_start = .;
        *(BB_SRAM_GLOBALS)
        *(.bbsram_globals)
        _bb_sram_romed_globals_end = .;
    }
    
    /* BBSRAM overflow */
    LD_ASSERT(SIZEOF(.bbsram_globals) <= BB_SRAM_GLOBALS_SIZE, "BBSO", "BBSRAM Overflozed !!")
    
    /* ========================================================= */
    /* INT SRAM                                                  */
    /* ========================================================= */
    .sram_romed_top SYS_SRAM_BASE + SYS_SRAM_SIZE - RESERVED_MBX_SIZE - 4 : AT(ADDR(.sram_romed_top))
    {
        _sys_sram_non_romed_top = .;
    }


    .mailbox SYS_SRAM_BASE + SYS_SRAM_SIZE - RESERVED_MBX_SIZE + 0x20000000 : AT(ADDR(.mailbox))
    {
        _sys_sram_romed_mbx_start = .;
        *spc_mailbox.o(.mailbox)
        . = ALIGN(0x10);
    } 

    /* Mailbox overflow */
    LD_ASSERT(SIZEOF(.mailbox) <= RESERVED_MBX_SIZE, "MBXO", "Mailbox Overlfowed !!")

    /* ========================================================= */
    /* Back in the INT ROM                                       */
    /* ========================================================= */
    
    .boot_rom_version_number INT_ROM_END - 4: AT(ADDR(.boot_rom_version_number))
    {
        *(.boot_rom_version_number)
    }
    
    .internal_rom.crc INT_ROM_END: AT(ADDR(.internal_rom.crc))
    {
        __int_rom_crc = . ;
        LONG(0xc001c001)
    }

    /DISCARD/ :
    {
        *.o
    }
}

