#ifndef _TCU_ASM_H_
#define _TCU_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'tcu'."
#endif


#define NB_TCO                                   (10)
#define NB_TCU_PROG_EVENTS                       (60)
/// Internal TCO mapping
#define TCO_GMSK_ON                              (8)
#define TCO_RX_DEC_ON                            (9)
/// Clear TCO 0 : set the TCO 0 to the inactive state
/// To clear TCO n, use event 2*n
#define CLR_TCO_0                                (0)
/// Set TCO 0 : set the TCO 0 to the active state
/// To set TCO n, use event 2*n+1
#define SET_TCO_0                                (1)
/// ...
#define CLR_TCO_1                                (2)
/// stop modulation
#define STOP_GMSK                                (TCO_GMSK_ON*2)
/// starts modulation and output on IQ DAC
#define START_GMSK                               (TCO_GMSK_ON*2+1)
/// stop recording IQ samples
#define STOP_RFIN_RECORD                         (TCO_RX_DEC_ON*2)
/// start recording IQ samples
#define START_RFIN_RECORD                        (TCO_RX_DEC_ON*2+1)
/// Send RF spi command
#define SEND_SPI_CMD                             (NB_TCO*2)
#define NEXT_GAIN                                (21)
#define FIRST_GAIN                               (22)
#define RX_SOC                                   (23)
#define DIGRF_STB                                (24)
/// Trigger BCPU TCU irq 0
#define BCPU_TCU_IRQ0                            (25)
/// Trigger BCPU TCU irq 1
#define BCPU_TCU_IRQ1                            (26)
/// Trigger XCPU TCU irq 0
#define XCPU_TCU_IRQ0                            (27)
/// Trigger XCPU TCU irq 1
#define XCPU_TCU_IRQ1                            (28)
/// End of the WakeUp Mode
#define WAKEUP_DONE                              (29)
#define NO_EVENT                                 (31)
#define LP_FRAC_NB_BITS                          (16)
#define LP_COUNT_INT_NB_BITS                     (19)
#define LP_COUNT_NB_BITS                         (LP_COUNT_INT_NB_BITS + LP_FRAC_NB_BITS)
#define LP_RATE_INT_NB_BITS                      (12)
#define LP_RATE_NB_BITS                          (LP_RATE_INT_NB_BITS + LP_FRAC_NB_BITS)
#define RF_COUNT_NB_BITS                         (29)
#define FRAME_COUNT_NB_BITS                      (10)
#define PU_COUNT_NB_BITS                         (12)

//==============================================================================
// Internal_TCO_mapping
//------------------------------------------------------------------------------
/// 
//==============================================================================



//==============================================================================
// TCU_Event
//------------------------------------------------------------------------------
/// 
//==============================================================================



//==============================================================================
// tcu
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_TCU_BASE                0x01A0E000

#define REG_TCU_BASE_HI             BASE_HI(REG_TCU_BASE)
#define REG_TCU_BASE_LO             BASE_LO(REG_TCU_BASE)

#define REG_TCU_CTRL                REG_TCU_BASE_LO + 0x00000000
#define REG_TCU_WRAP_VAL            REG_TCU_BASE_LO + 0x00000004
#define REG_TCU_CUR_VAL             REG_TCU_BASE_LO + 0x00000008
#define REG_TCU_LATCH               REG_TCU_BASE_LO + 0x0000000C
#define REG_TCU_SETUP               REG_TCU_BASE_LO + 0x00000010
#define REG_TCU_DISABLE_EVENT       REG_TCU_BASE_LO + 0x00000014
#define REG_TCU_ENABLE_EVENT        REG_TCU_BASE_LO + 0x00000018
#define REG_TCU_SET_TCO             REG_TCU_BASE_LO + 0x0000001C
#define REG_TCU_CLR_TCO             REG_TCU_BASE_LO + 0x00000020
#define REG_TCU_SNAPSHOT            REG_TCU_BASE_LO + 0x00000024
#define REG_TCU_LPS_SF_CTRL         REG_TCU_BASE_LO + 0x00000028
#define REG_TCU_LPS_SF_STATUS       REG_TCU_BASE_LO + 0x0000002C
#define REG_TCU_LPS_SF_FRAMES       REG_TCU_BASE_LO + 0x00000030
#define REG_TCU_LPS_SF_RESTART_TIME REG_TCU_BASE_LO + 0x00000034
#define REG_TCU_LPS_SF_FRAME_PERIOD REG_TCU_BASE_LO + 0x00000038
#define REG_TCU_LPS_SF_RATE         REG_TCU_BASE_LO + 0x0000003C
#define REG_TCU_LPS_SF_ELAPSED_FRAMES REG_TCU_BASE_LO + 0x00000040
#define REG_TCU_LPS_SF_RF_COUNT     REG_TCU_BASE_LO + 0x00000044
#define REG_TCU_LPS_PU_CTRL         REG_TCU_BASE_LO + 0x00000048
#define REG_TCU_LPS_PU_STATUS       REG_TCU_BASE_LO + 0x0000004C
#define REG_TCU_LPS_PU_TIMES        REG_TCU_BASE_LO + 0x00000050
#define REG_TCU_LPS_PU_IRQ          REG_TCU_BASE_LO + 0x00000054
#define REG_TCU_LPS_PU_IRQ_MASK     REG_TCU_BASE_LO + 0x00000058
#define REG_TCU_EVENT_0             REG_TCU_BASE_LO + 0x00000100
#define REG_TCU_EVENT_1             REG_TCU_BASE_LO + 0x00000104
#define REG_TCU_EVENT_2             REG_TCU_BASE_LO + 0x00000108
#define REG_TCU_EVENT_3             REG_TCU_BASE_LO + 0x0000010C
#define REG_TCU_EVENT_4             REG_TCU_BASE_LO + 0x00000110
#define REG_TCU_EVENT_5             REG_TCU_BASE_LO + 0x00000114
#define REG_TCU_EVENT_6             REG_TCU_BASE_LO + 0x00000118
#define REG_TCU_EVENT_7             REG_TCU_BASE_LO + 0x0000011C
#define REG_TCU_EVENT_8             REG_TCU_BASE_LO + 0x00000120
#define REG_TCU_EVENT_9             REG_TCU_BASE_LO + 0x00000124
#define REG_TCU_EVENT_10            REG_TCU_BASE_LO + 0x00000128
#define REG_TCU_EVENT_11            REG_TCU_BASE_LO + 0x0000012C
#define REG_TCU_EVENT_12            REG_TCU_BASE_LO + 0x00000130
#define REG_TCU_EVENT_13            REG_TCU_BASE_LO + 0x00000134
#define REG_TCU_EVENT_14            REG_TCU_BASE_LO + 0x00000138
#define REG_TCU_EVENT_15            REG_TCU_BASE_LO + 0x0000013C
#define REG_TCU_EVENT_16            REG_TCU_BASE_LO + 0x00000140
#define REG_TCU_EVENT_17            REG_TCU_BASE_LO + 0x00000144
#define REG_TCU_EVENT_18            REG_TCU_BASE_LO + 0x00000148
#define REG_TCU_EVENT_19            REG_TCU_BASE_LO + 0x0000014C
#define REG_TCU_EVENT_20            REG_TCU_BASE_LO + 0x00000150
#define REG_TCU_EVENT_21            REG_TCU_BASE_LO + 0x00000154
#define REG_TCU_EVENT_22            REG_TCU_BASE_LO + 0x00000158
#define REG_TCU_EVENT_23            REG_TCU_BASE_LO + 0x0000015C
#define REG_TCU_EVENT_24            REG_TCU_BASE_LO + 0x00000160
#define REG_TCU_EVENT_25            REG_TCU_BASE_LO + 0x00000164
#define REG_TCU_EVENT_26            REG_TCU_BASE_LO + 0x00000168
#define REG_TCU_EVENT_27            REG_TCU_BASE_LO + 0x0000016C
#define REG_TCU_EVENT_28            REG_TCU_BASE_LO + 0x00000170
#define REG_TCU_EVENT_29            REG_TCU_BASE_LO + 0x00000174
#define REG_TCU_EVENT_30            REG_TCU_BASE_LO + 0x00000178
#define REG_TCU_EVENT_31            REG_TCU_BASE_LO + 0x0000017C
#define REG_TCU_EVENT_32            REG_TCU_BASE_LO + 0x00000180
#define REG_TCU_EVENT_33            REG_TCU_BASE_LO + 0x00000184
#define REG_TCU_EVENT_34            REG_TCU_BASE_LO + 0x00000188
#define REG_TCU_EVENT_35            REG_TCU_BASE_LO + 0x0000018C
#define REG_TCU_EVENT_36            REG_TCU_BASE_LO + 0x00000190
#define REG_TCU_EVENT_37            REG_TCU_BASE_LO + 0x00000194
#define REG_TCU_EVENT_38            REG_TCU_BASE_LO + 0x00000198
#define REG_TCU_EVENT_39            REG_TCU_BASE_LO + 0x0000019C
#define REG_TCU_EVENT_40            REG_TCU_BASE_LO + 0x000001A0
#define REG_TCU_EVENT_41            REG_TCU_BASE_LO + 0x000001A4
#define REG_TCU_EVENT_42            REG_TCU_BASE_LO + 0x000001A8
#define REG_TCU_EVENT_43            REG_TCU_BASE_LO + 0x000001AC
#define REG_TCU_EVENT_44            REG_TCU_BASE_LO + 0x000001B0
#define REG_TCU_EVENT_45            REG_TCU_BASE_LO + 0x000001B4
#define REG_TCU_EVENT_46            REG_TCU_BASE_LO + 0x000001B8
#define REG_TCU_EVENT_47            REG_TCU_BASE_LO + 0x000001BC
#define REG_TCU_EVENT_48            REG_TCU_BASE_LO + 0x000001C0
#define REG_TCU_EVENT_49            REG_TCU_BASE_LO + 0x000001C4
#define REG_TCU_EVENT_50            REG_TCU_BASE_LO + 0x000001C8
#define REG_TCU_EVENT_51            REG_TCU_BASE_LO + 0x000001CC
#define REG_TCU_EVENT_52            REG_TCU_BASE_LO + 0x000001D0
#define REG_TCU_EVENT_53            REG_TCU_BASE_LO + 0x000001D4
#define REG_TCU_EVENT_54            REG_TCU_BASE_LO + 0x000001D8
#define REG_TCU_EVENT_55            REG_TCU_BASE_LO + 0x000001DC
#define REG_TCU_EVENT_56            REG_TCU_BASE_LO + 0x000001E0
#define REG_TCU_EVENT_57            REG_TCU_BASE_LO + 0x000001E4
#define REG_TCU_EVENT_58            REG_TCU_BASE_LO + 0x000001E8
#define REG_TCU_EVENT_59            REG_TCU_BASE_LO + 0x000001EC

//Ctrl
#define TCU_LOAD_VAL(n)             (((n)&0x3FFF)<<0)
#define TCU_ENABLE_DISABLED         (0<<16)
#define TCU_ENABLE_ENABLED          (1<<16)
#define TCU_LOAD                    (1<<20)
#define TCU_NOLATCH_NORMAL          (0<<28)
#define TCU_NOLATCH_FORCE_ONLY      (1<<28)

//Wrap_Val
#define TCU_WRAP_VAL(n)             (((n)&0x3FFF)<<0)

//Cur_Val
#define TCU_CUR_VAL(n)              (((n)&0x3FFF)<<0)

//Latch
#define TCU_FORCELATCH_AREA_0       (1<<0)
#define TCU_FORCELATCH_AREA_1       (1<<1)
#define TCU_FORCELATCH_AREA_2       (1<<2)
#define TCU_FORCELATCH_AREA_3       (1<<3)
#define TCU_FORCELATCH_AREA_4       (1<<4)
#define TCU_FORCELATCH_AREA_5       (1<<5)
#define TCU_FORCELATCH_AREA_6       (1<<6)
#define TCU_FORCELATCH_AREA_7       (1<<7)
#define TCU_FORCE_NOEVENT           (1<<16)
#define TCU_CLEARPROGAREA           (1<<31)
#define TCU_FORCELATCH(n)           (((n)&0xFF)<<0)
#define TCU_FORCELATCH_MASK         (0xFF<<0)
#define TCU_FORCELATCH_SHIFT        (0)

//Setup
#define TCU_POL_TCO_0_ACTIVE_HIGH   (0<<0)
#define TCU_POL_TCO_0_ACTIVE_LOW    (1<<0)
#define TCU_POL_TCO_1_ACTIVE_HIGH   (0<<1)
#define TCU_POL_TCO_1_ACTIVE_LOW    (1<<1)
#define TCU_POL_TCO_2_ACTIVE_HIGH   (0<<2)
#define TCU_POL_TCO_2_ACTIVE_LOW    (1<<2)
#define TCU_POL_TCO_3_ACTIVE_HIGH   (0<<3)
#define TCU_POL_TCO_3_ACTIVE_LOW    (1<<3)
#define TCU_POL_TCO_4_ACTIVE_HIGH   (0<<4)
#define TCU_POL_TCO_4_ACTIVE_LOW    (1<<4)
#define TCU_POL_TCO_5_ACTIVE_HIGH   (0<<5)
#define TCU_POL_TCO_5_ACTIVE_LOW    (1<<5)
#define TCU_POL_TCO_6_ACTIVE_HIGH   (0<<6)
#define TCU_POL_TCO_6_ACTIVE_LOW    (1<<6)
#define TCU_POL_TCO_7_ACTIVE_HIGH   (0<<7)
#define TCU_POL_TCO_7_ACTIVE_LOW    (1<<7)
#define TCU_POL_TCO_8_ACTIVE_HIGH   (0<<8)
#define TCU_POL_TCO_8_ACTIVE_LOW    (1<<8)
#define TCU_POL_TCO_9_ACTIVE_HIGH   (0<<9)
#define TCU_POL_TCO_9_ACTIVE_LOW    (1<<9)
#define TCU_WRITE_ERROR             (1<<28)
#define TCU_DEBUG_ACTIVE_NORMAL     (0<<31)
#define TCU_DEBUG_ACTIVE_DEBUG      (1<<31)
#define TCU_TCO_POLARITY(n)         (((n)&0x3FF)<<0)
#define TCU_TCO_POLARITY_MASK       (0x3FF<<0)
#define TCU_TCO_POLARITY_SHIFT      (0)

//Disable_Event
#define TCU_DISABLE_TCO_0           (1<<0)
#define TCU_DISABLE_TCO_1           (1<<1)
#define TCU_DISABLE_TCO_2           (1<<2)
#define TCU_DISABLE_TCO_3           (1<<3)
#define TCU_DISABLE_TCO_4           (1<<4)
#define TCU_DISABLE_TCO_5           (1<<5)
#define TCU_DISABLE_TCO_6           (1<<6)
#define TCU_DISABLE_TCO_7           (1<<7)
#define TCU_DISABLE_TCO_GMSK_ON     (1<<8)
#define TCU_DISABLE_TCO_RX_DEC_ON   (1<<9)
#define TCU_DISABLE_SEND_SPI_CMD    (1<<10)
#define TCU_DISABLE_NEXT_GAIN       (1<<11)
#define TCU_DISABLE_FIRST_GAIN      (1<<12)
#define TCU_DISABLE_RX_SOC          (1<<13)
#define TCU_DISABLE_DIGRF_STROBE    (1<<14)
#define TCU_DISABLE_BCPU_IRQ_0      (1<<15)
#define TCU_DISABLE_BCPU_IRQ_1      (1<<16)
#define TCU_DISABLE_XCPU_IRQ_0      (1<<17)
#define TCU_DISABLE_XCPU_IRQ_1      (1<<18)
#define TCU_DISABLE_RF_SPI_MARKED_CMD (1<<31)
#define TCU_DISABLE_TCO(n)          (((n)&0xFF)<<0)
#define TCU_DISABLE_TCO_MASK        (0xFF<<0)
#define TCU_DISABLE_TCO_SHIFT       (0)
#define TCU_DISABLE_INTERNAL_TCO(n) (((n)&3)<<8)
#define TCU_DISABLE_INTERNAL_TCO_MASK (3<<8)
#define TCU_DISABLE_INTERNAL_TCO_SHIFT (8)
#define TCU_DISABLE_BCPU_IRQ(n)     (((n)&3)<<15)
#define TCU_DISABLE_BCPU_IRQ_MASK   (3<<15)
#define TCU_DISABLE_BCPU_IRQ_SHIFT  (15)
#define TCU_DISABLE_XCPU_IRQ(n)     (((n)&3)<<17)
#define TCU_DISABLE_XCPU_IRQ_MASK   (3<<17)
#define TCU_DISABLE_XCPU_IRQ_SHIFT  (17)

//Enable_Event
#define TCU_ENABLE_TCO_0            (1<<0)
#define TCU_ENABLE_TCO_1            (1<<1)
#define TCU_ENABLE_TCO_2            (1<<2)
#define TCU_ENABLE_TCO_3            (1<<3)
#define TCU_ENABLE_TCO_4            (1<<4)
#define TCU_ENABLE_TCO_5            (1<<5)
#define TCU_ENABLE_TCO_6            (1<<6)
#define TCU_ENABLE_TCO_7            (1<<7)
#define TCU_ENABLE_TCO_GMSK_ON      (1<<8)
#define TCU_ENABLE_TCO_RX_DEC_ON    (1<<9)
#define TCU_ENABLE_SEND_SPI_CMD     (1<<10)
#define TCU_ENABLE_NEXT_GAIN        (1<<11)
#define TCU_ENABLE_FIRST_GAIN       (1<<12)
#define TCU_ENABLE_RX_SOC           (1<<13)
#define TCU_ENABLE_DIGRF_STROBE     (1<<14)
#define TCU_ENABLE_BCPU_IRQ_0       (1<<15)
#define TCU_ENABLE_BCPU_IRQ_1       (1<<16)
#define TCU_ENABLE_XCPU_IRQ_0       (1<<17)
#define TCU_ENABLE_XCPU_IRQ_1       (1<<18)
#define TCU_ENABLE_RF_SPI_MARKED_CMD (1<<31)
#define TCU_ENABLE_TCO(n)           (((n)&0xFF)<<0)
#define TCU_ENABLE_TCO_MASK         (0xFF<<0)
#define TCU_ENABLE_TCO_SHIFT        (0)
#define TCU_ENABLE_INTERNAL_TCO(n)  (((n)&3)<<8)
#define TCU_ENABLE_INTERNAL_TCO_MASK (3<<8)
#define TCU_ENABLE_INTERNAL_TCO_SHIFT (8)
#define TCU_ENABLE_BCPU_IRQ(n)      (((n)&3)<<15)
#define TCU_ENABLE_BCPU_IRQ_MASK    (3<<15)
#define TCU_ENABLE_BCPU_IRQ_SHIFT   (15)
#define TCU_ENABLE_XCPU_IRQ(n)      (((n)&3)<<17)
#define TCU_ENABLE_XCPU_IRQ_MASK    (3<<17)
#define TCU_ENABLE_XCPU_IRQ_SHIFT   (17)

//Set_TCO
#define TCU_SET_TCO_0               (1<<0)
#define TCU_SET_TCO_1               (1<<1)
#define TCU_SET_TCO_2               (1<<2)
#define TCU_SET_TCO_3               (1<<3)
#define TCU_SET_TCO_4               (1<<4)
#define TCU_SET_TCO_5               (1<<5)
#define TCU_SET_TCO_6               (1<<6)
#define TCU_SET_TCO_7               (1<<7)
#define TCU_SET_TCO_8               (1<<8)
#define TCU_SET_TCO_9               (1<<9)
#define TCU_SET_TCO(n)              (((n)&0x3FF)<<0)
#define TCU_SET_TCO_MASK            (0x3FF<<0)
#define TCU_SET_TCO_SHIFT           (0)

//Clr_TCO
#define TCU_CLR_TCO_0               (1<<0)
#define TCU_CLR_TCO_1               (1<<1)
#define TCU_CLR_TCO_2               (1<<2)
#define TCU_CLR_TCO_3               (1<<3)
#define TCU_CLR_TCO_4               (1<<4)
#define TCU_CLR_TCO_5               (1<<5)
#define TCU_CLR_TCO_6               (1<<6)
#define TCU_CLR_TCO_7               (1<<7)
#define TCU_CLR_TCO_8               (1<<8)
#define TCU_CLR_TCO_9               (1<<9)
#define TCU_CLR_TCO(n)              (((n)&0x3FF)<<0)
#define TCU_CLR_TCO_MASK            (0x3FF<<0)
#define TCU_CLR_TCO_SHIFT           (0)

//Snapshot
#define TCU_SNAPSHOT(n)             (((n)&3)<<0)

//LPS_SF_Ctrl
#define TCU_LPS_SF_ENABLE           (1<<0)
#define TCU_LPS_SF_LOWPOWER(n)      (((n)&3)<<4)
#define TCU_LPS_SF_LOWPOWER_MASK    (3<<4)
#define TCU_LPS_SF_LOWPOWER_SHIFT   (4)
#define TCU_LPS_SF_LOWPOWER_STOP    (0<<4)
#define TCU_LPS_SF_LOWPOWER_SKIPFRAME (1<<4)
#define TCU_LPS_SF_LOWPOWER_CALIB   (3<<4)

//LPS_SF_Status
#define TCU_LPS_SF_READY            (1<<0)
#define TCU_LPS_SF_SLOWRUNNING      (1<<4)
#define TCU_LPS_SF_CALIBRATIONDONE  (1<<8)
#define TCU_LPS_SF_PU_REACHED       (1<<31)

//LPS_SF_Frames
#define TCU_LPS_SF_FRAME(n)         (((n)&0x3FF)<<0)
#define TCU_LPS_SF_PU_FRAME(n)      (((n)&0x3FF)<<16)

//LPS_SF_Restart_Time
#define TCU_LPS_SF_RESTART_TIME(n)  (((n)&0x7FFFF)<<0)

//LPS_SF_Frame_Period
#define TCU_LPS_SF_FRAME_PERIOD(n)  (((n)&0x7FFFF)<<0)

//LPS_SF_Rate
#define TCU_LPS_SF_RATE_INT(n)      (((n)&0xFFF)<<16)
#define TCU_LPS_SF_RATE_FRAC(n)     (((n)&0xFFFF)<<0)

//LPS_SF_Elapsed_Frames
#define TCU_LPS_SF_ELAPSED_FRAMES(n) (((n)&0x3FF)<<0)

//LPS_SF_RF_Count
#define TCU_LPS_SF_RF_COUNT(n)      (((n)&0x1FFFFFFF)<<0)

//LPS_PU_Ctrl
#define TCU_LPS_PU_ENABLE           (1<<0)
#define TCU_LPS_PU_ENABLE_ENABLE    (1<<0)
#define TCU_LPS_PU_ENABLE_DISABLE   (0<<0)
#define TCU_LPS_PU_FORCESTART       (1<<4)
#define TCU_LPS_PU_FORCESTART_ENABLE (1<<4)
#define TCU_LPS_PU_FORCESTART_DISABLE (0<<4)
#define TCU_LPS_PU_CO0_ON           (1<<8)
#define TCU_LPS_PU_CO1_ON           (1<<9)
#define TCU_LPS_PU_CLK_RF_ON        (1<<10)
#define TCU_LPS_PU_SPI_ON           (1<<11)
#define TCU_LPS_PU_CO0_POL          (1<<12)
#define TCU_LPS_PU_CO1_POL          (1<<13)
#define TCU_LPS_PU_CLK_RF_POL       (1<<14)
#define TCU_LPS_PU_SPI_STEP_CO_S1   (0<<16)
#define TCU_LPS_PU_SPI_STEP_RFCLKREADY (1<<16)
#define TCU_LPS_PU_SPI_STEP_READY   (2<<16)
#define TCU_LPS_PU_SPI_STEP_DONE    (3<<16)
#define TCU_LPS_PU_MASK_CO0_S1_ENABLE (1<<20)
#define TCU_LPS_PU_MASK_CO0_S1_IGNORE (0<<20)
#define TCU_LPS_PU_MASK_CO1_S1_ENABLE (1<<21)
#define TCU_LPS_PU_MASK_CO1_S1_IGNORE (0<<21)
#define TCU_LPS_PU_MASK_CO0_S2_ENABLE (1<<22)
#define TCU_LPS_PU_MASK_CO0_S2_IGNORE (0<<22)
#define TCU_LPS_PU_MASK_CO1_S2_ENABLE (1<<23)
#define TCU_LPS_PU_MASK_CO1_S2_IGNORE (0<<23)
#define TCU_LPS_PU_MASK_CLK_RF_ENABLE (1<<24)
#define TCU_LPS_PU_MASK_CLK_RF_IGNORE (0<<24)
#define TCU_LPS_PU_MASK_ST_PLL_ENABLE (1<<25)
#define TCU_LPS_PU_MASK_ST_PLL_IGNORE (0<<25)
#define TCU_LPS_PU_MASK_SW_RF_SWITCH (1<<26)
#define TCU_LPS_PU_MASK_SW_RF_IGNORE (0<<26)
#define TCU_LPS_PU_MASK_SW_FAST_SWITCH (1<<27)
#define TCU_LPS_PU_MASK_SW_FAST_IGNORE (0<<27)
#define TCU_LPS_PU_CPU_DONE         (1<<28)
#define TCU_LPS_PU_RUN_TCU_WAKEUP   (1<<29)
#define TCU_LPS_PU_CO_ON(n)         (((n)&3)<<8)
#define TCU_LPS_PU_CO_ON_MASK       (3<<8)
#define TCU_LPS_PU_CO_ON_SHIFT      (8)
#define TCU_LPS_PU_CO_POL(n)        (((n)&3)<<12)
#define TCU_LPS_PU_CO_POL_MASK      (3<<12)
#define TCU_LPS_PU_CO_POL_SHIFT     (12)
#define TCU_LPS_PU_MASK_CO_S1(n)    (((n)&3)<<20)
#define TCU_LPS_PU_MASK_CO_S1_MASK  (3<<20)
#define TCU_LPS_PU_MASK_CO_S1_SHIFT (20)
#define TCU_LPS_PU_MASK_CO_S2(n)    (((n)&3)<<22)
#define TCU_LPS_PU_MASK_CO_S2_MASK  (3<<22)
#define TCU_LPS_PU_MASK_CO_S2_SHIFT (22)

//LPS_PU_Status
#define TCU_LPS_PU_DONE             (1<<0)
#define TCU_LPS_PU_READY            (1<<4)

//LPS_PU_Times
#define TCU_LPS_PU_TCO_S2(n)        (((n)&31)<<0)
#define TCU_LPS_PU_TCLK_RF_EN(n)    (((n)&31)<<6)
#define TCU_LPS_PU_TCLK_RF(n)       (((n)&0xFFF)<<12)
#define TCU_LPS_PU_TCLK_PLL(n)      (((n)&31)<<24)
#define TCU_LPS_PU_PLL_LOCK_BYPASS  (0<<31)
#define TCU_LPS_PU_PLL_LOCK_CHECK   (1<<31)

//LPS_PU_IRQ
#define TCU_LPS_IRQ_CALIBRATION_DONE_CAUSE (1<<0)
#define TCU_LPS_IRQ_SLOW_RUNNING_CAUSE (1<<1)
#define TCU_LPS_IRQ_PU_DONE_CAUSE   (1<<2)
#define TCU_LPS_IRQ_PU_READY_CAUSE  (1<<3)
#define TCU_LPS_IRQ_PU_REACHED_CAUSE (1<<4)
#define TCU_LPS_IRQ_CALIBRATION_DONE_STATUS (1<<16)
#define TCU_LPS_IRQ_SLOW_RUNNING_STATUS (1<<17)
#define TCU_LPS_IRQ_PU_DONE_STATUS  (1<<18)
#define TCU_LPS_IRQ_PU_READY_STATUS (1<<19)
#define TCU_LPS_IRQ_PU_REACHED_STATUS (1<<20)
#define TCU_LPS_PU_IRQ_CAUSE(n)     (((n)&31)<<0)
#define TCU_LPS_PU_IRQ_CAUSE_MASK   (31<<0)
#define TCU_LPS_PU_IRQ_CAUSE_SHIFT  (0)
#define TCU_LPS_PU_IRQ_STATUS(n)    (((n)&31)<<16)
#define TCU_LPS_PU_IRQ_STATUS_MASK  (31<<16)
#define TCU_LPS_PU_IRQ_STATUS_SHIFT (16)

//LPS_PU_IRQ_Mask
#define TCU_LPS_IRQ_CALIBRATION_DONE_MASK (1<<0)
#define TCU_LPS_IRQ_SLOW_RUNNING_MASK (1<<1)
#define TCU_LPS_IRQ_PU_DONE_MASK    (1<<2)
#define TCU_LPS_IRQ_PU_READY_MASK   (1<<3)
#define TCU_LPS_IRQ_PU_REACHED_MASK (1<<4)
#define TCU_LPS_PU_IRQ_MASK(n)      (((n)&31)<<0)
#define TCU_LPS_PU_IRQ_MASK_MASK    (31<<0)
#define TCU_LPS_PU_IRQ_MASK_SHIFT   (0)

//Event
#define TCU_EVENT_TIME(n)           (((n)&0x3FFF)<<0)
#define TCU_EVENT_ID(n)             (((n)&31)<<16)
#define TCU_EVENT_ID_CLR_TCO_0      (0<<16)
#define TCU_EVENT_ID_SET_TCO_0      (1<<16)
#define TCU_EVENT_ID_CLR_TCO_1      (2<<16)
#define TCU_EVENT_ID_STOP_GMSK      (16<<16)
#define TCU_EVENT_ID_START_GMSK     (17<<16)
#define TCU_EVENT_ID_STOP_RFIN_RECORD (18<<16)
#define TCU_EVENT_ID_START_RFIN_RECORD (19<<16)
#define TCU_EVENT_ID_SEND_SPI_CMD   (20<<16)
#define TCU_EVENT_ID_NEXT_GAIN      (21<<16)
#define TCU_EVENT_ID_FIRST_GAIN     (22<<16)
#define TCU_EVENT_ID_RX_SOC         (23<<16)
#define TCU_EVENT_ID_DIGRF_STB      (24<<16)
#define TCU_EVENT_ID_BCPU_TCU_IRQ0  (25<<16)
#define TCU_EVENT_ID_BCPU_TCU_IRQ1  (26<<16)
#define TCU_EVENT_ID_XCPU_TCU_IRQ0  (27<<16)
#define TCU_EVENT_ID_XCPU_TCU_IRQ1  (28<<16)
#define TCU_EVENT_ID_WAKEUP_DONE    (29<<16)
#define TCU_EVENT_ID_NO_EVENT       (31<<16)




#endif
