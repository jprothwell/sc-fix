/***********************************************************************
 *
 * MODULE NAME:    host_buf.h
 * DESCRIPTION:    Host System buffers
 * MAINTAINER:     Dave Airlie
 *
 * SOURCE CONTROL: $Id: host_buf.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies.
 *     All rights reserved.
 *
 ***********************************************************************/
#ifndef PRH_HOST_BUF_H
#define PRH_HOST_BUF_H


#include "host_config.h"
#include "host_types.h"


struct host_buf 
{

	/*
		IMPORTANT:
		The first three members of the structure should be data, len, and totalsize
		in that order. The user-level headers see a structure with only these members
		present. 
        
        This structure should be a multiple of 4 bytes unless host_buf_alloc is updated 
        
          -MM
	*/


  /* len,data and next are public attributes and can be directly manipulated
   * by the user, they are also manipulated using the reserve and release
   *  header routines 
   */

  /*
   * len stores the current length of buffer 
   * data pointer to the current start of data 
   * - moved using the reserve_header and release_header
   */
   u_int8 *data;
   u_int32 len;

#if COMBINED_HOST==1
  u_int16 link_handle;
  u_int8 flags;
#else
  /* Below this is private attributes and should only be manipulated 
     through the service interface */
  /*
   * Total size of buffer 
   * Start and end of buffer pointers
   * - fixed at allocation 
   */
  u_int32 totalsize;
  u_int8 *head;
  u_int8 *tail;

   /* linked list ability - flow control can use this */
  struct host_buf *next;

 /* parent buffer and child counter */
  struct host_buf *parent;
  u_int16 child_count;

  /* the header is present in the PDU 
     otherwise build the header from the 4 bytes here on tx */
  u_int16 flags; 
  u_int16 hci_handle_flags;
  u_int16 hci_length;
#endif  
};


/* Functions */
struct host_buf * host_buf_alloc(u_int32 size);
struct host_buf * host_buf_child_alloc(struct host_buf *parent, u_int8 *new_data, u_int32 length);
void host_buf_free(struct host_buf *buf);
t_api host_buf_reserve_header(struct host_buf *buf, u_int32 size);
t_api host_buf_release_header(struct host_buf *buf, u_int32 size);

#if COMBINED_HOST==1
/* get a write buffer once we have a handle */
t_api host_buf_GetWriteBuffer(u_int16 handle, u_int16 length, u_int32 flags, struct host_buf **p_buf);
t_api host_buf_FreeWriteBuffer(struct host_buf *p_buf);

/* copy data to a2dp queue */
t_api host_buf_CopyToA2dpBuffer(struct host_buf *p_buf);
#endif  

#define DEFAULT_HOST_BUF_SIZE 127

/* header sizes to reserve at each layer */

#define PRH_HOST_BUF_HCI_HEADER_PRESENT 0x0001
#define PRH_HOST_BUF_USE_RESV_ACL_MEM   0x0002
/* is this packet a packet allocated on the way out or way in? */
/* 1 is a packet from the queues */
#define PRH_HOST_BUF_OUTGOING_PACKET    0x0004

#define PRH_HOST_BUF_PICONET_BROADCAST  0x0008
#define PRH_HOST_BUF_ACTIVE_BROADCAST   0x0010

#endif
