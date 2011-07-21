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
*	aud_daf_parser.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   daf parsing definitions
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/
#ifndef __DAF_PARSER_H__
#define __DAF_PARSER_H__

#include "aud_id3_parser.h"

#define DAF_FRAME_CHECK_SIZE (20*1024)

typedef enum
{
	DAF_VERSION_1,
	DAF_VERSION_2,
	DAF_VERSION_NONE,
	DAF_VERSION_2_5
}
DAF_VERSION_ID_ENUM;

typedef enum
{
	DAF_LAYER_I,
	DAF_LAYER_II,
	DAF_LAYER_III,
	DAF_LAYER_NONE
}
DAF_LAYER_ENUM;

typedef enum
{
	DAF_CHECK_FRAME_TRUE,
	DAF_CHECK_FRAME_FALSE,
	DAF_CHECK_FRAME_EOF
}
DAF_CHECK_NEXT_FRAME_RESULT_ENUM;

typedef enum
{
	DAF_FIND_FRAME_TRUE,
	DAF_FIND_FRAME_FALSE,
	DAF_FIND_FRAME_FREE_BITRATE
}
DAF_FIND_NEXT_FRAME_RESULT_ENUM;

typedef struct
{
	kal_uint32 duration; // seconds
	kal_uint32 bitrate; // bps
	kal_uint32 samplerate;
	kal_uint32 channel_num;
}
daf_data_info_struct;

typedef struct
{
	kal_uint8 version; // DAF_VERSION_ID_ENUM
	kal_uint8 layer; // DAF_LAYER_ENUM
	kal_uint8 channel_num; // 1 -> mono, 2 -> stereo
	kal_bool padding; // 1 -> yes, 0 -> no
	kal_uint32 sample_rate; // bps
	kal_uint32 bitrate; // bps
	kal_uint32 min_frame_size;
	kal_uint32 max_frame_size;
	kal_uint32 frame_size;
}
daf_header_struct;

kal_bool daf_parse_frame_header( kal_uint8 *data, daf_header_struct* header );

kal_bool daf_find_next_frame( audio_input_stream_struct *input_stream, daf_header_struct* header, kal_uint32 move_offset );

kal_uint8 daf_check_next_frame( audio_input_stream_struct *input_stream, daf_header_struct* prev_header, kal_int32 min_next_pos, kal_int32 max_next_pos, kal_int32 *next_pos );

kal_bool daf_parse( audio_input_stream_struct *input_stream, kal_uint32* offset, daf_data_info_struct* info_p );

void daf_set_data_info( aud_info_struct *info_p, kal_uint32 channel_num, kal_uint32 duration, kal_uint32 size, kal_uint32 bitrate, kal_uint32 samplerate );

void daf_parse_data_info( aud_info_struct *info_p, audio_input_stream_struct* input_stream, id3_tag_struct* id3_tag );

kal_bool daf_seek( audio_input_stream_struct *input_stream, kal_uint32* offset );

kal_bool get_aud_info( kal_wchar* filename, aud_info_struct* aud_info_p );

#endif // __DAF_PARSER_H__
