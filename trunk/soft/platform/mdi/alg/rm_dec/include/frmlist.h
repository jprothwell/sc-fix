/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: frmlist.h,v 1.4 2005/04/27 19:35:00 hubbe Exp $
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


#ifndef FRMLIST_H__
#define FRMLIST_H__

#include "rvtypes.h"
#include "frame.h"


#ifdef DECODE_FRAME_LIST_MUTEX
#include "rv89thread.h" /* needed for the mutex in DecodedFrameList2 */
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* The DecodedFrameList class implements a doubly-linked list of */
/* DecodedFrame objects.  It uses the m_pPreviousFrame and m_pFutureFrame */
/* members of the DecodedFrame class to implement the links. */

struct DecodedFrameList
{
        struct DecodedFrame               *m_pHead;
        struct DecodedFrame               *m_pTail;
            /* m_pHead points to the first element in the list, and m_pTail */
            /* points to the last.  m_pHead->previous() and m_pTail->future() */
            /* are both NULL. */

		U32							m_uLength;

        /*bool            isEmpty() { return !m_pHead; } */

};
void DecodedFrameList_Init(struct DecodedFrameList *t);

void DecodedFrameList_Delete(struct DecodedFrameList *t);

struct DecodedFrame  *DecodedFrameList_detachHead(struct DecodedFrameList *t);
    /* Detach the first frame and return a pointer to it, */
    /* or return NULL if the list is empty. */

void DecodedFrameList_append(struct DecodedFrameList *t,
							struct DecodedFrame *pFrame);
    /* Append the given frame to our tail */

void DecodedFrameList_insertList(struct DecodedFrameList *t,
	 							struct DecodedFrameList *src);
    /* Move the given list to the beginning of our list. */


/* The DecodedFrameList2 class implements a doubly-linked list of */
/* DecodedFrame objects.  It uses the m_pPreviousFrame2 and m_pFutureFrame2 */
/* members of the DecodedFrame class to implement the links. */
/* It is not affects when a frame is decoded as a B frame, and thus */
/* has its reference frame pointers modified. */

struct DecodedFrameList2
{
        struct DecodedFrame               *m_pHead;
        struct DecodedFrame               *m_pTail;
            /* m_pHead points to the first element in the list, and m_pTail */
            /* points to the last.  m_pHead->previous() and m_pTail->future() */
            /* are both NULL. */

		U32							m_uLength;
#ifdef DECODE_FRAME_LIST_MUTEX
		RV89Mutex*					m_pMutex; 
#endif
 /* protects the head & tail from HX threads operating in detachHead() and append() simultaneously */
        /*bool            isEmpty() { return !m_pHead; } */

			/* returns the length of the list */
};

void DecodedFrameList2_Init(struct DecodedFrameList2 *t); 

void DecodedFrameList2_Delete(struct DecodedFrameList2 *t);
struct DecodedFrame*
DecodedFrameList2_detachHead(struct DecodedFrameList2 *t);
void
DecodedFrameList2_append(struct DecodedFrameList2 *t, 
						 struct DecodedFrame *pFrame);


#ifdef __cplusplus
}
#endif

#endif /* FRMLIST_H__ */



