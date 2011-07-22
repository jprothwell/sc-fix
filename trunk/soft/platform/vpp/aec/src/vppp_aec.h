
/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2008, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vppp_aec.h                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This is private header of VPP AEC.                                */
/*                                                                       */
/*************************************************************************/
///
///   @file vppp_hello.h
///   This is private header of VPP Multimedia.
///

///
///  @mainpage Granite VOC Processing Control API
///  Granite VOC Processing Control (VPP) API description.
///  @author Coolsand Technologies, Inc.
///  @version 1.0
///  @date    march 2007
///
///  @par Purpose
///
///  This document describes the Granite software VPP layer.
///
///  @par
///
///  VPP MULTIMEDIA API is composed of the following division :
///  - @ref vppp_hello_struct "vpp private multimedia structures and defines" \n
///

#ifndef VPPP_HELLO_H
#define VPPP_HELLO_H

#include "cs_types.h"

///@defgroup vppp_hello_struct
///@{

// ============================================================================
// Types
// ============================================================================


// ============================================================================
// VPP_HELLO_CODE_CFG_T
// ----------------------------------------------------------------------------
/// Internal VPP Hello configuration structure
// ============================================================================

typedef struct
{
    /// pointer to the hi code
    INT32* hiPtr;
    /// pointer to the bye code
    INT32* byePtr;
    /// pointer to the data constants
    INT32* constPtr;
    
} VPP_HELLO_CODE_CFG_T;



// ============================================================================
// VPP_SPEECH_AEC_INIT_T
// ----------------------------------------------------------------------------
/// Internal VPP AEC initialisation structure
// ============================================================================
typedef struct
{
	//ProcessPara
	INT16 ProcessMode;
	INT16 InitRegularFactor;
	INT16 AFUpdateFactor;
	INT16 EchoEstMethod;
	INT16 NoiseUpdateFactor;
	INT16 SNRUpdateFactor;
	INT16 SNRLimit;
	INT16 NoiseOverEstFactor;
	INT16 NoiseGainLimit;
	INT16 EchoUpdateFactor;
	INT16 SERUpdateFactor;
	INT16 SERLimit;
	INT16 EchoOverEstFactor;
	INT16 EchoGainLimit;
	INT16 CNGFactor;
	INT16 AmpGain;	
	INT32 NoiseMin;



	
	
	//soft limiter

//	INT16 SoftLimiter_Beta_order;
//	INT16 SoftLimiter_Scale;
//	INT32 SoftLimiter_T;


	//NonLinear filter

//	INT16 NonLinear_Beta_order;
//	INT16 NonLinear_Scale;
//	INT32 NonLinear_T;


	//pre-filter

//	INT16 g_PE_PPFCoef[5]; //g_PE_PPFilterLen 5
//	INT16 NoiseWeight_threshold;

	INT16 NoiseGainLimitStep;
	INT16 AmpThr;
	
	INT16 PFCLDataSmoothFactor;
	INT16 PFCLNoiseUpdateFactor;
	INT16 PFCLThrSigDet;
	INT16 PFCLThrDT;

	INT16 DataSmoothFactor;

	INT16 PFCLThrNT;
	INT16 PFCLThrDTDiff;
	INT16 PFCLDTHoldTime;
	INT16 PFCLDTStartTime;



} VPP_SPEECH_AEC_INIT_T;






///  @} <- End of the vppp_hello_struct group


#endif  // VPPP_HELLO_H
