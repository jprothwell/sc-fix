//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file espal_dmasc.h
///
/// This file contains the Header for the DMASC.
///
/// @date 15/11/07
/// @author Eric Zabré
//-----------------------------------------------------------------------------


#ifndef __ESPAL_DMASC_H
#define __ESPAL_DMASC_H



//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"


//------------------------------------------------------------------------------
// Defines 
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// spal_DmascRecombine
//-----------------------------------------------------------------------------
// This function performs the recombination of IR blocks
// @param inBuf0            UINT8*.  IN        uncompressed input buffer (recombined or directly from EQ)
// @param inOutPps0         UINT32*  INOUT  puncturing scheme of inBuf0, and at return of outBuf
// @param inBuf1            UINT8*.  IN        input buffer from IR memory space (compressed or not)
// @param inPps1            UINT32*  IN     puncturing scheme of inBuf1
// @param len               UINT16   IN     length of the full buffer (bits to read from the puncturing table)
// @param outBuf            UINT8*.  OUT    output buffer, punctured with the scheme returned in inOutPps0
//=============================================================================

PUBLIC VOID spal_DmascRecombine(    UINT8*  inBuf0,
                                    UINT32* inOutPps0,
                                    UINT8*  inBuf1,
                                    UINT32* inPps1,
                                    UINT16  len,
                                    UINT8*  outBuf);



//-----------------------------------------------------------------------------
// spal_DmascSoftCompressSave
//-----------------------------------------------------------------------------
// This function performs the 8-to-4 bit compression of the soft bits
// @param src                UINT8*.  IN            data in buffer
// @param dest                UINT8*.  OUT        data out buffer
// @param length            UINT8.   INPUT        data buffer length.
//=============================================================================

PUBLIC VOID spal_DmascSoftCompressSave(    UINT8 *src, 
                                        INT16 length, 
                                        UINT8 *dest);


#endif  // __ESPAL_DMASC_H

