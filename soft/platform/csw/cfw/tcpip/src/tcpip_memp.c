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
//#include "tcpip_opt.h"

//#include "tcpip_memp.h"
#include "tcpip_api.h"
//#include "tcpip_sys.h"

//#include "tcpip_pbuf.h"
//#include <csw.h>
//#include "tcpip_udp.h"
//#include "tcpip_raw.h"
//#include "tcpip_tcp.h"
//#include "tcpip_api_msg.h"
#include "tcpip.h"

#include "tcpip_stats.h"

struct memp
{
    struct memp *next;
};

static struct memp *memp_tab[MEMP_MAX];

static const UINT16 memp_sizes[MEMP_MAX] = {
    sizeof(struct pbuf),
    sizeof(struct raw_pcb),
    sizeof(struct udp_pcb),
    sizeof(struct tcp_pcb),
    sizeof(struct tcp_pcb_listen),
    sizeof(struct tcp_seg),
    sizeof(struct netbuf),
    sizeof(struct netconn),
    sizeof(struct api_msg),
    sizeof(struct tcpip_msg),
    sizeof(struct sys_timeout)
};

static const UINT16 memp_num[MEMP_MAX] = {
    MEMP_NUM_PBUF,
    MEMP_NUM_RAW_PCB,
    MEMP_NUM_UDP_PCB,
    MEMP_NUM_TCP_PCB,
    MEMP_NUM_TCP_PCB_LISTEN,
    MEMP_NUM_TCP_SEG,
    MEMP_NUM_NETBUF,
    MEMP_NUM_NETCONN,
    MEMP_NUM_API_MSG,
    MEMP_NUM_TCPIP_MSG,
    MEMP_NUM_SYS_TIMEOUT
};

static UINT8  *memp_memory = NULL;
#if 0
static UINT8 memp_memory[(MEMP_NUM_PBUF *
             MEM_ALIGN_SIZE(sizeof(struct pbuf) +
                    sizeof(struct memp)) +
             MEMP_NUM_RAW_PCB *
             MEM_ALIGN_SIZE(sizeof(struct raw_pcb) +
                    sizeof(struct memp)) +
             MEMP_NUM_UDP_PCB *
             MEM_ALIGN_SIZE(sizeof(struct udp_pcb) +
                    sizeof(struct memp)) +
             MEMP_NUM_TCP_PCB *
             MEM_ALIGN_SIZE(sizeof(struct tcp_pcb) +
                    sizeof(struct memp)) +
             MEMP_NUM_TCP_PCB_LISTEN *
             MEM_ALIGN_SIZE(sizeof(struct tcp_pcb_listen) +
                    sizeof(struct memp)) +
             MEMP_NUM_TCP_SEG *
             MEM_ALIGN_SIZE(sizeof(struct tcp_seg) +
                    sizeof(struct memp)) +
             MEMP_NUM_NETBUF *
             MEM_ALIGN_SIZE(sizeof(struct netbuf) +
                    sizeof(struct memp)) +
             MEMP_NUM_NETCONN *
             MEM_ALIGN_SIZE(sizeof(struct netconn) +
                    sizeof(struct memp)) +
             MEMP_NUM_API_MSG *
             MEM_ALIGN_SIZE(sizeof(struct api_msg) +
                    sizeof(struct memp)) + 
             MEMP_NUM_TCPIP_MSG *
            MEM_ALIGN_SIZE(sizeof(struct tcpip_msg) +
                    sizeof(struct memp)) +
             MEMP_NUM_SYS_TIMEOUT *
            MEM_ALIGN_SIZE(sizeof(struct sys_timeout) +
                    sizeof(struct memp)))];
#endif


#if !SYS_LIGHTWEIGHT_PROT
static sys_sem_t mutex = SYS_SEM_NULL;
#endif

#if MEMP_SANITY_CHECK
static INT32 memp_sanity(VOID)
{
    INT16 i, c;
    struct memp *m, *n;

    for (i = 0; i < MEMP_MAX; i++)
    {
        for (m = memp_tab[i]; m != NULL; m = m->next)
        {
            c = 1;
            for (n = memp_tab[i]; n != NULL; n = n->next)
            {
                if (n == m)
                {
                    --c;
                }
                if (c < 0)
                    return 0;    /* LW was: abort(); */
            }
        }
    }
    return 1;
}
#endif /* MEMP_SANITY_CHECK */

VOID memp_init(VOID)
{
    struct memp *m, *memp;
    UINT16 i, j;
    UINT16 size;
	UINT32 memSize;
	UINT16 mempSize;
	
#if 1
    mempSize = sizeof(struct memp);
    memSize =
        MEMP_NUM_PBUF *
            MEM_ALIGN_SIZE(sizeof(struct pbuf) + mempSize) +
        MEMP_NUM_RAW_PCB *
            MEM_ALIGN_SIZE(sizeof(struct raw_pcb) + mempSize) +
        MEMP_NUM_UDP_PCB *
            MEM_ALIGN_SIZE(sizeof(struct udp_pcb) + mempSize) +
        MEMP_NUM_TCP_PCB *
            MEM_ALIGN_SIZE(sizeof(struct tcp_pcb) + mempSize) +
        MEMP_NUM_TCP_PCB_LISTEN *
            MEM_ALIGN_SIZE(sizeof(struct tcp_pcb_listen) + mempSize) +
        MEMP_NUM_TCP_SEG *
            MEM_ALIGN_SIZE(sizeof(struct tcp_seg) + mempSize) +
        MEMP_NUM_NETBUF *
            MEM_ALIGN_SIZE(sizeof(struct netbuf) + mempSize) +
        MEMP_NUM_NETCONN *
            MEM_ALIGN_SIZE(sizeof(struct netconn) + mempSize) +
        MEMP_NUM_API_MSG *
            MEM_ALIGN_SIZE(sizeof(struct api_msg) + mempSize) +
        MEMP_NUM_TCPIP_MSG *
            MEM_ALIGN_SIZE(sizeof(struct tcpip_msg) + mempSize) +
        MEMP_NUM_SYS_TIMEOUT *
            MEM_ALIGN_SIZE(sizeof(struct sys_timeout) + mempSize);

    memp_memory = CSW_TCPIP_MALLOC(memSize);
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("memp size: %d, %p\n"), memSize, (VOID*)memp_memory);

#endif

#if MEMP_STATS
    for (i = 0; i < MEMP_MAX; ++i)
    {
        lwip_stats.memp[i].used = lwip_stats.memp[i].max = lwip_stats.memp[i].err = 0;
        lwip_stats.memp[i].avail = memp_num[i];
    }
#endif /* MEMP_STATS */

    memp = (struct memp *) &memp_memory[0];
    for (i = 0; i < MEMP_MAX; ++i)
    {
        size = MEM_ALIGN_SIZE(memp_sizes[i] + sizeof(struct memp));
        if (memp_num[i] > 0)
        {
            memp_tab[i] = memp;
            m = memp;

            for (j = 0; j < memp_num[i]; ++j)
            {
                m->next = (struct memp *) MEM_ALIGN((UINT8 *) m + size);
                memp = m;
                m = m->next;
            }
            memp->next = NULL;
            memp = m;
        }
        else
        {
            memp_tab[i] = NULL;
        }
    }

#if !SYS_LIGHTWEIGHT_PROT
//    mutex = sys_sem_new(1);
    mutex = COS_CreateSemaphore(1);  // add semaphore dupb
#endif

}


#if STATIC_MEMORY
 extern struct tcp_pcb *tcp_bound_pcbs; 
UINT8 memp_nebuf_used =0;
UINT8 memp_tcppcb_used =0;
VOID *memp_malloc(UINT8 type)
{
    struct memp *memp;
    VOID *mem;
#if SYS_LIGHTWEIGHT_PROT
    SYS_ARCH_DECL_PROTECT(old_level);
#endif

  //  LWIP_ASSERT("memp_malloc: type < MEMP_MAX", type < MEMP_MAX);

#if SYS_LIGHTWEIGHT_PROT
//    SYS_ARCH_PROTECT(old_level);
#else /* SYS_LIGHTWEIGHT_PROT */
//    sys_sem_wait(mutex);
    COS_WaitForSemaphore(mutex, COS_WAIT_FOREVER); // add semaphore dupb
#endif /* SYS_LIGHTWEIGHT_PROT */

    memp = memp_tab[type];

    if (memp != NULL)
    {
        memp_tab[type] = memp->next;
        memp->next = NULL;
#if MEMP_STATS
        ++lwip_stats.memp[type].used;
        if (lwip_stats.memp[type].used > lwip_stats.memp[type].max)
        {
            lwip_stats.memp[type].max = lwip_stats.memp[type].used;
        }
#endif /* MEMP_STATS */
#if SYS_LIGHTWEIGHT_PROT
        SYS_ARCH_UNPROTECT(old_level);
#else /* SYS_LIGHTWEIGHT_PROT */
 //       sys_sem_signal(mutex);
          COS_ReleaseSemaphore(mutex); // release semaphore dupb
#endif /* SYS_LIGHTWEIGHT_PROT */
        LWIP_ASSERT("memp_malloc: memp properly aligned", ((mem_ptr_t) MEM_ALIGN((UINT8 *) memp + sizeof(struct memp)) % MEM_ALIGNMENT) == 0);

        mem = MEM_ALIGN((UINT8 *) memp + sizeof(struct memp));
	if(type==0x6)
	{
	       memp_nebuf_used ++;
	       CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("memp_malloc  nebuf: 0x%x, used: %d"), mem, memp_nebuf_used);
	}else if(type==MEMP_TCP_PCB)
	{
		memp_tcppcb_used++;
	       CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("memp_malloc: 0x%x, tcp_active_pcbs: 0x%x; tw: 0x%x, used pcb:0x%x"), mem, tcp_active_pcbs, tcp_tw_pcbs,memp_tcppcb_used);
	}
	TCP_ASSERT(mem!= tcp_active_pcbs);
	TCP_ASSERT(mem!= tcp_tw_pcbs);
       TCP_ASSERT(mem!= tcp_bound_pcbs);

	SUL_MemSet8(mem, 0x00, memp_sizes[type]);
        return mem;
    }
    else
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("memp_malloc: out of memory in pool 0x%x\n"), type);
#if MEMP_STATS
        ++lwip_stats.memp[type].err;
#endif /* MEMP_STATS */
#if SYS_LIGHTWEIGHT_PROT
        SYS_ARCH_UNPROTECT(old_level);
#else /* SYS_LIGHTWEIGHT_PROT */
 //       sys_sem_signal(mutex);
        COS_ReleaseSemaphore(mutex); // release semaphore dupb
#endif /* SYS_LIGHTWEIGHT_PROT */
        return NULL;
    }
}



 
 void memp_assert(VOID* mem)
 {
     struct tcp_pcb * tmp_pcb;
     UINT8 uu=0;
	// UINT8 num=0;
     if(tcp_active_pcbs!=NULL)
     	{
     	    
	    for(tmp_pcb = tcp_active_pcbs;tmp_pcb !=NULL;tmp_pcb = tmp_pcb->next)
	    {
			if((VOID*)tmp_pcb == mem)
			{
			        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("memp_assert: in  tcp_active_pcbs\n"));
				 uu =1;
			}
	    }

     	}

	 if(tcp_tw_pcbs!=NULL)
     	{
	    for(tmp_pcb = tcp_tw_pcbs;tmp_pcb !=NULL;tmp_pcb = tmp_pcb->next)
			if((VOID*)tmp_pcb == mem)
			{
			       CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("memp_assert: in  tcp_tw_pcbs\n"));
				uu =2;
			}
     	}
	
	 if(tcp_bound_pcbs!=NULL)
     	{
	    for(tmp_pcb = tcp_bound_pcbs;tmp_pcb !=NULL;tmp_pcb = tmp_pcb->next)
			if((VOID*)tmp_pcb == mem)
			{
			       CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("memp_assert: in  tcp_bound_pcbs\n"));
				uu =3;
			}
     	}
	TCP_ASSERT(uu==0);

 }

VOID memp_free(UINT8 type, VOID *mem)
{
    struct memp *memp;
    struct memp *memp_tmp;
#if SYS_LIGHTWEIGHT_PROT
    SYS_ARCH_DECL_PROTECT(old_level);
#endif /* SYS_LIGHTWEIGHT_PROT */

  
   memp_assert(mem);
   TCP_ASSERT(mem!= tcp_active_pcbs);
   TCP_ASSERT(mem!= tcp_tw_pcbs);
   TCP_ASSERT(mem!= tcp_bound_pcbs);
    
    
    if (mem == NULL)
    {
        return;
    }
    SUL_MemSet8(mem, 0x00, memp_sizes[type]);
    memp = (struct memp *) ((UINT8 *) mem - sizeof(struct memp));

#if SYS_LIGHTWEIGHT_PROT
    SYS_ARCH_PROTECT(old_level);
#else /* SYS_LIGHTWEIGHT_PROT */
 //   sys_sem_wait(mutex);
    COS_WaitForSemaphore(mutex, COS_WAIT_FOREVER); // add semaphore dupb
#endif /* SYS_LIGHTWEIGHT_PROT */

#if MEMP_STATS
    lwip_stats.memp[type].used--;
#endif /* MEMP_STATS */
   for(memp_tmp =memp_tab[type]; memp_tmp!=NULL; memp_tmp=memp_tmp->next)
   	{ 
   	   if(memp_tmp==memp)
	   	break;
   	
   	}
   if(memp_tmp!=NULL)
   	{
   	    COS_ReleaseSemaphore(mutex); 
   	    		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("memp_free: the memp: 0x%x is free!"), mem);

   	    return;

   	}
    memp->next = memp_tab[type];
    memp_tab[type] = memp;

#if MEMP_SANITY_CHECK
    LWIP_ASSERT("memp sanity", memp_sanity());
#endif

#if SYS_LIGHTWEIGHT_PROT
    SYS_ARCH_UNPROTECT(old_level);
#else /* SYS_LIGHTWEIGHT_PROT */
 //       sys_sem_signal(mutex);
 if(type==0x6)
			{
		memp_nebuf_used --;
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("free MEMP_NETBUF: 0x%x, used: %d"), memp+sizeof(struct memp), memp_nebuf_used);
			}
  if(type==MEMP_TCP_PCB)
  	{
  	      memp_tcppcb_used --;
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("memp_free: 0x%x, tcp_active_pcbs: 0x%x; tw: 0x%x, used pcd: 0x%x"), mem, tcp_active_pcbs, tcp_tw_pcbs,memp_tcppcb_used);
  	}
    COS_ReleaseSemaphore(mutex); // release semaphore dupb
#endif /* SYS_LIGHTWEIGHT_PROT */
}

#else

void *memp_malloc(UINT8 type)
{
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(".........memp_malloc....... type = 0x%x\n"), type);						
    return CSW_TCPIP_MALLOC( MEM_ALIGN_SIZE(memp_sizes[type]) );
}


void memp_free(UINT8 type, void *mem)
{
    type = type;
    
    CSW_TCPIP_FREE(mem);

    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(".........memp_free....... type = 0x%x\n"), type);		
    return ;
}

#endif

//add by zouy;    
void *msg_memp_malloc(UINT8 type,UINT8 len)
{
    //return CSW_MALLOC( memp_sizes[type] + len );    //Updated by Qi
    return 0;
}

void free_sxr_memp()
{
    if(memp_memory != NULL)
        CSW_TCPIP_FREE (memp_memory);
    if(mutex != SYS_SEM_NULL)
    {    
        sys_sem_free(mutex);
        mutex = SYS_SEM_NULL ;
    }
}

#endif

