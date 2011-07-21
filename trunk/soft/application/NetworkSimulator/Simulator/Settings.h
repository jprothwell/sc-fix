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
typedef unsigned char   U8;
typedef unsigned short  U16;
	

#define MAX_SIM_PASSWD_LEN 9

typedef enum {
   SETTINGS_FALSE=0,
   SETTINGS_TRUE=1
} result_bool;

typedef struct{
		LOCAL_PARA_HDR
		U8	opcode; /* sim_security_operation_enum */
		U8	type; /* smu_security_type_enum */
		U8	passwd[MAX_SIM_PASSWD_LEN];
	} SMU_LOCK_REQUEST_STRUCT;

typedef struct
	{
		LOCAL_PARA_HDR
		result_bool	result;
		U16	cause;
	} SMU_LOCK_RESPONSE_STRUCT;


typedef struct
{
	LOCAL_PARA_HDR
	U8	type;
} smu_password_required;

typedef struct
{
	LOCAL_PARA_HDR
	U8	pin_type; /* smu_security_type_enum */
	U8	pin[MAX_SIM_PASSWD_LEN];
	U8	new_pin[MAX_SIM_PASSWD_LEN];
} smu_verify_pin_req_struct;


typedef struct
{
	LOCAL_PARA_HDR
	result_bool	result;
	U8	status;
} smu_check_pin_status_rsp_struct;

typedef struct
{
	LOCAL_PARA_HDR
	result_bool	result;
	U8	type;
} nw_gprs_connect_type_rsp_struct;

typedef struct
{
	LOCAL_PARA_HDR
	U8	type;
} nw_gprs_connect_type_req_struct;
