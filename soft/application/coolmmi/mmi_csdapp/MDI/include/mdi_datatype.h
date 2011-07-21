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
 *	mdi_datatype.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Audio interfce header file
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
#ifndef _MDI_DATATYPE_H_
#define _MDI_DATATYPE_H_

#include "kal_non_specific_general_types.h"		/* include this for kal data type */
#include "cswtype.h"
#include "fs_type.h"
#include "mmi_data_types.h" 

/* Assert */
#ifdef MMI_ON_HARDWARE_P
#define MDI_ASSERT(exp)	
#define MDI_RECOVERABLE_ASSERT(exp)
#else
#define MDI_ASSERT(exp)				
#define MDI_RECOVERABLE_ASSERT(exp)	
#endif /* MMI_ON_HARDWARE_P */


typedef U16 mdi_state; 
typedef S32	mdi_handle;
typedef S32	mdi_result;
typedef void (*mdi_callback)(mdi_result result); 
typedef void (*mdi_bt_callback) (U8 profile, mdi_result result);
typedef BOOL (*mdi_bg_callback)(mdi_result result); 
typedef void (*mdi_mma_callback)( kal_int32 handle, kal_int32 event );

typedef U16 jdi_state; 
typedef S32	jdi_handle;
typedef S32	jdi_result;
typedef void (*jdi_callback)(jdi_result result); 
typedef BOOL (*jdi_bg_callback)(jdi_result result); 
typedef void (*jdi_mma_callback)( kal_int32 handle, kal_int32 event );

#define MDI_HANDLE	mdi_handle
#define MDI_RESULT	mdi_result

#define JDI_HANDLE	jdi_handle
#define JDI_RESULT	jdi_result

/* camera 1000 */
#define MDI_RES_CAMERA_SUCCEED								0
#define MDI_RES_CAMERA_ERR_FAILED							-1001
#define MDI_RES_CAMERA_ERR_DISK_FULL						-1002
#define MDI_RES_CAMERA_ERR_WRITE_PROTECTION				-1003
#define MDI_RES_CAMERA_ERR_NO_DISK							-1004
#define MDI_RES_CAMERA_ERR_HW_NOT_READY					-1005
#define MDI_RES_CAMERA_ERR_MEMORY_NOT_ENOUGH				-1006
#define MDI_RES_CAMERA_ERR_PREVIEW_FAILED					-1007
#define MDI_RES_CAMERA_ERR_CAPTURE_FAILED					-1008

/* webcam 1500 */
#define MDI_RES_WEBCAM_SUCCEED                      0
#define MDI_RES_WEBCAM_DRV_ABORT                    1501
#define MDI_RES_WEBCAM_ERR_FAILED                   -1502

/* video recorder 2000 */
#define MDI_RES_VDOREC_SUCCEED								0
#define MDI_RES_VDOREC_DISK_FULL								2001
#define MDI_RES_VDOREC_REACH_SIZE_LIMIT					2002
#define MDI_RES_VDOREC_RECORD_ALREADY_STOP				2003
#define MDI_RES_VDOREC_REACH_DISK_LIMIT					2004
#define MDI_RES_VDOREC_ERR_FAILED							-2001
#define MDI_RES_VDOREC_ERR_DISK_FULL						-2002
#define MDI_RES_VDOREC_ERR_WRITE_PROTECTION				-2003
#define MDI_RES_VDOREC_ERR_NO_DISK							-2004
#define MDI_RES_VDOREC_ERR_HW_NOT_READY					-2005
#define MDI_RES_VDOREC_ERR_PREVIEW_FAILED					-2006
#define MDI_RES_VDOREC_ERR_RESUME_FAILED					-2007
#define MDI_RES_VDOREC_ERR_PAUSE_FAILED					-2008
#define MDI_RES_VDOREC_ERR_SAVE_FILE_FAILED				-2009
#define MDI_RES_VDOREC_ERR_POWER_ON_FAILED				-2010
#define MDI_RES_VDOREC_ERR_RECORD_FAILED					-2011
#define MDI_RES_VDOREC_ERR_FILE_SIZE					-2012
#define MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH				-2013


/* vdioe player 3000 */
#define MDI_RES_VDOPLY_SUCCEED								0
#define MDI_RES_VDOPLY_SEEK_PARTIAL_DONE					3001
#define MDI_RES_VDOPLY_ERR_FAILED							-3001
#define MDI_RES_VDOPLY_ERR_OPEN_FILE_FAILED				-3002
#define MDI_RES_VDOPLY_ERR_SNAPSHOT_FAILED				-3003
#define MDI_RES_VDOPLY_ERR_PLAY_FAILED						-3004
#define MDI_RES_VDOPLY_ERR_SEEK_FAILED						-3005
#define MDI_RES_VDOPLY_ERR_SNAPSHOT_DISK_FULL			-3006
#define MDI_RES_VDOPLY_ERR_SNAPSHOT_WRITE_PROTECTION	-3007
#define MDI_RES_VDOPLY_ERR_SNAPSHOT_NO_DISK				-3008
#define MDI_RES_VDOPLY_ERR_SNAPSHOT_ROOT_DIR_FULL		-3009

#endif /* _MDI_DATATYPE_H_ */


