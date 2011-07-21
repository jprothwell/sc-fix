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
/*****************************************************************************
 *
 * Filename:
 * ---------
 * ThemeManager.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This is the source file for Theme Manager.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "thememanager.h"
#include "themeconverter.h"
#include "gui.h"
#include "unicodexdcl.h"
#include "filesystemdef.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "nvramtype.h"
#include "debuginitdef.h"
#include "mmithemes.h"
#include "filesystemgprot.h"
#include "eventsgprot.h"
#include "funandgamesdefs.h"
#include "gui_themes.h"
#include "drm_gprot.h"
#include "wallpaperdefs.h"
#include "screensaverdefs.h"
#include "profilegprots.h"
#include "downloaddefs.h"
#include "queuegprot.h"
#include "simdetectiongexdcl.h"
#include "phonebookdef.h"
#include "globaldefs.h"
#include "protocolevents.h"
 
#include "filemgr.h"
#include "filesystemdef.h"
#include "filemanagerdef.h"
#include "mypicture.h"
#include "commonscreens.h"
#undef __NEWSIMULATOR 

/***************************************************************************** 
* Define
*****************************************************************************/
#if (defined(__MMI_THEMES_V2_SUPPORT__) || defined(__MMI_DOWNLOADABLE_THEMES_SUPPORT__)) //ritesh

#if (!defined(MMI_ON_HARDWARE_P) && !defined(NVRAM_EF_THEME_MANAGER_RECORD_SIZE) ) 
#define NVRAM_EF_THEME_MANAGER_RECORD_SIZE (sizeof(theme_list_struct))*MAX_DOWNLOADED_THEMES
#endif
#define NVRAM_EF_THEME_MANAGER_RECORD_TOTAL 1
/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Local Variable
*****************************************************************************/

/***************************************************************************** 
* Local Function
*****************************************************************************/

/***************************************************************************** 
* Global Variable
*****************************************************************************/
static mmi_theme_manager_context_struct g_thm_mgr_cntx;
static theme_list_struct g_theme_list[TOTAL_THEMES_COUNT]; 
static theme_details_struct g_activated_theme; 
static U8 g_theme_image_dirty_bits[(MAX_MMI_IMAGEID_IMAGES/8)+1];
static S8 *gpPhThmNamePtr;
static S8 gCardThmNameArr[20*MAX_THEME_NAME_LENGTH*ENCODING_LENGTH];
static theme_list_struct gNvramThmArr[MAX_DOWNLOADED_THEMES];

extern oslMsgqid mmi_ext_qid;
extern kal_uint8 keypad_flag;
extern drv_get_key_func keypad_ptr;
extern DYNAUDIOINFO gDwnlThmToneInfo[MAXIMUM_THEMES_TONES];
extern void initialize_status_icons();
extern void mmi_nsm_create_dir(S8 *DirInAnsii);
extern U8 gaudiothmfileformat;
/***************************************************************
Start Supported Theme version Table
***************************************************************/
float32 gSupportedThemeVersionTable[MAX_SUPPORTED_VERSION_NUM]=
{
	2.0
};

/***************************************************************
End of Supported Theme version Table
***************************************************************/
/***************************************************************************** 
* Global Function
*****************************************************************************/


/*****************************************************************************
* FUNCTION
*  myfunc
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh
void tm_create_image_header_pool(theme_image_header_struct* ds, void* mempool, U16 unit_size, U32 total_pool_size)
{ 
	ds->mempool = mempool;
	ds->current_offset = 0;
	ds->unit_size = unit_size;
	ds->total_pool_size = total_pool_size;

}

void tm_reset_imageid_image_header_pool(void) 
{
	tm_reset_image_header_pool(&(g_thm_mgr_cntx.imageid_images_headers));
	memset(&g_thm_mgr_cntx.image_cache,0xFF,sizeof(g_thm_mgr_cntx.image_cache));
}

void tm_reset_non_imageid_image_header_pool(void) 
{
	tm_reset_image_header_pool(&(g_thm_mgr_cntx.non_imageid_images_headers));
}

void tm_reset_image_header_pool(theme_image_header_struct* ds)
{ 
	ds->current_offset = 0;
}


void* tm_add_image_header_to_pool(theme_image_header_struct* ds, void* buff) 
{
	void* temp_ptr;

	if((ds->current_offset+ds->unit_size) > ds->total_pool_size)
	{
		return NULL;
	}
	else 
	{
		memcpy((ds->mempool+ds->current_offset), buff, ds->unit_size);
		temp_ptr = ds->mempool + ds->current_offset;
		ds->current_offset += ds->unit_size;
		return temp_ptr;
	}
}
#endif //ritesh
void mmi_theme_manager_init(void) 
{
	memset((void*)&g_thm_mgr_cntx, 0, sizeof(mmi_theme_manager_context_struct));
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh
	tm_create_image_header_pool(&(g_thm_mgr_cntx.imageid_images_headers), g_thm_mgr_cntx.imageid_images_headers_pool, MAX_THEME_IMAGE_HEADER_LENGTH, (MAX_THEME_IMAGE_HEADER_LENGTH*MAX_THEME_IMAGEID_IMAGES));
	tm_create_image_header_pool(&(g_thm_mgr_cntx.non_imageid_images_headers), g_thm_mgr_cntx.non_imageid_images_headers_pool, MAX_THEME_IMAGE_HEADER_LENGTH, (MAX_THEME_IMAGE_HEADER_LENGTH*MAX_THEME_NON_IMAGEID_IMAGES));
#endif //ritesh
}

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh
void* tm_get_downloaded_theme_image(U32 image_offset, S32 nArrIndex) 
{
	S32 size;
	void*	pImage;
	U8 buff[MAX_THEME_IMAGE_HEADER_LENGTH]={0};

	if(FS_ParseFH(g_thm_mgr_cntx.theme_file_handle)>=0)
	{
		if(g_thm_mgr_cntx.image_cache[nArrIndex].nOffset != -1)
			return (void*)((S8*)g_thm_mgr_cntx.imageid_images_headers.mempool+g_thm_mgr_cntx.image_cache[nArrIndex].nOffset);
			
	if(DRM_seek_file(g_thm_mgr_cntx.theme_file_handle, image_offset, FS_FILE_BEGIN))
	{
		if(DRM_read_file(g_thm_mgr_cntx.theme_file_handle, buff, THEME_IMAGE_INFO_HEADER, (UINT*)&size) != FS_NO_ERROR) 
		{
			return GetDefaultImage();
		}
	}
	else
		return GetDefaultImage();

	//offset in next 4 bytes
	buff[8] = (U8) (image_offset & 0x000000ff);
	buff[9] = (U8) ((image_offset >> 8) & 0x000000ff);
	buff[10] = (U8) ((image_offset >> 16) & 0x000000ff);
	buff[11] = (U8) ((image_offset >> 24) & 0x000000ff);

	//file handle in next 4 bytes
	buff[12] = (U8) (g_thm_mgr_cntx.theme_file_handle & 0x000000ff);
	buff[13] = (U8) ((g_thm_mgr_cntx.theme_file_handle >> 8) & 0x000000ff);
	buff[14] = (U8) ((g_thm_mgr_cntx.theme_file_handle >> 16) & 0x000000ff);
	buff[15] = (U8) ((g_thm_mgr_cntx.theme_file_handle >> 24) & 0x000000ff);
	
		pImage= tm_add_image_header_to_pool(&(g_thm_mgr_cntx.imageid_images_headers), buff);
		if(pImage)
		{
			g_thm_mgr_cntx.image_cache[nArrIndex].nOffset = (U16)((S8*)pImage - (S8*)g_thm_mgr_cntx.imageid_images_headers.mempool);
			return pImage;
		}
	}

	return GetDefaultImage();

}
#endif //ritesh
void* tm_get_system_theme_image(U32 image_index)
{
	return g_activated_theme.system_image_list[image_index];
}

 
 
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
extern U16 mmi_check_theme_file_version_compatibility(float32 theme_version);
extern U16 mmi_check_theme_file_lcd_dimension_compatibiltiy(U32 lcd_width, U32 lcd_height);

extern XML_PARSER_STRUCT parser;
extern deviceinfo gdeviceInfo;
extern theme_download_info_struct gthemeInfo;

U16 gcheckThemeVersion;//Variable check for theme versioning
BOOL theme_version_and_deviceinfo_check(theme_list_struct* theme_struct)
{
	
	
	U8 theme_file_name[100]={0};
	U8 tmp_path[50]={0};
	U16 i=1;
	U16 j=1;
	FS_HANDLE tmp_theme_file_handle;
	

	gcheckThemeVersion = TRUE;
	if(theme_struct->theme_type == THEME_TYPE_DOWNLOADED_PHONE)
	{
		sprintf((PS8)tmp_path, "%c%s", MMI_PUBLIC_DRV, DOWNLOADED_THEMES_PATH);
		AnsiiToUnicodeString((PS8)theme_file_name, (PS8)tmp_path);  
	}
	else
	{
		sprintf((PS8)tmp_path, "%c%s", MMI_CARD_DRV, DOWNLOADED_THEMES_PATH);
		AnsiiToUnicodeString((PS8)theme_file_name, (PS8)tmp_path);  
	}
	pfnUnicodeStrcat((PS8)theme_file_name, (PS8)theme_struct->theme_name);

//	last_theme_file_handle = g_thm_mgr_cntx.theme_file_handle;
	tmp_theme_file_handle = DRM_open_file((U16*)theme_file_name, FS_READ_ONLY, 0/*DRM_PERMISSION_NONE*/); 
	if(tmp_theme_file_handle < FS_NO_ERROR)
		return FALSE;


	if(xml_new_parser(&parser) == -1)
		return FALSE;
	xml_register_element_handler(&parser,my_xml_elem_start_hdlr,my_xml_elem_end_hdlr);
	
	xml_parse_file_from_offset(&parser,tmp_theme_file_handle);
	
	DRM_close_file(tmp_theme_file_handle);
	gcheckThemeVersion = FALSE;
	if(mmi_check_theme_file_version_compatibility((float32)gthemeInfo.version))
	{
		if(mmi_check_theme_file_lcd_dimension_compatibiltiy(gdeviceInfo.lcdwidth,gdeviceInfo.lcdht))
			return TRUE;
	}

	return FALSE;

}

#endif
/************************************************************/
void tm_set_imageid(U16 image_id, U32 image_offset) 
{

	U16 image_index;
	U16	n_byte;
	U8  n_bit;

	if(g_thm_mgr_cntx.theme_imageid_image_count >= MAX_THEME_IMAGEID_IMAGES)
		return;
	else 
	{
		if(image_id == CURRENT_THEME_WALLPAPER)
			g_thm_mgr_cntx.theme_has_wallpaper = TRUE;

		if(image_id == CURRENT_THEME_SCREENSAVER)
			g_thm_mgr_cntx.theme_has_screensaver = TRUE;

		g_thm_mgr_cntx.theme_image_list[g_thm_mgr_cntx.theme_imageid_image_count].image_id = image_id;
		g_thm_mgr_cntx.theme_image_list[g_thm_mgr_cntx.theme_imageid_image_count].image_offset = image_offset;
		++g_thm_mgr_cntx.theme_imageid_image_count;

		g_thm_mgr_cntx.theme_has_alternate_image = TRUE;
		image_index = GetImageIndex(image_id);

		n_byte = (image_index>>3);
		n_bit = (image_index%8);

		g_theme_image_dirty_bits[n_byte] |= (0x80 >> n_bit);
	}
}
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh

UI_image_type tm_create_non_imageid_image(U32 image_offset) 
{
	S32 size;
	U32 fp;
	U8 buff[MAX_THEME_IMAGE_HEADER_LENGTH]={0};

	FS_GetFilePosition(g_thm_mgr_cntx.theme_file_handle, &fp);
	DRM_seek_file(g_thm_mgr_cntx.theme_file_handle, image_offset, FS_FILE_BEGIN); 
	DRM_read_file(g_thm_mgr_cntx.theme_file_handle, buff, THEME_IMAGE_INFO_HEADER, (UINT*)&size);

	//offset in next 4 bytes
	buff[8] = (U8) (image_offset & 0x000000ff);
	buff[9] = (U8) ((image_offset >> 8) & 0x000000ff);
	buff[10] = (U8) ((image_offset >> 16) & 0x000000ff);
	buff[11] = (U8) ((image_offset >> 24) & 0x000000ff);

	//file handle in next 4 bytes
	buff[12] = (U8) (g_thm_mgr_cntx.theme_file_handle & 0x000000ff);
	buff[13] = (U8) ((g_thm_mgr_cntx.theme_file_handle >> 8) & 0x000000ff);
	buff[14] = (U8) ((g_thm_mgr_cntx.theme_file_handle >> 16) & 0x000000ff);
	buff[15] = (U8) ((g_thm_mgr_cntx.theme_file_handle >> 24) & 0x000000ff);

	DRM_seek_file(g_thm_mgr_cntx.theme_file_handle, fp, FS_FILE_BEGIN); 
	
	return (UI_image_type)tm_add_image_header_to_pool(&(g_thm_mgr_cntx.non_imageid_images_headers), buff);
}

void tm_handle_audio(U32 audio_offset, U32 audio_size)
{
	S8 audioName[MAX_IMAGE_NAME_PATH_WIDTH+MAX_AUDIO_NAME_WIDTH];

	if(gaudiothmfileformat==THEME_AUDIO)
	{
	AnsiiToUnicodeString((S8*)gDwnlThmToneInfo[0].AudioName, THEME_AUDIO_FILE);//currently only one audio is suported. Will be changed
	}
	else if(gaudiothmfileformat==THEME_MIDI)
	{
	AnsiiToUnicodeString((S8*)gDwnlThmToneInfo[0].AudioName, THEME_AUDIO_MID_FILE);
	}
	mmi_nsm_create_dir("C:\\Audio");
//	AnsiiToUnicodeString((S8*)gDwnlThmToneInfo[0].AudioName, THEME_AUDIO_FILE);//currently only one audio is suported. Will be changed
	AnsiiToUnicodeString((S8*)audioName, THEME_AUDIO_FILE_PATH);//currently only one audio is suported. Will be changed

	pfnUnicodeStrcat((PS8)audioName, (PS8)gDwnlThmToneInfo[0].AudioName);
	if(pCopyFileFromOffset(g_thm_mgr_cntx.theme_file_handle, (PU8)audioName, audio_offset, audio_size) == FS_NO_ERROR)
		g_thm_mgr_cntx.theme_has_ringtone = TRUE;

	gDwnlThmToneInfo[0].AudioId = CURRENT_THEME_INCOMING_CALL_TONE;
}
#endif  //ritesh

extern S8 nPrintableStr[];
U8 tm_get_themes(theme_list_struct** theme_list)
{
	U8  dwnl_ph_thm_cnt = 0;
	U8  dwnl_card_thm_cnt = 0;
	U8	nvm_ph_thm_cnt = 0;
	U8	nvm_card_thm_cnt = 0;
	theme_list_struct* tmp_theme_list;
	U8 thm_dirty_bits[TOTAL_THEMES_COUNT] = {0};
	U8 tmp_cnt;

	U8 i, j, k;
	S16 err;
	U32 max_theme_id;
	U64 buff;

	gpPhThmNamePtr=(PS8)&nPrintableStr;	

	memset((void*)g_theme_list, 0, sizeof(theme_list_struct)*TOTAL_THEMES_COUNT);
	g_thm_mgr_cntx.total_thm_cnt = 0;
	g_thm_mgr_cntx.sys_thm_cnt = 0;


	//Get System Themes
	for(i = 0; i < MAX_SYSTEM_THEMES; ++i) 
	{
		pfnUnicodeStrncpy((PS8)g_theme_list[i].theme_name, GetString((U16)(STR_THEME_DEFAULT + i)), MAX_THEME_NAME_LENGTH);
		g_theme_list[i].theme_checksum = 0xFFFF;
		g_theme_list[i].theme_checksum_verified = CHECKSUM_VERIFIED_OK;
		g_theme_list[i].theme_id = i+1;
		g_theme_list[i].theme_type = THEME_TYPE_SYSTEM;
	}
	g_thm_mgr_cntx.total_thm_cnt = g_thm_mgr_cntx.sys_thm_cnt = MAX_SYSTEM_THEMES;
#ifndef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
#ifdef __MMI_THEMES_V2_SUPPORT__
		*theme_list = g_theme_list;
		return g_thm_mgr_cntx.total_thm_cnt;
#endif
#endif
	//Read Themes from NVRAM
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh
	ReadRecord(NVRAM_EF_THEME_MANAGER_LID, 1, (void*)(g_theme_list+g_thm_mgr_cntx.sys_thm_cnt), NVRAM_EF_THEME_MANAGER_RECORD_SIZE, &err);
	err=1;
	if(err != NVRAM_READ_SUCCESS)
	{
		*theme_list = g_theme_list;
		return g_thm_mgr_cntx.total_thm_cnt;
	}
	
	//Read actual Downloaded Phone themes
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	dwnl_ph_thm_cnt = tm_get_downloaded_themes(THEME_TYPE_DOWNLOADED_PHONE);
	//Read actual Downloaded Card themes
	dwnl_card_thm_cnt = tm_get_downloaded_themes(THEME_TYPE_DOWNLOADED_CARD);
#endif	
	//Check which of the phone themes in NVRAM list are actually on Phone 
	tmp_cnt = dwnl_ph_thm_cnt;
	for(j = g_thm_mgr_cntx.sys_thm_cnt; j < TOTAL_THEMES_COUNT; ++j)
	{	U8 found = 0;
		if(g_theme_list[j].theme_type == THEME_TYPE_DOWNLOADED_PHONE)
		{
			for(k = 0; (k < tmp_cnt) && !found; ++k)
			{
				if(!pfnUnicodeStrcmp((PS8)g_theme_list[j].theme_name, (PS8)gpPhThmNamePtr+(k*MAX_THEME_NAME_LENGTH*ENCODING_LENGTH)))
				{
					thm_dirty_bits[j] = TRUE;
					memset(gpPhThmNamePtr+(k*MAX_THEME_NAME_LENGTH*ENCODING_LENGTH), 0, MAX_THEME_NAME_LENGTH*ENCODING_LENGTH);
					nvm_ph_thm_cnt++;
					dwnl_ph_thm_cnt--;
					found = 1;
				}
			}
		}
	}
	
	//Check which of the card themes in NVRAM list are actually on card 
	tmp_cnt = dwnl_card_thm_cnt;
	for(j = g_thm_mgr_cntx.sys_thm_cnt; j < TOTAL_THEMES_COUNT; ++j)
	{	U8 found = 0;
		if(g_theme_list[j].theme_type == THEME_TYPE_DOWNLOADED_CARD)
		{
			for(k = 0; (k < tmp_cnt) && !found; ++k)
			{
				if(!pfnUnicodeStrcmp((PS8)g_theme_list[j].theme_name, (PS8)gCardThmNameArr+(k*MAX_THEME_NAME_LENGTH*ENCODING_LENGTH)))
				{
					thm_dirty_bits[j] = TRUE;
					memset(gCardThmNameArr+(k*MAX_THEME_NAME_LENGTH*ENCODING_LENGTH), 0, MAX_THEME_NAME_LENGTH*ENCODING_LENGTH);
					nvm_card_thm_cnt++;
					dwnl_card_thm_cnt--;
					found = 1;
				}
			}
		}
	}
	

	ReadValue(THEME_MANAGER_MAX_THEME_ID, (void*)&buff,DS_DOUBLE, &err);
	max_theme_id = (U32)buff;
	if(max_theme_id == 0xFFFFFFFF || err == -1) 
	{
		max_theme_id = 10;
		WriteValue(THEME_MANAGER_MAX_THEME_ID, (void*)&max_theme_id,DS_DOUBLE, &err);
	}

	tmp_theme_list = (theme_list_struct*) &gNvramThmArr ;

	//Get all themes from NVRAM list, which are actually present on Phone, in a new theme list 
	for(j = g_thm_mgr_cntx.sys_thm_cnt, k = 0; j < TOTAL_THEMES_COUNT; ++j)
	{
		if(thm_dirty_bits[j] && (g_theme_list[j].theme_type == THEME_TYPE_DOWNLOADED_PHONE))
		{
			memcpy((void*)(tmp_theme_list+k), (void*)&g_theme_list[j], sizeof(theme_list_struct));
			++k;	
		}
	}

	//Insert new Phone themes in NVRAM list
	i = j = 0;
	while(i < dwnl_ph_thm_cnt && k < (MAX_DOWNLOADED_THEMES - nvm_card_thm_cnt))
	{
		if(pfnUnicodeStrlen((PS8)gpPhThmNamePtr+(j*MAX_THEME_NAME_LENGTH*ENCODING_LENGTH))){
			memset((void*)&tmp_theme_list[k], 0, sizeof(theme_list_struct));
			pfnUnicodeStrncpy((PS8)tmp_theme_list[k].theme_name, (PS8)gpPhThmNamePtr+(j*MAX_THEME_NAME_LENGTH*ENCODING_LENGTH), MAX_THEME_NAME_LENGTH);
			tmp_theme_list[k].theme_checksum_verified = CHECKSUM_NOT_VERIFIED;
			tmp_theme_list[k].theme_checksum = 0xFFFF;
			tmp_theme_list[k].theme_id = ++max_theme_id;
			tmp_theme_list[k].theme_type = THEME_TYPE_DOWNLOADED_PHONE;
			++i;
			++k;
		}
		++j;
	}

	//Get all themes from NVRAM list, which are actually present on card, in a new theme list 
	for(j = g_thm_mgr_cntx.sys_thm_cnt; j < TOTAL_THEMES_COUNT; ++j)
	{
		if(thm_dirty_bits[j] && (g_theme_list[j].theme_type == THEME_TYPE_DOWNLOADED_CARD))
		{
			memcpy((void*)(tmp_theme_list+k), (void*)&g_theme_list[j], sizeof(theme_list_struct));
			++k;	
		}
	}

	//Insert new Card themes in NVRAM list
	i = j = 0;
	while(i < dwnl_card_thm_cnt && k < MAX_DOWNLOADED_THEMES)
	{
		if(pfnUnicodeStrlen((PS8)gCardThmNameArr+(j*MAX_THEME_NAME_LENGTH*ENCODING_LENGTH))){
			memset((void*)&tmp_theme_list[k], 0, sizeof(theme_list_struct));
			pfnUnicodeStrncpy((PS8)tmp_theme_list[k].theme_name, (PS8)gCardThmNameArr+(j*MAX_THEME_NAME_LENGTH*ENCODING_LENGTH), MAX_THEME_NAME_LENGTH);
			tmp_theme_list[k].theme_checksum_verified = CHECKSUM_NOT_VERIFIED;
			tmp_theme_list[k].theme_checksum = 0xFFFF;
			tmp_theme_list[k].theme_id = ++max_theme_id;
			tmp_theme_list[k].theme_type = THEME_TYPE_DOWNLOADED_CARD;
			++i;
			++k;
		}
		++j;
	}
	
	g_thm_mgr_cntx.total_thm_cnt= g_thm_mgr_cntx.sys_thm_cnt + k;

	memset((void*)(g_theme_list+g_thm_mgr_cntx.sys_thm_cnt), 0, sizeof(theme_list_struct)*MAX_DOWNLOADED_THEMES);
	memcpy((void*)(g_theme_list+g_thm_mgr_cntx.sys_thm_cnt), tmp_theme_list, sizeof(theme_list_struct)*k);

	//Write Themes back to NVRAM
	WriteRecord(NVRAM_EF_THEME_MANAGER_LID, 1, (void*)(g_theme_list+g_thm_mgr_cntx.sys_thm_cnt), NVRAM_EF_THEME_MANAGER_RECORD_SIZE, &err);
	WriteValue(THEME_MANAGER_MAX_THEME_ID, (void*)&max_theme_id,DS_DOUBLE, &err);
	
	*theme_list = g_theme_list;
	return g_thm_mgr_cntx.total_thm_cnt;
#endif //ritesh
}
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
U8 tm_get_downloaded_themes(theme_type_enum theme_type)
{
	FS_HANDLE fh_cur;
	FS_DOSDirEntry file_info;
	S8 path[MAX_DWNL_THM_PATH_LEN+MAX_THEME_SEARCH_STR_LEN];
	UI_character_type filename[MAX_THEME_NAME_LENGTH];
	S8 tmp_str[30];
	U8 count = 0, j;
	S8* themeNamePtr;

	if(theme_type == THEME_TYPE_DOWNLOADED_PHONE)
	{
		sprintf(tmp_str,"%c%s%s", MMI_PUBLIC_DRV, DOWNLOADED_THEMES_PATH, THEME_SEARCH_STR);
		j = 0;
		themeNamePtr=gpPhThmNamePtr;
	}
	else if(theme_type == THEME_TYPE_DOWNLOADED_CARD)
	{
		sprintf(tmp_str,"%c%s%s", MMI_CARD_DRV, DOWNLOADED_THEMES_PATH, THEME_SEARCH_STR);
		j = MAX_DOWNLOADED_THEMES;
		themeNamePtr=(PS8)gCardThmNameArr;
	}
	else 
		return 0;


	AnsiiToUnicodeString((PS8)path, tmp_str);

	if( (fh_cur = FS_FindFirst((U16*)path, 0, 0, &file_info, (U16*)filename, MAX_THEME_NAME_LENGTH*ENCODING_LENGTH)) >= 0 )
	{
		count = 0;
		do
		{
			/* filter out folder results*/
			if( !(file_info.Attributes & FS_ATTR_DIR) )
			{
				pfnUnicodeStrcpy(themeNamePtr+count*MAX_THEME_NAME_LENGTH*ENCODING_LENGTH, (S8*)filename);
				count++;
			}
		} while((FS_FindNext( fh_cur, &file_info, (U16*)filename, MAX_THEME_NAME_LENGTH*ENCODING_LENGTH) == FS_NO_ERROR) && (count < MAX_DOWNLOADED_THEMES));
		pffindclose( fh_cur );
	}

	return count;
}
#endif
extern BOOL PhnsetIsWPCurrentTheme(void);
extern BOOL PhnsetIsScrSvrCurrentTheme(void);
extern void ThemeManagerSetWallpaper(U16);
extern void ThemeManagerSetScreensaver(U16);

void tm_activate_theme_at_bootup(void)
{
	U8 i = 0, found = 0, result;
	U16 CurtWpID=0;
	S16 error;
	U32 curr_theme_id = tm_get_current_theme_id();

	for(i = 0; i < g_thm_mgr_cntx.total_thm_cnt; ++i) 
	{
		if(curr_theme_id == g_theme_list[i].theme_id) 
		{
			found = 1;
			break;
		}
	}

	if(found){
		if(g_theme_list[i].theme_type == THEME_TYPE_SYSTEM)
			result = tm_activate_system_theme(&g_theme_list[i]);
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh
		else
			result = tm_activate_downloaded_theme(&g_theme_list[i]);
#endif
		if(result != THEME_ERROR_SUCCESS)
		{
			tm_activate_system_theme(&g_theme_list[0]);
			if(PhnsetIsWPCurrentTheme())
				ThemeManagerSetWallpaper(WALLPAPER_1);
			if(PhnsetIsScrSvrCurrentTheme())
				ThemeManagerSetScreensaver(DIGITAL_CLOCK);
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh
			ThemeManagerResetRingTone();
			ThemeManagerResetAlarmTone();
#endif //ritesh
		}
		else
		{
			if(g_thm_mgr_cntx.theme_has_wallpaper)
			{
				ReadValue( NVRAM_FUNANDGAMES_SETWALLPAPER, &CurtWpID, DS_SHORT, &error);
				if(CURRENT_THEME_WALLPAPER==CurtWpID) //Ritesh :added for wallpaper set by user other than theme wallpaper and at bootup theme wallpaper gets activated
				{
				   ThemeManagerSetWallpaper(CURRENT_THEME_WALLPAPER);
				}
				else
				{
					ThemeManagerSetWallpaper(CurtWpID);
				}
			}
			else
			{
				if(PhnsetIsWPCurrentTheme())
					ThemeManagerSetWallpaper(WALLPAPER_1);
			}

			if(g_thm_mgr_cntx.theme_has_screensaver)
				ThemeManagerSetScreensaver(CURRENT_THEME_SCREENSAVER);
			else
			{
				if(PhnsetIsScrSvrCurrentTheme())
					ThemeManagerSetScreensaver(DIGITAL_CLOCK);
			}
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh
			if(g_thm_mgr_cntx.theme_has_ringtone)
			{
				ThemeManagerSetRingTone(CURRENT_THEME_INCOMING_CALL_TONE);
				ThemeManagerSetAlarmTone(CURRENT_THEME_ALARM_EXPIRY_TONE);
			}
			else
			{
				ThemeManagerResetRingTone();
				ThemeManagerResetAlarmTone();
			}
#endif //ritesh
		}

	}
	else
	{
		tm_activate_system_theme(&g_theme_list[0]);
		if(PhnsetIsWPCurrentTheme())
			ThemeManagerSetWallpaper(WALLPAPER_1);
		if(PhnsetIsScrSvrCurrentTheme())
			ThemeManagerSetScreensaver(DIGITAL_CLOCK);
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__  //ritesh
		ThemeManagerResetRingTone();
		ThemeManagerResetAlarmTone();
#endif //ritesh
	}

	if(tm_does_theme_have_alt_image())
		initialize_status_icons();
}

theme_error_enum tm_activate_theme(THEME_ID theme_id)
{
	U8 i;
	theme_error_enum result;
//	ShowCategory66Screen(STR_SCR3001_THEMES_CAPTION,0, 0, 0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, (PU8)GetString(STR_ID_DLT_THM_PROGRESS), IMG_THEME_ACTIVATION, NULL);

	if(g_thm_mgr_cntx.curr_activated_theme_id == theme_id)
		return THEME_ERROR_ALREADY_ACTIVATED;

	for(i = 0; i < g_thm_mgr_cntx.total_thm_cnt; ++i)
		if(theme_id == g_theme_list[i].theme_id)
			break;

	if(g_theme_list[i].theme_type == THEME_TYPE_SYSTEM)
		result = tm_activate_system_theme(&g_theme_list[i]);
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh
	else
		result = tm_activate_downloaded_theme(&g_theme_list[i]);
#endif //ritesh

	if(result == THEME_ERROR_SUCCESS)
	{
		if(g_thm_mgr_cntx.theme_has_wallpaper)
			ThemeManagerSetWallpaper(CURRENT_THEME_WALLPAPER);
		else
		{
			if(PhnsetIsWPCurrentTheme())
				ThemeManagerSetWallpaper(WALLPAPER_1);
		}

		if(g_thm_mgr_cntx.theme_has_screensaver)
			ThemeManagerSetScreensaver(CURRENT_THEME_SCREENSAVER);
		else
		{
			if(PhnsetIsScrSvrCurrentTheme())
				ThemeManagerSetScreensaver(DIGITAL_CLOCK);
		}
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__  //ritesh
		if(g_thm_mgr_cntx.theme_has_ringtone)
		{
			ThemeManagerSetRingTone(CURRENT_THEME_INCOMING_CALL_TONE);
			ThemeManagerSetAlarmTone(CURRENT_THEME_ALARM_EXPIRY_TONE);
		}
		else
		{
			ThemeManagerResetRingTone();
			ThemeManagerResetAlarmTone();
		}
#endif //ritesh
//		if(tm_does_theme_have_alt_image())
			initialize_status_icons();
	}

	return result;
}
#ifdef __MMI_THEMES_V2_SUPPORT__
//extern theme_details_struct **MMI_theme_details; //ritesh 
extern theme_details_struct *MMI_themes; //ritesh 
#else
extern MMI_theme **MMI_themes;
#endif
theme_error_enum tm_activate_system_theme(theme_list_struct* theme_struct)
{
	S16 err;

	//todo: To copy from MCT
	g_thm_mgr_cntx.curr_activated_theme_type = theme_struct->theme_type;
	g_thm_mgr_cntx.theme_has_alternate_image = FALSE;
	g_activated_theme.system_image_list = NULL;
	g_activated_theme.theme = NULL;
	g_activated_theme.theme_image_details_list = NULL;
	g_thm_mgr_cntx.theme_has_wallpaper = FALSE;
	g_thm_mgr_cntx.theme_has_screensaver = FALSE;
	g_thm_mgr_cntx.theme_has_ringtone = FALSE;
#ifdef __MMI_THEMES_V2_SUPPORT__ //ritesh
	set_MMI_theme((MMI_theme *)MMI_themes[(theme_struct->theme_id - 1)].theme);
#else
	set_MMI_theme((MMI_theme *)MMI_themes[(theme_struct->theme_id - 1)]);
#endif//ritesh
	MMI_apply_current_theme();

	DRM_close_file(g_thm_mgr_cntx.theme_file_handle);
	g_thm_mgr_cntx.theme_file_handle = 0;

	g_thm_mgr_cntx.curr_activated_theme_id = theme_struct->theme_id;
	g_thm_mgr_cntx.curr_activated_theme_type = theme_struct->theme_type;
	WriteValue(THEME_MANAGER_CURR_THEME_ID, (void*)&g_thm_mgr_cntx.curr_activated_theme_id, DS_DOUBLE, &err);
	return THEME_ERROR_SUCCESS;
}
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh
theme_error_enum tm_activate_downloaded_theme(theme_list_struct* theme_struct)
{
	theme_error_enum result;
	S16 err;
	U8 theme_file_name[100]={0};
	U8 tmp_path[50]={0};
	FS_HANDLE last_theme_file_handle;

	if(theme_struct->theme_type == THEME_TYPE_DOWNLOADED_PHONE)
	{
		sprintf((PS8)tmp_path, "%c%s", MMI_PUBLIC_DRV, DOWNLOADED_THEMES_PATH);
		AnsiiToUnicodeString((PS8)theme_file_name, (PS8)tmp_path);  
	}
	else
	{
		sprintf((PS8)tmp_path, "%c%s", MMI_CARD_DRV, DOWNLOADED_THEMES_PATH);
		AnsiiToUnicodeString((PS8)theme_file_name, (PS8)tmp_path);  
	}
	pfnUnicodeStrcat((PS8)theme_file_name, (PS8)theme_struct->theme_name);

	last_theme_file_handle = g_thm_mgr_cntx.theme_file_handle;
	g_thm_mgr_cntx.theme_file_handle = DRM_open_file((U16*)theme_file_name, FS_READ_ONLY, 0/*DRM_PERMISSION_NONE*/); 
	if(g_thm_mgr_cntx.theme_file_handle < FS_NO_ERROR)
	{
		g_thm_mgr_cntx.theme_file_handle = last_theme_file_handle;
		return THEME_ERROR_THEME_NOT_AVAILABLE;
	}

	
	if((theme_struct->theme_checksum_verified == CHECKSUM_NOT_VERIFIED) || (theme_struct->theme_checksum_verified == CHECKSUM_VERIFIED_BAD))
	{
		result = tm_verify_theme_checksum(theme_file_name);

		if(result == THEME_ERROR_SUCCESS)
		{
			theme_struct->theme_checksum_verified = CHECKSUM_VERIFIED_OK;
			WriteRecord(NVRAM_EF_THEME_MANAGER_LID, 1, (void*)(g_theme_list+g_thm_mgr_cntx.sys_thm_cnt), NVRAM_EF_THEME_MANAGER_RECORD_SIZE, &err);
		}
		else 
		{
			theme_struct->theme_checksum_verified = CHECKSUM_VERIFIED_BAD;
			WriteRecord(NVRAM_EF_THEME_MANAGER_LID, 1, (void*)(g_theme_list+g_thm_mgr_cntx.sys_thm_cnt), NVRAM_EF_THEME_MANAGER_RECORD_SIZE, &err);
			DRM_close_file(g_thm_mgr_cntx.theme_file_handle);
			g_thm_mgr_cntx.theme_file_handle = last_theme_file_handle;
			return THEME_ERROR_FILE_FORMAT_BAD;
		}
	}

	memset(&gdeviceInfo,0,sizeof(deviceinfo));
	memset(&gthemeInfo,0,sizeof(theme_download_info_struct));


	if(theme_version_and_deviceinfo_check(theme_struct)!=TRUE)
	{
		DRM_close_file(g_thm_mgr_cntx.theme_file_handle);
		g_thm_mgr_cntx.theme_file_handle = last_theme_file_handle;
		return THEME_ERROR_VERSION_FAILURE;
	}

	DRM_close_file(last_theme_file_handle);

	g_thm_mgr_cntx.theme_has_alternate_image = FALSE;
	g_activated_theme.system_image_list = NULL;
	g_activated_theme.theme_image_details_list = g_thm_mgr_cntx.theme_image_list;


	tm_reset_imageid_image_header_pool();
	tm_reset_non_imageid_image_header_pool();
	g_thm_mgr_cntx.theme_imageid_image_count = 0;
	memset(g_theme_image_dirty_bits, 0, sizeof(g_theme_image_dirty_bits));
	g_thm_mgr_cntx.theme_has_wallpaper = FALSE;
	g_thm_mgr_cntx.theme_has_screensaver = FALSE;
	g_thm_mgr_cntx.theme_has_ringtone = FALSE;

	if(!convert_theme_file_to_mmi_theme(&(g_activated_theme.theme), g_thm_mgr_cntx.theme_file_handle))
		return THEME_ERROR_FAILURE;
	tm_sort_theme_image_table();

	set_MMI_theme(g_activated_theme.theme);
	MMI_apply_current_theme();


	g_thm_mgr_cntx.curr_activated_theme_id = theme_struct->theme_id;
	g_thm_mgr_cntx.curr_activated_theme_type = theme_struct->theme_type;
	WriteValue(THEME_MANAGER_CURR_THEME_ID, (void*)&g_thm_mgr_cntx.curr_activated_theme_id, DS_DOUBLE, &err);

	return THEME_ERROR_SUCCESS;
}
#endif
void tm_sort_theme_image_table(void) 
{
	if(g_thm_mgr_cntx.theme_imageid_image_count) 
	{	
		tm_image_table_quick_sort(0, (U16)(g_thm_mgr_cntx.theme_imageid_image_count-1));
		tm_image_table_insertion_sort(0, (U16)(g_thm_mgr_cntx.theme_imageid_image_count-1));
	}
}


void tm_image_table_quick_sort(U16 l, U16 r)
{
	U16 i, j; 
	U32 pivot;
    
	if ((r - l) > 4) 
	{
		i = (r + l)/2;
		if(g_activated_theme.theme_image_details_list[l].image_id > g_activated_theme.theme_image_details_list[i].image_id)
			tm_image_table_swap_node(l, i);
		if(g_activated_theme.theme_image_details_list[l].image_id > g_activated_theme.theme_image_details_list[r].image_id)
			tm_image_table_swap_node(l, r);
		if(g_activated_theme.theme_image_details_list[i].image_id > g_activated_theme.theme_image_details_list[r].image_id)
			tm_image_table_swap_node(i, r);
    
		j = r - 1;
		tm_image_table_swap_node(i, j);
		i = l;
		pivot = g_activated_theme.theme_image_details_list[j].image_id;
		
		for(;;) 
		{
			do{} while(g_activated_theme.theme_image_details_list[++i].image_id < pivot);
			do{} while(g_activated_theme.theme_image_details_list[--j].image_id > pivot);
      
			if(j < i)
				break;
			
			tm_image_table_swap_node(i, j);
		}

		tm_image_table_swap_node(i, (U16)(r - 1));
		tm_image_table_quick_sort(l, j);
		tm_image_table_quick_sort((U16)(i + 1), r);
	}
}

void tm_image_table_insertion_sort(U16 lo, U16 hi) 
{
	U16 i, j;
	theme_image_detail_struct elem;
  
	for(i = lo + 1; i <= hi; ++i)
	{
		memcpy(&elem, &g_activated_theme.theme_image_details_list[i], sizeof(theme_image_detail_struct));

		j = i;
		while(j > lo) 
		{
			if(g_activated_theme.theme_image_details_list[j-1].image_id <= elem.image_id)
				break;
			memcpy(&g_activated_theme.theme_image_details_list[j], &g_activated_theme.theme_image_details_list[j-1], sizeof(theme_image_detail_struct));
			j--;
		}
		memcpy(&g_activated_theme.theme_image_details_list[j], &elem, sizeof(theme_image_detail_struct));
	}
}

void tm_image_table_swap_node(U16 i, U16 j) 
{
	theme_image_detail_struct temp;

	memcpy(&temp, &g_activated_theme.theme_image_details_list[i], sizeof(theme_image_detail_struct)); 
	memcpy(&g_activated_theme.theme_image_details_list[i], &g_activated_theme.theme_image_details_list[j], sizeof(theme_image_detail_struct));
	memcpy(&g_activated_theme.theme_image_details_list[j], &temp, sizeof(theme_image_detail_struct));
}

U32	tm_get_current_theme_id(void)
{
	S16 err;
	U64 buff;

	ReadValue(THEME_MANAGER_CURR_THEME_ID, (void*)&buff,DS_DOUBLE, &err);
	g_thm_mgr_cntx.curr_activated_theme_id = (U32)buff;
	if(g_thm_mgr_cntx.curr_activated_theme_id == 0xFFFFFFFF || err == -1) 
	{
		g_thm_mgr_cntx.curr_activated_theme_id = 1;
		WriteValue(THEME_MANAGER_CURR_THEME_ID, (void*)&g_thm_mgr_cntx.curr_activated_theme_id, DS_DOUBLE, &err);
	}

	return g_thm_mgr_cntx.curr_activated_theme_id;
}

U8	tm_does_theme_have_alt_image(void)
{
	return g_thm_mgr_cntx.theme_has_alternate_image;
}

U8 tm_is_theme_dirty_bit_set(U16 image_index)
{
	U16 n_byte;
	U16 n_bit;

	n_byte = (image_index>>3);
	n_bit = (image_index%8);

	return (U8)((g_theme_image_dirty_bits[n_byte]) & (0x80 >> n_bit));
}


U32	tm_get_theme_image_offset_or_index(U16 image_id, S32* nArrayIndex)
{
	S32	nFirst = 0;
	S32 nLast = g_thm_mgr_cntx.theme_imageid_image_count;
	S32 nMid;

	while(nLast>=nFirst)
	{
		nMid = (nFirst+nLast)/2;;
		if(g_activated_theme.theme_image_details_list[nMid].image_id == image_id)
		{
			* nArrayIndex = nMid;
			return g_activated_theme.theme_image_details_list[nMid].image_offset;
		}
		if(image_id > g_activated_theme.theme_image_details_list[nMid].image_id)
			nFirst = nMid+1;
		else
			nLast = nMid-1;
	}

	return 0;

}

U8 tm_get_curr_theme_type(void)
{
	return g_thm_mgr_cntx.curr_activated_theme_type;
}

U8	tm_get_total_themes_count(void)
{
	return g_thm_mgr_cntx.total_thm_cnt;
}
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__  //ritesh
theme_error_enum tm_activate_theme_from_fmgr(theme_type_enum theme_type, U8* theme_name)
{
	U8 i;
	U8 found = 0;
	S16 err;
	U32 max_theme_id;
	theme_error_enum result;
// added by Deepali
	FS_HANDLE fh_cur;
//	FS_DOSDirEntry file_info;
	S8 path[(FMGR_MAX_FILE_LEN+15)*ENCODING_LENGTH];
	UI_character_type filename[MAX_THEME_NAME_LENGTH];
	S8 tmp_str[15];
	S8 tmp_str2[10];
	FS_FileInfo file_info; 
	theme_list_struct *theme_list;


	if(pfnUnicodeStrlen((PS8)theme_name) > MAX_THEME_NAME_LENGTH)
	{
		if(theme_type == THEME_TYPE_DOWNLOADED_PHONE)
		{
			sprintf(tmp_str,"%c%s", MMI_PUBLIC_DRV, DOWNLOADED_THEMES_PATH);
		}
		else if(theme_type == THEME_TYPE_DOWNLOADED_CARD)
		{
			sprintf(tmp_str,"%c%s", MMI_CARD_DRV, DOWNLOADED_THEMES_PATH);
		}
		AnsiiToUnicodeString((PS8)path, tmp_str);
		pfnUnicodeStrncat((PS8)path,(PS8)theme_name,FMGR_MAX_FILE_LEN);
		fh_cur = pfopen((const WCHAR *)path, FS_READ_ONLY, 0);        /* file path is the one with long file name e.g. L"C:\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.jpg" */ 
		FS_GetFileInfo (fh_cur, &file_info); 
			
		memset((PS8)filename,0,MAX_THEME_NAME_LENGTH*ENCODING_LENGTH);
	// file name
		memset((PS8)tmp_str,0,15);
		memcpy((PS8)tmp_str,(PS8)file_info.DirEntry->FileName,8);// truncated file name length is 8.
		AnsiiToUnicodeString((PS8)filename,(PS8)tmp_str);
	//.
		AnsiiToUnicodeString((PS8)tmp_str,(PS8)".");
		pfnUnicodeStrcat((PS8)filename,(PS8)tmp_str);
	// Extension
		memset((PS8)tmp_str2,0,10);
		memcpy((PS8)tmp_str2,(PS8)file_info.DirEntry->Extension,3);// truncated file name length is 8.
		AnsiiToUnicodeString((PS8)tmp_str,(PS8)tmp_str2);
		pfnUnicodeStrcat((PS8)filename,(PS8)tmp_str);
	
		pfclose (fh_cur);
	}
	else
	{
		pfnUnicodeStrcpy((PS8)filename,(PS8)theme_name);
	}


	tm_get_themes(&theme_list);// to sync theme list.

	for(i = g_thm_mgr_cntx.sys_thm_cnt; i < g_thm_mgr_cntx.total_thm_cnt; ++i)
	{
		if((pfnUnicodeStrcmp((PS8)g_theme_list[i].theme_name, (PS8)filename)==0) && (g_theme_list[i].theme_type == theme_type)) 
		{
			found = 1;
			break;
		}
	}
	 
	if(found==0) 
	{
		if (g_thm_mgr_cntx.total_thm_cnt >= TOTAL_THEMES_COUNT)
			return THEME_ERROR_MAX_COUNT_REACHED;
		else // add new theme in the list and give a new id
		{
			U64 buff;
			ReadValue(THEME_MANAGER_MAX_THEME_ID, (void*)&buff, DS_DOUBLE, &err);
			max_theme_id = (U32)buff;
			if(max_theme_id == 0xFFFFFFFF || err == -1) 
			{
				max_theme_id = 10;
				WriteValue(THEME_MANAGER_MAX_THEME_ID, (void*)&max_theme_id,DS_DOUBLE, &err);
			}

			if(theme_type == THEME_TYPE_DOWNLOADED_PHONE)
			{
				for(i = g_thm_mgr_cntx.total_thm_cnt; g_theme_list[i].theme_type == THEME_TYPE_DOWNLOADED_CARD; --i)
				{
					memcpy((void*)&g_theme_list[i], (void*)&g_theme_list[i-1], sizeof(theme_list_struct));
				}
			}
			else if(theme_type == THEME_TYPE_DOWNLOADED_CARD) 
			{
				i = g_thm_mgr_cntx.total_thm_cnt;
			}
			memset((void*)&g_theme_list[i], 0, sizeof(theme_list_struct));
			pfnUnicodeStrncpy((PS8)g_theme_list[i].theme_name, (PS8)theme_name, MAX_THEME_NAME_LENGTH);
			g_theme_list[i].theme_checksum = 0xFFFF;
			g_theme_list[i].theme_checksum_verified = CHECKSUM_NOT_VERIFIED;
			g_theme_list[i].theme_type = theme_type;
			g_theme_list[i].theme_id = ++max_theme_id;
			++g_thm_mgr_cntx.total_thm_cnt;
			WriteValue(THEME_MANAGER_MAX_THEME_ID, (void*)&max_theme_id,DS_DOUBLE, &err);
			WriteRecord(NVRAM_EF_THEME_MANAGER_LID, 1, (void*)(g_theme_list+g_thm_mgr_cntx.sys_thm_cnt), NVRAM_EF_THEME_MANAGER_RECORD_SIZE, &err);
			result = tm_activate_downloaded_theme(&g_theme_list[i]);
		}
	}
	else
	{
		if(g_thm_mgr_cntx.curr_activated_theme_id == g_theme_list[i].theme_id)
			return THEME_ERROR_ALREADY_ACTIVATED;
		else
			result = tm_activate_downloaded_theme(&g_theme_list[i]);
	}

	if(result == THEME_ERROR_SUCCESS)
	{
		if(g_thm_mgr_cntx.theme_has_wallpaper)
			ThemeManagerSetWallpaper(CURRENT_THEME_WALLPAPER);
		else
		{
			if(PhnsetIsWPCurrentTheme())
				ThemeManagerSetWallpaper(WALLPAPER_1);
		}

		if(g_thm_mgr_cntx.theme_has_screensaver)
			ThemeManagerSetScreensaver(CURRENT_THEME_SCREENSAVER);
		else
		{
			if(PhnsetIsScrSvrCurrentTheme())
				ThemeManagerSetScreensaver(DIGITAL_CLOCK);
		}

		if(g_thm_mgr_cntx.theme_has_ringtone)
		{
			ThemeManagerSetRingTone(CURRENT_THEME_INCOMING_CALL_TONE);
			ThemeManagerSetAlarmTone(CURRENT_THEME_ALARM_EXPIRY_TONE);
		}
		else
		{
			ThemeManagerResetRingTone();
			ThemeManagerResetAlarmTone();
		}
		if(tm_does_theme_have_alt_image())
			initialize_status_icons();

	}

	return result;
}

theme_error_enum tm_delete_theme(THEME_ID theme_id, PS32 error_type)
{
	U8 i, found = 0;
	S16 err;

	for(i = g_thm_mgr_cntx.sys_thm_cnt; i < g_thm_mgr_cntx.total_thm_cnt; ++i)
	{
		if(g_theme_list[i].theme_id == theme_id)
		{
			found = 1;
			break;
		}	
	}

	if(!found)
		return THEME_ERROR_THEME_NOT_AVAILABLE;
	else
	{
		U8 tmp_file_name[50];
		U8 file_name[100];
		if(g_theme_list[i].theme_type == THEME_TYPE_DOWNLOADED_PHONE)
		{
			sprintf((PS8)tmp_file_name, "%c%s", MMI_PUBLIC_DRV, DOWNLOADED_THEMES_PATH);
			AnsiiToUnicodeString((PS8)file_name, (PS8)tmp_file_name);
		}
		else
		{
			sprintf((PS8)tmp_file_name, "%c%s", MMI_CARD_DRV, DOWNLOADED_THEMES_PATH);
			AnsiiToUnicodeString((PS8)file_name, (PS8)tmp_file_name);
		}

		pfnUnicodeStrcat((PS8)file_name, (PS8)g_theme_list[i].theme_name);

		if((*error_type = MMI_FS_Delete((U16*)file_name)) == FS_NO_ERROR)
		{
			for(; i < g_thm_mgr_cntx.total_thm_cnt-1; ++i)
			{
				memcpy((void*)&g_theme_list[i], (void*)&g_theme_list[i+1], sizeof(theme_list_struct));
			}
			memset((void*)&g_theme_list[g_thm_mgr_cntx.total_thm_cnt-1], 0, sizeof(theme_list_struct));
			WriteRecord(NVRAM_EF_THEME_MANAGER_LID, 1, (void*)(g_theme_list+g_thm_mgr_cntx.sys_thm_cnt), NVRAM_EF_THEME_MANAGER_RECORD_SIZE, &err);
			--g_thm_mgr_cntx.total_thm_cnt;

			return THEME_ERROR_SUCCESS;
		}
		else
			return THEME_ERROR_FAILURE;
	}
}

theme_error_enum tm_delete_theme_from_fmgr(theme_type_enum theme_type, U8* theme_name, PS32 error_type)
{
	U8 i, found = 0;
	U8 tmp_file_name[50];
	U8 file_name[100];
	S16 err;

	for(i = g_thm_mgr_cntx.sys_thm_cnt; i < g_thm_mgr_cntx.total_thm_cnt; ++i)
	{
		if(!pfnUnicodeStrcmp((PS8)g_theme_list[i].theme_name, (PS8)theme_name) && (g_theme_list[i].theme_type == theme_type)) 
		{
			found = 1;
			break;
		}
	}

	if(theme_type == THEME_TYPE_DOWNLOADED_PHONE)
	{
		sprintf((PS8)tmp_file_name, "%c%s", MMI_PUBLIC_DRV, DOWNLOADED_THEMES_PATH);
		AnsiiToUnicodeString((PS8)file_name, (PS8)tmp_file_name);
	}
	else
	{
		sprintf((PS8)tmp_file_name, "%c%s", MMI_CARD_DRV, DOWNLOADED_THEMES_PATH);
		AnsiiToUnicodeString((PS8)file_name, (PS8)tmp_file_name);
	}

	pfnUnicodeStrcat((PS8)file_name, (PS8)theme_name);

	if(found)
	{
		if(g_theme_list[i].theme_id == g_thm_mgr_cntx.curr_activated_theme_id)
			return THEME_ERROR_ALREADY_ACTIVATED;
		if((*error_type = MMI_FS_Delete((U16*)file_name)) == FS_NO_ERROR)
		{
			for(; i < g_thm_mgr_cntx.total_thm_cnt-1; ++i)
			{
				memcpy((void*)&g_theme_list[i], (void*)&g_theme_list[i+1], sizeof(theme_list_struct));
			}
			memset((void*)&g_theme_list[g_thm_mgr_cntx.total_thm_cnt-1], 0, sizeof(theme_list_struct));
			WriteRecord(NVRAM_EF_THEME_MANAGER_LID, 1, (void*)(g_theme_list+g_thm_mgr_cntx.sys_thm_cnt), NVRAM_EF_THEME_MANAGER_RECORD_SIZE, &err);
			--g_thm_mgr_cntx.total_thm_cnt;

			return THEME_ERROR_SUCCESS;
		}
		else
			return THEME_ERROR_DELETE_FAILURE;
	}
	else if((*error_type = MMI_FS_Delete((U16*)file_name)) == FS_NO_ERROR)
		return THEME_ERROR_SUCCESS;
	else 
		return THEME_ERROR_FAILURE;
}
#endif //ritesh

pBOOL tm_get_current_theme_wallapaper(void) 
{
	return g_thm_mgr_cntx.theme_has_wallpaper;
}

pBOOL tm_get_current_theme_screensaver(void)
{
	return g_thm_mgr_cntx.theme_has_screensaver;
}

pBOOL tm_get_current_theme_ringtone(void)
{
	return g_thm_mgr_cntx.theme_has_ringtone;
}

const float32* tm_get_theme_version_table(void)
{
	return gSupportedThemeVersionTable;
}

const U32 CRCTable[256]=
{
	0x00000000L, 0x04C11DB7L, 0x09823B6EL, 0x0D4326D9L, 0x130476DCL,
	0x17C56B6BL, 0x1A864DB2L, 0x1E475005L, 0x2608EDB8L, 0x22C9F00FL,
	0x2F8AD6D6L, 0x2B4BCB61L, 0x350C9B64L, 0x31CD86D3L, 0x3C8EA00AL,
	0x384FBDBDL, 0x4C11DB70L, 0x48D0C6C7L, 0x4593E01EL, 0x4152FDA9L,
	0x5F15ADACL, 0x5BD4B01BL, 0x569796C2L, 0x52568B75L, 0x6A1936C8L,
	0x6ED82B7FL, 0x639B0DA6L, 0x675A1011L, 0x791D4014L, 0x7DDC5DA3L,
	0x709F7B7AL, 0x745E66CDL, 0x9823B6E0L, 0x9CE2AB57L, 0x91A18D8EL,
	0x95609039L, 0x8B27C03CL, 0x8FE6DD8BL, 0x82A5FB52L, 0x8664E6E5L,
	0xBE2B5B58L, 0xBAEA46EFL, 0xB7A96036L, 0xB3687D81L, 0xAD2F2D84L,
	0xA9EE3033L, 0xA4AD16EAL, 0xA06C0B5DL, 0xD4326D90L, 0xD0F37027L,
	0xDDB056FEL, 0xD9714B49L, 0xC7361B4CL, 0xC3F706FBL, 0xCEB42022L,
	0xCA753D95L, 0xF23A8028L, 0xF6FB9D9FL, 0xFBB8BB46L, 0xFF79A6F1L,
	0xE13EF6F4L, 0xE5FFEB43L, 0xE8BCCD9AL, 0xEC7DD02DL, 0x34867077L,
	0x30476DC0L, 0x3D044B19L, 0x39C556AEL, 0x278206ABL, 0x23431B1CL,
	0x2E003DC5L, 0x2AC12072L, 0x128E9DCFL, 0x164F8078L, 0x1B0CA6A1L,
	0x1FCDBB16L, 0x018AEB13L, 0x054BF6A4L, 0x0808D07DL, 0x0CC9CDCAL,
	0x7897AB07L, 0x7C56B6B0L, 0x71159069L, 0x75D48DDEL, 0x6B93DDDBL,
	0x6F52C06CL, 0x6211E6B5L, 0x66D0FB02L, 0x5E9F46BFL, 0x5A5E5B08L,
	0x571D7DD1L, 0x53DC6066L, 0x4D9B3063L, 0x495A2DD4L, 0x44190B0DL,
	0x40D816BAL, 0xACA5C697L, 0xA864DB20L, 0xA527FDF9L, 0xA1E6E04EL,
	0xBFA1B04BL, 0xBB60ADFCL, 0xB6238B25L, 0xB2E29692L, 0x8AAD2B2FL,
	0x8E6C3698L, 0x832F1041L, 0x87EE0DF6L, 0x99A95DF3L, 0x9D684044L,
	0x902B669DL, 0x94EA7B2AL, 0xE0B41DE7L, 0xE4750050L, 0xE9362689L,
	0xEDF73B3EL, 0xF3B06B3BL, 0xF771768CL, 0xFA325055L, 0xFEF34DE2L,
	0xC6BCF05FL, 0xC27DEDE8L, 0xCF3ECB31L, 0xCBFFD686L, 0xD5B88683L,
	0xD1799B34L, 0xDC3ABDEDL, 0xD8FBA05AL, 0x690CE0EEL, 0x6DCDFD59L,
	0x608EDB80L, 0x644FC637L, 0x7A089632L, 0x7EC98B85L, 0x738AAD5CL,
	0x774BB0EBL, 0x4F040D56L, 0x4BC510E1L, 0x46863638L, 0x42472B8FL,
	0x5C007B8AL, 0x58C1663DL, 0x558240E4L, 0x51435D53L, 0x251D3B9EL,
	0x21DC2629L, 0x2C9F00F0L, 0x285E1D47L, 0x36194D42L, 0x32D850F5L,
	0x3F9B762CL, 0x3B5A6B9BL, 0x0315D626L, 0x07D4CB91L, 0x0A97ED48L,
	0x0E56F0FFL, 0x1011A0FAL, 0x14D0BD4DL, 0x19939B94L, 0x1D528623L,
	0xF12F560EL, 0xF5EE4BB9L, 0xF8AD6D60L, 0xFC6C70D7L, 0xE22B20D2L,
	0xE6EA3D65L, 0xEBA91BBCL, 0xEF68060BL, 0xD727BBB6L, 0xD3E6A601L,
	0xDEA580D8L, 0xDA649D6FL, 0xC423CD6AL, 0xC0E2D0DDL, 0xCDA1F604L,
	0xC960EBB3L, 0xBD3E8D7EL, 0xB9FF90C9L, 0xB4BCB610L, 0xB07DABA7L,
	0xAE3AFBA2L, 0xAAFBE615L, 0xA7B8C0CCL, 0xA379DD7BL, 0x9B3660C6L,
	0x9FF77D71L, 0x92B45BA8L, 0x9675461FL, 0x8832161AL, 0x8CF30BADL,
	0x81B02D74L, 0x857130C3L, 0x5D8A9099L, 0x594B8D2EL, 0x5408ABF7L,
	0x50C9B640L, 0x4E8EE645L, 0x4A4FFBF2L, 0x470CDD2BL, 0x43CDC09CL,
	0x7B827D21L, 0x7F436096L, 0x7200464FL, 0x76C15BF8L, 0x68860BFDL,
	0x6C47164AL, 0x61043093L, 0x65C52D24L, 0x119B4BE9L, 0x155A565EL,
	0x18197087L, 0x1CD86D30L, 0x029F3D35L, 0x065E2082L, 0x0B1D065BL,
	0x0FDC1BECL, 0x3793A651L, 0x3352BBE6L, 0x3E119D3FL, 0x3AD08088L,
	0x2497D08DL, 0x2056CD3AL, 0x2D15EBE3L, 0x29D4F654L, 0xC5A92679L,
	0xC1683BCEL, 0xCC2B1D17L, 0xC8EA00A0L, 0xD6AD50A5L, 0xD26C4D12L,
	0xDF2F6BCBL, 0xDBEE767CL, 0xE3A1CBC1L, 0xE760D676L, 0xEA23F0AFL,
	0xEEE2ED18L, 0xF0A5BD1DL, 0xF464A0AAL, 0xF9278673L, 0xFDE69BC4L,
	0x89B8FD09L, 0x8D79E0BEL, 0x803AC667L, 0x84FBDBD0L, 0x9ABC8BD5L,
	0x9E7D9662L, 0x933EB0BBL, 0x97FFAD0CL, 0xAFB010B1L, 0xAB710D06L,
	0xA6322BDFL, 0xA2F33668L, 0xBCB4666DL, 0xB8757BDAL, 0xB5365D03L,
	0xB1F740B4
};

U32 checksumkey=0xFFFFFFFF;	
extern U8	gFileBuffer[MAX_BUFF_SIZE];
extern S32 gnFileBytesRead;
/* update the CRC on the data block one byte at a time */
U32 UpdateCRC(U32 CRCAccum, U8 *Data, U16 DataBlkSize)
{ 
	register U16 i, j;
	for ( j = 0;  j < DataBlkSize;  j++ )
    	{ 
		i = ( (U16) ( CRCAccum >> 24) ^ *Data++ ) & 0xff;
		CRCAccum = ( CRCAccum << 8 ) ^ CRCTable[i]; 
	}
	return ~CRCAccum; 
}

U32 CRC32(U8 *Data, U16 Length)
{
  return UpdateCRC(checksumkey, Data, Length);
}

theme_error_enum tm_verify_theme_checksum(U8* file_name)
{
	S32 no_of_char;
	U32 checksum;
	FILE_HANDLE file;


#if defined(MMI_ON_HARDWARE_P)
	file=OpenCache((U8*)file_name,PFS_READ_BINARY);
#else
	file=OpenCache((U8*)file_name,(U8*)"r");
#endif

	if(!file)
		return THEME_ERROR_FILE_FORMAT_BAD;
	else
	{
		
		if(gnFileBytesRead > THEME_CHECKSUM_HEADER_SIZE)
		{
			pCacheFileRead(file, THEME_CHECKSUM_HEADER_SIZE, &no_of_char, (U8*)&checksum);
			pfseek(file, THEME_CHECKSUM_HEADER_SIZE, FS_FILE_BEGIN);
		}
		else
		{
			pfclose(file);
			return THEME_ERROR_FILE_FORMAT_BAD;
		}

		while(1)
		{
			
			StartFileRead(file,0);
			if(gnFileBytesRead==MAX_BUFF_SIZE)
			{
				checksumkey=CRC32(gFileBuffer, (U16)gnFileBytesRead);
			}
			else if(gnFileBytesRead!=0 && gnFileBytesRead<MAX_BUFF_SIZE)
			{
				checksumkey=CRC32(gFileBuffer, (U16)gnFileBytesRead);
				break;
			}
		}
		
	}
	pfclose(file);
	if(checksumkey==checksum)
	{
		checksumkey=0xFFFFFFFF;
		return THEME_ERROR_SUCCESS;
	}
	else
	{
		checksumkey=0xFFFFFFFF;
		return THEME_ERROR_FILE_FORMAT_BAD;
        }
	
}

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__

/*****************************************************************************
* FUNCTION
*	HandleThemeActivationErrorStatus
* DESCRIPTION
*  	function to activate default theme
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void HandleThemeActivationErrorStatus(U16 status)
{
	switch(status)
	{
	case THEME_ERROR_SUCCESS:
		DisplayPopup((PU8)GetString(STR_THEME_ACTIVATE_SUCCESS), IMG_GLOBAL_EMPTY,
                     0, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
		break;
	case THEME_ERROR_SPACE_NOT_AVAILABLE:
		DisplayPopup((PU8)GetString(STR_ID_FMGR_SPACE_NOT_AVAILABLE), IMG_GLOBAL_EMPTY,
                     0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		break;
	case THEME_ERROR_THEME_NOT_AVAILABLE:
		DisplayPopup((PU8)GetString(STR_THEME_NOT_AVAILABLE), IMG_GLOBAL_EMPTY,
                     0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		break;
	case THEME_ERROR_FILE_FORMAT_BAD:
		DisplayPopup((PU8)GetString(STR_THEME_FORMAT_NOT_SUPPORTED), IMG_GLOBAL_EMPTY,
                     0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		break;
	case THEME_ERROR_FAILURE:
		DisplayPopup((PU8)GetString(STR_THEME_UNKNOWN_ERROR), IMG_GLOBAL_EMPTY,
                     0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		break;
	case THEME_ERROR_ALREADY_ACTIVATED:
		DisplayPopup((PU8)GetString(STR_ID_FMGR_ALREADY_ACTIVE), IMG_GLOBAL_EMPTY,
                     0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		break;
	case THEME_ERROR_MAX_COUNT_REACHED:
		DisplayPopup((PU8)GetString(STR_ID_FMGR_MAX_COUNT_REACHED), IMG_GLOBAL_EMPTY,
                     0, 1000, WARNING_TONE);
		break;
	case THEME_ERROR_VERSION_FAILURE:
		DisplayPopup((PU8)GetString(STR_DOWNLOAD_THEME_UNSUPPORTED), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		break;
	case THEME_ERROR_PROCESS_QUEUE_SUCESS:
		DisplayPopup(NULL ,IMG_PROCESSING_PHONEBOOK, 1,UI_POPUP_NOTIFYDURATION_TIME,0);
		break;
	default:
		DisplayPopup((PU8)GetString(STR_THEME_UNKNOWN_ERROR), IMG_GLOBAL_EMPTY,
                     0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		break;
	}

}

#endif
#endif
