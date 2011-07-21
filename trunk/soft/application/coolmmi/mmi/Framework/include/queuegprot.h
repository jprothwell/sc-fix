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
 *	QueueGprot.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for message queue handle.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef _QUEUE_GPROT_H
#define _QUEUE_GPROT_H

//#include "mmi_data_types.h"
#include "oslmemory.h"

/***************************************************************************** 
* Define
*****************************************************************************/

#ifdef MMI_ON_WIN32
#if 0
#if (!defined LOCAL_PARA_HDR) && (!defined __NEWSIMULATOR )
	  #define LOCAL_PARA_HDR\
	  U8  ref_count;\
	  U16 msg_len;
#endif
#endif
#endif

#ifdef MMI_ON_HARDWARE_P
	#include "kal_release.h"
	#include "stack_common.h"
	#include "stack_msgs.h"
	#include "app_ltlcom.h"
	#include "lcd_ip_cqueue.h"
	#include "stack_types.h"
	#include "stack_ltlcom.h"
	#define MYQUEUE	ilm_struct
	 
	#define oslMsgqid	kal_msgqid    //dyj undel 20060609
	//#define OslReceiveMsgExtQ	receive_msg_ext_q
	//#define OslGetMyTaskIndex	kal_get_my_task_index
	//#define OslStackSetActiveModuleID	stack_set_active_module_id
	 
	
	/*define LOCAL_PARA_HDR\
	  U8  ref_count;\
	  U16 msg_len;*/
	#define oslModuleType module_type
	#define oslMsgType msg_type_t
	#define	oslParaType local_para_struct 
	#define	oslSrcId	src_mod_id
	#define oslDestId	dest_mod_id
	#define oslMsgId	msg_id
	#define oslSapId	sap_id
	#define oslDataPtr	local_para_ptr
	#define oslPeerBuffPtr peer_buff_ptr
	#define OSLWAITMODE kal_wait_mode
	#define	OSL_NO_WAIT KAL_NO_WAIT
	#define	OSL_INFINITE_WAIT KAL_INFINITE_WAIT

	
	 
	void OslIntMsgSendExtQueue(MYQUEUE *Message);
	void *OslIntConstructDataPtr (U32 size);
	void OslIntFreeDataPtr(void * ptr);
	U32 OslIntSizeofDataPtr(void * ptr);
	void OslIntFreeInterTaskMsg(void * Message);
	
	#define OslCreateMsgQ	kal_create_msg_q
	#define OslDeleteMsgQ	kal_delete_msg_q
	#define OslWriteMsgQ	kal_enque_msg
	#define	OslReadMsgQ		kal_deque_msg
	#define	OslMsgSendExtQueue	OslIntMsgSendExtQueue
	#define OslConstructDataPtr	OslIntConstructDataPtr
	#define	OslFreeDataPtr	OslIntFreeDataPtr
	//#define	OslSizeofDataPtr	OslIntSizeofDataPtr   //dyj del 20060630
	#define	OslFreeInterTaskMsg	OslIntFreeInterTaskMsg
	
#else
    #define MYQUEUE		WINTERTASKMSG
	#define oslModuleType U32
	#define oslMsgType U32
	#define	oslParaType void 
	#define oslPeerParaPtr void
	#define	oslSrcId	wSrcId
	#define oslDestId	wDestId
	#define oslMsgId	wMsgId
	#define oslSapId	wSapId 
	#define oslDataPtr	wDataPtr
	#define oslPeerBuffPtr	wPeerBuffPtr
	#define OslCreateMsgQ	OslIntCreateMsgQ
	#define OslDeleteMsgQ	OslIntDeleteMsgQ
	#define OslWriteMsgQ	OslIntWriteMsgQ
	#define	OslReadMsgQ		OslIntReadMsgQ
	
	typedef struct wintertaskmsgstruct {
		oslModuleType     	wSrcId;
		oslModuleType     	wDestId;
		oslMsgType			wMsgId;
		oslMsgType			wSapId;
		oslParaType		*wDataPtr;
		oslPeerParaPtr	*wPeerBuffPtr;
	} WINTERTASKMSG;

	
	oslMsgqid	OslIntCreateMsgQ(
					  PS8 queue_name,
					  U32 max_msg_size, //Size of Message in bytes (divisible by 4)
					  U32 max_msgs);     

	void OslIntDeleteMsgQ (oslMsgqid msgqid);

	OSLSTATUS OslIntWriteMsgQ(oslMsgqid msgqid,
					 void *msgPtr,
					 U32 msgSize,
					 OSLWAITMODE wait_mode);

	OSLSTATUS OslIntReadMsgQ(oslMsgqid msgqid,
					 void *msgPtr,
					 U32 *msgSize,
					 OSLWAITMODE wait_mode);

	U32 OslIntReceiveMsgExtQ(oslMsgqid msgqid, MYQUEUE *Message);
	void OslIntMsgSendExtQueue(MYQUEUE *Message);
	void *OslIntConstructDataPtr (U32 size);
	void OslIntFreeDataPtr(void * ptr);
	U32 OslIntSizeofDataPtr(void * ptr);
	void OslIntFreeInterTaskMsg(void * Message);

	#define OslCreateMsgQ	OslIntCreateMsgQ
	#define OslDeleteMsgQ	OslIntDeleteMsgQ
	#define OslWriteMsgQ		OslIntWriteMsgQ
	#define OslReadMsgQ		OslIntReadMsgQ
	#define OslReceiveMsgExtQ	OslIntReceiveMsgExtQ
	#define OslMsgSendExtQueue	OslIntMsgSendExtQueue
	#define OslGetMyTaskIndex	
	#define OslStackSetActiveModuleID	
	//#define OslConstructDataPtr	OslIntConstructDataPtr
		
	#define OslConstructDataPtr(size) lastMemory= (void *)((((U8 *)OslMalloc((size)+sizeof(TSIZE))) + sizeof(TSIZE)));\
									  lastSize=(TSIZE *)lastMemory - 1;\
									  lastSize->s=(U32)(size); 	
	
	
	#define	OslFreeDataPtr(frp)	if((frp)) OslMfree((U8 *)(frp) - sizeof(TSIZE));


	#define	OslSizeofDataPtr	((TSIZE *)((TSIZE *)(ptr) - 1))->s   //dyj del 20060626
	
	
	#define	OslFreeInterTaskMsg(frp) \
	{\
	extern MYQUEUE* qPtr;\
	qPtr = (MYQUEUE*)frp;\
	if(qPtr->oslSrcId==MOD_L4C)\
	{\
			qPtr->oslSrcId=MOD_MMI;\
			qPtr->oslDestId=MOD_L4C;\
			qPtr->oslMsgId = 0;\
			OslMsgSendExtQueue(qPtr);\
	}\
	else\
	{\
		if(*(frp.oslDataPtr) != NULL)\
		{\
			void *actualAddress;\
			actualAddress=(void *)((U8 *)(*(frp.oslDataPtr)) - sizeof(TSIZE));\
			OslMfree(actualAddress);\
		}\
	}\
	}
	
   U8 OslNumOfQMsgs(void);
#endif
/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/

U8 OslReadCircularQ(void *msgPtr);
U8 OslLookUpLastCircularQMsg(void *msgPtr);
U8 OslWriteCircularQ(void *msgPtr);
U8 OslNumOfCircularQMsgs(void);
U8	OslIsCircularQFull(void);


#endif //_QUEUE_GPROT_H

