#ifndef _SIMDP_DEBUG_H_
#define _SIMDP_DEBUG_H_

#include "hal_debug.h"


// =============================================================================
//  TRACE Level mapping
// -----------------------------------------------------------------------------
/// Important trace that can be enabled all the time (low rate)
#define SIMD_WARN_TRC EDRV_WARN_TRC
/// Informational trace interesting for SIMD only
#define SIMD_INFO_TRC EDRV_SIMD_TRC
/// reserved for debug (can be very verbose, but should probably not stay in code)
#define SIMD_DBG_TRC EDRV_DBG_TRC

// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string 
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
// =============================================================================
// Conditional Printf Usage
#ifndef SIMD_NO_PRINTF
#include "cs_types.h"
#include "sxs_io.h"
#include "edrvp_debug.h"

/// #warning TRACE DEFINED !!!!
#define SIMD_TRACE(level, tstmap, format, ...)  EDRV_TRACE(level,tstmap,format, ##__VA_ARGS__)
#else
#define SIMD_TRACE(level, tstmap, format, ...)  
#endif //SIMD_PRINTF



// =============================================================================
//  ASSERT
// -----------------------------------------------------------------------------
/// Assert: If the boolean condition (first parameter) is false, 
/// raise an assert and print the decorated string passed in the other
/// parameter.
// =============================================================================
#ifdef SIMD_NO_ASSERT
#define SIMD_ASSERT(boolCondition, format, ...)
#else
#define SIMD_ASSERT(boolCondition, format, ...)                   \
    if (!(boolCondition)) {            \
        hal_DbgAssert(format, ##__VA_ARGS__);                             \
    }
#endif


#endif //_SIMDP_DEBUG_H_

