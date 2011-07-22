/***********************************************************************
 *
 * MODULE NAME:    rfcomm_buffer.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM Macros for allocating buffers at differnet levels
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  15 Oct 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_buffer.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/
#ifndef PRH_RFCOMM_BUFFER_H
#define PRH_RFCOMM_BUFFER_H

/* 4 bytes for HCI, 4 bytes for L2CAP max */
#define PRH_RFCOMM_BUF_LOWER_LAYER_HEADERS 8

/* 1 byte for RFCOMM FCS */
#define PRH_RFCOMM_BUF_FCS 1

/* Define UIH Data Headers */
#define PRH_RFCOMM_BUF_UIH_SHORT 3
#define PRH_RFCOMM_BUF_UIH_LONG 4

/* Control Channel Header (length + message type) */
#define PRH_RFCOMM_BUF_CHAN_HEAD 3

#define PRH_RFCOMM_BUF_CONTROL_HEADERS PRH_RFCOMM_BUF_UIH_LONG

#define PRH_RFCOMM_GET_L2_FRAME(cid, length, buffer) (prh_l2_GetWriteBuffer(cid, (u_int16)(length+PRH_RFCOMM_BUF_FCS), PRH_HOST_BUF_USE_RESV_ACL_MEM, buffer))

#define PRH_RFCOMM_GET_L2_DATA_FRAME(cid, length, buffer) (prh_l2_GetWriteBuffer(cid, (u_int16)(length+PRH_RFCOMM_BUF_FCS), 0, buffer))

#define PRH_RFCOMM_GET_L2_CONTROL_FRAME(cid, length, buffer) (prh_l2_GetWriteBuffer(cid, (u_int16)(length+PRH_RFCOMM_BUF_CHAN_HEAD+PRH_RFCOMM_BUF_UIH_LONG+PRH_RFCOMM_BUF_FCS), PRH_HOST_BUF_USE_RESV_ACL_MEM, buffer))


#endif
