// =============================================================================
// MEMD config
// =============================================================================
#ifndef TGT_MEMD_CONFIG
#define TGT_MEMD_CONFIG
// -------------------------------
// RAM SETTINGS
// -------------------------------
//   Time register           h R h e  P h W h  e R f
//#define SRAM_TIME_SETTING {0,4,0,0, 0,0,4,0, 0,1,0}
//   Mode register           e p A w  b d W w  P m B m  a p A s  h
//   added :                     v      v                   v
//#define SRAM_MODE_SETTING {1,0,0,1, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0}
//
#ifdef MEMD_RAM_IS_ADMUX
#ifdef MEMD_RAM_IS_BURST
#define SRAM_TIME_SETTING { \
    .rdLatHalfMode= 1, \
    .rdLat=         6, \
    .oedlyHalfMode= 1, \
    .oedly=         1, \
    .pageLat=       0, \
    .wrLatHalfMode= 0, \
    .wrLat=         6, \
    .wedlyHalfMode= 1, \
    .wedly=         1, \
    .relax=         2, \
    .forceRelax=    1}
#define SRAM_MODE_SETTING { \
    .csEn=          1, \
    .polarity=      0, \
    .admuxMode=     1, \
    .writeAllow=    1, \
    .wbeMode=       0, \
    .bedlyMode=     0, \
    .waitMode=      0, \
    .pageSize=      5, \
    .pageMode=      0, \
    .writePageMode= 0, \
    .burstMode=     1, \
    .writeBurstMode=0, \
    .advAsync=      0, \
    .advwePulse=    0, \
    .admuxDly=      0, \
    .writeSingle=   0, \
    .writeHold=     0}

#else //!MEMD_RAM_IS_BURST
#define SRAM_TIME_SETTING { \
    .rdLatHalfMode= 0, \
    .rdLat=         6, \
    .oedlyHalfMode= 1, \
    .oedly=         1, \
    .pageLat=       0, \
    .wrLatHalfMode= 0, \
    .wrLat=         6, \
    .wedlyHalfMode= 1, \
    .wedly=         1, \
    .relax=         1, \
    .forceRelax=    0}
#define SRAM_MODE_SETTING { \
    .csEn=          1, \
    .polarity=      0, \
    .admuxMode=     1, \
    .writeAllow=    1, \
    .wbeMode=       0, \
    .bedlyMode=     0, \
    .waitMode=      0, \
    .pageSize=      0, \
    .pageMode=      0, \
    .writePageMode= 0, \
    .burstMode=     0, \
    .writeBurstMode=0, \
    .advAsync=      0, \
    .advwePulse=    0, \
    .admuxDly=      0, \
    .writeSingle=   0, \
    .writeHold=     0}
#endif //!MEMD_RAM_IS_BURST

#else //!MEMD_RAM_IS_ADMUX
#ifdef MEMD_RAM_IS_BURST
#define SRAM_TIME_SETTING { \
    .rdLatHalfMode= 1, \
    .rdLat=         6, \
    .oedlyHalfMode= 1, \
    .oedly=         1, \
    .pageLat=       0, \
    .wrLatHalfMode= 0, \
    .wrLat=         4, \
    .wedlyHalfMode= 0, \
    .wedly=         1, \
    .relax=         3, \
    .forceRelax=    1}
#define SRAM_MODE_SETTING { \
    .csEn=          1, \
    .polarity=      0, \
    .admuxMode=     0, \
    .writeAllow=    1, \
    .wbeMode=       0, \
    .bedlyMode=     0, \
    .waitMode=      0, \
    .pageSize=      5, \
    .pageMode=      0, \
    .writePageMode= 0, \
    .burstMode=     1, \
    .writeBurstMode=0, \
    .advAsync=      0, \
    .advwePulse=    0, \
    .admuxDly=      0, \
    .writeSingle=   0, \
    .writeHold=     0}

#else //!MEMD_RAM_IS_BURST
#define SRAM_TIME_SETTING { \
    .rdLatHalfMode= 0, \
    .rdLat=         6, \
    .oedlyHalfMode= 0, \
    .oedly=         0, \
    .pageLat=       0, \
    .wrLatHalfMode= 0, \
    .wrLat=         6, \
    .wedlyHalfMode= 0, \
    .wedly=         0, \
    .relax=         1, \
    .forceRelax=    0}
#define SRAM_MODE_SETTING { \
    .csEn=          1, \
    .polarity=      0, \
    .admuxMode=     0, \
    .writeAllow=    1, \
    .wbeMode=       0, \
    .bedlyMode=     0, \
    .waitMode=      0, \
    .pageSize=      0, \
    .pageMode=      0, \
    .writePageMode= 0, \
    .burstMode=     0, \
    .writeBurstMode=0, \
    .advAsync=      0, \
    .advwePulse=    0, \
    .admuxDly=      0, \
    .writeSingle=   0, \
    .writeHold=     0}

#endif //!MEMD_RAM_IS_BURST
#endif //!MEMD_RAM_IS_ADMUX


#define TGT_MEMD_RAM_CONFIG \
    {\
        {SRAM_TIME_SETTING, SRAM_MODE_SETTING}  \
    }


// -------------------------------
// FLASH SETTINGS
// -------------------------------

#ifdef MEMD_FLASH_IS_ADMUX
#define FLASH_TIME_SETTING { \
    .rdLatHalfMode= 0, \
    .rdLat=         15, \
    .oedlyHalfMode= 0, \
    .oedly=         5, \
    .pageLat=       0, \
    .wrLatHalfMode= 0, \
    .wrLat=         15, \
    .wedlyHalfMode= 0, \
    .wedly=         5, \
    .relax=         3, \
    .forceRelax=    1}
#define FLASH_MODE_SETTING { \
    .csEn=          1, \
    .polarity=      0, \
    .admuxMode=     1, \
    .writeAllow=    1, \
    .wbeMode=       0, \
    .bedlyMode=     0, \
    .waitMode=      0, \
    .pageSize=      0, \
    .pageMode=      0, \
    .writePageMode= 0, \
    .burstMode=     0, \
    .writeBurstMode=0, \
    .advAsync=      0, \
    .advwePulse=    0, \
    .admuxDly=      3, \
    .writeSingle=   0, \
    .writeHold=     0}

#else //!MEMD_FLASH_IS_ADMUX
#ifdef MEMD_FLASH_IS_BURST
#define FLASH_TIME_SETTING { \
    .rdLatHalfMode= 0, \
    .rdLat=         2, \
    .oedlyHalfMode= 0, \
    .oedly=         1, \
    .pageLat=       0, \
    .wrLatHalfMode= 1, \
    .wrLat=         9, \
    .wedlyHalfMode= 1, \
    .wedly=         1, \
    .relax=         2, \
    .forceRelax=    1}
#define FLASH_MODE_SETTING { \
    .csEn=          1, \
    .polarity=      0, \
    .admuxMode=     0, \
    .writeAllow=    1, \
    .wbeMode=       0, \
    .bedlyMode=     0, \
    .waitMode=      0, \
    .pageSize=      1, \
    .pageMode=      0, \
    .writePageMode= 0, \
    .burstMode=     1, \
    .writeBurstMode=0, \
    .advAsync=      0, \
    .advwePulse=    0, \
    .admuxDly=      0, \
    .writeSingle=   0, \
    .writeHold=     0}
#else //!MEMD_FLASH_IS_BURST
#define FLASH_TIME_SETTING { \
    .rdLatHalfMode= 1, \
    .rdLat=         9, \
    .oedlyHalfMode= 1, \
    .oedly=         1, \
    .pageLat=       1, \
    .wrLatHalfMode= 1, \
    .wrLat=         9, \
    .wedlyHalfMode= 1, \
    .wedly=         1, \
    .relax=         3, \
    .forceRelax=    0}
#define FLASH_MODE_SETTING { \
    .csEn=          1, \
    .polarity=      0, \
    .admuxMode=     0, \
    .writeAllow=    1, \
    .wbeMode=       0, \
    .bedlyMode=     0, \
    .waitMode=      0, \
    .pageSize=      0, \
    .pageMode=      0, \
    .writePageMode= 0, \
    .burstMode=     0, \
    .writeBurstMode=0, \
    .advAsync=      0, \
    .advwePulse=    0, \
    .admuxDly=      0, \
    .writeSingle=   0, \
    .writeHold=     0}
#endif //!MEMD_FLASH_IS_BURST
#endif //!MEMD_FLASH_IS_ADMUX


#define TGT_MEMD_FLASH_CONFIG                                           \
    {                                                                   \
        {FLASH_TIME_SETTING, FLASH_MODE_SETTING}                        \
    }

#endif // TGT_MEMD_CONFIG


