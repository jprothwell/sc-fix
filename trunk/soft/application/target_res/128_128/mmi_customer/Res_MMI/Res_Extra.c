/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
 * Res_Extra.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Populate Resource for Extra. 
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "stdc.h"
#include "customcfg.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

//#include "wgui_categories.h"
//#include "extradef.h"


#include "mmi_features.h"

#if defined(__MMI_BMI__)
#include "bmi.h"
#endif
#if defined(__MMI_CALORIE__)
/* under construction !*/
#endif
#if defined(__MMI_MENSTRUAL__)
#include "healthmenstural.h"
#endif
#if defined(__MMI_DICTIONARY__)
#include "dictionaryMain.h"
#endif

#if defined(__MMI_CURRENCY_CONVERTER__)
#include "currencyconvertor.h"
#elif defined(__MMI_EXTENDED_CURRENCY_CONVERTER__)
#include "currencyconvertore.h"
#endif

#if defined(__MMI_UNIT_CONVERTER__) 
#include "unitconvertor.h"
#endif

#if defined(__MMI_CALCULATOR__)
#include "gui_data_types.h"
#include "gdi_include.h"
#include "calculator.h"
#endif
#if defined(__MMI_EBOOK_READER__)
#include "ebookdefs.h"
#include "filemanagerdef.h"
#endif 

#include "organizerdef.h"

#ifndef __DISABLE_SHORTCUTS_MENU__
#include "shortcutsdefs.h"
#endif

#ifdef __MMI_NOTEPAD__
#include "notepadenum.h"
#endif

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"
#include "custresdef.h"


#if 0
// Health Enum
typedef enum
{
#if defined(__MMI_BMI__)
	HALETH_ENUM_BMI,
#endif

#if defined(__MMI_MENSTRUAL__)
	HALETH_ENUM_MENSTRUAL,
#endif

#if defined(__MMI_CALORIE__)
/* under construction !*/
#endif
	HALETH_ENUM_TOTAL
} HealthMenu;

#endif 

#if defined(__MMI_CALCULATOR__)

void PopulateCalculator(void)
{
	ADD_APPLICATION_STRING2(STR_ID_CALC_MENU,"Calculator","To Go to Calculator Screen.");
#if !defined(MEM_ULC_3216)
	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_MENU_ICON,CUST_IMG_PATH"/MainLCD/Calculator/SB_OCACU.gif","Icon for Calculator Options Menu.");
#endif
#if 0
#if defined(__MMI_VERSION_2__)
		ADD_APPLICATION_MENUITEM((EXTRA_CALCULATOR_MENU,MAIN_MENU_EXTRA_MENUID,0,SHOW,MOVEABLEACROSSPARENT|SHORTCUTABLE,DISP_LIST,
			STR_ID_CALC_MENU,IMG_ID_CALC_MENU_ICON));
#else
	ADD_APPLICATION_MENUITEM((EXTRA_CALCULATOR_MENU,MAIN_MENU_ORGANIZER_MENUID,0,SHOW,MOVEABLEACROSSPARENT|SHORTCUTABLE,DISP_LIST,
		STR_ID_CALC_MENU,IMG_ID_CALC_MENU_ICON));
#endif
#endif	
#ifndef __MMI_TOUCH_CALCULATOR__
#ifdef __MMI_SLIM_CALCULATOR__
	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_PLUS,CUST_IMG_PATH"/MainLCD/Calculator/Slim/PLUS.jpg","DIVIDE BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_MINUS,CUST_IMG_PATH"/MainLCD/Calculator/Slim/SUBTRACT.jpg","DIVIDE BMP");	
	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_MULTIPLY,CUST_IMG_PATH"/MainLCD/Calculator/Slim/MULTI.jpg","DIVIDE BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_DIVIDE,CUST_IMG_PATH"/MainLCD/Calculator/Slim/DIVIDE.jpg","DIVIDE BMP");	
	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_MAIN,CUST_IMG_PATH"/MainLCD/Calculator/Slim/MAP.jpg","DIVIDE BMP");
#else
	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_PLUS,CUST_IMG_PATH"/MainLCD/Calculator/PLUS.gif","DIVIDE BMP");
//del by reslim v0.1	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_MINUS,CUST_IMG_PATH"/MainLCD/Calculator/SUBTRACT.jpg","DIVIDE BMP");	
//del by reslim v0.1	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_MULTIPLY,CUST_IMG_PATH"/MainLCD/Calculator/MULTI.jpg","DIVIDE BMP");
//del by reslim v0.1	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_DIVIDE,CUST_IMG_PATH"/MainLCD/Calculator/DIVIDE.jpg","DIVIDE BMP");	
//del by reslim v0.1	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_EQUAL,CUST_IMG_PATH"/MainLCD/Calculator/EQUAL.gif","DIVIDE BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_MAIN,CUST_IMG_PATH"/MainLCD/Calculator/MAP.gif","DIVIDE BMP");
//del by reslim v0.1	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_MP,CUST_IMG_PATH"/MainLCD/Calculator/MP.JPG","DIVIDE BMP");
//del by reslim v0.1	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_MM,CUST_IMG_PATH"/MainLCD/Calculator/Mm.JPG","DIVIDE BMP");
//del by reslim v0.1	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_MC,CUST_IMG_PATH"/MainLCD/Calculator/MC.JPG","DIVIDE BMP");
//del by reslim v0.1	ADD_APPLICATION_IMAGE2(IMG_ID_CALC_MR,CUST_IMG_PATH"/MainLCD/Calculator/MR.JPG","DIVIDE BMP");
#endif	
#else
//#ifdef __MMI_TOUCH_CALCULATOR__
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_MAIN,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_MAIN.GIF","MAIN TOUCH CALC BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_KEY0,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_KEY0.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_KEY1,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_KEY1.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_KEY2,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_KEY2.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_KEY3,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_KEY3.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_KEY4,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_KEY4.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_KEY5,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_KEY5.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_KEY6,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_KEY6.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_KEY7,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_KEY7.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_KEY8,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_KEY8.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_KEY9,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_KEY9.BMP","TOUCH CALC KEY BMP");

	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_KEY_DOT,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_DOT.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_KEY_EQUAL,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_EQUAL.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_PLUS,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_PLUS.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_MINUS,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_SUBTRACT.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_MULTIPLY,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_MULTI.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_DIVIDE,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_DIV.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_MP,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_MP.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_MM,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_MM.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_MC,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_MC.BMP","TOUCH CALC KEY BMP");
	ADD_APPLICATION_IMAGE2(IMG_ID_TOUCH_CALC_MR,CUST_IMG_PATH"/MainLCD/Calculator/Touch/TH_CALC_MR.BMP","TOUCH CALC KEY BMP");
#endif
}
#endif

/*****************************************************************************
 * FUNCTION
 *  PopulateEbook
 * DESCRIPTION
 *  To populate the Ebook Resources
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
#if defined (__MMI_EBOOK_READER__)
void PopulateEbook(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_OPEN_CARD_DRV, "Card Memory", "Bookshelf Screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_OPEN_PRIVATE_DRV, "Phone Memory", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(EBOOK_TITLE_BOOKSHELF, "Bookshelf", "Bookshelf Screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(EBOOK_BKSLF_OPTION, "Bookshelf Options", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_REFRESH_EBOOK,"CleanUp Bookshelf","Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_OPEN_EBOOK, "Open E-Book", "Bookshelf Option Screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_RENAME_BOOK_OPTIONS, "Rename", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_INFO_EBOOK, "Book Info", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_DELETE_EBOOK, "Delete E-Book", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_DELETE_ALL_EBOOK, "Delete All E-Books", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_RESTORE_DEFAULT, "Apply Settings", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_HELP_EBOOK, "Help", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_EBOOK, "Global Settings", "Bookshelf Option Screen");

	
	ADD_APPLICATION_STRING2(STR_EBOOK_MEMORY_LESS,"Not Enough Memory","Bookshelf Option Screen");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_EBOOK_LIST_LSK_EDIT,"Edit","Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_BOOK_MARK_NAME_IS_EMPTY, "Empty Bookmark", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_CONFIRM_SAVE_SETTINGS, "Do you want to save the settings?", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_NO_BOOK_MARK_PRESENT, "Bookmark Name list is Empty", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_EBOOK_FORMAT_NOT_SUPPORTED,"Internal Pdb File Format Is invalid","Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_EBOOK_SEARCH_UN_SUCESSFULL, "Searched string not found", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_ERROR_OPENINIG_BOOK, "Error Occured While Opening the Book", "Bookshelf Option Screen");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ADD_TO_BOOK_SHELF,"This book is not added in bookshelf.Add Now?","Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_ADD_TO_EBOOK_FOLDER,"Book not present in Ebook Folder, do you want to move and open?","Bookshelf Option Screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_OPEN_BOOK_ERROR, "Error Occured While Opening the File", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_FONT_STYLE, "Font Style", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_FONT_SIZE, "Font Size", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_SCROLL_BY, "Scroll By", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_AUTO_SCROLL, "Auto Scroll", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_AUTO_SCROLL_SPEED, "Auto Scroll Speed", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_FULL_SCREEN, "Full Screen", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_ENCODING_FORMAT, "Encoding Format", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_FONT_STYLE_BOLD, "Bold", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_FONT_STYLE_ITALIC, "Italic", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_FONT_STYLE_NORMAL, "Normal", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_FONT_SIZE_LARGE, "Large", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_FONT_SIZE_SMALL, "Small", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_FONT_SIZE_MEDIUM, "Medium", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_SCROLL_BY_PAGE, "Page", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_SCROLL_BY_LINE, "Line", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_AUTO_SCROLL_ON, "On", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_AUTO_SCROLL_OFF, "Off", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_FULL_SCREEN_ON, "On", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_FULL_SCREEN_OFF, "Off", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_AUTO_SCROLL_SPEED_1, "1", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_AUTO_SCROLL_SPEED_2, "2", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_AUTO_SCROLL_SPEED_3, "3", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_AUTO_SCROLL_SPEED_4, "4", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_AUTO_SCROLL_SPEED_5, "5", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_SELECT_MEMORY, "Select Memory", "Bookshelf Option Screen");

    ADD_APPLICATION_STRING2(STR_EBOOK_WAIT, "Ebook: Please Wait!!!", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SUCESSFULL_RESTORE_DEFAULT, "Settings Applied", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_UNSUCESSFULL_RESTORE_DEFAULT,"Error Occured while Restoring the default settings.","Bookshelf Option Screen");

    ADD_APPLICATION_STRING2(STR_SETTINGS_PHONE_MEMORY, "Phone Memory", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SETTINGS_CARD_MEMORY, "Card Memory", "Bookshelf Option Screen");

    ADD_APPLICATION_STRING2(STR_DELETED, "Deleted", "Deleted");
    ADD_APPLICATION_STRING2(STR_MAX_BOOK_MARK, "Maximum Number of Bookmarks Added", "Deleted");
 
	ADD_APPLICATION_STRING2(STR_EBOOK_TOUCH_SCREEN_HELP,"How to use Shorcuts\n\nUse Key 7 to Toggle the auto scroll.\n\nUse Key 9 to Toggle Full Screen Mode.\n\nUse Key 1 for Help.\n\nUse Key 2 to Add a bookmark.\n\nUse Key 3 to go to Find Screen.\n\nUse Key 4 to go to Jump To Screen.\n\nUse Key 5 to Goto Bookmark.\n\nUse Key 6 for Find Next.\n\nUse Key 8 to Remove Bookmark.\n\nUse Key 0 to go to Book Setting Screen","Ebook Help Description");
	 ADD_APPLICATION_STRING2(STR_INCOMPLETE_SEARCH_STRING, "Search String is Incomplte", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_FIND_EBOOK, "Find", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_FIND_NEXT_EBOOK, "Find Next", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_JUMP_EBOOK, "Jump To", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_GOTO_BOOKMARK_EBOOK, "Go To Bookmark", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_ADD_BOOKMARK_EBOOK, "Add Bookmark", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_RMV_BOOKMARK_EBOOK, "Remove Bookmark", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_BOOK_INFO_EBOOK, "Book Info", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_BOOK_SETTINGS_EBOOK, "Settings", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_CLOSE_EBOOK, "Close", "Bookshelf Option Screen");

    ADD_APPLICATION_STRING2(STR_FULL_SCREEN_OFF, "Full Screen Off", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_FULL_SCREEN_ON, "Full Screen On", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_AUTO_SCROLL_ON, "Auto Scroll On", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_AUTO_SCROLL_OFF, "Auto Scroll Off", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_WAIT_STRING, "Please Wait", "Bookshelf Option Screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(EBOOK_DRIVE_SELECT_TYPE, "Select Drive", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SRCH_CASE_EBOOK, "Case Sensitive", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_SRCH_CASEIN_EBOOK, "Case Insensitive", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_JMP_BEG_EBOOK, "Beginning", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_JMP_MID_EBOOK, "Middle", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_JMP_END_EBOOK, "End", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_JMP_PERCENTAGE_EBOOK, "Enter Percentage", "Bookshelf Option Screen");

//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_USB_IS_PLUGGED_IN,"Usb Plugged in","Bookshelf Option Screen");

    ADD_APPLICATION_STRING2(STR_EBOOK_BOOK_INFO_TITLE,"Title:","Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_EBOOK_BOOK_INFO_SIZE,"Size:","Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_EBOOK_BOOK_INFO_FORMAT,"Format :","Bookshelf Option Screen");


	//ADD_APPLICATION_STRING2(STR_CHANGE_CARD_TO_PHONE,"Do you want the default memory location to be changed from card to phone?","Bookshelf Option Screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_CHANGE_CARD_TO_PHONE,"Change E-Book Bookshelf location from card to phone?","Bookshelf Option Screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_CHANGE_PHONE_TO_CARD,"Change E-Book Bookshelf location from phone to card?","Bookshelf Option Screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_MEMORY_TYPE_CHANGED, "Memory Type Changed", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_RESTORE_DEFAULT_SETTINGS, "Apply Settings?", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_DELETE_ALL_EBOOK_CONFIRM, "Delete All E-Books?", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_PERCENTAGE_SCREEN, "Enter Percentage", "Bookshelf Option Screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_DELETE_EBOOK_CONFIRM, "Delete E-Book?", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_DUPLICATE_BOOK_MARK,"Overwrite the previous bookmark with the same name?","Bookshelf Option Screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_OPEN_BOOK_OPTION, "Open Book Options", "Bookshelf Option Screen");

    ADD_APPLICATION_STRING2(STR_BOOK_TOO_SMALL, "Book size too small", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_WRONG_PERCENTAGE_VALUE,"Percentage entered must be in the Range 1-100","Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_DELETE_BOOK_MARK, "Delete the Bookmark?", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING2(STR_EBOOK_BOOKMARK_ADDED, "Bookmark Added", "Bookshelf Option Screen");
 
    ADD_APPLICATION_STRING2(STR_BOOK_SHELF_REFRESHED, "Bookshelf Cleaned", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_EBOOK_SEARCH_INVALID, "Incorrect Encoding", "Bookshelf Option Screen");
    ADD_APPLICATION_STRING(MAIN_MENU_EBOOK_TEXT, "E-Book Reader");

	ADD_APPLICATION_STRING2(STR_EBOOK_DEFAULT_BOOKMARK, "Bookmark ", "Default Bookmark name");
	ADD_APPLICATION_STRING2(STR_EBOOK_DELETE_INQUIRE, "Delete ebook?", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_EBOOK_QUESTION_SIGN, "?", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_EBOOK_CONFIRM_JUMP, "Confirm jump?", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_EBOOK_WAIT_FOR_MOVING_FILE, "Ebook: Moving file ", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_SETTINGS_LEFT_ALIGNMENT, "Left", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_SETTINGS_RIGHT_ALIGNMENT, "Right", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_SETTINGS_ALIGNMENT_STYLE, "Alignment", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_EBOOK_BOOKMARK_ADD_FAIL, "Bookmark Added Fail", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_EBOOK_IS_INITIALIZING, "The Ebook is initializing, please wait", "The Ebook is initializing, please wait");

	ADD_APPLICATION_IMAGE2(MAIN_MENU_EBOOK_ICON,CUST_IMG_PATH"/MainLCD/SubMenu/Ebook/EbookAppIcon.gif","Icon for Ebook Reader Application.");
	ADD_APPLICATION_IMAGE2(IMAGE_EBOOK_WAIT,CUST_IMG_PATH"/MainLCD/UIElement/progress/progress.gif", "Image Diaplayed in Please Wait Screen");
    /*
	ADD_APPLICATION_MENUITEM((MENU_ID_EBK_BOOKSHELF,IDLE_SCREEN_MENU_ID,0,
			SHOW, MOVEABLEWITHINPARENT, DISP_LIST,MAIN_MENU_EBOOK_TEXT,IMG_GLOBAL_OPTIONS));
     */
    ADD_APPLICATION_MENUITEM((MAIN_MENU_EBOOK_MENUID, MAIN_MENU_EXTRA_MENUID, 0,
                              SHOW, MOVEABLEWITHINPARENT | INSERTABLE, DISP_LIST | SHORTCUTABLE,
                              MAIN_MENU_EBOOK_TEXT, MAIN_MENU_EBOOK_ICON));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_BOOKSHELF_OPTION, 0, 9,
                              MENU_ID_EBK_OPEN,
                              MENU_ID_EBK_RFS_OPTION,
                              MENU_ID_EBK_SETTINGS,
                              MENU_ID_EBK_INFO,
                              MENU_ID_EBK_DELETE,
                              MENU_ID_EBK_DELETE_ALL,
							  MENU_FMGR_GEN_OPTION_SORT,
                              MENU_ID_EBK_RESTORE_DEFAULT,
                              MENU_ID_EBK_HELP,
                              SHOW, MOVEABLEWITHINPARENT | INSERTABLE, DISP_LIST,
                              MAIN_MENU_EBOOK_TEXT, MAIN_MENU_EBOOK_ICON));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION, 0, 2,
                              MENU_ID_EBK_RFS_OPTION_LIMITED,
                              MENU_ID_EBK_SETTINGS_LIMITED,
                              SHOW, MOVEABLEWITHINPARENT | INSERTABLE, DISP_LIST,
                              MAIN_MENU_EBOOK_TEXT, MAIN_MENU_EBOOK_ICON));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_RFS_OPTION, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_REFRESH_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_DELETE_ALL, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_DELETE_ALL_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_RESTORE_DEFAULT, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_RESTORE_DEFAULT, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_RFS_OPTION_LIMITED, MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_REFRESH_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_SETTINGS_LIMITED, MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SETTINGS_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_OPEN, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_OPEN_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_OPEN_OPTIONS, 0, 10,
                              MENU_ID_EBK_FIND,
                              MENU_ID_EBK_JUMP,
                              MENU_ID_EBK_GOTO_BOOKMARK,
                              MENU_ID_EBK_ADD_BOOKMARK,
                              MENU_ID_EBK_REMOVE_BOOKMARK,
                              MENU_ID_EBK_BOOK_INFO,
                              MENU_ID_EBK_BOOK_SETTINGS,
                              MENU_ID_EBK_BOOK_RESTORE_DEFAULT,
                              MENU_ID_EBK_HELP,
                              MENU_ID_EBK_CLOSE,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_OPEN_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_REDUCED_OPEN_OPTIONS_1, 0, 9,
                              MENU_ID_EBK_FIND,
                              MENU_ID_EBK_GOTO_BOOKMARK,
                              MENU_ID_EBK_ADD_BOOKMARK,
                              MENU_ID_EBK_REMOVE_BOOKMARK,
                              MENU_ID_EBK_BOOK_INFO,
                              MENU_ID_EBK_BOOK_SETTINGS,
                              MENU_ID_EBK_BOOK_RESTORE_DEFAULT,
                              MENU_ID_EBK_HELP,
                              MENU_ID_EBK_CLOSE,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_OPEN_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_REDUCED_OPEN_OPTIONS, 0, 10,
                              MENU_ID_EBK_FIND,
                              MENU_ID_EBK_FIND_NEXT,
                              MENU_ID_EBK_GOTO_BOOKMARK,
                              MENU_ID_EBK_ADD_BOOKMARK,
                              MENU_ID_EBK_REMOVE_BOOKMARK,
                              MENU_ID_EBK_BOOK_INFO,
                              MENU_ID_EBK_BOOK_SETTINGS,
                              MENU_ID_EBK_BOOK_RESTORE_DEFAULT,
                              MENU_ID_EBK_HELP,
                              MENU_ID_EBK_CLOSE,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_OPEN_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_EBOOK_EDIT_BOOK_MARK_NAME, 0, 2,
                              MENU_ID_EBK_DONE_BOOK_MARK,
                              MENU_ID_EBK_INPUT_METHOD_BOOK_MARK,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_FIND_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_DONE_BOOK_MARK, MENU_EBOOK_EDIT_BOOK_MARK_NAME, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_EBOOK_DONE, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_INPUT_METHOD_BOOK_MARK, MENU_EBOOK_EDIT_BOOK_MARK_NAME, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_INPUT_METHOD, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_STRING2(STR_EBOOK_DONE, "Done", "Bookshelf Option Screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_EBOOK_INPUT_METHOD, "Input Method", "Bookshelf Option Screen");

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_OPEN_OPTIONS_1, 0, 11,
                              MENU_ID_EBK_FIND,
                              MENU_ID_EBK_FIND_NEXT,
                              MENU_ID_EBK_JUMP,
                              MENU_ID_EBK_GOTO_BOOKMARK,
                              MENU_ID_EBK_ADD_BOOKMARK,
                              MENU_ID_EBK_REMOVE_BOOKMARK,
                              MENU_ID_EBK_BOOK_INFO,
                              MENU_ID_EBK_BOOK_SETTINGS,
                              MENU_ID_EBK_BOOK_RESTORE_DEFAULT,
                              MENU_ID_EBK_HELP,
                              MENU_ID_EBK_CLOSE,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_OPEN_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_FIND, MENU_ID_EBK_OPEN_OPTIONS, 2,
                              MENU_ID_EBK_CASE_SENSITIVE,
                              MENU_ID_EBK_CASE_INSENSITIVE,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_FIND_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_FIND_NEXT, MENU_ID_EBK_OPEN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_FIND_NEXT_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_CASE_SENSITIVE, MENU_ID_EBK_FIND, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SRCH_CASE_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_BOOK_RESTORE_DEFAULT, MENU_ID_EBK_FIND, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_RESTORE_DEFAULT, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_CASE_INSENSITIVE, MENU_ID_EBK_FIND, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SRCH_CASEIN_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_JUMP, MENU_ID_EBK_OPEN_OPTIONS, 4,
                              MENU_ID_EBK_BEGINNING,
                              MENU_ID_EBK_MIDDLE,
                              MENU_ID_EBK_END,
                              MENU_ID_EBK_PERCENTAGE,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_JUMP_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_BEGINNING, MENU_ID_EBK_JUMP, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_JMP_BEG_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_MIDDLE, MENU_ID_EBK_JUMP, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_JMP_MID_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_END, MENU_ID_EBK_JUMP, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_JMP_END_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_PERCENTAGE, MENU_ID_EBK_JUMP, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_JMP_PERCENTAGE_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_GOTO_BOOKMARK, MENU_ID_EBK_OPEN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GOTO_BOOKMARK_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_ADD_BOOKMARK, MENU_ID_EBK_OPEN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ADD_BOOKMARK_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_REMOVE_BOOKMARK, MENU_ID_EBK_OPEN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_RMV_BOOKMARK_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_BOOK_INFO, MENU_ID_EBK_OPEN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_BOOK_INFO_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_BOOK_SETTINGS, MENU_ID_EBK_OPEN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_BOOK_SETTINGS_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_CLOSE, MENU_ID_EBK_OPEN_OPTIONS, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_CLOSE_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_INFO, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_INFO_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_DELETE, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_DELETE_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_HELP, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_HELP_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_SETTINGS, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SETTINGS_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_STRING2(STR_SETTINGS_ENCODING_FORMAT_UCS2, "UCS2(Unicode)", "Bookshelf Option Screen");
	ADD_APPLICATION_STRING2(STR_SETTINGS_ENCODING_FORMAT_UTF8, "UTF8", "Bookshelf Option Screen");

#ifdef __MMI_CHSET_BIG5__
    ADD_APPLICATION_STRING2(STR_SETTINGS_ENCODING_FORMAT_BIG5, "BIG5(Traditional Chienese)", "Bookshelf Option Screen");
#endif 

#ifdef __MMI_CHSET_GB2312__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_GB2312,
        "GB2312(Simplified Chienese)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_GB2312__ */ 

#ifdef __MMI_CHSET_TURKISH_ISO__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_TURKISH_ISO,
        "TURKISH ISO(Turkish International)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_TURKISH_ISO__ */ 

#ifdef __MMI_CHSET_TURKISH_WIN__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_TURKISH_WIN,
        "TURKISH WIN(Turkish Windows)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_TURKISH_WIN__ */ 
#ifdef __MMI_CHSET_BALTIC_ISO__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_BALTIC_ISO,
        "BALTIC ISO(Baltic International)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_BALTIC_ISO__ */ 
#ifdef __MMI_CHSET_BALTIC_WIN__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_BALTIC_WIN,
        "BALTIC WIN(Baltic Windows)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_BALTIC_WIN__ */ 
#ifdef __MMI_CHSET_CEURO_ISO__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_CUEURO_ISO,
        "EURO ISO(Euro International)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_CEURO_ISO__ */ 
#ifdef __MMI_CHSET_CEURO_WIN__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_CUEURO_WIN,
        "EURO WIN(Euro Windows)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_CEURO_WIN__ */ 
#ifdef __MMI_CHSET_GREEK_ISO__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_GREEK_ISO,
        "GREEK ISO(Euro International)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_GREEK_ISO__ */ 
#ifdef __MMI_CHSET_GREEK_WIN__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_GREEK_WIN,
        "GREEK WIN(Euro Windows)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_GREEK_WIN__ */ 
#ifdef __MMI_CHSET_HEBREW_ISO__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_HEBREW_ISO,
        "HEBREW ISO(Hebrew International)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_HEBREW_ISO__ */ 
#ifdef __MMI_CHSET_LATIN_ISO__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_LATIN_ISO,
        "LATIN ISO(Latin International)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_LATIN_ISO__ */ 
#ifdef __MMI_CHSET_NORDIC_ISO__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_NORDIC_ISO,
        "NORDIC ISO(Nordic International)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_NORDIC_ISO__ */ 
#ifdef __MMI_CHSET_SEURO_ISO__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_SEURO_ISO,
        "SEURO ISO(Seuro International)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_SEURO_ISO__ */ 
#ifdef __MMI_CHSET_WESTERN_ISO__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_WESTERN_ISO,
        "WESTERN ISO(Western International)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_WESTERN_ISO__ */ 
#ifdef __MMI_CHSET_WESTERN_WIN__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_WESTERN_WIN,
        "ASCII(Western Windows)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_WESTERN_WIN__ */ 
#ifdef __MMI_CHSET_ARABIC_ISO__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_ARABIC_ISO,
        "ARABIC ISO(Arabic International)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_ARABIC_ISO__ */ 
#ifdef __MMI_CHSET_ARABIC_WIN__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_ARABIC_WIN,
        "ARABIC WIN(Arabic Windows)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_ARABIC_WIN__ */ 
#ifdef __MMI_CHSET_HEBREW_WIN__
    ADD_APPLICATION_STRING2(
        STR_SETTINGS_ENCODING_FORMAT_HEBREW_WIN,
        "HEBREW WIN(Hebrew Windows)",
        "Bookshelf Option Screen");
#endif /* __MMI_CHSET_HEBREW_WIN__ */ 

}

#endif /* defined (__MMI_EBOOK_READER__) */ 
#if defined(__MMI_CALORIE__)
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif //#if defined(__MMI_CALORIE__)


#if defined(__MMI_MENSTRUAL__)
/***********************************************************************

	FUNCTION NAME		: PopulateHealthMensisRes(void)

  	PURPOSE				: Populate Menu for Health Mensis Application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

************************************************************************/
void PopulateHealthMensisRes(void)
{
  ADD_APPLICATION_STRING2(STR_SCR_HEALTH_CAPTION,"Health","Health");
	ADD_APPLICATION_STRING2(EXTRA_HEALTH_MENU_MENSTRUAL_STRINGID,"Menstrual","Health->Menstrual");
	#if 0
	ADD_APPLICATION_MENUITEM((EXTRA_HEALTH_MENU_MENSTRUAL,EXTRA_HEALTH_MENU,0,
		SHOW,MOVEABLEACROSSPARENT|SHORTCUTABLE,DISP_LIST,
		EXTRA_HEALTH_MENU_MENSTRUAL_STRINGID,0));
	#endif
	ADD_APPLICATION_STRING2(STR_HEALTH_MENSIS_DATE_CAPTION,"Input", "Input Menstrual Health screen title");
	ADD_APPLICATION_STRING2(STR_SCR_HEALTH_MENSIS_CAPTION,"Menstrual", "Health Menstrual screen title");

	ADD_APPLICATION_STRING2(STR_SCR_HEALTH_MENSIS_PROBABILITY_CAPTION,"Pregnancy", "Pregnancy Probability caption");

	ADD_APPLICATION_STRING2(STR_SCR_HEALTH_MENSIS_PROBABILITY_MESSAGE,"Today's Probability", "Today's Probability");

	ADD_APPLICATION_STRING2(STR_LAST_DATE,"Last Mens Date", "Health Menstrual screen last date");
	ADD_APPLICATION_STRING2(STR_DATE_FORMAT_HEALTH_MENSIS,"(Days)", "Date format Health Screen Mensis");

	ADD_APPLICATION_STRING2(STR_INVALID_AVERAGE_DAY_MESSAGE,"Days: 14 ~ 99", "Invalid avg days message in Health Screen Mensis");

	ADD_APPLICATION_STRING2(STR_CANNOT_NAVIGATE_FURTHER_MESSAGE,"Out of Boundary","Sorry You Cannot Navigate Further Message");

	ADD_APPLICATION_STRING2(STR_AVERAGE_DAYS_HEALTH_MENSIS,"Average Period", "Average Period Days Health Screen Mensis");

//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_AVERAGE_DAYS_HEALTH_NAVIGATE,"Next Screen", "Go To Next Screen");


//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CALENDER_MENSES_CAPTION,"Period", "Menses Calender Period ");

	ADD_APPLICATION_STRING2(STR_CALENDER_MESSAAGE1,"P.P", "Menses Calender message 1");

	ADD_APPLICATION_STRING2(STR_CALENDER_MESSAAGE2,"Menses", "Menses Calender message 2");

	ADD_APPLICATION_STRING2(STR_HEALTH_WRONG_DATE_MSG,"Enter Correct Date"," Notification Msg for wrong date in health mestural.");

	ADD_APPLICATION_IMAGE2(IMG_SCR_HEALTH_MENSIS,CUST_IMG_PATH"/MainLCD/Health/H_MENS.gif","Health screen title image");	

#ifdef __MMI_CALENDAR_V2__
//del by reslim v0.1        ADD_APPLICATION_IMAGE2(IMG_HEALTH_MENSIS_PP,CUST_IMG_PATH"/MainLCD/Calendar/icon/PP.gif","PP color");	

//del by reslim v0.1        ADD_APPLICATION_IMAGE2(IMG_HEALTH_MENSIS_MENSES,CUST_IMG_PATH"/MainLCD/Calendar/icon/MENS.gif","Menses color");	
#endif /*__MMI_CALENDAR_V2__*/ 
}
#endif // #if defined(__MMI_MENSTRUAL__)

#if defined(__MMI_BMI__)
/**************************************************************

	FUNCTION NAME		: PopulateBMIData

  	PURPOSE				: 

	INPUT PARAMETERS	: void 

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PopulateBMIData()
{
	ADD_APPLICATION_STRING2(EXTRA_HEALTH_MENU_BMI_STRINGID,"BMI","Health->BMI");
	#if 0
	ADD_APPLICATION_MENUITEM((EXTRA_HEALTH_MENU_BMI,EXTRA_HEALTH_MENU,0,
		SHOW,MOVEABLEACROSSPARENT|SHORTCUTABLE,DISP_LIST,
		EXTRA_HEALTH_MENU_BMI_STRINGID,0));
	#endif
#if defined(__MMI_MAINLCD_128X160__)
	ADD_APPLICATION_IMAGE2(IMG_BMI_WELCOME_SCREEN,CUST_IMG_PATH"/MainLCD/Health/H_BMI.jpg","BMI welcome image");

	ADD_APPLICATION_IMAGE2(IMG_BMI_RED_FLOWER,CUST_IMG_PATH"/MainLCD/Health/H_RF.gif","red flower image");
	ADD_APPLICATION_IMAGE2(IMG_BMI_BLUE_FLOWER,CUST_IMG_PATH"/MainLCD/Health/H_BF.gif","Blue flower image");
#else
	ADD_APPLICATION_IMAGE2(IMG_BMI_WELCOME_SCREEN,CUST_IMG_PATH"/MainLCD/Health/H_BMI.bmp","BMI welcome image");

	ADD_APPLICATION_IMAGE2(IMG_BMI_RED_FLOWER,CUST_IMG_PATH"/MainLCD/Health/H_RF.gif","red flower image");
	ADD_APPLICATION_IMAGE2(IMG_BMI_BLUE_FLOWER,CUST_IMG_PATH"/MainLCD/Health/H_BF.gif","Blue flower image");
#endif
	ADD_APPLICATION_STRING2(STR_BMI_WELCOME_CAPTION,"BMI", "BMI Welcome title");
	ADD_APPLICATION_STRING2(STR_BMI_INPUT_CAPTION,"Input", "BMI Input title");
	ADD_APPLICATION_STRING2(STR_BMI_RESULT_CAPTION,"BMI Result", "BMI Result title");

	ADD_APPLICATION_STRING2(STR_BMI_MALE_TEXT,"Male", "Male Gender");
	ADD_APPLICATION_STRING2(STR_BMI_FEMALE_TEXT,"Female", "Female Gender");
	ADD_APPLICATION_STRING2(STR_BMI_HEIGHT_TEXT,"Height (Cm)", "Height");
	ADD_APPLICATION_STRING2(STR_BMI_WEIGHT_TEXT,"Weight (Kg)", "Weight");

	ADD_APPLICATION_STRING2(STR_BMI_HEIGHT_ERROR_TEXT,"Height: 30 ~ 300", "Height");
	ADD_APPLICATION_STRING2(STR_BMI_WEIGHT_ERROR_TEXT,"Weight: 1 ~ 300", "Weight");
	ADD_APPLICATION_STRING2(STR_BMI_HEIGHT_WEIGHT_ERROR,"Height: 30 ~ 300 \nWeight: 1 ~ 300 ", "Height & Weight error");
	
	ADD_APPLICATION_STRING2(STR_BMI_BMI_TEXT,"BMI :", "BMI text ");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_BMI_LEVEL,"Level :", "BMI level ");
	ADD_APPLICATION_STRING2(STR_BMI_NORMAL,"Normal", "BMI Normal");
	ADD_APPLICATION_STRING2(STR_BMI_FAT,"Fat", "BMI Fat ");
	ADD_APPLICATION_STRING2(STR_BMI_THIN,"Thin", "BMI Thin ");

}
#endif //#if defined(__MMI_BMI__)

#if defined (__MMI_CURRENCY_CONVERTER__) 

void PopulateCurrencyConvertor(void)
{	
#if !defined(MMI_ULC)
	ADD_APPLICATION_IMAGE2(ORGANIZER_MENU_CURRENCYCONVERTOR_IMAGEID,CUST_IMG_PATH"/MainLCD/Currency/SB_OCUR.gif","Icon for Currency Convertor Menu.");
#endif	
	ADD_APPLICATION_STRING2(ORGANIZER_MENU_CURRENCYCONVERTOR_STRINGID,"Currency Converter","To Go to Currency Convertor Screen.");
	
	#if defined(__MMI_VERSION_2__)
		ADD_APPLICATION_MENUITEM((ORGANIZER_CURRENCYCONVERTOR_MENU,MAIN_MENU_EXTRA_MENUID,0,SHOW,MOVEABLEACROSSPARENT|SHORTCUTABLE,DISP_LIST,
			ORGANIZER_MENU_CURRENCYCONVERTOR_STRINGID,ORGANIZER_MENU_CURRENCYCONVERTOR_IMAGEID));
	#else
	ADD_APPLICATION_MENUITEM((ORGANIZER_CURRENCYCONVERTOR_MENU,MAIN_MENU_ORGANIZER_MENUID,0,SHOW,MOVEABLEACROSSPARENT|SHORTCUTABLE,DISP_LIST,
		ORGANIZER_MENU_CURRENCYCONVERTOR_STRINGID,ORGANIZER_MENU_CURRENCYCONVERTOR_IMAGEID));
	#endif
    
  //ADD_APPLICATION_STRING2(STR_ID_CURRENCYCONVERTOR_RESET,"Reset","Reset Input Text");
       
#if defined (__MMI_CURRENCY_CONVERTER__)
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONVERTOR_RATEINPUT,"Rate Input","Title for CC.");
	ADD_APPLICATION_STRING2(STR_CURRENCYCONVERTOR_RATE,"Rate","Rate.");
	ADD_APPLICATION_STRING2(STR_CURRENCYCONVERTOR_INPUT,"Input Currency","Input Currency");
	ADD_APPLICATION_STRING2(STR_CURRENCYCONVERTOR_LOCAL,"Local","Local");
	ADD_APPLICATION_STRING2(STR_CURRENCYCONVERTOR_FOREIGN,"Foreign","Foreign");

	ADD_APPLICATION_STRING2(ERROR_CURRENCYCONVERTOR_RATE1,"Wrong Entry","error1");

	ADD_APPLICATION_IMAGE2(CURRENCY_CONVERTOR_IMAGE,CUST_IMG_PATH"/MainLCD/Currency/C_RATE.gif","currency convertor image");
	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_INPUT,"Input Currency","Input Currency");
#else
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_RATEINPUT,"Rate Input","Title for CC.");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_RATE,"Rate","Rate.");
	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_INPUT,"Input Currency","Input Currency");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_LOCAL,"Local","Local");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_FOREIGN,"Foreign","Foreign");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_USD,"USD (US Dollars)","USD");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_EUR,"EUR (Euro)","EUR");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_RUB,"RUB (Russia Rubles)","RUB");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_JPY,"JPY (Japanese Yen)","JPY");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_GBP,"GBP (UK Pounds)","GBP");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_HKD,"HKD (Hong Kong Dollar)","HKD");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_CUSTOM1,"Custom1","Custom1");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_CUSTOM2,"Custom2","Custom2");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_CUSTOM3,"Custom3","Custom3");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_USD_ABBR,"USD","USD");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_EUR_ABBR,"EUR","EUR");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_RUB_ABBR,"RUB","RUB");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_JPY_ABBR,"JPY","JPY");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_GBP_ABBR,"GBP","GBP");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_CURRENCYCONV_HKD_ABBR,"HKD","HKD");

//del by reslim v0.1	ADD_APPLICATION_STRING2(ERROR_CURRENCYCONV_RATE1,"Wrong Entry","error1");
//del by reslim v0.1	ADD_APPLICATION_STRING2(ERROR_CURRENCYCONV_RATE2,"Enter exchange rates now?", "No exchange rate is defined.");

	ADD_APPLICATION_IMAGE2(CURRENCY_CONVERTOR_IMAGE,CUST_IMG_PATH"/MainLCD/Currency/C_RATE.gif","currency convertor image");
#endif
}
#endif

#if defined (__MMI_UNIT_CONVERTER__)
void PopulateUnitConvertor()
{

	ADD_APPLICATION_IMAGE2(ORGANIZER_MENU_UNITSCONVERTOR_IMAGEID,CUST_IMG_PATH"/MainLCD/UnitsConverter/SB_OUNIT.gif","Icon for Calculator Units Converter.");
	ADD_APPLICATION_STRING2(ORGANIZER_MENU_UNITSCONVERTOR_STRINGID,"Units Converter","To Go to Units Converter Screen.");

 #ifdef __MMI_EXTENDED_UNIT_CONVERTER__
	#if defined(__MMI_VERSION_2__)
	ADD_APPLICATION_MENUITEM((EXTRA_UNITCONVERTOR_MENU,MAIN_MENU_EXTRA_MENUID,8,		
		ORGANIZER_UNITCONVERTOR_WEIGHT,
		ORGANIZER_UNITCONVERTOR_LENGTH,
		MENU_ID_EXTRA_UNITCONVERTOR_VOLUME,
		MENU_ID_EXTRA_UNITCONVERTOR_TEMPERATURE,
		MENU_ID_EXTRA_UNITCONVERTOR_VELOCITY,
		MENU_ID_EXTRA_UNITCONVERTOR_SHOEW,		
		MENU_ID_EXTRA_UNITCONVERTOR_CLOTHES,
		MENU_ID_EXTRA_UNITCONVERTOR_SURFACE,		
		SHOW,MOVEABLEACROSSPARENT|INSERTABLE|SHORTCUTABLE,DISP_LIST,
		EXTRA_MENU_UNITSCONVERTOR_STRINGID,EXTRA_MENU_UNITSCONVERTOR_IMAGEID));
	
	#else
	ADD_APPLICATION_MENUITEM((ORGANIZER_UNITCONVERTOR_MENU,MAIN_MENU_ORGANIZER_MENUID,8,
		ORGANIZER_UNITCONVERTOR_WEIGHT,
		ORGANIZER_UNITCONVERTOR_LENGTH,
		MENU_ID_EXTRA_UNITCONVERTOR_VOLUME,
		MENU_ID_EXTRA_UNITCONVERTOR_TEMPERATURE,
		MENU_ID_EXTRA_UNITCONVERTOR_VELOCITY,
		MENU_ID_EXTRA_UNITCONVERTOR_SHOEW,		
		MENU_ID_EXTRA_UNITCONVERTOR_CLOTHES,
		MENU_ID_EXTRA_UNITCONVERTOR_SURFACE,		
		SHOW,MOVEABLEACROSSPARENT|INSERTABLE|SHORTCUTABLE,DISP_LIST,
		EXTRA_MENU_UNITSCONVERTOR_STRINGID,EXTRA_MENU_UNITSCONVERTOR_IMAGEID));	
	#endif
 #else 
 #if 0
	#if defined(__MMI_VERSION_2__)
	ADD_APPLICATION_MENUITEM((EXTRA_UNITCONVERTOR_MENU,MAIN_MENU_EXTRA_MENUID,2,
		EXTRA_UNITCONVERTOR_WEIGHT,
		EXTRA_UNITCONVERTOR_LENGTH,SHOW,MOVEABLEACROSSPARENT|INSERTABLE|SHORTCUTABLE,DISP_LIST,
		EXTRA_MENU_UNITSCONVERTOR_STRINGID,EXTRA_MENU_UNITSCONVERTOR_IMAGEID));
	
	#else
	ADD_APPLICATION_MENUITEM((EXTRA_UNITCONVERTOR_MENU,MAIN_MENU_ORGANIZER_MENUID,2,
		EXTRA_UNITCONVERTOR_WEIGHT,
		EXTRA_UNITCONVERTOR_LENGTH,SHOW,MOVEABLEACROSSPARENT|INSERTABLE|SHORTCUTABLE,DISP_LIST,
		EXTRA_MENU_UNITSCONVERTOR_STRINGID,EXTRA_MENU_UNITSCONVERTOR_IMAGEID));
	#endif
#endif	
 #endif
	#if 0
	ADD_APPLICATION_MENUITEM((EXTRA_UNITCONVERTOR_WEIGHT,EXTRA_UNITCONVERTOR_MENU,0,SHOW,MOVEABLEWITHINPARENT,DISP_LIST,STR_UNITCONVERTOR_WEIGHT,0));
	ADD_APPLICATION_MENUITEM((EXTRA_UNITCONVERTOR_LENGTH,EXTRA_UNITCONVERTOR_MENU,0,SHOW,MOVEABLEWITHINPARENT,DISP_LIST,STR_UNITCONVERTOR_LENGTH,0));
#endif
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_WEIGHT,"Weight","weight");	
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_LENGTH,"Length","Length");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_CAPTION,"Unit Converter","UnitConverter");
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_CAPTION1,"Input","Input");
//	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_RESET,"Reset","Reset Input Text");
	
#ifdef __MMI_EXTENDED_UNIT_CONVERTER__
	ADD_APPLICATION_MENUITEM((MENU_ID_EXTRA_UNITCONVERTOR_VOLUME,EXTRA_UNITCONVERTOR_MENU,0,SHOW,NONMOVEABLE,DISP_LIST,STR_ID_UNITCONVERTOR_VOLUME,0));
	ADD_APPLICATION_MENUITEM((MENU_ID_EXTRA_UNITCONVERTOR_TEMPERATURE,EXTRA_UNITCONVERTOR_MENU,0,SHOW,NONMOVEABLE,DISP_LIST,STR_ID_UNITCONVERTOR_TEMPERATURE,0));
	ADD_APPLICATION_MENUITEM((MENU_ID_EXTRA_UNITCONVERTOR_VELOCITY,EXTRA_UNITCONVERTOR_MENU,0,SHOW,NONMOVEABLE,DISP_LIST,STR_ID_UNITCONVERTOR_VELOCITY,0));
	ADD_APPLICATION_MENUITEM((MENU_ID_EXTRA_UNITCONVERTOR_SHOEW,EXTRA_UNITCONVERTOR_MENU,0,SHOW,NONMOVEABLE,DISP_LIST,STR_ID_UNITCONVERTOR_SHOES,0));
	ADD_APPLICATION_MENUITEM((MENU_ID_EXTRA_UNITCONVERTOR_CLOTHES,EXTRA_UNITCONVERTOR_MENU,0,SHOW,NONMOVEABLE,DISP_LIST,STR_ID_UNITCONVERTOR_CLOTHES,0));
	ADD_APPLICATION_MENUITEM((MENU_ID_EXTRA_UNITCONVERTOR_SURFACE,EXTRA_UNITCONVERTOR_MENU,0,SHOW,NONMOVEABLE,DISP_LIST,STR_ID_UNITCONVERTOR_SURFACE,0));

	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_VOLUME,"Volume","Volume");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_MILLILITER,"Milliliter","Milliliter");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_LITER,"Liter","Liter");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_CUBICMM,"Cubic Mm","Cubic Mm");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_CUBICCM,"Cubic Cm","Cubic Cm");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_CUBICMETER,"Cubic Meter","Cubic Meter");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_CUBICINCH,"Cubic Inch","Cubic Inch");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_GALLON,"Gallon","Gallon");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_LIQPT,"Liq. Pt","Liquid Pint");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_LIQOZ,"Liq. Oz","Liquid Ounce");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_FLOZ,"Fluid Oz","Fluid Ounce");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_BARREL,"Barrel","Barrel");
	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_TEMPERATURE,"Temperature","Temperature");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_CELSIUS,"Celsius","Celsius");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_FAHRENHEIT,"Fahrenheit","Fahrenheit");
	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_VELOCITY,"Velocity","Velocity");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_KMH,"Km/H","KmPerHour");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_METERSEC,"Meter/S","MeterPerSecond");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_MPH,"Mile/H","MilePerHour");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_FEETSEC,"Feet/S","FeelPerSecond");

//del by reslim v0.1	ADD_APPLICATION_IMAGE2(IMG_ID_UNITCONVERTOR_RESET, "../../mmi_customer/Images/EmptyImage.gif","Reset RSK image");
	
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_LENGTH_MM,"MM","MM");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_LENGTH_CM,"CM","CM");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_LENGTH_M,"M","M");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_LENGTH_KM,"KM","KM");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_LENGTH_INCH,"Inch","Inch");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_LENGTH_FEET,"Feet","Feet");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_LENGTH_YARD,"Yard","Yard");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_LENGTH_MILES,"Miles","Miles");
	
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_WEIGHT_MG,"MG","MG");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_WEIGHT_G,"G","G");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_WEIGHT_KG,"KG","KG");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_WEIGHT_TON,"Ton","Ton");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_WEIGHT_OUNCE,"Ounce","Ounce");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_WEIGHT_POUND,"Pound","Pound");
	
//del by reslim v0.1	ADD_APPLICATION_STRING2(UC_ERROR_UNCORRECT_INPUT,"Incorrect Input","Incorrect Input");
	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SHOES,"Shoes","Shoes");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SHOES_USA,"USA","USA");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SHOES_EUROPE,"EUROPE","EUROPE");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SHOES_KOREA,"KOREA","KOREA");
	
	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_CLOTHES,"Clothes","Clothes");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_CLOTHES_EUROPE,"Europe","Europe");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_CLOTHES_KOREA1,"Korea1","Korea1");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_CLOTHES_KOREA2,"Korea2","Korea2");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_CLOTHES_JAPAN,"JAPAN","JAPAN");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_CLOTHES_USA,"USA","USA");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_CLOTHES_UK,"UK","UK");
	
	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SURFACE,"Surface","Surface");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SURFACE_ACRE,"Acre","Acre");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SURFACE_HECTARE,"Hectare","Hectare");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SURFACE_SQUARE_IN,"Square Inch","Square Inch");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SURFACE_SQUARE_FEET,"Square Feet","Square feet");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SURFACE_SQUARE_YD,"Square Yard","Square yard");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SURFACE_SQUARE_MILES,"Square Miles","Square miles");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SURFACE_SQUARE_MM,"Square MM","Square MM");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SURFACE_SQUARE_CM,"Square CM","Square CM");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SURFACE_SQUARE_M,"Square M","Square M");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_UNITCONVERTOR_SURFACE_SQUARE_KM,"Square KM","Square KM");

#else 
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_KGPOUND,"Kg<-->Pound","kgtopound");
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_KG,"Kg","kg");
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_POUND,"Pound","Pound");
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_KMMILE,"Km<-->Mile","KmtoMile");
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_MYARD,"M<-->Yard","MtoYard");
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_MFOOT,"M<-->Foot","MtoFoot");
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_CMINCH,"Cm<-->Inch","CmToInch");
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_KM,"Km","Km");
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_MILE,"Mile","Mile");
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_CM,"Cm","Cm");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_INCH,"Inch","Inch");
	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_METER,"Meter","Meter");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_YARD,"Yard","Yard");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_UNITCONVERTOR_FOOT,"Foot","Foot");
	ADD_APPLICATION_STRING2(UC_ERROR_EMPTY_STRING,"Wrong Entry","Number is null");
	ADD_APPLICATION_STRING2(STR_UCONV_KGOUNCE,"Kg<-->Ounce","KG to Ounce");
	ADD_APPLICATION_STRING2(STR_UCONV_OUNCE,"Ounce","Ounce");

#endif
}
#endif

#ifndef __DISABLE_SHORTCUTS_MENU__
void PopulateShortcutsRes(void)
{
	ADD_APPLICATION_MENUITEM((EXTRA_SHORTCUTS_OPTION_MENUID,0,4,
		EXTRA_SHORTCUTS_OPTION_ADD_MENUID,
		EXTRA_SHORTCUTS_OPTION_EDIT_MENUID,
		EXTRA_SHORTCUTS_OPTION_DELETE_MENUID,
		EXTRA_SHORTCUTS_OPTION_DELETEALL_MENUID,
		//EXTRA_SHORTCUTS_OPTION_ENABLE_MENUID,
		//EXTRA_SHORTCUTS_OPTION_DISABLE_MENUID,
		//EXTRA_SHORTCUTS_OPTION_CHAGNE_ORDER_MENUID,
		SHOW,MOVEABLEWITHINPARENT|INSERTABLE,DISP_LIST,
		STR_GLOBAL_OPTIONS,0));

	ADD_APPLICATION_MENUITEM((EXTRA_SHORTCUTS_OPTION_ADD_MENUID,EXTRA_SHORTCUTS_OPTION_MENUID,0,		
		SHOW,MOVEABLEWITHINPARENT|INSERTABLE,DISP_LIST,STR_GLOBAL_ADD,0));
	ADD_APPLICATION_MENUITEM((EXTRA_SHORTCUTS_OPTION_EDIT_MENUID,EXTRA_SHORTCUTS_OPTION_MENUID,0,		
		SHOW,MOVEABLEWITHINPARENT|INSERTABLE,DISP_LIST,STR_GLOBAL_EDIT,0));
	ADD_APPLICATION_MENUITEM((EXTRA_SHORTCUTS_OPTION_DELETE_MENUID,EXTRA_SHORTCUTS_OPTION_MENUID,0,		
		SHOW,MOVEABLEWITHINPARENT|INSERTABLE,DISP_LIST,STR_GLOBAL_DELETE,0));
	ADD_APPLICATION_MENUITEM((EXTRA_SHORTCUTS_OPTION_DELETEALL_MENUID,EXTRA_SHORTCUTS_OPTION_MENUID,0,		
		SHOW,MOVEABLEWITHINPARENT|INSERTABLE,DISP_LIST,STR_GLOBAL_DELETE_ALL,0));	
	#if 0
	ADD_APPLICATION_MENUITEM((EXTRA_SHORTCUTS_OPTION_ENABLE_MENUID,EXTRA_SHORTCUTS_OPTION_MENUID,0,		
		SHOW,MOVEABLEWITHINPARENT|INSERTABLE,DISP_LIST,SHORTCUTS_ENABLE_STRING_ID,0));	
	ADD_APPLICATION_MENUITEM((EXTRA_SHORTCUTS_OPTION_DISABLE_MENUID,EXTRA_SHORTCUTS_OPTION_MENUID,0,		
		SHOW,MOVEABLEWITHINPARENT|INSERTABLE,DISP_LIST,SHORTCUTS_DISABLE_STRING_ID,0));	
	ADD_APPLICATION_MENUITEM((EXTRA_SHORTCUTS_OPTION_CHAGNE_ORDER_MENUID,EXTRA_SHORTCUTS_OPTION_MENUID,0,		
		SHOW,MOVEABLEWITHINPARENT|INSERTABLE,DISP_LIST,SHORTCUTS_CHANGE_ORDER_STRING_ID,0));	
	#endif
	/* Resources for Shortcuts Screen */
	ADD_APPLICATION_STRING2(SHORTCUTS_TITLE_STRING_ID,"Shortcuts","Title displayed in Shortcuts Application.");

//del by reslim v0.1	ADD_APPLICATION_IMAGE2(SHORTCUTS_TITLE_IMAGE_ID,CUST_IMG_PATH"/MainLCD/TitleBar/TB_SC.gif","Icon for Title displayed in Shortcuts Application.");

	/* Resources for Options Screen */
//del by reslim v0.1	ADD_APPLICATION_IMAGE2(SHORTCUTS_OPTIONS_EDIT_IMAGE_ID,CUST_IMG_BASE_PATH"/EmptyImage.gif","Image for Edit Button displayed in Shortcuts Screen Menu.");
//del by reslim v0.1	ADD_APPLICATION_STRING2(SHORTCUTS_OPTIONS_EDIT_STRING_ID,"Edit","Edit Button displayed in Shortcuts Screen Menu.");
	ADD_APPLICATION_STRING2(SHORTCUTS_HINT_STRING_ID,"Press UP key in IDLE to use","Hint message for shortcut");
	#if 0
	ADD_APPLICATION_STRING2(SHORTCUTS_ENABLE_STRING_ID,"Enable","Enable a shortcut item");
	ADD_APPLICATION_STRING2(SHORTCUTS_DISABLE_STRING_ID,"Disable","Disable a shortcut item");
//del by reslim v0.1	ADD_APPLICATION_STRING2(SHORTCUTS_FULL_MSG_STRING_ID,"Full","Message to User:- full Shortcut");
	ADD_APPLICATION_STRING2(SHORTCUTS_CHANGE_ORDER_STRING_ID,"Change Order","change shortcuts order");
	#endif

}
#endif

#if defined(__MMI_EBOOK_READER__)

void PopulateEBOOKMenu(void)
{
    ADD_APPLICATION_MENUITEM((MAIN_MENU_EBOOK_MENUID, MAIN_MENU_FUNANDGAMES_MENUID, 0,
                SHOW, MOVEABLEWITHINPARENT | INSERTABLE, DISP_LIST,
                MAIN_MENU_EBOOK_TEXT, MAIN_MENU_TITLE_EBOOK_ICON));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_BOOKSHELF_OPTION, 0, 9,
                MENU_ID_EBK_OPEN,
                MENU_ID_EBK_RFS_OPTION,
                MENU_ID_EBK_SETTINGS,
                MENU_ID_EBK_INFO,
                MENU_ID_EBK_DELETE,
                MENU_ID_EBK_DELETE_ALL,
				MENU_FMGR_GEN_OPTION_SORT,
                MENU_ID_EBK_RESTORE_DEFAULT,
                MENU_ID_EBK_HELP,
                SHOW, MOVEABLEWITHINPARENT | INSERTABLE, DISP_LIST,
                MAIN_MENU_EBOOK_TEXT, MAIN_MENU_TITLE_EBOOK_ICON));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION, 0, 2,
                MENU_ID_EBK_RFS_OPTION_LIMITED,
                MENU_ID_EBK_SETTINGS_LIMITED,
                SHOW, MOVEABLEWITHINPARENT | INSERTABLE, DISP_LIST,
                MAIN_MENU_EBOOK_TEXT, MAIN_MENU_TITLE_EBOOK_ICON));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_RFS_OPTION, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_REFRESH_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_DELETE_ALL, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_DELETE_ALL_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_RESTORE_DEFAULT, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_RESTORE_DEFAULT, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_RFS_OPTION_LIMITED, MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_REFRESH_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_SETTINGS_LIMITED, MENU_ID_EBK_LIMITED_BOOKSHELF_OPTION, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SETTINGS_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_OPEN, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_OPEN_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_OPEN_OPTIONS, 0, 10,
                MENU_ID_EBK_FIND,
                MENU_ID_EBK_JUMP,
                MENU_ID_EBK_GOTO_BOOKMARK,
                MENU_ID_EBK_ADD_BOOKMARK,
                MENU_ID_EBK_REMOVE_BOOKMARK,
                MENU_ID_EBK_BOOK_INFO,
                MENU_ID_EBK_BOOK_SETTINGS,
                MENU_ID_EBK_BOOK_RESTORE_DEFAULT,
                MENU_ID_EBK_HELP,
                MENU_ID_EBK_CLOSE,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_OPEN_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_REDUCED_OPEN_OPTIONS_1, 0, 9,
                MENU_ID_EBK_FIND,
                MENU_ID_EBK_GOTO_BOOKMARK,
                MENU_ID_EBK_ADD_BOOKMARK,
                MENU_ID_EBK_REMOVE_BOOKMARK,
                MENU_ID_EBK_BOOK_INFO,
                MENU_ID_EBK_BOOK_SETTINGS,
                MENU_ID_EBK_BOOK_RESTORE_DEFAULT,
                MENU_ID_EBK_HELP,
                MENU_ID_EBK_CLOSE,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_OPEN_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_REDUCED_OPEN_OPTIONS, 0, 10,
                MENU_ID_EBK_FIND,
                MENU_ID_EBK_FIND_NEXT,
                MENU_ID_EBK_GOTO_BOOKMARK,
                MENU_ID_EBK_ADD_BOOKMARK,
                MENU_ID_EBK_REMOVE_BOOKMARK,
                MENU_ID_EBK_BOOK_INFO,
                MENU_ID_EBK_BOOK_SETTINGS,
                MENU_ID_EBK_BOOK_RESTORE_DEFAULT,
                MENU_ID_EBK_HELP,
                MENU_ID_EBK_CLOSE,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_OPEN_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_EBOOK_EDIT_BOOK_MARK_NAME, 0, 2,
                MENU_ID_EBK_DONE_BOOK_MARK,
                MENU_ID_EBK_INPUT_METHOD_BOOK_MARK,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_FIND_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_DONE_BOOK_MARK, MENU_EBOOK_EDIT_BOOK_MARK_NAME, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_EBOOK_DONE, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_INPUT_METHOD_BOOK_MARK, MENU_EBOOK_EDIT_BOOK_MARK_NAME, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GLOBAL_INPUT_METHOD, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_OPEN_OPTIONS_1, 0, 11,
                MENU_ID_EBK_FIND,
                MENU_ID_EBK_FIND_NEXT,
                MENU_ID_EBK_JUMP,
                MENU_ID_EBK_GOTO_BOOKMARK,
                MENU_ID_EBK_ADD_BOOKMARK,
                MENU_ID_EBK_REMOVE_BOOKMARK,
                MENU_ID_EBK_BOOK_INFO,
                MENU_ID_EBK_BOOK_SETTINGS,
                MENU_ID_EBK_BOOK_RESTORE_DEFAULT,
                MENU_ID_EBK_HELP,
                MENU_ID_EBK_CLOSE,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_OPEN_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_FIND, MENU_ID_EBK_OPEN_OPTIONS, 2,
                MENU_ID_EBK_CASE_SENSITIVE,
                MENU_ID_EBK_CASE_INSENSITIVE,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_FIND_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_FIND_NEXT, MENU_ID_EBK_OPEN_OPTIONS, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_FIND_NEXT_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_CASE_SENSITIVE, MENU_ID_EBK_FIND, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SRCH_CASE_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_BOOK_RESTORE_DEFAULT, MENU_ID_EBK_FIND, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_RESTORE_DEFAULT, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_CASE_INSENSITIVE, MENU_ID_EBK_FIND, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SRCH_CASEIN_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_JUMP, MENU_ID_EBK_OPEN_OPTIONS, 4,
                MENU_ID_EBK_BEGINNING,
                MENU_ID_EBK_MIDDLE,
                MENU_ID_EBK_END,
                MENU_ID_EBK_PERCENTAGE,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_JUMP_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_BEGINNING, MENU_ID_EBK_JUMP, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_JMP_BEG_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_MIDDLE, MENU_ID_EBK_JUMP, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_JMP_MID_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_END, MENU_ID_EBK_JUMP, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_JMP_END_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_PERCENTAGE, MENU_ID_EBK_JUMP, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_JMP_PERCENTAGE_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_GOTO_BOOKMARK, MENU_ID_EBK_OPEN_OPTIONS, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_GOTO_BOOKMARK_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_ADD_BOOKMARK, MENU_ID_EBK_OPEN_OPTIONS, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_ADD_BOOKMARK_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_REMOVE_BOOKMARK, MENU_ID_EBK_OPEN_OPTIONS, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_RMV_BOOKMARK_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_BOOK_INFO, MENU_ID_EBK_OPEN_OPTIONS, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_BOOK_INFO_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_BOOK_SETTINGS, MENU_ID_EBK_OPEN_OPTIONS, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_BOOK_SETTINGS_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_CLOSE, MENU_ID_EBK_OPEN_OPTIONS, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_CLOSE_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_INFO, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_INFO_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_DELETE, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_DELETE_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_HELP, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_HELP_EBOOK, IMG_GLOBAL_OPTIONS));

    ADD_APPLICATION_MENUITEM((MENU_ID_EBK_SETTINGS, MENU_ID_EBK_BOOKSHELF_OPTION, 0,
                SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_SETTINGS_EBOOK, IMG_GLOBAL_OPTIONS));
		/* ebook files */
	ADD_APPLICATION_MENUITEM((MITEM_FMGR_EBOOK_TYPE_OPTIONS,0,9,
				MENU_FMGR_EBK_OPTION_OPEN, 
				MENU_FMGR_GEN_OPTION_FORWARD,
				MENU_FMGR_GEN_OPTION_DETAIL,
				MENU_FMGR_GEN_OPTION_RENAME,
				MENU_FMGR_GEN_OPTION_COPY, 
				MENU_FMGR_GEN_OPTION_MOVE, 
				MENU_FMGR_GEN_OPTION_DELETE, 
				MENU_FMGR_GEN_OPTION_DELETE_ALL, 
				MENU_FMGR_GEN_OPTION_SORT,
				SHOW, NONMOVEABLE, DISP_LIST, STR_FMGR_GEN_OPTION,0));

	ADD_APPLICATION_MENUITEM((MITEM_FMGR_EBOOK_NO_FWD_TYPE_OPTIONS,0,8,
				MENU_FMGR_EBK_OPTION_OPEN, 
				MENU_FMGR_GEN_OPTION_DETAIL,
				MENU_FMGR_GEN_OPTION_RENAME,
				MENU_FMGR_GEN_OPTION_COPY, 
				MENU_FMGR_GEN_OPTION_MOVE, 
				MENU_FMGR_GEN_OPTION_DELETE, 
				MENU_FMGR_GEN_OPTION_DELETE_ALL, 
				MENU_FMGR_GEN_OPTION_SORT,
				SHOW, NONMOVEABLE, DISP_LIST, STR_FMGR_GEN_OPTION,0));
		
	ADD_APPLICATION_MENUITEM((MITEM_FMGR_EBOOK_NFO_TYPE_OPTIONS,0,2,
				MENU_FMGR_EBK_OPTION_OPEN, 
				MENU_FMGR_GEN_OPTION_DETAIL, 
				SHOW, NONMOVEABLE, DISP_LIST, STR_FMGR_GEN_OPTION,0));

	ADD_APPLICATION_MENUITEM((MENU_FMGR_EBK_OPTION_OPEN,MITEM_FMGR_GEN_TYPE_OPTIONS,0,
				SHOW, NONMOVEABLE, DISP_LIST, STR_OPEN_EBOOK,0));

    ADD_APPLICATION_MENUITEM((MITEM_FMGR_APP_EBOOK_TYPE_OPTIONS,0,7,
				MENU_FMGR_EBK_OPTION_OPEN, 
				MENU_FMGR_GEN_OPTION_FORWARD,
				MENU_FMGR_GEN_OPTION_DETAIL,
				MENU_FMGR_GEN_OPTION_RENAME,
				MENU_FMGR_GEN_OPTION_DELETE, 
				MENU_FMGR_GEN_OPTION_DELETE_ALL, 
				MENU_FMGR_GEN_OPTION_SORT,
				SHOW, NONMOVEABLE, DISP_LIST, STR_FMGR_GEN_OPTION,0));

	ADD_APPLICATION_MENUITEM((MITEM_FMGR_APP_EBOOK_NO_FWD_TYPE_OPTIONS,0,6,
				MENU_FMGR_EBK_OPTION_OPEN, 
				MENU_FMGR_GEN_OPTION_DETAIL,
				MENU_FMGR_GEN_OPTION_RENAME,
				MENU_FMGR_GEN_OPTION_DELETE, 
				MENU_FMGR_GEN_OPTION_DELETE_ALL, 
				MENU_FMGR_GEN_OPTION_SORT,
				SHOW, NONMOVEABLE, DISP_LIST, STR_FMGR_GEN_OPTION,0));

}

#endif

// Add by kenneth
#if defined (__MMI_NOTEPAD__)
void PopulatateNotePadStrRes(void)
{

/* Add Notepad strins */
  ADD_APPLICATION_STRING2(STR_NOTEPAD_MAIN,"Notepad","NotePad strings");
  ADD_APPLICATION_STRING2(STR_NOTEPAD_EMPTY,"<Empty>","NotePad strings");
  ADD_APPLICATION_STRING2(STR_NOTEPAD_RECORD_LIST,"Notepad List","NotePad strings");
  ADD_APPLICATION_STRING2(STR_NOTEPAD_ADD,"Add","NotePad strings");
  ADD_APPLICATION_STRING2(STR_NOTEPAD_DELETE_ONE_RECORD_QUERY,"Delete one record?","NotePad strings");
  ADD_APPLICATION_STRING2(STR_NOTEPAD_DELETE_ALL_TASK_QUERY,"Delte all record?","NotePad strings");
  ADD_APPLICATION_STRING2(STR_NOTEAD_VIEW,"View","NotePad strings");
  ADD_APPLICATION_STRING2(STR_NOTEPAD_EDIT,"Edit","NotePad strings");
  ADD_APPLICATION_STRING2(STR_NOTEPAD_DELETE,"Delete","NotePad strings");
  ADD_APPLICATION_STRING2(STR_NOTEPAD_DELETE_ALL,"Delte All","NotePad strings");
  ADD_APPLICATION_STRING2(STR_NOTEPAD_EMPTY_RECORD_MSG,"Record is empty","NotePad strings");
  ADD_APPLICATION_STRING2(STR_NOTEPAD_CAP_OPTION,"Option","NotePad strings");
  ADD_APPLICATION_STRING2(STR_NOTEPAD_ABANDON,"Return","NotePad strings");

/* Add Notepad Menu resource */
	ADD_APPLICATION_MENUITEM((ORGANIZER_NOTEPAD_MENU,MAIN_MENU_ORGANIZER_MENUID,0,
				SHOW, NONMOVEABLE, 1, STR_NOTEPAD_MAIN, 0));
	
	ADD_APPLICATION_MENUITEM((MITEM_NOTEPAD_LIST_OPTION_1,0,5,
				MITEM_NOTEPAD_VIEW,
				MITEM_NOTEPAD_ADD,
				MITEM_NOTEPAD_EDIT,
				MITEM_NOTEPAD_DELETE,
				MITEM_NOTEPAD_DELETE_ALL,
				SHOW, NONMOVEABLE, 1, STR_GLOBAL_OPTIONS, IMG_NOTEPAD_CAPTION));

	ADD_APPLICATION_MENUITEM((MITEM_NOTEPAD_LIST_OPTION_2, 0 ,4,
				MITEM_NOTEPAD_VIEW,
				MITEM_NOTEPAD_EDIT,
				MITEM_NOTEPAD_DELETE,
				MITEM_NOTEPAD_DELETE_ALL,
				SHOW, NONMOVEABLE, 1, STR_GLOBAL_OPTIONS, 0));

	ADD_APPLICATION_MENUITEM((MITEM_NOTEPAD_VIEW,MITEM_NOTEPAD_LIST_OPTION_1,0,
				SHOW, NONMOVEABLE, 1, STR_NOTEAD_VIEW, 0));

	ADD_APPLICATION_MENUITEM((MITEM_NOTEPAD_ADD,MITEM_NOTEPAD_LIST_OPTION_1,0,
				SHOW, NONMOVEABLE, 1, STR_NOTEPAD_ADD, 0));

	ADD_APPLICATION_MENUITEM((MITEM_NOTEPAD_EDIT,MITEM_NOTEPAD_LIST_OPTION_1,0,
				SHOW, NONMOVEABLE, 1, STR_NOTEPAD_EDIT, 0));

	ADD_APPLICATION_MENUITEM((MITEM_NOTEPAD_DELETE,MITEM_NOTEPAD_LIST_OPTION_1,0,
				SHOW, NONMOVEABLE, 1, STR_NOTEPAD_DELETE, 0));

	ADD_APPLICATION_MENUITEM((MITEM_NOTEPAD_DELETE_ALL,MITEM_NOTEPAD_LIST_OPTION_1,0,
				SHOW, NONMOVEABLE, 1, STR_NOTEPAD_DELETE_ALL, 0));
	
	ADD_APPLICATION_MENUITEM((MITEM_NOTEPAD_EDIT_OPTION,0,3,
				 MITEM_NOTEPAD_SAVE,
				 MITEM_NOTEPAD_BOPOMOFO,
				 MITEM_NOTEPAD_ABANDON,
				 SHOW, NONMOVEABLE, DISP_LIST,STR_GLOBAL_OPTIONS,0));
	
	ADD_APPLICATION_MENUITEM((MITEM_NOTEPAD_SAVE,MITEM_NOTEPAD_EDIT_OPTION,0,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_GLOBAL_SAVE,0));
	
	ADD_APPLICATION_MENUITEM((MITEM_NOTEPAD_BOPOMOFO,MITEM_NOTEPAD_EDIT_OPTION,0,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_GLOBAL_INPUT_METHOD,0));

	ADD_APPLICATION_MENUITEM((MITEM_NOTEPAD_ABANDON,MITEM_NOTEPAD_EDIT_OPTION,0,
				SHOW, MOVEABLEWITHINPARENT, DISP_LIST,STR_NOTEPAD_ABANDON,0));
  
}
#endif

void PopulateExtraRes(void)
{
	//ADD_APPLICATION_IMAGE2(ORGANIZER_TITLE_IMAGEID,CUST_IMG_PATH"/MainLCD/Titlebar/TB_OR.PBM","Icon for Title of Organizer Screen.");
/* Menu item of Unit Converter */
	#if defined(__MMI_EBOOK_READER__)
	//PopulateEBOOKMenu();
	#endif
	
#if defined (__MMI_UNIT_CONVERTER__) 
 PopulateUnitConvertor();
#endif

/* Menu item of Currency Converter */
#if defined (__MMI_CURRENCY_CONVERTER__) 
	PopulateCurrencyConvertor();
#endif

#if defined(__MMI_BMI__) || defined(__MMI_MENSTRUAL__) || defined(__MMI_CALORIE__)
#if 0
	ADD_APPLICATION_IMAGE2(EXTRA_MENU_HEALTH_IMAGEID,CUST_IMG_PATH"/MainLCD/Health/SB_HEAL.PBM","Icon for Health Menu.");
//del by reslim v0.1	ADD_APPLICATION_STRING2(EXTRA_MENU_HEALTH_STRINGID,"Health","To Go to Health Screen.");
	ADD_APPLICATION_STRING2(STR_SCR_HEALTH_CAPTION,"Health", "Health screen title");
	
	#if defined(__MMI_VERSION_2__)
		ADD_APPLICATION_MENUITEM((EXTRA_HEALTH_MENU,MAIN_MENU_EXTRA_MENUID,HALETH_ENUM_TOTAL,
		#if defined(__MMI_BMI__)
			EXTRA_HEALTH_MENU_BMI,
		#endif
		#if defined(__MMI_MENSTRUAL__)
			EXTRA_HEALTH_MENU_MENSTRUAL,
		#endif
		#if defined(__MMI_CALORIE__)
/* under construction !*/
		#endif
			SHOW,MOVEABLEACROSSPARENT,DISP_LIST,
			EXTRA_MENU_HEALTH_STRINGID,EXTRA_MENU_HEALTH_IMAGEID));
	#else
			ADD_APPLICATION_MENUITEM((EXTRA_HEALTH_MENU,MAIN_MENU_ORGANIZER_MENUID,HALETH_ENUM_TOTAL,
		#if defined(__MMI_BMI__)
			EXTRA_HEALTH_MENU_BMI,
		#endif
		#if defined(__MMI_MENSTRUAL__)
			EXTRA_HEALTH_MENU_MENSTRUAL,
		#endif
		#if defined(__MMI_CALORIE__)
/* under construction !*/
		#endif
			SHOW,MOVEABLEACROSSPARENT,DISP_LIST,
			EXTRA_MENU_HEALTH_STRINGID,EXTRA_MENU_HEALTH_IMAGEID));
	#endif
	#endif
#endif

#if defined(__MMI_APHORISM__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#if defined(__MMI_VERSION_2__)
/* under construction !*/
/* under construction !*/
		#else
/* under construction !*/
/* under construction !*/
		#endif
#endif
	//ADD_APPLICATION_STRING2(ORGANIZER_TITLE_STRINGID,"Organizer","Title of Organizer Screen.");

#ifdef __MMI_CALCULATOR__
	PopulateCalculator();
#endif
#if defined(__MMI_EBOOK_READER__)
	PopulateEbook();
#endif 
#if defined(__MMI_BMI__)
	PopulateBMIData();
#endif

#if defined(__MMI_MENSTRUAL__)
	PopulateHealthMensisRes();
#endif

#if defined(__MMI_CALORIE__)
/* under construction !*/
#endif
#ifndef __DISABLE_SHORTCUTS_MENU__
	PopulateShortcutsRes();
#endif
	#if defined(__MMI_DICTIONARY__)
		PopulateDictionaryData();
		ADD_APPLICATION_MENUITEM((EXTRA_DICTIONARY_MENU,MAIN_MENU_EXTRA_MENUID,0,SHOW,NONMOVEABLE,DISP_LIST,
				STR_DICTIONARY_TITLE,IMG_DICTIONARY_EXTRA_MENU_SCREEN));
		
		
	#endif
  #if defined (__MMI_NOTEPAD__)
  PopulatateNotePadStrRes();
  #endif
}
#endif
