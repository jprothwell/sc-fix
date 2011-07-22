////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2008, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/patch/gallite/src/patch_open.c $ //
//    $Author: zhanghy $                                                        // 
//    $Date: 2010-12-24 14:57:58 +0800 (Fri, 24 Dec 2010) $                     //   
//    $Revision: 4836 $                                                        //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//       
/// @file patch_open.c
/// That file implement the patch_Open function
//
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "patch_m.h"

PRIVATE CONST UINT32 GALLITE_8805_ROM_VERSION = 0x11001203;
extern CONST UINT32 g_bootRomVersionNumber;

// =============================================================================
//  FUNCTIONS
// =============================================================================

// declare patched function as external and (CONST VOID (*)(VOID)) to avoid a cast

// For BCPU buffer profiling
extern PUBLIC VOID spal_RegisterProfileBuffer(VOID);
extern CONST VOID bb_prog_rx_win(VOID);
extern CONST VOID patch_bb_prog_rx_win(VOID);
extern CONST VOID spal_IrqHandler(VOID);
extern CONST VOID patch_spal_IrqHandler(VOID);

// For Rx/Tx debugging
extern CONST VOID spc_RxProcess(VOID);
extern CONST VOID patch_spc_RxProcess(VOID);
extern CONST VOID spp_BuildNBurst(VOID);
extern CONST VOID patch_spp_BuildNBurst(VOID);

// ----------------------------------------

// For real fixes
extern CONST VOID spp_Modulation(VOID);
extern CONST VOID patch_spp_Modulation(VOID);
extern CONST VOID spc_IrqHandler(VOID);
extern CONST VOID patch_spc_IrqHandler(VOID);
extern CONST VOID spc_SchedulerMain(VOID);
extern CONST VOID patch_spc_SchedulerMain(VOID);
extern CONST VOID spc_AFSdecoding(VOID);
extern CONST VOID patch_spc_AFSdecoding(VOID);
extern CONST VOID spc_AHSdecoding(VOID);
extern CONST VOID patch_spc_AHSdecoding(VOID);
extern CONST VOID spp_EqualizeNBurst(VOID);
extern CONST VOID patch_spp_EqualizeNBurst(VOID);
extern CONST VOID spp_SearchFcch(VOID);
extern CONST VOID patch_spp_SearchFcch(VOID);
extern CONST VOID spal_IfcDisableRf(VOID);
extern CONST VOID patch_spal_IfcDisableRf(VOID);
CONST VOID (*spc_FcchProcess_part1)(VOID) = (CONST VOID (*)(VOID))0x81e11691;
extern CONST VOID patch_spc_FcchProcess_part1(VOID);

// For AFC deugging
extern CONST VOID patch_spp_SearchFcch_Old(VOID);
extern CONST VOID patch_spp_SearchFcch_Cmb(VOID);




// =============================================================================
// patch_Open
// -----------------------------------------------------------------------------
/// This function install all the hardware patch available for the current chip
// =============================================================================
PUBLIC VOID patch_Open(VOID)
{
    // NOTE:
    // The macros are defined in make.srcs

    // For BCPU buffer profiling
    spal_RegisterProfileBuffer();
#ifdef ENABLE_PATCH_BB_PROG_RX_WIN
    patch_ReplaceFunction(bb_prog_rx_win, patch_bb_prog_rx_win);
#endif // ENABLE_PATCH_BB_PROG_RX_WIN
#ifdef ENABLE_PATCH_SPAL_IRQHANDLER
    patch_ReplaceFunction(spal_IrqHandler, patch_spal_IrqHandler);
#endif // ENABLE_PATCH_SPAL_IRQHANDLER

    // For Rx/Tx debugging
#ifdef ENABLE_PATCH_SPC_RXPROCESS
    patch_ReplaceFunction(spc_RxProcess, patch_spc_RxProcess);
#endif // ENABLE_PATCH_SPC_RXPROCESS
#ifdef ENABLE_PATCH_SPP_BUILDNBURST
    patch_ReplaceFunction(spp_BuildNBurst, patch_spp_BuildNBurst);
#endif // ENABLE_PATCH_SPP_BUILDNBURST

    // ----------------------------------------

    // Fix preample bit error in Tx
#ifdef ENABLE_PATCH_SPP_MODULATION
    patch_ReplaceFunction(spp_Modulation, patch_spp_Modulation);
#endif // ENABLE_PATCH_SPP_MODULATION

    // Fix snapshot number for 2nd win issue
#ifdef ENABLE_PATCH_SPC_IRQHANDLER
    patch_ReplaceFunction(spc_IrqHandler, patch_spc_IrqHandler);
#endif // ENABLE_PATCH_SPC_IRQHANDLER
#ifdef ENABLE_PATCH_SPC_SCHEDULERMAIN
    patch_ReplaceFunction(spc_SchedulerMain, patch_spc_SchedulerMain);
#endif // ENABLE_PATCH_SPC_SCHEDULERMAIN

    // Fix AMR issue
#ifdef ENABLE_PATCH_SPC_AFSDECODING
    patch_ReplaceFunction(spc_AFSdecoding, patch_spc_AFSdecoding);
#endif // ENABLE_PATCH_SPC_AFSDECODING
#ifdef ENABLE_PATCH_SPC_AHSDECODING
    patch_ReplaceFunction(spc_AHSdecoding, patch_spc_AHSdecoding);
#endif // ENABLE_PATCH_SPC_AHSDECODING

    // Fix NB equalization issue
#ifdef ENABLE_PATCH_SPP_EQUALIZENBURST
    patch_ReplaceFunction(spp_EqualizeNBurst, patch_spp_EqualizeNBurst);
#endif // ENABLE_PATCH_SPP_EQUALIZENBURST

    // Fix IFC2 disabling issue
#ifdef ENABLE_PATCH_SPAL_IFCDISABLERF
    patch_ReplaceFunction(spal_IfcDisableRf, patch_spal_IfcDisableRf);
    if (g_bootRomVersionNumber == GALLITE_8805_ROM_VERSION)
    {
        // Do not disable IFC2 in spc_FcchProcess
        patch_ReplaceFunction(spc_FcchProcess_part1, patch_spc_FcchProcess_part1);
    }
#endif // ENABLE_PATCH_SPAL_IFCDISABLERF


#ifdef ENABLE_PATCH_SPP_SEARCHFCCH_CMB
    patch_ReplaceFunction(spp_SearchFcch, patch_spp_SearchFcch_Cmb);
#else

    // Improve FCCH searching efficiency
#ifdef ENABLE_PATCH_SPP_SEARCHFCCH
    patch_ReplaceFunction(spp_SearchFcch, patch_spp_SearchFcch);
#endif // ENABLE_PATCH_SPP_SEARCHFCCH
   // Only for Fcch Search @AFC Locked debugging
#ifdef ENABLE_PATCH_SPP_SEARCHFCCH_OLD
    patch_ReplaceFunction(spp_SearchFcch, patch_spp_SearchFcch_Old);
#endif

#endif




}

