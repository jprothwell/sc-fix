 /*
 * MODULE NAME:    mgr_inqdb.c
 * DESCRIPTION:    Host Manager Inquiry Database Functionality
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: mgr_inqdb.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 * 
 */


#include "host_types.h"
#include "host_config.h"
#include "host_error.h"
#include "papi.h"
#include "mgr_utils.h"
#include "mgr_inqdb.h"


struct st_t_BT_InquiryInfo *prh_mgr_inqdb_head, *prh_mgr_inqdb_tail;
t_pMutex prh_mgr_inqdb_list_lock; 
u_int32 prh_mgr_inqdb_num_entries; 
t_pTimer prh_mgr_inqdb_timer_handle;

extern u_int8 prh_mgr_discoveryInProgress;
/*************************************************************************************************
* FUNCTION :- prh_init_inquiry_db
*
* DESCRIPTION :- Initialise the inquiry database (inq db)
*                 
************************************************************************************************/
void prh_init_inquiry_db(void) {
    prh_mgr_inqdb_head=NULL;
    prh_mgr_inqdb_tail=NULL;
    prh_mgr_inqdb_num_entries=0;
    prh_mgr_inqdb_list_lock=pMutexCreate(FALSE);
    prh_mgr_inqdb_timer_handle=0; 
}
/*************************************************************************************************
* FUNCTION :- prh_free_inquiry_db
*
* DESCRIPTION :- This function frees the inq db and all of its entries
*                 
************************************************************************************************/
void prh_free_inquiry_db(void) {
    struct st_t_BT_InquiryInfo *current, *next;

    /* cancel timer if it is set - no race since handler aborts if prh_mgr_inqdb_num_entries==0 */
    pMutexLock(prh_mgr_inqdb_list_lock);
    prh_mgr_inqdb_num_entries=0;
    pMutexUnlock(prh_mgr_inqdb_list_lock);
    //pTimerCancel(prh_mgr_inqdb_timer_handle);

    pMutexFree(prh_mgr_inqdb_list_lock);

    current=prh_mgr_inqdb_head;

    while (current) {
#if pDEBUG
        if (current->entryUsage)
            pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"prh_mgr_free_inquiry_db: freeing in-use record at 0x%p\n",current));
#endif
        next=current->next;
        if (current->deviceName)
            pFree(current->deviceName);
        pFree(current);
        current=next;        
    }
    prh_mgr_inqdb_head = NULL;
	pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"free inquiry db finished...................................\n",current));

}
/*************************************************************************************************
* FUNCTION :- prh_mgr_unlink_inquiry_entry
*
* DESCRIPTION :- This function allows the removal of the structure from the linked list, 
*                 but does not free it 
*                 
************************************************************************************************/
void prh_mgr_unlink_inquiry_entry(struct st_t_BT_InquiryInfo *entry) {
    /* remove the record from the list */ 
    if (entry->prev) 
        entry->prev->next=entry->next;
    else 
        prh_mgr_inqdb_head=entry->next; 

    if (entry->next) 
        entry->next->prev=entry->prev;
    else 
        prh_mgr_inqdb_tail=entry->prev; 

}
/*************************************************************************************************
* FUNCTION :- prh_mgr_store_inquiry_entry
*
* DESCRIPTION :- This function stores an inquiry result in the inq db. If the device is already 
*                 in the database the parameters will be updates otherwise a new entry will be
*                 added to the inq db.
*                 NOTE:inquiryresult passed in is one set of page parameters, there may be many 
*                 in a single hci event                          
*                 
************************************************************************************************/
struct st_t_BT_InquiryInfo *prh_mgr_store_inquiry_entry(u_int8 *inquiryResult) 
{
    struct st_t_BT_InquiryInfo *current;
    
	extern u_int32 prh_mgr_inqdb_num_entries;
//    char err_str[40]={0};


	if(inquiryResult == NULL)
		return  NULL;
	
    pMutexLock(prh_mgr_inqdb_list_lock);

	current=prh_mgr_inqdb_head;

	pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_store_inquiry_entry: linklist is NULL? %d\n", prh_mgr_inqdb_head?1:0));

	
    while (current) 
    {
        if (BD_ADDR_CMP(current->bdAddress,*(t_bdaddr*)(inquiryResult+0))) /* check alignment correctness here */
            break;
        current=current->next;
    }
#if pDEBUG
    if (!current) 
    {
        pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_store_inquiry_entry: cannot find existing entry for device %s\n",prh_mgr_printBDAddress(*(t_bdaddr*)(inquiryResult+0))));
    } 
#endif

    if (current) 
   {
        if(current->entryUsage)
	{
            /* entry already exists, but is in use, so cannot be updated */
	    pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_store_inquiry_entry: cannot find existing entry for device %s\n",prh_mgr_printBDAddress(*(t_bdaddr*)(inquiryResult+0))));

            pMutexUnlock(prh_mgr_inqdb_list_lock);
            return current;
        }
	else 
	{
            prh_mgr_unlink_inquiry_entry(current);
        }
    } 
   else 
   {  
        pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_store_inquiry_entry: prh_mgr_inqdb_num_entries=%d\n", prh_mgr_inqdb_num_entries));
        if (prh_mgr_inqdb_num_entries<(u_int32)HOST_INQDB_MAX_DEVICE_ENTRIES) 
        {
            /* alloc a new record */
            current=pMalloc(sizeof(struct st_t_BT_InquiryInfo));
            if (!current) 
            {
                pMutexUnlock(prh_mgr_inqdb_list_lock);
                return NULL;
            }
			current->deviceName = NULL;

#if HOST_INQDB_CULL_THRESHOLD!=0
            if (!prh_mgr_inqdb_num_entries)  /* we are adding to an empty database */
                prh_mgr_inqdb_timer_handle=pTimerCreate(HOST_INQDB_CULL_GRANULARITY,prh_mgr_cull_inquiry_db,NULL,pTIMER_ONESHOT);
#endif

            prh_mgr_inqdb_num_entries++;
        } 
        else 
        {
            /* find the oldest non-inuse record */        
            current=prh_mgr_inqdb_tail;
            while (current) 
            {
                if (current->entryUsage)
                    current=current->prev;
                else
                    break;
            }

            if (!current) 
            {
                /* no available entry */
                pMutexUnlock(prh_mgr_inqdb_list_lock);
                pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"prh_mgr_store_inquiry_entry: prh_mgr_inqdb_num_entries==HOST_INQDB_MAX_DEVICE_ENTRIES (%d) and  all entries in use\n",HOST_INQDB_MAX_DEVICE_ENTRIES));
                return NULL;
            }
            prh_mgr_unlink_inquiry_entry(current);
        }

        current->entryAge=1; /* only modified when an entry did not already exist for this adress */
		current->nameRetrieved = 0;
    }
    
    //Lu added
    

    /* fill out the parameters */
    pMemcpy(&current->bdAddress,inquiryResult+0,6);
    current->pageScanRepetitionMode=inquiryResult[6];
    current->pageScanPeriodMode=inquiryResult[7];
    current->pageScanMode=inquiryResult[8];
    current->classOfDevice=inquiryResult[9]|(inquiryResult[10]<<8)|(inquiryResult[11]<<16);
    current->clockOffset=inquiryResult[12]|(inquiryResult[13]<<8);

	/* Do not fill in current->deviceName until RemoteNameRequest is complete */

    current->entryUsage=1; /* entry starts marked as 'in-use' */

    /* add this new record (to the head of the list) */
    current->next=prh_mgr_inqdb_head;
    current->prev=NULL;
    if (current->next)
        current->next->prev=current;
    prh_mgr_inqdb_head=current;   
    if (!prh_mgr_inqdb_tail)
        prh_mgr_inqdb_tail=current;

    pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_store_inquiry_entry: current->entryAge=%d, current->classOfDevice=0x%x\n", current->entryAge,current->classOfDevice));


    pMutexUnlock(prh_mgr_inqdb_list_lock);
    return current;
}
/*************************************************************************************************
* FUNCTION :- prh_mgr_retrieve_inquiry_entry
*
* DESCRIPTION :- This function searches the inq db for an entry by the BDAddress. If a device
*                 is found with a matching BDAddress, it is returned, otherwise NULL is returned.
*                 
************************************************************************************************/
struct st_t_BT_InquiryInfo *prh_mgr_retrieve_inquiry_entry(t_bdaddr address) {
    struct st_t_BT_InquiryInfo *current;

    current=prh_mgr_inqdb_head;

    while (current) {
        if (BD_ADDR_CMP(address,current->bdAddress))
            break;
        current=current->next;
    }
    
    if (current) {  
        pMutexLock(prh_mgr_inqdb_list_lock);
        current->entryUsage++;
        pMutexUnlock(prh_mgr_inqdb_list_lock);
    }
#if pDEBUG
    else {
        pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_retrieve_inquiry_entry: cannot find entry for device %s\n",prh_mgr_printBDAddress(address)));
    }
#endif

    return current;
}
/*************************************************************************************************
* FUNCTION :- prh_mgr_inqdb_inquiry_complete
*
* DESCRIPTION :- This function Ages all the entries currently in the inq db which have not 
*                 previously been aged.
*                 
************************************************************************************************/
void prh_mgr_inqdb_inquiry_complete(void) {
    struct st_t_BT_InquiryInfo *current;
    current=prh_mgr_inqdb_head;
    pMutexLock(prh_mgr_inqdb_list_lock);
    while (current) {
        if (current->entryAge==0)
            current->entryAge=1;
        current=current->next;
    }
    pMutexUnlock(prh_mgr_inqdb_list_lock);
}

/*************************************************************************************************
* FUNCTION :- prh_mgr_release_inquiry_entry
*
* DESCRIPTION :- After a process is finished using an inq db entry it must call this function to
*                 indicate that it is not using it anymore.
*                 
************************************************************************************************/
void prh_mgr_release_inquiry_entry(struct st_t_BT_InquiryInfo *entry) {
    if (entry) {
        pMutexLock(prh_mgr_inqdb_list_lock);
        if (entry->entryUsage>0)
            entry->entryUsage--;
#if pDEBUG
        else
            pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"prh_mgr_release_inquiry_entry: reference count for device %s is already zero\n",prh_mgr_printBDAddress(entry->bdAddress)));
#endif
        pMutexUnlock(prh_mgr_inqdb_list_lock);
    }
}


/*************************************************************************************************
* FUNCTION :- prh_mgr_cull_inquiry_db
*
* DESCRIPTION :- This function is called by a timer callback at intervals of 
*                 HOST_INQDB_CULL_GRANULARITY. It performs clean up on the inq db.

  
*                 
************************************************************************************************/
#if HOST_INQDB_CULL_THRESHOLD!=0

/* this is called by a timer callback at intervals of HOST_INQDB_CULL_GRANULARITY */
void prh_mgr_cull_inquiry_db(void *unused) {

    extern u_int8 prh_mgr_inquiryInProgress;
	extern u_int32 prh_mgr_inqdb_num_entries;
    /* increment each age by one, and remove any old entry  */
    struct st_t_BT_InquiryInfo *current, *temp;
	char err_str[50]={0};




    pMutexLock(prh_mgr_inqdb_list_lock);

    if (!(prh_mgr_discoveryInProgress || prh_mgr_inquiryInProgress)) {
        if (!prh_mgr_inqdb_num_entries) {
            pMutexUnlock(prh_mgr_inqdb_list_lock);
            return;
        }
    
        current=prh_mgr_inqdb_head;

        while (current) {
            temp=current->next;
            if (current->entryAge>HOST_INQDB_CULL_THRESHOLD && prh_mgr_inqdb_num_entries>HOST_INQDB_CULL_MIN_ENTRIES && !current->entryUsage) {
                /* remove the record */
                pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_cull_inquiry_db: removing device entry for %s, age=%d, prh_mgr_inqdb_num_entries=%d\n",prh_mgr_printBDAddress(current->bdAddress),current->entryAge,prh_mgr_inqdb_num_entries));

                prh_mgr_unlink_inquiry_entry(current);
                if (current->deviceName)
                    pFree(current->deviceName);
                pFree(current);

                prh_mgr_inqdb_num_entries--;   

            } else {
                if (current->entryAge+1>current->entryAge && current->entryAge!=0) /* don't allow age to wrap, age=0 means inquiry is in progress */
                    current->entryAge++;
            }        
            current=temp;
        }
    }

    if (prh_mgr_inqdb_num_entries) /* reset the timer */
        prh_mgr_inqdb_timer_handle=pTimerCreate(HOST_INQDB_CULL_GRANULARITY,prh_mgr_cull_inquiry_db,NULL,pTIMER_ONESHOT);

    pMutexUnlock(prh_mgr_inqdb_list_lock);  
}

#endif




/********************************************************************************************
* FUNCTION: MGR_GetNextDeviceRecord
*
* DESCRIPTION:
*           Takes a pointer to an entry in the device database and the returns the next entry. 
*           If NULL is passed into the function, the head of the device database is returned.
* INPUTS:
*           current: pointer to current entry in the Security Database.          
* RETURNS:
*           next:    pointer to next entry in the Security Database.
*           t_api:   status return code 
*********************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_GetNextDeviceRecord(struct st_t_BT_DeviceEntry *current, struct st_t_BT_DeviceEntry **next) {

    pMutexLock(prh_mgr_inqdb_list_lock);

    if(!current) {
        if (prh_mgr_inqdb_head)
            prh_mgr_inqdb_head->entryUsage++;
		*next=(struct st_t_BT_DeviceEntry *)prh_mgr_inqdb_head;
    } else {

        if (((struct st_t_BT_InquiryInfo *)current)->entryUsage>0)
            ((struct st_t_BT_InquiryInfo *)current)->entryUsage--;
#if pDEBUG
        else
            pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"MGR_GetNextDeviceRecord: reference count for device %s is already zero\n",prh_mgr_printBDAddress(((struct st_t_BT_InquiryInfo *)current)->bdAddress)));
#endif
        if (((struct st_t_BT_InquiryInfo *)current)->next)
            ((struct st_t_BT_InquiryInfo *)current)->next->entryUsage++;
		*next = (struct st_t_BT_DeviceEntry *)(((struct st_t_BT_InquiryInfo *)current)->next);
    }

    pMutexUnlock(prh_mgr_inqdb_list_lock);
	
	return BT_NOERROR;
}

