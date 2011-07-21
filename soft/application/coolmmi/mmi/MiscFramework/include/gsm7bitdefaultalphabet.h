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
 *  GSM7BitDefaultAlphabet.h
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

/**************************************************************

	FILENAME	: GSM7BitDefaultAlphabet.h

  	PURPOSE		: .

 

	AUTHOR		: .

	DATE		: 11-07-03

**************************************************************/

#ifndef _PIXCOM_GSM_7BIT_DEFAULT_ALPHABET_H
#define _PIXCOM_GSM_7BIT_DEFAULT_ALPHABET_H

#include "mmi_data_types.h"

#ifndef OSL_MEMORY_DUMP
U8 *CovertStringForPlatform(U8 *data, U16 len, U8 format, U16 *outLen);
U8 *CovertStringForSIM(U8 *data, U16 len, U8 format, U16 *outLen);
U8 *CovertStringForGetSMS(U8 *data, U16 len, U8 format, U16 *outLen);
U8 *CovertStringForSetSMS(U8 *data, U16 len, U8 format, U16 *outLen);
#else
U8 *CovertStringForPlatformInt(U8 *data, U16 len, U8 format, U16 *outLen, S8 * fileName, S32 lineNumber);
U8 *CovertStringForSIMInt(U8 *data, U16 len, U8 format, U16 *outLen, S8 * fileName, S32 lineNumber);
#define CovertStringForPlatform(data,len,format,outLen) CovertStringForPlatformInt(data,len,format,outLen,__FILE__,__LINE__)
#define CovertStringForSIM(data,len,format,outLen) CovertStringForSIMInt(data,len,format,outLen,__FILE__,__LINE__)
#endif

U8 ConvertDigitToChar(U8 digit);
void *SwapEndian(void* Addr, const int Nb);
U8 *ConvertBCDToString(U8 *BCDNumber, U16 len, U16 *outLen);


extern void ConvertGSM7BitDefaultEncodingToAscii (U8* message, U8* msg, U16 length, U16 *outLen);
extern void ConvertAsciiEncodingToGSM7BitDefault (U8* message, U8* msg, U16 length, U16 *outLen);
extern U8   GSM7BitMapDefaultToASCIIByte         (U8 input);
extern U8   GSM7BitMapASCIIToDefaultByte         (U8 input);


typedef enum
{
   MMI_DEFAULT_DCS        = 0x00, /* GSM 7-bit */
   MMI_8BIT_DCS           = 0x04, /* 8-bit */
//micha1008
   MMI_UCS2_DCS           = 0x08,  /* UCS2 */   
   MMI_UCS2_81 = 0x09, /*Add for extra handle for UCS2 0x81*/
   MMI_UCS2_82 = 0x10 /*Add for extra handle for UCS2 0x82*/
   
} MMIDCSENUM;

S32 InputboxConvertGSMToDeviceEncoding(U8 *inBuffer,U8 *outBuffer, U8 inBufferLength,S32 outBufferSize,S32 UCS2_count,S32 GSM_ext_count,U8 flag);
S32 InputboxConvertDeviceEncodingToGSM(U8 *inBuffer,U8 *outBuffer,S32 outBufferSize,S32 UCS2_count,S32 GSM_ext_count,U8 flag);

#endif // _PIXCOM_GSM_7BIT_DEFAULT_ALPHABET_H




