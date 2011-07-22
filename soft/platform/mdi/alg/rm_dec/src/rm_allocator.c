/* ***** BEGIN LICENSE BLOCK *****
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

#include "rm_allocator.h"
//#include "helix_memory.h"

/* Defines */
#define HX_ALLOC_NUM_BLOCKS_ALLOC_INITIAL    8
#define HX_ALLOC_NUM_BLOCKS_ALLOC_INCREASE   8


HX_RESULT rm_allocate_resize_array(rm_allocator* pAlloc,
                                   UINT32        ulNewNumBlocks)
{
    HX_RESULT           retVal    = HXR_INVALID_PARAMETER;
    UINT32              ulSize    = 0;
    rm_allocator_block* pNewBlock = HXNULL;

    if (pAlloc && pAlloc->fpMalloc && pAlloc->fpFree && ulNewNumBlocks &&
        ulNewNumBlocks >= pAlloc->ulNumBlocks)
    {
        /* Set the return value */
        retVal = HXR_OUTOFMEMORY;
        /* Compute the size of the new array */
        ulSize = ulNewNumBlocks * sizeof(rm_allocator_block);
        /* Allocate the new array */
        pNewBlock = (rm_allocator_block*) pAlloc->fpMalloc(pAlloc->pUserMem, ulSize);
        if (pNewBlock)
        {
            /* Null out the array */
            memset(pNewBlock, 0, ulSize);
            /* Clear the return value */
            retVal = HXR_OK;
            /* Do we have any old rm_allocator_block's to copy? */
            if (pAlloc->pBlock && pAlloc->ulNumBlocks)
            {
                /* Copy the old block array to the new block array */
                memcpy(pNewBlock, pAlloc->pBlock, pAlloc->ulNumBlocks * sizeof(rm_allocator_block));
                /* Delete the old array */
                pAlloc->fpFree(pAlloc->pUserMem, pAlloc->pBlock);
            }
            /* Assign the new block array */
            pAlloc->pBlock = pNewBlock;
            /* Assign the new number of blocks */
            pAlloc->ulNumBlocks = ulNewNumBlocks;
        }
    }

    return retVal;
}

HX_RESULT rm_allocator_init(rm_allocator*      pAlloc,
                            void*              pUserMem,
                            rm_malloc_func_ptr fpMalloc,
                            rm_free_func_ptr   fpFree)
{
    HX_RESULT retVal = HXR_INVALID_PARAMETER;
    //UINT32    ulSize = 0;

    if (pAlloc && fpMalloc && fpFree)
    {
        /* Clean up any existing rm_allocator struct */
        rm_allocator_cleanup(pAlloc);
        /* Save the function pointers */
        pAlloc->pUserMem = pUserMem;
        pAlloc->fpMalloc = fpMalloc;
        pAlloc->fpFree   = fpFree;
        /* Allocate an initiall number of rm_allocator_block structs */
        retVal = rm_allocate_resize_array(pAlloc, HX_ALLOC_NUM_BLOCKS_ALLOC_INITIAL);
    }

    return retVal;
}

HX_RESULT rm_allocator_preallocate(rm_allocator* pAlloc,
                                   UINT32        ulNumBuffers,
                                   UINT32        ulBufferSize)
{
    HX_RESULT retVal       = HXR_INVALID_PARAMETER;
    UINT32    i            = 0;
    BYTE*     pBuf         = HXNULL;
    UINT32    ulNumCurrent = 0;

    if (pAlloc && pAlloc->pBlock && pAlloc->ulNumBlocks)
    {
        /* Clear the return value */
        retVal = HXR_OK;
        /* Loop through the blocks */
        for (i = 0; i < pAlloc->ulNumBlocks; i++)
        {
            /* Is this buffer big enough and not being used? */
            if (pAlloc->pBlock[i].pPtr                   &&
                pAlloc->pBlock[i].ulSize >= ulBufferSize &&
                pAlloc->pBlock[i].ulRefCount == 1)
            {
                ulNumCurrent++;
            }
        }
        /* Do we already have enough unused buffers of this size? */
        if (ulNumCurrent < ulNumBuffers)
        {
            /* Reduce the number we need to create by the number we already have */
            ulNumBuffers -= ulNumCurrent;
            /* Loop ulNumBuffers times */
            for (i = 0; i < ulNumBuffers && retVal == HXR_OK; i++)
            {
                /*
                 * Get a buffer of size ulBufferSize and force a buffer
                 * to be created and not pulled from the buffer pool.
                 */
                retVal = rm_allocator_get_buffer(pAlloc, ulBufferSize, TRUE, &pBuf);
                if (retVal == HXR_OK)
                {
                    /*
                     * We don't really need the buffer now, so release our ref on it.
                     * This will not delete it, but just rather make it available
                     * for later re-use.
                     */
                    retVal = rm_allocator_release_buffer(pAlloc, pBuf);
                }
            }
        }
    }

    return retVal;
}

HX_RESULT rm_allocator_get_buffer(rm_allocator* pAlloc,
                                  UINT32        ulSize,
                                  HXBOOL        bNoReUse,
                                  BYTE**        ppBuf)
{
    HX_RESULT retVal      = HXR_INVALID_PARAMETER;
    UINT32    i           = 0;
    UINT32    ulDiff      = 0;
    UINT32    ulBestDiff  = 0;
    UINT32    ulBestIndex = 0;

    if (pAlloc && pAlloc->pBlock && pAlloc->ulNumBlocks &&
        pAlloc->fpMalloc && ulSize && ppBuf)
    {
        /* Set the out parameter default */
        *ppBuf = HXNULL;
        /* Set the return value */
        retVal = HXR_FAIL;
        /* Are we allowed to re-use? */
        if (!bNoReUse)
        {
            /*
             * Yes, we can re-use. Search for a block that we can re-use.
             * We want to find the block whose size is >= ulSize, but
             * has the smallest difference between the requested size
             * and this size.
             */
            ulBestIndex = pAlloc->ulNumBlocks;
            ulBestDiff  = 0xFFFFFFFF;
            for (i = 0; i < pAlloc->ulNumBlocks; i++)
            {
                /* Is this buffer big enough and unused? */
                if (pAlloc->pBlock[i].pPtr             &&
                    pAlloc->pBlock[i].ulSize >= ulSize &&
                    pAlloc->pBlock[i].ulRefCount == 1)
                {
                    /* Compute the difference between this buffer's size and our desired size */
                    ulDiff = pAlloc->pBlock[i].ulSize - ulSize;
                    /* Is this diff the best so far? */
                    if (ulDiff < ulBestDiff)
                    {
                        ulBestDiff  = ulDiff;
                        ulBestIndex = i;
                    }
                }
            }
            /* Did we find a buffer to re-use? */
            if (ulBestIndex < pAlloc->ulNumBlocks)
            {
                /* We found a buffer to re-use, so set the out parameter */
                *ppBuf = pAlloc->pBlock[ulBestIndex].pPtr;
                /* Increase the ref count for this buffer */
                pAlloc->pBlock[ulBestIndex].ulRefCount++;
                /* Clear the return value */
                retVal = HXR_OK;
            }
        }
        /* Did we find a buffer to re-use? */
        if (retVal == HXR_FAIL)
        {
            /* Clear the return value */
            retVal = HXR_OK;
            /*
             * We did not find a buffer to re-use, so we will
             * have to allocate one. Do we have space for it?
             * Search the existing blocks and find an empty slot.
             */
            for (i = 0; i < pAlloc->ulNumBlocks; i++)
            {
                /* Is this block empty? */
                if (pAlloc->pBlock[i].pPtr == HXNULL &&
                    pAlloc->pBlock[i].ulSize == 0    &&
                    pAlloc->pBlock[i].ulRefCount == 0)
                {
                    /* We can use this block */
                    break;
                }
            }
            /* Did we find an empty slot? */
            if (i == pAlloc->ulNumBlocks)
            {
                /* We don't have an empty slot, so we'll have to increase the array */
                retVal = rm_allocate_resize_array(pAlloc, pAlloc->ulNumBlocks + HX_ALLOC_NUM_BLOCKS_ALLOC_INCREASE);
            }
            if (retVal == HXR_OK)
            {
                /* Sanity check */
                if (i < pAlloc->ulNumBlocks)
                {
                    /* Set the return value */
                    retVal = HXR_OUTOFMEMORY;
                    /* Allocate a buffer of size ulSize */
                    pAlloc->pBlock[i].pPtr = pAlloc->fpMalloc(pAlloc->pUserMem, ulSize);
                    if (pAlloc->pBlock[i].pPtr)
                    {
                        /* Null out this buffer */
                        memset(pAlloc->pBlock[i].pPtr, 0, ulSize);
                        /* Save the size */
                        pAlloc->pBlock[i].ulSize = ulSize;
                        /* Set the ref count to 2 (one for allocator, one for out param) */
                        pAlloc->pBlock[i].ulRefCount = 2;
                        /* Set the out param */
                        *ppBuf = pAlloc->pBlock[i].pPtr;
                        /* Clear the return value */
                        retVal = HXR_OK;
                    }
                }
                else
                {
                    /* Something went wrong, so set return value */
                    retVal = HXR_FAIL;
                }
            }
        }
    }

    return retVal;
}

HX_RESULT rm_allocator_addref_buffer(rm_allocator* pAlloc,
                                     BYTE*         pBuf)
{
    HX_RESULT retVal = HXR_INVALID_PARAMETER;
    UINT32    i      = 0;

    if (pAlloc && pAlloc->pBlock && pAlloc->ulNumBlocks && pBuf)
    {
        /* Loop through all the existing blocks */
        for (i = 0; i < pAlloc->ulNumBlocks; i++)
        {
            /* Is this the buffer we want? */
            if (pBuf == pAlloc->pBlock[i].pPtr)
            {
                /* This is the buffer, so increase its ref count */
                pAlloc->pBlock[i].ulRefCount++;
                /* Break out of the loop */
                break;
            }
        }
        /* Did we find the buffer? */
        if (i < pAlloc->ulNumBlocks)
        {
            /* Clear the return value */
            retVal = HXR_OK;
        }
    }

    return retVal;
}

HX_RESULT rm_allocator_release_buffer(rm_allocator* pAlloc,
                                      BYTE*         pBuf)
{
    HX_RESULT retVal = HXR_INVALID_PARAMETER;
    UINT32    i      = 0;

    if (pAlloc && pAlloc->fpFree && pAlloc->pBlock && pAlloc->ulNumBlocks && pBuf)
    {
        /* Loop through all the existing blocks */
        for (i = 0; i < pAlloc->ulNumBlocks; i++)
        {
            /* Is this the buffer we want? */
            if (pBuf == pAlloc->pBlock[i].pPtr)
            {
                /* This is the buffer, so decrease its ref count (if we can) */
                if (pAlloc->pBlock[i].ulRefCount)
                {
                    pAlloc->pBlock[i].ulRefCount--;
                }
                /* Has the ref count fallen to zero? */
                if (pAlloc->pBlock[i].ulRefCount == 0)
                {
                    /* Free the buffer */
                    pAlloc->fpFree(pAlloc->pUserMem, pAlloc->pBlock[i].pPtr);
                    /* Null out the pointer */
                    pAlloc->pBlock[i].pPtr = HXNULL;
                    /* Zero out the size */
                    pAlloc->pBlock[i].ulSize = 0;
                }
                /* Break out of the loop */
                break;
            }
        }
        /* Did we find the buffer? */
        if (i < pAlloc->ulNumBlocks)
        {
            /* Clear the return value */
            retVal = HXR_OK;
        }
    }

    return retVal;
}

void rm_allocator_cleanup(rm_allocator* pAlloc)
{
    UINT32 i = 0;

    if (pAlloc)
    {
        /* Are there any rm_allocator_block structs? */
        if (pAlloc->pBlock && pAlloc->ulNumBlocks)
        {
            /* Loop through all the rm_allocator_block structs */
            for (i = 0; i < pAlloc->ulNumBlocks; i++)
            {
                if (pAlloc->pBlock[i].pPtr && pAlloc->pBlock[i].ulSize)
                {
                    /* Release this buffer */
                    rm_allocator_release_buffer(pAlloc, pAlloc->pBlock[i].pPtr);
                }
            }
            /* Free the array of rm_allocator_block structs */
            if (pAlloc->fpFree)
            {
                /* Free the array */
                pAlloc->fpFree(pAlloc->pUserMem, pAlloc->pBlock);
            }
            /* Null out the memory */
            pAlloc->pBlock = HXNULL;
            /* Zero out the number of structs */
            pAlloc->ulNumBlocks = 0;
        }
        /* Null out the function pointers */
        pAlloc->pUserMem = HXNULL;
        pAlloc->fpMalloc = HXNULL;
        pAlloc->fpFree   = HXNULL;
    }
}
