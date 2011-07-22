/*
 * Module NAME:    rfcomm_types.h
 * PROJECT CODE:   
 * DESCRIPTION:    RFCOMM Types
 * MAINTAINER:     David Airlie
 * DATE:           14 Jan 2000
 *
 * SOURCE CONTROL: $Id: rftest_types.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 * SOURCE:
 *
 * ISSUES:
 *
 * NOTES TO USERS:
 * 
 */
#ifndef PRH_RFCOMM_TYPES_H
#define PRH_RFCOMM_TYPES_H

/* Internal RFCOMM types */
typedef u_int8 prh_t_rfcomm_accept;

#define PRH_RFCOMM_REFUSE 0
#define PRH_RFCOMM_ACCEPT 1

typedef u_int8 prh_t_rfcomm_dlci;
typedef u_int16 prh_t_rfcomm_cid;
typedef u_int8 prh_t_rfcomm_schan;

typedef enum {
  PRH_RFCOMM_CB_CFM,
  PRH_RFCOMM_CB_IND
} prh_t_rfcomm_cb_type;

typedef enum {
    RFDS5, 
    RFDS7, 
    RFDS6, 
    RFDS8 
} prh_t_rfcomm_data_bits;

typedef enum { 
    RF2400, 
    RF4800, 
    RF7200, 
    RF9600, 
    RF19200, 
    RF38400, 
    RF57600, 
    RF115200, 
    RF230400 
} prh_t_rfcomm_port_speed;

typedef enum { 
    RFSTOP1, 
    RFSTOP15 
} prh_t_rfcomm_stop_bits;

typedef enum { 
    RFNONE, 
    RFPARENB 
} prh_t_rfcomm_parity;

typedef enum { 
    RFODD, 
    RFEVEN, 
    RFMARK, 
    RFSPACE 
} prh_t_rfcomm_paritytype;

typedef enum {
  PRH_RFCOMM_FLOW_DATA_STOP,
  PRH_RFCOMM_FLOW_DATA_GO
} prh_t_rfcomm_flow_state;

/* RFCOMM Start - Ref TS 07.10 5.1.2 */
struct PRH_RFCOMM_start_sys_params 
{
  /* From reading of RFCOMM spec + TS07.10, not need anything except below */
  /* Port Speed from the enum */
  prh_t_rfcomm_port_speed port_speed;
  
  /* Max Frame Size 1-128 for RFCOMM, Default: 31 */
  u_int16 max_frame_size;
#if PRH_RFCOMM_CREDIT_FLOW_CONTROL
  /* Request credit based flow from the peer */
  u_int8 enable_credit_flow_control; 
#endif
};
typedef struct PRH_RFCOMM_start_sys_params prh_t_rfcomm_start_sys_params;

/* RFCOMM DLC Parameters - Ref TS 07.10 5.1.8.1.1 */
struct PRH_RFCOMM_dlc_params
{
  /* Priority 0-63 */
  u_int8 priority;
  
  /* Maximum Frame Size 1-32768, Default: 31 */
  u_int16 max_frame_size;
#if PRH_RFCOMM_CREDIT_FLOW_CONTROL
  u_int8 credit_flow_enabled;
  u_int8 local_credits;
  u_int8 peer_credits;
#endif
};
typedef struct PRH_RFCOMM_dlc_params prh_t_rfcomm_dlc_params;

/* defines for flow mask in port parameters */
#define PRH_RPN_FLOW_INPUT_XONXOFF 0x01
#define PRH_RPN_FLOW_OUTPUT_XONXOFF 0x02
#define PRH_RPN_FLOW_INPUT_RTR 0x04
#define PRH_RPN_FLOW_OUTPUT_RTR 0x08
#define PRH_RPN_FLOW_INPUT_RTC 0x10
#define PRH_RPN_FLOW_OUTPUT_RTC 0x20

/* defines for parameter mask in port parameters */
#define PRH_RFCOMM_CC_RPN_PM_BITRATE 0x0001
#define PRH_RFCOMM_CC_RPN_PM_DATABITS 0x0002
#define PRH_RFCOMM_CC_RPN_PM_STOPBITS 0x0004
#define PRH_RFCOMM_CC_RPN_PM_PARITY 0x0008
#define PRH_RFCOMM_CC_RPN_PM_PARITY_TYPE 0x0010
#define PRH_RFCOMM_CC_RPN_PM_XON_CH 0x0020
#define PRH_RFCOMM_CC_RPN_PM_XOFF_CH 0x0040
#define PRH_RFCOMM_CC_RPN_PM_RESV1 0x0080
#define PRH_RFCOMM_CC_RPN_PM_XONXOFF_INPUT 0x0100
#define PRH_RFCOMM_CC_RPN_PM_XONXOFF_OUTPUT 0x0200
#define PRH_RFCOMM_CC_RPN_PM_RTR_INPUT 0x0400
#define PRH_RFCOMM_CC_RPN_PM_RTR_OUTPUT 0x0800
#define PRH_RFCOMM_CC_RPN_PM_RTC_INPUT 0x1000
#define PRH_RFCOMM_CC_RPN_PM_RTC_OUTPUT 0x2000

/* RFCOMM Port Parameters - Ref TS 07.10 5.1.8.2.1 */
struct PRH_RFCOMM_port_parameters {
  prh_t_rfcomm_port_speed port_speed;
  prh_t_rfcomm_data_bits data_bits;
  prh_t_rfcomm_stop_bits stop_bits;
  prh_t_rfcomm_parity parity_enable;
  prh_t_rfcomm_paritytype paritytype;
  u_int16 param_mask;
  u_int8 flow_mask;
  /* Need to add an xon/xoff char */
  u_int8 xon;
  u_int8 xoff;
};
typedef struct PRH_RFCOMM_port_parameters prh_t_rfcomm_port_params;

/* RFCOMM Modem Signal - Ref TS 07.10 5.1.8.2.2 */

/* DTR/DSR -> RTC, RTSCTS -> RTR, RI->IC, DCD->DV, p392 RFCOMM */
#define PRH_RFCOMM_MSC_DTRDSR 1
#define PRH_RFCOMM_MSC_RTSCTS 2
#define PRH_RFCOMM_MSC_RI 4
#define PRH_RFCOMM_MSC_DCD 8

typedef u_int8 prh_t_rfcomm_modem_signal;

/* RFCOMM Control Parameters - Ref TS 07.10 5.1.8.2.2 */
struct PRH_RFCOMM_control_parameters {
  prh_t_rfcomm_modem_signal modem_signal;
  u_int8 break_signal;
  prh_t_rfcomm_flow_state flow_state;
};
typedef struct PRH_RFCOMM_control_parameters prh_t_rfcomm_control_params;

/* RFCOMM Line Status Parameters - Ref TS 07.10 5.1.8.2.3 */

#define PRH_RFCOMM_LSTAT_NO_ERROR 0
#define PRH_RFCOMM_LSTAT_ERROR 1
#define PRH_RFCOMM_LSTAT_OVERRUN_ERROR 2 
#define PRH_RFCOMM_LSTAT_PARITY_ERROR 4
#define PRH_RFCOMM_LSTAT_FRAMING_ERROR 6

typedef unsigned int prh_t_rfcomm_line_stat_params;

#define PRH_RFCOMM_PN_CREDIT_FLOW_REQ 0x0F
#define PRH_RFCOMM_PN_CREDIT_FLOW_RESP 0x0E

#endif
