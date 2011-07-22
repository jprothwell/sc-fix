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
#include "l2_sar.h"
#include "l2cap_primitives.h"
#include "test_l2cap_ch.h"
#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"
#include "mgr_core.h"
#include "bd_addr_list.h"

#include "hci_primitives.h"
#include "l2_channel.h"

/*#include "test_utils.h"*/
#include <assert.h>

void L2_DataHandler(u_int16 cid, struct host_buf* p_buf);
void Setup_L2CAP_Out_Channel(t_bdaddr target_bdaddr, u_int16 target_PSM  );
void Setup_L2CAP_Out_Channel_With_QoS(t_bdaddr target_bdaddr, u_int16 target_PSM );
void Setup_L2CAP_In_Channel(t_bdaddr target_bdaddr, u_int16 psm);
void Loc_Disconnect_L2CAP_Out_Channel(u_int16 release_cid);
void Rem_Disconnect_L2CAP_In_Channel(u_int16 release_cid);
void Out_Reconfig_L2CAP_Channel_MTU(u_int16 loc_cid, u_int16 newMTU);
void In_Reconfig_L2CAP_Channel_MTU(u_int16 loc_cid, u_int16 newMTU);
void Reconfig_x2_L2CAP_Channel_MTU(u_int16 loc_cid, u_int16 newMTU);
u_int8 Test_L2CAP_Groups(void);
u_int8 Test_L2CAP_Service_Interface_Invalid_Params(void);
void init_event(t_L2_ControlCallbackArgs *args);
u_int8 Test_L2CAP_QOS(void);

void print_group_members(t_bdaddr *addrList, u_int16 addrListLen);

void L2_Event_Handler(u_int8 eventType, t_L2_ControlCallbackArgs *args);


#define mSet_Uint16(p_pdu,val) { { p_pdu[0] = val & 0xFF; p_pdu[1] = val >> 8; } }
#define mSet_Uint32(p_pdu,val) { { p_pdu[0] = (u_int8)val; p_pdu[1] = (u_int8)(val >> 8); p_pdu[2] = (u_int8)(val >> 16); p_pdu[3] = (u_int8)(val >> 24);} } 
#define mSet_Identifier(p_pdu,identifier) { *p_pdu = identifier; }
#define mSet_Uint8(p_pdu,val) { *p_pdu = val; }
#define mGet_Uint16(p_pdu, val){ val = ((u_int16)(p_pdu[1] << 8)) | p_pdu[0]; }
#define mGet_Uint8(p_pdu, val){ val = *p_pdu; }

extern int     g_l2cap_acl_size;
static t_bdaddr master_bd_addr = {0x01,0x02,0x03,0x04,0x05,0x06};
static t_bdaddr slave_bd_addr = {0x11,0x12,0x13,0x14,0x15,0x16};
static t_bdaddr bd_addr1 = {0x11,0xa1,0x11,0xa1,0x11,0xa1};
static t_bdaddr bd_addr2 = {0x22,0xa2,0x22,0xa2,0x22,0xa2};
static t_bdaddr bd_addr3 = {0x33,0xa3,0x33,0xa3,0x33,0xa3};
static t_bdaddr bd_addr4 = {0x44,0xa4,0x44,0xa4,0x44,0xa4};

static u_int16 handle = 0x13;
static u_int16 rem_cid = 0x94;
static u_int16 loc_cid = 0x40;
static u_int8 loc_tid = 0x45;
static u_int8 rem_tid = 0x84;
static t_L2_ControlCallbackArgs upper_layer_event;

u_int8  data_field[20] = {0x01,0x02,0x03,0x04,0x05,
                          0x11,0x12,0x13,0x14,0x15,
                          0x01,0x02,0x03,0x04,0x05,
                          0x11,0x12,0x13,0x14,0x15};

tid_t current_Tid;

#include "host_types.h"

#if 1
t_api prh_mgr_releaseACL(u_int16 handle, u_int8 reason)
{

     return BT_PENDING;

}

t_api prh_mgr_createACL(t_bdaddr target, u_int16 psm, u_int16 *handle, void (*callbackFunc)(tid_t transactionId, t_api status), tid_t transactionId)
{
  static u_int16 cur_handle = 0x13;

  *handle = cur_handle;
   cur_handle++;

    current_Tid = transactionId;
   return BT_PENDING;

}

#endif

void Create_L2CAP_Connect_Rsp_PDU(u_int16 handle, u_int8 remote_Tid, u_int16 remote_cid, u_int16 local_cid,
                                        u_int16 result,
                                        u_int16 status)
{
    u_int8* p_pdu;
    struct host_buf *p_buf;
    unsigned int i;

    p_buf = host_buf_alloc(L2CAP_CONNECTION_RSP_PARAM_LENGTH+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH);
    p_buf->len = L2CAP_CONNECTION_RSP_PARAM_LENGTH+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH;  
    p_pdu = p_buf->data;

    if (p_pdu) 
    {
        mSet_Uint16(p_pdu, L2CAP_CONNECTION_RSP_PARAM_LENGTH+L2CAP_SIG_HEADER_LENGTH);
        mSet_Uint16((p_pdu+2),0x0001); /* Signalling Cid */
        mSet_Uint8((p_pdu+4),L2CAP_CONNECTION_RESPONSE);
        mSet_Uint8((p_pdu+5),remote_Tid);
        mSet_Uint16((p_pdu+6),L2CAP_CONNECTION_RSP_PARAM_LENGTH);
        if (result == 0) /* Only include SCID if result = 0 */
        {
            mSet_Uint16((p_pdu+8),remote_cid);  
        }
        else
        {
            mSet_Uint16((p_pdu+8),0x33);  
        }

        mSet_Uint16((p_pdu+10),local_cid);
        mSet_Uint16((p_pdu+12),result);
        mSet_Uint16((p_pdu+14),status);      
    }
    printf(" Recieved ACL on Handle %x  ",handle);
    for (i = 0; i < (p_buf->len); i++)
        printf("%#x,",p_buf->data[i]);

    printf("\n");
    prh_l2_sar_data_ind(handle, p_buf, HCI_ACL_START_FRAGMENT);
}

void Create_L2CAP_Connect_Req_PDU(u_int16 handle,u_int8 remote_Tid, u_int16 psm, u_int16 remote_cid)
{
    u_int8* p_pdu;
    struct host_buf *p_buf;
    u_int8 pdu_len;
    unsigned int i;

    pdu_len = 0x04;

    p_buf = host_buf_alloc(pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH);
    p_buf->len = pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH;  
    p_pdu = p_buf->data;

    if (p_pdu) 
    {
        mSet_Uint16(p_pdu, (pdu_len+L2CAP_SIG_HEADER_LENGTH));
        mSet_Uint16((p_pdu+2),0x0001); /* Signalling Cid */
        mSet_Uint8((p_pdu+4),L2CAP_CONNECTION_REQUEST);
        mSet_Uint8((p_pdu+5),remote_Tid);
        mSet_Uint16((p_pdu+6),pdu_len);
        mSet_Uint16((p_pdu+8),psm);  
    mSet_Uint16((p_pdu+10),remote_cid);
    }
    printf(" Recieved ACL on Handle %x  ",handle);
    for (i = 0; i < (p_buf->len); i++)
        printf("%#x,",p_buf->data[i]);

    printf("\n");
    prh_l2_sar_data_ind(handle, p_buf, HCI_ACL_START_FRAGMENT);
}

void Create_L2CAP_Config_Rsp_PDU(u_int16 handle, u_int32 remote_Tid,u_int16 remote_cid, u_int16 flags,
                                 u_int16 result, u_int16 inMtu, u_int16 flushTO,t_L2_ChannelFlowSpec* flow)
{
    u_int8* p_pdu;
    struct host_buf *p_buf;
    unsigned int i;
    u_int8 pdu_len;

    pdu_len = 0x06;
    if (inMtu != 0)
      pdu_len += 4;
    if (flushTO != 0)
      pdu_len += 4;
    if (flow != 0)
      pdu_len += 24;    

    p_buf = host_buf_alloc(pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH);
    p_buf->len = pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH;  
    p_pdu = p_buf->data;

    /* write the PDU into the memory of the Queue and Advance the state*/

    if (p_pdu) 
    {
        mSet_Uint16(p_pdu, (pdu_len+L2CAP_SIG_HEADER_LENGTH));
        mSet_Uint16((p_pdu+2),0x0001); /* Signalling Cid */
        mSet_Uint8((p_pdu+4),L2CAP_CONFIGURE_RESPONSE);
        mSet_Uint8((p_pdu+5),remote_Tid);
        mSet_Uint16((p_pdu+6),pdu_len);
        mSet_Uint16((p_pdu+8),remote_cid);  
        mSet_Uint16((p_pdu+10),flags);
        mSet_Uint16((p_pdu+12),result);
        p_pdu+= 14;
        if(inMtu != 0)
        {
           mSet_Uint8(p_pdu,01);
           mSet_Uint8((p_pdu+1),02);
           mSet_Uint16((p_pdu+2),inMtu);
           p_pdu+=4;
        }
        if (flushTO != 0)
        {
           mSet_Uint8(p_pdu,02);
           mSet_Uint8((p_pdu+1),02);
           mSet_Uint16((p_pdu+2),flushTO);
           p_pdu+=4;
        }
        if (flow != 0)
        {
           mSet_Uint8(p_pdu,03);
           mSet_Uint8((p_pdu+1),22);
           mSet_Uint8((p_pdu+2),flow->flags);
           mSet_Uint8((p_pdu+3),flow->serviceType);
           mSet_Uint32((p_pdu+4),flow->tokenRate);
           mSet_Uint32((p_pdu+8),flow->bucketSize);
           mSet_Uint32((p_pdu+12),flow->peakBandwidth);
           mSet_Uint32((p_pdu+16),flow->latency);
           mSet_Uint32((p_pdu+20),flow->delayVariation);
           p_pdu += 24;
        }
    }
    printf(" Recieved ACL on Handle %x  ",handle);
    for (i = 0; i < (p_buf->len); i++)
        printf("%#x,",p_buf->data[i]);

    printf("\n");
    prh_l2_sar_data_ind(handle, p_buf, HCI_ACL_START_FRAGMENT);
}

    /* NOW TRY AN INCOMING L2CAP CONFIG REQ */

void Create_L2CAP_Config_Req_PDU(u_int16 handle,u_int32 remote_Tid,u_int16 remote_cid, u_int16 flags, 
                                 u_int16 inMtu, u_int16 flushTO, t_L2_ChannelFlowSpec* flow)                                 
{
    u_int8* p_pdu;
    struct host_buf *p_buf;
    u_int8 pdu_len;
    unsigned int i;

    pdu_len = 0x04;
    if (inMtu != 0)
      pdu_len += 4;
    if (flushTO != 0)
      pdu_len += 4;
    if (flow != 0)
      pdu_len += 24;

    p_buf = host_buf_alloc(pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH);
    p_buf->len = pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH;  
    p_pdu = p_buf->data;

    /* write the PDU into the memory of the Queue and Advance the state*/

    if (p_pdu) 
    {
        mSet_Uint16(p_pdu, (pdu_len+L2CAP_SIG_HEADER_LENGTH));
        mSet_Uint16((p_pdu+2),0x0001); /* Signalling Cid */
        mSet_Uint8((p_pdu+4),L2CAP_CONFIGURE_REQUEST);
        mSet_Uint8((p_pdu+5),remote_Tid);
        mSet_Uint16((p_pdu+6),pdu_len);
        mSet_Uint16((p_pdu+8),remote_cid);  
        mSet_Uint16((p_pdu+10),flags);
        p_pdu+= 12;
        if(inMtu != 0)
        {
           mSet_Uint8(p_pdu,01);
           mSet_Uint8((p_pdu+1),02);
           mSet_Uint16((p_pdu+2),inMtu);
           p_pdu+=4;
        }
        if (flushTO != 0)
        {
           mSet_Uint8(p_pdu,02);
           mSet_Uint8((p_pdu+1),02);
           mSet_Uint16((p_pdu+2),flushTO);
           p_pdu+=4;
        }
        if (flow != 0)
        {
           mSet_Uint8(p_pdu,03);
           mSet_Uint8((p_pdu+1),22);
           mSet_Uint8((p_pdu+2),flow->flags);
           mSet_Uint8((p_pdu+3),flow->serviceType);
           mSet_Uint32((p_pdu+4),flow->tokenRate);
           mSet_Uint32((p_pdu+8),flow->bucketSize);
           mSet_Uint32((p_pdu+12),flow->peakBandwidth);
           mSet_Uint32((p_pdu+16),flow->latency);
           mSet_Uint32((p_pdu+20),flow->delayVariation);
           p_pdu += 24;
        }
    }
    printf(" Recieved ACL on Handle %x  ",handle);
    for (i = 0; i < (p_buf->len); i++)
        printf("%#x,",p_buf->data[i]);

    printf("\n");
    prh_l2_sar_data_ind(handle, p_buf, HCI_ACL_START_FRAGMENT);
}



void Create_L2CAP_Disconnect_Req_PDU(u_int16 handle,u_int32 remote_Tid,u_int16 remote_cid, u_int16 local_cid)
{
    u_int8* p_pdu;
    struct host_buf *p_buf;
    u_int8 pdu_len;
    unsigned int i;

#if 0
    printf("\n Expect - Incoming L2CAP_Disconnect_Request on Handle %x\n", handle);
    printf("       Len     = 0x0008 \n"); 
    printf("       CID     = 0x0001 \n");
    printf("       Code    =   0x07 \n");
    printf("       Id      =   0x%x \n", remote_Tid);
    printf("       Len     = 0x0004 \n");
    printf("       DCID    =   0x%x \n", remote_cid);
    printf("       SCID    =   0x%x \n", local_cid);
#endif

    pdu_len = 0x04;
    p_buf = host_buf_alloc(pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH);
    p_buf->len = pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH;  
    p_pdu = p_buf->data;

    /* write the PDU into the memory of the Queue and Advance the state*/

    if (p_pdu) 
    {
        mSet_Uint16(p_pdu, (pdu_len+L2CAP_SIG_HEADER_LENGTH));
        mSet_Uint16((p_pdu+2),0x0001); /* Signalling Cid */
        mSet_Uint8((p_pdu+4),L2CAP_DISCONNECTION_REQUEST);
        mSet_Uint8((p_pdu+5),remote_Tid);
        mSet_Uint16((p_pdu+6),pdu_len);
        mSet_Uint16((p_pdu+8),remote_cid);  
    mSet_Uint16((p_pdu+10),local_cid);
    }
    printf(" Recieved ACL on Handle %x  ",handle);
    for (i = 0; i < (p_buf->len); i++)
        printf("%#x,",p_buf->data[i]);

    printf("\n");
    prh_l2_sar_data_ind(handle, p_buf, HCI_ACL_START_FRAGMENT);
}


void Create_L2CAP_Disconnect_Rsp_PDU(u_int16 handle,u_int32 remote_Tid,u_int16 remote_cid, u_int16 local_cid)
{
    u_int8* p_pdu;
    struct host_buf *p_buf;
    u_int8 pdu_len;
    unsigned int i;

#if 0
    printf("\n Expect - Incoming L2CAP_Disconnect_Response on Handle %x\n", handle);
    printf("       Len     = 0x0008 \n"); 
    printf("       CID     = 0x0001 \n");
    printf("       Code    =   0x07 \n");
    printf("       Id      =   0x%x \n", remote_Tid);
    printf("       Len     = 0x0004 \n");
    printf("       DCID    =   0x%x \n", remote_cid);
    printf("       SCID    =   0x%x \n", local_cid);
#endif
    pdu_len = 0x04;
    p_buf = host_buf_alloc(pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH);
    p_buf->len = pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH;  
    p_pdu = p_buf->data;

    /* write the PDU into the memory of the Queue and Advance the state*/

    if (p_pdu) 
    {
        mSet_Uint16(p_pdu, (pdu_len+L2CAP_SIG_HEADER_LENGTH));
        mSet_Uint16((p_pdu+2),0x0001); /* Signalling Cid */
        mSet_Uint8((p_pdu+4),L2CAP_DISCONNECTION_RESPONSE);
        mSet_Uint8((p_pdu+5),remote_Tid);
        mSet_Uint16((p_pdu+6),pdu_len);
        mSet_Uint16((p_pdu+8),remote_cid);  
    mSet_Uint16((p_pdu+10),local_cid);
    }
    printf(" Recieved ACL on Handle %x  ",handle);
    for (i = 0; i < (p_buf->len); i++)
        printf("%#x,",p_buf->data[i]);

    printf("\n");
    prh_l2_sar_data_ind(handle, p_buf, HCI_ACL_START_FRAGMENT);
}


void Create_L2CAP_Echo_Rsp_PDU(u_int16 handle,u_int32 remote_Tid,u_int16 length, u_int8* p_data)
{
    u_int8* p_pdu;
    struct host_buf *p_buf;
    u_int16 pdu_len;
    unsigned int i;

    printf("\n Expect - Incoming L2CAP_Echo_Response on Handle %x\n", handle);
    printf("       Len     =   0x%x \n",length+0x04); 
    printf("       CID     = 0x0001 \n");
    printf("       Code    =   0x09 \n");
    printf("       Id      =   0x%x \n", remote_Tid);
    printf("       Len     =   0x%x \n", length);

    if (length != 0)
    {
        printf(" Data Field :- \n");
        for (i=0; i < length; i++)
            printf("%x,",p_data[i]);        
    }
    printf("\n\n");


    pdu_len = length;
    p_buf = host_buf_alloc(pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH);
    p_buf->len = pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH;  
    p_pdu = p_buf->data;

    /* write the PDU into the memory of the Queue and Advance the state*/

    if (p_pdu) 
    {
        mSet_Uint16(p_pdu, (pdu_len+L2CAP_SIG_HEADER_LENGTH));
        mSet_Uint16((p_pdu+2),0x0001); /* Signalling Cid */
        mSet_Uint8((p_pdu+4),L2CAP_ECHO_RESPONSE);
        mSet_Uint8((p_pdu+5),remote_Tid);
        mSet_Uint16((p_pdu+6),pdu_len);
        p_pdu+=8;

        if (length != 0)
        {
            for (i=0; i < length; i++)
                mSet_Uint8((p_pdu+i),p_data[i]);        
        }
    }
    printf(" Recieved ACL on Handle %x  ",handle);
    for (i = 0; i < (p_buf->len); i++)
        printf("%#x,",p_buf->data[i]);

    printf("\n");
    prh_l2_sar_data_ind(handle, p_buf, HCI_ACL_START_FRAGMENT);
}

void Create_L2CAP_Echo_Req_PDU(u_int16 handle,u_int32 remote_Tid,u_int16 length, u_int8* p_data)
{
    u_int8* p_pdu;
    struct host_buf *p_buf;
    u_int16 pdu_len;
    unsigned int i;

    printf("\n Expect - Incoming L2CAP_Echo_Request on Handle %x\n", handle);
    printf("       Len     =   0x%x \n",length+0x04); 
    printf("       CID     = 0x0001 \n");
    printf("       Code    =   0x08 \n");
    printf("       Id      =   0x%x \n", remote_Tid);
    printf("       Len     =   0x%x \n", length);

    if (length != 0)
    {
        printf(" Data Field :- \n");
        for (i=0; i < length; i++)
            printf("%x,",p_data[i]);        
    }
    printf("\n\n");


    pdu_len = length;
    p_buf = host_buf_alloc(pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH);
    p_buf->len = pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH;  
    p_pdu = p_buf->data;

    /* write the PDU into the memory of the Queue and Advance the state*/

    if (p_pdu) 
    {
        mSet_Uint16(p_pdu, (pdu_len+L2CAP_SIG_HEADER_LENGTH));
        mSet_Uint16((p_pdu+2),0x0001); /* Signalling Cid */
        mSet_Uint8((p_pdu+4),L2CAP_ECHO_REQUEST);
        mSet_Uint8((p_pdu+5),remote_Tid);
        mSet_Uint16((p_pdu+6),pdu_len);
        p_pdu+=8;

        if (length != 0)
        {
            for (i=0; i < length; i++)
                mSet_Uint8((p_pdu+i),p_data[i]);        
        }
    }
    printf(" Recieved ACL on Handle %x  ",handle);
    for (i = 0; i < (p_buf->len); i++)
        printf("%#x,",p_buf->data[i]);

    printf("\n");
    prh_l2_sar_data_ind(handle, p_buf, HCI_ACL_START_FRAGMENT);
}


void Create_L2CAP_Information_Rsp_PDU(u_int16 handle,u_int32 remote_Tid, u_int16 length, u_int16 infoType, u_int16 result, u_int8* p_data)
{
    u_int8* p_pdu;
    struct host_buf *p_buf;
    u_int16 pdu_len;
    u_int32 i;

    printf("\n Expect - Incoming L2CAP_Information_Response on Handle %x\n", handle);
    printf("       Len     =   0x%x \n",length+0x08); 
    printf("       CID     = 0x0001 \n");
    printf("       Code    =   0x0B \n");
    printf("       Id      =   0x%x \n", remote_Tid);
    printf("       Len     =   0x%x \n", length);
    printf("       infoType=   0x%x \n", infoType);
    printf("       result  =   0x%x \n", result);
    
    if (length > 4)
    {
        printf(" Data Field :- \n");
        for (i=0; i < (length-4); i++)
            printf("%x,",p_data[i]);        
    }
    printf("\n\n");


    pdu_len = length;
    p_buf = host_buf_alloc(pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH);
    p_buf->len = pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH;  
    p_pdu = p_buf->data;

    /* write the PDU into the memory of the Queue and Advance the state*/

    if (p_pdu) 
    {
        mSet_Uint16(p_pdu, (pdu_len+L2CAP_SIG_HEADER_LENGTH));
        mSet_Uint16((p_pdu+2),0x0001); /* Signalling Cid */
        mSet_Uint8((p_pdu+4),L2CAP_INFORMATION_RESPONSE);
        mSet_Uint8((p_pdu+5),remote_Tid);
        mSet_Uint16((p_pdu+6),pdu_len);
        mSet_Uint16((p_pdu+8),infoType);
        mSet_Uint16((p_pdu+10),result);
        p_pdu+=12;

        if (length >4)
        {
            for (i=0; i < (length-4); i++)
                mSet_Uint8((p_pdu+i),p_data[i]);        
        }
    }
    printf(" Recieved ACL on Handle %x  ",handle);
    for (i = 0; i < (p_buf->len); i++)
        printf("%#x,",p_buf->data[i]);

    printf("\n");
    prh_l2_sar_data_ind(handle, p_buf, HCI_ACL_START_FRAGMENT);
}

void Create_L2CAP_Information_Req_PDU(u_int16 handle,u_int32 remote_Tid,u_int16 length, u_int16 infoType)
{
    u_int8* p_pdu;
    struct host_buf *p_buf;
    u_int16 pdu_len;
    unsigned int i;

    printf("\n Expect - Incoming L2CAP_Information_Request on Handle %x\n", handle);
    printf("       Len     =   0x%x \n",length+0x06); 
    printf("       CID     = 0x0001 \n");
    printf("       Code    =   0x0A \n");
    printf("       Id      =   0x%x \n", remote_Tid);
    printf("       Len     =   0x%x \n", length);
    printf("       infoType=   0x%x \n", infoType);

    pdu_len = length;
    p_buf = host_buf_alloc(pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH);
    p_buf->len = pdu_len+L2CAP_SIG_HEADER_LENGTH+L2CAP_PDU_HEADER_LENGTH;  
    p_pdu = p_buf->data;

    /* write the PDU into the memory of the Queue and Advance the state*/

    if (p_pdu) 
    {
        mSet_Uint16(p_pdu, (pdu_len+L2CAP_SIG_HEADER_LENGTH));
        mSet_Uint16((p_pdu+2),0x0001); /* Signalling Cid */
        mSet_Uint8((p_pdu+4),L2CAP_INFORMATION_REQUEST);
        mSet_Uint8((p_pdu+5),remote_Tid);
        mSet_Uint16((p_pdu+6),pdu_len);
        mSet_Uint16((p_pdu+8),infoType);
    }
    printf(" Recieved ACL on Handle %x  ",handle);
    for (i = 0; i < (p_buf->len); i++)
        printf("%#x,",p_buf->data[i]);

    printf("\n");
    prh_l2_sar_data_ind(handle, p_buf, HCI_ACL_START_FRAGMENT);
}



void Create_L2CAP_Group_Data_In(u_int16 handle,u_int16 group_psm, u_int16 group_data_len)
{
    u_int8* p_pdu;
    struct host_buf *p_buf;
    unsigned int i;

    p_buf = host_buf_alloc(group_data_len+L2CAP_PDU_HEADER_LENGTH+2);
    p_buf->len = group_data_len+L2CAP_PDU_HEADER_LENGTH+2;  
    p_pdu = p_buf->data;

    if (p_pdu) 
    {
        mSet_Uint16(p_pdu, (group_data_len+2));
        mSet_Uint16((p_pdu+2),0x0002); /* Signalling Cid */
        mSet_Uint16((p_pdu+4),group_psm);

        for (i=0;i<group_data_len;i++)
            mSet_Uint8((p_pdu+6+i),i);
    }

    printf(" Recieved ACL on Handle %x  ",handle);
        for (i = 0; i < (p_buf->len); i++)
        printf("%#x,",p_buf->data[i]);

    prh_l2_sar_data_ind(handle, p_buf, HCI_ACL_START_FRAGMENT);
}


void Create_L2CAP_Group_Data_Out(u_int16 group_cid, u_int16 group_data_len)
{
    struct host_buf * p_buf;
    u_int8* p_pdu;
    int i;

    p_buf = host_buf_alloc(group_data_len+2+L2CAP_PDU_HEADER_LENGTH+HEADERS_FOR_HCI);
    host_buf_reserve_header(p_buf,L2CAP_PDU_HEADER_LENGTH+HEADERS_FOR_HCI+2);
    p_buf->len = group_data_len;
    p_pdu = p_buf->data;

    for (i=0;i<group_data_len;i++)
        p_buf->data[i] = i;

    L2_Write(group_cid,p_buf);
    
}


#if 0

/*
 * Initialise the callback arrays to re-direct L2CAP
 * events to the test module.
 *
 */

static t_bd_addr master_bd_addr = {0x01,0x02,0x03,0x04,0x05,0x06};
static t_bd_addr slave_bd_addr = {0x11,0x12,0x13,0x14,0x15,0x16};

void init_l2ca_callback(void);

static t_l2ca_disconnect_req disconReqInfo;
static u_int16  Master_LinkId =  0x40;
static u_int16  Slave_LinkId =   0x41;
static u_int16  Master_Handle =  0x0123;
static u_int16  Slave_Handle =   0x0345;


static t_error L2_QoSViolation_Ind(t_l2ca_event_info* event_info)
{
   return BT_NOERROR;
}


u_int8 Test_L2CAP_Connection_Handler()
{
   init_l2ca_callback();

   /* First Test A Success L2CAP Connection when no LMP connection exists */

   _Test_L2CAP_Channel_Setup_No_LMP();

   /* Now Set Up an L2CAP Connection when an LMP connection exists */

   _Test_L2CAP_Channel_Setup_With_LMP();

   return 1;
}



void _Test_L2CAP_Channel_Setup_No_LMP()
{
   t_l2ca_connect_req  conReqInfo;
   t_l2ca_ping_req echoReqInfo;
   t_error result=BT_NOERROR;
   u_int8  data_field[20] = {0x01,0x02,0x03,0x04,0x05,
                             0x11,0x12,0x13,0x14,0x15,
                             0x01,0x02,0x03,0x04,0x05,
                             0x11,0x12,0x13,0x14,0x15};

   u_int8  data_field2[20] = {0x10,0x00,0x40,0x00,0x01,
                             0x02,0x03,0x04,0x05,0x06,
                             0x07,0x08,0x09,0x010,0x11,
                             0x12,0x13,0x14,0x15,0x16};


   u_int8  data_field3[0x34] = {0x30,0x00,0x40,0x00,
                             0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,
                             0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,
                             0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30};

   u_int8  connect_req_pdu[0x0c] = {0x08,0x00,0x01,0x00, 
                                    0x02,0x01,0x04,0x00,
                                    0x31,0x12,0x40,0x00};
                                    

   int i;

   t_l2ca_data_write_req write_info;

   struct host_buf *p_host_pdu_buffer;

   echoReqInfo.data = data_field;
   echoReqInfo.length = 10;

   conReqInfo.psm = 0x1231;

   _Set_Bd_Addr(&conReqInfo.bd_addr,&slave_bd_addr);

   _Set_Bd_Addr(&conReqInfo.bd_addr,&master_bd_addr);
   
  /* L2CA_Connect_Req(&conReqInfo); */

 /*  assert( W4_LP_CONNECT_CFM == L2CA_Get_State(Master_LinkId)); */
   
   /* Trigger the establishment of the LMP Connection */

   prh_l2_connect_lp_connect_ind(&master_bd_addr,Slave_Handle);

   /* LP_Connect_Cfm(&slave_bd_addr,result,Master_Handle);*/ /* Handle = 0x0123 from the HC */

   L2CA_Data_Ind(0x0345,connect_req_pdu,0x04,1);
   L2CA_Data_Ind(0x0345,connect_req_pdu+4,0x04,0);
   L2CA_Data_Ind(0x0345,connect_req_pdu+8,0x04,0);

 /*  assert(PRH_L2_OPEN == L2CA_Get_State(Master_LinkId)); */
   assert(PRH_L2_OPEN == L2CA_Get_State(Slave_LinkId));


   _Set_Bd_Addr(&echoReqInfo.bd_addr,&master_bd_addr);

   L2CA_Ping_Req(&echoReqInfo);

   _Set_Bd_Addr(&echoReqInfo.bd_addr,&slave_bd_addr);

   L2CA_Ping_Req(&echoReqInfo);

   p_host_pdu_buffer = host_buf_alloc(0x100);
   for(i=0;i<0x100;i++)
       p_host_pdu_buffer->data[i] = i;
   p_host_pdu_buffer->len = 0x30;

   write_info.p_buf = p_host_pdu_buffer;
   write_info.cid = 0x040;

   L2CA_Data_Write_Req(&write_info);
 
   L2CA_Data_Ind(0x123,data_field2,0x14,1);

   /***************************/
   /*     DATA IN TEST        */
   /***************************/

   L2CA_Data_Ind(0x123,data_field3,06,1);

   L2CA_Data_Ind(0x123,data_field3+6,03,0);

   L2CA_Data_Ind(0x123,data_field3+9,19,0);

   L2CA_Data_Ind(0x123,data_field3+28,9,0);

   L2CA_Data_Ind(0x123,data_field3+37,15,0);

   /***************************/
   /*     DATA IN TEST        */
   /***************************/
   
   L2CA_Data_Ind(0x123,data_field3,04,1);

   L2CA_Data_Ind(0x123,data_field3+4,05,0);

   L2CA_Data_Ind(0x123,data_field3+9,19,0);

   L2CA_Data_Ind(0x123,data_field3+28,9,0);

   L2CA_Data_Ind(0x123,data_field3+37,15,0);


}



void _Test_L2CAP_Channel_Setup_With_LMP()
{


}


void HC_Data_Req(u_int16 handle,u_int8* p_pdu,u_int8 length)
{
    int i;
    u_int8* new_pdu;

    printf("\n HC Rx PDU from L2CAP =");
    for(i=0;i<length;i++)
    {
        printf("%02x",p_pdu[i]);
        
    }
    printf("\n");

    if (handle == Master_Handle)
    {
        new_pdu = (u_int8*)malloc(length+4);
        assert(new_pdu != 0);
        for(i=0;i<length+4;i++)
           new_pdu[i] = p_pdu[i];
    /*    free(p_pdu);*/

    /*    HC_Data_Ind(Slave_Handle,new_pdu, 0x00); */
    }        
    else if (handle == Slave_Handle)
    {
        new_pdu = (u_int8*)malloc(length+4);
        assert(new_pdu != 0);
        for(i=0;i<length+4;i++)
           new_pdu[i] = p_pdu[i];
      /*  free(p_pdu);*/

   /*     HC_Data_Ind(Master_Handle,new_pdu, 0x00); */
    }        
    else 
    {
        printf("\n\n CONNECTION HANDLE ERROR  \n\n");
        pFree(p_pdu);
    }
}


void L2CA_Data_Report(u_int16 cid,struct host_buf* p_buf)
{
    int i;

    printf("\n CID = %x", cid);
    printf("\n Data = ");

    for (i=0;i<p_buf->len;i++)
        printf("%x", p_buf->data[i]);
}

#endif

void init_event(t_L2_ControlCallbackArgs *args)
{
    t_bdaddr zero_addr = {0};
    /* args->BDAddress= zero_addr; */
    args->psm =0;
    args->cid =0;
    args->mtu =0;
    args->flushTimeout =0;
    args->flow =0;
    args->status =0;
    args->result =0;
    args->identifier =0;
    args->timerType =0;    /* Identifies RTX or ERTX timer                  */
    args->p_buf =0;
}

void L2_Event_Handler(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
    int i;
    printf("\n        *****************************************\n\n");

/*  init_event(&upper_layer_event); */
    switch (eventType)
    {

    case L2CAP_evCONNECTCONF :
        printf(" Higer Layer Event - L2CA Connect Confirm \n");
        printf("\n       Bd_Addr = %x,%x,%x,%x,%x,%x ",args->BDAddress.bytes[0],args->BDAddress.bytes[1],args->BDAddress.bytes[2],
                                                    args->BDAddress.bytes[3],args->BDAddress.bytes[4],args->BDAddress.bytes[5]);
        printf("\n       LCID    = %x",args->cid);
        printf("\n       Result  = %x",args->result);
        printf("\n       Status  = %x",args->status);
        printf("\n");

        upper_layer_event.BDAddress = args->BDAddress;
        upper_layer_event.cid = args->cid;
        upper_layer_event.result = args->psm;
        upper_layer_event.status = args->status;
        break;

    case L2CAP_evCONNECTIND :
        printf(" Higer Layer Event - L2CA Connect Ind \n");
        printf("\n       Bd_Addr = %x,%x,%x,%x,%x,%x ",args->BDAddress.bytes[0],args->BDAddress.bytes[1],args->BDAddress.bytes[2],
                                                    args->BDAddress.bytes[3],args->BDAddress.bytes[4],args->BDAddress.bytes[5]);
        printf("\n       LCID    = %x",args->cid);
        printf("\n       PSM     = %x",args->psm);
        printf("\n       Ident   = %x",args->identifier);
        printf("\n");

        upper_layer_event.BDAddress = args->BDAddress;
        upper_layer_event.cid = args->cid;
        upper_layer_event.psm = args->psm;
        upper_layer_event.identifier = args->identifier;
        break;

    case L2CAP_evCONFIGUREIND :
        printf(" Higer Layer Event - L2CA Configure Ind \n");
        printf("\n       CID             = %x",args->cid);
        printf("\n       OutMTU          = %x",args->mtu);
        printf("\n       InFlushTO       = %x",args->flushTimeout);
        if (args->flow != 0)
        {
            upper_layer_event.flow = args->flow;
            printf("\n       QoS  Flags      = %x",args->flow->flags);
            printf("\n       QoS  Service    = %x",args->flow->serviceType);
            printf("\n       QoS  Tk Rate    = %x",args->flow->tokenRate);
            printf("\n       QoS  Bkt Size   = %x",args->flow->bucketSize);
            printf("\n       QoS  Bandwidth  = %x",args->flow->peakBandwidth);
            printf("\n       QoS  Latency    = %x",args->flow->latency);
            printf("\n       QoS  Variation  = %x",args->flow->delayVariation);
        }
        else
            printf("\n       QoS  Not Provided     ");

        upper_layer_event.result = args->result;
        upper_layer_event.mtu = args->mtu;
        upper_layer_event.flushTimeout = args->flushTimeout;
        break;

    case L2CAP_evCONFIGURECONF :
        printf(" Higer Layer Event - L2CA Configure Cfm \n");
        printf("\n       Result       = %x",args->result);
        printf("\n       InMTU        = %x",args->mtu);
        printf("\n       OutFlushTO   = %x",args->flushTimeout);
        if (args->flow != 0)
        {
            upper_layer_event.flow = args->flow;
            printf("\n       QoS  Flags      = %x",args->flow->flags);
            printf("\n       QoS  Service    = %x",args->flow->serviceType);
            printf("\n       QoS  Tk Rate    = %x",args->flow->tokenRate);
            printf("\n       QoS  Bkt Size   = %x",args->flow->bucketSize);
            printf("\n       QoS  Bandwidth  = %x",args->flow->peakBandwidth);
            printf("\n       QoS  Latency    = %x",args->flow->latency);
            printf("\n       QoS  Variation  = %x",args->flow->delayVariation);
        }
        else
            printf("\n       QoS  Not Provided     ");

        upper_layer_event.result = args->result;
        upper_layer_event.mtu = args->mtu;
        upper_layer_event.flushTimeout = args->flushTimeout;
        break;

    case L2CAP_evDISCONNECTCONF :
        printf(" Higer Layer Event - L2CA Disconnect Cfm \n");
        printf("\n       CID          = %x",args->cid);
        upper_layer_event.cid = args->cid;
        break;

    case L2CAP_evDISCONNECTIND :
        printf(" Higer Layer Event - L2CA Disconnect Ind \n");
        printf("\n       CID          = %x",args->cid);
        upper_layer_event.cid = args->cid;
        break;
       
    case L2CAP_evTIMEOUTIND :
        printf(" Higer Layer Event - L2CA Disconnect Ind \n");
        printf("\n       CID          = %x",args->cid);
        printf("\n       Timer        = %x",args->timerType);
        upper_layer_event.cid = args->cid;
        upper_layer_event.timerType = args->timerType;

        break;

    case L2CAP_evDATAIND :
        printf(" Callback on  GroupDataCallback  -- CID = %x \n", args->cid);
        printf("Data = ");
        for(i=0; i<args->p_buf->len; i++)
            printf(" %x,",args->p_buf->data[i]);
        printf("\n");
        break;
    default :
        break;
    }
    
}

void L2_DataHandler(u_int16 cid, struct host_buf* p_buf)
{
    int i;

    printf(" Callback on  GroupDataCallback  -- CID = %x \n", cid);
    printf("Data = ");
    for(i=0; i<p_buf->len; i++)
        printf(" %x,",p_buf->data[i]);
    printf("\n");

}
void L2_Ping_Callback(t_bdaddr BDAddress, u_int16 result, u_int8 *echoData, u_int16 length)
{
    printf("\n  Ping Callback \n");
    printf("         Bd_Addr = [%x:%x:%x:%x:%x:%x] \n",BDAddress.bytes[0],BDAddress.bytes[1],BDAddress.bytes[2],BDAddress.bytes[3],BDAddress.bytes[4],BDAddress.bytes[5]);
}

void L2_GetInfo_Callback(t_bdaddr BDAddress,u_int16 result, u_int8* infoData, u_int16 info_len)
{
    int i;

    printf("\n  Get Info Callback \n");
    printf("         Bd_Addr = [%x:%x:%x:%x:%x:%x]",BDAddress.bytes[0],BDAddress.bytes[1],BDAddress.bytes[2],BDAddress.bytes[3],BDAddress.bytes[4],BDAddress.bytes[5]);
    printf("         Result =  %x \n", result);
    printf("         Info_Len = %x \n",info_len);
    printf("\n       Info Data = ");
    for (i=0; i < info_len; i++)
        printf("%x,",infoData[i]);
    printf("\n");
}

void L2_GroupDataCallback(u_int16 cid, u_int8 *buf, u_int32 bufLen)
{
    u_int32 i;

    printf(" Callback on  GroupDataCallback  -- CID = %x \n", cid);

    printf("Data = ");
    for(i=0; i<bufLen; i++)
        printf(" %x,",buf[i]);
    printf("\n");
}


void L2_GroupAddMemberCfm(u_int16 cid,  t_bdaddr BDA, u_int16 status)
{
    printf(" Callback on GroupAddMember  :--  Cid = %x, Status = %x \n", cid, status);
    printf("                                  BDA = %x:%x:%x:%x:%x:%x \n",BDA.bytes[0],BDA.bytes[1],BDA.bytes[2],BDA.bytes[3],BDA.bytes[4],BDA.bytes[5]);
}

void print_group_members(t_bdaddr *addrList, u_int16 addrListLen)
{
    int i,k;

    printf(" L2CAP Group Members :- Num = %x \n", addrListLen);
    printf("     BD ADDRESS List  \n");
    for (i=0;i<addrListLen;i++)
    {
        printf(" BD Address Num %x = ",i);
        for (k=0;k<6;k++)
            printf("%x:",addrList[i].bytes[k]);
        printf("\n");
    }
}

u_int8 Test_L2CAP_PSM(void)
{
    l2_callbacks cbs;

    cbs.connectionIndication = &L2_Event_Handler;
    cbs.configureIndication = &L2_Event_Handler;
    cbs.disconnectionIndication = &L2_Event_Handler;
    cbs.qosviolationIndication = L2_Event_Handler;
    cbs.connectionConfirmation = L2_Event_Handler;
    cbs.configureConfirmation = L2_Event_Handler;
    cbs.disconnectionConfirmation = L2_Event_Handler;

#if 0
    assert(BT_NOERROR == L2_RegisterPSM(0x21, &cbs));
    assert(BT_NOERROR == L2_DeregisterPSM(0x21, &cbs));


    /* ReRegister a PSM */
    assert(BT_NOERROR == L2_RegisterPSM(0x21, &cbs));
    assert(BT_NOERROR == L2_RegisterPSM(0x21, &cbs));
#endif

    assert(BT_NOERROR != L2_DeregisterPSM(0x21));

    return 1;
}
    


u_int8 Test_L2CAP_Service_Interface_Invalid_Params(void)
{
    l2_callbacks cbs;

    cbs.connectionIndication = &L2_Event_Handler;
    cbs.configureIndication = &L2_Event_Handler;
    cbs.disconnectionIndication = &L2_Event_Handler;
    cbs.qosviolationIndication = L2_Event_Handler;
    cbs.connectionConfirmation = L2_Event_Handler;
    cbs.configureConfirmation = L2_Event_Handler;
    cbs.disconnectionConfirmation = L2_Event_Handler;

    assert(BT_INVALIDPARAM == L2_Connect(slave_bd_addr,0x011)); /* Connect on Unregisted PSM */
    assert(BT_INVALIDPARAM == L2_Disconnect(0x082)); /* Disonnect on invalid CID */
    assert(BT_INVALIDPARAM == L2_DeregisterPSM(0x81)); /* Deregister an un-used PSM */
    assert(BT_INVALIDPARAM == L2_ConnectResponse(slave_bd_addr, 0x82,0x0, 0x0)); /* Connect Response on un-used CID */
    assert(BT_INVALIDPARAM == L2_Configure(0x82, 0x4000, 0xFFFF, 0x0100, 0)); /* Configure on un-used CID */
    assert(BT_INVALIDPARAM == L2_ConfigureResponse(0x82, 0x4000, 0x0, 0x0));
    assert(BT_INVALIDPARAM == L2_GroupAddMember(0x45,slave_bd_addr,L2_GroupAddMemberCfm));
    assert(BT_INVALIDPARAM == L2_GroupRemoveMember(0x50,slave_bd_addr));
    assert(BT_INVALIDPARAM == L2_RegisterPSM(0x22, &cbs)); /* Register an event PSM */
    assert(BT_INVALIDPARAM == L2_GroupClose(0x56)); /* Close a non existant group */

    return 1;   
}

int main(void)
{
    l2_callbacks cbs;
    HCI_Event_t  hci_event;

    u_int8 info_data_field[8] = {0xff,0x03,0x11,0x22,0x33,0x44,0x55,0x66};

    pInit();
    prh_l2_connect_sm_initialise();

    Test_L2CAP_Service_Interface_Invalid_Params();

    Test_L2CAP_PSM();

    g_l2cap_acl_size = 20;

    cbs.connectionIndication = &L2_Event_Handler;
    cbs.configureIndication = &L2_Event_Handler;
    cbs.disconnectionIndication = &L2_Event_Handler;
    cbs.qosviolationIndication = &L2_Event_Handler;
    cbs.connectionConfirmation = &L2_Event_Handler;
    cbs.configureConfirmation = &L2_Event_Handler;
    cbs.disconnectionConfirmation = &L2_Event_Handler;
    cbs.dataIndication = &L2_DataHandler;

   /* cbs.dataIndication = L2_Event_Handler; */

    assert(BT_NOERROR == L2_RegisterPSM(0x21, &cbs));

    /************************************
     *  Setup 3 outgoing L2CAP channels
     ************************************/

    Setup_L2CAP_Out_Channel(slave_bd_addr,0x21);

  /*  Loc_Disconnect_L2CAP_Out_Channel(0x40); */

    Setup_L2CAP_Out_Channel(slave_bd_addr,0x21);

    Setup_L2CAP_Out_Channel(slave_bd_addr,0x21);

    /************************************
     *  Setup 3 incoming L2CAP channels
     ************************************/

    Setup_L2CAP_In_Channel(slave_bd_addr,0x21);

    Setup_L2CAP_In_Channel(slave_bd_addr,0x21);

    Setup_L2CAP_In_Channel(slave_bd_addr,0x21);

    /******************************************
     * Local Disconnect of an Outgoing Channel
     ******************************************/

    Loc_Disconnect_L2CAP_Out_Channel(0x41);

    loc_cid = 0x41;

    Setup_L2CAP_Out_Channel(slave_bd_addr,0x21  );


    /******************************************
     * Local Disconnect of an Incoming Channel
     ******************************************/

    Loc_Disconnect_L2CAP_Out_Channel(0x45);

    /******************************************
     * Remote Disconnect of an Outgoing Channel
     ******************************************/

    Rem_Disconnect_L2CAP_In_Channel(0x42);
    
    /******************************************
     * Remote Disconnect of an Incoming Channel
     ******************************************/

    Rem_Disconnect_L2CAP_In_Channel(0x44);

    /*********************************/
    /* Extablish a simple connection */
    /*********************************/
    
    Out_Reconfig_L2CAP_Channel_MTU(0x40,0x4000);

    Out_Reconfig_L2CAP_Channel_MTU(0x40,0x1327);

    In_Reconfig_L2CAP_Channel_MTU(0x40,0x1327);

    Reconfig_x2_L2CAP_Channel_MTU(0x40,0x1500);

    /*****************************************************
     * Try A Ping before the connection release
     *****************************************************/

    L2_Ping(slave_bd_addr,(u_int8*)0x00,0x00, L2_Ping_Callback);

    Create_L2CAP_Echo_Rsp_PDU(handle,loc_tid,0x00,0x00); 

    loc_cid++;
    loc_tid++; 

/*    L2_Ping(slave_bd_addr,data_field, 0x10, L2_Ping_Callback);*/

    /************************************************** 
     *  Now try a connection Release
     **************************************************/

    printf("                      ________________________________________________________________ \n");
    printf("                     |   Len    |   CID    | Code | Id  |   Len   |  DCID   |  SCID   | \n");
    printf("                     +----------+----------+------+-----+---------+---------+---------+ \n");
    printf(" Expected ACL to Tx =| 0x08,0x00|0x01,0x00 | 0x06 |0x47 |0x04,0x00|0x94,0x00|0x40,0x00| \n");

    hci_event.Type=HCI_evDISCONNECTIONCOMPLETE;
    hci_event.Parms.DisconnectionComplete.ConnectionHandle = handle;
    prh_l2_connect_event_handler(&hci_event);


    /*****************************************************
     * Try A Ping with NO ACL Active
     *****************************************************/

    L2_Ping(slave_bd_addr,(u_int8*)0x00,0x00, L2_Ping_Callback);

    prh_l2_connect_lp_connect_cfm(current_Tid,0x00);

    handle = 0x14;
    Create_L2CAP_Echo_Rsp_PDU(handle,loc_tid,0x00,0x00);

    loc_cid++;
    loc_tid++;  

    /* Test above here */

    /*****************************************************
     * Try A Ping with NO ACL Active
     *****************************************************/   


    L2_GetInfo(slave_bd_addr, 0x0001, L2_GetInfo_Callback);

    prh_l2_connect_lp_connect_cfm(current_Tid,0x00);

    handle = 0x15;

    Create_L2CAP_Information_Rsp_PDU(handle,loc_tid /* Tid */, 0x06 /* Len*/, 0x01 /*Info Type*/, 0x00 /* Success*/, info_data_field /* clsMTU */);

    loc_cid++;
    loc_tid++;  
    
    /* Note Need to establish an ACL link first */

    handle = 0x16;
    prh_l2_connect_lp_connect_ind(&slave_bd_addr, handle);

    Create_L2CAP_Echo_Req_PDU(handle,0x60,0x00,0x00);

    Create_L2CAP_Information_Req_PDU(handle,0x51,0x02,0x01);

    hci_event.Type=HCI_evDISCONNECTIONCOMPLETE;
    hci_event.Parms.DisconnectionComplete.ConnectionHandle = handle;
    prh_l2_connect_event_handler(&hci_event);

    rem_cid = 0x94;
    loc_cid = 0x40;

    Test_L2CAP_QOS();

    Test_L2CAP_Groups();

}

u_int8 Test_L2CAP_QOS()
{

    Setup_L2CAP_Out_Channel_With_QoS(slave_bd_addr,0x21);

  /*  Loc_Disconnect_L2CAP_Out_Channel(0x40); */

    Setup_L2CAP_Out_Channel_With_QoS(slave_bd_addr,0x21  );

    Setup_L2CAP_Out_Channel_With_QoS(slave_bd_addr,0x21  );

    return 1;
}

u_int8 Test_L2CAP_Groups()
{

    u_int16 group_cid;
    t_bdaddr* p_bdaddr_list;
    u_int16 num_members;
    l2_callbacks cbs;
    /*********************************************/
    /* TEST OF L2CAP GROUPS FUNCTIONALITY        */
    /*********************************************/

    cbs.connectionIndication = &L2_Event_Handler;
    cbs.configureIndication = &L2_Event_Handler;
    cbs.disconnectionIndication = &L2_Event_Handler;
    cbs.qosviolationIndication = &L2_Event_Handler;
    cbs.connectionConfirmation = &L2_Event_Handler;
    cbs.configureConfirmation = &L2_Event_Handler;
    cbs.disconnectionConfirmation = &L2_Event_Handler;
    cbs.dataIndication = &L2_DataHandler;

   /* cbs.dataIndication = L2_Event_Handler; */

    assert(BT_NOERROR == L2_RegisterPSM(0x501, &cbs));
    assert(BT_NOERROR == L2_RegisterPSM(0x451, &cbs));

    assert(BT_NOERROR == L2_GroupCreate(0x451, &group_cid));

    assert(BT_PENDING == L2_GroupAddMember(group_cid,bd_addr1, L2_GroupAddMemberCfm)); 
    prh_l2_connect_lp_connect_cfm(current_Tid,0x00);

/*  L2_GroupClose(group_cid);*/

    assert(BT_PENDING == L2_GroupAddMember(group_cid,bd_addr2, L2_GroupAddMemberCfm)); 
    prh_l2_connect_lp_connect_cfm(current_Tid,0x00);

    handle = 0x18;

    assert(BT_PENDING == L2_GroupAddMember(group_cid,bd_addr3, L2_GroupAddMemberCfm)); 
    prh_l2_connect_lp_connect_cfm(current_Tid,0x00);

    assert(BT_PENDING == L2_GroupAddMember(group_cid,bd_addr4, L2_GroupAddMemberCfm)); 
    prh_l2_connect_lp_connect_cfm(current_Tid,0x00);

#if 0
    L2_GroupRemoveMember(group_cid, bd_addr3);

    L2_GroupRemoveMember(group_cid, bd_addr1);

    L2_GroupRemoveMember(group_cid, bd_addr4);

    L2_GroupRemoveMember(group_cid, bd_addr2);

    L2_GroupClose(group_cid);
#endif

    assert(BT_NOERROR == L2_GroupCreate(0x501, &group_cid));

    assert(BT_NOERROR == L2_GroupAddMember(group_cid,bd_addr1, L2_GroupAddMemberCfm)); 
/*  LP_Connect_Cfm(current_Tid,0x00);*/

    assert(BT_NOERROR == L2_GroupAddMember(group_cid,bd_addr2, L2_GroupAddMemberCfm)); 
/*  LP_Connect_Cfm(current_Tid,0x00);*/

    assert(BT_NOERROR == L2_GroupAddMember(group_cid,bd_addr3, L2_GroupAddMemberCfm)); 
/*  LP_Connect_Cfm(current_Tid,0x00);*/

    assert(BT_NOERROR == L2_GroupAddMember(group_cid,bd_addr4, L2_GroupAddMemberCfm)); 
/*  LP_Connect_Cfm(current_Tid,0x00); */

    assert(BT_NOERROR == L2_GroupCreate(0x501, &group_cid));

    assert(BT_NOERROR == L2_GroupAddMember(group_cid,bd_addr1, L2_GroupAddMemberCfm)); 
/*  LP_Connect_Cfm(current_Tid,0x00);*/

    assert(BT_NOERROR == L2_GroupAddMember(group_cid,bd_addr2, L2_GroupAddMemberCfm)); 
/*  LP_Connect_Cfm(current_Tid,0x00);*/

    assert(BT_NOERROR == L2_GroupAddMember(group_cid,bd_addr3, L2_GroupAddMemberCfm)); 
/*  LP_Connect_Cfm(current_Tid,0x00);*/

    assert(BT_NOERROR == L2_GroupAddMember(group_cid,bd_addr4, L2_GroupAddMemberCfm));

    num_members = 10;
    p_bdaddr_list = pMalloc(sizeof(t_bdaddr)*num_members);
    assert(BT_NOERROR == L2_GroupGetMembers(group_cid,p_bdaddr_list,&num_members));
    print_group_members(p_bdaddr_list,num_members);
    pFree(p_bdaddr_list);


    num_members = 2;
    p_bdaddr_list = pMalloc(sizeof(t_bdaddr)*num_members);
    assert(BT_RETRY == L2_GroupGetMembers(group_cid,p_bdaddr_list,&num_members));
    assert(num_members == 0x04);
    pFree(p_bdaddr_list);


    num_members = 0;
    p_bdaddr_list = pMalloc(sizeof(t_bdaddr)*num_members);
    assert(BT_NOERROR == L2_GroupGetMembers(group_cid,p_bdaddr_list,&num_members));
    assert(num_members == 0x04);
    pFree(p_bdaddr_list);


#if 1
    assert(BT_NOERROR == L2_DisableConnectionlessTraffic(0x0501));

    assert(BT_NOERROR == L2_EnableConnectionlessTraffic(0x0501));

    assert(BT_NOERROR == L2_DisableConnectionlessTraffic(0x0451));

    assert(BT_NOERROR == L2_EnableConnectionlessTraffic(0x0451));

    assert(BT_NOERROR == L2_DisableConnectionlessTraffic(0x0000));

    assert(BT_NOERROR == L2_EnableConnectionlessTraffic(0x0000));

    Create_L2CAP_Group_Data_In(handle,0x0501,0x30);

    assert(BT_NOERROR == L2_DisableConnectionlessTraffic(0x0501));

    Create_L2CAP_Group_Data_In(handle,0x0501,0x30);

    assert(BT_NOERROR == L2_EnableConnectionlessTraffic(0x0501));

    Create_L2CAP_Group_Data_In(handle,0x0501,0x30);

    Create_L2CAP_Group_Data_In(handle,0x0451,0x20);

    Create_L2CAP_Group_Data_Out(group_cid,0x4);

    Create_L2CAP_Group_Data_Out(group_cid,0x30);
#endif
    L2_GroupClose(group_cid);

    return 1;
}


void Setup_L2CAP_In_Channel(t_bdaddr target_bdaddr, u_int16 psm)
{

    prh_t_l2_channel* p_channel;

    printf(" Expected ACL to Rx = 0x08,0x00,0x01,0x00,0x02,%x,0x04,0x00,%x,0x00,%x,0x00 \n",rem_tid, psm,rem_cid);

    Create_L2CAP_Connect_Req_PDU(0x13 /* handle     */, 
                                 rem_tid /* remote_Tid */, 
                                 psm /*  Psm */,
                                 rem_cid /* local_cid  */);

    printf(" Expected ACL to Tx = 0x0c,0x00,0x01,0x00,0x03,%x,0x08,0x00,%x,0x00,%x, 0x00, 0x00,0x00,0x00,0x00\n",rem_tid,loc_cid,rem_cid);

        
    /* printf(" Expected ACL to Tx = 0x10,0x00,0x01,0x00,0x03,0x46,0x0c,0x00,0x94,0x00,0x00,0x00,0x01,0x02,0xFF,0x00,0x02,0x02,0xFF,0xFF \n"); */

    L2_ConnectResponse(target_bdaddr, loc_cid, 0x0 /* result */, 0x00 /* Status */);

    printf(" Expected ACL to Rx = 0x08,0x00,0x01,0x00,0x04,%x,0x04,0x00,%x,0x00,0x00,0x00 \n", rem_tid,loc_cid);

      rem_tid++;

    Create_L2CAP_Config_Req_PDU( handle /* handle      */,
                                 rem_tid /* remote_Tid  */, 
                                 loc_cid /* local_cid  */,
                                 0x00 /* flags       */,
                                 0x00 /* MTU         */,
                                 0x00 /* Flow        */,
                                 0x00 /* QoS         */);

    printf("                     |  Len    |   CID   |Code| Id  |  Len   |  SCID   |  Flags  |  Result | Option Field [MTU] | \n");
    printf(" Expected ACL to Tx = 0x0e,0x00,0x01,0x00,0x05,%x,0x0a,0x00,0x%x,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0xFF,0x7D \n", rem_tid,rem_cid);

    L2_ConfigureResponse(loc_cid /* CID */, 0x7dff /* outMTU */ , 0x00 /* Result */, 0 /* InFlow */) ;

    L2_Configure(loc_cid   /* cid             */,
                 0x00ff /* inMTU           */,
                 0xFFFF /* outFlushTimeout */,
                 0xFFFF /* linkTimeout     */,
                 0x00);

    p_channel = prh_l2_chn_get_p_channel(loc_cid);
    prh_l2_config_link_supervision_timeout_complete((tid_t)p_channel, BT_NOERROR);

    prh_l2_config_flush_timeout_complete((tid_t)p_channel, BT_NOERROR);

    Create_L2CAP_Config_Rsp_PDU( handle /* handle      */,
                                 loc_tid /* remote_Tid  */, 
                                 loc_cid /* local_cid  */,
                                 0x00 /* flags       */,
                                 0x00 /* result      */,
                                 0x00 /* InMTU       */,
                                 0x00 /* Flush TO    */,
                                 0x00 /* OutFlow     */
                                 );

    p_channel = prh_l2_chn_get_p_channel(loc_cid);

    assert(p_channel->state == PRH_L2_OPEN);
    assert(p_channel->config_req_path == PRH_L2_CONFIG_END);
    assert(p_channel->config_rsp_path == PRH_L2_CONFIG_END);

    printf("\n ***************************************************");  
    printf("\n CHANNEL %x IS NOW ACTIVE !!!! - IN THE PRH_L2_OPEN STATE     ", loc_cid);
    printf("\n ***************************************************\n");

    loc_cid++;
    rem_cid++;
    rem_tid++;
    loc_tid++;  
}

void Setup_L2CAP_Out_Channel(t_bdaddr target_bdaddr, u_int16 target_PSM )
{

    prh_t_l2_channel* p_channel;
    t_api status;

    printf(" Expected ACL to Tx = 0x08,0x00,0x01,0x00,0x02,%x,0x04,0x00,%x,0x00,%x,0x00 \n",loc_tid, target_PSM,loc_cid);

    status = L2_Connect(target_bdaddr,target_PSM); /* Connect on PSM 0x21*/

    if (status == BT_PENDING)
       prh_l2_connect_lp_connect_cfm(current_Tid,0x00);
    else if (status == BT_NOERROR)
       ;
    else 
        assert (1==0);
       

    printf(" Expected ACL to Rx = 0x0c,0x00,0x01,0x00,0x03,%x,0x08,0x00,%x,0x00,%x,0x00,0x00,0x00,0x00,0x00 \n", loc_tid,rem_cid,loc_cid);

    Create_L2CAP_Connect_Rsp_PDU(0x13 /* handle     */, 
                                 loc_tid /* remote_Tid */, 
                                 rem_cid /* remote_cid */,
                                 loc_cid /* local_cid  */,
                                 0x00 /* result     */,
                                 0x00 /* status     */);

    printf(" Expected ACL to Tx = 0x10,0x00,0x01,0x00,0x04,0x46,0x0c,0x00,0x94,0x00,0x00,0x00,0x01,0x02,0xFF,0x00,0x02,0x02,0xFF,0xFF \n");

    loc_tid++;

    L2_Configure(loc_cid   /* cid             */,
                 0x00ff /* inMTU           */,
                 0xFFFF /* outFlushTimeout */,
                 0xFFFF /* linkTimeout     */,
                 0);

    p_channel = prh_l2_chn_get_p_channel(loc_cid);
    prh_l2_config_link_supervision_timeout_complete((tid_t)p_channel, BT_NOERROR);

    prh_l2_config_flush_timeout_complete((tid_t)p_channel, BT_NOERROR);

    printf(" Expected ACL to Rx = 0x0a,0x00,0x01,0x00,0x05,%x,0x06,0x00,%x,0x00,0x00,0x00,0x00,0x00 \n", loc_tid,loc_cid);

    Create_L2CAP_Config_Rsp_PDU( handle /* handle      */,
                                 loc_tid /* remote_Tid  */, 
                                 loc_cid /* local_cid  */,
                                 0x00 /* flags       */,
                                 0x00 /* result      */,
                                 0x00 /* InMTU       */,
                                 0x00 /* Flush TO    */,
                                 0x00 /* OutFlow     */
                                 );

    rem_tid++;

    printf(" Expected ACL to Rx = 0x08,0x00,0x01,0x00,0x04,%x,0x04,0x00,%x,0x00,0x00,0x00 \n", rem_tid,loc_cid);

    Create_L2CAP_Config_Req_PDU( handle /* handle      */,
                                 rem_tid /* remote_Tid  */, 
                                 loc_cid /* local_cid  */,
                                 0x00 /* flags       */,
                                 0x00 /* MTU         */,
                                 0x00 /* Flow        */,
                                 0x00 /* QoS         */);

    printf("                     |  Len    |   CID   |Code| Id  |  Len   |  SCID   |  Flags  |  Result | Option Field [MTU] | \n");
    printf(" Expected ACL to Tx = 0x0e,0x00,0x01,0x00,0x05,%x,0x0a,0x00,0x%x,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0xFF,0x7D \n", rem_tid,rem_cid);

    L2_ConfigureResponse(loc_cid /* CID */, 0x7dff /* outMTU */ , 0x00 /* Result */, 0 /* InFlow */) ;

    p_channel = prh_l2_chn_get_p_channel(loc_cid);

    assert(p_channel->state == PRH_L2_OPEN);
    assert(p_channel->config_req_path == PRH_L2_CONFIG_END);
    assert(p_channel->config_rsp_path == PRH_L2_CONFIG_END);
    assert(p_channel->outMTU == 0x7dff);

    printf("\n ***************************************************");  
    printf("\n CHANNEL %x IS NOW ACTIVE !!!! - IN THE OPEN STATE     ", loc_cid);
    printf("\n ***************************************************\n");

    loc_cid++;
    rem_cid++;
    rem_tid++;
    loc_tid++;  
}


void Setup_L2CAP_Out_Channel_With_QoS(t_bdaddr target_bdaddr, u_int16 target_PSM )
{

    prh_t_l2_channel* p_channel;
    t_api status;
    t_L2_ChannelFlowSpec outflow;

    outflow.flags = 0;
    outflow.latency = 0x100;
    outflow.serviceType = 0x02;
    outflow.delayVariation = 0x200;
    outflow.peakBandwidth = 0x4000;
    outflow.tokenRate = 0x8000;
    outflow.bucketSize = 0x3500;


    printf(" Expected ACL to Tx = 0x08,0x00,0x01,0x00,0x02,%x,0x04,0x00,%x,0x00,%x,0x00 \n",loc_tid, target_PSM,loc_cid);

    status = L2_Connect(target_bdaddr,target_PSM); /* Connect on PSM 0x21*/

    if (status == BT_PENDING)
       prh_l2_connect_lp_connect_cfm(current_Tid,0x00);
    else if (status == BT_NOERROR)
       ;
    else 
        assert (1==0);
       

    printf(" Expected ACL to Rx = 0x0c,0x00,0x01,0x00,0x03,%x,0x08,0x00,%x,0x00,%x,0x00,0x00,0x00,0x00,0x00 \n", loc_tid,rem_cid,loc_cid);

    Create_L2CAP_Connect_Rsp_PDU(handle /* handle     */, 
                                 loc_tid /* remote_Tid */, 
                                 rem_cid /* remote_cid */,
                                 loc_cid /* local_cid  */,
                                 0x00 /* result     */,
                                 0x00 /* status     */);

    printf(" Expected ACL to Tx = 0x10,0x00,0x01,0x00,0x04,0x46,0x0c,0x00,0x94,0x00,0x00,0x00,0x01,0x02,0xFF,0x00,0x02,0x02,0xFF,0xFF \n");

    loc_tid++;

    L2_Configure(loc_cid   /* cid             */,
                 0x00ff /* inMTU           */,
                 0xFFFF /* outFlushTimeout */,
                 0xFFFF /* linkTimeout     */,
                 &outflow);

    p_channel = prh_l2_chn_get_p_channel(loc_cid);
    prh_l2_config_link_supervision_timeout_complete((tid_t)p_channel, BT_NOERROR);

    prh_l2_config_flush_timeout_complete((tid_t)p_channel, BT_NOERROR);

    printf(" Expected ACL to Rx = 0x0a,0x00,0x01,0x00,0x05,%x,0x06,0x00,%x,0x00,0x00,0x00,0x00,0x00 \n", loc_tid,loc_cid);

    Create_L2CAP_Config_Rsp_PDU( handle /* handle      */,
                                 loc_tid /* remote_Tid  */, 
                                 loc_cid /* local_cid  */,
                                 0x00 /* flags       */,
                                 0x00 /* result      */,
                                 0x03FF /* InMTU       */,
                                 0x12ff /* Flush TO    */,
                                 &outflow /* OutFlow     */
                                 );

    rem_tid++;

    printf(" Expected ACL to Rx = 0x08,0x00,0x01,0x00,0x04,%x,0x04,0x00,%x,0x00,0x00,0x00 \n", rem_tid,loc_cid);

    Create_L2CAP_Config_Req_PDU( handle /* handle      */,
                                 rem_tid /* remote_Tid  */, 
                                 loc_cid /* local_cid  */,
                                 0x00 /* flags       */,
                                 0x0244 /* MTU         */,
                                 0x0340 /* Flow        */,
                                 &outflow /* QoS         */);

    printf("                     |  Len    |   CID   |Code| Id  |  Len   |  SCID   |  Flags  |  Result | Option Field [MTU] | \n");
    printf(" Expected ACL to Tx = 0x0e,0x00,0x01,0x00,0x05,%x,0x0a,0x00,0x%x,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0xFF,0x7D \n", rem_tid,rem_cid);

    L2_ConfigureResponse(loc_cid /* CID */, 0x7dff /* outMTU */ , 0x00 /* Result */, &outflow ) ;

    p_channel = prh_l2_chn_get_p_channel(loc_cid);

    assert(p_channel->state == PRH_L2_OPEN);
    assert(p_channel->config_req_path == PRH_L2_CONFIG_END);
    assert(p_channel->config_rsp_path == PRH_L2_CONFIG_END);
    assert(p_channel->outMTU == 0x7dff);

    printf("\n ***************************************************");  
    printf("\n CHANNEL %x IS NOW ACTIVE !!!! - IN THE OPEN STATE     ", loc_cid);
    printf("\n ***************************************************\n");

    loc_cid++;
    rem_cid++;
    rem_tid++;
    loc_tid++;  
}

void Loc_Disconnect_L2CAP_Out_Channel(u_int16 release_cid)
{
    prh_t_l2_channel* p_channel;

    L2_Disconnect(release_cid);

    /* Need to determine the DCID for this channel */
    p_channel = prh_l2_chn_get_p_channel(release_cid);
    Create_L2CAP_Disconnect_Rsp_PDU(handle /* handle     */,
                                      loc_tid     /* remote_Tid */, 
                                      p_channel->remote_cid      /* DCID       */,
                                      release_cid /* SCID       */);   

    p_channel = prh_l2_chn_get_p_channel(release_cid);
    assert(p_channel == 0);

    loc_tid++;

}

void Rem_Disconnect_L2CAP_In_Channel(u_int16 release_cid)
{
    prh_t_l2_channel* p_channel;

    /* Need to determine the DCID for this channel */
    p_channel = prh_l2_chn_get_p_channel(release_cid);

    Create_L2CAP_Disconnect_Req_PDU(handle,rem_tid,release_cid,p_channel->remote_cid);

    p_channel = prh_l2_chn_get_p_channel(release_cid);
    assert(p_channel == 0);

    rem_tid++;
}

void Out_Reconfig_L2CAP_Channel_MTU(u_int16 loc_cid, u_int16 newMTU)
{
    prh_t_l2_channel* p_channel;

    p_channel = prh_l2_chn_get_p_channel(loc_cid);

    L2_Configure(loc_cid   /* cid             */,
                 newMTU /* inMTU           */,
                 0x0000 /* outFlushTimeout */,
                 0x0000 /* linkTimeout     */,
                 0x00);

    assert(p_channel->state == PRH_L2_CONFIG);
    assert(p_channel->config_req_path == PRH_L2_REQUEST_SENT);

    Create_L2CAP_Config_Rsp_PDU( handle /* handle      */,
                                 loc_tid /* p_channel->local_transaction_identifier */ /* remote_Tid  */, 
                                 loc_cid /* remote_cid  */,
                                 0x00 /* flags       */,
                                 0x00 /* result      */,
                                 0x00 /* InMTU       */,
                                 0x00 /* Flush TO    */,
                                 0x00 /* OutFlow     */
                                 );

    loc_tid++;

    assert(p_channel->config_req_path == PRH_L2_CONFIG_END);
    assert(p_channel->state == PRH_L2_CONFIG);

    Create_L2CAP_Config_Req_PDU( handle /* handle      */,
                                 rem_tid /* remote_Tid  */, 
                                 loc_cid /* remote_cid  */,
                                 0x00 /* flags       */,
                                 (u_int16)(newMTU*3) /* MTU         */,
                                 0x00 /* Flow        */,
                                 0x00 /* QoS         */);


    assert(p_channel->config_rsp_path == PRH_L2_REQUEST_RECEIVED);
    assert(p_channel->state == PRH_L2_CONFIG);

    L2_ConfigureResponse(loc_cid,(u_int16)(newMTU*2), 0x00, 0);

    rem_tid++;

    assert(p_channel->state == PRH_L2_OPEN);
    assert(p_channel->config_rsp_path == PRH_L2_CONFIG_END);
    assert(p_channel->config_req_path == PRH_L2_CONFIG_END);

    assert(p_channel->inMTU == newMTU);
    assert(p_channel->outMTU == (newMTU*2));


    printf("\n ***************************************************");  
    printf("\n CHANNEL RECONFIGURED FOR MTU OF  %x  ", newMTU );
    printf("\n ***************************************************\n");

}



void In_Reconfig_L2CAP_Channel_MTU(u_int16 loc_cid, u_int16 newMTU)
{
    prh_t_l2_channel* p_channel;

    p_channel = prh_l2_chn_get_p_channel(loc_cid);

    Create_L2CAP_Config_Req_PDU( handle /* handle      */,
                                 rem_tid /* remote_Tid  */, 
                                 loc_cid /* remote_cid  */,
                                 0x00 /* flags       */,
                                 newMTU /* MTU         */,
                                 0x00 /* Flow        */,
                                 0x00 /* QoS         */);

    assert(p_channel->config_rsp_path == PRH_L2_REQUEST_RECEIVED);
    assert(p_channel->state == PRH_L2_CONFIG);

    L2_ConfigureResponse(loc_cid,newMTU, 0x00, 0);

    rem_tid++;

    assert(p_channel->config_rsp_path == PRH_L2_CONFIG_END);
    assert(p_channel->state == PRH_L2_CONFIG);


    L2_Configure(loc_cid   /* cid             */,
                 newMTU /* inMTU           */,
                 0x0000 /* outFlushTimeout */,
                 0x0000 /* linkTimeout     */,
                 0x00);

    assert(p_channel->state == PRH_L2_CONFIG);
    assert(p_channel->config_req_path == PRH_L2_REQUEST_SENT);

    Create_L2CAP_Config_Rsp_PDU( handle /* handle      */,
                                 loc_tid /* p_channel->local_transaction_identifier */ /* remote_Tid  */, 
                                 loc_cid /* remote_cid  */,
                                 0x00 /* flags       */,
                                 0x00 /* result      */,
                                 0x00 /* InMTU       */,
                                 0x00 /* Flush TO    */,
                                 0x00 /* OutFlow     */
                                 );

    loc_tid++;
    assert(p_channel->state == PRH_L2_OPEN);
    assert(p_channel->config_rsp_path == PRH_L2_CONFIG_END);
    assert(p_channel->config_req_path == PRH_L2_CONFIG_END);

    assert(p_channel->inMTU == newMTU);
    assert(p_channel->outMTU == newMTU);


    printf("\n ***************************************************");  
    printf("\n CHANNEL RECONFIGURED FOR MTU OF  %x  ", newMTU );
    printf("\n ***************************************************\n");

}


void Reconfig_x2_L2CAP_Channel_MTU(u_int16 loc_cid, u_int16 newMTU)
{
    prh_t_l2_channel* p_channel;

    p_channel = prh_l2_chn_get_p_channel(loc_cid);

    L2_Configure(loc_cid   /* cid             */,
                 newMTU /* inMTU           */,
                 0x0000 /* outFlushTimeout */,
                 0x0000 /* linkTimeout     */,
                 0x00);

    assert(p_channel->state == PRH_L2_CONFIG);
    assert(p_channel->config_req_path == PRH_L2_REQUEST_SENT);

    Create_L2CAP_Config_Rsp_PDU( handle /* handle      */,
                                 loc_tid /* p_channel->local_transaction_identifier */ /* remote_Tid  */, 
                                 loc_cid /* remote_cid  */,
                                 0x00 /* flags       */,
                                 0x00 /* result      */,
                                 0x00 /* InMTU       */,
                                 0x00 /* Flush TO    */,
                                 0x00 /* OutFlow     */
                                 );

    loc_tid++;

    assert(p_channel->config_req_path == PRH_L2_CONFIG_END);
    assert(p_channel->state == PRH_L2_CONFIG);

    Create_L2CAP_Config_Req_PDU( handle /* handle      */,
                                 rem_tid /* remote_Tid  */, 
                                 loc_cid /* remote_cid  */,
                                 0x01 /* flags       */,
                                 newMTU /* MTU         */,
                                 0x00 /* Flow        */,
                                 0x00 /* QoS         */);



  /*  assert(p_channel->config_rsp_path == CONFIG_STA */
    assert(p_channel->state == PRH_L2_CONFIG);



    L2_ConfigureResponse(loc_cid,newMTU, 0x00, 0);

    rem_tid++;

    Create_L2CAP_Config_Req_PDU( handle /* handle      */,
                                 rem_tid /* remote_Tid  */, 
                                 loc_cid /* remote_cid  */,
                                 0x00 /* flags       */,
                                 newMTU /* MTU         */,
                                 0x00 /* Flow        */,
                                 0x00 /* QoS         */);

    assert(p_channel->config_rsp_path == PRH_L2_REQUEST_RECEIVED);
    assert(p_channel->state == PRH_L2_CONFIG);

    L2_ConfigureResponse(loc_cid,newMTU, 0x00, 0);

    rem_tid++;

    assert(p_channel->state == PRH_L2_OPEN);
    assert(p_channel->config_rsp_path == PRH_L2_CONFIG_END);
    assert(p_channel->config_req_path == PRH_L2_CONFIG_END);

    assert(p_channel->inMTU == newMTU);
    assert(p_channel->outMTU == newMTU);


    printf("\n ***************************************************");  
    printf("\n CHANNEL RECONFIGURED FOR MTU OF  %x  ", newMTU );
    printf("\n ***************************************************\n");

}