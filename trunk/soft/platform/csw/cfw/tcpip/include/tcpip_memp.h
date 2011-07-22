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
#ifndef __LWIP_MEMP_H__
#define __LWIP_MEMP_H__

#include "tcpip_opt.h"

#define old_level   //temp by zouy

#define MEMP_PBUF 0
#define MEMP_RAW_PCB 1
#define MEMP_UDP_PCB 2
#define MEMP_TCP_PCB 3
#define MEMP_TCP_PCB_LISTEN 4
#define MEMP_TCP_SEG 5
#define MEMP_NETBUF 6
#define MEMP_NETCONN 7
#define MEMP_API_MSG 8
#define MEMP_TCPIP_MSG 9
#define MEMP_SYS_TIMEOUT 10
#define MEMP_MAX 11

VOID memp_init(VOID);

VOID *memp_malloc(UINT8 type);
VOID *memp_realloc(UINT8 fromtype, UINT8 totype, VOID *mem);
VOID memp_free(UINT8 type, VOID *mem);
void free_sxr_memp();

#endif /* __LWIP_MEMP_H__  */
#endif
