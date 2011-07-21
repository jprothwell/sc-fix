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
 * MessagesMiscell.H
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: MessagesMiscell.h

  	PURPOSE		: ---

 

	AUTHOR		: Hiran and Magesh k

	DATE		: ---

**************************************************************/

#ifndef _PIXCOM_MESSAGES_MISCELL_H
#define _PIXCOM_MESSAGES_MISCELL_H
//
//
#include  "mmi_data_types.h"
#include "messagesl4def.h"

#define		USE_NUM_MIN_LENGTH				3
#define  		IS_NUM(c)  (((c)>='0') && ((c)<='9'))


#define MAX_DIGITS					41		// Maximum number of digits in phone number
#define MAX_DIGITS_SMS				21		// Maximum number of digits in sms source/destination
#define MAX_TIMESTAMP_SIZE		18


#define MAX_PROFILE_EDIT_ITEMS	4
#define MAX_PROFILE_NAME_LEN		30		// check with ps

#define MAX_STATUS_REPORT_NUM	10

 
#define	MESSAGES_POPUP_TIME_OUT     			UI_POPUP_NOTIFYDURATION_TIME_2000
#define	MESSAGES_SUBLCD_TIME_OUT    			1000*10
#define	MESSAGES_LMS_TIME_OUT				1000*10
#define	MESSAGES_INPROGRESS_TIME_OUT    		1000*50

#define MAX_LFOUR_PROFILES						4
#define MAX_LFOUR_VP							5
#define MAX_LFOUR_MSG_TYPE					4
#define MAX_MAILBOX_PS_SUPPORT				2


//  typedef void (*FuncPtr)(void);    // zrx del  20060612 
typedef enum
{
	SMS_OK,
	SMS_ERROR
}msg_result_enum;

typedef enum
{
	POST_PARAMS_FO,
	POST_PARAMS_PROFILE_NAME,
	POST_PARAMS_ALL_LANGUAGES,
	POST_PARAMS_ALL_LANGUAGES_DEFAULT_FACTORY,
	POST_PARAMS_REST	// PID, DCS, VP
}msg_post_params_enum;


typedef struct
{
	U8 totalInboxStr[24*ENCODING_LENGTH];
	U8 totalOutboxStr[24*ENCODING_LENGTH];
#ifdef __MMI_MESSAGES_DRAFT_BOX__
	U8 totalDraftboxStr[24*ENCODING_LENGTH];
#endif
}msg_in_out_struct;
//
typedef struct
{
	S32 vpIndex;
	S32 msgTypeIndex;
	U8 profileName[(MAX_PROFILE_NAME_LEN*ENCODING_LENGTH)+ENCODING_LENGTH];
	U8 profileNameDcs;
	U8 profileIndex;
	U8 scAddress[(MAX_CC_ADDR_LEN*ENCODING_LENGTH)+ENCODING_LENGTH];
}msg_profile_edit_struct;

typedef struct
{
	U8 mailboxName[(MAX_PROFILE_NAME_LEN*ENCODING_LENGTH)+ENCODING_LENGTH];
	U8 mailboxNameDcs;
	U8 malboxNameLength;
	U8 mailboxIndex;
	U8 mailboxAddress[(MAX_CC_ADDR_LEN*ENCODING_LENGTH)+ENCODING_LENGTH];
}msg_mailbox_edit_struct;
//
//
#if defined(__MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__) 
typedef struct
{
	U16 simUsed;
	U16 simTotal;
	U16 meUsed;
	U16 meTotal;
	U8 prefStorage;
}msg_mem_status_struct;


typedef enum
{
	DISP_NONE,
	DISP_MEM_STATUS,
	DISP_PREF_STORAGE,
	UPDATE_MEM_STATUS,
	UPDATE_PREF_STORAGE
}msg_mem_status_enum;
#endif // defined(__MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__) 

//
typedef struct 
{
	U8 currReqProfileNo;
	U8 currMaxPsProfiles;
	U8 profileNameLength;
	U8 profileName[MAX_LFOUR_PROFILES][(MAX_PROFILE_NAME_LEN*ENCODING_LENGTH)+ENCODING_LENGTH];
} msg_profile_list_struct;

typedef struct 
{
	l4c_number_struct	scAddr;
	U8	profile;						/* SMS_DEST_NO_ENUM */
	U8	para;							/* MMI_PARA_ENUM */
	U8	vp;								/* smsal_vp_enum */
	U8	pid;							/* smsal_pid_enum */
	U8	dcs;							/* smsal_dcs_enum */
}msg_active_profile_struct;

typedef struct
{
	unsigned short totalinbox;
	unsigned short totaloutbox;
	unsigned short totaldraftbox;
}msg_msgbox_info_struct;


#if defined(__MMI_MESSAGES_TEMPLATE__)

U8	InitTemplates				(void) ;
void	DeInitTemplates				(void) ;
void	LoadTemplatesInDisplayList	(void) ;
void	GetTemplatesToEdit			(S8* buffer, U32 index) ;
U8	DeleteTemplate				(U32 index) ;
U8 StoreTemplate 					(U32 index);
U16	CalTemplateBase				(void) ;

#endif // defined(__MMI_MESSAGES_TEMPLATE__)

extern void GetCurrEndKeyDownHandler (void);
extern void ExecCurrEndKeyDownHandler (void);
//
extern void SetMessagesScrnIdToDelHistoryNodes (U16 ScrnId);
extern void SetMessagesScrnIdDelUpToHistoryNodes (U16 StartScrnId);
extern void DeleteMessagesHistoryNodes (void);
//
extern void Messages2GoBackHistory (void);
extern void Messages4GoBackHistory(void);
//
extern U16 GetMessagesCurrScrnID (void);
extern void SetMessagesCurrScrnID (U16 scrnID);
//
extern U16 IsMMIInIdleState (void);
extern U8   IsKeyPadLockState (void);
extern void PlayMessageArrivalTone (void);
extern void RestoreMessagesSettings (void);		
extern U8 IsMessagesReEntering (void);
extern U16 ObtainNumbersFromMessageText	(char * phNumber, U8 * smsText, U16 textLength, U8 startindex);
extern S8* GetPhoneNumber (S8 * number);
#ifdef __MMI_MESSAGES_CHAT__
extern pBOOL SmsChatCompareFunction(char * number, U8 len, U8 type, U16 index);
#endif
//

#endif /* _PIXCOM_MESSAGES_MISCELL_H */


