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
 *  ConnectStructs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/include/ConnectStructs.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:59:30   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:40:02   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:26:02   zhangxb
 * Initial revision.
 * Revision 1.1.1.1  2006/06/08 12:43:16  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: ConnectStructs.h

  	PURPOSE		: Connect application definitions

 

 

	DATE		: 

**************************************************************/
#ifndef _PIXCOM_CONNECTSTRUCTS_H
#define _PIXCOM_CONNECTSTRUCTS_H

#include "mmi_data_types.h"

#define PLMNIDLENGTH 10
//micha1119
#define MAX_PLMN_LEN 6
#define MAX_PLMN_LIST_LEN 16
#define MAX_SIM_PASSWD_LEN 9
#define MAXPRLENTH 3
#define MAXNAMELENGHT 16
#define MAXPASSWORDLENGTH 16

typedef enum _cops_opcode{
	MMI_AUTO_SELECT,
	MMI_MANUAL_SEL,
	MMI_DEREG,
	MMI_MANUAL_THEN_AUTO
}COPSOPCODE;

typedef struct _select_net {
	COPSOPCODE opcode;
	U8 plmn_id[PLMNIDLENGTH];
}select_net;

typedef enum _sel_mode {
	AUTO_SELECT_MODE,
	MANUAL_SEL_MODE
}SELMODE;

typedef enum _l4c_result_enum
{
   MMI_L4C_OK,
   MMI_L4C_ERROR

}result_enum;
// ********** Hardware Declarations ******************** //
#ifdef MMI_ON_HARDWARE_P
#include "l4dr.h"
#define L4RESULTSTRUCT	l4_result_struct
#define PLMNRESULT		mmi_nw_get_plmn_list_rsp_struct
#define PLMNINFO		l4c_plmn_info_struct
#define SELECTIONMODE	plmn_selection_mode_enum
#define PLMNMODE		mmi_nw_set_plmn_select_mode_req_struct
#define SMUOPCODE		smu_plmnsel_op_enum
#define PLMNPREFS		mmi_smu_set_preferred_oper_list_req_struct
#define SELECTNET		mmi_nw_set_plmn_req_struct
#define SELECTNETRSP    mmi_nw_set_plmn_rsp_struct
#define PREFRESULT		mmi_smu_get_preferred_oper_list_rsp_struct
#define GETPREFERREDOPERLISTREQ	mmi_smu_get_preferred_oper_list_req_struct
/* Bug Fix 63,116 starts here */
#define ABORTRESPONSE	mmi_nw_abort_plmn_list_rsp_struct
/* Bug Fix 63,116 ends here */

#else 

//  ********* Windows Declarations ******************** //
typedef struct _mmi_nw_abort_plmn_list_rsp_struct{
	U8	result;
}ABORTRESPONSE; 
	
typedef struct L4_result_struct{
	U8 flag;
	U16 cause;
}L4RESULTSTRUCT;

typedef struct _plmn_info {
	U8 plmn_id[PLMNIDLENGTH];
	U8 status;
} PLMNINFO;

typedef struct _plmn_result	{
	L4RESULTSTRUCT result;
	U8 length;
	PLMNINFO list[MAX_PLMN_LIST_LEN];
}PLMNRESULT;

typedef enum _plmn_selection_mode_enum
{
    MANUAL_MODE,
    AUTOMATIC_MODE
} SELECTIONMODE;

typedef struct _mmi_nw_set_plmn_select_mode_req_struct{
	U8	mode;
}PLMNMODE;

typedef enum _smu_plmnsel_op_enum
{
   SMU_ADD_PLMNSEL,
   SMU_REMOVE_PLMNSEL,
   SMU_REPLACE_PLMNSEL
} SMUOPCODE;

typedef struct _mmi_smu_set_preferred_oper_list_req_struct
{
	U8			index;
	SMUOPCODE	opcode;
//micha1119
	U8			oper[MAX_PLMN_LEN+1];
}PLMNPREFS ;

typedef struct _mmi_nw_set_plmn_req_struct {
	COPSOPCODE opcode;
	U8 oper[PLMNIDLENGTH];
}SELECTNET;


typedef struct _mmi_nw_set_plmn_rsp_struct{
	L4RESULTSTRUCT result;
		U8	plmn[MAX_PLMN_LEN+1];
} SELECTNETRSP;
	
typedef struct _mmi_smu_get_preferred_oper_list_req_struct	{
		U8	index;
		U16	num;
}GETPREFERREDOPERLISTREQ;

typedef struct _mmi_smu_get_preferred_oper_list_rsp_struct {
	U8 result;
	U16 retnum;
	U8 oper_list[128];
}PREFRESULT;


#endif // End of Window Version

/*extern PLMNRESULT *gPlmnList;
extern PREFRESULT *gPrefList;
extern PLMNPREFS gPlmnPref;
extern U8 gPriority;
extern S8 gName[MAXNAMELENGHT];
*/
#endif



/* Structures for connect */

/* Network Related Messages */
/*
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint8	imei[15];
	} mmi_nw_get_imei_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
	} mmi_nw_abort_plmn_list_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	type;  //l4crac_attach_type_enum 
		kal_uint8	opcode; // l4crac_ps_attach_enum 
		kal_bool	is_poweroff;
	} mmi_nw_set_attach_req_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint16	cause;
	} mmi_nw_set_attach_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	type; // l4crac_attach_type_enum 
	} mmi_nw_get_attach_req_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint8	status; // l4crac_ps_attach_enum 
	} mmi_nw_get_attach_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	type;
	} mmi_nw_set_mobile_class_req_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint16	cause;
	} mmi_nw_set_mobile_class_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint8	type;
	} mmi_nw_get_mobile_class_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	mode;
	} mmi_nw_set_plmn_select_mode_req_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
	} mmi_nw_set_plmn_select_mode_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint8	mode;
	} mmi_nw_get_plmn_select_mode_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	opcode; // l4crac_cops_opcode_enum 
		kal_uint8	oper[MAX_PLMN_LEN];
	} mmi_nw_set_plmn_req_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		l4c_result_struct	result;
		kal_uint8	plmn[MAX_PLMN_LEN];
	} mmi_nw_set_plmn_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		l4c_result_struct	result;
		kal_uint8	length;
		l4c_plmn_info_struct	list[MAX_PLMN_LIST_LEN];
	} mmi_nw_get_plmn_list_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint8	oper[MAX_PLMN_LEN];
	} mmi_nw_get_current_plmn_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint8	rssi;
		kal_uint8	ver;
	} mmi_nw_get_signal_level_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	type; // band_selection_enum 
	} mmi_nw_set_band_req_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
	} mmi_nw_set_band_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint8	type; // band_selection_enum 
	} mmi_nw_get_band_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
	} mmi_nw_force_search_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	status; // l4c_rac_response_enum 
		kal_uint8	plmn[MAX_PLMN_LEN];
		kal_uint8	gsm_state;
		kal_uint8	gprs_state;
	} mmi_nw_attach_ind_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	lac;
		kal_uint16	cell_id;
		kal_uint8	gsm_state;
		kal_uint8	gprs_state;
	} mmi_nw_reg_state_ind_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	level;
	} mmi_nw_rx_level_ind_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	time_zone;
	} mmi_nw_time_zone_ind_struct;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	index;
		kal_uint8	opcode;
		kal_uint8	oper[MAX_PLMN_LEN];
	} mmi_smu_set_preferred_oper_list_req_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint16	cause;
	} mmi_smu_set_preferred_oper_list_rsp_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	index;
		kal_uint16	num;
	} mmi_smu_get_preferred_oper_list_req_struct;
	typedef struct
	{
		LOCAL_PARA_HDR
		kal_bool	result;
		kal_uint16	retnum;
		kal_uint8	oper_list[128];
	} mmi_smu_get_preferred_oper_list_rsp_struct;

*/	
 


