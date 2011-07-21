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
 * EbookSrc.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/***************** Ebook Application implementation *********************/

/**************************************************************

   FILENAME : EbookSrc.c

   PURPOSE     : Ebook Appliocation

 

 

   DATE     : 

**************************************************************/

/*  */
#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "settingprofile.h"
#include "l4dr.h"
#include "wgui_categories.h"


#ifdef __MMI_EBOOK_READER__
#include "gpioinc.h"
#include "ebookdefs.h"
#include "ebookprot.h"
#include "ebookmanager.h"
#include "commonscreens.h"
#include "downloaddefs.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "wgui_inputs.h"
#include "drm_gprot.h"
#include "filemanagerdef.h"
#include "simdetectiondef.h"
#include "usbdevicegprot.h"
#include "keybrd.h"
#include "mmi_trace.h"
#include "wgui_categories_popup.h"
#include "extdevicedefs.h"

#undef __NEWSIMULATOR
#ifdef __MMI_BPP_SUPPORT__  /*BPP revise*/
#include "BTMMICm.h"
#include "BTMMIBppSpGprot.h"
#include "SimDetectionGprot.h"

extern S8* ebr_get_current_book_name(void);
extern U32 ebr_get_current_book_size(void);

#endif /* __MMI_BPP_SUPPORT__ */ 

/* Global Variables */
extern pBOOL IsBackHistory;
g_ebook_app_cntx g_ebook_app_context;
g_ebook_app_cntx *g_ebook_app_cntx_p = &g_ebook_app_context;
g_ebook_inline_const_string_cntx g_ebook_inline_const_string_context;
g_ebook_inline_const_string_cntx *g_ebook_inline_const_string_context_p = &g_ebook_inline_const_string_context;
EBOOKSET_CNTX g_ebookset_cntx;
EBOOKSET_CNTX *g_ebookset_cntx_p = &g_ebookset_cntx;
gui_page_info_app ebook_frm_setting_struct;
gui_page_info_app *ebook_frm_setting = &ebook_frm_setting_struct;

extern g_current_book_context *g_curr_book_cntx_p;


static U8 search_string_book_name[(MAX_DWNL_EBK_PATH_LEN + 1) * ENCODING_LENGTH];

/* FMGR_Revise */
U16 g_ebook_explorer_id;

static const S8 ebook_head_template[]=    
"<head>\r\n"
"<title>ebook reader</title>\r\n"
"<style type=\"text/css\">\r\n";


static const S8 ebook_style_template[]=  
"table {border-collapse: collapse; border-spacing: 0pt;}\r\n"
"div.center { text-align: center; }\r\n"
".text-css {size:%dmm %dmm; color: Black; vertical-align: top; text-align: left;}\r\n"
"</style>\r\n"
"</head>\r\n";

static const S8 ebook_body_template[]=
"<body>\r\n"
"<p class=\"text-css\">\r\n"
"<bpp-inline src=\"%s\" inline-size=\"%d\" type=\"text/plain\" />\r\n" /*ebook file*/
"</p>\r\n"
"</body>\r\n"
"</html>\r\n";

static const S8 ebr_page_header[]=
"ebook reader";

static const S8 ebr_page_footer[]=
"\" Page \" counter(pages)";

#define ebr_compose_buf_size 1024
#define MAX_EBOOK_SIZE_STR  10

#define EBR_FLUSH_BUF(fh,pbuf,buf_remain)\
do{\
    if(mmi_bt_bpp_flush_buf(fh, pbuf,ebr_compose_buf_size,&buf_remain)<0)\
    {\
        goto BPP_ERROR_CLEAN;\
    }\
}while(0)

#define EBR_WRITE_DATA(fh,pbuf,buf_remain,pdata)\
do{\
    if(mmi_bt_bpp_write_data(fh,pbuf,ebr_compose_buf_size,&buf_remain,(S8*)pdata,strlen(pdata))<0)\
    {\
        goto BPP_ERROR_CLEAN;\
    }\
}while(0)

#ifdef __MMI_GB__
extern void GBSetInputboxLSKFunction(FuncPtr f);
#endif

/*****************************************************************************
 * FUNCTION
 *  mmi_ebook_usb_mode_on
 * DESCRIPTION
 *  For USB Power On Mode
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ebook_usb_mode_on(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     
    g_ebookset_cntx_p->is_usb_plugged_in = TRUE;

}


/*****************************************************************************
 * FUNCTION
 *  mmi_ebook_usb_mode_off
 * DESCRIPTION
 *  For USB Power On Mode
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ebook_usb_mode_off(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ebookset_cntx_p->is_usb_plugged_in = FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_book_shel_screen
 * DESCRIPTION
 *  Entry function for book shelf screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_book_shel_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FMGR_FILTER filter;
    S32 return_value;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if (g_ebookset_cntx_p->is_usb_plugged_in == TRUE)
    {
    	 DisplayPopup(
            (PU8) GetString(STR_ID_USB_MODE_EXPORTED_WARNING),
            IMG_GLOBAL_ERROR,
            0,
            UI_POPUP_NOTIFYDURATION_TIME,
            (U8) ERROR_TONE);      
         
        return;
    }
            
    ebr_frm_set_src_application_type();
    return_value = ebr_frm_read_from_nvram();
    ebr_frm_delete_unused_nfo_file();

    if (return_value == 1)
    {
        gdi_layer_lock_frame_buffer();
    }
    {
        FMGR_FILTER_INIT(&filter);
        FMGR_FILTER_SET(&filter, FMGR_TYPE_EBOOK_TXT);
        FMGR_FILTER_SET(&filter, FMGR_TYPE_EBOOK_PDB);
        
        mmi_fmgr_select_path_and_enter(
                APP_EBOOK,
                FMGR_SELECT_BROWSE,
                filter,
                (PS8) ebr_frm_get_path_for_bookshelf(),
                ebr_book_file_callback);
#if 0        
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
    }
     g_ebook_explorer_id = GetActiveScreenId();
    if (return_value == 1)
    {
        gdi_layer_unlock_frame_buffer();
        
        ebr_entry_open_book();
    }
    ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
/*****************************************************************************
 * FUNCTION
 *  ebr_book_file_callback
 * DESCRIPTION
 *  Call back for Book shelf Screen
 * PARAMETERS
 *  fullname        [IN]        
 *  is_short        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_book_file_callback(void *fullname, int is_short)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GoBackHistory();
}

/*****************************************************************************
 * FUNCTION
 *  ebr_show_book_shelf_option
 * DESCRIPTION
 *  Options for Book Shelf
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_show_book_shelf_option(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U16 nNumofItem;
    U16 nStrItemList[MAX_SUB_MENUS];
    U16 nImgIltemList[MAX_SUB_MENUS];
    U8 *hintList[MAX_SUB_MENUS];
    S16 count_no_of_nfo_file;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(EBOOK_OPEN_OPTION_SCREENID, NULL, ebr_show_book_shelf_option, NULL);
    guiBuffer = GetCurrGuiBuffer(EBOOK_OPEN_OPTION_SCREENID);
    count_no_of_nfo_file = ebr_frm_get_total_countof_nfo_file();

    if (count_no_of_nfo_file <= 0)
    {

        nNumofItem = GetNumOfChild(MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION);
        GetSequenceStringIds(MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION, nStrItemList);
        GetSequenceImageIds(MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION, nImgIltemList);
        SetParentHandler(MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION);
    }
    else
    {
        nNumofItem = GetNumOfChild(MENU_ID_EBK_BOOKSHELF_OPTION);
        GetSequenceStringIds(MENU_ID_EBK_BOOKSHELF_OPTION, nStrItemList);
        GetSequenceImageIds(MENU_ID_EBK_BOOKSHELF_OPTION, nImgIltemList);
        SetParentHandler(MENU_ID_EBK_BOOKSHELF_OPTION);
    }

    /* Construct List To Show Hint */
    ConstructHintsList(MENU_ID_EBK_BOOKSHELF_OPTION, hintList);

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    ShowCategory52Screen(
        STR_GLOBAL_OPTIONS,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nNumofItem,
        nStrItemList,
        (U16*) gIndexIconsImageList,
        hintList,
        0,
        0,
        guiBuffer);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    //  ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    // ClearKeyHandler(KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************
 * FUNCTION
 *  mmi_ebook_get_menuId
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
U16 mmi_ebook_get_menuId(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 count_no_of_nfo_file;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    count_no_of_nfo_file = ebr_frm_get_total_countof_nfo_file();
    mmi_trace(g_sw_Ebr,"[EBOOK] mmi_ebook_get_menuId count_no_of_nfo_file=%d",count_no_of_nfo_file);
    //ebr_frm_write_default_value_for_nfo_id_in_nvram();
    if (count_no_of_nfo_file <= 0)
    {
        return MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION;
    }
    else
    {
        return MENU_ID_EBK_BOOKSHELF_OPTION;
    }
}

/* FMGR_Revise */
/*****************************************************************************
 * FUNCTION
 *  mmi_ebook_option_enabler
 * DESCRIPTION
 *  Enable & Disable the option menu item
 * PARAMETERS
 *  menu_id
 *  fileinfo
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ebook_option_enabler(U16 menu_id, const FMGR_FILE_INFO_STRUCT* fileinfo)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 count_no_of_nfo_file;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    count_no_of_nfo_file = ebr_frm_get_total_countof_nfo_file();
    ebr_frm_rwrite_into_nvram();
    if (count_no_of_nfo_file <= 0)
    {
        mmi_frm_hide_menu_item(MENU_ID_EBK_OPEN);
        mmi_frm_hide_menu_item(MENU_ID_EBK_RFS_OPTION);
        mmi_frm_hide_menu_item(MENU_ID_EBK_SETTINGS);
        mmi_frm_hide_menu_item(MENU_ID_EBK_INFO);
        mmi_frm_hide_menu_item(MENU_ID_EBK_DELETE);
        //mmi_frm_hide_menu_item(MENU_ID_FMGR_GEN_OPTION_DELETE_ALL);
        //mmi_frm_hide_menu_item(MENU_ID_FMGR_GEN_OPTION_SORT);
        mmi_frm_hide_menu_item(MENU_ID_EBK_RESTORE_DEFAULT);
        mmi_frm_hide_menu_item(MENU_ID_EBK_HELP);
        mmi_frm_hide_menu_item(MENU_ID_EBK_RFS_OPTION_LIMITED);
    }
    else
    {
        mmi_frm_hide_menu_item(MENU_ID_EBK_RFS_OPTION);
        mmi_frm_hide_menu_item(MENU_ID_EBK_RFS_OPTION_LIMITED);
        mmi_frm_hide_menu_item(MENU_ID_EBK_SETTINGS_LIMITED);
    }

}

/*****************************************************************************
 * FUNCTION
 *  ebr_exit_book_shel_screen
 * DESCRIPTION
 *  Exit function of book shelf screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_exit_book_shel_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 return_value = 0;
    U8 isShelfExist = 1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_Ebr,"[EBOOK]ebr_exit_book_shel_screen begin");
    
    if (!IsScreenPresent(EBOOK_BOOKSHELF_SCREENID))
    {
        mmi_trace(g_sw_Ebr,"[EBOOK]ebr_exit_book_shel_screen 1");
        GenericExitScreen(EBOOK_BOOKSHELF_SCREENID, ebr_entry_book_shel_screen);
        isShelfExist = 0;
    }
    else if(GetExitScrnID() == EBOOK_BOOKSHELF_SCREENID)
    {
        mmi_trace(g_sw_Ebr,"[EBOOK]ebr_exit_book_shel_screen 2");
        GenericExitScreen(EBOOK_BOOKSHELF_SCREENID, ebr_entry_book_shel_screen);
    }

    /* If bookshelf screen will entry open book screen directly, 
    EBOOK_BOOKSHELF_SCREENID should be deleted manually 
    because decrease command in goback procedure won't be executed 
    The condition entry ebook reader by previous reading should be exclusive*/
    return_value = ebr_frm_read_from_nvram();
    if (return_value == 1 && isShelfExist == 1)
    {
        mmi_trace(g_sw_Ebr,"[EBOOK]ebr_exit_book_shel_screen 3");
        DeleteNScrId(EBOOK_BOOKSHELF_SCREENID);
    }

    mmi_trace(g_sw_Ebr,"[EBOOK]ebr_exit_book_shel_screen end");
}


/*****************************************************************************
 * FUNCTION
 *  InitEbookApp
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void InitEbookApp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetHiliteHandler(MAIN_MENU_EBOOK_MENUID, highlight_mainmenu_ebook);
    SetHiliteHandler(MENU_ID_EBK_RFS_OPTION, ebr_highlight_refresh_book_shelf);
    SetHiliteHandler(MENU_ID_EBK_RFS_OPTION_LIMITED, ebr_highlight_refresh_book_shelf);
    SetHiliteHandler(MENU_ID_EBK_OPEN, ebr_highlight_open_ebook);
    SetHiliteHandler(MENU_ID_EBK_INFO, ebr_higlight_book_info);

    SetHiliteHandler(MENU_ID_EBK_DELETE, ebr_highlight_delete_ebook);
    SetHiliteHandler(MENU_ID_EBK_HELP, ebr_highlight_help);
    SetHiliteHandler(MENU_ID_EBK_SETTINGS, ebr_highlight_book_shelf_settings);
    SetHiliteHandler(MENU_ID_EBK_SETTINGS_LIMITED, ebr_highlight_book_shelf_settings);

    SetHiliteHandler(MENU_ID_EBK_FIND, ebr_highlight_search);
    SetHiliteHandler(MENU_ID_EBK_FIND_NEXT, ebr_highlight_search_next);
    SetHiliteHandler(MENU_ID_EBK_JUMP, ebr_highlight_jump_to);
    SetHiliteHandler(MENU_ID_EBK_GOTO_BOOKMARK, ebr_highlight_goto_book_mark);
    SetHiliteHandler(MENU_ID_EBK_REMOVE_BOOKMARK, ebr_highlight_remove_book_mark);
    SetHiliteHandler(MENU_ID_EBK_ADD_BOOKMARK, ebr_highlight_add_book_mark);
    SetHiliteHandler(MENU_ID_EBK_BOOK_SETTINGS, ebr_highlight_book_settings);
    SetHiliteHandler(MENU_ID_EBK_BOOK_INFO, ebr_highlight_open_book_info);
    SetHiliteHandler(MENU_ID_EBK_CLOSE, ebr_close_book);

    SetHiliteHandler(MENU_ID_EBK_CASE_SENSITIVE, ebr_highlight_case_sensitive_serach);
    SetHiliteHandler(MENU_ID_EBK_CASE_INSENSITIVE, ebr_highlight_case_in_sensitive_serach);
    SetHiliteHandler(MENU_ID_EBK_BOOK_RESTORE_DEFAULT, ebr_highlight_ebk_restore_default);
    SetHiliteHandler(MENU_ID_EBK_DELETE_ALL, ebr_highlight_delete_all);
    SetHiliteHandler(MENU_ID_EBK_RESTORE_DEFAULT, ebr_highlight_book_shelf_restore_default);
    memset(g_ebook_app_cntx_p->ebr_search_string_input, 0, sizeof(g_ebook_app_cntx_p->ebr_search_string_input));
    ebr_frm_init();
}

/* FMGR_Revise */
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
#endif

/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_delete_all
 * DESCRIPTION
 *  Set highlight handler Delete All
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_delete_all(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    highlight_fmgr_delete_all_file();
}

/*****************************************************************************
 * FUNCTION
 *  ebr_book_fmgr_empty_list_callback
 * DESCRIPTION
 *  Call back for Book shelf Screen
 * PARAMETERS
 *  fullname        [IN]        
 *  is_short        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_book_fmgr_empty_list_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	DeleteScreenIfPresent(EBOOK_BOOKSHELF_SCREENID);
    GoBackHistory();
}

/*****************************************************************************
 * FUNCTION
 *  ebr_show_limit_book_shelf_option
 * DESCRIPTION
 *  Options for Limited Book Shelf
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_show_limit_book_shelf_option(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U16 nNumofItem;
    U16 nStrItemList[MAX_SUB_MENUS];
    U16 nImgIltemList[MAX_SUB_MENUS];
    U8 *hintList[MAX_SUB_MENUS];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(EBOOK_OPEN_OPTION_SCREENID, NULL, ebr_show_limit_book_shelf_option, NULL);
    guiBuffer = GetCurrGuiBuffer(EBOOK_OPEN_OPTION_SCREENID);
    
    nNumofItem = GetNumOfChild(MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION);
    GetSequenceStringIds(MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION, nStrItemList);
    GetSequenceImageIds(MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION, nImgIltemList);
    SetParentHandler(MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION);
    
    /* Construct List To Show Hint */
    ConstructHintsList(MENU_ID_EBK_BOOKSHELF_OPTION, hintList);

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    ShowCategory52Screen(
        STR_GLOBAL_OPTIONS,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nNumofItem,
        nStrItemList,
        (U16*) gIndexIconsImageList,
        hintList,
        0,
        0,
        guiBuffer);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}

/* FMGR_Revise */
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
#endif

/*****************************************************************************
 * FUNCTION
 *  ebr_entry_refresh_book_shelf
 * DESCRIPTION
 *  Refreshed Book Shelf Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_refresh_book_shelf(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_please_wait_actual_screen_2();
    ebr_frm_entry_refresh_book_shelf();
    DisplayPopup(
        (PU8) GetString(STR_BOOK_SHELF_REFRESHED),
        IMG_GLOBAL_ACTIVATED,
        1,
        EBR_ST_NOTIFYDURATION,
        SUCCESS_TONE);
    //mmi_fmgr_close_option_screen();
	/*if (IsScreenPresent(SCR_FMGR_APP_FILE_OPTION))
    {
        DeleteNScrId(SCR_FMGR_APP_FILE_OPTION);
    }
    else
    {
        DeleteNScrId(EBOOK_OPEN_OPTION_SCREENID);
    }*/
}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_refresh_book_shelf
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_refresh_book_shelf(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* 1 Change left soft key icon and label */
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

    /* 2 Change right soft key icon and label */
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);

    /* 3 Register function for left soft key */
    SetLeftSoftkeyFunction(ebr_entry_refresh_book_shelf, KEY_EVENT_UP);

    /* 4 Register function for right soft key */
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_ebk_restore_default
 * DESCRIPTION
 *  Set highlight handler for Restore Default
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_ebk_restore_default(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);

    SetLeftSoftkeyFunction(ebr_app_restore_default_book_settings, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/* FMGR_Revise */
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
#endif


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_book_shelf_restore_default
 * DESCRIPTION
 *  Set highlight handler for Restore Default
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_book_shelf_restore_default(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);

    SetLeftSoftkeyFunction(ebr_app_restore_default_book_shelf_settings, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/* FMGR_Revise */
/*****************************************************************************
 * FUNCTION
 *  ebr_open_lsk_hdlr
 * DESCRIPTION
 *  Left soft key handler for open ebook
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void ebr_open_lsk_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FMGR_FILE_INFO_STRUCT file_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_frm_reset_search_flag();
    //mmi_fmgr_get_current_fileinfo(&file_info, NULL);
    mmi_fmgr_kernel_get_file_info(mmi_fmgr_kernel_get_cur_idx(), &file_info);
    ebr_set_selected_file_name(&file_info);

    ebr_entry_open_book();
	//mmi_fmgr_close_option_screen();
}

/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_open_ebook
 * DESCRIPTION
 *  Set highlight handler for Open book
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
/* FMGR_Revise start */
void ebr_highlight_open_ebook(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);

    SetLeftSoftkeyFunction(ebr_open_lsk_hdlr, KEY_EVENT_UP);
    SetKeyHandler(ebr_open_lsk_hdlr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
/* FMGR_Revise end */


/* FMGR_Revise */
/*****************************************************************************
 * FUNCTION
 *  ebr_info_lsk_hdlr
 * DESCRIPTION
 *  Left soft key handler for open ebook info
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void ebr_info_lsk_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FMGR_FILE_INFO_STRUCT file_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    //mmi_fmgr_get_current_fileinfo(&file_info, NULL);
    mmi_fmgr_kernel_get_file_info(mmi_fmgr_kernel_get_cur_idx(), &file_info);
    ebr_set_selected_file_name(&file_info);

    ebr_entry_book_info();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_higlight_book_info
 * DESCRIPTION
 *  Set highlight handler for Book Info
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_higlight_book_info(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
/* FMGR_Revise */
    SetLeftSoftkeyFunction(ebr_info_lsk_hdlr, KEY_EVENT_UP);
    SetKeyHandler(ebr_info_lsk_hdlr, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_delete_ebook
 * DESCRIPTION
 *  Set highlight handler for Delete Book
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_delete_ebook(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_entry_delete_book_popup, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_delete_book_popup
 * DESCRIPTION
 *  Display popUp for Delete and confirm deletion
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 extern U8 PhnsetGetDefEncodingType(void);
void ebr_entry_delete_book_popup(void)
{
    /* FMGR_Revise */
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FMGR_FILE_INFO_STRUCT file_info;
	U8	str[100 * ENCODING_LENGTH];
    S32 len = 0;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    //mmi_fmgr_get_current_fileinfo(&file_info, NULL);
    mmi_fmgr_kernel_get_file_info(mmi_fmgr_kernel_get_cur_idx(), &file_info);
    ebr_set_selected_file_name(&file_info);
	pfnUnicodeStrcpy((S8*)str, (S8*)get_string(STR_EBOOK_DELETE_INQUIRE));

    if (file_info.is_short)
    {
        len = pfnUnicodeStrlen((S8*) str) << 1;
        mmi_chset_mixed_text_to_ucs2_str(
            (U8*) & str[len],
            (MAX_SUBMENU_CHARACTERS - 1) * ENCODING_LENGTH,
            (U8*) file_info.file_name,
            PhnsetGetDefEncodingType());
    }
    else
    {
        pfnUnicodeStrcat((S8*)str, (S8*)file_info.file_name);
    }
  

     
    pfnUnicodeStrcat((S8*) str, (S8*)get_string(STR_EBOOK_QUESTION_SIGN));

    DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        (UI_string_type)str,
        IMG_GLOBAL_QUESTION,
        WARNING_TONE);

    SetLeftSoftkeyFunction(ebr_entry_delete_ebook, KEY_EVENT_UP);
    SetRightSoftkeyFunction(ebr_delete_no_ebook, KEY_EVENT_UP);
    //mmi_fmgr_close_option_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_help
 * DESCRIPTION
 *  Set highlight handler for Help
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_help(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_entry_help, KEY_EVENT_UP);
    SetKeyHandler(ebr_entry_help, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_book_shelf_settings
 * DESCRIPTION
 *  Set highlight handler for Settings
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_book_shelf_settings(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_entry_settings, KEY_EVENT_UP);
    SetKeyHandler(ebr_entry_settings, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    g_ebook_app_cntx_p->ebr_set_local_setting_flag = 0;
	g_ebook_app_cntx_p->current_setting_highlight_item = 0;
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_search
 * DESCRIPTION
 *  Set highlight handler for Search
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_search(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_entry_search, KEY_EVENT_UP);
    SetKeyHandler(ebr_entry_search, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_search_next
 * DESCRIPTION
 *  Set highlight handler for find next Search
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_search_next(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_app_search_string_next_from_menu, KEY_EVENT_UP);
    SetKeyHandler(ebr_app_search_string_next_from_menu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_search_result_successful
 * DESCRIPTION
 *  Set the flag
 * PARAMETERS
 *  result      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_search_result_successful(pBOOL result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ebook_app_cntx_p->ebr_search_found_next_flag = result;
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_search_string_next_from_menu
 * DESCRIPTION
 *  Shows the Find NExt menu option
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_search_string_next_from_menu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_INDIC_ALG__
    pBOOL full_character;
#endif 
	S32 search_satus;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(!ebr_frm_check_file_exist())
	{
		ebr_app_display_pop_up_error_opening_book();
		go_back_to_bookshelf_screen();
		return;
	}
    if (pfnUnicodeStrcmp((S8 *)search_string_book_name, (S8 *)ebr_fmgr_get_file_name()))
    {
        return;
    }
    ebr_please_wait_actual_screen_2();
#ifdef __MMI_INDIC_ALG__
    full_character = ebr_frm_check_for_full_characters((S16*) g_ebook_app_cntx_p->ebr_search_string_input);
    if (!full_character)
    {
        ebr_app_show_string_incomplete_pop_up();
        return;
    }
#endif /* __MMI_INDIC_ALG__ */ 
	search_satus = ebr_frm_search_string_algo_find_next();
	if (search_satus == 0xFF)
    {
        return;
    }
    if (search_satus==1)
    {
        ebr_app_search_result_successful(TRUE);
        ebr_app_delete_upto_open_book_screen_id();
        GoBackHistory();
        ebr_frm_set_data_for_display();
    }
    else
    {
        if(search_satus==2)
		{
			ebr_app_search_result_invalid();
		}
		else
		{
        ebr_app_search_result_unsucessfull();
    }
}
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_search_string_next
 * DESCRIPTION
 *  called when find next is executed
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_search_string_next(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#ifdef __MMI_INDIC_ALG__
    pBOOL full_character;
#endif 
	S32 search_satus;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	if(!ebr_frm_check_file_exist())
	{
		ebr_app_display_pop_up_error_opening_book();
		go_back_to_bookshelf_screen();
		return;
	}
    if (pfnUnicodeStrcmp((S8 *)search_string_book_name, (S8 *)ebr_fmgr_get_file_name()))
    {
        return;
    }
    ebr_please_wait_actual_screen_2();
#ifdef __MMI_INDIC_ALG__
    full_character = ebr_frm_check_for_full_characters((S16*) g_ebook_app_cntx_p->ebr_search_string_input);
    if (!full_character)
    {
        ebr_app_show_string_incomplete_pop_up();
        return;
    }
#endif /* __MMI_INDIC_ALG__ */ 
	search_satus = ebr_frm_search_string_algo_find_next();
	if (search_satus == 0xFF)
    {
        return;
    }
    else if (search_satus==1)
    {
        ebr_app_search_result_successful(TRUE);
        g_ebook_app_cntx_p->enable_screen_exit = 0;
        EntryNewScreen(SCR_ID_EBOOK_WAITSCREEN, NULL, NULL, NULL);
        g_ebook_app_cntx_p->enable_screen_exit = 1;
        ebr_app_delete_upto_open_book_screen_id();
        GoBackHistory();
        ebr_frm_set_data_for_display();
    }
    else
    {
        if(search_satus==2)
		{
			ebr_app_search_result_invalid();
		}
		else
		{
        ebr_app_search_result_unsucessfull();
    }
}
}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_jump_to
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_jump_to(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_entry_jump_to, KEY_EVENT_UP);
    SetKeyHandler(ebr_entry_jump_to, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    memset(g_ebook_app_cntx_p->percentage_val, 0, (EBR_MAX_EBOOK_PERCENTAGE_VALUE + 1) * ENCODING_LENGTH);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 *  ebr_pre_entry_jump_to
 * DESCRIPTION
 *  Pre Entry function for jumpTo
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_pre_entry_jump_to(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset(g_ebook_app_cntx_p->percentage_val, 0, (EBR_MAX_EBOOK_PERCENTAGE_VALUE + 1) * ENCODING_LENGTH);
    ebr_entry_jump_to();
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_goto_book_mark(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_entry_goto_book_mark, KEY_EVENT_UP);
    SetKeyHandler(ebr_entry_goto_book_mark, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_remove_book_mark(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_entry_remove_book_mark, KEY_EVENT_UP);
    SetKeyHandler(ebr_entry_remove_book_mark, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_remove_book_mark(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    S16 no_of_book_marks = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(!ebr_frm_check_file_exist())
	{
		ebr_app_display_pop_up_error_opening_book();
		go_back_to_bookshelf_screen();
		return;
	}
    no_of_book_marks = ebr_frm_get_count_of_book_marks();

    EntryNewScreen(EBOOK_REMOVE_BOOKMARK_SCREENID, NULL, ebr_entry_remove_book_mark, NULL);

    guiBuffer = GetCurrGuiBuffer(EBOOK_REMOVE_BOOKMARK_SCREENID);
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
    SetParentHandler(0);
    if (no_of_book_marks == 0)
    {   /*   ShowCategory184Screen(STR_RMV_BOOKMARK_EBOOK, (U16)NULL,
           0, 0,
           STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
           no_of_book_marks,ebr_get_book_mark_name_list,
           NULL, 0, guiBuffer); */
        DisplayPopup(
            (PU8) GetString(STR_NO_BOOK_MARK_PRESENT),
            IMG_GLOBAL_WARNING,
            1,
            EBR_ST_NOTIFYDURATION,
            EMPTY_LIST_TONE);
        DeleteNScrId(EBOOK_REMOVE_BOOKMARK_SCREENID);
        return;

        /* GoBackHistory(); */

    }
    else
    {
        ShowCategory184Screen(
            STR_RMV_BOOKMARK_EBOOK,
            GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
            STR_GLOBAL_OK,
            IMG_GLOBAL_OK,
            STR_GLOBAL_BACK,
            IMG_GLOBAL_BACK,
            no_of_book_marks,
            ebr_get_book_mark_name_list,
            NULL,
            0,
            guiBuffer);
    }

    /* register LSK and right key handlers */
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetLeftSoftkeyFunction(ebr_delete_bookmark, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_delete_bookmark(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(!ebr_frm_check_file_exist())
	{
		ebr_app_display_pop_up_error_opening_book();
		go_back_to_bookshelf_screen();
		return;
	}
    DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        get_string(STR_DELETE_BOOK_MARK),
        IMG_GLOBAL_QUESTION,
        WARNING_TONE);

    SetLeftSoftkeyFunction(ebr_remove_selected_bookmark, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_goto_book_mark(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    S16 no_of_book_marks = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(!ebr_frm_check_file_exist())
	{
		ebr_app_display_pop_up_error_opening_book();
		go_back_to_bookshelf_screen();
		return;
	}
    no_of_book_marks = ebr_frm_get_count_of_book_marks();

    EntryNewScreen(EBOOK_GOTO_BOOKMARK_SCREENID, NULL, ebr_entry_goto_book_mark, NULL);

    guiBuffer = GetCurrGuiBuffer(EBOOK_GOTO_BOOKMARK_SCREENID);
    RegisterHighlightHandler(ebr_highlight_selected_bookmark);
    SetParentHandler(0);
    if (no_of_book_marks == 0)
    {   /* ShowCategory184Screen(STR_GOTO_BOOKMARK_EBOOK, (U16)NULL,
           0, 0,
           STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
           no_of_book_marks ,ebr_get_book_mark_name_list,
           NULL, 0, guiBuffer); */
        DisplayPopup(
            (PU8) GetString(STR_NO_BOOK_MARK_PRESENT),
            IMG_GLOBAL_WARNING,
            1,
            EBR_ST_NOTIFYDURATION,
            EMPTY_LIST_TONE);
        DeleteNScrId(EBOOK_GOTO_BOOKMARK_SCREENID);
        return;

    }
    else
    {
        ShowCategory184Screen(
            STR_GOTO_BOOKMARK_EBOOK,
            GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
            STR_GLOBAL_OK,
            IMG_GLOBAL_OK,
            STR_GLOBAL_BACK,
            IMG_GLOBAL_BACK,
            no_of_book_marks,
            ebr_get_book_mark_name_list,
            NULL,
            0,
            guiBuffer);
    }
    /* register LSK and right key handlers */
    SetLeftSoftkeyFunction(ebr_goto_selected_bookmark, KEY_EVENT_UP);
    SetKeyHandler(ebr_goto_selected_bookmark, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_selected_bookmark(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ebook_app_cntx_p->current_selected_bookmark = index;
    SetKeyHandler(ebr_goto_selected_bookmark, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_goto_selected_bookmark(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(!ebr_frm_check_file_exist())
	{
		ebr_app_display_pop_up_error_opening_book();
		go_back_to_bookshelf_screen();
		return;
	}
    ebr_frm_get_record_for_bookmark(g_ebook_app_cntx_p->current_selected_bookmark);
    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
    GoBackHistory();
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_remove_selected_bookmark(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_frm_remove_bookmark(GetCurrHiliteID());
    if (!ebr_frm_get_count_of_book_marks())
	{
    if (IsScreenPresent(EBOOK_BOOK_SETTINGS_SCREENID))
    {
        DeleteUptoScrID(EBOOK_BOOK_SETTINGS_SCREENID);
    }
    else
    {
        DeleteNScrId(EBOOK_REMOVE_BOOKMARK_SCREENID);
    }
    }

    DisplayPopup((PU8) GetString(STR_DELETED), IMG_GLOBAL_ACTIVATED, 1, EBR_ST_NOTIFYDURATION, SUCCESS_TONE);
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_add_book_mark(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_entry_add_book_mark, KEY_EVENT_UP);
    SetKeyHandler(ebr_entry_add_book_mark, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_add_book_mark(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer = NULL;
    S16 no_of_book_marks = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(!ebr_frm_check_file_exist())
	{
		ebr_app_display_pop_up_error_opening_book();
		go_back_to_bookshelf_screen();
		return;
	}
    no_of_book_marks = ebr_frm_get_count_of_book_marks();
    if (no_of_book_marks >= EBR_FRM_MAX_NO_OF_BOOKMARKS)
    {
         
        if (IsScreenPresent(EBOOK_BOOK_SETTINGS_SCREENID))
        {
            DeleteUptoScrID(EBOOK_BOOK_SETTINGS_SCREENID);
        }
        DisplayPopup((PU8) GetString(STR_MAX_BOOK_MARK), IMG_GLOBAL_WARNING, 1, EBR_ST_NOTIFYDURATION, WARNING_TONE);
        return;

    }
    EntryNewScreen(EBOOK_ADD_BOOK_MARK_SCREEN_ID, NULL, ebr_entry_add_book_mark, NULL);
    guiBuffer = GetCurrGuiBuffer(EBOOK_ADD_BOOK_MARK_SCREEN_ID);
    if (guiBuffer == NULL)
    {
        ebr_frm_get_default_name_of_book_marks((S8*)g_ebook_app_cntx_p->book_mark_name);
    }

    ShowCategory5Screen(
        STR_ADD_BOOKMARK_EBOOK,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        STR_GLOBAL_OPTIONS,
        IMG_GLOBAL_OPTIONS,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
        g_ebook_app_cntx_p->book_mark_name,
        EBR_FRM_MAX_BOOKMARK_NAME_SIZE + 1,
        guiBuffer);
    
#if defined(__MMI_GB__)
    GBSetInputboxLSKFunction(ebr_entry_book_mark_edit_list); 
#endif

    SetLeftSoftkeyFunction(ebr_entry_book_mark_edit_list, KEY_EVENT_UP);
    SetCategory5RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_book_mark_edit_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U16 ItemList[2];
    U16 nItems;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_EDIT_BOOK_MARK_NAME, NULL, ebr_entry_book_mark_edit_list, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_EDIT_BOOK_MARK_NAME);

    nItems = GetNumOfChild(MENU_EBOOK_EDIT_BOOK_MARK_NAME);
    GetSequenceStringIds(MENU_EBOOK_EDIT_BOOK_MARK_NAME, ItemList);

    RegisterHighlightHandler(ebr_get_add_book_mark_options);

    ShowCategory52Screen(
        STR_GLOBAL_OPTIONS,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nItems,
        ItemList,
        (PU16) gIndexIconsImageList,
        0,
        0,
        0,
        guiBuffer);

    /* Register function for left soft key */
    SetLeftSoftkeyFunction(ebr_get_add_book_mark, KEY_EVENT_UP);
    /* Register function for right soft key */
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 *  Processing for Highlight Handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_get_add_book_mark(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (g_ebook_app_cntx_p->current_highlighted_item)
    {
        case 0:
            ebr_frm_add_book_mark_pre_entry();
            break;
        case 1: /* Input Methods */
            EntryInputMethodScreen();
            RegisterInputMethodScreenCloseFunction(GoBackHistoryFromInputMethod);
            break;
        default:
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 *  Highlight Handler
 * PARAMETERS
 *  nIndex      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_get_add_book_mark_options(S32 nIndex)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ebook_app_cntx_p->current_highlighted_item = nIndex;
    switch (nIndex)
    {
        case 0: /* Done */
            ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
            break;
        case 1: /* Input Methods */
            SetKeyHandler(ebr_get_add_book_mark, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
            break;
        default:
            break;

    }
}


/*****************************************************************************
 * FUNCTION
 *  GoBackHistoryFromInputMethod
 * DESCRIPTION
 *  Go back History
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void GoBackHistoryFromInputMethod(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GoBacknHistory(1);
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  S8*
 *****************************************************************************/
S8 *ebr_get_book_mark_name(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (S8*) g_ebook_app_cntx_p->book_mark_name;
}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_book_settings
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_book_settings(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_entry_settings, KEY_EVENT_UP);
    SetKeyHandler(ebr_entry_settings, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    g_ebook_app_cntx_p->ebr_set_local_setting_flag = 1;
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_open_book_info
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_open_book_info(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_entry_book_info, KEY_EVENT_UP);
    SetKeyHandler(ebr_entry_book_info, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 *  ebr_close_book
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_close_book(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_entry_close_book, KEY_EVENT_UP);
    SetKeyHandler(ebr_entry_close_book, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_case_in_sensitive_serach
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_case_in_sensitive_serach(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_ebook_search_string, KEY_EVENT_UP);
    SetKeyHandler(ebr_ebook_search_string, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_case_sensitive_serach
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_case_sensitive_serach(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
    SetLeftSoftkeyFunction(ebr_ebook_search_string, KEY_EVENT_UP);
    SetKeyHandler(ebr_ebook_search_string, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_jump_to
 * DESCRIPTION
 *  Entry Function for jumpTo
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_jump_to(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer = NULL;
    U8 *inputBuffer;
    U16 nNumofItem = 4; /* Stores no of children in the submenu */
    U16 inputBufferSize;
    S16 item_index = 0;
    S32 book_size;
    S32 page_size;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    book_size = ebr_frm_get_book_temp_size_from_nfo();
    page_size = ebr_frm_get_page_size();
    if (book_size <= page_size)
    {
        DisplayPopup((PU8) GetString(STR_BOOK_TOO_SMALL), IMG_GLOBAL_WARNING, 1, EBR_ST_NOTIFYDURATION, WARNING_TONE);
        return;
    }
    EntryNewScreen(EBOOK_JUMPTO_SCREEN_ID, ebr_exit_jump_to, NULL, NULL);
    guiBuffer = GetCurrGuiBuffer(EBOOK_JUMPTO_SCREEN_ID);
    inputBuffer = GetCurrNInputBuffer(EBOOK_JUMPTO_SCREEN_ID, &inputBufferSize);        /* added for inline edit history */

    InitializeCategory57Screen();

    g_ebook_inline_const_string_context_p->ebook_jump_text_end_beg[EBR_JUMP_BEGIN] = GetString(STR_JMP_BEG_EBOOK);
    g_ebook_inline_const_string_context_p->ebook_jump_text_end_beg[EBR_JUMP_MIDDLE] = GetString(STR_JMP_MID_EBOOK);
    g_ebook_inline_const_string_context_p->ebook_jump_text_end_beg[EBR_JUMP_END] = GetString(STR_JMP_END_EBOOK);

    SetInlineItemActivation(&wgui_inline_items[item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&wgui_inline_items[item_index], (PU8) (U8*) GetString(STR_JUMP_EBOOK));

    SetInlineItemActivation(&wgui_inline_items[++item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(
        &wgui_inline_items[item_index],
        3,
        (U8**)g_ebook_inline_const_string_context_p->ebook_jump_text_end_beg,
        (S32*) & (g_ebookset_cntx_p->inlineHighligtItem) /* CurrSelect */ );

    SetInlineItemActivation(&wgui_inline_items[++item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&wgui_inline_items[item_index], (PU8) (U8*) GetString(STR_JMP_PERCENTAGE_EBOOK));

    SetInlineItemActivation(&wgui_inline_items[++item_index], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemDisplayOnly(&(wgui_inline_items[item_index]), (U8*) "1");

    RegisterHighlightHandler(ebr_highlight_jump_to_item);

    if (inputBuffer != NULL)
    {
        SetCategory57Data(wgui_inline_items, nNumofItem, inputBuffer);
    }

    if (GetInlineDoneFlag(guiBuffer) == 0 && g_ebook_app_cntx_p->percentage_val == '\0')
    {
        SetInlineDoneFlag(guiBuffer);
    }
    ShowCategory57Screen(
        STR_JUMP_EBOOK,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nNumofItem,
        NULL,
        wgui_inline_items,
        0,
        guiBuffer);

    /* Register function for right soft key */
    SetCategory57RightSoftkeyFunctions(ebr_entry_jump_to_done_hndlr, GoBackHistory);

}


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_jump_to_item
 * DESCRIPTION
 *  Changes the Left soft key options in case of Percentage
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_jump_to_item(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (index == 3)
    {
        ChangeLeftSoftkey(STR_GLOBAL_EDIT, 0);
        SetLeftSoftkeyFunction(ebr_entry_percentage_screen, KEY_EVENT_UP);
    }
}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_percentage_screen
 * DESCRIPTION
 *  Entry Function for Percentage Screen.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_percentage_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_EBOOK_ENTER_PERCENTAGE_SCREEN_ID, NULL, ebr_entry_percentage_screen, NULL);
    guiBuffer = GetCurrGuiBuffer(SCR_EBOOK_ENTER_PERCENTAGE_SCREEN_ID);
    SetParentHandler(0);

    ShowCategory5Screen(
        STR_PERCENTAGE_SCREEN,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        (U16) STR_GLOBAL_OK,
        (U16) IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        INPUT_TYPE_NUMERIC,
        (U8*) g_ebook_app_cntx_p->percentage_val,
        (S32) EBR_MAX_EBOOK_PERCENTAGE_VALUE + 1,
        guiBuffer);

 #if defined(__MMI_GB__)
    GBSetInputboxLSKFunction(ebr_go_back_to_inline_edit_screen); 
#endif

    SetLeftSoftkeyFunction(ebr_go_back_to_inline_edit_screen, KEY_EVENT_UP);
    SetCategory5RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_go_back_to_inline_edit_screen
 * DESCRIPTION
 *  Screen for the percentage editor.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_go_back_to_inline_edit_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 percentage_number_buffer[4] = {0};
    S16 percentage_val = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    UnicodeToAnsii((S8*) percentage_number_buffer, (S8*) g_ebook_app_cntx_p->percentage_val);
    percentage_val = atoi((char const*)percentage_number_buffer);
    if ((percentage_val > 0) && (percentage_val <= 100))
    {

    }
    else
    {
        /* DeleteNScrId(EBOOK_JUMPTO_DONE_SCREEN_ID); */
        DisplayPopup(
            (PU8) GetString(STR_WRONG_PERCENTAGE_VALUE),
            IMG_GLOBAL_ERROR,
            1,
            EBR_ST_NOTIFYDURATION,
            ERROR_TONE);
        return;
    }

    ebr_frm_jump_to_record(percentage_val, g_ebookset_cntx_p->inlineHighligtItem);

    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
    GoBackHistory();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_exit_jump_to
 * DESCRIPTION
 *  Exit function for Jump To
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_exit_jump_to(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    history_t Scr;
    S16 nHistory = 0;
    U16 size;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CloseCategory57Screen();
    Scr.scrnID = EBOOK_JUMPTO_SCREEN_ID;
    CloseCategory57Screen();
    Scr.entryFuncPtr = ebr_entry_jump_to;
    pfnUnicodeStrcpy((S8*) Scr.inputBuffer, (S8*) & nHistory);
    GetCategoryHistory(Scr.guiBuffer);
    size = (U16) GetCategory57DataSize();
    GetCategory57Data((U8*) Scr.inputBuffer);
    AddNHistory(Scr, size);
}


void ebr_book_jump_confirm(void)
{
    S16 percentage_val = 0;
    ebr_frm_jump_to_record(percentage_val, g_ebookset_cntx_p->inlineHighligtItem);

    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
    GoBackHistory();

}

void ebr_book_jump_cancel(void)
{
    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
    GoBackHistory();

}

/*****************************************************************************
 * FUNCTION
 *  ebr_entry_jump_to_done_hndlr
 * DESCRIPTION
 *  Entry Function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_jump_to_done_hndlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Call Exit Handler */

    if (IsScreenPresent(SCR_FMGR_APP_FILE_OPTION))
    {
        EntryNewScreen(EBOOK_JUMPTO_DONE_SCREEN_ID, NULL, NULL, NULL);
    }
    DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        get_string(STR_EBOOK_CONFIRM_JUMP),
        IMG_GLOBAL_QUESTION,
        WARNING_TONE);


    SetLeftSoftkeyFunction(ebr_book_jump_confirm, KEY_EVENT_UP);
    SetRightSoftkeyFunction(ebr_book_jump_cancel, KEY_EVENT_UP);
    
    /*ebr_frm_jump_to_record(percentage_val, g_ebookset_cntx_p->inlineHighligtItem);

    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
    GoBackHistory();*/
}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_close_book
 * DESCRIPTION
 *  Entry Function for Close
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_close_book(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    pBOOL is_src_app_is_file_manager;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_frm_rwrite_into_nvram();
    ebr_restore_file_read_only(ebr_fmgr_get_file_name());
    is_src_app_is_file_manager = ebr_frm_ebook_exit(EBR_FRM_EXIT_BY_CLOSE_BOOK);
    if (is_src_app_is_file_manager)
    {
        /* FMGR_Revise */
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif            
        if (IsScreenPresent(EBOOK_OPEN_SCREENID))
        {
            DeleteScreenIfPresent(EBOOK_OPEN_SCREENID);
        }
        GoBackHistory();
    }
    else
    {
        /* FMGR_Revise */
        if (IsScreenPresent(g_ebook_explorer_id))
        {
            DeleteUptoScrID(g_ebook_explorer_id);
            GoBackHistory();
        }
#if 0        
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif        
    }

}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_search
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_search(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U16 nNumofItem;
    U16 nStrItemList[MAX_SUB_MENUS];
    U16 nImgIltemList[MAX_SUB_MENUS];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(EBOOK_SEARCH_SCREENID, NULL, ebr_entry_search, NULL);
    guiBuffer = GetCurrGuiBuffer(EBOOK_SEARCH_SCREENID);

    nNumofItem = GetNumOfChild(MENU_ID_EBK_FIND);
    GetSequenceStringIds(MENU_ID_EBK_FIND, nStrItemList);
    GetSequenceImageIds(MENU_ID_EBK_FIND, nImgIltemList);
    SetParentHandler(MENU_ID_EBK_FIND);

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    ShowCategory52Screen(
        STR_FIND_EBOOK,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        STR_GLOBAL_OPTIONS,
        IMG_GLOBAL_OPTIONS,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nNumofItem,
        nStrItemList,
        (U16*) gIndexIconsImageList,
        0,
        0,
        0,
        guiBuffer);

    SetLeftSoftkeyFunction(ebr_ebook_search_string, KEY_EVENT_UP);
    SetKeyHandler(ebr_ebook_search_string, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(go_back_to_settings_screen, KEY_EVENT_UP);
    SetKeyHandler(go_back_to_settings_screen, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 *  go_back_to_settings_screen
 * DESCRIPTION
 *  Go back to Setting screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void go_back_to_settings_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DeleteUptoScrID(EBOOK_BOOK_SETTINGS);
    GoBackHistory();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_ebook_search_string
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_ebook_search_string(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(EBOOK_SEARCH_STR_SCREENID, ebr_exit_ebook_search_string, ebr_ebook_search_string, NULL);
    guiBuffer = GetCurrGuiBuffer(EBOOK_SEARCH_STR_SCREENID);
    
    ShowCategory5Screen(
        STR_FIND_EBOOK,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        STR_GLOBAL_OPTIONS,
        IMG_GLOBAL_OPTIONS,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
        (U8*) g_ebook_app_cntx_p->ebr_search_string_input,
        EBR_MAX_STR_SEARCH_LENGTH + 1,
        guiBuffer);
    
 #if defined(__MMI_GB__)
    GBSetInputboxLSKFunction(ebr_entry_search_edit_list); 
#endif

    SetLeftSoftkeyFunction(ebr_entry_search_edit_list, KEY_EVENT_UP);
    SetCategory5RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  ebr_exit_ebook_search_string
 * DESCRIPTION
 *  exit handler of input search string screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_exit_ebook_search_string(void)
{
	
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/       
    S8* last_search_string;

	/*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_frm_get_last_search_string(&last_search_string);
    if (pfnUnicodeStrcmp(last_search_string, g_ebook_app_cntx_p->ebr_search_string_input) != 0)
    {
        pfnUnicodeStrncpy(last_search_string, g_ebook_app_cntx_p->ebr_search_string_input, (EBR_MAX_STR_SEARCH_LENGTH + 1) *ENCODING_LENGTH);
    }
    
}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_search_edit_list
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_search_edit_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U16 ItemList[2];
    U16 nItems;

#ifdef __MMI_INDIC_ALG__
    pBOOL full_character;
#endif 

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_INDIC_ALG__
    full_character = ebr_frm_check_for_full_characters((S16*) g_ebook_app_cntx_p->ebr_search_string_input);
    if (!full_character)
    {
        ebr_app_show_string_incomplete_pop_up();
        return;
    }
#endif /* __MMI_INDIC_ALG__ */ 
    EntryNewScreen(SCR_EDIT_BOOK_MARK_NAME, NULL, ebr_entry_search_edit_list, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_EDIT_BOOK_MARK_NAME);

    nItems = GetNumOfChild(MENU_EBOOK_EDIT_BOOK_MARK_NAME);
    GetSequenceStringIds(MENU_EBOOK_EDIT_BOOK_MARK_NAME, ItemList);

    RegisterHighlightHandler(ebr_get_add_search_string_options);

    ShowCategory52Screen(
        STR_GLOBAL_OPTIONS,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nItems,
        ItemList,
        (PU16) gIndexIconsImageList,
        0,
        0,
        0,
        guiBuffer);

    /* Register function for left soft key */
    SetLeftSoftkeyFunction(ebr_get_add_search_string, KEY_EVENT_UP);
    /* Register function for right soft key */
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_get_add_search_string
 * DESCRIPTION
 *  Processing for Highlight Handler
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_get_add_search_string(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (g_ebook_app_cntx_p->current_highlighted_item)
    {
        case 0:
            ebr_app_search_string();
            break;
        case 1: /* Input Methods */
            EntryInputMethodScreen();
            RegisterInputMethodScreenCloseFunction(GoBackHistoryFromInputMethod);
            break;
        default:
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  ebr_get_add_search_string_options
 * DESCRIPTION
 *  Highlight Handler
 * PARAMETERS
 *  nIndex      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_get_add_search_string_options(S32 nIndex)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ebook_app_cntx_p->current_highlighted_item = nIndex;
    switch (nIndex)
    {
        case 0: /* Done */
            ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
            break;
        case 1: /* Input Methods */
            SetKeyHandler(ebr_get_add_search_string, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
            break;
        default:
            break;

    }
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_search_string
 * DESCRIPTION
 *  Function for Search
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_search_string(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ebr_app_search_type_enum search_type = GetCurrHiliteID();
    S32 search_satus;

    S32 str_length = pfnUnicodeStrlen((const S8*)g_ebook_app_cntx_p->ebr_search_string_input);
    if(!ebr_frm_check_file_exist())
    {
    	ebr_app_display_pop_up_error_opening_book();
    	go_back_to_bookshelf_screen();
    	return;
    }
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (str_length == 0)
    {
        ebr_frm_search_string_empty();
        return;
    }
    ebr_please_wait_actual_screen_2();
    ebr_frm_reset_find_next_data();
    search_satus = ebr_frm_search_string_algo(search_type, (S8*) g_ebook_app_cntx_p->ebr_search_string_input, MMI_FALSE);
    /* search not complete */
    if (search_satus == 0xFF)
    {
        return;
    }
    else if ((search_satus ==1) &&
        ((g_ebook_app_cntx_p->ebr_search_string_input[0] != '\0') || (g_ebook_app_cntx_p->ebr_search_string_input[1] != '\0' )))
    {
        if (!ebr_frm_get_auto_scroll_status())
        {
            ebr_frm_reset_auto_scroll();
        }
        ebr_app_search_result_successful((pBOOL) search_type);
        ebr_app_delete_upto_open_book_screen_id();
        GoBackHistory();
        ebr_frm_set_data_for_display();
    }
    else
    {
	if(search_satus == 2)
	{
            ebr_app_search_result_invalid();
	}
	else
	{
            ebr_app_search_result_unsucessfull();
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_save_settings_scroll
 * DESCRIPTION
 *  Save scroll flag in nfo file
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_save_settings_scroll(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_app_delete_upto_open_book_screen_id();
    ebr_app_display_pop_up_changed_auto_scroll(ebr_frm_save_settings_scroll());
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_save_settings_screen_mode
 * DESCRIPTION
 *  Saves screen mode in NFO file.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_save_settings_screen_mode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_app_delete_upto_open_book_screen_id();
    ebr_app_display_pop_up_changed_screen_mode(ebr_frm_save_settings_screen_mode());
}


void ebr_app_save_settings_fullscreen_to_normal(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_app_delete_upto_open_book_screen_id();
    ebr_frm_save_settings_screen_mode();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_delete_all_ebook_relavant_screen
 * DESCRIPTION
 *  Delete all the ebook screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_delete_all_ebook_relavant_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	DeleteScreenIfPresent(EBOOK_OPEN_SCREENID);
	DeleteScreenIfPresent(EBOOK_INFO_SCREENID);
	DeleteScreenIfPresent(EBOOK_OPEN_OPTION_SCREENID);
	DeleteScreenIfPresent(EBOOK_SEARCH_SCREENID);
	DeleteScreenIfPresent(EBOOK_BOOK_SETTINGS_SCREENID);
	DeleteScreenIfPresent(EBOOK_SEARCH_STR_SCREENID);
	DeleteScreenIfPresent(EBOOK_JUMPTO_SCREENID);
	DeleteScreenIfPresent(EBOOK_SETTINGS_SCREEID);
	DeleteScreenIfPresent(EBOOK_BOOK_SETTINGS_SCREENID);
    DeleteScreenIfPresent(EBOOK_REMOVE_BOOKMARK_SCREENID);
	DeleteScreenIfPresent(EBOOK_GOTO_BOOKMARK_SCREENID);
	DeleteScreenIfPresent(EBOOK_ADD_BOOK_MARK_SCREEN_ID);
	DeleteScreenIfPresent(EBOOK_JUMPTO_SCREEN_ID);
	DeleteScreenIfPresent(EBOOK_JUMPTO_DONE_SCREEN_ID);
	DeleteScreenIfPresent(EBOOK_BOOK_SHELF_SETTINGS_SCREEN_ID);
	DeleteScreenIfPresent(SCR_ID_EBOOK_FILE_MOVE_WAITSCREEN);
	DeleteScreenIfPresent(SCR_EBOOK_ENTER_PERCENTAGE_SCREEN_ID);
	DeleteScreenIfPresent(SCR_EDIT_BOOK_MARK_NAME);

#ifdef __MMI_BPP_SUPPORT__  /*BPP revise*/
    DeleteScreenIfPresent(SCR_BT_MY_SPECIFIC_DEV_DUMMY);
    DeleteScreenIfPresent(SCR_BT_POWER_ON_CNF);
#endif /* __MMI_BPP_SUPPORT__ */

    DeleteScreenIfPresent(SCR_SYMBOL_PICKER_SCREEN);
    DeleteScreenIfPresent(SCR_INPUT_METHOD_LIST);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_open_book
 * DESCRIPTION
 *  Entry Function for Open Book
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern pBOOL isInCall(void);
void ebr_entry_open_book(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer = NULL;
    S8 *last_search_string;
    S16 buffer_size;
    U16 lsk=STR_GLOBAL_OPTIONS;
    U16 display_name[(MAX_EBOOK_NAME_LENGTH + 1) * ENCODING_LENGTH];
    S16 return_val;
    S32 book_size;
    S32 page_size;
    S16 full_screen, auto_scroll_staus;
    U16 screenID;

    ebr_frm_nfo_create_type_errors_enum error_type = EBR_FRM_NFO_CREATE_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_Ebr, "[EBOOK] func:%s, state = %d", __FUNCTION__, ebr_frm_get_current_recursive_conversion_state());
#ifdef __MMI_EBOOK_SPEED_OPEN__
    if(ebr_frm_get_current_recursive_conversion_state() != EBR_FRM_CONVERSION_BACKGROUND)
#endif
    ebr_please_wait_actual_screen();
    
    PowerAndEndKeyHandler();
    if (ebr_frm_get_current_recursive_conversion_state() == EBR_FRM_CONVERSION_UNDERWAY)
    {
        ebr_frm_convert_record_record_recursively();
        return;
    }
    if (ebr_frm_get_current_recursive_conversion_state() == EBR_FRM_NFO_CONVERSION_COMPLETED)
    {
        DeleteNScrId(EBOOK_OPEN_SCREENID);
        ebr_frm_set_current_recursive_conversion_state(EBR_FRM_NO_CONVERSION);
    }
    guiBuffer = GetCurrGuiBuffer(EBOOK_OPEN_SCREENID);
    if (guiBuffer == NULL)
    {
        error_type = ebr_frm_reset_for_new_book();
        mmi_trace(g_sw_Ebr, "[EBOOK] func:%s, error_type = %d", __FUNCTION__, error_type);

        g_ebook_app_cntx_p->enable_screen_exit = 1;
        if ((ebr_frm_nfo_create_type_errors_enum) error_type == EBR_FRM_NFO_CREATE_FAILURE)
        {
            if (ebr_frm_check_nfo_file_size())
            {
                ebr_app_dispaly_less_memory_pop_up();
            }
            else
            {
                ebr_app_display_pop_up_error_opening_book();
            }
            DeleteNScrId(EBOOK_OPEN_SCREENID);
            ebr_frm_rwrite_into_nvram();
            return;
        }
        else
        {
            if ((ebr_frm_nfo_create_type_errors_enum) error_type == EBR_FRM_NFO_CREATE_FILE_TYPE_NOT_SUPPORTED)
            {
                ebr_app_display_pop_up_format_not_supported();
                DeleteNScrId(EBOOK_OPEN_SCREENID);
                ebr_frm_rwrite_into_nvram();
                return;
            }
        }
        if ((ebr_frm_nfo_create_type_errors_enum) error_type == EBR_FRM_EMPTY_BOOK)
        {
            ebr_app_display_pop_up_empty_book();
            DeleteNScrId(EBOOK_OPEN_SCREENID);
            ebr_frm_rwrite_into_nvram();
            return;
        }
    }
    memset(g_ebook_app_cntx_p->buffer_data, 0, EBR_RECORD_SIZE_MAX + GUI_SHARED_BYTES);
    ebr_frm_register_recursive_callback_fn(ebr_entry_open_book);
    return_val = ebr_frm_get_data_to_open_book(ebook_frm_setting, g_ebook_app_cntx_p->buffer_data, (PS8)display_name);
    mmi_trace(g_sw_Ebr, "[EBOOK] func:%s, return_val = %d", __FUNCTION__, return_val);

    if (return_val == 0)
    {
        ebr_app_dispaly_less_memory_pop_up();
        DeleteNScrId(EBOOK_OPEN_SCREENID);
        ebr_frm_rwrite_into_nvram();
        return;
    }
    else
    {
        if (return_val == -1)
        {
            ebr_app_display_pop_up_error_opening_book();
            DeleteNScrId(EBOOK_OPEN_SCREENID);
            ebr_frm_rwrite_into_nvram();
            return;
        }
    }
    if (return_val == 3)
    {
        ebr_app_display_pop_up_error_opening_book();
        DeleteNScrId(EBOOK_OPEN_SCREENID);
        ebr_frm_rwrite_into_nvram();
        return;
    }
    if (return_val == 2)
    {
        ebr_app_display_pop_up_empty_book();
        DeleteNScrId(EBOOK_OPEN_SCREENID);
        ebr_frm_rwrite_into_nvram();
        return;
    }
    else if (return_val < 0)
    {
        ebr_display_error_pop_up((S32)return_val);
        /* FMGR_Revise */
        if(IsScreenPresent(g_ebook_explorer_id))
        {
            DeleteUptoScrID(g_ebook_explorer_id);
//        DeleteUptoScrID(SCR_FMGR_EXPLORER);
        }
        ebr_frm_rwrite_into_nvram();
        return;
    }
    if (ebr_frm_get_current_recursive_conversion_state() == EBR_FRM_CONVERSION_UNDERWAY)
    {
        return;
    }
    buffer_size = pfnUnicodeStrlen((const S8*)g_ebook_app_cntx_p->buffer_data);
    full_screen = ebr_frm_get_full_screen_flag_status();
    auto_scroll_staus = ebr_frm_get_auto_scroll_flag_status();
    if (auto_scroll_staus)
    {
        TurnOnBacklight(0);
    }
    screenID = GetExitScrnID();

    //ebr_set_file_read_only(ebr_fmgr_get_file_name());   

    EntryNewScreen(EBOOK_OPEN_SCREENID, ebr_mmi_ebook_exit, ebr_entry_open_book, NULL);
#ifdef __MMI_TOUCH_SCREEN__
    if (!isInCall())
    {
        ebook_frm_setting->key_callback = ebr_set_key_TS;
    }
#endif 
    if (isInCall())
        lsk = 0;
    if (full_screen == 0)
    {
#ifndef __MMI_MY_FAVORITE__
        ShowCategory75Screen(
            display_name,
            GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
            lsk,
            IMG_GLOBAL_OK,
            STR_GLOBAL_BACK,
            IMG_GLOBAL_BACK,
            (U8*) g_ebook_app_cntx_p->buffer_data,
            buffer_size,
            NULL,
            &ebook_frm_setting);
#else
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

        SetLeftSoftkeyFunction(ebr_entry_book_settings, KEY_EVENT_UP);
        SetRightSoftkeyFunction(go_back_to_bookshelf_screen_entry, KEY_EVENT_UP);
    }
    else
    {
#ifndef __MMI_MY_FAVORITE__
        ShowCategory77Screen(
            (U16*) display_name,
            GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
            0,
            IMG_GLOBAL_OK,
            0,
            IMG_GLOBAL_BACK,
            (U8*) g_ebook_app_cntx_p->buffer_data,
            buffer_size,
            NULL,
            &ebook_frm_setting);
#else
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
#endif
        if (!isInCall())
        {
            SetKeyHandler(ebr_entry_book_settings, KEY_LSK, KEY_EVENT_UP);
        }
        else
        {
            SetKeyHandler(NULL, KEY_LSK, KEY_EVENT_UP);
        }
	#if defined(__MMI_MAINLCD_220X176__)
	 SetKeyHandler(ebr_app_save_settings_screen_mode, KEY_RSK, KEY_EVENT_UP);
	#else	
        SetKeyHandler(go_back_to_bookshelf_screen_entry, KEY_RSK, KEY_EVENT_UP);
	#endif	
    }
    /*    PowerAndEndKeyHandler(); */
    register_multiline_inputbox_navigate_callback(ebr_frm_reset_find_next_data);
    if (ebook_frm_setting->search_flag_is_on)
    {
        ebr_frm_get_and_set_current_page_values();
    }
    g_ebook_app_cntx_p->ebr_set_local_setting_flag = 1;
    book_size = ebr_frm_get_book_size_from_nfo();
    page_size = ebr_frm_get_page_size();
    if (!isInCall())
    {
        SetKeyHandler(ebr_entry_settings, KEY_0, KEY_EVENT_UP);
        SetKeyHandler(ebr_entry_help, KEY_1, KEY_EVENT_UP);
        SetKeyHandler(ebr_entry_add_book_mark, KEY_2, KEY_EVENT_UP);
        SetKeyHandler(ebr_entry_search, KEY_3, KEY_EVENT_UP);
        SetKeyHandler(ebr_pre_entry_jump_to, KEY_4, KEY_EVENT_UP);
        SetKeyHandler(ebr_entry_goto_book_mark, KEY_5, KEY_EVENT_UP);
        ebr_frm_get_last_search_string(&last_search_string);
        if (((*last_search_string != '\0') || (*(last_search_string+1) != '\0')) && ebr_frm_get_auto_scroll_status())
        {
            SetKeyHandler(ebr_app_search_string_next, KEY_6, KEY_EVENT_UP);
        }
        SetKeyHandler(ebr_entry_remove_book_mark, KEY_8, KEY_EVENT_UP);
        SetKeyHandler(ebr_app_save_settings_scroll, KEY_7, KEY_EVENT_UP);       /* Auto Scroll */
        SetKeyHandler(ebr_app_save_settings_screen_mode, KEY_9, KEY_EVENT_UP);   /* Full Screen */
    }
    ebr_app_delete_all_ebook_relavant_screen();

}

extern void ShowCategory63Screen(U8 *message, U16 message_icon, U8 *history_buffer);

/*****************************************************************************
 * FUNCTION
 *  ebr_please_wait_actual_screen
 * DESCRIPTION
 *  Please Wait Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_please_wait_actual_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_ID_EBOOK_WAITSCREEN, NULL, NULL, NULL);
    ClearHighlightHandler();
#if 0
    ShowCategory63Screen(
    (U8*) GetString(STR_EBOOK_WAIT),
            IMAGE_EBOOK_WAIT,
            NULL);
#endif
	//ShowCategory151Screen(0, 0, 0, 0, 0, 0, (PU8)GetString(STR_EBOOK_WAIT), 0, NULL); 
    ShowCategory165Screen(0,0,0,0,
    			(UI_string_type)GetString(STR_EBOOK_WAIT),
        		IMAGE_EBOOK_WAIT,
        		NULL);
    ClearAllKeyHandler();
    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_file_move_wait_screen
 * DESCRIPTION
 *  Please Wait Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_file_move_wait_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_ID_EBOOK_FILE_MOVE_WAITSCREEN, exit_file_move_wait_screen, NULL, NULL);
    ClearHighlightHandler();

    ShowCategory66Screen(
        (U16) STR_EBOOK_WAIT,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        0,
        0,
        0,
        0,
        (U8*) GetString(STR_EBOOK_WAIT_FOR_MOVING_FILE),
        IMAGE_EBOOK_WAIT,
        NULL);
    ClearAllKeyHandler();
    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
}

extern void fmt_abort_copy (void);
void ebr_send_abort_coy_req(void)
{
	fmt_abort_copy();
}

/*****************************************************************************
 * FUNCTION
 *  exit_file_move_wait_screen
 * DESCRIPTION
 *  Please Wait Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void exit_file_move_wait_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    history_t wait_screen;
    U16 inputBufferSize;
    
    /*----------------------------------------------------------------*/
     /* Code Body                                                      */
     /*----------------------------------------------------------------*/

    if (/* GetActiveScreenId() != POPUP_SCREENID && */ ebr_frm_fmgr_is_moving_file() == MMI_TRUE)
    {
        //mmi_fmgr_send_abort_copy_req();        
        ebr_send_abort_coy_req(); 
    }
    else
    {
        wait_screen.scrnID = SCR_ID_EBOOK_FILE_MOVE_WAITSCREEN;
        wait_screen.entryFuncPtr = ebr_file_move_wait_screen;
        inputBufferSize = 0;
        AddNHistory(wait_screen, inputBufferSize);          /* added for inline edit history */  
    }
}

/*****************************************************************************
 * FUNCTION
 *  ebr_please_wait_actual_screen_2
 * DESCRIPTION
 *  Please Wait Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_please_wait_actual_screen_2(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_ID_EBOOK_WAITSCREEN, NULL, NULL, NULL);
    ClearHighlightHandler();

    ShowCategory63Screen(
    (U8*) GetString(STR_EBOOK_WAIT),
            IMAGE_EBOOK_WAIT,
            NULL);
    /* ShowCategory9Screen(wait_string, wait_image_id, NULL); */
    ClearAllKeyHandler();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_please_wait_move_screen
 * DESCRIPTION
 *  Please Wait Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_please_wait_move_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* EntryNewScreen(SCR_ID_EBOOK_WAITSCREEN, NULL, NULL, NULL); */
    ClearHighlightHandler();
    ClearKeyEvents();
    /* ShowCategory151Screen(0, 0, 0, 0, 0, 0, (PU8)GetString(STR_WAIT_STRING), 0, NULL); */
    ShowCategory66Screen(
        (U16) STR_EBOOK_WAIT,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        0,
        0,
        0,
        0,
        (U8*) GetString(STR_EBOOK_WAIT),
        IMAGE_EBOOK_WAIT,
        NULL);
    /* ShowCategory9Screen(wait_string, wait_image_id, NULL); */
    ClearAllKeyHandler();
    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  go_back_to_bookshelf_screen
 * DESCRIPTION
 *  Go to Book shelf Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void go_back_to_bookshelf_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_frm_close_the_ebook();
    DeleteScreenIfPresent(EBOOK_BOOK_SETTINGS_SCREENID);
	DeleteNScrId(EBOOK_OPEN_SCREENID);
}


/*****************************************************************************
* FUNCTION
*  go_back_to_bookshelf_screen_entry
* DESCRIPTION
*  Go to Book shelf Screen
* PARAMETERS
*  void
* RETURNS
*  void
*****************************************************************************/
void go_back_to_bookshelf_screen_entry(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_frm_close_the_ebook();
    DeleteNScrId(EBOOK_OPEN_SCREENID);
    GoBackHistory();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_frm_set_full_screen_on
 * DESCRIPTION
 *  Set full screen ON
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_frm_set_full_screen_on(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
    ebr_entry_open_book();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_frm_set_full_screen_off
 * DESCRIPTION
 *  Set full screen OFF
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_frm_set_full_screen_off(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
    ebr_entry_open_book();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_delete_global_setting_screen
 * DESCRIPTION
 *  Delete global setting screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_delete_global_setting_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    mmi_trace(MMI_COMMON_TRC_G1_EBOOK, TRC_MMI_EBOOK_DELETE_SETTING, g_ebook_app_cntx_p->ebr_set_local_setting_flag);

	if (!g_ebook_app_cntx_p->ebr_set_local_setting_flag)
        DeleteScreenIfPresent(EBOOK_BOOK_SHELF_SETTINGS_SCREEN_ID);

}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_book_settings
 * DESCRIPTION
 *  Entry Function for Setting Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_book_settings(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U16 nNumofItem;
    U16 nStrItemList[MAX_SUB_MENUS];
    U16 nImgIltemList[MAX_SUB_MENUS];
    U8 *hintList[MAX_SUB_MENUS];
    S32 book_size;
    S32 page_size;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_EBOOK_SPEED_OPEN__
    if(ebr_frm_get_current_recursive_conversion_state() == EBR_FRM_CONVERSION_BACKGROUND)
    {
	DisplayPopup((U8 *)get_string(STR_GLOBAL_PLEASE_WAIT), IMG_GLOBAL_PROGRESS, 0, 2000, 0);
    	return;
    }
#endif
    EntryNewScreen(EBOOK_BOOK_SETTINGS_SCREENID, NULL, ebr_entry_book_settings, NULL);
    guiBuffer = GetCurrGuiBuffer(EBOOK_BOOK_SETTINGS_SCREENID);
    book_size = ebr_frm_get_book_size_from_nfo();
    page_size = ebr_frm_get_page_size();
	/* The UCS2 encoding of Chinese word '¤@' is 00 4e */
#if defined(__MMI_BPP_SUPPORT__) && defined(__FLIGHT_MODE_SUPPORT__) /*BPP revise*/
    if (mmi_bootup_get_active_flight_mode() == 1) /* in flight mode */
    {
        mmi_frm_hide_menu_item(MENU_ID_EBK_BPP_PRINT);        
    }
    else
    {
        mmi_frm_unhide_menu_item(MENU_ID_EBK_BPP_PRINT);        
    }
#endif /*__MMI_BPP_SUPPORT__ && __FLIGHT_MODE_SUPPORT__*/
    if (pfnUnicodeStrcmp((S8 *)search_string_book_name, ebr_fmgr_get_file_name()))
    {
        memset(g_ebook_app_cntx_p->ebr_search_string_input, 0x00, (EBR_MAX_STR_SEARCH_LENGTH + 1) *ENCODING_LENGTH + ENCODING_LENGTH);
        memset(search_string_book_name, 0x00, sizeof(search_string_book_name));
        pfnUnicodeStrcpy((S8 *)search_string_book_name, ebr_fmgr_get_file_name());
    }
    if ((g_ebook_app_cntx_p->ebr_search_string_input[0] != '\0' || g_ebook_app_cntx_p->ebr_search_string_input[1] != '\0') 
		&& ebr_frm_get_auto_scroll_status())
    {
        SetParentHandler(MENU_ID_EBK_OPEN_OPTIONS_1);
        nNumofItem = GetNumOfChild_Ext(MENU_ID_EBK_OPEN_OPTIONS_1);
        GetSequenceStringIds_Ext(MENU_ID_EBK_OPEN_OPTIONS_1, nStrItemList);
        GetSequenceImageIds_Ext(MENU_ID_EBK_OPEN_OPTIONS_1, nImgIltemList);
        ConstructHintsList(MENU_ID_EBK_OPEN_OPTIONS_1, hintList);

    }
    else
    {
        SetParentHandler(MENU_ID_EBK_OPEN_OPTIONS);
        nNumofItem = GetNumOfChild_Ext(MENU_ID_EBK_OPEN_OPTIONS);
        GetSequenceStringIds_Ext(MENU_ID_EBK_OPEN_OPTIONS, nStrItemList);
        GetSequenceImageIds_Ext(MENU_ID_EBK_OPEN_OPTIONS, nImgIltemList);
        /* Construct List To Show Hint */
        ConstructHintsList(MENU_ID_EBK_OPEN_OPTIONS, hintList);

    }
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    ShowCategory52Screen(
        STR_GLOBAL_OPTIONS,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nNumofItem,
        nStrItemList,
        (U16*) gIndexIconsImageList,
        hintList,
        0,
        0,
        guiBuffer);
    SetRightSoftkeyFunction(go_back_to_settings_screen, KEY_EVENT_UP);
    SetKeyHandler(go_back_to_settings_screen, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_book_info
 * DESCRIPTION
 *  Entry Function for Book Info Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_book_info(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U8 book_size[EBR_DISPLAY_INFO_DATA];
    U8 new_line_info[EBR_NEW_LINE_SIZE];
    S32 file_size, total_size;
    U8 *type;
    S8 *ext = NULL;
    S8 *buffer = NULL;
    S8 new_line[]="\n";
    U8 type_info[EBR_DISPLAY_INFO_DATA * ENCODING_LENGTH];
    U8 *tmp_path_buf = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    tmp_path_buf = (U8 *)OslMalloc((MAX_DWNL_EBK_PATH_LEN + 1) * ENCODING_LENGTH);
    if(tmp_path_buf == NULL)
    {
        return ;
    }
    memset(tmp_path_buf, 0x00, ((MAX_DWNL_EBK_PATH_LEN + 1) * ENCODING_LENGTH));    

    buffer = (S8 *)OslMalloc((MAX_EBOOK_NAME_LENGTH + 1) * ENCODING_LENGTH);
    if(buffer == NULL)
    {
        return ;
    }
    memset(buffer, 0x00, ((MAX_EBOOK_NAME_LENGTH + 1) * ENCODING_LENGTH));  
    
    EntryNewScreen(EBOOK_INFO_SCREENID, NULL, ebr_entry_book_info, NULL);
    SetDelScrnIDCallbackHandler(EBOOK_INFO_SCREENID, (HistoryDelCBPtr)ebr_book_info_del_callback);
    guiBuffer = GetCurrGuiBuffer(EBOOK_INFO_SCREENID);
    type = ebr_frm_get_book_type();
    AnsiiToUnicodeString((S8*) new_line_info, (S8*) new_line);
    AnsiiToUnicodeString((S8*) type_info, (S8*) type);
    if (mmi_ebr_is_short_file_name())
        {
                memset(tmp_path_buf, 0, (FMGR_MAX_PATH_LEN + 1) * ENCODING_LENGTH);
                mmi_chset_mixed_text_to_ucs2_str(
                            (U8*) tmp_path_buf,
                            (FMGR_MAX_PATH_LEN + 1) * ENCODING_LENGTH,
                            (U8*) ebr_frm_get_book_name(),
                            g_chset_text_encoding);                           
        }
        else
        {
            pfnUnicodeStrncpy(
                (S8*) tmp_path_buf,
                (S8*) ebr_frm_get_book_name(),
                (FMGR_MAX_PATH_LEN + 1) * ENCODING_LENGTH);
        } 
	

    ext = mmi_fmgr_extract_ext_file_name((S8*)tmp_path_buf);
    if(ext)
    {
        ext -= 2;
        ext[0] = ext[1] = 0;
    }
    if (g_ebook_app_cntx_p->info_buffer == NULL)
    {
        g_ebook_app_cntx_p->info_buffer = OslMalloc(EBR_DISPLAY_INFO_SIZE);
    }
    pfnUnicodeStrcpy((PS8) g_ebook_app_cntx_p->info_buffer, (PS8) get_string(STR_EBOOK_BOOK_INFO_TITLE));
    pfnUnicodeStrcat((S8*) g_ebook_app_cntx_p->info_buffer, (S8*) L" ");
    pfnUnicodeStrcat((PS8) g_ebook_app_cntx_p->info_buffer, (PS8) tmp_path_buf);
    pfnUnicodeStrcat((PS8) g_ebook_app_cntx_p->info_buffer, (S8*) new_line_info);
    pfnUnicodeStrcat((PS8) g_ebook_app_cntx_p->info_buffer, (S8*) get_string(STR_EBOOK_BOOK_INFO_SIZE));
    pfnUnicodeStrcat((S8*) g_ebook_app_cntx_p->info_buffer, (S8*) L" ");
    total_size = ebr_frm_get_book_size();
    if (total_size > 1024 * 1024)
    {
        file_size = total_size / 1024;
        sprintf(buffer, "%d.%dM", (U16) (file_size / 1024), (U16) ((file_size % 1024) / 103));
    }
    else if (total_size > 1024)
    {
        sprintf(buffer, "%d.%dK", (U16) (total_size / 1024), (U16) ((total_size % 1024) / 103));
    }
    else    /* less than 1024 */
    {
        sprintf(buffer, "%dB", (U16) (total_size));
    }

    AnsiiToUnicodeString((S8*) book_size, (S8*) buffer);
    pfnUnicodeStrcat((PS8) g_ebook_app_cntx_p->info_buffer, (PS8) book_size);
    pfnUnicodeStrcat((PS8) g_ebook_app_cntx_p->info_buffer, (PS8) new_line_info);
    pfnUnicodeStrcat((PS8) g_ebook_app_cntx_p->info_buffer, (PS8) get_string(STR_EBOOK_BOOK_INFO_FORMAT));
    pfnUnicodeStrcat((S8*) g_ebook_app_cntx_p->info_buffer, (S8*) L" ");
    pfnUnicodeStrcat((PS8) g_ebook_app_cntx_p->info_buffer, (PS8) type_info);
    
    if(tmp_path_buf != NULL)
    {
        OslMfree(tmp_path_buf);
        tmp_path_buf = NULL;
    }   
    
    if(buffer != NULL)
    {
        OslMfree(buffer);
        buffer = NULL;
    }
    
    ShowCategory74Screen(
        STR_INFO_EBOOK,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        0,
        0,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        (U8*) g_ebook_app_cntx_p->info_buffer,
        pfnUnicodeStrlen((const S8*)g_ebook_app_cntx_p->info_buffer),
        guiBuffer);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
 * FUNCTION
 *  ebr_book_info_del_callback
 * DESCRIPTION
 *  Delete callback function of book info screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/    
U8 ebr_book_info_del_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_ebook_app_cntx_p->info_buffer)
    {
        OslMfree(g_ebook_app_cntx_p->info_buffer);
        g_ebook_app_cntx_p->info_buffer = NULL;
    }
    return MMI_FALSE;
}

/*****************************************************************************
 * FUNCTION
 *  ebr_entry_delete_ebook
 * DESCRIPTION
 *  Entry Function for Delete Book Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_delete_ebook(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	S32 return_type = 0;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return_type = ebr_frm_delete_ebook();
    
    if(return_type==-45)
    {
        DisplayPopup((PU8) GetString(FMGR_FS_READ_ONLY_ERROR_TEXT), IMG_GLOBAL_ERROR, 1, EBR_ST_NOTIFYDURATION, ERROR_TONE);
    }
    else if (return_type < 0)
    {
        DisplayPopup((PU8) GetString(GetFileSystemErrorString(return_type)), IMG_GLOBAL_ERROR, 1, EBR_ST_NOTIFYDURATION, ERROR_TONE);   
    }
    else
    {
        DisplayPopup((PU8) GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, EBR_ST_NOTIFYDURATION, SUCCESS_TONE);
    }
    /* FMGR_Revise */
//    DeleteNScrId(SCR_FMGR_APP_FILE_OPTION);
    //mmi_fmgr_close_option_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_help
 * DESCRIPTION
 *  Entry Function for Help Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_help(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    PS8 buffer_data;
    U8 *guiBuffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(EBOOK_HELP_SCREEN, NULL, ebr_entry_help, NULL);
    guiBuffer = GetCurrGuiBuffer(EBOOK_HELP_SCREEN);
    buffer_data = GetString(STR_EBOOK_TOUCH_SCREEN_HELP);

    ShowCategory74Screen(
        STR_HELP_EBOOK,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        0,
        0,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        (U8*) buffer_data,
        510,
        guiBuffer);

    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_settings
 * DESCRIPTION
 *  Entry Function for Settings Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_settings(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer = NULL;
    U8 *inputBuffer;
    U16 nNumofItem = EBR_TOTAL_NUMBER_OF_ITEM_BKSHLF;   /* Stores no of children in the submenu */
    U16 inputBufferSize;
    S16 phoneDrvLetter, cardDrvLetter;   
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    S16 seccardDrvLetter; 
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
    U16 gIndexIconsImageList_local[] = 
    {
        IMG_GLOBAL_L1,
        0,
        IMG_GLOBAL_L2,
        0,
        IMG_GLOBAL_L3,
        0,
        IMG_GLOBAL_L4,
        0,
        IMG_GLOBAL_L5,
        0,
        IMG_GLOBAL_L6,
        0,
        IMG_GLOBAL_L7,
        0,
        IMG_GLOBAL_L8,
        0,
        IMG_GLOBAL_L9,
        0
    };

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    
    nNumofItem = EBR_TOTAL_NUMBER_OF_ITEM_BKSHLF;

    //mmi_fmgr_get_exist_storage_inline_list(0, &status, NULL, NULL);
    phoneDrvLetter = MMI_FS_GetDrive( FS_DRIVE_V_NORMAL, 2, FS_DRIVE_I_SYSTEM | FS_DRIVE_V_NORMAL );
    cardDrvLetter = MMI_FS_GetDrive( FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE );  
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    seccardDrvLetter = MMI_FS_GetDrive( FS_DRIVE_TFCARD_SECOND, 1, FS_NO_ALT_DRIVE );  
#endif
/*- zhouqin modify for dual t-flash card 20110503*/

    if (g_ebook_app_cntx_p->ebr_set_local_setting_flag == 1 || 
        (g_ebook_app_cntx_p->ebr_set_local_setting_flag != 1 && !((phoneDrvLetter > 0) && (cardDrvLetter > 0)
                                                                      /*+ zhouqin modify for dual t-flash card 20110503*/
                                                                      #if defined(DUAL_TFLASH_SUPPORT)
                                                                      && (seccardDrvLetter > 0)
                                                                      #endif
                                                                      /*- zhouqin modify for dual t-flash card 20110503*/  )))
    {
        gIndexIconsImageList_local[16] = 0;
        gIndexIconsImageList_local[17] = 0;
        nNumofItem = EBR_TOTAL_NUMBER_OF_ITEM_BOOK;
    }                    

    EntryNewScreen(EBOOK_BOOK_SHELF_SETTINGS_SCREEN_ID, ebr_exit_settings, NULL, NULL);
    InitializeCategory57Screen();
        
    guiBuffer = GetCurrGuiBuffer(EBOOK_BOOK_SHELF_SETTINGS_SCREEN_ID);
    inputBuffer = GetCurrNInputBuffer(EBOOK_BOOK_SHELF_SETTINGS_SCREEN_ID, &inputBufferSize);   /* added for inline edit history */	
    if (inputBuffer != NULL)
    {
        SetCategory57Data(wgui_inline_items, nNumofItem, inputBuffer);		
    }    
    else
    {
        //ClearHighlightHandler();
		/* Ge the Default value from NVRAM */
        ebr_frm_get_default_settings_value(g_ebookset_cntx_p, g_ebook_app_cntx_p->ebr_set_local_setting_flag);    
    }
    ebr_fill_settings_inline_edit_struct((void*)&wgui_inline_items);	
    RegisterHighlightHandler(mmi_ebr_highlight_setting);

    if (g_ebook_app_cntx_p->ebr_set_local_setting_flag != 1 && 
	 mmi_ebr_is_memory_type_changed(ebr_get_drive_by_inlineHighligtItemSelectMemory()) && 
	 GetInlineDoneFlag(guiBuffer) == 0)
    {
        SetInlineDoneFlag(guiBuffer);
    }
    
    if (g_ebook_app_cntx_p->ebr_set_local_setting_flag != 1)
    {

	#if 0
	    if (guiBuffer)
		{
            list_menu_category_history *h;
            h = (list_menu_category_history *)guiBuffer;
            h->num_items =nNumofItem;
		}	
	#endif
    ShowCategory57Screen(
        STR_SETTINGS_EBOOK,
        GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_DONE,
        IMG_GLOBAL_BACK,
        nNumofItem,
        (U16*) gIndexIconsImageList_local,
        wgui_inline_items,
        0,
        guiBuffer);
    }
    else
    {
        ShowCategory57Screen(
            STR_BOOK_SETTINGS_EBOOK,
            GetRootTitleIcon(MAIN_MENU_EBOOK_MENUID),
            STR_GLOBAL_OK,
            IMG_GLOBAL_OK,
            STR_GLOBAL_DONE,
            IMG_GLOBAL_BACK,
            nNumofItem,
            (U16*) gIndexIconsImageList_local,
            wgui_inline_items,
            0,
            guiBuffer);
    }

    /* Register function for right soft key */
    SetCategory57RightSoftkeyFunctions(ebr_book_settings_confirm_pop_up, GoBackHistory);

}

#ifdef __MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  ebr_set_key_TS
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void              
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_set_key_TS(UI_character_type c)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	S8 *last_search_string;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (c)
    {

        case '0':
            ebr_entry_settings();
            break;

        case '1':
            ebr_entry_help();
            break;

        case '2':
            ebr_entry_add_book_mark();
            break;

        case '3':
            ebr_entry_search();
            break;

        case '4':
            ebr_pre_entry_jump_to();
            break;

        case '5':
            ebr_entry_goto_book_mark();
            break;

        case '6':
			ebr_frm_get_last_search_string(&last_search_string);
			if (((*last_search_string != '\0') || (*(last_search_string+1) != '\0')) && ebr_frm_get_auto_scroll_status())
			{
				ebr_app_search_string_next();
			}
            break;

        case '7':
            ebr_app_save_settings_scroll();
            break;

        case '8':
            ebr_entry_remove_book_mark();
            break;

        case '9':
            ebr_app_save_settings_screen_mode();
            break;
        default:
            break;
    }
}
#endif /* __MMI_TOUCH_SCREEN__ */ 


/*****************************************************************************
 * FUNCTION
 *  ebr_book_settings_confirm_pop_up
 * DESCRIPTION
 *  Confirmation Screen for Settings
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_book_settings_confirm_pop_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        get_string(STR_CONFIRM_SAVE_SETTINGS),
        IMG_GLOBAL_QUESTION,
        WARNING_TONE);

    if (g_ebook_app_cntx_p->ebr_set_local_setting_flag == 1)
    {
        SetLeftSoftkeyFunction(ebr_book_settings_done_hdlr, KEY_EVENT_UP);
        SetRightSoftkeyFunction(go_back_to_open_book_screen, KEY_EVENT_UP);
    }
    else
    {
        SetLeftSoftkeyFunction(ebr_entry_settings_done_hndlr, KEY_EVENT_UP);
        SetRightSoftkeyFunction(go_back_to_book_shelf_scren, KEY_EVENT_UP);
    }

}


/*****************************************************************************
 * FUNCTION
 *  go_back_to_book_shelf_scren
 * DESCRIPTION
 *  Go to book Shelf Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void go_back_to_book_shelf_scren(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* FMGR_Revise */
    if(IsScreenPresent(g_ebook_explorer_id))
        DeleteUptoScrID(g_ebook_explorer_id);
//  DeleteUptoScrID(SCR_FMGR_EXPLORER);
	GoBackHistory();
}


/*****************************************************************************
 * FUNCTION
 *  go_back_to_open_book_screen
 * DESCRIPTION
 *  Go to Open Book Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void go_back_to_open_book_screen(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (IsScreenPresent(EBOOK_BOOK_SETTINGS_SCREENID))
    {
        DeleteUptoScrID(EBOOK_OPEN_SCREENID);
        GoBackHistory();
    }
    else
    {
        DeleteUptoScrID(EBOOK_OPEN_SCREENID);
        GoBackHistory();
    }
}


/*****************************************************************************
 * FUNCTION
 *  ebr_exit_settings
 * DESCRIPTION
 *  Exit function for Settings Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_exit_settings(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    history_t Scr;
    S16 nHistory = 0;
    U16 size;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CloseCategory57Screen();
    Scr.scrnID = EBOOK_BOOK_SHELF_SETTINGS_SCREEN_ID;
    CloseCategory57Screen();
    Scr.entryFuncPtr = ebr_entry_settings;
    pfnUnicodeStrcpy((S8*) Scr.inputBuffer, (S8*) & nHistory);
    GetCategoryHistory(Scr.guiBuffer);
    size = (U16) GetCategory57DataSize();
    GetCategory57Data((U8*) Scr.inputBuffer);
    AddNHistory(Scr, size);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_entry_settings_done_hndlr
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_entry_settings_done_hndlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup(
        (PU8) GetString(STR_GLOBAL_SAVED),
        IMG_GLOBAL_ACTIVATED,
        1,
        EBR_ST_NOTIFYDURATION,
        SUCCESS_TONE);
#if 0    
    if (mmi_ebr_is_memory_type_changed(ebr_get_drive_by_inlineHighligtItemSelectMemory()))
    {
        DeleteUptoScrID(EBOOK_BOOKSHELF_SCREENID);
    }
    else
#endif        
    {
        /* FMGR_Revise */
        if(IsScreenPresent(g_ebook_explorer_id))
        {
            DeleteNScrId(g_ebook_explorer_id);
        }
        else if(IsScreenPresent(EBOOK_BOOKSHELF_SCREENID))
        {
            DeleteUptoScrID(EBOOK_BOOKSHELF_SCREENID);
        }
        //		DeleteUptoScrID(SCR_FMGR_EXPLORER);
    }
    ebr_frm_set_settings_and_save_in_nvram(g_ebookset_cntx_p);
    ebr_entry_book_shel_screen();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_book_settings_done_hdlr
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_book_settings_done_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_frm_set_settings_and_save(g_ebookset_cntx_p);
    if (IsScreenPresent(EBOOK_BOOK_SETTINGS_SCREENID))
    {
        DeleteUptoScrID(EBOOK_OPEN_SCREENID);
    }
    else
    {
        DeleteUptoScrID(EBOOK_OPEN_SCREENID);
    }
	DisplayPopup(
        (PU8) GetString(STR_GLOBAL_SAVED),
        IMG_GLOBAL_ACTIVATED,
        1,
        EBR_ST_NOTIFYDURATION,
        SUCCESS_TONE);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_fill_settings_inline_edit_struct
 * DESCRIPTION
 *  Set highlight handler for Menu Ooptions
 * PARAMETERS
 *  pInlineStruct       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_fill_settings_inline_edit_struct(void *pInlineStruct)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 i = 0;
    InlineItem *InlineStruct = (InlineItem*) pInlineStruct;
    S16 phoneDrvLetter, cardDrvLetter;
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    S16  seccardDrvLetter;
#endif
/*- zhouqin modify for dual t-flash card 20110503*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ebook_inline_const_string_context_p->ebook_settings_font_style[EBR_FONT_STYLE_NORMAL] =
        GetString(STR_SETTINGS_FONT_STYLE_NORMAL);
    g_ebook_inline_const_string_context_p->ebook_settings_font_style[EBR_FONT_STYLE_ITALIC] =
        GetString(STR_SETTINGS_FONT_STYLE_ITALIC);
    g_ebook_inline_const_string_context_p->ebook_settings_font_style[EBR_FONT_STYLE_BOLD] = GetString(STR_SETTINGS_FONT_STYLE_BOLD);

    g_ebook_inline_const_string_context_p->ebook_settings_font_size[EBR_FONT_SIZE_SMALL] = GetString(STR_SETTINGS_FONT_SIZE_SMALL);
    g_ebook_inline_const_string_context_p->ebook_settings_font_size[EBR_FONT_SIZE_MEDIUM] =
        GetString(STR_SETTINGS_FONT_SIZE_MEDIUM);
    g_ebook_inline_const_string_context_p->ebook_settings_font_size[EBR_FONT_SIZE_LARGE] = GetString(STR_SETTINGS_FONT_SIZE_LARGE);

    g_ebook_inline_const_string_context_p->ebook_settings_scroll_by[EBR_SCROLL_BY_LINE] = GetString(STR_SETTINGS_SCROLL_BY_LINE);
    g_ebook_inline_const_string_context_p->ebook_settings_scroll_by[EBR_SCROLL_BY_PAGE] = GetString(STR_SETTINGS_SCROLL_BY_PAGE);

    g_ebook_inline_const_string_context_p->ebook_settings_auto_scroll[EBR_SCROLL_OFF] = GetString(STR_SETTINGS_AUTO_SCROLL_OFF);
    g_ebook_inline_const_string_context_p->ebook_settings_auto_scroll[EBR_SCROLL_ON] = GetString(STR_SETTINGS_AUTO_SCROLL_ON);

    g_ebook_inline_const_string_context_p->ebook_settings_full_screen[EBR_FULL_SCREEN_OFF] =
        GetString(STR_SETTINGS_FULL_SCREEN_OFF);
    g_ebook_inline_const_string_context_p->ebook_settings_full_screen[EBR_FULL_SCREEN_ON] = GetString(STR_SETTINGS_FULL_SCREEN_ON);

    g_ebook_inline_const_string_context_p->ebook_settings_alignment_style[EBR_ALIGNMENT_LEFT] = GetString(STR_SETTINGS_LEFT_ALIGNMENT);
    g_ebook_inline_const_string_context_p->ebook_settings_alignment_style[EBR_ALIGNMENT_RIGHT] = GetString(STR_SETTINGS_RIGHT_ALIGNMENT);
	
#ifdef __MMI_CHSET_WESTERN_WIN__
    g_ebook_inline_const_string_context_p->ebook_settings_encoding_format[i++] =
        GetString(STR_SETTINGS_ENCODING_FORMAT_WESTERN_WIN);
#endif
    g_ebook_inline_const_string_context_p->ebook_settings_encoding_format[i++] = GetString(STR_SETTINGS_ENCODING_FORMAT_UCS2);
    g_ebook_inline_const_string_context_p->ebook_settings_encoding_format[i++] = GetString(STR_SETTINGS_ENCODING_FORMAT_UTF8);

#ifdef __MMI_CHSET_BIG5__
    g_ebook_inline_const_string_context_p->ebook_settings_encoding_format[i++] = GetString(STR_SETTINGS_ENCODING_FORMAT_BIG5);
#endif 
#ifdef __MMI_CHSET_GB2312__
    g_ebook_inline_const_string_context_p->ebook_settings_encoding_format[i++] = GetString(STR_SETTINGS_ENCODING_FORMAT_GB2312);
#endif 
#if 0
#ifdef __MMI_CHSET_TURKISH_ISO__
/* under construction !*/
/* under construction !*/
#endif /* __MMI_CHSET_TURKISH_ISO__ */ 
/* under construction !*/
#ifdef __MMI_CHSET_TURKISH_WIN__
/* under construction !*/
/* under construction !*/
#endif /* __MMI_CHSET_TURKISH_WIN__ */ 
#ifdef __MMI_CHSET_BALTIC_ISO__
/* under construction !*/
#endif 
#ifdef __MMI_CHSET_BALTIC_WIN__
/* under construction !*/
#endif 
#ifdef __MMI_CHSET_CEURO_ISO__
/* under construction !*/
/* under construction !*/
#endif /* __MMI_CHSET_CEURO_ISO__ */ 
#ifdef __MMI_CHSET_CEURO_WIN__
/* under construction !*/
#endif 
#ifdef __MMI_CHSET_GREEK_ISO__
/* under construction !*/
#endif 
#ifdef __MMI_CHSET_GREEK_WIN__
/* under construction !*/
#endif 
#ifdef __MMI_CHSET_HEBREW_ISO__
/* under construction !*/
#endif 
#ifdef __MMI_CHSET_HEBREW_WIN__
/* under construction !*/
#endif 
#ifdef __MMI_CHSET_LATIN_ISO__
/* under construction !*/
#endif 
#ifdef __MMI_CHSET_NORDIC_ISO__
/* under construction !*/
#endif 
#ifdef __MMI_CHSET_SEURO_ISO__
/* under construction !*/
/* under construction !*/
#endif /* __MMI_CHSET_SEURO_ISO__ */ 
#ifdef __MMI_CHSET_WESTERN_ISO__
/* under construction !*/
/* under construction !*/
#endif /* __MMI_CHSET_WESTERN_ISO__ */ 
#ifdef __MMI_CHSET_ARABIC_ISO__
/* under construction !*/
/* under construction !*/
#endif /* __MMI_CHSET_ARABIC_ISO__ */ 
#ifdef __MMI_CHSET_ARABIC_WIN__
/* under construction !*/
#endif 
#endif /* 0 */ 

    g_ebook_inline_const_string_context_p->ebook_settinsg_auto_scroll_speed[EBR_AUTO_SCROLL_SPEED_1] =
        GetString(STR_SETTINGS_AUTO_SCROLL_SPEED_1);
    g_ebook_inline_const_string_context_p->ebook_settinsg_auto_scroll_speed[EBR_AUTO_SCROLL_SPEED_2] =
        GetString(STR_SETTINGS_AUTO_SCROLL_SPEED_2);
    g_ebook_inline_const_string_context_p->ebook_settinsg_auto_scroll_speed[EBR_AUTO_SCROLL_SPEED_3] =
        GetString(STR_SETTINGS_AUTO_SCROLL_SPEED_3);
    g_ebook_inline_const_string_context_p->ebook_settinsg_auto_scroll_speed[EBR_AUTO_SCROLL_SPEED_4] =
        GetString(STR_SETTINGS_AUTO_SCROLL_SPEED_4);
    g_ebook_inline_const_string_context_p->ebook_settinsg_auto_scroll_speed[EBR_AUTO_SCROLL_SPEED_5] =
        GetString(STR_SETTINGS_AUTO_SCROLL_SPEED_5);
    SetInlineItemActivation(&InlineStruct[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&InlineStruct[0], (PU8) (U8*) GetString(STR_SETTINGS_FONT_STYLE));
    SetInlineItemActivation(&InlineStruct[2], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&InlineStruct[2], (PU8) (U8*) GetString(STR_SETTINGS_FONT_SIZE));

    SetInlineItemActivation(&InlineStruct[4], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&InlineStruct[4], (PU8) (U8*) GetString(STR_SETTINGS_SCROLL_BY));

    SetInlineItemActivation(&InlineStruct[6], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&InlineStruct[6], (PU8) (U8*) GetString(STR_SETTINGS_AUTO_SCROLL));

    SetInlineItemActivation(&InlineStruct[8], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&InlineStruct[8], (PU8) (U8*) GetString(STR_SETTINGS_AUTO_SCROLL_SPEED));

    SetInlineItemActivation(&InlineStruct[10], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&InlineStruct[10], (PU8) (U8*) GetString(STR_SETTINGS_FULL_SCREEN));

    SetInlineItemActivation(&InlineStruct[12], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&InlineStruct[12], (PU8) (U8*) GetString(STR_SETTINGS_ENCODING_FORMAT));

    SetInlineItemActivation(&InlineStruct[14], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&InlineStruct[14], (PU8) (U8*) GetString(STR_SETTINGS_ALIGNMENT_STYLE));

    SetInlineItemActivation(&InlineStruct[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(
        &InlineStruct[1],
        3,
        (U8**)g_ebook_inline_const_string_context_p->ebook_settings_font_style,
        (S32*) & (g_ebookset_cntx_p->inlineHighligtItemFontStyle) /* CurrSelect */ );

    SetInlineItemActivation(&InlineStruct[3], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(
        &InlineStruct[3],
        3,
        (U8**)g_ebook_inline_const_string_context_p->ebook_settings_font_size,
        (S32*) & (g_ebookset_cntx_p->inlineHighligtItemFontSize) /* CurrSelect */ );

    SetInlineItemActivation(&InlineStruct[5], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(
        &InlineStruct[5],
        2,
        (U8**)g_ebook_inline_const_string_context_p->ebook_settings_scroll_by,
        (S32*) & (g_ebookset_cntx_p->inlineHighligtItemScrollBy) /* CurrSelect */ );

    SetInlineItemActivation(&InlineStruct[7], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(
        &InlineStruct[7],
        2,
        (U8**)g_ebook_inline_const_string_context_p->ebook_settings_auto_scroll,
        (S32*) & (g_ebookset_cntx_p->inlineHighligtItemAutoScroll) /* CurrSelect */ );

    SetInlineItemActivation(&InlineStruct[9], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(
        &InlineStruct[9],
        5,
        (U8**)g_ebook_inline_const_string_context_p->ebook_settinsg_auto_scroll_speed,
        (S32*) & (g_ebookset_cntx_p->inlineHighligtItemScrollSpeed) /* CurrSelect */ );

    SetInlineItemActivation(&InlineStruct[11], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(
        &InlineStruct[11],
        2,
        (U8**)g_ebook_inline_const_string_context_p->ebook_settings_full_screen,
        (S32*) & (g_ebookset_cntx_p->inlineHighligtItemFulScreenMode) /* CurrSelect */ );

    SetInlineItemActivation(&InlineStruct[13], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(
        &InlineStruct[13],
        i,
        (U8**)g_ebook_inline_const_string_context_p->ebook_settings_encoding_format,
        (S32*) & (g_ebookset_cntx_p->inlineHighligtItemEncodingFormat) /* CurrSelect */ );

    SetInlineItemActivation(&InlineStruct[15], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(
        &InlineStruct[15],
        2,
        (U8**)g_ebook_inline_const_string_context_p->ebook_settings_alignment_style,
        (S32*) & (g_ebookset_cntx_p->inlineHighlightItemAlignmentStyle) /* CurrSelect */ );

    
    if (g_ebook_app_cntx_p->ebr_set_local_setting_flag != 1)
    {
        phoneDrvLetter = MMI_FS_GetDrive( FS_DRIVE_V_NORMAL, 2, FS_DRIVE_I_SYSTEM | FS_DRIVE_V_NORMAL );
        cardDrvLetter = MMI_FS_GetDrive( FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE );  
    /*+ zhouqin modify for dual t-flash card 20110503*/
    #if defined(DUAL_TFLASH_SUPPORT)
        seccardDrvLetter = MMI_FS_GetDrive( FS_DRIVE_TFCARD_SECOND, 1, FS_NO_ALT_DRIVE );  
    #endif
    /*- zhouqin modify for dual t-flash card 20110503*/
        
        if((phoneDrvLetter > 0) && (cardDrvLetter > 0)
                /*+ zhouqin modify for dual t-flash card 20110503*/
                #if defined(DUAL_TFLASH_SUPPORT)
                && (seccardDrvLetter > 0)
                #endif
                /*- zhouqin modify for dual t-flash card 20110503*/
            )
        {
            g_ebook_inline_const_string_context_p->ebook_settings_storage[EBR_PHONE_MEMORY] = GetString( STR_SETTINGS_PHONE_MEMORY );
            g_ebook_inline_const_string_context_p->ebook_settings_storage[EBR_CARD_MEMORY] = GetString( STR_SETTINGS_CARD_MEMORY );
            /*+ zhouqin modify for dual t-flash card 20110503*/
            #if defined(DUAL_TFLASH_SUPPORT)
            g_ebook_inline_const_string_context_p->ebook_settings_storage[EBR_SEC_CARD_MEMORY] = GetString( STR_SETTINGS_SEC_CARD_MEMORY );
            #endif
            /*- zhouqin modify for dual t-flash card 20110503*/
            
            SetInlineItemActivation(&InlineStruct[16], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
            SetInlineItemCaption(&InlineStruct[16], (PU8) (U8*) GetString(STR_SETTINGS_SELECT_MEMORY));

            if(ebr_get_drive_by_inlineHighligtItemSelectMemory() == cardDrvLetter)
            {
                g_ebookset_cntx_p->inlineHighligtItemSelectMemory = EBR_CARD_MEMORY;
            }
            /*+ zhouqin modify for dual t-flash card 20110503*/
            #if defined(DUAL_TFLASH_SUPPORT)
            else if(ebr_get_drive_by_inlineHighligtItemSelectMemory() == seccardDrvLetter)
            {
                g_ebookset_cntx_p->inlineHighligtItemSelectMemory = EBR_SEC_CARD_MEMORY;
            }
            #endif
            /*- zhouqin modify for dual t-flash card 20110503*/
            SetInlineItemActivation(&InlineStruct[17], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
            SetInlineItemSelect(
                &InlineStruct[17],
                /*+ zhouqin modify for dual t-flash card 20110503*/
                #if defined(DUAL_TFLASH_SUPPORT)
                EBR_MAX_SETTINGS_STORAGE,
                #else
                2,
                #endif
                /*- zhouqin modify for dual t-flash card 20110503*/
                
                (U8**)g_ebook_inline_const_string_context_p->ebook_settings_storage,
                (S32*) & (g_ebookset_cntx_p->inlineHighligtItemSelectMemory));
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  ebr_mmi_ebook_exit
 * DESCRIPTION
 *  exit function for Open Book
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_mmi_ebook_exit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 screenID;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    TurnOffBacklight();
    if (g_ebook_app_cntx_p->enable_screen_exit)
    {
        ebr_frm_ebook_exit(EBR_FRM_EXIT_BY_GOBACK_TO_BOOKSHELF);
        if (ebr_frm_get_current_recursive_conversion_state() == EBR_FRM_CONVERSION_UNDERWAY)
        {

            ebr_frm_stop_recursion();
        }
	 #ifdef __MMI_EBOOK_SPEED_OPEN__
	 if(ebr_frm_get_current_recursive_conversion_state() == EBR_FRM_CONVERSION_BACKGROUND)
	 {
	 	ebr_frm_stop_recursion();
		g_curr_book_cntx_p->state_recursive_conversion = EBR_FRM_NO_CONVERSION;
	 }
	 #endif
    }
    screenID = GetActiveScreenId();

    ebr_restore_file_read_only(ebr_fmgr_get_file_name());

}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void entry_duplicate_book_mark_pop_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        get_string(STR_DUPLICATE_BOOK_MARK),
        IMG_GLOBAL_QUESTION,
        WARNING_TONE);

    SetLeftSoftkeyFunction(ebr_frm_add_book_mark, KEY_EVENT_UP);
    SetRightSoftkeyFunction(ebr_duplicate_book_mark_back, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_duplicate_book_mark_back(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DeleteScreenIfPresent(SCR_EDIT_BOOK_MARK_NAME);
    GoBackHistory();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_delete_all_from_book_shelf
 * DESCRIPTION
 *  Pop Up for Delete All
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_delete_all_from_book_shelf(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        get_string(STR_DELETE_ALL_EBOOK_CONFIRM),
        IMG_GLOBAL_QUESTION,
        WARNING_TONE);

    SetLeftSoftkeyFunction(ebr_confirm_deletion_of_all_files, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_confirm_deletion_of_all_files
 * DESCRIPTION
 *  Pop Up for Delete All
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_confirm_deletion_of_all_files(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_frm_delete_all_from_book_shelf();
    DeleteNScrId(SCR_FMGR_APP_FILE_OPTION);
    DisplayPopup((PU8) GetString(STR_DELETED), IMG_GLOBAL_ACTIVATED, 1, EBR_ST_NOTIFYDURATION, SUCCESS_TONE);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_restore_default_book_settings_thru_frm
 * DESCRIPTION
 *  Pop Up for Delete All
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_restore_default_book_settings_thru_frm(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ebr_frm_restore_default_book_settings())
    {
        ebr_app_display_pop_up_default_settings_for_book_restored();
    }
    else    /* Error Settings could not be restored */
    {
        ebr_app_display_pop_up_default_settings_not_restored();
    }

}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_restore_default_book_settings
 * DESCRIPTION
 *  Confirmation screen for Restore default settings
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_restore_default_book_settings(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        get_string(STR_RESTORE_DEFAULT_SETTINGS),
        IMG_GLOBAL_QUESTION,
        WARNING_TONE);

    SetLeftSoftkeyFunction(ebr_app_restore_default_book_settings_thru_frm, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_restore_default_book_shelf_settings_thru_frm
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_restore_default_book_shelf_settings_thru_frm(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ebr_frm_restore_default_book_shelf_settings();
    ebr_app_display_pop_up_default_settings_restored();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_restore_default_book_shelf_settings
 * DESCRIPTION
 *  Confirmation screen for Restore default settings.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_restore_default_book_shelf_settings(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        get_string(STR_RESTORE_DEFAULT_SETTINGS),
        IMG_GLOBAL_QUESTION,
        WARNING_TONE);

    SetLeftSoftkeyFunction(ebr_app_restore_default_book_shelf_settings_thru_frm, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    //mmi_fmgr_close_option_screen();
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_display_pop_up_maximum_book_marks(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup((PU8) GetString(STR_MAX_BOOK_MARK), IMG_GLOBAL_WARNING, 1, EBR_ST_NOTIFYDURATION, ERROR_TONE);
    DeleteUptoScrID(EBOOK_BOOK_SETTINGS_SCREENID);
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_display_pop_up_book_mark_name_empty(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup(
        (PU8) GetString(STR_BOOK_MARK_NAME_IS_EMPTY),
        IMG_GLOBAL_UNFINISHED,
        1,
        EBR_ST_NOTIFYDURATION,
        ERROR_TONE);
    /* DeleteUptoScrID(EBOOK_BOOK_SETTINGS_SCREENID); */
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_display_pop_up_book_mark_sucessfully_added(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup(
        (PU8) GetString(STR_EBOOK_BOOKMARK_ADDED),
        IMG_GLOBAL_ACTIVATED,
        1,
        EBR_ST_NOTIFYDURATION,
        SUCCESS_TONE);
    if (IsScreenPresent(EBOOK_BOOK_SETTINGS_SCREENID))
    {
        DeleteNScrId(EBOOK_BOOK_SETTINGS_SCREENID);
    }
    else
    {
        DeleteNScrId(EBOOK_ADD_BOOK_MARK_SCREEN_ID);
    }
}


/*****************************************************************************
 * FUNCTION
 
 * DESCRIPTION
 
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_display_pop_up_book_mark_not_sucessfully_added(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup(
        (PU8) GetString(STR_EBOOK_BOOKMARK_ADD_FAIL),
        IMG_GLOBAL_ERROR,
        1,
        EBR_ST_NOTIFYDURATION,
        ERROR_TONE);
    if (IsScreenPresent(EBOOK_BOOK_SETTINGS_SCREENID))
    {
        DeleteUptoScrID(EBOOK_BOOK_SETTINGS_SCREENID);
    }
    else
    {
        DeleteNScrId(EBOOK_ADD_BOOK_MARK_SCREEN_ID);
    }
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_search_result_unsucessfull
 * DESCRIPTION
 *  Pop Up for Search Uncessfull
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_search_result_unsucessfull(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup(
        (U8*) GetString(STR_EBOOK_SEARCH_UN_SUCESSFULL),
        IMG_GLOBAL_UNFINISHED,
        1,
        UI_POPUP_NOTIFYDURATION_TIME,
        ERROR_TONE);
    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
    ebr_app_search_result_successful(FALSE);
}

/*****************************************************************************
 * FUNCTION
 *  ebr_app_search_result_invalid
 * DESCRIPTION
 *  Pop Up for Search Invalid
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_search_result_invalid(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup(
        (U8*) GetString(STR_EBOOK_SEARCH_INVALID),
        IMG_GLOBAL_UNFINISHED,
        1,
        UI_POPUP_NOTIFYDURATION_TIME,
        ERROR_TONE);
    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
    ebr_app_search_result_successful(FALSE);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_frm_search_string_empty
 * DESCRIPTION
 *  Pop Up for Search Uncessfull
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_frm_search_string_empty(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup(
        (U8*) GetString(STR_GLOBAL_EMPTY),
        IMG_GLOBAL_UNFINISHED,
        1,
        UI_POPUP_NOTIFYDURATION_TIME,
        ERROR_TONE);
    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
    ebr_app_search_result_successful(FALSE);
}



/*****************************************************************************
 * FUNCTION
 *  ebr_app_display_pop_up_changed_screen_mode
 * DESCRIPTION
 *  Pop Up for Screen mode changed.
 * PARAMETERS
 *  screen_mode     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_display_pop_up_changed_screen_mode(S32 screen_mode)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (screen_mode == 0)
    {
        DisplayPopup(
            (PU8) GetString(STR_FULL_SCREEN_OFF),
            IMG_GLOBAL_ACTIVATED,
            1,
            EBR_ST_NOTIFYDURATION,
            SUCCESS_TONE);
    }
    else
    {
        DisplayPopup((PU8) GetString(STR_FULL_SCREEN_ON), IMG_GLOBAL_ACTIVATED, 1, EBR_ST_NOTIFYDURATION, SUCCESS_TONE);
    }
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_display_pop_up_changed_auto_scroll
 * DESCRIPTION
 *  Pop Up for Auto Scroll changed.
 * PARAMETERS
 *  auto_scroll_mode        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_display_pop_up_changed_auto_scroll(S32 auto_scroll_mode)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (auto_scroll_mode == 0)
    {
        DisplayPopup(
            (PU8) GetString(STR_AUTO_SCROLL_OFF),
            IMG_GLOBAL_ACTIVATED,
            1,
            EBR_ST_NOTIFYDURATION,
            SUCCESS_TONE);
    }
    else
    {
        DisplayPopup((PU8) GetString(STR_AUTO_SCROLL_ON), IMG_GLOBAL_ACTIVATED, 1, EBR_ST_NOTIFYDURATION, SUCCESS_TONE);
    }
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_display_pop_up_error_opening_book
 * DESCRIPTION
 *  Pop Up for Error Opening book
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_display_pop_up_error_opening_book(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup((PU8) GetString(STR_ERROR_OPENINIG_BOOK), IMG_GLOBAL_UNFINISHED, 1, EBR_ST_NOTIFYDURATION, ERROR_TONE);
}



/*****************************************************************************
 * FUNCTION
 *  ebr_app_display_pop_up_empty_book
 * DESCRIPTION
 *  Pop Up for Error Opening book
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_display_pop_up_empty_book(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup((PU8) GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_UNFINISHED, 1, EBR_ST_NOTIFYDURATION, ERROR_TONE);
}



/*****************************************************************************
 * FUNCTION
 *  ebr_app_display_pop_up_format_not_supported
 * DESCRIPTION
 *  Pop Up for File fomat not supported.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_display_pop_up_format_not_supported(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup(
        (U8*) GetString(STR_EBOOK_FORMAT_NOT_SUPPORTED),
        IMG_GLOBAL_UNFINISHED,
        1,
        UI_POPUP_NOTIFYDURATION_TIME,
        ERROR_TONE);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_display_pop_up_default_settings_restored
 * DESCRIPTION
 *  Pop Up for Resore defauklt settings.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_display_pop_up_default_settings_restored(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    IsBackHistory = MMI_TRUE;
     DisplayPopup(
        (PU8) GetString(STR_SUCESSFULL_RESTORE_DEFAULT),
        IMG_GLOBAL_ACTIVATED,
        1,
        EBR_ST_NOTIFYDURATION,
        SUCCESS_TONE);
    /* FMGR_Revise */
    if(IsScreenPresent(g_ebook_explorer_id))
        DeleteUptoScrID(g_ebook_explorer_id);
//    DeleteUptoScrID(SCR_FMGR_EXPLORER);    
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_display_pop_up_default_settings_not_restored
 * DESCRIPTION
 *  Pop Up for DDefauklt settings unsucessfull.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_display_pop_up_default_settings_not_restored(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup(
        (PU8) GetString(STR_UNSUCESSFULL_RESTORE_DEFAULT),
        IMG_GLOBAL_UNFINISHED,
        1,
        EBR_ST_NOTIFYDURATION,
        ERROR_TONE);
    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_display_pop_up_default_settings_for_book_restored
 * DESCRIPTION
 *  Pop Up for Default settings restored.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_display_pop_up_default_settings_for_book_restored(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup(
        (PU8) GetString(STR_SUCESSFULL_RESTORE_DEFAULT),
        IMG_GLOBAL_ACTIVATED,
        1,
        EBR_ST_NOTIFYDURATION,
        SUCCESS_TONE);
    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_delete_upto_open_book_screen_id
 * DESCRIPTION
 *  History Handling
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_delete_upto_open_book_screen_id(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DeleteUptoScrID(EBOOK_OPEN_SCREENID);
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_go_back_history
 * DESCRIPTION
 *  History handling
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_go_back_history(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GoBackHistory();
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_check_nfo_from_fmgr
 * DESCRIPTION
 *  Function to get the Nf o id for File manager
 * PARAMETERS
 *  file_name       [IN]        
 *  file_path       [IN]        
 *  file_ext        [IN]        
 *  is_short        [IN]        
 * RETURNS
 *  S32
 *****************************************************************************/
S32 ebr_app_check_nfo_from_fmgr(S8 *file_name, S8 *file_path, S8 *file_ext, BOOL is_short)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    S32 return_value1 = ebr_frm_fmgr_pre_entry_open_book(file_name, file_path, file_ext);
    S32 return_value2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     ebr_frm_fmgr_set_nfo_name(ebr_frm_get_nfo_file_name((U8*)file_name));

    /* Check wether NFo file is present or not. If not then cretae a new nfo file */
    if (return_value1 == 1)
    {
        return_value2 = ebr_frm_check_nfo_file_present((U8*)file_name);
        return return_value2;
        
    }
    return return_value1;
}


/*****************************************************************************
 * FUNCTION
 *  ebr_frm_open_book_from_file_manager
 * DESCRIPTION
 *  open a file from file manager
 * PARAMETERS
 *  file_name       [IN]        
 *  file_path       [IN]        
 *  icon_id         [IN]
 *  is_short        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_frm_open_book_from_file_manager(S8 *file_name, S8 *file_path, U16 icon_id, BOOL is_short, BOOL entry_from_myfavorite)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 drive_type;
    S32 return_value;
    PS8 file_ext;
    ebr_memory_type_enum memory_selected;

    pBOOL is_file_valid = FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ebr_frm_get_current_recursive_conversion_state() == EBR_FRM_CONVERSION_UNDERWAY)
    {
		DisplayPopup(
			(PU8) GetString(STR_EBOOK_IS_INITIALIZING),
			IMG_GLOBAL_WARNING,
			0,
			1000,
			WARNING_TONE);
        return;
    }
    
    ebr_app_delete_all_ebook_relavant_screen();

    g_ebook_app_cntx_p->icon_id = icon_id;
    g_ebook_app_cntx_p->entry_from_my_favorite = entry_from_myfavorite;
    pfnUnicodeStrcpy((PS8)g_ebook_app_cntx_p->ebr_file_mgr_file_path, (PS8)file_path);
    file_ext = mmi_fmgr_extract_ext_file_name((PS8) file_name);
    ebr_frm_reset_search_flag();
    ebr_frm_fmgr_set_application_type();
    drive_type = ebr_fmgr_is_book_shelf_path(g_ebook_app_cntx_p->ebr_file_mgr_file_path);
    memory_selected = ebr_frm_get_drive_type();
    ebr_frm_fmgr_set_drive_type(drive_type);
    ebr_fmgr_set_file_name((PS8) file_name);
    ebr_frm_set_file_info_from_fmgr((S8*) file_name, g_ebook_app_cntx_p->ebr_file_mgr_file_path, file_ext, is_short);
    ebr_frm_fmgr_set_file_type((S8*) file_name);
    if (drive_type < 0)
    {
        return_value = -1;
    }
    else
    {
        return_value = ebr_app_check_nfo_from_fmgr((S8*) file_name, g_ebook_app_cntx_p->ebr_file_mgr_file_path, file_ext, is_short);
    }
    if (return_value > 0)
    {
        ebr_entry_open_book();
    }
    else
    {
        if (return_value == 0)    /* nfo file dose not exist // Ask the user if he wants to add this in the bookshelf */
        {
            ebr_app_add_to_book_shelf();
        }
        else    /* Means the book is not in the ebook Folder */
        {
            ebr_frm_fmgr_set_drive_type(memory_selected);
            // Check here that the file is a valid file.
            // If not a valid file pop an error and no need to move the file
            is_file_valid = ebr_frm_check_file_validity(g_ebook_app_cntx_p->ebr_file_mgr_file_path, file_name);
            if (!is_file_valid)
            {
                ebr_app_display_pop_up_format_not_supported();
            }
            else
            {
                DisplayConfirm(
                    STR_GLOBAL_YES,
                    IMG_GLOBAL_YES,
                    STR_GLOBAL_NO,
                    IMG_GLOBAL_NO,
                    get_string(STR_ADD_TO_EBOOK_FOLDER),
                    IMG_GLOBAL_QUESTION,
                    WARNING_TONE);

                SetLeftSoftkeyFunction(ebr_fmgr_move_to_ebook_folder, KEY_EVENT_UP);
                SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
            }
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_add_to_book_shelf
 * DESCRIPTION
 *  Add files from file manager to book shelf
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_add_to_book_shelf(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ebr_frm_check_nfo_file_size())
    {
        DeleteNScrId(SCR_FMGR_APP_FILE_OPTION);
        ebr_app_dispaly_less_memory_pop_up();
    }
    else
    {
        switch (ebr_frm_add_to_book_shelf())
        {
            case EBR_FRM_NFO_CREATE_SUCCESS:
                ebr_entry_open_book();
                break;
            case EBR_FRM_NFO_CREATE_FILE_TYPE_NOT_SUPPORTED:
                DeleteNScrId(SCR_FMGR_APP_FILE_OPTION);
                ebr_app_display_pop_up_format_not_supported();
                ebr_frm_rwrite_into_nvram();
                break;
            case EBR_FRM_EMPTY_BOOK:
                DeleteNScrId(SCR_FMGR_APP_FILE_OPTION);
                ebr_app_display_pop_up_empty_book();
                ebr_frm_rwrite_into_nvram();
                break;
            default:
                DeleteNScrId(SCR_FMGR_APP_FILE_OPTION);
                ebr_app_display_pop_up_error_opening_book();
                ebr_frm_rwrite_into_nvram();
                break;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  ebr_display_error_pop_up
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_display_error_pop_up(S32 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup((PU8) GetString(GetFileSystemErrorString(result)), IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
    return;
}


/*****************************************************************************
 * FUNCTION
 *  ebr_app_dispaly_less_memory_pop_up
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_dispaly_less_memory_pop_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup((PU8) GetString(STR_EBOOK_MEMORY_LESS), IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
}

#ifdef __MMI_INDIC_ALG__


/*****************************************************************************
 * FUNCTION
 *  ebr_app_show_string_incomplete_pop_up
 * DESCRIPTION
 *  Display incomplete popup
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_show_string_incomplete_pop_up(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    DisplayPopup(
        (PU8) GetString(STR_INCOMPLETE_SEARCH_STRING),
        IMG_GLOBAL_WARNING,
        1,
        EBR_ST_NOTIFYDURATION,
        WARNING_TONE);
}
#endif /* __MMI_INDIC_ALG__ */ 


/*****************************************************************************
 * FUNCTION
 *  ebr_app_clear_all_key_handlers
 * DESCRIPTION
 *  Clear All key Handlers
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_clear_all_key_handlers(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearKeyEvents();
    ClearAllKeyHandler();
    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
}

/*****************************************************************************
 * FUNCTION
 *  ebr_app_clear_all_key_handlers
 * DESCRIPTION
 *  Highlight handler of ebook setting screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ebr_highlight_setting(S32 index)
{
#if 0
    if (index == EBR_TOTAL_NUMBER_OF_ITEM_BKSHLF - 1)
    {
        ChangeLeftSoftkey(STR_GLOBAL_EDIT, 0);
        SetLeftSoftkeyFunction(mmi_ebr_entry_select_drive, KEY_EVENT_UP);
    }
    else
#endif        
    {
        ChangeLeftSoftkey(0, 0);
        SetLeftSoftkeyFunction(NULL, KEY_EVENT_UP);
    }
    g_ebook_app_cntx_p->current_setting_highlight_item = index;
}

void mmi_ebr_entry_select_drive(void)
{        
    mmi_fmgr_sel_drv_and_enter(APP_EBOOK, mmi_ebr_save_drive_selection);    
}

void mmi_ebr_save_drive_selection(S16 drive_letter)
{
    /* drive_letter == 0 means user press back and give up the change */
    if (drive_letter != 0)
    {
        ebr_set_inlineHighligtItemSelectMemory(drive_letter);
    }    
}


/*****************************************************************************
 * FUNCTION
 *  ebr_delete_no_ebook
 * DESCRIPTION
 *  select "no" for Delete Book Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_delete_no_ebook(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    //mmi_fmgr_close_option_screen();
	GoBackHistory();
}

/*****************************************************************************
 * FUNCTION
 *  ebr_set_inlineHighligtItemSelectMemory
 * DESCRIPTION
 *  set the g_ebookset_cntx_p->inlineHighligtItemSelectMemory.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_set_inlineHighligtItemSelectMemory(S8 drive)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if(drive == MMI_PUBLIC_DRV)
    {
        g_ebookset_cntx_p->inlineHighligtItemSelectMemory = EBR_PHONE_MEMORY;
    }
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    else if(drive == MMI_SEC_CARD_DRV)
    {
        g_ebookset_cntx_p->inlineHighligtItemSelectMemory = EBR_SEC_CARD_MEMORY;
    }
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
    else
    {
        g_ebookset_cntx_p->inlineHighligtItemSelectMemory = EBR_CARD_MEMORY;
    }
}

/*****************************************************************************
 * FUNCTION
 *  ebr_get_drive_by_inlineHighligtItemSelectMemory
 * DESCRIPTION
 *  Get the dirve by g_ebookset_cntx_p->inlineHighligtItemSelectMemory.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
S8 ebr_get_drive_by_inlineHighligtItemSelectMemory(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if(g_ebookset_cntx_p->inlineHighligtItemSelectMemory == EBR_PHONE_MEMORY)
    {
        return MMI_PUBLIC_DRV;
    }
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    else if(g_ebookset_cntx_p->inlineHighligtItemSelectMemory == EBR_SEC_CARD_MEMORY)
    {
        return MMI_SEC_CARD_DRV;
    }
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
    else
    {
        return MMI_CARD_DRV;
    }
}

#ifdef __MMI_BPP_SUPPORT__ /*BPP revise*/


/*****************************************************************************
 * FUNCTION
 *  ebr_highlight_book_bpp_print
 * DESCRIPTION
 *  Set highlight handler for BPP print
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_highlight_book_bpp_print(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);

    SetLeftSoftkeyFunction(ebr_app_bpp_print, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
    SetKeyHandler(ebr_app_bpp_print, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	/*ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);*/
}

#if 0 /*BPP revise*/
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
/* under construction !*/
/* under construction !*/
#endif


/*****************************************************************************
 * FUNCTION
 *  ebr_compose_xhtml_file
 * DESCRIPTION
 *  compose xhtml file
 * PARAMETERS
 *  handle            [IN]
 *  pfile_name      [IN]
 *  page_info        [IN]
 * RETURNS
 *  bpp_app_ret_code_enum
 *****************************************************************************/
static S32 ebr_compose_xhtml_file(BPP_INSTANCE handle,S8* pfile_name,bpp_app_page_info_struct* page_info)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	FS_HANDLE   fh = FS_Open((U16*)pfile_name, FS_READ_WRITE|FS_CREATE_ALWAYS);
    U32         buf_remain = ebr_compose_buf_size;
    S8*         pbuf = (S8*)OslMalloc(buf_remain);
    U32         len = 0;
    const S8*   cdata = 0;
    S32         ret = BPP_APP_ERROR;
    S8*         pname_buf = NULL;
    S32         ebr_width = 0;
    S32         ebr_heigh = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if(fh <0 || pbuf==NULL)
    {
        goto BPP_ERROR_CLEAN;
    }
    ebr_width = page_info->page_width - page_info->left_marign_size - page_info->right_marign_size;
    ebr_heigh = page_info->page_height - page_info->page_header_size - page_info->page_footer_size;

    cdata = bt_mmi_bpp_get_xhtml_start();
    
    EBR_WRITE_DATA(fh,pbuf,buf_remain,cdata);
    EBR_WRITE_DATA(fh,pbuf,buf_remain,ebook_head_template);
    EBR_FLUSH_BUF(fh,pbuf,buf_remain);
    
    if(bt_mmi_bpp_get_default_page_style(handle,pbuf,(U16*)&buf_remain,NULL,ebr_page_footer) == BPP_APP_SUCCESS)
    {
        kal_char* pebook_name = ebr_get_current_book_name();
        U32       size      = ebr_get_current_book_size();
        
        EBR_FLUSH_BUF(fh,pbuf,buf_remain);
        
        buf_remain-=sprintf(pbuf,ebook_style_template,ebr_width, ebr_heigh);

        EBR_FLUSH_BUF(fh,pbuf,buf_remain);
	
        len = pfnUnicodeStrlen(pebook_name)*(ENCODING_LENGTH+1);
        pname_buf = OslMalloc(len+2);
        mmi_chset_ucs2_to_utf8_string((U8*)pname_buf,len,(U8*)pebook_name);
	
        if(strlen(ebook_body_template) + strlen(pname_buf) < ebr_compose_buf_size - MAX_EBOOK_SIZE_STR)
        {
            U32 fsize = 0;
            buf_remain -=sprintf(pbuf,ebook_body_template,pname_buf,size);
            EBR_FLUSH_BUF(fh,pbuf,buf_remain);
            FS_GetFileSize(fh,&fsize);
            ret = size + fsize;
        }
    }

BPP_ERROR_CLEAN:
   if(pbuf)
   {
        OslMfree(pbuf);
   }
   if(fh >0)
   {
        FS_Close(fh);
   }
   if(pname_buf)
	{
        OslMfree(pname_buf);
   }
   return ret; 
}
        

/*****************************************************************************
 * FUNCTION
 *  ebr_print_callback
 * DESCRIPTION
 *  bpp call back function
 * PARAMETERS
 *  handle            [IN]
 *  event      [IN]
 *  wparam        [OUT]
 *  lparam        [IN]
 * RETURNS
 *  bpp_app_ret_code_enum
 *****************************************************************************/

bpp_app_ret_code_enum ebr_print_callback(BPP_INSTANCE handle, bpp_app_event_enum event, void* wparam, void* lparam)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bpp_app_ret_code_enum ret = BPP_APP_SUCCESS;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch(event)
    {
        case BPP_PRINT_COMPLETE:
            mmi_bt_bpp_destory_job(handle);
            break;
        
        case BPP_POVIDE_DATA:
        {
            bpp_app_print_data_struct* print_data = (bpp_app_print_data_struct*)wparam ;
            S8* pfile_name = print_data? (S8*)print_data->pfile_name:NULL;
            bpp_app_page_info_struct* page_info = (bpp_app_page_info_struct*)lparam;
            if(pfile_name && page_info)
            {
                S32 size = ebr_compose_xhtml_file(handle,pfile_name,page_info);
                if(size > 0)
                {
                    print_data->total_size = size;
                    print_data->pdu = NULL;
                }
                else
                {

                    ret = BPP_APP_ERROR;
                }
            }
            else
    		{
                    ret = BPP_APP_ERROR;
    		}
        }
            break;
            
        case BPP_PRINT_PREVIEW:
		    break;
        default:
            ret = BPP_APP_ERROR;
            break;
    }
    return ret;
}

		
/*****************************************************************************
 * FUNCTION
 *  ebr_app_bpp_print
 * DESCRIPTION
 *  print function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ebr_app_bpp_print(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BPP_INSTANCE hjob = BPP_INVALID_HINSTANCE;

	/*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/    
    hjob= mmi_bt_bpp_new_job(ebr_print_callback);
        
    if(hjob != BPP_INVALID_HINSTANCE)
    {
        bpp_page_setup_struct page_setup;
        page_setup.paper_size = BPP_MEDIA_SIZE_A4;
        page_setup.left_margin = 6;
        page_setup.right_margin = 6;
        page_setup.top_margin = 6;
        page_setup.bottom_margin = 6;
        mmi_bt_bpp_page_setup(hjob,&page_setup);
        mmi_bt_bpp_job_setup(hjob,NULL);
        mmi_bt_bpp_preview_print(hjob,MMI_FALSE);
    }
    return;
}



#endif /* __MMI_BPP_SUPPORT__ */ 
BOOL mmi_ebook_is_active(void)
{
    U16 topscrID;
    
    topscrID = GetActiveScreenId();

    if(topscrID<EBOOK_SCREENID|| topscrID> SCR_ID_EBOOK_FILE_MOVE_WAITSCREEN  )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }


}

#endif /* __MMI_EBOOK_READER__ */ 
