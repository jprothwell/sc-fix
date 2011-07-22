/***********************************************************************
 *
 * MODULE NAME:    avdtp_callbacks.c
 * DESCRIPTION:    Audio/video Distribution Transport Profile request decode
 * MAINTAINER:      Zhu Jianguo
 *
 * 
 * LICENSE:
 *     This source code is copyright (c) 2008 RDA Ltd.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/
#include "l2cap_primitives.h"
#include "host_config.h"
#include "host_types.h"
#include "host_ctrl.h"
#include "sdp_constants.h"
#include "sdp_db.h"
#include "sdp_server.h"
#include "papi.h"

#include "avrcp_core.h"
#include "avrcp.h"
#include "avrcp_callbacks.h"
#include "host_msg.h"

extern AVRCP gAVRCP;
//#ifdef COOLSAND
//avrcp_cmd_callbacks gCmd_Cbs;
//#endif
extern u_int32 rdabt_put_message(u_int16 q, u_int16 mi, void *mv);

t_api Avctp_Layer_Ctrl(u_int8 flags) 
{
	if (flags&PRH_HOST_STARTUP) 
	{
		AVRCP_RegisterPSM();
	} 
	else if (flags&PRH_HOST_RESET) 
	{
		AVRCP_RegisterPSM();
	} 
	else if (flags&PRH_HOST_SHUTDOWN) 
	{

	}
	
	return BT_NOERROR;
}



/***********************************************************************
 *
 * FUNCTION NAME:   AVRCP_RegisterPSM
 * DESCRIPTION:
 
 This function will perform some initialisation routines for the avrcp. At
the moment this is simply the registering of the callbacks.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void AVRCP_RegisterPSM(void)
{
#if 1
	l2_callbacks cbs;
	
	cbs.disconnectionIndication = Avrcp_L2cap_Disconnect_Ind_Cb;
	cbs.qosviolationIndication = Avrcp_L2cap_Qos_Violation_Ind_Cb; 
	cbs.connectionIndication = Avrcp_L2cap_Connect_Ind_Cb;
	cbs.disconnectionConfirmation = Avrcp_L2cap_Disconnect_Cfm_Cb;
	cbs.dataIndication = Avrcp_L2cap_Data_Ind_Cb;
	cbs.flushOccurred=pNULL;
	cbs.bufferOverflow=pNULL;
	cbs.timeoutIndication=pNULL;
   #if  AVRCP_TG
   	cbs.connectionConfirmation = pNULL;
   #else
	cbs.connectionConfirmation = Avrcp_L2cap_Connect_Cfm_Cb;
   #endif
	cbs.configureIndication = Avrcp_L2cap_Config_Ind_Cb;
	cbs.configureConfirmation = Avrcp_L2cap_Configure_Cfm_Cb;
	cbs.connectionConfiguredConfirmation=pNULL;//avrcp_l2cap_connect_configured_cfm_cb;
	
	L2_RegisterPSM(AVCTP_PSM,&cbs);
	#endif
}

t_AVRCP_STATE Get_Avrcp_State(void)
{
	return gAVRCP.state;
}

void Avrcp_Set_State(t_AVRCP_STATE state)
{
	gAVRCP.state = state;
}

void Avrcp_Init(void)
{
	//int status;
	/* Set the state to initialising */
	//Avrcp_Set_State(AVRCP_INITIALISING);

	/* Set the handler function */
	//gAVRCP->task.handler = avrcpProfileHandler;

	/* Init the remaining profile state */
	//gAVRCP.sink = 0;
	gAVRCP.transaction_label = 0;
	gAVRCP.pending = AVRCP_NONE;
	gAVRCP.block_received_data = 0;
	gAVRCP.fragmented = AVRCP_FRAG_NONE;
	gAVRCP.l2cap_mtu = 672; /* default l2cap mtu */

	/* Store the app task so we know where to return responses */
	//gAVRCP->clientTask = theAppTask;

	/* Store the low power mode priority of this lib instance */
	//gAVRCP->priority = config->priority;
#if AVRCP_TG
    gAVRCP.device_type = AVRCP_TARGET;
#else
	gAVRCP.device_type = AVRCP_CONTROLLER;
#endif

	/* Reset the identifier field */
	gAVRCP.identifier = 0;

	//status = sdp_init_AVRCP();
	//if(status)
	//	pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"A2DP - SDP record:AVRCP init failure"));
	Avrcp_Set_State(AVRCP_READY);

}

#if 0
int sdp_init_AVRCP(void)
{
	int status=0;
#if SDP_RECORD_GEN
	t_SDP_serviceRecord *pRecord;
	u_int32 recordHandle;

	status = SDP_CreateRecord(&recordHandle, &pRecord);
	if (status!=BT_NOERROR)
		return 1;

	{
		/********************************/
		/*** add the Service Class List */
		/********************************/
		u_int16 uuid;
		t_SDP_dataElement elem_avremotecontrolUUID;
		t_SDP_dataElement *elem_serviceUUIDList[10];
		t_SDP_dataElement elem_ServiceClassIDList;

		/* Encode the AV Remote Control uuid to a data element */
		uuid = SDP_SCLASS_UUID_AVREMOTECONTROL; 
		status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_avremotecontrolUUID);
		if (status!=BT_NOERROR)
			return 1;

		/* Add the encoded uuids to a list */
		elem_serviceUUIDList[0] = &elem_avremotecontrolUUID;
		status = SDP_CreateList(SDP_SEQUENCE, 1, elem_serviceUUIDList, &elem_ServiceClassIDList);
		if (status!=BT_NOERROR)
			return 1;
		
		status = SDP_AddAttribute(pRecord, SDP_AID_SERVICE_CLASS, &elem_ServiceClassIDList);
		if (status!=BT_NOERROR)
			return 1;
	}



	{
		/******* add the Protocol Descriptor List (PDL) 
		The PDL is a list containing two lists, one for each lower protocol 
		<PDL:  <L2CAP: UUID, PSM> <AVCTP: UUID, version> >
		*/
		t_SDP_dataElement elem_l2capProtocolInfoList;
		t_SDP_dataElement elem_avctpProtocolInfoList;
		t_SDP_dataElement elem_protocolDescriptorList;
		t_SDP_dataElement *elem_protocolDescriptorElements[10];

		{
			/**** create the L2CAP protocol information list */
			u_int16 uuid;
			t_SDP_dataElement elem_l2capUUID;
			t_SDP_dataElement elem_psm;
			t_SDP_dataElement *elem_l2capProtocolInfoElements[10];
			u_int16 psm=0;

			/* encode the l2cap protocol uuid */
			uuid = SDP_PROTOCOL_UUID_L2CAP;
			status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_l2capUUID);
			if (status!=BT_NOERROR)
				return 1;

			/* encode the psm */
			psm = 0x0017;  //AVCTP 
			status = SDP_EncodeAttribute(SDP_UINT, 2, (u_int8 *)&psm, &elem_psm);
			if (status!=BT_NOERROR)
				return 1;

			/* Add the l2cap protocol uuid to a list */
			elem_l2capProtocolInfoElements[0] = &elem_l2capUUID;
			elem_l2capProtocolInfoElements[1] = &elem_psm;
			status = SDP_CreateList(SDP_SEQUENCE, 2, elem_l2capProtocolInfoElements, &elem_l2capProtocolInfoList);
			if (status!=BT_NOERROR)
				return 1;
		}

		{
			/**** create the AVCTP protocol information list */
			u_int16 uuid;
			t_SDP_dataElement elem_avctpUUID;
			t_SDP_dataElement elem_avctpVersion;
			t_SDP_dataElement *elem_avctpProtocolInfoElements[10];
			u_int16 protocolVersion=0x0100;

			/* encode the avctp protocol uuid */
			uuid = SDP_PROTOCOL_UUID_AVCTP;
			status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_avctpUUID);
			if (status!=BT_NOERROR)
				return 1;

			/* encode the avctp version */
			status = SDP_EncodeAttribute(SDP_UINT, 2, (u_int8 *)&protocolVersion, &elem_avctpVersion);
			if (status!=BT_NOERROR)
				return 1;

			/*  Add the both of the attributes to an avctp protocol information list */
			elem_avctpProtocolInfoElements[0] = &elem_avctpUUID;
			elem_avctpProtocolInfoElements[1] = &elem_avctpVersion;
			status = SDP_CreateList(SDP_SEQUENCE, 2, elem_avctpProtocolInfoElements, &elem_avctpProtocolInfoList);
			if (status!=BT_NOERROR)
				return 1;
		}


		/* Enclose both L2CAP and AVCTP protocol information lists in an outer protocol descriptor list */
		elem_protocolDescriptorElements[0] = &elem_l2capProtocolInfoList;
		elem_protocolDescriptorElements[1] = &elem_avctpProtocolInfoList;

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
			uuid = SDP_SCLASS_UUID_AVREMOTECONTROL;
			status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_profile0UUID);
			if (status!=BT_NOERROR)
				return 1;

			/* encode the profile0 version */
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

		/* Enclose the Profile0 Descriptor List in an outer Profile Descriptor List */
		elem_ProfileDescriptorElements[0] = &elem_profile0DescriptorList;
		status = SDP_CreateList(SDP_SEQUENCE, 1, elem_ProfileDescriptorElements, &elem_ProfileDescriptorList);
		if (status!=BT_NOERROR)
			return 1;

		/* Add the the Profile Descriptor List to the record  */
		status = SDP_AddAttribute(pRecord, SDP_AID_PROFILE_LIST, &elem_ProfileDescriptorList);
		if (status!=BT_NOERROR)
			return 1;
	}


	{
		/**** add the supported features */
		t_SDP_dataElement elem_SupportedFeatures;
		u_int16 supportedFeatures = 0x0001;   //only category 1 supported.
 
		/*  Encode the supported features */
		status = SDP_EncodeAttribute(SDP_UINT, 2, (u_int8 *)&supportedFeatures, &elem_SupportedFeatures);
		if (status!=BT_NOERROR)
			return 1;

		/* Add the the Profile0 Descriptor List to the record  */
		status = SDP_AddAttribute(pRecord, SDP_AID_SUPPORTED_FEATURES, &elem_SupportedFeatures);
		if (status!=BT_NOERROR)
			return 1;
	}
#endif

	return 0; /* success */
}
#endif

/*****************************************************************
	Returns the next transaction label. This is used to route responses from 
	the remote device to the correct local initiator. Note that zero will 
	never be issued and hence can be used as a special case to indicate no 
	command is pending.
******************************************************************/
static u_int8 getNextTransactionLabel()
{
	gAVRCP.transaction_label = (gAVRCP.transaction_label + 1) & 0xf;

	/* 
		We treat a label of zero as a special case indicating there is no 
		transaction pending. 
	*/
	if (gAVRCP.transaction_label==0) 
		gAVRCP.transaction_label = 1;
	
	return gAVRCP.transaction_label;
}

/****************************************************************************
NAME	
	AvrcpPassthrough

DESCRIPTION
	This function is called to request that a Pass Through control command
	is sent to the target on the the connection identified by the sink.
	
	The Passthrough command is used to convey the proper user operation to 
	the target(transparently to the user).
	
	
	identifier 				- Used to identify the matching AV/C Command 
	subunit_type
	subunit_id			- AV/C protocol - Used to form the targets address
	opid					- Identifies the button pressed
	state				- Indicates the user action of pressing or releasing the button identified by opid.  Active low.
	operation_data		- Required for the Vendor Unique operation.  For other 
	operation_dataLlength 	      operations both fields should be zero
	
					 -----------------------------------------------
					| MSB |		|	  |		|	  |		|	  |	LSB |
					|-----------------------------------------------|
	opcode			|		      PASSTHROUGH (0x7C) 				|
					 -----------------------------------------------
	operand(0)		|state|			operation_id					|
	 				 -----------------------------------------------
	operand(1)		|  		operation data field length				|
					 -----------------------------------------------
	operand(2)		|	 operation data(operation id dependant)		|
		:			|												|
					 -----------------------------------------------

MESSAGE RETURNED
	AVRCP_PASSTHROUGH_CFM 
*****************************************************************************/

static void Avrcp_Passthrough(t_AVC_SUBUNIT_TYPE subunit_type, u_int16 subunit_id, u_int8 state, t_AVC_OPERATION_ID opid)
{
	u_int8 transaction_label;
	//u_int8 req[8];
	//struct st_t_dataBuf h_buff;
#if ((pDEBUG==0)&&(PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
	u_int8 temp[2];
#endif
	t_DataBuf *h_buff;
	//h_buff.buf = req;
	u_int32 ret;
	ret = L2_GetWriteBuffer(gAVRCP.cid, 8, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}
	
	if (subunit_type > 0x1F)
	{
		//pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"Out of range subunit type  0x%x\n", subunit_type));
#if pDEBUG
		pDebugPrintfEX((pLOGERROR,pLOGA2DP,"AVRCP - Out of range subunit type  0x%x\n", subunit_type));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
              temp[0]=subunit_type;   
              SYSDebug_PrintEx(pLOGDEBUG,pLOGA2DP, LOG_A2TP_AVRCP_CORE_INDEX, temp, 1,BT_CODE_BYTE); 
#endif
	}
	
	if (subunit_id > 0x07)
	{
		//pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"Out of range subunit id  0x%x\n", subunit_id));

#if pDEBUG         
		pDebugPrintfEX((pLOGERROR,pLOGA2DP,"AVRCP - Out of range subunit id  0x%x\n", subunit_id));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
              BT_WRITE_LE_UINT16(temp,subunit_id);
              SYSDebug_PrintEx(pLOGDEBUG,pLOGA2DP, (LOG_A2TP_AVRCP_CORE_INDEX+1), temp, 2,BT_CODE_WORD); 
#endif
        
	}
	
	if (opid > 0x7F)
	{
#if pDEBUG
		pDebugPrintfEX((pLOGERROR,pLOGA2DP,"AVRCP - Out of range op id  0x%x\n", opid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
              temp[0]=opid;   
              SYSDebug_PrintEx(pLOGDEBUG,pLOGA2DP, (LOG_A2TP_AVRCP_CORE_INDEX+2), temp, 1,BT_CODE_BYTE);   
#endif
	}

	if(gAVRCP.state != AVRCP_CONNECTED)
		return;
	
	transaction_label = getNextTransactionLabel();

	/* AVCTP header */
	h_buff->buf[0] = ((transaction_label<< AVCTP0_TRANSACTION_SHIFT) | AVCTP0_PACKET_TYPE_SINGLE | AVCTP0_CR_COMMAND);
	h_buff->buf[1] = AVCTP1_PROFILE_AVRCP_HIGH;
	h_buff->buf[2] = AVCTP2_PROFILE_AVRCP_LOW;

	/* AVRCP header */
	h_buff->buf[3] = AVRCP0_CTYPE_CONTROL;
	h_buff->buf[4] = ((subunit_type<< AVRCP1_SUBUNIT_TYPE_SHIFT) & AVRCP1_SUBUNIT_TYPE_MASK) | (subunit_id & AVRCP1_SUBUNIT_ID_MASK);
	h_buff->buf[5] = AVRCP2_PASSTHROUGH;

	/* Pass-through */
	h_buff->buf[6] = opid | (state ? AVRCP3_PASSTHROUGH_STATE_MASK : 0);
	h_buff->buf[7] = 0;

	h_buff->dataLen = 8;
	gAVRCP.pending = AVRCP_PASSTHROUGH;

	//L2_Write(gAVCTP_cid, &h_buff);
	L2_FreeWriteBuffer(h_buff);
}


static void Headset_Pause_Press(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 0, OPID_PAUSE);
}

static void Headset_Pause_Release(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 1, OPID_PAUSE);
}

static void Headset_Play_Press(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 0, OPID_PLAY);
}

static void Headset_Play_Release(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 1, OPID_PLAY);
}

static void Headset_Stop_Press(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 0, OPID_STOP);
}

static void Headset_Stop_Release(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 1, OPID_STOP);
}

static void Headset_Forward_Press(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 0, OPID_FORWARD);
}

static void Headset_Forward_Release(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 1, OPID_FORWARD);
}

static void Headset_Backward_Press(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 0, OPID_BACKWARD);
}

static void Headset_Backward_Release(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 1, OPID_BACKWARD);
}

static void Headset_Fast_Forward_Press(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 0, OPID_FAST_FORWARD);
}

static void Headset_Fast_Forward_Release(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 1, OPID_FAST_FORWARD);
}

static void Headset_Rewind_Press(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 0, OPID_REWIND);
}

static void Headset_Rewind_Release(void)
{
	Avrcp_Passthrough(SUBUNIT_PANEL, 0, 1, OPID_REWIND);
}

/***********************************************************************
 *
 * FUNCTION NAME:   Avrcp_Connect_Req
 * DESCRIPTION:
 
	This function was called to create a avrcp connection.

 * KNOWN ISSUES:
	none.
 ***********************************************************************/
void Avrcp_Connect_Req(t_bdaddr bdaddr)
{
	t_api status;
	if(gAVRCP.state !=AVRCP_READY)
		return;
		
	Avrcp_Set_State(AVRCP_CONNECTING);

	status=L2_Connect(bdaddr, AVCTP_PSM);
	if(status==BT_NOERROR)
		 Avrcp_Set_State(AVRCP_CONNECTED);
}

void Headset_Send_Pause(void)
{
	if (Get_Avrcp_State()==AVRCP_CONNECTED)
	{
		Headset_Pause_Press();
		Headset_Pause_Release();
	}
}

void Headset_Send_Play(void)
{
	if (Get_Avrcp_State()==AVRCP_CONNECTED)
	{
		Headset_Play_Press();
		Headset_Play_Release();
	}
}

void Headset_Send_Stop(void)
{
	if (Get_Avrcp_State()==AVRCP_CONNECTED)
	{
		Headset_Stop_Press();
		Headset_Stop_Release();
	}
}

void Headset_Send_Forward(void)
{
	if (Get_Avrcp_State()==AVRCP_CONNECTED)
	{
		Headset_Forward_Press();
		Headset_Forward_Release();
	}
}

void Headset_Send_Backward(void)
{
	if (Get_Avrcp_State()==AVRCP_CONNECTED)
	{
		Headset_Backward_Press();
		Headset_Backward_Release();
	}
}

void Headset_Send_Fast_Forward(void)
{
	if (Get_Avrcp_State()==AVRCP_CONNECTED)
	{
		Headset_Fast_Forward_Press();
		Headset_Fast_Forward_Release();
	}
}

void Headset_Send_Rewind(void)
{
	if (Get_Avrcp_State()==AVRCP_CONNECTED)
	{
		Headset_Rewind_Press();
		Headset_Rewind_Release();
	}
}

/****************************************************************************
NAME	
	Avrcp_Process_Response

DESCRIPTION
	This function is called to process an AVRCP Response packet
*****************************************************************************/
static void Avrcp_Process_Response(const u_int8 *ptr)
{
	gAVRCP.pending= AVRCP_NONE;
}

/****************************************************************************
NAME	
	avrcpSendResponse
	
DESCRIPTION
	This function is used to send a response back to the CT device.
	Currently doesn't handle a packet size > max_mtu as the received 
	packet should always be <= max_mtu, and split into a start, continue 
	and end packet if neccessary.
****************************************************************************/
static void Avrcp_Send_Response(const u_int8 *ptr, u_int16 packet_size, t_AVRCP_RESPONSE_TYPE response)
{
#ifdef AVRCP_TG

	t_DataBuf *h_buff;
       u_int8 status=BT_NOERROR;
	status = L2_GetWriteBuffer(gAVRCP.cid, 48, &h_buff); 

   if (status!=BT_NOERROR || h_buff == NULL)
    {
#if pDEBUG
       pDebugPrintf((pLOGERROR,"Avrcp_Send_Response : Error - Memory could not be allocated for L2CAP Packet. status =%d\n",status));
#endif  
        return ; //insufficent resource
    }

	/* Copy the original message */
	pMemcpy(h_buff->buf, (void*)ptr, packet_size);

	/* Translate our bad profile error into a general error with the invalid PID bit set. */
	if (response == AVCTP_RESPONSE_BAD_PROFILE)
	{
		h_buff->buf[0] |= AVCTP0_IPID;
		response = AVCTP_RESPONSE_NOT_IMPLEMENTED;
	}

	/* flip CR bit */
	//h_buff->buf[0] ^= AVCTP0_CR_MASK;

	/* Overwrite command type with response */
	if ((ptr[0] & AVCTP0_PACKET_TYPE_MASK) == AVCTP0_PACKET_TYPE_START)
		h_buff->buf[4] = response;
	else if (((ptr[0] & AVCTP0_PACKET_TYPE_MASK) == AVCTP0_PACKET_TYPE_CONTINUE) ||
		((ptr[0] & AVCTP0_PACKET_TYPE_MASK) == AVCTP0_PACKET_TYPE_END))
		h_buff->buf[1] = response;
	else
		h_buff->buf[3] = response;

	h_buff->dataLen =  packet_size;

        L2_Write(gAVRCP.cid, h_buff);
	
#endif
}

/****************************************************************************
NAME	
	Avrcp_Process_Command

DESCRIPTION
	This function is called to process an AVRCP Command packet
****************************************************************************/
static void Avrcp_Process_Command(const u_int8 *ptr, t_channelId cid, u_int16 packet_size)
{
#ifdef AVRCP_TG
    //t_DataBuf *h_buff;

    //L2_GetWriteBuffer(gAVRCP.cid, 10, &h_buff);
    //pDebugPrintfEX((pLOGDEBUG, pLOGA2DP,"Avrcp_Process_Command packet_size=%d cid=0x%x,  gAVRCP.chnl_num=%d\n",packet_size, cid, gAVRCP.chnl_num));
	
	if ((ptr[3] == AVRCP0_CTYPE_CONTROL) && (ptr[5] == AVRCP2_PASSTHROUGH))
	{
		/* Check packet has a valid header and payload if any */
		if ((packet_size < (AVRCP_TOTAL_HEADER_SIZE + 2)) ||
			(packet_size < (AVRCP_TOTAL_HEADER_SIZE + ptr[7])))
		{
			/* Reject the response - invalid header or payload */
			Avrcp_Send_Response(ptr, packet_size, AVCTP_RESPONSE_REJECTED);
		}
		else
		{
			/* Any reply must contain the original message so store it with the instance data */
                    /*pMemcpy(h_buff->buf, ptr, packet_size);
                    h_buff->buf[0] |= ((AVRCP_TYPE_RESPONSE<<1) |AVRCP_MESSAGE_VALID);
                    h_buff->buf[3] |= AVCTP_RESPONSE_ACCEPTED;
                    h_buff->dataLen= packet_size;
                    L2_Write(gAVRCP.cid, h_buff);
                    */
#ifdef COOLSAND                
                    t_avrcp_cmd_frame_ind cmd_frame;
                    cmd_frame.chnl_num = gAVRCP.chnl_num;
                    cmd_frame.seq_id = (ptr[0]>>4) & 0x0F;
                    cmd_frame.c_type = (ptr[3] & 0x0F);
                    cmd_frame.subunit_type = (ptr[4]>>3)& 0x1F;
                    cmd_frame.subunit_id = ptr[4]&0x7;
                    cmd_frame.data_len = 3;
                    cmd_frame.profile_id = (u_int16)(ptr[1]<<8 | ptr[2]);
                    pMemcpy(cmd_frame.frame_data, (void*)&ptr[5], 3);

                    rdabt_put_message(EXTERNAL_TASK_MASK,AVRCP_Q|AVRCP_CMD_FRAME_IND,&cmd_frame);
#endif
                    return;

		}
	}
	else if ((ptr[3] == AVRCP0_CTYPE_STATUS) && (ptr[5] == AVRCP2_UNITINFO))
	{
		/* Check packet has a valid header and payload if any */
		if (packet_size < (AVRCP_TOTAL_HEADER_SIZE + 5))
		{
			Avrcp_Send_Response(ptr, packet_size, AVCTP_RESPONSE_REJECTED);
			return;
		}
		else
		{
			/* Any reply must contain the original message so store it with the instance data */
			//L2_GetWriteBuffer(gAVRCP.cid, 48, &h_buff); 
			//pMemcpy(h_buff->buf, ptr, packet_size);
			//To do later!!!!!!!!!!!! 
		}
	}
	else if ((ptr[3] == AVRCP0_CTYPE_STATUS) && (ptr[5] == AVRCP2_SUBUNITINFO))
	{
		/* Check packet has a valid header and payload if any */
		if (packet_size < (AVRCP_TOTAL_HEADER_SIZE + 5))
		{
			Avrcp_Send_Response( ptr, packet_size, AVCTP_RESPONSE_REJECTED);
		}
		else
		{
			/* 	Any reply must contain the original message. Store the msg 
				while we wait for the response from the client.	*/
				//L2_GetWriteBuffer(gAVRCP.cid, 48, &h_buff); 
				//pMemcpy(h_buff->buf, ptr, packet_size);
				//To do later!!!!!!!!!!!! 
		}
	}
	else
	{
		/* Reject this its an unknown command */
		Avrcp_Send_Response( ptr, packet_size, AVCTP_RESPONSE_NOT_IMPLEMENTED);
	}
#endif
}

/****************************************************************************
NAME	
	Avrcp_Handle_Received_Data

DESCRIPTION
	This function is called to process data received over the L2cap connection
****************************************************************************/
void Avrcp_Handle_Received_Data(u_int8* ptr,t_channelId cid, u_int16 packet_size)
{
	//Packet is too small to be valid - ignore!
		   pDebugPrintfEX((pLOGERROR, pLOGA2DP,"Avrcp_Handle_Received_Data packet_size=%d cid=0x%x",packet_size, cid));

	if(packet_size<=6)
		return;

	if ((ptr[0] & AVCTP0_PACKET_TYPE_MASK) == AVCTP0_PACKET_TYPE_SINGLE)
	{
		if ((ptr[0] & AVCTP0_CR_MASK) == AVCTP0_CR_COMMAND)
		{
			/* which profile? */
			if ((ptr[1] == AVCTP1_PROFILE_AVRCP_HIGH) &&
				(ptr[2] == AVCTP2_PROFILE_AVRCP_LOW))
			{
				if (ptr[5] == AVRCP2_VENDORDEPENDENT)
				{
					//Avrcp_Handle_Vendor_Message(ptr, packet_size);  //
				}
				else
					Avrcp_Process_Command(ptr,cid, packet_size);
			}
			else
				Avrcp_Send_Response( ptr, packet_size, AVCTP_RESPONSE_BAD_PROFILE);
		}
		else  //get the response
		{
			/* which profile? */
			if ((ptr[1] == AVCTP1_PROFILE_AVRCP_HIGH) &&
				(ptr[2] == AVCTP2_PROFILE_AVRCP_LOW))
				Avrcp_Process_Response(ptr);
			/* else unknown profile - ignore */
		}
	}
	//else  //fragment, to do later!!!!!!
	
}

#ifdef COOLSAND
// activate
void rda_bt_avrcp_send_activate_req(u_int8 chnl_num,u_int8 local_role )
{
    u_int8 result[2];
    
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_avrcp_send_activate_req  local_role=%d, chnl_num=%d\n", local_role, chnl_num));
    
    Avrcp_Init();

    result[0] = BT_NOERROR;
    result[1] = chnl_num;
    gAVRCP.chnl_num = chnl_num;

    rdabt_put_message(EXTERNAL_TASK_MASK,AVRCP_Q|AVRCP_ACTIVATE_CNF,&result);
}

// deactivate
void rda_bt_avrcp_send_deactivate_req(u_int8 chnl_num )
{
    u_int8 result[2];
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_avrcp_send_deactivate_req"));
    Avrcp_Set_State(AVRCP_INITIALISING);
    
    result[0] = BT_NOERROR;
    result[1] = chnl_num;
    
    rdabt_put_message(EXTERNAL_TASK_MASK,AVRCP_Q|AVRCP_DEACTIVATE_CNF,&result);
}

// connect
void rda_bt_avrcp_send_connect_req( u_int8 chnl_num, t_bdaddr device_addr, u_int8 local_role )
{
    int status;

    //Avrcp_Set_Connect_Cfm_Callback(chnl_num, local_role);
    gAVRCP.chnl_num=chnl_num;
    gAVRCP.local_role = local_role;   
    status=L2_Connect(device_addr, AVCTP_PSM);
}

// disconnect
void rda_bt_avrcp_send_disconnect_req(u_int8 chnl_num/*, void (*callbackFunc)(void *pArgs)*/)
{
    int status;
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_avrcp_send_disconnect_req chnl_num=%d,gAVRCP.cid:0x%x",chnl_num,gAVRCP.cid));
    status = L2_Disconnect(gAVRCP.cid);
}

//command frame
void rda_bt_avrcp_send_cmd_frame_req(u_int8 chnl_num, u_int8 seq_id, u_int8 c_type, u_int8 subunit_type, u_int8 subunit_id, u_int16 data_len, u_int8* frame_data)
{
    //Avrcp_Set_Cmd_Frame_Cnf_Callback(callbackFunc);
}

void rda_bt_avrcp_send_cmd_frame_ind_res(u_int8 chnl_num, u_int8 seq_id, u_int16 profile_id, u_int8 result, u_int8 subunit_type, u_int8 subunit_id, u_int16 data_len, u_int8* frame_data)
{
    u_int8 data[48];
    u_int16 packet_size =5+data_len;

    data[0] = (seq_id<<4) |(AVCTP0_PACKET_TYPE_SINGLE<<2) |(AVRCP_TYPE_RESPONSE<<1) | AVRCP_MESSAGE_VALID;
    data[1] = (u_int8)((profile_id >>8) &0xFF);
    data[2] = (u_int8)(profile_id  & 0xFF);
    data[3] = result;
    data[4] = (subunit_type<<3) |subunit_id;
    pMemcpy(&data[5], frame_data, data_len);
    
    Avrcp_Send_Response(data, packet_size, result);
}
    

#endif

