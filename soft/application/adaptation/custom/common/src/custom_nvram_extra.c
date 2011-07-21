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
 * custom_nvram_extra.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *    This file is the implementation of the methods to manuplate the file object defined 
 *    in NVRAM data items.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 
/***************************************************************************** 
* Include
*****************************************************************************/
#include "kal_release.h"

/* Task Message Communication */
#include "stack_common.h"
#include "stack_msgs.h"
#include "app_ltlcom.h"    /* local/peer struct */
#include "stack_ltlcom.h"

/* Buffer Management */

#include "nvram_defs.h"
#include "nvram_user_defs.h"
#include "custom_nvram_editor_data_item.h"
#include "custom_nvram_extra.h"
#include "oslmemory.h"
/***************************************************************************** 
* Define of SMU
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
/* Load a new data object */
static kal_bool smu_load(void * /*pobjFromlid*/);
/* Update a the data object */
static kal_bool smu_update(void * /*pobjTolid*/);
/* Release the data object */
static kal_bool smu_destory(void);
/* Query the value of the item in kal_uint8 */
static kal_uint8 smu_query_u8(smu_item_enum /*eItem*/);
/* Query the value of the item in kal_uint8[] */
static void smu_query_a(smu_item_enum /*eItem*/, kal_uint8 */*pstrTo*/, kal_uint8 /*chlen*/);
/* Assign the value of the item in kal_uint8 */
static void smu_assign_u8(smu_item_enum /*eItem*/, 
                          kal_uint8 /*chValue*/);
/* Assign the value of the item in string with length */
static void smu_assign_a(smu_item_enum /*eItem*/, 
                         kal_uint8 * /*pstrValue*/, 
                         kal_uint8 /*chlen*/);
/* Compare the string with the data item */
static kal_bool smu_match(smu_item_enum /*eItem*/, 
                          kal_uint8 * /*pstrValue*/, 
                          kal_uint8 /*chlen*/);

 /***************************************************************************** 
* Typedef of SMU
*****************************************************************************/

/***************************************************************************** 
* Local Variable of SMU
*****************************************************************************/
static nvram_ef_ms_security_struct nvram_ef_ms_security_obj_g = {
    NULL,
    smu_load,
    smu_update,
    smu_destory,
    smu_query_u8,
    smu_query_a,
    smu_assign_u8,
    smu_assign_a,
    smu_match
};

/***************************************************************************** 
* Global Variable of SMU
*****************************************************************************/

nvram_ef_ms_security_struct * pSmuSecurityInfog = & nvram_ef_ms_security_obj_g;

/***************************************************************************** 
* Global Function of SMU
*****************************************************************************/

/***************************************************************************** 
* Local Function of SMU
*****************************************************************************/
/*added by elisa for declaration */
#include "mmi_trace.h"
#include "ts.h"
//ended of elisa         2008_9_27

/* Load a new data object */
static kal_bool smu_load(void * pobjFromlid)
{
    if(smu_destory())
    	kal_print("SMU: previous obj not free!");
    
    nvram_ef_ms_security_obj_g.smu_security_info_ptr = 
                                OslMalloc(sizeof(smu_security_info_struct));
    kal_mem_set((kal_uint8 *) nvram_ef_ms_security_obj_g.smu_security_info_ptr,
                              0xFF,
                              sizeof(smu_security_info_struct));
    kal_mem_cpy((kal_uint8 *) nvram_ef_ms_security_obj_g.smu_security_info_ptr,
    	                       (kal_uint8 *) pobjFromlid,
    	                       sizeof(smu_security_info_struct));
    return KAL_TRUE;
}
/* Update a the data object */
static kal_bool smu_update(void * pobjTolid)
{
    ASSERT(nvram_ef_ms_security_obj_g.smu_security_info_ptr != NULL);
    kal_mem_cpy((kal_uint8 *) pobjTolid,
    	                      (kal_uint8 *) nvram_ef_ms_security_obj_g.smu_security_info_ptr,
    	                      sizeof(smu_security_info_struct));
    return KAL_TRUE;
}
/* Release the data object */
static kal_bool smu_destory(void)
{
    if(nvram_ef_ms_security_obj_g.smu_security_info_ptr != NULL) {
        OslMfree(nvram_ef_ms_security_obj_g.smu_security_info_ptr);
        return KAL_TRUE;
    }
    return KAL_FALSE;
}
/* Query the value of the item in kal_uint8 */
static kal_uint8 smu_query_u8(smu_item_enum eItem)
{
    smu_security_info_struct * info_ptr = (smu_security_info_struct * )
                                           nvram_ef_ms_security_obj_g.smu_security_info_ptr;
    ASSERT(NULL != info_ptr);
    switch(eItem) {
    case SMU_SECURITY_INDICATION_U8:
    	return info_ptr->security_indication;
    	break;
    case SMU_AUTO_LOCK_ITEM_U8:
    	return info_ptr->auto_lock_item;
    	break;
    case SMU_PIN1_VALID_U8:
    	return info_ptr->pin1_valid;
    	break;
    case SMU_PHONE_LOCK_VERIFIED_U8:
    	return info_ptr->phone_lock_verified;
    	break;
    case SMU_REG_PS_KEY_A:   
    case SMU_NP_CODE_A:
    case SMU_NP_KEY_A:
    case SMU_NSP_CODE_A:
    case SMU_NSP_KEY_A:
    case SMU_NP_OF_SP_A:
    case SMU_NP_OF_CP_A:
    case SMU_GID1_A:
    case SMU_GID2_A:
    case SMU_SP_KEY_A:
    case SMU_CP_KEY_A:
    case SMU_IMSI_CODE_A:
    case SMU_IMSI_KEY_A:
    case SMU_PHONE_KEY_A:
    case SMU_LAST_IMSI_A:
    case SMU_PIN1_A:
    default:
    	ASSERT(0);  /* drop through! Wrong items */
       break;
    }
    return 0;
}
/* Query the value of the item in kal_uint8[] */
static void smu_query_a(smu_item_enum eItem, kal_uint8 *pstrTo, kal_uint8 chlen)
{
    smu_security_info_struct * info_ptr = (smu_security_info_struct * )
                                           nvram_ef_ms_security_obj_g.smu_security_info_ptr;
    ASSERT(NULL != info_ptr);
    switch(eItem) {
    case SMU_SECURITY_INDICATION_U8:
    case SMU_AUTO_LOCK_ITEM_U8:
    case SMU_PIN1_VALID_U8:
    case SMU_PHONE_LOCK_VERIFIED_U8:
    	ASSERT(0);
    	break;
    case SMU_REG_PS_KEY_A:   
    case SMU_NP_CODE_A:
    case SMU_NP_KEY_A:
    case SMU_NSP_CODE_A:
    case SMU_NSP_KEY_A:
    case SMU_NP_OF_SP_A:
    case SMU_NP_OF_CP_A:
    case SMU_GID1_A:
    case SMU_GID2_A:
    case SMU_SP_KEY_A:
    case SMU_CP_KEY_A:
    case SMU_IMSI_CODE_A:
    case SMU_IMSI_KEY_A:
    case SMU_PHONE_KEY_A:
    	break;
    case SMU_LAST_IMSI_A:
    	ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_IMSI);
    	kal_mem_cpy(pstrTo,info_ptr->last_imsi,NVRAM_EDITOR_NUM_OF_BYTE_IMSI);
    	break;
    case SMU_PIN1_A:
    	ASSERT(chlen == NVRAM_EDITOR_NUM_PIN1);
    	kal_mem_cpy(pstrTo,info_ptr->pin1,NVRAM_EDITOR_NUM_PIN1);
    	break;
    default:
    	ASSERT(0);  /* drop through! Wrong items */
       break;
    }
}
/* Assign the value of the item in kal_uint8 */
static void smu_assign_u8(smu_item_enum eItem, kal_uint8 chValue)
{
    smu_security_info_struct * info_ptr = (smu_security_info_struct * )
                                           nvram_ef_ms_security_obj_g.smu_security_info_ptr;
    ASSERT(NULL != info_ptr);    
    switch(eItem) {
    case SMU_SECURITY_INDICATION_U8:
    	info_ptr->security_indication = chValue;
    	break;
    case SMU_AUTO_LOCK_ITEM_U8:
    	info_ptr->auto_lock_item = chValue;
    	break;
    case SMU_PIN1_VALID_U8:
    	info_ptr->pin1_valid = chValue;
    	break;
    case SMU_PHONE_LOCK_VERIFIED_U8:
    	info_ptr->phone_lock_verified = chValue;
    	break;
    case SMU_REG_PS_KEY_A:   
    case SMU_NP_CODE_A:
    case SMU_NP_KEY_A:
    case SMU_NSP_CODE_A:
    case SMU_NSP_KEY_A:
    case SMU_NP_OF_SP_A:
    case SMU_NP_OF_CP_A:
    case SMU_GID1_A:
    case SMU_GID2_A:
    case SMU_SP_KEY_A:
    case SMU_CP_KEY_A:
    case SMU_IMSI_CODE_A:
    case SMU_IMSI_KEY_A:
    case SMU_PHONE_KEY_A:
    case SMU_LAST_IMSI_A:
    case SMU_PIN1_A:
    default:
    	ASSERT(0);  /* drop through! Wrong items */
       break;
    }
}
/* Assign the value of the item in string with length */
static void smu_assign_a(smu_item_enum eItem, kal_uint8 * pstrValue, kal_uint8 chlen)
{
    smu_security_info_struct * info_ptr = (smu_security_info_struct * )
                                           nvram_ef_ms_security_obj_g.smu_security_info_ptr;
    ASSERT(NULL != info_ptr);    
    switch(eItem) {
    case SMU_SECURITY_INDICATION_U8:
    case SMU_AUTO_LOCK_ITEM_U8:
    case SMU_PIN1_VALID_U8:
    case SMU_PHONE_LOCK_VERIFIED_U8:
    	 ASSERT(0);
        break;
    case SMU_REG_PS_KEY_A: 
    	 break;
    case SMU_NP_CODE_A:
    	 ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_NP);
    	 kal_mem_cpy(info_ptr->np_code, pstrValue,NVRAM_EDITOR_NUM_OF_BYTE_NP);
    	 break;
    case SMU_NP_KEY_A:
    	 break;
    case SMU_NSP_CODE_A:
    	 ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_NSP);
    	 kal_mem_cpy(info_ptr->nsp_code,pstrValue,NVRAM_EDITOR_NUM_OF_BYTE_NSP);
    case SMU_NSP_KEY_A:
    	 break;
    case SMU_NP_OF_SP_A:
    	 ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_NP);
    	 kal_mem_cpy(info_ptr->np_of_sp,pstrValue,NVRAM_EDITOR_NUM_OF_BYTE_NP);
    	 break;
    case SMU_NP_OF_CP_A:
    	 ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_NP);
    	 kal_mem_cpy(info_ptr->np_of_cp,pstrValue,NVRAM_EDITOR_NUM_OF_BYTE_NP);
    	 break;
    case SMU_GID1_A:
    	 ASSERT(chlen <= NVRAM_EDITOR_NUM_GID);
    	 kal_mem_cpy(info_ptr->gid1,pstrValue,chlen);
    	 break;
    case SMU_GID2_A:
    	 ASSERT(chlen <= NVRAM_EDITOR_NUM_GID);
    	 kal_mem_cpy(info_ptr->gid2,pstrValue,chlen);
    	 break;
    case SMU_SP_KEY_A:
    	 break;
    case SMU_CP_KEY_A:
    	 break;
    case SMU_IMSI_CODE_A:
    	 ASSERT(chlen <= NVRAM_EDITOR_NUM_OF_BYTE_IMSI);
    	 kal_mem_cpy(info_ptr->imsi_code,pstrValue,chlen);
    	 break;
    case SMU_IMSI_KEY_A:
    	 break;
    case SMU_PHONE_KEY_A:
    	ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_KEY);
    	kal_mem_cpy(info_ptr->phone_key,pstrValue,NVRAM_EDITOR_NUM_OF_BYTE_KEY);
    	break;    	
    case SMU_LAST_IMSI_A:
    	ASSERT(chlen <= NVRAM_EDITOR_NUM_OF_BYTE_IMSI);
    	kal_mem_cpy(info_ptr->last_imsi, pstrValue, chlen);
    	break;
    case SMU_PIN1_A:
    	ASSERT(chlen == NVRAM_EDITOR_NUM_PIN1);
    	kal_mem_cpy(info_ptr->pin1, pstrValue, NVRAM_EDITOR_NUM_PIN1);
    	break;
    default:
    	ASSERT(0);
       break;
    }
}
/* Compare the string with the data item */
// wangbei add start 20060614
__inline kal_int32 kal_mem_cmp(const void* src1, const void* src2, kal_uint32 size)
{
	return FALSE;    //add by elisa, for remove the warnings
}
// wangbei add end
static kal_bool smu_match(smu_item_enum eItem, kal_uint8 * pstrValue, kal_uint8 chlen)
{
    smu_security_info_struct * info_ptr = (smu_security_info_struct * )
                                           nvram_ef_ms_security_obj_g.smu_security_info_ptr;
    kal_uint8 idx = 0;
    ASSERT(NULL != info_ptr);     
    switch(eItem) {
    case SMU_SECURITY_INDICATION_U8:
    	ASSERT(chlen == sizeof(kal_uint8));
    	if(info_ptr->security_indication == *pstrValue)
    	    return KAL_TRUE;
    	else
    	    return KAL_FALSE;
    	break;
    case SMU_AUTO_LOCK_ITEM_U8:
    	ASSERT(chlen == sizeof(kal_uint8));    	
    	if(info_ptr->auto_lock_item == *pstrValue)
    	    return KAL_TRUE;
    	else
    	    return KAL_FALSE;
    	break;
    case SMU_PIN1_VALID_U8:
    	ASSERT(chlen == sizeof(kal_uint8));    	
    	if(info_ptr->pin1_valid == *pstrValue)
    	    return KAL_TRUE;
       else
    	    return KAL_FALSE;    	
    	break;
    case SMU_PHONE_LOCK_VERIFIED_U8:
    	ASSERT(chlen == sizeof(kal_uint8));    	
    	if(info_ptr->phone_lock_verified == *pstrValue) 
    	    return KAL_TRUE;
       else
    	    return KAL_FALSE;    	    	
    	break;
    case SMU_REG_PS_KEY_A:
    	ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_KEY);
    	if(kal_mem_cmp(info_ptr->reg_ps_key,
    		         pstrValue,
    		         NVRAM_EDITOR_NUM_OF_BYTE_KEY) == 0) 
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;
    	break;
    case SMU_NP_CODE_A:
    	ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_NP);
    	for(idx = 0;idx < NVRAM_EDITOR_NUM_NP;idx++) {
           if(kal_mem_cmp(pstrValue,
           	              (kal_uint8 *) &(info_ptr->np_code[idx * NVRAM_EDITOR_NUM_OF_BYTE_NP]),
           	              NVRAM_EDITOR_NUM_OF_BYTE_NP) == 0)
               return KAL_TRUE; /* Find one! */
    	}
    	    return KAL_FALSE;
    	break;
    case SMU_NP_KEY_A:
    	ASSERT(chlen <= NVRAM_EDITOR_NUM_OF_BYTE_KEY);
    	if(kal_mem_cmp(info_ptr->np_key,
    		         pstrValue,
    		         chlen) == 0) 
           return KAL_TRUE;
       else
    	    return KAL_FALSE;
    	break;
    case SMU_NSP_CODE_A:
    	ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_NSP);
    	for(idx = 0;idx < NVRAM_EDITOR_NUM_NSP;idx++) {
           if(kal_mem_cmp(pstrValue,
           	              (kal_uint8 *) &(info_ptr->nsp_code[idx * NVRAM_EDITOR_NUM_OF_BYTE_NSP]),
           	              NVRAM_EDITOR_NUM_OF_BYTE_NSP) == 0)
               return KAL_TRUE; /* Find one! */
    	}
    	    return KAL_FALSE;	
    	break;
    case SMU_NSP_KEY_A:
    	ASSERT(chlen <= NVRAM_EDITOR_NUM_OF_BYTE_KEY);
    	if(kal_mem_cmp(info_ptr->nsp_key,
    		         pstrValue,
    		         chlen) == 0) 
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;
    	break;
    case SMU_NP_OF_SP_A:
    	ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_NP);
    	if(kal_mem_cmp(pstrValue,
    		           info_ptr->np_of_sp,
    		           NVRAM_EDITOR_NUM_OF_BYTE_NP)==0)
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;
    	break;
    case SMU_NP_OF_CP_A:
    	ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_NP);
    	if(kal_mem_cmp(pstrValue,
    		           info_ptr->np_of_cp,
    		           NVRAM_EDITOR_NUM_OF_BYTE_NP)==0)
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;    	
    	break;
    case SMU_GID1_A:
    	ASSERT(chlen <= NVRAM_EDITOR_NUM_GID);
    	if(kal_mem_cmp(info_ptr->gid1,
    		         pstrValue,
    		         chlen) == 0) 
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;
    	break;
    case SMU_GID2_A:
    	ASSERT(chlen <= NVRAM_EDITOR_NUM_GID);
    	if(kal_mem_cmp(info_ptr->gid2,
    		         pstrValue,
    		         chlen) == 0) 
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;    	
    	break;
    case SMU_SP_KEY_A:
    	ASSERT(chlen <= NVRAM_EDITOR_NUM_OF_BYTE_KEY);
    	if(kal_mem_cmp(info_ptr->sp_key,
    		         pstrValue,
    		         chlen) == 0) 
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;
    	break;
    case SMU_CP_KEY_A:
    	ASSERT(chlen <= NVRAM_EDITOR_NUM_OF_BYTE_KEY);
    	if(kal_mem_cmp(info_ptr->cp_key,
    		         pstrValue,
    		         chlen) == 0) 
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;
    	break;
    case SMU_IMSI_CODE_A:
    	ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_IMSI);
    	if(kal_mem_cmp(info_ptr->imsi_code,
    		         pstrValue,
    		         NVRAM_EDITOR_NUM_OF_BYTE_IMSI) == 0) 
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;    	
    	break;
    case SMU_IMSI_KEY_A:
    	ASSERT(chlen <= NVRAM_EDITOR_NUM_OF_BYTE_KEY);
    	if(kal_mem_cmp(info_ptr->imsi_key,
    		         pstrValue,
    		         chlen) == 0) 
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;
    	break;
    case SMU_PHONE_KEY_A:
    	ASSERT(chlen <= NVRAM_EDITOR_NUM_OF_BYTE_KEY);
    	if(kal_mem_cmp(info_ptr->phone_key,
    		         pstrValue,
    		         chlen) == 0) 
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;
    	break;
    case SMU_LAST_IMSI_A:
    	ASSERT(chlen == NVRAM_EDITOR_NUM_OF_BYTE_IMSI);
    	if(kal_mem_cmp(info_ptr->last_imsi,
    		         pstrValue,
    		         NVRAM_EDITOR_NUM_OF_BYTE_IMSI) == 0) 
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;
    	break;
    case SMU_PIN1_A:
    	ASSERT(chlen == NVRAM_EDITOR_NUM_PIN1);
    	if(kal_mem_cmp(info_ptr->pin1,
    		         pstrValue,
    		         NVRAM_EDITOR_NUM_PIN1) == 0) 
           return KAL_TRUE;
    	else
    	    return KAL_FALSE;    	
    	break;
    default:
    	ASSERT(0);
       break;
    }
    return 0;
}

 
/* END: PH smu_security_info_struct (NVRAM_EF_MS_SECURITY_LID)  */
 
