/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

#include "javacall_lcd.h" 

#ifdef __cplusplus
extern "C" {
#endif
#include "kal_release.h"
#include "lcd_sw_inc.h"
#include "csw_lcd.h"
#include "mmi_trace.h"
#include "lcd_if.h"
#include "hal_gouda.h"
#include "sxr_tls.h"
#include "gdi_datatype.h"
#include "gdi_include.h"

extern gdi_handle JvmBaseLayer;
static javacall_pixel* lcdbuffer=NULL;
extern U32 g_jam_lock_screen;
/**
 * The function javacall_lcd_init is called by during Java VM startup, allowing the
 * platform to perform device specific initializations. The function is required to
 * provide the supported screen capabilities:
 * - Display Width
 * - Display Height
 * - Color encoding: Either 32bit ARGB format, 15 bit 565 encoding or 24 bit RGB encoding
 * 
 * \par
 * 
 * Once this API call is invoked, the VM will receive display focus.\n
 * <b>Note:</b> Consider the case where the platform tries to assume control over the
 * display while the VM is running by pausing the Java platform. In this case, the
 * platform is required to save the VRAM screen buffer: Whenever the Java
 * platform is resumed, the stored screen buffers must be restored to original
 * state.
 * 
 * @param screenWidth width of screen
 * @param screenHeight width of screen
 * @param colorEncoding color encoding, one of the following:
 *              -# JAVACALL_LCD_COLOR_RGB565
 *              -# JAVACALL_LCD_COLOR_ARGB
 *              -# JAVACALL_LCD_COLOR_RGB888   
 *              -# JAVACALL_LCD_COLOR_OTHER    
 *                
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_lcd_init(void) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
#if (CHIP_HAS_GOUDA == 1)
    //memset(lcd_layer_data[0].frame_buffer_address,0,LCD_WIDTH*LCD_HEIGHT*2);
    //memset(lcdbuffer,0,LCD_WIDTH*LCD_HEIGHT*2);
    
    gdi_layer_set_active(JvmBaseLayer);
    gdi_layer_get_buffer_ptr(&lcdbuffer);
    if (lcdbuffer == NULL)
        asm("break 1");
    return JAVACALL_OK;
#endif
    return JAVACALL_OK;
}


/**
 * The function javacall_lcd_finalize is called by during Java VM shutdown, 
 * allowing the  * platform to perform device specific lcd-related shutdown
 * operations.  
 * The VM guarantees not to call other lcd functions before calling 
 * javacall_lcd_init( ) again.
 *                
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_lcd_finalize(void){ mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    return JAVACALL_OK;
} 
    
/**
 * Get screen raster pointer
 *
 * @return pointer to video ram mapped memory region of size  
 *         ( LCDSGetScreenWidth() * LCDSGetScreenHeight() )  
 */

javacall_pixel* javacall_lcd_get_screen(int hardwareId,
                                    int* screenWidth,
                                    int* screenHeight,
                                    javacall_lcd_color_encoding_type* colorEncoding){ mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    (void)hardwareId;
    *screenWidth   = LCD_WIDTH;
    *screenHeight  = LCD_HEIGHT;
    *colorEncoding = JAVACALL_LCD_COLOR_RGB565;
#if (CHIP_HAS_GOUDA != 1)
    return (javacall_pixel* )get_lcd_frame_buffer_address();
#else
	//return (javacall_pixel* )lcd_layer_data[0].frame_buffer_address;
	return (javacall_pixel* )lcdbuffer;
#endif
}
    
/**
 * The following function is used to flush the image from the Video RAM raster to
 * the LCD display. \n
 * The function call should not be CPU time expensive, and should return
 * immediately. It should avoid memory bulk memory copying of the entire raster.
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_lcd_flush(int hardwareId) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    (void)hardwareId;
#if (CHIP_HAS_GOUDA != 1)
    BlockWrite_DI(0,0,
                LCD_WIDTH-1,LCD_HEIGHT-1/*, 
                LCD_WIDTH, LCD_HEIGHT*/);
#else
    if(g_jam_lock_screen == FALSE)
    {
	    mci_LcdBlockWrite((javacall_pixel* )lcdbuffer,0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
    }
#endif
    return JAVACALL_OK;
}
    
/**
 * Set or unset full screen mode.
 * 
 * This function should return <code>JAVACALL_FAIL</code> if full screen mode
 * is not supported.
 * Subsequent calls to <code>javacall_lcd_get_screen()</code> will return
 * a pointer to the relevant offscreen pixel buffer of the corresponding screen
 * mode as well s the corresponding screen dimensions, after the screen mode has
 * changed.
 * 
 * @param useFullScreen if <code>JAVACALL_TRUE</code>, turn on full screen mode.
 *                      if <code>JAVACALL_FALSE</code>, use normal screen mode.

 * @retval JAVACALL_OK   success
 * @retval JAVACALL_FAIL failure
 */
javacall_result javacall_lcd_set_full_screen_mode(int hardwareId, javacall_bool useFullScreen) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    (void)hardwareId;
    return JAVACALL_OK;
}

   
/**
 * Flush the screen raster to the display. 
 * This function should not be CPU intensive and should not perform bulk memory
 * copy operations.
 * The following API uses partial flushing of the VRAM, thus may reduce the
 * runtime of the expensive flush operation: It should be implemented on
 * platforms that support it
 * 
 * @param ystart start vertical scan line to start from
 * @param yend last vertical scan line to refresh
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail 
 */
javacall_result javacall_lcd_flush_partial(int hardwareId, int ystart, int yend){ //mmi_trace(1,"J2ME: %s, ystart=%d,yend=%d\n",__FUNCTION__,ystart,yend);
    (void)hardwareId;
    if (ystart < 0)
		ystart = 0;
    if (yend > LCD_HEIGHT-1)
		yend = LCD_HEIGHT-1;
	if (yend < 0)
		yend = 0;
#if (CHIP_HAS_GOUDA != 1)
    BlockWrite_DI(0,ystart,
                LCD_WIDTH-1,yend/*, 
                LCD_WIDTH, LCD_HEIGHT*/);
#else
    if(g_jam_lock_screen == FALSE)
    {
	    mci_LcdBlockWrite((javacall_pixel* )lcdbuffer,0,ystart,LCD_WIDTH-1,yend);
    }
#endif

    return JAVACALL_OK;
}
    
javacall_bool javacall_lcd_reverse_orientation(int hardwareId) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    (void)hardwareId;
    return JAVACALL_OK;
}

void javacall_lcd_handle_clamshell() { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    //Not implemented
}
 
javacall_bool javacall_lcd_get_reverse_orientation(int hardwareId) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    (void)hardwareId;
    return JAVACALL_OK;
}

/**
 * checks the implementation supports native softbutton layer.
 * 
 * @retval JAVACALL_TRUE   implementation supports native softbutton layer
 * @retval JAVACALL_FALSE  implementation does not support native softbutton layer
 */
javacall_bool javacall_lcd_is_native_softbutton_layer_supported () { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    return JAVACALL_FALSE;
}

/**
 * The following function is used to set the softbutton label in the native
 * soft button layer.
 * 
 * @param label the label for the softbutton
 * @param len the length of the label
 * @param index the corresponding index of the command
 * 
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_lcd_set_native_softbutton_label(const javacall_utf16* label,
                                                         int len,
                                                         int index){ mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
     return JAVACALL_OK;
}

int javacall_lcd_get_screen_width(int hardwareId) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    (void)hardwareId;
    return LCD_WIDTH;
}
 
int javacall_lcd_get_screen_height(int hardwareId) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    (void)hardwareId;
    return LCD_HEIGHT;
}

/**
 * get currently enabled hardware display id
 */
int javacall_lcd_get_current_hardwareId() { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    return 0;  
}


/** 
 * Get display device name by id
 * @param hardwareId unique id of hardware screen
 */
char* javacall_lcd_get_display_name(int hardwareId) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
  (void)hardwareId;
  return "csd";
}


/**
 * Check if the display device is primary
 * @param hardwareId unique id of hardware screen
 */
javacall_bool javacall_lcd_is_display_primary(int hardwareId) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    (void)hardwareId;
    return JAVACALL_TRUE;
}

/**
 * Check if the display device is build-in
 * @param hardwareId unique id of hardware screen
 */
javacall_bool javacall_lcd_is_display_buildin(int hardwareId) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    (void)hardwareId; 
    return JAVACALL_TRUE;
}

/**
 * Check if the display device supports pointer events
 * @param hardwareId unique id of hardware screen
 */
javacall_bool javacall_lcd_is_display_pen_supported(int hardwareId) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    (void)hardwareId; 
    return JAVACALL_FALSE;
}

/**
 * Check if the display device supports pointer motion  events
 * @param hardwareId unique id of hardware screen
 */
javacall_bool javacall_lcd_is_display_pen_motion_supported(int hardwareId){ mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
    (void)hardwareId; 
    return JAVACALL_FALSE;
}

/**
 * Get display device capabilities
 * @param hardwareId unique id of hardware screen
 */
int javacall_lcd_get_display_capabilities(int hardwareId) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
  return 0x3ff;
}


/**
 * Get the list of screen ids
 * @param return number of screens 
 * @return the lit of ids 
 */
int g_javacall_display_device_ids[1]={0};
int* javacall_lcd_get_display_device_ids(int* n) { mmi_trace(1,"J2ME: %s\n",__FUNCTION__);
	*n=1;
    return g_javacall_display_device_ids;
}


    
#ifdef __cplusplus
} //extern "C"
#endif


