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
 *  Queue.c
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
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: Queue.c

  	PURPOSE		: Task queue implementation functions 

 

 

	DATE		: Dec' 28, 2002

**************************************************************/



//#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
//#include "l4dr1.h"
#include "oslmemory.h" 
#include "mmi_data_types.h"
#include "queuegprot.h"
#include "debuginitdef.h"
//dyj add start 20060526
#include  "cos.h"
#include "app_ltlcom.h"
#include "mmitask.h"
#include "mmiapi.h"
#include "mmi_trace.h"
#include "taskinit.h"

#define __align(x)

void *lastMemory;
TSIZE *lastSize;
MYQUEUE* qPtr;

/**************************************************************

	FUNCTION NAME		: OslIntCreateMsgQ

  	PURPOSE				: Creates Message Queue

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
oslMsgqid OslIntCreateMsgQ (
					  PS8 queue_name,
					  U32 max_msg_size,
					  U32 max_msgs)
{
#ifdef MMI_ON_WIN32
	win32_msgqid *msgqid;
	msgqid=(win32_msgqid *)OslMalloc(sizeof(win32_msgqid));
	strcpy(msgqid->q_name, queue_name);
	msgqid->q_max_message_size=max_msg_size;
	msgqid->q_max_messages=max_msgs;
	msgqid->q_messages=0;
	msgqid->q_queue_size=msgqid->q_max_message_size * msgqid->q_max_messages;
	msgqid->q_semaphore_read = CreateSemaphore(NULL, 0,1000 , NULL);
	msgqid->q_semaphore_write = CreateSemaphore(NULL, 0,1000 , NULL);
	msgqid->q_start= (PU32)OslMalloc(msgqid->q_queue_size);
	msgqid->q_message_size= (PU32)OslMalloc(msgqid->q_max_messages * 4);
	msgqid->q_read=0;
	msgqid->q_write=0;
	msgqid->q_tasks_waiting_read=0;
	msgqid->q_tasks_waiting_write=0;
	InitializeCriticalSection(&msgqid->q_cs);
	return	msgqid;
#else
	return NULL;	
#endif

}

/**************************************************************

	FUNCTION NAME		: OslIntDeleteMsgQ

  	PURPOSE				: Deletes Message Queue

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/

void OslIntDeleteMsgQ (oslMsgqid msgqid)
{
#ifdef MMI_ON_WIN32
	DeleteCriticalSection (&msgqid->q_cs);
	CloseHandle(msgqid->q_semaphore_read);
	CloseHandle(msgqid->q_semaphore_write);
	OslMfree(msgqid->q_start);
	OslMfree(msgqid->q_message_size);
	OslMfree(msgqid);
#endif
}

/**************************************************************

	FUNCTION NAME		: OslIntWriteMsgQ

  	PURPOSE				: Writes to Message Queue

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
OSLSTATUS OslIntWriteMsgQ(oslMsgqid msgqid,
					 void *msgPtr,
					 U32 msgSize,
					 OSLWAITMODE wait_mode)
{
#ifdef MMI_ON_WIN32
	U32 index;
	PU32 ptr;
	EnterCriticalSection(&msgqid->q_cs);
	if(msgSize > msgqid->q_max_message_size)
		{
		LeaveCriticalSection(&msgqid->q_cs);
		return OSL_MESSAGE_TOO_BIG;
		}
	/* Check whether the queue is full) */
	if(msgqid->q_messages==msgqid->q_max_messages)
		{
		if(wait_mode==OSL_INFINITE_WAIT)
			{
			++msgqid->q_tasks_waiting_write;
			LeaveCriticalSection(&msgqid->q_cs);
			WaitForSingleObject(msgqid->q_semaphore_write, INFINITE);
			EnterCriticalSection(&msgqid->q_cs);
			}
			else
			{
			LeaveCriticalSection(&msgqid->q_cs);
			return OSL_Q_FULL;
		}
}
	msgqid->q_message_size[msgqid->q_write]=msgSize;
	index=msgqid->q_write * (msgqid->q_max_message_size/4);
	ptr=msgqid->q_start+index;
	memcpy(ptr, msgPtr, msgSize);
	if(++msgqid->q_write == msgqid->q_max_messages) msgqid->q_write=0;
	++msgqid->q_messages;
	if(msgqid->q_tasks_waiting_read !=0)
	{
		ReleaseSemaphore(msgqid->q_semaphore_read, 1, NULL);
		msgqid->q_tasks_waiting_read-=1;
	}
	LeaveCriticalSection(&msgqid->q_cs);
#endif
return OSL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: OslIntReadMsgQ

  	PURPOSE				: Reades from Message Queue

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/

OSLSTATUS OslIntReadMsgQ(oslMsgqid msgqid,
					 void *msgPtr,
					 U32 *msgSize,
					 OSLWAITMODE wait_mode)
		{
#ifdef MMI_ON_WIN32
	EnterCriticalSection(&msgqid->q_cs);
	
	if(msgqid->q_read==msgqid->q_write)
	{
		if(wait_mode==OSL_INFINITE_WAIT)
		{
			++msgqid->q_tasks_waiting_read;
			LeaveCriticalSection(&msgqid->q_cs);
			WaitForSingleObject(msgqid->q_semaphore_read, INFINITE);
			EnterCriticalSection(&msgqid->q_cs);
		}
		else
		{
			LeaveCriticalSection(&msgqid->q_cs);
			return OSL_Q_EMPTY;
	}
}
	*msgSize=msgqid->q_message_size[msgqid->q_read];
	memcpy(msgPtr, msgqid->q_start + (msgqid->q_read * (msgqid->q_max_message_size/4)),*msgSize);
	if(++msgqid->q_read == msgqid->q_max_messages) msgqid->q_read=0;
	--msgqid->q_messages;

	if(msgqid->q_tasks_waiting_write!=0)
{
		ReleaseSemaphore(msgqid->q_semaphore_write, 1, NULL);
		--msgqid->q_tasks_waiting_write;
	}
	LeaveCriticalSection(&msgqid->q_cs);

#endif
return OSL_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: OslIntReceiveMsgExtQ

  	PURPOSE				: Reades from Task External Queue

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/

U32 OslIntReceiveMsgExtQ(oslMsgqid msgqid, MYQUEUE *Message)
{
//dyj del 20060526
//#if 0
	U32 msgSize=0;
	OslIntReadMsgQ(msgqid, Message, &msgSize, OSL_INFINITE_WAIT);

// Add by zhuoxz, 2009-09-27
#ifdef MMI_ON_WIN32
  if (Message->wSrcId == MOD_PST)
  {
    Message->wSrcId = MOD_L4C;
  }
  if (Message->wSrcId == MOD_PST_2)
  {
    Message->wSrcId = MOD_L4C_2;
  }
#endif
	return msgSize;
//#endif
}

extern void CheckAndPrintMsgId(U16 Id);

ilm_struct *allocate_ilm(module_type module_id)
{
     ilm_struct *pilm;
	  UINT32  nCallerAdd = 0x00;
	  COS_GET_RA(&nCallerAdd);
	 
     pilm = (ilm_struct *)OslMalloc(sizeof(ilm_struct));
	 memset(pilm, 0, sizeof(ilm_struct));
     pilm->src_mod_id = module_id;
	 	  COS_UpdateMemCallAdd(pilm,nCallerAdd);

     return pilm;
}


/**************************************************************

	FUNCTION NAME		: OslIntMsgSendExtQueue

  	PURPOSE				: Writes to Task External Queue

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void OslIntMsgSendExtQueue(MYQUEUE *Message)
{
	#ifdef MMI_ON_HARDWARE_P
		MYQUEUE *oslMessage;
		oslMessage = allocate_ilm(Message->oslSrcId);
		memcpy(oslMessage, Message, sizeof(MYQUEUE));	
		oslMessage->sap_id=INVALID_SAP;
#if 1
		do 
		{ 
			COS_EVENT ev; 
			ev.nEventId = oslMessage->msg_id; 
			ev.nParam1 = (UINT32)oslMessage; 
			ev.nParam3 = oslMessage->dest_mod_id; 

			SEND_COSMSG(oslMessage->dest_mod_id, &ev); 
		} while (0);
#else
		msg_send_ext_queue(oslMessage);
#endif
		
		CheckAndPrintMsgId (Message->msg_id);
	#else
		if(Message->oslDestId==MOD_MMI) 
			Message->oslDestId=MOD_PRT;
		if(Message->oslSrcId==MOD_MMI) 
			Message->oslSrcId=MOD_PRT;
		if(Message->oslDestId==MOD_WAP) 
			Message->oslDestId=MOD_PRT;

    // Add by zhuoxz,2009-6-19
    if (Message->oslDestId==MOD_L4C)
      Message->oslDestId=MOD_PST;
    if(Message->oslSrcId==MOD_L4C) 
      Message->oslSrcId=MOD_PST;
    if (Message->oslDestId==MOD_L4C_2)
      Message->oslDestId=MOD_PST_2;
    if(Message->oslSrcId==MOD_L4C_2) 
      Message->oslSrcId=MOD_PST_2;
    
    if (Message->oslDestId == MOD_PST || Message->oslDestId == MOD_PST_2)
    {
      OslIntWriteMsgQ(task_info_g1[MOD_PST].task_ext_qid, Message, 
        sizeof(MYQUEUE), OSL_INFINITE_WAIT);
    }
    else
    {
		OslIntWriteMsgQ(task_info_g1[Message->oslDestId].task_ext_qid, Message, 
						sizeof(MYQUEUE), OSL_INFINITE_WAIT);
    }
	#endif
}

/**************************************************************

	FUNCTION NAME		: OslIntConstructDataPtr

  	PURPOSE				: Constructs Data Ptr

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
typedef struct {
 	kal_uint8	ref_count;
   	kal_uint16	msg_len;
}LOCAL_PARA_HDR_T;
void * OslIntConstructDataPtr(U32 size)
{

	#ifdef MMI_ON_HARDWARE_P
	       //dyj modify start 20060630
	 	//return(construct_local_para(size, TD_CTRL));
	 	void *ptr;
		ptr = OslMalloc(size);
		((LOCAL_PARA_HDR_T *)ptr)->msg_len = size; 
	 	return ptr;
	 	//dyj modify end
	#else
		void *ptr;
		TSIZE *temp, tempdata;
		temp = OslMalloc(size + sizeof(tempdata));
		tempdata.s=size;
		memcpy(temp, &tempdata, sizeof(tempdata));
		ptr = (void*)((char*)temp + sizeof(tempdata));
		return ptr;	
	#endif	
}

/**************************************************************

	FUNCTION NAME		: OslIntFreeDataPtr

  	PURPOSE				: Frees Data Ptr

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
			
void OslIntFreeDataPtr(void * ptr)
{
	#ifdef MMI_ON_HARDWARE_P
	       //dyj modify start 20060630
		//free_local_para(ptr);	
		OslMfree(ptr);
	       //dyj modify end
	#else
		TSIZE *temp;
		temp = (void*)((char*)ptr - sizeof(TSIZE));
		OslMfree(temp);
	#endif
}

/**************************************************************

	FUNCTION NAME		: OslIntSizeofDataPtr

  	PURPOSE				: Returns Size of Data Ptr

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/

U32 OslIntSizeofDataPtr(void * ptr)
{
	#ifdef MMI_ON_HARDWARE_P
		 struct tsize{
			U8  ref_count;
		    U16 size;
			};
			return 0;
	#else
		typedef struct tsize{
			U32 size;
			}TSIZE;
		TSIZE *temp;
		if(ptr == NULL)
			return 0;
		temp = (void*)((char*)ptr - sizeof(TSIZE));
		return (U32)temp->size;
	#endif
}

/**************************************************************

	FUNCTION NAME		: OslIntFreeInterTaskMsg

  	PURPOSE				: Frees InterTask Mesg

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
void OslIntFreeInterTaskMsg(void * Message)
{
#ifdef MMI_ON_HARDWARE_P
	 
#else

	MYQUEUE* ptr;
	ptr = Message;
	if(ptr->oslSrcId==MOD_L4C)
	{
		ptr->oslSrcId=MOD_MMI;
		ptr->oslDestId=MOD_L4C;
	
		ptr->oslMsgId = 0;
		OslMsgSendExtQueue(ptr);
	}
	else
	{
	if(ptr->oslDataPtr != NULL)
		OslIntFreeDataPtr(ptr->oslDataPtr);
	}
#endif
}

#ifdef MMI_ON_WIN32
/**************************************************************

	FUNCTION NAME		: OslNumOfQMsgs

  	PURPOSE				: Get the message number in MMI queue

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: message number 

 
**************************************************************/
U8 OslNumOfQMsgs(void)
{
   U8 num;
   oslMsgqid msgqid = task_info_g1[MOD_PRT].task_ext_qid;

	EnterCriticalSection(&msgqid->q_cs);
   num = (U8)(msgqid->q_messages);
	LeaveCriticalSection(&msgqid->q_cs);
   return num;
}

#endif
#ifndef MEM_ULC_3216

#define CIRCQ_NODE_SIZE sizeof(MYQUEUE)

/* 
 * Because MMI task will receive extQ message into circular queue,
 * we increase the circular queue size from 30 to 50
 */
#define CIRCQ_NO_OF_NODES 65


__align(4) U8 circq_array[CIRCQ_NODE_SIZE * CIRCQ_NO_OF_NODES];
U8 circq_read=0, circq_write=0, circq_messages=0, max_circq_messages=0;

/**************************************************************

	FUNCTION NAME		: OslReadCircularQ

  	PURPOSE				: Reads from Circ Queue

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
U8 OslReadCircularQ(void *msgPtr)
{
	PRINT_INFORMATION_2((MMI_TRACE_G1_FRM, "OslReadCircularQ: <#[%d], read:[%d], write:[%d]>", circq_messages, circq_read, circq_write));
	if(circq_messages==0)
		return 0;
	memcpy(msgPtr, circq_array + (circq_read * CIRCQ_NODE_SIZE), CIRCQ_NODE_SIZE);
	if(++circq_read == CIRCQ_NO_OF_NODES) circq_read=0;
	--circq_messages;
	return 1;
}



/**************************************************************

	FUNCTION NAME		: OslLookUpLastCircularQMsg

  	PURPOSE				: Look up the last message in Circ Queue

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
U8 OslLookUpLastCircularQMsg(void *msgPtr)
{
	U16	last_read_index;

	if (circq_messages>0)
	{
		if (circq_write==0)
		{
			last_read_index = CIRCQ_NO_OF_NODES;
		}
		else
		{
			last_read_index = circq_write-1;
		}
		memcpy(msgPtr, circq_array + (last_read_index * CIRCQ_NODE_SIZE), CIRCQ_NODE_SIZE);
		return 1;
	}
	else
	{
		memset(msgPtr, 0, CIRCQ_NODE_SIZE);
		return 0;
	}
}


/**************************************************************

	FUNCTION NAME		: OslWriteCircularQ

  	PURPOSE				: Writes to Circ Queue

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
U8 OslWriteCircularQ(void *msgPtr)
{
	PRINT_INFORMATION_2((MMI_TRACE_G1_FRM, "OslWriteCircularQ: <#[%d], read:[%d], write:[%d]>", circq_messages, circq_read, circq_write));
	if(circq_messages==CIRCQ_NO_OF_NODES)
	{
		MMI_ASSERT(0);
		return 0;
	}
	memcpy(circq_array + (circq_write * CIRCQ_NODE_SIZE), msgPtr, CIRCQ_NODE_SIZE);
	if(++circq_write == CIRCQ_NO_OF_NODES) circq_write=0;
	++circq_messages;
	if (max_circq_messages<circq_messages)
	{
		max_circq_messages = circq_messages;
	}
	return 1;
}


/**************************************************************

	FUNCTION NAME		: OslNumOfCircularQMsgs

  	PURPOSE				: Get the message number

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
U8 OslNumOfCircularQMsgs(void)
{
	return circq_messages;
}


/**************************************************************

	FUNCTION NAME		: OslIsCircularQFull

  	PURPOSE				: Check if Circ Queue is full

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: nil

 
**************************************************************/
U8	OslIsCircularQFull(void)
{
	return (circq_messages==CIRCQ_NO_OF_NODES);
}
#endif

