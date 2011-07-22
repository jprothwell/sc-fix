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
//  $HeadURL$
//	$Author$
//	$Date$
//	$Revision$
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file calib_stub.c                                                        //
/// That file implements the calibration stub mechanisms.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "cs_types.h"
#include "chip_id.h"

#include "calib_m.h"


#include "hal_pwm.h"
#include "hal_ana_gpadc.h"
#include "hal_aif.h"
#include "hal_speech.h"
#include "hal_host.h"

#include "rfd_defs.h"
#include "rfd_xcv.h"
#include "rfd_pa.h"

#include "aud_m.h"
#include "pmd_m.h"
#include "pmd_calib.h"

#include "calibp_debug.h"
#include "calibp_calibration.h"
#include "calibp_stub.h"


#include "sxs_io.h"

#include "myl1.h"
#include "singen.h"



// =============================================================================
//  MACROS
// =============================================================================

/// Reference voltage for the battery. 
#define VREF					2300

// Don't use the screen and the sin generation when compiling the mini version.
#ifndef CES_DISPLAY
#define sinGen(w, x, y, z)
#endif // CES_DISPLAY

#define AUDIO_FRAME_PER_SEC     50

/// Buffer used for the audio needs. 
#ifdef CES_DISPLAY
#define REC_DURATION	        2
#define SECOND_SIZE		        (AUDIO_FRAME_PER_SEC * \
                                HAL_SPEECH_FRAME_SIZE_PCM_BUF)

#define CALIB_AUDIO_BUF_SIZE    (REC_DURATION * SECOND_SIZE)
#else /// CES_DISPLAY
#define CALIB_AUDIO_BUF_SIZE    32
#endif /// CES_DISPLAY



// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

/// The global context.
PROTECTED VOLATILE CALIB_STUB_CTX_T g_calibStubGlobalCtx;

/// The local context.
PROTECTED CALIB_STUB_CTX_T          g_calibStubLocalCtx;

/// Pointer to the context, uncached. 
PROTECTED VOLATILE CALIB_STUB_CTX_T* g_calibStubGlobalCtxPtr;

/// DSP/EQUALIZATION READY.
EXPORT VOLATILE UINT8 g_calibEquRdyFlag;

// FIXME Should be private, but avoid a warning.
PROTECTED UINT32  g_calibAudioBuf[CALIB_AUDIO_BUF_SIZE/4];



// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// calib_StubGetFromGlobal
// -----------------------------------------------------------------------------
/// Copy the global ctx values to the local ctx. This will
/// avoid that the value of ARFCN changes before the value
/// of the Band, for example.
// =============================================================================
PRIVATE VOID calib_StubGetFromGlobal(VOID)
{
    CALIB_PROFILE_FUNCTION_ENTER(calib_StubGetFromGlobal);
    
    g_calibStubLocalCtx.state       = g_calibStubGlobalCtxPtr->state;
    g_calibStubLocalCtx.firstFint   = g_calibStubGlobalCtxPtr->firstFint;
    
    g_calibStubLocalCtx.txArfcn     = g_calibStubGlobalCtxPtr->txArfcn;
    g_calibStubLocalCtx.txPcl       = g_calibStubGlobalCtxPtr->txPcl;
    g_calibStubLocalCtx.txBand      = g_calibStubGlobalCtxPtr->txBand;
    g_calibStubLocalCtx.txTsc       = g_calibStubGlobalCtxPtr->txTsc;
    g_calibStubLocalCtx.txDac       = g_calibStubGlobalCtxPtr->txDac;

    g_calibStubLocalCtx.monArfcn    = g_calibStubGlobalCtxPtr->monArfcn;
    g_calibStubLocalCtx.monBand     = g_calibStubGlobalCtxPtr->monBand;
    g_calibStubLocalCtx.monExpPow   = g_calibStubGlobalCtxPtr->monExpPow;

    CALIB_PROFILE_FUNCTION_EXIT(calib_StubGetFromGlobal);
}




// =============================================================================
// calib_StubEquRdyHandler
// -----------------------------------------------------------------------------
/// Handler the PAL Dsp Interrupt.
// =============================================================================
PRIVATE VOID calib_StubEquRdyHandler(VOID)
{
    g_calibEquRdyFlag = TRUE;
}



// Dictaphone callback function (aka interrupt handler). 
// =============================================================================
// calib_StubAudioHandler
// -----------------------------------------------------------------------------
/// Audio calibration functions handler. This function is meant to be used as
/// the handler for end-of-buffer IRQ: once the 4 seconds buffer is elapsed,
/// it closes the audio. 
// =============================================================================
PRIVATE VOID calib_StubAudioHandler(VOID)
{
    CALIB_PROFILE_FUNCTION_ENTER(calib_StubAudioHandler);
    
    g_calibStubGlobalCtxPtr->audioDictaphoneStatus = 0;

    /// Stop the audio. Dangerous usage! 
    while (AUD_ERR_NO != aud_StreamStop(g_calibStubGlobalCtxPtr->itfSel));

    CALIB_PROFILE_FUNCTION_EXIT(calib_StubAudioHandler);
}













/// ----------------- FINT treatement 

// =============================================================================
// calib_StubFintIrqHandler
// -----------------------------------------------------------------------------
/// This function implements the calibration stub state machine as a handler
/// for the FINT interrupt.
// =============================================================================
PRIVATE VOID calib_StubFintIrqHandler(VOID)
{
    UINT32 i;

    /// Audio configuration. 
    AUD_LEVEL_T         audioLevel;
    HAL_AIF_STREAM_T    audioStream;

    // Very first FINT. Used to skip the handling of the very first
    // FINT. This is useful because, when the FINT is demasked, we don't
    // know when the previous FINT happened before. So if the demasking
    // is done very late in the frame, the FINT handling might take too
    // much time and lead to a FINT re-entrance.
    PRIVATE BOOL veryfirstFint = TRUE;

    CALIB_PROFILE_FUNCTION_ENTER(calib_StubFintIrqHandler);
    
    /// We skip the very first FINT handling. 
    if (veryfirstFint)
    {
        veryfirstFint = FALSE;
        CALIB_PROFILE_FUNCTION_EXIT(calib_StubFintIrqHandler);
        return;
    }
    
    pal_FrameStart();

    /// ----- Update the local parameters if a new state came from HST. 

    /// Copy the global ctx values to the local ctx. 
    if (TRUE == g_calibStubGlobalCtxPtr->firstFint)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdGlobal);
        
        calib_StubGetFromGlobal();

        /// Execute the command only once. 
        g_calibStubGlobalCtxPtr->firstFint = FALSE;
    }

    /// Update GSM counters. 
    g_calibStubGlobalCtxPtr->fn = (++g_calibStubGlobalCtxPtr->fn) % 2715648;
    g_calibStubGlobalCtxPtr->t3 = (++g_calibStubGlobalCtxPtr->t3) % 51;
    g_calibStubGlobalCtxPtr->t2 = (++g_calibStubGlobalCtxPtr->t2) % 26;
    
    /// Some LED blinking. 
    if (g_calibStubLocalCtx.state == CALIB_STUB_NO_STATE)
    {
        calib_PulseLight(2);
    }
    else
    {
        calib_PulseLight(15);
    }
    
    /// ----- Execute the subcommands from HST. 

    // Set the crystal frequency offset, used to adjust to rough
    // frequency parameter (sometimes called CDAC).
    if (CALIB_STUB_XTAL_IDLE != g_calibStubGlobalCtxPtr->setXtalFreqOffset)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdXtal);
        
        // Execute the command and send the result to HST thanks to
        // the field 'XtalCalibDone'.
        g_calibStubGlobalCtxPtr->xtalCalibDone =
            rfd_XcvCalibXtalFreqOffset(g_calibStubGlobalCtxPtr->setXtalFreqOffset);
            
        SXS_TRACE(TSTDOUT, "Calibration: Crystal offset set to: %d",
                                            g_calibStubGlobalCtxPtr->setXtalFreqOffset);
        
        /// Execute this command only once. 
        g_calibStubGlobalCtxPtr->setXtalFreqOffset = CALIB_STUB_XTAL_IDLE;
    }

    // Set the AFC bound, used to calculate the min and max AFC
    // influence on the frequency. 
    if (CALIB_STUB_AFC_BOUND_NO != g_calibStubGlobalCtxPtr->setAfcBound)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdAfcBound);
        
        rfd_XcvCalibSetAfcBound((RFD_AFC_BOUND_T)g_calibStubGlobalCtxPtr->setAfcBound);
        
        SXS_TRACE(TSTDOUT, "Calibration: AFC bound set to: %d",
                                            g_calibStubGlobalCtxPtr->setAfcBound);
        
        /// Execute this command only once. 
        g_calibStubGlobalCtxPtr->setAfcBound = CALIB_STUB_AFC_BOUND_NO;
    }
    
    /// Set the AFC frequency offset, used to calculate the AFC gain. 
    if (0 != g_calibStubGlobalCtxPtr->setAfcFreqOffset)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdAfcFOf);
        
        rfd_XcvCalibCalcAfcGain(    g_calibStubGlobalCtxPtr->setAfcFreqOffset,
                                (GSM_RFBAND_T)g_calibStubGlobalCtxPtr->txBand);
                                
        SXS_TRACE(TSTDOUT, "Calibration: AFC frequency offset set to: %d"
                             ", for band: %d",
                                            g_calibStubGlobalCtxPtr->setAfcFreqOffset,
                                            g_calibStubGlobalCtxPtr->txBand);
        
        /// Execute this command only once. 
        g_calibStubGlobalCtxPtr->setAfcFreqOffset = 0;
    }
    if (0 != g_calibStubGlobalCtxPtr->setChangeDACAfcFreq)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdAfcFOf);
        rfd_XcvCalibCalcAfcFreqChangeDAC(g_calibStubGlobalCtxPtr->setChangeDACAfcFreq, g_calibStubGlobalCtxPtr->txBand);
                                
        /// Execute this command only once. 
        g_calibStubGlobalCtxPtr->setChangeDACAfcFreq = 0;
    }

    /// Set the changed CDAC AFC frequency offset, used to calculate the AFC gain. 
    // influence on the frequency. 
    if (CALIB_STUB_AFC_BOUND_NO != g_calibStubGlobalCtxPtr->setChangeDACAfcBound)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdAfcBound);
        
        rfd_XcvCalibSetAfcBoundChangeDAC((RFD_AFC_BOUND_T)g_calibStubGlobalCtxPtr->setChangeDACAfcBound);
        
        SXS_TRACE(TSTDOUT, "Calibration: AFC bound set to: %d",
                                            g_calibStubGlobalCtxPtr->setChangeDACAfcBound);
        
        /// Execute this command only once. 
        g_calibStubGlobalCtxPtr->setChangeDACAfcBound = CALIB_STUB_AFC_BOUND_NO;
    }
    
    /// Set the AFC frequency offset, used to calculate the AFC gain. 
    if (0 != g_calibStubGlobalCtxPtr->setChangeDACAfcFreqOffset)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdAfcFOf);
        
        rfd_XcvCalibCalcAfcGainChangeDAC(    g_calibStubGlobalCtxPtr->setChangeDACAfcFreqOffset,
                                (GSM_RFBAND_T)g_calibStubGlobalCtxPtr->txBand);
                                
        SXS_TRACE(TSTDOUT, "Calibration: AFC frequency offset set to: %d"
                             ", for band: %d",
                                            g_calibStubGlobalCtxPtr->setChangeDACAfcFreqOffset,
                                            g_calibStubGlobalCtxPtr->txBand);
        
        /// Execute this command only once. 
        g_calibStubGlobalCtxPtr->setChangeDACAfcFreqOffset = 0;
    }
// Set the insertion loss offset, used to adjust the ILoss.
    if (CALIB_STUB_ILOSS_IDLE != g_calibStubGlobalCtxPtr->setILossOffset)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdILoss);
        
        // Execute the command and send the result to HST thanks to
        // the field 'ILossCalibDone'.
        g_calibStubGlobalCtxPtr->iLossCalibDone = rfd_XcvCalibSetILoss(
                                g_calibStubGlobalCtxPtr->setILossOffset,
                                (GSM_RFBAND_T)g_calibStubGlobalCtxPtr->monBand,
                                (UINT16*)&g_calibStubGlobalCtxPtr->iLossNextArfcn);
        
        SXS_TRACE(TSTDOUT, "Calibration: Insertion loss offset set to: %d"
                             ", for band: %d",
                                            g_calibStubGlobalCtxPtr->setILossOffset,
                                            g_calibStubGlobalCtxPtr->monBand);
        SXS_TRACE(TSTDOUT, "Calibration: Next ARFCN value: %d",
                                            g_calibStubGlobalCtxPtr->iLossNextArfcn);
        SXS_TRACE(TSTDOUT, "Calibration: Local State: %d",
                                            g_calibStubLocalCtx.state);
         
        /* Execute this command only once. */
        g_calibStubGlobalCtxPtr->setILossOffset = CALIB_STUB_ILOSS_IDLE;

        CALIB_PROFILE_PULSE(calib_StubCmdILoss);        
    }

    /// Set a PA profile measurement point, used interpolate the PA profile. 
    if (CALIB_STUB_PA_PROF_IDLE != g_calibStubGlobalCtxPtr->setPAProfMeas)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdPaProf);
    
        // Execute the command and send the result to HST thanks to
        // the field 'paProfCalibDone' and tell what is the next PA DAC
        // point to measure.
        g_calibStubGlobalCtxPtr->paProfCalibDone = rfd_PaCalibPaProfile(
                                    (GSM_RFBAND_T)g_calibStubGlobalCtxPtr->txBand,
                                    g_calibStubGlobalCtxPtr->setPAProfMeas,
                                    (UINT16*)&g_calibStubGlobalCtxPtr->paProfNextDacVal);
        
        SXS_TRACE(TSTDOUT, "Calibration: PA profile point set to power: %d"
                             ", for band: %d",
                                            g_calibStubGlobalCtxPtr->setPAProfMeas,
                                            g_calibStubGlobalCtxPtr->txBand);
        SXS_TRACE(TSTDOUT, "Calibration: Next DAC value: %d",
                                            g_calibStubGlobalCtxPtr->paProfNextDacVal);
        
        /// Execute this command only once. 
        g_calibStubGlobalCtxPtr->setPAProfMeas = CALIB_STUB_PA_PROF_IDLE;
    }
    
    // Start the power management driver (PMD) calibration.
    if (0 != g_calibStubGlobalCtxPtr->pmdCustomCalibStart)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdPmdCustom);
        
        // Execute the command and send the result to HST thanks to
        // the field 'pmdCustomCalibStatus'.
        g_calibStubGlobalCtxPtr->pmdCustomCalibStatus = pmd_CalibStartCustom(
            g_calibStubGlobalCtxPtr->pmdCustomCalibStart,
            (CALIB_PMD_T*)&calib_GetPointers()->hstBb->pmd);
            
        SXS_TRACE(TSTDOUT, "Calibration: PMD custom calibration process done: %d",
            g_calibStubGlobalCtxPtr->pmdCustomCalibStatus);
        
        /// Execute this command only once. 
        g_calibStubGlobalCtxPtr->pmdCustomCalibStart = 0;
    }
    
    /// Request a calibration update. 
    if (0 != g_calibStubGlobalCtxPtr->setCalibUpdate)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdUpdate);
        
        // Execute the function of all RF modules to update 
        // the calibration values. 
        rfd_XcvCalibUpdateValues();
        
        SXS_TRACE(TSTDOUT, "Calibration: Update values done.");
                                            
        /// Execute this command only once. 
        g_calibStubGlobalCtxPtr->setCalibUpdate = 0;
    }
    
    /// Request a GP ADC measurement from the begining. 
    if (0 != g_calibStubGlobalCtxPtr->setRestartGpadcMeasure)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdRstGpadc);
        
        /// Reset the values of the average system. 
        g_calibStubGlobalCtxPtr->gpadcAccCount = 0;
        for (i = 0; i < CALIB_GPADC_CH_QTY; i++)
            g_calibStubGlobalCtxPtr->gpadcAver[i] = CALIB_STUB_GPADC_IDLE;
        
        /// Execute this command only once. 
        g_calibStubGlobalCtxPtr->setRestartGpadcMeasure = 0;
    }
   
    // Start the dictaphone record or playback, only if the dictaphone is
    // not already recording or playing. 
    if (0 != g_calibStubGlobalCtxPtr->audioDictaphoneStatus ||
        CALIB_STUB_AUDIO_OUT == g_calibStubLocalCtx.state ||
        CALIB_STUB_AUDIO_SIDE == g_calibStubLocalCtx.state)
        g_calibStubGlobalCtxPtr->startAudioDictaphone = 0;

    if (0 != g_calibStubGlobalCtxPtr->startAudioDictaphone)
    {
        CALIB_PROFILE_PULSE(calib_StubCmdAudioDict);
    
        /// Configure the gains passed by HST. 
        audioLevel.micLevel   = g_calibStubGlobalCtxPtr->inGain;
        audioLevel.spkLevel   = g_calibStubGlobalCtxPtr->outGain;

        // TODO check that this consistency setting doesn't
        // break anything.
        audioLevel.sideLevel = SND_SIDE_MUTE;
        audioLevel.toneLevel = SND_TONE_0DB;
    
        // We calibrate using the gsm quality:
        // 8khz, mono
        audioStream.startAddress    = g_calibAudioBuf;
        audioStream.length          = CALIB_AUDIO_BUF_SIZE;
        audioStream.sampleRate      = HAL_AIF_FREQ_8000HZ;
        audioStream.channelNb       = HAL_AIF_MONO;
        audioStream.voiceQuality    = TRUE;
        audioStream.halfHandler     = NULL;
        audioStream.endHandler      = calib_StubAudioHandler;

        /// Stop the previous audio. Dangerous usage! 
        while (AUD_ERR_NO !=aud_StreamStop(g_calibStubGlobalCtxPtr->itfSel));
            
        /// Dispatch the commands. 
        if (CALIB_AUDIO_DICTA_REC == g_calibStubGlobalCtxPtr->startAudioDictaphone)
        {
            /// Start the audio In. 
            if (AUD_ERR_NO == aud_StreamRecord(g_calibStubGlobalCtxPtr->itfSel,
                                               &audioStream,
                                               &audioLevel))
            {
                /// Keep track of the dictaphone status. 
                g_calibStubGlobalCtxPtr->audioDictaphoneStatus =
                                                    CALIB_AUDIO_DICTA_REC;
            }
        }
        else
        {
            /// Start the audio Out. 
            if (AUD_ERR_NO == aud_StreamStart(g_calibStubGlobalCtxPtr->itfSel,
                                               &audioStream,
                                               &audioLevel))
            {
                /// Keep track of the dictaphone status. 
                g_calibStubGlobalCtxPtr->audioDictaphoneStatus =
                                                    CALIB_AUDIO_DICTA_PLAY;
            }
        }
 
        /// Execute this command only once. 
        g_calibStubGlobalCtxPtr->startAudioDictaphone = 0;
    }
    
    // Set the DC offset fields to "error" because DC offset can only
    // be measured when the Target is in power minotoring mode. 
    if (g_calibStubLocalCtx.state != CALIB_STUB_MONIT_STATE)
    {
        g_calibStubGlobalCtxPtr->dcoAverI = CALIB_STUB_DCO_ERROR;
        g_calibStubGlobalCtxPtr->dcoAverQ = CALIB_STUB_DCO_ERROR;
    }
    
    /// Do the action, depending on the state. 
    switch (g_calibStubLocalCtx.state)
    {
        ///  State off. 

        case CALIB_STUB_NO_STATE:
        {
            /// Init at first FINT. 
            if (g_calibStubLocalCtx.firstFint)
            {
                /// Close all and go to state off. 
                while (AUD_ERR_NO != aud_StreamStop(g_calibStubGlobalCtxPtr->itfSel));
            
                g_calibStubLocalCtx.firstFint = FALSE;
                
                /// Reset the dictaphone status. 
                g_calibStubGlobalCtxPtr->audioDictaphoneStatus = 0; 
            }
        }
        break;
        
        /// ----- MONIT_STATE. 

        case CALIB_STUB_MONIT_STATE:
        {
            /// Init at first FINT. 
            if (g_calibStubLocalCtx.firstFint)
            {
                g_calibStubLocalCtx.firstFint = FALSE;
                g_calibStubGlobalCtxPtr->monPower = CALIB_STUB_SEARCH_POWER;
                g_calibStubGlobalCtxPtr->nbPower = CALIB_STUB_SEARCH_POWER;
                g_calibStubGlobalCtxPtr->monBitmap = 0;
                
                /// Initialize PAL with the selected band. 
                myl1_BandInit(g_calibStubLocalCtx.monBand);

                /// Init the DC offset average system. 
                g_calibStubGlobalCtxPtr->dcoAccCount = 0;
                g_calibStubGlobalCtxPtr->dcoAverI = CALIB_STUB_DCO_IDLE;
                g_calibStubGlobalCtxPtr->dcoAverQ = CALIB_STUB_DCO_IDLE;
            }
            else
            {
                /// Display only when the monit is done. 
                myl1_Monit( g_calibStubLocalCtx.monArfcn,
                            g_calibStubLocalCtx.monBand,
                            g_calibStubLocalCtx.monExpPow,
                            g_calibStubGlobalCtxPtr->t2,
                            (UINT8*)&g_calibStubGlobalCtxPtr->monPower,
                            (UINT8*)&g_calibStubGlobalCtxPtr->nbPower );

                // When there is enough DC offset measurements,
                // do the average for HST needs. 
                if (g_calibStubGlobalCtxPtr->dcoAccCount >= CALIB_DCO_ACC_COUNT)
                {
                    INT32 dcoAccI = 0;
                    INT32 dcoAccQ = 0;
                    
                    for (i = 0; i < CALIB_DCO_ACC_COUNT; i++)
                    {
                        dcoAccI += g_calibStubGlobalCtxPtr->dcoI[i];
                        dcoAccQ += g_calibStubGlobalCtxPtr->dcoQ[i];
                    }
                    
                    g_calibStubGlobalCtxPtr->dcoAverI =
                                        dcoAccI / CALIB_DCO_ACC_COUNT;
                    g_calibStubGlobalCtxPtr->dcoAverQ =
                                        dcoAccQ / CALIB_DCO_ACC_COUNT;
                }
            }
        }
        break;
        
        /// ----- FOF_STATE. 
    
        case CALIB_STUB_FOF_STATE:
        {
            /// Init at first FINT. 
            if (g_calibStubLocalCtx.firstFint)
            {
                g_calibStubLocalCtx.firstFint = FALSE;
                g_calibStubGlobalCtxPtr->meanFOf = CALIB_STUB_SEARCH_FOF;
                
                /// L1 init for the get FOf FSM. 
                myl1_GetFOf(    TRUE,
                                g_calibStubLocalCtx.monArfcn,
                                g_calibStubLocalCtx.monBand,
                                g_calibStubLocalCtx.monExpPow,
                                (INT32*)&g_calibStubGlobalCtxPtr->meanFOf );
            }
            /// Search FOf 
            else
            {
                myl1_GetFOf(    FALSE,
                                g_calibStubLocalCtx.monArfcn,
                                g_calibStubLocalCtx.monBand,
                                g_calibStubLocalCtx.monExpPow,
                                (INT32*)&g_calibStubGlobalCtxPtr->meanFOf );
            }
        }
        break;

        /// ----- IDLE_STATE. 
        
        case CALIB_STUB_SYNCH_STATE:
            if (g_calibStubLocalCtx.firstFint)
            {
                g_calibStubLocalCtx.firstFint = FALSE;

                /// Initialize PAL with the selected band. 
                myl1_BandInit(g_calibStubLocalCtx.monBand);
            }
            else
            {
                myl1_Synch();
            }
        break;

        /// Called by MyL1. 
        case CALIB_STUB_IDLE_STATE:
            myl1_Idle();
        break;

        /// ----- PA_STATE. 

        case CALIB_STUB_PA_STATE:
        {
            /// Init at first FINT. 
            if (g_calibStubLocalCtx.firstFint)
            {
                g_calibStubGlobalCtxPtr->bsic = 0;
                g_calibStubGlobalCtxPtr->fn = 0;
                g_calibStubGlobalCtxPtr->t2 = 0;
                g_calibStubGlobalCtxPtr->t3 = 0;
                g_calibStubLocalCtx.firstFint = FALSE;
                
                /// Initialize PAL with the selected band. 
                myl1_BandInit(g_calibStubLocalCtx.txBand);
            }
            else
            {
                /// Send the burst. 
                myl1_SendBurstDac(  g_calibStubLocalCtx.txArfcn,
                                    g_calibStubLocalCtx.txDac,
                                    g_calibStubLocalCtx.txBand,
                                    g_calibStubLocalCtx.txTsc);
            }
        }
        break;

        /// ----- TX_STATE. 

        case CALIB_STUB_TX_STATE:
        {
            /// Init at first FINT. 
            if (g_calibStubLocalCtx.firstFint)
            {
                g_calibStubGlobalCtxPtr->bsic = 0;
                g_calibStubGlobalCtxPtr->fn = 0;
                g_calibStubGlobalCtxPtr->t2 = 0;
                g_calibStubGlobalCtxPtr->t3 = 0;
                g_calibStubLocalCtx.firstFint = FALSE;
                
                /// Initialize PAL with the selected band. 
                myl1_BandInit(g_calibStubLocalCtx.txBand);
            }
            else
            {
                /// Send the burst. 
                myl1_SendBurst( g_calibStubLocalCtx.txArfcn,
                                g_calibStubLocalCtx.txPcl,
                                g_calibStubLocalCtx.txBand,
                                g_calibStubLocalCtx.txTsc);
            }
        }
        break;


        /// ----- AUDIO_OUT. 

        case CALIB_STUB_AUDIO_OUT:
        {
            /// Init at first FINT. 
            if (g_calibStubLocalCtx.firstFint)
            {
                g_calibStubLocalCtx.firstFint = FALSE;

                /// The dictaphone will be stopped, if enabled. 
                g_calibStubGlobalCtxPtr->audioDictaphoneStatus = 0;
        
                /// Configure the gains passed by HST. 
                audioLevel.micLevel   = g_calibStubGlobalCtxPtr->inGain;
                audioLevel.spkLevel   = g_calibStubGlobalCtxPtr->outGain;

                // TODO check that this consistency setting doesn't
                // break anything.
                audioLevel.sideLevel = SND_SIDE_MUTE;
                audioLevel.toneLevel = SND_TONE_0DB;
            
                // We calibrate using the gsm quality:
                // 8khz, mono
                audioStream.startAddress    = g_calibAudioBuf;
                audioStream.length          = CALIB_AUDIO_BUF_SIZE;
                audioStream.sampleRate      = HAL_AIF_FREQ_8000HZ;
                audioStream.channelNb       = HAL_AIF_MONO;
                audioStream.voiceQuality    = TRUE;
                audioStream.halfHandler     = NULL;
                audioStream.endHandler      = calib_StubAudioHandler;

                /// Stop the previous audio. Dangerous usage! 
                while (AUD_ERR_NO != aud_StreamStop(g_calibStubGlobalCtxPtr->itfSel));

                /// Build the sine wave. 
                sinGen( g_calibStubGlobalCtxPtr->outFreq,
                        g_calibStubGlobalCtxPtr->outAmpl,
                        g_calibAudioBuf, CALIB_AUDIO_BUF_SIZE );

                /// Start the audio Out. 
                /// TODO: what if err ?
                aud_StreamStart(g_calibStubGlobalCtxPtr->itfSel,
                                               &audioStream,
                                               &audioLevel);
                // polyphonic management is integrated 
                // into the audio druver.
            }
        }
        break;

        /// ----- AUDIO_SIDE. 

        case CALIB_STUB_AUDIO_SIDE:
        {
#if 0
            /// Init at first FINT. 
            if (g_calibStubLocalCtx.firstFint)
            {
                g_calibStubLocalCtx.firstFint = FALSE;

                /// The dictaphone will be stopped, if enabled. 
                g_calibStubGlobalCtxPtr->audioDictaphoneStatus = 0;
        
                /// Configure the gains passed by HST. 
                audioLevel.micSel     = g_calibStubGlobalCtxPtr->inSel;
                audioLevel.micLevel   = g_calibStubGlobalCtxPtr->inGain;
                audioLevel.spkSel     = g_calibStubGlobalCtxPtr->outSel;
                audioLevel.spkLevel   = g_calibStubGlobalCtxPtr->outGain;

                // TODO check that this consistency setting doesn't
                // break anything.
                audioLevel.sideLevel = SND_SIDE_MUTE;
                audioLevel.toneLevel = SND_TONE_0DB;
            
                // We calibrate using the gsm quality:
                // 8khz, mono
                audioStream.startAddress    = g_calibAudioBuf;
                audioStream.length          = CALIB_AUDIO_BUF_SIZE;
                audioStream.sampleRate      = HAL_AIF_FREQ_8000HZ;
                audioStream.channelNb       = HAL_AIF_MONO;
                audioStream.voiceQuality    = TRUE;
                audioStream.halfHandler     = NULL;
                audioStream.endHandler      = calib_StubAudioHandler;


                /// Stop the previous audio. Dangerous usage! 
                while (AUD_ERR_NO != aud_StreamStop(g_calibStubGlobalCtxPtr->itfSel));

                /// Enable the audio In and Out. 
                hal_audio_MicOpen();
                hal_audio_SpeakerOpen();
                hal_audio_SideTest(TRUE, &audioLevel);

                /// Enable, if needed, the polyphonic. 
                if (g_calibStubGlobalCtxPtr->outSel == HAL_AUDIO_SPK_LOUD)
                {
                    util_SPMA_OpenLoudspeaker(g_calibStubGlobalCtxPtr->polyGain);
                }
                else
                {
                    util_SPMA_Close();
                }               
            }
#endif 
        }
        break;
        default:
            break;
    }

    /// Do the GPADC measurements and average
    {
        CALIB_PROFILE_PULSE(calib_StubTimGpadcMeas);
        UINT32 gpadcIdx;
        UINT32 k;
        UINT32 j;


        // The counter goes from 0 to 2*CALIB_GPADC_ACC_COUNT-1.
        // This allows to know when we have more than
        // CALIB_GPADC_ACC_COUNT measurements to do an average.
        gpadcIdx = g_calibStubGlobalCtxPtr->gpadcAccCount % CALIB_GPADC_ACC_COUNT;
        
        /// Measure all the GP ADC channels and 
        for (k = 0; k < CALIB_GPADC_CH_QTY; k++)
        {
            g_calibStubGlobalCtxPtr->gpadcAcc[gpadcIdx][k] = hal_AnaGpadcGetRaw(k);
        }
        
        // If we have more than CALIB_GPADC_ACC_COUNT-2 measurements
        // we can do the average and put it in the context.
        if (g_calibStubGlobalCtxPtr->gpadcAccCount > CALIB_GPADC_ACC_COUNT-2)
        {
            for (k = 0; k < CALIB_GPADC_CH_QTY; k++)
            {
                UINT32 temp = 0;

                /// Do the sum of all the samples. 
                for (j = 0; j < CALIB_GPADC_ACC_COUNT; j++)
                    temp += g_calibStubGlobalCtxPtr->gpadcAcc[j][k];
                
                /// Do the average. 
                g_calibStubGlobalCtxPtr->gpadcAver[k] =
                                    temp / CALIB_GPADC_ACC_COUNT;
            }

            // This does the conversion between the GPADC hex value
            // and the real battery voltage, in milliVolts.
            // hal_gpadc_SetBatVolt(
            //    (VREF*g_calibStubGlobalCtxPtr->gpadcAver[GPADC_VBAT])/511);
           
        }
        
        /// Keep track of the GP ADC measurment count. 
        g_calibStubGlobalCtxPtr->gpadcAccCount++;
        if (g_calibStubGlobalCtxPtr->gpadcAccCount == CALIB_GPADC_ACC_COUNT*2)
        {
            g_calibStubGlobalCtxPtr->gpadcAccCount = CALIB_GPADC_ACC_COUNT;
        }
    }

    /// End of FINT treatment. 
    pal_FrameEnd();
    g_calibEquRdyFlag = FALSE;

    CALIB_PROFILE_FUNCTION_EXIT(calib_StubFintIrqHandler);
}




// =============================================================================
// calib_StubOpen
// -----------------------------------------------------------------------------
/// Initializes the calibration stub. Among operation is the registering of the 
/// calib stub state machine as the FINT Irq handler.
// =============================================================================
PROTECTED VOID calib_StubOpen(VOID)
{
    UINT32 i;

    /// Inform HCS which calib context to use to send commands. 
    calib_DaemonSetContext((VOID*)&g_calibStubGlobalCtx);
    g_calibStubGlobalCtxPtr = (CALIB_STUB_CTX_T*)HAL_SYS_GET_UNCACHED_ADDR(&g_calibStubGlobalCtx);


    /// Init some of the context parameters. 
    g_calibStubLocalCtx.state = CALIB_STUB_NO_STATE;
    g_calibStubLocalCtx.firstFint = FALSE;

    g_calibStubGlobalCtxPtr->state = CALIB_STUB_NO_STATE;
    g_calibStubGlobalCtxPtr->firstFint = FALSE;
    g_calibStubGlobalCtxPtr->monBitmap = 0;
    
    g_calibStubGlobalCtxPtr->setXtalFreqOffset = CALIB_STUB_XTAL_IDLE;
    g_calibStubGlobalCtxPtr->setPAProfMeas = CALIB_STUB_PA_PROF_IDLE;
    g_calibStubGlobalCtxPtr->setAfcBound = CALIB_STUB_AFC_BOUND_NO;
    g_calibStubGlobalCtxPtr->setILossOffset = CALIB_STUB_ILOSS_IDLE;
    g_calibStubGlobalCtxPtr->setChangeDACAfcBound = CALIB_STUB_AFC_BOUND_NO;
    g_calibStubGlobalCtxPtr->setChangeDACAfcFreqOffset = 0;
    g_calibStubGlobalCtxPtr->setChangeDACAfcFreq = 0;
    g_calibStubGlobalCtxPtr->setAfcFreqOffset = 0;
    g_calibStubGlobalCtxPtr->setCalibUpdate = 0;
    g_calibStubGlobalCtxPtr->setRestartGpadcMeasure = 0;
    g_calibStubGlobalCtxPtr->startAudioDictaphone = 0;
    g_calibStubGlobalCtxPtr->audioDictaphoneStatus = 0;
    g_calibStubGlobalCtxPtr->pmdCustomCalibStart = 0;
    
    /// Init the DC offset average system. 
    g_calibStubGlobalCtxPtr->dcoAccCount = 0;
    g_calibStubGlobalCtxPtr->dcoAverI = CALIB_STUB_DCO_ERROR;
    g_calibStubGlobalCtxPtr->dcoAverQ = CALIB_STUB_DCO_ERROR;

    /// Init the GP ADC average system. 
    g_calibStubGlobalCtxPtr->gpadcAccCount = 0;
    for (i = 0; i < CALIB_GPADC_CH_QTY; i++)
    {
        g_calibStubGlobalCtxPtr->gpadcAver[i] = CALIB_STUB_GPADC_ERROR;
    }

    /// Start the GPADC on periodic measurements for all Channel
    /// One measure every 1ms means that we will have fresh data
    /// every fint (about 4,5 ms).
    for (i = 0; i < HAL_ANA_GPADC_CHAN_QTY; i++)
    {
        hal_AnaGpadcOpen(i, HAL_ANA_GPADC_ATP_1MS);
    }

    // PA calib init.
    rfd_PaCalibInit();
    
    /// RF init. 
    pal_RfWakeUp();


    /// FINT init. 
    pal_SetUsrVector(PAL_FINT_IRQ, (VOID (*)(UINT32))calib_StubFintIrqHandler);
    pal_SetUsrVectorMsk(PAL_FINT_IRQ, 1);
    pal_SetUsrVector (PAL_DSP_IRQ, (VOID (*)(UINT32))calib_StubEquRdyHandler);
    ///pal_SetUsrVectorMsk(PAL_DSP_IRQ, 1);

}




