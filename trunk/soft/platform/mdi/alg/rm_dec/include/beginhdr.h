/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: beginhdr.h,v 1.3 2005/04/27 19:35:00 hubbe Exp $
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
/*////////////////////////////////////////////////////////// */
/* */
/*    INTEL Corporation Proprietary Information */
/* */
/*    This listing is supplied under the terms of a license */
/*    agreement with INTEL Corporation and may not be copied */
/*    nor disclosed except in accordance with the terms of */
/*    that agreement. */
/* */
/*    Copyright (c) 1999 Intel Corporation. */
/*    All Rights Reserved. */
/* */
/*////////////////////////////////////////////////////////// */

#ifndef BEGINHDR_H__
#define BEGINHDR_H__

/* $Header: /cvsroot/rarvcode-video/codec/rv89combo/rv89combo_c/cdeclib/beginhdr.h,v 1.3 2005/04/27 19:35:00 hubbe Exp $ */

/* This file provides a crude "pre-include" mechanism.  Anything declared */
/* here is guaranteed to be included into every source file in the project, */
/* with the caveats noted below. */
/* The "guarantee" depends on a programming practice whereby every ".h" */
/* file in the project begins with a #include of this file.  Said #include */
/* should occur after system includes (i.e., #include <....>), and before */
/* any other project includes (i.e., #include "..."). */
/* */
/* Note that it is not necessary to include this file in any .c or .cpp files. */
/* As a matter of good programming practice, and perhaps correctness, such */
/* files should #include at least one meaningful ".h" file, and will thus */
/* include this file indirectly. */
/* */
/* RV89Combo-specific notes: */
/* */
/* We lied when we said this file is included everywhere.  This file is */
/* rv89combo-specific, and is thus included below the RV line in the RealVideo/Pia */
/* architecture.  We do not include this file in the RealVideo (though possibly */
/* I could imagine using it to configure the RealVideo RealVideo, since that */
/* RealVideo is more closely coupled with the codec than the other RealVideos), */
/* nor in the performance counter library.  We also do not include this */
/* file in "interface" files, such as trommsg.h, which are distributed */
/* to third parties for use in calling the codec. */


#include "rvcfg.h"


#endif /* BEGINHDR_H__ */
