/*
 * MODULE NAME:    mgr_devdb.c
 * DESCRIPTION:    Host Manager Device Database Functionality
 * AUTHOR:         Martin Mellody
 * DATE:           23 May 2000
 *
 * SOURCE CONTROL: $Id: mgr_devdb.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"
#include "mgr_devdb.h"
#include "mgr_utils.h"
#include "mgr_core.h"
#include "mgr_inqdb.h"


#define MGR_DEVDB_MIN_ENTRIES    5

#define MGR_DEVENTRY_REUSE_SEVERITY_MAX 3
/* GLOBAL VARIABLES */
struct st_t_BT_ConnectionInfo *prh_device_list_head;
t_pMutex prh_mgr_conndb_list_lock;

extern u_int8 prh_mgr_auth_supplied_link_key;
extern t_bdaddr prh_mgr_auth_supplied_link_key_addr;

/* API-FUNCTIONS */

/*************************************************************************************************
* FUNCTION :- MGR_SetDeviceTrustLevel
*
* Input Parameters
*    t_bdaddr addr     A Bluetooth device address.
*    u_int8 trustLevel The required trust level for a device in the database
*                           MGR_UNKNOWN   
*                           MGR_UNTRUSTED 
*                           MGR_TRUSTED   
*
* Returns 
*    t_api
*
* DESCRIPTION :- Sets the trust level (MGR_TRUSTED or MGR_UNTRUSTED) of a device in the database.
* If no entry for that address exists, a new entry is added to the device database and the 
* trust level is set.
*
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_SetDeviceTrustLevel(t_bdaddr address, u_int8 trustLevel) {	
	struct st_t_BT_ConnectionInfo *currentDevice;
	
    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_SetDeviceTrustLevel called for %s, new trustlevel %d\n",prh_mgr_printBDAddress(address),trustLevel));

    if(trustLevel==MGR_TRUSTED || trustLevel==MGR_UNTRUSTED || trustLevel==MGR_UNKNOWN) {
		currentDevice = prh_mgr_return_deviceEntryByAddress(address);
        if(currentDevice) {
			currentDevice->trustLevel = trustLevel;
            if (trustLevel!=MGR_UNKNOWN)
                currentDevice->recordState|=MGR_DEVDB_TRUSTLEVEL_STORED;
            else 
                currentDevice->recordState&=~MGR_DEVDB_TRUSTLEVEL_STORED;
			return BT_NOERROR;
        } else 
			return BT_NORESOURCES;
    } else 
		return BT_INVALIDPARAM;
}


/*************************************************************************************************
* FUNCTION :- MGR_GetDeviceTrustLevel
*
* Input Parameters 
*    t_bdaddr addr     A Bluetooth device address.
*
* Returns
*    u_int8   The current Trust level.
*                           MGR_UNKNOWN   
*                           MGR_UNTRUSTED 
*                           MGR_TRUSTED   
*
* DESCRIPTION :- Returns the trust level of a device tored in the device database. 
*    If the address is not found in the database, MGR_UNKNOWN is returned. 
*
************************************************************************************************/

APIDECL1 u_int8 APIDECL2 MGR_GetDeviceTrustLevel(t_bdaddr address) {
	struct st_t_BT_ConnectionInfo *dbEntry;
	
    dbEntry=prh_mgr_find_deviceEntryByAddress(address);

    if(dbEntry) 
        return dbEntry->trustLevel;
    else 
		return MGR_UNKNOWN; 
}

/*************************************************************************************************
* FUNCTION :- MGR_SetRemoteDevicePIN
*
* Input Parameters 
*    t_bdaddr address     A Bluetooth device address.
*    u_int8 pinType       The type of Pin Code supported by this device.
*                                MGR_ALLZEROPIN    
*                                MGR_PRESETPIN     
*                                MGR_USERREQPIN    
*    u_int8 pinLength     The length of the Pin Code
*    u_int8 *pinCode      A pointer to the first byte of the pin code.
*
* Returns
*    t_api
*
* DESCRIPTION :- Instructs the manager how to handle a PIN request for a specific device. 
*
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_SetRemoteDevicePIN(t_bdaddr address, u_int8 pinType, u_int8 pinLength, u_int8 *pinCode) {
	struct st_t_BT_ConnectionInfo *dbEntry;
    u_int8 *temp=NULL;

    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_SetRemoteDevicePIN called for %s, new PIN type 0x%02X\n",prh_mgr_printBDAddress(address),pinType));


	if(pinType==MGR_PRESETPIN) {
        if (pinLength<1 || pinLength>16)
            return BT_INVALIDPARAM;
        temp=pMalloc(pinLength);
        if(!temp) 
		    return BT_NORESOURCES;    
    }
	
	dbEntry = prh_mgr_return_deviceEntryByAddress(address);	
    if(!dbEntry) {
        if (temp)
            pFree(temp);
		return BT_NORESOURCES;
    }
	
    if (dbEntry->pinCode)
		{
			pFree(dbEntry->pinCode);
			dbEntry->pinCode=NULL;
		}

	dbEntry->pinType = pinType;

	if(pinType==MGR_PRESETPIN) {
		dbEntry->pinLength = pinLength;
		dbEntry->pinCode = temp;
		pMemcpy(dbEntry->pinCode, pinCode, pinLength);
    }

#if PRH_MGR_DEFAULTPIN_ALLZERO
    if(pinType!=MGR_ALLZEROPIN) {
#else
    if(pinType!=MGR_USERREQPIN) {
#endif
        dbEntry->recordState|=MGR_DEVDB_PINCODE_STORED;
    } else {
        dbEntry->recordState&=~MGR_DEVDB_PINCODE_STORED;
    } 
	return BT_NOERROR;
}

/*************************************************************************************************
* FUNCTION :- MGR_GetRemoteDevicePIN
*
* Input Parameters 
*    t_bdaddr address     A Bluetooth device address.
*    u_int8* buffer       A pointer to the buffer for the pin code.
*    u_int8 bufLen        The length of buffer for the pin code
*
* Output Parameters
*    u_int8 pinType       The type of Pin Code supported by this device.
*                                MGR_ALLZEROPIN    
*                                MGR_PRESETPIN     
*                                MGR_USERREQPIN    
*
*    u_int8 *pinLen      The actual length of the pin code.
*
* Returns
*    t_api
* 
* DESCRIPTION :- 
* Retrieves the Pin Code used for a specific device. The Pin Type, Length and contents are returned.
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_GetRemoteDevicePIN(t_bdaddr address, u_int8 *pinType, u_int8 *buffer, u_int8 bufLen, u_int8 *pinLen) {
	struct st_t_BT_ConnectionInfo *dbEntry;
	
	dbEntry = prh_mgr_find_deviceEntryByAddress(address);
    if (!dbEntry) {
#if PRH_MGR_DEFAULTPIN_ALLZERO
		*pinType = MGR_ALLZEROPIN;
#else
		*pinType = MGR_USERREQPIN;
#endif
    } else {
		*pinType = dbEntry->pinType;
		if(dbEntry->pinType==MGR_PRESETPIN) {
			*pinLen = dbEntry->pinLength;
            /* if bufLen==0, do not touch buffer as it may be NULL - just return the length */
            if (bufLen==0)
                return BT_NOERROR;
            else if(bufLen >= dbEntry->pinLength)            
                pMemcpy(buffer,dbEntry->pinCode,dbEntry->pinLength);
            else /* buffer is too small for the PIN */
				return BT_BUFTOOSMALL;
        }
    }	
	return BT_NOERROR;
}

/*************************************************************************************************
* FUNCTION :- MGR_ReadLinkKeyType
*
*
* DESCRIPTION :- Reads the Link Key Type for a given remote device (address). The value is placed 
* in linkKeyType. This will return an error if the HCI version is not 1.1 or later.
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_ReadLinkKeyType(t_bdaddr address, u_int8 *linkKeyType) {
	struct st_t_BT_ConnectionInfo *dbEntry;
	
	if(prh_mgr_HCIversion <	PRH_HCI_VERSION_BLUETOOTHSPEC11)	
		return BT_UNSUPPORTED;
	
	dbEntry = prh_mgr_find_deviceEntryByAddress(address);
	if(!dbEntry)
		return BT_INVALIDPARAM;
	
	*linkKeyType = dbEntry->linkKeyType;

	return BT_NOERROR;		
}



/*************************************************************************************************
* FUNCTION :- MGR_RetrieveDeviceList
*
* INPUTS:
*          addressBuffer: buffer that list of devices is read into.
*          bufLen:        length of buffer (in t_bdaddr) to read devices into. 
*          numReturned:   number of devices read into the buffer.
*          totalNum:      total number of devices in the Device Database.
*
* RETURNS: 
*           t_api
*
* DESCRIPTION :- Reads a list of bluetooth device addresses from the database into a buffer.
*
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_RetrieveDeviceList(t_bdaddr *addressBuffer, u_int16 buflen, u_int16 *numReturned, u_int16 *totalNum) {
    struct st_t_BT_ConnectionInfo *currentDevice;
    u_int16 i=0;
	
    pMutexLock(prh_mgr_conndb_list_lock);
    currentDevice=prh_device_list_head;

    while (currentDevice && i<buflen) {
        addressBuffer[i++]=currentDevice->bdAddress;
        currentDevice=currentDevice->nextDevice;
    }

    if (totalNum)
        *totalNum=prh_mgr_numDeviceDatabaseEntries;
	
    pMutexUnlock(prh_mgr_conndb_list_lock);

    if (numReturned)
        *numReturned=i;

	if (currentDevice)
        return BT_BUFTOOSMALL;
    else
        return BT_NOERROR;
}

/*************************************************************************************************
* FUNCTION :- MGR_GetNextConnectionRecord
*
* DESCRIPTION :- This function provide a method of traversing through the records that are
*                 held in the device database. Initially NULL should be passed in as the current
*                 entry. The head of the device database will be returned. Subsequentially the current
*                 device is passed in and the next device in the device database is returned, until
*                 a NULL is returned which indicates that there are no more devices in the database.
* 
************************************************************************************************/
APIDECL1 t_api APIDECL2 MGR_GetNextConnectionRecord(struct st_t_BT_ConnectionEntry *current, struct st_t_BT_ConnectionEntry **next)
{
    /* NB: callers should continue iterating until they get a NULL return in order to unlock all records */
		
    pMutexLock(prh_mgr_conndb_list_lock);

	if(!current)
	{
		*next=(struct st_t_BT_ConnectionEntry *)prh_device_list_head;
	}
	else 
	{
#if pDEBUG
        if (!((struct st_t_BT_ConnectionInfo *)current)->recordState&MGR_DEVDB_USER_POINTER)
            pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"MGR_GetNextConnectionRecord: incoming entry pointer 0x%p invalid\n",current));
           
        if (((struct st_t_BT_ConnectionInfo *)current)->entryUsage==0)
            pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"MGR_GetNextConnectionRecord: reference count for device %s is already zero\n",prh_mgr_printBDAddress(((struct st_t_BT_InquiryInfo *)current)->bdAddress)));
#endif
				((struct st_t_BT_ConnectionInfo *)current)->entryUsage--;
		*next=(struct st_t_BT_ConnectionEntry *)(((struct st_t_BT_ConnectionInfo *)current)->nextDevice);
    }
	if (*next) 
	{
        ((struct st_t_BT_ConnectionInfo *)*next)->entryUsage++;
		((struct st_t_BT_ConnectionInfo *)*next)->recordState&=~MGR_DEVDB_USER_POINTER;
    }
    pMutexUnlock(prh_mgr_conndb_list_lock);

    pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"MGR_GetNextConnectionRecord: incoming pointer 0x%p, usage %d outgoing pointer 0x%p, usage %d\n",current?current:0,current?((struct st_t_BT_ConnectionInfo *)current)->entryUsage:0,*next?*next:0,*next?((struct st_t_BT_ConnectionInfo *)*next)->entryUsage:0));
	return BT_NOERROR;
}


 t_api prh_mgr_remove_connection_record_protect(struct st_t_BT_ConnectionEntry *entry, u_int32 flags)
   {
    struct st_t_BT_ConnectionInfo *current, *previous;



    previous=NULL;
    current=prh_device_list_head;

    while (current) 
    {
        if (current==(struct st_t_BT_ConnectionInfo*)entry)        
            break;
        previous=current;
        current=current->nextDevice;        
    };
if(current->recordState >= MGR_DEVDB_DISCARDABLE_THRESHOLD)//zhou siyou add it .20090728
	current->recordState=0;
#if pDEBUG
    if (!current) 
    {
        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"MGR_RemoveConnectionRecord: Cannot find entry at 0x%p\n",entry));
        return BT_INVALIDPARAM;
    }
    
    if (current->aclState!=CONN_CLOSED)
    {
        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"MGR_RemoveConnectionRecord: Cannot remove entry for device %s - aclState=0x%02X\n",prh_mgr_printBDAddress(current->bdAddress), current->aclState));
        return BT_DISALLOWED;
    }

    if ((current->recordState >= MGR_DEVDB_DISCARDABLE_THRESHOLD)) 
    {
        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"MGR_RemoveConnectionRecord: Cannot remove entry for device %s - recordState=0x%02X\n",prh_mgr_printBDAddress(current->bdAddress), current->recordState));
        return BT_DISALLOWED;
    }
							/*during db pruning - entryusage will be 0 - during*/
    if (current->entryUsage > 1 ) { /* should never happen since recordState should be set */
        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"MGR_RemoveConnectionRecord: Cannot remove entry for device %s - entryUsage=0x%02X\n",prh_mgr_printBDAddress(current->bdAddress), current->recordState));
        return BT_DISALLOWED;    
    }
#else
    if (!current )
        return BT_INVALIDPARAM;
			
    if((current->aclState!=CONN_CLOSED) || (current->recordState >= MGR_DEVDB_DISCARDABLE_THRESHOLD) || (current->entryUsage > 1) )
		return BT_DISALLOWED;
#endif

    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_RemoveConnectionRecord: Removing entry for device %s\n",prh_mgr_printBDAddress(current->bdAddress)));

    if (previous)
        previous->nextDevice=current->nextDevice;
    else
        prh_device_list_head=current->nextDevice;

    if (current->linkKey)
        pFree(current->linkKey);
    pFree(current);
		prh_mgr_numDeviceDatabaseEntries--;

     
    return BT_NOERROR;

   }

/*************************************************************************************************
* FUNCTION :- MGR_RemoveConnectionRecord
*
* DESCRIPTION :- This function removes a device fron the device database.
*                 
* 
************************************************************************************************/
APIDECL1 t_api APIDECL2 MGR_RemoveConnectionRecord(struct st_t_BT_ConnectionEntry *entry, u_int32 flags)
{
  int ret;

  pMutexLock(prh_mgr_conndb_list_lock);
  ret = prh_mgr_remove_connection_record_protect(entry,flags);
  pMutexUnlock(prh_mgr_conndb_list_lock);  

  return ret;
}

/*************************************************************************************************
* FUNCTION :- MGR_DeleteLinkKey
*
* DESCRIPTION :- This function searchs the device database and if there is a Link key for that
*                 device, the link key is deleted.
*                 
************************************************************************************************/
APIDECL1 t_api APIDECL2 MGR_DeleteLinkKey(t_bdaddr address) 
{

    struct st_t_BT_ConnectionInfo *dbEntry;

    dbEntry=prh_mgr_find_deviceEntryByAddress(address);

    if (!dbEntry) 
    {
        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"MGR_DeleteLinkKey: Cannot find entry for device %s\n",prh_mgr_printBDAddress(address)));
        return BT_INVALIDPARAM;
    } 
    else 
    {
        if (!dbEntry->linkKey)
        {
            pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"MGR_DeleteLinkKey: no link key exists for device %s\n",prh_mgr_printBDAddress(dbEntry->bdAddress)));
            return BT_INVALIDPARAM;
        } 
        else 
        {
            pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_DeleteLinkKey: Deleting link key for device %s\n",prh_mgr_printBDAddress(dbEntry->bdAddress)));
            pFree(dbEntry->linkKey);
            dbEntry->linkKey=NULL;
            dbEntry->recordState&=~MGR_DEVDB_LINKKEY_STORED;
            return BT_NOERROR;
        }
    }
}

APIDECL1 t_api APIDECL2 MGR_SetDeviceInfo(t_bdaddr address,  u_int8 *linkKey,  u_int8 keyType)
{
	struct st_t_BT_ConnectionInfo *dbEntry;
	//int nameLen = 0;

	dbEntry = prh_mgr_find_deviceEntryByAddress(address);
	if(dbEntry)
		return 	BT_NOERROR;
	
	dbEntry = prh_mgr_return_deviceEntryByAddress(address);
	if( !dbEntry)
		return BT_NORESOURCES;
	
	/*
	if(deviceName)
	{
		 pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"MGR_SetDeviceInfo, deviceName:%s\n", deviceName));
		nameLen = pStrlen((char*)deviceName);
		dbEntry->deviceName = pMalloc(nameLen+1);
		if(dbEntry->deviceName)
		{
			pMemcpy(dbEntry->deviceName, deviceName, nameLen);
			dbEntry->deviceName[nameLen] = '\0';
		}
	}
	*/

	if( linkKey)
	{		
		//pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"MGR_SetDeviceInfo, link key :%s\n", linkKey));
		dbEntry->linkKey = pMalloc(0x10);
		if(dbEntry->linkKey)
		{
			pMemcpy(dbEntry->linkKey, linkKey,  0x10);
		}
	}

	dbEntry->linkKeyType = keyType;

	return  BT_NOERROR;
	
}

/***************************************** NON-API FUNCTIONS */




/*************************************************************************************************
* FUNCTION :- prh_mgr_find_deviceEntryByAddress
*
* Input Parameters
*    t_bdaddr addr     A Bluetooth device address.
*
* Returns 
*      struct st_t_BT_ConnectionInfo*  A pointer to the device database entry
* 
* DESCRIPTION :- 
* Retrieves a pointer to a device database entry using the device address as a key. If no match is
* found NULL is returned.
************************************************************************************************/

struct st_t_BT_ConnectionInfo *prh_mgr_find_deviceEntryByAddress(t_bdaddr addr) 
{
	struct st_t_BT_ConnectionInfo *currentDevice;
	
    pMutexLock(prh_mgr_conndb_list_lock);
    currentDevice=prh_device_list_head;

	while (currentDevice) {		
		if (BD_ADDR_CMP(addr,currentDevice->bdAddress)) {
			currentDevice->updated=0;
         
            pMutexUnlock(prh_mgr_conndb_list_lock);
			return currentDevice;
		}
		currentDevice=currentDevice->nextDevice;
	}
    pMutexUnlock(prh_mgr_conndb_list_lock);
	return NULL;  
}


//CuiJian add 2009/07/23
APIDECL1 int APIDECL2 MGR_GetACLState(t_bdaddr addr) 
{
	struct st_t_BT_ConnectionInfo *currentDevice;
	
    pMutexLock(prh_mgr_conndb_list_lock);
    currentDevice=prh_device_list_head;

	while (currentDevice) {		
		if (BD_ADDR_CMP(addr,currentDevice->bdAddress)) {
			currentDevice->updated=0;
            pMutexUnlock(prh_mgr_conndb_list_lock);
			if (currentDevice->aclState !=CONN_CLOSED) 
				{
  pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"currentDevice->aclState=%x!",currentDevice->aclState));				
  pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"currentDevice=%x!",currentDevice));				
				}
				return (currentDevice->aclState);  		
		}
		currentDevice=currentDevice->nextDevice;
	}
    pMutexUnlock(prh_mgr_conndb_list_lock);
	return CONN_NONE;  
}

//zhou siyou add 2009/08/26
APIDECL1 int APIDECL2 MGR_HaveDeviceConnected(t_bdaddr device_addr) 
{
	struct st_t_BT_ConnectionInfo *currentDevice=prh_device_list_head;	
	/* check that no ACLS are up */
	while(currentDevice) 
	{
		if((currentDevice->aclState!=CONN_CLOSED) && (!BD_ADDR_CMP(device_addr,currentDevice->bdAddress))) 
			break;
		currentDevice = currentDevice->nextDevice;
	}
	if(currentDevice)
		return 1;
	else 
		return 0;
}

/*************************************************************************************************
* FUNCTION :- prh_mgr_return_deviceEntryByAddress
*
* Input Parameters
*    t_bdaddr addr     A Bluetooth device address.
*
* Returns 
*      struct st_t_BT_ConnectionInfo*  A pointer to the device database entry
* 
* DESCRIPTION :- 
* Retrieves a pointer to a device database entry using the device address as a key. If no match is
* found a record is created and returned. If a new entry cannot be allocated, NULL is returned.
************************************************************************************************/
struct st_t_BT_ConnectionInfo *prh_mgr_return_deviceEntryByAddress(t_bdaddr addr) {
	struct st_t_BT_ConnectionInfo *deviceRecord;
	
    deviceRecord=prh_mgr_find_deviceEntryByAddress(addr);

    if (!deviceRecord)
			deviceRecord = prh_mgr_addDeviceEntry(addr);

	return deviceRecord;  
}

/*************************************************************************************************
* FUNCTION :- prh_mgr_get_deviceEntryByHandle
*
* Input Parameters
*      u_int16 handle  The connection handle for either a SCO or ACL link.
*
* Returns 
*      struct st_t_BT_ConnectionInfo*  A pointer to the device database entry
* 
* DESCRIPTION :- 
* Retrieves a pointer to a device database entry using the connection handle as a key. The 
* connection handle can be the handle of either an ACL or SCO link. If no match is found
* NULL is returned.
************************************************************************************************/

struct st_t_BT_ConnectionInfo *prh_mgr_get_deviceEntryByHandle(u_int16 handle, u_int8 *linkType) {
	int i;
	struct st_t_BT_ConnectionInfo *currentDevice;
	
    pMutexLock(prh_mgr_conndb_list_lock);
    currentDevice=prh_device_list_head;
	while (currentDevice) {
		
		if ((currentDevice->aclState==CONN_ACTIVE || currentDevice->aclState==CONN_RELEASEPENDING) && currentDevice->aclHandle==handle) {
			if(linkType)
				*linkType = HCI_linkACL;
            pMutexUnlock(prh_mgr_conndb_list_lock);
			return currentDevice; 
		}
		for (i=0; i<MAX_SCO_LINKS; i++) {
			if ( (currentDevice->scoState[i]==CONN_ACTIVE || CONN_RELEASEPENDING == currentDevice->scoState[i])&& currentDevice->scoHandle[i]==handle) {
				if(linkType)
					*linkType = HCI_linkSCO;
                pMutexUnlock(prh_mgr_conndb_list_lock);
				return currentDevice; 
			}
		}
		currentDevice=currentDevice->nextDevice;
	}
    pMutexUnlock(prh_mgr_conndb_list_lock);
	
	return NULL;
}


/*************************************************************************************************
* FUNCTION :- prh_mgr_addDeviceEntry
*
* Input Parameters
*    t_bdaddr addr     A Bluetooth device address.
*
* Returns 
*      struct st_t_BT_ConnectionInfo*  A pointer to the new device database entry
*
* DESCRIPTION :- 
* This functions adds a new entry to the device database and initialises all its fields.
************************************************************************************************/

struct st_t_BT_ConnectionInfo *prh_mgr_addDeviceEntry(t_bdaddr addr) {
	struct st_t_BT_ConnectionInfo *currentDevice;
	u_int8 i, severity, reuseRecord;
  void* temp;  

    currentDevice=NULL;
    reuseRecord=0;
	
    if (prh_mgr_numDeviceDatabaseEntries>=MGR_DEVDB_MAX_ENTRIES) {
        pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_addDeviceEntry: Reached limit of %d device records\n",MGR_DEVDB_MAX_ENTRIES));
        severity=1;
        do {
            currentDevice=prh_mgr_get_unused_entry(severity);
        } while (!currentDevice && (++severity)<=MGR_DEVENTRY_REUSE_SEVERITY_MAX);
        if (!currentDevice) {
            pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_addDeviceEntry:  cannot get unused device entry - returning NULL with %d records allocated\n",prh_mgr_numDeviceDatabaseEntries));
            return NULL;
        } else {
            pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_addDeviceEntry:  reusing existing device entry at 0x%p\n",currentDevice));
            reuseRecord=1;
            if (currentDevice->linkKey)
                pFree(currentDevice->linkKey);
            if (currentDevice->pinCode)
                pFree(currentDevice->pinCode);
        }
    }

    if (!reuseRecord) 
        currentDevice=pMalloc(sizeof(struct st_t_BT_ConnectionInfo));
	if(!currentDevice)
		return NULL;
	
	temp=currentDevice->nextDevice;
	/* zero-init to set all callback members to NULL */
	pMemset(currentDevice, 0, sizeof(struct st_t_BT_ConnectionInfo));	
	currentDevice->nextDevice=temp;
	
    /* Set Default Values */

	currentDevice->role = ROLE_MASTER;
	currentDevice->aclState = CONN_CLOSED;
	for (i=0; i<MGR_MAXNUM_SCOHANDLES; i++) 
		currentDevice->scoState[i]=CONN_CLOSED;
	currentDevice->aclFlushTimeoutFlag = FLUSH_TO_UNSET;
	currentDevice->aclFlushTimeout = DEFAULT_FLUSH_TIMEOUT;
	currentDevice->aclLinkSupTimeoutFlag = LINK_TO_UNSET;
	currentDevice->aclLinkSupTimeout = DEFAULT_LINK_SUP_TIMEOUT;
	currentDevice->aclLinkSupTimeoutNew = DEFAULT_LINK_SUP_TIMEOUT;
	currentDevice->accessReqAuthState = AUTH_IDLE;
	currentDevice->accessReqEncryptState = SET_ENCRYPT_IDLE;
	currentDevice->bondingState = BOND_INACTIVE; 
	currentDevice->deviceMode = MGR_DEVACTIVE;
	currentDevice->linkKeyType = MGR_COMBINATIONKEY;
	currentDevice->trustLevel=MGR_UNKNOWN;
#if PRH_MGR_DEFAULTPIN_ALLZERO
	currentDevice->pinType = MGR_ALLZEROPIN; 
#else
	currentDevice->pinType = MGR_USERREQPIN; 
#endif
    currentDevice->pinLength = MAX_PIN_LEN;

	
    prh_mgr_copyBDAddress(&currentDevice->bdAddress,&addr);

    if (!reuseRecord) {
        pMutexLock(prh_mgr_conndb_list_lock);

        if (prh_device_list_head) {
            currentDevice->nextDevice=prh_device_list_head;
            prh_device_list_head=currentDevice;
        } else {
		    prh_device_list_head=currentDevice;
            currentDevice->nextDevice=NULL;
        }

        prh_mgr_numDeviceDatabaseEntries++;
        pMutexUnlock(prh_mgr_conndb_list_lock);
    }

	return currentDevice; 
}

/*************************************************************************************************
* FUNCTION :- prh_mgr_isDeviceConnected
*
* Input Parameters
*    t_bdaddr addr     A Bluetooth device address.
*
* Returns
*    u_int8    Indicating the outcome of the operation. 
*
* DESCRIPTION :- 
* Check if a given Bluetooth device is already participating in an ACL link
* ( i.e in the device database and in the connected state ) This information is
* stale immediately the function returns, but this is required for SDP.
* 
************************************************************************************************/

u_int8 prh_mgr_isDeviceConnected(t_bdaddr addr)
{
	struct st_t_BT_ConnectionInfo *dbEntry;
	
	dbEntry = prh_mgr_find_deviceEntryByAddress(addr);
	if(dbEntry)
    {
		if(CONN_ACTIVE == dbEntry->aclState)
			return TRUE;
    }
	return FALSE;
}


/*************************************************************************************************
* FUNCTION :- prh_mgr_updateLinkKeyEntry
*
* Input Parameters
*    t_bdaddr addr     A Bluetooth device address.
*    u_int8*           A pointer to the first byte of the link key (all link keys are 16 bytes)
*
* Returns
*    void
*
* DESCRIPTION :- 
* Update the link key entry for a device in the device database.
************************************************************************************************/

void prh_mgr_updateLinkKeyEntry(t_bdaddr addr, u_int8 *linkKey, u_int8 linkKeyType)
{
	struct st_t_BT_ConnectionInfo *dbEntry;
	
	dbEntry = prh_mgr_return_deviceEntryByAddress(addr);

	if(!dbEntry)
		return; /* nothing can be done - link key will not be stored */

    if(!dbEntry->linkKey) 
		dbEntry->linkKey = pMalloc(LINKKEY_SIZE);
    
	if(!dbEntry->linkKey)
		return; /* nothing can be done - link key will not be stored */

    dbEntry->recordState|=MGR_DEVDB_LINKKEY_STORED;
    pMemcpy(dbEntry->linkKey,linkKey,LINKKEY_SIZE);

    /* Set Link Key Type, if HCI version is 1.1 or later */
	if(prh_mgr_HCIversion >= PRH_HCI_VERSION_BLUETOOTHSPEC11)
		dbEntry->linkKeyType = linkKeyType;

//	prh_mgr_auth_supplied_link_key=1;
//	memcpy(prh_mgr_auth_supplied_link_key_addr.bytes,addr.bytes, 6);
}

/*************************************************************************************************
* FUNCTION :- prh_mgr_get_unused_entry
*
* DESCRIPTION :- This function traversed through the devices in the database and selects
*                 a device for removing based on a criteria that is set with the severity
*                 parameter.
*                 severity 1: devices with CONN_CLOSED AND NO PIN CODE AND NO LINKKEY
*                 severity 2: devices with CONN_CLOSED AND NO PIN CODE 
*                 severity 3: devices with CONN_CLOSED 
*                 
************************************************************************************************/
struct st_t_BT_ConnectionInfo *prh_mgr_get_unused_entry(u_int8 severity) {
    struct st_t_BT_ConnectionInfo *currentDevice, *previousDevice;

    /* 
        make several passes through the database removing the least necessary devices 
        at each pass until we free enough entires or don't have anything left we can remove
        Each pass is of increasing severity in terms of stack operation (1=least severe).
    */
    currentDevice=prh_device_list_head;
    previousDevice=NULL;

    while (currentDevice && severity<=MGR_DEVENTRY_REUSE_SEVERITY_MAX) {
        if (currentDevice->aclState!=CONN_CLOSED)
				{
					currentDevice=currentDevice->nextDevice;
					continue;
				}



        if (severity==1) {
            /* remove enties which are unconnected, without link key or pin code */
            if ((currentDevice->aclState==CONN_CLOSED) && 
#if PRH_MGR_DEFAULTPIN_ALLZERO
								(currentDevice->pinType==MGR_ALLZEROPIN) &&
#else
								(currentDevice->pinType==MGR_USERREQPIN) &&
#endif          
                (!currentDevice->linkKey) ) {
										pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_get_unused_entry: re-using device database entry %s at severity %d\n",prh_mgr_printBDAddress(currentDevice->bdAddress),severity));
                    return currentDevice;
            }
        }
        if (severity==2) {
            /* remove enties which are unconnected, without a pin code */
            if ((currentDevice->aclState==CONN_CLOSED) &&
#if PRH_MGR_DEFAULTPIN_ALLZERO
                (currentDevice->pinType==MGR_ALLZEROPIN) ) {
#else
                (currentDevice->pinType==MGR_USERREQPIN) ) {
#endif          
										pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_get_unused_entry: re-using device database entry  %s at severity %d\n",prh_mgr_printBDAddress(currentDevice->bdAddress),severity));
                    return currentDevice;
            }
        }
        if (severity==3) {
            /* remove enties which are unconnected */
            if ((currentDevice->aclState==CONN_CLOSED) ) {
										pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_get_unused_entry: re-using device database entry %s at severity %d\n",prh_mgr_printBDAddress(currentDevice->bdAddress),severity));
                    return currentDevice;
            }
        }
        currentDevice=currentDevice->nextDevice;
    };

		pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"prh_mgr_get_unused_entry: cannot find device database entry for reuse at severity %d\n",severity));
    return NULL;
}

/*************************************************************************************************
* FUNCTION :- prh_mgr_prune_device_database
*
* DESCRIPTION :-  This function is called to remove devices from the device database
*                  It use the prh_mgr_get_unused_entry() function to select the devices
*                  that can be remove and then removes them
*                 
************************************************************************************************/
void prh_mgr_prune_device_database(void *dummy) {
    struct st_t_BT_ConnectionInfo *currentDevice, *temp;
    u_int8 severity=1;

    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"prh_mgr_prune_device_database called, currently %d entries, target %d entries\n",prh_mgr_numDeviceDatabaseEntries,MGR_DEVDB_MIN_ENTRIES));
		
		if(prh_mgr_numDeviceDatabaseEntries > MGR_DEVDB_MIN_ENTRIES)
		{
			do {
					do {
							currentDevice=prh_mgr_get_unused_entry(severity);
							if (currentDevice) {
									temp=currentDevice->nextDevice;
									MGR_RemoveConnectionRecord((struct st_t_BT_ConnectionEntry *)currentDevice,0);
									currentDevice=temp;
							}        
					} while (prh_mgr_numDeviceDatabaseEntries>MGR_DEVDB_MIN_ENTRIES && currentDevice);
			} while (prh_mgr_numDeviceDatabaseEntries>MGR_DEVDB_MIN_ENTRIES && (++severity)<=MGR_DEVENTRY_REUSE_SEVERITY_MAX);
		}
    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"prh_mgr_prune_device_database returning, currently %d entries\n",prh_mgr_numDeviceDatabaseEntries));
}
/*************************************************************************************************
* FUNCTION :- prh_mgr_set_callback
*
* DESCRIPTION :- This function sets the callback for a particular device entry
*                 
************************************************************************************************/
t_api prh_mgr_set_callback(u_int8 *offset, void **array, void *callbackFunc, struct st_t_BT_ConnectionInfo *dbEntry) 
{
    u_int8 i;

    for (i=0; i<MAX_OUTSTANDING_CALLBACKS; i++) {
        if (array[i]==NULL || array[i] == callbackFunc)
            break;
    }
    
    if (i==MAX_OUTSTANDING_CALLBACKS) 
    {
    	pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"prh_mgr_set_callback returning BT_NORESOURCES array[0]=%x,array[1]=%x \n",array[0],array[1]));
    	return BT_NORESOURCES;
    }

    array[i]=callbackFunc;
    *offset=i;

    if (dbEntry) {
        dbEntry->recordState|=MGR_DEVDB_CALLBACK_PENDING;
        dbEntry->outstandingCallbacks++;
    }

    return BT_NOERROR;
}        

/*************************************************************************************************
* FUNCTION :- prh_mgr_get_callback
*
* DESCRIPTION :- This function gets the callback for a particular device entry
*                 
************************************************************************************************/
void prh_mgr_get_callback(u_int8 *offset, void **array, void **callbackFunc, struct st_t_BT_ConnectionInfo *dbEntry) {
    u_int8 i;

    for (i=0; i<MAX_OUTSTANDING_CALLBACKS; i++) {
        if (array[i]!=NULL)
            break;
    }
    
    if (i<MAX_OUTSTANDING_CALLBACKS)  {
        *offset=i;
        *callbackFunc=array[i];
        array[i]=NULL;

        if (dbEntry && !--dbEntry->outstandingCallbacks)
            dbEntry->recordState&=~MGR_DEVDB_CALLBACK_PENDING;
    } else {
        *callbackFunc=NULL;
    }
}


#if 0 /* code unused until WUG implmenetation complete */

#if PRH_TCS_GATEWAY

/*************************************************************************************************
* FUNCTION :- prh_mgr_get_deviceEntryByTCSNum
*
*
* DESCRIPTION :- This function returns entries form the device database by there TCSNum.
*
************************************************************************************************/

struct st_t_BT_ConnectionInfo *prh_mgr_get_deviceEntryByTCSNum(u_int8 *tcsNum, u_int8 *uniqueEntry)
{
	struct st_t_BT_ConnectionInfo *currentDevice=prh_device_list_head;
	struct st_t_BT_ConnectionInfo *matchedDevice = NULL;
	u_int8 numFound = 0;
	
	while (currentDevice && numFound<2) {
		if (tcsNum)	{
			if (!pStrcmp((char *)currentDevice->tcsNum, (char *)tcsNum)) {
				numFound++;
				matchedDevice = currentDevice;
			}
		}
		currentDevice=currentDevice->nextDevice;
	}
	
	*uniqueEntry = (numFound<2); 
	return matchedDevice;
	
}

#endif /* PRH_TCS_GATEWAY */

#endif /* #if 0 */

