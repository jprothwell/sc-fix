// =============================================================================
// MEMD config
// =============================================================================
#ifndef TGT_MEMD_CONFIG
#define TGT_MEMD_CONFIG

// THIS COMBO IS ADMUX

// -------------------------------
// RAM SETTINGS for 52 MHz
// -------------------------------
#ifdef MEMD_RAM_IS_BURST
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 6,                                                \
    .oedlyHalfMode  = 1,                                                \
    .oedly          = 1,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 6,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 2,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 0,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 1,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}

#else //!MEMD_RAM_IS_BURST
#ifdef RAM_CLK_IS_52M
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 3,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 0,                                                \
    .pageLat        = 1,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 3,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 0,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 1,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 1}
#else
#ifdef RAM_CLK_IS_78M
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 5,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 0,                                                \
    .pageLat        = 2,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 5,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 0,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 1,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else
#ifdef RAM_CLK_IS_104M
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 7,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 0,                                                \
    .pageLat        = 3,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 7,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                             \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 0,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 1,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else

#endif//RAM_CLK_IS_104M
#endif//RAM_CLK_IS_78M
#endif//RAM_CLK_IS_52M
#endif //!MEMD_RAM_IS_BURST


#define TGT_MEMD_RAM_CONFIG                                             \
    {                                                                   \
        {SRAM_TIME_SETTING, SRAM_MODE_SETTING}                          \
    }


// -------------------------------
// FLASH SETTINGS
// -------------------------------
#ifdef MEMD_FLASH_IS_BURST
#define FLASH_TIME_SETTING {                                            \
    .rdLatHalfMode  = 1,                                                \
    .rdLat          = 5,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 0,                                                \
    .pageLat        = 0,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 10,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define FLASH_MODE_SETTING {                                            \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 0,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 0,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 1,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else //!MEMD_FLASH_IS_BURST
#ifdef RAM_CLK_IS_52M
#define FLASH_TIME_SETTING {                                            \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 3,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 0,                                                \
    .pageLat        = 1,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 5,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 0,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define FLASH_MODE_SETTING {                                            \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 0,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 1,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else
#ifdef RAM_CLK_IS_78M
#define FLASH_TIME_SETTING {                                            \
    .rdLatHalfMode  = 1,                                                \
    .rdLat          = 5,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 0,                                                \
    .pageLat        = 1,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 6,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 1,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define FLASH_MODE_SETTING {                                            \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 0,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 1,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else
#ifdef RAM_CLK_IS_104M
#define FLASH_TIME_SETTING {                                            \
    .rdLatHalfMode  = 1,                                                \
    .rdLat          = 7,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 0,                                                \
    .pageLat        = 2,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 7,                                                \
    .wedlyHalfMode  = 0,                                                \
    .wedly          = 1,                                                \
    .relax          = 1,                                                \
    .forceRelax     = 0}
#define FLASH_MODE_SETTING {                                            \
    .csEn           = 1,                                                \
    .polarity       = 0,                                                \
    .admuxMode      = 0,                                                \
    .writeAllow     = 1,                                                \
    .wbeMode        = 0,                                                \
    .bedlyMode      = 0,                                                \
    .waitMode       = 0,                                                \
    .pageSize       = 0,                                                \
    .pageMode       = 1,                                                \
    .writePageMode  = 0,                                                \
    .burstMode      = 0,                                                \
    .writeBurstMode = 0,                                                \
    .advAsync       = 0,                                                \
    .advwePulse     = 0,                                                \
    .admuxDly       = 0,                                                \
    .writeSingle    = 0,                                                \
    .writeHold      = 0}
#else

#endif//RAM_CLK_IS_104M
#endif//RAM_CLK_IS_78M
#endif//RAM_CLK_IS_52M
#endif

#define TGT_MEMD_FLASH_CONFIG                                           \
    {                                                                   \
        {FLASH_TIME_SETTING, FLASH_MODE_SETTING}                        \
    }

#endif // TGT_MEMD_CONFIG


