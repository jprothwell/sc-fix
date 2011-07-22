/***********************************************************************
 *
 * MODULE NAME:    obex_sec_wrap.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Object Exchange Profile
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_sec_wrap.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#include "host_config.h"

#if OBEX_SEC

#include "host_types.h"
#include "obex_sec_wrap.h"
#include "obex_sec.h"
#include "papi.h"
#include "obex_defines.h"
#include "obex_utils.h"


#define PRH_MAX_PINCODE_SIZE 20
#define PRH_SIZE_OF_NONCE 16
#define PRH_SIZE_OF_DIGEST 16

u_int8 prh_securityLevel;
char prh_pinCode1[20];
char prh_pinCode2[20];
u_int8 remoteCID;
u_int16 remoteSrvChanNum;


/***********************************************************************
 *
 * FUNCTION NAME: OBEX_AuthRequest
 * DESCRIPTION: generate a nonce and place it in a OBEX_AUTH_CHALLENGE
							  header in the txBuffer. Allows the user to have standard
								security operations without having to be familiar with
								GOEP level OBEX. If the user requires any unusual types
								of security ie multiple nonces, then GOEP level
								OBEX must be used.

								Even though the nonce is generated inside the function
								it must be a parameter so it can be passed as a return 
								argument to the user. This is so Authentication Checking
								can take place later on with the nonce.
 	
 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_AuthRequest(u_int8 *localPinCode, u_int8 localPinCodeLen, t_obex_nonce nonce, u_int16 *offset_len, t_DataBuf *txBuffer)
{
	u_int8 status;

	if(!localPinCode)
		return BT_INVALIDPARAM;

	if(!localPinCodeLen)
		return BT_INVALIDPARAM;

	if(!nonce)
		return BT_INVALIDPARAM;

	if(!txBuffer)
		return BT_INVALIDPARAM;

	if(*offset_len < OBEX_HEADER)
		*offset_len=OBEX_HEADER;


	nonce[0]=00;   /*tag of the nonce*/
	nonce[1]=0x10; /*length of the nonce*/

	/* nonce+2 = to skip the tag and nonce length bytes */
	status=OBEX_GenerateNonce(localPinCodeLen, localPinCode, nonce);
	if(status)
		return status;

	status=OBEX_CreateSequenceHeader(OBEX_AUTH_CHALLANGE,18,(u_int8*)nonce,(t_DataBuf*)txBuffer,offset_len);
	
	return status;
}


/***********************************************************************
 *
 * FUNCTION NAME: OBEX_AuthResponse
 * DESCRIPTION: take the pincode, and the nonce as sent by the remote 
								machine and work out the digest. add the digest to the
								the t_DataBuf, that is being used for the response.

							*NB*	take in the nonce as provided in the OBEX_AUTH_CHALLENGE
								header. and identified by the OBEX_ExtractHeaders() fn.
								This function requires to be pointed to the au_cal header,
								it will extract the nonce part of the header from this point. 
								
								This function is intended to abstract the operation of
								security and deos not deal with situation where,
								there are multiple nonces etc in the au_cal header.
								GOEP level OBEX should be used for these more advanced
								scenarios.
 	
 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_AuthResponse(u_int8 *remotePinCode, u_int8 remotePinCodeLen, u_int8 *remoteNonceHeader, u_int16 *offset_len, t_DataBuf *txBuffer)
{

	u_int8 status;
	t_obex_digest digest;
	
	if(!remotePinCode)
		return BT_INVALIDPARAM;

	if(!remotePinCodeLen)
		return BT_INVALIDPARAM;

	if(!remoteNonceHeader)
		return BT_INVALIDPARAM;

	if(!txBuffer)
		return BT_INVALIDPARAM;

	if(*offset_len < OBEX_HEADER)
		*offset_len=OBEX_HEADER;


	/* check that the nonce is in the format handled in these helper functions*/
	digest[0]=00;   /*tag of the digest*/
	digest[1]=0x10; /*length of the digest*/
	
	/* remoteNonce+2 = to skip the header length bytes, test the tag and the digest length */
	status=pMemcmp(digest,remoteNonceHeader+2,2);
	if(status)
		return BT_UNSUPPORTED;


	/* 2= skip the header length */
	status=OBEX_GenerateDigest(remotePinCodeLen, remotePinCode, remoteNonceHeader+2, digest);
	if(status)
		return status;

	status=OBEX_CreateSequenceHeader(OBEX_AUTH_RESPONSE,18,(u_int8*)digest,(t_DataBuf*)txBuffer,offset_len);
	
	return status;
}

/***********************************************************************
 *
 * FUNCTION NAME: OBEX_AuthCheck
 * DESCRIPTION: 
								This function takes the nonce and pin codes, and the digest
								as got from the remote device. It calculates what the digest
								should be from the nonce and pin code and compares it with
								the received digest. 

								This function is intended to abstract the operation of
								security and deos not deal with situation where,
								there are multiple nonces etc in the au_cal header.
								GOEP level OBEX should be used for these more advanced
								scenarios.
 	
 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_AuthCheck(u_int8 *localPinCode, u_int8 localPinCodeLen, t_obex_nonce localNonce, u_int8 *digestHeader)
{
	u_int8 status;
	t_obex_digest trueDigest;

	if(!localPinCode)
		return BT_INVALIDPARAM;

	if(!localPinCodeLen)
		return BT_INVALIDPARAM;

	if(!localNonce)
		return BT_INVALIDPARAM;
	
	if(!digestHeader)
		return BT_INVALIDPARAM;

	trueDigest[0]=0x00;
	trueDigest[1]=0x10;
	
	/* digest+2 = to skip the header length bytes and just test the tag and the digest length */
	status=pMemcmp(digestHeader+2,trueDigest,2);
	if(status)
		return BT_UNSUPPORTED;

	/* 2 = skip(1 for the nonce tag, 1 nonce length) */
	status=OBEX_GenerateDigest(localPinCodeLen, localPinCode, localNonce, trueDigest);
	if(status)
		return status;
	
	/* 4 = skip (2 for the header length)+(1 for the nonce tag, 1 nonce length) */
	/* 2 = skip (1 for the nonce tag, 1 nonce length) */
	status=pMemcmp(digestHeader+4, trueDigest+2,16);
	
	if(status)
		return BT_AUTHENTICATIONFAIL;
	else
		return BT_NOERROR;	
}

#endif
