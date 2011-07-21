#ifdef __ZYX_PLATFORM_SUPPORT__

#include "zyxconfig.h"
#include "soc_api.h"
#include "zyxsys.h"
#include "zyxsocket.h"
#include "zyxextern.h"

#include "adp_events.h"

extern int ZyxUsableSim;
extern BOOL ZyxCheckCallback(void* addr);

static ZyxSocketHandler ZyxSockCB = NULL;
static ZyxDnsCb ZyxDnsCallback = NULL;
static U32 Zyx_cmwap_accout = ZYX_CMWAP_ACCOUNT,Zyx_cmnet_accout = ZYX_CMNET_ACCOUNT;


#if 0
extern BOOL app_Data_Conn_Callback (COS_EVENT *pCosEvent);
extern BOOL app_SocketEventcb (COS_EVENT *pstCosEv);
extern INT32 soc_StartGPRS(CFW_SIM_ID nSimID,UINT16 nUTI );

void ZyxPrepareConnect(void)
{
	#ifdef MMI_ON_HARDWARE_P

	UINT32 retVal = 0,simId=Zyx_get_usable_sim();
	UINT8					State = 0 ;
	UINT32  result =0;
	CFW_SetDataConnFun (app_Data_Conn_Callback);//chenhe for kk,data conn cb

	CFW_SetTCPIPCallBack (app_SocketEventcb);//chenhe for kk

#ifdef __MMI_DUAL_SIM__
	result=CFW_GetGprsAttState (&State,simId);
#else
	result=CFW_GetGprsAttState (&State);
#endif

	if(State == 1) //already attached
	{
		soc_StartGPRS(simId,FREE_UTI);
	}
	else
	{
		#ifdef __MMI_DUAL_SIM__
		if(simId!=CFW_SIM_END)
		{
			UINT8 nUTI=0x00;
			CFW_GetFreeUTI(0,&nUTI);
			UINT8 nCID = 0x00;//, nState = 0x00;
			UINT32 nRet1 = 0x00;//, nRet2 = 0x00;

			nRet1 = CFW_GetFreeCID(&nCID, simId);
			mmi_trace(1,"NET_PrepareConnect,retVal is %x,simId is %d,nRet1 is %d,%d",retVal,simId,nRet1,nCID);

			retVal=CFW_GprsAtt(1, 1,simId);

		}
		#else
		retVal=CFW_GprsAtt(1, 1);
		#endif
	}
	mmi_trace(1,"NET_PrepareConnect,retVal is %x,simId is %d",retVal,simId);
	//DisplayPopup( (PU8)GetString( STR_GLOBAL_PLEASE_WAIT ), IMG_GLOBAL_LOADING, 0, 0, (U8)SAVE_TONE );
	//mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT, IMG_GLOBAL_PROGRESS);
	//WEP_ShowWaitScreen();
	
/*******test temporarily,start****/
//WEPTrueMain();
/*******test temporarily,end****/
#endif
}
#endif

static int Zyx_get_new_account_id(U32  nwk_account_id)
{
    return nwk_account_id;
}

#ifndef WIN32	
static void ZyxSocketMsg(void* inMsg)
{
    ZyxSocketEvent Zyx_socket_evt;
    app_soc_notify_ind_struct * soc_ind = (app_soc_notify_ind_struct *)inMsg;

	ZyxSysLockEnter();

    Zyx_socket_evt.socket_id = soc_ind->socket_id;
    Zyx_socket_evt.event_type = soc_ind->event_type;
    Zyx_socket_evt.result = soc_ind->result;   
    ZyxTrace("zyx msg socket=%d,event=%d,result=%d",
        Zyx_socket_evt.socket_id,Zyx_socket_evt.event_type,Zyx_socket_evt.result );

	ZyxTrace("ZyxSocketMsg Call %p before", ZyxSockCB);
	if (ZyxCheckCallback((void*)ZyxSockCB))
	{
		ZyxTrace("ZyxSocketMsg Call %p done", ZyxSockCB);
		ZyxSockCB(&Zyx_socket_evt);
	}
	ZyxTrace("ZyxSocketMsg Call %p done", ZyxSockCB);
	ZyxSysLockLeave();
}

int ZyxSocketCreate(ZYX_SOCK_TYPE protype, ZYX_CONNECT_TYPE connecttype ,ZyxSocketHandler cb)
{
    int  socket = -1;
    U8 val = 1;
    U32 saccount;

    ZyxTrace("zyx create socket prottype=%d,contype=%d",protype,connecttype);


    if(connecttype == ZYX_CMWAP)
        saccount = Zyx_cmwap_accout;
    else
        saccount = Zyx_cmnet_accout;
    ZyxTrace("zyx socket accout=%d",saccount);
    

    if (protype == ZYX_SOCK_STREAM)
        socket = soc_create(/*PF_INET, SOCK_STREAM,*/2,1, 0, MOD_MMI, saccount);
    else
        socket = soc_create(/*PF_INET, SOCK_DGRAM, */0,1,0, MOD_MMI, saccount);

    if(socket >= 0)
    {
        if( soc_setsockopt(socket, SOC_NBIO, &val, sizeof(val)) < 0 )
        {
            ZyxTrace("zyx SOC_NBIO fail");
        }
        else
        {
            val = SOC_READ | SOC_WRITE | SOC_CONNECT | SOC_CLOSE | SOC_ACCEPT;
            
            if(soc_setsockopt(socket, SOC_ASYNC, &val, sizeof(val)) < 0)
            {
                ZyxTrace("zyx set SOC_ASYNC fail");
            }
            else
            {
                ZyxSockCB = cb;
                ClearProtocolEventHandler(MSG_ID_APP_SOC_NOTIFY_IND);
                SetProtocolEventHandler(ZyxSocketMsg, MSG_ID_APP_SOC_NOTIFY_IND /*+ 3*/);
            }
        }
        ZyxTrace("zyx create socket=%d",socket);
    }
    else
    {
        ZyxTrace("zyx create socket fail");
    }
    
    return socket;
}

static U32 Zyx_htonl(U32 hostlong)
{
    U32 re = 0;
    U8 *p = (unsigned char *)&re;
    
    p[0] = (U8)((hostlong >> 24) & 0xff);
    p[1] = (U8)((hostlong >> 16) & 0xff);
    p[2] = (U8)((hostlong >> 8) & 0xff);
    p[3] = (U8)(hostlong & 0xff);
    
    return re;    
}

int ZyxSocketConnect(int socket_id,U32 ip,U16 port)
{
    sockaddr_struct addr;
    int ret_val;
    
    ip = Zyx_htonl(ip);
    memcpy(addr.addr, (char*)&ip, 4);
    addr.addr_len = 4;
    addr.port = port;
    ret_val = soc_connect((S8)socket_id, &addr);

    ZyxTrace("zyx connect socket=%d,ret=%d,ip=%x,port=%d",socket_id,ret_val,ip,port);
    if (ret_val == SOC_WOULDBLOCK)
        return 0;
    return ret_val;
}

int ZyxSocketClose(int socket_id)
{
    int ret;
    
    ZyxTrace("zyx close socket=%d",socket_id);
    ret = soc_close(socket_id);
    //ZyxSockCB = NULL;
    ZyxTrace("zyx close socket=%d,ret=%d",socket_id,ret);
    return 1;
}

extern UINT32 soc_StopGPRS();
extern void UpdateGPRSStatusIcon(U8 gprs_state);
void ZyxSocketExit()
{
	UINT32 ret = 0;
    //SetProtocolEventHandler(NULL, MSG_ID_APP_SOC_DEACTIVATE_CNF);
    ret = soc_StopGPRS();
	ZyxTrace("[gprs] exit %d\n", ret);
    
    UpdateGPRSStatusIcon(L4C_NONE_GPRS);
    //ClearProtocolEventHandler(MSG_ID_APP_SOC_NOTIFY_IND);
    //ClearProtocolEventHandler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND);
    //ClearProtocolEventHandler(MSG_ID_APP_SOC_GET_HOST_BY_ADDR_IND);

    ZyxSockCB = NULL;
}

int ZyxSocketSend(int socket_id,char* buf,int len)
{
    int ret;

    ret = soc_send(socket_id,(kal_uint8*)buf,len,0);
    ZyxTrace("zyx send socket=%d,ret=%d",socket_id,ret);
    return ret;
}

int ZyxSocketRecv(int socket_id, char* buf,int len)
{
    int ret;

    ret = soc_recv(socket_id,(kal_uint8*)buf,len,0);
    ZyxTrace("zyx recv socket=%d,ret=%d",socket_id,ret);
	
    return ret;
}

int ZyxSocketBind(int socket_id, ZYX_SOCKADDR_STRUCT *addr)
{
    int ret;
    sockaddr_struct sockaddr;

    memcpy(&sockaddr,addr,sizeof(addr));
    ret = soc_bind(socket_id,&sockaddr);
    ZyxTrace("zyx bind socket=%d,ret=%d",socket_id,ret);
	
    return ret;
}

int ZyxSocketSendTo(int socket_id,char* buf,int len,int flag, ZYX_SOCKADDR_STRUCT *addr)
{
    int ret;
    sockaddr_struct sockaddr;

    memcpy(&sockaddr,addr,sizeof(addr));
    ret = soc_sendto(socket_id,(kal_uint8*)buf,len,0,&sockaddr);
    ZyxTrace("zyx sendto socket=%d,ret=%d",socket_id,ret);
    return ret;
}

int ZyxSocketRecvFrom(int socket_id, char* buf,int len,int flag, ZYX_SOCKADDR_STRUCT *addr)
{
    int ret;
    sockaddr_struct sockaddr;

    memcpy(&sockaddr,addr,sizeof(addr));
    ret = soc_recvfrom(socket_id,(kal_uint8*)buf,len,0,&sockaddr);
    ZyxTrace("zyx recvfrom socket=%d,ret=%d",socket_id,ret);
    return ret;
}


static void ZyxDnsEvent(void * inMsg)
{
    app_soc_get_host_by_name_ind_struct *dns_ind;
    
    if( inMsg )
    {
        dns_ind = (app_soc_get_host_by_name_ind_struct*) inMsg;
        if (dns_ind->result == KAL_TRUE)
        {
             ClearProtocolEventHandler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND);
            if (ZyxCheckCallback((void*)ZyxDnsCallback))
                ZyxDnsCallback((char*)dns_ind->addr);
        }
    }
}

int ZyxSocketGetHostByName(char *host,char *ip_address,ZyxDnsCb cb)
{
    int ret;
    int addr_len = 5;
    U32 saccount;

    saccount = Zyx_get_new_account_id(Zyx_cmnet_accout);
    ZyxDnsCallback = cb;
    ret = soc_gethostbyname(KAL_FALSE, MOD_MMI, 0,(const char *)host, 
		(unsigned char*)ip_address, (unsigned char*)&addr_len, 0, saccount);
    ZyxTrace("zyx gethost by name host=%s,ip=%s,ret=%d",host,ip_address,ret);

    if (ret == SOC_WOULDBLOCK)
    {
    	SetProtocolEventHandler(ZyxDnsEvent, MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND);
    }
	
    return ret;
}
#else

#include <windows.h>
#include  <winsock.h> 

static void ZyxSocketMsg(void* inMsg)
{
    ZyxSocketEvent Zyx_socket_evt,*soc_ind;

    soc_ind = (ZyxSocketEvent *)inMsg;

    Zyx_socket_evt.socket_id = soc_ind->socket_id;
    Zyx_socket_evt.event_type = soc_ind->event_type;
    Zyx_socket_evt.result = soc_ind->result;   
    ZyxTrace("zyx msg socket=%d,event=%d,result=%d",
        Zyx_socket_evt.socket_id,Zyx_socket_evt.event_type,Zyx_socket_evt.result );
    if (ZyxCheckCallback((void*)ZyxSockCB))
        ZyxSockCB(&Zyx_socket_evt);
}

int ZyxSocketCreate(ZYX_SOCK_TYPE protype, ZYX_CONNECT_TYPE connecttype ,ZyxSocketHandler cb)
{
    int  socketid = -1;
    U8 val = 1;
    WSADATA wsa;
    
    WSAStartup(MAKEWORD(1,1),&wsa);

    if (protype == ZYX_SOCK_STREAM)
        socketid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    else
        socketid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ZyxSockCB = cb;
    
    return socketid;
}

static U32 Zyx_htonl(U32 hostlong)
{
    U32 re = 0;
    U8 *p = (unsigned char *)&re;
    
    p[0] = (U8)((hostlong >> 24) & 0xff);
    p[1] = (U8)((hostlong >> 16) & 0xff);
    p[2] = (U8)((hostlong >> 8) & 0xff);
    p[3] = (U8)(hostlong & 0xff);
    
    return re;    
}

static  ZyxSocketEvent Zyx_socket_evt;


void ReadTimerHandler (int timer_id)
{
    Zyx_socket_evt.event_type = ZYX_SOC_READ;
    ZyxSocketMsg((void*)&Zyx_socket_evt);
}

int ZyxSocketConnect(int socket_id,U32 ip,U16 port)
{
    struct sockaddr_in dest;
    int ret = 0;
    
    memset(&dest,0,sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port   = htons(port);
    dest.sin_addr.s_addr = ip;
    ret = connect(socket_id, (struct sockaddr *) &dest, sizeof(dest));

    Zyx_socket_evt.socket_id = socket_id;
    Zyx_socket_evt.result = 1;
    if(ret == 0)
    {
        Zyx_socket_evt.event_type = ZYX_SOC_CONNECT;
        ZyxSocketMsg((void*)&Zyx_socket_evt);

        ZyxStartTimer(1000, ReadTimerHandler);
    }
    
    return ret;
}

int ZyxSocketClose(int socket_id)
{
    closesocket(socket_id);
    return 1;
}

void ZyxSocketExit()
{
    WSACleanup();
}

int ZyxSocketSend(int socket_id,char* buf,int len)
{
    int ret;

    ret = send(socket_id, buf, len, 0);
    return ret;
}

int ZyxSocketRecv(int socket_id, char* buf,int len)
{
    int ret;

    ret = recv(socket_id,buf,len,0);
    return ret;
}

int ZyxSocketBind(int socket_id, ZYX_SOCKADDR_STRUCT *addr)
{
    return 0;
}
int ZyxSocketSendTo(int socket_id,char* buf,int len,int flag, ZYX_SOCKADDR_STRUCT *addr)
{
    return 0;
}

int ZyxSocketRecvFrom(int socket_id, char* buf,int len,int flag, ZYX_SOCKADDR_STRUCT *addr)
{
    return 0;
}


int ZyxSocketGetHostByName(char *host,char *ip_address,ZyxDnsCb cb)
{
    struct hostent *hptr;
    char *ip;
    
    if ( (hptr = gethostbyname(host)) != NULL )
    {
        ip = inet_ntoa(*((struct in_addr*)hptr->h_addr_list[0]));
        strcpy(ip_address, ip);
    }
    
    return 0;
}
#endif

#endif
