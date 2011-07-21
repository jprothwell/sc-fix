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
 *  Ucs2prot.h
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
	Filename:		UCS2prot.h
 
	Date Created:	Feb 5-2003
	Contains:		
**********************************************************************************/

#ifndef __UCS2_PROT_H__
#define __UCS2_PROT_H__


U8  UnicodeToUCS2Encoding(U16 unicode,U8 *charLength,U8 *arrOut);
U8  UCS2EncodingToUnicode(PU16 pUnicode ,PU8 arrOut);
S32 UCS2Strlen(const S8 *arrOut);
S32 UCS2Strcmp( const S8 *string1, const S8 *string2 );
S8 *UCS2Strcpy(S8 *strDestination, const S8 *strSource );

S32 UCS2Strncmp( const S8 *string1, const S8 *string2 , U32 size);
S8 *UCS2Strncpy(S8 *strDestination, const S8 *strSource , U32 size );
S8 *UCS2Strcat( S8 *strDestination, const S8 *strSource );
S8 *UCS2Strncat( S8 *strDestination, const S8 *strSource, U32 size);

S8 *UCS2StrAppendChar( S8 *strDestination, U16 ch);
S8 *UCS2StrNAppendChar( S8 *strDestination, U16 ch, U32 size );

U16 AnsiiToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer );
U16 AnsiiNToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer, U32 len  );
U16 UnicodeToAnsii(S8 *pOutBuffer, S8 *pInBuffer );
U16 UnicodeNToAnsii(S8 *pOutBuffer, S8 *pInBuffer, U32 len );
void csd_wsprintf(unsigned short *outstr, char *fmt,...);
S8 *UCS2Strchr(  const S8 *strSource , U16 ch );

extern U16 mmi_asc_to_ucs2(S8 *pOutBuffer, S8 *pInBuffer);
extern S8 *mmi_ucs2ncat(S8 *strDestination, const S8 *strSource, U32 size);
extern S32 mmi_ucs2strlen(const S8 *arrOut);
extern U16 mmi_ucs2_n_to_asc(S8 *pOutBuffer, S8 *pInBuffer, U32 len);
extern U16 mmi_asc_n_to_ucs2(S8 *pOutBuffer, S8 *pInBuffer, U32 len);

#endif





