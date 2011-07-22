/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: bit_utils.h,v 1.1 2007/12/18 17:04:59 ehyche Exp $
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

#ifndef BIT_UTILS_H
#define BIT_UTILS_H

/* Includes */
typedef struct TAG_LD
{
    BYTE* rdbfr;
    BYTE* rdptr;
    INT32 incnt;
    INT32 bitcnt;
} LD;

/*
 * Initializes bit-buffer.
 * Use:
 * 	void initbuffer (char *buffer, void *bufinfo);
 * Input:
 *	buffer - pointer to the input buffer to use
 *	bufinfo - pointer to the buffer information
 * Returns:
 * 	none
 */
HX_INLINE void rm_initbuffer(BYTE* buffer, void* bufinfo)
{
    if (buffer && bufinfo)
    {
        LD* ld = (LD*) bufinfo;
        ld->rdptr  = buffer;
        ld->rdbfr  = buffer;
        ld->incnt  = 0;
        ld->bitcnt = 0;
    }
}

/*
 * Returns next n bits (right adjusted) without advancing.
 * Use:
 *     unsigned int showbits(int n, void *inst);
 * Input:
 *	n - the number of bits to retrieve
 *	inst - decoder instance
 * Note:
 * 	len must be less than 24
 * Returns:
 *	the bit vector
 */
HX_INLINE UINT32 rm_showbits(UINT32 n, void* inst)
{
    UINT32 ulRet = 0;

    if (n && inst)
    {
        LD*    ld = (LD*)inst;
        BYTE*  p  = ld->rdptr;
        UINT32 a  = 0;
        UINT32 c  = ld->incnt;
        /* load in big-Endian order: */
        a = (UINT32) (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + (p[3]);
        ulRet = (a << c) >> (32 - n);
    }

    return ulRet;
}

/*
 * Advance by n bits.
 * Use:
 *	void flushbits(int n, void *inst);
 * Input:
 *	n - the number of bits to throw away
 *	inst - decoder instance
 * Returns:
 *	none
 */
HX_INLINE void rm_flushbits(UINT32 n, void* inst)
{
    if (inst)
    {
        LD* ld     = (LD*) inst;
        ld->incnt += n;
        ld->bitcnt+= n;
        ld->rdptr += ld->incnt >> 3;
        ld->incnt &= 0x07;
    }
}

/*
 * Returns next n bits (right adjusted)
 * Use:
 *  unsigned int getbits(int n, void *inst);
 * Input:
 *	n - the number of bits to retrieve
 *	inst - decoder instance
 * Note:
 * 	len must be less than 24
 * Returns:
 *	the bit vector
 */
HX_INLINE UINT32 rm_getbits(UINT32 n, void* inst)
{
    UINT32 ulRet = rm_showbits(n, inst);
    rm_flushbits(n, inst);
    return ulRet;
}

/*
 * Returns next bit from the bitstream.
 * Use:
 *	unsigned int getbits1 (void * inst);
 * Input:
 *	inst - decoder instance
 * Returns:
 *	bit value
 */
HX_INLINE UINT32 rm_getbits1(void* inst)
{
    UINT32 ulRet = 0;

    if (inst)
    {
        LD*   ld = (LD*)inst;
        UINT32 a = ld->rdptr[0];
        UINT32 c = ld->incnt + 1;
        ld->bitcnt++;
        ld->incnt  = c & 7;
        ld->rdptr += c >> 3;
        ulRet = (a >> (8 - c)) & 1;
    }

    return ulRet;
}

/*
 * Seek to a certain position in the bitstream:
 * Use:
 *	unsigned char *gotoByte (int byteInPacket, void *bufinfo);
 * Input:
 *	byteInPacket - target byte position
 *	bufinfo - pointer to the buffer information
 * Returns
 *  pointer to the requested position in the bistream
 */
HX_INLINE BYTE* rm_gotoByte(UINT32 byteInPacket, void* bufinfo)
{
    BYTE* pRet = NULL;

    if (bufinfo)
    {
        LD* ld = (LD*)bufinfo;
        BYTE* newPtrLocation = ld->rdbfr + byteInPacket;
        ld->rdptr  = newPtrLocation;
        ld->bitcnt = byteInPacket << 3;
        ld->incnt  = 0;
        pRet       = newPtrLocation;
    }

    return pRet;
}

#endif /* #ifndef BIT_UTILS_H */
