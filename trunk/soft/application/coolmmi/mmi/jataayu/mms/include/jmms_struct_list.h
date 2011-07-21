/***************************************************************************
 *
 * File Name : jmms_struct_list.h
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
 * @file		jmms_struct_list.h
 * @ingroup		jMMS
 * @brief		Describes the MMS structure APIs.
 *
 **/
 
#ifndef __JMMS_STRUCT
#define __JMMS_STRUCT 

/***************************************************************************
* Macros Definitions
**************************************************************************/

/***************************************************************************
* Global variables declarations
**************************************************************************/
#define CREATE_MODE_SIGN_BUFFER	128
#define	SENDSET_SLIDE_TIME_BUFFER_LEN	3

/***************************************************************************
* Local Function Prototypes
***************************************************************************/

typedef enum _delivery_time
{
	E_MSG_IMMIDIATE = 0x00,	
	E_MSG_1HOUR,		
	E_MSG_12HOUR,
	E_MSG_24HOUR,
} EDeliveryTime;

typedef enum _expiry_time
{
	E_MSG_MAX_EXPIRYTIME = 0x00,	
	E_MSG_1HOUR_EXPIRYTIME,		
	E_MSG_12HOUR_EXPIRYTIME,
	E_MSG_1DAY_EXPIRYTIME,
	E_MSG_1WEEK_EXPIRYTIME
} EExpiryTime;

typedef struct _jmms_compose_items_struct
{	
    S8 *auto_signature[2];
	U8 signature_buffer[CREATE_MODE_SIGN_BUFFER*2];	
} compose_items_struct;

typedef struct _jmms_send_items_struct
{    
	S8 *validity_period_list[5];
	S8 *send_delivert_report[2];
	S8 *send_read_report[2];
	S8 *priority_list[3];
	U8* slide_timing[SENDSET_SLIDE_TIME_BUFFER_LEN];
	S8 *delivery_time_list[4];
} send_items_struct;

typedef struct _jmms_retrieve_items_struct
{    
    S8 *home_network_list[3];
	S8 *read_report_list[3];
	S8 *rcv_DeliveryReport_List[2];
} retrieve_items_struct;

typedef struct _jmms_global_context_struct
{    
	compose_items_struct compose_items_list;
	send_items_struct	send_items_list;
	retrieve_items_struct	retrieve_items_list;
	
	JC_UINT32 currAutoSignItem;
	JC_UINT32 currValidityPeriodItem;
	JC_UINT32 currSendDeliveryReportItem;
	JC_UINT32 currSendReadReportItem;
	JC_UINT32 currPriortlistItem;
	JC_CHAR   szSlideTimeBuffer[SENDSET_SLIDE_TIME_BUFFER_LEN];
	JC_UINT32 currSendDeliveryTimeItem;
	JC_UINT32 currHomeNwkItem;
	JC_UINT32 currRcvReadReportItem;
	JC_UINT32 currRcvDeliveryReportItem;
	JC_UINT8  pcAutoSignBuff[MAX_AUTOSIGN_BUFF_SIZE*2];	
} global_context_struct;

typedef enum
{		
    E_COMPOSE_AUTO_SIGN_CAPTION = 0x00,
    E_COMPOSE_AUTO_SIGN_ITEM,
    E_COMPOSE_SIGNATURE_CAPTION,
    E_COMPOSE_SIGNATURE_ITEM,
    COMPOSE_SCREEN_ICON_TOTAL
} compose_items_list_icon_enum;

typedef enum
{
    E_VAlIDITY_PERIOD_CAPTION = 0x00,
	E_VALITY_PERIOD_ITEM,	
    E_SEND_DELIVERY_REPORT_CAPTION,
    E_SEND_DELIVERY_REPORT_ITEM,
    E_SEND_READ_REPORT_CAPTION,
    E_SEND_READ_REPORT_ITEM,
    E_PRIORITY_CAPTION,
    E_PRIORITY_ITEM,
    E_SLIDE_TIMING_CAPTION,
    E_SLIDE_TIMING_ITEM,
    E_SEND_DELIVERY_TIME_CAPTION,
    E_SEND_DELIVERY_TIME_ITEM,   
    SEND_SCREEN_ICON_TOTAL
} send_items_list_icon_enum;

typedef enum
{
	E_HOME_NETWORK_CAPTION = 0x00,
	E_HOME_NETWORK_ITEM,
	E_RTV_READ_REPORT_CAPTION,
	E_RTV_READ_REPORT_ITEM,
	E_RTV_DELIVERY_REPORT_CAPTION,
	E_RTV_DELIVERY_REPORT_ITEM,
	RETRIEVE_SCREEN_ICON_TOTAL
} retrieve_items_list_icon_enum;

#endif//__JMMS_STRUCT
