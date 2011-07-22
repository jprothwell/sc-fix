#ifdef CFW_TCPIP_SUPPORT
 
#include "tcpip_socket_data.h"
#include <csw.h>
#include "tcpip_arch.h"
#include "tcpip_api.h"
#include "ts.h"
#include "sul.h"
#if 1 //modify by wuys 2008-09-09 for test gprs

static struct socket_data socketid[MAX_SOCKET]; 
VOID socketdata_initial(INT32 sockfd, UINT8 type)
{

 CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("start socketdata_initial socketid: 0x%x, type: 0x%x !\n"),sockfd,type);

  COS_WaitForSemaphore(Tcp_receive_buf_sem, COS_WAIT_FOREVER);
 // void* buf = NULL;

  if(0xFF==sockfd)
  {
       INT32 sid=0;
       for(sid = 0; sid<MEMP_NUM_NETCONN; sid++)
       {
  	    SUL_MemSet8(&socketid[sid], 0, sizeof(struct socket_data));
           socketid[sid].type = type;
       }
	CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("socketdata_initial in TCP init!\n"));

	COS_ReleaseSemaphore(Tcp_receive_buf_sem);

       return;
  }
  if((sockfd < 0) && (sockfd>=MEMP_NUM_NETCONN))
  {
        COS_ReleaseSemaphore(Tcp_receive_buf_sem);
	 CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("socketdata_initial socket error!\n"));


  	return;
 }
  struct socket_data *p = &socketid[sockfd];

  switch(p->type)
  {
	 case NETCONN_UDPLITE:
        case NETCONN_UDPNOCHKSUM:
        case NETCONN_UDP:
           while(p->index[p->front])
           {
              
		netbuf_delete(p->index[p->front]);
	
	       p->index[p->front] = NULL;	
	       p->front = INDEX_MOD(p->front + 1) ;
	   }
           break;		   
        case NETCONN_TCP:
	    while(p->index[p->front])
           {
		pbuf_free(p->index[p->front]);
	       p->index[p->front] = NULL;	
	       p->front = INDEX_MOD(p->front + 1) ;
	   }
           break;
        default:
	    break;
		
	
  }

  
	
  SUL_MemSet8(&socketid[sockfd], 0, sizeof(struct socket_data));
  socketid[sockfd].type = type;
 CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("end socketdata_initial socketid: 0x%x, type: 0x%x !\n"),sockfd,type);

  COS_ReleaseSemaphore(Tcp_receive_buf_sem);
  return;
}

BOOL socketdata_store(INT32 sockfd, void* rec_data)
{
  COS_WaitForSemaphore(Tcp_receive_buf_sem, COS_WAIT_FOREVER);
  
  struct socket_data *p = &socketid[sockfd];
	if(!rec_data)
	{
         COS_ReleaseSemaphore(Tcp_receive_buf_sem);
	  return TRUE;
	}
	if(p->index[p->rear])
	{
	    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("store too fast!\n"));
            COS_ReleaseSemaphore(Tcp_receive_buf_sem);
	     return FALSE;
	}
	else
	{
		p->index[p->rear] = rec_data;	
		p->rear = INDEX_MOD(p->rear + 1);
              COS_ReleaseSemaphore(Tcp_receive_buf_sem);
		return TRUE;
	}
}

void* socketdata_get(INT32 sockfd)
{
	struct socket_data *p = &socketid[sockfd];
	void* buf = NULL;
	if(p->index[p->front])
	{
		buf = p->index[p->front];	
		p->index[p->front] = NULL;	
		p->front = INDEX_MOD(p->front + 1) ;
	}
	else
	{
	  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("get too fast!\n"));
	}
	return buf;
}
#else
static struct socket_data socketid[MAX_SOCKET]; 

VOID socketdata_initial(INT32 sockfd)
{
  SUL_MemSet8(&socketid[sockfd], 0, sizeof(struct socket_data));
}

BOOL socketdata_store(INT32 sockfd, void* rec_data)
{
    struct socket_data *p = &socketid[sockfd];
	if(!rec_data)
	{
	  return TRUE;
	}
	if(p->index[p->rear])
	{
	    CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("store too fast!\n"));
	     return FALSE;
	}
	else
	{
		p->index[p->rear] = rec_data;	
		p->rear = INDEX_MOD(p->rear + 1);
		return TRUE;
	}
}

void* socketdata_get(INT32 sockfd)
{
 CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("socketdata_get!\n"));
	struct socket_data *p = &socketid[sockfd];
	void* buf = NULL;
	if(p->index[p->front])
	{
		buf = p->index[p->front];	
		p->index[p->front] = NULL;	
		p->front = INDEX_MOD(p->front + 1) ;
	}
	else
	{
	  CSW_TRACE(CFW_TCPIP_TS_ID_SXS, TSTXT("get too fast!\n"));
	}
	return buf;
}
#endif

#endif
