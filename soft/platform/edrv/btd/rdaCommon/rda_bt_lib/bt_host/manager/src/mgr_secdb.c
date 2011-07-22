/*
 * MODULE NAME:    mgr_secdb.c
 * DESCRIPTION:    Host Manager Service Security Database Functionality
 * AUTHOR:         Tom Howley
 * DATE:           30 June 2000
 *
 * SOURCE CONTROL: $Id: mgr_secdb.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"
#include "mgr_secdb.h"
#include "mgr_utils.h"


u_int16 tmpHandle; 


/* LOCAL FUNCTIONS */
static t_api StartBonding(struct st_t_BT_ConnectionInfo *currentDevice, void (*callbackFunc)(t_bdaddr bdaddress, t_api status));
static t_api ProcessAccessRequest(u_int8 direction, struct st_t_mgr_secdbentry *secDbEntry, t_bdaddr address, callbackFuncPtr_t callbackFunc, u_int32 cbHandle);
static struct st_t_mgr_secdbentry *AddNewServiceEntry(void);
static struct st_t_mgr_secdbentry *GetServiceEntryByAppName(char *serviceName);
static struct st_t_mgr_secdbentry *GetServiceEntryByProtocolId(u_int16 multiplexProtocolId);
static struct st_t_mgr_secdbentry *GetServiceEntryByMultiplexInfo(u_int16 multiplexProtocolId, u_int32 multiplexChannelId);


/* STATIC VARIABLES */
static void (*prh_callback_authorisation)(struct st_t_BT_Authorisation *) = NULL;
static void (*prh_callback_pinRequest)(t_bdaddr ) = NULL;
static u_int8 prh_pairing_mode = MGR_PAIRABLE;        /* Default Pairing Mode for Host */
static u_int8 prh_allzero_pin[MAX_PIN_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  


void (*prh_callback_AcceptConnectionRequest)(t_bdaddr) = NULL;				//add by tan, 7-12-2009
/* GLOBAL VARIABLES */
struct st_t_mgr_secdbentry *prh_service_list_head = NULL, *prh_service_list_tail = NULL;
u_int8 prh_security_mode = MGR_SERVICELEVEL; /* Default Security Mode for Host */
u_int8 prh_security_mode_pending;
void (*prh_callback_set_pinType)(t_api status) = NULL;
void (*prh_callback_get_pinType)(t_api status, u_int8 pinType) = NULL; 
void (*setSecurityModeCallback)(t_api status);

u_int16 pendingSecHandle;
t_bdaddr prh_mgr_auth_supplied_link_key_addr;
u_int8 prh_mgr_auth_supplied_link_key;
/* API FUNCTIONS */


/****************************************************************************************************
 * FUNCTION :- MGR_InitBonding
 *
 * INPUTS:
 *         address:      the address of remote device that bonding is carried with.
 *         callbackFunc: function that is invoked when bonding procedure is completed.
 *
 * RETURNS:
 *         t_api: 
 *
 * DESCRIPTION :- Initiates a bonding procedure with a remote device. If an entry exists in the database
 * for this device, the bonding is started immediately with a call to StartBonding(). Otherwise, an
 * Inquiry procedure is started and the result of this inquiry is awaited.
 *
 ************************************************************************************************** */

APIDECL1 t_api APIDECL2 MGR_InitBonding(t_bdaddr address, void (*callbackFunc)(t_bdaddr address, t_api status))
{
  struct st_t_BT_ConnectionInfo *currentDevice;
  struct st_t_BT_InquiryInfo *inquiryInfo; 
  t_api status;

  pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_InitBonding called for %s\n",prh_mgr_printBDAddress(address)));

  
#if MGR_ALLOW_BONDING_INQUIRY   
  currentDevice = prh_mgr_find_deviceEntryByAddress(address);
#else
  currentDevice = prh_mgr_return_deviceEntryByAddress(address); /* will always return non-NULL, so no inquiry will be used */
#endif
  
  if(currentDevice) 
  {
  
  #ifdef COOLSAND					//04-07-2009
	inquiryInfo = prh_mgr_retrieve_inquiry_entry( address);
	if(inquiryInfo)
	{
		currentDevice->cod = inquiryInfo->classOfDevice;	
  		pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER," prh_mgr_retrieve_inquiry_entry found the entry, cod:%d\n", inquiryInfo->classOfDevice));
	}
       //prh_free_inquiry_db();
  #endif
  
      return StartBonding(currentDevice, callbackFunc);
  } else
    {
      currentDevice = prh_mgr_return_deviceEntryByAddress(address);
      if (!currentDevice)
          return BT_NORESOURCES;
      currentDevice->bondingState =  BOND_INQUIRY_PENDING;
      currentDevice->bondingCallbackFunc[0]= callbackFunc;

      status = MGR_Inquiry(BOND_INQUIRY_LENGTH, MGR_BONDING_INQUIRY_IAC, 0, prh_mgr_callback_bondingInquiry);
	 
	  /* If there was a problem with the Inquiry, update the bonding details */
	  if(status != BT_PENDING)
	  {
		 MGR_RemoveConnectionRecord((struct st_t_BT_ConnectionEntry*)currentDevice, 0x00);
	  }

	  return status;
    }
}




/****************************************************************************************************
 * FUNCTION :- MGR_RegisterAppSecurityLevel
 *
 * INPUTS:
 *         serviceName:         name of application to be registered.
 *         multiplexProtocolId: protocolId that identifies the lower layer multiplexing protocol.
 *         multiplexChannelId:  Id that is used by the multiplexing protocol to identify the application.
 *         securityLevel:       the security level required by the application.
 *
 * DESCRIPTION :- Registers the Security Level of an application in the Service Database. If an entry
 * already exists in the security database for this application, a BT_INVALIDPARAM error is returned.
 * Otherwise, a new entry is added to the database and the name, multiplexProtocolId, 
 * multiplexChannelId and security level are recorded.
 * 
 ****************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_RegisterAppSecurityLevel(char *serviceName, u_int16 multiplexProtocolId, u_int32 multiplexChannelId, u_int8 securityLevel)
{
  struct st_t_mgr_secdbentry *serviceEntry;
  u_int8 nameLen;

  pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_RegisterAppSecurityLevel called for service \"%s\", securityLevel=%d\n",serviceName, securityLevel));


  serviceEntry = GetServiceEntryByAppName(serviceName);
  if(!serviceEntry)
	serviceEntry = AddNewServiceEntry();
  if(serviceEntry)
    {
      if (serviceEntry->service.name) 
			{
          pFree(serviceEntry->service.name);
					serviceEntry->service.name = NULL;
			}
      serviceEntry->serviceType = MGR_APPSERVICE;
      nameLen = pStrlen(serviceName);
      if(nameLen <= MGR_MAXNAMELEN)
      {
        serviceEntry->service.name = (char *)pMalloc(nameLen+1);
				if(serviceEntry->service.name == NULL)
				{
					return BT_NORESOURCES;               
				}
				pStrcpy(serviceEntry->service.name, serviceName);
				
      }
      else /* Truncate Service Name */
      {
        serviceEntry->service.name = (char *)pMalloc(MGR_MAXNAMELEN+1);
				if(serviceEntry->service.name == NULL)
				{ 
					return BT_NORESOURCES;					
				}
				
				pStrncpy(serviceEntry->service.name, serviceName, MGR_MAXNAMELEN);
				serviceEntry->service.name[MGR_MAXNAMELEN] = '\0';
				
      }
      
      serviceEntry->multiplexProtocolId = multiplexProtocolId;
      serviceEntry->multiplexChannelId = multiplexChannelId;
      serviceEntry->securityLevel = securityLevel;
      
      return BT_NOERROR;
    }
  else
    return BT_NORESOURCES;
}





/****************************************************************************************************
 * FUNCTION :- MGR_RegisterProtocolSecurityLevel
 *
 * INPUTS:
 *         protocolId:          protocolId of protocol to be registered.
 *         multiplexProtocolId: protocolId that identifies the lower layer multiplexing protocol.
 *         multiplexChannelId:  Id that is used by the multiplexing protocol to identify the protocol.
 *         securityLevel:       the security level required by the protocol.
 *
 * RETURNS:
 *         t_api
 *
 * DESCRIPTION :- Registers the Security Level of a protocol in the Security Database. If an entry
 * already exists in the security database for this protocol, a BT_INVALIDPARAM error is returned.
 * Otherwise, a new entry is added to the database and the protocolId, multiplexProtocolId, 
 * multiplexChannelId and security level are recorded.
 * 
 ****************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_RegisterProtocolSecurityLevel(u_int16 protocolId, u_int16 multiplexProtocolId, u_int32 multiplexChannelId, u_int8 securityLevel)
{

  struct st_t_mgr_secdbentry *serviceEntry;

  pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_RegisterProtocolSecurityLevel called for protocol 0x%02X, securityLevel=%d\n",protocolId, securityLevel));


  serviceEntry = GetServiceEntryByProtocolId(protocolId);
  if(!serviceEntry)
	serviceEntry = AddNewServiceEntry();
  if(serviceEntry)
    {
      serviceEntry->serviceType = MGR_PROTOSERVICE;
      serviceEntry->service.protocolId = protocolId;
      serviceEntry->multiplexProtocolId = multiplexProtocolId;
      serviceEntry->multiplexChannelId = multiplexChannelId;
      serviceEntry->securityLevel = securityLevel;

      return BT_NOERROR;
    }
  else
    return BT_NORESOURCES;
}




/****************************************************************************************************
 * FUNCTION :- MGR_DeregisterAppSecurityLevel
 *
 * INPUTS:
 *         serviceName: name of application to be registered. 
 *
 * RETURNS:
 *         t_api
 *
 * DESCRIPTION :- If an entry exists in the Security Database for this application, it is removed.If
 * the entry is not found a BT_INVALIDPARAM error is returned.
 * 
 ****************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_DeregisterAppSecurityLevel(char *serviceName)
{
  struct st_t_mgr_secdbentry *secDbEntry = prh_service_list_head;
  struct st_t_mgr_secdbentry *tmpsecDbEntry;

  if(prh_service_list_head)
    {
      if(serviceName)
        {
          if (MGR_APPSERVICE == secDbEntry->serviceType && secDbEntry->service.name)
            {
              if (!pStrncmp(secDbEntry->service.name, serviceName,MGR_MAXNAMELEN))
                {
									if(prh_service_list_head == prh_service_list_tail)
									{
										prh_service_list_tail =NULL; 
										prh_service_list_head =NULL; 
									}
									else
									{
										prh_service_list_head = secDbEntry->nextService;
									}   
                  if (secDbEntry->service.name)
                      pFree(secDbEntry->service.name);
                  pFree(secDbEntry);
                  return BT_NOERROR;
                }
            }
          
   
          while (secDbEntry->nextService) {
            if (MGR_APPSERVICE == secDbEntry->nextService->serviceType && secDbEntry->nextService->service.name)
              {
                if (!pStrncmp(secDbEntry->nextService->service.name, serviceName,MGR_MAXNAMELEN)) 
                  {
                    if(prh_service_list_tail == secDbEntry->nextService)
                      prh_service_list_tail = secDbEntry;
                    tmpsecDbEntry = secDbEntry->nextService;
                    secDbEntry->nextService = secDbEntry->nextService->nextService;
                    if (tmpsecDbEntry->service.name)
											pFree(tmpsecDbEntry->service.name);
                    pFree(tmpsecDbEntry);
                    return BT_NOERROR;
                  }
              }
            secDbEntry=secDbEntry->nextService;
          }
        
        }
    }

  return BT_INVALIDPARAM; /* service not found */
}





/****************************************************************************************************
 * FUNCTION :- MGR_DeregisterProtocolSecurityLevel
 *
 * INPUTS:
 *         protocolId: Id of protocol to be registered. 
 *
 * RETURNS:
 *         t_api
 *
 * DESCRIPTION :- If an entry exists in the Security Database for this protocol, it is removed.If
 * the entry is not found a BT_INVALIDPARAM error is returned.
 * 
 ****************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_DeregisterProtocolSecurityLevel(u_int16 protocolId)
{

  struct st_t_mgr_secdbentry *secDbEntry = prh_service_list_head;
  struct st_t_mgr_secdbentry *tmpsecDbEntry;

  
  if(prh_service_list_head)
    {
      if (MGR_PROTOSERVICE == secDbEntry->serviceType && protocolId == secDbEntry->service.protocolId)
        {
					if(prh_service_list_head == prh_service_list_tail)
					{
						prh_service_list_tail =NULL; 
						prh_service_list_head =NULL; 
					}
					else
					{
						prh_service_list_head = secDbEntry->nextService;
					}
          pFree(secDbEntry);
          return BT_NOERROR;
        }
      
      
      
      while (secDbEntry->nextService) 
        {
          if (MGR_PROTOSERVICE == secDbEntry->nextService->serviceType && protocolId == secDbEntry->nextService->service.protocolId )
            {
              if(prh_service_list_tail == secDbEntry->nextService)
                prh_service_list_tail = secDbEntry;
              tmpsecDbEntry = secDbEntry->nextService;
              secDbEntry->nextService = secDbEntry->nextService->nextService;
              pFree(tmpsecDbEntry);
              return BT_NOERROR;
            }
          secDbEntry=secDbEntry->nextService;
        }
      

    }

  return BT_INVALIDPARAM; /* service not found */

}




/*************************************************************************************************
 * FUNCTION :-  MGR_ApplicationOutgoingAccessRequest
 *
 * INPUTS:
 *         serviceName:  name of application attempting to setup a connection.
 *         address:      address of the remote device that is being connected to.
 *         cbHandle:     a callback handle that is passed back when the access request is complete.
 *         callbackFunc: the callback function that is invoked when the access request is complete.
 *
 * RETURNS:
 *         t_api
 *
 * DESCRIPTION :- Handles an access request from an application that is attempting to setup a
 * connection to remote bluetooth device. The entry in the Security Database is retrieved based
 * on search by the application name. This database entry is passed to the ProcessRequest() function.
 *
 *************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_ApplicationOutgoingAccessRequest(char *serviceName, t_bdaddr address, u_int32 cbHandle, void (*callbackFunc)(u_int32 cbHandle, t_api status))
{
  struct st_t_mgr_secdbentry *secDbEntry;

  pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_ApplicationOutgoingAccessRequest called for service \"%s\" to address %s\n",serviceName, prh_mgr_printBDAddress(address)));

  secDbEntry = GetServiceEntryByAppName(serviceName);

  return ProcessAccessRequest(MGR_OUTGOING, secDbEntry, address, callbackFunc, cbHandle);
}





/*************************************************************************************************
 * FUNCTION :-  MGR_ProtocolOutgoingAccessRequest
 *
 * INPUTS:
 *         protocolId:   Id of protocol attempting to setup a connection.
 *         address:      address of the remote device that is being connected to.
 *         cbHandle:     a callback handle that is passed back when the access request is complete.
 *         callbackFunc: the callback function that is invoked when the access request is complete.
 *
 * RETURNS:
 *         t_api
 *
 * DESCRIPTION :- Handles an access request from a protocol that is attempting to setup a
 * connection to remote bluetooth device. The entry in the Security Database is retrieved based
 * on search by the protocolId. This database entry is passed to the ProcessRequest() function.
 *
 *************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_ProtocolOutgoingAccessRequest(u_int16 protocolId, t_bdaddr address, u_int32 cbHandle, void (*callbackFunc)(u_int32 cbHandle, t_api status))
{
  struct st_t_mgr_secdbentry *secDbEntry;

  pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_ProtocolOutgoingAccessRequest called for protocol 0x%02X to address %s\n",protocolId, prh_mgr_printBDAddress(address)));

  secDbEntry = GetServiceEntryByProtocolId(protocolId);

  return ProcessAccessRequest(MGR_OUTGOING, secDbEntry, address, callbackFunc, cbHandle);
}




/*************************************************************************************************
 * FUNCTION :-  MGR_MultiplexOutgoingAccessRequest
 *
 * INPUTS:
 *         multiplexProtocolId: Id of the multiplexing protocol that is submitting the request.
 *         multiplexChannelId:  Id of the upper layer service that is attempting to setup a connection.
 *         address:             address of the remote device that is being connected to.
 *         cbHandle:            a callback handle that is passed back when the access request is complete.
 *         callbackFunc:        the callback function that is invoked when the access request is complete.
 *
 * RETURNS:
 *         t_api
 *
 * DESCRIPTION :- Handles an access request from a multiplexing protocol for a higher layer service 
 * (application/protocol) that is attempting to setup a connection to a remote bluetooth device. The
 * entry in the Security Database is retrieved based on search by both the multiplexProtocolId and 
 * multiplexChannelId. This database entry is passed to the ProcessRequest() function.
 *
 *************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_MultiplexOutgoingAccessRequest(u_int16 multiplexProtocolId, u_int32 multiplexChannelId, t_bdaddr address, u_int32 cbHandle, void (*callbackFunc)(u_int32 cbHandle, t_api status))
{
  struct st_t_mgr_secdbentry *secDbEntry;

  pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_MultiplexOutgoingAccessRequest called for multiplexProtocolId 0x%02X to address %s\n",multiplexProtocolId, prh_mgr_printBDAddress(address)));

  secDbEntry = GetServiceEntryByMultiplexInfo(multiplexProtocolId, multiplexChannelId);

  return ProcessAccessRequest(MGR_OUTGOING, secDbEntry, address, callbackFunc, cbHandle);
}




/*************************************************************************************************
 * FUNCTION :-  MGR_IncomingAccessRequest
 *
 * INPUTS:
 *         multiplexProtocolId: Id of the multiplexing protocol that is submitting the request.
 *         multiplexChannelId:  Id of the upper layer service that the incoming connection is directed to.
 *         address:             address of the remote device that originated the incoming connection attempt
 *         cbHandle:            a callback handle that is passed back when the access request is complete.
 *         callbackFunc:        the callback function that is invoked when the access request is complete.
 *
 * RETURNS:
 *         t_api
 *
 * DESCRIPTION :- Handles an access request from a multiplexing protocol for an incoming connection
 * attempt to a higher layer service (application/protocol). The entry in the Security Database is 
 * retrieved based on search by both the multiplexProtocolId and multiplexChannelId. This database 
 * entry is passed to the ProcessRequest() function. 
 *
 *************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_IncomingAccessRequest(u_int16 multiplexProtocolId, u_int32 multiplexChannelId, t_bdaddr address, u_int32 cbHandle, void (*callbackFunc)(u_int32 cbHandle, t_api status))
{

  struct st_t_mgr_secdbentry *secDbEntry;

  pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_IncomingAccessRequest called for multiplexProtocolId 0x%02X, multiplexChannelId 0x%02X from address %s\n",multiplexProtocolId, multiplexChannelId,prh_mgr_printBDAddress(address)));

  secDbEntry = GetServiceEntryByMultiplexInfo(multiplexProtocolId, multiplexChannelId);

  return ProcessAccessRequest(MGR_INCOMING, secDbEntry, address, callbackFunc, cbHandle);

}




/****************************************************************************************************
 * FUNCTION :- MGR_RegisterAuthorisationHandler
 *
 * INPUTS:
 *         authorisationCallback: callback function to handle authorisation requests from the manager
 *         
 * RETURNS:
 *         t_api
 *
 * DESCRIPTION :- Registers a handler for authorisation requests from the manager. If a handler is
 * already registered, an error is returned.
 *
 ************************************************************************************************** */

APIDECL1 t_api APIDECL2 MGR_RegisterAuthorisationHandler(void (*authorisationCallback)(t_BT_Authorisation *authorisationInfo))
{

  if(prh_callback_authorisation == NULL)
    {
      prh_callback_authorisation = authorisationCallback;
      return BT_NOERROR;
    }
  else
    return BT_NORESOURCES; 

}




/****************************************************************************************************
 * FUNCTION :- MGR_RegisterPINHandler
 *
 * INPUTS:
 *          pinCallback: callback function to handle PIN requests from the manager.
 * 
 * RETURNS:
 *          t_api
 *
 * DESCRIPTION :- Registers a handler for PIN requests from the host controller. If a handler is
 * already registered, an error is returned.
 *
 ************************************************************************************************** */

APIDECL1 t_api APIDECL2 MGR_RegisterPINHandler(void (*pinCallback)(t_bdaddr address))
{
  if(!prh_callback_pinRequest)
    {
      prh_callback_pinRequest = pinCallback;
      return BT_NOERROR;
    }
  
  return BT_NORESOURCES; 
  
}

APIDECL1 t_api APIDECL2 MGR_RegisterAcceptCnnHandle(void (*AcceptCnnCallBack)(t_bdaddr address))
{
	if( !prh_callback_AcceptConnectionRequest)
	{
		prh_callback_AcceptConnectionRequest = AcceptCnnCallBack;
		return BT_NOERROR;
	}
	return  BT_NORESOURCES;
	
}

APIDECL1 int  APIDECL2 MGR_AcceptConnRequest(t_bdaddr address, int result)
{
	int status = 0;
	extern u_int32 prh_mgr_linkFlags;
	
	if( result == 0)
	{
		status = HCI_AcceptConnectionRequest(address, (u_int8)(((prh_mgr_linkFlags&MGR_CONNREQ_ACL_SWITCH)==MGR_CONNREQ_ACL_SWITCH)?0x00:0x01)); /* 0x01 == do not request role switch */
	}
	else
	{
		 status =  HCI_RejectConnectionRequest(address, MGR_HCI_CONNREJECT_REASON); 
	}
	return status;
}




/****************************************************************************************************
 * FUNCTION :- MGR_DeregisterAuthorisationHandler
 *
 * INPUTS:
 *         (none)
 * 
 * RETURNS:
 *          t_api
 *
 * DESCRIPTION :- Deregisters the current authorisation handler
 *
 ************************************************************************************************** */

APIDECL1 t_api APIDECL2 MGR_DeregisterAuthorisationHandler(void (*authorisationCallback)(t_BT_Authorisation *authorisationInfo))
{
  if(prh_callback_authorisation)
    {
      prh_callback_authorisation = NULL;
      return BT_NOERROR;
    }
  else
    return BT_INVALIDPARAM; /* no handler currently registered */
}




/****************************************************************************************************
 * FUNCTION :- MGR_DeregisterPINHandler
 * 
 * INPUTS:
 *         (none)
 *          
 * RETURNS:
 *          t_api
 *
 * DESCRIPTION :- Deregisters the current handler for PIN code requests from the Host Controller
 *
 ************************************************************************************************** */

APIDECL1 t_api APIDECL2 MGR_DeregisterPINHandler(void (*pinCallback)(t_bdaddr address))
{
  if(prh_callback_pinRequest)
    {
      prh_callback_pinRequest = NULL;
      return BT_NOERROR;
    }
  else
    return BT_INVALIDPARAM;
}




/*************************************************************************************************************
 * FUNCTION :- MGR_AuthorisationResponse
 *
 * INPUTS:
 *          authorisationInfo: pointer to structure containing information about the authorisation request.
 *          allowConnection:   result of the authorisation (TRUE/FALSE).
 *          makeTrusted:       future trust level to be assigned to the device that is being authorised.
 *
 * RETURNS:
 *          t_api
 *
 * DESCRIPTION :- Function called (by authorisation handler) to return back the result of an authorisation 
 * request from the Manager. The authorisation is based on outgoing/incoming connection request to/from a 
 * service to/from a remote device. This function instructs the manager on whether to allow the connection
 * or not and is part of a previous security access request.
 *
 * If the authorisation was failed by the authorisation handler, the access request callback is retrieved from
 * the device database and invoked returning the authorisation failure.
 * If authorisation passes an no encryption is required (checked in the device database), the access request
 * callback is invoked returning success. If encryption is required, a HCI command is sent to the Host
 * Controller to enable encryption on the link and the result of this command is awaited.
 *
 *************************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_AuthorisationResponse(t_BT_Authorisation *authorisationInfo, u_int8 allowConnection, u_int8 makeTrusted)
{
    
    struct st_t_BT_ConnectionInfo *devDbEntry;
    struct st_t_mgr_secdbentry *secDbEntry;
    u_int8 invokeCallback = 1; /* default is to invoke access request callback */
    int i;
    t_api status = BT_NOERROR;;
    
    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_AuthorisationResponse called for address %s, allow=%d\n",prh_mgr_printBDAddress(authorisationInfo->address),allowConnection));
    
    devDbEntry = prh_mgr_find_deviceEntryByAddress(authorisationInfo->address);
    if(devDbEntry)
    {      
        /* Find match for authorisationInfo pointer, if not found, return invalid params error */
        for(i=0; i<MAX_OUTSTANDING_CALLBACKS; i++) {
            if(authorisationInfo == devDbEntry->authorisationInfo[i]) /* this is not a callback ptr but a struct ptr */ 
                break;
        }
        
        if(i<MAX_OUTSTANDING_CALLBACKS)
        {           
            
            if(TRUE == makeTrusted)
                devDbEntry->trustLevel = MGR_TRUSTED;
            else
                devDbEntry->trustLevel = MGR_UNTRUSTED;
            
            if(MGR_APPSERVICE == authorisationInfo->serviceType)
                secDbEntry = GetServiceEntryByAppName(authorisationInfo->service.serviceName); 
            else
                secDbEntry = GetServiceEntryByProtocolId(authorisationInfo->service.protocolId);
            
            
            /* Check result of Authorisation */
            if(TRUE != allowConnection)
                status = BT_AUTHORISATIONFAIL; /* Authorisation Failed */
            else
            {
                /* Check if encryption is required */
                pFree(devDbEntry->authorisationInfo[i]);
                devDbEntry->authorisationInfo[i] = NULL;
                if(devDbEntry->accessReqCallbackEncryptOn[i])
                {
                    if(SET_ENCRYPT_IDLE == devDbEntry->accessReqEncryptState)
                    {
                        devDbEntry->accessReqEncryptState = SET_ENCRYPT_PENDING;
                        if(BT_NOERROR == HCI_SetConnectionEncryption(devDbEntry->aclHandle, ENCRYPT_ENABLE_ON) )
                        {
                            invokeCallback = 0;
                        }
                        else 
                        {
                            devDbEntry->accessReqEncryptState = SET_ENCRYPT_IDLE;
                            status = BT_ENCRYPTFAIL; 
                            devDbEntry->accessReqCallbackEncryptOn[i] = 0; /* Reset this in callback entry */
                        }
                    } 
                    else /* Encryption is already being set to this device */
                        invokeCallback = 0;
                }
            }
        }
        else /* corresponding access request callback reference not found, inform application */
            return BT_INVALIDPARAM;
        
        if(invokeCallback)
        {
            callbackFuncPtr_t tempFuncPtr=devDbEntry->accessReqCallbackFunc[i];
            devDbEntry->accessReqCallbackFunc[i] = NULL;
            if (!--devDbEntry->outstandingCallbacks)
                devDbEntry->recordState&=~MGR_DEVDB_CALLBACK_PENDING;            
            tempFuncPtr(devDbEntry->accessReqCallbackHandle[i], status); 
        }
    }
    else
    {
        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"MGR: Unknown BD Address passed in Authorisation Response\n\n"));
        return BT_INVALIDPARAM;
    }
    
    return BT_NOERROR;
}




/******************************************************************************************************
 * FUNCTION :- MGR_PINResponse
 *
 * INPUTS:
 *          address:     address of the device that the PIN code request is for.
 *          pinSupplied: indicates whether PIN handler successfully retrieved a PIN or not (TRUE/FALSE).
 *          pinCode:     pointer to PIN code returned by handler (1-16 bytes long).
 *          pinLen:      length of PIN being returned by PIN handler.
 *
 * RETURNS:
 *          t_api
 *
 * DESCRIPTION :- Invoked by an external PIN handler to return the result of a PIN code request
 * from the manager. If 'pinSupplied' is set to FALSE a negative PIN code request reply is sent to the
 * Host Controller. Otherwise, the PIN code an length is sent to the Host Controller in a positive PIN
 * code request reply.
 *
 *******************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_PINResponse(t_bdaddr address, u_int8 pinSupplied, u_int8 *pinCode, u_int8 pinLen)
{
    u_int8 status;

    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_PINResponse called for address %s, pinSupplied=%d\n",prh_mgr_printBDAddress(address),pinSupplied));

    if (pinLen>16)
        return BT_INVALIDPARAM;

  if(FALSE == pinSupplied )
    status = HCI_PINCodeRequestNegativeReply(address);
  else
    status = HCI_PINCodeRequestReply(address, pinLen, pinCode);
    
  return status;

}


/****************************************************************************************************
 * FUNCTION :- MGR_SetSecurityMode
 *
 * INPUTS:  mode: new setting for the security mode of the Host (MGR_NOSECURITY, MGR_SERVICELEVEL
 *                or MGR_LINKLEVEL).
 * 
 * RETURNS:
 *          t_api
 *
 * DESCRIPTION :- Sets a new security mode for the host. This mode determines where 
 * authentication is required in channel/link establishment.
 *
 ************************************************************************************************** */

APIDECL1 t_api APIDECL2 MGR_SetSecurityMode(u_int8 mode, void (*callbackFunc)(t_api status))
{
	u_int8 status;
	
    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"MGR_SetSecurityMode called, new mode=%d\n",mode));

    if(prh_security_mode_pending!=0) 
		return BT_RETRY;
	
	if(mode == 0 || mode > MGR_LINKLEVEL)
       return BT_INVALIDPARAM;
    
    /* 
        a callback parameter is not required if the application does 
        not want to be informed when the mode change is complete.
    */

	setSecurityModeCallback = callbackFunc;
	prh_security_mode_pending = mode;
	
	if(MGR_LINKLEVEL == mode)
    {
		status = HCI_WriteAuthenticationEnable(0x01);
        /* 
            We could enable encryption here also, but this has implications in piconet
            scenarios and for broadcast traffic. Because of this we leave the encryption 
            decision up to the application on a link-by-link basis via HCI_EncryptionRequested()
            or on a global basis via HCI_WriteEncryptionMode(). The encryption setup will 
            complete without PIN code or link key requests if authentication has succeeded.
            -MM
        */

    }
	else /* MGR_NOSECURITY or MGR_SERVICELEVEL */
    {           
        status=HCI_WriteAuthenticationEnable(0x00);
    }  
	
	if(status == BT_NOERROR)
		return BT_PENDING;
	else
	{
		/* Clear up after failure case */
	    setSecurityModeCallback = pNULL;
	    prh_security_mode_pending = 0;
		return status;  
	}
}




/****************************************************************************************************
 * FUNCTION :- MGR_GetSecurityMode
 *
 * INPUTS:
 *          (none)
 * 
 * RETURNS:
 *          u_int8: the current security mode of the Host (MGR_NOSECURITY, MGR_SERVICELEVEL
 *                  or MGR_LINKLEVEL).
 *
 * DESCRIPTION :- Returns the current security mode of the Host.This mode determines where 
 * authentication is required in channel/link establishment.
 *
 ************************************************************************************************** */

APIDECL1 u_int8 APIDECL2 MGR_GetSecurityMode(void)
{
  return prh_security_mode;
}




/****************************************************************************************************
 * FUNCTION :- MGR_SetPairingMode
 *
 * INPUTS:
 *          mode: new setting for the pairing mode of the Host (MGR_PAIRABLE or MGR_NONPAIRABLE).
 *
 * RETURNS:
 *          t_api
 *
 *
 * DESCRIPTION :- Sets the pairing mode for the Host. This determines whether the Host will accept
 * a pairing attempt from a remote device or not.
 *
 ************************************************************************************************** */

APIDECL1 t_api APIDECL2 MGR_SetPairingMode(u_int8 mode)
{
  if(mode == 0 || mode > MGR_NONPAIRABLE)
       return BT_INVALIDPARAM;

  prh_pairing_mode = mode;
  return BT_NOERROR;
}




/**********************************************************************************************************
 * FUNCTION :- MGR_GetPairingMode
 *
 * INPUTS:
 *          (none)
 *
 * RETURNS:
 *           u_int8: the current pairing mode of the Host (MGR_PAIRABLE or MGR_NONPAIRABLE).
 *
 *
 * DESCRIPTION :- Returns the current pairing mode of the Host. This determines whether the Host will accept
 * a pairing attempt from a remote device or not.
 *
 **********************************************************************************************************/

APIDECL1 u_int8 APIDECL2 MGR_GetPairingMode(void)
{
  return prh_pairing_mode;
}




/**********************************************************************************************************
 * FUNCTION :- MGR_SetLocalPINType
 *
 * INPUTS:
 *          pinType:      PIN type for the Host Controller (MGR_FIXEDPIN/MGR_VARIABLEPIN).
 *          callbackFunc: callback function that is invoked when the set PIN type operatio is completed.
 *
 * RETURNS:
 *          t_api
 *
 * DESCRIPTION :- Writes to the Host Controller whether the Host supports variable PIN codes
 * or only a fixed PIN code. This affects the behaviour of the HC during pairing. If type is
 * PIN_FIXED, the local device will not be able to pair with another device with a fixed PIN.
 *
 *********************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_SetLocalPINType(u_int8 pinType, void (*callbackFunc)(t_api status))
{
  t_api status;

    if (!callbackFunc)
        return BT_INVALIDPARAM;

  if(prh_callback_set_pinType)
      return BT_RETRY;
  else
    {
      prh_callback_set_pinType = callbackFunc;
      if( BT_NOERROR != (status = HCI_WritePINType(pinType)) )
        {
          prh_callback_set_pinType = NULL;
          return status;
        }
    }

  return BT_PENDING;
}




/************************************************************************************************
 * FUNCTION :- MGR_GetLocalPINType
 *
 * INPUTS:
 *         callbackFunc: callback function that is invoked when the current PIN type setting has
 *                       been retrieved from the Host Controller.
 *
 * DESCRIPTION :- Invoked to retrieve the current PIN type assumed by the Host. A HCI command is
 * issued and when the result of this received, the callback is invoked, returning the last
 * PIN type that was successfully written to the Host Controller.
 *
 ************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_GetLocalPINType(void (*callbackFunc)(t_api status, u_int8 pinType))
{
  t_api status;

    if (!callbackFunc)
        return BT_INVALIDPARAM;

  if(prh_callback_get_pinType)
    return BT_RETRY;
  else
    {
      prh_callback_get_pinType = callbackFunc;
      if( BT_NOERROR != (status = HCI_ReadPINType()) )
        {
          prh_callback_get_pinType = NULL;
          return status;
        }
    }

  return BT_PENDING;

}




/*************************************************************************************************
 * FUNCTION :- MGR_FlushSecurityDatabase
 *
 * INPUTS:
 *          flags: reserved for future use.
 *
 * RETURNS:
 *          
 *          t_api
 *
 * DESCRIPTION :- Removes all the entries from the Security Database.
 *                
 * 
 ************************************************************************************************/

APIDECL1 t_api APIDECL2 MGR_FlushSecurityDatabase(u_int32 flags)
{

  struct st_t_mgr_secdbentry *currentServiceEntry = prh_service_list_head;

  while(currentServiceEntry)
    {
      prh_service_list_head = prh_service_list_head->nextService;
      if(MGR_APPSERVICE == currentServiceEntry->serviceType)
				pFree(currentServiceEntry->service.name);
      pFree(currentServiceEntry);
      currentServiceEntry = prh_service_list_head;

    }
  prh_service_list_tail = prh_service_list_head;

  return BT_NOERROR;
}




/* NON-API FUNCTIONS */


/*************************************************************************************************
 * FUNCTION :- prh_mgr_get_next_serviceEntry
 *
 * INPUTS:
 *          secDbEntry: pointer to the current entry of the Security Database.
 *       
 * RETURNS:
 *          struct st_t_mgr_secdbentry: pointer to next entry in the Security Database.
 *
 * DESCRIPTION :- Takes a pointer to an entry in the Security Database and returns the next entry.
 * If NULL is passed, the head of the Security Database is returned.
 *
 ************************************************************************************************/

struct st_t_mgr_secdbentry * prh_mgr_get_next_serviceEntry(struct st_t_mgr_secdbentry *secDbEntry)
{
  if(NULL == secDbEntry)
    return prh_service_list_head;
  else
    return secDbEntry->nextService;

}




/*************************************************************************************************
 * FUNCTION :- prh_mgr_handle_security_hciEvent
 *
 * INPUTS:
 *          pdu: pointer to a HCI event packet.
 *
 * RETURNS:
 *          (none)
 *
 * DESCRIPTION :- Handles Security related HCI Events. It handles the following events:
 *     Authentication Complete
 *     Encryption Change
 *     Link Key Notification
 *     Link Key Request
 *     PIN Code Request    
 *
 *************************************************************************************************/

void prh_mgr_handle_security_hciEvent(u_int8 *pdu)
{
  struct st_t_BT_ConnectionInfo *currentDevice;
  u_int8 linkType;
  int i=0;
  t_api status = BT_NOERROR;
  u_int8 invokeCallback;
  u_int8 eventType;
  u_int8 eventStatus;
  u_int16 eventHandle=0;
  t_bdaddr eventBDAddress;
    
  /* Decode some common parameters for use later if applicable */
  eventType = pdu[0];
  eventStatus = pdu[2];
  if (pdu[1]>2)
    eventHandle = BT_READ_LE_UINT16(pdu+3);
  if (pdu[1]>5)
      pMemcpy(&eventBDAddress.bytes,pdu+2,6);
    
  switch(eventType)
    {
    case HCI_evCONNECTIONCOMPLETE:
	  if(eventStatus == BT_NOERROR)
	  {
		   /* For this event, have eventHandle in pdu bytes 3,4, address starts in pdu byte 5 */
		   pMemcpy(&eventBDAddress.bytes,pdu+5,6);
		   currentDevice = prh_mgr_return_deviceEntryByAddress(eventBDAddress);
           if(currentDevice)
		   {
		      if(currentDevice->bondingState == BOND_CONN_PENDING)
			  {
		          currentDevice->bondingState = BOND_AUTH_PENDING;
                  pendingSecHandle = eventHandle;
                  status = HCI_AuthenticationRequested(eventHandle);
			  }
		   }
	  }
      break;
	  

    case HCI_evAUTHENTICATIONCOMPLETE:
      currentDevice = prh_mgr_get_deviceEntryByHandle(eventHandle, &linkType);
      if(currentDevice)
        {
		  if(BT_NOERROR != eventStatus)
		  {
			  if (prh_mgr_auth_supplied_link_key) {
				  /* 
					if authentication failed and we had supplied the link key, the
					supplied (stored) link key should be deleted. This is only
					best effort - no error checking is required here. We then
					restart the authentication mechanism here.    -MM
				  */            
				  prh_mgr_auth_supplied_link_key=0;
				  MGR_DeleteLinkKey(prh_mgr_auth_supplied_link_key_addr);
				  HCI_DeleteStoredLinkKey(prh_mgr_auth_supplied_link_key_addr,0);                      
				  HCI_AuthenticationRequested(eventHandle);         
				  return;
			  }
		  }

          prh_mgr_auth_supplied_link_key=0;
          currentDevice->accessReqAuthState = AUTH_IDLE;
          while(currentDevice->accessReqCallbackFunc[i] && i<MAX_OUTSTANDING_CALLBACKS)
            {
              invokeCallback = 1;
                
              if(BT_NOERROR != eventStatus)
                {
                  status = BT_AUTHENTICATIONFAIL;
				  invokeCallback = 1;
                }
              else
                {
                    
                  if(currentDevice->authorisationInfo[i])
                    {
                      if(prh_callback_authorisation)
                        {
                          invokeCallback = 0;
                          prh_callback_authorisation(currentDevice->authorisationInfo[i]);
                        }
                      else
                        status = BT_AUTHORISATIONFAIL;
                    }
                  else
                    {/* If no Authorisation is required, check encryption bit */
                      if(currentDevice->accessReqCallbackEncryptOn[i])
                        {
                          if(SET_ENCRYPT_IDLE == currentDevice->accessReqEncryptState)
                            {
                              currentDevice->accessReqEncryptState = SET_ENCRYPT_PENDING;
                              if(BT_NOERROR == HCI_SetConnectionEncryption(currentDevice->aclHandle, ENCRYPT_ENABLE_ON) )
                                {
                                  invokeCallback = 0;
                                }
                              else 
                                {
                                  currentDevice->accessReqEncryptState = SET_ENCRYPT_IDLE;
                                  status = BT_ENCRYPTFAIL; 
                                  currentDevice->accessReqCallbackEncryptOn[i] = 0; /* Reset this in callback entry */
                                }
                            } 
                          else
                            invokeCallback = 0;
                        }
                      else 
                        {
                          if(eventStatus)
                            status = BT_AUTHENTICATIONFAIL;
                        }
                    }
                }
                
                
                /* Pass the Result via the callback and reset the callback entry */
              if(invokeCallback)
                {
                  currentDevice->accessReqCallbackFunc[i](currentDevice->accessReqCallbackHandle[i], status);
                  currentDevice->accessReqCallbackFunc[i] = NULL;
                if (!--currentDevice->outstandingCallbacks)
                    currentDevice->recordState&=~MGR_DEVDB_CALLBACK_PENDING;            
                }
              i++;
            }
            
          /* Now check if bonding was awaiting an Authentication Complete */
          i=0;
          if(eventStatus)
            status = BT_SECFAIL;
          else
            status = BT_NOERROR;

		while(currentDevice->bondingCallbackFunc[i] && i<MAX_OUTSTANDING_CALLBACKS)
        {
          if(BOND_AUTH_PENDING == currentDevice->bondingState)
		  {
			currentDevice->bondingCallbackFunc[i](currentDevice->bdAddress, status);
		  }
		  currentDevice->bondingCallbackFunc[i] = NULL;
          i++;
        }
		if(BOND_AUTH_PENDING == currentDevice->bondingState)
			currentDevice->bondingState = BOND_INACTIVE;
       }
      else
        pDebugPrintfEX((pLOGERROR,pLOGMANAGER,"MGR: Unknown handle passed in HCI_AuthenticationComplete Event\n\n"));
      break;
        
    case HCI_evENCRYPTIONCHANGE:
      /* The pending callbacks are only interested in the event when Encryption is being set ON */
      if(ENCRYPT_ENABLE_ON == pdu[5])
        {
          currentDevice = prh_mgr_get_deviceEntryByHandle(eventHandle, &linkType);
          currentDevice->accessReqEncryptState = SET_ENCRYPT_IDLE;
          if(eventStatus)
            status = BT_ENCRYPTFAIL;
          while(currentDevice->accessReqCallbackFunc[i] && i<MAX_OUTSTANDING_CALLBACKS)
            {            
                /* Pass the Result via the callback and reset the callback entry */
              currentDevice->accessReqCallbackFunc[i](currentDevice->accessReqCallbackHandle[i], status);
              currentDevice->accessReqCallbackFunc[i] = NULL;
                if (!--currentDevice->outstandingCallbacks)
                    currentDevice->recordState&=~MGR_DEVDB_CALLBACK_PENDING;            
              currentDevice->accessReqCallbackEncryptOn[i] = 0;
                
              i++;  
            }
      } else {
          /* need to handle failure case here...  -MM */
      }
      
        
      break;        
        
    case HCI_evLINKKEYNOTIFICATION:
		
		if(prh_mgr_HCIversion >= PRH_HCI_VERSION_BLUETOOTHSPEC11)	
			prh_mgr_updateLinkKeyEntry(eventBDAddress, pdu+8, pdu[24]);
		else
			prh_mgr_updateLinkKeyEntry(eventBDAddress, pdu+8, 0);
		break;
        
    case HCI_evLINKKEYREQUEST:
	
	    currentDevice = prh_mgr_find_deviceEntryByAddress(eventBDAddress);
        if(currentDevice && currentDevice->linkKey) {
            /* 
                we must track the supplied link key since if authentication fails
                the supplied (stored) link key should be deleted.  -MM
            */            
            prh_mgr_auth_supplied_link_key_addr=eventBDAddress;
            prh_mgr_auth_supplied_link_key=1;
            HCI_LinkKeyRequestReply(eventBDAddress, currentDevice->linkKey);
        } else {
            HCI_LinkKeyRequestNegativeReply(eventBDAddress);
        }
        break;

    case HCI_evPINCODEREQUEST:
      if(MGR_PAIRABLE == prh_pairing_mode) /* see GAP spec. */
        {
          currentDevice = prh_mgr_find_deviceEntryByAddress(eventBDAddress);

          if(!currentDevice)
            {

#if PRH_MGR_DEFAULTPIN_ALLZERO
              pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Got PIN code request for device %s and no db entry - sending all-zero pin (since PRH_MGR_DEFAULTPIN_ALLZERO is set)\n",prh_mgr_printBDAddress(eventBDAddress)));
              HCI_PINCodeRequestReply(eventBDAddress, MAX_PIN_LEN, prh_allzero_pin);
#else 
              if(prh_callback_pinRequest) {
                  pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Got PIN code request for device %s and no db entry - calling app callback\n",prh_mgr_printBDAddress(eventBDAddress)));
				  prh_callback_pinRequest(eventBDAddress);
              } else { /* if no handler exists, send negative reply */
                    pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Got PIN code request for device %s and no db entry and no callback set - sending negative reply\n",prh_mgr_printBDAddress(eventBDAddress)));
                    HCI_PINCodeRequestNegativeReply(eventBDAddress);       
              }
#endif
              break;
            }
            
          switch(currentDevice->pinType)
            {
            case MGR_ALLZEROPIN:
#if PRH_MGR_DEFAULTPIN_ALLZERO
              pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Got PIN code request for device %s and db entry pinType is MGR_ALLZEROPIN (default since PRH_MGR_DEFAULTPIN_ALLZERO is set)- sending all-zero pin\n",prh_mgr_printBDAddress(eventBDAddress)));
#else
              pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Got PIN code request for device %s and db entry pinType is MGR_ALLZEROPIN (explicitly set by application) - sending all-zero pin\n",prh_mgr_printBDAddress(eventBDAddress)));
#endif
              HCI_PINCodeRequestReply(eventBDAddress, MAX_PIN_LEN, prh_allzero_pin);
              break;
                
            case MGR_PRESETPIN:
              pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Got PIN code request for device %s and db entry pinType is MGR_PRESETPIN (PRH_MGR_DEFAULTPIN_ALLZERO is set)- sending preset pin (%d bytes)\n",prh_mgr_printBDAddress(eventBDAddress),currentDevice->pinLength));
              HCI_PINCodeRequestReply(eventBDAddress, currentDevice->pinLength, currentDevice->pinCode);
              break;
                
            case MGR_USERREQPIN:
              if(prh_callback_pinRequest) {
#if PRH_MGR_DEFAULTPIN_ALLZERO
                  pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Got PIN code request for device %s and db entry pinType is MGR_USERREQPIN (explicitly set by application)- calling app callback\n",prh_mgr_printBDAddress(eventBDAddress)));
#else
                  pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Got PIN code request for device %s and db entry pinType is MGR_USERREQPIN (default setting) -  calling app callback\n",prh_mgr_printBDAddress(eventBDAddress)));
#endif
                prh_callback_pinRequest(eventBDAddress);
              } else {
#if PRH_MGR_DEFAULTPIN_ALLZERO
                  pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Got PIN code request for device %s and db entry pinType is MGR_USERREQPIN (explicitly set by application) - no app callback set - sending negative reply\n",prh_mgr_printBDAddress(eventBDAddress)));
#else
                  pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Got PIN code request for device %s and db entry pinType is MGR_USERREQPIN (default setting) - no app callback set - sending negative reply\n",prh_mgr_printBDAddress(eventBDAddress)));
#endif
                HCI_PINCodeRequestNegativeReply(eventBDAddress);       
              }
              break;
                
            default: /* this case should not occur */
                HCI_PINCodeRequestNegativeReply(eventBDAddress);       
              break;
                
            }      
        }
      else {/* MGR_NONPAIRABLE: Host will not accept any pairing requests */
            pDebugPrintfEX((pLOGNOTICE,pLOGMANAGER,"Got PIN code request for device %s but MGR_NONPAIRABLE set - sending negative reply\n",prh_mgr_printBDAddress(eventBDAddress)));
            HCI_PINCodeRequestNegativeReply(eventBDAddress);
      }
      break;

	case HCI_evCOMMANDSTATUS :
		{
			u_int16 cmdOpcode;

			cmdOpcode = BT_READ_LE_UINT16(pdu+4);
			if (eventStatus)
			{
				switch(cmdOpcode)
				{
				case HCI_cmdAUTHENTICATIONREQUESTED :
					currentDevice = prh_mgr_get_deviceEntryByHandle(pendingSecHandle, &linkType);
					if(currentDevice)
					{
						currentDevice->accessReqAuthState = AUTH_IDLE;
                        i=0;
						while(currentDevice->accessReqCallbackFunc[i] && i<MAX_OUTSTANDING_CALLBACKS)
						{
							currentDevice->accessReqCallbackFunc[i](currentDevice->accessReqCallbackHandle[i], BT_AUTHENTICATIONFAIL);
							currentDevice->accessReqCallbackFunc[i] = NULL;
							if (!--currentDevice->outstandingCallbacks)
								currentDevice->recordState&=~MGR_DEVDB_CALLBACK_PENDING;            
						    i++;
						}
						
						/* Now check if bonding was awaiting an Authentication Complete */
						i=0;						
						while(currentDevice->bondingCallbackFunc[i] && i<MAX_OUTSTANDING_CALLBACKS)
						{
							if(BOND_AUTH_PENDING == currentDevice->bondingState)
							{
								currentDevice->bondingCallbackFunc[i](currentDevice->bdAddress, BT_SECFAIL); 
							}
							currentDevice->bondingCallbackFunc[i] = NULL;
							i++;
						}
						if(BOND_AUTH_PENDING == currentDevice->bondingState)
							currentDevice->bondingState = BOND_INACTIVE;
					}
					break;

				case HCI_cmdSETCONNECTIONENCRYPTION :
                    i=0;
					/* The pending callbacks are only interested in the event when Encryption is being set ON */
					currentDevice = prh_mgr_get_deviceEntryByHandle(pendingSecHandle, &linkType);
					if(currentDevice!=NULL)
					{
						while(currentDevice->accessReqCallbackFunc[i] && i<MAX_OUTSTANDING_CALLBACKS)
						{            
							/* Pass the Result via the callback and reset the callback entry */
							currentDevice->accessReqCallbackFunc[i](currentDevice->accessReqCallbackHandle[i], BT_ENCRYPTFAIL);
							currentDevice->accessReqCallbackFunc[i] = NULL;
							if (!--currentDevice->outstandingCallbacks)
								currentDevice->recordState&=~MGR_DEVDB_CALLBACK_PENDING;            
							currentDevice->accessReqCallbackEncryptOn[i] = 0;
							i++;  
						}
						/* Reset the encryption state for this device */
						currentDevice->accessReqEncryptState = SET_ENCRYPT_IDLE;
					}
					break;

                } // switch 
			} // if (eventStatus)

		}
		break; // case HCI_evCOMMANDSTATUS
		
    default:
      break;
    }
}






/****************************************************************************************************
 * FUNCTION :- prh_mgr_callback_bondingInquiry
 *
 * INPUTS:
 *          deviceInfo: pointer to an entry in the Device Database.
 *          status:     status of the Inquiry Result or Inquiry Complete event.
 *
 * RETURNS:
 *          (none)
 *
 * DESCRIPTION :- This callback function is invoked for every Inquiry Result or Inquiry Complete event
 * that occurs as a result of an Inquiry invoked for the Bonding procedure ( see InitBonding() ).
 * A pointer to an entry in the Device Database is passed to this function. The relevant state is 
 * updated in this Device entry. If this function was invoked from an Inquiry Complete event, the
 * Device Database is searched for all entries that are in a bonding pending state (BOND_INQUIRY_PENDING
 * or BOND_INQUIRY_COMPLETE), and the Bonding procedure for each Device entry.
 *
 ************************************************************************************************** */

 void prh_mgr_callback_bondingInquiry(struct st_t_BT_DeviceEntry *deviceInfo, t_api status)
 {
     struct st_t_BT_ConnectionInfo *devDbEntry; 
     
     if(deviceInfo)
     {
         devDbEntry=prh_mgr_find_deviceEntryByAddress(deviceInfo->address);
         if (devDbEntry) {
             if(BOND_INQUIRY_PENDING == devDbEntry->bondingState)
                 devDbEntry->bondingState = BOND_INQUIRY_COMPLETE;      
         }
     }
     else /* Inquiry is complete, look for every device entry that has a pending/complete bonding procedure */
     {
         devDbEntry = prh_device_list_head;
         while(devDbEntry)
         {
			if(status == BT_NOERROR)
            {
                /* If inquiry never found this device StartBonding() will try connection setup 
                   with default paging parameters */
                if(BOND_INQUIRY_PENDING == devDbEntry->bondingState)
                    devDbEntry->bondingState = BOND_INQUIRY_COMPLETE;
             
                if(BOND_INQUIRY_COMPLETE == devDbEntry->bondingState)
                    StartBonding(devDbEntry, NULL);
			}
			else
			{
				u_int8 i = 0;
               
                while(devDbEntry->bondingCallbackFunc[i] && i<MAX_OUTSTANDING_CALLBACKS)
				{
                    if(BOND_INQUIRY_PENDING == devDbEntry->bondingState)
					{
			            devDbEntry->bondingCallbackFunc[i](devDbEntry->bdAddress, status);
					}
		                devDbEntry->bondingCallbackFunc[i] = NULL;
                    i++;
				}
		        if(BOND_INQUIRY_PENDING == devDbEntry->bondingState)
			        devDbEntry->bondingState = BOND_INACTIVE;
			}

			devDbEntry = devDbEntry->nextDevice;
         }
         
     }
     
 }
 



/****************************************************************************************************
 * FUNCTION :- prh_mgr_callback_bondingACL
 *
 * INPUTS:   transactionId: transactionId for an ACL setup request.
 *           status:        result of the ACL setup request.
 *          
 * RETURNS:
 *           (none)
 *
 * DESCRIPTION :- This callback function is invoked when an ACL setup previously initiated as part of
 * bonding is completed. If Bonding was previously initiated while an ACL was already being setup 
 * (bondingState is BOND_CONN_PENDING), the link key is removed from the Host and HC and a 
 * HCI Authentication Requested command is issued. If the bondingState is BOND_AUTH_CONN_PENDING, then
 * the ACL setup incorporated the pairing required, therefore bonding is complete; the result of this
 * pairing is passed back through the bonding callback. In addition, the ACL is disconnected.
 *
 ************************************************************************************************** */
 
 void prh_mgr_callback_bondingACL(tid_t transactionId, t_api status)
 {
     
     u_int8 invokeCallback = 1;
     struct st_t_BT_ConnectionInfo *currentDevice;
     u_int8 i=0;
     t_api bondingStatus = BT_NOERROR;
     
     currentDevice = (struct st_t_BT_ConnectionInfo *)transactionId;
     if(currentDevice)
     {
         if(BT_NOERROR == status)
         {
             if(BOND_CONN_PENDING == currentDevice->bondingState)
             {
                 invokeCallback = 0;
                 /* delete the link key */
                 if (currentDevice->linkKey)
                    pFree(currentDevice->linkKey);
                 currentDevice->linkKey=NULL;
                 currentDevice->recordState&=~MGR_DEVDB_LINKKEY_STORED;
                 HCI_DeleteStoredLinkKey(currentDevice->bdAddress, 0x00);
                 currentDevice->bondingState = BOND_AUTH_PENDING;
                 pendingSecHandle=currentDevice->aclHandle;
                 HCI_AuthenticationRequested(currentDevice->aclHandle);
				  prh_mgr_auth_supplied_link_key=0;
             }
             else if(BOND_AUTH_CONN_PENDING == currentDevice->bondingState)
             { 
                 currentDevice->bondingState = BOND_RELEASE_CONN_PENDING;  
                 /* prh_mgr_releaseACL must check if anyone else is awaiting the connection*/
                 invokeCallback = 0; /* Don't invoke callback until DISCONNECTION COMPLETE event is received */
		   pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"it is about to release the acl...............\n"));
                 prh_mgr_releaseACL(currentDevice->aclHandle, 0x13, prh_mgr_callback_bondingReleaseACL);                 
             }
         }
         else /* No need to release the connection because conn setup failed */
         {
             currentDevice->bondingState = BOND_INACTIVE;
             bondingStatus = BT_SECFAIL;
         }
         
         
         if(invokeCallback) {
             void (*tempFuncPtr)(t_bdaddr address, t_api status_t);
             do {
                 prh_mgr_get_callback(&i,(void **)currentDevice->bondingCallbackFunc,(void **)&tempFuncPtr,currentDevice);
                 if (tempFuncPtr) {
                     tempFuncPtr(currentDevice-> bdAddress, bondingStatus); 
                 }
             } while (tempFuncPtr);
         }      
     }
 }


/*************************************************************************************************
* FUNCTION :- prh_mgr_callback_bondingReleaseACL
*
* DESCRIPTION :- This is the callback for release ACL that has brought up for bonding
*                 
************************************************************************************************/
void prh_mgr_callback_bondingReleaseACL(struct st_t_BT_ConnectionInfo *dbEntry, t_api status)
{
	u_int8 i;
       pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_callback_bondingReleaseACL\n"));

	if(dbEntry)
	{
		if(BOND_RELEASE_CONN_PENDING == dbEntry->bondingState)
		{
			dbEntry->bondingState = BOND_INACTIVE;
            {
                void (*tempFuncPtr)(t_bdaddr address, t_api status_t)=NULL;
                do {
                    prh_mgr_get_callback(&i,(void **)dbEntry->bondingCallbackFunc,(void **)&tempFuncPtr,dbEntry);
                    pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"prh_mgr_callback_bondingReleaseACL 2\n"));
                    if (tempFuncPtr) 
                        tempFuncPtr(dbEntry->bdAddress, status); 
                } while (tempFuncPtr);
            }
		}
	}
}

APIDECL1 int APIDECL2 MGR_SetBondingCallBack(t_bdaddr address,  void (*callbackfunc)(t_bdaddr address, t_api status))
{
	  u_int8 i= 0;
	  struct st_t_BT_ConnectionInfo *currentDevice;
	  
	  currentDevice = prh_mgr_find_deviceEntryByAddress( address);
	  if(!currentDevice)
	  	return 1;
	  if( callbackfunc )
	  	    prh_mgr_set_callback(&i,(void **)currentDevice->bondingCallbackFunc,(void *)callbackfunc,currentDevice);

         prh_mgr_set_callback(&i,(void **)currentDevice->aclReleaseCallbackFunc,(void *)prh_mgr_callback_bondingReleaseACL, currentDevice);

         if(currentDevice->linkKey != NULL)
	 {
	 	pFree(currentDevice->linkKey);
		currentDevice->linkKey= NULL;		
         }
	  currentDevice->bondingState = BOND_AUTH_CONN_PENDING;

	  return 0;
}

//CuiJian add 2009/07/23
APIDECL1 int APIDECL2 MGR_SetACLCallBack(t_bdaddr address, int index , void (*callbackfunc)(struct st_t_BT_ConnectionInfo *dbEntry, t_api status_t))
{
	  u_int8 i= 0;
	  
	  struct st_t_BT_ConnectionInfo *currentDevice;
	  
	  currentDevice = prh_mgr_find_deviceEntryByAddress( address);
	  if(!currentDevice)
	  	return 1;
	  if( callbackfunc )
	  	{
		  	switch(index)
		  	{
		  	case 0:
	         return(prh_mgr_set_callback(&i,(void **)currentDevice->aclReleaseCallbackFunc,(void *)callbackfunc, currentDevice));

			 break;	
			case 1:
	         return(prh_mgr_set_callback(&i,(void **)currentDevice->aclCallbackFunc,(void *)callbackfunc, currentDevice));

			 break;	
			}
	  	}

	  return 0;
}


/* LOCAL FUNCTIONS */


/****************************************************************************************************
 * FUNCTION :- StartBonding
 *
 * INPUTS:
 *          currentDevice: pointer to an entry in the device entry.
 *          callbackFunc:  callback function to be invoked when the Bonding procedure has been completed.
 *
 * RETURNS:
 *          t_api
 *
 * DESCRIPTION :- Initiates bonding with a remote bluetooth device. Any link key for this remote device
 * is removed from the Host and HC. If an ACL already exists to this, a HCI Authentication Requested command
 * is issued and the result awaited. If no ACL exists to this device, authentication is enable locally
 * and an ACL setup is initiated. If the local device is in the middle of setting up an ACL to the remote
 * device, a bonding State is set to BOND_CONN_PENDING and the Connection Complete event is awaited.
 *
 ************************************************************************************************** */

t_api StartBonding(struct st_t_BT_ConnectionInfo *currentDevice, void (*callbackFunc)(t_bdaddr bdaddress, t_api status))
{
  u_int8 i;
  t_api status=0;

  extern u_int8 prh_mgr_discoveryInProgress;

  pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"StartBonding called for address %s\n",prh_mgr_printBDAddress(currentDevice->bdAddress)));


  if(currentDevice)
    {
	  /* If Bond procedure in progress (one of the pending states,
	     cannot add a new callback so retry later 
	  */
	  if((BOND_INQUIRY_PENDING == currentDevice->bondingState) ||
         (BOND_AUTH_PENDING == currentDevice->bondingState) ||
		 (BOND_CONN_PENDING == currentDevice->bondingState) ||
		 (BOND_AUTH_CONN_PENDING == currentDevice->bondingState) ||
		 (BOND_RELEASE_CONN_PENDING== currentDevice->bondingState))
	  {
	      return BT_RETRY;
	  }

	  /* If there is an inquiry in progress, return BT_RETRY */
	  if(prh_mgr_discoveryInProgress== TRUE)
         return BT_RETRY;

      /* If Inquiry was required for Bonding, the callbacks have already been setup */
       if(BOND_INQUIRY_COMPLETE != currentDevice->bondingState) {
          status=prh_mgr_set_callback(&i,(void **)currentDevice->bondingCallbackFunc,(void *)callbackFunc,currentDevice);
          if (status)
              return status;
        }         

      /* If Bonding (or Inquiry) is currently in process with this device, don't start a new bonding procedure */
      if(BOND_INACTIVE == currentDevice->bondingState || BOND_INQUIRY_COMPLETE == currentDevice->bondingState)
        {
			if(currentDevice->linkKey)
			{
				pFree(currentDevice->linkKey);
				currentDevice->linkKey=NULL;
			}
          status = HCI_DeleteStoredLinkKey(currentDevice->bdAddress, 0x00);
		  prh_mgr_auth_supplied_link_key=0;
          if(status != BT_NOERROR)
          {
			   void (*tempFunc)(t_bdaddr address, t_api status_t);

			  /* Before returning delete the stored callback */
			  /* One way of doing this is to call prh_mgr_get_callback(),
			     this retrieves and deletes the callback. We won't actually
				 use the callback.
			  */
			  prh_mgr_get_callback(&i,(void **)currentDevice->bondingCallbackFunc,(void **)&tempFunc,currentDevice);
              return status;
		  }

          switch(currentDevice->aclState)
            {
            case CONN_ACTIVE:
              currentDevice->bondingState = BOND_AUTH_PENDING;
                pendingSecHandle = currentDevice->aclHandle;
              status = HCI_AuthenticationRequested(currentDevice->aclHandle);
              break;
              
            case CONN_PENDING:
              /* Wait for ConnectionComplete, then send HCI_AuthenticationRequested */
              currentDevice->bondingState = BOND_CONN_PENDING;                  
              break;
              
            case CONN_CLOSED:
              status = HCI_WriteAuthenticationEnable(0x01);
              if(status != BT_NOERROR)
                  break;
              currentDevice->bondingState = BOND_AUTH_CONN_PENDING;
              status = prh_mgr_createACL(currentDevice->bdAddress, 0, &tmpHandle, prh_mgr_callback_bondingACL, currentDevice);
              
              break;
              
            }
        }
      if(status == BT_NOERROR)
        return BT_PENDING;
      else
        return status;
              
    }
  return BT_UNKNOWNERROR;
}




/******************************************************************************************************************
 * FUNCTION :- ProcessAccessRequest
 * 
 * INPUTS:  direction:    direction of the connection that the access request is for (MGR_INCOMING or MGR_OUTGOING).
 *          secDbEntry:   entry in the Service Database for the service accepting/settup up the connection.
 *          address:      address of remote device that the access that the connection attempt is to/from.
 *          callbackFunc: callback function to be invoked when access request has been completed.
 *          cbHandle:     callback handle for this access request.
 *       
 * RETURNS:
 *          (none)
 *
 * DESCRIPTION :- Performs the access request for a connection to/from a service to/from a device. The security mode 
 * of the Host is first checked. This function will do nothing if security is not MGR_SERVICELEVEL. The security level
 * for the service is checked in 'secDbEntry' . If authorisation or authentication is required, it is initiated in 
 * the HC. The trust level of the remote device is checked in the Device database to determine if authorisation is 
 * required. If it is required, a structure is allocated in the device entry and the result of the authentication
 * is awaited.
 *
 ******************************************************************************************************************/

t_api ProcessAccessRequest(u_int8 direction, struct st_t_mgr_secdbentry *secDbEntry, t_bdaddr address, callbackFuncPtr_t callbackFunc, u_int32 cbHandle)
{
    
    
    struct st_t_BT_ConnectionInfo *currentDevice;
    u_int8 useDefault = 0; 
    u_int8 i;
    t_api status;
    
    if(MGR_NOSECURITY == prh_security_mode || MGR_LINKLEVEL == prh_security_mode || NULL == secDbEntry)
        return BT_NOERROR; /* No security procedure required */
    
  
    /* Authentication is required if the Authentication OR Authorisation bit is set in Security Level
    or if the default Security Level is being used */
    if(!secDbEntry || AUTHEN_ON(secDbEntry->securityLevel, direction) || AUTHOR_ON(secDbEntry->securityLevel, direction) )
    {

        currentDevice = prh_mgr_return_deviceEntryByAddress(address);
        if(!currentDevice)
            return BT_NORESOURCES; 
         
        status=prh_mgr_set_callback(&i,(void **)currentDevice->bondingCallbackFunc,(void *)callbackFunc,currentDevice);
        
		/* If the callback could not be set */
		if (status)
		{
			/* If the authentication is not pending */
			if(!((AUTH_PENDING == currentDevice->accessReqAuthState)  ||
			   (BOND_AUTH_PENDING == currentDevice->bondingState)))
                return status;
		}

        currentDevice->accessReqCallbackFunc[i]= callbackFunc;
        currentDevice->accessReqCallbackHandle[i]= cbHandle;
        currentDevice->recordState|=MGR_DEVDB_CALLBACK_PENDING;
        currentDevice->outstandingCallbacks++;
        if(!useDefault)
        { /* Check Enryption bit so that it can be initiated when Authentication is completed */
            if(ENCRYPT_ON(secDbEntry->securityLevel, direction))
                currentDevice->accessReqCallbackEncryptOn[i] = 1;
            /* If Authorisation is set, set up Authorisation Info structure to be passed to Authorisation callback */
            /* when Authentication is completed */
            if(AUTHOR_ON(secDbEntry->securityLevel, direction) && MGR_TRUSTED != currentDevice->trustLevel )
            {
                currentDevice->authorisationInfo[i] = pMalloc(sizeof(struct st_t_BT_Authorisation));
                if(NULL == currentDevice->authorisationInfo[i])
                    return BT_NORESOURCES;
                currentDevice->authorisationInfo[i]->direction = direction;   
                prh_mgr_copyBDAddress(&currentDevice->authorisationInfo[i]->address,&currentDevice->bdAddress);
                currentDevice->authorisationInfo[i]->serviceType = secDbEntry->serviceType;
                if(MGR_APPSERVICE == secDbEntry->serviceType)
                    currentDevice->authorisationInfo[i]->service.serviceName = secDbEntry->service.name;
                else
                    currentDevice->authorisationInfo[i]->service.protocolId = secDbEntry->service.protocolId;
                currentDevice->authorisationInfo[i]->multiplexProtocolId = secDbEntry->multiplexProtocolId;
                currentDevice->authorisationInfo[i]->multiplexChannelId = secDbEntry->multiplexChannelId;
            }
        }
        
        /* If Authentication has recently been invoked, do not invoke again, the event from the
        pending authentication can be used for this access request */
        if((AUTH_PENDING == currentDevice->accessReqAuthState) ||
		   (BOND_AUTH_PENDING == currentDevice->bondingState))
            return BT_PENDING;
        else
        {
            currentDevice->accessReqAuthState = AUTH_PENDING;
            if(BT_NOERROR == HCI_AuthenticationRequested(currentDevice->aclHandle) )
            {
                pendingSecHandle=currentDevice->aclHandle;
                return BT_PENDING;
            }
            else /* Reset the callback entry */
            {
                currentDevice->accessReqAuthState = AUTH_IDLE;
                currentDevice->accessReqCallbackFunc[i] = NULL;
                if (!--currentDevice->outstandingCallbacks)
                    currentDevice->recordState&=~MGR_DEVDB_CALLBACK_PENDING;            
                currentDevice->accessReqCallbackEncryptOn[i] = 0;
                return BT_AUTHENTICATIONFAIL; 
            }
        }
        
    }
    
    return BT_NOERROR;
}




/********************************************************************************************
 * FUNCTION :- AddNewServiceEntry
 * 
 * INPUTS:
 *          (none)
 *
 * RETURNS:
 *          struct st_t_mgr_secdbentry: pointer to new entry in the Security Database.
 *          
 *
 * DESCRIPTION :- Adds a new entry to the Security Database and returns a pointer to this entry
 *
 ********************************************************************************************/

struct st_t_mgr_secdbentry *AddNewServiceEntry(void)
{
  struct st_t_mgr_secdbentry *newServiceEntry;

  if (prh_service_list_tail) 
    {
			if(prh_service_list_head == prh_service_list_tail)
			{
				prh_service_list_tail = NULL;
				prh_service_list_tail=pMalloc(sizeof(struct st_t_mgr_secdbentry));
				if(prh_service_list_tail == NULL)
					return NULL;
				newServiceEntry=prh_service_list_tail;
				prh_service_list_head->nextService=prh_service_list_tail;
			}
			else
			{
				prh_service_list_tail->nextService=pMalloc(sizeof(struct st_t_mgr_secdbentry));
				if(prh_service_list_tail->nextService == NULL)
					return NULL;
				newServiceEntry=prh_service_list_tail->nextService;
			}
    } 
  else 
    {
			prh_service_list_head=pMalloc(sizeof(struct st_t_mgr_secdbentry));
			if(prh_service_list_head == NULL)
					return NULL;
			newServiceEntry=prh_service_list_head;

    }

  if(!newServiceEntry)
    return NULL;

  newServiceEntry->service.name=NULL;
  newServiceEntry->nextService = NULL;
  prh_service_list_tail = newServiceEntry;

  return newServiceEntry;
}



/**********************************************************************************************
 * FUNCTION :- GetServiceEntryByAppName
 *
 * INPUTS:
 *          serviceName: name of application entry being searched for.
 *
 * RETURNS:
 *          struct st_t_mgr_secdbentry: pointer to Security Database entry found.
 *
 * DESCRIPTION :- Returns a pointer to an entry in the Security database for a service with a
 *                particular name.
 * 
 ***********************************************************************************************/

struct st_t_mgr_secdbentry *GetServiceEntryByAppName(char *serviceName)
{
  struct st_t_mgr_secdbentry *currentServiceEntry = prh_service_list_head;

  if(serviceName)
    {
      while (currentServiceEntry) {
        if (MGR_APPSERVICE == currentServiceEntry->serviceType && currentServiceEntry->service.name)
          {
            if (!pStrncmp(currentServiceEntry->service.name, serviceName,MGR_MAXNAMELEN)) 
              return currentServiceEntry; 
          }
        currentServiceEntry=currentServiceEntry->nextService;
      }
    }
  
  return NULL;
}




/**********************************************************************************************
 * FUNCTION :- GetServiceEntryByProtocolId
 *
 * INPUTS:
 *          protocolId: Id of protocol entry being searched for.
 *
 * RETURNS:
 *          struct st_t_mgr_secdbentry: pointer to Security Database entry found.
 *
 * DESCRIPTION :- Returns a pointer to an entry in the Security database for a service with a
 *                particular protocol ID.
 * 
 ***********************************************************************************************/

struct st_t_mgr_secdbentry *GetServiceEntryByProtocolId(u_int16 protocolId)
{
  struct st_t_mgr_secdbentry *secDbEntry = prh_service_list_head;

  while(secDbEntry) {
    if(MGR_PROTOSERVICE == secDbEntry->serviceType)
      {
        if(protocolId == secDbEntry->service.protocolId)
          return secDbEntry;
      }
    secDbEntry = secDbEntry->nextService;
  }

  return NULL;
}




/**********************************************************************************************
 * FUNCTION :- GetServiceEntryByMultiplexInfo
 *
 * INPUTS:
 *          multiplexProtocolId: Id of a multiplexing protocol 
 *          multiplexChannelId:  Id that is used by the multiplexing protocol to identify a
 *                               higher layer service (application/service).
 *
 * RETURNS:
 *          struct st_t_mgr_secdbentry: pointer to Security Database entry found.
 *
 * DESCRIPTION :- Returns a pointer to an entry in the Security database based on a search by
 *                multiplex protocol Id and multiplex channel Id.
 * 
 ***********************************************************************************************/

struct st_t_mgr_secdbentry *GetServiceEntryByMultiplexInfo(u_int16 multiplexProtocolId, u_int32 multiplexChannelId)
{
  struct st_t_mgr_secdbentry *secDbEntry = prh_service_list_head;

  
  while (secDbEntry) {
        if(multiplexProtocolId == secDbEntry->multiplexProtocolId)
          {
            if(multiplexChannelId == secDbEntry->multiplexChannelId)
              return secDbEntry;
          }
    secDbEntry=secDbEntry->nextService;
  }
  
  return NULL;
}
