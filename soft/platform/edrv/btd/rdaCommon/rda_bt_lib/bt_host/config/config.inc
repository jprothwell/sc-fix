/*
 * module name : default.inc
 * Default stack settings for normal build 
 *
 * Source Control : $Id: config.inc 6166 2011-02-23 10:34:40Z admin $
 *
 * LICENSE: Copyright (C) Parthus Technologies 2002.
 */


/* All timer values are in tenths of a second, i.e. 400 - 40 seconds */


/* Stack Level Defines */
#define RFCOMM_PRESENT 1

#define BOOTUP_HC_RESPONSE_TIMEOUT  30

#define HOST_SECURITY	1

/* Device Mode */
#define PRH_SNIFF_SUPPORTED 1
#define PRH_PARK_SUPPORTED 1

/* Stack/HCI - Buffer limits */
#define STACK_MAX_OUTGOING_QUEUED_HCI_COMMAND_BYTES 512
#define STACK_MAX_OUTGOING_QUEUED_ACL_BYTES_PER_HANDLE    4256*7 //12768
#define STACK_MAX_OUTGOING_QUEUED_SCO_BYTES_PER_HANDLE    0
#define STACK_MAX_OUTGOING_CONTROL_MESSAGE_RESERVED_BYTES 512

/* HCI - HC to Host Flow Control */
#define PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED                  1
#define PRH_HOST_HC_TO_HOST_FLOW_CONTROL_REPORTING_FREQUENCY        0 
#define PRH_HOST_HC_TO_HOST_FLOW_CONTROL_ACL_REPORTING_THRESHOLD    10
#define HCI_HC_TO_HOST_FLOWCTRL_ACL_NUM     20
#define HCI_HC_TO_HOST_FLOWCTRL_ACL_SIZE    400
#define HCI_HC_TO_HOST_FLOWCTRL_SCO_NUM     0
#define HCI_HC_TO_HOST_FLOWCTRL_SCO_SIZE    0

/* headset app */
#define HEADSET_PRESENT 1 

#ifndef __BT_NO_A2DP_PROFILE__
#define __BT_A2DP_PROFILE__
#define __BT_AVRCP_PROFILE__
#endif

/* A2DP profile */
#ifdef  __BT_A2DP_PROFILE__
#define A2DP_PRESENT 1
#define A2DP_SRC     1
#define A2DP_SNK     0
#else
#define A2DP_PRESENT 0
#define A2DP_SRC     1
#define A2DP_SNK     0
#endif

/* HFP profile */
#define HFP_PRESENT 1
#define HFP_AG      1
#define HFP_HF      0

/* AVRCP profile */
#ifdef  __BT_AVRCP_PROFILE__
#define AVRCP_PRESENT 1 
#define AVRCP_TG      1
#define AVRCP_CT      0
#else
#define AVRCP_PRESENT 0 
#define AVRCP_TG      1
#define AVRCP_CT      0
#endif

/* SDP - Server */
#define SDP_SERVER 1
#define SDP_RECORD_GEN 0
#define SDP_ROM_SUPPORT 1
#define SDP_ROM_CREATION 0

/* SDP - Client */
#define SDP_CLIENT 1
#define SDP_SS_AND_SA_SUPPORT 1
#define SDP_SSA_SUPPORT 1
#define SDP_GENERIC_BROWSE 1
#define SDP_SDAP 1
#define SDP_INTERFACE 1
#define SDP_DB_TO_FILE 0

/* RFCOMM */
#define PRH_RFCOMM_RSE_SUPPORT 1
#define PRH_RFCOMM_PE_SUPPORT 0

#define PRH_RFCOMM_INCOMING_SM3 0

#define PRH_RFCOMM_INITIAL_CREDITS 7
#define PRH_RFCOMM_MAX_CREDITS 255
#define PRH_RFCOMM_CREDIT_MIN 3
#define PRH_RFCOMM_CREDIT_ISSUE 7

//#define PRH_RFCOMM_MAX_FRAME_SIZE 510//10240  //cuijian changed it.
#define PRH_RFCOMM_MAX_FRAME_SIZE 329//10240  //cuijian changed it.

/* L2CAP */
#define PRH_L2CAP_AUTO_CONFIG 1

#define L2CAP_GROUPS_SUPPORTED 1
#define L2CAP_CLS_TRAFFIC_SUPPORTED 1
#define L2CAP_PING_SUPPORTED 1
#define L2CAP_INFO_SUPPORTED 1
#define L2CAP_AGGREGATE_QOS 0 
#define L2CAP_CONFIG_VALUES_BELOW_HCI 1

#define PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS  0x30
#define PRH_L2_MAX_NUM_LINKS 0x06
#define PRH_L2_MAX_ECHO_DATA_SIZE 0x100
#define PRH_L2_MAX_CLS_MTU 0x04ED
#define PRH_L2_MAX_NUM_PSM 10
#define PRH_L2_MAX_GROUPS 10

/* TCS */
#define PRH_TCS_GATEWAY		0
#define PRH_TCS_TERMINAL	0

#ifndef PRH_TCI_TCS_BIN
#define PRH_TCI_TCS_BIN     0
#endif

/* Manager */
#define MGR_DEVDB_MAX_ENTRIES           10 
#define MGR_DEVDB_CACHE_REMOTE_NAME     1 
	
#define HOST_INQDB_CULL_THRESHOLD		0 
#define HOST_INQDB_MAX_DEVICE_ENTRIES   20
#define HOST_INQDB_CULL_GRANULARITY     0 
#define HOST_INQDB_CULL_MIN_ENTRIES     10 

/* See MGR_SetLinkFlags */
#define MGR_LINKFLAGS_DEFAULT           (MGR_KEYREQ_HANDLE)


/* OBEX */
#define OBEX_SERVER 1
#define OBEX_CLIENT 1
#define OBEX_SERIAL 0
#define OBEX_SOCKET 0
#define OBEX_RFCOMM 1

#define OBEX_GENERIC 1
#define OBEX_SEC 1

#define OBEX_PUSH_CLIENT 0
#define OBEX_PUSH_SERVER 1

#define OBEX_SYNC_CLIENT 0
#define OBEX_SYNC_SERVER 0

#define OBEX_FTP_CLIENT 1
#define OBEX_FTP_SERVER 1


/* Debug */
#if pDEBUG
#	define pDEBUGLEVEL pLOGNOTICE
#define pDEBUGLAYERS pLOGALWAYS
#endif

