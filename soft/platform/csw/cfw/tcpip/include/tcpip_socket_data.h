#ifdef CFW_TCPIP_SUPPORT
#ifndef __TCPIP_SOCKET_DATA_H__
#define __TCPIP_SOCKET_DATA_H__



#include "tcpip_opt.h"
#include "tcpip_sys.h"
#include <cswtype.h>
#include "tcpip_pbuf.h"



#define MAX_ENVELOP    40
#define MAX_SOCKET      MEMP_NUM_NETCONN
#define INDEX_MOD(_x)  ( ((_x) + MAX_ENVELOP) % MAX_ENVELOP )
struct socket_data
{
       UINT8 type;
	UINT8 pad[3];
	UINT16 front;
	UINT16   rear;
	void* index[MAX_ENVELOP];
};

VOID socketdata_initial(INT32 sockfd, UINT8 type);

BOOL socketdata_store(INT32 sockfd, void* rec_data);

void* socketdata_get(INT32 sockfd);

extern sys_sem_t Tcp_receive_buf_sem;

#endif
#endif

