/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
	
//#include "tcpip_opt.h"
#ifdef CFW_TCPIP_SUPPORT
#include "cmn_defs.h"
#include "tcpip_sys.h"
#include "tcpip_ip_frag.h"
#include "tcpip_udp.h"
#include "tcpip_tcp.h"
#include "tcpip_memp.h"
#include "tcpip.h"

//#include <cfw.h>

//#include "tcpip_pbuf.h"
#include "cos.h"

#include "tcpip_sys.h"

#include "cmn_defs.h"

//#include "tcpip_sys.h"

extern err_t resolv_init(const INT8 * cp);

err_t data_output(struct netif *netif, struct pbuf *p,struct ip_addr *ipaddr);

static VOID (*tcpip_init_done) (VOID *arg) = NULL;
static VOID *tcpip_init_done_arg;
static UINT32 mbox;

static INT8 DNS_Server_IP_ChinaMobile[16] = "211.136.17.107";
//static INT8 DNS_Server_IP_ChinaUnicom[16] = "";

//extern static struct lwip_socket sockets[NUM_SOCKETS];

#if CII_TCP
static INT32 tcpip_tcp_timer_active = 0;

 VOID tcpip_tcp_timer(VOID *arg)
{
    (VOID) arg;
    BOOL ret; 
    /* call TCP timer handler */
    tcp_tmr();
    /* timer still needed? */
    if (tcp_active_pcbs || tcp_tw_pcbs)
    {
        /* restart timer */
      ret = COS_SetTimerEX(
                BAL_TH(BAL_CFW_ADV_TASK_PRIORITY), PRV_CFW_TCP_TIMER_ID, 
                COS_TIMER_MODE_SINGLE, TCP_TMR_INTERVAL MILLI_SECOND);		

	if(ret)
	{
	    //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("COS_SetTimer start success\n"));
	}
	else
	{
	    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("COS_SetTimer start fail\n"));
	}
    }
    else
    {
        /* disable timer */
        tcpip_tcp_timer_active = 0;
    }
}
 VOID ip_timer(VOID *data);  // look at the bellow: 
#if !NO_SYS
VOID tcp_timer_needed(VOID)
{
    BOOL ret; 
    /* timer is off but needed again? */

    if (!tcpip_tcp_timer_active && (tcp_active_pcbs || tcp_tw_pcbs))
    {
        /* enable and start timer */
        tcpip_tcp_timer_active = 1;
        //sys_timeout(TCP_TMR_INTERVAL, tcpip_tcp_timer, NULL);
       // time id : PRV_CFW_TCP_TIMER_ID
       //COS_SendEvent(BAL_TH(BAL_CFW_ADV_TASK_PRIORITY) , &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
		ret = COS_SetTimerEX(
                BAL_TH(BAL_CFW_ADV_TASK_PRIORITY), PRV_CFW_TCP_TIMER_ID, 
                COS_TIMER_MODE_SINGLE, TCP_TMR_INTERVAL MILLI_SECOND);

		if(ret)
		{
		    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("COS_SetTimer start success\n"));
		}
		else
		{
		    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("COS_SetTimer start fail\n"));
		}
		ip_timer(NULL);
    }
}
#endif /* !NO_SYS */
#endif /* CII_TCP */

#if IP_REASSEMBLY
VOID ip_timer(VOID *data)  // look at the bellow: 
{
    BOOL ret;     
    static INT32 nTimer = 0;
//    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcpip: ip_timer()\n"));
    ip_reass_tmr();
    if(tcpip_tcp_timer_active)
    {
		ret = COS_SetTimerEX(
		        BAL_TH(BAL_CFW_ADV_TASK_PRIORITY), PRV_CFW_IP_TIMER_ID, 
		        COS_TIMER_MODE_SINGLE, 1000 MILLI_SECOND);

		if(ret)
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("Timer:  this time is %d s\n"), nTimer++);
		}
		else
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("ip_timer start fail\n"));
		}
    }
}
#endif
#if 0
static VOID tcpip_thread(VOID *arg)
{
    struct tcpip_msg *msg;

    (VOID) arg;

    ip_init();
#if CII_UDP
    udp_init();
#endif
#if CII_TCP
    tcp_init();
#endif
#if IP_REASSEMBLY
    sys_timeout(1000, ip_timer, NULL);
#endif
    if (tcpip_init_done != NULL)
    {
        tcpip_init_done(tcpip_init_done_arg);
    }

    while (1)
    {            /* MAIN Loop */
        sys_mbox_fetch(mbox, (VOID *) &msg);
        switch (msg->type)
        {
        case TCPIP_MSG_API:
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcpip_thread: API message %p\n"), (VOID *) msg);
            api_msg_input(msg->msg.apimsg);
            break;
        case TCPIP_MSG_INPUT:
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcpip_thread: IP packet %p\n"), (VOID *) msg);
            ip_input(msg->msg.inp.p, msg->msg.inp.netif);
            break;
        case TCPIP_MSG_CALLBACK:
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcpip_thread: CALLBACK %p\n"), (VOID *) msg);
            msg->msg.cb.f(msg->msg.cb.ctx);
            break;
        default:
            break;
        }
        memp_free(MEMP_TCPIP_MSG, msg);
    }
}

#endif



err_t tcpip_input(struct pbuf *p, struct ip_addr *inp)
{
    struct tcpip_msg *msg;

    msg = memp_malloc(MEMP_TCPIP_MSG);
    if (msg == NULL)
    {
        pbuf_free(p);
        return ERR_TCPIP_MEM;
    }

    msg->type = TCPIP_MSG_INPUT;
    msg->msg.inp.p = p;
    msg->msg.inp.netif = inp;
    sys_mbox_post(mbox, msg);
    return ERR_TCPIP_OK;
}

err_t tcpip_callback(VOID (*f) (VOID *ctx), VOID *ctx)
{
    struct tcpip_msg *msg;

    msg = memp_malloc(MEMP_TCPIP_MSG);
    if (msg == NULL)
    {
        return ERR_TCPIP_MEM;
    }

    msg->type = TCPIP_MSG_CALLBACK;
    msg->msg.cb.f = f;
    msg->msg.cb.ctx = ctx;
    sys_mbox_post(mbox, msg);
    return ERR_TCPIP_OK;
}

VOID tcpip_apimsg(struct api_msg *apimsg)
{

#if 0    
struct tcpip_msg *msg;
    msg = memp_malloc(MEMP_TCPIP_MSG);
    if (msg == NULL)
    {
        memp_free(MEMP_API_MSG, apimsg);
        return;
    }
    msg->type = TCPIP_MSG_API;
    msg->msg.apimsg = apimsg;
    sys_mbox_post(mbox, msg);
#endif	

   COS_EVENT       msg;

//msg.nEventId = (UINT32)TCPIP_MSG_API;
   msg.nEventId = (UINT32)EV_INTER_APS_TCPIP_REQ;
   msg.nParam1 = (UINT32)apimsg;
   sys_post_event_to_APS( &msg );

}

VOID tcpip_init(VOID (*initfunc) (VOID *), VOID *arg)
{
    tcpip_init_done = initfunc;
    tcpip_init_done_arg = arg;
    mbox = sys_mbox_new();
   // sys_thread_new(tcpip_thread, NULL, TCPIP_THREAD_PRIO);
}



err_t data_output(struct netif *netif, struct pbuf *p,
       struct ip_addr *ipaddr)
{

    UINT16  offset = 0;
    struct pbuf *q;
    CFW_GPRS_DATA *pGprsData = NULL;
    pGprsData = CSW_TCPIP_MALLOC(sizeof(CFW_GPRS_DATA) + p ->tot_len);
    if(pGprsData == NULL)
    {
       CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("data_output : CSW_TCPIP_MALLOC == 0    !!!\n"));
	return !ERR_TCPIP_OK;
    }else{
           CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("data_output : CSW_TCPIP_MALLOC  success   !!!\n"));

    	}
    pGprsData->nDataLength = p ->tot_len;
    
    for (q = p; q != NULL; q = q->next) 
    {
      SUL_MemCopy8 (&pGprsData->pData[offset],q->payload,q->len);
      offset += q->len ;
    }
   #ifdef CFW_MULTI_SIM
    CFW_GprsSendData(netif->lowaddr.cid & 0x0000ffff,  pGprsData, (netif->lowaddr.cid & 0xffff0000) >> 16);
   #else 
    CFW_GprsSendData(netif->lowaddr.cid,pGprsData);
    #endif
    CSW_TCPIP_FREE(pGprsData);  
    
    return ERR_TCPIP_OK;

}
sys_sem_t Tcp_receive_buf_sem;//modify by wusy 2008-11-26
VOID socketdata_initial(INT32 sockfd, UINT8 type);
//extern sys_sem_t Tcp_receive_buf_sem;//add by wuys 2008-09-09 for test gprs
void TCpIPInit(void)
{
//    UINT8 addr[4],lenth;

#if STATIC_MEMORY
    mem_init();
    memp_init();  
    pbuf_init(); 
#endif    

    ip_reass_mem_init();

    ip_init();
#if  CII_UDP  
    udp_init();
#endif
#if CII_TCP
    tcp_init();
#endif

#if CII_RESOLV
    resolv_init(DNS_Server_IP_ChinaMobile);
#endif
Tcp_receive_buf_sem = COS_CreateSemaphore(1);//add by wuys  2008-09-09 for test gprs
 socketdata_initial(0xFF, 0xFF);
  //  tcp_NSapi = 1;
    //sys_sem_signal(tcpip_ctx.sem);
    //sys_mbox_post(tcpip_ctx.mbx, NULL);   		        
}


VOID Tcpip_Destroy(VOID)
{

#if STATIC_MEMORY
          free_sxr_mem();
	        free_sxr_memp();
	        free_sxr_pbuf();
#endif            

}
#endif

