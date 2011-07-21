/*****************************************************************************
 * Filename:
 * ---------
 * custom_video.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is for customize some video parameters.
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
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "kal_release.h"

/*added by elisa for declaration */
#include "ts.h"               
//ended of elisa         2008_9_27

/* this must be sync with med_api.h */
enum {
	VID_REC_QTY_LOW = 0,
	VID_REC_QTY_NORMAL,
	VID_REC_QTY_HIGH,
	VID_REC_QTY_FINE,
	VID_NO_OF_REC_QTY
};


/*************************************************************************
* FUNCTION
*   vid_convert_quality_to_bitrate
*
* DESCRIPTION
*   This function is to convert quality to bitrate
*
* PARAMETERS
*   width: record image width
*   height: record image height
*   quality: required encode quality
*   bitrate_p: the pointer to output bitrate 
*   skip_frame_p: a flag to indicate skip frame
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void vid_convert_quality_to_bitrate(kal_uint16 width, kal_uint16 height, kal_uint8 quality, kal_uint32* bitrate_p, kal_uint8* skip_frame_p)
{
   kal_uint32 area;

   area=width*height;
   
   if(area<=128*96)
   {
      switch(quality)
      {
         case VID_REC_QTY_LOW:
            *bitrate_p=32000;
            *skip_frame_p=1;
            break;
         case VID_REC_QTY_NORMAL:
            *bitrate_p=64000;
            *skip_frame_p=1;
            break;
         case VID_REC_QTY_HIGH:
            *bitrate_p=96000; 
            *skip_frame_p=0;
            break;
         case VID_REC_QTY_FINE:
            *bitrate_p=128000;
            *skip_frame_p=0;
            break;
         default:
            ASSERT(0);
            *bitrate_p=128000;
            *skip_frame_p=0;
            break;
      }
   }   
   else if(area<=160*120)
   {
      switch(quality)
      {
         case VID_REC_QTY_LOW:
            *bitrate_p=50000;
            *skip_frame_p=1;
            break;
         case VID_REC_QTY_NORMAL:
            *bitrate_p=100000;
            *skip_frame_p=0;
            break;
         case VID_REC_QTY_HIGH:
            *bitrate_p=150000; 
            *skip_frame_p=0;
            break;
         case VID_REC_QTY_FINE:
            *bitrate_p=200000;
            *skip_frame_p=0;
            break;
         default:
            ASSERT(0);
            *bitrate_p=200000;
            *skip_frame_p=0;
            break;
      }
   }
   else if(area<=176*144)
   {
      switch(quality)
      {
         case VID_REC_QTY_LOW:
            *bitrate_p=64000;
            *skip_frame_p=1;
            break;
         case VID_REC_QTY_NORMAL:
            *bitrate_p=128000;
            *skip_frame_p=0;
            break;
         case VID_REC_QTY_HIGH:
            *bitrate_p=192000; 
            *skip_frame_p=0;
            break;
         case VID_REC_QTY_FINE:
            *bitrate_p=256000;
            *skip_frame_p=0;
            break;
         default:
            ASSERT(0);
            *bitrate_p=256000;
            *skip_frame_p=0;
            break;
      }
   }
   else if(area<=352*288)
   {
      switch(quality)
      {
         case VID_REC_QTY_LOW:
            *bitrate_p=150000;
            *skip_frame_p=0;
            break;
         case VID_REC_QTY_NORMAL:
            *bitrate_p=300000;
            *skip_frame_p=0;
            break;
         case VID_REC_QTY_HIGH:
            *bitrate_p=450000; 
            *skip_frame_p=0;
            break;
         case VID_REC_QTY_FINE:
            *bitrate_p=600000;
            *skip_frame_p=0;
            break;
         default:
            ASSERT(0);
            *bitrate_p=600000;
            *skip_frame_p=0;
            break;
      }
   }
   else /* larger than 352*288 */
   {
      switch(quality)
      {
         case VID_REC_QTY_LOW:
            *bitrate_p=150000;
            *skip_frame_p=0;
            break;
         case VID_REC_QTY_NORMAL:
            *bitrate_p=300000;
            *skip_frame_p=0;
            break;
         case VID_REC_QTY_HIGH:
            *bitrate_p=450000; 
            *skip_frame_p=0;
            break;
         case VID_REC_QTY_FINE:
            *bitrate_p=600000;
            *skip_frame_p=0;
            break;
         default:
            ASSERT(0);
            *bitrate_p=600000;
            *skip_frame_p=0;
            break;
      }
   }
   
}

