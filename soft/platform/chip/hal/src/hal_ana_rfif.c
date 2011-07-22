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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_ana_rfif.c $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_ana_rfif.c                                                    
/// This file contains the RF interface driver. It is used to enable the      //
/// analog macro.                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "cs_types.h"

#include "global_macros.h"
#include "ana_acco.h"

#include "hal_ana_pa.h"
#include "hal_sys.h"
#include "halp_sys.h"
#include "halp_debug.h"

PUBLIC VOID hal_AnaRfIfTurnOn(VOID)
{
    hwp_anaAccoFe->rf_ctrl_set = ANA_ACCO_TX_DAC_ENABLE
                            | ANA_ACCO_RX_AAF_BYPASS
                            | ANA_ACCO_RF_DEEP_PD_RELEASE
                            | ANA_ACCO_RF_RESET_RELEASE ;



    hwp_anaAccoFe->pa_afc_gp_ctrl_set = ANA_ACCO_PADAC_RESET_RELEASE
                            | ANA_ACCO_PADAC_ENABLE ;                                
    
    // Init PA DAC 
    hal_AnaPaOpen(TRUE);
    
    // Set Global RF resource as active
    hal_SysRequestFreq(HAL_SYS_FREQ_RF, HAL_SYS_FREQ_26M, NULL);
    //HAL_TRACE(_PAL|TSTDOUT, 0,"RF IF turned ON");
}


PUBLIC VOID hal_AnaRfIfTurnOff(VOID)
{
    // Disable PA DAC
    hal_AnaPaClose();
    
    // NOTE: do not power down AFC here as it's still in use,
    // it is powered down and up in hal_lowpower (?)

    // Send PA DAC to powerdown
    hwp_anaAccoFe->pa_afc_gp_ctrl_clr = ANA_ACCO_PADAC_RESET_N
                                | ANA_ACCO_PADAC_DISABLE;

    // NOTE: do not use RF_DEEP_PD or it will also power down the PLL
    // moreover when PLL is off RF_DEEP_PD does not reduce the current anymore
    // Turn off RX ADC and TX DAC


    hwp_anaAccoFe->rf_ctrl_clr = ANA_ACCO_TX_DAC_DISABLE
                                | ANA_ACCO_RF_RESET_N;

    // Set Global RF resource as inactive
    hal_SysRequestFreq(HAL_SYS_FREQ_RF, HAL_SYS_FREQ_32K, NULL);
}



