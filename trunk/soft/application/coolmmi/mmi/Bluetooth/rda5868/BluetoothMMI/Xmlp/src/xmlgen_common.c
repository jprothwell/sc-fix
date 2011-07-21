/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *  xmlgen_common.c
 *
 * Project:
 * -------- 
 
 *
 * Description:
 * ------------
 *  xml generator file
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
  
 *============================================================================
 ****************************************************************************/
#if (defined(__XML_SUPPORT__) || defined(__WBXML_SUPPORT__))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kal_release.h"        /* basic data type */
//#include "app_buff_alloc.h"

#include "fs_type.h"
#include "fat_fs.h"
#include "oslmemory.h"
#include "xmlgen_common.h"

#define COM_BUF_SIZE    512




/*****************************************************************************
 * FUNCTION
 *  xml_generate_write_filetobuf
 * DESCRIPTION
 *  this function is used to write data from file to buffer
 * PARAMETERS
 *  dest        [IN]    dest buffer
 *  src         [IN]    src file handle  
 *  len         [IN]    actual length to be written
 * RETURNS
 *      <0 : means failed, returns error code;
 *      >0 :  means succeed
 *****************************************************************************/

kal_int32 xml_generate_write_filetobuf (char * dest, FS_HANDLE src, kal_uint32 *len)
{
    char *load = NULL, *p;
    INT32 length;
    kal_int32 ret;
    

    load = OslMalloc(COM_BUF_SIZE);
    if (load == NULL)
    {
        return XG_E_NO_MEMORY;
    }
    p = dest;
    *len = 0;
    while ((ret = MMI_FS_Read(src, load, COM_BUF_SIZE-1, &length)) >= FS_NO_ERROR)
    {
        if (length == 0)
        {
            break;
        }
        memcpy(p, load, length);
        p += length;
        *len += length;
    }
    OslMfree(load);
    if (ret < FS_NO_ERROR)
    {
        return XG_E_FAIL; 
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************
 * FUNCTION
 *  xml_generate_write_filetofile
 * DESCRIPTION
 *  this function is used to write data from file to file
 * PARAMETERS
 *  dest        [IN]    dest file handle which data will be copy to
 *  src         [IN]    src file handle 
 *  len         [IN]    actual length to be written
 * RETURNS
 *      <0 : means failed, returns error code;
 *      >0 :  means succeed
 *****************************************************************************/

kal_int32 xml_generate_write_filetofile(FS_HANDLE dest, FS_HANDLE src, kal_uint32 *len)
{
    char *load = NULL;
    INT32 len_read, len_write;
    kal_int32 ret;
    

    load = OslMalloc(COM_BUF_SIZE);
    if (load == NULL)
    {
        return XG_E_NO_MEMORY;
    }

    *len = 0;
    while ((ret = MMI_FS_Read(src, load, COM_BUF_SIZE-1, &len_read)) >= FS_NO_ERROR)
    {
        if (len_read == 0)
        {
            break;
        }
        ret = MMI_FS_Write(dest, load, len_read, &len_write);
        if (ret < FS_NO_ERROR)
        {
            OslMfree(load);            
            return XG_E_FAIL;
        }
        *len += len_write;
    }
    OslMfree(load);
    if (ret < FS_NO_ERROR)
    {
        return XG_E_FAIL; 
    }
    else
    {
        return 0;
    }
}


#endif     /* (defined(__XML_SUPPORT__) || defined(__WBXML_SUPPORT__)) */
