// =============================================================================
// MEMD config
// =============================================================================
#ifndef TGT_MEMD_CONFIG
#define TGT_MEMD_CONFIG
// -------------------------------
// RAM SETTINGS
// -------------------------------

#define SRAM_TIME_SETTING {                                         \
    .rdLatHalfMode  = 0,                                            \
    .rdLat          = 10,                                            \
    .oedlyHalfMode  = 0,                                            \
    .oedly          = 0,                                            \
    .pageLat        = 0,                                            \
    .wrLatHalfMode  = 0,                                            \
    .wrLat          = 10,                                            \
    .wedlyHalfMode  = 0,                                            \
    .wedly          = 0,                                            \
    .relax          = 2,                                            \
    .forceRelax     = 1}
#define SRAM_MODE_SETTING {                                         \
    .csEn           = 1,                                            \
    .polarity       = 0,                                            \
    .admuxMode      = 0,                                            \
    .writeAllow     = 1,                                            \
    .wbeMode        = 0,                                            \
    .bedlyMode      = 0,                                            \
    .waitMode       = 0,                                            \
    .pageSize       = 0,                                            \
    .pageMode       = 0,                                            \
    .writePageMode  = 0,                                            \
    .burstMode      = 0,                                            \
    .writeBurstMode = 0,                                            \
    .advAsync       = 0,                                            \
    .advwePulse     = 0,                                            \
    .admuxDly       = 0,                                            \
    .writeSingle    = 0,                                            \
    .writeHold      = 0}


#define TGT_MEMD_RAM_CONFIG                                         \
    {                                                               \
        {SRAM_TIME_SETTING, SRAM_MODE_SETTING}                      \
    }


// -------------------------------
// FLASH SETTINGS
// -------------------------------

#define FLASH_TIME_SETTING {                                        \
    .rdLatHalfMode  = 1,                                            \
    .rdLat          = 15,                                            \
    .oedlyHalfMode  = 0,                                            \
    .oedly          = 1,                                            \
    .pageLat        = 0,                                            \
    .wrLatHalfMode  = 1,                                            \
    .wrLat          = 15,                                            \
    .wedlyHalfMode  = 0,                                            \
    .wedly          = 1,                                            \
    .relax          = 3,                                            \
    .forceRelax     = 1}
#define FLASH_MODE_SETTING {                                        \
    .csEn           = 1,                                            \
    .polarity       = 0,                                            \
    .admuxMode      = 0,                                            \
    .writeAllow     = 1,                                            \
    .wbeMode        = 0,                                            \
    .bedlyMode      = 0,                                            \
    .waitMode       = 0,                                            \
    .pageSize       = 0,                                            \
    .pageMode       = 0,                                            \
    .writePageMode  = 0,                                            \
    .burstMode      = 0,                                            \
    .writeBurstMode = 0,                                            \
    .advAsync       = 0,                                            \
    .advwePulse     = 0,                                            \
    .admuxDly       = 0,                                            \
    .writeSingle    = 0,                                            \
    .writeHold      = 0}


#define TGT_MEMD_FLASH_CONFIG                                       \
    {                                                               \
        {FLASH_TIME_SETTING, FLASH_MODE_SETTING}                    \
    }

#endif // TGT_MEMD_CONFIG
