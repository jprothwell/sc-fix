/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: rvgen.c,v 1.5 2005/04/27 19:34:55 hubbe Exp $
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
/* */
/*/////////////////////////////////////////////////////////////////////////// */
#ifdef SHEEN_VC_DEBUG
#include "stdio.h"
#include "stdarg.h"
#include "stdlib.h"
#include "memory.h"
#endif
#include "rvtypes.h"
#include "rvdebug.h"
#include "rm_memory_default.h"

U32          g_number_of_loads = 0;
            /* This counter allows Load() and Unload() to perform */
            /* initialization only once, and cleanup only after all */
            /* loaded instances have terminated. */

#if defined(DEBUG)

FILE_HDL g_debug_fp = NULL;

RV_Trace_Level g_debug_trace_level = RV_TL_ALWAYS;

char* g_debug_module_name = NULL;



void 
RV_Debug_Message(U32 level, char *fmt, ...)
{
    va_list va;

/*	g_debug_fp = stdout; */
/*	g_debug_module_name = "rnconsole"; */

    if (level > g_debug_trace_level
            || !g_debug_fp
            || !fmt)
        return;

    va_start(va, fmt);


    fprintf(g_debug_fp, "%s : ",
			g_debug_module_name
            ? g_debug_module_name : " ");

    vfprintf(g_debug_fp, fmt, va);

    (void) fprintf(g_debug_fp, "\n");
    (void) fflush(g_debug_fp);

    va_end(va);
}

/* RV_Assert_Failed */
/* */
/* Print out a message indicating that the assertion failed, and give the */
/* user the option of aborting. */

void 
RV_Assert_Failed
    (
    char   *file_name,
    I32     line,
    char*   expr
    )
{
    if (!expr)
        expr = "0";

    if (!file_name)
        file_name = "- unknown -";

    RVDebug((RV_TL_ALWAYS, "Assertion failed:  %s, file %s, line %ld.",
                   expr, file_name, line));

    (void) fprintf(stderr,"Assertion failed:  %s, file %s, line %ld\n",
                   expr, file_name, line);
    (void) fflush(stderr);
#if defined (ARM) || defined(_ARM_) || defined(_WINCE)
	exit(3);
#else
    abort();
#endif
}


#endif	/* DEBUG */

void * 
RV_Allocate(U32 size, Bool32 zero_init)
{
    void *p;

    if (size == 0)
    {
        RVDebug((RV_TL_ALWAYS,
            "WARNING: RV_Allocate() called with zero size"));
        size++;  /* Allocate at least one byte */
    }

    //p = malloc(size);
	p = rm_memory_default_malloc(0, size);

	//printf("RV_Allocate %d\n", size);

    if (p && zero_init)
        (void) memset(p, 0, size);

    RVDebug((RV_TL_SUBPICT,
        "RV_Allocate(%6ld)  returning  0x%0.8lx",size,p));

    return p;
}

void 
RV_Free(void *p)
{
    RVDebug((RV_TL_SUBPICT, "RV_Free(0x%0.8lx)",p));

    if (p)
    {
        //free(p);
		rm_memory_default_free(0, p);
		//printf("RV_Free \n");
    }
    else
        RVDebug((RV_TL_ALWAYS, "ERROR  RV_Free(NULL)"));
}




