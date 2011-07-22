/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: supervlc.h,v 1.3 2005/04/27 19:35:00 hubbe Exp $
 * 
 * REALNETWORKS CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Portions Copyright (c) 1995-2002 RealNetworks, Inc.
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

/*/////////////////////////////////////////////////////////////////////////// */
/*    RealNetworks, Inc. Confidential and Proprietary Information. */
/* */
/*    Copyright (c) 1995-2002 RealNetworks, Inc. */
/*    All Rights Reserved. */
/* */
/*    Do not redistribute. */
/* */
/*/////////////////////////////////////////////////////////////////////////// */

/* supervlc.h -- functions for 4x4 transform coefficients encoding/decoding */
/* 03-21-00 06:38pm, written by Yuriy A. Reznik */

#if !defined(SUPERVLC_H__)
#define SUPERVLC_H__    1

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Explicitly declare the calling convention to eliminate potential
 * conflicts with compilation options. This is also forced via the
 * build system makefiles that build the library.
 */

#define SVLC_CALL

/*
 * These are STATIC constructors and they must be called only once: 
 * when DLL is loaded.
 */

/*
 * Initializes 4x4 encoding engine.
 * Use:
 *  void encode_4x4_init ();
 * Returns:
 *  none.
 */ 
void SVLC_CALL encode_4x4_init();
/*
 * Initializes 4x4 decoding engine.
 * Use:
 *  void decode_4x4_init ();
 * Returns:
 *  none.
 */
 int SVLC_CALL decode_4x4_init();

/* After init functions have been executed the other functions can  */
/* be called in  any order regardless of their completion (i.e. they  */
/* are fully reentrant). */
/*
 * Checks if 4x4 block needs to be encoded and
 * creates descriptor to be used in the encode process.
 * Use:
 *  unsigned int preprocess_4x4_block (short *block, unsigned long *descriptor);
 * Input:
 *  block - a pointer to 4x4 progressively scanned coefficients
 *  coded - a pointer indicating significant coefficients
 *   0 - all coefficients in block are zeros
 *   !0 - the block's encoding desriptor (to be passed to encode_4x4_block())
 * Returns: 0 if no clipping has occured, 1 otherwise.
 */
unsigned int SVLC_CALL preprocess_4x4_block (short *block, unsigned long *descriptor);
unsigned int SVLC_CALL preprocess_4x4_block_check (short *block, U32 *descriptor);

/*
 * Assess block "weight" (to aid R(D) heuristic).
 * Use:
 *  unsigned int weight_4x4_block (unsigned int dsc);
 * Input:
 *  dsc   - block's descriptor (value returned by preprocess_4x4_block())
 * Returns:
 *  a value in the range of 0..9 assessing block's importance
 */
 unsigned int SVLC_CALL weight_4x4_block (int dsc);

/*
 * Encodes a 4x4 block using the results of pre-processing.
 * Use:
 *  void encode_4x4_block (short *block, unsigned int dsc, int intra,
 *      int chroma, int qp, unsigned char **p_pbs, unsigned int *p_bitOffset);
 * Input:
 *  block - a pointer to 4x4 progressively scanned coefficients
 *  dsc   - block's descriptor (a value returned by preprocess_4x4_block())
 *  intra - 0, if inter coded block
 *          1, if normal intra block
 *          2, if DC-removed intra block
 *          3, if double-transformed intra DC block
 *  chroma - 1, if chroma block, 0 if luma
 *  qp    - quantization parameter (1..31)
 *  p_pbs - pointer to pointer to the current position in the bitstream
 *  p_bitOffset - pointer to the bit position in the byte pointed by *p_pbs
 * Returns:
 *  none.
 */
void SVLC_CALL encode_4x4_block (short *block, unsigned int dsc, int mbtype,
    int chroma, int qp, unsigned char **p_pbs, unsigned int *p_bitOffset);



/*
 * Decodes a 4x4 block.
 * Use:
 *  void decode_4x4_block (short *block, int intra, int chroma, int qp,
 *      unsigned char **p_pbs, unsigned int *p_bitOffset);
 * Input:
 *  block - a pointer to 4x4 progressively scanned coefficients
 *  intra - 0, if inter coded block
 *          1, if normal intra block
 *          2, if DC-removed intra block
 *          3, if double-transformed intra DC block
 *  chroma - 1, if chroma block, 0 if luma
 *  qp    - quantization parameter (1..31)
 *  p_pbs - pointer to pointer to the current position in the bitstream
 *  p_bitOffset - pointer to the bit position in the byte pointed by *p_pbs
 * Returns:
 *  none.
 */
void SVLC_CALL decode_4x4_block (short *block, int mbtype, int chroma, int qp,
    unsigned char **p_pbs, unsigned int *p_bitOffset);

/*****************************************************
 *
 * CBP descriptor encoding/decoding functions
 *
 *****************************************************/

/*
 * Encodes a CBP descriptor.
 * Use:
 *  void encode_cbp (unsigned int cbp, int intra, int qp,
 *      unsigned char **p_pbs, unsigned int *p_bitOffset);
 * Input:
 *  cbp   - a 24-bit macroblock descriptor to encode
 *  intra - 0, if inter coded macroblock
 *          1, if normal intra macroblock
 *          2, if DC-removed intra macroblock
 *  qp    - quantization parameter (1..31)
 *  p_pbs - pointer to pointer to the current position in the bitstream
 *  p_bitOffset - pointer to the bit position in the byte pointed by *p_pbs
 * Returns:
 *  none.
 */
void SVLC_CALL encode_cbp (unsigned int dsc, int mbtype, int qp,
    unsigned char **p_pbs, unsigned int *p_bitOffset);


/*
 * Decodes a cbp descriptor.
 * Use:
 *  unsigned int decode_cbp (int intra, int qp,
 *      unsigned char **p_pbs, unsigned int *p_bitOffset);
 * Input:
 *  intra - 0, if inter coded macroblock
 *          1, if normal intra macroblock
 *          2, if DC-removed intra macroblock
 *  qp    - quantization parameter (1..31)
 *  p_pbs - pointer to pointer to the current position in the bitstream
 *  p_bitOffset - pointer to the bit position in the byte pointed by *p_pbs
 * Returns:
 *  decoded cbp descriptor
 */
unsigned int SVLC_CALL decode_cbp (int mbtype, int qp,
    unsigned char **p_pbs, unsigned int *p_bitOffset);

#ifdef __cplusplus
}
#endif

#endif /* SUPERVLC_H__ */
