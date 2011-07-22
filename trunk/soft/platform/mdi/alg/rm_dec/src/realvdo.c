/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: realvdo.c,v 1.4 2005/04/27 19:34:52 hubbe Exp $
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
/*    Copyright (c) 1998 Intel Corporation. */
/*    All Rights Reserved. */
/* */
/*////////////////////////////////////////////////////////// */

/* $Header: /cvsroot/rarvcode-video/codec/rv89combo/rv89combo_c/common/realvdo.c,v 1.4 2005/04/27 19:34:52 hubbe Exp $ */

/* This module implements the generic encoder and decoder RealVideos, as */
/* well as base RealVideo class functionality for these environments. */


#include "stdlib.h"
#include "stdarg.h"
#include "string.h"

#include "realvdo.h"
#include "rm_memory_default.h"



void RVDecoder_Init(struct RVDecoder *t, RV_FID fid, RV_Status *ps)
{
    U16 test_endian;
	t->m_signature = RV_SIGNATURE; 
	t->m_is_big_endian = FALSE;
	t->m_picture_clock_frequency = ((F64)29.97); 

    /* Just to be safe */
    RVAssert(sizeof(U8) == 1);
    RVAssert(sizeof(I8) == 1);
    RVAssert(sizeof(U16) == 2);
    RVAssert(sizeof(I16) == 2);
    RVAssert(sizeof(U32) == 4);
    RVAssert(sizeof(I32) == 4);
    RVAssert(sizeof(F32) == 4);
    RVAssert(sizeof(F64) == 8);

    test_endian = 0x1234;
    t->m_is_big_endian = (RV_Boolean)((*(U8*)&test_endian) != 0x34);

	/* sheen
    RV_Identify_CPU(&t->m_cpu);

    RVDebug((RV_TL_INIT,
        "RealVideo::RealVideo(generic implementation), architecture %ld, "
        " %s-endian",
        (U32)(t->m_cpu.architecture),
        (t->m_is_big_endian ? "big" : "little")));
	*/
    t->m_coredecoder = NULL;
    t->m_coredecoder = Decoder_CreateDecoder(fid, t, ps);

    if (!t->m_coredecoder)
    {
        RVDebug((RV_TL_ALWAYS, "ERROR ::Generic_Decoder_RealVideo --"
            " failed to allocate RV_Decoder for %d",
            fid));
        *ps = RV_S_OUT_OF_MEMORY;
    }
    else if (*ps != RV_S_OK)
    {
		Decoder_Delete(t->m_coredecoder);
        rm_memory_default_free(0, t->m_coredecoder);
        t->m_coredecoder = NULL;
        RVDebug((RV_TL_ALWAYS, "ERROR ::Generic_Decoder_RealVideo --"
            " failed to create RV_Decoder for %d",
            fid));
    }
}

RV_Boolean RVDecoder_is_active(struct RVDecoder *h)
{
    return (RV_Boolean)(
        h && h->m_signature == RV_SIGNATURE);
}


RV_Boolean 
RVDecoder_Load()
{
    RV_Status  ps = RV_S_OK;

    g_number_of_loads++;

    if (g_number_of_loads == 1)
    {

        ps = Decoder_Startup();
    }

    return (ps == RV_S_OK) ? TRUE : FALSE;
}

RV_Boolean 
RVDecoder_Unload()
{
    RV_Status  ps = RV_S_OK;

    if (g_number_of_loads > 0)
    {
        g_number_of_loads--;

        if (g_number_of_loads == 0)
        {
	        ps = Decoder_Shutdown();
        }
    }

    return (ps == RV_S_OK) ? TRUE : FALSE;
}

void RVDecoder_Delete(struct RVDecoder *t)
{

    if (t->m_coredecoder)
	{
		Decoder_Delete(t->m_coredecoder);
        rm_memory_default_free(0, t->m_coredecoder);
	}
    t->m_coredecoder = 0;

    t->m_signature = 0;
}




