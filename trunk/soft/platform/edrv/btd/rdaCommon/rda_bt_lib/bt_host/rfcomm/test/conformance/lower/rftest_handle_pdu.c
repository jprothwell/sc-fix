/***********************************************************************
 *
 * MODULE NAME:    rfcomm_test_gen.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:
 * MAINTAINER:     Dave Airlie
 * CREATION DATE: 
 *
 * SOURCE CONTROL: $Id: rftest_handle_pdu.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    02.Feb.2000 -   DA - first import
 *
 *
 * ISSUES:
 *
 ***********************************************************************/
#include "conformance_tests.h"

t_DataBuf *event_buf;
t_pEvent test_event;
int event_type;

int rftest_init_events(void)
{
  test_event=pEventCreate(FALSE);
}

int rftest_free_events(void)
{
  pEventFree(test_event);
}

int rftest_dumppdu(void)
{
  pEventWait(test_event);
  L2_FreeWriteBuffer(event_buf);
  event_type=0;
  return 0;
}

int rftest_takepdu(t_DataBuf **pdu_buf, int *pdu_type)
{
  pEventWait(test_event);
  *pdu_buf=event_buf;
  *pdu_type=event_type;
  return 0;
}

int rftest_gotpdu(t_DataBuf *p_buf, int pdutype)
{
  event_buf=p_buf;
  event_type=pdutype;
  pEventSet(test_event);
  return 0;
}
