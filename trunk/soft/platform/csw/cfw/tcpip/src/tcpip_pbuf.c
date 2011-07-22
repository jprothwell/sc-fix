/**
 * @file
 * Packet buffer management
 *
 * Packets are built from the pbuf data structure. It supports dynamic
 * memory allocation for packet contents or can reference externally
 * managed packet contents both in RAM and ROM. Quick allocation for
 * incoming packets is provided through pools with fixed sized pbufs.
 *
 * A packet may span over multiple pbufs, chained as a singly linked
 * list. This is called a "pbuf chain".
 *
 * Multiple packets may be queued, also using this singly linked list.
 * This is called a "packet queue".
 * 
 * So, a packet queue consists of one or more pbuf chains, each of
 * which consist of one or more pbufs. Currently, queues are only
 * supported in a limited section of lwIP, this is the etharp queueing
 * code. Outside of this section no packet queues are supported yet.
 * 
 * The differences between a pbuf chain and a packet queue are very
 * precise but subtle. 
 *
 * The last pbuf of a packet has a ->tot_len field that equals the
 * ->len field. It can be found by traversing the list. If the last
 * pbuf of a packet has a ->next field other than NULL, more packets
 * are on the queue.
 *
 * Therefore, looping through a pbuf of a single packet, has an
 * loop end condition (tot_len == p->len), NOT (next == NULL).
 */

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

//#include <string.h>
#ifdef CFW_TCPIP_SUPPORT
#include "tcpip_opt.h"
#include "tcpip_sys.h"
//#include <csw.h>
#include "tcpip_stats.h"
#include "tcpip_def.h"
#include "tcpip_mem.h"
//#include "tcpip_memp.h"
#include "tcpip_pbuf.h"
//#include "perf.h"  //temp by zouy

static UINT8 *pbuf_pool_memory;

#if !SYS_LIGHTWEIGHT_PROT
static volatile UINT8 pbuf_pool_free_lock, pbuf_pool_alloc_lock;
static sys_sem_t pbuf_pool_free_sem;
#endif

static struct pbuf *pbuf_pool = NULL;

/**
 * Initializes the pbuf module.
 *
 * A large part of memory is allocated for holding the pool of pbufs.
 * The size of the individual pbufs in the pool is given by the size
 * parameter, and the number of pbufs in the pool by the num parameter.
 *
 * After the memory has been allocated, the pbufs are set up. The
 * ->next pointer in each pbuf is set up to point to the next pbuf in
 * the pool.
 *
 */
//VOID SYS_ARCH_DECL_PROTECT(lev)  {  lev;};
//VOID SYS_ARCH_PROTECT(lev)    {  lev;};
//VOID  SYS_ARCH_UNPROTECT(lev)  {  lev;};


VOID pbuf_init(VOID)
{
    struct pbuf *p, *q = NULL;
    UINT16 i;
	pbuf_pool_memory =CSW_TCPIP_MALLOC(MEM_ALIGNMENT - 1 + (PBUF_POOL_SIZE + 0) * MEM_ALIGN_SIZE(PBUF_POOL_BUFSIZE + sizeof(struct pbuf)));
    pbuf_pool = (struct pbuf *) MEM_ALIGN(pbuf_pool_memory);
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_pool_memory %p\n"), (VOID*)pbuf_pool_memory);
#if PBUF_STATS
    lwip_stats.pbuf.avail = PBUF_POOL_SIZE;
#endif /* PBUF_STATS */

    /* Set up ->next pointers to link the pbufs of the pool together */
    p = pbuf_pool;

    for (i = 0; i < PBUF_POOL_SIZE; ++i)
    {
        p->next = (struct pbuf *) ((UINT8 *) p + PBUF_POOL_BUFSIZE + sizeof(struct pbuf));
        p->len = p->tot_len = PBUF_POOL_BUFSIZE;
        p->payload = MEM_ALIGN((VOID *) ((UINT8 *) p + sizeof(struct pbuf)));
        p->flags = PBUF_FLAG_POOL;
        q = p;
        p = p->next;
    }

    /* The ->next pointer of last pbuf is NULL to indicate that there
       are no more pbufs in the pool */
    q->next = NULL;

#if !SYS_LIGHTWEIGHT_PROT
    pbuf_pool_alloc_lock = 0;
    pbuf_pool_free_lock = 0;
//    pbuf_pool_free_sem = sys_sem_new(1);
    pbuf_pool_free_sem = COS_CreateSemaphore(1);
#endif
}

/**
 * @internal only called from pbuf_alloc()
 */
 #if STATIC_MEMORY
static struct pbuf *pbuf_pool_alloc(VOID)
{
    struct pbuf *p = NULL;

    SYS_ARCH_DECL_PROTECT(old_level);
    SYS_ARCH_PROTECT(old_level);

#if !SYS_LIGHTWEIGHT_PROT
    /* Next, check the actual pbuf pool, but if the pool is locked, we
       pretend to be out of buffers and return NULL. */
    if (pbuf_pool_free_lock)
    {
#if PBUF_STATS
        ++lwip_stats.pbuf.alloc_locked;
#endif /* PBUF_STATS */
        return NULL;
    }
    pbuf_pool_alloc_lock = 1;
    if (!pbuf_pool_free_lock)
    {
#endif /* SYS_LIGHTWEIGHT_PROT */
        COS_WaitForSemaphore(pbuf_pool_free_sem, COS_WAIT_FOREVER);  //add wuys 2009-03-03      
        p = pbuf_pool;
        if (p)
        {
            pbuf_pool = p->next;
        }
       COS_ReleaseSemaphore(pbuf_pool_free_sem); //add wuys 2009-03-03
#if !SYS_LIGHTWEIGHT_PROT
#if PBUF_STATS
    }
    else
    {
        ++lwip_stats.pbuf.alloc_locked;
#endif /* PBUF_STATS */
    }
    pbuf_pool_alloc_lock = 0;
#endif /* SYS_LIGHTWEIGHT_PROT */

#if PBUF_STATS
    if (p != NULL)
    {
        ++lwip_stats.pbuf.used;
        if (lwip_stats.pbuf.used > lwip_stats.pbuf.max)
        {
            lwip_stats.pbuf.max = lwip_stats.pbuf.used;
        }
    }
#endif /* PBUF_STATS */

    SYS_ARCH_UNPROTECT(old_level);
    return p;
}
#else

static struct pbuf *
pbuf_pool_alloc(void)
{
    struct pbuf *p = NULL;

    p = (struct pbuf *)CSW_TCPIP_MALLOC( MEM_ALIGN_SIZE(PBUF_POOL_BUFSIZE + sizeof(struct pbuf)) );
    
    p->next = NULL;
    p->len = p->tot_len = PBUF_POOL_BUFSIZE;
    p->payload = MEM_ALIGN((void *)((UINT8 *)p + sizeof(struct pbuf)));
    p->flags = PBUF_FLAG_POOL;
   
    return p;
}

#endif


/**
 * Allocates a pbuf of the given type (possibly a chain for PBUF_POOL type).
 *
 * The actual memory allocated for the pbuf is determined by the
 * layer at which the pbuf is allocated and the requested size
 * (from the size parameter).
 *
 * @param flag this parameter decides how and where the pbuf
 * should be allocated as follows:
 *
 * - PBUF_RAM: buffer memory for pbuf is allocated as one large
 *             chunk. This includes protocol headers as well.
 * - PBUF_ROM: no buffer memory is allocated for the pbuf, even for
 *             protocol headers. Additional headers must be prepended
 *             by allocating another pbuf and chain in to the front of
 *             the ROM pbuf. It is assumed that the memory used is really
 *             similar to ROM in that it is immutable and will not be
 *             changed. Memory which is dynamic should generally not
 *             be attached to PBUF_ROM pbufs. Use PBUF_REF instead.
 * - PBUF_REF: no buffer memory is allocated for the pbuf, even for
 *             protocol headers. It is assumed that the pbuf is only
 *             being used in a single thread. If the pbuf gets queued,
 *             then pbuf_take should be called to copy the buffer.
 * - PBUF_POOL: the pbuf is allocated as a pbuf chain, with pbufs from
 *              the pbuf pool that is allocated during pbuf_init().
 *
 * @return the allocated pbuf. If multiple pbufs where allocated, this
 * is the first pbuf of a pbuf chain.
 */

struct pbuf *pbuf_alloc(UINT8 l, UINT16 length, UINT8 flag)
{
    struct pbuf *p, *q, *r;
    UINT16 offset;
    INT32 rem_len;        /* remaining length */
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_alloc(length=0x%x)(l=0x%x)(flag=0x%x)\n"), length, l, flag);

    /* determine header offset */
    offset = 0;
    switch (l)
    {
    case PBUF_TRANSPORT:
        /* add room for transport (often TCP) layer header */
        offset += PBUF_TRANSPORT_HLEN;
        /* FALLTHROUGH */
    case PBUF_IP:
        /* add room for IP layer header */
        offset += PBUF_IP_HLEN;
        /* FALLTHROUGH */
    case PBUF_LINK:
        /* add room for link layer header */
        offset += PBUF_LINK_HLEN;
        break;
    case PBUF_RAW:
        break;
    default:
        LWIP_ASSERT("pbuf_alloc: bad pbuf layer", 0);
        return NULL;
    }

    switch (flag)
    {
    case PBUF_POOL:
	
        /* allocate head of pbuf chain into p */
        p = pbuf_pool_alloc();
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_alloc: allocated pbuf %p\n"), (VOID *) p);
        if (p == NULL)
        {
#if PBUF_STATS
            ++lwip_stats.pbuf.err;
#endif /* PBUF_STATS */
            return NULL;
        }
        p->next = NULL;

        /* make the payload pointer point 'offset' bytes into pbuf data memory */
        p->payload = MEM_ALIGN((VOID *) ((UINT8 *) p + (sizeof(struct pbuf) + offset)));
        LWIP_ASSERT("pbuf_alloc: pbuf p->payload properly aligned", ((mem_ptr_t) p->payload % MEM_ALIGNMENT) == 0);
        /* the total length of the pbuf chain is the requested size */
        p->tot_len = length;
        /* set the length of the first pbuf in the chain */
        //p->len = length > PBUF_POOL_BUFSIZE - offset ? PBUF_POOL_BUFSIZE - offset : length;
            p->len = length > PBUF_POOL_BUFSIZE - MEM_ALIGN_SIZE(offset)? PBUF_POOL_BUFSIZE - MEM_ALIGN_SIZE(offset): length;
        /* set reference count (needed here in case we fail) */
        p->ref = 1;

        /* now allocate the tail of the pbuf chain */

        /* remember first pbuf for linkage in next iteration */
        r = p;
        /* remaining length to be allocated */
        rem_len = length - p->len;
        /* any remaining pbufs to be allocated? */
        while (rem_len > 0)
        {
            q = pbuf_pool_alloc();
            if (q == NULL)
            {
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_alloc: Out of pbufs in pool.\n"));
#if PBUF_STATS
                ++lwip_stats.pbuf.err;
#endif /* PBUF_STATS */
                /* free chain so far allocated */
                pbuf_free(p);
                /* bail out unsuccesfully */
                return NULL;
            }
            q->next = NULL;
            /* make previous pbuf point to this pbuf */
            r->next = q;
            /* set total length of this pbuf and next in chain */
            q->tot_len = rem_len;
            /* this pbuf length is pool size, unless smaller sized tail */
            q->len = rem_len > PBUF_POOL_BUFSIZE ? PBUF_POOL_BUFSIZE : rem_len;
            q->payload = (VOID *) ((UINT8 *) q + sizeof(struct pbuf));
            LWIP_ASSERT("pbuf_alloc: pbuf q->payload properly aligned", ((mem_ptr_t) q->payload % MEM_ALIGNMENT) == 0);
            q->ref = 1;
            /* calculate remaining length to be allocated */
            rem_len -= q->len;
            /* remember this pbuf for linkage in next iteration */
            r = q;
        }
        /* end of chain */
        /*r->next = NULL; */

        break;
    case PBUF_RAM:
        /* If pbuf is to be allocated in RAM, allocate memory for it. */
        p = mem_malloc(MEM_ALIGN_SIZE(sizeof(struct pbuf) + offset) + MEM_ALIGN_SIZE(length));
        if (p == NULL)
        {
            return NULL;
        }
        /* Set up internal structure of the pbuf. */
        p->payload = MEM_ALIGN((VOID *) ((UINT8 *) p + sizeof(struct pbuf) + offset));
        p->len = p->tot_len = length;
        p->next = NULL;
        p->flags = PBUF_FLAG_RAM;

        LWIP_ASSERT("pbuf_alloc: pbuf->payload properly aligned", ((mem_ptr_t) p->payload % MEM_ALIGNMENT) == 0);
        break;
        /* pbuf references existing (non-volatile static constant) ROM payload? */
    case PBUF_ROM:
        /* pbuf references existing (externally allocated) RAM payload? */
    case PBUF_REF:
        /* only allocate memory for the pbuf structure */
		CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("PBUF_ROM REF\n"));
        p = memp_malloc(MEMP_PBUF);
        if (p == NULL)
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_alloc: Could not allocate MEMP_PBUF for PBUF_0x%x.\n"), flag);
            return NULL;
        }
        /* caller must set this field properly, afterwards */
        p->payload = NULL;
        p->len = p->tot_len = length;
        p->next = NULL;
        p->flags = (flag == PBUF_ROM ? PBUF_FLAG_ROM : PBUF_FLAG_REF);
        break;
    default:
	 CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_alloc: erroneous flag\n"));
        LWIP_ASSERT("pbuf_alloc: erroneous flag", 0);
        return NULL;
    }
    /* set reference count */
    p->ref = 1;
 //   CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf____   alloc(length=0x%x) == %p\n"), length, (VOID *) p);	
    return p;
}


#if PBUF_STATS
#define DEC_PBUF_STATS do { --lwip_stats.pbuf.used; } while (0)
#else /* PBUF_STATS */
#define DEC_PBUF_STATS
#endif /* PBUF_STATS */

#if STATIC_MEMORY
#define PBUF_POOL_FAST_FREE(p)  do {                                    \
                                  p->next = pbuf_pool;                  \
                                  pbuf_pool = p;                        \
                                  DEC_PBUF_STATS;                       \
                                } while (0)
#else

#define PBUF_POOL_FAST_FREE(p)  CSW_TCPIP_FREE(p)

#endif


#if SYS_LIGHTWEIGHT_PROT
#define PBUF_POOL_FREE(p)  do {                                         \
                                SYS_ARCH_DECL_PROTECT(old_level);       \
                                SYS_ARCH_PROTECT(old_level);            \
                                PBUF_POOL_FAST_FREE(p);                 \
                                SYS_ARCH_UNPROTECT(old_level);          \
                               } while (0)
#else /* SYS_LIGHTWEIGHT_PROT */
#define PBUF_POOL_FREE(p)  do {                                         \
                             COS_WaitForSemaphore(pbuf_pool_free_sem, COS_WAIT_FOREVER);          \
                             PBUF_POOL_FAST_FREE(p);                    \
                             COS_ReleaseSemaphore(pbuf_pool_free_sem);        \
                           } while (0)
#endif /* SYS_LIGHTWEIGHT_PROT */

/**
 * Shrink a pbuf chain to a desired length.
 *
 * @param p pbuf to shrink.
 * @param new_len desired new length of pbuf chain
 *
 * Depending on the desired length, the first few pbufs in a chain might
 * be skipped and left unchanged. The new last pbuf in the chain will be
 * resized, and any remaining pbufs will be freed.
 *
 * @note If the pbuf is ROM/REF, only the ->tot_len and ->len fields are adjusted.
 * @note May not be called on a packet queue.
 *
 * @bug Cannot grow the size of a pbuf (chain) (yet).
 */
VOID pbuf_realloc(struct pbuf *p, UINT16 new_len)
{
    struct pbuf *q;
    UINT16 rem_len;        /* remaining length */
    INT32 grow;

    CSW_TRACE(CFW_TCPIP_TS_ID_SXS , TSTXT("pbuf_realloc: sane p->flags"), p->flags == PBUF_FLAG_POOL || p->flags == PBUF_FLAG_ROM || p->flags == PBUF_FLAG_RAM || p->flags == PBUF_FLAG_REF);

    /* desired length larger than current length? */
    if (new_len >= p->tot_len)
    {
        /* enlarging not yet supported */
        return;
    }

    /* the pbuf chain grows by (new_len - p->tot_len) bytes
     * (which may be negative in case of shrinking) */
    grow = new_len - p->tot_len;

    /* first, step over any pbufs that should remain in the chain */
    rem_len = new_len;
    q = p;
    /* should this pbuf be kept? */
    while (rem_len > q->len)
    {
        /* decrease remaining length by pbuf length */
        rem_len -= q->len;
        /* decrease total length indicator */
        q->tot_len += grow;
        /* proceed to next pbuf in chain */
        q = q->next;
    }
    /* we have now reached the new last pbuf (in q) */
    /* rem_len == desired length for pbuf q */

    /* shrink allocated memory for PBUF_RAM */
    /* (other types merely adjust their length fields */
    if ((q->flags == PBUF_FLAG_RAM) && (rem_len != q->len))
    {
        /* reallocate and adjust the length of the pbuf that will be split */
        mem_realloc(q, (UINT8 *) q->payload - (UINT8 *) q + rem_len);
    }
    /* adjust length fields for new last pbuf */
    q->len = rem_len;
    q->tot_len = q->len;

    /* any remaining pbufs in chain? */
    if (q->next != NULL)
    {
        /* free remaining pbufs in chain */
        pbuf_free(q->next);
    }
    /* q is last packet in chain */
    q->next = NULL;

}

/**
 * Adjusts the payload pointer to hide or reveal headers in the payload.
 *
 * Adjusts the ->payload pointer so that space for a header
 * (dis)appears in the pbuf payload.
 *
 * The ->payload, ->tot_len and ->len fields are adjusted.
 *
 * @param hdr_size_inc Number of bytes to increment header size which
 * increases the size of the pbuf. New space is on the front.
 * (Using a negative value decreases the header size.)
 * If hdr_size_inc is 0, this function does nothing and returns succesful.
 *
 * PBUF_ROM and PBUF_REF type buffers cannot have their sizes increased, so
 * the call will fail. A check is made that the increase in header size does
 * not move the payload pointer in front of the start of the buffer.
 * @return non-zero on failure, zero on success.
 *
 */
UINT8 pbuf_header(struct pbuf *p, INT16 header_size_increment)
{
    VOID *payload;

    LWIP_ASSERT("p != NULL", p != NULL);
    if ((header_size_increment == 0) || (p == NULL))
        return 0;
      UINT16 increment_magnitude;
      if (header_size_increment < 0)
  	{
            increment_magnitude = -header_size_increment;
                  /* Check that we aren't going to move off the end of the pbuf */
                  LWIP_ASSERT("increment_magnitude <= p->len", increment_magnitude <= p->len);
      } 
	else 
	{
    increment_magnitude = header_size_increment;
#if 0 /* Can't assert these as some callers speculatively call
         pbuf_header() to see if it's OK.  Will return 1 below instead. */
    /* Check that we've got the correct type of pbuf to work with */
    LWIP_ASSERT("p->flags == PBUF_FLAG_RAM || p->flags == PBUF_FLAG_POOL", 
                p->flags == PBUF_FLAG_RAM || p->flags == PBUF_FLAG_POOL);
    /* Check that we aren't going to move off the beginning of the pbuf */
    LWIP_ASSERT("p->payload - increment_magnitude >= p + sizeof(struct pbuf)",
                (u8_t *)p->payload - increment_magnitude >= (u8_t *)p + sizeof(struct pbuf));
#endif
      }



    /* remember current payload pointer */
    payload = p->payload;

    /* pbuf types containing payloads? */
    if (p->flags == PBUF_FLAG_RAM || p->flags == PBUF_FLAG_POOL)
    {
        /* set new payload pointer */
        p->payload = (UINT8 *) p->payload - header_size_increment;
        /* boundary check fails? */
        if ((UINT8 *) p->payload < (UINT8 *) p + sizeof(struct pbuf))
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_header: failed as %p < %p (not enough space for new header size)\n"), (VOID *) p->payload, (VOID *) (p + 1));
            /* restore old payload pointer */
            p->payload = payload;
            /* bail out unsuccesfully */
            return 1;
        }
        /* pbuf types refering to external payloads? */
    }
    else if (p->flags == PBUF_FLAG_REF || p->flags == PBUF_FLAG_ROM)
    {
        /* hide a header in the payload? */
            if ((header_size_increment < 0) && (increment_magnitude <= p->len))
        {
            /* increase payload pointer */
            p->payload = (UINT8 *) p->payload - header_size_increment;
        }
        else
        {
            /* cannot expand payload to front (yet!)
             * bail out unsuccesfully */
            return 1;
        }
  }
  else 
  {
    /* Unknown type */
    LWIP_ASSERT("bad pbuf flag type", 0);
    return 1;
		
    }
	
    /* modify pbuf length fields */
    p->len += header_size_increment;
    p->tot_len += header_size_increment;

    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_header: old %p new %p (0x%x)\n"), (VOID *) payload, (VOID *) p->payload, header_size_increment);

    return 0;
}

/**
 * Dereference a pbuf chain or queue and deallocate any no-longer-used
 * pbufs at the head of this chain or queue.
 *
 * Decrements the pbuf reference count. If it reaches zero, the pbuf is
 * deallocated.
 *
 * For a pbuf chain, this is repeated for each pbuf in the chain,
 * up to the first pbuf which has a non-zero reference count after
 * decrementing. So, when all reference counts are one, the whole
 * chain is free'd.
 *
 * @param pbuf The pbuf (chain) to be dereferenced.
 *
 * @return the number of pbufs that were de-allocated
 * from the head of the chain.
 *
 * @note MUST NOT be called on a packet queue (Not verified to work yet).
 * @note the reference counter of a pbuf equals the number of pointers
 * that refer to the pbuf (or into the pbuf).
 *
 * @internal examples:
 *
 * Assuming existing chains a->b->c with the following reference
 * counts, calling pbuf_free(a) results in:
 * 
 * 1->2->3 becomes ...1->3
 * 3->3->3 becomes 2->3->3
 * 1->1->2 becomes ......1
 * 2->1->1 becomes 1->1->1
 * 1->1->1 becomes .......
 *
 */
UINT8 pbuf_free(struct pbuf * p)
{
    struct pbuf *q;
    UINT8 count;
    SYS_ARCH_DECL_PROTECT(old_level); 

    LWIP_ASSERT("p != NULL", p != NULL);
    /* if assertions are disabled, proceed with debug output */
    if (p == NULL)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_free(p == NULL) was called.\n"));
        return 0;
    }
    //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_free(%p)\n"), (VOID *) p);

    PERF_START;

    LWIP_ASSERT("pbuf_free: sane flags", p->flags == PBUF_FLAG_RAM || p->flags == PBUF_FLAG_ROM || p->flags == PBUF_FLAG_REF || p->flags == PBUF_FLAG_POOL);

    count = 0;
    /* Since decrementing ref cannot be guaranteed to be a single machine operation
     * we must protect it. Also, the later test of ref must be protected.
     */
    SYS_ARCH_PROTECT(old_level);
    /* de-allocate all consecutive pbufs from the head of the chain that
     * obtain a zero reference count after decrementing*/
    while (p != NULL)
    {
         UINT16  ref;

        /* all pbufs in a chain are referenced at least once */
        LWIP_ASSERT("pbuf_free: p->ref > 0", p->ref > 0);
        /* decrease reference count (number of pointers to pbuf) */
        //p->ref--;
            ref = --(p->ref);

        /* this pbuf is no longer referenced to? */
        if (ref == 0)
        {
            /* remember next pbuf in chain for next iteration */
            q = p->next;
            //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_free: deallocating %p\n"), (VOID *) p);
            /* is this a pbuf from the pool? */
            if (p->flags == PBUF_FLAG_POOL)
            {
                p->len = p->tot_len = PBUF_POOL_BUFSIZE;
                p->payload = (VOID *) ((UINT8 *) p + sizeof(struct pbuf));
                PBUF_POOL_FREE(p);
                /* is this a ROM or RAM referencing pbuf? */
            }
            else if (p->flags == PBUF_FLAG_ROM || p->flags == PBUF_FLAG_REF)
            {
                memp_free(MEMP_PBUF, p);
                /* p->flags == PBUF_FLAG_RAM */
            }
            else
            {
                mem_free(p);
            }
            count++;
            /* proceed to next pbuf */
            p = q;
            /* p->ref > 0, this pbuf is still referenced to */
            /* (and so the remaining pbufs in chain as well) */
        }
        else
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_free: %p has ref 0x%x, ending here.\n"), (VOID *) p, (UINT16) ref);
            /* stop walking through the chain */
            p = NULL;
        }
    }
    SYS_ARCH_UNPROTECT(old_level);
    PERF_STOP("pbuf_free");
    /* return number of de-allocated pbufs */
    return count;
}

/**
 * Count number of pbufs in a chain
 *
 * @param p first pbuf of chain
 * @return the number of pbufs in a chain
 */

UINT8 pbuf_clen(struct pbuf * p)
{
    UINT8 len;

    len = 0;
    while (p != NULL)
    {
        ++len;
        p = p->next;
    }
    return len;
}

/**
 * Increment the reference count of the pbuf.
 *
 * @param p pbuf to increase reference counter of
 *
 */



VOID pbuf_ref(struct pbuf *p)
{
    SYS_ARCH_DECL_PROTECT(old_level);
    /* pbuf given? */
    if (p != NULL)
    {
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("call  pbuf_ref  function %p\n"), p);    
        SYS_ARCH_PROTECT(old_level);
        ++(p->ref);
        SYS_ARCH_UNPROTECT(old_level);
    }
}

/**
 * Concatenate two pbufs (each may be a pbuf chain) and take over
 * the caller's reference of the tail pbuf.
 * 
 * @note The caller MAY NOT reference the tail pbuf afterwards.
 * Use pbuf_chain() for that purpose.
 * 
 * @see pbuf_chain()
 */

VOID pbuf_cat(struct pbuf *h, struct pbuf *t)
{
    struct pbuf *p;

    LWIP_ASSERT("h != NULL (programmer violates API)", h != NULL);
    LWIP_ASSERT("t != NULL (programmer violates API)", t != NULL);
    if ((h == NULL) || (t == NULL))
        return;

    /* proceed to last pbuf of chain */
    for (p = h; p->next != NULL; p = p->next)
    {
        /* add total length of second chain to all totals of first chain */
        p->tot_len += t->tot_len;
    }
    /* { p is last pbuf of first h chain, p->next == NULL } */
    LWIP_ASSERT("p->tot_len == p->len (of last pbuf in chain)", p->tot_len == p->len);
    LWIP_ASSERT("p->next == NULL", p->next == NULL);
    /* add total length of second chain to last pbuf total of first chain */
    p->tot_len += t->tot_len;
    /* chain last pbuf of head (p) with first of tail (t) */
    p->next = t;
    /* p->next now references t, but the caller will drop its reference to t,
     * so netto there is no change to the reference count of t.
     */
}

/**
 * Chain two pbufs (or pbuf chains) together.
 * 
 * The caller MUST call pbuf_free(t) once it has stopped
 * using it. Use pbuf_cat() instead if you no longer use t.
 * 
 * @param h head pbuf (chain)
 * @param t tail pbuf (chain)
 * @note The pbufs MUST belong to the same packet.
 * @note MAY NOT be called on a packet queue.
 *
 * The ->tot_len fields of all pbufs of the head chain are adjusted.
 * The ->next field of the last pbuf of the head chain is adjusted.
 * The ->ref field of the first pbuf of the tail chain is adjusted.
 *
 */
VOID pbuf_chain(struct pbuf *h, struct pbuf *t)
{
    pbuf_cat(h, t);
    /* t is now referenced by h */
    pbuf_ref(t);
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_chain: %p references %p\n"), (VOID *) h, (VOID *) t);
}

/* For packet queueing. Note that queued packets MUST be dequeued first
 * using pbuf_dequeue() before calling other pbuf_() functions. */
#if ARP_QUEUEING
/**
 * Add a packet to the end of a queue.
 *
 * @param q pointer to first packet on the queue
 * @param n packet to be queued
 *
 * Both packets MUST be given, and must be different.
 */
VOID pbuf_queue(struct pbuf *p, struct pbuf *n)
{
#if PBUF_DEBUG            /* remember head of queue */
    struct pbuf *q = p;
#endif
    /* programmer stupidity checks */
    LWIP_ASSERT("p == NULL in pbuf_queue: this indicates a programmer error\n", p != NULL);
    LWIP_ASSERT("n == NULL in pbuf_queue: this indicates a programmer error\n", n != NULL);
    LWIP_ASSERT("p == n in pbuf_queue: this indicates a programmer error\n", p != n);
    if ((p == NULL) || (n == NULL) || (p == n))
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_queue: programmer argument error\n"));
        return;
    }

    /* iterate through all packets on queue */
    while (p->next != NULL)
    {
/* be very picky about pbuf chain correctness */
#if PBUF_DEBUG
        /* iterate through all pbufs in packet */
        while (p->tot_len != p->len)
        {
            /* make sure invariant condition holds */
            LWIP_ASSERT("p->len < p->tot_len", p->len < p->tot_len);
            /* make sure each packet is complete */
            LWIP_ASSERT("p->next != NULL", p->next != NULL);
            p = p->next;
            /* { p->tot_len == p->len => p is last pbuf of a packet } */
        }
        /* { p is last pbuf of a packet } */
        /* proceed to next packet on queue */
#endif
        /* proceed to next pbuf */
        if (p->next != NULL)
            p = p->next;
    }
    /* { p->tot_len == p->len and p->next == NULL } ==>
     * { p is last pbuf of last packet on queue } */
    /* chain last pbuf of queue with n */
    p->next = n;
    /* n is now referenced to by the (packet p in the) queue */
    pbuf_ref(n);
#if PBUF_DEBUG
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_queue: newly queued packet %p sits after packet %p in queue %p\n"), (VOID *) n, (VOID *) p, (VOID *) q);
#endif
}

/**
 * Remove a packet from the head of a queue.
 *
 * The caller MUST reference the remainder of the queue (as returned). The
 * caller MUST NOT call pbuf_ref() as it implicitly takes over the reference
 * from p.
 * 
 * @param p pointer to first packet on the queue which will be dequeued.
 * @return first packet on the remaining queue (NULL if no further packets).
 *
 */
struct pbuf *pbuf_dequeue(struct pbuf *p)
{
    struct pbuf *q;
    LWIP_ASSERT("p != NULL", p != NULL);

    /* iterate through all pbufs in packet p */
    while (p->tot_len != p->len)
    {
        /* make sure invariant condition holds */
        LWIP_ASSERT("p->len < p->tot_len", p->len < p->tot_len);
        /* make sure each packet is complete */
        LWIP_ASSERT("p->next != NULL", p->next != NULL);
        p = p->next;
    }
    /* { p->tot_len == p->len } => p is the last pbuf of the first packet */
    /* remember next packet on queue in q */
    q = p->next;
    /* dequeue packet p from queue */
    p->next = NULL;
    /* any next packet on queue? */
    if (q != NULL)
    {
        /* although q is no longer referenced by p, it MUST be referenced by
         * the caller, who is maintaining this packet queue. So, we do not call
         * pbuf_free(q) here, resulting in an implicit pbuf_ref(q) for the caller. */
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_dequeue: first remaining packet on queue is %p\n"), (VOID *) q);
    }
    else
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_dequeue: no further packets on queue\n"));
    }
    return q;
}
#endif

/**
 *
 * Create PBUF_POOL (or PBUF_RAM) copies of PBUF_REF pbufs.
 *
 * Used to queue packets on behalf of the lwIP stack, such as
 * ARP based queueing.
 *
 * Go through a pbuf chain and replace any PBUF_REF buffers
 * with PBUF_POOL (or PBUF_RAM) pbufs, each taking a copy of
 * the referenced data.
 *
 * @note You MUST explicitly use p = pbuf_take(p);
 * The pbuf you give as argument, may have been replaced
 * by a (differently located) copy through pbuf_take()!
 *
 * @note Any replaced pbufs will be freed through pbuf_free().
 * This may deallocate them if they become no longer referenced.
 *
 * @param p Head of pbuf chain to process
 *
 * @return Pointer to head of pbuf chain
 */
struct pbuf *pbuf_take(struct pbuf *p)
{
    struct pbuf *q, *prev, *head;
    LWIP_ASSERT("pbuf_take: p != NULL\n", p != NULL);
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_take(%p)\n"), (VOID *) p);

    prev = NULL;
    head = p;
    /* iterate through pbuf chain */
    do
    {
        /* pbuf is of type PBUF_REF? */
        if (p->flags == PBUF_FLAG_REF)
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_take: encountered PBUF_REF %p\n"), (VOID *) p);
            /* allocate a pbuf (w/ payload) fully in RAM */
            /* PBUF_POOL buffers are faster if we can use them */
            if (p->len <= PBUF_POOL_BUFSIZE)
            {
                q = pbuf_alloc(PBUF_RAW, p->len, PBUF_POOL);
                if (q == NULL)
                {
                    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_take: Could not allocate PBUF_POOL\n"));
                }
            }
            else
            {
                /* no replacement pbuf yet */
                q = NULL;
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_take: PBUF_POOL too small to replace PBUF_REF\n"));
            }
            /* no (large enough) PBUF_POOL was available? retry with PBUF_RAM */
            if (q == NULL)
            {
                q = pbuf_alloc(PBUF_RAW, p->len, PBUF_RAM);
                if (q == NULL)
                {
                    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_take: Could not allocate PBUF_RAM\n"));
                }
            }
            /* replacement pbuf could be allocated? */
            if (q != NULL)
            {
                /* copy p to q */
                /* copy successor */
                q->next = p->next;
                /* remove linkage from original pbuf */
                p->next = NULL;
                /* remove linkage to original pbuf */
                if (prev != NULL)
                {
                    /* prev->next == p at this point */
                    LWIP_ASSERT("prev->next == p", prev->next == p);
                    /* break chain and insert new pbuf instead */
                    prev->next = q;
                    /* prev == NULL, so we replaced the head pbuf of the chain */
                }
                else
                {
                    head = q;
                }
                /* copy pbuf payload */
                SUL_MemCopy8(q->payload, p->payload, p->len);
                q->tot_len = p->tot_len;
                q->len = p->len;
                /* in case p was the first pbuf, it is no longer refered to by
                 * our caller, as the caller MUST do p = pbuf_take(p);
                 * in case p was not the first pbuf, it is no longer refered to
                 * by prev. we can safely free the pbuf here.
                 * (note that we have set p->next to NULL already so that
                 * we will not free the rest of the chain by accident.)
                 */
                pbuf_free(p);
                /* do not copy ref, since someone else might be using the old buffer */
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_take: replaced PBUF_REF %p with %p\n"), (VOID *) p, (VOID *) q);
                p = q;
            }
            else
            {
                /* deallocate chain */
                pbuf_free(head);
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_take: failed to allocate replacement pbuf for %p\n"), (VOID *) p);
                return NULL;
            }
            /* p->flags != PBUF_FLAG_REF */
        }
        else
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_take: skipping pbuf not of type PBUF_REF\n"));
        }
        /* remember this pbuf */
        prev = p;
        /* proceed to next pbuf in original chain */
        p = p->next;
    }
    while (p);
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_take: end of chain reached.\n"));

    return head;
}

/**
 * Dechains the first pbuf from its succeeding pbufs in the chain.
 *
 * Makes p->tot_len field equal to p->len.
 * @param p pbuf to dechain
 * @return remainder of the pbuf chain, or NULL if it was de-allocated.
 * @note May not be called on a packet queue.
 */
struct pbuf *pbuf_dechain(struct pbuf *p)
{
    struct pbuf *q;
    UINT8 tail_gone = 1;
    /* tail */
    q = p->next;
    /* pbuf has successor in chain? */
    if (q != NULL)
    {
        /* assert tot_len invariant: (p->tot_len == p->len + (p->next? p->next->tot_len: 0) */
        LWIP_ASSERT("p->tot_len == p->len + q->tot_len", q->tot_len == p->tot_len - p->len);
        /* enforce invariant if assertion is disabled */
        q->tot_len = p->tot_len - p->len;
        /* decouple pbuf from remainder */
        p->next = NULL;
        /* total length of pbuf p is its own length only */
        p->tot_len = p->len;
        /* q is no longer referenced by p, free it */
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_dechain: unreferencing %p\n"), (VOID *) q);
        tail_gone = pbuf_free(q);
        if (tail_gone > 0)
        {
            CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pbuf_dechain: deallocated %p (as it is no longer referenced)\n"), (VOID *) q);
        }
        /* return remaining tail or NULL if deallocated */
    }
    /* assert tot_len invariant: (p->tot_len == p->len + (p->next? p->next->tot_len: 0) */
    LWIP_ASSERT("p->tot_len == p->len", p->tot_len == p->len);
    return (tail_gone > 0 ? NULL : q);
}



void free_sxr_pbuf()
{
    if(pbuf_pool_memory != NULL)
    {
        CSW_TCPIP_FREE (pbuf_pool_memory);
    }
    if(pbuf_pool_free_sem != SYS_SEM_NULL)
    {
//        sys_sem_free(pbuf_pool_free_sem);
        COS_DeleteSemaphore(pbuf_pool_free_sem);
        pbuf_pool_free_sem = SYS_SEM_NULL;
    }
}

#endif

