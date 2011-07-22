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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_ana_pa.c $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///     @file  hal_ana_pa.c                                             
///     Implementation of Garnet's PA Ramps Driver              
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "global_macros.h"
#include "pa_ctrl.h"

#include "hal_ana_gpadc.h"
#include "hal_mem_map.h"

#include "hal_sys.h"
#include "halp_sys.h"
#include "hal_ana_pa.h"

//UINT16 hanning_win[16] = {0,10,41,92,159,241,334,434,538,640,737,825,900,960,1001,1023};
//UINT16 linear_win[16] = {0,50,60,70,80,150,350,550,850,963,973,983,993,1003,1013,1023};
//UINT16 blackman_win[16] = {0,4,16,38,73,123,190,274,375,487,605,723,831,922,987,1023};

//#define UNCACHE(n) (UINT32)(*((UINT32*)KSEG1((UINT32)&n)))

#define LOW_VOLTAGE_CORRECTION

// Used to set the three values by the same time
#define PA_RAMP(val0, val1, val2)            (PA_CTRL_VALUE_2(val2) | PA_CTRL_VALUE_1(val1) | PA_CTRL_VALUE_0(val0))
#define PA_RAMP_LAST(val)                    (val)


// =============================================================================
// hal_AnaPaSetRamp
// -----------------------------------------------------------------------------
/// Program a ramp in the PA controller
/// The controller can store up to 5 ramps, which 
/// are indexed by the \c .id field of the #HAL_ANA_PA_RAMP_T
/// structure
///
/// @param ramp The ramp to program. 
// =============================================================================
PUBLIC VOID HAL_FUNC_INTERNAL hal_AnaPaSetRamp(HAL_ANA_PA_RAMP_T ramp)
{    
    int i = 0;
    
    for (i=0;i<5;i++)
    {
        UINT16 ramp1 = ramp.rampDacVal[3*i];
        UINT16 ramp2 = ramp.rampDacVal[3*i+1];
        UINT16 ramp3 = ramp.rampDacVal[3*i+2];

        hwp_paCtrl->Ramp[6*ramp.id+i] = PA_RAMP(ramp1,ramp2,ramp3);
    }
    hwp_paCtrl->Ramp[6*ramp.id+5] = PA_RAMP_LAST(ramp.rampDacVal[15]);
}



// =============================================================================
// hal_AnaPaSetRampCalib
// -----------------------------------------------------------------------------
/// This is a "calibration only" version of the ramp
/// driver. It generates 2 ramps of linear DAC values
/// between minDac and dacValue.
///
/// @param minValue Minimum value, start of the first ramp, end of the second.
/// @param dacValue Summit Dac Value, end of the first ramp and start of the
/// second.
// =============================================================================
PUBLIC VOID hal_AnaPaSetRampCalib (UINT16 minDac,UINT16 dacValue)
{
    int i;
    UINT16 dacRamp[16];
    int Ramp_id = 0;
    
    for (i=0;i<16;i++)
    {
        //dacRamp[i] = minDac + (((dacValue - minDac) * i) / 15);
        dacRamp[i] = dacValue;
    }

    // Rising
    for (i=0;i<5;i++)
    {
        hwp_paCtrl->Ramp[6*Ramp_id+i] = 
            PA_RAMP((dacRamp[3*i]),
                    (dacRamp[3*i+1]),
                    (dacRamp[3*i+2]));
    }
    hwp_paCtrl->Ramp[6*Ramp_id+5] = PA_RAMP_LAST(dacRamp[15]);
    
    // Falling
    Ramp_id = 1;
    for (i=0;i<5;i++)
    {
        hwp_paCtrl->Ramp[6*Ramp_id+i] = 
            PA_RAMP((dacRamp[15-(3*i)]),
                    (dacRamp[15-(3*i+1)]),
                    (dacRamp[15-(3*i+2)]));
    }
    hwp_paCtrl->Ramp[6*Ramp_id+5] = PA_RAMP_LAST(dacRamp[0]);

}

// =============================================================================
// hal_AnaPaOpen
// -----------------------------------------------------------------------------
/// Configure and enable the PA controller
///
/// @param rampOn16Qbits If \c TRUE,  the ramp duration
/// is set to 16 quarter bits, with a new value every quarter bit \n
///                        If \c FALSE, the ramp duration
/// is set to 32 quarter bits, with a new value every other quarter bit.
// =============================================================================
PUBLIC VOID hal_AnaPaOpen(BOOL rampOn16Qbits)
{
    hwp_paCtrl->ctrl = (PA_CTRL_ENABLE_ENABLE) |
    (rampOn16Qbits?PA_CTRL_RAMP_DURATION_16QBITS:PA_CTRL_RAMP_DURATION_32QBITS);// |
    //PA_CTRL_FORCE_H | // FIXME:Forced for testing 
    //(PA_CTRL_FORCED_VAL(0x2a5)); 

    // Set the resource as active
    hal_SysRequestFreq(HAL_SYS_FREQ_PA, HAL_SYS_FREQ_26M, NULL);
}



// =============================================================================
// hal_AnaPaClose
// -----------------------------------------------------------------------------
/// Disable the PA controller
// =============================================================================
PUBLIC VOID hal_AnaPaClose(VOID)
{
      hwp_paCtrl->ctrl = (PA_CTRL_ENABLE_DISABLE) |
        0 |
        PA_CTRL_FORCE_H | // FIXME:Forced for testing 
        (PA_CTRL_FORCED_VAL(0x2a5)); 

    // release the resource
    hal_SysRequestFreq(HAL_SYS_FREQ_PA, HAL_SYS_FREQ_32K, NULL);
}

// =============================================================================
// hal_AnaPaSleep
// -----------------------------------------------------------------------------
/// Releases the resource
// =============================================================================
PROTECTED VOID hal_AnaPaSleep(VOID)
{
    hal_SysRequestFreq(HAL_SYS_FREQ_PA, HAL_SYS_FREQ_32K, NULL);
}


// =============================================================================
// hal_AnaPaWakeup
// -----------------------------------------------------------------------------
/// Set the resource as active
// =============================================================================
PROTECTED VOID hal_AnaPaWakeup(VOID)
{
    hal_SysRequestFreq(HAL_SYS_FREQ_PA, HAL_SYS_FREQ_26M, NULL);
}

