// =============================================================================
// MEMD config
// =============================================================================
#ifndef TGT_MEMD_CONFIG
#define TGT_MEMD_CONFIG
// -------------------------------
// RAM SETTINGS
// -------------------------------
#define SRAM_TIME_SETTING {                                             \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 3,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 0,                                                \
    .pageLat        = 2,                                                \
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
    .writeHold      = 0}


#define TGT_MEMD_RAM_CONFIG                                             \
    {                                                                   \
        {SRAM_TIME_SETTING, SRAM_MODE_SETTING}                          \
    }


// -------------------------------
// FLASH SETTINGS
// -------------------------------
#define FLASH_TIME_SETTING {                                            \
    .rdLatHalfMode  = 0,                                                \
    .rdLat          = 6,                                                \
    .oedlyHalfMode  = 0,                                                \
    .oedly          = 1,                                                \
    .pageLat        = 2,                                                \
    .wrLatHalfMode  = 0,                                                \
    .wrLat          = 8,                                                \
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

#define TGT_MEMD_FLASH_CONFIG \
    {\
        {FLASH_TIME_SETTING, FLASH_MODE_SETTING}  \
    }

#endif // TGT_MEMD_CONFIG

