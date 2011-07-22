// =============================================================================
/// LCDD config
//
// THIS IS ACTUALLY NEPHRITE_FPGA TIMINGS
// =============================================================================
#ifndef TGT_LCDD_CONFIG

#define LCDD_CONFIG {                                                   \
    {.cs            =   HAL_GOUDA_LCD_CS_0,                             \
    .outputFormat   =   HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565,      \
    .cs0Polarity    =   FALSE,                                          \
    .cs1Polarity    =   FALSE,                                          \
    .rsPolarity     =   FALSE,                                          \
    .wrPolarity     =   FALSE,                                          \
    .rdPolarity     =   FALSE}}

///@toto check bellow timings with LCD spec

#define LCDD_TIMING_32K {                                               \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  2,                                                   \
    .pwh        =  2}}                                                  \


#define LCDD_TIMING_13M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  2,                                                   \
    .pwh        =  2}}                                                  \

#define LCDD_TIMING_26M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  2,                                                   \
    .pwh        =  2}}                                                  \

#define LCDD_TIMING_39M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  2,                                                   \
    .pwh        =  2}}                                                  \

#define LCDD_TIMING_52M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  2,                                                   \
    .pwh        =  2}}                                                  \

#define LCDD_TIMING_78M {                                               \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  3,                                                   \
    .pwh        =  3}}                                                  \

#define LCDD_TIMING_104M {                                              \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  4,                                                   \
    .pwh        =  4}}                                                  \

#define LCDD_TIMING_156M {                                              \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  5,                                                   \
    .pwh        =  5}}                                                  \

#define TGT_LCDD_CONFIG                                                 \
    {                                                                   \
        .config     = LCDD_CONFIG,                                      \
        .timings    = {                                                 \
            LCDD_TIMING_32K,                                            \
            LCDD_TIMING_13M,                                            \
            LCDD_TIMING_26M,                                            \
            LCDD_TIMING_39M,                                            \
            LCDD_TIMING_52M,                                            \
            LCDD_TIMING_78M,                                            \
            LCDD_TIMING_104M,                                           \
            LCDD_TIMING_156M,                                           \
        }                                                               \
    }

#endif // TGT_LCDD_CONFIG

