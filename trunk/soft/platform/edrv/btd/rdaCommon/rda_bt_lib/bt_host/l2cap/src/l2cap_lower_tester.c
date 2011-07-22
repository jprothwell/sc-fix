/********************************************************************
 * MODULE NAME:    test_l2cap_ch
 * PROJECT CODE:   Test
 * DESCRIPTION:    Routines to test L2CAP functionality.
 * AUTHOR :        Gary Fleming
 * CREATION DATE:  21 June 1999
 *
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2002 Parthus Technologies.
 *     All rights reserved.
 *
 * SOURCE:         Bluetooth Specification V1.0
 * ISSUES:         <none>
 * NOTES TO USERS: <none>
 *
 *             
 ********************************************************************/

#include <stdio.h>  
#include <stdlib.h>
#include "l2_connect_sm.h"
#include "l2_const.h"
#include "l2cap_primitives.h"
#include "test_l2cap_ch.h"
#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"

#include "bd_addr_list.h"

#include "hci_primitives.h"
#include "l2_channel.h"
#include "l2_layer_config.h"
#include "l2cap_tester.h"
#include "l2_sar.h"
#include "l2_enc_dec.h"
#include "host_types.h"

#include <assert.h>


void L2_DataHandler(u_int16 cid, struct host_buf* p_buf);

void print_group_members(t_bdaddr *addrList, u_int16 addrListLen);

void L2_Event_Handler(u_int8 eventType, t_L2_ControlCallbackArgs *args);
int L2_Send_Data_Packet(u_int16 cid,u_int16 length,u_int8* p_data);

u_int8  data_field[50] = {1,2,3,4,5,6,7,8,9,10,
                          11,12,13,14,15,16,17,18,19,20,
                          21,22,23,24,25,26,27,28,29,30,
                          31,32,33,34,35,36,37,38,39,40,
                          41,42,43,44,45,46,47,48,49,50 };

u_int8  big_data_field[100] = {1,2,3,4,5,6,7,8,9,10,
                          11,12,13,14,15,16,17,18,19,20,
                          21,22,23,24,25,26,27,28,29,30,
                          31,32,33,34,35,36,37,38,39,40,
                          41,42,43,44,45,46,47,48,49,50,
                          51,52,53,54,55,56,57,58,59,60,
                          61,62,63,64,65,66,67,68,69,70,
                          71,72,73,74,75,76,77,78,79,80,
                          81,82,83,84,85,86,87,88,89,90,
                          91,92,93,94,95,96,97,98,99,100 };


static  u_int8 test_pass = 0x00;
static  u_int8 test_id = 0x0;
static  u_int8 channel_open;
static  u_int8 open_chn_on_config_conf = 0x0;

void L2_Event_Handler(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
    pDebugPrintf((pLOGDEBUG,"\n        *****************************************\n\n"));

    switch (eventType)
    {

    case L2CAP_evCONNECTCONF :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - L2CA Connect Confirm \n"));
        pDebugPrintf((pLOGDEBUG,"\n       Bd_Addr = %x,%x,%x,%x,%x,%x ",args->BDAddress.bytes[0],args->BDAddress.bytes[1],args->BDAddress.bytes[2],
                                                    args->BDAddress.bytes[3],args->BDAddress.bytes[4],args->BDAddress.bytes[5]));
        pDebugPrintf((pLOGDEBUG,"\n       LCID    = %x",args->cid));
        pDebugPrintf((pLOGDEBUG,"\n       Result  = %x",args->result));
        pDebugPrintf((pLOGDEBUG,"\n       Status  = %x",args->status));
        pDebugPrintf((pLOGDEBUG,"\n"));

        if (test_id == L2_COS_CED_BV_05)
        {
            if ((args->result == 0) && (args->status == 0))
                test_pass = 0x01;
        }
        break;

    case L2CAP_evCONNECTIND :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - L2CA Connect Ind \n"));
        pDebugPrintf((pLOGDEBUG,"\n       Bd_Addr = %x,%x,%x,%x,%x,%x ",args->BDAddress.bytes[0],args->BDAddress.bytes[1],args->BDAddress.bytes[2],
                                                    args->BDAddress.bytes[3],args->BDAddress.bytes[4],args->BDAddress.bytes[5]));
        pDebugPrintf((pLOGDEBUG,"\n       LCID    = %x",args->cid));
        pDebugPrintf((pLOGDEBUG,"\n       PSM     = %x",args->psm));
        pDebugPrintf((pLOGDEBUG,"\n       Ident   = %x",args->identifier));
        pDebugPrintf((pLOGDEBUG,"\n"));

        switch( test_id)
        {
        case L2_COS_CED_BV_01:
        case L2_COS_CED_BV_02:
        case L2_COS_CED_BV_03:
        case L2_COS_CED_BV_04:
        case L2_COS_CED_BV_06:
        case L2_COS_CED_BV_07:
        case L2_COS_CED_BV_08:
        case L2_COS_CFD_BV_01:
        case L2_COS_CFD_BV_02:
        case L2_COS_CFD_BV_03:
        case L2_COS_CFD_BV_04:
        case L2_COS_RCO_BV_01 :
        case L2_COS_RCO_BI_01 :
        
            L2_ConnectResponse(args->BDAddress,args->cid,0x00 /* Result */, 0x00 /* Status */);
            if (test_id == L2_COS_CED_BV_01)
                test_pass = 0x01;
            if ((test_id == L2_COS_CED_BV_06))
            {
                L2_Configure(args->cid,0x400,0xFFFF, 0xFFFF, (t_L2_ChannelFlowSpec *)0);
            }
            else if (test_id == L2_COS_CFD_BV_01)
            {
                /* Configure for C bit usage */
                L2_Configure(args->cid,0x400,0, 0, (t_L2_ChannelFlowSpec *)0);
            }
            break;
        case L2_COS_TMH_BV_02 :
            L2_ConnectResponse(args->BDAddress,args->cid,0x01 /* Pending Result */, 0x00 /* Status */);
            break;
        }
        break;

    case L2CAP_evCONFIGUREIND :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - L2CA Configure Ind \n"));
        pDebugPrintf((pLOGDEBUG,"\n       CID             = %x",args->cid));
        pDebugPrintf((pLOGDEBUG,"\n       OutMTU          = %x",args->mtu));
        pDebugPrintf((pLOGDEBUG,"\n       InFlushTO       = %x",args->flushTimeout));
        if (args->flow != 0)
        {
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Flags      = %x",args->flow->flags));
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Service    = %x",args->flow->serviceType));
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Tk Rate    = %x",args->flow->tokenRate));
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Bkt Size   = %x",args->flow->bucketSize));
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Bandwidth  = %x",args->flow->peakBandwidth));
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Latency    = %x",args->flow->latency));
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Variation  = %x",args->flow->delayVariation));
        }
        else
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Not Provided     "));

        switch(test_id)
        {
        case L2_COS_CED_BV_02:
        case L2_COS_CED_BV_03:
        case L2_COS_CED_BV_04:
        case L2_COS_CED_BV_07:
        case L2_COS_CED_BV_08:
        case L2_COS_CFD_BV_03:
            L2_ConfigureResponse(args->cid,0x400,0,(t_L2_ChannelFlowSpec *)0x0);
            L2_Configure(args->cid,0x400,0xFFFF,0xFFFF,0);
            break;

        case L2_COS_RCO_BV_01 :
        case L2_COS_RCO_BI_01 :
            L2_ConfigureResponse(args->cid,48,0,(t_L2_ChannelFlowSpec *)0x0);
            L2_Configure(args->cid,48,0xFFFF,0xFFFF,0);
            break;

        case L2_COS_CFD_BV_01:
            L2_ConfigureResponse(args->cid,0x400,0,(t_L2_ChannelFlowSpec *)0x0);
            L2_Set_C_Bit();
            L2_Configure(args->cid,0x400,0x0,0x0,0);
            break;

        case L2_COS_CFD_BV_04:
            if (channel_open == 0)
            {
                L2_ConfigureResponse(args->cid,0x400,0,(t_L2_ChannelFlowSpec *)0x0);
                L2_Configure(args->cid,0x400,0xFFFF,0xFFFF,0);
            }
            else
            {
                L2_ConfigureResponse(args->cid,0x500,0,0);
                test_pass = 0x01;
            }
            break;

        case L2_COS_CED_BV_06:
            L2_ConfigureResponse(args->cid,0x400,0,(t_L2_ChannelFlowSpec *)0x0);
            test_pass = 0x01;
            break;

        case L2_COS_CFD_BV_02:
            if (args->mtu > 0x800)
            {
                L2_ConfigureResponse(args->cid,0x600,L2CAP_CONFIG_FAILURE_UNACCEPTABLE_PARAMS,(t_L2_ChannelFlowSpec *)0x0);
            }
            else
            {
                L2_ConfigureResponse(args->cid,0x300,0,(t_L2_ChannelFlowSpec *)0x0);
                test_pass = 0x01;
            }
            break;
        }
        break;

    case L2CAP_evCONFIGURECONF :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - L2CA Configure Cfm \n"));
        pDebugPrintf((pLOGDEBUG,"\n       Result       = %x",args->result));
        pDebugPrintf((pLOGDEBUG,"\n       InMTU        = %x",args->mtu));
        pDebugPrintf((pLOGDEBUG,"\n       OutFlushTO   = %x",args->flushTimeout));
        if (args->flow != 0)
        {
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Flags      = %x",args->flow->flags));
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Service    = %x",args->flow->serviceType));
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Tk Rate    = %x",args->flow->tokenRate));
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Bkt Size   = %x",args->flow->bucketSize));
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Bandwidth  = %x",args->flow->peakBandwidth));
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Latency    = %x",args->flow->latency));
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Variation  = %x",args->flow->delayVariation));
        }
        else
            pDebugPrintf((pLOGDEBUG,"\n       QoS  Not Provided     "));

        switch(test_id)
        {
        case L2_COS_CED_BV_02 :
            if (args->result == 0x00)
                test_pass = 0x01;
            else
                test_pass = 0x02;
            break;

        case L2_COS_CED_BV_07 :
            L2_Disconnect(args->cid);
            break;

        case L2_COS_CFD_BV_03 :
            if (channel_open == 0)
            {
                channel_open = 0x01;
                L2_Configure(args->cid,0,0,0,0);
            }
            else
            {
                test_pass = 0x01;
            }
            break;
        case L2_COS_CFD_BV_04 :
            if (channel_open == 0)
            {
                channel_open = 0x01;
            }
            break;

        case L2_COS_CED_BV_03 :
            channel_open = 0x01;
            break;

        case L2_COS_RCO_BV_01 :
            channel_open = 0x01;
            break;

        case L2_COS_RCO_BI_01 :
            channel_open = 0x01;
            /* Send Data */
            /* DAVE THIS IS TO BE COMPLETED !! NEW CODE TO GO HERE */
            break;

        case L2_COS_CFD_BV_01:
            if (open_chn_on_config_conf)
            {
                /* Do Nothing : Wait for Data */
               ;
            }
            else
            {
                L2_Configure(args->cid,0x0,0xFFFF,0xFFFF,0);
                open_chn_on_config_conf = 0x01;
            }
            break;

        }
        break;

    case L2CAP_evDISCONNECTCONF :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - L2CA Disconnect Cfm \n"));
        pDebugPrintf((pLOGDEBUG,"\n       CID          = %x",args->cid));
        pDebugPrintf((pLOGDEBUG,"\n       RESULT       = %x" ,args->result));
        assert((args->result == 0x00) || (args->result == 0xEEEE));
        if (test_id == L2_COS_CED_BV_07)
        {
            test_pass = 0x01;
        }
        break;

    case L2CAP_evDISCONNECTIND :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - L2CA Disconnect Ind \n"));
        pDebugPrintf((pLOGDEBUG,"\n       CID          = %x",args->cid));
        if (( test_id == L2_COS_CED_BV_04) || ( test_id == L2_COS_CED_BV_08))
            test_pass = 0x01;

        break;
       
    case L2CAP_evTIMEOUTIND :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - L2CA Timeout Ind \n"));
        pDebugPrintf((pLOGDEBUG,"\n       CID          = %x",args->cid));
        pDebugPrintf((pLOGDEBUG,"\n      Timer        = %x",args->timerType));  
        break;

    case L2CAP_evDATAIND :

        break;
    default :
        break;
    }
}

void L2_DataHandler(u_int16 cid, struct host_buf* p_buf)
{
    int i;

    pDebugPrintf((pLOGDEBUG," Callback on  GroupDataCallback  -- CID = %x \n", cid));
    pDebugPrintf((pLOGDEBUG,"Data = "));
    for(i=0; i<p_buf->len; i++)
        pDebugPrintf((pLOGDEBUG," %x,",p_buf->data[i]));
    pDebugPrintf((pLOGDEBUG,"\n"));

    switch(test_id)
    {
    case L2_COS_RCO_BV_01 :
        L2_Send_Data_Packet(cid,48,data_field);
        test_pass = 0x01;
        break;

    case L2_COS_CED_BV_03 :
        L2_Send_Data_Packet(cid,100,big_data_field);
        test_pass = 0x01;
        break;

    case L2_COS_CFD_BV_01 :
        test_pass = 0x01;
        break;
    }

}

void L2_Ping_Callback(t_bdaddr BDA, u_int16 result, u_int8 *echoData, u_int16 length)
{
    int i;

    pDebugPrintf((pLOGDEBUG,"  Ping Callback \n"));
    pDebugPrintf((pLOGDEBUG,"  Bd_Addr = [%x:%x:%x:%x:%x:%x] \n",BDA.bytes[0],BDA.bytes[1],BDA.bytes[2],BDA.bytes[3],BDA.bytes[4],BDA.bytes[5]));
    pDebugPrintf((pLOGDEBUG,"  Length = %x \n", length));
    for (i=0;i<length;i++)
        pDebugPrintf((pLOGDEBUG," Ping_Data[%x] = %x \n",i,echoData[i]));
    if (test_id == L2_COS_ECH_BV_02)
        test_pass = 0x01;

}

void L2_GetInfo_Callback(t_bdaddr BDAddress,u_int16 result, u_int8* infoData, u_int16 info_len)
{
    int i;

    pDebugPrintf((pLOGDEBUG,"\n  Get Info Callback \n"));
    pDebugPrintf((pLOGDEBUG,"         Bd_Addr = [%x:%x:%x:%x:%x:%x]",BDAddress.bytes[0],BDAddress.bytes[1],BDAddress.bytes[2],BDAddress.bytes[3],BDAddress.bytes[4],BDAddress.bytes[5]));
    pDebugPrintf((pLOGDEBUG,"         Result =  %x \n", result));
    pDebugPrintf((pLOGDEBUG,"         Info_Len = %x \n",info_len));
    pDebugPrintf((pLOGDEBUG,"\n       Info Data = "));
    for (i=0; i < info_len; i++)
        pDebugPrintf((pLOGDEBUG,"%x,",infoData[i]));
    pDebugPrintf((pLOGDEBUG,"\n"));
}

void L2_GroupDataCallback(u_int16 cid, u_int8 *buf, u_int32 bufLen)
{
    u_int32 i;

    pDebugPrintf((pLOGDEBUG," Callback on  GroupDataCallback  -- CID = %x \n", cid));

    pDebugPrintf((pLOGDEBUG,"Data = "));
    for(i=0; i<bufLen; i++)
        pDebugPrintf((pLOGDEBUG," %x,",buf[i]));
    pDebugPrintf((pLOGDEBUG,"\n"));
}


void L2_GroupAddMemberCfm(u_int16 cid,  t_bdaddr BDA, u_int16 status)
{
    pDebugPrintf((pLOGDEBUG," Callback on GroupAddMember  :--  Cid = %x, Status = %x \n", cid, status));
    pDebugPrintf((pLOGDEBUG,"                                  BDA = %x:%x:%x:%x:%x:%x \n",BDA.bytes[0],BDA.bytes[1],BDA.bytes[2],BDA.bytes[3],BDA.bytes[4],BDA.bytes[5]));
}

void print_group_members(t_bdaddr *addrList, u_int16 addrListLen)
{
    int i,k;

    pDebugPrintf((pLOGDEBUG," L2CAP Group Members :- Num = %x \n", addrListLen));
    pDebugPrintf((pLOGDEBUG,"     BD ADDRESS List  \n"));
    for (i=0;i<addrListLen;i++)
    {
        pDebugPrintf((pLOGDEBUG," BD Address Num %x = ",i));
        for (k=0;k<6;k++)
            pDebugPrintf((pLOGDEBUG,"%x:",addrList[i].bytes[k]));
        pDebugPrintf((pLOGDEBUG,"\n"));
    }
}

void l2_init_lower_tester()
{
    l2_callbacks cbs;
    u_int16 target_psm = 0x21;

    cbs.connectionIndication = &L2_Event_Handler;
    cbs.configureIndication = &L2_Event_Handler;
    cbs.disconnectionIndication = &L2_Event_Handler;
    cbs.qosviolationIndication = &L2_Event_Handler;
    cbs.connectionConfirmation = &L2_Event_Handler;
    cbs.configureConfirmation = &L2_Event_Handler;
    cbs.disconnectionConfirmation = &L2_Event_Handler;
    cbs.dataIndication = &L2_DataHandler;
    cbs.timeoutIndication = &L2_Event_Handler;

    assert(BT_NOERROR == L2_RegisterPSM(target_psm, &cbs));
}

int l2_lower_tester(u_int8 test_case, t_bdaddr target_addr)
{
    u_int16 target_psm = 0x21;
    u_int16 echo_length = 0x20;
    u_int8 echo_data[0x20] = {0x0,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
                        0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};

    test_pass = 0x00;
    test_id = test_case;

    switch(test_case)
    {
    case L2_COS_CED_BV_01 :
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_01 passed"));
        break;

    case L2_COS_CED_BV_02 :
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_02 passed"));
        break;

    case L2_COS_CED_BV_03 :
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_03 passed"));
        break;

    case L2_COS_CED_BV_04 :
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_04 passed"));
        break;

    case L2_COS_CED_BV_05 :
        L2_Connect(target_addr, target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_05 passed"));
        break;

    case L2_COS_CED_BV_06 :
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_06 passed"));
        break;

    case L2_COS_CED_BV_07 :
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_07 passed"));
        break;

    
    case L2_COS_CED_BV_08 :
        L2_Dont_Send_Disconnect_Rsp();
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_08 passed"));
        break;
        
    case L2_COS_CED_BV_09 :
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_09 passed"));
        break;

    case L2_COS_CFD_BV_01 :
        open_chn_on_config_conf = 0x00;
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CFD_BV_01 passed"));
        break;

    case L2_COS_CFD_BV_02 :
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CFD_BV_02 passed"));
        break;

    case L2_COS_CFD_BV_03 :
        L2_Connect(target_addr, target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CFD_BV_03 passed"));
        break;

    case L2_COS_CFD_BV_04 :
        L2_Connect(target_addr, target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CFD_BV_04 passed"));
        break;

    case L2_COS_ECH_BV_01 :
        L2_Ping(target_addr,echo_data,echo_length,&L2_Ping_Callback);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_ECH_BV_01 passed"));
        break;

    case L2_COS_ECH_BV_02 :
        /* NO ACTIONS TO BE PERFORMED BY IUT_TESTER */
        test_pass = 0x01;
        break;

    case L2_COS_TMH_BV_01 :
        L2_Dont_Send_Echo_Response();
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_TMR_BV_01 passed"));
        break;

    case L2_COS_TMH_BV_02 :
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_TMR_BV_02 passed"));
        break;

    case L2_COS_RCO_BV_01 :
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_RCO_BV_01 passed"));
        while(test_pass == 0)
            ;
        break;

    case L2_COS_RCO_BI_01 :
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_RCO_BV_01 passed"));
        while(test_pass == 0)
            ;
        break;

    }
    return test_pass;
}

int L2_Send_Data_Packet(u_int16 cid,u_int16 length,u_int8* p_data)
{
    struct host_buf* buf;

    buf=host_buf_alloc(length+HEADERS_FOR_L2CAP);
    host_buf_reserve_header(buf,HEADERS_FOR_L2CAP);
    if (!buf)
        return 0;
    buf->len = length;
    pMemcpy(buf->data,p_data,length);
    prh_l2_sar_data_req(0,cid,buf);  

    return 1;
}
