/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
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
 *  ScreenRotation.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  MMI screen rotation 
 
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#include "mmi_features.h"

#ifdef __MMI_SCREEN_ROTATE__

#include "mmi_data_types.h"
#include "gdi_include.h"
#include "kal_non_specific_general_types.h"
#include "lcd_if.h"
#include "eventsgprot.h"
#include "globaldefs.h"
#include "frameworkstruct.h"
#include "debuginitdef.h"

#include "gdi_include.h"
#include "wgui_categories.h"
#include "idleappdef.h" /* screen_rotation_dummy_id */
#include "timerevents.h"        /* frm_screen_rotation_timer */
#include "eventsdcl.h"  /* startmytimer() */
#include "historygprot.h"       /* displayidlescreen() */
#include "allappgprot.h"        /* setup_ui_wrappers() */

#include "wgui_status_icons.h"

#include "screenrotationgprot.h"

#include "mmitask.h"

#ifndef GDI_USING_LAYER_ROTATE
#error "Hardware does not support rotation"
#endif 

extern U8 MMI_disable_title_shortcut_display;
extern S32 MAIN_LCD_device_width;
extern S32 MAIN_LCD_device_height;
extern void UpdateUITheme(void);
#ifdef MMI_ON_HARDWARE_P		
extern BOOL MCI_LcdSizeSet(INT32 LcdWidth, INT32 LcdHeight);      
#endif /* MMI_ON_HARDWARE_P */	


/***************************************************************************** 
* Define
*****************************************************************************/

/***************************************************************************** 
* Typedef 
*****************************************************************************/

typedef struct
{
    U8 wh_swapped;  /* width-height swapped */
    U8 lcd_layer_rotate;
} frm_screen_rotation_struct;

/***************************************************************************** 
* Local Variable
*****************************************************************************/

static const frm_screen_rotation_struct g_screen_rotation_data[MMI_FRM_SCREEN_ROTATE_MAX_TYPE] = 
{
    {0, GDI_LCD_LAYER_ROTATE_0},
    {1, GDI_LCD_LAYER_ROTATE_90},
    {0, GDI_LCD_LAYER_ROTATE_180},
    {1, GDI_LCD_LAYER_ROTATE_270},
    {0, GDI_LCD_LAYER_ROTATE_0_MIRROR},
    {1, GDI_LCD_LAYER_ROTATE_90_MIRROR},
    {0, GDI_LCD_LAYER_ROTATE_180_MIRROR},
    {1, GDI_LCD_LAYER_ROTATE_270_MIRROR}
};

/* Rotation value of the current screen with mmi_frm_screen_rotate() */
static mmi_frm_screen_rotate_enum g_mmi_frm_screen_rotate = MMI_FRM_SCREEN_ROTATE_0;
static mmi_frm_screen_rotate_enum g_mmi_frm_screen_rotate_new = MMI_FRM_SCREEN_ROTATE_0;

/* Rotation value of the current screen with mmi_frm_set_self_rotation_flag()  (-1 if not set) */
static S32 g_mmi_frm_self_screen_rotate = -1;

/* Rotation value of the previous screen */
static mmi_frm_screen_rotate_enum g_mmi_frm_previous_screen_rotate = MMI_FRM_SCREEN_ROTATE_0;

/***************************************************************************** 
* Local Function
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  mmi_frm_rotate_screen_setup_environment
 * DESCRIPTION
 *  
 * PARAMETERS
 *  data        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_frm_rotate_screen_setup_environment(const frm_screen_rotation_struct *data)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    
    mmi_trace(1," mmi_frm_rotate_screen_setup_environment %d  ",data->wh_swapped);
    
    if (data->wh_swapped)
    {
        MAIN_LCD_device_height = GDI_LCD_WIDTH;
        MAIN_LCD_device_width = GDI_LCD_HEIGHT;
    }
    else
    {
        MAIN_LCD_device_height = GDI_LCD_HEIGHT;
        MAIN_LCD_device_width = GDI_LCD_WIDTH;
    }
    gdi_layer_resize(MAIN_LCD_device_width, MAIN_LCD_device_height);

    /* Setup basic UI global varaibles */
    setup_UI_wrappers();
    /* Re-intialize UI components . */
    wgui_init();
    arrange_status_icons();
    /* Update to current theme setting. */
    UpdateUITheme();
#ifdef MMI_ON_HARDWARE_P		
    /*Setup the size of the LCD in MCI layer.*/
   // MCI_LcdSizeSet(MMC_GetLcdWidth(),MMC_GetLcdHeight());
#endif /* MMI_ON_HARDWARE_P */	
}

/***************************************************************************** 
* Global Variable
*****************************************************************************/

/***************************************************************************** 
* Global Function
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  mmi_frm_screen_rotate
 * DESCRIPTION
 *  
 * PARAMETERS
 *  rotation        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_frm_screen_rotate(mmi_frm_screen_rotate_enum rotation)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (rotation < 0 || rotation >= MMI_FRM_SCREEN_ROTATE_MAX_TYPE)
    {
        return;
    }
    
    g_mmi_frm_previous_screen_rotate = g_mmi_frm_screen_rotate;
    if(g_mmi_frm_screen_rotate == rotation) return;

    if (GDI_LCD_WIDTH != GDI_LCD_HEIGHT &&
        (g_screen_rotation_data[rotation].wh_swapped != g_screen_rotation_data[g_mmi_frm_screen_rotate].wh_swapped))
    {
        g_mmi_frm_screen_rotate = rotation;
	 MMI_disable_title_shortcut_display=1;

        gdi_lcd_set_rotate(g_screen_rotation_data[rotation].lcd_layer_rotate);
        mmi_frm_rotate_screen_setup_environment(&g_screen_rotation_data[g_mmi_frm_screen_rotate]);
    }
    else
    {
        g_mmi_frm_screen_rotate = rotation;
	MMI_disable_title_shortcut_display=0;

        gdi_lcd_set_rotate(g_screen_rotation_data[rotation].lcd_layer_rotate);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_frm_set_self_rotation_flag
 * DESCRIPTION
 *  Sometimes applications changes the rotation of GDI layer by itself without using mmi_frm_screen_rotate() for
 *  some reasons. (For example, when image sensor is rotated, GDI layer for camera preview needs to be rotated, too)
 *  
 *  After screen is switched, mmi_frm_get_previous_screen_rotate() returns the value set in this function.
 * PARAMETERS
 *  rotation        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_frm_set_self_rotation_flag(mmi_frm_screen_rotate_enum rotation)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_frm_self_screen_rotate = (S32) rotation;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_frm_reset_screen_rotation
 * DESCRIPTION
 *  Reset screen rotation and context and reset the rotation of base layer.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_frm_reset_screen_rotation(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MMI_BOOL clear_flag = MMI_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_mmi_frm_self_screen_rotate >= 0)
    {
        g_mmi_frm_previous_screen_rotate = (mmi_frm_screen_rotate_enum) g_mmi_frm_self_screen_rotate;
        g_mmi_frm_self_screen_rotate = -1;
        /* Unnecessary to set 'clear_flag' because the application will do it in its screen exit handler */
    }
    else
    {
        g_mmi_frm_previous_screen_rotate = g_mmi_frm_screen_rotate;
        if (g_mmi_frm_screen_rotate != MMI_FRM_SCREEN_ROTATE_0)
        {
            clear_flag= MMI_TRUE;
        }
    }
    mmi_frm_screen_rotate(MMI_FRM_SCREEN_ROTATE_0);

    /* 
     * Note: after screen is rotated, the content of LCD double buffer is wrong.
     *
     * If backlight was off and the next screen turn on backlight, it will BLT the 
     * content of LCD double buffer to the screen, and the screen is messed.
     *
     * Framework should be responsible to clear the screen to black to avoid blinking 
     * when backlight is turned on.
     */
    if (clear_flag)
    {
        gdi_layer_clear(GDI_COLOR_BLACK);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_frm_is_previous_screen_width_height_swapped
 * DESCRIPTION
 *  Check whether the previous screen has swapped width and height
 * PARAMETERS
 *  void
 * RETURNS
 *  MMI_BOOL
 *****************************************************************************/
MMI_BOOL mmi_frm_is_previous_screen_width_height_swapped(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GDI_LCD_WIDTH != GDI_LCD_HEIGHT && g_screen_rotation_data[g_mmi_frm_previous_screen_rotate].wh_swapped)
    {
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_frm_get_previous_screen_rotate
 * DESCRIPTION
 *  Get the previous screen rotation value
 *  
 *  It is typically used screens like popup dialog because they need to preserve the display of the previous screens.
 * PARAMETERS
 *  void
 * RETURNS
 *  mmi_frm_screen_rotate_enum
 *****************************************************************************/
mmi_frm_screen_rotate_enum mmi_frm_get_previous_screen_rotate(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return g_mmi_frm_previous_screen_rotate;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_frm_is_screen_width_height_swapped
 * DESCRIPTION
 *  Check whether the current screen has swapped width and height
 * PARAMETERS
 *  void
 * RETURNS
 *  MMI_BOOL
 *****************************************************************************/
MMI_BOOL mmi_frm_is_screen_width_height_swapped(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (GDI_LCD_WIDTH != GDI_LCD_HEIGHT && g_screen_rotation_data[g_mmi_frm_screen_rotate].wh_swapped)
    {
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_frm_get_screen_rotate
 * DESCRIPTION
 *  Get the current screen rotation value
 * PARAMETERS
 *  void
 * RETURNS
 *  mmi_frm_screen_rotate_enum
 *****************************************************************************/
mmi_frm_screen_rotate_enum mmi_frm_get_screen_rotate(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return g_mmi_frm_screen_rotate;
}

void mmi_frm_screen_rotate_handler(void)
{
    if(mmi_frm_get_screen_rotate() != g_mmi_frm_screen_rotate_new )
    {
	    mmi_frm_screen_rotate(g_mmi_frm_screen_rotate_new);
		gdi_layer_clear(GDI_COLOR_BLACK);
    }
}

#include "gui.h"
#include "simdetectiongprot.h"
#include "quickaccessmenuitem_def.h"

#include "globalscrenum.h"
//#include "calculator.h"
//#include "unitconvertor.h"
//#include "currencyconvertor.h"
#include "poweronoffdisplaydefs.h"
#include "settingdefs.h"
//#include "calendardef.h"

//#include "organizerdef.h"
#include "factorymodedef.h"
#include "mmi_features_camera.h"
//#include "cameraresdefs.h"
#include "wallpaperdefs.h"
#include "screensaverdefs.h"


void	mmi_frm_screen_rotate_req(mmi_frm_screen_rotate_enum rotate)
	{
	if(g_mmi_frm_screen_rotate != rotate){
		g_mmi_frm_screen_rotate_new = rotate; 
    //Notice camera the slide status, FALSE->ROTATE_0, TRUE->ROTATE_270
		//cam_screen_width_height_swapped = TRUE;

	if(IsScreenPresent(SCR_ID_FM_AUTO_TEST_START) || IsScreenPresent(SCR_ID_FM_ROOT)) return;

    EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
        GoBackHistory();
    }
}


#else /* __MMI_SCREEN_ROTATE__ */ 

int screen_rotation_dummy;  /* Get rid of compile warning */
int mmi_frm_is_screen_width_height_swapped(void)
{
        return 0;
}

#endif /* __MMI_SCREEN_ROTATE__ */ 

