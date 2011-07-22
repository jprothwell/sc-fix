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
/*#include "l2cap_primitives.h"*/
#include "bt.h"
#include "papi.h"
#include "l2_const.h"
#include "l2cap_tester.h"
#include <assert.h>


void L2_DataHandler(u_int16 cid, t_DataBuf *buffer);

void init_event(t_L2_ControlCallbackArgs *args);

void print_group_members(t_bdaddr *addrList, u_int16 addrListLen);

void L2_Event_Handler(u_int8 eventType, t_L2_ControlCallbackArgs *args);
void L2_Test_Send_Valid_Data(int cid, u_int8 *data, int lendata);
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
    int i;

    t_L2_ChannelFlowSpec outflow;

    outflow.flags = 0;
    outflow.latency = 0x100;
    outflow.serviceType = 0x02;
    outflow.delayVariation = 0x200;
    outflow.peakBandwidth = 0x4000;
    outflow.tokenRate = 0x8000;
    outflow.bucketSize = 0x3500;

    pDebugPrintf((pLOGDEBUG,"\n        *****************************************\n\n"));


    switch (eventType)
    {

    case L2CAP_evCONNECTCONF :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - L2CA Connect Confirm \n"));
        pDebugPrintf((pLOGDEBUG,"\n       Bd_Addr = %x,%x,%x,%x,%x,%x ",args->bdaddr.bytes[0],args->bdaddr.bytes[1],args->bdaddr.bytes[2],
                                                    args->bdaddr.bytes[3],args->bdaddr.bytes[4],args->bdaddr.bytes[5]));
        pDebugPrintf((pLOGDEBUG,"\n       LCID    = %x",args->cid));
        pDebugPrintf((pLOGDEBUG,"\n       Result  = %x",args->result));
        pDebugPrintf((pLOGDEBUG,"\n       Status  = %x",args->status));
        pDebugPrintf((pLOGDEBUG,"\n"));

        switch(test_id)
        {

        case L2_COS_CED_BV_01 :
            if ((args->result == 0x0) && (args->status == 0x0))
                test_pass = 0x01;
            else
                test_pass = 0x02;
            break;


        case L2_COS_CED_BV_02 :
        case L2_COS_CED_BV_03 :
        case L2_COS_CED_BV_04 :
        case L2_COS_CED_BV_07 :
        case L2_COS_CED_BV_08 :

        case L2_COS_CFD_BV_01 :
        case L2_COS_CFD_BV_03 :
        case L2_COS_CFD_BV_04 :

            L2_Configure(args->cid,0x400,0xFFFF, 0xFFFF, (t_L2_ChannelFlowSpec *)0);
            break;

        case L2_COS_RCO_BV_01 :
        case L2_COS_RCO_BI_01 :
            L2_Configure(args->cid,48,0xFFFF, 0xFFFF, (t_L2_ChannelFlowSpec *)0);
            break;

        case L2_COS_CFD_BV_02 :
            L2_Configure(args->cid,0x1000,0x3333, 0x4000, (t_L2_ChannelFlowSpec *)0);
            break;

        case L2_COS_TMH_BV_02 :
            if (args->result != 0x01)
                test_pass = 0x02;
            break;

        }
        break;

    case L2CAP_evCONNECTIND :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - L2CA Connect Ind \n"));
        pDebugPrintf((pLOGDEBUG,"\n       Bd_Addr = %x,%x,%x,%x,%x,%x ",args->bdaddr.bytes[0],args->bdaddr.bytes[1],args->bdaddr.bytes[2],
                                                    args->bdaddr.bytes[3],args->bdaddr.bytes[4],args->bdaddr.bytes[5]));
        pDebugPrintf((pLOGDEBUG,"\n       LCID    = %x",args->cid));
        pDebugPrintf((pLOGDEBUG,"\n       PSM     = %x",args->psm));
        pDebugPrintf((pLOGDEBUG,"\n       Ident   = %x",args->identifier));
        pDebugPrintf((pLOGDEBUG,"\n"));

        switch(test_id)
        {
        case L2_COS_CED_BV_05 :
            L2_ConnectResponse(args->bdaddr,args->cid,0x00 /* Result */, 0x00 /* Status */);
            test_pass = 0x01;
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
        case L2_COS_CED_BV_02 :
            L2_ConfigureResponse(args->cid,0x400,0,(t_L2_ChannelFlowSpec *)0x0);
            test_pass = 0x01;
            break;

        case L2_COS_CED_BV_03 :
            /* Write Data */
            L2_ConfigureResponse(args->cid,0x400,0,(t_L2_ChannelFlowSpec *)0x0);
            channel_open = 0x01;
            L2_Send_Data_Packet(args->cid,100,big_data_field);
            break;

        case L2_COS_CED_BV_04 :
            L2_ConfigureResponse(args->cid,0x400,0,(t_L2_ChannelFlowSpec *)0x0);
            for (i = 0; i < 0xFFFFFF; i++)
                ;
            L2_Disconnect(args->cid);
            break;

        case L2_COS_CED_BV_06 :
            L2_ConfigureResponse(args->cid,0x400,0,(t_L2_ChannelFlowSpec *)0x0);
            L2_Configure(args->cid,0x400,0xFFFF, 0xFFFF, (t_L2_ChannelFlowSpec *)0);
            break;

        case L2_COS_CED_BV_07 :
            L2_ConfigureResponse(args->cid,0x400,0,(t_L2_ChannelFlowSpec *)0x0);
            break;

        case L2_COS_CED_BV_08 :
            L2_ConfigureResponse(args->cid,0x400,0,(t_L2_ChannelFlowSpec *)0x0);
            L2_Disconnect(args->cid);
            break;

        case L2_COS_CFD_BV_03 :
            if (channel_open == 0)
            {
                L2_ConfigureResponse(args->cid,0x400,0,(t_L2_ChannelFlowSpec *)0x0);
                channel_open = 0x01;
            }
            else if (channel_open == 0x01)
            {
                L2_ConfigureResponse(args->cid,0x1000,0,(t_L2_ChannelFlowSpec *)&outflow);
                test_pass = 0x01;
            }
            break;

        case L2_COS_CFD_BV_04 :
            if (channel_open == 0)
            {
                L2_ConfigureResponse(args->cid,0x400,0,(t_L2_ChannelFlowSpec *)0x0);
                channel_open = 0x01;
                for (i=0;i<0xFFFF;i++)
                    ;
                L2_Configure(args->cid,0x0,0x0,0x0,0);
            }
            break;

        case L2_COS_CFD_BV_01 :
            if (open_chn_on_config_conf)
            {
                /* Send Config Resp & Send Data */
                L2_ConfigureResponse(args->cid,0,0x0,0);
                L2_Send_Data_Packet(args->cid,100,big_data_field);
                test_pass = 0x01;
            }
            else
            {
                L2_ConfigureResponse(args->cid,0x300,0x0,0);
                open_chn_on_config_conf = 0x01;
            }

            break;

        case L2_COS_RCO_BV_01 :

            L2_ConfigureResponse(args->cid,0x48,0,(t_L2_ChannelFlowSpec *)0x0);
            channel_open = 0x01;
            L2_Send_Data_Packet(args->cid,0x48,data_field);
            break;

        case L2_COS_RCO_BI_01 :
            L2_ConfigureResponse(args->cid,0x48,0,(t_L2_ChannelFlowSpec *)0x0);
            channel_open = 0x01;
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
        case L2_COS_CED_BV_06 :
            if (args->result == 0x00)
                test_pass = 0x01;
            else
                test_pass = 0x02;
            break;

        case L2_COS_CFD_BV_02 :
            if(args->result = 0x01 /*L2CAP_CONFIG_FAILURE_UNACCEPTABLE_PARAMS*/)
                L2_Configure(args->cid,args->mtu,args->flushTimeout, 0x4000,args->flow);
            else if (args->result == 0x0)
                test_pass = 0x01;
            else
                test_pass = 0x02;
            break;

        case L2_COS_CFD_BV_04 :
            if (channel_open == 0x01)
                test_pass = 0x01;
            break;

        case L2_COS_CFD_BV_01 :
            /* Waits for the peer to init configure */
            break;

        case L2_COS_RCO_BV_01 :
        case L2_COS_RCO_BI_01 :

            break;
        }
        break;

    case L2CAP_evDISCONNECTCONF :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - L2CA Disconnect Cfm \n"));
        pDebugPrintf((pLOGDEBUG,"\n       CID          = %x",args->cid));
        pDebugPrintf((pLOGDEBUG,"\n       RESULT       = %x" ,args->result));
        assert((args->result == 0x00) || (args->result == 0xEEEE));
        switch(test_id)
        {
        case L2_COS_CED_BV_04 :
            test_pass = 0x01;
            break;
        case L2_COS_CED_BV_08 :
            if (args->result == 0xEEEE)
                test_pass = 0x01;
            break;
        }
        break;

    case L2CAP_evDISCONNECTIND :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - L2CA Disconnect Ind \n"));
        pDebugPrintf((pLOGDEBUG,"\n       CID          = %x",args->cid));
        if ( test_id == L2_COS_CED_BV_07)
            test_pass = 0x01;

        break;
       
    case L2CAP_evTIMEOUT :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - L2CA Timeout Ind \n"));
        pDebugPrintf((pLOGDEBUG,"\n       CID          = %x",args->cid));
        pDebugPrintf((pLOGDEBUG,"\n      Timer        = %x",args->timerType));
        if (test_id == L2_COS_TMH_BV_02)
            test_pass = 01;
        break;

    case L2CAP_evQOSVIOLATIONIND :
        pDebugPrintf((pLOGDEBUG," Higer Layer Event - QoS Violation \n"));
        pDebugPrintf((pLOGDEBUG,"\n       CID          = %x",args->cid));
        pDebugPrintf((pLOGDEBUG,"\n QoS Violation OR Inconsistent Data contained in MTU "));

    default :
        break;
    }
    
}

void L2_DataHandler(u_int16 cid, t_DataBuf *p_buf)
{
    int i;

    pDebugPrintf((pLOGDEBUG," Callback on  Data  -- CID = %x \n", cid));
    pDebugPrintf((pLOGDEBUG,"Data = "));
    for(i=0; i< p_buf->dataLen; i++)
        pDebugPrintf((pLOGDEBUG," %x,", p_buf->data[i]));
    pDebugPrintf((pLOGDEBUG,"\n"));
    switch(test_id)
    {
    case L2_COS_CED_BV_03 :
    case L2_COS_RCO_BV_01 :
        test_pass = 0x01;
        break;
    }
}

void L2_Ping_Callback(t_bdaddr BDA, u_int8 result, u_int8 *echoData, u_int16 length)
{
    int i;

    pDebugPrintf((pLOGDEBUG,"  Ping Callback \n"));
    if (result == 0)
    {
        pDebugPrintf((pLOGDEBUG,"  Bd_Addr = [%x:%x:%x:%x:%x:%x] \n",BDA.bytes[0],BDA.bytes[1],BDA.bytes[2],BDA.bytes[3],BDA.bytes[4],BDA.bytes[5]));
        pDebugPrintf((pLOGDEBUG,"  Length = %x \n", length));
        pDebugPrintf((pLOGDEBUG,"  Result = %x \n", result));
        for (i=0;i<length;i++)
            pDebugPrintf((pLOGDEBUG," Ping_Data[%x] = %x \n",i,echoData[i]));
    }
    else
        pDebugPrintf((pLOGDEBUG,"  Result = %x \n", result));

    switch(test_id)
    {
    case L2_COS_ECH_BV_02:
        test_pass = 0x01;
        break;
    case L2_COS_TMH_BV_01:
        if (result == 0x01)
            test_pass = 0x01;
        break;
    }
}

void L2_GetInfo_Callback(t_bdaddr bdaddr,u_int16 result, u_int8* infoData, u_int16 info_len)
{
    int i;

    pDebugPrintf((pLOGDEBUG,"\n  Get Info Callback \n"));
    pDebugPrintf((pLOGDEBUG,"         Bd_Addr = [%x:%x:%x:%x:%x:%x]",bdaddr.bytes[0],bdaddr.bytes[1],bdaddr.bytes[2],bdaddr.bytes[3],bdaddr.bytes[4],bdaddr.bytes[5]));
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

void l2_init_iut_tester()
{
    t_L2_PsmCallbacks cbs;
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


int l2_iut_tester(u_int8 test_case, t_bdaddr target_addr)
{
    u_int16 target_psm = 0x21;
    u_int16 echo_length = 0x20;
    u_int8 echo_data[0x20] = {0x0,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
                              0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};
    
    channel_open = 0x00;
    test_pass = 0x00;
    test_id = test_case;

    switch(test_case)
    {
    case L2_COS_CED_BV_01 :
        L2_Connect(target_addr,target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_01 passed"));
        break;

    case L2_COS_CED_BV_02 :
        L2_Connect(target_addr,target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_02 passed"));
        break;

    case L2_COS_CED_BV_03 :
        L2_Connect(target_addr,target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_03 passed"));
        break;

    case L2_COS_CED_BV_04 :
        L2_Connect(target_addr, target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_04 passed"));
        break;

    case L2_COS_CED_BV_05 :

        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_05 passed"));
        break;

    case L2_COS_CED_BV_06 :
        L2_Connect(target_addr, target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_06 passed"));
        break;

    case L2_COS_CED_BV_07 :
        L2_Connect(target_addr, target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_07 passed"));
        break;

    
    case L2_COS_CED_BV_08 :
        L2_Connect(target_addr, target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_08 passed"));
        break;
        
    case L2_COS_CED_BV_09 :
        L2_Connect(target_addr, target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CED_BV_09 passed"));
        break;

    case L2_COS_CFD_BV_01 :
        L2_Connect(target_addr, target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_CFD_BV_01 passed"));
        break;

    case L2_COS_CFD_BV_02 :
        L2_Connect(target_addr, target_psm);
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

    case L2_COS_RCO_BV_01 :
        L2_Connect(target_addr, target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_RCO_BV_01 passed"));
        break;

    case L2_COS_RCO_BI_01 :
        L2_Connect(target_addr, target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_RCO_BI_01 passed"));
        break;

    case L2_COS_ECH_BV_01 :
        /* NO ACTIONS TO BE PERFORMED BY IUT_TESTER */
        test_pass = 0x01;

        break;

    case L2_COS_ECH_BV_02 :
        L2_Ping(target_addr,echo_data,echo_length,&L2_Ping_Callback);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_ECH_BV_01 passed"));
        break;

    case L2_COS_TMH_BV_01 :
        L2_Ping(target_addr,echo_data,echo_length,&L2_Ping_Callback);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_TMR_BV_01 passed"));
        break;

    case L2_COS_TMH_BV_02 :
        L2_Connect(target_addr, target_psm);
        while(test_pass == 0)
            ;
        pDebugPrintf((pLOGDEBUG,"\n Test L2_COS_TMR_BV_02 passed"));
        break;

    }
    return test_pass;
}



int L2_Send_Data_Packet(u_int16 cid,u_int16 length,u_int8* p_data)
{
    t_DataBuf* p_writeBuffer;

    L2_GetWriteBuffer(cid,length, &p_writeBuffer);

    if (p_writeBuffer == 0)
    {
        pDebugPrintf((pLOGERROR,"L2CAP Enc_Dec : Error - Memory could not be allocated for L2CAP Packet \n"));
        return 0; 
    }
    pMemcpy(p_writeBuffer->data,data_field,length);
    L2_Write(cid, p_writeBuffer);

    return 1;
}
