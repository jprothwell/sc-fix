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
#ifndef __LWIP_PBUF_H__
#define __LWIP_PBUF_H__

//#include "cc.h"

#define PBUF_TRANSPORT_HLEN 20
#define PBUF_IP_HLEN        20



#define PBUF_TRANSPORT 0
#define PBUF_IP 1
#define PBUF_LINK 2
#define PBUF_RAW 3

#define PBUF_RAM 0
#define PBUF_ROM 1
#define PBUF_REF 2
#define PBUF_POOL 3


/* Definitions for the pbuf flag field. These are NOT the flags that
 * are passed to pbuf_alloc(). */
#define PBUF_FLAG_RAM   0x00U    /* Flags that pbuf data is stored in RAM */
#define PBUF_FLAG_ROM   0x01U    /* Flags that pbuf data is stored in ROM */
#define PBUF_FLAG_POOL  0x02U    /* Flags that the pbuf comes from the pbuf pool */
#define PBUF_FLAG_REF   0x04U    /* Flags thet the pbuf payload refers to RAM */

/** indicates this packet was broadcast on the link */
#define PBUF_FLAG_LINK_BROADCAST 0x80U

struct pbuf
{
  /** next pbuf in singly linked pbuf chain */
    struct pbuf *next;

  /** pointer to the actual data in the buffer */
    VOID *payload;

  /**
   * total length of this buffer and all next buffers in chain
   * belonging to the same packet.
   *
   * For non-queue packet chains this is the invariant:
   * p->tot_len == p->len + (p->next? p->next->tot_len: 0)
   */
    UINT16 tot_len;

  /** length of this buffer */
    UINT16 len;

  /** flags telling the type of pbuf, see PBUF_FLAG_ */
    UINT16 flags;

  /**
   * the reference count always equals the number of pointers
   * that refer to this pbuf. This can be pointers from an application,
   * the stack itself, or pbuf->next pointers from a chain.
   */
    UINT16 ref;

};

VOID pbuf_init(VOID);

struct pbuf *pbuf_alloc(UINT8 l, UINT16 size, UINT8 flag);
VOID pbuf_realloc(struct pbuf *p, UINT16 size);
UINT8 pbuf_header(struct pbuf *p, INT16 header_size);
VOID pbuf_ref(struct pbuf *p);
VOID pbuf_ref_chain(struct pbuf *p);
UINT8 pbuf_free(struct pbuf *p);
UINT8 pbuf_clen(struct pbuf *p);
VOID pbuf_cat(struct pbuf *h, struct pbuf *t);
VOID pbuf_chain(struct pbuf *h, struct pbuf *t);
struct pbuf *pbuf_take(struct pbuf *f);
struct pbuf *pbuf_dechain(struct pbuf *p);
VOID pbuf_queue(struct pbuf *p, struct pbuf *n);
struct pbuf *pbuf_dequeue(struct pbuf *p);
void free_sxr_pbuf();

#endif /* __LWIP_PBUF_H__ */

#endif

