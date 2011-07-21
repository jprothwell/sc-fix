////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2007, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: Camera_DI_Nochip.h
// 
// DESCRIPTION:
//   TODO... 
// 
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   TODO... 
//   Dingjian         070620          build
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef __CAMERA_DI_NOCHIP_H__
#define __CAMERA_DI_NOCHIP_H__

////////////////////////////////////////////////////////////////////////////////
// DECLARE HEAD FILES
////////////////////////////////////////////////////////////////////////////////
// TODO... 

////////////////////////////////////////////////////////////////////////////////
// DEFINE LOCAL CONSTANTS 
////////////////////////////////////////////////////////////////////////////////
// TODO.. 

////////////////////////////////////////////////////////////////////////////////
// DEFINE LOCAL MACROS 
////////////////////////////////////////////////////////////////////////////////
// TODO... 
#define SET_PARAM_ZOOM 1
#define SET_PARAM_CONTRAST 2
#define SET_PARAM_BRIGHTNESS 3
#define SET_PARAM_EFFECT 8
#define SET_BANDING_FREQ 9
#define SET_PARAM_IMAGESIZE 24
#define SET_PARAM_QUALITY 23

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// DECLARE GLOBAL FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
// TODO... 

////////////////////////////////////////////////////////////////////////////////
// DEFINE VARIABLES TYPE 
////////////////////////////////////////////////////////////////////////////////
// TODO... 
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	VOID * burst_capture_ptr;//ext_camera_burst_struct	*burst_capture_ptr;
	UINT16	image_width;
	UINT16	image_height;
	UINT16  buffer_width;
	UINT16  buffer_height;
	UINT16  preview_offset_x;
	UINT16  preview_offset_y;
	UINT16	preview_width;
	UINT16	preview_height;
	UINT32	total_file_size;
	UINT32  src_key_color;	
	UINT8	captured_picture;
	UINT8   snapshot_number;
	UINT8	white_balance_mode1;
	UINT8	ev_value;
	UINT8	gamma_table;
	UINT8	zoom_factor;
	UINT8 	image_effect1;
	UINT8 	post_effect;			
	UINT8 	sticker_capture;		
	UINT8	lcd_id;					
	UINT8	frame_rate;
	UINT8	jpeg_compression_ratio;
	UINT8 	preview_rotate;
	UINT8 	banding_freq;
	UINT8	*jpeg_buffer_ptr;
	UINT16	*image_buffer_ptr;
	UINT32  image_buffer_size;
	UINT8	continue_capture;	
	BOOL    video_mode;
} ext_camera_para_struct_di;
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    VOID (* cam_module_power_on)(VOID);
    VOID (* cam_module_cmd_mapping)(VOID);
    UINT8 (* cam_module_reset_status)(VOID);
    UINT8 (* cam_module_preview_ready_check)(VOID);
    UINT32 (* cam_module_capture_ready_check)(VOID);
    VOID (* cam_module_power_off)(VOID);
    VOID (* cam_module_preview)(ext_camera_para_struct_di *ext_cam_para);
    VOID (* exit_cam_module_preview)(VOID);
    VOID (* cam_module_capture)(ext_camera_para_struct_di *ext_cam_para);
    UINT32 (* cam_module_jpeg_encode) (ext_camera_para_struct_di *ext_cam_para, BOOL back_to_preview);
    VOID (* cam_module_set_para)(UINT8 cmd, UINT8 para);
    VOID (* cam_module_write_para)(UINT16 cmd, UINT16 para);
    UINT16 (* cam_module_read_para)(UINT16 cmd);
    UINT8 (* cam_module_frame_rate)(VOID);
    VOID (* cam_module_write_lcdfb)(ext_camera_para_struct_di *ext_cam_para);
} Camera_Funcs_DI;

////////////////////////////////////////////////////////////////////////////////
// DECLARE GLOBAL VARIABLES
////////////////////////////////////////////////////////////////////////////////
// TODO... 

////////////////////////////////////////////////////////////////////////////////


#endif // __CAMERA_DI_NOCHIP_H__ 


