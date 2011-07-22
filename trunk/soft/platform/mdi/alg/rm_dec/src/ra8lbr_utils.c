/* ***** BEGIN LICENSE BLOCK *****
 * 
 * REALNETWORKS CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Portions Copyright (c) 1995-2005 RealNetworks, Inc.
 * All Rights Reserved.
 * 
 * The contents of this file, and the files included with this file,
 * are subject to the current version of the Real Format Source Code
 * Porting and Optimization License, available at
 * https://helixcommunity.org/2005/license/realformatsource (unless
 * RealNetworks otherwise expressly agrees in writing that you are
 * subject to a different license).  You may also obtain the license
 * terms directly from RealNetworks.  You may not use this file except
 * in compliance with the Real Format Source Code Porting and
 * Optimization License. There are no redistribution rights for the
 * source code of this file. Please see the Real Format Source Code
 * Porting and Optimization License for the rights, obligations and
 * limitations governing use of the contents of the file.
 * 
 * RealNetworks is the developer of the Original Code and owns the
 * copyrights in the portions it created.
 * 
 * This file, and the files included with this file, is distributed and
 * made available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL
 * SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT
 * OR NON-INFRINGEMENT.
 * 
 * Technology Compatibility Kit Test Suite(s) Location:
 * https://rarvcode-tck.helixcommunity.org
 * 
 * Contributor(s):
 * 
 * ***** END LICENSE BLOCK ***** */

#include "ra8lbr_utils.h"
//#include "pack_utils.h"
//#include "gecko2codec.h"
//#include "helix_memory.h"
#include "pack_utils.h"

HX_RESULT ra8lbr_unpack_opaque_data(ra8lbr_data* pData,
                                    BYTE**       ppBuf,
                                    UINT32*      pulLen)
{
    HX_RESULT retVal = HXR_INVALID_PARAMETER;

    if (pData && ppBuf && *ppBuf && pulLen && *pulLen >= 8)
    {
        /* Clear the return value */
        retVal = HXR_OK;
        /* Unpack the version, samples, and regions */
        pData->version  = (INT32) rm_unpack32(ppBuf, pulLen);
        pData->nSamples = (INT16) rm_unpack16(ppBuf, pulLen);
        pData->nRegions = (INT16) rm_unpack16(ppBuf, pulLen);
        /* Is the version later than GECKO_VERSION? */
        if (pData->version >= GECKO_VERSION)
        {
            /* Set the return value */
            retVal = HXR_FAIL;
            /* Make sure we have at least 8 more bytes */
            if (*pulLen >= 8)
            {
                /* Unpack the delay, cplStart, and cplQBits */
                pData->delay    = (INT32) rm_unpack32(ppBuf, pulLen);
                pData->cplStart = rm_unpack16(ppBuf, pulLen);
                pData->cplQBits = rm_unpack16(ppBuf, pulLen);
                /* Clear the return value */
                retVal = HXR_OK;
            }
        }
        else
        {
            /*
             * The fixed point ra8lbr decoder supports dual-mono decoding with
             * a single decoder instance if cplQBits is set to zero.
             */
            pData->delay    = 0;
            pData->cplStart = 0;
            pData->cplQBits = 0;
        }
        if (retVal == HXR_OK)
        {
            /* Is this multi-channel? */
            if (pData->version == GECKO_MC1_VERSION)
            {
                /* Set the return value */
                retVal = HXR_FAIL;
                /* Make sure we have at least 4 more bytes */
                if (*pulLen >= 4)
                {
                    /* Unpack the channel mask */
                    pData->channelMask = rm_unpack32(ppBuf, pulLen);
                    /* Clear the return value */
                    retVal = HXR_OK;
                }
            }
        }
    }

    return retVal;
}

#if 0//sheen
UINT32 hx_get_num_bits_set(UINT32 ulFlags)
{
    UINT32 ulRet = 0;

    while (ulFlags)
    {
        ulRet++;
        ulFlags &= (ulFlags - 1) ; // clear rightmost bit
    }

    return ulRet;
}

HX_RESULT ra8lbr_get_channel_map(UINT32  ulChannelMaskAllCodecs,
                                 UINT32  ulChannelMaskThisCodec,
                                 UINT32  ulNumChannelsAllCodecs,
                                 UINT32  ulNumChannelsThisCodec,
                                 UINT32* pulChannelMap)
{
    HX_RESULT retVal      = HXR_INVALID_PARAMETER;
    UINT32    i           = 0;
    UINT32    ulMask      = 0;
    UINT32    ulIndexAll  = 0;
    UINT32    ulIndexThis = 0;

    if (ulChannelMaskAllCodecs && ulChannelMaskThisCodec &&
        ulNumChannelsAllCodecs && ulNumChannelsThisCodec &&
        ulNumChannelsThisCodec <= ulNumChannelsAllCodecs &&
        pulChannelMap)
    {
        /*
         * Make sure there are no bits set in the mask
         * for this codec that are NOT set in the overall mask.
         */
        if ((ulChannelMaskThisCodec | ulChannelMaskAllCodecs) == ulChannelMaskAllCodecs)
        {
            /* Clear the return value */
            retVal = HXR_OK;
            /* Loop through all the bits in a 32-bit mask */
            for (i = 0; i < 32 && retVal == HXR_OK; i++)
            {
                /* Get a mask with only the i-th bit set */
                ulMask = (1 << i);
                /* Is the i-th bit set in the overall channel mask? */
                if (ulChannelMaskAllCodecs & ulMask)
                {
                    /* Is the i-th bit set in the channel mask for this codec? */
                    if (ulChannelMaskThisCodec & ulMask)
                    {
                        /* Make sure we don't overrun the buffer */
                        if (ulIndexThis < ulNumChannelsThisCodec)
                        {
                            /* Put an entry into the channel map */
                            pulChannelMap[ulIndexThis] = ulIndexAll;
                            /* Increment the index in this codec's channel map */
                            ulIndexThis++;
                        }
                        else
                        {
                            retVal = HXR_FAIL;
                        }
                    }
                    /* Increment the overall channel index */
                    ulIndexAll++;
                }
            }
        }
    }

    return retVal;
}

void ra8lbr_cleanup_decode_struct(ra8lbr_decode*   pDecode,
                                  void*            pUserMem,
                                  rm_free_func_ptr fpFree)
{
    UINT32 i = 0;

    if (pDecode && fpFree)
    {
        /* Free the flush data buffer */
        if (pDecode->pFlushData)
        {
            fpFree(pUserMem, pDecode->pFlushData);
            pDecode->pFlushData = HXNULL;
        }
        /* Free the non-multi-channel decoder */
        if (pDecode->pDecoder)
        {
            /* Call into the fixed-point ra8lbr decoding library */
            Gecko2FreeDecoder(pDecode->pDecoder);
            /* Null out the pointer */
            pDecode->pDecoder = HXNULL;
        }
        /* Free multi-channel resoures */
        if (pDecode->bMultiChannel)
        {
            /* Free the multi-channel interleave buffer */
            if (pDecode->multiChannel.pusInterleaveBuffer)
            {
                fpFree(pUserMem, pDecode->multiChannel.pusInterleaveBuffer);
                pDecode->multiChannel.pusInterleaveBuffer = HXNULL;
            }
            /* Free multi-channel codecs */
            if (pDecode->multiChannel.pCodec)
            {
                /* Cleanup the ra8lbr_mc_codec struct */
                for (i = 0; i < pDecode->multiChannel.ulNumCodecs; i++)
                {
                    /* Free the fixed-point decoder instance */
                    if (pDecode->multiChannel.pCodec[i].pDecoder)
                    {
                        /* Call into the fixed-point ra8lbr decoding library */
                        Gecko2FreeDecoder(pDecode->multiChannel.pCodec[i].pDecoder);
                        /* Null out the pointer */
                        pDecode->multiChannel.pCodec[i].pDecoder = HXNULL;
                    }
                    /* Free the channel map */
                    if (pDecode->multiChannel.pCodec[i].pulChannelMap)
                    {
                        fpFree(pUserMem, pDecode->multiChannel.pCodec[i].pulChannelMap);
                        pDecode->multiChannel.pCodec[i].pulChannelMap = HXNULL;
                    }
                }
                /* Free the array of ra8lbr_mc_codec structs */
                fpFree(pUserMem, pDecode->multiChannel.pCodec);
                /* Null out the pointer */
                pDecode->multiChannel.pCodec = HXNULL;
            }
        }
        /* Null out the ra8lbr_decode struct */
        memset(pDecode, 0, sizeof(ra8lbr_decode));
    }
}
#endif
