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

/* inet.c
 *
 * Functions common to all TCP/IP modules, such as the Internet checksum and the
 * byte order functions.
 *
 */

#ifdef CFW_TCPIP_SUPPORT
#include "tcpip_opt.h"
//#include <cswtype.h>
#include <csw.h>

#include "tcpip_arch.h"

#include "tcpip_def.h"
#include "tcpip_inet.h"

#include "tcpip_sys.h"

#define BYTE_ORDER LITTLE_ENDIAN  //temp zouy

/* This is a reference implementation of the checksum algorithm, with the
 * aim of being simple, correct and fully portable. Checksumming is the
 * first thing you would want to optimize for your platform. You will
 * need to port it to your architecture and in your sys_arch.h:
 * 
 * #define LWIP_CHKSUM <your_checksum_routine> 
*/
#ifndef LWIP_CHKSUM
#define LWIP_CHKSUM lwip_standard_chksum


/**
 * lwip checksum
 *
 * @param dataptr points to start of data to be summed at any boundary
 * @param len length of data to be summed
 * @return host order (!) lwip checksum (non-inverted Internet sum) 
 *
 * @note accumulator size limits summable lenght to 64k
 * @note host endianess is irrelevant (p3 RFC1071)
 */
static UINT16 lwip_standard_chksum(VOID *dataptr, UINT16 len)
{
    UINT32 acc;
    UINT16 src;
    UINT8 *octetptr;

    acc = 0;
    /* dataptr may be at odd or even addresses */
    octetptr = (UINT8 *) dataptr;
    while (len > 1)
    {
        /* declare first octet as most significant
           thus assume network order, ignoring host order */
        src = (*octetptr) << 8;
        octetptr++;
        /* declare second octet as least significant */
        src |= (*octetptr);
        octetptr++;
        acc += src;
        len -= 2;
    }
    if (len > 0)
    {
        /* accumulate remaining octet */
        src = (*octetptr) << 8;
        acc += src;
    }
    /* add deferred carry bits */
    acc = (acc >> 16) + (acc & 0x0000ffffUL);
    if ((acc & 0xffff0000) != 0)
    {
        acc = (acc >> 16) + (acc & 0x0000ffffUL);
    }
    /* This maybe a little confusing: reorder sum using htons()
       instead of ntohs() since it has a little less call overhead.
       The caller must invert bits for Internet sum ! */
    return htons((UINT16) acc);
}

#endif

#if 0
/*
 * Curt McDowell
 * Broadcom Corp.
 * csm@broadcom.com
 *
 * IP checksum two bytes at a time with support for
 * unaligned buffer.
 * Works for len up to and including 0x20000.
 * by Curt McDowell, Broadcom Corp. 12/08/2005
 */

static UINT16 lwip_standard_chksum2(VOID *dataptr, INT32 len)
{
    UINT8 *pb = dataptr;
    UINT16 *ps, t = 0;
    UINT32 sum = 0;
    INT32 odd = ((UINT32) pb & 1);

    /* Get aligned to UINT16 */
    if (odd && len > 0)
    {
        ((UINT8 *) & t)[1] = *pb++;
        len--;
    }

    /* Add the bulk of the data */
    ps = (UINT16 *) pb;
    while (len > 1)
    {
        sum += *ps++;
        len -= 2;
    }

    /* Consume left-over byte, if any */
    if (len > 0)
        ((UINT8 *) & t)[0] = *(UINT8 *) ps;;

    /* Add end bytes */
    sum += t;

    /*  Fold 32-bit sum to 16 bits */
    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    /* Swap if alignment was odd */
    if (odd)
        sum = ((sum & 0xff) << 8) | ((sum & 0xff00) >> 8);

    return sum;
}

/**
 * An optimized checksum routine. Basically, it uses loop-unrolling on
 * the checksum loop, treating the head and tail bytes specially, whereas
 * the inner loop acts on 8 bytes at a time. 
 *
 * @arg start of buffer to be checksummed. May be an odd byte address.
 * @len number of bytes in the buffer to be checksummed.
 * 
 * @todo First argument type conflicts with generic checksum routine.
 * 
 * by Curt McDowell, Broadcom Corp. December 8th, 2005
 */

static UINT16 lwip_standard_chksum4(UINT8 * pb, INT32 len)
{
    UINT16 *ps, t = 0;
    UINT32 *pl;
    UINT32 sum = 0, tmp;
    /* starts at odd byte address? */
    INT32 odd = ((UINT32) pb & 1);

    if (odd && len > 0)
    {
        ((UINT8 *) & t)[1] = *pb++;
        len--;
    }

    ps = (UINT16 *) pb;

    if (((UINT32) ps & 3) && len > 1)
    {
        sum += *ps++;
        len -= 2;
    }

    pl = (UINT32 *) ps;

    while (len > 7)
    {
        tmp = sum + *pl++;    /* ping */
        if (tmp < sum)
            tmp++;    /* add back carry */

        sum = tmp + *pl++;    /* pong */
        if (sum < tmp)
            sum++;    /* add back carry */

        len -= 8;
    }

    /* make room in upper bits */
    sum = (sum >> 16) + (sum & 0xffff);

    ps = (UINT16 *) pl;

    /* 16-bit aligned word remaining? */
    while (len > 1)
    {
        sum += *ps++;
        len -= 2;
    }

    /* dangling tail byte remaining? */
    if (len > 0)        /* include odd byte */
        ((UINT8 *) & t)[0] = *(UINT8 *) ps;

    sum += t;        /* add end bytes */

    while (sum >> 16)    /* combine halves */
        sum = (sum >> 16) + (sum & 0xffff);

    if (odd)
        sum = ((sum & 0xff) << 8) | ((sum & 0xff00) >> 8);

    return sum;
}
#endif

/* inet_chksum_pseudo:
 *
 * Calculates the pseudo Internet checksum used by TCP and UDP for a pbuf chain.
 */

UINT16 inet_chksum_pseudo(struct pbuf * p, struct ip_addr * src, struct ip_addr * dest, UINT8 proto, UINT16 proto_len)
{
    UINT32 acc;
    struct pbuf *q;
    UINT8 swapped;

    acc = 0;
    swapped = 0;
    /* iterate through all pbuf in chain */
    for (q = p; q != NULL; q = q->next)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("inet_chksum_pseudo(): checksumming pbuf %p (has next %p) \n"), (VOID *) q, (VOID *) q->next);
        acc += LWIP_CHKSUM(q->payload, q->len);
        /*CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("inet_chksum_pseudo(): unwrapped lwip_chksum()=0x%x \n"), acc); */
        while (acc >> 16)
        {
            acc = (acc & 0xffffUL) + (acc >> 16);
        }
        if (q->len % 2 != 0)
        {
            swapped = 1 - swapped;
            acc = ((acc & 0xff) << 8) | ((acc & 0xff00UL) >> 8);
        }
        /*CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("inet_chksum_pseudo(): wrapped lwip_chksum()=0x%x \n"), acc); */
    }

    if (swapped)
    {
        acc = ((acc & 0xff) << 8) | ((acc & 0xff00UL) >> 8);
    }
    acc += (src->addr & 0xffffUL);
    acc += ((src->addr >> 16) & 0xffffUL);
    acc += (dest->addr & 0xffffUL);
    acc += ((dest->addr >> 16) & 0xffffUL);
    acc += (UINT32) htons((UINT16) proto);
    acc += (UINT32) htons(proto_len);

    while (acc >> 16)
    {
        acc = (acc & 0xffffUL) + (acc >> 16);
    }
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("inet_chksum_pseudo(): pbuf chain lwip_chksum()=0x%x\n"), acc);
    return (UINT16) ~ (acc & 0xffffUL);
}

/* inet_chksum:
 *
 * Calculates the Internet checksum over a portion of memory. Used primarely for IP
 * and ICMP.
 */

UINT16 inet_chksum(VOID *dataptr, UINT16 len)
{
    UINT32 acc;

    acc = LWIP_CHKSUM(dataptr, len);
    while (acc >> 16)
    {
        acc = (acc & 0xffff) + (acc >> 16);
    }
    return (UINT16) ~ (acc & 0xffff);
}

UINT16 inet_chksum_pbuf(struct pbuf * p)
{
    UINT32 acc;
    struct pbuf *q;
    UINT8 swapped;

    acc = 0;
    swapped = 0;
    for (q = p; q != NULL; q = q->next)
    {
        acc += LWIP_CHKSUM(q->payload, q->len);
        while (acc >> 16)
        {
            acc = (acc & 0xffffUL) + (acc >> 16);
        }
        if (q->len % 2 != 0)
        {
            swapped = 1 - swapped;
            acc = (acc & 0x00ffUL << 8) | (acc & 0xff00UL >> 8);
        }
    }

    if (swapped)
    {
        acc = ((acc & 0x00ffUL) << 8) | ((acc & 0xff00UL) >> 8);
    }
    return (UINT16) ~ (acc & 0xffffUL);
}

/* Here for now until needed in other places in lwIP */
#ifndef isascii
#define in_range(c, lo, up)  ((UINT8)c >= lo && (UINT8)c <= up)
#define isascii(c)           in_range(c, 0x20, 0x7f)
#define isdigit(c)           in_range(c, '0', '9')
#define isxdigit(c)          (isdigit(c) || in_range(c, 'a', 'f') || in_range(c, 'A', 'F'))
#define islower(c)           in_range(c, 'a', 'z')
#define isspace(c)           (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
#endif


INT8 *CFW_TcpipInet_ntoa(in_addr addr)
{
	return inet_ntoa(addr);
}

 /*
  * Ascii internet address interpretation routine.
  * The value returned is in network order.
  */

 /*  */

UINT32 CFW_TcpipInetAddr(const INT8 *cp)
{
	return inet_addr(cp);
}


 /* inet_addr */
UINT32 inet_addr(const INT8 *cp)
{
    in_addr val;

    if (inet_aton(cp, &val))
    {
        return (val.s_addr);
    }
    return (INADDR_NONE);
}

 /*
  * Check whether "cp" is a valid ascii representation
  * of an Internet address and convert to a binary address.
  * Returns 1 if the address is valid, 0 if not.
  * This replaces inet_addr, the return value from which
  * cannot distinguish between failure and a local broadcast address.
  */
 /*  */
 /* inet_aton */
 
INT8 inet_aton(const INT8 *cp, in_addr * addr)
{
    UINT32 val;
    INT32 base, n;
    INT8 c;
    UINT32 parts[4];
    UINT32 *pp = parts;

    c = *cp;
    for (;;)
    {
        /*
         * Collect number up to ``.''.
         * Values are specified as for C:
         * 0x=hex, 0=octal, isdigit=decimal.
         */
        if (!isdigit(c))
            return (0);
        val = 0;
        base = 10;
        if (c == '0')
        {
            c = *++cp;
            if (c == 'x' || c == 'X')
                base = 16, c = *++cp;
            else
                base = 8;
        }
        for (;;)
        {
            if (isdigit(c))
            {
                val = (val * base) + (INT16) (c - '0');
                c = *++cp;
            }
            else if (base == 16 && isxdigit(c))
            {
                val = (val << 4) | (INT16) (c + 10 - (islower(c) ? 'a' : 'A'));
                c = *++cp;
            }
            else
                break;
        }
        if (c == '.')
        {
            /*
             * Internet format:
             *  a.b.c.d
             *  a.b.c   (with c treated as 16 bits)
             *  a.b (with b treated as 24 bits)
             */
            if (pp >= parts + 3)
                return (0);
            *pp++ = val;
            c = *++cp;
        }
        else
            break;
    }
    /*
     * Check for trailing characters.
     */
    if (c != '\0' && (!isascii(c) || !isspace(c)))
        return (0);
    /*
     * Concoct the address according to
     * the number of parts specified.
     */
    n = pp - parts + 1;
    switch (n)
    {

    case 0:
        return (0);    /* initial nondigit */

    case 1:        /* a -- 32 bits */
        break;

    case 2:        /* a.b -- 8.24 bits */
        if (val > 0xffffff)
            return (0);
        val |= parts[0] << 24;
        break;

    case 3:        /* a.b.c -- 8.8.16 bits */
        if (val > 0xffff)
            return (0);
        val |= (parts[0] << 24) | (parts[1] << 16);
        break;

    case 4:        /* a.b.c.d -- 8.8.8.8 bits */
        if (val > 0xff)
            return (0);
        val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
        break;
    }
    if (addr)
        addr->s_addr = htonl(val);
    return (1);
}
/****************

********************/
INT8 check_ipa(const INT8 *cp)
{
    UINT32 val;
    INT32 base, n;
    INT8 c;
    UINT32 parts[4];
    UINT32 *pp = parts;

    c = *cp;
    for (;;)
    {
        /*
         * Collect number up to ``.''.
         * Values are specified as for C:
         * 0x=hex, 0=octal, isdigit=decimal.
         */
        if (!isdigit(c))
            return (0);
        val = 0;
        base = 10;
        if (c == '0')
        {
            c = *++cp;
            if (c == 'x' || c == 'X')
                base = 16, c = *++cp;
            else
                base = 8;
        }
        for (;;)
        {
            if (isdigit(c))
            {
                val = (val * base) + (INT16) (c - '0');
                c = *++cp;
            }
            else if (base == 16 && isxdigit(c))
            {
                val = (val << 4) | (INT16) (c + 10 - (islower(c) ? 'a' : 'A'));
                c = *++cp;
            }
            else
                break;
        }
        if (c == '.')
        {
            /*
             * Internet format:
             *  a.b.c.d
             *  a.b.c   (with c treated as 16 bits)
             *  a.b (with b treated as 24 bits)
             */
            if ((pp >= parts + 3)||(val>0xff))
                return (0);
            
            *pp++ = val;
            c = *++cp;
        }
        else
            break;
    }
    /*
     * Check for trailing characters.
     */
    if (c != '\0' && (!isascii(c) || !isspace(c)))
        return (0);
    /*
     * Concoct the address according to
     * the number of parts specified.
     */
    n = pp - parts + 1;
    if(n <4)
       return (0);
    
    switch (n)
    {

    case 0:
        return (0);    /* initial nondigit */

    case 1:        /* a -- 32 bits */
        break;

    case 2:        /* a.b -- 8.24 bits */
        if (val > 0xffffff)
            return (0);
        val |= parts[0] << 24;
        break;

    case 3:        /* a.b.c -- 8.8.16 bits */
        if (val > 0xffff)
            return (0);
        val |= (parts[0] << 24) | (parts[1] << 16);
        break;

    case 4:        /* a.b.c.d -- 8.8.8.8 bits */
        if (val > 0xff)
            return (0);
        val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
        break;
    }
  //  if (addr)
   //     addr->s_addr = htonl(val);
    return (1);
}

/* Convert numeric IP address into decimal dotted ASCII representation.
 * returns ptr to static buffer; not reentrant!
 */
INT8 *inet_ntoa(in_addr addr)
{
    static INT8 str[16];
    UINT32 s_addr = addr.s_addr;
    INT8 inv[3];
    INT8 *rp;
    UINT8 *ap;
    UINT8 rem;
    UINT8 n;
    UINT8 i;

    rp = str;
    ap = (UINT8 *) & s_addr;
    for (n = 0; n < 4; n++)
    {
        i = 0;
        do
        {
            rem = *ap % (UINT8) 10;
            *ap /= (UINT8) 10;
            inv[i++] = '0' + rem;
        }
        while (*ap);
		
        while (i--)
            *rp++ = inv[i];
        *rp++ = '.';
        ap++;
    }
    *--rp = 0;
    return str;
}

#ifndef BYTE_ORDER
#error BYTE_ORDER is not defined
#endif
#if BYTE_ORDER == LITTLE_ENDIAN

UINT16 htons(UINT16 n)
{
    return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

UINT16 ntohs(UINT16 n)
{
    return htons(n);
}

UINT32 htonl(UINT32 n)
{
    return ((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8) | ((n & 0xff000000) >> 24);
}

UINT32 ntohl(UINT32 n)
{
    return htonl(n);
}

#endif /* BYTE_ORDER == LITTLE_ENDIAN */

#endif


