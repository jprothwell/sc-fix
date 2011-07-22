/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: dirb.h,v 1.4 2005/04/27 19:35:00 hubbe Exp $
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
/*///////////////////////////////////////////////////////////////////////////// */
/*    INTEL Corporation Proprietary Information */
/* */
/*    This listing is supplied under the terms of a license */
/*    agreement with INTEL Corporation and may not be copied */
/*    nor disclosed except in accordance with the terms of */
/*    that agreement. */
/* */
/*    Copyright (c) 2000 Intel Corporation. */
/*    All Rights Reserved. */
/* */
/*////////////////////////////////////////////////////////////////////////// */
/* $Header: /cvsroot/rarvcode-video/codec/rv89combo/rv89combo_c/cdeclib/dirb.h,v 1.4 2005/04/27 19:35:00 hubbe Exp $ */
/*////////////////////////////////////////////////////////////////////////// */

#ifndef DIRECTB_H__
#define DIRECTB_H__

#include "beginhdr.h"
#include "rvtypes.h"

#define TR_SHIFT	14
#define TR_RND		(1 << (TR_SHIFT - 1))

typedef void (RV_CDECL T_DirectB_InterpolMB)(
	U8 *const		pDirB,		/* pointer to current direct mode MB buffer */
	const U8 *const	pPrev,		/* pointer to previous ref plane buffer */
	const U8 *const	pFutr,		/* pointer to future ref plane buffer */
	const U32		uPredPitch,	/* for pPrev and pFutr */
	const U32		uPitch,		/* buffer pitch for pDirB */
	const Bool32	bSkipInterp,
	const I32		uFwdRatio,
	const I32		uBwdRatio
) GNUCDECL;

#ifdef __cplusplus
extern "C" {
#endif
T_DirectB_InterpolMB C_DirectB_InterpolMB_Lu;
T_DirectB_InterpolMB C_DirectB_InterpolMB_Cr;
#ifdef APK
T_DirectB_InterpolMB MMX_DirectB_InterpolMB_Lu;
T_DirectB_InterpolMB MMX_DirectB_InterpolMB_Cr;
T_DirectB_InterpolMB KNI_DirectB_InterpolMB_Cr;
T_DirectB_InterpolMB KNI_DirectB_InterpolMB_Lu;
T_DirectB_InterpolMB AltiVec_DirectB_InterpolMB_Cr;
T_DirectB_InterpolMB AltiVec_DirectB_InterpolMB_Lu;
#endif
#if defined(ARM) || defined(_ARM_)
T_DirectB_InterpolMB SA_DirectB_InterpolMB_Lu;
T_DirectB_InterpolMB SA_DirectB_InterpolMB_Cr;
#ifdef ARM_COTULLA
T_DirectB_InterpolMB XC_DirectB_InterpolMB_Lu;
T_DirectB_InterpolMB XC_DirectB_InterpolMB_Cr;
#endif
#endif
#ifdef __cplusplus
}
#endif

#endif /* DIRECTB_H__ */
