////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2009, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: tcpip_resolv.c
// 
// DESCRIPTION:
// TODO: ...
// 
// REVISION HISTORY:
//    NAME              DATE                REMAKS
// Wuys      2008-1-1       Created initial version 1.0
//
////////////////////////////////////////////////////////////////////////////////
 
#ifdef CFW_TCPIP_SUPPORT
#include "cmn_defs.h"
#include <string.h>
#include "tcpip_sys.h"
#include "tcpip_stats.h"
#include "tcpip_udp.h"
#include "tcpip_netif.h"
#include "reg.h"
#include "tcpip_resolv.h"


static DNS_TABLE_ENTRY dns_table[LWIP_RESOLV_ENTRIES];
static struct udp_pcb *resolv_pcb = NULL; /* our UDP connection to DNS server */
static struct ip_addr serverIP;
static struct ip_addr Sec_serverIP;
static UINT8 initFlag;
static UINT8 seqno;
static UINT8 tcpip_resolv_timer_active = 0;

//add for clean the old entry, 2010-02-22
INT8 get_Times =0;
//add end

BOOL IsDnsPCB(struct udp_pcb *u_pcb)
{
   if(u_pcb==resolv_pcb)
   	return TRUE;
   else
   	return FALSE;
   
}

#ifdef CFW_MULTI_SIM
extern UINT32 CFW_SetResolvCid( UINT8 cid, UINT8 nSimId );
extern UINT32 CFW_getDnsServerbyPdp( UINT8 nCid, UINT8 nSimID );
#else
extern UINT32 CFW_SetResolvCid( UINT8 cid );
extern UINT32 CFW_getDnsServerbyPdp( UINT8 nCid );
#endif

extern VOID sys_post_event_to_APP(UINT32 nEventId, UINT32 nParam1, UINT32 nParam2, UINT8 nType, UINT8 nFlag, UINT16 reserved);


/*---------------------------------------------------------------------------
 *  parse_name() - walk through a compact encoded DNS name and return the end 
 *  of the name.
*---------------------------------------------------------------------------*/
static UINT8* parse_name(UINT8* query)
{
  UINT8 n;

  do
  {
    n = *query++;
    
    while(n > 0)
    {
      /*      printf("%c", *query);*/
      ++query;
      --n;
    };
  } while(*query != 0);

  return query + 1;
}


/*---------------------------------------------------------------------------
 * check_entries() - Runs through the list of names to see if there are any 
 * that have not yet been queried and, if so, sends out a query.
 *---------------------------------------------------------------------------*/
static void check_entries(void)
{
  register DNS_HDR *hdr;
  char *query, *nptr, *pHostname;
  static UINT8 i;
  static UINT8 n;
  register DNS_TABLE_ENTRY *pEntry;
  struct pbuf *p;
  BOOL bQuery = 0;

  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("Enter check_entries\n"));
  for(i = 0; i < LWIP_RESOLV_ENTRIES; ++i)
  {
    pEntry = &dns_table[i];
    if(pEntry->state == STATE_NEW || pEntry->state == STATE_ASKING) 
    {
      if(pEntry->state == STATE_ASKING) 
      {
        if(--pEntry->tmr == 0) 
        {
          if(++pEntry->retries == MAX_RETRIES)
          {
            pEntry->state = STATE_ERROR;
            if (pEntry->found) /* call specified callback function if provided */
              (*pEntry->found)(pEntry->name, NULL);
        #if 0//modify the parameter2 form "dns_table index to the name  " by wuys 2009-02-24
	     sys_post_event_to_APP(EV_CFW_DNS_RESOLV_ERR_IND, 0, i, RESOLV_NETWORK_ERROR, 0, 0);
	#else
	sys_post_event_to_APP(EV_CFW_DNS_RESOLV_ERR_IND, 0, (UINT32)pEntry->name, RESOLV_NETWORK_ERROR, 0,pEntry->simIdCid);

					
	#endif
	     CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" check_entries: resolve fail\n"));
            continue;
          }
          pEntry->tmr = pEntry->retries;
        } 
        else
        {
          /*  printf("Timer %d\n", pEntry->tmr);*/
          /* Its timer has not run out, so we move on to next
          entry. */
          continue;
        }
      } 
      else
      {
        pEntry->state = STATE_ASKING;
        pEntry->tmr = 1;
        pEntry->retries = 0;
      }
	  
      /* if here, we have either a new query or a retry on a previous query to process */
      p = pbuf_alloc(PBUF_TRANSPORT, sizeof(DNS_HDR)+MAX_NAME_LENGTH+5, PBUF_RAM);
      hdr = (DNS_HDR *)p->payload;
      memset(hdr, 0, sizeof(DNS_HDR));
      hdr->id = htons(i);
      hdr->flags1 = DNS_FLAG1_RD; 
      hdr->numquestions = htons(1);
      query = (char *)hdr + sizeof(DNS_HDR);
      pHostname = pEntry->name;
      --pHostname;
      /* Convert hostname into suitable query format. */
      do
      {
        ++pHostname;
        nptr = query;
        ++query;
        for(n = 0; *pHostname != '.' && *pHostname != 0; ++pHostname)
        {
          *query = *pHostname;
          ++query;
          ++n;
        }
        *nptr = n;
      } 
      while(*pHostname != 0);
	  
      {
        static unsigned char endquery[] = {0,0,1,0,1};
        memcpy(query, endquery, 5);
      }
      pbuf_realloc(p, sizeof(DNS_HDR)+strlen(pEntry->name)+6);
      udp_send(resolv_pcb, p);
	  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" check_entries: send dns query server IP: 0x%x\n"),serverIP.addr);
      pbuf_free(p);
      break;
    }
  }

  /* Because the timer is only effective for once, so we should restart it */
   #if 0 //2009-04-15, modify by wuys
  COS_SetTimerEX(BAL_TH(BAL_CFW_ADV_TASK_PRIORITY), PRV_CFW_DNS_TIMER_ID, 
                COS_TIMER_MODE_SINGLE, 2000 MILLI_SECOND);
  #else
    COS_SetTimerEX(BAL_TH(BAL_CFW_ADV_TASK_PRIORITY), PRV_CFW_DNS_TIMER_ID, 
                COS_TIMER_MODE_SINGLE, 3000 MILLI_SECOND);

  #endif
  
  /* if none of entry is in query state(STATE_NEW, STATE_ASKING), the timer
     should be killed */
  for (i = 0; i < LWIP_RESOLV_ENTRIES; i++)
  {
  	pEntry = &dns_table[i];
  	if (pEntry->state == STATE_NEW || pEntry->state == STATE_ASKING)
  	{
		bQuery = 1;
		break;
  	}
  }

  if (bQuery == 0)
  {
  	COS_KillTimerEX(BAL_TH(BAL_CFW_ADV_TASK_PRIORITY), PRV_CFW_DNS_TIMER_ID);
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" check_entries: kill timer\n"));
  }
}


/*---------------------------------------------------------------------------*
 *
 * Callback for DNS responses
 *
 *---------------------------------------------------------------------------*/
static void resolv_recv(void *s, struct udp_pcb *pcb, struct pbuf *p,
                                  struct ip_addr *addr, UINT16 port)
{
  INT8 *pHostname;
  DNS_ANSWER ans;
  DNS_HDR *hdr;
  static UINT16 nquestions, nanswers;
  static UINT16 i;
  register DNS_TABLE_ENTRY *pEntry;
  
  hdr = (DNS_HDR *)p->payload;
  
  /*  
      printf("ID %d\n", htons(hdr->id));
      printf("Query %d\n", hdr->flags1 & DNS_FLAG1_RESPONSE);
      printf("Error %d\n", hdr->flags2 & DNS_FLAG2_ERR_MASK);
      printf("Num questions %d, answers %d, authrr %d, extrarr %d\n",
      htons(hdr->numquestions),
      htons(hdr->numanswers),
      htons(hdr->numauthrr),
      htons(hdr->numextrarr));
      
  */
  

  /* The ID in the DNS header should be our entry into the name table. */
  i = ntohs(hdr->id);
  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" resolv_recv: i = %d, buf:\n"), i);

  SXS_DUMP(CFW_TCPIP_TS_ID_SXS, 0, p->payload, p->len);
  if(i >= LWIP_RESOLV_ENTRIES)
  {
  	pbuf_free(p);
  	return;
  }

  memset(&ans, 0, SIZEOF(DNS_ANSWER));
  
  pEntry = &dns_table[i];
  if( (i < LWIP_RESOLV_ENTRIES) && (pEntry->state == STATE_ASKING) )
  {
    /* This entry is now finished. */
    pEntry->state = STATE_DONE;
    pEntry->err = hdr->flags2 & DNS_FLAG2_ERR_MASK;

    /* Check for error. If so, call callback to inform. */
    if(pEntry->err != 0)
    {
      pEntry->state = STATE_ERROR;
      if (pEntry->found) /* call specified callback function if provided */
        (*pEntry->found)(pEntry->name, NULL);
#if 0//modify the parameter2 form "dns_table index to the name  " by wuys 2009-02-24
	    sys_post_event_to_APP(EV_CFW_DNS_RESOLV_ERR_IND, 0, i, RESOLV_HOST_NO_FOUND, 0, 0);
#else
	    sys_post_event_to_APP(EV_CFW_DNS_RESOLV_ERR_IND, 0, (UINT32)pEntry->name, RESOLV_HOST_NO_FOUND, 0, pEntry->simIdCid);

	    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" resolv_recv: resolve err\n"));
#endif
	    pbuf_free(p);
      return;
    }

    /* We only care about the question(s) and the answers. The authrr
       and the extrarr are simply discarded. */
    nquestions = ntohs(hdr->numquestions);
    nanswers = ntohs(hdr->numanswers);
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" resolv_recv: nquestions = %d\n"), nquestions);
	  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" resolv_recv: nanswers = %d\n"), nanswers);
	
    /* Skip the name in the question. XXX: This should really be
       checked agains the name in the question, to be sure that they
       match. */
    pHostname = (UINT8*) parse_name((UINT8*)p->payload + 12) + 4;
	  //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" resolv_recv: p->payload = %p\n"), p->payload);
    //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" resolv_recv: pHostname = %p\n"), pHostname);
	
    for(; nanswers > 0; nanswers--)
    {
      /* The first byte in the answer resource record determines if it
         is a compressed record or a normal one. */
      if(*pHostname & 0xc0)
      { /* Compressed name. */
        pHostname += 2;
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" resolv_recv: Compressed anwser\n"));
      }
      else
      { /* Not compressed name. */
        pHostname = (char *) parse_name((unsigned char *)pHostname);
      }

      memcpy(&ans, (DNS_ANSWER*)pHostname, SIZEOF(DNS_ANSWER));
	  
      CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" Answer: type %x, class %x, ttl %x, %x length %x\n"),
         ntohs(ans.type), ntohs(ans.class), ntohs(ans.ttl[0]), ntohs(ans.ttl[1]), ntohs(ans.len));
	  
      /* Check for IP address type and Internet class. Others are
       discarded. */
      if((ntohs(ans.type) == 1) && (ntohs(ans.class) == 1) && (ntohs(ans.len) == 4))
      { /* TODO: we should really check that this IP address is the one we want. */
        pEntry->ipaddr = ans.ipaddr[1] << 16 | ans.ipaddr[0];
	 //add for clean the old entry, 2010-02-22

	  pEntry->get_times= get_Times;
       //add end

		    //CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" resolv_recv: ans.ipaddr = %x %x \n"), ans.ipaddr[0], ans.ipaddr[1]);
		    
        if (pEntry->found) /* call specified callback function if provided */
        {
          (*pEntry->found)(pEntry->name, (struct ip_addr*)(&(pEntry->ipaddr)));
        }    
        
        #if 0//modify the parameter2 form "dns_table index to the name  " by wuys 2009-02-24
          sys_post_event_to_APP(EV_CFW_DNS_RESOLV_SUC_IND, pEntry->ipaddr, i, 0, 0, 0);
        #else
          sys_post_event_to_APP(EV_CFW_DNS_RESOLV_SUC_IND, pEntry->ipaddr, (UINT32)pEntry->name, 0, 0, pEntry->simIdCid);
          CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" resolv success in resolv_recv, pEntry->ipaddr: 0x%x\n"),pEntry->ipaddr);
        #endif
        
      	pbuf_free(p);
        return;
      }
      else
      {
        pHostname = pHostname + 10 + ntohs(ans.len);
      }
	  
    }
  }

  pbuf_free(p);
}


/*---------------------------------------------------------------------------
 * The DNS resolver client timer - handle retries and timeouts
 *---------------------------------------------------------------------------*/
void resolv_tmr(void)
{
  check_entries();
}


/*---------------------------------------------------------------------------
 * Queues a name so that a question for the name will be sent out.
 * param name - The hostname that is to be queried.
 *---------------------------------------------------------------------------*/
void resolv_query(char *name, void (*found)(char *name, struct ip_addr *addr),UINT16 simIdCid)
{
  static UINT8 i;
  static UINT8 lseq, lseqi;
  register DNS_TABLE_ENTRY *pEntry=NULL;

  lseq = lseqi = 0;

  for (i = 0; i < LWIP_RESOLV_ENTRIES; ++i)
  {
    pEntry = &dns_table[i];
    if (pEntry->state == STATE_UNUSED)
      break;

    if (seqno - pEntry->seqno > lseq)
    {
      lseq = seqno - pEntry->seqno;
      lseqi = i;
    }
  }

  if (i == LWIP_RESOLV_ENTRIES)
  {
    i = lseqi;
    pEntry = &dns_table[i];
  }

  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("Using entry %d\n"), i);
  strcpy(pEntry->name, name);
  pEntry->found = found;
  pEntry->state = STATE_NEW;
  pEntry->seqno = seqno;
   pEntry->simIdCid = simIdCid;
  ++seqno;
}


/*---------------------------------------------------------------------------*
 * Look up a hostname in the array of known hostnames.
 *
 * \note This function only looks in the internal array of known
 * hostnames, it does not send out a query for the hostname if none
 * was found. The function resolv_query() can be used to send a query
 * for a hostname.
 *
 * return A pointer to a 4-byte representation of the hostname's IP
 * address, or NULL if the hostname was not found in the array of
 * hostnames.
 *---------------------------------------------------------------------------*/
UINT32 resolv_lookup(char *name, UINT8* index)
{
  static UINT8 i;
  DNS_TABLE_ENTRY *pEntry;

  /* Walk through name list, return entry if found. If not, return NULL. */
  for(i=0; i<LWIP_RESOLV_ENTRIES; ++i)
  {
    pEntry = &dns_table[i];
    if ( (pEntry->state==STATE_DONE) && (strcmp(name, pEntry->name)==0) )
    {
      //add for clean the old entry, 2010-02-22

	  if((get_Times +128 -pEntry->get_times) % 128 >10 )
	  	{
	  	pEntry->state=STATE_UNUSED;
		 return 0;
	  	}
//add end

      *index = i;
      return pEntry->ipaddr;
	 
    }
  }
  return 0;
}


/*---------------------------------------------------------------------------*
 * Obtain the currently configured DNS server.
 * return unsigned long encoding of the IP address of
 * the currently configured DNS server or NULL if no DNS server has
 * been configured.
 *---------------------------------------------------------------------------*/
UINT32 resolv_getserver(void)
{
  if(resolv_pcb == NULL)
    return 0;
  return resolv_pcb->remote_ip.addr;
}


/*---------------------------------------------------------------------------
 * Initialize the resolver and configure which DNS server to use for queries.
 *
 * param cp points to a dot-decimal ip address of the DNS server to be configured.
 *---------------------------------------------------------------------------*/
err_t resolv_init(const INT8 * cp)
{
  static UINT8 i;

  tcpip_resolv_timer_active = 0;
  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" CFW_gethostbyname:  DNS server: %s\n"),cp);
  
  //serverIP.addr = CFW_TcpipInetAddr(cp);
  //serverIP.addr = CFW_TcpipInetAddr("211.136.17.107");
  
  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" CFW_gethostbyname:  DNS server: 0x%x\n"),serverIP.addr );
  getDnsServer(); //add by wuys 2008-05-21
  
  for(i = 0; i < LWIP_RESOLV_ENTRIES; ++i)
  {
  	memset((DNS_TABLE_ENTRY*)(&dns_table[i]), 0, sizeof(DNS_TABLE_ENTRY));
    dns_table[i].state = STATE_UNUSED;
  }
  if(resolv_pcb != NULL)
    udp_remove(resolv_pcb);

  /*TODO: check for valid IP address for DNS server? */

  resolv_pcb = udp_new();
  udp_bind(resolv_pcb, IP_ADDR_ANY, 0);
  udp_connect(resolv_pcb, &serverIP, DNS_SERVER_PORT);

  udp_recv(resolv_pcb, resolv_recv, NULL);

  initFlag = 1;

  return 0;
}



/* NON-BLOCKING callback version for use with raw API */
#if 1
//[[hameina[+]2008.9.9 clear warning
#define isdigit(c)           in_range(c, '0', '9')
#define in_range(c, lo, up)  ((UINT8)c >= lo && (UINT8)c <= up)
//]]hameina[+]2008.9.9 clear warning
#ifdef CFW_MULTI_SIM
UINT32 CFW_Gethostbyname(char *hostname, struct ip_addr *addr, UINT8 nCid, CFW_SIM_ID nSimID)
{
  UINT8 index = 0xff;
  UINT32 nRet = 0x00;
  //add for clean the old entry, 2010-02-22
  get_Times = (get_Times+1)%128;
//add end

  
  if ((!hostname) || (!hostname[0]))
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname Hostname Err %x\n"),(!hostname) );
    return RESOLV_QUERY_INVALID;
  }

  if (strlen(hostname) > MAX_NAME_LENGTH)
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname strlen(hostname) Err %x\n"),strlen(hostname));
    return RESOLV_QUERY_INVALID;
  }

  if (isdigit(hostname[0]))
  {
    /* host name already in octet notation?
      - set ip addr and return COMPLETE */
    addr->addr = CFW_TcpipInetAddr(hostname);
  	#if 1//modify 2009-04 
    if (INADDR_NONE != addr->addr)
      return RESOLV_COMPLETE;
    #else
  	 if (INADDR_NONE == addr->addr)
        return RESOLV_QUERY_INVALID;
      return RESOLV_COMPLETE;
    #endif
    
  }
  
  if(ERR_SUCCESS != (nRet = CFW_SetResolvCid( nCid,  nSimID )))
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname CFW_SetResolvCid Err %x, nCid %d, nSimID %d\n"),nRet,nCid,nSimID);
    
    return RESOLV_QUERY_INVALID;
  }

  if (!initFlag)     /* not initialized or no valid server yet */
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname initFlag Err %x\n"),initFlag);
    return RESOLV_QUERY_INVALID;
  }

  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" CFW_gethostbyname:  enter here\n"));
      CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("host name : %s\n"),hostname);

  if ((addr->addr = resolv_lookup(hostname, (UINT8*)&index)) != 0) /* already have this address cached? */
  {
   // remove by wuys 2008-06-25, for the ip address will be save to the  parameter "addr"
  //	sys_post_event_to_APP(EV_CFW_DNS_RESOLV_SUC_IND, addr->addr, index, 0, 0, 0);
	  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" CFW_gethostbyname: DNS resolving success\n"));
    return RESOLV_COMPLETE;
  }


  serverIP.addr =  CFW_getDnsServerbyPdp(nCid, nSimID );//add 2009-10-22
 
  udp_connect(resolv_pcb, &serverIP, DNS_SERVER_PORT);
  resolv_query(hostname, NULL,(nSimID<<8|nCid));      /* queue query with specified callback */
  check_entries();
  
  return RESOLV_QUERY_QUEUED;
}
   #else
//UINT32 CFW_Gethostbyname(char *hostname, struct ip_addr *addr, UINT8 nCid)
UINT32 CFW_Gethostbyname(char *hostname, struct ip_addr *addr, UINT8 nCid)
{
  UINT8 index = 0xff;
  UINT32 nRet = 0x00;
  //add for clean the old entry, 2010-02-22
  get_Times = (get_Times+1)%128;
//add end

  
  if ((!hostname) || (!hostname[0]))
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname Hostname Err %x\n"),((!hostname) || (!hostname[0])));
    return RESOLV_QUERY_INVALID;
  }

  if (strlen(hostname) > MAX_NAME_LENGTH)
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname strlen(hostname) Err %x\n"),strlen(hostname));
    return RESOLV_QUERY_INVALID;
  }

  if (isdigit(hostname[0]))
  {
    /* host name already in octet notation?
      - set ip addr and return COMPLETE */
    addr->addr = CFW_TcpipInetAddr(hostname);
  	#if 1//modify 2009-04 
    if (INADDR_NONE != addr->addr)
      return RESOLV_COMPLETE;
    #else
  	 if (INADDR_NONE == addr->addr)
        return RESOLV_QUERY_INVALID;
      return RESOLV_COMPLETE;
    #endif
    
  }
  
  if(ERR_SUCCESS != (nRet = CFW_SetResolvCid( nCid )))
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname CFW_SetResolvCid Err %x, nCid %d\n"),nRet,nCid);
    
    return RESOLV_QUERY_INVALID;
  }

  if (!initFlag)     /* not initialized or no valid server yet */
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname initFlag Err %x\n"),initFlag);
    return RESOLV_QUERY_INVALID;
  }

  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" CFW_gethostbyname:  enter here\n"));
  if ((addr->addr = resolv_lookup(hostname, (UINT8*)&index)) != 0) /* already have this address cached? */
  {
   // remove by wuys 2008-06-25, for the ip address will be save to the  parameter "addr"
  //	sys_post_event_to_APP(EV_CFW_DNS_RESOLV_SUC_IND, addr->addr, index, 0, 0, 0);
	  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" CFW_gethostbyname: DNS resolving success\n"));
    return RESOLV_COMPLETE;
  }


  serverIP.addr =  CFW_getDnsServerbyPdp(nCid );//add 2009-10-22
 
  udp_connect(resolv_pcb, &serverIP, DNS_SERVER_PORT);
  resolv_query(hostname, NULL,nCid);      /* queue query with specified callback */
  check_entries();
  
  return RESOLV_QUERY_QUEUED;
}


#if 0


UINT32 CFW_Gethostbyname(char *hostname, struct ip_addr *addr)

{
  UINT8 index = 0xff;
 // UINT32 nRet = 0x00;
  if ((!hostname) || (!hostname[0]))
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname Hostname Err %x\n"),((!hostname) || (!hostname[0])));

    return RESOLV_QUERY_INVALID;
  }

  if (strlen(hostname) > MAX_NAME_LENGTH)
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname strlen(hostname) Err %x\n"),strlen(hostname));
    return RESOLV_QUERY_INVALID;
  }

  if (isdigit(hostname[0]))
  {
    /* host name already in octet notation?
      - set ip addr and return COMPLETE */
    addr->addr = CFW_TcpipInetAddr(hostname);
#if 1//modify 2009-04 
    if (INADDR_NONE != addr->addr)
    {
      CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname addr->addr Err %x\n"),addr->addr);
      return RESOLV_COMPLETE;
    }
#else

	 if (INADDR_NONE == addr->addr)
      return RESOLV_QUERY_INVALID;
    return RESOLV_COMPLETE;
#endif
  }
/*
    if(ERR_SUCCESS != (nRet = CFW_SetResolvCid( nCid )))
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname CFW_SetResolvCid Err %x, nCid %d, nSimID %d\n"),nRet,nCid,nSimID);
    
    return RESOLV_QUERY_INVALID;
  }
*/

  if (!initFlag)     /* not initialized or no valid server yet */
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("CFW_Gethostbyname initFlag Err %x\n"),initFlag);
    return RESOLV_QUERY_INVALID;

  }

  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" CFW_gethostbyname:  enter here\n"));
  if ((addr->addr = resolv_lookup(hostname, (UINT8*)&index)) != 0) /* already have this address cached? */
  {
    // remove by wuys 2008-06-25, for the ip address will be save to the  parameter "addr"
    //	sys_post_event_to_APP(EV_CFW_DNS_RESOLV_SUC_IND, addr->addr, index, 0, 0, 0);
	  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" CFW_gethostbyname: DNS resolving success\n"));
    return RESOLV_COMPLETE;
  }

 //   serverIP.addr =  CFW_getDnsServerbyPdp(nCid);//add 2009-10-22
    
//  udp_connect(resolv_pcb, &serverIP, DNS_SERVER_PORT);

  resolv_query(hostname, NULL, 0);      /* queue query with specified callback */
  check_entries();
  
  return RESOLV_QUERY_QUEUED;
}
#endif
   #endif
#else
UINT32 CFW_Gethostbyname(char *hostname, struct ip_addr *addr, 
                       void (* found)(char *name, struct ip_addr *ipaddr))
{
  UINT8 index = 0xff;
  
  if ((!hostname) || (!hostname[0]))
    return RESOLV_QUERY_INVALID;

  if (strlen(hostname) > MAX_NAME_LENGTH)
    return RESOLV_QUERY_INVALID;

  if (isdigit(hostname[0]))
  {
    /* host name already in octet notation?
      - set ip addr and return COMPLETE */
    addr->addr = CFW_TcpipInetAddr(hostname);
    if (INADDR_NONE == addr->addr)
      return RESOLV_QUERY_INVALID;
    return RESOLV_COMPLETE;
  }

  if (!initFlag)     /* not initialized or no valid server yet */
    return RESOLV_QUERY_INVALID;

  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" CFW_gethostbyname:  enter here\n"));
  if ((addr->addr = resolv_lookup(hostname, (UINT8*)&index)) != 0) /* already have this address cached? */
  {
   // remove by wuys 2008-06-25, for the ip address will be save to the  parameter "addr"
  //	sys_post_event_to_APP(EV_CFW_DNS_RESOLV_SUC_IND, addr->addr, index, 0, 0, 0);
	  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" CFW_gethostbyname: DNS resolving success\n"));
    return RESOLV_COMPLETE;
  }
  
  resolv_query(hostname, found);      /* queue query with specified callback */
  check_entries();
  
  return RESOLV_QUERY_QUEUED;
}
#endif


extern INT8 check_ipa(const INT8 *cp);

/**************************************************/
//set DNS server
//add by wuys 2008-05-21
/**************************************************/
UINT32 CFW_SetDnsServer(INT8 *PriDnsServer, INT8 *SecDnsServer)
{
 // UINT32 ret = ERR_SUCCESS;
  UINT32 addr;
  HKEY hkey; 
   
  REG_OpenKey(REG_HKEY_CURRENT_COMM, "TCPIP", &hkey);

  if(PriDnsServer == NULL)
  {
   return ERR_EMPTY_PRI_DNSSERVER_IP;
  }
  SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" CFW_SetDnsServer Pri\n"));
//modify by wuys 2009-04-29
  if(check_ipa(PriDnsServer)==0)
    return ERR_INVALID_PARAMETER;
   
  addr = CFW_TcpipInetAddr(PriDnsServer);
  
  serverIP.addr = addr;
  REG_SetValue(hkey, "DNS_ADR1", REG_TYPE_STRING, PriDnsServer, strlen(PriDnsServer));
  
  udp_connect(resolv_pcb, &serverIP, DNS_SERVER_PORT);//add by wuys 2009-04-29
   SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" ---------ServerIP: 0x%x , resolv_pcb->remote_Ip:\n"), serverIP.addr, resolv_pcb->remote_ip);
  // SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" ---------ServerIP: 0x%x , resolv_pcb->remote_Ip:\n"), serverIP.addr, resolv_pcb->remote_ip);

  if(SecDnsServer != NULL)
  {
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" CFW_SetDnsServer sec\n"));
    if(check_ipa(SecDnsServer)==0)
       return ERR_INVALID_PARAMETER;
    
    addr = CFW_TcpipInetAddr(SecDnsServer);
     
    Sec_serverIP.addr = addr;
    REG_SetValue(hkey, "DNS_ADR2", REG_TYPE_STRING, SecDnsServer, strlen(SecDnsServer));
  }
  REG_CloseKey(REG_HKEY_CURRENT_COMM);
  return ERR_SUCCESS;
  
}
/*
UINT32 CFW_SetDnsServer(INT8 *DnsServer, BOOL isPrimary)
{
  UINT32 ret = ERR_SUCCESS;
  UINT32 addr;
  if(DnsServer != NULL)
  {
    addr = CFW_TcpipInetAddr(DnsServer);
    if((addr = CFW_TcpipInetAddr(DnsServer))==INADDR_NONE)
    {
      //ret =  ERR_INVALD_PRIMARY_DNS_IP
    }else{
      HKEY hkey; 
      REG_OpenKey(REG_HKEY_CURRENT_COMM, "TCPIP", &hkey);

      if(isPrimary)
      {
        serverIP.addr = addr;
        REG_SetValue(hkey, "DNS_ADR1", REG_TYPE_STRING, DnsServer, strlen(DnsServer));
      }else{
        Sec_serverIP.addr = addr;
        REG_SetValue(hkey, "DNS_ADR2", REG_TYPE_STRING, DnsServer, strlen(DnsServer));
      }
      REG_CloseKey(REG_HKEY_CURRENT_COMM);
    }
  }else{
    //ret =  ERR_EMPTY_DNSSERVER_IP
  }
  return ret;
  
}
*/
/**************************************************/
//get DNS server
//add by wuys 2008-05-21
/**************************************************/
UINT32 getDnsServer(void)
{
  UINT32 ret = ERR_SUCCESS;
  HKEY hkey=0; 
  UINT8 reg_buf[20];
  PVOID pvData = reg_buf;
  UINT8 uType = REG_TYPE_STRING;
  UINT8 pDataSize = 20;
  
  REG_OpenKey(REG_HKEY_CURRENT_COMM, "TCPIP", &hkey);
  
  //REG_GetValue(hkey, " ", &uType, pvData, &pDataSize);
  
  //pvData = CSW_Malloc(pDataSize);
  memset(pvData, 0, 20);
  REG_GetValue(hkey, "DNS_ADR1", &uType, pvData, &pDataSize);
  serverIP.addr = CFW_TcpipInetAddr((INT8*)pvData);
 // CSW_Free(pvData);
  
  //pvData = NULL;
  memset(pvData, 0, 20);
  pDataSize = 20;
  //REG_GetValue(hkey, " ", &uType, pvData, &pDataSize);
  
  //pvData = CSW_Malloc(pDataSize);
  REG_GetValue(hkey, "DNS_ADR2", &uType, pvData, &pDataSize);
  Sec_serverIP.addr = CFW_TcpipInetAddr((INT8*)pvData);
//  CSW_Free(pvData);
  
  REG_CloseKey(REG_HKEY_CURRENT_COMM);
  
  return ret;
  
}
/**************************************************
**************************************************/
UINT32 CFW_GetDnsAddr( struct ip_addr *PriDnsSer, struct ip_addr *SecDnsSer )
{
  if (PriDnsSer == NULL)
    return ERR_INVALID_PARAMETER;
  
  PriDnsSer->addr = serverIP.addr;
  
  if (SecDnsSer != NULL)
    SecDnsSer->addr = Sec_serverIP.addr;
  
  return ERR_SUCCESS;
}


#ifdef CFW_MULTI_SIM
UINT32  CFW_SetResolvCid( UINT8 cid, UINT8 nSimId )
{
  struct netif * netif;
  UINT32 T_cid = 0x00;

  T_cid = nSimId <<16 |cid ;

    netif = netif_find_byCid(T_cid);
  if(netif != NULL)
  {
    if( !ip_addr_cmp(&(resolv_pcb->local_ip), &(netif->ip_addr)))
    {
      udp_bind(resolv_pcb, &(netif->ip_addr), 0);
    }
  }
  else
  {
    return ERR_INVALID_CONFIGURATION;
  }
  
  return ERR_SUCCESS;
}
#else
UINT32  CFW_SetResolvCid( UINT8 cid )
{
  struct netif * netif;

  netif = netif_find_byCid(cid);
  if(netif != NULL)
  {
    if( !ip_addr_cmp(&(resolv_pcb->local_ip), &(netif->ip_addr)))
    {
      udp_bind(resolv_pcb, &(netif->ip_addr), 0);
    }
  }
  else
  {
    return ERR_INVALID_CONFIGURATION;
  }
  
  return ERR_SUCCESS;
}
#endif
#endif
