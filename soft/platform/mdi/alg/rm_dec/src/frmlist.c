/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: frmlist.c,v 1.6 2005/04/27 19:35:00 hubbe Exp $
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


#if defined(_WIN32)
/* <windows.h> is needed for registry and ini file access */
#include "hxtypes.h"
#include <windows.h>
#endif /* defined(_WIN32) */

#include "beginhdr.h"
#include "frmlist.h"
#include "rvdecoder.h"

#if defined(DEBUG_FRAMELIST)
#include <stdio.h>
extern FILE_HDL g_framelist;
#endif


/* TBD: the mutexes are C++ only, so DECODE_FRAME_LIST_MUTEX will not work */


void DecodedFrameList_Init(struct DecodedFrameList *t)
{
	t->m_pHead = t->m_pTail = 0;
	t->m_uLength = 0;
}



void DecodedFrameList_Delete(struct DecodedFrameList *t)
{
    struct DecodedFrame *pCurr = t->m_pHead;
    while (pCurr)
    {
        struct DecodedFrame *pNext = pCurr->m_pFutureFrame;;
        DecodedFrame_Delete(pCurr);
        pCurr = pNext;
    }

    t->m_pHead = t->m_pTail = 0;
	t->m_uLength = 0;
}

struct DecodedFrame*
DecodedFrameList_detachHead(struct DecodedFrameList *t)
{
    struct DecodedFrame *pHead = t->m_pHead;
    if (pHead)
    {
		t->m_uLength--;
#if defined (DEBUG) && defined(DEBUG_FRAMELIST)
		fprintf(g_framelist, "%x detachHead : %d (%d)\n",
				(U32)t, pHead->m_uFrameNumber,
				t->m_uLength);
#endif
        t->m_pHead = t->m_pHead->m_pFutureFrame;
        if (t->m_pHead)
            t->m_pHead->m_pPreviousFrame = 0;
        else
            t->m_pTail = 0;
    }
    return pHead;
}

void
DecodedFrameList_append(struct DecodedFrameList *t,
						 struct DecodedFrame *pFrame)
{
    pFrame->m_pPreviousFrame = t->m_pTail;
    pFrame->m_pFutureFrame = 0;
    if (t->m_pTail)
        t->m_pTail->m_pFutureFrame = pFrame;
    else
        t->m_pHead = pFrame;

    t->m_pTail = pFrame;
	t->m_uLength++;
#if defined (DEBUG) && defined(DEBUG_FRAMELIST)
	fprintf(g_framelist, "%x append     : %d (%d)\n",
			(U32)t, pFrame->m_uFrameNumber,
			t->m_uLength);
#endif
}

void
DecodedFrameList_insertList(struct DecodedFrameList *t, 
							struct DecodedFrameList *src)
{
    if (src->m_pHead)
    {
        src->m_pTail->m_pFutureFrame = t->m_pHead;
        if (t->m_pHead)
            t->m_pHead->m_pPreviousFrame = src->m_pTail;
        t->m_pHead = src->m_pHead;

        if (!t->m_pTail)
            t->m_pTail = src->m_pTail;

        src->m_pHead = src->m_pTail = 0;
    }
}

void DecodedFrameList2_Init(struct DecodedFrameList2 *t) 
{ 
	t->m_pHead = t->m_pTail = 0; 
	t->m_uLength = 0;
#ifdef DECODE_FRAME_LIST_MUTEX
	Decoder_makeMutex(&(t->m_pMutex));
		/* Make a mutex to protect members from concurrent access */
		/* by RMA threads. Typically: One thread enters the decoder through Decode() */
		/* and then detachHead() here, setting m_pTail to NULL.  Meanwhile another  */
		/* RMA thread (usually the renderer) enters the decoder through  */
		/* ReleaseFrame() and then here through append() and *uses* m_pTail right  */
		/* after it was set to NULL by the other thread. */
#endif
}



void DecodedFrameList2_Delete(struct DecodedFrameList2 *t)
{
    t->m_pHead = t->m_pTail = 0;
	t->m_uLength = 0;
#ifdef DECODE_FRAME_LIST_MUTEX
	Decoder_deleteMutex(t->m_pMutex);
#endif
}

struct DecodedFrame*
DecodedFrameList2_detachHead(struct DecodedFrameList2 *t)
{
	struct DecodedFrame *pHead;
#ifdef DECODE_FRAME_LIST_MUTEX
	Decoder_lockMutex(t->m_pMutex);
#endif
    pHead = t->m_pHead;
    if (pHead)
    {
		t->m_uLength--;
#if defined (DEBUG) && defined(DEBUG_FRAMELIST)
		fprintf(g_framelist, "%x detachHead2 : %d (%d)\n",
				(U32)t, pHead->m_uFrameNumber,
				t->m_uLength);
#endif
        t->m_pHead = t->m_pHead->m_pFutureFrame2;
        if (t->m_pHead)
            t->m_pHead->m_pPreviousFrame2 = 0;
        else
            t->m_pTail = 0;
    }
#ifdef DECODE_FRAME_LIST_MUTEX
	Decoder_unlockMutex(t->m_pMutex);
#endif
    return pHead;
}

void
DecodedFrameList2_append(struct DecodedFrameList2 *t, 
						 struct DecodedFrame *pFrame)
{
#ifdef DECODE_FRAME_LIST_MUTEX
	Decoder_lockMutex(t->m_pMutex);
#endif
    pFrame->m_pPreviousFrame2=t->m_pTail;
    pFrame->m_pFutureFrame2=0;
    if (t->m_pTail)
        t->m_pTail->m_pFutureFrame2=pFrame;
    else
        t->m_pHead = pFrame;

    t->m_pTail = pFrame;
	t->m_uLength++;
#if defined (DEBUG) && defined(DEBUG_FRAMELIST)
	fprintf(g_framelist, "%x append2     : %d (%d)\n",
			(U32)t, pFrame->m_uFrameNumber,
			t->m_uLength);
#endif
#ifdef DECODE_FRAME_LIST_MUTEX
	Decoder_unlockMutex(t->m_pMutex);
#endif
}





