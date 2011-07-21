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
*	daf_parser.c
*
* Project:
* --------
 
*
* Description:
* ------------
*   daf parsing process
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
#ifndef MED_NOT_PRESENT

#include "med_global.h"

#ifdef __RICH_AUDIO_PROFILE__

#include "aud_daf_parser.h"
#include "drm_gprot.h"
#include "med_main.h"
#include "mmi_trace.h"
// import from other files 
#ifdef __AUD_NEW_INTERFACE__
#include "l1audio.h"
#include "fsal.h"
typedef Media_Status (*media_get_info_func_ptr)( STFSAL *pstFSAL, audInfoStruct *contentInfo );
#ifdef WMA_DECODE
extern Media_Status WMA_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo );
#endif // WMA_DECODE
#ifdef AAC_DECODE
extern Media_Status AAC_GetContentDescInfo( STFSAL *pstFSAL, audInfoStruct *contentInfo );
#endif // AAC_DECODE
#endif // __AUD_NEW_INTERFACE__
extern void csd_wsprintf(unsigned short *outstr, char *fmt,...);

extern id3_tag_struct id3_tag; // from aud_id3_parser.c
extern kal_int16 med_get_media_type(kal_wchar* file_name); // from med_utility.c

// DAF_VERSION_1
kal_uint16 const DAF_V1_BITRATE_TAB[3][15] = 
{
	{ 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448 }, // Layer I
	{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384 }, // Layer II
	{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 }, // Layer III
};
// DAF_VERSION_2, DAF_VERSION_2_5
kal_uint16 const DAF_V2_BITRATE_TAB[2][15] = 
{
	{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256 }, // Layer I
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160 } // Layer II & III
};

kal_uint16 const DAF_SAMPLERATE_TAB[3] = { 44100, 48000, 32000 };

/*****************************************************************************
* FUNCTION
*     daf_parse_frame_header
* DESCRIPTION
*     this function will try to parse frame header from current input stream position to 
*     the range of DAF_FRAME_CHECK_SIZE so even if the target is not a valid daf file,
*     the processing time is bounded.
*
*     syncword					11 bits
*     version					2 bits
*     layer						2 bits
*     protection(CRC) bit		1 bit
*     bitrate index				4 bits
*     sampling frequency index	2 bits
*     padding bit				1 bit
*     private bit				1 bit
*     channel mode				2 bits
*     mode extension			2 bits
*     copyright					1 bit
*     original/home				1 bit
*     emphasis					2 bits
* IMPACT 
*     
* PARAMETERS
*     kal_uint8 *data
*     daf_header_struct* header
* RETURNS
*     kal_int32
* GLOBALS AFFECTED
*     
*****************************************************************************/
kal_bool daf_parse_frame_header( kal_uint8 *data, daf_header_struct* header )
{
	kal_uint8 rate_index, version, layer;
	kal_uint8 slot_bytes;
	kal_uint32 frame_slots;
	kal_uint32 temp;
	kal_uint32 bitcount = 0;

	/* check syncword */
	temp = (kal_uint32)aud_get_bits( data, &bitcount, 8 ) << 3;
	temp |= (kal_uint32)aud_get_bits( data, &bitcount, 3 );
	if( temp != 0x7FF ) // 111 1111 1111
		return KAL_FALSE;
	
	/* version */
	version = 3 - aud_get_bits( data, &bitcount, 2 );
	if( version == DAF_VERSION_NONE )
		return KAL_FALSE;
	header->version = version;
	
	/* layer */
	layer = 3 - aud_get_bits( data, &bitcount, 2 );
	if( layer == DAF_LAYER_NONE )
		return KAL_FALSE;
	header->layer = layer;

	/* protection(CRC) bit */
	aud_get_bits( data, &bitcount, 1 );

	/* bitrate index */
	rate_index = aud_get_bits( data, &bitcount, 4 );
	if( rate_index == 15 ) // bad
		return KAL_FALSE;
	switch( version )
	{
	case DAF_VERSION_1:
		header->bitrate = DAF_V1_BITRATE_TAB[layer][rate_index]; // kbps
		break;
	case DAF_VERSION_2:
	case DAF_VERSION_2_5:
		header->bitrate = DAF_V2_BITRATE_TAB[(layer+1)>>1][rate_index]; // kbps
		break;
	}
	header->bitrate *= 1000; // bps
	
	/* sampling frequency index */
	rate_index = aud_get_bits( data, &bitcount, 2 );
	if( rate_index == 3 ) // reserved
		return KAL_FALSE;
	header->sample_rate = DAF_SAMPLERATE_TAB[rate_index];
	switch( version )
	{
	case DAF_VERSION_2:
		header->sample_rate >>= 1;
		break;
	case DAF_VERSION_2_5:
		header->sample_rate >>= 2;
		break;
	}

	/* padding bit */
	header->padding = aud_get_bits( data, &bitcount, 1 );

	/* private bit */
	aud_get_bits( data, &bitcount, 1 );
	
	/* channel mode */
	header->channel_num = (aud_get_bits( data, &bitcount, 2 ) == 3) ? 1: 2;

	/* mode extension */
	aud_get_bits( data, &bitcount, 2 );
	
	/* copyright */
	aud_get_bits( data, &bitcount, 1 );
	
	/* original/home */
	aud_get_bits( data, &bitcount, 1 );

	/* emphasis */
	if( aud_get_bits( data, &bitcount, 2 ) == 2 ) // reserved
		return KAL_FALSE;

	/* calculate frame length */
	if( header->bitrate != 0 )
	{
		slot_bytes = 1;
		if( layer == DAF_LAYER_I )
		{
			frame_slots = 12;
			slot_bytes = 4;
		}
		else if( (layer == DAF_LAYER_III ) && (version == DAF_VERSION_2 || version == DAF_VERSION_2_5) )
			frame_slots = 72;
		else
			frame_slots = 144;
		
		header->min_frame_size = (frame_slots * header->bitrate / header->sample_rate) * slot_bytes;
		header->max_frame_size = header->min_frame_size + slot_bytes;
	}
	return KAL_TRUE;
}

/*****************************************************************************
* FUNCTION
*     daf_find_next_frame
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     audio_input_stream_struct *input_stream
*     daf_header_struct* header
*     kal_uint32 move_offset
* RETURNS
*     kal_bool
* GLOBALS AFFECTED
*     
*****************************************************************************/
kal_bool daf_find_next_frame( audio_input_stream_struct *input_stream, daf_header_struct* header, kal_uint32 move_offset )
{
	daf_header_struct free_bitrate_header;
	kal_uint32 round, read_size;
	kal_uint32 sample_rate, bitrate, frame_slots, channel_num;
	kal_uint8 layer, version, slot_bytes;
	kal_uint8 *ptr, *end;
	kal_uint8 result = DAF_FIND_FRAME_FALSE;

	for( round = 0; result != DAF_FIND_FRAME_TRUE; round++ )
	{
		// check if it is out of searching range
		if( input_stream->total_load > input_stream->skip + DAF_FRAME_CHECK_SIZE )
			return KAL_FALSE;
		
		// refill data
		audio_input_stream_read( input_stream, 0 );
		read_size = input_stream->tail - input_stream->ptr;
		if( round == 0 )
		{
			if( read_size < 4 + move_offset ) // we need at least 4 + move_offset bytes to check frame header
				return KAL_FALSE; // eof
			input_stream->ptr += move_offset;
		}
		else if( read_size < 4 ) // we need at least 4 bytes to check frame header
			return KAL_FALSE; // eof
		
		if( result == DAF_FIND_FRAME_FREE_BITRATE )
		{
			// search next frame for calculating free bitrate
			bitrate = 0;
			version = header->version;
			layer = header->layer;
			sample_rate = header->sample_rate;
			channel_num = header->channel_num;
			for( ptr = input_stream->ptr + 4, end = input_stream->tail - 4; ptr <= end; ptr++ )
			{
				// check if ptr reference to a valid frame header
				if( daf_parse_frame_header( ptr, &free_bitrate_header ) )
				{
					if( version == free_bitrate_header.version &&
						layer == free_bitrate_header.layer &&
						sample_rate == free_bitrate_header.sample_rate &&
						bitrate == free_bitrate_header.bitrate &&
						channel_num == free_bitrate_header.channel_num )
					{
						slot_bytes = 1;
						if( layer == DAF_LAYER_I )
						{
							frame_slots = 12;
							slot_bytes = 4;
						}
						else if( (layer == DAF_LAYER_III ) && (version == DAF_VERSION_2 || version == DAF_VERSION_2_5) )
							frame_slots = 72;
						else
							frame_slots = 144;
						header->min_frame_size = header->max_frame_size = ptr - input_stream->ptr;
						bitrate = (header->min_frame_size / slot_bytes) * sample_rate / frame_slots;
						if( bitrate >= 8000 && !(layer == DAF_LAYER_III && bitrate > 640000) )
						{
							header->bitrate = input_stream->free_bitrate = bitrate;
							input_stream->ptr = ptr;
							return KAL_TRUE;
						}
					}
					
					// if the latest found frame header doesn't match with the previous frame header,
					// we discard the previous one, and use the latest found frame header
					memcpy( header, &free_bitrate_header, sizeof(daf_header_struct) );
					bitrate = header->bitrate;
					layer = header->layer;
					if( header->bitrate != 0 )
						result = DAF_FIND_FRAME_TRUE;
					break;
				}
			}
			input_stream->ptr = ptr;
		}
		else
		{
			// loop through all data in input_stream
			for( ptr = input_stream->ptr, end = input_stream->tail - 4; ptr <= end; ptr++ )
			{
				// check if ptr reference to a valid frame header
				if( daf_parse_frame_header( ptr, header ) )
				{
					result = DAF_FIND_FRAME_TRUE;
					
					// check if it's free bitrate
					if( header->bitrate == 0 )
					{
						if( input_stream->free_bitrate == 0 )
							result = DAF_FIND_FRAME_FREE_BITRATE;
						else
						{
							header->bitrate = input_stream->free_bitrate;
							// calculate frame length
							version = header->version;
							layer = header->layer;
							slot_bytes = 1;
							if( layer == DAF_LAYER_I )
							{
								frame_slots = 12;
								slot_bytes = 4;
							}
							else if( (layer == DAF_LAYER_III ) && (version == DAF_VERSION_2 || version == DAF_VERSION_2_5) )
								frame_slots = 72;
							else
								frame_slots = 144;
							
							header->min_frame_size = (frame_slots * header->bitrate / header->sample_rate) * slot_bytes;
							header->max_frame_size = header->min_frame_size + slot_bytes;
						}
					}
					break;
				}
			}
			input_stream->ptr = ptr;
		}
	}
	return KAL_TRUE;
}

/*****************************************************************************
* FUNCTION
*     daf_check_next_frame
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     audio_input_stream_struct *input_stream
*     daf_header_struct* prev_header
*     kal_int32 min_next_pos
*     kal_int32 max_next_pos
*     kal_int32 *next_pos
* RETURNS
*     kal_uint8 : DAF_CHECK_NEXT_FRAME_RESULT_ENUM
* GLOBALS AFFECTED
*     
*****************************************************************************/
kal_uint8 daf_check_next_frame( audio_input_stream_struct *input_stream, daf_header_struct* prev_header, kal_int32 min_next_pos, kal_int32 max_next_pos, kal_int32 *next_pos )
{
	daf_header_struct header;
	kal_int32 search_pos;
	kal_uint8 search_count;
	
	if( (kal_uint32)max_next_pos + 4 > input_stream->total_load ) // check if enough space for decoding next frame header
	{
		// refill buffer
		audio_input_stream_read( input_stream, 0 );
		if( (kal_uint32)max_next_pos + 4 > input_stream->total_load ) // check if still have enough space
			return DAF_CHECK_FRAME_EOF; // eof
	}
	// check if it's a valid frame header
	for( search_count = 0; search_count < 2; search_count++ )
	{
		if( search_count == 0 )
			search_pos = max_next_pos;
		else
			search_pos = min_next_pos;
		if( daf_parse_frame_header( input_stream->tail + search_pos - input_stream->total_load, &header ) )
		{
			if( prev_header->version == header.version && prev_header->layer == header.layer &&
				prev_header->channel_num == header.channel_num && prev_header->sample_rate == header.sample_rate )
			{
				*next_pos = search_pos;
				return DAF_CHECK_FRAME_TRUE;
			}
		}
	}
	return DAF_CHECK_FRAME_FALSE;
}

/*****************************************************************************
* FUNCTION
*     daf_parse
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     FILE* file
*     audio_input_stream_struct *input_stream
*     kal_uint32* offset
*     daf_data_info_struct* info_p
* RETURNS
*     kal_bool
* GLOBALS AFFECTED
*     
*****************************************************************************/
kal_bool daf_parse( audio_input_stream_struct *input_stream, kal_uint32* offset, daf_data_info_struct* info_p )
{
	kal_uint8 result, sync_count;
	kal_uint8 slot_bytes, frame_slots;
	kal_int32 cur_pos, min_next_pos, max_next_pos, next_pos;
	kal_uint32 data_size, movement;
	daf_header_struct header;

	input_stream->skip = *offset;
	data_size = (kal_uint32)audio_input_stream_get_data_size( input_stream );
	
	// find the first frame with 2 consecutive frames
	for( movement = 0; ; input_stream->free_bitrate = 0, movement = 1 )
	{
		// find the first frame
		if( !daf_find_next_frame( input_stream, &header, movement ) )
			return KAL_FALSE; // not found
		
		// calculate current frame position
		cur_pos = input_stream->total_load - (input_stream->tail - input_stream->ptr);
		
		// calculate next frame position because we need to find 2 consecutive frames
		min_next_pos = cur_pos + header.min_frame_size;
		max_next_pos = cur_pos + header.max_frame_size;
		
		// check if next frame position reference to a valid frame
		result = daf_check_next_frame( input_stream, &header, min_next_pos, max_next_pos, &next_pos );
		if( result == DAF_CHECK_FRAME_EOF )
			break;
		else if( result == DAF_CHECK_FRAME_TRUE )
		{
			// calculate the offset of the first found frame position
			*offset = (kal_uint32) cur_pos;

			// calc frame_size
			header.frame_size = next_pos - cur_pos;

			// check 2 consecutive frames
			for( sync_count = 0; sync_count < 2; sync_count++ )
			{
				// move to next frame
				input_stream->ptr += header.frame_size;
				
				ASSERT( input_stream->ptr < input_stream->tail );
				
				// decode frame header
				daf_parse_frame_header( input_stream->ptr, &header );
				
				// calculate frame length for free bitrate
				if( header.bitrate == 0 )
				{
					// if it's a free bitrate frame but the first frame is not, it's false
					if( input_stream->free_bitrate == 0 )
						break;
					
					header.bitrate = input_stream->free_bitrate;
					slot_bytes = 1;
					if( header.layer == DAF_LAYER_I )
					{
						frame_slots = 12;
						slot_bytes = 4;
					}
					else if( (header.layer == DAF_LAYER_III ) && (header.version == DAF_VERSION_2 || header.version == DAF_VERSION_2_5) )
						frame_slots = 72;
					else
						frame_slots = 144;
							
					header.min_frame_size = (frame_slots * header.bitrate / header.sample_rate) * slot_bytes;
					header.max_frame_size = header.min_frame_size + slot_bytes;
				}
				
				// calculate current frame position
				cur_pos = input_stream->total_load - (input_stream->tail - input_stream->ptr);
		
				// calculate next frame position because we need to find 2 consecutive frames
				min_next_pos = cur_pos + header.min_frame_size;
				max_next_pos = cur_pos + header.max_frame_size;
		
				// check if next frame position reference to a valid frame
				result = daf_check_next_frame( input_stream, &header, min_next_pos, max_next_pos, &next_pos );
				
				if( result != DAF_CHECK_FRAME_TRUE )
					break;

				// calc frame_size
				header.frame_size = next_pos - cur_pos;
			}
			
			if( result == DAF_CHECK_FRAME_EOF || sync_count == 2 )
				break;
			else
				audio_input_stream_seek( input_stream, (kal_int32) *offset );
		}
	}

	if( info_p != NULL )
	{
		info_p->bitrate = header.bitrate;
		info_p->duration = (data_size - *offset) * 8 / header.bitrate;
		info_p->samplerate = header.sample_rate;
		info_p->channel_num = header.channel_num;
	}
	
	return KAL_TRUE;
}

/*****************************************************************************
* FUNCTION
*     daf_set_data_info
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     aud_info_struct *info_p
*     kal_uint32 channel_num
*     kal_uint32 duration
*     kal_uint32 size
*     kal_uint32 bitrate
*     kal_uint32 samplerate
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void daf_set_data_info( aud_info_struct *info_p, kal_uint32 channel_num, kal_uint32 duration, kal_uint32 size, kal_uint32 bitrate, kal_uint32 samplerate )
{
   kal_wchar temp[30];
   kal_uint32 bitrate_fraction, samplerate_fraction;
   mmi_trace(1,"chenhe,daf_set_data_info,duration is %d,size is %d",duration,size);
   // channel num
   info_p->channel_num = channel_num;
   // quality
   if( bitrate > 0 || samplerate > 0 )
   {
      if( bitrate > 0 )
      {
         bitrate_fraction = bitrate / 100;
         bitrate_fraction = bitrate_fraction - (bitrate_fraction/10)*10;
         csd_wsprintf( (kal_wchar*)info_p->quality, "%d.%dkbps", bitrate/1000, bitrate_fraction );
         if( samplerate > 0 )
            kal_wstrcat( (kal_wchar*)info_p->quality, (kal_wchar*)L" / " );
      }
      if( samplerate > 0 )
      {
         samplerate_fraction = samplerate / 100;
         samplerate_fraction = samplerate_fraction - (samplerate_fraction/10)*10;
         csd_wsprintf( (kal_wchar*)temp, "%d.%dkHz", samplerate/1000, samplerate_fraction );
         kal_wstrcat( (kal_wchar*)info_p->quality, temp );
      }
   }
   // size
   if( size >= 1024 )
      csd_wsprintf( (kal_wchar*) info_p->size, "%d.%dKB", size/1024, (size%1024)/103 );
   else
      csd_wsprintf( (kal_wchar*) info_p->size, "%dB", size );
   // time
   if( duration > 0 )
      csd_wsprintf( (kal_wchar*) info_p->duration, "%02d:%02d", (int)(duration/60), (int)(duration%60) );
}

/*****************************************************************************
* FUNCTION
*     daf_parse_data_info
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     aud_info_struct *info_p
*     audio_input_stream_struct* input_stream
*     id3_tag_struct* id3_tag
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void daf_parse_data_info( aud_info_struct *info_p, audio_input_stream_struct* input_stream, id3_tag_struct* lid3_tag )
{
	kal_int32 data_size;
	kal_uint32 offset;
	kal_bool v2_result = KAL_FALSE;
	kal_bool v1_result = KAL_FALSE;
	kal_bool daf_parse_result;
	daf_data_info_struct daf_data_info;
	
	// init id3 tag data structure
	id3_tag_init( lid3_tag );
	
	audio_input_stream_seek( input_stream, 0 );
	/*
	 * check id3v2 magic word -- "ID3"
	 * and parse & save data to frames
	 */
	v2_result = id3_tag_parse_v2_tag( input_stream, lid3_tag );
	mmi_trace(1,"chenhe,daf_parse_data_info,v2_result is %d",v2_result);
	/*
	 * parse daf frame to get duration, bitrate, samplerate, channel_num
	 */
	if( v2_result )
	{
		// skip id3v2 tag
		offset = lid3_tag->v2_size + 10;
		if( lid3_tag->v2_footer )
			offset += 10;
	}
	else
		offset = 0;
	
	audio_input_stream_seek( input_stream, offset );
	daf_parse_result = daf_parse( input_stream, &offset, &daf_data_info );
	
	/* 
	 * parse id3v1 tag
	 */
	if( (data_size = audio_input_stream_get_data_size( input_stream )) >= 128 )
	{
		// move to last 128 bytes before file end for checking id3v1 tag
		audio_input_stream_seek( input_stream, data_size - 128 );
		
		/*
		 * check id3v1 magic word -- "TAG"
		 * and parse & save data to frames
		 */
		v1_result = id3_tag_parse_v1_tag( input_stream, lid3_tag );
	}
	else
		v1_result = KAL_FALSE;
	mmi_trace(1,"chenhe,daf_parse_data_info,v1_result is %d",v1_result);

	// copy title to info_p
	if( lid3_tag->frames[ID3_FRAME_TITLE].text != NULL )
		kal_wstrncpy( (kal_wchar*)info_p->title, (const kal_wchar*)lid3_tag->frames[ID3_FRAME_TITLE].text, MAX_ID3_TITLE_LEN );
	
	// copy artist to info_p
	if( lid3_tag->frames[ID3_FRAME_ARTIST].text != NULL )
		kal_wstrncpy( (kal_wchar*)info_p->artist, (const kal_wchar*)lid3_tag->frames[ID3_FRAME_ARTIST].text, MAX_ID3_ARTIST_LEN );
	
	// copy album to info_p
	if( lid3_tag->frames[ID3_FRAME_ALBUM].text != NULL )
		kal_wstrncpy( (kal_wchar*)info_p->album, (const kal_wchar*)lid3_tag->frames[ID3_FRAME_ALBUM].text, MAX_ID3_ALBUM_LEN );
	
	// copy genre to info_p
	if( lid3_tag->frames[ID3_FRAME_GENRE].text != NULL )
		kal_wstrncpy( (kal_wchar*)info_p->genre, (const kal_wchar*)lid3_tag->frames[ID3_FRAME_GENRE].text, MAX_ID3_GENRE_LEN );
	
	// copy year to info_p
	if( lid3_tag->frames[ID3_FRAME_YEAR].text != NULL )
		kal_wstrncpy( (kal_wchar*)info_p->year, (const kal_wchar*)lid3_tag->frames[ID3_FRAME_YEAR].text, MAX_ID3_YEAR_LEN );
	mmi_trace(1,"chenhe,daf_parse_data_info,daf_parse_result is %d",daf_parse_result);
	if( daf_parse_result )
		daf_set_data_info( info_p, daf_data_info.channel_num, daf_data_info.duration, data_size, daf_data_info.bitrate, daf_data_info.samplerate );
}

/*****************************************************************************
* FUNCTION
*     daf_seek
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     aud_info_struct *info_p
*     kal_uint32* offset
* RETURNS
*     kal_bool
* GLOBALS AFFECTED
*     
*****************************************************************************/
kal_bool daf_seek( audio_input_stream_struct *input_stream, kal_uint32* offset )
{
	kal_int32 v2_tag_size;
	
	if( *offset == 0 )
	{
		// skip id3v2 tag
		if( audio_input_stream_read( input_stream, 10 ) != 10 )
			return KAL_FALSE;
		if( ( v2_tag_size = id3_tag_parse_v2_tag_size( input_stream->ptr )) > 0 )
			*offset = v2_tag_size;
	}
	audio_input_stream_seek( input_stream, (kal_int32) *offset );
	if( !daf_parse( input_stream, offset, NULL ) )
		return KAL_FALSE;
	audio_input_stream_seek( input_stream, (kal_int32) *offset );
	return KAL_TRUE;
}

/*****************************************************************************
* FUNCTION
*     get_aud_info
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     kal_wchar* filename
*     aud_info_struct* aud_info_p
* RETURNS
*     kal_bool
* GLOBALS AFFECTED
*     
*****************************************************************************/
kal_bool get_aud_info( kal_wchar* filename, aud_info_struct* aud_info_p )
{
	kal_int16 format;
	kal_uint32 size;
	FS_HANDLE file;
#ifdef __AUD_NEW_INTERFACE__
	STFSAL fsal_handle;
	audInfoStruct* aud_info;
#endif // __AUD_NEW_INTERFACE__ 
	audio_input_stream_struct* input_stream;
	mmi_trace(1,"chenhe,get_aud_info");
	if( (file = DRM_open_file( (const UINT8 *)filename, FS_READ_ONLY|FS_OPEN_NO_DIR, 0 )) < 0 )
		return KAL_FALSE;

	format = med_get_media_type( filename );
	
	input_stream = audio_file_input_stream_alloc( file );
	size = audio_input_stream_get_data_size( input_stream );

	memset(aud_info_p, 0, sizeof(aud_info_struct) );
	daf_set_data_info( aud_info_p, 0, 0, size, 0, 0 );
		mmi_trace(1,"chenhe,get_aud_info,format is %d,size is %p",format,size);

	if( format == MED_TYPE_DAF )
		daf_parse_data_info( aud_info_p, input_stream, &id3_tag );
	else
	{
#ifdef AAC_DECODE
		if( format == MED_TYPE_AAC )
		{
			if( id3_tag_parse( input_stream, &id3_tag ) )
			{
				// copy title to info_p
				if( id3_tag.frames[ID3_FRAME_TITLE].text != NULL )
					kal_wstrncpy( (kal_wchar*)aud_info_p->title, (const kal_wchar*)id3_tag.frames[ID3_FRAME_TITLE].text, MAX_ID3_TITLE_LEN );
	
				// copy artist to info_p
				if( id3_tag.frames[ID3_FRAME_ARTIST].text != NULL )
					kal_wstrncpy( (kal_wchar*)aud_info_p->artist, (const kal_wchar*)id3_tag.frames[ID3_FRAME_ARTIST].text, MAX_ID3_ARTIST_LEN );
	
				// copy album to info_p
				if( id3_tag.frames[ID3_FRAME_ALBUM].text != NULL )
					kal_wstrncpy( (kal_wchar*)aud_info_p->album, (const kal_wchar*)id3_tag.frames[ID3_FRAME_ALBUM].text, MAX_ID3_ALBUM_LEN );
			
				// copy genre to info_p
				if( id3_tag.frames[ID3_FRAME_GENRE].text != NULL )
					kal_wstrncpy( (kal_wchar*)aud_info_p->genre, (const kal_wchar*)id3_tag.frames[ID3_FRAME_GENRE].text, MAX_ID3_GENRE_LEN );
			
				// copy year to info_p
				if( id3_tag.frames[ID3_FRAME_YEAR].text != NULL )
					kal_wstrncpy( (kal_wchar*)aud_info_p->year, (const kal_wchar*)id3_tag.frames[ID3_FRAME_YEAR].text, MAX_ID3_YEAR_LEN );
			}
		}
#endif // AAC_DECODE
	}
	DRM_close_file( file );

#ifdef __AUD_NEW_INTERFACE__
	if( format == MED_TYPE_AAC || format == MED_TYPE_WMA )
	{
		aud_info = (audInfoStruct*) &id3_tag; // reuse id3_tag memory for aud_info
		if( FSAL_Open( &fsal_handle, filename, FSAL_READ ) == FSAL_OK )
		{
			media_get_info_func_ptr getInfoFunc;
			switch( format )
			{
#ifdef AAC_DECODE
			case MED_TYPE_AAC:
				getInfoFunc = AAC_GetContentDescInfo;
				break;
#endif // AAC_DECODE
#ifdef WMA_DECODE
			case MED_TYPE_WMA:
				getInfoFunc = WMA_GetContentDescInfo;
				break;
#endif // WMA_DECODE
			default:
				ASSERT(0);
				break;
			}
			if( getInfoFunc( &fsal_handle, aud_info ) == MEDIA_SUCCESS )
			{
#ifdef WMA_DECODE
				if( format == MED_TYPE_WMA )
				{
					kal_wstrncpy( (kal_wchar*)aud_info_p->title, (const kal_wchar*)aud_info->title, MAX_ID3_TITLE_LEN );
					kal_wstrncpy( (kal_wchar*)aud_info_p->artist, (const kal_wchar*)aud_info->artist, MAX_ID3_ARTIST_LEN );
					kal_wstrncpy( (kal_wchar*)aud_info_p->album, (const kal_wchar*)aud_info->album, MAX_ID3_ALBUM_LEN );
				}
#endif // WMA_DECODE
				channel_num = (aud_info->stereo ? 2: 1);
				daf_set_data_info( aud_info_p, channel_num, aud_info->time, size, aud_info->bitRate, aud_info->sampleRate );
			}
			FSAL_Close( &fsal_handle );
		}
	}
#endif // __AUD_NEW_INTERFACE__

	audio_input_stream_free( input_stream );
	
	return KAL_TRUE;
}

#endif // __RICH_AUDIO_PROFILE__

#endif // MED_NOT_PRESENT

