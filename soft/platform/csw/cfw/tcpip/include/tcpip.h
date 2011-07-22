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
#ifdef CFW_TCPIP_SUPPORT
#ifndef __CII_TCPIP_H__
#define __CII_TCPIP_H__

#include "tcpip_api_msg.h"
#include "tcpip_pbuf.h"

VOID tcpip_init(VOID (*tcpip_init_done) (VOID *), VOID *arg);
VOID tcpip_apimsg(struct api_msg *apimsg);
err_t tcpip_input(struct pbuf *p, struct ip_addr *inp);
err_t tcpip_callback(VOID (*f) (VOID *ctx), VOID *ctx);

VOID tcpip_tcp_timer_needed(VOID);
VOID sys_post_event_to_APS(VOID *msg);
VOID sys_post_event_to_APP(UINT32 nEventId, UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT8 nFlag, UINT16 reserved);
err_t data_output(struct netif *netif, struct pbuf *p, struct ip_addr *ipaddr);

#define TCPIP_MSG_API 0
#define TCPIP_MSG_INPUT 1
#define TCPIP_MSG_CALLBACK 2

struct tcpip_msg
{
    UINT8 type;
    sys_sem_t *sem;
    union
    {
        struct api_msg *apimsg;
        struct
        {
            struct pbuf *p;
            struct ip_addr *netif;
        } inp;
        struct
        {
            VOID (*f) (VOID *ctx);
            VOID *ctx;
        } cb;
    } msg;
};


#endif
#endif /* __CII_TCPIP_H__ */
