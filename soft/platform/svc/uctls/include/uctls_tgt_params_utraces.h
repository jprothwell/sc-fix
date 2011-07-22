// =============================================================================
// TGT_UCTLS_CONFIG
// -----------------------------------------------------------------------------
/// List of services
// =============================================================================
#ifndef TGT_UCTLS_CONFIG

#define TGT_UCTLS_CONFIG                                                \
{                                                                       \
    {                                                                   \
        .servicesList[0] =                                              \
        {                                                               \
            .serviceCallback = &g_utracesCallback                       \
        }                                                               \
    },                                                                  \
    {                                                                   \
        .servicesList[0].serviceCallback = 0                            \
    }                                                                   \
}

#endif // TGT_UCTLS_CONFIG

