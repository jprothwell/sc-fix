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
*	aud_amr_parser.c
*
* Project:
* --------
 
*
* Description:
* ------------
*   amr parsing process
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
#ifndef MED_NOT_PRESENT

#include "med_global.h"

#ifdef __RICH_AUDIO_PROFILE__

#include "aud_input_stream.h"
#include "drm_gprot.h"

const kal_uint8 AMR_FRAME_SIZE_TAB[16] =
{
   12, 13, 15, 17,
   19, 20, 26, 31,
   5, 0xFF, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0
};

const kal_uint8 AMRWB_FRAME_SIZE_TAB[16] =
{
   17, 23, 32, 36,
   46, 50, 58,
   60, 5, 0xFF, 0xFF,
   0xFF, 0xFF, 0, 0 
};

kal_int16 check_amr_header_and_get_frame_size( kal_uint8 frame_header, kal_bool is_wideband )
{
   kal_uint8 frame_type;
   kal_uint8 frame_size;
   kal_uint8 FQI;
   
   if( frame_header & 0x83 )
      return -1;
   frame_type = (kal_uint8)((frame_header >> 3) & 0x0F);
   frame_size = is_wideband ? AMRWB_FRAME_SIZE_TAB[frame_type] : AMR_FRAME_SIZE_TAB[frame_type];
   if( frame_size == 0xFF )
      return -1;
   FQI = (kal_uint8)((frame_header >> 2) & 0x01);
   if( frame_type == 15 && FQI != 1 )
      return -1;
   if( is_wideband )
   {
      if( frame_type == 14 && FQI != 0 )
      	return -1;
   }
   return (kal_int16)frame_size;
}

int goto_amr_data_frame( FS_HANDLE fd, kal_uint32 seek_offset, kal_bool is_wideband )
{
	kal_bool out_of_limit;
	kal_int16 frame_size;
	kal_uint32 len, offset, search_offset, found_count, match_offset = 0;
	kal_uint8 *ptr;
	audio_input_stream_struct *input_stream;

	input_stream = audio_file_input_stream_alloc( fd );
	audio_input_stream_seek( input_stream, (kal_int32) seek_offset );
	audio_input_stream_read( input_stream, 0 );
	len = (kal_uint32) (input_stream->tail - input_stream->ptr);
	ptr = input_stream->buf;
	for( offset = 0; offset < len; offset++ )
	{
		search_offset = offset;
		found_count = 0;
		// found 5 valid frames consecutively
		while( (frame_size = check_amr_header_and_get_frame_size( ptr[search_offset], is_wideband )) >= 0 )
		{
			// increment found count
			found_count++;
			// store the 2nd match offset
			if( found_count == 2 )
				match_offset = search_offset;
			// update search offset to next frame
			search_offset += (frame_size+1);
			// check search boundary
			if( search_offset >= len )
				out_of_limit = 1;
			else
				out_of_limit = 0;
			if( found_count >= 5 || (out_of_limit && found_count >= 3) )
			{
				DRM_seek_file( fd, (kal_int32)match_offset - (kal_int32)len, FS_FILE_CURRENT );
				audio_input_stream_free( input_stream );
				return match_offset ;
			}
			else if( out_of_limit )
			{
				audio_input_stream_free( input_stream );
				return -1;
			}
		}
	}
	audio_input_stream_free( input_stream );
	return -1;
/*
	kal_int16 frame_size, target_len;
	kal_uint32 len, offset;
	
	DRM_seek_file( fd, prev_offset, FS_FILE_CURRENT );
	target_len = seek_offset - prev_offset;
	DRM_read_file( fd, data_buffer, BUF_SIZE, &len );
	for( offset = 0; offset < len; )
	{
		frame_size = check_amr_header_and_get_frame_size( data_buffer[offset], is_wideband );
		ASSERT( frame_size >= 0 )
		offset += (frame_size+1);
		if( offset >= target_len )
		{
			DRM_seek_file( fd, (kal_int32)offset - (kal_int32)len, FS_FILE_CURRENT );
			offset -=  target_len;
			return offset ;
		}
	}
	
	return -1;
*/
}

#endif // __RICH_AUDIO_PROFILE__

#endif // MED_NOT_PRESENT

