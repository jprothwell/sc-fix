/**
 * lwip DNS resolver header file.

 * Author: Jim Pettinato 
 *   April 2007

 * ported from uIP resolv.c Copyright (c) 2002-2003, Adam Dunkels.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifdef CFW_TCPIP_SUPPORT
#ifndef RESOLV_H
#define RESOLV_H
#include "tcpip_err.h"
/*The maximum length of a host name supported in the name table.*/
#define MAX_NAME_LENGTH 101//32 modified by wuys 2008-05-21
/* The maximum number of retries when asking for a name. */
#define MAX_RETRIES  10// 4 modify 2010-01-18

/*The maximum number of table entries to maintain locally*/
#ifndef LWIP_RESOLV_ENTRIES
#define LWIP_RESOLV_ENTRIES  6//old 4  
#endif

#ifndef DNS_SERVER_PORT
#define DNS_SERVER_PORT 53
#endif


/*The DNS message header*/
typedef struct s_dns_hdr {
  UINT16 id;
  UINT8 flags1, flags2;
#define DNS_FLAG1_RESPONSE        0x80
#define DNS_FLAG1_OPCODE_STATUS   0x10
#define DNS_FLAG1_OPCODE_INVERSE  0x08
#define DNS_FLAG1_OPCODE_STANDARD 0x00
#define DNS_FLAG1_AUTHORATIVE     0x04
#define DNS_FLAG1_TRUNC           0x02
#define DNS_FLAG1_RD              0x01
#define DNS_FLAG2_RA              0x80
#define DNS_FLAG2_ERR_MASK        0x0f
#define DNS_FLAG2_ERR_NONE        0x00
#define DNS_FLAG2_ERR_NAME        0x03
  UINT16 numquestions;
  UINT16 numanswers;
  UINT16 numauthrr;
  UINT16 numextrarr;
} DNS_HDR;

/* The DNS answer message structure */
typedef struct s_dns_answer {
  /* DNS answer record starts with either a domain name or a pointer
     to a name already present somewhere in the packet. */
  UINT16 type;
  UINT16 class;
  UINT16 ttl[2];
  UINT16 len;
  UINT16 ipaddr[2];
} DNS_ANSWER;

/* enumerated list of possible result values returned by gethostname() */
//remove to CFW.h


/* Functions. */
typedef struct namemap {
#define STATE_UNUSED 0
#define STATE_NEW    1
#define STATE_ASKING 2
#define STATE_DONE   3
#define STATE_ERROR  4
  UINT8 state;
  UINT8 tmr;
  UINT8 retries;
  UINT8 seqno;
  INT8 get_times;
  UINT8 err;
  INT16 simIdCid;
  char name[MAX_NAME_LENGTH];
  UINT32 ipaddr;
  void (* found)(char *name, struct ip_addr* ipaddr); /* pointer to callback on DNS query done */
}DNS_TABLE_ENTRY;


/* gethostbyname() - Returns immediately with one of RESOLV_RESULT return codes.
 *                   Return value will be RESOLV_COMPLETE if hostName is a valid *
 *                   IP address string or the host name is already in the local  *
 *                   names table. Returns RESOLV_REQUEST_QUEUED and queues a     *
 *                   request to be sent to the DNS server for resolution if no   *
 *                   errors are present.                                         */

/* resolv_found_func() - Callback which is invoked when a hostname is found.

 * This function should be implemented by the application using the DNS resolver.
 *  param 'name'   - pointer to the name that was looked up.
 *  param 'ipaddr' - pointer to a struct ip_addr containing the IP address of the
 *                   hostname, or NULL if the name could not be found.
*/

err_t resolv_init(const INT8 * cp);  /* initializes the resolver */
void resolv_tmr(void); /* handles requests, retries and timeouts - call every tcp_slowtmr tick */
UINT32 resolv_getserver(void);    /* returns configured DNS server IP addr */
UINT32 resolv_lookup(char *name, UINT8* index); /* returns IP for host 'name' only if already in table */
void resolv_query(char *name, void (*found)(char *name, struct ip_addr *addr),UINT16 simIdCid);
UINT32 getDnsServer(void);
  /* activates request for named host which will be processed on next timer tick */

#endif /* RESOLV_H */
#endif
