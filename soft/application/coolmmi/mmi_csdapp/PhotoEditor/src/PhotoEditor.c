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
 *	 PhotoEditor.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  PhotoEditor Source
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

/****************************************************************************
* Include Files                                                                
*****************************************************************************/
#include "mmi_features.h"
#ifdef __MMI_PHOTOEDITOR__

#define __NEWSIMULATOR 

#ifdef MMI_ON_HARDWARE_P
	#include "kal_release.h"
	#include "nvram_data_items.h" 	
#else
	#include "kal_non_specific_general_types.h"
#endif /* MMI_ON_HARDWARE_P */

#include "custmenures.h"
#include "globalconstants.h"				/* key id define */
#include "globaldefs.h"						/* golbal image and string id. */
#include "fat_fs.h"							/* must after globalconstants.h */
#include "gui.h"								/* gui functions */
#include "wgui_categories.h"
#include "historygprot.h"					/* screen history */
#include "unicodexdcl.h"					/* unicode transform functions */
#include "nvramtype.h"						/* nvram data type */
#include "nvramprot.h"						/* nvram access fucntions */	
#include "nvramenum.h"						/* nvram id define */
#include "eventsgprot.h"		
#include "settingdefs.h"					/* st_notifyduration define */
#include "settingprofile.h"				/* success_tone define */
#include "wgui.h"								/* for getdatetime() */
#include "commonscreens.h"					/* displaypopup() */
#include "debuginitdef.h"					/* mmi_assert */
#include "wgui_categories_multimedia.h"
#include "wgui_categories_popup.h"
#include "filemanagerdef.h"
#include "filemanagergprot.h"				/* file path */
#include "filemgr.h"
#include "gdi_include.h"					/* graphic library */

#include "photoeditor.h"
#include "photoeditorgprot.h"
#include "phoedtscrenum.h"
#include "phoedtstrenum.h"
#include "phoedtimgenum.h"
#include "photoart.h"
#include "mmi_trace.h"

#ifdef __PHOEDT_FEATURE_PHOART__

/***************************************************************************** 
* Global Variable
*****************************************************************************/

/***************************************************************************** 
* Global Function
*****************************************************************************/
extern void mmi_phoart_init_app(void);
extern void mmi_scenmast_init_app(void);
extern void mmi_imgtile_init_app(void);
extern void mmi_slidshow_init_app(void);

/*****************************************************************************
* Local Vairable                                                           
*****************************************************************************/
phoedt_cntx_struct g_phoedt_cntx;


/*****************************************************************************
* Local Function                                                           
*****************************************************************************/
/* Photo Editor Applcation */
static void mmi_phoedt_hilight_app(void);
static void mmi_phoedt_entry_app_screen(void);





/*****************************************************************************
*
* Photo Editor Applcation                                                           
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_phoedt_init_app()
* DESCRIPTION
*   init photo editor applcation
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_phoedt_init_app(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
//	SetHiliteHandler(MENU_ID_VDOPLY_APP, mmi_phoedt_hilight_app);
	SetHiliteHandler(MENU_ID_PHOEDT_APP, mmi_phoedt_hilight_app);

#ifdef __PHOEDT_FEATURE_PHOART__
	mmi_phoart_init_app();
#endif

#ifdef __PHOEDT_FEATURE_SCENMAST__
	mmi_scenmast_init_app();
#endif 

#ifdef __PHOEDT_FEATURE_IMGTILE__
	mmi_imgtile_init_app();
#endif

#ifdef __PHOEDT_FEATURE_SLIDSHOW__
	mmi_slidshow_init_app();
#endif
   
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoedt_hilight_app()
* DESCRIPTION
*   hilight phoedt menu item hdlr
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
 void mmi_phoedt_hilight_app(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_phoedt_entry_app_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_phoedt_entry_app_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);



}



/*****************************************************************************
* FUNCTION
*	 mmi_phoedt_entry_app_screen()
* DESCRIPTION
*   entry photo editor applcation
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_phoedt_entry_app_screen(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	U16	item_list[16];
	U16	item_icons[16];
	U16	num_item;
	U8		*guiBuffer;
	U8		*popup_list[16];
	S32	i;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	EntryNewScreen(SCR_ID_PHOEDT_APP, NULL, mmi_phoedt_entry_app_screen, NULL);

	guiBuffer 	= GetCurrGuiBuffer(SCR_ID_PHOEDT_APP);
	num_item		= GetNumOfChild(MENU_ID_PHOEDT_APP);
	
	GetSequenceStringIds(MENU_ID_PHOEDT_APP, item_list);
	GetSequenceImageIds(MENU_ID_PHOEDT_APP, item_icons);
	SetParentHandler(MENU_ID_PHOEDT_APP);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i = 0 ; i < num_item; i++)
		popup_list[i] = NULL;

	ConstructHintsList(MENU_ID_PHOEDT_APP, popup_list);

	ShowCategory52Screen(STR_ID_PHOEDT_APP_NAME,	
								IMG_ID_PHOEDT_APP_ICON,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								num_item, 
								item_list,
								item_icons, 
								(U8**)popup_list, 
								0, 
								0,
								guiBuffer);


	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoedt_load_data()
* DESCRIPTION
*   load photo artist's data from NVRAM
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_phoedt_load_data(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16	error;
   	S8 tmp[30];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* if setting is already loaded, avoid load again to save time */

	ReadRecord(	NVRAM_EF_PHOTO_EDITOR_SETTING_LID, 
					1, 
					(void*)&g_phoedt_cntx.setting_data, 
					NVRAM_EF_PHOTO_EDITOR_SETTING_SIZE, 
					&error);
   	
	/* first time init */
	if(g_phoedt_cntx.setting_data.pa_add_text_string[0] == 0xffffffff)
	{
		memset(g_phoedt_cntx.setting_data.pa_add_text_string, 0, sizeof(g_phoedt_cntx.setting_data.pa_add_text_string));

		AnsiiToUnicodeString(tmp,"Hello!");
		pfnUnicodeStrcpy(g_phoedt_cntx.setting_data.pa_add_text_string, (PS8)tmp);
			
		g_phoedt_cntx.setting_data.pa_text_color 		= 0;
		g_phoedt_cntx.setting_data.pa_border_color 	= 0;
		g_phoedt_cntx.setting_data.pa_shadow_color 	= 0;
		g_phoedt_cntx.setting_data.pa_text_size 		= 1;		/* mediun */
		g_phoedt_cntx.setting_data.pa_text_italic 	= 0;		
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoedt_store_data()
* DESCRIPTION
*   load photo artist's data from NVRAM
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_phoedt_store_data(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16	error;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  	WriteRecord(NVRAM_EF_PHOTO_EDITOR_SETTING_LID, 
  					1, 
  					(void*)&g_phoedt_cntx.setting_data, 
  					NVRAM_EF_PHOTO_EDITOR_SETTING_SIZE, 
  					&error);
}



#endif /* __PHOEDT_FEATURE_PHOART__ */
#endif /* __MMI_PHOTOEDITOR__ */


