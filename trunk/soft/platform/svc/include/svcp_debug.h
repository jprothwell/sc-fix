////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://subversion-server/svn/developing/modem2G/trunk/platform/edrv/aud_beijing/include/audp_debug.h $ //
//	$Author: romuald $                                                        // 
//	$Date: 2008-08-01 19:29:54 +0200 (ven 01 aoû 2008) $                     //   
//	$Revision: 16228 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file audp_debug.h                                                          //
/// Debug features to be used only inside the module (protected).
/// Is this file is used for all the debug needs of the module, it will be
/// easier to disable those debug features for the whole module (by changing
/// the value of a define while compiling).
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hal_debug.h"
#include "sxs_io.h"

#ifndef _SVCP_DEBUG_H_
#define _SVCP_DEBUG_H_

// =============================================================================
//  TRACE Level mapping
// -----------------------------------------------------------------------------
/// Important trace that can be enabled all the time (low rate)
#define SVC_WARN_TRC        (_SVC1 | TLEVEL(1))

/// AVCTLS specific trace
#define SVC_AVCTLS_TRC      (_SVC1 | TLEVEL(2))

/// AVPS specific trace
#define SVC_AVPS_TRC        (_SVC1 | TLEVEL(3))

/// AVRS specific trace
#define SVC_AVRS_TRC        (_SVC1 | TLEVEL(4))

/// CAMS specific trace
#define SVC_CAMS_TRC        (_SVC1 | TLEVEL(5))

/// FMG specific trace
#define SVC_FMG_TRC         (_SVC1 | TLEVEL(6))

/// FSS specific trace
#define SVC_FSS_TRC         (_SVC1 | TLEVEL(7))

/// IMGS specific trace
#define SVC_IMGS_TRC        (_SVC1 | TLEVEL(8))

/// MPS specific trace
#define SVC_MPS_TRC         (_SVC1 | TLEVEL(9))

/// MRS specific trace
#define SVC_MRS_TRC         (_SVC1 | TLEVEL(10))

/// UCTLS specific trace
#define SVC_UCTLS_TRC       (_SVC1 | TLEVEL(11))

/// UMSS specific trace
#define SVC_UMSS_TRC        (_SVC1 | TLEVEL(12))

/// UTRACES specific trace
#define SVC_UTRACES_TRC     (_SVC1 | TLEVEL(13))

/// VOIS specific trace
#define SVC_VOIS_TRC        (_SVC1 | TLEVEL(14))

/// UVIDEOS specific trace
#define SVC_UVIDEOS_TRC     (_SVC1 | TLEVEL(15))


// add more here


/// APS specific trace
#define SVC_APS_TRC         (_SVC2 | TLEVEL(14))

/// ARS specific trace
#define SVC_ARS_TRC         (_SVC2 | TLEVEL(15))

/// reserved for debug (can be very verbose, but should probably not stay in code)
#define SVC_DBG_TRC         (_SVC2 | TLEVEL(16))

// =============================================================================
//  TRACE
// -----------------------------------------------------------------------------
/// Trace macro to use to send a trace. The parameter \c format is a string 
/// containing parameters in the "print fashion", but limited for trace to 6
/// parameters. The parameter \c tstmap defines which parameter is a string.
/// (Leave it to 0, if you don't use %s in fmt);
// =============================================================================
// Conditional Printf Usage
#ifndef SVC_NO_TRACE
#define SVC_TRACE(level, tstmap, format, ...)  hal_DbgTrace(level,tstmap,format, ##__VA_ARGS__)
#else
#define SVC_TRACE(level, tstmap, format, ...)  
#endif //SVC_NO_TRACE



#endif // _SVCP_DEBUG_H_

