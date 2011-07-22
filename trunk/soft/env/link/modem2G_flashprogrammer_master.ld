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
/* This linker script is dedicated to the flash_programmer.
 * The linker script depends on the following defines */
/*  - CONST_IN_RAM : all constants are placed in SRAMrun */
/*  - CODE_IN_RAM : All code is executed from SRAM, this is useful for ram_run or code running from RAM on romulator */
/*  - PRELOADED_INT_SRAM : We fill it with a ramrun, so this is always true. */
/*  - PRELOADED_RAM : We don't use any external ram */
/*  - RAMRUN :  The flashprogrammer is always ramruned */

/* Those are constants of the system */


/* ROM content selection: things to include
    WARNING: the DISCARD section uses the opposite -->
    do all inclusion before discard !*/


/* Take into account assert bug in binutils-2.15*/
#if (USE_BINUTILS_2_19 == 1)
#define LD_ASSERT(condition, small, full)   ASSERT(condition, full)
#else
#define LD_ASSERT(condition, small, full)   ASSERT(condition, small)
#endif

ENTRY (__int_rom_main)
EXTERN (__int_rom_main boot_Loader boot_Sector __bcpu_start)


SECTIONS
{
    _bb_sram_flash_start_location = . ;


    _bb_sram_flash_end_location  = _bb_sram_flash_start_location;


    /* ===================================================== */
    /* in SYS SRAM                                           */
    /* ===================================================== */
    _sys_sram_flash_start_location = _bb_sram_flash_end_location;

    /*----------------- */
    /* SYS SRAM UNCACHED */
    /*----------------- */

    /* HAL Boot Sector structure, contains information written by
     * HAL during the normal execution of the code and used by the
     * Boot Sector when the phone boots.
     * It is mapped in the Internal SRAM because we don't want the Boot
     * Sector code to access the External RAM. And it is mapped at a fixed
     * location (after the last patch for the chip) because we
     * don't want the RAMRUN to overwrite it.
     * This structure is "no load" as we don't want it to be loaded
     * by the Ramrun process.
     * For now, the size of the structure g_halBootSectorStruct is 4*4 bytes. */
    /* Use the last patch for the structure */
    .hal_boot_sector_struct (BOOT_SECTOR_STRUCT_BASE) (NOLOAD) :
        AT (ADDR(.hal_boot_sector_struct))
    {
        KEEP(*(.hal_boot_sector_struct))
    }

    /* For ramruns, this section loads an execution context
     * content allowing to directly executed the 
     * boot sector (preloaded with PC=boot_Sector and SP=0,
     * cf boot_sector.c) */
     .executor_ctx ROM_MISC_UBASE : AT (ADDR(.executor_ctx))
    { 
        /* For ramruns, avoid removing .executor_ctx even when option
         * --gc-sections is enabled, as its content must be loaded from
         * the ramrun lod */
        KEEP(*(.executor_ctx))
    }


    /*----------------- */
    /* SYS SRAM CACHED  */
    /*----------------- */

    /* Boot Sector reload structure, contains information written by
     * HAL during the normal execution of the code and used by the
     * Boot Sector to reload the ram image when the phone boots.
     * It is mapped in the Internal SRAM because we don't want the Boot
     * Sector code to access the External RAM. And it is mapped at a fixed
     * location (after the last patch for the chip) because we
     * don't want the RAMRUN to overwrite it.
     * This structure is "no load" as we don't want it to be loaded
     * by the Ramrun process. */
    .boot_sector_reload_struct ( SYS_SRAM_AVAILABLE_BASE-4-4
                                 -SIZEOF(.hal_boot_sector_struct)
                                 -(2*4+3*3*4) ) (NOLOAD) :
        AT (ADDR(.boot_sector_reload_struct))
    {
        KEEP(*(.boot_sector_reload_struct))
    }

    /* Boot Sector structure pointer, used to retrieve the command
     * structure filled by HAL and used by the Boot Sector code.
     * This structure is "no load" as we don't want it to be loaded
     * by the Ramrun process. */
    .boot_sector_struct_ptr (SYS_SRAM_AVAILABLE_BASE-4-4) (NOLOAD) :
        AT (ADDR(.boot_sector_struct_ptr))
    {
        KEEP(*(.boot_sector_struct_ptr))
    }

    .fixptr (SYS_SRAM_AVAILABLE_BASE-4) : AT (ADDR(.fixptr))
    {
        KEEP(*(.fixptr))
    }

    .sramtext (SYS_SRAM_AVAILABLE_BASE) : AT (ADDR(.sramtext))
    {
        _sys_sram_start = .;
        _sys_sram_cached_start = .;
        _sys_sram_txt_start = .;

        _boot_sector = .;
        *(.boot_sector_start)
        *boot_sector.o(.text .text.*)
        *boot_sector_chip.o(.text .text.*)

        _code_base = .;
        *boot_loader.o(.text.boot_loader_start)
        *boot_loader.o(.text .text.* .rdata .rodata .rodata.*)
        *(.sramtext)
        *(.ram)
        *(.text .text.*)

        . = ALIGN(0x10);
    }
    _sys_sram_txt_end = .;
    _sys_sram_txt_size = _sys_sram_txt_end - _sys_sram_start;


    .sramdata _sys_sram_txt_end : AT (ADDR(.sramdata))
    {
        _sys_sram_data_start = .;
        *(.sramdata)

        *(.srrodata)
        *(.rodata.*)
        *(.rodata)
        *(.sdata2.*)
        *(.sdata2)
        *(.sdata.*)
        *(.sdata)
        *(.data.*)
        *(.data)
    } 
    _sys_sram_data_end = .;
    _sys_sram_cached_end = .;
    _sys_sram_data_size = _sys_sram_data_end - _sys_sram_data_start;
    _sys_sram_cached_size = _sys_sram_cached_end - _sys_sram_cached_start;

    /*-------------------- */
    /* SYS SRAM UNCACHED   */
    /*-------------------- */
    /* Go Uncached */
    . += 0x20000000;

    .sramucdata . : AT (ADDR(.sramucdata))
    {
        _sys_sram_uncached_start = .;
        *(.sramucdata)
        *(.srroucdata)
        *(.ucdata)
        *(.backup)
        /* FIXME We don't know what this is for but
         * we put it there for now. */
        *(.rel.dyn)
    } 
    _sys_sram_uncached_end = .;
    _sys_sram_uncached_size = _sys_sram_uncached_end - _sys_sram_uncached_start;
  
    _sys_sram_size = _sys_sram_cached_size + _sys_sram_uncached_size;
    _sys_sram_end = _sys_sram_start + _sys_sram_size;

    _sys_sram_flash_end_location = _sys_sram_flash_start_location;

    /*----------------------- */
    /* SYS SRAM BSS (CACHED)  */
    /*----------------------- */
    .sramucbss . (NOLOAD) : AT (ADDR(.sramucbss))
    {
        _sys_sram_ucbss_start = .;
        *(.sramucbss)
        *(.ucbss)
        . = ALIGN(0x10);
    }
    _sys_sram_ucbss_end = .;
    _sys_sram_ucbss_size = _sys_sram_ucbss_end - _sys_sram_ucbss_start;

    /* Go Cached */
    . -= 0x20000000;

    .srambss . (NOLOAD) : AT (ADDR(.srambss))
    {
        _sys_sram_bss_start = .;
        *(.srambss)

        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
        *(.scommon)
        *(.dynbss)
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(0x10);
    } 
    _sys_sram_bss_end = .;
    _sys_sram_bss_size = _sys_sram_bss_end - _sys_sram_bss_start;

    /* ============================== */
    /* calib sector in sram            */
    /* ============================== */
    .calib . : AT (ADDR(.calib))
    {
        _calib_start = .;
        *(.calib);
        . = ALIGN(0x10);
    } 
    _calib_end = .;
    _calib_size = _calib_end - _calib_start;
  
    /* ============================== */
    /* factory settings in sram      */
    /* ============================== */
    .factory_settings . : AT (ADDR(.factory_settings))
    {
        _factory_start = .;
        *(.factory_settings)
    } 
    _factory_end = .;
    _factory_size = _factory_end - _factory_start;

    /* ============================== */
    /* Main entry point after monitor */
    /* ============================== */
    .main_entry_section . : AT (ADDR(.main_entry_section))
    {
        _board_config_start = .;
        *(.board_config)
        . = ALIGN(0x10);
        _board_config_end = .;
        . = ALIGN(0x10);
    } 

    _board_config_size = _board_config_end - _board_config_start;

    /*----------------------- */
    /* SYS SRAM HEAP 1        */
    /*----------------------- */
    _sys_sram_top = SYS_SRAM_END; /* End of internal sram */
    _sys_sram_heap_start = _board_config_end;
    _sys_sram_heap_end = _sys_sram_top - (SYS_SRAM_IRQ_STACKWATCH_SIZE) - (XCPU_IRQ_STACK_SIZE);
    _sys_sram_heap_size = _sys_sram_heap_end - _sys_sram_heap_start;

    /* Check that the INT SRAM is not over stuffed */
    LD_ASSERT(ABSOLUTE(_sys_sram_heap_end) > ABSOLUTE(_sys_sram_heap_start), "INTS", "Internal SRAM overflow !")


    /*----------------------- */
    /* SYS SRAM IRQ STACK WATCH */
    /*----------------------- */
    _sys_sram_irq_stackwatch_size = (SYS_SRAM_IRQ_STACKWATCH_SIZE) ;
    _sys_sram_irq_stackwatch_start = _sys_sram_heap_end;
    _sys_sram_irq_stackwatch_end = _sys_sram_irq_stackwatch_start + _sys_sram_irq_stackwatch_size;


    /*---------------- */
    /* End of SYS SRAM */
    /*---------------- */

#if (CHIP_HAS_PROXY_MEMORY == 1)
    /* ========================================== */
    /* Proxy Memory                               */
    /* ========================================== */
    /* Standard usage of proxy memory */
    /* Manual placing and copy from flash */
    /* FIXME This section will probably not remain like this,
     * so check it's up-to-date. */
    _proxy_memory_flash_start_location = _sys_sram_flash_end_location;
    .proxy_memory SYS_PROXY_MEM_BASE : AT (ADDR(.proxy_memory))
    {
        _proxy_memory_start = .;
/* Loadable as a ramrun since the proxy memory is enabled
   by the romed monitor.*/ 
        *sxs_rlm.o(.text .text.*)
        *sxr_rlm.o(.text .text.*)
        *hal_timer.o(.text .text.*)
        *sxr_tksd.o(.text .text.*)
        *sxs_io.o(.text .text.*)
        *sxr_tls.o(.text .text.*)
        *sxs_tls.o(.text .text.*)
        *sxr_sbx.o(.text .text.*)
        *sxs_sbx.o(.text .text.*)
        *sxr_tim.o(.text .text.*)
        *hal_trace.o(.text .text.*)
        . = ALIGN(0x10);
    }

    _proxy_memory_end = . ;
    _proxy_memory_size = _proxy_memory_end - _proxy_memory_start ;

    _proxy_memory_flash_end_location = _proxy_memory_flash_start_location;
    _proxy_memory_top = SYS_PROXY_MEM_BASE + SYS_PROXY_MEM_SIZE - 4;
	/* FIXME Add a check about the IRQ reserved size and the no
	overwrite of the code section of this proxy memory */
#else // CHIP_HAS_PROXY_MEMORY
    _proxy_memory_flash_end_location = _sys_sram_flash_end_location;
#endif // CHIP_HAS_PROXY_MEMORY


    /* ========================================== */
    /* in External RAM                            */
    /* ========================================== */
    _ram_flash_start_location = _proxy_memory_flash_end_location;

    /* ------------------------------------------------ */
    /* Ext RAM CACHED  - SECTIONS ALWAYS IN RAM */
    /* ------------------------------------------------ */
    .ram EXT_RAM_BASE : AT (ADDR(.ram))
    {
        _ram_start = .;
        _ram_special_cached_start = .;
        _ram_txt_start = .;
    } 
    _ram_special_cached_end = .;
    _ram_special_cached_size = _ram_special_cached_end - _ram_special_cached_start;


    /*---------------------------- */
    /* Ext RAM CACHED TXT          */
    /*---------------------------- */
    .text  _ram_special_cached_end : AT (ADDR(.text))
    {
        _ram_cached_txt_start = . ;
    } =0
    _ram_cached_txt_end = . ;
    ram_cached_txt_size = _ram_cached_txt_end - _ram_cached_txt_start;

    _ram_txt_end = .;


    /*---------------------- */
    /* Ext RAM UNCACHED Data */
    /*---------------------- */
    /* Go Uncached */
    . += 0x20000000;

    .ucdata . : AT (ADDR(.ucdata))
    {
        _ram_uncached_data_start = .;
    } 
    _ram_uncached_data_end = .;
    _ram_uncached_data_size = _ram_uncached_data_end - _ram_uncached_data_start;

    /*---------------------- */
    /* Ext RAM CACHED Data */
    /*---------------------- */
    /* Go Cached */
    . -= 0x20000000;

    .data . : AT (ADDR(.data))
    {
        _ram_cached_data_start = . ;
    } 
    _ram_cached_data_end = . ;
    _ram_cached_data_size = _ram_cached_data_end - _ram_cached_data_start;
    _ram_end = .;
    _ram_size = _ram_end - _ram_start;

    _ram_flash_end_location = _ram_flash_start_location;


    /*-------------------------------------------------------  */
    /* RAM BSS (content reset to 0 )                           */
    /*-------------------------------------------------------  */
    /* Go Uncached */
    . += 0x20000000;

    .ucbss    . (NOLOAD) : AT (ADDR(.ucbss))
    {
        _ram_ucbss_start = .;
        *(.ramucbss)
        . = ALIGN(0x10);
    }
    _ram_ucbss_end = .;
    _ram_ucbss_size = _ram_ucbss_end - _ram_ucbss_start;

    /* Go Cached */
    . -= 0x20000000;

    .bss . (NOLOAD) : AT (ADDR(.bss))
    {
        _ram_bss_start = .;
        *(.rambss)
        . = ALIGN(0x10);
    } 
    _ram_bss_end = .;
    _ram_bss_size = _ram_bss_end - _ram_bss_start;

    /*----------- */
    /* End of RAM */
    /*----------- */
    _ram_top = EXT_RAM_BASE + RAM_SIZE;
    _heap_size = _ram_top -_ram_bss_end;
    _heap_start =  _ram_bss_end;
    _heap_end =  _ram_bss_end + _heap_size;

    /* Check that the external RAM is not over stuffed */
    LD_ASSERT(ABSOLUTE(_ram_top) > ABSOLUTE(_ram_bss_end), "EXTS", "External RAM overflow !")


    /* kept for compatibility */
    _etext = _ram_txt_end;
    _esramtext = _sys_sram_txt_end;
    _end = _ram_bss_end;





    /*=================================================== */
    /* Placeholder for misc hacks, not used for real apps */
    /*=================================================== */
    .misc . : AT (ADDR(.misc))
    {
        *(.misc)
    }


    /*================================================= */
    /* Define user data boundaries based on the defines */
    /*================================================= */
    _user_data_start = USER_DATA_BASE;
    _user_data_end = USER_DATA_BASE+USER_DATA_SIZE;
    _user_data_size = USER_DATA_SIZE;


    #include "discard.ld"
}

