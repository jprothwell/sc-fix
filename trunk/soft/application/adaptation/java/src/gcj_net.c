#include "stddef.h"
#include "stdarg.h"
#include "cos.h"
#include "cswtype.h"
#include "fs_type.h"
#include "tm.h"
#include "mmi_trace.h"
#include "sxr_mem.h"

#include "cfw.h"
#include "sockets.h"
#include "soc_api.h"
#include "tcpip_tcp.h"
#include "mmi_msg_struct.h"
#include "internal.h"
#include "javadef.h"
#include "dual_sim.h"
#include "settingprofile.h"
#include "java.h"

#ifdef __GPRS_MODE__
#include "dataaccountgprot.h"
#include "dataaccount.h"
#include "dataaccountenum.h"
#include "dataaccountdef.h"
#include "dataaccountstruct.h"
#endif

typedef struct sockt_t
{
	char socketId;
	unsigned char taskId;
	unsigned char thisReadSem;
	unsigned char isWaitForGprs;
	unsigned char isPerpareForRead;
	struct sockt_t *next;
}_SOCKET_T;

_SOCKET_T socketHead;
_GCJ_MUTEX_T netMutex;

unsigned char socketReadSemaphore;
//unsigned char gprsStateSemaphore;
unsigned char gcj_IsGprsActived=0;
unsigned char gcj_socke_count=0;

void gcj_net_reset()
{
	//if (gcj_StubIsForceToQuit()  || !gcj_IsJavaOn())
		gcj_StopGprs();
	//else
	//	gcjEventAPI_StopGprs();
	_SOCKET_T *thisH = socketHead.next;
	while (thisH)
	{
		_SOCKET_T *tmp = thisH->next;
		CFW_SetTCPIPCallBackEx (NULL,thisH->socketId);
		free (thisH);
		thisH = tmp;
	}
	memset(&socketHead,0,sizeof(socketHead));
	gcj_IsGprsActived = 0;
	gcj_socke_count = 0;
	
}
void gcj_StubPlatformRequest(unsigned short *url)
{
#ifdef JATAAYU_SUPPORT
	unsigned char urlstring  [200];
	UnicodeToAnsii(urlstring,url);
	gcj_TraceOut(0,"gcj_StubPlatformRequest URL :%s",urlstring);

     	unsigned short  *urldata = OslMalloc(mmi_ucs2strlen(url)*sizeof(unsigned short)+2) ;
	if (!urldata)
		abort();
	UCS2Strncpy(urldata, url, mmi_ucs2strlen(url));
       
	COS_EVENT evt;
	evt.nEventId = (unsigned int)MSG_ID_MMI_JAVA_RUN_WAP_REQ;
	evt.nParam1 = urldata;
	gcj_StubSendEvent(&evt,MOD_MMI);
//	gcj_media_semId = gcj_StubNewSemphone();
//	gcj_StubTakeSemphone(gcj_media_semId);
//	gcj_SoundEnd = 1;
      #else

	#endif
}

void gcj_GprsCallback(void *info)
{
	mmi_ps_gprs_connect_status_ind_struct *pGprsConnetStatus = (mmi_ps_gprs_connect_status_ind_struct*)info;
	if (pGprsConnetStatus->status != 0)
	{
		gcj_IsGprsActived =0;
		DisplayPopup((PU8)GetString(STR_GLOBAL_SOC_ERROR), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8) WARNING_TONE);
		gcj_PrepareNetwork();
		return;
	}
	else
		gcj_IsGprsActived=1;
	gcj_StubTakeMutex(&netMutex);
	_SOCKET_T *thisH = socketHead.next;
	while (thisH)
	{
		if (thisH->isWaitForGprs)
		{
			//gcj_StubReleaseSemphone(thisH->thisReadSem);
			gcj_StubCondNotify(thisH->thisReadSem);
			thisH->isWaitForGprs = 0;
		}
		thisH = thisH->next;
	}
	gcj_StubReleaseMutex(&netMutex);
	gcj_TraceOut(0,"j2me: file %s,function %s\n,gcj_IsGprsActived=%d",__FILE__,__FUNCTION__,gcj_IsGprsActived);
}
unsigned char gcj_PrepareNetworkError=0;
#ifdef __MMI_GPRS_FEATURES__

extern GPRSDataAccountNode g_GPRS_DataAccount[];

#endif /* __MMI_GPRS_FEATURES__ */ 
int gcj_PrepareNetwork()
{
	int ret=-1;
	uint32 whichSim=0;
	static int fistin=1;
	U8 APN[MAX_GPRS_MMI_APN_LEN + 1];  
#ifdef __MMI_MULTI_SIM__
	SetProtocolEventHandler(gcj_GprsCallback,MSG_ID_MMI_NW_GPRS_CONNECTED_RSP);
	//	SetProtocolEventHandler(WEPTrueExit,MSG_ID_MMI_NW_GPRS_DECONNECTED_RSP);
	SetSlaveProtocolEventHandler(gcj_GprsCallback,MSG_ID_MMI_NW_GPRS_CONNECTED_RSP);
	//	SetSlaveProtocolEventHandler(WEPTrueExit,MSG_ID_MMI_NW_GPRS_DECONNECTED_RSP);

	whichSim = g_mmi_java_sim_id ;
#else
	SetProtocolEventHandler(gcj_GprsCallback,MSG_ID_MMI_NW_GPRS_CONNECTED_RSP);
	whichSim = 0;
#endif
	memset(APN, 0x00, sizeof(APN));
	UnicodeToAnsii(APN, g_GPRS_DataAccount[g_mmi_java_wap_prof_id].APN);
	gcj_TraceOut(0,"j2me: gcj_PrepareNetwork whichSim=%d APN = %s",whichSim, APN);
	ret = adp_PrepareGPRS(whichSim , strlen(APN) , ( char *)APN);
	gcj_TraceOut(0,"j2me: task=%d,file %s,function %s,ret=0x%x",sxr_GetCurrentTaskId(),__FILE__,__FUNCTION__,ret);
	if(ret != 0)
	{
		//gcj_StubNetAlert();
	}
    return ret;
}

int gcj_StopGprs()
{
	ClearProtocolEventHandler(MSG_ID_MMI_NW_GPRS_CONNECTED_RSP);
#ifdef __MMI_MULTI_SIM__	
	ClearSlaveProtocolEventHandler(MSG_ID_MMI_NW_GPRS_CONNECTED_RSP);
#endif
	ClearProtocolEventHandler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND);
	int ret = soc_StopGPRS();
	return ret;
}

int gcj_GetTcpipSocket()
{
	return CFW_TcpipSocket(CFW_TCPIP_AF_INET,CFW_TCPIP_SOCK_STREAM,CFW_TCPIP_IPPROTO_IP);
}

int gcj_SetSocketOpt(SOCKET socketId,unsigned short option, unsigned char *value, unsigned char value_size)
{
	soc_setsockopt(socketId,option,value,value_size);
}
int gcj_GetHostName(const unsigned char * domain,unsigned char *address,unsigned char *address_len)
{
	return soc_gethostbyname(0, 0, 0, domain, address, address_len, 0, 0);
}
#define USE_EVENT_API
#ifdef USE_EVENT_API
#define DEBUG_SIGN   -12345678
int gcjEventApi_NetRet= DEBUG_SIGN;
extern unsigned char netResultSem;
#define MAX_SEM 20
unsigned char sempArray[MAX_SEM];
volatile unsigned int eventApiNetInProcess=0;
extern unsigned char QueueEventInputMbox;
extern unsigned char QueueEventTaskInEventApi;

unsigned char CheckSempInEventApi(unsigned char semp);

void EventNetworkFinish(unsigned char semp,int *ret,int result)
{
/*
	sxr_TakeSemaphore(netResultSem);
	if (gcjEventApi_NetRet != DEBUG_SIGN)
		abort();
	gcjEventApi_NetRet = result;
*/	
	if (gcj_StubIsForceToQuit()  || !gcj_IsJavaOn() ||!CheckSempInEventApi(semp) )
	{
		//gcjEventApi_NetRet = DEBUG_SIGN;
		//sxr_ReleaseSemaphore(netResultSem);
		gcj_TraceOut(1,"EventNetworkFinish semp=%d,result=%d,eventApiNetInProcess=%d\n",semp,result,eventApiNetInProcess);
		for (int i=0;i<MAX_SEM;i++)
		{
			gcj_TraceOut(1,"sempArray[%d]=%d",i,sempArray[i]);
		}
		return;
	}
	if (*ret != DEBUG_SIGN)
		abort();
	*ret = result;	
	gcj_StubReleaseSemphone(semp);
	//CheckAndRemoveAllPending(semp);
}

void EventNetworkFinishNoRet(unsigned char semp,int result)
{

}
void recordSempInEventApi(unsigned semp)
{
	unsigned int status;
	int i;
	//gcj_TraceOut(0,"recordSempInEventApi semp =%d",semp);
	status = sxr_EnterSc();
	if (QueueEventInputMbox!=0 && QueueEventInputMbox==semp)
		QueueEventTaskInEventApi=1;
	eventApiNetInProcess++;
	for(i=0;i<MAX_SEM;i++)
	{
		if (sempArray[i]==0)
		{
			sempArray[i] = semp;
			break;
		}
	}
	if (i==MAX_SEM)
		abort();
	sxr_ExitSc(status);
}

void removeSempInEventApi(unsigned semp)
{
	unsigned int status;
	int i;
	//gcj_TraceOut(0,"removeSempInEventApi semp =%d",semp);
	status = sxr_EnterSc();
	if (QueueEventInputMbox!=0 && QueueEventInputMbox==semp)
		QueueEventTaskInEventApi=0;
	for (i=0;i<MAX_SEM;i++)
	{
		if (sempArray[i] == semp)
		{
			sempArray[i]=0;
			break;
		}
	}
	if (i==MAX_SEM)
		abort();
	eventApiNetInProcess--;
	sxr_ExitSc(status);
}

unsigned char CheckAndRemoveSempInEventApi(unsigned char semp)
{
	unsigned int status;
	int i;
	gcj_TraceOut(0,"CheckAndRemoveSempInEventApi semp =%d",semp);	
	status = sxr_EnterSc();
	if (eventApiNetInProcess>0)
	{
		for (i=0;i<MAX_SEM;i++)
		{
			if (sempArray[i] == semp)
			{
				sempArray[i]=0;
				eventApiNetInProcess--;
				sxr_ExitSc(status);
				return TRUE;
			}
		}
	}
	sxr_ExitSc(status);
	return FALSE;
}
unsigned char CheckSempInEventApi(unsigned char semp)
{
	unsigned int status;
	int i;
	status = sxr_EnterSc();
	if (eventApiNetInProcess>0)
	{
		for (i=0;i<MAX_SEM;i++)
		{
			if (sempArray[i] == semp)
			{
				sxr_ExitSc(status);
				return TRUE;
			}
		}
	}
	sxr_ExitSc(status);
	return FALSE;
}

void gcjEventAPI_PrepareNetwork()
{
	int ret = DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	gcj_TraceOut(0,"func:%s,tmpSem=%d\n",__FUNCTION__,tmpSem);	
	COS_EVENT evt;
	_NET_EVENT_T *data = (_NET_EVENT_T *)OslMalloc(sizeof (_NET_EVENT_T));
	if (!data)
		abort();
	data->semp = tmpSem;
	data->finish = EventNetworkFinishNoRet;
	//data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_PREPARE_GPRS_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_MMI);
/*	
	recordSempInEventApi(tmpSem);
	gcj_StubTakeSemphone(tmpSem);
	ret = gcjEventApi_NetRet;
	if (gcjEventApi_NetRet == DEBUG_SIGN)
		abort();
	gcjEventApi_NetRet = DEBUG_SIGN;
	removeSempInEventApi(tmpSem);
	//sxr_ReleaseSemaphore(netResultSem);
*/	
}

void gcjEventAPI_StopGprs()
{
	int ret = DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	gcj_TraceOut(0,"func:%s,tmpSem=%d\n",__FUNCTION__,tmpSem);	
	COS_EVENT evt;
	_NET_EVENT_T *data = (_NET_EVENT_T *)OslMalloc(sizeof (_NET_EVENT_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->finish = EventNetworkFinishNoRet;
	//data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_STOP_GPRS_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_MMI);	
/*	
	recordSempInEventApi(tmpSem);
	gcj_StubTakeSemphone(tmpSem);
	ret = gcjEventApi_NetRet;
	if (gcjEventApi_NetRet == DEBUG_SIGN)
		abort();
	gcjEventApi_NetRet = DEBUG_SIGN;
	
	removeSempInEventApi(tmpSem);
	//sxr_ReleaseSemaphore(netResultSem);
*/	
}

int gcjEventAPI_GetTcpipSocket()
{
	int ret = DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	gcj_TraceOut(0,"func:%s,tmpSem=%d\n",__FUNCTION__,tmpSem);	
	COS_EVENT evt;
	_NET_EVENT_T *data = (_NET_EVENT_T *)OslMalloc(sizeof (_NET_EVENT_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->finish = EventNetworkFinish;
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_GET_TCPIP_SOCKET_REQ;
	evt.nParam1 = data;
	recordSempInEventApi(tmpSem);	
	gcj_StubSendEvent(&evt,MOD_MMI);	
	gcj_StubTakeSemphone(tmpSem);
/*	
	ret = gcjEventApi_NetRet;
	if (gcjEventApi_NetRet == DEBUG_SIGN)
		abort();
	gcjEventApi_NetRet = DEBUG_SIGN;
*/	
	removeSempInEventApi(tmpSem);
	//sxr_ReleaseSemaphore(netResultSem);
	if (ret == DEBUG_SIGN)
		abort();
	return ret;
}



int gcjEventAPI_SetSocketOpt(SOCKET socketId,unsigned short option, unsigned char *value, unsigned char value_size)
{
	int ret = DEBUG_SIGN;
	if (socketId>5)
		abort();
	int tmpSem = gcj_StubNewSemphone();
	gcj_TraceOut(0,"func:%s,tmpSem=%d\n",__FUNCTION__,tmpSem);	
	COS_EVENT evt;
	_NET_EVENT_SET_OPT_T *data = (_NET_EVENT_SET_OPT_T *)OslMalloc(sizeof (_NET_EVENT_SET_OPT_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->socketId = socketId;
	data->option = option;
	data->value = value;
	data->value_size = value_size;
	data->finish = EventNetworkFinish;
	data->result = &ret;	
	evt.nEventId = (unsigned int)MSG_ID_GCJ_SET_SOCKET_OPT_REQ;
	evt.nParam1 = data;
	recordSempInEventApi(tmpSem);	
	gcj_StubSendEvent(&evt,MOD_MMI);	
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_NetRet;
	if (gcjEventApi_NetRet == DEBUG_SIGN)
		abort();
	gcjEventApi_NetRet = DEBUG_SIGN;
	*/
	removeSempInEventApi(tmpSem);
	//sxr_ReleaseSemaphore(netResultSem);
	if (ret == DEBUG_SIGN)
		abort();
	return ret;
}


int gcjEventAPI_GetHostName(const unsigned char * domain,unsigned char *address,unsigned char *address_len)
{
	int ret = DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	gcj_TraceOut(0,"func:%s,tmpSem=%d\n",__FUNCTION__,tmpSem);	
	COS_EVENT evt;
	_NET_EVENT_GET_HOST_T *data = (_NET_EVENT_GET_HOST_T *)OslMalloc(sizeof (_NET_EVENT_GET_HOST_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->domain = domain;
	data->address = address;
	data->adress_len = address_len;
	data->finish = EventNetworkFinish;
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_GET_HOST_NAME_REQ;
	evt.nParam1 = data;
	recordSempInEventApi(tmpSem);	
	gcj_StubSendEvent(&evt,MOD_MMI);	
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_NetRet;
	if (gcjEventApi_NetRet == DEBUG_SIGN)
		abort();
	gcjEventApi_NetRet = DEBUG_SIGN;
	*/
	removeSempInEventApi(tmpSem);
	//sxr_ReleaseSemaphore(netResultSem);
	if (ret == DEBUG_SIGN)
		abort();
	return ret;
}


int gcjEventAPI_SocketConnect(SOCKET nSocket,  CFW_TCPIP_SOCKET_ADDR *pName, UINT8 nNameLen)
{
	int ret = DEBUG_SIGN;
	
	if (nSocket>5)
		abort();
	int tmpSem = gcj_StubNewSemphone();
	gcj_TraceOut(0,"func:%s,tmpSem=%d\n",__FUNCTION__,tmpSem);	
	COS_EVENT evt;
	_NET_EVENT_SOC_CON_T *data = (_NET_EVENT_SOC_CON_T *)OslMalloc(sizeof (_NET_EVENT_SOC_CON_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->nSocket = nSocket;
	data->pName = pName;
	data->nNameLen = nNameLen;
	data->finish = EventNetworkFinish;
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_SOCKET_CONNECT_REQ;
	evt.nParam1 = data;
	recordSempInEventApi(tmpSem);	
	gcj_StubSendEvent(&evt,MOD_MMI);	
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_NetRet;
	if (gcjEventApi_NetRet == DEBUG_SIGN)
		abort();
	gcjEventApi_NetRet = DEBUG_SIGN;
	*/
	removeSempInEventApi(tmpSem);
	//sxr_ReleaseSemaphore(netResultSem);
	if (ret == DEBUG_SIGN)
		abort();	
	return ret;
}


int gcjEventAPI_SocketRead(SOCKET nSocket ,char *inBuf,int maxLen)
{
	int ret = DEBUG_SIGN;
	
	if (nSocket>5)
		abort();
	int tmpSem = gcj_StubNewSemphone();
	gcj_TraceOut(0,"func:%s,tmpSem=%d\n",__FUNCTION__,tmpSem);
	COS_EVENT evt;
	_NET_EVENT_SOC_READ_T *data = (_NET_EVENT_SOC_READ_T *)OslMalloc(sizeof (_NET_EVENT_SOC_READ_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->nSocket = nSocket;
	data->inBuf = inBuf;
	data->maxLen = maxLen;
	data->finish = EventNetworkFinish;
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_SOCKET_READ_REQ;
	evt.nParam1 = data;
	recordSempInEventApi(tmpSem);	
	gcj_StubSendEvent(&evt,MOD_MMI);	
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_NetRet;
	if (gcjEventApi_NetRet == DEBUG_SIGN)
		abort();
	gcjEventApi_NetRet = DEBUG_SIGN;
	*/
	removeSempInEventApi(tmpSem);
	//sxr_ReleaseSemaphore(netResultSem);
	if (ret == DEBUG_SIGN)
		abort();
	
	return ret;
}


int gcjEventAPI_SocketWrite(SOCKET nSocket ,char *outBuf,int writeLen)
{
	int ret = DEBUG_SIGN;
	
	if (nSocket>5)
		abort();
	int tmpSem = gcj_StubNewSemphone();
	gcj_TraceOut(0,"func:%s,tmpSem=%d\n",__FUNCTION__,tmpSem);	
	COS_EVENT evt;
	_NET_EVENT_SOC_WRITE_T *data = (_NET_EVENT_SOC_WRITE_T *)OslMalloc(sizeof (_NET_EVENT_SOC_WRITE_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->nSocket = nSocket;
	data->outBuf = outBuf;
	data->writeLen = writeLen;
	data->finish = EventNetworkFinish;	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_SOCKET_WRITE_REQ;
	evt.nParam1 = data;
	recordSempInEventApi(tmpSem);	
	gcj_StubSendEvent(&evt,MOD_MMI);	
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_NetRet;
	if (gcjEventApi_NetRet == DEBUG_SIGN)
		abort();
	gcjEventApi_NetRet = DEBUG_SIGN;
	*/
	removeSempInEventApi(tmpSem);
	//sxr_ReleaseSemaphore(netResultSem);
	if (ret == DEBUG_SIGN)
		abort();
	
	return ret;
}


int gcjEventAPI_SocketClose(SOCKET nSocket)
{
	int ret = DEBUG_SIGN;
	
	if (nSocket>5)
		abort();
	int tmpSem = gcj_StubNewSemphone();
	gcj_TraceOut(0,"func:%s,tmpSem=%d\n",__FUNCTION__,tmpSem);	
	COS_EVENT evt;
	_NET_EVENT_SOC_CLOSE_T *data = (_NET_EVENT_SOC_CLOSE_T *)OslMalloc(sizeof (_NET_EVENT_SOC_CLOSE_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->nSocket = nSocket;
	data->finish = EventNetworkFinish;	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_SOCKET_CLOSE_REQ;
	evt.nParam1 = data;
	recordSempInEventApi(tmpSem);	
	gcj_StubSendEvent(&evt,MOD_MMI);	
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_NetRet;
	if (gcjEventApi_NetRet == DEBUG_SIGN)
		abort();
	gcjEventApi_NetRet = DEBUG_SIGN;
	*/
	removeSempInEventApi(tmpSem);
	//sxr_ReleaseSemaphore(netResultSem);
	
	if (ret == DEBUG_SIGN)
		abort();
	return ret;
}

#else
#define gcjEventAPI_PrepareNetwork gcj_PrepareNetwork
#define gcjEventAPI_StopGprs  gcj_StopGprs
#define gcjEventAPI_GetTcpipSocket	gcj_GetTcpipSocket
#define gcjEventAPI_SetSocketOpt gcj_SetSocketOpt
#define gcjEventAPI_GetHostName gcj_GetHostName
#define gcjEventAPI_SocketConnect CFW_TcpipSocketConnect
#define gcjEventAPI_SocketRead CFW_TcpipSocketRead
#define gcjEventAPI_SocketWrite CFW_TcpipSocketWrite
#define gcjEventAPI_SocketClose CFW_TcpipSocketClose

#endif
void gcj_StubNetworkInitialize()
{
	gcj_TraceOut(0,"gcj_StubNetworkInitialize");
	static int incount;
	gcj_StubInitMutex(&netMutex);
	gcjEventAPI_PrepareNetwork();
	if (incount++)
		abort();
/*	
	if (netResultSem != 0)
		abort();
	netResultSem = sxr_NewSemaphore(1);
*/	
}
unsigned char gcj_dnsBuff[256];
unsigned char gcj_dnsLen;
//char gcj_sockedId=-1;
BOOL gcj_SocketEventcb (COS_EVENT *pstCosEv)
{
            kal_uint8 addr[4];
            kal_uint8 addr_len;
	kal_char  *hostname;
       kal_int8      nsimID;
       kal_int8      ncid;
       kal_int8 count=5;
       int i;
	BOOL bIsProcessed = TRUE;

	gcj_TraceOut(g_sw_ADP_GPRS," gcj_SocketEventcb :nEventId [%d] nParam1[%d] nParam2[%d] nParam3[%d]",pstCosEv->nEventId,
		pstCosEv->nParam1,pstCosEv->nParam2,pstCosEv->nParam3) ;
	gcj_StubTakeMutex(&netMutex);
	_SOCKET_T *thisH = socketHead.next;
	while (thisH)
	{
		if (thisH->socketId == pstCosEv->nParam1)
			break;
		else
			thisH = thisH->next;
	}
	gcj_StubReleaseMutex(&netMutex);
	gcj_TraceOut(0,"gcj_SocketEventcb, thisH=0x%x\n",thisH);
	if (thisH == NULL)
		return;
	switch (pstCosEv->nEventId)
	{
		case EV_CFW_TCPIP_SOCKET_CONNECT_RSP:
			gcj_TraceOut(g_sw_ADP_GPRS," EV_CFW_TCPIP_SOCKET_CONNECT_RSP Func: %s ", __FUNCTION__);
			if (thisH != NULL)
			{
				thisH->isWaitForGprs = 0;
				gcj_StubCondNotify(thisH->thisReadSem);
			}
			break ;
	
	    	case EV_CFW_TCPIP_SOCKET_CLOSE_RSP:
			gcj_TraceOut(g_sw_ADP_GPRS," EV_CFW_TCPIP_SOCKET_CLOSE_RSP Func: %s ", __FUNCTION__);
			break ;
			
	    	case EV_CFW_TCPIP_SOCKET_SEND_RSP:
			gcj_TraceOut(g_sw_ADP_GPRS," EV_CFW_TCPIP_SOCKET_SEND_RSP Func: %s ", __FUNCTION__);
			break ;
		case EV_CFW_TCPIP_REV_DATA_IND:
			gcj_TraceOut(g_sw_ADP_GPRS," EV_CFW_TCPIP_REV_DATA_IND Func: %s %d", __FUNCTION__, pstCosEv->nParam2);
			if (thisH == NULL)
				abort();
			//while (thisH->isPerpareForRead == 0 && count--)
			//	COS_Sleep(100);
			/*
			if (thisH != NULL && thisH->isPerpareForRead)
			{
				gcj_StubReleaseSemphone(thisH->thisReadSem);
				thisH->isPerpareForRead = 0;
			}
			*/
			if (thisH != NULL)
			{
				gcj_StubCondNotify(thisH->thisReadSem);
			}
			break ;
		case EV_CFW_TCPIP_ACCEPT_IND:
			gcj_TraceOut(g_sw_ADP_GPRS," EV_CFW_TCPIP_ACCEPT_IND Func: %s ", __FUNCTION__);
			break ;
		case EV_CFW_DNS_RESOLV_SUC_IND:
			gcj_TraceOut(g_sw_ADP_GPRS," EV_CFW_DNS_RESOLV_SUC_IND Func: %s ", __FUNCTION__);
			break ;
		case EV_CFW_DNS_RESOLV_ERR_IND:				 
			gcj_TraceOut(g_sw_ADP_GPRS,"SOC: EV_CFW_DNS_RESOLV_ERR_IND Func: %s ", __FUNCTION__);
			break ;

		case EV_CFW_TCPIP_ERR_IND:
			gcj_TraceOut(TRUE,"SOC: EV_CFW_TCPIP_ERR_IND Func: %s ,cause=%d,detail_cause=%d", __FUNCTION__,pstCosEv->nParam2, CFW_TcpipGetLastError());
			break ;

		case EV_CFW_TCPIP_CLOSE_IND:
			//if (thisH != NULL)
			//	gcj_StubSocketClose((int)thisH);
			gcj_TraceOut(g_sw_ADP_GPRS,"SOC: EV_CFW_TCPIP_CLOSE_IND Func: %s ", __FUNCTION__);
			break ;
			
		default:
			gcj_TraceOut(g_sw_ADP_GPRS,"SOC: jdd_SocketEventcb : Unknown case") ;
			break ;
	}

	return bIsProcessed ;
		
}

static void gcjDnsEvent(void * inMsg)
{
    app_soc_get_host_by_name_ind_struct *dns_ind;
    gcj_dnsLen=0xff;
    gcj_TraceOut(0,"j2me: file %s,function %s,inMsg=%x",__FILE__,__FUNCTION__,inMsg);
    if( inMsg )
    {
        dns_ind = (app_soc_get_host_by_name_ind_struct*) inMsg;
        if (dns_ind->result == KAL_TRUE)
        {
             ClearProtocolEventHandler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND);
		memcpy(gcj_dnsBuff,dns_ind->addr,dns_ind->addr_len);
		gcj_dnsLen=dns_ind->addr_len;
		gcj_TraceOut(0,"j2me: file %s,function %s,,gcj_dnsLen=%d",__FILE__,__FUNCTION__,gcj_dnsLen);
        }
    }
    gcj_StubTakeMutex(&netMutex);
    _SOCKET_T *thisH = socketHead.next;
    while (thisH)
    {
    	if (thisH->isWaitForGprs)
    	{
    		gcj_StubReleaseSemphone(thisH->thisReadSem);
    		thisH->isWaitForGprs = 0;
    	}
    	thisH = thisH->next;
    }
    gcj_StubReleaseMutex(&netMutex);
}


static char* CharType2String(unsigned char inputchar, char *buffer) {
    char *pstr = buffer;
    if (inputchar == 0)
    {
    	*pstr++ = '0';
	*pstr = '\0';
	return buffer;
    }
    if (inputchar/100 >0)
		pstr+=2;
    else if (inputchar/10>0)
		pstr +=1;
    pstr++;
    *pstr = '\0';
    do {
		pstr--;
		*pstr = ((inputchar % 10)+'0');
		inputchar = inputchar/10;
    }while (inputchar>0);
    return buffer;
}

void gcj_StubNetAlert()
{
    if (!gcj_StubIsPrepareToQuit())
    	j2me_send_alertuser((PU8)GetString(STR_GLOBAL_SOC_ERROR),UCS2Strlen((const S8*)GetString(STR_GLOBAL_SOC_ERROR)));
}
void netCleanErrorSocket(_SOCKET_T *thisH)
{
	char Id = thisH->socketId;
	gcj_TraceOut(0,"netCleanErrorSocket thisH=0x%x,socketId = %d\n",thisH,Id);
	if (Id>=0)
	{
	
		CFW_SetTCPIPCallBackEx (NULL,thisH->socketId);
		gcjEventAPI_SocketClose(thisH->socketId);
	}
	gcj_StubTakeMutex(&netMutex);
	if (socketHead.next == thisH)
		socketHead.next = thisH->next;
	else
	{
		_SOCKET_T *tmp = socketHead.next;
		int found=0;
		while (tmp)
		{
			if (tmp->next == thisH)
			{
				tmp->next = thisH->next;
				break;
			}
			else
				tmp = tmp->next;
		}
	}
	gcj_StubReleaseMutex(&netMutex);
	free(thisH);
	///if (gcj_socke_count >20 && Id == -1)
	//	gcj_net_reset();
	gcj_StubNetAlert();
	gcj_StubThreadSleep(2000);
}

void gcj_net_reset_mailId(unsigned char mailBoxId)
{
#if 1
	_SOCKET_T *found=NULL;
	unsigned int status = sxr_EnterSc();
	if (socketHead.next != NULL && socketHead.next->thisReadSem == mailBoxId)
	{
		found = socketHead.next;
		socketHead.next = found->next;
	}
	else
	{
		_SOCKET_T *tmp = socketHead.next;
		_SOCKET_T *prev=NULL; 
		while (tmp)
		{
			if (tmp->thisReadSem  == mailBoxId)
			{
				found = tmp;
				prev->next = tmp->next;
				break;
			}
			prev = tmp;
			tmp = tmp->next;
		}
	}
	sxr_ExitSc(status);
	if (found)
	{
		gcj_TraceOut(0,"gcj_net_reset_mailId found=0x%x,mailBoxId=%d,socketId = %d,socketHead.nex=0x%x\n",found,mailBoxId,found->socketId,socketHead.next);		
		CFW_SetTCPIPCallBackEx (NULL,found->socketId);
		CFW_TcpipSocketClose(found->socketId);
		if (CheckAndRemoveSempInEventApi(found->thisReadSem))
		{
			//gcjEventApi_NetRet = DEBUG_SIGN;
			//sxr_ReleaseSemaphore(netResultSem);
			gcj_TraceOut(0,"CheckAndRemoveSempInEventApi remove sem =%d%x\n",found->thisReadSem); 	
		}
		free(found);
		//if (socketHead.next==NULL)
		//	gcj_net_reset();		
	}
#else
#ifdef USE_EVENT_API
	if (CheckAndRemoveSempInEventApi(mailBoxId))
	{
		gcj_TraceOut(0,"CheckAndRemoveSempInEventApi remove sem =%d%x\n",mailBoxId);	
	}
#endif
#endif	
}
/**
 * Opens a TCP connection to a server.
 * @param ipAddress Byte array that represents a raw IP address
 * @param port TCP port at host
 * @return a native handle to the network connection.
 */
int gcj_StubSocketOpen(char *ipAddress, int port)
{
	int handle=-1;
	int ret;
	unsigned char val=1;
	CFW_TCPIP_SOCKET_ADDR stSockConnect;
	unsigned char buff[5];
	unsigned char ip_validity=0;
	_SOCKET_T *thisH = OslMalloc(sizeof (_SOCKET_T));	
	if (!thisH)
		return -1;
	memset(thisH,0,sizeof(_SOCKET_T));
	gcj_StubTakeMutex(&netMutex);
	if (socketHead.next == NULL)
		socketHead.next = thisH;
	else
	{
		_SOCKET_T *tmp = socketHead.next;
		while (tmp)
		{
			if (tmp->next == NULL)
			{
				tmp->next = thisH;
				break;
			}
			else
				tmp = tmp->next;
		}
	}
	gcj_StubReleaseMutex(&netMutex);
	gcj_socke_count++;
	gcj_TraceOut(0,"j2me: task=%d, file %s,function %s,ipAddress=%s,port=%d,gcj_socke_count=%d,gcj_IsGprsActived=%d",sxr_GetCurrentTaskId(),__FILE__,__FUNCTION__,ipAddress,port,gcj_socke_count,gcj_IsGprsActived);
	//if (gcj_sockedId>=0)
	//	abort();
	//ret = soc_ip_check((kal_char *)ipAddress, buff, &ip_validity);
	//if (!ip_validity ||!ret )
	//	return -1;
	thisH->thisReadSem = gcj_StubNewSemphone();
	unsigned char count=0;
	while (!gcj_IsGprsActived && count <5) 
	{
		//gprsStateSemaphore = gcj_StubNewSemphone();
		//gcjEventAPI_PrepareNetwork();
		thisH->isWaitForGprs = 1;
		//gcj_StubTakeSemphone(thisH->thisReadSem);
		gcj_StubThreadSleep(5000);
		count ++;
	}
	thisH->socketId = -1;
	if (!gcj_IsGprsActived)
	{
		gcj_TraceOut(0,"j2me:function %s,line %d,ipAddress=%s,handle=%d",__FUNCTION__,__LINE__,ipAddress,thisH->socketId);
		netCleanErrorSocket(thisH);
		return -1;
	}
	thisH->socketId = gcjEventAPI_GetTcpipSocket();//CFW_TcpipSocket(CFW_TCPIP_AF_INET,CFW_TCPIP_SOCK_STREAM,CFW_TCPIP_IPPROTO_IP);
	gcj_TraceOut(0,"j2me: file %s,function %s,ipAddress=%s,handle=%d",__FILE__,__FUNCTION__,ipAddress,thisH->socketId);
	if (thisH==NULL || thisH->socketId <0)
	{
		gcj_TraceOut(0,"j2me:function %s,line %d,ipAddress=%s,handle=%d",__FUNCTION__,__LINE__,ipAddress,thisH->socketId);
		netCleanErrorSocket(thisH);
		return -1;
	}
	else
	{
		/*
		// set socket to  non-blocking mode
		ret =gcjEventAPI_SetSocketOpt(thisH->socketId, SOC_NBIO, &val, sizeof(val));
		gcj_TraceOut(1,"gcj_StubSocketOpen: soc_setsockopt, ,ret=%d",ret);
		  if( ret< 0 )
		  {
			  gcj_TraceOut(0,"j2me:function %s,line %d,ipAddress=%s,handle=%d,ret =%d",__FUNCTION__,__LINE__,ipAddress,thisH->socketId,ret);
			  netCleanErrorSocket(thisH);
			  return -1;
		  }
		// set socket to Async mode
		val = SOC_READ | SOC_WRITE | SOC_CONNECT | SOC_CLOSE | SOC_ACCEPT;
	
		ret = gcjEventAPI_SetSocketOpt(thisH->socketId, SOC_ASYNC, &val, sizeof(val));
		gcj_TraceOut(1,"gcj_StubSocketOpen: setsockopt SOC_ASYNC,ret=%d",ret);
		if(ret < 0)
		{
			gcj_TraceOut(0,"j2me:function %s,line %d,ipAddress=%s,handle=%d,ret =%d",__FUNCTION__,__LINE__,ipAddress,thisH->socketId,ret);
			netCleanErrorSocket(thisH);
			return -1;
		}
		*/
		ret=gcjEventAPI_GetHostName((const kal_char *)ipAddress, gcj_dnsBuff, &gcj_dnsLen);
		gcj_TraceOut(1,"soc_gethostbyname,re=%d",ret);
		if (ret == SOC_WOULDBLOCK)
		{
			thisH->isWaitForGprs = 1;
			SetProtocolEventHandler( gcjDnsEvent, MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND/*MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND*/);
			gcj_StubTakeSemphone(thisH->thisReadSem);
			if (gcj_dnsLen == 0xff)
				ret = SOC_ERROR;
			else
				ret = SOC_SUCCESS;
		}
		else if (ret != 0)
		{
			gcj_TraceOut(0,"j2me:function %s,line %d,ipAddress=%s,handle=%d,ret =%d",__FUNCTION__,__LINE__,ipAddress,thisH->socketId,ret);
			netCleanErrorSocket(thisH);
			return -1;
		}
		if(ret == SOC_SUCCESS)
		{
			// Step 3: Connect the Socket			
			UINT32 s_addr = (gcj_dnsBuff[0]<<24)||(gcj_dnsBuff[1]<<16)||(gcj_dnsBuff[2]<<8)||(gcj_dnsBuff[3]<<0);
			gcj_TraceOut(1,"gcj_StubSocketOpen: sock_addr,%d.%d.%d.%d:%d",gcj_dnsBuff[0],gcj_dnsBuff[1],gcj_dnsBuff[2],gcj_dnsBuff[3],port);
			CFW_SetTCPIPCallBackEx (gcj_SocketEventcb,thisH->socketId);
			char buff[20]="";
			char tmp[4];
			strcat(buff,CharType2String(gcj_dnsBuff[0],tmp));
			strcat(buff,".");
			strcat(buff,CharType2String(gcj_dnsBuff[1],tmp));
			strcat(buff,".");
			strcat(buff,CharType2String(gcj_dnsBuff[2],tmp));
			strcat(buff,".");
			strcat(buff,CharType2String(gcj_dnsBuff[3],tmp));
			gcj_TraceOut(0,"ipAddress = %s\n",buff);
			memset (&stSockConnect, 0, sizeof (CFW_TCPIP_SOCKET_ADDR)) ;
			stSockConnect.sin_len = 0 ;
			stSockConnect.sin_family = CFW_TCPIP_AF_INET ;
			stSockConnect.sin_port = htons(port) ;
			//stSockConnect.sin_addr.s_addr = *pcIpAddress ;
			stSockConnect.sin_addr.s_addr = CFW_TcpipInetAddr(buff) ;
			if ((ret = gcjEventAPI_SocketConnect (thisH->socketId, &stSockConnect, sizeof (CFW_TCPIP_SOCKET_ADDR)))== SOCKET_ERROR)
			{
				gcj_TraceOut(0,"j2me:function %s,line %d,ipAddress=%s,handle=%d,ret =%d",__FUNCTION__,__LINE__,ipAddress,thisH->socketId,ret);
				netCleanErrorSocket(thisH);
				return -1;
			}
			else
			{
				unsigned char count=0;
				thisH->isWaitForGprs = 1;
				while (count <5 && thisH->isWaitForGprs) 
				{
					//gprsStateSemaphore = gcj_StubNewSemphone();
					//gcjEventAPI_PrepareNetwork();
					//gcj_StubTakeSemphone(thisH->thisReadSem);
					gcj_StubThreadSleep(5000);
					count ++;
				}
				if (thisH->isWaitForGprs)
				{
					gcj_TraceOut(0,"j2me:function %s,line %d,ipAddress=%s,handle=%d,ret =%d",__FUNCTION__,__LINE__,ipAddress,thisH->socketId,ret);
					netCleanErrorSocket(thisH);
					return -1;
				}
			}
		}
		else 
		{
			gcj_TraceOut(0,"j2me:function %s,line %d,ipAddress=%s,handle=%d,ret =%d",__FUNCTION__,__LINE__,ipAddress,thisH->socketId,ret);
			netCleanErrorSocket(thisH);
			return -1;
		} 		
	}
	return (int)thisH;
}

void gcj_StubWaitForIO(void)
{
/*
	unsigned char taskId = sxr_GetCurrentTaskId();
	gcj_StubTakeMutex(&netMutex);
	_SOCKET_T *tmp = socketHead.next;
	while (tmp)
	{
		if (tmp->taskId == taskId)
		{
			tmp->isPerpareForRead = 1;
			break;
		}
		tmp = tmp->next;
	}
	gcj_StubReleaseMutex(&netMutex);
	if (tmp != NULL)
	{
		gcj_TraceOut(0,"gcj_StubWaitForIO taskId=%d, Sem = %d\n",taskId,tmp->thisReadSem);
		gcj_StubTakeSemphone(tmp->thisReadSem);
	}
	else
		gcj_TraceOut(0,"Error gcj_StubWaitForIO taskId=%d\n",taskId);
	*/
	gcj_StubThreadSleep(20000);
}

/**
 * Reads from the open socket connection.
 * @param inBuf the buffer into which the data is read.
 * @param maxLen the maximum number of bytes to read.
 *
 * @return the total number of bytes read into the buffer, or
 *         <tt>-1</tt> if there is no more data because the end of
 *         the stream has been reached.
 */

int gcj_StubSocketRead(int handle,char *inBuf,int maxLen)
{
	int ret = -1;
	_SOCKET_T *thisH=(_SOCKET_T *)handle;
	if (thisH==NULL || thisH->socketId<0 || socketHead.next==NULL)
		return -1;	
	gcj_StubTakeMutex(&netMutex);
	_SOCKET_T *tmp = socketHead.next;
	while (tmp)
	{
		if (tmp == thisH)
		{
			thisH->isPerpareForRead = 1;
			thisH->taskId = sxr_GetCurrentTaskId();
			thisH->thisReadSem = gcj_StubNewSemphone();
			break;
		}
		tmp = tmp->next;
	}
	gcj_StubReleaseMutex(&netMutex);	
	if (tmp == NULL)
		return -1;
	ret = gcjEventAPI_SocketRead(thisH->socketId,inBuf,maxLen);
	gcj_TraceOut(0,"j2me: file %s,function %s,handle=%d,taskId=%d,inBuf=%x,maxLength=%d,ret=%d\n",__FILE__,__FUNCTION__,thisH->socketId,thisH->taskId,inBuf,maxLen,ret);
	if (ret >0)
	{
	/*
		char *dump=OslMalloc(ret+1);
		for(int i=0;i<ret;i++)
			dump[i] = inBuf[i];
		dump[ret] = '\0';
		gcj_TraceOut(0,"dump:%s\n",dump);
		free(dump);
		*/
	}
	else  if (ret <0)
		ret = -1;
	return ret;
}

/**
 * Writes to the open socket connection.
 * @param outBuf the buffer of the data to write
 * @param writeLen the number of bytes to write.
 *
 * @return the total number of bytes written
 */

int gcj_StubSocketWrite(int handle,char *outBuf,int writeLen)
{
	int ret=-1;
	_SOCKET_T *thisH=(_SOCKET_T *)handle;
	if (thisH==NULL || thisH->socketId<0 || socketHead.next==NULL)
		return -1;
	gcj_StubTakeMutex(&netMutex);
	_SOCKET_T *tmp = socketHead.next;
	while (tmp)
	{
		if (tmp == thisH)
		{
			break;
		}
		tmp = tmp->next;
	}
	gcj_StubReleaseMutex(&netMutex);	
	if (tmp == NULL)
		return -1;
	ret = gcjEventAPI_SocketWrite(thisH->socketId,outBuf,writeLen);
	gcj_TraceOut(0,"j2me: file %s,function %s,handle=%d,outBuf=%x,writeLen=%d,ret=%d\n",__FILE__,__FUNCTION__,thisH->socketId,outBuf,writeLen,ret);
	/*char *dump=OslMalloc(writeLen+1);
	for(int i=0;i<writeLen;i++)
		dump[i] = outBuf[i];
	dump[writeLen] = '\0';
	gcj_TraceOut(0,"dump:%s\n",dump);
	free(dump);*/
	return ret;
}

int gcj_StubSocketAvailable(int handle)
{
	int ret=-1;
	_SOCKET_T *thisH=(_SOCKET_T *)handle;
	if (thisH==NULL || thisH->socketId<0 || socketHead.next==NULL)
		return -1;
	gcj_StubTakeMutex(&netMutex);
	_SOCKET_T *tmp = socketHead.next;
	while (tmp)
	{
		if (tmp == thisH)
		{
			break;
		}
		tmp = tmp->next;
	}
	gcj_StubReleaseMutex(&netMutex);	
	if (tmp == NULL)
		return -1;
	ret = CFW_TcpipAvailableBuffer(thisH->socketId);
	gcj_TraceOut(0,"j2me: file %s,function %s,handle=%d,ret=%d\n",__FILE__,__FUNCTION__,thisH->socketId,ret);
	return ret;
}

int gcj_StubSocketClose(int handle)
{
	int ret = -1;
	_SOCKET_T *thisH=(_SOCKET_T *)handle;
	if (thisH==NULL || thisH->socketId<0 || socketHead.next==NULL)
		return -1;
	gcj_StubTakeMutex(&netMutex);
	_SOCKET_T *tmp = socketHead.next;
	while (tmp)
	{
		if (tmp == thisH)
		{
			break;
		}
		tmp = tmp->next;
	}
	gcj_StubReleaseMutex(&netMutex);	
	if (tmp == NULL)
		return -1;
	CFW_SetTCPIPCallBackEx (NULL,thisH->socketId);
	ret = gcjEventAPI_SocketClose(thisH->socketId);
	gcj_TraceOut(0,"j2me: file %s,function %s,thisH=0x%x,handle=%d,ret=%d\n",__FILE__,__FUNCTION__,thisH,thisH->socketId,ret);
	gcj_StubTakeMutex(&netMutex);
	tmp = socketHead.next;
	if (socketHead.next!=NULL && socketHead.next == thisH)
	{
		socketHead.next = thisH->next;
		free(thisH);
	}
	else
	{
		_SOCKET_T *tmp = socketHead.next;
		while (tmp)
		{
			if (tmp->next == thisH)
			{
				tmp->next = thisH->next;
				free(thisH);
				break;
			}
			else
				tmp = tmp->next;
		}
	}
	gcj_StubReleaseMutex(&netMutex);
	/*
	if (socketHead.next ==NULL)
	{
		gcjEventAPI_StopGprs();
		gcj_IsGprsActived = 0;
	}
	*/
	gcj_socke_count--;
	return ret;
}

void gcj_StubSocketGetHost(char **ipAddress,int handle, unsigned char isLocal)
{
	_SOCKET_T *thisH=(_SOCKET_T *)handle;
	if (thisH==NULL || thisH->socketId<0 || socketHead.next==NULL)
		return -1;
	gcj_TraceOut(0,"j2me: file %s,function %s,handle=%d,local=%d\n",__FILE__,__FUNCTION__,thisH->socketId,isLocal);
	*ipAddress = '\0';
}

int gcj_StubSocketGetPort(int handle, unsigned char isLocal)
{
	_SOCKET_T *thisH=(_SOCKET_T *)handle;
	if (thisH==NULL || thisH->socketId<0 || socketHead.next==NULL)
		return -1;
	gcj_TraceOut(0,"j2me: file %s,function %s,handle=%d,local=%d\n",__FILE__,__FUNCTION__,thisH->socketId,isLocal);
	return -1;
}

int gcj_StubSocketGetOpt(int handle, int option)
{
	int ret= -1;
	_SOCKET_T *thisH=(_SOCKET_T *)handle;
	if (thisH==NULL || thisH->socketId<0 || socketHead.next==NULL)
		return -1;
	gcj_StubTakeMutex(&netMutex);
	_SOCKET_T *tmp = socketHead.next;
	while (tmp)
	{
		if (tmp == thisH)
		{
			break;
		}
		tmp = tmp->next;
	}
	gcj_StubReleaseMutex(&netMutex);	
	if (tmp == NULL)
		return -1;
	int optval=-1,optlen=-1;
	ret = CFW_TcpipSocketGetsockopt(thisH->socketId,SOL_SOCKET,option,&optval,&optlen);
	gcj_TraceOut(0,"j2me: file %s,function %s,handle=%d,option=%d,ret=%d,optval=%d,optlen=%d\n",__FILE__,__FUNCTION__,thisH->socketId,option,ret,optval,optlen);
	return optval;
}

typedef enum {
    /**
     * Socket option for the small buffer <em>writing delay</em> (0).
     * Set to zero to disable Nagle algorithm for 
     * small buffer operations.
     * Set to a non-zero value to enable.
     */
    DELAY = 0,
 
    /**
     * Socket option for the <em>linger time</em> to wait in seconds
     * before closing a connection with
     * pending data output (1). Setting the linger time to zero
     * disables the linger wait interval.
     */
    LINGER = 1,

    /**
     * Socket option for the <em>keep alive</em> feature (2).
     * Setting KEEPALIVE to zero will disable the feature.
     * Setting KEEPALIVE to a non-zero value will enable the feature.
     */
    KEEPALIVE = 2,


    /**
     * Socket option for the size of the <em>receiving buffer</em> (3).
     */
    RCVBUF = 3,

    /**
     * Socket option for the size of the <em>sending buffer</em> (4).
     */
    SNDBUF = 4,
}SOCKET_OPTION_T;

void gcj_StubSocketSetOpt(int handle,int option,int value)
{
	int ret = -1;
	int level = SOL_SOCKET;
	switch (option)
	{
		case DELAY:
			level = CFW_TCPIP_IPPROTO_TCP;
			option = TCP_NODELAY;
			break;
		case LINGER:
			break;
		case KEEPALIVE:
			level = CFW_TCPIP_IPPROTO_TCP;
			option = TCP_KEEPALIVE;
			break;
		case RCVBUF:
			break;
		case SNDBUF:
			break;
		default:
			abort();
	}
	_SOCKET_T *thisH=(_SOCKET_T *)handle;
	if (thisH==NULL || thisH->socketId<0 || socketHead.next==NULL)
		return -1;
	gcj_StubTakeMutex(&netMutex);
	_SOCKET_T *tmp = socketHead.next;
	while (tmp)
	{
		if (tmp == thisH)
		{
			break;
		}
		tmp = tmp->next;
	}
	gcj_StubReleaseMutex(&netMutex);	
	if (tmp == NULL)
		return -1;
	ret = CFW_TcpipSocketSetsockopt(thisH->socketId,level,option,&value,sizeof(value));
	gcj_TraceOut(0,"j2me: file %s,function %s,handle=%d,option=%d,value=%d,ret=%d\n",__FILE__,__FUNCTION__,thisH->socketId,option,value,ret);
}
void gcj_StubSocketShutDownOutput(int handle)
{
	_SOCKET_T *thisH=(_SOCKET_T *)handle;

	gcj_TraceOut(0,"j2me: current task=%d,file %s,function %s,handle=%d\n",sxr_GetCurrentTaskId(),__FILE__,__FUNCTION__,thisH->socketId);
	
	if (gcj_PrepareNetworkError)
	{
		//DisplayPopup( (PU8)GetString(STR_GLOBAL_ERROR) , IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		gcj_PrepareNetworkError=0;
	}
}

