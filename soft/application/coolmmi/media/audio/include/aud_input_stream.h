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
*	aud_input_stream.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   audio input stream functions
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log$
 *
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/
#ifndef __AUDIO_INPUT_STREAM_H__
#define __AUDIO_INPUT_STREAM_H__

#include "fat_fs.h"

#define AUDIO_INPUT_STREAM_BUFFER_SIZE (4*1024)

typedef struct
{
	kal_uint8 buf[AUDIO_INPUT_STREAM_BUFFER_SIZE + 1]; // one byte reserved for tail reference to a valid address when it's full
	kal_uint8* ptr;
	kal_uint8* tail;
	kal_uint32 buf_size;
	kal_uint32 skip;
	kal_uint32 total_load;
	kal_uint32 free_bitrate; // bps
	FS_HANDLE file;
	kal_uint8* mem_src; // when file is null, the input soruce is from memory so mem_src must be set
	kal_uint32 mem_src_size;
} 
audio_input_stream_struct;

kal_uint8 aud_get_bits( kal_uint8 *buff, kal_uint32 *pbitcount, kal_uint32 bitlen );

audio_input_stream_struct* audio_file_input_stream_alloc( FS_HANDLE file );

audio_input_stream_struct* audio_byte_input_stream_alloc( kal_uint8* mem_src, kal_uint32 mem_src_size );

void audio_input_stream_free( audio_input_stream_struct* input_stream );

void audio_input_stream_init( audio_input_stream_struct *input_stream, FS_HANDLE file, kal_uint8* mem_src, kal_uint32 mem_src_size );

void audio_input_stream_seek( audio_input_stream_struct* input_stream, kal_int32 offset );

kal_int32 audio_input_stream_get_data_size( audio_input_stream_struct* input_stream );

kal_int32 audio_input_stream_read( audio_input_stream_struct *input_stream, kal_int32 size );

#endif // __AUDIO_INPUT_STREAM_H__
