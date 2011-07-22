//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spp_cipher.c
///
/// Cipher module driver
///
/// @date 16/05/07
/// @author Laurent Bastide
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"
#include "global_macros.h"
#include "spal_cipher.h" 


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------


//======================================================================
// Initialize the count register
//----------------------------------------------------------------------
// This function initializes the count register. 
//
// @param count UINT32. Counter register.
//======================================================================
PUBLIC UINT8 spp_CipherCountInit (UINT32 count)
{
    return spal_CipherCountInit(count);
}

//======================================================================
// spp_CipherStart
//----------------------------------------------------------------------
// Start the encipherment. 
//
// @param algo UINT8. 
//
// @return UINT8. Error or no error??
//======================================================================
PUBLIC UINT8 spp_CipherStart(UINT8 algo)
{
    return spal_CipherStart(algo); 
}

//======================================================================
// spp_CipherCipherNB
//----------------------------------------------------------------------
// This function cipher 114 bits for normal burst. 
//
// @param BufferIn UINT32*. Pointer to input buffer.
// @param BufferOut UINT32*. Pointer to output buffer.
//
//======================================================================
PUBLIC VOID spp_CipherCipherNb(UINT32* BufferIn, UINT32* BufferOut)
{
    spal_CipherCipherNb(BufferIn, BufferOut); 
}

//======================================================================
// spp_CipherDecipherNB
//----------------------------------------------------------------------
// This function deciphers 114 bits for normal burst. 
//
// @param BufferIn UINT32*. Pointer to input buffer.
// @param BufferOut UINT32*. Pointer to output buffer.
//
//======================================================================
PUBLIC VOID spp_CipherDecipherNb(UINT32* BufferIn, UINT32* BufferOut)
{
    spal_CipherDecipherNb(BufferIn, BufferOut);
}

