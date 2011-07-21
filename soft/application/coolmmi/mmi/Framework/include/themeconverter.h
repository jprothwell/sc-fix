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
 *   ThemeConverterXML.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 * Header file for the ThemeConverter of DLT project
 *
 * Author:
 * -------
 * -------
 
 *   
 **************************************************************/
/* if some header should be include before this one */
#define __NEWSIMULATOR 

#include "gui_data_types.h"
#include "mmi_data_types.h"
#include "custthemesres.h"

#undef __NEWSIMULATOR 


#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
/***************************************************************************** 
 

*****************************************************************************/
#define SIZE_XML_TAG 20
#define SIZE_IMAGE_TAG 50

/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef void (*XMLTagHandler)(U8** ); 


/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/

	

/**************  flash tables   ****************/
/*We wll form table containing the all tags and their corresponding function pointers */
typedef struct{
	U8 xml_tag[SIZE_XML_TAG];
	XMLTagHandler tag_handler;
	//void (*funcptr)(char **attr);
}all_tags_index_struct;



/*The table for the image tag and all the image attributes is defined below*/
typedef struct
{
	U8 image_tag[SIZE_IMAGE_TAG];
	U16 imageId;
}image_tags_index_struct;



/* In this table each alphabet corresponds to first location of a tag starting with this index.Elements*/
typedef struct {
    U8 index;
	U8 no_of_elements;
}search_all_tags_struct;


/* The Table for the searching for getting the required image ID*/
typedef struct {
	U8 index;
	U8 no_of_elements;
}search_image_all_tags_struct;



/*Table for the Deviceinfo*/
typedef struct {
	U8 model_ID;
	U32 lcdwidth;
	U32 lcdht;
}deviceinfo;

typedef struct {
	U8 *name;
	float32 version;
}theme_download_info_struct;




/*************** end of flash tables ********************/


/***********  Context structure **************************/
void theme_imageid_image_handler(U16 imageid, U32 image_offset);
UI_image_type theme_non_imageid_image_handler(U32 image_offset);
void play_audio_funcptr(U32 audio_offset);


/******** end of the context structure   ***************/


/*********  function prototypes ***************/
BOOL convert_theme_file_to_mmi_theme(MMI_theme** theme, U32 file_handle);

void inline_list_menu_tag_handler(U8** attr);
void call_scr_tag_handler(U8** attr);
void multi_tap_tag_handler(U8** attr);
void scroll_bar_tag_handler(U8** attr);
void multi_ip_box_tag_handler(U8** attr);
void font_col_tag_handler(U8** attr);
void title_tag_handler(U8** attr);
void rsk_down_tag_handler(U8** attr);
void rsk_up_tag_handler(U8** attr);
void lsk_down_tag_handler(U8** attr);
void lsk_up_tag_handler(U8** attr);
void dialer_ip_box_tag_handler(U8** );
void inline_edit_tag_handler(U8** ) ;
void main_menu_tag_handler(U8** );
void progress_scr_menu_tag_handler(U8**);
void setting_pin_scr_tag_handler(U8**);
void image_list_tag_handler(U8**);
void font_col_list_tag_handler(U8**);
void tones_list_tag_handler(U8**);
void deviceinfo_tag_handler(U8**);
void progress_scr_tag_handler(U8**);
void shortcut_ind_tag_handler(U8** attr);
void statusbar_tag_handler(U8** attr);
void matrix_menu_tag_handler(U8** attr);
void list_menu_tag_handler(U8** attr);
void infobar_tag_handler(U8** attr);
void general_bkgrnd_tag_handler(U8** attr);
void format_ipbox_tag_handler(U8** attr);
void virtual_kb_tag_handler(U8** attr);
void popup_description_tag_handler(U8** attr);
void aphorism_scr_tag_handler(U8** attr);
void popup_scr_tag_handler(U8** attr);
void nw_scr_tag_handler(U8** attr);
void datetimebar_tag_handler(U8** attr);
void call_scr_tag_handler(U8** attr);
void ip_box_tag_handler(U8** attr);
void pin_ip_box_tag_handler(U8** attr);
void drop_dn_tag_handler(U8** attr);
void buttonbar_tag_handler(U8** attr);
void menubar_tag_handler(U8** attr);
int processqueue(void);
void device_theme_ver_info(U8**);
//void fixed_menu_tag_handler(U8**);
U8 extract_no_of_bits(U8  *Data,U16  Offset, U16  Bits);
U8 ascii_to_hex_convert(S8  character);
void my_xml_elem_end_hdlr(void *data, const char *el);
void my_xml_elem_start_hdlr(void *data, const char *el, const char **attr);

#endif
