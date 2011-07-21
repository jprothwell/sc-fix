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

	FILENAME	: Res_InputMethod.c

  	PURPOSE		: Populate Resource for InputMethod

	REMARKS		: nil

	AUTHOR		: Yogesh

	DATE		: August-15-2003

**************************************************************/

#include "cs_types.h"
#include "custdatares.h"
#include "mmi_features.h" 

#include "stdc.h"
#include "customcfg.h"


#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#include "commonscreensenum.h"
#include "settingdefs.h"
#include "wgui_categories_defs.h"
#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"


void PopulateInputMethodRes(void)
{
	#if 0
    ADD_APPLICATION_STRING2(STR_T9_EXP_UP_ABC,"Tap_ABC","multitap ABC");
    ADD_APPLICATION_STRING2(STR_T9_EXP_LO_abc,"Tap_abc","multitap abc");
    ADD_APPLICATION_STRING2(STR_T9_AMB_UP_ABC,"Smart_ABC","smart ABC");
    ADD_APPLICATION_STRING2(STR_T9_AMB_LO_abc,"Smart_abc","smart abc");
    ADD_APPLICATION_STRING2(STR_T9_123,"123","numeric input");
    ADD_APPLICATION_STRING2(STR_T9_BOPOMOFO,"BoPoMo","bopomofo input");
    ADD_APPLICATION_STRING2(STR_T9_TR_STROKE,"Tr_Stroke","stroke for traditional chinese");
    ADD_APPLICATION_STRING2(STR_T9_PINYIN,"PinYin","pinyin for simplifed chinese");
    ADD_APPLICATION_STRING2(STR_T9_SM_STROKE,"Sm_Stroke","stroke for simplified chinese");
    //ADD_APPLICATION_STRING2(STR_T9_CLEAR,"Clear","clear"); ==> replaced by Global String
    //ADD_APPLICATION_STRING2(STR_T9_BACK,"Back","back"); ==> replaced by Global String
    //ADD_APPLICATION_STRING2(STR_T9_SELECT,"Select","select");==>replaced by Global String
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_REACH_MAX_LEN,"Reach Max Length","Reach Max Length");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_THAI,"Thai","thai input method indicator");
    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_ARABIC,"AR","Arabic input method indicator");    
//PMT START PERSIAN
//    ADD_APPLICATION_STRING2(STR_INPUT_METHOD_PERSIAN,"FA","Persian input method indicator");    
//PMT END PERSIAN
//PMT START GR_HE_CZ_FI_DA_HU 20051007
//	  ADD_APPLICATION_STRING2(STR_INPUT_METHOD_HEBREW,"HE","Hebrew input method indicator"); 
//PMT END GR_HE_CZ_FI_DA_HU 20051007
	#endif  
	
	//Add Guobi Input method string  ,Kenneth
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
	//Fix Bug#12068 for M301WapMMS by cong.li on 2009.05.14.
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_HINDI,"Smart Hindi","Input methods list Option Smart Hindi");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SIMPLIFIED_CHINESE_STROKE,"Simplified Chinese Stroke","Input methods list Option 11");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_INSERT_SYMBOLS,"Insert Symbols","Input methods list Option 12");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SELECT_SYMBOL,"Select Symbol","Title for the symbol picker screen");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_HINDI,"Multitap Hindi","Input methods list Option Multitap Hindi");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_HINDI,"Smart Hindi","Input methods list Option SMART Hindi");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_SPANISH,"Multitap ES","Input methods list Option Multitap ES");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_SPANISH,"Multitap es","Input methods list Option Multitap es");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_SPANISH,"Smart ES","Input methods list Option Smart ES");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_SPANISH,"Smart es","Input methods list Option Smart es");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_DANISH,"Multitap DA","Input methods list Option Multitap DA");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_DANISH,"Multitap da","Input methods list Option Multitap da");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_DANISH,"Smart DA","Input methods list Option Smart DA");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_DANISH,"Smart da","Input methods list Option Smart da");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_FRENCH,"Multitap FR","Input methods list Option Multitap FR");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_FRENCH,"Multitap fr","Input methods list Option Multitap fr");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_FRENCH,"Smart FR","Input methods list Option Smart FR");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_FRENCH,"Smart fr","Input methods list Option Smart fr");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_GERMAN,"Multitap DE","Input methods list Option Multitap DE");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_GERMAN,"Multitap de","Input methods list Option Multitap de");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_GERMAN,"Smart DE","Input methods list Option Smart DE");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_GERMAN,"Smart de","Input methods list Option Smart de");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_ITALIAN,"Multitap IT","Input methods list Option Multitap IT");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_ITALIAN,"Multitap it","Input methods list Option Multitap it");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_ITALIAN,"Smart IT","Input methods list Option Smart IT");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_ITALIAN,"Smart it","Input methods list Option Smart it");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_RUSSIAN,"Multitap RU","Input methods list Option Multitap RU");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_RUSSIAN,"Multitap ru","Input methods list Option Multitap ru");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_RUSSIAN,"Smart RU","Input methods list Option Smart RU");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_RUSSIAN,"Smart ru","Input methods list Option Smart ru");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_ARABIC,"Smart Arabic","Input methods list Option Smart Arabic");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_TURKISH,"Multitap TR","Input methods list Option Multitap TR");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_TURKISH,"Multitap tr","Input methods list Option Multitap tr");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_TURKISH,"Smart TR","Input methods list Option Smart TR");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_TURKISH,"Smart tr","Input methods list Option Smart tr");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_PORTUGUESE,"Multitap PT","Input methods list Option Multitap PT");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_PORTUGUESE,"Multitap pt","Input methods list Option Multitap pt");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_PORTUGUESE,"Smart PT","Input methods list Option Smart PT");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_PORTUGUESE,"Smart pt","Input methods list Option Smart pt");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_INDONESIAN,"Multitap ID","Input methods list Option Multitap ID");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_INDONESIAN,"Multitap id","Input methods list Option Multitap id");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_INDONESIAN,"Smart ID","Input methods list Option Smart ID");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_INDONESIAN,"Smart id","Input methods list Option Smart id");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_MALAY,"Multitap MS","Input methods list Option Multitap MS");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_MALAY,"Multitap ms","Input methods list Option Multitap ms");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_MALAY,"Smart MS","Input methods list Option Smart MS");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_MALAY,"Smart ms","Input methods list Option Smart ms");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_U_VIETNAMESE,"Multitap VI","Input methods list Option Multitap VI");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_L_VIETNAMESE,"Multitap vi","Input methods list Option Multitap vi");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_U_VIETNAMESE,"Smart VI","Input methods list Option Smart VI");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_L_VIETNAMESE,"Smart vi","Input methods list Option Smart vi");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_MULTITAP_THAI,"Thai","Input methods list Option Multitap Thai");
	ADD_APPLICATION_STRING2(STR_INPUT_METHOD_MENU_SMART_THAI,"Smart Thai","Input methods list Option Smart Thai");
}

#endif


