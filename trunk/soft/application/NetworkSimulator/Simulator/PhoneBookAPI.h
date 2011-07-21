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
/**************************************************************

	FILENAME	: PhonebookAPI.h

  	PURPOSE		: phone book application 


	REMARKS		: nil

	AUTHOR		: nil

	DATE		: 

**************************************************************/

#ifndef _PHONEBOOKAPI_H
#define _PHONEBOOKAPI_H

//#include "L4Dr.h"
#define MAX_PB_ENTRIES_SIM	   150
#define MAX_PB_ENTRIES_PHONE   100

#define MAX_PB_ENTRIES		   (MAX_PB_ENTRIES_SIM+MAX_PB_ENTRIES_PHONE)
#define MAX_NAME_LENGTH		32
#define MAX_NUMBER_LENGTH	41

#define MAX_ENTRIES_IN_LIST	250
#define MAX_SIM_PASSWD_LEN	 9

#define LOCAL_PARA_HDR \
   U8	ref_count; \
   U16	msg_len;

#ifdef L4API 
#define PHB_NAME_STRUCT								l4_name_struct
#define PHB_NUMBER_STRUCT							l4c_number_struct
#define PHB_ENTRY_STRUCT							l4c_phb_entry_struct
#define PHB_RESULT_STRUCT							l4c_result_struct
#define	MMI_PHB_STORAGE_LOCATION					phb_storage_enum
#define	MMI_PHB_CAUSE_ENUM							phb_cause_enum
#define MMI_PHB_CHARSET_ENUM						phb_charset_enum
#define MSG_PHB_GET_ENTRY_BY_NAME_REQ_STRUCT		mmi_phb_get_entry_by_name_req_struct
#define MSG_PHB_GET_ENTRY_BY_NAME_RSP_STRUCT		mmi_phb_get_entry_by_name_rsp_struct
#define MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT		mmi_phb_get_entry_by_index_req_struct
#define MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT		mmi_phb_get_entry_by_index_rsp_struct
#define MSG_PHB_SET_ENRTY_REQ_STRUCT				mmi_phb_set_entry_req_struct
#define MSG_PHB_SET_ENRTY_RSP_STRUCT				mmi_phb_set_entry_rsp_struct
#define MSG_PHB_DELETE_ENTRY_REQ_STRUCT				mmi_phb_del_entry_req_struct
#define MSG_PHB_DELETE_ENTRY_RSP_STRUCT				mmi_phb_del_entry_rsp_struct
#define MSG_PHB_GET_PREFERRED_STORAGE_RSP_STRUCT	mmi_phb_get_preferred_storage_rsp_struct
#define MSG_PHB_SET_PREFERRED_STORAGE_REQ_STRUCT	mmi_phb_set_preferred_storage_req_struct
#define MSG_PHB_SET_PREFERRED_STORAGE_RSP_STRUCT	mmi_phb_set_preferred_storage_rsp_struct

#else 



typedef double 			DOUBLE;
typedef float			FLOAT;
typedef char			S8;			
typedef char			*PS8;		
typedef unsigned char   U8;			
typedef unsigned char	*PU8;	

typedef short			S16; 		
typedef short			*PS16; 		
typedef unsigned short  U16; 		
typedef unsigned short  *PU16;
typedef unsigned int	U32;		

typedef enum {
   MMI_PHB_NONE,
   MMI_PHB_ECC,
   MMI_PHB_FDN,
   MMI_PHB_BDN,
   MMI_PHB_MSISDN,
   MMI_PHB_SDN,
   MMI_PHB_PHONEBOOK
} MMI_PHB_TYPE;

typedef enum {
	MMI_STORAGE_NONE,
	MMI_SIM,
	MMI_NVRAM
} PHB_STORAGE_LOCATION_ENUM;

typedef struct {
    U8	name_length;			// Name Length
    U8	name_dcs;				// Name Data Coding Scheme
    U8 name[MAX_NAME_LENGTH];
} PHB_NAME_STRUCT;

typedef struct {
	U8 type;					//129-default, 145-international
	U8 length;
	U8 number[MAX_NUMBER_LENGTH];
} PHB_NUMBER_STRUCT;

/* Mandatory fields in a phonebook entry */ 
typedef struct {
	U8					storage;			//0-none, 1-SIM, 2-ME
	U8					type;
	U16					index;
	U16					record_index;
	PHB_NUMBER_STRUCT	tel;
	PHB_NAME_STRUCT		alpha_id;
} PHB_ENTRY_STRUCT;

/* Search result structure */
typedef struct
{
   U8   flag;	// OK,
				// ERROR
   U16  cause;  // L4C_GEN_CAUSE,
				// L4C_WRONG_PASSWORD,
				// RMMI_INVALID_CMD,
				// TCM_INVALID_PARAMETER,
				// TCM_NSAPI_NOT_IN_USE
} PHB_RESULT_STRUCT;

typedef struct {
	LOCAL_PARA_HDR
	U8				type;	
	U16 no_data;
	PHB_NAME_STRUCT name;
} MSG_PHB_GET_ENTRY_BY_NAME_REQ_STRUCT;

typedef struct
{
	LOCAL_PARA_HDR
	PHB_RESULT_STRUCT	result;
	U16					no_list;
	PHB_ENTRY_STRUCT	list[MAX_ENTRIES_IN_LIST];
} MSG_PHB_GET_ENTRY_BY_NAME_RSP_STRUCT;
		
typedef struct {
	LOCAL_PARA_HDR
	U8 type;	// 0-PHB_NONE,1-PHB_ECC,2-PHB_FDN
				// 3-PHB_BDN,4-PHB_MSISDN,5-PHB_SDN,
				// 6-PHB_PHONEBOOK
	U8 storage;	//0-none, 1-SIM, 2-NVRAM
	U16 index;
	U16 record_index;
	U16 no_data;
} MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT;

typedef struct {
	LOCAL_PARA_HDR
	PHB_RESULT_STRUCT	result;
	U16					no_list;
	PHB_ENTRY_STRUCT	list[MAX_ENTRIES_IN_LIST];
} MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT;

typedef struct {
	LOCAL_PARA_HDR
	U8	type;
	U16					no_data;
	PHB_ENTRY_STRUCT	list;
} MSG_PHB_SET_ENRTY_REQ_STRUCT;

typedef struct {
		LOCAL_PARA_HDR
		PHB_RESULT_STRUCT	result;
		U16	no_list;
		PHB_ENTRY_STRUCT	list[MAX_ENTRIES_IN_LIST];
} MSG_PHB_SET_ENRTY_RSP_STRUCT;

typedef enum {
   MMI_PHB_BCD = 0x01,
   MMI_PHB_ASCII = 0x04,
   MMI_PHB_UCS2 = 0x08
} MMI_PHB_CHARSET_ENUM;

typedef enum {
   MMI_PHB_NO_CAUSE,
   MMI_PHB_CAUSE_STORAGE_FULL,
   MMI_PHB_CAUSE_CAPACITY_EXCEEDED
} MMI_PHB_CAUSE_ENUM;

typedef enum {
   PB_FALSE=0,
   PB_TRUE=1
} MMI_BOOL;

typedef struct {
	LOCAL_PARA_HDR
	U8			type;
	U16			index;
	U8			no_data;
	U8			storage;
	MMI_BOOL	del_all;
} MSG_PHB_DELETE_ENTRY_REQ_STRUCT;

typedef struct {
	LOCAL_PARA_HDR
	PHB_RESULT_STRUCT result;
	U16 no_list;
	PHB_ENTRY_STRUCT list[MAX_ENTRIES_IN_LIST];
} MSG_PHB_DELETE_ENTRY_RSP_STRUCT;

typedef struct {
	LOCAL_PARA_HDR
	MMI_BOOL	result;
    U8			storage; //0-None, 1-SIM, 2-NVRAM
	U16			used;
	U16			total;
} MSG_PHB_GET_PREFERRED_STORAGE_RSP_STRUCT;

typedef struct {
	U8	pin[MAX_SIM_PASSWD_LEN];
	U8	storage; /* phb_storage_enum */	
} MSG_PHB_SET_PREFERRED_STORAGE_REQ_STRUCT;

typedef struct {
	LOCAL_PARA_HDR
	MMI_BOOL	result;
	U16	cause;	
} MSG_PHB_SET_PREFERRED_STORAGE_RSP_STRUCT;

#endif 

void *HandleWriteEntryRequest(void *setEntryReq);
void ReadFromFile(U8 PbookType);
void WriteToFile(U8 PbookTyp) ;


#endif //_PHONEBOOKAPI_H
