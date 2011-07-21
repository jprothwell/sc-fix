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
 *	mmiapi.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends to dispatch the message
 *
 * Author:
 * -------
 * -------
 *
*==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

#define NEW_LOCAL_PTR(t) (t*)construct_local_para(sizeof(t),TD_CTRL);
 
#define SEND_MESSAGE(MOD_SRC,MOD_DEST,MSG_SAP,MSG_ID,LOCAL,PEER) \
do \
{ \
	ilm_struct *ilm_ptr = NULL; \
	ilm_ptr = allocate_ilm(MOD_SRC); \
	ilm_ptr->msg_id = (kal_uint16)MSG_ID; \
	ilm_ptr->local_para_ptr = (local_para_struct *)LOCAL; \
	ilm_ptr->peer_buff_ptr = (peer_buff_struct *)PEER; \
	SEND_ILM(MOD_SRC,MOD_DEST,MSG_SAP,ilm_ptr); \
} while (0)

#define SEND_COSMSG(MOD_DEST,P_COS_EVENT) \
	COS_SendEvent(GetMmiTaskHandle(MOD_DEST), P_COS_EVENT, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL)

#define GET_COSMSG(MOD_SRC,P_COS_EVENT) \
	COS_WaitEvent(GetMmiTaskHandle(MOD_SRC), P_COS_EVENT, COS_WAIT_FOREVER)

#define PEEK_COSMSG(MOD_SRC,P_COS_EVENT) \
	COS_WaitEvent(GetMmiTaskHandle(MOD_SRC), P_COS_EVENT, COS_NO_WAIT)

#ifdef MMIAPI_USING_BLOCKING
// engine initialize
extern void mmiapi_init_engine(void);
extern void mmiapi_register_blocking(module_type mod,kal_char* mutex_name);
extern void mmiapi_unregister_blocking(module_type mod);
extern void mmiapi_lock(module_type mod);
extern void mmiapi_unlock(module_type mod);
extern int mmiapi_mod_index(module_type mod);
extern mmiapi_blocking_data_struct* mmiapi_get_blocking_data(module_type mod);
#endif

