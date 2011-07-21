#ifndef __ZYX_SOCKET_H__
#define __ZYX_SOCKET_H__

#include "zyxtypes.h"

typedef enum
{
    ZYX_CMWAP = 0,  /* cmwap */
    ZYX_CMNET       /* cmnet */
} ZYX_CONNECT_TYPE;

typedef enum
{
    ZYX_SOCK_STREAM = 0,  /* stream socket, TCP */
    ZYX_SOCK_DGRAM       /* datagram socket, UDP */
} ZYX_SOCK_TYPE;

//event type
enum
{
    ZYX_SOC_READ    = 0x01,  /* Notify for read */
    ZYX_SOC_WRITE   = 0x02,  /* Notify for write */
    ZYX_SOC_ACCEPT  = 0x04,  /* Notify for accept */
    ZYX_SOC_CONNECT = 0x08,  /* Notify for connect */
    ZYX_SOC_CLOSE   = 0x10   /* Notify for close */
};

typedef struct
{
    int socket_id;
    int event_type;
    int result;
}ZyxSocketEvent,*ZyxSocketEventPtr;

#ifndef MAX_SOCK_ADDR_LEN
#define MAX_SOCK_ADDR_LEN           (28)
#endif

typedef struct
{
    ZYX_SOCK_TYPE	sock_type;
    short	addr_len;
    unsigned short	port;
    unsigned char	addr[MAX_SOCK_ADDR_LEN]; 
} ZYX_SOCKADDR_STRUCT;

typedef void (*ZyxSocketHandler) (ZyxSocketEventPtr Zyx_socket_evt);
typedef void (*ZyxDnsCb) (char *pIp);

extern int ZyxSocketCreate(ZYX_SOCK_TYPE protype, ZYX_CONNECT_TYPE connecttype ,ZyxSocketHandler cb);
extern int ZyxSocketConnect(int socket_id,U32 ip,U16 port);
extern int ZyxSocketClose(int socket_id);
extern void ZyxSocketExit();
extern int ZyxSocketSend(int socket_id,char* buf,int len);
extern int ZyxSocketRecv(int socket_id, char* buf,int len);

extern int ZyxSocketBind(int socket_id, ZYX_SOCKADDR_STRUCT *addr);
extern int ZyxSocketSendTo(int socket_id,char* buf,int len,int flag, ZYX_SOCKADDR_STRUCT *addr);
extern int ZyxSocketRecvFrom(int socket_id, char* buf,int len,int flag, ZYX_SOCKADDR_STRUCT *addr);
extern int ZyxSocketGetHostByName(char *host,char *ip_address,ZyxDnsCb cb);

#endif
