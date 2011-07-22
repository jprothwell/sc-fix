//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spal_cipher.c
///
/// Cipher module driver
///
/// @date 16/05/07
/// @author Yordan Tabakov
//-----------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"
#include "global_macros.h"
#include "spal_cipher.h"
#include "cipher.h"


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
PUBLIC UINT8 spal_CipherCountInit (UINT32 count)
{
    UINT32 error;

    // Check if the cipher A5 is already processing 
    error = hwp_cipher->status & CIPHER_RUNNING;
    if (error) 
    {
         return cipher_err_running;
    }
    else
    {
        hwp_cipher->Count = count;
        return cipher_err_none;
    }
}

//======================================================================
// spal_CipherStart
//----------------------------------------------------------------------
/// Start the encipherment. 
///
/// @param algo UINT8. 
///
/// @return UINT8. Error or no error??
//======================================================================
PUBLIC UINT8 spal_CipherStart(UINT8 algo)
{
    UINT32 error;

    // Check if the cipher A5 is already processing
    error = hwp_cipher->status & CIPHER_RUNNING;
    if (error) 
    {
        return cipher_err_running;
    }
    else 
    {
        if (algo == CIPHER_A51) // A5/1
    {
            hwp_cipher->ctrl = CIPHER_ENABLE | CIPHER_ALGORITHM_A5_1;
        }
        else
        {
            if (algo == CIPHER_A52) // A5/2
            {
                hwp_cipher->ctrl = CIPHER_ENABLE | CIPHER_ALGORITHM_A5_2;
            }
            else // A5/3
            {
#if (CHIP_CIPHER_A53_SUPPORTED)
                hwp_cipher->ctrl = CIPHER_ENABLE | CIPHER_ALGORITHM_A5_3;
#endif
            }
        }
        return cipher_err_none;
    }
}


//======================================================================
// spal_CipherCipherNB
//----------------------------------------------------------------------
/// This function cipher 114 bits for normal burst. 
///
/// @param BufferIn UINT32*. Pointer to input buffer.
/// @param BufferOut UINT32*. Pointer to output buffer.
//
//======================================================================
PUBLIC VOID spal_CipherCipherNb(UINT32 *BufferIn, UINT32 *BufferOut)
{
    register UINT8 i;
    UINT32 CipherStream[4];

    // Wait for the ciphering to finish
    while(hwp_cipher->status & CIPHER_RUNNING);
    // Get data block 1
    CipherStream[0] = hwp_cipher->Data0_block2;
    CipherStream[1] = hwp_cipher->Data1_block2;
    CipherStream[2] = hwp_cipher->Data2_block2;
    CipherStream[3] = hwp_cipher->Data3_block2;
    // !! the BufferIn and BufferOut COULD be the same one !!
    for (i = 0; i < 4; i++)
        BufferOut[i] = BufferIn[i] ^ CipherStream[i];
}

//======================================================================
// spal_CipherDecipherNB
//----------------------------------------------------------------------
/// This function deciphers 114 bits for normal burst. 
///
/// @param BufferIn UINT32*. Pointer to input buffer.
/// @param BufferOut UINT32*. Pointer to output buffer.
//
//======================================================================
PUBLIC VOID spal_CipherDecipherNb(UINT32 *BufferIn, UINT32 *BufferOut)
{
    register UINT8 i;
    while(hwp_cipher->status & CIPHER_RUNNING);

    //hwp_cipher-> ctrl = CIPHER_SOFTBIT8 ;
    for (i = 0; i < 29; i++)
        BufferOut[i] = BufferIn[i] ^ (hwp_cipher->Data_block1[i].Data_block1);
}

// //======================================================================
// // spal_CipherRunning
// //----------------------------------------------------------------------
// // This functions tests whether Cipher is running or not. 
// //
// // @return BOOL. 0: cipher ready. 1: cipher running.
// //======================================================================
// BOOL spal_CipherRunning()
// {
//     UINT32 blk_ready;
//     blk_ready = hwp_cipher->status & CIPHER_DATA_BLK_RDY;
//     if (blk_ready)
//         return 0;
//     else
//         return 1;
// }


// // GLTODO This function is NEVER called in the code!
// /* Get the results of Block 2*/
// //======================================================================
// // spal_CipherGetBlk2
// //----------------------------------------------------------------------
// // This functions gets the results of Block 2 ciphering. 
// //
// // @param data_blk2 UINT32*. Pointer to the block2 data.
// //======================================================================
// VOID spal_CipherGetBlk2(UINT32* data_blk2)
// {
//     data_blk2[0] = hwp_cipher->Data0_block2;
//     data_blk2[1] = hwp_cipher->Data1_block2;
//     data_blk2[2] = hwp_cipher->Data2_block2;
//     data_blk2[3] = hwp_cipher->Data3_block2;
// }


// // GLTODO This function is NEVER called in the code!
// /* Get the results of Block 1*/
// //======================================================================
// // spal_CipherGetBlk1
// //----------------------------------------------------------------------
// // This functions gets the results of Block 1 ciphering. 
// //
// // @param data_blk1 UINT32*. Pointer to the block1 data.
// //======================================================================
// VOID spal_CipherGetBlk1(UINT32* data_blk1)
// {
//     UINT8 i ;    
//     for (i = 0; i < 29; i++)
//         data_blk1[i] = (hwp_cipher->Data_block1[i].Data_block1);
// }



