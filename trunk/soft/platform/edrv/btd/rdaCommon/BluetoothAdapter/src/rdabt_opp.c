
#ifndef _RDABT_OPP_C
#define _RDABT_OPP_C


/* basic system service headers */
#include "stdlib.h"
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
#include "gpio_sw.h"               /* for gpio */

#include "nvram_struct.h"           /* for nvram read/write */
#include "nvram_user_defs.h"
#include "cos.h"


#include "host_config.h"

#include "bt.h"

#include "bluetooth_bm_struct.h"
#include "mmi_trace.h"

#include "rdabt.h"
#include "rdabt_msg.h"
#include "rdabt_opp.h"
#include "papi.h"
#include "manager.h"

/***************************************************************************** 
* Define
*****************************************************************************/

/***************************************************************************** 
* Typedef 
*****************************************************************************/
extern S32 UCS2Strlen(const S8 *arrOut);
extern void pFree(void *buffer);

/***************************************************************************** 
* Local Variable
*****************************************************************************/

rdabt_client_struct rdabt_client;
rdabt_client_struct* rdabt_clientp = &rdabt_client;

rdabt_opps_struct rdabt_opps;
rdabt_opps_struct* rdabt_oppsp = &rdabt_opps;

rdabt_ftps_struct rdabt_ftps;
rdabt_ftps_struct* rdabt_ftpsp = &rdabt_ftps;

static	u_int8 NAME_obex;
static   t_DataBuf *FTPSbuffer;
t_obex_nonce nonce;
u_int8 temp_pin_len = 1;
u_int8 temp_pin_code[1]={0x31};
u_int8 securitySet=0;
scheduler_identifier  oppc_timer = 0,opps_timer = 0,ftps_timer = 0;
kal_uint32 goep_rfcomm_tidOPPC;  /* goep_tid of bt */
kal_uint8 Timer_REOUT = 0;


void rdabt_goep_disconnect_req_reply(kal_uint32 req_id,  kal_uint8 goep_conn_id);
void rdabt_goep_disconnect_rsp_oppc(u_int8 status, u_int8* data, u_int16 length);
void rdabt_goep_callback_ReleaseACL(void *dbEntry, int status);
void rdabt_goep_disconnect_rsp_oppc_callback(void);
void rdabt_goep_connect_req_reply(t_bdaddr address,u_int8 channel,u_int8* formatc);

 int  MGR_GetACLState(t_bdaddr addr) ;
 int  MGR_SetACLCallBack(t_bdaddr address, int index , void (*callbackfunc)(void *dbEntry, int status_t));
void prh_l2_acl_timer_expired(void *arg);
void  prh_l2_acl_shutdown_all(u_int32 flags);


/***************************************************************************** 
* Global Function
*****************************************************************************/
void rdabt_goep_init(void) 
{
	goep_rfcomm_tidOPPC = 0xff;
       rdabt_clientp->goep_tidClient = 0xff;             
       rdabt_clientp->req_idClient = 0xffff;               
       rdabt_clientp->ClientStatus = GOEP_Client_Idle;
	
       rdabt_oppsp->goep_tid_OPPS = 0xffff;            
       rdabt_oppsp->req_idOPPS = 0xff;               
       rdabt_oppsp->OPPStid = 0xffff;                  
       rdabt_oppsp->OPPSfirst=1;                       
	rdabt_oppsp->OPPSstatus = GOEP_Client_Idle;

       rdabt_ftpsp->goep_tid_FTPS =0xffff;             
       rdabt_ftpsp->req_idFTPS = 0xff;               
       rdabt_ftpsp->FTPStid = 0xffff;                  
       rdabt_ftpsp->FTPSfirst=1;
	rdabt_ftpsp->FTPSstatus = GOEP_Client_Idle;
	oppc_timer = 0;
	opps_timer = 0;
	ftps_timer = 0;
	Timer_REOUT = 0;

       OBEX_Init_Variable();

}
/*****************************************************************************
 * FUNCTION
 *  OBEX_Connect_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void OBEX_ACL_oppC_callback(t_bdaddr address, int status)
{
    kal_prompt_trace(0,"1 Got response of %x from a OBEX_ACL_oppC_callback",status);
    switch(status) 
    {
        case BT_NOERROR:
        {
            break;
        }
        case BT_TIMEOUT:
        {
			if(GOEP_Client_Connect == rdabt_clientp->ClientStatus)
			    kal_prompt_trace(0,"2 Got response of %x from a OBEX_ACL_oppC_callback",status);
			//rdabt_goep_disconnect_req_reply(rdabt_clientp->req_idClient, rdabt_clientp->goep_tidClient);
            break;
        }
        default:
        {
            /*
            this was a failure I think that I will have to kill the thread.
            */
            kal_prompt_trace(0,"3 Got response of %x from a OBEX_ACL_oppC_callback",status);
            break;
        }
    }
}
/*****************************************************************************
 * FUNCTION
 *  OBEX_Connect_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void OBEX_Connect_callback(kal_uint8 status, 
kal_uint8* finger, 
kal_uint16 length)
{
    goep_connect_rsp_struct *param_ptr ;

    kal_prompt_trace(0, "OBEX_Connect_callback...  20090220-Rdabt_opp 1 !@!!!!!!,rdabt_clientp->goep_tidClient = %d,rdabt_clientp->ClientStatus = %d",rdabt_clientp->goep_tidClient,rdabt_clientp->ClientStatus);	

    param_ptr = (goep_connect_rsp_struct*) construct_local_para(sizeof(goep_connect_rsp_struct), TD_CTRL);
    kal_prompt_trace(0, "OBEX_Connect_callback...  20090220-Rdabt_opp 2 !@!!!!!!,*finger = %x",*finger);	

    switch(*finger) 
    {
        case OBEX_OK:
        {
            /*I need to read back the size field.*/
            param_ptr->peer_mru =(finger[5] << 8) + finger[6] - 200;
            param_ptr->rsp_code = GOEP_STATUS_SUCCESS;//0;
	     rdabt_clientp->ClientStatus = GOEP_Client_Connect;
	     if (rdabt_clientp->isFtpClient)
	     {
	     		t_obex_header_offsets headers;
			u_int16 offset_len = OBEX_CONNECT_HEADER;	
	     		OBEX_ExtractHeaders(&headers, &offset_len,finger,length);
			if(headers.cid)
			{
				rdabt_clientp->goep_cidClient = 
				 ((finger+headers.cid)[0] << 24) + ((finger+headers.cid)[1] << 16) + ((finger+headers.cid)[2] << 8) + ((finger+headers.cid)[3]);
				
			}
            		kal_prompt_trace(0,"OBEX_Connect_callback rdabt_clientp->goep_cidClient =%d",rdabt_clientp->goep_cidClient);
	     }
            break;
        }
        
        case 1:
        {
		if(oppc_timer)
		{
			pTimerCancel(oppc_timer);
			oppc_timer = 0;
		}

	       OBEX_TransportDisconnect(rdabt_clientp->goep_tidClient);	
		if(GOEP_Client_Idle == rdabt_clientp->ClientStatus  || GOEP_Client_pending_Disconnect == rdabt_clientp->ClientStatus)
		{
			rdabt_clientp->ClientStatus = GOEP_Client_Disconnect;		

			if(!MGR_SetACLCallBack(rdabt_clientp->ClientAddress,0,rdabt_goep_callback_ReleaseACL))
				kal_prompt_trace(0, "MGR_SetACLCallBack!@!!!!!!");	
			else
				kal_prompt_trace(0, "MGR_SetACLCallBack fail");	

			return;
		}
            param_ptr->peer_mru = 0;
            param_ptr->rsp_code = GOEP_STATUS_FAILED;//0;
	     rdabt_clientp->ClientStatus = GOEP_Client_Idle;
	     MGR_GetACLState(rdabt_clientp->ClientAddress);	 
            break;
        }

        default:
        {
            /*
            this was a failure I think that I will have to kill the thread.
            */
            param_ptr->rsp_code = GOEP_STATUS_BUSY;
            param_ptr->peer_mru = 0;
            kal_prompt_trace(0,"Got response of %x from a OBEX_Connect",*finger);
	      rdabt_clientp->ClientStatus = GOEP_Client_Idle;
    	      OBEX_TransportDisconnect(rdabt_clientp->goep_tidClient);
	      MGR_GetACLState(rdabt_clientp->ClientAddress);	 	  
            break;
        }
    }

    param_ptr->ref_count = 1;
    param_ptr->msg_len = 0x20;
    if (rdabt_clientp->isFtpClient == 0)
    {
    	param_ptr->uuid[0] = 0;
    	param_ptr->uuid_len = 0;
	param_ptr->cm_conn_id = 0;	
    }
    else
    {
	memcpy(param_ptr->uuid,OBEX_FTP_UUID,16);
    	param_ptr->uuid_len = 16;
	param_ptr->cm_conn_id = 1;	
    }		
    param_ptr->req_id = rdabt_clientp->req_idClient;
    param_ptr->goep_conn_id = rdabt_clientp->goep_tidClient ;//0x2;


    rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_CONNECT_RSP, (void *)param_ptr, NULL);
	
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_connect_req_reply
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_TransportConnect_RF_reply(u_int32 tid)
{
    //	u_int32 tid;
    t_DataBuf *buff;
    kal_uint8 finger = 1;
   
    //rdabt_clientp->goep_tidClient =    tid;
    kal_prompt_trace(0, "rdabt_goep_TransportConnect_RF_reply...  rdabt_clientp->isFtpClient = %d, tid = %d rdabt_clientp->ClientStatus=%d\n",rdabt_clientp->isFtpClient,tid,rdabt_clientp->ClientStatus);	
    if(rdabt_clientp->ClientStatus == GOEP_Client_Disconnect )
	  return;		
	
    if(oppc_timer)
    {
	  pTimerCancel(oppc_timer);
	  oppc_timer = 0;
    }

    if(tid == 0xffff)
    {
        OBEX_Connect_callback(0,&finger,0);
        return;
    }


    if(!MGR_SetACLCallBack(rdabt_clientp->ClientAddress,0,rdabt_goep_callback_ReleaseACL))
	    kal_prompt_trace(0, "MGR_SetACLCallBack!@!!!!!!");	
    else
	    kal_prompt_trace(0, "MGR_SetACLCallBack fail");	

    if(GOEP_Client_Idle == rdabt_clientp->ClientStatus || GOEP_Client_pending_Disconnect == rdabt_clientp->ClientStatus)
    {
	rdabt_clientp->ClientStatus = GOEP_Client_Disconnect;		
    	OBEX_TransportDisconnect(rdabt_clientp->goep_tidClient);
	return;
    }
	
    rdabt_clientp->ClientStatus = GOEP_Client_Disconnect;		

    OBEX_GetWriteBuffer(100, &buff);
    if (rdabt_clientp->isFtpClient == 0)
    {
	    if(OBEX_Connect(rdabt_clientp->goep_tidClient,0x10,0x00,0x3800,0,0x00,0x00,(t_DataBuf*)buff,OBEX_Connect_callback))
		{
			OBEX_Connect_callback(0,&finger,0);
			OBEX_FreeWriteBuffer(buff);	
			return;
		}
	}
	else
	{
	    	if(OBEX_Connect(rdabt_clientp->goep_tidClient,0x10,0x00,0x3800,16,(u_int8 *)OBEX_FTP_UUID,0x00,(t_DataBuf*)buff,OBEX_Connect_callback))
		{
			OBEX_Connect_callback(0,&finger,0);
			OBEX_FreeWriteBuffer(buff);
		}    
	}

	/*
	I might have to keep track of lenghts here.
	*/
}


/*************************************************************************************************
* FUNCTION :- rdabt_goep_sdp_timeout
*
* DESCRIPTION :- This is the callback for release ACL that has brought up for bonding
*                 
************************************************************************************************/
void rdabt_goep_acl_timeout(void)
{
	oppc_timer = 0;
	if(MGR_GetACLState(rdabt_clientp->ClientAddress) == 2)//CONN_CLOSED
	{
	       kal_prompt_trace(0, "rdabt_goep_acl_timeout1\n");	
		rdabt_goep_callback_ReleaseACL(NULL,0);
	}
	else
	{
		       kal_prompt_trace(0, "rdabt_goep_acl_timeout2\n");	
		       if(OBEX_L2_ACL_Expired(&(rdabt_clientp->ClientAddress)))
		       kal_prompt_trace(0, "OBEX_L2_ACL_Expired fail!\n");	
	}
	return;
}


/*************************************************************************************************
* FUNCTION :- rdabt_goep_sdp_timeout
*
* DESCRIPTION :- This is the callback for release ACL that has brought up for bonding
*                 
************************************************************************************************/
void rdabt_goep_sdp_timeout(void)
{
	kal_uint8 finger = 0;
	oppc_timer = 0;

	if(MGR_GetACLState(rdabt_clientp->ClientAddress) == 2)//CONN_CLOSED
	{
	       kal_prompt_trace(0, "rdabt_goep_sdp_timeout1\n");	
		 if(GOEP_Client_Idle == rdabt_clientp->ClientStatus || GOEP_Client_pending_Disconnect == rdabt_clientp->ClientStatus)
		{
			rdabt_goep_disconnect_rsp_oppc_callback();
		}
		else
		{
			OBEX_Connect_callback(0,&finger,0);
		}
	}
	else
	{
		       kal_prompt_trace(0, "rdabt_goep_sdp_timeout2\n");	
		       if(OBEX_L2_ACL_Expired(&(rdabt_clientp->ClientAddress)))
		    	{
			       kal_prompt_trace(0, "OBEX_L2_ACL_Expired fail!\n");			    	
		    		prh_l2_acl_shutdown_all(0);
				 if(rdabt_clientp->ClientStatus == GOEP_Client_Idle || GOEP_Client_pending_Disconnect == rdabt_clientp->ClientStatus)
				{
					rdabt_goep_disconnect_rsp_oppc_callback();
				}
				else
				{
					OBEX_Connect_callback(0,&finger,0);
				}
		    	}
	}
	return;
}

/*************************************************************************************************
* FUNCTION :- rdabt_goep_callback_timeout
*
* DESCRIPTION :- This is the callback for release ACL that has brought up for bonding
*                 
************************************************************************************************/
void rdabt_goep_callback_timeout(void)
{
	kal_uint8 finger = 0;
	oppc_timer = 0;
    	kal_prompt_trace(0, "rdabt_goep_callback_timeout TY_rfcomm , ClientStatus = %x",rdabt_clientp->ClientStatus);	
	if(	rdabt_clientp->ClientStatus == GOEP_Client_wait)
	{
		if(MGR_GetACLState(rdabt_clientp->ClientAddress) != 2)//CONN_CLOSED
			OBEX_Connect_callback(0,&finger,0);
		else
		{
			rdabt_clientp->ClientStatus = GOEP_Client_pending_sdp;
			if (rdabt_clientp->isFtpClient == 0)
				OBEX_FindObjectPushServer(&(rdabt_clientp->ClientAddress),NULL,NULL,rdabt_goep_connect_req_reply);
			else
				OBEX_FindFTPServer(&(rdabt_clientp->ClientAddress),NULL,NULL,rdabt_goep_connect_req_reply);
		}
	}
	else if(GOEP_Client_pending_rfcomm == rdabt_clientp->ClientStatus || GOEP_Client_Idle == rdabt_clientp->ClientStatus  || GOEP_Client_pending_Disconnect == rdabt_clientp->ClientStatus)
	{
		if(MGR_GetACLState(rdabt_clientp->ClientAddress) != 2)//CONN_CLOSED
		{
			if(!MGR_SetACLCallBack(rdabt_clientp->ClientAddress,0,rdabt_goep_callback_ReleaseACL))
				kal_prompt_trace(0, "MGR_SetACLCallBack!@!!!!!!");	
			else
				kal_prompt_trace(0, "MGR_SetACLCallBack fail");	
			if(rdabt_clientp->ClientStatus == GOEP_Client_Idle || GOEP_Client_pending_Disconnect == rdabt_clientp->ClientStatus)
			{
				OBEX_TransportDisconnect(goep_rfcomm_tidOPPC);
			}
			else
			{
				OBEX_TransportDisconnect(rdabt_clientp->goep_tidClient);
			}
			Timer_REOUT = 0;
		    	oppc_timer = pTimerCreate(30,(rdabt_timer_func)rdabt_goep_sdp_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend		
		}
		else
		{
			if(rdabt_clientp->ClientStatus == GOEP_Client_Idle || GOEP_Client_pending_Disconnect == rdabt_clientp->ClientStatus || GOEP_Client_pending_Disconnect == rdabt_clientp->ClientStatus)
			{
				rdabt_goep_disconnect_rsp_oppc_callback();
			}
			else
			{
				OBEX_Connect_callback(0,&finger,0);
			}
		}
	}
	else if(rdabt_clientp->ClientStatus == GOEP_Client_pending_sdp)
	{
		if(MGR_GetACLState(rdabt_clientp->ClientAddress) != 2)//CONN_CLOSED
		{
			if(!MGR_SetACLCallBack(rdabt_clientp->ClientAddress,0,rdabt_goep_callback_ReleaseACL))
				kal_prompt_trace(0, "MGR_SetACLCallBack!@!!!!!!");	
			else
				kal_prompt_trace(0, "MGR_SetACLCallBack fail");	
			Timer_REOUT = 0;
			rdabt_goep_sdp_timeout();
		}
		else
		{
			OBEX_Connect_callback(0,&finger,0);
		}
	}
	return;
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_connect_req_reply
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_connect_req_reply(t_bdaddr address,u_int8 channel,u_int8* formatc)
{
	kal_uint8 finger = 0;
	if(oppc_timer)
	{
		pTimerCancel(oppc_timer);
		oppc_timer = 0;
	}

	if((0 !=channel) && (rdabt_clientp->ClientStatus == GOEP_Client_pending_sdp))
	{
		rdabt_clientp->goep_cidClient = channel; 
    		kal_prompt_trace(0, "rdabt_goep_connect_req_reply rdabt_clientp->goep_cidClient = %d\n",rdabt_clientp->goep_cidClient);	
              if(OBEX_TransportConnect_RF(address, rdabt_clientp->goep_cidClient, &rdabt_clientp->goep_tidClient,0x3800,rdabt_goep_TransportConnect_RF_reply))
			OBEX_Connect_callback(0,&finger,0);
		else
		{
			rdabt_clientp->ClientStatus = GOEP_Client_pending_rfcomm;
		    	oppc_timer = pTimerCreate(150,(rdabt_timer_func)rdabt_goep_callback_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend		
		}
	/*
	I might have to keep track of lenghts here.
	*/
	}	
	else if((0 ==channel) && (formatc))
	{
		kal_prompt_trace(0, "rdabt_goep_connect_req_reply (0 ==channel) && (formatc)\n");	
		Timer_REOUT = 0;
		oppc_timer = pTimerCreate(30,(rdabt_timer_func)rdabt_goep_sdp_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend
		return;
	}
	else if(rdabt_clientp->ClientStatus == GOEP_Client_Idle || GOEP_Client_pending_Disconnect == rdabt_clientp->ClientStatus)
	{
		rdabt_goep_disconnect_rsp_oppc_callback();
		return;
	}
	else
	{
		OBEX_Connect_callback(0,&finger,0);
	}
}


/*************************************************************************************************
* FUNCTION :- rdabt_goep_callback_ReleaseACL
*
* DESCRIPTION :- This is the callback for release ACL that has brought up for anytime
*                 
************************************************************************************************/
void rdabt_goep_callback_ReleaseACL(void *dbEntry, int status)
{/*
	if (BD_ADDR_CMP(rdabt_ftpsp->FTPSaddress, dbEntry->bdAddress))
	{
		rdabt_goep_disconnect_rsp_ftps_callback();
	}
	else if (BD_ADDR_CMP(rdabt_clientp->ClientAddress, dbEntry->bdAddress))
	{
	*/
		kal_prompt_trace(0, "rdabt_goep_callback_ReleaseACL... rdabt_clientp->ClientStatus  %d", rdabt_clientp->ClientStatus);
		kal_uint8 finger = 0;
		if(oppc_timer)
		{
			pTimerCancel(oppc_timer);
			oppc_timer = 0;
		}
		kal_prompt_trace(0, "rdabt_goep_callback_ReleaseACL rdabt_clientp->ClientStatus = %d", rdabt_clientp->ClientStatus);
		if(	rdabt_clientp->ClientStatus == GOEP_Client_wait)
		{
			rdabt_clientp->ClientStatus = GOEP_Client_pending_sdp;
			if (rdabt_clientp->isFtpClient == 0)
			{
				status = OBEX_FindObjectPushServer(&(rdabt_clientp->ClientAddress),NULL,NULL,rdabt_goep_connect_req_reply);
				{
					kal_prompt_trace(0, "rdabt_goep_callback_ReleaseACL...OBEX_FindObjectPushServer returned error code %d", status);
					if(status !=1 )
						OBEX_Connect_callback(0,&finger,0);
					else
					{
						oppc_timer = pTimerCreate(60,(rdabt_timer_func)rdabt_goep_callback_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend
					}
					return;
				}

			}
			else
				OBEX_FindFTPServer(&(rdabt_clientp->ClientAddress),NULL,NULL,rdabt_goep_connect_req_reply);
		}
		else if (rdabt_clientp->ClientStatus == GOEP_Client_Connect || 
			     rdabt_clientp->ClientStatus == GOEP_Client_Disconnect  ||
			     rdabt_clientp->ClientStatus == GOEP_Client_Idle || 
			     rdabt_clientp->ClientStatus == GOEP_Client_pending_Disconnect )
		{
			rdabt_goep_disconnect_rsp_oppc_callback();
		}
		else if (rdabt_clientp->ClientStatus == GOEP_Client_pending_sdp|| rdabt_clientp->ClientStatus == GOEP_Client_pending_rfcomm)
		{
			OBEX_Connect_callback(0,&finger,0);
		}
/*	}
	else
	{
		asm("break 1");
	}
	*/
	return;
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_connect_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_connect_req(t_bdaddr address,goep_connect_req_struct *msg_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int status;
    kal_uint8 finger = 0;
    bt_bm_link_state_ind_struct *param_ptr ;
    param_ptr = (bt_bm_link_state_ind_struct*) construct_local_para(sizeof(bt_bm_link_state_ind_struct), TD_CTRL);
	kal_prompt_trace(0, "rdabt_goep_connect_req...  20090220-Rdabt_opp!@!!!!!!");	
    param_ptr->max_number = 3;
    param_ptr->current_number = 1 ;//0x2;
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_STATE_IND, (void *)param_ptr, NULL);
    kal_prompt_trace(0, "rdabt_goep_connect_req...  msg_p->uuid_len=%d, rdabt_clientp->ClientStatus = %d \n",msg_p->uuid_len,rdabt_clientp->ClientStatus);	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   if (msg_p->uuid_len==0)
   {
   	rdabt_clientp->isFtpClient = 0;
   }
   else
   {
   	rdabt_clientp->isFtpClient = 1;
       rdabt_clientp->buf_ptrFTC = msg_p->buf_ptr;
	rdabt_clientp->buf_sizeFTC = msg_p->buf_size;
   }

	rdabt_clientp->ClientAddress.bytes[0] = address.bytes[0];
	rdabt_clientp->ClientAddress.bytes[1] = address.bytes[1];
	rdabt_clientp->ClientAddress.bytes[2] = address.bytes[2];
	rdabt_clientp->ClientAddress.bytes[3] = address.bytes[3];
	rdabt_clientp->ClientAddress.bytes[4] = address.bytes[4];
	rdabt_clientp->ClientAddress.bytes[5] = address.bytes[5];

	rdabt_clientp->req_idClient = msg_p ->req_id;
  
	if(MGR_GetACLState(address) != 2)//CONN_CLOSED
	{
		if ( (rdabt_oppsp->OPPSstatus == GOEP_Client_Connect) || (rdabt_ftpsp->FTPSstatus == GOEP_Client_Connect))
		{
			OBEX_Connect_callback(0,&finger,0);
			return;
		}
		rdabt_clientp->ClientStatus = GOEP_Client_wait;	
		if(oppc_timer)
			pTimerCancel(oppc_timer);
	       oppc_timer = pTimerCreate(100,(rdabt_timer_func)rdabt_goep_callback_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend
		MGR_SetACLCallBack(address,0,rdabt_goep_callback_ReleaseACL);
	}
	else
	{
		rdabt_clientp->ClientStatus = GOEP_Client_pending_sdp;
		if (rdabt_clientp->isFtpClient == 0)
		{
			status = OBEX_FindObjectPushServer(&address,NULL,NULL,rdabt_goep_connect_req_reply);
			{
				kal_prompt_trace(0, "rdabt_goep_connect_req...OBEX_FindObjectPushServer returned error code %d", status);
				if(status !=1 )
					OBEX_Connect_callback(0,&finger,0);
				else
				{
					if(oppc_timer)
						pTimerCancel(oppc_timer);
					oppc_timer = pTimerCreate(306,(rdabt_timer_func)rdabt_goep_callback_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend
				}
				return;
			}
		}
		else
		{
			status = OBEX_FindFTPServer(&address,NULL,NULL,rdabt_goep_connect_req_reply);
			{
				kal_prompt_trace(0, "rdabt_goep_connect_req...OBEX_FindFTPServer returned error code %d", status);
				return;
			}
		}
	}
    return;
}


/*****************************************************************************
 * FUNCTION
 *  goep_push_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void goep_push_callback(u_int8 messageType, u_int8 *data, u_int16 len)
{
    goep_push_rsp_struct *param_ptr ;
    param_ptr = (goep_push_rsp_struct*) construct_local_para(sizeof(goep_push_rsp_struct), TD_CTRL);
    kal_prompt_trace(0, "goep_push_callback...  20090220-Rdabt_opp!@!!!!!! messageType = %x",messageType);	
    kal_prompt_trace(0, "!@!!!!!! len = %d",len);	
    param_ptr->goep_conn_id = rdabt_clientp->goep_tidClient;
    if (rdabt_clientp->isFtpClient ==0)
    {
    	param_ptr->uuid[0] = 0;
    	param_ptr->uuid_len = 0;
    }
    else
    {
    	memcpy(param_ptr->uuid,OBEX_FTP_UUID,16);
    	param_ptr->uuid_len = 16;    
    }
	
    if((OBEX_OK == messageType)||(OBEX_CONTINUE == messageType))
    {
        param_ptr->rsp_code =0;
        param_ptr->reason =0;		
    }
    else
    if((OBEX_DISCONNECT == messageType))
    {

        kal_prompt_trace(0, "goep_push_callback...  !@!!!!!! messageType = %d",messageType);	
        param_ptr->rsp_code =1;
        param_ptr->reason =messageType;	
    }
    else //OBEX_FORBIDDEN	
    {

        kal_prompt_trace(0, "goep_push_callback...  !@!!!!!! messageType = %d",messageType);	
        param_ptr->rsp_code =1;
        param_ptr->reason =messageType;	
    }
        rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PUSH_RSP, (void *)param_ptr, NULL);
}

void rdabt_FolderListing_callback(u_int8 status, u_int8* data, u_int16 length)
{
        kal_prompt_trace(0, "rdabt_FolderListing_callback...  !@!!!!!! status = %d,length = %d\n",status,length);	
}

void goep_pull_callback(u_int8 status, u_int8 *finger, u_int16 length)
{	
	u_int8* result_data = 0x00;
	u_int16 result_length;
	goep_pull_rsp_struct *param_ptr = (goep_pull_rsp_struct*) construct_local_para(sizeof(goep_pull_rsp_struct), TD_CTRL);;
	
	u_int16 offset_len;
	t_obex_header_offsets headers;
	u_int8 retValue=1;
	u_int32 fileRxLen=0;
       kal_prompt_trace(0, "goep_pull_callback... rdabt_clientp->goep_tidClient =%d !@!!!!!! status = %d,finger= 0x%x,length = %d\n",rdabt_clientp->goep_tidClient,status,finger,length);	

	param_ptr->goep_conn_id = rdabt_clientp->goep_tidClient;
	if (rdabt_clientp->isFtpClient ==0)
	{
		param_ptr->uuid[0] = 0;
		param_ptr->uuid_len = 0;
	}
	else
	{
		memcpy(param_ptr->uuid,OBEX_FTP_UUID,16);
		param_ptr->uuid_len = 16;    
	}

	result_data = rdabt_clientp->buf_ptrFTC;
	result_length = 0;
	offset_len = OBEX_HEADER;
	if ( status == OBEX_DISCONNECT)
	{
	        param_ptr->rsp_code =GOEP_STATUS_DISCONNECT;
	        param_ptr->reason =status;		
	        rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PULL_RSP, (void *)param_ptr, NULL);
		 //rdabt_clientp->ClientStatus = GOEP_Client_Idle;
	 	//rdabt_goep_disconnect_rsp_oppc(0, NULL, 0);
		return;
	}
	retValue=OBEX_ExtractHeaders(&headers, &offset_len,finger,length);

	/* check headers were extracted ok*/
	if(retValue)
	{
	        param_ptr->rsp_code =1;
	        param_ptr->reason =status;		
	        rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PULL_RSP, (void *)param_ptr, NULL);
		 rdabt_clientp->ClientStatus = GOEP_Client_Idle;
		return;
	}
       kal_prompt_trace(0, "goep_pull_callback...  headers.len = %d,headers.body= %d,headers.eob = %d\n",headers.len,headers.body,headers.eob);	
	switch(status)
	{
	/*
	Success 
	*/
	case OBEX_OK:
		if(headers.len)
		{
			fileRxLen= ((finger+headers.len)[0] << 24) + ((finger+headers.len)[1] << 16) + ((finger+headers.len)[2] << 8) + ((finger+headers.len)[3]);
		}
		kal_prompt_trace(0, "OBEX_OK...  !@!!!!!! fileRxLen = %d",fileRxLen);	
		if(headers.body)
		{
			u_int16 temp;
			/*
			shift 1st byte as it's the msb, add second byte to give the len. 
			-OBEX_HEADER for the header id and length fields
			*/
			temp = ((finger + headers.body)[0] << 8) + (finger + headers.body)[1] -OBEX_HEADER;
			pMemcpy(result_data + result_length, finger + headers.body + 2,temp);
			result_length += temp;
		}

		if(headers.eob)
		{
			u_int16 temp;
			/*
			shift 1st byte as it's the msb, add second byte to give the len. 
			-OBEX_HEADER for the header id and length fields
			*/
			temp = ((finger + headers.eob)[0] << 8) + (finger + headers.eob)[1] -OBEX_HEADER;
			pMemcpy(result_data + result_length, finger + headers.eob + 2,temp);
			result_length += temp;
		}
		/*
		Call the user registered callback function - OBEX_OK indicates it's finished
		so no need to put the below code in the "if(headers.eob)" path.
		*/	
		//prh_user_obex_ftp_genpurpose_callback(OBEX_OK,result_data,result_length);
	        param_ptr->rsp_code =0;
	        param_ptr->reason =0;		
		 param_ptr->frag_len = result_length;
		 param_ptr->frag_ptr = result_data;
		 param_ptr->total_obj_len = fileRxLen;
		 param_ptr->to_self = 0;
		 kal_prompt_trace(0, "frag_len = %d frag_ptr=0x%x total_obj_len = %d...  !@!!!!!! ",param_ptr->frag_len,param_ptr->frag_ptr,fileRxLen);	
		 if (rdabt_clientp->FTCfirst)
		 	 param_ptr->pkt_type = GOEP_SINGLE_PKT;
		 else
		 	param_ptr->pkt_type = GOEP_FINAL_PKT;
		 rdabt_clientp->FTCfirst = 1;
	        rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PULL_RSP, (void *)param_ptr, NULL);
		break;
	case OBEX_CONTINUE:
		if(headers.len)
		{
			fileRxLen= ((finger+headers.len)[0] << 24) + ((finger+headers.len)[1] << 16) + ((finger+headers.len)[2] << 8) + ((finger+headers.len)[3]);
		}
		kal_prompt_trace(0, "OBEX_CONTINUE...  !@!!!!!! fileRxLen = %d",fileRxLen);	
		/*
		Means I have to send another Get with no headers. but there is can still be a body here
		so I must take the data if it exists.
		*/
		if(headers.body)
		{
			/*
			shift 1st byte as it's the msb, add second byte to give the len. 
			-3 for the header id and length fields
			*/
			u_int16 temp;
			temp = ((finger + headers.body)[0] << 8) + (finger + headers.body)[1] -OBEX_HEADER;
			pMemcpy(result_data + result_length, finger + headers.body + 2,temp);
			result_length += temp;
		}
	        param_ptr->rsp_code =0;
	        param_ptr->reason =0;		
		 param_ptr->frag_len = result_length;
		 param_ptr->frag_ptr = result_data;
		 param_ptr->total_obj_len = fileRxLen;
		 param_ptr->to_self = 0;
		 kal_prompt_trace(0, "frag_len = %d frag_ptr=0x%x total_obj_len = %d...  !@!!!!!! ",param_ptr->frag_len,param_ptr->frag_ptr,fileRxLen);	
		 if (rdabt_clientp->FTCfirst)
		 	param_ptr->pkt_type = GOEP_FIRST_PKT;
		 else
		 	param_ptr->pkt_type = GOEP_NORMAL_PKT;
		 rdabt_clientp->FTCfirst = 0;
	        rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PULL_RSP, (void *)param_ptr, NULL);

		break;
	default:
		/* error case */
		kal_prompt_trace(0, "goep_pull_callback...  default case rdabt_clientp->ClientStatus = %d",rdabt_clientp->ClientStatus);	
	        param_ptr->rsp_code =1;
	        param_ptr->reason =status;	
		 param_ptr->to_self = 0;	
	        rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PULL_RSP, (void *)param_ptr, NULL);
		// rdabt_clientp->ClientStatus = GOEP_Client_Idle;
		break;
	}
	return;	
}

void goep_set_folder_callback(u_int8 status, u_int8* finger, u_int16 length)
{	
	u_int8 retValue;
	u_int16 offset_len;
	t_obex_header_offsets headers;
	goep_set_folder_rsp_struct *param_ptr = (goep_set_folder_rsp_struct*) construct_local_para(sizeof(goep_set_folder_rsp_struct), TD_CTRL);;
	memcpy(param_ptr->uuid,OBEX_FTP_UUID,16);
	param_ptr->uuid_len = 16;    
	param_ptr->goep_conn_id = rdabt_clientp->goep_tidClient;
	
	offset_len=OBEX_HEADER;
	if ( status == OBEX_DISCONNECT)
	{
	        param_ptr->rsp_code =GOEP_STATUS_DISCONNECT;
	        param_ptr->reason =status;		
	        rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_SET_FOLDER_RSP, (void *)param_ptr, NULL);
		 //rdabt_clientp->ClientStatus = GOEP_Client_Idle;
	 	//rdabt_goep_disconnect_rsp_oppc(0, NULL, 0);
		return;
	}
	retValue=OBEX_ExtractHeaders(&headers, &offset_len,finger,length);

	/* check headers were extracted ok*/
	if(retValue)
	{
	        param_ptr->rsp_code =1;
	        param_ptr->reason =*finger;		
	        rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_SET_FOLDER_RSP, (void *)param_ptr, NULL);
		return;
	}

	/* I will take this out later as it increases code size */

	switch(*finger)
	{
		/*
		Success - callback with the cid to the user
		*/
		case OBEX_OK:

			/*
			Must be from a setpath call
			*/
		        param_ptr->rsp_code =0;
		        param_ptr->reason =0;		
		        rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_SET_FOLDER_RSP, (void *)param_ptr, NULL);

			break;

			/*
			not successful - return status and data to user
			*/
		default:

		        param_ptr->rsp_code =1;
		        param_ptr->reason =*finger;		
		        rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_SET_FOLDER_RSP, (void *)param_ptr, NULL);

			break;
	}

	return;
}


void rdabt_goep_set_folder_req(goep_set_folder_req_struct *msg_p)
{
	u_int8 status;
	u_int16 t_offset;
	t_DataBuf* buff;
	u_int8 setPathFlags=0;
	u_int16 NameLength = (u_int16) UCS2Strlen(msg_p ->folder_name);

			/*
	set the offset
	*/
	t_offset = OBEX_SETPATH_HEADER;

	status=OBEX_GetWriteBuffer(300,&buff); /* I will need to trim this down a bit*/
	if(status)
		return ;
	if (msg_p->setpath_flag == GOEP_FORWARD_FOLDER)
	{
		status=OBEX_Create4ByteHeader(OBEX_CID,rdabt_clientp->goep_cidClient,buff,&t_offset);
		if(status)
			return ;
		status=OBEX_CreateStringHeader(OBEX_NAME,NameLength,(uint16 *)msg_p->folder_name,buff,&t_offset);
		setPathFlags = 0X02;
	}
	else if (msg_p->setpath_flag == GOEP_BACK_FOLDER)
	{
		status=OBEX_Create4ByteHeader(OBEX_CID,rdabt_clientp->goep_cidClient,buff,&t_offset);
		if(status)
			return ;

		setPathFlags = 0X03;
	}
	else if (msg_p->setpath_flag == GOEP_ROOT_FOLDER)
	{
		status=OBEX_Create4ByteHeader(OBEX_CID,rdabt_clientp->goep_cidClient,buff,&t_offset);
		if(status)
			return ;
		status=OBEX_CreateStringHeader(OBEX_NAME,0,0,buff,&t_offset);
		setPathFlags = 0X02;
	}
	/*
	Call OBEX_SetPath();
	*/
	status=OBEX_SetPath(rdabt_clientp->goep_tidClient,setPathFlags,0,t_offset,buff,goep_set_folder_callback);

	return;
}
void rdabt_goep_pull_req(goep_pull_req_struct *msg_p)
{
/*
	if (!strcmp(msg_p->obj_mime_type,"x-obex/folder-listing"))
		OBEX_GetFolderListingObject(rdabt_clientp->goep_tidClient,rdabt_clientp->goep_cidClient,rdabt_FolderListing_callback);
	*/
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	t_DataBuf *buff;
	u_int16 Offset;
    u_int16 NameLength = (u_int16) UCS2Strlen(msg_p ->obj_name);
    u_int32 TypeLength = (u_int32)strlen((char* )msg_p ->obj_mime_type)+1;
	

    kal_prompt_trace(0, "rdabt_goep_pull_req...  20090220-Rdabt_opp!@!!!!!!,TypeLength =%d rdabt_clientp->goep_cidClient=%d",TypeLength,rdabt_clientp->goep_cidClient);	

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	switch( msg_p ->pkt_type )
        {
            case GOEP_FIRST_PKT:
	     	     rdabt_clientp->FTCfirst = 1;		
	            Offset = 3;		
	            kal_prompt_trace(0, "GOEP_FIRST_PKT,NameLength =%d",NameLength);	
	            kal_prompt_trace(0, "GOEP_FIRST_PKT,TypeLength =%d",TypeLength);	
		     OBEX_GetWriteBuffer(NameLength*2+TypeLength+50,&buff); /*need to reallocate as the transport free the */
		     OBEX_Create4ByteHeader(OBEX_CID,rdabt_clientp->goep_cidClient,buff,&Offset);
		     OBEX_CreateStringHeader(OBEX_NAME,NameLength,(uint16 *)msg_p ->obj_name,buff,&Offset);	
		     OBEX_CreateSequenceHeader(OBEX_TYPE,TypeLength==1?0:TypeLength,msg_p ->obj_mime_type,buff,&Offset);
		     OBEX_Get(msg_p ->goep_conn_id,1,Offset,(t_DataBuf*)buff,goep_pull_callback);
            break;
                
            case GOEP_NORMAL_PKT:
		  kal_prompt_trace(0, "GOEP_NORMAL_PKT");
		   if (TypeLength > 1)
		   {
                	OBEX_GetWriteBuffer(0,&buff);
		   }
		   else
		   {
                	OBEX_GetWriteBuffer(20,&buff);
		   }
		   OBEX_Get(msg_p ->goep_conn_id,1,0,buff,goep_pull_callback);
            	break;
                
            case GOEP_FINAL_PKT:
            case GOEP_SINGLE_PKT:
		  kal_prompt_trace(0, "GOEP_FINAL_PKT msg_p ->pkt_type=%d\n",msg_p ->pkt_type);	
/*
		  Offset = 3;
                max_len = msg_p ->frag_len;
                OBEX_GetWriteBuffer(max_len+50,&buff);
                OBEX_CreateSequenceHeader(OBEX_END_BODY,max_len,msg_p ->frag_ptr,buff,&Offset);
                OBEX_Get(msg_p ->goep_conn_id,1,Offset,buff,goep_pull_callback);
*/
            	break;
                
            default:
               {
		  	kal_prompt_trace(0, "default msg_p ->pkt_type=%d\n",msg_p ->pkt_type);	
                  break;
               }
        }			
    return;
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_push_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_push_req(goep_push_req_struct *msg_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	t_DataBuf *buff;
	u_int16 Offset;
	u_int16 max_len;
    u_int16 NameLength = (u_int16) UCS2Strlen(msg_p ->obj_name);
    u_int32 TypeLength = (u_int32)strlen((char* )msg_p ->obj_mime_type)+1;
	u_int16 *unicode_str =NULL;
	

		kal_prompt_trace(0, "rdabt_goep_push_req...  20090220-Rdabt_opp!@!!!!!!,TypeLength =%d",TypeLength);	

	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	switch( msg_p ->pkt_type )
        {
            case GOEP_FIRST_PKT:
            Offset = 3;		
            kal_prompt_trace(0, "GOEP_FIRST_PKT,NameLength =%d",NameLength);	
            kal_prompt_trace(0, "OBEX_GetWriteBuffer,NameLength =%d",NameLength);	

            OBEX_GetWriteBuffer(NameLength*2+TypeLength+50,&buff); /*need to reallocate as the transport free the */
            OBEX_CreateStringHeader(OBEX_NAME,NameLength,(u_int16*)msg_p ->obj_name,buff,&Offset);
            OBEX_CreateSequenceHeader(OBEX_TYPE,TypeLength,msg_p ->obj_mime_type,buff,&Offset);
            OBEX_Create4ByteHeader(OBEX_LENGTH,msg_p ->total_obj_len,buff,&Offset);
            if(NULL != unicode_str)
            {
                pFree(unicode_str);
                unicode_str = NULL;
            }
            OBEX_Put(msg_p ->goep_conn_id,0,Offset,(t_DataBuf*)buff,goep_push_callback);
            break;
                
            case GOEP_NORMAL_PKT:
		  kal_prompt_trace(0, "GOEP_NORMAL_PKT");	
                Offset = 3;
                max_len = msg_p ->frag_len;
                OBEX_GetWriteBuffer(max_len+50,&buff);
                //size = f.Read(t_buff,max_len);
                OBEX_CreateSequenceHeader(OBEX_BODY,max_len,msg_p ->frag_ptr,buff,&Offset);
                //					OBEX_CreateSequenceHeader(OBEX_BODY,max_len,t_buff,buff,&Offset);
                OBEX_Put(msg_p ->goep_conn_id,0,Offset,buff,goep_push_callback);
            	break;
                
            case GOEP_FINAL_PKT:
            case GOEP_SINGLE_PKT:
		  kal_prompt_trace(0, "GOEP_FINAL_PKT");	
                Offset = 3;
                max_len = msg_p ->frag_len;
                OBEX_GetWriteBuffer(max_len+50,&buff);
                OBEX_CreateSequenceHeader(OBEX_END_BODY,max_len,msg_p ->frag_ptr,buff,&Offset);
                OBEX_Put(msg_p ->goep_conn_id,1,Offset,buff,goep_push_callback);
            	break;
                
            default:
               {

                  break;
               }
        }			
    return;
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_disconnect_req_reply
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_disconnect_req_reply(kal_uint32 req_id,  kal_uint8 goep_conn_id)
{

       goep_disconnect_ind_struct *param_ptr ;
       bt_bm_link_state_ind_struct *param_ptr_link_state ;

       param_ptr = (goep_disconnect_ind_struct*) construct_local_para(sizeof(goep_disconnect_ind_struct), TD_CTRL);
       param_ptr_link_state = (bt_bm_link_state_ind_struct*) construct_local_para(sizeof(bt_bm_link_state_ind_struct), TD_CTRL);

	//OBEX_TransportDisconnect(goep_conn_id);

    param_ptr->ref_count = 1;
    param_ptr->msg_len = 0x18;
    if (rdabt_clientp->isFtpClient ==0)
    {
    	param_ptr->uuid[0] = 0;
    	param_ptr->uuid_len = 0;
    }
    else
    {
    	memcpy(param_ptr->uuid,OBEX_FTP_UUID,16);
    	param_ptr->uuid_len = 16;    
    }
    param_ptr->req_id = req_id ;
    param_ptr->goep_conn_id = goep_conn_id;
	/*
	I might have to keep track of lenghts here.
	*/
    rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_DISCONNECT_IND, (void *)param_ptr, NULL);
    param_ptr_link_state->ref_count = 1;
    param_ptr_link_state->msg_len = 6 ;//0x2;
    param_ptr_link_state->max_number = 3;
    param_ptr_link_state->current_number = 0 ;//0x2;
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_STATE_IND, (void *)param_ptr_link_state, NULL);
    rdabt_clientp->goep_tidClient = 0xff;
    rdabt_clientp->ClientStatus = GOEP_Client_Idle;

}


void rdabt_goep_disconnect_rsp_oppc_callback(void)
{
    goep_disconnect_ind_struct *param_ptr ;
    bt_bm_link_state_ind_struct *param_ptr_link_state ;

    param_ptr = (goep_disconnect_ind_struct*) construct_local_para(sizeof(goep_disconnect_ind_struct), TD_CTRL);
    param_ptr_link_state = (bt_bm_link_state_ind_struct*) construct_local_para(sizeof(bt_bm_link_state_ind_struct), TD_CTRL);
    kal_prompt_trace(0, "rdabt_goep_disconnect_rsp_oppc_callback...  rdabt_clientp->ClientStatus = %d",rdabt_clientp->ClientStatus);
    kal_prompt_trace(0, "rdabt_clientp->goep_tidClient = %d,rdabt_clientp->goep_tidClient_mmi = %d",rdabt_clientp->goep_tidClient,rdabt_clientp->goep_tidClient_mmi);
    // if(rdabt_clientp->goep_tidClient != 0xff)

    param_ptr->ref_count = 1;
    param_ptr->msg_len = 0x18;
    if (rdabt_clientp->isFtpClient ==0)
    {
    	param_ptr->uuid[0] = 0;
    	param_ptr->uuid_len = 0;
    }
    else
    {
    	memcpy(param_ptr->uuid,OBEX_FTP_UUID,16);
    	param_ptr->uuid_len = 16;    
    }
    param_ptr->req_id = rdabt_clientp->req_idClient ;
   	if(rdabt_clientp->ClientStatus == GOEP_Client_Connect)
   	{
   		param_ptr->goep_conn_id = rdabt_clientp->goep_tidClient;
   	}
    else
    param_ptr->goep_conn_id = rdabt_clientp->goep_tidClient_mmi;
	/*
	I might have to keep track of lenghts here.
	*/
    rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_DISCONNECT_IND, (void *)param_ptr, NULL);
    param_ptr_link_state->ref_count = 1;
    param_ptr_link_state->msg_len = 6 ;//0x2;
    param_ptr_link_state->max_number = 3;
    param_ptr_link_state->current_number = 0 ;//0x2;
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_STATE_IND, (void *)param_ptr_link_state, NULL);
    rdabt_clientp->ClientStatus = GOEP_Client_Idle;
    rdabt_clientp->goep_tidClient = 0xff;
}

void rdabt_goep_disconnect_rsp_oppc_callback_sdp(void *dbEntry, int status)
{
	rdabt_goep_disconnect_rsp_oppc_callback();
}

void rdabt_goep_OBEX_Disconnect_callback(u_int8 status, u_int8* data, u_int16 length)
{
	kal_prompt_trace(0, "rdabt_goep_OBEX_Disconnect_callback rdabt_clientp->ClientStatus =%d\n ",rdabt_clientp->ClientStatus);	
}
/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_disconnect_rsp_oppc
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_disconnect_rsp_oppc(u_int8 status, u_int8* data, u_int16 length)
{
	kal_prompt_trace(0, "rdabt_goep_disconnect_rsp_oppc rdabt_clientp->ClientStatus =%d\n ",rdabt_clientp->ClientStatus);	
	if(GOEP_Client_Connect == rdabt_clientp->ClientStatus)
	{
		if (MGR_GetACLState(rdabt_clientp->ClientAddress) == 2)//CONN_CLOSED
		{
			rdabt_goep_disconnect_rsp_oppc_callback();
		}
		else
		{
		         u_int16 offset_len;
		         t_DataBuf *txBuffer;
		         offset_len = OBEX_HEADER;
		        OBEX_GetWriteBuffer(100, &txBuffer);
		        OBEX_Disconnect(rdabt_clientp->goep_tidClient , offset_len, txBuffer, rdabt_goep_OBEX_Disconnect_callback);                
			if(oppc_timer)
			{
				pTimerCancel(oppc_timer);
			}
			rdabt_clientp->ClientStatus = GOEP_Client_Disconnect;
		    	oppc_timer = pTimerCreate(20,(rdabt_timer_func)rdabt_goep_disconnect_rsp_oppc,NULL,pTIMER_ONESHOT);//60 is 6 secend
		}	
	}
	else if(GOEP_Client_Disconnect == rdabt_clientp->ClientStatus)
	{
		if (MGR_GetACLState(rdabt_clientp->ClientAddress) == 2)//CONN_CLOSED
		{
			rdabt_goep_disconnect_rsp_oppc_callback();
		}
		else
		{
		    	status=OBEX_TransportDisconnect(rdabt_clientp->goep_tidClient);
			if(oppc_timer)
			{
				pTimerCancel(oppc_timer);
			}
		    	oppc_timer = pTimerCreate(60,(rdabt_timer_func)rdabt_goep_acl_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend
		}
	}
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_disconnect_rsp_ftps
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_disconnect_rsp_ftps_callback(void *dbEntry, int status)
{
    kal_prompt_trace(0, "rdabt_goep_disconnect_rsp_ftps_callback!@!!!!!!");	
    goep_disconnect_ind_struct *param_ptr ;
    bt_bm_link_state_ind_struct *param_ptr_link_state ;
   if(ftps_timer)
   	{
	   	pTimerCancel(ftps_timer);
		ftps_timer = 0;
   	}

    param_ptr = (goep_disconnect_ind_struct*) construct_local_para(sizeof(goep_disconnect_ind_struct), TD_CTRL);
    param_ptr_link_state = (bt_bm_link_state_ind_struct*) construct_local_para(sizeof(bt_bm_link_state_ind_struct), TD_CTRL);

    param_ptr->ref_count = 1;
    param_ptr->msg_len = 0x18;
    memcpy((char*)param_ptr->uuid , (const char*)(OBEX_FTP_UUID) , 16);
    param_ptr->uuid_len = 0x16;
    param_ptr->req_id = rdabt_ftpsp->req_idFTPS ;
    param_ptr->goep_conn_id = rdabt_ftpsp->goep_tid_FTPS;

    rdabt_ftpsp->FTPSfirst = 1;
    rdabt_ftpsp->FTPSstatus = GOEP_Client_Disconnect;

	/*
	I might have to keep track of lenghts here.
	*/
    rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_DISCONNECT_IND, (void *)param_ptr, NULL);
    param_ptr_link_state->ref_count = 1;
    param_ptr_link_state->msg_len = 6 ;//0x2;
    param_ptr_link_state->max_number = 3;
    param_ptr_link_state->current_number = 0 ;//0x2;
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_STATE_IND, (void *)param_ptr_link_state, NULL);

}


/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_disconnect_rsp_ftps
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_disconnect_rsp_ftps_callback_full(void *dbEntry, int status)
{
    OBEX_ServerTransportDisconnect(rdabt_ftpsp->FTPStid);
    rdabt_goep_disconnect_rsp_ftps_callback(dbEntry,status);
}

extern u_int8 *prh_mgr_printBDAddress(t_bdaddr address) ;

void rdabt_goep_disconnect_rsp_ftps(u_int8 status, u_int8* data, u_int16 length)
{
	kal_prompt_trace(0, "rdabt_goep_disconnect_rsp_ftps rdabt_ftpsp->FTPStid =%d,rdabt_ftpsp->FTPSaddress =%s\n ",rdabt_ftpsp->FTPStid,prh_mgr_printBDAddress(rdabt_ftpsp->FTPSaddress));

       OBEX_ServerTransportDisconnect(rdabt_ftpsp->FTPStid);
	if (rdabt_ftpsp->FTPSstatus == GOEP_Client_Disconnect)
	{
           rdabt_goep_disconnect_rsp_ftps_callback(0,0);
	}
}


/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_disconnect_rsp_opps
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_disconnect_rsp_opps_callback(void *dbEntry, int status)
{
	if(opps_timer)
   	{
	   	pTimerCancel(opps_timer);
		opps_timer = 0;
   	}

	goep_disconnect_ind_struct *param_ptr ;
	bt_bm_link_state_ind_struct *param_ptr_link_state ;

	param_ptr = (goep_disconnect_ind_struct*) construct_local_para(sizeof(goep_disconnect_ind_struct), TD_CTRL);
	param_ptr_link_state = (bt_bm_link_state_ind_struct*) construct_local_para(sizeof(bt_bm_link_state_ind_struct), TD_CTRL);

	param_ptr->ref_count = 1;
	param_ptr->msg_len = 0x18;
	param_ptr->uuid[0] = 0;
	param_ptr->uuid_len = 0;
	param_ptr->req_id = rdabt_oppsp->req_idOPPS ;
	param_ptr->goep_conn_id = rdabt_oppsp->goep_tid_OPPS;

	rdabt_oppsp->OPPSfirst = 1;		
	rdabt_oppsp->OPPSstatus = GOEP_Client_Disconnect;

	/*
	I might have to keep track of lenghts here.
	*/
	rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_DISCONNECT_IND, (void *)param_ptr, NULL);
	param_ptr_link_state->ref_count = 1;
	param_ptr_link_state->msg_len = 6 ;//0x2;
	param_ptr_link_state->max_number = 3;
	param_ptr_link_state->current_number = 0 ;//0x2;
	rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_STATE_IND, (void *)param_ptr_link_state, NULL);

}



/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_disconnect_rsp_opps
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_disconnect_rsp_opps_callback_full(void *dbEntry, int status)
{
	OBEX_ServerTransportDisconnect(rdabt_oppsp->OPPStid);
	rdabt_goep_disconnect_rsp_opps_callback(dbEntry, status);

}

void rdabt_goep_disconnect_rsp_opps(u_int8 status, u_int8* data, u_int16 length)
{
	kal_prompt_trace(0, "rdabt_goep_disconnect_rsp_opps rdabt_oppsp->OPPStid =%d\n ",rdabt_oppsp->OPPStid);	
       OBEX_ServerTransportDisconnect(rdabt_oppsp->OPPStid);
	if (rdabt_oppsp->OPPSstatus == GOEP_Client_Disconnect)
	{
           rdabt_goep_disconnect_rsp_opps_callback(0,0);
	}
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_disconnect_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_disconnect_req(goep_disconnect_req_struct *msg_p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
 //   int status;
    u_int16 offset_len;
//    t_DataBuf *txBuffer;

    kal_prompt_trace(0, "rdabt_goep_disconnect_req...  msg_p ->goep_conn_id =%x, rdabt_clientp->goep_tidClient =%d rdabt_clientp->ClientStatus=%d",msg_p ->goep_conn_id, rdabt_clientp->goep_tidClient,rdabt_clientp->ClientStatus);	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if((msg_p ->goep_conn_id == rdabt_clientp->goep_tidClient)||(msg_p ->goep_conn_id == 0xff))
    {
    	offset_len = OBEX_HEADER;
    	rdabt_clientp->req_idClient = msg_p ->req_id ;
    	if(rdabt_clientp->ClientStatus >= GOEP_Client_Disconnect)
    	{
    		rdabt_clientp->goep_tidClient_mmi = msg_p ->goep_conn_id;
    	}
    	else 
    		rdabt_clientp->goep_tidClient_mmi = rdabt_clientp->goep_tidClient;
    	
    	if(msg_p ->goep_conn_id == 0xff && (rdabt_clientp->ClientStatus == GOEP_Client_Connect))
    	{
    		rdabt_clientp->ClientStatus = GOEP_Client_Disconnect;		
    	}	
    	kal_prompt_trace(0, "rdabt_goep_disconnect_req OPPC...  20090220-Rdabt_opp!@!!!!!!,oppCstatus==%x",rdabt_clientp->ClientStatus);	

		switch(rdabt_clientp->ClientStatus) 
		{
		case GOEP_Client_Connect:
		case GOEP_Client_Disconnect:
		       rdabt_goep_disconnect_rsp_oppc(0, NULL, 0);
			break;
		case GOEP_Client_pending_rfcomm:
		    	goep_rfcomm_tidOPPC = rdabt_clientp->goep_tidClient;
		case GOEP_Client_pending_sdp:
			if(oppc_timer)
			{
				pTimerCancel(oppc_timer);
			}
		    	oppc_timer = pTimerCreate(30,(rdabt_timer_func)rdabt_goep_callback_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend
	              rdabt_clientp->ClientStatus = GOEP_Client_pending_Disconnect;
			break;

		case GOEP_Client_wait:
			if(oppc_timer)
			{
				pTimerCancel(oppc_timer);
				oppc_timer = 0;
			}		
		case GOEP_Client_Idle:
	       	rdabt_goep_disconnect_req_reply(rdabt_clientp->req_idClient, rdabt_clientp->goep_tidClient_mmi);
			break;
		default:
			return ;
		}
    }
    else if(msg_p ->goep_conn_id == rdabt_ftpsp->goep_tid_FTPS)
    {
    	offset_len = OBEX_HEADER;
    	rdabt_ftpsp->req_idFTPS = msg_p ->req_id;
    	kal_prompt_trace(0, "rdabt_goep_disconnect_req FTPS...  20090220-Rdabt_opp!@!!!!!!");	
    	
           rdabt_goep_disconnect_rsp_ftps(0, NULL, 0);
    }
    else if(msg_p ->goep_conn_id == rdabt_oppsp->goep_tid_OPPS)
    {
    	offset_len = OBEX_HEADER;
    	rdabt_oppsp->req_idOPPS = msg_p ->req_id ;
    	kal_prompt_trace(0, "rdabt_goep_disconnect_req OPPS...  20090220-Rdabt_opp!@!!!!!!");	
    
           rdabt_goep_disconnect_rsp_opps(0, NULL, 0);
    }
    return;
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_link_disallow
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/

void rdabt_goep_link_disallow(void)
{
	//mmi_trace(1,"rdabt_goep_link_disallow rdabt_oppsp->OPPSstatus=%d,rdabt_ftpsp->FTPSstatus=%d ",rdabt_oppsp->OPPSstatus,rdabt_ftpsp->FTPSstatus);
	if(rdabt_oppsp->OPPSstatus == GOEP_Client_Connect)
	{
	       //OBEX_ServerTransportDisconnect(rdabt_oppsp->OPPStid);
	       rdabt_goep_disconnect_rsp_opps(0, NULL, 0);
	}
	
	if(rdabt_ftpsp->FTPSstatus == GOEP_Client_Connect)
	{
	       //OBEX_ServerTransportDisconnect(rdabt_ftpsp->FTPStid);
	       rdabt_goep_disconnect_rsp_ftps(0, NULL, 0);
	}
       COS_Sleep(20);
	
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_abort_req_reply
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/

void rdabt_goep_abort_req_reply(u_int8 status, u_int8* data, u_int16 length)
{
    goep_abort_rsp_struct *param_ptr ;
    param_ptr = (goep_abort_rsp_struct*) construct_local_para(sizeof(goep_abort_rsp_struct), TD_CTRL);
    kal_prompt_trace(0, "rdabt_goep_abort_req_reply...  20090220-Rdabt_opp!@!!!!!!,status = %d",status);
    kal_prompt_trace(0, "!@!!!!!!,length = %d",length);
    if(rdabt_clientp->ClientStatus != GOEP_Client_Connect)
		return;

    if (status == OBEX_DISCONNECT)
    {
    	//status=OBEX_TransportDisconnect(rdabt_clientp->goep_tidClient);
	//rdabt_goep_disconnect_rsp_oppc(0, NULL, 0);
	status = GOEP_STATUS_DISCONNECT;
	//return;
    }
    param_ptr ->ref_count = 1;
    param_ptr ->msg_len = 0x18;
    if (rdabt_clientp->isFtpClient ==0)
    {
    	param_ptr->uuid[0] = 0;
    	param_ptr->uuid_len = 0;
    	rdabt_clientp->ClientStatus = GOEP_Client_Disconnect;
    }
    else
    {
    	memcpy(param_ptr->uuid,OBEX_FTP_UUID,16);
    	param_ptr->uuid_len = 16;    
    }
    param_ptr ->goep_conn_id = rdabt_clientp->goep_tidClient;
    param_ptr ->rsp_code = status ;
    rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_ABORT_RSP, (void *)param_ptr, NULL);
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_abort_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_abort_req(goep_abort_req_struct *msg_p)
{
    int status;
    u_int16 offset_len;
    t_DataBuf *txBuffer;
    kal_prompt_trace(0, "rdabt_goep_abort_req...  20090220-Rdabt_opp!@!!!!!!rdabt_clientp->ClientStatus =%d msg_p ->goep_conn_id =%d\n",rdabt_clientp->ClientStatus,msg_p->goep_conn_id);	
    offset_len = OBEX_HEADER;
    if (!rdabt_clientp->isFtpClient)
		rdabt_goep_abort_req_reply(0, NULL, 0);
    else
    {
	    if(rdabt_clientp->ClientStatus == GOEP_Client_Connect)
	    {
	        OBEX_GetWriteBuffer(300, &txBuffer);
	        status= OBEX_Abort(msg_p->goep_conn_id, offset_len, txBuffer, rdabt_goep_abort_req_reply);
		//  rdabt_clientp->ClientStatus = GOEP_Client_Disconnect;
	    }
	    //else
	        rdabt_goep_abort_req_reply(0, NULL, 0);
    }
    return;
}

void rdabt_goep_connect_ind(t_bdaddr address,u_int16 ftps);
void rdabt_goep_disconnect_ind(u_int16 ftps);
void rdabt_goep_abort_ind(u_int16 ftps);

/*************************************************************************************************
* FUNCTION :- rdabt_goep_sdp_timeout
*
* DESCRIPTION :- This is the callback for release ACL that has brought up for bonding
*                 
************************************************************************************************/
void rdabt_goep_ftps_timeout_acl(void)
{
	ftps_timer = 0;

	if(MGR_GetACLState(rdabt_ftpsp->FTPSaddress) == 2)//CONN_CLOSED
	{
	       kal_prompt_trace(0, "rdabt_goep_ftps_timeout_acl.1\n");	
		rdabt_goep_disconnect_rsp_ftps_callback(NULL,0);
	}
	else
	{
		       kal_prompt_trace(0, "rdabt_goep_ftps_timeout_acl.2\n");	
		       if(OBEX_L2_ACL_Expired(&(rdabt_ftpsp->FTPSaddress)))
		    	{
		    		prh_l2_acl_shutdown_all(0);
				rdabt_goep_disconnect_rsp_ftps_callback(NULL,0);
		    	}
	}
	return;
}

/*************************************************************************************************
* FUNCTION :- rdabt_goep_ftps_timeout
*
* DESCRIPTION :- This is the callback for release RFCOMM channel that has brought up for FTPS
*                 
************************************************************************************************/
void rdabt_goep_ftps_timeout(void)
{
	OBEX_ServerTransportDisconnect(rdabt_ftpsp->FTPStid);
	ftps_timer = pTimerCreate(20,(rdabt_timer_func)rdabt_goep_ftps_timeout_acl,NULL,pTIMER_ONESHOT);//60 is 6 secend	
	
}

/*************************************************************************************************
* FUNCTION :- rdabt_goep_sdp_timeout
*
* DESCRIPTION :- This is the callback for release ACL that has brought up for bonding
*                 
************************************************************************************************/
void rdabt_goep_opps_timeout_acl(void)
{
	opps_timer = 0;

	if(MGR_GetACLState(rdabt_oppsp->OPPSaddress) == 2)//CONN_CLOSED
	{
	       kal_prompt_trace(0, "rdabt_goep_opps_timeout_acl.1\n");	
		rdabt_goep_disconnect_rsp_opps_callback(NULL,0);
	}
	else
	{
		       kal_prompt_trace(0, "rdabt_goep_opps_timeout_acl.2\n");	
		       if(OBEX_L2_ACL_Expired(&(rdabt_oppsp->OPPSaddress)))
		    	{
		    		prh_l2_acl_shutdown_all(0);
				rdabt_goep_disconnect_rsp_opps_callback(NULL,0);
		    	}
	}
	return;
}

/*************************************************************************************************
* FUNCTION :- rdabt_goep_ftps_timeout
*
* DESCRIPTION :- This is the callback for release RFCOMM channel that has brought up for OPPS
*                 
************************************************************************************************/
void rdabt_goep_opps_timeout(void)
{
	kal_prompt_trace(0, "rdabt_goep_opps_timeout.\n");
	OBEX_ServerTransportDisconnect(rdabt_oppsp->OPPStid);
	opps_timer = pTimerCreate(20,(rdabt_timer_func)rdabt_goep_opps_timeout_acl,NULL,pTIMER_ONESHOT);//60 is 6 secend	
}

/////////////////////////////////////OPP SERVER///////////////////////////////////////
void FTPS_worker_handler(u_int32 tid, u_int8 messageType, t_obex_headers headers, u_int8 *data,u_int16 len,void* cbref)
{
    t_DataBuf *buffer;
    u_int16 offset_incoming;
    u_int16 offset_len;
    static u_int32 len_data_recieved;
    u_int8 status=0;

    t_obex_header_offsets internal;
    int i =0;
    rdabt_ftpsp->FTPStid = tid;
    offset_incoming=OBEX_CONNECT_HEADER;

    kal_prompt_trace(0, "Received a FTPS tid %x\n",tid);

    switch(messageType)
    {
        case OBEX_CONNECT:
        {
        	u_int32 FTPScid;
        	kal_prompt_trace(0, "Received a Connect message\n");

        	rdabt_ftpsp->FTPSmax_len = (data[5] << 8) + data[6];
        	if(rdabt_oppsp->buf_sizeOPPS < rdabt_ftpsp->FTPSmax_len)
        	rdabt_ftpsp->FTPSmax_len = rdabt_ftpsp->buf_sizeFTPS;
        	rdabt_ftpsp->FTPSmax_len -=20;

        	rdabt_ftpsp->FTPSoffsetVar=OBEX_CONNECT_HEADER;
                  OBEX_ExtractHeaders(&internal,&(rdabt_ftpsp->FTPSoffsetVar),data,len);

        	rdabt_goep_connect_ind(rdabt_ftpsp->FTPSaddress,1);

        	rdabt_ftpsp->FTPS_offset_len = OBEX_CONNECT_HEADER;
        	
        	OBEX_GetWriteBuffer(300,&(FTPSbuffer));
        	OBEX_ExtractHeaders(&internal,&offset_incoming,data,len);

        	if(internal.au_cal)
        	{
            			OBEX_AuthResponse(temp_pin_code, temp_pin_len, data+internal.au_cal, &offset_len,buffer);
        	}

        	OBEX_GetCID(tid,((data+internal.tar)[0] << 8) + (data+internal.tar)[1] -3,data+internal.tar + 2,&FTPScid);

        	FTPSbuffer->data[3] = data[3];
        	FTPSbuffer->data[4] = data[4];
        	FTPSbuffer->data[5] = ( rdabt_ftpsp->FTPSmax_len >> 8 )  & 0xFF;
        	FTPSbuffer->data[6] = rdabt_ftpsp->FTPSmax_len & 0xFF;
        	OBEX_Create4ByteHeader(OBEX_CID, FTPScid, FTPSbuffer, &(rdabt_ftpsp->FTPS_offset_len));
        	if(internal.tar)
        		OBEX_CreateSequenceHeader(OBEX_WHO,((data + internal.tar)[0] << 8 ) + (data+internal.tar)[1] -3,data + internal.tar +2,FTPSbuffer,&(rdabt_ftpsp->FTPS_offset_len));

        	if(internal.au_rsp)
        	{
            			status=OBEX_AuthCheck(temp_pin_code, temp_pin_len, nonce, data+internal.au_rsp);	
		        	kal_prompt_trace(0, "Received a au_rsp\n");
        			if(status)// means the client could not authenticate
        			{
        				offset_len = OBEX_CONNECT_HEADER;
        				OBEX_GetWriteBuffer(100, &buffer);

        				//should I disconnect the transport ??					
        				OBEX_Response(tid,OBEX_UNAUTHORIZED,(offset_len),buffer);

        				break;
        				
        			}			

        	}else
        	{
			if(securitySet)	
			{
     				offset_len = OBEX_CONNECT_HEADER;
      				OBEX_GetWriteBuffer(100, &buffer);
				status=OBEX_AuthRequest(temp_pin_code, temp_pin_len, nonce, &offset_len, buffer);
				OBEX_Response(tid,OBEX_UNAUTHORIZED,(offset_len),buffer);
				break;		
			}
        	}
        	break;
        }

        case OBEX_PUT:
        {
            goep_push_ind_struct *param_ptr;
            kal_prompt_trace(0, "Received a Put message\n");
            param_ptr = (goep_push_ind_struct*)construct_local_para((kal_uint16)sizeof(goep_push_ind_struct), TD_CTRL);
            param_ptr->ref_count = 1;
            param_ptr->msg_len = 0x174;
            memcpy((char*)param_ptr->uuid , (const char*)(OBEX_FTP_UUID) , 16);
            param_ptr->uuid_len = 0x10;
            param_ptr->goep_conn_id = rdabt_ftpsp->goep_tid_FTPS ;
            param_ptr->frag_ptr = rdabt_ftpsp->buf_ptrFTPS;

            param_ptr->total_obj_len = 0;
            param_ptr->obj_name[0]=0;
            param_ptr->obj_mime_type[0]=0;
            param_ptr->frag_ptr = 0;
            param_ptr->frag_len= 0;
            param_ptr->put_type = GOEP_PUT_NORMAL ;

            /* I will need to store data here */
            offset_len = OBEX_HEADER;
            OBEX_ExtractHeaders(&internal,&offset_len,data,len);


            if(rdabt_ftpsp->FTPSfirst)
            {
            	if(internal.name)
            	{

            		memcpy((char*)param_ptr->obj_name , (const char*)((data + internal.name+2)) , (((data + internal.name)[0] << 8 ) + (data+internal.name)[1] -3));
            		if(param_ptr->obj_name)
            		{
            			len_data_recieved = 0;
            			rdabt_ftpsp->FTPSfirst = 0;
            		} else {
            			OBEX_GetWriteBuffer(0,&buffer);
            			OBEX_Response(tid,OBEX_BAD_REQUEST,0,buffer);
            			break;
            		}
            	}
            	if(internal.type)
            	{
            		strncpy(  (char*)param_ptr->obj_mime_type , (const char*)(data + internal.type + 2) , (((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3));
            		param_ptr->obj_mime_type[(((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3)] = 0;
            	}
            	if(internal.len)
            	param_ptr->total_obj_len = (((data + internal.len)[0] << 24 ) + ((data + internal.len)[1] << 16 ) + ((data + internal.len)[2] << 8 ) + (data+internal.len)[3] );

               param_ptr->pkt_type = GOEP_FIRST_PKT;

            }
            else
                   param_ptr->pkt_type = GOEP_NORMAL_PKT;

            if(internal.body)
            {
            	len_data_recieved += ((data + internal.body)[0] << 8) + (data + internal.body)[1] -3;
            	param_ptr->frag_ptr= (data + internal.body + 2);		   
            	param_ptr->frag_len= (((data + internal.body)[0] << 8) + (data + internal.body)[1] -3);

            	if(data != rdabt_ftpsp->buf_ptrFTPS)
            		kal_prompt_trace(0, "May a error!!!!!!! FTPS 1\n");
            }
			
            kal_prompt_trace(0, "Received a Final Put message ,frag_len = %x\n",param_ptr->frag_len);

            rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PUSH_IND, (void *)param_ptr, NULL);	
            break;

        }
    	break;
        
        case OBEX_DISCONNECT:
        	kal_prompt_trace(0, "Received a Disconnect message\n");
        	OBEX_GetWriteBuffer(0,&buffer);
        	OBEX_Response(tid,OBEX_OK,0,buffer);
		if(!ftps_timer)
		    	ftps_timer = pTimerCreate(30,(rdabt_timer_func)rdabt_goep_ftps_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend	
//        	rdabt_goep_disconnect_ind(1);
        	kal_prompt_trace(0, "he want disconnect ftps two.. \n");
        	break;

        case OBEX_GET:
        	kal_prompt_trace(0, "Received a OBEX_GET message\n");
        	break;
            
        case OBEX_PUT_FINAL:
        {
            goep_push_ind_struct *param_ptr;
            kal_prompt_trace(0, "Received a Final Put message\n");
            param_ptr = (goep_push_ind_struct*)construct_local_para((kal_uint16)sizeof(goep_push_ind_struct), TD_CTRL);
            param_ptr->ref_count = 1;
            param_ptr->msg_len = 0x174;
            memcpy((char*)param_ptr->uuid , (const char*)(OBEX_FTP_UUID) , 16);
            param_ptr->uuid_len = 0x10;
            param_ptr->goep_conn_id = rdabt_ftpsp->goep_tid_FTPS ;
            param_ptr->frag_ptr = rdabt_ftpsp->buf_ptrFTPS;

            param_ptr->total_obj_len = 0;
            param_ptr->obj_name[0]=0;
            param_ptr->obj_mime_type[0]=0;
            param_ptr->frag_ptr = 0;
            param_ptr->frag_len= 0;
            /* only here can I process data*/
            offset_len = OBEX_HEADER;
            OBEX_ExtractHeaders(&internal,&offset_len,data,len);

            if(internal.eob || internal.body)
            {	
            	if(rdabt_ftpsp->FTPSfirst)
            	{
            		memcpy((char*)param_ptr->obj_name , (const char*)((data + internal.name+2)) , (((data + internal.name)[0] << 8 ) + (data+internal.name)[1] -3));
            		if(param_ptr->obj_name)
            		{
            			len_data_recieved = 0;
            			rdabt_ftpsp->FTPSfirst = 0;
            			} else {
            			OBEX_GetWriteBuffer(0,&buffer);
            			OBEX_Response(tid,OBEX_BAD_REQUEST,0,buffer);
            			break;
            		}

            		if(internal.type)
            		{
            			strncpy(  (char*)param_ptr->obj_mime_type , (const char*)(data + internal.type + 2) , (((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3));
            			param_ptr->obj_mime_type[(((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3)] = 0;
            		}

            		if(internal.len)
            		param_ptr->total_obj_len = (((data + internal.len)[0] << 8 ) + (data+internal.len)[1] );

            	       param_ptr->pkt_type = GOEP_SINGLE_PKT;//? or GOEP_FIRST_PKT
            	       param_ptr->put_type = GOEP_PUT_NORMAL ;

            		if((internal.body == 0)&&(internal.eob == 0))
    			{
    				rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PUSH_IND, (void *)param_ptr, NULL);	
    				break;
    			}
            	}
            	rdabt_ftpsp->FTPSfirst = 1;

            	if(internal.body)
            	{
            		len_data_recieved += ((data + internal.body)[0] << 8) + (data + internal.body)[1] -3;
            	       param_ptr->pkt_type = GOEP_NORMAL_PKT;//or ?
            	       param_ptr->put_type = GOEP_PUT_NORMAL ;
            		param_ptr->frag_ptr= (data + internal.body + 2);		   
            		param_ptr->frag_len= (((data + internal.body)[0] << 8) + (data + internal.body)[1] -3);

            		if(data != rdabt_ftpsp->buf_ptrFTPS)
            			kal_prompt_trace(0, "May a error!!!!!!! FTPS 3 \n");


			kal_prompt_trace(0, "frag_len = %x\n",param_ptr->frag_len);

            		rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PUSH_IND, (void *)param_ptr, NULL);	
            		break;
            	}
            	
            	if(internal.eob)
            	{
            		len_data_recieved += ((data + internal.eob)[0] << 8) + (data + internal.eob)[1] -3;

            	       param_ptr->pkt_type = GOEP_FINAL_PKT;
            	       param_ptr->put_type = GOEP_PUT_NORMAL ;
            		param_ptr->frag_ptr= (data + internal.eob + 2);		   
            		param_ptr->frag_len= (((data + internal.eob)[0] << 8) + (data + internal.eob)[1] -3);

            		if(data != rdabt_ftpsp->buf_ptrFTPS)
            			kal_prompt_trace(0, "May a error!!!!!!! FTPS 4 \n");


			kal_prompt_trace(0, "frag_len = %x\n",param_ptr->frag_len);

            		rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PUSH_IND, (void *)param_ptr, NULL);	
            		break;

            	}
            		kal_prompt_trace(0, "May a error!!!!!!! FTPS 5 \n");
            	
            	rdabt_ftpsp->FTPSfirst = 1;
            	OBEX_GetWriteBuffer(20,&buffer);
            	OBEX_Response(tid,OBEX_OK,0,buffer);
            } 
            else 
            { //delete
                if(rdabt_ftpsp->FTPSfirst)
                {
                	memcpy((char*)param_ptr->obj_name , (const char*)((data + internal.name+2)) , (((data + internal.name)[0] << 8 ) + (data+internal.name)[1] -3));
                	if(param_ptr->obj_name)
                	{
                		len_data_recieved = 0;
                        // 	rdabt_ftpsp->FTPSfirst = 0;
                		} else {
                		OBEX_GetWriteBuffer(0,&buffer);
                		OBEX_Response(tid,OBEX_BAD_REQUEST,0,buffer);
                		break;
                	}

                	if(internal.type)
                	{
                		strncpy(  (char*)param_ptr->obj_mime_type , (const char*)(data + internal.type + 2) , (((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3));
                		param_ptr->obj_mime_type[(((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3)] = 0;
                	}

                	if(internal.len)
                	param_ptr->total_obj_len = (((data + internal.len)[0] << 8 ) + (data+internal.len)[1] );

                       param_ptr->pkt_type = GOEP_FIRST_PKT;//? or GOEP_FIRST_PKT
                       param_ptr->put_type = GOEP_PUT_DELETE ;

                	rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PUSH_IND, (void *)param_ptr, NULL);	
                	break;
                }
                else
                    kal_prompt_trace(0, "May a error!!!!!!! FTPS 6 \n");
        	}
        	break;
        }
        
        case OBEX_GET_FINAL:
        	{
        		goep_pull_ind_struct *param_ptr;
        		kal_prompt_trace(0, "Received a Final Get message\n");
        	       param_ptr = (goep_pull_ind_struct*)construct_local_para((kal_uint16)sizeof(goep_pull_ind_struct), TD_CTRL);
        	       param_ptr->ref_count = 1;
        	       param_ptr->msg_len = 0x168;
        			memcpy((char*)param_ptr->uuid , (const char*)(OBEX_FTP_UUID) , 16);
        	       param_ptr->uuid_len = 0x10;
        	       param_ptr->goep_conn_id = rdabt_ftpsp->goep_tid_FTPS ;
        	       param_ptr->pkt_type = GOEP_NORMAL_PKT ;

        		offset_len = OBEX_HEADER;
        		OBEX_ExtractHeaders(&internal,&offset_len,data,len);

        		memset((char*)param_ptr->obj_name , 0 , GOEP_MAX_OBJ_NAME);
        		memset((char*)param_ptr->obj_mime_type , 0 , GOEP_MAX_MIME_TYPE);

        		if((internal.type == 0) && (internal.name == 0))
        		{
        			memcpy(  (char*)param_ptr->obj_mime_type , (const char*)(rdabt_ftpsp->ftps_objs_mime_type) , (80));
        			memcpy(  (char*)param_ptr->obj_name , (const char*)(rdabt_ftpsp->ftps_obj_name) , (256));
        		}

        		if(internal.type)
        		{
    				strncpy(  (char*)param_ptr->obj_mime_type , (const char*)(data + internal.type + 2) , (((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3));
        			param_ptr->obj_mime_type[(((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3)] = 0;
    				strncpy(  (char*)rdabt_ftpsp->ftps_objs_mime_type , (const char*)(data + internal.type + 2) , (((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3));
        			rdabt_ftpsp->ftps_objs_mime_type[(((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3)] = 0;
        		}else
        		{
	        		memset((char*)rdabt_ftpsp->ftps_objs_mime_type , 0 , GOEP_MAX_MIME_TYPE);
        		}
        		

        		if(internal.name)
        		{
        			for( i =0;i<(((data + internal.name)[0] << 8 ) + (data+internal.name)[1] -3);i++)
    				{
    					param_ptr->obj_name[i]=(data + internal.name+2)[i];
    					rdabt_ftpsp->ftps_obj_name[i]=(data + internal.name+2)[i];
    				}
        			param_ptr->obj_name[i]=0;
        			rdabt_ftpsp->ftps_obj_name[i]=0;
        			param_ptr->obj_name[i+1]=0;
        			rdabt_ftpsp->ftps_obj_name[i+1]=0;
        		}
        		else
        		{
	        		memset((char*)rdabt_ftpsp->ftps_obj_name , 0 , GOEP_MAX_OBJ_NAME);
        		}
        		
        		rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PULL_IND, (void *)param_ptr, NULL);	
        	break;
        	}
        	
        case OBEX_SETPATH:
        {
        		goep_set_folder_ind_struct *param_ptr;
        		kal_prompt_trace(0, "Received a SetPath message\n");
        	       param_ptr = (goep_set_folder_ind_struct*)construct_local_para((kal_uint16)sizeof(goep_set_folder_ind_struct), TD_CTRL);
        	       param_ptr->ref_count = 1;
        	       param_ptr->msg_len = 0x118;
    			memcpy((char*)param_ptr->uuid , (const char*)(OBEX_FTP_UUID) , 16);
        	       param_ptr->uuid_len = 0x10;
        	       param_ptr->goep_conn_id = rdabt_ftpsp->goep_tid_FTPS ;

        		offset_len = OBEX_HEADER;
        		OBEX_ExtractHeaders(&internal,&offset_len,data,len);

        	if(data[3] & 0x01)    // go back up a directory
        	{
        	       param_ptr->setpath_flag = GOEP_BACK_FOLDER ;
        	}
        	else if(!(data[3] & 0x2))
        	{
        	       param_ptr->setpath_flag = GOEP_CREATE_FOLDER ;
        	}
        	else if(((data + internal.name)[0] << 8 ) + (data+internal.name)[1] -3)
        	{
        	       param_ptr->setpath_flag = GOEP_FORWARD_FOLDER ;
        	}
        	else
        	{
        	       param_ptr->setpath_flag = GOEP_ROOT_FOLDER ;
        	}

        		
        		memset((char*)param_ptr->folder_name , 0 , GOEP_MAX_FOLDER_NAME);

        		if(internal.name)
        		{
        			for( i =0;i<(((data + internal.name)[0] << 8 ) + (data+internal.name)[1] -3);i++)
    				{
    					param_ptr->folder_name[i]=(data + internal.name+2)[i];
    				}
        		}

        	rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_SET_FOLDER_IND, (void *)param_ptr, NULL);	


        	break;
        	}
        
        case OBEX_ABORT:
        		kal_prompt_trace(0, "Received a Abort message\n");
        	rdabt_goep_abort_ind(1);		
        	break;
	default:
        	kal_prompt_trace(0, "Received a message : %x \n",messageType);
		if((0 == messageType) && (rdabt_ftpsp->FTPSstatus == GOEP_Client_Connect) && (NULL == data))
		{
			//rdabt_goep_abort_ind(1);		
			//rdabt_ftpsp->FTPSstatus = GOEP_Client_Disconnect;
			//OBEX_ServerTransportDisconnect(rdabt_ftpsp->FTPStid);
			//rdabt_goep_disconnect_rsp_ftps(0, NULL, 0);			
//			rdabt_goep_disconnect_rsp_ftps_callback(0,0);
	        	kal_prompt_trace(0, "he want disconnect ftps one.. \n");
			if(!ftps_timer)
	    			ftps_timer = pTimerCreate(30,(rdabt_timer_func)rdabt_goep_ftps_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend	
		}
		else
		{
        	kal_prompt_trace(0, "Received a message : %x \n",messageType);
            //OBEX_GetWriteBuffer(0,&buf);
            //OBEX_Response(tid,OBEX_NOT_IMPLEMENTED,0,(t_DataBuf*)buf);
		}

        	break;
    }
		
    return;

}
	
/*****************************************************************************
 * FUNCTION
 *  ServiceHandler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void OPPServiceHandler(u_int32 tid, u_int8 messageType, t_obex_headers headers, u_int8 *data,u_int16 len,void* cbref)
{
    t_DataBuf* buf;
    t_DataBuf *buffer;
    u_int16 offset_len;
    static u_int32 len_data_recieved = 0;
    t_obex_header_offsets internal;

    rdabt_oppsp->OPPStid = tid;

    kal_prompt_trace(0, "Received a OPPS tid %x\n",tid);

    if(data != rdabt_oppsp->buf_ptrOPPS)
    {
        kal_prompt_trace(0, "May a error!!!!!!! 0 \n");
        kal_prompt_trace(0, "data = 0x %x 0 \n",data);
        kal_prompt_trace(0, "buf_ptrOPPS = 0x %x 0 \n",rdabt_oppsp->buf_ptrOPPS);
    }

    switch(messageType)
    {
        case OBEX_CONNECT:
            kal_prompt_trace(0, "Received a Connect message\n");

            rdabt_oppsp->OPPSmax_len = (data[5] << 8) + data[6];
            if(rdabt_oppsp->buf_sizeOPPS < rdabt_oppsp->OPPSmax_len)
                rdabt_oppsp->OPPSmax_len = rdabt_oppsp->buf_sizeOPPS;
            rdabt_oppsp->OPPSmax_len -=20;
		
            rdabt_oppsp->OPPSoffsetVar=OBEX_CONNECT_HEADER;
            OBEX_ExtractHeaders(&internal,&(rdabt_oppsp->OPPSoffsetVar),data,len);
            rdabt_goep_connect_ind(rdabt_oppsp->OPPSaddress,0);
        break;
	
        case OBEX_DISCONNECT:
            kal_prompt_trace(0, "Received a Disconnect message\n");
            OBEX_GetWriteBuffer(0,&buffer);
            OBEX_Response(tid,OBEX_OK,0,buffer);
	     if(!opps_timer)
		    opps_timer = pTimerCreate(60,(rdabt_timer_func)rdabt_goep_opps_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend	
            kal_prompt_trace(0, "he want disconnect opps two.. \n");
//	     rdabt_oppsp->OPPSfirst = 1;
//            rdabt_goep_disconnect_ind(0);
        break;
		
        case OBEX_PUT:
        {
            goep_push_ind_struct *param_ptr;
            kal_prompt_trace(0, "Received a Put message\n");
            param_ptr = (goep_push_ind_struct*)construct_local_para((kal_uint16)sizeof(goep_push_ind_struct), TD_CTRL);
            param_ptr->ref_count = 1;
            param_ptr->msg_len = 0x174;
            param_ptr->uuid[0] = 0;
            param_ptr->uuid_len = 0;
            param_ptr->goep_conn_id = rdabt_oppsp->goep_tid_OPPS ;
            param_ptr->frag_ptr = rdabt_oppsp->buf_ptrOPPS;
            param_ptr->total_obj_len = 0;
            param_ptr->obj_name[0]=0;
            param_ptr->obj_mime_type[0]=0;
            param_ptr->frag_len= 0;
            param_ptr->put_type = GOEP_PUT_NORMAL ;

            /* I will need to store data here */
            offset_len = OBEX_HEADER;
            OBEX_ExtractHeaders(&internal,&offset_len,data,len);

            if(rdabt_oppsp->OPPSfirst)
            {
                if(internal.name)
                {
                    memcpy((char*)param_ptr->obj_name , (const char*)((data + internal.name+2)) , (((data + internal.name)[0] << 8 ) + (data+internal.name)[1] -3));
                    if(param_ptr->obj_name)
                    {
                        len_data_recieved = 0;
                        rdabt_oppsp->OPPSfirst = 0;
                    } 
                    else 
                    {
                        OBEX_GetWriteBuffer(0,&buffer);
                        OBEX_Response(tid,OBEX_BAD_REQUEST,0,buffer);
                        break;
                    }
                }

                if(internal.type)
                {
                    strncpy(  (char*)param_ptr->obj_mime_type , (const char*)(data + internal.type + 2) , (((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3));
                    param_ptr->obj_mime_type[(((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3)] = 0;
                }

                if(internal.len)
                    param_ptr->total_obj_len = (((data + internal.len)[0] << 24 ) + ((data + internal.len)[1] << 16 ) + ((data + internal.len)[2] << 8 ) + (data+internal.len)[3] );

                param_ptr->pkt_type = GOEP_FIRST_PKT;
                param_ptr->put_type = GOEP_PUT_NORMAL ;

            }
            else
                param_ptr->pkt_type = GOEP_NORMAL_PKT;
		

            if(internal.body)
            {
                len_data_recieved += ((data + internal.body)[0] << 8) + (data + internal.body)[1] -3;
                param_ptr->put_type = GOEP_PUT_NORMAL ;
                param_ptr->frag_ptr= (data + internal.body + 2);		   
                param_ptr->frag_len= (((data + internal.body)[0] << 8) + (data + internal.body)[1] -3);

                if(data != rdabt_oppsp->buf_ptrOPPS)
                    kal_prompt_trace(0, "May a error!!!!!!! OPPS 1 \n");

            }
	      kal_prompt_trace(0, "param_ptr->frag_len = %d param_ptr->total_obj_len=%d\n",param_ptr->frag_len,param_ptr->total_obj_len);
	      kal_prompt_trace(0, "internal.name=%d,internal.type=%d,internal.body=%d,internal.len=%d \n",internal.name,internal.type,internal.body,internal.len);
            rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PUSH_IND, (void *)param_ptr, NULL);	
        break;
        }
        
        case OBEX_PUT_FINAL:
        {
            goep_push_ind_struct *param_ptr;
            kal_prompt_trace(0, "Received a Final Put message\n");
            param_ptr = (goep_push_ind_struct*)construct_local_para((kal_uint16)sizeof(goep_push_ind_struct), TD_CTRL);
            param_ptr->ref_count = 1;
            param_ptr->msg_len = 0x174;
            param_ptr->uuid[0] = 0;
            param_ptr->uuid_len = 0;
            param_ptr->goep_conn_id = rdabt_oppsp->goep_tid_OPPS ;
            param_ptr->frag_ptr = rdabt_oppsp->buf_ptrOPPS;
            param_ptr->total_obj_len = 0;
            param_ptr->obj_name[0]=0;
            param_ptr->obj_mime_type[0]=0;
            param_ptr->frag_len= 0;

            /* only here can I process data*/
            offset_len = OBEX_HEADER;
            OBEX_ExtractHeaders(&internal,&offset_len,data,len);

            if(internal.eob || internal.body)
            {	
                if(rdabt_oppsp->OPPSfirst)
                {
                    memcpy((char*)param_ptr->obj_name , (const char*)((data + internal.name+2)) , (((data + internal.name)[0] << 8 ) + (data+internal.name)[1] -3));
                    if(param_ptr->obj_name)
                    {
                        len_data_recieved = 0;
                        rdabt_oppsp->OPPSfirst = 0;
                    } 
                    else 
                    {
                        OBEX_GetWriteBuffer(0,&buffer);
                        OBEX_Response(tid,OBEX_BAD_REQUEST,0,buffer);
                        break;
                    }

                    if(internal.type)
                    {
                        strncpy(  (char*)param_ptr->obj_mime_type , (const char*)(data + internal.type + 2) , (((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3));
                        param_ptr->obj_mime_type[(((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3)] = 0;
                    }

                    if(internal.len)
                        param_ptr->total_obj_len = (((data + internal.len)[0] << 8 ) + (data+internal.len)[1] );

                    param_ptr->pkt_type = GOEP_SINGLE_PKT;//? or GOEP_FIRST_PKT
                    param_ptr->put_type = GOEP_PUT_NORMAL ;

                    if((internal.body == 0)&&(internal.eob == 0))
                    {
                        rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PUSH_IND, (void *)param_ptr, NULL);	
                        break;
                    }
                }

                rdabt_oppsp->OPPSfirst = 1;

                if(internal.body)
                {
                    len_data_recieved += ((data + internal.body)[0] << 8) + (data + internal.body)[1] -3;
                    param_ptr->pkt_type = GOEP_NORMAL_PKT;//or ?
                    param_ptr->put_type = GOEP_PUT_NORMAL ;
                    param_ptr->frag_ptr= (data + internal.body + 2);		   
                    param_ptr->frag_len= (((data + internal.body)[0] << 8) + (data + internal.body)[1] -3);

                    if(data != rdabt_oppsp->buf_ptrOPPS)
                        kal_prompt_trace(0, "May a error!!!!!!! OPPS 3 \n");

                    rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PUSH_IND, (void *)param_ptr, NULL);	
                    break;
                }

                if(internal.eob)
                {
                    len_data_recieved += ((data + internal.eob)[0] << 8) + (data + internal.eob)[1] -3;

                    param_ptr->pkt_type = GOEP_FINAL_PKT;
                    param_ptr->put_type = GOEP_PUT_NORMAL ;
                    param_ptr->frag_ptr= (data + internal.eob + 2);		   
                    param_ptr->frag_len= (((data + internal.eob)[0] << 8) + (data + internal.eob)[1] -3);

                    if(data != rdabt_oppsp->buf_ptrOPPS)
                        kal_prompt_trace(0, "May a error!!!!!!! OPPS 4 \n");

                    rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PUSH_IND, (void *)param_ptr, NULL);	
                    break;

                }

                kal_prompt_trace(0, "May a error!!!!!!! OPPS 5 \n");

                OBEX_GetWriteBuffer(20,&buffer);
                OBEX_Response(tid,OBEX_OK,0,buffer);
            } 
            else 
            { //delete
			if(rdabt_oppsp->OPPSfirst)
                {
                    memcpy((char*)param_ptr->obj_name , (const char*)((data + internal.name+2)) , (((data + internal.name)[0] << 8 ) + (data+internal.name)[1] -3));
                    if(param_ptr->obj_name)
                    {
                        len_data_recieved = 0;
					rdabt_oppsp->OPPSfirst = 0;
                    } 
                    else 
                    {
                        OBEX_GetWriteBuffer(0,&buffer);
                        OBEX_Response(tid,OBEX_BAD_REQUEST,0,buffer);
                        break;
                    }

                    if(internal.type)
                    {
                        strncpy(  (char*)param_ptr->obj_mime_type , (const char*)(data + internal.type + 2) , (((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3));
                        param_ptr->obj_mime_type[(((data + internal.type)[0] << 8 ) + (data+internal.type)[1] -3)] = 0;
                    }

                    if(internal.len)
                        param_ptr->total_obj_len = (((data + internal.len)[0] << 8 ) + (data+internal.len)[1] );

                    param_ptr->pkt_type = GOEP_SINGLE_PKT;//? or GOEP_FIRST_PKT
                    param_ptr->put_type = GOEP_PUT_NORMAL ;

                    rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_PUSH_IND, (void *)param_ptr, NULL);	
                    break;
                }
                else
                    kal_prompt_trace(0, "May a error!!!!!!! OPPS 6 \n");

/*
			if(internal.name)
			{
				name = u2a(data + internal.name + 2,((data + internal.name)[0] << 8 ) + (data+internal.name)[1] -3);
				if(name)
				{
					CFileFind finder;
					CString filepath;
					int status;
					filepath=rootDir + currentDir + '\\' +(char*)name;

					if(finder.FindFile((LPCTSTR)filepath)) // see if there is a file called this
					{
						status=finder.FindNextFile();// actually find that file

						//filepath=finder.GetFilePath();

						if(finder.IsDirectory())
						{
							finder.Close();
							status=_rmdir((LPCTSTR)filepath);
							if(status==-1)
							{
								OBEX_GetWriteBuffer(0,&buffer);
								OBEX_Response(tid,OBEX_PRECONDITION_FAIL,0,buffer);
								break;
							}		

						}else
						{
							finder.Close();
							status=remove((LPCTSTR)filepath);
							if(status==-1)
							{
								OBEX_GetWriteBuffer(0,&buffer);
								OBEX_Response(tid,OBEX_FORBIDDEN,0,buffer);
								break;
							}
						}
						
						OBEX_GetWriteBuffer(0,&buffer);
						OBEX_Response(tid,OBEX_OK,0,buffer);

					} else {
						// not found
						OBEX_GetWriteBuffer(0,&buffer);
						OBEX_Response(tid,OBEX_NOT_FOUND,0,buffer);
					}
				} else {
					// invalid
					OBEX_GetWriteBuffer(0,&buffer);
					OBEX_Response(tid,OBEX_BAD_REQUEST,0,buffer);
					
				}
			} else {
				// invalid
				OBEX_GetWriteBuffer(0,&buffer);
				OBEX_Response(tid,OBEX_BAD_REQUEST,0,buffer);
			}
*/			
		}
		break;
        }
        
	case OBEX_GET:
            kal_prompt_trace(0, "Received a Get message OPPS\n");
        break;
        
	case OBEX_GET_FINAL:
			kal_prompt_trace(0, "Received a Final Get message OPPS\n");
        break;
        
	case OBEX_SETPATH:
			kal_prompt_trace(0, "Received a SetPath message OPPS\n");
        break;
        
	case OBEX_ABORT:
                kal_prompt_trace(0, "Received a Abort message\n");
                //		OBEX_GetWriteBuffer(0,&buffer);
                //		OBEX_Response(tid,OBEX_CONTINUE,0,buffer);
                rdabt_goep_abort_ind(0);		
        break;
        
	default:
            kal_prompt_trace(0, "Received a message of type 0x%x from the Client\n",messageType);

			if((0 == messageType) && (rdabt_oppsp->OPPSstatus == GOEP_Client_Connect) && (NULL == data))
			{
				//  rdabt_goep_abort_ind(0);		
				//	rdabt_oppsp->OPPSstatus = GOEP_Client_Disconnect;
				//	OBEX_ServerTransportDisconnect(rdabt_oppsp->OPPStid);
//				rdabt_goep_disconnect_rsp_opps_callback(0,0);
		        	kal_prompt_trace(0, "he want disconnect opps one.. \n");
			       if(!opps_timer)
					opps_timer = pTimerCreate(60,(rdabt_timer_func)rdabt_goep_opps_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend	
			}
			else
			{
	            OBEX_GetWriteBuffer(0,&buf);
	            OBEX_Response(tid,OBEX_NOT_IMPLEMENTED,0,(t_DataBuf*)buf);
			}
            return;
	}

    //	OPPS_worker_handler(tid,messageType,data,len);
    kal_prompt_trace(0, "Received a  %x byte\n",len_data_recieved);

}

/*****************************************************************************
 * FUNCTION
 *  rdabt_BM_LINK_CONNECT_ACCEPT_IND
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_BM_LINK_CONNECT_ACCEPT_IND(t_bdaddr address)
{
    bt_bm_link_connect_accept_ind_struct *param_ptr ;
    param_ptr = (bt_bm_link_connect_accept_ind_struct*) construct_local_para(sizeof(bt_bm_link_connect_accept_ind_struct), TD_CTRL);
	kal_prompt_trace(0, "rdabt_BM_LINK_CONNECT_ACCEPT_IND...  20090220-Rdabt_opp!@!!!!!!");

	param_ptr->ref_count = 1;
	param_ptr->msg_len = 0xc;
	param_ptr->bd_addr.lap= BDADDR_Get_LAP(&address);
	param_ptr->bd_addr.uap = BDADDR_Get_UAP(&address);
	param_ptr->bd_addr.nap = BDADDR_Get_NAP(&address);

    //HCI_AcceptConnectionRequest(address,0x00); /* 0x01 == do not request role switch */
	rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_CONNECT_ACCEPT_IND, (void *)param_ptr, NULL);
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_bt_link_connect_accept_rsp
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_bt_link_connect_accept_rsp(bt_bm_link_connect_accept_rsp_struct *msg_p)
{
	t_bdaddr tempbd;
	BDADDR_Set_LAP(&tempbd,msg_p ->bd_addr.lap);
	BDADDR_Set_UAP(&tempbd,msg_p ->bd_addr.uap);
	BDADDR_Set_NAP(&tempbd,msg_p ->bd_addr.nap);	

	if(msg_p ->result == BTBM_ADP_SUCCESS )
		{
			    bt_bm_link_state_ind_struct *param_ptr ;
		
			    param_ptr = (bt_bm_link_state_ind_struct*) construct_local_para(sizeof(bt_bm_link_state_ind_struct), TD_CTRL);
		    	param_ptr->ref_count  = 1;
		    	param_ptr->msg_len    = 6;
		    	param_ptr->max_number = 3;
			    param_ptr->current_number = 1 ;//0x2;
				rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_STATE_IND, (void *)param_ptr, NULL);
				//    HCI_AcceptConnectionRequest(tempbd,0x01); /* 0x01 == do not request role switch */
				//    HCI_AcceptConnectionRequest(tempbd,0x00); /* 0x01 == do not request role switch */
		}		
	//else
				//    HCI_RejectConnectionRequest(tempbd,MGR_HCI_CONNREJECT_REASON); 

}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_authorize_ind_ftps
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_authorize_ind_ftps(t_bdaddr address, int status)
{
    goep_authorize_ind_struct *param_ptr ;
    param_ptr = (goep_authorize_ind_struct*) construct_local_para(sizeof(goep_authorize_ind_struct), TD_CTRL);
	kal_prompt_trace(0, "rdabt_goep_authorize_ind_ftps...  20090220-Rdabt_opp!@!!!!!!  rdabt_clientp->ClientStatus =%d, rdabt_ftpsp->goep_tid_FTPS = %d rdabt_ftpsp->FTPStid =%d\n",rdabt_clientp->ClientStatus, rdabt_ftpsp->goep_tid_FTPS,rdabt_ftpsp->FTPStid);

#ifndef MGR_MULTI_CONNECTION
   if( rdabt_clientp->ClientStatus != GOEP_Client_Idle)  
   {
	RSE_SrvConnectResponse(rdabt_ftpsp->FTPSaddress, rdabt_ftpsp->FTPSdlci, 0);
	//OBEX_ServerTransportDisconnect(rdabt_ftpsp->FTPStid);
	//rdabt_goep_disconnect_rsp_ftps(0, NULL, 0);
	return;	
   }
#endif   
	param_ptr->ref_count = 1;
	param_ptr->msg_len = 0x70;
		{
			memcpy((char*)param_ptr->uuid , (const char*)(OBEX_FTP_UUID) , 16);
                     param_ptr->uuid_len = 0x10;
			param_ptr->goep_conn_id = rdabt_ftpsp->goep_tid_FTPS;

			rdabt_ftpsp->FTPSaddress.bytes[0] = address.bytes[0];
			rdabt_ftpsp->FTPSaddress.bytes[1] = address.bytes[1];
			rdabt_ftpsp->FTPSaddress.bytes[2] = address.bytes[2];
			rdabt_ftpsp->FTPSaddress.bytes[3] = address.bytes[3];
			rdabt_ftpsp->FTPSaddress.bytes[4] = address.bytes[4];
			rdabt_ftpsp->FTPSaddress.bytes[5] = address.bytes[5];
		}
	param_ptr->bd_addr.lap= BDADDR_Get_LAP(&address);
	param_ptr->bd_addr.uap = BDADDR_Get_UAP(&address);
	param_ptr->bd_addr.nap = BDADDR_Get_NAP(&address);
	memset((char*)param_ptr->dev_name , 0 , 80);	
	if( 0 == status )
	{
		memcpy((char*)param_ptr->dev_name , (const char*)(rdabt_ftpsp->ftps_obj_name) , 80);	
	}
	memset(rdabt_ftpsp->ftps_obj_name, 0, 256);

	if(!MGR_SetACLCallBack(rdabt_ftpsp->FTPSaddress,0,rdabt_goep_disconnect_rsp_ftps_callback_full))
		kal_prompt_trace(0, "MGR_SetACLCallBack!@!!!!!!");	
	else
		kal_prompt_trace(0, "MGR_SetACLCallBack fail");

	rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_AUTHORIZE_IND, (void *)param_ptr, NULL);
}



/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_authorize_ind_ftps
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_authorize_ind_opps(t_bdaddr address, int status)
{
    goep_authorize_ind_struct *param_ptr ;
    param_ptr = (goep_authorize_ind_struct*) construct_local_para(sizeof(goep_authorize_ind_struct), TD_CTRL);
    kal_prompt_trace(0, "rdabt_goep_authorize_ind_ftps...  20090220-Rdabt_opp!@!!!!!! status=%d, rdabt_clientp->ClientStatus=%d\n",status,rdabt_clientp->ClientStatus);
#ifndef MGR_MULTI_CONNECTION

   if(  rdabt_clientp->ClientStatus != GOEP_Client_Idle)
   {
		RSE_SrvConnectResponse(rdabt_oppsp->OPPSaddress, rdabt_oppsp->OPPSdlci, 0);
		return;	
   }
#endif
	param_ptr->ref_count = 1;
	param_ptr->msg_len = 0x70;
		{
			param_ptr->uuid[0] = 0;
			param_ptr->uuid_len = 0;
			param_ptr->goep_conn_id = rdabt_oppsp->goep_tid_OPPS;

			rdabt_oppsp->OPPSaddress.bytes[0] = address.bytes[0];
			rdabt_oppsp->OPPSaddress.bytes[1] = address.bytes[1];
			rdabt_oppsp->OPPSaddress.bytes[2] = address.bytes[2];
			rdabt_oppsp->OPPSaddress.bytes[3] = address.bytes[3];
			rdabt_oppsp->OPPSaddress.bytes[4] = address.bytes[4];
			rdabt_oppsp->OPPSaddress.bytes[5] = address.bytes[5];
		}

	param_ptr->bd_addr.lap= BDADDR_Get_LAP(&address);
	param_ptr->bd_addr.uap = BDADDR_Get_UAP(&address);
	param_ptr->bd_addr.nap = BDADDR_Get_NAP(&address);
	memset((char*)param_ptr->dev_name , 0 , 80);	
	if( 0 == status )
	{
		memcpy((char*)param_ptr->dev_name , (const char*)(rdabt_oppsp->opps_obj_name) , 80);	
	}
	memset(rdabt_oppsp->opps_obj_name, 0, 80);

	if(!MGR_SetACLCallBack(rdabt_oppsp->OPPSaddress,0,rdabt_goep_disconnect_rsp_opps_callback_full))
		kal_prompt_trace(0, "MGR_SetACLCallBack!@!!!!!!");	
	else
		kal_prompt_trace(0, "MGR_SetACLCallBack fail");	

	rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_AUTHORIZE_IND, (void *)param_ptr, NULL);
}


/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_authorize_ind_ftps
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_authorize_ind(t_bdaddr address, u_int8 dlci, u_int8 schan, u_int8 tids)
{
	int status = 0;
	kal_prompt_trace(0, "rdabt_goep_authorize_ind_ftps...  20090220-Rdabt_opp!@!!!!!! schan=%d dlci=%d",schan,dlci);

	if(schan == RDA_FTPS_Server_Channel)
		{
			  rdabt_ftpsp->FTPStid = tids;
			memset(rdabt_ftpsp->ftps_obj_name, 0, 256);
		   	status = MGR_GetRemoteName( address,  (char*)rdabt_ftpsp->ftps_obj_name, 80,  &NAME_obex, rdabt_goep_authorize_ind_ftps);
			if( status == 0)
			{
				rdabt_goep_authorize_ind_ftps(address, 0);	
			}
                     rdabt_ftpsp->FTPSdlci = dlci;			
		}
	else
	if(schan == RDA_OPPS_Server_Channel)
		{
			rdabt_oppsp->OPPStid = tids;
			memset(rdabt_oppsp->opps_obj_name, 0, 80);
		   	status = MGR_GetRemoteName( address,  (char*)rdabt_oppsp->opps_obj_name, 80,  &NAME_obex, rdabt_goep_authorize_ind_opps);
			if( status == 0)
			{
				rdabt_goep_authorize_ind_opps(address, 0);	
			}
                     rdabt_oppsp->OPPSdlci = dlci;

		}
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_authorize_res
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_authorize_res(goep_authorize_res_struct *msg_p)
{
	kal_prompt_trace(0, "rdabt_goep_authorize_res... msg_p ->goep_conn_id =%d,msg_p ->rsp_code =%d,rdabt_oppsp->goep_tid_OPPS =%d,rdabt_ftpsp->goep_tid_FTPS=%d ",msg_p ->goep_conn_id,msg_p ->rsp_code,rdabt_oppsp->goep_tid_OPPS,rdabt_ftpsp->goep_tid_FTPS);	
	if(msg_p ->goep_conn_id == rdabt_oppsp->goep_tid_OPPS )
	{
		if(msg_p ->rsp_code == GOEP_STATUS_SUCCESS )
		{
			RSE_SrvConnectResponse(rdabt_oppsp->OPPSaddress, rdabt_oppsp->OPPSdlci, 1);
            /* for this link */
			//	MGR_SetLinkSupervisionTimeout(rdabt_oppsp->OPPSaddress,20000);
			//	MGR_SetACLCallBack(rdabt_oppsp->OPPSaddress,OBEX_ACL_oppC_callback);
		}
		else
			RSE_SrvConnectResponse(rdabt_oppsp->OPPSaddress, rdabt_oppsp->OPPSdlci, 0);
	}
	else if(msg_p ->goep_conn_id == rdabt_ftpsp->goep_tid_FTPS )
	{
		if(msg_p ->rsp_code == GOEP_STATUS_SUCCESS )
		{
			RSE_SrvConnectResponse(rdabt_ftpsp->FTPSaddress, rdabt_ftpsp->FTPSdlci, 1);
            /* for this link */
			//	MGR_SetLinkSupervisionTimeout(rdabt_ftpsp->FTPSaddress,20000);
			//	MGR_SetACLCallBack(rdabt_ftpsp->FTPSaddress,OBEX_ACL_oppC_callback);
		}
		else
			RSE_SrvConnectResponse(rdabt_ftpsp->FTPSaddress, rdabt_ftpsp->FTPSdlci, 0);
	}
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_register_server_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_register_server_req(goep_register_server_req_struct *msg_p)
{
	int status;
    goep_register_server_rsp_struct *param_ptr ;
    param_ptr = (goep_register_server_rsp_struct*) construct_local_para(sizeof(goep_register_server_rsp_struct), TD_CTRL);

	kal_prompt_trace(0, "rdabt_goep_register_server_req...  20090220-Rdabt_opp!@!!!!!! ");	

	if(msg_p ->uuid_len == 0)
	{
		rdabt_clientp->goep_tidClient = 0xff;
		rdabt_clientp->goep_tidClient_mmi = 0xff;
		rdabt_clientp->req_idClient = 0x00;
		rdabt_clientp->ClientStatus = GOEP_Client_Idle;

		rdabt_oppsp->goep_tid_OPPS = 0xffff;
		rdabt_oppsp->OPPSstatus = GOEP_Client_Idle;
		rdabt_oppsp->req_idOPPS = 0xff;
		rdabt_oppsp->OPPStid = 0xffff;
		rdabt_oppsp->OPPSfirst = 1;
		rdabt_oppsp->buf_sizeOPPS = msg_p ->buf_size;
		rdabt_oppsp->buf_ptrOPPS = msg_p ->buf_ptr;

		status = OBEX_TransportListen_RF(RDA_OPPS_Server_Channel,&(rdabt_oppsp->goep_tid_OPPS),rdabt_oppsp->buf_sizeOPPS,rdabt_oppsp->buf_ptrOPPS,0x00,rdabt_BM_LINK_CONNECT_ACCEPT_IND,rdabt_goep_authorize_ind);
		OBEX_RegisterInbox(rdabt_oppsp->goep_tid_OPPS,OPPServiceHandler,0x00);	
		//rdabt_oppsp->goep_tid_OPPS = 1;
	    param_ptr->ref_count = 1;
	    param_ptr->msg_len = 0x18;
	    param_ptr->uuid[0] = 0;
	    param_ptr->uuid_len = 0;
	    param_ptr->req_id = msg_p ->req_id ;
		param_ptr->rsp_code = 0 ;
	    param_ptr->goep_conn_id = rdabt_oppsp->goep_tid_OPPS;
		rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_REGISTER_SERVER_RSP, (void *)param_ptr, NULL);
	}
	else
	{
		int lcv = 0;

		rdabt_ftpsp->goep_tid_FTPS = 0xffff;
		rdabt_ftpsp->req_idFTPS = 0xff;
		rdabt_ftpsp->FTPSstatus = GOEP_Client_Idle;
		rdabt_ftpsp->FTPStid = 0xffff;
		rdabt_ftpsp->FTPSfirst = 1;
		rdabt_ftpsp->buf_sizeFTPS = msg_p ->buf_size;
		rdabt_ftpsp->buf_ptrFTPS = msg_p ->buf_ptr;

		status = OBEX_TransportListen_RF(RDA_FTPS_Server_Channel,&(rdabt_ftpsp->goep_tid_FTPS),rdabt_ftpsp->buf_sizeFTPS,rdabt_ftpsp->buf_ptrFTPS,0x00,rdabt_BM_LINK_CONNECT_ACCEPT_IND,rdabt_goep_authorize_ind);
		OBEX_RegisterService(rdabt_ftpsp->goep_tid_FTPS,FTPS_worker_handler, msg_p ->uuid_len, (u_int8*)msg_p ->uuid,0x00);
		//rdabt_ftpsp->goep_tid_FTPS = 2;
	    param_ptr->ref_count = 1;
	    param_ptr->msg_len = 0x18;
	    param_ptr->uuid_len = msg_p ->uuid_len;
		for(lcv = 0;lcv < param_ptr->uuid_len;lcv++)
		{
			param_ptr->uuid[lcv] = msg_p->uuid[lcv];
		}
	    param_ptr->req_id = msg_p ->req_id ;
		param_ptr->rsp_code = 0 ;
	    param_ptr->goep_conn_id = rdabt_ftpsp->goep_tid_FTPS;
		rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_REGISTER_SERVER_RSP, (void *)param_ptr, NULL);
	}

    return;
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_bt_sdpdb_register_req_reply
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_bt_sdpdb_register_req_reply(kal_uint32 uuid, u_int8* data, u_int16 length)
{
    bt_sdpdb_register_cnf_struct *param_ptr ;
    param_ptr = (bt_sdpdb_register_cnf_struct*) construct_local_para(sizeof(bt_sdpdb_register_cnf_struct), TD_CTRL);
	kal_prompt_trace(0, "rdabt_bt_sdpdb_register_req_reply...  20090220-Rdabt_opp!@!!!!!!,uuid = %d",uuid);

    param_ptr->ref_count = 1;
    param_ptr->msg_len = 0xc;
    param_ptr->result = 0;
    param_ptr-> uuid = uuid;

	rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_SDPDB_REGISTER_CNF, (void *)param_ptr, NULL);
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_bt_sdpdb_register_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_bt_sdpdb_register_req(bt_sdpdb_register_req_struct *msg_p)
{
	
	kal_prompt_trace(0, "rdabt_bt_sdpdb_register_req...  20090220-Rdabt_opp!@!!!!!! ,msg_p ->uuid  =%x",msg_p ->uuid);	

#if 0
	if(msg_p ->uuid == 0x1106)
		OBEX_RegisterFTPServer(RDA_FTPS_Server_Channel);
	else if(msg_p ->uuid == 0x1105)
		OBEX_RegisterObjectPushServer(RDA_OPPS_Server_Channel,(u_int8)types);
#endif    
	    rdabt_bt_sdpdb_register_req_reply(msg_p ->uuid, NULL, 0);
    return;
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_connect_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_connect_ind_ftps(t_bdaddr address, int status)
{
	goep_connect_ind_struct *param_ptr;
	
	kal_prompt_trace(0,"rdabt_goep_connect_ind");
       param_ptr = (goep_connect_ind_struct*)construct_local_para((kal_uint16)sizeof(goep_connect_ind_struct), TD_CTRL);

       param_ptr->ref_count = 1;
       param_ptr->msg_len = 0x78;
	   {
		memcpy((char*)param_ptr->uuid , (const char*)(OBEX_FTP_UUID) , 16);
              param_ptr->uuid_len = 0x10;
	       param_ptr->goep_conn_id = rdabt_ftpsp->goep_tid_FTPS ;
	       param_ptr->peer_mru = rdabt_ftpsp->FTPSmax_len - 20;
	   }
	param_ptr->bd_addr.lap= BDADDR_Get_LAP(&address);
	param_ptr->bd_addr.uap = BDADDR_Get_UAP(&address);
	param_ptr->bd_addr.nap = BDADDR_Get_NAP(&address);
	memset((char*)param_ptr->dev_name , 0 , 80);	
	if( 0 == status )
	{
		memcpy((char*)param_ptr->dev_name , (const char*)(rdabt_ftpsp->ftps_obj_name) , 80);	
	}
       param_ptr->cm_conn_id = 1;
	rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_CONNECT_IND, (void *)param_ptr, NULL);	
	memset(rdabt_ftpsp->ftps_obj_name, 0, 80);
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_GetRemoteName_cb
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_connect_ind_opps(t_bdaddr address, int status)
{
	goep_connect_ind_struct *param_ptr;
	
	kal_prompt_trace(0,"rdabt_goep_connect_ind");
	
    param_ptr = (goep_connect_ind_struct*)construct_local_para((kal_uint16)sizeof(goep_connect_ind_struct), TD_CTRL);

    param_ptr->ref_count = 1;
    param_ptr->msg_len = 0x78;
 	{
 	    param_ptr->uuid[0] = 0;
 	    param_ptr->uuid_len = 0;
 	    param_ptr->goep_conn_id = rdabt_oppsp->goep_tid_OPPS ;
 	    param_ptr->peer_mru = rdabt_oppsp->OPPSmax_len;
 	}
	param_ptr->bd_addr.lap= BDADDR_Get_LAP(&address);
	param_ptr->bd_addr.uap = BDADDR_Get_UAP(&address);
	param_ptr->bd_addr.nap = BDADDR_Get_NAP(&address);
	memset((char*)param_ptr->dev_name , 0 , 80);	
	if( 0 == status )
	{
		memcpy((char*)param_ptr->dev_name , (const char*)(rdabt_oppsp->opps_obj_name) , 80);	
	}
	memset(rdabt_oppsp->opps_obj_name, 0, 80);

    param_ptr->cm_conn_id = 0;
	rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_CONNECT_IND, (void *)param_ptr, NULL);	
	
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_connect_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_connect_ind(t_bdaddr address,u_int16 ftps)
{
   int status = 0;
	
	kal_prompt_trace(0,"rdabt_goep_connect_ind");

   if(ftps == 1)
   {
		memset(rdabt_ftpsp->ftps_obj_name, 0, 80);
	   	status = MGR_GetRemoteName( address,  (char*)rdabt_ftpsp->ftps_obj_name, 80,  &NAME_obex, rdabt_goep_connect_ind_ftps);
		if( status == 0)
		{
			rdabt_goep_connect_ind_ftps(address, 0);	
		}

   }else
   {
		memset(rdabt_oppsp->opps_obj_name, 0, 80);   
	   	status = MGR_GetRemoteName( address,  (char*)rdabt_oppsp->opps_obj_name, 80,  &NAME_obex, rdabt_goep_connect_ind_opps);
		if( status == 0)
		{
			rdabt_goep_connect_ind_opps(address, 0);	
		}
   }
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_connect_res
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_connect_res(goep_connect_res_struct *msg_p)
{
	t_DataBuf* buf;

	kal_prompt_trace(0,"rdabt_goep_connect_res msg_p ->rsp_code =%d\n",msg_p ->rsp_code);

	OBEX_GetWriteBuffer(100,&buf);

	if(msg_p ->goep_conn_id == rdabt_oppsp->goep_tid_OPPS )
	{
		if(msg_p ->rsp_code == GOEP_STATUS_SUCCESS )
		{
			rdabt_oppsp->OPPSstatus = GOEP_Client_Connect;
			OBEX_ConnectResponse(rdabt_oppsp->OPPStid,OBEX_OK,0x10,0x00,rdabt_oppsp->OPPSmax_len,rdabt_oppsp->OPPSoffsetVar,(t_DataBuf*)buf);			
		}
		else
			OBEX_ConnectResponse(rdabt_oppsp->OPPStid,(msg_p ->rsp_code + 0x80),0x10,0x00,rdabt_oppsp->OPPSmax_len,rdabt_oppsp->OPPSoffsetVar,(t_DataBuf*)buf);			
	}
	else if(msg_p ->goep_conn_id == rdabt_ftpsp->goep_tid_FTPS )
	{
		if(msg_p ->rsp_code == GOEP_STATUS_SUCCESS )
		{
			rdabt_ftpsp->FTPSstatus = GOEP_Client_Connect;
			OBEX_Response(rdabt_ftpsp->FTPStid,OBEX_OK,(rdabt_ftpsp->FTPS_offset_len),FTPSbuffer);
		}
		else
			OBEX_ConnectResponse(rdabt_ftpsp->FTPStid, (msg_p ->rsp_code + 0x80) ,0x10,0x00,rdabt_ftpsp->FTPSmax_len,rdabt_ftpsp->FTPSoffsetVar,(t_DataBuf*)buf);			
	}
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_connect_res
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_push_res(goep_push_res_struct *msg_p)
{
	t_DataBuf* buffer;

	kal_prompt_trace(0,"rdabt_goep_push_res msg_p ->rsp_code =%d,rdabt_oppsp->OPPSstatus =%d\n",msg_p ->rsp_code,rdabt_oppsp->OPPSstatus);

	if(msg_p ->goep_conn_id == rdabt_oppsp->goep_tid_OPPS || msg_p ->goep_conn_id == rdabt_ftpsp->goep_tid_FTPS)
	{
		OBEX_GetWriteBuffer(0,&buffer);
	}

	if(msg_p ->goep_conn_id == rdabt_oppsp->goep_tid_OPPS )
	{ 
		if(rdabt_oppsp->OPPSstatus == GOEP_Client_Disconnect)
		{
			return;
		}
		if(msg_p ->rsp_code == GOEP_STATUS_SUCCESS )
		{
			if(rdabt_oppsp->OPPSfirst==0)
				OBEX_Response(rdabt_oppsp->OPPStid,OBEX_CONTINUE,0,buffer);
			else
				OBEX_Response(rdabt_oppsp->OPPStid,OBEX_OK,0,buffer);
		}
		else
		{
			OBEX_Response(rdabt_oppsp->OPPStid,(msg_p ->rsp_code+0x80),0,buffer);
//			rdabt_oppsp->OPPSfirst = 1;
//			rdabt_goep_disconnect_ind(0);		
		     if(!opps_timer)
			    opps_timer = pTimerCreate(60,(rdabt_timer_func)rdabt_goep_opps_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend	
	        	kal_prompt_trace(0, "he want disconnect opps 3.. \n");
		}
	}
	else if(msg_p ->goep_conn_id == rdabt_ftpsp->goep_tid_FTPS )
	{
		if(rdabt_ftpsp->FTPSstatus == GOEP_Client_Disconnect)
		{
			return;
		}
		if(msg_p ->rsp_code == GOEP_STATUS_SUCCESS )
		{
			if(rdabt_ftpsp->FTPSfirst==0)
				OBEX_Response(rdabt_ftpsp->FTPStid,OBEX_CONTINUE,0,buffer);
			else
				OBEX_Response(rdabt_ftpsp->FTPStid,OBEX_OK,0,buffer);
		}
		else
		{
			OBEX_Response(rdabt_ftpsp->FTPStid,(msg_p ->rsp_code+0x80),0,buffer);
			rdabt_ftpsp->FTPSfirst = 1;
		}
	}else
		kal_prompt_trace(0,"rdabt_goep_push_res may be error!");


}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_disconnect_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_disconnect_ind(u_int16 ftps)
{
	goep_disconnect_ind_struct *param_ptr;
    bt_bm_link_state_ind_struct *param_ptr2 ;

	kal_prompt_trace(0,"rdabt_goep_disconnect_ind");
	
    param_ptr = (goep_disconnect_ind_struct*)construct_local_para((kal_uint16)sizeof(goep_disconnect_ind_struct), TD_CTRL);

    param_ptr->ref_count = 1;
    param_ptr->msg_len = 0x18;
    param_ptr->req_id = 0;
	if(ftps == 1)
	{
		memcpy((char*)param_ptr->uuid , (const char*)(OBEX_FTP_UUID) , 16);
		param_ptr->uuid_len = 0x10;
		param_ptr->goep_conn_id = rdabt_ftpsp->goep_tid_FTPS ;
		rdabt_ftpsp->FTPSstatus = GOEP_Client_Disconnect;
	}
    else
    {
		param_ptr->uuid[0] = 0;
		param_ptr->uuid_len = 0;
		param_ptr->goep_conn_id = rdabt_oppsp->goep_tid_OPPS ;
		rdabt_oppsp->OPPSstatus = GOEP_Client_Disconnect;
   	}

	rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_DISCONNECT_IND, (void *)param_ptr, NULL);	

	param_ptr2= (bt_bm_link_state_ind_struct*) construct_local_para(sizeof(bt_bm_link_state_ind_struct), TD_CTRL);
	param_ptr2->ref_count  = 1;
	param_ptr2->msg_len    = 6;
	param_ptr2->max_number = 3;
	param_ptr2->current_number = 0 ;//0x2;
	rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_STATE_IND, (void *)param_ptr2, NULL);
		
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_abort_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_abort_ind(u_int16 ftps)
{
	goep_abort_ind_struct *param_ptr;

	kal_prompt_trace(0,"rdabt_goep_abort_ind");
	
	param_ptr = (goep_abort_ind_struct*)construct_local_para((kal_uint16)sizeof(goep_abort_ind_struct), TD_CTRL);

	param_ptr->ref_count = 1;
	param_ptr->msg_len = 0x16;
	if(ftps)
	{
	   memcpy((char*)param_ptr->uuid , (const char*)(OBEX_FTP_UUID) , 16);
	   param_ptr->uuid_len = 0x10;
	   param_ptr->goep_conn_id = rdabt_ftpsp->goep_tid_FTPS ;
	}
	else
	{
	   param_ptr->uuid[0] = 0;
	   param_ptr->uuid_len = 0;
	   param_ptr->goep_conn_id = rdabt_oppsp->goep_tid_OPPS ;
	}

	rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_ABORT_IND, (void *)param_ptr, NULL);	
		
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_abort_res
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_abort_res(goep_abort_res_struct *msg_p)
{
	t_DataBuf* buffer;

	kal_prompt_trace(0,"rdabt_goep_abort_res,rsp_code=%x",msg_p ->rsp_code);

	if(msg_p ->goep_conn_id == rdabt_oppsp->goep_tid_OPPS || msg_p ->goep_conn_id == rdabt_ftpsp->goep_tid_FTPS)
	{
		OBEX_GetWriteBuffer(0,&buffer);
	}

	if(msg_p ->goep_conn_id == rdabt_oppsp->goep_tid_OPPS )
	{
		rdabt_oppsp->OPPSfirst = 1;		
		if(rdabt_oppsp->OPPSstatus != GOEP_Client_Connect)
			return;

		if(msg_p ->rsp_code == GOEP_STATUS_SUCCESS )
		{
			OBEX_Response(rdabt_oppsp->OPPStid,OBEX_OK,0,buffer);
		}
		else
		{
			OBEX_Response(rdabt_oppsp->OPPStid,(msg_p ->rsp_code+0x80),0,buffer);
		}
	}
	else
	if(msg_p ->goep_conn_id == rdabt_ftpsp->goep_tid_FTPS )
	{
		rdabt_ftpsp->FTPSfirst = 1;
		if(rdabt_ftpsp->FTPSstatus != GOEP_Client_Connect)
			return;
		
		if(msg_p ->rsp_code == GOEP_STATUS_SUCCESS )
		{
			OBEX_Response(rdabt_ftpsp->FTPStid,OBEX_OK,0,buffer);
		}
		else
		{
			OBEX_Response(rdabt_ftpsp->FTPStid,(msg_p ->rsp_code+0x80),0,buffer);
		}
	}
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_sdpdb_deregister_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_sdpdb_deregister_req(bt_sdpdb_deregister_req_struct *msg_p)
{

	if(msg_p->uuid == 0x1105)
	{
	/*
		goep_deregister_server_rsp_struct *param_ptr ;

		param_ptr = (goep_deregister_server_rsp_struct*) construct_local_para(sizeof(goep_deregister_server_rsp_struct), TD_CTRL);

		param_ptr->ref_count = 1;
		param_ptr->msg_len = 0x18;
		param_ptr->goep_conn_id = rdabt_oppsp->goep_tid_OPPS;
		param_ptr->rsp_code = 0;
		param_ptr->uuid[0] = 0;
		param_ptr->uuid_len = 0;

		//	kal_prompt_trace(0,"rdabt_goep_deregister_req OPPS");
*/
		bt_sdpdb_deregister_cnf_struct *param_ptr ;
		param_ptr = (bt_sdpdb_deregister_cnf_struct*) construct_local_para(sizeof(bt_sdpdb_deregister_cnf_struct), TD_CTRL);
		param_ptr->ref_count = 1;
		param_ptr->msg_len = 0xc;
		param_ptr->result = 0;
		param_ptr->uuid = msg_p->uuid;
		rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_SDPDB_DEREGISTER_CNF, (void *)param_ptr, NULL);
		RSE_SrvDeregisterPort(RDA_OPPS_Server_Channel);
		//rdabt_goep_init();
	}
	else
	{
		bt_sdpdb_deregister_cnf_struct *param_ptr ;

		//	kal_prompt_trace(0,"rdabt_sdpdb_deregister_req FTPS");

		param_ptr = (bt_sdpdb_deregister_cnf_struct*) construct_local_para(sizeof(bt_sdpdb_deregister_cnf_struct), TD_CTRL);

		param_ptr->ref_count = 1;
		param_ptr->msg_len = 0xc;
		param_ptr->result = 0;
		param_ptr->uuid = msg_p->uuid;
		rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_SDPDB_DEREGISTER_CNF, (void *)param_ptr, NULL);
		RSE_SrvDeregisterPort(RDA_FTPS_Server_Channel);

		if(rdabt_ftpsp->FTPSstatus == GOEP_Client_Connect)
		{
 			rdabt_goep_disconnect_rsp_ftps(0, NULL, 0);
		}

	}
	
}


/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_deregister_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_deregister_req(goep_deregister_server_req_struct *msg_p)
{
	goep_deregister_server_rsp_struct *param_ptr ;
	param_ptr = (goep_deregister_server_rsp_struct*) construct_local_para(sizeof(goep_deregister_server_rsp_struct), TD_CTRL);
//	mmi_trace(1,"msg_p->goep_conn_id = %d,rdabt_oppsp->goep_tid_OPPS =%d,rdabt_ftpsp->goep_tid_FTPS =%d\n",msg_p->goep_conn_id,rdabt_oppsp->goep_tid_OPPS,rdabt_ftpsp->goep_tid_FTPS );
	param_ptr->ref_count = 1;
	param_ptr->msg_len = 0x18;
	param_ptr->goep_conn_id = msg_p->goep_conn_id;
	param_ptr->rsp_code = 0;
	
	if( rdabt_oppsp->goep_tid_OPPS == msg_p->goep_conn_id)
	{
		param_ptr->uuid[0] = 0;
		param_ptr->uuid_len = 0;
	}
		
	if( rdabt_ftpsp->goep_tid_FTPS == msg_p->goep_conn_id)
	{
		int lcv = 0;
		for(lcv = 0;lcv < 16;lcv++)
		{
		param_ptr->uuid[lcv] = OBEX_FTP_UUID[lcv];
		}
		param_ptr->uuid_len = 0x10;
		rdabt_goep_init();
	}
	rdabt_send_msg_up(MOD_MMI, MSG_ID_GOEP_DEREGISTER_SERVER_RSP, (void *)param_ptr, NULL);
	
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_connect_res
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_notify_evm_ind(void)
{
   ilm_struct *msg = allocate_ilm(MOD_BT);

	kal_prompt_trace(0,"rdabt_notify_evm_ind");

   msg->src_mod_id = MOD_BT;
   msg->dest_mod_id = (module_type)MOD_BT;
   msg->sap_id = INVALID_SAP;
   msg->msg_id = MSG_ID_BT_NOTIFY_EVM_IND;
   msg->local_para_ptr = NULL;
   msg->peer_buff_ptr = NULL;
   msg_send_ext_queue(msg);
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_connect_res
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_pull_res(goep_pull_res_struct *msg_p)
{
		t_DataBuf* buf;
		u_int16 offset_len;

		kal_prompt_trace(0,"rdabt_goep_pull_res,rsp_code=%x",msg_p ->rsp_code);

		if(msg_p ->goep_conn_id != rdabt_ftpsp->goep_tid_FTPS )
			return;
		if(rdabt_ftpsp->FTPSstatus == GOEP_Client_Disconnect)
		{
			return;
		}

		if(msg_p ->rsp_code != GOEP_STATUS_SUCCESS )
		{
			OBEX_GetWriteBuffer(0,&buf);
			OBEX_Response(rdabt_ftpsp->FTPStid,(msg_p ->rsp_code + 0x80),0,buf);
			return;
		}


		kal_prompt_trace(0,"rdabt_goep_pull_res FTPS,total_obj_len=%x",msg_p ->total_obj_len);
		offset_len = 3;
		OBEX_GetWriteBuffer(msg_p ->frag_len,&buf);
		if(msg_p ->total_obj_len)
		OBEX_Create4ByteHeader(OBEX_LENGTH,msg_p ->total_obj_len,buf,&offset_len);
		if(msg_p->pkt_type == GOEP_FINAL_PKT ||msg_p->pkt_type == GOEP_SINGLE_PKT)
		{
			kal_prompt_trace(0,"rdabt_goep_pull_res GOEP_FINAL_PKT");
			if((msg_p ->frag_len) && (msg_p ->frag_ptr))
			OBEX_CreateSequenceHeader(OBEX_END_BODY,msg_p ->frag_len,msg_p ->frag_ptr,buf,&offset_len);
			OBEX_Response(rdabt_ftpsp->FTPStid,OBEX_OK,offset_len,buf);
		}
		else
		{
			kal_prompt_trace(0,"rdabt_goep_pull_res !GOEP_FINAL_PKT");
			if((msg_p ->frag_len) && (msg_p ->frag_ptr))
			OBEX_CreateSequenceHeader(OBEX_BODY,msg_p ->frag_len,msg_p ->frag_ptr,buf,&offset_len);
			OBEX_Response(rdabt_ftpsp->FTPStid,OBEX_CONTINUE,offset_len,buf);			
		}
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_goep_set_floder_res
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_goep_set_floder_res(goep_set_folder_res_struct *msg_p)
{
	t_DataBuf* buf;

	kal_prompt_trace(0,"rdabt_goep_set_floder_res,rsp_code=%x",msg_p ->rsp_code);
	if(msg_p ->goep_conn_id == rdabt_ftpsp->goep_tid_FTPS )
	{
		OBEX_GetWriteBuffer(0,&buf);
		if(msg_p ->rsp_code == GOEP_STATUS_SUCCESS )
		{
			OBEX_Response(rdabt_ftpsp->FTPStid,OBEX_OK,0,buf);
		}
		else
			OBEX_Response(rdabt_ftpsp->FTPStid,(msg_p ->rsp_code + 0x80),0,buf);
	}
}

/*****************************************************************************
 * FUNCTION
 *  rdabt_opp_msg_hdler
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void rdabt_opp_msg_hdler(ilm_struct *ext_message)
{
    switch(ext_message->msg_id)
    {
	case MSG_ID_GOEP_CONNECT_REQ:
	{
	        goep_connect_req_struct *msg_p = (goep_connect_req_struct *)ext_message->local_para_ptr;
            	 t_bdaddr tempbd;
		BDADDR_Set_LAP(&tempbd,msg_p ->bd_addr.lap);
		BDADDR_Set_UAP(&tempbd,msg_p ->bd_addr.uap);
		BDADDR_Set_NAP(&tempbd,msg_p ->bd_addr.nap);	
		rdabt_goep_connect_req(tempbd,msg_p);///PIN CODE Negative
		break;
	}
    
	case MSG_ID_GOEP_PUSH_REQ:
	{
	        goep_push_req_struct *msg_p = (goep_push_req_struct *)ext_message->local_para_ptr;
		 rdabt_goep_push_req(msg_p);
		
		break;
	}

	case MSG_ID_GOEP_PULL_REQ:
	{
		goep_pull_req_struct *msg_p = (goep_pull_req_struct *)ext_message->local_para_ptr;
		rdabt_goep_pull_req(msg_p);
		break;
	}

	case MSG_ID_GOEP_SET_FOLDER_REQ:
	{
		goep_set_folder_req_struct *msg_p = (goep_set_folder_req_struct *)ext_message->local_para_ptr;
		rdabt_goep_set_folder_req(msg_p);
		break;
	}
	
	case MSG_ID_GOEP_DISCONNECT_REQ:
	case MSG_ID_GOEP_TPDISCONNECT_REQ:
	{
	        goep_disconnect_req_struct *msg_p = (goep_disconnect_req_struct *)ext_message->local_para_ptr;
		 rdabt_goep_disconnect_req(msg_p);
		
		break;
	}
    
	case MSG_ID_GOEP_ABORT_REQ:
	{
	        goep_abort_req_struct *msg_p = (goep_abort_req_struct *)ext_message->local_para_ptr;
		 rdabt_goep_abort_req(msg_p);

		break;
	}
    
	case MSG_ID_GOEP_REGISTER_SERVER_REQ:
	{
	        goep_register_server_req_struct *msg_p = (goep_register_server_req_struct *)ext_message->local_para_ptr;
		 rdabt_goep_register_server_req(msg_p);

		break;
	}

#if 0    
	case MSG_ID_BT_SDPDB_REGISTER_REQ:
	{
	        bt_sdpdb_register_req_struct *msg_p = (bt_sdpdb_register_req_struct *)ext_message->local_para_ptr;
		 rdabt_bt_sdpdb_register_req(msg_p);

		break;
	}
#endif

	case MSG_ID_GOEP_AUTHORIZE_RES:
	{
	        goep_authorize_res_struct *msg_p = (goep_authorize_res_struct *)ext_message->local_para_ptr;
		 rdabt_goep_authorize_res(msg_p);

		break;
	}
    
	case MSG_ID_GOEP_CONNECT_RES:
	{
	        goep_connect_res_struct *msg_p = (goep_connect_res_struct *)ext_message->local_para_ptr;
		 rdabt_goep_connect_res(msg_p);

		break;
	}
    
	case MSG_ID_GOEP_PUSH_RES:
	{
	        goep_push_res_struct *msg_p = (goep_push_res_struct *)ext_message->local_para_ptr;
		 rdabt_goep_push_res(msg_p);

		break;
	}

	case MSG_ID_GOEP_ABORT_RES:
	{
	        goep_abort_res_struct *msg_p = (goep_abort_res_struct *)ext_message->local_para_ptr;
		 rdabt_goep_abort_res(msg_p);

		break;
	}
    
	case MSG_ID_GOEP_PULL_RES:
	{
	        goep_pull_res_struct *msg_p = (goep_pull_res_struct *)ext_message->local_para_ptr;
		 rdabt_goep_pull_res(msg_p);

		break;
	}
    
	case MSG_ID_GOEP_SET_FOLDER_RES:
	{
	        goep_set_folder_res_struct *msg_p = (goep_set_folder_res_struct *)ext_message->local_para_ptr;
		 rdabt_goep_set_floder_res(msg_p);

		break;
	}
    
#if 0
	case MSG_ID_BT_SDPDB_DEREGISTER_REQ:
	{
	        bt_sdpdb_deregister_req_struct *msg_p = (bt_sdpdb_deregister_req_struct *)ext_message->local_para_ptr;
		kal_prompt_trace(0,"MSG_ID_BT_SDPDB_DEREGISTER_REQ");
		 rdabt_sdpdb_deregister_req(msg_p);

		break;
	}
#endif
    
	case MSG_ID_GOEP_DEREGISTER_SERVER_REQ:
	{
	        goep_deregister_server_req_struct *msg_p = (goep_deregister_server_req_struct *)ext_message->local_para_ptr;
		kal_prompt_trace(0,"MSG_ID_GOEP_DEREGISTER_SERVER_REQ");
		 rdabt_goep_deregister_req(msg_p);

		break;
	}
	default :
		break;
    
    }
}
	
#endif /* _RDABT_OPP_C */ 
