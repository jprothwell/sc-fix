#ifndef _UVIDEOSP_DEBUG_H_
#define _UVIDEOSP_DEBUG_H_

#include "cs_types.h"
#include "svcp_debug.h"

// =============================================================================
// MACROS                                                                       
// =============================================================================

// =============================================================================
//  TRACE Level mapping
// -----------------------------------------------------------------------------
/// Important trace that can be enabled all the time (low rate)
// =============================================================================
#define UVIDEOS_WARN_TRC   SVC_WARN_TRC

/// Informational trace interesting for AVRS only
#define UVIDEOS_INFO_TRC   SVC_UVIDEOS_TRC

/// reserved for debug (can be very verbose, but should probably not stay in code)
#define UVIDEOS_DBG_TRC    SVC_DBG_TRC

#ifndef UVIDEOS_NO_PRINTF
  #define UVIDEOS_TRACE(level, tsmap, format, ...)   SVC_TRACE(level,tsmap,format, ##__VA_ARGS__)
#else
  #define UVIDEOS_TRACE(level, tsmap, fmt, ...)
#endif // UVIDEOS_NO_PRINTF

// =============================================================================
// TYPES                                                                        
// =============================================================================


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================


#endif // _UVIDEOSP_DEBUG_H_

