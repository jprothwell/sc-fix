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
/*******************************************************************************
 * Filename:
 * ---------
 *	ThemeArchiver.c
 *
 * Project:
 * --------
 *	DLT
 *
 * Description:
 * ------------
 *	This file defines all message sequence between WAP & Theme Archiver.
 *
 * Author: Deepali
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef _MMI_THEME_ARCHIVER_C
#define _MMI_THEME_ARCHIVER_C

#include "mmi_features.h" 
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
#include "stdc.h"
#include "mmi_data_types.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "queuegprot.h"
#include "oslmemory.h"
#include "timerevents.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_popup.h"
#include "wgui_categories_stopwatch.h"
#include "globaldefs.h"
#include "globalscrenum.h"
#include "funandgamesdefs.h"
#include "commonscreens.h"
#include "filesystemdef.h"
#include "filemanagerdef.h"
#include "filesystemgprot.h"

#include "filemgr.h"
#include "settingprofile.h"
#include "stack_common.h"
#include "wap.h"
 
#include "themearchiverdef.h"
#include "themearchivermsgstruct.h"
#include "themearchiverprot.h"
#include "calculator.h"
#include "thememanager.h"

#ifdef MMI_ON_HARDWARE_P
#include "app_ltlcom.h"
#endif

da_descriptor_details_struct gParseResultData;
DLAgentContextStruct gDLAContext;

/*****************************************************************************
* FUNCTION
*	 mmi_init_themearchiver()
* DESCRIPTION
*   initialize protocol events.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_init_themearchiver(void)
{
	SetProtocolEventHandler(da_content_recvd_hdlr,MSG_ID_MMI_THM_CONTENT_RCVD);
	SetProtocolEventHandler(mmi_handle_download_response,MSG_ID_MMI_DOWNLOAD_RES);
	SetProtocolEventHandler(mmi_handle_abort_req,MSG_ID_DA_ABORT_REQ);
}

/*****************************************************************************
* FUNCTION
*	 da_content_recvd_hdlr()
* DESCRIPTION
*   initialize protocol events.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/

void da_content_recvd_hdlr(void *inMsg)
{

	da_content_received *msg=(da_content_received *) inMsg;

	switch(msg->content_type)
	{
		case DA_DESCRIPTOR_DATA:
			da_handle_descriptor(msg);
			break;
		case NON_DESCRIPTOR_THEME_DATA:
			da_handle_non_OMA_content(msg);
			break;
		default:
			break;
	}

	if(msg->download_url)
	{
		OslMfree(msg->download_url);
	}
	if(msg->file_path_name)
	{
		OslMfree(msg->file_path_name);
	}

}

/*****************************************************************************
* FUNCTION
*	 da_handle_non_OMA_content()
* DESCRIPTION
*   Handle downloaded non-OMA theme content from JAS.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void da_handle_non_OMA_content(da_content_received *msg)
{
	S8* temp_file_name;
	gDLAContext.content_type=CONTENT_TYPE_NON_OMA;
	temp_file_name=mmi_parse_url_extract_file_name((S8*)msg->download_url);
	
	memset(gDLAContext.file_name,0,(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH);
	pfnUnicodeStrncpy((S8*)gDLAContext.file_name,(S8*)temp_file_name,FMGR_MAX_FILE_LEN*ENCODING_LENGTH);
	EntryScrSelectStorageScreen();

}

/*****************************************************************************
* FUNCTION
*	 da_handle_descriptor()
* DESCRIPTION
*   Handle Descriptor content from JAS.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void da_handle_descriptor(da_content_received *msg)
{
	gDLAContext.content_type=CONTENT_TYPE_OMA;

	da_parse_descriptor((U8*)msg->file_path_name);

}

/*****************************************************************************
* FUNCTION
*	 da_parse_descriptor()
* DESCRIPTION
*   parse descriptor file.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void da_parse_descriptor(U8* filePath)
{
	FS_HANDLE  fd;
	XML_RESULT_ENUM parse_result=XML_RESULT_FAIL;

	gDLAContext.discElementPresent=0x0000;

	gDLAContext.numThemeType=0;

	fd = MMI_FS_Open((WCHAR*)filePath, FS_READ_ONLY);
	
	if (fd >= 0)
	{
		pfclose(fd);

		if(gDLAContext.parser == NULL)
		{
			gDLAContext.parser= (XML_PARSER_STRUCT*)OslMalloc(sizeof(XML_PARSER_STRUCT));
		}
		xml_new_parser(gDLAContext.parser);
		xml_register_data_handler (gDLAContext.parser, descriptor_data_handler);
		parse_result=xml_parse(gDLAContext.parser, (U16*)filePath);
		xml_stop_parse(gDLAContext.parser);
		xml_close_parser(gDLAContext.parser);
		if(gDLAContext.parser!=NULL)
		{
			OslMfree(gDLAContext.parser);
			gDLAContext.parser=NULL;
		}
	}

	if(parse_result==XML_RESULT_OK)
	{
		if((gDLAContext.discElementPresent & 0x00ff)==0x00ff)
		{
			da_compare_theme_compatibility_withphone();
		}
		else
		{
			gDLAContext.discElementPresent=0x0000;
			FreeDynamicMemoryThemeArchiver();
			DisplayPopup((PU8)GetString(STR_ID_DLT_ERROR_IN_FILE), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		}
	}
	else
	{
		gDLAContext.discElementPresent=0x0000;
		FreeDynamicMemoryThemeArchiver();
		DisplayPopup((PU8)GetString(STR_ID_DLT_ERROR_IN_FILE), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	}

	MMI_FS_Delete((WCHAR*)filePath);

}

/*****************************************************************************
* FUNCTION
*	 descriptor_data_handler()
* DESCRIPTION
*   dat handler for descriptor file.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void descriptor_data_handler( void *data, const char *el, const char *value, kal_int32 len)
{
	U8 i=0;
	U16 err_status;
	U8 tempbuffer[100];
	if(len>99)
		len=99;
	
	memcpy(tempbuffer,value,len);

	tempbuffer[len]=0;

	for(i=0; i<MAX_DESCRIPTOR_ELEMENTS_NUM;i++)
	{
		if(strcmp((S8*)gThemeDesriptorTags[i],el)==0)
		{
			switch(i)
			{
			case DLT_TYPE:
				if((err_status = mmi_check_theme_type_compatibility((S8*)tempbuffer))==TRUE)
				{
					gDLAContext.numThemeType++;
				}
				gDLAContext.discElementPresent |= 0x0001;
				break;
			case DLT_URI:
				if((gDLAContext.discElementPresent & 0x0002) == 0)
				{
					if(gParseResultData.uri != NULL)
					{ 
						OslMfree(gParseResultData.uri);
						gParseResultData.uri=NULL;
					}
					
					gParseResultData.uri=(U8*)OslMalloc((len+1)*ENCODING_LENGTH);
					AnsiiToUnicodeString((S8*)gParseResultData.uri,(S8*)tempbuffer);
					gDLAContext.discElementPresent |= 0x0002;
				}
				break;
			case DLT_SIZE:
				if((gDLAContext.discElementPresent & 0x0004) == 0)
				{
					if(gParseResultData.size != NULL)
					{ 
						OslMfree(gParseResultData.size);
						gParseResultData.size=NULL;
					}
					gParseResultData.size=(U8*)OslMalloc((len+1)*ENCODING_LENGTH);
					AnsiiToUnicodeString((S8*)gParseResultData.size,(S8*)tempbuffer);
					gDLAContext.da_file_size = (float32) atof((S8*)tempbuffer);
					gDLAContext.discElementPresent |= 0x0004;
				}
				break;
			case DLT_VERSION:
				if((gDLAContext.discElementPresent & 0x0008) == 0)
				{
					if(gParseResultData.version!=NULL)
					{ 
						OslMfree(gParseResultData.version);
						gParseResultData.version=NULL;
					}
					
					gParseResultData.version=(U8*)OslMalloc((len+1)*ENCODING_LENGTH);
					AnsiiToUnicodeString((S8*)gParseResultData.version,(S8*)tempbuffer);
					gDLAContext.discElementPresent |= 0x0008;
				}
				break;
			case DLT_NOTIFY_URI:
				if((gDLAContext.discElementPresent & 0x0010) == 0)
				{
					gDLAContext.discElementPresent |= 0x0010;
				}
				break;
			case DLT_FILE_NAME:
				if((gDLAContext.discElementPresent & 0x0020) == 0)
				{
					if(gParseResultData.file_name!=NULL)
					{ 
						OslMfree(gParseResultData.file_name);
						gParseResultData.file_name=NULL;
					}
					
					gParseResultData.file_name=(U8*)OslMalloc((len+1)*ENCODING_LENGTH);
					AnsiiToUnicodeString((S8*)gParseResultData.file_name,(S8*)tempbuffer);
					gDLAContext.discElementPresent |= 0x0020;
				}
				break;
			case DLT_LCD_HEIGHT:
				if((gDLAContext.discElementPresent & 0x0040) == 0)
				{
					gParseResultData.lcd_height = (U16) atoi((S8*)tempbuffer);
					gDLAContext.discElementPresent |= 0x0040;
				}
				break;
			case DLT_LCD_WIDTH:
				if((gDLAContext.discElementPresent & 0x0080) == 0)
				{
					gParseResultData.lcd_width = (U16) atoi((S8*)tempbuffer);
					gDLAContext.discElementPresent |= 0x0080;
				}
				break;
			}
			break;
		}
	}
}

/*****************************************************************************
* FUNCTION
*	 mmi_check_theme_file_version_compatibility()
* DESCRIPTION
*   validates Theme version.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/

U16 mmi_check_theme_file_version_compatibility(float32 theme_version)
{
	U8 i;

	float32* VersionTable;
	VersionTable = (float32*)tm_get_theme_version_table();

	for(i=0; i<MAX_SUPPORTED_VERSION_NUM; i++)
	{
		if(VersionTable[i]==theme_version)
			return TRUE;
	}
	
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*	 mmi_check_theme_file_lcd_dimension_compatibiltiy()
* DESCRIPTION
*   validates LCD dimensions.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
U16 mmi_check_theme_file_lcd_dimension_compatibiltiy(U32 lcd_width, U32 lcd_height)
{
	if(lcd_height==LCD_HEIGHT && lcd_width==LCD_WIDTH)
		return TRUE;
	else
		return FALSE;

}

/*****************************************************************************
* FUNCTION
*	 mmi_check_theme_type_compatibility()
* DESCRIPTION
*   validates Theme type.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/

U16 mmi_check_theme_type_compatibility(S8* input_type )
{
	U8 i=0;

	for(i=0;i<MAX_ACCEPTABLE_THEME_FILE_TYPE;i++)
	{
		if(strcmp(input_type,gAcceptableThemeFileTypes[i])==0)
		{
			return TRUE;			
		}
	}

	return FALSE;
	
}


/*****************************************************************************
* FUNCTION
*	 da_compare_theme_compatibility_withphone()
* DESCRIPTION
*   validates the Theme descriptor data.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void da_compare_theme_compatibility_withphone(void)
{
	float32 theme_version;
	U16 err_status;

	if(gDLAContext.numThemeType == 0 || gDLAContext.numThemeType > MAX_ACCEPTABLE_THEME_FILE_TYPE)
	{
		FreeDynamicMemoryThemeArchiver();
		DisplayPopup((PU8)GetString(STR_DOWNLOAD_THEME_UNSUPPORTED), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		if(IsScreenPresent(WAP_SCREEN_WIDGET))
		{
			DeleteUptoScrID(WAP_SCREEN_WIDGET);
		}
		return;
	
	}	
	theme_version = (float32)coolsand_UI_atof((UI_string_type)gParseResultData.version);

	err_status = mmi_check_theme_file_version_compatibility(theme_version);

	if(err_status==FALSE)
	{
		FreeDynamicMemoryThemeArchiver();
		DisplayPopup((PU8)GetString(STR_DOWNLOAD_THEME_UNSUPPORTED), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		if(IsScreenPresent(WAP_SCREEN_WIDGET))
		{
			DeleteUptoScrID(WAP_SCREEN_WIDGET);
		}
		return;
	
	}
	err_status=mmi_check_theme_file_lcd_dimension_compatibiltiy((U32)gParseResultData.lcd_width,(U32)gParseResultData.lcd_height);
	if(err_status==FALSE)
	{
		DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_ID_DLT_LCD_MISMATCH),
						IMG_GLOBAL_QUESTION,WARNING_TONE);
		SetLeftSoftkeyFunction(EntryDownloadInfoScreen,KEY_EVENT_UP);
		SetRightSoftkeyFunction(GoBackHistoryThemeArchiver,KEY_EVENT_UP);
	}
	else
	{
		EntryDownloadInfoScreen();
	}
}

/*****************************************************************************
* FUNCTION
*	 EntryDownloadInfoScreen()
* DESCRIPTION
*   Show download info screen.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void EntryDownloadInfoScreen(void)
{
	U8* guiBuffer;
	S8 spaceBuffer[5];
	S8 newLineBuffer[5];
	S8 kbBuffer[5];
	U32 remainingBufferSize;
	U32 currBufferSize;

	EntryNewScreen(SCR_ID_DOWNLOAD_THM_INFO, NULL, EntryDownloadInfoScreen, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_DOWNLOAD_THM_INFO); 

	AnsiiToUnicodeString(spaceBuffer," ");
	AnsiiToUnicodeString(newLineBuffer,"\n");
	AnsiiToUnicodeString(kbBuffer,"K");

	currBufferSize=(U32)sizeof(subMenuData)-ENCODING_LENGTH;// for NULL chararcter
	
	memset(subMenuData,0,sizeof(subMenuData));
	pfnUnicodeStrncpy( (PS8)subMenuData, (PS8)GetString(STR_ID_DOWNLOAD_THEME_FILE_NAME),currBufferSize);
	
	remainingBufferSize= currBufferSize - (U32)pfnUnicodeStrlen((PS8)subMenuData);
	pfnUnicodeStrncat( (PS8)subMenuData, (PS8)spaceBuffer,remainingBufferSize);

	remainingBufferSize= currBufferSize - (U32)pfnUnicodeStrlen((PS8)subMenuData);
	pfnUnicodeStrncat( (PS8)subMenuData, (PS8)gParseResultData.file_name,remainingBufferSize);

//new line	
	remainingBufferSize= currBufferSize - (U32)pfnUnicodeStrlen((PS8)subMenuData);
	pfnUnicodeStrncat( (PS8)subMenuData, (PS8)newLineBuffer,remainingBufferSize);
	
	remainingBufferSize= currBufferSize - (U32)pfnUnicodeStrlen((PS8)subMenuData);
	pfnUnicodeStrncat( (PS8)subMenuData, (PS8)GetString(STR_ID_DOWNLOAD_THEME_FILE_SIZE),remainingBufferSize);
	
	remainingBufferSize= currBufferSize - (U32)pfnUnicodeStrlen((PS8)subMenuData);
	pfnUnicodeStrncat( (PS8)subMenuData, (PS8)spaceBuffer,remainingBufferSize);
	
	remainingBufferSize= currBufferSize - (U32)pfnUnicodeStrlen((PS8)subMenuData);
	pfnUnicodeStrncat( (PS8)subMenuData, (PS8)(PS8)gParseResultData.size,remainingBufferSize);
	
	remainingBufferSize= currBufferSize - (U32)pfnUnicodeStrlen((PS8)subMenuData);
	pfnUnicodeStrncat( (PS8)subMenuData, (PS8)kbBuffer,remainingBufferSize);

//new line	
	remainingBufferSize= currBufferSize - (U32)pfnUnicodeStrlen((PS8)subMenuData);
	pfnUnicodeStrncat( (PS8)subMenuData, (PS8)newLineBuffer,remainingBufferSize);
	
	remainingBufferSize= currBufferSize - (U32)pfnUnicodeStrlen((PS8)subMenuData);
	pfnUnicodeStrncat( (PS8)subMenuData, (PS8)GetString(STR_ID_DOWNLOAD_THEME_URL),remainingBufferSize);
	
	remainingBufferSize= currBufferSize - (U32)pfnUnicodeStrlen((PS8)subMenuData);
	pfnUnicodeStrncat( (PS8)subMenuData, (PS8)spaceBuffer,remainingBufferSize);
	
	remainingBufferSize= currBufferSize - (U32)pfnUnicodeStrlen((PS8)subMenuData);
	pfnUnicodeStrncat( (PS8)subMenuData, (PS8)gParseResultData.uri,remainingBufferSize);


	ShowCategory74Screen( STR_DOWNLOAD_INFO_TITLE_ID, IMG_DOWNLOAD_TITLE_ID,
						  STR_GLOBAL_SAVE, 0, STR_GLOBAL_BACK,0,
						  (PU8)subMenuData, pfnUnicodeStrlen((PS8)subMenuData), NULL);
	
	SetLeftSoftkeyFunction(EntryScrSelectStorageScreen, KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistoryThemeArchiver, KEY_EVENT_UP);

	SetKeyHandler(EndKeyHandlerThemeArchiver,KEY_END, KEY_EVENT_DOWN);

}

/*****************************************************************************
* FUNCTION
*	 EntryScrSelectStorageScreen()
* DESCRIPTION
*   Show select storage screen.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void EntryScrSelectStorageScreen(void)
{
	U8* guiBuffer;
	U16 nStrItemList[MAX_SUB_MENUS]; 
	U16 nNumofItem;      
	U16 nDispAttribute;  

	EntryNewScreen(SCR_ID_DOWNLOAD_THM_STORAGE, NULL, EntryScrSelectStorageScreen, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_DOWNLOAD_THM_STORAGE); 
	
	nNumofItem = GetNumOfChild(MENU_DOWNLOAD_THEMES_STORAGE);

	nDispAttribute = GetDispAttributeOfItem(MENU_DOWNLOAD_THEMES_STORAGE);

	GetSequenceStringIds(MENU_DOWNLOAD_THEMES_STORAGE,nStrItemList);

	SetParentHandler(MENU_DOWNLOAD_THEMES_STORAGE);

	RegisterHighlightHandler(SelectThmDownloadStorageType);

	ShowCategory11Screen(STR_SELECT_STORAGE_TITLE_ID, IMG_SELECT_STORAGE_TITLE_ID, STR_GLOBAL_OK, 0, STR_GLOBAL_BACK,0,
						nNumofItem, nStrItemList,0,guiBuffer);

	SetLeftSoftkeyFunction(SetThmDownloadStroageType, KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

	SetKeyHandler(EndKeyHandlerThemeArchiver,KEY_END, KEY_EVENT_DOWN);

}

/*****************************************************************************
* FUNCTION
*	 SelectThmDownloadStorageType()
* DESCRIPTION
*   CallBack function for highlight movement on Select Storage screen.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void SelectThmDownloadStorageType(S32 index)
{
	gDLAContext.currentHiliteStorageType=index;
}

/*****************************************************************************
* FUNCTION
*	 SetThmDownloadStroageType()
* DESCRIPTION
*   Validate the selected storage type and set storage location for 
*	theme download content.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void SetThmDownloadStroageType(void)
{
	S32				fs_RetDrv;
	U64				disk_free_space;
	U64				file_size;
	FS_DiskInfo		disk_info;
	FS_HANDLE	file_handle;

	switch(gDLAContext.currentHiliteStorageType)
	{
	case 0:
		fs_RetDrv	= FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);
		gDLAContext.currStorageType=THEME_TYPE_DOWNLOADED_PHONE;
		break;
	case 1:
		fs_RetDrv = FS_GetDrive(FS_DRIVE_V_REMOVABLE , 1, FS_NO_ALT_DRIVE);
		gDLAContext.currStorageType=THEME_TYPE_DOWNLOADED_CARD;
		break;
	default:
		break;

	}

	if(fs_RetDrv >= 0)
	{
		S8	buf[6];
		S32	create_result;

		sprintf(buf, "%c:\\", (U8)fs_RetDrv);
		memset(subMenuData,0,(FMGR_MAX_PATH_LEN+FMGR_MAX_FILE_LEN)*ENCODING_LENGTH);
		AnsiiToUnicodeString((S8*)subMenuData,(S8*) buf);

		file_handle = pfopen((kal_uint16*)subMenuData, FS_OPEN_DIR | FS_READ_ONLY, 0);

		if(IsScreenPresent(WAP_SCREEN_WIDGET))
		{
			DeleteUptoScrID(WAP_SCREEN_WIDGET);
		}

		if(file_handle >= 0)
		{
			pfclose(file_handle);
			/* drive is avaiable */

			FS_GetDiskInfo((PU16)subMenuData, &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
			
			disk_free_space	= disk_info.FreeClusters*disk_info.SectorsPerCluster*disk_info.BytesPerSector;

			file_size=(U64)(gDLAContext.da_file_size*1024);

			if(disk_free_space >= file_size)
			{

				pfnUnicodeStrcat((PS8)subMenuData, (PS8)FMGR_DEFAULT_FOLDER_THEMES);
				file_handle = MMI_FS_Open((kal_uint16*)subMenuData, FS_OPEN_DIR | FS_READ_ONLY);

				/* path already exist */
				if(file_handle>=0)
				{
					pfclose(file_handle);
					create_result=0;
				}
				else
				{
					create_result = FS_CreateDir((PU16)subMenuData);	
				}

				
				if(create_result == FS_WRITE_PROTECTION)
				{
					FreeDynamicMemoryThemeArchiver();
					DisplayPopup((PU8)GetString(STR_ID_DLT_WRITE_PROTECTED), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
				}
				else if(create_result == FS_DISK_FULL)
				{
					FreeDynamicMemoryThemeArchiver();
					DisplayPopup((PU8)GetString(STR_ID_DLT_DISK_FULL), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
				}
				else
				{
					GoBackHistory();
					SendGetThemeContentReqToWap((PS8)subMenuData);
				}
			}
			else
			{
				FreeDynamicMemoryThemeArchiver();
				DisplayPopup((PU8)GetString(STR_ID_DLT_UNSUFFICIENT_MEMORY), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
			}

		}
		else
		{
			FreeDynamicMemoryThemeArchiver();
			DisplayPopup((PU8)GetString(STR_ID_DLT_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		}
	}
	else	
	{
		/* get drive error  */
		FreeDynamicMemoryThemeArchiver();
		DisplayPopup((PU8)GetString(STR_ID_DLT_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	}

}

/*****************************************************************************
* FUNCTION
*	 SendGetThemeContentReqToWap()
* DESCRIPTION
*   Send get content request to JAS after receiving OMA descriptor.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void SendGetThemeContentReqToWap(S8 *filepath)
{
#ifdef MMI_ON_HARDWARE_P
	struct ilm_struct *ilm_ptr = NULL;
	U8 len;

	da_wap_get_file_req_struct *da_send_req = ((da_wap_get_file_req_struct *)construct_local_para(sizeof(da_wap_get_file_req_struct), TD_CTRL));
	ilm_ptr = allocate_ilm( MOD_MMI );


	switch(gDLAContext.content_type)
	{
	case CONTENT_TYPE_OMA:

		memset(gDLAContext.file_name,0,(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH);
		pfnUnicodeStrncpy(gDLAContext.file_name,(S8*)gParseResultData.file_name,FMGR_MAX_FILE_LEN*ENCODING_LENGTH);
	
		CheckIfFileAlreadyExistInFM((S8*)filepath,(S8*)gDLAContext.file_name);

		len = (pfnUnicodeStrlen((S8*)filepath) + pfnUnicodeStrlen((S8*)gDLAContext.file_name) + 1) * ENCODING_LENGTH;
		da_send_req->save_file_path = (kal_wchar*) OslMalloc(len);

		if(da_send_req->save_file_path != NULL)
		{
			pfnUnicodeStrcpy((S8*)da_send_req->save_file_path, (S8*)filepath);
		}
		pfnUnicodeStrcat((S8*)da_send_req->save_file_path, (S8*)gDLAContext.file_name);

		len = (pfnUnicodeStrlen((S8*)gParseResultData.uri) + 1) * ENCODING_LENGTH;
		da_send_req->download_url= (kal_wchar*) OslMalloc(len);
		if(da_send_req->download_url != NULL)
		{
			memset(da_send_req->download_url, 0, len);
			pfnUnicodeStrcpy((S8*)da_send_req->download_url, (S8*)gParseResultData.uri);

		}

		ilm_ptr->msg_id = MSG_ID_DA_GET_CONTENT_REQ;
		break;
	case CONTENT_TYPE_NON_OMA:
		
		CheckIfFileAlreadyExistInFM((S8*)filepath,(S8*)gDLAContext.file_name);

		len = (pfnUnicodeStrlen((S8*)filepath) + pfnUnicodeStrlen((S8*)gDLAContext.file_name) + 1) * ENCODING_LENGTH;
		da_send_req->save_file_path = (kal_wchar*) OslMalloc(len);

		if(da_send_req->save_file_path != NULL)
		{
			pfnUnicodeStrcpy((S8*)da_send_req->save_file_path, (S8*)filepath);
		}
		pfnUnicodeStrcat((S8*)da_send_req->save_file_path, (S8*)gDLAContext.file_name);
		da_send_req->download_url = NULL;
		ilm_ptr->msg_id = MSG_ID_DA_CONTINUE_DOWNLOAD;
		break;
	}


	ilm_ptr->peer_buff_ptr = NULL;
	ilm_ptr->local_para_ptr = (local_para_struct *) da_send_req;

	SEND_ILM(MOD_MMI, MOD_WAP, INVALID_SAP, ilm_ptr);
#endif
	FreeDynamicMemoryThemeArchiver();

}

/*****************************************************************************
* FUNCTION
*	 mmi_handle_download_response()
* DESCRIPTION
*   Handle Download Theme data response.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_handle_download_response(void *inMsg)
{
	da_content_received *msg=(da_content_received *) inMsg;
	switch(msg->error_code)
	{
	case DLT_NO_ERROR:
		{
			PS8 temp_file_name;

			temp_file_name = mmi_fmgr_extract_file_name((PS8)msg->file_path_name);
			memset(gDLAContext.file_name,0,(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH);
			pfnUnicodeStrncpy(gDLAContext.file_name,temp_file_name,FMGR_MAX_FILE_LEN*ENCODING_LENGTH);
			mmi_theme_download_complete_notification();
		}
		break;
	case DLT_ERROR_CREATE_FILE:
		DeleteIntermediateDLTFiles(msg->file_path_name);
		DisplayPopup((PU8)GetString(STR_ID_DLT_ERROR_CREATE_FILE), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		break;
	case DLT_ERROR_DOWNLOAD_FILE:
		DeleteIntermediateDLTFiles(msg->file_path_name);
		DisplayPopup((PU8)GetString(STR_ID_ERROR_DOWNLOAD), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		break;
	case DLT_ERROR_CONNECTION_ABORT:
		DeleteIntermediateDLTFiles(msg->file_path_name);
		DisplayPopup((PU8)GetString(STR_ID_CONNECTION_BREAK), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		break;
	case DLT_ERROR_USER_CANCEL:
		DeleteIntermediateDLTFiles(msg->file_path_name);
		DisplayPopup((PU8)GetString(STR_ID_USER_CANCEL), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
		break;
	default:
		DeleteIntermediateDLTFiles(msg->file_path_name);
		DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
	}

	if(msg->download_url)
	{
		OslMfree(msg->download_url);
	}
	if(msg->file_path_name)
	{
		OslMfree(msg->file_path_name);
	}

}

/*****************************************************************************
* FUNCTION
*	 mmi_theme_download_complete_notification()
* DESCRIPTION
*   show download complete notification..
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_theme_download_complete_notification(void)
{

	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO, IMG_GLOBAL_NO, 
						get_string(STR_ID_THEME_DOWNLOAD_COMPLETE),
						IMG_GLOBAL_QUESTION,WARNING_TONE);

	SetLeftSoftkeyFunction(SetDLTasActivated, KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}

/*****************************************************************************
* FUNCTION
*	 SetDLTasActivated()
* DESCRIPTION
*   Apply downloaded theme as currrent theme.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void SetDLTasActivated(void)
{
	int status;
	
	EntryNewScreen(0, NULL, NULL, NULL);
	ShowCategory66Screen(STR_SCR3001_THEMES_CAPTION,0, 0, 0, 0, 0, (PU8)GetString(STR_ID_DLT_THM_PROGRESS), IMG_THEME_ACTIVATION, NULL);
	
	status=tm_activate_theme_from_fmgr(gDLAContext.currStorageType,(PU8)gDLAContext.file_name);
	
	HandleThemeActivationErrorStatus(status);

	if(IsScreenPresent(WAP_SCREEN_WIDGET))
	{
		DeleteUptoScrID(WAP_SCREEN_WIDGET);
	}

}

/*****************************************************************************
* FUNCTION
*	 mmi_handle_abort_req()
* DESCRIPTION
*   handle download abort request from JAS
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_handle_abort_req(void *inMsg)
{
}

/*****************************************************************************
* FUNCTION
*	 mmi_parse_url_extract_file_name()
* DESCRIPTION
*   extract file name from URL for non-oma
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
PS8 mmi_parse_url_extract_file_name(S8 *path)
{
   	S8 i, length;

   	length = (U8)pfnUnicodeStrlen(path)*2;
   	i = length - 1;

   	while (i>0)
   	{
      	if (path[i-1] == '/' && path[i] == 0 && (i != length - ENCODING_LENGTH))
         		break;
     	 	i-=2;
   	}

	if (i<0)
		return NULL;
	
	return &path[i+1];
}

/*****************************************************************************
* FUNCTION
*	 GoBackHistoryThemeArchiver()
* DESCRIPTION
*   free allocated memory before go back history.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void GoBackHistoryThemeArchiver(void)
{
	FreeDynamicMemoryThemeArchiver();
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*	 GoBackHistoryThemeArchiver()
* DESCRIPTION
*   free allocated memory before go back history.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void EndKeyHandlerThemeArchiver(void)
{
	FreeDynamicMemoryThemeArchiver();
	DisplayIdleScreen();
}

/*****************************************************************************
* FUNCTION
*	 FreeDynamicMemoryThemeArchiver()
* DESCRIPTION
*   To free allocated memory.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void FreeDynamicMemoryThemeArchiver(void)
{
	if(gParseResultData.uri != NULL)
	{ 
		OslMfree(gParseResultData.uri);
		gParseResultData.uri=NULL;
	}
	
	if(gParseResultData.size != NULL)
	{ 
		OslMfree(gParseResultData.size);
		gParseResultData.size=NULL;
	}
	if(gParseResultData.version!=NULL)
	{ 
		OslMfree(gParseResultData.version);
		gParseResultData.version=NULL;
	}
	
	if(gParseResultData.file_name!=NULL)
	{ 
		OslMfree(gParseResultData.file_name);
		gParseResultData.file_name=NULL;
	}
	gDLAContext.da_file_size=0;
	gDLAContext.numThemeType=0;
	gParseResultData.lcd_height=0;
	gParseResultData.lcd_width=0;
}


/*****************************************************************************
* FUNCTION
*	 DeleteIntermediateDLTFiles()
* DESCRIPTION
*   To delete intermediate downloaded files.
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void DeleteIntermediateDLTFiles(U16* file_path)
{
	FS_HANDLE  fd;
	/* if file exists. Delete it. */
	fd = MMI_FS_Open((WCHAR*)file_path, FS_READ_WRITE);
	if (fd >= 0)
	{
		pfclose(fd);
		MMI_FS_Delete((WCHAR*)file_path);
	}

}

/*****************************************************************************
* FUNCTION
*	 CheckIfFileAlreadyExistInFM()
* DESCRIPTION
*   Check if file Already exists in File manager.
* PARAMETERS
*	 PS8 file name and file path
* RETURNS
*   nil
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void CheckIfFileAlreadyExistInFM(PS8 file_path, PS8 file_name)
{
	FS_HANDLE  fd;
	U8 file_count=0;
	S8 tempStr[6];
	S8 ucs2_dup_file_rename[12];
	U8 len;
	S8 file_ext_buf[(FMGR_MAX_EXT_LEN+1)*ENCODING_LENGTH];
	S8 *file_ext;
	S8 temp_file_name[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];

	memset(temp_file_name,0,(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH);
	pfnUnicodeStrncpy(temp_file_name,file_name,FMGR_MAX_FILE_LEN*ENCODING_LENGTH);

	while(file_count < 65535)
	{
		memset(hintData,0,(FMGR_MAX_PATH_LEN+FMGR_MAX_FILE_LEN)*ENCODING_LENGTH);
		pfnUnicodeStrncpy((PS8)hintData,file_path,FMGR_PATH_BUFFER_SIZE-ENCODING_LENGTH);
		pfnUnicodeStrncat((PS8)hintData,temp_file_name,(FMGR_MAX_FILE_LEN)*ENCODING_LENGTH);
		file_count++;
		fd = pfopen((WCHAR*)hintData, FS_READ_ONLY, 0);
		if(fd >= 0 )
		{
			pfclose(fd);
			sprintf(tempStr,"%d",file_count);
			AnsiiToUnicodeString(ucs2_dup_file_rename,tempStr);

			memset(temp_file_name,0,(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH);
			pfnUnicodeStrncpy(temp_file_name,file_name,FMGR_MAX_FILE_LEN*ENCODING_LENGTH);

			file_ext=(S8*)mmi_fmgr_extract_ext_file_name(temp_file_name);
			pfnUnicodeStrcpy(file_ext_buf,file_ext);
			mmi_thmgr_hide_ext_name(temp_file_name);

			len = FMGR_MAX_INPUT_FILE_LEN - pfnUnicodeStrlen(temp_file_name);

			if(len >= pfnUnicodeStrlen(ucs2_dup_file_rename))
			{
				pfnUnicodeStrcat(temp_file_name,ucs2_dup_file_rename);
			}
			else
			{
				memset(temp_file_name + (FMGR_MAX_INPUT_FILE_LEN - 6) * ENCODING_LENGTH,0,12);
				AnsiiToUnicodeString(tempStr,"~");
				pfnUnicodeStrcat(temp_file_name,tempStr);
				pfnUnicodeStrcat(temp_file_name,ucs2_dup_file_rename);
			}
			AnsiiToUnicodeString(tempStr,".");
			pfnUnicodeStrcat(temp_file_name,tempStr);
			pfnUnicodeStrcat(temp_file_name,file_ext_buf);
		}
		else
		{
			pfnUnicodeStrcpy(file_name,temp_file_name);
			break;
		}
	}

}


/*****************************************************************************
* FUNCTION
*	mmi_thmgr_hide_ext_name
* DESCRIPTION
*	hide extension file name
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void mmi_thmgr_hide_ext_name(S8* file_name)
{
	/* find ext name - by finding "." */
	S32	str_len;
	S32	index;

	str_len	= pfnUnicodeStrlen(file_name);
	index		= (str_len-1)*ENCODING_LENGTH;

	while(index >0)
	{
		if( pfnUnicodeStrncmp((PS8)&file_name[index], (PS8)L".", 1) == 0)
		{
			file_name[index]		= '\0';
			file_name[index+1]	= '\0';
			break;
		}

		file_name[index]		= '\0';
		file_name[index+1]	= '\0';
		index-=2;
	}
}
#endif // #ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
#endif// #ifndef _MMI_THEME_ARCHIVER_C


