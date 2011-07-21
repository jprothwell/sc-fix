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

	FILENAME	: queue_gprot.h

  	PURPOSE		: Task queue implementation functions 

	REMARKS		: nil

	AUTHOR		: Vijay Vaidya

	DATE		: Aug' 28, 2002

**************************************************************/

#ifndef _QUEUE_GPROT_H
#define _QUEUE_GPROT_H

  #include "MMI_Data_Types.h"

	#define LOCAL_PARA_HDR  U8 temp;\
							U16 len;	
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
	#define MYQUEUE		WINTERTASKMSG
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
					  U32 max_msgs);    //Max No. of Messages

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
	#define	OslMsgSendExtQueue	OslIntMsgSendExtQueue
	#define OslConstructDataPtr	OslIntConstructDataPtr
	#define	OslFreeDataPtr	OslIntFreeDataPtr
	#define	OslSizeofDataPtr	OslIntSizeofDataPtr
	#define	OslFreeInterTaskMsg	OslIntFreeInterTaskMsg

#endif //_QUEUE_GPROT_H