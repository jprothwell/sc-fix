/*
 * MODULE NAME:    tcs_cc_dec.c
 * DESCRIPTION:    TCS Call Control Decoder
 * AUTHOR:         Thomas Howley
 * DATE:           07 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_dec.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "tcs_cc_dec.h"
#include "tcs_const.h"
#include "tcs_cc_types.h"

#include "papi.h"


t_api decode_infoElementBearerCapability(u_int8* pdu, u_int8 numBytesLeft, struct st_t_TCS_callParams *callParams, u_int8 *size);
t_api store_partyNumber(u_int8* pdu, u_int8 numNewDigits, u_int8 **number_ref);

t_api prh_tcs_cc_decodeMsg(struct host_buf* h_buff, struct st_t_TCS_callParams *callParams)
{
	u_int8 *pdu =  (h_buff->data)+1;
	u_int8 numInfoElementBytes =0;
	u_int8 totalNumBytesRead = 1;

	callParams->bitMask = 0;
	while(totalNumBytesRead < h_buff->len)
	{
		
		switch(pdu[0])
		{
		case PRH_TCS_IE_SO_SENDING_COMPLETE:
			callParams->bitMask|= PRH_TCS_CC_PARAM_SENDINGCOMPLETE;
			numInfoElementBytes = PRH_TCS_IE_SIZE_SENDINGCOMPLETE;
			break;
			
		case PRH_TCS_IE_DO_CALL_CLASS:
			callParams->bitMask|= PRH_TCS_CC_PARAM_CALLCLASS;
			callParams->callClass = pdu[1];
			numInfoElementBytes += PRH_TCS_IE_SIZE_CALLCLASS;
			break;
			
		case PRH_TCS_IE_DO_CAUSE:
			callParams->bitMask|= PRH_TCS_CC_PARAM_CAUSE;
			callParams->cause = pdu[1];
			numInfoElementBytes = PRH_TCS_IE_SIZE_CAUSE;
			break;

		case PRH_TCS_IE_DO_PROGRESS_INDICATOR:
			callParams->bitMask|= PRH_TCS_CC_PARAM_PROGRESSINDICATOR;
			callParams->progressIndicator = pdu[1];
			numInfoElementBytes = PRH_TCS_IE_SIZE_PROGRESSINDICATOR;
			break;

		case PRH_TCS_IE_DO_SIGNAL:
			callParams->bitMask|= PRH_TCS_CC_PARAM_SIGNAL;
			callParams->signal = pdu[1];
			numInfoElementBytes = PRH_TCS_IE_SIZE_SIGNAL;
			break;

		case PRH_TCS_IE_DO_KEYPAD_FACILITY:
			callParams->bitMask|= PRH_TCS_CC_PARAM_KEYPADFACILITY;
			callParams->keypadFacility = pdu[1];
			numInfoElementBytes = PRH_TCS_IE_SIZE_KEYPADFACILITY;
			break;

		case PRH_TCS_IE_DO_SCO_HANDLE:
			callParams->bitMask|= PRH_TCS_CC_PARAM_SCOHANDLE;
			callParams->scoHandle = pdu[1];
			numInfoElementBytes = PRH_TCS_IE_SIZE_SCOHANDLE;
			break;

		case PRH_TCS_IE_VL_BEARER_CAP:			
			if(BT_NOERROR == decode_infoElementBearerCapability(pdu,(u_int8)( h_buff->len - totalNumBytesRead), callParams, &numInfoElementBytes) )
				callParams->bitMask|= PRH_TCS_CC_PARAM_BEARERCAPABILITY;
			break;

		case PRH_TCS_IE_VL_DEST_CID:
			if((0 != pdu[0]) && ( (u_int8)PRH_TCS_IE_SIZE_DESTCID <= (u_int8)(h_buff->len - totalNumBytesRead) ) && (PRH_TCS_IE_SIZE_DESTCID-PRH_TCS_IE_VL_SIZE_HEADER == pdu[1]) )
			{
				numInfoElementBytes = PRH_TCS_IE_SIZE_DESTCID;
				callParams->bitMask|= PRH_TCS_CC_PARAM_DESTCID;
				callParams->cid = BT_READ_LE_UINT16((pdu+2));
			}
			else
				numInfoElementBytes = PRH_TCS_IE_VL_SIZE_HEADER;
			break;

		case PRH_TCS_IE_VL_CALLING_PARTY_NUM:
			if( (0 != pdu[0]) && (  (pdu[1]+(u_int8)PRH_TCS_IE_VL_SIZE_HEADER) <= (u_int8)(h_buff->len-totalNumBytesRead)) )
			{
				numInfoElementBytes = PRH_TCS_IE_VL_SIZE_HEADER + pdu[1];
				callParams->bitMask|= PRH_TCS_CC_PARAM_CALLINGPARTYNUMBER;
				callParams->callingPartyNumberLength = pdu[1] - PRH_TCS_IE_BASESIZE_CALLINGPARTYNUMBER;
				callParams->callingPartyNumberType = pdu[2];
				callParams->callingPartyPresScreenIndicator = pdu[3];
				callParams->callingPartyNumber = pMalloc(callParams->callingPartyNumberLength);
				if(callParams->callingPartyNumber)
					pMemcpy(callParams->callingPartyNumber, pdu+4, callParams->callingPartyNumberLength);
				else
					return BT_NORESOURCES;						
			}
			else
				numInfoElementBytes = PRH_TCS_IE_VL_SIZE_HEADER;
			break;

		case PRH_TCS_IE_VL_CALLED_PARTY_NUM:
			if( (0 != pdu[0]) && ( (pdu[1]+(u_int8)PRH_TCS_IE_VL_SIZE_HEADER) <= (u_int8)(h_buff->len-totalNumBytesRead)) )
			{
				numInfoElementBytes = PRH_TCS_IE_VL_SIZE_HEADER + pdu[1];
				callParams->bitMask |= PRH_TCS_CC_PARAM_CALLEDPARTYNUMBER;
				callParams->calledPartyNumberLength = pdu[1] - PRH_TCS_IE_BASESIZE_CALLEDPARTYNUMBER;
				callParams->calledPartyNumberType = pdu[2];
				callParams->calledPartyNumber = pMalloc(callParams->calledPartyNumberLength);
				if(callParams->calledPartyNumber)
					pMemcpy(callParams->calledPartyNumber, pdu+3, callParams->calledPartyNumberLength);
				else
					return BT_NORESOURCES;	
			}
			else
				numInfoElementBytes = PRH_TCS_IE_VL_SIZE_HEADER;
			break;

		case PRH_TCS_IE_VL_AUDIO_CONTROL:
			if( (0!= pdu[0]) && ( (pdu[1]+(u_int8)PRH_TCS_IE_VL_SIZE_HEADER) <= (u_int8)(h_buff->len-totalNumBytesRead)) )
			{
				callParams->audioControlInfoLength = pdu[1];
				callParams->audioControlInfo = pMalloc(callParams->audioControlInfoLength);
				if(callParams->audioControlInfo)
					pMemcpy(callParams->audioControlInfo, (pdu+2), callParams->audioControlInfoLength);
				else
					return BT_NORESOURCES;
				numInfoElementBytes = PRH_TCS_IE_VL_SIZE_HEADER + pdu[1];
			}
			else
				numInfoElementBytes = PRH_TCS_IE_VL_SIZE_HEADER;
			break;
		
		case PRH_TCS_IE_VL_COMPANY_SPECIFIC:
			if( (0!= pdu[0]) && ( (pdu[1]+(u_int8)PRH_TCS_IE_VL_SIZE_HEADER) <= (u_int8)(h_buff->len-totalNumBytesRead)) )
			{
				callParams->companySpecificLength = pdu[1] - PRH_TCS_IE_BASESIZE_COMPANYSPECIFIC;
				callParams->companyID = BT_READ_LE_UINT16((pdu+2));
				callParams->companySpecific = pMalloc(callParams->companySpecificLength);
				if(callParams->companySpecific)
					pMemcpy(callParams->companySpecific, (pdu+4), callParams->companySpecificLength);
				else
					return BT_NORESOURCES;
			}
			else
				numInfoElementBytes = PRH_TCS_IE_VL_SIZE_HEADER;
			break;

		default:
			break;
		}
		
		pdu+= numInfoElementBytes;
		totalNumBytesRead+= numInfoElementBytes;

	}

	return BT_NOERROR;
}

t_api decode_infoElementBearerCapability(u_int8* pdu, u_int8 numBytesLeft, struct st_t_TCS_callParams *callParams, u_int8 *size)
{
	
	if(0 == pdu[1])
	{
		*size = PRH_TCS_IE_VL_SIZE_HEADER;
		return BT_INVALIDPARAM; /* A zero in the length field is actually legal, treat as if info element was absent */
	}

	callParams->linkType = pdu[2];
	
	/* Check that the len field matches the linkType and also that there are enough bytes left on the buffer 'numBytesLeft' */
	if(PRH_TCS_BEARER_CAP_LINK_TYPE_ACL == callParams->linkType && (PRH_TCS_IE_BASESIZE_BEARERCAPABILITY + PRH_TCS_IE_SIZE_ACLBEARERCAPABILITY == pdu[1]) && (numBytesLeft >= pdu[1]))
	{
		callParams->aclBearer = pMalloc(sizeof(struct st_t_TCS_aclBearer));
		if(!callParams->aclBearer)
			return BT_NORESOURCES;
		callParams->aclBearer->qos.flags = pdu[3];
		callParams->aclBearer->qos.serviceType = pdu[4];
		callParams->aclBearer->qos.tokenRate = BT_READ_LE_UINT32((pdu+5));
		callParams->aclBearer->qos.bucketSize = BT_READ_LE_UINT32((pdu+9));
		callParams->aclBearer->qos.peakBandwidth = BT_READ_LE_UINT32((pdu+13));
		callParams->aclBearer->qos.latency = BT_READ_LE_UINT32((pdu+17));
		callParams->aclBearer->qos.delayVariation = BT_READ_LE_UINT32((pdu+21));
		callParams->aclBearer->layerInfo = pdu[25];	
	}
	else if(PRH_TCS_BEARER_CAP_LINK_TYPE_SCO == callParams->linkType && (PRH_TCS_IE_BASESIZE_BEARERCAPABILITY + PRH_TCS_IE_SIZE_SCOBEARERCAPABILITY == pdu[1]) && (numBytesLeft >= pdu[1]))
		callParams->scoPacketType = pdu[3];
	else if(PRH_TCS_BEARER_CAP_LINK_TYPE_NONE != callParams->linkType )
		return BT_INVALIDPARAM;

	/* If Info element is invalid, ignore contents and skip the number of bytes in pdu[1] - the len field */
	/* If the len field is wrong, the decoding of subsequent info elements will be incorrect */
	*size = PRH_TCS_IE_VL_SIZE_HEADER + pdu[1];

	return BT_NOERROR;
	
}


t_api store_partyNumber(u_int8* pdu, u_int8 numNewDigits, u_int8 **number_ref)
{
	u_int8 *tmpNumber;
	u_int8 *oldNumber = *number_ref;
	u_int8 oldNumLen;

	if(oldNumber)
	{
		oldNumLen = pStrlen((char *)oldNumber);
		tmpNumber = pMalloc(oldNumLen+numNewDigits+1);
		if(!tmpNumber)
			return BT_NORESOURCES;
		pMemcpy(tmpNumber, oldNumber, oldNumLen);
		pMemcpy(tmpNumber+oldNumLen, pdu, numNewDigits);
		pFree(oldNumber);
		*number_ref = tmpNumber;
	}
	else
	{
		*number_ref = pMalloc(numNewDigits);
		if(!(*number_ref))
			return BT_NORESOURCES;		
		pMemcpy(*number_ref, pdu, numNewDigits);
	}
	
	return BT_NOERROR;
}
#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/
