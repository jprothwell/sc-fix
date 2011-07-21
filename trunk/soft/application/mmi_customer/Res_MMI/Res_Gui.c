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

/**************************************************************

	FILENAME	: Res_.c

  	PURPOSE		: Populate Resource for 

	REMARKS		: nil

	AUTHOR		: Yogesh

	DATE		: August-11-2003

**************************************************************/
//PMT NEERAJ START 20050616
#include "cs_types.h"
#include "custdatares.h"
#include "mmi_features.h"
//PMT NEERAJ END 20050616
#include "stdc.h"
#include "customcfg.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

//#include "wgui_categories.h"
//#include "wgui_categories_inputs.h"
#include "gui_data_types.h"
#include "wgui_categories.h"
#include "wgui_categories_defs.h"
#include "gui_config.h"
#include "statusiconres.h"
#include "mmi_data_types.h"
//#include "wgui_fixed_menus.h"
#ifdef __MMI_WLAN_FEATURES__
#include "dataaccount.h"
#endif

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"

void PopulateUnifiedComposerEditorRes (void)
{
#ifdef __UI_UNIFIED_COMPOSER_EDITOR_CATEGORY__
ADD_APPLICATION_STRING2 (WGUI_UCE_SMS_STRING_ID, "SMS", "SMS string used in information bar showing size of SMS");
ADD_APPLICATION_STRING2 (WGUI_UCE_MMS_STRING_ID, "MMS", "MMS string used in information bar showing size of MMS");

#ifdef __MMI_TOUCH_SCREEN__
ADD_APPLICATION_IMAGE2 (WGUI_UCE_ATTACHFILE_ICON_IMAGE_ID, CUST_IMG_PATH"/MainLCD/UCE/LAttachfile.pbm", "Attachment ICON");
ADD_APPLICATION_IMAGE2 (WGUI_UCE_AUDIO_ICON_IMAGE_ID, CUST_IMG_PATH"/MainLCD/UCE/LAudio.pbm", "Audio ICON");
ADD_APPLICATION_IMAGE2 (WGUI_UCE_PICTURE_ICON_IMAGE_ID, CUST_IMG_PATH"/MainLCD/UCE/LPic.pbm", "Picture ICON");
ADD_APPLICATION_IMAGE2 (WGUI_UCE_VIDEO_ICON_IMAGE_ID, CUST_IMG_PATH"/MainLCD/UCE/LVideo.pbm", "Video ICON");
#else
ADD_APPLICATION_IMAGE2 (WGUI_UCE_ATTACHFILE_ICON_IMAGE_ID, CUST_IMG_PATH"/MainLCD/UCE/SAttachfile.pbm", "Attachment ICON");
ADD_APPLICATION_IMAGE2 (WGUI_UCE_AUDIO_ICON_IMAGE_ID, CUST_IMG_PATH"/MainLCD/UCE/SAudio.pbm", "Audio ICON");
ADD_APPLICATION_IMAGE2 (WGUI_UCE_PICTURE_ICON_IMAGE_ID, CUST_IMG_PATH"/MainLCD/UCE/Spic.pbm", "Picture ICON");
ADD_APPLICATION_IMAGE2 (WGUI_UCE_VIDEO_ICON_IMAGE_ID, CUST_IMG_PATH"/MainLCD/UCE/SVideo.pbm", "Video ICON");
#endif /*__MMI_TOUCH_SCREEN__ */
ADD_APPLICATION_IMAGE2 (WGUI_DEFAULT_PREVIEW_IMAGE_ID, CUST_IMG_PATH"/MainLCD/UCE/broken.gif", "Default Preview");
#endif /* __UI_UNIFIED_COMPOSER_EDITOR_CATEGORY__ */
}

void PopulateCommonCategoryResources(void)
{	
      /*
      0x87=>Monday for English
      0x88=>Tuesday
      0x89=>Wednesday
      0x8a=>Thursday
      0x8b=>Friday
      0x8c=>Saturday
      0x8d=>Sunday
      0x8e=>Monday for Traditional Chinese and Simplified Chinese
      0x8f=>Tuesday
      0x90=>Wednesday
      0x91=>Thursday
      0x92=>Friday
      0x93=>Saturday
      0x94=>Sunday
      */
      #ifdef __MMI_SUBLCD__
      S8 str_english_monday[] = { (S8)0x87, (S8)0x00 };
      S8 str_english_tuesday[] ={ (S8)0x88, (S8)0x00 };
      S8 str_english_wednesday[] ={ (S8)0x89, (S8)0x00 };
      S8 str_english_thursday[] ={ (S8)0x8a, (S8)0x00 };
      S8 str_english_friday[] ={ (S8)0x8b, (S8)0x00 };
      S8 str_english_saturday[] ={ (S8)0x8c, (S8)0x00 };
      S8 str_english_sunday[] ={ (S8)0x8d, (S8)0x00 };

      S8 str_chinese_monday[] ={ '(', (S8)0x8e, ')', (S8)0x00 };
      S8 str_chinese_tuesday[] ={ '(', (S8)0x8f, ')', (S8)0x00 };
      S8 str_chinese_wednesday[] ={ '(', (S8)0x90, ')', (S8)0x00 };
      S8 str_chinese_thursday[] ={ '(', (S8)0x91, ')', (S8)0x00 };
      S8 str_chinese_friday[] ={ '(', (S8)0x92, ')', (S8)0x00 };
      S8 str_chinese_saturday[] ={ '(', (S8)0x93, ')', (S8)0x00 };
      S8 str_chinese_sunday[] ={ '(', (S8)0x94, ')', (S8)0x00 };
      #endif
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_DD_STRING_ID,"DD","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_MM_STRING_ID,"MM","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_YYYY_STRING_ID,"YYYY","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_THH_STRING_ID,"HH","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_TMM_STRING_ID,"MM","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_CLEAR_STRING_ID,"Clear","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_ABC_STRING_ID,"ABC","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_abc_STRING_ID,"abc","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_123_STRING_ID,"123","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_YYYY_MM_DD_STRING_ID,"YYYY/MM/DD","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_HH_MM_STRING_ID,"HH:MM","Category screens string");

//PMT dara added for Multitap thai 
#if defined(__MMI_MULTITAP_THAI__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_thai_STRING_ID,"Th","Category screens string");
#endif 
//PMT START GR_HE_CZ_FI_DA_HU 20051007
#if defined(__MMI_MULTITAP_HEBREW__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_hebrew_STRING_ID,"He","Category screens string");
#endif 
//PMT END GR_HE_CZ_FI_DA_HU 20051007
#if defined(__MMI_MULTITAP_SPANISH__)||defined(__MMI_T9_SPANISH__)||defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)||defined(__MMI_SMART_SPANISH__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_SPANISH_STRING_ID,"ES","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_spanish_STRING_ID,"es","Category screens string");
#endif	
//PMT CZ_PO_TU START 20050812
#if defined(__MMI_MULTITAP_POLISH__)||defined(__MMI_T9_POLISH__)||defined(__MMI_ZI_POLISH__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_POLISH_STRING_ID,"PO","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_polish_STRING_ID,"po","Category screens string");
#endif
//PMT CZ_PO_TU END 20050812
//PMT VIKASG START 20050729
#if defined(__MMI_MULTITAP_DANISH__)||defined(__MMI_T9_DANISH__)||defined(__MMI_ZI_DANISH__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_DANISH_STRING_ID,"DA","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_danish_STRING_ID,"da","Category screens string");
#endif	
//PMT VIKASG END 20050729
#if defined(__MMI_MULTITAP_FRENCH__)||defined(__MMI_T9_FRENCH__)||defined(__MMI_ZI_CA_FRENCH__)||defined(__MMI_ZI_EU_FRENCH__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_FRENCH_STRING_ID,"FR","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_french_STRING_ID,"fr","Category screens string");
#endif
#if defined(__MMI_MULTITAP_MYANMAR__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_MYANMAR_STRING_ID, "My", "Category screens string");
#endif
#if defined(__MMI_MULTITAP_GERMAN__)||defined(__MMI_T9_GERMAN__)||defined(__MMI_ZI_GERMAN__)||defined(__MMI_SMART_GERMAN__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_GERMAN_STRING_ID,"DE","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_german_STRING_ID,"de","Category screens string");
#endif	
#if defined(__MMI_MULTITAP_ITALIAN__)||defined(__MMI_T9_ITALIAN__)||defined(__MMI_ZI_ITALIAN__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_ITALIAN_STRING_ID,"IT","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_italian_STRING_ID,"it","Category screens string");
#endif	
//START PMT VANDANA 20051506
#if defined (__MMI_T9_RUSSIAN__) || defined(__MMI_MULTITAP_RUSSIAN__)||defined(__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
//PMT END
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_RUSSIAN_STRING_ID,"RU","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_russian_STRING_ID,"ru","Category screens string");
#endif	
//PMT BULGARIAN START 20051201
#if defined (__MMI_T9_BULGARIAN__) || defined(__MMI_MULTITAP_BULGARIAN__)||defined(__MMI_ZI_BULGARIAN__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_BULGARIAN_STRING_ID,"BG","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_bulgarian_STRING_ID,"bg","Category screens string");
#endif
//PMT BULGARIAN END 20051201
//PMT CZ_PO_TU START 20050812
#if defined(__MMI_MULTITAP_TURKISH__)||defined(__MMI_ZI_TURKISH__)||defined(__MMI_T9_TURKISH__)
//PMT CZ_PO_TU END 20050812
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_TURKISH_STRING_ID,"TR","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_turkish_STRING_ID,"tr","Category screens string");
#endif	
//PMT START GR_HE_CZ_FI_DA_HU 20051007
//PMT LANGUAGE START 20051221
#if defined(__MMI_MULTITAP_HEBREW__)||defined(__MMI_ZI_HEBREW__)||defined(__MMI_T9_HEBREW__)
//PMT LANGUAGE END 20051221
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_HEBREW_STRING_ID,"HE","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_hebrew_STRING_ID,"he","Category screens string");
#endif
//PMT END GR_HE_CZ_FI_DA_HU 20051007
//PMT SHARIQ START 20050729
#if defined(__MMI_MULTITAP_PORTUGUESE__)||defined(__MMI_T9_PORTUGUESE__)||defined(__MMI_ZI_EU_PORTUGUESE__)||defined(__MMI_ZI_SA_PORTUGUESE__)||defined(__MMI_ZI_BZ_PORTUGUESE__)
//PMT SHARIQ END 20050729
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_PORTUGUESE_STRING_ID,"PT","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_portuguese_STRING_ID,"pt","Category screens string");
#endif	
//PMT Gurdev Start 20050729
#if defined(__MMI_MULTITAP_INDONESIAN__)||defined(__MMI_ZI_INDONESIAN__)||defined(__MMI_T9_INDONESIAN__)  
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_INDONESIAN_STRING_ID,"ID","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_indonesian_STRING_ID,"id","Category screens string");
#endif	
//PMT Gurdev End 20050729
//PMT CZ_PO_TU START 20050812
//PMT START GR_HE_CZ_FI_DA_HU 20051007
#if defined(__MMI_MULTITAP_CZECH__)||defined(__MMI_T9_CZECH__)||defined(__MMI_ZI_CZECH__)
//PMT END GR_HE_CZ_FI_DA_HU 20051007
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_CZECH_STRING_ID,"CZ","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_czech_STRING_ID,"cz","Category screens string");
#endif	
//PMT CZ_PO_TU END 20050812
//PMT START LANGUAGE 20060104
#if defined(__MMI_MULTITAP_MALAY__)||defined(__MMI_ZI_MALAY__)||defined(__MMI_T9_MALAY__)
//PMT END LANGUAGE 20060104
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_MALAY_STRING_ID,"MS","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_malay_STRING_ID,"ms","Category screens string");
#endif	
// PMT BHASKAR START 20052907
#if defined(__MMI_MULTITAP_VIETNAMESE__)||defined(__MMI_ZI_VIETNAMESE__)||defined(__MMI_T9_VIETNAMESE__) ||defined(__MMI_MULTITAP_VIETNAMESE__) 
// PMT BHASKAR END 20052907
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_VIETNAMESE_STRING_ID,"VI","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_vietnamese_STRING_ID,"vi","Category screens string");
#endif	
//PMT START HU_FI 20050825
#if defined(__MMI_MULTITAP_FINNISH__)||defined(__MMI_T9_FINNISH__)||defined(__MMI_ZI_FINNISH__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_FINNISH_STRING_ID,"FI","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_finnish_STRING_ID,"fi","Category screens string");
#endif
//PMT START GR_HE_CZ_FI_DA_HU 20051007
#if defined(__MMI_MULTITAP_HUNGARIAN__)||defined(__MMI_T9_HUNGARIAN__)||defined(__MMI_ZI_HUNGARIAN__) 
//PMT END GR_HE_CZ_FI_DA_HU 20051007
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_HUNGARIAN_STRING_ID,"HG","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_hungarian_STRING_ID,"hg","Category screens string");
#endif	
//PMT END HU_FI 20050825
//PMT NO_AR_SL_DU START 20050909
#if defined(__MMI_MULTITAP_ARABIC__)||defined(__MMI_T9_ARABIC__) || defined(__MMI_ZI_ARABIC__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_ARABIC_STRING_ID,"AR","Category screens string");
#endif	
//PMT START PERSIAN
#if defined(__MMI_MULTITAP_PERSIAN__) || defined(__MMI_ZI_PERSIAN__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_PERSIAN_STRING_ID,"FA","Category screens string");
#endif	
#if defined(__MMI_MULTITAP_URDU__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_URDU_STRING_ID,"URDU","Category screens string");
#endif
//PMT END PERSIAN
//PMT SHARIQ END 20050802
//PMT VIKAS GUPTA START 20051129
#if defined(__MMI_MULTITAP_HINDI__)||defined(__MMI_T9_HINDI__) || defined(__MMI_ZI_HINDI__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_HINDI_STRING_ID,"AAaI","Category screens string");
#endif
//PMT VIKAS GUPTA END 20051129
//Norwegian start
//PMT VIKAS GUPTA START PO_NO_ZI 20050930
#if defined(__MMI_MULTITAP_NORWEGIAN__)||defined(__MMI_T9_NORWEGIAN__)|| defined(__MMI_ZI_NORWEGIAN__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_NORWEGIAN_STRING_ID,"NO","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_norwegian_STRING_ID,"no","Category screens string");
#endif
//PMT VIKAS GUPTA END PO_NO_ZI 20050930
//Norwegian end
#if defined(__MMI_MULTITAP_SLOVAK__)||defined(__MMI_T9_SLOVAK__)||defined(__MMI_ZI_SLOVAK__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_SLOVAK_STRING_ID,"SK","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_slovak_STRING_ID,"sk","Category screens string");
#endif
#if defined(__MMI_MULTITAP_DUTCH__)||defined(__MMI_ZI_DUTCH__)||defined(__MMI_T9_DUTCH__)  
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_DUTCH_STRING_ID,"NL","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_dutch_STRING_ID,"nl","Category screens string");
#endif
//PMT NO_AR_SL_DU END 20050909
//PMT START SWEDISH 20050916 
#if defined(__MMI_MULTITAP_SWEDISH__)||defined(__MMI_T9_SWEDISH__)||defined(__MMI_ZI_SWEDISH__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_SWEDISH_STRING_ID,"SV","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_swedish_STRING_ID,"sv","Category screens string");
#endif
//PMT END SWEDISH 20050916

//PMT START CR_ROM_SLO 20050923
#if defined(__MMI_MULTITAP_CROATIAN__)||defined(__MMI_T9_CROATIAN__)||defined(__MMI_ZI_CROATIAN__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_CROATIAN_STRING_ID,"HR","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_croatian_STRING_ID,"hr","Category screens string");
#endif
//PMT START LANGUAGE 20060104
#if defined(__MMI_MULTITAP_ROMANIAN__)||defined(__MMI_ZI_ROMANIAN__)||defined(__MMI_T9_ROMANIAN__)
//PMT END LANGUAGE 20060104
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_ROMANIAN_STRING_ID,"RO","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_romanian_STRING_ID,"ro","Category screens string");
#endif
#if defined(__MMI_MULTITAP_SLOVENIAN__)||defined(__MMI_T9_SLOVENIAN__)||defined(__MMI_ZI_SLOVENIAN__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_SLOVENIAN_STRING_ID,"SL","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_slovenian_STRING_ID,"sl","Category screens string");
#endif
//PMT END CR_ROM_SLO 20050923

//PMT START GR_HE_CZ_FI_DA_HU 20051007
#if defined(__MMI_MULTITAP_GREEK__)||defined(__MMI_T9_GREEK__)||defined(__MMI_ZI_GREEK__)
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_GREEK_STRING_ID,"EL","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_greek_STRING_ID,"el","Category screens string");
#endif
//PMT END GR_HE_CZ_FI_DA_HU 20051007
#ifdef __MMI_SUBLCD__
     ADD_APPLICATION_STRING2(STR_SUBLCD_CHI_SUNDAY,str_chinese_sunday,"#NO_TRANS# Day of week string for Sub LCD");
	ADD_APPLICATION_STRING2(STR_SUBLCD_CHI_MONDAY,str_chinese_monday,"#NO_TRANS# Day of week string for Sub LCD");
	ADD_APPLICATION_STRING2(STR_SUBLCD_CHI_TUESDAY,str_chinese_tuesday,"#NO_TRANS# Day of week string for Sub LCD");
	ADD_APPLICATION_STRING2(STR_SUBLCD_CHI_WEDNESDAY,str_chinese_wednesday,"#NO_TRANS# Day of week string for Sub LCD");
	ADD_APPLICATION_STRING2(STR_SUBLCD_CHI_THURSDAY,str_chinese_thursday,"#NO_TRANS# Day of week string for Sub LCD");
	ADD_APPLICATION_STRING2(STR_SUBLCD_CHI_FRIDAY,str_chinese_friday,"#NO_TRANS# Day of week string for Sub LCD");
	ADD_APPLICATION_STRING2(STR_SUBLCD_CHI_SATURDAY,str_chinese_saturday,"#NO_TRANS# Day of week string for Sub LCD");

     ADD_APPLICATION_STRING2(STR_SUBLCD_ENG_SUNDAY,str_english_sunday,"#NO_TRANS# Day of week string for Sub LCD");
	ADD_APPLICATION_STRING2(STR_SUBLCD_ENG_MONDAY,str_english_monday,"#NO_TRANS# Day of week string for Sub LCD");
	ADD_APPLICATION_STRING2(STR_SUBLCD_ENG_TUESDAY,str_english_tuesday,"#NO_TRANS# Day of week string for Sub LCD");
	ADD_APPLICATION_STRING2(STR_SUBLCD_ENG_WEDNESDAY,str_english_wednesday,"#NO_TRANS# Day of week string for Sub LCD");
	ADD_APPLICATION_STRING2(STR_SUBLCD_ENG_THURSDAY,str_english_thursday,"#NO_TRANS# Day of week string for Sub LCD");
	ADD_APPLICATION_STRING2(STR_SUBLCD_ENG_FRIDAY,str_english_friday,"#NO_TRANS# Day of week string for Sub LCD");
	ADD_APPLICATION_STRING2(STR_SUBLCD_ENG_SATURDAY,str_english_saturday,"#NO_TRANS# Day of week string for Sub LCD");
#endif /* #ifdef __MMI_SUBLCD__ */

	/*code added vandana mehtani 12 june*/
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_TSS_STRING_ID,"SS","Category screens string");

	ADD_APPLICATION_STRING2(STR_GUI_MONTH_JAN,"JAN","String for January");
	ADD_APPLICATION_STRING2(STR_GUI_MONTH_FEB,"FEB","String for January");                           
	ADD_APPLICATION_STRING2(STR_GUI_MONTH_MAR,"MAR","String for January");
	ADD_APPLICATION_STRING2(STR_GUI_MONTH_APL,"APL","String for January");
	ADD_APPLICATION_STRING2(STR_GUI_MONTH_MAY,"MAY","String for January");
	ADD_APPLICATION_STRING2(STR_GUI_MONTH_JUN,"JUN","String for January");
	ADD_APPLICATION_STRING2(STR_GUI_MONTH_JLY,"JLY","String for January");
	ADD_APPLICATION_STRING2(STR_GUI_MONTH_AUG,"AUG","String for January");
	ADD_APPLICATION_STRING2(STR_GUI_MONTH_SEP,"SEP","String for January");
	ADD_APPLICATION_STRING2(STR_GUI_MONTH_OCT,"OCT","String for January");
	ADD_APPLICATION_STRING2(STR_GUI_MONTH_NOV,"NOV","String for January");
	ADD_APPLICATION_STRING2(STR_GUI_MONTH_DEC,"DEC","String for January");
	ADD_APPLICATION_STRING2(STR_GUI_TIME_AM,"AM","String for AM");
	ADD_APPLICATION_STRING2(STR_GUI_TIME_PM,"PM","String for PM");

	ADD_APPLICATION_STRING(STR_GUI_DAY, "day");


	/*code ended vandana mehtani*/
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_EDIT_STRING_ID,"Edit","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_CANCEL_STRING_ID,"Cancel","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_DONE_STRING_ID,"Done","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_ON_STRING_ID,"On","Category screens string");
	ADD_APPLICATION_STRING2(WGUI_CATEGORY_OFF_STRING_ID,"Off","Category screens string");
#ifndef __PRODUCT_QPAD__                             			
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL0,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC00.gif","VOL LEVEL0");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL1,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC01.gif","VOL LEVEL1");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL2,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC02.gif","VOL LEVEL2");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL3,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC03.gif","VOL LEVEL3");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL4,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC04.gif","VOL LEVEL4");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL5,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC05.gif","VOL LEVEL5");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL6,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC06.gif","VOL LEVEL6");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL7,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC07.gif","VOL LEVEL7");
#else
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL1,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC01.bmp","VOL LEVEL1");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL2,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC02.bmp","VOL LEVEL2");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL3,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC03.bmp","VOL LEVEL3");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL4,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC04.bmp","VOL LEVEL4");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL5,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC05.bmp","VOL LEVEL5");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL6,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC06.bmp","VOL LEVEL6");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL7,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_SC07.bmp","VOL LEVEL7");
#if 0
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL0,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/SC_Volume_Level_00.bmp","VOL LEVEL0");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL1,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/SC_Volume_Level_01.bmp","VOL LEVEL1");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL2,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/SC_Volume_Level_02.bmp","VOL LEVEL2");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL3,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/SC_Volume_Level_03.bmp","VOL LEVEL3");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL4,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/SC_Volume_Level_04.bmp","VOL LEVEL4");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL5,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/SC_Volume_Level_05.bmp","VOL LEVEL5");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL6,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/SC_Volume_Level_06.bmp","VOL LEVEL6");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL7,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/SC_Volume_Level_07.bmp","VOL LEVEL7");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL8,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/SC_Volume_Level_08.bmp","VOL LEVEL8");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_LEVEL9,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/SC_Volume_Level_09.bmp","VOL LEVEL9");	
#endif
#endif	
//PMT HIMANSHU START 20050729
#if defined(__MMI_TOUCH_SCREEN__)      /* 081206 volume control */
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_UP_UP_IMAGE_ID,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_up_up.pbm","Volume up button normal image");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_UP_DOWN_IMAGE_ID,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_up_down.pbm","Volume up button pressed image");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_DOWN_UP_IMAGE_ID,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_down_up.pbm","Volume down button normal image");
	ADD_APPLICATION_IMAGE2(WGUI_VOLUME_DOWN_DOWN_IMAGE_ID,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_down_down.pbm","Volume down button pressed image");
#endif
//PMT HIMANSHU END 20050729
	/*code ended vandana mehtani*/
	ADD_APPLICATION_IMAGE2(WGUI_SLIDE_CONTROL_BG_IMAGE_ID,CUST_IMG_PATH"/MainLCD/UIElement/Valuemap/MAP.BMP","Background for Slide Control");
	ADD_APPLICATION_IMAGE2(WGUI_SLIDE_CONTROL_SI_IMAGE_ID,CUST_IMG_PATH"/MainLCD/UIElement/Valuemap/INDICATE.BMP","Slide Indicator image for Slide Control");

	ADD_APPLICATION_IMAGE2(WGUI_INPUT_INFORMATION_ICON1,CUST_IMG_PATH"/MainLCD/Edit/MUT.BMP","Icon for Multitap input mode");
//For dialing screen
	//start vijay 20050203
#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
#if defined(__MMI_MAINLCD_176X220__)		//for 176X220 image's character.
	ADD_APPLICATION_IMAGE2(IMG_DIALING_SCREEN,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/DUALMODE_TH_DIALING_SCREEN.gif","Image of Dialing Screen background");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_0,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY0.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_1,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY1.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_2,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY2.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_3,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY3.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_4,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY4.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_5,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY5.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_6,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY6.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_7,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY7.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_8,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY8.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_9,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY9.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_HASH,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY_HASH.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_STAR,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY_STAR.bmp","Image of Dialing Key");
#else
	ADD_APPLICATION_IMAGE2(IMG_DIALING_SCREEN,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_SCREEN.gif","Image of Dialing Screen background");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_0,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY0.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_1,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY1.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_2,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY2.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_3,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY3.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_4,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY4.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_5,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY5.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_6,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY6.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_7,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY7.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_8,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY8.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_9,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY9.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_HASH,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY_HASH.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_STAR,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_KEY_STAR.gif","Image of Dialing Key");
#endif
#else   /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */
#ifdef __MMI_MAINLCD_240X320__
	ADD_APPLICATION_IMAGE2(IMG_DIALING_SCREEN,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_SCREEN.GIF","Image of Dialing Screen");
#else
	ADD_APPLICATION_IMAGE2(IMG_DIALING_SCREEN,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_SCREEN.BMP","Image of Dialing Screen");
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_0,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_KEY0.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_1,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_KEY1.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_2,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_KEY2.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_3,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_KEY3.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_4,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_KEY4.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_5,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_KEY5.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_6,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_KEY6.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_7,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_KEY7.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_8,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_KEY8.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_9,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_KEY9.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_HASH,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_KEY_HASH.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_STAR,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_KEY_STAR.BMP","Image of Dialing Key");
#endif  /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */
/* 053005 Calvin added for function key */
#if defined (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
#ifdef __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__
#if defined(__MMI_MAINLCD_176X220__)		//for 176X220 image's character.
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_PHB,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/DUALMODE_TH_DIALING_PHONEBOOK.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_CALL,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/DUALMODE_TH_DIALING_CALL_SLAVE.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_CALL_SLAVE,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/DUALMODE_TH_DIALING_CALL.bmp","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_PHB_DISABLED,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_PHONEBOOK_DISABLE.bmp","Image of Disabled Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_CALL_DISABLED,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/DUALMODE_TH_DIALING_CALL_DISABLED.bmp","Image of Disabled Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_CALL_SLAVE_DISABLED,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/DUALMODE_TH_DIALING_CALL_SLAVE_DISABLED.bmp","Image of Dialing Key");
#else
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_PHB,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_PHONEBOOK.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_CALL,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_CALL.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_CALL_SLAVE,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/DUALMODE_TH_DIALING_CALL_SLAVE.gif","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_PHB_DISABLED,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_PHONEBOOK_DISABLE.gif","Image of Disabled Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_CALL_DISABLED,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/TH_DIALING_CALL_DISABLE.gif","Image of Disabled Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_CALL_SLAVE_DISABLED,CUST_IMG_PATH"/MainLCD/DialingScreen/DualSIM/DUALMODE_TH_DIALING_CALL_SLAVE_DISABLE.gif","Image of Dialing Key");
#endif
#else   /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_PHB,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_PHONEBOOK.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_CALL,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_CALL.BMP","Image of Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_PHB_DISABLED,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_PHONEBOOK_DISABLED.BMP","Image of Disabled Dialing Key");
	ADD_APPLICATION_IMAGE2(IMG_DIALING_KEY_CALL_DISABLED,CUST_IMG_PATH"/MainLCD/DialingScreen/TH_DIALING_CALL_DISABLED.BMP","Image of Disabled Dialing Key");
#endif  /* __DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__ */
	
#endif
/* Calvin end */

#ifdef __MMI_SCREEN_ROTATE__
	ADD_APPLICATION_IMAGE2(IMG_SCREEN_ROTATE_TITLE,CUST_IMG_PATH"/MainLCD/UIElement/Horizontal/Rotated_title.bmp","Image of rotated title");
	ADD_APPLICATION_IMAGE2(IMG_SCREEN_ROTATE_BACKGROUND,CUST_IMG_PATH"/MainLCD/UIElement/Horizontal/Rotated_bkgrnd.gif","Image of rotated background");	
#endif 

#ifdef __MMI_TOUCH_SCREEN__
	/* Wait long-press animation */
	ADD_APPLICATION_IMAGE2(IMG_PEN_WAITING_LONGPRESS,CUST_IMG_PATH"/MainLCD/UIElement/Touch/TH_LongPress.gif","Animation when pen is long-tapped");		

	/* Virtual keyboard */
	ADD_APPLICATION_IMAGE2(IMG_VKBD_STANDARD,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_STANDARD.GIF","Standard keyboard");	
	ADD_APPLICATION_IMAGE2(IMG_VKBD_EXTENDED,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_EXTENDED.GIF","Extended keyboard");	
	ADD_APPLICATION_IMAGE2(IMG_VKBD_TRAY,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_TRAY.GIF","Minimized keyboard ");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_NUMBER,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_NUM.GIF","Number keyboard");
#ifdef __MMI_LANG_SM_CHINESE__
	ADD_APPLICATION_IMAGE2(IMG_VKBD_PINYIN,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_PINYIN.GIF","Pinyin keyboard");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_PINYIN_DISPAREA,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_PINYIN_DISPAREA.BMP","Display area of Pinyin keyboard");
#endif /* __MMI_LANG_SM_CHINESE__ */
#ifdef __MMI_TR_CHINESE_VK__
	ADD_APPLICATION_IMAGE2(IMG_VKBD_BOPOMO,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_BOPOMO.GIF","Bopomo keyboard");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_BOPOMO_DISPAREA,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_BOPOMO_DISPAREA.BMP","Display area of Bopomo keyboard");
#endif /* __MMI_TR_CHINESE_VK__ */
	ADD_APPLICATION_IMAGE2(IMG_VKBD_CUSTOM_HIDE,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_CUSTOM_HIDE.BMP","Custom key - Hide");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_CUSTOM_SHOW,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_CUSTOM_SHOW.BMP","Custom key - Show");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_CUSTOM_SWITCH_KEYBOARD,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_CUSTOM_KEYBOARD.BMP","Custom key - switch to big keyboard");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_CUSTOM_SWITCH_HANDWRITING,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_CUSTOM_HAND.BMP","Custom key - switch to multi-block handwriting");	
	ADD_APPLICATION_IMAGE2(IMG_VKBD_CUSTOM_BACKSPACE,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_CUSTOM_BAKSPACE.BMP","Custom key - Backspace");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_CUSTOM_CAPSLOCK,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_CUSTOM_CAPS.BMP","Custom key - Caps lock");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_CUSTOM_SPACE,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_CUSTOM_SPACE.BMP","Custom key - Space");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_CUSTOM_SYMBPICKER,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_CUSTOM_SYMBPICKER.BMP","Custom key - symbol picker");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_CUSTOM_EUROSYMB,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_CUSTOM_EUROSYMB.BMP","Custom key - european symbol picker");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_CUSTOM_ENTER,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_CUSTOM_ENTER.BMP","Custom key - Enter");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_CUSTOM_BRACKET,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_CUSTOM_BRACKET.BMP","Custom key - Bracket");
	ADD_APPLICATION_IMAGE2(IMG_VKBD_CUSTOM_HIDE_SMALL,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_CUSTOM_HAND_SMALL.BMP","Custom key - Hide");
	#ifdef __MMI_ARABIC_VK__
	ADD_APPLICATION_IMAGE2(IMG_VKBD_ARABIC,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_ARABIC.bmp","Arabic keyboard");
	#endif
	#if (defined(__MMI_PERSIAN_VK__)|| defined(__MMI_ARABIC_VK__))
 	ADD_APPLICATION_IMAGE2(IMG_VKBD_FARSI_HUNGARIAN_ARABICSYMBOL,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_HUNGARIAN_FARSI.bmp","Hungarian_Farsi keyboard");
	#endif
	#if defined (__MMI_VIETNAMESE_VK__)||defined(__MMI_HINDI_VK__)
	ADD_APPLICATION_IMAGE2(IMG_VKBD_VIETNAMESE,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_VIETNAMESE.bmp","Vietnamese keyboard");
	#endif
	#ifdef __MMI_THAI_VK__
	ADD_APPLICATION_IMAGE2(IMG_VKBD_THAI,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_THAI.bmp","Thai keyboard");
	#endif
	#ifdef __MMI_PERSIAN_VK__
	ADD_APPLICATION_IMAGE2(IMG_VKBD_FARSI_SYMBOL,CUST_IMG_PATH"/MainLCD/UIElement/VirtualKeyboard/V_KBD_FARSI_SYMBOL.gif","Persian symbol keyboard");
	#endif
#endif /* __MMI_TOUCH_SCREEN__ */

#ifdef UI_SCROLLBAR_STYLE_3
	ADD_APPLICATION_IMAGE2(IMG_SCROLLBAR_BUTTON_UP,CUST_IMG_PATH"/MainLCD/UIElement/Scrollbar/button_up.pbm", "Scrollbar - ");
	ADD_APPLICATION_IMAGE2(IMG_SCROLLBAR_BUTTON_DOWN,CUST_IMG_PATH"/MainLCD/UIElement/Scrollbar/button_down.pbm", "Scrollbar - ");
	ADD_APPLICATION_IMAGE2(IMG_SCROLLBAR_V_BAR_TOP,CUST_IMG_PATH"/MainLCD/UIElement/Scrollbar/v_bar_top.pbm", "Scrollbar - ");
	ADD_APPLICATION_IMAGE2(IMG_SCROLLBAR_V_BAR_MIDDLE,CUST_IMG_PATH"/MainLCD/UIElement/Scrollbar/v_bar_middle.pbm", "Scrollbar - ");
	ADD_APPLICATION_IMAGE2(IMG_SCROLLBAR_V_BAR_BOTTOM,CUST_IMG_PATH"/MainLCD/UIElement/Scrollbar/v_bar_bottom.pbm", "Scrollbar - ");
	ADD_APPLICATION_IMAGE2(IMG_SCROLLBAR_H_BAR_LEFT,CUST_IMG_PATH"/MainLCD/UIElement/Scrollbar/h_bar_left.pbm", "Scrollbar - ");
	ADD_APPLICATION_IMAGE2(IMG_SCROLLBAR_H_BAR_MIDDLE,CUST_IMG_PATH"/MainLCD/UIElement/Scrollbar/h_bar_middle.pbm", "Scrollbar - ");
	ADD_APPLICATION_IMAGE2(IMG_SCROLLBAR_H_BAR_RIGHT,CUST_IMG_PATH"/MainLCD/UIElement/Scrollbar/h_bar_right.pbm", "Scrollbar - ");	
#endif /* UI_SCROLLBAR_STYLE_3 */

#if defined(__MMI_ZI__)
	ADD_APPLICATION_IMAGE2(WGUI_ZI_EZITEXT_LOGO,CUST_IMG_PATH"/MainLCD/Edit/eZiText_N12px_col.bmp","Icon for eZiText");
	ADD_APPLICATION_IMAGE2(WGUI_ZI_EZITEXT_BPMF_LOGO,CUST_IMG_PATH"/MainLCD/Edit/TC_eZiText_Bopomofo_N12px_col.bmp","Icon for eZiText BPMF");
	ADD_APPLICATION_IMAGE2(WGUI_ZI_EZITEXT_PINYIN_LOGO,CUST_IMG_PATH"/MainLCD/Edit/SC_eZiText_Pinyin_N12px_col.bmp","Icon for eZiText Pinyin");
	ADD_APPLICATION_IMAGE2(WGUI_ZI_EZITEXT_SM_STROKE_LOGO,CUST_IMG_PATH"/MainLCD/Edit/SC_eZiText_Stroke_N12px_col.bmp","Icon for eZiText SC Stroke");
	ADD_APPLICATION_IMAGE2(WGUI_ZI_EZITEXT_TR_STROKE_LOGO,CUST_IMG_PATH"/MainLCD/Edit/TC_eZiText_Stroke_N12px_col.bmp","Icon for eZiText TC Stroke");
#elif defined(__MMI_T9__)||defined(__MMI_ITAP__)||defined(__MMI_KA__)
	ADD_APPLICATION_IMAGE2(WGUI_IME_MULTITAP_BPMF_IMG,CUST_IMG_PATH"/MainLCD/Edit/Multitap BPMF.bmp","Icon for T9 multitap BPMF");
	ADD_APPLICATION_IMAGE2(WGUI_IME_MULTITAP_PINYIN_IMG,CUST_IMG_PATH"/MainLCD/Edit/Multiatp Pinyin.bmp","Icon for T9 multitap pinyin");
	ADD_APPLICATION_IMAGE2(WGUI_IME_SMART_BPMF_IMG,CUST_IMG_PATH"/MainLCD/Edit/Smart BPMF.bmp","Icon for T9 smart BPMF");
	ADD_APPLICATION_IMAGE2(WGUI_IME_SMART_PINYIN_IMG,CUST_IMG_PATH"/MainLCD/Edit/Smart Pinyin.bmp","Icon for T9 smart pinyin");
	ADD_APPLICATION_IMAGE2(WGUI_IME_SM_STROKE_IMG,CUST_IMG_PATH"/MainLCD/Edit/Simplified Chinese stroke.bmp","Icon for T9 SC Stroke");
	ADD_APPLICATION_IMAGE2(WGUI_IME_TR_STROKE_IMG,CUST_IMG_PATH"/MainLCD/Edit/Traditonal Chinese stroke.bmp","Icon for T9 TC Stroke");
	ADD_APPLICATION_IMAGE2(WGUI_IME_SMART_ENGLISH_IMG,CUST_IMG_PATH"/MainLCD/Edit/Smartamb.bmp","Icon for T9 smart English");
#endif

	ADD_APPLICATION_IMAGE2 (CHAT_LOCAL_IMG, CUST_IMG_PATH"/MainLCD/Chat/CT_IN.PBM", "CHAT app Menu item OUT common Image");	
	ADD_APPLICATION_IMAGE2 (CHAT_REMOTE_IMG, CUST_IMG_PATH"/MainLCD/Chat/CT_OUT.PBM", "CHAT app Menu item IN common Image");	

	ADD_APPLICATION_IMAGE2(IMG_H_SELECT_LEFT,CUST_IMG_PATH"/MainLCD/UIElement/Arrow/SELECT_LEFT.pbm", "Left arrow in horizontal select");
	ADD_APPLICATION_IMAGE2(IMG_H_SELECT_RIGHT,CUST_IMG_PATH"/MainLCD/UIElement/Arrow/SELECT_RIGHT.pbm", "Right arrow in horizontal select");
	ADD_APPLICATION_IMAGE2 (RIGHT_RED_ARROW, CUST_IMG_PATH"/MainLCD/UIElement/Arrow/PV_ARR.BMP","right arrow");
	ADD_APPLICATION_IMAGE2 (LEFT_RED_ARROW, CUST_IMG_PATH"/MainLCD/UIElement/Arrow/PV_ARL.BMP","left arrow");

#if defined(__MMI_MAINLCD_240X320__)//052206 NITZ category Calvin
#if 0
	ADD_APPLICATION_IMAGE2 (LEFT_ARROW_DOWN, CUST_IMG_PATH"/MainLCD/UIElement/Arrow/LEFT_ARROW_DOWN.pbm","LEFT_ARROW_DOWN");
	ADD_APPLICATION_IMAGE2 (LEFT_ARROW_UP, CUST_IMG_PATH"/MainLCD/UIElement/Arrow/LEFT_ARROW_UP.pbm","LEFT_ARROW_UP");
	ADD_APPLICATION_IMAGE2 (RIGHT_ARROW_DOWN, CUST_IMG_PATH"/MainLCD/UIElement/Arrow/RIGHT_ARROW_DOWN.pbm","RIGHT_ARROW_DOWN");
	ADD_APPLICATION_IMAGE2 (RIGHT_ARROW_UP, CUST_IMG_PATH"/MainLCD/UIElement/Arrow/RIGHT_ARROW_UP.pbm","RIGHT_ARROW_UP");
	#endif
#endif

#ifndef __MMI_TOUCH_DIAL_SCREEN__
#if defined( __MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_240X320__) \
	||defined( __MMI_MAINLCD_220X176__)||defined( __MMI_MAINLCD_128X160__)||defined( __MMI_MAINLCD_128X128__)//041205 Calvin modified
	ADD_APPLICATION_IMAGE2 (IMG_POPUP_TITLE, CUST_IMG_PATH"/MainLCD/WarningPage/PopUpPage/PP_TB.BMP","Background image of popup screen");
#endif
#endif

//PMT VIKAS START 20050707 	
#ifdef __MMI_UI_DALMATIAN_EDITOR__
	ADD_APPLICATION_IMAGE2 (IMG_EDITOR_DALMATIAN,
								CUST_IMG_PATH"/MainLCD/Edit/Dalmatian/EDITOR_DALMATIAN_IMAGE.gif",
								"Image for editor");
	ADD_APPLICATION_IMAGE2 (IMG_EDITOR_DALMATIAN_SMALL,
								CUST_IMG_PATH"/MainLCD/Edit/Dalmatian/EDITOR_DALMATIAN_IMAGE_SMALL.gif",
								"Image for editor");
#endif
#ifdef __MMI_UI_DALMATIAN_MAINMENU__
	ADD_APPLICATION_IMAGE2(MAIN_MENU_DALMATIAN_BALL, CUST_IMG_PATH"/MainLCD/MainMenu/PAGE/Dalmatian/MM_BALL.gif","Main Menu Ball Animation");

#endif
//PMT VIKAS END 20050707
#ifdef UI_SOFTKEY_STYLE_2
#if	defined (__MMI_MAINLCD_128X160__) || defined (__MMI_MAINLCD_128X128__)
	ADD_APPLICATION_IMAGE2 (IMG_SOFTKEY_BAR, CUST_IMG_PATH"/MainLCD/UIElement/softkey/softkey.gif","Background image of SOFTKEY");
#else
#ifdef __PRODUCT_QPAD__
	ADD_APPLICATION_IMAGE2 (IMG_SOFTKEY_BAR, CUST_IMG_PATH"/MainLCD/UIElement/softkey/softkey_1.bmp","Background image of SOFTKEY");
#else
	ADD_APPLICATION_IMAGE2 (IMG_SOFTKEY_BAR, CUST_IMG_PATH"/MainLCD/UIElement/softkey/softkey.bmp","Background image of SOFTKEY");
#endif
#endif
#endif
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
	ADD_APPLICATION_STRING2 (STR_GLOBAL_LOGO, "TCL","TCL");
	ADD_APPLICATION_IMAGE2 (IMG_SUBLCD_ANALOG_CLOCK, CUST_IMG_PATH"/SubLCD/Active/SB_Analog.gif","Background image of sublcd analog clock");
#endif
#ifdef __MMI_EDITABLE_AUDIO_EQUALIZER__
	ADD_APPLICATION_IMAGE2(SOUND_EFFECT_AUDIO_EQ_BG, CUST_IMG_PATH"/MainLCD/UIElement/AudioEqualizer/EQ_BG.gif","Audio Equalizer Setting Editing Screen");
	ADD_APPLICATION_IMAGE2(SOUND_EFFECT_AUDIO_EQ_ACTIVE_BAR, CUST_IMG_PATH"/MainLCD/UIElement/AudioEqualizer/EQ_BAR.bmp","Activated Bar Image For Audio Equalizer Setting Editing Screen");
	ADD_APPLICATION_IMAGE2(SOUND_EFFECT_AUDIO_EQ_VAL_INDICATOR, CUST_IMG_PATH"/MainLCD/UIElement/AudioEqualizer/EQ_indicator.bmp","Level Indicator For Audio Equalizer Setting Editing Screen");
#endif
#ifdef __MMI_TOUCH_SCREEN__
	ADD_APPLICATION_IMAGE2 (IMG_PASSWD_KEYBOARD, CUST_IMG_PATH"/MainLCD/UIElement/Touch/pim.bmp","keyboard for password input");
	ADD_APPLICATION_IMAGE2 (IMG_PASSWD_KEYBOARD_PRESS, CUST_IMG_PATH"/MainLCD/UIElement/Touch/pim_h.bmp","pressed keyboard for password input");
#endif	
#if (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_WGUI_CSK_ENABLE__)
	ADD_APPLICATION_IMAGE2(IMG_CSK_DIAL_ICON,CUST_IMG_PATH"/MainLCD/Call/Touch/Call.gif","icons used in csk to make call");
#endif

#ifdef __MMI_CALENDAR__
//PMT VIKAS START 20050913
#ifdef __MMI_UI_DALMATIAN_CALENDAR__ 
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_JAN,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/JAN.BMP","BackGround image JAN.");
#ifndef __MMI_SLIM_CALENDAR__
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_FEB,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/FEB.bmp","BackGround image FEB.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_MAR,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/MARCH.bmp","BackGround image MAR.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_APR,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/APRIL.bmp","BackGround image APR.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_MAY,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/MAY.bmp","BackGround image MAY.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_JUN,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/JUNE.bmp","BackGround image JUN.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_JUL,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/JULY.bmp","BackGround image JUL.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_AUG,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/AUG.bmp","BackGround image AUG.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_SEP,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/SEP.bmp","BackGround image SEP.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_OCT,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/OCT.bmp","BackGround image OCT.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_NOV,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/NOV.bmp","BackGround image NOV.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_DEC,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/DEC.bmp","BackGround image DEC.");
#endif /* ~__MMI_SLIM_CALENDAR__ */
#else
//PMT VIKAS END 20050913
	#ifdef __MMI_UI_INTEGRATED_CALENDAR_SUPPORT__
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_JAN,CUST_IMG_PATH"/MainLCD/Calendar/new_bg.BMP","BackGround image JAN.");//072406 new calendar
	#else
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_JAN,CUST_IMG_PATH"/MainLCD/Calendar/JAN.bmp","BackGround image JAN.");
	#endif
#ifndef __MMI_SLIM_CALENDAR__
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_FEB,CUST_IMG_PATH"/MainLCD/Calendar/FEB.bmp","BackGround image FEB.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_MAR,CUST_IMG_PATH"/MainLCD/Calendar/MARCH.bmp","BackGround image MAR.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_APR,CUST_IMG_PATH"/MainLCD/Calendar/APRIL.bmp","BackGround image APR.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_MAY,CUST_IMG_PATH"/MainLCD/Calendar/MAY.bmp","BackGround image MAY.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_JUN,CUST_IMG_PATH"/MainLCD/Calendar/JUNE.bmp","BackGround image JUN.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_JUL,CUST_IMG_PATH"/MainLCD/Calendar/JULY.bmp","BackGround image JUL.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_AUG,CUST_IMG_PATH"/MainLCD/Calendar/AUG.bmp","BackGround image AUG.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_SEP,CUST_IMG_PATH"/MainLCD/Calendar/SEP.bmp","BackGround image SEP.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_OCT,CUST_IMG_PATH"/MainLCD/Calendar/OCT.bmp","BackGround image OCT.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_NOV,CUST_IMG_PATH"/MainLCD/Calendar/NOV.bmp","BackGround image NOV.");
	ADD_APPLICATION_IMAGE2(IMG_CALENDAR_DEC,CUST_IMG_PATH"/MainLCD/Calendar/DEC.bmp","BackGround image DEC.");
#endif /* ~__MMI_SLIM_CALENDAR__ */
//PMT VIKAS START 20050913
#endif
#ifdef __MMI_UI_DALMATIAN_CALENDAR__ 
	ADD_APPLICATION_IMAGE2(DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_JAN,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/WEEK/JAN.gif","Box image JAN.");
	ADD_APPLICATION_IMAGE2(DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_FEB,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/WEEK/FEB.gif","Box image FEB.");
	ADD_APPLICATION_IMAGE2(DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_MAR,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/WEEK/MARCH.gif","Box image MAR.");
	ADD_APPLICATION_IMAGE2(DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_APR,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/WEEK/APRIL.gif","Box image APR.");
	ADD_APPLICATION_IMAGE2(DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_MAY,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/WEEK/MAY.gif","Box image MAY.");
	ADD_APPLICATION_IMAGE2(DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_JUN,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/WEEK/JUNE.gif","Box image JUN.");
	ADD_APPLICATION_IMAGE2(DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_JUL,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/WEEK/JULY.gif","Box image JUL.");
	ADD_APPLICATION_IMAGE2(DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_AUG,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/WEEK/AUG.gif","Box image AUG.");
	ADD_APPLICATION_IMAGE2(DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_SEP,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/WEEK/SEP.gif","Box image SEP.");
	ADD_APPLICATION_IMAGE2(DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_OCT,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/WEEK/OCT.gif","Box image OCT.");
	ADD_APPLICATION_IMAGE2(DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_NOV,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/WEEK/NOV.gif","Box image NOV.");
	ADD_APPLICATION_IMAGE2(DALMATIAN_WEEK_HIGHLIGHTER_IMAGE_ID_DEC,CUST_IMG_PATH"/MainLCD/Calendar/DALMATIAN/WEEK/DEC.gif","Box image DEC.");
#endif
//PMT VIKAS END 20050913
	ADD_APPLICATION_IMAGE2(CAL_ENGLISH_DAYS_IMAGE,CUST_IMG_PATH"/MainLCD/Calendar/Week/week.bmp","Day of Week display");
	ADD_APPLICATION_IMAGE2(CAL_CHINESE_DAYS_IMAGE,CUST_IMG_PATH"/MainLCD/Calendar/Week/chinese week.bmp","Day of Week display");
#ifdef __MMI_TOUCH_SCREEN__
	ADD_APPLICATION_IMAGE2 (CAL_LEFT_ARROW_UP_IMAGE, CUST_IMG_PATH"/MainLCD/Calendar/Touch/Th_upL.bmp","calendar title left arrow");
	ADD_APPLICATION_IMAGE2 (CAL_LEFT_ARROW_DOWN_IMAGE, CUST_IMG_PATH"/MainLCD/Calendar/Touch/Th_downL.bmp","calendar title left arrow down");
	ADD_APPLICATION_IMAGE2 (CAL_RIGHT_ARROW_UP_IMAGE, CUST_IMG_PATH"/MainLCD/Calendar/Touch/Th_upR.bmp","calendar title right arrow");
	ADD_APPLICATION_IMAGE2 (CAL_RIGHT_ARROW_DOWN_IMAGE, CUST_IMG_PATH"/MainLCD/Calendar/Touch/Th_downR.bmp","calendar title right arrow down");
#endif /* __MMI_TOUCH_SCREEN__ */
#endif /*__MMI_CALENDAR__*/

/* 080905 Calvin added */
#ifdef __MMI_MAINLCD_240X320__
	ADD_APPLICATION_IMAGE2 (IMG_POPUP_BG, CUST_IMG_PATH"/MainLCD/WarningPage/PopUpPage/PP_BG.bmp","keyboard for password input");
#endif
/* Calvin end */

	ADD_APPLICATION_IMAGE2 (IMG_TITLEBAR_BG, CUST_IMG_PATH"/MainLCD/Titlebar/TB_BG_1.gif","keyboard for password input");//070505 Calvin should remove 
//PMT HIMANSHU START 20050625
#ifdef __MMI_UI_TECHNO_MAINMENU__
	ADD_APPLICATION_IMAGE2(MAIN_MENU_PAGE_BG, CUST_IMG_PATH"/MainLCD/MainMenu/PAGE/Techno/MM_BG.gif","Main Menu Background Animo Image");
	ADD_APPLICATION_IMAGE2(MAIN_MENU_PAGE_BG2, CUST_IMG_PATH"/MainLCD/MainMenu/PAGE/Techno/MM_BG2.gif","Main Menu Background Animo Image");
	ADD_APPLICATION_IMAGE2(MAIN_MENU_PAGE_BG3, CUST_IMG_PATH"/MainLCD/MainMenu/PAGE/Techno/MM_BG3.gif","Main Menu Background Animo Image");
	ADD_APPLICATION_IMAGE2(MAIN_MENU_PAGE_BG4, CUST_IMG_PATH"/MainLCD/MainMenu/PAGE/Techno/MM_BG4.gif","Main Menu Background Animo Image");
	ADD_APPLICATION_IMAGE2(MAIN_MENU_PAGE_BG5, CUST_IMG_PATH"/MainLCD/MainMenu/PAGE/Techno/MM_BG5.gif","Main Menu Background Animo Image");
#endif
//PMT HIMANSHU END

//PMT HIMANSHU START 20050721
#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
	ADD_APPLICATION_STRING2(STR_IDLESCREEN_SUNDAY, "Sunday", "String Sunday shown in Techno Idlescreen Bar");
	ADD_APPLICATION_STRING2(STR_IDLESCREEN_MONDAY, "Monday", "String Sunday shown in Techno Idlescreen Bar");
	ADD_APPLICATION_STRING2(STR_IDLESCREEN_TUESDAY, "Tuesday", "String Sunday shown in Techno Idlescreen Bar");
	ADD_APPLICATION_STRING2(STR_IDLESCREEN_WEDNESDAY, "Wednesday", "String Sunday shown in Techno Idlescreen Bar");
	ADD_APPLICATION_STRING2(STR_IDLESCREEN_THURSDAY, "Thursday", "String Sunday shown in Techno Idlescreen Bar");
	ADD_APPLICATION_STRING2(STR_IDLESCREEN_FRIDAY, "Friday", "String Sunday shown in Techno Idlescreen Bar");
	ADD_APPLICATION_STRING2(STR_IDLESCREEN_SATURDAY, "Saturday", "String Sunday shown in Techno Idlescreen Bar");
	/* 070306 Alpha layer Start */
	//ADD_APPLICATION_IMAGE2(IMG_TECHNO_IDLESCREEN_BAR, CUST_IMG_PATH"/MainLCD/IdleScreen/Techno/IDLESCREEN_BAR.gif", " Techno Idlescreen Bar Image");
    #ifdef __MMI_UI_TECHNO_STATUS_ICON__
	ADD_APPLICATION_IMAGE2(IMG_TECHNO_ANALOG_CLOCK, CUST_IMG_PATH"/MainLCD/IdleScreen/Techno/ANALOG_CLOCK_SHIFT.gif", " Techno Idlescreen Analog Clock Image");
	ADD_APPLICATION_IMAGE2(IMG_TECHNO_DIGITAL_CLOCK, CUST_IMG_PATH"/MainLCD/IdleScreen/Techno/DIGITAL_CLOCK_SHIFT.gif", " Techno Idlescreen Digital Clock Image");
    #else
	ADD_APPLICATION_IMAGE2(IMG_TECHNO_ANALOG_CLOCK, CUST_IMG_PATH"/MainLCD/IdleScreen/Techno/ANALOG_CLOCK.gif", " Techno Idlescreen Analog Clock Image");
	ADD_APPLICATION_IMAGE2(IMG_TECHNO_DIGITAL_CLOCK, CUST_IMG_PATH"/MainLCD/IdleScreen/Techno/DIGITAL_CLOCK.gif", " Techno Idlescreen Digital Clock Image");
    #endif
	/* 070306 Alpha layer End */
#endif
//PMT HIMANSHU END 20050721

//PMT HIMANSHU START 20050815
#if defined(__MMI_DRAW_MANAGER__)
	ADD_APPLICATION_IMAGE2(IMAGE_VIEW_TITLEBAR_BG, CUST_IMG_PATH"/MainLCD/Titlebar/TB_BG_1.gif","Title bar of Wallpaper and ScreenSaver Image View Screen");
#endif
	ADD_APPLICATION_IMAGE2(IMG_FLEXIBLE_TITLEBAR_BG, CUST_IMG_PATH"/MainLCD/Titlebar/TB_BG_1.gif","Title bar for flexible size");//093006 popup title
//PMT HIMANSHU END 20050815
//PMT NEERAJ START 20050825
#ifdef __MMI_UI_TWO_LINE_MENUITEM_STYLES__
	ADD_APPLICATION_IMAGE2(TWO_LINE_VOLUME_IMAGE,CUST_IMG_PATH"/MainLCD/UIElement/SlideControls/V_INLINE.gif","Inline Volume Image");
#endif
//PMT NEERAJ END 20050825

	ADD_APPLICATION_IMAGE2(IMG_HORIZONTAL_NORMAL_TAB, CUST_IMG_PATH"/MainLCD/UIElement/TabBar/TAB_NORMAL.GIF", "normal tab");
	ADD_APPLICATION_IMAGE2(IMG_HORIZONTAL_FOCUSED_TAB, CUST_IMG_PATH"/MainLCD/UIElement/TabBar/TAB_FOCUSED.GIF", "focused tab");
	ADD_APPLICATION_IMAGE2(IMG_HORIZONTAL_BLINKING_TAB, CUST_IMG_PATH"/MainLCD/UIElement/TabBar/TAB_BLINKING.GIF", "blinking tab");
	ADD_APPLICATION_IMAGE2(IMG_HORIZONTAL_TAB_L_ARROW, CUST_IMG_PATH"/MainLCD/UIElement/TabBar/TAB_ARROW_L.GIF", "right arrow on tab");
	ADD_APPLICATION_IMAGE2(IMG_HORIZONTAL_TAB_R_ARROW, CUST_IMG_PATH"/MainLCD/UIElement/TabBar/TAB_ARROW_R.GIF", "left arrow on tab");
	ADD_APPLICATION_IMAGE2(IMG_HORIZONTAL_TAB_HINT_AREA, CUST_IMG_PATH"/MainLCD/UIElement/TabBar/TITLEBAR.GIF", "tab hint area");

	ADD_APPLICATION_IMAGE2(IMG_PERCENTAGE_BAR_LEFT_IMAGE, CUST_IMG_PATH"/MainLCD/UIElement/ProgressBar/load_left.pbm", "aa");
	ADD_APPLICATION_IMAGE2(IMG_PERCENTAGE_BAR_RIGHT_IMAGE, CUST_IMG_PATH"/MainLCD/UIElement/ProgressBar/load_right.pbm", "bb");
	ADD_APPLICATION_IMAGE2(IMG_PERCENTAGE_BAR_BG_IMAGE, CUST_IMG_PATH"/MainLCD/UIElement/ProgressBar/load_bg.pbm", "cc");

//    ADD_APPLICATION_IMAGE2(GUI_COLOR_SLIDER_ARROW_IMAGE_ID,CUST_IMG_PATH"/MainLCD/UIElement/ColorSlider/color_slider_arrow.pbm","Color Slider Arrow");
    //ADD_APPLICATION_IMAGE2(GUI_CLOCK_DOT_IMAGE_ID,CUST_IMG_PATH"/MainLCD/UIElement/Clock/analog_clock_dot.pbm","Analog Clock Dot");
    
#if(EMS_DISPLAY_SPECIAL_CHARACTERS)
    ADD_APPLICATION_IMAGE2(WGUI_CR_CHARACTER_IMAGE_ID,CUST_IMG_PATH"/MainLCD/Edit/EMSCR.BMP","EMS CR character display");
    ADD_APPLICATION_IMAGE2(WGUI_ESC_CHARACTER_IMAGE_ID,CUST_IMG_PATH"/MainLCD/Edit/EMSESC.BMP","EMS ESC character display");
    ADD_APPLICATION_IMAGE2(WGUI_LF_CHARACTER_IMAGE_ID,CUST_IMG_PATH"/MainLCD/Edit/EMSLF.BMP","EMS LF character display");
#endif
    
    PopulateUnifiedComposerEditorRes();
}

//PMT VIKAS START 20050720
#if defined( __MMI_UI_ANIMATED_TITLE_EFFECT__)||defined(__MMI_UI_LIST_HIGHLIGHT_EFFECTS__)
void PopulateTitleListAnimationResources(void)
{
      #ifdef __MMI_UI_ANIMATED_TITLE_EFFECT__
      ADD_APPLICATION_IMAGE2(IMG_TITLE_SCROLL,CUST_IMG_PATH"/MainLCD/UIElement/AnimationEffects/Title/AnimationTitle.gif", "Animation Image for title");
      #endif

       #ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
        ADD_APPLICATION_IMAGE2(IMG_LIST_HIGHLIGHTER,CUST_IMG_PATH"/MainLCD/UIElement/AnimationEffects/List/AnimationHighlighter.gif", "Highlighter animatian image");
      #endif
}
#endif
//PMT VIKAS END 20050720

//PMT NEERAJ START 20050616
#ifdef __MMI_UI_TAB_PANE__
void PopulateTabResources(void)
{	
	ADD_APPLICATION_IMAGE2(TAB_PANE_ICON_HIGHLIGHT,CUST_IMG_PATH"/MainLCD/UIElement/TabPane/icon_highlight.gif","Image for highlight Item");
	ADD_APPLICATION_IMAGE2(TAB_PANE_ICON_UN_HIGHLIGHT,CUST_IMG_PATH"/MainLCD/UIElement/TabPane/icon_normal.gif","Image for unhighlight Item");
	ADD_APPLICATION_IMAGE2(TAB_PANE_TEXT_HIGHLIGHT,CUST_IMG_PATH"/MainLCD/UIElement/TabPane/text_highlight.gif","Image for highlight Item");
	ADD_APPLICATION_IMAGE2(TAB_PANE_TEXT_UN_HIGHLIGHT,CUST_IMG_PATH"/MainLCD/UIElement/TabPane/text_normal.gif","Image for unhighlight Item");
}
#endif
//PMT NEERAJ END 20050616

void PopulateStatusIconResources(void)
{	


#ifdef __PRODUCT_QPAD__
	/*Signal*/
	ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/signal/SI_SIG","Status Icon: Signal strength");

#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
	ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_IDLE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/Techno/signal/SI_SIG_IDLE","Status Icon: Signal strength");
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */

#ifdef __UNI_MMI_TWO_SIM_CARD_SIGNAL_  /*UNI@lizongzheng_20070420 10:19:__UNI_MMI_TWO_SIM_CARD_SIGNAL_ 双卡两个信号条 */
       ADD_APPLICATION_IMAGE2( IMG_SI_SIGNAL_STRENGTH_CARD1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG_S1","Status Icon: Signal strength");
	ADD_APPLICATION_IMAGE2( IMG_SI_SIGNAL_STRENGTH_CARD2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG_S2","Status Icon: Signal strength");
#endif

	/*Line switch*/
#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__ 
	ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/signal/SI_SIG_L1","Status Icon: Signal strength with L1");
	ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/signal/SI_SIG_L2","Status Icon: Signal strength with L2");
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
	ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_IDLE_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/Techno/signal/SI_SIG_IDLE_L1","Status Icon: Signal strength with L1");
	ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_IDLE_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/Techno/signal/SI_SIG_IDLE_L2","Status Icon: Signal strength with L2");
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */
#else
	ADD_APPLICATION_IMAGE2(IMG_SI_LINE_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_L1.BMP","Status Icon: Line 1");
	ADD_APPLICATION_IMAGE2(IMG_SI_LINE_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_L2.BMP","Status Icon: Line 2");
#endif

	/*Battery*/
	ADD_APPLICATION_IMAGE2(IMG_SI_BATTERY_STRENGTH,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/battery/SI_BAT","Status Icon: Battery strength");

#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
	ADD_APPLICATION_IMAGE2(IMG_SI_BATTERY_STRENGTH_IDLE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/Techno/battery/SI_BAT_IDLE","Status Icon: Battery strength");
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */

#ifdef __MMI_VOIP__
	ADD_APPLICATION_IMAGE2(IMG_SI_VOIP,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_VOIP.BMP","Status Icon: VOIP");
#endif
	/*Keypad Lock/Unlock*/
	ADD_APPLICATION_IMAGE2(IMG_SI_KEYPAD_LOCK,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_LOCK.bmp","Status Icon: Keypad lock");
	ADD_APPLICATION_IMAGE2(IMG_SI_KEYPAD_UNLOCK,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_UNLOCK.BMP","Status Icon: Keypad lock");

	/*Profiles*/
	ADD_APPLICATION_IMAGE2(IMG_SI_VIBRATE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_VIB.bmp","Status Icon: Vibrator");
	ADD_APPLICATION_IMAGE2(IMG_SI_SILENT,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_SILENT.bmp","Status Icon: Silent");	
	ADD_APPLICATION_IMAGE2(IMG_SI_RING,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_RING.bmp","Status Icon: Ring");
	ADD_APPLICATION_IMAGE2(IMG_SI_VIBRATE_AND_RING,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_VR.BMP","Status Icon: Vibrate and ring");
	ADD_APPLICATION_IMAGE2(IMG_SI_VIBRATE_THEN_RING,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_VTR.BMP","Status Icon: Vibrate then ring");

	/*Alarm*/
	ADD_APPLICATION_IMAGE2(IMG_SI_ALARM_ACTIVATED,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_ALARM.bmp","Status Icon: Alarm");

	/*Roaming*/
	ADD_APPLICATION_IMAGE2(IMG_SI_ROAMING_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_RM.bmp","Status Icon: Roaming");

	/*Missed Call*/
	ADD_APPLICATION_IMAGE2(IMG_SI_MISSED_CALL_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_MCA.BMP","Status Icon: Missed Call");
	
#ifdef __DOUBLE_PHONE_MISSED_SIM2_CALL_ICON
#ifdef __UNI_TWIN_C__
      ADD_APPLICATION_IMAGE2(IMG_SI_MISSED_SIM2_CALL_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_MCA_C.BMP","Status Icon: Missed Call");
#else
       ADD_APPLICATION_IMAGE2(IMG_SI_MISSED_SIM2_CALL_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_MCA2.BMP","Status Icon: Missed Call");
#endif
#endif
	/*Gprs*/
	ADD_APPLICATION_IMAGE2(IMG_SI_GPRS_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_GPRS.BMP","Status Icon: GPRS");

	/*Earphone*/
#if (defined (__MMI_STATUS_ICON_EN_EARPHONE__) || defined(__MMI_BT_HP_SUPPORT__)  )
	ADD_APPLICATION_IMAGE2(IMG_SI_EARPHONE_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_EAR.bmp","Status Icon: EarPhone");
#endif

	/*MMS*/
//#ifdef __MMI_STATUS_ICON_MMS__
	ADD_APPLICATION_IMAGE2(IMG_SI_MMS_UNREAD_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_MMSU.BMP","Status Icon: EarPhone");
	ADD_APPLICATION_IMAGE2(IMG_SI_MMS_BUSY_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_MMSB.BMP","Status Icon: EarPhone");
//#endif


#ifdef __MMI_IRDA_SUPPORT__
     ADD_APPLICATION_IMAGE2(IMG_SI_IR_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_IR.gif","Status Icon: IR");
#endif


//Robin 1014 Bluetooth
#if defined(__MMI_BT_HP_SUPPORT__)  || defined(__MMI_BT_SUPPORT__)
     ADD_APPLICATION_IMAGE2(IMG_SI_BT,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_BT.bmp","Status Icon: BT");
     ADD_APPLICATION_IMAGE2(IMG_SI_BT_CON,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_BT_CON.bmp","Status Icon: BT");
     ADD_APPLICATION_IMAGE2(IMG_SI_BT_ND,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_BT_ND.bmp","Status Icon: BT");
#endif

#ifdef __MMI_POC__
     ADD_APPLICATION_IMAGE2(IMG_SI_POC,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_Walkt.bmp","Status Icon: PoC registration");
#endif

#ifdef __MMI_BG_SOUND_EFFECT__
     ADD_APPLICATION_IMAGE2(IMG_SI_BGSND,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_BGSND.bmp","Status Icon: background sound effect");
#endif

#ifdef __MMI_AUDIO_REVERB_EFFECT__
     ADD_APPLICATION_IMAGE2(IMG_SI_REVERBSND,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_AudReverb.bmp","Status Icon: audio reverb effect");
#endif

#ifdef __MMI_AUDIO_SURROUND_EFFECT__
     ADD_APPLICATION_IMAGE2(IMG_SI_SURROUNDSND,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_3DSound.bmp","Status Icon: audio surround effect");
#endif

#ifdef __MMI_HOMEZONE_STATUS_ICON__
     ADD_APPLICATION_IMAGE2(IMG_SI_HZONE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_HZONE.bmp","Status Icon: home zone");
     ADD_APPLICATION_IMAGE2(IMG_SI_CZONE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_CZONE.bmp","Status Icon: city zone");
#endif

#ifdef __CTM_SUPPORT__
     ADD_APPLICATION_IMAGE2(IMG_SI_TTY_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_TTY.bmp","Status Icon: TTY indication");
#endif


	ADD_APPLICATION_IMAGE2(IMG_SI_GPRS_ATT_NO_PDP_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_GPRSC.BMP","Status Icon: GPRS, attach without pdp");	
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_VOICE_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_VOML1.BMP","Status Icon: Unread Voice L1");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_VOICE_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_VOML2.BMP","Status Icon: Unread Voice L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_VOICE_L1L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_VOML12.BMP","Status Icon: Unread Voice L1+L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_FAX_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_FM1.BMP","Status Icon: Unread Fax L1");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_FAX_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_FM2.BMP","Status Icon: Unread Fax L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_FAX_L1L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_FM12.BMP","Status Icon: Unread Fax L1+L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_EMAIL_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_EM1.BMP","Status Icon: Unread Email L1");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_EMAIL_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_EM2.BMP","Status Icon: Unread Email L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_EMAIL_L1L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_EM12.BMP","Status Icon: Unread Email L1+L2");
	
	/*Sms*/
	ADD_APPLICATION_IMAGE2(IMG_SI_SMS_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_MSG.bmp","Status Icon: Message");

	/*Call Divert*/
	ADD_APPLICATION_IMAGE2(IMG_SI_CALL_DIVERT_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_FOR01.bmp","Status Icon: Call Divert L1");
	ADD_APPLICATION_IMAGE2(IMG_SI_CALL_DIVERT_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_FOR02.BMP","Status Icon: Call Divert L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_CALL_DIVERT_L1L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_FOR.BMP","Status Icon: Call Divert L1+L2");
	
	ADD_APPLICATION_IMAGE2(IMG_SI_VOICE_RECORD,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_REC.BMP","Status Icon: Voice record");
	ADD_APPLICATION_IMAGE2(IMG_SI_MUTE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_MUTE.BMP","Status Icon: Mute");
	
	ADD_APPLICATION_IMAGE2(IMG_BLINKINGICON_CHAT_ROOM1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_CHAT1.BMP","Chat Room1 Blinking Icon");
	ADD_APPLICATION_IMAGE2(IMG_BLINKINGICON_CHAT_ROOM2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_CHAT2.BMP","Chat Room2 Blinking Icon");
	ADD_APPLICATION_IMAGE2(IMG_SI_STOPWATCH,CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/SI_STWATCH.bmp","Image for Stopwatch Icon");
#ifdef __NATIVE_JAVA__
    ADD_APPLICATION_IMAGE2(IMG_SI_JAVA_PAUSE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_QQIM.png","Java pause");
#else
    ADD_APPLICATION_IMAGE2(IMG_SI_JAVA_PAUSE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_Jvlogo.bmp","Java pause");
#endif
	
	//ADD_APPLICATION_IMAGE2(IMG_SI_WAP,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_WMAL.bmp","Sub LCD Status Icon: Vibrate then Ring");
    	ADD_APPLICATION_IMAGE2(IMG_SI_WAP,CUST_IMG_PATH"/MainLCD/WAP/SW_UNREAD.pbm","Sub LCD Status Icon: Vibrate then Ring");//2010-4-14 used for wap_push_indicator

	ADD_APPLICATION_IMAGE2(IMG_SI_NETWORK_CIPHER_GSM,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_NCGSM.bmp","Network Ciphering for GSM");//021505 Calvin added
	ADD_APPLICATION_IMAGE2(IMG_SI_NETWORK_CIPHER_GPRS,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_NCGPRS.bmp","Network Ciphering for GPRS");//021505 Calvin added

   /* Max, for IMPS */
   #ifdef __MMI_IMPS__
	ADD_APPLICATION_IMAGE2 ( IMG_SI_IMPS_MSG, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/qpad/SI_IMPSMG.bmp", "Status icon in Idle Screen when new msg comes");
	ADD_APPLICATION_IMAGE2 ( IMG_SI_IMPS_LOGIN, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/qpad/SI_IMPS.bmp", "Status icon in Idle Screen when login server");
   #endif

#ifdef __MMI_WLAN_FEATURES__
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_DISCONNECT, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/wifi_disconnect.bmp","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_SCANNING, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/wifi_connecting.gif","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_CONNECT1, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/WiFi_connect_1.bmp","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_CONNECT2, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/WiFi_connect_2.bmp","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_CONNECT3, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/WiFi_connect_3.bmp","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_CONNECT4, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/WiFi_connect_4.bmp","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_CONNECT5, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/WiFi_connect_5.bmp","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_CONNECT6, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/WiFi_connect_6.bmp","Image of WLAN");
#endif

#ifdef __MMI_SUBLCD__
	ADD_APPLICATION_IMAGE2(IMG_SLSI_SILENT,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_SIL.BMP","Sub LCD Status Icon: Silent");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_SIGNAL_STRENGTH,CUST_IMG_PATH"/SubLCD/StatusIcons/signal/SB_SIG","Sub LCD Status Icon: Signal strength");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_BATTERY_STRENGTH,CUST_IMG_PATH"/SubLCD/StatusIcons/battery/SB_BAT","Sub LCD Status Icon: Battery strength");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_SMS_INDICATOR,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_MG.BMP","Sub LCD Status Icon: Message");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_RING,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_RING.BMP","Sub LCD Status Icon: Ring");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_VIBRATE,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_VIB.BMP","Sub LCD Status Icon: Vibrator");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_VIBRATE_AND_RING,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_VR.BMP","Sub LCD Status Icon: Vibrate and Ring");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_VIBRATE_THEN_RING,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_VTR.BMP","Sub LCD Status Icon: Vibrate then Ring");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_ALARM,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_ALARM.BMP","Sub LCD Status Icon: Alarm");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_MISSED_CALL,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_MCA.BMP","Sub LCD Status Icon: Missed Call");
#endif
#if (defined __UNI_TWIN_A_IDLE__)  /*UNI@baiyanjun_20070929 12:43:48 双待机信号条*/
  #if (defined __UNI_TWIN_C__) /*UNI@baiyanjun_20080326 18:01:52 G+C: 信号条修改   */
      ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_TWIN_ALL ,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG_GC","Status Icon: Signal strength");
  #else
      ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_TWIN_ALL ,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG_SA","Status Icon: Signal strength");
  #endif 
#endif

       /*Fix Bug#12707 for cs2324c by cong.li on 2009.07.02. */
	ADD_APPLICATION_IMAGE2(IMG_SLSI_USB_CONNECTED,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/qpad/SI_USB.bmp","Status Icon: usb connected");

#ifdef __MMI_UI_TAB_PANE__
	PopulateTabResources();
#endif

#else /*__PRODUCT_QPAD__*/
	//ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG.GIF","Status Icon: Signal strength");
	ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG","Status Icon: Signal strength");
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
	ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_IDLE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/Techno/signal/SI_SIG_IDLE","Status Icon: Signal strength");
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */
#ifdef __UNI_MMI_TWO_SIM_CARD_SIGNAL_  /*UNI@lizongzheng_20070420 10:19:__UNI_MMI_TWO_SIM_CARD_SIGNAL_ 双卡两个信号条 */
       ADD_APPLICATION_IMAGE2( IMG_SI_SIGNAL_STRENGTH_CARD1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG_S1","Status Icon: Signal strength");
	ADD_APPLICATION_IMAGE2( IMG_SI_SIGNAL_STRENGTH_CARD2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG_S2","Status Icon: Signal strength");
#endif
#ifdef __MMI_STATUS_ICON_SIGNAL_WITH_LINE_SWITCH__ //022505 Calvin added
	//ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG_L1.GIF","Status Icon: Signal strength with L1");
	//ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG_L2.GIF","Status Icon: Signal strength with L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG_L1","Status Icon: Signal strength with L1");
	ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG_L2","Status Icon: Signal strength with L2");
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
	ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_IDLE_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/Techno/signal/SI_SIG_IDLE_L1","Status Icon: Signal strength with L1");
	ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_IDLE_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/Techno/signal/SI_SIG_IDLE_L2","Status Icon: Signal strength with L2");
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */
#else
	ADD_APPLICATION_IMAGE2(IMG_SI_LINE_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_L1.BMP","Status Icon: Line 1");
	ADD_APPLICATION_IMAGE2(IMG_SI_LINE_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_L2.BMP","Status Icon: Line 2");
#endif
	//ADD_APPLICATION_IMAGE2(IMG_SI_BATTERY_STRENGTH,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/battery/SI_BAT.GIF","Status Icon: Battery strength");
	ADD_APPLICATION_IMAGE2(IMG_SI_BATTERY_STRENGTH,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/battery/SI_BAT","Status Icon: Battery strength");
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
	ADD_APPLICATION_IMAGE2(IMG_SI_BATTERY_STRENGTH_IDLE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/Techno/battery/SI_BAT_IDLE","Status Icon: Battery strength");
#endif /* __MMI_UI_TECHNO_STATUS_ICON__ */
//	ADD_APPLICATION_IMAGE2(IMG_SI_3G,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_3G.BMP","Status Icon: WCDMA");
	//ADD_APPLICATION_IMAGE2(IMG_SI_2G,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_2G.BMP","Status Icon: GSM_GPRS");
	//ADD_APPLICATION_IMAGE2(IMG_SI_ANY,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_Any.BMP","Status Icon: Dual Mode");
#ifdef __MMI_VOIP__//091806 voip icon
	ADD_APPLICATION_IMAGE2(IMG_SI_VOIP,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_VOIP.BMP","Status Icon: VOIP");
#endif
	ADD_APPLICATION_IMAGE2(IMG_SI_KEYPAD_LOCK,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_LOCK.BMP","Status Icon: Keypad lock");
	ADD_APPLICATION_IMAGE2(IMG_SI_KEYPAD_UNLOCK,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_UNLOCK.BMP","Status Icon: Keypad lock");
	ADD_APPLICATION_IMAGE2(IMG_SI_VIBRATE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_VIB.BMP","Status Icon: Vibrator");
	ADD_APPLICATION_IMAGE2(IMG_SI_SILENT,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_SILENT.BMP","Status Icon: Silent");
	ADD_APPLICATION_IMAGE2(IMG_SI_RING,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_RING.BMP","Status Icon: Ring");
	ADD_APPLICATION_IMAGE2(IMG_SI_VIBRATE_AND_RING,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_VR.BMP","Status Icon: Vibrate and ring");
	ADD_APPLICATION_IMAGE2(IMG_SI_VIBRATE_THEN_RING,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_VTR.BMP","Status Icon: Vibrate then ring");
	ADD_APPLICATION_IMAGE2(IMG_SI_ALARM_ACTIVATED,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_ALARM.BMP","Status Icon: Alarm");
	ADD_APPLICATION_IMAGE2(IMG_SI_ROAMING_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_RM.BMP","Status Icon: Roaming");
	ADD_APPLICATION_IMAGE2(IMG_SI_MISSED_CALL_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_MCA.BMP","Status Icon: Missed Call");
#ifdef __DOUBLE_PHONE_MISSED_SIM2_CALL_ICON
#ifdef __UNI_TWIN_C__
      ADD_APPLICATION_IMAGE2(IMG_SI_MISSED_SIM2_CALL_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_MCA_C.BMP","Status Icon: Missed Call");
#else
       ADD_APPLICATION_IMAGE2(IMG_SI_MISSED_SIM2_CALL_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_MCA2.BMP","Status Icon: Missed Call");
#endif
#endif
	ADD_APPLICATION_IMAGE2(IMG_SI_GPRS_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_GPRS.BMP","Status Icon: GPRS");
#if (defined (__MMI_STATUS_ICON_EN_EARPHONE__) || defined(__MMI_BT_HP_SUPPORT__)  )
	ADD_APPLICATION_IMAGE2(IMG_SI_EARPHONE_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_EAR.BMP","Status Icon: EarPhone");
#endif

//2009-6-9 for MMS indicator
//#ifdef __MMI_STATUS_ICON_MMS__
	ADD_APPLICATION_IMAGE2(IMG_SI_MMS_UNREAD_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_MMSU.BMP","Status Icon: EarPhone");
	ADD_APPLICATION_IMAGE2(IMG_SI_MMS_BUSY_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_MMSB.BMP","Status Icon: EarPhone");
//#endif


#ifdef __MMI_IRDA_SUPPORT__
     ADD_APPLICATION_IMAGE2(IMG_SI_IR_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_IR.gif","Status Icon: IR");
#endif


//Robin 1014 Bluetooth
#if defined(__MMI_BT_HP_SUPPORT__)  || defined(__MMI_BT_SUPPORT__)
     ADD_APPLICATION_IMAGE2(IMG_SI_BT,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_BT.bmp","Status Icon: BT");
     ADD_APPLICATION_IMAGE2(IMG_SI_BT_CON,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_BT_CON.bmp","Status Icon: BT");
     ADD_APPLICATION_IMAGE2(IMG_SI_BT_ND,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_BT_ND.bmp","Status Icon: BT");
#endif

#ifdef __MMI_POC__
     ADD_APPLICATION_IMAGE2(IMG_SI_POC,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_Walkt.bmp","Status Icon: PoC registration");//030705 Calvin added
#endif

/* 101205 new icon Calvin Start */
#ifdef __MMI_BG_SOUND_EFFECT__
     ADD_APPLICATION_IMAGE2(IMG_SI_BGSND,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_BGSND.bmp","Status Icon: background sound effect");
#endif

#ifdef __MMI_AUDIO_REVERB_EFFECT__
     ADD_APPLICATION_IMAGE2(IMG_SI_REVERBSND,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_AudReverb.bmp","Status Icon: audio reverb effect");
#endif

#ifdef __MMI_AUDIO_SURROUND_EFFECT__
     ADD_APPLICATION_IMAGE2(IMG_SI_SURROUNDSND,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_3DSound.bmp","Status Icon: audio surround effect");
#endif

#ifdef __MMI_HOMEZONE_STATUS_ICON__
     ADD_APPLICATION_IMAGE2(IMG_SI_HZONE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_HZONE.bmp","Status Icon: home zone");
     ADD_APPLICATION_IMAGE2(IMG_SI_CZONE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_CZONE.bmp","Status Icon: city zone");
#endif

#ifdef __CTM_SUPPORT__
     ADD_APPLICATION_IMAGE2(IMG_SI_TTY_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_TTY.bmp","Status Icon: TTY indication");
#endif
/* 101205 new icon Calvin End */


	ADD_APPLICATION_IMAGE2(IMG_SI_GPRS_ATT_NO_PDP_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_GPRSC.BMP","Status Icon: GPRS, attach without pdp");	
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_VOICE_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_VOML1.BMP","Status Icon: Unread Voice L1");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_VOICE_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_VOML2.BMP","Status Icon: Unread Voice L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_VOICE_L1L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_VOML12.BMP","Status Icon: Unread Voice L1+L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_FAX_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_FM1.BMP","Status Icon: Unread Fax L1");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_FAX_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_FM2.BMP","Status Icon: Unread Fax L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_FAX_L1L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_FM12.BMP","Status Icon: Unread Fax L1+L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_EMAIL_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_EM1.BMP","Status Icon: Unread Email L1");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_EMAIL_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_EM2.BMP","Status Icon: Unread Email L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_EMAIL_L1L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_EM12.BMP","Status Icon: Unread Email L1+L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_SMS_INDICATOR,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_MGE.BMP","Status Icon: Message");
	ADD_APPLICATION_IMAGE2(IMG_SI_CALL_DIVERT_L1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_FOR01.BMP","Status Icon: Call Divert L1");
	ADD_APPLICATION_IMAGE2(IMG_SI_CALL_DIVERT_L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_FOR02.BMP","Status Icon: Call Divert L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_CALL_DIVERT_L1L2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_FOR.BMP","Status Icon: Call Divert L1+L2");
	ADD_APPLICATION_IMAGE2(IMG_SI_VOICE_RECORD,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_REC.BMP","Status Icon: Voice record");
	ADD_APPLICATION_IMAGE2(IMG_SI_MUTE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_MUTE.BMP","Status Icon: Mute");
	ADD_APPLICATION_IMAGE2(IMG_BLINKINGICON_CHAT_ROOM1,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_CHAT1.BMP","Chat Room1 Blinking Icon");
	ADD_APPLICATION_IMAGE2(IMG_BLINKINGICON_CHAT_ROOM2,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_CHAT2.BMP","Chat Room2 Blinking Icon");
	ADD_APPLICATION_IMAGE2(IMG_SI_STOPWATCH,CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/SI_STWATCH.bmp","Image for Stopwatch Icon");
#ifdef __NATIVE_JAVA__
	ADD_APPLICATION_IMAGE2(IMG_SI_JAVA_PAUSE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_QQIM.png","Java pause");
#else
    ADD_APPLICATION_IMAGE2(IMG_SI_JAVA_PAUSE,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_Jvlogo.bmp","Java pause");
#endif
//	ADD_APPLICATION_IMAGE2(IMG_SI_WMAL,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_WMAL.bmp","Sub LCD Status Icon: Vibrate then Ring");
	//change according to JL mail 
	
	//ADD_APPLICATION_IMAGE2(IMG_SI_WAP,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_WMAL.bmp","Sub LCD Status Icon: Vibrate then Ring");
	ADD_APPLICATION_IMAGE2(IMG_SI_WAP,CUST_IMG_PATH"/MainLCD/WAP/SW_UNREAD.pbm","Sub LCD Status Icon: Vibrate then Ring");//2010-4-14 used for wap_push_indicator
    
	ADD_APPLICATION_IMAGE2(IMG_SI_NETWORK_CIPHER_GSM,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_NCGSM.bmp","Network Ciphering for GSM");//021505 Calvin added
	ADD_APPLICATION_IMAGE2(IMG_SI_NETWORK_CIPHER_GPRS,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_NCGPRS.bmp","Network Ciphering for GPRS");//021505 Calvin added

	//ADD_APPLICATION_IMAGE2(IMG_SI_JAVA_CONNECT, CUST_IMG_PATH"/MainLCD/JAVA/JAVA_connect.gif","Java connecting");

   /* Max, for IMPS */
   #ifdef __MMI_IMPS__
	ADD_APPLICATION_IMAGE2 ( IMG_SI_IMPS_MSG, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/SI_IMPSMG.bmp", "Status icon in Idle Screen when new msg comes");
	ADD_APPLICATION_IMAGE2 ( IMG_SI_IMPS_LOGIN, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/SI_IMPS.bmp", "Status icon in Idle Screen when login server");
   #endif

#ifdef __MMI_WLAN_FEATURES__
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_DISCONNECT, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/wifi_disconnect.bmp","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_SCANNING, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/wifi_connecting.gif","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_CONNECT1, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/WiFi_connect_1.bmp","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_CONNECT2, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/WiFi_connect_2.bmp","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_CONNECT3, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/WiFi_connect_3.bmp","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_CONNECT4, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/WiFi_connect_4.bmp","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_CONNECT5, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/WiFi_connect_5.bmp","Image of WLAN");
	ADD_APPLICATION_IMAGE2(IMG_ID_DTCNT_WLAN_STATUS_CONNECT6, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/WiFi/WiFi_connect_6.bmp","Image of WLAN");
#endif

#ifdef __MMI_SUBLCD__
	ADD_APPLICATION_IMAGE2(IMG_SLSI_SILENT,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_SIL.BMP","Sub LCD Status Icon: Silent");
	//ADD_APPLICATION_IMAGE2(IMG_SLSI_SIGNAL_STRENGTH,CUST_IMG_PATH"/SubLCD/StatusIcons/signal/SB_SIG.GIF","Sub LCD Status Icon: Signal strength");
	//ADD_APPLICATION_IMAGE2(IMG_SLSI_BATTERY_STRENGTH,CUST_IMG_PATH"/SubLCD/StatusIcons/battery/SB_BAT.GIF","Sub LCD Status Icon: Battery strength");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_SIGNAL_STRENGTH,CUST_IMG_PATH"/SubLCD/StatusIcons/signal/SB_SIG","Sub LCD Status Icon: Signal strength");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_BATTERY_STRENGTH,CUST_IMG_PATH"/SubLCD/StatusIcons/battery/SB_BAT","Sub LCD Status Icon: Battery strength");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_SMS_INDICATOR,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_MG.BMP","Sub LCD Status Icon: Message");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_RING,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_RING.BMP","Sub LCD Status Icon: Ring");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_VIBRATE,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_VIB.BMP","Sub LCD Status Icon: Vibrator");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_VIBRATE_AND_RING,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_VR.BMP","Sub LCD Status Icon: Vibrate and Ring");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_VIBRATE_THEN_RING,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_VTR.BMP","Sub LCD Status Icon: Vibrate then Ring");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_ALARM,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_ALARM.BMP","Sub LCD Status Icon: Alarm");
	ADD_APPLICATION_IMAGE2(IMG_SLSI_MISSED_CALL,CUST_IMG_PATH"/SubLCD/StatusIcons/SB_MCA.BMP","Sub LCD Status Icon: Missed Call");
#endif
#if (defined __UNI_TWIN_A_IDLE__)  /*UNI@baiyanjun_20070929 12:43:48 双待机信号条*/
  #if (defined __UNI_TWIN_C__) /*UNI@baiyanjun_20080326 18:01:52 G+C: 信号条修改   */
      ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_TWIN_ALL ,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG_GC","Status Icon: Signal strength");
  #else
      ADD_APPLICATION_IMAGE2(IMG_SI_SIGNAL_STRENGTH_TWIN_ALL ,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/signal/SI_SIG_SA","Status Icon: Signal strength");
  #endif //
#endif
       /*Fix Bug#12707 for cs2324c by cong.li on 2009.07.02. */
	ADD_APPLICATION_IMAGE2(IMG_SLSI_USB_CONNECTED,CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_USB.bmp","Status Icon: usb connected");

//PMT NEERAJ START 20050616
#ifdef __MMI_UI_TAB_PANE__
	PopulateTabResources();
#endif
#endif //endif __PRODUCT_QPAD__
}

#if(ENABLE_CIRCULAR_3D_MENU)
/*----------------------------------------------------------------------------
Function:			PopulateCircular3DMenuResources
Description:		Resource populator for the circular 3D menu
Input Parameters:	none
Output Parameters:	none
Returns:			void
----------------------------------------------------------------------------*/

void PopulateCircular3DMenuResources(void)
{	

	ADD_APPLICATION_IMAGE(CM3D_COMMON_IMAGE0_IMAGE_ID,CUST_IMG_PATH"/MainLCD/MainMenu/CIRCULAR_3D/CM3D4ball.gif");
	ADD_APPLICATION_IMAGE(CM3D_COMMON_IMAGE1_IMAGE_ID,CUST_IMG_PATH"/MainLCD/MainMenu/CIRCULAR_3D/CM3D5ball.gif");
	ADD_APPLICATION_IMAGE(CM3D_COMMON_IMAGE2_IMAGE_ID,CUST_IMG_PATH"/MainLCD/MainMenu/CIRCULAR_3D/CM3D6ball.gif");
	ADD_APPLICATION_IMAGE(CM3D_COMMON_IMAGE3_IMAGE_ID,CUST_IMG_PATH"/MainLCD/MainMenu/CIRCULAR_3D/CM3D7ball.gif");

}
#endif
void PopulateThemeResources(void)
{	
//qiff add UI resource,del for too slow display
#if 0 //defined __MMI_MAINLCD_240X320__
	      ADD_APPLICATION_IMAGE2(IMG_LIST_MENU_BG_IMAGE,CUST_IMG_PATH"/MainLCD/Theme/select_bg.bmp", "Animation Image for title");
#endif


}

#endif


