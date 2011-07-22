/*
 * MODULE NAME:    mgr_core.c
 * DESCRIPTION:    Host Manager Core Functionality
 * AUTHOR:         Martin Mellody
 * DATE:           10 March 2000
 *
 * SOURCE CONTROL: $Id: mgr_core.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 * 
 */

#include "host_types.h"
#include "host_config.h"

#include "host_config.h"
#include "papi.h"
#include "host_error.h"
#include "host_ll.h"

#include "mgr_core.h"
#include "mgr_utils.h"

#include "l2_acl_link.h"
#include "l2_connect_sm.h"
#include "sdp_ue.h"
#include "hci_constants.h"
#include "host_ctrl.h"
#include "mgr_inqdb.h"

#define	  SLICE_INQUIRY_SUPPORT		
volatile u_int8 prh_mgr_conn_accept_flag;
u_int16 prh_mgr_numDeviceDatabaseEntries;


/*  local device configuration information */
u_int8 prh_mgr_HCIversion;
u_int16 prh_mgr_HCIrevision;
u_int8 prh_mgr_LMPversion;
u_int16 prh_mgr_LMPsubversion;
u_int16 prh_mgr_ManufacturerId;
u_int8 *prh_mgr_LocalName;


t_bdaddr prh_mgr_LocalBDADDR;
t_bdaddr prh_mgr_pendingBDAddress;
struct st_t_BT_ConnectionInfo *prh_mgr_pendingNameEntry;
t_api prh_mgr_pendingNameStatus;

u_int16 prh_mgr_pendingDisconnectHandle;


u_int8 prh_mgr_localLMPFeatures[NUM_BYTES_LMP_FEATURES];
u_int8 prh_mgr_ReadLocalFeaturesComplete;
u_int16 prh_mgr_linkpolicyflags;

u_int8 prh_mgr_currentMode;
u_int8 prh_mgr_currentRole;
u_int8 prh_mgr_periodicInquiryModeActive;
u_int32 prh_mgr_periodicInquiryModeLap;
u_int8 prh_mgr_inquiryInProgress;
u_int8 prh_mgr_discoveryInProgress;
u_int8 prh_mgr_IsNameNeedRetrieve;
u_int32 	prh_mgr_ClassofDeviceToRetrieve;
u_int32	prh_mgr_AccessCode;
u_int8 prh_mgr_MaxResult;
u_int8 prh_mgr_DiscoveryTimeout;
u_int8 prh_mgr_disoverFlag;
u_int8 prh_mgr_deviceNumFound;
u_int8 prh_mgr_deviceNameRetrieved;
u_int8 prh_mgr_InquiryCanceled;
u_int8 prh_mgr_nameRequestStarted;			// a static variable in 

t_pTimer prh_mgr_InquiryTimerHandle;
struct st_t_BT_InquiryInfo *theEntryRetrievingName=NULL;

u_int8 prh_mgr_autoAcceptAll;
u_int32 prh_mgr_linkFlags;
u_int8 prh_mgr_authenticationMode;
u_int16 prh_mgr_connectionAcceptTimeout;
u_int16 prh_mgr_pageTimeout;
extern t_pMutex prh_mgr_conndb_list_lock;

void (*prh_mgr_mode_callbackFunc)(t_api status);

void (*prh_mgr_WriteAddress_callbackFunc)(t_api status);
void (*prh_mgr_ReadPHYRegister_callbackFunc)(t_api status, int result);
void (*prh_mgr_WritePHYRegister_callbackFunc)(t_api status);
void (*prh_mgr_BaseBandRxText_callbackFunc)(t_api status);
void (*prh_mgr_BaseBandTxText_callbackFunc)(t_api status);
void (*prh_mgr_BaseBandTestEnd_cbFunc)(t_api status);

void (*prh_mgr_InquiryCancelCallBack)(int num, int status);

extern void (*prh_callback_AcceptConnectionRequest)(t_bdaddr);  
u_int8 prh_mgr_mode_flag;
u_int8 prh_mgr_scan_enable;
u_int8 prh_mgr_scan_enable_pending;
u_int8 prh_mgr_liac_present;
u_int8 prh_mgr_liac_present_pending;

/* used by prh_mgr_sniffACL */
static struct st_t_pendingCmdStatus pendingCmdStatus[PRH_MGR_SIZE_PENDINGCMDARRAY];


/* Used for Inquiry waiting on Hold of all ACLs */
static u_int8 pendingInquiryLength;
static u_int32 pendingInquiryAccessCode;
static u_int8 pendingInquiryMaxResults;

/* prh_mgr_holdAllOpenACLs */
t_pMutex prh_mgr_hold_acl_lock;
static struct st_t_BT_ConnectionInfo *holdAllACLsPendingACLDbEntry;
static void (*MGR_holdAllOpenACLsStartCallback)(t_api);
static void (*MGR_holdAllOpenACLsEndCallback)(t_api);
static u_int8 numPendingStartHolds;
static u_int8 numPendingEndHolds;
volatile u_int8 MGR_holdAllOpenACLsEndCallbackStatus;

void (*inquiryCallbackFunc[MAX_OUTSTANDING_CALLBACKS])(struct st_t_BT_DeviceEntry *newDevice, t_api status) ={0};
void (*discoveryCallbackFunc[MAX_OUTSTANDING_CALLBACKS])(struct st_t_BT_DeviceEntry *newDevice, t_api status) ={0};

extern t_bdaddr prh_mgr_auth_supplied_link_key_addr;
extern u_int8 prh_mgr_auth_supplied_link_key;

static void prh_mgr_DiscoveryCanceledComplete();


/*************************************************************************************************
* FUNCTION :- MGR_Init
*
* DESCRIPTION :- External API Stack startup function. This does not start or stop the stack, but
*                cound be used to track running applications, start/stop runtime libs, etc. 
************************************************************************************************/
APIDECL1 t_api APIDECL2 MGR_Init(u_int32 flags) {

    volatile extern t_api prh_host_startup_complete; 

    if (prh_host_startup_complete==BT_INVALIDERRORCODE) 
        return BT_RETRY;
    else
        return prh_host_startup_complete; 
}


/*************************************************************************************************
* FUNCTION :- MGR_Shutdown
*
* DESCRIPTION :- External API Stack shutdown function. This does not start or stop the stack, but
*                cound be used to track running applications, start/stop runtime libs, etc. 
************************************************************************************************/
APIDECL1 t_api APIDECL2 MGR_Shutdown(u_int32 flags) {
    return BT_NOERROR; 
}

/*************************************************************************************************
* FUNCTION :- prh_manager_layer_shutdown
*
* DESCRIPTION :- This function frees any alloced memory and shuts down the manager
************************************************************************************************/
void prh_manager_layer_shutdown(void) {
    struct st_t_BT_ConnectionInfo *currentDevice, *temp;
	int lcv;

    /* deallocate device database */
    currentDevice=prh_device_list_head;
    while (currentDevice) {
#if pDEBUG
		if (currentDevice->aclState==CONN_ACTIVE)      
            pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_manager_shutdown: Shutting down while device %s connection handle 0x%02X still active\n",prh_mgr_printBDAddress(currentDevice->bdAddress),currentDevice->aclHandle));
		if (currentDevice->recordState)      
            pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_manager_shutdown: Shutting down while device %s record state=0x%02X\n",prh_mgr_printBDAddress(currentDevice->bdAddress),currentDevice->recordState));
#endif
	    if (currentDevice->linkKey)
		    pFree(currentDevice->linkKey);

		for(lcv=0;lcv<MAX_OUTSTANDING_CALLBACKS; lcv++)
		{
			if (currentDevice->authorisationInfo[lcv])
				pFree(currentDevice->authorisationInfo[lcv]);
		}
		
        temp=currentDevice;
        currentDevice=currentDevice->nextDevice;
        pFree(temp);
    }

    prh_free_inquiry_db(); 

    MGR_FlushSecurityDatabase(0);

    if (prh_mgr_LocalName)
        pFree(prh_mgr_LocalName);

    /* release manager global resources  */
    pMutexFree(prh_mgr_hold_acl_lock);
    pMutexFree(prh_mgr_conndb_list_lock);

}
/*************************************************************************************************
* FUNCTION :- prh_manager_layer_startup
*
* DESCRIPTION :- This function starts up the manager layer and initialises the relevant variables
************************************************************************************************/
void prh_manager_layer_startup(void) {
    /* Init global variables */
	prh_mgr_auth_supplied_link_key = 0;
    prh_mgr_numDeviceDatabaseEntries=0;
    prh_device_list_head=NULL;
    prh_mgr_LocalName = NULL;
    prh_mgr_ReadLocalFeaturesComplete=FALSE;
    prh_mgr_currentMode = MGR_DEVACTIVE;
    prh_mgr_currentRole = ROLE_MASTER;
    prh_mgr_periodicInquiryModeActive = FALSE;
    prh_mgr_inquiryInProgress = FALSE;
    prh_mgr_discoveryInProgress = FALSE;
    prh_mgr_scan_enable = PRH_MGR_NOSCANENABLE;
    prh_mgr_scan_enable_pending = PRH_MGR_NOSCANENABLE;
	prh_mgr_liac_present = 0x0;
	prh_mgr_liac_present_pending = 0x0;
	prh_mgr_mode_flag = 0x0;
	holdAllACLsPendingACLDbEntry = NULL;
    MGR_holdAllOpenACLsStartCallback = NULL;
	MGR_holdAllOpenACLsEndCallback = NULL;
    numPendingStartHolds = 0;
	numPendingEndHolds = 0;
    prh_mgr_linkFlags=MGR_LINKFLAGS_DEFAULT; 
    prh_mgr_authenticationMode=0x00;        /* default setting from the spec. */
    prh_mgr_pageTimeout=0x2000;             /* default setting from the spec. */
    prh_mgr_connectionAcceptTimeout=0x1FA0; /* default setting from the spec. */  
    prh_mgr_conn_accept_flag=1;
	prh_mgr_hold_acl_lock = pMutexCreate(FALSE);
    prh_mgr_conndb_list_lock = pMutexCreate(FALSE);
	prh_security_mode = MGR_SERVICELEVEL; /* If this is linklevel we must call HCI_WriteAuthenticationEnable(0x01) */

	prh_mgr_disoverFlag = 0;
	prh_mgr_DiscoveryTimeout = 1;

    prh_hci_register_manager_event_cb(prh_mgr_callbackHCI_Ctrl);

    prh_init_inquiry_db();

    pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Size of manager device database entry == %d bytes\n",sizeof(struct st_t_BT_ConnectionInfo)));
    pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Size of manager inquiry database entry == %d bytes\n",sizeof(struct st_t_BT_InquiryInfo)));
    pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Size of host_buf == %d bytes\n",sizeof(struct host_buf)));
}
/*************************************************************************************************
* FUNCTION :- prh_manager_layer_ctrl
*
* DESCRIPTION :- This function allows the manager to be passed a flag which controls 
*									whether the stack should be started up or shutdown
************************************************************************************************/
t_api prh_manager_layer_ctrl(u_int8 flags) {

	if (flags&PRH_HOST_STARTUP || flags&PRH_HOST_RESET) {
		if (flags&PRH_HOST_RESET) 
			prh_manager_layer_shutdown();		
		prh_manager_layer_startup();
	} else if (flags&PRH_HOST_SHUTDOWN) {
		prh_manager_layer_shutdown();	
	}
    return BT_NOERROR;
}

/*************************************************************************************************
* FUNCTION :- MGR_SetLinkFlags
*
* DESCRIPTION :- This function allows link related flags to be passed to the manager
************************************************************************************************/
APIDECL1 t_api APIDECL2 MGR_SetLinkFlags(u_int32 flags) {
    u_int16 temp;
    u_int32 tempFlags;

    tempFlags=prh_mgr_linkFlags;

	/* Passing in flags value of 0 Sets Link Flags back to default value */
	if (flags==0)
		flags=MGR_LINKFLAGS_DEFAULT;

    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_SetLinkFlags called, flags = 0x%02X\n",flags));

    temp=flags&(0x7<<0);
    if (temp==MGR_KEYREQ_HANDLE || temp==MGR_KEYREQ_IGNORE || temp==MGR_KEYREQ_REJECT || temp==0) {
		if (temp)
			tempFlags=(tempFlags&(~(0x7<<0)))|temp;
	} else
		return BT_INVALIDPARAM;

    temp=flags&(0x7<<3);
    if (temp==MGR_PINREQ_HANDLE || temp==MGR_PINREQ_IGNORE || temp==MGR_PINREQ_REJECT || temp==0) {
		if (temp)
			tempFlags=(tempFlags&(~(0x7<<3)))|temp;
	} else
        return BT_INVALIDPARAM;

    temp=flags&(0x7<<6);
    if (temp==MGR_CONNREQ_SCO_ACCEPT || temp==MGR_CONNREQ_SCO_IGNORE || temp==MGR_CONNREQ_SCO_REJECT || temp==0) {
		if (temp)
			tempFlags=(tempFlags&(~(0x7<<6)))|temp;
	} else
        return BT_INVALIDPARAM;

    temp=flags&(0xF<<9);
    if (temp==MGR_CONNREQ_ACL_ACCEPT || temp==MGR_CONNREQ_ACL_IGNORE || temp==MGR_CONNREQ_ACL_REJECT || temp==(MGR_CONNREQ_ACL_SWITCH) || temp==0) {
		if (temp)
			tempFlags=(tempFlags&(~(0xF<<9)))|temp;
	} else
        return BT_INVALIDPARAM;

    temp=flags&(0x3<<13);
    if (temp==MGR_CONNSETUP_ALLOW_SWITCH || temp==MGR_CONNSETUP_REFUSE_SWITCH || temp==0) {
		if (temp)
			tempFlags=(tempFlags&(~(0x3<<13)))|temp;
	} else
        return BT_INVALIDPARAM;
    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_SetLinkFlags setting flags to 0x%02X\n",tempFlags));


    /* The MGR_DISABLE_MANAGER_RXPATH flag is for Parthus internal use only and is unsupported */
    if (flags&MGR_DISABLE_MANAGER_RXPATH) 
            prh_hci_register_manager_event_cb(NULL);
    else if (prh_mgr_linkFlags&MGR_DISABLE_MANAGER_RXPATH) 
            prh_hci_register_manager_event_cb(prh_mgr_callbackHCI_Ctrl);

    prh_mgr_linkFlags=tempFlags;
    
    return BT_NOERROR;
}

/*************************************************************************************************
* FUNCTION :- MGR_GetLinkFlags
*
* DESCRIPTION :- returns whether the connection accept flag is set or not.
************************************************************************************************/
APIDECL1 u_int32 APIDECL2 MGR_GetLinkFlags(void) {
    return prh_mgr_linkFlags;
}


void prh_mgr_name_normal_callback(t_api status,struct st_t_BT_ConnectionInfo *dbEntry)
{
	u_int8 i;	
	/* Invoke callbacks for the MGR_GetRemoteName primitive */
	void (*tempFunc)(t_bdaddr address, t_api status_t);
	do {
		prh_mgr_get_callback(&i,(void **)dbEntry->nameCallbackFunc,(void **)&tempFunc,dbEntry);
		if (tempFunc) {					
			tempFunc(dbEntry->bdAddress,status);
		}
	} while (tempFunc);
}

t_api prh_mgr_name_normal_request(t_bdaddr address)
{
	struct st_t_BT_InquiryInfo *inquiryInfo;
	t_api retVal;

	inquiryInfo=prh_mgr_retrieve_inquiry_entry(address);
	if (inquiryInfo) {
		retVal=HCI_RemoteNameRequest(address,inquiryInfo->pageScanRepetitionMode,inquiryInfo->pageScanMode,inquiryInfo->clockOffset);
		prh_mgr_release_inquiry_entry(inquiryInfo);
	} else {
		retVal=HCI_RemoteNameRequest(address,HOST_PAGING_DEFAULT_PSRM,HOST_PAGING_DEFAULT_PSM,HOST_PAGING_DEFAULT_CLKOFFSET);
	}

	return retVal;
}

void prh_mgr_name_callback_haoa_end(t_api status) 
{
	/* Is a error in the name request trying to send back the status */

	if(prh_mgr_pendingNameStatus != BT_NOERROR) 
        status = prh_mgr_pendingNameStatus;

	prh_mgr_name_normal_callback(status,prh_mgr_pendingNameEntry);
	
	/* reset entry */
	prh_mgr_pendingNameEntry = NULL; 
}

/*************************************************************************************************
* FUNCTION :- prh_mgr_name_callback_holdAllOpenACLs
*
* DESCRIPTION :- If successful, callback with the results.
* 
************************************************************************************************/
void prh_mgr_name_callback_haoa_start(t_api status) 
{
	t_api retVal = BT_NOERROR;
	
    if (!status) {
		retVal = prh_mgr_name_normal_request(prh_mgr_pendingNameEntry->bdAddress);
	} else {
		retVal=BT_UNKNOWNERROR;
	}

	if(retVal) {		
		prh_mgr_name_callback_haoa_end(retVal);		
    }
}




/*************************************************************************************************
* FUNCTION :- MGR_GetRemoteName
*
* Input Parameters
*      t_bdaddr address    The address of the Bluetooth device from which the name is to be obtained.
*      u_int8 bufLen       The length of the buffer.
*      char* outBuffer     A buffer in which the remote device name is to be stored
*      callbackFunc        The function to be called back when the HCI operation is complete.
*
* Returns
*      char* outBuffer     A buffer containing the remote device name.
*      u_int8* nameLen     The total length of the name.
*.
* DESCRIPTION :- 
* This function is used to deterine the the "user friendly" name of an other bluetooth device.
* If the outBuffer is not large enough for the name, it is truncated but is always NUL-terminated.
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_GetRemoteName(t_bdaddr address, char *outBuffer, u_int8 bufLen, u_int8 *nameLen, void (*callbackFunc)(t_bdaddr address, t_api status))
{

    struct st_t_BT_InquiryInfo *inquiryInfo;
    struct st_t_BT_ConnectionInfo *dbEntry;
    u_int8 i;
    t_api status=BT_NOERROR;
    
    /* If the name is already in the database, return it back immediately */
    /* If the name is not found and a callback is passed, send HCI_RemoteNameRequest and callback app later */
    /* If no callback was passed, return the name if available, do nothing otherwise */
        
    pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"MGR_GetRemoteName called for %s\n",prh_mgr_printBDAddress(address)));


#if MGR_DEVDB_CACHE_REMOTE_NAME
    inquiryInfo=prh_mgr_retrieve_inquiry_entry(address);
    if (inquiryInfo && inquiryInfo->deviceName) { 
        if (outBuffer) {
	        pStrncpy(outBuffer,inquiryInfo->deviceName,bufLen-1);
	        outBuffer[bufLen-1]='\0';	        
        }
	    if (nameLen)
		    *nameLen=pStrlen(inquiryInfo->deviceName);

        return BT_NOERROR;
    }
    prh_mgr_release_inquiry_entry(inquiryInfo);
#endif

    if(callbackFunc) { 
        /* retrieve the name and return make a callback when available */
        dbEntry=prh_mgr_return_deviceEntryByAddress(address);
        
        if (!dbEntry) 
            return BT_NORESOURCES;
        
        status=prh_mgr_set_callback(&i,(void **)dbEntry->nameCallbackFunc,(void *)callbackFunc,dbEntry);
        if (status)
            return status;
        
        /* there was an empty callback slot at offset i */  
        dbEntry->nameCallbackBufLen[i] = bufLen;
        dbEntry->nameCallbackNameLen[i] = nameLen;
        dbEntry->nameCallbackOutBuffer[i] = (u_int8 *)outBuffer;
        pMemcpy(&prh_mgr_pendingBDAddress.bytes,&address.bytes,6);
	
#if MGR_HOLD_DURING_PAGE
		if(dbEntry->aclState != CONN_ACTIVE)
		{
			status = prh_mgr_holdAllOpenACLs((u_int16)(prh_mgr_pageTimeout+0x400),(u_int16)(prh_mgr_pageTimeout+0x800),prh_mgr_name_callback_haoa_start,prh_mgr_name_callback_haoa_end);
        }
#endif

		if(status == BT_PENDING)
		{
			/* setup info needed */
			prh_mgr_pendingNameEntry = dbEntry;
			prh_mgr_pendingNameStatus = BT_NOERROR;
		} else if (!status) {			
			prh_mgr_pendingNameEntry = NULL;

			status = prh_mgr_name_normal_request(address);			
		}

        if (status)
            return status;
        else
            return BT_PENDING; 
    } else   
        return BT_NOERROR;  
}

/*************************************************************************************************
* FUNCTION :- MGR_AddSCO
*
*
*  u_int16 aclHandle   The connection handle for an ACL link
*  u_int16 scoHandle   A pointer to the connection handle that will be allocated to the SCO link.
*  u_int16 packetType  The SCO packet types to be used on the SCO link. A bitwise Or of the following.
*                                  0x20  HV1
*                                  0x40  HV2
*                                  0x80  HV3
*
*  callbackfunc        The function to be called one the addition of the SCO link is complete. 
*                                  
* DESCRIPTION :- 
* Adds a SCO to an existing ACL link. The source of the SCO data is assumed to be on the
* Host and SCO packets will be delivered to the baseband via the HCI.
*
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_AddSCO(u_int16 aclHandle, u_int16 *scoHandle, u_int16 packetType, void (*callbackFunc)(u_int16 *scoHandle, t_api status)) {
    return prh_mgr_add_sco(aclHandle,scoHandle,MGR_HCI_SCO,packetType,0,callbackFunc);
}


/*************************************************************************************************
* FUNCTION :- MGR_ReleaseSCO
*
* Input Parameters
*      u_int16 handle  The connection handle of the SCO link.
*      u_int8 reason   The release reason which will be presented to peer device on the SCO link.
*
* Returns 
*      t_api           The outcome of the operation
* DESCRIPTION :- 
* Releases a SCO link, by removing it from the managers device database and then releasing the SCO
* in the lower protocol stack.
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_ReleaseSCO(u_int16 handle, u_int8 reason, void (*callbackFunc)(u_int16 scoHandle, t_api status)) 
{
    struct st_t_BT_ConnectionInfo *dbEntry;
    int i;
    t_api retval=BT_NOERROR;
	u_int8 linkType;
    dbEntry=prh_mgr_get_deviceEntryByHandle(handle, &linkType);
    pDebugPrintfEX((pLOGNOTICE, pLOGHFP,"MGR_ReleaseSCO dbEntry=0x%x linkType=%d\n",dbEntry, linkType));
    
	if(!dbEntry || (HCI_linkSCO != linkType))
		return BT_INVALIDPARAM;
	
    for (i=0; i<3; i++) 
    {
        if (dbEntry->scoHandle[i]==handle)
            break;
    }
    if (i==3) 
    {
        return BT_INVALIDPARAM;
    }
    
        pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_ReleaseSCO dbEntry->scoState[i]=%d\n",dbEntry->scoState[i]));
	/* Should not be possible to call this for handle with state CONN_PENDING */
	switch(dbEntry->scoState[i])
	{
	case CONN_ACTIVE:
	  if(BT_NOERROR==(retval= HCI_Disconnect(handle,reason)))
	    {
          dbEntry->scoState[i]=CONN_RELEASEPENDING;
	      retval = BT_PENDING;
	      prh_mgr_pendingDisconnectHandle = handle;
	      dbEntry->scoReleaseCallbackFunc[i] = callbackFunc;
	    }
	  break;

	case CONN_CLOSED:
		retval = BT_NOERROR;
		break;
		
	default:
        retval=BT_UNKNOWNERROR;
		break;
	}
    
    return retval;
}


/*************************************************************************************************
* FUNCTION :- MGR_AddHWSCO
*
*  u_int16 aclHandle   The connection handle for an ACL link
*  u_int16 scoHandle   A pointer to the connection handle that will be allocated to the SCO link.
*  u_int16 packetType  The SCO packet types to be used on the SCO link. A bitwise Or of the following.
*                                  0x20  HV1
*                                  0x40  HV2
*                                  0x80  HV3
*
*  u_int32 hwConfig    The configuration values for the Baseband Codec.
*  callbackfunc        The function to be called one the addition of the SCO link is complete. 
*                                  
* DESCRIPTION :- 
* Adds a Hardware SCO to an existing ACL link. The source of the SCO data is assumed to be on the
* the other side of the HCI ( i.e Codec co-located with baseband).
************************************************************************************************/

t_api MGR_AddHWSCO(u_int16 aclHandle, u_int16 *scoHandle, u_int16 packetType, u_int32 hwConfig, void (*callbackFunc)(u_int16 *scoHandle, t_api status)) {
    return prh_mgr_add_sco(aclHandle,scoHandle,MGR_HW_SCO,packetType,hwConfig,callbackFunc);
}


t_api prh_mgr_add_sco(u_int16 aclHandle, u_int16 *scoHandle, u_int8 scoType, u_int16 packetType, u_int32 hwConfig, void (*callbackFunc)(u_int16 *scoHandle, t_api status)){
	
    struct st_t_BT_ConnectionInfo *dbEntry;
    t_api status;
	u_int8 linkType;
    u_int8 i,j;
    
    dbEntry=prh_mgr_get_deviceEntryByHandle(aclHandle, &linkType);
    if (!dbEntry || HCI_linkACL!=linkType) 
        return BT_INVALIDPARAM;
    

    for (i=0; i<MAX_SCO_LINKS; i++)     {
        if (dbEntry->scoState[i]==CONN_CLOSED)
            break;
    }
    if (i==MAX_SCO_LINKS) {
        /* all SCOs are open or pending */
        return BT_NORESOURCES;
    }
    
    status=prh_mgr_set_callback(&j,(void **)dbEntry->scoCallbackFunc,(void *)callbackFunc,dbEntry);
    if (status)
        return status;
    dbEntry->scoCallbackReturn[j] = scoHandle;

    dbEntry->scoState[i]=CONN_PENDING;  
	
    if (scoType==MGR_HW_SCO)
			status = HCI_AddHardwareSCO(aclHandle,hwConfig);     
    else /* scoType==MGR_HCI_SCO */
        status = HCI_AddSCOConnection(aclHandle,packetType);

    if(!status) {
		return BT_PENDING;
    } else {
		dbEntry->scoCallbackFunc[j] = NULL; /* cancel callback */
		dbEntry->scoState[i]=CONN_CLOSED;  
		return status;
	}
}


    // ? Shouldn't we check if a Discovery is already in progress before setting the callback.

/*************************************************************************************************
* FUNCTION :- MGR_Inquiry
*
* Input Parameters
*  u_int8 inquiryLength    The length of time the inquiry should be performed for ( N * 1.28 sec)
*  u_int32 accessCode      The Inquiry Access Code which is to be used for inquiry
*  u_int8 maxResults       The Number of inquiry results for the inquiry after this number the inquiry 
*                          will be automatically stopped by the lower layers of the stack.
*
*  callbackFunc            The callback function used when inquiry is complete
*                                  struct st_t_BT_ConnectionInfo    *newDevice
*                                  t_api               status
* Returns
*  t_api                           
*
* DESCRIPTION :- 
*   Performs the inquiry and calls the user back for each address. sets complete when complete.
*
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_Inquiry(u_int8 inquiryLength, u_int32 accessCode, u_int8 maxResults, void (*callbackFunc)(struct st_t_BT_DeviceEntry *newDevice, t_api status)) {
    u_int8 i;
    t_api retVal=BT_NOERROR;


	if (prh_mgr_inquiryInProgress) {
		retVal = BT_RETRY;
	} else {
		prh_mgr_inquiryInProgress = TRUE;
	}

	retVal=prh_mgr_set_callback(&i,(void **)inquiryCallbackFunc,(void *)callbackFunc,NULL);

        if (retVal)
		{
			prh_mgr_inquiryInProgress = FALSE;
            return retVal;
		}
#if MGR_HOLD_DURING_INQUIRY
        retVal = prh_mgr_holdAllOpenACLs((u_int16)(inquiryLength*0x800+0x200),(u_int16)(inquiryLength*0x800+0x700), prh_mgr_inquiry_callback_holdAllOpenACLs, NULL);
#endif
        if(retVal==BT_PENDING)
		{
            pendingInquiryLength = inquiryLength;
            pendingInquiryAccessCode = accessCode;
            pendingInquiryMaxResults = maxResults;
            
        } 
		else if(!retVal) 
		{//0xA1A2, 0xB1B2, 0x2A2B2C, 0x33, 0x44
			prh_mgr_deviceNumFound = 0;
			prh_mgr_deviceNameRetrieved = 0;
			prh_mgr_nameRequestStarted = FALSE;
			prh_mgr_InquiryCanceled = 0;		
			prh_mgr_DiscoveryTimeout = 0;
			
			retVal=HCI_Inquiry(accessCode, inquiryLength, maxResults);
            if (!retVal) 
			{	
#ifdef SLICE_INQUIRY_SUPPORT

				if( prh_mgr_IsNameNeedRetrieve == NEED_TO_RETRIEVE_NAME)
				{
				HCI_WritePageTimeout(0x1350);
				}
				
				prh_mgr_disoverFlag = 1;
				pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"next will create a timer for inquiry process..............\n"));

				prh_mgr_InquiryTimerHandle = pTimerCreate((inquiryLength-3)*10, prh_mgr_DiscoveryTimerCallBack, NULL, pTIMER_ONESHOT);
#endif				
				retVal=BT_PENDING;
            } 
			else 
			{
                /* Inquiry has failed, so remove the callback */
				inquiryCallbackFunc[i] = NULL;
				prh_mgr_inquiryInProgress = FALSE;
			}
		}
		
    return retVal;
}

/*************************************************************************************************
* FUNCTION :- MGR_InitDeviceDiscovery
*
* DESCRIPTION :- This primitive initiates a device discovery procedure. This procedure involves 
*	                an inquiry procedure followed by retrieval of the Bluetooth name of each of the 
*                 devices found in the inquiry procedure. If an inquiry is ongoing, either from a 
*                 periodic inquiry or application-initiated, this primitive returns a BT_NORESOURCES 
*                 error.  The inquiry duration in seconds specified by the inquiryLength parameter 
*                 is rounded to multiples of 1.28 seconds.
************************************************************************************************/

APIDECL1 t_api APIDECL2  MGR_InitDeviceDiscovery(u_int8 IsNameNeeded, u_int32 classOfdevice, u_int8 discoveryLength, u_int32 accessCode, u_int8 maxResults, void (*callbackFunc)(struct st_t_BT_DeviceEntry *newDevice, t_api status)) {
	u_int8 i;
	t_api retVal=BT_NOERROR;
	struct st_t_BT_ConnectionInfo *currentDevice=prh_device_list_head;	

	extern u_int32 prh_mgr_inqdb_num_entries;
	
	
    	pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_InitDeviceDiscovery called, db entries number:%d IsNameNeeded=%d\n", prh_mgr_inqdb_num_entries,IsNameNeeded));


	prh_mgr_MaxResult = maxResults;
	prh_mgr_IsNameNeedRetrieve = IsNameNeeded;
	prh_mgr_AccessCode = accessCode;
	prh_mgr_ClassofDeviceToRetrieve = classOfdevice;
	theEntryRetrievingName = NULL;

    	pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"classOfdevice%d,   param callbackFunc is NULL? %d\n", prh_mgr_ClassofDeviceToRetrieve, callbackFunc?0:1));
	

	/* If there is a device discovery already 
			in progress 
	*/
	if(prh_mgr_discoveryInProgress == TRUE)
	{
			/* Do not start a new device discovery
			   The application must wait 
			*/
		pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"inquiry In process..................\n"));

		return BT_RETRY;
	}

    
    /* check that no ACLS are up */
	while(currentDevice) {
		if(currentDevice->aclState!=CONN_CLOSED) 
			break;
		currentDevice = currentDevice->nextDevice;
	}
    if (currentDevice) {
        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"currentDevice->aclState=%d\n", currentDevice->aclState));
        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"MGR_InitDeviceDiscovery called but ACLs active - returning BT_DISALOWED\n"));
        return BT_DISALLOWED;
    }

   	prh_free_inquiry_db();

    pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"MGR_InitDeviceDiscovery called - starting inquiry\n"));

	retVal=prh_mgr_set_callback(&i,(void **)discoveryCallbackFunc,(void *)callbackFunc,NULL);
    if (retVal) {
        return retVal;
    } else {		
        prh_mgr_discoveryInProgress = TRUE;
		
		/*modified by tan .., in order to add a switch to decide whether retrieve device name*/
		if(prh_mgr_IsNameNeedRetrieve == NEED_TO_RETRIEVE_NAME)			
			retVal = MGR_Inquiry(discoveryLength, accessCode, maxResults, prh_mgr_callback_DeviceDiscovery);
		else
			retVal = MGR_Inquiry(discoveryLength, accessCode, maxResults, callbackFunc);

		/* If the inquiry could not be started */
		if(!((retVal == BT_PENDING) || (retVal == BT_NOERROR)))
		{
			/* Remove the discovery callback function stored */
			discoveryCallbackFunc[i] = NULL;
			prh_mgr_discoveryInProgress = FALSE;
		}
	}
	
	return retVal;
}


APIDECL1 t_api APIDECL2 MGR_InquiryCancel(void (*InquiryCancelCallBackfun)(int num, int  status))
{
	int status = 0;
	if( prh_mgr_discoveryInProgress == FALSE)
	{
		pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"not in inquiry process, so call back directly\n"));
		InquiryCancelCallBackfun(0, 0);
		return 1;
	}
	
	prh_mgr_InquiryCanceled = 1;
	prh_mgr_InquiryCancelCallBack = InquiryCancelCallBackfun;
	
	if(	prh_mgr_DiscoveryTimeout == 0)
	{
		pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Inquiry process will be canceled, so shut down the timer.....\n"));
		status = pTimerCancel(prh_mgr_InquiryTimerHandle);
		prh_mgr_InquiryTimerHandle=0;
		prh_mgr_DiscoveryTimeout = 1;
	}

	if(prh_mgr_disoverFlag == 1 || prh_mgr_IsNameNeedRetrieve == NONEED_TO_RETRIEVE_NAME)
	{
	    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_InquiryCancel, Inquiry in process, so call HCI_InquiryCancel......\n"));
	    status = HCI_InquiryCancel();
        }
	else if(prh_mgr_IsNameNeedRetrieve == NEED_TO_RETRIEVE_NAME)
        {
              pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_InquiryCancel, It since we should wait a moment till......(retrieving name process) \n"));    
		if( theEntryRetrievingName)
			HCI_RemoteNameRequestCancel(theEntryRetrievingName->bdAddress);
        }   


	return status;
	
}

/*************************************************************************************************
* FUNCTION :- MGR_GetLocalFeatures
*
* Input Parameters
*      u_int8* outBuffer   The buffer into which the Supported features are to be copied.
*      
* DESCRIPTION :- 
* Reads the local supported features of the device. These are obtained from the lower protocol 
* stack at Host Startup.
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_GetLocalFeatures(u_int8 *outBuffer)
{
    u_int8 status;

    if (prh_mgr_ReadLocalFeaturesComplete) {
        pMemcpy(outBuffer, prh_mgr_localLMPFeatures, NUM_BYTES_LMP_FEATURES);
        return BT_NOERROR;
    } else {
        status = HCI_ReadLocalSupportedFeatures();
        if(BT_NOERROR == status)
            return BT_RETRY;
        else
            return status;
    }
}


/*************************************************************************************************
* FUNCTION :- MGR_GetLocalBDAddress
*
* DESCRIPTION :- 
* Returns the Bluetooth address of the local device.
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_GetLocalBDAddress(t_bdaddr *address)
{  
	prh_mgr_copyBDAddress(address,&prh_mgr_LocalBDADDR);
	return BT_NOERROR;
}


/*************************************************************************************************
* FUNCTION :- MGR_HintMTU
*
* 
* DESCRIPTION :- 
* Used to suggest an MTU to be used for a specific protocol or service.
* Currently the SDP is the only Protocol which uses the hint.
************************************************************************************************/

APIDECL1 u_int16 APIDECL2 MGR_HintMTU(u_int16 psm)
{
	return 512; 
}



/*************************************************************************************************
* FUNCTION :- MGR_SetMode
*
* DESCRIPTION :- Sets Connectable and Discoverable mode. Overwrites ALL (Conn + Disc) previous modes
*               with the one that is being set presently.
* 
************************************************************************************************/
APIDECL1 t_api APIDECL2 MGR_SetMode(u_int8 mode, void (*callbackFunc)(t_api status))
{
	u_int8 newScanEnable = 0x00;
	u_int8 retStatus = BT_NOERROR;

    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_SetMode called with new mode 0x%02X\n",mode));

	/* check if current mode is pending */
	if(PRH_MGR_MODEPENDING(prh_mgr_mode_flag))
		return BT_RETRY;
	prh_mgr_mode_flag |= PRH_MGR_SETMODEPENDING_BIT;
	
	/* this may be NULL if app doesn't need success/failure notification */
	prh_mgr_mode_callbackFunc = callbackFunc;

	/* Check if Scan Enable needs to be changed */
	if(PRH_MGR_SET_DISCOVERABLE(mode))
		newScanEnable |= PRH_MGR_INQUIRYSCANENABLE;
	if(PRH_MGR_SET_CONNECTABLE(mode))
		newScanEnable |= PRH_MGR_PAGESCANENABLE;
	if(prh_mgr_scan_enable != newScanEnable)
	{
		retStatus = HCI_WriteScanEnable(newScanEnable);
		if(retStatus == BT_NOERROR)
			retStatus = BT_PENDING;
	}
	
    /* 
        Should handle an error from HCI_WriteScanEnable or HCI_WriteCurrentIACLAP 
        in this function - clear callback and return.
    */


	/* Check if IACLAP list needs to be changed */
	if(PRH_MGR_SET_LIMDISCOVERABLE(mode))
	{
		if(!prh_mgr_liac_present)
		{
			retStatus = HCI_WriteCurrentIACLAP(0x2, (u_int *)PRH_GIAC_PLUS_LIAC);
			if(retStatus == BT_NOERROR)
				retStatus = BT_PENDING;
		}
	}
	else if(prh_mgr_liac_present)
	{
		retStatus = HCI_WriteCurrentIACLAP(0x1, (u_int *)PRH_GIAC);
		if(retStatus == BT_NOERROR)
		    retStatus = BT_PENDING;
	}

	/* If retStatus is still BT_NOERROR, nothing was sent to Host Controller => reset callback and 'prh_mgr_mode_flag' */
	if(BT_NOERROR == retStatus)
	{
		prh_mgr_mode_flag ^= PRH_MGR_SETMODEPENDING_BIT;
           if(prh_mgr_mode_callbackFunc)
           {
	       void (*temp_mode_callbackFunc)(t_api status)=NULL;           
		temp_mode_callbackFunc=prh_mgr_mode_callbackFunc;
		prh_mgr_mode_callbackFunc=NULL;
		temp_mode_callbackFunc(HCI_errNOERROR);
           }
      }

	return retStatus;
}
/*************************************************************************************************
* FUNCTION :- MGR_GetMode
*
* DESCRIPTION :- Returns the current Connectable and Discoverable mode.
* 
************************************************************************************************/
APIDECL1 t_api APIDECL2 MGR_GetMode(u_int8 *mode)
{
	u_int8 currentMode = 0x00;

	/* check if current mode is pending */
	if(PRH_MGR_MODEPENDING(prh_mgr_mode_flag))
		return BT_RETRY;

	if(PRH_MGR_INQUIRYSCANENABLE_ON(prh_mgr_scan_enable))
		currentMode |= PRH_MGR_DISCOVERABLE_BIT;
	if(PRH_MGR_PAGESCANENABLE_ON(prh_mgr_scan_enable))
		currentMode |= PRH_MGR_CONNECTABLE_BIT;
	if(prh_mgr_liac_present)
		currentMode |= PRH_MGR_LIMDISCOVERABLE_BIT;
	*mode = currentMode;

	return BT_NOERROR;
}

/*************************************************************************************************
* FUNCTION :- MGR_GetConnectableMode
*
* 
* DESCRIPTION :- 
*
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_GetConnectableMode(u_int8 *mode)
{
	u_int8 currentMode = 0x0;
	t_api status;

	pDebugPrintfEX((pLOGWARNING,pLOGMANAGER,"Manager: Deprecated API function - use MGR_GetMode\n"));
	status = MGR_GetMode(&currentMode);
	*mode = currentMode & PRH_MGR_CONNECTABLE_BIT;
	return status;

}


/*************************************************************************************************
* FUNCTION :- MGR_SetConnectableMode
*
* 
* DESCRIPTION :- 
*
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_SetConnectableMode(u_int8 mode, void (*callbackFunc)(t_api status))
{
	u_int8 currentMode = 0x00;
	t_api status;

	pDebugPrintfEX((pLOGWARNING,pLOGMANAGER,"Manager: Deprecated API function - use MGR_SetMode\n"));
	if(BT_NOERROR != (status= MGR_GetMode(&currentMode)) )
		return status;
	
	return MGR_SetMode( (u_int8)((mode & PRH_MGR_CONNECTABLE_BIT) | (currentMode & ~PRH_MGR_CONNECTABLE_BITMASK) ), callbackFunc);
}


/*************************************************************************************************
* FUNCTION :- MGR_GetDiscoverableMode
*
* 
* DESCRIPTION :- 
*
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_GetDiscoverableMode(u_int8 *mode)
{
	u_int8 currentMode = 0x0;
	t_api status;

	pDebugPrintfEX((pLOGWARNING,pLOGMANAGER,"Manager: Deprecated API function - use MGR_GetMode\n"));
	status = MGR_GetMode(&currentMode);
	*mode = currentMode & PRH_MGR_DISCOVERABLE_BITMASK;
	return status;

}

/*************************************************************************************************
* FUNCTION :- MGR_SetDiscoverableMode
*
* 
* DESCRIPTION :- 
*
************************************************************************************************/


APIDECL1 t_api APIDECL2 MGR_SetDiscoverableMode(u_int8 mode, void (*callbackFunc)(t_api status))
{
	u_int8 currentMode = 0x00;
	t_api status;

	pDebugPrintfEX((pLOGWARNING,pLOGMANAGER,"Manager: Deprecated API function - use MGR_SetMode\n"));
	if(BT_NOERROR != (status= MGR_GetMode(&currentMode)) )
		return status;

	return MGR_SetMode((u_int8)((mode & PRH_MGR_DISCOVERABLE_BITMASK) | (currentMode & ~PRH_MGR_DISCOVERABLE_BITMASK)), callbackFunc);

}


APIDECL1 t_api APIDECL2 MGR_GetDeviceNumFound(void)
{
	if( prh_mgr_IsNameNeedRetrieve == NEED_TO_RETRIEVE_NAME)
		return prh_mgr_deviceNameRetrieved;
	else
		return prh_mgr_deviceNumFound;
}




/* NON-API FUNCTIONS */

/*************************************************************************************************
* FUNCTION :- prh_mgr_holdAllOpenACLs
*
* INPUTS:
*          minHoldInterval: minimum Hold interval.
*          maxHoldInterval: maximum Hold interval.
*
* RETURNS:
*          t_api
*
* DESCRIPTION :- Puts all currently active ACLs on hold according to maximum and minimum Hold
* intervals specified. Sends a HCI Hold Mode command for device in the database that has an
* ACL with state CONN_ACTIVE.
*
************************************************************************************************/

t_api prh_mgr_holdAllOpenACLs(u_int16 minHoldInterval, u_int16 maxHoldInterval, void (*callbackFuncStart)(t_api status), void (*callbackFuncEnd)(t_api status)) 
{
	struct st_t_BT_ConnectionInfo *currentDevice=prh_device_list_head;
	t_api retStatus = BT_NOERROR;
	u_int8 numHoldsRequired = 0;
	

	if(MGR_holdAllOpenACLsStartCallback || MGR_holdAllOpenACLsEndCallback)
		return BT_RETRY;	
	
	pMutexLock(prh_mgr_hold_acl_lock);
	
	/* look ahead to see how many holds required - used to calculate hold intervals */
	while(currentDevice)
	{
		if(currentDevice->aclState==CONN_ACTIVE && MGR_DEVACTIVE == currentDevice->deviceMode) 
			numHoldsRequired++;
		currentDevice = currentDevice->nextDevice;
	}

	if(numHoldsRequired > 0)
	{	
		MGR_holdAllOpenACLsStartCallback = callbackFuncStart;
		MGR_holdAllOpenACLsEndCallback = callbackFuncEnd;
		currentDevice=prh_device_list_head;
		while (currentDevice) {
			if (currentDevice->aclState==CONN_ACTIVE && MGR_DEVACTIVE == currentDevice->deviceMode) 
			{
				currentDevice->deviceMode = MGR_DEVPENDINGHOLD;
				numPendingStartHolds++;
				retStatus = HCI_HoldMode(currentDevice->aclHandle, (u_int16)(maxHoldInterval+((numHoldsRequired-numPendingStartHolds)*PRH_MGR_EXTRAHOLDINTERVALPERDEVICE)), (u_int16)(minHoldInterval+((numHoldsRequired-numPendingStartHolds)*PRH_MGR_EXTRAHOLDINTERVALPERDEVICE)) );
				if(retStatus)
				{
					currentDevice->deviceMode = MGR_DEVACTIVE;
					numPendingStartHolds--; /* leave pending holds to completion, do not attempt to hold remaining ACL(s) */
					MGR_holdAllOpenACLsStartCallback = NULL;
					MGR_holdAllOpenACLsEndCallback = NULL;
					retStatus =  BT_UNKNOWNERROR;
					break;
				}
				else
					retStatus = BT_PENDING;
			}
			currentDevice=currentDevice->nextDevice;
		}
		
	}
	else
		retStatus = BT_NOERROR;

	numPendingEndHolds = numPendingStartHolds;
	pMutexUnlock(prh_mgr_hold_acl_lock);

	return retStatus;

}


/*************************************************************************************************
* FUNCTION :- prh_mgr_createACL
*
* Input Parameters 
*      t_bdaddr target     The Bluetooth address of the target device.
*      u_int16* handle     A pointer to Connection Handle that will be assinged to the ACL link.
*      u_int16 psm         Identifier for the invoking protocol - this is intended to allow the
*                          packet_types and other profile/protocol specific parameters.
*      callbackFunc        This is the address of the function to be called back one the HCI 
*                          operation is complete. It carries the following parameters :
*
*                              tid_t transactionId :- Allows correlation to the original request.
*                              t_api status        :- Error code Indicating the outcome of the operation.
*                              
*      tid_t transactionId Allows correlation between this ACL establisment request and the function called
*                          back on the completion of the HCI operation.
*
* Returns 
*      t_api
*
* DESCRIPTION :- 
* This function triggers the establishment of an ACL link by interacting with the lower stack via the HCI.
* Many of the parameters for the connection establishment are obtained from the device database.
*
************************************************************************************************/

t_api prh_mgr_createACL(t_bdaddr target, u_int16 psm, u_int16 *handle, void (*callbackFunc)(tid_t transactionId, t_api status), tid_t transactionId) 
{
    struct st_t_BT_InquiryInfo *inquiryInfo;
    struct st_t_BT_ConnectionInfo *dbEntry;
	t_api status=BT_NOERROR;
    u_int8 i;
	
	
    dbEntry=prh_mgr_return_deviceEntryByAddress(target);
    if (!dbEntry) 
        return BT_NORESOURCES;

    pDebugPrintfEX((pLOGWARNING,pLOGMANAGER,"prh_mgr_createACL dbEntry->aclState=%d",dbEntry->aclState));
    switch (dbEntry->aclState) 
    {
    case CONN_CLOSED:
    case CONN_PENDING:
        status=prh_mgr_set_callback(&i,(void **)dbEntry->aclCallbackFunc,(void *)callbackFunc,dbEntry);
        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"prh_mgr_createACL status=%d,prh_mgr_currentRole:%d",status,prh_mgr_currentRole));
        if (status)
            return status;
        dbEntry->aclCallbackTid[i]=transactionId;
        dbEntry->aclCallbackReturn[i]=handle;
		
        if (dbEntry->aclState==CONN_CLOSED) 
        {
	        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"CONN_PENDING dbEntry=%x",dbEntry));
			dbEntry->aclState=CONN_PENDING;
            dbEntry->recordState|=MGR_DEVDB_CONNECTION_ACTIVE;
            if (prh_mgr_currentRole==ROLE_MASTER) 
			{
	   	 		pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"prh_mgr_createACL line:%d dbEntry->aclState=%d",__LINE__,dbEntry->aclState));			
				pMemcpy(&prh_mgr_pendingBDAddress.bytes,&target.bytes,6);
				holdAllACLsPendingACLDbEntry = dbEntry;
#if MGR_HOLD_DURING_PAGE
                status = prh_mgr_holdAllOpenACLs((u_int16)(prh_mgr_pageTimeout+0x400),(u_int16)(prh_mgr_pageTimeout+0x800), prh_mgr_createACL_callback_StartholdAllOpenACLs, prh_mgr_createACL_callback_EndholdAllOpenACLs);
#endif
				if(status)
				{
					if(BT_PENDING != status)
						dbEntry->aclState = CONN_CLOSED;
					if(BT_UNSUPPORTED == status)
						status=BT_DISALLOWED; /* If hold is unsupported, return disallowed for second connection attempt */
                    
                   			 dbEntry->recordState&=~MGR_DEVDB_CONNECTION_ACTIVE;
	   	 			pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"prh_mgr_createACL line:%d dbEntry->aclState=%d",__LINE__,dbEntry->aclState));			
					return status;
				}
				else
					holdAllACLsPendingACLDbEntry = NULL;
            }
            else
            {
                /* roleswitch here */
				dbEntry->aclState = CONN_CLOSED;
                dbEntry->recordState&=~MGR_DEVDB_CONNECTION_ACTIVE;
                pDebugPrintfEX((pLOGWARNING,pLOGMANAGER,"Manager: Role switch not currently supported - cannot establish ACL\n"));
                return BT_UNSUPPORTED;
            }

			

            inquiryInfo=prh_mgr_retrieve_inquiry_entry(target);
            if (inquiryInfo) 
            {
                status=HCI_CreateConnection(target,MGR_DEFAULTPACKETTYPE,inquiryInfo->pageScanRepetitionMode,inquiryInfo->pageScanMode,inquiryInfo->clockOffset,(u_int8)((prh_mgr_linkFlags&MGR_CONNSETUP_ALLOW_SWITCH)?1:0));
                prh_mgr_release_inquiry_entry(inquiryInfo);
            } else {
                status=HCI_CreateConnection(target,MGR_DEFAULTPACKETTYPE,HOST_PAGING_DEFAULT_PSRM,HOST_PAGING_DEFAULT_PSM,HOST_PAGING_DEFAULT_CLKOFFSET,(u_int8)((prh_mgr_linkFlags&MGR_CONNSETUP_ALLOW_SWITCH)?1:0));
            }

            pDebugPrintfEX((pLOGWARNING,pLOGMANAGER,"prh_mgr_createACL status=%d",status));

            if (status)
                return status;
		} 
		return BT_PENDING;
		break;

    case CONN_RELEASEPENDING:
        /* an ACL to this device is in the process of being disconnected */
        pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Manager: prh_mgr_createACL found existing ACL pending DisconnectComplete event - returning BT_RETRY\n"));
        return BT_RETRY;
		
#if pDEBUG          
    case CONN_ACTIVE:
        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"Manager: Got prh_mgr_createACL when one already exists!!\n"));
        return BT_UNKNOWNERROR;
        break;
		
    default:
        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"Manager: prh_mgr_createACL found device entry with unknown state - device database possibly corrupt!!\n"));
        return BT_UNKNOWNERROR;
        break;
#endif
    }   
    return BT_UNKNOWNERROR;
}




/*************************************************************************************************
* FUNCTION :- prh_mgr_releaseACL
*
* Input Parameters 
*      u_int16 handle      The Connection Handle for the ACL link to be released.
*      u_int8 reason       The reason for the connection release. For valid set of release
*                          reasons see Bluetooth Specification Ver 1.1 
*
* Returns 
*      t_api
*
* DESCRIPTION :- 
* This function triggers to release of an ACL link by interacting with the lower stack via the HCI
************************************************************************************************/

t_api prh_mgr_releaseACL(u_int16 handle, u_int8 reason, void (*callbackFunc)(struct st_t_BT_ConnectionInfo *dbEntry, t_api status))
{
    struct st_t_BT_ConnectionInfo *dbEntry;
    u_int8 i;
    t_api retval=BT_NOERROR;
    t_api status;
	u_int8 linkType;
    dbEntry=prh_mgr_get_deviceEntryByHandle(handle, &linkType);
    
	if(!dbEntry || (HCI_linkACL != linkType))
	{
		pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Manager: Cannot find device entry for ACL endpoint!!\n"));
		return BT_INVALIDPARAM;
	}
	pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"prh_mgr_releaseACL dbEntry->aclState = %d \n",dbEntry->aclState));
	/* Should not be possible to call this for handle with state CONN_PENDING */
	switch(dbEntry->aclState)
	{
    case CONN_ACTIVE:
    case CONN_RELEASEPENDING:
        if(callbackFunc) {/* if the caller wants to be notified when the release is complete */
   	    	prh_mgr_pendingDisconnectHandle = handle;
            status=prh_mgr_set_callback(&i,(void **)dbEntry->aclReleaseCallbackFunc,(void *)callbackFunc,dbEntry);
            if (status)
                return status;
	     pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"prh_mgr_set_callback callbackFunc= 0x%x \n",callbackFunc));
        }
		/* If state is already CONN_RELEASEPENDING, only need to setup callback and wait for disconnection complete event */
        if (CONN_ACTIVE == dbEntry->aclState) 
        {
			dbEntry->aclState = CONN_RELEASEPENDING;
			if(BT_NOERROR == HCI_Disconnect(handle,reason))
				retval = BT_PENDING;
			else
			{
				dbEntry->aclReleaseCallbackFunc[i] = NULL;
				retval = BT_UNKNOWNERROR;
			}
		}
		
		break;
		
	case CONN_CLOSED:
		retval = BT_NOERROR;
		break;
		
	default:
		break;
	}
	pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"prh_mgr_releaseACL retval= %d \n",retval));
    return retval;
}

extern u_int8 Bt_init_Finished;
/*************************************************************************************************
* FUNCTION :- prh_mgr_callbackHCI_Ctrl
*
*  Input Parameters
*      u_int8 *pdu     A pointer to the first byte of the HCI event PDU.
*
*  Returns 
*      t_api           BT_NOERROR
*                      BT_UNKNOWNERROR
*                      BT_UNSUPPORTED
* DESCRIPTION :- 
* This is the handler for HCI events. 
************************************************************************************************/
	extern void rdabt_send_ready_ind(void);//zhou siyou20100611

t_api prh_mgr_callbackHCI_Ctrl(u_int8 *pdu) 
{
    u_int8 i,j;
	static	u_int8 InquiryCanceled = 0;
    struct st_t_BT_ConnectionInfo *dbEntry = NULL;
    u_int8 linkType;
	u_int8 eventType;
    u_int8 eventStatus;
    t_bdaddr BDAddress;
    u_int16 aclHandle;
    u_int16 cmdOpcode;
    u_int8 nameLen;
    u_int8 mgrStatus = BT_NOERROR;
	u_int16 handle;

//	u_int16 status;

	u_int16 interval;
	
	u_int8 currentMode;
	void (*temp_mode_callbackFunc)(t_api status_t)=NULL;
    u_int8 *inquiryResult=NULL;
    struct st_t_BT_InquiryInfo *inquiryInfo=NULL;
	
    eventType = pdu[0];
    eventStatus = pdu[2]; /* this is in the general case, some events are exceptions */
	
    switch (eventType) 
    {
		
    case HCI_evINQUIRYRESULT:
	if(prh_mgr_InquiryCanceled == 1)
	{
		InquiryCanceled = 0;
		pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Inquiry process has been canceled, so discard the result...............\n"));
		break;
	}
        prh_mgr_inquiryInProgress=TRUE; /* This is needed in the case of a periodic inquiry */
        for (i=0; i<pdu[2]; i++) 
        {
            inquiryResult=pdu+3+i*14;
            inquiryInfo=prh_mgr_store_inquiry_entry(inquiryResult);
            if (inquiryInfo && inquiryInfo->entryAge>0) 
		{ 
				/* the record is new - no callback has been made yet */
                /* the prh_mgr_get_callback function is not used as this inquiry callback should not be removed */
#ifdef	SLICE_INQUIRY_SUPPORT
				 if((prh_mgr_IsNameNeedRetrieve == NEED_TO_RETRIEVE_NAME 
				 	&& InquiryCanceled == 0 
				 	&&( prh_mgr_ClassofDeviceToRetrieve & inquiryInfo->classOfDevice|| prh_mgr_ClassofDeviceToRetrieve == 0)) 	
				 	|| prh_mgr_DiscoveryTimeout == 1)
				 {
					 pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"a new device has been found, so call InquiryCancel-------------\n"));
					 InquiryCanceled = 1;
					  HCI_InquiryCancel();
				 }
#endif

                for (j=0; j<MAX_OUTSTANDING_CALLBACKS; j++) 
                {
                    if (inquiryCallbackFunc[j] && inquiryInfo) 
			{
				if(prh_mgr_ClassofDeviceToRetrieve & inquiryInfo->classOfDevice
					|| prh_mgr_ClassofDeviceToRetrieve == 0)	/*modified by Tan, filter by class of device*/
				{
					prh_mgr_deviceNumFound ++;
					pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"prh_mgr_ClassofDeviceToRetrieve=0x%x, inquiryInfo->classOfDevice=0x%x\n", prh_mgr_ClassofDeviceToRetrieve, inquiryInfo->classOfDevice));
					inquiryCallbackFunc[j]((struct st_t_BT_DeviceEntry *)inquiryInfo, BT_NOERROR); /* no status code in inquiryResult events */

					prh_mgr_MaxResult--;
				}
			}
                }
                inquiryInfo->entryAge=0; /* age==0 means entry was reported already in this inquiry period */
            }

			pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"prh_mgr_release_inquiry_entry will be called\n"));
            prh_mgr_release_inquiry_entry(inquiryInfo);

        }
        break;
		
    case HCI_evINQUIRYCOMPLETE:

hci_handle_inquiry_complete: /* A commandComplete for an inquiryCancel jumps to here */

		pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"HCI_evINQUIRYCOMPLETE............................\n"));

		prh_mgr_disoverFlag = 0;
		
		InquiryCanceled = 0;	
		if(prh_mgr_InquiryCanceled == 1)
		{
			pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Inquiry process has been canceled, so no need to make final callback as well as retrieve name...., device Num:%d\n", prh_mgr_deviceNumFound));
			if( prh_mgr_InquiryCancelCallBack)
			{
				if(prh_mgr_IsNameNeedRetrieve == NEED_TO_RETRIEVE_NAME)
				{
					prh_mgr_InquiryCancelCallBack(prh_mgr_deviceNameRetrieved, 0);
					HCI_WritePageTimeout(0x2000);
				}
				else
					prh_mgr_InquiryCancelCallBack(prh_mgr_deviceNumFound, 0);				
			}
			prh_mgr_InquiryCancelCallBack = NULL;
			prh_mgr_InquiryCanceled = 0;
			
			prh_mgr_DiscoveryCanceledComplete();
			prh_mgr_inqdb_inquiry_complete();
			break;
		}

		prh_mgr_inquiryInProgress=FALSE;		
				
        {
            void (*tempFuncPtr)(struct st_t_BT_InquiryInfo *newDevice_t, t_api status_t);

			if(prh_mgr_IsNameNeedRetrieve != NEED_TO_RETRIEVE_NAME)
			{
				prh_mgr_get_callback(&i,(void **)inquiryCallbackFunc,(void **)&tempFuncPtr,NULL);
				if (tempFuncPtr) 
				{
					tempFuncPtr(NULL, prh_mgr_hci2bt_error(eventStatus));
				}
				
				prh_mgr_discoveryInProgress = FALSE;
				for( i=0; i<MAX_OUTSTANDING_CALLBACKS; i++)
				{
					inquiryCallbackFunc[i] = NULL;
					discoveryCallbackFunc[i] = NULL;
				}
			}
			else
			{
				prh_mgr_callback_DeviceDiscovery(NULL, prh_mgr_hci2bt_error(eventStatus));
			}
			
			if(prh_mgr_MaxResult == 0 || prh_mgr_DiscoveryTimeout == 1)
				prh_mgr_inqdb_inquiry_complete();

		}
		
        break;
		
    case HCI_evCONNECTIONCOMPLETE:  
        pMemcpy(&BDAddress.bytes,pdu+5,6);
        dbEntry=prh_mgr_return_deviceEntryByAddress(BDAddress);
        if (!dbEntry) 
            return BT_NORESOURCES;
		
        eventStatus=pdu[2];
		
        if (eventStatus!=HCI_errNOERROR)  {
            pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"Manager:  Failure on connection complete!! eventStatus=%d",eventStatus));

            if (prh_mgr_auth_supplied_link_key && (eventStatus==HCI_errAUTHFAILURE || eventStatus==HCI_errKEYMISSING || eventStatus==HCI_errNOPARING)) {
              /* 
                if authentication failed and we had supplied the link key, the
                supplied (stored) link key should be deleted. This is only
                best effort - no error checking is required here. We cannot restart
                the authentication mechanism due to the connection setup.  -MM
              */            
              prh_mgr_auth_supplied_link_key=0;
              MGR_DeleteLinkKey(prh_mgr_auth_supplied_link_key_addr);
              HCI_DeleteStoredLinkKey(prh_mgr_auth_supplied_link_key_addr,0);
            }
        }
		prh_mgr_auth_supplied_link_key=0;
		
        if (pdu[11]==HCI_linkACL)
        { 
            if (dbEntry->aclState!=CONN_PENDING) 
            {
                if (eventStatus==0) {
                    pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"Manager:  Got incoming ACL connection...dbEntry = %x",dbEntry));
					
                    dbEntry->aclState=CONN_ACTIVE;
                    dbEntry->aclHandle=BT_READ_LE_UINT16(pdu+3);
					
					
                }
            } 
            else 
            {
	                prh_mgr_handle_security_hciEvent(pdu);
	                if (!eventStatus)  
			  {
                    pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"CONN_ACTIVE...dbEntry = %x",dbEntry));
	                    dbEntry->aclState=CONN_ACTIVE;
	                    dbEntry->aclHandle=BT_READ_LE_UINT16(pdu+3);
	                } 
			  else  
			  {
	                    dbEntry->recordState&=~MGR_DEVDB_CONNECTION_ACTIVE;
	                    pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"CONN_CLOSED...dbEntry = %x",dbEntry));
	                    dbEntry->aclState=CONN_CLOSED;
	                }
					
					
	                if (holdAllACLsPendingACLDbEntry) 
			  {
	                    MGR_holdAllOpenACLsEndCallbackStatus=eventStatus;
	                }
			  else 
			  {
				void (*tempFunc)(tid_t transactionId, t_api status_t);	
	                     do 
				{
		                        prh_mgr_get_callback(&i,(void **)dbEntry->aclCallbackFunc,(void **)&tempFunc,dbEntry);
		                        if (tempFunc) 
					   {
		                            if (dbEntry->aclState==CONN_ACTIVE) 
		                            	{
			                            	if(dbEntry->aclCallbackReturn[i])
				                                *dbEntry->aclCallbackReturn[i]=dbEntry->aclHandle;
											else
				                                dbEntry->aclCallbackReturn[i]=&dbEntry->aclHandle;											
		                            	}
		                            tempFunc(dbEntry->aclCallbackTid[i], prh_mgr_hci2bt_error(eventStatus)); 
		                        }
	                    } while (tempFunc);

	                }
            }
            if (eventStatus==BT_NOERROR) 
            {
                /* Enable hold mode for this link (will fail if unsuported by the HC)*/
				HCI_WriteLinkPolicySettings(dbEntry->aclHandle,prh_mgr_linkpolicyflags);

                /* 
                    Read the clock offset to speed up future connections to this device 
                    This is best effort only - no error checking required.
                */
                HCI_ReadClockOffset(dbEntry->aclHandle);
				
            }
			/* Else if ACL connection setup failed */
			else
			{
                /* If the ACL was required for a bonding procedure, we must make the bonding callback */
                void (*tempFuncPtr)(t_bdaddr address, t_api status_t);

                mgrStatus=prh_mgr_hci2bt_error(eventStatus);

                do {
                    prh_mgr_get_callback(&i,(void **)dbEntry->bondingCallbackFunc,(void **)&tempFuncPtr,dbEntry);
                    if (tempFuncPtr) {
                        tempFuncPtr(dbEntry->bdAddress, mgrStatus); 
					}
				} while (tempFuncPtr);

                /* Bonding procedure is finisged due to this failure */
				dbEntry->bondingState = BOND_INACTIVE;
			}
        }
        if (pdu[11]==HCI_linkSCO) 
        { 
            for (i=0; i<3; i++) 
            {
                if (dbEntry->scoState[i]!=CONN_ACTIVE) 
                {
                    break;
                }
            }
            if (i==3) 
            {
                /* this shouldn't happen - As Max number SCOs in Bluetooth is 3 */
                pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Manager:  SCO connection setup but three SCOs already active!!"));
                break;
            }
            
            for (i=0; i<3; i++) 
            {
                if (dbEntry->scoState[i]==CONN_PENDING) 
                {
                    break;
                }
            }
            if (i==3) 
            {
                pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Manager:  SCO connection auto-accepted or setup via HCI..."));
                for (i=0; i<3; i++) 
                {
                    if (dbEntry->scoState[i]==CONN_CLOSED) 
                    {
                        break;
                    }
                }
				
            }
			
            if (eventStatus==HCI_errNOERROR) {
                dbEntry->scoState[i]=CONN_ACTIVE;
                dbEntry->scoHandle[i]=BT_READ_LE_UINT16(pdu+3);
            }
            else {
                dbEntry->scoState[i]=CONN_CLOSED;
            }
			
            {
				void (*tempFunc)(u_int16 *scoHandle, t_api status_t);
                do {
                    prh_mgr_get_callback(&j,(void **)dbEntry->scoCallbackFunc,(void **)&tempFunc,dbEntry);
                    if (tempFunc) {
                        if (dbEntry->scoState[i]==CONN_ACTIVE) 
                            *dbEntry->scoCallbackReturn[j]=dbEntry->scoHandle[i];
                        tempFunc(dbEntry->scoCallbackReturn[j], prh_mgr_hci2bt_error(eventStatus));
                    }
                } while (tempFunc);
            }
			
        }
        break;
		
    case HCI_evCONNECTIONREQUEST:
        if (pdu[11]==HCI_linkACL) {
#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.3.11
//JianCui add 2009 08 22
#ifndef  MGR_MULTI_CONNECTION
//#if PRH_L2CAP_MultiConnect_Disnable 
		struct st_t_BT_ConnectionInfo *currentDevice=prh_device_list_head;	
	    /* check that no ACLS are up */
		while(currentDevice) {
			if(currentDevice->aclState!=CONN_CLOSED) 
				break;
			currentDevice = currentDevice->nextDevice;
		}
            if (prh_mgr_linkFlags&MGR_CONNREQ_ACL_ACCEPT && !currentDevice) {
                pMemcpy(BDAddress.bytes,pdu+2,6);
                pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Manager: accepting ACL connection request from %s due to link flag MGR_CONNREQ_ACL_ACCEPT set\n",prh_mgr_printBDAddress(BDAddress)));
                HCI_AcceptConnectionRequest(BDAddress,(u_int8)(((prh_mgr_linkFlags&MGR_CONNREQ_ACL_SWITCH)==MGR_CONNREQ_ACL_SWITCH)?0x00:0x01)); /* 0x01 == do not request role switch */
            } else if (prh_mgr_linkFlags&MGR_CONNREQ_ACL_REJECT || currentDevice) {
#else
            if (prh_mgr_linkFlags&MGR_CONNREQ_ACL_ACCEPT ) {
                pMemcpy(BDAddress.bytes,pdu+2,6);
                pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Manager: accepting ACL connection request from %s due to link flag MGR_CONNREQ_ACL_ACCEPT set\n",prh_mgr_printBDAddress(BDAddress)));
                HCI_AcceptConnectionRequest(BDAddress,(u_int8)(((prh_mgr_linkFlags&MGR_CONNREQ_ACL_SWITCH)==MGR_CONNREQ_ACL_SWITCH)?0x00:0x01)); /* 0x01 == do not request role switch */
            } else if (prh_mgr_linkFlags&MGR_CONNREQ_ACL_REJECT) {
#endif
                pMemcpy(BDAddress.bytes,pdu+2,6);
                pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Manager: rejecting ACL connection from %s due to link flag MGR_CONNREQ_ACL_REJECT set\n",prh_mgr_printBDAddress(BDAddress)));
                HCI_RejectConnectionRequest(BDAddress,MGR_HCI_CONNREJECT_REASON); 
            } 
#if pDEBUG 
            else {
                /* MGR_CONNREQ_ACL_IGNORE */
                pMemcpy(BDAddress.bytes,pdu+2,6);
                pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Manager: ignoring ACL connection request from %s due to link flag MGR_CONNREQ_ACL_IGNORE set\n",prh_mgr_printBDAddress(BDAddress)));
			}
#endif

#else
        /* accept the acl link connect req ?*/
{
//JianCui add 2009 08 22
#ifndef MGR_MULTI_CONNECTION 

//#if PRH_L2CAP_MultiConnect_Disnable 
		struct st_t_BT_ConnectionInfo *currentDevice=prh_device_list_head;	
	    /* check that no ACLS are up */
		while(currentDevice) {
			if(currentDevice->aclState!=CONN_CLOSED) 
				break;
			currentDevice = currentDevice->nextDevice;
		}
        if (prh_mgr_linkFlags&MGR_CONNREQ_ACL_ACCEPT&& prh_callback_AcceptConnectionRequest!=NULL && !currentDevice) 
        {
            pMemcpy(BDAddress.bytes,pdu+2,6);
//#if COOLSAND
		{
			struct st_t_BT_ConnectionInfo * entry;
			entry = prh_mgr_return_deviceEntryByAddress(BDAddress);
			if(entry)
			{
				entry->cod = BT_READ_LE_UINT24(pdu+8);
				pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"HCI_evCONNECTIONREQUEST cod : %d\n",entry->cod));
			}
		}              
 	     prh_callback_AcceptConnectionRequest(BDAddress);
        }
        else if (prh_mgr_linkFlags&MGR_CONNREQ_ACL_REJECT || currentDevice) 
#else
        if (prh_mgr_linkFlags&MGR_CONNREQ_ACL_ACCEPT&& prh_callback_AcceptConnectionRequest!=NULL) 
        {
            pMemcpy(BDAddress.bytes,pdu+2,6);
		{
			struct st_t_BT_ConnectionInfo * entry;
			entry = prh_mgr_return_deviceEntryByAddress(BDAddress);
			if(entry)
			{
				entry->cod = BT_READ_LE_UINT24(pdu+8);
				pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"HCI_evCONNECTIONREQUEST cod : %d\n",entry->cod));
			}
		}              
 	     prh_callback_AcceptConnectionRequest(BDAddress);

        }
        else if (prh_mgr_linkFlags&MGR_CONNREQ_ACL_REJECT) 
#endif
        {
            pMemcpy(BDAddress.bytes,pdu+2,6);
            pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Manager: rejecting ACL connection from %s due to link flag MGR_CONNREQ_ACL_REJECT set\n",prh_mgr_printBDAddress(BDAddress)));
            HCI_RejectConnectionRequest(BDAddress,MGR_HCI_CONNREJECT_REASON); 
        } 
#if pDEBUG 
        else 
        {
            /* MGR_CONNREQ_ACL_IGNORE */
            pMemcpy(BDAddress.bytes,pdu+2,6);
            pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Manager: ignoring ACL connection request from %s due to link flag MGR_CONNREQ_ACL_IGNORE set\n",prh_mgr_printBDAddress(BDAddress)));
        }
#endif
    
}

#endif
        } else if (pdu[11]==HCI_linkSCO) {
            if (prh_mgr_linkFlags&MGR_CONNREQ_SCO_ACCEPT) {
                pMemcpy(BDAddress.bytes,pdu+2,6);
                pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Manager: accepting SCO connection request from %s due to link flag MGR_CONNREQ_SCO_ACCEPT set\n",prh_mgr_printBDAddress(BDAddress)));
                HCI_AcceptConnectionRequest(BDAddress,0x00); /* role switch argument ignored in SCO case */
            } else if (prh_mgr_linkFlags&MGR_CONNREQ_SCO_REJECT) {
                pMemcpy(BDAddress.bytes,pdu+2,6);
                pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Manager: rejecting SCO connection from %s due to link flag MGR_CONNREQ_SCO_REJECT set\n",prh_mgr_printBDAddress(BDAddress)));
                HCI_RejectConnectionRequest(BDAddress,MGR_HCI_CONNREJECT_REASON); 
            } 
#if pDEBUG 
            else {
                /* MGR_CONNREQ_SCO_IGNORE */
                pMemcpy(BDAddress.bytes,pdu+2,6);
                pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Manager: ignoring SCO connection request from %s due to link flag MGR_CONNREQ_SCO_IGNORE set\n",prh_mgr_printBDAddress(BDAddress)));
			}
#endif
        }
        break;
		
    case HCI_evDISCONNECTIONCOMPLETE:
	pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"HCI_evDISCONNECTIONCOMPLETE event has been received......\n"));

	handle = BT_READ_LE_UINT16(pdu+3);
        dbEntry=prh_mgr_get_deviceEntryByHandle(handle, &linkType);
        if (!dbEntry) 
        {
            pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"Manager: got disconnection event for handle not in database!"));
            break;
        }
		if(HCI_linkACL == linkType && (CONN_ACTIVE == dbEntry->aclState || CONN_RELEASEPENDING == dbEntry->aclState) )
		{
		  void (*tempFunc)(t_bdaddr address, t_api status_t);
		  
		  u_int8 reason = *(pdu + 5); /* (pdu+5) contains the reason for disconnection */
            pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"HCI_linkACL"));

            /* 
                The host has no real recourse if the host controller refuses to disconnect
                an ACL. If we retry the disconnect here, we may enter a loop where the HC
                continues to refuse the disconnect. If we perform a full reset here, we 
                will cause other apps to fail. The best course of action is to pass the
                error to the application and leave the ACL up. This may affect future ACL
                establishment (if we are a slave, etc.) but there is no other solution.
                The Parthus Host Controller will never fail a HCI_Disconnect() command.
                -MM
            */
			if (eventStatus==HCI_errNOERROR)  
			{
            pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"dbEntry->aclState = CONN_CLOSED!dbEntry=%x",dbEntry));
				dbEntry->aclState = CONN_CLOSED;

                /* Only change the record state if all SCOs are already closed */
			    for (i=0; i<3; i++) 			    
                    if (dbEntry->scoState[i]!=CONN_CLOSED) 
                        break;			    
                if (i==3)  
                    dbEntry->recordState&=~MGR_DEVDB_CONNECTION_ACTIVE;                
            } 
			else
            pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"eventStatus=%x!",eventStatus));				

			{
				void (*tempFunc_t)(struct st_t_BT_ConnectionInfo *dbEntry_t, t_api status_t);
				pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"COMPLETE aclReleaseCallbackFunc will be called\n"));

				do 
				{
					prh_mgr_get_callback(&i,(void **)dbEntry->aclReleaseCallbackFunc,(void **)&tempFunc_t,dbEntry);
					pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"aclReleaseCallbackFunc tempFunc_t = 0x%x\n",tempFunc_t));
					if (tempFunc_t)
						tempFunc_t(dbEntry, prh_mgr_hci2bt_error(eventStatus)); 
				} while (tempFunc_t);				
			}

		    i = 0;

			while(dbEntry->accessReqCallbackFunc[i] && i<MAX_OUTSTANDING_CALLBACKS)
			{            
                /* 
                    In this case, an access request was in progress (possibly pairing/authorisation/etc.) and
                    the ACL has been disconnected before the procedure has completed. In this case, we must
                    fail the access request, so we call the callback passing the disconnect reason code as the error.
                    -MM
                 */				 
				dbEntry->accessReqCallbackFunc[i](dbEntry->accessReqCallbackHandle[i], prh_mgr_hci2bt_error(reason)); 
				dbEntry->accessReqCallbackFunc[i] = NULL;
				dbEntry->accessReqCallbackEncryptOn[i] = 0;
				i++;  
			}
			dbEntry->accessReqEncryptState = SET_ENCRYPT_IDLE;
			dbEntry->accessReqAuthState = AUTH_IDLE;
		    dbEntry->trustLevel=MGR_UNKNOWN;

            /* 
                If bonding was in progress, we must fail this procedure, so we make the callback
                with the disconnection reason code as the error value.

			    To delete the stored callback we call prh_mgr_get_callback(),
			    which retrieves and deletes the callback. We only actually
			    use the callback if the bonding state is other than BOND_INACTIVE
                -MM
			 */        
            
			prh_mgr_get_callback(&i,(void **)dbEntry->bondingCallbackFunc,(void **)&tempFunc,dbEntry);

			if(dbEntry->bondingState != BOND_INACTIVE)
			{
			    if(tempFunc)
				   tempFunc(dbEntry->bdAddress, prh_mgr_hci2bt_error(reason)); 

			    dbEntry->bondingState = BOND_INACTIVE;
			}
		} /* End ACL */
		else if(HCI_linkSCO == linkType)
		{
            pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"HCI_linkSCO"));
			for (i=0; i<3; i++) 
			{
                if (handle==dbEntry->scoHandle[i]) {
				    if (eventStatus==HCI_errNOERROR) 
				    {
					    dbEntry->scoState[i]=CONN_CLOSED;
				    }
					else
					{
                        dbEntry->scoState[i]=CONN_ACTIVE;
					}
				    if(dbEntry->scoReleaseCallbackFunc[i])
				    {
					    dbEntry->scoReleaseCallbackFunc[i](dbEntry->scoHandle[i], prh_mgr_hci2bt_error(eventStatus));
					    dbEntry->scoReleaseCallbackFunc[i] = NULL;
				    }
				    break;
                }
			}  
            /* Change the record state if the ACL is already closed and this is the last SCO*/
            if (dbEntry->aclState==CONN_CLOSED) {
			    for (i=0; i<3; i++) 			    
                    if (dbEntry->scoState[i]!=CONN_CLOSED) 
                        break;			    
                if (i==3)  
                    dbEntry->recordState&=~MGR_DEVDB_CONNECTION_ACTIVE;                
            }
        }
		
        break;
		
    case HCI_evQOSSETUPCOMPLETE:
        aclHandle = BT_READ_LE_UINT16(pdu+3);
        dbEntry=prh_mgr_get_deviceEntryByHandle(aclHandle, &linkType);
        if (!dbEntry)
        {   
            pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Manager: got QoS Setup Complete event for handle not in database!"));
            break;
        }
		
        if(HCI_errNOERROR == eventStatus)
        {					
			dbEntry->qosParams = (t_L2_ChannelFlowSpec*)pMalloc(sizeof(t_L2_ChannelFlowSpec));
            if(dbEntry->qosParams)
            {
                dbEntry->qosParams->flags = pdu[5];
                dbEntry->qosParams->serviceType = pdu[6];
                dbEntry->qosParams->tokenRate = BT_READ_LE_UINT32(pdu+7);
                dbEntry->qosParams->bucketSize= BT_READ_LE_UINT32(pdu+11);
                dbEntry->qosParams->peakBandwidth= BT_READ_LE_UINT32(pdu+15);
                dbEntry->qosParams->latency = BT_READ_LE_UINT32(pdu+19);
				dbEntry->qosParams->delayVariation= BT_READ_LE_UINT32(pdu+23);
				
            }
        }
        if(dbEntry->qosSetupCallbackFunc)
		{
			dbEntry->qosSetupCallbackFunc(dbEntry->qosSetupCallbackTid, dbEntry->qosParams, prh_mgr_hci2bt_error(eventStatus));
			dbEntry->qosSetupCallbackFunc = NULL;
		}
		
        break;
		
    case HCI_evCHANGECONNECTIONLINKKEYCOMPLETE:     
    case HCI_evREADREMOTESUPPORTEDFEATURESCOMPLETE:
    case HCI_evREADREMOTEVERSIONINFORMATIONCOMPLETE:
    case HCI_evROLECHANGE:
    case HCI_evMAXSLOTSCHANGE:
    case HCI_evQOSVIOLATION:
    case HCI_evFLUSHOCCURRED:
    case HCI_evDATABUFFEROVERFLOW:
        break;
		
    case HCI_evLINKKEYREQUEST:
        if (prh_mgr_linkFlags&MGR_KEYREQ_HANDLE) {
#if pDEBUG
            pMemcpy(&BDAddress.bytes,pdu+2,6);
            pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Manager: handling link key request for %s due to link flag MGR_KEYREQ_HANDLE set\n",prh_mgr_printBDAddress(BDAddress)));
#endif
            prh_mgr_handle_security_hciEvent(pdu);
        } else if (prh_mgr_linkFlags&MGR_KEYREQ_REJECT) {
            pMemcpy(&BDAddress.bytes,pdu+2,6);
            pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Manager: rejecting link key request for %s due to link flag MGR_KEYREQ_REJECT set\n",prh_mgr_printBDAddress(BDAddress)));
            HCI_LinkKeyRequestNegativeReply(BDAddress);
        } 
#if pDEBUG
        else { /* MGR_KEYREQ_IGNORE */
            pMemcpy(&BDAddress.bytes,pdu+2,6);
            pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Manager: ignoring link key request for %s due to link flag MGR_KEYREQ_IGNORE set\n",prh_mgr_printBDAddress(BDAddress)));
        }
#endif
        break;
        
    case HCI_evPINCODEREQUEST:
        if (prh_mgr_linkFlags&MGR_PINREQ_HANDLE) {
#if pDEBUG
            pMemcpy(&BDAddress.bytes,pdu+2,6);
            pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Manager: handling pin code request for %s due to link flag MGR_PINREQ_HANDLE set\n",prh_mgr_printBDAddress(BDAddress)));
#endif
            prh_mgr_handle_security_hciEvent(pdu);
        } else if (prh_mgr_linkFlags&MGR_PINREQ_REJECT) {
            pMemcpy(&BDAddress.bytes,pdu+2,6);
            pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Manager: rejecting pin code request for %s due to link flag MGR_PINREQ_REJECT set\n",prh_mgr_printBDAddress(BDAddress)));
            HCI_PINCodeRequestNegativeReply(BDAddress); 
        } 
#if pDEBUG
        else { /* MGR_PINREQ_IGNORE */
            pMemcpy(&BDAddress.bytes,pdu+2,6);
            pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Manager: ignoring pin code request for %s due to link flag MGR_PINREQ_IGNORE set\n",prh_mgr_printBDAddress(BDAddress)));
        }
#endif
        break;
		
    case HCI_evLINKKEYNOTIFICATION:    
    case HCI_evENCRYPTIONCHANGE:
    case HCI_evAUTHENTICATIONCOMPLETE:
        prh_mgr_handle_security_hciEvent(pdu);
        break;
		
    case HCI_evREMOTENAMEREQUESTCOMPLETE:
	 pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"HCI_evREMOTENAMEREQUESTCOMPLETE received..........\n"));

        mgrStatus=prh_mgr_hci2bt_error(eventStatus);
        pMemcpy(&BDAddress.bytes,pdu+3,6);
		
		nameLen=0;
		if (eventStatus==HCI_errNOERROR)
			for (; pdu[9+nameLen] && nameLen<MGR_MAX_REMOTE_NAME_LEN; nameLen++) ; 
			
		inquiryInfo=prh_mgr_retrieve_inquiry_entry(BDAddress);
#if MGR_DEVDB_CACHE_REMOTE_NAME 
		if (inquiryInfo && eventStatus==HCI_errNOERROR) {
			if (inquiryInfo->deviceName) {
				pFree(inquiryInfo->deviceName);
			}
			inquiryInfo->deviceName=pMalloc(nameLen+1); 
			if (inquiryInfo->deviceName) {
				pMemcpy(inquiryInfo->deviceName,pdu+9,nameLen);
				inquiryInfo->deviceName[nameLen]='\0';
			 pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"cache the device name ...........................................\n"));
			}

		}
#endif 
			
		dbEntry=prh_mgr_find_deviceEntryByAddress(BDAddress);
		if (dbEntry) {		
			/* Invoke callbacks for the MGR_GetRemoteName primitive */
			if(prh_mgr_pendingNameEntry)
			{
				for (i=0; i<MAX_OUTSTANDING_CALLBACKS; i++) {
					if (prh_mgr_pendingNameEntry->nameCallbackFunc[i]!=NULL)
					{
						/* The status needs to be sent back some how */
						if(eventStatus==HCI_errNOERROR) {
							u_int8 nameLenToCopy;
							if(dbEntry->nameCallbackOutBuffer[i]) {
								nameLenToCopy=nameLen<dbEntry->nameCallbackBufLen[i]-1?nameLen:dbEntry->nameCallbackBufLen[i]-1;
								pMemcpy((char *)dbEntry->nameCallbackOutBuffer[i],pdu+9,nameLenToCopy);  
								dbEntry->nameCallbackOutBuffer[i][nameLenToCopy] = '\0';
							}
							if (dbEntry->nameCallbackNameLen[i])
								*(dbEntry->nameCallbackNameLen[i]) = nameLenToCopy;
						}
						prh_mgr_pendingNameStatus = prh_mgr_hci2bt_error(eventStatus);
					}
				}
			} else {
				void (*tempFunc)(t_bdaddr address, t_api status_t);
				do {
					prh_mgr_get_callback(&i,(void **)dbEntry->nameCallbackFunc,(void **)&tempFunc,dbEntry);
					
					if (tempFunc) {
						if(eventStatus==HCI_errNOERROR) {
							u_int8 nameLenToCopy;
							if(dbEntry->nameCallbackOutBuffer[i]) {
								nameLenToCopy=nameLen<dbEntry->nameCallbackBufLen[i]-1?nameLen:dbEntry->nameCallbackBufLen[i]-1;
								pMemcpy((char *)dbEntry->nameCallbackOutBuffer[i],pdu+9,nameLenToCopy);  
								dbEntry->nameCallbackOutBuffer[i][nameLenToCopy] = '\0';
							}
							if (dbEntry->nameCallbackNameLen[i])
								*(dbEntry->nameCallbackNameLen[i]) = nameLenToCopy;
						} 						
						tempFunc(BDAddress, mgrStatus); 
					}
				} while (tempFunc);
			}				
		}

		pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"HCI_evREMOTENAMEREQUESTCOMPLETE: ++++++++++++++"));

		if (prh_mgr_discoveryInProgress || prh_mgr_InquiryCanceled == 1) {
			/* Don't pass the mgrStatus as the result as an erroneous result will
			   cause the manager to stop issuing remote name request commands for
			   the other devices.
			*/
			pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"HCI_evREMOTENAMEREQUESTCOMPLETE: ++++++++++++++"));
			prh_mgr_callback_DeviceDiscovery((struct st_t_BT_DeviceEntry *)inquiryInfo,0x00);			
		} 
		
		prh_mgr_release_inquiry_entry(inquiryInfo);
		break;
			
    case HCI_evREADCLOCKOFFSETCOMPLETE:
        if (!eventStatus) {
            dbEntry=prh_mgr_get_deviceEntryByHandle(BT_READ_LE_UINT16(pdu+3),NULL);
            if (dbEntry) {
                inquiryInfo=prh_mgr_retrieve_inquiry_entry(dbEntry->bdAddress); 

                if (inquiryInfo) {
                    inquiryInfo->clockOffset=BT_READ_LE_UINT16(pdu+5);
                    prh_mgr_release_inquiry_entry(inquiryInfo);
                }
            }
        }
#if TCSBIN_PRESENT
        prh_tcs_wug_retreiveClockOffset(BT_READ_LE_UINT16(pdu+3), BT_READ_LE_UINT16(pdu+5), eventStatus);
#endif
        break;
		
    case HCI_evMODECHANGE:
		aclHandle = BT_READ_LE_UINT16(pdu+3);
		interval = BT_READ_LE_UINT16(pdu+6);
		prh_mgr_handle_HCI_evMODECHANGE(aclHandle, eventStatus, pdu[5] /*mode*/, interval);
		break;
		
    case HCI_evCOMMANDSTATUS:
		cmdOpcode = BT_READ_LE_UINT16(pdu+4);
		
		switch(cmdOpcode)
        {
			
        case HCI_cmdINQUIRY:
			if(HCI_errNOERROR != eventStatus)
            {
                mgrStatus = prh_mgr_hci2bt_error(eventStatus);

				
                if(prh_mgr_IsNameNeedRetrieve == NONEED_TO_RETRIEVE_NAME)
                {
					void (*tempFuncPtr)(struct st_t_BT_InquiryInfo *newDevice, t_api status_t);
					do {
						prh_mgr_get_callback(&i,(void **)inquiryCallbackFunc,(void **)&tempFuncPtr,NULL);
						if (tempFuncPtr) 
							tempFuncPtr(NULL, mgrStatus);      
					} while (tempFuncPtr);
				prh_mgr_inquiryInProgress = FALSE;
                }
		   else
		   {
		   		prh_mgr_callback_DeviceDiscovery(NULL, BT_NOERROR);
		   }
            }
			break;

		case HCI_cmdADDHARDWARESCO:
			if(HCI_errNOERROR != eventStatus)
            {
                dbEntry=prh_device_list_head;
                while (dbEntry) 
				{
				    void (*tempFunc)(u_int16 *scoHandle, t_api status_s);
                    do {
                        prh_mgr_get_callback(&i,(void **)dbEntry->scoCallbackFunc,(void **)&tempFunc,dbEntry);
                        if (tempFunc) 
						{
                            tempFunc(dbEntry->scoCallbackReturn[i], prh_mgr_hci2bt_error(eventStatus)); 
						}
					} while (tempFunc);

					dbEntry=dbEntry->nextDevice;
				}
            }
			break;

		case HCI_cmdADDSCOCONNECTION:
			if(HCI_errNOERROR != eventStatus)
            {
                dbEntry=prh_device_list_head;
                while (dbEntry) 
				{
				    void (*tempFunc)(u_int16 *scoHandle, t_api status_t);
                    do {
                        prh_mgr_get_callback(&i,(void **)dbEntry->scoCallbackFunc,(void **)&tempFunc,dbEntry);
                        if (tempFunc) 
						{
                            tempFunc(dbEntry->scoCallbackReturn[i], prh_mgr_hci2bt_error(eventStatus)); 
						}
					} while (tempFunc);

					dbEntry=dbEntry->nextDevice;
				}
            }
			break;
			
		case HCI_cmdHOLDMODE:
			mgrStatus = prh_mgr_hci2bt_error(eventStatus);

			if(MGR_holdAllOpenACLsStartCallback && !mgrStatus)
			{
				pMutexLock(prh_mgr_hold_acl_lock);				
				
				MGR_holdAllOpenACLsStartCallback(mgrStatus);
				MGR_holdAllOpenACLsStartCallback = NULL;
				numPendingStartHolds--;
				
				pMutexUnlock(prh_mgr_hold_acl_lock);
			}
			else if(pendingCmdStatus[PRH_MGR_INDEXPENDINGCOMMANDSTATUS_HOLDMODE].count)
			{
				dbEntry = pendingCmdStatus[PRH_MGR_INDEXPENDINGCOMMANDSTATUS_HOLDMODE].devDbEntry[pendingCmdStatus[PRH_MGR_INDEXPENDINGCOMMANDSTATUS_HOLDMODE].currentIndex];
				if(!mgrStatus && dbEntry && dbEntry->holdACLCallbackFunc)
				{
					dbEntry->deviceMode = MGR_DEVACTIVE;
					dbEntry->holdACLCallbackFunc(mgrStatus, dbEntry->aclHandle, 0);
					dbEntry->holdACLCallbackFunc = NULL;
					dbEntry->exitHoldACLCallbackFunc = NULL;
				}
			}
			if(++(pendingCmdStatus[PRH_MGR_INDEXPENDINGCOMMANDSTATUS_HOLDMODE].currentIndex) == MGR_MAXACTIVEACLS)
				pendingCmdStatus[PRH_MGR_INDEXPENDINGCOMMANDSTATUS_HOLDMODE].currentIndex = 0;
			pendingCmdStatus[PRH_MGR_INDEXPENDINGCOMMANDSTATUS_HOLDMODE].count--;
			break;
			
				
		case HCI_cmdREMOTENAMEREQUEST :
			if (eventStatus!=HCI_errNOERROR)
			{
				pMemcpy(&BDAddress.bytes, &prh_mgr_pendingBDAddress.bytes,6);
				
				nameLen=0;
				inquiryInfo=prh_mgr_retrieve_inquiry_entry(BDAddress);
				dbEntry=prh_mgr_find_deviceEntryByAddress(BDAddress);

				if(prh_mgr_pendingNameEntry)
				{ /* acls held */
					/* The status needs to be sent back some how as I cant call the callbacks yet...*/
					prh_mgr_pendingNameStatus = prh_mgr_hci2bt_error(eventStatus);
				}

				if (dbEntry)
				{
					/* Invoke callbacks for the MGR_GetRemoteName primitive */
					prh_mgr_name_normal_callback(prh_mgr_hci2bt_error(eventStatus),dbEntry);
				}
				if (prh_mgr_discoveryInProgress) {
					prh_mgr_callback_DeviceDiscovery((struct st_t_BT_DeviceEntry *)inquiryInfo,BT_NOERROR);			
				} 
				prh_mgr_release_inquiry_entry(inquiryInfo);
			}
			
			break;
		
			
		case HCI_cmdDISCONNECT :
			aclHandle = prh_mgr_pendingDisconnectHandle;
			if (eventStatus) 
			{
				dbEntry=prh_mgr_get_deviceEntryByHandle(aclHandle, &linkType);
				if (!dbEntry) 
				{
					pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"Manager: got disconnection event for handle not in database!"));
					break;
				}
				if(HCI_linkACL == linkType && (CONN_ACTIVE == dbEntry->aclState || CONN_RELEASEPENDING == dbEntry->aclState) )
				{
					{
						void (*tempFunc)(struct st_t_BT_ConnectionInfo *dbEntry_t, t_api status_t);
						do 
						{
							prh_mgr_get_callback(&i,(void **)dbEntry->aclReleaseCallbackFunc,(void **)&tempFunc,dbEntry);
							if (tempFunc)
								tempFunc(dbEntry, prh_mgr_hci2bt_error(eventStatus)); 
						} while (tempFunc);
					}	
				}
				else if(HCI_linkSCO == linkType)
				{
					for (i=0; i<3; i++) 
					{
						if (dbEntry->scoState[i]==CONN_ACTIVE && dbEntry->scoHandle[i]==aclHandle) 
						{
							if(dbEntry->scoReleaseCallbackFunc[i])
							{
								dbEntry->scoReleaseCallbackFunc[i](dbEntry->scoHandle[i], prh_mgr_hci2bt_error(eventStatus));
								dbEntry->scoReleaseCallbackFunc[i] = NULL;
							}
							break;
						}
					}
				}  
			}
			break;

		case HCI_cmdCREATECONNECTION :
			dbEntry=prh_mgr_return_deviceEntryByAddress(prh_mgr_pendingBDAddress);
			if (!dbEntry) 
				return BT_NORESOURCES;
			
			if (eventStatus!=0)  
			{ 
				dbEntry->recordState&=~MGR_DEVDB_CONNECTION_ACTIVE;
				dbEntry->aclState=CONN_CLOSED;
				
				{
					void (*tempFunc)(tid_t transactionId, t_api status_t);
					do {
						prh_mgr_get_callback(&i,(void **)dbEntry->aclCallbackFunc,(void **)&tempFunc,dbEntry);
						if (tempFunc) {
							if (dbEntry->aclState==CONN_ACTIVE) 
								*dbEntry->aclCallbackReturn[i]=dbEntry->aclHandle;
							tempFunc(dbEntry->aclCallbackTid[i], prh_mgr_hci2bt_error(eventStatus));
						}
					} while (tempFunc);
				}
			}
			break;

		case HCI_cmdAUTHENTICATIONREQUESTED :
		case HCI_cmdLINKKEYREQUESTNEGATIVEREPLY :
		case HCI_cmdLINKKEYREQUESTREPLY :
		case HCI_cmdPINCODEREQUESTREPLY :
		case HCI_cmdPINCODEREQUESTNEGATIVEREPLY :
		case HCI_cmdSETCONNECTIONENCRYPTION :
		case HCI_cmdCHANGECONNECTIONLINKKEY :
		case HCI_cmdMASTERLINKKEY :
			prh_mgr_handle_security_hciEvent(pdu);
			break;
		}
		break;
		
		
		case HCI_evCOMMANDCOMPLETE:
			cmdOpcode = BT_READ_LE_UINT16(pdu+3);
			eventStatus = pdu[5];
pDebugPrintfEX((pLOGNOTICE,pLOGHCI," zhou siyou HCI_evCOMMANDCOMPLETE type=%d!! ",cmdOpcode ));

			switch (cmdOpcode) 
			{
				
			case HCI_cmdINQUIRYCANCEL:     
                /* treat this as an inquiry complete */
                if (eventStatus==0 || prh_mgr_IsNameNeedRetrieve == NEED_TO_RETRIEVE_NAME)					
                    goto hci_handle_inquiry_complete;                
                break;
			case HCI_cmdREMOTENAMEREQUESTCANCEL:
				//no need to do anything since host will generate a  HCI_cmdREMOTENAMECOMPLETE  event...
				break;
			case HCI_cmdCHANGELOCALNAME:                 
				if(!eventStatus)
				{
					if (prh_mgr_LocalName) 
					{
						pFree(prh_mgr_LocalName);
						prh_mgr_LocalName=NULL;
					}
					HCI_ReadLocalName();
				}
				//Bt_init_Finished=1;
				//pDebugPrintfEX((pLOGNOTICE,pLOGHCI," zhou siyou Bt_init_Finished has be set" ));
				break;
             case HCI_cmdWRITECLASSOFDEVICE: //zhou siyou
             	{
             		//Bt_init_Finished=1;
				pDebugPrintfEX((pLOGNOTICE,pLOGHCI," zhou siyou HCI_cmdWIRTECLASSOFDEVICE!! " ));
				rdabt_send_ready_ind();
					
             	}
			 break;
			case HCI_cmdWRITEAUTHENTICATIONENABLE:                 
				if(HCI_errNOERROR == eventStatus)
				{
				/* 
				if the manager has affected this call
				set the new security mode and reset flag
					*/
					if(prh_security_mode_pending != 0)
					{
						prh_security_mode = prh_security_mode_pending;
						prh_security_mode_pending = 0;
					}
					else /* otherwise use the flag that was set in the HCI */
					{
						prh_security_mode = prh_hci_security_mode_pending;
					}
					mgrStatus = BT_NOERROR;
				}
				else /* there's an error in the Command Complete event*/
				{
					/* clear the manager flag if set */
					if(prh_security_mode_pending != 0)
						prh_security_mode_pending = 0;
					
					mgrStatus = prh_mgr_hci2bt_error(eventStatus); 
				}			  
				
				/* if the manager was used, call the app callback */
				
				if(setSecurityModeCallback)
				{       
					setSecurityModeCallback(mgrStatus);
					setSecurityModeCallback = NULL;
				}
				
				break;
				
			case HCI_cmdWRITESCANENABLE:
			case HCI_cmdWRITECURRENTIACLAP:				
				/* if this is from a HCI_cmdWRITESCANENABLE*/
				if(HCI_cmdWRITESCANENABLE == cmdOpcode)
				{
					/* remove the SCAN Enable Pending from the mode flag */
					prh_mgr_mode_flag ^= PRH_MGR_SCANENABLEPENDING_BIT;
					/* assign the actual scan enable currently in place
					to be the one that was called - only if there was NOERROR*/
					if(HCI_errNOERROR == eventStatus)
						prh_mgr_scan_enable = prh_mgr_scan_enable_pending;
				}
				else /*this is from a HCI_cmdWRITECURRENTIACLAP*/
				{
					/* remove the IACLAP Pending from the mode flag */
					prh_mgr_mode_flag ^= PRH_MGR_IACLAPPENDING_BIT;
					/* assign the actual IAACLAP currently stored
					in the manager to be the one that was called - only if there was NOERROR*/
					if(HCI_errNOERROR == eventStatus)
						prh_mgr_liac_present = prh_mgr_liac_present_pending;
				}
				
				/* 
				if (there is a pending in the mode flag)  && (there isn't the IACLAP pending or the SCAN Enable pending)
				
				  this condition is true if the MGR_SetMode() fn was used to modify the WRITESCANENABLE
				  or the WRITECURRENTIACLAP - if they were modified using the HCI commands directly,
				  then this condition is false.
				*/
				if(PRH_MGR_SETMODEPENDING(prh_mgr_mode_flag) && !PRH_MGR_MODEPENDING(prh_mgr_mode_flag))
				{
					/*clear the pending bit*/
					prh_mgr_mode_flag ^= PRH_MGR_SETMODEPENDING_BIT;
					/* if callback was set, call it passing the current mode....Note: WriteIACLAP cmd does not return an error */
					if(prh_mgr_mode_callbackFunc)
					{
						MGR_GetMode(&currentMode);
						temp_mode_callbackFunc=prh_mgr_mode_callbackFunc;
						prh_mgr_mode_callbackFunc=NULL;
						temp_mode_callbackFunc(prh_mgr_hci2bt_error(eventStatus));
					}
				}
				break;
				
			case HCI_cmdREADLOCALVERSIONINFORMATION:
				prh_mgr_HCIversion=pdu[6];
				prh_mgr_HCIrevision=BT_READ_LE_UINT16(pdu+7);
				prh_mgr_LMPversion=pdu[9];
				prh_mgr_ManufacturerId=BT_READ_LE_UINT16(pdu+10); 
				prh_mgr_LMPsubversion=BT_READ_LE_UINT16(pdu+12);                  
				break;
				
			case HCI_cmdREADLOCALNAME:
				if (!pdu[5] && !prh_mgr_LocalName) 
				{
					/* Find the NUL termination, but stop at max len of 248 */
					nameLen=0;
					while (pdu[6+nameLen] && nameLen<249)
						nameLen++;
					
					prh_mgr_LocalName=pMalloc(nameLen+1);
					if(prh_mgr_LocalName)
					{
						pMemcpy(prh_mgr_LocalName,pdu+6,nameLen);
						/* Force NUL termination */
						prh_mgr_LocalName[nameLen]=0;
					}
				}
				Bt_init_Finished=1;
				pDebugPrintfEX((pLOGNOTICE,pLOGHCI," zhou siyou Bt_init_Finished has be set" ));
				break;
				
			case HCI_cmdWRITEAUTOMATICFLUSHTIMEOUT:
				aclHandle = BT_READ_LE_UINT16(pdu+6);
				dbEntry = prh_mgr_get_deviceEntryByHandle(aclHandle, &linkType);
				
				if (!dbEntry) 
				{
					pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Manager: got command complete (WriteAutomaticFlushTimeout) event for handle not in database!"));
					break;
				}
				
				if(FLUSH_TO_SET_PENDING == dbEntry->aclFlushTimeoutFlag)
				{
					if(dbEntry->flushTOCallbackFunc)
					{
						/* If the status is OK, store the new flush time out value */
						if(HCI_errNOERROR == eventStatus)   
						{
							dbEntry->aclFlushTimeoutFlag = FLUSH_TO_SET;
							dbEntry->flushTOCallbackFunc(dbEntry->flushTOCallbackTid, BT_NOERROR);
						}
						else
						{
							dbEntry->aclFlushTimeoutFlag = FLUSH_TO_UNSET;
							dbEntry->aclFlushTimeout = DEFAULT_FLUSH_TIMEOUT;
							dbEntry->flushTOCallbackFunc(dbEntry->flushTOCallbackTid, prh_mgr_hci2bt_error(eventStatus));
						}
						
					}                
				}
				break;
				
			case HCI_cmdWRITELINKSUPERVISIONTIMEOUT:
				aclHandle = BT_READ_LE_UINT16(pdu+6);
				dbEntry = prh_mgr_get_deviceEntryByHandle(aclHandle, &linkType);
				if (!dbEntry)
				{
					pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Manager: got command complete (WriteLinkSupervisionTimeout) event for handle not in database!"));
					break;
				}
				
				if(LINK_TO_SET_PENDING == dbEntry->aclLinkSupTimeoutFlag)
				{
					/* If the status is OK, store the new link supervision time out value */
					if(BT_NOERROR == eventStatus)   
					{ /* Link timeout was changed, new value is passed back in a callback (to L2CAP) */
						dbEntry->aclLinkSupTimeout = dbEntry->aclLinkSupTimeoutNew;
						dbEntry->aclLinkSupTimeoutFlag = (u_int8)LINK_TO_SET;                   
					}
					else
					{ /* Link timeout was not changed, the old value is passed back in a callback (to L2CAP) */
                        eventStatus=prh_mgr_hci2bt_error(eventStatus);
						dbEntry->aclLinkSupTimeoutFlag = (u_int8)LINK_TO_UNSET;                                   
					}
					dbEntry->aclLinkSupTOCallbackFunc(dbEntry->aclLinkSupTOCallbackTid, eventStatus, dbEntry->aclLinkSupTimeout);   
				}
				break;
				
			case HCI_cmdREADLOCALSUPPORTEDFEATURES:
				if(!eventStatus && !prh_mgr_ReadLocalFeaturesComplete)
				{
					pMemcpy(prh_mgr_localLMPFeatures, pdu+6, NUM_BYTES_LMP_FEATURES);
					
					prh_mgr_linkpolicyflags=0;
					if (prh_mgr_localLMPFeatures[0] & 0x20) /* MSS */
						prh_mgr_linkpolicyflags |= 0x1;
					if (prh_mgr_localLMPFeatures[0] & 0x40) /* Hold */
						prh_mgr_linkpolicyflags |= 0x2;
					if (prh_mgr_localLMPFeatures[0] & 0x80) /* Sniff */
						prh_mgr_linkpolicyflags |= 0x4;
					if (prh_mgr_localLMPFeatures[1] & 0x01) /* Park */
						prh_mgr_linkpolicyflags |= 0x8;
					prh_mgr_ReadLocalFeaturesComplete=TRUE;
				}
				break;
				
			case HCI_cmdWRITEPINTYPE:
				if(prh_callback_set_pinType)
				{
					prh_callback_set_pinType(prh_mgr_hci2bt_error(eventStatus));
					prh_callback_set_pinType = NULL;
				}  
				break;
				
			case HCI_cmdREADPINTYPE:
				if(prh_callback_get_pinType)
				{
					prh_callback_get_pinType(prh_mgr_hci2bt_error(eventStatus), pdu[6]);
					prh_callback_get_pinType = NULL;
				}
				break;
				
			case HCI_cmdROLEDISCOVERY :
			case HCI_cmdREADLINKPOLICYSETTINGS :
			case HCI_cmdWRITELINKPOLICYSETTINGS :
				
				break;
				
			case HCI_cmdREADBDADDR :
				if (pdu[5]==0) { /* status */
					pMemcpy(&prh_mgr_LocalBDADDR, pdu+6, sizeof(prh_mgr_LocalBDADDR));              
					pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"*** local BDaddress %s ***\n",prh_mgr_printBDAddress(prh_mgr_LocalBDADDR)));
				} else {
					/* workaround for an ericsson bug which sometimes returns an error for readbdaddress */
					HCI_ReadBDAddress(); 
				}
				break;

			case TCI_WRITE_LOCAL_DEVICE_ADDRESS:
				pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"write local address event received , it is about to callback.....\n"));
				if(prh_mgr_WriteAddress_callbackFunc)
				{
					prh_mgr_WriteAddress_callbackFunc(pdu[5]);
					prh_mgr_WriteAddress_callbackFunc = NULL;
				}
				break;

			case TCI_READ_PHY_REGISTER:
				if(prh_mgr_ReadPHYRegister_callbackFunc)
				{
					prh_mgr_ReadPHYRegister_callbackFunc(pdu[5], BT_READ_LE_UINT32(pdu+6));
					prh_mgr_ReadPHYRegister_callbackFunc = NULL;
				}
				break;

			case TCI_WRITE_PHY_REGISTER:
				if(prh_mgr_WritePHYRegister_callbackFunc)
				{
					prh_mgr_WritePHYRegister_callbackFunc(pdu[5]);
					prh_mgr_WritePHYRegister_callbackFunc = NULL;
				}
				break;
			
			case TCI_BASEBAND_RXTEST:
				if(prh_mgr_BaseBandRxText_callbackFunc)
				{
					prh_mgr_BaseBandRxText_callbackFunc(pdu[5]);
					prh_mgr_BaseBandRxText_callbackFunc = NULL;
				}
				break;
			case TCI_BASEBAND_TXTEST:
				if(prh_mgr_BaseBandTxText_callbackFunc)
				{
					prh_mgr_BaseBandTxText_callbackFunc(pdu[5]);
					prh_mgr_BaseBandTxText_callbackFunc = NULL;
				}
				break;
	
			case TCI_BASEBAND_TESTEND:
				if(prh_mgr_BaseBandTestEnd_cbFunc)
				{
					prh_mgr_BaseBandTestEnd_cbFunc(pdu[5]);
					prh_mgr_BaseBandTestEnd_cbFunc = NULL;
				}
				 break;
				
			default:
				break;
				
        }   /* End of Command Complete Event Switch Statement */
    default :
        break;
    }; /* End of Event Handling Switch Statement */
	
    return BT_NOERROR;
}



void  prh_mgr_callback_DeviceDiscovery(struct st_t_BT_DeviceEntry *newDevice, t_api status_t)
{

    struct st_t_BT_InquiryInfo *next = NULL;
    struct st_t_BT_DeviceEntry *tmp = NULL;
	u_int8 i;
    int requestIssued=0;

    if(prh_mgr_discoveryInProgress == FALSE ||prh_mgr_disoverFlag == 1)            //this should not be happen
        	return ; 

    if(prh_mgr_InquiryCanceled == 1)
    	{
    		prh_mgr_DiscoveryCanceledComplete();
		if( prh_mgr_InquiryCancelCallBack)
		{
			prh_mgr_InquiryCancelCallBack(prh_mgr_deviceNameRetrieved, 0);		
		}

		HCI_WritePageTimeout(0x2000);
		
		prh_mgr_InquiryCancelCallBack = NULL;
		prh_mgr_InquiryCanceled = 0;
		
    		return ;
    	}

pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_callback_DeviceDiscovery newDevice=0x%x, status=%d\n", newDevice, status_t));

    if(newDevice==NULL) 
   { /* the inquiry is complete */
	pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"a new device has been found, set the condition variable for name request, or search finished\n"));

		prh_mgr_nameRequestStarted = TRUE;
        MGR_GetNextDeviceRecord(NULL, &tmp);
        while (tmp) 
	{

            next=(struct st_t_BT_InquiryInfo *)tmp;
            if (  next->entryAge<1 && next->nameRetrieved == 0 
			&& (prh_mgr_ClassofDeviceToRetrieve & next->classOfDevice
			|| prh_mgr_ClassofDeviceToRetrieve == 0))
	    {   
	    		next->deviceDiscoveryStatus=1; /* 0x01 == pending name request */
		   	pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"set the variable deviceDiscoveryStatus.............\n"));
            }
	     MGR_GetNextDeviceRecord(tmp,&tmp);
        };      
    } 
   else 
   {
        if (prh_mgr_nameRequestStarted) 
	{
            /* newDevice has its name member filled out, so make the callback */
            ((struct st_t_BT_InquiryInfo *)newDevice)->deviceDiscoveryStatus=0;
			((struct st_t_BT_InquiryInfo *)newDevice)->nameRetrieved =1;
            for (i=0; i<MAX_OUTSTANDING_CALLBACKS; i++) 
	     {
	            if(discoveryCallbackFunc[i] && newDevice->deviceName != NULL) 
			{
			     pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"make the callback to notify the MMI a device name and address has been retrieved.................\n"));
			     prh_mgr_deviceNameRetrieved++;
		            discoveryCallbackFunc[i](newDevice, status_t);
                	}
            }
        } 
	else 
	{
            /* this is an inquiry response - ignore it */
            return;
        }
    }


    /* make the next name request, or the final NULL callback , 
	   only if the device discovery is error free 
	*/
    if(status_t == BT_NOERROR && prh_mgr_InquiryCanceled == 0 && prh_mgr_DiscoveryTimeout == 0)
	{
       	 MGR_GetNextDeviceRecord(NULL, &tmp);
        	while (tmp && !requestIssued) 
		{
            		next=(struct st_t_BT_InquiryInfo *)tmp;
            		if (next->deviceDiscoveryStatus==1) 
	    		{ /* 0x01 == pending name request */

		  		pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"trigger a  name retrieve request .................\n"));

                		HCI_RemoteNameRequest(next->bdAddress, next->pageScanRepetitionMode, next->pageScanMode, next->clockOffset);
                		requestIssued=1;
				theEntryRetrievingName = next;
		  		pMemcpy(&prh_mgr_pendingBDAddress.bytes,next->bdAddress.bytes,6);
		  		prh_mgr_release_inquiry_entry(next);
		  		break;
	     		}
		       MGR_GetNextDeviceRecord(tmp,&tmp);
		};    
	}

    
	if(status_t != BT_NOERROR && prh_mgr_InquiryCanceled == 0){      
		/* Discovery has failed, make the final callback */
         void (*tempFuncPtr)(struct st_t_BT_DeviceEntry *newDevice_t, t_api status);
		
#ifdef COOLSAND				//
	  	if(prh_mgr_DiscoveryTimeout == 0)
		{
			pTimerCancel(prh_mgr_InquiryTimerHandle);
			prh_mgr_InquiryTimerHandle = 0;
		}		
	         do
		  {
	             		prh_mgr_get_callback(&i,(void **)discoveryCallbackFunc,(void **)&tempFuncPtr,NULL);	
	             		if (tempFuncPtr) 
	                 		tempFuncPtr(NULL, 0);      
	         } while (tempFuncPtr);
		HCI_WritePageTimeout(0x2000);		
	  	prh_mgr_DiscoveryCanceledComplete();	
#else
		 /* Pass back via the callback any discovered device */
		 MGR_GetNextDeviceRecord(NULL, &tmp);
         do {
             		prh_mgr_get_callback(&i,(void **)discoveryCallbackFunc,(void **)&tempFuncPtr,NULL);
	
             		if (tempFuncPtr) 
                 		tempFuncPtr(tmp, status_t);      
			 
			 MGR_GetNextDeviceRecord(tmp, &tmp);
         } while (tempFuncPtr);
		prh_mgr_nameRequestStarted = FALSE;
		prh_mgr_discoveryInProgress = FALSE;
#endif		
    }
	else if (!tmp && !requestIssued) 
	{   					
		void (*tempFuncPtr)(struct st_t_BT_DeviceEntry *newDevice_t, t_api status_tt);

		/* Remote Name request has been performed for all recently discovered devices, make the final callback */
#ifdef SLICE_INQUIRY_SUPPORT
		if(prh_mgr_MaxResult > 0 && prh_mgr_DiscoveryTimeout == 0 && prh_mgr_InquiryCanceled == 0)
		{	
			pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"maxResult still not equals 0 and time has not out, so continue search device\n"));

			prh_mgr_inquiryInProgress = TRUE;
			HCI_Inquiry(prh_mgr_AccessCode, 300, 15);
			prh_mgr_disoverFlag = 1;
			prh_mgr_nameRequestStarted = FALSE;
			return ;
		}
		else 
#endif
		{
			if(prh_mgr_DiscoveryTimeout == 0)
			{
				pTimerCancel(prh_mgr_InquiryTimerHandle);
				prh_mgr_InquiryTimerHandle = 0;
			}
			
				pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Search finished, make final callback......device Number:%d, timerout? %d \n", prh_mgr_deviceNameRetrieved, prh_mgr_DiscoveryTimeout?1:0));

				do 
				{
					prh_mgr_get_callback(&i,(void **)discoveryCallbackFunc,(void **)&tempFuncPtr,NULL);
					if (tempFuncPtr) 
						tempFuncPtr(NULL, status_t);             
				} while (tempFuncPtr);
				

			HCI_WritePageTimeout(0x2000);

			prh_mgr_ClassofDeviceToRetrieve = 0;
			prh_mgr_IsNameNeedRetrieve = NONEED_TO_RETRIEVE_NAME;
			prh_mgr_InquiryCanceled = 0;
			prh_mgr_nameRequestStarted =  FALSE;
			prh_mgr_disoverFlag = 0;
			prh_mgr_discoveryInProgress = FALSE;
			inquiryCallbackFunc[0] = NULL;
			discoveryCallbackFunc[0] =  NULL;	
		}
    }
	
}

/*************************************************************************************************
* FUNCTION :- prh_mgr_inquiry_callback_holdAllOpenACLs
*
* DESCRIPTION :- If successful, callback with the results.
* 
************************************************************************************************/

void prh_mgr_inquiry_callback_holdAllOpenACLs(t_api status)
{
	u_int8 i;
	t_api retVal = BT_NOERROR;
	
    if (!status) {
        retVal=HCI_Inquiry(pendingInquiryAccessCode, pendingInquiryLength, pendingInquiryMaxResults);
        if (!retVal) {
            prh_mgr_inquiryInProgress=TRUE;
        }
    } else {
		retVal=BT_UNKNOWNERROR;
    }
	if(retVal) {
        void (*tempFuncPtr)(struct st_t_BT_InquiryInfo *newDevice, t_api status_t);
        prh_mgr_get_callback(&i,(void **)inquiryCallbackFunc,(void **)&tempFuncPtr,NULL);
        if (tempFuncPtr) 
            tempFuncPtr(NULL, retVal);
    }
}


/*************************************************************************************************
* FUNCTION :- prh_mgr_createACL_callback_StartholdAllOpenACLs
*
* DESCRIPTION :- 
* This function is invoked when all currently active ACLs have been put on hold via the 
* prh_mgr_holdAllOpenACLs primitive.
************************************************************************************************/

void prh_mgr_createACL_callback_StartholdAllOpenACLs(t_api status)
{
    struct st_t_BT_InquiryInfo *inquiryInfo;
	u_int8 i;
	
	if(holdAllACLsPendingACLDbEntry)
	{
		if(BT_NOERROR == status)
		{
            inquiryInfo=prh_mgr_retrieve_inquiry_entry(holdAllACLsPendingACLDbEntry->bdAddress);
            if (inquiryInfo) {
                HCI_CreateConnection(holdAllACLsPendingACLDbEntry->bdAddress,MGR_DEFAULTPACKETTYPE,inquiryInfo->pageScanRepetitionMode,inquiryInfo->pageScanMode,inquiryInfo->clockOffset,(u_int8)((prh_mgr_linkFlags&MGR_CONNSETUP_ALLOW_SWITCH)?1:0));
                prh_mgr_release_inquiry_entry(inquiryInfo);
            } else {
                HCI_CreateConnection(holdAllACLsPendingACLDbEntry->bdAddress,MGR_DEFAULTPACKETTYPE,HOST_PAGING_DEFAULT_PSRM,HOST_PAGING_DEFAULT_PSM,HOST_PAGING_DEFAULT_CLKOFFSET,(u_int8)((prh_mgr_linkFlags&MGR_CONNSETUP_ALLOW_SWITCH)?1:0));
            }
            
		}
		else
		{
			holdAllACLsPendingACLDbEntry->aclState = CONN_CLOSED;
            holdAllACLsPendingACLDbEntry->recordState&=~MGR_DEVDB_CONNECTION_ACTIVE;

            {
                 void (*tempFuncPtr)(tid_t transactionId, t_api status_t);
                 do {
                     prh_mgr_get_callback(&i,(void **)holdAllACLsPendingACLDbEntry->aclCallbackFunc,(void **)&tempFuncPtr,holdAllACLsPendingACLDbEntry);
                     if (tempFuncPtr) {
                         *holdAllACLsPendingACLDbEntry->aclCallbackReturn[i]=holdAllACLsPendingACLDbEntry->aclHandle;
                         tempFuncPtr(holdAllACLsPendingACLDbEntry->aclCallbackTid[i], status); 
                     }
                 } while (tempFuncPtr);
            }

		}
		
	}
	
}

/*************************************************************************************************
* FUNCTION :- prh_mgr_createACL_callback_EndholdAllOpenACLs
*
* DESCRIPTION :- 
* This function is invoked when all devices previously put into hold mode via 
* prh_mgr_holdAllOpenACLS have come out of hold mode.
************************************************************************************************/

void prh_mgr_createACL_callback_EndholdAllOpenACLs(t_api status)
{
	u_int8 i=0;
	
	if(holdAllACLsPendingACLDbEntry)
	{
        if (holdAllACLsPendingACLDbEntry->aclState==CONN_ACTIVE)
        {
             void (*tempFuncPtr)(tid_t transactionId, t_api status_t);
             do {
                 prh_mgr_get_callback(&i,(void **)holdAllACLsPendingACLDbEntry->aclCallbackFunc,(void **)&tempFuncPtr,holdAllACLsPendingACLDbEntry);
                 if (tempFuncPtr) {
                     *holdAllACLsPendingACLDbEntry->aclCallbackReturn[i]=holdAllACLsPendingACLDbEntry->aclHandle;
                     tempFuncPtr(holdAllACLsPendingACLDbEntry->aclCallbackTid[i], status);              
                 }
             } while (tempFuncPtr);
        }
		holdAllACLsPendingACLDbEntry = NULL;
	}
}

/*************************************************************************************************
* FUNCTION :- prh_mgr_updateFlushTimeout
*
* Input Parameters
*      u_int16 handle                      The HCI Connection Handle for the ACL link
*      u_int16 *outFlushTimeout            A pointer to the flush timeout (in Milliseconds)
*      u_int16 flushTOCallback             The callback function to be invoked when the HCI command
*                                          is complete.
*      tid_t transactionId                 Unique transaction identifier for the operation,    
*
* Returns
*      t_api               BT_NOERROR
*                          BT_PENDING
*                          BT_INVALIDPARAM
*                          BT_UNKNOWNERROR
* DESCRIPTION :- 
* This function writes a new Flush Timout value for a given ACL link to the lower stack
* using the HCI. A callback is stored  which is invoked on receipt of the outcome of the operation.
* The callback function will be invoked when the HCI return a Connand Status containing an Error 
* OR when the HCI returns Command Complete for the operation.
*
*
************************************************************************************************/

t_api prh_mgr_updateFlushTimeout(u_int16 handle, u_int16* outFlushTimeout,
                                 void (*flushTOCallback)(tid_t transactionId, t_api status_t), tid_t transactionId)
{
    u_int8 status=0;
    struct st_t_BT_ConnectionInfo *devDbEntry;
	u_int8 linkType;
	
    devDbEntry = prh_mgr_get_deviceEntryByHandle(handle, &linkType);
    if(!devDbEntry || HCI_linkACL != linkType)
        return BT_INVALIDPARAM;
	
    if(FLUSH_TO_UNSET == devDbEntry->aclFlushTimeoutFlag)
    { 
		if ((*outFlushTimeout != 0xFFFF) && (*outFlushTimeout >=  0x500 /* Max Flush TO in Milisec */))
		{
			status = BT_INVALIDPARAM;
		}
		else
		{
			/* Flush Timeout will be set via HCI , command_complete event is awaited */
			devDbEntry->aclFlushTimeoutFlag = (u_int8)FLUSH_TO_SET_PENDING;
			devDbEntry->flushTOCallbackTid = transactionId;
			devDbEntry->flushTOCallbackFunc = flushTOCallback;
			devDbEntry->aclFlushTimeout = *outFlushTimeout; /* If HCI cmd fails, revert back to default */
			if (*outFlushTimeout == 0xFFFF)
			{
				status = HCI_WriteAutomaticFlushTimeout(handle,0);
			}
			else
			{
				status = HCI_WriteAutomaticFlushTimeout(handle, (u_int16)MGR_mSec_To_Slots(*outFlushTimeout));
			}
		}
        if(BT_NOERROR == status)
            return BT_PENDING;
        else
            return status;
    }
	
    if(devDbEntry->aclFlushTimeout == *outFlushTimeout || *outFlushTimeout == 0)
        return BT_NOERROR; /* Flush Timeout is unchanged and request has been accepted */
    else
    {
        *outFlushTimeout = devDbEntry->aclFlushTimeout;
        return BT_UNKNOWNERROR; /* Flush Timeout is unchanged and request has been rejected */
    }
}


/*************************************************************************************************
* FUNCTION :- prh_mgr_updateLinkSupervisionTimeout
*
* Input Parameters
*      u_int16 handle                      The HCI Connection Handle for the ACL link
*      u_int16 *linkTimeout                A pointer to the link supervision timeout (in Milliseconds)
*	   u_int16 linkTOCalleback			   The callback function to be invoked when the HCI command
*                                          is complete.
*      tid_t transactionId                 Unique transaction identifier for the operation,    
*
* Returns
*      t_api               BT_NOERROR
*                          BT_PENDING
*                          BT_INVALIDPARAM
* DESCRIPTION :- 
* This function writes a new Link Supervision Timout value for a given ACL link to the lower stack
* using the HCI. A callback is stored  which is invoked on receipt of the outcome of the operation.
* The callback function will be invoked when the HCI return a Connand Status containing an Error 
* OR when the HCI returns Command Complete for the operation.
*
************************************************************************************************/

t_api prh_mgr_updateLinkSupervisionTimeout(u_int16 handle, u_int16 *linkTimeout, void (*linkTOCallback)(tid_t, t_api, u_int16), tid_t transactionId)										   
{
    u_int8 status=0;
    struct st_t_BT_ConnectionInfo *devDbEntry;
	u_int8 linkType;
	
    devDbEntry = prh_mgr_get_deviceEntryByHandle(handle, &linkType);
    if(!devDbEntry || HCI_linkACL != linkType)
        return BT_INVALIDPARAM;
	
    if(LINK_TO_UNSET == devDbEntry->aclLinkSupTimeoutFlag || devDbEntry->aclLinkSupTimeout >= *linkTimeout)
    {
		if (*linkTimeout < 0xA000 /* Supervision TO in MiliSec */)
		{
			devDbEntry->aclLinkSupTimeoutFlag = (u_int8)LINK_TO_SET_PENDING;
			devDbEntry->aclLinkSupTimeoutNew = *linkTimeout;
			devDbEntry->aclLinkSupTOCallbackTid = transactionId;
			devDbEntry->aclLinkSupTOCallbackFunc = linkTOCallback;
			status = HCI_WriteLinkSupervisionTimeout(handle, (u_int16)MGR_mSec_To_Slots(*linkTimeout));
			if(BT_NOERROR == status)
				return BT_PENDING;
			else
				return status;
		}
		else
		{
			return BT_INVALIDPARAM;
		}
    }
    else /* Requested Link TO is greater than the existing value that was set via the Manager */
    {
        *linkTimeout = devDbEntry->aclLinkSupTimeout; /* return the current value to requester (L2CAP) */
        return BT_NOERROR;
    }
}

/*************************************************************************************************
* FUNCTION :- prh_mgr_updateQoS
*
* Input Parameters
*      u_int16 handle                      The HCI Connection Handle for the ACL link
*      t_L2_ChannelFlowSpec *p_flowSpec    A pointer the structure containing the requested QOS values
*      updateQoSCallback                   The callback function to be invoked when the HCI command
*                                          is complete.
*
* Returns
*      t_api               BT_NOERROR
*                          BT_PENDING
*                          BT_INVALIDPARAM
* DESCRIPTION :- 
* This function writes a set of QOS values for a given ACL link to the lower stack
* using the HCI. A callback is stored  which is invoked on receipt of the outcome of the operation.
* The callback function will be invoked when the HCI return a Connand Status containing an Error 
* OR when the HCI returns Command Complete for the operation.
************************************************************************************************/

t_api prh_mgr_updateQoS(u_int16 handle,t_L2_ChannelFlowSpec *p_flowSpec, void (*updateQoSCallback)(tid_t,t_L2_ChannelFlowSpec*, t_api), tid_t transactionId)
{
    u_int8 status=0;
    struct st_t_BT_ConnectionInfo *devDbEntry;
	u_int8 linkType;
	
    devDbEntry = prh_mgr_get_deviceEntryByHandle(handle, &linkType);
    if(!devDbEntry || HCI_linkACL != linkType)
        return BT_INVALIDPARAM;
	
    devDbEntry->qosSetupCallbackTid = transactionId;
    devDbEntry->qosSetupCallbackFunc = updateQoSCallback;
    status = HCI_QoSSetup(handle, p_flowSpec->flags, p_flowSpec->serviceType, p_flowSpec->tokenRate, 
		p_flowSpec->peakBandwidth, p_flowSpec->latency, p_flowSpec->delayVariation);
    if(BT_NOERROR == status)
        return BT_PENDING;
    else
        return status;
}

/*************************************************************************************************
* FUNCTION :- MGR_GetDeviceVersion
*      
* DESCRIPTION :- 
* Reads the local device version information, if it is available in the host. 
************************************************************************************************/
APIDECL1 t_api APIDECL2 MGR_GetDeviceVersion(t_MGR_HCVersion *ver) {
	
    ver->HCIversion=prh_mgr_HCIversion;
    ver->HCIrevision=prh_mgr_HCIrevision;
    ver->LMPversion=prh_mgr_LMPversion;
    ver->LMPsubversion=prh_mgr_LMPsubversion;
    ver->ManufacturerId=prh_mgr_ManufacturerId;
	
    return BT_NOERROR;
}

/*************************************************************************************************
* FUNCTION :- MGR_GetHostVersion
*      
* DESCRIPTION :- 
* Returns a host version identification string. 
************************************************************************************************/
APIDECL1 char * APIDECL2 MGR_GetHostVersion(void) {
    return BT_VALUEOF(BT_HOST_VERSION)"."BT_VALUEOF(BT_HOST_REVISION)"."BT_VALUEOF(BT_HOST_PATCHLEVEL)" ("BT_VALUEOF(PLATFORMSTRING)")";
}

/*************************************************************************************************
* FUNCTION :- MGR_GetLocalName
*      
* DESCRIPTION :- 
* Reads the local deice name, if it is available in the host. 
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_GetLocalName(char *outBuffer, u_int8 bufLen, u_int8 *nameLen) {
	
    u_int8 status=0;

    if (prh_mgr_LocalName) {
        *nameLen=pStrlen((char *)prh_mgr_LocalName);
        pStrncpy(outBuffer,(char *)prh_mgr_LocalName,bufLen);
        if (*nameLen+1>bufLen)
            *(outBuffer+bufLen-1)=0;
        return BT_NOERROR;
    } else {
        status = HCI_ReadLocalName();
        if(BT_NOERROR == status)
            return BT_RETRY;    
        else
            return status;
    }
}


/*************************************************************************************************
* FUNCTION :- MGR_ChangeLocalName
*
* DESCRIPTION :- change the local device name.
*
* NOTICE:	parameter should be a null-terminated string, and this function will not CHECK validity
*
*			of the parameter
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_ChangeLocalName(u_int8 *name)
{
	return  HCI_ChangeLocalName(name);	
}


/*************************************************************************************************
* FUNCTION :- MGR_WriteLocalDeviceAddress
*
* DESCRIPTION :- write the local device address.
*
* NOTICE:	this function will not CHECK validity of the parameter
*
************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_WriteLocalDeviceAddress(u_int8 *address, void (*callbakFunc)(t_api status))
{
	//unsigned char  foo[6] = {0};
	
	//t_bdaddr addr;
	
	//int i = 0;

	pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"MGR_WriteLocalDeviceAddress function has been called\n"));
	
	prh_mgr_WriteAddress_callbackFunc = callbakFunc;
	
	//MGR_ParseHexBDAddress((char*)address, &addr);
	
	//for(i= 0; i<6; i++)
		//foo[i]  = addr.bytes[i];
	
	return TCI_WriteLocalDeviceAddress(address);
	
}

APIDECL1 t_api APIDECL2 MGR_ReadPHYRegister(u_int8 flag, u_int32 address, void(*callbakFunc)(t_api status, int result))
{
	prh_mgr_ReadPHYRegister_callbackFunc = callbakFunc;
	
	return TCI_ReadPHYRegister(flag, address);
}


APIDECL1 t_api APIDECL2 MGR_WritePHYRegister(u_int8 flag, u_int32 address, u_int8 count, u_int32 *value, void (*callbakFunc)(t_api status))
{
	prh_mgr_WritePHYRegister_callbackFunc = callbakFunc;
	
	return TCI_WritePHYRegister(flag, address, count, value);
}


APIDECL1 t_api APIDECL2 MGR_BaseBand_TXTest( u_int8 hop_frq, u_int8 msg_sco, u_int8 msg_burst,
											u_int8 msg_type, u_int8 power_level,u_int16 PRbs_init, u_int16 msg_data,
											u_int8 tx_packet_type, u_int8 EDR_mode, u_int32 tx_len,
											u_int8 am_addr, u_int32 syncword_low, u_int32 syncword_hight, 
							void (*callbakFunc)(t_api status))
{
	prh_mgr_BaseBandTxText_callbackFunc = callbakFunc;

	return TCI_BaseBand_TXTest(hop_frq, msg_sco, msg_burst, msg_type, 
								power_level, PRbs_init, msg_data, tx_packet_type,
								EDR_mode, tx_len, am_addr, syncword_low, syncword_hight);
}


APIDECL1 t_api APIDECL2 MGR_BaseBand_RXTest(void(*callbackFunc)(t_api status))
{
	 prh_mgr_BaseBandRxText_callbackFunc = callbackFunc;

	 return TCI_BaseBand_RXTest();
}


APIDECL1 t_api APIDECL2 MGR_BaseBand_TestEnd(void(*callbackFunc)(t_api status))
{
	prh_mgr_BaseBandTestEnd_cbFunc = callbackFunc;
	return TCI_BaseBand_TestEnd();
}


/*************************************************************************************************
* FUNCTION :- prh_mgr_handle_HCI_evMODECHANGE
*
* DESCRIPTION :- This function is the handler for the device mode change event.
*               It adjust variables as neccessary and makes the relevant callback.
* 
************************************************************************************************/


void prh_mgr_handle_HCI_evMODECHANGE(u_int16 aclHandle, u_int8 eventStatus, u_int8 newMode, u_int16 interval)
{
	struct st_t_BT_ConnectionInfo *dbEntry;
	t_api mgrStatus;
	u_int8 oldMode;
	u_int8 linkType;

	dbEntry=prh_mgr_get_deviceEntryByHandle(aclHandle, &linkType);
	if( !dbEntry || HCI_linkACL != linkType  )
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"Manager: got mode change event for invalid handle!"));
		return;
	}
	oldMode = dbEntry->deviceMode;

    mgrStatus=prh_mgr_hci2bt_error(eventStatus);

	if(dbEntry)
	{

		if(BT_NOERROR == mgrStatus)
			dbEntry->deviceMode = newMode;
		else
		{	/* attempted change of mode failed */


			if(MGR_DEVPENDINGHOLD == oldMode || MGR_DEVPENDINGSNIFF == oldMode || MGR_DEVPENDINGPARK == oldMode)
				dbEntry->deviceMode = MGR_DEVACTIVE;
		}


		switch(newMode)
		{	
	
		case MGR_DEVHOLD:
			
			if(0 != numPendingStartHolds)
			{
				/* If all devices have entered hold mode invoke this callback */
				if(--numPendingStartHolds == 0 && MGR_holdAllOpenACLsStartCallback)
				{
					MGR_holdAllOpenACLsStartCallback(mgrStatus); 
					MGR_holdAllOpenACLsStartCallback = NULL;
				}
			}				
		

		
			if(dbEntry->holdACLCallbackFunc)
			{
				dbEntry->holdACLCallbackFunc(mgrStatus, dbEntry->aclHandle, interval);
				dbEntry->holdACLCallbackFunc = NULL;
			}
			
			break;

		case MGR_DEVACTIVE:	
			if(BT_NOERROR == mgrStatus)
				dbEntry->deviceMode = newMode;

			switch(oldMode)
			{

			case MGR_DEVHOLD:
												
				if(0 != numPendingEndHolds)
				{
					/* If all previously held devices have come out of hold invoke this callback */
					if(--numPendingEndHolds == 0 && MGR_holdAllOpenACLsEndCallback)
					{
						MGR_holdAllOpenACLsEndCallback(prh_mgr_hci2bt_error(MGR_holdAllOpenACLsEndCallbackStatus));
						MGR_holdAllOpenACLsEndCallback = NULL;
					}
				}				
				

				
				if(dbEntry->exitHoldACLCallbackFunc)
				{
					dbEntry->exitHoldACLCallbackFunc(mgrStatus, dbEntry->aclHandle);
					dbEntry->exitHoldACLCallbackFunc = NULL;
				}
				
				break;

			}
			break;

		default:
			if(BT_NOERROR == mgrStatus)
				dbEntry->deviceMode = newMode;
			break;
		}
	}
	
}
/*************************************************************************************************
* FUNCTION :- prh_mgr_setupPendingCommandStatus
*
* DESCRIPTION :- Sets up a pending command status for a particular device.
* 
************************************************************************************************/
t_api prh_mgr_setupPendingCommandStatus(u_int8 hciCmdIndex, struct st_t_BT_ConnectionInfo *devDbEntry)
{
	u_int8 newIndex;

	if(pendingCmdStatus[hciCmdIndex].count >= MGR_MAXACTIVEACLS)
		return BT_RETRY;
	newIndex = pendingCmdStatus[hciCmdIndex].currentIndex+pendingCmdStatus[hciCmdIndex].count;
	if(newIndex > MGR_MAXACTIVEACLS-1)
		newIndex -= MGR_MAXACTIVEACLS;
	pendingCmdStatus[hciCmdIndex].count++;
	pendingCmdStatus[hciCmdIndex].devDbEntry[newIndex] = devDbEntry;

	return BT_NOERROR;
}

static void prh_mgr_DiscoveryCanceledComplete()
{
	prh_mgr_ClassofDeviceToRetrieve = 0;
	prh_mgr_InquiryCanceled = 0;
       prh_mgr_InquiryTimerHandle = 0;
       prh_mgr_IsNameNeedRetrieve = NONEED_TO_RETRIEVE_NAME;
       prh_mgr_disoverFlag = 0;
	prh_mgr_nameRequestStarted = FALSE;
	prh_mgr_inquiryInProgress  = FALSE;
	prh_mgr_discoveryInProgress = FALSE;
	inquiryCallbackFunc[0] = NULL;
	discoveryCallbackFunc[0] =  NULL;	
}

void  prh_mgr_DiscoveryTimerCallBack(void * handle)
{
	pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"the discovery timeout....prh_mgr_disoverFlag=%d,theEntryRetrievingName=0x%x  \n",prh_mgr_disoverFlag,theEntryRetrievingName));

	prh_mgr_DiscoveryTimeout = 1;
	prh_mgr_InquiryTimerHandle = 0;

	if(prh_mgr_InquiryCanceled == 1)
	{
		pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"the discovery timeout, but the user has canceled this inquiry process..\n"));
		prh_mgr_inqdb_inquiry_complete();
		prh_mgr_DiscoveryCanceledComplete();	
		return ;
	}

	if(prh_mgr_disoverFlag == 1)		//inquiry is in process
		HCI_InquiryCancel();
	else								//host is retrieving the name ......, notice the MMI right away..
	{
		if( theEntryRetrievingName)
			HCI_RemoteNameRequestCancel(theEntryRetrievingName->bdAddress);
		else
			prh_mgr_callback_DeviceDiscovery(NULL, 0);
	}
}


