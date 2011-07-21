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
 *   fmt_struct.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2002
*
*******************************************************************************
 *  FILENAME : fmt_struct.h
 *  SYNOPSIS : Functions for encoding/decoding.
 *
 *                      R E V I S I O N    H I S T O R Y
 *
 */

#ifndef _FMT_STRUCT_H
#define _FMT_STRUCT_H

/* Enum and Struct */

/* Structure for Primitive */

/* for MSG_ID_MMI_FMT_COPY_REQ */
typedef struct {
	 kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8   action;
    kal_uint8   *src_file;
    kal_uint8   dest_file[(260+1)<<1];
} mmi_fmt_copy_req_strcut;

/* for MSG_ID_MMI_FMT_READ_REQ */

typedef struct {
 	  kal_int32 iFd;
    kal_uint8 *pBuff;
    kal_uint32 uSize;    
} mmi_fmt_read_req_strcut;


/* for MSG_ID_MMI_FMT_WRITE_REQ */

typedef struct {
    kal_int32 iFd;
    kal_uint8 *pBuff;
    kal_uint32 uSize;    
} mmi_fmt_write_req_strcut;

typedef struct {	 
    kal_uint32   uSize;		
    kal_uint8* pBuff;
    kal_int32 	 iResult;
} mmi_fmt_read_rsp_strcut;

typedef struct {	 
    kal_uint32   uSize;		
    kal_uint8* pBuff;
    kal_int32 	 iResult;
} mmi_fmt_write_rsp_strcut;

typedef struct {
	 kal_uint8 ref_count;
    kal_uint16 msg_len;		
    kal_int32 	 result;
} mmi_fmt_copy_rsp_strcut;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8	drive;
    kal_uint32	level;
} mmi_fmt_format_req_strcut;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8	drive;
} mmi_fmt_check_drive_req_strcut;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint32	type;
    void	*filter;   /* address of filter mask */
    kal_uint8	*path;
} mmi_fmt_delete_req_strcut;

typedef mmi_fmt_copy_rsp_strcut mmi_fmt_format_rsp_strcut;
typedef mmi_fmt_copy_rsp_strcut mmi_fmt_check_drive_rsp_strcut;
typedef mmi_fmt_copy_rsp_strcut mmi_fmt_unmount_rsp_strcut;
typedef mmi_fmt_copy_rsp_strcut mmi_fmt_delete_rsp_strcut;

typedef struct {
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8	index;
    kal_uint32	mode;
} mmi_fmt_unmount_req_strcut;

typedef struct {
	kal_uint8 ref_count;
	kal_uint16 msg_len;
	kal_bool present;
	kal_uint32 type;
} mmi_fmt_get_msdc_status_rsp_strcut;

typedef struct {
	kal_uint8 ref_count;
	kal_uint16 msg_len;   
	kal_uint32 type;
	kal_int32 cause;
} fmt_mmi_msdc_ind_struct;

typedef fmt_mmi_msdc_ind_struct fmt_mmi_card_plug_in_ind_struct;
typedef fmt_mmi_msdc_ind_struct fmt_mmi_card_plug_out_ind_struct;

#endif /*#ifdef _FMT_STRUCT_H */

