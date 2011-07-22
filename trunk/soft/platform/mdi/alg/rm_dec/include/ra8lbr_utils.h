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

#ifndef RA8LBR_UTILS_H
#define RA8LBR_UTILS_H

/* Includes */
#include "helix_types.h"
#include "helix_result.h"
#include "rm_memory.h"

/* Defines */
#define GECKO_VERSION                 ((1L<<24)|(0L<<16)|(0L<<8)|(3L))
#define GECKO_MC1_VERSION             ((2L<<24)|(0L<<16)|(0L<<8)|(0L))
#define HX_MC_MAX_NUM_CODECS          5
#define HX_MC_MAX_NUM_CHANNELS        6
#define HX_MC_CHANNEL_FRONT_LEFT      1
#define HX_MC_CHANNEL_FRONT_RIGHT     2
#define HX_MC_CHANNEL_CENTER          4
#define HX_MC_CHANNEL_LOWFREQ         8
#define HX_MC_CHANNEL_SURROUND_LEFT  16
#define HX_MC_CHANNEL_SURROUND_RIGHT 32
#define HX_MC_CHANNEL_CENTER_BACK   256

#define HX_MC_CHANNEL_MONO            4 /* center channel only */
#define HX_MC_CHANNEL_STEREO          3 /* front-left + front-right */

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

/* ra8lbr opaque data struct */
typedef struct ra8lbr_data_struct
{
    INT32   version;
    INT16   nSamples;
    INT16   nRegions;
    INT32   delay;
    UINT16  cplStart;
    UINT16  cplQBits;
    UINT32  channelMask;
} ra8lbr_data;

/* ra8lbr multi-channel per-codec info struct */
typedef struct ra8lbr_mc_codec_struct
{
    UINT32      ulNumChannels;
    UINT32      ulSamplesPerFrameOneChannel;
    UINT32      ulFrameSize;
    UINT32      ulDelayFrames;
    UINT32      ulDelayRemaining;
    ra8lbr_data codecData;
    void*       pDecoder;
    UINT32*     pulChannelMap;
} ra8lbr_mc_codec;

/* ra8lbr multi-channel struct */
typedef struct ra8lbr_mc_struct
{
    UINT32           ulNumCodecs;
    ra8lbr_mc_codec* pCodec;
    UINT16*          pusInterleaveBuffer;
} ra8lbr_mc;

/* ra8lbr decode struct */
typedef struct ra8lbr_decode_struct
{
    void*     pDecoder;
    UINT32    ulNumChannels;
    UINT32    ulChannelMask;
    UINT32    ulFrameSize;
    UINT32    ulFramesPerBlock;
    UINT32    ulSamplesPerFrame;
    UINT32    ulSampleRate;
    UINT32    ulDelayFrames;
    UINT32    ulDelayRemaining;
    UINT32    ulFramesToConceal;
    UCHAR*    pFlushData;
    HXBOOL    bMultiChannel;
    ra8lbr_mc multiChannel;
} ra8lbr_decode;

/*
 * ra8lbr_unpack_opaque_data
 *
 * This unpacks the opaque data for a single ra8lbr codec
 */
HX_RESULT ra8lbr_unpack_opaque_data(ra8lbr_data* pData,
                                    BYTE**       ppBuf,
                                    UINT32*      pulLen);


/* Utility to count the number of bits set */
UINT32 hx_get_num_bits_set(UINT32 ulFlags);

/*
 * ra8lbr_get_channel_map
 *
 * Given the channel mask for all the multi-channel codecs,
 * and the channel mask for a single codec, this routine
 * calculates a "channel map", which is an interleaving
 * offset to map between the output of a single codec
 * into the interleaved output from all codecs.
 */
HX_RESULT ra8lbr_get_channel_map(UINT32  ulChannelMaskAllCodecs,
                                 UINT32  ulChannelMaskThisCodec,
                                 UINT32  ulNumChannelsAllCodecs,
                                 UINT32  ulNumChannelsThisCodec,
                                 UINT32* pulChannelMap);

/*
 * ra8lbr_cleanup_decode_struct
 *
 * This routine cleans up a ra8lbr_decode struct
 */
void ra8lbr_cleanup_decode_struct(ra8lbr_decode*   pDecode,
                                  void*            pUserMem,
                                  rm_free_func_ptr fpFree);


#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef RA8LBR_UTILS_H */
