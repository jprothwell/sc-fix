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
 *	 FileMgrCore.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  File Manager.
 *
 * Author:
 * -------
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 ********************************************************************************/
#ifndef MMI_ON_HARDWARE_P
    #undef __DRM_SUPPORT__
    #undef __DRM_V01__
#endif

#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

#include "debuginitdef.h"
#include "commonscreens.h"
#include "settingprofile.h"		/* error tone */

#include "fat_fs.h"
#include "fmt_def.h"

#include "filemgr.h"

#ifdef __DRM_SUPPORT__
    #include "drm_gprot.h"
    #include "drm_main.h"
#endif /*__DRM_SUPPORT__*/

#include "filemanagerdef.h"
#include "filemgrstruct.h"
#include "filemanagergprot.h"
#include "fs_func.h"

#undef __NEWSIMULATOR

/*WUZC Modify Start For MMIFS Ver:    on 2007-1-12 16:48 */
#include "mmi_trace.h"
//S8 nPrintableStr[256*2];
/*WUZC Modify End  For MMIFS Ver:    on 2007-1-12 16:48 */
//extern S8 nPrintableStr[];
#include "gdi_internal.h"
extern fmgr_context_struct    *fmgr_p;
extern  fmgr_filter_struct       fmgr_filter[FMGR_MAX_FILTER_COUNT];

U8 fmgr_construct_filter_pattern(void)
{
    U8      i, count=0;

    TRACE_FMGR_FUNCTION();
    memset (fmgr_p->filter_pattern, 0, sizeof (fmgr_p->filter_pattern));

    if ( FMGR_FILTER_IS_SET(&fmgr_p->filter_type,FMGR_TYPE_ALL) )
        return 0;

    for ( i=0; i<FMGR_MAX_FILTER_COUNT; i++ )
    {
        if ( FMGR_FILTER_IS_SET(&fmgr_p->filter_type, fmgr_filter[i].type) )
        {
            S8 *ptr;
            U16 len;

            pfnUnicodeStrcpy((S8*)fmgr_p->filter_pattern[count].Pattern, (S8*)L"*.");
            if ( (ptr = (S8*)strstr((S8*)fmgr_filter[i].ext, (S8*)" ")) != NULL )
                len = ptr - fmgr_filter[i].ext ;
            else
                len = strlen (fmgr_filter[i].ext);
            AnsiiNToUnicodeString((S8*)&fmgr_p->filter_pattern[count].Pattern[2],
            (S8*)fmgr_filter[i].ext, len);
            count++;
        }
    }
    return count;
}

S32 fmgr_get_parent_highlight_id(void)
{
    U16 i=0;

    while ( fmgr_p->cache_end_idx <= fmgr_p->file_count )
    {
        for ( i=fmgr_p->cache_start_idx; i<fmgr_p->cache_end_idx; i++ )
        {
            if ( pfnUnicodeStrcmp((PS8)fmgr_p->parent_name, 
            (PS8)fmgr_p->file_cache[i-fmgr_p->cache_start_idx].file_name) == 0 )
                return i;
        }
        if ( i < fmgr_p->file_count - 1 )
            mmi_fmgr_kernel_load_cache((U16)(i+1));
        else
            break;
    }

    return 0;
}

/*****************************************************************************
******************************************************************************
* From this line is File Manager [Caching Kernal]
*****************************************************************************
*****************************************************************************/
void mmi_fmgr_kernel_move_selection_up(void)
{
    S32 load_index;

    if ( fmgr_p->cur_index > 0 )
        fmgr_p->cur_index--;
    else
	/*WeiD Modify Start For 6176 Ver: TBM780  on 2007-8-20 14:38 */
	fmgr_p->cur_index = fmgr_p->file_filter_count-1;
	/*WeiD Modify End  For 6176 Ver: TBM780  on 2007-8-20 14:38 */

    /* out of cache, reload cache */
    if ( (fmgr_p->cur_index < fmgr_p->cache_start_idx) || (fmgr_p->cur_index >= fmgr_p->cache_end_idx) )
    {
        /* not in the cache, reload the cache, and put the desired entry in the middle of cache */
        load_index = fmgr_p->cur_index - (FMGR_CACHE_ENTRY_COUNT/2);

        if ( load_index < 0 )
            load_index = 0;

        /* load the info into cache */
        mmi_fmgr_kernel_load_cache((U16)load_index);
    }

}

void mmi_fmgr_kernel_move_selection_down(void)
{
    S32 load_index;


    /*WeiD Modify Start For 6176 Ver: TBM780  on 2007-8-20 14:39 */
    if ( fmgr_p->cur_index < fmgr_p->file_filter_count-1 )
    /*WeiD Modify End  For 6176 Ver: TBM780  on 2007-8-20 14:39 */
        fmgr_p->cur_index++;
    else
        fmgr_p->cur_index = 0;

    /* out of cache, reload cache */
    if ( (fmgr_p->cur_index < fmgr_p->cache_start_idx) || (fmgr_p->cur_index >= fmgr_p->cache_end_idx) )
    {
        /* not in the cache, reload the cache, and put the desired entry in the middle of cache */
        load_index = fmgr_p->cur_index - (FMGR_CACHE_ENTRY_COUNT/2);

        if ( load_index < 0 )
            load_index = 0;

        /* load the info into cache */
        mmi_fmgr_kernel_load_cache((U16)load_index);
    }
}

/*WeiD Add Start For 6176 Ver: TBM780  on 2007-8-20 14:39 */
U16 mmi_fmgr_kernel_get_file_filter_count()
{
	return fmgr_p->file_filter_count;	
}
/*WeiD Add End  For 6176 Ver: TBM780  on 2007-8-20 14:39 */

U16 mmi_fmgr_kernel_get_file_count(void)
{
    return fmgr_p->file_count;
}

S32 mmi_fmgr_kernel_get_cur_idx(void)
{
    return fmgr_p->cur_index;
}

void mmi_fmgr_kernel_set_cur_idx(S32 index)
{
    fmgr_p->cur_index = index;
}

void mmi_fmgr_kernel_hide_ext_name(S8 *str_buf_ptr, FMGR_FILE_INFO_STRUCT *info_ptr)
{
    S32 str_len;
    S32 ext_str_len;

    TRACE_FMGR_FUNCTION();
    str_len     = pfnUnicodeStrlen((PS8)str_buf_ptr);
    ext_str_len = pfnUnicodeStrlen((PS8)info_ptr->file_ext);
    ext_str_len += 1;

#ifndef __MMI_SHOW_FILE_EXT__
    /* hide all ext */
    if ( !(info_ptr->attribute & FS_ATTR_DIR) ) /* not folder */
        memset(&str_buf_ptr[(str_len-ext_str_len)*ENCODING_LENGTH], 0, ext_str_len*ENCODING_LENGTH);
#else
    /* show others except and test daf */
#ifndef __MMI_SHOW_DAF_FILE_EXT__
    /* hide daf */

    if ( FMGR_FILTER_IS_SET(&info_ptr->file_type, FMGR_TYPE_DAF) )
    {
        memset(&str_buf_ptr[(str_len-ext_str_len)*ENCODING_LENGTH], 0, ext_str_len*ENCODING_LENGTH); /* 4 is for ".daf" */
    }
#endif /* __MMI_SHOW_DAF_FILE_EXT__ */
#endif /* __MMI_SHOW_FILE_EXT__ */

}

void mmi_fmgr_kernel_get_path_name(S8 *full_path_name)
{
    pfnUnicodeStrcpy((PS8)full_path_name, (PS8)fmgr_p->file_path);
}

BOOL mmi_fmgr_kernel_pass_filter_check(FS_DOSDirEntry *file_info_p, FMGR_FILTER *file_type_p, S8* buf_file_name)
{
    BOOL    is_pass_filter;
    S32 i;
    S32 filter_count;
    FMGR_FILTER file_type;

    /* init */
    is_pass_filter  = FALSE;
    FMGR_FILTER_INIT(&file_type);
    FMGR_FILTER_SET(&file_type, FMGR_TYPE_UNKNOW);

    /* is folder */
    if ( file_info_p->Attributes & FS_ATTR_DIR )
    {
        if ( pfnUnicodeStrcmp( buf_file_name, (S8*)L".." ) == 0 )
        {
            FMGR_FILTER_SET(&file_type, FMGR_TYPE_FOLDER_DOT);
            FMGR_FILTER_CLEAR(&file_type, FMGR_TYPE_UNKNOW);
        }
        else
        {
            FMGR_FILTER_SET(&file_type, FMGR_TYPE_FOLDER);
            FMGR_FILTER_CLEAR(&file_type, FMGR_TYPE_UNKNOW);
        }


        if ( FMGR_FILTER_IS_SET(&fmgr_p->filter_type,FMGR_TYPE_ALL) )
        {
            /* don't show . and .. folder */
            if ( pfnUnicodeStrcmp( buf_file_name, (S8*)L"." ) != 0 && 
            pfnUnicodeStrcmp( buf_file_name, (S8*)L".." ) != 0 )
            {
                is_pass_filter = TRUE;
            }
        }
        else
        {
            if ( FMGR_FILTER_IS_SET(&fmgr_p->filter_type, FMGR_TYPE_FOLDER) )
            {
                if ( (pfnUnicodeStrcmp( buf_file_name, (S8*)L"." ) != 0) &&
                (pfnUnicodeStrcmp( buf_file_name, (S8*)L".." ) != 0) )
                {
                    is_pass_filter = TRUE;
                }
            }
            else if ( FMGR_FILTER_IS_SET(&fmgr_p->filter_type, FMGR_TYPE_FOLDER_DOT) )
            {
                if ( pfnUnicodeStrcmp( buf_file_name, (S8*)L"." ) != 0 )
                {
                    FMGR_FILTER_CLEAR(&file_type, FMGR_TYPE_UNKNOW);
                    is_pass_filter      = TRUE;
                }
            }
        }
    }
    else
    {
        S8        buf_file_ext[FMGR_MAX_EXT_LEN+1];
        S8 *ext_ptr;

        /* get file extension - and make it upper case */
        memset (buf_file_ext, 0, sizeof(buf_file_ext));
        ext_ptr = mmi_fmgr_extract_ext_file_name((PS8)buf_file_name);
        if ( ext_ptr != NULL )
        {
            UnicodeNToAnsii((PS8)buf_file_ext, ext_ptr, FMGR_MAX_EXT_LEN<<1);
        }

        for ( i = 0 ; i < (S32)strlen ((char*)buf_file_ext) ; i++ )
        {
            if ( buf_file_ext[i] >= 'a' )
                buf_file_ext[i] += 'A' - 'a';
        }

        if ( buf_file_ext[2] == 0 )
            buf_file_ext[2] = ' ';

        /* use extension to check filter */
        filter_count    = sizeof(fmgr_filter)/sizeof(fmgr_filter_struct);

        for ( i = 0 ; i < filter_count ; i++ )
        {
            if ( FMGR_FILTER_IS_SET(&fmgr_p->filter_type, fmgr_filter[i].type) )
            {
                if ( strcmp ((char*)buf_file_ext, (char*)fmgr_filter[i].ext) == 0 )
                /*if((buf_file_ext[0] == fmgr_filter[i].ext[0]) &&
                  (buf_file_ext[1] == fmgr_filter[i].ext[1]) &&
                  (buf_file_ext[2] == fmgr_filter[i].ext[2]))*/
                {

                    FMGR_FILTER_SET(&file_type, fmgr_filter[i].type);
                    FMGR_FILTER_CLEAR(&file_type, FMGR_TYPE_UNKNOW);
                    is_pass_filter  = TRUE;
        			mmi_trace (g_sw_FMGR, "gdm: into mmi_fmgr_kernel_pass_filter_check() return is_pass_filter=TRUE");
	        		mmi_trace (g_sw_FMGR, "gdm: buf_file_ext = %s, fmgr_filter[%d].ext=%s", buf_file_ext, i,fmgr_filter[i].ext);
                    break;
                }
            }
        }

        if ( FMGR_FILTER_IS_SET(&fmgr_p->filter_type, FMGR_TYPE_ALL) )
        {
            is_pass_filter = TRUE;
        }
    }

    /* assign return value */
    *file_type_p = file_type;

    return is_pass_filter;
}


BOOL mmi_fmgr_kernel_delete_all(void)
{
    S32 fs_ret;

    /* delete files including drm file + rights */
    fs_ret = MMI_FS_XDelete ((PU8)fmgr_p->file_path, FS_FILE_TYPE);

    if ( !(fs_ret >= FS_NO_ERROR) )
    {
        /* fail */
        UnicodeToAnsii((PS8)nPrintableStr, (PS8)fmgr_p->file_path);
        PRINT_INFORMATION(("delete all fail: %s", nPrintableStr));

        DisplayPopup((PU8)GetString(GetFileSystemErrorString(fs_ret)),
        IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);

        return FALSE;
    }

    return TRUE;
}

void mmi_fmgr_kernel_load_cache(U16 start_index)
{
    S8              buf_file_path[(FMGR_MAX_PATH_LEN+5)*ENCODING_LENGTH];
    U16         cache_index=0;
#ifdef __FS_SORT_SUPPORT__
    UINT            seq = 1;
#endif
#if defined(MMI_ON_WIN32)
    S8              buf_file_ext[(FMGR_MAX_EXT_LEN+1)*ENCODING_LENGTH];
    S32         fs_ret;
    U8              filter_count = 0;
#endif

    TRACE_FMGR_FUNCTION();

    GDI_ASSERT(start_index < fmgr_p->file_count);

    /* init some value */
    fmgr_p->cache_start_idx = start_index;
    //fmgr_p->cache_end_idx		= 0;

    memset ((PU8)fmgr_p->file_cache, 0, sizeof(FMGR_FILE_INFO_STRUCT)*FMGR_CACHE_ENTRY_COUNT);

    /* define search file */
    /* create "filepath\*.*" */
#if defined(MMI_ON_WIN32)
    AnsiiToUnicodeString((PS8)buf_file_ext,"*");
    pfnUnicodeStrcpy((PS8)buf_file_path, (PS8)fmgr_p->file_path);
    if ( buf_file_path[(pfnUnicodeStrlen(buf_file_path) - 1) << 1] != '/' )
    {
        pfnUnicodeStrcat((PS8)buf_file_path, (PS8)L"/");
    }
    pfnUnicodeStrcat((PS8)buf_file_path, (PS8)buf_file_ext);

    filter_count = fmgr_construct_filter_pattern();
   
#ifdef __FS_SORT_SUPPORT__
    if ( fmgr_p->src_application != APP_FILEMANAGER && 
    (fmgr_p->sel_type & FMGR_SELECT_BROWSE) )
    {
        fs_ret = MMI_FS_XFindReset((PU16)buf_file_path, fmgr_p->filter_pattern, filter_count,
        0, FS_ATTR_HIDDEN | FS_ATTR_SYSTEM | FS_ATTR_VOLUME,
        FS_FILE_TYPE|fmgr_p->sort_option, TRUE, &seq);
    }
    else
    {
        fs_ret = MMI_FS_XFindReset((PU16)buf_file_path,  fmgr_p->filter_pattern, filter_count,
        FS_ATTR_DIR, FS_ATTR_HIDDEN|FS_ATTR_SYSTEM|FS_ATTR_VOLUME,
        FS_DIR_TYPE|FS_FILE_TYPE|fmgr_p->sort_option, TRUE, &seq);
    }

#else
    fs_ret = FS_TOO_MANY_FILES;
#endif
    if ( fs_ret < FS_NO_ERROR )
    {
        if ( fs_ret == FS_TOO_MANY_FILES )
        {
            /* too many files, can't sort */
            TRACE_EF(g_sw_FMGR, "mmi_fmgr_kernel_load_cache(): start_index = %d",start_index);
            mmi_fmgr_kernel_list((PS8)buf_file_path, &cache_index,
            fmgr_p->cache_start_idx, FALSE);
        }
        else
        {
            fmgr_p->cache_end_idx = start_index;
            return;
        }
    }
#endif

#ifdef __FS_SORT_SUPPORT__
    if( fmgr_p->file_count > 0)
     {
       mmi_fmgr_kernel_init_continue(buf_file_path, &seq, fmgr_p->file_count,
                                          &cache_index, fmgr_p->cache_start_idx, FALSE);
    }
#if defined(MMI_ON_WIN32)
    MMI_FS_XFindClose(&seq);
#else
    //MMI_FS_XFindClose(&seq);
#endif
#endif
    fmgr_p->cache_start_idx = start_index;
    fmgr_p->cache_end_idx = start_index + cache_index;
}


void mmi_fmgr_kernel_get_file_info(S32 index, FMGR_FILE_INFO_STRUCT *file_info_ptr)
{
    S16 load_index;

    GDI_ASSERT(index < fmgr_p->file_count);
    if ( (index < fmgr_p->cache_start_idx) || (index >= fmgr_p->cache_end_idx) )
    {
        /* desired entry in the middle of cache */
        load_index = index - (FMGR_CACHE_ENTRY_COUNT/2);

        if ( load_index < 0 )
        {
            load_index = 0;
        }

        mmi_fmgr_kernel_load_cache(load_index);
    }

    memcpy(file_info_ptr, &fmgr_p->file_cache[index-fmgr_p->cache_start_idx], sizeof(FMGR_FILE_INFO_STRUCT));
}

void mmi_fmgr_kernel_init(S8 *file_path, FMGR_FILTER filter,  U16 *file_count_ptr)
{
 
    S8              buf_file_path[(FMGR_MAX_PATH_LEN+5)*ENCODING_LENGTH];
    S8              buf_file_ext[(FMGR_MAX_EXT_LEN+1)*ENCODING_LENGTH];
    S32             fs_ret;
#ifdef __FS_SORT_SUPPORT__
    UINT            seq;
#endif	
    U8              filter_count = 0;

    TBM_ENTRY(0x2848);
    TRACE_FMGR_FUNCTION();
    pfnUnicodeStrcpy((PS8)fmgr_p->file_path, (PS8)file_path);

    /* init some global value */
    fmgr_p->file_count      = 0;
    fmgr_p->cache_start_idx = 0;
    fmgr_p->cache_end_idx   = 0;
    fmgr_p->filter_type     = filter;
    (*file_count_ptr) = 0;

    memset ((PU8)fmgr_p->file_cache, 0, sizeof(FMGR_FILE_INFO_STRUCT)*FMGR_CACHE_ENTRY_COUNT);

    pfnUnicodeStrcpy((PS8)buf_file_path, (PS8)file_path);
    if ( buf_file_path[(pfnUnicodeStrlen(buf_file_path) - 1) << 1] != '/' )
        pfnUnicodeStrcat((PS8)buf_file_path, (PS8)L"/");

    /* define search file */
    /* create "filepath\*.*" */
    AnsiiToUnicodeString((PS8)buf_file_ext,"*");

    pfnUnicodeStrcat((PS8)buf_file_path, (PS8)buf_file_ext);
    filter_count = fmgr_construct_filter_pattern ();
#ifdef __FS_SORT_SUPPORT__
    /* don't display folder when browse from app */
    if ( fmgr_p->src_application != APP_FILEMANAGER && 
    (fmgr_p->sel_type & FMGR_SELECT_BROWSE) )
    {
		if ( ( fmgr_p->src_application == APP_SOUNDRECORDER ) && ( fmgr_p->sort_option & FS_SORT_TIME))
		{
			fmgr_p->sort_option = FS_SORT_DTIME;
		}
		fs_ret = MMI_FS_XFindReset((PU8)buf_file_path, fmgr_p->filter_pattern, filter_count,
		0, FS_ATTR_HIDDEN | FS_ATTR_SYSTEM | FS_ATTR_VOLUME,
		FS_FILE_TYPE|fmgr_p->sort_option, TRUE, (void*)&seq);
		if ( ( fmgr_p->src_application == APP_SOUNDRECORDER ) && ( fmgr_p->sort_option & FS_SORT_DTIME))
		{
			fmgr_p->sort_option = FS_SORT_TIME;
		}
    }
    else
        fs_ret = MMI_FS_XFindReset((PU8)buf_file_path, fmgr_p->filter_pattern, filter_count,
        FS_ATTR_DIR, FS_ATTR_HIDDEN | FS_ATTR_SYSTEM | FS_ATTR_VOLUME,
        FS_DIR_TYPE|FS_FILE_TYPE|fmgr_p->sort_option, TRUE, (void*)&seq);
#else
    fs_ret = FS_TOO_MANY_FILES;
#endif

    if ( fs_ret >= 0 )
    {
        fmgr_p->file_count = fs_ret;   //record all file count
    }

#if defined(MMI_ON_WIN32)
    if ( fs_ret < FS_NO_ERROR )
    {
        if ( fs_ret == FS_TOO_MANY_FILES )
        {
            /* too many files, can't sort */
            fmgr_p->file_count = mmi_fmgr_kernel_list((PS8)buf_file_path,
            &fmgr_p->cache_end_idx, fmgr_p->cache_start_idx, TRUE);
            TRACE_EF(g_sw_FMGR, " fmgr_p->file_count = %d, fmgr_p->cache_end_idx = %d, fmgr_p->cache_start_idx = %d", fmgr_p->file_count, fmgr_p->cache_end_idx, fmgr_p->cache_start_idx);
            (*file_count_ptr) = fmgr_p->file_count;

        }
        return;
    }
#endif

#ifdef __FS_SORT_SUPPORT__
    TRACE_EF(g_sw_FMGR, "fs_ret = %d, seq = %d", fs_ret, seq);

    if ( fs_ret > 0 )
    {
       // fs_ret : record the file count accord with the filter
       fs_ret = mmi_fmgr_kernel_init_continue(buf_file_path, &seq, fmgr_p->file_count, &fmgr_p->cache_end_idx, 
                                                             fmgr_p->cache_start_idx, TRUE);
    }
    else
    {
        fs_ret = 0;
    }
#ifdef MMI_ON_WIN32
    fmgr_p->file_count = fs_ret;
    MMI_FS_XFindClose(&seq);
#else
	//MMI_FS_XFindClose((INT32*)&seq);
#endif
#endif
    (*file_count_ptr) = fs_ret;
    /*WeiD Add Start For 6176 Ver: TBM780  on 2007-8-20 14:39 */
    fmgr_p->file_filter_count	= fs_ret;
    /*WeiD Add End  For 6176 Ver: TBM780  on 2007-8-20 14:39 */
    TBM_EXIT(0x2848);

}

#ifdef __FS_SORT_SUPPORT__
U16 mmi_fmgr_kernel_init_continue(PS8 path, UINT *seq, S32 total,
PU16 cached_num, U16 start_index,
BOOL read_all)
{

    S8                  buf_file_name[(FMGR_MAX_FILE_NAME_LEN+1)*ENCODING_LENGTH];
    S8                  *ext_ptr = NULL;
    FS_DOSDirEntry  file_info;
    FS_HANDLE       file_handle=0;
    FMGR_FILTER     file_type;
    U16             file_count=0, cached_index, i;
    U16             passflt_count = 0;
    //S16             j;
    BOOL                is_pass_filter, is_short = FALSE;
    TRACE_FMGR_FUNCTION();

    cached_index = *cached_num;

    for ( i=0; i<total; i++ )        
    {
#ifdef  NAME_TRUNC_T0_83      
        file_handle = MMI_FS_XFindStart((PU16)path, &file_info, (PU16)buf_file_name,
        FMGR_MAX_FILE_LEN*ENCODING_LENGTH,
        i, seq, FS_FIND_DEFAULT);
#else    

        file_handle = MMI_FS_XFindStart((PU8)path, &file_info, (PU8)buf_file_name,
        FMGR_MAX_FILE_LEN*ENCODING_LENGTH,
        i, (INT32*)seq, FS_FIND_LFN_TRUNC);
#endif

  
        if ( file_handle >= 0 )
        {
            /* check if pass filter and get file type */
            is_pass_filter = mmi_fmgr_kernel_pass_filter_check(&file_info, &file_type, buf_file_name);

            /* test if pass the filter */
            if ( is_pass_filter == TRUE && ((file_info.Attributes & FS_ATTR_HIDDEN) == 0) )
            {
                passflt_count++;
                /* still have entry to fill in the cache */
                if ( (passflt_count > start_index) && (cached_index < FMGR_CACHE_ENTRY_COUNT ) )
                {
#if 0    
#ifndef __MMI_SHOW_FILE_EXT__
                    if ( !(file_info.Attributes & FS_ATTR_DIR) )
                    {
                        U16 len;

                        len = pfnUnicodeStrlen((PS8)buf_file_name);
                        if ( file_info.Extension[0] != ' ' )
                            memset (&buf_file_name[(len-4)<<1], 0, 8);
                    }
#endif
#endif
                    /* copy file name */
                    is_short = FALSE;
                    if ( file_info.NTReserved == FS_SFN_MATCH )
                    {
                        is_short = TRUE;
                    }
                    pfnUnicodeStrcpy((PS8)fmgr_p->file_cache[cached_index].file_name, (PS8)buf_file_name);
                    /* get file extension */
                    memset((PS8)fmgr_p->file_cache[cached_index].file_ext, 0, sizeof(fmgr_p->file_cache[cached_index].file_ext));
#if 0                    
#ifndef __MMI_SHOW_FILE_EXT__

                    fmgr_p->file_cache[cached_index].file_ext[0] = '.';
                    AnsiiNToUnicodeString((PS8)&(fmgr_p->file_cache[cached_index].file_ext[2]), (PS8)file_info.Extension, 3);
#else
                    AnsiiNToUnicodeString((PS8)fmgr_p->file_cache[cached_index].file_ext, (PS8)file_info.Extension, 3);

                    for ( j=(sizeof (file_info.Extension)-1)*2; j>=0; j-=2 )
                    {
                        if ( fmgr_p->file_cache[cached_index].file_ext[j] == ' ' )
                       {
                            fmgr_p->file_cache[cached_index].file_ext[j] = '\0';
                        }
                    }
#endif
#endif
                    ext_ptr = mmi_fmgr_extract_ext_file_name((PS8)buf_file_name);
                    if ( ext_ptr != NULL )
                    {
                        pfnUnicodeStrncpy((PS8)(fmgr_p->file_cache[cached_index].file_ext), ext_ptr, FMGR_MAX_EXT_LEN<<1);
                    }

                    fmgr_p->file_cache[cached_index].is_short           = is_short;
                    fmgr_p->file_cache[cached_index].index                  = fmgr_p->file_count;
                    fmgr_p->file_cache[cached_index].file_type          = file_type;
                    fmgr_p->file_cache[cached_index].attribute          = file_info.Attributes;
                    fmgr_p->file_cache[cached_index].time.nYear         = file_info.DateTime.Year1980+1980;
                    fmgr_p->file_cache[cached_index].time.nMonth            = file_info.DateTime.Month;
                    fmgr_p->file_cache[cached_index].time.nDay          = file_info.DateTime.Day;
                    fmgr_p->file_cache[cached_index].time.nHour         = file_info.DateTime.Hour;
                    fmgr_p->file_cache[cached_index].time.nMin          = file_info.DateTime.Minute;
                    fmgr_p->file_cache[cached_index].time.nSec          = file_info.DateTime.Second2;
                    fmgr_p->file_cache[cached_index].file_size          = file_info.FileSize;
                    fmgr_p->file_cache[cached_index].is_used                = TRUE;
                    fmgr_p->file_cache[cached_index].is_able_display    = TRUE;


                    cached_index++;
                }

                /* increase counter */
                file_count++;
            }

            if ( cached_index == FMGR_CACHE_ENTRY_COUNT && read_all == FALSE )
                break;

        }
    }
    *cached_num = cached_index;
    return file_count;
}
#endif

U16 mmi_fmgr_kernel_list (S8 *file_path, U16 *cached_num, U16 start_index,
BOOL read_all)
{
    S8                  buf_file_name[FMGR_MAX_FILE_NAME_LEN*ENCODING_LENGTH];
    S8                  *ext_ptr = NULL;
    FS_DOSDirEntry  file_info;
    FS_HANDLE       file_handle;
    FMGR_FILTER     file_type;
    S16             return_value;
    U16             file_count = 0, cache_index;
    BOOL                is_pass_filter, is_short = FALSE;
    U8                  filter_count=0;

    TRACE_FMGR_FUNCTION();
    TRACE_EF(g_sw_FMGR, "file_path = %s", file_path);
    cache_index = *cached_num;

    filter_count = fmgr_construct_filter_pattern ();
    if ( fmgr_p->src_application != APP_FILEMANAGER && 
    (fmgr_p->sel_type & FMGR_SELECT_BROWSE) )
    {
        TRACE_FMGR_FUNC("FMGR_SELECT_BROWSE");
        file_handle = MMI_FS_FindFirst((PU8)file_path, 0, FS_ATTR_SYSTEM|FS_ATTR_HIDDEN|FS_ATTR_VOLUME,
        &file_info, (PU8)buf_file_name,
        FMGR_MAX_FILE_LEN*ENCODING_LENGTH);
    }
    else
    {
        TRACE_FMGR_FUNC("!FMGR_SELECT_BROWSE");
        file_handle = MMI_FS_FindFirst((PU8)file_path, 0, FS_ATTR_SYSTEM|FS_ATTR_HIDDEN|FS_ATTR_VOLUME,
        &file_info, (PU8)buf_file_name,
        FMGR_MAX_FILE_LEN*ENCODING_LENGTH);
    }
    TRACE_EF(g_sw_FMGR, "file_handle = %d", file_handle);
    if ( file_handle >= 0 )
    {
        do
        {
            /* check if pass filter and get file type */
            is_pass_filter = mmi_fmgr_kernel_pass_filter_check(&file_info, &file_type, buf_file_name);

            /* test if pass the filter */
            if ( is_pass_filter == TRUE && ((file_info.Attributes & FS_ATTR_HIDDEN) == 0) )
            {
                /* still have entry to fill in the catch */
                if ( cache_index < FMGR_CACHE_ENTRY_COUNT )
                {
#ifndef __MMI_SHOW_FILE_EXT__
                    if ( !(file_info.Attributes & FS_ATTR_DIR) )
                    {
                        U16 len;

                        len = pfnUnicodeStrlen((PS8)buf_file_name);
                        if ( file_info.Extension[0] != ' ' )
                            memset (&buf_file_name[(len-sizeof (file_info.Extension)-1)<<1], 0, 8);
                    }
#endif

                    /* get file name */
                    is_short = FALSE;
                    if ( file_info.NTReserved == FS_SFN_MATCH )
                        is_short = TRUE;
                    pfnUnicodeStrcpy((PS8)fmgr_p->file_cache[cache_index].file_name, 
                    (PS8)buf_file_name);

                    /* get file extension */
                    memset((PS8)fmgr_p->file_cache[cache_index].file_ext, 0,
                    sizeof(fmgr_p->file_cache[cache_index].file_ext));
#ifndef __MMI_SHOW_FILE_EXT__
                    fmgr_p->file_cache[cache_index].file_ext[0] = '.';
                    AnsiiNToUnicodeString((PS8)&(fmgr_p->file_cache[cache_index].file_ext[2]),
                    (PS8)file_info.Extension, 3);
#else
                    AnsiiNToUnicodeString((PS8)fmgr_p->file_cache[cache_index].file_ext, (PS8)file_info.Extension, 3);
#endif
#if 0
                    for ( j=(sizeof (file_info.Extension)-1)*2; j>=0; j-=2 )
                    {
                        if ( fmgr_p->file_cache[cache_index].file_ext[j] == ' ' )
                            fmgr_p->file_cache[cache_index].file_ext[j] = '\0';
                    }
#endif
                    ext_ptr = mmi_fmgr_extract_ext_file_name((PS8)buf_file_name);
                    if ( ext_ptr != NULL )
                    {
                        pfnUnicodeStrncpy((PS8)(fmgr_p->file_cache[cache_index].file_ext), ext_ptr, FMGR_MAX_EXT_LEN<<1);
                    }

                    fmgr_p->file_cache[cache_index].is_short                = is_short;
                    fmgr_p->file_cache[cache_index].index                   = file_count;
                    fmgr_p->file_cache[cache_index].file_type               = file_type;
                    fmgr_p->file_cache[cache_index].attribute               = file_info.Attributes;
                    fmgr_p->file_cache[cache_index].time.nYear          = file_info.DateTime.Year1980+1980;
                    fmgr_p->file_cache[cache_index].time.nMonth         = file_info.DateTime.Month;
                    fmgr_p->file_cache[cache_index].time.nDay               = file_info.DateTime.Day;
                    fmgr_p->file_cache[cache_index].time.nHour          = file_info.DateTime.Hour;
                    fmgr_p->file_cache[cache_index].time.nMin               = file_info.DateTime.Minute;
                    fmgr_p->file_cache[cache_index].time.nSec               = file_info.DateTime.Second2;
                    fmgr_p->file_cache[cache_index].file_size               = file_info.FileSize;
                    fmgr_p->file_cache[cache_index].is_used             = TRUE;
                    fmgr_p->file_cache[cache_index].is_able_display = TRUE;

                    cache_index++;
                }

                /* increase counter */
                file_count++;
            }

            if ( cache_index == FMGR_CACHE_ENTRY_COUNT && read_all == FALSE )
                break;

            if ( fmgr_p->src_application != APP_FILEMANAGER && 
            (fmgr_p->sel_type & FMGR_SELECT_BROWSE) )
                return_value = MMI_FS_FindNext(file_handle, &file_info, (PU8)buf_file_name, FMGR_MAX_FILE_LEN*ENCODING_LENGTH);
            else
                return_value = MMI_FS_FindNext(file_handle, &file_info, (PU8)buf_file_name, FMGR_MAX_FILE_LEN*ENCODING_LENGTH);

        } while ( return_value == FS_NO_ERROR );

        MMI_FS_FindClose(file_handle);
    }
    *cached_num = cache_index;
    return file_count;
}

