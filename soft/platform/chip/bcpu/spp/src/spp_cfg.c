#include "cs_types.h"
#include "spp_cfg.h"
#include "spal_debug.h"
#include "spal_mem.h"
#include "global_macros.h"


volatile SPP_GLOBAL_PARAMS_T g_sppGlobalParams SPAL_BBSRAM_GLOBAL;

PUBLIC VOID spp_Open(SPP_GLOBAL_PARAMS_T * globalParams)
{
    // Initialized GDB variables.
    spal_GdbOpen();

    // copy cfg to fixed memory location defined by section (".spp.global")
    g_sppGlobalParams = *globalParams;
}

