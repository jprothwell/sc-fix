// =============================================================================
/// LCDD config
// =============================================================================
#ifndef TGT_LCDD_CONFIG

#define LCDD_CONFIG {                                                   \
    .busWidth   =   HAL_LCDC_BUS_WIDTH_16_BITS,                         \
    .cs         =   2,                                      \
    .csPolarity =   FALSE,                                              \
    .busKind    =   HAL_LCDC_BUS_KIND_80}

#define LCDD_TIMING_32K {                                               \
    {.tas       =  2,                                                   \
    .tch        =  4,                                                   \
    .tcl        =  4}}                                                  \


#define LCDD_TIMING_13M {                                               \
    {.tas       =  2,                                                   \
    .tch        =  4,                                                   \
    .tcl        =  4}}                                                  \

#define LCDD_TIMING_26M {                                               \
    {.tas       =  2,                                                   \
    .tch        =  4,                                                   \
    .tcl        =  4}}                                                  \

#define LCDD_TIMING_39M {                                               \
    {.tas       =  2,                                                   \
    .tch        =  4,                                                   \
    .tcl        =  4}}                                                  \

#define LCDD_TIMING_52M {                                               \
    {.tas       =  2,                                                   \
    .tch        =  4,                                                   \
    .tcl        =  4}}                                                  \

#define LCDD_TIMING_78M {                                               \
    {.tas       =  2,                                                   \
    .tch        =  4,                                                   \
    .tcl        =  4}}                                                  \

#define LCDD_TIMING_104M {                                              \
    {.tas       =  2,                                                   \
    .tch        =  4,                                                   \
    .tcl        =  4}}                                                  \

#define LCDD_TIMING_156M {                                              \
    {.tas       =  2,                                                   \
    .tch        =  4,                                                   \
    .tcl        =  4}}                                                  \

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

