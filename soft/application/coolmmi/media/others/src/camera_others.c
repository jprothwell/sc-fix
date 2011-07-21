#include "mmi_data_types.h"
#include "mdi_datatype.h"
#include "gdi_datatype.h"
#include "cswtype.h"

#include "stack_config.h"
#include "mdi_audio.h"
#include "gdi_const.h"

#include "camera_others.h"
#include "kal_non_specific_general_types.h"
#include "mmi_features_camera.h"

#include "frameworkstruct.h"
#include "filemgr.h"
#include "camera_m.h"
#include "mmi_trace.h"

#ifdef __MMI_CAMERA__
int g_camera_feature_zoom[CAMERA_FEATURE_ZOOM_COUNT] = {0,1,2,3,4,5};
#endif
void custom_start_flashlight(kal_uint8 red_level, kal_uint8 green_level, kal_uint8 blue_level, kal_uint8 duty)
{
	mmi_trace(1,"chenhe,custom_start_flashlight,duty is %d",duty);

	camera_SetFlashL(CAM_FLASH_FORCED);
}

void custom_stop_flashlight(void)
{ 
	mmi_trace(1,"chenhe,custom_stop_flashlight,duty is %d");

	camera_SetFlashL(CAM_FLASH_NONE);
}

void PlayAudioDviStream(  U8* audioBuffer, U16 length, U8 style)
{
}

#if 0
GDI_RESULT gdi_image_jpeg_get_dimension(U8 *jpeg_src, U32 size, S32 *width, S32 *height)
{
    return GDI_FAILED;
}

GDI_RESULT gdi_image_jpeg_draw_internal(
                        S32 ox, S32 oy, 
                        S32 resized_width, S32 resized_height, 
                        S32 output_clipx1, S32 output_clipy1, 
                        S32 output_clipx2, S32 output_clipy2,
                        U8 *src,U32 size,BOOL is_file,BOOL is_resized)
{
    return GDI_FAILED;
}
#endif

/*************************************************************************************************
 *
 *               档案管理部分的接口
 *
 *************************************************************************************************/
#if 0
void mmi_fmgr_kernel_get_path_name(S8 *full_path_name)
{
}

S32 mmi_fmgr_kernel_get_cur_idx(void)
{
    return -1;
}

U16 mmi_fmgr_kernel_get_file_count(void)
{
    return 0;
}

void mmi_fmgr_kernel_get_file_info(S32 index, FMGR_FILE_INFO_STRUCT *file_info_ptr)
{
}

void mmi_fmgr_kernel_move_selection_down(void)
{
}

void mmi_fmgr_kernel_move_selection_up(void)
{
}
#endif
/*************************************************************************************************
 *
 *               wgui_categories_multimedia.c中涉及的接口
 *
 *************************************************************************************************/
/*void ShowCategory222Screen( U16         title,
                                        U16         title_icon,
                                        U16         left_softkey,
                                        U16         left_softkey_icon,
                                        U16         right_softkey,
                                        U16         right_softkey_icon,
                                        gdi_color   background_color,
                                        S8              *button_string,
                                        S8              *file_name_ptr,
                                        BOOL            is_short_filename,
                                        void            (*decode_result_callback)(GDI_RESULT))
{
}*/

/*************************************************************************************************
 *
 *               游戏和功能程序的初始化函数，camera模块在该函数里初始化 FunAndGamesSrc.c中涉及的接口
 *
 *************************************************************************************************/
#if 0
void mmi_fng_init_app(void)
{
    #if defined( __MMI_CAMERA__)
        mmi_camera_init_app();
        mmi_imgview_init_app();
    #endif /* __MMI_CAMERA__ */
}
#endif
/*
void fmt_abort_copy (void)
{
	//fmt_need_abort = KAL_TRUE;
}

kal_bool fmt_get_abort_copy (void)
{
	//return fmt_need_abort;
	return FALSE;
}*/
#if 0
void * kal_mem_cpy(void* dest, const void* src, kal_uint32 size)
{
	return memcpy ( dest, src, size );
}

#endif

