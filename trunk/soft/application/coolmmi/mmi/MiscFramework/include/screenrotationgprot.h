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
 *  ScreenRotationGprot.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Exported interface for MMI screen rotation
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

#ifndef __SCREEN_ROTATION_GPROT_H__
#define __SCREEN_ROTATION_GPROT_H__

#ifndef _PIXCOM_DATA_TYPES_H
#error "MMI_Data_Types.h should be included first"
#endif /* ~ _PIXCOM_DATA_TYPES_H */


/***************************************************************************** 
* Define
*****************************************************************************/

/***************************************************************************** 
* Typedef 
*****************************************************************************/

typedef enum {
	MMI_FRM_SCREEN_ROTATE_0,
	MMI_FRM_SCREEN_ROTATE_90,
	MMI_FRM_SCREEN_ROTATE_180,
	MMI_FRM_SCREEN_ROTATE_270,
	MMI_FRM_SCREEN_ROTATE_MIRROR_0,
	MMI_FRM_SCREEN_ROTATE_MIRROR_90,
	MMI_FRM_SCREEN_ROTATE_MIRROR_180,
	MMI_FRM_SCREEN_ROTATE_MIRROR_270,
	MMI_FRM_SCREEN_ROTATE_MAX_TYPE
} mmi_frm_screen_rotate_enum;

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
extern void mmi_frm_screen_rotate(mmi_frm_screen_rotate_enum rotation);
extern void mmi_frm_set_self_rotation_flag(mmi_frm_screen_rotate_enum rotation);

extern MMI_BOOL mmi_frm_is_previous_screen_width_height_swapped(void);
extern mmi_frm_screen_rotate_enum mmi_frm_get_previous_screen_rotate(void);

extern MMI_BOOL mmi_frm_is_screen_width_height_swapped(void);
extern mmi_frm_screen_rotate_enum mmi_frm_get_screen_rotate(void);
extern void mmi_frm_reset_screen_rotation(void);
extern void	mmi_frm_screen_rotate_req(mmi_frm_screen_rotate_enum rotate);

#endif /* ~ __SCREEN_ROTATION_GPROT_H__ */


