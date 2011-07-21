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
 *  gdi_image_decoder.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  Image decoder. Decode in media task' time. Non-blocking.
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#define __NEWSIMULATOR
//#include "mmi_features.h"
//#include "mmidatatype.h"
//#include "fat_fs.h"
#include "gdi_internal.h"

 
#ifdef GDI_USING_MEDIA_TASK_DECODER

#include "gui.h"        /* timer */
#include "queuegprot.h" /* message handling */
#include "eventsgprot.h"        /* event hadnling - get message return from camera driver */
#include "filemgr.h"    /* include this to get file name's lenght define */
#include "fat_fs.h"     /* for access file system */
/* DRM_REPLACE */
#include "drm_gprot.h"

#include "med_api.h"
#include "med_main.h"
#include "med_struct.h"

#ifdef GDI_HW_JPEG_SUPPORT_COLOR_FORMAT
#include "imgdma.h"
#endif 

extern BOOL gdi_memory_output;
extern U16 gdi_memory_output_width;
extern U16 gdi_memory_output_height;
extern U32 gdi_memory_output_buffer_address;
extern U32 gdi_memory_output_buffer_size;

typedef struct
{
    BOOL is_used;
    BOOL is_blt;
    FS_HANDLE file_handle;
    U16 img_type;
    S32 dest_offset_x;
    S32 dest_offset_y;
    S32 dest_width;
    S32 dest_height;
    S32 output_clipx1;
    S32 output_clipy1;
    S32 output_clipx2;
    S32 output_clipy2;
    S16 next_idx;
    S16 seq_num;                /* a seq number to sync with driver */
    gdi_handle layer_handle;    /* draw on which layer */
    gdi_handle lcd_handle;      /* draw on which lcd */
    S16 clipx1, clipy1, clipx2, clipy2;
} gdi_image_decoder_item_struct;

#define JPEG_MARKER_SOF0      0xC0
#define JPEG_MARKER_SOF1      0xC1
#define JPEG_MARKER_SOF2      0xC2
#define JPEG_MARKER_SOF3      0xC3
#define JPEG_MARKER_SOF15     0xCF
#define JPEG_MARKER_SOS       0xDA
#define JPEG_MARKER_DQT       0xDB
#define JPEG_MARKER_DNL       0xDC
#define JPEG_MARKER_DRI       0xDD
#define JPEG_MARKER_DHP       0xDE
#define JPEG_MARKER_EXP       0xDF
#define JPEG_MARKER_APP0      0xE0
#define JPEG_MARKER_APP15     0xEF
#define JPEG_MARKER_COM       0xFE

#define GDI_IMAGE_DECODER_ITEM_COUNT                  (10)
#define GDI_IMAGE_DECODER_ITEM_SEQ_NUM_LIMIT          (1000)

BOOL gdi_image_decoder_is_created = FALSE;

static S32 gdi_image_decoder_frame_buf_width;
static S32 gdi_image_decoder_frame_buf_height;

/* may extern to other gdi_image_xxxx_decoder for buffer access  */
U32 gdi_image_decoder_frame_buf_size;

static gdi_image_decoder_item_struct gdi_image_decoder_item_list[GDI_IMAGE_DECODER_ITEM_COUNT];
static S16 gdi_image_decoder_item_first_idx;
static S16 gdi_image_decoder_item_last_idx;
static S16 gdi_image_decoder_item_seq_num_idx;

/* callback function */
static void (*gdi_image_decoder_result_callback) (GDI_RESULT decoder_result, gdi_handle handle) = NULL;

/* extern uililty function in gdi_image.c */
extern U16 gdi_image_get_type_from_file(S8 *file_name);

/* BOOL gdi_image_jpeg_is_hw_init = FALSE; */

/************************************************************************/
/* Function Pre-Declare                                                 */
/************************************************************************/
GDI_RESULT gdi_image_decoder_decode_from_list(void);
void gdi_image_decoder_set_event_hdlr(void);

/* void                 gdi_image_decoder_jpeg_send_msg_decode_jpeg(void); */
void gdi_image_decoder_decode_finish_hdlr(void *msgPtr);


/*****************************************************************************
 * FUNCTION
 *  gdi_image_decoder_init
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_decoder_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < GDI_IMAGE_DECODER_ITEM_COUNT; i++)
    {
        gdi_image_decoder_item_list[i].is_used = FALSE;
    }

    gdi_image_decoder_item_first_idx = -1;
    gdi_image_decoder_item_last_idx = -1;

    /* we have to use a seperate number as the handle instead of using entry index */
    gdi_image_decoder_item_seq_num_idx = 1;

    return GDI_SUCCEED;

}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_decoder_create
 * DESCRIPTION
 *  
 * PARAMETERS
 *  max_img_width           [IN]        
 *  max_img_height          [IN]        
 *  result_callback_ptr     [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_decoder_create(
            S32 max_img_width,
            S32 max_img_height,
            void (*result_callback_ptr) (GDI_RESULT decoder_result, gdi_handle handle))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(void)
    S16 i;

    if (gdi_image_decoder_is_created)
    {
        ASSERT(0);
    }

    gdi_image_decoder_is_created = TRUE;

    /* TODO : allocate memory from mmi_memory */
    gdi_image_decoder_frame_buf_width = max_img_width + 16; /* +16 for jpeg decoder's stuffing */
    gdi_image_decoder_frame_buf_height = max_img_height + 16;   /* +16 for jpeg decoder's stuffing */
    gdi_image_decoder_frame_buf_size =
        (gdi_image_decoder_frame_buf_width * gdi_image_decoder_frame_buf_height * GDI_MAINLCD_BIT_PER_PIXEL) >> 3;

    /* register decoder result call back function */
    gdi_image_decoder_result_callback = result_callback_ptr;

    /* clear deocde item flags */
    for (i = 0; i < GDI_IMAGE_DECODER_ITEM_COUNT; i++)
    {
        gdi_image_decoder_item_list[i].is_used = FALSE;
    }

    gdi_image_decoder_item_first_idx = -1;
    gdi_image_decoder_item_last_idx = -1;

    gdi_image_decoder_item_first_idx = -1;
    gdi_image_decoder_item_last_idx = -1;

    /* set event handler */
    gdi_image_decoder_set_event_hdlr();

    GDI_RETURN(GDI_SUCCEED);

    GDI_EXIT_CRITICAL_SECTION(void)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_decoder_free
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_decoder_free(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_decoder_free)
    gdi_image_decoder_is_created = FALSE;

    /* clear call back function */
    gdi_image_decoder_result_callback = NULL;

    GDI_RETURN(GDI_SUCCEED);
    GDI_EXIT_CRITICAL_SECTION(gdi_image_decoder_free)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_decoder_decode_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  offset_x        [IN]        
 *  offset_y        [IN]        
 *  width           [IN]        
 *  height          [IN]        
 *  is_blt          [IN]        
 *  image_name      [?]         
 *  handle_ptr      [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_decoder_decode_file(
            S32 offset_x,
            S32 offset_y,
            S32 width,
            S32 height,
            BOOL is_blt,
            S8 *image_name,
            gdi_handle *handle_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_image_decoder_decode_file_with_output_clipping(
            offset_x,
            offset_y,
            width,
            height,
            0,
            0,
            width - 1,
            height - 1,
            is_blt,
            image_name,
            handle_ptr);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_decoder_decode_file_with_output_clipping
 * DESCRIPTION
 *  
 * PARAMETERS
 *  offset_x            [IN]        
 *  offset_y            [IN]        
 *  width               [IN]        
 *  height              [IN]        
 *  output_clipx1       [IN]        
 *  output_clipy1       [IN]        
 *  output_clipx2       [IN]        
 *  output_clipy2       [IN]        
 *  is_blt              [IN]        
 *  image_name          [?]         
 *  handle_ptr          [?]         
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_decoder_decode_file_with_output_clipping(
            S32 offset_x,
            S32 offset_y,
            S32 width,
            S32 height,
            S32 output_clipx1,
            S32 output_clipy1,
            S32 output_clipx2,
            S32 output_clipy2,
            BOOL is_blt,
            S8 *image_name,
            gdi_handle *handle_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_decoder_decode_file)
    S16 valid_idx;
    S16 this_idx;
    U32 file_size;
    BOOL has_entry = FALSE;
    FS_HANDLE file_handle;
    U16 image_type;

    if (handle_ptr != NULL)
    {
        *handle_ptr = GDI_ERROR_HANDLE;
    }

     
    if (width == 0 || height == 0)
    {
        GDI_RETURN(GDI_SUCCEED);
    }
    /* check if it is a decodable file format */
    image_type = gdi_image_get_type_from_file(image_name);

    if (image_type != GDI_IMAGE_TYPE_JPG_FILE)  /* currently, we only support jpeg file */
    {
        GDI_RETURN(GDI_IMAGE_DECODER_ERR_INVALID_IMG_TYPE);
    }

    /* find a valid entry to record this jpeg decoder */
    for (valid_idx = 0; valid_idx < GDI_IMAGE_DECODER_ITEM_COUNT; valid_idx++)
    {
        if (gdi_image_decoder_item_list[valid_idx].is_used == FALSE)
        {
            has_entry = TRUE;
            break;
        }
    }

    if (has_entry == FALSE)
    {
        /* cant find entry to save jpeg decoder data */
        GDI_RETURN(GDI_IMAGE_DECODER_ERR_NO_ITEM_ENTRY);
    }

    /* open file */
    file_handle = DRM_open_file((U16*) image_name, FS_READ_ONLY, 0);

    if (file_handle >= 0)
    {
        /* check if size can fit into file buffer */
        /* DRM_REPLACE */
        /* FS_GetFileSize(file_handle, (kal_uint32*)&file_size); */
        DRM_file_size(file_handle, (kal_uint32*) & file_size);

        /* check if dest size larger than intermediate frame buffer */
        if (gdi_image_decoder_frame_buf_size < (U32) ((width + 16) * (height + 16)))    /* +16 for jpeg's stuffing */
        {
            /* DRM_REPLACE */
            /* FS_Close(file_handle); */
            DRM_close_file(file_handle);
            GDI_RETURN(GDI_IMAGE_DECODER_ERR_FRAME_BUF_NOT_ENOUGH);
        }

        if (gdi_image_decoder_item_first_idx == -1)
        {
            /* first in the list, re-assign first index */
            gdi_image_decoder_item_first_idx = valid_idx;
        }
        else
        {
            this_idx = gdi_image_decoder_item_first_idx;

            /* loop to find next */
            while (this_idx != -1)
            {
                if (gdi_image_decoder_item_list[this_idx].next_idx == -1)
                {
                    gdi_image_decoder_item_list[this_idx].next_idx = valid_idx;
                    break;
                }

                this_idx = gdi_image_decoder_item_list[this_idx].next_idx;
            }
        }

        /* find a valid seq number */
        if (gdi_image_decoder_item_seq_num_idx < GDI_IMAGE_DECODER_ITEM_SEQ_NUM_LIMIT - 1)
        {
            gdi_image_decoder_item_seq_num_idx++;
        }
        else
        {
            gdi_image_decoder_item_seq_num_idx = 1;
        }

        gdi_image_decoder_item_list[valid_idx].is_used = TRUE;
        gdi_image_decoder_item_list[valid_idx].file_handle = file_handle;
        gdi_image_decoder_item_list[valid_idx].is_blt = is_blt;
        gdi_image_decoder_item_list[valid_idx].img_type = image_type;
        gdi_image_decoder_item_list[valid_idx].dest_offset_x = offset_x;
        gdi_image_decoder_item_list[valid_idx].dest_offset_y = offset_y;
        gdi_image_decoder_item_list[valid_idx].dest_width = (U16) width;
        gdi_image_decoder_item_list[valid_idx].dest_height = (U16) height;
        gdi_image_decoder_item_list[valid_idx].next_idx = -1;
        gdi_image_decoder_item_list[valid_idx].seq_num = gdi_image_decoder_item_seq_num_idx;
        gdi_image_decoder_item_list[valid_idx].layer_handle = (gdi_handle) gdi_act_layer;
        gdi_image_decoder_item_list[valid_idx].lcd_handle = GDI_LCD->act_handle;
        gdi_image_decoder_item_list[valid_idx].clipx1 = gdi_act_layer->clipx1;
        gdi_image_decoder_item_list[valid_idx].clipy1 = gdi_act_layer->clipy1;
        gdi_image_decoder_item_list[valid_idx].clipx2 = gdi_act_layer->clipx2;
        gdi_image_decoder_item_list[valid_idx].clipy2 = gdi_act_layer->clipy2;
        gdi_image_decoder_item_list[valid_idx].output_clipx1 = output_clipx1;
        gdi_image_decoder_item_list[valid_idx].output_clipy1 = output_clipy1;
        gdi_image_decoder_item_list[valid_idx].output_clipx2 = output_clipx2;
        gdi_image_decoder_item_list[valid_idx].output_clipy2 = output_clipy2;

        if (gdi_image_decoder_item_list[gdi_image_decoder_item_first_idx].next_idx == -1)
        {
            /* only one jpeg wait to decoder, call this function, if already has another jpeg is decoding,
               the following function will be called when the previous jpeg is decoderd */
            gdi_image_decoder_decode_from_list();
        }

        /* succeed create a jpeg decoder entry , return it's sequential no as handle */
        if (handle_ptr != NULL)
        {
            *handle_ptr = gdi_image_decoder_item_seq_num_idx;
        }

        GDI_RETURN(GDI_SUCCEED);
    }
    else
    {
        /* open File error */
        GDI_RETURN(GDI_IMAGE_DECODER_ERR_OPEN_FILE_FAILED);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_image_decoder_decode_file)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_decoder_decode_from_list
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_decoder_decode_from_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 idx;
    FS_HANDLE file_handle;

    media_img_decode_req_struct img_decode_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gdi_image_decoder_item_first_idx != -1)
    {
        idx = gdi_image_decoder_item_first_idx;

        /* load image from file to buffer */
        file_handle = gdi_image_decoder_item_list[idx].file_handle;

         
        memset(gdi_work_buffer, 0xff, gdi_image_decoder_frame_buf_size);

        img_decode_data.display_width = (kal_uint16) gdi_image_decoder_item_list[idx].dest_width;
        img_decode_data.display_height = (kal_uint16) gdi_image_decoder_item_list[idx].dest_height;
        img_decode_data.image_buffer_p = (void*)gdi_work_buffer;
        img_decode_data.image_buffer_size = (kal_uint32) gdi_image_decoder_frame_buf_size;
        img_decode_data.media_mode = (kal_uint16) MED_MODE_FILE;
        img_decode_data.media_type = (kal_uint16) MED_TYPE_JPG;
        img_decode_data.data = (void*)file_handle;
        img_decode_data.seq_num = (kal_uint16) gdi_image_decoder_item_list[idx].seq_num;
        img_decode_data.blocking = (kal_bool) FALSE;    /* non blocking */
        img_decode_data.image_clip_x1 = gdi_image_decoder_item_list[idx].output_clipx1;
        img_decode_data.image_clip_y1 = gdi_image_decoder_item_list[idx].output_clipy1;
        img_decode_data.image_clip_x2 = gdi_image_decoder_item_list[idx].output_clipx2;
        img_decode_data.image_clip_y2 = gdi_image_decoder_item_list[idx].output_clipy2;

    #ifdef GDI_HW_JPEG_SUPPORT_OUTPUT_ANOTHER_BUFFER
        img_decode_data.memory_output = gdi_memory_output;
        img_decode_data.memory_output_width = gdi_memory_output_width;
        img_decode_data.memory_output_height = gdi_memory_output_height;
        img_decode_data.memory_output_buffer_address = gdi_memory_output_buffer_address;
        img_decode_data.memory_output_buffer_size = gdi_memory_output_buffer_size;
    #endif /* GDI_HW_JPEG_SUPPORT_OUTPUT_ANOTHER_BUFFER */ 
    #ifdef GDI_HW_JPEG_SUPPORT_DIRECT_OUTPUT_BUFFER
        img_decode_data.image_pitch_mode = FALSE;   /* non blocking should not direct output to layer */
    #endif 
        img_decode_data.jpeg_thumbnail_mode = TRUE;

    #ifdef GDI_HW_JPEG_SUPPORT_COLOR_FORMAT
        switch (gdi_act_layer->vcf)
        {
            case GDI_COLOR_FORMAT_16:
                img_decode_data.image_data_format = IMGDMA_IBW_OUTPUT_RGB565;
                break;
            case GDI_COLOR_FORMAT_24:
            case GDI_COLOR_FORMAT_32:
                img_decode_data.image_data_format = IMGDMA_IBW_OUTPUT_RGB888;
                break;
            default:
                GDI_ASSERT(0);  /* strange  color format */
        }
    #endif /* GDI_HW_JPEG_SUPPORT_COLOR_FORMAT */ 

        img_decode_data.callback = NULL;

        /* call each image types decoder */
        switch (gdi_image_decoder_item_list[idx].img_type)
        {
            case GDI_IMAGE_TYPE_JPG_FILE:
                media_img_decode(MOD_MMI, (void*)&img_decode_data);
                break;

            default:
                GDI_ASSERT(0);
        }

        return GDI_SUCCEED;
    }

    return GDI_SUCCEED;

}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_decoder_stop
 * DESCRIPTION
 *  
 * PARAMETERS
 *  handle      [IN]        
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_decoder_stop(gdi_handle handle)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_decoder_stop)
    S16 this_idx;
    S16 prev_idx = -1;

    this_idx = gdi_image_decoder_item_first_idx;

    while (this_idx != -1)
    {
        if (handle == gdi_image_decoder_item_list[this_idx].seq_num)
        {
            /* stop jpeg decoderr driver */

            if (this_idx == gdi_image_decoder_item_first_idx)
            {
                /* is the one that currently decoding - stop it */

                /* handle each decoder's stop function */
                switch (gdi_image_decoder_item_list[this_idx].img_type)
                {
                    case GDI_IMAGE_TYPE_JPG_FILE:
                        media_img_stop(MOD_MMI);
                        break;

                    default:
                        GDI_ASSERT(0);
                }

                /* close file */
                /* DRM_REPLACE */
                /* FS_Close(gdi_image_decoder_item_list[this_idx].file_handle); */
                DRM_close_file(gdi_image_decoder_item_list[this_idx].file_handle);

                gdi_image_decoder_item_list[this_idx].is_used = FALSE;
                gdi_image_decoder_item_first_idx = gdi_image_decoder_item_list[this_idx].next_idx;

                if (gdi_image_decoder_item_first_idx != -1)
                {
                    gdi_image_decoder_decode_from_list();
                }
            }
            else
            {
                /* not first one */
                /* DRM_REPLACE */
                /* FS_Close(gdi_image_decoder_item_list[this_idx].file_handle); */
                DRM_close_file(gdi_image_decoder_item_list[this_idx].file_handle);

                /* file is successfully read into buffer, clear file handle */
                gdi_image_decoder_item_list[this_idx].file_handle = -1;
                /* gdi_image_decoder_item_list[this_idx].file_size              = 0; */

                gdi_image_decoder_item_list[this_idx].is_used = FALSE;
                gdi_image_decoder_item_list[prev_idx].next_idx = gdi_image_decoder_item_list[this_idx].next_idx;
            }

            GDI_RETURN(GDI_SUCCEED);
        }

        prev_idx = this_idx;
        this_idx = gdi_image_decoder_item_list[this_idx].next_idx;
    }

    GDI_RETURN(GDI_IMAGE_DECODER_ERR_WRONG_HANDLE);

    GDI_EXIT_CRITICAL_SECTION(gdi_image_decoder_stop)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_decoder_stop_all
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
GDI_RESULT gdi_image_decoder_stop_all(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_decoder_stop_all)
    S16 next_idx;

    /* check if anything in decoder */
    if (gdi_image_decoder_item_first_idx == -1)
    {
        GDI_RETURN(GDI_SUCCEED);
    }

    /* stop current running decoder */
    switch (gdi_image_decoder_item_list[gdi_image_decoder_item_first_idx].img_type)
    {
        case GDI_IMAGE_TYPE_JPG_FILE:
            media_img_stop(MOD_MMI);
            break;

        default:
            GDI_ASSERT(0);
    }

    /* close file */
    /* DRM_REPLACE */
    /* FS_Close(gdi_image_decoder_item_list[gdi_image_decoder_item_first_idx].file_handle); */
    DRM_close_file(gdi_image_decoder_item_list[gdi_image_decoder_item_first_idx].file_handle);

    next_idx = gdi_image_decoder_item_first_idx;

    while (next_idx != -1)
    {
        /* close all opened file */
        if (gdi_image_decoder_item_list[next_idx].file_handle >= 0)
        {
            /* DRM_REPLACE */
            /* FS_Close(gdi_image_decoder_item_list[next_idx].file_handle); */
            DRM_close_file(gdi_image_decoder_item_list[next_idx].file_handle);
            gdi_image_decoder_item_list[next_idx].file_handle = -1;
            /* gdi_image_decoder_item_list[next_idx].file_size        = 0; */
        }

        gdi_image_decoder_item_list[next_idx].is_used = FALSE;
        next_idx = gdi_image_decoder_item_list[next_idx].next_idx;
    }

    gdi_image_decoder_item_first_idx = -1;
    gdi_image_decoder_item_last_idx = -1;

    GDI_RETURN(GDI_SUCCEED);

    GDI_EXIT_CRITICAL_SECTION(gdi_image_decoder_stop_all)
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_decoder_set_event_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_image_decoder_set_event_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetProtocolEventHandler(gdi_image_decoder_decode_finish_hdlr, MSG_ID_MEDIA_IMG_DECODE_FINISH_IND);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_image_decoder_decode_finish_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msgPtr      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_image_decoder_decode_finish_hdlr(void *msgPtr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *dest_buf_ptr;
    U8 *src_buf_ptr;

    S32 index;
    S32 dest_width;
    S32 dest_height;
    S32 dest_offset_x;
    S32 dest_offset_y;
    S32 dest_pitch;
    S32 src_pitch;
    S32 layer_offset_x, layer_offset_y;
    BOOL is_blt;
    gdi_rect_struct dest_clip;
    gdi_handle handle;
    S32 fs_ret;
    gdi_handle old_layer_handle;
    gdi_handle old_lcd_handle;
    U8 src_cf;

    media_img_decode_finish_ind_struct *para_ptr = (media_img_decode_finish_ind_struct*) msgPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* no item. this will occure if driver already send out message to GDI, meanwhile user call
       decoder_stop to remove this decoder item. so this index will be -1 */
    if (gdi_image_decoder_item_first_idx == -1)
    {
        return;
    }

    /* jpeg_local_para_struct *para_ptr = (jpeg_local_para_struct*)msgPtr; */

    /* ignore this message if it is not in the list */
    if (para_ptr->seq_num != gdi_image_decoder_item_list[gdi_image_decoder_item_first_idx].seq_num)
    {
        return;
    }

    /* check if hw is powered on */
    /* ASSERT(gdi_image_jpeg_is_hw_init == TRUE); */

    /* close file handle */
    /* DRM_REPLACE */
    /* fs_ret = FS_Close(gdi_image_decoder_item_list[gdi_image_decoder_item_first_idx].file_handle); */
    fs_ret = DRM_close_file(gdi_image_decoder_item_list[gdi_image_decoder_item_first_idx].file_handle);
    ASSERT(fs_ret >= 0);

    handle = gdi_image_decoder_item_list[gdi_image_decoder_item_first_idx].seq_num;

    /* if decode succeed */
    if (para_ptr->result == MED_RES_OK)
    {
        /* set variables */
        index = gdi_image_decoder_item_first_idx;

        dest_width = gdi_image_decoder_item_list[index].dest_width;
        dest_height = gdi_image_decoder_item_list[index].dest_height;

        dest_offset_x = gdi_image_decoder_item_list[index].dest_offset_x;
        dest_offset_y = gdi_image_decoder_item_list[index].dest_offset_y;
        dest_clip.x1 = gdi_image_decoder_item_list[index].clipx1;
        dest_clip.y1 = gdi_image_decoder_item_list[index].clipy1;
        dest_clip.x2 = gdi_image_decoder_item_list[index].clipx2;
        dest_clip.y2 = gdi_image_decoder_item_list[index].clipy2;
        dest_pitch = gdi_act_layer->width;

        is_blt = gdi_image_decoder_item_list[index].is_blt;

        /* get actual resized image, with stuffing pixels */
        src_pitch = para_ptr->image_width;

        /* if not totally out of clip region */
        if (!((dest_offset_x > dest_clip.x2) ||
              (dest_offset_y > dest_clip.y2) ||
              (dest_offset_x + dest_width < dest_clip.x1) || (dest_offset_y + dest_height < dest_clip.y1)))
        {

            gdi_lcd_get_active(&old_lcd_handle);
            gdi_lcd_set_active(gdi_image_decoder_item_list[index].lcd_handle);

            gdi_layer_get_active(&old_layer_handle);
            gdi_layer_set_active(gdi_image_decoder_item_list[index].layer_handle);

            gdi_layer_get_buffer_ptr(&dest_buf_ptr);
            src_buf_ptr = (U8*) gdi_work_buffer;

    #ifdef GDI_USING_2D_ENGINE_V2
            if (gdi_act_layer->vcf == GDI_COLOR_FORMAT_32)
            {
                src_cf = GDI_COLOR_FORMAT_24;
            }
            else
            {
                src_cf = gdi_act_layer->vcf;
            }
       		gdi_bitblt_internal((U8*)gdi_work_buffer, dest_width, 
	    		0, 0, 
		    	dest_width, dest_height,
			    src_cf,
			    gdi_act_layer->buf_ptr, gdi_act_layer->width,
			    dest_offset_x, dest_offset_y,
			    dest_clip.x1, dest_clip.y1, dest_clip.x2, dest_clip.y2,
			    gdi_act_layer->vcf, gdi_act_layer->layer_size,
			    FALSE,
			    FALSE, 0,
			    FALSE, 0,
			    FALSE, 0,
			    GDI_TRANSFORM_NONE,
			    GDI_TRANSFORM_DIRECTION_LT);
            if (gdi_act_layer_info->source_key_enable)
            {
                gdi_act_replace_src_key(
                    gdi_act_layer->buf_ptr,
                    gdi_act_layer->width,
                    dest_offset_x,
                    dest_offset_y,
                    dest_clip.x1,
                    dest_clip.y1,
                    dest_clip.x2,
                    dest_clip.y2,
                    gdi_act_layer_info->source_key,
                    dest_width,
                    dest_height);
            }

    #else
            gdi_2d_memory_blt(
                src_buf_ptr,    /* src_ptr */
                src_pitch,      /* src_pitch */
                0,              /* src_offset_x */
                0,              /* src_offset_y */
                dest_width,     /* src_width */
                dest_height,    /* src_height */
                dest_buf_ptr,   /* dest_ptr */
                dest_pitch,     /* dest_pitch */
                dest_offset_x,  /* dest_offset_x */
                dest_offset_y,  /* dest_offset_y */
                dest_clip);     /* dest_clip */
    #endif

            gdi_layer_get_position(&layer_offset_x, &layer_offset_y);

            layer_offset_x += dest_offset_x;
            layer_offset_y += dest_offset_y;

            if (is_blt)
            {
                gdi_layer_blt_previous(
                    layer_offset_x,
                    layer_offset_y,
                    layer_offset_x + dest_width,
                    layer_offset_y + dest_height);
            }

            gdi_layer_set_active(old_layer_handle);
            gdi_lcd_set_active(old_lcd_handle);
        }

        if (gdi_image_decoder_result_callback != NULL)
        {
            gdi_image_decoder_result_callback(GDI_SUCCEED, handle);
        }
    }
    else
    {
        if (gdi_image_decoder_result_callback != NULL)
        {
            gdi_image_decoder_result_callback(GDI_FAILED, handle);
        }
    }

    gdi_image_decoder_item_list[gdi_image_decoder_item_first_idx].is_used = FALSE;
    gdi_image_decoder_item_first_idx = gdi_image_decoder_item_list[gdi_image_decoder_item_first_idx].next_idx;

    if (gdi_image_decoder_item_first_idx != -1)
    {
        gdi_image_decoder_decode_from_list();
    }

}

#endif /* GDI_USING_MEDIA_TASK_DECODER */ 

