/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: rvdebug.h,v 1.3 2005/04/27 19:35:00 hubbe Exp $
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

#ifndef RVDEBUG_H__
#define RVDEBUG_H__

#include "rvtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void*    RV_Allocate(U32 size, Bool32 zero_init);
	  /* Allocate 'size' bytes of memory, optionally clear it, and */
    /* return a pointer to it.  Returns NULL if allocation fails. */
    /* The caller is repsonsible for recovering from a failed allocation. */
    /* Allocating 0 bytes is supported, though it will result in */
    /* some non-zero number of bytes actually being allocated. */

extern void      RV_Free(void*);
    /* Free memory previously allocated by RV_Allocate. */




#define  RV_TL_ALWAYS	0
#define  RV_TL_INIT		2
#define  RV_TL_PICT		4
#define  RV_TL_SUBPICT	6
#define  RV_TL_MBLK		8
#define  RV_TL_BLK		10
#define  RV_TL_HDET		12

typedef U32                 RV_Trace_Level;
#if defined (DEBUG)

extern  void   RV_Debug_Message
                                    (
                                    RV_Trace_Level,
                                    char *fmt,
                                    ...
                                    );

extern  void    RV_Assert_Failed
                                    (
                                    char   *file_name,
                                    I32     line,
                                    char*   expr
                                    );
#define RVAssert(expr)   do { \
                              if (!(expr)) \
                                  RV_Assert_Failed(__FILE__,__LINE__,#expr);\
                          } while (0)


#define RVDebug(params)  //RV_Debug_Message params


#else
#define RVDebug(params)
#define RVAssert(expr)

#endif



#ifdef __cplusplus
}
#endif

#endif /* RVDEBUG_H__ */
