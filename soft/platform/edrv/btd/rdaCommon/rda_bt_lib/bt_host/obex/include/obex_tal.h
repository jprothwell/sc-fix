/****************************************************************
 *
 * MODULE NAME:    obex_tal.h
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    transport abstraction layer
 * MAINTAINER:     Mark Donnelly, kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_tal.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/


struct prh_obex_transport{
  u_int32 tid;
  u_int16 max_transport;
  u_int16 max_obex;
  u_int16 length;
  u_int8* rx_buffer;
  u_int8 type;
  t_pEvent connect_wait;
  union {
    struct {
      t_bdaddr address;
      u_int8 server_channel;
      u_int8 flow_state;
      u_int8 dlci;
      void (*disconnect)(u_int32 tid);
    }rfcomm;
    struct {
      u_int32 port;
    }serial;
    struct {
      u_int32 socket;
    }socket;
    struct {
      char* url;
    }gen;
  }use;
  struct prh_obex_transport* p_next;
};


/*int OBEX_TransportListen_Serial(port);
  int OBEX_TransportListen_Socket(port);*/



int prh_obex_transport_tx(u_int32 tid, t_DataBuf* txbuffer);
u_int32 prh_obex_allocate_tid(void);
void prh_obex_clean_tid(void);
#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.2.20
t_api prh_obex_tal_rfcomm_reg_port(u_int8 server_channel);
#else
t_api prh_obex_tal_rfcomm_reg_port(u_int8 server_channel,u_int16 max_obex,u_int8* rx_buffer,void (*authorizecallbackFunc)(t_bdaddr address, u_int8 dlci, u_int8 schan, u_int8 tid_s));
#endif

t_api prh_obex_tal_rfcomm_connect_port(t_bdaddr bdaddr, u_int8 schan, u_int16 mtu);
int prh_obex_create_client_callback(u_int32 tid, t_obex_command_handler handler);
void prh_obex_tal_remove_transport(struct prh_obex_transport* target);

