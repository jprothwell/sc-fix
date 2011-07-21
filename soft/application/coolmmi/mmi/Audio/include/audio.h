/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                                           */
/* Filename:    Audio.h                                                     */
/*                                                                                                   */
/* Description:                                                                                 */
/*                                */
/*     @Pan Xu and Jakcie  2006-12-22.                                                               */
/******************************************************************************/

#ifndef _AUDIO_H
#define _AUDIO_H

#define STOP_ALL_TONE 1000

#define CARD_SRC_ID 0xFF
typedef struct
{
   kal_uint8   audio_id;
   kal_uint8   play_style;
   kal_uint8   volume;
   kal_uint8   output_path;
   //kal_uint16  identifier;
} aud_play_id_struct;

typedef struct
{
   kal_uint8   src_id;
   kal_wchar  *file_name_p;
   kal_uint8   play_style;
   kal_uint8   volume;
   kal_uint8   output_path;
   kal_uint16  identifier;
// set both start_offset and end_offset to 0 for backward compatible
   kal_uint32   start_offset; /* 0 : file begining */
   kal_uint32   end_offset;  /* 0 or file data len-1: file end */
}aud_play_file_struct;


typedef struct
{
   kal_uint8   src_id;
   kal_uint8   *data_p;
   kal_uint32  len;
   kal_uint8   format;
   kal_uint8   play_style;
   kal_uint8   volume;
   kal_uint8   output_path;
   kal_uint16  identifier;
   kal_uint8   blocking;
}
aud_play_string_struct;

#endif
