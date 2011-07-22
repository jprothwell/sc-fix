// =============================================================================
/// LCDD config
// =============================================================================
#ifndef TGT_LCDD_CONFIG

#define LCDD_MAIN_TIME_SETTING {                                        \
    .rdLatHalfMode  =  0,                                               \
    .rdLat          = 10,                                               \
    .oedlyHalfMode  =  0,                                               \
    .oedly          =  3,                                               \
    .pageLat        =  0,                                               \
    .wrLatHalfMode  =  0,                                               \
    .wrLat          =  6,                                               \
    .wedlyHalfMode  =  0,                                               \
    .wedly          =  1,                                               \
    .relax          =  1,                                               \
    .forceRelax     =  0}
#define LCDD_MAIN_MODE_SETTING {                                        \
    .csEn           =  1,                                               \
    .polarity       =  0,                                               \
    .admuxMode      =  0,                                               \
    .writeAllow     =  1,                                               \
    .wbeMode        =  0,                                               \
    .bedlyMode      =  0,                                               \
    .waitMode       =  0,                                               \
    .pageSize       =  0,                                               \
    .pageMode       =  0,                                               \
    .writePageMode  =  0,                                               \
    .burstMode      =  0,                                               \
    .writeBurstMode =  0,                                               \
    .advAsync       =  0,                                               \
    .advwePulse     =  0,                                               \
    .admuxDly       =  0,                                               \
    .writeSingle    =  0,                                               \
    .writeHold      =  0}


#define TGT_LCDD_CONFIG                                                 \
    {                                                                   \
        HAL_EBC_CS2,                                                    \
        {LCDD_MAIN_TIME_SETTING,LCDD_MAIN_MODE_SETTING},                \
        0                                                               \
    }

#endif // TGT_LCDD_CONFIG

