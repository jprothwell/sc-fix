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
 *  gdi.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#include "gdi_internal.h"


/*****************************************************************************
 * FUNCTION
 *  gdi_init
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 extern void initLcdFrameBuffer(void);
extern void dm_reset_layer_handle(void);
void gdi_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    static BOOL is_init = FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (is_init == FALSE)
    {
        /* init the gdi global mutex */
        gdi_fast_mutex_init();

        /* init lcd */
        gdi_lcd_init();

        /* init layer */
        gdi_layer_init();
	/*init the frame buffer pointer variable,chenhe*/
	#ifdef MMI_ON_HARDWARE_P
	initLcdFrameBuffer();
	#endif
        /* init 2D engine */
    #if defined(GDI_USING_2D_ENGINE) || defined(GDI_USING_2D_ENGINE_V2)
        gdi_2d_engine_init();
    #endif 

        /* init image */
        gdi_image_init();

        /* init animation */
        gdi_anim_init();

    #if defined(GDI_USING_HW_GIF) || defined(GDI_USING_HW_GIF_V2)
        /* init the hardware gif */
        gdi_image_hwgif_init();
    #endif /* defined(GDI_USING_HW_GIF) || defined(GDI_USING_HW_GIF_V2) */ 

    #if defined(GDI_USING_HW_PNG)
        gdi_image_png_init();
    #endif 

    #if defined(WIN32)
    #if defined(GDI_USING_W32_JPEG) || defined(GDI_USING_W32_PNG)
        w32_image_init();
    #endif 
    #endif /* defined(WIN32) */ 
        /* set flag to true avoid re-init */
        is_init = TRUE;

        /* set Main LCD and base layer active */
        gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
    }
    else //this modify by cuijian,for charging power on
    {
        gdi_layers_free();
        dm_reset_layer_handle();
        gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
    }
}

