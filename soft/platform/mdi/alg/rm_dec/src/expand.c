/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: expand.c,v 1.5 2005/04/27 19:35:00 hubbe Exp $
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
/*/////////////////////////////////////////////////////////////////////////////////// */
/*    INTEL Corporation Proprietary Information */
/* */
/*    This listing is supplied under the terms of a license */
/*    agreement with INTEL Corporation and may not be copied */
/*    nor disclosed except in accordance with the terms of */
/*    that agreement. */
/* */
/*    Copyright (c) 1998 Intel Corporation. */
/*    All Rights Reserved. */
/* */
/*/////////////////////////////////////////////////////////////////////////////////// */
/*  $Header: /cvsroot/rarvcode-video/codec/rv89combo/rv89combo_c/cdeclib/expand.c,v 1.5 2005/04/27 19:35:00 hubbe Exp $ */
/*/////////////////////////////////////////////////////////////////////////////////// */

/*--------------------------------------------------------------------------; */
/* */
/*	ExpandPlane */
/* */
/*  Description: */
/*    This routine expands a picture frame by 16 or 8 bytes in all */
/*    directions for unrestricted motion vector mode.  It assumes */
/*    that there is space in memory around the existing frame */
/*    and simply writes to there (i.e., clobbers it). */
/* */
/* */
/*  Inputs */
/*    StartPtr     pointer to the first byte of the original frame. */
/*    FrameWidth   Width (in bytes) of the original frame. */
/*                 THIS MUST BE AT LEAST 32 & A MULTIPLE OF 4. */
/*    FrameHeight  Height (in rows) of the original frame. */
/*    Pitch        Pitch */
/*    Pels         Number of pels to expand the plane. */
/*                 16 for lumina, 8 for chroma.  This MUST */
/*                 BE A MULTIPLE OF 8. */
/* */
/*--------------------------------------------------------------------------; */
#include "beginhdr.h"
#include "rvtypes.h"

/*--------------------------------------------------------------------------; */
/* */
/*  The algorithm fills in (1) the bottom (not including corners), */
/*  then (2) the sides (including the bottom corners, but not the */
/*  top corners), then (3) the top (including the top */
/*  corners) as shown below, replicating the outermost bytes */
/*  of the original frame outward: */
/* */
/*               ---------------------------- */
/*              |                            | */
/*              |            (3)             | */
/*              |                            | */
/*              |----------------------------| */
/*              |     |                |     | */
/*              |     |                |     | */
/*              |     |                |     | */
/*              |     |    original    |     | */
/*              |     |     frame      |     | */
/*              |     |                |     | */
/*              | (2) |                | (2) | */
/*              |     |                |     | */
/*              |     |                |     | */
/*              |     |----------------|     | */
/*              |     |                |     | */
/*              |     |      (1)       |     | */
/*              |     |                |     | */
/*               ---------------------------- */
/* */
/*--------------------------------------------------------------------------; */
void RV_CDECL ExpandPlane
(
	U8 *StartPtr,
	U32 uFrameWidth,
	U32 uFrameHeight,
	U32 uPitch,
	U32 uPels
)
{
	U32 row, col;
	U8 uLeftFillVal;
	U8 uRightFillVal;
	PU32 pSrc;
	PU32 pDst;
	PU8 pByteSrc;


	/* section 1 at bottom */
	/* obtain pointer to start of bottom row of original frame */
	pSrc = (PU32)StartPtr + ((uFrameHeight*uPitch - uPitch)>>2);
	pDst = pSrc + (uPitch>>2);
	for (row=0; row<uPels; row++, pDst += (uPitch>>2))
		for (col=0; col<uFrameWidth>>2; col++)
			pDst[col] = pSrc[col];


	/* section 2 on left and right */
	/* obtain pointer to start of first row of original frame */
	pByteSrc = StartPtr;
	for (row=0; row<(uFrameHeight + uPels); row++, pByteSrc += uPitch)
	{
		/* get fill values from left and right columns of original frame */
		uLeftFillVal = *pByteSrc;
		uRightFillVal = *(pByteSrc + uFrameWidth - 1);

		/* fill all bytes on both edges */
		for (col=0; col<uPels; col++)
		{
			*(pByteSrc - uPels + col) = uLeftFillVal;
			*(pByteSrc + uFrameWidth + col) = uRightFillVal;
		}
	}
		
	/* section 3 at top */
	/* obtain pointer to top row of original frame, less expand pels */
	pSrc = (PU32)StartPtr - (uPels>>2);
	pDst = pSrc - (uPitch>>2);
	for (row=0; row<uPels; row++, pDst -= (uPitch>>2))
		for (col=0; col<(uFrameWidth+uPels+uPels)>>2; col++)
			pDst[col] = pSrc[col];

}	/* end ExpandPlane */

#ifdef INTERLACED_CODE
void RV_CDECL ExpandPlanei
(
	U8 *StartPtr,
	U32 uFrameWidth,
	U32 uFrameHeight,
	U32 uPitch,
	U32 uPels
)
{
	U32 row, col;
	U8 uLeftFillVal;
	U8 uRightFillVal;
	PU32 pSrc, pSrc2;
	PU32 pDst, pDst2;
	PU8 pByteSrc;


	// section 1 at bottom
	// obtain pointer to start of bottom row of original frame
	pSrc = (PU32)StartPtr + ((uFrameHeight*uPitch - uPitch)>>2);
	pSrc2 = pSrc - (uPitch>>2);
	pDst = pSrc + 2*(uPitch>>2);
	pDst2 = pSrc + (uPitch>>2);
	for (row=0; row<uPels; row +=2, pDst += 2*(uPitch>>2), pDst2 += 2*(uPitch>>2))
		for (col=0; col<uFrameWidth>>2; col++)
		{
			pDst[col] = pSrc[col];
			pDst2[col] = pSrc2[col];
		}


	// section 2 on left and right
	// obtain pointer to start of first row of original frame
	pByteSrc = StartPtr;
	for (row=0; row<(uFrameHeight + uPels); row++, pByteSrc += uPitch)
	{
		// get fill values from left and right columns of original frame
		uLeftFillVal = *pByteSrc;
		uRightFillVal = *(pByteSrc + uFrameWidth - 1);

		// fill all bytes on both edges
		for (col=0; col<uPels; col++)
		{
			*(pByteSrc - uPels + col) = uLeftFillVal;
			*(pByteSrc + uFrameWidth + col) = uRightFillVal;
		}
	}
		
	// section 3 at top
	// obtain pointer to top row of original frame, less expand pels
	pSrc = (PU32)StartPtr - (uPels>>2);
	pSrc2 = pSrc + (uPitch>>2);
	pDst = pSrc - 2*(uPitch>>2);
	pDst2 = pSrc - (uPitch>>2);

	for (row=0; row<uPels; row += 2, pDst -= 2*(uPitch>>2), pDst2 -= 2*(uPitch>>2))
		for (col=0; col<(uFrameWidth+uPels+uPels)>>2; col++)
		{
			pDst[col] = pSrc[col];
			pDst2[col] = pSrc2[col];
		}


//	fwrite(pDst, 1, uPitch *  (uFrameHeight+uPels*2), g_predfile);
//	U8 bb[1];
//	bb[0] = 128;
//	for (I32 i = 0; i < uPitch *  (uFrameHeight+uPels*2) / 2; i++)
//		fwrite(bb,1,  1,  g_predfile);
}	// end ExpandPlane
#endif
