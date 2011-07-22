/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: parse_sdp_chunk.h,v 1.1 2007/12/18 17:04:59 ehyche Exp $
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

#ifndef PARSE_SDP_CHUNK_H
#define PARSE_SDP_CHUNK_H

#include "helix_types.h"
#include "helix_result.h"
#include "rm_property.h"

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

UINT32    determine_property_type(const char* pszStr, UINT32 ulLen);
HX_RESULT parse_sdp_chunk(const char*         pszStr,
                          rm_malloc_func_ptr  fpMalloc,
                          rm_free_func_ptr    fpFree,
                          void*               pUserMem,
                          UINT32*             pulNumProp,
                          rm_property*        pProp);
HX_RESULT parse_property_set_from_sdp_chunk(const char*         pszStr,
                                            rm_malloc_func_ptr  fpMalloc,
                                            rm_free_func_ptr    fpFree,
                                            void*               pUserMem,
                                            rm_property_set*    pSet);

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef PARSE_SDP_CHUNK_H */
