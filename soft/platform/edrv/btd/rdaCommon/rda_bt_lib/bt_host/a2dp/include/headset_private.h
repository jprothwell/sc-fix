/***********************************************************************
 *
 * MODULE NAME:    headset_private.h
 * DESCRIPTION:     
 * MAINTAINER:     Zhu Jianguo
 * CREATION DATE:  17 Sept. 2008
 *
 * *
 * LICENSE:
 *     This source code is copyright (c) 2008 RDA Inc.
 *     All rights reserved.
 *
 ***********************************************************************/
#ifndef _HEADSET_PRIVATE_H_
#define _HEADSET_PRIVATE_H_

typedef struct
{
    u_int8 hfp_volume;
    u_int8 av_volume;
} volume_type;

typedef enum
{
    none_active,
    hfp_active,
    av_active
} activeProfileState;


typedef struct
{

    /* Common states */
	volume_type         speaker_volume;
	activeProfileState  active_profile;
    
    /* AV states */
	//A2DP*              a2dp;
	//AVRCP*            avrcp;
	//boolean                  avrcp_pending; 	
	
    /* HFP states */
/*	headsetHfpState     hfp_state;
	uint16              headset_connecting_hfp;
	Sink                slc_sink;
	Sink                sco_sink;
	uint16              supp_features_0;
	hfp_profile         profile_connected;
	HFP                 *hfp;
	HFP                 *hsp;    
	uint16              mic_volume;
	*/
	
	/* Boolean variables */
	unsigned			mic_mute_on:1;
	unsigned			using_wbs:1;	
	unsigned			charger_connected:1;
	unsigned			battery_full:1;
	
	unsigned            voice_recognition_enabled:1;
	unsigned            in_band_ring_enabled:1;
	unsigned            pairing_enabled:1;
	unsigned            sent_suspend:1;
	
	unsigned			connect_av:1;	
	unsigned            av_stream_stopped:1;	
	unsigned            stored_av_stream_stopped:1;	
	unsigned            headset_connecting_av:1;
	
	unsigned 			play_state:1;
	unsigned			av_stream_connected:1;
	unsigned			dummy:2;
        
} t_headsetData;

#endif 
