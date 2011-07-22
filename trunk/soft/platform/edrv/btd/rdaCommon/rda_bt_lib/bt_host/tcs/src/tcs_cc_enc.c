/*
 * MODULE NAME:    tcs_cc_enc.c
 * DESCRIPTION:    TCS Call Control Encoder
 * AUTHOR:         Thomas Howley
 * DATE:           05 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_enc.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "tcs_cc_enc.h"
#include "tcs_cc_types.h"
#include "tcs_const.h"
#include "host_buf.h"
#include "papi.h"


/* Local Functions */
t_api calculate_tcs_msg_buffer_size(struct st_t_TCS_callParams *callParams, u_int16 bitMask, u_int8 *size);
struct host_buf *get_tcs_msg_buffer(u_int16 msgSize, u_int8 msgType, u_int16 cid);

t_api enode_information_elements(u_int16 bitMask, struct st_t_TCS_callParams *callParams, u_int8 **pdu);

void encode_infoElement_callClass(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);
void encode_infoElement_sendingComplete(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);
void encode_infoElement_bearerCapability(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);
void encode_infoElement_cause(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);
void encode_infoElement_progressIndicator(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);
void encode_infoElement_signal(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);
void encode_infoElement_keypadFacility(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);
void encode_infoElement_callingPartyNumber(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);
void encode_infoElement_calledPartyNumber(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);
void encode_infoElement_audioControl(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);
void encode_infoElement_scoHandle(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);
void encode_infoElement_destCID(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);
void encode_infoElement_companySpecific(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref);

#define PRH_TCS_CC_SIZE_INFOELEMENTLIST 13


u_int8 infoElementIndexToSize[PRH_TCS_CC_SIZE_INFOELEMENTLIST] = {
	PRH_TCS_IE_SIZE_CALLCLASS,
	PRH_TCS_IE_SIZE_SENDINGCOMPLETE,
	PRH_TCS_IE_VL_SIZE_HEADER + PRH_TCS_IE_BASESIZE_BEARERCAPABILITY,
	PRH_TCS_IE_SIZE_CAUSE,
	PRH_TCS_IE_SIZE_PROGRESSINDICATOR,
	PRH_TCS_IE_SIZE_SIGNAL,
	PRH_TCS_IE_SIZE_KEYPADFACILITY,
	PRH_TCS_IE_VL_SIZE_HEADER + PRH_TCS_IE_BASESIZE_CALLINGPARTYNUMBER,
	PRH_TCS_IE_VL_SIZE_HEADER + PRH_TCS_IE_BASESIZE_CALLEDPARTYNUMBER,
	PRH_TCS_IE_VL_SIZE_HEADER,
	PRH_TCS_IE_SIZE_SCOHANDLE,
	PRH_TCS_IE_SIZE_DESTCID,
	PRH_TCS_IE_VL_SIZE_HEADER + PRH_TCS_IE_BASESIZE_COMPANYSPECIFIC
};


void (*infoElementIndexToEncodeFunction[])(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref) = {
	encode_infoElement_callClass,
	encode_infoElement_sendingComplete,
	encode_infoElement_bearerCapability,
	encode_infoElement_cause,
	encode_infoElement_progressIndicator,
	encode_infoElement_signal,
	encode_infoElement_keypadFacility,
	encode_infoElement_callingPartyNumber,
	encode_infoElement_calledPartyNumber,
	encode_infoElement_audioControl,
	encode_infoElement_scoHandle,
	encode_infoElement_destCID,
	encode_infoElement_companySpecific
};


u_int16 tcsMsgToParamBitMask[] = {
	(u_int16)PRH_TCS_CC_MSG_ALERTING_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_CALLPROCEEDING_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_CONNECT_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_CONNECTACKNOWLEDGE_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_PROGRESS_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_SETUP_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_SETUPACKNOWLEDGE_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_DISCONNECT_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_RELEASE_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_RELEASECOMPLETE_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_INFORMATION_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_STARTDTMF_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_STARTDTMFACKNOWLEDGE_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_STARTDTMFREJECT_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_STOPDTMF_BITMASK,
	(u_int16)PRH_TCS_CC_MSG_STOPDTMFACKNOWLEDGE_BITMASK
};


t_api prh_tcs_cc_encodeMsg(u_int8 tcsMsg, struct st_t_TCS_callParams *callParams, u_int16 cid)
{
	u_int16 finalBitMask;
	struct host_buf* h_buff;
	u_int8 *pdu;
	u_int8 msgSize;
	t_api status = BT_NOERROR;
	u_int8 index;

    
    if(callParams)
    {
	    if(PRH_TCS_CC_ILLEGAL_BITMASK(callParams->bitMask) || ((tcsMsg & PRH_TCS_CC_MSG_MASK) > PRH_TCS_MSG_STOP_DTMF_ACK))
		    return BT_INVALIDPARAM;

	    index = (tcsMsg <= PRH_TCS_MSG_INFORMATION)?tcsMsg:tcsMsg-5;
	    /* Need to calculate the "FINAL" bit mask first, get rid of bits(parameters) set unnecessarily by user */
	    finalBitMask = tcsMsgToParamBitMask[index] & callParams->bitMask;

	    if(BT_NOERROR != (status = calculate_tcs_msg_buffer_size(callParams, finalBitMask, &msgSize)))
		    return status;

	    h_buff = get_tcs_msg_buffer(msgSize, tcsMsg, cid);
	    if(!h_buff)
		    return BT_NORESOURCES;

	    pdu = (h_buff->data)+1;
	    /* Encode each information element according to the calculated finalBitMask & parameter structre */
	    if(BT_NOERROR != (status = enode_information_elements(finalBitMask, callParams, &pdu)))
		    return status;

	    status = L2_Write(cid, (t_DataBuf*)h_buff);
	    
	    return status;
    }
    else
        return BT_INVALIDPARAM;
}



t_api enode_information_elements(u_int16 bitMask, struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 currentBitPos = 0;
	u_int16 bitMaskCheck = 1;
	u_int16 encodeBitMask = bitMask;

	while(encodeBitMask)
	{
		if( ((bitMaskCheck & encodeBitMask)>>currentBitPos) )
		{
			infoElementIndexToEncodeFunction[currentBitPos](callParams, pdu_ref);
			encodeBitMask ^= bitMaskCheck;
		}
		bitMaskCheck <<= 1;
		currentBitPos++;
	}

	return BT_NOERROR;
}


void encode_infoElement_callClass(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 *pdu = *pdu_ref;

	pdu[0] = PRH_TCS_IE_DO_CALL_CLASS;
	pdu[1] = callParams->callClass;
	(*pdu_ref)+= PRH_TCS_IE_SIZE_CALLCLASS;
}

void encode_infoElement_sendingComplete(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	(*pdu_ref)[0] = PRH_TCS_IE_SO_SENDING_COMPLETE;
	(*pdu_ref)+= PRH_TCS_IE_SIZE_SENDINGCOMPLETE;
}	


void encode_infoElement_bearerCapability(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 *pdu = *pdu_ref;

	pdu[0] = PRH_TCS_IE_VL_BEARER_CAP;
	pdu[1] = PRH_TCS_IE_BASESIZE_BEARERCAPABILITY;
	pdu[2] = callParams->linkType;
	
	if(PRH_TCS_BEARER_CAP_LINK_TYPE_ACL == callParams->linkType)
	{
		pdu[1]+= PRH_TCS_IE_SIZE_ACLBEARERCAPABILITY;
		if(callParams->aclBearer)
		{
			pdu[3] = callParams->aclBearer->qos.flags;
			pdu[4] = callParams->aclBearer->qos.serviceType;
			BT_WRITE_LE_UINT32(pdu+5, callParams->aclBearer->qos.tokenRate);
			BT_WRITE_LE_UINT32(pdu+9, callParams->aclBearer->qos.bucketSize);
			BT_WRITE_LE_UINT32(pdu+13, callParams->aclBearer->qos.peakBandwidth);
			BT_WRITE_LE_UINT32(pdu+17, callParams->aclBearer->qos.latency);
			BT_WRITE_LE_UINT32(pdu+21, callParams->aclBearer->qos.delayVariation);
			pdu[25] = callParams->aclBearer->layerInfo;
			(*pdu_ref)+=  PRH_TCS_IE_VL_SIZE_HEADER + PRH_TCS_IE_BASESIZE_BEARERCAPABILITY + PRH_TCS_IE_SIZE_ACLBEARERCAPABILITY;
		}
		else
			(*pdu_ref)+=  PRH_TCS_IE_VL_SIZE_HEADER + PRH_TCS_IE_BASESIZE_BEARERCAPABILITY;
		
		
	}
	else if(PRH_TCS_BEARER_CAP_LINK_TYPE_SCO == callParams->linkType)
	{
		pdu[1]+= PRH_TCS_IE_SIZE_SCOBEARERCAPABILITY;
		pdu[3] = callParams->scoPacketType;
		(*pdu_ref)+= PRH_TCS_IE_VL_SIZE_HEADER + PRH_TCS_IE_BASESIZE_BEARERCAPABILITY + PRH_TCS_IE_SIZE_SCOBEARERCAPABILITY;
	}
	else if(PRH_TCS_BEARER_CAP_LINK_TYPE_NONE == callParams->linkType)
	{
		(*pdu_ref)+= PRH_TCS_IE_VL_SIZE_HEADER + PRH_TCS_IE_BASESIZE_BEARERCAPABILITY;
	}


}


void encode_infoElement_cause(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 *pdu = *pdu_ref;

	pdu[0] = PRH_TCS_IE_DO_CAUSE;
	pdu[1] = callParams->cause;
	(*pdu_ref)+= PRH_TCS_IE_SIZE_CAUSE;
}


void encode_infoElement_progressIndicator(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 *pdu = *pdu_ref;

	pdu[0] = PRH_TCS_IE_DO_PROGRESS_INDICATOR;
	pdu[1] = callParams->progressIndicator;
	(*pdu_ref)+= PRH_TCS_IE_SIZE_PROGRESSINDICATOR;
}


void encode_infoElement_signal(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 *pdu = *pdu_ref;

	pdu[0] = PRH_TCS_IE_DO_SIGNAL;
	pdu[1] = callParams->signal;
	(*pdu_ref)+= PRH_TCS_IE_SIZE_SIGNAL;
}


void encode_infoElement_keypadFacility(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 *pdu = *pdu_ref;

	pdu[0] = PRH_TCS_IE_DO_KEYPAD_FACILITY;
	pdu[1] = callParams->keypadFacility;
	(*pdu_ref)+= PRH_TCS_IE_SIZE_KEYPADFACILITY;
}

void encode_infoElement_callingPartyNumber(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 *pdu = *pdu_ref;

	pdu[0] = PRH_TCS_IE_VL_CALLING_PARTY_NUM;
	pdu[1] = PRH_TCS_IE_BASESIZE_CALLINGPARTYNUMBER + callParams->callingPartyNumberLength;
	pdu[2] = callParams->callingPartyNumberType;
	pdu[3] = callParams->callingPartyPresScreenIndicator;
	pMemcpy((pdu+4), callParams->callingPartyNumber, callParams->callingPartyNumberLength);
	(*pdu_ref)+= PRH_TCS_IE_VL_SIZE_HEADER + PRH_TCS_IE_BASESIZE_CALLINGPARTYNUMBER +  callParams->callingPartyNumberLength;
}


void encode_infoElement_calledPartyNumber(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 *pdu = *pdu_ref;

	pdu[0] = PRH_TCS_IE_VL_CALLED_PARTY_NUM;
	pdu[1] = PRH_TCS_IE_BASESIZE_CALLEDPARTYNUMBER + callParams->calledPartyNumberLength;
	pdu[2] = callParams->calledPartyNumberType;
	pMemcpy((pdu+3), callParams->calledPartyNumber, callParams->calledPartyNumberLength);
	(*pdu_ref)+= PRH_TCS_IE_VL_SIZE_HEADER + PRH_TCS_IE_BASESIZE_CALLEDPARTYNUMBER + callParams->calledPartyNumberLength;
}


void encode_infoElement_audioControl(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 *pdu = *pdu_ref;

	pdu[0] = PRH_TCS_IE_VL_AUDIO_CONTROL;
	pdu[1] = callParams->audioControlInfoLength;
	pMemcpy((pdu+2), callParams->audioControlInfo, callParams->audioControlInfoLength);
	(*pdu_ref)+= PRH_TCS_IE_VL_SIZE_HEADER + callParams->audioControlInfoLength;
}


void encode_infoElement_scoHandle(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 *pdu = *pdu_ref;

	pdu[0] = PRH_TCS_IE_DO_SCO_HANDLE;
	pdu[1] = (u_int8)(callParams->scoHandle);
	(*pdu_ref)+= PRH_TCS_IE_SIZE_SCOHANDLE;
}

void encode_infoElement_destCID(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 *pdu = *pdu_ref;

	pdu[0] = PRH_TCS_IE_VL_DEST_CID;
	pdu[1] = PRH_TCS_IE_SIZE_DESTCID - PRH_TCS_IE_VL_SIZE_HEADER;
	BT_WRITE_LE_UINT16((pdu+2), callParams->cid);
	(*pdu_ref)+= PRH_TCS_IE_SIZE_DESTCID;
}


void encode_infoElement_companySpecific(struct st_t_TCS_callParams *callParams, u_int8 **pdu_ref)
{
	u_int8 *pdu = *pdu_ref;
	
	pdu[0] = PRH_TCS_IE_VL_COMPANY_SPECIFIC;
	pdu[1] = PRH_TCS_IE_BASESIZE_COMPANYSPECIFIC + callParams->companySpecificLength;
	BT_WRITE_LE_UINT16((pdu+2), callParams->companyID);
	pMemcpy((pdu+4), callParams->companySpecific, callParams->companySpecificLength);
	(*pdu_ref)+= PRH_TCS_IE_VL_SIZE_HEADER + PRH_TCS_IE_BASESIZE_COMPANYSPECIFIC + callParams->companySpecificLength;
}




t_api calculate_tcs_msg_buffer_size(struct st_t_TCS_callParams *callParams, u_int16 bitMask, u_int8 *size)
{
	u_int8 currentBitPos;
	u_int16 bitMaskCheck = 1;


	*size = PRH_TCS_CC_MSG_SIZE_HEADER;
	for(currentBitPos=0; currentBitPos<PRH_TCS_CC_SIZE_INFOELEMENTLIST; currentBitPos++)
	{
		if( ((bitMaskCheck & bitMask)>>currentBitPos) )
			(*size)+= infoElementIndexToSize[currentBitPos];
		bitMaskCheck<<= 1;
	}

	/* Variable Length Information Elements */
	if(PRH_TCS_CC_PARAM_BEARERCAPABILITY_EXISTS(bitMask))
	{
		if(PRH_TCS_BEARER_CAP_LINK_TYPE_ACL == callParams->linkType)
			(*size)+= PRH_TCS_IE_SIZE_ACLBEARERCAPABILITY;
		else if(PRH_TCS_BEARER_CAP_LINK_TYPE_SCO == callParams->linkType)
			(*size)+= PRH_TCS_IE_SIZE_SCOBEARERCAPABILITY;
		else if(PRH_TCS_BEARER_CAP_LINK_TYPE_NONE != callParams->linkType)
			return BT_INVALIDPARAM;
	}
	if(PRH_TCS_CC_PARAM_CALLINGPARTYNUMBER_EXISTS(bitMask))
		*size += callParams->callingPartyNumberLength; 
	if(PRH_TCS_CC_PARAM_CALLEDPARTYNUMBER_EXISTS(bitMask))
		*size += callParams->calledPartyNumberLength; 
	if(PRH_TCS_CC_PARAM_AUDIOCONTROL_EXISTS(bitMask))
		*size += callParams->audioControlInfoLength;
	if(PRH_TCS_CC_PARAM_COMPANYSPECIFIC_EXISTS(bitMask))
		*size += callParams->companySpecificLength;
	
	return BT_NOERROR;
}




struct host_buf *get_tcs_msg_buffer(u_int16 msgSize, u_int8 msgType, u_int16 cid)
{
	struct host_buf* h_buff;
	int ret;
	
    h_buff=NULL;
	ret=prh_l2_GetWriteBuffer(cid,  msgSize, 0, &h_buff);
	if (ret==BT_NOERROR)
	  {
	    h_buff->data[0]=msgType;
	    h_buff->len=msgSize;
	  }
	return h_buff;
}
#endif /* (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/
