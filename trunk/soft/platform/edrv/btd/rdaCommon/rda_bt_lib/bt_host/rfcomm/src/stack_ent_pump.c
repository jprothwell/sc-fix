/***********************************************************************
 *
 * MODULE NAME:    stack_ent_pump.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Stack Entity code
 * MAINTAINER:     David Airlie
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: stack_ent_pump.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/
#include "rfcomm_lower_layer.h"

#if PRH_RFCOMM_RSE_DATA_PUMP_ENABLE==1
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_debug.h"
#include "rfcomm_proto.h"
#include "stack_ent_core.h"

t_pEvent flow_event;

t_api prh_rse_data_pump_flow_ind(t_bdaddr address, prh_t_rfcomm_dlci dlci, u_int8 flow_state)
{
  return BT_NOERROR;
}

t_api prh_rse_data_pump_data_ind(t_bdaddr address, prh_t_rfcomm_dlci dlci, struct host_buf *p_buf)
{
  /* Rx packets from pump on other side end up here */
  struct prh_rfcomm_se_user *se_info;
  int ret;
  
  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, address, dlci);
  if (ret==BT_NOERROR)
    {
      pMutexLock(se_info->rx_stats_lock);
      if (se_info->rx_traffic==0)
	se_info->rx_time_first = pExGetCurrentTicks();
      
      se_info->rx_traffic += p_buf->len;
      se_info->rx_pkt_count++;

      se_info->rx_time_last = pExGetCurrentTicks();
      pMutexUnlock(se_info->rx_stats_lock);
      RSE_FreeWriteBuffer(p_buf);
    }
}

t_api prh_rse_data_pump_start(struct prh_rfcomm_se_user *se_info, u_int8 pattern_num, u_int16 pkt_size, u_int32 num_packets)
{
  int i;
  char *test_pattern_buf;
  u_int16 count;
  char ch;
  struct host_buf *tx_buffer;
  int ret;

  /* pre-create the test pattern */
  test_pattern_buf = (char *)pMalloc(pkt_size);

  ch='a';
  for (count=0; count<pkt_size; count++)
    {
      test_pattern_buf[count]=ch++;
    }
  
  for (count=0; count<num_packets; count++)
    {
      do{
	ret=RSE_GetWriteBuffer(se_info->remote_device_addr, se_info->dlci, pkt_size, &tx_buffer);
      } while(ret==BT_FLOWCTRL);
      
      if (ret==BT_NOERROR)
	{
	  pMemcpy(tx_buffer->data, test_pattern_buf, pkt_size);
	  tx_buffer->len = pkt_size;

	  /* attempt to send */
	  do {
	    ret=RSE_Write(se_info->remote_device_addr, se_info->dlci, tx_buffer);
	  } while(ret==BT_FLOWCTRL);
	  
	}

    }
  return BT_NOERROR;
}


#endif
