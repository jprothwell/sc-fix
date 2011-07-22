/*
 * module name : default.inc
 * Default stack settings for normal build 
 *
 * Source Control : $Id: headset.inc 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE: Copyright (C) Parthus Technologies 2002.
 */


/* All timer values are in tenths of a second, i.e. 400 - 40 seconds */


/* Stack Level Defines */
#define RFCOMM_PRESENT 1

#define BOOTUP_HC_RESPONSE_TIMEOUT  30

#define HOST_SECURITY	0

/* headset app */
#define HEADSET_PRESENT 1 

/* A2DP profile */
#define A2DP_PRESENT 1
#define A2DP_SRC     0
#define A2DP_SNK     1

/* HFP profile */
#define HFP_PRESENT 1
#define HFP_AG      0
#define HFP_HF      1

/* AVRCP profile */
#define AVRCP_PRESENT 1 
#define AVRCP_TG      0
#define AVRCP_CT      1

/* Device Mode */
#define PRH_SNIFF_SUPPORTED 0
#define PRH_PARK_SUPPORTED 0

/* Stack/HCI - Buffer limits */
#define STACK_MAX_OUTGOING_QUEUED_HCI_COMMAND_BYTES 512
#define STACK_MAX_OUTGOING_QUEUED_ACL_BYTES_PER_HANDLE    1024
#define STACK_MAX_OUTGOING_QUEUED_SCO_BYTES_PER_HANDLE    0
#define STACK_MAX_OUTGOING_CONTROL_MESSAGE_RESERVED_BYTES 512

/* HCI - HC to Host Flow Control */
#define PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED                  0
#define PRH_HOST_HC_TO_HOST_FLOW_CONTROL_REPORTING_FREQUENCY        0 
#define PRH_HOST_HC_TO_HOST_FLOW_CONTROL_ACL_REPORTING_THRESHOLD    10
#define HCI_HC_TO_HOST_FLOWCTRL_ACL_NUM     10
#define HCI_HC_TO_HOST_FLOWCTRL_ACL_SIZE    512
#define HCI_HC_TO_HOST_FLOWCTRL_SCO_NUM     0
#define HCI_HC_TO_HOST_FLOWCTRL_SCO_SIZE    0

/* SDP - Server */
#define SDP_SERVER 1
#define SDP_RECORD_GEN 0
#define SDP_ROM_SUPPORT 1
#define SDP_ROM_CREATION 0

/* SDP - Client */
#define SDP_CLIENT 0
#define SDP_SS_AND_SA_SUPPORT 0
#define SDP_SSA_SUPPORT 0
#define SDP_GENERIC_BROWSE 0
#define SDP_SDAP 0
#define SDP_INTERFACE 0
#define SDP_DB_TO_FILE 0

/* RFCOMM */
#define PRH_RFCOMM_RSE_SUPPORT 1
#define PRH_RFCOMM_PE_SUPPORT 0

#define PRH_RFCOMM_INCOMING_SM3 0

#define PRH_RFCOMM_INITIAL_CREDITS 7
#define PRH_RFCOMM_MAX_CREDITS 255
#define PRH_RFCOMM_CREDIT_MIN 3
#define PRH_RFCOMM_CREDIT_ISSUE 7

#define PRH_RFCOMM_MAX_FRAME_SIZE 127

/* L2CAP */
#define PRH_L2CAP_AUTO_CONFIG 1

#define L2CAP_GROUPS_SUPPORTED 0
#define L2CAP_CLS_TRAFFIC_SUPPORTED 0
#define L2CAP_PING_SUPPORTED 0
#define L2CAP_INFO_SUPPORTED 0
#define L2CAP_AGGREGATE_QOS 0 
#define L2CAP_CONFIG_VALUES_BELOW_HCI 0

#define PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS  0x30
#define PRH_L2_MAX_NUM_LINKS 0x01
#define PRH_L2_MAX_ECHO_DATA_SIZE 0x100
#define PRH_L2_MAX_CLS_MTU 0x04ED
#define PRH_L2_MAX_NUM_PSM 5
#define PRH_L2_MAX_GROUPS 10

/* TCS */
#define PRH_TCS_GATEWAY		0
#define PRH_TCS_TERMINAL	0
#define PRH_TCI_TCS_BIN  	0

/* Manager Database Configuration */
#define MGR_DEVDB_MAX_ENTRIES           5
#define MGR_DEVDB_CACHE_REMOTE_NAME     0
	
#define HOST_INQDB_CULL_THRESHOLD		0 
#define HOST_INQDB_MAX_DEVICE_ENTRIES   20
#define HOST_INQDB_CULL_GRANULARITY     0 
#define HOST_INQDB_CULL_MIN_ENTRIES     10 

/* See MGR_SetLinkFlags */
#define MGR_LINKFLAGS_DEFAULT           (MGR_CONNREQ_ACCEPT_ALL|MGR_SECREQ_HANDLE_ALL|MGR_CONNSETUP_REFUSE_SWITCH)

/* OBEX */
#define OBEX_SERVER 0
#define OBEX_CLIENT 0
#define OBEX_SERIAL 0
#define OBEX_SOCKET 0
#define OBEX_RFCOMM 0

#define OBEX_GENERIC 0
#define OBEX_SEC 0

#define OBEX_PUSH_CLIENT 0
#define OBEX_PUSH_SERVER 0

#define OBEX_SYNC_CLIENT 0
#define OBEX_SYNC_SERVER 0

#define OBEX_FTP_CLIENT 0
#define OBEX_FTP_SERVER 0


/* Debug */
#if pDEBUG
#	define pDEBUGLEVEL pLOGNOTICE
#define pDEBUGLAYERS pLOGALWAYS
#endif

