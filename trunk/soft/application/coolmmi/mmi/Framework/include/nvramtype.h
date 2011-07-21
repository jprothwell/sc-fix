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
 *  NVRAMType.h
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
	Filename:		NVRMType.h
 
	Date Created:	
	Contains:		NVRAM declarations
**********************************************************************************/


#ifndef _NVRM_TYPE_H
#define _NVRM_TYPE_H


#include "mmi_data_types.h"



 

#define NVRM_FILES_PATH				"./SimulatorLayer/nvram/nvram"         

typedef enum
{
	DS_BYTE   = 1,  
	DS_SHORT   = 2, 
	DS_DOUBLE = 8 

}DATASIZE; /* Enum for caching purposes*/


typedef enum 
{

	NVRAM_READ_FAIL,
	NVRAM_READ_SUCCESS,
	NVRAM_ERROR_ACTUAL_READ_GREATER_THAN_REQUESTED_READ,
	NVRAM_ERROR_ACTUAL_READ_LESS_THAN_REQUESTED_READ,
	NVRAM_WRITE_SUCCESS,
	NVRAM_WRITE_FAIL

}MMINVRAMERRORSENUM;





typedef enum   //l4c_result_enum;
{
   MMI_OK,
   MMI_ERROR

} MMIRESULTENUM;

typedef enum  //l4c_cause_enum;
{
   MMI_NO_CAUSE,
   MMI_GEN_CAUSE,
   MMI_WRONG_PASSWORD,
   MMI_RMMI_INVALID_CMD,
   MMI_TCM_INVALID_PARAMETER,
   MMI_TCM_NSAPI_NOT_IN_USE

} MMICAUSEENUM;


#ifdef MMI_ON_WIN32

typedef struct  MMI_RESULT_STRUCT  //l4c_result_struct
{
   U8   flag;    /*l4c_result_enum*/
   U16  cause;

} MMIRESULT;



typedef struct _MMI_EQ_NVRAM_READ_REQ_STRUCT
{
	
	U8	file_idx;
	U8	para;
}MMIEQNVRAMREADREQ; 


typedef struct _MMI_EQ_NVRAM_READ_RSP_STRUCT
{
	
	MMIRESULT	result;
	U16			length;
	U8			data[1];

}MMIEQNVRAMREADRSP; 





typedef struct _MMI_EQ_NVRAM_WRITE_REQ_STRUCT
{
	
	U8	file_idx;
	U8	para;
	U16	length;
	U8	data[1];
}MMIEQNVRAMWRITEREQ; 



typedef struct _MMI_EQ_NVRAM_WRITE_RSP_STRUCT
{
	
	MMIRESULT	result;
	U8			file_idx;
	U16			para;
}MMIEQNVRAMWRITERSP;


typedef struct _MMI_EQ_NVRAM_RESET_REQ_STRUCT
{
		U8	reset_category;
		U8	lid;
}MMIEQNVRAMRESETREQ;

typedef struct _MMI_EQ_NVRAM_RESET_RSP_STRUCT
{
	MMIRESULT	result;
}MMIEQNVRAMRESETRSP;

#endif //#ifdef MMI_ON_WIN32

#endif // _NVRM_TYPE_H

