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
 *	 mdi_webcam.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Webcam related interface header file
 *
 * Author:
 * -------
 *	                     
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Revision:   1.3  $
 * $Modtime:   Jun 08 2006 20:43:28  $
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi_CSDapp/MDI/include/mdi_webcam.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:05:56   lin
 * �ָ�CSW����ǰ�İ汾
 * 
 *    Rev 1.1   Nov 24 2006 18:45:50   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:31:16   zhangxb
 * Initial revision.
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MDI_WEBCAM_H_
#define _MDI_WEBCAM_H_

#ifdef __MMI_WEBCAM__

/***************************************************************************** 
* Include
*****************************************************************************/

/***************************************************************************** 
* Define
*****************************************************************************/

#define MDI_WEBCAM_PREVIEW_ROTATE_0					0
#define MDI_WEBCAM_PREVIEW_ROTATE_90				1
#define MDI_WEBCAM_PREVIEW_ROTATE_180				2
#define MDI_WEBCAM_PREVIEW_ROTATE_270				3
#define MDI_WEBCAM_PREVIEW_ROTATE_0_MIRROR		4
#define MDI_WEBCAM_PREVIEW_ROTATE_90_MIRROR		5
#define MDI_WEBCAM_PREVIEW_ROTATE_180_MIRROR		6
#define MDI_WEBCAM_PREVIEW_ROTATE_270_MIRROR		7




/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef enum {
	MDI_WEBCAM_JPG_160x120,
	MDI_WEBCAM_JPG_320x240,		
	MDI_WEBCAM_JPG_640x480,

	MDI_WEBCAM_JPG_COUNT
} mdi_webcam_jpg_enum;

typedef void (*mdi_webcam_event_callback) (MDI_RESULT, BOOL);

/*****************************************************************************              
* Extern Global Variable                                                                    
*****************************************************************************/             

/*****************************************************************************              
* Extern Global Function                                                                    
*****************************************************************************/    
extern MDI_RESULT mdi_webcam_init(void);
extern MDI_RESULT mdi_webcam_set_pause_jpeg_file(U16 idx, PU8 jpeg_file_ptr, U32 file_length, S32 width, S32 height);
extern MDI_RESULT mdi_webcam_start(U16 rotate, U16 banding, mdi_webcam_event_callback event_callback);
extern MDI_RESULT mdi_webcam_stop(void);
extern MDI_RESULT mdi_webcam_pause(void);
extern MDI_RESULT mdi_webcam_resume(U16 rotate);
extern MDI_RESULT mdi_webcam_capture(void);
extern MDI_RESULT mdi_webcam_rotate(U16 rotate);

extern U16			mdi_webcam_get_pause_jpeg_count(void);
extern MDI_RESULT	mdi_webcam_get_pause_jpeg_info(U16 idx, S32 *image_width, S32 *image_height);





#endif /* __MMI_WEBCAM_APP__ */
#endif /* _MDI_WEBCAM_H_ */


