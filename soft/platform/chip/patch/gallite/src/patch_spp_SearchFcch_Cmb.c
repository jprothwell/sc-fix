//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file patch_spp_SearchFcch_Cmb.c
///
/// This file contains the patch functions for the FCCH Search. 2 fcch search engine
/// combined in this function. Use g_fcch_detect_mode to decide which engine will be used.
///
//    $Author: Chunjiang TU $                                                        // 
///  $Date: 2010-12-23 $ //
//-----------------------------------------------------------------------------

#ifdef  ENABLE_PATCH_SPP_SEARCHFCCH_CMB


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

// TOP
// include
#include "cs_types.h"
// common/include
#include "gsm.h"
#include "baseband_defs.h"
// spal public headers
#include "spal_xcor.h"
#include "spal_cap.h"
#include "spal_itlv.h"
// spp public headers
#include "spp_gsm.h"
#include "spp_profile_codes.h"
// spp private headers
#include "sppp_gsm_private.h"
#include "sppp_cfg.h"
// spal
#include "spal_debug.h"
#include "spal_mem.h"

#include "xcor.h"


#include "global_macros.h"
#include "bb_ifc.h"


extern PUBLIC SPP_FCCH_RETURN_T patch_spp_SearchFcch_Old(UINT32* RxBuffer, BOOL Init, SPP_FCCH_MODE_T Mode, SPP_FCCH_RESULT_T* Res);
extern PUBLIC SPP_FCCH_RETURN_T patch_spp_SearchFcch(UINT32* RxBuffer, BOOL Init, SPP_FCCH_MODE_T Mode, SPP_FCCH_RESULT_T* Res);
extern UINT8 g_fcch_detect_mode; // 0: FCCH Search with AFC-Unlocked. 1: FCCH Search with AFC-LOCKED.


PUBLIC SPP_FCCH_RETURN_T SPAL_FUNC_INTERNAL patch_spp_SearchFcch_Cmb(UINT32* RxBuffer, BOOL Init, SPP_FCCH_MODE_T Mode, SPP_FCCH_RESULT_T* Res)
{
    if ( g_fcch_detect_mode == 0 )
        return patch_spp_SearchFcch_Old(RxBuffer, Init, Mode, Res);
    else
        return patch_spp_SearchFcch(RxBuffer, Init, Mode, Res);
}




#endif
