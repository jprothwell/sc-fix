/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: parse_sdp_chunk.c,v 1.1 2007/12/18 17:04:59 ehyche Exp $
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

#include "parse_sdp_chunk.h"
//#include "helix_string.h"
//#include "helix_ctype.h"
//#include "helix_stdlib.h"
#ifndef SHEEN_VC_DEBUG
extern unsigned long strtoul(const char *nptr, char **endptr, int base);
#endif

UINT32 determine_property_type(const char* pszStr, UINT32 ulLen)
{
    UINT32 ulRet     = RM_PROPERTY_TYPE_CSTRING;
    HXBOOL bAllDigit = TRUE;
    UINT32 i         = 0;

    if (pszStr && ulLen)
    {
        /* Run through all characters and see if any are non-digits */
        for (i = 0; i < ulLen; i++)
        {
            if (!isdigit((int) pszStr[i]))
            {
                bAllDigit = FALSE;
                break;
            }
        }
        /* Set the return value property type */
        ulRet = (bAllDigit ? RM_PROPERTY_TYPE_UINT32 : RM_PROPERTY_TYPE_CSTRING);
    }

    return ulRet;
}

HX_RESULT parse_sdp_chunk(const char*         pszStr,
                          rm_malloc_func_ptr  fpMalloc,
                          rm_free_func_ptr    fpFree,
                          void*               pUserMem,
                          UINT32*             pulNumProp,
                          rm_property*        pProp)
{
    HX_RESULT   retVal = HXR_FAIL;
    const char* pszWS  = " \t\r\n";
    const char* pszWSS = " \t\r\n;";
    const char* pszWSE = " \t\r\n=";
    UINT32      ulLen  = 0;
    UINT32      ulIndx = 0;
    UINT32      ulNum  = 0;
    UINT32      ulBeg  = 0;
    UINT32      ulSize = 0;

    if (pszStr && fpMalloc && fpFree && pulNumProp)
    {
        /* Clear the return value */
        retVal = HXR_OK;
        /* Get the length of the input string */
        ulLen = (UINT32) strlen(pszStr);
        if (ulLen > 0)
        {
            /* Find the first whitespace in the string */
            ulIndx = (UINT32) strcspn(pszStr, pszWS);
            if (ulIndx < ulLen)
            {
                /* Loop through the string, parsing the name value pairs */
                while (ulIndx < ulLen)
                {
                    /* Look for the beginning of the name */
                    ulIndx += (UINT32) strspn(pszStr + ulIndx, pszWSS);
                    if (ulIndx < ulLen)
                    {
                        /* Save the begin index */
                        ulBeg = ulIndx;
                        /* Look for the end of the name */
                        ulIndx += (UINT32) strcspn(pszStr + ulIndx, pszWSE);
                        if (ulIndx < ulLen)
                        {
                            /* Save the property name */
                            if (pProp && ulNum < *pulNumProp)
                            {
                                /* Compute the name string size */
                                ulSize = ulIndx - ulBeg;
                                /* Allocate a buffer to hold the name */
                                pProp[ulNum].pName = fpMalloc(pUserMem, ulSize + 1);
                                if (pProp[ulNum].pName)
                                {
                                    /* Copy the name string */
                                    if (ulIndx - ulBeg > 0)
                                    {
                                        memcpy(pProp[ulNum].pName, pszStr + ulBeg, ulSize);
                                    }
                                    /* Set the NULL terminator */
                                    pProp[ulNum].pName[ulSize] = '\0';
                                }
                            }
                            /* Look for the beginning of the value */
                            ulIndx += (UINT32) strspn(pszStr + ulIndx, pszWSE);
                            if (ulIndx < ulLen)
                            {
                                /* Save the beginning of the value */
                                ulBeg = ulIndx;
                                /* Look for the end of the value */
                                ulIndx += (UINT32) strcspn(pszStr + ulIndx, pszWSS);
                                if (ulIndx < ulLen)
                                {
                                    /* Save the property value */
                                    if (pProp && ulNum < *pulNumProp)
                                    {
                                        /* Compute the size of the property value */
                                        ulSize = ulIndx - ulBeg;
                                        /* Determine the property type */
                                        pProp[ulNum].ulType = determine_property_type(pszStr + ulBeg, ulSize);
                                        /* Is this a UINT32 property type? */
                                        if (pProp[ulNum].ulType == RM_PROPERTY_TYPE_UINT32)
                                        {
                                            /* Convert string to a UINT32 */
                                            pProp[ulNum].pValue = (BYTE*) strtoul(pszStr + ulBeg, NULL, 10);
                                        }
                                        else if (pProp[ulNum].ulType == RM_PROPERTY_TYPE_CSTRING)
                                        {
                                            /* This is a string property, so allocate a buffer to hold it */
                                            pProp[ulNum].pValue = (BYTE*) fpMalloc(pUserMem, ulSize + 1);
                                            if (pProp[ulNum].pValue)
                                            {
                                                /* Copy the value string */
                                                if (ulIndx - ulBeg > 0)
                                                {
                                                    memcpy(pProp[ulNum].pValue, pszStr + ulBeg, ulSize);
                                                }
                                                /* Set the NULL terminator */
                                                pProp[ulNum].pValue[ulSize] = '\0';
                                            }
                                        }
                                    }
                                    /* Increment the number of properties */
                                    ulNum++;
                                }
                            }
                        }
                    }
                }
                /* Save the number of name-value property pairs found */
                *pulNumProp = ulNum;
            }
        }
    }

    return retVal;
}

HX_RESULT parse_property_set_from_sdp_chunk(const char*         pszStr,
                                            rm_malloc_func_ptr  fpMalloc,
                                            rm_free_func_ptr    fpFree,
                                            void*               pUserMem,
                                            rm_property_set*    pSet)
{
    HX_RESULT retVal = HXR_FAIL;
    UINT32    ulSize = 0;

    if (pszStr && fpMalloc && fpFree && pSet)
    {
        /* Clean up any existing property set */
        rm_property_set_cleanup(pSet, fpFree, pUserMem);
        /* Parse the SDP string once to determine the number of properties */
        retVal = parse_sdp_chunk(pszStr, fpMalloc, fpFree, pUserMem, &pSet->ulNumProperties, NULL);
        if (retVal == HXR_OK)
        {
            /* Did we detect any name-value pairs? */
            if (pSet->ulNumProperties > 0)
            {
                /* Set the return value */
                retVal = HXR_OUTOFMEMORY;
                /* Allocate a buffer big enough for the property array */
                ulSize = pSet->ulNumProperties * sizeof(rm_property);
                pSet->property = (rm_property*) fpMalloc(pUserMem, ulSize);
                if (pSet->property)
                {
                    /* Zero out the array */
                    memset(pSet->property, 0, ulSize);
                    /* Now parse the SDP string a second time, this time saving the properties */
                    retVal = parse_sdp_chunk(pszStr, fpMalloc, fpFree, pUserMem, &pSet->ulNumProperties, pSet->property);
                }
            }
        }
    }

    return retVal;
}
