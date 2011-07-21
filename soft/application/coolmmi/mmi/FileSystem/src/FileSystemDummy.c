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
 *  FileSystemDummy.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Stub functions for WAP integration
 *
 * Author:
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * 
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/* 
 * We do not want to recompile WAP integration after MMI settings are changed.
 * As a result, we keep the code related to MMI settings here.
 *
 */

#ifdef MMI_ON_WIN32

#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

#include "wgui.h"   /* for ui_string_id_type */
#include "fmt_def.h"
#include "fmt_struct.h"
#include "fat_fs.h"
#include "conversions.h"
#include "time.h"
#include "aud_id3_parser.h"
#undef __NEWSIMULATOR 

#ifdef __MMI_FILE_MANAGER__
extern void fmgr_copy_rsp_hdlr(void *info);
extern void fmgr_delete_rsp_hdlr(void *info);
#endif

//void FS_Read(){};
//void FS_Seek(){};
void kal_assert_fail(char* expr, char* file, kal_uint32 line, kal_bool isext, kal_uint32 ex1, kal_uint32 ex2, kal_uint32 ex3){};
//void FS_GetFileSize(){};
//void FS_Open(){};
//void FS_Close(){};
//void mmi_audply_stop_playing(){};
//void mmi_audply_show_playing_song_in_sub_LCD(){};
//int g_audply(){return 0;};
void mmiapi_enter_idle_screen_notify(){};
//void mmi_audply_entry_main(){};

#if 0
void mmi_fmgr_init(){};
void fmgr_launch(){};
void highlight_fmgr_rename(){};
void highlight_fmgr_delete_all_file(){};
void highlight_fmgr_delete(){};
void mmi_fmgr_select_path_and_enter(){};
void mmi_fmgr_kernel_get_file_count(){};
void mmi_fmgr_kernel_get_cur_idx(){};
void mmi_fmgr_hide_ext_name(){};
void mmi_fmgr_extract_ext_file_name() {};
void mmi_fmgr_kernel_get_file_info() {};
void fmgr_reset_app_select(){};
UI_string_ID_type GetFileSystemErrorString(S32 error) { return 0; };
#endif

//void mmi_audply_save_volume(){};
//void InitAudioPlayerApp(){};
//void evshed_resume_all_events(event_scheduler *es){};
void event_scheduler1_ptr(){};
//void lcd_fb_update_to_memory(){};
//void FS_GetDrive(){};
void mmi_phb_image_check_drive(){};
//void FS_Delete(){};
//void FS_CreateDir(){};
void writeRoamingStatusForMMS(){};
void setMmscVersion(){};
//void FS_Write(){};
//void mmi_vobj_entry_recv_object(){};
//void mmi_vobj_get_file_buffer_size(){};
//void mmi_vobj_init(){};
//void custom_get_fixed_ems_imelody_range(){};
//void mmi_vcard_forward_option_select_card(){};
//void mmi_vcard_forward_option_my_card(){};
//int mmi_audply_is_playing(){return 0;};
//void InitStopwatchApp(){};
//void InitStopwatchNVRAM(){};
//void mmi_audply_is_play_activated(){};
//void FS_SetAttributes() {};
//void mmi_audply_press_dec_volume() {};
//void mmi_audply_press_inc_volume() {};
//void mmi_audply_single_play(){};
//void mmi_audply_exit_subLCD() {};
//void mmi_audply_redraw_subLCD(BOOL is_show) {};
//void FS_FindClose() {};
//void FS_FindNext() {};
//void FS_FindFirst() {};

#ifdef MMI_ON_HARDWARE_P /* the same define in "Conversions.c", zhuoxz,2009-5-4 */
kal_int32 mmi_chset_convert(mmi_chset_enum src_type, mmi_chset_enum dest_type, char* src_buff, char* dest_buff, kal_int32 dest_size)
{
	memcpy(dest_buff, src_buff, dest_size );
	return pfnUnicodeStrlen( dest_buff ) * 2;
}

void mmi_chset_mixed_text_to_ucs2_str( kal_uint8 *dest, kal_int16 dest_size, kal_uint8 *src, mmi_chset_enum encoding )
{
	memcpy( dest, src, dest_size );
}

kal_int32 mmi_chset_text_to_ucs2( kal_uint8 *dest, kal_int32 dest_size, kal_uint8 *src )
{
	memcpy( dest, src, dest_size );
	return pfnUnicodeStrlen( (const S8*) dest );
}

const kal_uint16 g_chset_unknown_encode_char_no_space[] = 
{
    0xFFFF,
    0xFFFF
};

const kal_uint16 g_chset_unknown_encode_char_space[] = 
{
    0x20,
    0x20
};

const kal_uint16 *g_chset_unknown_encode_char=g_chset_unknown_encode_char_no_space;

void mmi_chset_use_space_for_unknown_character(void)
{
    g_chset_unknown_encode_char=g_chset_unknown_encode_char_space;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_chset_dont_use_space_for_unknown_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void mmi_chset_do_not_use_space_for_unknown_character(void)
{
    g_chset_unknown_encode_char=g_chset_unknown_encode_char_no_space;
}
#endif /* MMI_ON_HARDWARE_P */

#if 0
void kal_get_time(kal_uint32* ticks_ptr)
{
	time_t cur_time;
	time( &cur_time );
	*ticks_ptr = (kal_uint32)((double)cur_time/0.004615);
}
#endif

#if 0 //def __MMI_FILE_MANAGER__
extern fmgr_filter_struct fmgr_filter[];

S32 fmt_delete_by_filter_hdlr(U8 *path, FMGR_FILTER *filter_mask)
{
	FS_HANDLE		handle;
	FS_DOSDirEntry	file_info;
	U8 		file_name[40];
	S32		fs_ret;
	U8		i;

	pfnUnicodeStrcat((PS8)path, (PS8)L"*.*");
	handle = FS_FindFirst((WCHAR *)path, 0, 0, &file_info, (WCHAR *)file_name, 40);
	if (handle < 0)
		return FS_NO_ERROR;

	mmi_fmgr_remove_last_dir((char*)path);
	
	fs_ret = FS_SetCurrentDir((WCHAR *)path);
	if (fs_ret < 0)
		return fs_ret;

	while (1)
	{
		for(i = 0 ; i < FMGR_MAX_FILTER_COUNT ; i++)
		{
			if( FMGR_FILTER_IS_SET(filter_mask, fmgr_filter[i].type) )
			{
				if (strncmp((char *)file_info.Extension, (char *)fmgr_filter[i].ext, 3) == 0)
				{
					fs_ret = FS_Delete((WCHAR *)file_name);
					if (fs_ret < 0)
						return fs_ret;
					break;
				}
			}
		}

		fs_ret = FS_FindNext (handle, &file_info, (WCHAR *)file_name, 40);
		if (fs_ret < 0)
			break;
	}

	FS_FindClose (handle);
	return FS_NO_ERROR;
}

void fmt_delete_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr)
{
	mmi_fmt_delete_req_strcut *msg_ptr;
	mmi_fmt_delete_rsp_strcut param_ptr;
   S32 fs_ret;
   
	msg_ptr = (mmi_fmt_delete_req_strcut *)local_para_ptr;

	if (msg_ptr->type == FS_RECURSIVE_TYPE)
	{
		fs_ret = FS_XDelete((WCHAR*)msg_ptr->path, FS_FILE_TYPE | FS_DIR_TYPE | FS_RECURSIVE_TYPE);
	}
	else if (msg_ptr->type == FS_DIR_TYPE)
	{
		if (FMGR_FILTER_IS_SET(((FMGR_FILTER*)(msg_ptr->filter)), FMGR_TYPE_ALL))
		fs_ret = FS_XDelete ((WCHAR*)msg_ptr->path, FS_FILE_TYPE);
		else
			fs_ret = fmt_delete_by_filter_hdlr(msg_ptr->path, (FMGR_FILTER*)msg_ptr->filter);
	}
	else if (msg_ptr->type == FS_FILE_TYPE)
	{
	   fs_ret = FS_Delete((WCHAR*)msg_ptr->path);
	}

   param_ptr.result = fs_ret;
   
   fmgr_delete_rsp_hdlr(&param_ptr);
   return;
}

void fmt_copy_hdlr(local_para_struct *local_para_ptr,
                                   			peer_buff_struct *peer_buff_ptr)
{
	mmi_fmt_copy_req_strcut *msg_ptr;
	mmi_fmt_copy_rsp_strcut param_ptr;
   S32 fs_ret;
   
	msg_ptr = (mmi_fmt_copy_req_strcut *)local_para_ptr;

	if (msg_ptr->action == FMT_COPY)
	{
	   fs_ret = FS_Move((WCHAR *)msg_ptr->src_file, (WCHAR *)msg_ptr->dest_file,
	            FS_MOVE_COPY, NULL);
	}
	else 
	{
	   fs_ret = FS_Move((WCHAR *)msg_ptr->src_file, (WCHAR *)msg_ptr->dest_file,
	            FS_MOVE_KILL, NULL);
	}

   param_ptr.result = fs_ret;

   fmgr_copy_rsp_hdlr(&param_ptr);
   return;
}
#endif

int g_chset_text_encoding;

#endif


