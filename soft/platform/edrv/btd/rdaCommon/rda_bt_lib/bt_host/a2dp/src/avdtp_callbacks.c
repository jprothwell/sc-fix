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

#include "a2dp.h"
#include "avdtp_core.h"
#include "avdtp.h"
#include "papi.h"
#include "host_msg.h"

AVDTP gAvdtp;   
#ifdef COOLSAND
//avdtp_signal_callbacks signal_cbs; 
#endif

static boolean gFirstConnectReq = TRUE; //Indicate whether the singal(1st connection) or meida connect(2nd connection)
static boolean gIsSRCFirstConnectReq = TRUE; //Indicat whether the SRC or SNK sending the connection firstly.

extern u_int32 rdabt_put_message(u_int16 q, u_int16 mi, void *mv);

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
void Avdtp_L2cap_Connect_Ind_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs)
{
    u_int16 response;
    u_int8 error_code;
    t_L2_ConfigReqParams req_params;

    req_params.inMTUmin=PRH_L2CAP_MTU_MIN;
    req_params.inMTUmax=900;
    req_params.outMTUmin=PRH_L2CAP_MTU_MIN;
    req_params.outMTUmax=900;
    req_params.flushTO=0xffff;
    req_params.linkTO=0;

    response = 0; /* accept connection */

#if pDEBUG
    pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"AVDTP - Connection ind gFirstConnectReq=%d, cid=0x%x",gFirstConnectReq,pArgs->cid)); 
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    SYSDebug_PrintEx(pLOGDEBUG,pLOGA2DP, LOG_A2TP_AVDTP_CALLBACKS_INDEX, NULL, 0,0);
#endif 

    if(gFirstConnectReq)
    {
        gIsSRCFirstConnectReq = FALSE;
        //gAvdtp.signal_cid = pArgs->cid;  //first connect req get the signal cid.
        pMemcpy(gAvdtp.bd_addr.bytes,pArgs->BDAddress.bytes, sizeof(gAvdtp.bd_addr.bytes));
    }
    //else
      //  gAvdtp.media_cid = pArgs->cid;   //second connect req get the media cid.

    //pMemcpy(gAvdtp.bd_addr.bytes,pArgs->BDAddress.bytes, sizeof(gAvdtp.bd_addr.bytes));
    error_code = L2_ConnectResponseandConfig(pArgs->BDAddress, pArgs->cid, response, 0, &req_params);
    if (error_code!=BT_NOERROR)
    {/* what can I do? */
    }
}

/***********************************************************************
 *
 * FUNCTION NAME:   Avdtp_L2cap_Connection_Cfm_Cb
 * DESCRIPTION:
 
 This function will handle the connection confirm callback.

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
void  Avdtp_L2cap_Connection_Cfm_Cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
#if pDEBUG
	pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"Avdtp_L2cap_Connection_Cfm_Cb result:%d,status:%d,cid=0x%x,gFirstConnectReq:%d",pArgs->result,pArgs->status,pArgs->cid,gFirstConnectReq));
      pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"gAvdtp.signal_cid=0x%x,gAvdtp.media_cid=0X%X",gAvdtp.signal_cid,gAvdtp.media_cid));
#endif
    
    if(pArgs->status!=BT_NOERROR)
        return;
    
    if(gFirstConnectReq)
    {
        gIsSRCFirstConnectReq = TRUE;
        gAvdtp.signal_cid = pArgs->cid;  //first connect req get the signal cid.
        pMemcpy(gAvdtp.bd_addr.bytes,pArgs->BDAddress.bytes, sizeof(gAvdtp.bd_addr.bytes));
        if(pArgs->result !=0)
            rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SIGNAL_CONNECT_CNF,(void*)pArgs);
    }
    else
    {
        gAvdtp.media_cid = pArgs->cid;   //second connect req get the media cid.
    }

}

/***********************************************************************
 *
 * FUNCTION NAME:   Avdtp_l2cap_config_ind_cb
 * DESCRIPTION:
 
 This function will handle the config indication callback.

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
void Avdtp_l2cap_config_ind_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
    u_int16 response;
    u_int8 error_code;

    response = 0;

#if pDEBUG
    pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"Avdtp - config ind cid=%x",pArgs->cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    SYSDebug_PrintEx(pLOGDEBUG,pLOGA2DP,  LOG_A2TP_AVDTP_CALLBACKS_INDEX, NULL, 0,0);
#endif
	
    error_code = L2_ConfigureResponse(pArgs->cid, 0,response,0x00);
    if(error_code==BT_NOERROR)
    {
         error_code=L2_Configure(pArgs->cid, 672, 65535,0,0);
    }

}

/***********************************************************************
 *
 * FUNCTION NAME:   Avdtp_L2cap_Configure_Cfm_Cb
 * DESCRIPTION:
 
 This function will handle the config confirm callback.

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
void Avdtp_L2cap_Configure_Cfm_Cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
#if pDEBUG
       pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"Avdtp Configure_Cfm_Cb:cid:%x,gFirstConnectReq:%d,gIsSRCFirstConnectReq:%d", pArgs->cid,gFirstConnectReq,gIsSRCFirstConnectReq));
#endif    
    if(gIsSRCFirstConnectReq)
     {
            if(gFirstConnectReq)
            {
#ifdef COOLSAND    
                pMemcpy(pArgs->BDAddress.bytes,gAvdtp.bd_addr.bytes,sizeof(gAvdtp.bd_addr.bytes));
               // if(signal_cbs.SingnalConnectionConf)
                //    signal_cbs.SingnalConnectionConf((void*)pArgs); 
                rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SIGNAL_CONNECT_CNF,(void*)pArgs);

#else
                Avdtp_Start_Discover(gAvdtp.signal_cid, 0);
#endif
                gFirstConnectReq= FALSE;
            }
            else
            {
                Avdtp_Start_Start();
                gFirstConnectReq = TRUE;
            }
    }
    else
    {
#ifdef COOLSAND    
        if(gFirstConnectReq)
        {
            t_a2dp_signal_connect_ind msg;

            gFirstConnectReq=FALSE;
            gAvdtp.signal_cid = pArgs->cid;  //first connect req get the signal cid.

            msg.connect_id =  gAvdtp.signal_cid;
            pMemcpy(msg.device_addr.bytes,gAvdtp.bd_addr.bytes, sizeof(gAvdtp.bd_addr.bytes));
            rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SIGNAL_CONNECT_IND,&msg);
        }
        else
        {
            gAvdtp.media_cid = pArgs->cid;   //second connect req get the media cid.
            gFirstConnectReq = TRUE;
        }
#endif
    }
		
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
void Avdtp_L2cap_Data_Ind_Cb(u_int16 cid, struct host_buf * p_buf)
{  
	u_int8* pData;
	u_int16 packet_size=0;
#if A2DP_SRC  //A2dp_SRC
	pData = p_buf->data;
	packet_size = p_buf->len;
    
#if pDEBUG
       //pDebugPrintfEX((pLOGDEBUG, pLOGA2DP,"Avdtp_L2cap_Data_Ind_Cb gAvdtp.signal_cid:0x%x, cid:0x%x",gAvdtp.signal_cid,cid));
#endif

	if(gAvdtp.signal_cid == cid)  //signal
	{
		Avdtp_Request_Dispatcher(pData, cid, packet_size, AVDTP_SIGNAL,gAvdtp.bd_addr);
	}

#else //A2dp_SNK

#if pDEBUG
       pDebugPrintfEX((pLOGDEBUG, pLOGA2DP,"AVDTP Received data from peer"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
       SYSDebug_PrintEx(pLOGDEBUG,pLOGA2DP, (LOG_A2TP_AVDTP_CALLBACKS_INDEX+1), NULL, 0,0);
#endif

	pData = p_buf->data;
	packet_size = p_buf->len;

	if(gAvdtp.signal_cid == cid)  //signal
		Avdtp_Request_Dispatcher(pData, cid, packet_size, AVDTP_SIGNAL,gAvdtp.bd_addr);
	else if(gAvdtp.media_cid == cid)   //media
	{
#if COMBINED_HOST==1
		host_buf_CopyToA2dpBuffer(p_buf);
#endif
	}
    
#endif
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
void Avdtp_L2cap_Disconnect_Ind_Cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
    u_int16 cid=pArgs->cid;
#if pDEBUG
    pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"AVDTP - Disconnect ind"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    SYSDebug_PrintEx(pLOGDEBUG,pLOGA2DP,  (LOG_A2TP_AVDTP_CALLBACKS_INDEX+2), NULL, 0,0);
#endif

#ifdef COOLSAND
    if(cid ==gAvdtp.signal_cid ) //signal disconection
    {
        //if(signal_cbs.SingnalDisconnectionInd)
         //   signal_cbs.SingnalDisconnectionInd(&cid);
        rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SIGNAL_DISCONNECT_IND,&cid);
         
        gAvdtp.signal_cid = 0;
    }
    else if(cid ==gAvdtp.media_cid ) //media disconnection
    {
        
    }
#endif
    
    gAvdtp.media_cid  = 0;
    gAvdtp.active_seid = 0;
    gFirstConnectReq=TRUE;
    gIsSRCFirstConnectReq= TRUE;
}


/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_l2cap_disconnect_cfm_cb
 * DESCRIPTION:
 
 This function will handle the disconnection indication callback.

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
 u_int8 Close_resp[4];
void Avdtp_L2cap_Disconnect_Cfm_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs)
{
#if pDEBUG
    pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"AVDTP - Disconnect cfm pArgs->cid=0x%x", pArgs->cid));	
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    SYSDebug_PrintEx(pLOGDEBUG,pLOGA2DP,  (LOG_A2TP_AVDTP_CALLBACKS_INDEX+3), NULL, 0,0);
#endif

#ifdef COOLSAND
    if(pArgs->cid == gAvdtp.signal_cid )  //signal disconnection
    { 
        rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SIGNAL_DISCONN_CNF,(void*)pArgs);
        gAvdtp.signal_cid = 0;
        gAvdtp.active_seid = 0;
        gFirstConnectReq=TRUE;
        gIsSRCFirstConnectReq= TRUE;
    }
    else if(pArgs->cid == gAvdtp.media_cid )  //media disconnection
    {
        gAvdtp.media_cid=0;
        gFirstConnectReq = FALSE;
        rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_CLOSE_CNF,Close_resp);
    }
#endif

}

void Avdtp_L2cap_Qos_Violation_Ind_Cb(u_int8 type, t_L2_ControlCallbackArgs *pArgs)
{
    pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"Avdtp_Qos_Violation_Ind_Cb"));
}

u_int8 Avdtp_Is_First_Conection(void)
{
    return gFirstConnectReq;
}

void Avdtp_Set_First_Conection(u_int8 flag)
{
    gFirstConnectReq = flag;
}

void Avdtp_Set_SRC_First_Conection_Flag(u_int8 flag)
{
    gIsSRCFirstConnectReq = flag;
}

