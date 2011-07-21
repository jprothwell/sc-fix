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
 * SSCStringHandle.h
 *
 * Project:
 * --------
 *   PlutoMMI
 *
 * Description:
 * ------------
 *   To handle the ssc string.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef _SSCSTRINGHANDLE_H
#define _SSCSTRINGHANDLE_H

//micha1217
#include "mmi_features.h"

extern void SSCStringParsing1(void);
 
extern BOOL SSCStringParsing2(void);
extern BOOL SSCStringParsing3(void);

#define MAX_IMEI_LEN	15
#define MAX_SW_VER_LEN	 30
#define MAX_SSC_STRING  30 * ENCODING_LENGTH// it means the maximun can not be more than 60/2(UCS2) word

#define SSC_POP_UP_TIME		UI_POPUP_NOTIFYDURATION_TIME_2000

//micha1129
#define SSC_AUTO "*#0000#"
#define SSC_SCHINESE	"*#0086#"
#define SSC_TCHINESE	"*#0886#"
#define SSC_ENGLISH		"*#0044#"
#define SSC_DEFAULT		"*#0044#"
 
#define SSC_SPANISH	"*#0034#"
 
#define SSC_DANISH   "*#0045#"
 
#define SSC_THAI	"*#0066#"
#define SSC_FRENCH		"*#0033#"
#define SSC_GERMAN	"*#0049#"
#define SSC_ITALIAN	"*#0039#"
#define SSC_RUSSIAN	"*#0007#"
#define SSC_PORTUGUESE	"*#0351#"
#define SSC_TURKISH	"*#0090#"
#define SSC_VIETNAMESE		"*#0084#"
#define SSC_INDONESIAN	"*#0062#"
#define SSC_MALAY	"*#0060#"
#define SSC_MYANMAR "*#0095#"
#define SSC_TAMIL "*#9144#"
/*CSD end*/
#define SSC_ARABIC "*#0966#"
#define SSC_PERSIAN "*#0098#"
#define SSC_HINDI	"*#0091#"
#define SSC_TELUGU "*#9140#"
#define SSC_MARATHI  "*#9122#"
#define SSC_URDU "*#0092#"

#define MEM_CTRL_BUF_DBG    "*035670766*001*"
#define MEM_CTRL_BUF_QRY    "*#035670766*001#"

#define TASK_CTRL_BUF_DBG    "*035670766*002*"
#define TASK_CTRL_BUF_QRY    "*#035670766*002#"

enum STR_SSC_STRING_HANDLIE
{
	SSC_SCR_LANGUAGE_ENGLISH_STR_ID		=		IDLE_APP_BASE+300,
	SSC_SCR_LANGUAGE_SCHINESE_STR_ID,
	SSC_SCR_LANGUAGE_TCHINESE_STR_ID,
	SSC_SCR_LANGUAGE_DEFAULT_STR_ID,
 
#if defined(__MMI_LANG_SPANISH__)
	SSC_SCR_LANGUAGE_SPANISH_STR_ID,
#endif
 
#if defined(__MMI_LANG_DANISH__)
	SSC_SCR_LANGUAGE_DANISH_STR_ID,
#endif
 
#if defined(__MMI_LANG_FRENCH__)
	SSC_SCR_LANGUAGE_FRENCH_STR_ID,
#endif
#if defined(__MMI_LANG_GERMAN__)
	SSC_SCR_LANGUAGE_GERMAN_STR_ID,
#endif
#if defined(__MMI_LANG_ITALIAN__)
	SSC_SCR_LANGUAGE_ITALIAN_STR_ID,
#endif
#if defined(__MMI_LANG_THAI__)
	SSC_SCR_LANGUAGE_THAI_STR_ID,
#endif

#if defined(__MMI_LANG_RUSSIAN__)
	SSC_SCR_LANGUAGE_RUSSIAN_STR_ID,
#endif
#if defined(__MMI_LANG_PORTUGUESE__)
	SSC_SCR_LANGUAGE_PORTUGUESE_STR_ID,
#endif
#if defined(__MMI_LANG_TURKISH__)
	SSC_SCR_LANGUAGE_TURKISH_STR_ID,
#endif
#if defined(__MMI_LANG_VIETNAMESE__)
	SSC_SCR_LANGUAGE_VIETNAMESE_STR_ID,
#endif
#if defined(__MMI_LANG_INDONESIAN__)
	SSC_SCR_LANGUAGE_INDONESIAN_STR_ID,
#endif
#if defined(__MMI_LANG_MALAY__)
	SSC_SCR_LANGUAGE_MALAY_STR_ID,
#endif
/*CSD end*/

      SSC_SCR_LANGUAGE_ARABIC_STR_ID,
      SSC_SCR_LANGUAGE_PERSIAN_STR_ID,
	SSC_SCR_LANGUAGE_HINDI_STR_ID,
	SSC_SCR_LANGUAGE_MARATHI_STR_ID,

	SSC_SCR_ENGINEER_MODE_STR_ID,
	SSC_SCR_IMEI_CAPTION_ID,
	SSC_SCR_IMEI_STR_ID,
	SSC_SCR_SW_VERSION_CAPTION_ID,
//micha1111, #219
       SSC_SCR_HW_VERSION_CAPTION_ID,
	SSC_SCR_SERIAL_NUMBER_CAPTION_ID,
	SSC_STR_LANGUAGE_ENGLISH,
	SSC_STR_LANGUAGE_SCHINESE,
	SSC_STR_LANGUAGE_TCHINESE,
 
#if defined(__MMI_LANG_SPANISH__)
	SSC_STR_LANGUAGE_SPANISH,
#endif
 
#if defined(__MMI_LANG_DANISH__)
	SSC_STR_LANGUAGE_DANISH,
#endif
 
#if defined(__MMI_LANG_FRENCH__)
	SSC_STR_LANGUAGE_FRENCH,
#endif
#if defined(__MMI_LANG_GERMAN__)
	SSC_STR_LANGUAGE_GERMAN,
#endif
#if defined(__MMI_LANG_ITALIAN__)
	SSC_STR_LANGUAGE_ITALIAN,
#endif
#if defined(__MMI_LANG_THAI__)
	SSC_STR_LANGUAGE_THAI,
#endif
#if defined(__MMI_LANG_RUSSIAN__)
	SSC_STR_LANGUAGE_RUSSIAN,
#endif
#if defined(__MMI_LANG_PORTUGUESE__)
	SSC_STR_LANGUAGE_PORTUGUESE,
#endif
#if defined(__MMI_LANG_TURKISH__)
	SSC_STR_LANGUAGE_TURKISH,
#endif
#if defined(__MMI_LANG_VIETNAMESE__)
	SSC_STR_LANGUAGE_VIETNAMESE,
#endif
#if defined(__MMI_LANG_INDONESIAN__)
	SSC_STR_LANGUAGE_INDONESIAN,
#endif
#if defined(__MMI_LANG_MALAY__)
	SSC_STR_LANGUAGE_MALAY,
#endif
/*CSD end*/
         
        SSC_STR_LANGUAGE_ARABIC,
        //CSD end
        SSC_STR_LANGUAGE_PERSIAN,
	SSC_STR_ERR_SET_DEFAULT,
	SSC_STR_ERR_SET_ENGLISH,
	SSC_STR_ERR_SET_SCHINESE,
	SSC_STR_ERR_SET_TCHINESE,
 
#if defined(__MMI_LANG_SPANISH__)
	SSC_STR_ERR_SET_SPANISH,
#endif
 
#if defined(__MMI_LANG_DANISH__)
	SSC_STR_ERR_SET_DANISH,
#endif
 
#if defined(__MMI_LANG_FRENCH__)
	SSC_STR_ERR_SET_FRENCH,
#endif
#if defined(__MMI_LANG_GERMAN__)
	SSC_STR_ERR_SET_GERMAN,
#endif
#if defined(__MMI_LANG_ITALIAN__)
	SSC_STR_ERR_SET_ITALIAN,
#endif
#if defined(__MMI_LANG_THAI__)
	SSC_STR_ERR_SET_THAI,
#endif
#if defined(__MMI_LANG_RUSSIAN__)
	SSC_STR_ERR_SET_RUSSIAN,
#endif
#if defined(__MMI_LANG_PORTUGUESE__)
	SSC_STR_ERR_SET_PORTUGUESE,
#endif
#if defined(__MMI_LANG_TURKISH__)
	SSC_STR_ERR_SET_TURKISH,
#endif
#if defined(__MMI_LANG_VIETNAMESE__)
	SSC_STR_ERR_SET_VIETNAMESE,
#endif
#if defined(__MMI_LANG_INDONESIAN__)
	SSC_STR_ERR_SET_INDONESIAN,
#endif
#if defined(__MMI_LANG_MALAY__)
	SSC_STR_ERR_SET_MALAY,
#endif

/*CSD end*/
      SSC_STR_ERR_SET_ARABIC,
      SSC_STR_ERR_SET_PERSIAN,
	SSC_STR_ERR_SET_HINDI,
	SSC_STR_ERR_SET_MARATHI,
	SSC_SCR_FACTORY_MODE_STR_ID
};

enum SCR_SSC_STRING_HANDLIE
{
	SCR_ENTER_SSC	=		IDLE_APP_BASE+300
};

#define SSC_STRING_TIMEOUT		2000

#ifdef MMI_ON_HARDWARE_P
extern const kal_char* release_verno(void);
//micha1217
#ifdef __MMI_HW_VERSION__
extern const kal_char* release_hw_ver(void);
#endif
//extern kal_char* get_imei(void);

#endif
void get_imeirsp(void * buf);


#endif /* uem_ft.h */


