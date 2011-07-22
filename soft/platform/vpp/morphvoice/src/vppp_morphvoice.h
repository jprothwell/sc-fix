
/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2008, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vppp_hello.h                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This is private header of VPP Multimedia.                         */
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
    /// pointer to the data constants
    INT32* constPtr;
    
} VPP_HELLO_CODE_CFG_T;


///  @} <- End of the vppp_hello_struct group


#endif  // VPPP_HELLO_H
