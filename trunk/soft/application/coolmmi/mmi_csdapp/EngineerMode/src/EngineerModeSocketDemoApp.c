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
 * EngineerModeSocketDemoApp.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for engineer mode socket demo application
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
 
 
#include "mmi_features.h" 
#include "kal_non_specific_general_types.h"
//CSD End

#if 0//JIASHUO DEL FOR WAP/MMS


/**********************/
/*****  Header Files *****/
/**********************/

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#include "custdatares.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "custmenures.h"
#include "wgui_categories.h"
#include "hardwareevents.h"
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"

#include "timerevents.h"
#include "mainmenudef.h"
#include "engineermodedef.h"
#include "engineermodetype.h"
#include "engineermodeprot.h"
#include "mmi_data_types.h"
#include "customcfg.h"
#include "unicodexdcl.h"
#include "audioinc.h"
#include "idleappdef.h"
#include "bootup.h"
//#include "profiles_prot.h"
#include "globaldefs.h"


#include "app2soc_struct.h"
#include "engineermodesocketappping.h"




soc_demo_app_transaction_struct* soc_demo_transaction=NULL;


/* Engineer Mode Socket Demo Application core functions */
int mmi_soc_demo_app_request(mmi_soc_demo_app_enum app_id, int account_id,
			char server_ip_address[4],
			char* url, int url_len, 
			char* echo_text, int echo_text_len, mmi_soc_demo_app_rsp_t callback);

int mmi_soc_demo_app_request_abort(void);
void en_soc_output_result(int ret,char *out_str, int len);


/* HTTP apllication functions */
int en_soc_demo_http_send_request(void);
void en_soc_demo_tcp_app_recv_response(void);

/* DAYTIME and ECHO apllication functions */
int en_soc_demo_udp_app_send_request(void);
void en_soc_demo_udp_app_recv_response(void);

/* DNS apllication functions */
void en_soc_demo_get_host_by_name(mmi_soc_demo_app_enum app_id, const kal_uint8 *hostname);
void em_soc_demo_app_soc_get_host_by_name_ind(void* inMsg);

/* Socket handle functions*/
kal_bool en_soc_demo_app_create_socket(void);
void em_soc_demo_app_socket_notify(void* inMsg);

/* timer event handler*/
void em_soc_demo_app_start_timer(void);
void em_soc_demo_app_stop_timer(void);
void em_soc_demo_app_timer_handler(void);

/* misc function */
soc_demo_app_http_url_struct* en_soc_demo_http_get_parsed_url(const kal_int8* in_str, int in_str_len);

/* external functions*/
extern void StartTimer(U16 timerid, U32 delay, FuncPtr funcPtr);
extern void StopTimer(U16 timerid);


/****************************************************************
* FUNCTION                                                                                                                       
*	mmi_soc_demo_app_request
* DESCRIPTION                                                           
*   MMI calls this function to send request to server in internet.
*   This function will start a request and response transaction.
*   When a transaction	already exists, the function will return Busy.
*
* PARAMETERS
*	mmi_soc_demo_app app_id,
* 	int account_id
*	char server_ip_address[4],
*	char* url, int url_len, 
*	char* char *echo_text, int echo_text_len
*	mmi_soc_demo_app_rsp_t callback
*
*
*	Note that following items.
*	1. url and echo_text muset be rstricted to us-ascii characters.
*	2. MMI has not to free *url and *exco_text. 
*	   They will be freed when the request-response transaction ends.	
*
*	If app_id is HTTP,please specify *url and url_len.
*	If app_id is DNS, please specify *url and url_len.
*	If app_id is DAYTIME, please specify server_ip_address[4].
*	If app_id is ECHO,please specify server_ip_address[4], *echo_text and echo_text_len.
*
* RETURNS
* 	 0, EN_SOC_SUCCESS
*	-1, EN_SOC_NO_MEMORY
*	-2, EN_SOC_BUSY
*	-3, EN_SOC_UNKNOWN_APP
*	-4, EN_SOC_BEARER_FAIL
*	-5, EN_SOC_CNT_RESET_BY_PEER
*	-6, EN_SOC_PEER_NO_RESPONSE
*	-7, EN_SOC_PEER_NOT_REACHABLE
*	-8, EN_SOC_ERROR
*	-9, EN_SOC_INVAL_PARA
*
* GLOBALS AFFECTED
*   After MMI calls this function, one socket resource will be created 
*	and one request is sent to Server.
****************************************************************/
int mmi_soc_demo_app_request(mmi_soc_demo_app_enum app_id, int account_id,
			char server_ip_address[4],
			char* url, int url_len, 
			char* echo_text, int echo_text_len, mmi_soc_demo_app_rsp_t callback)
{
	if(soc_demo_transaction)
	{
#ifdef MMI_ON_HARDWARE_P
		kal_print("Transaction existed!!");
#endif
		return EN_SOC_BUSY;
	}
	else
	{
		if((soc_demo_transaction=OslMalloc(sizeof(soc_demo_app_transaction_struct)))==NULL)
		{
#ifdef MMI_ON_HARDWARE_P
			kal_print("No memory");
#endif
			en_soc_output_result(EN_SOC_NO_MEMORY, NULL, 0);
			return EN_SOC_NO_MEMORY;
		}
		else
		{
			memset(soc_demo_transaction,0,sizeof(soc_demo_app_transaction_struct));
			if((soc_demo_transaction->rcvd_buffer=OslMalloc(MAX_RCV_BUFFER_SIZE))==NULL)
			{
#ifdef MMI_ON_HARDWARE_P
				kal_print("No memory");
#endif
				en_soc_output_result(EN_SOC_NO_MEMORY, NULL, 0);
				return EN_SOC_NO_MEMORY;
			}
			else
			{
				if((soc_demo_transaction->snd_buffer=OslMalloc(MAX_SND_BUFFER_SIZE))==NULL)
				{
#ifdef MMI_ON_HARDWARE_P
					kal_print("No memory");
#endif
					en_soc_output_result(EN_SOC_NO_MEMORY, NULL, 0);
					return EN_SOC_NO_MEMORY;
				}
				else
				{
					/* Set  initial values to soc_demo_transaction */
					memset((kal_int8*)soc_demo_transaction->rcvd_buffer,0, MAX_RCV_BUFFER_SIZE);
					memset((kal_int8*)soc_demo_transaction->snd_buffer,0,MAX_SND_BUFFER_SIZE);
					
					soc_demo_transaction->soc_demo_app_id=app_id;
					soc_demo_transaction->nwt_acount_id=account_id;
					if(server_ip_address)
					{
						memcpy(soc_demo_transaction->server_ip_addr.addr,server_ip_address,4);
						soc_demo_transaction->server_ip_addr.addr_len=4;
					}
					soc_demo_transaction->url=(kal_int8*)url;
					soc_demo_transaction->url_len=url_len;

					soc_demo_transaction->callback=callback;
					if(echo_text)
					{
						if(echo_text_len>MAX_SND_BUFFER_SIZE)
						{
							memcpy(soc_demo_transaction->snd_buffer,echo_text,MAX_SND_BUFFER_SIZE);
							soc_demo_transaction->snd_data_len=MAX_SND_BUFFER_SIZE;
						}
						else
						{
							memcpy(soc_demo_transaction->snd_buffer,echo_text,echo_text_len);
							soc_demo_transaction->snd_data_len=echo_text_len;
						}

						OslMfree(echo_text);
					}
					if(en_soc_demo_app_create_socket()==KAL_FALSE)
					{
						en_soc_output_result(EN_SOC_NO_MEMORY, NULL, 0);
						return EN_SOC_NO_MEMORY;
					}
					
					switch(app_id)
					{
						case HTTP:
						{
							soc_demo_transaction->state=HTTP_DNS_QUERY;
							return en_soc_demo_http_send_request();
						}
						case DNS:
						{
							(void)en_soc_demo_get_host_by_name(DNS,(kal_uint8 *)soc_demo_transaction->url);
							return EN_SOC_SUCCESS;
						}
						case DAYTIME:
						{
							soc_demo_transaction->server_ip_addr.port=SOC_DAYTIME_RESVD_PORT;
							soc_demo_transaction->snd_data_len=10; //garbage data
							return en_soc_demo_udp_app_send_request();
						}
						case ECHO:
						{
							soc_demo_transaction->server_ip_addr.port=SOC_ECHO_RESVD_PORT;
							return en_soc_demo_udp_app_send_request();
						}
						default:
						{
							en_soc_output_result(EN_SOC_UNKNOWN_APP, NULL, 0);
							return EN_SOC_UNKNOWN_APP;			
						}
					
					}
				}

			}
		}
	}
}
/****************************************************************
* FUNCTION                                                                                                                       
*	en_soc_output_result
* DESCRIPTION                                                           
*	This function is used to output the transaction result.
*	It will call user's callback function and then release all resources.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	Socket demo application transaction is completed.   
*****************************************************************/
void en_soc_output_result(int ret,char *out_str, int len)
{
	if(soc_demo_transaction)
	{
#ifdef MMI_ON_HARDWARE_P
		kal_prompt_trace(MOD_MMI,"soc demp app result: %d",ret);	
#endif
		soc_demo_transaction->callback(soc_demo_transaction->soc_demo_app_id,ret,out_str,len);
		if(ret!=EN_SOC_SUCCESS)
			soc_demo_transaction->state=SOC_CLOSING;
		mmi_soc_demo_app_request_abort();	
	}
}
/****************************************************************
* FUNCTION                                                                                                                       
*	mmi_soc_demo_app_request_abort* DESCRIPTION                                                           
*   	MMI calls this function to abort the existing transaction.
*	This function will close socket and free all unused memory.
* PARAMETERS
*   none
* RETURNS
*   int result,
* 	 0, EN_SOC_SUCCESS
*	-1, EN_SOC_NO_MEMORY
*	-2, EN_SOC_BUSY
*	-3, EN_SOC_UNKNOWN_APP
*	-4, EN_SOC_BEARER_FAIL
*	-5, EN_SOC_CNT_RESET_BY_PEER
*	-6, EN_SOC_PEER_NO_RESPONSE
*	-7, EN_SOC_PEER_NOT_REACHABLE
*	-8, EN_SOC_ERROR
*	-9, EN_SOC_INVAL_PARA

* GLOBALS AFFECTED
*   none
****************************************************************/
int mmi_soc_demo_app_request_abort(void)
{
	if(soc_demo_transaction)
	{
		if(soc_demo_transaction->socket_id>=0)
		{
#ifdef MMI_ON_HARDWARE_P
			ilm_struct *ilm_send = NULL;
#endif
			soc_close(soc_demo_transaction->socket_id);
#ifdef MMI_ON_HARDWARE_P
			//close socket and disconnect bearer here			
			ilm_send = allocate_ilm(MOD_MMI);	
			ilm_send->msg_id = MSG_ID_APP_SOC_DEACTIVATE_REQ;
			ilm_send->peer_buff_ptr = NULL;
			ilm_send->local_para_ptr = NULL;
			SEND_ILM(MOD_MMI,MOD_SOC,SOC_APP_SAP,ilm_send);			   
#endif		
		}
		if(soc_demo_transaction->url)
		{
			OslMfree(soc_demo_transaction->url);
		}		
		//We have to free recv_buffer.
		if(soc_demo_transaction->rcvd_buffer)
		{
			OslMfree(soc_demo_transaction->rcvd_buffer);
		}
		if(soc_demo_transaction->snd_buffer)
		{
			OslMfree(soc_demo_transaction->snd_buffer);
		}
		if(soc_demo_transaction->url_parsed_info)
		{
			if(soc_demo_transaction->url_parsed_info->host_name)
			{
				OslMfree(soc_demo_transaction->url_parsed_info->host_name);
			}
			if(soc_demo_transaction->url_parsed_info->url_link)
			{
				OslMfree(soc_demo_transaction->url_parsed_info->url_link);
			}
			OslMfree(soc_demo_transaction->url_parsed_info);
		}
		em_soc_demo_app_stop_timer();
		OslMfree(soc_demo_transaction);
		soc_demo_transaction=NULL;
	}
	return 0;
}
/****************************************************************
* FUNCTION                                                                                                                       
*	en_soc_demo_http_send_request
* DESCRIPTION                                                           
*	This fucntion is used to send HTTP request to specific server.
*	And receive server's response. 
*	If SOC can't receive any data for now, SOC will send MSG_ID_APP_SOC_NOTIFY_IND to MMI.
*	Then MMI will call em_soc_demo_app_socket_notify() to receive incoming data.
*
* PARAMETERS
*	None
* RETURNS
* 	 0, EN_SOC_SUCCESS
*	-1, EN_SOC_NO_MEMORY
*	-2, EN_SOC_BUSY
*	-3, EN_SOC_UNKNOWN_APP
*	-4, EN_SOC_BEARER_FAIL
*	-5, EN_SOC_CNT_RESET_BY_PEER
*	-6, EN_SOC_PEER_NO_RESPONSE
*	-7, EN_SOC_PEER_NOT_REACHABLE
*	-8, EN_SOC_ERROR
*	-9, EN_SOC_INVAL_PARA
* GLOBALS AFFECTED
*	None   
*****************************************************************/
int en_soc_demo_http_send_request(void)
{   	
	if(soc_demo_transaction)
	{
		if(soc_demo_transaction->state==HTTP_DNS_QUERY && (soc_demo_app_http_url_struct *)soc_demo_transaction->url_parsed_info==NULL)
		{
			soc_demo_transaction->url_parsed_info=(soc_demo_app_http_url_struct *)en_soc_demo_http_get_parsed_url(soc_demo_transaction->url, soc_demo_transaction->url_len);
			if(soc_demo_transaction->url_parsed_info)
			{
				
				(void)en_soc_demo_get_host_by_name(HTTP,(kal_uint8 *)soc_demo_transaction->url_parsed_info->host_name);				
				return EN_SOC_SUCCESS;
			}
			else
			{
				en_soc_output_result(EN_SOC_INVAL_PARA,NULL,0);
				return EN_SOC_INVAL_PARA;
			}
		}
		else if(soc_demo_transaction->state==HTTP_TCP_CON_CREATING)
		{
			kal_int8 ret;

			char CRLF[3]={0x0d, 0x0a, 0x00};
			soc_demo_transaction->state=HTTP_TCP_CON_CREATED;
			/* Create HTTP Get Message */

			soc_demo_transaction->snd_data_len=0;
			strcat(soc_demo_transaction->snd_buffer,"GET ");
			soc_demo_transaction->snd_data_len+=4;

			if(soc_demo_transaction->url_parsed_info->url_link)
			{
				strcat((char*)soc_demo_transaction->snd_buffer,(char*)soc_demo_transaction->url_parsed_info->url_link);				
				soc_demo_transaction->snd_data_len+=soc_demo_transaction->url_parsed_info->url_link_len;

					strcat((char*)soc_demo_transaction->snd_buffer," ");				
				soc_demo_transaction->snd_data_len+=2;	
			}
			else
			{
				strcat((char*)soc_demo_transaction->snd_buffer,"/ ");				
				soc_demo_transaction->snd_data_len+=2;				
			}

			strcat((char*)soc_demo_transaction->snd_buffer,"HTTP/1.1");				
			soc_demo_transaction->snd_data_len+=8;				

			strcat((char*)soc_demo_transaction->snd_buffer,CRLF);
			soc_demo_transaction->snd_data_len+=2;

			strcat((char*)soc_demo_transaction->snd_buffer,"Host: ");
			soc_demo_transaction->snd_data_len+=6;

			strcat((char*)soc_demo_transaction->snd_buffer,	(char*)soc_demo_transaction->url_parsed_info->host_name);				
			soc_demo_transaction->snd_data_len+=soc_demo_transaction->url_parsed_info->host_name_len;

			strcat((char*)soc_demo_transaction->snd_buffer,CRLF);
			soc_demo_transaction->snd_data_len+=2;				


			strcat((char*)soc_demo_transaction->snd_buffer,"Connection: Closed");
			soc_demo_transaction->snd_data_len+=18;

			strcat((char*)soc_demo_transaction->snd_buffer,CRLF);
			soc_demo_transaction->snd_data_len+=2;				

			strcat((char*)soc_demo_transaction->snd_buffer,CRLF);
			soc_demo_transaction->snd_data_len+=2;								

			soc_demo_transaction->server_ip_addr.port=soc_demo_transaction->url_parsed_info->ser_port_num;

#ifdef MMI_ON_HARDWARE_P
			kal_prompt_trace(MOD_MMI,"Http connect to %d.%d,%d,%d and port: %d", soc_demo_transaction->server_ip_addr.addr[0],soc_demo_transaction->server_ip_addr.addr[1],soc_demo_transaction->server_ip_addr.addr[2],soc_demo_transaction->server_ip_addr.addr[3],soc_demo_transaction->server_ip_addr.port);	
#endif
			ret=soc_connect(soc_demo_transaction->socket_id,&soc_demo_transaction->server_ip_addr);
#ifdef MMI_ON_HARDWARE_P
			kal_prompt_trace(MOD_MMI,"Http connect result is %d",ret);	
#endif
			if(ret==SOC_SUCCESS)
			{
				en_soc_demo_http_send_request();
				return EN_SOC_SUCCESS;
			}
			else if(ret==SOC_WOULDBLOCK)
			{
				SetProtocolEventHandler( em_soc_demo_app_socket_notify, MSG_ID_APP_SOC_NOTIFY_IND);
				return EN_SOC_SUCCESS;
			}
			else
			{
				if(ret==SOC_ERROR)
				{
					en_soc_output_result(EN_SOC_PEER_NOT_REACHABLE,NULL,0);
					return EN_SOC_PEER_NOT_REACHABLE;
				}
				else
				{
					en_soc_output_result(EN_SOC_ERROR,NULL,0);
					return EN_SOC_ERROR;
				}		
			}
		}
		else if(soc_demo_transaction->state==HTTP_TCP_CON_CREATED||soc_demo_transaction->state==REQ_SEND_RETRY||soc_demo_transaction->state==REQ_SENDING)
		{
			kal_int32 ret;
			
			if(soc_demo_transaction->state!=REQ_SENDING)
			{
				soc_demo_transaction->snd_counter=0;
			}
			soc_demo_transaction->state=REQ_SENDING;
#ifdef MMI_ON_HARDWARE_P
			kal_prompt_trace(MOD_MMI,"Http send request to %d.%d,%d,%d and port: %d", soc_demo_transaction->server_ip_addr.addr[0],soc_demo_transaction->server_ip_addr.addr[1],soc_demo_transaction->server_ip_addr.addr[2],soc_demo_transaction->server_ip_addr.addr[3],soc_demo_transaction->server_ip_addr.port);	
#endif			
			ret = soc_send(soc_demo_transaction->socket_id,(kal_uint8*)(soc_demo_transaction->snd_buffer+soc_demo_transaction->snd_counter),
				(soc_demo_transaction->snd_data_len-soc_demo_transaction->snd_counter), 0);
#ifdef MMI_ON_HARDWARE_P
			kal_prompt_trace(MOD_MMI,"Http send request result, sent_bytes:%d",ret);	
#endif			
			if(ret>0)
			{
				if(ret==(kal_int32)soc_demo_transaction->snd_data_len)
				{
					soc_demo_transaction->state=REQ_SENT;
					soc_demo_transaction->post_retry_counter++;
					em_soc_demo_app_start_timer(); //wait for servwer's response
					en_soc_demo_tcp_app_recv_response();
					return EN_SOC_SUCCESS;
				}
				else
				{
					soc_demo_transaction->snd_counter+=ret;
					return EN_SOC_SUCCESS;
				}
			}
			else
			{
				if(ret==SOC_WOULDBLOCK)
				{
					//waits for socket notify
					SetProtocolEventHandler( em_soc_demo_app_socket_notify, MSG_ID_APP_SOC_NOTIFY_IND);
					return EN_SOC_SUCCESS;
				}
				else
				{
					if(ret==SOC_ERROR)
					{
						en_soc_output_result(EN_SOC_PEER_NOT_REACHABLE,NULL,0);
						return EN_SOC_PEER_NOT_REACHABLE;
					}
					else
					{
						en_soc_output_result(EN_SOC_ERROR,NULL,0);
						return EN_SOC_ERROR;
					}
				}
			}
		}
		else
		{
			en_soc_output_result(EN_SOC_ERROR,NULL,0);
			return EN_SOC_ERROR;			
		}
	}
	else
	{
		en_soc_output_result(EN_SOC_ERROR,NULL,0);
		return EN_SOC_ERROR;	
	}
}
/****************************************************************
* FUNCTION                                                                                                                       
*	en_soc_demo_tcp_app_recv_response
* DESCRIPTION                                                           
*	The  em_soc_demo_app_socket_notify() calls this function to receive TCP incoming data. 
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	Socket demo application transaction may be completed.
*****************************************************************/
void en_soc_demo_tcp_app_recv_response(void)
{
	if(soc_demo_transaction && soc_demo_transaction->state<RSP_RCVD)
	{
		int ret_val=0;
		soc_demo_transaction->state=RSP_RCVING;
		ret_val = soc_recv(soc_demo_transaction->socket_id, (kal_uint8*)(soc_demo_transaction->rcvd_buffer+soc_demo_transaction->rcvd_counter), 
               (MAX_RCV_BUFFER_SIZE-soc_demo_transaction->rcvd_counter), 0);
		if(ret_val>0)
		{
			soc_demo_transaction->rcvd_counter+=ret_val;

			en_soc_output_result(EN_SOC_SUCCESS,
				(char*)soc_demo_transaction->rcvd_buffer,
				soc_demo_transaction->rcvd_counter);	
		}
		else
		{
			if(ret_val==SOC_WOULDBLOCK)
			{
				SetProtocolEventHandler( em_soc_demo_app_socket_notify, MSG_ID_APP_SOC_NOTIFY_IND);
				return;
			}
			else
			{
				en_soc_output_result(EN_SOC_ERROR,NULL,0);
			}				
		}
	}
}
/****************************************************************
* FUNCTION                                                                                                                       
*	en_soc_demo_udp_app_send_request
* DESCRIPTION                                                           
*	This function is used to send UDP datagram to specific server.
*	And receive server's response. 
*	If SOC can't receive any data for now, SOC will send MSG_ID_APP_SOC_NOTIFY_IND to MMI.
*	Then MMI will call em_soc_demo_app_socket_notify() to receive incoming data.
*	In the meantime, it will start a timer. If the timer is expired, there is no response from server.
*	We will resend the request to the same server until retry counter reaches MAX_POST_RETRY_NUM.
*
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	Socket demo application transaction may be completed.
*	or wait for MSG_ID_APP_SOC_NOTIFY_IND from SOC task
*****************************************************************/
int en_soc_demo_udp_app_send_request(void)
{
	kal_int32 ret;
	if(soc_demo_transaction)
	{
		if(soc_demo_transaction->socket_id>=0 && soc_demo_transaction->state>=SOC_CREATED)
		{

			/* Retry to send request to server*/
			if(soc_demo_transaction->state==REQ_SEND_RETRY)
				soc_demo_transaction->snd_counter=0;

			soc_demo_transaction->state=REQ_SENDING;
			ret = soc_sendto(soc_demo_transaction->socket_id,(kal_uint8*)(soc_demo_transaction->snd_buffer+soc_demo_transaction->snd_counter),
				(soc_demo_transaction->snd_data_len-soc_demo_transaction->snd_counter), 0, &soc_demo_transaction->server_ip_addr);
			if(ret>0)
			{
				if(ret==(kal_int32)soc_demo_transaction->snd_data_len)
				{
					soc_demo_transaction->state=REQ_SENT;
					soc_demo_transaction->post_retry_counter++;
					em_soc_demo_app_start_timer(); //wait for servwer's response
					en_soc_demo_udp_app_recv_response();
				}
				else
				{
					soc_demo_transaction->snd_counter+=ret;
				}
				return EN_SOC_SUCCESS;
			}
			else
			{
				if(ret==SOC_WOULDBLOCK)
				{
					SetProtocolEventHandler( em_soc_demo_app_socket_notify, MSG_ID_APP_SOC_NOTIFY_IND);
					return EN_SOC_SUCCESS;
				}
				else
				{
					en_soc_output_result(EN_SOC_ERROR,NULL,0);					
					return EN_SOC_ERROR;
				}
			}
		}
		else
		{
			en_soc_output_result(EN_SOC_ERROR,NULL,0);					
			return EN_SOC_ERROR;		
		}
	}
	else
	{
		en_soc_output_result(EN_SOC_ERROR,NULL,0);					
		return EN_SOC_ERROR;
	}
}
/****************************************************************
* FUNCTION                                                                                                                       
*	
* DESCRIPTION                                                           
*
* PARAMETERS
*
* RETURNS
*	None
* GLOBALS AFFECTED
*	None   
*****************************************************************/
void en_soc_demo_udp_app_recv_response(void)
{
	if(soc_demo_transaction && soc_demo_transaction->state<RSP_RCVD)
	{
		int ret_val=0;
		soc_demo_transaction->state=RSP_RCVING;
		ret_val = soc_recvfrom(soc_demo_transaction->socket_id, (kal_uint8*)(soc_demo_transaction->rcvd_buffer+soc_demo_transaction->rcvd_counter), 
               (MAX_RCV_BUFFER_SIZE-soc_demo_transaction->rcvd_counter), 0, &soc_demo_transaction->server_ip_addr);
		if(ret_val>0)
		{
			soc_demo_transaction->rcvd_counter+=ret_val;
			en_soc_output_result(EN_SOC_SUCCESS,
				(char*)soc_demo_transaction->rcvd_buffer,
				soc_demo_transaction->rcvd_counter);		
		}
		else
		{
			if(ret_val==SOC_WOULDBLOCK)
			{
				SetProtocolEventHandler( em_soc_demo_app_socket_notify, MSG_ID_APP_SOC_NOTIFY_IND);
				return;
			}
			else
			{
				en_soc_output_result(EN_SOC_ERROR,NULL,0);
			}
				
		}
	}
}
/****************************************************************
* FUNCTION                                                                                                                       
*	en_soc_demo_get_host_by_name
* DESCRIPTION                                                           
*	This function is used to get ip address of specific host name.
* PARAMETERS
*	mmi_soc_demo_app_enum app_id, (HTTP or DNS)
*	const kal_uint8 *hostname
* RETURNS
*	None
* GLOBALS AFFECTED
*	Socket demo application transaction may be completed.
*	Or it will trigger en_soc_demo_http_send_request() to sned request to HTTP server.
*	or wait for MSG_ID_APP_SOC_NOTIFY_IND from SOC task
*****************************************************************/
void en_soc_demo_get_host_by_name(mmi_soc_demo_app_enum app_id, const kal_uint8 *hostname)
{
	kal_int32 ret;
	soc_demo_transaction->state=REQ_SENDING;
	ret= soc_gethostbyname(KAL_FALSE, MOD_MMI,
			(kal_int32)app_id,
                     	(kal_char*)hostname,
                     	(kal_uint8*)soc_demo_transaction->rcvd_buffer,
                     	(kal_uint8*)&soc_demo_transaction->rcvd_counter,
                     	0,
                     	(kal_uint8)soc_demo_transaction->nwt_acount_id);

	if(ret==SOC_SUCCESS)
	{				
		if(app_id==DNS)
		{
			sprintf((char*)soc_demo_transaction->rcvd_buffer, "%d.%d.%d.%d", 
				(kal_uint8)*soc_demo_transaction->rcvd_buffer,
				(kal_uint8)*(soc_demo_transaction->rcvd_buffer+1),
				(kal_uint8)*(soc_demo_transaction->rcvd_buffer+2),
				(kal_uint8)*(soc_demo_transaction->rcvd_buffer+3));
				
				soc_demo_transaction->rcvd_counter=strlen((char*)soc_demo_transaction->rcvd_buffer);
				en_soc_output_result(EN_SOC_SUCCESS,
					(char*)soc_demo_transaction->rcvd_buffer,
					soc_demo_transaction->rcvd_counter);
		}
		else if(app_id==HTTP)
		{
			memcpy(soc_demo_transaction->server_ip_addr.addr,soc_demo_transaction->rcvd_buffer,4);
			soc_demo_transaction->rcvd_counter=0;
			soc_demo_transaction->server_ip_addr.addr_len=4;
			soc_demo_transaction->state=HTTP_TCP_CON_CREATING;
			en_soc_demo_http_send_request();
			
		}
		else
		{
			en_soc_output_result(EN_SOC_UNKNOWN_APP, NULL,0);
		}
	}
	else if(ret==SOC_WOULDBLOCK)
	{
		//waits for APP_SOC_GET_HOST_BY_NAME_IND
		SetProtocolEventHandler( em_soc_demo_app_soc_get_host_by_name_ind, MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND);
	}
	else
	{
		if(ret==SOC_ERROR)
		{
			en_soc_output_result(EN_SOC_PEER_NOT_REACHABLE,NULL,0);
		}
		else
		{
			en_soc_output_result(EN_SOC_ERROR,NULL,0);
		}				
	}			
}
/****************************************************************
* FUNCTION                                                                                                                       
*	em_soc_demo_app_soc_get_host_by_name_ind
* DESCRIPTION                                                           
*	This function is used to handle MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND from SOC.
* PARAMETERS
*	void *inMsg
* RETURNS
*	None
* GLOBALS AFFECTED
*	Socket demo application transaction may be completed.
*	Or it will trigger en_soc_demo_http_send_request() to sned request to HTTP server.
*****************************************************************/
void em_soc_demo_app_soc_get_host_by_name_ind(void* inMsg)
{
	app_soc_get_host_by_name_ind_struct *dns_ind;
	if(!inMsg)
	{
		en_soc_output_result(EN_SOC_ERROR,NULL,0);
   	}
	dns_ind = (app_soc_get_host_by_name_ind_struct *) inMsg;
	/* Check if the result is OK */
	if(dns_ind->result==KAL_TRUE)
	{
		switch(dns_ind->request_id)
		{
			/*If app_id is DNS, the socket demo transactio is completed. */	
			case DNS:
			{
				memcpy(soc_demo_transaction->rcvd_buffer,dns_ind->addr,dns_ind->addr_len);				

				sprintf((char*)soc_demo_transaction->rcvd_buffer, "%d.%d.%d.%d", 
					(kal_uint8)*soc_demo_transaction->rcvd_buffer,
					(kal_uint8)*(soc_demo_transaction->rcvd_buffer+1),
					(kal_uint8)*(soc_demo_transaction->rcvd_buffer+2),
					(kal_uint8)*(soc_demo_transaction->rcvd_buffer+3));
				
				soc_demo_transaction->rcvd_counter=strlen((char*)soc_demo_transaction->rcvd_buffer);

				en_soc_output_result(EN_SOC_SUCCESS,
					(char*)soc_demo_transaction->rcvd_buffer,
					soc_demo_transaction->rcvd_counter);

				break;
			}
			/*If app_id is HTTP, call en_soc_demo_http_send_request() to send request this ip address. */	
			case HTTP:
			{
				memcpy(soc_demo_transaction->server_ip_addr.addr,dns_ind->addr,dns_ind->addr_len);
				soc_demo_transaction->rcvd_counter=0;
				soc_demo_transaction->server_ip_addr.addr_len=4;
				soc_demo_transaction->state=HTTP_TCP_CON_CREATING;
				en_soc_demo_http_send_request();			
				break;
			}
			default:
				en_soc_output_result(EN_SOC_UNKNOWN_APP,NULL,0);
				break;
		}
	}
	else
	{
		en_soc_output_result(EN_SOC_ERROR,NULL,0);
	}	
}
/****************************************************************
* FUNCTION                                                                                                                       
*	en_soc_demo_app_create_socket
* DESCRIPTION                                                           
*	This function is used to create UDP or TCP socket
* PARAMETERS
*	None
* RETURNS
*	TRUE or FALSE
* GLOBALS AFFECTED
*	None   
*****************************************************************/
kal_bool en_soc_demo_app_create_socket()
{
	kal_uint8 val = 1;
	if(soc_demo_transaction)
	{
		soc_demo_transaction->state=SOC_CREATING;
		switch(soc_demo_transaction->soc_demo_app_id)
		{
			case HTTP:
			{
				soc_demo_transaction->socket_id=soc_create(PF_INET, SOCK_STREAM, 0, MOD_MMI, soc_demo_transaction->nwt_acount_id);
				if(soc_demo_transaction->socket_id>=0)
				{
				      if(soc_setsockopt(soc_demo_transaction->socket_id, SOC_NBIO, &val, sizeof(val)) < 0)
				      {
#ifdef MMI_ON_HARDWARE_P
				         kal_print("Set socket to nonblock mode error !!");
#endif
				         return KAL_FALSE;
				      }
      					val = SOC_READ | SOC_WRITE | SOC_CLOSE | SOC_CONNECT;
      					if(soc_setsockopt(soc_demo_transaction->socket_id , SOC_ASYNC, &val, sizeof(val)) < 0)
				      {
#ifdef MMI_ON_HARDWARE_P
				         kal_print("Set socket to nonblock mode error !!");
#endif
				         return KAL_FALSE;
				      }		      					    
					soc_demo_transaction->state=SOC_CREATED;		
					return KAL_TRUE;
				}
				else
				{
					return KAL_FALSE;
				}
				break;
			}
			case DNS:
			case DAYTIME:
			case ECHO:
			{	
				soc_demo_transaction->socket_id=soc_create(PF_INET, SOCK_DGRAM, 0, MOD_MMI, soc_demo_transaction->nwt_acount_id);
				if(soc_demo_transaction->socket_id>=0)
				{
				      if(soc_setsockopt(soc_demo_transaction->socket_id, SOC_NBIO, &val, sizeof(val)) < 0)
				      {
#ifdef MMI_ON_HARDWARE_P
				         kal_print("Set socket to nonblock mode error !!");
#endif
				         return KAL_FALSE;
				      }
      					val = SOC_READ | SOC_WRITE | SOC_CLOSE;
      					if(soc_setsockopt(soc_demo_transaction->socket_id , SOC_ASYNC, &val, sizeof(val)) < 0)
				      {
#ifdef MMI_ON_HARDWARE_P
				         kal_print("Set socket to nonblock mode error !!");
#endif
				         return KAL_FALSE;
				      }		      
				    SetProtocolEventHandler( em_soc_demo_app_socket_notify, MSG_ID_APP_SOC_NOTIFY_IND);
					soc_demo_transaction->state=SOC_CREATED;		
					return KAL_TRUE;
				}
				else
				{
					return KAL_FALSE;
				}
				
			}
			default:
				return KAL_FALSE;
		}
	}
	else
	{
		return KAL_FALSE;	
	}
}
/****************************************************************
* FUNCTION                                                                                                                       
*	em_soc_demo_app_socket_notify	
* DESCRIPTION                                                           
*	This function is used to handle events sent by SOC task
* PARAMETERS
*	void *inMsg
* RETURNS
*	None
* GLOBALS AFFECTED
*	None   
*****************************************************************/
void em_soc_demo_app_socket_notify(void* inMsg)
{
   app_soc_notify_ind_struct *soc_notify = (app_soc_notify_ind_struct *) inMsg;
   if(!soc_demo_transaction)
   	return;
#ifdef MMI_ON_HARDWARE_P
   kal_prompt_trace(MOD_MMI,"Got socket notify, event_type:%d",soc_notify->event_type);	
#endif
   switch( soc_notify->event_type)
   {
       case SOC_WRITE:
       {
		switch(soc_demo_transaction->soc_demo_app_id)
		{
			case HTTP:
				en_soc_demo_http_send_request();
				break;			
			case DAYTIME:
			case ECHO:					
				en_soc_demo_udp_app_send_request();
				break;				
		}       	
		break;
       }
       case SOC_READ:
       {
		switch(soc_demo_transaction->soc_demo_app_id)
		{
			case HTTP:
				en_soc_demo_tcp_app_recv_response();
				break;					
			case DAYTIME:
			case ECHO:
				en_soc_demo_udp_app_recv_response();
				break;								
		}       	
		break;
       }     
       case SOC_CONNECT:
       {
		if(soc_demo_transaction->soc_demo_app_id==HTTP)
		{
			if(soc_notify->result==KAL_TRUE)
			{
				soc_demo_transaction->state=HTTP_TCP_CON_CREATED;
				en_soc_demo_http_send_request();
			}
			else
			{
				en_soc_output_result(EN_SOC_PEER_NOT_REACHABLE,NULL,0);
			}
		}
		break;
       }
       case SOC_CLOSE:
       {
		en_soc_output_result(EN_SOC_CNT_RESET_BY_PEER,NULL,0);
       }
       default:
		en_soc_output_result(EN_SOC_ERROR,NULL,0);
       break;
    }
}
/****************************************************************
* FUNCTION                                                                                                                       
*	em_soc_demo_app_start_timer
* DESCRIPTION                                                           
*	This function is used to start EM_GPRS_SOC_DEMO_APP_TIMER timer.
*	The timeout value is SOC_DEMO_APP_POST_TIMEOUT.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None   
*****************************************************************/
void em_soc_demo_app_start_timer(void)
{
	StartTimer(EM_GPRS_SOC_DEMO_APP_TIMER,SOC_DEMO_APP_POST_TIMEOUT,em_soc_demo_app_timer_handler);
}
/****************************************************************
* FUNCTION                                                                                                                       
*	em_soc_demo_app_stop_timer
* DESCRIPTION                                                           
*	This function is used to stop EM_GPRS_SOC_DEMO_APP_TIMER timer.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None   
*****************************************************************/
void em_soc_demo_app_stop_timer(void)
{
     StopTimer( EM_GPRS_SOC_DEMO_APP_TIMER);
}
/****************************************************************
* FUNCTION                                                                                                                       
*	em_soc_demo_app_timer_handler
* DESCRIPTION                                                           
*	When EM_GPRS_SOC_DEMO_APP_TIMER expires, 
*	MMI will call this function to resend request to server.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	Socket demo application transaction may be completed.
*	If retry counter reaches MAX_POST_RETRY_NUM. 
*****************************************************************/
void em_soc_demo_app_timer_handler(void)
{
	if(soc_demo_transaction->post_retry_counter<MAX_POST_RETRY_NUM)
	{
		soc_demo_transaction->state=REQ_SEND_RETRY;
		switch(soc_demo_transaction->soc_demo_app_id)
		{
			case HTTP:
				en_soc_demo_http_send_request();
				break;						
			case DAYTIME:
			case ECHO:
				en_soc_demo_udp_app_send_request();
				break;
			default:
				en_soc_output_result(EN_SOC_UNKNOWN_APP,NULL,0);
				break;					
		}
	}
	else
	{
		en_soc_output_result(EN_SOC_PEER_NO_RESPONSE,NULL,0);	
	}
}
/****************************************************************
* FUNCTION                                                                                                                       
*	en_soc_demo_http_get_parsed_url
* DESCRIPTION                                                           
*	This functino is used to parse uri given by user. 
*	It can retrieve hostname and port number.
* PARAMETERS
*	const kal_int8* in_str, 
*	int str_len
* RETURNS
*	soc_demo_app_http_url_struct* (parsed uri information)
* GLOBALS AFFECTED
*	None   
*****************************************************************/
soc_demo_app_http_url_struct* en_soc_demo_http_get_parsed_url(const kal_int8* in_str, int str_len)
{
	char *hostname=NULL;
	soc_demo_app_http_url_struct* rsp=NULL;
	
	rsp=OslMalloc(sizeof(soc_demo_app_http_url_struct));
	if(rsp)
	{
		memset(rsp,0,sizeof(soc_demo_app_http_url_struct));
		hostname=strstr((char*)in_str,"http://");
		if(!hostname)
		{		
			OslMfree(rsp);
			return NULL;
		}
		else
		{
			char *slash, *colon;
			hostname+=7; //strlen(http://)
			slash=strchr(hostname,'/');
			if(!slash)
			{
				colon=strchr(hostname,':');
				if(colon)
				{					
					rsp->host_name_len=(colon-hostname);
					rsp->host_name=OslMalloc((U16)(rsp->host_name_len+1));
					if(!rsp->host_name)
					{
						OslMfree(rsp);
						return NULL;
					}
					memset(rsp->host_name,0,rsp->host_name_len+1);
					memcpy(rsp->host_name,hostname,rsp->host_name_len);					
				
					rsp->ser_port_num=atoi(colon);					
				}
				else
				{
					rsp->ser_port_num=80;
					rsp->host_name_len=strlen(hostname);
					rsp->host_name=OslMalloc((U16)(rsp->host_name_len+1));
					if(!rsp->host_name)
					{
						OslMfree(rsp);
						return NULL;
					}
					memset(rsp->host_name,0,rsp->host_name_len+1);
					memcpy(rsp->host_name,hostname,rsp->host_name_len);					
				}							
				return rsp;			
			}
			else
			{	
				colon=strchr(hostname,':');
				if(colon)
				{					
					char *port_str;

					rsp->host_name_len=(colon-hostname);
					rsp->host_name=OslMalloc((U16)(rsp->host_name_len+1));
					if(!rsp->host_name)
					{
						OslMfree(rsp);
						return NULL;
					}
					memset(rsp->host_name,0,rsp->host_name_len+1);
					memcpy(rsp->host_name,hostname,rsp->host_name_len);	
					
					port_str=OslMalloc((U16)(slash-colon));
					memset(port_str,0,slash-colon);
					if(!port_str)
					{
						OslMfree(rsp->host_name);
						OslMfree(rsp);
						return NULL;
					}
					strncpy(port_str,colon+1,slash-colon-1);
					rsp->ser_port_num=atoi(port_str);
					OslMfree(port_str);
				}
				else
				{
					rsp->ser_port_num=80;
					rsp->host_name_len=slash-hostname;
					rsp->host_name=OslMalloc((U16)(rsp->host_name_len+1));
					if(!rsp->host_name)
					{
						OslMfree(rsp);
						return NULL;
					}
					memset(rsp->host_name,0,rsp->host_name_len+1);
					memcpy(rsp->host_name,hostname,rsp->host_name_len);	
				}
				rsp->url_link_len=strlen(slash);
				rsp->url_link=OslMalloc((U16)(rsp->url_link_len+1));
				if(!rsp->url_link)
				{
					OslMfree(rsp->host_name);
					OslMfree(rsp->url_link);
					OslMfree(rsp);
					return NULL;
				}				
				memset(rsp->url_link,0,rsp->url_link_len+1);
				memcpy(rsp->url_link,slash,rsp->url_link_len);									
				return rsp;
			}
		}		
	}
	else
	{
		return NULL;
	}
}
#endif /* (defined(__MMI_ENGINEER_MODE__) && defined(__TCPIP__) ) */

