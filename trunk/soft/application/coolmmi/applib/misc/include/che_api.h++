/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2007
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
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
 *    che_api.h
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *   Cipher/Hash Engine API.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#ifndef _CHE_API_H
#define _CHE_API_H

#include "kal_release.h"

typedef struct _STCHE_{
#if defined(__HW_CHE__)

    kal_uint8           cheCKey[36]; 
    kal_uint8           cheDataAddr[120]; 

    kal_uint8           *cheKey;
    kal_uint8           *cheIV;
    kal_uint32           cheKeyLength;
    kal_uint32           cheIVLength;
    
    kal_bool            cheResetKeyFlag;
    kal_bool            cheResetIVFlag;
    kal_bool            cheFirstFlag;

#else
    
    kal_uint8           cheLastBlock[16];  
    kal_uint8           cheLastReminder[16];
    kal_uint8           cheKeySchedule[128]; 
    kal_uint32          cheLastLength;
    kal_uint8           *cheLastDstAddr;
     
    kal_uint8           *cheKey;
    kal_uint8           *cheIV;
    kal_uint32           cheKeyLength;
    kal_uint32           cheIVLength;
    
    kal_bool            cheFirstFlag;
	//RSA
    kal_uint8           *modulusN;
    kal_uint32           modulusNLen;
	  kal_uint8           *pubExp;
	  kal_uint32           pubExpLen;
	  kal_uint8           *privExpD;
	  kal_uint32           privExpDLen;
	  kal_uint8           *primeP;
	  kal_uint32           primePLen;
	  kal_uint8           *primeQ;
	  kal_uint32           primeQLen;
	  kal_uint8           *dModPm1;
	  kal_uint32           dModPm1Len;
	  kal_uint8           *dModQm1;
	  kal_uint32           dModQm1Len;
	  kal_uint8           *qInvModP;
	  kal_uint32           qInvModPLen;
    
    //Certicom temporary solution
/*    sb_GlobalCtx sbGloCtx;  
    sb_Context  sbCtx;                
    sb_YieldCtx yieldCtx;                 
    sb_RNGCtx rngCtx;                    

    sb_Params sbParams;          
    sb_Context sbContext;        
    sb_Key sbKey; 
*/
#endif 
} STCHE;



typedef enum {
   CHE_MD5,
   CHE_SHA1,
   CHE_DES,
   CHE_3DES,
   CHE_AES,

   CHE_MD4,
   CHE_MAC_MD5,
   CHE_MAC_SHA1,
   CHE_RC4,

   CHE_RSA
} CHE_TYPE;

typedef enum {
   CHE_MODE_NULL = 0,
   CHE_ECB,
   CHE_CBC,
   CHE_CTR,
   CHE_EAX,
   CHE_F8,
   CHE_SKIP,
   CHE_KEY_WRAP,
   CHE_KEY_UNWRAP
} CHE_OPERATION_MODE;


typedef enum {
   CHE_HASH,
   CHE_ENC,
   CHE_DEC,
   CHE_MAC_HASH,

   RSA_PUBLIC_ENC,
   RSA_PUBLIC_DEC,
   RSA_PRIVATE_ENC,
   RSA_PRIVATE_DEC
} CHE_ACTION;

typedef enum {
   CHE_GEN_KEY,
   CHE_SET_KEY,
   CHE_GET_KEY,
   CHE_DEL_KEY,

   CHE_SET_RSA_N,
   CHE_SET_RSA_E,
   CHE_SET_RSA_D,
   CHE_SET_RSA_P,
   CHE_SET_RSA_Q,
   CHE_SET_RSA_dModPm1,
   CHE_SET_RSA_dModQm1,
   CHE_SET_RSA_qInvModP

} CHE_KEY_ACTION;


void che_init(STCHE* che_context);

void che_key_action(STCHE* che_context, CHE_KEY_ACTION key_act, kal_uint8 *key, kal_int32 key_len);

void che_set_iv(STCHE* che_context, kal_uint8 *iv, kal_uint32 iv_len);

kal_bool che_process(STCHE* che_context, 
                     CHE_TYPE type, CHE_OPERATION_MODE mode, CHE_ACTION act,
                     kal_uint8 *source, kal_uint8 *dest, kal_uint32 source_length, kal_bool last_block);







#endif /* _CHE_API_H */  
