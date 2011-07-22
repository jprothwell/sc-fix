/* !=============================================================! */
/* !                M O D U L E     B O D Y                      ! */
/* !=============================================================! */
/* !                                                             ! */
/* ! Filename: hfp_type.h  (Nov, 2004)                           ! */
/* !                                                             ! */
/* ! Authors:	G.Fleming, D.McNamara	                         ! */
/* !                                                             ! */
/* ! Description:                                                ! */
/* !	Base hfp_types                                           ! */
/* !                                                             ! */
/* ! History:                                                    ! */
/* !                                                             ! */
/* !=============================================================! */
#ifndef HFP_TYPES_H
#define HFP_TYPES_H

#include "host_types.h"
#if COMBINED_HOST==0
#include "papi.h"
#else
typedef u_int t_pEvent;
#endif



/*************************************
 *  t_HFP_AG_Peer_Device
 *
 *  Structure desctibing an SLC link.
 *
 *
 *************************************/

typedef struct 
{
	t_bdaddr address;                 // Bluetooth Address of the peer AG
	u_int8 scm_state;                 // Current State of the Service link to the peer
	u_int8 acl_state;                 // Current mode of the ACL link to the peer.
	u_int8 operation_pending;         // Current AT Command pending from the peer.
	void* p_service;                  // A pointer to service information on the peer.
	u_int8 call_handling;  // 
	u_int8 serverChannel;
	u_int8 local_serverChannel;
	u_int16 versionInfo;
	u_int8 call_state;
	
	u_int8 service_ind_order;
	u_int8 call_setup_ind_order;
	u_int8 call_ind_order;         
	u_int8 service_ind;             //service availability indicator. 0/1: Home/Roam network not/available
	u_int8 call_ind;                  //call status indicator: 0:no call active, 1:a call is active
	u_int8 call_setup_ind;       //call setup indicator  // 0: not currently in call setup, 1: an incoming call process ongoing
											// 2: an outgoing call setup process ongoing, 3: remote party being alerted in an outgoing call						
	
	u_int16 dlci;
	u_int8 rf_role;
	u_int8 remote_feature_status;
	u_int32 supportedFeatures;       // Features supported by the peer 
	u_int8 pending_data[50];//u_int8* pending_data; was used to save the AT command, so 50 bytes maybe enough. zhujianguo.
	u_int8  pending_data_len;
	u_int16 acl_handle;
	u_int16 sco_handle;
	u_int8 audio_state;
	u_int8 inband_tone;
	t_pEvent evHFP_TxOK;
	u_int8 HFP_TxOK;
	t_pTimer enter_sniff_mode_timer_handle;
	t_pTimer slc_setup_timer_handle;
	t_pTimer ring_timer;
	u_int8 active_profile;
} t_HFP_AG_Peer_Device;

/**********************************
 * t_AT_Event
 *
 * Structure for Queueing API Rx AT-COMMANDS 
 ***********************************/

typedef struct st_t_AT_Event {
  t_bdaddr address;
  u_int8 cmd;
  u_int8 n_value;
  u_int16 dlci;
  u_int32 u_int16_value;
  u_int16 ind_value;
  u_int32* p_ind_status;
  void* p_service;

}t_AT_Event;

typedef struct st_t_SCN_values {
	u_int8 number;
	u_int8 used;
}t_SCN_values;

// add by tianwq
typedef struct st_TdbDeviceRecord {
	t_bdaddr bdAddress;
	struct {
		struct {
			u_int8 sdp_state;
		}hfp;
	}profile;
} TdbDeviceRecord ;

#endif /*GW_AG_L_H*/

