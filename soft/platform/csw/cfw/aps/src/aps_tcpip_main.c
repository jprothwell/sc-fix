/******************************************************************************/
/*              Copyright (C) 2006, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    aps_main.c                                                    */
/*                                                                            */
/* Description:                                                               */
/*                                                                            */
/******************************************************************************/
#ifdef CFW_TCPIP_SUPPORT


#include <csw.h>
#include "tcpip_err.h"
#include "tcpip_pbuf.h"
#include "tcpip_memp.h"

#include "tcpip_ip_addr.h"
#include "sxs_io.h"

#include "tcpip_api_msg.h"
#include "string.h"
#include "tcpip_netif.h"
#include "tcpip_opt.h"

extern void TCpIPInit(void);
extern VOID Tcpip_Destroy(VOID);
extern err_t ip_input(struct pbuf * p, struct netif * inp);
extern  VOID tcpip_tcp_timer(VOID *arg);
extern  VOID ip_timer(VOID *data);
extern  void resolv_tmr(void);
BOOL IsDnsPCB(struct udp_pcb *u_pcb);

extern void Modify_SmarchAttStatus(void);// add by wuys for patching QQ, 2010-06-09



#define CFW_POP3_TS_ID_SXS (_MMI |TLEVEL(8))
#define CFW_GPRS_TRACE_DATA  (_CSW| TLEVEL(9))
//#define CFW_TCPIP_TS_ID_SXS (_MMI |TLEVEL(4))
extern struct tcp_pcb *tcp_active_pcbs;
extern struct tcp_pcb *tcp_tw_pcbs;
extern struct udp_pcb *udp_pcbs;

// Task Event pro
// 
#ifdef CFW_MULTI_SIM
extern BOOL start_gprsDetach_timer [CFW_SIM_COUNT];
#else

extern BOOL start_gprsDetach_timer ;

#endif

VOID CFW_SmarchAttStatus(VOID)
{
  COS_EVENT EV;
  SUL_ZeroMemory8(&EV, SIZEOF(COS_EVENT));
  EV.nEventId = EV_CFW_ATT_STATUS_IND;
  COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &EV, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}
//add for QQ, 2010-05-10
VOID CFW_SmarchCCStatus(UINT32 Status)
{
  COS_EVENT EV;
  SUL_ZeroMemory8(&EV, SIZEOF(COS_EVENT));
  EV.nEventId = EV_CFW_CC_STATUS_IND;
  EV.nParam1= Status;
  COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &EV, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}
// add end
BOOL BAL_ApsTaskTcpipProc (COS_EVENT* pEvent)
{   	

    //
    // Add your tcp ip pro code.
    //
    //
    //SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("BAL_ApsTaskTcpipProc recive a event \n") );
    struct netif *netif, *tmp_netif;
    struct ip_addr *ip = NULL;
    struct ip_addr netmask;

    struct tcp_pcb *pcb, *tmp_pcb;
    struct udp_pcb  *u_pcb,*tmp_u_pcb;
	if(pEvent->nEventId != 93)
    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pEvent->nEventId : 0x%x\n"),pEvent->nEventId);

    if(pEvent->nEventId == EV_CFW_GPRS_DATA_IND)
    {
        struct pbuf     *p, *q;
		
	 UINT16          len;
	 UINT16          offset;
	 CFW_GPRS_DATA * Msg;
        UINT32 cid;
	 struct netif * inp_netif;
		
        Msg = (CFW_GPRS_DATA *)pEvent->nParam2;


             //add for 221 2009-04-21
			
		 
		#ifdef CFW_MULTI_SIM
			cid = pEvent->nParam1|(LOUINT8(pEvent->nParam3)<<16);
			#else
			cid = pEvent->nParam1;
		#endif
		//add end
		
	// cid = pEvent->nParam2;
        //SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("EV_CFW_GPRS_DATA_IND data: p = %p, data len = 0x%x \n"), Msg, Msg->nDataLength );
		
        //SXS_DUMP(CFW_TCPIP_TS_ID, 0, Msg->pData, Msg->nDataLength);
                CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("EV_CFW_GPRS_DATA_IND, data len = %d \n"),  Msg->nDataLength );
		
       SXS_DUMP(CFW_GPRS_TRACE_DATA, 0, Msg->pData, Msg->nDataLength + 4);

	 len = Msg->nDataLength;
		
	 p = (struct pbuf *)pbuf_alloc(PBUF_RAW, len, PBUF_POOL);    //PBUF_RAW 3 // PBUF_POOL 3
	 offset = 0;
	 if(p != NULL)
	 {
	     if(len > p->len)
	     {
		   for (q = p; len > q->len; q = q->next) 
		   {
		       SUL_MemCopy8 (q->payload, &(Msg->pData[offset]), q->len);
			//SXS_DUMP(_TCP,TSTXT "%2x", q->payload, q->len);
			len -= q->len;
			offset += q->len;
		   }
		  if(len != 0)
		  {
			 SUL_MemCopy8 (q->payload, &(Msg->pData[offset]), len);
			 //SXS_DUMP(_TCP,TSTXT "%2x", q->payload, len);
		  }
	    }
	    else
	    {
	         SUL_MemCopy8 (p->payload, &(Msg->pData[offset]), len);
		  //SXS_DUMP(_TCP,TSTXT "%2x", p->payload, len);
	    }
			
	    inp_netif = netif_find_byCid(cid);
				
	    if(inp_netif != NULL)
	    ip_input(p, inp_netif);
      }
	else
	{
	    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("****Receive Data request memory :out of place\n"));
	}
	CSW_TCPIP_FREE(Msg);

    }
    else if(pEvent->nEventId == EV_INTER_APS_TCPIP_REQ)
    {
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("aps: nEventId == EV_INTER_APS_TCPIP_REQ\n"));   	 
        struct api_msg *msg;
 
        msg = (struct api_msg *)pEvent->nParam1;

        api_msg_input( msg );
    	 memp_free(MEMP_API_MSG, msg);
        CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("memp_free api msg %p\n"), msg);   	 
     	 
    }
   else if(pEvent->nEventId == EV_TIMER)
   {
       if(pEvent->nParam1 == PRV_CFW_TCP_TIMER_ID)
       {
           tcpip_tcp_timer(NULL);
       }
   	else if(pEvent->nParam1 == PRV_CFW_IP_TIMER_ID)
       {
           ip_timer(NULL);
       }
   	else if(pEvent->nParam1 == PRV_CFW_DNS_TIMER_ID)
       {
           resolv_tmr();
       }
   	else
   	{
   	}
   }else if(pEvent->nEventId == EV_CFW_GPRS_ACT_RSP)
	{
	    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("BAL_ApsTaskTcpipProc recive EV_CFW_GPRS_ACT_RSP \n") );


  
		//SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("APS : active success cid = 0x%x\n"), pEvent->nParam1);	 

               //  reveive the ncid and ip address from the gprs active success
               // 			 cos_ev.nParam1 = proc->Cid2Index.nCid;
             //			 cos_ev.nParam2 = Ip_Addr;
         //                  cos_ev.nParam3 = CFW_GPRS_ACTIVED; or CFW_GPRS_DEACTIVED


		//add for 221 2009-04-21
			
		 UINT32 T_cid = 0x00;
		#ifdef CFW_MULTI_SIM
			T_cid = pEvent->nParam1|(LOUINT8(pEvent->nParam3)<<16);
			#else
			T_cid = pEvent->nParam1;
		#endif
		//add end
		if(HIUINT8(pEvent->nParam3) == CFW_GPRS_ACTIVED)
		{
				    UINT8 addr[4];				
	    UINT32 Ip_Addr;

	    UINT8 lenth;
#ifdef CFW_MULTI_SIM
	    CFW_GprsGetPdpAddr(pEvent->nParam1, &lenth, addr,(LOUINT8(pEvent->nParam3)));
#else
	    CFW_GprsGetPdpAddr(pEvent->nParam1, &lenth, addr);
#endif
	    Ip_Addr = (addr[3]<<24) |(addr[2]<<16) | (addr[1]<< 8) |addr[0] ;

			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("APS : active success cid = 0x%x\n"), T_cid);   
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("IP: %d.%d.%d.%d"), 
					      	(UINT16)(ntohl(Ip_Addr) >> 24) & 0xff, 
		            			(UINT16)(ntohl(Ip_Addr) >> 16) & 0xff, 
					      	(UINT16)(ntohl(Ip_Addr) >> 8) & 0xff, 
		            			(UINT16) ntohl(Ip_Addr) & 0xff);

			//add by wys 2007-02-14, there are something tobe done to 
			//set the netif configure value such as "mtu",by now, these are not be done
			netif = CSW_TCPIP_MALLOC(SIZEOF(struct netif));
			memset(netif, 0, SIZEOF(struct netif));

			ip = CSW_TCPIP_MALLOC(SIZEOF(struct ip_addr));
			memset(ip, 0, SIZEOF(struct ip_addr));
      			netmask.addr = 0x00ffffff;
      
			ip->addr = Ip_Addr;

			netif_add(netif, ip, &netmask, NULL, NULL, NULL, NULL);
			
			
			
			netif->lowaddr.cid = T_cid;
			
			netif->mtu = GPRS_MTU;// add for test IP fragment, by wuys 2007-12-06

			if(NULL == netif_default)
			{
				netif_set_default(netif);
			}
			netif_set_up(netif);
			//add end
		}
		else if(HIUINT8(pEvent->nParam3) == CFW_GPRS_DEACTIVED)
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("APS : De active cid = 0x%x; tcp_active_pcbs: 0x%x; tcp_tw_pcbs:0x%x\n"), T_cid,tcp_active_pcbs,tcp_tw_pcbs);

			
			
		#if 1
		netif = netif_list;
	            while(netif!=NULL)
	            {
	            	     tmp_netif = netif->next;
			     if (netif->lowaddr.cid == T_cid)
				{
          ip = CSW_TCPIP_MALLOC(SIZEOF(struct ip_addr));
			       memset(ip, 0, SIZEOF(struct ip_addr));
      		
				      ip->addr=netif->ip_addr.addr;
					CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("netif_remove: 0x%x\n"), netif->lowaddr.cid);
					netif_remove(netif);
          CSW_TCPIP_FREE(netif);

              pcb = tcp_active_pcbs;
		 
		while(pcb != NULL)
		{
			
                      tmp_pcb = pcb->next;
					  
			if (ip_addr_cmp(ip, &(pcb->local_ip)))
				{
				CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pcb remove:\n"));
		            // tcp_pcb_remove_all(pcb,1);//2009-12-16, tcp_pcb_remove(&tcp_active_pcbs, pcb);
		             if(tcp_pcb_remove_all(pcb,1))
					  memp_free(MEMP_TCP_PCB, pcb);
				}
			
			 pcb = tmp_pcb;
		}


		  pcb = tcp_tw_pcbs;
		 
		while(pcb != NULL)
		{
                      tmp_pcb = pcb->next;
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tw pcb remove: %x\n"),pcb);
			
		        if (ip_addr_cmp(ip, &(pcb->local_ip)))
		        	{
		        	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tw pcb remove:\n"));
		        //tcp_pcb_remove_all(pcb,2);//2009-12-16, tcp_pcb_remove(&tcp_tw_pcbs, pcb);
		            if(tcp_pcb_remove_all(pcb,2))
			 memp_free(MEMP_TCP_PCB, pcb);
		        	}
		        	
			 pcb = tmp_pcb;
		}

		u_pcb = udp_pcbs;
		while(u_pcb!=NULL)
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" udp pcb remove:\n"));

			tmp_u_pcb = u_pcb->next;
			if (ip_addr_cmp(ip, &(u_pcb->local_ip))&&(!IsDnsPCB(u_pcb)))
			   udp_remove( u_pcb);
			u_pcb = tmp_u_pcb;
		}
		
					

				}
			     netif = tmp_netif;
	            }
		
		 CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("APS : De active , tcp_active_pcbs: 0x%x; tcp_tw_pcbs:0x%x\n"),tcp_active_pcbs,tcp_tw_pcbs);

		   
		#else
		//add by wuys 2008-10-13
		 		  
		    for (pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next)
		    {
		        SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pcb remove:\n"));

		        tcp_pcb_remove(&tcp_active_pcbs, pcb);
			 memp_free(MEMP_TCP_PCB, pcb);
		          
		    }
                  for (pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next)
		    {
		        SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pcb remove:\n"));

		        tcp_pcb_remove(&tcp_tw_pcbs, pcb);
			 memp_free(MEMP_TCP_PCB, pcb);
		          
		    }
			
		//end add
		
		for (u_pcb = udp_pcbs; u_pcb != NULL; u_pcb = u_pcb->next)
		    {
		        SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" udp pcb remove:\n"));

		        udp_remove( u_pcb);
		
		          
		    }
		#endif
		}
    		COS_EVENT cos_ev;
		cos_ev.nEventId = EV_CFW_GPRS_ACT_RSP;
		cos_ev.nParam1 = pEvent->nParam1;
		cos_ev.nParam2 = pEvent->nParam2;
		cos_ev.nParam3 = pEvent->nParam3;
    		COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &cos_ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
     
	}else if(pEvent->nEventId == EV_CFW_GPRS_ATT_RSP)
	{
     		SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("BAL_ApsTaskTcpipProc recive EV_CFW_GPRS_ATT_RSP \n") );
		//add by wuys for detach 2008-10-13
		if(HIUINT8(pEvent->nParam3)== 0x00)
		{
		
                  
		   
		   	
		#if 1
#if 1
		netif = netif_list;

 
	            while(netif!=NULL)
	            {
	            	     tmp_netif = netif->next;
			     if (((netif->lowaddr.cid >>16)&0xFF) == LOUINT8(pEvent->nParam3) )
				{
          ip = CSW_TCPIP_MALLOC(SIZEOF(struct ip_addr));
			       memset(ip, 0, SIZEOF(struct ip_addr));
      		
				      ip->addr=netif->ip_addr.addr;
					CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("netif_remove: 0x%x\n"), netif->lowaddr.cid);
					netif_remove(netif);
          CSW_TCPIP_FREE(netif);

              pcb = tcp_active_pcbs;
		 
		while(pcb != NULL)
		{
			
                      tmp_pcb = pcb->next;
					  
			if (ip_addr_cmp(ip, &(pcb->local_ip)))
				{
				CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pcb remove:\n"));
		            // tcp_pcb_remove_all(pcb,1);//2009-12-16, tcp_pcb_remove(&tcp_active_pcbs, pcb);
		             if(tcp_pcb_remove_all(pcb,1))
					  memp_free(MEMP_TCP_PCB, pcb);
				}
			
			 pcb = tmp_pcb;
		}


		  pcb = tcp_tw_pcbs;
		 
		while(pcb != NULL)
		{
                      tmp_pcb = pcb->next;
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tw pcb remove: %x\n"),pcb);
			
		        if (ip_addr_cmp(ip, &(pcb->local_ip)))
		        	{
		        	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tw pcb remove:\n"));
		        //tcp_pcb_remove_all(pcb,2);//2009-12-16, tcp_pcb_remove(&tcp_tw_pcbs, pcb);
		            if(tcp_pcb_remove_all(pcb,2))
			 memp_free(MEMP_TCP_PCB, pcb);
		        	}
		        	
			 pcb = tmp_pcb;
		}

		u_pcb = udp_pcbs;
		while(u_pcb!=NULL)
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" udp pcb remove:\n"));

			tmp_u_pcb = u_pcb->next;
			if (ip_addr_cmp(ip, &(u_pcb->local_ip))&&(!IsDnsPCB(u_pcb)))
			   udp_remove( u_pcb);
			u_pcb = tmp_u_pcb;
		}
		
					

				}
			     netif = tmp_netif;
	            }
#else
		 netif = netif_list;
	            while(netif!=NULL)
	            {
	            	     tmp_netif = netif->next;
			     netif_remove(netif);
				CSW_TCPIP_FREE(netif);
			     netif = tmp_netif;
	            }
				
		 pcb = tcp_active_pcbs;
		 
		while(pcb != NULL)
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pcb remove:\n"));
                      tmp_pcb = pcb->next;
		       // tcp_pcb_remove_all(pcb,1);//2009-12-16, tcp_pcb_remove(&tcp_active_pcbs, pcb);
		            if(tcp_pcb_remove_all(pcb,1))
			 memp_free(MEMP_TCP_PCB, pcb);
			 pcb = tmp_pcb;
		}


		  pcb = tcp_tw_pcbs;
		 
		while(pcb != NULL)
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pcb remove:\n"));
                      tmp_pcb = pcb->next;
		        //tcp_pcb_remove_all(pcb,2);//2009-12-16, tcp_pcb_remove(&tcp_tw_pcbs, pcb);
		            if(tcp_pcb_remove_all(pcb,2))
			 memp_free(MEMP_TCP_PCB, pcb);
			 pcb = tmp_pcb;
		}

		u_pcb = udp_pcbs;
		while(u_pcb!=NULL)
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" udp pcb remove:\n"));

			tmp_u_pcb = u_pcb->next;
			if(!IsDnsPCB(u_pcb))
			    udp_remove( u_pcb);
			u_pcb = tmp_u_pcb;
		}
		
		#endif          
		   
		#else
		for (netif = netif_list; netif != NULL; netif = netif->next)
			{
				SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("netif_remove: 0x%x\n"), netif->lowaddr.cid);
				netif_remove(netif);
			}
			//add by wuys 2008-10-13
   		    for (pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next)
   		    {
   		    	  SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pcb remove:\n"));

   		         tcp_pcb_remove(&tcp_active_pcbs, pcb);
			  memp_free(MEMP_TCP_PCB, pcb);
   		          
   		    }
			for (pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next)
		    {
		        SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pcb remove:\n"));

		        tcp_pcb_remove(&tcp_tw_pcbs, pcb);
			 memp_free(MEMP_TCP_PCB, pcb);
		          
		    }
			//end add
			for (u_pcb = udp_pcbs; u_pcb != NULL; u_pcb = u_pcb->next)
		    {
		        SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" udp pcb remove:\n"));

		        udp_remove( u_pcb);
		
		          
		    }
			#endif
			Modify_SmarchAttStatus();// add by wuys for patching QQ, 2010-06-09

		}
            //add end
            	#ifdef CFW_MULTI_SIM
		            if(start_gprsDetach_timer[LOUINT8(pEvent->nParam3)]&&(HIUINT8(pEvent->nParam3)== 0x00))
				start_gprsDetach_timer[LOUINT8(pEvent->nParam3)]  = FALSE;

#else

            if(start_gprsDetach_timer&&(HIUINT8(pEvent->nParam3)== 0x00))
				start_gprsDetach_timer  = FALSE;


#endif
			else{
               COS_EVENT cos_ev;
		 cos_ev.nEventId = EV_CFW_GPRS_ATT_RSP;
		 cos_ev.nParam1 = pEvent->nParam1;
		 cos_ev.nParam2 = pEvent->nParam2;
		 cos_ev.nParam3 = pEvent->nParam3;
               COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &cos_ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
				}
	}else if(pEvent->nEventId == EV_CFW_GPRS_CXT_DEACTIVE_IND)
	{

	//add for 221 2009-04-21
			
		 UINT32 T_cid = 0x00;
		#ifdef CFW_MULTI_SIM
			T_cid = pEvent->nParam1|(LOUINT8(pEvent->nParam3)<<16);
			#else
			T_cid = pEvent->nParam1;
		#endif
		//add end
		
     		     //  SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("APS : EV_CFW_GPRS_CXT_DEACTIVE_IND cid = 0x%x ,cause: 0x%x\n"), T_cid, pEvent->nParam2);

	#if 1
				CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("APS : EV_CFW_GPRS_CXT_DEACTIVE_IND, cid = 0x%x; tcp_active_pcbs: 0x%x; tcp_tw_pcbs:0x%x\n"), T_cid,tcp_active_pcbs,tcp_tw_pcbs);


		 netif = netif_list;
	            while(netif!=NULL)
	            {
	            	     tmp_netif = netif->next;
			     if (netif->lowaddr.cid == T_cid)
				{
					ip = CSW_TCPIP_MALLOC(SIZEOF(struct ip_addr));
			               memset(ip, 0, SIZEOF(struct ip_addr));
                                    ip->addr=netif->ip_addr.addr;
                                   CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("netif_remove: 0x%x\n"), netif->lowaddr.cid);
					netif_remove(netif);
					CSW_TCPIP_FREE(netif);


 pcb = tcp_active_pcbs;
		 
		while(pcb != NULL)
		{
		
                      tmp_pcb = pcb->next;
			if (ip_addr_cmp(ip, &(pcb->local_ip)))
		             {
				CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("pcb remove:\n"));
		             //tcp_pcb_remove_all(pcb,1);//2009-12-16, tcp_pcb_remove(&tcp_active_pcbs, pcb);
		                 if(tcp_pcb_remove_all(pcb,1))
					 memp_free(MEMP_TCP_PCB, pcb);
				}
			 
			 pcb = tmp_pcb;
		}


		  pcb = tcp_tw_pcbs;
		 
		while(pcb != NULL)
		{
			
                      tmp_pcb = pcb->next;
			 if (ip_addr_cmp(ip, &(pcb->local_ip)))
			 	{
			 	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tw pcb remove:\n"));
		             //tcp_pcb_remove_all(pcb,2);//2009-12-16, tcp_pcb_remove(&tcp_tw_pcbs, pcb);
		                 if(tcp_pcb_remove_all(pcb,2))
			 memp_free(MEMP_TCP_PCB, pcb);
			 	}
			 pcb = tmp_pcb;
		}

		u_pcb = udp_pcbs;
		while(u_pcb!=NULL)
		{
			CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" udp pcb remove:\n"));

			tmp_u_pcb = u_pcb->next;
			if (ip_addr_cmp(ip, &(u_pcb->local_ip))&&(!IsDnsPCB(u_pcb)))
			   udp_remove( u_pcb);
			u_pcb = tmp_u_pcb;
		}
		
					

				}
			     netif = tmp_netif;
	            }
		
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("APS , cid = 0x%x; tcp_active_pcbs: 0x%x; tcp_tw_pcbs:0x%x\n"), T_cid,tcp_active_pcbs,tcp_tw_pcbs);

		          
		   
		#else


			for (netif = netif_list; netif != NULL; netif = netif->next)
			{
				if (netif->lowaddr.cid == T_cid)
				{
					SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("netif_remove: 0x%x\n"), netif->lowaddr.cid);
					netif_remove(netif);

				}
			}
			
		//add by wuys 2008-10-13
		  
		    for (pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next)
		    {
		        SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp pcb remove:\n"));

		        tcp_pcb_remove(&tcp_active_pcbs, pcb);
			 memp_free(MEMP_TCP_PCB, pcb);
		          
		    }
                  for (pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next)
		    {
		        SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("tcp pcb remove:\n"));

		        tcp_pcb_remove(&tcp_tw_pcbs, pcb);
			 memp_free(MEMP_TCP_PCB, pcb);
		          
		    }

			
		//end add
		
		for (u_pcb = udp_pcbs; pcb != NULL; u_pcb = u_pcb->next)
		    {
		        SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT(" udp pcb remove:\n"));

		        udp_remove( u_pcb);
		
		          
		    }
#endif
		
    		COS_EVENT cos_ev;
		cos_ev.nEventId = EV_CFW_GPRS_CXT_DEACTIVE_IND;
		cos_ev.nParam1 = pEvent->nParam1;
		cos_ev.nParam2 = pEvent->nParam2;
		cos_ev.nParam3 = pEvent->nParam3;
    		COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &cos_ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
   
	}


	  if(ip)
     CSW_TCPIP_FREE(ip);
    return TRUE;    
}

//add by wuys for gprs test 2008-09-04


void Gprs_TcpIp(CFW_GPRS_DATA *pData ,UINT32 cid)
{
     struct pbuf     *p, *q;
     UINT16          len;
     UINT16          offset;
     CFW_GPRS_DATA * Msg;
     struct netif * inp_netif;
     
     Msg = pData;
     //SXS_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("Gprs_TcpIp data: p = %p, data len = 0x%x \n"), Msg, Msg->nDataLength );
    //SXS_DUMP(CFW_TCPIP_TS_ID, 0, Msg->pData, Msg->nDataLength);
     len = Msg->nDataLength;
     p = (struct pbuf *)pbuf_alloc(PBUF_RAW, len, PBUF_POOL);    //PBUF_RAW 3 // PBUF_POOL 3
     offset = 0;
     if(p != NULL)
     {
     	if(len > p->len)
     	{
       	for (q = p; len > q->len; q = q->next) 
       	{
       		SUL_MemCopy8 (q->payload, &(Msg->pData[offset]), q->len);
       		//SXS_DUMP(_TCP,TSTXT "%2x", q->payload, q->len);
       		len -= q->len;
       		offset += q->len;
       	}
       	if(len != 0)
       	{
       		SUL_MemCopy8 (q->payload, &(Msg->pData[offset]), len);
       		//SXS_DUMP(_TCP,TSTXT "%2x", q->payload, len);
       	}
     	}
     	else
     	{
     		SUL_MemCopy8 (p->payload, &(Msg->pData[offset]), len);
     		//SXS_DUMP(_TCP,TSTXT "%2x", p->payload, len);
     	}
     	
     	inp_netif = netif_find_byCid(cid);
     		
     	if(inp_netif != NULL)
     		ip_input(p, inp_netif);
     }
     else
     {
     	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("****Receive Data request memory :out of place\n"));
     }
}
//add end
#endif

