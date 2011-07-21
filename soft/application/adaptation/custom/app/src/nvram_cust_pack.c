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
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2002
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 * nvram_cust_pack.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifdef MMI_ON_HARDWARE_P
#include "kal_release.h"

#include "nvram_defs.h"
#include "nvram_user_defs.h"
#include "custom_nvram_editor_data_item.h"
#include "custom_mmi_default_value.h"
#include "mmi_data_types.h"
/*
 * Warning! Never Change the order of everything in this file
 */
//////////Changes for the Greeting Text//////////////////
const kal_uint8 NVRAM_SETTING_DEFAULT[NVRAM_SETTING_SIZE] = 
{
	'W' ,0x00,'e' ,0x00,'l' ,0x00,'c' ,0x00,'o' ,0x00,'m' ,0x00,'e' ,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

/*For Cache file*/
const kal_uint8 NVRAM_CACHE_BYTE_DEFAULT[NVRAM_CACHE_SIZE]=
{
	/*
	NVRAM_PHONELOCK_INDEX, 				//0
	NVRAM_AUTOLOCK_TIMEOUT_INDEX,
	NVRAM_SETTING_PHONELOCK,
	NVRAM_SETTING_AUTOKEYPADLOCK,
	NVRAM_SETTING_AUTOREDIAL,
	NVRAM_SETTING_AUTOKEYPADLOCK_TIME,		//5
	NVRAM_SETTING_WELCOME_TEXT,
	NVRAM_SETTING_APHORISM_TEXT,
	NVRAM_SETTING_CALL_TIME_DISPLAY,
	NVRAM_SETTING_SPEED_DIAL,
	NVRAM_SETTING_CONTRAST_LEVEL,			//10
	NVRAM_SETTING_AUTOUPDATE_DT_TIME,
	NVRAM_SETTING_OWNER_NO,
	NVRAM_SETTING_STATUS_DT_TIME,
	NVRAM_SETTING_LANG,				//14 0x00 ENG 0x01 TC 0x02 SC
	NVRAM_SETTING_CTR,				//15 call time remaind
	NVRAM_SETTING_CONTRAST_SUBLCD_LEVEL,
	NVRAM_HELP_ENABLED_FLAG,
	NVRAM_PROFILES_ACTIVATED,
	NVRAM_MISSED_CALL_NUM,
	NVRAM_SMSCOUNTERS_SENT,				//20
	NVRAM_SMSCOUNTERS_RECD,
        NVRAM_DOWNLOAD_IMAGE,
	NVRAM_DOWNLOAD_TONE,
	CURRENT_CITY,					//24 0x07 BEJ
	TIME_FORMAT,					//25 0x00 12h 0x01 24h
	DATE_FORMAT,					//26 0x00 DD-MMM-YYYY
							     0x01 DD/MM/YYYY
							     0x02 MM/DD/YYYY
							     0x03 YYYY/MM/DD
							     0x04 YYYY-MM-DD
							     0x05 MMM DD,YYYY
	NVRAM_GAME_SNAKE_LEVEL,
	LUNAR_CALENDAR,
	NVRAM_DOWNLOAD_CITY1,
	NVRAM_DOWNLOAD_CITY2,				//30
	NVRAM_SCREENSAVER_STATUS,			// 0 for Off, 1 for On
	NVRAM_SCREENSAVER_WAITING,			// 0 for 5 Sec, 1 for 15 Sec, 2 for 30 Sec, 3 for 1 Minute
	NVRAM_CALLERID,						//0: hide, 1:send, 2:auto
	NVRAM_GROUPINDEX,
        NVRAM_GAME_RICHES_STATUS,			//35
	NVRAM_RICHES_OPTION_STATUS,
	NVRAM_PROFILES_LASTACTIVATED,
	TOH_CURRENT_LEVEL,
	SMASH_CURRENT_LEVEL,
	NVRAM_SMS_ACTIVE_PROFILE_INDEX,			//40
	SMS_CONCAT_MSG_REF_VALUE,
	NVRAM_COLORBALLS_OPTION_STATUS,
        NVRAM_PROFILES_LOUDSPKR,
        
	RESTORE_DEFAULT_SPEED_DIAL,//44
	RESTORE_DEFAULT_CONTRAST_SUBLCD_LEVEL,		//45
	RESTORE_DEFAULT_CONTRAST_LEVEL,
	RESTORE_DEFAULT_WELCOME_TEXT,
	RESTORE_DEFAULT_APHORISM_TEXT,
	RESTORE_DEFAULT_OWNER_NO,
	RESTORE_DEFAULT_STATUS_SHOW_DT_TIME,		//50
	RESTORE_DEFAULT_AUTOUPDATE_TIME,
	RESTORE_DEFAULT_SCREENSAVER_STATUS,
	RESTORE_DEFAULT_SCREENSAVER_WAITING,	
	RESTORE_DEFAULT_PROFILES_ACTIVATED,
	RESTORE_DEFAULT_TIME_FORMAT,			//55
	RESTORE_DEFAULT_DATE_FORMAT,
        NVRAM_DOWNLOAD_MELODY,
	NVRAM_NORMAL_MODE_VOICE_LEVEL,
	NVRAM_LDSPK_MODE_VOICE_LEVEL,
	NVRAM_HDSET_MODE_VOICE_LEVEL,			//60
	NVRAM_NITZTIMEZONE,
	NVRAM_WALLPAPER_ROTATE_STATUS,
	NVRAM_IP_NUMBER_SETTING
	*/

  0xFF,0xFF,0x00,0x00,0x00,	// 0~4  
#if defined(__PROJECT_GALLITE_C01__) 
  0x02,0x00,0x01,0x01,0x00,	// 5~9  
  0x07,0x00,0x00,0x01,0x01,	// 10~14  //cs_xxx chinese = 0x01
  0x00,0x07,0xFF,0x00,0xFF,	// 15~19
#else
  0x00,0x00,0x01,0x01,0x00,	// 5~9  
  0x07,0x00,0x00,0x01,0x00,	// 10~14  //cs_xxx chinese = 0x01
  0x02,0x07,0xFF,0x00,0xFF,	// 15~19
#endif
  0xFF,0xFF,0x00,0x00,0x07,	// 20~24
  0x01,0x05,0xFF,0xFF,0x01,	// 25~29
  0x00,0x00,0x1E,0x01,0xFF,	// 30~34
  0xFF,0xFF,0xFF,0xFF,0xFF,	// 35~39
  0xFF,0xFF,0xFF,0xFF,0xFF,     	// 40~44
  0xFF,0xFF,
  
  FACTORY_RESTORE_DEFAULT_SPEED_DIAL,				
  FACTORY_RESTORE_DEFAULT_CONTRAST_SUBLCD_LEVEL_VALUE,         
  FACTORY_RESTORE_DEFAULT_CONTRAST_MAINLCD_LEVEL_VALUE,         
  FACTORY_RESTORE_DEFAULT_WELCOME_TEXT_STATUS,                 
  FACTORY_RESTORE_DEFAULT_APHORISM_TEXT_STATUS,              
  FACTORY_RESTORE_DEFAULT_OWNER_NO_STATUS,                     
  FACTORY_RESTORE_DEFAULT_STATUS_SHOW_DT_TIME_STATUS,         
  FACTORY_RESTORE_DEFAULT_AUTOUPDATE_TIME_VALUE,               
  FACTORY_RESTORE_DEFAULT_SCREENSAVER_STATUS,                  
  FACTORY_RESTORE_DEFAULT_SCREENSAVER_WAITING_TIME,             
  FACTORY_RESTORE_DEFAULT_PROFILES_ACTIVATED_ID,              
                                                                        
#if defined(__PROJECT_GALLITE_C01__) 
  0x00,0x00,0x00,7,7,7,                         
#else
  0x00,0x00,0x00,LEVEL4,LEVEL4,LEVEL4,                         
#endif
  0x00,0x00,0xFF,0xFF,
  0x00,
  FACTORY_RESTORE_DEFAULT_FMRDO_BACKGROUND_PLAY,
  FACTORY_RESTORE_DEFAULT_FMRDO_SKIN,
  FACTORY_RESTORE_DEFAULT_FMRDO_VOLUME,
  FACTORY_RESTORE_DEFAULT_AUDPLY_LIST_AUTO_GEN,
  FACTORY_RESTORE_DEFAULT_AUDPLY_VOLUME,
  FACTORY_RESTORE_DEFAULT_AUDPLY_PREFER_LIST,
  FACTORY_RESTORE_DEFAULT_AUDPLY_SKIN,
  FACTORY_RESTORE_DEFAULT_AUDPLY_REPEAT,
  FACTORY_RESTORE_DEFAULT_AUDPLY_SHUFFLE,
  FACTORY_RESTORE_DEFAULT_AUDPLY_BACKGROUND_PLAY,
  FACTORY_RESTORE_DEFAULT_SNDREC_STORAGE,
  FACTORY_RESTORE_DEFAULT_SNDREC_FORMAT,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
#if defined(__PROJECT_GALLITE_C01__) 
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,
#else
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
#endif
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	  
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

const kal_uint8 NVRAM_CACHE_SHORT_DEFAULT[NVRAM_CACHE_SIZE]=
{
  	/*
	NVRAM_SHORTDATA1_INDEX,				// 0
	NVRAM_SHORTDATA2_INDEX,
	NVRAM_SNAKE_LEVEL1_SCORE,
	NVRAM_SNAKE_LEVEL2_SCORE,
	NVRAM_SNAKE_LEVEL3_SCORE,
	NVRAM_SNAKE_LEVEL4_SCORE,			// 5
	NVRAM_F1RACE_SCORE,				// 6
	NVRAM_CURRENT_SCREENSVER_ID,
	NVRAM_PHB_STORAGE_LOCATION,
	NVRAM_FUNANDGAMES_SETWALLPAPER,
	NVRAM_GAME_DOLL_GRADE,				// 10
	TOH_LEVEL1_HIGHEST_SCORE,			// 11
	TOH_LEVEL2_HIGHEST_SCORE,
	TOH_LEVEL3_HIGHEST_SCORE,
	TOH_LEVEL4_HIGHEST_SCORE,
	NVRAM_RICHES_EASY_SCORE,			// 15
	NVRAM_RICHES_NORMAL_SCORE,			// 16
	NVRAM_RICHES_HARD_SCORE,
	NVRAM_COLORBALLS_EASY_SCORE,
	NVRAM_COLORBALLS_NORMAL_SCORE,
	NVRAM_COLORBALLS_HARD_SCORE,			// 20
	NVRAM_SMASH_NORMAL_SCORE,			// 21
	NVRAM_SMASH_SPEEDY_SCORE,
	NVRAM_SMASH_ACCURACY_SCORE,
	NVRAM_SETTING_CTR_TIME,
		 
	NVRAM_SETTING_PREFER_INPUT_METHOD,		// 25
        	//CSD end
        RESTORE_DEFAULT_CURRENT_SCREENSVER_ID,		// 26
	RESTORE_DEFAULT_FUNANDGAMES_SETWALLPAPER,	// 27
        	 
	RESTORE_PREFER_INPUT_METHOD			// 28
		//CSD end
	*/

  
/*0~4*/	0xFF,0xFF, 0xFF,0xFF, 0x00,0x00, 0x00,0x00, 0x00,0x00,
/*5~9*/	0x00,0x00, 0x00,0x00, 0x91,0x33, 0x01,0x00, (kal_uint8)(DEFAULT_FUNANDGAMES_SETWALLPAPER_ID  % 256),
  (kal_uint8)(DEFAULT_FUNANDGAMES_SETWALLPAPER_ID >> 8),
  0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,  // 10~14
  0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,  // 15~19
  0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x32,0x00,  // 20~24
  0xFF,0x00,									// 25
  (kal_uint8)(FACTORY_RESTORE_DEFAULT_CURRENT_SCREENSVER_ID % 256),		// 26
  (kal_uint8)(FACTORY_RESTORE_DEFAULT_CURRENT_SCREENSVER_ID >> 8),
  (kal_uint8)(FACTORY_RESTORE_DEFAULT_FUNANDGAMES_SETWALLPAPER_ID  % 256),        // 27
  (kal_uint8)(FACTORY_RESTORE_DEFAULT_FUNANDGAMES_SETWALLPAPER_ID >> 8),
  0xFF,0x00/*28*/,
  0xFF,0xFF,0xFF,0xFF/*30*/,
  0xFF,0xFF/*31*/,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF/*39*/,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF/*47*/,
  0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  (kal_uint8)(DEFAULT_SETWALLPAPER_SUB_ID  % 256),
  (kal_uint8)(DEFAULT_SETWALLPAPER_SUB_ID >> 8), /*59*/
  (kal_uint8)(FACTORY_RESTORE_DEFAULT_SETWALLPAPER_SUB_ID % 256),
  (kal_uint8)(FACTORY_RESTORE_DEFAULT_SETWALLPAPER_SUB_ID>> 8), /*60*/
  0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	  
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

const kal_uint8 NVRAM_CACHE_DOUBLE_DEFAULT[NVRAM_CACHE_SIZE]=
{
//zough add   20061130 begin
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//zough add   20061130 end  
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	  
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};




const kal_uint8 NVRAM_PHONEBOOK_SPEEDDIAL_DEFAULT [NVRAM_PHONEBOOK_SPEEDDIAL_RECORD_SIZE];
#if defined(CSD_NO_SLIM_NVRAM)	                   

const kal_uint8  NVRAM_EF_SMSAL_MAILBOX_ADDR_DEFAULT[] = {
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* Alpha ID */
                  0x00,                            /* Length of BCD number */
                  0xff,                            /* TON and NPI */
                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* Dailling number */
                  0xff,                            /* capability/configuration identifier */
                  0xff                             /* extension 1 record identifier */
                  };
#endif
#if defined(CSD_NO_SLIM_NVRAM)	                   

const kal_uint8 NVRAM_EF_MS_SECURITY_DEFAULT[] = {
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,/* reg_ps_key */
                  0x00,/* security_indication */ 
                  0x00,/* auto_lock_item */ 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,/* np_code */ 
                  0x21, 0x43, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,/* np_key */  
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,/* nsp_code */ 
                  0x65, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,/* nsp_key */  
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00,/* np_of_sp */
                  0x00, 0x00, 0x00,/* np_of_cp */
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,/* gid1 */ 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,/* gid2 */ 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,/* sp_key */ 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,/* cp_key */ 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,/* imsi_code */ 
                  0x33, 0x44, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,/* imsi_key */ 
                  0x21, 0x43, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,/* phone_key */ 
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* last_imsi */
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* pin1 */
                  0x00, /* pin1_valid */
                  0x00  /* phone_lock_verified */
                  };
#endif
#if defined(CSD_NO_SLIM_NVRAM)	                   

const kal_uint8 NVRAM_EF_ALS_LINE_ID_DEFAULT[NVRAM_EF_ALS_LINE_ID_SIZE] = 
{
	0x0 , 0x0
};
const kal_uint8 NVRAM_EF_MSCAP_DEFAULT[NVRAM_EF_MSCAP_SIZE] = 
{
#if defined(__AMR_SUPPORT__)
	0x1f,       /* Supported speech version (FR,HR,EFR,AMR FR,AMR HR) */
#elif !defined(MT6205)
	0x07,       /* Supported speech version (FR,HR,EFR) */
#else   
	0x05,       /* Supported speech version (FR,EFR) */
#endif                  

#if defined(__CSD_T__) && defined(__CSD_NT__) && defined(__CSD_FAX__)
	0x63,       /* Supported data capability 1 (T & NT & FAX_T) */                  
#elif defined(__CSD_T__) && defined(__CSD_NT__)
	0x61,       /* Supported data capability 1 (T & NT) */
#elif defined(__CSD_T__) && defined(__CSD_FAX__)
	0x23,       /* Supported data capability 1 (T & FAX_T)*/
#elif defined(__CSD_NT__) && defined(__CSD_FAX__)                 
	0x43,       /* Supported data capability 1 (NT & FAX_T)*/
#elif defined(__CSD_T__)
	0x21,       /* Supported data capability 1 (ONLY T)*/
#elif defined(__CSD_NT__)                  
	0x41,       /* Supported data capability 1 (ONLY NT)*/
#elif defined(__CSD_FAX__)                  
	0x02,       /* Supported data capability 1 (ONLY FAX_T)*/
#else
	0x00,       /* Supported data capability 1 (NO SUPPORT CSD)*/
#endif
	0x00,       /* Supported data capability 2 */
#if defined(CSD_SUPPORT) || defined(__CSD_FAX__)
#if !defined(MT6205)                  
	0x1f        /* Supported channel coding (2.4k,4.8k,9.6k,14.4k,HR)*/
#else
	0x0f        /* Supported channel coding (2.4k,4.8k,9.6k,14.4k)*/
#endif                  
#else
	0x00
#endif                  
};

const kal_uint8  NVRAM_EF_RAC_PREFERENCE_DEFAULT[]=
	{
	    /* Ripple: 20050513, Auto band, then turn on all supported bands. */
#if defined( __EGSM900__ ) && defined( __DCS1800__ ) && defined( __GSM850__ ) && defined( __PCS1900__ ) 
                  0x9A,
#elif defined( __EGSM900__ ) && defined( __DCS1800__ ) && defined( __GSM850__ )
                  0x8A,
#elif defined( __EGSM900__ ) && defined( __DCS1800__ ) && defined( __PCS1900__ ) 
                  0x1A,
#elif defined( __GSM850__ ) && defined( __PCS1900__ ) && defined( __EGSM900__ )
                  0x92,
#elif defined( __GSM850__ ) && defined( __PCS1900__ ) && defined( __DCS1800__ )
                  0x98,                  
#elif defined( __EGSM900__ ) && defined( __DCS1800__ )
                  0x0A,      /* prefered band: 900 + 1800 */
#elif defined( __GSM850__ ) && defined( __PCS1900__ )   
                  0x90,      /* prefered band: 850 + 1900 */
#elif defined( __GSM850__ ) && defined( __DCS1800__ )   
                  0x88,      /* prefered band: 850 + 1800 */
#elif defined( __EGSM900__ ) && defined( __PCS1900__ )   
                  0x12,      /* prefered band: 900 + 1900 */
#elif defined( __EGSM900__ )  
                  0x02,      /* prefered band: 900 */
#elif defined( __DCS1800__ )       
                  0x08,      /* prefered band: 1800 */
#elif defined( __GSM850__ )  
                  0x80,      /* prefered band: 850 */
#elif defined( __PCS1900__ )  
                  0x10,      /* prefered band: 1900 */                
#else
                  0x0A,      /* prefered band: 900 + 1800 */
#endif          
                  //0x80                            /* PLMN selection mode and Preferred Service in NMO III */
                  0xC0                            /* GPRS auto attach */
                  };


static kal_uint8 const NVRAM_EF_SMSAL_COMMON_PARAM_DEFAULT[] = {
#if defined (__GSM_MODE__) && defined (__GPRS_MODE__)
                  0x03,  /* bearer service : GSM prefer */
#elif defined (__GPRS_MODE__)
                  0x00,  /* bearer service : GPRS Only */
#else                  
                  0x01,  /* bearer service : GSM Only */
#endif
                  0x00, 0x00,       /* status report, reply path OFF */
                  0x02,             /* VP: Relative format */
                  0x07, 0x05, 0x07, /* mem1, mem2, mem3 */
                  0x00, 0x01,       /* CB setting */
                  0x00,             /* AT default profile ID */
                  0x00,             /* First Octet */
                  0x00              /* Turn Off SMS FDN */
                  };

/* Cell Broadcast Default Channel Setting. */
/* The channels set will not shown in MMI, but it will received CB if CB is turned on. */
/* This setting may be changed by customer. DON'T remove it. */
static kal_uint8 const NVRAM_EF_CB_DEFAULT_CH_DEFAULT[] = {
                  0xFF,0xFF,     /* default CH 1 , for example:  chanel 50 is 0x00, 0x32 */
                  0xFF,0xFF,     /* default CH 2  */
                  0xFF,0xFF,     /* default CH 3  */
                  0xFF,0xFF,     /* default CH 4  */
                  0xFF,0xFF,     /* default CH 5  */
                  0xFF,0xFF,     /* default CH 6  */
                  0xFF,0xFF,     /* default CH 7  */
                  0xFF,0xFF,     /* default CH 8  */
                  0xFF,0xFF,     /* default CH 9  */
                  0xFF,0xFF      /* default CH 10 */
};
#endif


 
kal_uint8 const* NVRAM_EF_CUSTPACK_DEFAULT[NVRAM_EF_LAST_LID_CUSTPACK-NVRAM_EF_CUSTPACK_BASE];
 

#if 0
const custpack_nvram_header custpack_nvram_ptr=
{
	0,//version
	{//default value arraies
		NVRAM_SETTING_DEFAULT,
		NVRAM_CACHE_BYTE_DEFAULT,
		NVRAM_CACHE_SHORT_DEFAULT,
		NVRAM_CACHE_DOUBLE_DEFAULT,
		NVRAM_PHONEBOOK_SOS_DEFAULT,
		NVRAM_PHONEBOOK_SPEEDDIAL_DEFAULT ,
		NVRAM_EF_SMSAL_MAILBOX_ADDR_DEFAULT,
		NVRAM_EF_MS_SECURITY_DEFAULT,
		NVRAM_EF_ALS_LINE_ID_DEFAULT,
		NVRAM_EF_MSCAP_DEFAULT,
		NVRAM_EF_RAC_PREFERENCE_DEFAULT,
      NVRAM_EF_SMSAL_COMMON_PARAM_DEFAULT,
      NVRAM_EF_CB_DEFAULT_CH_DEFAULT
	}
};
#endif
#endif// MMI_ON_HARDWARE_P

