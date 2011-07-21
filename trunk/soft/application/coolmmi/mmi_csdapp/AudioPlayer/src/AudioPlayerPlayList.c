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
*	AudioPlayerPlayList.c
*
* Project:
* --------
 
*
* Description:
* ------------
*   Audio player play list
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 * 
 *
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/

#include "mmi_features.h"
#include "filesystemdef.h"
#include "mmi_trace.h"
#ifdef __MMI_AUDIO_PLAYER__

#define ClrBitSet( index, bitset )		((bitset)[(index)>>3] &= ~(1<<((index)&0x07)))
#define SetBitSet( index, bitset )		((bitset)[(index)>>3] |= (1<<((index)&0x07)))
#define IsBitSetOn( index, bitset )		((bitset)[(index)>>3] & (1<<((index)&0x07)))

#define __NEWSIMULATOR
#include "stdlib.h"
//#include "time.h"
#include "unicodexdcl.h"
#include "fat_fs.h"

// dependent headers for AudioPlayerXXX.h"
#include "gui_data_types.h"
#include "frameworkstruct.h"
#include "globalconstants.h"
#include "mdi_datatype.h"
#include "kal_release.h"
#include "conversions.h"
#include "filemgr.h"

#include "audioplayerdef.h"
#include "audioplayertype.h"
#include "audioplayerprot.h"
//#include "audioplayerplaylist.h"
#undef __NEWSIMULATOR

#include "mmi_trace.h"
//
const UI_character_type * const audio_file_restorable_upper_exts[] = {
#ifdef DAF_DECODE
	(UI_character_type*)	L"MP3",
#endif
	(UI_character_type*)	L"MID",
	(UI_character_type*)	L"IMY",
	(UI_character_type*)	L"WAV",
	(UI_character_type*)	L"AU ",
	(UI_character_type*)	L"SND",
	(UI_character_type*)	L"AIF",
	(UI_character_type*)	L"AIFF",
	(UI_character_type*)	L"AIFC",
	(UI_character_type*)	L"AMR",
#ifdef AMRWB_DECODE
	(UI_character_type*)	L"AWB",
#endif
#if defined( MP4_CODEC ) && defined( AAC_DECODE )
	(UI_character_type*)	L"M4A",
#endif
#ifdef MMF_DECODE
	(UI_character_type*)	L"MMF",
#endif
#ifdef WMA_DECODE
	(UI_character_type*)	L"WMA",
#endif
#ifdef AAC_DECODE
	(UI_character_type*)	L"AAC",
#endif
	(UI_character_type*)	L"\0"	// end of list
};
const UI_character_type * const audio_file_restorable_lower_exts[] = {
#ifdef DAF_DECODE
	(UI_character_type*)	L"mp3",
#endif
	(UI_character_type*)	L"mid",
	(UI_character_type*)	L"imy",
	(UI_character_type*)	L"wav",
	(UI_character_type*)	L"au ",
	(UI_character_type*)	L"snd",
	(UI_character_type*)	L"aif",
	(UI_character_type*)	L"aiff",
	(UI_character_type*)	L"aifc",
	(UI_character_type*)	L"amr",
#ifdef AMRWB_DECODE
	(UI_character_type*)	L"awb",
#endif
#if defined( MP4_CODEC ) && defined( AAC_DECODE )
	(UI_character_type*)	L"m4a",
#endif
#ifdef MMF_DECODE
	(UI_character_type*)	L"mmf",
#endif
#ifdef WMA_DECODE
	(UI_character_type*)	L"wma",
#endif
#ifdef AAC_DECODE
	(UI_character_type*)	L"aac",
#endif
	(UI_character_type*)	L"\0"	// end of list
};

// drives and paths
extern S16 phoneDrvLetter;
extern S16 cardDrvLetter;
extern UI_character_type root_dir_phone[4];
extern UI_character_type root_dir_card[4];
extern UI_character_type phone_list_name[FMGR_MAX_FILE_LEN+4];
UI_character_type present_list_name[FMGR_MAX_FILE_LEN + 4];
extern UI_character_type card_list_name[FMGR_MAX_FILE_LEN+4];
extern const UI_character_type audio_file_lower_exts[NO_AUDIO_FILE_TYPES][4];
extern const UI_character_type audio_file_upper_exts[NO_AUDIO_FILE_TYPES][4];
extern audply_struct g_audply;
extern UI_character_type fullfilename_temp[FMGR_MAX_PATH_LEN+1];

/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
extern  S16 seccardDrvLetter;
extern UI_character_type root_dir_sec_card[4];
extern UI_character_type sec_card_list_name[FMGR_MAX_FILE_LEN+40];
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
// steal using subMenuData as a buffer to move data in play list file
extern U8 subMenuData[MAX_SUB_MENUS][MAX_SUB_MENU_SIZE];
void mmi_audply_split_filename_ext( UI_string_type filename, UI_string_type filename_without_ext, UI_string_type ext );
void mmi_audply_extract_filename( UI_string_type filename, UI_string_type fullfilename );
BOOL mmi_audply_playlist_remove_all();
U8 mmi_audply_playlist_get_fullfilename( UI_string_type fullfilename, S32 index );
S32 mmi_audply_pick_playlist();

/*****************************************************************************
* FUNCTION
*     mmi_audply_check_restorable
* DESCRIPTION
*     check does the audio type support store/restore function (real resume function)
* IMPACT 
*     
* PARAMETERS
*     UI_string_type ext
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_audply_check_restorable( UI_string_type ext )
{
	U8 i, j;
	for( i = 0; audio_file_restorable_lower_exts[i][0]; i++ )
	{
		BOOL match = TRUE;
		for( j = 0; j < 4; j++ ) // it's is not necessary to check j < 4, but for safety
		{
			UI_character_type file_ext_char = ext[j];
			if( file_ext_char != audio_file_restorable_lower_exts[i][j] && file_ext_char != audio_file_restorable_upper_exts[i][j] )
			{
				match = FALSE;
				break;
			}
			else if( file_ext_char == 0 ) // both are end and length less than 4
				return TRUE;
		}
		if( match )
			return TRUE;
	}
	
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_apply_picked_file
* DESCRIPTION
*     apply pick index to get full filename and title
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_audply_apply_picked_file()
{
	U8 result;
	BOOL isOK;
	UI_character_type filename[FMGR_MAX_PATH_LEN+1];
	UI_character_type wchar_filename[FMGR_MAX_PATH_LEN+1];
	UI_character_type ext[FMGR_MAX_EXT_LEN+1];
	
	if( (result = mmi_audply_playlist_get_fullfilename( g_audply.filefullname, g_audply.pick_index )) == 0 )
	{
		isOK = FALSE;
		g_audply.restorable = FALSE;
	}
	else
	{
		
		if( result == 2 ) // if it is a short filename
		{
			mmi_audply_extract_filename( wchar_filename, g_audply.filefullname );
			mmi_chset_mixed_text_to_ucs2_str( (U8*)filename, (FMGR_MAX_FILE_LEN+1) * ENCODING_LENGTH, (U8*)wchar_filename, g_chset_text_encoding );
		}
		else
			mmi_audply_extract_filename( filename, g_audply.filefullname);
		mmi_audply_split_filename_ext( filename, g_audply.title, ext );
		g_audply.restorable = mmi_audply_check_restorable( ext );
		isOK = TRUE;
	}
	
	//return isOK;
	return TRUE;//chenhe test
}

/*****************************************************************************
* FUNCTION
*     UCS2StrLastChr
* DESCRIPTION
*     find last matched character in string
* IMPACT 
*     
* PARAMETERS
*     UI_string_type ustring1
*     UI_character_type uchar2
* RETURNS
*     UI_string_type
* GLOBALS AFFECTED
*     
*****************************************************************************/
UI_string_type UCS2StrLastChr( UI_string_type ustring1, UI_character_type uchar2 )
{
	UI_string_type result;
	for( result = NULL; *ustring1; ustring1++ )
	{
		if( *ustring1 == uchar2 )
			result = ustring1;
	}
	return result;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_start_playlist_at
* DESCRIPTION
*     seek play list index
* IMPACT 
*     
* PARAMETERS
*     S32 index
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_start_playlist_at( S32 index )
{
	memset( g_audply.bitset, 0, MAX_PLAY_LIST_BYTES );
	g_audply.pick_index = index;
	g_audply.pick_count = 0;
	/* Seed the random-number generator with current time so that
    * the numbers will be different every time we run.
    */
//    srand( (unsigned)time( NULL ) ); this line cause fatal error in target!!!
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_pick_playlist_at
* DESCRIPTION
*     seek play list index
* IMPACT 
*     
* PARAMETERS
*     S32 index
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_pick_playlist_at( S32 index )
{
	g_audply.pick_index = index;
	if( !IsBitSetOn( index, g_audply.bitset ) )
	{
		SetBitSet( index, g_audply.bitset );
		g_audply.pick_count++;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_extract_ext
* DESCRIPTION
*     extract filename extension
* IMPACT 
*     
* PARAMETERS
*     UI_string_type filename
*     UI_string_type ext
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_extract_ext( UI_string_type filename, UI_string_type ext )
{
#if 0
	UI_string_type ptr = filename;
	UI_string_type dot;
	S32 ext_size;
	for( dot = NULL; *ptr; ptr++ )
	{
		if( *ptr == (_T_MMI'.') )
			dot = ptr;
	}
	
	if( dot != NULL )
	{
		pfnUnicodeStrncpy( (S8*)ext, (S8*)(dot + 1), FMGR_MAX_EXT_LEN );
		for( ext_size = pfnUnicodeStrlen( (S8*)ext ); ext_size < 3; ext_size++ )
			pfnUnicodeStrcat( (S8*)ext, (S8*)L" " );
	}
	else
	{
		ext[0] = 0;
	}
	
	//////////////////////////////////////////////
	
	U8 namelength = 0;
	int i;
	S8 extbuf[8] = "\0";
	S8 parabuf[(FMGR_MAX_PATH_LEN+1)*2] = "\0";
	


	UnicodeToAnsii((S8*)parabuf, (S8*)filename);
	namelength = strlen(parabuf);
	mmi_trace(1,"in function namelength is %d",namelength);

	for(i = namelength-1; i >= 0; i--)
	{
		if('.' == parabuf[i])
		  break;	
	}
	

	if(i == namelength-4)
	{
		memcpy(extbuf, parabuf+i+1 , strlen(parabuf+i+1));
		mmi_trace(1,"file name extersion is %s",extbuf);
		AnsiiToUnicodeString(ext, extbuf);
	}
	
	else
	{
		mmi_trace(1,"the dot == null");
		ext[0] = 0;
		ext[1] = 0;
	}
	#endif
	S16 i, length;
	S8 *path=NULL;

   	length = (S16)pfnUnicodeStrlen((const S8 *)filename);
	path = (S8 *)filename;
   	i = length - 1;

   	while (i>=0)
   	{
   		
      	if (path[i<<1] == '.' && path[(i<<1)+1] == 0 &&(i != length - 1))
         		break;
     	 	i--;
   	}

	if (i<0)
	{
		//mmi_trace(1,"the dot == null");
		ext[0] = 0;
		return ;
	}
	else
	{
	pfnUnicodeStrncpy((S8*)ext,&path[(i+1)<<1],pfnUnicodeStrlen((const S8 *)&path[(i+1)<<1]));
	}	
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_split_filename_ext
* DESCRIPTION
*     split filename into two parts: name and extension
* IMPACT 
*     
* PARAMETERS
*     UI_string_type filename
*     UI_string_type filename_without_ext
*     UI_string_type ext
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_split_filename_ext( UI_string_type filename, UI_string_type filename_without_ext, UI_string_type ext )
{
	UI_string_type ptr = filename;
	UI_string_type dot;

	for( dot = NULL; *ptr; ptr++ )
	{
		if( *ptr == (UI_character_type)_T_MMI'.' )
			dot = ptr;
	}
	#if 0
	if( dot != NULL )
	{
		pfnUnicodeStrncpy( (S8*)filename_without_ext, (S8*)filename, dot - filename );
		pfnUnicodeStrncpy( (S8*)ext, (S8*)(dot + 1), FMGR_MAX_EXT_LEN );
		for( ext_size = pfnUnicodeStrlen( (S8*)ext ); ext_size < 3; ext_size++ )
			pfnUnicodeStrcat( (S8*)ext, (S8*)L" " );
	}
	else
	#endif
	{
		pfnUnicodeStrncpy( (S8*)filename_without_ext, (S8*)filename, FMGR_MAX_FILE_LEN-1);
		ext[0] = 0;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_extract_filename
* DESCRIPTION
*     get filename without path
* IMPACT 
*     
* PARAMETERS
*     UI_string_type filename
*     UI_string_type fullfilename
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_extract_filename( UI_string_type filename, UI_string_type fullfilename )
{
	UI_string_type ch;
	UINT16 s = 0x002F;
	ch = UCS2StrLastChr( fullfilename, s );
	if( ch == NULL )
	{
		TRACE_FMGR_FUNC("( ch == NULL )");
		filename[0] = 0;
	}
	else
	{
		TRACE_FMGR_FUNC("( ch != NULL )");
		pfnUnicodeStrcpy( (S8*)filename, (S8*)(ch+1) );
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_load_playlist
* DESCRIPTION
*     load play list to buffer
* IMPACT 
*     
* PARAMETERS
*     UI_string_type list_file
*     S32 index
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_load_playlist( UI_string_type list_file, S32 index )
{
	INT32 i = 0, size, no_items;
	FS_HANDLE fd;
	
	index -= (PLAY_LIST_BUFF_SIZE/2 - 1);
	if( index < 0 )
		index = 0;
	no_items = MAX_PLAY_LIST_NUM - index;
	if( no_items > PLAY_LIST_BUFF_SIZE)
		no_items = PLAY_LIST_BUFF_SIZE;
	g_audply.buff_head = index;
	g_audply.n_buff_items = 0;
	fd = MMI_FS_Open((const UINT8 *) list_file, FS_READ_ONLY );
	if( fd >= 0 )
	{
		if( MMI_FS_Seek( fd, index * PLAY_LIST_RECORD_SIZE, FS_FILE_BEGIN ) >= 0 )
		{
			for( ; i < no_items && MMI_FS_Read( fd, fullfilename_temp, PLAY_LIST_RECORD_SIZE, (INT32*)&size ) == FS_NO_ERROR; i++ )
			{
				if(pfnUnicodeStrlen((const S8 *)fullfilename_temp) != 0)
					{
				if( size < PLAY_LIST_RECORD_SIZE )
					{
					mmi_trace(1,"chenhe,mmi_audply_load_playlist ,size<PLAY_LIST_RECORD_SIZE is %d",size);
					break;
					}
				// load filename to play list buffer
				fullfilename_temp[EOS(size)] = 0; // make it must be in boundary
				#if 0
				if( IS_SNAME( fullfilename_temp ) )
				{
					mmi_audply_extract_filename( filename, fullfilename_temp );
					mmi_chset_mixed_text_to_ucs2_str( (U8*)g_audply.buff_filename[i], (FMGR_MAX_FILE_LEN+1) * ENCODING_LENGTH, (U8*)filename, g_chset_text_encoding );
				}
				else
					#endif
					mmi_audply_extract_filename( g_audply.buff_filename[i], fullfilename_temp );
				g_audply.n_buff_items++;
					}
				else
					{
					mmi_trace(1,"chenhe,mmi_audply_load_playlist ,fullfilename_temp is 0");
					}
			}
		}
		else
		{
			mmi_trace(1,"chenhe,mmi_audply_load_playlist seek failure");
		}
		mmi_trace(1,"chenhe,mmi_audply_load_playlist g_audply.n_buff_items is %d",g_audply.n_buff_items);
		MMI_FS_Close( fd );
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_init_playlist
* DESCRIPTION
*     init play list
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     void
* GLOBALS AFFECTED
*     
*****************************************************************************/
void mmi_audply_init_playlist()
{

	
	
	INT32 i = 0,size = 0,filesize = 0;
	FS_HANDLE fd;
	//UI_character_type filename[FMGR_MAX_FILE_LEN+1];
	
	TBM_ENTRY(0x2A2B);
	
	mmi_audply_clear_play_seconds();
	g_audply.restorable = FALSE;
	g_audply.title[0] = 0;
	g_audply.filefullname[0] = 0;
	g_audply.buff_head = 0;
	g_audply.n_buff_items = 0;
	
	fd = MMI_FS_Open( (const UINT8 *)PRESENT_LIST_FILE, FS_READ_ONLY );
	if(fd >= 0 && (MMI_FS_GetFileSize(fd,(INT32*)&filesize) == FS_NO_ERROR))
	{
	if(  filesize > 0)
	 {
		for( ; i < MAX_PLAY_LIST_NUM && MMI_FS_Read( fd, fullfilename_temp, PLAY_LIST_RECORD_SIZE, (INT32*)&size ) == FS_NO_ERROR; i++ )
		{
			  if (pfnUnicodeStrlen((const S8 *)fullfilename_temp) != 0)
			  {	
				
				
				if( size < PLAY_LIST_RECORD_SIZE )
				{
					TRACE_FMGR_FUNC("Read Failed!!!");
					break;
				}
				if( i < PLAY_LIST_BUFF_SIZE )
				{
					// load filename to play list buffer
					fullfilename_temp[EOS(size)] = 0; // make it must be in boundary
					#if 0
					if( IS_SNAME( fullfilename_temp ) )
					{
						mmi_audply_extract_filename( filename, fullfilename_temp );
						mmi_chset_mixed_text_to_ucs2_str( (U8*)g_audply.buff_filename[i], (FMGR_MAX_FILE_LEN+1) * ENCODING_LENGTH, (U8*)filename, g_chset_text_encoding );
					}
					else
						#endif
						mmi_audply_extract_filename( g_audply.buff_filename[i], fullfilename_temp );
					g_audply.n_buff_items++;
				}
				else
				{
					MMI_FS_GetFileSize(fd, (INT32 *)&size);
					i = size/PLAY_LIST_RECORD_SIZE-1;
					break;
				}
			}
			else
			{
				break;
			}
		}
		MMI_FS_Close( fd );
	}
	else
		{
		MMI_FS_Close( fd );
		}
		}
	else if (fd >= 0)
		{
		MMI_FS_Close( fd );
	}
	
	g_audply.n_total_items = i;
	mmi_audply_start_playlist_at( -1 );

	TBM_EXIT(0x2A2B);
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_playlist_append_one
* DESCRIPTION
*     append one song at tail
* IMPACT 
*     
* PARAMETERS
*     S8* fullfilename
* RETURNS
*     U8 (0: success, 1: disk full, 2: other error)
* GLOBALS AFFECTED
*     
*****************************************************************************/
U8 mmi_audply_playlist_append_one( S8* fullfilename )
{
	
	
	INT32 size;
	FS_HANDLE fd;
	U8 result = 2;
	int write_result;
	UI_character_type ext[FMGR_MAX_EXT_LEN+10] = {0};
	UI_character_type terminal[FMGR_MAX_PATH_LEN+1] = {0};

	TBM_ENTRY(0x2A26);
	
   	mmi_audply_extract_ext((UI_string_type) fullfilename, ext );

	if (mmi_audply_lookup_audio_file_format( ext ) > 0)
       {
	if( (fd = MMI_FS_Open( (const UINT8 *)PRESENT_LIST_FILE, FS_READ_WRITE | FS_CREATE )) >= 0 )
	{
		if( MMI_FS_Seek( fd, g_audply.n_total_items * PLAY_LIST_RECORD_SIZE, FS_FILE_BEGIN ) >= 0 )
		{
			if( (write_result = MMI_FS_Write( fd, fullfilename, PLAY_LIST_RECORD_SIZE, (INT32*)&size )) == FS_NO_ERROR )
			{
				g_audply.n_total_items++;
				result = 0;
			}
			else if( write_result == FS_DISK_FULL )
				{
				result = 1;
				
			}
		}
		//MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (UINT*)&size ) ;
		//MMI_FS_Close( fd );
	
	if(result == 0)
	  {	
	    if( (write_result = MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (INT32*)&size )) != 0 )
		{
			if(write_result == FS_DISK_FULL)
				{
				
				result = 1;
				MMI_FS_Seek(fd, -PLAY_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
				MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (INT32*)&size );
				}
			else{
				MMI_FS_Seek(fd, -PLAY_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
				MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (INT32*)&size );
				result = 2;
				}
		}
		MMI_FS_Close( fd );
	   }	
 	else
	 {
		if(result == 1)
		{
			MMI_FS_Seek(fd, -PLAY_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
		       MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (INT32*)&size );
		}
		else
		{
			MMI_FS_Seek(fd, -PLAY_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
		       MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (INT32*)&size );
		}
		MMI_FS_Close( fd );
	  }
      }
    }
else
       {
	  result = 4;
	}
#if 0

if(result == 0)
	  {	
	    if( (write_result = MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (UINT*)&size )) != 0 )
		{
			if(write_result == FS_DISK_FULL)
				{
				
				result = 1;
				MMI_FS_Seek(fd, -PLAY_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
				MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (UINT*)&size );
				}
			else
				result = 2;
		}
		MMI_FS_Close( fd );
	   }	
 else
	 {
		if(result == 1)
		{
			MMI_FS_Seek(fd, -PLAY_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
		       MMI_FS_Write( fd, PLAY_LIST_RECORD_SIZE, size, (UINT*)&size );
		}
		MMI_FS_Close( fd );
	  }
#endif
	  
	TBM_EXIT(0x2A26);
	
	return result;

	
}

UI_string_type mmi_audply_get_present_list_file()
{

    kal_wsprintf((kal_wchar*)present_list_name, "%c:\\audio_play_list.txt", (S8)g_audply.present_list);
    return present_list_name ;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_playlist_remove_one
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     S32 index
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL  Mp3playlist_Truncate(FS_HANDLE FileHandle,UINT32 size);
BOOL mmi_audply_playlist_remove_one( S32 index )
{
	UI_string_type file_list_buff = (UI_string_type) subMenuData; // use 4698 bytes (it has 5040 bytes)
	S32 max_buff_size = MAX_SUB_MENUS * MAX_SUB_MENU_SIZE / PLAY_LIST_RECORD_SIZE;
	INT32 n_items_in_buff, buff_size, size;
	S32 file_size = g_audply.n_total_items * PLAY_LIST_RECORD_SIZE;
	S32 offset = index * PLAY_LIST_RECORD_SIZE;
	S32 count = index;
	FS_HANDLE fd;
	BOOL isOK = FALSE;
	BOOL need_apply_file = FALSE;
	
	if( g_audply.n_total_items <= 1 )
		return mmi_audply_playlist_remove_all();

	if( (fd = MMI_FS_Open((const UINT8 *) PRESENT_LIST_FILE, FS_READ_WRITE )) >= 0 )
	{
		isOK = TRUE;
		n_items_in_buff = g_audply.n_total_items - count - 1;
		while( n_items_in_buff > 0 )
		{
			if( n_items_in_buff > max_buff_size )
				n_items_in_buff = max_buff_size;
			buff_size = PLAY_LIST_RECORD_SIZE * n_items_in_buff;
			if( MMI_FS_Seek( fd, offset + PLAY_LIST_RECORD_SIZE, FS_FILE_BEGIN ) < 0 )
			{
				isOK = FALSE;
				break;
			}
			if( MMI_FS_Read( fd, file_list_buff, buff_size, (INT32*)&size ) != FS_NO_ERROR )
			{
				isOK = FALSE;
				break;
			}
			if( MMI_FS_Seek( fd, offset, FS_FILE_BEGIN ) < 0 )
			{
				isOK = FALSE;
				break;
			}
			if( MMI_FS_Write( fd, file_list_buff, buff_size, (INT32*)&size ) != FS_NO_ERROR )
			{
				isOK = FALSE;
				break;
			}
			offset += buff_size;
			count += n_items_in_buff;
			n_items_in_buff = g_audply.n_total_items - count - 1;
		}
		mmi_trace(1,"chenhe,mmi_audply_playlist_remove_one,isOK is %d,count is %d, g_audply.n_total_items is %d",isOK,count, g_audply.n_total_items);
		if( isOK )
		{
			// erase the last record
			if( MMI_FS_Seek( fd, file_size - PLAY_LIST_RECORD_SIZE, FS_FILE_BEGIN ) >= 0 )
			{
				if( Mp3playlist_Truncate( fd ,file_size - PLAY_LIST_RECORD_SIZE)  )
				//if( FS_Truncate( fd ) == FS_NO_ERROR )
				{
					/*
					 * eliminate the bit at index and left-shift the rest of bitset after index
					 */
					S32 m = index / 8;
					S32 k = g_audply.n_total_items / 8;
					S32 n;
					U8 CR, LSB;
					// update pick count before we shift it
					if( IsBitSetOn( index, g_audply.bitset ) )
						g_audply.pick_count--;
					for( CR = 0, LSB = 0; k > m; k-- )
					{
						LSB = ((g_audply.bitset[k] & 0x01) != 0) ? 0x80: 0; // store LSB
						g_audply.bitset[k] = (g_audply.bitset[k] >> 1) | CR;
						CR = LSB;
					}
					for( n = index & 0x07; n < 7; n++ )
					{
						if( (g_audply.bitset[m] & (1<<(n+1))) != 0 )
							g_audply.bitset[m] |= (1<<n); // set bit
						else
							g_audply.bitset[m] &= ~(1<<n); // clear bit
					}
					if( CR != 0 )
						g_audply.bitset[m] |= 0x80; // set MSB
					else
						g_audply.bitset[m] &= 0x7f; // clear MSB
					
					g_audply.n_total_items--;
					
					if( g_audply.pick_index == index )
					{
						need_apply_file = TRUE;
						if( g_audply.pick_index == g_audply.n_total_items ) // it's the tail before, but now it's out of list
						{
							mmi_audply_start_playlist_at( -1 ); // repick one
							mmi_audply_pick_playlist();
						}
						else if( !IsBitSetOn( index, g_audply.bitset ) ) // keep pick_index, but need to update bitset & pick_count
						{
							SetBitSet( g_audply.pick_index, g_audply.bitset );
							g_audply.pick_count++;
						}
					}
					else if( g_audply.pick_index > index )
						g_audply.pick_index--;
					
					g_audply.buff_head = 0;
					g_audply.n_buff_items = 0;
				}
				else
					isOK = FALSE;
			}
			else
				isOK = FALSE;
		}
		MMI_FS_Close( fd );
	}
	
	if( need_apply_file )
	{
		mmi_audply_apply_picked_file();
		mmi_audply_clear_play_seconds();
	}
	return isOK;
}
/*****************************************************************************
* FUNCTION
*     Mp3playlist_Truncate
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL  Mp3playlist_Truncate(FS_HANDLE FileHandle,UINT32 size)
{


#if 0
UI_character_type terminal[FMGR_MAX_PATH_LEN+40] = {0};
INT32 size,writeresult;
MMI_FS_Seek(FileHandle, 0, FS_FILE_CURRENT);

if((writeresult =MMI_FS_Write(FileHandle, terminal, PLAY_LIST_RECORD_SIZE, (INT32*)&size)) == FS_ERROR_RESERVED)
	return FALSE;
#else
	FS_ChangeSize(FileHandle,size);
#endif

return TRUE;
}



/*****************************************************************************
* FUNCTION
*     mmi_audply_playlist_remove_all
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_audply_playlist_remove_all()
{
	FS_HANDLE fd;
	BOOL isOK = FALSE;
	   	
   	if( g_audply.n_total_items == 0 )
		return TRUE;

	if( (fd = MMI_FS_Open( (const UINT8 *)PRESENT_LIST_FILE, FS_READ_WRITE )) >= 0 )
	{
		if( MMI_FS_Seek( fd, 0, FS_FILE_BEGIN ) >= 0 )
		{
			//if( FS_Truncate( fd ) == FS_NO_ERROR )
			if(Mp3playlist_Truncate(fd,0))
			{	
				mmi_audply_clear_play_seconds();
				g_audply.restorable = FALSE;
				g_audply.title[0] = 0;
				g_audply.filefullname[0] = 0;
				g_audply.buff_head = 0;
				g_audply.n_buff_items = 0;
				g_audply.n_total_items = 0;
				mmi_audply_start_playlist_at( -1 );
				isOK = TRUE;
			}
		}
		FS_ChangeSize(fd,0);
		MMI_FS_Close( fd );
	}
	
	return isOK;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_playlist_get_filename
* DESCRIPTION
*     retrieve file name from play list buffer or reload buffer
* IMPACT 
*     
* PARAMETERS
*     UI_string_type filename
*     S32 index
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_audply_playlist_get_filename( UI_string_type filename, S32 index )
{
	S32 offset;
	if( index < 0 || index >= g_audply.n_total_items )
		return FALSE;
	offset = index - g_audply.buff_head;
	if( offset >= 0 && offset < g_audply.n_buff_items )
	{
		// hit: get item from buffer
		//pfnUnicodeStrcpy( (S8*)filename, (S8*)g_audply.buff_filename[offset] );
		pfnUnicodeStrncpy((S8*)filename, (S8*)g_audply.buff_filename[offset] , MAX_SUBMENU_CHARACTERS-1);
		return TRUE;
	}
	
	// miss: load items to buffer
	mmi_audply_load_playlist( PRESENT_LIST_FILE, index );
	
	// double check if load successfully
	offset = index - g_audply.buff_head;
	if( offset >= 0 && offset < g_audply.n_buff_items )
	{
		//pfnUnicodeStrcpy( (S8*)filename, (S8*)g_audply.buff_filename[offset] );
		pfnUnicodeStrncpy((S8*)filename, (S8*)g_audply.buff_filename[offset] , MAX_SUBMENU_CHARACTERS-1);
		return TRUE;
	}
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_playlist_get_fullfilename
* DESCRIPTION
*     retrieve file path and name from play list file
* IMPACT 
*     
* PARAMETERS
*     UI_string_type fullfilename
*     S32 index
* RETURNS
*     U8
* GLOBALS AFFECTED
*     
*****************************************************************************/
U8 mmi_audply_playlist_get_fullfilename( UI_string_type fullfilename, S32 index )
{
	U8 result = 0;
	INT32 size;
	FS_HANDLE fd = MMI_FS_Open( (const UINT8 *)PRESENT_LIST_FILE, FS_READ_ONLY );
	
	if( fd >= 0 )
	{
		if( MMI_FS_Seek( fd, index * PLAY_LIST_RECORD_SIZE, FS_FILE_BEGIN ) >= 0 )
		{
			if( MMI_FS_Read( fd, fullfilename, PLAY_LIST_RECORD_SIZE, (INT32*)&size ) == FS_NO_ERROR )
			{
				if( size > 0 )
				{
					result = IS_SNAME( fullfilename ) ? 2: 1;
					 
					fullfilename[EOS(size)] = 0; // make it must be in boundary
				}
			}
		}
		MMI_FS_Close( fd );
	}
	
	if( !result ) fullfilename[0] = 0;
	
	return result;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_pick_playlist
* DESCRIPTION
*     pick a song from play list
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     S32
* GLOBALS AFFECTED
*     
*****************************************************************************/
S32 mmi_audply_pick_playlist()
{
	S32 i;
	if( g_audply.n_total_items == 0 )
		return -1;

	if( g_audply.pick_count == 0 || g_audply.repeat_mode != REPEAT_ONE )
	{
		if( g_audply.random_mode )
		{
			if( g_audply.n_total_items == g_audply.pick_count )
			{
				// we already played one round
				mmi_audply_start_playlist_at( -1 );
				if( g_audply.repeat_mode == REPEAT_OFF )
					return -1;
			}
			// randomly pick next item
			i = (g_audply.pick_index + (rand() % g_audply.n_total_items) + 1) % g_audply.n_total_items;
			while( IsBitSetOn( i, g_audply.bitset ) )
				i = (i + 1) % g_audply.n_total_items;
			g_audply.pick_index = i;
			g_audply.pick_count++;
		}
		else
		{
			if( g_audply.pick_index == g_audply.n_total_items - 1 )
			{
				// we reached the last song
				mmi_audply_start_playlist_at( -1 );
				if( g_audply.repeat_mode == REPEAT_OFF )
					return -1;
			}
			// sequentially pick next item
			g_audply.pick_index++;
			if( !IsBitSetOn( g_audply.pick_index, g_audply.bitset ) )
				g_audply.pick_count++;
		}
		SetBitSet( g_audply.pick_index, g_audply.bitset );
	}
	
	return g_audply.pick_index;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_pick_playlist_next
* DESCRIPTION
*     pick the next song from play list
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     S32
* GLOBALS AFFECTED
*     
*****************************************************************************/
S32 mmi_audply_pick_playlist_next()
{
	S32 i;
	if( g_audply.n_total_items == 0 )
		return -1;

	if( g_audply.random_mode )
	{
		if( g_audply.n_total_items == g_audply.pick_count )
			mmi_audply_start_playlist_at( -1 );

		// randomly pick next item
		i = (g_audply.pick_index + (rand() % g_audply.n_total_items) + 1) % g_audply.n_total_items;
		while( IsBitSetOn( i, g_audply.bitset ) )
			i = (i + 1) % g_audply.n_total_items;
		g_audply.pick_index = i;
		g_audply.pick_count++;
	}
	else
	{
		if( g_audply.pick_index == g_audply.n_total_items - 1 )
			mmi_audply_start_playlist_at( -1 );

		// sequentially pick next item
		g_audply.pick_index++;
		if( !IsBitSetOn( g_audply.pick_index, g_audply.bitset ) )
			g_audply.pick_count++;
	}
	SetBitSet( g_audply.pick_index, g_audply.bitset );
	
	return g_audply.pick_index;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_pick_playlist_prev
* DESCRIPTION
*     pick the previous song from play list
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     S32
* GLOBALS AFFECTED
*     
*****************************************************************************/
S32 mmi_audply_pick_playlist_prev()
{
	S32 index;
	if( g_audply.n_total_items == 0 )
		return -1;

	if( g_audply.pick_index == 0 )
		index = g_audply.n_total_items - 1;
	else
		index = g_audply.pick_index - 1;
	mmi_audply_start_playlist_at( index );
	mmi_audply_pick_playlist_at( index );
	
	return g_audply.pick_index;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_gen_playlist
* DESCRIPTION
*     generate a play list from all files in the folder
* IMPACT 
*     
* PARAMETERS
*     UI_string_type folder
* RETURNS
*     U8 (0: success, 1: disk full, 2: access file fail, 3: list full)
* GLOBALS AFFECTED
*     
*****************************************************************************/
extern BOOL mmi_audply_gen_playlist_progress(U8 index);
U8 mmi_audply_gen_playlist( UI_string_type folder )
{

	
	
	
	INT32 size, i;
	FS_HANDLE fd, fd_cursor;
	FS_DOSDirEntry file_info;
	UI_character_type filename[FMGR_MAX_PATH_LEN+1] = {0};
	UI_character_type filexpr_temp[FMGR_MAX_PATH_LEN+1] = {0};
	UI_character_type ext[FMGR_MAX_EXT_LEN+10];
	
	BOOL is_shortname;
	U8 result = 0;
	int write_result;

	S8 filefliter[8] = "*";
	S8 filefliterbuf[16] ;
	S8 musicfolder[10] = "My Music/";
	S8 umusicfolder[20];
	
	UI_character_type terminal[FMGR_MAX_PATH_LEN+1] = {0};
	

	TBM_ENTRY(0x2A2A);
	
	
	AnsiiToUnicodeString(umusicfolder, musicfolder);
	AnsiiToUnicodeString(filefliterbuf, filefliter);
	
	//pfnUnicodeStrcpy(flashPlayList,PRESENT_LIST_FILE+2);//chenhe
	//if( (fd = MMI_FS_Open( PRESENT_LIST_FILE, FS_CREATE_ALWAYS | FS_ATTR_HIDDEN )) >= 0 )
	if( (fd = MMI_FS_Open( (const UINT8 *)PRESENT_LIST_FILE, FS_CREATE_ALWAYS | FS_ATTR_HIDDEN )) >= 0 ) //chenhe
	{
		FS_HANDLE folder_handle;
		mmi_trace(g_sw_AUDIO,"mmi_audply_gen_playlist,fd is %d",fd);
		pfnUnicodeStrcpy( (S8*) filexpr_temp, (const S8*) folder );
		pfnUnicodeStrcat( (S8*) filexpr_temp, (const S8*) umusicfolder );
		// check does the folder exist, create it if it's not there
		folder_handle = MMI_FS_Open( (const UINT8 *)filexpr_temp, FS_OPEN_DIR | FS_READ_ONLY );
		mmi_trace(g_sw_AUDIO,"mmi_audply_gen_playlist,1111,folder_handle is %d",folder_handle);

		if( folder_handle < 0 )
			result = MMI_FS_CreateDir( (const UINT8 *)filexpr_temp );
		else
			MMI_FS_Close( folder_handle );
		
		mmi_trace(g_sw_AUDIO,"mmi_audply_gen_playlist,folder_handle is %d",folder_handle);
		pfnUnicodeStrcat( (S8*) filexpr_temp, (const S8*) filefliterbuf );
	
		if( (fd_cursor = MMI_FS_FindFirst( (const UINT8 *)filexpr_temp, 0, 0, &file_info, (UINT8*)filename, (FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH )) >= 0 )
		{	
		
			i = 0;
			do
			{
			 mmi_trace(g_sw_AUDIO,"mmi_audply_gen_playlist,(!(file_info.Attributes == 16)) is %d",(!(file_info.Attributes == 16)));
			mmi_audply_gen_playlist_progress(i);
              if(!(file_info.Attributes == 16))		
			   {
					size = 0;
					
					is_shortname = ((file_info.NTReserved & FS_SFN_MATCH) != 0);
					is_shortname  = 0;
					#if 0
					if( is_shortname )
					{
						for( j = 0; j < 3; j++ )
							ext[j] = (UI_character_type) file_info.Extension[j];
						ext[j] = 0;
					}
					else
					#endif
					mmi_audply_extract_ext( filename, ext );
					if( mmi_audply_lookup_audio_file_format( ext ) > 0 )
					{
					
						memset(fullfilename_temp,0x0,sizeof(fullfilename_temp));
						pfnUnicodeStrcpy( (S8*) fullfilename_temp, (S8*) folder );
						pfnUnicodeStrcat( (S8*) fullfilename_temp, (const S8*) umusicfolder );
						
						pfnUnicodeStrcat( (S8*) fullfilename_temp, (S8*) filename );
						

					#if 0
						if( is_shortname )
							MARK_SNAME( fullfilename_temp );
						else
							MARK_LNAME( fullfilename_temp );
					#endif
						
						
						
						if( i >= MAX_PLAY_LIST_NUM  ) // list full
						{
							result = 3;
							break;
						}
						else if( (write_result = MMI_FS_Write( fd, fullfilename_temp, PLAY_LIST_RECORD_SIZE, (INT32*)&size )) != 0 )
						{
							TRACE_EF(g_sw_AUDIO, "result = %d", write_result);
							if( write_result == FS_DISK_FULL )
								result = 1; // disk full
							else
								result = 2; // other file access error
							break;
						}

						TRACE_EF(g_sw_AUDIO, " i = %d,result is %d",  i,result);
						i++;
					}
				
				
			   	}
			      
				
			} while( MMI_FS_FindNext( fd_cursor, &file_info, (UINT8*)filename, (FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH ) == FS_NO_ERROR );
			
			MMI_FS_FindClose( fd_cursor );
		}
		// MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (UINT*)&size ) ;
		//MMI_FS_Close( fd );

	mmi_trace(g_sw_AUDIO,"result is %d",result);
	//result  = 0;//chenhe test
	if(result == 0)
	  {	
	    if( (write_result = MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (INT32*)&size )) != 0 )
		{
			if(write_result == FS_DISK_FULL)
				{
				
				result = 1;
				MMI_FS_Seek(fd, -PLAY_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
				MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (INT32*)&size );
				}
			else
				{
				
				result = 2;
				MMI_FS_Seek(fd, -PLAY_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
				MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (INT32*)&size );
				}
		}
		MMI_FS_Close( fd );
	   }	
	 else
	 {
		if(result == 1)
		{
			MMI_FS_Seek(fd, -PLAY_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
		       MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (INT32*)&size );
		}
		else{
			MMI_FS_Seek(fd, -PLAY_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
		       MMI_FS_Write( fd, terminal, PLAY_LIST_RECORD_SIZE, (INT32*)&size );

		}
		MMI_FS_Close( fd );
	  }
	}
	else
		result = 2;

	TBM_EXIT(0x2A2A);

	return result;

}

/*****************************************************************************
* FUNCTION
*     mmi_audply_is_dir_ready
* DESCRIPTION
*     check whether is the phone fs available
* IMPACT 
*     
* PARAMETERS
*     UI_string_type path
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_audply_is_dir_ready( UI_string_type path )
{
	FS_HANDLE hd;

	if( (hd = MMI_FS_Open( (const UINT8 *)path, FS_OPEN_DIR | FS_READ_ONLY)) >= 0 ) 
	{
		MMI_FS_Close( hd );
		return TRUE;
	}
	else
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_does_file_exist
* DESCRIPTION
*     check whether does a file exist
* IMPACT 
*     
* PARAMETERS
*     UI_string_type path
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_audply_does_file_exist( UI_string_type path )
{
	FS_HANDLE hd;

	if( (hd = MMI_FS_Open( (const UINT8 *)path, FS_READ_ONLY)) >= 0 )
	{
		MMI_FS_Close( hd );
		       mmi_trace(1, "mmi_audply_does_file_exist is called return true");

		return TRUE;
	}
	else
		{
				       mmi_trace(1, "mmi_audply_does_file_exist is called return False");

		return FALSE;
		}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_get_preferred_or_available_list
* DESCRIPTION
*     get preferred or available list
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     U8
* GLOBALS AFFECTED
*     
*****************************************************************************/
U8 mmi_audply_get_preferred_or_available_list()
{
	UI_string_type dir[NO_OF_LIST] = {root_dir_phone, root_dir_card
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
, root_dir_sec_card
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
};
	
	if( (phoneDrvLetter > 0) && (cardDrvLetter > 0) )
	{
		if( mmi_audply_is_dir_ready( dir[g_audply.preferred_list] ) )
			return g_audply.preferred_list;
		return (LIST_FROM_CARD - g_audply.preferred_list);
	}
    /*+ zhouqin modify for dual t-flash card 20110503*/
    #if defined(DUAL_TFLASH_SUPPORT)
    else if( (phoneDrvLetter > 0) && (seccardDrvLetter > 0) )
	{
		if( mmi_audply_is_dir_ready( dir[g_audply.preferred_list] ) )
			return g_audply.preferred_list;
                if( g_audply.preferred_list == LIST_FROM_CARD)
                        return  (LIST_FROM_CARD - g_audply.preferred_list);
                
		return (LIST_FROM_SEC_CARD - g_audply.preferred_list);
	}
    #endif
    /*- zhouqin modify for dual t-flash card 20110503*/
	else
	{	
		if(g_audply.preferred_list == LIST_FROM_CARD)
		{
			g_audply.preferred_list = LIST_FROM_PHONE;
		}
        /*+ zhouqin modify for dual t-flash card 20110503*/
        #if defined(DUAL_TFLASH_SUPPORT)
        else if (g_audply.preferred_list == LIST_FROM_SEC_CARD)
        {
            g_audply.preferred_list = LIST_FROM_PHONE;
        }
        #endif
        /*- zhouqin modify for dual t-flash card 20110503*/  
		return g_audply.preferred_list;
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_regen_and_reload_playlist
* DESCRIPTION
*     stop playing, regen, and reload playlist 
* IMPACT 
*     
* PARAMETERS
*     BOOL need_regen_list
*     BOOL need_reload_list
* RETURNS
*     U8 (0: success, 1: disk full, 2: access file fail, 3: list full)
* GLOBALS AFFECTED
*     
*****************************************************************************/
U8 mmi_audply_regen_and_reload_playlist( BOOL need_regen_list, BOOL need_reload_list )
{
	U8 result = 0;

	mmi_trace(g_sw_AUDIO,"mmi_audply_regen_and_reload_playlist,need_regen_list is %d,need_reload_list is %d",need_regen_list,need_reload_list);
	if( need_regen_list )
	{
		result = mmi_audply_gen_playlist( PRESENT_LIST_ROOT ); // regen list
	}
	
	if( need_reload_list )
	{
		mmi_audply_stop_playing();


		mmi_audply_init_playlist(); // reload list
			
		// pick one
		if( mmi_audply_pick_playlist_next() >= 0 )
			mmi_audply_apply_picked_file();
	}
	mmi_trace(g_sw_AUDIO,"mmi_audply_regen_and_reload_playlist,result is %d",result);
	return result;
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_is_any_file_selected
* DESCRIPTION
*     check does any song is selected
* IMPACT 
*     
* PARAMETERS
*     void
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_audply_is_any_file_selected( void )
{
	if( g_audply.n_total_items > 0 && g_audply.pick_index >= 0 )
		return TRUE;
	else
		return FALSE;
}
#endif // __MMI_AUDIO_PLAYER__

