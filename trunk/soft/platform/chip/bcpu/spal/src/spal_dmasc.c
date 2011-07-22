//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spal_dmasc.c
///
/// This file contains the DMASC drivers.
///
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#if (CHIP_EDGE_SUPPORTED)
// include
#include "cs_types.h"
// chip/include
#include "global_macros.h"
#include "dmasc.h"
// spal public header
//#include "spal_dmasc.h"
// spal private header
#include "spalp_private.h"
#include "spalp_debug.h"

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

PUBLIC VOID spal_DmascRecombine(    UINT8*  inBuf0,
                                    UINT32* inOutPps0,
                                    UINT8*  inBuf1,
                                    UINT32* inPps1,
                                    UINT16  len,
                                    UINT8*  outBuf)
{
    // TODO...
    return;
}

PUBLIC VOID spal_DmascSoftCompressSave(    UINT8 *src, 
                                        INT16 length, 
                                        UINT8 *dest)
{
    // TODO...
    return;
}

#endif //CHIP_EDGE_SUPPORTED
