/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2008, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vppp_amjp.h                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This is private header of VPP Multimedia.                         */
/*                                                                       */
/*************************************************************************/
///
///   @file vppp_amjp.h
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
///  - @ref vppp_amjp_struct "vpp private multimedia structures and defines" \n
///

#ifndef VPPP_AMJP_H
#define VPPP_AMJP_H

#include "cs_types.h"

///@defgroup vppp_amjp_struct
///@{

// ============================================================================
// Types
// ============================================================================

// ============================================================================
// VPP_AMJP_CFG_T
// ----------------------------------------------------------------------------
/// VPP AMJP configuration structure
// ============================================================================

// struct of input parameters
typedef struct
{
    // --------------
    // General config
    // --------------
    /// codec mode
    INT8 audioMode;
    INT8 videoMode;

    /// global reset
    INT16 reset;

    // --------------
    // Audio config
    // --------------
    /// equalizer
    INT16 eqType;
    /// unused (for data alignment)
    INT16 reserved;
    /// start address of the input stream buffer
    /// (VOC format, burst read)
    INT32* inStreamBufAddrStart;
    /// first address after the last field of the input stream buffer
    /// (VOC format, burst read)
    INT32* inStreamBufAddrEnd;
    /// start address of the output audio buffer
    /// (VOC format, burst write)
    INT32* outStreamBufAddr;
    /// address of the IFC status register containing the current ifc read AHB address
    /// (VOC format, single read)
    INT32* outStreamIfcStatusRegAddr;

    // --------------
    // Video config
    // --------------
    /// zoom
    INT16 zoomMode;
    /// rotate
    INT16 rotateMode;
    /// resize to width (valid if positive and if the native size is bigger)
    INT16 resizeToWidth;
    /// global reset
    INT16 vidBufMode;
    /// start address of the input video buffer
    /// (VOC format, burst read)
    INT32* inVidBufAddrStart;
    /// first address after the last field of the input video buffer
    /// (VOC format, burst read)
    INT32* inVidBufAddrEnd;
    /// start address of the output video buffer
    /// (VOC format, burst write)
    INT32* outVidBufAddr;

    // --------------
    // Extra Video config (for zoom)
    // --------------
    /// source width
    INT16 sourceWidth;
    /// source heigth
    INT16 sourceHeight;
    /// cropped width
    INT16 croppedWidth;
    /// cropped heigth
    INT16 croppedHeight;
    /// zoomed width
    INT16 zoomedWidth;
    /// zoomed heigth
    INT16 zoomedHeight;
    /// start address of the input Y component buffer
    /// (VOC format, burst read)
    INT32* inYBufAddrStart;
    /// start address of the input V component buffer
    /// (VOC format, burst read)
    INT32* inVBufAddrStart;

} VPP_AMJP_CFG_T;


// ============================================================================
// VPP_AMJP_CODE_CFG_T
// ----------------------------------------------------------------------------
/// Internal VPP Audio Jpeg configuration structure
// ============================================================================

typedef struct
{
    /// pointers to the jpeg code and constant data
    INT32* jpegPtrs[2];
    /// pointers to the mp12 code and constant data
    INT32* mp12Ptrs[2];
    /// pointers to the mp3 code and constant data
    INT32* mp3Ptrs[3];
    /// pointers to the amr code and constant data
    INT32* amrPtrs[2];
    /// pointers to the aac code and constant data
    INT32* aacPtrs[2];
    /// pointers to the equalizer table
    INT32* eqTable;
    /// pointers to the zoom code and const data
    INT32* zoomPtrs[2];

} VPP_AMJP_CODE_CFG_T;

///  @} <- End of the vppp_amjp_struct group


#endif  // VPPP_AMJP_H
