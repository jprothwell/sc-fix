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
 * SoundRecorderSrc.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is for sound recorder.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"
#ifdef __MMI_SOUND_RECORDER__


#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#define __NEWSIMULATOR 
#include "globaldefs.h"
#include "globalscrenum.h"
#include "commonscreens.h"
#include "custdatares.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "custmenures.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_multimedia.h" 
#include "wgui_categories_popup.h"
#include "hardwareevents.h"  
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"
#include "callmanagementiddef.h"
#include "callmanagementgprot.h"

#include "timerevents.h"
#include "mainmenudef.h"
#include "funandgamesdefs.h"
#include "mmi_data_types.h"
#include "customcfg.h"
#include "unicodexdcl.h"
#include "filesystemdef.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "gpioinc.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "fat_fs.h"
#ifdef MMI_ON_HARDWARE_P
#include "rtc_sw.h"
#endif
#include "profilegprots.h"
#include "settingprofile.h"
#include "wgui_inline_edit.h"

#include "filemanagergprot.h"
#include "filemanagerdef.h"
#include "filemgr.h"

#include "soundrecorderdef.h"
#include "soundrecordertype.h"
#include "soundrecorderprot.h"
#include "gbhmi.h"
#include "vdoplystrenum.h"
#include "editorpen.h"

#include "mmi_trace.h"       //added by elisa for mem_cpy

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
#include "profilemgrgprot.h"
#endif
#undef __NEWSIMULATOR 

#ifdef __DRM_SUPPORT__
#define DRM_NOT_PERMIT_TO_PLAY	9999
#define DRM_NOT_PERMIT_TO_APPEND	10000
#endif
#include "mmi_trace.h"
//#include "jasper.h"
/*=============  Constants & Global Variables  ================ */

SoundRecorderStruct SoundRecorder;
static SoundFileListStruct *FileList_p=&SoundRecorder.SoundFileList;
static SoundRecorderSettingsStruct *Settings_p=&SoundRecorder.Settings;
U8 mmi_sndrec_is_rename_editor_in_call = 0;
/*============= Local Variables =================*/

/* settings */
#define MAX_SR_SETTINGS_FIELDS 2

U16 SRTitleStrings[MAX_SR_SETTINGS_FIELDS];

S32 selectedStorageIndex;
S32 selectedFormatIndex;

/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
#define NO_OF_STORAGES  3
#else
#define NO_OF_STORAGES  2
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
UI_string_ID_type gStorageIds[] =
{
   STR_ID_SNDREC_SETTINGS_PHONE,
   STR_ID_SNDREC_SETTINGS_CARD
   /*+ zhouqin modify for dual t-flash card 20110503*/
    #if defined(DUAL_TFLASH_SUPPORT)
    ,STR_ID_SNDREC_SETTINGS_SEC_CARD
    #endif
    /*- zhouqin modify for dual t-flash card 20110503*/
};
UI_string_type gStorages[NO_OF_STORAGES];

#ifdef MT6205B
#define NO_OF_FORMATS  NO_OF_FORMAT /* vm is explicit if this is our only one format*/
#else
#define NO_OF_FORMATS  (NO_OF_FORMAT-1) /* vm is not explicit */
#endif

UI_string_ID_type gFormatIds[] =
{

#ifndef MT6205B
   STR_ID_SNDREC_SETTINGS_AMR,
   STR_ID_SNDREC_SETTINGS_WAV,
#endif
#if defined( AMRWB_ENCODE ) && defined( AMRWB_DECODE )
   STR_ID_SNDREC_SETTINGS_AMRWB,
   STR_ID_SNDREC_SETTINGS_VM,	// chenxia move above for jasperII 20071121
#endif
   //STR_ID_SNDREC_SETTINGS_WAV,   
   STR_ID_SNDREC_SETTINGS_AMR,
   #if 0
   STR_ID_SNDREC_SETTINGS_MP3	// chenxia add for jasperII 20071121
   #else
   STR_ID_SNDREC_SETTINGS_WAV
   #endif
};

UI_string_type gFormats[NO_OF_FORMATS];

static S32 currFileIndex;
static UI_character_type editorBuffer[MAX_INPUT_FILE_LEN+2];

static UI_character_type strBuffer[30]; 
static UI_character_type extBuffer[FMGR_MAX_EXT_LEN+1];

#define NO_BLOCK_INT  2
/*
U16 BlockInterruptTable[]=
{
   PRT_EQ_GPIO_DETECT_IND,
   PRT_BATTERY_STATUS_IND   
};

#define NO_ENABLE_INT  2

U16 EnableInterruptTable[]=
{
   PRT_INCOMINGCALL_EVENT,
   MSG_ID_SIM_CARD_FAILURE
};
*/

static U16 current_scr_id;
static FuncPtr current_entry_func;

/*===============    Functions    ====================*/



#define SR_MAIN

void InitSoundRecorderApp(void)
{
	TBM_ENTRY(Rec_stub_0);
   SetHiliteHandler(MENU_ID_SNDREC_MAIN, mmi_sndrec_hightlight_main);
#ifdef __MMI_SOUND_RECORDER_STYLE_X__
   SetHiliteHandler(MENU_ID_SNDREC_NEW_RECORD, mmi_sndrec_hightlight_record);
   SetHiliteHandler(MENU_ID_SNDREC_LIST, mmi_sndrec_highlight_list);
#endif
   SetHiliteHandler(MENU_ID_SNDREC_RECORD, mmi_sndrec_hightlight_record);
   SetHiliteHandler(MENU_ID_SNDREC_PLAY, mmi_sndrec_hightlight_play);
   SetHiliteHandler(MENU_ID_SNDREC_APPEND, mmi_sndrec_hightlight_append);
   SetHiliteHandler(MENU_ID_SNDREC_RENAME, highlight_fmgr_rename);
   SetHiliteHandler(MENU_ID_SNDREC_OPTION_SAVE, mmi_sndrec_hightlight_save);
   SetHiliteHandler(MENU_ID_SNDREC_OPTION_INPUT_METHOD, mmi_sndrec_hightlight_input_method);
   SetHiliteHandler(MENU_ID_SNDREC_DELETE, mmi_sndrec_hightlight_delete);
   SetHiliteHandler(MENU_ID_SNDREC_DELETE_ALL, mmi_sndrec_hightlight_delete_all);
   SetHiliteHandler(MENU_ID_SNDREC_SETTINGS, mmi_sndrec_hightlight_settings);

   SoundRecorder.inCall=FALSE;
   SoundRecorder.isFromCallExpress = FALSE;
   SoundRecorder.isSlefStop=FALSE;
   SoundRecorder.isAppend = FALSE;
   mmi_sndrec_init_settings();
   mmi_sndrec_init_file_list();
//   mmi_sndrec_create_sound_folder();
	TBM_EXIT(Rec_stub_0);
}

#ifdef __MMI_SOUND_RECORDER_STYLE_X__
void mmi_sndrec_highlight_list( void )
{
   /*1 Change left soft key icon and label */
   ChangeLeftSoftkey(STR_GLOBAL_OK,0);

   /*2 Change right soft key icon and label */
   ChangeRightSoftkey(STR_GLOBAL_BACK,0);

   /*3 Register function for left soft key */
   SetLeftSoftkeyFunction(mmi_sndrec_entry_main,KEY_EVENT_UP);

   /*4 Register function for right soft key */
   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   /*5 Register function for right  key */
   SetKeyHandler(mmi_sndrec_entry_main,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

   /*6 Register function for left  key */
   SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

void mmi_sndrec_entry_style_x_main( void )
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U8* guiBuffer;       /* Buffer holding history data */
	
	/* 1 Call Exit Handler*/
	EntryNewScreen( SCR_ID_SNDREC_MAIN, NULL, mmi_sndrec_entry_style_x_main, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer( SCR_ID_SNDREC_MAIN );	

	/* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild( MENU_ID_SNDREC_MAIN );

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds( MENU_ID_SNDREC_MAIN, nStrItemList );

	/* 6 Set current parent id*/
	SetParentHandler( MENU_ID_SNDREC_MAIN );

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler( ExecuteCurrHiliteHandler );

	/* 8 show category screen */
	ShowCategory15Screen( SCR_ID_SNDREC_MAIN, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
		STR_GLOBAL_OK, IMG_GLOBAL_OK,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
		nNumofItem,
		nStrItemList,
		(PU16)gIndexIconsImageList,
		1, 0,
		guiBuffer );

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}
#endif

extern int checkUsbstate(void);
extern VOID mmi_display_usb_busy(void);
void mmi_sndrec_entry_in_idle(void)
{
     mmi_trace(g_sw_SREC,"mmi_sndrec_entry_in_idle,MMI_CARD_DRV is %d",MMI_CARD_DRV);
	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}
	if(checkVMOn())
	{
	    	mmi_show_qq_select_confirm(mmi_sndrec_entry_in_idle);
		return;
	}
#ifndef COOLSAND_MULTIMEDIA_USE_FLASHFS
	if( MMI_CARD_DRV>0 )
	{
#endif
		SoundRecorder.inCall=FALSE;
		SoundRecorder.isFromCallExpress = FALSE;
		mmi_sndrec_is_rename_editor_in_call = 0;
		//mmi_sndrec_create_sound_folder();

#ifdef __MMI_SOUND_RECORDER_STYLE_X__
		mmi_sndrec_entry_style_x_main();
#else
		mmi_sndrec_entry_main();
#endif

#ifndef COOLSAND_MULTIMEDIA_USE_FLASHFS

	}
	else
	{
		mmi_sndrec_error_hdlr(MDI_AUDIO_NO_DISC);
		return;
	}
#endif
}

void mmi_sndrec_entry_in_call(void)
{
   SoundRecorder.inCall=TRUE;
   SoundRecorder.isFromCallExpress = FALSE;
   mmi_sndrec_is_rename_editor_in_call = 0;
   mmi_sndrec_send_record_req();
}


void mmi_sndrec_entry_from_call(void)
{
   SoundRecorder.inCall=TRUE;
   SoundRecorder.isFromCallExpress = TRUE;
   mmi_sndrec_is_rename_editor_in_call = 0;
   mmi_sndrec_send_record_req();
}

void mmi_sndrec_filename_callback(void* fullname, int is_short)
{
   if( fullname == NULL )
      GoBackHistory();
   else
      pfnUnicodeStrcpy((S8*)SoundRecorder.fullName, (S8*)fullname);  
}

void mmi_sndrec_entry_main(void)
{
	FMGR_FILTER	filter;
	TBM_ENTRY(Rec_stub_1);
	mmi_trace(g_sw_MED_Aud, "chenxia!!!mmi_sndrec_entry_main");

	FMGR_FILTER_INIT(&filter);
#ifndef MT6205B
	FMGR_FILTER_SET(&filter, FMGR_TYPE_WAV);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_AMR);
#endif
#if defined( AMRWB_ENCODE ) && defined( AMRWB_DECODE )
	FMGR_FILTER_SET(&filter, FMGR_TYPE_AWB);
#endif
	FMGR_FILTER_SET(&filter, FMGR_TYPE_AMR);//chenhe
	//FMGR_FILTER_SET(&filter, FMGR_TYPE_VM);
	//FMGR_FILTER_SET(&filter, FMGR_TYPE_DAF);	// gdm 090611 fix bug 11567
	FMGR_FILTER_SET(&filter, FMGR_TYPE_WAV);	

	
	mmi_sndrec_get_settings();
		
	mmi_trace(g_sw_MED_Aud, "chenxia!!! before print FileList_p->storagePath[STORAGE_CARD]'s value");
	//mmi_trace(g_sw_SREC,"mmi_sndrec_entry_main,before,FileList_p->storagePath[Settings_p->storage] is %s;storage is %d",FileList_p->storagePath[Settings_p->storage], Settings_p->storage);
#if 0
   AnsiiToUnicodeString(temp,FileList_p->storagePath[Settings_p->storage]);
   memset(FileList_p->storagePath[Settings_p->storage],0x0,sizeof(FileList_p->storagePath[Settings_p->storage]));   
   pfnUnicodeStrcpy(FileList_p->storagePath[Settings_p->storage],temp);
#endif   
   mmi_fmgr_select_path_and_enter(APP_SOUNDRECORDER, FMGR_SELECT_BROWSE, filter, 
  (PS8)FileList_p->storagePath[Settings_p->storage], mmi_sndrec_filename_callback);
   mmi_trace(g_sw_SREC,"chenxia!!!mmi_sndrec_entry_main,main menuId is %d,MENU_ID_SNDREC_OPTION_4 is %d,MENU_ID_SNDREC_OPTION_1 is %d",MENU_ID_SNDREC_MAIN,MENU_ID_SNDREC_OPTION_4,MENU_ID_SNDREC_OPTION_1);
   ClearKeyHandler( KEY_LEFT_ARROW, KEY_EVENT_DOWN );
   	TBM_EXIT(Rec_stub_1);

}

void mmi_sndrec_hightlight_main(void)
{
	
		/*1 Change left soft key icon and label */
		ChangeLeftSoftkey(STR_GLOBAL_OK,0);

		/*2 Change right soft key icon and label */
		ChangeRightSoftkey(STR_GLOBAL_BACK,0);

		/*3 Register function for left soft key */
		SetLeftSoftkeyFunction(mmi_sndrec_entry_in_idle,KEY_EVENT_UP);

		/*4 Register function for right soft key */
		SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		
        /*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
		/*5 Register function for right  key */
		SetKeyHandler(mmi_sndrec_entry_in_idle,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

		/*6 Register function for left  key */
		SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);	
	#endif
        /*- zhouqin 2011-02-16 modify for q6 320X240 */	
}


#define SR_FILE_LIST

void mmi_sndrec_create_sound_folder(void)
{
#ifdef MMI_ON_HARDWARE_P
   FS_HANDLE file_handle;
   S32 result;
#if 1 //chenhe for jasper tmp;we dont need create a folder on flash
      if(Settings_p->cardAvailable)
      {
          ASSERT(FileList_p->storagePath[STORAGE_CARD] != 0);
          file_handle = MMI_FS_Open((const UINT8 *)FileList_p->storagePath[STORAGE_CARD], FS_OPEN_DIR | FS_READ_ONLY );
          if (file_handle < 0) {
             result = MMI_FS_CreateDir((const UINT8 *)FileList_p->storagePath[STORAGE_CARD]);
          }
          else
             MMI_FS_Close(file_handle);
       }
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
      if(Settings_p->seccardAvailable)
      {
          ASSERT(FileList_p->storagePath[STORAGE_SEC_CARD] != 0);
          file_handle = MMI_FS_Open((const UINT8 *)FileList_p->storagePath[STORAGE_SEC_CARD], FS_OPEN_DIR | FS_READ_ONLY );
          if (file_handle < 0) {
             result = MMI_FS_CreateDir((const UINT8 *)FileList_p->storagePath[STORAGE_SEC_CARD]);
          }
          else
             MMI_FS_Close(file_handle);
       }
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
#else
   for(i=0;i<NO_OF_STORAGE;i++)
   {
      file_handle = MMI_FS_Open(FileList_p->storagePath[i], FS_OPEN_DIR | FS_READ_ONLY );
      if (file_handle < 0) {
         result = MMI_FS_CreateDir(FileList_p->storagePath[i]);
      }
      else
         MMI_FS_Close(file_handle);
   }     
#endif 
#endif      
}

BOOL mmi_sndrec_check_dest_folder(U8 storage)
{
#ifdef MMI_ON_HARDWARE_P
   FS_HANDLE file_handle;
   S32 result;
   S8 tmpStr[128]={0};

   file_handle = MMI_FS_Open((const UINT8 *)FileList_p->storagePath[storage], FS_OPEN_DIR | FS_READ_ONLY );
  // file_handle=FS_FOLDER_HANDLE;//chenhe for jasperII
   mmi_trace(g_sw_SREC, "mmi_sndrec_check_dest_folder,file_handle %d,FS_FOLDER_HANDLE is %d",file_handle,FS_FOLDER_HANDLE);
   if (file_handle != FS_FOLDER_HANDLE) 
   {
   //to create dir without last '/'
	pfnUnicodeStrncpy(tmpStr,(const S8 *)FileList_p->storagePath[storage],pfnUnicodeStrlen((const S8 *)FileList_p->storagePath[storage])-1);
      //result = MMI_FS_CreateDir(FileList_p->storagePath[storage]);
        result = MMI_FS_CreateDir(tmpStr);

       mmi_trace(g_sw_SREC, "mmi_sndrec_check_dest_folder,result %d",result);

      if(result < 0)
      {
         DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8)ERROR_TONE);
         return FALSE;
      }
   }
   else
   {
      MMI_FS_Close(file_handle);
   }
#endif

   return TRUE;
}

void mmi_sndrec_init_file_list(void)
{
   S16 phoneDrvLetter, cardDrvLetter;
   S8 tmp[256];
	TBM_ENTRY(Rec_stub_9);
   FileList_p->noOfEntries=FileList_p->noOfEntriesPhone=FileList_p->noOfEntriesCard=0;
   phoneDrvLetter = MMI_PUBLIC_DRV;
   //kal_wsprintf((kal_wchar*)FileList_p->storagePath[STORAGE_PHONE], "%c:\\%w", phoneDrvLetter, FMGR_DEFAULT_FOLDER_AUDIO);
   //kal_wsprintf((kal_wchar*)FileList_p->storagePath[STORAGE_PHONE], "%c:/%s", phoneDrvLetter, FMGR_DEFAULT_RECORDER_FOLDER);//chenhe,070622
	sprintf((char *)FileList_p->storagePath[STORAGE_PHONE], "%c:/%s", phoneDrvLetter, FMGR_DEFAULT_RECORDER_FOLDER);//chenhe,070622
   mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_init_file_list,phoneDrvLetter is %c,FileList_p->storagePath[STORAGE_PHONE] is %s",phoneDrvLetter,FileList_p->storagePath[STORAGE_PHONE]);
	   AnsiiToUnicodeString(tmp, (S8 *)FileList_p->storagePath[STORAGE_PHONE]);

   memset(FileList_p->storagePath[STORAGE_PHONE],0x0,sizeof(FileList_p->storagePath[STORAGE_PHONE]) );
   pfnUnicodeStrcpy((S8 *)FileList_p->storagePath[STORAGE_PHONE],tmp);
   
   if(Settings_p->cardAvailable)
   {
	cardDrvLetter = MMI_CARD_DRV;
	//kal_wsprintf((kal_wchar*)FileList_p->storagePath[STORAGE_CARD], "%c:\\%w", cardDrvLetter, FMGR_DEFAULT_FOLDER_AUDIO);
	//kal_wsprintf((kal_wchar*)FileList_p->storagePath[STORAGE_CARD], "%c:/%s", cardDrvLetter, FMGR_DEFAULT_RECORDER_FOLDER);//chenhe,070622
	sprintf((char *)FileList_p->storagePath[STORAGE_CARD], "%c:/%s", cardDrvLetter, FMGR_DEFAULT_RECORDER_FOLDER);//chenhe,070622
	mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_init_file_list,22phoneDrvLetter is %c,FileList_p->storagePath[STORAGE_CARD] is %s",phoneDrvLetter,FileList_p->storagePath[STORAGE_CARD]);
	AnsiiToUnicodeString(tmp, (S8 *)FileList_p->storagePath[STORAGE_CARD]);
	memset(FileList_p->storagePath[STORAGE_CARD],0x0,sizeof(FileList_p->storagePath[STORAGE_CARD]) );
	pfnUnicodeStrcpy((S8 *)FileList_p->storagePath[STORAGE_CARD],tmp);
	//ass(1);
   }
   else
   {
	memset(FileList_p->storagePath[STORAGE_CARD],0x0,sizeof(FileList_p->storagePath[STORAGE_CARD]) );
   }

/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
  if(Settings_p->seccardAvailable)
   {
	cardDrvLetter = MMI_SEC_CARD_DRV;
	sprintf((char *)FileList_p->storagePath[STORAGE_SEC_CARD], "%c:/%s", cardDrvLetter, FMGR_DEFAULT_RECORDER_FOLDER);//chenhe,070622
	mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_init_file_list,22phoneDrvLetter is %c,FileList_p->storagePath[STORAGE_SEC_CARD] is %s",phoneDrvLetter,FileList_p->storagePath[STORAGE_SEC_CARD]);
	AnsiiToUnicodeString(tmp, (S8 *)FileList_p->storagePath[STORAGE_SEC_CARD]);
	memset(FileList_p->storagePath[STORAGE_SEC_CARD],0x0,sizeof(FileList_p->storagePath[STORAGE_SEC_CARD]) );
	pfnUnicodeStrcpy((S8 *)FileList_p->storagePath[STORAGE_SEC_CARD],tmp);
   }
   else
   {
	memset(FileList_p->storagePath[STORAGE_SEC_CARD],0x0,sizeof(FileList_p->storagePath[STORAGE_SEC_CARD]) );
   }
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
   	TBM_EXIT(Rec_stub_9);

}

UI_string_type mmi_sndrec_get_file_name(UI_string_type fullname)
{
   UI_string_type p;

   p=fullname+pfnUnicodeStrlen((const S8*)fullname);
   #if 1
   while(p>fullname)
   {
      if (*p=='/')
      {
         return (p+1);
      }
      p--;
   }
   #else
      while(p>fullname)
   {
      if (*p==_T_MMI'\\')
      {
         return (p+1);
      }
      p--;
   }
   #endif //chenhe
   
   return p;
}


void mmi_sndrec_remove_file_ext(UI_string_type filename)
{
   UI_string_type p;

   extBuffer[0] = 0;
   p=filename+pfnUnicodeStrlen((const S8*)filename);
   #if 0
   while(p>filename)
   {
      if (*p==_T_MMI'.')
      {
         *p=0;
         pfnUnicodeStrncpy( (S8*)extBuffer, (S8*)(p+1), FMGR_MAX_EXT_LEN );
         break;
      }
      p--;
   }
   
   #else
      while(p>filename)
   {
      if (*p=='.')
      {
         *p=0;
         *(p-1)=0;
         pfnUnicodeStrncpy( (S8*)extBuffer, (S8*)(p+1), FMGR_MAX_EXT_LEN );
         break;
      }
      p--;
   }
	  
   #endif //chenhe
}

void mmi_sndrec_append_file_ext(UI_string_type filename)
{
	U8 format =
#ifndef MT6205B
#if !defined( __MMI_DEDI_AMR_REC__ )
	SoundRecorder.inCall ? FORMAT_WAV:
#elif defined( AMRWB_ENCODE ) && defined( AMRWB_DECODE )
	(SoundRecorder.inCall && SoundRecorder.Settings.format == FORMAT_AWB) ? FORMAT_AMR:
#endif
#endif
	SoundRecorder.Settings.format;


      switch(format)
      {
#ifndef MT6205B
         case FORMAT_AMR:
            //pfnUnicodeStrncat((S8*)filename, (S8*)FILE_EXT_AMR, MAX_FILE_NAME_LEN);//chenhe,20070622
            strcat((S8*)filename,(S8*)FILE_EXT_AMR);
            break;
         case FORMAT_WAV:
           // pfnUnicodeStrncat((S8*)filename, (S8*)FILE_EXT_WAV, MAX_FILE_NAME_LEN);//chenhe,20070622
           strcat((S8*)filename,(S8*)FILE_EXT_WAV);
            break;
#endif
#if defined( AMRWB_ENCODE ) && defined( AMRWB_DECODE )
         case FORMAT_AWB:
           /// pfnUnicodeStrncat((S8*)filename, (S8*)FILE_EXT_AWB, MAX_FILE_NAME_LEN);//chenhe,20070622
              strcat((S8*)filename,(S8*)FORMAT_AWB);
            break;
		 case FORMAT_VM:	// chenxia move above for jasperII
           // pfnUnicodeStrncat((S8*)filename, (S8*)FILE_EXT_VM, MAX_FILE_NAME_LEN);//chenhe,20070622
              strcat((S8*)filename,(S8*)FILE_EXT_VM);
                //strcat((S8*)filename,(S8*)FILE_EXT_AMR);
            break;
#endif


         case FORMAT_AMR://chenhe move for above for jasperII
            //pfnUnicodeStrncat((S8*)filename, (S8*)FILE_EXT_AMR, MAX_FILE_NAME_LEN);//chenhe,20070622
            strcat((S8*)filename,(S8*)FILE_EXT_AMR);
			break;
			#if 0
		 case FORMAT_MP3:	// add for jasperII  	chenxia	  20071121
			strcat((S8*)filename, (S8*)FILE_EXT_MP3);
			break;
			#else
			 case FORMAT_WAV:	
			strcat((S8*)filename, (S8*)FILE_EXT_WAV);
			break;

			#endif
		default:
			break;
      }
//   }
}

/*
void mmi_sndrec_restore_file_ext(UI_string_type filename)
{
#ifdef MMI_ON_HARDWARE_P
   if(FileList_p->noOfEntries>0&&currFileIndex<FileList_p->noOfEntries)
   {
   	FMGR_FILE_INFO_STRUCT file_info;
	   mmi_fmgr_kernel_get_file_info( (U16) currFileIndex, &file_info);
      pfnUnicodeStrcat((S8*)filename, (S8*)L".");
      pfnUnicodeStrcat((S8*)filename, (S8*)file_info.file_ext);
   }
   else
   {
      mmi_sndrec_append_file_ext(filename);
   }
#endif
}
*/
 

void mmi_sndrec_increase_file_name(UI_string_type filename)
{
   UI_string_type p;
   U16 fileCount=0;

   p=filename+pfnUnicodeStrlen((const S8*)filename);
   while(p>filename)
   {
      if (*p==(UI_character_type)_T_MMI'.')
      {
         fileCount=(*(p-2)-'0')*10+(*(p-1)-'0');
         fileCount++;
         *(p-1)=fileCount%10+'0';
         *(p-2)=fileCount/10+'0';
         break;
      }
      p--;
   }
}


BOOL mmi_sndrec_check_file_exist(UI_string_type filename)
{
#ifdef MMI_ON_HARDWARE_P
   FS_HANDLE fd;
   fd=MMI_FS_Open((const UINT8 *)filename,FS_READ_ONLY);
   mmi_trace(1," mmi_sndrec_check_file_exist fd=%d",fd);
   if(fd>=0)
   {
      MMI_FS_Close(fd);
      return TRUE;
   }
   else
      return FALSE;
#else
      return FALSE;
#endif
}

void mmi_sndrec_get_new_file(UI_string_type filename, UI_string_type fullname)  //chenhe,we get a ansii full name 
{
#ifdef MMI_ON_HARDWARE_P
   t_rtc rtc_time;
   int i, j;
   S8 tmpPath[MAX_STORAGE_PATH_LEN],tmpFullname[256];

   mmi_sndrec_create_sound_folder();
   RTC_GetTime(&rtc_time);
#ifdef _WIN32
	rtc_time.rtc_year = 123;
   rtc_time.rtc_mon = 3;
   rtc_time.rtc_day = 6;
   rtc_time.rtc_hour = 12;
   rtc_time.rtc_min = 34;
   rtc_time.rtc_sec = 56;
#endif

#ifdef __MMI_SOUND_RECORDER_STYLE_X__
   kal_wsprintf((kal_wchar*)filename, "V%02d%02d%04d_%02d%02d_00", 
               rtc_time.rtc_day, rtc_time.rtc_mon, rtc_time.rtc_year + 2000,
               rtc_time.rtc_hour, rtc_time.rtc_min);
#else
   kal_wsprintf((kal_wchar*)filename, "%02d%02d%02d%02d00", 
               rtc_time.rtc_mon, rtc_time.rtc_day,
               rtc_time.rtc_hour, rtc_time.rtc_min);
#endif
  // kal_wsprintf((kal_wchar*)fullname,"%s%s", FileList_p->storagePath[Settings_p->storage],(const kal_wchar*)filename);//chenhe,%w change to %s
  UnicodeToAnsii(tmpPath,(S8 *)FileList_p->storagePath[Settings_p->storage]);
mmi_trace(g_sw_SREC,"mmi_sndrec_get_new_file,filename is %s,tmpPath is %s,FileList_p->storagePath[Settings_p->storage] is %s",filename,tmpPath,FileList_p->storagePath[Settings_p->storage]);

	sprintf((char *)fullname,"%s%s", tmpPath,(S8 *)filename);//chenhe,%w change to %s
mmi_trace(g_sw_SREC,"mmi_sndrec_get_new_file,fullname is %s",fullname);
   mmi_sndrec_append_file_ext(fullname);
mmi_trace(g_sw_SREC,"mmi_sndrec_get_new_file,fullname is %s",fullname);

   AnsiiToUnicodeString(tmpFullname,(S8 *)fullname);
   memset(fullname,0x0,sizeof(fullname));
   pfnUnicodeStrcpy((S8 *)fullname,tmpFullname);
   mmi_trace(g_sw_SREC,"mmi_sndrec_get_new_file,tmpFullname is %s",tmpFullname);
  for( i = j = 0; mmi_sndrec_check_file_exist(fullname); i++ ) //chenhe test for jasperII
//    for( i = j = 0; i<1; i++ )
   {
      // this check prevent from infinite loop if 00~99 are all used
      if( i == 100 )
      {
         j++;
#ifdef __MMI_SOUND_RECORDER_STYLE_X__
         kal_wsprintf((kal_wchar*)filename, "V%02d%02d%04d_%02d%02d_%d00",
            rtc_time.rtc_day, rtc_time.rtc_mon, rtc_time.rtc_year + 2000,
            rtc_time.rtc_hour, rtc_time.rtc_min, j);
#else
         kal_wsprintf((kal_wchar*)filename, "%02d%02d%02d%02d%d00", 
            rtc_time.rtc_mon, rtc_time.rtc_day,
            rtc_time.rtc_hour, rtc_time.rtc_min, j);
#endif
         i = -1;
      }
      else
         mmi_sndrec_increase_file_name(fullname);
   }
   
#endif         
}

void mmi_sndrec_get_new_file_name(UI_string_type filename)
{
   SoundRecorder.inCall=TRUE;
   mmi_sndrec_get_new_file(SoundRecorder.fileName, filename);
}

void mmi_sndrec_get_current_file(UI_string_type filename, UI_string_type fullname)
{
#ifdef MMI_ON_HARDWARE_P
   if(currFileIndex<FileList_p->noOfEntries)
   {
   	FMGR_FILE_INFO_STRUCT file_info;
	   mmi_fmgr_kernel_get_file_info( (U16) currFileIndex, &file_info);
      pfnUnicodeStrcpy((S8*)filename, (S8*)file_info.file_name);
      //kal_wsprintf(fullname, "%w%w", FileList_p->storagePath[Settings_p->storage], filename);
	 pfnUnicodeStrcpy((S8 *)fullname,(const S8 *)FileList_p->storagePath[Settings_p->storage]);
	 pfnUnicodeStrcat((S8 *)fullname,(const S8 *)filename);
   }
#endif         
}

FMGR_FILTER mmi_sndrec_get_current_file_format(void)
{
	FMGR_FILTER filter;
#ifdef MMI_ON_HARDWARE_P
   if(currFileIndex<FileList_p->noOfEntries)
   {
   	FMGR_FILE_INFO_STRUCT file_info;
	   mmi_fmgr_kernel_get_file_info( (U16) currFileIndex, &file_info);

      return file_info.file_type;
   }

	FMGR_FILTER_INIT(&filter);
#else
	memset( &filter, 0, sizeof(FMGR_FILTER) );
#endif
   return filter;
}

U16 mmi_sndrec_get_menuId( void )
{
	U16 menuId;
	FMGR_FILTER filter;
	currFileIndex = mmi_fmgr_kernel_get_cur_idx();
	FileList_p->noOfEntries=mmi_fmgr_kernel_get_file_count();
	filter = mmi_sndrec_get_current_file_format();
	
	if( FMGR_FILTER_IS_SET( &filter, FMGR_TYPE_WAV ) )
	{
#ifdef MT6205B
		menuId = MENU_ID_SNDREC_OPTION_5;
#else
		menuId = MENU_ID_SNDREC_OPTION_2; /* record, play,rename, delete, delete all, settings, forward */
#endif
	}
	else
	{
#ifdef MT6205B
		menuId = MENU_ID_SNDREC_OPTION_3;
#else
		menuId = MENU_ID_SNDREC_OPTION; /* record, play, append, rename, delete, delete all, settings, forward */
#endif
	}
	return menuId;
}
 
BOOL mmi_sndrec_is_file_exist(void)
{
   if(mmi_sndrec_check_file_exist(SoundRecorder.fullName))
      return TRUE;
   else
      return FALSE;
}

#define SR_INTERRUPT_HANDLE

void mmi_sndrec_abort(void)
{
   if(SoundRecorder.state!=STATE_IDLE){
      SoundRecorder.cmdProcessing = FALSE;
      if (SoundRecorder.state==STATE_RECORD ||
         SoundRecorder.state==STATE_RECORD_PAUSED)
      {   
         SoundRecorder.state=STATE_IDLE;
         mmi_sndrec_stop_record();
      }   
      else
      {
         SoundRecorder.state=STATE_IDLE;
         mmi_sndrec_stop_play();
      }
      SoundRecorder.cmdProcessing = FALSE;
   }
}
extern void record_time();
extern void set_time_update();
extern void time_update();
void mmi_sndrec_exit_hdlr(void)
{
   SoundRecorder.inCall = FALSE;
   SoundRecorder.isAppend = FALSE;

   mmi_sndrec_abort();
}

void mmi_sndrec_set_exit_hdlr(U16 scrnID, FuncPtr exitFuncPtr, FuncPtr entryFuncPtr)
{
	current_scr_id = scrnID;
	current_entry_func = entryFuncPtr;
	ReplaceNewScreenHandler(scrnID, exitFuncPtr, NULL);
}


U8 mmi_sndrec_battery_status_interrupt_hdlr(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
	mmi_eq_battery_status_ind_struct *p=(mmi_eq_battery_status_ind_struct*) inMsg;

	switch(p->battery_status)
	{
		case PMIC_CHARGER_IN://charger connected
		case PMIC_CHARGER_OUT://charger out
		case PMIC_CHARGE_COMPLETE://battery full charged.
         SoundRecorder.exitToStop=FALSE;
         //playRequestedTone(AUX_TONE);
         SetInterruptPopupDisplay(POPUP_TONE_ON);
         return FALSE;
		case PMIC_VBAT_STATUS://battery voltage status
      /* Abnormal case*/
		case PMIC_INVALID_BATTERY:     
		case PMIC_OVERBATTEMP:         
		case PMIC_OVERVOLPROTECT:     
		case PMIC_OVERCHARGECURRENT: 
		case PMIC_LOWBATTEMP:          
		case PMIC_CHARGING_TIMEOUT:   
		case PMIC_INVALID_CHARGER:     
		case PMIC_LOWCHARGECURRENT:	  
		case PMIC_CHARGE_BAD_CONTACT:
		case PMIC_BATTERY_BAD_CONTACT:
	   default:
         return FALSE;
	}	
#else
	return FALSE;
#endif	
}

U8 mmi_sndrec_sms_interrupt_hdlr(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
         SetInterruptPopupDisplay(POPUP_ALL_OFF);
         return FALSE;
#else
	  return FALSE;
#endif	
}

U8 mmi_sndrec_earphone_interrupt_hdlr(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
	 SetInterruptPopupDisplay(POPUP_ALL_OFF);
	 return FALSE;
#else
	 return FALSE;
#endif	
}

/*****************************************************************************
* FUNCTION
*	mmi_sndrec_post_interrupt_hdlr
* DESCRIPTION
*  Resume popup, will be called after all interrupt
* PARAMETERS
*	a IN  message pointer (not used)
* RETURNS
*  U8
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
static U8 mmi_sndrec_post_interrupt_hdlr(void* inMsg)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetInterruptPopupDisplay(POPUP_ALL_ON);
	return FALSE;
}


U8 mmi_sndrec_enabled_interrupt_hdlr(void* inMsg)
{
   return FALSE;
}


void mmi_sndrec_set_interrupt_hdlr(void)
{
   SoundRecorder.exitToStop=TRUE;
   SetInterruptEventHandler(mmi_sndrec_battery_status_interrupt_hdlr,  NULL, PRT_BATTERY_STATUS_IND);
   /* msg */
   SetInterruptEventHandler(mmi_sndrec_sms_interrupt_hdlr, mmi_sndrec_post_interrupt_hdlr, PRT_MSG_ID_MMI_SMS_DELIVER_MSG_IND);
   /*ear phone */
   SetInterruptEventHandler(mmi_sndrec_earphone_interrupt_hdlr, mmi_sndrec_post_interrupt_hdlr, PRT_EQ_GPIO_DETECT_IND);
   //SetGroupInterruptEventHandler(mmi_sndrec_enabled_interrupt_hdlr, EnableInterruptTable, NO_ENABLE_INT);
}


#define SR_RECORD


void mmi_sndrec_entry_record(void)
{
   U8* guiBuffer;
   U32 duration = mmi_sndrec_get_duration_ticks();
   
   ClearAllKeyHandler();
   mmi_sndrec_set_interrupt_hdlr();
   guiBuffer = GetCurrGuiBuffer(SCR_ID_SNDREC_RECORD);
#ifdef __MMI_SOUND_RECORDER_STYLE_X__
   ShowCategory223Screen(STR_ID_SNDREC_NEW_RECORD, MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
                        STR_ID_SNDREC_OPTION_SAVE,0,
                        STR_ID_SNDREC_PAUSE, 0, 
                        IMG_ID_SNDREC_RECORDING, 1,
                        duration, guiBuffer);
   SetLeftSoftkeyFunction(mmi_sndrec_stop_record, KEY_EVENT_UP);
   SetRightSoftkeyFunction(mmi_sndrec_pause, KEY_EVENT_UP);
#else
   ShowCategory223Screen(STR_ID_SNDREC_RECORD, MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
                        STR_ID_SNDREC_PAUSE,0,
                        STR_ID_SNDREC_STOP, 0, 
                        IMG_ID_SNDREC_RECORDING, 1,
                        duration, guiBuffer);
   SetLeftSoftkeyFunction(mmi_sndrec_pause, KEY_EVENT_UP);
   SetRightSoftkeyFunction(mmi_sndrec_stop_record, KEY_EVENT_UP);
#endif
   mmi_sndrec_set_exit_hdlr(SCR_ID_SNDREC_RECORD, mmi_sndrec_exit_hdlr, mmi_sndrec_entry_record);
   mmi_sndrec_set_sidekey_handler();
}

void mmi_sndrec_entry_record_paused(void)
{
   U8* guiBuffer;
   U16 title;
   U32 duration = mmi_sndrec_get_duration_ticks();
   mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_entry_record_paused,");

   mmi_sndrec_set_interrupt_hdlr();
   guiBuffer = GetCurrGuiBuffer(SCR_ID_SNDREC_RECORD_PAUSED);
#ifdef __MMI_SOUND_RECORDER_STYLE_X__
   if( SoundRecorder.isAppend )
      title = STR_ID_SNDREC_APPEND;
   else
      title = STR_ID_SNDREC_NEW_RECORD;
   ShowCategory223Screen(title, MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
                        STR_ID_SNDREC_OPTION_SAVE,0,
                        STR_ID_SNDREC_RESUME, 0, 
                        IMG_ID_SNDREC_RECORDING_PAUSED, 0, 
                        duration, guiBuffer);
   SetLeftSoftkeyFunction(mmi_sndrec_stop_record, KEY_EVENT_UP);
   SetRightSoftkeyFunction(mmi_sndrec_resume, KEY_EVENT_UP);
#else
   if( SoundRecorder.isAppend )
      title = STR_ID_SNDREC_APPEND;
   else
      title = STR_ID_SNDREC_RECORD;
   ShowCategory223Screen(title, MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
                        STR_ID_SNDREC_RESUME,0,
                        STR_ID_SNDREC_STOP, 0, 
                        IMG_ID_SNDREC_RECORDING_PAUSED, 0, 
                        duration, guiBuffer);
   SetLeftSoftkeyFunction(mmi_sndrec_resume, KEY_EVENT_UP);
   SetRightSoftkeyFunction(mmi_sndrec_stop_record, KEY_EVENT_UP);
#endif
   mmi_sndrec_set_exit_hdlr(SCR_ID_SNDREC_RECORD_PAUSED, mmi_sndrec_exit_hdlr, mmi_sndrec_entry_record_paused);
   mmi_sndrec_set_sidekey_handler();
}

void mmi_sndrec_record_stopped_hdlr(void)
{
   UI_string_type filename;
   SoundRecorder.isAppend=FALSE;
   SoundRecorder.isSlefStop=FALSE;
  mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_record_stopped_hdlr");
   filename=mmi_sndrec_get_file_name(SoundRecorder.fullName);
   memcpy(editorBuffer, filename, (MAX_INPUT_FILE_LEN+2)*ENCODING_LENGTH);
   mmi_sndrec_remove_file_ext(editorBuffer);
   /* delete the same editor screens if in history */
   mmi_sndrec_delete_name_editor_history_nodes();

   if( IsScreenPresent( GetCmMarkerScrnID() ) )
        mmi_sndrec_is_rename_editor_in_call = 1;

   mmi_sndrec_entry_edit_name();
}

void mmi_sndrec_entry_edit_name(void)
{
   U8* guiBuffer;

   EntryNewScreen( SCR_ID_SNDREC_RENAME, NULL, mmi_sndrec_entry_edit_name, NULL );
   ClearAllKeyHandler();

   guiBuffer = GetCurrGuiBuffer(SCR_ID_SNDREC_RENAME);
   ShowCategory5Screen(STR_ID_SNDREC_OPTION_SAVE, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
               STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
               STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
               INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
               (U8*) editorBuffer, MAX_INPUT_FILE_LEN+1, 
               guiBuffer);
   SetLeftSoftkeyFunction(mmi_sndrec_entry_rename_option, KEY_EVENT_UP);
   SetCategory5RightSoftkeyFunction(mmi_sndrec_entry_abort_query, KEY_EVENT_UP);

	#if defined(__MMI_GB__)
	GBSetInputboxLSKFunction(mmi_sndrec_entry_rename_option); 
	#endif

	#ifdef __MMI_TOUCH_SCREEN__
	mmi_pen_editor_set_inputbox_LSK_Function(mmi_sndrec_entry_rename_option);
	#endif

   RegisterInputMethodScreenCloseFunction(mmi_sndrec_go_back_2_history);

}

void mmi_sndrec_entry_abort_query(void)
{
   DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO, 
               get_string(STR_ID_SNDREC_QUERY_ABORT), IMG_GLOBAL_QUESTION, WARNING_TONE);

   SetLeftSoftkeyFunction(mmi_sndrec_abort_file, KEY_EVENT_UP);
   SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void mmi_sndrec_abort_file(void)
{
#ifdef MMI_ON_HARDWARE_P
   S32 result;

   result = MMI_FS_Delete((const UINT8 *)SoundRecorder.fullName);
   DeleteScreenIfPresent(SCR_ID_SNDREC_RENAME);
   if(result<0)
      DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_WARNING, 1, 1000, (U8)WARNING_TONE);
   else
#ifdef __MMI_SOUND_RECORDER_STYLE_X__
   if( !IsScreenPresent( GetCmMarkerScrnID() ) && !mmi_sndrec_is_rename_editor_in_call )
      GoBackToHistory( SCR_ID_SNDREC_MAIN );
   else
#endif
      GoBackHistory();
   mmi_sndrec_is_rename_editor_in_call = 0;
#endif
}

void mmi_sndrec_send_record_req(void)
{
//at_parseOneInstruction("abc.txt");
//return;
   S8 tmpFullName[128]={'\0'};
	TBM_ENTRY(Rec_stub_2);
   mmi_trace(g_sw_SREC, "mmi_sndrec_send_record_req,Settings_p->storage is %d",Settings_p->storage);
   /*Check if destination folder exists before record.*/
   if(!mmi_sndrec_check_dest_folder(Settings_p->storage))
      return;
  mdi_audio_stop_all();
   ClearAllKeyHandler();
   mmi_sndrec_get_new_file(SoundRecorder.fileName, SoundRecorder.fullName);
    mmi_trace(g_sw_SREC, "mmi_sndrec_send_record_req,fileName is %s,full name is %s",SoundRecorder.fileName,SoundRecorder.fullName);
   // strcpy(tmpFullName,SoundRecorder.fullName);
	//memset(SoundRecorder.fullName,0x0,sizeof(SoundRecorder.fullName));	
	//AnsiiToUnicodeString(SoundRecorder.fullName,tmpFullName);
	UnicodeToAnsii(tmpFullName,(S8 *)SoundRecorder.fullName);

    mmi_trace(g_sw_SREC, "mmi_sndrec_send_record_req,SoundRecorder.fullName is %s",tmpFullName);
	   	UnicodeToAnsii(tmpFullName,(S8 *)SoundRecorder.fileName);
    mmi_trace(g_sw_SREC, "mmi_sndrec_send_record_req,SoundRecorder.fileName is %s",tmpFullName);

	
   mmi_sndrec_start_record(SoundRecorder.fullName, (U8)(
#ifndef MT6205B
#if !defined( __MMI_DEDI_AMR_REC__ )
	SoundRecorder.inCall ? FORMAT_WAV:
#elif defined( AMRWB_ENCODE ) && defined( AMRWB_DECODE )
	(SoundRecorder.inCall && SoundRecorder.Settings.format == (U8)FORMAT_AWB) ? FORMAT_AMR:
#endif
#endif
   	SoundRecorder.Settings.format )
   );
   mmi_sndrec_set_interrupt_hdlr();
   /* To stop the play when interrupt happened before play confirm */
   ReplaceNewScreenHandler(SCR_ID_SNDREC_RECORD, mmi_sndrec_exit_hdlr, NULL);

   	TBM_EXIT(Rec_stub_2);

}


void mmi_sndrec_hightlight_record(void)
{
   /*1 Change left soft key icon and label */
   ChangeLeftSoftkey(STR_GLOBAL_OK,0);

   /*2 Change right soft key icon and label */
   ChangeRightSoftkey(STR_GLOBAL_BACK,0);

   /*3 Register function for left soft key */
   SetLeftSoftkeyFunction(mmi_sndrec_send_record_req,KEY_EVENT_UP);

   /*4 Register function for right soft key */
   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   /*5 Register function for right  key */
   ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

   /*6 Register function for left  key */
   SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

#define SR_PLAY

void mmi_sndrec_entry_play(void)
{
   U8* guiBuffer;
   U32 duration = mmi_sndrec_get_duration_ticks();

   ClearAllKeyHandler();
   mmi_sndrec_set_interrupt_hdlr();
   guiBuffer = GetCurrGuiBuffer(SCR_ID_SNDREC_PLAY);       
   ShowCategory223Screen(STR_ID_SNDREC_PLAY, MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
                        STR_ID_SNDREC_PAUSE,0,
                        STR_ID_SNDREC_STOP, 0, 
                        IMG_ID_SNDREC_PLAYING, 1, 
                        duration, guiBuffer);
   SetLeftSoftkeyFunction(mmi_sndrec_pause, KEY_EVENT_UP);
   SetRightSoftkeyFunction(mmi_sndrec_stop_play, KEY_EVENT_UP);
   mmi_sndrec_set_exit_hdlr(SCR_ID_SNDREC_PLAY, mmi_sndrec_exit_hdlr, mmi_sndrec_entry_play);
   mmi_sndrec_set_sidekey_handler();
}



void mmi_sndrec_hightlight_play(void)
{
   /*1 Change left soft key icon and label */
   ChangeLeftSoftkey(STR_GLOBAL_OK,0);

   /*2 Change right soft key icon and label */
   ChangeRightSoftkey(STR_GLOBAL_BACK,0);

   /*3 Register function for left soft key */
   SetLeftSoftkeyFunction(mmi_sndrec_send_play_req,KEY_EVENT_UP);

   /*4 Register function for right soft key */
   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   /*5 Register function for right  key */
   SetKeyHandler(mmi_sndrec_send_play_req,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

   /*6 Register function for left  key */
   SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

void mmi_sndrec_entry_play_paused(void)
{
   U8* guiBuffer;
   U32 duration = mmi_sndrec_get_duration_ticks();

   mmi_sndrec_set_interrupt_hdlr();
   guiBuffer = GetCurrGuiBuffer(SCR_ID_SNDREC_PLAY_PAUSED);       
   ShowCategory223Screen(STR_ID_SNDREC_PLAY, MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
                        STR_ID_SNDREC_RESUME,0,
                        STR_ID_SNDREC_STOP, 0, 
                        IMG_ID_SNDREC_PLAYING_PAUSED, 0, 
                        duration, guiBuffer);
   SetLeftSoftkeyFunction(mmi_sndrec_resume, KEY_EVENT_UP);
   SetRightSoftkeyFunction(mmi_sndrec_stop_play, KEY_EVENT_UP);
   mmi_sndrec_set_exit_hdlr(SCR_ID_SNDREC_PLAY_PAUSED, mmi_sndrec_exit_hdlr, mmi_sndrec_entry_play_paused);
   mmi_sndrec_set_sidekey_handler();
}

#if 0
void rec_start_play()
{
	mmi_sndrec_start_play(SoundRecorder.fullName);
	ReplaceNewScreenHandler(SCR_ID_SNDREC_PLAY, mmi_sndrec_exit_hdlr, NULL);
}
#endif
void mmi_sndrec_send_play_req(void)
{
   ClearAllKeyHandler();
   mmi_sndrec_get_current_file(SoundRecorder.fileName, SoundRecorder.fullName);
   mmi_sndrec_set_interrupt_hdlr();

  mdi_audio_stop_all();
  
     /* To stop the play when interrupt happened before play confirm */
   ReplaceNewScreenHandler(SCR_ID_SNDREC_PLAY, mmi_sndrec_exit_hdlr, NULL);//chenhe place the screen handle function before start_play;as the end_file ouccur immediately,so the handler will not excute as soon
   mmi_sndrec_start_play(SoundRecorder.fullName);

}



#define SR_APPEND

void mmi_sndrec_entry_append(void)
{
   U8* guiBuffer;
   U32 duration = mmi_sndrec_get_duration_ticks();
   ClearAllKeyHandler();
   mmi_sndrec_set_interrupt_hdlr();
   guiBuffer = GetCurrGuiBuffer(SCR_ID_SNDREC_RECORD);
#ifdef __MMI_SOUND_RECORDER_STYLE_X__
   ShowCategory223Screen(STR_ID_SNDREC_APPEND, MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
                        STR_ID_SNDREC_OPTION_SAVE,0,
                        STR_ID_SNDREC_PAUSE, 0, 
                        IMG_ID_SNDREC_RECORDING, 1,
                        duration, guiBuffer);
   SetLeftSoftkeyFunction(mmi_sndrec_stop_record, KEY_EVENT_UP);
   SetRightSoftkeyFunction(mmi_sndrec_pause, KEY_EVENT_UP);
#else
   ShowCategory223Screen(STR_ID_SNDREC_APPEND, MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
                        STR_ID_SNDREC_PAUSE,0,
                        STR_ID_SNDREC_STOP, 0, 
                        IMG_ID_SNDREC_RECORDING, 1,
                        duration, guiBuffer);
   SetLeftSoftkeyFunction(mmi_sndrec_pause, KEY_EVENT_UP);
   SetRightSoftkeyFunction(mmi_sndrec_stop_record, KEY_EVENT_UP);
#endif
   mmi_sndrec_set_exit_hdlr(SCR_ID_SNDREC_RECORD, mmi_sndrec_exit_hdlr, mmi_sndrec_entry_record);
   mmi_sndrec_set_sidekey_handler();
}

void mmi_sndrec_hightlight_append(void)
{
   /*1 Change left soft key icon and label */
   ChangeLeftSoftkey(STR_GLOBAL_OK,0);

   /*2 Change right soft key icon and label */
   ChangeRightSoftkey(STR_GLOBAL_BACK,0);

   /*3 Register function for left soft key */
   SetLeftSoftkeyFunction(mmi_sndrec_send_append_req,KEY_EVENT_UP);

   /*4 Register function for right soft key */
   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   /*5 Register function for right  key */
   ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

   /*6 Register function for left  key */
   SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

void mmi_sndrec_send_append_req(void)
{
   /*Check if destination folder exists before record.*/
   if(!mmi_sndrec_check_dest_folder(Settings_p->storage))
      return;

   SoundRecorder.isAppend=TRUE;
   ClearAllKeyHandler();
   mmi_sndrec_get_current_file(SoundRecorder.fileName, SoundRecorder.fullName);
   mmi_sndrec_set_interrupt_hdlr();
   mdi_audio_stop_all();
   mmi_sndrec_start_record(SoundRecorder.fullName, Settings_p->format);/* if no file ext, use default format */
   /* To stop the play when interrupt happened before play confirm */
   ReplaceNewScreenHandler(SCR_ID_SNDREC_RECORD, mmi_sndrec_exit_hdlr, NULL);
}

#define SR_RENAME


void mmi_sndrec_entry_rename_option(void)
{
   U8* guiBuffer;
   U16 nStrItemList[10];
   U16 numItems;
   U16 itemIcons[10];
   U8 i;

   EntryNewScreen( SCR_ID_SNDREC_RENAME_OPTION, NULL, mmi_sndrec_entry_rename_option, NULL );

   ClearAllKeyHandler();
   guiBuffer = GetCurrGuiBuffer(SCR_ID_SNDREC_RENAME_OPTION);       
   SetParentHandler(MENU_ID_SNDREC_RENAME_OPTION);

   RegisterHighlightHandler(ExecuteCurrHiliteHandler);
   numItems = GetNumOfChild(MENU_ID_SNDREC_RENAME_OPTION);
   GetSequenceStringIds(MENU_ID_SNDREC_RENAME_OPTION, nStrItemList);
   for(i=0; (i<numItems); i++)
   {
      itemIcons[i] = IMG_GLOBAL_L1 + i;
   }
   ShowCategory52Screen(STR_ID_SNDREC_RENAME_OPTION, MAIN_MENU_TITLE_MULTIMEDIA_ICON, 
                          STR_GLOBAL_OK, IMG_GLOBAL_OK,
                          STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
                          numItems, nStrItemList, itemIcons,
                          NULL , 0, 0, guiBuffer );
}

/* ------------------------beginning kecx added on 20080819---------------------------- */
/**************************************************************
	FUNCTION NAME		: snd_strElementNum

	PURPOSE		               : count the element No. of UNICODE string.

	INPUT PARAMETERS        : UI_string_type  srcStr	

	OUTPUT PARAMETERS	:  

	RETURNS		               : UINT32  count

 
**************************************************************/
UINT32 snd_strElementNum(UI_string_type srcStr)
{
	UINT32 count=0;

	ASSERT(srcStr != NULL);
	TRACE_EF(g_sw_SREC, "snd_strElementNum, srcStr[0]=%x",*srcStr);

	while(*srcStr++ != 0)
			count++;

	return count;
}

/**************************************************************

	FUNCTION NAME		: snd_strnchar

  	PURPOSE				: count the No. of deschar in sourceStr.

	INPUT PARAMETERS	       : UI_string_type  sourceStr, s8  deschar

	OUTPUT PARAMETERS	: 

	RETURNS				: UINT32  vret

 

**************************************************************/
UINT32 snd_strnchar(UI_string_type sourceStr, U8 deschar)
{
	UINT16 vi=0,vsearchi=0,vret=0;
	U16 DesChar=0;

	ASSERT(sourceStr != NULL);
	DesChar = 0x00FF&((U16)deschar);

	vi= snd_strElementNum(sourceStr);
	for(vsearchi=0;vsearchi<vi;vsearchi++)
	{
		if( *(sourceStr+vsearchi)== DesChar)
			vret++;
	}
	return vret;
}

/**************************************************************

	FUNCTION NAME		: findCharInUnicodeStr

  	PURPOSE				: search subChar in srcStr.

	INPUT PARAMETERS	       : UI_string_type  srcStr, U8  subChar	

	OUTPUT PARAMETERS	:  

	RETURNS				: BOOL

 

**************************************************************/
BOOL findCharInUnicodeStr(UI_string_type srcStr, U8 subChar) 
{
	UINT16 vi=0, i=0;
	U16 desChar=0;

	ASSERT(srcStr != NULL);
	desChar = 0x00FF&((U16)subChar);    /* srcStr is big endian, kecx commented on 20080819 */

	TRACE_EF(g_sw_SREC, "desChar=0x%x, char=0x%x",desChar,subChar);

	vi=snd_strElementNum(srcStr);
	for(i=0; i<vi; i++)
	{
		if( srcStr[i]== desChar) return TRUE;
	}

	return FALSE;	
}
/* ------------------------end kecx added on 20080819---------------------------- */
void mmi_sndrec_entry_save(void)
{
#ifdef MMI_ON_HARDWARE_P
   S32 result;
   UI_character_type fullname[MAX_FULL_NAME_LEN];
   
TBM_ENTRY(Rec_stub_4);
   if( editorBuffer[0] == 0 )
   {
#ifdef __MMI_SOUND_RECORDER_STYLE_X__   
      mmi_sndrec_entry_abort_query();
#else
      DisplayPopup((PU8)GetString(STR_ID_SNDREC_EMPTY_FILENAME), IMG_GLOBAL_WARNING, 1, 1000, (U8)WARNING_TONE);
      DeleteScreenIfPresent(SCR_ID_SNDREC_RENAME_OPTION);
#endif
      return;
   }
   else 
   {
     	pfnUnicodeStrcpy((S8 *)fullname,(const S8 *)editorBuffer);     /* beginning kecx modified on 20080819*/
     	U8 spaceChar = ' ';
     	TRACE_EF(g_sw_SREC, "in mmi_sndrec_entry_save, snd_strElementNum=%d",snd_strElementNum(fullname));
     	TRACE_EF(g_sw_SREC, "in mmi_sndrec_entry_save, snd_strnchar=%d",snd_strnchar(fullname,spaceChar));
     
     	if(snd_strElementNum(fullname) == snd_strnchar(fullname,spaceChar))
     	{
     		DisplayPopup((PU8)GetString(STR_ID_SNDREC_EMPTY_FILENAME), IMG_GLOBAL_WARNING, 1, 1000, (U8)WARNING_TONE);
     		DeleteScreenIfPresent(SCR_ID_SNDREC_RENAME_OPTION);
     		return;
     	}
     	else if( findCharInUnicodeStr(fullname,'.')||findCharInUnicodeStr(fullname,'\n')||findCharInUnicodeStr(fullname,'\r') )   /* end kecx modified on 20080819*/
     	{
     		DisplayPopup((PU8)GetString(FMGR_FS_INVALID_FILENAME_TEXT), IMG_GLOBAL_WARNING, 1, 1000, (U8)WARNING_TONE);
     		DeleteScreenIfPresent(SCR_ID_SNDREC_RENAME_OPTION);
     		return;		
     	}
   }
   //kal_wsprintf(fullname, "%w%w.%w", FileList_p->storagePath[SoundRecorder.Settings.storage], editorBuffer, extBuffer );
   pfnUnicodeStrcpy((S8 *)fullname,(const S8 *)FileList_p->storagePath[SoundRecorder.Settings.storage]);
   pfnUnicodeStrcat((S8 *)fullname,"/");
   pfnUnicodeStrcat((S8 *)fullname,(const S8 *)editorBuffer);
   pfnUnicodeStrcat((S8 *)fullname,".");
   pfnUnicodeStrcat((S8 *)fullname,(const S8 *)extBuffer);
   result = MMI_FS_Rename((const UINT8 *)SoundRecorder.fullName, (const UINT8 *)fullname);

   mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_entry_save,result is %d",result);
   if(  (result<0)&&( result != ERR_FS_FILE_EXIST)&&FS_FILE_EXISTS != result  )
   {
      DisplayPopup((PU8)GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_WARNING, 1, 1000, (U8)WARNING_TONE);
      DeleteScreenIfPresent(SCR_ID_SNDREC_RENAME_OPTION);
      return;
   }
   else if( !IsScreenPresent( GetCmMarkerScrnID() ) && !mmi_sndrec_is_rename_editor_in_call )
   {
#ifdef __MMI_SOUND_RECORDER_STYLE_X__
      DeleteUptoScrID( SCR_ID_SNDREC_MAIN );
      mmi_sndrec_entry_main();
      DeleteScreenIfPresent( SCR_ID_SNDREC_RENAME_OPTION );
#else
      //GoBackToHistory( SCR_FMGR_EXPLORER );
      DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, 1000, (U8)SUCCESS_TONE);
      DeleteUptoScrID(SCR_FMGR_EXPLORER);
#endif
   }
   else
   {
      DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, 1000, (U8)SUCCESS_TONE);   	
      DeleteNScrId( SCR_ID_SNDREC_RENAME );
      //GoBackHistory();
   }
   mmi_sndrec_is_rename_editor_in_call = 0;
#endif      

TBM_EXIT(Rec_stub_4);
}


void mmi_sndrec_hightlight_save(void)
{
   /*1 Change left soft key icon and label */
   ChangeLeftSoftkey(STR_GLOBAL_OK,0);

   /*2 Change right soft key icon and label */
   ChangeRightSoftkey(STR_GLOBAL_BACK,0);

   /*3 Register function for left soft key */
   SetLeftSoftkeyFunction(mmi_sndrec_entry_save,KEY_EVENT_UP);

   /*4 Register function for right soft key */
   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   /*5 Register function for right  key */
   ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

   /*6 Register function for left  key */
   SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

void mmi_sndrec_hightlight_input_method(void)
{
   /*1 Change left soft key icon and label */
   ChangeLeftSoftkey(STR_GLOBAL_OK,0);

   /*2 Change right soft key icon and label */
   ChangeRightSoftkey(STR_GLOBAL_BACK,0);

   /*3 Register function for left soft key */
   SetLeftSoftkeyFunction(EntryInputMethodScreen,KEY_EVENT_UP);

   /*4 Register function for right soft key */
   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   /*5 Register function for right  key */
   SetKeyHandler(EntryInputMethodScreen,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

   /*6 Register function for left  key */
   SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


#define SR_SETTINGS

void mmi_sndrec_init_settings(void)
{   
   S16 error;

	TBM_ENTRY(Rec_stub_8);

   Settings_p->storage = STORAGE_PHONE;
#ifndef MT6205B
   Settings_p->format = FORMAT_AMR;
   Settings_p->dataRate[FORMAT_AMR] = 700;  /* 700 bytes per second  */
   Settings_p->dataRate[FORMAT_WAV] = 4055;  /* 4055 bytes per second  */
#else
   //Settings_p->format = FORMAT_VM;
   Settings_p->format = FORMAT_AMR;//chenhe modify for jasperII
#endif
#if defined( AMRWB_ENCODE ) && defined( AMRWB_DECODE )
   Settings_p->dataRate[FORMAT_AWB] = 900;  /*  900 bytes per second */
   Settings_p->dataRate[FORMAT_VM] = 1700;  /* 1700 bytes per second  */ 		// chenxia move above for jasperII
#endif
   Settings_p->dataRate[FORMAT_WAV] = 16*1024;  /* 4055 bytes per second  */
	Settings_p->dataRate[FORMAT_AMR] = 1525;	/*  1525 bytes per second */	//chenxia modify for jasperII
   
#ifdef MMI_ON_HARDWARE_P
   if(MMI_CARD_DRV > 0)
      Settings_p->cardAvailable=TRUE;
   else
      Settings_p->cardAvailable=FALSE;      
   /*+ zhouqin modify for dual t-flash card 20110503*/
   #if defined(DUAL_TFLASH_SUPPORT)
   if(MMI_SEC_CARD_DRV > 0)
      Settings_p->seccardAvailable=TRUE;
   else
      Settings_p->seccardAvailable=FALSE;      
   #endif
   /*- zhouqin modify for dual t-flash card 20110503*/
#endif
   mmi_sndrec_get_settings();
	if(Settings_p->cardAvailable)
	{
	 Settings_p->storage=STORAGE_CARD;//chenhe,20770723
 	WriteValue(NVRAM_SNDREC_STORAGE, &Settings_p->storage, DS_BYTE, &error);

	}
	/*+ zhouqin modify for dual t-flash card 20110503*/
	#if defined(DUAL_TFLASH_SUPPORT)
	else if(Settings_p->seccardAvailable)
	{
        Settings_p->storage=STORAGE_SEC_CARD;
        WriteValue(NVRAM_SNDREC_STORAGE, &Settings_p->storage, DS_BYTE, &error);
	}
	#endif
	/*- zhouqin modify for dual t-flash card 20110503*/

	TBM_EXIT(Rec_stub_8);


}



void mmi_sndrec_get_settings(void)
{
   S16 error;
   ReadValue(NVRAM_SNDREC_STORAGE, &Settings_p->storage, DS_BYTE, &error);
   ReadValue(NVRAM_SNDREC_FORMAT, &Settings_p->format, DS_BYTE, &error);

/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
 if (Settings_p->storage>NO_OF_STORAGE||(!Settings_p->cardAvailable && ( Settings_p->storage== STORAGE_CARD))
         ||(!Settings_p->seccardAvailable && ( Settings_p->storage== STORAGE_SEC_CARD)) )
      Settings_p->storage=STORAGE_PHONE;
#else
   if (Settings_p->storage>NO_OF_STORAGE||!Settings_p->cardAvailable)
      Settings_p->storage=STORAGE_PHONE;
#endif
/*- zhouqin modify for dual t-flash card 20110503*/


   if (Settings_p->format>NO_OF_FORMATS)
#ifdef MT6205B
      Settings_p->format=FORMAT_AMR;
#else
      Settings_p->format=FORMAT_VM;
#endif
	if(Settings_p->storage == STORAGE_PHONE)//chenhe,patch when power on with setting format is wav ,path is phone,we don't allow to save wave to phone now
	{
		Settings_p->format=FORMAT_AMR;
	}
   selectedStorageIndex=Settings_p->storage;
   selectedFormatIndex=Settings_p->format;
}

BOOL mmi_sndrec_is_settings_changed(void)
{
	mmi_trace(1,"mmi_sndrec_is_settings_changed,Settings_p->format is %d",Settings_p->format);
   if (selectedStorageIndex!=Settings_p->storage)
      return TRUE;
   if (selectedFormatIndex!=Settings_p->format)
      return TRUE;
   return FALSE;   
}

void mmi_sndrec_pre_entry_settings(void) 
{
	TBM_ENTRY(Rec_stub_7);

   mmi_sndrec_get_settings();
   mmi_sndrec_entry_inline_edit();

   	TBM_EXIT(Rec_stub_7);

}

void mmi_sndrec_entry_inline_edit(void) 
{
   U8* guiBuffer;
   U8* inputBuffer;
   U16 inputBufferSize;
   U8 i,j;
   U16 DetailImageList[MAX_SR_SETTINGS_FIELDS];

   EntryNewScreen( SCR_ID_SNDREC_SETTINGS, mmi_sndrec_exit_inline_edit, NULL, NULL );
   
   InitializeCategory57Screen();
   guiBuffer = GetCurrGuiBuffer(SCR_ID_SNDREC_SETTINGS);   
   inputBuffer = GetCurrNInputBuffer(SCR_ID_SNDREC_SETTINGS, &inputBufferSize);

   RegisterHighlightHandler(mmi_sndrec_inline_edit_hightlight_hdlr);

   j=0;
   mmi_trace(g_sw_SREC,"mmi_sndrec_entry_inline_edit,Settings_p->cardAvailable is %d",Settings_p->cardAvailable);
   
#ifdef COOLSAND_MULTIMEDIA_USE_FLASHFS   
   //if(Settings_p->cardAvailable)
   //{
      for(i=0;i<NO_OF_STORAGES;i++)
         gStorages[i] = (UI_string_type)GetString(gStorageIds[i]);
      SetInlineItemActivation(&wgui_inline_items[j], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
      SetInlineItemSelect(&wgui_inline_items[j], NO_OF_STORAGES, (U8**)gStorages, &selectedStorageIndex);
      SRTitleStrings[j] = STR_ID_SNDREC_SETTINGS_STORAGE;
      DetailImageList[j++] = IMG_ID_SNDREC_SETTINGS_STORAGE;
   //}
#endif
   
   //gFormats[0] = (UI_string_type)GetString(STR_ID_SNDREC_SETTINGS_AMR);
   //gFormats[1] = (UI_string_type)GetString(STR_ID_SNDREC_SETTINGS_MP3);
   for(i=0;i<NO_OF_FORMATS;i++)
   {
      gFormats[i] = (UI_string_type)GetString(gFormatIds[i]);
   }
   SetInlineItemActivation(&wgui_inline_items[j], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
   SetInlineItemSelect(&wgui_inline_items[j], NO_OF_FORMATS, (U8**)gFormats, &selectedFormatIndex);
   SRTitleStrings[j] = STR_ID_SNDREC_SETTINGS_FORMAT;
   DetailImageList[j++] = IMG_ID_SNDREC_SETTINGS_FORMAT;

   ShowCategory57Screen(STR_ID_SNDREC_SETTINGS, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
           STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
           j, DetailImageList, wgui_inline_items, 0, guiBuffer);

   SetCategory57LeftSoftkeyFunction(mmi_sndrec_settings_pre_save);
   DisableCategory57ScreenDone();
   SetCategory57RightSoftkeyFunctions(GoBackHistory,GoBackHistory);
}


void mmi_sndrec_exit_inline_edit(void) 
{
   history_t   h;
   U16 inputBufferSize;
   
   CloseCategory57Screen();
   h.scrnID = SCR_ID_SNDREC_SETTINGS;
   h.entryFuncPtr = mmi_sndrec_entry_inline_edit;
   GetCategoryHistory(h.guiBuffer);
   inputBufferSize = (U16)GetCategory57DataSize();  
   GetCategory57Data((U8*) h.inputBuffer);       
   AddNHistory(h, inputBufferSize);           
}

void mmi_sndrec_hightlight_delete(void)
{
	TBM_ENTRY(Rec_stub_5);

   ChangeLeftSoftkey(STR_GLOBAL_OK,0);
   ChangeRightSoftkey(STR_GLOBAL_BACK,0);
   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
   ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
   SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
   highlight_fmgr_delete();

   TBM_EXIT(Rec_stub_5);
}

void mmi_sndrec_hightlight_delete_all(void)
{
	TBM_ENTRY(Rec_stub_6);

   ChangeLeftSoftkey(STR_GLOBAL_OK,0);
   ChangeRightSoftkey(STR_GLOBAL_BACK,0);
   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
   ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
   SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
   highlight_fmgr_delete_all_file();

   	TBM_EXIT(Rec_stub_6);

}

void mmi_sndrec_hightlight_settings(void)
{
   /*1 Change left soft key icon and label */
   ChangeLeftSoftkey(STR_GLOBAL_OK,0);

   /*2 Change right soft key icon and label */
   ChangeRightSoftkey(STR_GLOBAL_BACK,0);

   /*3 Register function for left soft key */
   SetLeftSoftkeyFunction(mmi_sndrec_pre_entry_settings,KEY_EVENT_UP);

   /*4 Register function for right soft key */
   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   /*5 Register function for right  key */
   SetKeyHandler(mmi_sndrec_pre_entry_settings,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

   /*6 Register function for left  key */
   SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}



void mmi_sndrec_inline_edit_hightlight_hdlr(S32 index) 
{
   ChangeTitleString((PU8)GetString(SRTitleStrings[index]));
    
   draw_title();
    
   ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
   SetCategory57LeftSoftkeyFunction(mmi_sndrec_settings_pre_save);   
}



void mmi_sndrec_settings_pre_save(void)
{
   if(mmi_sndrec_is_settings_changed())
      mmi_sndrec_entry_settings_save_query();
   else 
      GoBackHistory();
}

void mmi_sndrec_write_settings_to_nvram(void)
{
	S16 error;
	TBM_ENTRY(Rec_stub_15);

	Settings_p->storage=(U8)selectedStorageIndex;
	Settings_p->format=(U8)selectedFormatIndex;

#ifdef SND_NOT_SUPPORT_WAV_ON_FLASH
	if(Settings_p->format == FORMAT_WAV)
	{
	/*+ zhouqin modify for dual t-flash card 20110503*/
    	#if defined(DUAL_TFLASH_SUPPORT)
        if(selectedStorageIndex == STORAGE_SEC_CARD)
            Settings_p->storage=STORAGE_SEC_CARD;
        else
        #endif
        /*- zhouqin modify for dual t-flash card 20110503*/
	    Settings_p->storage=STORAGE_CARD;
	}
#endif

	WriteValue(NVRAM_SNDREC_STORAGE, &Settings_p->storage, DS_BYTE, &error);
	WriteValue(NVRAM_SNDREC_FORMAT, &Settings_p->format, DS_BYTE, &error);
	DeleteNScrId(SCR_FMGR_EXPLORER);

	mmi_sndrec_entry_main();
	//mmi_sndrec_go_back_2_history();

	TBM_EXIT(Rec_stub_15);

}

BOOL mmi_sndrec_get_remain_time_string(UI_string_type string)
{
	mmi_trace(g_sw_SREC, "chenxia!!! fun: mmi_sndrec_get_remain_time_string; string is %s", string);
#ifdef MMI_ON_HARDWARE_P
   kal_int32 result;
   FS_DiskInfo disc_info;
   kal_wchar dir[3];
   S32 sec;
//	char tmpNewline[10];
   kal_mem_set(dir, 0, 6);
   kal_mem_cpy(dir, FileList_p->storagePath[selectedStorageIndex], 4);
   /* check the available free space */
   result=MMI_FS_GetDiskInfo((const UINT8 *)dir, &disc_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
  result = FS_DI_FREE_SPACE;//chenhe for jasperII
   mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_get_remain_time_string,result,%d,",result);
   mmi_trace(g_sw_SREC,"chenhe,mmi_sndrec_get_remain_time_string,dir is %d,,FileList_p->storagePath[selectedStorageIndex], is %c",dir,FileList_p->storagePath[selectedStorageIndex]);
   if((result>0) && (result&(FS_DI_BASIC_INFO|FS_DI_FREE_SPACE)))
   {
     s8 tmpStr[128];
      sec=(disc_info.FreeClusters*disc_info.SectorsPerCluster*disc_info.BytesPerSector)/Settings_p->dataRate[selectedFormatIndex];
	  //sec=(2000000000)/Settings_p->dataRate[selectedFormatIndex];	// chenxia test for jasperII

      //kal_wsprintf(string, "%d:%2d:%2d  ", sec/3600, (sec%3600)/60, sec%60);//chenhe
      sprintf(tmpStr, "%d:%2d:%2d  ", sec/3600, (sec%3600)/60, sec%60);//chenhe
      AnsiiToUnicodeString((S8 *)string,tmpStr);
      pfnUnicodeStrcat((S8*)string, (S8*)GetString(STR_ID_SNDREC_AVAILABLE));

      return TRUE;
   }
   else
   {
      pfnUnicodeStrcpy((S8*)string, (S8*)GetString(/*STR_ID_SNDREC_NO_CARD*/ STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD));
      return FALSE;
   }

#else
   pfnUnicodeStrcpy((S8*)string, (S8*)L" 10:10:10 Available");
   return TRUE;
#endif
}

void mmi_sndrec_entry_settings_save_query(void)
{
   if(selectedStorageIndex == STORAGE_CARD && !Settings_p->cardAvailable)
   	{
		mmi_sndrec_error_hdlr(MDI_AUDIO_NO_DISC);
		return;
   	}
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
     if(selectedStorageIndex == STORAGE_SEC_CARD && !Settings_p->seccardAvailable)
   	{
		mmi_sndrec_error_hdlr(MDI_AUDIO_NO_DISC);
		return;
   	}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
#ifdef SND_NOT_SUPPORT_WAV_ON_FLASH     //modified by kecx for fixing bug 12502 on 20090615
	if((selectedStorageIndex == STORAGE_PHONE)&&(selectedFormatIndex== FORMAT_WAV))
	{
		//	AnsiiToUnicodeString(tmpNewline,"\n");
		//	pfnUnicodeStrcpy((S8*)string, tmpNewline);
		//pfnUnicodeStrcpy((S8*)string, (S8*)GetString(STR_ID_SNDREC_WAV_NOT_ON_FLASH));
		DisplayPopup(GetString(STR_ID_SNDREC_WAV_NOT_ON_FLASH), IMG_GLOBAL_WARNING, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		return;
	}
#endif

   mmi_sndrec_get_remain_time_string(strBuffer);
   DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO, 
                           strBuffer, IMG_GLOBAL_QUESTION, WARNING_TONE);

   SetLeftSoftkeyFunction(mmi_sndrec_write_settings_to_nvram, KEY_EVENT_UP);
   SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void mmi_sndrec_exit_settings_save_query(void)
{
}
 
#define SR_VOLUME

void mmi_sndrec_save_speech_volume(void)
{
   SaveSpeechVolume((U8)SoundRecorder.volume);
}

void mmi_sndrec_increase_speech_volume(void)
{
   if(SoundRecorder.volume<MAX_VOL_LEVEL-1)
   {
      SoundRecorder.volume++;
      mmi_sndrec_set_speech_volume(SoundRecorder.volume); 
      StartTimer(KEY_VOL_TIMER_ID, 2000, mmi_sndrec_save_speech_volume);
   }   
}

void mmi_sndrec_decrease_speech_volume(void)
{
   if(SoundRecorder.volume>0)
   {
      SoundRecorder.volume--;
      mmi_sndrec_set_speech_volume(SoundRecorder.volume);   
      StartTimer(KEY_VOL_TIMER_ID, 2000, mmi_sndrec_save_speech_volume);
   }
}

void mmi_sndrec_increase_media_volume(void)
{
   if(SoundRecorder.volume<MAX_VOL_LEVEL-1)
   {
      SoundRecorder.volume++;
      mmi_sndrec_set_media_volume(SoundRecorder.volume); 
   }
}


void mmi_sndrec_decrease_media_volume(void)
{
   if(SoundRecorder.volume>0)
   {
      SoundRecorder.volume--;
      mmi_sndrec_set_media_volume(SoundRecorder.volume);   
   }   
}

void mmi_sndrec_set_current_volume(void)
{
   mmi_sndrec_set_media_volume(SoundRecorder.volume);   
}


void mmi_sndrec_set_sidekey_handler(void)
{
   if(SoundRecorder.inCall)
   {
      SoundRecorder.volume=GetModeSpeechVolume();
//modify by niej for CS621 adjust volume by key5 and key6  2007/09/14 begin
#if defined(__NO_SIDE_VOLUME_KEY_56__)
	{
	      SetKeyHandler(mmi_sndrec_increase_speech_volume,KEY_6,KEY_EVENT_DOWN);
	      SetKeyHandler(mmi_sndrec_decrease_speech_volume,KEY_5,KEY_EVENT_DOWN);
	}
#elif defined(__NO_SIDE_VOLUME_KEY_46__)
	{
	      SetKeyHandler(mmi_sndrec_increase_speech_volume,KEY_6,KEY_EVENT_DOWN);
	      SetKeyHandler(mmi_sndrec_decrease_speech_volume,KEY_4,KEY_EVENT_DOWN);
	}
#elif defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__)
  {
	      SetKeyHandler(mmi_sndrec_increase_speech_volume,KEY_POUND,KEY_EVENT_DOWN);
	      SetKeyHandler(mmi_sndrec_decrease_speech_volume,KEY_STAR,KEY_EVENT_DOWN);
	}

#else
	{
		SetKeyHandler(mmi_sndrec_increase_speech_volume,KEY_VOL_UP,KEY_EVENT_DOWN);
	       SetKeyHandler(mmi_sndrec_decrease_speech_volume,KEY_VOL_DOWN,KEY_EVENT_DOWN);
	}
#endif  
//modify by niej for CS621 adjust volume by key5 and key6  2007/09/14 end
   }
   else
   {
      //SoundRecorder.volume=GetRingVolumeLevel();
	  //modify by niej for CS621 adjust volume by key5 and key6  2007/09/14 begin
#if defined(__NO_SIDE_VOLUME_KEY_56__)
	{
	      SetKeyHandler(mmi_sndrec_increase_media_volume,KEY_6,KEY_EVENT_DOWN);
	      SetKeyHandler(mmi_sndrec_decrease_media_volume,KEY_5,KEY_EVENT_DOWN);
	}
#elif defined(__NO_SIDE_VOLUME_KEY_46__)
	{
	      SetKeyHandler(mmi_sndrec_increase_media_volume,KEY_6,KEY_EVENT_DOWN);
	      SetKeyHandler(mmi_sndrec_decrease_media_volume,KEY_4,KEY_EVENT_DOWN);
	}
#elif defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__)
    {
	      SetKeyHandler(mmi_sndrec_increase_media_volume,KEY_POUND,KEY_EVENT_DOWN);
	      SetKeyHandler(mmi_sndrec_decrease_media_volume,KEY_STAR,KEY_EVENT_DOWN);
	}
#else
	{
		  SetKeyHandler(mmi_sndrec_increase_media_volume,KEY_VOL_UP,KEY_EVENT_DOWN);
	      SetKeyHandler(mmi_sndrec_decrease_media_volume,KEY_VOL_DOWN,KEY_EVENT_DOWN);
	}
#endif
//modify by niej for CS621 adjust volume by key5 and key6  2007/09/14 end
   }
}

#define SR_MISC

void mmi_sndrec_error_hdlr(S32 cause)
{
#ifdef MMI_ON_HARDWARE_P
   switch(cause)
   {
      case MDI_AUDIO_SUCCESS:
         GoBackHistory();
         break;
#ifdef __DRM_SUPPORT__
      case DRM_NOT_PERMIT_TO_PLAY:
      case DRM_NOT_PERMIT_TO_APPEND:
         DisplayPopup((PU8)GetString(STR_GLOBAL_DRM_PROHIBITED) ,IMG_GLOBAL_UNFINISHED, 1, 1000, ERROR_TONE);
      	  break;
#endif
      case MDI_AUDIO_FAIL:
      case MDI_AUDIO_ERROR:
      	  DisplayPopup((PU8)GetString(STR_ID_SNDREC_ERROR) ,IMG_GLOBAL_UNFINISHED, 1, 1000, ERROR_TONE);
      	  break;
      case MDI_AUDIO_BAD_FORMAT:
      	  DisplayPopup((PU8)GetString(STR_ID_SNDREC_BAD_FORMAT) ,IMG_GLOBAL_UNFINISHED, 1, 1000, ERROR_TONE);
      	  break;
      case MDI_AUDIO_INVALID_FORMAT:
         DisplayPopup((PU8)GetString(STR_ID_SNDREC_INVALID_FORMAT) ,IMG_GLOBAL_UNFINISHED, 1, 1000, ERROR_TONE);
         break;
      case MDI_AUDIO_BUSY:
         DisplayPopup((PU8)GetString(STR_ID_SNDREC_DEVICE_BUSY) ,IMG_GLOBAL_UNFINISHED, 1, 1000, ERROR_TONE);
         break;
      case MDI_AUDIO_DISC_FULL:
      case MDI_AUDIO_NO_SPACE:
//         DisplayPopup((PU8)GetString(STR_ID_SNDREC_DISC_FULL) ,IMG_GLOBAL_UNFINISHED, 1, 1000, ERROR_TONE);
         // use file manager disk full string id for unified error message notification
         DisplayPopup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT) ,IMG_GLOBAL_UNFINISHED, 1, 1000, ERROR_TONE);
         break;
      case MDI_AUDIO_OPEN_FILE_FAIL:
         DisplayPopup((PU8)GetString(STR_ID_SNDREC_OPEN_FILE_ERROR) ,IMG_GLOBAL_UNFINISHED, 1, 1000, ERROR_TONE);
         break;
      case MDI_AUDIO_NO_DISC:
         DisplayPopup((PU8)GetString(/*STR_ID_SNDREC_NO_CARD*/ STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD) ,/*IMG_GLOBAL_UNFINISHED*/ IMG_GLOBAL_WARNING, 1, 1000, WARNING_TONE);
         break;
      case MDI_AUDIO_WRITE_PROTECTION:
      	  DisplayPopup((PU8)GetString(STR_ID_SNDREC_WRITE_PROTECTION_ERROR) ,IMG_GLOBAL_UNFINISHED, 1, 1000, ERROR_TONE);
         break;
      case MDI_AUDIO_END_OF_FILE:
      case MDI_AUDIO_TERMINATED:
         GoBackHistory();
         break;
	case MDI_AUDIO_UNSUPPORTED_CHANNEL:
	      	  DisplayPopup((PU8)GetString(STR_ID_SNDREC_WRITE_PROTECTION_ERROR) ,IMG_GLOBAL_UNFINISHED, 1, 1000, ERROR_TONE);
		break;
	case MDI_AUDIO_UNSUPPORTED_FREQ:
	      	  DisplayPopup((PU8)GetString(STR_ID_SNDREC_WRITE_PROTECTION_ERROR) ,IMG_GLOBAL_UNFINISHED, 1, 1000, ERROR_TONE);
		break;		
      default:
	  GoBackHistory();
	  break;
   }
#endif   
}

void mmi_sndrec_delete_name_editor_history_nodes(void)
{
   if(IsScreenPresent (SCR_ID_SNDREC_RENAME) == TRUE)
   {
      DeleteScreenIfPresent(SCR_ID_SNDREC_RENAME);
      DeleteScreenIfPresent(SCR_ID_SNDREC_RENAME_OPTION);
      DeleteScreenIfPresent(SCR_INPUT_METHOD_LIST);
   }
}

 
void mmi_sndrec_go_back_2_history(void) 
{
   GoBacknHistory(1);
}
/*****************************************************************************
 * FUNCTION
 *  mmi_sndrec_record_is_on
 * DESCRIPTION
 *  querry if sndrec is running
 * PARAMETERS
 *  void
 * RETURNS
 *  MMI_BOOL
 *****************************************************************************/
MMI_BOOL mmi_sndrec_record_is_on(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (SoundRecorder.state != STATE_IDLE)
    {
        return MMI_TRUE;
    }

    return MMI_FALSE;
}
#endif


