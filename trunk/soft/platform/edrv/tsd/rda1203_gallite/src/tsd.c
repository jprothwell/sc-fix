///////////////////////////////////////////////////////////////////////////////
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
//  $HeadURL:                                                                 //
//  $Author: chenq $                                                       //
//  $Date:                                                                    //
//  $Revision:                                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///                                                                           //
///     @file tsd.h                                                           //
///                                                                           //
///     Touch panel driver                                                    //
///     opal                                                                  //
///                                                                           //
//////////////////////////////////////////////////////////////////////////////// 


#include "sxr_ops.h"
#include "sxs_io.h"
#include "cs_types.h"
#include "sxr_tls.h"
#include "tsdp_debug.h"
#include "tsd_config.h"
#include "tsd_m.h"
#include "tgt_tsd_cfg.h"
#include "tsdp_calib.h"
#include "hal_host.h"
#include "opal.h"

// ============================================================================
// LOCAL DEFINITIONS
// ============================================================================

// Number of samples we ask the touch screen controller for each coordinate.
// We need more than one because of the touch screen settling time.
// Indeed, the voltage across the touch panel "ring" and then settle down due
// to mechanical bouncing and switch driver parasiting.
// From 1 to 4 : 4 is obviously the more precise.
// well 4 is too long, settling time of switch is not relevant with opal, as opal
// switch between each measure, we keep 3 here to have some filtering, but 1 would
// probably be nice enough for Opal tsd, as it seams fairly stable !
// Security would choose 3
// According to the test, we choose 3 now. You may easily find unstable pen tracking during
// the pen keeping pressing on the same position, especially while handwriting.
#define NUMBER_SAMPLES_PER_POINT 3


// With 12-bit Conversion, keeping the tenth most significant bits.
#define MAX_ADC_VAL    0x3ff

// Flag to indicate if the tsd_GetRawPoints succeeded or not.
#define SUCCESSFUL   1
#define NOT_SUCCESSFUL   0
#define SAMPLE_VALID 3


#define READ_X_AXIS  1
#define READ_Y_AXIS  2

// Parameter of sxr_StartFunctionTimer
// #define SXR_REGULAR_TIMER 0x03

PRIVATE VOID tsd_Debounce(VOID);

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

// TSD config
PRIVATE CONST TSD_CONFIG_T*  g_tsdConfig;

// HAL GPIO CFG
PRIVATE HAL_GPIO_CFG_T       g_gpioCfg;
PRIVATE BOOL                 g_gpioIrqOn;

// Tab of NUMBER_SAMPLES_PER_POINT TSC_POINT samples.
PRIVATE TSD_POINT_T          g_tabSamples[NUMBER_SAMPLES_PER_POINT];

PRIVATE TSD_POINT_T          g_tabCalibratedExpectedPoint [3];

PRIVATE TSD_POINT_T          g_tabCalibratedMeasuredPoint [3];

PRIVATE TSD_MATRIX_T         g_calibrationMatrix;


// This variable is a counter used in #debounce() function to count how many 
// period of g_tsdConfig->debounceTime msec the pen state hold the same value.
PRIVATE UINT8                g_count;

PRIVATE BOOL                 g_isCalibSuccess = TRUE;

// This variable stores the previous pen state as it has been detected by the 
// #debounce() function and is used to detect a change in the pen state.
// When 0, the pen is considered not touching the screen. 
// When 1, the pen is considered touching the screen.
PRIVATE UINT32 g_previousPenIrqPinState = 0;

PRIVATE UINT32 g_isCalibrationDone = 0;

PRIVATE TSD_CALLBACK_T          g_userCallback;

PRIVATE TSD_REPORTING_MODE_T    g_userReportingMode = { FALSE, FALSE, FALSE, 0};

PRIVATE TSD_PEN_STATE_T         g_penState = TSD_PEN_UP;

PRIVATE TSD_PEN_STATE_T         g_lastSendPenState = TSD_PEN_UP;

// ============================================================================
//  tsd_DisableHostPenIrq
// ----------------------------------------------------------------------------
/// This function disables host pen irq. 
// ============================================================================
PRIVATE VOID tsd_DisableHostPenIrq(VOID)
{    EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd:  tsd_DisableHostPenIrq ");
    // set irq as currently disabled
    g_gpioIrqOn = FALSE;
    // actually stop irq
    g_gpioCfg.irqMask.rising= FALSE;
    hal_GpioIrqSetMask(g_tsdConfig->penGpio, &g_gpioCfg.irqMask);
    /// Addind Reset IRQ to avoid unwanted IRQ
    hal_GpioResetIrq(g_tsdConfig->penGpio);
}



// ============================================================================
//  tsd_ClearTouchIrq
// ----------------------------------------------------------------------------
/// This function enables host pen irq. 
// ============================================================================
VOID tsd_ClearTouchIrq(VOID)
{
    EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd:  tsd_ClearTouchIrq ");

    if (FALSE == pmd_TsdClearTouchIrq())
    {
        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_ClearTouchIrq,(VOID*)NULL,0x03);
        return;

    }

    EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd:  tsd_ClearTouchIrq end ");

}

// ============================================================================
//  tsd_EnableHostPenIrq
// ----------------------------------------------------------------------------
/// This function enables host pen irq. 
// ============================================================================
VOID tsd_EnableHostPenIrq(VOID)
{
    EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd:  tsd_EnableHostPenIrq ");

    if (FALSE == pmd_ResetTouch())
    {
        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_EnableHostPenIrq,(VOID*)NULL,0x03);
        return;

    }

    // filter ou previous in queue Irq
    hal_GpioResetIrq(g_tsdConfig->penGpio);
    // set irq as currently enabled
    g_gpioIrqOn = TRUE;
    // but only if user want some events
        // at least one kind of events enabled, we need to allow pen Irq
    g_gpioCfg.irqMask.rising = TRUE;
    if(1==hal_GpioGet(g_tsdConfig->penGpio))   
    {
        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03);
        return;
    }
    else
    {
        hal_GpioIrqSetMask(g_tsdConfig->penGpio, &g_gpioCfg.irqMask);
    }
    if(1==hal_GpioGet(g_tsdConfig->penGpio)&&(g_gpioIrqOn==TRUE))   
    {
        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03);
    }
    EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd:  tsd_EnableHostPenIrq end ");

}


// ============================================================================
//  tsd_CoordFromMode
// ----------------------------------------------------------------------------
/// This function returns the &coordinate corresponding to the mode_xy
// ============================================================================
PRIVATE inline UINT32* tsd_CoordFromMode(TSD_POINT_T* sample,UINT8 mode_xy)
{
    switch(mode_xy)
    {
        case READ_X_AXIS :
            return &sample->x;
            break;
        case READ_Y_AXIS:
            return &sample->y;
            break;
        default:
            break;
    }
    return((UINT32*)1);
}




// ============================================================================
//  tsd_GetSamples
// ----------------------------------------------------------------------------
// ============================================================================
PRIVATE TSD_ERR_T tsd_GetSamples(TSD_POINT_T* TabSamples)
{   
    //EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd:  tsd_GetSamples ");

    UINT16 i,k;
    UINT16 xy[2];
    BOOL result;
    TSD_PROFILE_FUNCTION_ENTRY(tsd_GetSamples);

   // if(FALSE == pmd_OpalSpiActivateCs())
    if(FALSE == pmd_TSDGetSPI())
    {
        TSD_PROFILE_FUNCTION_EXIT(tsd_GetSamples);
        return TSD_ERR_RESOURCE_BUSY;
    }
    
    for(i = 0;i < NUMBER_SAMPLES_PER_POINT;i++)
    { 
        k=0;
        do{
            result = pmd_TsdReadCoordinatesInternal(&xy[0],&xy[1]);
            k++;
        }while ((result == FALSE) &&( k < 10));

        if(result == FALSE)
        {
            break;
        }
        EDRV_TRACE(EDRV_TSD_TRC, 0, "TSD OPAL  x=0x%04X, y=0x%02X,i=%d ",xy[0], xy[1],i);
        pmd_TsdClearEomIrqInternal();
        *(tsd_CoordFromMode(&TabSamples[i], READ_X_AXIS)) = xy[0];
        *(tsd_CoordFromMode(&TabSamples[i], READ_Y_AXIS)) = xy[1];
    }

    hal_TimDelay(1);
    pmd_TsdClearTouchIrqInternal();
    pmd_TSDReleaseSPI();
    TSD_PROFILE_FUNCTION_EXIT(tsd_GetSamples);
    if(result == FALSE)
        return TSD_ERR_RESOURCE_BUSY;
    return TSD_ERR_NO;
}

// ============================================================================
//  tsd_EvaluateSamples
// ----------------------------------------------------------------------------
/// Each time,
/// on a pen down, NUMBER_SAMPLES_PER_POINT pairs XY from the ADC are collected.
/// NUMBER_SAMPLES_PER_POINT=4 :
/// The first sample is discarding (less time of acquisition) and the 3 others 
/// will be used here. This function implements the algorithm to the best sample 
/// from 3 pairs of samples by discarding one that is too way out and take a mean
/// of the rest two.
/// NUMBER_SAMPLES_PER_POINT=3 :
/// The same thing but we take in consideration the first sample.
/// NUMBER_SAMPLES_PER_POINT=2 :
/// Average of the two samples.
/// NUMBER_SAMPLES_PER_POINT=1 :
/// No evaluation here, we take the keep the only sample we have.
///
/// @param TabSamples the sampled data
/// @param sample where to fill an evaluation of the sample(s).
/// @param mode_xy selects the coordinate on wich we apply the evaluation
/// @return \c TRUE if the evaluation is valid

/// Evaluate both samples, 4 X's and 4 Y's.
/// Use global var g_samplesX, g_samplesY instead mode_xy
// ============================================================================
PRIVATE BOOL tsd_EvaluateSamples (TSD_POINT_T* TabSamples,UINT32 *sample,UINT8 mode_xy)
{

    UINT16 val0, val1, val2, diff0, diff1, diff2;
    BOOL retval = FALSE;

    TSD_PROFILE_FUNCTION_ENTRY(tsd_EvaluateSamples);

    switch(NUMBER_SAMPLES_PER_POINT)
    {
        case 4:

            // We discard the first sample : The one with the less time of acquisition. 
            val0=*tsd_CoordFromMode(&TabSamples[1],mode_xy);
            val1=*tsd_CoordFromMode(&TabSamples[2],mode_xy);
            val2=*tsd_CoordFromMode(&TabSamples[3],mode_xy);

            // We check if there is no flagrant mistake during the transfert by the SPI.
            if ((val0 <= MAX_ADC_VAL) && (val1 <= MAX_ADC_VAL) && (val2 <= MAX_ADC_VAL) 
                    && (val0 > 0) && (val1 > 0) && (val2 > 0))
            {
                // Calculate the absolute value of the differences of the samples.
                diff0 = (val0> val1)  ? (val0 -val1) : (val1 - val0);
                diff1 = (val1> val2)  ? (val1 -val2) : (val2 - val1);
                diff2 = (val2> val0)  ? (val2 -val0) : (val0 - val2);

                // We estimate the average valid if the difference between two samples are less than g_tsdConfig->maxError.
                if ((diff0 < g_tsdConfig->maxError) && (diff1 < g_tsdConfig->maxError) && (diff2 < g_tsdConfig->maxError))
                {
                    retval = TRUE;
                    // Eliminate the one away from other two and add the two others.
                    if (diff0 < diff1)
                    {
                        *sample=(UINT16)(val0 + ((diff2 < diff0) ? val2 : val1));
                    }
                    else
                    {
                        *sample=(UINT16)(val2 + ((diff2 < diff1) ? val0 : val1));
                    }
                    // Get the average of the two good samples.
                    *sample>>=1;
                } 
                else
                {   
                    SXS_TRACE(TSTDOUT," Samples not valid (>= maxError: 0x%X)", g_tsdConfig->maxError);
                }

            }    
            break;
            
            case 3:
            
            // We take in consideration all samples. 
            val0=*tsd_CoordFromMode(&TabSamples[0],mode_xy);
            val1=*tsd_CoordFromMode(&TabSamples[1],mode_xy);
            val2=*tsd_CoordFromMode(&TabSamples[2],mode_xy);

            // We check if there is no flagrant mistake during the transfert by the SPI.
            if ((val0 <= MAX_ADC_VAL) && (val1 <= MAX_ADC_VAL) && (val2 <= MAX_ADC_VAL)
                    && (val0 > 0) && (val1 > 0) && (val2 > 0))
            {
                // Calculate the absolute value of the differences of the samples.
                diff0 = (val0> val1)  ? (val0 -val1) : (val1 - val0);
                diff1 = (val1> val2)  ? (val1 -val2) : (val2 - val1);
                diff2 = (val2> val0)  ? (val2 -val0) : (val0 - val2);

                // We estimate the average valid if the difference between two samples are less than g_tsdConfig->maxError.
                if ((diff0 < g_tsdConfig->maxError) && (diff1 < g_tsdConfig->maxError) && (diff2 < g_tsdConfig->maxError))
                {
                    retval = TRUE;
                    // Eliminate the one away from other two and add the two others.
                    if (diff0 < diff1) 
                    {    
                        *sample=(UINT16)(val0 + ((diff2 < diff0) ? val2 : val1));
                    }
                    else
                    {
                        *sample=(UINT16)(val2 + ((diff2 < diff1) ? val0 : val1));
                    }
                    // Get the average of the two good samples.
                    *sample>>=1;
                }
                else
                {   
                    SXS_TRACE(TSTDOUT," Samples not valid with g_tsdConfig->maxError ! ");
                }

            }    
            break;

        case 2:
            
            // We take in consideration all samples. 
            val0=*tsd_CoordFromMode(&TabSamples[0],mode_xy);
            val1=*tsd_CoordFromMode(&TabSamples[1],mode_xy);

            // We check if there is no flagrant mistake during the transfert by the SPI.
            if ((val0 <= MAX_ADC_VAL) && (val1 <= MAX_ADC_VAL)
                    && (val0 > 0) && (val1 > 0))
            {
                // Calculate the absolute value of the differences of the samples.
                diff0 = (val0> val1)  ? (val0 -val1) : (val1 - val0);
                
                // We estimate the average valid if the difference between two samples are less than g_tsdConfig->maxError.
                if (diff0 < g_tsdConfig->maxError)
                {
                    retval = TRUE;
                    *sample=(UINT16)(val0 + val1);
                    // Get the average of the two good samples.
                    *sample>>=1;
                }
                else
                {   
                    SXS_TRACE(TSTDOUT," Samples not valid with g_tsdConfig->maxError ! ");
                }

            }    
            break;

        case 1:

            val0=*tsd_CoordFromMode(&TabSamples[0],mode_xy);
            // We check if there is no flagrant mistake during the transfert by the SPI.
            if ((val0 <= MAX_ADC_VAL) && (val0 > 0))
            {
                retval = TRUE;
                *sample=(UINT16)(val0);
            }                
                
            break;

        default:
            break;         
    }
    TSD_PROFILE_FUNCTION_EXIT(tsd_EvaluateSamples);
    return(retval);
}


// ============================================================================
//  tsd_GetScreenPoints
// ----------------------------------------------------------------------------
/// Get X and Y samples from the Touch Screen, evaluate them, then store
/// the average of valid raw samples: the raw point corresponding to the point
/// touched on the touch screen.
/// This raw point presents distortion errors, so has to be calibrated.
///
/// @screenPoint will contain raw coordinates if calibration parameters aren't as
/// accurate as we need (calibration process will be re-launched), else it will
/// contain a calibrated coordinates ready for display.
///
/// @return 1 if the function succeeded, 0 otherwise.
// ============================================================================


PRIVATE UINT32 tsd_GetScreenPoints(TSD_POINT_T* screenPoint)
{
    UINT32 retval = NOT_SUCCESSFUL;
    // lcdScreenTouchPoint : Estimated lcd coordinates ( average of samples )
   // but without calibration fixing.
   // lcdScreenTouchPoint.x = 0;
   // lcdScreenTouchPoint.y = 0;
    TSD_POINT_T lcdScreenTouchPoint;
    TSD_PROFILE_FUNCTION_ENTRY(tsd_GetScreenPoints);
    // Get 4 X samples and 4 Y samples and store them to g_tabSamples
    if (tsd_GetSamples(g_tabSamples) == TSD_ERR_RESOURCE_BUSY)
    {
        return NOT_SUCCESSFUL;
    }

  
    // Then, If the samples are valid,evaluate them and use matrix calibration to fix them.
    if ( tsd_EvaluateSamples(g_tabSamples,&(lcdScreenTouchPoint.x),READ_X_AXIS)
        && tsd_EvaluateSamples(g_tabSamples,&(lcdScreenTouchPoint.y),READ_Y_AXIS))
    {  

        SXS_TRACE(TSTDOUT,"tsd_GetScreenPoints: calib: x=%d, y=%d ",lcdScreenTouchPoint.x,lcdScreenTouchPoint.y);

        retval = SAMPLE_VALID;
        if( g_isCalibrationDone == 1)
        {
            retval = SUCCESSFUL;
            // Provide calibrated coordinates ready for display
            GetDisplayPoint(screenPoint,&lcdScreenTouchPoint,&g_calibrationMatrix);
            
         //       SXS_TRACE(TSTDOUT,"calib:  tsd 2 x = %d   y=%d ",lcdScreenTouchPoint.x,lcdScreenTouchPoint.y);
         //       SXS_TRACE(TSTDOUT,"calib:  tsd 3 x = %d   y=%d  %d ",screenPoint->x,screenPoint->y,retval2);
         //       SXS_TRACE(TSTDOUT,"calib:  1= %d 2=  %d ",g_calibrationMatrix.An,g_calibrationMatrix.Bn);           
         //       SXS_TRACE(TSTDOUT,"calib: 3= %d 4= %d  ",g_calibrationMatrix.Cn,g_calibrationMatrix.Dn);         
        //        SXS_TRACE(TSTDOUT,"calib:  5=  %d 6= %d 7=%d  ",g_calibrationMatrix.En,g_calibrationMatrix.Fn,g_calibrationMatrix.Divider);
              
            if(!g_isCalibSuccess)
            {
                // If calibration matrix not accurate, raw point will be
                // delivered for a new calibration process
                screenPoint->x = lcdScreenTouchPoint.x;
                screenPoint->y = lcdScreenTouchPoint.y;
            }
        }
        else
        {
            // If the calibration has not been done yet, we return raw coordinate with scaling,rotating errors.
            screenPoint->x = lcdScreenTouchPoint.x;
            screenPoint->y = lcdScreenTouchPoint.y;
        }

    }

    TSD_PROFILE_FUNCTION_EXIT(tsd_GetScreenPoints);
    return(retval);
}


// ============================================================================
//  tsd_SetCalibStatus
// ----------------------------------------------------------------------------
/// Set the calibration status
/// @param isCalibrated \c TRUE when calibrated, \c FALSE to restart calibration
/// Once the status is set to \c TRUE, the callback #TSD_CALLBACK_T will be
/// called with corrected values.
/// note that tsd_SetCalibPoints() must be called before setting the calibration
/// to \c TRUE
// ============================================================================
PUBLIC VOID tsd_SetCalibStatus( BOOL isCalibStatus)
{
    g_isCalibSuccess = isCalibStatus;
}

// ============================================================================
//  tsd_Debounce
// ----------------------------------------------------------------------------
/// This function debounces signal coming from pen irq gpio pin.
/// It return a raw point coordinates to the g_userCallback function which will
/// do calibration process.
// ============================================================================
PRIVATE VOID tsd_Debounce(VOID)
{

    TSD_POINT_T          screenPoint;
    TSD_PROFILE_FUNCTION_ENTRY(tsd_Debounce);
    UINT32 resultOfsample = 0;
    UINT32 tsdClearState=0;
    UINT32 currentPenIrqPinState = hal_GpioGet(g_tsdConfig->penGpio);
    EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_Debounce: curState=%d, prevState=%d",
        currentPenIrqPinState, g_previousPenIrqPinState);

    static int count = 0;
    count++;
    //g_previousPenIrqPinState = currentPenIrqPinState;
    UINT32 irqState;
    if (!pmd_OpalSpiActivateCs())
    {
        EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_Debounce: ISPI busy");
        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03);
        return;
    }
    irqState = pmd_OpalSpiRead(RDA_ADDR_IRQ_SETTINGS);
    EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_Debounce: EOM=%d, PENIRQ=%d",
        (irqState&(1<<15))!=0, (irqState&(1<<14))!=0);
    //pmd_OpalSpiWrite(RDA_ADDR_IRQ_SETTINGS,
    //    irqState|RDA1203_PENIRQ_CLEAR|RDA1203_EOMIRQ_CLEAR);
    pmd_OpalSpiDeactivateCs();
#if 0
    if (count > 100)
    {
        EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_Debounce: finish tracing & reenable GPIO irq");
        count = 0;
        tsd_EnableHostPenIrq();
    }
    else
    {
        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03);
    }
    return;
#endif

    if(currentPenIrqPinState != g_previousPenIrqPinState)
    {
        // The pen state has just changed : We reset the counter.
        g_count = 0;
        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03);
    }
    else
    {
        // The pen state does not change.
        g_count++;
        switch (g_penState)
        {
            case TSD_PEN_DOWN :
            {
                  EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_Debounce:  TSD_PEN_DOWN ");

                // If we reach DEBOUNCE_UP_TIME_MSEC and if PenIrqPinState is high.
                if( (g_count >= g_tsdConfig->upPeriod) && (currentPenIrqPinState == 0) )
                {
                    // We consider the pen down.
                    g_penState = TSD_PEN_UP;
                    // We reset the counter.
                    g_count = 0;

                    // We calculate the coordinates of the touch screen point.

                    // If the user wants a call to g_userCallback function.
                    if (g_userReportingMode.onUp)
                    {
                        if(g_lastSendPenState == TSD_PEN_DOWN || g_lastSendPenState == TSD_PEN_PRESSED)
                        {
                            g_userCallback(&screenPoint, g_penState);
                            g_lastSendPenState = TSD_PEN_UP;
                        }
                    }
                    // We enable the host pen irq and do not reload the timer.
                    tsdClearState =1;
                }
                else 
                {
                    // If we reach g_userReportingMode.repetitionPeriod and if PenIrqPinState is low.
                    if( (g_count >= g_userReportingMode.repetitionPeriod) && (currentPenIrqPinState == 1) )
                    {
                        // We consider the pen down.
                        g_penState = TSD_PEN_PRESSED;
                        // We reset the counter.
                        g_count = 0;

                        // We calculate the coordinates of the touch screen point.

                        // If the user wants a call to g_userCallback function.
                        resultOfsample = tsd_GetScreenPoints(&screenPoint);
                        if (g_userReportingMode.onPressed)
                        {
                            if(resultOfsample != NOT_SUCCESSFUL)
                            {
                                g_userCallback(&screenPoint,g_penState);
                                g_lastSendPenState = TSD_PEN_PRESSED;
                            }
                        }
                    }

                    // We reload the timer.
                    sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03); 
                }

                break;
            }

            case TSD_PEN_UP :
            {
                EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_Debounce:  TSD_PEN_UP");

                if(currentPenIrqPinState == 1)
                {
                    if (g_count >= g_tsdConfig->downPeriod)
                    {
                        // We consider the pen down.
                        g_penState = TSD_PEN_DOWN;
                        // We reset the counter.
                        g_count=0;

                        // We calculate the coordinates of the touch screen point.

                        // If the user wants a call to g_userCallback function.
                        resultOfsample = tsd_GetScreenPoints(&screenPoint);
                        if (g_userReportingMode.onDown)
                        {
                            if(resultOfsample != NOT_SUCCESSFUL)
                            {
                                g_userCallback(&screenPoint, g_penState);
                                g_lastSendPenState = TSD_PEN_DOWN;
                            }
                            else
                            {
                                g_penState = TSD_PEN_UP;                        
                            }
                        }
                    }

                    sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03);
                }
                else
                {
                    if ( g_count <= g_tsdConfig->upPeriod )
                    {
                        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03);
                    }
                    else
                    {
                        g_count = 0;
                        tsdClearState=1;
                    }
                }
                break;

            }

            case TSD_PEN_PRESSED :
            {   
                EDRV_TRACE(EDRV_TSD_TRC, 0, "tsd_Debounce:  TSD_PEN_PRESSED");

                // If we reach DEBOUNCE_UP_TIME_MSEC and if PenIrqPinState is high.
                if( (g_count >= g_tsdConfig->upPeriod) && (currentPenIrqPinState == 0) )
                {
                    // We consider the pen up.
                    g_penState = TSD_PEN_UP;
                    // We reset the counter.
                    g_count = 0;

                    // We calculate the coordinates of the touch screen point.

                    // If the user wants a call to g_userCallback function.
                    if (g_userReportingMode.onUp)
                    {
                        if(g_lastSendPenState == TSD_PEN_DOWN || g_lastSendPenState == TSD_PEN_PRESSED)
                        {
                            g_userCallback(&screenPoint, g_penState);
                            g_lastSendPenState = TSD_PEN_UP;
                        }
                    }
                    // We enable the host pen irq and do not reload the timer.
                    tsdClearState=1;                    
                }
                else
                {
                    if( (g_count >= g_userReportingMode.repetitionPeriod) && (currentPenIrqPinState==1) )
                    {
                        // We consider the pen still pressed and we reset the counter.
                        g_count=0;

                        // We calculate the coordinates of the touch screen point.

                        // If the user wants a call to g_userCallback function.
                        resultOfsample = tsd_GetScreenPoints(&screenPoint);
                        if (g_userReportingMode.onPressed)
                        {
                            if(resultOfsample != NOT_SUCCESSFUL)
                            {
                                g_userCallback(&screenPoint,g_penState);
                                g_lastSendPenState = TSD_PEN_PRESSED;
                            }
                        }
                    }

                    // We reload the timer.
                    sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03);
                }

                break;

            }

            default:
            {
            }
        }
    }

    g_previousPenIrqPinState = currentPenIrqPinState;

    tsd_ClearTouchIrq();

    if(tsdClearState == 1)    
       tsd_EnableHostPenIrq();

    TSD_PROFILE_FUNCTION_EXIT(tsd_Debounce);
}


// ============================================================================
//  tsd_GpioIrqHandler
// ----------------------------------------------------------------------------
// This function handles GPIO hard interruptions.
// ============================================================================
PRIVATE VOID tsd_GpioIrqHandler(VOID)
{
    TSD_PROFILE_FUNCTION_ENTRY(tsd_GpioIrqHandler);
    // We check the value of the penIrqPin to avoid false triggering when we reset
    // the touch screen controler.

    if ( hal_GpioGet(g_tsdConfig->penGpio) == 1 )
    {
        // We disable host Pen IRQ while the screen is pressed.
        tsd_DisableHostPenIrq();
        sxr_StartFunctionTimer(g_tsdConfig->debounceTime,tsd_Debounce,(VOID*)NULL,0x03);
    }
    TSD_PROFILE_FUNCTION_EXIT(tsd_GpioIrqHandler);
}



// ============================================================================
//  tsd_Open
// ----------------------------------------------------------------------------
/// This function initializes the touch screen driver. The configuration of the
/// touch screen pins used is board dependent and stored in a TSD_CONFIG_T struct.
// ============================================================================

PUBLIC VOID tsd_Open()
{
    g_gpioIrqOn = TRUE;

    g_tsdConfig = tgt_GetTsdConfig();
    g_gpioCfg.direction = HAL_GPIO_DIRECTION_INPUT;

    // GPIO irq mask cfg
    g_gpioCfg.irqMask.falling = FALSE;
    g_gpioCfg.irqMask.debounce = TRUE;
    g_gpioCfg.irqHandler = tsd_GpioIrqHandler;
    g_gpioCfg.irqMask.level = FALSE;
   // if (g_userReportingMode.onDown 
   //     || g_userReportingMode.onPressed
  //      || g_userReportingMode.onUp)
 //   {
  //      g_gpioCfg.irqMask.rising = TRUE;
  //  }
 //   else
 //   {
  //      g_gpioCfg.irqMask.rising = FALSE;
 //   }
    g_gpioCfg.irqMask.rising = TRUE;
    hal_GpioOpen(g_tsdConfig->penGpio, &g_gpioCfg);

    while(!pmd_TsdEnableIrq())
    {
        sxr_Sleep(10);
    }
    tsd_EnableHostPenIrq();
}


// ============================================================================
//  tsd_Close
// ----------------------------------------------------------------------------
/// This function closes the touch screen driver. Deactivate and close the SPI
// ============================================================================
PUBLIC VOID tsd_Close(VOID)
{
    while(!pmd_TsdDisableIrq())
    {
        sxr_Sleep(10);
    }
    
    hal_GpioClose(g_tsdConfig->penGpio);
    g_gpioIrqOn = FALSE;
}

// ============================================================================
//  tsd_SetCallback
// ----------------------------------------------------------------------------
/// This function configures which user function will be called by the
/// touch screen driver when the callback is invoked. See the function
/// #tsd_SetReportingMode() for details about when the callback is invoked.
/// 
/// @param callback Pointer to a user function called when an interruption
/// is generated by the touch screen driver.
// ============================================================================
PUBLIC VOID tsd_SetCallback(TSD_CALLBACK_T callback)
{
    g_userCallback = callback;
}


// ============================================================================
//  tsd_SetReportingMode
// ----------------------------------------------------------------------------
/// This function controls under which conditions the touch screen interrupt 
/// will be generated. It can be when the pen is pressed, when it is hold down 
/// and when it is released. Use the function #tsd_SetCallback() to configure
/// which user function will be called in case of a touch screen interruption.
///
/// @param mode Defines which event will generate a call to the user
/// callback function. See the documentation of the type for details.
// ============================================================================
PUBLIC VOID tsd_SetReportingMode(TSD_REPORTING_MODE_T* mode)
{
    g_userReportingMode.onDown    = mode->onDown;
    g_userReportingMode.onPressed = mode->onPressed;
    g_userReportingMode.onUp      = mode->onUp;
    g_userReportingMode.repetitionPeriod = mode->repetitionPeriod;

    TSD_ASSERT( (g_userReportingMode.repetitionPeriod != 0) 
                || (!g_userReportingMode.onPressed), 
            "Please set the repetitionPeriod when using onPressed!");
    // don't actually enable IRQ before Open.
    if (g_tsdConfig)
    {
        // depending on wanted events we can disable the IRQ.
        if (g_userReportingMode.onDown 
            || g_userReportingMode.onPressed
            || g_userReportingMode.onUp)
        {
            // at least one kind of events enabled, we need to allow pen Irq
            if (g_gpioIrqOn)
            {   // but only when the timer is not active
                // filter ou previous in queue Irq
                hal_GpioResetIrq(g_tsdConfig->penGpio);
                g_gpioCfg.irqMask.rising= TRUE;
                hal_GpioIrqSetMask(g_tsdConfig->penGpio, &g_gpioCfg.irqMask);
            }
        }
        else
        {
            // no events enabled, stop pen Irq
         //   g_gpioCfg.irqMask.rising= FALSE;
         //   hal_GpioIrqSetMask(g_tsdConfig->penGpio, &g_gpioCfg.irqMask);
        }
    }
}


// ============================================================================
//  tsd_GetReportingMode
// ----------------------------------------------------------------------------
/// This function returns the touch screen reporting mode. Refer to the type 
/// documentation for its interpretation.
///
/// @param mode A pointer to receive the current reporting mode.
// ============================================================================
PUBLIC VOID tsd_GetReportingMode(TSD_REPORTING_MODE_T* mode)
{
    TSD_ASSERT( mode != NULL, "Pointer mode is NULL!");
    mode->onDown    = g_userReportingMode.onDown;
    mode->onUp      = g_userReportingMode.onUp;
    mode->onPressed = g_userReportingMode.onPressed;
    mode->repetitionPeriod = g_userReportingMode.repetitionPeriod;
}


// ============================================================================
//  tsd_SetCalibPoints
// ----------------------------------------------------------------------------
/// This function fixes the calibration parameters of the touch screen driver.
/// From an array of 3 samples per point, it sets an array of
/// 3 Display points and another array of 3 Screen points needed for calibration
///
/// @param calibPoints array of 3 calibration points
//  @return TSD_ERR_INVALID_CALIBRATION if the calibration matrix is invalid.
///
/// This function include a fatal assert that will be triggered if the coordinates
/// of the expected points do not allow a valid calibration (divide by 0)
/// this occurs when the point are aligned, or some have the same value for X or Y
// ============================================================================
PUBLIC TSD_ERR_T tsd_SetCalibPoints(TSD_CALIBRATION_POINT_T calibPoints[3])
{
    UINT8  i;

    // Fix tab of 3 calibration points from tab calibPoints[3] of 3 

    for(i = 0; i < 3; i++)
    {
    // fill tab of 3 samples for each point N°1 N°2 and N°3
        g_tabCalibratedExpectedPoint[i].x = calibPoints[i].expected.x;
        g_tabCalibratedExpectedPoint[i].y = calibPoints[i].expected.y;
        g_tabCalibratedMeasuredPoint[i].x  = calibPoints[i].measured.x;
        g_tabCalibratedMeasuredPoint[i].y  = calibPoints[i].measured.y;
    }

    g_isCalibrationDone = SetCalibrationMatrix(g_tabCalibratedExpectedPoint, g_tabCalibratedMeasuredPoint
                                  , &g_calibrationMatrix);

    if(g_isCalibrationDone)
    { 
        return TSD_ERR_NO;
    }

    return TSD_ERR_INVALID_CALIBRATION;
}

