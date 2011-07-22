// ***********************************************************************
//                                                                        
//               Copyright (C) 2004, Coolsand Technologies, Inc.          
//                             All Rights Reserved                        
//                                                                        
// ***********************************************************************
/// 
///   @file spp_cfg.h
///   This File contains the SPP configuration functions API
/// 
// ***********************************************************************

#ifndef SPP_CFG_H
#define SPP_CFG_H

#include "spp_gsm.h"

///  @defgroup cfg_func SPP Configuration
///@{


typedef struct
{
    // Flags
    BOOL bypassDcoc;
    BOOL correctCS1;
    BOOL correctCS4;
    BOOL equ_hburst_mode;
    // Parameters
    INT16 symb_max;
    INT16 symb_min;
    UINT8 fof_nb_symb;
    UINT8 fof_start_1;
    UINT8 fof_start_2;
    UINT8 fof_corr_gap;
    UINT32 agc_threshold;
    UINT8 mon_win_size;    
    UINT8 flag_enableDC;
    UINT8 flag_enableIQ;
    INT8  shift_bm_offset;
    INT8  shift_sb_offset;
    // function
#if (SPC_IF_VER >= 2)
    BOOL (*fcch_detect_func) (UINT32* Ptr_old,
                        UINT32* Ptr_new,
                        SPP_FCCH_PARAM_T* Params,
                        SPP_FCCH_RESULT_T* Res);
    UINT8  fcch_size;
    BOOL fcch_sliding_energy;
    BOOL fcch_hw_sliding_window;
#endif

    //UINT16 debug[16];
    //
} SPP_GLOBAL_PARAMS_T;

// =============================================================================
// spp_InitGlobalParams
// -----------------------------------------------------------------------------
/// This function MUST be in RAM and not in ROM !
/// This function initializes the global parameter structure used by SPP.
/// This allows to select (and tune) some SPP processing options.
// =============================================================================
PUBLIC VOID spp_Open(SPP_GLOBAL_PARAMS_T * globalParams);

///@}

#endif //  SPP_CFG_H 
