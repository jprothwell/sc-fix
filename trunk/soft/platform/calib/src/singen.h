/*************************************************************************/
/*                                                                       */
/*            Copyright (C) 2003-2005, Coolsand Technologies, Inc.       */
/*                            All Rights Reserved                        */
/*                                                                       */
/*  This source code is the property of Coolsand Technologies and is     */
/*  confidential. Any modification, distribution, reproduction or        */
/*  exploitation of any content of this file is totally forbidden,       */
/*  except with the written permission of Coolsand Technologies.         */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*                                                                       */
/*      singen.h                                                         */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      Generate a sine wave at a specified frequency and amplitude.     */
/*                                                                       */
/*      !!! COOLSAND CONFIDENTIAL !!!                                    */
/*                                                                       */
/*************************************************************************/

#ifndef _SINGEN_H_
#define _SINGEN_H_


#include "cs_types.h"



// =============================================================================
// sinGen
// -----------------------------------------------------------------------------
/// Generate a sine wave
/// @param f Frequency
/// @param ampl Amplitude
/// @param sinBuf Pointer to the buffer where the sinwave is stored.
/// @param bufSize Size of the buffer, in bytes.
// =============================================================================
PROTECTED VOID sinGen(UINT16 f, UINT8 ampl, UINT32 *sinBuf, UINT32 bufSize);



#endif /* _SINGEN_H_ */

