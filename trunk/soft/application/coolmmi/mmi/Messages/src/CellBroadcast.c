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
 * CellBroadcast.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for MMI SMS APP.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

 /**************************************************************

	FILENAME	: CellBroadcast.c

  	PURPOSE		: Cell Broadcast application 

 

	AUTHOR		: Hariprasad, Magesh k

	DATE		: JAN 09,03

**************************************************************/
#ifdef __MOD_SMSAL__

#define __NEWSIMULATOR 
//
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

//... Add More MMI header
#include "eventsgprot.h"
#include "historygprot.h"
#include "globalmenuitems.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "custmenures.h"
#include "commonscreens.h"
#include "unicodexdcl.h"
#include "timerevents.h"
#include "debuginitdef.h"
#include "protocolevents.h"

//...Add MSG header
#include "customer_ps_inc.h"
#include "mmi_msg_context.h"
#include "messagesl4def.h"
#include "messagesresourcedata.h"
#include "messagesmiscell.h"
#include "messagesexdcl.h"
#include "smsguiinterfaceprot.h"
#include "smsguiinterfacetype.h"
#include "smspshandler.h"
#include "cellbroadcasttype.h"
//
#include "gui_ems.h"
#include "wgui_ems_categories.h"
#include "gsm7bitdefaultalphabet.h"
//
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"

#include "settingprofile.h"
#include "profilegprots.h"
#include "settingprot.h"
#include "phonebookgprot.h"

#include "gpioinc.h"
#include "keybrd.h"
//
#undef __NEWSIMULATOR
#ifdef MMI_ON_HARDWARE_P
#include "nvram_user_defs.h"
#endif 

#include "smsapi.h"
#include "smsstruct.h"//temp!!

//
#ifndef NVRAM_SMS_CB_CHNL_RECORD_SIZE
#define NVRAM_SMS_CB_CHNL_RECORD_SIZE	180
#endif

#ifndef NVRAM_SMS_CB_SMS_RECORD_SIZE	
//CSD Joy modified for corrected NVRAM data size, 1207
#define NVRAM_SMS_CB_SMS_RECORD_SIZE	826
//CSD end
#endif
//
//CSD Joy added for multipage CB, 0113
#define SWAP_USHORT(Var) *Var = *(unsigned short*)SwapEndian((void*)Var, sizeof(short))

#ifndef NVRAM_SMS_CB_SMS_PAGE_SIZE
#define NVRAM_SMS_CB_SMS_PAGE_SIZE			94
#endif
#ifndef NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL
#define NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL			30
#endif
#ifndef NVRAM_SMS_CB_SMS_INFO_SIZE
#define NVRAM_SMS_CB_SMS_INFO_SIZE			20
#endif
#define SMS_CB_SMS_PAGE_SIZE_UCS2			82
//CSD end
#define MESSAGES_CB_BOOTUP_TIMEOUT_DURATION 3000	// 3 Seconds
static msg_cb_channel_nvram_struct		cbNvramStruct;
static U8 IsCBMessageReadOperRequired=TRUE;
static msg_cb_msg_index_struct 				cbMsgIndex;
static msg_cb_msg_info_struct 		cbMsgInfo;
static msg_cb_msg_content_struct	cbMsgContent[MAX_CB_MSG_TOTAL_PAGE][NVRAM_SMS_CB_SMS_PAGE_SIZE];
static U8 *cbMsgStart;
#ifdef __MMI_MESSAGES_TWO_CB_MODES_DISPLAY__
static U16 kplLSKStringtID;
static U16 kplLSKIconID;
static U16 kplRSKStringID; 
static U16 kplRSKIconID;
#endif
static U8 dispCBMessage[NVRAM_SMS_CB_SMS_PAGE_SIZE*ENCODING_LENGTH*MAX_CB_MSG_PAGE];
static U32 dispCBLength=0;
static U16 cbChnlEditorIcons[MAX_CB_CHNL_INLINE_ITEM] = 
{ 
	IMG_CB_CHNL_TITLE, 
	IMG_SMS_COMMON_NOIMAGE,
	IMG_CB_CHNL_NUMBER,
	IMG_SMS_COMMON_NOIMAGE 
};
const U16 cbDefaultChnlNameStrId[SMS_CB_MAX_ENTRY] =
{ 
	STR_CB_CHNL_NAME_1,
	STR_CB_CHNL_NAME_2,
	STR_CB_CHNL_NAME_3,
	STR_CB_CHNL_NAME_4,
	STR_CB_CHNL_NAME_5,
	STR_CB_CHNL_NAME_6,	
	STR_CB_CHNL_NAME_7,
	STR_CB_CHNL_NAME_8,
	STR_CB_CHNL_NAME_9,
	STR_CB_CHNL_NAME_10,
        STR_CB_CHNL_NAME_11,
	STR_CB_CHNL_NAME_12,
	STR_CB_CHNL_NAME_13,
	STR_CB_CHNL_NAME_14,
	STR_CB_CHNL_NAME_15,
	STR_CB_CHNL_NAME_16,
	STR_CB_CHNL_NAME_17,
	STR_CB_CHNL_NAME_18,
	STR_CB_CHNL_NAME_19,
	STR_CB_CHNL_NAME_20,
        STR_CB_CHNL_NAME_21,
	STR_CB_CHNL_NAME_22,
	STR_CB_CHNL_NAME_23,
	STR_CB_CHNL_NAME_24,
	STR_CB_CHNL_NAME_25,
	STR_CB_CHNL_NAME_26,
	STR_CB_CHNL_NAME_27,
	STR_CB_CHNL_NAME_28,
	STR_CB_CHNL_NAME_29,
	STR_CB_CHNL_NAME_30,
        STR_CB_CHNL_NAME_31,
	STR_CB_CHNL_NAME_32,
	STR_CB_CHNL_NAME_33,
	STR_CB_CHNL_NAME_34,
	STR_CB_CHNL_NAME_35,
	STR_CB_CHNL_NAME_36,
	STR_CB_CHNL_NAME_37,
	STR_CB_CHNL_NAME_38,
	STR_CB_CHNL_NAME_39,
	STR_CB_CHNL_NAME_40,
};
//


/**************************************************************

				revised part--variable

**************************************************************/
static msg_cb_info_struct 	cb_info;
static msg_cb_state_enum		cb_state;
static msg_cb_display_struct	cb_display;
extern wgui_inline_item	wgui_inline_items[];

/*************************************************************

				Cb language

*************************************************************/
const UINT LangEnumAndStrID[MAX_LANG_SUPPORT][2] =
 {
		{MMI_CBS0_GERMAN	, 	STR_LANG_GERMAN},			
		{MMI_CBS0_ENGLISH, 	STR_LANG_ENGLISH},			
		{MMI_CBS0_ITALIAN,	STR_LANG_ITALIAN},				
		{MMI_CBS0_FRENCH, 	STR_LANG_FRENCH},
		{MMI_CBS0_SPANISH, 	STR_LANG_SPANISH},
		{MMI_CBS0_DUTCH, 		STR_LANG_DUTCH},		
		{MMI_CBS0_SWEDISH,	STR_LANG_SWEDISH},
		{MMI_CBS0_DANISH,		STR_LANG_DANISH},
		{MMI_CBS0_PORTUGUESE, STR_LANG_PORTUGUESE},
		{MMI_CBS0_FINNISH,	STR_LANG_FINNISH},	
		{MMI_CBS0_NORWEGIAN,	STR_LANG_NORWEGIAN},	
		{MMI_CBS0_GREEK,		STR_LANG_GREEK},		
		{MMI_CBS0_TURKISH,	STR_LANG_TURKISH},	
		{MMI_CBS0_HUNGARIAN,	STR_LANG_HUNGARIAN},
		{MMI_CBS0_POLISH,		STR_LANG_POLISH},
		{MMI_CBS0_LANG_UNSPECIFIED,	STR103_MITEM103_CB_LANGUAGE_UNSPECIFIED},
		{MMI_CBS1_CZECH,		STR_LANG_CZECH},
		{MMI_CBS1_HEBREW,		STR_LANG_HEBREW},	
		{MMI_CBS1_ARABIC,		STR_LANG_ARABIC},	
		{MMI_CBS1_RUSSIAN,	STR_LANG_RUSSIAN},	
		{MMI_CBS2_AA,	STR_LANG_AFAR},
		{MMI_CBS2_AB,	STR_LANG_ABKHAZIAN},
		{MMI_CBS2_AF,	STR_LANG_AFRIKAANS},
		{MMI_CBS2_AM,	STR_LANG_AMHARIC},
		{MMI_CBS2_AR,	STR_LANG_ARABIC},
		{MMI_CBS2_AS,	STR_LANG_ASSAMESE},
		{MMI_CBS2_AY,	STR_LANG_AYMARA},
		{MMI_CBS2_AZ,	STR_LANG_AZERBAIJANI},
		{MMI_CBS2_BA,	STR_LANG_BASHKIR},
		{MMI_CBS2_BE,	STR_LANG_BYELORUSSIAN},
		{MMI_CBS2_BG,	STR_LANG_BULGARIAN},
		{MMI_CBS2_BH,	STR_LANG_BIHARI},
		{MMI_CBS2_BI,	STR_LANG_BISLAMA},
		{MMI_CBS2_BN,	STR_LANG_BENGALI},  
		{MMI_CBS2_BO,	STR_LANG_TIBETAN},
		{MMI_CBS2_BR,	STR_LANG_BRETON},
		{MMI_CBS2_CA,	STR_LANG_CATALAN},
		{MMI_CBS2_CO,	STR_LANG_CORSICAN},
		{MMI_CBS2_CS,	STR_LANG_CZECH},
		{MMI_CBS2_CY,	STR_LANG_WELSH},
		{MMI_CBS2_DA,	STR_LANG_DANISH} ,   
		{MMI_CBS2_DE,	STR_LANG_GERMAN},
		{MMI_CBS2_DZ,	STR_LANG_BHUTANI},
		{MMI_CBS2_EL,	STR_LANG_GREEK},
		{MMI_CBS2_EN,	STR_LANG_ENGLISH},
		{MMI_CBS2_EO,	STR_LANG_ESPERANTO},
		{MMI_CBS2_ES,	STR_LANG_SPANISH},
		{MMI_CBS2_ET,	STR_LANG_ESTONIAN},
		{MMI_CBS2_EU,	STR_LANG_BASQUE},
		{MMI_CBS2_FA,	STR_LANG_PERSIAN},
		{MMI_CBS2_FI,	STR_LANG_FINNISH},   
		{MMI_CBS2_FJ,	STR_LANG_FIJI},
		{MMI_CBS2_FO,	STR_LANG_FAROESE},
		{MMI_CBS2_FR,	STR_LANG_FRENCH},
		{MMI_CBS2_FY,	STR_LANG_FRISIAN},
		{MMI_CBS2_GA,	STR_LANG_IRISH},
		{MMI_CBS2_GD, 	STR_LANG_SCOTS_GAELIC},
		{MMI_CBS2_GL, 	STR_LANG_GALICIAN},
		{MMI_CBS2_GN, 	STR_LANG_GUARANI},
		{MMI_CBS2_GU, 	STR_LANG_GUJARATI},
		{MMI_CBS2_HA, 	STR_LANG_HAUSA}  ,  
		{MMI_CBS2_HE, 	STR_LANG_HEBREW}, 
		{MMI_CBS2_HI, 	STR_LANG_HINDI},
		{MMI_CBS2_HR, 	STR_LANG_CROATIAN},
		{MMI_CBS2_HU, 	STR_LANG_HUNGARIAN},
		{MMI_CBS2_HY, 	STR_LANG_ARMENIAN},
		{MMI_CBS2_IA, 	STR_LANG_INTERLINGUA},
		{MMI_CBS2_ID, 	STR_LANG_INDONESIAN}, 
		{MMI_CBS2_IE, 	STR_LANG_INTERLINGUE},
		{MMI_CBS2_IK, 	STR_LANG_INUPIAK},
		{MMI_CBS2_IS, 	STR_LANG_ICELANDIC} , 
		{MMI_CBS2_IT, 	STR_LANG_ITALIAN},
		{MMI_CBS2_IU, 	STR_LANG_INUKTITUT},
		{MMI_CBS2_JA, 	STR_LANG_JAPANESE},
		{MMI_CBS2_JW, 	STR_LANG_JAVANESE},
		{MMI_CBS2_KA, 	STR_LANG_GEORGIAN},
		{MMI_CBS2_KK, 	STR_LANG_KAZAKH},
		{MMI_CBS2_KL, 	STR_LANG_GREENLANDIC},
		{MMI_CBS2_KM, 	STR_LANG_CAMBODIAN},
		{MMI_CBS2_KN, 	STR_LANG_KANNADA},
		{MMI_CBS2_KO, 	STR_LANG_KOREAN}  , 
		{MMI_CBS2_KS, 	STR_LANG_KASHMIRI},
		{MMI_CBS2_KU, 	STR_LANG_KURDISH},
		{MMI_CBS2_KY, 	STR_LANG_KIRGHIZ},
		{MMI_CBS2_LA, 	STR_LANG_LATIN},
		{MMI_CBS2_LN, 	STR_LANG_LINGALA},
		{MMI_CBS2_LO, 	STR_LANG_LAOTHIAN},
		{MMI_CBS2_LT, 	STR_LANG_LITHUANIAN},
		{MMI_CBS2_LV, 	STR_LANG_LATVIAN}, 
		{MMI_CBS2_MG, 	STR_LANG_MALAGASY},
		{MMI_CBS2_MI, 	STR_LANG_MAORI} ,    
		{MMI_CBS2_MK, 	STR_LANG_MACEDONIAN},
		{MMI_CBS2_ML, 	STR_LANG_MALAYALAM},
		{MMI_CBS2_MN, 	STR_LANG_MONGOLIAN},
		{MMI_CBS2_MO, 	STR_LANG_MOLDAVIAN},
		{MMI_CBS2_MR, 	STR_LANG_MARATHI},
		{MMI_CBS2_MS, 	STR_LANG_MALAY},
		{MMI_CBS2_MT, 	STR_LANG_MALTESE},
		{MMI_CBS2_MY, 	STR_LANG_BURMESE},
		{MMI_CBS2_NA, 	STR_LANG_NAURU},
		{MMI_CBS2_NE, 	STR_LANG_NEPALI},  
		{MMI_CBS2_NL, 	STR_LANG_DUTCH},
		{MMI_CBS2_NO, 	STR_LANG_NORWEGIAN},
		{MMI_CBS2_OC, 	STR_LANG_OCCITAN},
		{MMI_CBS2_OM, 	STR_LANG_OROMO}, 
		{MMI_CBS2_OR, 	STR_LANG_ORIYA},
		{MMI_CBS2_PA, 	STR_LANG_PUNJABI},
		{MMI_CBS2_PL, 	STR_LANG_POLISH},
		{MMI_CBS2_PS, 	STR_LANG_PASHTO}, 
		{MMI_CBS2_PT, 	STR_LANG_PORTUGUESE},
		{MMI_CBS2_QU, 	STR_LANG_QUECHUA},  
		{MMI_CBS2_RM, 	STR_LANG_RHAETO_ROMANCE},
		{MMI_CBS2_RN, 	STR_LANG_KIRUNDI},
		{MMI_CBS2_RO, 	STR_LANG_ROMANIAN},
		{MMI_CBS2_RU, 	STR_LANG_RUSSIAN},
		{MMI_CBS2_RW, STR_LANG_KINYARWANDA},
		{MMI_CBS2_SA, 	STR_LANG_SANSKRIT},
		{MMI_CBS2_SD, 	STR_LANG_SINDHI},
		{MMI_CBS2_SG, 	STR_LANG_SANGHO},
		{MMI_CBS2_SH, 	STR_LANG_SERBO_CROATIAN},
		{MMI_CBS2_SI, 	STR_LANG_SINHALESE} ,  
		{MMI_CBS2_SK, 	STR_LANG_SLOVAK},
		{MMI_CBS2_SL, 	STR_LANG_SLOVENIAN},
		{MMI_CBS2_SM, 	STR_LANG_SAMOAN},
		{MMI_CBS2_SN, 	STR_LANG_SHONA},
		{MMI_CBS2_SO, 	STR_LANG_SOMALI},
		{MMI_CBS2_SQ, 	STR_LANG_ALBANIAN},
		{MMI_CBS2_SR, 	STR_LANG_SERBIAN},
		{MMI_CBS2_SS, 	STR_LANG_SISWATI},
		{MMI_CBS2_ST, 	STR_LANG_SESOTHO},
		{MMI_CBS2_SU, 	STR_LANG_SUNDANESE} ,
		{MMI_CBS2_SV, 	STR_LANG_SWEDISH},
		{MMI_CBS2_SW, 	STR_LANG_SWAHILI},
		{MMI_CBS2_TA, 	STR_LANG_TAMIL},		
		{MMI_CBS2_TE, 	STR_LANG_TELUGU},
		{MMI_CBS2_TG, 	STR_LANG_TAJIK},
		{MMI_CBS2_TH, 	STR_LANG_THAI},
		{MMI_CBS2_TI, 	STR_LANG_TIGRINYA},
		{MMI_CBS2_TK, 	STR_LANG_TURKMEN},
		{MMI_CBS2_TL, 	STR_LANG_TAGALOG},
		{MMI_CBS2_TN, 	STR_LANG_SETSWANA} , 
		{MMI_CBS2_TO, 	STR_LANG_TONGA},
		{MMI_CBS2_TR, 	STR_LANG_TURKISH},
		{MMI_CBS2_TS, 	STR_LANG_TSONGA},
		{MMI_CBS2_TT, 	STR_LANG_TATAR},
		{MMI_CBS2_TW, 	STR_LANG_TWI},
		{MMI_CBS2_UG, 	STR_LANG_UIGHUR},
		{MMI_CBS2_UK, 	STR_LANG_UKRAINIAN},
		{MMI_CBS2_UR, 	STR_LANG_URDU},
		{MMI_CBS2_UZ, 	STR_LANG_UZBEK},
		{MMI_CBS2_VI, 	STR_LANG_VIETNAMESE} , 
		{MMI_CBS2_VO, 	STR_LANG_VOLAPUK},
		{MMI_CBS2_WO, STR_LANG_WOLOF},
		{MMI_CBS2_XH, 	STR_LANG_XHOSA},
		{MMI_CBS2_YI,	STR_LANG_YIDDISH },
		{MMI_CBS2_YO, 	STR_LANG_YORUBA},
		{MMI_CBS2_ZA, 	STR_LANG_ZHUANG},
		{MMI_CBS2_ZH, 	STR_LANG_CHINESE},
		{MMI_CBS2_ZU, 	STR_LANG_ZULU},
		{MMI_CBS_INVALID,0}
	};


/*****************************************************************************
* FUNCTION
*  MMICbsDecodeLang
* DESCRIPTION
*   Decode CB language
*
* PARAMETERS
*  char1  		IN		char 1
*  char2  		IN		char 2
*  lang_type  	IN/OUT	language type
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void MMICbsDecodeLang(U8 char1, U8 char2, msg_cb_lang_enum *lang_type )
{
   *lang_type = MMI_CBS_INVALID;
   switch (char1)
   {
      case 'a':switch (char2){case 'a': *lang_type = MMI_CBS2_AA; break;
                              case 'b': *lang_type = MMI_CBS2_AB; break;
                              case 'f': *lang_type = MMI_CBS2_AF; break;
                              case 'm': *lang_type = MMI_CBS2_AM; break;
                              case 'r': *lang_type = MMI_CBS2_AR; break;
                              case 's': *lang_type = MMI_CBS2_AS; break;
                              case 'y': *lang_type = MMI_CBS2_AY; break;
                              case 'z': *lang_type = MMI_CBS2_AZ; break;
                             }break;
      case 'b':switch (char2){case 'a': *lang_type = MMI_CBS2_BA; break;
                              case 'e': *lang_type = MMI_CBS2_BE; break;
                              case 'g': *lang_type = MMI_CBS2_BG; break;
                              case 'h': *lang_type = MMI_CBS2_BH; break;
                              case 'i': *lang_type = MMI_CBS2_BI; break;
                              case 'n': *lang_type = MMI_CBS2_BN; break;
                              case 'o': *lang_type = MMI_CBS2_BO; break;
                              case 'r': *lang_type = MMI_CBS2_BR; break;
                             }break;
      case 'c':switch (char2){case 'a': *lang_type = MMI_CBS2_CA; break;
                              case 'o': *lang_type = MMI_CBS2_CO; break;
                              case 's': *lang_type = MMI_CBS2_CS; break;
                              case 'y': *lang_type = MMI_CBS2_CY; break;
                             }break;
      case 'd':switch (char2){case 'a': *lang_type = MMI_CBS2_DA; break;
                              case 'e': *lang_type = MMI_CBS2_DE; break;
                              case 'z': *lang_type = MMI_CBS2_DZ; break;
                             }break;
      case 'e':switch (char2){case 'l': *lang_type = MMI_CBS2_EL; break;
                              case 'n': *lang_type = MMI_CBS2_EN; break;
                              case 'o': *lang_type = MMI_CBS2_EO; break;
                              case 's': *lang_type = MMI_CBS2_ES; break;
                              case 't': *lang_type = MMI_CBS2_ET; break;
                              case 'u': *lang_type = MMI_CBS2_EU; break;
                             }break;
      case 'f':switch (char2){case 'a': *lang_type = MMI_CBS2_FA; break;
                              case 'i': *lang_type = MMI_CBS2_FI; break;
                              case 'j': *lang_type = MMI_CBS2_FJ; break;
                              case 'o': *lang_type = MMI_CBS2_FO; break;
                              case 'r': *lang_type = MMI_CBS2_FR; break;
                              case 'y': *lang_type = MMI_CBS2_FY; break;
                             }break;
      case 'g':switch (char2){case 'a': *lang_type = MMI_CBS2_GA; break;
                              case 'd': *lang_type = MMI_CBS2_GD; break;
                              case 'l': *lang_type = MMI_CBS2_GL; break;
                              case 'n': *lang_type = MMI_CBS2_GN; break;
                              case 'u': *lang_type = MMI_CBS2_GU; break;
                             }break;
      case 'h':switch (char2){case 'a': *lang_type = MMI_CBS2_HA; break;
                              case 'e': *lang_type = MMI_CBS2_HE; break;
                              case 'i': *lang_type = MMI_CBS2_HI; break;
                              case 'r': *lang_type = MMI_CBS2_HR; break;
                              case 'u': *lang_type = MMI_CBS2_HU; break;
                              case 'y': *lang_type = MMI_CBS2_HY; break;
                             }break;
      case 'i':switch (char2){case 'a': *lang_type = MMI_CBS2_IA; break;
                              case 'd': *lang_type = MMI_CBS2_ID; break;
                              case 'e': *lang_type = MMI_CBS2_IE; break;
                              case 'k': *lang_type = MMI_CBS2_IK; break;
                              case 's': *lang_type = MMI_CBS2_IS; break;
                              case 't': *lang_type = MMI_CBS2_IT; break;
                              case 'u': *lang_type = MMI_CBS2_IU; break;
                             }break;
      case 'j':switch (char2){case 'a': *lang_type = MMI_CBS2_JA; break;
                              case 'w': *lang_type = MMI_CBS2_JW; break;
                             }break;
      case 'k':switch (char2){case 'a': *lang_type = MMI_CBS2_KA; break;
                              case 'k': *lang_type = MMI_CBS2_KK; break;
                              case 'l': *lang_type = MMI_CBS2_KL; break;
                              case 'm': *lang_type = MMI_CBS2_KM; break;
                              case 'n': *lang_type = MMI_CBS2_KN; break;
                              case 'o': *lang_type = MMI_CBS2_KO; break;
                              case 's': *lang_type = MMI_CBS2_KS; break;
                              case 'u': *lang_type = MMI_CBS2_KU; break;
                              case 'y': *lang_type = MMI_CBS2_KY; break;
                             }break;
      case 'l':switch (char2){case 'a': *lang_type = MMI_CBS2_LA; break;
                              case 'n': *lang_type = MMI_CBS2_LN; break;
                              case 'o': *lang_type = MMI_CBS2_LO; break;
                              case 't': *lang_type = MMI_CBS2_LT; break;
                              case 'v': *lang_type = MMI_CBS2_LV; break;
                             }break;
      case 'm':switch (char2){case 'g': *lang_type = MMI_CBS2_MG; break;
                              case 'i': *lang_type = MMI_CBS2_MI; break;
                              case 'k': *lang_type = MMI_CBS2_MK; break;
                              case 'l': *lang_type = MMI_CBS2_ML; break;
                              case 'n': *lang_type = MMI_CBS2_MN; break;
                              case 'o': *lang_type = MMI_CBS2_MO; break;
                              case 'r': *lang_type = MMI_CBS2_MR; break;
                              case 's': *lang_type = MMI_CBS2_MS; break;
                              case 't': *lang_type = MMI_CBS2_MT; break;
                              case 'y': *lang_type = MMI_CBS2_MY; break;
                             }break;
      case 'n':switch (char2){case 'a': *lang_type = MMI_CBS2_NA; break;
                              case 'e': *lang_type = MMI_CBS2_NE; break;
                              case 'l': *lang_type = MMI_CBS2_NL; break;
                              case 'o': *lang_type = MMI_CBS2_NO; break;
                             }break;
      case 'o':switch (char2){case 'c': *lang_type = MMI_CBS2_OC; break;
                              case 'm': *lang_type = MMI_CBS2_OM; break;
                              case 'r': *lang_type = MMI_CBS2_OR; break;
                             }break;
      case 'p':switch (char2){case 'a': *lang_type = MMI_CBS2_PA; break;
                              case 'l': *lang_type = MMI_CBS2_PL; break;
                              case 's': *lang_type = MMI_CBS2_PS; break;
                              case 't': *lang_type = MMI_CBS2_PT; break;
                             }break;
      case 'q':switch (char2){case 'u': *lang_type = MMI_CBS2_QU; break;
                             }break;
      case 'r':switch (char2){case 'm': *lang_type = MMI_CBS2_RM; break;
                              case 'n': *lang_type = MMI_CBS2_RN; break;
                              case 'o': *lang_type = MMI_CBS2_RO; break;
                              case 'u': *lang_type = MMI_CBS2_RU; break;
                              case 'w': *lang_type = MMI_CBS2_RW; break;
                             }break;
      case 's':switch (char2){case 'a': *lang_type = MMI_CBS2_SA; break;
                              case 'd': *lang_type = MMI_CBS2_SD; break;
                              case 'g': *lang_type = MMI_CBS2_SG; break;
                              case 'h': *lang_type = MMI_CBS2_SH; break;
                              case 'i': *lang_type = MMI_CBS2_SI; break;
                              case 'k': *lang_type = MMI_CBS2_SK; break;
                              case 'l': *lang_type = MMI_CBS2_SL; break;
                              case 'm': *lang_type = MMI_CBS2_SM; break;
                              case 'n': *lang_type = MMI_CBS2_SN; break;
                              case 'o': *lang_type = MMI_CBS2_SO; break;
                              case 'q': *lang_type = MMI_CBS2_SQ; break;
                              case 'r': *lang_type = MMI_CBS2_SR; break;
                              case 's': *lang_type = MMI_CBS2_SS; break;
                              case 't': *lang_type = MMI_CBS2_ST; break;
                              case 'u': *lang_type = MMI_CBS2_SU; break;
                              case 'v': *lang_type = MMI_CBS2_SV; break;
                              case 'w': *lang_type = MMI_CBS2_SW; break;
                             }break;
      case 't':switch (char2){case 'a': *lang_type = MMI_CBS2_TA; break;
                              case 'e': *lang_type = MMI_CBS2_TE; break;
                              case 'g': *lang_type = MMI_CBS2_TG; break;
                              case 'h': *lang_type = MMI_CBS2_TH; break;
                              case 'i': *lang_type = MMI_CBS2_TI; break;
                              case 'k': *lang_type = MMI_CBS2_TK; break;
                              case 'l': *lang_type = MMI_CBS2_TL; break;
                              case 'n': *lang_type = MMI_CBS2_TN; break;
                              case 'o': *lang_type = MMI_CBS2_TO; break;
                              case 'r': *lang_type = MMI_CBS2_TR; break;
                              case 's': *lang_type = MMI_CBS2_TS; break;
                              case 't': *lang_type = MMI_CBS2_TT; break;
                              case 'w': *lang_type = MMI_CBS2_TW; break;
                             }break;
      case 'u':switch (char2){case 'g': *lang_type = MMI_CBS2_UG; break;
                              case 'k': *lang_type = MMI_CBS2_UK; break;
                              case 'r': *lang_type = MMI_CBS2_UR; break;
                              case 'z': *lang_type = MMI_CBS2_UZ; break;
                             }break;
      case 'v':switch (char2){case 'i': *lang_type = MMI_CBS2_VI; break;
                              case 'o': *lang_type = MMI_CBS2_VO; break;
                             }break;
      case 'w':switch (char2){case 'o': *lang_type = MMI_CBS2_WO; break;
                             }break;
      case 'x':switch (char2){case 'h': *lang_type = MMI_CBS2_XH; break;
                             }break;
      case 'y':switch (char2){case 'i': *lang_type = MMI_CBS2_YI; break;
                              case 'o': *lang_type = MMI_CBS2_YO; break;
                             }break;
      case 'z':switch (char2){case 'a': *lang_type = MMI_CBS2_ZA; break;
                              case 'h': *lang_type = MMI_CBS2_ZH; break;
                              case 'u': *lang_type = MMI_CBS2_ZU; break;
                             }break;
   }
}


/*****************************************************************************
* FUNCTION
*  DecodeCbsDcs
* DESCRIPTION
*   Decode Cb DCS
*
* PARAMETERS
*  dcs  	IN		DCS
*  char1  IN		char 1
*  char2  IN		char 2
*  char3  IN		char 3
*  lang_type  		IN/OUT 	language type
*  alphabet_type  	IN/OUT 	alphabet type
*  msg_class  		IN/OUT 	message class
*  is_compress  	IN/OUT 	is compress
*  is_reserved  	IN/OUT 	is reserved
* RETURNS
*  skip character
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 DecodeCbsDcs(U8 dcs, U8 char1, U8 char2, U8 char3, msg_cb_lang_enum *lang_type,
                       smsal_dcs_enum *alphabet_type, msg_cb_class_enum *msg_class,
                       pBOOL *is_compress, pBOOL *is_reserved )
{
   kal_uint8 coding_group = (dcs >> 4);
   kal_uint8 coding_bits = (dcs & 0x0f);
   kal_uint8 skip_char = 0;
   
   *lang_type = MMI_CBS0_LANG_UNSPECIFIED;
   *alphabet_type = SMSAL_DEFAULT_DCS;
   *msg_class = MMI_CBS_CLASS_UNSPECIFIED;

   *is_compress = KAL_FALSE;
   *is_reserved = KAL_FALSE;

   switch (coding_group)
   {
      case 0x00:
         *lang_type = (msg_cb_lang_enum)coding_bits;
         break;
      case 0x01:        
         /* 
          * GSM7: PS side already unpack 
          * UCS2: PS side didn't unpack 
          */
         if (coding_bits == 0) // GSM7
         {
            if (char3 == 0x0d) // CR
            {
               MMICbsDecodeLang(char1, char2, lang_type);
               skip_char = 3;
            }
         }
         else if (coding_bits == 1) // UCS2
         {
            *alphabet_type = SMSAL_UCS2_DCS;
            MMICbsDecodeLang((U8)(char1&0x7f), (U8)(((char2&0x3f)<<1)|((char1&0x80)>>7)), lang_type);
            skip_char = 2;
         }
         //CSD end
         else
         {
            *is_reserved = KAL_TRUE;
         }
         break;
      case 0x02:
         if (coding_bits < 0x04)
            *lang_type = (msg_cb_lang_enum)(16 + coding_bits);
         else
            *is_reserved = KAL_TRUE;
         break;
      case 0x03:
         *is_reserved = KAL_TRUE;
         break;
      case 0x04: /* General Data Coding Scheme */
      case 0x05:
      case 0x06:
      case 0x07:
         if ((coding_group & 0x02) !=0)
            *is_compress = KAL_TRUE;
         if((coding_group & 0x01) == 0x01)
            *msg_class = (msg_cb_class_enum)(dcs & 0x03);
         *alphabet_type = (smsal_dcs_enum)(dcs & 0x0C);
         break;
      case 0x08: /* 0x08 - 0x0d: Reserved */
      case 0x09:
      case 0x0a:
      case 0x0b:
      case 0x0c:
      case 0x0d:
         *is_reserved = KAL_TRUE;
         break;
      case 0x0e: /* defined by WAP forum */
         break;
      case 0x0f: /* Data Coding/Message Class */
         *alphabet_type = (smsal_dcs_enum)(dcs & 0x04);
         *msg_class = (msg_cb_class_enum)(dcs & 0x03);
         break;
      default:
         break;
   }
   return skip_char;   
} /* end of DecodeCbsDcs */


/*****************************************************************************
* FUNCTION
*  DeleteCBMessage
* DESCRIPTION
*   Delete CB message
*
* PARAMETERS
*  index  IN		index
* RETURNS
*  delete length
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 DeleteCBMessage(S32 index)
{
	S32 i=0;
	U8 deleteLen=cbMsgInfo.MsgPage[index];
	//first, clear the content
	memset(cbMsgIndex.pCBMsgContent[index], 0, cbMsgInfo.MsgPage[index]*NVRAM_SMS_CB_SMS_PAGE_SIZE);
	//second, reset the info
	cbMsgInfo.MsgDCS[index]=0;
	cbMsgInfo.MsgPage[index]=0;
	//CSD Joy 0222
	cbMsgInfo.MsgMI[index]=SMS_CB_UNUSED_CBMI;
	//CSD end
	//third, compute the new index
	cbMsgIndex.currCBTotal--;
	//forth, move the content and update the info
	if(index<(cbMsgIndex.currCBTotal))
	{
		for(i=index; i<(cbMsgIndex.currCBTotal); i++)
		{
			memcpy(cbMsgIndex.pCBMsgContent[i], cbMsgIndex.pCBMsgContent[i+1], cbMsgInfo.MsgPage[i+1]*NVRAM_SMS_CB_SMS_PAGE_SIZE);			
			cbMsgInfo.MsgDCS[i]=cbMsgInfo.MsgDCS[i+1];
			cbMsgInfo.MsgPage[i]=cbMsgInfo.MsgPage[i+1];
			//CSD Joy 0222
			cbMsgInfo.MsgMI[i]=cbMsgInfo.MsgMI[i+1];
			//CSD end
			cbMsgIndex.pCBMsgContent[i+1]=cbMsgIndex.pCBMsgContent[i]+(cbMsgInfo.MsgPage[i]*NVRAM_SMS_CB_SMS_PAGE_SIZE);			
		}
	}
	//fifth, clear the endings
	cbMsgIndex.pCBMsgContent[cbMsgIndex.currCBTotal]=NULL;	
	cbMsgInfo.MsgDCS[cbMsgIndex.currCBTotal]=0;
	cbMsgInfo.MsgPage[cbMsgIndex.currCBTotal]=0;
	//CSD Joy 0222
	cbMsgInfo.MsgMI[cbMsgIndex.currCBTotal]=SMS_CB_UNUSED_CBMI;
	//CSD end
	
	if(cbMsgIndex.currCBTotal==0)
		cbMsgIndex.pCBMsgTail=(U8*)cbMsgContent[0];
	else if(cbMsgIndex.currCBTotal<MAX_STORE_CB_MSG)
		cbMsgIndex.pCBMsgTail=cbMsgIndex.pCBMsgContent[cbMsgIndex.currCBTotal-1]+cbMsgInfo.MsgPage[cbMsgIndex.currCBTotal-1]*NVRAM_SMS_CB_SMS_PAGE_SIZE;
	else
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] DeleteCBMessage: total number error ---*\n"));
		cbMsgIndex.pCBMsgTail=NULL;
		return 0;
	}	
	memset(cbMsgIndex.pCBMsgTail, 0, deleteLen*NVRAM_SMS_CB_SMS_PAGE_SIZE);
	return deleteLen;
}

/*****************************************************************************
* FUNCTION
*  AddCBMessage
* DESCRIPTION
*   Add CB message to storage buffer
*
* PARAMETERS
*  cbMI  		IN		cbmi
*  cbDCS  	IN		cb DCS
*  cbPage	IN		cb page number
*  cbMsg  	IN		cb message pointer
*  totalLen  	IN		cb message length
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void AddCBMessage(U16 cbMI, U8 cbDCS, U8 cbPage, PU8 cbMsg,  U16 totalLen)
{
	msg_cb_lang_enum  langType;
	smsal_dcs_enum    alphabetType;
	msg_cb_class_enum   msgClass;
	pBOOL            	 	isCompress;
	pBOOL             		isReserved;

	if((cbMsgIndex.pCBMsgTail==NULL)||((cbMsgIndex.pCBMsgTail)>((cbMsgStart)+NVRAM_SMS_CB_SMS_PAGE_SIZE*NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL)))
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] AddCBMessage: tail address error ---*\n"));
	else
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] AddCBMessage: tail address ok = %d---*\n", cbMsgIndex.pCBMsgTail));
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] AddCBMessage: currCBTotal = %d", cbMsgIndex.currCBTotal));
		cbMsgIndex.currCBTotal++;
		cbMsgIndex.pCBMsgContent[cbMsgIndex.currCBTotal-1]=cbMsgIndex.pCBMsgTail;
		memcpy(cbMsgIndex.pCBMsgContent[cbMsgIndex.currCBTotal-1], cbMsg, totalLen);
		DecodeCbsDcs(cbDCS, cbMsg[0], cbMsg[1], cbMsg[2], &langType, &alphabetType, &msgClass, &isCompress, &isReserved);
		switch(alphabetType)
		{
			case SMSAL_DEFAULT_DCS: 
				memset(cbMsgIndex.pCBMsgContent[cbMsgIndex.currCBTotal-1]+totalLen, 0x20, cbPage*NVRAM_SMS_CB_SMS_PAGE_SIZE-totalLen);
			break;		
			case SMSAL_8BIT_DCS:
			case SMSAL_UCS2_DCS:
				memset(cbMsgIndex.pCBMsgContent[cbMsgIndex.currCBTotal-1]+totalLen, 0x00, cbPage*NVRAM_SMS_CB_SMS_PAGE_SIZE-totalLen);
			break;
			default:
				break;
		}
		cbMsgInfo.MsgDCS[cbMsgIndex.currCBTotal-1]=cbDCS;
		cbMsgInfo.MsgPage[cbMsgIndex.currCBTotal-1]=cbPage;
		//CSD Joy 0222
		cbMsgInfo.MsgMI[cbMsgIndex.currCBTotal-1]=cbMI;
		//CSD end
		if (((cbMsgIndex.pCBMsgTail)+cbPage*NVRAM_SMS_CB_SMS_PAGE_SIZE)<((cbMsgStart)+NVRAM_SMS_CB_SMS_PAGE_SIZE*NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL))
			(cbMsgIndex.pCBMsgTail)+=(cbPage*NVRAM_SMS_CB_SMS_PAGE_SIZE);
		else
			cbMsgIndex.pCBMsgTail=NULL;
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] AddCBMessage: tail address ok = %d---*\n", cbMsgIndex.pCBMsgTail));		
	}	
}

/*****************************************************************************
* FUNCTION
*  ReadCbChnlInfoFromNvram
* DESCRIPTION
 
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ReadCbChnlInfoFromNvram (void)
{
	S16	error=0;
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] ReadCbChnlInfoFromNvram ---*\n"));
	if (ReadRecord (NVRAM_EF_SMS_CB_CHNL_LID, 1 ,&cbNvramStruct, NVRAM_SMS_CB_CHNL_RECORD_SIZE, &error) < 0)
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] ResetCBInfoStructs-failed ---*\n"));
	}
}

/*****************************************************************************
* FUNCTION
*  WriteCbChnlInfoToNvram
* DESCRIPTION
*   Write CB channel info to NVRAM
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void WriteCbChnlInfoToNvram (void)
{
	S16	error=0;
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] WriteCbChnlInfoToNvram ---*\n"));
	if (WriteRecord (NVRAM_EF_SMS_CB_CHNL_LID, 1,&cbNvramStruct, NVRAM_SMS_CB_CHNL_RECORD_SIZE, &error) != NVRAM_SMS_CB_CHNL_RECORD_SIZE)
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] WriteCbChnlInfoToNvram-: failed (ecode%d) ---*\n",(int) error));
	}
}

/*****************************************************************************
* FUNCTION
*  ReadCBMessageInfoFromNvram
* DESCRIPTION
*   Read CB Message info from NVRAM
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ReadCBMessageInfoFromNvram (void)
{
	S16	error=0;
	U16 cbTotalLen=0;
	INT i=0, j=0; 
	S8 readBase=0;
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] ReadCBMessageInfoFromNvram ---*\n"));
	if (ReadRecord (NVRAM_EF_SMS_CB_SMS_INFO_LID, 1 ,&cbMsgInfo, NVRAM_SMS_CB_SMS_INFO_SIZE, &error) < 0)
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] ReadCBMessageInfoFromNvram-(code=%d) ---*\n",(int)error));
	cbMsgStart=(U8*)cbMsgContent[0];	
	for (i=0; i<MAX_STORE_CB_MSG; i++)
	{
		if (cbMsgInfo.MsgPage[i]==0)
			break;
		for (j=readBase; (j<(readBase+cbMsgInfo.MsgPage[i]))&&(j<NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL); j++)
		{
			U16 index = (U16)j + 1;
			if (ReadRecord (NVRAM_EF_SMS_CB_SMS_LID, index ,cbMsgContent[j], NVRAM_SMS_CB_SMS_PAGE_SIZE, &error) < 0)
				PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] ReadCBMessageInfoFromNvram-(code=%d) ---*\n",(int)error));
		}
		cbMsgIndex.pCBMsgContent[i]=cbMsgStart+cbTotalLen;	
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] ReadCBMessageInfoFromNvram-address=%d ---*\n", cbMsgIndex.pCBMsgContent[i]));		
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] CB[%d] dcs=%d page=%d", i, cbMsgInfo.MsgDCS[i], cbMsgInfo.MsgPage[i]));		
		cbTotalLen+=(U16)(cbMsgInfo.MsgPage[i]*NVRAM_SMS_CB_SMS_PAGE_SIZE);
		readBase=readBase+(S8)cbMsgInfo.MsgPage[i];		
	}
	cbMsgIndex.currCBTotal=i;
	if (cbTotalLen==NVRAM_SMS_CB_SMS_PAGE_SIZE*NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL)
		cbMsgIndex.pCBMsgTail=NULL;
	else
		cbMsgIndex.pCBMsgTail=cbMsgStart+cbTotalLen;
}

/*****************************************************************************
* FUNCTION
*  WriteCBMessageInfoToNvram
* DESCRIPTION
*   Write CB message info to NVRAM
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void WriteCBMessageInfoToNvram (void)
{
	INT i=0, j=0; 
	S8 writeBase=0;
	S16	error=0;
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] cbmsgaddress=%d", &cbMsgInfo));
	if (WriteRecord (NVRAM_EF_SMS_CB_SMS_INFO_LID, 1 ,&cbMsgInfo, NVRAM_SMS_CB_SMS_INFO_SIZE, &error) !=NVRAM_SMS_CB_SMS_INFO_SIZE)
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] WriteCBMessageInfoFromNvram-(code=%d) ---*\n",(int)error));
	}
	for (i=0; i<MAX_STORE_CB_MSG; i++)
	{
		if (cbMsgInfo.MsgPage[i]==0)
			break;
		for (j=writeBase; (j<(writeBase+cbMsgInfo.MsgPage[i]))&&(j<NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL); j++)
		{
			U16 index = (U16)j + 1;
			PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] cbmsgcontent=%d", cbMsgContent[i]));		
			if (WriteRecord (NVRAM_EF_SMS_CB_SMS_LID, index ,cbMsgContent[j], NVRAM_SMS_CB_SMS_PAGE_SIZE, &error) !=NVRAM_SMS_CB_SMS_PAGE_SIZE)
			{
				PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] WriteCBMessageInfoFromNvram-(code=%d) ---*\n",(int)error));
			}
			}
		writeBase=writeBase+(S8)cbMsgInfo.MsgPage[i];				
		}

}
/*****************************************************************************
* FUNCTION
*  ConvertCBForDisplay
* DESCRIPTION
*   Convert CB message for display, according to its DCS
*
* PARAMETERS
*  data  	IN	message data
*  len  	IN    message length
*  format  	IN    message format
*  outLen  	IN/OUT    message output length
* RETURNS
*  none
* GLOBALS AFFECTED
*   dispCBMessage
*****************************************************************************/
void ConvertCBForDisplay(U8 *data, U16 len, U8 format, U32 *outLen)
{
	U16 outLength;
	if(len > 0)
   	{
		switch (format)
      		{
      			case (MMI_DEFAULT_DCS):
      			{
				U8 *tempData;
				tempData=OslMalloc(len);;
				ConvertGSM7BitDefaultEncodingToAscii(tempData,data,len,&outLength);
				AnsiiNToUnicodeString((PS8)dispCBMessage,(PS8)tempData,outLength);
				memset((void *)&dispCBMessage[outLength*ENCODING_LENGTH],0,ENCODING_LENGTH);
				OslMfree(tempData);				
				if(outLen!=NULL) *outLen=(U32)outLength*ENCODING_LENGTH+ENCODING_LENGTH;
      			}
			break;

			case (MMI_8BIT_DCS):
				AnsiiNToUnicodeString((PS8)dispCBMessage,(PS8)data,len);
				memset((void *)&dispCBMessage[len*ENCODING_LENGTH],0,ENCODING_LENGTH);
				if(outLen!=NULL) *outLen=len*ENCODING_LENGTH+ENCODING_LENGTH;
        		break;

			case (MMI_UCS2_DCS):
				memset((void *)dispCBMessage,0,len+1+ENCODING_LENGTH);			
				memcpy(dispCBMessage,data,len);
				if(outLen!=NULL) *outLen=len+ENCODING_LENGTH;
				#ifdef MMI_ON_HARDWARE_P
				{
					U16 *tempData;
					int i;
					tempData=(U16 *)dispCBMessage;
					for(i=0;i<len;i+=2)
					{
						SWAP_USHORT(tempData);
						tempData++;
					}
				}
				#endif
			break;
   		}
		return;
	}
	else
	{
		memset((void *)dispCBMessage,0,ENCODING_LENGTH);
		if(outLen!=NULL) *outLen=ENCODING_LENGTH;
	 	return;
	}
}

/*****************************************************************************
* FUNCTION
*  ReadCBMessageContent
* DESCRIPTION
*   Read CB message content for dislpay
*
* PARAMETERS
*  outLen  IN/OUT      	output length
*  msgData  	IN		message data     
*  msgLength 	IN		message length     
*  dcs  		IN		message DCS    
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ReadCBMessageContent (PU32 outLen, PU8 msgData, U16 msgLength, U8 dcs)
{
	msg_cb_lang_enum  langType;
	smsal_dcs_enum    alphabetType;
	msg_cb_class_enum   msgClass;
	pBOOL            	 	isCompress;
	pBOOL             		isReserved;

	//CSD add Kevin
	U8 skipChar;
	//CSD end

	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] ReadCBMessageContent ---*\n"));

        //CSD add Kevin
	skipChar = DecodeCbsDcs(dcs, msgData[0], msgData[1], msgData[2], &langType, &alphabetType, &msgClass, &isCompress, &isReserved);
        //CSD end

	if(isCompress==TRUE || isReserved==TRUE)
	{
		DisplayPopup ((PU8)GetString (STR_SMS_DOES_NOT_SUPPORT), IMG_GLOBAL_WARNING, 0, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);
		DeleteMessagesHistoryNodes ();
	}
	else
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] alphabetType=%d ---*\n", alphabetType));
		switch(alphabetType)
		{
			//CSD add Kevin for skipping some characters containing dcs info 
			case SMSAL_DEFAULT_DCS: 
				ConvertCBForDisplay (msgData+skipChar, (U16)(msgLength-skipChar), SMSAL_DEFAULT_DCS, outLen);
				return;
			case SMSAL_8BIT_DCS: 
				ConvertCBForDisplay (msgData+skipChar, (U16)(msgLength-skipChar), SMSAL_8BIT_DCS, outLen);
				return;
			case SMSAL_UCS2_DCS: 
				ConvertCBForDisplay (msgData+skipChar, (U16)(msgLength-skipChar), SMSAL_UCS2_DCS, outLen);
				return;
		        //CSD end
			case SMSAL_RESERVED_DCS: 	
				DisplayPopup ((PU8)GetString (STR_SMS_DOES_NOT_SUPPORT), IMG_GLOBAL_WARNING, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);
				DeleteMessagesHistoryNodes ();
				return;
			default:
				ConvertCBForDisplay (msgData+skipChar, (U16)(msgLength-skipChar), SMSAL_DEFAULT_DCS, outLen);
				return;
		}
	}
	return;
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_get_cb_mode_req
* DESCRIPTION
*   Get CB mode
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_get_cb_mode_req (void)
{
	MYQUEUE			Message; 
	Message.oslSrcId	=	MOD_MMI;
	Message.oslDestId	=	MOD_L4C;
	Message.oslMsgId	=	PRT_MSG_ID_MMI_SMS_GET_CB_MODE_REQ;
	Message.oslDataPtr		=	NULL;
	Message.oslPeerBuffPtr	=	NULL;
	OslMsgSendExtQueue(&Message);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_cb_mode_rsp_active
* DESCRIPTION
*   Get CB mode for current status
*
* PARAMETERS
 
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_get_cb_mode_rsp_active (void* inMsg)
{
	MMI_FRM_CB_GET_CB_MODE_RSP_STRUCT *msgRsp = (MMI_FRM_CB_GET_CB_MODE_RSP_STRUCT*)inMsg;
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] GetCBServiceModeRsp ---*\n"));
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_GET_CB_MODE_RSP);	
	/* if incoming call ringing or alarm playing, do not show response result in case interrupt current screen */
	if (mmi_msg_check_interrupt() == TRUE)
		return;
	if(msgRsp->result==TRUE)
	{
		cb_info.mode = msgRsp->mode;
		memset(cb_display.mode, 0, ENCODING_LENGTH);
		if(cb_info.mode==TRUE)
			pfnUnicodeStrcpy((S8*)cb_display.mode, (S8*)GetString((U16)STR_GLOBAL_ON));
		else
			pfnUnicodeStrcpy((S8*)cb_display.mode, (S8*)GetString((U16)STR_GLOBAL_OFF));
		mmi_msg_entry_cb_main_menu();
	}
	else
		DisplayPopup ((PU8)GetString(STR_CB_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_cb_mode_rsp_display
* DESCRIPTION
*   Get CB mode to display channel list
*
* PARAMETERS
 
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_get_cb_mode_rsp_display (void* inMsg)
{
	MMI_FRM_CB_GET_CB_MODE_RSP_STRUCT *msgRsp = (MMI_FRM_CB_GET_CB_MODE_RSP_STRUCT*)inMsg;
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] GetCBServiceModeRsp ---*\n"));
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_GET_CB_MODE_RSP);
	/* if incoming call ringing or alarm playing, do not show response result in case interrupt current screen */
	if (mmi_msg_check_interrupt() == TRUE)
		return;
	if(msgRsp->result==TRUE)
	{
		U8 i=0;
		U8 total=0;
		U8 addindex=msgRsp->total_mid_num;
		/*delete if apply new interface*/
		ReadCbChnlInfoFromNvram();
		/*end*/
		for(i=0; i<msgRsp->total_mid_num; i++)
		{
			if(msgRsp->info.mid[i]!=SMSAL_CB_UNUSED_CBMI)
			{
				U8 j=0;
				cb_info.cbchnlentry[total].mi=msgRsp->info.mid[i];
				cb_info.cbchnlentry[total].mask=msgRsp->info.mid_mask[i];
				cb_info.cbchnlentry[total].index=i;
				/*delete if apply new interface*/
				memset(cb_info.cbchnlentry[total].name, 0, MAX_CB_CHNL_NAME_LENGTH+1);
				while((cbNvramStruct.l4MidIndex[j]!=i)&&(j<SMS_CB_MAX_ENTRY))
					j++;
				if(j==SMS_CB_MAX_ENTRY)
				{
					memset((S8*)cb_info.cbchnlentry[total].name, 0, MAX_CB_CHNL_NAME_LENGTH);
					cb_info.cbchnlentry[total].dcs=PHB_ASCII;
				}
				else
				{
					memcpy((S8*)cb_info.cbchnlentry[total].name, (S8*)cbNvramStruct.chnlNames[j].name, MAX_CB_CHNL_NAME_LENGTH);
					cb_info.cbchnlentry[total].dcs=cbNvramStruct.chnlNames[j].name_dcs;
				}
				/*end*/
				total++;
			}
			else
			{
				if(addindex==msgRsp->total_mid_num)
					addindex=i;
			}
		}
		cb_info.totalmi=total;
		memset(&cbNvramStruct, 0xff, sizeof(msg_cb_channel_nvram_struct));
		for(i=0; i<cb_info.totalmi; i++)
		{
			cbNvramStruct.l4MidIndex[i]=cb_info.cbchnlentry[i].index;
			cbNvramStruct.chnlNames[i].name_dcs=cb_info.cbchnlentry[i].dcs;
			memcpy((S8*)cbNvramStruct.chnlNames[i].name, (S8*)cb_info.cbchnlentry[i].name, MAX_CB_CHNL_NAME_LENGTH+1);
		}
		/*delete if apply new interface*/
		WriteCbChnlInfoToNvram();
		/*end*/
		switch(cb_state)
		{
			case SMS_CB_CHNL_SELECT_STATE:
				if(cb_info.totalmi==0)
					DisplayPopup ((PU8)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_WARNING, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);
				else
					mmi_msg_entry_cb_channel_select();		
				break;
			case SMS_CB_CHNL_EDIT_STATE:	
			case SMS_CB_CHNL_DELETE_STATE:
				if(cb_info.totalmi==0)
					DisplayPopup ((PU8)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_WARNING, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);
				else
					mmi_msg_entry_cb_channel_list();			
				break;
			case SMS_CB_CHNL_ADD_STATE:
				if(cb_info.totalmi==msgRsp->total_mid_num)
					DisplayPopup ((PU8)GetString (STR_SMS_CB_CHNL_FULL), IMG_GLOBAL_WARNING, 1, MESSAGES_POPUP_TIME_OUT, (U8)WARNING_TONE);
				else
				{
					cb_display.addindex=addindex;
					memset(cb_display.name, 0, (MAX_CB_CHNL_NAME_LENGTH+1)*ENCODING_LENGTH);
					memset(cb_display.mi, 0, (MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1)*ENCODING_LENGTH);	
					cb_display.dcs=PHB_ASCII;
					mmi_msg_entry_cb_channel_edit();
				}
				break;
			case SMS_CB_READ_MESSAGE_STATE:		
				mmi_msg_entry_cb_read_message();			
				break;	
			default:
				PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[CellBroadcast.c] cb_info is filled up-------*\n"));
				break;
		}
	}
	else
		DisplayPopup ((PU8)GetString (STR_CB_ERROR_MESSAGE), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_get_cb_mode_rsp_language
* DESCRIPTION
*   Get CB mode to display CB language list
*
* PARAMETERS
 
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_get_cb_mode_rsp_language (void* inMsg)
{
	MMI_FRM_CB_GET_CB_MODE_RSP_STRUCT *msgRsp = (MMI_FRM_CB_GET_CB_MODE_RSP_STRUCT*)inMsg;
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] GetCBServiceModeRsp ---*\n"));
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_GET_CB_MODE_RSP);
	/* if incoming call ringing or alarm playing, do not show response result in case interrupt current screen */
	if (mmi_msg_check_interrupt() == TRUE)
		return;
	if(msgRsp->result==TRUE)
	{
		U8 i=0;
		U8 total=0;
		U8 firstByte=0;
		U8 secondByte=0;

		pBOOL            	 			isCompress;
		pBOOL             			isReserved;
		msg_cb_lang_enum  	langType;
		smsal_dcs_enum    	alphabetType;
		msg_cb_class_enum   msgClass;

		for(i=0; i<msgRsp->total_dcs_num; i++)
		{
			if(msgRsp->info.dcs[i]!=SMSAL_CB_UNUSED_DCS)
			{

				firstByte = (msgRsp->info.dcs[i] & 0xff00) >> 8; 
				secondByte = (msgRsp->info.dcs[i] & 0x00ff);
				if(firstByte==0x00)
					DecodeCbsDcs(secondByte, 0, 0, 0, &langType, &alphabetType, &msgClass, &isCompress, &isReserved);
				else
					MMICbsDecodeLang(firstByte, secondByte, &langType);

				if((langType!=MMI_CBS0_LANG_UNSPECIFIED)&&(langType!=MMI_CBS_INVALID))
				{
					cb_info.cblanentry[total].dcs=langType;
					cb_info.cblanentry[total].mask=msgRsp->info.dcs_mask[i];
					cb_info.cblanentry[total].index=i;
					total++;
				}
			}
			firstByte = 0;
			secondByte = 0;
		}
		cb_info.totaldcs=total;
		cb_info.dcsall=msgRsp->info.is_all_language_on;
		mmi_msg_entry_cb_language();
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_set_cb_mode_req
* DESCRIPTION
*   Set CB mode request
*
* PARAMETERS
*  para  IN	setting type flag
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_set_cb_mode_req (U8 para)
{
	MYQUEUE											Message; 
	MMI_FRM_CB_SET_CB_MODE_REQ_STRUCT			*msgReq; 
	U8 i=0;
	WriteCbChnlInfoToNvram ();
	Message.oslSrcId	=	MOD_MMI;
	Message.oslDestId	=	MOD_L4C;
	Message.oslMsgId	=	PRT_MSG_ID_MMI_SMS_SET_CB_MODE_REQ;
	msgReq = (MMI_FRM_CB_SET_CB_MODE_REQ_STRUCT*)OslConstructDataPtr(sizeof(MMI_FRM_CB_SET_CB_MODE_REQ_STRUCT));

	if((para & SMSAL_CB_MI)==SMSAL_CB_MI)
	{
		for(i=0; i<SMS_CB_MAX_ENTRY; i++)
			msgReq->info.mid[i]=SMSAL_CB_UNUSED_CBMI;
		for(i=0; i<cb_info.totalmi; i++)
			msgReq->info.mid[cb_info.cbchnlentry[i].index]=cb_info.cbchnlentry[i].mi;
	}
	if(((para & SMSAL_CB_MI)==SMSAL_CB_MI)||((para & SMSAL_CB_MI_MASK)==SMSAL_CB_MI_MASK))
	{
		for(i=0; i<SMS_CB_MAX_ENTRY; i++)
			msgReq->info.mid_mask[i]=1;
		for(i=0; i<cb_info.totalmi; i++)
			msgReq->info.mid_mask[cb_info.cbchnlentry[i].index]=cb_info.cbchnlentry[i].mask;
	}
	if((para & SMSAL_CB_DCS_MASK)==SMSAL_CB_DCS_MASK)
	{
		for(i=0; i<SMS_CB_MAX_LANG; i++)
			msgReq->info.dcs_mask[i]=1;
		for(i=0; i<cb_info.totaldcs; i++)
			msgReq->info.dcs_mask[cb_info.cblanentry[i].index]=cb_info.cblanentry[i].mask;
	}
	if ((para & SMSAL_CB_ALL_LANG)==SMSAL_CB_ALL_LANG)
		msgReq->info.is_all_language_on = cb_info.dcsall;
	msgReq->info.para_ind	=	para;
	Message.oslDataPtr		=	(oslParaType*)msgReq;
	Message.oslPeerBuffPtr	=	NULL;
	OslMsgSendExtQueue(&Message);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_set_cb_mode_rsp
* DESCRIPTION
*   Set CB mode response
*
* PARAMETERS
 
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_set_cb_mode_rsp (void* inMsg)
{
	MMI_FRM_CB_SET_CB_MODE_RSP_STRUCT *msgRsp = (MMI_FRM_CB_SET_CB_MODE_RSP_STRUCT*)inMsg;
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_SMS_SET_CB_MODE_RSP);
	if(msgRsp->result == TRUE)
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] SetCBServiceModeRsp-ok ---*\n"));
		switch(cb_state)
		{
			case SMS_CB_DEFAULT_SETTINGS_MODE_STATE:
				if(msgRsp->result == TRUE)
					RstRestoreDisplayPopup(TRUE);
				else
					RstRestoreDisplayPopup(FALSE);		
				break;
			case SMS_CB_CHNL_SELECT_STATE:
			case SMS_CB_CHNL_ADD_STATE:
			case SMS_CB_CHNL_EDIT_STATE:
				DisplayPopup ((PU8)GetString (STR_GLOBAL_SAVED), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
				DeleteMessagesHistoryNodes ();
			break;
			case SMS_CB_CHNL_DELETE_STATE:
				if(g_msg_cntx.currHiliteIndex==cb_info.totalmi)
					g_msg_cntx.currHiliteIndex--;
				DisplayPopup ((PU8)GetString (STR_MESSAGES_DELETED_ID), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
				if(cb_info.totalmi==0)
					DeleteScreenIfPresent(SCR_ID_MSG_CB_CHANNEL_LIST);
			break;
			case SMS_CB_LANGUAGE_STATE:
				if(msgRsp->result == TRUE)
				{
					DisplayPopup((PU8)GetString(STR_GLOBAL_SAVED), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
					DeleteMessagesHistoryNodes();
				}
				else
				{
					DisplayPopup((PU8)GetString (STR_MESSAGES_UNKNOWN_ERROR), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
					DeleteMessagesHistoryNodes();
				}	
			break;
			default:
				break;
		}
	}
	else
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] SetCBServiceModeRsp-failed ---*\n"));
		if(cb_state==SMS_CB_DEFAULT_SETTINGS_MODE_STATE)
			RstRestoreDisplayPopup (FALSE);
		else
		{
			SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_CB_CHANNEL_LIST);
			DisplayPopup ((PU8)GetString (STR_CB_ERROR_MESSAGE), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			DeleteMessagesHistoryNodes ();
		}
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_subscribe_cb_req
* DESCRIPTION
*   Subscribe CB request
*
* PARAMETERS
*  subscribe  IN	subscribe type
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_subscribe_cb_req (U8 subscribe)
{
	MYQUEUE									Message; 
	MMI_FRM_CB_SUBSCRIBE_REQ_STRUCT *msgReq; 
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] EnableOrDiasableCBServiceReq ---*\n"));
	Message.oslSrcId	=	MOD_MMI;
	Message.oslDestId	=	MOD_L4C;
	Message.oslMsgId	=	PRT_MSG_ID_MMI_CB_SUBSCRIBE_REQ;
	msgReq = (MMI_FRM_CB_SUBSCRIBE_REQ_STRUCT*)OslConstructDataPtr(sizeof(MMI_FRM_CB_SUBSCRIBE_REQ_STRUCT)); 
	msgReq->on_off		=	subscribe;
	Message.oslDataPtr		=	(oslParaType*)msgReq;
	Message.oslPeerBuffPtr	=	NULL;
	OslMsgSendExtQueue(&Message);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_subscribe_cb_rsp
* DESCRIPTION
*   Subscribe CB response
*
* PARAMETERS
 
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_subscribe_cb_rsp(void* inMsg)
{
	MMI_FRM_CB_SUBSCRIBE_RSP_STRUCT* msgRsp = (MMI_FRM_CB_SUBSCRIBE_RSP_STRUCT*)inMsg;
	ClearProtocolEventHandler(PRT_MSG_ID_MMI_CB_SUBSCRIBE_RSP);
	if(msgRsp->result == TRUE)
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] mmi_msg_subscribe_cb_rsp -OK ---*\n"));
		if(cb_state==SMS_CB_DEFAULT_SETTINGS_MODE_STATE)
		{
			U8 i=0;
			for(i=0; i<SMS_CB_MAX_ENTRY; i++)
				cb_info.cbchnlentry[i].mask=1;
                        for(i=0; i<SMS_CB_MAX_LANG; i++)
				cb_info.cblanentry[i].mask=1;
			cb_info.dcsall=1;
			SetProtocolEventHandler(mmi_msg_set_cb_mode_rsp, PRT_MSG_ID_MMI_SMS_SET_CB_MODE_RSP);
			mmi_msg_set_cb_mode_req(SMSAL_CB_MI_MASK|SMSAL_CB_DCS_MASK);
		}
		else
		{
			U8 subscribe=(!cb_info.mode);
			cb_info.mode=subscribe;
			memset(cb_display.mode, 0, ENCODING_LENGTH);
			if(cb_info.mode==TRUE)
				pfnUnicodeStrcpy((S8*)cb_display.mode, (S8*)GetString((U16)STR_GLOBAL_ON));
			else
				pfnUnicodeStrcpy((S8*)cb_display.mode, (S8*)GetString((U16)STR_GLOBAL_OFF));
			mmi_msg_refresh_cb_main_menu();
		}
	}
	else
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] mmi_msg_subscribe_cb_rsp-Failed ---*\n"));
		if(cb_state==SMS_CB_DEFAULT_SETTINGS_MODE_STATE)
			RstRestoreDisplayPopup (FALSE);
		else
			DisplayPopup ((PU8)GetString (STR_CB_ERROR_MESSAGE), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	}

}

/*****************************************************************************
* FUNCTION
*  mmi_msg_handle_new_cb_ind
* DESCRIPTION
*   Handle new CB indication
*
* PARAMETERS
 
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_handle_new_cb_ind (void* inMsg)
{
	MMI_FRM_CB_MSG_IND_STRUCT* CbsMsg = (MMI_FRM_CB_MSG_IND_STRUCT*)inMsg;
	U8 i=0;
	U8 deleteIndex=0;
	U16 inCBMI=SMS_CB_UNUSED_CBMI;
	U8 inCBDCS=0;
	U8 inCBPage=0;
	U8 currCBPageTotal=0;
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] mmi_msg_handle_new_cb_ind ---*\n"));
	if (IsCBMessageReadOperRequired)
	{
		IsCBMessageReadOperRequired=FALSE;
		#ifdef MMI_ON_WIN32
		cbMsgStart=(U8*)cbMsgContent[0];
		cbMsgIndex.pCBMsgTail=(U8*)cbMsgContent[0];
		#else		
		ReadCBMessageInfoFromNvram ();
		#endif
	}
	inCBMI=CbsMsg->mi;
	inCBDCS=CbsMsg->dcs;
	inCBPage=((CbsMsg->msg_length)/NVRAM_SMS_CB_SMS_PAGE_SIZE);
	if(((CbsMsg->msg_length)%NVRAM_SMS_CB_SMS_PAGE_SIZE)!=0)
		inCBPage++;
	inCBPage=(inCBPage>MAX_CB_MSG_PAGE)?MAX_CB_MSG_PAGE:inCBPage;
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] Page= %d---*\n", inCBPage));	
	for (i=0;i<cbMsgIndex.currCBTotal; i++)
		currCBPageTotal+=cbMsgInfo.MsgPage[i];
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] CurrTotalPage= %d---*\n", currCBPageTotal));
	if ((currCBPageTotal+inCBPage)<NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL)
	{
		if(cbMsgIndex.currCBTotal== MAX_STORE_CB_MSG)
		{
			if(cb_display.currindex==MAX_STORE_CB_MSG-1)
			{
				deleteIndex=0;
				cb_display.currindex--;
			}	
			else
				deleteIndex=MAX_STORE_CB_MSG-1;
			DeleteCBMessage(deleteIndex);
		}
	}
	else
	{
		if(cb_display.currindex==(cbMsgIndex.currCBTotal-1))
		{
			for (i=0; i<cbMsgIndex.currCBTotal-1; i++)
			{
				currCBPageTotal-=DeleteCBMessage(0);
				cb_display.currindex--;			
				if((currCBPageTotal+inCBPage)<=NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL)
					break;
			}
		}
		else
		{
			for (i=cbMsgIndex.currCBTotal-1; ; i--)
			{
				if((cb_display.currindex==MAX_STORE_CB_MSG)||(i>cb_display.currindex))
				{
					currCBPageTotal-=DeleteCBMessage(i);
					if((currCBPageTotal+inCBPage)<=NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL)
						break;
				}
				else if(i<cb_display.currindex)
				{
					currCBPageTotal-=DeleteCBMessage(i);
					cb_display.currindex--;
					if((currCBPageTotal+inCBPage)<=NVRAM_SMS_CB_SMS_PAGE_NUM_TOTAL)
						break;
				}
				if(i==0) break;
			}

		}
	}

	AddCBMessage(inCBMI, inCBDCS, inCBPage, CbsMsg->msg_data, CbsMsg->msg_length);
	cb_display.unread[cbMsgIndex.currCBTotal-1]=TRUE;
	/* gs code is the first two most significant bits in the first octet */
	if((CbsMsg->sn & 0xc000) == 0)
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[CellBroadcast.c] Immediate mode-------*\n"));
		cb_display.immediate[cbMsgIndex.currCBTotal-1]=TRUE;
	}
	else
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[CellBroadcast.c] Normal mode-------*\n"));
		cb_display.immediate[cbMsgIndex.currCBTotal-1]=FALSE;
	}
	mmi_msg_new_cb_ind();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_new_cb_ind
* DESCRIPTION
*   New CB message indication
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_new_cb_ind (void)
{	
	TurnOnBacklight(1);

	if (IsMMIInIdleState () || GetMessagesCurrScrnID()==SCR_ID_MSG_CB_MESSAGE_IDLE || GetMessagesCurrScrnID()==SCR_ID_MSG_CB_MESSAGE_IND) 
	{
		ClearKeyEvents ();	// key flussing

		if(IsKeyPadLockState ())
			EntryIdleScreen (); 
		else
		{
			cbMsgIndex.currCBIndex=cbMsgIndex.currCBTotal-1;
			mmi_msg_entry_new_cb_ind();
		}
	}
	else if(GetMessagesCurrScrnID()==SCR_ID_MSG_CB_MESSAGE_LIST)
	{
		mmi_msg_entry_cb_read_message();
		DeleteScreenIfPresent(SCR_ID_MSG_CB_MESSAGE_LIST);
	}
	WriteCBMessageInfoToNvram ();//->no need!!! only write when shut down: check if ok

}

/*****************************************************************************
* FUNCTION
*  mmi_msg_delete_new_cb_message
* DESCRIPTION
*   Delete new CB message
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_delete_new_cb_message (void)
{
	DeleteCBMessage((S32)cbMsgIndex.currCBIndex);
	cb_display.unread[cbMsgIndex.currCBIndex]=FALSE;
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_from_new_cb_ind
* DESCRIPTION
*   Reset CB unread index before go back screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_go_back_from_new_cb_ind (void)
{
	cb_display.unread[cbMsgIndex.currCBIndex]=FALSE;
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_end_key_from_new_cb_ind
* DESCRIPTION
*   Reset CB unread index before go to idle
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_end_key_from_new_cb_ind (void)
{
	memset(&cb_display.unread, 0, MAX_STORE_CB_MSG);
	ExecCurrEndKeyDownHandler();	
}
#ifdef __MMI_MESSAGES_TWO_CB_MODES_DISPLAY__
/*****************************************************************************
* FUNCTION
*  mmi_msg_get_new_cb_ind_string
* DESCRIPTION
*   Get new msg indication string
*
* PARAMETERS
*  none
* RETURNS
*  new msg indication string
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* mmi_msg_get_new_cb_ind_string (void)
{
	U8 i=0;
	U8 number[(MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1)*ENCODING_LENGTH];
	static S8 temp[(MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1)*ENCODING_LENGTH];

	memset (number, 0, MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1);	
	for(i=0; i<cb_info.totalmi; i++)
	{
		if((cb_info.cbchnlentry[i].mi==cbMsgInfo.MsgMI[cbMsgIndex.currCBIndex])&&(strlen((S8*)cb_info.cbchnlentry[i].name)>0))
			break;
	}
	if(i<cb_info.totalmi)
	{
		if(cb_info.cbchnlentry[i].dcs==PHB_ASCII)
			AnsiiToUnicodeString(temp, (S8*)cb_info.cbchnlentry[i].name);
		else
			pfnUnicodeStrcpy(temp, (S8*)cb_info.cbchnlentry[i].name);
	}
	else
	{
		sprintf ((PS8)number, "%d", cbMsgInfo.MsgMI[cbMsgIndex.currCBIndex]);			
		AnsiiToUnicodeString(temp, (S8*)number);
	}
	return (U8*)temp;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_new_cb_immediate_ind
* DESCRIPTION
*   Entry new CB indication
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_new_cb_immediate_ind (void)
{
	U16 page = (U16)cbMsgInfo.MsgPage[cbMsgIndex.currCBIndex]*NVRAM_SMS_CB_SMS_PAGE_SIZE;
	EntryNewScreen(SCR_ID_MSG_CB_MESSAGE_IDLE, mmi_msg_exit_generic, NULL, NULL);
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] EntryScrCBMessageBodyWhenIdle ---*\n"));
	GetCurrEndKeyDownHandler();

	ReadCBMessageContent((PU32)&dispCBLength, cbMsgIndex.pCBMsgContent[cbMsgIndex.currCBIndex], 
		page, cbMsgInfo.MsgDCS[cbMsgIndex.currCBIndex]);			

	if(!IsKeyPadLockState())	// keypad is unlocked
	{
		SetMessagesCurrScrnID(SCR_ID_MSG_CB_MESSAGE_IDLE);
		GetCurrEndKeyDownHandler();
		ShowCategory74Screen(STR_CB_MSG_IND_INFORMATION_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
								STR_GLOBAL_DELETE, IMG_GLOBAL_OK,
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
								(PU8)dispCBMessage, (S32)dispCBLength, NULL );
		SetLeftSoftkeyFunction(mmi_msg_delete_new_cb_message, KEY_EVENT_UP);
		SetRightSoftkeyFunction(mmi_msg_go_back_from_new_cb_ind, KEY_EVENT_UP);
		SetKeyHandler(mmi_msg_end_key_from_new_cb_ind, KEY_END, KEY_EVENT_DOWN);
	}
	else		// keypad is locked
	{
		ShowCategory74Screen(STR_CB_MSG_IND_INFORMATION_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
										kplLSKStringtID, kplLSKIconID,
										kplRSKStringID, kplRSKIconID,
								(PU8)dispCBMessage, (S32)dispCBLength, NULL );
                /* prevent category scrolling */
	        SetKeyHandler(NULL, KEY_UP_ARROW, KEY_EVENT_DOWN);
	        SetKeyHandler(NULL, KEY_DOWN_ARROW, KEY_EVENT_DOWN);

	   	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_new_cb_normal_ind
* DESCRIPTION
*   Entry new CB indication
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_new_cb_normal_ind (void)
{
	EntryNewScreen(SCR_ID_MSG_CB_MESSAGE_IND, mmi_msg_exit_generic, NULL, NULL);
	SetMessagesCurrScrnID(SCR_ID_MSG_CB_MESSAGE_IND);
	GetCurrEndKeyDownHandler();

	if(!IsKeyPadLockState())	// keypad is unlocked
	{
		ShowCategory154Screen(0, 0,
										STR_SCR6035_LSK, IMG_SMS_COMMON_NOIMAGE,
										STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE,
										(PU8)GetString (STR_NEW_MESSAGE_FROM_ID), (PU8)mmi_msg_get_new_cb_ind_string(), 
										IMG_NEW_MESSAGE_NOTIFICATION_MSG, NULL);
		SetRightSoftkeyFunction(mmi_msg_go_back_from_new_cb_ind, KEY_EVENT_UP);
		SetLeftSoftkeyFunction(mmi_msg_entry_new_cb_immediate_ind, KEY_EVENT_UP);
		SetKeyHandler(mmi_msg_end_key_from_new_cb_ind, KEY_END, KEY_EVENT_DOWN);
	}
	else		// keypad is locked
	{
		ShowCategory154Screen(0, 0,
										kplLSKStringtID, kplLSKIconID,
										kplRSKStringID, kplRSKIconID,
										(PU8)GetString (STR_NEW_MESSAGE_FROM_ID), (PU8)mmi_msg_get_new_cb_ind_string(), 
										IMG_NEW_MESSAGE_NOTIFICATION_MSG, NULL);
	   	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	}
}


/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_new_cb_ind
* DESCRIPTION
*   Entry new CB indication
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_new_cb_ind (void)
{
	if(cb_display.immediate[cbMsgIndex.currCBIndex]==TRUE)
		mmi_msg_entry_new_cb_immediate_ind();
	else if(GetMessagesCurrScrnID()!=SCR_ID_MSG_CB_MESSAGE_IDLE)
		mmi_msg_entry_new_cb_normal_ind();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_new_cb_ind_when_kpl
* DESCRIPTION
*   Entry new CB indication when key pad lock
*
* PARAMETERS
*  LSKStringtID  	IN		LSK string ID
*  LSKIconID	 	IN       	LSK icon ID
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_new_cb_ind_when_kpl (U16 LSKStringtID, U16 LSKIconID, U16 RSKStringID, U16 RSKIconID)
{
	kplLSKStringtID=LSKStringtID;
	kplLSKIconID=LSKIconID;
	kplRSKStringID=RSKStringID; 
	kplRSKIconID=RSKIconID;
	mmi_msg_entry_new_cb_ind();
}
#else
/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_new_cb_ind_when_kpl
* DESCRIPTION
*   Entry new CB indication when key pad lock
*
* PARAMETERS
*  LSKStringtID  	IN		LSK string ID
*  LSKIconID	 	IN       	LSK icon ID
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_new_cb_ind_when_kpl (U16 LSKStringtID, U16 LSKIconID, U16 RSKStringID, U16 RSKIconID)
{
	U16 page = (U16)cbMsgInfo.MsgPage[cbMsgIndex.currCBIndex]*NVRAM_SMS_CB_SMS_PAGE_SIZE;
	ReadCBMessageContent((PU32)&dispCBLength, cbMsgIndex.pCBMsgContent[cbMsgIndex.currCBIndex], 
		page, cbMsgInfo.MsgDCS[cbMsgIndex.currCBIndex]);
	ShowCategory74Screen (  STR_CB_MSG_IND_INFORMATION_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
							LSKStringtID, LSKIconID,
							RSKStringID, RSKIconID, 
							(PU8)dispCBMessage, (S32)dispCBLength, NULL );
	/* prevent category scrolling */
	SetKeyHandler(NULL, KEY_UP_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(NULL, KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(NULL, KEY_VOL_UP, KEY_EVENT_DOWN);
	SetKeyHandler(NULL, KEY_VOL_DOWN, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_new_cb_ind
* DESCRIPTION
*   Entry new CB indication
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_new_cb_ind (void)
{
	U16 page = (U16)cbMsgInfo.MsgPage[cbMsgIndex.currCBIndex]*NVRAM_SMS_CB_SMS_PAGE_SIZE;
	EntryNewScreen(SCR_ID_MSG_CB_MESSAGE_IDLE, mmi_msg_exit_generic, NULL, NULL);
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] EntryScrCBMessageBodyWhenIdle ---*\n"));

	ReadCBMessageContent((PU32)&dispCBLength, cbMsgIndex.pCBMsgContent[cbMsgIndex.currCBIndex], 
		page, cbMsgInfo.MsgDCS[cbMsgIndex.currCBIndex]);			

	SetMessagesCurrScrnID(SCR_ID_MSG_CB_MESSAGE_IDLE);
	GetCurrEndKeyDownHandler();
	ShowCategory74Screen(STR_CB_MSG_IND_INFORMATION_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
								STR_GLOBAL_DELETE, IMG_GLOBAL_OK,
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
								(PU8)dispCBMessage, (S32)dispCBLength, NULL );
	SetLeftSoftkeyFunction(mmi_msg_delete_new_cb_message, KEY_EVENT_UP);
	SetRightSoftkeyFunction(mmi_msg_go_back_from_new_cb_ind, KEY_EVENT_UP);
	SetKeyHandler(mmi_msg_end_key_from_new_cb_ind, KEY_END, KEY_EVENT_DOWN);
}
#endif
/*****************************************************************************
* FUNCTION
*  mmi_msg_need_new_cb_ind
* DESCRIPTION
*   Check if new CB indication is needed
*
* PARAMETERS
*  none
* RETURNS
*  TRUE: indication needed, FALSE: indication not needed
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_msg_need_new_cb_ind (void)
{
	U8 i=0;
	if(cbMsgIndex.currCBTotal==0)
		return FALSE;
	for(i=(cbMsgIndex.currCBTotal-1); ; i--)
	{
		if(cb_display.unread[i]==TRUE)
		{
			cbMsgIndex.currCBIndex=i;
			return TRUE;
		}
		if(i==0) break;
	}
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_restore_cb
* DESCRIPTION
*   Restore CB settings
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_restore_cb (void)
{
	cb_state=SMS_CB_DEFAULT_SETTINGS_MODE_STATE;
	SetProtocolEventHandler(mmi_msg_subscribe_cb_rsp, PRT_MSG_ID_MMI_CB_SUBSCRIBE_RSP);
	mmi_msg_subscribe_cb_req(FALSE);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_subscribe_cb
* DESCRIPTION
*   Subscribe CB
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_subscribe_cb (void)
{
	U8 mode=!cb_info.mode;
//#ifndef MMI_ON_HARDWARE_P // wangbei del 20060619
	cb_info.mode=mode;
	if(cb_info.mode==TRUE)
		pfnUnicodeStrcpy((S8*)cb_display.mode, (S8*)GetString((U16)STR_GLOBAL_ON));
	else
		pfnUnicodeStrcpy((S8*)cb_display.mode, (S8*)GetString((U16)STR_GLOBAL_OFF));
	mmi_msg_refresh_cb_main_menu();
// wangbei del 20060619
//#else
//	ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
//	SetProtocolEventHandler(mmi_msg_subscribe_cb_rsp, PRT_MSG_ID_MMI_CB_SUBSCRIBE_RSP);
//	mmi_msg_subscribe_cb_req(mode);
//#endif
// wangbei del end
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_save_cb_channel
* DESCRIPTION
*   Save CB channel info
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_save_cb_channel (void)
{
	U32 mi=0;
	U8 number[(MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1)*ENCODING_LENGTH]={0,0};
	U16 length;
	U8 i=0;

	if((pfnUnicodeStrlen((S8*)cb_display.mi))==0)
	{
		DisplayPopup ((PU8)GetString (STR_SMS_NUM_CANNOT_BE_EMPTY), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
	memset(&number, 0, (MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1)*ENCODING_LENGTH);
	length = UnicodeNToAnsii((S8*)number, (S8*)cb_display.mi, (pfnUnicodeStrlen((S8*)cb_display.mi))*ENCODING_LENGTH);
	number[length] = '\0';
	mi = atoi((S8*)number);
	if(mi>MAX_CB_CHNL_NUMBER)
	{
		DisplayPopup ((PU8)GetString (STR_CB_CHNL_NUMBER_LEN_EXCEEDED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}	
	for(i=0; i<cb_info.totalmi; i++)
	{
		if((cb_state==SMS_CB_CHNL_EDIT_STATE)&&(i==g_msg_cntx.currHiliteIndex))
			continue;
		if(cb_info.cbchnlentry[i].mi==(U16)mi)
		{
			DisplayPopup ((PU8)GetString (STR_CB_CHNL_DUPLICATE_NUMBER_NOT_ALLOWED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			return;
		}
	}
	if(cb_state==SMS_CB_CHNL_ADD_STATE)
		mmi_msg_add_cb_channel((U16)mi);
	else
		mmi_msg_edit_cb_channel((U16)mi);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_save_cb_channel
* DESCRIPTION
*   Save CB channel info
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_save_cb_channel (void)
{
	mmi_msg_set_confirm_screen(STR_GLOBAL_SAVE, mmi_msg_save_cb_channel, Messages2GoBackHistory);
	mmi_msg_entry_confirm_generic();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_select_cb_channel
* DESCRIPTION
*   Select CB channel
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_select_cb_channel (void)
{
	U8 i=0;
	for(i=0; i<cb_info.totalmi; i++)
		cb_info.cbchnlentry[i].mask=cb_display.mask[i+1];
	ClearAllKeyHandler();
	SetProtocolEventHandler(mmi_msg_set_cb_mode_rsp, PRT_MSG_ID_MMI_SMS_SET_CB_MODE_RSP);
	mmi_msg_set_cb_mode_req(SMSAL_CB_MI_MASK);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_add_cb_channel
* DESCRIPTION
*   Add CB channel
*
* PARAMETERS
*  mi  IN		mi to be added
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_add_cb_channel (U16 mi)
{
	cb_info.cbchnlentry[cb_info.totalmi].index=cb_display.addindex;
	cb_info.cbchnlentry[cb_info.totalmi].mask=1;
	cb_info.cbchnlentry[cb_info.totalmi].mi=mi;
	memset(cb_info.cbchnlentry[cb_info.totalmi].name, 0, ENCODING_LENGTH);
	if(GetUCS2Flag ((PS8)cb_display.name))
		cb_display.dcs= PHB_UCS2 ;
	else
		cb_display.dcs = PHB_ASCII ;
	if(cb_display.dcs==PHB_ASCII)
		UnicodeToAnsii((S8*)cb_info.cbchnlentry[cb_info.totalmi].name, (S8*)cb_display.name);
	else
		pfnUnicodeStrcpy((S8*)cb_info.cbchnlentry[cb_info.totalmi].name, (S8*)cb_display.name);
	cb_info.cbchnlentry[cb_info.totalmi].dcs=cb_display.dcs;
	/*delete if apply new interface*/
	memset(cbNvramStruct.chnlNames[cb_info.totalmi].name, 0, MAX_CB_CHNL_NAME_LENGTH+1);
	memcpy(cbNvramStruct.chnlNames[cb_info.totalmi].name, (S8*)cb_info.cbchnlentry[cb_info.totalmi].name, MAX_CB_CHNL_NAME_LENGTH);
	cbNvramStruct.chnlNames[cb_info.totalmi].name_dcs=cb_display.dcs;
	cbNvramStruct.l4MidIndex[cb_info.totalmi]=cb_display.addindex;
	WriteCbChnlInfoToNvram();
	/*end*/
	cb_info.totalmi++;
	ClearAllKeyHandler();
	SetProtocolEventHandler(mmi_msg_set_cb_mode_rsp, PRT_MSG_ID_MMI_SMS_SET_CB_MODE_RSP);
	mmi_msg_set_cb_mode_req(SMSAL_CB_MI | SMSAL_CB_MI_MASK);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_edit_cb_channel
* DESCRIPTION
*   Edit CB channel
*
* PARAMETERS
*  mi  IN		mi to be modified
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_edit_cb_channel (U16 mi)
{
	cb_info.cbchnlentry[g_msg_cntx.currHiliteIndex].mi=mi;
	memset(cb_info.cbchnlentry[g_msg_cntx.currHiliteIndex].name, 0, MAX_CB_CHNL_NAME_LENGTH+1);
	if(GetUCS2Flag ((PS8)cb_display.name))
		cb_display.dcs= PHB_UCS2 ;
	else
		cb_display.dcs = PHB_ASCII ;
	if(cb_display.dcs==PHB_ASCII)
		UnicodeNToAnsii((S8*)cb_info.cbchnlentry[g_msg_cntx.currHiliteIndex].name, (S8*)cb_display.name, MAX_CB_CHNL_NAME_LENGTH*ENCODING_LENGTH);
	else
		pfnUnicodeStrcpy((S8*)cb_info.cbchnlentry[g_msg_cntx.currHiliteIndex].name, (S8*)cb_display.name);
	cb_info.cbchnlentry[g_msg_cntx.currHiliteIndex].dcs=cb_display.dcs;
	/*delete if apply new interface*/
	memset(cbNvramStruct.chnlNames[g_msg_cntx.currHiliteIndex].name, 0, MAX_CB_CHNL_NAME_LENGTH+1);
	memcpy(cbNvramStruct.chnlNames[g_msg_cntx.currHiliteIndex].name, (S8*)cb_info.cbchnlentry[g_msg_cntx.currHiliteIndex].name, MAX_CB_CHNL_NAME_LENGTH);
	cbNvramStruct.chnlNames[g_msg_cntx.currHiliteIndex].name_dcs=cb_display.dcs;
	WriteCbChnlInfoToNvram();
	/*end*/
	ClearAllKeyHandler();
	SetProtocolEventHandler(mmi_msg_set_cb_mode_rsp, PRT_MSG_ID_MMI_SMS_SET_CB_MODE_RSP);
	mmi_msg_set_cb_mode_req(SMSAL_CB_MI);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_delete_cb_channel
* DESCRIPTION
*   Delete CB channel
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_delete_cb_channel (void)
{
	U8 i=0;
	for(i=(g_msg_cntx.currHiliteIndex+1); i<cb_info.totalmi; i++)
		memcpy(&cb_info.cbchnlentry[i-1], &cb_info.cbchnlentry[i], sizeof(msg_cb_channel_struct));
	cb_info.totalmi--;
	cb_info.cbchnlentry[cb_info.totalmi].mi=SMSAL_CB_UNUSED_CBMI;
	cb_info.cbchnlentry[cb_info.totalmi].mask=0;
	memset(cb_info.cbchnlentry[cb_info.totalmi].name, 0, MAX_CB_CHNL_NAME_LENGTH);
	memset(&cbNvramStruct, 0xff, sizeof(msg_cb_channel_nvram_struct));
	for(i=0; i<cb_info.totalmi; i++)
	{
		cbNvramStruct.l4MidIndex[i]=cb_info.cbchnlentry[i].index;
		cbNvramStruct.chnlNames[i].name_dcs=cb_info.cbchnlentry[i].dcs;
		memcpy((S8*)cbNvramStruct.chnlNames[i].name, (S8*)cb_info.cbchnlentry[i].name, MAX_CB_CHNL_NAME_LENGTH+1);
	}
	WriteCbChnlInfoToNvram();
	ClearAllKeyHandler();
	SetProtocolEventHandler(mmi_msg_set_cb_mode_rsp, PRT_MSG_ID_MMI_SMS_SET_CB_MODE_RSP);
	mmi_msg_set_cb_mode_req(SMSAL_CB_MI|SMSAL_CB_MI_MASK);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_select_cb_language
* DESCRIPTION
*   Select CB language
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_select_cb_language (void)
{
	U8 i=0;
	for(i=0; i<cb_info.totaldcs; i++)
		cb_info.cblanentry[i].mask=cb_display.mask[i+1];
	cb_info.dcsall=cb_display.mask[0];
	ClearAllKeyHandler();
	SetProtocolEventHandler(mmi_msg_set_cb_mode_rsp, PRT_MSG_ID_MMI_SMS_SET_CB_MODE_RSP);
	mmi_msg_set_cb_mode_req(SMSAL_CB_DCS_MASK|SMSAL_CB_ALL_LANG);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_refresh_cb_main_menu
* DESCRIPTION
*   Refresh CB main menu
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_refresh_cb_main_menu (void)
{
	S32 index=GetChildMenuIDIndexByParentMenuID (MESSAGES_MENU_CB_MENUID, MITEM102_CB_SERVICE);;
	if(index!=-1)
		Category52ChangeItemDescription(index, cb_display.mode);
	if(cb_info.mode==TRUE)
		ChangeLeftSoftkey(STR_GLOBAL_OFF, IMG_GLOBAL_OK);
	else
		ChangeLeftSoftkey(STR_GLOBAL_ON, IMG_GLOBAL_OK);
	SetLeftSoftkeyFunction(mmi_msg_subscribe_cb, KEY_EVENT_UP);
	RedrawCategoryFunction();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_cb_main_menu
* DESCRIPTION
*   Entry CB main menu
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_cb_main_menu (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];
	U8*	popUpList[MAX_SUB_MENUS];
	
	EntryNewScreen(SCR_ID_MSG_CB_MAIN_MENU, mmi_msg_exit_generic, mmi_msg_entry_cb_main_menu, NULL);	
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[SmsMoMtGuiInterface.c] mmi_msg_entry_cb_main_menu -------*\n"));
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_CB_MAIN_MENU);
	
	numItems = GetNumOfChild (MESSAGES_MENU_CB_MENUID);	
	GetSequenceStringIds(MESSAGES_MENU_CB_MENUID, nStrItemList);	
	SetParentHandler (MESSAGES_MENU_CB_MENUID);
	ConstructHintsList(MESSAGES_MENU_CB_MENUID, popUpList);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_CB_MAIN_MENU);
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_CB_MAIN_MENU);
		
	ShowCategory52Screen(STR_CB_MENUENTRY,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, popUpList,
						  			0, 0,	guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_cb_main_menu
* DESCRIPTION
*   Get CB mode request before entry CB main menu
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_entry_cb_main_menu (void)
{
#ifdef MMI_ON_HARDWARE_P
	if(mmi_frm_sms_get_sms_list_size(MMI_FRM_SMS_NOBOX)==MMI_FRM_SMS_INVALID_INDEX)
	{
		DisplayPopup ((PU8)GetString(STR_CB_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
    	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	else if(mmi_frm_sms_check_action_pending())
	{
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
#endif
	cb_state = SMS_CB_SERVICE_MODE_STATE;
	SetProtocolEventHandler(mmi_msg_get_cb_mode_rsp_active, PRT_MSG_ID_MMI_SMS_GET_CB_MODE_RSP);
	mmi_msg_get_cb_mode_req();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_cb_channel_settings
* DESCRIPTION
*   Entry CB channel settings screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_cb_channel_settings (void)
{
	U8*	guiBuffer=NULL;
	U16	numItems=0;
	U16	nStrItemList[MAX_SUB_MENUS];
	
	EntryNewScreen(SCR_ID_MSG_CB_CHANNEL_SETTINGS, mmi_msg_exit_generic, mmi_msg_entry_cb_channel_settings, NULL);	
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[SmsMoMtGuiInterface.c] mmi_msg_entry_option_other -------*\n"));
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_CB_CHANNEL_SETTINGS);
	
	numItems = GetNumOfChild (MITEM103_CB_CHANNEL_ENBL_OPTIONS);	
	GetSequenceStringIds(MITEM103_CB_CHANNEL_ENBL_OPTIONS, nStrItemList);	
	SetParentHandler (MITEM103_CB_CHANNEL_ENBL_OPTIONS);
	
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	SetMessagesCurrScrnID(SCR_ID_MSG_CB_CHANNEL_SETTINGS);
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_CB_CHANNEL_SETTINGS);
		
	ShowCategory52Screen(STR102_SCR103_CAPTION,	IMG_SMS_ENTRY_SCRN_CAPTION, 
						  			STR_GLOBAL_OK,		IMG_GLOBAL_OK,
						  			STR_GLOBAL_BACK,	IMG_GLOBAL_BACK,
						  			numItems, nStrItemList, (U16 *)gIndexIconsImageList, NULL,
						  			0, 0,	guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_cb_channel_bootup
* DESCRIPTION
*   Get CB mode request in bootup procedure. 
*   This is for filling up cb_info to display channel name when getting a CB message.
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_entry_cb_channel_bootup (void)
{
	cb_state = SMS_CB_BOOT_UP_STATE;
	SetProtocolEventHandler(mmi_msg_get_cb_mode_rsp_display, PRT_MSG_ID_MMI_SMS_GET_CB_MODE_RSP);
	mmi_msg_get_cb_mode_req();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_cb_channel_select
* DESCRIPTION
*   Entry CB channel select screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_cb_channel_select (void)
{

	U8*	guiBuffer;
	U16	numItems;
	U8 	i=0;
	U8		selectall=TRUE;
	U8		number[MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1];

	EntryNewScreen(SCR_ID_MSG_CB_CHANNEL_LIST, mmi_msg_exit_generic, mmi_msg_entry_cb_channel_select, NULL);
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[CellBroadcast.c] mmi_msg_entry_cb_channel_select -------*\n"));

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_CB_CHANNEL_LIST);

	pfnUnicodeStrncpy((S8*)subMenuDataPtrs[0], (S8*)GetString((U16)STR_CB_ALL_CHANNELS_FOR_SELECT), MAX_CB_CHNL_NAME_LENGTH*ENCODING_LENGTH);
	for(i=0; i<cb_info.totalmi; i++)
	{
		if(strlen((S8*)cb_info.cbchnlentry[i].name)==0)
		{
			memset (number, 0, sizeof(number));
			sprintf ((PS8)number, "%d", cb_info.cbchnlentry[i].mi);
			AnsiiToUnicodeString((S8*)subMenuDataPtrs[i+1], (S8*)number);
		}
		else
		{
			if(cb_info.cbchnlentry[i].dcs==PHB_ASCII)
				AnsiiToUnicodeString((S8*)subMenuDataPtrs[i+1], (S8*)cb_info.cbchnlentry[i].name);
			else
				pfnUnicodeStrcpy((S8*)subMenuDataPtrs[i+1], (S8*)cb_info.cbchnlentry[i].name);
		}
		cb_display.mask[i+1]=cb_info.cbchnlentry[i].mask;
		if(cb_display.mask[i+1]==0)
			selectall=FALSE;
	}

	cb_display.mask[0]=selectall;
	numItems=cb_info.totalmi+1;
	
	SetMessagesCurrScrnID(SCR_ID_MSG_CB_CHANNEL_LIST);
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_CB_CHANNEL_LIST);
	
	mmi_msg_set_confirm_screen(STR_GLOBAL_SAVE, mmi_msg_select_cb_channel, Messages2GoBackHistory);

	SetCheckboxToggleRightSoftkeyFunctions (mmi_msg_entry_confirm_generic, GoBackHistory);	
	ShowCategory140Screen (STR_CB_SELECT_CHNL, IMG_SMS_ENTRY_SCRN_CAPTION,
										numItems,  (U8**)subMenuDataPtrs, (U8*)cb_display.mask, 0, guiBuffer);
	SetLeftSoftkeyFunction (special_check_list_handle_left_softkey_up, KEY_EVENT_UP);				
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_cb_channel_select
* DESCRIPTION
*   Get CB mode request before enter CB channel select
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_entry_cb_channel_select (void)
{
	cb_state = SMS_CB_CHNL_SELECT_STATE;
	SetProtocolEventHandler(mmi_msg_get_cb_mode_rsp_display, PRT_MSG_ID_MMI_SMS_GET_CB_MODE_RSP);
	mmi_msg_get_cb_mode_req();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_cb_channel_list
* DESCRIPTION
*   Entry CB channel list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_cb_channel_list (void)
{
	U8*	guiBuffer=NULL;
	S32	hiliteitem=0;	

		EntryNewScreen(SCR_ID_MSG_CB_CHANNEL_LIST, mmi_msg_exit_generic, mmi_msg_entry_cb_channel_list, NULL);		
	PRINT_INFORMATION_2((MMI_TRACE_G6_SMS, "*-------[CellBroadcast.c] mmi_msg_entry_cb_channel_list -------*\n"));

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_CB_CHANNEL_LIST);		 
	if(guiBuffer!=NULL)
		hiliteitem=(g_msg_cntx.currHiliteIndex<cb_info.totalmi)?g_msg_cntx.currHiliteIndex:0;
	RegisterHighlightHandler (GetHiliteIndex);	
	
	SetMessagesCurrScrnID(SCR_ID_MSG_CB_CHANNEL_LIST);		
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_CB_CHANNEL_LIST);
	ShowCategory184Screen(STR_CB_CHNL_LIST, IMG_SMS_ENTRY_SCRN_CAPTION, 
							STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							cb_info.totalmi,
							mmi_msg_cb_channel_list_get_item, 
							mmi_msg_cb_channel_list_get_hint, 
							hiliteitem, 
							guiBuffer);
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	if(cb_state==SMS_CB_CHNL_EDIT_STATE)
		SetLeftSoftkeyFunction (mmi_msg_pre_entry_cb_channel_edit, KEY_EVENT_UP);
	else
	{
		mmi_msg_set_confirm_screen(STR_GLOBAL_DELETE, mmi_msg_delete_cb_channel, GoBackHistory);
		SetLeftSoftkeyFunction (mmi_msg_entry_confirm_generic, KEY_EVENT_UP);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_cb_channel_list_get_item
* DESCRIPTION
*   Get CB channel list item
*
* PARAMETERS
*  a  IN		item index
*  b  IN/OUT	string buffer
*  c  IN/OUT	image buffer
*  d  IN/OUT	string image mask
* RETURNS
*  TRUE
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL mmi_msg_cb_channel_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	U8 number[MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1];
	
	if((item_index < 0) || (item_index >= cb_info.totalmi))
		return FALSE;

	if (cb_info.cbchnlentry[item_index].dcs==PHB_ASCII)
	{
		if (strlen((S8*)cb_info.cbchnlentry[item_index].name)==0)
		{
			memset (number, 0, MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1);
			sprintf ((PS8)number, "%d", cb_info.cbchnlentry[item_index].mi);
			AnsiiToUnicodeString((S8*)str_buff, (S8*)number);
		}
		else
		{
			AnsiiToUnicodeString((S8*)str_buff, (S8*)cb_info.cbchnlentry[item_index].name);
		}
	}
	else
	{
		if (pfnUnicodeStrlen((S8*)cb_info.cbchnlentry[item_index].name)==0)
		{
			memset (number, 0, MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1);
			sprintf ((PS8)number, "%d", cb_info.cbchnlentry[item_index].mi);
			AnsiiToUnicodeString((S8*)str_buff, (S8*)number);
		}
		else
		{
			pfnUnicodeStrcpy((S8*)str_buff, (S8*)cb_info.cbchnlentry[item_index].name);
		}
	}
	*img_buff_p = get_image(IMG_CB_CHNL_TITLE);
	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_cb_channel_list_get_hint
* DESCRIPTION
*   Get CB channel list hint
*
* PARAMETERS
*  a  IN		item index
*  b  IN/OUT	hint array
* RETURNS
*  TRUE: display hint, FALSE: not display hint
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 mmi_msg_cb_channel_list_get_hint ( S32 item_index, UI_string_type *hint_array )
{
	U8 number[MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1];
	if(strlen((S8*)cb_info.cbchnlentry[item_index].name)==0)
		return 0;
	
	memset (number, 0, MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1);
	sprintf ((PS8)number, "%d", cb_info.cbchnlentry[item_index].mi);				
	AnsiiNToUnicodeString((S8*)hint_array[0], (S8*)number, MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1);
	return 1;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_cb_channel_add
* DESCRIPTION
*   Before add CB channel, check if unused CBMI available
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_entry_cb_channel_add (void)
{
	cb_state = SMS_CB_CHNL_ADD_STATE;
	SetProtocolEventHandler(mmi_msg_get_cb_mode_rsp_display, PRT_MSG_ID_MMI_SMS_GET_CB_MODE_RSP);
	mmi_msg_get_cb_mode_req();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_cb_channel_list_edit
* DESCRIPTION
*   Before enter CB channel edit, get chennel list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_entry_cb_channel_list_edit (void)
{
	cb_state = SMS_CB_CHNL_EDIT_STATE;
	SetProtocolEventHandler(mmi_msg_get_cb_mode_rsp_display, PRT_MSG_ID_MMI_SMS_GET_CB_MODE_RSP);
	mmi_msg_get_cb_mode_req();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_cb_channel_list_delete
* DESCRIPTION
*   Before enter CB channel delete, get chennel list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_entry_cb_channel_list_delete (void)
{
	cb_state = SMS_CB_CHNL_DELETE_STATE;
	SetProtocolEventHandler(mmi_msg_get_cb_mode_rsp_display, PRT_MSG_ID_MMI_SMS_GET_CB_MODE_RSP);
	mmi_msg_get_cb_mode_req();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_set_cb_channel_inline_item
* DESCRIPTION
*   Set CB channel inline item for channel edit screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_set_cb_channel_inline_item (void)
{
	SetInlineItemActivation(&wgui_inline_items[0], KEY_LSK, KEY_EVENT_UP);
	SetInlineItemActivation(&wgui_inline_items[1], KEY_LSK, KEY_EVENT_UP);
	SetInlineItemActivation(&wgui_inline_items[2], KEY_LSK, KEY_EVENT_UP);
	SetInlineItemActivation(&wgui_inline_items[3], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[0], (U8*) GetString (STR_CB_CHNL_TITLE));
	SetInlineItemFullScreenEdit (&wgui_inline_items[1], STR_CB_CHNL_TITLE_FULL_EDIT, IMG_SMS_ENTRY_SCRN_CAPTION, 
								(U8*)cb_display.name, MAX_CB_CHNL_NAME_LENGTH+1, 
								INPUT_TYPE_USE_ENCODING_BASED_LENGTH | INPUT_TYPE_ONE_LESS_CHARACTER | INPUT_TYPE_ALPHANUMERIC_SENTENCECASE);
	SetInlineFullScreenEditCustomFunction(&wgui_inline_items[1], mmi_msg_inline_generic_custom_function);
	SetInlineItemCaption(&wgui_inline_items[2], (U8*)GetString (STR_CB_CHNL_NUMBER));
	SetInlineItemTextEdit(&wgui_inline_items[3], (U8*)cb_display.mi, MAX_CB_CHNL_NUMBER_LENGTH+1, INPUT_TYPE_NUMERIC);
	DisableInlineItemHighlight(&wgui_inline_items[3]);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_cb_channel_edit
* DESCRIPTION
*   Entry CB channel edit screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_cb_channel_edit (void)
{
	PU8 guiBuffer=NULL;     
	PU8 inputBuffer=NULL;
	U16 inputBufferSize=0;
	EntryNewScreen(SCR_ID_MSG_CB_CHANNEL_EDIT, mmi_msg_exit_cb_channel_edit, NULL, NULL);
	
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] EntryScrCBChannelEditor ---*\n"));

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_CB_CHANNEL_EDIT);
	inputBuffer = GetCurrNInputBuffer ( SCR_ID_MSG_CB_CHANNEL_EDIT, &inputBufferSize );
	InitializeCategory57Screen();
	mmi_msg_set_cb_channel_inline_item();
	if(inputBuffer!=NULL)
		SetCategory57Data(wgui_inline_items, MAX_CB_CHNL_INLINE_ITEM, inputBuffer);

	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_CB_CHANNEL_EDIT);
	SetMessagesCurrScrnID(SCR_ID_MSG_CB_CHANNEL_EDIT);

	ShowCategory57Screen ( STR_CB_CHNL_EDITOR, IMG_SMS_ENTRY_SCRN_CAPTION,						
						  			STR_GLOBAL_EDIT, IMG_GLOBAL_OK,
						   		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						   		MAX_CB_CHNL_INLINE_ITEM, cbChnlEditorIcons, 
						   		wgui_inline_items, 0, guiBuffer);

	SetCategory57RightSoftkeyFunctions (mmi_msg_pre_save_cb_channel, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_exit_cb_channel_edit
* DESCRIPTION
*   Exit CB channel inline edit screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_exit_cb_channel_edit (void)
{
	history_t currHistory; 
	U16 inputBufferSize=0;
	S16 nHistory = 0;
	g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=SCR_ID_MSG_CB_CHANNEL_EDIT;
	SetMessagesCurrScrnID(0);
	
	currHistory.scrnID = SCR_ID_MSG_CB_CHANNEL_EDIT;
	CloseCategory57Screen();
	currHistory.entryFuncPtr = mmi_msg_entry_cb_channel_edit;
	pfnUnicodeStrcpy((S8*)currHistory.inputBuffer,(S8*)&nHistory);
	GetCategory57History (currHistory.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();
	GetCategory57Data ((U8*)currHistory.inputBuffer);
	AddNHistory(currHistory, inputBufferSize);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_cb_channel_edit
* DESCRIPTION
*   Before edit CB channel, set the item for edit
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_entry_cb_channel_edit (void)
{
	U8 number[(MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1)*ENCODING_LENGTH];
	cb_display.dcs=cb_info.cbchnlentry[g_msg_cntx.currHiliteIndex].dcs;
	if(cb_info.cbchnlentry[g_msg_cntx.currHiliteIndex].dcs==PHB_ASCII)
		AnsiiToUnicodeString((S8*)cb_display.name, (S8*)cb_info.cbchnlentry[g_msg_cntx.currHiliteIndex].name);
	else
		pfnUnicodeStrcpy((S8*)cb_display.name, (S8*)cb_info.cbchnlentry[g_msg_cntx.currHiliteIndex].name);

	memset (number, 0, sizeof(number));
	sprintf ((S8*)number, "%d", cb_info.cbchnlentry[g_msg_cntx.currHiliteIndex].mi);				
	memset(cb_display.mi, 0, sizeof(cb_display.mi));
	AnsiiNToUnicodeString((S8*)cb_display.mi, (S8*)number, strlen((S8*)number));
	mmi_msg_entry_cb_channel_edit();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_cb_language
* DESCRIPTION
*   Entry CB language list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_cb_language (void)
{
	U8*	guiBuffer;
	U16	numItems;
	U8 	i=0;

	EntryNewScreen(SCR_ID_MSG_CB_LANGUAGE, mmi_msg_exit_generic, mmi_msg_entry_cb_language, NULL);
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[CellBroadcast.c] mmi_msg_entry_cb_channel_select -------*\n"));

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_CB_LANGUAGE);

	pfnUnicodeStrncpy((S8*)subMenuDataPtrs[0], (S8*)GetString((U16)STR_CB_ALL_LANGUAGES_FOR_SELECT), MAX_CB_CHNL_NAME_LENGTH*ENCODING_LENGTH);
	for(i=0; i<cb_info.totaldcs; i++)
	{
		pfnUnicodeStrcpy((S8*)subMenuDataPtrs[i+1], (PS8)GetString((U16)LangEnumAndStrID[cb_info.cblanentry[i].dcs][1]));
		cb_display.mask[i+1]=cb_info.cblanentry[i].mask;
	}
	cb_display.mask[0]=cb_info.dcsall;
	numItems=cb_info.totaldcs+1;
	
	SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_CB_LANGUAGE);
	SetMessagesCurrScrnID(SCR_ID_MSG_CB_LANGUAGE);
	
	//mmi_msg_set_confirm_screen(STR_GLOBAL_SAVE, mmi_msg_select_cb_language, Messages2GoBackHistory);// wangbei del 20060619
	mmi_msg_set_confirm_screen(STR_GLOBAL_SAVE, NULL, Messages2GoBackHistory);// wangbei add 20060619

	SetCheckboxToggleRightSoftkeyFunctions (mmi_msg_entry_confirm_generic, GoBackHistory);	
	ShowCategory140Screen (STR103_SCR101_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
								  numItems,  (U8**)subMenuDataPtrs, (U8*)cb_display.mask, 0, guiBuffer);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_cb_language
* DESCRIPTION
*   Before enter CB language screen, get language list
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_entry_cb_language (void)
{
	cb_state = SMS_CB_LANGUAGE_STATE;
	SetProtocolEventHandler(mmi_msg_get_cb_mode_rsp_language, PRT_MSG_ID_MMI_SMS_GET_CB_MODE_RSP);
	mmi_msg_get_cb_mode_req();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_cb_read_message
* DESCRIPTION
*   Entry CB message list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_cb_read_message (void)
{
	U8*	guiBuffer=NULL;
	S32	hiliteitem=0;	

	EntryNewScreen(SCR_ID_MSG_CB_MESSAGE_LIST, mmi_msg_exit_generic, mmi_msg_pre_entry_cb_read_message, NULL);		
	PRINT_INFORMATION_2((MMI_TRACE_G6_SMS, "*-------[CellBroadcast.c] mmi_msg_entry_cb_channel_list -------*\n"));

	memset(&cb_display.unread, 0, MAX_STORE_CB_MSG);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_CB_MESSAGE_LIST);		 
	if((guiBuffer!=NULL)||(g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes==SCR_ID_MSG_CB_MESSAGE_LIST)
		||(g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes==SCR_ID_MSG_CB_MESSAGE))
		hiliteitem=(g_msg_cntx.currHiliteIndex<cbMsgIndex.currCBTotal)?g_msg_cntx.currHiliteIndex:0;
	RegisterHighlightHandler (GetHiliteIndex);	
	
	SetMessagesCurrScrnID(SCR_ID_MSG_CB_MESSAGE_LIST);		
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_CB_MESSAGE_LIST);
	ShowCategory184Screen(STR101_MITEM102_CB_READ_MESSAGE, IMG_SMS_ENTRY_SCRN_CAPTION, 
							STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							cbMsgIndex.currCBTotal,
							mmi_msg_cb_read_message_get_item, 
							NULL, 
							hiliteitem, 
							guiBuffer);
	
	SetLeftSoftkeyFunction (mmi_msg_entry_cb_message, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_entry_cb_message, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_pre_entry_cb_read_message
* DESCRIPTION
*   Before entering CB message list screen, get channel info for display
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_pre_entry_cb_read_message (void)
{
	if(cbMsgIndex.currCBTotal==0)
		DisplayPopup ((PU8)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
	else
	{
		cb_state=SMS_CB_READ_MESSAGE_STATE;
		SetProtocolEventHandler(mmi_msg_get_cb_mode_rsp_display, PRT_MSG_ID_MMI_SMS_GET_CB_MODE_RSP);
		mmi_msg_get_cb_mode_req();
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_cb_read_message_get_item
* DESCRIPTION
*   Get CB message list item
*
* PARAMETERS
*  a  IN		item index
*  b  IN/OUT	string buffer
*  c  IN/OUT	image buffer
*  d  IN/OUT	string image mask
* RETURNS
*  TRUE
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL mmi_msg_cb_read_message_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	U8 number[(MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1)*ENCODING_LENGTH];
	U8 i=0;
	
	if((item_index < 0) || (item_index >= cbMsgIndex.currCBTotal))
		return FALSE;

	for(i=0; i<cb_info.totalmi; i++)
	{
		if((cb_info.cbchnlentry[i].mi==cbMsgInfo.MsgMI[item_index])&&(strlen((S8*)cb_info.cbchnlentry[i].name)>0))
			break;
	}
	if(i<cb_info.totalmi)
	{
		if(cb_info.cbchnlentry[i].dcs==PHB_ASCII)
			AnsiiToUnicodeString((S8*)str_buff, (S8*)cb_info.cbchnlentry[i].name);
		else
			pfnUnicodeStrcpy((S8*)str_buff, (S8*)cb_info.cbchnlentry[i].name);
	}
	else
	{
		memset (number, 0, MAX_CB_CHNL_NUMBER_DISPLAY_LENGTH+1);	
		sprintf ((PS8)number, "%d", cbMsgInfo.MsgMI[item_index]);			
		AnsiiToUnicodeString((S8*)str_buff, (S8*)number);
	}
	
	*img_buff_p = get_image(gIndexIconsImageList[item_index]);
	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_go_back_from_cb_message
* DESCRIPTION
*   Reset current display index before go back screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_go_back_from_cb_message (void)
{
	cb_display.currindex=MAX_STORE_CB_MSG;
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_end_key_from_cb_message
* DESCRIPTION
*   Reset current display index before go to idle
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_end_key_from_cb_message (void)
{
	cb_display.currindex=MAX_STORE_CB_MSG;
	ExecCurrEndKeyDownHandler();
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_entry_cb_message
* DESCRIPTION
*   Entry CB message screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_entry_cb_message (void)
{
	U8*	guiBuffer=NULL;
	U16 page;

	EntryNewScreen(SCR_ID_MSG_CB_MESSAGE, mmi_msg_exit_generic, mmi_msg_entry_cb_message, NULL);
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] EntryScrCBMessageBody ---*\n"));
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_CB_MESSAGE);		 
	if(guiBuffer==NULL)
		cb_display.currindex=(U8)g_msg_cntx.currHiliteIndex;
	
	SetMessagesCurrScrnID(SCR_ID_MSG_CB_MESSAGE);
	SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_CB_MESSAGE);
	
	page = (U16)cbMsgInfo.MsgPage[cb_display.currindex]*NVRAM_SMS_CB_SMS_PAGE_SIZE;
	ReadCBMessageContent((PU32)&dispCBLength, cbMsgIndex.pCBMsgContent[cb_display.currindex], 
		page, cbMsgInfo.MsgDCS[cb_display.currindex]);	

	mmi_msg_set_confirm_screen(STR_GLOBAL_DELETE, mmi_msg_delete_cb_message, GoBackHistory);
	GetCurrEndKeyDownHandler();
	ShowCategory74Screen (STR_CB_MESSAGE_BODY, IMG_SMS_ENTRY_SCRN_CAPTION,
									STR_GLOBAL_DELETE, IMG_SMS_COMMON_NOIMAGE,
									STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE, 
									(PU8) dispCBMessage,
									(S32)(cbMsgInfo.MsgPage[cb_display.currindex]*NVRAM_SMS_CB_SMS_PAGE_SIZE*ENCODING_LENGTH),
									guiBuffer);
	SetLeftSoftkeyFunction(mmi_msg_entry_confirm_generic, KEY_EVENT_UP);
	SetRightSoftkeyFunction(mmi_msg_go_back_from_cb_message, KEY_EVENT_UP);
	SetKeyHandler (mmi_msg_go_back_from_cb_message, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_msg_end_key_from_cb_message, KEY_END, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_delete_cb_message
* DESCRIPTION
*   Delete CB message
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_delete_cb_message (void)
{
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] mmi_msg_delete_cb_message ---*\n"));
	SetMessagesScrnIdDelUpToHistoryNodes(SCR_ID_MSG_CB_MESSAGE);
	if(cbMsgIndex.pCBMsgContent[cb_display.currindex])
	{
		DeleteCBMessage(cb_display.currindex);
		if(cbMsgIndex.currCBTotal==0)
			SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_CB_MESSAGE_LIST);
		else
		{
			if(g_msg_cntx.currHiliteIndex==(S32)cbMsgIndex.currCBTotal)
				g_msg_cntx.currHiliteIndex--;
			else if(g_msg_cntx.currHiliteIndex>(S32)cbMsgIndex.currCBTotal)
				g_msg_cntx.currHiliteIndex=0;
		
			SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_CB_MESSAGE);
		}
		DisplayPopup ((PU8)GetString (STR_MESSAGES_DELETED_ID), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
		cb_display.currindex=MAX_STORE_CB_MSG;
		DeleteMessagesHistoryNodes ();
	}
	else
	{
		SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_CB_MESSAGE_LIST);
		DisplayPopup ((PU8)GetString (STR_CB_ERROR_MESSAGE), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		DeleteMessagesHistoryNodes ();
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_cb_mode
* DESCRIPTION
*   Highlight CB mode handler, check current CB mode
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_cb_mode (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
								STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
								mmi_msg_subscribe_cb, GoBackHistory);

	if(cb_info.mode==TRUE)
		ChangeLeftSoftkey(STR_GLOBAL_OFF, IMG_GLOBAL_OK);
	else
		ChangeLeftSoftkey(STR_GLOBAL_ON, IMG_GLOBAL_OK);	
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_hint_cb_mode
* DESCRIPTION
*   Hint CB mode handler
*
* PARAMETERS
*  index	IN		current menu index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_hint_cb_mode (U16 index)
{
	memset((S8*)hintData[index], 0, sizeof(hintData[index]));
	pfnUnicodeStrcpy((S8*)hintData[index], (S8*)cb_display.mode);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_cb_read_message
* DESCRIPTION
*   Highlight CB read message handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_cb_read_message (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_entry_cb_read_message, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_cb_language
* DESCRIPTION
*   Highlight CB language handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_cb_language (void)
{
       // wangbei del start 20060619
	/*mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_entry_cb_language, GoBackHistory);*/
       // wangbei del end
       // wangei add 20060619
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_cb_language, GoBackHistory);
       // wangbei add end
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_cb_channel_settings
* DESCRIPTION
*   Highlight CB channel settings handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_cb_channel_settings (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_entry_cb_channel_settings, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_cb_channel_select
* DESCRIPTION
*   Highlight CB channel select handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_cb_channel_select (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_entry_cb_channel_select, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_cb_channel_add
* DESCRIPTION
*   Highlight CB channel add handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_cb_channel_add (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_entry_cb_channel_add, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_cb_channel_edit
* DESCRIPTION
*   Highlight CB channel edit handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_cb_channel_edit (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_entry_cb_channel_list_edit, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  mmi_msg_highlight_cb_channel_delete
* DESCRIPTION
*   Highlight CB channel delete handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_msg_highlight_cb_channel_delete (void)
{
	mmi_msg_highlight_generic(	STR_GLOBAL_OK, 		IMG_GLOBAL_OK, 
										STR_GLOBAL_BACK, 		IMG_GLOBAL_BACK, 
										mmi_msg_pre_entry_cb_channel_list_delete, GoBackHistory);
}

/*****************************************************************************
* FUNCTION
*  InitCellBroadcastProtocolEventHanler
* DESCRIPTION
*   Init CB protocol event handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void InitCellBroadcastProtocolEventHanler(void)
{
	SetProtocolEventHandler (mmi_msg_handle_new_cb_ind, PRT_MSG_ID_MMI_CB_MSG_IND);
}

/*****************************************************************************
* FUNCTION
*  InitCBNVRAMData
* DESCRIPTION
*   Init CB NVRAMData
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void InitCBNVRAMData (void)
{
	if (IsCBMessageReadOperRequired)
	{
		IsCBMessageReadOperRequired=FALSE;
		#ifdef MMI_ON_WIN32
		cbMsgStart=(U8*)cbMsgContent[0];
		cbMsgIndex.pCBMsgTail=(U8*)cbMsgContent[0];
		#else		
		ReadCBMessageInfoFromNvram ();
		#endif
	}
}

/*****************************************************************************
* FUNCTION
*  InitCellBroadcastApp
* DESCRIPTION
*   Init CB application: highlight handler, data structure
*
* PARAMETERS
*  none
* RETURNS
*  the description of return value, if any.
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void InitCellBroadcastApp(void)
{
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*[CellBroadcast.c] InitCellBroadcastApp*\n"));
	// Set Hilite Handlers
	SetHiliteHandler (MITEM102_CB_SERVICE,			mmi_msg_highlight_cb_mode);
	SetHintHandler (MITEM102_CB_SERVICE,			mmi_msg_hint_cb_mode);
	SetHiliteHandler (MITEM102_CB_READ_MESSAGE,	mmi_msg_highlight_cb_read_message);
	SetHiliteHandler (MITEM102_CB_LANGUAGE,		mmi_msg_highlight_cb_language);
	SetHiliteHandler (MITEM102_CB_CHANNELS,		mmi_msg_highlight_cb_channel_settings);
	SetHiliteHandler (MITEM104_CB_CHANNEL_ADD,	mmi_msg_highlight_cb_channel_add);
	SetHiliteHandler (MITEM104_CB_CHANNEL_EDIT,  mmi_msg_highlight_cb_channel_edit);
	SetHiliteHandler (MITEM104_CB_CHANNEL_DELETE,mmi_msg_highlight_cb_channel_delete);
	SetHiliteHandler (MITEM104_CB_CHANNEL_SELECT,mmi_msg_highlight_cb_channel_select);
}

/*****************************************************************************
* FUNCTION
*  ResetCBInfoStructs
* DESCRIPTION
*   Reset Cb info structures
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ResetCBInfoStructs (void)
{
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*[CellBroadcast.c] ResetCBInfoStructs*\n"));
	memset(&cbNvramStruct, 0, sizeof(msg_cb_channel_nvram_struct));
	memset(&cbMsgInfo, 0, sizeof(msg_cb_msg_info_struct));
	memset(&cbMsgContent, 0, sizeof(msg_cb_msg_content_struct)*MAX_CB_MSG_TOTAL_PAGE*NVRAM_SMS_CB_SMS_PAGE_SIZE);
	memset(&cbMsgIndex, 0, sizeof(msg_cb_msg_index_struct));
#ifndef MMI_ON_HARDWARE_P
	cb_info.mode=TRUE;
	pfnUnicodeStrcpy((S8*)cb_display.mode, (S8*)GetString((U16)STR_GLOBAL_ON));
#endif	
}

/*****************************************************************************
* FUNCTION
*  DeInitCellBroadcastApp
* DESCRIPTION
*   Deinit Cb applications: reset data structures, write CB message to NVRAM
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DeInitCellBroadcastApp (void)
{
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*---[CellBroadcast.c] DeInitCellBroadcastApp ---*\n"));
	WriteCBMessageInfoToNvram();
	memset(&cbNvramStruct, 0, sizeof(msg_cb_channel_nvram_struct));
	memset(&cbMsgIndex, 0, sizeof(msg_cb_msg_info_struct));
	memset(&cbMsgContent, 0, sizeof(msg_cb_msg_content_struct)*MAX_CB_MSG_TOTAL_PAGE*NVRAM_SMS_CB_SMS_PAGE_SIZE);
	memset(&cbMsgIndex, 0, sizeof(msg_cb_msg_index_struct));
}
#endif // __MOD_SMSAL__
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

