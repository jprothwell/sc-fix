/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__23____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// 联网模块 /////////////////////////////////////////////////////////////
/* event */
typedef enum
{
    SOC_READ    = 0x01,  /* Notify for read */
    SOC_WRITE   = 0x02,  /* Notify for write */
    SOC_ACCEPT  = 0x04,  /* Notify for accept */
    SOC_CONNECT = 0x08,  /* Notify for connect */
    SOC_CLOSE   = 0x10,   /* Notify for close */
    SOC_DNS     = 0x100,
    SOC_DNS_ERR = 0x200
} soc_event_enum;
extern BOOL wait_time ;
extern kal_uint8 custom_get_csd_profile_num();
extern U8 SetSimChannel(void);
extern void MMI_Sleep(u32 um);
#ifndef WIN32
extern int dbg_TraceOutputText(u16 nIndex, const char* fmt, ...);
#define kal_prompt_trace(trace_level, fmt, ...) \
			dbg_TraceOutputText(1, (fmt), ##__VA_ARGS__);
#endif
extern int mod_mmi;
extern int mod_trace;
#ifndef WIN32
typedef struct _COS_EVENT {
  u32 nEventId;
  u32 nParam1;
  u32 nParam2;
  u32 nParam3;
} COS_EVENT;
//typedef unsigned char               BOOL;
#endif
/* Protocol Familty */
#define PF_INET (0) /* currently only support Internet */
#define JMETOC_TIMER_2      J2ME_NOT_READY_TIMER + 1
#define JMEOTC_SOC_TIMEOUT  10000
typedef void (*PsFuncPtr) (void *);
extern int smarchbox_account_id;
extern u16 app_soc_notify_ind;
extern u16 app_soc_get_host_ind;
extern void jmetoc_notify_event(void);
extern s8 jmetocSocketConnection(s8 socketid, s32 port, void* host_addr);
extern JMETOC_BOOL vm_status;
extern int smarchbox_attrsp;
extern void jmetocResetNetwork();
extern U8 jmetoc_gprs_activing;
typedef BOOL( *PFN_DATA_CONN_CALLBACK )( COS_EVENT *pEV );
extern u32 CFW_SetDataConnFunEx ( PFN_DATA_CONN_CALLBACK pfnEv, u8 SimId,  u8 Cid );


/**
 * 接收系统消息。
 * @param msgid 需要接受的消息ID
 * @return 是否接收到了某个消息
 */
extern COS_EVENT* waitforSystemMessage(u32);

/**
 * 初始化网络账户。
 * <p>
 * 目前至少要保证 cmwap 和 cmnet 正常工作。
 * 应用会根据自身联网的模式来创建账户。
 * </p>
 * @param apnMode 账户模式，如 "cmwap"
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
extern void jmetocinitNetwork(const char* apnMode);

/**
 * 获得上网帐号。
 * <p>
 * 系统选择最后一个帐号并且将它按照指定配置设定。
 * 设置帐号信息通过方法 setNetworkAPN。
 * </p>
 * @return 上网帐号索引
 */
int wap_init_account(void) {
  if (SetSimChannel() == 1) {
    //return always_ask_encode_data_account_id(JMETOC_NWK_ACCOUNT_ID, 1, 0, 0);
    jmetoc_trace("acc sim1");
    return smarchbox_account_id;
  } else {
    //return always_ask_encode_data_account_id(JMETOC_NWK_ACCOUNT_ID, 2, 0, 0);
    jmetoc_trace("acc sim2");
    return smarchbox_account_id | 0x10000;
  }
}

typedef struct _in_addr
{
	u32 s_addr;
}in_addr;
typedef struct _CFW_TCPIP_SOCKET_ADDR
{
	u8 sin_len;
	u8 sin_family;
	u16 sin_port;
       in_addr sin_addr;
	s8 sin_zero[8];
}CFW_TCPIP_SOCKET_ADDR;
#ifdef __SUPPORT_SLAVESIM__
typedef  unsigned char CFW_SIM_ID;
extern u32 CFW_ReleaseCID(u8 nCID, CFW_SIM_ID nSimID);
#else
extern u32 CFW_ReleaseCID(u8 nCID);
#endif
extern u32 CFW_TcpipSocket(u8 nDomain, u8 nType, u8 nProtocol);
extern u32 CFW_TcpipSocketClose(u8 nSocket);
#define CFW_TCPIP_AF_INET         2
#define CFW_TCPIP_SOCK_STREAM     1

extern u8 jmetoc_SIM;


#define EV_CFW_IND_BASE              (5000)
//
// CFW IND Event.
//
#define EV_CFW_INIT_IND                          (EV_CFW_IND_BASE+1)
#define EV_CFW_SRV_STATUS_IND                    (EV_CFW_IND_BASE+2) 
#define EV_CFW_NW_SIGNAL_QUALITY_IND             (EV_CFW_IND_BASE+3)
#define EV_CFW_NW_REG_STATUS_IND                 (EV_CFW_IND_BASE+5)
#define EV_CFW_CC_SPEECH_CALL_IND                (EV_CFW_IND_BASE+6)
#define EV_CFW_CC_CRSSINFO_IND                   (EV_CFW_IND_BASE+7)
#define EV_CFW_NEW_SMS_IND                       (EV_CFW_IND_BASE+8)
#define EV_CFW_SMS_INFO_IND                      (EV_CFW_IND_BASE+9)
#define EV_CFW_CC_CALL_INFO_IND                  (EV_CFW_IND_BASE+10)
#define EV_CFW_SIM_STATUS_IND                    (EV_CFW_IND_BASE+11)
#define EV_CFW_SS_USSD_IND                       (EV_CFW_IND_BASE+12)
#define EV_CFW_CC_RELEASE_CALL_IND               (EV_CFW_IND_BASE+13)
#define EV_CFW_CC_ERROR_IND                      (EV_CFW_IND_BASE+14)
#define EV_CFW_SAT_CMDTYPE_IND                   (EV_CFW_IND_BASE+15)
#define EV_CFW_PBK_IND                           (EV_CFW_IND_BASE+16)
#define EV_CFW_NW_NETWORKINFO_IND                (EV_CFW_IND_BASE+17)
#define EV_CFW_GPRS_CXT_ACTIVE_IND               (EV_CFW_IND_BASE+18)
#define EV_CFW_GPRS_CXT_DEACTIVE_IND             (EV_CFW_IND_BASE+19)
#define EV_CFW_GPRS_MOD_IND                      (EV_CFW_IND_BASE+20) 
#define EV_CFW_GPRS_STATUS_IND                   (EV_CFW_IND_BASE+21)
#define EV_CFW_GPRS_DATA_IND                     (EV_CFW_IND_BASE+22)
#define EV_CFW_TCPIP_REV_DATA_IND                     (EV_CFW_IND_BASE+23)  // TCP/IP IND
#define EV_CFW_TCPIP_CLOSE_IND                     (EV_CFW_IND_BASE+24)  // TCP/IP close IND
#define EV_CFW_TCPIP_ERR_IND                     (EV_CFW_IND_BASE+25)  // TCP/IP ERROR IND
#define EV_CFW_TCPIP_ACCEPT_IND                     (EV_CFW_IND_BASE+26)  // TCP/IP accept IND
#define EV_CFW_DNS_RESOLV_SUC_IND                (EV_CFW_IND_BASE+27)
#define EV_CFW_DNS_RESOLV_ERR_IND                (EV_CFW_IND_BASE+28)
#define EV_CFW_PPP_DATA_IND                      (EV_CFW_IND_BASE+29)
#define EV_CFW_PPP_TERMINATE_IND                 (EV_CFW_IND_BASE+30)
//Frank add used MMS/PUSH
#define EV_CFW_NEW_EMS_IND                       (EV_CFW_IND_BASE+31)
#define EV_CFW_EXIT_IND                          (EV_CFW_IND_BASE+32)
#define EV_CFW_XX_IND_END                        (EV_CFW_IND_BASE+33)

#define EV_CFW_GPRS_RSP_BASE         (850)
#define EV_CFW_TCPIPUDP_RSP_BASE     (1000)
//
// GPRS Response Event.
//
#define EV_CFW_GPRS_CTX_MODIFY_ACC_RSP       (EV_CFW_GPRS_RSP_BASE+1)
#define EV_CFW_GPRS_ATT_RSP                  (EV_CFW_GPRS_RSP_BASE+2)
#define EV_CFW_GPRS_ACT_RSP                  (EV_CFW_GPRS_RSP_BASE+3)
#define EV_CFW_GPRS_CXT_ACTIVE_RSP           (EV_CFW_GPRS_RSP_BASE+4)
#define EV_CFW_GPRS_MOD_RSP                  (EV_CFW_GPRS_RSP_BASE+5)
//
// GPRS
//
#define CFW_GPRS_DETACHED       0   //detached 
#define CFW_GPRS_ATTACHED       1   //attached 
//
//TCP(UDP) / IP Event
//
#define EV_CFW_TCPIP_SOCKET_CONNECT_RSP           (EV_CFW_TCPIPUDP_RSP_BASE+1)
#define EV_CFW_TCPIP_SOCKET_CLOSE_RSP             (EV_CFW_TCPIPUDP_RSP_BASE+2)
#define EV_CFW_TCPIP_SOCKET_SEND_RSP              (EV_CFW_TCPIPUDP_RSP_BASE+3)

typedef BOOL ( *PFN_TCPIP_CALLBACK )( COS_EVENT *pEV );
extern u32 CFW_SetTCPIPCallBack ( PFN_TCPIP_CALLBACK pfnEv ) ;
#define ERR_SUCCESS                                       0x0

/**
 * 关闭 GPRS 连接。
 * <p>
 * 在 Coolsand 中，GPRS 的开启并非通过底层自动控制。
 * 而是需要手动开启。
 * 这里通过方法 CFW_ReleaseCID 来释放可用 GPRS ID。
 * </p>
 * @param cid 可用 GPRS 连接 ID
 * @author runasea
 * @version 1.0.0.1, 2009-12-01
 */
void close_gprs(u8 cid) {
  if (cid > 0) {
    #ifdef __SUPPORT_SLAVESIM__
  //  CFW_ReleaseCID(cid, jmetoc_SIM);
    #else
  //  CFW_ReleaseCID(cid);
    #endif
  }
  kal_prompt_trace(1, "gp cls %d", cid);
}

void jmetoc_closesocket(u8 sid) {
  CFW_TcpipSocketClose((u8)sid);
  kal_prompt_trace(1, "soc cls %d", sid);
  jmetoc_soc_id = -1;
}

/**
 * 通知关闭 GPRS。
 * @return 状态
 * @author runasea
 * @version 1.0.0.1, 2009-12-01
 */
void gprs_notifyGPRSClose() {
    kal_prompt_trace(1, "gprs_notifyGPRSClose ,socket_callback: 0x%x, status:0x%x", socket_callback,jmetoc_channel_updateing_status);

  if (jmetoc_channel_updateing_status > 0) {
//    close_gprs(jmetoc_channel_updateing_status);
    if (socket_callback != null) {
      socket_callback(SOC_CLOSE);
      socket_callback = null;
    }
  }
  //jmetocResetNetwork();
  jmetoc_channel_updateing_status = -1;
}

extern u8 jmetoc_cid;
/**
 * 获得 GPRS 连接 ID。
 * <p>
 * 在 Coolsand 中，GPRS 的开启并非通过底层自动控制。
 * 而是需要手动开启。
 * 这里通过方法 CFW_GetFreeCID 来获得可用 GPRS ID。
 * </p>
 * @return 可用 GPRS 连接 ID
 * @author runasea
 * @version 1.0.0.1, 2009-12-01
 */
u8 get_gprs() {
  s8 socket_id;
  //if (jmetoc_channel_updateing_status >= 0) {   //jmetoc_channel_updateing_status 在 coolsand 中用作域名处理的 gprs id。
  //  close_gprs(jmetoc_channel_updateing_status);
  //  jmetoc_channel_updateing_status = -1;
  //}
  //if (jmetoc_channel_updateing_status >= 0) {
  //  return jmetoc_channel_updateing_status;
  //}
  gprs_notifyGPRSClose();
  if (jmetoc_SIM == 0xff) 
  	socket_id = -1;
  else{

      if((jmetoc_cid <=0)||(jmetoc_cid>7))
  	{
  #ifdef __SUPPORT_SLAVESIM__
   CFW_GetFreeCID(&socket_id, jmetoc_SIM);
  #else
   CFW_GetFreeCID(&socket_id);
  #endif
  	}else
  	{
  	    kal_prompt_trace(1, "ready, gp %d", jmetoc_cid);
  	    return jmetoc_cid;
  	}
  }
  jmetoc_cid = socket_id;
  kal_prompt_trace(1, "gp %d", socket_id);
  return socket_id;
}

typedef struct _CFW_GPRS_QOS {
    u8 nPrecedence;
    u8 nDelay;
    u8 nReliability;
    u8 nPeak;
    u8 nMean; 
    u8 padding[3];
} CFW_GPRS_QOS;
extern s32 gprs_getIP();
extern U8 smarchbox_uti;
BOOL gprs_conCallBack(COS_EVENT* event);

/**
 * 激活 GPRS。
 * <p>
 * 在 Coolsand 中，GPRS 的开启并非通过底层自动控制。
 * 而是需要手动开启。
 * 这里通过方法 CFW_GprsAct 来激活 GPRS。
 * </p>
 * <p>
 * 如果在已经激活的状态下，本方法会直接获得域名解析。
 * </p>
 * @return 状态
 * @author runasea
 * @version 1.0.0.1, 2009-12-01
 */
s32 gprs_tcpAct() {
  CFW_GPRS_QOS qos;
  u32 rVal;
  u8 status;
  qos.nDelay = 4;
  qos.nMean = 16;
  qos.nPeak = 4;
  qos.nPrecedence = 3;
  qos.nReliability = 3;
  if (jmetoc_channel_updateing_status < 0) {
    jmetoc_channel_updateing_status = get_gprs();
  }
  #ifdef __SUPPORT_SLAVESIM__
	  //rVal = CFW_GetFreeCID(&nCid, jmetoc_SIM);
	  rVal = CFW_GetGprsActState(jmetoc_channel_updateing_status, &status, jmetoc_SIM);
  #else
	  //rVal = CFW_GetFreeCID(&nCid);
	  rVal = CFW_GetGprsActState(jmetoc_channel_updateing_status,&status);
  #endif
  kal_prompt_trace(1, "get act %d, %d", jmetoc_channel_updateing_status, status);
  if (status != 1) {
    if (jmetoc_gprs_activing) {
      return -1;
    }
  	smarchbox_uti = ADP_GetFreeUti();
    #ifdef __SUPPORT_SLAVESIM__
    rVal = CFW_GprsSetReqQos(jmetoc_channel_updateing_status, &qos, jmetoc_SIM);
    jmetocInitApn((u8)jmetoc_channel_updateing_status);
    rVal = CFW_GprsAct(1, jmetoc_channel_updateing_status, smarchbox_uti, jmetoc_SIM);
	if(ERR_SUCCESS ==rVal)
        CFW_SetDataConnFunEx(gprs_conCallBack,jmetoc_SIM,jmetoc_channel_updateing_status);//add by wuys for background QQ, 2010-05-31

    kal_prompt_trace(1, "do act0 %d", rVal);
    #else
    rVal = CFW_GprsSetReqQos(jmetoc_channel_updateing_status, &qos);
    jmetocInitApn((u8)jmetoc_channel_updateing_status);
    rVal = CFW_GprsAct(1, jmetoc_channel_updateing_status, smarchbox_uti);
	if(ERR_SUCCESS ==rVal)
        CFW_SetDataConnFunEx(gprs_conCallBack,jmetoc_channel_updateing_status);//add by wuys for background QQ, 2010-05-31
    #endif
    if (rVal != 0) {
      return -1;
    } else {
      jmetoc_gprs_activing = true;
    }
    return -2;  //激活GPRS进入堵塞状态，等待消息
  } else {
    return gprs_getIP();
  }
}

struct ip_addr
{
	u32 addr;
};
typedef enum e_resolv_result {
  RESOLV_QUERY_INVALID,
  RESOLV_QUERY_QUEUED,
  RESOLV_COMPLETE
} RESOLV_RESULT;
BOOL gprs_jmetocCallBack(COS_EVENT* event);
BOOL gprs_jmetocCallBack2(COS_EVENT* event) ;


/**
 * 获得 ip 地址。
 * @return 状态
 * @author runasea
 * @version 1.0.0.1, 2009-12-01
 */
s32 gprs_getIP(){
  struct ip_addr stIPAddress ;
  u32 ret;
      kal_prompt_trace(1, "gprs_getIP ,socket_callback: 0x%x, status:0x%x", socket_callback,jmetoc_channel_updateing_status);

  #if __SUPPORT_SLAVESIM__
  
  ret = CFW_Gethostbyname(jmetoc_soc_host, &stIPAddress, jmetoc_channel_updateing_status, jmetoc_SIM);
  #else
  ret = CFW_Gethostbyname(jmetoc_soc_host, &stIPAddress,jmetoc_channel_updateing_status);
  #endif
  if(RESOLV_COMPLETE == ret) {
    jmetoc_host_addr = stIPAddress.addr;
    kal_prompt_trace(1, "host %x", jmetoc_host_addr);
    //gprs_notifyGPRSClose();
    if (socket_callback != null) {
      socket_callback(SOC_DNS);
    }
    return 0;
  } else if(RESOLV_QUERY_QUEUED == ret) {
      kal_prompt_trace(1, "gprs_getIP socket_callback :0x%x", socket_callback);

  if (socket_callback != null) 
  	{
      CFW_SetTCPIPCallBackEx(gprs_jmetocCallBack,4);//add by wuys for background QQ, 2010-05-31
  	}
  else{
  	CFW_SetTCPIPCallBackEx(gprs_jmetocCallBack2,4);//add by wuys for background QQ, 2010-05-31
  	}

    kal_prompt_trace(1, "dns wait");
    return -2;
  } else {
    kal_prompt_trace(1, "bad host");
    //gprs_notifyGPRSClose();
    return -1;
  }
}

typedef enum
{
   L4C_GPRS_ATTACHED, //0
   L4C_NONE_GPRS, // 1
   L4C_GPRS_DETACHED, // 2
   L4C_INVALID_SIM,// 3
   L4C_GPRS_ATTEMPT_ATTACH,// 4
   L4C_GPRS_COVERAGE,//5 
   L4C_PDP_ATTEMPT_ACT,//6
   L4C_PDP_DEACTIVED,//7
   L4C_PDP_ACTIVED//8
} l4c_gprs_status_enum;
extern void Send_GPRS_Status_Ind(l4c_gprs_status_enum status);
extern int jmetoc_event_break;

void jmetoc_socket_bind() {
	  kal_prompt_trace(1, "jmetoc_socket_bind entry");

	u8 uiLen =0 ;
	u8 acIPAddr[18]= {0,};
	CFW_TCPIP_SOCKET_ADDR stLocalAddr = {0, } ;
if (jmetoc_soc_id >= 0) {
  #if __SUPPORT_SLAVESIM__
		CFW_GprsGetPdpAddr(jmetoc_cid, &uiLen, acIPAddr,jmetoc_SIM);
  #else
		CFW_GprsGetPdpAddr(jmetoc_cid, &uiLen, acIPAddr);
  #endif
    stLocalAddr.sin_len = 0;	
	  stLocalAddr.sin_family = CFW_TCPIP_AF_INET;   
	  stLocalAddr.sin_port = 0 ;
	  stLocalAddr.sin_addr.s_addr = jdd_NetHtonl(acIPAddr[0]<<24 | acIPAddr[1]<<16 | acIPAddr[2]<<8 | acIPAddr[3]);

	  CFW_TcpipSocketBind(jmetoc_soc_id, &stLocalAddr, sizeof(CFW_TCPIP_SOCKET_ADDR));
  }
}

/**
 * GPRS 消息回调。
 */
BOOL gprs_conCallBack(COS_EVENT* event) {

	  kal_prompt_trace(1, "gprs_conCallBack entry");

  u8 type = (u8)(event->nParam3 >> 8);
  jmetoc_event_break = 1;
  switch (event->nEventId) {
  case EV_CFW_GPRS_ATT_RSP:
    if (type == 0) {  //detach success
      Send_GPRS_Status_Ind(L4C_GPRS_DETACHED);
    } else if (type == 1) {  //attach success
      Send_GPRS_Status_Ind(L4C_GPRS_ATTACHED);
      smarchbox_attrsp = 1;
	wait_time = FALSE;
      gprs_tcpAct();
    } else {
      jmetoc_soc_ret = -1; //消息等待返回
      gprs_notifyGPRSClose();
      kal_prompt_trace(1, "att rsp %d", type);
    }
    break;
  case EV_CFW_GPRS_ACT_RSP:
    if (type == 0) {
      Send_GPRS_Status_Ind(L4C_PDP_DEACTIVED);
    } else if (type == 1) {
      jmetoc_socket_bind();
      Send_GPRS_Status_Ind(L4C_PDP_ACTIVED);
      jmetoc_soc_ret = gprs_getIP();
    }
    jmetoc_gprs_activing = false;
    break;
  case EV_CFW_GPRS_STATUS_IND:
  default:
    kal_prompt_trace(1, "con ev %d", event->nEventId);
    break;
  }
  return true;
}

/**
 * 联网消息回调。
 */
BOOL gprs_jmetocCallBack(COS_EVENT* event) {
    kal_prompt_trace(1, "gprs_jmetocCallBack ,socket_callback: 0x%x", socket_callback);

  jmetoc_event_break = 1;
  
  if(socket_callback==NULL)
        return true;

  switch (event->nEventId) {
  case EV_CFW_DNS_RESOLV_SUC_IND:
    jmetoc_host_addr = event->nParam1;
    kal_prompt_trace(1, "dns ip %x", jmetoc_host_addr);
    //gprs_notifyGPRSClose();
    socket_callback(SOC_DNS);
    break;
  case EV_CFW_DNS_RESOLV_ERR_IND:
    kal_prompt_trace(1, "sms dns fail %d", CFW_TcpipGetLastError());
    //gprs_notifyGPRSClose();
    socket_callback(SOC_DNS_ERR);
    break;
  case EV_CFW_TCPIP_SOCKET_CONNECT_RSP:
    jmetoc_trace("soc con suc");
    socket_callback(SOC_CONNECT);
    break;
  case EV_CFW_TCPIP_SOCKET_SEND_RSP:
    jmetoc_trace("soc snd suc");
    socket_callback(SOC_WRITE);
    break;
  case EV_CFW_TCPIP_REV_DATA_IND:
    jmetoc_trace("soc rsv suc");
    socket_callback(SOC_READ);
    break;
  case EV_CFW_TCPIP_CLOSE_IND:
  case EV_CFW_TCPIP_SOCKET_CLOSE_RSP:
    //gprs_notifyGPRSClose();
    kal_prompt_trace(1, "soc close");
    //socket_callback(SOC_CLOSE);
    break;
  case EV_CFW_TCPIP_ERR_IND:
    kal_prompt_trace(1, "tcp err");
    gprs_notifyGPRSClose();
    //socket_callback(SOC_CLOSE);
    break;
  default:
    kal_prompt_trace(1, "sk ev %d", event->nEventId);
    break;
  }
  return true;
}

/**
 * DNS解析。
 * @param host 解析的域名名称
 * @param callback 消息回调
 * @return -1表示失败，-2表示进入消息等待
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
s32 gprs_getHostByName(char* host, void* callback) {
    kal_prompt_trace(1, "gprs_getHostByName ,callback: 0x%x", callback);

  u8 status;
  u32 val = 0;
  //modify by wuys for  background QQ, 2010-05-31
 #if 0
  if(ERR_SUCCESS != CFW_SetDataConnFun(gprs_conCallBack)) {
    return -1;
  }
  if (ERR_SUCCESS != CFW_SetTCPIPCallBack(gprs_jmetocCallBack)) {
    return -1;
  }
  #else
  CFW_SetTCPIPCallBackEx(gprs_jmetocCallBack,jmetoc_soc_id);
  #endif
  
  socket_callback = callback;
  jmetoc_soc_host = host;
  smarchbox_uti = ADP_GetFreeUti();
  #if __SUPPORT_SLAVESIM__
    CFW_GetGprsAttState(&status, jmetoc_SIM);
  smarchbox_attrsp = status;
    kal_prompt_trace(1, "gprs status %d", smarchbox_attrsp);
    if (smarchbox_attrsp != 1) {
      val = CFW_GprsAtt( CFW_GPRS_ATTACHED, smarchbox_uti, jmetoc_SIM);
	 wait_time = TRUE;
	kal_prompt_trace(1, "CFW_GprsAtt return: 0x%x", val);

	if(0 ==val)
	{
	       CFW_SetDataConnFunEx(gprs_conCallBack,jmetoc_SIM,0);//add by wuys for background QQ, 2010-05-31
             smarchbox_attrsp = 1;
	}
	  return -2;
    } else {
      gprs_tcpAct();
      return -2;
    }
  #else
    //CFW_GetGprsAttState(&status);
    if (smarchbox_attrsp != 1) {
      CFW_GprsAtt( CFW_GPRS_ATTACHED, smarchbox_uti);
	CFW_SetDataConnFunEx(gprs_conCallBack,0);//add by wuys for background QQ, 2010-05-31

      return -2;
    } else {
      gprs_tcpAct();
      return -2;
    }
  #endif
}

/**
 * 获得socket连接。
 * @param nwk_account_id 网关ID
 * @return 成功返回 socket ID。如果失败，返回-1。
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI s8 get_socket() {
	  kal_prompt_trace(1, "get_socket entry");

  s8 socket_id;
  if (jmetoc_SIM == 0xff) {
    return -1;
  }
  
  if (jmetoc_channel_updateing_sid >= 0) {
    CFW_TcpipSocketClose((u8)jmetoc_channel_updateing_sid);
    jmetoc_channel_updateing_sid = -1;
  }
  if (jmetoc_soc_id >= 0) {
    CFW_TcpipSocketClose(jmetoc_soc_id);
  }
    jmetoc_soc_host = NULL;

  jmetoc_soc_id = socket_id = CFW_TcpipSocket(CFW_TCPIP_AF_INET, CFW_TCPIP_SOCK_STREAM, 0);
  kal_prompt_trace(1, "soc %d", socket_id);
  return socket_id;
}

/**
 * 关闭socket连接。
 * @param socketid socket ID
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void close_socket(s8 socketid) {
	  kal_prompt_trace(1, "close_socket entry");

  jmetoc_closesocket(socketid);
  gprs_notifyGPRSClose();
}

/**
 * 临时计数器逻辑。
 * <p>
 * 本计数器负责对网络连接堵塞和超时的逻辑。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
void jmetoc_tmp_timer() {
//	  kal_prompt_trace(1, "close_socket entry");

  if (!vm_status) { return; }
  soc_retry_times++;
  jmetoc_notify_event();
  if (soc_retry_times >= 10000) {
    jmetoc_soc_ret = -1; //消息等待返回
    return;
  }
  StartTimer(JMETOC_TIMER_2, 5, (void (*)(void))jmetoc_tmp_timer);
}

extern JMETOC_BOOL smarchbox_robot_booted;
/**
 * 启动临时计数器。
 * <p>
 * 本计数器负责对网络连接堵塞和超时的逻辑。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
void jmetoc_tmp_timer_start() {
  soc_retry_times = 0;
  if (!smarchbox_robot_booted) {
    StartTimer(JMETOC_TIMER_2, 5, (void (*)(void))jmetoc_tmp_timer);
  }
}

#define LOCAL_PARA_HDR \
   kal_uint8	ref_count; \
   kal_uint16	msg_len;

	typedef struct
	{
		LOCAL_PARA_HDR
		kal_uint8	status; /* l4c_gprs_status_enum */
	} mmi_ps_gprs_status_update_ind_struct;

/**
 * 联网消息回调。
 */
//BOOL gprs_conCallBack2(COS_EVENT* event) {
//  u8 type = (u8)(event->nParam3 >> 8);
//  switch (event->nEventId) {
//  case EV_CFW_GPRS_ATT_RSP:
//    if (type == 1) {
//      gprs_tcpAct();
//    } else {
//      kal_prompt_trace(1, "att rsp %d", type);
//    }
//    break;
//  case EV_CFW_GPRS_ACT_RSP:
//    gprs_getIP();
//    break;
//  default:
//    kal_prompt_trace(1, "con ev %d", event->nEventId);
//    break;
//  }
//  return true;
//}
BOOL gprs_conCallBack3(COS_EVENT* event);
/**
 * 激活 GPRS。
 * <p>
 * 在 Coolsand 中，GPRS 的开启并非通过底层自动控制。
 * 而是需要手动开启。
 * 这里通过方法 CFW_GprsAct 来激活 GPRS。
 * </p>
 * @return 状态
 * @author runasea
 * @version 1.0.0.1, 2009-12-01
 */
void gprs_tcpAct2() {
  kal_prompt_trace(1, "gprs_tcpAct2 entry");

  CFW_GPRS_QOS qos;
  u32 rVal;
  u8 status;
  qos.nDelay = 4;
  qos.nMean = 16;
  qos.nPeak = 4;
  qos.nPrecedence = 3;
  qos.nReliability = 3;
  if (jmetoc_channel_updateing_status < 0) {
    jmetoc_channel_updateing_status = get_gprs();
  }
  #ifdef __SUPPORT_SLAVESIM__
	  //rVal = CFW_GetFreeCID(&nCid, jmetoc_SIM);
	  rVal = CFW_GetGprsActState(jmetoc_channel_updateing_status, &status, jmetoc_SIM);
  #else
	  //rVal = CFW_GetFreeCID(&nCid);
	  rVal = CFW_GetGprsActState(jmetoc_channel_updateing_status,&status);
  #endif
  kal_prompt_trace(1, "get act2:  %d, %d", jmetoc_channel_updateing_status, status);
  if (status != 1) {
    if (jmetoc_gprs_activing) {
      jmetoc_soc_ret = -1;
      return;
    }
  	smarchbox_uti = ADP_GetFreeUti();
    #ifdef __SUPPORT_SLAVESIM__
    rVal = CFW_GprsSetReqQos(jmetoc_channel_updateing_status, &qos, jmetoc_SIM);
    jmetocInitApn((u8)jmetoc_channel_updateing_status);
    rVal = CFW_GprsAct(1, jmetoc_channel_updateing_status, smarchbox_uti, jmetoc_SIM);
	if(ERR_SUCCESS==rVal)
				  CFW_SetDataConnFunEx(gprs_conCallBack3,jmetoc_SIM,jmetoc_channel_updateing_status);//add by wuys for background QQ, 2010-05-31

    kal_prompt_trace(1, "do act2 %d", rVal);
    #else
    rVal = CFW_GprsSetReqQos(jmetoc_channel_updateing_status, &qos);
    jmetocInitApn((u8)jmetoc_channel_updateing_status);
    rVal = CFW_GprsAct(1, jmetoc_channel_updateing_status, smarchbox_uti);
    if(ERR_SUCCESS==rVal)
		CFW_SetDataConnFunEx(gprs_conCallBack3,jmetoc_channel_updateing_status);//add by wuys for background QQ, 2010-05-31

    #endif
    if (rVal != 0) {
      jmetoc_soc_ret = -1;
    } else {
      jmetoc_gprs_activing = true;
    }
  } else {
    //gprs_notifyGPRSClose();
    jmetoc_soc_ret = 0;
  }
}

/**
 * GPRS 消息回调。
 */
BOOL gprs_conCallBack3(COS_EVENT* event) {
	  kal_prompt_trace(1, "gprs_conCallBack3 entry");

  u8 type = (u8)(event->nParam3 >> 8);
  jmetoc_event_break = 1;
  switch (event->nEventId) {
  case EV_CFW_GPRS_ATT_RSP:
    if (type == 0) {
      Send_GPRS_Status_Ind(L4C_GPRS_DETACHED);
    } else if (type == 1) {
      Send_GPRS_Status_Ind(L4C_GPRS_ATTACHED);
      smarchbox_attrsp = 1;
      gprs_tcpAct2();
    } else {
      jmetoc_soc_ret = -1; //消息等待返回
      gprs_notifyGPRSClose();
      kal_prompt_trace(1, "att rsp %d", type);
    }
    break;
  case EV_CFW_GPRS_ACT_RSP:
  	jmetoc_socket_bind();
    jmetoc_soc_ret = 0; //消息等待返回
    jmetoc_gprs_activing = false;
    break;
  case EV_CFW_GPRS_STATUS_IND:
    break;
  default:
    jmetoc_soc_ret = -1; //消息等待返回
    kal_prompt_trace(1, "con ev %d", event->nEventId);
    break;
  }
  return true;
}

extern u32 jmetoc_soc_rsv_buffer_size;
BOOL COS_Sleep (
  u32 nMillisecondes
);
/**
 * 联网消息回调。
 */
BOOL gprs_jmetocCallBack2(COS_EVENT* event) {
	  kal_prompt_trace(1, "gprs_jmetocCallBack2 entry");

  jmetoc_event_break = 1;
  switch (event->nEventId) {
  case EV_CFW_DNS_RESOLV_SUC_IND:
  	/*
  	if(  jmetoc_soc_host == NULL)
  	{
		kal_prompt_trace(1, "no wait dns ,host :NULL");
		return true;
  	}
	if(!strcmp(jmetoc_soc_host, event->nParam2))
	{
		kal_prompt_trace(1, "wait dns ,host other");
		return true;
	}*/
    jmetoc_host_addr = event->nParam1;
    kal_prompt_trace(1, "dns ip %x", jmetoc_host_addr);
    //gprs_notifyGPRSClose();
    jmetoc_soc_ret = 0; //消息等待返回
    break;
  case EV_CFW_DNS_RESOLV_ERR_IND:
    jmetoc_trace("sms dns fail");
    //gprs_notifyGPRSClose();
    jmetoc_soc_ret = -1; //消息等待返回
    break;
  case EV_CFW_TCPIP_SOCKET_CONNECT_RSP:
    jmetoc_trace("soc con suc");
    jmetoc_soc_ret = 0; //消息等待返回
    break;
  case EV_CFW_TCPIP_SOCKET_SEND_RSP:
    jmetoc_trace("soc snd suc");
    jmetoc_soc_ret = 0; //消息等待返回
    break;
  case EV_CFW_TCPIP_REV_DATA_IND:
    jmetoc_soc_ret = 0; //消息等待返回
    jmetoc_soc_rsv_buffer_size += event->nParam2;
    kal_prompt_trace(1, "soc rsv %d", jmetoc_soc_rsv_buffer_size);
    break;
  case EV_CFW_TCPIP_CLOSE_IND:
  case EV_CFW_TCPIP_SOCKET_CLOSE_RSP:
    jmetoc_soc_ret = 0; //消息等待返回
    break;
  case EV_CFW_TCPIP_ERR_IND:
    kal_prompt_trace(1, "tcp err %d", event->nParam2);
    jmetoc_soc_ret = -1; //消息等待返回
    gprs_notifyGPRSClose();
    break;
  default:
    jmetoc_soc_ret = -1; //消息等待返回
    kal_prompt_trace(1, "sk ev %d", event->nEventId);
    break;
  }
  return true;
}

extern void jmetoc_dispatchEvents();
/**
 * 获得域名空间对应的 ip 地址。
 * @param socketid socket ID
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
void jmetoc_soc_gethostbyname() {
	  kal_prompt_trace(1, "jmetoc_soc_gethostbyname entry");

  COS_EVENT* ev;
  u8 status;
  u8 type;
  u32 val = 0;
  mmi_ps_gprs_status_update_ind_struct *pGPRSStatusInd;
  jmetoc_host_addr = 0;
    //modify by wuys for  background QQ, 2010-05-31
#if 0
  if(ERR_SUCCESS != CFW_SetDataConnFun(gprs_conCallBack)) {
    return -1;
  }
  if (ERR_SUCCESS != CFW_SetTCPIPCallBack(gprs_jmetocCallBack2)) {
    return -1;
  }
#else
  CFW_SetTCPIPCallBackEx(gprs_jmetocCallBack2,jmetoc_soc_id);

#endif
  jmetoc_soc_ret = -2;
  #if __SUPPORT_SLAVESIM__
    CFW_GetGprsAttState(&status, jmetoc_SIM);
    smarchbox_attrsp = status;
    kal_prompt_trace(1, "gprs status %d", smarchbox_attrsp);
    if (smarchbox_attrsp != 1) {  //GPRS 还没有激活
      	  smarchbox_uti = ADP_GetFreeUti();
      
         val = CFW_GprsAtt( CFW_GPRS_ATTACHED, smarchbox_uti, jmetoc_SIM);
		 	kal_prompt_trace(1, "CFW_GprsAtt return: 0x%x", val);

	   if(0 == val){
	       CFW_SetDataConnFunEx(gprs_conCallBack,jmetoc_SIM,0);//add by wuys for background QQ, 2010-05-31
		smarchbox_attrsp = 1;
	   }else
	   	jmetoc_soc_ret = -1;
    } else {
      jmetoc_soc_ret = gprs_tcpAct(); //激活了GPRS就可以直接操作了
    }
  #else
   CFW_GetGprsAttState(&status);
    if (smarchbox_attrsp != 1) {
      CFW_GprsAtt( CFW_GPRS_ATTACHED, smarchbox_uti);
	  	  CFW_SetDataConnFunEx(gprs_conCallBack,jmetoc_SIM,0);//add by wuys for background QQ, 2010-05-31

    } else {
      jmetoc_soc_ret = gprs_tcpAct();
    }
  #endif
  //kal_prompt_trace(mod_trace, "ip ret %d", jmetoc_soc_ret);
  jmetoc_tmp_timer_start();
  while (jmetoc_soc_ret == -2) {  //等待消息，coolsand 中，系统消息和 MMI 应用消息分别在两个 task 中。系统消息在 gprs_conCallBack2 和 gprs_jmetocCallBack2 回调中处理。
    jmetoc_dispatchEvents();
    if (!vm_status) {
      jmetoc_soc_ret = -1;
      jmetoc_trace("force exit1");  //如果虚拟机强制退出，就不再等待消息了
      return;
    }
  }
  if (!smarchbox_robot_booted) {
    StopTimer(JMETOC_TIMER_2);
  }
  kal_prompt_trace(mod_trace, "jc dns ok");
}

extern u16 jmetoc_htons(u16 p);
s32 cs_connect_byip(u16 port, u8 sid) {
		  kal_prompt_trace(1, "cs_connect_byip entry");

  //JDD_LOG: SocConnect: IP=ac00000a Port=20480
  s32 ret;
  CFW_TCPIP_SOCKET_ADDR dest_addr;
  dest_addr.sin_len = 0 ;
  dest_addr.sin_family = CFW_TCPIP_AF_INET;
  dest_addr.sin_port = jmetoc_htons(port);
  dest_addr.sin_addr.s_addr = jmetoc_host_addr;
  ret = CFW_TcpipSocketConnect(sid, &dest_addr, sizeof(CFW_TCPIP_SOCKET_ADDR));
  return ret;
}

/**
 * socket连接到指定服务器操作。
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
void jmetoc_connect_byip() {
		  kal_prompt_trace(1, "jmetoc_connect_byip entry");

  COS_EVENT* ev;
  u8 status;
  u8 type;
    u32 val = 0;

      //modify by wuys for  background QQ, 2010-05-31
#if 0

  if (jmetoc_soc_id != -1) {
    if(ERR_SUCCESS != CFW_SetDataConnFun(gprs_conCallBack3)) {
      return -1;
    }
    if (ERR_SUCCESS != CFW_SetTCPIPCallBack(gprs_jmetocCallBack2)) {
      return -1;
    }
#else
  CFW_SetTCPIPCallBackEx(gprs_jmetocCallBack2,jmetoc_soc_id);

#endif
    jmetoc_soc_ret = -2;
    #if __SUPPORT_SLAVESIM__
      CFW_GetGprsAttState(&status, jmetoc_SIM);
	smarchbox_attrsp = status;

      kal_prompt_trace(1, "gprs status %d", smarchbox_attrsp);
      if (smarchbox_attrsp != 1) {
	   smarchbox_uti = ADP_GetFreeUti();

        val = CFW_GprsAtt( CFW_GPRS_ATTACHED, smarchbox_uti, jmetoc_SIM);
			kal_prompt_trace(1, "CFW_GprsAtt return: 0x%x", val);

	   if(0 == val){
		 CFW_SetDataConnFunEx(gprs_conCallBack3,jmetoc_SIM,0);//add by wuys for background QQ, 2010-05-31
		smarchbox_attrsp = 1;
	   } else
	   	jmetoc_soc_ret = -1;

      } else {
        gprs_tcpAct2();
      }
    #else
      //CFW_GetGprsAttState(&status);
      smarchbox_uti = ADP_GetFreeUti();
      if (smarchbox_attrsp != 1) {
        CFW_GprsAtt( CFW_GPRS_ATTACHED, smarchbox_uti);
			  CFW_SetDataConnFunEx(gprs_conCallBack3,0);//add by wuys for background QQ, 2010-05-31
      } else {
        gprs_tcpAct2();
      }
    #endif
    jmetoc_tmp_timer_start();
    while (jmetoc_soc_ret == -2) {  //等待消息，coolsand 中，系统消息和 MMI 应用消息分别在两个 task 中。系统消息在 gprs_conCallBack2 和 gprs_jmetocCallBack2 回调中处理。
      jmetoc_dispatchEvents();
      if (!vm_status) {
        jmetoc_soc_ret = -1;
        jmetoc_trace("force exit1");
        return;
      }
    }
    if (!smarchbox_robot_booted) {
      StopTimer(JMETOC_TIMER_2);
    }
    if (jmetoc_soc_ret == -1) {
      return;
    }

    jmetoc_soc_ret = cs_connect_byip(jmetoc_soc_port, jmetoc_soc_id);  //成功的话进入堵塞
    kal_prompt_trace(mod_trace, "jc con %d", jmetoc_soc_ret);
    if (jmetoc_soc_ret != -1) {
      jmetoc_soc_ret = -2;
      jmetoc_tmp_timer_start();
      while (jmetoc_soc_ret == -2) {  //等待消息，coolsand 中，系统消息和 MMI 应用消息分别在两个 task 中。系统消息在 gprs_conCallBack2 和 gprs_jmetocCallBack2 回调中处理。
        jmetoc_dispatchEvents();
        if (!vm_status) {
          jmetoc_soc_ret = -1;
          jmetoc_trace("force exit2");
          return;
        }
      }
      if (!smarchbox_robot_booted) {
        StopTimer(JMETOC_TIMER_2);
      }
      jmetoc_soc_rsv_buffer_size = 0;
    }
#if 0
  }
#endif
}

/**
 * socket连接到指定服务器。
 * <p>
 * 本方法自动进入堵塞状态。
 * 在堵塞状态下，系统仍然能够相应其他消息，但是连接请求不会返回。
 * </p>
 * @param socketid socket ID
 * @param ip 服务器地址
 * @param port 端口
 * @return 连接是否成功
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI JMETOC_BOOL connect_byip(s8 socketid, u32 ip, u32 port, void* callback) {
    kal_prompt_trace(1, "connect_byip ,callback: 0x%x", callback);
    if(wait_time)
    	{
    	    kal_prompt_trace(1, "connect_serv sleep ");

		MMI_Sleep(3000);
    	}

  jmetoc_soc_id = socketid;
  jmetoc_host_addr = ip;
  jmetoc_soc_port = port;
  socket_callback = (void (*)(s32))callback;
  jmetoc_connect_byip();
  return jmetoc_soc_ret == 0;
}

/**
 * socket 连接到指定服务器。
 * <p>
 * 这个函数实际上完成了服务器域名到 ip 映射的工作，没有进行实际连接。
 * 实际连接操作通过方法 connect_byip 完成。
 * </p>
 * <p>
 * 本方法自动进入堵塞状态。
 * 在堵塞状态下，系统仍然能够相应其他消息，但是连接请求不会返回。
 * </p>
 * @param socketid socket ID
 * @param host 服务器地址
 * @param port 端口
 * @return 是否连接成功
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI JMETOC_BOOL connect_serv(s8 socketid, char* host, u32 port, void* callback) {
    kal_prompt_trace(1, "connect_serv ,callback: 0x%x", callback);
    if(wait_time)
    	{
    	    kal_prompt_trace(1, "connect_serv sleep ");

		MMI_Sleep(3000);
    	}
  socket_callback = (void (*)(s32))callback;
  jmetoc_soc_host = host;
  jmetoc_soc_gethostbyname();
  return (jmetoc_soc_ret == 0 || jmetoc_soc_ret == -2);
}

/**
 * socket发送数据。
 * <p>
 * 本方法自动进入堵塞状态。
 * 在堵塞状态下，系统仍然能够相应其他消息，但是连接请求不会返回。
 * </p>
 * @param socketid socket ID
 * @param data 数据
 * @param len 数据长度
 * @return 成功返回发送长度，失败返回负数错误码。
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI s32 send_data(s8 socketid, u8* data, s32 len) {

  s32 ret;
  ret = CFW_TcpipSocketSend(socketid, data, len, 0);
  kal_prompt_trace(mod_trace, "soc snd %d", ret);
  if (jmetoc_channel_updateing_status == -1) return -1;
  if (ret < len) {  //发不出去，堵塞
    if (ret <= 0) return -1;
    jmetoc_soc_ret = -2;
    jmetoc_tmp_timer_start();
    while (jmetoc_soc_ret == -2) {  //等待消息，coolsand 中，系统消息和 MMI 应用消息分别在两个 task 中。系统消息在 gprs_conCallBack2 和 gprs_jmetocCallBack2 回调中处理。
      jmetoc_dispatchEvents();
      if (!vm_status) {
        jmetoc_trace("force exit3");
        return -1;
      }
    }
    if (!smarchbox_robot_booted) {
      StopTimer(JMETOC_TIMER_2);
    }
  }
  return len;
}

/**
 * socket 是否就绪。
 * <p>
 * 这里所说的就绪是指是否就绪好了读取操作。
 * </p>
 * @param socketid socket ID
 * @return 是否就绪
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI JMETOC_BOOL soc_ready(s8 socketid) {
		  kal_prompt_trace(1, "soc_ready entry");

  //return (nw_event_status & 2) > 0;
  return jmetoc_soc_rsv_buffer_size > 0 || jmetoc_channel_updateing_status == -1;
}

/**
 * socket接收数据。
 * <p>
 * 本方法自动进入堵塞状态。
 * 在堵塞状态下，系统仍然能够相应其他消息，但是连接请求不会返回。
 * </p>
 * @param socketid socket ID
 * @param data 数据
 * @param len 数据长度
 * @return 成功返回接收长度，失败返回负数错误码。
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI s32 rsv_data(s8 socketid, u8* data, s32 len) {
		  kal_prompt_trace(1, "rsv_data entry");

  //void* info;
  s32 ret;
  if (jmetoc_channel_updateing_status == -1) return -1;
  jmetoc_tmp_timer_start();
  while (jmetoc_soc_rsv_buffer_size <= 0) {
    jmetoc_dispatchEvents();
    if (!vm_status) {
      jmetoc_trace("force exit4");
      return -1;
    }
    if (jmetoc_soc_ret == -1) {
      ret = -1;
      goto exit;
    }
  }
  ret = CFW_TcpipSocketRecv(socketid, data, len, 0);
  if (ret > 0) {
    jmetoc_soc_rsv_buffer_size -= ret;
  }
exit:
  kal_prompt_trace(mod_trace, "rsv %d", ret);
  if (!smarchbox_robot_booted) {
    StopTimer(JMETOC_TIMER_2);
  }
  return ret;
}

/**
 * 初始化网络账户。
 * <p>
 * 目前至少要保证 cmwap 和 cmnet 正常工作。
 * </p>
 * @param apn 账户模式，如 "cmwap"
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
extern void soc_close_nwk_account(int mod_id);
JMETOCAPI void setNetworkAPN(const char* apn) {
			  kal_prompt_trace(1, "setNetworkAPN entry");

  //soc_close_nwk_account_by_id(mod_mmi, wap_init_account(), 0);
  //soc_close_nwk_account(mod_mmi);
  jmetocinitNetwork(apn);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
