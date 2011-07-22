// =============================================================================
// TGT_UCTLS_CONFIG
// -----------------------------------------------------------------------------
/// List of services
/// See the file ${SOFT_WORKDIR}/platform/svc/uctls/uctls.def for details.
// =============================================================================
#if (1 == UCTLS_SERVICE)
#include "uctls_tgt_params_utraces.h"
#elif (2 == UCTLS_SERVICE)
#include "uctls_tgt_params_umss_storage_mc.h"
#elif (3 == UCTLS_SERVICE)
#include "uctls_tgt_params_umss_storage_ram.h"
#elif (4 == UCTLS_SERVICE)
#include "uctls_tgt_params_umss_storage_flashmc.h"
#elif (5 == UCTLS_SERVICE)
#include "uctls_tgt_params_umss_storage_flashmc_video.h"
#elif (6 == UCTLS_SERVICE)
#include "uctls_tgt_params_umss_storage_flashmc2.h"
#else
#include "uctls_tgt_params_no.h"
#endif

// NOTE: the value of UCTLS_SERVICE is decoded by ../uctls.def

