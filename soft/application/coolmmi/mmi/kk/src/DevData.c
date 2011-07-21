/*************************************************************************
*	
*	文件：DevData.c
* 描述：客户端数据存储	
* 日期：2005-8-31   	
* 作者：czm	    	    	
* 
*	Copyright (c) 2005 WEP All Rights Reserved.
************************************************************************/

#ifdef MMI_ON_WIN32
//#include "windows.h"
#endif 

#include "devcfg.h"
#include "devdef.h"
#include "interface.h"
#include "utility.h"
#include "devdecl.h"
#include "devdata.h"
#include "wepcam.h"

#include "dual_sim.h"

#ifdef _KK_SUPPORT_ 
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

static bool_t  LD_DataLoad(ILD * pILd);
static bool_t  LD_DataSave(ILD * pILd);

static bool_t  LD_StrDataSet(ILD * pILd, int32_t nId, byte_t *pData, int32_t nLen);
static int32_t LD_StrDataGet(ILD * pILd, int32_t nId, byte_t *pData);

static bool_t  LD_IntDataSet(ILD * pILd, int32_t nId, int32_t iData);
static bool_t  LD_IntDataGet(ILD * pILd, int32_t nId, int32_t *pData);

static int32_t LD_BinDataSet(ILD * pILd, int32_t nId, byte_t *pData, int32_t dataSize);
static int32_t LD_BinDataGet(ILD * pILd, int32_t nId, byte_t *pBuff, int32_t buffSize);
// static bool_t  LD_GPDataSet(ILD * pILd, int32_t gameKind, const byte_t *pData, int32_t nLen);
// static byte_t * LD_GPDataGet(ILD * pILd, int32_t gameKind, int32_t *pDataLen);

static bool_t  LD_NotifySoundPlay(ILD * pILd, int32_t nDelayMs);
//static void    LD_SoundDelaySet(ILD * pILd, int32_t nDelayMs);

static void LD_SoundDestroy(ILD * pILd);
static bool_t LD_SoundDataProcess(ILD * pILd, int32_t sndType, int32_t sndIdx,
			byte_t * sndName, int32_t sndNameSize, byte_t buffer[], int32_t sndSize);
static bool_t LD_DownloadSoundSave(ILD * pILd, int32_t sndType,
			byte_t * sndName, int32_t sndNameSize, byte_t buffer[], int32_t sndSize);
//static void LD_SoundIdSet(ILD *pILD,int32_t soundId) ;
static bool_t LD_SoundDataPlay(ILD * pILd, int32_t sndIdx, bool_t isGame);
static void LD_SoundDataRelease(ILD * pILd);

extern int32_t DEV_GetDiskFreeBytes(const WCHAR *fileName);


#ifdef SUPPORT_FTP

static int32_t LD_FTP_CliPathNameGet(ILD * pILd, byte_t * pBuffer, int32_t nBufLen);
static int32_t LD_FTP_CliFileNameGet(ILD * pILd, byte_t * pBuffer, int32_t nBufLen);
static bool_t  LD_FTP_CliPathNameSet(ILD * pILd, byte_t * pName, int32_t nNameLen);
static bool_t  LD_FTP_CliFileNameSet(ILD * pILd, byte_t * pName, int32_t nNameLen);

static int32_t LD_FTP_FileReTransPointGet(ILD * pILd);
static bool_t  LD_FTP_FileReTransPointSet(ILD * pILd, int32_t reTransPoint);

static int32_t LD_FTP_UploadFileSelect(ILD * pILd);
static bool_t  LD_FTP_UploadFileGet(ILD * pILd, 
								byte_t * pPathName, int32_t nPathNameLen, 
								byte_t * pFileName, int32_t nFileNameLen, int32_t reTransferPoint);
static bool_t  LD_FTP_UploadFileDataGet(ILD * pILd, 
										byte_t * pOutBuffer, int32_t nBufferLen, int32_t * pDataLen);

static int32_t LD_FTP_DownloadFileDataSet(ILD * pILd, 
								byte_t * pInData, int32_t nDataLen, byte_t bEndFlag);
static int32_t LD_FTP_DownloadFileGet(ILD * pILd, 
			byte_t * pPathName, int32_t nPathNameLen, byte_t * pFileName, int32_t nFileNameLen);
#endif//SUPPORT_FTP

#ifndef FILESYSTEM_ON_DEV
void CSD_WEPDirCreat(void);
#endif


#ifdef SUPPORT_BLOCK_SP_SMS
static bool_t  LD_CHPDataSet(ILD * pILd, int32_t index, const byte_t *pData, int32_t nLen);
static int32_t LD_CHPDataGet(ILD * pILd, int32_t index, byte_t *pData,int32_t nLen);
#endif

#ifdef SUPPORT_FTP
//extern fmgr_context_struct			g_fmgr_context;
extern fmgr_context_struct			*fmgr_p;
extern DeviceData					*s_DeviceData;
extern void mmi_fmgr_highlight_matrix_node(S32 index);
extern void mmi_vdoply_exit_from_fmgr_emptry_folder(void);
extern void RngcExecAdd(void);
extern bool_t UI_MsgBoxShow(IUI * pIUI);



bool_t CSD_fmgr_launch (void);
void CSD_entry_fmgr_main(void);
void CSD_highlight_fmgr_drive_list(S32 hiliteID);
void CSD_entry_fmgr_explorer(void);
void CSD_highlight_fmgr_node(S32 index);
void CSD_fmgr_set_curr_path_and_enter (void);
void CSD_fmgr_back_to_parent (void);
void CSD_entry_fmgr_detail (void);
void ReturnTransInformationToWep(void);
void CSD_fmgr_view_image_hdlr (void);
pBOOL CSD_mmi_fmgr_get_drv_item_callback(S32 item_idx, UI_string_type str_buff, UI_image_type *img_buff_p, U8 str_img_mask);
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool_t CSD_fmgr_launch (void)
{
	DeviceData * pDd = s_DeviceData;
	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
	LD * pLD = (LD*)pWep->pILd;
	if(!pDd->b_ImageInformationHaveGot)
	{
		pWep->FTP_SelectingFile = TRUE;
		pDd->b_EnterFileManagerScreen = TRUE;
		pLD->fileLen = 0;
		DEV_MemSet(pLD->clientPath,0,MAX_FILENAME_LEN);
		DEV_MemSet(pLD->clientFile,0,MAX_FILENAME_LEN);

		fmgr_p->src_application = APP_FILEMANAGER;
		FMGR_FILTER_SET_ALL(&fmgr_p->filter_type);
		fmgr_p->tree_action = FMGR_ACTION_NONE;
		CSD_entry_fmgr_main();
		return FALSE;
	}else
	{
		return TRUE;
	}
}

void CSD_entry_fmgr_main(void)
{
	S32		i, drv_idx;
    U8* 		guiBuffer;
	U16		drive_count, lsk_str=0, lsk_img=0, imageId=0, titleId=STR_FMGR_TITLE;
	pBOOL 	thick_style = FALSE;

    guiBuffer = GetCurrGuiBuffer(SCR_FMGR_MAIN);

	/* init active explorer as default explorer */
	fmgr_p->active_explorer_screen = SCR_FMGR_EXPLORER;

   fmgr_p->cache_end_idx = 0;
   fmgr_p->cache_start_idx = 0;
   memset ((PU8)fmgr_p->file_cache, 0, sizeof (FMGR_FILE_INFO_STRUCT) * FMGR_CACHE_ENTRY_COUNT);

   if (fmgr_is_msdc_present() == FALSE)
   {
      PRINT_INFORMATION(("No Card\n"));
   }

   if (fmgr_p->drv_count == 0)
   {
      PRINT_INFORMATION(("No Drive\n"));
      DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY,
                     0, 1000, EMPTY_LIST_TONE);
      if (fmgr_p->src_application != APP_FILEMANAGER)
         fmgr_abort_app_select();
      return;
   }

   for (i=0; i<FMGR_TOTAL_STORAGE && fmgr_p->drv_list[i][0] != 0; i++)
   {
	if (FS_GetDevStatus(fmgr_p->drv_list[i][0], FS_FEATURE_STATE_ENUM) == FS_DEVICE_BUSY)
	{
		DisplayPopup((PU8)GetString(FMGR_FS_DEVICE_BUSY_TEXT), IMG_GLOBAL_ERROR, 0,
      				1000, ERROR_TONE);
		if (fmgr_p->src_application != APP_FILEMANAGER)
         	   fmgr_abort_app_select();
		return;
	}
   }


    EntryNewScreen(SCR_FMGR_MAIN, NULL, CSD_entry_fmgr_main, NULL);

	RegisterHighlightHandler(CSD_highlight_fmgr_drive_list);
    DeleteScreenIfPresent(SCR_FMGR_TREE_EXPLORER);
    DeleteScreenIfPresent(SCR_FMGR_FILE_OPTION);
 	if (fmgr_p->tree_action == FMGR_ACTION_NONE)
 		DeleteScreenIfPresent(SCR_FMGR_EXPLORER);

	/* get actual drive count */
	drive_count = 0;

	for( i = 0 ; i < fmgr_p->drv_count; i++)
	{
		drv_idx = fmgr_get_drive_index(i);

		if (drv_idx == FMGR_REMOVABLE_STORAGE)
		{
			/* test if card is actually present */
			if(fmgr_is_msdc_present() == TRUE)
				drive_count++;
		}
		else
		{
			if(drv_idx < 0 || drv_idx>= FMGR_TOTAL_STORAGE)
			{
				continue;
			}
			drive_count++;
		}
	}

	if (drive_count > 0)
	{
		lsk_str = STR_GLOBAL_OK;
		lsk_img = IMG_GLOBAL_OK;
	}

	//bruce0912010
#ifdef __KK_UI_V2__
	imageId = 0;
#else
	imageId = IMG_VIEWCHAT_SLOGO;
#endif
    ShowCategory212Screen( 	titleId, imageId, 						/* title */
	                           lsk_str, lsk_img,  						/* lsk */
	                           STR_GLOBAL_BACK, IMG_GLOBAL_BACK,	/* rsk */
	                           drive_count,
	                           CSD_mmi_fmgr_get_drv_item_callback,
	                           mmi_fmgr_get_drv_hint_callback,
	                           mmi_fmgr_draw_drv_info_callback,
	                           mmi_fmgr_hide_drv_info_callback,
	                           2,	/* info box row count */
 	                           0, /* info bg img id */		
	                           0, 
	                           (U8*)guiBuffer);

	if (drive_count == 0)
   	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	return;
}
pBOOL CSD_mmi_fmgr_get_drv_item_callback(S32 item_idx, UI_string_type str_buff, UI_image_type *img_buff_p, U8 str_img_mask)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
   S16 	i;
	U16	drv_str_id = 0; 

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/ 
	*img_buff_p = NULL;

	/* TODO: FS_GetDrive */
	if (item_idx > fmgr_p->drv_count)
	   return FALSE;
#ifdef WEP_DEBUG
	WM_DebugPrint("######### CSD_mmi_fmgr_get_drv_item_callback## item_idx = %d",item_idx);
#endif
   i = fmgr_get_drive_index(item_idx);
#ifdef WEP_DEBUG
	WM_DebugPrint("######### CSD_mmi_fmgr_get_drv_item_callback## i = %d",i);
#endif
	if (i < 0 || i >= FMGR_TOTAL_STORAGE)
	{/* device not exist */
	   return FALSE;
	}

	switch(i)
   {
      case FMGR_NAND_STORAGE:
      	drv_str_id = STR_FMGR_NOR_FLASH;
      	break;
      case FMGR_NOR_STORAGE:
         drv_str_id = STR_FMGR_NAND_FLASH;
         break;
#ifdef __DRM_V01__
      case FMGR_DRM_STORAGE:
         drv_str_id = STR_FMGR_DRM_DATA;
         break;
#endif
#ifdef __DRM_SUPPORT__         
      case FMGR_DRM_RO_STORAGE:
         drv_str_id = STR_FMGR_DRM_RIGHTS;
         break;
#endif         
      case FMGR_REMOVABLE_STORAGE:
         drv_str_id = STR_FMGR_REM_DEVICE;
         break;
      default:
         MMI_ASSERT(0);
         break;
   }

	pfnKKUnicodeStrcpy((PS8)str_buff, (PS8)(GetString(drv_str_id)));

	if(i == FMGR_NAND_STORAGE || 
		i == FMGR_NOR_STORAGE 
		#ifdef __DRM_SUPPORT__
			|| i == FMGR_DRM_RO_STORAGE
		#endif
		)
	{
		*img_buff_p = (UI_image_type)GetImage(IMG_ID_FMGR_DRIVE_PHONE);
	}
	else if (i == FMGR_REMOVABLE_STORAGE)
	{
		/* test if card is actually present */
		if(fmgr_is_msdc_present() == TRUE)
			*img_buff_p = (UI_image_type)GetImage(IMG_ID_FMGR_DRIVE_MEM_CARD);
		else
			return FALSE;
	}

	return TRUE;
}
void CSD_highlight_fmgr_drive_list(S32 hiliteID)
{
   S16 index;
   fmgr_p->cur_index = hiliteID;
   index = fmgr_get_drive_index (hiliteID);
   if (index >= 0)
      pfnKKAnsiiToUnicodeString((PS8)fmgr_p->file_path, (PS8)fmgr_p->drv_list[index]);
   if (fmgr_p->src_application == APP_FILEMANAGER)
   {
        ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
      	SetKeyHandler(CSD_entry_fmgr_explorer, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
      	SetLeftSoftkeyFunction(CSD_entry_fmgr_explorer, KEY_EVENT_UP);
   		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
          
         #ifdef __MMI_TOUCH_SCREEN__
         wgui_register_list_item_selected_callback(CSD_entry_fmgr_explorer);
         #endif
          
	  	ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
		return;
   }
}


void CSD_entry_fmgr_explorer(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/	
   U8* guiBuffer;
   S32 highlightId=0;
   U16 count, iconId;
	U8	arrow_display;

	#ifdef __MMI_IMAGE_VIEWER__
		U16	imgview_style;
		S32	box_width;
		S32	box_height;	
	#endif /* __MMI_IMAGE_VIEWER__ */

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	if (FS_GetDevStatus(fmgr_p->file_path[0], FS_FEATURE_STATE_ENUM) == FS_DEVICE_BUSY)
	{
		DisplayPopup((PU8)GetString(FMGR_FS_DEVICE_BUSY_TEXT), IMG_GLOBAL_ERROR, 0,
      				1000, ERROR_TONE);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_UP);
         #ifdef __MMI_TOUCH_SCREEN__
         	wgui_reset_list_item_selected_callback();
         #endif
		return;
	}

	mmi_fmgr_kernel_init ((PS8)fmgr_p->file_path, fmgr_p->filter_type, &count);


	/* browse type do not pop up empty */
	if (count == 0)
	{
		if( fmgr_p->src_application == APP_FILEMANAGER || 
#if defined(__MMI_IRDA_SUPPORT__) || defined(__MMI_OPP_SUPPORT__)
		   fmgr_p->src_application == APP_EXTDEVICE ||
#endif
#ifdef __MMI_FTS_SUPPORT__
			fmgr_p->src_application == APP_FTP ||
#endif
#ifdef __MMI_FM_RADIO_RECORD__
		   fmgr_p->src_application == APP_FMRDO ||
#endif
			(fmgr_p->src_application != APP_FILEMANAGER && 
			fmgr_p->sel_type != FMGR_SELECT_BROWSE))
		{
			//UnicodeToAnsii((PS8)nPrintableStr, (PS8)fmgr_p->file_path);
   			//PRINT_INFORMATION(("entry explorer fail: %s", nPrintableStr));
			mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);

     		DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 0,
      					1000, EMPTY_LIST_TONE);
   		DeleteScreenIfPresent(SCR_FMGR_FOLDER_OPTION);
   		DeleteScreenIfPresent(SCR_FMGR_DRIVE_OPTION);
			ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_UP);
   		fmgr_p->enter_folder = FALSE;
         #ifdef __MMI_TOUCH_SCREEN__
         	wgui_reset_list_item_selected_callback();
         #endif
			return;
		}
	}

   EntryNewScreen(SCR_FMGR_EXPLORER, exit_fmgr_explorer,
                   CSD_entry_fmgr_explorer, NULL);

   guiBuffer = GetCurrGuiBuffer(SCR_FMGR_EXPLORER);

   if (fmgr_p->enter_folder == TRUE)
   {
      /* enter explorer screen by user selection, not go back history after interrupt */
   	fmgr_p->enter_folder = FALSE;
      DeleteUptoScrID(SCR_FMGR_EXPLORER);

      /* enter explorer screen by user selection shall always highlight first item */
   	guiBuffer = NULL;
   }
   if (GetCurrScrnId() == SCR_FMGR_DRIVE_OPTION)
    	DeleteScreenIfPresent(SCR_FMGR_DRIVE_OPTION);

   if (fmgr_p->parent_name[0] != 0 || fmgr_p->parent_name[1] != 0)
   {
		highlightId = fmgr_get_parent_highlight_id();
		memset (fmgr_p->parent_name, 0, sizeof(fmgr_p->parent_name));
		guiBuffer = NULL;
   }
   RegisterHighlightHandler(CSD_highlight_fmgr_node);

	/****************************************************************************
	* Display Menu                                                              
	*
	* 1. File Manager		: Use standard list menu with info box. 
	* 2. Image Viewer		: Use thick list menu.
	* 3. Video Player		: Use thick list menu.
	*****************************************************************************/
	/* detemine display arrow type */
	/* if is browse mode, only display up and down arrow */

	if((fmgr_p->sel_type & 0x0f) == FMGR_SELECT_BROWSE)
		arrow_display = CAT213_ARROW_UP | CAT213_ARROW_DOWN;
	else
		arrow_display = CAT213_ARROW_ALL;

   iconId = 0 /*bruce091207 IMG_VIEWCHAT_SLOGO*/;

	if(fmgr_p->cur_index > fmgr_p->file_count)
		fmgr_p->cur_index = 0;

		{

			/* create image cache */
			mmi_fmgr_create_thumb_image_cache(MMI_MENUITEM_HEIGHT*2);

 			ShowCategory212Screen(	STR_FMGR_TITLE, iconId, 						/* title */
											STR_GLOBAL_OK, IMG_GLOBAL_OK,	/* lsk */
											STR_GLOBAL_BACK, IMG_GLOBAL_BACK,			/* rsk */
											count,
											mmi_fmgr_get_file_item_callback,
											mmi_fmgr_get_file_hint_callback,
											mmi_fmgr_draw_file_info_callback,
											mmi_fmgr_hide_file_info_callback,
											2,					/* info box row count */
											0, 				/* info bg img id */		
											highlightId,	/* hiligth item idx */
											(U8*)guiBuffer);
		}
}

void CSD_highlight_fmgr_node(S32 index)
{
   FMGR_FILE_INFO_STRUCT file_info;
   U8 sel_type;

   mmi_fmgr_kernel_get_file_info(index, &file_info);
   fmgr_p->cur_index = index;

   if (fmgr_p->src_application == APP_FILEMANAGER)
   {
#ifdef __DRM_SUPPORT__
	if (pfnKKUnicodeStrncmp ((PS8)fmgr_p->file_path, (PS8)DRM_RO_PATH, 
						pfnKKUnicodeStrlen((PS8)DRM_RO_PATH)) == 0)
	{
		/* DRM rights, back and left arrow only go back to drive list */
		ChangeLeftSoftkey(STR_GLOBAL_DELETE, 0);
         	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
      	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
         	SetLeftSoftkeyFunction(entry_fmgr_delete_confirm, KEY_EVENT_UP);
   		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
          
         #ifdef __MMI_TOUCH_SCREEN__
         wgui_reset_list_item_selected_callback();
         #endif
          
   		return;
	}
	else
#endif	
#ifdef __DRM_V01__
	if (pfnKKUnicodeStrncmp ((PS8)fmgr_p->file_path, (PS8)DRM_NOR_PATH, 
						pfnKKUnicodeStrlen((PS8)DRM_NOR_PATH)) == 0)
	{
		/* DRM folder, back and left arrow only go back to drive list */
         	ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
      		SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
         	SetLeftSoftkeyFunction(CSD_fmgr_set_curr_path_and_enter, KEY_EVENT_UP);
   		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
          
         #ifdef __MMI_TOUCH_SCREEN__
         wgui_reset_list_item_selected_callback();
         #endif
          
   		return;
	}
	else
#endif
	{
      if (file_info.attribute & FS_ATTR_DIR)
      {
         /* folder node */
         SetKeyHandler(CSD_fmgr_set_curr_path_and_enter, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
         SetLeftSoftkeyFunction(CSD_fmgr_set_curr_path_and_enter, KEY_EVENT_UP);
          
         #ifdef __MMI_TOUCH_SCREEN__
         wgui_register_list_item_selected_callback(CSD_fmgr_set_curr_path_and_enter);
         #endif
          
      }
      else
      {
         /* file node */
         ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
		 if(mmi_fmgr_check_image_filter(&file_info.file_type))
		 {
			SetLeftSoftkeyFunction(CSD_fmgr_view_image_hdlr, KEY_EVENT_UP);
		 }else
		 {
			SetLeftSoftkeyFunction(CSD_entry_fmgr_detail, KEY_EVENT_UP);
		 }
         #ifdef __MMI_TOUCH_SCREEN__
        wgui_reset_list_item_selected_callback();
         #endif
      }
      SetKeyHandler(CSD_fmgr_back_to_parent, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
   	SetRightSoftkeyFunction(CSD_fmgr_back_to_parent, KEY_EVENT_UP);
	}
      return;
   }

}

void CSD_fmgr_set_curr_path_and_enter (void)
{
	FMGR_FILE_INFO_STRUCT file_info;

   mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

	if (pfnKKUnicodeStrcmp((PS8)file_info.file_name, (PS8)L".") == 0)
	{
		return;
	}

	if (pfnKKUnicodeStrcmp((PS8)file_info.file_name, (PS8)L"..") == 0)
	{
		CSD_fmgr_back_to_parent();
		return;
	}

   if (mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == FALSE ||
      mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)L"\\") == FALSE)
      return;

	if ((fmgr_p->src_application != APP_FILEMANAGER) &&
		(fmgr_p->sel_type == FMGR_SELECT_FOLDER) &&
		(FS_Count((PU16)fmgr_p->file_path, FS_DIR_TYPE | FS_FILTER_SYSTEM_ATTR,NULL, 0)) == 0)
	{
		mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);

		DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_EMPTY, 0,
							1000, EMPTY_LIST_TONE);
		DeleteUptoScrID(SCR_FMGR_EXPLORER);
		return;
	}

	if (pfnKKUnicodeStrlen((PS8)fmgr_p->file_path) + 3 >= FMGR_MAX_PATH_LEN)
	{
		mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
		DisplayPopup((PU8)GetString(FMGR_FS_PATH_OVER_LEN_ERROR_TEXT), IMG_GLOBAL_ERROR, 0,
							1000, ERROR_TONE);
		DeleteUptoScrID(SCR_FMGR_EXPLORER);
		return;
	}
	fmgr_p->enter_folder = TRUE;
	CSD_entry_fmgr_explorer();
}


void CSD_fmgr_back_to_parent (void)
{
	PS8 ptr;
	
   if (pfnKKUnicodeStrlen((PS8)fmgr_p->file_path) <= 3)
   {
      if (fmgr_p->src_application == APP_FILEMANAGER &&
      	IsScreenPresent(SCR_FMGR_MAIN))
      {
         GoBackToHistory(SCR_FMGR_MAIN);
      }
      else
      {
         CSD_entry_fmgr_main();
      }
      return;
   }

   /* remove last level of folder, find last back slash character in the path string */
   fmgr_p->enter_folder = TRUE;
   ptr = mmi_fmgr_extract_file_name((PS8)fmgr_p->file_path);
   if (ptr != NULL)
   {
   	S32 end;
   	
   	end = pfnKKUnicodeStrlen(ptr)-1;
   	if (ptr[end<<1] == '\\' && ptr[(end<<1)+1] == 0)
   		ptr[end<<1] = 0;
   	pfnKKUnicodeStrcpy((PS8)fmgr_p->parent_name, ptr);
		ptr[0] = 0;
		ptr[1] = 0;
   }
   else
   mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
   if (IsScreenPresent(SCR_FMGR_EXPLORER))
	   GoBackToHistory(SCR_FMGR_EXPLORER);
	else
   	CSD_entry_fmgr_explorer();
}



void CSD_entry_fmgr_detail (void)
{
#ifdef __DRM_SUPPORT__	
	FS_HANDLE handle;
	BOOL need_activate=FALSE;
#endif
	FMGR_FILE_INFO_STRUCT file_info;
	S8		buffer[32];
	U32	file_size;
	//U8*	description[10];
	U8	*guiBuffer, *info;
	
	EntryNewScreen(SCR_FMGR_DETAIL, NULL,
	               CSD_entry_fmgr_detail, NULL);

   mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

	guiBuffer = GetCurrGuiBuffer( SCR_FMGR_DETAIL );

   info = (PU8)subMenuData;
   
   pfnKKUnicodeStrcpy((PS8)info, (PS8)GetString(STR_ID_FMGR_FILE_DATE));
   pfnKKUnicodeStrcat ((PS8)info, (PS8)L"\n");	   
   date_string(&file_info.time, (PU16)&info[pfnKKUnicodeStrlen((PS8)info)<<1], DT_IDLE_SCREEN);
   pfnKKUnicodeStrcat ((PS8)info, (PS8)L"\n");
	
   pfnKKUnicodeStrcat((PS8)info, (PS8)GetString(STR_ID_FMGR_FILE_SIZE));
   pfnKKUnicodeStrcat ((PS8)info, (PS8)L"\n");	   
	if(file_info.file_size > 1024*1024)
	{
		file_size = file_info.file_size/1024;
		sprintf(buffer, "%d.%dM", (U16)(file_size/1024), (U16)((file_size%1024)/103));
	}
	else if(file_info.file_size > 1024)
	{
		sprintf(buffer, "%d.%dK", (U16)(file_info.file_size/1024), (U16)((file_info.file_size%1024)/103));
	}
	else	/* less than 1024 */
	{
		sprintf(buffer, "%dB", (U16)(file_info.file_size));
	}
   pfnKKAnsiiToUnicodeString((PS8)&info[pfnKKUnicodeStrlen((PS8)info)<<1], (PS8)buffer);
   pfnKKUnicodeStrcat ((PS8)info, (PS8)L"\n");	

#ifdef __DRM_SUPPORT__	
	if (mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == TRUE
#ifndef __MMI_SHOW_FILE_EXT__
      && mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext) == TRUE
#endif
      )
	{
#if 0	
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
		{
			U8 perm=0;
			
			if (FMGR_FILTER_IS_SET_AUDIO(&file_info.file_type))
				perm = DRM_PERMISSION_PLAY;
			else if (FMGR_FILTER_IS_SET_IMAGE(&file_info.file_type))
				perm = DRM_PERMISSION_DISPLAY;
#ifdef __J2ME__			
			else if (FMGR_FILTER_IS_SET (&file_info.file_type, FMGR_TYPE_JAR) ||
			            FMGR_FILTER_IS_SET (&file_info.file_type, FMGR_TYPE_JAD))
			   perm = DRM_PERMISSION_EXECUTE;
#endif			
			handle = DRM_open_file ((PU16)fmgr_p->file_path, FS_READ_ONLY, perm);
			if (handle > FS_NO_ERROR)
			{
				need_activate = fmgr_populate_drm_info (handle, perm, info);
				DRM_close_file (handle);
			}
		}
		mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
	}
#endif /*__DRM_SUPPORT__*/

	
#ifdef __DRM_SUPPORT__
   if (need_activate == TRUE)
   {
      ShowCategory74Screen(STR_FMGR_DETAIL, 0 /*bruce091207 IMG_VIEWCHAT_SLOGO*/, 
         STR_FMGR_LSK_ACTIVATE, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
         (PU8)subMenuData, MAX_SUB_MENUS*MAX_SUB_MENU_SIZE, guiBuffer);	   
	if (FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_UNKNOW))
	   SetLeftSoftkeyFunction(entry_fmgr_request_unknown_rights_confirm, KEY_EVENT_UP);
   else
	   SetLeftSoftkeyFunction(entry_fmgr_request_rights_confirm, KEY_EVENT_UP);
   }
   else
#endif      
   ShowCategory74Screen(STR_FMGR_DETAIL, 0 /*bruce091207 IMG_VIEWCHAT_SLOGO*/, 
         STR_GLOBAL_SEND, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
         (PU8)subMenuData, MAX_SUB_MENUS*MAX_SUB_MENU_SIZE, guiBuffer);

	SetKeyHandler( GoBackHistory,KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction( ReturnTransInformationToWep, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}



void CSD_fmgr_view_image_hdlr (void)
{
	FMGR_FILE_INFO_STRUCT file_info;

   if (fmgr_check_file_present() < FMGR_NO_ERROR)
      return;

	mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

   if (mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_name) == FALSE
#ifndef __MMI_SHOW_FILE_EXT__
      || mmi_fmgr_append_file_name((PS8)fmgr_p->file_path, (PS8)file_info.file_ext) == FALSE
#endif
      )
      return;

#ifdef __DRM_SUPPORT__
   {
      U8 result;
      
      result = fmgr_check_drm_rights(NULL, DRM_PERMISSION_DISPLAY);
      if (result == FMGR_DRM_NO_PERMISSION)
      {
         DisplayPopup((PU8)GetString(STR_GLOBAL_DRM_PROHIBITED), IMG_GLOBAL_ERROR,
                     0, 1000, ERROR_TONE);
         mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);         
         return;
      }
      else if (result == FMGR_DRM_REQUEST_RIGHTS)
      {
         entry_fmgr_request_rights_confirm();
         mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
         return;
      }
   }
#endif /*__DRM_SUPPORT__*/

	EntryNewScreen(SCR_ID_FMGR_VIEW_IMAGE, NULL,
                   CSD_fmgr_view_image_hdlr, NULL);

#ifdef __MMI_MESSAGES_EMS__
	if (FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_EMS))
		ShowCategory144Screen (0, 0 /*bruce091207 IMG_VIEWCHAT_SLOGO*/,
								  	STR_GLOBAL_SEND, IMG_GLOBAL_OK, 
								  	STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								  	EMS_OBJECT_MY_PICTURE_STATE|CATEGORY144_FMGR_PREVIEW, 1,
								  	NULL, (U16*)&fmgr_p->file_path, file_info.is_short, NULL,
								  	0, NULL); 
	else if (FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_ANM))
		ShowCategory144Screen (0, 0 /*bruce091207 IMG_VIEWCHAT_SLOGO*/,
								  	STR_GLOBAL_SEND, IMG_GLOBAL_OK, 
								  	STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
								  	EMS_OBJECT_MY_ANIMATION_STATE|CATEGORY144_FMGR_PREVIEW, 1,
								  	NULL, (U16*)&fmgr_p->file_path, file_info.is_short, NULL,
								  	0, NULL); 
	else
#endif	   
	/* entry cat222 to decode and display a image from file */
	ShowCategory222Screen(	0,												/* title */
									0 /*bruce091207 IMG_VIEWCHAT_SLOGO*/,		/* title icon */
									STR_GLOBAL_SEND, IMG_GLOBAL_OK,											/* left soft key */
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK,	/* right soft key */
									GDI_COLOR_WHITE,							/* content BG color */
									(PS8)NULL,									/* button bar string */
									(PS8)fmgr_p->file_path,					/* image file name */
									file_info.is_short,						/* is_short */
									NULL);										/* result callback */


   ClearAllKeyHandler();
   SetLeftSoftkeyFunction( ReturnTransInformationToWep, KEY_EVENT_UP );
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	
	mmi_fmgr_remove_last_dir((PS8)fmgr_p->file_path);
   /* pass file name to image viewer
      hook image view func */
}



void ReturnTransInformationToWep(void)
{
	FMGR_FILE_INFO_STRUCT file_info;
	S16 index,i=0;
	DeviceData * pDd = s_DeviceData;
	WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
	LD * pLD = (LD*)pWep->pILd;

	if (fmgr_check_file_present() < FMGR_NO_ERROR)
      return;

	mmi_fmgr_kernel_get_file_info(fmgr_p->cur_index, &file_info);

   	index = (S16)pfnKKUnicodeStrlen(fmgr_p->file_path);
	//DEV_NetUniByte2LocStr(pLD->clientPath, fmgr_p->file_path, index*2);
	while(i<index)
	{
		//pLD->clientPath[i] = fmgr_p->file_path[i*2+1] | fmgr_p->file_path[i*2];  //zshangzhong modify code 20070328 for 5002053
                pLD->clientPath[i] = (fmgr_p->file_path[i*2+1] << 8) | fmgr_p->file_path[i*2];
		i++;
	}

	index = (S16)pfnKKUnicodeStrlen(file_info.file_name);
	//DEV_NetUniByte2LocStr(pLD->clientFile, file_info.file_name, index*2);
	i=0;
	while(i<index)
	{
		//pLD->clientFile[i] = file_info.file_name[i*2+1] | file_info.file_name[i*2];
		pLD->clientFile[i] = (file_info.file_name[i*2+1]<<8) | file_info.file_name[i*2];  //zshangzhong modify code 20070328 for 5002053
		i++;
	}
	pLD->selectedType = 1;
	pLD->fileLen = file_info.file_size;
	pDd->b_ImageInformationHaveGot = TRUE;
	GoBackToHistory(pDd->screenId);
}



#endif



////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
ILD * LD_Create(WEPAppData *pWep)
{
	LD * pLD = NULL;
// 	DeviceData * pDd = (DeviceData *)(pWep->pDeviceData);

	pLD = (LD *)DEV_Malloc(MALLOC_MODULE_LOCAL_DATA, sizeof(LD));
	if (pLD != NULL)
	{
		DEV_MemSet(pLD, 0x00, sizeof(LD));

		pLD->pWep = pWep;

		pLD->Vtbl.DataLoad    = LD_DataLoad;
		pLD->Vtbl.DataSave    = LD_DataSave;

		pLD->Vtbl.IntDataSet	= LD_IntDataSet;
		pLD->Vtbl.IntDataGet	= LD_IntDataGet;

		pLD->Vtbl.StrDataSet	= LD_StrDataSet;
		pLD->Vtbl.StrDataGet	= LD_StrDataGet;

		pLD->Vtbl.BinDataSet	= LD_BinDataSet;
		pLD->Vtbl.BinDataGet	= LD_BinDataGet;

// 		pLD->Vtbl.GPDataSet		= LD_GPDataSet;
// 		pLD->Vtbl.GPDataGet		= LD_GPDataGet;
#ifdef SUPPORT_BLOCK_SP_SMS
 		pLD->Vtbl.CHPDataSet	= LD_CHPDataSet;
 		pLD->Vtbl.CHPDataGet	= LD_CHPDataGet;
#endif		
		pLD->Vtbl.DownloadSoundSave = LD_DownloadSoundSave;

		pLD->Vtbl.NotifySoundPlay   = LD_NotifySoundPlay;

		pLD->Vtbl.SoundDataProcess = LD_SoundDataProcess;
		pLD->Vtbl.SoundDestroy = LD_SoundDestroy;
		pLD->Vtbl.SoundDataPlay			= LD_SoundDataPlay;

#ifdef SUPPORT_FTP
		pLD->Vtbl.FTP_ClientPathNameGet	= LD_FTP_CliPathNameGet;
		pLD->Vtbl.FTP_ClientFileNameGet	= LD_FTP_CliFileNameGet;
		pLD->Vtbl.FTP_ClientPathNameSet = LD_FTP_CliPathNameSet;
		pLD->Vtbl.FTP_ClientFileNameSet = LD_FTP_CliFileNameSet;

		pLD->Vtbl.FTP_FileReTransPointGet = LD_FTP_FileReTransPointGet;
		pLD->Vtbl.FTP_FileReTransPointSet = LD_FTP_FileReTransPointSet;

		pLD->Vtbl.FTP_UploadFileSelect	= LD_FTP_UploadFileSelect;
		pLD->Vtbl.FTP_UploadFileGet			= LD_FTP_UploadFileGet;
		pLD->Vtbl.FTP_UploadFileDataGet	= LD_FTP_UploadFileDataGet;

		pLD->Vtbl.FTP_DownloadFileDataSet	= LD_FTP_DownloadFileDataSet;
		pLD->Vtbl.FTP_DownloadFileGet			= LD_FTP_DownloadFileGet;
	
#endif//SUPPORT_FTP

		pLD->Vtbl.CameraPicGet = CameraPicGet;
		pLD->Vtbl.cameraIntervalGet = cameraIntervalGet;

// 		pLD->soundDelay = 0;
#ifndef FILESYSTEM_ON_DEV
	  CSD_WEPDirCreat();
#endif
	pfnKKAnsiiToUnicodeString((char *)pWep->recordfilefullName, (char *)"e:/WEP/ldf/123.amr");
	}

	return (ILD *)pLD;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void LD_Destroy(ILD * pILd)
{
	LD * pLD = (LD*)pILd;

	LD_SoundDataRelease(pILd);

	if(pLD != NULL) {
		DEV_Free(pLD);
	}
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static bool_t LD_DataLoad(ILD * pILd)
{
	LD * pLD = (LD*)pILd;
	bool_t result = FALSE;

#ifndef FILESYSTEM_ON_DEV
	{
		FS_HANDLE fd;
		/*S32*/INT32 size = 0;
		UI_character_type fileName[MAX_FILENAME_LEN];

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_DataLoad");
#endif
		pfnKKAnsiiToUnicodeString((S8 *)fileName,WEP_LD_PATH);
		fd = MMI_FS_Open((PU8)fileName, FS_OPEN_DIR| FS_READ_ONLY );
		if( fd >= 0 )
		{
			MMI_FS_Close( fd );

			pfnKKAnsiiToUnicodeString((S8 *)fileName,WEP_LD_PATH);
			size = pfnKKUnicodeStrlen((S8 *)fileName);
			pfnKKAnsiiToUnicodeString((S8 *)&fileName[size],WEP_LD_FILENAME);

			fd = MMI_FS_Open((PU8)fileName, FS_READ_ONLY);
			if( fd >= 0 )
			{
				if( MMI_FS_Seek( fd, 0, FS_FILE_BEGIN ) >= 0 )
				{
					if(FS_NO_ERROR == MMI_FS_Read( fd, (PU8)&pLD->ldGP, sizeof(LDGP), /*(UINT*)*/(INT32*)&size)) 
					{
						result = TRUE;
					}
				}
				MMI_FS_Close( fd );
			}
			else
			{
//init here//james 070523 start
				LD_IntDataSet(pILd, LOCAL_DATA_RECORD_SETTING_SOUND,
						WEP_ON);
#ifdef SUPPORT_PROXY
				LD_IntDataSet(pILd, LOCAL_DATA_RECORD_SETTING_USEPROXY,
						WEP_OFF);
#endif
				LD_IntDataSet(pILd, LOCAL_DATA_RECORD_SETTING_AUTOSAVE,
						WEP_ON);				
#ifdef SUPPORT_DUAL_SIM
				{	
					int whichSim = 0;
					E_MTPNP_AD_SIMCARD_USABLE_SIDE simStatus = MTPNP_AD_Get_UsableSide();
					if (MTPNP_AD_DUALSIM_USABLE == simStatus || MTPNP_AD_SIMCARD1_USABLE == simStatus)
					{
						whichSim = 0;
					}
					else
					{
						whichSim = 1;
					}
					LD_IntDataSet(pILd, LOCAL_DATA_RECORD_WHICH_SIM, whichSim); 	//wyl20100408
				}
#endif

#ifdef _KK_SELECT_ACCOUNT_
				//设置默认DataAccount
				LD_IntDataSet(pILd, LOCAL_DATA_RECORD_WHICH_ACCOUNT, -1);
#endif
//james 070523 end
			}
		}
	}
#endif

	return result;
}
#ifdef _KK_SELECT_ACCOUNT_
void LD_RestAccount(void)
{
	FS_HANDLE fd;
	S32 size = 0;
	UI_character_type fileName[MAX_FILENAME_LEN];
	LDGP *ldGP = 0;
	
#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_RestAccount");
#endif

	mmi_asc_to_ucs2((S8 *)fileName,WEP_LD_PATH);
	fd = MMI_FS_Open((PU8)fileName, FS_OPEN_DIR| FS_READ_ONLY );
	if( fd >= 0 )
	{
		MMI_FS_Close( fd );

		mmi_asc_to_ucs2((S8 *)fileName,WEP_LD_PATH);
		size = mmi_ucs2strlen((S8 *)fileName);
		mmi_asc_to_ucs2((S8 *)&fileName[size],WEP_LD_FILENAME);

		fd = MMI_FS_Open((PU8)fileName, FS_READ_ONLY );
		if( fd >= 0 )
		{
			ldGP = DEV_Malloc(0, sizeof(LDGP));
			memset(ldGP, 0, sizeof(LDGP));
	
			if (FS_NO_ERROR == MMI_FS_Read( fd, ldGP, sizeof(LDGP),/*(UINT*)*/(INT32*)&size)) 
			{
#ifdef SUPPORT_DUAL_SIM
 				ldGP->intData[LOCAL_DATA_RECORD_WHICH_SIM] = -1;
#endif

				//设置默认DataAccount
				ldGP->intData[LOCAL_DATA_RECORD_WHICH_ACCOUNT] = -1;
				MMI_FS_Write( fd, ldGP, sizeof(LDGP), /*(UINT*)*/(INT32*)&size) ;
			}
			
			DEV_Free(ldGP);
			MMI_FS_Close( fd );
		}		
	}
}
#endif

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static bool_t LD_DataSave(ILD * pILd)
{
	LD * pLD = (LD*)pILd;
	bool_t result = FALSE;
	S32 free_space = 0;

#ifndef FILESYSTEM_ON_DEV 
	{
		FS_HANDLE fd;
		/*S32*/INT32 size = 0;
		UI_character_type fileName[MAX_FILENAME_LEN];

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_DataSave");
#endif
		pfnKKAnsiiToUnicodeString((S8 *)fileName,WEP_LD_PATH);
		fd = MMI_FS_Open((PU8)fileName, FS_OPEN_DIR| FS_READ_ONLY);
#ifdef WEP_DEBUG			
	WM_DebugPrint("FS_Open1=%d",fd);
#endif
		if (fd >=0)
		{
			MMI_FS_Close(fd);
		}
		/*else if (fd == FS_PATH_NOT_FOUND)
		{
			if (FS_CreateDir((PU16)fileName) < 0)
				return result;
		}*/
		else
		{
			return result;
		}

		//pfnKKAnsiiToUnicodeString((S8 *)fileName,WEP_LD_PATH);
		size = pfnKKUnicodeStrlen((S8 *)fileName);
		pfnKKAnsiiToUnicodeString((S8 *)&fileName[size],WEP_LD_FILENAME);

		//Add by YHB, 2007-04-27. {{{
		free_space = DEV_GetDiskFreeBytes((const WCHAR *)&fileName);
		if (free_space <= sizeof(LDGP))
			return FALSE;
		//Add by YHB, 2007-04-27. }}}

		if( (fd = MMI_FS_Open((PU8)fileName, FS_READ_WRITE | FS_CREATE )) >= 0 )
		{
#ifdef WEP_DEBUG			
	WM_DebugPrint("FS_Open2=%d",fd);
#endif
			if( MMI_FS_Seek( fd, 0, FS_FILE_BEGIN ) >= 0 )
			{
				if(FS_NO_ERROR == MMI_FS_Write( fd, (PU8)&pLD->ldGP, sizeof(LDGP), /*(uint32_t*)*/(INT32 *)&size)) 
				{
					result = TRUE;
				}
			}
			MMI_FS_Close( fd );
		}
	}
#endif
	return result;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static bool_t LD_StrDataSet(ILD * pILd, int32_t nId, byte_t *pData, int32_t nLen)
{
 	LD * pLD = (LD*)pILd;
//	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);
	int32_t i;

	if((nId < 0) || (nId >= MAX_STRING_RECORD))
		return FALSE;
	if((nLen < 0) || (nLen > MAX_STRING_RECORD_LENGTH))
		return FALSE;

	pLD->ldGP.szDataLen[nId] = nLen;
	for(i=0;i<nLen;i++) {
		pLD->ldGP.szData[nId][i] = pData[i];
	}

	return TRUE;
} // LD_StrDataSet()

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static int32_t LD_StrDataGet(ILD * pILd, int32_t nId, byte_t *pData)
{
 	LD * pLD = (LD*)pILd;
//	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);

	int32_t i;

	if((nId < 0) || (nId >= MAX_STRING_RECORD)||pData == NULL)
		return -1;

	if(pData != NULL) {
		for (i=0; i < pLD->ldGP.szDataLen[nId]; i++)
			pData[i] = pLD->ldGP.szData[nId][i];
	}
	return pLD->ldGP.szDataLen[nId];
} // LD_StrDataGet()

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static int32_t LD_BinDataSet(ILD * pILd, int32_t nId, byte_t *pData, int32_t dataSize)
{
 	LD * pLD = (LD*)pILd;
//	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);
	S32 free_space = 0;

#ifdef FILESYSTEM_ON_DEV
	int32_t len =0;

	if(pLD->pWep->pIPdp == NULL)
		return -4;//not init

// 	len = (dataSize < MAX_GP_RECORD_LENGTH)?dataSize : MAX_GP_RECORD_LENGTH;
// 	pLD->ldGP.GPLen[nId] = len;
// 	for(i=0;i<len;i++) {
// 		pLD->ldGP.GPData[nId][i] = pData[i];
// 	}

	return len;
#else
	UI_character_type		filepath[MAX_FILENAME_LEN];
	UI_character_type		filenameptr[MAX_FILENAME_LEN];
	UI_character_type 		fileName[MAX_FILENAME_LEN];
	FS_HANDLE fd;
	/*UINT*/ INT32 dwBytesWritten = 0;
	int retval=1;

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_BinDataSet,%d",nId);
#endif

	if(pLD->pWep->pIPdp == NULL)
	{
#ifdef WEP_DEBUG			
	WM_DebugPrint("pLD->pWep->pIPdp == NULL");
#endif
		return -4;//not init
	}	
	pfnKKAnsiiToUnicodeString((S8 *)filepath,WEP_LDF_PATH);
	fd = MMI_FS_Open((PU8)filepath, FS_OPEN_DIR| FS_READ_ONLY);
#ifdef WEP_DEBUG			
	WM_DebugPrint("FS_Open =%d",fd);
#endif
	if( fd >= 0 )
	{
		MMI_FS_Close(fd);

		sprintf((PS8)fileName, "%sldf_%03d.dat", WEP_LDF_PATH, nId);
		pfnKKAnsiiToUnicodeString((S8 *)filenameptr,(S8 *)fileName);

		// if size is zero, delete saved data
		if(dataSize == 0) {
			retval = MMI_FS_Delete((PU8)filenameptr);
			return (retval ? dataSize : -1);
		}

		//Add by YHB, 2007-04-27. {{{
		free_space = DEV_GetDiskFreeBytes((const WCHAR *)&filenameptr);
		if (free_space <= dataSize)
			return -1;//Must ensure this return variable's value.
		//Add by YHB, 2007-04-27. }}}
	
		if((fd = MMI_FS_Open((PU8)filenameptr, FS_READ_WRITE|FS_CREATE)) >=0)
		{
			retval = MMI_FS_Write( fd, pData, dataSize,(INT32*)&dwBytesWritten);
			MMI_FS_Close(fd);

			return ((retval==FS_NO_ERROR) ? dwBytesWritten : -1);
		}
	}
	return -2;
#endif
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static int32_t LD_BinDataGet(ILD * pILd, int32_t nId, byte_t *pBuff, int32_t buffSize)
{
 	LD * pLD = (LD*)pILd;
//	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);

#ifdef FILESYSTEM_ON_DEV
	int32_t len=0;

	if(pLD->pWep->pIPdp == NULL)
		return -4;//not init

// 	len = (buffSize < pLD->ldGP.GPLen[nId])? buffSize:pLD->ldGP.GPLen[nId];
// 	if(pBuff != NULL) {
// 		for (i=0; i < len; i++)
// 			pBuff[i] = pLD->ldGP.GPData[nId][i];
// 	}

	return len;
#else
	UI_character_type		filepath[MAX_FILENAME_LEN];
	UI_character_type		filenameptr[MAX_FILENAME_LEN];
	UI_character_type 		fileName[MAX_FILENAME_LEN];
	FS_HANDLE fd;
	/*UINT*/INT32 dwBytesRead = 0;
	/*UINT*/INT32 filelen = 0;
	int retval;

#ifdef WEP_DEBUG			
//	WM_DebugPrint("LD_BinDataGet::nid=%d",nId);
#endif

	if(pLD->pWep->pIPdp == NULL)
	return -4;//not init

	pfnKKAnsiiToUnicodeString((S8 *)filepath,WEP_LDF_PATH);
	fd = MMI_FS_Open((PU8)filepath, FS_OPEN_DIR| FS_READ_ONLY);
#ifdef WEP_DEBUG			
	WM_DebugPrint("FS_Open:filepath:fd=%d",fd);
#endif
	if( fd >= 0 )
	{
		MMI_FS_Close(fd);

		sprintf((PS8)fileName, "%sldf_%03d.dat", WEP_LDF_PATH,nId);
		pfnKKAnsiiToUnicodeString((S8 *)filenameptr,(S8 *)fileName);

		if((fd = MMI_FS_Open((PU8)filenameptr, FS_READ_ONLY)) >=0)
		{
#ifdef WEP_DEBUG			
	WM_DebugPrint("FS_Open:filenameptr:fd=%d",fd);
#endif
			MMI_FS_GetFileSize(fd, (INT32*)&filelen);
			if(filelen > 0)
			{
				retval = MMI_FS_Read(fd, pBuff, buffSize, (INT32*)&dwBytesRead);
#ifdef WEP_DEBUG			
	WM_DebugPrint("FS_Read:retval:retval=%d",retval);
#endif
				if( retval != FS_NO_ERROR ) 
				{
					dwBytesRead = -1;
				}
			}
			else 
			{
				dwBytesRead = -2;
			}
			MMI_FS_Close(fd);
		}
		else
		{
			dwBytesRead = -3;
		}
	}
	return dwBytesRead;
#endif
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static bool_t LD_IntDataSet(ILD * pILd, int32_t nId, int32_t iData)
{
 	LD * pLD = (LD*)pILd;
//	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);

	if((nId < 0) || (nId >= MAX_INTEGER_RECORD))
		return FALSE;

	pLD->ldGP.intData[nId] = iData;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static bool_t LD_IntDataGet(ILD * pILd, int32_t nId, int32_t *pData)
{
	LD * pLD = (LD*)pILd;
//	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);

	if((nId < 0) || (nId >= MAX_INTEGER_RECORD) || pData == NULL)
		return FALSE;

	*pData = pLD->ldGP.intData[nId];

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/*static void LD_BeepCB(ILD * pILd)
{
// 	LD * pLD = (LD*)pILd;

}*/

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static void LD_SoundDataRelease(ILD * pILd)
{
// 	LD * pLD = (LD*)pILd;
// 	WEPAppData * pWep = pLD->pWep;
// 	DeviceData * pDd = (DeviceData *)(pWep->pDeviceData);
// 
// 	LDSoundData * pstSnd;
// 	int32_t i;
// 
// 	for(i = 0;i < LD_MAX_SOUND_NUM;i++) {
// 		pstSnd = &pDd->gameSound[i];
// 		if(pstSnd->pszSndData != NULL) {
// 			DEV_Free(pstSnd->pszSndData);
// 			pstSnd->pszSndData = NULL;
// 		}
// 		pstSnd = &pDd->UISound[i];
// 		if(pstSnd->pszSndData != NULL) {
// 			DEV_Free(pstSnd->pszSndData);
// 			pstSnd->pszSndData = NULL;
// 		}
// 	}
}//LD_SoundDataRelease()

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//stop sound
static void LD_SoundDestroy(ILD * pILd)
{
//	LD * pLD = (LD*)pILd;
//	WEPAppData * pWep = pLD->pWep;
//	DeviceData * pDd = (DeviceData *)(pWep->pDeviceData);

}//LD_SoundDestroy()


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static bool_t LD_SoundDataPlay(ILD * pILd, int32_t sndIdx, bool_t isCycle)
{
	LD * pLD = (LD*)pILd;
	WEPAppData * pWep = pLD->pWep;
	DeviceData * pDd = (DeviceData *)(pWep->pDeviceData);

#ifndef FILESYSTEM_ON_DEV
	S8	filepath[MAX_FILENAME_LEN];
	UI_character_type  fileName[MAX_FILENAME_LEN];
	FS_HANDLE fd;
	S32 size = 0;
//	int retval;
	int i;

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_SoundDataPlay");
#endif

	pWep->pILd->SoundDestroy(pWep->pILd);

	for(i=0;i<TOTAL_ELEMENTS(pDd->SoundData);i++) {
		if(pDd->SoundData[i].sndIdx == sndIdx) {
			break;
		}
	}
	if(i < TOTAL_ELEMENTS(pDd->SoundData))
	{
		pDd->SoundData[i].isCycle = isCycle;
		pfnKKAnsiiToUnicodeString((S8 *)filepath,WEP_TMP_PATH);
		fd = MMI_FS_Open((PU8)WEP_TMP_PATH, FS_OPEN_DIR| FS_READ_ONLY );
		if( fd >= 0 )
		{
			MMI_FS_Close(fd);

			pfnKKAnsiiToUnicodeString((S8 *)fileName,WEP_TMP_PATH);
			size = pfnKKUnicodeStrlen((S8 *)fileName);
			pfnKKUnicodeStrcat((S8 *)&fileName[size],(S8 *)(pDd->SoundData[i].sndName));
// 			for(j=0;pDd->SoundData[i].sndName[j-1]!='\0';j++){
// 				fileName[size+j]= pDd->SoundData[i].sndName[j];
// 			}
			//pfnKKAnsiiToUnicodeString((S8 *)&fileName[size],(S8 *)(pDd->SoundData[i].sndName));
			
			if(isCycle) {
				;//循环播放声音
			}
			else {
				;//播放一次
			}
			return TRUE;
		}
	}
#endif

	return FALSE;

}//LD_SoundDataInit()

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static bool_t LD_SoundDataProcess(ILD * pILd, int32_t sndType, int32_t sndIdx,
					byte_t * sndName, int32_t sndNameSize, byte_t buffer[], int32_t sndSize)
{
	LD * pLD = (LD*)pILd;
	WEPAppData * pWep = pLD->pWep;
	DeviceData * pDd = (DeviceData *)(pWep->pDeviceData);
	S32 free_space = 0;
	
#ifndef FILESYSTEM_ON_DEV
	UI_character_type  fileName[MAX_FILENAME_LEN];
	UI_character_type postName[16];
	FS_HANDLE fd;
//	int retval ;
	int  i, tmp;
//	UINT dwBytesWritten;
#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_SoundDataProcess");
#endif



	for(i=0;i<TOTAL_ELEMENTS(pDd->SoundData);i++) {
		if(pDd->SoundData[i].sndIdx == 0 || pDd->SoundData[i].sndIdx == sndIdx) {
			break;
		}
	}
	if(i < TOTAL_ELEMENTS(pDd->SoundData))
	{
		pfnKKAnsiiToUnicodeString((S8 *)fileName,WEP_TMP_PATH);
		fd = MMI_FS_Open((PU8)fileName, FS_OPEN_DIR | FS_READ_ONLY);
		if( fd >= 0 )
		{
			MMI_FS_Close(fd);

			tmp = pfnKKUnicodeStrlen((const S8 *)fileName);
			if(sndNameSize > 0) {
				DEV_NetUniByte2LocStr(&fileName[tmp], sndName, sndNameSize);
			}
			else
			{
				switch(sndType)
				{
				case SOUND_FORMAT_WAV:
					pfnKKAnsiiToUnicodeString((S8 *)postName, "wav");
					break;
				case SOUND_FORMAT_MIDI:
					pfnKKAnsiiToUnicodeString((S8 *)postName,"mid");	
					break;
				case SOUND_FORMAT_MMF:
					pfnKKAnsiiToUnicodeString((S8 *)postName, "mmf");	
					break;
				case SOUND_FORMAT_QCELP:
					pfnKKAnsiiToUnicodeString((S8 *)postName, "qcp");	
					break;
				default:
					pfnKKAnsiiToUnicodeString((S8 *)postName, "dat");	
					break;
				}
//				sprintf((PS8)&fileName[tmp], "%snd03d.%s", i, postName);
			}

			//Add by YHB, 2007-04-27. {{{
			free_space = DEV_GetDiskFreeBytes((const WCHAR *)&fileName);
			if (free_space <= sndSize)
				return FALSE;
			//Add by YHB, 2007-04-27. }}}

			fd = MMI_FS_Open((PU8)fileName, FS_READ_WRITE|FS_CREATE);
			if(fd >= 0)
			{
			//	retval = FS_Write( fd, buffer, sndSize);//, &dwBytesWritten);
				MMI_FS_Close(fd);

				pDd->SoundData[i].sndIdx = sndIdx;
	//			pDd->SoundData[i].isCycle = isCycle;
				pDd->SoundData[i].lenOfSndData = sndSize;
				pDd->SoundData[i].sndType = sndType;
				pfnKKUnicodeStrcpy((S8 *)(pDd->SoundData[i].sndName),(const S8 *)fileName);
			}
		}
	}
//	return (FS_NO_ERROR == retval);
#endif
	return FALSE;
}//end LD_SoundDataProcess()

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static bool_t LD_DownloadSoundSave(ILD * pILd, int32_t sndType,
			byte_t * sndName, int32_t sndNameSize, byte_t buffer[], int32_t sndSize)
{
//	LD * pLD = (LD*)pILd;
//	WEPAppData * pWep = pLD->pWep;
//	DeviceData * pDd = (DeviceData *)(pWep->pDeviceData);

	S32 free_space = 0;	//Add by YHB, 2007-04-27

#ifndef FILESYSTEM_ON_DEV
	UI_character_type  fileName[MAX_FILENAME_LEN];
	UI_character_type  postName[16];
	FS_HANDLE fd;
//	int retval;
	int  tmp;
//	UINT dwBytesWritten;
#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_DownloadSoundSave");
#endif

	pfnKKAnsiiToUnicodeString((S8 *)fileName, WEP_DOWNLOAD_PATH);
	fd = MMI_FS_Open((PU8)fileName, FS_OPEN_DIR | FS_READ_ONLY);
	if( fd >= 0 )
	{
		MMI_FS_Close(fd);

		tmp = pfnKKUnicodeStrlen((const S8 *)fileName);
		if(sndNameSize > 0) {
			DEV_NetUniByte2LocStr(&fileName[tmp], sndName, sndNameSize);
		}
		else
		{
			switch(sndType)
			{
			case SOUND_FORMAT_WAV:
					pfnKKAnsiiToUnicodeString((S8 *)postName, "wav");
					break;
				case SOUND_FORMAT_MIDI:
					pfnKKAnsiiToUnicodeString((S8 *)postName,"mid");	
					break;
				case SOUND_FORMAT_MMF:
					pfnKKAnsiiToUnicodeString((S8 *)postName, "mmf");	
					break;
				case SOUND_FORMAT_QCELP:
					pfnKKAnsiiToUnicodeString((S8 *)postName, "qcp");	
					break;
				default:
					pfnKKAnsiiToUnicodeString((S8 *)postName, "dat");	
					break;
			}
//			sprintf((PS8)&fileName[tmp], "%d.%s", sndSize, postName);
		}

		//Add by YHB, 2007-04-27. {{{
		free_space = DEV_GetDiskFreeBytes((const WCHAR *)&fileName);
		if (free_space <= sndSize)
			return FALSE;
		//Add by YHB, 2007-04-27. }}}

		fd = MMI_FS_Open((PU8)fileName, FS_READ_WRITE|FS_CREATE);
		if(fd >= 0)
		{
//			retval = FS_Write( fd, buffer, sndSize);//, &dwBytesWritten);
			MMI_FS_Close(fd);
		}
//		return (FS_NO_ERROR == retval);
	}
#endif
	return FALSE;
}//end LD_DownloadSoundSave()
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static bool_t LD_NotifySoundPlay(ILD * pILd, int32_t nDelayMs)
{
	LD * pLD = (LD*)pILd;
//	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);

#ifndef FILESYSTEM_ON_DEV
	if (pLD->pWep->appSoundOn)
	{
		//playRequestedTone(MESSAGE_TONE);
		//AudioPlayReq(CAMERA_DAOJISHI,DEVICE_AUDIO_PLAY_ONCE);


	}
// 	if(pLD->soundDelay > 0) {
// 
// 	}
#endif

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// static void LD_SoundDelaySet(ILD * pILd, int32_t nDelayMs)
// {
// 	LD * pLD = (LD*)pILd;
// 
// 	pLD->soundDelay = nDelayMs;
// }

#ifdef SUPPORT_FTP

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// generate UI(dir & file list) for user to select the file to upload
// return: <=0: error
//				 > 0: selected file size
static int32_t LD_FTP_UploadFileSelect(ILD * pILd)
{
	LD * pLD = (LD*)pILd;
 	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);

#ifndef FILESYSTEM_ON_DEV
	UI_character_type fileName[MAX_FILENAME_LEN] ;       // buffer for file name
//	UI_character_type str1[512],str2[128];
	byte_t szBuf[MAX_FILENAME_LEN];  // buffer for unicode bytes
	byte_t * pSZ;
	bool_t bRet = FALSE;
	int32_t i, nLen;
	UINT fileLen = 0;
	FS_HANDLE fd;

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_FTP_UploadFileSelect");
#endif

	pLD->reTransferPoint = 0;

	// dialog title
//	memset(str1,0x0,sizeof(str1));
//	pSZ = GETSTRRES(SZ_UPLOADNOTE, pLD->pWep->languageId);
//	DEV_NetUniByte2LocStr(str1, pSZ, UTL_UniStrLen(pSZ));

	if(!pDd->b_ImageInformationHaveGot)
	{
		pDd->hideWin = TRUE;//避免在打开文件夹时调用UI_WinShow()
		WM_MsgNotify(pDd, WEP_FILE_SELECT, 0, 0);
		bRet = FALSE;	// 打开文件夹,bRet = TRUE 表示选择了文件,FALSE 表示没有选择
	}
	else
	{
		bRet = TRUE;	// 打开文件夹,bRet = TRUE 表示选择了文件,FALSE 表示没有选择
		pDd->hideWin = FALSE;
	}

	if ( !bRet ) {
		return 0;
	}
	if(pLD->fileLen <= 0)
	{
//		DEV_MemSet(str1,0x0,sizeof(str1));
//		DEV_MemSet(str2,0x0,sizeof(str2));

		pSZ= GETSTRRES(SZ_FILEISNULL, pLD->pWep->languageId);
		DEV_NetUniByte2LocStr(pDd->bufMessage, pSZ, UTL_UniStrLen(pSZ));

		pSZ= GETSTRRES(SZ_MSGTITLE, pLD->pWep->languageId);
		DEV_NetUniByte2LocStr(pDd->titleMessage, pSZ, UTL_UniStrLen(pSZ));
		UI_MsgBoxShow(pLD->pWep->pIUI);
		return 0;	// invalid file size
	}
	

	if(pLD->fileLen > 30*1024)//30k////james 070508 begin
	{
//		memset(str1,0x0,sizeof(str1));
//		memset(str2,0x0,sizeof(str2));

		pSZ= GETSTRRES(SZ_FILETOOLONG, pLD->pWep->languageId);
		DEV_NetUniByte2LocStr(pDd->bufMessage, pSZ, UTL_UniStrLen(pSZ));  //added by zjwang in 06/25/2007 当上传的文件超过30K时，显示提示消息。

		pSZ= GETSTRRES(SZ_MSGTITLE, pLD->pWep->languageId);
		DEV_NetUniByte2LocStr(pDd->titleMessage,pSZ,UTL_UniStrLen(pSZ));

		UI_MsgBoxShow(pLD->pWep->pIUI);

		return 0;	// file too long 
	}//james 070508 end

	// check if file name too long or path name too long,
	nLen = DEV_LocStr2NetUniByte(szBuf, pLD->clientFile, pfnKKUnicodeStrlen((S8 *)pLD->clientFile));
	if(nLen > MAX_FILE_NAME_SIZE * 2) {
//		DEV_MemSet(str1,0x0,sizeof(str1));
//		DEV_MemSet(str2,0x0,sizeof(str2));

		pSZ= GETSTRRES(SZ_FILENAMETOOLONG, pLD->pWep->languageId);
		DEV_NetUniByte2LocStr(pDd->bufMessage, pSZ, UTL_UniStrLen(pSZ));

		pSZ= GETSTRRES(SZ_MSGTITLE, pLD->pWep->languageId);
		DEV_NetUniByte2LocStr(pDd->titleMessage, pSZ, UTL_UniStrLen(pSZ));
		UI_MsgBoxShow(pLD->pWep->pIUI);
		return 0;	// file name too long
	}
	nLen = DEV_LocStr2NetUniByte(szBuf, pLD->clientPath, pfnKKUnicodeStrlen((S8 *)pLD->clientPath));
	if(nLen > MAX_FILE_NAME_SIZE * 2) {
//		DEV_MemSet(str1,0x0,sizeof(str1));
//		DEV_MemSet(str2,0x0,sizeof(str2));

		pSZ= GETSTRRES(SZ_PATHTOOLONG, pLD->pWep->languageId);
		DEV_NetUniByte2LocStr(pDd->bufMessage, pSZ, UTL_UniStrLen(pSZ));

		pSZ= GETSTRRES(SZ_MSGTITLE, pLD->pWep->languageId);
		DEV_NetUniByte2LocStr(pDd->titleMessage, pSZ, UTL_UniStrLen(pSZ));

		UI_MsgBoxShow(pLD->pWep->pIUI);
		return 0;	// path name too long
	}

	
	return pLD->fileLen;
#endif

// 	return 0;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// (FTP)
static int32_t LD_FTP_CliPathNameGet(ILD * pILd, byte_t * pBuffer, int32_t nBufLen)
{
	LD * pLD = (LD*)pILd;

	return DEV_LocStr2NetUniByte(pBuffer, pLD->clientPath, pfnKKUnicodeStrlen((S8 *)pLD->clientPath));
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// (FTP) 
static int32_t LD_FTP_CliFileNameGet(ILD * pILd, byte_t * pBuffer, int32_t nBufLen)
{
	LD * pLD = (LD*)pILd;

	return DEV_LocStr2NetUniByte(pBuffer, (const UI_character_type *)pLD->clientFile, pfnKKUnicodeStrlen((S8 *)pLD->clientFile));
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// (FTP) 
static bool_t LD_FTP_CliPathNameSet(ILD * pILd, byte_t * pName, int32_t nNameLen)
{
	LD * pLD = (LD*)pILd;
// 	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);

#ifndef FILESYSTEM_ON_DEV
	UI_character_type szPathFile[MAX_FILENAME_LEN];
	int32_t nStartInx, nEndInx;

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_FTP_CliPathNameSet");
#endif

	if(nNameLen > 0) {
		DEV_NetUniByte2LocStr(szPathFile, pName, nNameLen);
		nStartInx = 0;
		nEndInx = pfnKKUnicodeStrlen((S8 *)szPathFile)-1;
		if(szPathFile[0] == '#') {
			nStartInx++;
		}
		if(szPathFile[nEndInx] == '#') {
			szPathFile[nEndInx] = '\0';
		}
		pfnKKUnicodeStrcpy((S8 *)pLD->clientPath, (const S8 *)&szPathFile[nStartInx]);
	} else {
		pLD->clientPath[0] = '\0';	// null string
		pLD->clientPath[1] = '\0';	// null string
	}
	return TRUE;
#endif
// 	return FALSE;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static bool_t LD_FTP_CliFileNameSet(ILD * pILd, byte_t * pName, int32_t nNameLen)
{
	LD * pLD = (LD*)pILd;
	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);

#ifndef FILESYSTEM_ON_DEV
	UI_character_type szPathFile[MAX_FILENAME_LEN];
	int32_t nStartInx, nEndInx;

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_FTP_CliFileNameSet");
#endif

	if(nNameLen > 0) {
		DEV_NetUniByte2LocStr(szPathFile, pName, nNameLen);
		nStartInx = 0;
		nEndInx = pfnKKUnicodeStrlen((const S8 *)szPathFile)-1;
		if(szPathFile[0] == '#') {
			nStartInx++;
		}
		if(szPathFile[nEndInx] == '#') {
			szPathFile[nEndInx] = '\0';
		}
		pfnKKUnicodeStrcpy((S8 *)pLD->clientFile, (const S8 *)&szPathFile[nStartInx]);
	}
	else
	{
		pfnKKUnicodeStrcpy((S8 *)pLD->clientFile, "Untitled-1");
	}

	if(pfnKKUnicodeStrlen((const S8 *)pLD->clientPath) == 0)//选择保存文件的路径
	{
		UI_character_type szFile[MAX_FILENAME_LEN]; // buffer for file name
		UI_character_type szDlgTitle[200];
		bool_t bRet = FALSE;
		byte_t * pSZ;

		// dialog title
		DEV_MemSet(szDlgTitle,0x0,sizeof(szDlgTitle));
		pSZ = GETSTRRES(SZ_DOWNLOADSAVE, pLD->pWep->languageId);
		DEV_NetUniByte2LocStr(szDlgTitle, pSZ, UTL_UniStrLen(pSZ));

		DEV_MemSet(szFile,0x0,sizeof(szFile));
		pfnKKUnicodeStrcpy((S8 *)szFile, (const S8 *)pLD->clientFile);

		// 打开文件夹
		//bRet = TRUE 表示选择了路径,FALSE 表示没有选择 
		if ( bRet ) {//选择了路径,保存于szFile
			pfnKKUnicodeStrcpy((S8 *)pLD->clientPath, (const S8 *)szFile);
			pLD->clientFile[0] = '\0';
		} else {
// 			// if user canceld,
// 			if(0 == CommDlgExtendedError()) {
// 				return FALSE;
// 			}
			pfnKKUnicodeStrcpy((S8 *)pLD->clientPath, WEP_PATH);
		}
	}

	return TRUE;
#endif
// 	return FALSE;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// (FTP) 
static int32_t LD_FTP_FileReTransPointGet(ILD * pILd)
{
	LD * pLD = (LD*)pILd;

	return pLD->reTransferPoint;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// (FTP) 
static bool_t LD_FTP_FileReTransPointSet(ILD * pILd, int32_t reTransPoint)
{
	LD * pLD = (LD*)pILd;

#ifndef FILESYSTEM_ON_DEV
	UI_character_type szPathFile[MAX_FILENAME_LEN];
	int32_t fileLen;
	FS_HANDLE fd;
	S32 size;

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_FTP_FileReTransPointSet");
#endif

	if(reTransPoint == 0) {
		pLD->reTransferPoint = 0;
		return TRUE;
	}

	pfnKKUnicodeStrcpy((S8 *)szPathFile, (const S8 *)pLD->clientPath);
	size = pfnKKUnicodeStrlen((const S8 *)szPathFile);
	pfnKKUnicodeStrcpy((S8 *)&szPathFile[size], (const S8 *)pLD->clientFile);

  if( (fd = MMI_FS_Open((PU8)szPathFile, FS_READ_ONLY)) >= 0 )
	{
		MMI_FS_GetFileSize(fd , (UINT *)&fileLen);
		if(reTransPoint <= fileLen) {
			MMI_FS_Close(fd);
			pLD->reTransferPoint = reTransPoint;
			return TRUE;
		}
		MMI_FS_Close(fd);
	}
#endif 
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static bool_t LD_FTP_UploadFileGet(ILD * pILd, 
			byte_t * pPathName, int32_t nPathNameLen, byte_t * pFileName, int32_t nFileNameLen, int32_t reTransferPoint)
{
	LD * pLD = (LD*)pILd;

#ifndef FILESYSTEM_ON_DEV
	UI_character_type szPathFile[MAX_FILENAME_LEN];
	S32 nSize;
	FS_HANDLE fd;

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_FTP_UploadFileGet");
#endif

	nSize = DEV_NetUniByte2LocStr(szPathFile, pPathName, nPathNameLen);
	DEV_NetUniByte2LocStr(&szPathFile[nSize], pFileName, nFileNameLen);

	// if [szPathFile] exist and file size longer than reTransferPoint
  if( (fd = MMI_FS_Open((PU8)szPathFile, FS_READ_ONLY)) >= 0)
	{
		MMI_FS_GetFileSize(fd, (UINT *)&nSize);
		MMI_FS_Close(fd);

		if(nSize >= reTransferPoint) {
			DEV_NetUniByte2LocStr(pLD->clientPath, pPathName, nPathNameLen);
			DEV_NetUniByte2LocStr(pLD->clientFile, pFileName, nFileNameLen);
			pLD->reTransferPoint = reTransferPoint;
			return TRUE;
		}
	}
#endif
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static bool_t LD_FTP_UploadFileDataGet(ILD * pILd, 
										byte_t * pOutBuffer, int32_t nBufferLen, int32_t * pDataLen)
{
	LD * pLD = (LD*)pILd;
	WEPAppData * pWep = pLD->pWep;
	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);
#ifndef FILESYSTEM_ON_DEV
	UI_character_type szPathFile[MAX_FILENAME_LEN];
	int32_t fileLen;
	int32_t len, bytesRead;
	int32_t nStart = pLD->reTransferPoint;
	FS_HANDLE fd;
	bool_t fileEnd = FALSE;

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_FTP_UploadFileDataGet");
#endif

	pfnKKUnicodeStrcpy((S8 *)szPathFile, (const S8 *)pLD->clientPath);
	len = pfnKKUnicodeStrlen((const S8 *)szPathFile);
	pfnKKUnicodeStrcpy((S8 *)&szPathFile[len], (const S8 *)pLD->clientFile);
	
	bytesRead = -1;
  if( (fd = MMI_FS_Open((PU8)szPathFile, FS_READ_ONLY)) >= 0 )
	{
		MMI_FS_GetFileSize(fd, (UINT *)&fileLen);
		if(fileLen == nStart)
		{
			bytesRead = 0;
		}
		else if(fileLen > nStart)
		{
#ifdef SUPPORT_PROXY
			if(pWep->appUseProxy)
			{
				if(nBufferLen > 600) 
				{
					nBufferLen = 600;
				}
			}	
			else
#endif
			if(nBufferLen > 1024)
			{
				nBufferLen = 1024;
			}
			len = fileLen - nStart;
			len = (len > nBufferLen) ? nBufferLen : len;

			MMI_FS_Seek(fd, nStart, FS_FILE_CURRENT);
			MMI_FS_Read(fd, pOutBuffer, (UINT)len,/*(UINT*)*/(INT32*)&bytesRead);
			if(bytesRead == (fileLen - nStart)) {
				fileEnd = TRUE;
			}
		}
		MMI_FS_Close(fd);
	}

	if(pDataLen != NULL) {
		*pDataLen = bytesRead;
	}

	return fileEnd;
#endif
 	return FALSE;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// (FTP) set download file data
static int32_t LD_FTP_DownloadFileDataSet(ILD * pILd, 
								byte_t * pInData, int32_t nDataLen, byte_t bEndFlag)
{
	LD * pLD = (LD*)pILd;

#ifndef FILESYSTEM_ON_DEV
	UI_character_type szPathFile[MAX_FILENAME_LEN];
	int32_t fileLen;
	int32_t bytesWrite = -1;
	int32_t nStart = pLD->reTransferPoint;
	FS_HANDLE fd;
	S32 size, free_space;

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_FTP_DownloadFileDataSet");
#endif

	pfnKKUnicodeStrcpy((S8 *)szPathFile, (S8 *)pLD->clientPath);
	size = pfnKKUnicodeStrlen((S8 *)szPathFile);
	pfnKKUnicodeStrcpy((S8 *)&szPathFile[size], (S8 *)pLD->clientFile);
	
	//Add by YHB, 2007-04-27. {{{
	free_space = DEV_GetDiskFreeBytes((const WCHAR *)&szPathFile);
	if (free_space <= nDataLen)
		return -1;
	//Add by YHB, 2007-04-27. }}}
	
	bytesWrite = -1;
  if( (fd = MMI_FS_Open((PU8)szPathFile, FS_READ_WRITE | FS_CREATE))  >= 0 )
	{
		MMI_FS_GetFileSize(fd, (UINT *)&fileLen);
		if(fileLen >= nStart) {
			MMI_FS_Seek(fd, nStart, FS_FILE_CURRENT);
			MMI_FS_Write(fd, pInData, nDataLen,(UINT *)&bytesWrite);
			if(bytesWrite != nDataLen) {
				bytesWrite = -2;
			}
		} 
		else {
			// error
		}
		MMI_FS_Close(fd);
	} else {
			// error
	}
	
	// if download OK,
	if((bytesWrite == nDataLen) && bEndFlag)
	{
		DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);
		byte_t * pSZ;
			
		pSZ	= GETSTRRES(SZ_DOWNLOADSAVE, pLD->pWep->languageId);
		nStart = DEV_NetUniByte2LocStr(szPathFile, pSZ, UTL_UniStrLen(pSZ));
		sprintf((PS8)&szPathFile[nStart], ":\n%s%s", pLD->clientPath, pLD->clientFile);

// 		MessageBoxIndirect(&msg);//提示文件下载成功
	}
	if(bytesWrite == nDataLen) {
		pLD->reTransferPoint += nDataLen;
	}

	return bytesWrite;
#endif
	return -1;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// (FTP) get download file length
static int32_t LD_FTP_DownloadFileGet(ILD * pILd, 
			byte_t * pPathName, int32_t nPathNameLen, byte_t * pFileName, int32_t nFileNameLen)
{
	LD * pLD = (LD*)pILd;

#ifndef FILESYSTEM_ON_DEV
	UI_character_type szPathFile[MAX_FILENAME_LEN];
	S32 size = -1;
	FS_HANDLE fd;

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_FTP_DownloadFileGet");
#endif

	pfnKKUnicodeStrcpy((S8 *)szPathFile, (const S8 *)pLD->clientPath);
	size = pfnKKUnicodeStrlen((const S8 *)szPathFile);
	pfnKKUnicodeStrcpy((S8 *)&szPathFile[size], (const S8 *)pLD->clientFile);

	// if [szPathFile] exist and file size longer than reTransferPoint
  if( (fd = MMI_FS_Open((PU8)szPathFile, FS_READ_ONLY)) >= 0 )
	{
		MMI_FS_GetFileSize(fd,(UINT *)&size);
		MMI_FS_Close(fd);
	}
	return size;
#endif
	return -1;
}

#endif//SUPPORT_FTP

#ifndef FILESYSTEM_ON_DEV
#if 0
static BOOL mmi_wep_get_storage_disk_path(PS8 drv_buf)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32		ret_drv;
	S8		buf[64];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef NAND_SUPPORT
	ret_drv	= FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);
#else
	ret_drv	= FS_GetDrive(FS_DRIVE_V_REMOVABLE , 1, FS_NO_ALT_DRIVE);
#endif /*NAND_SUPPORT*/

	if(ret_drv >=0)
	{
		sprintf(buf, "%c:\\", (U8)ret_drv);
		pfnKKAnsiiToUnicodeString(drv_buf, buf);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
#endif

S32 mmi_wep_create_file_dir(S8 *filepath, S8 dir_level)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8			buf_createpath[128];
	S8			buf_filepath[128];//[FMGR_PATH_BUFFER_SIZE];
	S32			fs_ret;
	S32			str_len;
	S32			cmp_ret;
	S32			i;
	FS_HANDLE	file_handle;
	S32			skip_tag;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	fs_ret = 0;
   
	file_handle = MMI_FS_Open((PU8)filepath, FS_OPEN_DIR | FS_READ_ONLY);

	/* path already exist */
	if(file_handle>=0)
	{
#ifdef WEP_DEBUG
//	WM_DebugPrint("mmi_wep_create_file_dir::file_handle>=0");
#endif
	//kal_prompt_trace(MOD_MMI,"mmi_wep_create_file_dir::file_handle>=0");
		MMI_FS_Close(file_handle);
		return 0; /* already exist, 0 means successful in file system */
	}
	
	fs_ret = MMI_FS_CreateDir((PU8)filepath);

		return fs_ret;	
			
	/* create directory if it is not there */
	pfnKKUnicodeStrcpy((PS8)buf_filepath, (PS8)filepath);


	str_len = pfnKKUnicodeStrlen((PS8)buf_filepath);
	str_len *= ENCODING_LENGTH;
	skip_tag=(dir_level==0)?3:7;
	/* skip drive. "D:\" or"D:\WEP\"*/
	for(i = skip_tag*ENCODING_LENGTH ; i < str_len; i+=2)
	{
		cmp_ret = pfnKKUnicodeStrncmp((PS8)&buf_filepath[i], (PS8)L"\\", 1);
		if(cmp_ret == 0)
		{
			/* create dir */

			pfnKKUnicodeStrncpy((PS8)buf_createpath, (PS8)buf_filepath, i/ENCODING_LENGTH);
			buf_createpath[i]		= '\0';
			buf_createpath[i+1]	= '\0';
			
//			fs_ret = FS_CreateDir((PU16)buf_createpath);	

			if(fs_ret < 0)
				return fs_ret;
		}
	}

	return fs_ret;
}
void CSD_WEPDirCreat(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S8		filepath[MAX_FILENAME_LEN];
   	S32		rt;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
//FS_CreateDir(L"D:\\WEP\\ld\\");	
	memset(filepath, 0, sizeof(filepath)); 
	pfnKKAnsiiToUnicodeString((PS8)filepath,WEP_PATH); 
	rt=mmi_wep_create_file_dir((PS8)filepath,0);
	//kal_prompt_trace(MOD_MMI,"mmi_wep_create_file_dir::WEP_PATH=%d",rt);
	pfnKKAnsiiToUnicodeString((PS8)filepath,WEP_LD_PATH);
	rt=mmi_wep_create_file_dir((PS8)filepath,1);
	//kal_prompt_trace(MOD_MMI,"mmi_wep_create_file_dir::WEP_LD_PATH=%d",rt);
	pfnKKAnsiiToUnicodeString((PS8)filepath,WEP_RES_PATH);
	rt=mmi_wep_create_file_dir((PS8)filepath,1);
	//kal_prompt_trace(MOD_MMI,"mmi_wep_create_file_dir::WEP_RES_PATH=%d",rt);
	pfnKKAnsiiToUnicodeString((PS8)filepath,WEP_TMP_PATH);
	mmi_wep_create_file_dir((PS8)filepath,1);
	pfnKKAnsiiToUnicodeString((PS8)filepath,WEP_LDF_PATH);
	mmi_wep_create_file_dir((PS8)filepath,1);
	pfnKKAnsiiToUnicodeString((PS8)filepath,WEP_DOWNLOAD_PATH);
	mmi_wep_create_file_dir((PS8)filepath,1);
}
#endif

#ifdef SUPPORT_BLOCK_SP_SMS
/*庞鹏 090506 关键字屏蔽短信 begin*/	
extern byte_t Wep_SmsKeyword[MAX_KEYWORD_NUM][MAX_STRING_RECORD_LENGTH];
/*
开机后从手机存储空间读取要屏蔽的短信关键字
*/
bool_t WEP_KeywordLoad(void)
{
	//LD * pLD = (LD*)pILd;
	bool_t result = FALSE;

#ifndef FILESYSTEM_ON_DEV
	{
		FS_HANDLE fd;
		/*S32*/INT32 size = 0;
		UI_character_type fileName[MAX_FILENAME_LEN];

#ifdef WEP_DEBUG			
	WM_DebugPrint("LD_DataLoad");
#endif
		pfnKKAnsiiToUnicodeString((S8 *)fileName,WEP_LD_PATH);
		fd = MMI_FS_Open((PU8)fileName, FS_OPEN_DIR| FS_READ_ONLY );
		if( fd >= 0 )
		{
			MMI_FS_Close( fd );

			pfnKKAnsiiToUnicodeString((S8 *)fileName,WEP_LD_PATH);
			size = pfnKKUnicodeStrlen((S8 *)fileName);
			pfnKKAnsiiToUnicodeString((S8 *)&fileName[size],WEP_LD_FILENAME);

			fd = MMI_FS_Open((PU8)fileName, FS_READ_ONLY );
			if( fd >= 0 )
			{
				if( MMI_FS_Seek( fd, (sizeof (int32_t))*(MAX_STRING_RECORD + MAX_INTEGER_RECORD)  + MAX_STRING_RECORD * MAX_STRING_RECORD_LENGTH + 4, 
					FS_FILE_BEGIN ) >= 0 )
				{
					if(FS_NO_ERROR == MMI_FS_Read( fd, (PU8)&Wep_SmsKeyword, MAX_KEYWORD_NUM*MAX_STRING_RECORD_LENGTH, /*(UINT*)*/(INT32*)&size)) 
					{
						result = TRUE;
					}
				}
				MMI_FS_Close( fd );
			}

		}
	}
#endif

	return result;
}

static bool_t  LD_CHPDataSet(ILD * pILd, int32_t index, const byte_t *pData, int32_t nLen)
{
	LD * pLD = (LD*)pILd;
//	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);
//	WEPAppData * pWep = (WEPAppData *)(pLD->pWep);

//	int32_t i;

	if((index < 0) )
		return FALSE;

	if(nLen < 0) 
		return FALSE;

	if(pData != NULL) {
		DEV_MemCpy(pLD->ldGP.CHPData, pData, nLen);
	}

	return TRUE;	
}

static int32_t LD_CHPDataGet(ILD * pILd, int32_t index, byte_t *pData,int32_t nLen)
{
	LD * pLD = (LD*)pILd;
//	DeviceData * pDd = (DeviceData *)(pLD->pWep->pDeviceData);

//	int32_t i;

	if((index < 0) || (index >= MAX_SUPPORT_SP_LIST)||pData == NULL)
		return -1;


	if(pData != NULL) {
		DEV_MemCpy(pData, pLD->ldGP.CHPData, nLen);
	}
	
	return 1;

}

#endif

#endif
