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
 *   StandaloneRes.c
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *   This file implements standalone resource related functions
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 *******************************************************************************/

#include "mmi_features.h"

#include "string.h"
#include "mmi_data_types.h"
#include "custdatares.h"
#include "custmenures.h"
#include "gui_themes.h"
#include "fontdcl.h"
#include "wgui.h"
#include "downloadprotos.h"
//#include "pwic.h"
#include "imeres.h"

#ifdef MMI_ON_HARDWARE_P
#include "custom_jump_tbl.h"
#include "verno.h"
#ifdef __J2ME__
#include "j2me_custom.h"
#endif 

#if (!defined(__L1_STANDALONE__) && !defined(EMPTY_MMI))
extern void MMIResourceMismatchWarning(
                kal_int8 errType,
                kal_int8 errResType,
                kal_uint8 const *codeversion,
                kal_uint8 const *resversion);
#endif /* (!defined(__L1_STANDALONE__) && !defined(EMPTY_MMI)) */ 

#include "resource_audio.h"
#include "nvram_user_defs.h"
#include "resource_custpack_jtbl.h"
#if defined(OBIGO_SUPPORT) && defined(WAP_SUPPORT)
#include "custom_wap_config.h"
#endif 

/* IMAGE variables */
extern void *image2ndJumpTbl[];
extern Type_Preamble_Content ImageResPreamble;
CUSTOM_IMAGE *nCustImageNames = NULL;
U16 CurrMaxImageNum = 0;
S32 n_MMI_themes = 0;

char *mmi_theme_phone_model;


#ifdef __MMI_THEMES_V2_SUPPORT__
theme_details_struct *MMI_themes;
#else 
MMI_theme **MMI_themes = NULL;
#endif 

/* 101205 audio resource Calvin Start */
/* AUTOGEN AUDIO variables */
extern void *autogenaudio2ndJumpTbl[];
extern Type_Preamble_Content AutogenAudioResPreamble;
CUSTOM_AUDIO *nCustAudioNames = NULL;
U16 CurrMaxAudioNum = 0;

/* 101205 audio resource Calvin End */

/* Audio variables */
extern const device_tones_struct *mtk_resource_tones[];
extern const audio_resource_struct mtk_resource_midis[];
extern const audio_resource_struct mtk_resource_sounds[];
extern const audio_resource_struct mtk_resource_ems_sounds[];
extern const audio_resource_struct mtk_resource_imelodys[];
extern const audio_resource_struct mtk_resource_ems_imelodys[];
extern const audio_resource_struct mtk_resource_message_sounds[];
extern const audio_resource_struct mtk_resource_mms_sounds[];

#ifdef DIGIT_TONE_SUPPORT
extern const audio_resource_struct mtk_resource_human_voice_tones[];
#endif 
#ifdef SIMPLE_TTS
extern const audio_resource_struct mtk_resource_tts_sounds[];
extern const start_end_t mtk_TTS_UCS2_MSB_index[];
extern const unsigned char mtk_TTS_UCS2_LSB[];
extern const unsigned short mtk_TTS_index_table[];
#endif /* SIMPLE_TTS */ 
#ifdef CUST_KEYPAD_TONE_SUPPORT
extern const audio_resource_struct mtk_resource_keypad_tones[];
#endif 

extern Type_Preamble_Content AudioResPreamble;

#ifdef DIGIT_TONE_SUPPORT
audio_resource_struct *resource_human_voice_tones = NULL;
#endif 
#ifdef SIMPLE_TTS
audio_resource_struct *resource_tts_sounds = NULL;
start_end_t *TTS_UCS2_MSB_index = NULL;
unsigned char *TTS_UCS2_LSB = NULL;
unsigned short *TTS_index_table = NULL;
#endif /* SIMPLE_TTS */ 
#ifdef CUST_KEYPAD_TONE_SUPPORT
audio_resource_struct *resource_keypad_tones = NULL;
#endif 

/* Str variables */
extern void *langpack2ndJumpTbl[];
extern Type_Preamble_Content LangPackResPreamble;

//071706 menu resource
CUSTOM_MENU *nCustMenus = NULL;

sLanguageDetails *gLanguageArray = NULL;
U16 gMaxDeployedLangs = 0;

StringResList *gStringList = NULL;

#if defined(__MMI_T9__)
#if defined(__MMI_T9_V7__)
const sIMEModuleDetails gIMEModule = T9V7;
#else 
const sIMEModuleDetails gIMEModule = T9V6;
#endif 
#elif defined(__MMI_ZI__)
const sIMEModuleDetails gIMEModule = ZiV6;
#else 
const sIMEModuleDetails gIMEModule = IME_OTHERS;
#endif 

const sIMEModeDetails *gIMEModeArray = NULL;
const sIMEModeDetails *gIMEQSearchModeArray = NULL;
const sIMELDBDetails *gIMELDBArray = NULL;

#if defined(__MMI_T9__)
#if defined(__MMI_T9_V7__)
#if defined(__MMI_T9_TR_CHINESE__)
T9ChineseData T9FARDATA gT9TRChineseLDB;
#endif /* defined(__MMI_T9_TR_CHINESE__) */
#if defined(__MMI_T9_SM_CHINESE__)
T9ChineseData T9FARDATA gT9SMChineseLDB;
#endif /* defined(__MMI_T9_SM_CHINESE__) */
#else /* defined(__MMI_T9_V7__) */
#if defined(__MMI_T9_TR_CHINESE__) 
T9ChineseData FARDATA gT9TRChineseLDB;
#endif /* defined(__MMI_T9_TR_CHINESE__) */
#if defined(__MMI_T9_SM_CHINESE__)
T9ChineseData FARDATA gT9SMChineseLDB;
#endif /* defined(__MMI_T9_SM_CHINESE__) */
#endif /* defined(__MMI_T9_V7__) */ 
#endif /* defined(__MMI_T9__) */ 

/* J2ME variables */
#ifdef __J2ME__
extern void *J2ME2ndJumpTbl[];
extern Type_Preamble_Content J2MEResPreamble;
#endif /* __J2ME__ */ 

custpack_nvram_header *custpack_nvram_header_ptr;

extern void *custpack2ndJumpTbl[];
extern Type_Preamble_Content CustPackResPreamble;

/* CustPack image variables */
CUSTOM_IMAGE *nCustImageNamesEXT = NULL;
U16 CurrMaxImageNumEXT = 0;
CUSTPACK_IMAGE_HEADER *pimage_header = NULL;

/* 101205 audio resource Calvin Start */
/* CustPack image variables */
CUSTOM_AUDIO *nCustAudioNamesEXT = NULL;
U16 CurrMaxAudioNumEXT = 0;
CUSTPACK_AUDIO_HEADER *paudio_header = NULL;


/*****************************************************************************
 * FUNCTION
 *  StandaloneResValidation
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void StandaloneResValidation(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    JUMP_TABLE_ENTRY_ENUM res_type;
    kal_uint32 res_type_code, res_MAGIC;
    kal_char *project_ID;
    kal_int8 errorType = -1;
    kal_int8 errorResType = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (res_type = JUMP_TABLE_ENTRY_START; res_type <= JUMP_TABLE_ENTRY_END; res_type++)
    {
        project_ID = (kal_char*) GET_Res_Addr(res_type, 0);
        res_type_code = *(kal_uint32*) GET_Res_Addr(res_type, RESOURCE_TYPE_OFFSET);
        res_MAGIC = *(kal_uint32*) GET_Res_Addr(res_type, MAGIC_NUM_OFFSET);

        /* Check consistency between JUMP TABLE and the RESOURCE BIN */
        if (strncmp(project_ID, (const char*)GET_Res_ProjectID(res_type), strlen(VERNO_STR)))
        {
            errorType = 0;
            errorResType = res_type;
            goto Validation_Exit;
        }

        if (res_MAGIC != GET_Res_Magic(res_type))
        {
            goto Validation_Exit;
        }

        /* ID verification: PROJECT_ID */
        if (strncmp(project_ID, VERNO_STR, strlen(VERNO_STR)))
        {
            errorType = 0;
            errorResType = res_type;
            goto Validation_Exit;
        }

        /* ID verification: RESOURCE TYPE */
        if (((res_type_code & RESOURCE_TYPE_MASK) != RESOURCE_TYPE_AUDIO) &&
            ((res_type_code & RESOURCE_TYPE_MASK) != RESOURCE_TYPE_IMAGE) &&
            ((res_type_code & RESOURCE_TYPE_MASK) != RESOURCE_TYPE_LANG_PACK)
    #ifdef __J2ME__
            && ((res_type_code & RESOURCE_TYPE_MASK) != RESOURCE_TYPE_J2ME)
    #endif 
            && ((res_type_code & RESOURCE_TYPE_MASK) != RESOURCE_TYPE_CUSTPACK))
            goto Validation_Exit;

        /* MAGIC# verification */
        if (res_MAGIC != MAGIC_NUMBER)
        {
            goto Validation_Exit;
        }

    }

    return;

  Validation_Exit:
    {
        kal_uint8 dspl_text[PROJECTID_LEN];

        DRV_POWERON();

        if (strlen(VERNO_STR) < PROJECTID_LEN)
        {
            kal_mem_set(dspl_text, '\0', PROJECTID_LEN);
            strncpy((char*)dspl_text, (const char*)project_ID, sizeof(VERNO_STR));
            dspl_text[sizeof(VERNO_STR)] = '\0';

            MMIResourceMismatchWarning(
                errorType,
                errorResType,
                (kal_uint8 const*)VERNO_STR,
                (kal_uint8 const*)dspl_text);
        }
        else    /* VERNO string too long (> 32 characters) */
        {
            ASSERT(KAL_FALSE);
        }

        while (1);
    }
}


/*****************************************************************************
 * FUNCTION
 *  InitializeResourceVariables
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void InitializeResourceVariables(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    void **langpack_JTbl = (void **)GET_ResLangPack_Addr(ENTRY_PREAMBLE);
    void **custpack_header = (void **)GET_ResCUSTPACK_Addr(ENTRY_PREAMBLE);     /* __CUSTPACK_MULTIBIN__ */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* IMAGE Initialization */
    {
        nCustImageNames = (CUSTOM_IMAGE*) (image2ndJumpTbl[0]);
        CurrMaxImageNum = *((unsigned short*)(image2ndJumpTbl[1]));
    #ifdef __MMI_THEMES_V2_SUPPORT__
        MMI_themes = (theme_details_struct*) (image2ndJumpTbl[2]);
    #else 
        MMI_themes = (MMI_theme **) (image2ndJumpTbl[2]);
    #endif 
        n_MMI_themes = *((S32*) (image2ndJumpTbl[3]));
        mmi_theme_phone_model = (char*)(image2ndJumpTbl[4]);
    }

    /* AUDIO Initialization */
    {
        resource_tones = (device_tones_struct **) mtk_resource_tones;

    #ifdef MIDI_IN_CUSTPACK_BIN
        resource_midis =
            (audio_resource_struct*) ((custpack_audio_header*) (custpack_header[CUSTPACK_AUDIO]))->mtk_resource_midis;
    #else /* MIDI_IN_CUSTPACK_BIN */ 
        resource_midis = (audio_resource_struct*) mtk_resource_midis;
    #endif /* MIDI_IN_CUSTPACK_BIN */ 

    #ifdef SND_IN_CUSTPACK_BIN
        resource_sounds =
            (audio_resource_struct*) ((custpack_audio_header*) (custpack_header[CUSTPACK_AUDIO]))->
            mtk_resource_sounds;
    #else /* SND_IN_CUSTPACK_BIN */ 
        resource_sounds = (audio_resource_struct*) mtk_resource_sounds;
    #endif /* SND_IN_CUSTPACK_BIN */ 

    #ifdef EMS_SND_IN_CUSTPACK_BIN
        resource_ems_sounds =
            (audio_resource_struct*) ((custpack_audio_header*) (custpack_header[CUSTPACK_AUDIO]))->
            mtk_resource_ems_sounds;
    #else /* EMS_SND_IN_CUSTPACK_BIN */ 
        resource_ems_sounds = (audio_resource_struct*) mtk_resource_ems_sounds;
    #endif /* EMS_SND_IN_CUSTPACK_BIN */ 

    #ifdef IMY_IN_CUSTPACK_BIN
        resource_imelodys =
            (audio_resource_struct*) ((custpack_audio_header*) (custpack_header[CUSTPACK_AUDIO]))->
            mtk_resource_imelodys;
    #else /* IMY_IN_CUSTPACK_BIN */ 
        resource_imelodys = (audio_resource_struct*) mtk_resource_imelodys;
    #endif /* IMY_IN_CUSTPACK_BIN */ 

    #ifdef EMS_IMY_IN_CUSTPACK_BIN
        resource_ems_imelodys =
            (audio_resource_struct*) ((custpack_audio_header*) (custpack_header[CUSTPACK_AUDIO]))->
            mtk_resource_ems_imelodys;
    #else /* EMS_IMY_IN_CUSTPACK_BIN */ 
        resource_ems_imelodys = (audio_resource_struct*) mtk_resource_ems_imelodys;
    #endif /* EMS_IMY_IN_CUSTPACK_BIN */ 

    #ifdef MSG_SND_IN_CUSTPACK_BIN
        resource_message_sounds =
            (audio_resource_struct*) ((custpack_audio_header*) (custpack_header[CUSTPACK_AUDIO]))->
            mtk_resource_message_sounds;
    #else /* MSG_SND_IN_CUSTPACK_BIN */ 
        resource_message_sounds = (audio_resource_struct*) mtk_resource_message_sounds;
    #endif /* MSG_SND_IN_CUSTPACK_BIN */ 

    #ifdef MMS_SND_IN_CUSTPACK_BIN
        resource_mms_sounds =
            (audio_resource_struct*) ((custpack_audio_header*) (custpack_header[CUSTPACK_AUDIO]))->
            mtk_resource_mms_sounds;
    #else /* MMS_SND_IN_CUSTPACK_BIN */ 
        resource_mms_sounds = (audio_resource_struct*) mtk_resource_mms_sounds;
    #endif /* MMS_SND_IN_CUSTPACK_BIN */ 

    #ifdef DIGIT_TONE_SUPPORT
    #ifdef HUMAN_VOICE_IN_CUSTPACK_BIN
        resource_human_voice_tones =
            (audio_resource_struct*) ((custpack_audio_header*) (custpack_header[CUSTPACK_AUDIO]))->
            mtk_resource_human_voice_tones;
    #else /* HUMAN_VOICE_IN_CUSTPACK_BIN */ 
        resource_human_voice_tones = (audio_resource_struct*) mtk_resource_human_voice_tones;
    #endif /* HUMAN_VOICE_IN_CUSTPACK_BIN */ 
    #endif /* DIGIT_TONE_SUPPORT */ 

    #ifdef SIMPLE_TTS
        resource_tts_sounds = (audio_resource_struct*) mtk_resource_tts_sounds;
        TTS_UCS2_MSB_index = (start_end_t*) mtk_TTS_UCS2_MSB_index;
        TTS_UCS2_LSB = (unsigned char*)mtk_TTS_UCS2_LSB;
        TTS_index_table = (unsigned short*)mtk_TTS_index_table;
    #endif /* SIMPLE_TTS */ 

    #ifdef CUST_KEYPAD_TONE_SUPPORT
    #ifdef CUST_KEYPAD_TONE_IN_CUSTPACK_BIN
        resource_keypad_tones =
            (audio_resource_struct*) ((custpack_audio_header*) (custpack_header[CUSTPACK_AUDIO]))->
            mtk_resource_keypad_tones;
    #else /* CUST_KEYPAD_TONE_IN_CUSTPACK_BIN */ 
        resource_keypad_tones = (audio_resource_struct*) mtk_resource_keypad_tones;
    #endif /* CUST_KEYPAD_TONE_IN_CUSTPACK_BIN */ 
    #endif /* CUST_KEYPAD_TONE_SUPPORT */ 
    }

    /* 101205 audio resource Calvin Start */
    /* AUTOGEN AUDIO Initialization */
    {
        nCustAudioNames = (CUSTOM_AUDIO*) (autogenaudio2ndJumpTbl[0]);
        CurrMaxAudioNum = *((unsigned short*)(autogenaudio2ndJumpTbl[1]));
    }
    /* 101205 audio resource Calvin End */

#ifdef __J2ME__
    /* For backward compatiable old project without j2me feature */
    /* J2ME Initialization */
    {
        j2me_custom_file_info_ptr = (j2me_custom_file_info_struct*) (J2ME2ndJumpTbl[GAMETYPE_JAVA_GAME]);
        if (j2me_custom_file_info_ptr)
        {
            game_j2me_default_game_count = j2me_custom_file_info_ptr->default_mids_count;
            game_virtual_file_count = j2me_custom_file_info_ptr->virtual_file_info.virtual_file_count;
        }

	#ifdef __SUPPORT_INFUSIO__
        eureka_custom_file_info_ptr = (j2me_custom_file_info_struct*) (J2ME2ndJumpTbl[GAMETYPE_INFUSIO_EUREKA]);
        if (eureka_custom_file_info_ptr)
        {
            eureka_default_game_count = eureka_custom_file_info_ptr->default_mids_count;
            eureka_virtual_file_count = eureka_custom_file_info_ptr->virtual_file_info.virtual_file_count;
        }	
	#endif /* __SUPPORT_INFUSIO__ */
	
	#ifdef SUPPORT_SURFKITCHEN
        surfkitchen_custom_file_info_ptr = (j2me_custom_file_info_struct*) (J2ME2ndJumpTbl[GAMETYPE_SURFKITCHEN]);
        if (surfkitchen_custom_file_info_ptr)
        {
            surfkitchen_default_game_count = surfkitchen_custom_file_info_ptr->default_mids_count;
            surfkitchen_virtual_file_count = surfkitchen_custom_file_info_ptr->virtual_file_info.virtual_file_count;
        }	
	#endif /* SUPPORT_SURFKITCHEN */

	#ifdef SUPPORT_MUSICWAV
        musicwav_custom_file_info_ptr = (j2me_custom_file_info_struct*) (J2ME2ndJumpTbl[GAMETYPE_MUSICWAV]);
        if (musicwav_custom_file_info_ptr)
        {
            musicwav_default_game_count = musicwav_custom_file_info_ptr->default_mids_count;
            musicwav_virtual_file_count = musicwav_custom_file_info_ptr->virtual_file_info.virtual_file_count;
        }	
	#endif /* SUPPORT_MUSICWAV */
	
    }
#endif /* __J2ME__ */ 

    if (((kal_uint32) langpack_JTbl > 0x1) && ((kal_uint32) & LangPackResPreamble > 0x1))       /* dummy reference to make the variable exist */
    {
        gLanguageArray = (sLanguageDetails*) (langpack2ndJumpTbl[0]);
        gMaxDeployedLangs = *((unsigned short*)(langpack2ndJumpTbl[1]));

        gStringList = (StringResList*) (langpack2ndJumpTbl[2]);

        if (gIMEModule != *((sIMEModuleDetails*) (langpack2ndJumpTbl[6])))
        {
            ASSERT(0);
        }
        gIMEModeArray = (sIMEModeDetails*) (langpack2ndJumpTbl[3]);
        gIMEQSearchModeArray = (sIMEModeDetails*) (langpack2ndJumpTbl[4]);
        gIMELDBArray = (sIMELDBDetails*) (langpack2ndJumpTbl[5]);
    #if defined(__MMI_T9__)
    #if defined(__MMI_T9_TR_CHINESE__)
    #if defined(__MMI_T9_V7__)
        gT9TRChineseLDB = *((T9ChineseData const T9FARDATA*)(langpack2ndJumpTbl[7]));
    #else 
        gT9TRChineseLDB = *((T9ChineseData const FARDATA*)(langpack2ndJumpTbl[7]));
    #endif 
    #endif /* defined(__MMI_T9_TR_CHINESE__) */ 
    #if defined(__MMI_T9_SM_CHINESE__)
    #if defined(__MMI_T9_V7__)
        gT9SMChineseLDB = *((T9ChineseData const T9FARDATA*)(langpack2ndJumpTbl[8]));
    #else 
        gT9SMChineseLDB = *((T9ChineseData const FARDATA*)(langpack2ndJumpTbl[8]));
    #endif 
    #endif /* defined(__MMI_T9_SM_CHINESE__) */ 
    #endif /* defined(__MMI_T9__) */ 
        nCustMenus = (CUSTOM_MENU *)(langpack2ndJumpTbl[9]);//071706 menu resource
    }

    /* CustPack Initialization */
    if (((kal_uint32) custpack2ndJumpTbl > 0x1) && ((kal_uint32) & CustPackResPreamble > 0x1))  /* dummy reference to make the variable exist */
    {
        /* CustPack NVRAM Initialized in NVRAM init procedure */
        custpack_nvram_header_ptr = (custpack_nvram_header*) custpack_header[CUSTPACK_NVRAM];

        /* CustPack Image Initialization */
        pimage_header = (CUSTPACK_IMAGE_HEADER*) (custpack_header[CUSTPACK_IMAGE]);
        nCustImageNamesEXT = (CUSTOM_IMAGE*) pimage_header->CustImageNamesEXT;
        CurrMaxImageNumEXT = pimage_header->MaxImageNumEXT;

        /* CustPack Autogen Audio Initialization */
        paudio_header = (CUSTPACK_AUDIO_HEADER*) (custpack_header[CUSTPACK_AUTOGEN_AUDIO]);
        nCustAudioNamesEXT = (CUSTOM_AUDIO*) paudio_header->CustAudioNamesEXT;
        CurrMaxAudioNumEXT = paudio_header->MaxAudioNumEXT;

    #if defined(OBIGO_SUPPORT) && defined(WAP_SUPPORT)
        {
            wap_custpack_header *custpack_wap_header_ptr = (wap_custpack_header*) custpack_header[CUSTPACK_WAP];

            custpack_wap_default.wap_profile_default = custpack_wap_header_ptr->wap_profile_default;
            custpack_wap_default.mms_profile_default = custpack_wap_header_ptr->mms_profile_default;
            custpack_wap_default.wap_bkm_default = custpack_wap_header_ptr->wap_bkm_default;
            custpack_wap_default.wap_root_ca_default = custpack_wap_header_ptr->wap_root_ca_default;
            custpack_wap_default.wap_config_default = custpack_wap_header_ptr->wap_config_default;
            custpack_wap_default.mms_config_default = custpack_wap_header_ptr->mms_config_default;
        }
    #endif /* defined(OBIGO_SUPPORT) && defined(WAP_SUPPORT) */ 
    }

    StandaloneResValidation();
}

#else /* MMI_ON_HARDWARE_P */ /* PC_SIMULATOR */
HINSTANCE hinstLib;

device_tones_struct **resource_tones = NULL;
audio_resource_struct *resource_midis = NULL;
audio_resource_struct *resource_sounds = NULL;
audio_resource_struct *resource_ems_sounds = NULL;
audio_resource_struct *resource_imelodys = NULL;
audio_resource_struct *resource_ems_imelodys = NULL;
audio_resource_struct *resource_message_sounds = NULL;
audio_resource_struct *resource_mms_sounds = NULL;

#ifdef DIGIT_TONE_SUPPORT
audio_resource_struct *resource_human_voice_tones = NULL;
#endif 
#ifdef SIMPLE_TTS
audio_resource_struct *resource_tts_sounds = NULL;
start_end_t *TTS_UCS2_MSB_index = NULL;
unsigned char *TTS_UCS2_LSB = NULL;
unsigned short *TTS_index_table = NULL;
#endif /* SIMPLE_TTS */ 
#ifdef CUST_KEYPAD_TONE_SUPPORT
audio_resource_struct *resource_keypad_tones = NULL;
#endif 

/* Img variables */
CUSTOM_IMAGE *nCustImageNames;
CUSTOM_IMAGE_MAP *ImageIdMap;
U16 CurrMaxImageId;     /* Max String Ids */
U16 *pCurrMaxImageId;   /* Max String Ids */
U16 CurrMaxSearchImageId;
U16 *pCurrMaxSearchImageId;
U16 CurrMaxImageNum;    /* Max Number Ids */
U16 *pCurrMaxImageNum;  /* Max Number Ids */
const CUSTOM_IMAGE_SEARCH_MAP *ImageIdSearchMap;

/* CustPack Image variables */
CUSTPACK_IMAGE_HEADER *pimage_header;
CUSTOM_IMAGE *nCustImageNamesEXT;
CUSTOM_IMAGE_MAP *ImageIdMapEXT;
U16 CurrMaxImageIdEXT;      /* Max String Ids */
U16 *pCurrMaxImageIdEXT;    /* Max String Ids */
U16 CurrMaxSearchImageIdEXT;
U16 *pCurrMaxSearchImageIdEXT;
U16 CurrMaxImageNumEXT;     /* Max Number Ids */
const CUSTOM_IMAGE_SEARCH_MAP *ImageIdSearchMapEXT;

/* 101205 audio resource Calvin Satrt */
/* Ado variables */
CUSTOM_AUDIO *nCustAudioNames;
CUSTOM_AUDIO_MAP *AudioIdMap;
U16 CurrMaxAudioId;
U16 *pCurrMaxAudioId;
U16 CurrMaxSearchAudioId;
U16 *pCurrMaxSearchAudioId;
U16 CurrMaxAudioNum;
U16 *pCurrMaxAudioNum;
const CUSTOM_AUDIO_SEARCH_MAP *AudioIdSearchMap;

/* CustPack Audio variables */
CUSTPACK_AUDIO_HEADER *paudio_header;
CUSTOM_AUDIO *nCustAudioNamesEXT;
CUSTOM_AUDIO_MAP *AudioIdMapEXT;
U16 CurrMaxAudioIdEXT;
U16 *pCurrMaxAudioIdEXT;
U16 CurrMaxSearchAudioIdEXT;
U16 *pCurrMaxSearchAudioIdEXT;
U16 CurrMaxAudioNumEXT;
const CUSTOM_AUDIO_SEARCH_MAP *AudioIdSearchMapEXT;

/* 101205 audio resource Calvin End */

/* 101806 E-NFB start */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
U32 CurrMaxENFBAssociatedIDNum = 0;
CUSTOM_ENFB *AssociatedID_map = NULL;
#endif
/* 101806 E-NFB end */

/* Menu variables */
CUSTOM_MENU *nCustMenus;

/* Str variables */
StringResList *gStringList = NULL;

/* Language variables */
sLanguageDetails *gLanguageArray;
U16 gMaxDeployedLangs;
U16 *pgMaxDeployedLangs = NULL;

/* Font variables */
stFontAttribute *pUI_DEFAULT_FONT;
stFontAttribute UI_DEFAULT_FONT;
stFontAttribute *pMMI_default_font;
stFontAttribute MMI_default_font;
stFontAttribute *pMMI_large_font;
stFontAttribute MMI_large_font;
stFontAttribute *pMMI_medium_font;
stFontAttribute MMI_medium_font;
stFontAttribute *pMMI_small_font;
stFontAttribute MMI_small_font;
stFontAttribute *pMMI_medium_bold_font;
stFontAttribute MMI_medium_bold_font;
stFontAttribute *pMMI_medium_bold_font;
stFontAttribute MMI_medium_bold_font;
stFontAttribute *pwgui_dialer_box_f1;
stFontAttribute wgui_dialer_box_f1;
stFontAttribute *pwgui_dialer_box_f2;
stFontAttribute wgui_dialer_box_f2;
stFontAttribute *pMMI_sublcd_font;
stFontAttribute MMI_sublcd_font;
stFontAttribute *pMMI_virtual_keyboard_font;
stFontAttribute MMI_virtual_keyboard_font;

/* __CUSTPACK_MULTIBIN     Terry   BEGIN */
/* IME variables */
#if defined(__MMI_T9__)
#if defined(__MMI_T9_V7__)
const sIMEModuleDetails gIMEModule = T9V7;
#else 
const sIMEModuleDetails gIMEModule = T9V6;
#endif 
#elif defined(__MMI_ZI__)
const sIMEModuleDetails gIMEModule = ZiV6;
#else 
const sIMEModuleDetails gIMEModule = IME_OTHERS;
#endif 
const sIMEModeDetails *gIMEModeArray = NULL;
const sIMEModeDetails *gIMEQSearchModeArray = NULL;
const sIMELDBDetails *gIMELDBArray = NULL;

#if defined(__MMI_T9__)
#if defined(__MMI_T9_V7__)
T9ChineseData T9FARDATA *pgT9TRChineseLDB;
T9ChineseData T9FARDATA gT9TRChineseLDB;
T9ChineseData T9FARDATA *pgT9SMChineseLDB;
T9ChineseData T9FARDATA gT9SMChineseLDB;
#else /* defined(__MMI_T9_V7__) */ 
T9ChineseData FARDATA *pgT9TRChineseLDB;
T9ChineseData FARDATA gT9TRChineseLDB;
T9ChineseData FARDATA *pgT9SMChineseLDB;
T9ChineseData FARDATA gT9SMChineseLDB;
#endif /* defined(__MMI_T9_V7__) */ 
#endif /* defined(__MMI_T9__) */ 
/* __CUSTPACK_MULTIBIN     Terry   END */
/* Theme variables */
S32 *pn_MMI_themes;
S32 n_MMI_themes;

PS8 mmi_theme_phone_model;


#ifdef __MMI_THEMES_V2_SUPPORT__
theme_details_struct *MMI_themes;
#else 
MMI_theme **MMI_themes;
#endif 


/*****************************************************************************
 * FUNCTION
 *  InitializeResourceVariables
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void InitializeResourceVariables(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int fRunTimeLinkSuccess = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Load dll */
    hinstLib = LoadLibrary("mmiresource");

    /* assign address */
    if (hinstLib != NULL)
    {
        nCustImageNames = (CUSTOM_IMAGE*) GetProcAddress(hinstLib, "mtk_nCustImageNames");
        pCurrMaxImageId = (U16*) GetProcAddress(hinstLib, "CurrMaxImageId");
        CurrMaxImageId = *pCurrMaxImageId;
        pCurrMaxSearchImageId = (U16*) GetProcAddress(hinstLib, "CurrMaxSearchImageId");
        CurrMaxSearchImageId = *pCurrMaxSearchImageId;
        pCurrMaxImageNum = (U16*) GetProcAddress(hinstLib, "mtk_CurrMaxImageNum");
        CurrMaxImageNum = *pCurrMaxImageNum;
        ImageIdMap = (CUSTOM_IMAGE_MAP*) GetProcAddress(hinstLib, "ImageIdMap");
        ImageIdSearchMap = (CUSTOM_IMAGE_SEARCH_MAP*) GetProcAddress(hinstLib, "ImageIdSearchMap");

        /* __CUSTPACK_MULTIBIN Calvin BEGIN */
        /* 040805 CustPack: Calvin added */
        pimage_header = (CUSTPACK_IMAGE_HEADER*) GetProcAddress(hinstLib, "mtk_image_header");
        /* nCustImageNamesEXT = (CUSTOM_IMAGE*) GetProcAddress(hinstLib,"mtk_nCustImageNamesEXT"); */
        nCustImageNamesEXT = (CUSTOM_IMAGE*) pimage_header->CustImageNamesEXT;
        pCurrMaxImageIdEXT = (U16*) GetProcAddress(hinstLib, "CurrMaxImageIdEXT");
        CurrMaxImageIdEXT = *pCurrMaxImageIdEXT;
        pCurrMaxSearchImageIdEXT = (U16*) GetProcAddress(hinstLib, "CurrMaxSearchImageIdEXT");
        CurrMaxSearchImageIdEXT = *pCurrMaxSearchImageIdEXT;
        //pCurrMaxImageNumEXT = (U16*) GetProcAddress(hinstLib,"mtk_CurrMaxImageNumEXT");
        //CurrMaxImageNumEXT = *pCurrMaxImageNumEXT;
        CurrMaxImageNumEXT = pimage_header->MaxImageNumEXT;
        ImageIdMapEXT = (CUSTOM_IMAGE_MAP*) GetProcAddress(hinstLib, "ImageIdMapEXT");
        ImageIdSearchMapEXT = (CUSTOM_IMAGE_SEARCH_MAP*) GetProcAddress(hinstLib, "ImageIdSearchMapEXT");
        /* Calvin end */
        /* __CUSTPACK_MULTIBIN Calvin END */

        /* 101806 E-NFB start */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
        CurrMaxENFBAssociatedIDNum = *(U32 *)GetProcAddress(hinstLib, "CurrMaxENFBAssociatedIDNum");
        AssociatedID_map = (CUSTOM_ENFB *)GetProcAddress(hinstLib, "AssociatedID_map");
#endif /* __MMI_RESOURCE_ENFB_SUPPORT__ */
        /* 101806 E-NFB end */

        /* 101205 audio resource Calvin Start */
        nCustAudioNames = (CUSTOM_AUDIO*) GetProcAddress(hinstLib, "mtk_nCustAudioNames");
        pCurrMaxAudioId = (U16*) GetProcAddress(hinstLib, "CurrMaxAudioId");
        CurrMaxAudioId = *pCurrMaxAudioId;
        pCurrMaxSearchAudioId = (U16*) GetProcAddress(hinstLib, "CurrMaxSearchAudioId");
        CurrMaxSearchAudioId = *pCurrMaxSearchAudioId;
        pCurrMaxAudioNum = (U16*) GetProcAddress(hinstLib, "mtk_CurrMaxAudioNum");
        CurrMaxAudioNum = *pCurrMaxAudioNum;
        AudioIdMap = (CUSTOM_AUDIO_MAP*) GetProcAddress(hinstLib, "AudioIdMap");
        AudioIdSearchMap = (CUSTOM_AUDIO_SEARCH_MAP*) GetProcAddress(hinstLib, "AudioIdSearchMap");

        paudio_header = (CUSTPACK_AUDIO_HEADER*) GetProcAddress(hinstLib, "mtk_audio_header");
        nCustAudioNamesEXT = (CUSTOM_AUDIO*) paudio_header->CustAudioNamesEXT;
        pCurrMaxAudioIdEXT = (U16*) GetProcAddress(hinstLib, "CurrMaxAudioIdEXT");
        CurrMaxAudioIdEXT = *pCurrMaxAudioIdEXT;
        pCurrMaxSearchAudioIdEXT = (U16*) GetProcAddress(hinstLib, "CurrMaxSearchAudioIdEXT");
        CurrMaxSearchAudioIdEXT = *pCurrMaxSearchAudioIdEXT;
        CurrMaxAudioNumEXT = paudio_header->MaxAudioNumEXT;
        AudioIdMapEXT = (CUSTOM_AUDIO_MAP*) GetProcAddress(hinstLib, "AudioIdMapEXT");
        AudioIdSearchMapEXT = (CUSTOM_AUDIO_SEARCH_MAP*) GetProcAddress(hinstLib, "AudioIdSearchMapEXT");
        /* 101205 audio resource End */

        nCustMenus = (CUSTOM_MENU*) GetProcAddress(hinstLib, "mtk_nCustMenus");//071706 menu resource

        gStringList = (StringResList*) GetProcAddress(hinstLib, "mtk_gStringList");

        pgMaxDeployedLangs = (U16*) GetProcAddress(hinstLib, "mtk_gMaxDeployedLangs");
        gMaxDeployedLangs = *pgMaxDeployedLangs;
        gLanguageArray = (sLanguageDetails*) GetProcAddress(hinstLib, "mtk_gLanguageArray");

        pUI_DEFAULT_FONT = (stFontAttribute*) GetProcAddress(hinstLib, "UI_DEFAULT_FONT");
        UI_DEFAULT_FONT = *pUI_DEFAULT_FONT;
        pMMI_default_font = (stFontAttribute*) GetProcAddress(hinstLib, "MMI_default_font");
        MMI_default_font = *pMMI_default_font;
        pMMI_large_font = (stFontAttribute*) GetProcAddress(hinstLib, "MMI_large_font");
        MMI_large_font = *pMMI_large_font;
        pMMI_medium_font = (stFontAttribute*) GetProcAddress(hinstLib, "MMI_medium_font");
        MMI_medium_font = *pMMI_medium_font;
        pMMI_medium_bold_font = (stFontAttribute*) GetProcAddress(hinstLib, "MMI_medium_bold_font");
        MMI_medium_bold_font = *pMMI_medium_bold_font;
        pMMI_small_font = (stFontAttribute*) GetProcAddress(hinstLib, "MMI_small_font");
        MMI_small_font = *pMMI_small_font;
        pwgui_dialer_box_f1 = (stFontAttribute*) GetProcAddress(hinstLib, "wgui_dialer_box_f1");
        wgui_dialer_box_f1 = *pwgui_dialer_box_f1;
        pwgui_dialer_box_f2 = (stFontAttribute*) GetProcAddress(hinstLib, "wgui_dialer_box_f2");
        wgui_dialer_box_f2 = *pwgui_dialer_box_f2;
        pMMI_sublcd_font = (stFontAttribute*) GetProcAddress(hinstLib, "MMI_sublcd_font");
        MMI_sublcd_font = *pMMI_sublcd_font;
        pMMI_virtual_keyboard_font = (stFontAttribute*) GetProcAddress(hinstLib, "MMI_virtual_keyboard_font");
        MMI_virtual_keyboard_font = *pMMI_virtual_keyboard_font;

        /* __CUSTPACK_MULTIBIN     Terry   BEGIN */
        gIMEModeArray = (sIMEModeDetails*) GetProcAddress(hinstLib, "mtk_gIMEModeArray");
        gIMEQSearchModeArray = (sIMEModeDetails*) GetProcAddress(hinstLib, "mtk_gIMEQSearchModeArray");
        gIMELDBArray = (sIMELDBDetails*) GetProcAddress(hinstLib, "mtk_gIMELDBArray");
    #if defined(__MMI_T9__)
    #if defined(__MMI_T9_TR_CHINESE__)
    #if defined(__MMI_T9_V7__)
        pgT9TRChineseLDB = ((T9ChineseData const T9FARDATA*)GetProcAddress(hinstLib, "l0104"));
        gT9TRChineseLDB = *pgT9TRChineseLDB;
    #else /* defined(__MMI_T9_V7__) */ 
        pgT9TRChineseLDB = ((T9ChineseData const FARDATA*)GetProcAddress(hinstLib, "l0104"));
        gT9TRChineseLDB = *pgT9TRChineseLDB;
    #endif /* defined(__MMI_T9_V7__) */ 
    #endif /* defined(__MMI_T9_TR_CHINESE__) */ 
    #if defined(__MMI_T9_SM_CHINESE__)
    #if defined(__MMI_T9_V7__)
        pgT9SMChineseLDB = ((T9ChineseData const T9FARDATA*)(GetProcAddress(hinstLib, "l0204")));
        gT9SMChineseLDB = *pgT9SMChineseLDB;
    #else /* defined(__MMI_T9_V7__) */ 
        pgT9SMChineseLDB = ((T9ChineseData const FARDATA*)(GetProcAddress(hinstLib, "l0204")));
        gT9SMChineseLDB = *pgT9SMChineseLDB;
    #endif /* defined(__MMI_T9_V7__) */ 
    #endif /* defined(__MMI_T9_SM_CHINESE__) */ 
    #endif /* defined(__MMI_T9__) */ 
        /* __CUSTPACK_MULTIBIN     Terry   END */
        pn_MMI_themes = (S32*) GetProcAddress(hinstLib, "mtk_n_MMI_themes");
        n_MMI_themes = *pn_MMI_themes;
    #ifdef __MMI_THEMES_V2_SUPPORT__
        MMI_themes = (theme_details_struct*) GetProcAddress(hinstLib, "mtk_MMI_themes");
    #else 
        MMI_themes = (MMI_theme **) GetProcAddress(hinstLib, "mtk_MMI_themes");
    #endif 
        mmi_theme_phone_model = (char*)GetProcAddress(hinstLib, "theme_phone_model");
    }
}

#endif /* MMI_ON_HARDWARE_P */ 

