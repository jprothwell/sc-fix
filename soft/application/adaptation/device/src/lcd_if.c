/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2003
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   lcd_if.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Lcd interface between upper application layer and driver
 *   FIXME THAT THE WHOLE CONCEPT OF A DRIVER, WHY THIS LAYER THEN ??????
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
#if 0 // zrx del 20060517
#include "kal_release.h"
#include "stack_common.h"
#include "stack_msgs.h"
#include "app_ltlcom.h"       /* task message communiction */
#include "stacklib.h"
#include "stack_timer.h"      /*stack_timer_struct....definitions*/
#include "drv_comm.h"

#include "gpt_sw.h"
#include "intrctrl.h"
#include "gpio_sw.h"
#endif
#include "lcd_sw_inc.h"
#include "lcd_if.h"
#include "di.h"
#include "mmi_trace.h"
#include "ts.h"
#include "kal_release.h"  //add by weid

#if (CHIP_HAS_GOUDA == 1)
#include "hal_gouda.h"
#include "sxr_tls.h"

#define GDI_LAYER_TOTAL_LAYER_COUNT          (4)
#define GDI_LAYER_OFFSET                  (1024/2)
#endif // (CHIP_HAS_GOUDA == 1)
#if defined(LCM_ROTATE_SUPPORT)
#include "screenrotationgprot.h"
#endif


#if 0    
extern kal_uint8 L1SM_GetHandle(void);
extern void L1SM_SleepEnable(kal_uint8 handle);
extern void L1SM_SleepDisable(kal_uint8 handle);
kal_uint8	lcd_sleep_mode_handler=0xFF;
#endif	


LCD_OPERATION_STATE_ENUM main_lcd_operation_state=LCD_IDLE_STATE;
lcd_frame_update_struct main_lcd_fb_update_para;
#ifdef DUAL_LCD
 extern LCD_Funcs_DI  *SubLCD;
 LCD_OPERATION_STATE_ENUM sub_lcd_operation_state=LCD_IDLE_STATE;
 lcd_frame_update_struct sub_lcd_fb_update_para;
#endif

kal_uint32 lcd_frame_buffer_address;

volatile kal_uint8 current_update_lcd=MAIN_LCD;

kal_uint8 lcd_rotate_value=0;
kal_uint32 current_lcd_hw_update_layer=0;
kal_bool lcd_window_swap_flag=KAL_FALSE;
kal_bool lcd_update_internal_ram=KAL_FALSE;

#ifndef NO_CS_LOGO
#include "lcdd_m.h"

#define NCHAN_16                1
#define IMAGE_WIDTH             128
#define IMAGE_HEIGHT            60
#define IMAGE_FORMAT            LCDD_COLOR_FORMAT_RGB_565

CONST UINT16 g_LogoRgb[(IMAGE_WIDTH*IMAGE_HEIGHT)*NCHAN_16] =
{
#include "logo_128x60_16.dat"
};

/// A frame buffer window containing the logo.
/// The region of interest is the full image.
CONST LCDD_FBW_T g_LogoFbw =
{
    {(UINT16*)&g_LogoRgb, IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_FORMAT},
    {0, 0, IMAGE_WIDTH, IMAGE_HEIGHT}
};

#endif

extern void init_lcd_interface(void);
extern UINT8 ufo_flag;


kal_bool get_lcd_status(void)
{
	return FALSE;
}

void lcd_power_up(void)
{
	
}

void lcd_power_down(void)
{
#ifdef __MMI_NOKIA_STYLE_N800__
        while(hal_GoudaIsActive())
        {
            sxr_Sleep(32);
        }
#endif
}	/* lcd_power_down() */

void swap_lcd_layer_window(kal_uint32 swap_layer)
{
}

void set_lcd_layer_roate(kal_uint32 rotate_layer,kal_uint8 rotate_value)
{
}	/* set_lcd_layer_roate() */

void resume_lcd_layer_rotate(void)
{
}	/* resume_lcd_layer_rotate() */

/*************************************************************************
* FUNCTION
*  config_lcd_layer_window_queue
*
* DESCRIPTION
*     This function is to configure the layer parameters in the layer parameter queue
*     for LCD multi-layer structure.
*
* CALLS
*
* PARAMETERS
*	None
*
* RETURNS
*
*
* GLOBALS AFFECTED
*   lcd_layer_data[].layer_update_queue will be set as KAL_FALSE
**************************************************************************/
void config_lcd_layer_window_queue(void)
{
}	/* config_lcd_layer_window_queue() */

/*************************************************************************
* FUNCTION
*  config_lcd_layer_window
*
* DESCRIPTION
*     This function is to configure the layer parameters for LCD multi-layer structure.
*
* CALLS
*
* PARAMETERS
*  lcd_layer   : configured layer (from LCD_LAYER0 to LCD_LAYER3)
*  layer_data  : the parameters for lcd_layer
*
* RETURNS
*  return KAL_TRUE if configure successful
*
* GLOBALS AFFECTED
*   none
**************************************************************************/
extern lcd_layer_struct	lcd_layer_data[];//2007-7-3 fengzb
kal_bool config_lcd_layer_window(kal_uint8 lcd_layer,lcd_layer_struct *layer_data)
{  /* LCM independent */

//	lcd_frame_buffer_address=layer_data->frame_buffer_address;//2007-7-3 fengzb
  	lcd_layer_data[lcd_layer] = *layer_data;//2007-7-3 fengzb
    

  return TRUE;

}  /* config_lcd_layer_window() */

void config_lcd_layer_offset(kal_uint8 lcd_layer, kal_uint16 layer_offset_x, kal_uint16 layer_offset_y)
{
}	/* config_lcd_layer_offset() */

/*************************************************************************
* FUNCTION
*  config_lcd_roi_window
*
* DESCRIPTION
*     This function configures the ROI window offset and size of LCD interface.
*
* CALLS
*
* PARAMETERS
*  roi_x_offset : ROI X offset
*  roi_y_offset : ROI Y offset
*  roi_column   : ROI width
*  roi_row      : ROI height
*
* RETURNS
*  return KAL_TRUE if configure successful
*
* GLOBALS AFFECTED
*   none
**************************************************************************/
kal_bool config_lcd_roi_window(kal_uint16 roi_offset_x, kal_uint16 roi_offset_y, kal_uint16 roi_column,
                        kal_uint16 roi_row)
{  /* LCM independent */
   return KAL_TRUE;
}  /* config_lcd_roi_window() */

/*************************************************************************
* FUNCTION
*  set_lcd_color_palette
*
* DESCRIPTION
*     This function set the color palette of LCD interface.
*
* CALLS
*
* PARAMETERS
*	color_palette_select	: choose LCD color palette 0 or color palette 1
*	color_palette_addr_ptr : the address ptr that points to the color with offset
*									 (start_index) from the 0th color in color palette
*  start_index : the offset from the 0th color in color palette
*  number_of_color : number of colors in color palette will be set.
*
* RETURNS
*  return KAL_TRUE if configure successful
*
* GLOBALS AFFECTED
*   none
**************************************************************************/
void set_lcd_color_palette(kal_uint8 color_palette_select,kal_uint32 *color_palette_addr_ptr,
								   kal_uint8 start_index, kal_uint8 number_of_color)
{
}	/* set_lcd_color_palette() */

/*************************************************************************
* FUNCTION
*     lcd_LISR
*
* DESCRIPTION
*     LCD interface LISR entry
*
* PARAMETERS
*
* RETURNS
*     None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_LISR(void)
{
}  /* lcd_LISR() */

/*************************************************************************
* FUNCTION
*     lcd_HISR
*
* DESCRIPTION
*     LCD interface HISR entry
*
* PARAMETERS
*
* RETURNS
*     None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_HISR(void)
{
}  /* lcd_HISR() */

/*************************************************************************
* FUNCTION
*     lcd_system_init
*
* DESCRIPTION
*     This function is used to initialize the system correlated parameters of
*     LCD interface
*
* PARAMETERS
*     None
*
* RETURNS
*     None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_system_init(void)
{

	lcd_power_up();
	lcd_power_down();
}  /* lcd_system_init() */

/*************************************************************************
* FUNCTION
*     lcd_init
*
* DESCRIPTION
*     This function is used to initialize the LCD interface and LCD module
*
* PARAMETERS
*     lcd_id   : selection MAIN_LCD or SUB_LCD
*     background_color  :  RGB565 color that LCM will display after initial
*
* RETURNS
*     None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_init(kal_uint8 lcd_id, kal_uint16 background_color)
{

#ifndef DUAL_LCD
   if (main_lcd_operation_state==LCD_IDLE_STATE)
      lcd_system_init();
#else
   if ((main_lcd_operation_state==LCD_IDLE_STATE)&&(sub_lcd_operation_state==LCD_IDLE_STATE))
      lcd_system_init();
#endif


lcd_power_up();
   switch (lcd_id)
   {
      case MAIN_LCD:
         main_lcd_operation_state=LCD_INITIAL_STATE;
	     Init_DI(background_color,0); 
         main_lcd_operation_state=LCD_STANDBY_STATE;
      break;
#ifdef DUAL_LCD
      case SUB_LCD:
         sub_lcd_operation_state=LCD_INITIAL_STATE;
         SubLCD->Init(background_color,0);
         sub_lcd_operation_state=LCD_STANDBY_STATE;
      break;
#endif   /* DUAL_LCD */
      default:
        // ASSERT(0);
      break;
   }

}  /* lcd_init() */

void lcd_present_CS_log()
#ifndef NO_CS_LOGO
{
    Init_DI(0xffff, 0);// Init LCD, add fill white
    //    lcdd_SetStandbyMode(FALSE);

    /// Draw a cool logo.
	while (lcdd_Blit16(&g_LogoFbw, 
        (LCD_WIDTH-IMAGE_WIDTH)>>1, ((IMAGE_HEIGHT<LCD_HEIGHT*0.8)?(LCD_HEIGHT*0.4-IMAGE_HEIGHT/2):0)) != LCDD_ERR_NO);
    lcdd_SetBrightness(7);
}
#else
{}
#endif
/*************************************************************************
* FUNCTION
*     lcd_sleep_in
*
* DESCRIPTION
*     This function is used to let LCD module enter sleep mode
*
* PARAMETERS
*     lcd_id   : selection MAIN_LCD or SUB_LCD
*
* RETURNS
*     None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_sleep_in(kal_uint8 lcd_id)
{
	lcd_power_up();
   switch (lcd_id)
   {
      case MAIN_LCD:
         SleepIn_DI();
         main_lcd_operation_state=LCD_SLEEP_STATE;
      break;
#ifdef DUAL_LCD
      case SUB_LCD:
         SubLCD->SleepIn();
         sub_lcd_operation_state=LCD_SLEEP_STATE;
      break;
#endif   /* DUAL_LCD */
      default:
         //ASSERT(0);
      break;
   }
	lcd_power_down();
}  /* lcd_sleep_in() */

/*************************************************************************
* FUNCTION
*     lcd_sleep_out
*
* DESCRIPTION
*     This function is used to let LCD module leave sleep mode
*
* PARAMETERS
*     lcd_id   : selection MAIN_LCD or SUB_LCD
*
* RETURNS
*     None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_sleep_out(kal_uint8 lcd_id)
{
	lcd_power_up();
   switch (lcd_id)
   {
      case MAIN_LCD:
         SleepOut_DI();
         main_lcd_operation_state=LCD_STANDBY_STATE;
      break;
#ifdef DUAL_LCD
      case SUB_LCD:
         SubLCD->SleepOut();
         sub_lcd_operation_state=LCD_STANDBY_STATE;
      break;
#endif   /* DUAL_LCD */
      default:
        // ASSERT(0);
      break;
   }
   lcd_power_down();
}  /* lcd_sleep_out() */

/*************************************************************************
* FUNCTION
*     lcd_partial_on
*
* DESCRIPTION
*     This function is used to display partial screen on LCD
*
* PARAMETERS
*     lcd_id   : selection MAIN_LCD or SUB_LCD
*     start_line  : the first line to display
*     end_line    : the last line to display
*
* RETURNS
*     None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_partial_on(kal_uint8 lcd_id, kal_uint16 start_line, kal_uint16 end_line)
{
   lcd_power_up();
   switch (lcd_id)
   {
      case MAIN_LCD:
         	PartialOn_DI(start_line,end_line);
      break;
   #ifdef DUAL_LCD
      case SUB_LCD:
		if (SubLCD->PartialOn != NULL)
         	SubLCD->PartialOn(start_line,end_line);
      break;
   #endif   /* DUAL_LCD */
      default:
        // ASSERT(0);
      break;
   }
   lcd_power_down();
}  /* lcd_partial_on() */

/*************************************************************************
* FUNCTION
*     lcd_partial_off
*
* DESCRIPTION
*     This function is used to stop partial display mode
*
* PARAMETERS
*     lcd_id   : selection MAIN_LCD or SUB_LCD
*
* RETURNS
*     None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_partial_off(kal_uint8 lcd_id)
{
	lcd_power_up();
   switch (lcd_id)
   {
      case MAIN_LCD:
         	PartialOff_DI();
      break;
   #ifdef DUAL_LCD
      case SUB_LCD:
		if (SubLCD->PartialOff != NULL)
         	SubLCD->PartialOff();
      break;
   #endif   /* DUAL_LCD */
      default:
        // ASSERT(0);
      break;
   }
   lcd_power_down();
}  /* lcd_partial_on() */

kal_uint8 lcd_partial_display_align_line(kal_uint8 lcd_id)
{
   switch (lcd_id)
   {
      case MAIN_LCD:
         	return (partial_display_align_DI());
      break;
   #ifdef DUAL_LCD
      case SUB_LCD:
		if (SubLCD->partial_display_align != NULL)
         	return (SubLCD->partial_display_align());
         else
         	return 0;
      break;
   #endif   /* DUAL_LCD */
      default:
       //  ASSERT(0);
      break;
   }
   return 0;
} /* lcd_partial_display_align_line() */

/*************************************************************************
* FUNCTION
*     lcd_bright_level
*
* DESCRIPTION
*     This function is used to set the brightness of LCM
*
* PARAMETERS
*     lcd_id      : selection MAIN_LCD or SUB_LCD
*     bright_level: brightness level of LCM
*
* RETURNS
*     None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_bright_level(kal_uint8 lcd_id, kal_uint8 bright_level)
{
	lcd_power_up();
   switch (lcd_id)
   {
      case MAIN_LCD:
         BrightLevel_DI(bright_level);
      break;
#ifdef DUAL_LCD
      case SUB_LCD:
         SubLCD->BrightLevel(bright_level);
      break;
#endif   /* DUAL_LCD */
      default:
       //  ASSERT(0);
      break;
   }
   lcd_power_down();
}  /* lcd_bright_level() */

/*************************************************************************
* FUNCTION
*     lcd_power_on
*
* DESCRIPTION
*     This function turns LCM power on/off
*
* PARAMETERS
*     lcd_id   : selection MAIN_LCD or SUB_LCD
*     on       : power on(KAL_TRUE)/off(KAL_FALSE) LCM
*
* RETURNS
*     None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_power_on(kal_uint8 lcd_id, kal_bool on)
{
	lcd_power_up();
   switch (lcd_id)
   {
      case MAIN_LCD:
         PWRON_DI(on);
      break;
#ifdef DUAL_LCD
      case SUB_LCD:
         SubLCD->PWRON(on);
      break;
#endif   /* DUAL_LCD */
      default:
        // ASSERT(0);
      break;
   }
   lcd_power_down();
}  /* lcd_power_on() */

/*************************************************************************
* FUNCTION
*     lcd_screen_on
*
* DESCRIPTION
*     This function turns LCM screen on/off
*
* PARAMETERS
*     lcd_id   : selection MAIN_LCD or SUB_LCD
*     on       : screen on(KAL_TRUE)/off(KAL_FALSE) LCM
*
* RETURNS
*     None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_screen_on(kal_uint8 lcd_id, kal_bool on)
{
	lcd_power_up();
   switch (lcd_id)
   {
      case MAIN_LCD:
         SCREENON_DI(on);
      break;
#ifdef DUAL_LCD
      case SUB_LCD:
         SubLCD->SCREENON(on);
      break;
#endif   /* DUAL_LCD */
      default:
         //ASSERT(0);
      break;
   }
   lcd_power_down();
}  /* lcd_screen_on() */

/*************************************************************************
* FUNCTION
*     lcd_get_parameter
*
* DESCRIPTION
*     This function returns specified parameter of LCM
*
* PARAMETERS
*     lcd_id   : selection MAIN_LCD or SUB_LCD
*     type     : which parameter will be returned (bias, contrast, linerate or
*                temperature compensation)
*
* RETURNS
*     the value of the specified parameter of LCM
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_uint8 lcd_get_parameter(kal_uint8 lcd_id,lcd_func_type type)
{
	kal_uint8 return_value = 0;

   switch (lcd_id)
   {
      case MAIN_LCD:
         return_value=get_param_number_DI(type);
      break;
#ifdef DUAL_LCD
      case SUB_LCD:
			return_value=SubLCD->get_param_number(type);
      break;
#endif   /* DUAL_LCD */
      default:
       //  ASSERT(0);
      break;
   }
	return return_value;
}  /* lcd_get_parameter() */

/*************************************************************************
* FUNCTION
*     lcd_get_size
*
* DESCRIPTION
*     This function returns the width and height of LCM
*
* PARAMETERS
*     lcd_id      : selection MAIN_LCD or SUB_LCD
*     *lcd_width  : the width of LCM
*     *lcd_height : the height of LCM
*
* RETURNS
*     none
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_get_size(kal_uint8 lcd_id, kal_uint16 *lcd_width, kal_uint16 *lcd_height)
{
   switch (lcd_id)
   {
      case MAIN_LCD:
         GetSize_DI(lcd_width, lcd_height);
      break;
#ifdef DUAL_LCD
      case SUB_LCD:
         SubLCD->GetSize(lcd_width, lcd_height);
      break;
#endif   /* DUAL_LCD */
      default:
        // ASSERT(0);
      break;
   }
}  /* lcd_get_size() */

/*************************************************************************
* FUNCTION
*     lcd_set_bias
*
* DESCRIPTION
*     This function set the bias of LCM
*
* PARAMETERS
*     lcd_id   : selection MAIN_LCD or SUB_LCD
*     *bias    : the bias of LCM
*
* RETURNS
*     none
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_set_bias(kal_uint8 lcd_id, kal_uint8 *bias)
{
	lcd_power_up();
   switch (lcd_id)
   {
      case MAIN_LCD:
         set_bias_DI(*bias);
      break;
#ifdef DUAL_LCD
      case SUB_LCD:
         SubLCD->set_bias(bias);
      break;
#endif   /* DUAL_LCD */
      default:
         //ASSERT(0);
      break;
   }
   lcd_power_down();
}  /* lcd_set_bias() */

/*************************************************************************
* FUNCTION
*     lcd_set_contrast
*
* DESCRIPTION
*     This function set the contrast of LCM
*
* PARAMETERS
*     lcd_id      : selection MAIN_LCD or SUB_LCD
*     *contrast   : the contrast of LCM
*
* RETURNS
*     none
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_set_contrast(kal_uint8 lcd_id, kal_uint8 *contrast)
{
	lcd_power_up();
   switch (lcd_id)
   {
      case MAIN_LCD:
         set_contrast_DI(*contrast);
      break;
#ifdef DUAL_LCD
      case SUB_LCD:
         SubLCD->set_contrast(contrast);
      break;
#endif   /* DUAL_LCD */
      default:
        // ASSERT(0);
      break;
   }
   lcd_power_down();
}  /* lcd_set_contrast() */

/*************************************************************************
* FUNCTION
*     lcd_set_linerate
*
* DESCRIPTION
*     This function set the line rate of LCM
*
* PARAMETERS
*     lcd_id   : selection MAIN_LCD or SUB_LCD
*     *lienrate: the line rate of LCM
*
* RETURNS
*     none
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_set_linerate(kal_uint8 lcd_id, kal_uint8 *linereate)
{
	lcd_power_up();
   switch (lcd_id)
   {
      case MAIN_LCD:
         set_linerate_DI(*linereate);
      break;
#ifdef DUAL_LCD
      case SUB_LCD:
         SubLCD->set_linerate(linereate);
      break;
#endif   /* DUAL_LCD */
      default:
        // ASSERT(0);
      break;
   }
   lcd_power_down();
}  /* lcd_set_linerate() */

/*************************************************************************
* FUNCTION
*     lcd_set_temp_compensate
*
* DESCRIPTION
*     This function set the temperature compensation parameters of LCM
*
* PARAMETERS
*     lcd_id      : selection MAIN_LCD or SUB_LCD
*     *compensate : the temperature compensate of LCM
*
* RETURNS
*     none
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_set_temp_compensate(kal_uint8 lcd_id, kal_uint8 *compensate)
{
	lcd_power_up();
   switch (lcd_id)
   {
      case MAIN_LCD:
         set_temp_compensate_DI(*compensate);
      break;
#ifdef DUAL_LCD
      case SUB_LCD:
         SubLCD->set_temp_compensate(compensate);
      break;
#endif   /* DUAL_LCD */
      default:
        // ASSERT(0);
      break;
   }
   lcd_power_down();
}  /* lcd_set_temp_compensate() */

kal_uint32 get_lcd_frame_buffer_address(void)
{
	return lcd_frame_buffer_address;
} /* get_lcd_frame_buffer_address */

kal_uint32 set_lcd_frame_buffer_address(kal_uint32 new_addr)
{
	lcd_frame_buffer_address = new_addr;
	return 0;
}


#if defined  (LCD_SCALE_176_220_TO_240_320)|| defined(LCD_SCALE_128_160_TO_176_220) || defined(__PRJ_WITH_SPILCD__)
#define GOUDA_STATE      2
#else
#define GOUDA_STATE      1
#endif

#if (CHIP_HAS_GOUDA != GOUDA_STATE)
 extern U16			simulator_Merge_buffer[LCD_WIDTH * LCD_HEIGHT];
#endif
void initLcdFrameBuffer(void)
{
#if (CHIP_HAS_GOUDA != GOUDA_STATE)
	lcd_frame_buffer_address = (U32)simulator_Merge_buffer;
#endif

}
extern void gdi_layer_Merge_frame_buffer(lcd_frame_update_struct *);

#if (CHIP_HAS_GOUDA == 1)
//static    DWORD       update_layer_back;
kal_uint32 lcd_frame_buffer_address_suspend;

void lcd_fb_update_overlay(lcd_frame_update_struct *lcd_para)
{
    U8          layer_id;
       HAL_GOUDA_OVL_LAYER_DEF_T def;
    U32     layer_flag_table[4];
    U32     layer_table[4];
    U16     layer_offset_x; 
    U16     layer_offset_y; 
    DWORD   update_layer;

    update_layer = lcd_para->update_layer;

    /* create this table, so we can use for loop to prcess each layer */
    layer_flag_table[0] = LCD_LAYER0_ENABLE;
    layer_flag_table[1] = LCD_LAYER1_ENABLE;
    layer_flag_table[2] = LCD_LAYER2_ENABLE;
    layer_flag_table[3] = LCD_LAYER3_ENABLE;

    layer_table[0] = HAL_GOUDA_OVL_LAYER_ID0;
    layer_table[1] = HAL_GOUDA_OVL_LAYER_ID1;
    layer_table[2] = HAL_GOUDA_OVL_LAYER_ID2;

    while(hal_GoudaIsActive())
    {
        sxr_Sleep(32);
    }
    lcd_SemTake();
    for (layer_id = 0 ; layer_id <GDI_LAYER_TOTAL_LAYER_COUNT-1; layer_id++)
    {
        hal_GoudaOvlLayerClose(layer_table[layer_id]);
    }

    for (layer_id = 0 ; layer_id < GDI_LAYER_TOTAL_LAYER_COUNT - 1; layer_id++)
    {
        /* check if this layer need to merge or not */
        if ((update_layer & layer_flag_table[layer_id]) == 0) continue;

    //  layer_info->x_offset = (U16) (offset_x + GDI_LAYER_OFFSET);
    //  layer_info->y_offset = (U16) (offset_y + GDI_LAYER_OFFSET);

        def.addr        = (UINT32 *)lcd_layer_data[layer_id].frame_buffer_address;
           layer_offset_x = lcd_layer_data[layer_id].x_offset;
        layer_offset_y = lcd_layer_data[layer_id].y_offset;
           def.pos.tlPX = layer_offset_x - GDI_LAYER_OFFSET;
        def.pos.tlPY = layer_offset_y - GDI_LAYER_OFFSET;
        def.pos.brPX = layer_offset_x + lcd_layer_data[layer_id].column_number - 1 - GDI_LAYER_OFFSET;
        def.pos.brPY = layer_offset_y + lcd_layer_data[layer_id].row_number - 1 - GDI_LAYER_OFFSET;

        //mmi_trace(1,"layer_id %x , (%x , %x),(%x , %x)",layer_id,def.pos.tlPX,def.pos.tlPY,def.pos.brPX,def.pos.brPY);

        // configure ovl layer 0 as buffer
        def.fmt = HAL_GOUDA_IMG_FORMAT_RGB565; //TODO convert from .colorFormat
    //  def.stride = frameBufferWin->fb.width * 2;
        def.stride = 0; // let hal gouda decide
#ifdef  MEM_ULC_3216
        if (lcd_layer_data[layer_id].opacity_enable == KAL_TRUE)        
            def.alpha = lcd_layer_data[layer_id].opacity_value << 3;
        else
            def.alpha = 255;
#else
        def.alpha = 255;
#endif
        def.cKeyEn = lcd_layer_data[layer_id].source_key_enable;
        def.cKeyColor = lcd_layer_data[layer_id].source_key;
        def.cKeyMask = HAL_GOUDA_CKEY_MASK_OFF;

// set default color when bottom layer with color transparent.		
 //   #ifdef __MMI_NOKIA_STYLE_N800__
	if(layer_id==0)
	{	
	  UINT32 pixCount=lcd_layer_data[0].column_number*lcd_layer_data[0].row_number;
	  UINT16 srcColor=def.cKeyColor;
	  UINT32 pixIdx=0;
	  UINT16* gRam=(UINT16*)def.addr;
            if(def.cKeyEn)
            {
        	  for(pixIdx=0;pixIdx<pixCount;pixIdx++)
        	  {
        	    if(srcColor==*(gRam+pixIdx))
                      *(gRam+pixIdx)=0xffff;//fill white color as before
        	  }
            }
	}
//    #endif
        /* check if this layer need to merge or not */
        if(layer_id !=GDI_LAYER_TOTAL_LAYER_COUNT-1)
            hal_GoudaOvlLayerOpen(layer_table[layer_id], &def);
        else
            mmi_trace(1,"lcd_fb_update_overlay err");
    }
    lcd_SemFree();	
}  /* lcd_fb_update_overlay() */


void frame_buffer_resume(void)
{
    set_lcd_frame_buffer_address(lcd_frame_buffer_address_suspend);
}
void lcd_fb_close_overlay(void)
{
    U8          layer_id;
    U32     layer_table[4];

    layer_table[0] = HAL_GOUDA_OVL_LAYER_ID1;
    layer_table[1] = HAL_GOUDA_OVL_LAYER_ID2;
    layer_table[2] = HAL_GOUDA_OVL_LAYER_ID0;

    for (layer_id = 0 ; layer_id <GDI_LAYER_TOTAL_LAYER_COUNT-1; layer_id++)
    {
        hal_GoudaOvlLayerClose(layer_table[layer_id]);
    }

}  /* lcd_fb_close_overlay() */



// ============================================================================
// layerMerge_DI
// ----------------------------------------------------------------------------
VOID layerMerge_DI(UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy)
{
    mci_LcdLayerMerge((UINT16*)get_lcd_frame_buffer_address(),startx,starty,endx,endy);
}

VOID layerMerge_2buffer(UINT16*buffer,UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy)
{
    mci_LcdLayerMerge(buffer,startx,starty,endx,endy);
}

#endif // (CHIP_HAS_GOUDA == 1)


/*************************************************************************
* FUNCTION
*     lcd_fb_update
*
* DESCRIPTION
*     This function trigger LCD interface to update the display RAM of LCM in
*     specified area
*
* PARAMETERS
*   *lcd_para : the parameters for lcd frame buffer update
*
* RETURNS
*     none
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_fb_update(lcd_frame_update_struct *lcd_para)
{
//panxu add used to cs621 game 20070915
	if (1 == ufo_flag)
	{
		 lcd_frame_buffer_address = lcd_layer_data[0].frame_buffer_address;
#if (CHIP_HAS_GOUDA == 1)
        lcd_fb_close_overlay();         
#endif // (CHIP_HAS_GOUDA == 1)
	}
	else
	{
#if defined(__PRJ_WITH_SPILCD__)
		lcd_fb_update_overlay(lcd_para);//2010-5-4 CuiJian add
		lcd_frame_buffer_address = (U32)simulator_Merge_buffer;
#else
#if (CHIP_HAS_GOUDA != GOUDA_STATE)
		gdi_layer_Merge_frame_buffer(lcd_para);//2007-7-3 fengzb
		lcd_frame_buffer_address = (U32)simulator_Merge_buffer;
#else // (CHIP_HAS_GOUDA == GOUDA_STATE)
        lcd_fb_update_overlay(lcd_para);//2010-5-4 CuiJian add
#if (CHIP_HAS_GOUDA != GOUDA_STATE)
        lcd_frame_buffer_address = (U32)simulator_Merge_buffer;
        layerMerge_DI(lcd_para->lcm_start_x,lcd_para->lcm_start_y,
                lcd_para->lcm_end_x,lcd_para->lcm_end_y);
#endif
#endif // (CHIP_HAS_GOUDA == GOUDA_STATE)
#endif // __PRJ_WITH_SPILCD__
	}
#if (defined(DUAL_LCD))
	if (lcd_para->lcd_id==MAIN_LCD)
	{
		if ((lcd_para->lcm_start_x>=LCD_WIDTH) || (lcd_para->lcm_end_x>=LCD_WIDTH)||
   	    (lcd_para->lcm_start_y>=LCD_HEIGHT) || (lcd_para->lcm_end_y>=LCD_HEIGHT))
      	 //ASSERT(0);
	}
	else
	{
		if ((lcd_para->lcm_start_x>=SUBLCD_WIDTH) || (lcd_para->lcm_end_x>=SUBLCD_WIDTH)||
   	    (lcd_para->lcm_start_y>=SUBLCD_HEIGHT) || (lcd_para->lcm_end_y>=SUBLCD_HEIGHT))
      	// ASSERT(0);
	}
#else
	if ((lcd_para->lcm_start_x>=LCD_WIDTH) || (lcd_para->lcm_end_x>=LCD_WIDTH)||
       (lcd_para->lcm_start_y>=LCD_HEIGHT) || (lcd_para->lcm_end_y>=LCD_HEIGHT))
     //  ASSERT(0);
#endif

	lcd_power_up();

	if (lcd_para->lcd_id==MAIN_LCD)
	{
#if defined(LCM_ROTATE_SUPPORT)
        if(mmi_frm_is_screen_width_height_swapped())
        { 
            BlockWrite_DI(lcd_para->lcm_start_x,lcd_para->lcm_start_y,
                lcd_para->lcm_end_x,lcd_para->lcm_end_y/*, 
                LCD_HEIGHT, LCD_WIDTH*/);
        }
        else
#endif
        { 
            BlockWrite_DI(lcd_para->lcm_start_x,lcd_para->lcm_start_y,
                lcd_para->lcm_end_x,lcd_para->lcm_end_y/*, 
                LCD_WIDTH, LCD_HEIGHT*/);
        }
	}
       lcd_power_down();
	if (1 == ufo_flag)
	{
#if (CHIP_HAS_GOUDA != GOUDA_STATE)
		 lcd_frame_buffer_address = (U32)simulator_Merge_buffer;
#endif
	}
#if (CHIP_HAS_GOUDA == GOUDA_STATE)
    frame_buffer_resume();
#endif // (CHIP_HAS_GOUDA == 1)
}  /* lcd_fb_update() */

/*************************************************************************
* FUNCTION
*     lcd_fb_update_to_memory
*
* DESCRIPTION
*     This function trigger LCD interface to output the MMI screen to one buffer
*
* PARAMETERS
*   *lcd_para : the parameters for lcd frame buffer update to memory
*
* RETURNS
*     none
*
* GLOBALS AFFECTED
*
*************************************************************************/

/*************************************************************************
* FUNCTION
*     lcd_stop_hw_update
*
* DESCRIPTION
*     This function stops the hardware trigger LCD update operation
*
* PARAMETERS
*     lcd_id   : selection MAIN_LCD or SUB_LCD
*
* RETURNS
*     none
*
* GLOBALS AFFECTED
*
*************************************************************************/

void reset_lcd_if(void)
{
}	/* reset_lcd_if() */

/*************************************************************************
* FUNCTION
*     assert_lcd_fb_update
*
* DESCRIPTION
*     This function dump the information to LCD panel when an assert occurs
*
*
* PARAMETERS
*     lcd_id   : selection MAIN_LCD or SUB_LCD
*		start_x	: start x of LCD panel
*		start_y	: start y of LCD panel
*		end_x		: end x of LCD panel
*		end_y		: end y of LCD panel
*     roi_offset_x : ROI window offset x
*		roi_offset_y : ROI window offset y
* 		update_layer : LCD update layers
*
* RETURNS
*     none
*
* GLOBALS AFFECTED
*
*************************************************************************/
void assert_lcd_fb_update(kal_uint8 lcd_id, kal_uint16 start_x, kal_uint16 start_y, kal_uint16 end_x,
				   	  		  kal_uint16 end_y,kal_uint16 roi_offset_x, kal_uint16 roi_offset_y, kal_uint32 update_layer)

{
	
}

/*************************************************************************
* FUNCTION
*     lcd_dma_slow_down
*
* DESCRIPTION
*     This function will change the lcd DMA transfer rate to slowest rate
*
* PARAMETERS

*
* RETURNS
*     none
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_dma_slow_down(kal_uint8 level)
{


}	/* lcd_dma_slow_down() */

/*************************************************************************
* FUNCTION
*     lcd_dma_recovery
*
* DESCRIPTION
*     This function will change the lcd DMA transfer rate to normal rate
*
* PARAMETERS

*
* RETURNS
*     none
*
* GLOBALS AFFECTED
*
*************************************************************************/
void lcd_dma_recovery(void)
{

}	/* lcd_dma_recovery() */

/*************************************************************************
* FUNCTION
*     set_lcd_driving_current
*
* DESCRIPTION
*     This function is for setting the driving current of LCM interface
*
* PARAMETERS
*     current : the driving current of LCM interface
*
* RETURNS
*     none
*
* GLOBALS AFFECTED
*
*************************************************************************/
void set_lcd_driving_current(kal_uint8 current)
{
	
}	/* set_lcd_driving_current() */
#if defined(LCM_ROTATE_SUPPORT)
void lcd_set_rotate(kal_uint8 lcd_id, kal_uint8 rotate_value)
{
    //if(lcd_id != MAIN_LCD) return;

    switch(rotate_value)
    { 
        case LCD_LAYER_ROTATE_90:
            lcdd_SetDirRotation(LCDD_ROTATION_90);
            break;
        case LCD_LAYER_ROTATE_180:
            lcdd_SetDirRotation(LCDD_ROTATION_180);
            break;
        case LCD_LAYER_ROTATE_270:
            lcdd_SetDirRotation(LCDD_ROTATION_270);
            break;
        case LCD_LAYER_ROTATE_NORMAL:
        default:
            lcdd_SetDirRotation(LCDD_ROTATION_0);
            break;
    }
}
#endif
