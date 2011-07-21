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
 *   StandaloneRes.c
 *
 * Project:
 * --------
 
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
 
 *------------------------------------------------------------------------------
 *********************************************************************/
#include "mmi_features.h"

#include "string.h"
#include "mmi_data_types.h"
#include "custdatares.h"
#include "custmenures.h"
#include "gui_themes.h"
#include "fontdcl.h"
#include "wgui.h"
#include "downloadprotos.h"
#ifndef MMI_ON_HARDWARE_P
#include "nvramenum.h"
#endif
//#include "pwic.h"
 
#include "imeres.h"
 


#include "custom_jump_tbl.h"
#include "verno.h"
#ifdef __J2ME__
#if 0//JiaShuo Delete for J2ME
#include "j2me_custom.h"
#endif
#endif /* __J2ME__ */

#if (!defined(__L1_STANDALONE__) && !defined(EMPTY_MMI) )
extern void  MMIResourceMismatchWarning(kal_int8 errType, kal_int8 errResType, kal_uint8 const* codeversion, kal_uint8 const* resversion);
#endif

#include "resource_audio.h"

//__CUSTPACK_MULTIBIN__
#include "nvram_user_defs.h" 
#include "resource_custpack_jtbl.h"
#ifdef WAP_SUPPORT
#include "custom_wap_config.h"
#endif
#include "csw.h"

/* IMAGE variables */
extern void* image2ndJumpTbl[];
extern Type_Preamble_Content ImageResPreamble;
CUSTOM_IMAGE* nCustImageNames = NULL;
U16 CurrMaxImageNum = 0; 
S32 n_MMI_themes = 0;
#ifdef __MMI_THEMES_V2_SUPPORT__
	theme_details_struct *MMI_themes;
#else
MMI_theme **MMI_themes = NULL;
#endif
//MMI_theme **MMI_themes = NULL;


/* Audio variables */
//extern const device_tones_struct *CSD_resource_tones[];//Jiashuo remove
extern const audio_resource_struct CSD_resource_midis[];
extern const audio_resource_struct CSD_resource_sounds[];
extern const audio_resource_struct CSD_resource_poweronoff_sounds[];
//extern const audio_resource_struct CSD_resource_ems_sounds[];//Jiashuo remove
extern const audio_resource_struct CSD_resource_imelodys[];
//extern const audio_resource_struct CSD_resource_ems_imelodys[];//Jiashuo remove
extern const audio_resource_struct CSD_resource_message_sounds[];
//extern const audio_resource_struct CSD_resource_mms_sounds[];//Jiashuo remove
#if defined(__MMI_GAME__)
extern const audio_resource_struct CSD_resource_fng_sounds[];    //Added by jinzh:20070724
#endif
/*Jinzh Add Start For 5754 Ver: TBM780  on 2007-8-8 16:13 */
extern const audio_resource_struct CSD_resource_camera_sounds[];
/*Jinzh Add End  For 5754 Ver: TBM780  on 2007-8-8 16:13 */
#ifdef DIGIT_TONE_SUPPORT
extern const audio_resource_struct CSD_resource_human_voice_tones[];
#endif
#ifdef SIMPLE_TTS
extern const audio_resource_struct CSD_resource_tts_sounds[];
extern const start_end_t CSD_TTS_UCS2_MSB_index[];
extern const unsigned char CSD_TTS_UCS2_LSB[];
extern const unsigned short CSD_TTS_index_table[];
#endif
#ifdef CUST_KEYPAD_TONE_SUPPORT
extern const audio_resource_struct CSD_resource_keypad_tones[];
#endif

extern Type_Preamble_Content AudioResPreamble;
device_tones_struct** resource_tones = NULL;
audio_resource_struct* resource_midis = NULL;
audio_resource_struct* resource_sounds = NULL;
audio_resource_struct* resource_ems_sounds = NULL;	
audio_resource_struct* resource_imelodys = NULL;
audio_resource_struct* resource_ems_imelodys = NULL;
audio_resource_struct* resource_message_sounds = NULL;
audio_resource_struct* resource_poweronoff_sounds = NULL;
audio_resource_struct* resource_mms_sounds = NULL;
audio_resource_struct* resource_fng_sounds = NULL;   //Added by jinzh:20070724
/*Jinzh Add Start For 5754 Ver: TBM780  on 2007-8-8 16:23 */
audio_resource_struct* resource_camera_sounds = NULL;   
/*Jinzh Add End  For 5754 Ver: TBM780  on 2007-8-8 16:23 */
#ifdef DIGIT_TONE_SUPPORT
audio_resource_struct* resource_human_voice_tones = NULL;
#endif
#ifdef SIMPLE_TTS
audio_resource_struct* resource_tts_sounds = NULL;
start_end_t* TTS_UCS2_MSB_index = NULL;
unsigned char* TTS_UCS2_LSB = NULL;
unsigned short* TTS_index_table = NULL;
#endif
#ifdef CUST_KEYPAD_TONE_SUPPORT
audio_resource_struct* resource_keypad_tones = NULL;
#endif

/* Str variables */
extern void* langpack2ndJumpTbl[];
extern Type_Preamble_Content LangPackResPreamble;

sLanguageDetails* gLanguageArray = NULL;
U16 gMaxDeployedLangs = 0;

StringResList	*gStringList = NULL;

 
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

const sIMEModeDetails* gIMEModeArray = NULL;
const sIMEModeDetails* gIMEQSearchModeArray = NULL;
const sIMELDBDetails* gIMELDBArray=NULL;
#if defined(__MMI_T9__)
#if defined(__MMI_T9_V7__)
T9ChineseData T9FARDATA gT9TRChineseLDB=NULL;
T9ChineseData T9FARDATA gT9SMChineseLDB=NULL;
#else
T9ChineseData FARDATA gT9TRChineseLDB=NULL;
T9ChineseData FARDATA gT9SMChineseLDB=NULL;
#endif
#endif
 

/* J2ME variables */
#ifdef __J2ME__
extern void* J2ME2ndJumpTbl[];
extern Type_Preamble_Content J2MEResPreamble;
#endif /* __J2ME__ */
 //__CUSTPACK_MULTIBIN__

custpack_nvram_header *custpack_nvram_header_ptr;

extern kal_uint8 const* NVRAM_EF_CUSTPACK_DEFAULT[];
extern void* custpack2ndJumpTbl[];
extern Type_Preamble_Content CustPackResPreamble;

/* CustPack image variables */
CUSTOM_IMAGE* nCustImageNamesEXT = NULL;
U16 CurrMaxImageNumEXT = 0; 
CUSTPACK_IMAGE_HEADER* pimage_header = NULL;
// wangbei add start 20060707
kal_bool DRV_POWERON(void)
{
    return KAL_TRUE;
}
// wangbei add end
#if 0

static void StandaloneResValidation(void)
{
   JUMP_TABLE_ENTRY_ENUM res_type;
   kal_uint32    res_type_code, res_MAGIC;
   kal_char       *project_ID; 
   kal_int8      errorType = -1; 
   kal_int8      errorResType = 0; 

   for (res_type=JUMP_TABLE_ENTRY_START; res_type <= JUMP_TABLE_ENTRY_END; res_type++)
   {
      project_ID  = (kal_char*) GET_Res_Addr(res_type, 0);     
      res_type_code     = *(kal_uint32*) GET_Res_Addr(res_type, RESOURCE_TYPE_OFFSET);
      res_MAGIC = *(kal_uint32*) GET_Res_Addr(res_type, MAGIC_NUM_OFFSET);

      /* Check consistency between JUMP TABLE and the RESOURCE BIN */
      if (strncmp(project_ID, (const char*) GET_Res_ProjectID(res_type), strlen(VERNO_STR))) 
      {
         errorType = 0;
         errorResType = res_type;
         goto Validation_Exit;
      }

      if (res_MAGIC != GET_Res_Magic(res_type))
        goto Validation_Exit;

      /* ID verification: PROJECT_ID */
      if (strncmp(project_ID, VERNO_STR, strlen(VERNO_STR)))
      {
         errorType = 0;
         errorResType = res_type;
         goto Validation_Exit;
      }
      
      /* ID verification: RESOURCE TYPE */
      if (((res_type_code & RESOURCE_TYPE_MASK) !=RESOURCE_TYPE_AUDIO) &&
      	    ((res_type_code & RESOURCE_TYPE_MASK) != RESOURCE_TYPE_IMAGE) &&  
 
      	    ((res_type_code & RESOURCE_TYPE_MASK) != RESOURCE_TYPE_LANG_PACK)   
 
#ifdef __J2ME__
            && ((res_type_code & RESOURCE_TYPE_MASK) != RESOURCE_TYPE_J2ME)        
#endif /* __J2ME__ */
				&& ((res_type_code & RESOURCE_TYPE_MASK) != RESOURCE_TYPE_CUSTPACK)  //__CUSTPACK_MULTIBIN__
        )
        goto Validation_Exit;
      
      /* MAGIC# verification */
      if (res_MAGIC != MAGIC_NUMBER)
        goto Validation_Exit;      

   }

   return;
   
Validation_Exit:
   {
      kal_uint8 dspl_text[PROJECTID_LEN];

      DRV_POWERON();  

      if (strlen(VERNO_STR) < PROJECTID_LEN)
      {
         kal_mem_set(dspl_text, '\0', PROJECTID_LEN);
         strncpy((char*) dspl_text, (const char *) project_ID, sizeof(VERNO_STR));
         dspl_text[sizeof(VERNO_STR)] = '\0';

         MMIResourceMismatchWarning(errorType, errorResType, (kal_uint8 const*) VERNO_STR, (kal_uint8 const*) dspl_text);
      }
      else  /* VERNO string too long (> 32 characters) */
      {
         ASSERT(KAL_FALSE);
      }
      
      while (1);
   }
}

/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef __J2ME__
/* under construction !*/
#endif /* __J2ME__ */
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
      #ifdef __MMI_THEMES_V2_SUPPORT__
/* under construction !*/
	#else
/* under construction !*/
	#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef DIGIT_TONE_SUPPORT
/* under construction !*/
#endif
#ifdef SIMPLE_TTS
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
#ifdef CUST_KEYPAD_TONE_SUPPORT
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined(__MMI_T9__)
#if defined(__MMI_T9_CHINESE__)
#if defined(__MMI_T9_V7__)
/* under construction !*/
#else
/* under construction !*/
#endif
#endif
#if defined(__MMI_T9_SM_CHINESE__)
#if defined(__MMI_T9_V7__)
/* under construction !*/
#else
/* under construction !*/
#endif
#endif
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
   #ifdef __J2ME__
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
   #endif /* __J2ME__ */
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef WAP_SUPPORT      
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif      
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
void cdfu_init(void);
#endif

void InitializeResourceVariables(void)
{
   void** langpack_JTbl  =  (void**)GET_ResLangPack_Addr(ENTRY_PREAMBLE); 
   void** custpack_header = (void**)GET_ResCUSTPACK_Addr(ENTRY_PREAMBLE);   //__CUSTPACK_MULTIBIN__
//dyj add start 20060523
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE

	/****init cut down flash ***/
	cdfu_init();
#endif

#if 1
      nCustImageNames = (CUSTOM_IMAGE*) (image2ndJumpTbl[0]);
      CurrMaxImageNum = *((unsigned short *) (image2ndJumpTbl[1]));

// wangbei del start 20060707
#if 0
     // gLanguageArray = (sLanguageDetails*) (langpack2ndJumpTbl[0]);
      //gMaxDeployedLangs = *((unsigned short *) (langpack2ndJumpTbl[1]));

     // gStringList = (StringResList*) (langpack2ndJumpTbl[2]);
#endif
// wangbei del end
// wangbei add start 20060707
     if (((kal_uint32) langpack_JTbl > 0x1) && 
   	 ((kal_uint32)  &LangPackResPreamble> 0x1)) /* dummy reference to make the variable exist */
   {
      gLanguageArray = (sLanguageDetails*) (langpack2ndJumpTbl[0]);
      gMaxDeployedLangs = *((unsigned short *) (langpack2ndJumpTbl[1]));

      gStringList = (StringResList*) (langpack2ndJumpTbl[2]);

	if (gIMEModule != *((sIMEModuleDetails*)(langpack2ndJumpTbl[6])))
   {
		ASSERT(0);
	}
	gIMEModeArray = (sIMEModeDetails*)(langpack2ndJumpTbl[3]);
	gIMEQSearchModeArray = (sIMEModeDetails*)(langpack2ndJumpTbl[4]);
	gIMELDBArray=(sIMELDBDetails*)(langpack2ndJumpTbl[5]);
#if defined(__MMI_T9__)
#if defined(__MMI_T9_TR_CHINESE__)
#if defined(__MMI_T9_V7__)
	gT9TRChineseLDB = *((T9ChineseData const T9FARDATA*)(langpack2ndJumpTbl[7]));
#else
	gT9TRChineseLDB = *((T9ChineseData const FARDATA*)(langpack2ndJumpTbl[7]));
#endif
#endif
#if defined(__MMI_T9_SM_CHINESE__)
#if defined(__MMI_T9_V7__)
	gT9SMChineseLDB = *((T9ChineseData const T9FARDATA*)(langpack2ndJumpTbl[8]));
#else
	gT9SMChineseLDB = *((T9ChineseData const FARDATA*)(langpack2ndJumpTbl[8]));
#endif
#endif
#endif
   }
     // wangbei add end
        if (((kal_uint32) custpack2ndJumpTbl > 0x1) && 
   	 ((kal_uint32)  &CustPackResPreamble> 0x1)) /* dummy reference to make the variable exist */
   {
		// custpack_nvram_header_ptr = (custpack_nvram_header *) (GET_ResCUSTPACK_Addr_Of(CUSTPACK_NVRAM));   //__CUSTPACK_MULTIBIN__
        custpack_nvram_header_ptr = (custpack_nvram_header *) custpack_header[CUSTPACK_NVRAM];

	 
   	 
  	/* CustPack Image Initialization */
      pimage_header = (CUSTPACK_IMAGE_HEADER*) (custpack_header[CUSTPACK_IMAGE]);
      nCustImageNamesEXT = (CUSTOM_IMAGE*)pimage_header->CustImageNamesEXT;
      CurrMaxImageNumEXT = pimage_header->MaxImageNumEXT;
   	 
   	 

      }
 
#if 1
/* IMAGE Initialization */
   {

      #ifdef __MMI_THEMES_V2_SUPPORT__
      		MMI_themes = ( theme_details_struct*) (image2ndJumpTbl[2]);
	#else
      MMI_themes = (MMI_theme**) (image2ndJumpTbl[2]);     
	#endif
      n_MMI_themes = *((S32*) (image2ndJumpTbl[3]));
   }
#endif 
 
//dyj add end
#endif

//dyj del start 20060523
#if 0
   /* IMAGE Initialization */
   {
      nCustImageNames = (CUSTOM_IMAGE*) (image2ndJumpTbl[0]);
      CurrMaxImageNum = *((unsigned short *) (image2ndJumpTbl[1]));
      #ifdef __MMI_THEMES_V2_SUPPORT__
      		MMI_themes = ( theme_details_struct*) (image2ndJumpTbl[2]);
	#else
      MMI_themes = (MMI_theme**) (image2ndJumpTbl[2]);     
	#endif
      n_MMI_themes = *((S32*) (image2ndJumpTbl[3]));
   }
#endif
//added by panxu on 20061222
// wangbei del start 20060707
 //resource_message_sounds = (audio_resource_struct*) CSD_resource_message_sounds;
#if 1
   /* AUDIO Initialization */
   {
#if 0//Jiashuo remove
      resource_tones = (device_tones_struct**) CSD_resource_tones;
#endif

#if 0//Jiashuo remove
#ifdef MIDI_IN_CUSTPACK_BIN
      resource_midis = (audio_resource_struct*) ((custpack_audio_header *) (custpack_header[CUSTPACK_AUDIO]))->CSD_resource_midis;
#else
      resource_midis = (audio_resource_struct*) CSD_resource_midis;
#endif
#endif

#ifdef SND_IN_CUSTPACK_BIN
      resource_sounds = (audio_resource_struct*) ((custpack_audio_header *) (custpack_header[CUSTPACK_AUDIO]))->CSD_resource_sounds;
#else
      resource_sounds = (audio_resource_struct*) CSD_resource_sounds;
#endif

#ifdef SND_IN_CUSTPACK_BIN
      resource_poweronoff_sounds = (audio_resource_struct*) ((custpack_audio_header *) (custpack_header[CUSTPACK_AUDIO]))->CSD_resource_poweronoff_sounds;
#else
      resource_poweronoff_sounds = (audio_resource_struct*) CSD_resource_poweronoff_sounds;
#endif

#if 0//Jiashuo remove
#ifdef EMS_SND_IN_CUSTPACK_BIN
      resource_ems_sounds = (audio_resource_struct*) ((custpack_audio_header *) (custpack_header[CUSTPACK_AUDIO]))->CSD_resource_ems_sounds;
#else
      resource_ems_sounds = (audio_resource_struct*) CSD_resource_ems_sounds;
#endif
#endif

#ifdef IMY_IN_CUSTPACK_BIN
      resource_imelodys = (audio_resource_struct*) ((custpack_audio_header *) (custpack_header[CUSTPACK_AUDIO]))->CSD_resource_imelodys;
#else
      resource_imelodys = (audio_resource_struct*) CSD_resource_imelodys;
#endif

#if 0//Jiashuo remove
#ifdef EMS_IMY_IN_CUSTPACK_BIN
      resource_ems_imelodys = (audio_resource_struct*) ((custpack_audio_header *) (custpack_header[CUSTPACK_AUDIO]))->CSD_resource_ems_imelodys;
#else
      resource_ems_imelodys = (audio_resource_struct*) CSD_resource_ems_imelodys;
#endif
#endif

#ifdef MSG_SND_IN_CUSTPACK_BIN
      resource_message_sounds = (audio_resource_struct*) ((custpack_audio_header *) (custpack_header[CUSTPACK_AUDIO]))->CSD_resource_message_sounds;
#else
      resource_message_sounds = (audio_resource_struct*) CSD_resource_message_sounds;
#endif

#if 0//Jiashuo remove
#ifdef MMS_SND_IN_CUSTPACK_BIN
      resource_mms_sounds = (audio_resource_struct*) ((custpack_audio_header *) (custpack_header[CUSTPACK_AUDIO]))->CSD_resource_mms_sounds;
#else
      resource_mms_sounds = (audio_resource_struct*) CSD_resource_mms_sounds;
#endif
#endif
#if defined(__MMI_GAME__)

#ifdef FNG_SND_IN_CUSTPACK_BIN
	  resource_fng_sounds = (audio_resource_struct*)  ((custpack_audio_header *) (custpack_header[CUSTPACK_AUDIO]))->CSD_resource_fng_sounds;
#else
	  resource_fng_sounds = (audio_resource_struct*) CSD_resource_fng_sounds;
#endif
#endif
/*Jinzh Add Start For 5754 Ver: TBM780  on 2007-8-8 16:24 */
#ifdef CAMERA_MODULE
#ifdef CAMERA_SND_IN_CUSTPACK_BIN
	  resource_camera_sounds = (audio_resource_struct*)  ((custpack_audio_header *) (custpack_header[CUSTPACK_AUDIO]))->CSD_resource_camera_sounds;
#else
	  resource_camera_sounds = (audio_resource_struct*) CSD_resource_camera_sounds;
#endif
#endif
/*Jinzh Add End  For 5754 Ver: TBM780  on 2007-8-8 16:24 */

#ifdef DIGIT_TONE_SUPPORT
#ifdef HUMAN_VOICE_IN_CUSTPACK_BIN
      resource_human_voice_tones = (audio_resource_struct*) ((custpack_audio_header *) (custpack_header[CUSTPACK_AUDIO]))->CSD_resource_human_voice_tones;
#else
      resource_human_voice_tones = (audio_resource_struct*) CSD_resource_human_voice_tones;
#endif
#endif

#ifdef SIMPLE_TTS
      resource_tts_sounds = (audio_resource_struct*) CSD_resource_tts_sounds;
      TTS_UCS2_MSB_index = (start_end_t*) CSD_TTS_UCS2_MSB_index;
      TTS_UCS2_LSB = (unsigned char*) CSD_TTS_UCS2_LSB;
      TTS_index_table = (unsigned short*) CSD_TTS_index_table;
#endif

#ifdef CUST_KEYPAD_TONE_SUPPORT
#ifdef CUST_KEYPAD_TONE_IN_CUSTPACK_BIN
      resource_keypad_tones = (audio_resource_struct*) ((custpack_audio_header *) (custpack_header[CUSTPACK_AUDIO]))->CSD_resource_keypad_tones;
#else
      resource_keypad_tones = (audio_resource_struct*) CSD_resource_keypad_tones;
#endif
#endif
   }


#ifdef __J2ME__
#if 0//JiaShuo Delete for J2ME
   /* For backward compatiable old project without j2me feature */
   /* J2ME Initialization */
   {
      j2me_custom_file_info_ptr = (j2me_custom_file_info_struct*)(J2ME2ndJumpTbl[0]);
	  if (j2me_custom_file_info_ptr) {
		  j2me_default_game_count = j2me_custom_file_info_ptr->default_mids_count;
		  virtual_file_count = j2me_custom_file_info_ptr->virtual_file_info.virtual_file_count;
	  }
   }
#endif
#endif /* __J2ME__ */
#endif

#if 0
// wangbei del end
 
   if (((kal_uint32) langpack_JTbl > 0x1) && 
   	 ((kal_uint32)  &LangPackResPreamble> 0x1)) /* dummy reference to make the variable exist */
   {
      gLanguageArray = (sLanguageDetails*) (langpack2ndJumpTbl[0]);
      gMaxDeployedLangs = *((unsigned short *) (langpack2ndJumpTbl[1]));

      gStringList = (StringResList*) (langpack2ndJumpTbl[2]);

	if (gIMEModule != *((sIMEModuleDetails*)(langpack2ndJumpTbl[6])))
   {
		ASSERT(0);
	}
	gIMEModeArray = (sIMEModeDetails*)(langpack2ndJumpTbl[3]);
	gIMEQSearchModeArray = (sIMEModeDetails*)(langpack2ndJumpTbl[4]);
	gIMELDBArray=(sIMELDBDetails*)(langpack2ndJumpTbl[5]);
#if defined(__MMI_T9__)
#if defined(__MMI_T9_TR_CHINESE__)
#if defined(__MMI_T9_V7__)
	gT9TRChineseLDB = *((T9ChineseData const T9FARDATA*)(langpack2ndJumpTbl[7]));
#else
	gT9TRChineseLDB = *((T9ChineseData const FARDATA*)(langpack2ndJumpTbl[7]));
#endif
#endif
#if defined(__MMI_T9_SM_CHINESE__)
#if defined(__MMI_T9_V7__)
	gT9SMChineseLDB = *((T9ChineseData const T9FARDATA*)(langpack2ndJumpTbl[8]));
#else
	gT9SMChineseLDB = *((T9ChineseData const FARDATA*)(langpack2ndJumpTbl[8]));
#endif
#endif
#endif
   }
 

   /* CustPack Initialization */ //__CUSTPACK_MULTIBIN__
   if (((kal_uint32) custpack2ndJumpTbl > 0x1) && 
   	 ((kal_uint32)  &CustPackResPreamble> 0x1)) /* dummy reference to make the variable exist */
   {
   	kal_uint8 i;
		// custpack_nvram_header_ptr = (custpack_nvram_header *) (GET_ResCUSTPACK_Addr_Of(CUSTPACK_NVRAM));   //__CUSTPACK_MULTIBIN__
        custpack_nvram_header_ptr = (custpack_nvram_header *) custpack_header[CUSTPACK_NVRAM];
   	NVRAM_EF_CUSTPACK_DEFAULT[0] = custpack_nvram_header_ptr->version;   

   	for(i=1;i<(NVRAM_EF_LAST_LID_CUSTPACK-NVRAM_EF_CUSTPACK_BASE);i++)     
			NVRAM_EF_CUSTPACK_DEFAULT[i] = custpack_nvram_header_ptr->custpack_default[i-1];  

	 
   	 
  	/* CustPack Image Initialization */
      pimage_header = (CUSTPACK_IMAGE_HEADER*) (custpack_header[CUSTPACK_IMAGE]);
      nCustImageNamesEXT = (CUSTOM_IMAGE*)pimage_header->CustImageNamesEXT;
      CurrMaxImageNumEXT = pimage_header->MaxImageNumEXT;
   	 
   	 

#ifdef WAP_SUPPORT      
      // __CUSTPACK_MULTIBIN     Johnnie   BEGIN
      {
         wap_custpack_header *custpack_wap_header_ptr = (wap_custpack_header *) custpack_header[CUSTPACK_WAP];
         custpack_wap_default.wap_profile_default = custpack_wap_header_ptr->wap_profile_default;
         custpack_wap_default.mms_profile_default = custpack_wap_header_ptr->mms_profile_default;
         custpack_wap_default.wap_bkm_default = custpack_wap_header_ptr->wap_bkm_default;
         custpack_wap_default.wap_root_ca_default = custpack_wap_header_ptr->wap_root_ca_default;
         custpack_wap_default.wap_config_default = custpack_wap_header_ptr->wap_config_default;
         custpack_wap_default.mms_config_default = custpack_wap_header_ptr->mms_config_default;
      }         
      // __CUSTPACK_MULTIBIN     Johnnie   END
#endif      
   }

  //add by panxu 20061222
 //  StandaloneResValidation();   
   #endif
}

#endif



