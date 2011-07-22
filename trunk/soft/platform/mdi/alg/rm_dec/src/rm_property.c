/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: rm_property.c,v 1.5 2007/12/18 17:04:59 ehyche Exp $
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

#include "helix_types.h"
//#include "helix_memory.h"
//#include "helix_string.h"
#include "rm_property.h"

const char* rm_property_get_name(rm_property* prop)
{
    const char* pRet = (const char*)HXNULL;

    if (prop)
    {
        pRet = (const char*) prop->pName;
    }

    return pRet;
}

UINT32 rm_property_get_type(rm_property* prop)
{
    UINT32 ulRet = 0;

    if (prop)
    {
        ulRet = prop->ulType;
    }

    return ulRet;
}

UINT32 rm_property_get_value_uint32(rm_property* prop)
{
    UINT32 ulRet = 0;

    if (prop)
    {
        ulRet = (UINT32) prop->pValue;
    }

    return ulRet;
}

const char* rm_property_get_value_cstring(rm_property* prop)
{
    const char* pRet = (const char*)HXNULL;

    if (prop)
    {
        pRet = (const char*) prop->pValue;
    }

    return pRet;
}

UINT32 rm_property_get_value_buffer_length(rm_property* prop)
{
    UINT32 ulRet = 0;

    if (prop)
    {
        ulRet = prop->ulValueLen;
    }

    return ulRet;
}

BYTE* rm_property_get_value_buffer(rm_property* prop)
{
    BYTE* pRet = (BYTE*)HXNULL;

    if (prop)
    {
        pRet = prop->pValue;
    }

    return pRet;
}

void rm_property_cleanup(rm_property* pProp, rm_free_func_ptr fpFree, void* pUserMem)
{
    if (pProp && fpFree)
    {
        /* Cleanup the name */
        if (pProp->pName)
        {
            fpFree(pUserMem, pProp->pName);
        }
        /* Cleanup the value */
        if (pProp->pValue &&
            (pProp->ulType == RM_PROPERTY_TYPE_BUFFER ||
             pProp->ulType == RM_PROPERTY_TYPE_CSTRING))
        {
            fpFree(pUserMem, pProp->pValue);
        }
        /* Zero out the struct */
        memset(pProp, 0, sizeof(rm_property));
    }
}

void rm_property_set_cleanup(rm_property_set* pSet, rm_free_func_ptr fpFree, void* pUserMem)
{
    UINT32 i = 0;

    if (pSet && fpFree)
    {
        /* Do we have any properties? */
        if (pSet->ulNumProperties && pSet->property)
        {
            /* Clean up the individual properties */
            for (i = 0; i < pSet->ulNumProperties; i++)
            {
                rm_property_cleanup(&pSet->property[i], fpFree, pUserMem);
            }
            /* Clean up the array of properties */
            fpFree(pUserMem, pSet->property);
        }
        /* Zero out the struct */
        memset(pSet, 0, sizeof(rm_property_set));
    }
}

HX_RESULT find_property_in_set(rm_property_set* pSet, const char* pszName, rm_property** ppProp)
{
    HX_RESULT retVal = HXR_FAIL;
    UINT32    i      = 0;

    if (pSet && pszName && ppProp && pSet->ulNumProperties && pSet->property)
    {
        for (i = 0; i < pSet->ulNumProperties; i++)
        {
            if (pSet->property[i].pName &&
                !strcmp(pSet->property[i].pName, pszName))
            {
                break;
            }
        }
        if (i < pSet->ulNumProperties)
        {
            *ppProp = &pSet->property[i];
            retVal  = HXR_OK;
        }
    }

    return retVal;
}

HXBOOL is_property_present(rm_property_set* pSet, const char* pszName, UINT32* pulType)
{
    HXBOOL       bRet  = FALSE;
    HX_RESULT    rv    = HXR_OK;
    rm_property* pProp = NULL;

    if (pulType)
    {
        rv = find_property_in_set(pSet, pszName, &pProp);
        if (rv == HXR_OK)
        {
            bRet     = TRUE;
            *pulType = pProp->ulType;
        }
    }

    return bRet;
}

HX_RESULT get_rm_property_int(rm_property_set* pSet,
                              const char*      pszStr,
                              UINT32*          pulVal)
{
    HX_RESULT    retVal = HXR_FAIL;
    rm_property* pProp  = NULL;

    if (pulVal)
    {
        retVal = find_property_in_set(pSet, pszStr, &pProp);
        if (retVal == HXR_OK)
        {
            *pulVal = (UINT32) pProp->pValue;
        }
    }

    return retVal;
}

HX_RESULT get_rm_property_buf(rm_property_set* pSet,
                              const char*      pszStr,
                              BYTE**           ppBuf,
                              UINT32*          pulLen)
{
    HX_RESULT    retVal = HXR_FAIL;
    rm_property* pProp  = NULL;

    if (ppBuf && pulLen)
    {
        retVal = find_property_in_set(pSet, pszStr, &pProp);
        if (retVal == HXR_OK)
        {
            *ppBuf  = pProp->pValue;
            *pulLen = pProp->ulValueLen;
        }
    }

    return retVal;
}

HX_RESULT get_rm_property_str(rm_property_set* pSet,
                              const char*      pszStr,
                              char**           ppszStr)
{
    HX_RESULT    retVal = HXR_FAIL;
    rm_property* pProp  = NULL;

    if (ppszStr)
    {
        retVal = find_property_in_set(pSet, pszStr, &pProp);
        if (retVal == HXR_OK)
        {
            *ppszStr = (char*) pProp->pValue;
        }
    }

    return retVal;
}
