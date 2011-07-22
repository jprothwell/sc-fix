/*
 * MODULE NAME:    mgr_inqdb.h
 * DESCRIPTION:    Host Manager Inquiry Database Header
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: mgr_inqdb.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies.
 *     All rights reserved.
 * 
 */

#ifndef __MGR_INQDB_H
#define __MGR_INQDB_H


typedef struct st_t_BT_DeviceEntry {
    /*
        WARNING::

        This structure MUST match that in user/manager.h and the 
        first 8 members of struct st_t_BT_InquiryInfo below.
        This structure is used only on the user API and represents
        a subset of the struct st_t_BT_InquiryInfo used internally.
        -MM
    */
  t_bdaddr address;
  u_int8 pageScanRepMode;
  u_int8 pageScanPeriodMode;
  u_int8 pageScanMode;
  u_int32 classOfDevice;
  u_int16 clockOffset;
  char *deviceName;
  u_int32 entryAge;
} t_MGR_DeviceEntry;


typedef struct st_t_BT_InquiryInfo {	
    /*
        WARNING::

        The order of the first 8 members of this structure MUST match 
        that of struct st_t_MGR_DeviceEntry above, and of that 
        in user/manager.h to allow applications access the 
        inquiry results without having access to the full declaration
        -MM
    */
    t_bdaddr bdAddress;
	u_int8 pageScanRepetitionMode;
	u_int8 pageScanPeriodMode;
	u_int8 pageScanMode;
	u_int32 classOfDevice;
	u_int16 clockOffset;
	char *deviceName;	/* always NUL terminated string, or NULL */
	u_int32 entryAge;	

    /* Only the above members visible on the API */
	u_int8	nameRetrieved;
    u_int8 deviceDiscoveryStatus;
    u_int8 entryUsage;
    struct st_t_BT_InquiryInfo *prev, *next;
} t_BT_InquiryInfo;

extern struct st_t_BT_InquiryInfo *prh_mgr_inqdb_head, *prh_mgr_inqdb_tail;
extern t_pMutex prh_mgr_inqdb_list_lock; 
extern u_int32 prh_mgr_inqdb_num_entries; 

void prh_init_inquiry_db(void);
void prh_free_inquiry_db(void);
void prh_mgr_inqdb_inquiry_complete(void);
struct st_t_BT_InquiryInfo *prh_mgr_store_inquiry_entry(u_int8 *inquiryResult);
struct st_t_BT_InquiryInfo *prh_mgr_retrieve_inquiry_entry(t_bdaddr address);
void prh_mgr_release_inquiry_entry(struct st_t_BT_InquiryInfo *entry);
void prh_mgr_cull_inquiry_db(void *unused);


APIDECL1 t_api APIDECL2 MGR_GetNextDeviceRecord(struct st_t_BT_DeviceEntry *current, struct st_t_BT_DeviceEntry **next);


#endif

