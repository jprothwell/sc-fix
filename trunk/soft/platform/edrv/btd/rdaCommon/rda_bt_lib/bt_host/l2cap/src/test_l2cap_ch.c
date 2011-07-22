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
#include "l2cap_ch.h"
#include "l2cap_const.h"
/* #include "l2cap_types.h" */
#include "test_l2cap_ch.h"
#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"
/*#include "test_utils.h"*/
#include <assert.h>


/* Handlers for L2CAP events  - i.e. events on the service interface 
 * to the L2CAP protocol.
 */

/*void *__rt_embeddedalloc_init(void* base, size_t size); */

void _Test_L2CAP_Channel_Setup_No_LMP(void);
void _Test_L2CAP_Channel_Setup_With_LMP(void);
/* void _Set_Bd_Addr(t_bd_addr* bd_addr,t_bd_addr* new_addr); */
void L2CAP_Queue_Copy(u_int16 src,u_int16 dest);
void BTms_Sched(u_int8 interations);
/*
void L2CA_Data_Report(u_int16 cid,struct host_buf* p_buf);
*/

static t_error L2_Connect_Ind(t_l2ca_event_info* event_info);
static t_error L2_Config_Ind(t_l2ca_event_info* event_info);
static t_error L2_Disconnect_Ind(t_l2ca_event_info* event_info);
static t_error L2_Disconnect_Cfm(t_l2ca_event_info* event_info);
static t_error L2_QoSViolation_Ind(t_l2ca_event_info* event_info);

static t_error L2_Connect_Cfm(t_l2ca_event_info* event_info);
static t_error L2_Config_Cfm(t_l2ca_event_info* event_info);

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

t_error L2_Connect_Ind(t_l2ca_event_info* event_info)
{
   t_l2ca_connect_ind*  conIndInfo;
   t_l2ca_connect_rsp  conRspInfo;
   t_l2ca_config_req  confReqInfo;

   conIndInfo = (t_l2ca_connect_ind*)event_info;
   
   assert( W4_L2CA_CONNECT_RSP == L2CA_Get_State(Master_LinkId));
   conRspInfo.LCID = conIndInfo->LCID;

   conRspInfo.bd_addr = conIndInfo->bd_addr;
 /*  conRspInfo.bd_addr = conIndInfo->bd_addr;
   conRspInfo.bd_addr = conIndInfo->bd_addr; */
   conRspInfo.identifier = conIndInfo->identifier;
   conRspInfo.result = 0;
   conRspInfo.status = 0;

   
   L2CA_Connect_Rsp(&conRspInfo);
   /*assert( CONFIG == L2CA_Get_State(Slave_LinkId));*/

   confReqInfo.LCID = conRspInfo.LCID;
   confReqInfo.inMTU = 0x48;
   confReqInfo.flushTO = 0x100;
   confReqInfo.linkTO = 0x200;

   L2CA_Config_Req(&confReqInfo);

   return E_NO_ERROR;
}

static t_error L2_Config_Ind(t_l2ca_event_info* event_info)
{
   t_l2ca_config_ind*  confIndInfo;
   t_l2ca_config_rsp   confRspInfo;

   confIndInfo = (t_l2ca_config_ind*)event_info;

   confRspInfo.LCID = confIndInfo->LCID;
   confRspInfo.outMTU = confIndInfo->outMTU;
   confRspInfo.result = 0x00;

   L2CA_Config_Rsp(&confRspInfo);

   return E_NO_ERROR;
}


static t_error  L2_Disconnect_Ind(t_l2ca_event_info* p_event_info)
{
    t_error status;
    t_l2ca_disconnect_ind*  p_discon_ind_info;
    t_l2ca_disconnect_rsp  discon_rsp_info;

    p_discon_ind_info = (t_l2ca_disconnect_ind*)p_event_info;

    discon_rsp_info.LCID = p_discon_ind_info->LCID;
    status = L2CA_Disconnect_Rsp(&discon_rsp_info);

    return status;
}

static t_error L2_QoSViolation_Ind(t_l2ca_event_info* event_info)
{
   return E_NO_ERROR;
}


static t_error L2_Connect_Cfm(t_l2ca_event_info* event_info)
{
   t_l2ca_connect_cfm*  conCfmInfo;
   t_l2ca_config_req  confReqInfo;

   conCfmInfo = (t_l2ca_connect_cfm*)event_info;

   confReqInfo.LCID = conCfmInfo->LCID;
   confReqInfo.inMTU = 0x48;
   confReqInfo.flushTO = 0x100;
   confReqInfo.linkTO = 0x200;
   
   L2CA_Config_Req(&confReqInfo);

   return E_NO_ERROR;
}

static t_error L2_Config_Cfm(t_l2ca_event_info* event_info)
{
   t_l2ca_config_cfm*  confCfmInfo;

   confCfmInfo = (t_l2ca_config_cfm*)event_info;

   return E_NO_ERROR;
}


static t_error L2_Disconnect_Cfm(t_l2ca_event_info* event_info)
{
   /* t_l2ca_disconnect_cfm*  disconCfmInfo = (t_l2ca_disconnect_cfm*)event_info; */

   return E_NO_ERROR;
}

static t_error L2_Echo_Cfm(t_l2ca_event_info* event_info)
{
   return E_NO_ERROR;
}

void init_l2ca_callback()
{
   L2CA_EventInd(CB_L2CA_CONNECT_IND, &L2_Connect_Ind);
   L2CA_EventInd(CB_L2CA_CONFIG_IND, &L2_Config_Ind);
   L2CA_EventInd(CB_L2CA_DISCONNECT_IND, &L2_Disconnect_Ind);
   L2CA_EventInd(CB_L2CA_QoSVIOLATION_IND, &L2_QoSViolation_Ind);
   L2CA_EventInd(CB_L2CA_CONNECT_CFM, &L2_Connect_Cfm);
   L2CA_EventInd(CB_L2CA_CONFIG_CFM, &L2_Config_Cfm);
   L2CA_EventInd(CB_L2CA_DISCONNECT_CFM, &L2_Disconnect_Cfm);
   L2CA_EventInd(CB_L2CA_ECHO_CFM, &L2_Echo_Cfm);
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
   t_error result=E_NO_ERROR;
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

   LP_Connect_Ind(&master_bd_addr,Slave_Handle);

   /* LP_Connect_Cfm(&slave_bd_addr,result,Master_Handle);*/ /* Handle = 0x0123 from the HC */

   L2CA_Data_Ind(0x0345,connect_req_pdu,0x04,1);
   L2CA_Data_Ind(0x0345,connect_req_pdu+4,0x04,0);
   L2CA_Data_Ind(0x0345,connect_req_pdu+8,0x04,0);

 /*  assert(OPEN == L2CA_Get_State(Master_LinkId)); */
   assert(OPEN == L2CA_Get_State(Slave_LinkId));


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

#if 0

   t_l2ca_connect_req  conReqInfo;
   t_l2ca_ping_req echoReqInfo;
   u_int8  data_field[20];

   echoReqInfo.data = data_field;
   echoReqInfo.length = 15;

   _Set_Bd_Addr(&conReqInfo.bd_addr,&master_bd_addr );
   
   L2CA_Connect_Req(&conReqInfo);

   assert( W4_L2CAP_CONNECT_RSP == L2CA_Get_State(Master_LinkId));
   L2CAP_Queue_Copy(Master_LinkId,Slave_LinkId);

   BTms_Sched(100);

   assert( CONFIG == L2CA_Get_State(Slave_LinkId));

   L2CAP_Queue_Copy(Slave_LinkId,Master_LinkId); 
   L2CAP_Queue_Copy(Slave_LinkId,Master_LinkId); 
   BTms_Sched(100);

   L2CAP_Queue_Copy(Master_LinkId,Slave_LinkId);
   BTms_Sched(100);

   L2CAP_Queue_Copy(Slave_LinkId,Master_LinkId); 
   BTms_Sched(100);

   L2CAP_Queue_Copy(Master_LinkId,Slave_LinkId);
   BTms_Sched(100);

   assert(OPEN == L2CA_Get_State(Master_LinkId));
   assert(OPEN == L2CA_Get_State(Slave_LinkId));

   _Set_Bd_Addr(echoReqInfo.bd_addr,master_bd_addr);
   echoReqInfo.length = 10;

   L2CA_Ping_Req(&echoReqInfo);
   L2CAP_Queue_Copy(Master_LinkId,Slave_LinkId);
   BTms_Sched(100);

   L2CAP_Queue_Copy(Slave_LinkId,Master_LinkId);
   BTms_Sched(100);

   disconReqInfo.LCID = 0x3f;

   L2CA_Disconnect_Req(&disconReqInfo);

   L2CAP_Queue_Copy(Master_LinkId,Slave_LinkId);

   BTms_Sched(100);

   L2CAP_Queue_Copy(Slave_LinkId,Master_LinkId);
   BTms_Sched(100);

   assert(CLOSED == L2CA_Get_State(Master_LinkId));
   assert(CLOSED == L2CA_Get_State(Slave_LinkId));

#endif 
}

#if 0
void _Set_Bd_Addr(t_bd_addr* my_bd_addr,t_bd_addr* new_addr)
{
    int i;

    for(i=0;i<6;i++)
        my_bd_addr->bytes[i] = new_addr->bytes[i];
}

#endif

void L2CAP_Queue_Copy(u_int16 src,u_int16 dest)
{

   ;
}


void BTms_Sched(u_int8 interations)
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

int main(void)
{

  /*  __rt_embeddedalloc_init((void*)0x20000, 0x2000);*/

    Test_L2CAP_Connection_Handler();
    return 0;
}
