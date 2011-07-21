/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
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

/*******************************************************************************
 * Filename:
 * ---------
 *  ResBytestream.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/Resource/include/ResBytestream.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:02:50   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:43:12   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:28:12   zhangxb
 * Initial revision.
 * Revision 1.1.1.1  2006/06/08 12:43:25  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
	External tool required for the MMI

	Filename:		bytestream.h
	Date Created:	June-10-2002
	Contains:		bytestream data type header file
					These are used to process ROM images of binary files
**********************************************************************************/

#ifndef __BYTESTREAM_H__
#define __BYTESTREAM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include "resdevice.h"

    typedef struct _bytestream
    {
        long int size;
        long int current_offset;
        U8 *data;
    } bytestream;

    extern void bytestream_initialize(bytestream *file, U8 *data, long int size);
    extern void bytestream_fclose(bytestream *file);
    extern U8 bytestream_fseek(bytestream *file, long int offset, int mode);
    extern U8 bytestream_feof(bytestream *file);
    extern U8 bytestream_fgetbyte(bytestream *file);
    extern U8 bytestream_fputbyte(bytestream *file, U8 c);
    extern size_t bytestream_fread(U8 *buffer, size_t size, size_t number, bytestream *file);
    extern U16 bytestream_fgetword(bytestream *file);
    extern U32 bytestream_fgetdword(bytestream *file);
    extern U8 bytestream_fputword(bytestream *file, U16 w);
    extern U8 bytestream_fputword_bigendian(bytestream *file, U16 w);
    extern U8 bytestream_fputdword(bytestream *file, U32 d);
    extern U8 bytestream_fputdword_bigendian(bytestream *file, U32 d);
    extern U16 bytestream_fgetword_bigendian(bytestream *file);
    extern U32 bytestream_fgetdword_bigendian(bytestream *file);
    /* Used to read variable length data: Useful only for MIDI files  */
    extern U32 bytestream_fgetvdata(bytestream *file);

#ifdef __cplusplus
}
#endif

#endif
