#ifndef _RDABT_MSG_C
#define _RDABT_MSG_C

/***************************************************************************** 
* Include
*****************************************************************************/
/* basic system service headers */
#include "kal_release.h"      	/* basic data type */
#include "stack_common.h"          /* message and module ids */
#include "stack_msgs.h"            /* enum for message ids */
#include "app_ltlcom.h"       	/* task message communiction */
//#include "syscomp_config.h"         /* type of system module components */
//#include "task_config.h"      	/* task creation */
#include "stacklib.h"        	   /* basic type for dll, evshed, stacktimer */
#include "event_shed.h"       	/* event scheduler */
#include "stack_timer.h"      	/* stack timer */
//#include "stdio.h"                  /* basic c i/o functions */



#include "rdabt.h"
#include "rdabt_msg.h"
#include "bt.h"
//#include "manage.h"
//#include "rda5868.h"
#include "rdabt_config.h"
#include "rdabt_task.h"
#include "rdabt_profile.h"
#include "rdabt_link_prim.h"
#include "rdabt_adp_link.h"
#include "ts.h"
#include "cos.h"
#include "stack_ltlcom.h"
#include "sxr_sbx.h"

#ifdef __BT_HFG_PROFILE__
#include "rdabt_hfp.h"
#include "rdabt_hsp.h"
#endif
#ifdef  __BT_OPP_PROFILE__
#include "rdabt_opp.h"
#endif
#ifdef  __BT_FTS_PROFILE__
#endif
#ifdef  __BT_FTC_PROFILE__
#endif
#ifdef  __BT_A2DP_PROFILE__
#include "l1audio.h"
#include "bt_a2dp_struct.h"
#include "rdabt_a2dp.h"
#endif
#ifdef  __BT_AVRCP_PROFILE__
#include "rdabt_avrcp.h"
#endif
#include "papi.h"
#include "mmi_trace.h"

extern kal_uint32 rdabt_get_seq_num(void);
#ifdef RDA_BT_DEBUG
extern void Debug_BT_Sap(BOOL dir,module_type src_mod,UINT32 msgid);
#endif
/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef struct
{
   kal_uint16 mi;
   kal_uint16 type;
   taskid   destid;
   sap_type sadid;
   rdabt_external_msg_func msg_func;
}rda_msg_table_struct;
/***************************************************************************** 
* Local Variable
*****************************************************************************/
extern rdabt_context_struct* rdabt_p;

/***************************************************************************** 
* Local Function
*****************************************************************************/

/***************************************************************************** 
* Global Variable
*****************************************************************************/
/* message lookup table */
const rda_msg_table_struct rda_msg_table[] =
{

   {LINK_Q, 0xFFFF, MOD_MMI,MOD_BT, NULL},
   {LINK_Q, LINK_DISCOVERY_RESULT_IND, MOD_MMI, MOD_BT,rdabt_link_discovery_result_ind},
   {LINK_Q, LINK_DISCOVERY_CNF, MOD_MMI, MOD_BT,rdabt_link_discovery_cnf},      
   {LINK_Q, LINK_DISCOVERY_CANCEL_CNF, MOD_MMI, MOD_BT,rdabt_link_inquiry_cancel_cnf},   
   {LINK_Q, LINK_WRITE_SCANENABLE_MODE_CNF, MOD_MMI, MOD_BT,rdabt_link_write_scanenable_mode_cnf},   
   {LINK_Q, LINK_WRITE_LOCALADDR_CNF, MOD_MMI, MOD_BT,rdabt_link_write_localaddr_cnf},   
   {LINK_Q, LINK_WRITE_AUTHENTICATION_MODE_CNF, MOD_MMI, MOD_BT,rdabt_link_write_authentication_cnf},   
   {LINK_Q, LINK_PIN_CODE_IND, MOD_MMI, MOD_BT,rdabt_link_pin_code_ind},   
   {LINK_Q, LINK_BONDING_RESULT_IND, MOD_MMI, MOD_BT,rdabt_link_bonding_result_ind},   
   {LINK_Q, LINK_BONDING_CNF, MOD_MMI, MOD_BT,rdabt_link_bonding_cnf},   



   
   


};

#define RDABT_MSG_TABLE_SIZE sizeof(rda_msg_table)/sizeof(rda_msg_table_struct)

/* table that list of primitive segmentation that uses one byte message id */
const kal_uint16 rdabt_one_byte_type_list[] = 
{   
   LINK_Q,
   A2DP_Q
};


#define RDABT_ONE_BYTE_TABLE_SIZE sizeof(rdabt_one_byte_type_list)/sizeof(kal_uint16)


/***************************************************************************** 
* Global Function
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*   rdabt_get_message
* DESCRIPTION
*   Get message from internal queue
* PARAMETERS
*   q    IN    ID of the queue
*   mi   IN    first parameter of message
*   mv   IN   second parameter of message
* RETURNS
*   TRUE if message exists; otherwise, FALSE.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
kal_bool rdabt_get_message(qid q, kal_uint16 *pmi, void **pmv)
{
   if (!(q & EXTERNAL_TASK_MASK))
   {
      rdabt_queue_entry_struct * message;

      ASSERT(q < RDABT_NUM_TASK);
      ASSERT(q == rdabt_p->curr_queue);

      message = rdabt_p->task_list[q].q_head;
      if (message)
      {
         /* take first message form queue */
         rdabt_p->task_list[q].q_head = message->next;
         if(rdabt_p->task_list[q].q_head == NULL)
            rdabt_p->task_list[q].q_tail = NULL;

         if(pmi)
            *pmi = message->mi;

         if(pmv)
            *pmv = message->mv;
         
         rdabt_p->num_int_msg--; 

         pFree(message);
         return TRUE;
      }
      return FALSE;
   }

   /* put assertion to prevent abnormal case */
   ASSERT(0);
   return FALSE; 
}

/*****************************************************************************
* FUNCTION
*   rdabt_msg_deinit
* DESCRIPTION
*   Remove all messages from all internal queues
* PARAMETERS
*   None
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
void rdabt_msg_deinit(void)
{
   kal_int32 i;
   kal_uint16 pmi;
   void *pmv = NULL;

   for(i=0; i<RDABT_NUM_TASK; i++)
   {
      rdabt_p->curr_queue = i;
      while(rdabt_get_message((qid)i, &pmi, &pmv))
      {
           if(pmv)
		pFree(pmv);
      }
   }
}


/*****************************************************************************
* FUNCTION
*   rdabt_msg_get_int_msg_id
* DESCRIPTION
*   function to retrieve the internal message id (type)
* PARAMETERS
*   mi      IN   message group
*   mv      IN  message instance
* RETURNS
*   internal message id
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
kal_uint16 rdabt_msg_get_int_msg_id(kal_uint16 mi, void*mv)
{
   kal_int32 i;
   kal_uint8 *msg1 = (kal_uint8*)mv;
   kal_uint16 *msg2 = (kal_uint16*)mv;

   for(i=0; i<RDABT_ONE_BYTE_TABLE_SIZE; i++)
      if(mi == rdabt_one_byte_type_list[i])
         return (kal_uint16)msg1[0];

   return msg2[0];
}


/*****************************************************************************
* FUNCTION
*   rdabt_do_put_message
* DESCRIPTION
*   Instance of sending message routine
* PARAMETERS
*   q        IN    destination of the message to be cancelled
*   mi      IN    buffer to store the first parameter of timer callback function     
*   mv      IN   buffer to store the second parameter of timer callback function
*   id      IN   sequence number of the sending event
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
void rdabt_do_put_message(qid q, kal_uint16 mi, void *mv, kal_uint32 id)
{

   if(q & EXTERNAL_TASK_MASK)   /* external tasks */
   {
    
      if( (mi & 0xff00) ==A2DP_Q)
      {
       #ifdef  __BT_A2DP_PROFILE__  	
         rdabt_a2dp_int_msg_hdlr( mi,mv );
	  #endif  
      }

     else if((mi &0xff00) == AVRCP_Q)
      {
 	#ifdef  __BT_AVRCP_PROFILE__	       
	  rdabt_avrcp_int_msg_hdlr(mi,mv);
       #endif	
      }
      else if((mi &0xff00) == HFP_Q)
      {
	  rdabt_hfp_int_msg_hdlr(mi,mv);
      }		
      else if((mi &0xff00) == HSP_Q)
      {
	  rdabt_hsp_int_msg_hdlr(mi,mv);
      }	
      else
      {
         kal_uint32 i;
         ilm_struct *msg;
         kal_uint16 *msg2 = (kal_uint16*)mv;

         kal_uint16 type;// = rdabt_msg_get_int_msg_id(mi, mv);
       type = msg2[0];
       kal_prompt_trace(0,"type = %d",type);
         /* look up message table */
         for(i=0;i<RDABT_MSG_TABLE_SIZE; i++)
         {
            if(rda_msg_table[i].mi == mi && rda_msg_table[i].type == type)
               break;
         }
   
         ASSERT(i<RDABT_MSG_TABLE_SIZE);
   	  ASSERT(i!=0);
         /* construct message */
         msg = allocate_ilm(MOD_BT);
         msg->src_mod_id = MOD_BT;
         msg->dest_mod_id = rda_msg_table[i].destid;
         msg->sap_id= rda_msg_table[i].sadid;
         rda_msg_table[i].msg_func((void*)msg, mv);
#ifdef RDA_BT_DEBUG		 
         Debug_BT_Sap(1,MOD_BT,msg->msg_id);
#endif				 
         rda_msg_send_ext_queue(msg);
 
      }
   }
   else /* internal task */
   {
   	asm("break 1");
      /* The message queue "q" is one of the Scheduler queues */
      rdabt_queue_entry_struct *msg;

      ASSERT(q<RDABT_NUM_TASK);

      msg = (rdabt_queue_entry_struct*)pMalloc(sizeof(rdabt_queue_entry_struct));
      msg->next = NULL;
      msg->mi = mi;
      msg->mv = mv;
      msg->id = id;

      if (rdabt_p->task_list[q].q_head == NULL)
      {
         rdabt_p->task_list[q].q_head = rdabt_p->task_list[q].q_tail = msg;
      }
      else
      {
         rdabt_p->task_list[q].q_tail->next = msg;
         rdabt_p->task_list[q].q_tail = msg;
      }

      rdabt_p->num_int_msg++;
   }
   
}


/*****************************************************************************
* FUNCTION
*   rdabt_put_message
* DESCRIPTION
*   Send message to destination queue
* PARAMETERS
*   q    IN    ID of the destination queue
*   mi   IN    first parameter of message
*   mv   IN   second parameter of message
* RETURNS
*   message sqeuence number (for cancel case).
* GLOBALS AFFECTED
*   None
*****************************************************************************/
scheduler_identifier rdabt_put_message(qid q, kal_uint16 mi, void *mv)
{

   scheduler_identifier msg_id = rdabt_get_seq_num();

   rdabt_do_put_message(q, mi, mv, msg_id);
   return msg_id;


   
}



/*****************************************************************************
* FUNCTION
*   rdabt_cancel_message
* DESCRIPTION
*   Remove message from internal queue
* PARAMETERS
*   q     IN    ID of the queue
*   mid   IN    message ID
*   pmi   IN    buffer to store first parameter of message
*   pmv   IN   buffer to second parameter of message
* RETURNS
*   TRUE if message exists; otherwise, FALSE.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/

kal_bool rdabt_cancel_message(qid q, scheduler_identifier mid, kal_uint16 *pmi, void **pmv)
{


   if(!(q & EXTERNAL_TASK_MASK))
   {
      rdabt_queue_entry_struct *curr_msg;
      rdabt_queue_entry_struct *prev_msg = NULL;
      rdabt_int_task_info_struct *task;
      ASSERT(q < RDABT_NUM_TASK);

      task = &rdabt_p->task_list[q];
      curr_msg = task->q_head;

      for (curr_msg = task->q_head; curr_msg != NULL; prev_msg = curr_msg, curr_msg = curr_msg->next)
      {
         if(curr_msg->id == mid)
            break;         
      }

      if(curr_msg)
      {
         /* remove message from list */
         if(curr_msg == task->q_head)
         {
            if(task->q_head == task->q_tail)
               task->q_head = task->q_tail = NULL;
            else
               task->q_head = curr_msg->next;               
         }
         else if(curr_msg == task->q_tail)
         {
            task->q_tail = prev_msg;
            task->q_tail->next = NULL;
         }
         else
            prev_msg->next = curr_msg->next;

         if(pmi)
            *pmi = curr_msg->mi;

         if(pmv)
            *pmv = curr_msg->mv;

         pFree(curr_msg);

         rdabt_p->num_int_msg--; 

         return TRUE;
      }

      return FALSE;
   }
   /* put assertion to prevent abnormal case */
   ASSERT(0);
   return FALSE; 
   
}


/*****************************************************************************
* FUNCTION
*   rdabt_send_msg_up
* DESCRIPTION
*   Send a message to upper layer
* PARAMETERS
*   dest                IN    destination module ID
*   msg_id             IN    ID of the message
*   local_param_ptr      IN    pointer to local parameter
*   peer_buf_ptr        IN    pointer to peer buffer
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
void rdabt_send_msg_up(kal_uint16 dest, kal_uint32 msg_id, void *local_param_ptr, void *peer_buf_ptr)
{
   ilm_struct *msg = allocate_ilm(MOD_BT);
#ifdef RDA_BT_DEBUG   
   Debug_BT_Sap(1,MOD_BT,msg_id);
 #endif  
   msg->src_mod_id = MOD_BT;
   msg->dest_mod_id = (module_type)dest;
   msg->sap_id = BT_APP_SAP;
   msg->msg_id = msg_id;
   msg->local_para_ptr = local_param_ptr;
   msg->peer_buff_ptr = peer_buf_ptr;
   rda_msg_send_ext_queue(msg);
}



/*****************************************************************************
* FUNCTION
*   rdabt_send_msg_to_head
* DESCRIPTION
*   Send a message to upper layer
* PARAMETERS
*   dest                IN    destination module ID
*   msg_id             IN    ID of the message
*   local_param_ptr      IN    pointer to local parameter
*   peer_buf_ptr        IN    pointer to peer buffer
* RETURNS
*   None.
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
/*
void rdabt_send_msg_to_head(kal_uint16 dest, kal_uint32 msg_id, void *local_param_ptr, void *peer_buf_ptr)
{

 #ifdef RDA_BT_DEBUG  
   Debug_BT_Sap(1,MOD_BT,msg_id);
#endif 
   ilm_struct *msg = allocate_ilm(MOD_BT);

   msg->src_mod_id = MOD_BT;
   msg->dest_mod_id = (module_type)dest;
   msg->sap_id = BT_APP_SAP;
   msg->msg_id = msg_id;
   msg->local_para_ptr = local_param_ptr;
   msg->peer_buff_ptr = peer_buf_ptr;
  
   rda_msg_send_ext_queue_to_head(msg);

}
*/
#endif /* #ifndef _rdabt_MSG_C */

