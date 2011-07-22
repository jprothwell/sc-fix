/**************************************************************************
 * MODULE NAME:    hfp_const.h
 * PROJECT CODE:
 * DESCRIPTION:    HFP Constants
 * AUTHOR:         Gary Fleming, Daire McNamara
 * DATE:           15-10-2004
 *
 * SOURCE CONTROL: 
 *
 * LICENSE:
 *     This source code is copyright (c) 2004 Ceva Inc.
 *     All rights reserved.
 *
 **************************************************************************/

#ifndef _HFP_CONST_H
#define _HFP_CONST_H

/***************************************************************/
/*  Stuff From HFP_Const.h                                     */
/***************************************************************/


#define HFP_BT_WRITE_LE_UINT32(ptr,val)	(((*((u_int8 *)(ptr)))=((u_int8)(val))),((*((u_int8 *)((ptr)+1)))=((u_int8)((val)>>8))),((*((u_int8 *)((ptr)+2)))=((u_int8)((val)>>16))),((*((u_int8 *)((ptr)+3)))=((u_int8)((val)>>24))))
#define HFP_BT_READ_LE_UINT32(ptr)		((u_int32)(((u_int32)(*((u_int8 *)(ptr))))|((*((u_int8 *)((ptr)+1)))<<8)|((*((u_int8 *)((ptr)+2)))<<16)|((*((u_int8 *)((ptr)+3)))<<24)))
#define HFP_BT_WRITE_LE_UINT16(ptr,val)	(((*((u_int8 *)(ptr)))=((u_int8)(val))),((*((u_int8 *)((ptr)+1)))=((u_int8)((val)>>8))))
#define HFP_BT_READ_LE_UINT16(ptr)		((u_int16)(((u_int16)(*((u_int8 *)(ptr))))|((*((u_int8 *)((ptr)+1)))<<8)))

/*******************************************
 *  SLC States
 *******************************************/
#define SCM_CLOSED 0
#define SCM_W4_INDICATOR_CONTROL_OK 1
#define SCM_W4_INDICATOR_STATUS 2
#define SCM_W4_INDICATOR 3
#define SCM_W4_CALL_HOLD_INFO_OK 4
#define SCM_W4_ENABLE_INDICATOR_STATUS_UPDATE_OK 5
#define SCM_W4_CALL_HOLD_INFO   6
#define SCM_W4_SUPPORTED_FEATURES 7
#define SCM_W4_INDICATOR_CONTROL 8
#define SCM_W4_INDICATOR_STATUS_OK  10
#define SCM_W4_RFCOMM_CONNECT 11
#define SCM_OPEN 12
#define SCM_W4_SUPPORTED_FEATURES_OK 13
#define SCM_W4_USER_ACCEPT 14
#define SCM_SDP_DISCOVERY_PENDING 15
#define SCM_W4_EXIT_SNIFF_PRIOR_TO_DISCONNECT 16
#define SCM_W4_RFCOMM_DISCONNECT_INDICATION  17

/********************************************************************
 * For release purposes indicate if we connected as client or server
 ********************************************************************/
#define RFCOMM_CLIENT  2
#define RFCOMM_SERVER  3

/********************************************************************
 * Default Supported Features
 ********************************************************************/
#define HF_UNIT_SUPPORTED_FEATURES_U_INT16  24  //remote volume control:supported, voice recognition activation:supported



/********************************************************************
 * Defines for each distinct type of AT-COMMAND/USR/Test command.
 * Currently not used - intended to be used for OS Queueing of AT-commands
 ********************************************************************/
// AT COMMANDS SUPPORTED
#define HF_AT_CMD_CALL_ANSWER                   1 //  "ATA\r"
#define HF_AT_CMD_CALL_WAITING                  2 // "AT+CCWA\r"
#define HF_AT_CMD_CALL_HOLD						3 // "AT+CHLD\r"
#define HF_AT_CMD_CALL_HANGUP				    4 // "AT+CHUP\r"
#define HF_AT_CMD_CALL_INDICATORS				5 // "AT+CIND\r"
#define HF_AT_CMD_CALL_LINE_ID					6 // "AT+CLIP\r"
#define HF_AT_CMD_INDICATORS_EVENT_REPORTING    7 // "AT+CHLD\r"
#define HF_AT_CMD_GENERATE_DTMF                 8 // "AT+VTS\r"
#define HF_AT_CMD_BT_REQ_INPUT                  9 // "AT+BINP\r"
#define HF_AT_CMD_BT_LAST_DIALED_NUM           10 // "AT+BLDN\r"
#define HF_AT_CMD_BT_VOICE_REC_ACT             11 // "AT+BVRA\r"
#define HF_AT_CMD_BT_RET_SUPPORTED_FEATURES    12 // "AT+BRSF\r"
#define HF_AT_CMD_BT_DISABLE_EC_RN             13 // "AT+NREC\r"
#define HF_AT_CMD_BT_MICROPHONE_GAIN            14 // "AT+VGM\r"
#define HF_AT_CMD_BT_SPEAKER_GAIN              15 // "AT+VGS\r"
#define HF_AT_CMD_CKPD                         16 // "AT+CKPD=200\r"

// AT TEST COMMANDS SUPPORTED
#define HF_AT_TST_CMD_CALL_HOLD				   21 // "AT+CHLD=?\r"
#define HF_AT_TST_CMD_CALL_INDICATORS		   22 // "AT+CIND=?\r

// AT READ COMMANDS
#define HF_AT_READ_CMD_CALL_INDICATORS		   31 // "AT+CIND?\r"

// AT UNSOLICITATED RESULT CODES
#define HF_AT_USR_INBAND_RING_TONE             39
#define HF_AT_USR_CALL_INDICATOR_CONTROL       40
#define HF_AT_USR_CALL_WAITING                 41 //"\r\n+CCWA\r\n"
#define HF_AT_USR_CALLING_LINE_ID              42 //"\r\n+CLIP\r\n"
#define HF_AT_USR_CALL_INDICATORS              43 //"\r\n+CIEV\r\n"
#define HF_AT_USR_BT_VOICE_REC_ACT             44 //"\r\n+BVRA\r\n"
#define HF_AT_USR_BT_RET_SUPPORTED_FEATURES    45 //"\r\n+BRSF\r\n"
#define HF_AT_USR_BT_SET_IN_BAND_RING_TONE     46 //"\r\n+BSIR\r\n"
#define HF_AT_USR_BT_MICROPONE_GAIN            47 //"\r\n+VGM\r\n"
#define HF_AT_USR_BT_SPEAKER_GAIN              48 //"\r\n+VGS\r\n"
#define HF_AT_USR_CALL_HOLD                    49

#define HF_AT_ERROR                            50
#define HF_AT_OK                               60
#define HF_AT_UNKNOWN_COMMAND                  90


/********************************************************************
 * Operation codes used to indicate what AT Command / USR is pending
 ********************************************************************/
#define AT_BRSF         10
#define AT_CIND_TEST    11
#define AT_CLIP_0       12
#define AT_CLIP_1       13
#define AT_CHLD_TEST    14
#define AT_CHUP         15
#define AT_BLDN         16
#define AT_VGM          17
#define AT_NREC_0       18
#define AT_BRVA_0       19
#define AT_BRVA_1       20
#define AT_CMER         21
#define AT_CIEV_3001    22
#define AT_CIEV_3000    23
#define AT_CCWA_0       24
#define AT_CCWA_1       25
#define AT_CIND         26
#define AT_VGS          27
#define ATD             28
#define AT_VTS          29
#define AT_ANSWER       30
#define AT_CHLD         31
#define AT_BINP         32

/***********************************************
 * Call States
 * Currently not complete and not extensively used
 *************************************************/
#define HFP_CALL_CLOSED                02
#define HFP_CALL_ESTABLISHMENT         04
#define HFP_CALL_W4_ADT_OK             10
#define HFP_CALL_W4_CIEV_CALL_SETUP_2  11
#define HFP_CALL_W4_CIEV_CALL_1        12
#define HFP_CALL_ACTIVE                13
#define HFP_CALL_W4_USER_ACCEPT        14
#define HFP_W4_PEER_CALL_CONNECT       15
#define HFP_LOCAL_USER_INIT_CALL_REJECT 16
#define HFP_LOCAL_USER_INIT_CALL_RELEASE 17
// Additional States for Call handling when acting as AG
#define HFP_CALL_ALERTING              20
#define HFP_CALL_ANSWERED              21



// Indicators

#define HFP_SERVICE_INDICATOR        20
#define HFP_CALLSETUP_INDICATOR      21
#define HFP_CALL_INDICATOR           22

//#define DIALED_NUMBER_DIALING  1
//#define MEMORY_INDEX_DIALING   2
//#define LAST_NUMBER_REDIAL     3

/*********************************************
 * HFP Server Channel - Uses in SDP records                      
 *********************************************/

#define PRH_HANDS_FREE_PROFILE_SER_CHANNEL 8


/*********************************************
 * HFP Class of Device                     
 *********************************************/

#define HANDS_FREE_UNIT_COD     0x200408
#define HANDS_FREE_AG_COD       0x200408
#define HEADSET_UNIT_COD        0x200404
#define HEADSET_AG_COD          0x200404


/************************************************
 * MESSAGE IDs for OS Queues                  
 * Used for the Application to deliver API calls into
 * the system.
 ************************************************/
#if COMBINED_HOST==0

#define MSG_HFP_SDP_BROWSE      0
#define MSG_HFP_SCM_CONNECT     1
#define MSG_HFP_SCM_ACCEPT      2
#define MSG_HFP_SCM_REJECT      3
#define MSG_HFP_SCM_DISCONNECT  4
#define MSG_HFP_BOND            5
#define MSG_HFP_SCM_SEND_DTMF   6
#define MSG_HFP_SETUP_AUDIO     7
#define MSG_HFP_RELEASE_AUDIO   8
#define MSG_HFP_RFCOMM_EVENT_DISCONNECT 9
#define MSG_HFP_SETUP_CALL_NUM      10
#define MSG_HFP_SETUP_CALL_MEM      11
#define MSG_HFP_SETUP_CALL_LAST     12

#define MSG_HFP_ACCEPT_CALL     13
#define MSG_HFP_REJECT_CALL     14
#define MSG_HFP_RELEASE_CALL    15
#define MSG_HFP_ENABLE_CALL_WAITING 16
#define MSG_HFP_DISABLE_CALL_WAITING 17
#define MSG_HFP_ENABLE_CLIP 18
#define MSG_HFP_DISABLE_CLIP 19
#endif

/**************************************
 *  AUDIO STATES
 **************************************/
#define HFP_AUDIO_OPEN   10
#define HFP_AUDIO_CLOSED 11
#define HFP_AUDIO_SETUP_IN_PROGRESS 12
#define HFP_AUDIO_SETUP_PENDING  13
#define HFP_AUDIO_W4_EXIT_SNIFF 14
#define HFP_AUDIO_W4_USER_ACCEPT 15
#define HFP_REJECTING_AUDIO_W4_CONNECTION_COMPLETE 16


#define PRH_HFP_MAX_DTMF_STRING_SIZE 20

#define BT_HFP_PROTOCOL_VERSION 0x0105

#define BT_HFP_PROTOCOL_VERSION_105 0x0105
#define BT_HFP_PROTOCOL_VERSION_100 0x0101
#define BT_HFP_PROTOCOL_VERSION_096 0x0100

#define PRH_HFP_SUPPORTED_FEATURES 0x0009

#define HFP_DEFAULT_SUPPORTED_FEATURES  0x0000
#define HFP_DEFAULT_VERSION_INFORMATION 0x0100

/**************************************
 * CALL CONTROL COMMAND ID - Used for CBs
 ****************************************/
#if COMBINED_HOST==0
#define HFP_CALL_REQUEST_API  1
#define HFP_CALL_ACCEPT_API   2
#define HFP_CALL_REJECT_API   3
#define HFP_CALL_RELEASE_API  4
#define HFP_MULTIP_PARTY_API  5
#endif

//ACL status
#define HFP_DEVACTIVE			0
#define HFP_DEVHOLD				1
#define HFP_DEVSNIFF			2
#define HFP_DEVPARK				3
#define HFP_DEVPENDINGHOLD		14
#define HFP_DEVPENDINGSNIFF		15
#define HFP_DEVPENDINGEXITSNIFF	16
#define HFP_DEVPENDINGPARK		17
#define HFP_DEVPENDINGEXITPARK	18
#define HFP_DEVIDLE             20


#define PRH_HFP_RSE_RETRANSMIT_TIMEOUT 10 // 1 second delay on re-transits due to flowcontrol
#define PRH_HFP_SNIFFMODE_TIMEOUT 30 // 3 Second Delay on entering Sniff
#define PRH_HFP_REATTACH_TIMEOUT  100 // NOT USED
#define PRH_HFP_SLC_SETUP_TIMEOUT  400  // 40 seconds ( has to be larger than the L2CAP RTX timeout)
#define PRH_HFP_RING_TIMEOUT      20  // 2 Seconds between rings


#define HFP_SPEAKER    0
#define HFP_MICROPHONE 1

#define HFP_HCI_cmdSNIFFMODE							((0x2<<10) | 0x3)
#define HFP_HCI_cmdEXITSNIFFMODE						((0x2<<10) | 0x4)


#define ACTIVE_PROFILE_HEADSET            0x01
#define ACTIVE_PROFILE_HEADSET_AG         0x02
#define ACTIVE_PROFILE_HANDSFREE          0x04
#define ACTIVE_PROFILE_HANDSFREE_AG       0x08

// add by tianwq
//#define SDP_SCLASS_UUID_HANDSFREE	  0x111E   //moved to the sdp_constants.h. Zhu jianguo. 2007.1.4
//#define SDP_SCLASS_UUID_HANDSFREE_AUDIOGATEWAY 0x111F
//#define SDP_AID_SUPPORTED_FEATURES	  0x1309

#define DB_SDP_PROGRESS			0

//HCI_SYNCHRONOUS_CONNECTION_COMPLETE_EVENT
//#define HCI_evSYNCHRONOUSCONNECTIONCOMPLETE           0x2C   moved to the hci_constants.h  Zhu Jianguo. 2007.1.4

#define DB_SDP_TRUE	1
#define DB_SDP_FALSE	0
#endif

