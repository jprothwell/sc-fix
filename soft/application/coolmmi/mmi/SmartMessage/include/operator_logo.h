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
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *	Operator_logo.h
 *
 * Project:
 * --------
 *	Nokia Smart Messaging
 *
 * Description:
 * ------------
 *	This file defines global declaration for Operator Logo.
 *
 * Date :
 * ------
 *	9th May 2005	
 *
 * Author:
 * --------
 *	JaiPrakash Singh
 *
 ********************************************************************************/


#ifndef _MMI_OPERATOR_LOGO_H_
#define _MMI_OPERATOR_LOGO_H_

#ifdef __MMI_OPERATOR_LOGO__

/**************** Start : INCLUDE SECTION *************************/
#include "commonnsm.h"
/**************** End : INCLUDE SECTION ***************************/

/**************** Start : DEFINE SECTION*************************/

/**************** End : DEFINE SECTION***************************/

/**************** Start : TYPEDEFS SECTION *************************/

/* 
 *	Sequence as appear in actual data : 
 *	<ver><mmc><mnc><ota_header><Line-feed><OTA_DATA> 
 */
typedef struct
{
	U16 mmc;	/* <mmc> : Mobile Country Code */
	OTA_BMP_HEADER_STRUCT op_logo_ota_bmp_header;
	OTA_VERSION_STRUCT op_logo_ota_ver;
	U8	mnc;	/* <mnc> : Mobile Network Code */
} OP_LOGO_HEADER_STRUCT;

/**************** End : TYPEDEFS SECTION ***************************/

/**************** Start:GLOBAL FUNCTION DECLARATION *************************/
extern void mmi_op_logo_parse_ota_data(void);
/**************** End : GLOBAL FUNCTION DECLARATION *************************/

#endif  /*__MMI_OPERATOR_LOGO__*/

#endif	/*_MMI_OPERATOR_LOGO_H_*/