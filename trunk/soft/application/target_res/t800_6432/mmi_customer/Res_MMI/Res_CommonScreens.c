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

 

 

	DATE		: August-11-2003

**************************************************************/
#include "stdc.h"
#include "customcfg.h"

#include "mmi_features.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif
//#include "wgui_categories_inputs.h"

//#include "CommonScreens.h"
#include "commonscreensenum.h"
#include "settingdefs.h"
#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"
void PopulateCommonScreensResData(void)
{
	ADD_APPLICATION_MENUITEM ((MENU_INPUT_METHOD_AND_DONE_OPTION_ID, 0, 2,
								MENU_DONE_OPTION_ID,
								MENU_INPUT_METHOD_OPTION_ID,
								SHOW, NONMOVEABLE, DISP_LIST,
								STR_GLOBAL_OPTIONS, 0));
	ADD_APPLICATION_MENUITEM ((MENU_DONE_OPTION_ID,
								MENU_INPUT_METHOD_AND_DONE_OPTION_ID, 0, 
								SHOW, NONMOVEABLE, DISP_LIST,
								STR_GLOBAL_DONE, 0));
	ADD_APPLICATION_MENUITEM ((MENU_INPUT_METHOD_OPTION_ID,
								MENU_INPUT_METHOD_AND_DONE_OPTION_ID, 0, 
								SHOW, NONMOVEABLE, DISP_LIST,
								STR_GLOBAL_INPUT_METHOD, 0));
        //replaced by Global String  
	//ADD_APPLICATION_STRING2(STR_INPUT_METHOD_CAPTION,"Input Methods","Title string for input methods screen");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_ABC,"Multitap ABC","Input methods list Option 1");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_abc,"Multitap abc","Input methods list Option 2");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_ABC,"Smart ABC","Input methods list Option 3");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_abc,"Smart abc","Input methods list Option 4");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_NUMERIC,"Numeric","Input methods list Option 5");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_BOPOMO,"Multitap BoPoMo","Input methods list Option 6");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_BOPOMO,"Smart BoPoMo","Input methods list Option 7");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_TRADITIONAL_CHINESE_STROKE,"Traditional Chinese Stroke","Input methods list Option 8");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_PINYIN,"Multitap PinYin","Input methods list Option 9");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_PINYIN,"Smart PinYin","Input methods list Option 10");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SIMPLIFIED_CHINESE_STROKE,"Simplified Chinese Stroke","Input methods list Option 11");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_INSERT_SYMBOLS,"Insert Symbols","Input methods list Option 12");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SELECT_SYMBOL,"Select Symbol","Title for the symbol picker screen");
//START 
#if defined(__MMI_LANG_HINDI__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SELECT_VOWEL,"Select Symbol","Title for the Hindi Vowel picker screen");
	//Fix Bug#12068 for M301WapMMS by cong.li on 2009.05.14.
       ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_HINDI,"Smart Hindi","Input methods list Option Smart Hindi");
#endif
//END 
 
#if defined(__MMI_LANG_MARATHI__) 
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_MARATHI,"Multitap Marathi","Input methods list Option Multitap Mara");
#endif
 
#if defined(__MMI_MULTITAP_SPANISH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_SPANISH,"Multitap ES","Input methods list Option Multitap ES");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_SPANISH,"Multitap es","Input methods list Option Multitap es");
#endif	

//qiff add multi-input method support
#if defined(__MMI_MULTITAP_TAMIL__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_TAMIL,"Multitap tamil","Input methods list Option Multitap ES");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_TAMIL,"Multitap tamil","Input methods list Option Multitap es");
#endif
#if defined(__MMI_MULTITAP_BENGALI__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_BENGALI,"Multitap bengali","Input methods list Option Multitap ES");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_BENGALI,"Multitap bengali","Input methods list Option Multitap es");
#endif
#if defined(__MMI_MULTITAP_KANNADA__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_KANNADA,"Multitap kannada","Input methods list Option Multitap ES");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_KANNADA,"Multitap kannada","Input methods list Option Multitap es");
#endif
#if defined(__MMI_MULTITAP_MALAYALAM__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_MALAYALAM,"Multitap malayalam","Input methods list Option Multitap ES");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_MALAYALAM,"Multitap malayalam","Input methods list Option Multitap es");
#endif
#if defined(__MMI_MULTITAP_ORIYA__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_ORIYA,"Multitap oriya","Input methods list Option Multitap ES");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MULTITAP_ORIYA,"Multitap oriya","Input methods list Option Multitap es");
#endif
//qiff add end

 
#if defined(__MMI_MULTITAP_POLISH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_POLISH,"Multitap PO","Input methods list Option Multitap Po");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_POLISH,"Multitap po","Input methods list Option Multitap po");
#endif
 
 

#if defined(__MMI_T9_POLISH__)||defined(__MMI_ZI_POLISH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_POLISH,"Smart PO","Input methods list Option Smart PO");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_POLISH,"Smart po","Input methods list Option Smart po");
#endif
 
 
#if defined(__MMI_MULTITAP_DANISH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_DANISH,"Multitap DA","Input methods list Option Multitap DA");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_DANISH,"Multitap da","Input methods list Option Multitap da");
#endif
 
#if defined(__MMI_T9_SPANISH__)||defined(__MMI_ZI_EU_SPANISH__)||defined(__MMI_ZI_SA_SPANISH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_SPANISH,"Smart ES","Input methods list Option Smart ES");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_SPANISH,"Smart es","Input methods list Option Smart es");
#endif	
 
#if defined(__MMI_T9_DANISH__)||defined(__MMI_ZI_DANISH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_DANISH,"Smart DA","Input methods list Option Smart DA");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_DANISH,"Smart da","Input methods list Option Smart da");
#endif
 
 
#if defined(__MMI_MULTITAP_HUNGARIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_HUNGARIAN,"Multitap HG","Input methods list Option Multitap HG");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_HUNGARIAN,"Multitap hg","Input methods list Option Multitap hg");
#endif

#if defined(__MMI_T9_HUNGARIAN__)||defined(__MMI_ZI_HUNGARIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_HUNGARIAN,"Smart HG","Input methods list Option Smart HG");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_HUNGARIAN,"Smart hg","Input methods list Option Smart hg");
#endif
 
#if defined(__MMI_MULTITAP_FRENCH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_FRENCH,"Multitap FR","Input methods list Option Multitap FR");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_FRENCH,"Multitap fr","Input methods list Option Multitap fr");
#endif
#if defined(__MMI_T9_FRENCH__)||defined(__MMI_ZI_CA_FRENCH__)||defined(__MMI_ZI_EU_FRENCH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_FRENCH,"Smart FR","Input methods list Option Smart FR");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_FRENCH,"Smart fr","Input methods list Option Smart fr");
#endif	
#if defined(__MMI_MULTITAP_GERMAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_GERMAN,"Multitap DE","Input methods list Option Multitap DE");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_GERMAN,"Multitap de","Input methods list Option Multitap de");
#endif

#if defined(__MMI_MULTITAP_RUSSIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_RUSSIAN,"Multitap RU","Input methods list Option Multitap RU");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_RUSSIAN,"Multitap ru","Input methods list Option Multitap ru");
#endif
 
#if defined(__MMI_MULTITAP_ARABIC__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_ARABIC,"Multitap Arabic","Input methods list Option Multitap AR");
#endif
 
#if defined(__MMI_MULTITAP_PERSIAN__) 
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_PERSIAN,"Multitap Persian","Input methods list Option Multitap FA");
#endif
 
 
 
#if defined(__MMI_T9_RUSSIAN__)|| defined (__MMI_ZI_RUSSIAN__)||defined(__MMI_ZI_RECOMMENDED_SIZE_RUSSIAN__)
//END 
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_RUSSIAN,"Smart RU","Input methods list Option Smart RU");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_RUSSIAN,"Smart ru","Input methods list Option Smart ru");
#endif	
 
#if defined(__MMI_MULTITAP_BULGARIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_BULGARIAN,"Multitap BG","Input methods list Option Multitap BG");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_BULGARIAN,"Multitap bg","Input methods list Option Multitap bg");
#endif
#if defined(__MMI_T9_BULGARIAN__)|| defined (__MMI_ZI_BULGARIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_BULGARIAN,"Smart BG","Input methods list Option Smart BG");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_BULGARIAN,"Smart bg","Input methods list Option Smart bg");
#endif
 
 
#if defined(__MMI_ZI_THAI__)|| defined (__MMI_T9_THAI__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_THAI,"Smart Thai","Input methods list Option Smart Thai");
#endif	
 
 
#if defined(__MMI_ZI_HEBREW__)||defined(__MMI_T9_HEBREW__)
 
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_HEBREW,"Smart Hebrew","Input methods list Option Smart HEBREW");
#endif	
 
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_ARABIC,"Smart Arabic","Input methods list Option Smart Arabic");
 
#ifdef __MMI_SMART_PERSIAN__
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_PERSIAN,"Smart Persian","Input methods list Option Smart Persian");
#endif
 
 
//	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_HINDI,"Smart Hindi","Input methods list Option Smart Hindi");
 
 
#if defined(__MMI_T9_PORTUGUESE__)||defined(__MMI_ZI_EU_PORTUGUESE__)||defined(__MMI_ZI_SA_PORTUGUESE__)||defined(__MMI_ZI_BZ_PORTUGUESE__)
 
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_PORTUGUESE,"Smart PT","Input methods list Option Smart PT");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_PORTUGUESE,"Smart pt","Input methods list Option Smart pt");
#endif	
#if defined(__MMI_MULTITAP_PORTUGUESE__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_PORTUGUESE,"Multitap PT","Input methods list Option Multitap PT");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_PORTUGUESE,"Multitap pt","Input methods list Option Multitap pt");
#endif

 
#if defined (__MMI_MULTITAP_THAI__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_THAI,"Multitap Thai","Input methods list Option Multitap Thai");
#endif

 
#if defined (__MMI_MULTITAP_HEBREW__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_HEBREW,"Multitap Hebrew","Input methods list Option Multitap Hebrew");
#endif
 
#if defined(__MMI_MULTITAP_HINDI__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_HINDI,"Multitap Hindi","Input methods list Option Multitap Hindi");
#endif
 
#if defined(__MMI_ZI_TURKISH__)||defined(__MMI_T9_TURKISH__)||defined(__MMI_SMART_TURKISH__)
 
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_TURKISH,"Smart TR","Input methods list Option Smart TR");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_TURKISH,"Smart tr","Input methods list Option Smart tr");
#endif	
#if defined(__MMI_MULTITAP_TURKISH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_TURKISH,"Multitap TR","Input methods list Option Multitap TR");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_TURKISH,"Multitap tr","Input methods list Option Multitap tr");
#endif
 
#if defined(__MMI_ZI_INDONESIAN__)||defined(__MMI_T9_INDONESIAN__)||defined(__MMI_SMART_INDONESIAN__) 
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_INDONESIAN,"Smart ID","Input methods list Option Smart ID");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_INDONESIAN,"Smart id","Input methods list Option Smart id");
#endif	
 
#if defined(__MMI_MULTITAP_INDONESIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_INDONESIAN,"Multitap ID","Input methods list Option Multitap ID");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_INDONESIAN,"Multitap id","Input methods list Option Multitap id");
#endif
 
#if defined(__MMI_ZI_CZECH__)||defined(__MMI_T9_CZECH__)  
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_CZECH,"Smart CZECH","Input methods list Option Smart CZECH");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_CZECH,"Smart czech","Input methods list Option Smart czech");
#endif	
#if defined(__MMI_MULTITAP_CZECH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_CZECH,"Multitap CZECH","Input methods list Option Multitap CZECH");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_CZECH,"Multitap czech","Input methods list Option Multitap czech");
#endif
 
 
#if defined(__MMI_ZI_MALAY__)||defined(__MMI_T9_MALAY__)||defined(__MMI_SMART_MALAY__) 
 
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_MALAY,"Smart MS","Input methods list Option Smart MS");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_MALAY,"Smart ms","Input methods list Option Smart ms");
#endif	
#if defined(__MMI_MULTITAP_MALAY__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_MALAY,"Multitap MS","Input methods list Option Multitap MS");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_MALAY,"Multitap ms","Input methods list Option Multitap ms");
#endif
 
#if defined(__MMI_ZI_VIETNAMESE__)||defined(__MMI_T9_VIETNAMESE__)  
 
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_VIETNAMESE,"Smart VI","Input methods list Option Smart VI");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_VIETNAMESE,"Smart vi","Input methods list Option Smart vi");
#endif	
#if defined(__MMI_MULTITAP_VIETNAMESE__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_VIETNAMESE,"Multitap VI","Input methods list Option Multitap VI");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_VIETNAMESE,"Multitap vi","Input methods list Option Multitap vi");
#endif
 
#if defined(__MMI_MULTITAP_FINNISH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_FINNISH,"Multitap FI","Input methods list Option Multitap FI");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_FINNISH,"Multitap fi","Input methods list Option Multitap fi");
#endif	
#if defined(__MMI_T9_FINNISH__)||defined(__MMI_ZI_FINNISH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_FINNISH,"Smart FI","Input methods list Option Smart FI");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_FINNISH,"Smart fi","Input methods list Option Smart fi");
#endif
#if defined(__MMI_ZI_HUNGARIAN__)||defined(__MMI_T9_HUNGARIAN__)  
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_HUNGARIAN,"Smart HUNGARIAN","Input methods list Option Smart HUNGARIAN");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_HUNGARIAN,"Smart hungarian","Input methods list Option Smart hungarian");
#endif	
#if defined(__MMI_MULTITAP_HUNGARIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_HUNGARIAN,"Multitap HUNGARIAN","Input methods list Option Multitap HUNGARIAN");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_HUNGARIAN,"Multitap hungarian","Input methods list Option Multitap hungarian");
#endif
 
 
 
#if defined(__MMI_T9_NORWEGIAN__)  ||defined(__MMI_ZI_NORWEGIAN__)  
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_NORWEGIAN,"Smart NO","Input methods list Option Smart NO");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_NORWEGIAN,"Smart no","Input methods list Option Smart no");
#endif	
 
#if defined(__MMI_MULTITAP_NORWEGIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_NORWEGIAN,"Multitap NO","Input methods list Option Multitap NO");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_NORWEGIAN,"Multitap no","Input methods list Option Multitap no");
#endif
//Norwegian end
#if defined(__MMI_MULTITAP_SLOVAK__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_SLOVAK,"Multitap SK","Input methods list Option Multitap SK");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_SLOVAK,"Multitap sk","Input methods list Option Multitap sk");
#endif	
#if defined(__MMI_T9_SLOVAK__)||defined(__MMI_ZI_SLOVAK__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_SLOVAK,"Smart SK","Input methods list Option Smart SK");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_SLOVAK,"Smart sk","Input methods list Option Smart sk");
#endif
#if defined(__MMI_ZI_DUTCH__)||defined(__MMI_T9_DUTCH__)  
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_DUTCH,"Smart NL","Input methods list Option Smart VI");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_DUTCH,"Smart nl","Input methods list Option Smart vi");
#endif	
#if defined(__MMI_MULTITAP_DUTCH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_DUTCH,"Multitap NL","Input methods list Option Multitap VI");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_DUTCH,"Multitap nl","Input methods list Option Multitap vi");
#endif
 
 
#if defined(__MMI_MULTITAP_SWEDISH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_SWEDISH,"Multitap SV","Input methods list Option Multitap SV");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_SWEDISH,"Multitap sv","Input methods list Option Multitap sv");
#endif	
#if defined(__MMI_T9_SWEDISH__)||defined(__MMI_ZI_SWEDISH__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_SWEDISH,"Smart SV","Input methods list Option Smart SV");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_SWEDISH,"Smart sv","Input methods list Option Smart sv");
#endif
 

 
#if defined(__MMI_MULTITAP_CROATIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_CROATIAN,"Multitap HR","Input methods list Option Multitap HR");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_CROATIAN,"Multitap hr","Input methods list Option Multitap hr");
#endif	
#if defined(__MMI_T9_CROATIAN__)||defined(__MMI_ZI_CROATIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_CROATIAN,"Smart HR","Input methods list Option Smart HR");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_CROATIAN,"Smart hr","Input methods list Option Smart hr");
#endif
 
#if defined(__MMI_ZI_ROMANIAN__)||defined(__MMI_T9_ROMANIAN__)
 
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_ROMANIAN,"Smart RO","Input methods list Option Smart RM");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_ROMANIAN,"Smart ro","Input methods list Option Smart rm");
#endif	
#if defined(__MMI_MULTITAP_ROMANIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_ROMANIAN,"Multitap RO","Input methods list Option Multitap RM");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_ROMANIAN,"Multitap ro","Input methods list Option Multitap rm");
#endif
#if defined(__MMI_MULTITAP_SLOVENIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_SLOVENIAN,"Multitap SL","Input methods list Option Multitap SL");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_SLOVENIAN,"Multitap sl","Input methods list Option Multitap sl");
#endif	
#if defined(__MMI_T9_SLOVENIAN__)||defined(__MMI_ZI_SLOVENIAN__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_SLOVENIAN,"Smart SL","Input methods list Option Smart SL");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_SLOVENIAN,"Smart sl","Input methods list Option Smart sl");
#endif
 

 
#if defined(__MMI_MULTITAP_GREEK__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_GREEK,"Multitap EL","Input methods list Option Multitap EL");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_GREEK,"Multitap el","Input methods list Option Multitap el");
#endif	
#if defined(__MMI_T9_GREEK__)||defined(__MMI_ZI_GREEK__)
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_GREEK,"Smart EL","Input methods list Option Smart EL");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_GREEK,"Smart el","Input methods list Option Smart el");
#endif
 
#ifdef APPMEM_SUPPORT
	ADD_APPLICATION_STRING2(STR_ID_ASM_MEM_FULL_1 ,"The memory is not enough for executing","STR_ID_ASM_MEM_FULL_1");
	ADD_APPLICATION_STRING2(STR_ID_ASM_MEM_FULL_2 ,"Please terminate the following applications,","STR_ID_ASM_MEM_FULL_2");
	ADD_APPLICATION_STRING2(STR_ID_ASM_MEM_FULL_3 ,"Can not execute the application!","STR_ID_ASM_MEM_FULL_3");
	ADD_APPLICATION_STRING2(STR_ID_ASM_MEM_FULL_TITLE ,"Memory Full!","STR_ID_ASM_MEM_FULL_TITLE");
#endif /* APPMEM_SUPPORT */
#if 0
    ADD_APPLICATION_STRING2(STR_ID_APPMEM_WAITING_FOR_STOP, "Please Wait...", "Waiting for stopping an application");
    ADD_APPLICATION_STRING2(STR_ID_APPMEM_ATTR_STOPPING, " (Stopping)", "Attribute of an application being stopped");
    ADD_APPLICATION_STRING2(STR_ID_APPMEM_PROMPT_STOP_APP, "Stop other applications to release memory", "help text");
    ADD_APPLICATION_STRING2(STR_ID_APPMEM_STOP, "Stop", "Stop text "); 
    ADD_APPLICATION_STRING2(STR_ID_APPMEM_WAIT, "Wait", "Wait text in Softkey");
    ADD_APPLICATION_STRING2(STR_ID_APPMEM_OUT_OF_MEMORY, "Out of Memory", "Out of Memory");
    ADD_APPLICATION_STRING2(STR_ID_APPMEM_NOTIFY_STARTING_NEW, "Starting New Application", "Dialog message");        
    ADD_APPLICATION_STRING2(STR_ID_APPMEM_NOTIFY_STOPPED, "Application Stopped", "Dialog message");
    ADD_APPLICATION_STRING2(STR_ID_APPMEM_NOTIFY_STOP_FAILED, "Failed to Stop Application", "Dialog message");    
#endif    
}

#endif


