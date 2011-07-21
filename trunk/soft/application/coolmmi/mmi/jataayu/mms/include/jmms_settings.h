/***************************************************************************
 *
 * File Name : jwap_settings.h
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND 
 * AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************/
 
 /** 
 * @file		jmms_settings.h
 * @ingroup		jWAP
 * @brief		Describes the MMS settings APIs.
 **/
 
#ifndef __JMMS_SETTINGS
#define __JMMS_SETTINGS 
 
/***************************************************************************
* Macros
**************************************************************************/

/***************************************************************************
* Global variables declarations
**************************************************************************/
#define MAX_AUTOSIGN_BUFF_SIZE 128
#define MMIMMS_SLIDE_DUARATION_DEFAULT (5*1000)

typedef enum _settings_message_priority
{
	E_MSG_SETTINGS_PRIORITY_LOW = 0x00,	/**< Specifies as low priority message*/
	E_MSG_SETTINGS_PRIORITY_NORMAL,		/**< Specifies as normal priority message*/
	E_MSG_SETTINGS_PRIORITY_HIGH			/**< Specifies as high priority message*/
} ESettingsMessagePriority;

typedef enum _settings_message_retrieve_read_report
{
	E_MSG_SETTINGS_READ_REPORT_SEND,	
	E_MSG_SETTINGS_READ_REPORT_NEVERSEND,		
	E_MSG_SETTINGS_READ_REPORT_ONREQUEST			
} ESettingsRetrieveReadReport;

typedef struct _mms_settings
{	
	JC_UINT32					ePriority;	
	JC_UINT32					bDeliveryReport;
	JC_UINT32					bReadReport;
	JC_UINT32					uiExpiryTime;
	JC_UINT32					uiDeliveryTime ; //earliest possible delivery time
	JC_UINT32					bIsAutoSignatureOn;	
	JC_UINT8                    pszAutoSignature[MAX_AUTOSIGN_BUFF_SIZE*2];	
	JC_UINT32					bRetrieveDeliveryReport ; //For Retrieve Settings
	JC_UINT32					uiRetrieveReadReport;
	JC_UINT32					uiDefaultSlideTime;	
}JMMS_SETTINGS;

typedef enum e_jmms_autosign_state
{
   E_JMMS_AUTOSIGN_STATE_OFF, 
   E_JMMS_AUTOSIGN_STATE_ON 	
}  E_JMMS_AUTOSIGN_STATE ;


/***************************************************************************
* Local Function Prototypes
***************************************************************************/
void	jMms_setting_entry_handler ();
void	jMms_setting_Hlt_handler(int iSelectIndex);
void	jMms_server_profile_entry_handler();
void	jmms_SettingComposeEntryHandler(); 
void	jMms_SettingSendEntryHandler();
void	jMms_SettingRetrieveEntryHandler();
void 	jmms_SettingsComposeFillInlineStruct(void *pComposeInlineStruct );
void 	jMMS_SettinginitProfiles () ;
void 	jMMS_SettingreadUpdateDataConnInfo () ;
void	jMMS_Setting_deInitProfiles () ;
void 	jMMS_GetUpdatedSetting(void) ;
void 	jmms_SetDefaultSettings();
//void 	jmms_SettingsEndKeyHandler();
JC_RETCODE jmms_ReadFromSettingsFile ();
JC_UINT32 jmms_GetPriorityValue();
JC_UINT32 jmms_GetExpiryTimeValue();
JC_UINT32 jmms_GetDeliveryTimeValue();

#endif //__JMMS_SETTINGS

