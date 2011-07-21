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
 * EmsMiscell.C
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
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

 /**************************************************************

	FILENAME	: EmsMiscell.c

  	PURPOSE		: Dislay all EMS objects(Manufacture picture, 
					User defined picture, Predefined animation, Usedefiend animation,
					Manufacturer melodies, Usedefined melodies, Predefined sound.)

 

	AUTHOR		: Hiran, Magesh k

	DATE		: 

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
#include "frameworkstruct.h"		/* for psextfuncptr */

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
//
#include "ems.h"
#include "gui_ems.h"
#include "wgui_ems_categories.h"
#include "gsm7bitdefaultalphabet.h"
//
#include "settingprofile.h"
#include "filesystemdef.h"
#include "filesystemgprot.h"
#include "downloaddefs.h"
#include "downloadprotos.h"
//
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
//
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
#include "filemgr.h"
#include "filemanagerdef.h"
#include "filemanagergprot.h"
#endif
#ifdef __DOWNLOAD__
#include "simdetectiongexdcl.h"
#endif
#ifdef __MMI_IMELODY_SUPPORT__
#include "resource_audio.h"
extern audio_resource_struct* resource_ems_imelodys;
#endif
#include "mmi_trace.h"
#undef __NEWSIMULATOR

#ifdef MMI_ON_HARDWARE_P
#include "nvram_user_defs.h"
#endif 

#include "smsapi.h"

#if defined(__MMI_MESSAGES_EMS__)
//
#ifndef NVRAM_EMS_MY_PICTURE_NAME_RECORD_SIZE
#define NVRAM_EMS_MY_PICTURE_NAME_RECORD_SIZE	140
#endif

#ifndef NVRAM_EMS_MY_ANIMATION_NAME_RECORD_SIZE
#define NVRAM_EMS_MY_ANIMATION_NAME_RECORD_SIZE	140
#endif

#if defined(__MMI_IMELODY_SUPPORT__)
#ifndef NVRAM_EMS_MY_MELODY_NAME_RECORD_SIZE
#define NVRAM_EMS_MY_MELODY_NAME_RECORD_SIZE	140
#endif
#endif //__MMI_IMELODY_SUPPORT__


#define   MESSAGES_ONE_SEC_TIME_OUT     1000	
#define   MAX_EMS_DOWNLOAD_OBJECT_NAME_LEN			13// wangbei add 20060614
#define   MAX_EMS_OBJECT_LIST_DISPLAY		15	
#define   MAX_EMS_PREDEFINED_ANIMATION   	15	
#define   MAX_EMS_PREDEFINED_SOUND		10
#ifdef __EMS_REL5__
#define   MAX_CURR_EMS_OBJECT_DATA_BUFF				1588+12  
#define   MAX_EMS_OBJECT_INSERT_SIZE				1588
#else
#define   MAX_CURR_EMS_OBJECT_DATA_BUFF				128+12  
#define   MAX_EMS_OBJECT_INSERT_SIZE					128     
#endif
#if defined(__DOWNLOAD__)||defined(MMI_ON_WIN32)
#define   MAX_EMS_DOWNLOAD_OBJECTS					MAXIMUM_EMS_DOWNLOAD_IMELODY    
#define   MAX_EMS_DOWNLOAD_PICTURE_OBJECTS			MAX_EMS_DOWNLOAD_OBJECTS	
#define   MAX_EMS_DOWNLOAD_ANIMATION_OBJECTS		MAX_EMS_DOWNLOAD_OBJECTS
#define   MAX_EMS_DOWNLOAD_IMELODY_OBJECTS			MAXIMUM_EMS_DOWNLOAD_IMELODY	

#define   MAX_EMS_DOWNLOAD_OBJECT_NAME_LEN			13
#define   MAX_EMS_DOWNLOAD_OBJECT_NAME_INIT_PATH	30
#define   MAX_EMS_DOWNLOAD_OBJECT_NAME_EXTN_LEN	5
#define	MAX_EMS_DISPLAY_NAME_LEN					(MAX_EMS_DOWNLOAD_OBJECT_NAME_LEN*ENCODING_LENGTH)+ENCODING_LENGTH
#define	MAX_EMS_DOWNLOAD_OBJECT_FILE_LEN			MAX_EMS_DOWNLOAD_OBJECT_NAME_LEN+MAX_EMS_DOWNLOAD_OBJECT_NAME_INIT_PATH+MAX_EMS_DOWNLOAD_OBJECT_NAME_EXTN_LEN
#elif defined (__MMI_FILE_MANAGER__)
#define   MAX_EMS_DOWNLOAD_OBJECT_NAME_LEN			FMGR_MAX_INPUT_FILE_LEN
#define   MAX_EMS_DOWNLOAD_OBJECT_NAME_INIT_PATH	FMGR_MAX_PATH_LEN
#define   MAX_EMS_DOWNLOAD_OBJECT_NAME_EXTN_LEN	FMGR_MAX_EXT_LEN
#define	MAX_EMS_DOWNLOAD_OBJECT_FILE_LEN			FMGR_MAX_PATH_LEN+FMGR_MAX_FILE_LEN
#endif
//
#define STR_EMS_PICTURE_OBJECT_FILE_NAME_EXTN_ID	".ems"	
#define STR_EMS_ANIMATION_OBJECT_FILE_NAME_EXTN_ID	".anm"	
#define STR_EMS_IMELODY_OBJECT_FILE_NAME_EXTN_ID	".imy"	
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
#define STR_EMS_PIC_A_ANIM_OBJECT_FILE_NAME_INIT_PATH_ID			FMGR_DEFAULT_FOLDER_IMAGES//"USER\\image\\ems\\"
#define STR_EMS_IMELODY_OBJECT_FILE_NAME_INIT_PATH_ID				FMGR_DEFAULT_FOLDER_AUDIO//"USER\\sound\\ems\\"
#else
#define STR_EMS_PIC_A_ANIM_OBJECT_FILE_NAME_INIT_PATH_ID			"c:\\USER\\image\\ems\\"
#define STR_EMS_IMELODY_OBJECT_FILE_NAME_INIT_PATH_ID				"c:\\USER\\sound\\ems\\"
#endif
//CSD Joy added for file names, 1128
#define STR_EMS_MANUFACTURER_PIC_OBJECT_FILE_NAME_0	"ems0"
#define STR_EMS_MANUFACTURER_PIC_OBJECT_FILE_NAME_1	"ems1"
#define STR_EMS_MANUFACTURER_PIC_OBJECT_FILE_NAME_2	"ems2"
#define STR_EMS_MANUFACTURER_PIC_OBJECT_FILE_NAME_3	"ems3"
#define STR_EMS_MANUFACTURER_PIC_OBJECT_FILE_NAME_4	"ems4"
#define STR_EMS_MANUFACTURER_PIC_OBJECT_FILE_NAME_5	"ems5"
#define STR_EMS_MANUFACTURER_PIC_OBJECT_FILE_NAME_6	"ems6"
#define STR_EMS_MANUFACTURER_PIC_OBJECT_FILE_NAME_7	"ems7"
#define STR_EMS_MANUFACTURER_PIC_OBJECT_FILE_NAME_8	"ems8"
#define STR_EMS_MANUFACTURER_PIC_OBJECT_FILE_NAME_9	"ems9"
//CSD end

extern INT pfError(INT err);
#ifdef MMI_ON_WIN32
extern INT fdelete(U8* fName);
#endif
extern void (*EMS_inputbox_input_callback)(void);
#endif // defined(__MMI_MESSAGES_EMS__)
#if defined (__MMI_VCARD__) || defined (__MMI_VCALENDAR__)
#ifndef MAX_EMS_OBJECT_INSERT_SIZE
#ifdef __EMS_REL5__
#define   MAX_EMS_OBJECT_INSERT_SIZE					1588
#else
#define   MAX_EMS_OBJECT_INSERT_SIZE					128     
#endif
#endif
#ifndef MAX_EMS_DOWNLOAD_OBJECT_FILE_LEN
#define	MAX_EMS_DOWNLOAD_OBJECT_FILE_LEN				260+41 // FMGR_MAX_PATH_LEN+FMGR_MAX_FILE_LEN
#endif
PsFuncPtr emscallback;
static S8* file_path;
#endif
//
#if defined(__MMI_MESSAGES_EMS__)
U16 PredefinedAnimationStrIDs [] =					// Predefined Animations
{
	STR_PREDEFINED_ANIMATION_0,
	STR_PREDEFINED_ANIMATION_1,
	STR_PREDEFINED_ANIMATION_2,
	STR_PREDEFINED_ANIMATION_3,
	STR_PREDEFINED_ANIMATION_4,
	STR_PREDEFINED_ANIMATION_5,
	STR_PREDEFINED_ANIMATION_6,
	STR_PREDEFINED_ANIMATION_7,
	STR_PREDEFINED_ANIMATION_8,
	STR_PREDEFINED_ANIMATION_9,
	STR_PREDEFINED_ANIMATION_10,
	STR_PREDEFINED_ANIMATION_11,
	STR_PREDEFINED_ANIMATION_12,
	STR_PREDEFINED_ANIMATION_13,
	STR_PREDEFINED_ANIMATION_14 
};
const U16 PredefinedSoundStrIDs [] =					// Predefined Sounds
{		
	STR_PREDEFINED_SOUND_0,
	STR_PREDEFINED_SOUND_1,
	STR_PREDEFINED_SOUND_2,
	STR_PREDEFINED_SOUND_3,
	STR_PREDEFINED_SOUND_4,
	STR_PREDEFINED_SOUND_5,
	STR_PREDEFINED_SOUND_6,
	STR_PREDEFINED_SOUND_7,
	STR_PREDEFINED_SOUND_8,
	STR_PREDEFINED_SOUND_9
};
U16 ManufacturerPictureStrIDs [] =					// ManufacturerPicture Str IDs
{
	STR_MANUFACTURER_PICTURE_0,
	STR_MANUFACTURER_PICTURE_1,
	STR_MANUFACTURER_PICTURE_2,
	STR_MANUFACTURER_PICTURE_3,
	STR_MANUFACTURER_PICTURE_4,
	STR_MANUFACTURER_PICTURE_5,
	STR_MANUFACTURER_PICTURE_6,
	STR_MANUFACTURER_PICTURE_7,
	STR_MANUFACTURER_PICTURE_8,
	STR_MANUFACTURER_PICTURE_9
};
const U16 ManufacturerMelodiesStrIDs [] =					// Manufacturer Melodies Str IDs
{		
	STR_MANUFACTURER_MELODIES_0,
	STR_MANUFACTURER_MELODIES_1,
	STR_MANUFACTURER_MELODIES_2,
	STR_MANUFACTURER_MELODIES_3,
	STR_MANUFACTURER_MELODIES_4,
	STR_MANUFACTURER_MELODIES_5,
	STR_MANUFACTURER_MELODIES_6,
	STR_MANUFACTURER_MELODIES_7,
	STR_MANUFACTURER_MELODIES_8,
	STR_MANUFACTURER_MELODIES_9
};

static U16 preDefinedObjectIds[MAX_EMS_OBJECT_LIST_DISPLAY]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
static U8* userDefinedObjectDataPtrs[MAX_EMS_OBJECT_LIST_DISPLAY];
static U8 userDefinedObjectDataBuff[MAX_CURR_EMS_OBJECT_DATA_BUFF]={0,0};
static S32 userDefinedObjectDataSize=0;
static S32 currObjectListHiliteIndex=0;
static U8 currMaxEmsObjectList=0;
static U8 currMaxEmsPictureObjects=0;
static U8 userEmsObjectNameInput[(MAX_EMS_DOWNLOAD_OBJECT_NAME_LEN+1)*ENCODING_LENGTH]={0,0};// wangbei del 20060614
//static U8 userEmsObjectNameInput[(13+1)*ENCODING_LENGTH]={0,0};// wangbei add 20060614
#ifdef __DOWNLOAD__
typedef struct 
{
	U8 emsObjectNames [MAX_EMS_DOWNLOAD_OBJECTS][(MAX_EMS_DOWNLOAD_OBJECT_NAME_LEN*ENCODING_LENGTH)+ENCODING_LENGTH];
} EMS_OBJECT_NAMES_NVRAM_STRUCT;

static S32 currDownloadIndex=0;
static S32 currMaxDownloadList=0;
#if defined(__MMI_IMELODY_SUPPORT__)
static U8 currMaxDownLoadedEmsIMelodies=0;
static DYNAUDIOINFO	emsMelodyInfoStruct[MAX_EMS_DOWNLOAD_OBJECTS];
#endif //__MMI_IMELODY_SUPPORT__
static EMS_OBJECT_NAMES_NVRAM_STRUCT nvramEmsObjectNames;
static U8 *downloadEmsPictureObjectNames[MAX_EMS_DOWNLOAD_OBJECTS]={NULL,NULL,NULL,NULL,NULL};		// picture
static U8 *downloadEmsAnimationObjectNames[MAX_EMS_DOWNLOAD_OBJECTS]={NULL,NULL,NULL,NULL,NULL};	// animation
#endif //__DOWNLOAD__

static U8 *emsObjectNameWhenPreview=NULL;
static EMS_OBJECT_DISPLAY_STATE	currEmsObjectDisplayListState;
static U8 emsFmtTextStyleStates[4]={0,0,0,0};
static U8 IsAllowed=FALSE;
static S32 listOfIsAllowedData[MAX_EMS_OBJECT_LIST_DISPLAY];
//
#if defined(__MMI_IMELODY_SUPPORT__) && defined(__DOWNLOAD__)
static U8 totalEmsDownloadMelody=0;
static U8 totalWapDownloadMelody=0;
static U8 totalComposedMelody=0;
static U16 downloadMelodyIDs[MAXIMUM_DOWNLOAD_AUDIO];
static U16 composedMelodyIDs[MAXIMUM_COMPOSED_RINGTONE];
static U16 playCurrIMelodyId=0;
#endif //__MMI_IMELODY_SUPPORT__
//
static EMSObject *previewObjectListPtr=NULL;
static S32 nObjectLists=0;
static S32 currObjectIndex=0;

extern UI_EMS_input_box	MMI_EMS_inputbox;
extern EMSTextFormat		wgui_EMS_text_format;

//CSD end
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
void mmi_ems_fm_send_by_ems (S8* emsObjectFileNameBuf, EMS_OBJECT_DISPLAY_STATE emsObjectType);
void mmi_ems_add_object_from_fm(void);
void mmi_ems_preview_object_from_fm (PU16 filePath, int is_short);
EMSTATUS mmi_ems_fm_open_file(S8* emsObjectFileNameBuf, EMS_OBJECT_DISPLAY_STATE emsObjectType);
#endif
#if defined(__MMI_IMELODY_SUPPORT__)
extern U32 GetEmsMelodyLength(S32 index);
extern PU8 GetEmsMelodyData(S32 index);
#ifdef MMI_ON_HARDWARE_P
extern void custom_get_fixed_ems_imelody_range(U8* min_id, U8* max_id);
#endif
#endif
extern U32 GetEmsPictureLength(S32 index);
extern PU8 GetEmsPictureData(S32 index);
extern void handle_category28_input(void); 
extern void T9SynT9WithInputBox(void);
#endif //__MMI_MESSAGES_EMS__
extern EMSData * GetEMSDataForView              (EMSData ** p, U8 force) ;
extern EMSData * GetEMSDataForEdit              (EMSData ** p, U8 force) ;
extern U8        DeInitMessagesEMSBuffer        (msg_ems_data_struct * pE);
// wangbei add start 20060614
#if 0
INT pfOpenError(FILE_HANDLE err)
{
        return 0;
}
int FS_Commit(FS_HANDLE FileHandle)
{
    return 0;
}
int FS_Delete(const WCHAR * FileName)
{
        return 0;
}
INT	CreateDir(U8*	pPath)
{
	return 0;
}
#endif
// wangbei add end
//////////////////////////////////////////////////////////////////////
// Routines for manipulating local buffers for EMS Edit and View
//////////////////////////////////////////////////////////////////////
/*****************************************************************************
* FUNCTION
*  ErrorHandlingWhileInsertingObject
* DESCRIPTION
*   Handle error case when insert object
*
* PARAMETERS
*  emsObjectStatus	IN		insert status
* RETURNS
*  none
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void ErrorHandlingWhileInsertingObject (EMSTATUS emsObjectStatus)
{
	// hander error case, if could not insert objects.
	switch(emsObjectStatus)
	{
		case EMS_OK: break;
		case EMS_NO_MEMORY:
			DisplayPopup ((PU8)GetString (STR_EMS_OBJECT_NUM_EXCEED),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			break;
		case EMS_NO_SPACE:
			DisplayPopup ((PU8)GetString (STR_NO_SPACE_TO_INSERT_OBJECT),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			break;
		case EMS_INVALID_OBJECT:
		#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
			DisplayPopup((PU8)GetString(STR_FMGR_INVALID_FORMAT), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		#else
			DisplayPopup ((PU8)GetString (STR_SMS_DOES_NOT_SUPPORT), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		#endif
			break;
		case EMS_EMS_DATA_EXCEEDED:
		#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
			DisplayPopup((PU8)GetString(FMGR_FS_FILE_TOO_LARGE_TEXT), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		#else
			DisplayPopup((PU8)GetString(STR_SMS_FAILURE_MSG_LEN_EXCEEDED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		#endif
			break;
		case EMS_NULL_POINTER:
			DisplayPopup((PU8)GetString(STR_READ_SMS_DATA_FAILED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			break;
		case EMS_NO_ALIGNMENT_FMT:
			DisplayPopup((PU8)GetString(STR_SMS_OBJECT_EXISTENCE_ERROR), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			break;
		default /*EMS_UNSPECIFIED_ERROR*/:
			DisplayPopup ((PU8)GetString (STR_SMS_DOES_NOT_SUPPORT), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			break;
	}
}


#if defined (__MMI_VCARD__) || defined (__MMI_VCALENDAR__)
/*****************************************************************************
* FUNCTION
*  mmi_ems_vobject_send_by_ems_callback
* DESCRIPTION
*   Call back vobject send by ems function
*
* PARAMETERS
*  flag		IN	send result
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_ems_vobject_send_by_ems_callback(void* dummy, module_type mod, U16 flag)
{
	switch (flag)
	{
		case MMI_FRM_SMS_OK:
		DisplayPopup ((PU8)GetString(STR_SMS_SEND_SUCCESS), IMG_SEND_SUCCESS_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
			mmi_frm_sms_delete_screen_history();
			break;
		case MMI_FRM_SMS_RETURN:
		case MMI_FRM_SMS_END:
			/* do not keep vcard and vcalendar content when sending fail */
			ReleaseEMSEditBuffer();
			break;
		case MMI_FRM_SMS_DATAEXCEED:
			#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
			DisplayPopup((PU8)GetString(FMGR_FS_FILE_TOO_LARGE_TEXT), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			#else
			DisplayPopup((PU8)GetString(STR_SMS_FAILURE_MSG_LEN_EXCEEDED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			#endif
			mmi_frm_sms_delete_screen_history();
			break;
		case MMI_FRM_SMS_NOTREADY:
		        DisplayPopup ((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);			
			break;
		default:
			/* do not keep vcard and vcalendar content when sending fail */
			ReleaseEMSEditBuffer();
		        DisplayPopup ((PU8)GetString(STR_SMS_SEND_FAILED), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);			
			mmi_frm_sms_delete_screen_history();
			break;		
	}
	if(file_path!=NULL)
	{
		OslMfree(file_path);
		file_path=NULL;
	}
	if(emscallback)
		emscallback(&flag);
}


/*****************************************************************************
* FUNCTION
*  mmi_ems_vobject_send_by_ems
* DESCRIPTION
*   Send vobject by ems
*
* PARAMETERS
*  scrport	IN	source port
*  dstport	IN	destination port
*  filepath	IN	filepath
*  callback	IN	callback function
* RETURNS
*  none
* GLOBALS AFFECTED
*   userEmsObjectNameInput, emscallback
*****************************************************************************/
void mmi_ems_vobject_send_by_ems (U16 scrport, U16 dstport, S8* filepath, PsFuncPtr callback)
{
	U16	result=TRUE;
	mmi_frm_sms_send_struct * sendData=OslMalloc(sizeof(mmi_frm_sms_send_struct));
	memset((S8*)sendData, 0, sizeof(mmi_frm_sms_send_struct));

	emscallback=callback;	
	sendData->sendrequire |= MMI_FRM_SMS_PORT;
	sendData->srcport = scrport;
	sendData->desport = dstport;
	sendData->dcs = SMSAL_8BIT_DCS;
	if (IsMessagesReEntering())
	{
		result=FALSE;
		DisplayPopup ((PU8)GetString (STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		if(emscallback!=NULL)
			emscallback((PU16)&result);
		OslMfree(sendData);
		return;
	}
	if((pfnUnicodeStrlen(filepath)==0)||(pfnUnicodeStrlen(filepath)>MAX_EMS_DOWNLOAD_OBJECT_FILE_LEN))
	{
		result=FALSE;
		DisplayPopup ((PU8)GetString (STR_EMS_OBJECT_SIZE_NOT_ALLOWED_ID), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		if(emscallback!=NULL)
			emscallback((PU16)&result);
		OslMfree(sendData);
		return;
	}
	if(file_path!=NULL)
	{
		OslMfree(file_path);
		file_path=NULL;
	}
	file_path=OslMalloc((pfnUnicodeStrlen(filepath)+1)*ENCODING_LENGTH);
	pfnUnicodeStrcpy(file_path, filepath);
	sendData->filename = file_path;
	mmi_frm_sms_send_sms(mmi_ems_vobject_send_by_ems_callback, MOD_MMI, sendData);
	OslMfree(sendData);
}
#endif


#if defined(__MMI_MESSAGES_EMS__)
/**************************************************************
				text format
**************************************************************/

/*****************************************************************************
* FUNCTION
*  GetFmtTextSize
* DESCRIPTION
*   Get text format size
*
* PARAMETERS
*  none
* RETURNS
*  size
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 GetFmtTextSize (void)
{
	switch(EMS_inputbox_get_text_size ())
	{
		case EMS_FONTSIZE_SMALL:  return (U8)0;
		case EMS_FONTSIZE_NORMAL: return (U8)1;
		case EMS_FONTSIZE_LARGE:  return (U8)2;
	}
	return (U8)0;
}

/*****************************************************************************
* FUNCTION
*  SetFmtTextSize
* DESCRIPTION
*   Set text format size
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SetFmtTextSize (void)
{
	switch( GetHighlightedItem ())
	{
	case 0:	EMS_inputbox_set_text_size (EMS_FONTSIZE_SMALL);
		break;
	case 1: EMS_inputbox_set_text_size (EMS_FONTSIZE_NORMAL);
		break;
	case 2: EMS_inputbox_set_text_size (EMS_FONTSIZE_LARGE);
		break;
	}
	GoBackToHistory (SCR_ID_MSG_WRITE);
}

/*****************************************************************************
* FUNCTION
*  SetFmtTextStyle
* DESCRIPTION
*   Set text format style
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SetFmtTextStyle (void)
{
	#ifndef __MMI_TEXT_FORMAT_WITHOUT_BOLD__
	EMS_inputbox_set_text_style (emsFmtTextStyleStates[0], emsFmtTextStyleStates[1], emsFmtTextStyleStates[2], emsFmtTextStyleStates[3]);
	#else
	EMS_inputbox_set_text_style (emsFmtTextStyleStates[3], emsFmtTextStyleStates[0], emsFmtTextStyleStates[1], emsFmtTextStyleStates[2]);
#endif	
	GoBackToHistory (SCR_ID_MSG_WRITE);
}

/*****************************************************************************
* FUNCTION
*  GetFmtTextAlignment
* DESCRIPTION
*   Get text format alignment
*
* PARAMETERS
*  none
* RETURNS
*  alignment
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 GetFmtTextAlignment (void)
{
	switch(EMS_inputbox_get_text_alignment ())
	{
	case EMS_ALIGN_LANG_DEP:	return (U8)0;
	case EMS_ALIGN_LEFT:			return (U8)1;
	case EMS_ALIGN_CENTER:		return (U8)2;
	case EMS_ALIGN_RIGHT:		return (U8)3;
	}
	return (U8)0;
}

/*****************************************************************************
* FUNCTION
*  SetFmtTextAlignment
* DESCRIPTION
*   Set text format alignment
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SetFmtTextAlignment (void)
{
	EMSTATUS emsObjectStatus;
	switch(GetHighlightedItem ())
	{
	case 0: emsObjectStatus=EMS_inputbox_set_text_alignment (EMS_ALIGN_LANG_DEP);
		break;
	case 1: emsObjectStatus=EMS_inputbox_set_text_alignment (EMS_ALIGN_LEFT);
		break;
	case 2: emsObjectStatus=EMS_inputbox_set_text_alignment (EMS_ALIGN_CENTER);
		break;
	case 3: emsObjectStatus=EMS_inputbox_set_text_alignment (EMS_ALIGN_RIGHT);
		break;
	default: emsObjectStatus=EMS_inputbox_set_text_alignment (EMS_ALIGN_LANG_DEP);
		break;
	}
	GoBackToHistory (SCR_ID_MSG_WRITE);
	ErrorHandlingWhileInsertingObject (emsObjectStatus); // check whetehr it is inserted or not.	
}

/*****************************************************************************
* FUNCTION
*  SetFmtTextNewParagraph
* DESCRIPTION
*   Set text format new paragraph
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SetFmtTextNewParagraph (void)
{
	EMSTATUS emsObjectStatus;

	switch(GetHighlightedItem ())
	{
	case 0: emsObjectStatus=EMS_inputbox_create_new_paragraph (EMS_ALIGN_LANG_DEP);
		break;
	case 1: emsObjectStatus=EMS_inputbox_create_new_paragraph (EMS_ALIGN_LEFT);
		break;
	case 2: emsObjectStatus=EMS_inputbox_create_new_paragraph (EMS_ALIGN_CENTER);
		break;
	case 3: emsObjectStatus=EMS_inputbox_create_new_paragraph (EMS_ALIGN_RIGHT);
		break;
	default: emsObjectStatus=EMS_inputbox_create_new_paragraph (EMS_ALIGN_LANG_DEP);
		break;
	}

	GoBackToHistory (SCR_ID_MSG_WRITE);
	ErrorHandlingWhileInsertingObject (emsObjectStatus); // check whetehr it is inserted or not.	
}

/*****************************************************************************
* FUNCTION
*  EntryScrFmtTextSizeMenu
* DESCRIPTION
*   Entry text format size screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryScrFmtTextSizeMenu (void)
{
	U8 index=0;
	U16 nNumofItem=0;
	U16 nStrItemListIDs[MAX_SUB_MENUS]={0,0,0};
	U8* guiBuffer=NULL;
	U8* nStrItemList[MAX_SUB_MENUS]={NULL, NULL, NULL};
	EntryNewScreen (SCR_ID_MSG_TEXT_FORMAT_SIZE, mmi_msg_exit_generic, EntryScrFmtTextSizeMenu, NULL);		 
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_TEXT_FORMAT_SIZE);
	if(guiBuffer==NULL) currObjectListHiliteIndex=GetFmtTextSize ();		 
	SetParentHandler (MENU_EMS_TEXT_SIZE_ID);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	nNumofItem = GetNumOfChild (MENU_EMS_TEXT_SIZE_ID);
	GetSequenceStringIds(MENU_EMS_TEXT_SIZE_ID, nStrItemListIDs);
	for(index=0;index<nNumofItem;index++)
		nStrItemList[index]=(PU8)GetString ((U16)nStrItemListIDs[index]);
	SetMessagesCurrScrnID(SCR_ID_MSG_TEXT_FORMAT_SIZE);
	ShowCategory109Screen (STR_EMS_TEXT_SIZE_ID, IMG_SMS_ENTRY_SCRN_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK, 
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, NULL,
						  currObjectListHiliteIndex, guiBuffer); 
}

/*****************************************************************************
* FUNCTION
*  EntryScrFmtTextStyleMenu
* DESCRIPTION
*   Entry text format style screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryScrFmtTextStyleMenu (void)
{
	U8 index=0;
	U16 nNumofItem=0;
	U16 nStrItemListIDs[MAX_SUB_MENUS]={0,0,0};
	U8* guiBuffer=NULL;
	U8* nStrItemList[MAX_SUB_MENUS]={NULL, NULL, NULL, NULL};
	EntryNewScreen(SCR_ID_MSG_TEXT_FORMAT_STYLE, mmi_msg_exit_generic, EntryScrFmtTextStyleMenu, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_TEXT_FORMAT_STYLE);	
	if(guiBuffer==NULL)
	{
		currObjectListHiliteIndex=0;
		#ifndef __MMI_TEXT_FORMAT_WITHOUT_BOLD__
		EMS_inputbox_get_text_style (&emsFmtTextStyleStates[0], &emsFmtTextStyleStates[1], &emsFmtTextStyleStates[2], &emsFmtTextStyleStates[3]);
		#else
		EMS_inputbox_get_text_style (&emsFmtTextStyleStates[3], &emsFmtTextStyleStates[0], &emsFmtTextStyleStates[1], &emsFmtTextStyleStates[2]);
		#endif
	}
	SetParentHandler (MENU_EMS_TEXT_STYLE_ID);
	nNumofItem = GetNumOfChild (MENU_EMS_TEXT_STYLE_ID);
	GetSequenceStringIds(MENU_EMS_TEXT_STYLE_ID, nStrItemListIDs);
	for(index=0;index<nNumofItem;index++)
	{
		nStrItemList[index]=(PU8)GetString ((U16)nStrItemListIDs[index]);
	}
	SetCheckboxToggleRightSoftkeyFunctions (SetFmtTextStyle, GoBackHistory);
	SetMessagesCurrScrnID(SCR_ID_MSG_TEXT_FORMAT_STYLE);
	ShowCategory140Screen (STR_EMS_TEXT_STYLE_ID, IMG_SMS_ENTRY_SCRN_CAPTION,
							nNumofItem,  nStrItemList, 
							emsFmtTextStyleStates, currObjectListHiliteIndex,
							guiBuffer);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  EntryScrFmtAlignmentMenu
* DESCRIPTION
*   Entry text format alignment screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryScrFmtAlignmentMenu (void)
{
	U8 index=0;
	U16 nNumofItem=0;
	U16 nStrItemListIDs[MAX_SUB_MENUS]={0,0,0};
	U8* guiBuffer=NULL;
	U8* nStrItemList[MAX_SUB_MENUS]={NULL, NULL, NULL};
	EntryNewScreen(SCR_ID_MSG_TEXT_FORMAT_ALIGNMENT, mmi_msg_exit_generic, EntryScrFmtAlignmentMenu, NULL);	
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_TEXT_FORMAT_ALIGNMENT);	
	if(guiBuffer==NULL) currObjectListHiliteIndex=GetFmtTextAlignment();
	SetParentHandler (MENU_EMS_ALIGNMENT_ID);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	nNumofItem = GetNumOfChild (MENU_EMS_ALIGNMENT_ID);
	GetSequenceStringIds(MENU_EMS_ALIGNMENT_ID, nStrItemListIDs);
	for(index=0;index<nNumofItem;index++)
		nStrItemList[index]=(PU8)GetString ((U16)nStrItemListIDs[index]);
	SetMessagesCurrScrnID(SCR_ID_MSG_TEXT_FORMAT_ALIGNMENT);
	ShowCategory109Screen (STR_EMS_ALIGNMENT_ID, IMG_SMS_ENTRY_SCRN_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK, 
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, NULL,
						  currObjectListHiliteIndex, guiBuffer); 
}

/*****************************************************************************
* FUNCTION
*  EntryScrFmtNewParagraphMenu
* DESCRIPTION
*   Entry text format new paragraph screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryScrFmtNewParagraphMenu (void)
{
	U8 index=0;
	U16 nNumofItem=0;
	U16 nStrItemListIDs[MAX_SUB_MENUS]={0,0,0};
	U8* guiBuffer=NULL;
	U8* nStrItemList[MAX_SUB_MENUS]={NULL, NULL, NULL};
	EntryNewScreen(SCR_ID_MSG_TEXT_FORMAT_NEW_PARA, mmi_msg_exit_generic, EntryScrFmtNewParagraphMenu, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_TEXT_FORMAT_NEW_PARA);	
	if(guiBuffer==NULL) currObjectListHiliteIndex=GetFmtTextAlignment();	 
	SetParentHandler (MENU_EMS_NEW_PARAGRAPH_ID);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	nNumofItem = GetNumOfChild (MENU_EMS_NEW_PARAGRAPH_ID);
	GetSequenceStringIds(MENU_EMS_NEW_PARAGRAPH_ID, nStrItemListIDs);
	for(index=0;index<nNumofItem;index++)
		nStrItemList[index]=(PU8)GetString ((U16)nStrItemListIDs[index]);
	SetMessagesCurrScrnID(SCR_ID_MSG_TEXT_FORMAT_NEW_PARA);
	ShowCategory109Screen (STR_EMS_NEW_PARAGRAPH_ID, IMG_SMS_ENTRY_SCRN_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK, 
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, NULL,
						  currObjectListHiliteIndex, guiBuffer); 
}

/*****************************************************************************
* FUNCTION
*  EntryScrEmsFormatTextMenu
* DESCRIPTION
*   Entry text format screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryScrEmsFormatTextMenu (void)
{
	U16 numItems=0;
	PU8 guiBuffer=NULL;
	U16 nStrItemList[MAX_SUB_MENUS];
	EntryNewScreen(SCR_ID_MSG_TEXT_FORMAT, mmi_msg_exit_generic, EntryScrEmsFormatTextMenu, NULL);

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_TEXT_FORMAT);		 
	SetParentHandler (MENU_EMS_FORMATING_TEXT_ID);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	numItems = GetNumOfChild (MENU_EMS_FORMATING_TEXT_ID);
	GetSequenceStringIds(MENU_EMS_FORMATING_TEXT_ID, nStrItemList);
	SetMessagesCurrScrnID(SCR_ID_MSG_TEXT_FORMAT);
	ShowCategory52Screen (STR_EMS_FORMATING_TEXT_ID, IMG_SMS_ENTRY_SCRN_CAPTION, 
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, (U16 *)gIndexIconsImageList,
						  NULL , 0, 0, guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsFormattingTextHandler
* DESCRIPTION
*   Highlight text format handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsFormattingTextHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryScrEmsFormatTextMenu, KEY_EVENT_UP);
	SetKeyHandler (EntryScrEmsFormatTextMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************
* FUNCTION
*  InitUserDefinedObjectDataPtrs
* DESCRIPTION
*   Init user defined object data pointers
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   userDefinedObjectDataPtrs
*****************************************************************************/
void InitUserDefinedObjectDataPtrs (void)
{
	U8 index=0;
	for(index=0;index<MAX_EMS_OBJECT_LIST_DISPLAY;index++)
		userDefinedObjectDataPtrs[index]=NULL;
}

/*****************************************************************************
* FUNCTION
*  MemsetUserDefinedObjectDataBuff
* DESCRIPTION
*   Reset user defined object data buffer
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void MemsetUserDefinedObjectDataBuff (void)
{
	memset (&userDefinedObjectDataBuff, 0, MAX_CURR_EMS_OBJECT_DATA_BUFF);
}

/*****************************************************************************
* FUNCTION
*  FreeUserEmsObjectNameInputBuff
* DESCRIPTION
*   Reset user defined object name input buffer
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void FreeUserEmsObjectNameInputBuff (void)
{
	memset (&userEmsObjectNameInput, 0, (MAX_EMS_DOWNLOAD_OBJECT_NAME_LEN*ENCODING_LENGTH)+ENCODING_LENGTH);
}

/*****************************************************************************
* FUNCTION
*  MessagesGetString
* DESCRIPTION
*   Get string in UCS2 format
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static S8* MessagesGetString (S8* pathStr)
{
	static S8 emsObjectFileNameStaticBuf[(MAX_EMS_DOWNLOAD_OBJECT_NAME_LEN+1)*ENCODING_LENGTH]={0,0};
	memset(emsObjectFileNameStaticBuf,0, (MAX_EMS_DOWNLOAD_OBJECT_NAME_LEN+1)*ENCODING_LENGTH);
	AnsiiToUnicodeString(emsObjectFileNameStaticBuf, pathStr);
	return emsObjectFileNameStaticBuf;
}

#ifdef __DOWNLOAD__
/*****************************************************************************
* FUNCTION
*  MaxSystemAllowedObjects
* DESCRIPTION
 
*
* PARAMETERS
*  none
* RETURNS
*  object number
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 MaxSystemAllowedObjects (void)
{
	switch (currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_MY_PICTURE_STATE:
			return MAX_EMS_DOWNLOAD_PICTURE_OBJECTS;
		case EMS_OBJECT_MY_ANIMATION_STATE:
			return MAX_EMS_DOWNLOAD_ANIMATION_OBJECTS;
#if defined(__MMI_IMELODY_SUPPORT__)			
		case EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE:
		case EMS_OBJECT_MY_MELODY_STATE:
			return MAX_EMS_DOWNLOAD_IMELODY_OBJECTS;
#endif
		case EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE:
			return MAX_EMS_DOWNLOAD_PICTURE_OBJECTS;

//CSD end
	}
	return 0;
}
#endif

/*****************************************************************************
* FUNCTION
*  GetEmsObjectDataSize
* DESCRIPTION
*   Get object data size
*
* PARAMETERS
*  none
* RETURNS
*  object data size
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 GetEmsObjectDataSize (void)
{
	IsAllowed=FALSE;
	switch (currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_PICTURE_STATE:
			return EMS_check_predefined_picture_data_size (userDefinedObjectDataBuff, &IsAllowed);
		case EMS_OBJECT_MY_PICTURE_STATE:
			return EMS_check_userdefined_picture_size (userDefinedObjectDataBuff, &IsAllowed);
		case EMS_OBJECT_MY_ANIMATION_STATE:
			return EMS_check_userdefined_animation_size (userDefinedObjectDataBuff, &IsAllowed);
#if defined(__MMI_IMELODY_SUPPORT__)			
		case EMS_OBJECT_MELODY_STATE:
			return EMS_check_predefined_melody_data_size (userDefinedObjectDataBuff, &IsAllowed);
		case EMS_OBJECT_MY_MELODY_STATE:
		case EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE:
			return EMS_check_userdefined_melody_size (userDefinedObjectDataBuff, &IsAllowed);
#endif	
		case EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE:
			if(currObjectListHiliteIndex<currMaxEmsPictureObjects)
				return EMS_check_userdefined_picture_size (userDefinedObjectDataBuff, &IsAllowed);
			else
				return EMS_check_userdefined_animation_size (userDefinedObjectDataBuff, &IsAllowed);
		default: 
			return 0;
	}
}


/*****************************************************************************
* FUNCTION
*  myfunc
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ErrorHandlingWhileSavingObject (EMSTATUS emsObjectStatus)
{
	switch(emsObjectStatus)
	{
		case EMS_OK: 
			DisplayPopup ((PU8)GetString (STR_GLOBAL_SAVED), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SAVE_TONE);
			break;
		case EMS_ALREADY_INITIALIZED:
		#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
			DisplayPopup ((PU8)GetString (FMGR_FS_FILE_EXISTS_TEXT),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		#else
			DisplayPopup ((PU8)GetString (STR_EMS_OBJECT_DUPLICATED_NAME_INPUT_AGAIN),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		#endif
			break;
		case EMS_NO_SPACE:
		#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
			DisplayPopup ((PU8)GetString (FMGR_FS_DISK_FULL_TEXT),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		#else
			DisplayPopup ((PU8)GetString (STR_EMS_OBJECT_STAORAGE_FULL),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		#endif
			break;
		default /*EMS_UNSPECIFIED_ERROR*/:
			DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			break;
	}
}

#ifdef __DOWNLOAD__
/*****************************************************************************
* FUNCTION
*  WriteEmsObjectNamesToNvram
* DESCRIPTION
*   Write object name to NVRAM
*
* PARAMETERS
*  writeRecordLID	IN	NVRAM record LID
*  writeRecordSize	IN	NVRAM record size
* RETURNS
*  write resule
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S8 WriteEmsObjectNamesToNvram (U8 writeRecordLID, U8 writeRecordSize)
{
	S16	error=0;
	if (WriteRecord (writeRecordLID, 1,&nvramEmsObjectNames, writeRecordSize, &error) != writeRecordSize)
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[EmsMiscell.c] WriteEmsObjectNamesToNvram - Failed-(code=%d) -------*\n",(U8)error));
	    	return (S8)-1;
	}
	else
	{
		return 0;
	}
	return (S8)-1;
}

/*****************************************************************************
* FUNCTION
*  ReadEmsObjectNamesFromNvram
* DESCRIPTION
*   Read object name from NVRAM
*
* PARAMETERS
*  writeRecordLID	IN	NVRAM record LID
*  writeRecordSize	IN	NVRAM record size
* RETURNS
*  name count
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S8 ReadEmsObjectNamesFromNvram (U8 writeRecordLID, U8 writeRecordSize)
{
	S16	error=0;
	memset(&nvramEmsObjectNames, 0xFF, sizeof(EMS_OBJECT_NAMES_NVRAM_STRUCT));
	if (ReadRecord (writeRecordLID, 1 ,&nvramEmsObjectNames, writeRecordSize, &error) < 0)
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[EmsMiscell.c] ReadEmsObjectNamesFromNvram - (code=%d) -------*\n",(U8)error));
		return 0;
	}
	else 
	{
		S8 count=0;
		U8 index=0;
		U8 maxSystemObjects=MaxSystemAllowedObjects ();
		for(index=0; index<maxSystemObjects; index++)
		{
			if(nvramEmsObjectNames.emsObjectNames[index][0]!=0xFF)	// check the data is there or not.
				count++;
		}
		return count;
	}
}

/*****************************************************************************
* FUNCTION
*  DeleteEmsObjectNameFromNvram
* DESCRIPTION
*   Delete object name from NVRAM
*
* PARAMETERS
*  writeRecordLID	IN	NVRAM record LID
*  writeRecordSize	IN	NVRAM record size
*  index			IN	index
* RETURNS
*  delete result
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S8 DeleteEmsObjectNameFromNvram (U8 writeRecordLID, U8 writeRecordSize, S32 index)
{
	U8 i=0,j=0,total=0;
	switch(currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE:
		{
			memset(&nvramEmsObjectNames, 0xFF, sizeof(EMS_OBJECT_NAMES_NVRAM_STRUCT));
			if(index<currMaxEmsPictureObjects) 	// delete's picture name
			{
				total=currMaxEmsPictureObjects;
				for(i=0; i<total; i++)
				{
					if(i!=index)
					{
						pfnUnicodeStrcpy((S8*)nvramEmsObjectNames.emsObjectNames[j], (S8*)downloadEmsPictureObjectNames[i]);
						j++;
					}
				}
			}
			else								// delete's animation name
			{
				total=currMaxEmsObjectList-currMaxEmsPictureObjects;
				for(i=0; i<total; i++)
				{
					if((i+currMaxEmsPictureObjects)!=index)
					{
						pfnUnicodeStrcpy((S8*)nvramEmsObjectNames.emsObjectNames[j], (S8*)downloadEmsAnimationObjectNames[i]);
						j++;
					}
				}
			}
		}
		break;
//CSD Joy added for customization, 1016
#if defined(__MMI_IMELODY_SUPPORT__)		
		case EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE:
		{
			total=currMaxDownLoadedEmsIMelodies;
			ReadDownloadObjectNamesFromNvram ();
			memset(nvramEmsObjectNames.emsObjectNames[index], 0xFF, sizeof(U8)*MAX_EMS_DISPLAY_NAME_LEN);
			for(i=(U8)index; i<(total-1); i++)
			{
				pfnUnicodeStrcpy((S8*)nvramEmsObjectNames.emsObjectNames[i], (S8*)nvramEmsObjectNames.emsObjectNames[i+1]);
				memset(&nvramEmsObjectNames.emsObjectNames[i+1], 0xFF, sizeof(U8)*MAX_EMS_DISPLAY_NAME_LEN);
			}
		}
		break;
#endif
//CSD end
	}
	if(!WriteEmsObjectNamesToNvram (writeRecordLID, writeRecordSize))
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*  ReadDownloadObjectNamesFromNvram
* DESCRIPTION
*   Read object name from NVRAM
*
* PARAMETERS
*  none
* RETURNS
*  object count
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S8 ReadDownloadObjectNamesFromNvram (void)
{
	S8 currMaxObjects=0;
	switch (currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_MY_PICTURE_STATE:
		   currMaxObjects = ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_PICTURE_NAME_LID, NVRAM_EMS_MY_PICTURE_NAME_RECORD_SIZE);
			break;
		case EMS_OBJECT_MY_ANIMATION_STATE:
			currMaxObjects = ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_ANIMATION_NAME_LID, NVRAM_EMS_MY_ANIMATION_NAME_RECORD_SIZE);	
			break;
//CSD Joy added for customization, 1016
#if defined(__MMI_IMELODY_SUPPORT__)			
		case EMS_OBJECT_MY_MELODY_STATE:
		case EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE:
			currMaxObjects = ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_MELODY_NAME_LID, NVRAM_EMS_MY_MELODY_NAME_RECORD_SIZE);	
			break;
#endif
//CSD end
	}
	return currMaxObjects;
}

/*****************************************************************************
* FUNCTION
*  SaveDownloadObjectNameIntoNvram
* DESCRIPTION
*   Save object name to NVRAM
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SaveDownloadObjectNameIntoNvram (void)
{
	switch (currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_MY_PICTURE_STATE:
			WriteEmsObjectNamesToNvram (NVRAM_EF_EMS_MY_PICTURE_NAME_LID, NVRAM_EMS_MY_PICTURE_NAME_RECORD_SIZE);	
			break;
		case EMS_OBJECT_MY_ANIMATION_STATE:
			WriteEmsObjectNamesToNvram (NVRAM_EF_EMS_MY_ANIMATION_NAME_LID, NVRAM_EMS_MY_ANIMATION_NAME_RECORD_SIZE);	
			break;
//CSD Joy added for customization, 1016
#if defined(__MMI_IMELODY_SUPPORT__)			
		case EMS_OBJECT_MY_MELODY_STATE:
			WriteEmsObjectNamesToNvram (NVRAM_EF_EMS_MY_MELODY_NAME_LID, NVRAM_EMS_MY_MELODY_NAME_RECORD_SIZE);	
			break;
#endif
//CSD end
	}
}

/*****************************************************************************
* FUNCTION
*  DeleteDownloadObjectDataFromFileSystem
* DESCRIPTION
*   Delete object data from file system
*
* PARAMETERS
*  index	IN	delete index
* RETURNS
*  delete result
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 DeleteDownloadObjectDataFromFileSystem (S32 index)
{
	S8 emsObjectFileNameBuf[(MAX_EMS_DOWNLOAD_OBJECT_FILE_LEN+1)*ENCODING_LENGTH]={0,0};


	switch (currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE: // pictures ans animations
			if(index<currMaxEmsPictureObjects)
			{
				pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_PIC_A_ANIM_OBJECT_FILE_NAME_INIT_PATH_ID));
				pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)downloadEmsPictureObjectNames[index]);
				pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_PICTURE_OBJECT_FILE_NAME_EXTN_ID));
			}
			else
			{
				pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_PIC_A_ANIM_OBJECT_FILE_NAME_INIT_PATH_ID));
				pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)downloadEmsAnimationObjectNames[index-currMaxEmsPictureObjects]);
				pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_ANIMATION_OBJECT_FILE_NAME_EXTN_ID));
			}
			break;
#if defined(__MMI_IMELODY_SUPPORT__)			
		case EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE: // Download iMelody
			pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_IMELODY_OBJECT_FILE_NAME_INIT_PATH_ID));
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)emsMelodyInfoStruct[index].AudioName);
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_IMELODY_OBJECT_FILE_NAME_EXTN_ID));
			break;
#endif
	}
	if(pfError(pfdelete(emsObjectFileNameBuf)))
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[EmsMiscell.c] DeleteDownloadObjectDataFromFileSystem - Failed! -------*\n"));
		return FALSE;
	}
	return TRUE;
}
#endif
/*****************************************************************************
* FUNCTION
*  WriteDownloadObjectDataIntoFileSystem
* DESCRIPTION
*   Write object data into file system
*
* PARAMETERS
*  none
* RETURNS
*  write result
* GLOBALS AFFECTED
*   none
*****************************************************************************/
EMSTATUS WriteDownloadObjectDataIntoFileSystem (void)
{
	INT wByte=0;
	INT gOpenError=0;
	INT gWriteResult=0;
	FILE_HANDLE	emsObjectFilePtr=0;
	S8 emsObjectFileNameBuf[(MAX_EMS_DOWNLOAD_OBJECT_FILE_LEN+1)*ENCODING_LENGTH]={0,0};
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
	U16 driveName;
   S8 temp[4];
   UI_character_type dir_ems_root[4];

	driveName = MMI_PUBLIC_DRV;
	sprintf( temp, "%c:\\", (S8) driveName );
	AnsiiToUnicodeString( (S8*)dir_ems_root, temp);
#endif
	switch (currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_MY_PICTURE_STATE: // All Download Objects Only...
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
			pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)dir_ems_root);
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)FMGR_DEFAULT_FOLDER_IMAGES);
#else
			pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_PIC_A_ANIM_OBJECT_FILE_NAME_INIT_PATH_ID));
#endif
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)userEmsObjectNameInput);
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_PICTURE_OBJECT_FILE_NAME_EXTN_ID));
			break;
		case EMS_OBJECT_MY_ANIMATION_STATE: 
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
			pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)dir_ems_root);
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)FMGR_DEFAULT_FOLDER_IMAGES);			
#else
			pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_PIC_A_ANIM_OBJECT_FILE_NAME_INIT_PATH_ID));
#endif
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)userEmsObjectNameInput);
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_ANIMATION_OBJECT_FILE_NAME_EXTN_ID));
			break;
#if defined(__MMI_IMELODY_SUPPORT__)			
		case EMS_OBJECT_MY_MELODY_STATE:
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
			pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)dir_ems_root);
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)FMGR_DEFAULT_FOLDER_AUDIO);			
#else
			pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_IMELODY_OBJECT_FILE_NAME_INIT_PATH_ID));
#endif
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)userEmsObjectNameInput);
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_IMELODY_OBJECT_FILE_NAME_EXTN_ID));
			break;
#endif
	}
	/* duplicated filename */
	emsObjectFilePtr = MMI_FS_Open(emsObjectFileNameBuf,PFS_READ);	
	if(emsObjectFilePtr >= 0)
	{
		pfclose(emsObjectFilePtr);
		return EMS_ALREADY_INITIALIZED;
	}
	emsObjectFilePtr = MMI_FS_Open(emsObjectFileNameBuf, PFS_WRITE);
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[EmsMiscell.c] fopen result = %d -------*", emsObjectFilePtr));
	gOpenError=pfOpenError(emsObjectFilePtr);
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[EmsMiscell.c] fopen error = %d -------*", gOpenError));
	if(!gOpenError)
	{
		gWriteResult = pfwrite(emsObjectFilePtr, userDefinedObjectDataSize, userDefinedObjectDataBuff);
		//gWriteResult=pfwrite (userDefinedObjectDataBuff, 1, userDefinedObjectDataSize, emsObjectFilePtr, &wByte);

		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[EmsMiscell.c] fwrite result = %d -------*", gWriteResult));
		//if(userDefinedObjectDataSize==wByte) success ele failure;
		pfclose (emsObjectFilePtr);
		if (gWriteResult<0)
		{
			pfdelete(emsObjectFileNameBuf);
			return EMS_NO_SPACE;
		}
		else
			return EMS_OK;
	}
	return EMS_UNSPECIFIED_ERROR;
}

/*****************************************************************************
* FUNCTION
*  ReadDownloadObjectDataFromFileSystem
* DESCRIPTION
*   Read object data from file system
*
* PARAMETERS
*  index
* RETURNS
*  read result
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 ReadDownloadObjectDataFromFileSystem (S32 index)
{
#ifdef __DOWNLOAD__				
	INT	errorCode=0;
	FILE_HANDLE	emsObjectFilePtr=0;
	S8 emsObjectFileNameBuf[(MAX_EMS_DOWNLOAD_OBJECT_FILE_LEN+1)*ENCODING_LENGTH]={0,0};
#endif
	switch (currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_PICTURE_STATE:
		{
			U8* imagedata;
			U16 imageindex = IMG_EMS01_PIC+(U16)index;
			U16 imagedatasize;
			MemsetUserDefinedObjectDataBuff ();
			imagedata=(U8*)GetImage(imageindex);
			imagedatasize=9+(*(imagedata+7))|(*(imagedata+8))<<8;
			memcpy(userDefinedObjectDataBuff, imagedata, imagedatasize);
			emsObjectNameWhenPreview = ( U8*)GetString (ManufacturerPictureStrIDs [index]);
			return TRUE;
		}
#ifdef __MMI_IMELODY_SUPPORT__
		case EMS_OBJECT_MELODY_STATE:
		{
			U8 userDefinedObjectDataBuffTemp[MAX_CURR_EMS_OBJECT_DATA_BUFF];
			MemsetUserDefinedObjectDataBuff ();
			memcpy(userDefinedObjectDataBuff, resource_ems_imelodys[index].data, resource_ems_imelodys[index].len);	
			userDefinedObjectDataSize=resource_ems_imelodys[index].len;
			EMS_predefined_melody_attach_header(userDefinedObjectDataBuff, userDefinedObjectDataBuffTemp, &userDefinedObjectDataSize);
			memcpy(userDefinedObjectDataBuff, userDefinedObjectDataBuffTemp, userDefinedObjectDataSize);	
			return TRUE;
		}
#endif //__MMI_IMELODY_SUPPORT__
#ifdef __DOWNLOAD__				
		case EMS_OBJECT_MY_PICTURE_STATE: // Download Picture
			pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_PIC_A_ANIM_OBJECT_FILE_NAME_INIT_PATH_ID));
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)nvramEmsObjectNames.emsObjectNames[index]);
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_PICTURE_OBJECT_FILE_NAME_EXTN_ID));
			emsObjectNameWhenPreview = ( U8*)nvramEmsObjectNames.emsObjectNames[index];
			break;
		case EMS_OBJECT_MY_ANIMATION_STATE: // Download Animation
			pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_PIC_A_ANIM_OBJECT_FILE_NAME_INIT_PATH_ID));
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)nvramEmsObjectNames.emsObjectNames[index]);
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_ANIMATION_OBJECT_FILE_NAME_EXTN_ID));
			emsObjectNameWhenPreview = ( U8*)nvramEmsObjectNames.emsObjectNames[index];
			break;
#if defined(__MMI_IMELODY_SUPPORT__)			
		case EMS_OBJECT_MY_MELODY_STATE: // Download iMelody
			if( (totalEmsDownloadMelody) && (index<totalEmsDownloadMelody))
			{
				pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_IMELODY_OBJECT_FILE_NAME_INIT_PATH_ID));
				pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)nvramEmsObjectNames.emsObjectNames[index]);
				pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_IMELODY_OBJECT_FILE_NAME_EXTN_ID));
			}
			#ifdef __MMI_WAP__
			else if ( (totalWapDownloadMelody) && (index<totalEmsDownloadMelody+totalWapDownloadMelody))
			{  
				S32 objectIndex=index-totalEmsDownloadMelody; 
				GetAudioNameWithPathInFileSystem (downloadMelodyIDs[objectIndex],(S8*)emsObjectFileNameBuf);
			}
			#endif
			#ifdef __MMI_RING_COMPOSER__
			else
			{
			#ifdef __MMI_WAP__
				S32 objectIndex=index-(totalEmsDownloadMelody+totalWapDownloadMelody); 
			#else //__MMI_WAP__
				S32 objectIndex=index-totalEmsDownloadMelody; 
			#endif //__MMI_WAP__
				GetAudioNameWithPathInFileSystem (composedMelodyIDs[objectIndex],(S8*)emsObjectFileNameBuf);
			}
			#endif //__MMI_RING_COMPOSER__
			break;
#endif //__MMI_IMELODY_SUPPORT__
		case EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE: // pictures ans animations
			if(index<currMaxEmsPictureObjects)
			{
				pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_PIC_A_ANIM_OBJECT_FILE_NAME_INIT_PATH_ID));
				pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)downloadEmsPictureObjectNames[index]);
				pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_PICTURE_OBJECT_FILE_NAME_EXTN_ID));
				emsObjectNameWhenPreview = ( U8*)downloadEmsPictureObjectNames [index];
			}
			else
			{
				pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_PIC_A_ANIM_OBJECT_FILE_NAME_INIT_PATH_ID));
				pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)downloadEmsAnimationObjectNames[index-currMaxEmsPictureObjects]);
				pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_ANIMATION_OBJECT_FILE_NAME_EXTN_ID));
				emsObjectNameWhenPreview = ( U8*)downloadEmsAnimationObjectNames[index-currMaxEmsPictureObjects];
			}
			break;
#endif //__DOWNLOAD__
	}
#ifdef __DOWNLOAD__						
	MemsetUserDefinedObjectDataBuff ();
	emsObjectFilePtr = MMI_FS_Open (emsObjectFileNameBuf, PFS_READ);
	if(!pfOpenError(emsObjectFilePtr))
	{
		userDefinedObjectDataSize = GetFSFileSize (emsObjectFilePtr);
		switch(currEmsObjectDisplayListState)
		{
#if defined(__MMI_IMELODY_SUPPORT__)				/* Pending work here ...abt hint for over sized..*/
			case EMS_OBJECT_MY_MELODY_STATE:
			case EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE:
				if(userDefinedObjectDataSize>MAX_EMS_OBJECT_INSERT_SIZE/*128 bytes*/)
				{
					pfclose (emsObjectFilePtr);
					return FALSE;
				}	
#endif
			default:
				if(userDefinedObjectDataSize>MAX_CURR_EMS_OBJECT_DATA_BUFF/* bytes*/)
				{
					pfclose (emsObjectFilePtr);				
					return FALSE;
				}
		}
		//pfread (userDefinedObjectDataBuff, 1, userDefinedObjectDataSize, emsObjectFilePtr, &errorCode);
		pfread (emsObjectFilePtr, userDefinedObjectDataBuff, userDefinedObjectDataSize);

		pfclose (emsObjectFilePtr);
		if(pfError (errorCode)) 
			return FALSE;
		switch (currEmsObjectDisplayListState)
		{
#if defined(__MMI_IMELODY_SUPPORT__)			
			case EMS_OBJECT_MY_MELODY_STATE:
			case EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE:
			{
				U8 userDefinedObjectDataBuffTemp[MAX_CURR_EMS_OBJECT_DATA_BUFF];
				EMS_userdefined_melody_attach_header(userDefinedObjectDataBuff, userDefinedObjectDataBuffTemp, &userDefinedObjectDataSize);
				memcpy(userDefinedObjectDataBuff, userDefinedObjectDataBuffTemp, userDefinedObjectDataSize);
			}
			break;
#endif
		}
		return TRUE;
	}
#endif //__DOWNLOAD__
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*  SetCurrEmsObjectDisplayListState
* DESCRIPTION
*   Set object state
*
* PARAMETERS
*  userDefinedObjectDataType	IN	object data type
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SetCurrEmsObjectDisplayListState (U8 userDefinedObjectDataType)
{
	switch (userDefinedObjectDataType)
	{
		case EMS_USERDEFINED_PICTURE: 
			currEmsObjectDisplayListState=EMS_OBJECT_MY_PICTURE_STATE;
			break;
		case EMS_USERDEFINED_ANIMATION:
			currEmsObjectDisplayListState=EMS_OBJECT_MY_ANIMATION_STATE;
			break;
	//CSD Joy added for customization, 1014
	#if defined(__MMI_IMELODY_SUPPORT__)			
		case EMS_USERDEFINED_MELODY:
			currEmsObjectDisplayListState=EMS_OBJECT_MY_MELODY_STATE;
			break;
	#endif
	//CSD end
	}
}

/*****************************************************************************
* FUNCTION
*  CallBackLoadUserDefinedObjectDataBuffPtr
* DESCRIPTION
*   Call back to get user defined object data
*
* PARAMETERS
*  index	IN	object index
* RETURNS
*  data buffer pointer
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* CallBackLoadUserDefinedObjectDataBuffPtr (S32 index)
{
	if(ReadDownloadObjectDataFromFileSystem (index))
	{
		userDefinedObjectDataSize=GetEmsObjectDataSize ();
		ChangeTitleString (emsObjectNameWhenPreview);
		draw_title();
		return userDefinedObjectDataBuff;
	}
	else
	{
		return NULL;
        }
}


/*****************************************************************************
* FUNCTION
*  GetEmsObjectDataInsertStatus
* DESCRIPTION
*   Get object insert status
*
* PARAMETERS
*  index	IN	object index
* RETURNS
*  insert status
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 GetEmsObjectDataInsertStatus (S32 index)
{
	if(ReadDownloadObjectDataFromFileSystem (index))
	{
		userDefinedObjectDataSize=GetEmsObjectDataSize ();
		listOfIsAllowedData[index]=IsAllowed;
	}
	else
	{	
		IsAllowed=0;
		listOfIsAllowedData[index]=IsAllowed;
	}
	if(IsAllowed)
		return EMS_CAN_INSERT_OBJECT_ID;
	else
		return EMS_CANNOT_INSERT_OBJECT_ID;
}

/*****************************************************************************
* FUNCTION
*  GetEmsObjectDataSizeHint
* DESCRIPTION
*   Get object data size hint
*
* PARAMETERS
*  index	IN	object index
* RETURNS
*  hint data pointer
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* GetEmsObjectDataSizeHint (U8 index)
{
	if(!IsAllowed)
	{
		U8 emsObjectDataSizeHintBuff[10]={'\0','\0'};
		memset(hintData[index], 0, MAX_SUB_MENU_HINT_SIZE);
		AnsiiToUnicodeString((S8*)hintData[index], (S8*)emsObjectDataSizeHintBuff); 
		return hintData[index];
	}
	return NULL;
}

/*****************************************************************************
* FUNCTION
*  GetEmsObjectDisplayListDetails
* DESCRIPTION
*   Get object display list
*
* PARAMETERS
*  lskStrID	IN/OUT	LSK string ID
*  strItemListPtr	IN/OUT string list pointer
*  imgItemListPtr	IN/OUT image list pointer
*  popUpList	IN/OUT	popup list pointer
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GetEmsObjectDisplayListDetails (U16 *lskStrID, U8** strItemListPtr, U16 *imgItemListPtr, U8** popUpList)
{
	U8 index=0;
#if defined(__MMI_IMELODY_SUPPORT__)
	U8 minid=0;
	U8 maxid=0;
#endif
	PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[EmsMiscell.c] GetEmsObjectListDetails -------*\n"));
	switch(currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_PICTURE_STATE:
			currMaxEmsObjectList = EmsPictureNum();	
			*lskStrID=STR_GLOBAL_VIEW;
			for(index=0; index<currMaxEmsObjectList; index++)
			{
				strItemListPtr[index] = (PU8)GetString ((U16)ManufacturerPictureStrIDs[index]);
				imgItemListPtr[index]= GetEmsObjectDataInsertStatus(index);
				popUpList[index] = GetEmsObjectDataSizeHint (index); 
			}
			break;
	#ifdef __DOWNLOAD__		
		case EMS_OBJECT_MY_PICTURE_STATE:
			currMaxEmsObjectList=ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_PICTURE_NAME_LID, NVRAM_EMS_MY_PICTURE_NAME_RECORD_SIZE);
			*lskStrID=STR_GLOBAL_VIEW;	
			for(index=0; index<currMaxEmsObjectList; index++)
			{
				strItemListPtr[index] = nvramEmsObjectNames.emsObjectNames[index];
				imgItemListPtr[index]= GetEmsObjectDataInsertStatus(index);
				popUpList[index] = GetEmsObjectDataSizeHint (index); 
			}
			break;
	#endif //__DOWNLOAD__
		case EMS_OBJECT_PREDEFINED_ANIMATION_STATE:
			currMaxEmsObjectList = MAX_EMS_PREDEFINED_ANIMATION;
			*lskStrID=STR_GLOBAL_VIEW;
			for(index=0; index<currMaxEmsObjectList; index++)
			{
				strItemListPtr[index] = (PU8)GetString ((U16)PredefinedAnimationStrIDs[index]);
				imgItemListPtr[index] = EMS_CAN_INSERT_OBJECT_ID;
				popUpList[index] = NULL;
			}
			break;
	#ifdef __DOWNLOAD__		
		case EMS_OBJECT_MY_ANIMATION_STATE:
			currMaxEmsObjectList=ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_ANIMATION_NAME_LID, NVRAM_EMS_MY_ANIMATION_NAME_RECORD_SIZE);
			*lskStrID=STR_GLOBAL_VIEW;	
			for(index=0; index<currMaxEmsObjectList; index++)
			{
				strItemListPtr[index] = nvramEmsObjectNames.emsObjectNames[index];
				imgItemListPtr[index]= GetEmsObjectDataInsertStatus(index);
				popUpList[index] = GetEmsObjectDataSizeHint (index); 
			}		
			break;
	#endif //__DOWNLOAD__
	#if defined(__MMI_IMELODY_SUPPORT__)			
		case EMS_OBJECT_MELODY_STATE:
			EnableDisableAudioPlayback ();
	#ifdef MMI_ON_HARDWARE_P
			custom_get_fixed_ems_imelody_range(&minid, &maxid);
			currMaxEmsObjectList = (maxid - minid +1);
	#else
			currMaxEmsObjectList = EmsMelodyNum();
	#endif
			*lskStrID=STR_GLOBAL_OK;
			for(index=0; index<currMaxEmsObjectList; index++)
			{
				strItemListPtr[index] = (PU8)GetString ((U16)ManufacturerMelodiesStrIDs[index]);
				imgItemListPtr[index]= GetEmsObjectDataInsertStatus(index);
				popUpList[index] = GetEmsObjectDataSizeHint (index); 
			}
			break;
	#ifdef __DOWNLOAD__		
		case EMS_OBJECT_MY_MELODY_STATE:
			{
				DYNAUDIOINFO *dynAudioInfo=NULL;
				EnableDisableAudioPlayback ();
				currMaxEmsObjectList=0;
				*lskStrID=STR_GLOBAL_OK;
				totalEmsDownloadMelody=ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_MELODY_NAME_LID, NVRAM_EMS_MY_MELODY_NAME_RECORD_SIZE);
				for(index=0; index<totalEmsDownloadMelody; index++)
				{
					strItemListPtr[index] = nvramEmsObjectNames.emsObjectNames[index];
					imgItemListPtr[index]= GetEmsObjectDataInsertStatus(index);
					popUpList[index] = GetEmsObjectDataSizeHint (index); 
				}				
				currMaxEmsObjectList+=totalEmsDownloadMelody;
	#if defined(__MMI_WAP__)
				dynAudioInfo=NULL;
				totalWapDownloadMelody=GetDownloadIMelodyInfo (&dynAudioInfo);
				for(index=0; index<totalWapDownloadMelody; index++)
				{
					downloadMelodyIDs[index]=dynAudioInfo[index].AudioId;
					strItemListPtr[index+currMaxEmsObjectList] = dynAudioInfo[index].AudioName;
					imgItemListPtr[index+currMaxEmsObjectList]= GetEmsObjectDataInsertStatus((S32)(index+currMaxEmsObjectList));
					popUpList[index+currMaxEmsObjectList] = GetEmsObjectDataSizeHint ((U8)(index+currMaxEmsObjectList));  
				}
				currMaxEmsObjectList+=totalWapDownloadMelody;
	#endif //__MMI_WAP__
	#if defined(__MMI_RING_COMPOSER__)
				dynAudioInfo=NULL;
				totalComposedMelody=GetComposedIMelodyInfo (&dynAudioInfo);
				for(index=0; index<totalComposedMelody; index++)
				{
					composedMelodyIDs[index]=dynAudioInfo[index].AudioId;
					strItemListPtr[index+currMaxEmsObjectList] = dynAudioInfo[index].AudioName;
					imgItemListPtr[index+currMaxEmsObjectList]= GetEmsObjectDataInsertStatus((S32)index+currMaxEmsObjectList);
					popUpList[index+currMaxEmsObjectList] = GetEmsObjectDataSizeHint ((U8)(index+currMaxEmsObjectList)); 
				}
				currMaxEmsObjectList+=totalComposedMelody;
	#endif //__MMI_RING_COMPOSER__
			}
			break;
	#endif //__DOWNLOAD__
	#endif //__MMI_IMELODY_SUPPORT__
		case EMS_OBJECT_PREDEFINED_SOUND_STATE:
			currMaxEmsObjectList = MAX_EMS_PREDEFINED_SOUND;
			*lskStrID=STR_GLOBAL_OK;
			for(index=0; index<currMaxEmsObjectList; index++)
			{
				strItemListPtr[index] = (PU8)GetString ((U16)PredefinedSoundStrIDs[index]);
				imgItemListPtr[index] = EMS_CAN_INSERT_OBJECT_ID;
				popUpList[index] = NULL;
			}
			break;
	#ifdef __DOWNLOAD__
		case EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE:
			currMaxEmsObjectList=0;
			*lskStrID=STR_GLOBAL_VIEW;
			currMaxEmsPictureObjects=ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_PICTURE_NAME_LID, NVRAM_EMS_MY_PICTURE_NAME_RECORD_SIZE);
			if(currMaxEmsPictureObjects>0)
			{
				AllocDownloadEmsPictureObjectNamesBuff (currMaxEmsPictureObjects);
				for(index=0; index<currMaxEmsPictureObjects; index++)
				{
					pfnUnicodeStrcpy ((S8*)downloadEmsPictureObjectNames[index], (S8*)nvramEmsObjectNames.emsObjectNames[index]);
					strItemListPtr[index] = downloadEmsPictureObjectNames[index];
					imgItemListPtr[index]= gIndexIconsImageList[index];
					popUpList[index] = NULL;
				}
			}
			currMaxEmsObjectList=ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_ANIMATION_NAME_LID, NVRAM_EMS_MY_ANIMATION_NAME_RECORD_SIZE);
			if(currMaxEmsObjectList>0)
			{
				AllocDownloadEmsAnimationObjectNamesBuff (currMaxEmsObjectList);
				for(index=0; index<currMaxEmsObjectList; index++)
				{
					pfnUnicodeStrcpy ((S8*)downloadEmsAnimationObjectNames[index], (S8*)nvramEmsObjectNames.emsObjectNames[index]);
					strItemListPtr[index+currMaxEmsPictureObjects] = downloadEmsAnimationObjectNames[index];
					imgItemListPtr[index+currMaxEmsPictureObjects]= gIndexIconsImageList[index+currMaxEmsPictureObjects];
					popUpList[index+currMaxEmsPictureObjects] = NULL;
				}
			}
			currMaxEmsObjectList+=currMaxEmsPictureObjects;
			break;
	#endif //__DOWNLOAD__
		default:
			currMaxEmsObjectList = 0;			
			break;
	}
}

/*****************************************************************************
* FUNCTION
*  EntryEMSObjectList
* DESCRIPTION
*   Entry object list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern void ShowCategoryEMSScreen(U16 title,U16 title_icon,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,S32 num_of_symbols,UI_image_ID_type* symbols,UI_string_ID_type * descriptions,S32 highlighted_item,U8* history_buffer);
extern U16 EMS_predefined_images[];
extern U16 EMS_predefined_animations[];
void EntryEMSObjectList (void)
{
	U16 lskStrID=0;
	U8* guiBuffer=NULL;
	U8* strItemList[MAX_EMS_OBJECT_LIST_DISPLAY
	//CSD Joy added for customization, 1014
	#if defined(__MMI_IMELODY_SUPPORT__)		
		+MAXIMUM_DOWNLOAD_AUDIO
		+MAXIMUM_COMPOSED_RINGTONE
	#endif
	//CSD end
		];
	U16 imgItemList[MAX_EMS_OBJECT_LIST_DISPLAY
	//CSD Joy added for customization, 1014
	#if defined(__MMI_IMELODY_SUPPORT__)			
		+MAXIMUM_DOWNLOAD_AUDIO
		+MAXIMUM_COMPOSED_RINGTONE
	#endif
	//CSD end
		];
	U8*	popUpList[MAX_EMS_OBJECT_LIST_DISPLAY
	//CSD Joy added for customization, 1014
	#if defined(__MMI_IMELODY_SUPPORT__)		
		+MAXIMUM_DOWNLOAD_AUDIO
		+MAXIMUM_COMPOSED_RINGTONE
	#endif
	//CSD end
		];

#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
	U16 driveName;
   S8 temp[4];
   UI_character_type dir_ems_root[4];
	S8 emsObjectFileNameBuf[(MAX_EMS_DOWNLOAD_OBJECT_FILE_LEN+1)*ENCODING_LENGTH]={0,0};
	FMGR_FILTER filter;

	driveName = MMI_PUBLIC_DRV;
	sprintf( temp, "%c:\\", (S8) driveName );
	AnsiiToUnicodeString( (S8*)dir_ems_root, temp);

	pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)dir_ems_root);
#endif

	
	switch (currEmsObjectDisplayListState)
	{
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
		case EMS_OBJECT_MY_PICTURE_STATE:
			/* [Bear] 2004/6/23 - begin */
			FMGR_FILTER_INIT(&filter);
			FMGR_FILTER_SET(&filter, FMGR_TYPE_EMS);
			FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
			FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER_DOT);

			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)FMGR_DEFAULT_FOLDER_IMAGES);
			mmi_fmgr_select_path_and_enter(APP_MESSAGES, FMGR_SELECT_FILE, filter, 
					(PS8)emsObjectFileNameBuf, (PsExtFuncPtr)mmi_ems_preview_object_from_fm);
			/* [Bear] 2004/6/23 - end */
			break;
		case EMS_OBJECT_MY_ANIMATION_STATE:
			/* [Bear] 2004/6/23 - begin */
			FMGR_FILTER_INIT(&filter);
			FMGR_FILTER_SET(&filter, FMGR_TYPE_ANM);
			FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
			FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER_DOT);

			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)FMGR_DEFAULT_FOLDER_IMAGES);
			mmi_fmgr_select_path_and_enter(APP_MESSAGES, FMGR_SELECT_FILE, filter, 
					(PS8)emsObjectFileNameBuf, (PsExtFuncPtr)mmi_ems_preview_object_from_fm);			
			/* [Bear] 2004/6/23 - end */
			break;
		#if defined(__MMI_IMELODY_SUPPORT__)
		case EMS_OBJECT_MY_MELODY_STATE:
			/* [Bear] 2004/6/23 - begin */
			FMGR_FILTER_INIT(&filter);
			FMGR_FILTER_SET(&filter, FMGR_TYPE_IMY);
			FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
			FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER_DOT);

			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)FMGR_DEFAULT_FOLDER_AUDIO);
			mmi_fmgr_select_path_and_enter(APP_MESSAGES, FMGR_SELECT_FILE, filter, 
					(PS8)emsObjectFileNameBuf, (PsExtFuncPtr)mmi_ems_preview_object_from_fm);			
			break;
		#endif	
#else
		case EMS_OBJECT_MY_PICTURE_STATE:
		case EMS_OBJECT_MY_ANIMATION_STATE:
		#if defined(__MMI_IMELODY_SUPPORT__)
		case EMS_OBJECT_MY_MELODY_STATE:
		#endif
#endif
		case EMS_OBJECT_PICTURE_STATE:
		case EMS_OBJECT_MELODY_STATE:
		case EMS_OBJECT_PREDEFINED_SOUND_STATE:
		#if !defined( __MMI_UI_STYLE_3__) && !defined(__MMI_UI_STYLE_4__) 
		case EMS_OBJECT_PREDEFINED_ANIMATION_STATE:
		#endif	
				
	GetEmsObjectDisplayListDetails (&lskStrID, strItemList, imgItemList, popUpList);
	if(currMaxEmsObjectList>0)
	{
		EntryNewScreen(SCR_ID_MSG_EMS_OBJECT_LIST, ExitEMSObjectList, EntryEMSObjectList, NULL);		
#if defined(__MMI_IMELODY_SUPPORT__)	&& defined(__DOWNLOAD__)	
		playCurrIMelodyId=0;
#endif
		RegisterHighlightHandler (GetEmsObjectHiliteIndex);	
		guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_EMS_OBJECT_LIST);
		if(guiBuffer!=NULL)
			change_list_menu_category_history (guiBuffer, currObjectListHiliteIndex, currMaxEmsObjectList, 0);
		else
			currObjectListHiliteIndex=0;
		SetMessagesCurrScrnID(SCR_ID_MSG_EMS_OBJECT_LIST);
		ShowCategory53Screen ( STR_EMS_OBJECT_LIST, IMG_SMS_ENTRY_SCRN_CAPTION, 
							   lskStrID, IMG_GLOBAL_OK,
							   STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							   currMaxEmsObjectList, (U8 **)strItemList, imgItemList,
							   (U8 **) popUpList , 0, currObjectListHiliteIndex, guiBuffer );
		SetLeftSoftkeyFunction (EntryScrPreviewEmsObject, KEY_EVENT_UP);
		SetKeyHandler (EntryScrPreviewEmsObject, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
		SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	}
	else
	{
		DisplayPopup ((PU8)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
	}
		break;
		#if defined( __MMI_UI_STYLE_3__) || defined(__MMI_UI_STYLE_4__) 
		case EMS_OBJECT_PREDEFINED_ANIMATION_STATE:
		{
			UI_image_ID_type* imglist=(currEmsObjectDisplayListState==EMS_OBJECT_PICTURE_STATE)?
								 EMS_predefined_images : EMS_predefined_animations;
			UI_string_ID_type* strlist=(currEmsObjectDisplayListState==EMS_OBJECT_PICTURE_STATE)?
								ManufacturerPictureStrIDs : PredefinedAnimationStrIDs;
			S32 objectcount=(currEmsObjectDisplayListState==EMS_OBJECT_PICTURE_STATE)?
				EmsPictureNum() : MAX_EMS_PREDEFINED_ANIMATION;
			EntryNewScreen(SCR_ID_MSG_EMS_OBJECT_LIST, mmi_msg_exit_generic, EntryEMSObjectList, NULL);		
			guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_EMS_OBJECT_LIST);
			if(guiBuffer==NULL)
				currObjectListHiliteIndex=0;
			SetMessagesCurrScrnID(SCR_ID_MSG_EMS_OBJECT_LIST);
			RegisterHighlightHandler(GetEmsObjectHiliteIndex);
			ShowCategoryEMSScreen(STR_EMS_OBJECT_LIST,IMG_SMS_ENTRY_SCRN_CAPTION,
					STR_GLOBAL_OK,IMG_GLOBAL_OK,STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
					objectcount,
					imglist, strlist,
					currObjectListHiliteIndex,guiBuffer);
			SetLeftSoftkeyFunction (PreviewEmsObjectIsDone, KEY_EVENT_UP);
			SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
		}
		break;
		#endif
	}
   }

/*****************************************************************************
* FUNCTION
*  ExitEMSObjectList
* DESCRIPTION
*   Exit object list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ExitEMSObjectList (void)
{
	//history currHistory;
	currObjectListHiliteIndex=GetHighlightedItem ();
	StopEmsObjectIMelodyAndSoundIfAny ();
//	currHistory.scrnID = SCR_ID_MSG_EMS_OBJECT_LIST;
	g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=SCR_ID_MSG_EMS_OBJECT_LIST;
	//currHistory.entryFuncPtr = EntryEMSObjectList;
	//memset (currHistory.inputBuffer, 0, ENCODING_LENGTH);
	//GetCategoryHistory (currHistory.guiBuffer);
	//AddHistory (currHistory);
}

/*****************************************************************************
* FUNCTION
*  RegisterLskAndRightArrowHandler
* DESCRIPTION
*   Register LSK handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static void RegisterLskAndRightArrowHandler (void)
{
#ifdef __DOWNLOAD__
	if(currEmsObjectDisplayListState==EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE)
	{
		mmi_msg_set_confirm_screen(STR_GLOBAL_DELETE, DeleteEmsDownloadObjectFrmFunAndGames, GoBackHistory);
		SetLeftSoftkeyFunction (mmi_msg_entry_confirm_generic, KEY_EVENT_UP);
	}
	else
#endif		
		SetLeftSoftkeyFunction (PreviewEmsObjectIsDone, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  RegisterEndKeyForPreviewEmsObjectScrnHandler
* DESCRIPTION
*   Register End key handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static void RegisterEndKeyForPreviewEmsObjectScrnHandler (void)
{
#ifdef __DOWNLOAD__
	if(currEmsObjectDisplayListState==EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE)
	{
		GetCurrEndKeyDownHandler ();
		SetKeyHandler (GotoIdleAfterFreeDownloadEmsPictureAnimtaionObjectNamesBuff, KEY_END, KEY_EVENT_DOWN);
	}
#endif	
}

/*****************************************************************************
* FUNCTION
*  GetLskStringID
* DESCRIPTION
*   Get LSK string
*
* PARAMETERS
*  none
* RETURNS
*  LSK string
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static U16 GetLskStringID (void)
{
#ifdef __DOWNLOAD__
	if(currEmsObjectDisplayListState==EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE)
		return STR_GLOBAL_DELETE;
	else
#endif		
		return STR_GLOBAL_OK;
}

/*****************************************************************************
* FUNCTION
*  GetLskImageID
* DESCRIPTION
*   Get LSK image
*
* PARAMETERS
*  none
* RETURNS
*  LSK image
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static U16 GetLskImageID (void)
{
#ifdef __DOWNLOAD__
	if(currEmsObjectDisplayListState==EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE)
		return 0;
	else
#endif		
		return IMG_GLOBAL_OK;
}

/*****************************************************************************
* FUNCTION
*  EntryScrPreviewEmsObject
* DESCRIPTION
*   Entry preview object screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryScrPreviewEmsObject (void)
{
	U32 objectType=0;
	U8* guiBuffer=NULL;
	U8 isPictureOrAnimation=FALSE;
	switch (currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_PICTURE_STATE:
			objectType=EMS_PREDEFINED_PICTURE;
			isPictureOrAnimation=TRUE;
			break;
		case EMS_OBJECT_MY_PICTURE_STATE:
			objectType=EMS_USERDEFINED_PICTURE;
			isPictureOrAnimation=TRUE;
			break;
		case EMS_OBJECT_PREDEFINED_ANIMATION_STATE:
			objectType=EMS_PREDEFINED_ANIMATION;
			isPictureOrAnimation=TRUE;
			break;
		case EMS_OBJECT_MY_ANIMATION_STATE:
			objectType=EMS_USERDEFINED_ANIMATION;
			isPictureOrAnimation=TRUE;
			break;
	#if defined(__MMI_IMELODY_SUPPORT__)			
		case EMS_OBJECT_MELODY_STATE:
			objectType=EMS_PREDEFINED_MELODY|CATEGORY144_DISABLE_PREVIEW;
			isPictureOrAnimation=FALSE;
			break;
		case EMS_OBJECT_MY_MELODY_STATE:
			objectType=EMS_USERDEFINED_MELODY|CATEGORY144_DISABLE_PREVIEW;
			isPictureOrAnimation=FALSE;
			break;
	#endif
		case EMS_OBJECT_PREDEFINED_SOUND_STATE:
			objectType=EMS_PREDEFINED_SOUND|CATEGORY144_DISABLE_PREVIEW;
			isPictureOrAnimation=FALSE;
			break;
	#ifdef __DOWNLOAD__
		case EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE:
			objectType=0;
			isPictureOrAnimation=TRUE;
			break;
	#endif //__DOWNLOAD__
	}
	if(isPictureOrAnimation==TRUE)
	{
		EntryNewScreen(SCR_ID_MSG_EMS_PREVIEW_OBJECT, mmi_msg_exit_generic, EntryScrPreviewEmsObject, NULL);
		guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_EMS_PREVIEW_OBJECT);	
		if (currEmsObjectDisplayListState==EMS_OBJECT_PREDEFINED_ANIMATION_STATE)
		{
			emsObjectNameWhenPreview = ( U8*)GetString (PredefinedAnimationStrIDs [currObjectListHiliteIndex]);
			RegisterHighlightHandler (ChangeEmsObjectTitleWithIndex);
		}
		else
			RegisterCategory144ObjectLoader (CallBackLoadUserDefinedObjectDataBuffPtr);
		SetMessagesCurrScrnID(SCR_ID_MSG_EMS_PREVIEW_OBJECT);
		ShowCategory144Screen (emsObjectNameWhenPreview, IMG_SMS_ENTRY_SCRN_CAPTION,
							  			GetLskStringID (), GetLskImageID (), 
							  			STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  			objectType, currMaxEmsObjectList,
							  			NULL, preDefinedObjectIds, FALSE, userDefinedObjectDataPtrs,
							  			(PS32)&currObjectListHiliteIndex, guiBuffer); 
		RegisterLskAndRightArrowHandler ();
		RegisterEndKeyForPreviewEmsObjectScrnHandler ();
		SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	}
	else
	{
		ExitCategoryFunction ();
		RegisterCategory144ObjectLoader (CallBackLoadUserDefinedObjectDataBuffPtr);
		ShowCategory144Screen ((PU8)GetString(STR_PREVIEW_EMS_OBJECT), IMG_SMS_ENTRY_SCRN_CAPTION,
							  STR_GLOBAL_OK, IMG_GLOBAL_OK, 
							  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  objectType, currMaxEmsObjectList,
							  NULL, preDefinedObjectIds, FALSE, userDefinedObjectDataPtrs,
							  (PS32)&currObjectListHiliteIndex, guiBuffer); 
		PreviewEmsObjectIsDone ();
	}
}

/*****************************************************************************
* FUNCTION
*  ChangeEmsObjectTitleWithIndex
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  index	IN	object index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ChangeEmsObjectTitleWithIndex (S32 index)
{
	emsObjectNameWhenPreview = ( U8*)GetString (PredefinedAnimationStrIDs [index]);
	ChangeTitleString (emsObjectNameWhenPreview);
	draw_title();
}

/*****************************************************************************
* FUNCTION
*  GetEmsObjectHiliteIndex
* DESCRIPTION
*   Get object highlight index
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GetEmsObjectHiliteIndex (S32 index)
{
	currObjectListHiliteIndex = index;
	switch (currEmsObjectDisplayListState)
	{
	#if defined(__MMI_IMELODY_SUPPORT__)	
		case EMS_OBJECT_MELODY_STATE:
			StopTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID);
			EMSObjectStopPlayBack ();
			if(ReadDownloadObjectDataFromFileSystem (index))
				StartTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID, MESSAGES_ONE_SEC_TIME_OUT, PlayEmsManufacturerIMelody);			
			break;
	#ifdef __DOWNLOAD__
		case EMS_OBJECT_MY_MELODY_STATE:
			StopTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID);
			if( (totalEmsDownloadMelody) && (index<totalEmsDownloadMelody))
			{
				EMSObjectStopPlayBack();
				if(playCurrIMelodyId) StopAudioFileSystem (playCurrIMelodyId);
				if(ReadDownloadObjectDataFromFileSystem (index))
					StartTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID, MESSAGES_ONE_SEC_TIME_OUT, PlayEmsUserDefinedIMelody);			
			}
			#ifdef __MMI_WAP__
			else if ( (totalWapDownloadMelody) && (index<totalEmsDownloadMelody+totalWapDownloadMelody))
			{ 
				S32 objectIndex=currObjectListHiliteIndex-totalEmsDownloadMelody; 
				EMSObjectStopPlayBack();
				if(playCurrIMelodyId) StopAudioFileSystem (playCurrIMelodyId); 
					StartTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID, MESSAGES_ONE_SEC_TIME_OUT, PlayWapDownlaodIMelody);
				playCurrIMelodyId=downloadMelodyIDs[objectIndex];
			}
			#endif //__MMI_WAP__
			#ifdef __MMI_RING_COMPOSER__
			else
			{
				#ifdef __MMI_WAP__
				S32 objectIndex=currObjectListHiliteIndex-(totalEmsDownloadMelody+totalWapDownloadMelody); 
				#else //__MMI_WAP__
				S32 objectIndex=currObjectListHiliteIndex-totalEmsDownloadMelody; 
				#endif //__MMI_WAP__
				EMSObjectStopPlayBack();
				if(playCurrIMelodyId) StopAudioFileSystem (playCurrIMelodyId); 
					StartTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID, MESSAGES_ONE_SEC_TIME_OUT, PlayUserComposeIMelody);			
				playCurrIMelodyId=composedMelodyIDs[objectIndex];
			}
			#endif //__MMI_RING_COMPOSER__
			break;
	#endif
	#endif //__MMI_IMELODY_SUPPORT__
		case EMS_OBJECT_PREDEFINED_SOUND_STATE:
			StopTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID);
			EMSObjectStopPlayBack ();
			StartTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID, MESSAGES_ONE_SEC_TIME_OUT, PlayEmsManufacturerSound);			
			break;
	}
}

//CSD Joy added for customization, 1014
#if defined(__MMI_IMELODY_SUPPORT__)
#ifdef __DOWNLOAD__
/*****************************************************************************
* FUNCTION
*  PlayWapDownlaodIMelody
* DESCRIPTION
*   Play wap download melody
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PlayWapDownlaodIMelody (void)
{
	S32 objectIndex=currObjectListHiliteIndex-totalEmsDownloadMelody;

	if( !IsSilentModeActivated() )
#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsMeetingModeActivated() )
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
	PlayAudioFileSystem (downloadMelodyIDs[objectIndex], DEVICE_AUDIO_PLAY_ONCE);
}
/*****************************************************************************
* FUNCTION
*  PlayUserComposeIMelody
* DESCRIPTION
*   Play user composed melody
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PlayUserComposeIMelody (void)
{
	S32 objectIndex=currObjectListHiliteIndex-(totalEmsDownloadMelody+totalWapDownloadMelody); 

	if( !IsSilentModeActivated() )
#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsMeetingModeActivated() )
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
	PlayAudioFileSystem (composedMelodyIDs[objectIndex], DEVICE_AUDIO_PLAY_ONCE);
}

/*****************************************************************************
* FUNCTION
*  PlayEmsUserDefinedIMelody
* DESCRIPTION
*   Play user defined melody
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PlayEmsUserDefinedIMelody (void)
{
	StopTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID);
	EMSObjectStopPlayBack ();

	if( !IsSilentModeActivated() )
#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsMeetingModeActivated() )
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
	EMSObjectPlayback (EMS_USERDEFINED_MELODY, userDefinedObjectDataBuff, -1);
}
#endif
/*****************************************************************************
* FUNCTION
*  PlayEmsManufacturerIMelody
* DESCRIPTION
*   Play manufacturer melody
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PlayEmsManufacturerIMelody (void)
{
	StopTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID);
	EMSObjectStopPlayBack ();

	if( !IsSilentModeActivated() )
#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsMeetingModeActivated() )
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
	EMSObjectPlayback (EMS_PREDEFINED_MELODY, userDefinedObjectDataBuff, -1);
}
#endif
//CSD end
/*****************************************************************************
* FUNCTION
*  PlayEmsManufacturerSound
* DESCRIPTION
*   Play manufacturer sound
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PlayEmsManufacturerSound (void)
{
	StopTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID);
	EMSObjectStopPlayBack ();

	if( !IsSilentModeActivated() )
#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsMeetingModeActivated() )
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
	EMSObjectPlayback (EMS_PREDEFINED_SOUND, NULL, currObjectListHiliteIndex);
}

/*****************************************************************************
* FUNCTION
*  StopEmsObjectIMelodyAndSoundIfAny
* DESCRIPTION
*   Stop melody/sound
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void StopEmsObjectIMelodyAndSoundIfAny (void)
{
	switch (currEmsObjectDisplayListState)
	{
#if defined(__MMI_IMELODY_SUPPORT__)	
		case EMS_OBJECT_MELODY_STATE:
			StopTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID);
			EMSObjectStopPlayBack ();
			break;
#ifdef __DOWNLOAD__
		case EMS_OBJECT_MY_MELODY_STATE:
		{
			StopTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID);
			if( (totalEmsDownloadMelody) && (currObjectListHiliteIndex<totalEmsDownloadMelody))
			{
				EMSObjectStopPlayBack ();
			}
			else if ( (totalWapDownloadMelody) && (currObjectListHiliteIndex<totalEmsDownloadMelody+totalWapDownloadMelody))
			{
				if(playCurrIMelodyId) StopAudioFileSystem (playCurrIMelodyId); 
				playCurrIMelodyId=0;
			}
			else if( (totalComposedMelody) && (currObjectListHiliteIndex<totalEmsDownloadMelody+totalWapDownloadMelody+totalComposedMelody))
			{
				if(playCurrIMelodyId) StopAudioFileSystem (playCurrIMelodyId); 
				playCurrIMelodyId=0;
			}
		}
		break;
#endif
#endif
		case EMS_OBJECT_PREDEFINED_SOUND_STATE:
			StopTimer (MESSAGES_EMS_OBJECT_PLAY_TIMER_ID);
			EMSObjectStopPlayBack ();
			break;
	}
}

/*****************************************************************************
* FUNCTION
*  PreviewEmsObjectIsDone
* DESCRIPTION
*   After preview, add object to message body
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PreviewEmsObjectIsDone (void)
{
	EMSTATUS emsObjectStatus;
	if( (currEmsObjectDisplayListState!=EMS_OBJECT_PREDEFINED_ANIMATION_STATE) && 
		(currEmsObjectDisplayListState!=EMS_OBJECT_PREDEFINED_SOUND_STATE) &&
		(!listOfIsAllowedData[currObjectListHiliteIndex]))
		DisplayPopup ((PU8)GetString (STR_EMS_OBJECT_SIZE_NOT_ALLOWED_ID), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
	else
	{
		switch (currEmsObjectDisplayListState)
		{
			case EMS_OBJECT_PICTURE_STATE:
					emsObjectStatus=EMS_inputbox_insert_predefined_picture (currObjectListHiliteIndex);
				break;
			case EMS_OBJECT_MY_PICTURE_STATE:
					emsObjectStatus=EMS_inputbox_insert_userdefined_picture (userDefinedObjectDataBuff);
				break;
			case EMS_OBJECT_PREDEFINED_ANIMATION_STATE:
					emsObjectStatus=EMS_inputbox_insert_predefined_animation (currObjectListHiliteIndex);
				break;
			case EMS_OBJECT_MY_ANIMATION_STATE:
					emsObjectStatus=EMS_inputbox_insert_userdefined_animation (userDefinedObjectDataBuff);
				break;
			#if defined(__MMI_IMELODY_SUPPORT__)				
			case EMS_OBJECT_MELODY_STATE:
					emsObjectStatus=EMS_inputbox_insert_predefined_melody (currObjectListHiliteIndex);
				break;
			case EMS_OBJECT_MY_MELODY_STATE:
					emsObjectStatus=EMS_inputbox_insert_userdefined_melody (userDefinedObjectDataBuff);
				break;
			#endif
			case EMS_OBJECT_PREDEFINED_SOUND_STATE:
					emsObjectStatus=EMS_inputbox_insert_predefined_sound (currObjectListHiliteIndex);
				break;
			default:
					emsObjectStatus=EMS_inputbox_insert_predefined_picture (currObjectListHiliteIndex);
				break;			
		}

		GoBackToHistory (SCR_ID_MSG_WRITE);
		ErrorHandlingWhileInsertingObject (emsObjectStatus); // check whetehr it is inserted or not.
	}
}

/*****************************************************************************
* FUNCTION
*  EntryScrInsertObjectMenu
* DESCRIPTION
*   Entry insert object screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryScrInsertObjectMenu (void)
{
	U16 numItems=0;
	PU8 guiBuffer=NULL;
	U16 nStrItemList[MAX_SUB_MENUS];
	EntryNewScreen(SCR_ID_MSG_EMS_INSERT_OBJECT, mmi_msg_exit_generic, EntryScrInsertObjectMenu, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_EMS_INSERT_OBJECT);		 
	SetParentHandler (EMS_ED_OPT_INS_OBJ);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	numItems = GetNumOfChild (EMS_ED_OPT_INS_OBJ);
	GetSequenceStringIds(EMS_ED_OPT_INS_OBJ, nStrItemList);
	SetMessagesCurrScrnID(SCR_ID_MSG_EMS_INSERT_OBJECT);
	ShowCategory52Screen (STR_OPT_INS_OBJECT_MENUENTRY, IMG_SMS_ENTRY_SCRN_CAPTION, 
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, (U16 *)gIndexIconsImageList,
						  NULL , 0, 0, guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  HighlightEditOptionInsertObject
* DESCRIPTION
*   Highlight insert object handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEditOptionInsertObject (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryScrInsertObjectMenu, KEY_EVENT_UP);
	SetKeyHandler (EntryScrInsertObjectMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightInsObjPicture
* DESCRIPTION
*   Highlight insert picture handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightInsObjPicture (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryEMSObjectList, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	currEmsObjectDisplayListState=EMS_OBJECT_PICTURE_STATE;
	currObjectListHiliteIndex=0;
}

/*****************************************************************************
* FUNCTION
*  HighlightInsObjMyPicture
* DESCRIPTION
*   Highlight insert my picture handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightInsObjMyPicture (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryEMSObjectList, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	currEmsObjectDisplayListState=EMS_OBJECT_MY_PICTURE_STATE;
}

/*****************************************************************************
* FUNCTION
*  HighlightInsObjPredefinedAnimation
* DESCRIPTION
*   Highlight insert predefined animation handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightInsObjPredefinedAnimation (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryEMSObjectList, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	currEmsObjectDisplayListState=EMS_OBJECT_PREDEFINED_ANIMATION_STATE;
	currMaxEmsObjectList=MAX_EMS_PREDEFINED_ANIMATION;
}

/*****************************************************************************
* FUNCTION
*  HighlightInsObjMyAnimation
* DESCRIPTION
*   Highlight insert my animation handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightInsObjMyAnimation (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryEMSObjectList, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	currEmsObjectDisplayListState=EMS_OBJECT_MY_ANIMATION_STATE;
}

/*****************************************************************************
* FUNCTION
*  HighlightInsObjMelody
* DESCRIPTION
*   Highlight insert melody handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
//CSD Joy added for customization, 1014
#if defined(__MMI_IMELODY_SUPPORT__)
void HighlightInsObjMelody (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryEMSObjectList, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	currEmsObjectDisplayListState=EMS_OBJECT_MELODY_STATE;
}

/*****************************************************************************
* FUNCTION
*  HighlightInsObjMyMelody
* DESCRIPTION
*   Highlight insert my melody handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightInsObjMyMelody (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryEMSObjectList, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	currEmsObjectDisplayListState=EMS_OBJECT_MY_MELODY_STATE;
}
#endif
//CSD end
/*****************************************************************************
* FUNCTION
*  HighlightInsObjPredefinedSound
* DESCRIPTION
*   Highlight insert predefined sound handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightInsObjPredefinedSound (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (EntryEMSObjectList, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	currEmsObjectDisplayListState=EMS_OBJECT_PREDEFINED_SOUND_STATE;
}
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
/*****************************************************************************
* FUNCTION
*  mmi_ems_add_object_from_fm
* DESCRIPTION
*   Add object from file manager
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_ems_add_object_from_fm(void)
{
	EMSTATUS emsObjectStatus;
	userDefinedObjectDataSize=GetEmsObjectDataSize();
	if(IsAllowed==TRUE)
	{
		switch (currEmsObjectDisplayListState)
		{
			case EMS_OBJECT_MY_PICTURE_STATE:
					emsObjectStatus=EMS_inputbox_insert_userdefined_picture(userDefinedObjectDataBuff);
				break;
			case EMS_OBJECT_MY_ANIMATION_STATE:
					emsObjectStatus=EMS_inputbox_insert_userdefined_animation(userDefinedObjectDataBuff);
				break;
			#if defined(__MMI_IMELODY_SUPPORT__)				
			case EMS_OBJECT_MY_MELODY_STATE:
					emsObjectStatus=EMS_inputbox_insert_userdefined_melody(userDefinedObjectDataBuff);
				break;
			#endif
			default:
					emsObjectStatus=EMS_inputbox_insert_userdefined_picture(userDefinedObjectDataBuff);
				break;
		}
		GoBackToHistory (SCR_ID_MSG_WRITE);
		ErrorHandlingWhileInsertingObject (emsObjectStatus);
	}
	else
		DisplayPopup((PU8)GetString(STR_EMS_OBJECT_SIZE_NOT_ALLOWED_ID), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
}

/*****************************************************************************
* FUNCTION
*  mmi_ems_go_back_from_preview_object
* DESCRIPTION
*   clear screen for file manager preview
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_ems_go_back_from_preview_object (void)
{
	GoBackToHistory(SCR_ID_MSG_EMS_INSERT_OBJECT);
}

/*****************************************************************************
* FUNCTION
*  mmi_ems_entry_preview_object_from_fm
* DESCRIPTION
*   Entry preview object screen from file manager
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_ems_entry_preview_object_from_fm (void)
{
	U32 objectType=0;
	U8* guiBuffer=NULL;
	EntryNewScreen(SCR_ID_MSG_EMS_PREVIEW_OBJECT, mmi_msg_exit_generic, mmi_ems_entry_preview_object_from_fm, NULL);
	switch (currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_MY_PICTURE_STATE:
			objectType=EMS_USERDEFINED_PICTURE;
			break;
		case EMS_OBJECT_MY_ANIMATION_STATE:
			objectType=EMS_USERDEFINED_ANIMATION;
			break;
		#if defined(__MMI_IMELODY_SUPPORT__)				
		case EMS_OBJECT_MY_MELODY_STATE:
			objectType=EMS_USERDEFINED_MELODY;
		#endif	
			break;
	}
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_EMS_PREVIEW_OBJECT);
	SetMessagesCurrScrnID(SCR_ID_MSG_EMS_PREVIEW_OBJECT);
		
	emsObjectNameWhenPreview=(U8*)GetString(STR_PREVIEW_EMS_OBJECT);
	currObjectListHiliteIndex=0;
	userDefinedObjectDataPtrs[0]=userDefinedObjectDataBuff;
	ShowCategory144Screen (emsObjectNameWhenPreview, IMG_SMS_ENTRY_SCRN_CAPTION,
							  			STR_GLOBAL_OK, IMG_GLOBAL_OK, 
							  			STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							  			objectType, 1,
							  			NULL, preDefinedObjectIds, FALSE, userDefinedObjectDataPtrs,
							  			(PS32)&currObjectListHiliteIndex, guiBuffer); 
	SetLeftSoftkeyFunction(mmi_ems_add_object_from_fm, KEY_EVENT_UP);
	SetRightSoftkeyFunction(mmi_ems_go_back_from_preview_object, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_ems_preview_object_from_fm
* DESCRIPTION
*   Preview object from file manager
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_ems_preview_object_from_fm (PU16 filePath, int is_short)
{
	EMSTATUS emsObjectStatus;
	if(filePath==NULL)
	{
		GoBackToHistory(SCR_ID_MSG_EMS_INSERT_OBJECT);
		return;
	}	
	emsObjectStatus=mmi_ems_fm_open_file((S8*)filePath, currEmsObjectDisplayListState);
	if(emsObjectStatus!=EMS_OK)
	{
		ErrorHandlingWhileInsertingObject(emsObjectStatus);
		DeleteUptoScrID(SCR_ID_MSG_EMS_INSERT_OBJECT);
		return;		
	}	
	else
		mmi_ems_entry_preview_object_from_fm();
}

/*****************************************************************************
* FUNCTION
*  mmi_ems_fm_open_file
* DESCRIPTION
*   Open file
*
* PARAMETERS
*  emsObjectFileNameBuf	IN	file name
*  emsObjectType			IN	object type
* RETURNS
*  TRUE: open success, FALSE: open fail
* GLOBALS AFFECTED
*   none
*****************************************************************************/
EMSTATUS mmi_ems_fm_open_file(S8* emsObjectFileNameBuf, EMS_OBJECT_DISPLAY_STATE emsObjectType)
{
	INT	errorCode=0;
	FILE_HANDLE	emsObjectFilePtr=0;
	MemsetUserDefinedObjectDataBuff ();
	emsObjectFilePtr = pfopen (emsObjectFileNameBuf, PFS_READ, 0);
	if(!pfOpenError(emsObjectFilePtr))
	{
		userDefinedObjectDataSize = GetFSFileSize (emsObjectFilePtr);
		switch(emsObjectType)
		{
#if defined(__MMI_IMELODY_SUPPORT__)				/* Pending work here ...abt hint for over sized..*/
			case EMS_OBJECT_MY_MELODY_STATE:
				if(userDefinedObjectDataSize>MAX_EMS_OBJECT_INSERT_SIZE/*128 bytes*/)
				{
					pfclose (emsObjectFilePtr);
					return EMS_EMS_DATA_EXCEEDED;
				}	
#endif
			default:
				if(userDefinedObjectDataSize>MAX_CURR_EMS_OBJECT_DATA_BUFF/* bytes*/)
				{
					pfclose (emsObjectFilePtr);				
					return EMS_EMS_DATA_EXCEEDED;
				}
		}
		//pfread (userDefinedObjectDataBuff, 1, userDefinedObjectDataSize, emsObjectFilePtr, &errorCode);
		pfread ( emsObjectFilePtr, userDefinedObjectDataBuff, userDefinedObjectDataSize);

		pfclose (emsObjectFilePtr);
		if(pfError (errorCode)) 
			return EMS_UNSPECIFIED_ERROR;
		switch (emsObjectType)
		{
			#if defined(__MMI_IMELODY_SUPPORT__)			
			case EMS_OBJECT_MY_MELODY_STATE:
			{
				U8 userDefinedObjectDataBuffTemp[MAX_CURR_EMS_OBJECT_DATA_BUFF];
				EMS_userdefined_melody_attach_header(userDefinedObjectDataBuff, userDefinedObjectDataBuffTemp, &userDefinedObjectDataSize);
				memcpy(userDefinedObjectDataBuff, userDefinedObjectDataBuffTemp, userDefinedObjectDataSize);
			}
			break;
			#endif
		}
		return EMS_OK;
	}
	return EMS_UNSPECIFIED_ERROR;
}

/*****************************************************************************
* FUNCTION
*  mmi_ems_fm_send_by_ems
* DESCRIPTION
*   Send by EMS from file manager
*
* PARAMETERS
*  emsObjectFileNameBuf	IN	file name
*  emsObjectType			IN	object type
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_ems_fm_send_by_ems (S8* emsObjectFileNameBuf, EMS_OBJECT_DISPLAY_STATE emsObjectType)
{
	EMSTATUS emsObjectStatus;
	currEmsObjectDisplayListState=emsObjectType;
	g_msg_cntx.number_from_phb=0;

	/* disallow re-entering SMS application when there is a pending SMS job running in the background */
	if(mmi_frm_sms_check_action_pending())
	{
		DisplayPopup((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return;
	}
	
	emsObjectStatus=mmi_ems_fm_open_file(emsObjectFileNameBuf, emsObjectType);
	if(emsObjectStatus!=EMS_OK)
	{
		ErrorHandlingWhileInsertingObject(emsObjectStatus);
		return ;
	}
	GetEmsObjectDataSize ();
	if(!IsAllowed)
	{
		DisplayPopup ((PU8)GetString (STR_EMS_OBJECT_SIZE_NOT_ALLOWED_ID), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		return ;
	}
	else
	{
		EMSTATUS emsObjectStatus=EMS_UNSPECIFIED_ERROR;
#if(UI_DOUBLE_BUFFER_SUPPORT)
		coolsand_UI_lock_double_buffer();
#endif
		GetEMSDataForEdit (0, 1);  // Force init
		g_msg_cntx.sendMessageCase = SEND_CASE_SEND_ONLY;
		mmi_msg_entry_write_msg();	
		switch(emsObjectType)
		{
			case 	EMS_OBJECT_MY_PICTURE_STATE:
					emsObjectStatus=EMS_inputbox_insert_userdefined_picture (userDefinedObjectDataBuff);
					break;
			case	EMS_OBJECT_MY_ANIMATION_STATE:
					emsObjectStatus=EMS_inputbox_insert_userdefined_animation (userDefinedObjectDataBuff);
					break;
			#if defined(__MMI_IMELODY_SUPPORT__)			
			case 	EMS_OBJECT_MY_MELODY_STATE:
					emsObjectStatus=EMS_inputbox_insert_userdefined_melody (userDefinedObjectDataBuff);
					break;
			#endif
		}
		handle_category28_input();
	#if(UI_DOUBLE_BUFFER_SUPPORT)
		coolsand_UI_unlock_double_buffer();
	#endif
	#ifdef __MMI_T9__
	    switch(MMI_current_input_type  & INPUT_TYPE_MASK)
	    {
	        case INPUT_TYPE_TR_BOPOMO:
	        case INPUT_TYPE_SM_PINYIN:
	        case INPUT_TYPE_TR_STROKE:
	        case INPUT_TYPE_SM_STROKE:
	        case INPUT_TYPE_SMART_UPPERCASE_ABC:
	        case INPUT_TYPE_SMART_LOWERCASE_ABC:
	        case INPUT_TYPE_TR_MULTITAP_BOPOMO:
	        case INPUT_TYPE_SM_MULTITAP_PINYIN:
 
#if defined(__MMI_T9_THAI__)
	        case INPUT_TYPE_SMART_THAI:
#endif 

	 
	#if defined(__MMI_T9_SPANISH__)
	        case INPUT_TYPE_SMART_UPPERCASE_SPANISH:
	        case INPUT_TYPE_SMART_LOWERCASE_SPANISH:
	#endif
	 
	#if defined(__MMI_T9_DANISH__) 
	        case INPUT_TYPE_SMART_UPPERCASE_DANISH:
	        case INPUT_TYPE_SMART_LOWERCASE_DANISH:
	#endif
	 
	#if defined(__MMI_T9_FRENCH__)
	        case INPUT_TYPE_SMART_UPPERCASE_FRENCH:
	        case INPUT_TYPE_SMART_LOWERCASE_FRENCH:
	#endif
	#if defined(__MMI_T9_GERMAN__)
	        case INPUT_TYPE_SMART_UPPERCASE_GERMAN:
	        case INPUT_TYPE_SMART_LOWERCASE_GERMAN:
	#endif
	#if defined(__MMI_T9_ITALIAN__)
	        case INPUT_TYPE_SMART_UPPERCASE_ITALIAN:
	        case INPUT_TYPE_SMART_LOWERCASE_ITALIAN:
	#endif

 

	#if defined(__MMI_T9_VIETNAMESE__)
	        case INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
	        case INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
	#endif

 

 
#if defined(__MMI_T9_INDONESIAN__)
	        case INPUT_TYPE_SMART_UPPERCASE_INDONESIAN:
	        case INPUT_TYPE_SMART_LOWERCASE_INDONESIAN:
	#endif
 
 
	#if defined(__MMI_T9_RUSSIAN__)
	        case INPUT_TYPE_SMART_UPPERCASE_RUSSIAN:
	        case INPUT_TYPE_SMART_LOWERCASE_RUSSIAN:
	#endif
//END 
	//CSD end

	 
	#if defined(__MMI_T9_PORTUGUESE__)
	        case INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE:
	        case INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
	#endif
	 

	        	  T9SynT9WithInputBox();
	        	  break;
	    }    
	#endif
#ifdef __MMI_DRAW_MANAGER__
		 
		dm_redraw_category_screen();
#else
		RedrawCategory28Screen();
#endif
		ErrorHandlingWhileInsertingObject(emsObjectStatus); // check whetehr it is inserted or not.
	}	
}
//CSD end
#endif

/*****************************************************************************
* FUNCTION
*  EntryScrSaveObjectList
* DESCRIPTION
*   Entry save object list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryScrSaveObjectList (void)
{
	U8* guiBuffer=NULL;
	EMSData * pEms ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_SAVE_OBJECT_LIST);	
	if(guiBuffer==NULL)
	{
		currObjectIndex=0;
	}
	CreateCategory143ObjectList(GetEMSDataForView(&pEms, 0), &previewObjectListPtr);
	if(previewObjectListPtr!=NULL)
	{
		EntryNewScreen(SCR_ID_MSG_SAVE_OBJECT_LIST, ExitScrSaveObjectList, NULL, NULL );	
		ShowCategory143Screen (STR_EMS_OBJECT_LIST, IMG_SMS_ENTRY_SCRN_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK, 
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  previewObjectListPtr, &nObjectLists, 
						  &currObjectIndex, guiBuffer);

		SetLeftSoftkeyFunction (PreEntryScrSaveObjectName, KEY_EVENT_UP);
		SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
		SetKeyHandler (PreEntryScrSaveObjectName, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
	else
		DisplayPopup ((PU8)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
}

/*****************************************************************************
* FUNCTION
*  ExitScrSaveObjectList
* DESCRIPTION
*   Exit save object list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ExitScrSaveObjectList (void)
{
	history_t currHistory;
	S16 ucs2InputBuffer=0;
	FreeCategory143ObjectList (&previewObjectListPtr);
	currHistory.scrnID = SCR_ID_MSG_SAVE_OBJECT_LIST;
	g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=SCR_ID_MSG_SAVE_OBJECT_LIST;
	currHistory.entryFuncPtr = EntryScrSaveObjectList;
	pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&ucs2InputBuffer);
	GetCategoryHistory (currHistory.guiBuffer);
	AddHistory (currHistory);
}

/*****************************************************************************
* FUNCTION
*  PreEntryScrSaveObjectName
* DESCRIPTION
*   Before entry save object name, get object type and size
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void PreEntryScrSaveObjectName (void)
{
	FreeUserEmsObjectNameInputBuff ();
	MemsetUserDefinedObjectDataBuff ();
	SetCurrEmsObjectDisplayListState (GetCategory143DataType ());
	userDefinedObjectDataSize=GetCategory143DataSize ();
	#if defined(__MMI_IMELODY_SUPPORT__)
	if(currEmsObjectDisplayListState==EMS_OBJECT_MY_MELODY_STATE)
	{
		U8 userDefinedObjectDataBuffTemp[MAX_CURR_EMS_OBJECT_DATA_BUFF];
		GetCategory143Data (userDefinedObjectDataBuffTemp);
		EMS_userdefined_melody_detach_header(userDefinedObjectDataBuffTemp, userDefinedObjectDataBuff, 
			&userDefinedObjectDataSize);
	}
	else
	#endif
	{
		GetCategory143Data (userDefinedObjectDataBuff);
	}
	EntryScrSaveObjectName ();
}

/*****************************************************************************
* FUNCTION
*  EntryScrSaveObjectName
* DESCRIPTION
*   Entry save object name screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryScrSaveObjectName (void)
{
	U8* guiBuffer=NULL;
	EntryNewScreen(SCR_ID_MSG_SAVE_OBJECT_NAME, mmi_msg_exit_generic, EntryScrSaveObjectName, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_SAVE_OBJECT_NAME);	
	// SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_SAVE_OBJECT_NAME);
	ShowCategory5Screen (STR_EMS_OBJECT_NAME, IMG_SMS_ENTRY_SCRN_CAPTION,
						  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK, INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
						  (PU8) userEmsObjectNameInput, MAX_EMS_DOWNLOAD_OBJECT_NAME_LEN, guiBuffer); 
	SetLeftSoftkeyFunction (EntryEmsSaveObjectOption, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  EntryEmsSaveObjectOption
* DESCRIPTION
*   Entry save object option screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryEmsSaveObjectOption (void)
{
	U16 numItems=0;
	U8* guiBuffer=NULL;
	U16 nStrItemList[MAX_SUB_MENUS];
	EntryNewScreen(SCR_ID_MSG_SAVE_OBJECT_OPTION, mmi_msg_exit_generic, EntryEmsSaveObjectOption, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_SAVE_OBJECT_OPTION);		 
	SetParentHandler (SMS_INBOX_OPT_SAVE_OBJECT_OPTION);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	numItems = GetNumOfChild (SMS_INBOX_OPT_SAVE_OBJECT_OPTION);
	GetSequenceStringIds(SMS_INBOX_OPT_SAVE_OBJECT_OPTION, nStrItemList);
	SetMessagesCurrScrnID(SCR_ID_MSG_SAVE_OBJECT_OPTION);
	ShowCategory52Screen (STR_GLOBAL_OPTIONS, IMG_SMS_ENTRY_SCRN_CAPTION, 
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, (U16 *)gIndexIconsImageList,
						  NULL , 0, 0, guiBuffer );
}

/*****************************************************************************
* FUNCTION
*  SaveEmsObjectNameAndDownLoadEmsObject
* DESCRIPTION
*   Save object name and data
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SaveEmsObjectNameAndDownLoadEmsObject (void)
{
	EMSTATUS saveResult;
#ifdef __DOWNLOAD__
	U8 index=0;
	S8 currMaxObjects=0;
	U8 currMaxOtherObjects=0;
	S8 maxSystemAllowObjects=0;
	DYNAUDIOINFO *dynAudioInfo=NULL;
	currMaxObjects = ReadDownloadObjectNamesFromNvram ();
	maxSystemAllowObjects = MaxSystemAllowedObjects ();
	if(currMaxObjects >= maxSystemAllowObjects)
	{
		DisplayPopup ((PU8)GetString (STR_EMS_OBJECT_STAORAGE_FULL),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		DeleteMessagesHistoryNodes ();
		return ;
	}
	if(!pfnUnicodeStrlen ((PS8)userEmsObjectNameInput))
	{
		SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_SAVE_OBJECT_OPTION);
		DisplayPopup ((PU8)GetString (STR_EMS_OBJECT_FILE_NAME_CANT_BE_EMPTY),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		DeleteMessagesHistoryNodes ();
		return ;
	}
#if defined(__MMI_WAP__) && defined(__MMI_IMELODY_SUPPORT__)
	if (currEmsObjectDisplayListState==EMS_OBJECT_MELODY_STATE)
	{
		currMaxOtherObjects=GetDownloadIMelodyInfo (&dynAudioInfo);
		for(index=0; index<currMaxOtherObjects; index++)
		{
			if(! pfnUnicodeStrcmp ((PS8)dynAudioInfo[index].AudioName, (PS8)userEmsObjectNameInput))
			{
				SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_SAVE_OBJECT_OPTION);
				DisplayPopup ((PU8)GetString (STR_EMS_OBJECT_DUPLICATED_NAME_INPUT_AGAIN),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
				DeleteMessagesHistoryNodes ();
				return ;
			}
		}
	}
#endif //defined(__MMI_WAP__) && defined(__MMI_IMELODY_SUPPORT__)
	for(index=0; index<currMaxObjects; index++)
	{
		if(! pfnUnicodeStrcmp ((PS8)nvramEmsObjectNames.emsObjectNames[index], (PS8)userEmsObjectNameInput))
		{
			SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_SAVE_OBJECT_OPTION);
			DisplayPopup ((PU8)GetString (STR_EMS_OBJECT_DUPLICATED_NAME_INPUT_AGAIN),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			DeleteMessagesHistoryNodes ();
			return ;
		}
	}
#endif //__MMI_FILE_MANAGER__
	mmi_ems_create_ems_folder();
	saveResult=WriteDownloadObjectDataIntoFileSystem ();
#ifdef __DOWNLOAD__
	if(saveResult==EMS_OK)
	{
		pfnUnicodeStrcpy ((PS8)nvramEmsObjectNames.emsObjectNames[currMaxObjects], (PS8)userEmsObjectNameInput);
		SaveDownloadObjectNameIntoNvram ();
#if defined(__MMI_IMELODY_SUPPORT__)		
		UpdateEmsMelodyInfoStruct ();
#endif //__MMI_IMELODY_SUPPORT__
	}
#endif //__DOWNLOAD__
	ErrorHandlingWhileSavingObject(saveResult);
	if(saveResult!=EMS_ALREADY_INITIALIZED)
	{
		DeleteUptoScrID(SCR_ID_MSG_SAVE_OBJECT_LIST);
		FreeUserEmsObjectNameInputBuff ();
	}
	else
		DeleteUptoScrID(SCR_ID_MSG_SAVE_OBJECT_NAME);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsSaveObjectDone
* DESCRIPTION
*   Highlight save object done handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsSaveObjectDone (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (SaveEmsObjectNameAndDownLoadEmsObject, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}



#ifdef __DOWNLOAD__
/*****************************************************************************
* FUNCTION
*  MemsetEmsMelodyInfoStruct
* DESCRIPTION
*   Reset EMS melody structure
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
//CSD Joy added for customization, 1016
#if defined(__MMI_IMELODY_SUPPORT__)
void MemsetEmsMelodyInfoStruct (void)
{
	U8 index=0;
	for(index=0;index<MAX_EMS_DOWNLOAD_OBJECTS;index++)
		memset(&emsMelodyInfoStruct[index], 0, sizeof(DYNAUDIOINFO));
}

/*****************************************************************************
* FUNCTION
*  UpdateEmsMelodyInfoStruct
* DESCRIPTION
*   Update melody info structure
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void UpdateEmsMelodyInfoStruct (void)
{
	U8 index=0;
	currMaxDownLoadedEmsIMelodies=0;
	MemsetEmsMelodyInfoStruct ();
	currMaxDownLoadedEmsIMelodies=ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_MELODY_NAME_LID, NVRAM_EMS_MY_MELODY_NAME_RECORD_SIZE);
	for(index=0;(index<currMaxDownLoadedEmsIMelodies)&&(index<MAX_EMS_DOWNLOAD_OBJECTS);index++)
	{
		pfnUnicodeStrcpy ((PS8)emsMelodyInfoStruct[index].AudioName, (PS8)nvramEmsObjectNames.emsObjectNames[index]);
		emsMelodyInfoStruct[index].AudioId=DOWNLOADED_EMS_IMELODY_BASE+index;
	}
}

/*****************************************************************************
* FUNCTION
*  GetEmsMelodyNameWithPathInFileSystem
* DESCRIPTION
*   Get meody name and path
*
* PARAMETERS
*  AudioId	IN		audio ID
* RETURNS
*  name buffer
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* GetEmsMelodyNameWithPathInFileSystem (U16 AudioId)
{
	U8 index=0;
	static S8 emsObjectFileNameBuf[(MAX_EMS_DOWNLOAD_OBJECT_FILE_LEN+1)*ENCODING_LENGTH]={0,0};
	for(index=0;index<currMaxDownLoadedEmsIMelodies;index++)
	{
		if(emsMelodyInfoStruct[index].AudioId==AudioId) 
		{
			pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_IMELODY_OBJECT_FILE_NAME_INIT_PATH_ID));
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)emsMelodyInfoStruct[index].AudioName);
			pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)MessagesGetString (( S8*)STR_EMS_IMELODY_OBJECT_FILE_NAME_EXTN_ID));
			return (U8*)emsObjectFileNameBuf;
		}
	}
	return NULL;
}
#endif
//CSD end
/////////////////////////////////////////////////////////////////
// "view/delete" ems objects from "FUN and GAMGES" application
/////////////////////////////////////////////////////////////////

/*****************************************************************************
* FUNCTION
*  DisplayUserDefinedImageList
* DESCRIPTION
*   Before entry image list, set parameters
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DisplayUserDefinedImageList (void)
{
	currEmsObjectDisplayListState=EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE;
	currObjectListHiliteIndex=0;
	currDownloadIndex=0;
	currMaxDownloadList=0;
	EntryScrnListUserDefinedPictureOrAnimations ();
}

/*****************************************************************************
* FUNCTION
*  EntryScrnListUserDefinedPictureOrAnimations
* DESCRIPTION
*   Entry image list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryScrnListUserDefinedPictureOrAnimations (void)
{
	U16 captionStrId=0;
	U8* guiBuffer=NULL;
	U8* strItemList [MAX_EMS_OBJECT_LIST_DISPLAY]={NULL};
	GetEmsDwonloadObjectDisplayListDetails (&captionStrId, strItemList);
	if(currMaxEmsObjectList>0)
	{
		EntryNewScreen(SCR_ID_MSG_EMS_IMAGE_OBJECT_LIST, ExitScrnListUserDefinedPictureOrAnimations, NULL, NULL);
		UpdateImageDataForSyncDownloadFileSystemFunc ();
		guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_EMS_IMAGE_OBJECT_LIST); 
		if(guiBuffer!=NULL)
		{
			currObjectListHiliteIndex=currDownloadIndex;
			change_list_menu_category_history (guiBuffer, currObjectListHiliteIndex, currMaxEmsObjectList, 0);
		}
		else
			currObjectListHiliteIndex=0;
		SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_EMS_IMAGE_OBJECT_LIST);
		ShowCategory6Screen (captionStrId, IMG_SMS_ENTRY_SCRN_CAPTION,
							 STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
							 STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE,
							 currMaxEmsObjectList, (U8**)strItemList,NULL, currObjectListHiliteIndex, guiBuffer);
		SetLeftSoftkeyFunction (EntryScrnEmsObjectOptionsList, KEY_EVENT_UP);
		SetKeyHandler (EntryScrnEmsObjectOptionsList, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		SetRightSoftkeyFunction (GoBackFrmEntryScrnListUserDefinedPictureOrAnimations, KEY_EVENT_UP);
		SetKeyHandler (GoBackFrmEntryScrnListUserDefinedPictureOrAnimations, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		GetCurrEndKeyDownHandler ();
		SetKeyHandler (GotoIdleAfterFreeDownloadEmsPictureAnimtaionObjectNamesBuff, KEY_END, KEY_EVENT_DOWN);
	}
	else
	{
		DisplayPopup ((U8*)GetString (STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 1, MESSAGES_POPUP_TIME_OUT, (U8)EMPTY_LIST_TONE);
	}
}

/*****************************************************************************
* FUNCTION
*  ExitScrnListUserDefinedPictureOrAnimations
* DESCRIPTION
*   Exit image list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ExitScrnListUserDefinedPictureOrAnimations (void)
{
	history_t currHistory;
	S16 ucs2InputBuffer=0;
	currObjectListHiliteIndex=GetHighlightedItem ();
	currDownloadIndex=currObjectListHiliteIndex;
	currMaxDownloadList=currMaxEmsObjectList;
	currHistory.scrnID = SCR_ID_MSG_EMS_IMAGE_OBJECT_LIST;
	g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes=SCR_ID_MSG_EMS_IMAGE_OBJECT_LIST;
	currHistory.entryFuncPtr = EntryScrnListUserDefinedPictureOrAnimations;
	pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&ucs2InputBuffer);
	GetCategoryHistory (currHistory.guiBuffer);
	AddHistory (currHistory);
}

/*****************************************************************************
* FUNCTION
*  GotoIdleAfterFreeDownloadEmsPictureAnimtaionObjectNamesBuff
* DESCRIPTION
*   Free object name buffer before go to idle
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GotoIdleAfterFreeDownloadEmsPictureAnimtaionObjectNamesBuff (void)
{
	FreeDownloadEmsPictureObjectNamesBuff ();
	FreeDownloadEmsAnimtaionObjectNamesBuff ();
	ExecCurrEndKeyDownHandler ();
}

/*****************************************************************************
* FUNCTION
*  GoBackFrmEntryScrnListUserDefinedPictureOrAnimations
* DESCRIPTION
*   Free object name buffer before go back screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GoBackFrmEntryScrnListUserDefinedPictureOrAnimations (void)
{
	FreeDownloadEmsPictureObjectNamesBuff ();
	FreeDownloadEmsAnimtaionObjectNamesBuff ();
	GoBackHistory ();
}

/*****************************************************************************
* FUNCTION
*  GetEmsDwonloadObjectDisplayListDetails
* DESCRIPTION
*   Get object display informatiln
*
* PARAMETERS
*  captionStrID	IN/OUT	caption string id
*  strItemListPtr	IN/OUT	string list pointer
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void GetEmsDwonloadObjectDisplayListDetails (U16 *captionStrID, U8** strItemListPtr)
{
	U8 index=0;
	*captionStrID=STR_EMS_OBJECT_EMS_ID;	
	// Picture
	currMaxEmsPictureObjects=ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_PICTURE_NAME_LID, NVRAM_EMS_MY_PICTURE_NAME_RECORD_SIZE);
	if(currMaxEmsPictureObjects>0)
	{
		AllocDownloadEmsPictureObjectNamesBuff (currMaxEmsPictureObjects);
		for(index=0; index<currMaxEmsPictureObjects; index++)
		{
			pfnUnicodeStrcpy ((S8*)downloadEmsPictureObjectNames[index], (S8*)nvramEmsObjectNames.emsObjectNames[index]);
			strItemListPtr[index] = downloadEmsPictureObjectNames[index];
		}
	}
	// Animations
	currMaxEmsObjectList=ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_ANIMATION_NAME_LID, NVRAM_EMS_MY_ANIMATION_NAME_RECORD_SIZE);
	if(currMaxEmsObjectList>0)
	{
		AllocDownloadEmsAnimationObjectNamesBuff (currMaxEmsObjectList);
		for(index=0; index<currMaxEmsObjectList; index++)
		{
			pfnUnicodeStrcpy ((S8*)downloadEmsAnimationObjectNames[index], (S8*)nvramEmsObjectNames.emsObjectNames[index]);
			strItemListPtr[index+currMaxEmsPictureObjects] = downloadEmsAnimationObjectNames[index];
		}
	}
	currMaxEmsObjectList+=currMaxEmsPictureObjects;
}

/*****************************************************************************
* FUNCTION
*  UpdateImageDataForSyncDownloadFileSystemFunc
* DESCRIPTION
*   Update image data to sync with file system
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void UpdateImageDataForSyncDownloadFileSystemFunc (void)
{	
	U8 index=0;
	DYNIMAGEINFO dynImageInfo[MAX_EMS_DOWNLOAD_OBJECTS+MAX_EMS_DOWNLOAD_OBJECTS];	// picture/animation
	DYNAMICINFO imageDynamicInfo;
	// fill picture names with extn
	for(index=0;index<currMaxEmsPictureObjects;index++)
	{
		pfnUnicodeStrcpy ((S8*)dynImageInfo[index].ImageName, (S8*)downloadEmsPictureObjectNames[index]); 
		pfnUnicodeStrcat ((S8*)dynImageInfo[index].ImageName, ( S8*)MessagesGetString (( S8*)STR_EMS_PICTURE_OBJECT_FILE_NAME_EXTN_ID)); 
		dynImageInfo[index].ImagesId=0; // not used by messages.
	}
	// fill animation names with extn
	for(;index<currMaxEmsObjectList;index++)
	{
		pfnUnicodeStrcpy ((S8*)dynImageInfo[index].ImageName, (S8*)downloadEmsAnimationObjectNames[index-currMaxEmsPictureObjects]); 
		pfnUnicodeStrcat ((S8*)dynImageInfo[index].ImageName, ( S8*)MessagesGetString (( S8*)STR_EMS_ANIMATION_OBJECT_FILE_NAME_EXTN_ID)); 
		dynImageInfo[index].ImagesId=0; // not used by messages.
	}
	// type, no of entries..
	imageDynamicInfo.DynInfoType = IMAGEINFO;
	imageDynamicInfo.NoOfEntries = currMaxEmsObjectList;
	imageDynamicInfo.DYNINFOUNION.ImageInfo = dynImageInfo;
	// send data to download app..
	SyncDownloadFileSystem (MessagesGetString(( S8*)STR_EMS_PIC_A_ANIM_OBJECT_FILE_NAME_INIT_PATH_ID), &imageDynamicInfo );
}

/*****************************************************************************
* FUNCTION
*  RefreshEmsDwonloadObjectNamesAfterDelete
* DESCRIPTION
*   Refresh download object name after delete
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void RefreshEmsDwonloadObjectNamesAfterDelete (void)
{
	U8 index=0;
	// Picture
	currMaxEmsPictureObjects=ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_PICTURE_NAME_LID, NVRAM_EMS_MY_PICTURE_NAME_RECORD_SIZE);
	if(currMaxEmsPictureObjects>0)
	{
		AllocDownloadEmsPictureObjectNamesBuff (currMaxEmsPictureObjects);
		for(index=0; index<currMaxEmsPictureObjects; index++)
		{
			pfnUnicodeStrcpy ((S8*)downloadEmsPictureObjectNames[index], (S8*)nvramEmsObjectNames.emsObjectNames[index]);
		}
	}
	// Animations
	currMaxEmsObjectList=ReadEmsObjectNamesFromNvram (NVRAM_EF_EMS_MY_ANIMATION_NAME_LID, NVRAM_EMS_MY_ANIMATION_NAME_RECORD_SIZE);
	if(currMaxEmsObjectList>0)
	{
		AllocDownloadEmsAnimationObjectNamesBuff (currMaxEmsObjectList);
		for(index=0; index<currMaxEmsObjectList; index++)
		{
			pfnUnicodeStrcpy ((S8*)downloadEmsAnimationObjectNames[index], (S8*)nvramEmsObjectNames.emsObjectNames[index]);
		}
	}
	currMaxEmsObjectList+=currMaxEmsPictureObjects;
}

/*****************************************************************************
* FUNCTION
*  FreeDownloadEmsPictureObjectNamesBuff
* DESCRIPTION
*   Free object name buffer
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void FreeDownloadEmsPictureObjectNamesBuff (void)
{
	U8 index=0;
	for(index=0;index<MAX_EMS_DOWNLOAD_OBJECTS;index++)
	{
		if(downloadEmsPictureObjectNames[index]!=NULL)
		{
			OslMfree (downloadEmsPictureObjectNames[index]);
			downloadEmsPictureObjectNames[index]=NULL;
		}
	}
}

/*****************************************************************************
* FUNCTION
*  AllocDownloadEmsPictureObjectNamesBuff
* DESCRIPTION
*   Allocate object name buffer
*
* PARAMETERS
*  nObjects	IN		object count
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void AllocDownloadEmsPictureObjectNamesBuff (U8 nObjects)
{
	U8 index=0;
	FreeDownloadEmsPictureObjectNamesBuff ();
	for(index=0;index<nObjects;index++)
	{
		downloadEmsPictureObjectNames[index] = OslMalloc (sizeof(U8)*MAX_EMS_DISPLAY_NAME_LEN);
	}
}

/*****************************************************************************
* FUNCTION
*  myfunc
* DESCRIPTION
*   This function is just for template.
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void FreeDownloadEmsAnimtaionObjectNamesBuff (void)
{
	U8 index=0;
	for(index=0;index<MAX_EMS_DOWNLOAD_OBJECTS;index++)
	{
		if(downloadEmsAnimationObjectNames[index]!=NULL)
		{
			OslMfree (downloadEmsAnimationObjectNames[index]);
			downloadEmsAnimationObjectNames[index]=NULL;
		}
	}
}

/*****************************************************************************
* FUNCTION
*  AllocDownloadEmsAnimationObjectNamesBuff
* DESCRIPTION
*   Allocate object name buffer
*
* PARAMETERS
*  nObjects	IN		item count
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void AllocDownloadEmsAnimationObjectNamesBuff (U8 nObjects)
{
	U8 index=0;
	FreeDownloadEmsAnimtaionObjectNamesBuff ();
	for(index=0;index<nObjects;index++)
	{
		downloadEmsAnimationObjectNames[index] = OslMalloc (sizeof(U8)*MAX_EMS_DISPLAY_NAME_LEN);
	}
}

/*****************************************************************************
* FUNCTION
*  EntryScrnEmsObjectOptionsList
* DESCRIPTION
*   Entry object list screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryScrnEmsObjectOptionsList (void)
{ 
	U16 numItems=0;
	U16 nStrItemList[MAX_SUB_MENUS];
	U8* guiBuffer=NULL;
	EntryNewScreen(SCR_ID_MSG_EMS_IMAGE_OBJECT_OPTION, mmi_msg_exit_generic, EntryScrnEmsObjectOptionsList, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_EMS_IMAGE_OBJECT_OPTION);	
	SetParentHandler (EMS_OBJECT_OPTION_MENUENTRY);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	numItems = GetNumOfChild (EMS_OBJECT_OPTION_MENUENTRY);
	GetSequenceStringIds(EMS_OBJECT_OPTION_MENUENTRY, nStrItemList);
	SetMessagesCurrScrnID(SCR_ID_MSG_EMS_IMAGE_OBJECT_OPTION);
	GetCurrEndKeyDownHandler ();
	ShowCategory52Screen (STR_GLOBAL_OPTIONS, IMG_SMS_ENTRY_SCRN_CAPTION, 
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, (U16 *)gIndexIconsImageList,
						  NULL , 0, 0, guiBuffer );
	SetKeyHandler (GotoIdleAfterFreeDownloadEmsPictureAnimtaionObjectNamesBuff, KEY_END, KEY_EVENT_DOWN);
}

#if defined(__MMI_IMELODY_SUPPORT__)
/*****************************************************************************
* FUNCTION
*  DeleteEmsMelodyObject
* DESCRIPTION
*   Delete melody object
*
* PARAMETERS
*  emsOBjectID	IN		audio id
* RETURNS
*  TRUE: delete success, FALSE: delete fail
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 DeleteEmsMelodyObject (U16 emsOBjectID)			// melody
{
	U16 index=0;
	currEmsObjectDisplayListState=EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE;
	for(index=0;(index<currMaxDownLoadedEmsIMelodies)&&(index<MAX_EMS_DOWNLOAD_OBJECTS);index++)
	{
		if(emsMelodyInfoStruct[index].AudioId==emsOBjectID)
		{
			currObjectListHiliteIndex=index;
			DeleteEmsDownloadObjectFrmFunAndGames ();
			return 1;
		}
	}
	return 0;
}
#endif

/*****************************************************************************
* FUNCTION
*  DeleteEmsDownloadObjectFrmFunAndGames
* DESCRIPTION
*   Delete object from fun and game menu
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DeleteEmsDownloadObjectFrmFunAndGames (void)
{
	U8 nvramObjectLID=0;
	U8 nvramObjectSize=0;
	if (currEmsObjectDisplayListState==EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE)
	{
		if (!IsScreenPresent(SCR_ID_MSG_EMS_PREVIEW_OBJECT)) //Hack: shall change this if option menu is moved
		currObjectListHiliteIndex=currDownloadIndex;
		currMaxEmsObjectList=(U8)currMaxDownloadList;
	}
	switch(currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE:
			if(currObjectListHiliteIndex<currMaxEmsPictureObjects)
			{
				nvramObjectLID=NVRAM_EF_EMS_MY_PICTURE_NAME_LID;
				nvramObjectSize=NVRAM_EMS_MY_PICTURE_NAME_RECORD_SIZE;
			}
			else
			{
				nvramObjectLID=NVRAM_EF_EMS_MY_ANIMATION_NAME_LID;
				nvramObjectSize=NVRAM_EMS_MY_ANIMATION_NAME_RECORD_SIZE;
			}
			break;
#if defined(__MMI_IMELODY_SUPPORT__)				
		case EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE:
			nvramObjectLID=NVRAM_EF_EMS_MY_MELODY_NAME_LID;
			nvramObjectSize=NVRAM_EMS_MY_MELODY_NAME_RECORD_SIZE;
			break;
#endif
	}
	if(DeleteEmsObjectNameFromNvram (nvramObjectLID, nvramObjectSize, currObjectListHiliteIndex))
	{
		DeleteDownloadObjectDataFromFileSystem (currObjectListHiliteIndex);
		if(currEmsObjectDisplayListState==EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE)
		{
			currMaxEmsObjectList--;
			currMaxDownloadList=currMaxEmsObjectList;
			if(currMaxEmsObjectList>0)
			{
				if(currObjectListHiliteIndex>=currMaxEmsObjectList)
					currObjectListHiliteIndex = 0;
				currDownloadIndex = currObjectListHiliteIndex;
				RefreshEmsDwonloadObjectNamesAfterDelete ();
				SetMessagesScrnIdToDelHistoryNodes(SCR_ID_MSG_EMS_IMAGE_OBJECT_OPTION);
			}
			else
			{
				FreeDownloadEmsPictureObjectNamesBuff ();
				FreeDownloadEmsAnimtaionObjectNamesBuff ();
				SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_EMS_IMAGE_OBJECT_LIST);
			}
		}
#if defined(__MMI_IMELODY_SUPPORT__)
		else if(currEmsObjectDisplayListState==EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE)
		{
			U8 i=0;
			memset(emsMelodyInfoStruct[currObjectListHiliteIndex].AudioName, 0 , sizeof(emsMelodyInfoStruct[currObjectListHiliteIndex].AudioName));
			emsMelodyInfoStruct[currObjectListHiliteIndex].AudioId=0;
			currMaxDownLoadedEmsIMelodies--;
			for(i=(U8)currObjectListHiliteIndex; i<(currMaxDownLoadedEmsIMelodies); i++)
			{
				pfnUnicodeStrcpy ((PS8)emsMelodyInfoStruct[i].AudioName, (PS8)nvramEmsObjectNames.emsObjectNames[i]);
				emsMelodyInfoStruct[i].AudioId=emsMelodyInfoStruct[i+1].AudioId;
			}
			memset(emsMelodyInfoStruct[currMaxDownLoadedEmsIMelodies].AudioName, 0 , sizeof(emsMelodyInfoStruct[currMaxDownLoadedEmsIMelodies].AudioName));
			emsMelodyInfoStruct[currMaxDownLoadedEmsIMelodies].AudioId=0;
		}
#endif
		DisplayPopup ((PU8)GetString (STR_GLOBAL_DONE),IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
		DeleteMessagesHistoryNodes ();
		
	}
	else
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G6_SMS, "*-------[EmsMiscell.c] DeleteEmsDownloadObjectFrmFunAndGames - FAIlED -------*\n"));
		DisplayPopup ((PU8)GetString (STR_GLOBAL_UNFINISHED),IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		DeleteMessagesHistoryNodes ();
	}
}

/*****************************************************************************
* FUNCTION
*  ViewEmsDownloadObjectFrmFunAndGames
* DESCRIPTION
*   Preview object from fun and game menu
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void ViewEmsDownloadObjectFrmFunAndGames (void)
{
	currEmsObjectDisplayListState=EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE;
	EntryScrPreviewEmsObject ();
}

/*****************************************************************************
* FUNCTION
*  SendMelodiesByEMS
* DESCRIPTION
*   Send by EMS from fun and game menu
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__MMI_IMELODY_SUPPORT__)
void SendMelodiesByEMS (void)
{
	currEmsObjectDisplayListState=EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE;
	EntryFunctionSendByEms ();
}
#endif

/*****************************************************************************
* FUNCTION
*  SendImagesByEMS
* DESCRIPTION
*   Send by EMS from fun and game menu
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void SendImagesByEMS (void)
{
	currEmsObjectDisplayListState=EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE;
	EntryFunctionSendByEms ();
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsObjectViewHandler
* DESCRIPTION
*   Highlight object preview handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsObjectViewHandler (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (ViewEmsDownloadObjectFrmFunAndGames, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightSendByEMSHandler
* DESCRIPTION
*   Highlight send by EMS handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightSendByEMSHandler (void)
{
	BOOL isSimValid = MMI_FALSE;
	U16 i;
	
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
#ifdef __MMI_MULTI_SIM__
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_pwr_context[i].IsSimValid == 1)
		{
			isSimValid = MMI_TRUE;
			break;
		}
	}
	if(isSimValid)		
#else
	if(g_pwr_context[0].IsSimValid == 1)		
#endif
	{
		SetLeftSoftkeyFunction (SendImagesByEMS, KEY_EVENT_UP);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}
	else
		SetLeftSoftkeyFunction (EntryForbiddenScreen, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  HighlightEmsObjectDeleteHandler
* DESCRIPTION
*   Highlight delete object handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void HighlightEmsObjectDeleteHandler (void)
{
	mmi_msg_set_confirm_screen(STR_GLOBAL_DELETE, DeleteEmsDownloadObjectFrmFunAndGames, GoBackHistory);
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	ChangeRightSoftkey (STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (mmi_msg_entry_confirm_generic, KEY_EVENT_UP);
	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler (GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  EntryFunctionSendByEms
* DESCRIPTION
*   Entry function for send by EMS
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void EntryFunctionSendByEms (void)
{
	U8 fileReadError=0;
	g_msg_cntx.number_from_phb=0;
	switch (currEmsObjectDisplayListState)
	{
		case EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE:
			currObjectListHiliteIndex=currDownloadIndex;
			currMaxEmsObjectList=(U8)currMaxDownloadList;
			if(ReadDownloadObjectDataFromFileSystem (currObjectListHiliteIndex))
				fileReadError=1;
			break;
	#if defined(__MMI_IMELODY_SUPPORT__)			
		case EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE:
			if(ReadDownloadObjectDataFromFileSystem (GetCurrentAudioID ()))
				fileReadError=1;
			break;
	#endif
	}
	if(!fileReadError)
	{
		SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_EMS_PREVIEW_OBJECT);
		DisplayPopup ((PU8)GetString (STR_MESSAGES_UNKNOWN_ERROR), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		DeleteMessagesHistoryNodes ();
		return ;
	}
	GetEmsObjectDataSize ();
	if(!IsAllowed)
	{
		SetMessagesScrnIdToDelHistoryNodes (SCR_ID_MSG_EMS_PREVIEW_OBJECT);
		DisplayPopup ((PU8)GetString (STR_EMS_OBJECT_SIZE_NOT_ALLOWED_ID), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
		DeleteMessagesHistoryNodes ();
		return ;
	}
	else
	{
		EMSTATUS emsObjectStatus=EMS_UNSPECIFIED_ERROR;
#if(UI_DOUBLE_BUFFER_SUPPORT)
		coolsand_UI_lock_double_buffer();
#endif
		GetEMSDataForEdit (0, 1);  // Force init
		g_msg_cntx.sendMessageCase = SEND_CASE_SEND_ONLY;
		mmi_msg_entry_write_msg();	
		switch(currEmsObjectDisplayListState)
		{
			case EMS_OBJECT_MY_PICTURE_A_ANIMATION_DOWNLOAD_APP_STATE:
				if(ReadDownloadObjectDataFromFileSystem (currObjectListHiliteIndex))
				{
					if(currObjectListHiliteIndex<currMaxEmsPictureObjects)
						emsObjectStatus=EMS_inputbox_insert_userdefined_picture (userDefinedObjectDataBuff);
					else
						emsObjectStatus=EMS_inputbox_insert_userdefined_animation (userDefinedObjectDataBuff);
				}
				break;
		#if defined(__MMI_IMELODY_SUPPORT__)			
			case EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE:
				if(ReadDownloadObjectDataFromFileSystem (GetCurrentAudioID ()))
				emsObjectStatus=EMS_inputbox_insert_userdefined_melody (userDefinedObjectDataBuff);
				break;
		#endif
		}
		handle_category28_input();
	#if(UI_DOUBLE_BUFFER_SUPPORT)
		coolsand_UI_unlock_double_buffer();
	#endif

	#ifdef __MMI_T9__
		switch(MMI_current_input_type  & INPUT_TYPE_MASK)
		{
		        case INPUT_TYPE_TR_BOPOMO:
		        case INPUT_TYPE_SM_PINYIN:
		        case INPUT_TYPE_TR_STROKE:
		        case INPUT_TYPE_SM_STROKE:
		        case INPUT_TYPE_SMART_UPPERCASE_ABC:
		        case INPUT_TYPE_SMART_LOWERCASE_ABC:
		        case INPUT_TYPE_TR_MULTITAP_BOPOMO:
		        case INPUT_TYPE_SM_MULTITAP_PINYIN:
 
#if defined(__MMI_T9_THAI__)
	        case INPUT_TYPE_SMART_THAI:
#endif 
		 
		#if defined(__MMI_T9_SPANISH__)
		        case INPUT_TYPE_SMART_UPPERCASE_SPANISH:
		        case INPUT_TYPE_SMART_LOWERCASE_SPANISH:
		#endif
 
		#if defined(__MMI_T9_DANISH__)
		        case INPUT_TYPE_SMART_UPPERCASE_DANISH:
		        case INPUT_TYPE_SMART_LOWERCASE_DANISH:
		#endif
 
		#if defined(__MMI_T9_FRENCH__)
		        case INPUT_TYPE_SMART_UPPERCASE_FRENCH:
		        case INPUT_TYPE_SMART_LOWERCASE_FRENCH:
		#endif
		#if defined(__MMI_T9_GERMAN__)
		        case INPUT_TYPE_SMART_UPPERCASE_GERMAN:
		        case INPUT_TYPE_SMART_LOWERCASE_GERMAN:
		#endif
		#if defined(__MMI_T9_ITALIAN__)
		        case INPUT_TYPE_SMART_UPPERCASE_ITALIAN:
		        case INPUT_TYPE_SMART_LOWERCASE_ITALIAN:
		#endif


 

		#if defined(__MMI_T9_VIETNAMESE__)
				case INPUT_TYPE_SMART_UPPERCASE_VIETNAMESE:
				case INPUT_TYPE_SMART_LOWERCASE_VIETNAMESE:
		#endif

 

 
		#if defined(__MMI_T9_INDONESIAN__)
		        case INPUT_TYPE_SMART_UPPERCASE_INDONESIAN:
		        case INPUT_TYPE_SMART_LOWERCASE_INDONESIAN:
		#endif
 
 
		#if defined(__MMI_T9_RUSSIAN__)
		        case INPUT_TYPE_SMART_UPPERCASE_RUSSIAN:
		        case INPUT_TYPE_SMART_LOWERCASE_RUSSIAN:
		#endif
//END 
		//CSD end

		 
		#if defined(__MMI_T9_PORTUGUESE__)
				case INPUT_TYPE_SMART_UPPERCASE_PORTUGUESE:
				case INPUT_TYPE_SMART_LOWERCASE_PORTUGUESE:
		#endif
		 

			T9SynT9WithInputBox();
			break;
		}
	#endif
		RedrawCategory28Screen ();
		ErrorHandlingWhileInsertingObject (emsObjectStatus); // check whetehr it is inserted or not.
	}
}

#if defined(__MMI_IMELODY_SUPPORT__)
/*****************************************************************************
* FUNCTION
*  UpdateDownloadEmsIMelodyInfoInBootUp
* DESCRIPTION
*   Update melody info in boot up
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void UpdateDownloadEmsIMelodyInfoInBootUp (void)
{
	static U8 IsDownloadEmsIMelodyInfoNotUpdated=FALSE;
	if(IsDownloadEmsIMelodyInfoNotUpdated==FALSE)
	{
		UpdateEmsMelodyInfoStruct ();
		IsDownloadEmsIMelodyInfoNotUpdated=TRUE;
	}
}

/*****************************************************************************
* FUNCTION
*  GetDownloadEmsIMelodyInfo
* DESCRIPTION
*   Get melody info
*
* PARAMETERS
*  downloadAudioInfo	IN/OUT	audio info struct
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 GetDownloadEmsIMelodyInfo (DYNAUDIOINFO ** downloadAudioInfo)
{
	currEmsObjectDisplayListState=EMS_OBJECT_MY_MELODY_DOWNLOAD_APP_STATE;
	UpdateDownloadEmsIMelodyInfoInBootUp ();
	*downloadAudioInfo=emsMelodyInfoStruct;
	return currMaxDownLoadedEmsIMelodies;
}

/*****************************************************************************
* FUNCTION
*  UpdateIMelodyDataForSyncDownloadFileSystemFunc
* DESCRIPTION
*   Update melody data to sync with file system
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void UpdateIMelodyDataForSyncDownloadFileSystemFunc (void)
{	
	U8 index=0;
	DYNAUDIOINFO dynMelodyInfo[MAX_EMS_DOWNLOAD_OBJECTS];	// iMelody
	DYNAMICINFO melodyDynamicInfo;
	// fill picture names with extn
	for(index=0;index<currMaxDownLoadedEmsIMelodies;index++)
	{
		pfnUnicodeStrcpy ((S8*)dynMelodyInfo[index].AudioName, (S8*)emsMelodyInfoStruct[index].AudioName); 
		pfnUnicodeStrcat ((S8*)dynMelodyInfo[index].AudioName, ( S8*)MessagesGetString (( S8*)STR_EMS_IMELODY_OBJECT_FILE_NAME_EXTN_ID)); 
		dynMelodyInfo[index].AudioId=0; // not used by messages.
	}
	// type, no of entries..
	melodyDynamicInfo.DynInfoType = AUDIOINFO;
	melodyDynamicInfo.NoOfEntries = currMaxDownLoadedEmsIMelodies;
	melodyDynamicInfo.DYNINFOUNION.AudioInfo = dynMelodyInfo;
	// send data to download app..
	SyncDownloadFileSystem (MessagesGetString(( S8*)STR_EMS_IMELODY_OBJECT_FILE_NAME_INIT_PATH_ID), &melodyDynamicInfo );
}
#endif //__MMI_IMELODY_SUPPORT__
#endif //__DOWNLOAD__

/*****************************************************************************
* FUNCTION
*  mmi_ems_create_ems_folder
* DESCRIPTION
*   Create EMS object folder in boot up
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_ems_create_ems_folder (void)
{
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
		S8 emsObjectFileNameBuf[(MAX_EMS_DOWNLOAD_OBJECT_NAME_INIT_PATH+1)*ENCODING_LENGTH]={0,0};
		U16 driveName;
	   	S8 temp[4];
	   UI_character_type dir_ems_root[4];

		driveName = MMI_PUBLIC_DRV;
		sprintf( temp, "%c:\\", (S8) driveName );
		AnsiiToUnicodeString( (S8*)dir_ems_root, temp);
		
		memset(emsObjectFileNameBuf, 0, ENCODING_LENGTH);
		pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)dir_ems_root);
		pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)FMGR_DEFAULT_FOLDER_IMAGES);
		pfcreateDir(emsObjectFileNameBuf);
		
		memset(emsObjectFileNameBuf, 0, ENCODING_LENGTH);
		pfnUnicodeStrcpy((S8*)emsObjectFileNameBuf, ( S8*)dir_ems_root);
		pfnUnicodeStrcat((S8*)emsObjectFileNameBuf, ( S8*)FMGR_DEFAULT_FOLDER_AUDIO);
		pfcreateDir(emsObjectFileNameBuf);
#else		
		pfcreateDir(MessagesGetString(( S8*)STR_EMS_PIC_A_ANIM_OBJECT_FILE_NAME_INIT_PATH_ID));
		pfcreateDir(MessagesGetString(( S8*)STR_EMS_IMELODY_OBJECT_FILE_NAME_INIT_PATH_ID));
#endif			
}


#endif // defined(__MMI_MESSAGES_EMS__)


#endif	/* __MOD_SMSAL__ */


