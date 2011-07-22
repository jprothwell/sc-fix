/***********************************************************************
 *
 * MODULE NAME:    avdtp_callbacks.c
 * DESCRIPTION:    Audio/video Distribution Profile callback code
 * MAINTAINER:      Zhu Jianguo
 *
 * 
 * LICENSE:
 *     This source code is copyright (c) 2008 RDA Ltd.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/


#include "host_config.h"
#include "host_buf.h"
#include "l2cap_primitives.h"
#include "l2_config_helper.h"
#include "papi.h"

#include "a2dp.h"
#include "avrcp.h"
#include "host_msg.h"

 AVRCP gAVRCP;

//#ifdef COOLSAND
//avrcp_signal_callbacks gSig_Cbs;
//#endif
extern u_int32 rdabt_put_message(u_int16 q, u_int16 mi, void *mv);
void Avrcp_Handle_Received_Data(u_int8* ptr,t_channelId cid, u_int16 packet_size);

/* Reset the local state values to their initial states */
void Avrcp_Set_State(t_AVRCP_STATE state);

static void Reset_Avrcp_Values()
{
    /* Set the local state to ready */
    Avrcp_Set_State(AVRCP_READY);

    /* Reset the remaining local state */
    gAVRCP.pending = AVRCP_NONE;
    gAVRCP.block_received_data = 0;
    gAVRCP.transaction_label = 0;
    gAVRCP.fragmented = AVRCP_FRAG_NONE;
    gAVRCP.cid =0;
    
#ifdef COOLSAND    
    gAVRCP.chnl_num=0;
    gAVRCP.local_role = 0;
#endif
    
}

/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_l2cap_connect_ind_cb
 * DESCRIPTION:
 
 This function will handle the callback for a connection indication. It
 will send back the response to the caller. This will also initiate the
 configuration of the link from this side.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void Avrcp_L2cap_Connect_Ind_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs)
{
	u_int16 response;
	u_int8 error_code;
	t_L2_ConfigReqParams req_params;
#ifdef COOLSAND	
       // t_avrcp_connect_ind msg;
#endif

    req_params.inMTUmin=PRH_L2CAP_MTU_MIN;
    req_params.inMTUmax=900;
    req_params.outMTUmin=PRH_L2CAP_MTU_MIN;
    req_params.outMTUmax=900;
	req_params.flushTO=0xffff;
	req_params.linkTO=0;

	response = 0; /* accept connection */
    
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avrcp_L2cap_Connect_Ind_Cb: type=%d pArgs->cid=0x%x\n",type,pArgs->cid));
#endif  

	Avrcp_Set_State(AVRCP_CONNECTING);	
        pMemcpy( gAVRCP.device_addr.bytes,pArgs->BDAddress.bytes,6);

	error_code = L2_ConnectResponseandConfig(pArgs->BDAddress, pArgs->cid, response, 0, &req_params);
}

/***********************************************************************
 *
 * FUNCTION NAME:   Avrcp_L2cap_Connect_Cfm_Cb
 * DESCRIPTION:
 
 This function will handle the callback for a connection confirm. 

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void Avrcp_L2cap_Connect_Cfm_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs)
{
#ifdef COOLSAND
#if AVRCP_CT
   t_avrcp_connect_cnf result;
#endif
#endif

     pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"AVRCP - Cconnect cfm,status:%x, result:%x",pArgs->status,pArgs->result));

    if(pArgs->status !=BT_NOERROR)
        return;
 
#ifdef COOLSAND
#if AVRCP_CT
    pMemcpy( gAVRCP.device_addr.bytes,pArgs->BDAddress.bytes,6);
    result.chnl_num =  gAVRCP.chnl_num;
    result.connect_id = pArgs->cid;
    result.result = BT_NOERROR;
    pMemcpy(result.device_addr.bytes, pArgs->bdaddr.bytes,6);
    rdabt_put_message(EXTERNAL_TASK_MASK,AVRCP_Q|AVRCP_CONNECT_CNF,&result);

#endif
#endif
 
}

/***********************************************************************
 *
 * FUNCTION NAME:   Avrcp_L2cap_Config_Ind_Cb
 * DESCRIPTION:
 
 This function will handle the callback for a config indication. 

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void Avrcp_L2cap_Config_Ind_Cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
	u_int16 response;
	u_int8 error_code;
	
	response = 0;
	
#if pDEBUG
	pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"Avrcp_l2cap_config_ind_cb pArgs->cid=%x",pArgs->cid));
#endif
	
	error_code = L2_ConfigureResponse(pArgs->cid, 0,response,0x00);
	if(error_code==BT_NOERROR)
	{
            error_code=L2_Configure(pArgs->cid, 672, 65535,0,0);
	}
}

/***********************************************************************
 *
 * FUNCTION NAME:   Avrcp_L2cap_Configure_Cfm_Cb
 * DESCRIPTION:
 
 This function will handle the callback for a config confirm. 

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void Avrcp_L2cap_Configure_Cfm_Cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
#ifdef COOLSAND	
        t_avrcp_connect_ind msg;
#endif

#if pDEBUG
       pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"Avrcp_L2cap_Configure_Cfm_Cb:psm:%x,cid:%x,",pArgs->psm, pArgs->cid));
#endif    

#ifdef COOLSAND	
        gAVRCP.cid=  pArgs->cid;
        msg.chnl_num = gAVRCP.chnl_num;
        msg.connect_id = pArgs->cid;
        pMemcpy(msg.device_addr.bytes,gAVRCP.device_addr.bytes,6);
        rdabt_put_message(EXTERNAL_TASK_MASK,AVRCP_Q|AVRCP_CONNECT_IND,&msg);
	Avrcp_Set_State(AVRCP_CONNECTED);
#endif	
}

/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_l2cap_data_ind_cb
 * DESCRIPTION:
 
 This function will firstly print out the contents of the recieved buffer.
 It is the callback for the reception of data for the avdtp psm. It will
 then call the avdtp_request dispatcher followed by the sdp_response_dispatcher.
 A pointer to the buffer the channel id are passed to the dispatchers.

 * KNOWN ISSUES:
 
 If the message is invalid it will do nothing. The flows it seems to just
 show an error being sent from the master to the slave.

 *
 *
 ***********************************************************************/
void Avrcp_L2cap_Data_Ind_Cb(u_int16 cid, struct host_buf * p_buf)
{  
	u_int8* pData;
	u_int16 packet_size=0;

       pDebugPrintfEX((pLOGDEBUG, pLOGA2DP,"Avrcp_L2cap_Data_Ind_Cb p_buf->len=%d cid=0x%x\n",p_buf->len, cid));

#if pDEBUG        
       pDebugPrintfEX((pLOGDEBUG, pLOGA2DP,"Avrcp_L2cap_Data_Ind_Cb p_buf->len=%d cid=0x%x\n",p_buf->len, cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
       SYSDebug_PrintEx(pLOGDEBUG,pLOGA2DP, LOG_A2TP_AVRCP_CALLBACKS_INDEX, NULL, 0,0);
#endif
	
	pData = p_buf->data;
	packet_size = p_buf->len;

	Avrcp_Handle_Received_Data(pData, cid, packet_size);

	host_buf_free(p_buf);
}


/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_l2cap_disconnect_ind_cb
 * DESCRIPTION:
 
 This function will handle the disconnection indication callback. It was called twice, 
 firstly to disconnect the media connection, and secondly to disconnect the signal connection.

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
void Avrcp_L2cap_Disconnect_Ind_Cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
    t_avrcp_disconnect_ind msg;
#if pDEBUG
    pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"AVRCP - Disconnect Ind cid=0x%x, result=%d",pArgs->cid,pArgs->result));	
#endif

    msg.chnl_num = gAVRCP.chnl_num;
    msg.connect_id = gAVRCP.cid;

    rdabt_put_message(EXTERNAL_TASK_MASK,AVRCP_Q|AVRCP_DISCONNECT_IND,&msg);

    Reset_Avrcp_Values();
    Avrcp_Set_State(AVRCP_READY);
}


/***********************************************************************
 *
 * FUNCTION NAME:   Avrcp_L2cap_Disconnect_Cfm_Cb
 * DESCRIPTION:
 
 This function will handle the disconnection confirm callback.

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
void Avrcp_L2cap_Disconnect_Cfm_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs)
{
#ifdef COOLSAND
    t_avrcp_disconnect_cnf msg;
#endif

#if pDEBUG
    pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"AVRCP - Disconnect cfm, cid=0x%x, status=%d, result:%d", pArgs->cid, pArgs->status,pArgs->result));	
#endif

#ifdef COOLSAND
    msg.chnl_num = gAVRCP.chnl_num;
    msg.connect_id = gAVRCP.cid;
    msg.result = BT_NOERROR;///pArgs->result;
    //if(gSig_Cbs.avrcpdisconnectcfm)
    //    gSig_Cbs.avrcpdisconnectcfm(&msg);
    rdabt_put_message(EXTERNAL_TASK_MASK,AVRCP_Q|AVRCP_DISCONNECT_CNF,&msg);
#endif
    Reset_Avrcp_Values();
    Avrcp_Set_State(AVRCP_READY);
}

void Avrcp_L2cap_Qos_Violation_Ind_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs)
{
    pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"Avrcp_Qos_Violation_Ind_Cb"));
}


#if 0//def COOLSAND
void Avrcp_Set_Connect_Ind_Callback(void (*CallbackFunc)(void *pArgs))
{
       //gSig_Cbs.avrcpconnectind= CallbackFunc;
}

void Avrcp_Set_Connect_Cfm_Callback(u_int8 chnl_num, u_int8 local_role)
{
#if AVRCP_CT    
   // gSig_Cbs.avrcpconnectcfm= CallbackFunc;
    gAVRCP.chnl_num=chnl_num;
    gAVRCP.local_role = local_role;      
#endif 
}

void Avrcp_Set_Disconnect_Ind_Callback(void (*CallbackFunc)(void *pArgs))
{
       //gSig_Cbs.avrcpdisconnectind= CallbackFunc;
}

void Avrcp_Set_Disconnect_Cfm_Callback(void (*CallbackFunc)(void *pArgs))
{
       //gSig_Cbs.avrcpdisconnectcfm= CallbackFunc;
}

#endif

