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
 *	 SceneMaster.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Scene Master Applcation Source of PhotoEditor
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

#ifdef __PHOEDT_FEATURE_SCENMAST__

/***************************************************************************** 
* Global Variable
*****************************************************************************/

/***************************************************************************** 
* Global Function
*****************************************************************************/

/*****************************************************************************
* Local Vairable                                                           
*****************************************************************************/

/*****************************************************************************
* Local Function                                                           
*****************************************************************************/
/* app related */
static void mmi_scenmast_hilight_app(void);

static void mmi_scenmast_entry_app_screen(void);






/*****************************************************************************
* FUNCTION
*	 mmi_scenmast_init_app()
* DESCRIPTION
*   init scene master applcation
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_scenmast_init_app(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetHiliteHandler(MENU_ID_SCENMAST_APP, mmi_scenmast_hilight_app);
}


/*****************************************************************************
* FUNCTION
*	 mmi_scenmast_hilight_app()
* DESCRIPTION
*   init photo art applcation
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_scenmast_hilight_app(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_scenmast_entry_app_screen, KEY_EVENT_UP);
	SetKeyHandler(mmi_scenmast_entry_app_screen, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}



/*****************************************************************************
* FUNCTION
*	 mmi_phoart_entry_app_screen()
* DESCRIPTION
*   entry photo art applcation
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_scenmast_entry_app_screen(void)
{

}


#endif /* __PHOEDT_FEATURE_SCENMAST__ */
#endif /* __MMI_PHOTOEDITOR__ */


