/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: helix_types.h,v 1.5 2007/12/18 17:04:58 ehyche Exp $
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

#ifndef HELIX_TYPES_H
#define HELIX_TYPES_H

#include "hxtypes.h"
#ifdef SHEEN_VC_DEBUG
#include <windows.h>
#endif

/*
 * HXBOOL Type definition
 *
 * HXBOOL is a boolean type
 */
#ifndef HXBOOL

#if defined(_SYMBIAN)
typedef TBool HXBOOL;
#else /* #if defined(_SYMBIAN) */
typedef int HXBOOL;
#endif /* #if defined(_SYMBIAN) */

#endif /* #ifndef HXBOOL */

/*
 * BOOL Type definition
 *
 * BOOL is another name for a HXBOOL
 */
 #ifdef SHEEN_VC_DEBUG
typedef HXBOOL BOOL;
 #endif


/*
 * HXDOUBLE Type definition
 *
 * HXDOUBLE is a double-precision floating-point type
 */
#ifndef HXDOUBLE

typedef double HXDOUBLE;

#endif /* #ifndef HXDOUBLE */

/*
 * HXNULL definition
 */
#ifndef HXNULL
#define HXNULL ((void *)0)
#endif

#ifdef SHEEN_VC_DEBUG
#define HX_INLINE _inline
#else
#define HX_INLINE inline
#endif

#endif /* #ifndef HELIX_TYPES_H */
