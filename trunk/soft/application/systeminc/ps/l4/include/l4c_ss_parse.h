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
 *	l4c_ss_parse.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for �K.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/********************************************************************************
*  Copyright Statement:
*  --------------------
*
*  This product has been developed using a protocol stack
*  developed by Sasken Communication Technologies Limited.
*
********************************************************************************/


#ifndef _L4C_SS_PARSE_H
#define _L4C_SS_PARSE_H

typedef struct
{
   kal_bool                cug_flag;
   kal_uint8               clir_flag;
   kal_uint8               call_type;
   csmss_string_op_enum   opcode;
   l4c_number_struct       addr;
} l4c_ss_string_info_struct;

typedef enum
{
   SS_DCS_ENGLISH = 0x0f,
   SS_DCS_CHINESE = 0x08
} l4c_ss_dcs_enum;

extern kal_bool l4c_ss_string_parsing (kal_uint8 src_id,
                                       kal_uint8 dcs,
                                       rmmi_string_struct *input,
                                       kal_uint8 length,
                                       l4c_ss_string_info_struct *info);

extern kal_bool is_sim_string (kal_uint8 src_id, rmmi_string_struct *input, kal_bool *format_valid);

extern kal_bool is_gprs_string (kal_uint8 src_id, rmmi_string_struct *input);

extern kal_bool is_cc_string (kal_uint8 src_id,
                              rmmi_string_struct *input,
                              l4c_ss_string_info_struct *info);

extern kal_bool setup_pdp_act_struct (kal_uint8 src_id,
                                      rmmi_string_struct *input);

extern kal_bool setup_sim_struct (kal_uint8 src_id,
                                  rmmi_string_struct *input,
                                  kal_uint8 type,
								  kal_bool *format_valid);

extern kal_bool setup_number_struct (kal_uint8 src_id,
                                   rmmi_string_struct *input,
                                   l4c_ss_string_info_struct *info);

extern kal_bool get_int_before_star(rmmi_string_struct *input,
                                    kal_uint16* number);

extern kal_bool get_uint32_before_star(rmmi_string_struct *input,
                                    kal_uint32* number);

extern kal_bool get_string_before_star(rmmi_string_struct *input,
                                    kal_uint8* string_ptr);

extern kal_bool get_dn_before_star(rmmi_string_struct *input,
                                    kal_uint8* string_ptr);

#endif /* l4c_ss_parse.h */


