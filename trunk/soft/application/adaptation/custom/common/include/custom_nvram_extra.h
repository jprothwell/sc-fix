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
 * custom_nvram_extra.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *    This file defines the methods to manuplate the file object defined in NVRAM data items. 
 *   
 *
 * Author:
 * -------
 * -------
 ****************************************************************************/
#ifndef _CUSTOM_NVRAM_EXTRA_H
#define _CUSTOM_NVRAM_EXTRA_H
/***************************************************************************** 
* Define
*****************************************************************************/
/***************************************************************************** 
* Typedef 
*****************************************************************************/
/* 
 *   BEGIN:PH add for smu_security_info_struct (NVRAM_EF_MS_SECURITY_LID)
 * typedef struct
 * {
 *     kal_uint8                reg_ps_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
 *     kal_uint8                security_indication;
 *     kal_uint8                auto_lock_item;
 *     kal_uint8                np_code[NVRAM_EDITOR_NUM_OF_BYTE_NP * NVRAM_EDITOR_NUM_NP];
 *     kal_uint8                np_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
 *     kal_uint8                nsp_code[NVRAM_EDITOR_NUM_OF_BYTE_NSP * NVRAM_EDITOR_NUM_NSP];
 *     kal_uint8                nsp_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
 *     kal_uint8                np_of_sp[NVRAM_EDITOR_NUM_OF_BYTE_NP];
 *     kal_uint8                np_of_cp[NVRAM_EDITOR_NUM_OF_BYTE_NP];
 *     kal_uint8                gid1[NVRAM_EDITOR_NUM_GID];
 *     kal_uint8                gid2[NVRAM_EDITOR_NUM_GID];
 *     kal_uint8                sp_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
 *     kal_uint8                cp_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
 *     kal_uint8                imsi_code[NVRAM_EDITOR_NUM_OF_BYTE_IMSI];
 *     kal_uint8                imsi_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
 *     kal_uint8                phone_key[NVRAM_EDITOR_NUM_OF_BYTE_KEY];
 *     kal_uint8                last_imsi[NVRAM_EDITOR_NUM_OF_BYTE_IMSI];
 *     kal_uint8                pin1[NVRAM_EDITOR_NUM_PIN1];
 *     kal_uint8                pin1_valid;
 *     kal_uint8                phone_lock_verified;
 *  }smu_security_info_struct;
 *
 */
typedef enum {
    SMU_REG_PS_KEY_A,
    SMU_SECURITY_INDICATION_U8,
    SMU_AUTO_LOCK_ITEM_U8,
    SMU_NP_CODE_A,
    SMU_NP_KEY_A,
    SMU_NSP_CODE_A,
    SMU_NSP_KEY_A,
    SMU_NP_OF_SP_A,
    SMU_NP_OF_CP_A,
    SMU_GID1_A,
    SMU_GID2_A,
    SMU_SP_KEY_A,
    SMU_CP_KEY_A,
    SMU_IMSI_CODE_A,
    SMU_IMSI_KEY_A,
    SMU_PHONE_KEY_A,
    SMU_LAST_IMSI_A,
    SMU_PIN1_A,
    SMU_PIN1_VALID_U8,
    SMU_PHONE_LOCK_VERIFIED_U8
} smu_item_enum;

typedef struct {
    /* Data object ptr */
    void * smu_security_info_ptr;
    /* Load a new data object */
    kal_bool (* load) (void * /*pobjFromlid*/);
    /* Update a the data object */
    kal_bool (* update) (void * /* pobjTolid*/);
    /* Release the data object */
    kal_bool (* destoy) (void);
    /* Query the value of the item in kal_uint8 */
    kal_uint8 (* query_u8) (smu_item_enum /*eItem*/);
    /* Query the value of the item in kal_uint8[] */
    void (* query_a) (smu_item_enum /*eItem*/, kal_uint8 */*pstrTo*/, kal_uint8 /*chlen*/);
    /* Assign the value of the item in kal_uint8 */
    void (* assign_u8) (smu_item_enum /*eItem*/, kal_uint8 /*Value*/);
    /* Assign the value of the item in string with length */
    void (* assign_a) (smu_item_enum /*eItem*/, kal_uint8 * /*pValue*/, kal_uint8 /*len*/);
    /* Compare the string with the data item */
    kal_bool (* match) (smu_item_enum /* eItem */, kal_uint8 * /*pstr*/, kal_uint8 /*len*/);
} nvram_ef_ms_security_struct;
/***************************************************************************** 
             
* Extern Global 
Variable                                                                    
*****************************************************************************/ 

extern nvram_ef_ms_security_struct * pSmuSecurityInfog; 
/* END: PH smu_security_info_struct (NVRAM_EF_MS_SECURITY_LID)  */
/***************************************************************************** 
             
* Extern Global 
Function                                                                    
*****************************************************************************/ 

#endif /* _CUSTOM_NVRAM_EXTRA_H */
