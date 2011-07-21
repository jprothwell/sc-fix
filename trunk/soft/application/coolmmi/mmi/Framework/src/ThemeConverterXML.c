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
 * Contains parser side handlers for the DLT project
 *
 * Author:
 * -------
 * -------
 
 *   
 **************************************************************/
#define __NEWSIMULATOR 

#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "fat_fs.h"
#include "mmi_data_types.h"
#include "themeconverter.h"
#include "thememanager.h"
#include "globalmenuitems.h"
#include "wallpaperdefs.h"
#include "screensaverdefs.h"
#include "mainmenudef.h"
#include "statusiconres.h"
#include "calendardef.h"
#include "phonebookdef.h"
#include "callmanagementiddef.h"
#include "messagesresourcedata.h"
#include "networksetupgprot.h"
#include "wgui_categories_defs.h"
#include "callhistoryenum.h"
 
#include "wgui_status_icons.h"
#include "phonebookdef.h"
#include "composeringtonedefs.h"
#include "funandgamesdefs.h"

#undef __NEWSIMULATOR 

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__

//Parser Object

XML_PARSER_STRUCT parser;//parser object


#define NO_IMG_ATTR 300
#define NO_XML_TAGS 42 
#define image_list_pos_xml_tag_map 11


//Temporary Images ID for some images



//End of the temporary id's

/*******book Keeping***************/
extern MMI_theme theme_DLT;
extern U16 gcheckThemeVersion;

/**********************************/


/*****************************************************************************
Various table structures for the themeconverter
******************************************************************************/

const all_tags_index_struct XML_tags_map[]={

	{"aphorism_scr",aphorism_scr_tag_handler},
	{"buttonbar",buttonbar_tag_handler},
	{"call_scr",call_scr_tag_handler},
	{"datetimebar",datetimebar_tag_handler},
	{"deviceinfo",deviceinfo_tag_handler},
	{"devicetheme",device_theme_ver_info},
	{"dialer_ip_box",dialer_ip_box_tag_handler},
	{"drop_dn",drop_dn_tag_handler},
	{"font_col",font_col_tag_handler},
	{"format_ipbox",format_ipbox_tag_handler},
	{"general_bkgrnd",general_bkgrnd_tag_handler},
	{"image_lists",image_list_tag_handler},
	{"infobar",infobar_tag_handler},
	{"inline_edit",inline_edit_tag_handler},
	{"inline_list_menu",inline_list_menu_tag_handler},
	{"ip_box",ip_box_tag_handler},
	{"list_menu",list_menu_tag_handler},
	{"lsk_down",lsk_down_tag_handler},
	{"lsk_up",lsk_up_tag_handler},	
	{"main_menu",main_menu_tag_handler},
	{"matrix_menu",matrix_menu_tag_handler},
	{"menubar",menubar_tag_handler},
	{"multi_tap",multi_tap_tag_handler},
	{"nw_scr",nw_scr_tag_handler},
	{"pin_ip_box",pin_ip_box_tag_handler},
	{"popup_description",popup_description_tag_handler},
	{"popup_scr",popup_scr_tag_handler},
	{"progress_scr",progress_scr_tag_handler},
	{"rsk_down",rsk_down_tag_handler},
	{"rsk_up",rsk_up_tag_handler},
	{"scroll_bar",scroll_bar_tag_handler},
	{"setting_pin_scr",setting_pin_scr_tag_handler},
	{"shortcut_ind",shortcut_ind_tag_handler},
	{"statusbar",statusbar_tag_handler},
	{"title",title_tag_handler},
	{"tones_list",tones_list_tag_handler},
	{"virtual_kb",virtual_kb_tag_handler}
};


const image_tags_index_struct image_tags_map[]={
	{"calender_may",IMG_CALENDAR_MAY},
	{"calender_jan",IMG_CALENDAR_JAN},
	{"calender_feb",IMG_CALENDAR_FEB},
	{"calender_mar",IMG_CALENDAR_MAR},
	{"calender_apr",IMG_CALENDAR_APR},	
	{"calender_jun",IMG_CALENDAR_JUN},
	{"calender_jul",IMG_CALENDAR_JUL},
	{"calender_aug",IMG_CALENDAR_AUG},
	{"calender_sep",IMG_CALENDAR_SEP},
	{"calender_oct",IMG_CALENDAR_OCT},
	{"calender_nov",IMG_CALENDAR_NOV},
	{"calender_dec",IMG_CALENDAR_DEC},
	{"calender_week",CAL_ENGLISH_DAYS_IMAGE},
	{"call_scr_out",IMG_CM_STATE_SINGLE_ACTIVE},
	{"call_scr_inc",IMG_PHB_DEFAULT},
	{"call_missed",MISSED_LSK_ICN},
	{"comp_bkg",IMG_RING_COMPOSER_BACKGROUND},
	{"inprogress",IMG_THEME_ACTIVATION},
#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__
	{"message_sending",IMG_NEW_SMS_SEND},
	{"mm_mm_oft",MAIN_MENU_MATRIX_MULTIMEDIA_ICON}, 
	{"mm_ch_oft",MAIN_MENU_MATRIX_CALL_HISTORY_ICON},
	{"mm_extras_oft",MAIN_MENU_MATRIX_EXTRA_ICON},
	{"mm_fng_oft",MAIN_MENU_MATRIX_FUNANDGAMES_ICON},
	{"mm_org_oft",MAIN_MENU_MATRIX_ORGANIZER_ICON},
	{"mm_phb_oft",MAIN_MENU_MATRIX_PHONEBOOK_ICON},
	{"mm_profile_oft",MAIN_MENU_MATRIX_PROFILES_ICON},
	{"mm_scut_oft",MAIN_MENU_MATRIX_SHORTCUTS_ICON},
	{"mm_service_oft",MAIN_MENU_MATRIX_SERVICES_ICON},
	{"mm_settings_oft",MAIN_MENU_MATRIX_SETTINGS_ICON},
	{"mm_sms_oft",MAIN_MENU_MATRIX_MESSAGES_ICON},
	{"mm_fm_oft",MAIN_MENU_MATRIX_FILEMNGR_ICON},
	{"mm_bkgrnd",IMG_MAIN_MENU_CIRCULAR_BG},
	{"mm_mm_list_oft",MAIN_MENU_LIST_MULTIMEDIA_ICON},
	{"mm_ch_list_oft",MAIN_MENU_LIST_CALL_HISTORY_ICON},
	{"mm_extras_list_oft",MAIN_MENU_LIST_EXTRA_ICON},
	{"mm_fng_list_oft",MAIN_MENU_LIST_FUNANDGAMES_ICON},
	{"mm_org_list_oft",MAIN_MENU_LIST_ORGANIZER_ICON},
	{"mm_phb_list_oft",MAIN_MENU_LIST_PHONEBOOK_ICON},
	{"mm_profile_list_oft",MAIN_MENU_LIST_PROFILES_ICON},
	{"mm_scut_list_oft",MAIN_MENU_LIST_SHORTCUTS_ICON},
	{"mm_service_list_oft",MAIN_MENU_LIST_SERVICES_ICON},
	{"mm_settings_list_oft",MAIN_MENU_LIST_SETTINGS_ICON},
	{"mm_sms_list_oft",MAIN_MENU_LIST_MESSAGES_ICON},
	{"mm_fm_list_oft",MAIN_MENU_LIST_FILEMNGR_ICON},
	{"mm_mm_circ_oft",MAIN_MENU_CIRCULAR_MULTIMEDIA_ICON},
	{"mm_ch_circ_oft",MAIN_MENU_CIRCULAR_CALL_HISTORY_ICON},
	{"mm_extras_circ_oft",MAIN_MENU_CIRCULAR_EXTRA_ICON},
	{"mm_fng_circ_oft",MAIN_MENU_CIRCULAR_FUNANDGAMES_ICON},
	{"mm_org_circ_oft",MAIN_MENU_CIRCULAR_ORGANIZER_ICON},
	{"mm_phb_circ_oft",MAIN_MENU_CIRCULAR_PHONEBOOK_ICON},
	{"mm_profile_circ_oft",MAIN_MENU_CIRCULAR_PROFILES_ICON},
	{"mm_scut_circ_oft",MAIN_MENU_CIRCULAR_SHORTCUTS_ICON},
	{"mm_service_circ_oft",MAIN_MENU_CIRCULAR_SERVICES_ICON},
	{"mm_settings_circ_oft",MAIN_MENU_CIRCULAR_SETTINGS_ICON},
	{"mm_fm_circ_oft",MAIN_MENU_CIRCULAR_FILEMNGR_ICON},
	{"mm_sms_circ_oft",MAIN_MENU_CIRCULAR_MESSAGES_ICON},
	{"mm_mm_rotate_oft",MAIN_MENU_ROTATE_MULTIMEDIA_ICON},
	{"mm_ch_rotate_oft",MAIN_MENU_ROTATE_CALL_HISTORY_ICON},
	{"mm_extras_rotate_oft",MAIN_MENU_ROTATE_EXTRA_ICON},
	{"mm_fng_rotate_oft",MAIN_MENU_ROTATE_FUNANDGAMES_ICON},
	{"mm_org_rotate_oft",MAIN_MENU_ROTATE_ORGANIZER_ICON},
	{"mm_phb_rotate_oft",MAIN_MENU_ROTATE_PHONEBOOK_ICON},
	{"mm_profile_rotate_oft",MAIN_MENU_ROTATE_PROFILES_ICON},
	{"mm_scut_rotate_oft",MAIN_MENU_ROTATE_SHORTCUTS_ICON},
	{"mm_service_rotate_oft",MAIN_MENU_ROTATE_SERVICES_ICON},
	{"mm_settings_rotate_oft",MAIN_MENU_ROTATE_SETTINGS_ICON},
	{"mm_sms_rotate_oft",MAIN_MENU_ROTATE_MESSAGES_ICON},
	{"mm_fm_rotate_oft",MAIN_MENU_ROTATE_FILEMNGR_ICON},
	{"mm_mm_ma_oft",MAIN_MENU_MATRIX_MULTIMEDIA_ANIMATION},
	{"mm_ch_ma_oft",MAIN_MENU_MATRIX_CALL_HISTORY_ANIMATION},
	{"mm_extras_ma_oft",MAIN_MENU_MATRIX_EXTRA_ANIMATION},
	{"mm_fng_ma_oft",MAIN_MENU_MATRIX_FUNANDGAMES_ANIMATION},
	{"mm_org_ma_oft",MAIN_MENU_MATRIX_ORGANIZER_ANIMATION},
	{"mm_phb_ma_oft",MAIN_MENU_MATRIX_PHONEBOOK_ANIMATION},
	{"mm_profile_ma_oft",MAIN_MENU_MATRIX_PROFILES_ANIMATION},
	{"mm_scut_ma_oft",MAIN_MENU_MATRIX_SHORTCUTS_ANIMATION},
	{"mm_service_ma_oft",MAIN_MENU_MATRIX_SERVICES_ANIMATION},
	{"mm_settings_ma_oft",MAIN_MENU_MATRIX_SETTINGS_ANIMATION},
	{"mm_sms_ma_oft",MAIN_MENU_MATRIX_MESSAGES_ANIMATION},
	{"mm_fm_ma_oft",MAIN_MENU_MATRIX_FILEMNGR_ANIMATION},
	{"mm_mm_pa_oft",MAIN_MENU_PAGE_MULTIMEDIA_ICON},
	{"mm_ch_pa_oft",MAIN_MENU_PAGE_CALL_HISTORY_ICON},
	{"mm_extras_pa_oft",MAIN_MENU_PAGE_EXTRA_ICON},
	{"mm_fng_pa_oft",MAIN_MENU_PAGE_FUNANDGAMES_ICON},
	{"mm_org_pa_oft",MAIN_MENU_PAGE_ORGANIZER_ICON},
	{"mm_phb_pa_oft",MAIN_MENU_PAGE_PHONEBOOK_ICON},
	{"mm_profile_pa_oft",MAIN_MENU_PAGE_PROFILES_ICON},
	{"mm_scut_pa_oft",MAIN_MENU_PAGE_SHORTCUTS_ICON},
	{"mm_service_pa_oft",MAIN_MENU_PAGE_SERVICES_ICON},
	{"mm_settings_pa_oft",MAIN_MENU_PAGE_SETTINGS_ICON},
	{"mm_sms_pa_oft",MAIN_MENU_PAGE_MESSAGES_ICON},
	{"mm_fm_pa_oft",MAIN_MENU_PAGE_FILEMNGR_ICON},
#endif
	{"popup",IMG_PROCESSING_PHONEBOOK},
	{"phb_sub_sear",IMG_ID_PHB_QUICK_SEARCH},
	{"phb_sub_se_en",IMG_PHB_SEARCH_ENTRY},
	{"phb_sub_add",IMG_PHB_ADD_ENTRY},
	{"phb_sub_copy",IMG_PHB_COPY_ALL},
	{"phb_sub_del",IMG_PHB_DELETE_ALL},
	{"phb_sub_grp",IMG_PHB_CALLER_GROUP},
	{"phb_sub_ext",IMG_PHB_OWNER_NUMBER},
	{"phb_sub_sett",IMG_PHB_SETTINGS},
	{"scr_idl_search",IMG_ID_NETSET_NW_SEARCH},
	{"scr_idl_logo",IMG_BOOTUP_SCANNING_LOGO},
	{"scsaver",CURRENT_THEME_SCREENSAVER},
	{"status_battery",IMG_SI_BATTERY_STRENGTH},
	{"status_sig",IMG_SI_SIGNAL_STRENGTH},
	{"st_ROAMING",IMG_SI_ROAMING_INDICATOR},
	{"st_CALL_DIVERT_L1",IMG_SI_CALL_DIVERT_L1},
	{"st_CALL_DIVERT_L2",IMG_SI_CALL_DIVERT_L2},
	{"st_MUTE",IMG_SI_MUTE},
	{"st_INCOMING_SMS",IMG_SI_SMS_INDICATOR},
	{"st_MMS_UNREAD",IMG_SI_MMS_UNREAD_INDICATOR},
	{"st_MMS_BUSY",IMG_SI_MMS_BUSY_INDICATOR},
	{"st_VIBRATION",IMG_SI_VIBRATE},
	{"st_SILENT",IMG_SI_SILENT},
	{"st_RING",IMG_SI_RING},
	{"st_VIBRATE_AND_RING",IMG_SI_VIBRATE_AND_RING},
	{"st_VIBRATE_THEN_RING",IMG_SI_VIBRATE_THEN_RING},
	{"st_GPRS_SERVICE",IMG_SI_GPRS_INDICATOR},
	{"st_EARPHONE_INDICATOR",IMG_SI_EARPHONE_INDICATOR},
	{"st_GPRS_ATT_NO_PDP_INDICATOR",IMG_SI_GPRS_ATT_NO_PDP_INDICATOR},
	{"st_ALARM",IMG_SI_ALARM_ACTIVATED},
	{"st_VOICE_RECORD",IMG_SI_VOICE_RECORD},
	{"st_KEYPAD_LOCK",IMG_SI_KEYPAD_LOCK},
	{"st_MISSED_CALL",IMG_SI_MISSED_CALL_INDICATOR},
	{"st_UNREAD_VOICE_L1L2",IMG_SI_UNREAD_VOICE_L1L2},
	{"st_UNREAD_VOICE_L1",IMG_SI_UNREAD_VOICE_L1},
	{"st_UNREAD_VOICE_L2",IMG_SI_UNREAD_VOICE_L2},
	{"st_UNREAD_FAX_L1L2",IMG_SI_UNREAD_FAX_L1L2},
	{"st_UNREAD_FAX_L1",IMG_SI_UNREAD_FAX_L1},
	{"st_UNREAD_FAX_L2",IMG_SI_UNREAD_FAX_L2},
	{"st_UNREAD_EMAIL_L1L2",IMG_SI_UNREAD_EMAIL_L1L2},
	{"st_UNREAD_EMAIL_L1",IMG_SI_UNREAD_EMAIL_L1},
	{"st_UNREAD_EMAIL_L2",IMG_SI_UNREAD_EMAIL_L2},
	{"st_IMPS_MSG",IMG_SI_IMPS_MSG},
	{"st_IMPS_LOGIN",IMG_SI_IMPS_LOGIN},
	{"st_WAP",IMG_SI_WAP},
	{"st_POC",IMG_SI_POC},
	{"st_BT",IMG_SI_BT},
	{"st_IR",IMG_SI_IR_INDICATOR},
	{"st_STOP_WATCH",IMG_SI_STOPWATCH},
	{"st_CHAT_INDICATION_ROOM1",IMG_BLINKINGICON_CHAT_ROOM1},
	{"st_CHAT_INDICATION_ROOM2",IMG_BLINKINGICON_CHAT_ROOM2},
	{"st_NETWORK_CIPHER_GSM",IMG_SI_NETWORK_CIPHER_GSM},
	{"st_NETWORK_CIPHER_GPRS",IMG_SI_NETWORK_CIPHER_GPRS},
	{"st_sig1",IMG_SI_SIGNAL_STRENGTH_L1},
	{"st_sig2",IMG_SI_SIGNAL_STRENGTH_L2},
	{"st_JAVA_PAUSE",IMG_SI_JAVA_PAUSE},
	{"wallpaper",CURRENT_THEME_WALLPAPER}
	};

const search_all_tags_struct search_all_tags_map[]={
	{0,1},//a
	{1,1},//b
	{2,1},//c
	{3,5},//d
	{0,0},//e
	{8,2},//f
	{10,1},//g
	{0,0},//h
	{11,5},//i
	{0,0},//j
	{0,0},//k
	{16,3},//l
	{19,4},//m
	{23,1},//n
	{0,0},//o
	{24,4},//p
	{0,0},//q
	{28,2},//r
	{30,4},//s
	{34,2},//t
	{0,0},//u
	{36,1},//v
	{0,0},//w
	{0,0},//x
	{0,0},//y
	{0,0}//z
};


#ifdef __MMI_MAINMENU_STYLE_CHANGE_EN__

const search_image_all_tags_struct search_all_tags_image_map[]={
	{0,0},//a
	{0,0},//b
	{0,17},//c
	{0,0},//d
	{0,0},//e
	{0,0},//f
	{0,0},//g
	{0,0},//h
	{17,1},//i
	{0,0},//j
	{0,0},//k
	{0,0},//l
	{18,74},//m
	{0,0},//n
	{0,0},//o
	{92,9},//p
	{0,0},//q
	{0,0},//r
	{101,47},//s
	{0,0},//t
	{0,0},//u
	{0,0},//v
	{148,1},//w
	{0,0},//x
	{0,0},//y
	{0,0}//z
};
#else

const search_image_all_tags_struct search_all_tags_image_map[]={
	{0,0},//a
	{0,0},//b
	{0,17},//c
	{0,0},//d
	{0,0},//e
	{0,0},//f
	{0,0},//g
	{0,0},//h
	{17,1},//i
	{0,0},//j
	{0,0},//k
	{0,0},//l
	{0,0},//m
	{0,0},//n
	{0,0},//o
	{18,9},//p
	{0,0},//q
	{0,0},//r
	{27,47},//s
	{0,0},//t
	{0,0},//u
	{0,0},//v
	{74,1},//w
	{0,0},//x
	{0,0},//y
	{0,0}//z
};
#endif

static U8 thm_process_call_count;//theme Process Queue Counter
/*****************************************************************************
* FUNCTION
*	 my_xml_elem_start_hdlr(void *data, const char *el, const char **attr)
* DESCRIPTION
*  function for handling of the start element tag
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/
void my_xml_elem_start_hdlr(void *data, const char *el, const char **attr)
{	
      
	
      if(gcheckThemeVersion)
	  {
		if(!(strncmp((PS8)"devicetheme",el,strlen(el)))||!(strncmp((PS8)"deviceinfo",el,strlen(el)))||!(strncmp((PS8)"media",el,strlen(el))))
		{/* continue*/
		}
		else
		{	xml_stop_parse(&parser );
			return;

		}
	  }
		if(!strncmp(el,"image_lists",strlen(el)))
		{
			(*XML_tags_map[image_list_pos_xml_tag_map].tag_handler)((U8**)attr);
			
		}
		else
		{
			U8 first_alpha_tag=*(el);
			U8 count1;//count for total number of the image attributes
			U8 loc;//position of the tag in the table
		    			
			loc=search_all_tags_map[first_alpha_tag-97].index;
			
				
				
				for( count1=0;count1<search_all_tags_map[first_alpha_tag-97].no_of_elements;count1++,loc++)
				{
					S16 check;
					check=strncmp((PS8)XML_tags_map[loc].xml_tag,el,strlen(el));
				
					if(check==0)
					{
						(*XML_tags_map[loc].tag_handler)((U8**)attr);
					}
					

				}
		}
     	
     thm_process_call_count++;	
}

/*****************************************************************************
* FUNCTION
*	 my_xml_elem_end_hdlr(void *data, const char *el)
* DESCRIPTION
*  function for handling of the end element tag
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/
void my_xml_elem_end_hdlr(void *data, const char *el)
{
	U8 end_element[]= "media";
	if(!strcmp(el,(PS8)end_element))
	{
	xml_stop_parse(&parser );
	}
	else
	{
		if(thm_process_call_count==15)
#ifdef __DLT_ENABLE_PROCESS_QUEUE__
		theme_process_queue();		
#else
		{}
#endif
	}
	
	
}


/*****************************************************************************
* FUNCTION
*	 processqueue(void)
* DESCRIPTION
*  function for polling of the queues
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/


int processqueue(void)
{
	return 0;
}


/*****************************************************************************
* FUNCTION
*	 image_list_tag_handler(U8 **attr)
* DESCRIPTION
*  handles the image_list tag obained by parsing the XML file
* PARAMETERS
*	 
* RETURNS
*   
* GLOBALS AFFECTED
*  
*****************************************************************************/
void image_list_tag_handler(U8 **attr)
{
	U8 first_alpha_attr;
	U8 count,count1;//count for total number of the image attributes
	U8 loc;//position of the attribute in the table

	for(count=0;count<NO_IMG_ATTR;count+=2)
	{
		if(attr[count] == NULL)
			break;
		first_alpha_attr=attr[count][0];				
		loc=search_all_tags_image_map[first_alpha_attr-97].index;		
        for( count1=loc;count1<=(search_all_tags_image_map[first_alpha_attr-97].no_of_elements+loc);count1++)
		{
			S16 check;
			check=strncmp((PS8)image_tags_map[count1].image_tag, (PS8)attr[count],strlen((PS8)attr[count]));
			if(check==0)
			{
				tm_set_imageid(image_tags_map[count1].imageId ,(U32)atoi((PS8)attr[count+1]));
			
			}
		}
	}
}

#endif//mmi downloadble themes support
