#include <stdio.h>
#include "host_config.h"
#include "host_types.h"
#include "host_error.h"
#include "test/unit_stub_l2cap.h"
#include "papi.h"

#define MAX_STUB_PSMS 4
#define MAX_STUB_CONNECTIONS 8

struct st_t_prh_tcs_unit_l2cap_psm {
  u_int8 used;
  u_int16 psm;
  l2_callbacks callbacks;
};

static struct st_t_prh_tcs_unit_l2cap_psm stub_psm_list[MAX_STUB_PSMS];

struct st_t_prh_tcs_unit_l2cap_channel {
  u_int8 used;
  u_int16 cid;
  t_bdaddr bdaddr;
  struct st_t_prh_tcs_unit_l2cap_psm *psm_ptr;
  u_int16 inmtu, outmtu;
  u_int8 initiator;
};
struct st_t_prh_tcs_unit_l2cap_channel *last_request;

static struct st_t_prh_tcs_unit_l2cap_channel stub_chan_list[MAX_STUB_CONNECTIONS];

t_api UNIT_STUB_RESET_L2(void)
{
  int i;

  for (i=0; i<MAX_STUB_PSMS; i++)
    pMemset(&stub_psm_list[i], 0, sizeof(struct st_t_prh_tcs_unit_l2cap_psm));


  for (i=0; i<MAX_STUB_CONNECTIONS; i++)
    pMemset(&stub_chan_list[i], 0, sizeof(struct st_t_prh_tcs_unit_l2cap_channel));
  
  return BT_NOERROR;
}

t_api UNIT_STUB_L2_RegisterPSM(u_int16 psm,  l2_callbacks *callbacks) 
{
  int i;

  for (i=0; i<MAX_STUB_PSMS; i++)
    if (stub_psm_list[i].used==0)
      break;

  if (i<MAX_STUB_PSMS)
    {
      stub_psm_list[i].used=1;
      stub_psm_list[i].psm=psm;
      stub_psm_list[i].callbacks = *callbacks;
      fprintf(stderr,"L2CAP register PSM called assigned slot %d to psm %d\n", i, psm);
      return BT_NOERROR;
    }
  else
    return BT_NORESOURCES;


}

t_api UNIT_STUB_L2_DeregisterPSM(u_int16 psm)
{
  int i;
  fprintf(stderr,"L2CAP deregister PSM called %d\n", psm);
  for (i=0; i<MAX_STUB_PSMS; i++)
    if (stub_psm_list[i].used==1)
      if (stub_psm_list[i].psm==psm)
	stub_psm_list[i].used=0;

  return BT_NOERROR;
}

t_api UNIT_STUB_L2_Connect(t_bdaddr bdaddr, u_int16 psm) 
{
  int i, j;
  t_L2_ControlCallbackArgs args;

  fprintf(stderr,"L2CAP Connect called\n");

  for (i=0; i<MAX_STUB_PSMS; i++)
    if (stub_psm_list[i].used==1)
      if (stub_psm_list[i].psm==psm)
	break;

  if (i<MAX_STUB_PSMS)
    {
      for (j=0; j<MAX_STUB_CONNECTIONS; j++)
	  if (stub_chan_list[j].used==0)
	    break;
      if (j<MAX_STUB_CONNECTIONS)
	{
	  stub_chan_list[j].used=1;
	  stub_chan_list[j].bdaddr=bdaddr;
	  stub_chan_list[j].cid=0x40+j;
	  stub_chan_list[j].initiator=1;
	  stub_chan_list[j].psm_ptr=&stub_psm_list[i];
	  last_request=&stub_chan_list[j];
	}
    }
  
    /* got L2CAP connect */
  return BT_PENDING;
}

t_api RFCOMMLT_GetLastConnectHandle(void **conn_handle)
{
  *conn_handle=last_request;
  last_request=NULL;
  return BT_NOERROR;
}

t_api RFCOMMLT_Send_ConnectConfirm(void *conn_handle)
{
  t_L2_ControlCallbackArgs args;
  struct st_t_prh_tcs_unit_l2cap_channel *chan_handle=(struct st_t_prh_tcs_unit_l2cap_channel *)conn_handle;  
  args.cid=chan_handle->cid;
  args.BDAddress=chan_handle->bdaddr;
  args.psm=chan_handle->psm_ptr->psm;
  args.result=0;
  chan_handle->psm_ptr->callbacks.connectionConfirmation(L2CAP_evCONNECTCONF, &args);	
  return BT_NOERROR;
}

t_api UNIT_STUB_L2_ConnectResponse(t_bdaddr bdaddr, u_int16 cid, u_int16 result, u_int16 status)
{
  fprintf(stderr,"L2CAP conn resp%d\n", cid);
}


t_api UNIT_STUB_L2_Write(u_int16 cid, t_DataBuf *writeBuffer) 
{
  fprintf(stderr, "Got %d bytes to write\n", writeBuffer->dataLen);
 #if pDEBUG
  pDebugDumpFunc(pLOGNOTICE, writeBuffer->buf, writeBuffer->dataLen);
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
  SYSDebug_Dump_FuncEX(pLOGNOTICE, pLOGALWAYS,writeBuffer->buf, writeBuffer->dataLen);
#endif
  rftest_gotpdu(writeBuffer, 0);
}

t_api UNIT_STUB_L2_Disconnect(u_int16 cid)
{


}

t_api UNIT_STUB_L2_Configure(u_int16 cid, u_int16 inMTU, u_int16 outFlushTimeout, u_int16 linkTimeout, t_L2_ChannelFlowSpec *outFlow)
{
  t_L2_ControlCallbackArgs args;
  int j;

  fprintf(stderr,"L2CAP configure called %d\n", cid);
  for (j=0; j<MAX_STUB_CONNECTIONS; j++)
    if (stub_chan_list[j].used==1)
      if (stub_chan_list[j].cid==cid)
	break;

  if (j>=MAX_STUB_CONNECTIONS)
        return BT_INVALIDPARAM;
  
  stub_chan_list[j].inmtu=inMTU;
  return BT_NOERROR;
}


void RFCOMMLT_Send_ConfigureIndication(void *conn_handle)
{
  t_L2_ControlCallbackArgs args;
  struct st_t_prh_tcs_unit_l2cap_channel *chan_handle=(struct st_t_prh_tcs_unit_l2cap_channel *)conn_handle;    

  pMemset(&args, 0, sizeof(t_L2_ControlCallbackArgs));
  args.cid=chan_handle->cid;
  args.mtu=chan_handle->inmtu;

  if (chan_handle->psm_ptr->callbacks.configureIndication)
    chan_handle->psm_ptr->callbacks.configureIndication(L2CAP_evCONFIGUREIND, &args);
}

void RFCOMMLT_Send_ConfigureComplete(void *conn_handle)
{
  t_L2_ControlCallbackArgs args;
  struct st_t_prh_tcs_unit_l2cap_channel *chan_handle=(struct st_t_prh_tcs_unit_l2cap_channel *)conn_handle;  

  pMemset(&args, 0, sizeof(t_L2_ControlCallbackArgs));

  args.cid=chan_handle->cid;
  args.mtu=chan_handle->inmtu;

  if (chan_handle->psm_ptr->callbacks.configureConfirmation)
    chan_handle->psm_ptr->callbacks.configureConfirmation(L2CAP_evCONFIGURECONF, &args);
}

t_api UNIT_STUB_L2_ConfigureResponse(u_int16 cid, u_int16 outMTU, u_int16 result, t_L2_ChannelFlowSpec *inFlow)
{
  fprintf(stderr,"L2CAP configure response %d\n", cid);
  return BT_NOERROR;
}


t_api UNIT_STUB_Data_From_L2(void *conn_handle, struct host_buf *pdu_buf)
{
  struct st_t_prh_tcs_unit_l2cap_channel *chan_handle=(struct st_t_prh_tcs_unit_l2cap_channel *)conn_handle;  

  pDebugDumpFunc(pLOGNOTICE, pdu_buf->data, pdu_buf->len);  
  if (chan_handle->psm_ptr->callbacks.dataIndication)
    chan_handle->psm_ptr->callbacks.dataIndication(chan_handle->cid, pdu_buf);

}

t_api RFCOMM_UNIT_GetWriteBuffer(u_int32 length, struct host_buf **frame)
{
  *frame=host_buf_alloc(length);
  return BT_NOERROR;
}

t_api rftest_get_crbit(void *conn_handle)
{
  struct st_t_prh_tcs_unit_l2cap_channel *chan_handle=(struct st_t_prh_tcs_unit_l2cap_channel *)conn_handle;  

  return chan_handle->initiator;

}

t_api prh_stub_do_connectind(u_int16 psm, t_bdaddr address, u_int16 cid, void **conn_handle)
{
  int i,j;
  t_L2_ControlCallbackArgs args;

  for (i=0; i<MAX_STUB_PSMS; i++)
    if (stub_psm_list[i].used==1)
      if (stub_psm_list[i].psm==psm)
	break;
  
  if (i<MAX_STUB_PSMS)
    {
      for (j=0; j<MAX_STUB_CONNECTIONS; j++)
	if (stub_chan_list[j].used==0)
	  break;
      if (j<MAX_STUB_CONNECTIONS)
	{
	  stub_chan_list[j].used=1;
	  stub_chan_list[j].bdaddr=address;
	  stub_chan_list[j].cid=cid;
	  stub_chan_list[j].initiator=0;
	  stub_chan_list[j].psm_ptr=&stub_psm_list[i];

	  args.BDAddress=address;
	  args.psm=psm;
	  args.cid=cid;
	  
	  *conn_handle=&stub_chan_list[j];
	  if (stub_psm_list[i].callbacks.connectionIndication)
	    stub_psm_list[i].callbacks.connectionIndication(L2CAP_evCONNECTIND, &args);
	  return BT_NOERROR;
	}
    }
  return BT_INVALIDPARAM;  
}

u_int16 prh_stub_l2cap_setup_l2_connection(t_bdaddr remote_dev, u_int16 psm, u_int16 cid, void **conn_handle_ptr)
{
  void *conn_handle;
  int ret;
  ret=prh_stub_do_connectind(3, remote_dev, 64, &conn_handle);
  if (ret==BT_NOERROR)
    {
      RFCOMMLT_Send_ConfigureIndication(conn_handle);
      RFCOMMLT_Send_ConfigureComplete(conn_handle);  
      *conn_handle_ptr=conn_handle;
    }
  else
    *conn_handle_ptr=NULL;
  return ret;

}

int prh_stub_l2cap_accept_l2_connection(void **conn_handle_ptr)
{
  void *conn_handle;
  
  RFCOMMLT_GetLastConnectHandle(&conn_handle);
  /* feed in a connect response */
  
  RFCOMMLT_Send_ConnectConfirm(conn_handle);
  
  RFCOMMLT_Send_ConfigureIndication(conn_handle);
  RFCOMMLT_Send_ConfigureComplete(conn_handle);
  
  *conn_handle_ptr=conn_handle;
  return 0;
}
