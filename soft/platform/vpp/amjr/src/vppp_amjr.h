
/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2008, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vppp_amjr.h                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This is private header of VPP Multimedia.                         */
/*                                                                       */
/*************************************************************************/
///
///   @file vppp_amjr.h
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
///  - @ref vppp_amjr_struct "vpp private multimedia structures and defines" \n
///

#ifndef VPPP_AMJR_H
#define VPPP_AMJR_H

#include "cs_types.h"

///@defgroup vppp_amjr_struct
///@{

// ============================================================================
// Types
// ============================================================================


// ============================================================================
// VPP_AMJR_CFG_T
// ----------------------------------------------------------------------------
/// VPP AMJR configuration structure
// ============================================================================

typedef struct
{
    // ----------------
    // common config
    // ----------------
 	INT8 audioMode;
 	INT8 videoMode;

    // ----------------
    // audio config
    // ----------------
	INT16  reset;
	INT32* inStreamBufAddr;
	INT32* outStreamBufAddr;
	INT32  sampleRate;
	INT16  bitRate;
	INT16  channelNum;
	INT32* inStreamBufIfcPtr;

    // ----------------
    // video config
    // ----------------
	INT32* imagInBufAddr;
	INT32* imagOutBufAddr;
	INT16  imagWidth;
	INT16  imagHeight;
	INT16  imagQuality;
	INT16  imagFormat;
	INT16  imagPxlSwap;
	INT16  imagReserved;
	INT32* previewOutBufAddr;
	INT16  previewWidth;
	INT16  previewHeight;
	INT16  previewFormat;
	INT16  previewScaleFactor;

} VPP_AMJR_CFG_T;



// ============================================================================
// VPP_AMJR_CODE_CFG_T
// ----------------------------------------------------------------------------
/// Internal VPP Audio Jpeg configuration structure
// ============================================================================

typedef struct
{
    /// pointers to the amr code and constant data
    INT32* amrPtrs[3];
    /// pointers to the mp3 code and constant data
    INT32* mp3Ptrs[3];
    /// pointers to the jpeg code and constant data
    INT32* jpegPtrs[2];

} VPP_AMJR_CODE_CFG_T;


///  @} <- End of the vppp_amjr_struct group


#endif  // VPPP_AMJR_H
