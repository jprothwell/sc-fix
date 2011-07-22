/*
 * MODULE NAME:    hs_core.c
 * DESCRIPTION:    Headset Profile Core Headset Application for the Bluedev/AMX platform
 * AUTHOR:         Jenny Murphy
 *
 * SOURCE CONTROL: $Id: hs_core.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2003 Parthus Technologies.
 *     All rights reserved.
 *
 */
#include <ctype.h>   //toupper
#include <stdio.h>   //sprintf
#include <string.h>

#include "papi.h"
#include "stack_ent_api.h"
#include "host_error.h"
#include "host_types.h"
#include "sdp_constants.h"
#include "rfcomm_types.h"
#include "host_config.h"
#include "host_buf.h"
#include "sdp_ue.h"
#include "hs_core.h"
#include "hci_primitives.h"
#include "mgr_core.h"
#include "l2cap_primitives.h"
#include "sdp_server.h"
#include "host_msg.h"

/* defines for connection confirmation */
#define RSE_CONNECT_REFUSE 0
#define RSE_CONNECT_ACCEPT 1
#define USE_SECURITY 0

#define VOLCTRLENABLED 1

#define HSP_RING_TIMEOUT      20  // 2 Seconds between rings

enum linkState aclState, scoState;

u_int16 gatewaySCOhandle, gatewayACLhandle;
u_int8 gatewayServerChannel, gatewayDLCI;
t_bdaddr gatewayBDAddress;
u_int8 headsetTxOK;
u_int8 HSSrvChan;
volatile int readyToReturn;

t_RSE_Callbacks HS_rfcommCallbacks;
t_bdaddr gCur_device_addr;
u_int8 gCur_Server_channel=0, gCur_DLCI =0, gDisconFlag=0;
u_int16 gCur_ACL_handle=0, gCur_SCO_handle=0;
t_pTimer ring_timer;
extern u_int8  gCurrent_Active_Profile;
extern u_int32 rdabt_put_message(u_int16 q, u_int16 mi, void *mv);

int HS_Init(void) 
{
    t_RSE_Callbacks rfcommCallbacks;

    int status;
		char *LocalName;
    u_int8 srvChan=5;

    headsetTxOK=1;
		readyToReturn=0;
    aclState=DISCONNECTED;
    scoState=DISCONNECTED;
    gatewaySCOhandle=0xFFFF; /* invalid handle value */
	
    status=HCI_RegisterEventHandler(hciEventCallback);
	if (status!=BT_NOERROR) 
        return 1;

    status=HCI_WriteVoiceSetting(VOICESETTING);
	if (status!=BT_NOERROR) 
        return 1;

    status=HCI_WriteClassOfDevice(HEADSETCOD); 
	if (status!=BT_NOERROR) 
        return 1;

	/* register as a RFCOMM server */
    memset(&rfcommCallbacks,0,sizeof(rfcommCallbacks));
	rfcommCallbacks.connectionConfirmation = HS_rfcommConnectConfirmCallback;
	rfcommCallbacks.connectionIndication = HS_rfcommConnectIndicationCallback;
	rfcommCallbacks.dataIndication = HS_rfcommRxDataCallback;
	rfcommCallbacks.disconnectionIndication = HS_rfcommDisconnectIndicationCallback;
	rfcommCallbacks.flowIndication = HS_rfcommFlowCtrlCallback;
	status = RSE_SrvRegisterPort(&srvChan, &rfcommCallbacks);

	if (status!=BT_NOERROR) 
        return 1;
		
		HSSrvChan=srvChan;

    status=HS_SetupSDPRecord(0x5, VOLCTRLENABLED, " RDA HS ", 8);
    if (status!=BT_NOERROR)
        return 1;


	status=HCI_WriteScanEnable(0x03);

	if (status!=BT_NOERROR)
    return 1;

	LocalName="Parthus Technologies BT Headset";

	HCI_ChangeLocalName((u_int8*)LocalName);


	/* enable authentication and encryption */
#if USE_SECURITY
    MGR_SetSecurityMode(MGR_LINKLEVEL,NULL);
    HCI_WriteEncryptionMode(0x01);
#endif
	
    return 0;
}

int HS_Shutdown(void)
{
	int status;

	status=RSE_SrvDeregisterPort(HSSrvChan);
	if(status)
		return 1;

	return 0;
}

int HS_ConnectToGateway() 
{
    int status;
   t_bdaddr address;
    u_int8 serverChannel;
    t_RSE_Callbacks rfcommCallbacks;

    if (aclState==DISCONNECTED) 
    {
        /* perform SDP lookup for servce channel number */
        status=HS_RetrieveSDPParms(address, &serverChannel);
        if(status!=BT_NOERROR)
        	return status;
        
	if(serverChannel > 32) /*retry*/
	{
		status=HS_RetrieveSDPParms(address, &serverChannel);
		if(status!=BT_NOERROR)
			return status;
        
		if(serverChannel > 32)
		{
			return 1;
		}
	}
    
        memset(&rfcommCallbacks,0,sizeof(rfcommCallbacks));
        rfcommCallbacks.connectionConfirmation = HS_rfcommConnectConfirmCallback;
        rfcommCallbacks.connectionIndication = HS_rfcommConnectIndicationCallback;
        rfcommCallbacks.dataIndication = HS_rfcommRxDataCallback;
        rfcommCallbacks.disconnectionIndication = HS_rfcommDisconnectIndicationCallback;
        rfcommCallbacks.flowIndication = HS_rfcommFlowCtrlCallback;
        aclState=PENDING;
  
        status=RSE_CliConnectPort(address,serverChannel,50,&rfcommCallbacks);       
        if(status!=BT_PENDING)
		return 1;
    }
    
    if (aclState==PARKED) 
    {
        aclState=PENDING;
        status=HCI_ExitParkMode(gatewayACLhandle);
	if(status!=BT_NOERROR)
            return 1;
    }
    return 0;
}


int HS_SetupSDPRecord(u_int8 rf_serverChannel, u_int8 volCtrlStatus, char *serviceName, u_int16 serviceNameLen) {
#if SDP_RECORD_GEN
	int status;
	t_SDP_serviceRecord *pRecord;
	u_int32 recordHandle;

	status = SDP_CreateRecord(&recordHandle, &pRecord);
    if (status!=BT_NOERROR)
        return 1;

    {
	    /*** add the Service Class List */
	    u_int16 uuid;
	    t_SDP_dataElement elem_headsetUUID;
	    t_SDP_dataElement elem_genAudioUUID;
	    t_SDP_dataElement *elem_serviceUUIDList[10];
	    t_SDP_dataElement elem_ServiceClassIDList;

        /* Encode the headset uuid to a data element */
	    uuid = SDP_SCLASS_UUID_HEADSET; 
	    status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_headsetUUID);
        if (status!=BT_NOERROR)
            return 1;
	    
        /* Encode the generic audio uuid to an attribute */
	    uuid = SDP_SCLASS_UUID_GEN_AUDIO; 
	    status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_genAudioUUID);
        if (status!=BT_NOERROR)
            return 1;

        /* Add both encoded uuids to a list */
	    elem_serviceUUIDList[0] = &elem_headsetUUID;
	    elem_serviceUUIDList[1] = &elem_genAudioUUID;
	    status = SDP_CreateList(SDP_SEQUENCE, 2, elem_serviceUUIDList, &elem_ServiceClassIDList);
        if (status!=BT_NOERROR)
            return 1;
	    status = SDP_AddAttribute(pRecord, SDP_AID_SERVICE_CLASS, &elem_ServiceClassIDList);
        if (status!=BT_NOERROR)
            return 1;

    }



    {
	    /******* add the Protocol Descriptor List (PDL) 
            The PDL is a list containing two lists, one for each lower protocol 
            <PDL:  <L2CAP: UUID> <RFCOMM: UUID, serverChannel> >
        */
        t_SDP_dataElement elem_l2capProtocolInfoList;
        t_SDP_dataElement elem_rfcommProtocolInfoList;
        t_SDP_dataElement elem_protocolDescriptorList;
        t_SDP_dataElement *elem_protocolDescriptorElements[10];

        {
            /**** create the L2CAP protocol information list */
            u_int16 uuid;
            t_SDP_dataElement elem_l2capUUID;
            t_SDP_dataElement *elem_l2capProtocolInfoElements[10];

	        /* encode the l2cap protocol uuid */
	        uuid = SDP_PROTOCOL_UUID_L2CAP;
	        status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_l2capUUID);
            if (status!=BT_NOERROR)
                return 1;

	        /* Add the l2cap protocol uuid to a list */
	        elem_l2capProtocolInfoElements[0] = &elem_l2capUUID;
	        status = SDP_CreateList(SDP_SEQUENCE, 1, elem_l2capProtocolInfoElements, &elem_l2capProtocolInfoList);
            if (status!=BT_NOERROR)
                return 1;
        }

        {
            /**** create the RFCOMM protocol information list */
            u_int16 uuid;
            t_SDP_dataElement elem_rfcommUUID;
            t_SDP_dataElement elem_rfcommServerChannel;
            t_SDP_dataElement *elem_rfcommProtocolInfoElements[10];

	        /* encode the rfcomm protocol uuid */
            uuid = SDP_PROTOCOL_UUID_RFCOMM;
            status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_rfcommUUID);
            if (status!=BT_NOERROR)
                return 1;

	        /* encode the rfcomm server channel number */
            status = SDP_EncodeAttribute(SDP_UINT, 1, (u_int8 *)&rf_serverChannel, &elem_rfcommServerChannel);
            if (status!=BT_NOERROR)
                return 1;

            /*  Add the both of the attributes to an RFCOMM protocol information list */
            elem_rfcommProtocolInfoElements[0] = &elem_rfcommUUID;
            elem_rfcommProtocolInfoElements[1] = &elem_rfcommServerChannel;
            status = SDP_CreateList(SDP_SEQUENCE, 2, elem_rfcommProtocolInfoElements, &elem_rfcommProtocolInfoList);
            if (status!=BT_NOERROR)
                return 1;
        }


        /* Enclose both L2CAP and RFCOMM protocol information lists in an outer protocol descriptor list */
        elem_protocolDescriptorElements[0] = &elem_l2capProtocolInfoList;
        elem_protocolDescriptorElements[1] = &elem_rfcommProtocolInfoList;

        status = SDP_CreateList(SDP_SEQUENCE, 2, elem_protocolDescriptorElements, &elem_protocolDescriptorList);
        if (status!=BT_NOERROR)
            return 1;

        /* Add the protocol descriptor list as a record attribute */
        status = SDP_AddAttribute(pRecord, SDP_AID_PROTOCOL, &elem_protocolDescriptorList);
				if (status!=BT_NOERROR)
            return 1;
    }




    {
        /**** add the Profile Descriptor List */
        t_SDP_dataElement elem_profile0DescriptorList;
	    t_SDP_dataElement elem_ProfileDescriptorList;
	    t_SDP_dataElement *elem_ProfileDescriptorElements[10];

        {
           /**** create the Profile0 Descriptor List */

            u_int16 uuid;
            t_SDP_dataElement elem_profile0UUID;
            t_SDP_dataElement elem_profile0Version;
            t_SDP_dataElement *elem_profile0DescriptorElements[10];
            u_int16 profile0Version=0x0100;

	        /* encode the profile0 protocol uuid */
            uuid = SDP_SCLASS_UUID_HEADSET;
            status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_profile0UUID);
            if (status!=BT_NOERROR)
                return 1;

	        /* encode the profile0 server channel number */
            status = SDP_EncodeAttribute(SDP_UINT, 2, (u_int8 *)&profile0Version, &elem_profile0Version);
            if (status!=BT_NOERROR)
                return 1;

            /*  Add the both of the attributes to an profile0 protocol information list */
            elem_profile0DescriptorElements[0] = &elem_profile0UUID;
            elem_profile0DescriptorElements[1] = &elem_profile0Version;
            status = SDP_CreateList(SDP_SEQUENCE, 2, elem_profile0DescriptorElements, &elem_profile0DescriptorList);
						if (status!=BT_NOERROR)
							return 1;
        }  
        
        /* Enclose both L2CAP and RFCOMM protocol information lists in an outer Profile Descriptor List */
        elem_ProfileDescriptorElements[0] = &elem_profile0DescriptorList;

        status = SDP_CreateList(SDP_SEQUENCE, 1, elem_ProfileDescriptorElements, &elem_ProfileDescriptorList);
        if (status!=BT_NOERROR)
            return 1;

        /* Add the the Profile0 Descriptor List to the record  */
        status = SDP_AddAttribute(pRecord, SDP_AID_PROFILE_LIST, &elem_ProfileDescriptorList);
        if (status!=BT_NOERROR)
            return 1;

    }


    {
        /**** add the Service Name */
        t_SDP_dataElement elem_serviceName;
        u_int16 nameAttributeId = SDP_AID_DEFAULT_LANGUAGE+SDP_AID_SERVICE_NAME_OFFSET; 

        /*  Encode the name as a string attrbiute */
        status = SDP_EncodeAttribute(SDP_TEXT, serviceNameLen, (u_int8*)serviceName, &elem_serviceName);
        if (status!=BT_NOERROR)
            return 1;

        /* Add the the Profile0 Descriptor List to the record  */
        status = SDP_AddAttribute(pRecord, nameAttributeId, &elem_serviceName);
        if (status!=BT_NOERROR)
            return 1;
    }

    {
        /**** add the Volume Control Support flag */
        t_SDP_dataElement elem_volCtrlSupported;

        /*  Encode the name as a string attrbiute */
        status = SDP_EncodeAttribute(SDP_BOOLEAN, 1, &volCtrlStatus, &elem_volCtrlSupported);
        if (status!=BT_NOERROR)
            return 1;

        /* Add the the Volume Control Support flag to the record  */
        status = SDP_AddAttribute(pRecord, SDP_AID_AUDIO_VOLUME, &elem_volCtrlSupported);
        if (status!=BT_NOERROR)
            return 1;
    }
#endif
    return 0; /* success */

}

#if 0
u_int8 get_list_length(u_int8* data,u_int32 *len,u_int8* offset) {
    u_int8* internal;
	
	if(!data)
	{
		return BT_INVALIDPARAM;
	}

	internal = data;
	
    switch(*internal) {

        case SDP_SEQUENCE_SMALL:
        case SDP_UNION_SMALL:
        case SDP_URL_SMALL:
        case SDP_TEXT_SMALL:
            *len = internal[1];
            *offset = 2;
            break;

        case SDP_SEQUENCE_MEDIUM:
        case SDP_UNION_MEDIUM:
        case SDP_URL_MEDIUM:
        case SDP_TEXT_MEDIUM:
            *len = (internal[1] << 8) + internal[2];
            *offset = 3;
            break;

        case SDP_SEQUENCE_LARGE:
        case SDP_UNION_LARGE:
        case SDP_URL_LARGE:
        case SDP_TEXT_LARGE:
            *len = (internal[1] << 24) + (internal[2] << 16) + (internal[3] << 8) + internal[4];
            *offset = 5;
            break;

        default:
            return BT_INVALIDPARAM;
    }

    return BT_NOERROR;
}


u_int8 read_uuid(u_int8* data,u_int32* uuid,u_int8* offset) {

	switch(*data) {
	    case SDP_UUID_2:
		    *uuid = (data[1] << 8) + data[2];
		    *offset = 3;
		    break;

	    case SDP_UUID_4:
		    *uuid = (data[1] << 24) + (data[2] << 16) + (data[3] << 8) + data[4];
		    *offset = 5;
		    break;

	    case SDP_UUID_16:
		    *uuid = (data[1] << 24) + (data[2] << 16) + (data[3] << 8) + data[4];
		    *offset = 17;
		    break;

	    default:
		    return BT_INVALIDPARAM;
	}
	return BT_NOERROR;
}
#endif

int sdpSearchComplete, sdpStatus;

void sdpSearchCompleteCallback(u_int8 status) {

    sdpStatus=status;
    sdpSearchComplete=1;
}

void OS_DELAY(u_int32 dl)
{
    u_int32 volatile delay;

    for (delay = 0; delay < dl * 100000; delay ++)
    {
	  ;
    }
	return;
}

extern u_int8 get_list_length(u_int8* data,u_int32 *len,u_int8* offset);
extern u_int8 read_uuid(u_int8* data,u_int32* uuid,u_int8* offset);
int HS_RetrieveSDPParms(t_bdaddr address, u_int8 *rf_serverChannel) 
{
    int status;
    u_int32 uuid;
    t_SDP_Addresses devices;
    u_int16 criteria;
    t_SDP_SearchPattern pattern;
    t_SDP_AttributeIDs attribs;

    t_SDP_StopRule rule;

    t_SDP_Results *pResults;
    u_int8* finger;
    u_int8 counter;
    u_int16 aid;
    u_int8 offset;
    u_int32 length;
    u_int32 outer;
    u_int32 inner;
    t_bdaddr m_bdaddr;
    u_int32  m_patternUUID;
    u_int32  m_attributeID;

    uuid = 0x1105; /* obex */

    /* address list to search for */
    devices.numItems = 1;
    devices.addresses = &m_bdaddr;  
    devices.addresses[0] = address;
    
    /* device relation criteria */
    criteria = 0;
    
    /* search pattern */
    pattern.numItems = 1;
    pattern.patternUUIDs = &m_patternUUID;
    pattern.patternUUIDs[0] = SDP_SCLASS_UUID_HEADSET;//SDP_SCLASS_UUID_HEADSET_GATEWAY;
    
    /* attributes to retrieve */
    attribs.numItems = 1;
    attribs.attributeIDs = &m_attributeID;
    attribs.attributeIDs[0] = SDP_AID_PROTOCOL;
    
    /* stop rule */
    rule.maxItems = 10;
    rule.maxDuration = 20;
    rule.maxBytes = 250;

    sdpSearchComplete = 0;
    
    /* call a service search  */ 
    status=SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,sdpSearchCompleteCallback);    
    if (status!=BT_PENDING)
    {
        return status;
    }
    
    while(!sdpSearchComplete) 
           sxr_Sleep(16384); //OS_DELAY(1000);   1s
	
    
    /* examine the results */
    if (pResults->status!=0x00  || sdpStatus!=0) 
    {
        return sdpStatus; /* error or timeout */
    }
    
    
    if(!pResults->element.pData)
    {
        return BT_UNKNOWNERROR;
    }

    finger = pResults->element.pData;
    
    get_list_length(finger,&length,&offset);
    finger+= offset;
    get_list_length(finger,&length,&offset);
    finger+= offset;
    counter = length;
    do {
        counter--;
        finger++; /* bypass type code */
        aid = (finger[0] << 8) + finger[1];
        finger+=2;
        counter-=2;
        switch(aid) 
        {
            case SDP_AID_PROTOCOL:
                get_list_length(finger,&length,&offset);
                finger += offset;
                counter -= offset;
                outer = length;
                do {
                    do {
                        get_list_length(finger,&length,&offset);
                        finger +=offset;
                        counter -=offset;
                        outer -=offset;
                        inner = length;
                        read_uuid(finger,&uuid,&offset);
                        finger += offset;
                        counter -= offset;
                        outer -= offset;
                        inner -= offset;
                        if(uuid == SDP_PROTOCOL_UUID_RFCOMM && inner) 
                        {
                            *rf_serverChannel = finger[1];
                            finger+=2;
                            counter-=2;
                            inner -= 2;
                            outer-=2;
                        } 
                        else 
                        {
                            finger += inner;
                            counter -= inner;
                            outer -= inner;
                            inner = 0;
                        }					
                    } while(inner);
                } while (outer);
        }
    } while (counter);

    return BT_NOERROR;
}

int HS_SendToGateway(u_int8 *data, int dataLen) 
{
#ifdef COOLSAND
    int status;
    struct host_buf  *txData;

	status = RSE_GetWriteBuffer(gCur_device_addr, gCur_DLCI, dataLen+10, &txData);
    if (status!=BT_NOERROR)
    {
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_SendToGateway get buffer error!!!"));
        return 1;
    }

    memcpy(txData->data,data,dataLen);
    txData->len=dataLen;
    /*
    while (!headsetTxOK) 
    {
        OS_DELAY(1000);
    }
*/
    if (headsetTxOK == 0)
    {
        RSE_FreeWriteBuffer(txData);
        return 1;
    }

    status = RSE_Write(gCur_device_addr, gCur_DLCI, txData);
    if (status!=BT_NOERROR) 
    {
        RSE_FreeWriteBuffer(txData);
        return 1;
    }
    else 
        return 0;
    
#else
    int status;
    struct host_buf  *txData;

	status = RSE_GetWriteBuffer(gatewayBDAddress, gatewayDLCI, dataLen+10, &txData);
    if (status!=BT_NOERROR)
        return 1;

    memcpy(txData->data,data,dataLen);
    txData->len=dataLen;

    while (!headsetTxOK) {
        OS_DELAY(1000);
    }

    status = RSE_Write(gatewayBDAddress, gatewayDLCI, txData);
    if (status!=BT_NOERROR) {
        RSE_FreeWriteBuffer(txData);
        return 1;
    } else 
        return 0;
#endif
}

int HS_Send_OK(void)
{
    char OK_Command[7] = "\r\nOK\r\n";
    return HS_SendToGateway((u_int8*)OK_Command,6);
}

int HS_Send_Error(void)
{
    char Error_Command[10] = "\r\nERROR\r\n";
    return HS_SendToGateway((u_int8*)Error_Command,9);
}

void HS_HandleRxATCommand(u_int8 *data, int dataLen) 
{
#ifdef COOLSAND
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_HandleRxATCommand aclState=%d,scoState=%d, ring_timer=%d", aclState, scoState, ring_timer));
#endif  

    if (!strncmp(HS_ATCKPDSTRING,(char*)data,strlen(HS_ATCKPDSTRING))) 
    {
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"AT+CKPD=200 aclState=%d scoState=%d ring_timer=%d\n", aclState,scoState, ring_timer));
        if(!ring_timer && scoState == DISCONNECTED)
        {
            HS_Send_Error();
            return;    
        }
            
        if(ring_timer && scoState == DISCONNECTED)
        {
            pTimerCancel(ring_timer);
            ring_timer =0;
            HS_Send_OK();
            scoState = CONNECTED;
            rdabt_put_message(EXTERNAL_TASK_MASK,HSP_Q|HSG_RING_CNF,NULL);
        }
        else  if(scoState == CONNECTED)//connected
        {
            scoState= PENDING;
            gDisconFlag = 1;
            HS_Send_OK();
            rdabt_put_message(EXTERNAL_TASK_MASK,HSP_Q|HSG_DISCONNECT_FROM_HS_IND,&gCur_ACL_handle);
        }
    }
     else  if (!strncmp(HS_ATCKPD,(char*)data,strlen(HS_ATCKPD))) 
    {
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_HandleRxATCommand AT+CKPD scoState=%d \n", aclState));
        /*if(aclState == PENDING)
        {
            st_t_hsg_connect_ind ret;
            u_int16 cid;
            u_int8 i=0;

            pTimerCancel(ring_timer);
            ring_timer =0;
            HS_Send_OK();

            RSE_GetCIDFromConnection(gCur_device_addr, gCur_DLCI, &cid);
            L2_RetrieveHandle(cid,&gCur_ACL_handle);
    
            ret.connection_id = gCur_ACL_handle;
            for(i=0;i<6;i++)
                ret.bt_addr.bytes[i] = gCur_device_addr.bytes[i];
            rdabt_put_message(EXTERNAL_TASK_MASK,HSP_Q|HSG_ACL_CONNECT_IND,&ret);
            aclState = CONNECTED;
        }
        else  if(aclState==CONNECTED)//connected
        {

        }*/
    }
    else if(!strncmp(HS_ATSPKRVOLSTRING,(char*)data,strlen(HS_ATSPKRVOLSTRING)))
    {
        st_t_hsg_gain_ind ret;
        u_int8 gain;
        sscanf((char*)data+7,"%c",&gain);   
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_HandleRxATCommand  gain=%d \n", gain));

        ret.connection_id = gCur_ACL_handle;
        ret.gain = gain;

        rdabt_put_message(EXTERNAL_TASK_MASK,HSP_Q|HSG_SPEAKER_GAIN_IND,&ret);
    }
        
#else
    int volumeSetting;

    if (!strncmp(HS_SPKRVOLSTRING,(char*)data,strlen(HS_SPKRVOLSTRING))) {
        sscanf((char*)data+7,"%d",&volumeSetting);        
        //MMI_SpkrVolSet(volumeSetting);
    } else if (!strncmp(HS_MICVOLSTRING,(char*)data,strlen(HS_MICVOLSTRING))) {
        sscanf((char*)data+7,"%d",&volumeSetting);        
        //MMI_MicVolSet(volumeSetting);
    } else if (!strncmp(HS_RINGSTRING,(char*)data,strlen(HS_RINGSTRING))) {
        //MMI_Ring();
    } 
#endif
}

int HS_HeadsetButtonPressed(void) {
    int status;
		if (scoState==DISCONNECTED) 
        status=HS_CreateAudioLink();
    else 
        status=HS_ReleaseAudioLink();

		return status;
}


int HS_CreateAudioLink(void) 
{
    int status;
    scoState=PENDING;
    if (aclState!=CONNECTED)
    {
    	status=HS_ConnectToGateway();
    	if(status)
    	{
    		scoState=DISCONNECTED;
    		return status;
    	}
    }
    else
    {
    	status=HS_SendToGateway((u_int8*)HS_ATCKPDSTRING,strlen(HS_ATCKPDSTRING));
    	if(status)
    		return 1;
    }

    while(!readyToReturn)
    {
    	OS_DELAY(1000);
    }
    
    readyToReturn=0;
    
    return 0;
}

int HS_ReleaseAudioLink(void) 
{
    int status;
    status=HS_SendToGateway((u_int8*)HS_ATCKPDSTRING,strlen(HS_ATCKPDSTRING));	
    if(status)
	return 1;
    while(!readyToReturn)
    {
        OS_DELAY(1000);
    }
    readyToReturn=0;	
    return 0;
}


int HS_rfcommRxDataCallback(t_bdaddr address, u_int8 dlci, struct host_buf * rxData) 
{
    unsigned int i;
    static char assemblyBuffer[100];
    static int assemblyOffset=0, lfFound=0;

    if (aclState!=CONNECTED) /* This can happen for incoming connections */
        aclState=CONNECTED;
    
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_rfcommRxDataCallback rxData->len=%d\n", rxData->len));
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_rfcommRxDataCallback data[0-11]=%c%c%c%c%c%c%c%c%c%c%c%c\n", 
                                                                 rxData->data[0],rxData->data[1], rxData->data[2],rxData->data[3],rxData->data[4],rxData->data[5],rxData->data[6], rxData->data[7],rxData->data[8],rxData->data[9],rxData->data[10], rxData->data[11]));
    
    /* 
        Since RFCOMM is a stream, AT commands may be fragmented or combined
        AT commands are terminated with a carriage return so use this to parse them.
    */
    /* The headset recieves unsolicited result codes - see spec for format */
    for (i=0; i<rxData->len; i++) 
    {
        assemblyBuffer[assemblyOffset]=rxData->data[i];
        if (assemblyOffset<100)
            assemblyOffset++;
#ifdef COOLSAND
        if (rxData->data[i]=='\r') 
            lfFound++;
        if (lfFound==1) 
        {
            HS_HandleRxATCommand((u_int8*)assemblyBuffer,assemblyOffset);
            assemblyOffset=0;
            lfFound=0;
        }
#else
        if (rxData->data[i]=='\n') 
            lfFound++;
        if (lfFound==2) 
        {
            HS_HandleRxATCommand((u_int8*)assemblyBuffer,assemblyOffset);
            assemblyOffset=0;
            lfFound=0;
        }
#endif
    }
    RSE_FreeWriteBuffer(rxData);
    return 0;
}


int HS_rfcommConnectIndicationCallback(u_int8 schan, t_bdaddr address, u_int8 dlci, u_int16 max_frame_size) 
{

    int status;
#ifdef COOLSAND  
    u_int16 cid;
    st_t_hsg_connect_ind ret;
    int i;

    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_rfcommConnectIndicationCallback  schan=%d, dlci=%d, aclState=%d\n", schan, dlci, aclState));
    
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_rfcommConnectIndicationCallback aclState=%d,scoState=%d, ring_timer=%d", aclState, scoState, ring_timer));
#endif  

    if (aclState==DISCONNECTED) 
    {
        aclState=PENDING;
        gCur_DLCI=dlci;
        gCur_device_addr=address;
        gCur_Server_channel=schan;
        gCurrent_Active_Profile = 2; //headset_AG
        status=RSE_SrvConnectResponse(address, dlci, RSE_CONNECT_ACCEPT);
    } 
    else 
    {
        status=RSE_SrvConnectResponse(address, dlci, RSE_CONNECT_REFUSE);
    }
    if(status!=BT_NOERROR)
        return 0;

    RSE_GetCIDFromConnection(gCur_device_addr, gCur_DLCI, &cid);
    L2_RetrieveHandle(cid,&gCur_ACL_handle);
    
    ret.connection_id = gCur_ACL_handle;
    for(i=0;i<6;i++)
        ret.bt_addr.bytes[i] = address.bytes[i];
    rdabt_put_message(EXTERNAL_TASK_MASK,HSP_Q|HSG_ACL_CONNECT_IND,&ret);
    aclState = CONNECTED;
    
#else
    if (aclState==DISCONNECTED) 
    {
        aclState=PENDING;
        gatewayDLCI=dlci;
        gatewayBDAddress=address;
        gatewayServerChannel=schan;
        status=RSE_SrvConnectResponse(address, dlci, RSE_CONNECT_ACCEPT);
    } 
    else 
    {
        status=RSE_SrvConnectResponse(address, dlci, RSE_CONNECT_REFUSE);
    }
    if(status!=BT_NOERROR)
        return 0;

    RSE_GetCIDFromConnection(gatewayBDAddress, gatewayDLCI, &cid);
    L2_RetrieveHandle(cid,&gatewayACLhandle);
    //MMI_ACLActive();
#endif

	return 0;
}


int HS_rfcommConnectConfirmCallback(u_int8 serverChannel, t_bdaddr address, u_int8 status, u_int8 dlci, u_int16 max_frame_size) 
{
#ifdef COOLSAND
    u_int16 cid;
    u_int16 acl_handle;
    st_t_hsp_connect_cnf ret;
    int i;
    
    pDebugPrintfEX((pLOGERROR,pLOGHFP,"HS_rfcommConnectConfirmCallback  status=%d, channel=%d\n", status, serverChannel));
        
#if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGHFP,"HS_rfcommConnectConfirmCallback aclState=%d,scoState=%d, ring_timer=%d", aclState, scoState, ring_timer));
#endif  

    if(status ==1)  //no error
    {
        RSE_GetCIDFromConnection(address, dlci,&cid);
        if (cid)
        {
            L2_RetrieveHandle(cid,&acl_handle);
        }
        ret.result = BT_NOERROR;
        
        ret.connection_id = acl_handle;
        gCur_ACL_handle = acl_handle;
        aclState = CONNECTED;
        gCur_DLCI=dlci;
    }
    else //error
    {
        ret.result = 1;
        ret.connection_id=0;
        aclState = DISCONNECTED;
        scoState = DISCONNECTED;
    }

    for(i=0;i<6;i++)
        ret.bt_addr.bytes[i] = address.bytes[i];
    gCurrent_Active_Profile = 2;  //headset_AG
    rdabt_put_message(EXTERNAL_TASK_MASK,HSP_Q|HSG_ACL_CONNECT_CNF,&ret);
    
#else
    if (status==1) /* no error */ 
    {
        aclState=CONNECTED;
        gatewayDLCI=dlci;
        gatewayBDAddress=address;
        gatewayServerChannel=serverChannel;
	//MMI_ACLActive();
        HS_SendToGateway((u_int8*)HS_ATCKPDSTRING,strlen(HS_ATCKPDSTRING));
    }
    else
    {	
    	if(status==0)/*error*/
    	{
    		aclState=DISCONNECTED;
    		scoState=DISCONNECTED;
    		//if(cmdLocallyIntiated)
		//readyToReturn=1;
    		//MMI_NoConn();
    	}
    }
#endif   
    return 0;

}

int HS_rfcommDisconnectIndicationCallback(t_bdaddr address, u_int8 dlci) 
{
#ifdef COOLSAND
    st_t_hsg_disconnect_ind ret;
    u_int8 i=0;
    
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_rfcommDisconnectIndicationCallback aclState=%d,scoState=%d, ring_timer=%d", aclState, scoState, ring_timer));
#endif  
    if(aclState==CONNECTED)
    {
        ret.connection_id = gCur_ACL_handle;

        for(i=0;i<6;i++)
            ret.bt_addr.bytes[i] = gCur_device_addr.bytes[i];    

        rdabt_put_message(EXTERNAL_TASK_MASK,HSP_Q|HSG_ACL_DISCONNECT_IND,&ret);
        aclState=DISCONNECTED;
        gCur_ACL_handle=0;
        gCur_DLCI=0;
        memset(gCur_device_addr.bytes, 0, 6);
    }
    //if(cmdLocallyIntiated)
    //	readyToReturn=1;
    //MMI_ACLRelease();
#endif
	return 0;
}


int HS_rfcommFlowCtrlCallback(t_bdaddr address, u_int8 dlci,  u_int8 flow_state) 
{
     pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_rfcommFlowCtrlCallback flow_state=%d\n", flow_state));
    if (flow_state==RSE_FLOW_STOP)
    {
        headsetTxOK=0;
    }
    else
    {    
        headsetTxOK=1;
    }

	return 0;
}


void hciEventCallback(u_int8 *eventPdu) 
{
    u_int8 eventType=eventPdu[0];
		t_bdaddr ag_bdaddr;


    if(gCurrent_Active_Profile !=0x02) //headset ag
        return;
	#if pDEBUG
    pDebugPrintfEX((pLOGERROR,pLOGHFP,"hciEventCallback eventType=0x%x,status=0x%x, gCurrent_Active_Profile=%x\n",eventType,eventPdu[2],gCurrent_Active_Profile));
#endif  
    switch (eventType) 
    {

        case HCI_evMODECHANGE:
        {
                u_int8 status;
                u_int16 handle;
                u_int8 newMode;
                status=eventPdu[2];
                if (status==0x00) /* no error */ 
                {
                    handle=eventPdu[3]|(eventPdu[4]<<8);
                    if (handle==gatewayACLhandle)
                    {
                        newMode=eventPdu[5];                    
                    
                        if (aclState==PENDING && newMode==0x00 /* active */) 
                        {
                            aclState=CONNECTED;
                            HS_SendToGateway((u_int8*)HS_RINGSTRING,strlen(HS_RINGSTRING));
                            break;
                        }
                        
                        if (aclState==PARKED && newMode==0x00 /* active */) 
                        {
                            aclState=CONNECTED;
                            break;
                        }
                        
                        if (aclState==CONNECTED && newMode==0x03 /* parked */) 
                        {
                            aclState=PARKED;
                            break;
                        }
                    
                    }
                }
            }
        
            break;

        case HCI_evCONNECTIONCOMPLETE:
            {
                u_int8 status;
                u_int16 linkType=0;
                status=eventPdu[2];
                pDebugPrintfEX((pLOGERROR,pLOGHFP," hciEventCallback status=%d,linkType=%d\n", status,linkType));
                if (status==0x00) /* no error */ 
                {
                    linkType=eventPdu[11];
                     pDebugPrintfEX((pLOGNOTICE,pLOGHFP," hciEventCallback linkType=%d\n", linkType));

                    if (linkType==0x00) /* SCO */ 
                    {
#ifdef COOLSAND
                        if(scoState==DISCONNECTED)
                        {
                            st_t_hsg_sco_connect_cnf ret;
                             scoState=CONNECTED;
                            gCur_SCO_handle=eventPdu[3]|(eventPdu[4]<<8);
                            ret.result = BT_NOERROR;
                            ret.connection_id = gCur_SCO_handle;
                             pDebugPrintfEX((pLOGNOTICE,pLOGHFP," hciEventCallback gCur_SCO_handle=0x%x\n", gCur_SCO_handle));
                           // rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_SCO_CONNECT_CNF,&ret);
                        }
                        else if(scoState==CONNECTED)
                        {
                            pTimerCancel(ring_timer);
                            ring_timer =0;
                        }
#else
                        gatewaySCOhandle=eventPdu[3]|(eventPdu[4]<<8);

                        scoState=CONNECTED;
                        readyToReturn=1;
                        //MMI_SCOActive();
#endif                
                   }
                }
/*	 else //zhou siyou add 20090729
			{
				pDebugPrintfEX((pLOGERROR,pLOGHFP,"if((linkType==0x00) &&( (status==0x06)||(status==0x18)\n"));

				pMemcpy(&ag_bdaddr.bytes,eventPdu+5,6);

				if(HS_rfcommCallbacks.connectionConfirmation)

					HS_rfcommCallbacks.connectionConfirmation(0, ag_bdaddr, 0, 0, 0) ;

                }
*/
		 
            }
            break;

        case HCI_evDISCONNECTIONCOMPLETE:
            {
                t_api status;
                u_int16 handle;
                status=eventPdu[2];
                
                 pDebugPrintfEX((pLOGNOTICE,pLOGHFP," hciEventCallback disconnect status=%d\n", status));
#ifdef COOLSAND     
                if (status==0x00) /* no error */ 
                {
                    handle=eventPdu[3]|(eventPdu[4]<<8);
                    pDebugPrintfEX((pLOGNOTICE,pLOGHFP," hciEventCallback disconnect handle=0x%x, gCur_SCO_handle=0x%x\n", handle, gCur_SCO_handle));
                    if (handle==gCur_SCO_handle) 
                    {
                        HS_SCO_Release_Mgr_Callback(gCur_SCO_handle, status);//zhou siyou opne 090813
                    }

                }
#else
                if (status==0x00) /* no error */ 
                {
                    handle=eventPdu[3]|(eventPdu[4]<<8);
                    if (handle==gatewaySCOhandle) 
                    {
                        scoState=DISCONNECTED;
                        //MMI_SCORelease();
                        if (PARKIDLEACL) 
                        {
                            HCI_ParkMode(gatewayACLhandle,PARKMINBEACON,PARKMAXBEACON);
                        }
                    }

                }
#endif
                
            }
            break;

        case HCI_evCOMMANDCOMPLETE:
            {
                u_int16 commandOpcode;
                u_int8 status;
                commandOpcode=eventPdu[3]|(eventPdu[4]<<8);
                if (commandOpcode==HCI_cmdWRITEVOICESETTING) 
                {
                    status=eventPdu[5];
                    if (status!=0x00) /* no error */ 
                    {
                       // MMI_FatalError();
                    }
                }
            }
            break;
    }
}


void HS_MicVolChanged(int volumeSetting) 
{
    char buffer[15];    
    sprintf(buffer,"%s%d\r",HS_ATMICVOLSTRING,volumeSetting);
    if(aclState==CONNECTED)
        HS_SendToGateway((u_int8*)buffer,strlen(buffer));

}

void HS_SpkrVolChanged(int volumeSetting) 
{
    char buffer[15];    
    sprintf(buffer,"%s%d\r",HS_ATSPKRVOLSTRING,volumeSetting);
    if(aclState==CONNECTED)
        HS_SendToGateway((u_int8*)buffer,strlen(buffer));

}

#ifdef COOLSAND
static t_SDP_Results *pResults;

static void SearchCompletCallback(void *pArgs)
{
    int status;
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"SearchCompletCallback serverChannel=%d\n", gCur_Server_channel)); 
    status=HS_CreateACLConnect(gCur_Server_channel, gCur_device_addr);
}

int  ServerChannelSearchCompleteCallback(u_int8 status) 
{
    u_int8* finger;
    u_int8 counter;
    u_int16 aid;
    u_int8 offset;
    u_int32 length;
    u_int32 outer;
    u_int32 inner;
   // t_bdaddr m_bdaddr;
   // u_int32  m_patternUUID;
   // u_int32  m_attributeID;
    u_int32 uuid;
    u_int8  serverChannel=0;
    
    
    /* examine the results */
    if (pResults->status!=0x00) 
    {
        status = BT_UNKNOWNERROR;
        return status; 
    }
    
    if(!pResults->element.pData)
    {
        return BT_UNKNOWNERROR;
    }

    finger = pResults->element.pData;
    
    get_list_length(finger,&length,&offset);
    finger+= offset;
    get_list_length(finger,&length,&offset);
    finger+= offset;
    counter = length;
    do {
        counter--;
        finger++; /* bypass type code */
        aid = (finger[0] << 8) + finger[1];
        finger+=2;
        counter-=2;
        switch(aid) 
        {
            case SDP_AID_PROTOCOL:
                get_list_length(finger,&length,&offset);
                finger += offset;
                counter -= offset;
                outer = length;
                do {
                    do {
                        get_list_length(finger,&length,&offset);
                        finger +=offset;
                        counter -=offset;
                        outer -=offset;
                        inner = length;
                        read_uuid(finger,&uuid,&offset);
                        finger += offset;
                        counter -= offset;
                        outer -= offset;
                        inner -= offset;
                        if(uuid == SDP_PROTOCOL_UUID_RFCOMM && inner) 
                        {
                            serverChannel = finger[1];
                            finger+=2;
                            counter-=2;
                            inner -= 2;
                            outer-=2;
                        } 
                        else 
                        {
                            finger += inner;
                            counter -= inner;
                            outer -= inner;
                            inner = 0;
                        }					
                    } while(inner);
                } while (outer);
        }
    } while (counter);
    
    if(pResults)
    {
        SDP_FreeResults(pResults);
    }

    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"ServerChannelSearchCompleteCallback serverChannel=%d\n", serverChannel));
    gCur_Server_channel = serverChannel;
    pTimerCreate(20, SearchCompletCallback,(void*)NULL, pTIMER_ONESHOT);
    return status;
}

int HS_RetrieveServerChannel(t_bdaddr address) 
{
    int status;
    u_int16 criteria;
    t_SDP_SearchPattern pattern;
    t_SDP_AttributeIDs attribs;
    t_SDP_Addresses devices;


    t_SDP_StopRule rule;

   // t_SDP_Results *pResults;
//   u_int8* finger;
  //  u_int8 counter;
  //  u_int16 aid;
  //  u_int8 offset;
  // u_int32 length;
   // u_int32 outer;
  //  u_int32 inner;
    t_bdaddr m_bdaddr;
    u_int32  m_patternUUID;
    u_int32  m_attributeID;

    /* address list to search for */
    devices.numItems = 1;
    devices.addresses = &m_bdaddr;  
    devices.addresses[0] = address;
    
    /* device relation criteria */
    criteria = 0;
    
    /* search pattern */
    pattern.numItems = 1;
    pattern.patternUUIDs = &m_patternUUID;
    pattern.patternUUIDs[0] = SDP_SCLASS_UUID_HEADSET;//SDP_SCLASS_UUID_HEADSET_GATEWAY;
    
    /* attributes to retrieve */
    attribs.numItems = 1;
    attribs.attributeIDs = &m_attributeID;
    attribs.attributeIDs[0] = SDP_AID_PROTOCOL;
    
    /* stop rule */
    rule.maxItems = 10;
    rule.maxDuration = 20;
    rule.maxBytes = 250;

    sdpSearchComplete = 0;
    
    /* call a service search  */ 
    status=SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,(sdp_api_callback)ServerChannelSearchCompleteCallback);    
    if (status!=BT_PENDING)
    {
        return status;
    }

    return BT_NOERROR;
    
}

int HS_CreateACLConnect(u_int8 chn_num, t_bdaddr address) 
{
    int status;
//    u_int8 serverChannel;

    if (aclState==DISCONNECTED) 
    {
        /* perform SDP lookup for servce channel number */
        if(chn_num==0) 
        {
            status=HS_RetrieveServerChannel(address);
            if(status!=BT_NOERROR)
                return status;
        }
        else
        {
            aclState=PENDING;
            gCur_Server_channel = chn_num;
            status=RSE_CliConnectPort(address,chn_num,50,&HS_rfcommCallbacks);       
            if(status!=BT_PENDING)
                return 1;
        }
    }
    
    return 0;
}

//activate
void rda_bt_hsg_send_activate_req(void )
{
    //t_RSE_Callbacks rfcommCallbacks;

    int status=BT_NOERROR;
//    char *LocalName;
    u_int8 srvChan=2;

    headsetTxOK=1;
    readyToReturn=0;
    aclState=DISCONNECTED;
    scoState=DISCONNECTED;
    gatewaySCOhandle=0xFFFF; /* invalid handle value */
	
    status=HCI_RegisterEventHandler(hciEventCallback);


    /* register as a RFCOMM server */
    memset(&HS_rfcommCallbacks,0,sizeof(HS_rfcommCallbacks));
    HS_rfcommCallbacks.connectionConfirmation = HS_rfcommConnectConfirmCallback;
    HS_rfcommCallbacks.connectionIndication = HS_rfcommConnectIndicationCallback;
    HS_rfcommCallbacks.dataIndication = HS_rfcommRxDataCallback;
    HS_rfcommCallbacks.disconnectionIndication = HS_rfcommDisconnectIndicationCallback;
    HS_rfcommCallbacks.flowIndication = HS_rfcommFlowCtrlCallback;
    status = RSE_SrvRegisterPort(&srvChan, &HS_rfcommCallbacks);

		
    HSSrvChan=srvChan;

    /* enable authentication and encryption */
#if USE_SECURITY
    MGR_SetSecurityMode(MGR_LINKLEVEL,NULL);
    HCI_WriteEncryptionMode(0x01);
#endif
	
    rdabt_put_message(EXTERNAL_TASK_MASK,HSP_Q|HSG_ACTIVATE_CNF,&status);
}

void rda_bt_hsg_send_reject_cfm()
{
    pTimerCancel(ring_timer);
    ring_timer =0;
}
// deactivate
void rda_bt_hsg_send_deactivate_req(void )
{
    u_int8 status = BT_NOERROR;
    
    HCI_DeregisterEventHandler(hciEventCallback);
    status=RSE_SrvDeregisterPort(HSSrvChan);

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hsg_send_deactivate_req HSSrvChan=%d\n",HSSrvChan));
#endif  

    rdabt_put_message(EXTERNAL_TASK_MASK,HSP_Q|HSG_DEACTIVATE_CNF,&status);
}

// connect
extern APIDECL1 int APIDECL2 MGR_HaveDeviceConnected(t_bdaddr device_addr) ;
void rda_bt_hsg_send_connect_req(u_int8 chn_num, t_bdaddr device_addr )
{
//    u_int8 result = BT_NOERROR;
    u_int8 status;
    u_int8 i=0;
  st_t_hsp_connect_cnf ret;
#if pDEBUG
    pDebugPrintfEX((pLOGERROR,pLOGHFP,"rda_bt_hsg_send_connect_req\n"));
#endif  
if(MGR_HaveDeviceConnected(device_addr))//zhou siyou 20090826
{
#if pDEBUG
    pDebugPrintfEX((pLOGERROR,pLOGHFP,"rda_bt_hsg_send_connect_req, connect exist!! \n"));
#endif  
	ret.result = 1;
	ret.connection_id=0;
	aclState = DISCONNECTED;
	scoState = DISCONNECTED;
	
	for(i=0;i<6;i++)
	ret.bt_addr.bytes[i] = device_addr.bytes[i];
	rdabt_put_message(EXTERNAL_TASK_MASK,HSP_Q|HSG_ACL_CONNECT_CNF,&ret);
	
	return;
}
 gCurrent_Active_Profile = 2;//zhou siyou 20090719
    for(i=0;i<6;i++)
        gCur_device_addr.bytes[i]=device_addr.bytes[i];

    if(chn_num==0)
        HS_RetrieveServerChannel(device_addr);
    else
    {
#if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGHFP,"rda_bt_hsg_send_connect_req aclState=%d,scoState=%d, ring_timer=%d", aclState, scoState, ring_timer));
#endif  

        if (aclState==DISCONNECTED) 
        {
            aclState=PENDING;
            gCur_Server_channel = chn_num;
            status=RSE_CliConnectPort(device_addr, chn_num, 50,&HS_rfcommCallbacks);    
        }
    }
}
extern unsigned char is_a2dp_connect(void);

// disconnect
void rda_bt_hsg_send_disconnect_req(u_int32 cid, t_bdaddr device_addr )
{
//    u_int16 acl_handle;
    st_t_hsp_connect_cnf ret;
    int i;
    
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hsg_send_disconnect_req aclState=%d,scoState=%d, ring_timer=%d", aclState, scoState, ring_timer));
#endif    

    RSE_CliDisconnectPort(gCur_device_addr, gCur_Server_channel);
    if(!is_a2dp_connect())
        prh_mgr_releaseACL(gCur_ACL_handle, PRH_USER_ENDED_CONNECTION,NULL);// zhou siyou 090930

    ret.result = BT_NOERROR;
    ret.connection_id = gCur_ACL_handle;    
   
    for(i=0;i<6;i++)
        ret.bt_addr.bytes[i] = gCur_device_addr.bytes[i];
    
    rdabt_put_message(EXTERNAL_TASK_MASK,HSP_Q|HSG_ACL_DISCONNECT_CNF,&ret);
    gCur_ACL_handle = 0;
    for(i=0;i<6;i++)
        gCur_device_addr.bytes[i] = 0;

    gCur_Server_channel = 0;
    aclState = DISCONNECTED;
    gCur_DLCI=0;
    
}

int HS_Send_Ring()
{
    char RING_Command[10]="\r\nRING\r\n";
    pDebugPrintfEX((pLOGWARNING,pLOGHFP,"HS_Send_Ring"));
    return HS_SendToGateway((u_int8*)RING_Command,8);
}


int HS_Set_Speaker_Gain_To_HS(u_int8 gain)
{
    char VGS_Command[12]="\r\n+VGS:00\r\n";

    if (gain > 9)
    {
        VGS_Command[7] = '1'; 
        VGS_Command[8] = '0' + (gain-10);
    }
    else
    {
        VGS_Command[7] = ' ';
        VGS_Command[8] = '0' + gain;
    }

    return HS_SendToGateway((u_int8*)VGS_Command,11);
}

void rda_bt_hsg_send_set_volume_req( u_int16 connect_id,u_int8 gain)
{
     pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hsg_send_set_volume_req gain=%d\n", gain));
    HS_Set_Speaker_Gain_To_HS(gain);
}

void HS_Ring_Timeout(void *pArgs)
{
    pDebugPrintfEX((pLOGWARNING,pLOGHFP,"HS_Ring_Timeout"));
    HS_Send_Ring();
    ring_timer=pTimerCreate((unsigned int)HSP_RING_TIMEOUT, HS_Ring_Timeout,NULL,pTIMER_ONESHOT);
}

void rda_bt_hsg_send_ring(void)
{
     pDebugPrintfEX((pLOGWARNING,pLOGHFP,"rda_bt_hsg_send_ring"));
    HS_Send_Ring();
    ring_timer =pTimerCreate((unsigned int)HSP_RING_TIMEOUT, HS_Ring_Timeout,NULL,pTIMER_ONESHOT);
}


void HS_SCO_Setup_Mgr_Callback(u_int16 *scoHandle, t_api status)
{
    st_t_hsg_sco_connect_cnf ret;
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_SCO_Setup_Mgr_Callback scoState=%d, ring_timer=%d, *scoHandle=0x%x",  scoState, ring_timer,*scoHandle));
#endif   

    if (aclState == CONNECTED)
    {
        if (status != BT_NOERROR)
        {
            gCur_SCO_handle = 0;
            ret.result = 1;
            ret.connection_id=0;
        }
        else
        {
            scoState = CONNECTED;
            ret.result = BT_NOERROR;
            ret.connection_id = gCur_SCO_handle;
        }

        /* Generate Callback - to the app */
        rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_SCO_CONNECT_CNF,&ret);
    }
}

void HS_SCO_Release_Mgr_Callback(u_int16 scoHandle, t_api status)
{
	st_t_hsg_sco_connect_cnf ret;
    
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_SCO_Release_Mgr_Callback scoHandle=0x%x,scoState=%d, ring_timer=%d", scoHandle, scoState, ring_timer));
#endif   

	if( (scoState == PENDING)||(scoState == CONNECTED))
        {
            if(status == BT_NOERROR)
            {   
                /* Generate Callback - to the app */
                ret.connection_id = gCur_SCO_handle;
                ret.result = BT_NOERROR;
            }
            else
            {
                ret.result = 1;
                ret.connection_id = gCur_SCO_handle;
            }
            gDisconFlag=0;
            scoState = DISCONNECTED;
            rdabt_put_message(EXTERNAL_TASK_MASK,HFP_Q|HFG_SCO_DISCONNECT_CNF,&ret);
        }
}

int HS_Audio_Setup(u_int16 handle)
/**********************************************************************/
{
	int status = BT_NOERROR;

	// Advanced Audio setup mechanism
	//  
	// If SCO Open establish Audio immediately
	// If peer_device exists but no SLC, then setup SLC and then Audio.
	// If no peer device exists then setup do ServiceDiscover, SLC setup and Then Audio.
	//
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_Audio_Setup aclState=%d,scoState=%d, ring_timer=%d", aclState, scoState, ring_timer));
#endif    
	if(aclState == CONNECTED)
	{
        
           pTimerCancel(ring_timer);
            ring_timer =0;
            status = MGR_AddSCO(gCur_ACL_handle, &gCur_SCO_handle, 0x80, HS_SCO_Setup_Mgr_Callback);

	}
	else if (aclState == DISCONNECTED)
	{
		status = HS_RetrieveServerChannel(gCur_device_addr);
	}

	return status;

}

int HS_Audio_Release(void)
/**********************************************************************/
{
	int status;
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"HS_Audio_Release aclState=%d,scoState=%d, ring_timer=%d, gCur_SCO_handle=0x%x", aclState, scoState, ring_timer, gCur_SCO_handle));
#endif    
	if ((gDisconFlag ==1 && scoState == PENDING)  //disconnect from the hs
           || (gDisconFlag ==0 && scoState == CONNECTED))
	{
		/* Call the manager to release the SCO link */
		status = MGR_ReleaseSCO(gCur_SCO_handle,HCI_errTERMUSER, HS_SCO_Release_Mgr_Callback);
              scoState = PENDING;  
	}
	else
	{
		status = BT_INVALIDPARAM;
	}

	return status;
}

int rda_bt_hsg_send_sco_disconnect_req( u_int16 connect_id)
{
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGHFP,"rda_bt_hsg_send_sco_disconnect_req aclState=%d,scoState=%d, ring_timer=%d", aclState, scoState, ring_timer));
#endif 

    HS_Audio_Release();
    return TRUE;
}

#endif





