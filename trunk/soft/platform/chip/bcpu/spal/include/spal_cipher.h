//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spal_cipher.h
///
/// Cipher module driver
///
/// @date 16/05/07
/// @author Laurent Bastide
//-----------------------------------------------------------------------------

#ifndef SPAL_CIPHER_H
#define SPAL_CIPHER_H

// SPAL CIPHER
///@defgroup spal_cipher SPAL Ciphering API
///@{



//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"
#include "global_macros.h"
#include "cipher.h"

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

//-----------------------
// Cipher parameter types
//-----------------------

// Errors returned by the cipher driver functions 
typedef enum
{
    cipher_err_none = 0,
    cipher_err_running = 0xFF
}SPAL_CIPHER_ERROR_T;

typedef enum
{
    CIPHER_A51 = 0,
    CIPHER_A52 = 1,
#if (CHIP_CIPHER_A53_SUPPORTED)
    CIPHER_A53 = 2
#endif
}SPAL_CIPHER_MODE_T;


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------


//======================================================================
// Initialize the count register
//----------------------------------------------------------------------
/// This function initializes the count register. 
///
/// @param count UINT32. Counter register.
//======================================================================
PUBLIC UINT8 spal_CipherCountInit (UINT32 count);

//======================================================================
// spal_CipherStart
//----------------------------------------------------------------------
/// Start the encipherment. 
///
/// @param algo UINT8. 
///
/// @return UINT8. Error or no error??
//======================================================================
PUBLIC UINT8 spal_CipherStart(UINT8 algo);

//======================================================================
// spal_CipherCipherNB
//----------------------------------------------------------------------
/// This function cipher 114 bits for normal burst. 
///
/// @param BufferIn UINT32*. Pointer to input buffer.
/// @param BufferOut UINT32*. Pointer to output buffer.
//
//======================================================================
PUBLIC VOID spal_CipherCipherNb(UINT32 *BufferIn, UINT32 *BufferOut);

//======================================================================
// spal_CipherDecipherNB
//----------------------------------------------------------------------
/// This function deciphers 114 bits for normal burst. 
///
/// @param BufferIn UINT32*. Pointer to input buffer.
/// @param BufferOut UINT32*. Pointer to output buffer.
//
//======================================================================
PUBLIC VOID spal_CipherDecipherNb(UINT32 *BufferIn, UINT32 *BufferOut);

///@}
//
#endif                                          // #ifndef SPAL_CIPHER_H


