/******************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                    */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    cfw_sms_tool.c                                               */
/*                                                                            */
/* Description:                                                               */
/*         All SMS MO API defined in CFW doc                                  */
/*                                                                            */
/* Revision :                                                                 */
/*                                                                            */
/*   1.0.0 : 2006-04-15, Wang Dahai, dahai_wang@ciitec.com                    */
/******************************************************************************/
#include "cfw_sms_prv.h"
#include <csw_mem_prv.h>
#include "sxs_io.h" //xueww[+] 2007.07.13
#include "csw_debug.h"
//Here, define a golobal variable for longer msg
extern UINT32* gpLongerMsgPara;
extern UINT32* gpLongerMsgParaIndex;
extern CFW_SMS_LONG_INFO* gpWriteLongerMsgPara;
extern UINT8   gnWriteLongerMsgParaCount;
//for long sms
extern PSTR pSMSDir;
extern PSTR pSIMFilename;
extern PSTR pMEFilename;
#define CHECK_STORAGE_SMS_INFO_TMP
#if (CFW_SMS_DUMP_ENABLE==0) && (CFW_SMS_SRV_ENABLE==1)
/**********************************************************************************/
/**********************************************************************************/


///////////////////////////////////////////////////////////////////////////////
//                 Local APIs For SMS MT   (sms_mt_Xxx...)                   //
///////////////////////////////////////////////////////////////////////////////

// pSrc: Source data pointer
// pTimeStamp: pointer to TM_SMS_TIME_STAMP
// ???There will some problems for the time zone transform
VOID sms_mt_SerializeTime(TM_SMS_TIME_STAMP *pTimeStamp, CONST UINT8* pSrc)
{
		CSW_PROFILE_FUNCTION_ENTER(sms_mt_SerializeTime);
    //Usually, time is saved in 7 bytes(low 4 bit& high 4 bit)
  
    pTimeStamp->uYear = (UINT16)(2000 + (*pSrc & 0x0f)*10 + (*pSrc >> 4));
    pSrc++;
    pTimeStamp->uMonth = (UINT8)((*pSrc & 0x0f)*10 + (*pSrc >> 4));
    pSrc++;
    pTimeStamp->uDay = (UINT8)((*pSrc & 0x0f)*10 + (*pSrc >> 4));
    pSrc++;
    pTimeStamp->uHour = (UINT8)((*pSrc & 0x0f)*10 + (*pSrc >> 4));
    pSrc++;
    pTimeStamp->uMinute = (UINT8)((*pSrc & 0x0f)*10 + (*pSrc >> 4));
    pSrc++;
    pTimeStamp->uSecond = (UINT8)((*pSrc & 0x0f)*10 + (*pSrc >> 4));
    pSrc++;
    //remove 
    //pTimeStamp->iZone = (UINT8)((*pSrc & 0x0f)*10 + (*pSrc >> 4) - 24);
    //add
    INT8 tmpZone = 0;
    tmpZone =(*pSrc & 0x07)*10 + (*pSrc >> 4);
    tmpZone = tmpZone > ((tmpZone/4)*4)?(tmpZone/4 + 1) : (tmpZone/4);
    if((*pSrc & 0x0f) >= 0x80)
    {
      pTimeStamp->iZone = 0 - tmpZone;
    }else{
      pTimeStamp->iZone = tmpZone;
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_mt_SerializeTime);

}

// BCD  -> ASCII
// pSrc: Source data pointer
// pDst: Dst char pointer
// nSrcLength: Dst char length
VOID sms_tool_BCD2ASCII(UINT8* pDst, CONST UINT8* pSrc, UINT8 nSrcLength)
{
    //Usually, max number count is less than 20
    UINT8       ch=0x00,i=0x00,buffer[20];         
    UINT8*      pBuf=NULL;
    CONST UINT8 tab[]="0123456789ABCDEF";    // 0x0-0xf char table
    CSW_PROFILE_FUNCTION_ENTER(sms_tool_BCD2ASCII);
    SUL_ZeroMemory8(buffer,20);
    pBuf = buffer;
    for(i=0; i<2*nSrcLength; i++)
    {
        // low 4 bit
        *pBuf++ = tab[*pSrc >> 4];
        // high 4 bit
        *pBuf++ = tab[*pSrc & 0x0f];
        pSrc++;
    }

    pBuf = buffer;    
    for(i=0; i<2*nSrcLength; i+=2)
    {
        //Switch the char
        ch = *pBuf++;        
        *pDst++ = *pBuf++;   
        *pDst++ = ch;        
    }
    
    // if the last char is "F"
    if(*(pDst-1) == 'F')
    {
        pDst--;
    }
    
    // zero-terminated. 
    *pDst = '\0';
    CSW_PROFILE_FUNCTION_EXIT(sms_tool_BCD2ASCII);
}

//ASCII->BCD码,如果转化奇数个数字的话，将会在对应高位补F
//13811189836 --> 0x31 0x18 0x11 0x98 0x38 0xF6
//

VOID sms_tool_ASCII2BCD(UINT8* pDst, CONST UINT8* pSrc,UINT8 nSrcLength)
{
    UINT8 nTmp=0x0,i=0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_tool_ASCII2BCD);
	
    for(i = 0; i < nSrcLength; i++)
    {
        CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ASCII2BCD, *pSrc=0x%x \n ", *pSrc);
        switch(*pSrc)
        {
        case '*':
            nTmp = (INT8)0x0A;
            break;
        case '#':
            nTmp = (INT8)0x0B;
            break;
        default:
            nTmp = (INT8)(*pSrc - '0');
            break;
        }
        if(i % 2)
        {
            *pDst++ |= (nTmp << 4) & 0xF0;
        }
        else
        {
            *pDst = (INT8)(nTmp & 0x0F);
        }
        pSrc++;
    }
    if(i % 2)
    {
        *pDst |= 0xF0;
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_tool_ASCII2BCD);
}



//3GPP TS 23.040 V3.9.0 (2002-06)" clause 9.2.2.1a"
//SMS-DELIVER-REPORT for RP-ACK
#ifndef CFW_MULTI_SIM
UINT32 sms_mt_SmsPPAckReq(void)
#else
UINT32 sms_mt_SmsPPAckReq(CFW_SIM_ID nSimId)
#endif
{
    api_SmsPPAckReq_t*   pSmsPPACKReq = NULL;
    CSW_PROFILE_FUNCTION_ENTER(sms_mt_SmsPPAckReq);
    
    //Malloc api_SmsPPAckReq_t(CFW --> SCL)
    //System will free the memory after get Msg
    pSmsPPACKReq = (api_SmsPPAckReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SmsPPAckReq_t) + 2);
    if(!pSmsPPACKReq)
    {
	CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_SmsPPAckReq pSmsPPACKReq malloc error!!! \n ");
	CSW_PROFILE_FUNCTION_EXIT(sms_mt_SmsPPAckReq);
	return ERR_NO_MORE_MEMORY;
    }
    SUL_ZeroMemory8(pSmsPPACKReq, SIZEOF(api_SmsPPAckReq_t));
    
    //Set Value
    //??? This value should be got from a cfg API
    pSmsPPACKReq->Path = 0; //0:GSM, 1:GPRS
    //Here,fill Data[] to be NULL,bacause this field is optional,see STACKCOM 10.1.3.1
    pSmsPPACKReq->DataLen = 0x02;
    pSmsPPACKReq->Data[0] = 0x00;
    pSmsPPACKReq->Data[1] = 0x00;
    
    //Send MSG to SCL
    UINT32 ret = 0x0;
#ifndef CFW_MULTI_SIM
    ret = CFW_SendSclMessage(API_SMSPP_ACK_REQ, pSmsPPACKReq);
#else
    ret = CFW_SendSclMessage(API_SMSPP_ACK_REQ, pSmsPPACKReq, nSimId);
#endif

	if(ERR_SUCCESS == ret)
	{
	    return ret;
	}
	else
	{
		CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_SmsPPAckReq, 2,call CFW_SendSclMessage = 0x%x \n ", ret);
	}
    CSW_PROFILE_FUNCTION_EXIT(sms_mt_SmsPPAckReq);
    return ret;
}

//3GPP TS 23.040 V3.9.0 (2002-06)" clause 9.2.3.22"
//SMS-DELIVER-REPORT for RP-ERROR
#ifndef CFW_MULTI_SIM
UINT32 sms_mt_SmsPPErrorReq(UINT8 nCause	)
#else
UINT32 sms_mt_SmsPPErrorReq(UINT8 nCause, CFW_SIM_ID nSimId)
#endif	
{
    api_SmsPPErrorReq_t*   pErrorReq = NULL;
    CSW_PROFILE_FUNCTION_ENTER(sms_mt_SmsPPErrorReq);
    
    //Malloc api_SmsPPErrorReq_t(CFW --> SCL)
    //System will free the memory after get Msg
    pErrorReq = (api_SmsPPErrorReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SmsPPErrorReq_t));
    if(!pErrorReq)
    {
		CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_SmsPPErrorReq pErrorReq malloc error!!! \n ");
        CSW_PROFILE_FUNCTION_EXIT(sms_mt_SmsPPErrorReq);
        return ERR_NO_MORE_MEMORY;
    }
    SUL_ZeroMemory8(pErrorReq, SIZEOF(api_SmsPPErrorReq_t));

    //Modify the RP-Error to 0x16 Memory capacity exceeded.
    if( 0xD3 == nCause ) nCause = 0x16;
    
    //??? This value should be got from a cfg API
    pErrorReq->Path     =  0; //0 GSM; 1 GPRS
    pErrorReq->CauseLen = 0x01;
    //3GPP TS 23.040 V3.9.0 (2002-06)" clause 9.2.3.22"
    pErrorReq->Cause[0] = nCause; // 0x16 Memory capacity exceeded
    //Here,fill Data[] to be NULL,because this field is optional,see STACKCOM 10.1.4.2
    //If want to fill Data[], see more details in "3GPP TS 23.040 V3.9.0 (2002-06)" clause 9.2.2.1a	SMS DELIVER REPORT type
    pErrorReq->DataLen  = 0x00;
    pErrorReq->Data[0]  = 0x00;
    
    //Send MSG to SCL
#ifndef CFW_MULTI_SIM
    CFW_SendSclMessage(API_SMSPP_ERROR_REQ, pErrorReq);
#else
    CFW_SendSclMessage(API_SMSPP_ERROR_REQ, pErrorReq, nSimId);
#endif
    CSW_PROFILE_FUNCTION_EXIT(sms_mt_SmsPPErrorReq);
    return ERR_SUCCESS;
}


//Parse TP-DCS , more in "3G TS 23.038 V3.3.0"
PRIVATE UINT32 sms_mt_SmsParseDCS(UINT8 nInputData, UINT8* pClassFlag, UINT8* pAlphabet,UINT8* pIsCompressed)
{
	CSW_PROFILE_FUNCTION_ENTER(sms_mt_SmsParseDCS);
    //Parse TP-DCS , more in "3G TS 23.038 V3.3.0"
    if((nInputData & 0xff) == 0x00)// bit(7-0): 0000 0000
    {
        *pClassFlag = MSG_CLASS_RESERVED;
        *pAlphabet  = MSG_GSM_7;
    }
    else if((nInputData & 0xf0) == 0xf0)  //bit(7-0): 1111 0xxx
    {
        if((nInputData & 0x04) == 0x04) // bit(7-0): 1111 01xx
            *pAlphabet  = MSG_8_BIT_DATA;
        else// bit(7-0): 1111 00xx
            *pAlphabet  = MSG_GSM_7;

        if((nInputData & 0x03) == 0x00) // bit(7-0): 1111 0x00
            *pClassFlag = MSG_CLASS_0;
        else if((nInputData & 0x03) == 0x01) // bit(7-0): 1111 0x01
            *pClassFlag = MSG_CLASS_1;
        else if((nInputData & 0x03) == 0x02) // bit(7-0): 1111 0x10
            *pClassFlag = MSG_CLASS_2;
        else if((nInputData & 0x03) == 0x03) // bit(7-0): 1111 0x11
            *pClassFlag = MSG_CLASS_3;
    }
    else if((nInputData & 0xc0) == 0x00)  //bit(7-0): 00xx xxxx
    {
        if((nInputData & 0x20) == 0x20)  // bit(7-0): 001x xxxx Compressed
            *pIsCompressed = 1; //Compressed
        if((nInputData & 0x10) == 0x10)  // bit(7-0): 00x1 xxxx 
        {
            //Set pClassFlag
            if((nInputData & 0x13) == 0x10)// bit(7-0): 00x1 xx00 
            {
                *pClassFlag = MSG_CLASS_0;
            }
            else if((nInputData & 0x13) == 0x11)// bit(7-0): 00x1 xx01 
            {
                *pClassFlag = MSG_CLASS_1;
            }
            else if((nInputData & 0x13) == 0x12)// bit(7-0): 00x1 xx10 
            {
                *pClassFlag = MSG_CLASS_2;
            }
            else if((nInputData & 0x13) == 0x13)// bit(7-0): 00x1 xx11 
            {
                *pClassFlag = MSG_CLASS_3;
            }
        }
        else // bit(7-0): 00x0 xxxx 
        {
            *pClassFlag = MSG_CLASS_RESERVED;
        }
        //Set pAlphabet
        if((nInputData & 0x0c) == 0x00)// bit(7-0): 00xx 00xx 
        {
            *pAlphabet = MSG_GSM_7;
        }
        else if((nInputData & 0x0c) == 0x04)// bit(7-0): 00xx 01xx 
        {
            *pAlphabet = MSG_8_BIT_DATA;
        }
        else if((nInputData & 0x0c) == 0x08)// bit(7-0): 00xx 10xx 
        {
            *pAlphabet = MSG_UCS2;
        }
        else if((nInputData & 0x0c) == 0x0c)// bit(7-0): 00xx 11xx 
        {
            *pAlphabet = MSG_RESERVED;
        }
    }    
    CSW_PROFILE_FUNCTION_EXIT(sms_mt_SmsParseDCS);    
    return ERR_SUCCESS;
}

//Get Original Character set from SMS received
//Input  Data : nChset, nAlphabet, nTP_UDL, pInputData(&pInMsg->Data[1+nOALen+1+1+7+1])
//Output Data : pSmsDataProc,pDestUDL
//TP-DCS TP-Data-Coding-Scheme,3G TS 23.038 V3.3.0 (2000-01) clause 4 "SMS Data Coding Scheme"
//Compare the Alphabet between User setting and Origianl setting
//process pInMsg->Data[1+nOALen+1+1+7+1]
PRIVATE UINT32 sms_mt_UncompressedDataDecode(UINT8 nAlphabet, UINT8 nTP_UDL, CONST UINT8* pInputData,
                                             UINT8** pOutputData, UINT16* pDestUDL)
{
    UINT16 tmp=0x0;
    UINT8* pSmsDataProc = NULL;
    CSW_PROFILE_FUNCTION_ENTER(sms_mt_UncompressedDataDecode);
    if(nTP_UDL == 0)
        return ERR_INVALID_PARAMETER;
    if(nAlphabet == MSG_GSM_7)// if the code received is MSG_GSM_7
    {
        UINT8 buf[CFW_SMS_LEN_7BIT_NOMAL];  // 内部用的缓冲区
        SUL_ZeroMemory32(buf,CFW_SMS_LEN_7BIT_NOMAL);
        //translate "GSM 7 bit default alphabet" into "CFW_CHSET_UCS2"
        //Copy the processed data into pSmsDataProc
        //Set nTP_UDL to be the length of CFW_CHSET_UCS2 data
        //modify by lxp at 20050104-18:19
        //nDestUDL = SUL_String2Bytes(&pInMsg->Data[1+nOALen+1+1+7+1], buf, nTP_UDL & 7 ? (int)nTP_UDL * 7 / 4 + 2 : (int)nTP_UDL * 7 / 4);  // 格式转换
        //nTP_UDL & 7 ? (int)nTP_UDL * 7 / 4 + 2 : (int)nTP_UDL * 7 / 4)
        //首先判断是否能被8整除，然后进行处理。
        pSmsDataProc = (UINT8*)CSW_SMS_MALLOC(nTP_UDL*2);
        if(!pSmsDataProc)
	 {
		CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_UncompressedDataDecode 1 pSmsDataProc malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(sms_mt_UncompressedDataDecode);
		return ERR_NO_MORE_MEMORY;
	 }
        SUL_ZeroMemory8(pSmsDataProc, nTP_UDL*2);
       // SUL_Decode7Bit(pInputData, pSmsDataProc, (UINT16)(nTP_UDL & 7 ? nTP_UDL * 7 / 8 + 1 : nTP_UDL * 7 / 8));    // 转换到TP-DU
		//[[2007.02.01[+] fix bug 3842
		nTP_UDL = SUL_Decode7Bit(pInputData, pSmsDataProc, (UINT16)nTP_UDL);    // 转换到TP-DU
		//]]2007.02.01[+]
        tmp = (UINT16)nTP_UDL;
    }
    else if(nAlphabet == MSG_8_BIT_DATA) 
    {   
        //translate "8 bit data" into "CFW_CHSET_UCS2"
        //Copy the processed data into pSmsDataProc
        //Set nTP_UDL to be the length of CFW_CHSET_UCS2 data  
        //modify by lxp at 20050104-18:19
        //nDestUDL = SUL_String2Bytes(&pInMsg->Data[1+nOALen+1+1+7+1], buf, nTP_UDL * 2);        // 格式转换
		//[[xueww[mod] 2007.09.20
		pSmsDataProc = (UINT8*)CSW_SMS_MALLOC(nTP_UDL);   

		if(!pSmsDataProc)
		{
			CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_UncompressedDataDecode 2 pSmsDataProc malloc error!!! \n ");
			CSW_PROFILE_FUNCTION_EXIT(sms_mt_UncompressedDataDecode);
			return ERR_NO_MORE_MEMORY;
		}

		SUL_ZeroMemory8(pSmsDataProc, nTP_UDL);
		SUL_MemCopy8(pSmsDataProc, pInputData,(UINT32)nTP_UDL);        // 格式转换

		tmp = (UINT16)nTP_UDL;
		CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_UncompressedDataDecode, nTP_UDL= %d\n ",nTP_UDL);
		//]]xueww[mod] 2007.09.20

		/*
		pSmsDataProc = (UINT8*)CSW_SMS_MALLOC(nTP_UDL * 2);   

		if(!pSmsDataProc){
		CSW_PROFILE_FUNCTION_EXIT(sms_mt_UncompressedDataDecode);
		return ERR_NO_MORE_MEMORY;
		}

		SUL_ZeroMemory8(pSmsDataProc, nTP_UDL * 2);

		sms_mt_GSM2UCS2(pInputData,pSmsDataProc,nTP_UDL);
		tmp = (UINT16)(nTP_UDL * 2);
		*/
    }
    else if(nAlphabet == MSG_UCS2)  
    {   
        pSmsDataProc = (UINT8*)CSW_SMS_MALLOC(nTP_UDL);

        if(!pSmsDataProc)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_UncompressedDataDecode 3 pSmsDataProc malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(sms_mt_UncompressedDataDecode);
		return ERR_NO_MORE_MEMORY;
	}

        SUL_ZeroMemory8(pSmsDataProc, nTP_UDL);

        SUL_MemCopy8(pSmsDataProc, pInputData,(UINT32)nTP_UDL);        // 格式转换
        tmp = (UINT16)nTP_UDL;
    }

    *pDestUDL  = tmp;
    *pOutputData = pSmsDataProc;

    CSW_PROFILE_FUNCTION_EXIT(sms_mt_UncompressedDataDecode);
    return ERR_SUCCESS;
}

//???  TO DO, later
PRIVATE UINT32 sms_mt_CompressedDataDecode(UINT8 nAlphabet, UINT8 nTP_UDL, CONST UINT8* pInputData,
                                           UINT8** pSmsDataProc, UINT16* pDestUDL)
{
	CSW_PROFILE_FUNCTION_ENTER(sms_mt_CompressedDataDecode);
    pSmsDataProc = pSmsDataProc;
    *pDestUDL    = *pDestUDL;
    pInputData   = pInputData;
    nTP_UDL      = nTP_UDL;
    nAlphabet    = nAlphabet;

    CSW_PROFILE_FUNCTION_EXIT(sms_mt_CompressedDataDecode);
    return ERR_SUCCESS;
}
//Parse Status Report
UINT32 sms_mt_Parse_SmsPPReceiveInd_SR(HAO hAo)
{
	//Msg Structure(CFW --> AT)
	CFW_SMS_MT_INFO*       pSmsMTInfo  = NULL;  //MT private data
	api_SmsPPReceiveInd_t* pInMsg      = NULL;
	CFW_NEW_SMS_NODE*      pSmsNode    = NULL;
	UINT8                  nFormat=0x0;
	UINT32                 nStructureSize=0x0; 
#ifndef CFW_MULTI_SIM
	CSW_PROFILE_FUNCTION_ENTER(sms_mt_Parse_SmsPPReceiveInd_SR);
#else
	CFW_SIM_ID nSimId = CFW_SIM_COUNT;
	CSW_PROFILE_FUNCTION_ENTER(sms_mt_Parse_SmsPPReceiveInd_SR);
	CFW_GetSimID(hAo, &nSimId);
#endif

	//Get private date
	pSmsMTInfo = CFW_GetAoUserData(hAo);
	if(!pSmsMTInfo)
	{
		CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd_SR);
		return ERR_NO_MORE_MEMORY;
	}

	//Pointer to pInMsg data
	pInMsg = pSmsMTInfo->pInMsg;

	//Get SMS format from user setting
#ifndef CFW_MULTI_SIM
	CFW_CfgGetSmsFormat(&nFormat);
#else

	CFW_CfgGetSmsFormat(&nFormat, nSimId);
#endif

	//TP-MTI, SMS STATUS REPORT
	//More details in "3GPP TS 23.040 V3.9.0 (2002-06)" clause 9.2.2.3	SMS STATUS REPORT type
	if (((pInMsg->Data[0]) & 0x03) == 0x02)
	{
		CFW_SMS_TXT_STATUS_IND* pSmsTxtReport = NULL;  //Status Report(Text)
		CFW_SMS_PDU_INFO*       pSmsPDU       = NULL;  //PDU Format
		pSmsMTInfo->nSmsType = 0; // 0 : STATUS REPORT

		//SMS-STATUS-REPORTs are routed to the TE using unsolicited result code
		if(nFormat == 0) //PDU mode
		{
			//Malloc CFW_SMS_PDU_INFO for PDU Status Report (CFW --> AT)
			//System will free the memory after get Msg
			nStructureSize = SIZEOF(CFW_NEW_SMS_NODE) + SIZEOF(CFW_SMS_PDU_INFO) + pInMsg->DataLen + pInMsg->AddressLen;
			pSmsNode = (CFW_NEW_SMS_NODE*)CSW_SMS_MALLOC(nStructureSize);
			if(!pSmsNode)
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd_SR nFormat[0] pSmsNode malloc error!!! \n ");
				CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd_SR);
				return ERR_NO_MORE_MEMORY;
			}
			SUL_ZeroMemory8(pSmsNode, nStructureSize);
			//Set pSmsNode Type
			pSmsNode->nType  = 0x40;
			pSmsNode->pNode = (CFW_SMS_PDU_INFO*)((UINT8*)pSmsNode + SIZEOF(CFW_NEW_SMS_NODE));
			//Set pSmsPDU Value
			pSmsPDU = (CFW_SMS_PDU_INFO*)(pSmsNode->pNode);
			//Set Value
			pSmsPDU->nDataSize = (UINT16)(pInMsg->DataLen + pInMsg->AddressLen);
			//???Maybe not necessary, think about it later
			pSmsPDU->nTpUdl  = 0; 
			pSmsPDU->nStatus = 0;
			//The Data field is the SM-TL PDU as defined in 3GPP TS 23.040 v3.10.0 Technical realization of the Short Message Service.
			//It conveys one of the SMS-DELIVER SM-TL PDU.
			//Pointer to data buffer to hold PDU(GSM 04.11 SC address followed by GSM 03.40 TPDU in hexadecimal format). 
			SUL_MemCopy8(pSmsPDU->pData, pInMsg->Address, (UINT32)pInMsg->AddressLen);
			SUL_MemCopy8((pSmsPDU->pData+pInMsg->AddressLen), pInMsg->Data, (UINT32)pInMsg->DataLen);
#ifndef CFW_MULTI_SIM
			//Send a msg to AT, nType =0,CFW_NEW_SMS_NODE.nType =0x40
			CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode, 0, SMS_MT_UTI, 0);
			sms_mt_SmsPPAckReq();
#else
			//Send a msg to AT, nType =0,CFW_NEW_SMS_NODE.nType =0x40
			CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode, 0, SMS_MT_UTI |(nSimId<<24), 0);
			sms_mt_SmsPPAckReq(nSimId);
#endif
		}
		else if(nFormat == 1) //Text mode
		{
			UINT8   nRA=0x0, nLen_ra=0x0;
			//Malloc CFW_SMS_TXT_REPORT_INFO for Text Status Report (CFW --> AT)
			//System will free the memory after get Msg
			//Set the length of TP-SCTS and TP-DT to post AT, here provide a whole char
			nLen_ra   = (UINT8)(pInMsg->Data[2] / 2 + pInMsg->Data[2] % 2);  //TP-RA, 3GPP TS 23.040 v3.10.0
			//Calulate the basic length after RA.
			nRA = (UINT8)(pInMsg->Data[2] / 2 + pInMsg->Data[2] % 2 + 2);
			//Malloc CFW_SMS_TXT_STATUS_IND
			nStructureSize = SIZEOF(CFW_NEW_SMS_NODE) + SIZEOF(CFW_SMS_TXT_STATUS_IND);
			pSmsNode = (CFW_NEW_SMS_NODE*)CSW_SMS_MALLOC(nStructureSize);
			if(!pSmsNode)
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd_SR nFormat[1]pSmsNode malloc error!!! \n ");
				CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd_SR);
				return ERR_NO_MORE_MEMORY;
			}
			SUL_ZeroMemory8(pSmsNode, nStructureSize);
			//Set pSmsNode Type
			pSmsNode->nType = 0x41;
			pSmsNode->pNode = (CFW_SMS_TXT_STATUS_IND*)((UINT8*)pSmsNode + SIZEOF(CFW_NEW_SMS_NODE));
			//Set pSmsPDU Value
			pSmsTxtReport = (CFW_SMS_TXT_STATUS_IND*)(pSmsNode->pNode);
			//Set value to Structure CFW_SMS_TXT_STATUS_IND
			pSmsTxtReport->fo   = 2; //pInMsg->Data[0]; //first octet of GSM 03.40 
			//TP-MR
			pSmsTxtReport->mr   = pInMsg->Data[1];//Specify the message reference from 0 to 255
			//TP-RA, Specify 3G TS 23.040 [3] TP-Recipient-Address Address-Value field in string format
			if(pInMsg->Data[2] > 0)
			{
				SUL_MemCopy8(pSmsTxtReport->ra,&pInMsg->Data[4], nLen_ra);
				pSmsTxtReport->ra_size = nLen_ra;  //BCD size
			}
			//TP-RA 3GPP TS 23.040 v3.10.0 "Clause9.1.2.5"
			//[[xueww[mod] 2007.08.08 (pInMsg->Data[3] & 0x90) --->(pInMsg->Data[3]>>4)......
			if((pInMsg->Data[3]>>4) == 9) //International number(bit7~bit0) : 1001 xxxx
				pSmsTxtReport->tora  = CFW_TELNUMBER_TYPE_INTERNATIONAL;
			else if((pInMsg->Data[3] >>4) == 0x0a) //National number(bit7~bit0) : 1010 xxxx
				pSmsTxtReport->tora  = CFW_TELNUMBER_TYPE_NATIONAL;
			else if((pInMsg->Data[3]>>4) == 0x0d) //National number(bit7~bit0) : 1101 0000//xueww[+] 2007.08.01
				pSmsTxtReport->tora  = 0xd0;
			else
				pSmsTxtReport->tora  = CFW_TELNUMBER_TYPE_UNKNOWN;
			//]]xueww[mod] 2007.08.08

			sms_mt_SerializeTime(&pSmsTxtReport->scts,&pInMsg->Data[1+1+nRA]);//TP Service Centre Time Stamp
			sms_mt_SerializeTime(&pSmsTxtReport->dt,&pInMsg->Data[1+1+nRA+7]);//TP Discharge Time
			//TP-ST
			pSmsTxtReport->st   = pInMsg->Data[2 + nRA +7 + 7];//TP Status
#ifndef CFW_MULTI_SIM
			//Send a msg to AT, nType =0,CFW_NEW_SMS_NODE.nType =0x41
			CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode,0 , SMS_MT_UTI, 0);
			sms_mt_SmsPPAckReq();
#else
			//Send a msg to AT, nType =0,CFW_NEW_SMS_NODE.nType =0x40
			CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode, 0, SMS_MT_UTI |(nSimId<<24), 0);
			sms_mt_SmsPPAckReq(nSimId);
#endif

		}
		pSmsNode->nConcatPrevIndex = 0xffff;
		pSmsNode->nConcatCurrentIndex = pSmsMTInfo->nIndex;
		pSmsNode->nStorage = pSmsMTInfo->nSRStorageID;

	} //end of if SMS STATUS REPORT

	CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd_SR);  
	return ERR_SUCCESS;
}


//Parse Short message, get the parameter of longer sms
UINT32 sms_mt_Parse_LongMSGPara(HAO hAo)
{
    //Msg Structure(CFW --> AT)
    CFW_SMS_MT_INFO*       pSmsMTInfo  = NULL;  //MT private data
    api_SmsPPReceiveInd_t* pInMsg      = NULL;
    CSW_PROFILE_FUNCTION_ENTER(sms_mt_Parse_LongMSGPara);

    //Get private date
    pSmsMTInfo = CFW_GetAoUserData(hAo);
    if(!pSmsMTInfo){
    	CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_LongMSGPara);
      return ERR_NO_MORE_MEMORY;
      }
    //Pointer to pInMsg data
    pInMsg = pSmsMTInfo->pInMsg;
    //verify TP-UDHI, The TP-UD field contains a Header
    //TP-UDHI, it is bit6 in "3GPP TS 23.040 V3.9.0  Clause 9.2.3.23"
    if((pInMsg->Data[0] & 0x40) == 0x40) // bit(7-0):x1xx xxxx
    {
        UINT8   nOALen=0x0,nIEIa=0x0, nIEIDLa=0x0;
    UINT8 nPID = 0x00,  nDCS = 0x00;
    UINT8 nClassFlag = 0x00, nAlphabet = 0x00, isCompressed = 0x00;

        //Get the length of TP Originating Address(TP-OA)
        nOALen = (UINT8)(pInMsg->Data[1] / 2 + pInMsg->Data[1] % 2 + 2);
    
    nPID = pInMsg->Data[1 + nOALen];
    nDCS = pInMsg->Data[1 + nOALen + 1];
    
      sms_mt_SmsParseDCS(nDCS, &nClassFlag, &nAlphabet, &isCompressed);
      if(( 0x7F == nPID ) && ( MSG_CLASS_2 == nClassFlag ))
      {
        return ERR_NO_NEED_SAVE;
      }
        //see more in "3GPP TS 23.040 V3.9.0 (2002-06)" clause 9.2.3.24"
        nIEIa   = pInMsg->Data[1+nOALen+1+1+7+1+1];    //IEIa 
        nIEIDLa = pInMsg->Data[1+nOALen+1+1+7+1+1+1];  //IEIDLa
        if(nIEIa == 0x00)  //0:8-bit reference number; 
        {
            pSmsMTInfo->nLongerMsgID       = (UINT16)pInMsg->Data[1+nOALen+1+1+7+nIEIDLa+1];
            pSmsMTInfo->nLongerMsgCount    = pInMsg->Data[1+nOALen+1+1+7+nIEIDLa+2];
            pSmsMTInfo->nLongerMsgCurrent  = pInMsg->Data[1+nOALen+1+1+7+nIEIDLa+3];
        }
        else if(nIEIa == 0x08) //8: 16-bit reference number
        {
            UINT16 nTmp=0x0;
            nTmp = (UINT16)pInMsg->Data[1+nOALen+1+1+7+nIEIDLa+1]<< 8;
            pSmsMTInfo->nLongerMsgID       = (UINT16)(nTmp + pInMsg->Data[1+nOALen+1+1+7+nIEIDLa+2]);
            pSmsMTInfo->nLongerMsgCount    = pInMsg->Data[1+nOALen+1+1+7+nIEIDLa+3];
            pSmsMTInfo->nLongerMsgCurrent  = pInMsg->Data[1+nOALen+1+1+7+nIEIDLa+4];
        }
        //Verfy is longer or not
        if(pSmsMTInfo->nLongerMsgCount > 1)
            pSmsMTInfo->isLongerMsg = 1;
        else
            pSmsMTInfo->isLongerMsg = 0;
    }//end of if((pInMsg->Data[0] & 0x40) == 0x40) 
  else
  {
    CSW_TRACE(CFW_SMS_TS_ID, TSTXT("No UDHI\n"));
    UINT8 nPID = 0x00,  nDCS = 0x00;
    UINT8 nClassFlag = 0x00, nAlphabet = 0x00, isCompressed = 0x00;
    UINT8 nOALen =0x00;
    // Get the length of TP Originating Address(TP-OA)
    nOALen = (UINT8)(pInMsg->Data[1] / 2 + pInMsg->Data[1] % 2 + 2);
    
    nPID = pInMsg->Data[1 + nOALen];
    nDCS = pInMsg->Data[1 + nOALen + 1];
    
    sms_mt_SmsParseDCS(nDCS, &nClassFlag, &nAlphabet, &isCompressed);
    if(( 0x7F == nPID ) && ( MSG_CLASS_2 == nClassFlag ))
    {
      return ERR_NO_NEED_SAVE;
    }

  }
    CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_LongMSGPara);
    return ERR_SUCCESS;
}

static BOOL _SMS_HaveUDHI(api_SmsPPReceiveInd_t* pInMsg)
{
  // verify TP-UDHI, The TP-UD field contains a Header
  // TP-UDHI, it is bit6 in "3GPP TS 23.040 V3.9.0  Clause 9.2.3.23"

  if ((pInMsg->Data[0] & 0x40) == 0x40)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
static UINT8 _SMS_GetOALen(api_SmsPPReceiveInd_t* pInMsg)
{
  return (UINT8)(pInMsg->Data[1] / 2 + pInMsg->Data[1] % 2 + 2);
}
typedef enum
{
  SMS_DELIVER,
  SMS_DELIVER_REPORT,
  SMS_STATUS_REPORT,
  SMS_COMMAND,
  SMS_SUBMIT,
  SMS_SUBMIT_REPORT,
  SMS_RESERVED
} SMS_TYPE;
 
//=====================================================================================
// FUNCTION: _SMS_GetSMSType
//
//=====================================================================================
static SMS_TYPE _SMS_GetSMSType(api_SmsPPReceiveInd_t* pInMsg)
{
/*
	bit1	bit0	Message type
	0	0	SMS DELIVER (in the direction SC to MS)	
	0	0	SMS DELIVER REPORT (in the direction MS to SC)	
	1	0	SMS STATUS REPORT (in the direction SC to MS)		
	1	0	SMS COMMAND (in the direction MS to SC)	
	0	1	SMS SUBMIT (in the direction MS to SC)	
	0	1	SMS SUBMIT REPORT (in the direction SC to MS)	
	1	1	Reserved
*/
  SMS_TYPE nSMSType = SMS_RESERVED;


  switch (((pInMsg->Data[0]) & 0x03))
  {
  case 0x00:
    {
      // TP-MTI, SMS DELIVER type
      // More details in "3GPP TS 23.040 V3.9.0 (2002-06)" clause 9.2.2.1 SMS DELIVER type
      nSMSType = SMS_DELIVER;
    }
    break;
  case 0x01:
    {
      nSMSType = SMS_SUBMIT;
    }
    break;
  case 0x02:
    {
      nSMSType = SMS_STATUS_REPORT;
    }
    break;
  default:
    break;
  }
  return nSMSType;

}

 
//=====================================================================================
// FUNCTION: _SMS_GetUDL
//
//=====================================================================================
static UINT8 _SMS_GetUDL(api_SmsPPReceiveInd_t* pInMsg)
{
  return (pInMsg->Data[1 + _SMS_GetOALen(pInMsg) + 1 + 1 + 7]);
}

 
//=====================================================================================
// FUNCTION: _SMS_GetUDHL
//
//=====================================================================================
static UINT8 _SMS_GetUDHL(api_SmsPPReceiveInd_t* pInMsg)
{
  return (pInMsg->Data[1 + _SMS_GetOALen(pInMsg) + 1 + 1 + 7 + 1]);
}

 
//=====================================================================================
// FUNCTION: _SMS_GetDCS
//
//=====================================================================================
static UINT8 _SMS_GetDCS(api_SmsPPReceiveInd_t* pInMsg)
{

    CSW_TRACE(CFW_SMS_TS_ID, "_SMS_GetDCS = %x\r\n", pInMsg->Data[1 + _SMS_GetOALen(pInMsg) + 1]);

  return (pInMsg->Data[1 + _SMS_GetOALen(pInMsg) + 1]);
}

 
//=====================================================================================
// FUNCTION: _SMS_GetUDAddr
//
//=====================================================================================
static UINT8* _SMS_GetUDAddr(api_SmsPPReceiveInd_t* pInMsg)
{

  UINT8 nClassFlag   = 0x00;
  UINT8 nAlphabet    = 0x00;
  UINT8 isCompressed = 0x00;

  // UINT8 nUDLen      = _SMS_GetUDL( pInMsg );
  UINT8 isUDHI = _SMS_HaveUDHI(pInMsg);
  UINT8 nUDHL  = _SMS_GetUDHL(pInMsg);

  UINT32 nRet = sms_mt_SmsParseDCS(_SMS_GetDCS(pInMsg), &nClassFlag, &nAlphabet, &isCompressed);

  if (ERR_SUCCESS != nRet)
  {
    // Should not be here
  }
  CSW_TRACE(CFW_SMS_TS_ID, "The isUDHI = %d,the nUDHL = %d\r\n", isUDHI,nUDHL);
  CSW_TRACE(CFW_SMS_TS_ID, "The isCompressed  = %x,the nAlphabet  = %x, the nClassFlag  =  %x\r\n", isCompressed,nAlphabet,nClassFlag);
  if (isCompressed) // ???TO DO
  {
    return& (pInMsg->Data[1 + _SMS_GetOALen(pInMsg) + 1 + 1 + 7 + 1]);
  }

  if (isUDHI & (nAlphabet == MSG_GSM_7))
  {
    return& (pInMsg->Data[1 + _SMS_GetOALen(pInMsg) + 1 + 1 + 7 + 1]);
  }
  else if (isUDHI)
  {
    return& (pInMsg->Data[1 + _SMS_GetOALen(pInMsg) + 1 + 1 + 7 + 1 + nUDHL + 1]);
  }
  else
  {
    return& (pInMsg->Data[1 + _SMS_GetOALen(pInMsg) + 1 + 1 + 7 + 1]);
  }

}

 
//=====================================================================================
// FUNCTION: _SMS_GetPID
//
//=====================================================================================
static UINT8 _SMS_GetPID(api_SmsPPReceiveInd_t* pInMsg)
{
    CSW_TRACE(CFW_SMS_TS_ID, "_SMS_GetPID  =  %x\r\n", pInMsg->Data[1 + _SMS_GetOALen(pInMsg)]);

  return pInMsg->Data[1 + _SMS_GetOALen(pInMsg)];
}

 
//=====================================================================================
// FUNCTION: _SMS_GetRealUDL
//
//=====================================================================================
static UINT8 _SMS_GetRealUDL(api_SmsPPReceiveInd_t* pInMsg)
{

  UINT8 nClassFlag   = 0x00;
  UINT8 nAlphabet    = 0x00;
  UINT8 isCompressed = 0x00;
  UINT8 nUDLen       = _SMS_GetUDL(pInMsg);

  UINT32 nRet = sms_mt_SmsParseDCS(_SMS_GetDCS(pInMsg), &nClassFlag, &nAlphabet, &isCompressed);

    if( ERR_SUCCESS != nRet )
    {
      //Couldnot failed.
    }

    
    if(nAlphabet == MSG_GSM_7)
    {
        CSW_TRACE(CFW_SMS_TS_ID, "_SMS_GetRealUDL  =  %x\r\n", ( nUDLen & 7 ? nUDLen * 7 / 8 + 1 : nUDLen * 7 / 8 ));

			return ( nUDLen & 7 ? nUDLen * 7 / 8 + 1 : nUDLen * 7 / 8 );
    }
		else
		{
        CSW_TRACE(CFW_SMS_TS_ID, "_SMS_GetRealUDL  =  %x\r\n", nUDLen);
			return  nUDLen;
    }
    
}

 
//=====================================================================================
// FUNCTION: _SMS_ProcessUDH
//
//=====================================================================================
static UINT32 _SMS_ProcessUDH(api_SmsPPReceiveInd_t* pInMsg, CFW_SMS_MT_INFO * pSmsMTInfo, UINT32* pPortNum,
                              UINT8* pMMSPushType)
{
  BOOL isUDHI = _SMS_HaveUDHI(pInMsg);

  if (TRUE != isUDHI)
  {
    return ERR_INVALID_PARAMETER;
  }

  UINT8 nUDHL     = _SMS_GetUDHL(pInMsg);
  UINT8* pUDHData = &(pInMsg->Data[1 + _SMS_GetOALen(pInMsg) + 1 + 1 + 7 + 1 + 1]);

  INT32 nCount = nUDHL;
  UINT8 nIEI   = 0x00;
  UINT8 nIEIL  = 0x00;

  while (nCount > 0x00)
  {
    nIEI  =* pUDHData;
    nIEIL = *(pUDHData + 1);
    if (0x00 == nIEI)
    {
      // ////////////////////////////////////////////////////////////////////////
      // Octet 1  Concatenated short message reference number.
      // This octet shall contain a modulo 256 counter indicating
      // the reference number for a particular concatenated short
      // message. This reference number shall remain constant for 
      // every short message which makes up a particular concatenated 
      // short message.
      // 
      // Octet 2  Maximum number of short messages in the concatenated short message.
      // This octet shall contain a value in the range 0 to 255 
      // indicating the total number of short messages within the
      // concatenated short message. The value shall start at 1 and 
      // remain constant for every short message which makes up the 
      // concatenated short message. If the value is zero then the 
      // receiving entity shall ignore the whole Information Element.
      // 
      // Octet 3  Sequence number of the current short message.
      // This octet shall contain a value in the range 0 to 255 
      // indicating the sequence number of a particular short 
      // message within the concatenated short message. The value 
      // shall start at 1 and increment by one for every short message
      // sent within the concatenated short message. If the value 
      // is zero or the value is greater than the value in octet 2 
      // then the receiving entity shall ignore the whole Information Element.
      // ////////////////////////////////////////////////////////////////////////////
      
      // example: data begin with UDL
      // UDL     | UDHL    | IEA   | IEAL  | MR  | total cnt   | curr id   | UD
      // a0      | 05      |  00   | 03    | 3e  |    02       |  01       | 6e b5 b3 9a 
      
      pSmsMTInfo->nLongerMsgID      = *(pUDHData + 1 + 1);
      pSmsMTInfo->nLongerMsgCount   = *(pUDHData + 1 + 1 + 1);
      pSmsMTInfo->nLongerMsgCurrent = *(pUDHData + 1 + 1 + 1 + 1);

    }
    else if (0x01 == nIEI)
    {

    }
    else if (0x02 == nIEI)
    {

    }
    else if (0x03 == nIEI)
    {

    }
    else if (0x04 == nIEI)
    {
      *pPortNum     = ((*(pUDHData + 1 + 1)) << 16) + (*(pUDHData + 1 + 1 + 1));
      *pMMSPushType = nIEI;

    }
    else if (0x05 == nIEI)
    {
      SUL_MemCopy8(pPortNum, (pUDHData + 1 + 1), 4);
      *pMMSPushType = nIEI;
    }
    else if (0x06 == nIEI)
    {

    }
    else if (0x07 == nIEI)
    {

    }
    else if (0x08 == nIEI)
    {
      // Octet 1-2  Concatenated short messages, 16-bit reference number.
      // This octet shall contain a modulo 65536 counter indicating
      // the reference number for a particular enhanced concatenated 
      // short message. This reference number shall remain constant 
      // for every short message which makes up a particular enhanced
      // concatenated short message.
      // 
      // Octet 3  Maximum number of short messages in the enhanced concatenated short message.
      // This octet shall contain a value in the range 0 to 255 
      // indicating the total number of short messages within the 
      // concatenated short message. The value shall start at 1 and 
      // remain constant for every short message which makes up the 
      // enhanced concatenated short message. If the value is zero 
      // then the receiving entity shall ignore the whole Information Element.
      // 
      // Octet 4  Sequence number of the current short message.
      // This octet shall contain a value in the range 0 to 255
      // indicating the sequence number of a particular short message
      // within the concatenated short message. The value shall start
      // at 1 and increment by one for every short message sent within
      // the concatenated short message. If the value is zero or the 
      // value is greater than the value in octet 3 then the receiving
      // entity shall ignore the whole Information Element.

      pSmsMTInfo->nLongerMsgID      = (*(pUDHData + 1 + 1) << 8) + *(pUDHData + 1 + 1 + 1);
      pSmsMTInfo->nLongerMsgCount   = *(pUDHData + 1 + 1 + 1 + 1);
      pSmsMTInfo->nLongerMsgCurrent = *(pUDHData + 1 + 1 + 1 + 1 + 1);

    }
    else if (0x09 == nIEI)
    {

    }
    else if (0x0A == nIEI)
    {

    }
    else if (0x0B == nIEI)
    {

    }
    else if (0x0C == nIEI)
    {

    }
    else if (0x0D == nIEI)
    {

    }
    else if (0x0E == nIEI)
    {

    }
    else if (0x0F == nIEI)
    {

    }
    else if (0x10 == nIEI)
    {

    }
    else if (0x11 == nIEI)
    {

    }
    else if (0x12 == nIEI)
    {

    }
    else if ((0x13 <= nIEI) && (0x1F >= nIEI))
    {

    }
    else if (0x20 == nIEI)
    {

    }
    else if ((0x21 <= nIEI) && (0x6F >= nIEI))
    {

    }
    else if ((0x70 <= nIEI) && (0x7F >= nIEI))
    {

    }
    else if ((0x80 <= nIEI) && (0x9F >= nIEI))
    {

    }
    else if ((0xA0 <= nIEI) && (0xBF >= nIEI))
    {

    }
    else if ((0xC0 <= nIEI) && (0xDF >= nIEI))
    {

    }
    else if ((0xE0 <= nIEI) && (0xFF == nIEI))
    {

    }
    nCount   = nCount - 2 - nIEIL;
    pUDHData = pUDHData + 2 + nIEIL;

  }

  if (nCount == 0x00)
  {
    return ERR_SUCCESS;
  }
  else
  {
    return ERR_INVALID_PARAMETER;
  }


}


//=====================================================================================
// FUNCTION: sms_mt_Parse_SmsPPReceiveInd
//
//=====================================================================================
UINT32 sms_mt_Parse_SmsPPReceiveInd(HAO hAo)
{
	CFW_SMS_MT_INFO*       pSmsMTInfo  = NULL;  //MT private data
	api_SmsPPReceiveInd_t* pInMsg      = NULL;
	CFW_NEW_SMS_NODE*      pSmsNode    = NULL;
	UINT8                  nFormat=0x0, isUDHI=0x0;
	UINT32                 nStructureSize=0x0; 
	UINT32                 nParam2 = 0x0;
	//UINT16                 nMR = 0X0;
	UINT8 nPID       = 0x00;

	CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd enter\n ");
#ifndef CFW_MULTI_SIM
	CSW_PROFILE_FUNCTION_ENTER(sms_mt_Parse_SmsPPReceiveInd);
#else
	CFW_SIM_ID nSimId = CFW_SIM_COUNT;
	CSW_PROFILE_FUNCTION_ENTER(sms_mt_Parse_SmsPPReceiveInd);
	CFW_GetSimID(hAo, &nSimId);
#endif

	//Get private date
	pSmsMTInfo = CFW_GetAoUserData(hAo);
	if(!pSmsMTInfo)
	{
		CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd);
		return ERR_NO_MORE_MEMORY;
	}

	//Pointer to pInMsg data
	pInMsg = pSmsMTInfo->pInMsg;
	CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, pInMsg->Address, pInMsg->AddressLen, 16);
	CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, pInMsg->Data, pInMsg->DataLen, 16);
	//Get SMS format from user setting
#ifndef CFW_MULTI_SIM
	CFW_CfgGetSmsFormat(&nFormat);
#else
	CFW_CfgGetSmsFormat(&nFormat, nSimId);
#endif

	isUDHI = _SMS_HaveUDHI(pInMsg);
	SMS_TYPE nSMSType = SMS_RESERVED;

	nSMSType = _SMS_GetSMSType(pInMsg);

	if (SMS_DELIVER == nSMSType)
	{
		CFW_SMS_PDU_INFO*      pSmsPDU     = NULL;  //PDU Format
		CFW_SMS_TXT_HRD_V1_IND* pSmsTxtDeliver = NULL;  //Text format with head
		CFW_SMS_TXT_NO_HRD_IND* pSmsTxtNoHrd   = NULL;  //Text format no head
		UINT8*                  pSmsDataProc   = NULL;  // for pInMsg->Data processing, decode
		UINT16  nDestUDL   = 0x0, nLastLongerMsgSize=0x0;
		UINT8   isCompressed = 0x0, nOALen = 0x0 ,nOctUDL=0x0, nUDHL=0x0;
		UINT8   nSeptetsUDL; //xueww[+] 2007.03.21
		UINT8   nClassFlag   = MSG_CLASS_RESERVED;  //Message Class
		UINT8   nAlphabet    = MSG_RESERVED;    //Alphabet
		UINT32  ret=0x0;
  	UINT8 nMMSPushType = 0x00;

		pSmsMTInfo->nSmsType = 1; //Set DELIVERS type
		nOALen = _SMS_GetOALen(pInMsg);

		nSeptetsUDL = _SMS_GetUDL(pInMsg);

			//TP-DCS, parse it
		nPID = _SMS_GetPID(pInMsg);
		sms_mt_SmsParseDCS(_SMS_GetDCS(pInMsg), &nClassFlag, &nAlphabet, &isCompressed);

		nOctUDL = _SMS_GetRealUDL(pInMsg);
		//For the user data with head, get nOctUDL
		if(isUDHI) 
		{
			nUDHL = _SMS_GetUDHL(pInMsg);
		}

		if(isCompressed)// ???TO DO
		{
			ret = sms_mt_CompressedDataDecode(nAlphabet, nOctUDL, _SMS_GetUDAddr(pInMsg), &pSmsDataProc, &nDestUDL);
		}
		else//uncompressed
		{
			if(isUDHI & (nAlphabet == MSG_GSM_7))
			{
				//decode the long msg with its head info
				ret = sms_mt_UncompressedDataDecode(nAlphabet, nOctUDL, _SMS_GetUDAddr(pInMsg), &pSmsDataProc, &nDestUDL);
				//then offset the confused code(head info),get the right data
				pSmsDataProc  = pSmsDataProc + nUDHL;

				//example: data begin with UDL
				//UDL   | UHDL | IEA  | IEAL | MR | total cnt  | curr id  | UD
				//a0     | 05     |  00   | 03    | 3e  |    02       |  01       | 6e b5 b3 9a 
				//nDestUDL = 160 - ((5+1)*8+6)/7 = 153;
				nDestUDL = nSeptetsUDL - ((nUDHL+1)*8+6)/7;

				CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd withUDHI&7BIT  nDestUDL= %d\n ",nDestUDL);
			}
			else if(isUDHI) //8 bits or 16 bits UCS2
			{
				ret = sms_mt_UncompressedDataDecode(nAlphabet, nOctUDL - nUDHL - 1, _SMS_GetUDAddr(pInMsg),
					                                &pSmsDataProc,&nDestUDL);
				CSW_TRACE(CFW_SMS_TS_ID, "nDestUDL[%d]\n ", nDestUDL);
			}
			else
			{
		        ret = sms_mt_UncompressedDataDecode(nAlphabet, nOctUDL, _SMS_GetUDAddr(pInMsg), &pSmsDataProc, 
					                                &nDestUDL);
				nDestUDL = nSeptetsUDL;//hameina[mod] get the right lengh
				CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd NoUDHI nDestUDL[%d]\n ",nDestUDL);
			}
		}

		if(ret != ERR_SUCCESS)
		{
			pSmsDataProc = (UINT8*)CSW_SMS_MALLOC(12);  // just for free, belowing
			nDestUDL     = 0;
		}

		//For the user data with head, related with longer msg
		if(isUDHI) 
		{
			UINT32 nRet = _SMS_ProcessUDH(pInMsg, pSmsMTInfo, &nParam2, &nMMSPushType);

			if (ERR_SUCCESS != nRet)
			{
			// TODO
			}
      // Frank add start:used to MMS/PUSH  

    	CFW_EMS_INFO* pEmsInfo = (CFW_EMS_INFO*)CSW_SIM_MALLOC(sizeof(CFW_EMS_INFO));
    	if (NULL == pEmsInfo)
    	{
    		CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd);
    		return ERR_NO_MORE_MEMORY;
    	}

    	SUL_MemSet8(pEmsInfo, 0x00, sizeof(CFW_EMS_INFO));

      // Frank add end:used to MMS/PUSH
			pEmsInfo->nCurrentNum = pSmsMTInfo->nLongerMsgCurrent;
			pEmsInfo->nMaxNum     = pSmsMTInfo->nLongerMsgCount;
			pEmsInfo->nRerNum     = pSmsMTInfo->nLongerMsgID;
			nParam2               = (UINT32)pEmsInfo;
			if(pSmsMTInfo->nLongerMsgCount > 1)
			{
				//set SMS MT private data
				pSmsMTInfo->isLongerMsg = 1;
				if(pSmsMTInfo->nLongerMsgCurrent == 1)
				{
					gpLongerMsgParaIndex = (UINT32*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MT_LONGER_MSG_INDEX));
					if(!gpLongerMsgParaIndex)
					{
						CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd gpLongerMsgParaIndex malloc error!!! \n ");
						CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd);
            CSW_SMS_FREE(pEmsInfo);
            pEmsInfo = NULL;
						return ERR_NO_MORE_MEMORY;
					}
					SUL_ZeroMemory8(gpLongerMsgParaIndex, SIZEOF(CFW_SMS_MT_LONGER_MSG_INDEX));
				}
			}
			else
			{
				pSmsMTInfo->isLongerMsg = 0;
			}
			//Process a longer msg which reassembles automatically as following.
			if((pSmsMTInfo->isLongerMsg) && (pSmsMTInfo->isListLongFirst))
			{
				//???为了处理方便,我们这里做一个假设,在接收一个超长短信时,不会另一个超长短信插入
				//若这个假设错误,下边的设计将要重新考虑.
				//这个假设还包括:收到长短信的几个后,不能关机,然后开机再收
				//When receiving the first part of a longer msg, malloc the memory 
				if(pSmsMTInfo->nLongerMsgCurrent == 1)
				{
					gpLongerMsgPara = (UINT32*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MT_LONGER_MSG));
					if(!gpLongerMsgPara)
					{
						CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd gpLongerMsgPara malloc error!!! \n ");
						CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd);
            CSW_SMS_FREE(pEmsInfo);
            pEmsInfo = NULL;
						return ERR_NO_MORE_MEMORY;
					}
					SUL_ZeroMemory8(gpLongerMsgPara, SIZEOF(CFW_SMS_MT_LONGER_MSG));
					//Get gLongerMsgID from the first Msg of Longer msg
					((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->gLongerMsgID = pSmsMTInfo->nLongerMsgID;
					//Malloc a max memory for the longer message
					((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBuf = \
						(UINT8*)CSW_SMS_MALLOC(pSmsMTInfo->nLongerMsgCount * nDestUDL);
					if(!((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBuf)
					{
						CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd (!((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBuf) malloc error!!! \n ");
						CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd);
            CSW_SMS_FREE(pEmsInfo);
            pEmsInfo = NULL;
						return ERR_NO_MORE_MEMORY;
					}
					SUL_ZeroMemory8(((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBuf,(pSmsMTInfo->nLongerMsgCount * nDestUDL));
					((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->nTmp = (UINT32)(((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBuf);
					((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBufBackup = \
						((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBuf;
				}

				if(!gpLongerMsgPara)
				{
					CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd);
          CSW_SMS_FREE(pEmsInfo);
          pEmsInfo = NULL;
					return ERR_NO_MORE_MEMORY;
				}

				//Caculate the real size of long message
				((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->gLongerMsgSize = \
					(UINT16)(((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->gLongerMsgSize + nDestUDL);

				//Copy a msg data to the max memory
				SUL_MemCopy8(((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBuf,pSmsDataProc,(UINT32)nDestUDL);
				//point to next position
				((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBuf = \
					((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBuf + nDestUDL;

				//Remeber the count and current of a longer msg
				((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->gLongerMsgCount   =  pSmsMTInfo->nLongerMsgCount;
				((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->gLongerMsgCurrent =  pSmsMTInfo->nLongerMsgCurrent;

				//Get the whole size of a longer msg
				if(pSmsMTInfo->nLongerMsgCurrent == pSmsMTInfo->nLongerMsgCount)
				{
					//Give the length(with head size) of the last part of a longer msg
					//                        nLastLongerMsgSize = (UINT16)(nDestUDL + 1 + nUDHL);
					nLastLongerMsgSize = (UINT16)(nDestUDL + nUDHL);
					nDestUDL = ((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->gLongerMsgSize; 
				}
			}
			//                }
		}// end of  if(isUDHI) 

		//SMS-DELIVERs, except class 2 messages and messages in the message waiting indication group (store message) are routed directly to the TE using unsolicited result code:
		if(nClassFlag != MSG_CLASS_2)
		{
			if(nFormat == 0) //PDU mode
			{
				if( (!pSmsMTInfo->isLongerMsg)                                   \
					|| (pSmsMTInfo->isLongerMsg && !pSmsMTInfo->isListLongFirst))
				{
					//Malloc CFW_SMS_PDU_INFO for PDU DELIVERs (CFW --> AT)
					//System will free the memory after get Msg
					nStructureSize = (SIZEOF(CFW_NEW_SMS_NODE)+SIZEOF(CFW_SMS_PDU_INFO)+pInMsg->DataLen+pInMsg->AddressLen+1);
					pSmsNode = (CFW_NEW_SMS_NODE*)CSW_SMS_MALLOC(nStructureSize);
					if(!pSmsNode)
					{
						CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd nFormat[0] pSmsNode malloc error!!! \n ");
						CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd);
            if( NULL !=  (VOID *)nParam2)
            {
               CSW_SMS_FREE(nParam2);
               nParam2 = 0x00;
            }
						return ERR_NO_MORE_MEMORY;
					}
					SUL_ZeroMemory8(pSmsNode, nStructureSize);
					pSmsNode->pNode = (CFW_SMS_PDU_INFO*)((UINT8*)pSmsNode + SIZEOF(CFW_NEW_SMS_NODE));
					//Set pSmsPDU Value
					pSmsPDU = (CFW_SMS_PDU_INFO*)(pSmsNode->pNode);
					pSmsPDU->nDataSize = (UINT16)(pInMsg->DataLen + pInMsg->AddressLen+1);

					pSmsPDU->nTpUdl    = (UINT16)(nOctUDL);
					pSmsPDU->nStatus   = CFW_SMS_STORED_STATUS_UNREAD;
					//The Data field is the SM-TL PDU as defined in 3GPP TS 23.040 v3.10.0 Technical realization of the Short Message Service.
					//It conveys one of the SMS-DELIVER SM-TL PDU.
					//Pointer to data buffer to hold PDU(GSM 04.11 SC address followed by GSM 03.40 TPDU in hexadecimal format). 
					pSmsPDU->pData[0] = pInMsg->AddressLen;
					SUL_MemCopy8(pSmsPDU->pData+1, pInMsg->Address, (UINT32)pInMsg->AddressLen);
					SUL_MemCopy8((pSmsPDU->pData+pInMsg->AddressLen+1), pInMsg->Data, (UINT32)pInMsg->DataLen);
					//Send a MSG to AT , nType = 0,CFW_NEW_SMS_NODE.nType =0x20   
					//
					//P2 表示是否为超常，如果函数有头UDH，认为为超常短信，不论总数有几条。
					//
#ifndef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode,(UINT32)nParam2, SMS_MT_UTI, 0);//xueww[mod] 2007.06.13 /(param2 = isUDHI)--->(param2 = nParam2)
#else
					CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode,(UINT32)nParam2, SMS_MT_UTI |(nSimId<<24), 0);
#endif
				}
				//When got the last part of a longer msg, Reassemble all together,and send it
				if(pSmsMTInfo->isLongerMsg && pSmsMTInfo->isListLongFirst && gpLongerMsgPara &&\
					(pSmsMTInfo->nLongerMsgCurrent == pSmsMTInfo->nLongerMsgCount))
				{
					UINT32 nMemTmp=0x0; 
					//Malloc CFW_SMS_PDU_INFO for PDU DELIVERs (CFW --> AT)
					//"1" means the length of SMSC address Length
					nStructureSize = SIZEOF(CFW_NEW_SMS_NODE) + SIZEOF(CFW_SMS_PDU_INFO) + 1 + pInMsg->AddressLen\
					+ pInMsg->DataLen - nLastLongerMsgSize  + nDestUDL; 
					pSmsNode = (CFW_NEW_SMS_NODE*)CSW_SMS_MALLOC(nStructureSize);
					if(!pSmsNode)
					{
						CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd pSmsNode malloc error!!! \n ");
						CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd);
            if( NULL !=  (VOID *)nParam2)
            {
               CSW_SMS_FREE(nParam2);
               nParam2 = 0x00;
            }
						return ERR_NO_MORE_MEMORY;
					}
					SUL_ZeroMemory8(pSmsNode, nStructureSize);
					pSmsNode->pNode = (CFW_SMS_PDU_INFO*)((UINT8*)pSmsNode + SIZEOF(CFW_NEW_SMS_NODE));
					//Set pSmsPDU Value
					pSmsPDU = (CFW_SMS_PDU_INFO*)(pSmsNode->pNode);
					pSmsPDU->nDataSize = (UINT16)(pInMsg->DataLen  + pInMsg->AddressLen - nLastLongerMsgSize + nDestUDL);
					pSmsPDU->nTpUdl    = nDestUDL;
					//The Data field is the SM-TL PDU as defined in 3GPP TS 23.040 v3.10.0 Technical realization of the Short Message Service.
					//It conveys one of the SMS-DELIVER SM-TL PDU.
					//Pointer to data buffer to hold PDU(GSM 04.11 SC address followed by GSM 03.40 TPDU in hexadecimal format). 
					//Copy SMSC address Length
					SUL_MemCopy8(pSmsPDU->pData, &(pInMsg->AddressLen), 1); 
					//Copy SMSC address octets
					SUL_MemCopy8((pSmsPDU->pData + 1), pInMsg->Address, (UINT32)pInMsg->AddressLen); 
					//Copy the data in PDU before TP-UDL(include it, note: this UDL is not used)
					SUL_MemCopy8((pSmsPDU->pData + 1 + pInMsg->AddressLen), pInMsg->Data,(UINT32)(pInMsg->DataLen - nLastLongerMsgSize));
					//Copy all the TP-UD
					SUL_MemCopy8((pSmsPDU->pData + 1 + pInMsg->AddressLen + pInMsg->DataLen - nLastLongerMsgSize), \
						((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBufBackup, (UINT32)pSmsPDU->nTpUdl);

					//Send a MSG to AT , nType = 0,CFW_NEW_SMS_NODE.nType =0x20 
#ifndef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode,(UINT32)nParam2, SMS_MT_UTI, 0);
#else
					CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode,(UINT32)nParam2, SMS_MT_UTI |(nSimId<<24), 0);
#endif
					//Free pgBuf
					nMemTmp = ((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->nTmp;
					CSW_SMS_FREE((UINT8*)nMemTmp);
					//Free the memory which points to a longer msg
					CSW_SMS_FREE(gpLongerMsgPara);
				}
				//Set pSmsNode Type
				if(pSmsNode)
					pSmsNode->nType  = 0x20;
			}
			else  //Text mode
			{
				UINT8 nShow = 0x0;

				//Get whether or not detailed header information is shown in SMS text mode.
#ifndef CFW_MULTI_SIM
				CFW_CfgGetSmsShowTextModeParam(&nShow);
#else
				CFW_CfgGetSmsShowTextModeParam(&nShow, nSimId);
#endif
				if(nShow == 1)//For AT :with head information  
				{
					UINT8 nLen_oa=0x0,nLen_sca=0x0;
					//Malloc CFW_SMS_TXT_HRD_V1_IND for Text Status Report (CFW --> AT)
					//System will free the memory after get Msg
					//For a normal msg or a longer msg which user will reassemble it
					if( (!pSmsMTInfo->isLongerMsg)                                   \
						|| (pSmsMTInfo->isLongerMsg && !pSmsMTInfo->isListLongFirst) \
						|| (pSmsMTInfo->isLongerMsg && pSmsMTInfo->isListLongFirst && (pSmsMTInfo->nLongerMsgCurrent == pSmsMTInfo->nLongerMsgCount)))
					{
						nLen_oa   = (UINT8)(pInMsg->Data[1] / 2 + pInMsg->Data[1] % 2); //pInMsg->Data[1] + 1;  // 3GPP TS 23.040 v3.10.0
						// nLen_sca  = (UINT8)(2*(pInMsg->AddressLen-1));  // StackCom Doc(p87) and 3GPP TS 24.011 v3.6.0
						nLen_sca  = pInMsg->AddressLen - 1;  // //BCD size
						//Memalloc
						nStructureSize = SIZEOF(CFW_NEW_SMS_NODE) + SIZEOF(CFW_SMS_TXT_HRD_V1_IND) + nDestUDL;
						pSmsNode = (CFW_NEW_SMS_NODE*)CSW_SMS_MALLOC(nStructureSize);
						if(!pSmsNode)
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd long pSmsNode malloc error!!! \n ");
							CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd);
              if( NULL !=  (VOID *)nParam2)
              {
                 CSW_SMS_FREE(nParam2);
                 nParam2 = 0x00;
              }
							return ERR_NO_MORE_MEMORY;
						}
						SUL_ZeroMemory8(pSmsNode, nStructureSize);
						pSmsNode->pNode = (CFW_SMS_TXT_HRD_V1_IND*)((UINT8*)pSmsNode + SIZEOF(CFW_NEW_SMS_NODE));
						//Set pSmsPDU Value
						pSmsTxtDeliver = (CFW_SMS_TXT_HRD_V1_IND*)(pSmsNode->pNode);
						//Set all the data to Structure CFW_SMS_TXT_HRD_V1_IND
						if(pInMsg->Data[1] > 0)
						{
							SUL_MemCopy8(pSmsTxtDeliver->oa, &pInMsg->Data[3], nLen_oa);
							pSmsTxtDeliver->oa_size = nLen_oa;  //BCD size
						}
						pSmsTxtDeliver->nIndex = pSmsMTInfo->nIndex;

						// Set pSmsNode Type
						if (pSmsNode)
						{
							if (0x00 == nMMSPushType)
							{
								pSmsNode->nType = 0x21;
							}
							else
							{
								pSmsNode->nType = nMMSPushType;
							}

						}

			            CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd, pSmsNode->nType 0x%x \n ", pSmsNode->nType);

						//hameina
						CSW_TRACE(CFW_SMS_TS_ID, "SmsPPReceiveInd Data[1+nOALen+1+1]= 0x%x \n ", pInMsg->Data[1+nOALen+1+1]);
						CSW_TRACE(CFW_SMS_TS_ID, "SmsPPReceiveInd Data[1+nOALen+1+2]= 0x%x \n ", pInMsg->Data[1+nOALen+1+2]);
						CSW_TRACE(CFW_SMS_TS_ID, "SmsPPReceiveInd Data[1+nOALen+1+3]= 0x%x \n ", pInMsg->Data[1+nOALen+1+3]);
						CSW_TRACE(CFW_SMS_TS_ID, "SmsPPReceiveInd Data[1+nOALen+1+4]= 0x%x \n ", pInMsg->Data[1+nOALen+1+4]);
						CSW_TRACE(CFW_SMS_TS_ID, "SmsPPReceiveInd Data[1+nOALen+1+5]= 0x%x \n ", pInMsg->Data[1+nOALen+1+5]);
						CSW_TRACE(CFW_SMS_TS_ID, "SmsPPReceiveInd Data[1+nOALen+1+6]= 0x%x \n ", pInMsg->Data[1+nOALen+1+6]);
						CSW_TRACE(CFW_SMS_TS_ID, "SmsPPReceiveInd Data[1+nOALen+1+7]= 0x%x \n ", pInMsg->Data[1+nOALen+1+7]);

						sms_mt_SerializeTime(&pSmsTxtDeliver->scts,&pInMsg->Data[1+nOALen+1+1]);//TP Service Centre Time Stamp
						// 3GPP TS 23.040 v3.10.0 "Clause9.1.2.5"
						//[[xueww[mod] 2007.08.08 (pInMsg->Data[3] & 0x90) --->(pInMsg->Data[3]>>4)......
						if((pInMsg->Data[2]>>4) == 9) //International number(bit7~bit0) : 1001 xxxx
							pSmsTxtDeliver->tooa  = CFW_TELNUMBER_TYPE_INTERNATIONAL;
						else if((pInMsg->Data[2]>>4) == 0x0a) //National number(bit7~bit0) : 1010 xxxx
							pSmsTxtDeliver->tooa  = CFW_TELNUMBER_TYPE_NATIONAL;
						else if((pInMsg->Data[2]>>4) == 0x0d) //National number(bit7~bit0) : 1101 0000//xueww[+] 2007.08.01
							pSmsTxtDeliver->tooa  = 0xd0;
						else
							pSmsTxtDeliver->tooa  = CFW_TELNUMBER_TYPE_UNKNOWN;
						//]]xueww[mod] 2007.08.08
						//Specify the depending on the command or result code: first octet of GSM 03.40 
						//SMS- DELIVER, SMS-SUBMIT (default 17), SMS-STATUS-REPORT, 
						//or SMS-COMMAND (default 2) in integer format. 
						pSmsTxtDeliver->fo  = 17;//pInMsg->Data[0]; 
						// 3GPP TS 23.040 v3.10.0 "Clause9.2.3.9"
						//(bit7~bit0) : 0010 0010 or 0010 0011 
						if((pInMsg->Data[1+nOALen]  == 0x22) | (pInMsg->Data[1+nOALen]  == 0x23)) 
							pSmsTxtDeliver->pid  = 1; //Fax message.
						else if(pInMsg->Data[1+nOALen]  == 0x22) //(bit7~bit0) : 0011 0001
							pSmsTxtDeliver->pid  = 2; //X.400 message
						else if(pInMsg->Data[1+nOALen]  == 0x26) //(bit7~bit0) : 0010 0110
							pSmsTxtDeliver->pid  = 3; //Paging message.
						else if(pInMsg->Data[1+nOALen]  == 0x32) //(bit7~bit0) : 0011 0010
							pSmsTxtDeliver->pid  = 4; //Email message.
						else if(pInMsg->Data[1+nOALen]  == 0x25) //(bit7~bit0) : 0010 0101
							pSmsTxtDeliver->pid  = 5; //ERMES(European Radio Messaging System) message.
						else if(pInMsg->Data[1+nOALen]  == 0x24) //(bit7~bit0) : 0010 0100
							pSmsTxtDeliver->pid  = 6; //Voice message.
						else
							pSmsTxtDeliver->pid  = 0; //Default message transport protocol
						//Bit 5, if set to 1, indicates the text is compressed using the compression algorithm defined in 3G TS 23.042 [13]
						if(isCompressed)
						{
							if(nAlphabet == MSG_GSM_7)
								pSmsTxtDeliver->dcs = 16;//GSM 7 bit default alphabet and compressed 
							else if(nAlphabet == MSG_8_BIT_DATA) //8 bit data
								pSmsTxtDeliver->dcs = 17;//8 bit data and compressed
							else if(nAlphabet == MSG_UCS2) 
								pSmsTxtDeliver->dcs = 18;//UCS2 (16bit) and compressed,ISO/IEC10646 [10 ] 
						}
						else
						{
							if(nAlphabet == MSG_GSM_7)
								pSmsTxtDeliver->dcs = 0;//GSM 7 bit default alphabet and uncompressed 
							else if(nAlphabet == MSG_8_BIT_DATA)
								pSmsTxtDeliver->dcs = 1;//8 bit data and uncompressed 
							else if(nAlphabet == MSG_UCS2)  
								pSmsTxtDeliver->dcs = 2;//UCS2 (16bit) and uncompressed,ISO/IEC10646 [10 ] 
						}
						if(pInMsg->AddressLen > 2)
						{
							SUL_MemCopy8(pSmsTxtDeliver->sca, &pInMsg->Address[1], nLen_sca);
							pSmsTxtDeliver->sca_size = nLen_sca;  //BCD size
						}
						// 3GPP TS 23.040 v3.10.0 "Clause9.1.2.5"
						//[[xueww[mod] 2007.08.08 (pInMsg->Data[3] & 0x90) --->(pInMsg->Data[3]>>4)......
						if((pInMsg->Address[0]>>4) == 9) //International number(bit7~bit0) : 1001 xxxx
							pSmsTxtDeliver->tosca  = CFW_TELNUMBER_TYPE_INTERNATIONAL;
						else if((pInMsg->Address[0]>>4) == 0x0a) //National number(bit7~bit0) : 1010 xxxx
							pSmsTxtDeliver->tosca  = CFW_TELNUMBER_TYPE_NATIONAL;
						else if((pInMsg->Address[0]>>4) == 0x0d) //National number(bit7~bit0) : 1101 0000//xueww[+] 2007.08.01
							pSmsTxtDeliver->tosca  = 0xd0;
						else
							pSmsTxtDeliver->tosca  = CFW_TELNUMBER_TYPE_UNKNOWN;
						//]]xueww[mod] 2007.08.08
					}

					//For a normal msg or a longer msg which user will reassemble it
					if((!pSmsMTInfo->isLongerMsg) || (pSmsMTInfo->isLongerMsg && !pSmsMTInfo->isListLongFirst))
					{
						pSmsTxtDeliver->nDataLen = nDestUDL;
						SUL_MemCopy8(pSmsTxtDeliver->pData, pSmsDataProc, (UINT32)nDestUDL);
						//Sent a msg to AT, nType = 0,CFW_NEW_SMS_NODE.nType =0x21 
						CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd, CFW_SMS_TXT_HRD_V1_IND, before call func CFW_PostNotifyEvent!pSmsTxtDeliver->tooa = 0x%x. \n ",pSmsTxtDeliver->tooa);
						SXS_DUMP(CFW_SMS_TS_ID, "%2x", pSmsTxtDeliver->oa, pSmsTxtDeliver->oa_size);

			            // Set pSmsNode Type
			            if (0x00 == nMMSPushType)
			            {
              CSW_TRACE(CFW_SMS_TS_ID, "Normal SMS \n ");
							CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, (UINT8 *) pSmsTxtDeliver->pData, 10, 16);
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode, nParam2, SMS_MT_UTI, 0);  // xueww[mod] 2007.06.13 /(param2 = isUDHI)--->(param2 = nParam2)
#else
							CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode, nParam2, SMS_MT_UTI|(nSimId<<24), 0);   // Modify by lixp at 20090513 for Dual sim issue
#endif
			            }
			            else
			            {
              CSW_TRACE(CFW_SMS_TS_ID, "PUSH SMS\n ");
							CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, (UINT8 *) pSmsTxtDeliver->pData, 10, 16);
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_NEW_EMS_IND, (UINT32)pSmsNode, nParam2, SMS_MT_UTI, 0);  // xueww[mod] 2007.06.13 /(param2 = isUDHI)--->(param2 = nParam2)
#else
							CFW_PostNotifyEvent(EV_CFW_NEW_EMS_IND, (UINT32)pSmsNode, nParam2, SMS_MT_UTI|(nSimId<<24), 0);  // Modify by lixp at 20090513 for Dual sim issue
#endif
			            }

					}

					//When got the last part of a longer msg, Reassemble all together,and send it
					if(pSmsMTInfo->isLongerMsg && pSmsMTInfo->isListLongFirst && gpLongerMsgPara && \
						(pSmsMTInfo->nLongerMsgCurrent == pSmsMTInfo->nLongerMsgCount))
					{
						UINT32 nMemTmp=0x0;
						pSmsTxtDeliver->nDataLen = ((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->gLongerMsgSize;
						SUL_MemCopy8(pSmsTxtDeliver->pData, ((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBufBackup,(UINT32)pSmsTxtDeliver->nDataLen);
						//Sent a msg to AT, nType = 0,CFW_NEW_SMS_NODE.nType =0x21 
						// Frank modify start:used to MMS/PUSH
						// Set pSmsNode Type
						if (0x00 == nMMSPushType)
						{
							CSW_TRACE(CFW_SMS_TS_ID, "0x00 == nMMSPushType\n ");
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode, nParam2, SMS_MT_UTI, 0);  // xueww[mod] 2007.06.13 /(param2 = isUDHI)--->(param2 = nParam2)
#else
							CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode, nParam2, SMS_MT_UTI|(nSimId<<24), 0);  //  // Modify by lixp at 20090513 for Dual sim issue
#endif
						}
						else
						{
							CSW_TRACE(CFW_SMS_TS_ID, "0x00 != nMMSPushType\n ");
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_NEW_EMS_IND, (UINT32)pSmsNode, nParam2, SMS_MT_UTI, 0);  // xueww[mod] 2007.06.13 /(param2 = isUDHI)--->(param2 = nParam2)
#else
							CFW_PostNotifyEvent(EV_CFW_NEW_EMS_IND, (UINT32)pSmsNode, nParam2, SMS_MT_UTI|(nSimId<<24), 0);  //  // Modify by lixp at 20090513 for Dual sim issue
#endif
						}

						//Free pgBuf
						nMemTmp = ((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->nTmp;
						CSW_SMS_FREE((UINT8*)nMemTmp);
						//Free the memory which points to a longer msg
						CSW_SMS_FREE(gpLongerMsgPara);
					}
					//Set pSmsNode Type
					if(pSmsNode)
						pSmsNode->nType  = 0x21;
				}
				else  // For AT:no head information
				{
					UINT8 nLen_oa=0x0;
					//Malloc CFW_SMS_TXT_NO_HRD_IND for Text Status Report (CFW --> AT)
					//System will free the memory after get Msg
					if( (!pSmsMTInfo->isLongerMsg)                                   \
						|| (pSmsMTInfo->isLongerMsg && !pSmsMTInfo->isListLongFirst) \
						|| (pSmsMTInfo->isLongerMsg && pSmsMTInfo->isListLongFirst \
						&& (pSmsMTInfo->nLongerMsgCurrent == pSmsMTInfo->nLongerMsgCount)))
					{
						nLen_oa   = (UINT8)(pInMsg->Data[1] / 2 + pInMsg->Data[1] % 2); // 3GPP TS 23.040 v3.10.0
						//Memory allocate
						nStructureSize = SIZEOF(CFW_NEW_SMS_NODE) + SIZEOF(CFW_SMS_TXT_NO_HRD_IND) + nDestUDL;
						pSmsNode = (CFW_NEW_SMS_NODE*)CSW_SMS_MALLOC(nStructureSize);
						if(!pSmsNode)
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd pSmsNode malloc error!!! \n ");
							CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd);
              if( NULL !=  (VOID *)nParam2)
              {
                 CSW_SMS_FREE(nParam2);
                 nParam2 = 0x00;
              }
							return ERR_NO_MORE_MEMORY;
						}
						SUL_ZeroMemory8(pSmsNode, nStructureSize);
						pSmsNode->pNode = (CFW_SMS_TXT_NO_HRD_IND*)((UINT8*)pSmsNode + SIZEOF(CFW_NEW_SMS_NODE));
						//Set pSmsPDU Value
						pSmsTxtNoHrd = (CFW_SMS_TXT_NO_HRD_IND*)(pSmsNode->pNode);

						//Set all the data to Structure CFW_SMS_TXT_NO_HRD_IND
						if(pInMsg->Data[1] > 0)
						{
							SUL_MemCopy8(pSmsTxtNoHrd->oa, &pInMsg->Data[3], nLen_oa);
							pSmsTxtNoHrd->oa_size = nLen_oa;  //BCD size
							pSmsTxtNoHrd->tooa = pInMsg->Data[2];
						}
						pSmsTxtNoHrd->nIndex = pSmsMTInfo->nIndex;
						sms_mt_SerializeTime(&pSmsTxtNoHrd->scts,&pInMsg->Data[1+nOALen+1+1]);//TP Service Centre Time Stamp
					}

					//For a normal msg or a longer msg which user will reassemble it
					if((!pSmsMTInfo->isLongerMsg) || (pSmsMTInfo->isLongerMsg && !pSmsMTInfo->isListLongFirst))
					{
						pSmsTxtNoHrd->nDataLen = nDestUDL;
						SUL_MemCopy8(pSmsTxtNoHrd->pData, pSmsDataProc, (UINT32)nDestUDL);
						//Sent a msg to AT, nType = 0,CFW_NEW_SMS_NODE.nType =0x22 
						//[[xueww[mod] 2007.02.01 (param2 = 0)--->(param2 = isUDHI)
						//[[xueww[+] 2007.07.13				    
						CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Parse_SmsPPReceiveInd, CFW_SMS_TXT_NO_HRD_IND, before call func CFW_PostNotifyEvent!pSmsTxtNoHrd->tooa = 0x%x. \n ",pSmsTxtNoHrd->tooa);

						SXS_DUMP(CFW_SMS_TS_ID, "%2x", pSmsTxtNoHrd->oa, pSmsTxtNoHrd->oa_size);
						//xueww[+] 2007.07.13
#ifndef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode, nParam2, SMS_MT_UTI, 0);//xueww[mod] 2007.06.13 /(param2 = isUDHI)--->(param2 = nParam2)
#else
						CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode, nParam2, SMS_MT_UTI |(nSimId<<24), 0);
#endif
						//]]xueww[mod] 2007.02.01 
					}
					//When got the last part of a longer msg, Reassemble all together,and send it
					if(pSmsMTInfo->isLongerMsg && pSmsMTInfo->isListLongFirst && gpLongerMsgPara && \
						(pSmsMTInfo->nLongerMsgCurrent == pSmsMTInfo->nLongerMsgCount))
					{
						UINT32 nMemTmp=0x0;
						pSmsTxtNoHrd->nDataLen = ((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->gLongerMsgSize;
						SUL_MemCopy8(pSmsTxtNoHrd->pData, ((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->pgBufBackup,(UINT32)pSmsTxtNoHrd->nDataLen);
						//Sent a msg to AT, nType = 0,CFW_NEW_SMS_NODE.nType =0x22 
						//[[xueww[mod] 2007.02.01 (param2 = 0)--->(param2 = isUDHI)
#ifndef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode, nParam2, SMS_MT_UTI, 0);//xueww[mod] 2007.06.13 /(param2 = isUDHI)--->(param2 = nParam2)
#else
						CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode, nParam2, SMS_MT_UTI |(nSimId<<24), 0);
#endif
						//]]xueww[mod] 2007.02.01 
						//Free pgBuf
						nMemTmp = ((CFW_SMS_MT_LONGER_MSG*)gpLongerMsgPara)->nTmp;
						CSW_SMS_FREE((UINT8*)nMemTmp);
						//Free the memory which points to a longer msg
						CSW_SMS_FREE(gpLongerMsgPara);
					}
					//Set pSmsNode Type
					if(pSmsNode)
						pSmsNode->nType  = 0x22;
				}
				//Memory free for processed data

				if(pSmsDataProc)
				{
					if(isUDHI & (nAlphabet == MSG_GSM_7))
					{
						CSW_SMS_FREE(pSmsDataProc - nUDHL);
					}
					else
					{
						CSW_SMS_FREE(pSmsDataProc);
					}

				}
			}
			// end if(nFormat == 0)
			//Set pSmsNode Value
			if(pSmsNode)
			{
				if(pSmsMTInfo->isLongerMsg)
				{
					pSmsNode->nConcatPrevIndex = 0x0;
				}
				else
				{
					pSmsNode->nConcatPrevIndex = 0xffff;
				}
				pSmsNode->nConcatCurrentIndex = pSmsMTInfo->nIndex;
				pSmsNode->nStorage = pSmsMTInfo->nSmsStorageID;
			}
		} //end of if(ClassFlag != 2)

	} 

	CSW_PROFILE_FUNCTION_EXIT(sms_mt_Parse_SmsPPReceiveInd);
	return ERR_SUCCESS;
}

//Save the para of long sms to file
UINT32 sms_mt_SetLongPara(HAO hAo)
{
    CFW_SMS_MT_INFO*      pSmsMTInfo = NULL; //Point to MT private data.
    CFW_SMS_LONG_INFO*    pTmpSmsLongInfo=NULL;   
    UINT32                ret=0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_mt_SetLongPara);

    //Get private date
    pSmsMTInfo = CFW_GetAoUserData(hAo);
    //Malloc CFW_SMS_LONG_INFO for pStru_SmsLongInfo
    pTmpSmsLongInfo = (CFW_SMS_LONG_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_LONG_INFO));
    if(!pTmpSmsLongInfo)
    {
	CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_SetLongPara pTmpSmsLongInfo malloc error!!! \n ");
	CSW_PROFILE_FUNCTION_EXIT(sms_mt_SetLongPara);
	return ERR_CMS_UNKNOWN_ERROR;
    }
    SUL_ZeroMemory8(pTmpSmsLongInfo, (SIZEOF(CFW_SMS_LONG_INFO)));
    pTmpSmsLongInfo->count   = pSmsMTInfo->nLongerMsgCount;
    pTmpSmsLongInfo->current = pSmsMTInfo->nLongerMsgCurrent;
    pTmpSmsLongInfo->id      = pSmsMTInfo->nLongerMsgID;
    pTmpSmsLongInfo->nStatus = CFW_SMS_STORED_STATUS_UNREAD;
    pTmpSmsLongInfo->index   = pSmsMTInfo->nIndex;
    pTmpSmsLongInfo->pNext   = NULL;
    //Conncet the struct to the link
    CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_mt_SetLongPara:  pSmsMTInfo->nIndex = %d\n"),pSmsMTInfo->nIndex);
    CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_mt_SetLongPara:  pSmsMTInfo->nLongerMsgCurrent = %d\n"),pSmsMTInfo->nLongerMsgCurrent);
    CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_mt_SetLongPara:  pSmsMTInfo->nLongerMsgCount = %d\n"),pSmsMTInfo->nLongerMsgCount);
   CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_mt_SetLongPara:  gpWriteLongerMsgPara = %x\n"),gpWriteLongerMsgPara);
    if(gpWriteLongerMsgPara)
    {
        CFW_SMS_LONG_INFO *pLink=NULL;
        pLink = gpWriteLongerMsgPara;
        while(pLink->pNext)
        {
            pLink = (CFW_SMS_LONG_INFO*)(pLink->pNext);
        }
        pLink->pNext = (UINT32*)pTmpSmsLongInfo;
    }
    else //if(1 == pSmsMTInfo->nLongerMsgCurrent)// for writing the first one
        gpWriteLongerMsgPara = pTmpSmsLongInfo;
    //Count++
    gnWriteLongerMsgParaCount++;
    //the last part of a long msg
    if(pSmsMTInfo->nLongerMsgCurrent == pSmsMTInfo->nLongerMsgCount)
    {
        CFW_SMS_PARAMETER  sInfo;
        UINT32             nWriteBuf=0x0;
        
        SUL_ZeroMemory8(&sInfo, SIZEOF(CFW_SMS_PARAMETER));
        ret = sms_tool_Parse_LongMSGPara(gpWriteLongerMsgPara,gnWriteLongerMsgParaCount,&nWriteBuf);
        if(ret != ERR_SUCCESS)
        {
            gnWriteLongerMsgParaCount = 0x0;
            gpWriteLongerMsgPara      =  NULL;
            CSW_PROFILE_FUNCTION_EXIT(sms_mt_SetLongPara);
            return ERR_CMS_UNKNOWN_ERROR;
        }
        ret = sms_tool_SetLongMSGPara(pSmsMTInfo->nSmsStorageID,(UINT8*)nWriteBuf,(INT32)(gnWriteLongerMsgParaCount*SIZEOF(CFW_SMS_LONG_INFO_NODE)),LONG_SMS_PARA_ADD);
        CSW_SMS_FREE((UINT32*)nWriteBuf);
        if(ret != ERR_SUCCESS)
        {
            gnWriteLongerMsgParaCount = 0x0;
            gpWriteLongerMsgPara      =  NULL;
            CSW_PROFILE_FUNCTION_EXIT(sms_mt_SetLongPara);
            return ERR_CMS_UNKNOWN_ERROR;
        }
        gnWriteLongerMsgParaCount = 0x0;
        gpWriteLongerMsgPara      =  NULL;
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_mt_SetLongPara);
    return ERR_SUCCESS;
}

UINT32 sms_mt_Store_SmsPPReceiveInd(HAO hAo, api_SmsPPReceiveInd_t* pInMsg,CFW_EV* pMeEvent)
{
	CFW_SMS_MT_INFO*     pSmsMTInfo     = NULL;  //MT private data
	UINT8                nSmsStorageId=0x0,nOption=0x0;
	UINT32               ret=0x0; 
#ifndef CFW_MULTI_SIM
	CSW_PROFILE_FUNCTION_ENTER(sms_mt_Store_SmsPPReceiveInd);
#else
	CFW_SIM_ID nSimId = CFW_SIM_COUNT;
	CSW_PROFILE_FUNCTION_ENTER(sms_mt_Store_SmsPPReceiveInd);
	CFW_GetSimID(hAo, &nSimId);
#endif
	//Get private date
	pSmsMTInfo = CFW_GetAoUserData(hAo);
	if(!pSmsMTInfo)
	{
		CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
		return ERR_NO_MORE_MEMORY;
	}
    if( 0x00!= pInMsg->DataLen)
      CSW_TC_MEMBLOCK(CFW_SMS_TS_ID,pInMsg->Data,pInMsg->DataLen,16);

	//Get SMS StorageID
#ifndef CFW_MULTI_SIM
	ret= CFW_CfgGetNewSmsOption(&nOption,&nSmsStorageId);
#else
	ret= CFW_CfgGetNewSmsOption(&nOption,&nSmsStorageId, nSimId);
#endif
	CSW_TRACE(CFW_SMS_TS_ID, TSTXT("CFW_CfgGetNewSmsOption, nOption = 0x%x\n"),nOption );
	CSW_TRACE(CFW_SMS_TS_ID, TSTXT("CFW_CfgGetNewSmsOption, nSmsStorageId = 0x%x\n"),nSmsStorageId );

	//Set option and StorageID
	if (ret == ERR_SUCCESS)
	{
		//default storage value 
		if(nSmsStorageId == 0)
		{
			pSmsMTInfo->nSmsStorageID = CFW_SMS_STORAGE_SM; 
			pSmsMTInfo->nSRStorageID  = 0; 
		}
		else
		{
			UINT8 nTmp=0x0;

			//Get short message storage
#ifdef CFW_MULTI_SIM
		      nTmp = (UINT8)((nSmsStorageId & CFW_SMS_STORAGE_ME) | (nSmsStorageId & CFW_SMS_STORAGE_SM));
#else
		      nTmp = (UINT8)((nSmsStorageId & CFW_SMS_STORAGE_ME) | (nSmsStorageId & CFW_SMS_STORAGE_SM) |
                     (nSmsStorageId & CFW_SMS_STORAGE_SM_ME) | (nSmsStorageId & CFW_SMS_STORAGE_MT));
#endif
			if(nTmp)
				pSmsMTInfo->nSmsStorageID = nTmp;
			else
				pSmsMTInfo->nSmsStorageID = CFW_SMS_STORAGE_SM;
#ifndef CFW_MULTI_SIM
			//Get Status Report storage, 0 means doesn't save Status Report
			nTmp = 0;
			nTmp = (UINT8)((nSmsStorageId & CFW_SMS_STORAGE_ME_SR) | (nSmsStorageId & CFW_SMS_STORAGE_SM_SR));
			pSmsMTInfo->nSRStorageID = nTmp;
#endif			
		}

		//default option value 
		if(nOption == 0)
			nOption = CFW_SMS_MULTIPART_OPTION_ALL; 

		//Get option 
		pSmsMTInfo->isBackupToSIM   = (UINT8)(nOption & CFW_SMS_MULTIPART_OPTION_SIM_BACKUP); 
		pSmsMTInfo->isListLongFirst = (UINT8)(nOption & CFW_SMS_MULTIPART_OPTION_FIRST); 
		pSmsMTInfo->isRoutDetail    = (UINT8)(nOption & CFW_SMS_ROUT_DETAIL_INFO); 

	}
	else
	{
		CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
		return ret;
	}
	CSW_TRACE(CFW_SMS_TS_ID, TSTXT("pSmsMTInfo->nSmsStorageID = 0x%x\n"),pSmsMTInfo->nSmsStorageID );

	//Pointer the pInMsg data to MT handle, and copy all the data to MT handle
	pSmsMTInfo->pInMsg = (api_SmsPPReceiveInd_t*)CSW_SMS_MALLOC(SIZEOF(api_SmsPPReceiveInd_t) + pInMsg->DataLen);
	if(!pSmsMTInfo->pInMsg)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Store_SmsPPReceiveInd pSmsMTInfo->pInMsg malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
		return ERR_NO_MORE_MEMORY;
	}
	SUL_ZeroMemory8(pSmsMTInfo->pInMsg, (UINT32)(SIZEOF(api_SmsPPReceiveInd_t) + pInMsg->DataLen));
	SUL_MemCopy8(pSmsMTInfo->pInMsg, pInMsg,(UINT32)(SIZEOF(api_SmsPPReceiveInd_t) + pInMsg->DataLen));

	//if DELIVER type
	//a SMS
	if (((pInMsg->Data[0]) & 0x03) == 0x00)
	{
		UINT8*   pPDU_Data = NULL;
		ret=0x0;
		CFW_SMS_STORAGE_INFO  sStorageInfo;

		SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
		pSmsMTInfo->nSmsType = 1;
		//Verify the short message is longer or not, check the free slot
		ret = sms_mt_Parse_LongMSGPara(hAo);

		//this is specially for class 2 message
		if(ERR_NO_NEED_SAVE == ret)
		{
			CSW_TRACE(CFW_SMS_TS_ID, "pInMsg->AddressLen %x pInMsg->DataLen %x\n",pInMsg->AddressLen,pInMsg->DataLen);
			CSW_TRACE(CFW_SMS_TS_ID, "_SMS_GetUDL(pInMsg) %x\n",_SMS_GetUDL(pInMsg));
			CSW_TC_MEMBLOCK(CFW_SMS_TS_ID,_SMS_GetUDAddr(pInMsg),_SMS_GetUDL(pInMsg),16);
#ifndef CFW_MULTI_SIM
			ret = CFW_SatResponse(0xD1, 0x00, 0x00, pInMsg, SIZEOF(api_SmsPPReceiveInd_t) + pInMsg->DataLen, SMS_MT_UTI_MIN);
#else
			ret = CFW_SatResponse(0xD1, 0x00, 0x00, pInMsg, SIZEOF(api_SmsPPReceiveInd_t) + pInMsg->DataLen, SMS_MT_UTI_MIN, nSimId);
#endif
			if(ERR_SUCCESS != ret)
			{
				return ERR_INVALID_PARAMETER;
			}
			// Switch state to be CFW_SM_STATE_WAIT 
			CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
			return ERR_NO_NEED_SAVE;

		}

    //When 0x40==nPID,we(short message type 0)discard its contents(3gpp 23040 9.2.3.9).
    UINT32 nPID = _SMS_GetPID(pInMsg);
    if(0x40 == nPID)
    {
      #ifndef CFW_MULTI_SIM
      				sms_mt_SmsPPAckReq();
      #else
      				sms_mt_SmsPPAckReq(nSimId);
      #endif
      CSW_SMS_FREE(pSmsMTInfo->pInMsg);
      CFW_UnRegisterAO(CFW_SMS_MT_SRV_ID,hAo);
			return ERR_NO_NEED_SAVE;
    }
    
		CSW_TRACE(CFW_SMS_TS_ID, TSTXT("pSmsMTInfo->nLongerMsgCount = 0x%x\n"),pSmsMTInfo->nLongerMsgCount );

		if(pSmsMTInfo->nLongerMsgCount > 1)
		{
#ifndef CFW_MULTI_SIM
			if((pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_SM_ME )||
				(pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_ME))
			{
				//Verify the free slot in ME firstly
				ret = CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_ME);
#else
			if(pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_ME)
			{
				//Verify the free slot in ME firstly
		 		ret = CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_ME, nSimId);
#endif
#ifndef CHECK_STORAGE_SMS_INFO_TMP
				if((pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1) > (sStorageInfo.totalSlot - sStorageInfo.usedSlot))
#else
        if( (sStorageInfo.totalSlot - sStorageInfo.usedSlot) <= 0)
#endif          
				{
					
					//if ME is not OK, then verify the free slot in SM 
					SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
					CFW_CfgGetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_SM);
#else
					CFW_CfgGetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_SM, nSimId);
#endif
#ifndef CHECK_STORAGE_SMS_INFO_TMP
          if((pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1) > (sStorageInfo.totalSlot - sStorageInfo.usedSlot))
#else
          if( (sStorageInfo.totalSlot - sStorageInfo.usedSlot) <= 0)
#endif
          {
						//added by fengwei 20080701 begin
						//we should update the information of full for new in order to notify when msg is deleted by user.
						UINT8 uNewCount, uNeedCount;
#ifndef CFW_MULTI_SIM
						CFW_CfgGetSmsFullForNew( (UINT8 *)&uNewCount );
#ifndef CHECK_STORAGE_SMS_INFO_TMP
						uNeedCount = (pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1) - (sStorageInfo.totalSlot - sStorageInfo.usedSlot);
#else
            uNeedCount = (pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1);
#endif
            uNewCount += uNeedCount;
						CFW_CfgSetSmsFullForNew(uNewCount);
						CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND, 2, (pSmsMTInfo->nSmsStorageID|(uNewCount<<8)),SMS_MT_UTI,0);
						//added by fengwei 20080701 end
						CSW_SMS_FREE(pSmsMTInfo->pInMsg);
						sms_mt_SmsPPErrorReq(0xd3); //0xd3: Memory capacity exceeded
#else
						CFW_CfgGetSmsFullForNew( (UINT8 *)&uNewCount, nSimId);
#ifndef CHECK_STORAGE_SMS_INFO_TMP
            uNeedCount = (pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1) - (sStorageInfo.totalSlot - sStorageInfo.usedSlot);
#else
            uNeedCount = (pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1);
#endif
            uNewCount += uNeedCount;
						CFW_CfgSetSmsFullForNew(uNewCount, nSimId);
						CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND, 2, (pSmsMTInfo->nSmsStorageID|(uNewCount<<8)),SMS_MT_UTI |(nSimId<<24),0);
						//added by fengwei 20080701 end
						CSW_SMS_FREE(pSmsMTInfo->pInMsg);
						sms_mt_SmsPPErrorReq(0xd3, nSimId); //0xd3: Memory capacity exceeded
#endif
						CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
						return ERR_CFW_INVALID_PARAMETER;
					}
					else
					{
						pSmsMTInfo->nSmsStorageID = CFW_SMS_STORAGE_SM;
#ifndef CFW_MULTI_SIM
						CFW_CfgSetNewSmsOption(nOption, CFW_SMS_STORAGE_SM);
#else
						CFW_CfgSetNewSmsOption(nOption, CFW_SMS_STORAGE_SM, nSimId);
#endif
					}
				}     
			}
			else if(pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_SM)
			{
#ifndef CFW_MULTI_SIM
				//Verify the free slot
				CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMTInfo->nSmsStorageID);
#ifndef CHECK_STORAGE_SMS_INFO_TMP
        if((pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1) > (sStorageInfo.totalSlot - sStorageInfo.usedSlot))
#else
				if( (sStorageInfo.totalSlot - sStorageInfo.usedSlot) <= 0)
#endif
        {
					//if SM is not OK, then verify the free slot in ME 
					SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
					CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_ME);
#ifndef CHECK_STORAGE_SMS_INFO_TMP
          if((pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1) > (sStorageInfo.totalSlot - sStorageInfo.usedSlot))
#else
          if( (sStorageInfo.totalSlot - sStorageInfo.usedSlot) <= 0)
#endif
          {
						UINT8 uNewCount, uNeedCount;
						CFW_CfgGetSmsFullForNew( (UINT8 *)&uNewCount );
						uNeedCount = (pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1) ;
						uNewCount += uNeedCount;
						CFW_CfgSetSmsFullForNew(uNewCount);
						CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND, 2, (pSmsMTInfo->nSmsStorageID|(uNewCount<<8)),SMS_MT_UTI,0);
						//Free pSmsMTInfo->pInMsg
						CSW_SMS_FREE(pSmsMTInfo->pInMsg);
						sms_mt_SmsPPErrorReq(0xd3); //0xd3: Memory capacity exceeded
						CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
						return ERR_CFW_INVALID_PARAMETER;
					}
					else
					{
						CFW_CfgSetNewSmsOption(nOption, CFW_SMS_STORAGE_ME);
						pSmsMTInfo->nSmsStorageID = CFW_SMS_STORAGE_ME;
					}
				}     
			}
			else// invalid storage ID
			{
				CSW_SMS_FREE(pSmsMTInfo->pInMsg);
				sms_mt_SmsPPErrorReq(0xd3); //0xd3: Memory capacity exceeded
				CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
				return ERR_CFW_INVALID_PARAMETER;
			}
#else
				//Verify the free slot
				CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMTInfo->nSmsStorageID, nSimId);

#ifndef CHECK_STORAGE_SMS_INFO_TMP
				if((pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1) > (sStorageInfo.totalSlot - sStorageInfo.usedSlot))
#else
        if( (sStorageInfo.totalSlot - sStorageInfo.usedSlot) <= 0)
#endif
        {
					//if SM is not OK, then verify the free slot in ME 
					SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
					CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_ME, nSimId);
#ifndef CHECK_STORAGE_SMS_INFO_TMP
          if((pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1) > (sStorageInfo.totalSlot - sStorageInfo.usedSlot))
#else
          if( (sStorageInfo.totalSlot - sStorageInfo.usedSlot) <= 0)
#endif
          {
						UINT8 uNewCount, uNeedCount;

						ret = CFW_CfgGetSmsFullForNew( (UINT8 *)&uNewCount, nSimId);
						if( ERR_SUCCESS != ret )
						{
							CSW_TRACE(CFW_SMS_TS_ID, "CFW_CfgGetSmsFullForNew error 0x%x\n",ret);
							CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND, ret, (pSmsMTInfo->nSmsStorageID),SMS_MT_UTI |(nSimId<<24),0xf0);
						}
						else
						{
#ifndef CHECK_STORAGE_SMS_INFO_TMP
              uNeedCount = (pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1) - (sStorageInfo.totalSlot - sStorageInfo.usedSlot);
#else
              uNeedCount = (pSmsMTInfo->nLongerMsgCount - pSmsMTInfo->nLongerMsgCurrent + 1);
#endif
              uNewCount += uNeedCount;
							
							ret = CFW_CfgSetSmsFullForNew(uNewCount, nSimId);
							if( ERR_SUCCESS != ret )
							{
								CSW_TRACE(CFW_SMS_TS_ID, "CFW_CfgSetSmsFullForNew error 0x%x\n",ret);
								CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND, ret, (pSmsMTInfo->nSmsStorageID),SMS_MT_UTI |(nSimId<<24),0xf0);
							}
							else
							{
								CSW_TRACE(CFW_SMS_TS_ID, "Send EV_CFW_SMS_INFO_IND success\n");
								CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND, 2, (pSmsMTInfo->nSmsStorageID|(uNewCount<<8)),SMS_MT_UTI |(nSimId<<24),0);
							}
						}
						
						//Free pSmsMTInfo->pInMsg
						CSW_SMS_FREE(pSmsMTInfo->pInMsg);
						ret = sms_mt_SmsPPErrorReq(0xd3, nSimId); //0xd3: Memory capacity exceeded
						if( ERR_SUCCESS != ret )
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_SmsPPErrorReq error 0x%x\n",ret);
						}
						CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
						return ret;
					}
					else
					{
						ret = CFW_CfgSetNewSmsOption(nOption, CFW_SMS_STORAGE_ME, nSimId);
						if( ERR_SUCCESS != ret )
						{
							CSW_TRACE(CFW_SMS_TS_ID, "CFW_CfgSetNewSmsOption error 0x%x\n",ret);
						}

						pSmsMTInfo->nSmsStorageID = CFW_SMS_STORAGE_ME;
					}
				}     
			}
			else// invalid storage ID
			{
				CSW_TRACE(CFW_SMS_TS_ID, "Error Storage ID 0x%x\n",pSmsMTInfo->nSmsStorageID);
			
				CSW_SMS_FREE(pSmsMTInfo->pInMsg);
				
				ret = sms_mt_SmsPPErrorReq(0xd3, nSimId); //0xd3: Memory capacity exceeded
				if( ERR_SUCCESS != ret )
				{
					CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_SmsPPErrorReq error 0x%x\n",ret);
				}
				
				CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
				return ret;
			}

#endif
		} // end of if(sLongerMsg.count > 0)

		pPDU_Data = (UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE*2);
		if(!pPDU_Data)
		{
			CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Store_SmsPPReceiveInd pSmsMTInfo->pInMsg malloc error!!! \n ");
			CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
			return ERR_NO_MORE_MEMORY;
		}
		SUL_ZeroMemory8(pPDU_Data, SMS_MO_ONE_PDU_SIZE*2);

		*pPDU_Data     = CFW_SMS_STORED_STATUS_UNREAD;
		*(pPDU_Data+1) = pInMsg->AddressLen;

		//Copy SC 
		SUL_MemCopy8((pPDU_Data + 2), pInMsg->Address,(UINT32)pInMsg->AddressLen);
		//Copy data
		SUL_MemCopy8((pPDU_Data + 2 + pInMsg->AddressLen), pInMsg->Data,(UINT32)pInMsg->DataLen);

		if((pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_ME) 
#ifndef CFW_MULTI_SIM
			|| (pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_SM_ME)
#endif			
			)
		{
			UINT32 nTime=0x0, nAppInt32=0x0;
			
#ifndef CFW_MULTI_SIM
				UINT8  nFlag_Storage_SM_ME=0x0;
			if(pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_SM_ME)
			{
				nFlag_Storage_SM_ME       = 1;
				pSmsMTInfo->nSmsStorageID = CFW_SMS_STORAGE_ME;
			}
			ret = CFW_MeWriteMessage(pSmsMTInfo->nSmsStorageID,0,pPDU_Data,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,pMeEvent);
			if( ERR_SUCCESS != ret )
			{
				CSW_TRACE(CFW_SMS_TS_ID, "CFW_MeWriteMessage error 0x%x\n",ret);
			}

			*pPDU_Data = CFW_SMS_STORED_STATUS_UNREAD;
			if(1 == nFlag_Storage_SM_ME)
			{
				if(0xf0 == pMeEvent->nType) 
				{
					pSmsMTInfo->nSmsStorageID = CFW_SMS_STORAGE_SM;
					ret = CFW_SimWriteMessage(pSmsMTInfo->nSmsStorageID,0,pPDU_Data,SMS_MO_ONE_PDU_SIZE,(SMS_MT_UTI_MIN));
				}
			}
			else
			{
				if(pSmsMTInfo->isBackupToSIM)
					ret = CFW_SimWriteMessage(CFW_SMS_STORAGE_SM,0,pPDU_Data,SMS_MO_ONE_PDU_SIZE,(SMS_MT_UTI_MIN));
			}
#else
			ret = CFW_MeWriteMessage(nSimId,0,pPDU_Data,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,pMeEvent);
			*pPDU_Data = CFW_SMS_STORED_STATUS_UNREAD;
			if(pSmsMTInfo->isBackupToSIM)
			{
				ret = CFW_SimWriteMessage(CFW_SMS_STORAGE_SM,0,pPDU_Data,SMS_MO_ONE_PDU_SIZE,(SMS_MT_UTI_MIN), nSimId);
				if( ERR_SUCCESS != ret )
				{
					CSW_TRACE(CFW_SMS_TS_ID, "CFW_MeWriteMessage error 0x%x\n",ret);
				}
			}
#endif			
		}
#ifndef CFW_MULTI_SIM
		else if(pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_SM)
		{
			ret = CFW_SimWriteMessage(CFW_SMS_STORAGE_SM,0,pPDU_Data,SMS_MO_ONE_PDU_SIZE,SMS_MT_UTI_MIN);
		}
		else if(pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_MT)
		{
			//Parse received SMS
			if(pSmsMTInfo->isRoutDetail)
				sms_mt_Parse_SmsPPReceiveInd(hAo);
			//Free pPDU_Data
			CSW_SMS_FREE(pPDU_Data);
			//Free pSmsMTInfo->pInMsg
			CSW_SMS_FREE(pSmsMTInfo->pInMsg);
			CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
			return ERR_CFW_INVALID_PARAMETER;
		}
		else
		{
			ret = sms_mt_SmsPPErrorReq(0xd3); //0xd3: Memory capacity exceeded
			//Free pPDU_Data
			CSW_SMS_FREE(pPDU_Data);
			//Free pSmsMTInfo->pInMsg
			CSW_SMS_FREE(pSmsMTInfo->pInMsg);
			CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
			return ERR_CFW_INVALID_PARAMETER;
		}
#else
		else if(pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_SM)
		{
			ret = CFW_SimWriteMessage(CFW_SMS_STORAGE_SM,0,pPDU_Data,SMS_MO_ONE_PDU_SIZE,SMS_MT_UTI_MIN, nSimId);
			if( ERR_SUCCESS != ret )
			{
				CSW_TRACE(CFW_SMS_TS_ID, "CFW_MeWriteMessage error 0x%x pSmsMTInfo->nSmsStorageID 0x%x\n",ret,pSmsMTInfo->nSmsStorageID);
			}
		}
		else
		{
			ret = sms_mt_SmsPPErrorReq(0xd3, nSimId); //0xd3: Memory capacity exceeded
			if( ERR_SUCCESS != ret )
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_SmsPPErrorReq error 0x%x pSmsMTInfo->nSmsStorageID 0x%x\n",ret,pSmsMTInfo->nSmsStorageID);
			}

			//Free pPDU_Data
			CSW_SMS_FREE(pPDU_Data);
			//Free pSmsMTInfo->pInMsg
			CSW_SMS_FREE(pSmsMTInfo->pInMsg);
			CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
			return ERR_CFW_INVALID_PARAMETER;
		}
#endif		
		//Free pPDU_Data
		CSW_SMS_FREE(pPDU_Data);
		if(ret == ERR_SUCCESS)
		{
			//Switch state to be CFW_SM_STATE_WAIT 
			CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
		}
		else
		{
			//added by fengwei 20080701 begin
			//we should update the information of full for new in order to notify when msg is deleted by user.
			UINT8 uNewCount;
#ifndef CFW_MULTI_SIM
			CFW_CfgGetSmsFullForNew( (UINT8 *)&uNewCount);
			uNewCount ++;
			CFW_CfgSetSmsFullForNew(uNewCount);
			CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND, 2, (pSmsMTInfo->nSmsStorageID|(uNewCount<<8)),SMS_MT_UTI,0);
			//added by fengwei 20080701 end
			sms_mt_SmsPPErrorReq(0xd3); //0xd3: Memory capacity exceeded
#else
			CFW_CfgGetSmsFullForNew( (UINT8 *)&uNewCount, nSimId);
			uNewCount ++;
			CFW_CfgSetSmsFullForNew(uNewCount, nSimId);
			CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND, 2, (pSmsMTInfo->nSmsStorageID|(uNewCount<<8)),SMS_MT_UTI |(nSimId<<24),0);
			//added by fengwei 20080701 end
			sms_mt_SmsPPErrorReq(0xd3, nSimId); //0xd3: Memory capacity exceeded
#endif
			//Free pSmsMTInfo->pInMsg
			CSW_SMS_FREE(pSmsMTInfo->pInMsg);
			CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
			return ERR_CFW_INVALID_PARAMETER;
		}
	} 
	else if(((pInMsg->Data[0]) & 0x03) == 0x02) //Status Report
	{
		ret=0x0;

		pSmsMTInfo->nSmsType = 0;
#ifndef CFW_MULTI_SIM
		if(pSmsMTInfo->nSRStorageID == CFW_SMS_STORAGE_ME_SR)
		{
			//??? later
			//ret = CFW_MeWriteMessage(CFW_SMS_STORAGE_ME_SR,0,(UINT8*)pInMsg,SMS_MO_ONE_PDU_SIZE,pMeEvent);
		}
		else if(pSmsMTInfo->nSRStorageID == CFW_SMS_STORAGE_SM_SR)
		{
			//???Later
			//ret = CFW_SimWriteMessage(CFW_SMS_STORAGE_SM_SR,0,(UINT8*)pInMsg,SMS_MO_ONE_PDU_SIZE,SMS_MT_UTI_MIN);
		}
		else
		{
#endif		
			if(pSmsMTInfo->isRoutDetail)
				sms_mt_Parse_SmsPPReceiveInd_SR(hAo);
			//Free pSmsMTInfo->pInMsg
			CSW_SMS_FREE(pSmsMTInfo->pInMsg);
			CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
			return ERR_CFW_INVALID_PARAMETER;
#ifndef CFW_MULTI_SIM
		}
#endif	

		if(ret == ERR_SUCCESS)
		{
			//Switch state to be CFW_SM_STATE_WAIT 
			CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
		}
		else
		{
			//Free pSmsMTInfo->pInMsg
			CSW_SMS_FREE(pSmsMTInfo->pInMsg);
			CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
			return ERR_CFW_INVALID_PARAMETER;
		}
	}
	else 
	{
		CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Store_SmsPPReceiveInd,pInMsg->Data[0] = 0x%x.\n",pInMsg->Data[0]);
#ifndef CFW_MULTI_SIM
		sms_mt_SmsPPErrorReq(0xd3); //0xd3: Memory capacity exceeded
#else
		sms_mt_SmsPPErrorReq(0xd3, nSimId); 
#endif
		CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);
		return ERR_CFW_INVALID_PARAMETER;				
	}

	CSW_PROFILE_FUNCTION_EXIT(sms_mt_Store_SmsPPReceiveInd);  
	return ERR_SUCCESS;
}




///////////////////////////////////////////////////////////////////////////////
//                         Local tool API                                    //
///////////////////////////////////////////////////////////////////////////////

//

UINT32 SUL_UserDialNum2StackDialNum(
					CFW_DIALNUMBER *pNumber,
					UINT8 * pSDN,
					UINT8 * pSDNLen
					)
{
	//
	//Max length of the DialNum is 28.
	//
	UINT8 DialNumber[TEL_NUMBER_MAX_LEN],nLength;
	CSW_PROFILE_FUNCTION_ENTER(SUL_UserDialNum2StackDialNum);
	if((pNumber == ((CFW_DIALNUMBER*) NULL))||(pSDN == ((UINT8*) NULL))||(pNumber->pDialNumber == ((UINT8*) NULL))){
		CSW_PROFILE_FUNCTION_EXIT(SUL_UserDialNum2StackDialNum);
	return ERR_CMS_UNKNOWN_ERROR;
	}

/*	if((pNumber->nDialNumberSize < 0) || 
		(pNumber->nDialNumberSize > TEL_NUMBER_MAX_LEN) || 
		(* pSDNLen < 0) || (* pSDNLen > (TEL_NUMBER_MAX_LEN + 2))){
*/		
	if((pNumber->nDialNumberSize > TEL_NUMBER_MAX_LEN) || ((*pSDNLen) > (TEL_NUMBER_MAX_LEN + 2)))
		{
			CSW_PROFILE_FUNCTION_EXIT(SUL_UserDialNum2StackDialNum);
    		return ERR_CMS_UNKNOWN_ERROR;
		}
	
#ifdef SMS_ARG_CHK_EN
	if((pNumber->nType != 0x81) && (pNumber->nType != 0x91) && (pNumber->nType != 0xA1)){
		CSW_PROFILE_FUNCTION_EXIT(SUL_UserDialNum2StackDialNum);
	return ERR_CMS_UNKNOWN_ERROR;
	}
#endif
	SUL_ZeroMemory32(DialNumber,TEL_NUMBER_MAX_LEN);
	nLength = pNumber->nDialNumberSize; 	
	(nLength & 0x01)  ? (nLength = nLength / 2 + 1) :(nLength = nLength / 2) ;
	
	if(pNumber->pDialNumber[0] == '+')
	{
		DialNumber[0] = 0x91;
		SUL_AsciiToGsmBcd(&(pNumber->pDialNumber[1]),(pNumber->nDialNumberSize - 1),&(DialNumber[1]));
		*pSDNLen = nLength + 1;
	}
	else
	{
#ifdef SMS_ARG_CHK_EN
		DialNumber[0] = pNumber->nType;
		SUL_AsciiToGsmBcd(pNumber->pDialNumber,pNumber->nDialNumberSize,&(DialNumber[1]));
		*pSDNLen = nLength + 1;		
#else
		(pNumber->nType == 0x91)?(DialNumber[0] = 0x91):(DialNumber[0] = 0x81);
		SUL_AsciiToGsmBcd(pNumber->pDialNumber,pNumber->nDialNumberSize,&(DialNumber[1]));
		*pSDNLen = nLength + 1;	
#endif
	}
	SUL_MemCopy8(pSDN,DialNumber,*pSDNLen);

	CSW_PROFILE_FUNCTION_EXIT(SUL_UserDialNum2StackDialNum);
	return ERR_SUCCESS;
}



#ifndef CFW_MULTI_SIM
UINT32 sms_tool_GetLongMSGPara(UINT8 nStorage,UINT16 nIndex,UINT16* pConcatSmsPrevRec, UINT16* pConcatSmsNextRec)
{
#if   USING_FS_SAVE_LONG_PARA
    PSTR   pDir=NULL;
    PSTR   pName=NULL;
    UINT32 hFile=0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_tool_GetLongMSGPara);

    pDir = pSMSDir;

    if(nStorage & CFW_SMS_STORAGE_SM)
    {
        pName =  pSIMFilename;
    }
    else if(nStorage & CFW_SMS_STORAGE_ME)
    {
        pName =  pMEFilename;
    }
    else
    {
    	CSW_PROFILE_FUNCTION_EXIT(sms_tool_GetLongMSGPara);
        return ERR_CMS_UNKNOWN_ERROR;
    }

    hFile = FS_Open(pName, FS_O_RDONLY, 0x0);
    if( hFile != HNULL )
    {
        UINT32  nStruCount=0x0;
        UINT32* pStruCount=NULL;
        INT32   nLen=0x0;

        pStruCount = &nStruCount;
        nLen       = 4;
        //Read the count of existed structure CFW_SMS_LONG_INFO_NODE
        if(nLen == FS_Read(hFile, (PSTR)pStruCount, nLen )) 
        {
            UINT32 nStruSize=0x0;
            CFW_SMS_LONG_INFO_NODE * pTmpReadBuf=NULL;
            CFW_SMS_LONG_INFO_NODE * pTmpReadBuf_Addr=NULL;

            nStruSize = 4 + nStruCount*SIZEOF(CFW_SMS_LONG_INFO_NODE);
            //malloc a temp memory for saving them to sim_long_sms_info.txt
            pTmpReadBuf = (CFW_SMS_LONG_INFO_NODE*)CSW_SMS_MALLOC(nStruSize);
            if(pTmpReadBuf == NULL)
	     {
   		  CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_GetLongMSGPara pTmpReadBuf malloc error!!! \n ");
   		  CSW_PROFILE_FUNCTION_EXIT(sms_tool_GetLongMSGPara);
   		  return ERR_CMS_UNKNOWN_ERROR;
            }
            SUL_ZeroMemory8(pTmpReadBuf, nStruSize);
            pTmpReadBuf_Addr = pTmpReadBuf;

            //Read the file again,get all the data
            nLen = nStruCount*SIZEOF(CFW_SMS_LONG_INFO_NODE);
            if(nLen == FS_Read( hFile, (PSTR)pTmpReadBuf, nLen )) 
            {
                UINT8 i=0x0,nFlag=0x0;

                for(i=0;i<nStruCount;i++,pTmpReadBuf++)
                {
                    if(nIndex == pTmpReadBuf->nConcatCurrentIndex)
                    {
                        *pConcatSmsPrevRec = pTmpReadBuf->nConcatPrevIndex;
                        *pConcatSmsNextRec = pTmpReadBuf->nConcatNextIndex;
                        nFlag = 1;
                        break;
                    }
                }
                CSW_SMS_FREE(pTmpReadBuf_Addr);
                FS_Close(hFile);
                if(nFlag){
                	CSW_PROFILE_FUNCTION_EXIT(sms_tool_GetLongMSGPara);
                  return ERR_SUCCESS;
                  }
                else{
                	CSW_PROFILE_FUNCTION_EXIT(sms_tool_GetLongMSGPara);
                  return ERR_CMS_UNKNOWN_ERROR;
                  }
            }
            else
            {
                CSW_SMS_FREE(pTmpReadBuf_Addr);
                FS_Close(hFile);
                CSW_PROFILE_FUNCTION_EXIT(sms_tool_GetLongMSGPara);
                return ERR_CMS_UNKNOWN_ERROR;
            }
        }
        else
        {
            FS_Close(hFile);
            CSW_PROFILE_FUNCTION_EXIT(sms_tool_GetLongMSGPara);
            return ERR_CMS_UNKNOWN_ERROR;
        }
    }
    else{
    	CSW_PROFILE_FUNCTION_EXIT(sms_tool_GetLongMSGPara);
      return ERR_CMS_UNKNOWN_ERROR;
      }
      CSW_PROFILE_FUNCTION_EXIT(sms_tool_GetLongMSGPara);
    return ERR_SUCCESS;

#else
    *pConcatSmsPrevRec = 0;
    *pConcatSmsNextRec = 0;
    return ERR_SUCCESS;

#endif
}

#endif

//nOption = LONG_SMS_PARA_ADD    : add some para to a new file or an existed file
//nOption = LONG_SMS_PARA_CREATE : init a new file
//nOption = LONG_SMS_PARA_DELETE : Delete some para from an existed file when deleting a longer sms
// pWriteBuf Structure
//  __________________________________________________________________________
// |        |                         |                          |
// |4 Bytes | CFW_SMS_LONG_INFO_NODE  |  CFW_SMS_LONG_INFO_NODE  |  .....
// |________|_________________________|__________________________|___________
//
UINT32 sms_tool_SetLongMSGPara(UINT8 nStorage,UINT8* pWriteBuf,INT32 nWriteBufSize,UINT8 nOption)
{
    PSTR   pDir=NULL;
    PSTR   pName=NULL;
    UINT32 hFile=0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_tool_SetLongMSGPara);

    pDir = pSMSDir;

    if(nStorage & CFW_SMS_STORAGE_SM)
    {
        pName =  pSIMFilename;
    }
    else if(nStorage & CFW_SMS_STORAGE_ME)
    {
        pName =  pMEFilename;
    }
    else
    {
    	CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
        return ERR_CMS_UNKNOWN_ERROR;
    }

    FS_MakeDir(pDir, 0x0);
    if(nOption == LONG_SMS_PARA_CREATE)// Create a new file
    {
        hFile = FS_Open(pName, FS_O_WRONLY | FS_O_CREAT |FS_O_TRUNC, 0x0);
        if( hFile != HNULL )
        {
            CFW_SMS_LONG_INFO_NODE * pTmpWriteBuf=NULL;
            
            //malloc a temp memory for saving them to sim_long_sms_info.txt
            pTmpWriteBuf = (CFW_SMS_LONG_INFO_NODE*)CSW_SMS_MALLOC(nWriteBufSize + 4);
            if(pTmpWriteBuf == NULL)
	     {
			CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_SetLongMSGPara pTmpWriteBuf malloc error!!! \n ");
			CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
			return ERR_CMS_UNKNOWN_ERROR;
            }
            SUL_ZeroMemory8(pTmpWriteBuf, (nWriteBufSize + 4));
            //Set the count of long msg
            *((INT32*)pTmpWriteBuf) = (INT32)(nWriteBufSize/SIZEOF(CFW_SMS_LONG_INFO_NODE));
            //Copy the data
            SUL_MemCopy8(((UINT8*)pTmpWriteBuf +4),pWriteBuf,nWriteBufSize);
            if(FS_Write(hFile,  (PSTR)pTmpWriteBuf,(nWriteBufSize+4)) != (nWriteBufSize+4))
            {
                CSW_SMS_FREE(pTmpWriteBuf);
                CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
                return ERR_CMS_UNKNOWN_ERROR;
            }
            CSW_SMS_FREE(pTmpWriteBuf);
            if (FALSE == FS_Close(hFile)){
            	CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
              return ERR_CMS_UNKNOWN_ERROR;
              }
        }
        else
        {
            CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
            return ERR_CMS_UNKNOWN_ERROR;
        }
    }
    else if(nOption == LONG_SMS_PARA_ADD)//add some para to a new file or an existed file
    {
        hFile = FS_Open(pName, FS_O_RDWR| FS_O_CREAT, 0x0);
        if( hFile != HNULL )
        {
            UINT32  nStruCount=0x0;
            UINT32* pStruCount=NULL;
            INT32   nLen=0x0;
            CFW_SMS_LONG_INFO_NODE * pTmpBuf=NULL;

            pStruCount = &nStruCount;
            nLen       = 4;
            //if is a new file, write the 4 bytes content for count            
            if(0 == FS_Seek(hFile, 0, FS_SEEK_END)) 
            {
                if(FS_Write(hFile, (PSTR)pStruCount,4) != 4)
                {
                    FS_Close(hFile);
                    CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
                    return ERR_CMS_UNKNOWN_ERROR;
                }
            }
            //Back to the start of file
            FS_Seek(hFile, 0, FS_SEEK_SET);
            //Read the count of existed structure CFW_SMS_LONG_INFO_NODE
            if(nLen == FS_Read( hFile, (PSTR)pStruCount, nLen )) 
            {
                UINT32 nStruSize=0x0;

                nStruSize = 4 + nStruCount*SIZEOF(CFW_SMS_LONG_INFO_NODE) + nWriteBufSize;
                //malloc a temp memory for saving them to sim_long_sms_info.txt
                pTmpBuf = (CFW_SMS_LONG_INFO_NODE*)CSW_SMS_MALLOC(nStruSize);
                if(pTmpBuf == NULL)
		  {
			CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_SetLongMSGPara pTmpWriteBuf malloc error!!! \n ");
                	CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
                     return ERR_CMS_UNKNOWN_ERROR;
                }
                SUL_ZeroMemory8(pTmpBuf, nStruSize);
                //Read the file again,get the content
                nLen = nStruCount*SIZEOF(CFW_SMS_LONG_INFO_NODE);
                if(nLen == FS_Read( hFile, ((UINT8*)pTmpBuf+4), nLen )) 
                {
                    //Copy the data
                    SUL_MemCopy8(((UINT8*)pTmpBuf + 4 + nLen),pWriteBuf,nWriteBufSize);
                    //Set the count of long msg
                    *((INT32*)pTmpBuf) = nStruCount + nWriteBufSize/SIZEOF(CFW_SMS_LONG_INFO_NODE);
                    //Back to the start of file
                    FS_Seek(hFile, 0, FS_SEEK_SET);
                    //Write the data
                    if(FS_Write(hFile, (PSTR)pTmpBuf,nStruSize) != (INT32)nStruSize)
                    {
                        CSW_SMS_FREE(pTmpBuf);
                        FS_Close(hFile);
                        CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                    else
                    {
                        CSW_SMS_FREE(pTmpBuf);
                        FS_Close(hFile);
                    }
                }
                else
                {
                    CSW_SMS_FREE(pTmpBuf);
                    FS_Close(hFile);
                    CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
                    return ERR_CMS_UNKNOWN_ERROR;
                }
            }
            else
            {
                CSW_SMS_FREE(pTmpBuf);
                FS_Close(hFile);
                CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
                return ERR_CMS_UNKNOWN_ERROR;
            }
        }
        else
        {
        	CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
            return ERR_CMS_UNKNOWN_ERROR;
        }
    }
    else if(nOption == LONG_SMS_PARA_DELETE)
    {
        CFW_SMS_LONG_INFO_NODE* pGetBuf=NULL;
        if(pWriteBuf)
        {
            INT32   nLen=0x0;
            UINT32  nStruCount=0x0;
            UINT32* pStruCount=NULL;
            CFW_SMS_LONG_INFO_NODE * pReadWriteBuf=NULL;
            
            pGetBuf = (CFW_SMS_LONG_INFO_NODE*)pWriteBuf;
            hFile = FS_Open(pName, FS_O_RDWR, 0x0);
            if( hFile == HNULL ){
            	CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
              return ERR_CMS_UNKNOWN_ERROR;
              }

            pStruCount = &nStruCount;
            nLen       = 4;
            //Read the count of existed structure CFW_SMS_LONG_INFO_NODE
            if(nLen == FS_Read( hFile, (PSTR)pStruCount, nLen )) 
            {
                UINT32 nStruSize=0x0;

                nStruSize = 4 + nStruCount*SIZEOF(CFW_SMS_LONG_INFO_NODE);
                //malloc a temp memory 
                pReadWriteBuf = (CFW_SMS_LONG_INFO_NODE*)CSW_SMS_MALLOC(nStruSize);
                if(pReadWriteBuf == NULL)
		  {
			CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_SetLongMSGPara pTmpWriteBuf malloc error!!! \n ");
                	CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
                     return ERR_CMS_UNKNOWN_ERROR;
                }
                SUL_ZeroMemory8(pReadWriteBuf, nStruSize);
                //Read the file again,get the content
                nLen = nStruCount*SIZEOF(CFW_SMS_LONG_INFO_NODE);
                if(nLen == FS_Read(hFile, ((UINT8*)pReadWriteBuf+4), nLen )) 
                {
                    UINT16 i=0x0,nDeleteCount=0x0;
                    INT32  nWriteSize=0x0;
                    CFW_SMS_LONG_INFO_NODE * pStrBuf=NULL;
                    pStrBuf = (CFW_SMS_LONG_INFO_NODE*)((UINT8*)pReadWriteBuf+4);
                    for(i=0;i<nStruCount;i++,pStrBuf++)
                    {
                        //Delete by Status
                        if(pGetBuf->nConcatCurrentIndex == 0)
                        {
                            //Verify there is the same Status in this buf or not
                            while(pGetBuf->nStatus & pStrBuf->nStatus)
                            {
                                UINT32 nCpySize=0x0;
                                nCpySize = (nStruCount - i -1 - nDeleteCount)*SIZEOF(CFW_SMS_LONG_INFO_NODE);
                                SUL_MemCopy8(pStrBuf,(pStrBuf+1),nCpySize);
                                SUL_ZeroMemory8(((UINT8*)pStrBuf + nCpySize),SIZEOF(CFW_SMS_LONG_INFO_NODE));
                                nDeleteCount++;
                                if(!(pGetBuf->nStatus & pStrBuf->nStatus))
                                    break;
                            }
                        }
                        else//Delete by index
                        {
                            //Verify there is the same Index in this buf or not
                            if(pGetBuf->nConcatCurrentIndex == pStrBuf->nConcatCurrentIndex)
                            {
                                UINT32 nCpySize=0x0;
                                nCpySize = (nStruCount - i -1)*SIZEOF(CFW_SMS_LONG_INFO_NODE);
                                SUL_MemCopy8(pStrBuf,(pStrBuf+1),nCpySize);
                                nDeleteCount++;
                            }
                        }
                    }
                    if(nDeleteCount == 0)
                    {
                        CSW_SMS_FREE(pReadWriteBuf);
                        FS_Close(hFile);
                        CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                    
                    //Back to the start of file
                    FS_Seek(hFile, 0, FS_SEEK_SET);
                    //Write new data to file
                    *((UINT32*)pReadWriteBuf) = (UINT32)(nStruCount-nDeleteCount);
                    nWriteSize = (INT32)(4 + (nStruCount-nDeleteCount)*SIZEOF(CFW_SMS_LONG_INFO_NODE));
                    if(FS_Write(hFile, (PSTR)pReadWriteBuf,nWriteSize) != nWriteSize)
                    {
                        CSW_SMS_FREE(pReadWriteBuf);
                        FS_Close(hFile);
                        CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                    else
                    {
                        CSW_SMS_FREE(pReadWriteBuf);
                        FS_Close(hFile);
                    }
                    
                }
            }
        }
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_tool_SetLongMSGPara);
    return ERR_SUCCESS;
}



UINT32 sms_tool_Parse_LongMSGPara(CFW_SMS_LONG_INFO* pLongInfo,UINT16 nLongStruCount,UINT32* pWriteBuf)
{
    UINT32                   nMallocSize=0x0;
    CFW_SMS_LONG_INFO_NODE*  pSMSLongInfoNode=NULL;
    CFW_SMS_LONG_INFO*       pTmp1_SmsLongInfo=NULL;
    CFW_SMS_LONG_INFO*       pTmp2_SmsLongInfo=NULL;
    UINT32*                  pTmp_FreeArrary=NULL;
    UINT32*                  pTmp1=NULL;
    UINT32*                  pTmp2=NULL;
    UINT16                   i=0x0,j=0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_tool_Parse_LongMSGPara);

    if(!pLongInfo){
    	CSW_PROFILE_FUNCTION_EXIT(sms_tool_Parse_LongMSGPara);
      return ERR_CMS_UNKNOWN_ERROR;
      }
    nMallocSize = nLongStruCount * SIZEOF(CFW_SMS_LONG_INFO_NODE);
    //malloc a temp memory for saving them to sim_long_sms_info.txt
    pSMSLongInfoNode = (CFW_SMS_LONG_INFO_NODE*)CSW_SMS_MALLOC(nMallocSize);
    if(pSMSLongInfoNode == NULL)
    {
	CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_Parse_LongMSGPara pSMSLongInfoNode malloc error!!! \n ");
	CSW_PROFILE_FUNCTION_EXIT(sms_tool_Parse_LongMSGPara);
	return ERR_CMS_UNKNOWN_ERROR;
    }
    SUL_ZeroMemory8(pSMSLongInfoNode, nMallocSize);
    *pWriteBuf = (UINT32)pSMSLongInfoNode;

    //Malloc for free next point
    pTmp_FreeArrary = (UINT32*)CSW_SMS_MALLOC(SIZEOF(UINT32)*nLongStruCount);
    if(pTmp_FreeArrary == NULL)
    {
	CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_Parse_LongMSGPara pTmp_FreeArrary malloc error!!! \n ");
    	CSW_PROFILE_FUNCTION_EXIT(sms_tool_Parse_LongMSGPara);
       return ERR_CMS_UNKNOWN_ERROR;
    }
    SUL_ZeroMemory32(pTmp_FreeArrary, nLongStruCount);
    pTmp1 = pTmp_FreeArrary;
    pTmp2 = pTmp_FreeArrary;
    pTmp1_SmsLongInfo = pLongInfo;
    for(i=0;i<nLongStruCount;i++,pSMSLongInfoNode++,pTmp_FreeArrary++)
    {
        pSMSLongInfoNode->nStatus             = pTmp1_SmsLongInfo->nStatus;
        pSMSLongInfoNode->nConcatCurrentIndex = pTmp1_SmsLongInfo->index;
        //Get the next addr
        *pTmp_FreeArrary = (UINT32)(pTmp1_SmsLongInfo);
        //Get nConcatPrevIndex and nConcatNextIndex
        pTmp2_SmsLongInfo = pLongInfo;
        for(j=0;j<nLongStruCount;j++)
        {
            if((pTmp2_SmsLongInfo->id == pTmp1_SmsLongInfo->id) && (pTmp2_SmsLongInfo->count == pTmp1_SmsLongInfo->count))
            {
                if(1 == pTmp1_SmsLongInfo->current)
                {
                    pSMSLongInfoNode->nConcatPrevIndex = pTmp1_SmsLongInfo->index;
                }
                if(pTmp2_SmsLongInfo->count == pTmp1_SmsLongInfo->current)
                {
                    pSMSLongInfoNode->nConcatNextIndex = pTmp1_SmsLongInfo->index;
                }
                if(1 == (pTmp2_SmsLongInfo->current - pTmp1_SmsLongInfo->current))
                {
                    pSMSLongInfoNode->nConcatNextIndex = pTmp2_SmsLongInfo->index;
                }
                if(1 == (pTmp1_SmsLongInfo->current - pTmp2_SmsLongInfo->current))
                {
                    pSMSLongInfoNode->nConcatPrevIndex = pTmp2_SmsLongInfo->index;
                }
            }
            //Point to next
            pTmp2_SmsLongInfo = (CFW_SMS_LONG_INFO*)(pTmp2_SmsLongInfo->pNext);
        }
        //Point to next
        pTmp1_SmsLongInfo = (CFW_SMS_LONG_INFO*)(pTmp1_SmsLongInfo->pNext);
    }
    //Free NEXT point got from CFW_SIMSmsInit()
    for(i=0;i<nLongStruCount;i++,pTmp2++)
    {
        CSW_SMS_FREE((UINT32*)(*pTmp2));
    }
    //Free pTmp_FreeArrary
    CSW_SMS_FREE(pTmp1);
    CSW_PROFILE_FUNCTION_EXIT(sms_tool_Parse_LongMSGPara);
    return ERR_SUCCESS;
}


UINT32 sms_mo_Parse_PDU(UINT8 nFormat,UINT8 nShow,UINT8 nStatus,UINT16 nStorage,PVOID pNode,UINT8* pType,UINT32* pDecomposeOutData,CFW_SMS_MULTIPART_INFO* pLongerMsg)
{
	UINT8   nTmp=0x0;
	UINT32  sDecomposeOutData=0x0,ret=0x0;
	CSW_PROFILE_FUNCTION_ENTER(sms_mo_Parse_PDU);

	if(nFormat == 1) //CFW_CfgSetSmsFormat(1),Text
	{
		if(nShow == 0) //CFW_CfgSetSmsShowTextModeParam(0)
		{
			//Delivered message. 
			if((nStatus == CFW_SMS_STORED_STATUS_UNREAD) || \
				(nStatus == CFW_SMS_STORED_STATUS_READ) )
			{
				//Text Mode, Delivered message. when  CFW_CfgSetSmsShowTextModeParam(0)
				//Parse pEvent->nParam1 to CFW_SMS_TXT_DELIVERED_NO_HRD_INFO 
				*pType = 0;
				ret = sms_tool_DecomposePDU(pNode,1,&sDecomposeOutData,&nTmp,pLongerMsg);
			}//Submitted  message
			else if((nStatus == CFW_SMS_STORED_STATUS_UNSENT)               || \
				(nStatus == CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)      || \
				(nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || \
				(nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || \
				(nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE) )
			{
				//Text Mode, Submitted  message. when  CFW_CfgSetSmsShowTextModeParam(0)
				//Parse pEvent->nParam1 to CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO  
				*pType  = 2;
				ret = sms_tool_DecomposePDU(pNode,3,&sDecomposeOutData,&nTmp,pLongerMsg);
			}
		}
		else if(nShow == 1)  //CFW_CfgSetSmsShowTextModeParam(1),Show all header information.
		{
			//Delivered message
			if((nStatus == CFW_SMS_STORED_STATUS_UNREAD) || \
				(nStatus == CFW_SMS_STORED_STATUS_READ) )
			{
				//Text Mode, Delivered message. when  CFW_CfgSetSmsShowTextModeParam(1)
				//Parse pEvent->nParam1 to CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO  
				*pType = 1;
				ret = sms_tool_DecomposePDU(pNode,2,&sDecomposeOutData,&nTmp,pLongerMsg);
			} //Submitted message.
			else if((nStatus == CFW_SMS_STORED_STATUS_UNSENT)               || \
				(nStatus == CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)      || \
				(nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || \
				(nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || \
				(nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE) )
			{
				//Text Mode, Submitted message. when  CFW_CfgSetSmsShowTextModeParam(1)
				//Parse pEvent->nParam1 to CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO  
				*pType = 3;
				ret = sms_tool_DecomposePDU(pNode,4,&sDecomposeOutData,&nTmp,pLongerMsg);
			}
		}
#ifndef CFW_MULTI_SIM
		//Text Mode, Report message.
		if((nStorage == CFW_SMS_STORAGE_SM_SR)  ||  \
			(nStorage == CFW_SMS_STORAGE_ME_SR))
		{
			*pType = 4;
			ret = sms_tool_DecomposePDU(pNode,5,&sDecomposeOutData,&nTmp,pLongerMsg);
		}
		//???Text Mode, Command message.
		//        if(0)
		//        {
		//            //Parse pEvent->nParam1 to xxx   
		//            *pType = 5;
		//        }
		//Text Mode, CBM Storage.
		if(nStorage == CFW_SMS_STORAGE_BM)
		{
			//Parse pEvent->nParam1 to xxx   
			*pType = 6;
		}
#endif		

	}
	else if(nFormat == 0)//CFW_CfgSetSmsFormat(0).PDU
	{
		if((nStorage == CFW_SMS_STORAGE_ME)  || \
			(nStorage == CFW_SMS_STORAGE_SM))
		{
			UINT32  sDecomposeOutData_temp=0x0;
			UINT8 n_stateTmp=0;
			ret = sms_tool_DecomposePDU(pNode,8,&sDecomposeOutData,&nTmp,pLongerMsg);
			//[[hameina[+] for bug 3811, 2007-1-31
			if(ret == ERR_SUCCESS)
			{
				if(nStatus == CFW_SMS_STORED_STATUS_UNREAD ||
					nStatus == CFW_SMS_STORED_STATUS_READ)
					ret = sms_tool_DecomposePDU(pNode,2,&sDecomposeOutData_temp,&n_stateTmp,pLongerMsg);
				else if((nStatus == CFW_SMS_STORED_STATUS_UNSENT)               || \
					(nStatus == CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)      || \
					(nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || \
					(nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || \
					(nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE) )
				{
					//Text Mode, Submitted message. when  CFW_CfgSetSmsShowTextModeParam(1)
					//Parse pEvent->nParam1 to CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO  
					ret = sms_tool_DecomposePDU(pNode,4,&sDecomposeOutData_temp,&n_stateTmp,pLongerMsg);
				}
				if(sDecomposeOutData_temp)
					CSW_SMS_FREE((void*)sDecomposeOutData_temp);
			}
			//]]hameina[+],2007-1-31
			*pType = 7;
		}
#ifndef CFW_MULTI_SIM
		else if(nStorage == CFW_SMS_STORAGE_BM)
		{
			*pType = 8;
		}
		else if ((nStorage == CFW_SMS_STORAGE_ME_SR) || \
			(nStorage == CFW_SMS_STORAGE_SM_SR))
		{
			*pType = 9;
		}
#endif		
	}

	*pDecomposeOutData = sDecomposeOutData;

	CSW_PROFILE_FUNCTION_EXIT(sms_mo_Parse_PDU);
	return ret;
}




UINT32 sms_mo_Parse_SmsPPErrorInd(HAO hAo,api_SmsPPErrorInd_t* pErrorInd)
{
	UINT32           nGetUti=0x0;   
	CFW_SMS_MO_INFO* pSmsMOInfo = NULL; //Point to MO private data.
	CSW_PROFILE_FUNCTION_ENTER(sms_mo_Parse_SmsPPErrorInd);

	//Get private date
	pSmsMOInfo = CFW_GetAoUserData(hAo);
	//Verify memory
	if(!pSmsMOInfo)
	{
		CSW_PROFILE_FUNCTION_EXIT(sms_mo_Parse_SmsPPErrorInd);
		return ERR_NO_MORE_MEMORY;
	}

	CFW_GetUTI(hAo, &nGetUti);
	pErrorInd = pErrorInd;
	CSW_TRACE(CFW_SMS_TS_ID, "The pErrorInd->ErrorType = %u\n ",pErrorInd->ErrorType);
	CSW_TRACE(CFW_SMS_TS_ID, "The pErrorInd->DataLen = %u\n ",pErrorInd->DataLen);
	//---------------------------------------------------------------------------
	//if the returned error cause is MM_FAILURE_CAUSE, and the cause[1] is set to 
	//SMS_MM_COLLISION/SMS_MM_LOWLAYER_FAILURE, CSW need to resend this SMS.
	//---------------------------------------------------------------------------
	if(pErrorInd->CauseLen == 2 && (pErrorInd->Cause[0] == MM_FAILURE_CAUSE))
	{
		CSW_TRACE(CFW_SMS_TS_ID, "The pErrorInd->Cause[0] = %u\n ",pErrorInd->Cause[0]);
		CSW_TRACE(CFW_SMS_TS_ID, "The pErrorInd->Cause[1] = %u\n ",pErrorInd->Cause[1]);
		if((pErrorInd->Cause[1] == SMS_MM_COLLISION)||(pErrorInd->Cause[1] == SMS_MM_LOWLAYER_FAILURE))
		{
			CSW_PROFILE_FUNCTION_EXIT(sms_mo_Parse_SmsPPErrorInd);
			return ERR_CMS_RESOURCES_UNAVAILABLE;
		}
	}

	CSW_PROFILE_FUNCTION_EXIT(sms_mo_Parse_SmsPPErrorInd);
	return ERR_CMS_PROTOCOLERROR;
}




// CFW->SCL, Send a notification to SCL that ME has memory available
#ifndef CFW_MULTI_SIM
UINT32 sms_mo_SmsPPSendMMAReq(void)
#else
UINT32 sms_mo_SmsPPSendMMAReq(CFW_SIM_ID nSimId)
#endif
{
    api_SmsPPSendMMAReq_t*   pSmsPPSendMMReq = NULL;
    CFW_SMS_PARAMETER   sSmsInfo;
    CSW_PROFILE_FUNCTION_ENTER(sms_mo_SmsPPSendMMAReq);

    SUL_ZeroMemory8(&sSmsInfo,SIZEOF(sSmsInfo));

    //Get SMS parameters
#ifndef CFW_MULTI_SIM
    CFW_CfgGetDefaultSmsParam(&sSmsInfo);
#else
    CFW_CfgGetDefaultSmsParam(&sSmsInfo, nSimId);
#endif
    
    //Malloc api_SmsPPAckReq_t(CFW --> SCL)
    //System will free the memory after get Msg
    pSmsPPSendMMReq = (api_SmsPPSendMMAReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_SmsPPSendMMAReq_t));
    if(!pSmsPPSendMMReq)
    {
	CSW_TRACE(CFW_SMS_TS_ID, "sms_mo_SmsPPSendMMAReq pSmsPPSendMMReq malloc error!!! \n ");
    	CSW_PROFILE_FUNCTION_EXIT(sms_mo_SmsPPSendMMAReq);
       return ERR_NO_MORE_MEMORY;
    }
    SUL_ZeroMemory8(pSmsPPSendMMReq, SIZEOF(api_SmsPPSendMMAReq_t));
    
    //Set Value
    pSmsPPSendMMReq->Path = sSmsInfo.bearer; //0:GSM, 1:GPRS
    
    //Send MSG to SCL
#ifndef CFW_MULTI_SIM
    CFW_SendSclMessage(API_SMSPP_SENDMMA_REQ, pSmsPPSendMMReq);
#else
    CFW_SendSclMessage(API_SMSPP_SENDMMA_REQ, pSmsPPSendMMReq, nSimId);
#endif
    CSW_PROFILE_FUNCTION_EXIT(sms_mo_SmsPPSendMMAReq);
    return ERR_SUCCESS;
}


#ifndef CFW_MULTI_SIM
UINT32 sms_mo_ComposeLongPDU(CFW_SMS_MO_INFO* pSmsMOInfo)
#else
UINT32 sms_mo_ComposeLongPDU(CFW_SMS_MO_INFO* pSmsMOInfo, CFW_SIM_ID nSimId)
#endif
{
    UINT32 ret = 0x0;//, nGetUti=0x0;
    UINT16 nPDU_UDL=0x0;
    UINT8  nStruSize=0x0,nVPF=0x0,nVPLength=0x0,nDALength=0x0,nSCALength=0x0,nDCS=0,nCopySize=0x0 ;
    UINT8* pTmp=NULL;
    UINT8* pCopyPoint=NULL;
    api_SmsPPSendReq_t* pSendSmsReq = NULL;
    CFW_SMS_PARAMETER   sInfo;
    CSW_PROFILE_FUNCTION_ENTER(sms_mo_ComposeLongPDU);

    //Verify VPF, bit4,bit3
    //TP-VPF   bit4-3: xxx1 0xxx: 
    //                   1 0   : TP-VP filed present-relative format
    //                   0 1   : TP-VP filed present-enhanced format
    //                   1 1   : TP-VP filed present-absolute format
    //                   0 0   : TP-VP filed no present
    // *pSmsMOInfo->pData is the length of SCA
    nSCALength = *pSmsMOInfo->pData;
    nVPF = (UINT8)((*(pSmsMOInfo->pData + nSCALength + 1) & 0x18) >> 3);
    if(nVPF == 2)  //TP-VP filed present-relative format
    {
        nVPLength = 1;
    }
    else if(nVPF == 1)//TP-VP filed present-enhanced format
    {
        nVPLength = 7;
    }
    else if(nVPF == 3)//TP-VP filed present-absolute format
    {
        nVPLength = 7;
    }
    else if(nVPF == 0)//TP-VP filed no present
    {
        nVPLength = 0;
    }

    pTmp      = (UINT8*)(pSmsMOInfo->pData + nSCALength + 3); //Point to the length of TP-DA
    nDALength = (UINT8)(*pTmp / 2 + *pTmp % 2 + 2);

    nDCS      = (UINT8)(*(pSmsMOInfo->pData + nSCALength + 1 + 1 + nDALength + 1 + 1));
    nPDU_UDL  = (UINT16)(pSmsMOInfo->nDataSize - 1 - nSCALength - 2 - nDALength - nVPLength - 3);

    //Verify max PDU size 7bit:(140-7)*256 other:(140-6)*256 
    if(((nDCS == 0) && (nPDU_UDL > 34048)) || ((nDCS != 0) && (nPDU_UDL > 34304)))
    {
        //CFW_GetUTI(pSmsMOInfo->hAo, &nGetUti);
        //CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
        //CFW_UnRegisterAO(CFW_SMS_MO_SRV_ID,pSmsMOInfo->hAo);//CFW_APP_SRV_ID
       CSW_PROFILE_FUNCTION_EXIT(sms_mo_ComposeLongPDU);
        return ERR_CMS_UNKNOWN_ERROR;
    }

    if(nPDU_UDL > 0x8C) // 140 bytes
    {
        pSmsMOInfo->isLongerMsg = 1;
        pSmsMOInfo->nLongerEachPartSize = (UINT8)(nDALength + nVPLength + 5 + 0x8C);  //api_SmsPPSendReq_t.DataLen
    }
    else
    {
        pSmsMOInfo->isLongerMsg = 0;
        pSmsMOInfo->nLongerEachPartSize = (UINT8)(pSmsMOInfo->nDataSize - nSCALength - 1); //api_SmsPPSendReq_t.DataLen
    }

    SUL_ZeroMemory8(&sInfo, SIZEOF(CFW_SMS_PARAMETER));
    //Get MR
#ifndef CFW_MULTI_SIM
    CFW_CfgGetDefaultSmsParam(&sInfo);
#else
    CFW_CfgGetDefaultSmsParam(&sInfo, nSimId);
#endif
    pSmsMOInfo->nMR = sInfo.mr;
    nStruSize = (UINT8)(SIZEOF(api_SmsPPSendReq_t) + pSmsMOInfo->nLongerEachPartSize);
    pSendSmsReq = (api_SmsPPSendReq_t*)CFW_MemAllocMessageStructure(nStruSize);
    if(!pSendSmsReq)
    {
	CSW_TRACE(CFW_SMS_TS_ID, "sms_mo_ComposeLongPDU pSendSmsReq malloc error!!! \n ");
    	CSW_PROFILE_FUNCTION_EXIT(sms_mo_ComposeLongPDU);
       return ERR_NO_MORE_MEMORY;
    }
    SUL_ZeroMemory8(pSendSmsReq, nStruSize);
    //Copy all the data to compose a Send data
    if(pSmsMOInfo->isLongerMsg)
        pSendSmsReq->Concat = 1;   
    else
        pSendSmsReq->Concat = 0;   
    pSendSmsReq->Path       = sInfo.bearer; // 0:GSM; 1:GPRS
    pSendSmsReq->AddressLen = nSCALength; //Get SCA length
    *(pSmsMOInfo->pData + nSCALength + 2) = pSmsMOInfo->nMR;   //Set MR
    SUL_MemCopy8((UINT8*)pSendSmsReq->Address,(UINT8*)(pSmsMOInfo->pData + 1),nSCALength);  //copy SCA Address
    //Set MTI
    //Set SSR and RP
    //TP-RP   bit7  : 1xxx xxxx: TP-RP
    //TP UDHI bit6  : x1xx xxxx: TP UDHI, for longer SMS
    //TP SRR  bit5  : xx1x xxxx: TP SRR
    if((pSmsMOInfo->isLongerMsg) && CFW_SIM_SMS_SUPPORT_LONG_MSG)
    {
        UINT8 sLongHead[10];

        SUL_ZeroMemory8(&sLongHead, 10);
        //Set UDHI
        *(pSmsMOInfo->pData + nSCALength + 1) = (UINT8)((*(pSmsMOInfo->pData + nSCALength + 1)) | 0x41);
        pSmsMOInfo->nLongerMsgID      = pSmsMOInfo->nMR;     
        pSmsMOInfo->nLongerMsgCurrent = 1;  
        pSmsMOInfo->nDCS              = nDCS;

        if(nDCS == 8) //UCS2
        {
            pSmsMOInfo->nLongerMsgCount = (UINT8)(nPDU_UDL / (0x8C-6) + 1);  
            pSendSmsReq->DataLen        = (UINT8)(nDALength + nVPLength + 5 + 0x8C);
            sLongHead[0] = 5;                            //UDHL
            sLongHead[1] = 0;                            //IEIa 
            sLongHead[2] = 3;                            //IEIDLa
            sLongHead[3] = (UINT8)pSmsMOInfo->nLongerMsgID;     //IEDa: Octet 1 Concatenated short message reference number.
            sLongHead[4] = pSmsMOInfo->nLongerMsgCount;  //IEDa: Octet 2 Maximum number of short messages in the concatenated short message.
            sLongHead[5] = pSmsMOInfo->nLongerMsgCurrent;//IEDa: Octet 3 Sequence number of the current short message.
            pSmsMOInfo->nLongerLastUserDataSize = (UINT8)(nPDU_UDL % (0x8C-6) + 6);
            //Set TP UDL
            *(pSmsMOInfo->pData + nSCALength + nDALength + nVPLength + 5) = 0x8C;
        }
        else if(nDCS == 0)//GSM 7 bit
        {
            pSmsMOInfo->nLongerMsgCount   = (UINT8)(nPDU_UDL / (0x8C-7)  + 1);  
            pSendSmsReq->DataLen          = (UINT8)(nDALength + nVPLength + 5 + 0x8C);               
            sLongHead[0] = 0x06;                            //UDHL
            sLongHead[1] = 0x00;                            //IEIa ,0:8-bit reference number; 0x08 means 16-bit reference number
            sLongHead[2] = 0x04;                            //IEIDLa
            sLongHead[3] = (UINT8)pSmsMOInfo->nLongerMsgID;     //IEDa: Octet 1 Concatenated short message reference number.
            sLongHead[4] = pSmsMOInfo->nLongerMsgCount;  //IEDa: Octet 2 Maximum number of short messages in the concatenated short message.
            sLongHead[5] = pSmsMOInfo->nLongerMsgCurrent;//IEDa: Octet 3 Sequence number of the current short message.
            sLongHead[6] = 0x0;                            //Fill bits
            pSmsMOInfo->nLongerLastUserDataSize = (UINT8)(nPDU_UDL % (0x8C-7) + 7); 
            //Set TP UDL
            *(pSmsMOInfo->pData + nSCALength + nDALength + nVPLength + 5) = 0xA0;
        }
        else //8 bit
        {
            pSmsMOInfo->nLongerMsgCount = (UINT8)(nPDU_UDL / (0x8C-6) + 1);  
            pSendSmsReq->DataLen        = (UINT8)(nDALength + nVPLength + 5 + 0x8C);
            sLongHead[0] = 5;                            //UDHL
            sLongHead[1] = 0;                            //IEIa 
            sLongHead[2] = 3;                            //IEIDLa
            sLongHead[3] = (UINT8)pSmsMOInfo->nLongerMsgID;     //IEDa: Octet 1 Concatenated short message reference number.
            sLongHead[4] = pSmsMOInfo->nLongerMsgCount;  //IEDa: Octet 2 Maximum number of short messages in the concatenated short message.
            sLongHead[5] = pSmsMOInfo->nLongerMsgCurrent;//IEDa: Octet 3 Sequence number of the current short message.
            pSmsMOInfo->nLongerLastUserDataSize = (UINT8)(nPDU_UDL % (0x8C-6)+ 6);
            //Set TP UDL
            *(pSmsMOInfo->pData + nSCALength + nDALength + nVPLength + 5) = 0x8C ;
        }
        //copy the data before head
        pTmp       = (UINT8*)(pSendSmsReq->Data);    
        pCopyPoint = (UINT8*)(pSmsMOInfo->pData + pSendSmsReq->AddressLen + 1);
        nCopySize  = (UINT8)(nDALength + 6);
        SUL_MemCopy8(pTmp, pCopyPoint,nCopySize);
        //copy long head content
        pTmp       = (UINT8*)(pTmp + nCopySize);
        pCopyPoint = (UINT8*)(&sLongHead);
        if(nDCS == 0)//GSM 7 bit
            nCopySize  = (UINT8)(sLongHead[0] + 2);
        else
            nCopySize  = (UINT8)(sLongHead[0] + 1);
        SUL_MemCopy8(pTmp, pCopyPoint,nCopySize);
        //copy the data after head
        pTmp       = (UINT8*)(pTmp + nCopySize);
        pCopyPoint = (UINT8*)(pSmsMOInfo->pData + nSCALength + nDALength + 7);
        nCopySize  = (UINT8)(0x8C - sLongHead[0] - 1);
        SUL_MemCopy8(pTmp, pCopyPoint,nCopySize);
        //pData offset
        pSmsMOInfo->pData = (UINT8*)(pSmsMOInfo->pData + nSCALength + 0x8C + nDALength + 7 - sLongHead[0] - 1);
        pSmsMOInfo->nLongerIndexOffset = (UINT8)(nDALength + 6);
    }
    else if((pSmsMOInfo->isLongerMsg) && !CFW_SIM_SMS_SUPPORT_LONG_MSG)//long sms, but doesn't support
    {
    	CSW_PROFILE_FUNCTION_EXIT(sms_mo_ComposeLongPDU);
        return ERR_CMS_UNKNOWN_ERROR;
    }
    else if(!pSmsMOInfo->isLongerMsg)//normal sms
    {
        *(pSmsMOInfo->pData + nSCALength + 1) = (UINT8)((*(pSmsMOInfo->pData + nSCALength + 1)) | 0x01);
        pSendSmsReq->DataLen    = pSmsMOInfo->nLongerEachPartSize;
        SUL_MemCopy8((UINT8*)pSendSmsReq->Data,(UINT8*)(pSmsMOInfo->pData + pSendSmsReq->AddressLen + 1),pSendSmsReq->DataLen);
    }

    if(pSmsMOInfo->isLongerMsg)
    {
        pSmsMOInfo->pSendSmsReq = (api_SmsPPSendReq_t*)CSW_SMS_MALLOC(nStruSize);
        if(!pSmsMOInfo->pSendSmsReq)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "sms_mo_ComposeLongPDU pSmsMOInfo->pSendSmsReq malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(sms_mo_ComposeLongPDU);
		return ERR_NO_MORE_MEMORY;
      }
        SUL_ZeroMemory8(pSmsMOInfo->pSendSmsReq, nStruSize);
        SUL_MemCopy8((UINT8*)pSmsMOInfo->pSendSmsReq,(UINT8*)(pSendSmsReq),nStruSize);
    }

	CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, (UINT8 *)pSendSmsReq,(UINT16)nStruSize ,16);
#ifndef CFW_MULTI_SIM

    ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq);
#else

    ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq, nSimId);
#endif
    if(ret == ERR_SUCCESS )
    {
        CFW_SetAoState(pSmsMOInfo->hAo, CFW_SM_STATE_WAIT);
    }
    else 
    {
        CSW_SMS_FREE(pSmsMOInfo->pSendSmsReq);
        return ERR_CMS_UNKNOWN_ERROR;
    }
    
    CSW_PROFILE_FUNCTION_EXIT(sms_mo_ComposeLongPDU); 
    return ERR_SUCCESS;
}

//[[xueww[+] 2007.08.21
UINT32 sms_tool_CheckDcs(UINT8* pDcs)
{
	UINT8 Dcs = *pDcs;
    CSW_PROFILE_FUNCTION_ENTER(sms_tool_CheckDcs); 

	//check bit7 bit6
	if((Dcs >> 6) == 0)
	{
		//bit5, if set to 0, indicates the text is uncompressed
		//bit5, if set to 1, indicates the text is compressed using the  compression algorithm defined in 3G TS 23.042 [13]
		if((Dcs & 0x20) == 0)//bit5 = 0
		{
			//Bit 4, if set to 0, indicates that bits 1 to 0 are reserved and have no message class meaning
			//Bit 4, if set to 1, indicates that bits 1 to 0 have a message class meaning
			if((Dcs & 0x10) == 0)//bit4 = 0
			{
				*pDcs = Dcs & 0x0c;
			}
			else if((Dcs & 0x10) == 0x10)//bit4 = 1
			{
				/*
				Bit1	Bit0 Message Class
				0    0  	Class 0
				0    1      Class 1   Default meaning: ME-specific.
				1    0  	Class 2   (U)SIM specific message
				1    1  	Class 3   Default meaning: TE specific (see 3G TS 27.005 [8]) 
				*/
				if((Dcs & 0x03) == 0)
				{
					//Class 0
				}
				else if((Dcs & 0x03) == 1)
				{
					//Class 1
				}
				else if((Dcs & 0x03) == 2)
				{
					//Class 2
				}
				else if((Dcs & 0x03) == 3)
				{
					//Class 3
				}
				
				/*
				Bit3	Bit2	 Alphabet:
				0    0      GSM 7 bit default alphabet
				0    1      8 bit data
				1    0      UCS2 (16bit) [10]
				1    1      Reserved
				*/
				if((Dcs & 0x0c) == 0x00)
				{
					*pDcs = 0;
				}
				else if((Dcs & 0x0c) == 0x04)
				{
					*pDcs = 4;
				}
				else if((Dcs & 0x0c) == 0x08)
				{
					*pDcs = 8;
				}
				else if((Dcs & 0x0c) == 0x0c)
				{
					//Reserved
					return ERR_INVALID_PARAMETER;
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_CheckDcs);
				}
			}
		}
		else if((Dcs & 0x20) == 0x20)//bit5 = 1
		{
			//Bit 4, if set to 0, indicates that bits 1 to 0 are reserved and have no message class meaning
			//Bit 4, if set to 1, indicates that bits 1 to 0 have a message class meaning
			if((Dcs & 0x10) == 0)//bit4 = 0
			{
				*pDcs = Dcs & 0x2c;
			}
			else if((Dcs & 0x10) == 0x10)//bit4 = 1
			{
				/*
				Bit1	Bit0 Message Class
				0    0  	Class 0
				0    1      Class 1   Default meaning: ME-specific.
				1    0  	Class 2   (U)SIM specific message
				1    1  	Class 3   Default meaning: TE specific (see 3G TS 27.005 [8]) 
				*/
				if((Dcs & 0x03) == 0)
				{
					//Class 0
				}
				else if((Dcs & 0x03) == 1)
				{
					//Class 1
				}
				else if((Dcs & 0x03) == 2)
				{
					//Class 2
				}
				else if((Dcs & 0x03) == 3)
				{
					//Class 3
				}
				
				/*
				Bit3	Bit2	 Alphabet:
				0    0      GSM 7 bit default alphabet
				0    1      8 bit data
				1    0      UCS2 (16bit) [10]
				1    1      Reserved
				*/
				if((Dcs & 0x0c) == 0x00)
				{
					*pDcs = 0x20;
				}
				else if((Dcs & 0x0c) == 0x04)
				{
					*pDcs = 0x24;
				}
				else if((Dcs & 0x0c) == 0x08)
				{
					*pDcs = 0x28;
				}
				else if((Dcs & 0x0c) == 0x0c)
				{
					//Reserved
					return ERR_INVALID_PARAMETER;
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_CheckDcs);
				}
			}
		}
	}
	else if(((Dcs >> 6) == 1) || ((Dcs >> 6) == 2))
	{
		//Reserved coding groups
		return ERR_INVALID_PARAMETER;
		CSW_PROFILE_FUNCTION_EXIT(sms_tool_CheckDcs);
	}
	else if((Dcs >> 6) == 3)
	{
		//check bit5 bit4
		if((Dcs >> 4) == 0X0C)
		{
			//discard the contents of the message. (3G TS 23.038 [4]) 
			*pDcs = 0;//to do
		}
		else if((Dcs >> 4) == 0X0D)
		{
			//Text included in the user data is coded in 
			//the GSM 7 bit default alphabet.(3G TS 23.038 [4]) 
			//bit3~bit0 to do...
			*pDcs = 0;
		}
		else if((Dcs >> 4) == 0X0E)
		{
			//Text included in the user data is coded in 
			//the uncompressed UCS2 alphabet.(3G TS 23.038 [4]) 
			//bit3~bit0 to do...
			*pDcs = 8;
		}
		else if((Dcs >> 4) == 0X0F)
		{
			/*
			Bit1	Bit0 Message Class
			0    0  	Class 0
			0    1      Class 1   Default meaning: ME-specific.
			1    0  	Class 2   (U)SIM specific message
			1    1  	Class 3   Default meaning: TE specific (see 3G TS 27.005 [8]) 
			*/
			if((Dcs & 0x03) == 0)
			{
				//Class 0
			}
			else if((Dcs & 0x03) == 1)
			{
				//Class 1
			}
			else if((Dcs & 0x03) == 2)
			{
				//Class 2
			}
			else if((Dcs & 0x03) == 3)
			{
				//Class 3
			}

			/*
			Bit 2	  Message coding:
			 0		GSM 7 bit default alphabet
			 1		8-bit data
			*/
			if((Dcs & 0x04) == 0)
			{
				*pDcs = 0;
			}
			else if((Dcs & 0x04) == 0x04)
			{
				*pDcs = 4;
			}
		}
	}
	
	return ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_EXIT(sms_tool_CheckDcs);
}
//]]xueww[+] 2007.08.21

#ifndef CFW_MULTI_SIM
//Get all the long sms info from ME,then save them to "C:/System/MeLong.txt"
UINT32 sms_mo_InitMeLongMSGPara()
{
    UINT16    i=0x0,nLongStruCount=0x0;
    UINT8*    pReadData=NULL; 
    UINT32    ret=0x0,nWriteBuf=0x0;
    CFW_EV    sMeEvent;
    CFW_SMS_STORAGE_INFO  sStorageInfo;
    CFW_SMS_LONG_INFO*    pSmsLongInfo=NULL;   
    CSW_PROFILE_FUNCTION_ENTER(sms_mo_InitMeLongMSGPara);   

    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
    SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
    //This is a temp memory. 
    pReadData = (UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4);
    if(!pReadData)
    {
         CSW_TRACE(CFW_SMS_TS_ID, "sms_mo_InitMeLongMSGPara pReadData malloc error!!! \n ");
         CSW_PROFILE_FUNCTION_EXIT(sms_mo_InitMeLongMSGPara);
         return ERR_NO_MORE_MEMORY;
    }
    SUL_ZeroMemory8(pReadData, SMS_MO_ONE_PDU_SIZE+4+4);
    sMeEvent.nParam1 = (UINT32)pReadData;
    CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_ME);
    for(i=1;i<=sStorageInfo.totalSlot;i++)
    {
        CFW_MeReadMessage(CFW_SMS_STORAGE_ME,i,&sMeEvent);
        if(sMeEvent.nType == 0)
        {
            UINT8   nStatus=0x0;
            UINT32  nPduOutData=0x0;
            CFW_SMS_MULTIPART_INFO sLongerMsg;

            //Zero memory
            SUL_ZeroMemory8(&sLongerMsg,SIZEOF(CFW_SMS_MULTIPART_INFO));
            //Just get SMS status
            sms_tool_DecomposePDU(pReadData,0,NULL,&nStatus,NULL);
            //Then get long info
            if((nStatus == CFW_SMS_STORED_STATUS_UNREAD) || \
                (nStatus == CFW_SMS_STORED_STATUS_READ) ) //Deliver sms
            {
                sms_tool_DecomposePDU(pReadData,2,&nPduOutData,&nStatus,&sLongerMsg);
                CSW_SMS_FREE((UINT8*)nPduOutData);
            }//Submitted message.
            else if((nStatus == CFW_SMS_STORED_STATUS_UNSENT)               || \
                    (nStatus == CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)      || \
                    (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || \
                    (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || \
                    (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE) )
            {
                //Text Mode, Submitted message. when  CFW_CfgSetSmsShowTextModeParam(1)
                //Parse pEvent->nParam1 to CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO  
                sms_tool_DecomposePDU(pReadData,4,&nPduOutData,&nStatus,&sLongerMsg);
                CSW_SMS_FREE((UINT8*)nPduOutData);
            }
            if(sLongerMsg.count > 0)
            {
                CFW_SMS_LONG_INFO*    pTmpSmsLongInfo=NULL;   

                //Malloc CFW_SMS_LONG_INFO for pStru_SmsLongInfo
                pTmpSmsLongInfo = (CFW_SMS_LONG_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_LONG_INFO));
                if(!pTmpSmsLongInfo)
		  {
			CSW_TRACE(CFW_SMS_TS_ID, "sms_mo_InitMeLongMSGPara pTmpSmsLongInfo malloc error!!! \n ");
			CSW_PROFILE_FUNCTION_EXIT(sms_mo_InitMeLongMSGPara);
			return ERR_CMS_UNKNOWN_ERROR;
                }
                SUL_ZeroMemory8(pTmpSmsLongInfo, (SIZEOF(CFW_SMS_LONG_INFO)));
                pTmpSmsLongInfo->count   = sLongerMsg.count;
                pTmpSmsLongInfo->current = sLongerMsg.current;
                pTmpSmsLongInfo->id      = sLongerMsg.id;
                pTmpSmsLongInfo->nStatus = nStatus;
                pTmpSmsLongInfo->index   = i;
                pTmpSmsLongInfo->pNext   = NULL;
                //Conncet the struct to the link
                if(pSmsLongInfo)
                {
                    CFW_SMS_LONG_INFO *pLink=NULL;
                    pLink = pSmsLongInfo;
                    while(pLink->pNext)
                    {
                        pLink = (CFW_SMS_LONG_INFO*)(pLink->pNext);
                    }
                    pLink->pNext = (UINT32*)pTmpSmsLongInfo;
                }
                else // for the first one
                    pSmsLongInfo = pTmpSmsLongInfo;

                //Get the total long count;
                nLongStruCount++;
            }//end  if(sLongerMsg.count > 0)

        }// end if(sMeEvent.nType == 0)
        SUL_ZeroMemory8(pReadData, SMS_MO_ONE_PDU_SIZE+4+4);
    }
    CSW_SMS_FREE(pReadData);
    if(nLongStruCount >0)
    {
        ret = sms_tool_Parse_LongMSGPara(pSmsLongInfo,nLongStruCount,&nWriteBuf);
        if(ret != ERR_SUCCESS){
        	CSW_PROFILE_FUNCTION_EXIT(sms_mo_InitMeLongMSGPara);
          return ERR_CMS_UNKNOWN_ERROR;
          }
        ret = sms_tool_SetLongMSGPara(CFW_SMS_STORAGE_ME,(UINT8*)nWriteBuf,(INT32)(nLongStruCount * SIZEOF(CFW_SMS_LONG_INFO_NODE)),LONG_SMS_PARA_CREATE);
        CSW_SMS_FREE((UINT32*)nWriteBuf);
        if(ret != ERR_SUCCESS){
        	CSW_PROFILE_FUNCTION_EXIT(sms_mo_InitMeLongMSGPara);
          return ERR_CMS_UNKNOWN_ERROR;
          }
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_mo_InitMeLongMSGPara);
    return ERR_SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////
//                         APIs for Sync operation                           //
///////////////////////////////////////////////////////////////////////////////
VOID sms_Sync_Read(CFW_SMS_MO_INFO* pSmsMOInfo, CFW_SMS_OUT_PARAM* pOutParam)
{
    UINT32  ret=0x0,sOutData=0x0;
    CFW_EV* pEvent=NULL;
    CFW_EV  sMeEvent;
    UINT8   nSmsStatus=0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_Sync_Read);
 
    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
    //Malloc for the read data.
    sMeEvent.nParam1 = (UINT32)((UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4));
    if(!(UINT8*)sMeEvent.nParam1)
    {
	 CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_Read sMeEvent.nParam1 malloc error!!! \n ");
        pSmsMOInfo->sCfw_Event.nFlag   = 1;
        pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
        pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
        pSmsMOInfo->sCfw_Event.nType   = 0xf0;
        goto nFlagEvent;
    }
    SUL_ZeroMemory8((UINT8*)sMeEvent.nParam1,SMS_MO_ONE_PDU_SIZE+4+4);
    ret = CFW_MeReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,&sMeEvent);
    if(ret == ERR_SUCCESS)
        pEvent = &sMeEvent;
    else  //other return value
    {
        pSmsMOInfo->sCfw_Event.nFlag   = 1;
        pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
        pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
        pSmsMOInfo->sCfw_Event.nType   = 0xf0;
        goto nFlagEvent;
    }
    if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1))
    {
        CFW_SMS_MULTIPART_INFO sLongerMsg;
        //Zero memory
        SUL_ZeroMemory8(&sLongerMsg,SIZEOF(CFW_SMS_MULTIPART_INFO));
        //Just get SMS status
        ret = sms_tool_DecomposePDU((PVOID)pEvent->nParam1,0,NULL,&nSmsStatus,NULL);
        if(ret != ERR_SUCCESS)
        {
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
            pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
            goto nFlagEvent;
        }
        //Parse the data
        ret = sms_mo_Parse_PDU(pSmsMOInfo->nFormat,pSmsMOInfo->nPath,nSmsStatus,pSmsMOInfo->nLocation,(PVOID)pEvent->nParam1,&pEvent->nType,&sOutData,&sLongerMsg);
        if(ret != ERR_SUCCESS)//other data, such as Status report or Command
        {
            CFW_SMS_NODE* pTmpNode=NULL;
            pTmpNode   = (CFW_SMS_NODE *)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO) + SIZEOF(CFW_SMS_NODE));
            if(!pTmpNode)
            {
		  CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_Read pTmpNode malloc error!!! \n ");
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
                pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                goto nFlagEvent;
            }
            SUL_ZeroMemory8(pTmpNode,(SIZEOF(CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO) + SIZEOF(CFW_SMS_NODE)));
		    pTmpNode->pNode = (PVOID)((UINT8*)pTmpNode + sizeof(CFW_SMS_NODE));
            sOutData = (UINT32)pTmpNode;
            nSmsStatus = CFW_SMS_STORED_STATUS_READ;
        }
        if(sLongerMsg.count > 0) // a longer msg
        {
            UINT16 nConcatSmsPrevRec=0x0, nConcatSmsNextRec=0x0;
            sms_tool_GetLongMSGPara((UINT8)pSmsMOInfo->nLocation,pSmsMOInfo->nIndex, &nConcatSmsPrevRec, &nConcatSmsNextRec);
            ((CFW_SMS_NODE*)sOutData)->nConcatPrevIndex = nConcatSmsPrevRec ;
            ((CFW_SMS_NODE*)sOutData)->nConcatNextIndex = nConcatSmsNextRec ;
        }
        else // a normal short message
        {
            ((CFW_SMS_NODE*)sOutData)->nConcatPrevIndex = 0xffff ;
        }
        //Set value
        ((CFW_SMS_NODE*)sOutData)->nConcatCurrentIndex   = pSmsMOInfo->nIndex;
        ((CFW_SMS_NODE*)sOutData)->nStatus               = nSmsStatus;
        ((CFW_SMS_NODE*)sOutData)->nType                 = pEvent->nType;
        ((CFW_SMS_NODE*)sOutData)->nStorage              = pSmsMOInfo->nLocation;
        //If the unread SMS is read by this function, the SMS message status will be changed to read status by CFW automaticly
        if(nSmsStatus == CFW_SMS_STORED_STATUS_UNREAD)
        {
            CFW_EV  sMeEvent1;
            CFW_SMS_STORAGE_INFO  sStorageInfo;

            //Zero memory
            SUL_ZeroMemory8(&sMeEvent1,SIZEOF(CFW_EV));
            SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
            CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
            if(sStorageInfo.unReadRecords > 0)
                sStorageInfo.unReadRecords--;
            if(sStorageInfo.readRecords < sStorageInfo.totalSlot)
                sStorageInfo.readRecords++;
            CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
            *((UINT8*)pEvent->nParam1) = CFW_SMS_STORED_STATUS_READ;
            if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
            {
                //Write it with CFW_SMS_STORED_STATUS_READ
                ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
            }
            else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
            {
                UINT32 nTime=0x0, nAppInt32=0x0;
                ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent1);
            }
            if(ret != ERR_SUCCESS)
            {
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
                pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                goto nFlagEvent;
            }
            pSmsMOInfo->nReadSmsStatus = CFW_SMS_STORED_STATUS_UNREAD;
        }
        if(pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)
            CSW_SMS_FREE((UINT8*)(sMeEvent.nParam1));
        pEvent->nParam2 = 0;
        pEvent->nType = 0;
    }
    else if(pEvent->nType == 0xf0) 
    {
        pEvent->nParam2 = pSmsMOInfo->nIndex;
        if(pEvent->nParam1 == ERR_CME_ME_FAILURE)  //Me Return error
        {
            sOutData = ERR_CMS_INVALID_MEMORY_INDEX;
        }
        else if(pEvent->nParam1 == ERR_CME_INVALID_INDEX)//Me Return error
        {
            sOutData = ERR_CMS_INVALID_MEMORY_INDEX;
        }
        else if(pEvent->nParam1 == ERR_CMS_INVALID_MEMORY_INDEX)
        {
            sOutData = ERR_CMS_INVALID_MEMORY_INDEX;
        }
        else if(pEvent->nParam1 == ERR_CME_SIM_PIN_REQUIRED)
        {
            sOutData = ERR_CMS_TEMPORARY_FAILURE;
        }
        else if(pEvent->nParam1 == ERR_CME_SIM_FAILURE)
        {
            sOutData = ERR_CMS_TEMPORARY_FAILURE;
        }
        else if(pEvent->nParam1 == ERR_CME_SIM_NOT_INSERTED)
        {
            sOutData = ERR_CMS_TEMPORARY_FAILURE;
        }
        else  
        {
            sOutData = ERR_CMS_UNKNOWN_ERROR;
        }
    }
    else
    {
        pEvent->nType   = 0xf0;
        pEvent->nParam2 = pSmsMOInfo->nIndex;
        sOutData        = ERR_CMS_UNKNOWN_ERROR;
    }
    pSmsMOInfo->sCfw_Event.nParam1 = sOutData;
    pSmsMOInfo->sCfw_Event.nParam2 = pEvent->nParam2;
    pSmsMOInfo->sCfw_Event.nType   = pEvent->nType;
    if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)    || \
       (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_BM)    || \
       (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME_SR) || \
       (pSmsMOInfo->nReadSmsStatus != CFW_SMS_STORED_STATUS_UNREAD))
    {
        pSmsMOInfo->sCfw_Event.nFlag   = 1;
        pSmsMOInfo->sCfw_Event.nType   = pEvent->nType;
        goto nFlagEvent;
    }

nFlagEvent:
    //Wait for the event result
    pOutParam->nParam1 = pSmsMOInfo->sCfw_Event.nParam1;
    pOutParam->nParam2 = pSmsMOInfo->sCfw_Event.nParam2;
    pOutParam->nType   = pSmsMOInfo->sCfw_Event.nType;
    CSW_PROFILE_FUNCTION_EXIT(sms_Sync_Read);
}

VOID sms_Sync_Delete(CFW_SMS_MO_INFO* pSmsMOInfo, CFW_SMS_OUT_PARAM* pOutParam)
{
    UINT32  ret=0x0;
    CFW_EV* pEvent=NULL;
    CFW_EV  sMeEvent;
    CSW_PROFILE_FUNCTION_ENTER(sms_Sync_Delete);
 
    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
    if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_ME)
    {
        if(pSmsMOInfo->nIndex == 0)//delete both by status
        {
            ret = CFW_MeDeleteMessage(CFW_SMS_STORAGE_ME,pSmsMOInfo->nIndex,pSmsMOInfo->nStatus,&sMeEvent);
            if(ret != ERR_SUCCESS)
            {
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
                pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                goto nFlagEvent;
            }
            pSmsMOInfo->nLocation = CFW_SMS_STORAGE_SM;
        }
        else //by index
        {
            pSmsMOInfo->nLocation = CFW_SMS_STORAGE_ME;
        }
    }
    if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) || (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_SR))
    {
        if(pSmsMOInfo->nIndex) // delete by index, read it first, got its status
        {
//            ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal);
//            if(ret == ERR_SUCCESS)
//            {
//                CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
//            }
//            else
//            {
//                CFW_GetUTI(hAo, &nGetUti);
//                CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
//                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
//                return ERR_CMS_UNKNOWN_ERROR;
//            }
        }
        else //by status, get the number of specified status
        {
//            ret = CFW_SimDeleteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nStatus,pSmsMOInfo->nUTI_Internal);
//            if(ret == ERR_SUCCESS)
//            {
//                CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
//            }
//            else 
//            {
//                CFW_GetUTI(hAo, &nGetUti);
//                CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
//                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
//                return ERR_CMS_UNKNOWN_ERROR;
//            }
        }
//        break;
    }
    if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)  || \
       (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_BM)  || \
       (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME_SR))
    {
        if(pSmsMOInfo->nIndex) // delete by index, read it first, got its status
        {
            sMeEvent.nParam1 = (UINT32)((UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4));
            if(!(UINT8*)sMeEvent.nParam1)
            {
		  CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_Delete 1 sMeEvent.nParam1 malloc error!!! \n ");
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = ERR_NO_MORE_MEMORY;
                pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
                pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                goto nFlagEvent;
            }
            SUL_ZeroMemory8((UINT8*)sMeEvent.nParam1,SMS_MO_ONE_PDU_SIZE+4+4);
            CFW_MeReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,&sMeEvent);
            pEvent = &sMeEvent;
            if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1))
            {
                //Just get SMS status
                sms_tool_DecomposePDU((PVOID)pEvent->nParam1,0,NULL,&(pSmsMOInfo->nStatus),NULL);
            }
            //Read a NULL SIM
            else if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1 == 0))
            {
                pSmsMOInfo->nStatus = 0;
            }                    
            else if(pEvent->nType == 0xf0) 
            {
                pSmsMOInfo->nStatus = 0;
            }
            CSW_SMS_FREE((UINT8*)sMeEvent.nParam1);
        }
        else //by status, get the number of specified status
        {
            sMeEvent.nParam1 = (UINT32)((UINT8*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_STORAGE_INFO)));
            if((UINT8*)sMeEvent.nParam1 == NULL)
            {
		  CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_Delete 2 sMeEvent.nParam1 malloc error!!! \n ");
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = ERR_NO_MORE_MEMORY;
                pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
                pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                goto nFlagEvent;
            }
            SUL_ZeroMemory8((UINT8*)sMeEvent.nParam1, SIZEOF(CFW_SMS_STORAGE_INFO));
            //Get ME totalSlot and usedSlot
            CFW_MeGetStorageInfo(CFW_SMS_STORAGE_ME,pSmsMOInfo->nStatus,&sMeEvent);
            if(sMeEvent.nType == 0) // use nListCount to record the number
                pSmsMOInfo->nListCount = ((CFW_SMS_STORAGE_INFO*)(sMeEvent.nParam1))->usedSlot;
            else
                pSmsMOInfo->nListCount = 0;
            CSW_SMS_FREE((UINT8*)sMeEvent.nParam1);
        }
        ret = CFW_MeDeleteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nStatus,&sMeEvent);
        if(ret == ERR_SUCCESS)
        {
            //Go to CFW_SM_STATE_WAIT directly!
            pEvent = &sMeEvent;
            pEvent->nTransId = pSmsMOInfo->nUTI_Internal;
        }
        else  //other return value
        {
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
            pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
            goto nFlagEvent;
        }
    }
    if((pEvent->nEventId == EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP) && (pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
    {
//        if(pEvent->nType == 0)
//            pSmsMOInfo->nListCount = (UINT16)(pEvent->nParam1 & 0x0000ffff);
//        else
//            pSmsMOInfo->nListCount = 0;
//        ret = CFW_SimDeleteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nStatus,pSmsMOInfo->nUTI_Internal);
//        if(ret == ERR_SUCCESS)
//        {
//        }
//        else 
//        {
//            pSmsMOInfo->sCfw_Event.nFlag   = 1;
//            pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
//            pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
//            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
//            goto nFlagEvent;
//        }
//        break;
    }
    else if((pEvent->nEventId == EV_CFW_SIM_READ_MESSAGE_RSP) && (pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
    {
//        if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1))
//        {
//            //Just get SMS status
//            sms_tool_DecomposePDU((PVOID)pEvent->nParam1,0,NULL,&(pSmsMOInfo->nStatus),NULL);
//        }
//        //Read a NULL SIM
//        else if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1 == 0))
//        {
//            pSmsMOInfo->nStatus = 0;
//        }                    
//        else if(pEvent->nType == 0xf0) 
//        {
//            pSmsMOInfo->nStatus = 0;
//        }
//        ret = CFW_SimDeleteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nStatus,pSmsMOInfo->nUTI_Internal);
//        if(ret == ERR_SUCCESS)
//        {
//            CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
//        }
//        else 
//        {
//            CFW_GetUTI(hAo, &nGetUti);
//            CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
//            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
//            return ERR_CMS_UNKNOWN_ERROR;
//        }
//        break;
    }
    else if(((pEvent->nEventId == EV_CFW_SIM_DELETE_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_DELETE_MESSAGE_RSP)) && \
            (pEvent->nTransId  == pSmsMOInfo->nUTI_Internal))
    {
        if(pEvent->nType == 0)
        {
            UINT8 nNewCount=0x0;
            CFW_SMS_STORAGE_INFO  sStorageInfo;
            CFW_SMS_LONG_INFO_NODE sSmsLongInfoNode;

            //Delete SIM by status, get the number of specified status sms
            if((pEvent->nEventId == EV_CFW_SIM_DELETE_MESSAGE_RSP) && (0 == pSmsMOInfo->nIndex))
                pSmsMOInfo->nListCount = (UINT16)pEvent->nParam1;
            SUL_ZeroMemory8(&sSmsLongInfoNode,SIZEOF(sSmsLongInfoNode));
            if((0 == pSmsMOInfo->nIndex) && pSmsMOInfo->nListCount) //delete by status
            {
                ret = CFW_CfgGetSmsFullForNew(&nNewCount);
                if(nNewCount >= 1)
                {
//                    CFW_GetUTI(hAo, &nGetUti);
//                    CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND,0,pSmsMOInfo->nLocation,nGetUti,0);
//                    sms_mo_SmsPPSendMMAReq();
                    if(nNewCount >= pSmsMOInfo->nListCount)
                        nNewCount = (UINT8)(nNewCount - pSmsMOInfo->nListCount);
                    else
                        nNewCount = 0;
                    CFW_CfgSetSmsFullForNew(nNewCount);
                }
                //Delete msg with status, then update CFW_SMS_STORAGE_INFO
                SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
                CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                if(sStorageInfo.usedSlot >= pSmsMOInfo->nListCount)
                    sStorageInfo.usedSlot = (UINT16)(sStorageInfo.usedSlot - pSmsMOInfo->nListCount);
                else
                    sStorageInfo.usedSlot = 0;
                if((0x7f == pSmsMOInfo->nStatus) ||(CFW_SMS_STORED_STATUS_STORED_ALL == pSmsMOInfo->nStatus))
                {
                    pSmsMOInfo->nStatus = 0x7f;
                    sStorageInfo.usedSlot = 0;
                }
                if(CFW_SMS_STORED_STATUS_UNREAD & pSmsMOInfo->nStatus)
                    sStorageInfo.unReadRecords = 0;
                if(CFW_SMS_STORED_STATUS_READ & pSmsMOInfo->nStatus)
                    sStorageInfo.readRecords = 0;
                if(CFW_SMS_STORED_STATUS_UNSENT & pSmsMOInfo->nStatus)
                    sStorageInfo.unsentRecords = 0;
                if((CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ & pSmsMOInfo->nStatus)         || \
                   (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV & pSmsMOInfo->nStatus)    || \
                   (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE & pSmsMOInfo->nStatus)   || \
                   (CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE & pSmsMOInfo->nStatus)) 
                    sStorageInfo.sentRecords = 0;
                CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation); 
                //Update longer sms para if it is
                sSmsLongInfoNode.nConcatCurrentIndex = pSmsMOInfo->nIndex;
                sSmsLongInfoNode.nStatus = pSmsMOInfo->nStatus;
#if USING_FS_SAVE_LONG_PARA                        
                sms_tool_SetLongMSGPara((UINT8)pSmsMOInfo->nLocation,(UINT8*)&sSmsLongInfoNode,0,LONG_SMS_PARA_DELETE);
#endif
            }
            else if(pSmsMOInfo->nStatus) //delete by index
            {
                ret = CFW_CfgGetSmsFullForNew(&nNewCount);
                if(nNewCount >= 1)
                {
//                    CFW_GetUTI(hAo, &nGetUti);
//                    CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND,0,pSmsMOInfo->nLocation,nGetUti,0);
//                    sms_mo_SmsPPSendMMAReq();
                    nNewCount--;
                    CFW_CfgSetSmsFullForNew(nNewCount);
                }
                //Delete a msg, then update CFW_SMS_STORAGE_INFO
                SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
                CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                if(sStorageInfo.usedSlot > 0)
                    sStorageInfo.usedSlot--;
                if((CFW_SMS_STORED_STATUS_UNREAD == pSmsMOInfo->nStatus) && (sStorageInfo.unReadRecords>0))
                    sStorageInfo.unReadRecords--;
                else if((CFW_SMS_STORED_STATUS_READ == pSmsMOInfo->nStatus) && (sStorageInfo.readRecords>0))
                    sStorageInfo.readRecords--;
                else if((CFW_SMS_STORED_STATUS_UNSENT == pSmsMOInfo->nStatus) && (sStorageInfo.unsentRecords>0))
                    sStorageInfo.unsentRecords--;
                else if(((CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ == pSmsMOInfo->nStatus)         || \
                         (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV == pSmsMOInfo->nStatus)    || \
                         (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE == pSmsMOInfo->nStatus)   || \
                         (CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE == pSmsMOInfo->nStatus))       && \
                         (sStorageInfo.sentRecords>0))
                    sStorageInfo.sentRecords--;
                CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);                        
                //Update longer sms para if it is
                sSmsLongInfoNode.nConcatCurrentIndex = pSmsMOInfo->nIndex;
                sSmsLongInfoNode.nStatus = pSmsMOInfo->nStatus;
#if USING_FS_SAVE_LONG_PARA                        
                sms_tool_SetLongMSGPara((UINT8)pSmsMOInfo->nLocation,(UINT8*)&sSmsLongInfoNode,0,LONG_SMS_PARA_DELETE);
#endif
            }
            pEvent->nParam1 = pSmsMOInfo->nIndex;
            pEvent->nParam2 = pSmsMOInfo->nLocation;
        }
        else if(pEvent->nType == 0xf0)
        {
            if(pEvent->nParam1 == ERR_CME_ME_FAILURE)  //Me Return error
            {
                pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
            }
            else if(pEvent->nParam1 == ERR_CME_INVALID_INDEX)
            {
                pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
            }
            else if(pEvent->nParam1 == ERR_CMS_INVALID_MEMORY_INDEX)
            {
                pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
            }
            else if(pEvent->nParam1 == ERR_CME_SIM_PIN_REQUIRED)
            {
                pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
            }
            else if(pEvent->nParam1 == ERR_CME_SIM_PIN_REQUIRED)
            {
                pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
            }
            else if(pEvent->nParam1 == ERR_CME_SIM_FAILURE)
            {
                pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
            }
            else if(pEvent->nParam1 == ERR_CME_SIM_NOT_INSERTED)
            {
                pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
            }
            else  
            {
                pEvent->nParam1 = ERR_CMS_UNKNOWN_ERROR;
            }
        }
        else
        {
            pEvent->nParam1 = ERR_CMS_UNKNOWN_ERROR;
        }
    }
    pSmsMOInfo->sCfw_Event.nFlag   = 1;
    pSmsMOInfo->sCfw_Event.nParam1 = pEvent->nParam1;
    pSmsMOInfo->sCfw_Event.nParam2 = pEvent->nParam2;
    pSmsMOInfo->sCfw_Event.nType   = pEvent->nType;
    goto nFlagEvent;
    
nFlagEvent:
    //Wait for the event result
    pOutParam->nParam1 = pSmsMOInfo->sCfw_Event.nParam1;
    pOutParam->nParam2 = pSmsMOInfo->sCfw_Event.nParam2;
    pOutParam->nType   = pSmsMOInfo->sCfw_Event.nType;
    CSW_PROFILE_FUNCTION_EXIT(sms_Sync_Delete);
}

VOID sms_Sync_List(CFW_SMS_MO_INFO* pSmsMOInfo, CFW_SMS_OUT_PARAM* pOutParam)
{
    UINT32  ret=0x0;
    CFW_EV* pEvent=NULL;
    CFW_EV  sMeEvent;
    CSW_PROFILE_FUNCTION_ENTER(sms_Sync_List);
 
    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
    if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)  || \
       (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_BM)  || \
       (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME_SR))
    {
        if(pSmsMOInfo->nListCount != 0)
        {
            UINT8* pListOutData=NULL; 
            for(pSmsMOInfo->i=0;pSmsMOInfo->i<pSmsMOInfo->nListCount;pSmsMOInfo->i++)
            {
                UINT8* pPduDataWithRealSize=NULL; 
                UINT8  nStatus=0x0;
                UINT16 nPduRealSize=0x0; 
                UINT32 nTmp=0x0;
                CFW_SMS_MULTIPART_INFO sLongerMsg;

                //Zero memory
                SUL_ZeroMemory8(&sLongerMsg,SIZEOF(CFW_SMS_MULTIPART_INFO));
                if(pSmsMOInfo->i == 0)
                {
                    //This is a temp memory. just for CFW_MeListMessage.
                    pListOutData = (UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4);
                    if(!pListOutData)
                    {
		          CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_List pListOutData malloc error!!! \n ");
                        pSmsMOInfo->sCfw_Event.nFlag   = 1;
                        pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_MEMORY_FULL;
                        pSmsMOInfo->sCfw_Event.nParam2 = 0;
                        pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                        goto nFlagEvent;
                    }
                    SUL_ZeroMemory8(pListOutData, SMS_MO_ONE_PDU_SIZE+4+4);
                    sMeEvent.nParam1 = (UINT32)pListOutData;
                    pEvent = &sMeEvent;
                }
                ret = CFW_MeListMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nStatus,1,pSmsMOInfo->nListStartIndex,&sMeEvent);
                if((ret == ERR_SUCCESS) && ((pEvent->nParam2 & 0xffff0000) == 0))
                {
                    UINT32 nPduOutData=0x0;
//                                UINT32 nTime=0x0, nAppInt32=0x0;
//                                nTime      = (UINT32)(*((UINT8*)sMeEvent.nParam1 + SMS_MO_ONE_PDU_SIZE));
//                                nAppInt32  = (UINT32)(*((UINT8*)sMeEvent.nParam1 + SMS_MO_ONE_PDU_SIZE + 4));
                    //Just get the size and status of each PDU
                    ret = sms_tool_DecomposePDU((PVOID)pEvent->nParam1,8,&nPduOutData,&nStatus,NULL);
                    if(ret != ERR_SUCCESS)
                    {
                        CSW_SMS_FREE(pListOutData);
                        pSmsMOInfo->sCfw_Event.nFlag   = 1;
                        pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                        pSmsMOInfo->sCfw_Event.nParam2 = 0;
                        pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                        goto nFlagEvent;
                    }
                    nPduRealSize = ((CFW_SMS_PDU_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->nDataSize;
                    if(nPduRealSize > 200)
                        nPduRealSize = SMS_MO_ONE_PDU_SIZE;
                    CSW_SMS_FREE((UINT8*)nPduOutData);

                    //Then Get the sLongerMsg
                    if((nStatus == CFW_SMS_STORED_STATUS_UNREAD) || \
                        (nStatus == CFW_SMS_STORED_STATUS_READ) )
                    {
                        ret = sms_tool_DecomposePDU((PVOID)pEvent->nParam1,2,&nPduOutData,&nStatus,&sLongerMsg);
                        CSW_SMS_FREE((UINT8*)nPduOutData);
                    }//Submitted message.
                    else if((nStatus == CFW_SMS_STORED_STATUS_UNSENT)               || \
                            (nStatus == CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)      || \
                            (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || \
                            (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || \
                            (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE) )
                    {
                        //Text Mode, Submitted message. when  CFW_CfgSetSmsShowTextModeParam(1)
                        //Parse pEvent->nParam1 to CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO  
                        ret = sms_tool_DecomposePDU((PVOID)pEvent->nParam1,4,&nPduOutData,&nStatus,&sLongerMsg);
                        CSW_SMS_FREE((UINT8*)nPduOutData);
                    }
                    //Malloc the real size for a PDU, 1 means to add status byte
                    pPduDataWithRealSize = (UINT8*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_NODE) + nPduRealSize + 1);
                    if(!pPduDataWithRealSize)
                    {
		          CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_List pPduDataWithRealSize malloc error!!! \n ");
                        pSmsMOInfo->sCfw_Event.nFlag   = 1;
                        pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_MEMORY_FULL;
                        pSmsMOInfo->sCfw_Event.nParam2 = 0;
                        pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                        goto nFlagEvent;
                    }
                    SUL_ZeroMemory8(pPduDataWithRealSize, (nPduRealSize + SIZEOF(CFW_SMS_NODE) + 1));
                    //Just do one time
                    if(pSmsMOInfo->i == 0)
                    {
                        //     | 4 bytes  |  4 bytes  |  4 bytes  |  4 bytes  |  4 bytes  |  4 bytes  |...
                        //     |addr count|   addr1   |   addr2   |   addr3   |   addr4   |   addr5   |....
                        //
                        //     ^ pSmsMOInfo->nListAddr = (UINT32)pListAddr 
                        UINT32* pListAddr=NULL;
                        pListAddr = (UINT32*)CSW_SMS_MALLOC(4*pSmsMOInfo->nListCount + 4);
                        if(!pListAddr)
                        {
		              CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_List pListAddr malloc error!!! \n ");
                            CSW_SMS_FREE(pListOutData);
                            pSmsMOInfo->sCfw_Event.nFlag   = 1;
                            pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_MEMORY_FULL;
                            pSmsMOInfo->sCfw_Event.nParam2 = 0;
                            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                            goto nFlagEvent;
                        }
                        SUL_ZeroMemory8(pListAddr, (4*pSmsMOInfo->nListCount + 4));
                        pSmsMOInfo->nListAddr = (UINT32)pListAddr;
                    }

                    //Set addr(i) each time
                    *((UINT32*)pSmsMOInfo->nListAddr + pSmsMOInfo->i + 1) = (UINT32)pPduDataWithRealSize;
                }
                if(ret != ERR_SUCCESS)
                {
                    CSW_SMS_FREE(pListOutData);
                    pSmsMOInfo->sCfw_Event.nFlag   = 1;
                    pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                    pSmsMOInfo->sCfw_Event.nParam2 = 0;
                    pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                    goto nFlagEvent;
                }

                //(H16bit)pEvent->nParam2: 0 means continue to list; 1 means list over .
                //(L16bit)pEvent->nParam2: The index listed.
                //Continue to list
                if((pEvent->nParam2 & 0xffff0000) == 0)
                {
                    nTmp = (UINT32)pPduDataWithRealSize;
                    //Set value and copy data
                    if(sLongerMsg.count > 0) // a longer msg
                    {
                        UINT16 nConcatSmsPrevRec=0x0, nConcatSmsNextRec=0x0;
                        sms_tool_GetLongMSGPara((UINT8)pSmsMOInfo->nLocation,(UINT16)(pEvent->nParam2 & 0x0000ffff), &nConcatSmsPrevRec, &nConcatSmsNextRec);
                        ((CFW_SMS_NODE*)nTmp)->nConcatPrevIndex = nConcatSmsPrevRec;
                        ((CFW_SMS_NODE*)nTmp)->nConcatNextIndex = nConcatSmsNextRec;
                    }
                    else // a normal short message
                    {
                        ((CFW_SMS_NODE*)nTmp)->nConcatPrevIndex  =   0xffff;
                    }
                    //Set value
                    ((CFW_SMS_NODE*)nTmp)->nConcatCurrentIndex   =  (UINT16)(pEvent->nParam2 & 0x0000ffff);
                    ((CFW_SMS_NODE*)nTmp)->nStatus  =   nStatus;
                    ((CFW_SMS_NODE*)nTmp)->nStorage =   pSmsMOInfo->nLocation;
                    //Pointer to 
                    ((CFW_SMS_NODE*)nTmp)->pNode    =  (UINT8*)((UINT8*)nTmp + SIZEOF(CFW_SMS_NODE));
                    //copy the pdu with (real size +1), including status byte
                    SUL_MemCopy8((UINT8*)((CFW_SMS_NODE*)nTmp)->pNode, (UINT8*)((UINT8*)pEvent->nParam1),(nPduRealSize+1));

                    pSmsMOInfo->nListStartIndex = (UINT16)((pEvent->nParam2 & 0x0000ffff) + 1);
                    //Get the size of list PDU(add total)
                    pSmsMOInfo->nListSize = (UINT16)(pSmsMOInfo->nListSize + nPduRealSize + SIZEOF(CFW_SMS_NODE) + 1);
                }
                //got the last one
                if( ((pEvent->nParam2 & 0xffff0000) == 0x00010000) ||  \
                    (((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount-1))) )
                {
                    //if list over and the comment is null
                    if((pSmsMOInfo->i == 0) && ( *((UINT8*)pEvent->nParam1) == 0))
                    {
                        pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_OPERATION_NOT_ALLOWED;
                        pSmsMOInfo->sCfw_Event.nParam2 = 0;
                        pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                        CSW_SMS_FREE((UINT32*)pSmsMOInfo->nListAddr);
                    }
                    else
                    {
                        //Set addr count
                        if((pEvent->nParam2 & 0xffff0000) == 0x00010000)
                            *((UINT32*)pSmsMOInfo->nListAddr) = (UINT32)(pSmsMOInfo->i);
                        if(((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount-1))) 
                            *((UINT32*)pSmsMOInfo->nListAddr) = (UINT32)(pSmsMOInfo->i + 1);
                        pEvent->nParam2 = (UINT32)(*((UINT32*)pSmsMOInfo->nListAddr)<<16) + (pSmsMOInfo->nListStartIndex-1);
                        pSmsMOInfo->sCfw_Event.nParam1 = pSmsMOInfo->nListAddr;
                        pSmsMOInfo->sCfw_Event.nParam2 = pEvent->nParam2;
                        pSmsMOInfo->sCfw_Event.nType   = pEvent->nType;
                    }
                    if(pListOutData)
                        CSW_SMS_FREE(pListOutData);
                    pSmsMOInfo->sCfw_Event.nFlag   = 1;
                    goto nFlagEvent;
                }
            }//End for(pSmsMOInfo->i=0;pSmsMOInfo->i<pSmsMOInfo->nListCount;pSmsMOInfo->i++)
        }
        else//(pSmsMOInfo->nListCount == 0)
        {
            ret = CFW_MeListMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nStatus,0,pSmsMOInfo->nListStartIndex,&sMeEvent);
            if(ret == ERR_SUCCESS)
            {
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = 0;
                pSmsMOInfo->sCfw_Event.nParam2 = sMeEvent.nParam2;
                pSmsMOInfo->sCfw_Event.nType   = sMeEvent.nType;
                goto nFlagEvent;
            }
            else 
            {
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                pSmsMOInfo->sCfw_Event.nParam2 = 0;
                pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                goto nFlagEvent;
            }
        }
    }
    
nFlagEvent:
    //Wait for the event result
    pOutParam->nParam1 = pSmsMOInfo->sCfw_Event.nParam1;
    pOutParam->nParam2 = pSmsMOInfo->sCfw_Event.nParam2;
    pOutParam->nType   = pSmsMOInfo->sCfw_Event.nType;
    CSW_PROFILE_FUNCTION_EXIT(sms_Sync_List);
}

VOID sms_Sync_ListHeader(CFW_SMS_MO_INFO* pSmsMOInfo, CFW_SMS_OUT_PARAM* pOutParam)
{
    UINT32  ret=0x0;
    CFW_EV* pEvent=NULL;
    CFW_EV  sMeEvent;
    CSW_PROFILE_FUNCTION_ENTER(sms_Sync_ListHeader);
 
    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));

    if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)  || \
       (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_BM)  || \
       (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME_SR))
    {
        if(pSmsMOInfo->nListCount != 0)
        {
            UINT8* pListOutData=NULL; 
            for(pSmsMOInfo->i=0;pSmsMOInfo->i<pSmsMOInfo->nListCount;pSmsMOInfo->i++)
            {
                CFW_SMS_HEADER* pSmsHeader=NULL; 

                if(pSmsMOInfo->i == 0)
                {
                    //This is a temp memory. just for CFW_MeListMessage.
                    pListOutData = (UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4);
                    if(!pListOutData)
                    {
	                 CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_ListHeader pListOutData malloc error!!! \n ");
                        pSmsMOInfo->sCfw_Event.nFlag   = 1;
                        pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_MEMORY_FULL;
                        pSmsMOInfo->sCfw_Event.nParam2 = 0;
                        pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                        goto nFlagEvent;
                    }
                    SUL_ZeroMemory8(pListOutData, SMS_MO_ONE_PDU_SIZE+4+4);
                    sMeEvent.nParam1 = (UINT32)pListOutData;
                    pEvent = &sMeEvent;
                }
                ret = CFW_MeListMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nStatus,1,pSmsMOInfo->nListStartIndex,&sMeEvent);
                if((ret == ERR_SUCCESS) && ((pEvent->nParam2 & 0xffff0000) == 0))
                {
    //                                UINT32 nTime=0x0, nAppInt32=0x0;

    //                                nTime      = (UINT32)(*((UINT8*)pEvent->nParam1 + SMS_MO_ONE_PDU_SIZE));
    //                                nAppInt32  = (UINT32)(*((UINT8*)pEvent->nParam1 + SMS_MO_ONE_PDU_SIZE + 4));
                    //Just do one time
                    if(pSmsMOInfo->i == 0)
                    {
                        UINT32* pListAddr=NULL;
                        pListAddr = (UINT32*)CSW_SMS_MALLOC(pSmsMOInfo->nListCount*SIZEOF(CFW_SMS_HEADER));
                        if(!pListAddr)
                        {
	                     CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_ListHeader pListAddr malloc error!!! \n ");
                            CSW_SMS_FREE(pListOutData);
                            pSmsMOInfo->sCfw_Event.nFlag   = 1;
                            pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_MEMORY_FULL;
                            pSmsMOInfo->sCfw_Event.nParam2 = 0;
                            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                            goto nFlagEvent;
                        }
                        SUL_ZeroMemory8(pListAddr, (pSmsMOInfo->nListCount*SIZEOF(CFW_SMS_HEADER)));
                        pSmsMOInfo->nListAddr = (UINT32)pListAddr;
                    }
                    pSmsHeader = (CFW_SMS_HEADER*)((UINT8*)pSmsMOInfo->nListAddr + pSmsMOInfo->i*SIZEOF(CFW_SMS_HEADER));
                }
                if(ret != ERR_SUCCESS)
                {
                    CSW_SMS_FREE(pListOutData);
                    pSmsMOInfo->sCfw_Event.nFlag   = 1;
                    pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                    pSmsMOInfo->sCfw_Event.nParam2 = 0;
                    pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                    goto nFlagEvent;
                }

                //(H16bit)pEvent->nParam2: 0 means continue to list; 1 means list over .
                //(L16bit)pEvent->nParam2: The index listed.
                //Continue to list
                if((pEvent->nParam2 & 0xffff0000) == 0)
                {
                    UINT8   nStatus=0x0;
                    UINT32  nPduOutData=0x0;
                    CFW_SMS_MULTIPART_INFO sLongerMsg;

                    //Zero memory
                    SUL_ZeroMemory8(&sLongerMsg,SIZEOF(CFW_SMS_MULTIPART_INFO));
                    //Just get SMS status
                    sms_tool_DecomposePDU(pListOutData,0,NULL,&nStatus,NULL);
                    if((nStatus == CFW_SMS_STORED_STATUS_UNREAD) || \
                        (nStatus == CFW_SMS_STORED_STATUS_READ) )
                    {
                        ret = sms_tool_DecomposePDU(pListOutData,2,&nPduOutData,&nStatus,&sLongerMsg);
                        if(ret == ERR_SUCCESS)
                        {
                            SUL_MemCopy8(&(pSmsHeader->nTime),&(((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->scts),SIZEOF(TM_SMS_TIME_STAMP));
                            SUL_MemCopy8(pSmsHeader->pTelNumber,((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->oa,TEL_NUMBER_MAX_LEN);
                            pSmsHeader->nTelNumberType = ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->tooa;
                        }
                        CSW_SMS_FREE((UINT8*)nPduOutData);
                    }//Submitted message.
                    else if((nStatus == CFW_SMS_STORED_STATUS_UNSENT)               || \
                            (nStatus == CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)      || \
                            (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || \
                            (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || \
                            (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE) )
                    {
                        //Text Mode, Submitted message. when  CFW_CfgSetSmsShowTextModeParam(1)
                        //Parse pEvent->nParam1 to CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO  
                        ret = sms_tool_DecomposePDU(pListOutData,4,&nPduOutData,&nStatus,&sLongerMsg);
                        if(ret == ERR_SUCCESS)
                        {
                            SUL_MemCopy8(pSmsHeader->pTelNumber,((CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->da,TEL_NUMBER_MAX_LEN);
                            pSmsHeader->nTelNumberType = ((CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->toda;
                        }
                        CSW_SMS_FREE((UINT8*)nPduOutData);
                    }
                    //Set value and copy data
                    if(sLongerMsg.count > 0) // a longer msg
                    {
                        UINT16 nConcatSmsPrevRec=0x0, nConcatSmsNextRec=0x0;
                        sms_tool_GetLongMSGPara((UINT8)pSmsMOInfo->nLocation,(UINT16)(pEvent->nParam2 & 0x0000ffff), &nConcatSmsPrevRec, &nConcatSmsNextRec);
                        pSmsHeader->nConcatPrevIndex  =   nConcatSmsPrevRec;
                        pSmsHeader->nConcatNextIndex  =   nConcatSmsNextRec;
                    }
                    else // a normal short message
                    {
                        pSmsHeader->nConcatPrevIndex  =   0xffff;
                    }
                    //Set value
                    pSmsHeader->nConcatCurrentIndex   =  (UINT16)(pEvent->nParam2 & 0x0000ffff);
                    pSmsHeader->nStatus  =   nStatus;
                    pSmsHeader->nStorage =   pSmsMOInfo->nLocation;
                    pSmsMOInfo->nListStartIndex = (UINT16)((pEvent->nParam2 & 0x0000ffff) + 1);
                }
                //got the last one
                if(((pEvent->nParam2 & 0xffff0000) == 0x00010000) ||  \
                         (((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount-1))) )
                {
                    //if list over and the comment is null
                    if((pSmsMOInfo->i == 0) && ( *((UINT8*)pEvent->nParam1) == 0))
                    {
                        CSW_SMS_FREE((UINT32*)pSmsMOInfo->nListAddr);
                        pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_OPERATION_NOT_ALLOWED;
                        pSmsMOInfo->sCfw_Event.nParam2 = 0;
                        pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                    }
                    else
                    {
                        //Set addr count
                        UINT16 nArrayCount=0x0;
                        if((pEvent->nParam2 & 0xffff0000) == 0x00010000)
                            nArrayCount = pSmsMOInfo->i;
                        if(((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount-1))) 
                            nArrayCount = (UINT16)(pSmsMOInfo->i + 1);
                        pEvent->nParam2 = (UINT32)((nArrayCount<<16) + (pSmsMOInfo->nListStartIndex-1));
                        pSmsMOInfo->sCfw_Event.nParam1 = pSmsMOInfo->nListAddr;
                        pSmsMOInfo->sCfw_Event.nParam2 = pEvent->nParam2;
                        pSmsMOInfo->sCfw_Event.nType   = pEvent->nType;
                    }
                    if(pListOutData)
                        CSW_SMS_FREE(pListOutData);
                    pSmsMOInfo->sCfw_Event.nFlag   = 1;
                    goto nFlagEvent;
                }
            }//End for(pSmsMOInfo->i=0;pSmsMOInfo->i<pSmsMOInfo->nListCount;pSmsMOInfo->i++)
        }
        else//(pSmsMOInfo->nListCount == 0)
        {
            ret = CFW_MeListMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nStatus,0,pSmsMOInfo->nListStartIndex,&sMeEvent);

            if(ret == ERR_SUCCESS)
            {
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = 0;
                pSmsMOInfo->sCfw_Event.nParam2 = sMeEvent.nParam2;
                pSmsMOInfo->sCfw_Event.nType   = sMeEvent.nType;
                goto nFlagEvent;
            }
            else 
            {
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                pSmsMOInfo->sCfw_Event.nParam2 = 0;
                pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                goto nFlagEvent;
            }
        }
    }

nFlagEvent:
    //Wait for the event result
    pOutParam->nParam1 = pSmsMOInfo->sCfw_Event.nParam1;
    pOutParam->nParam2 = pSmsMOInfo->sCfw_Event.nParam2;
    pOutParam->nType   = pSmsMOInfo->sCfw_Event.nType;
    CSW_PROFILE_FUNCTION_EXIT(sms_Sync_ListHeader);
}

VOID sms_Sync_Write(CFW_SMS_MO_INFO* pSmsMOInfo, CFW_SMS_OUT_PARAM* pOutParam)
{
    UINT32  ret=0x0;
    CFW_EV* pEvent=NULL;
    CFW_EV  sMeEvent,sComposeEvent;
    CSW_PROFILE_FUNCTION_ENTER(sms_Sync_Write);
  
    //Zero memory
    SUL_ZeroMemory8(&sComposeEvent,SIZEOF(CFW_EV));
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));

    if(pSmsMOInfo->nFormat)  //Write Text
    {
        if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) ||  \
           (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME) ||  \
           (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_ME) )
        {
            UINT8 nConcat=0x0;
            //should be deliver msg
            if((pSmsMOInfo->nStatus & CFW_SMS_STORED_STATUS_UNREAD) || \
               (pSmsMOInfo->nStatus & CFW_SMS_STORED_STATUS_READ))
            {
                nConcat =0x08;// 0000 1000   bit3=1:for write, bit0=0,bit1=0:Deliver
            } //should be submit msg
            else
            {
                nConcat =0x09; // 0000 1001  bit3=1:for write, bit0=1,bit1=0:Submit
            }
            ret = sms_tool_ComposePDU(nConcat,pSmsMOInfo->nStatus, pSmsMOInfo->pData,pSmsMOInfo->nDataSize,\
                                  &pSmsMOInfo->sNumber,&sComposeEvent);
        }
        else  //Write Status Report or Broadcast Message
        {
            sComposeEvent.nParam1 = (UINT32)(pSmsMOInfo->pData);
            sComposeEvent.nType   = 0x0;
            ret = ERR_SUCCESS;
        }
    
        if(ret == ERR_SUCCESS )
        {
            pSmsMOInfo->pTmpData_Addr = (UINT8*)(sComposeEvent.nParam1);
            pSmsMOInfo->pTmpPara_Addr = (UINT8*)(sComposeEvent.nParam2);
            if(sComposeEvent.nType == 0x0) //for a normal SMS
            {
                pSmsMOInfo->isLongerMsg = 0;  // a normal msg or the last part of a longer msg
                //To SIM
                if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)  ||  \
                   (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_SR))
                {
    //                ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)(sComposeEvent.nParam1),SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
    //                //Free the memory located by CFW_ComposePDU_ASYN
    //                if(ret == ERR_SUCCESS )
    //                    CFW_SetAoState(pSmsMOInfo->hAo, CFW_SM_STATE_WAIT);
    //                else
    //                    goto flag_fail;
    //                break;
                }
                //To ME
                else if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)     || \
                        (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_BM)     || \
                        (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME_SR)  || \
                        (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_ME))
                {
                    UINT32 nTime=0x0, nAppInt32=0x0;
                    UINT8  nFlag_Storage_SM_ME=0x0;
                
                    if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_ME)
                    {
                        nFlag_Storage_SM_ME = 1;
                        pSmsMOInfo->nLocation = CFW_SMS_STORAGE_ME;
                    }
                    ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)(sComposeEvent.nParam1),SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
                    if(ret == ERR_SUCCESS)
                    {
                        if(0 == sMeEvent.nType)
                        {
                            //Free the memory located by CFW_ComposePDU_ASYN
                            //CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
                            //Go to CFW_SM_STATE_WAIT directly!
//                            CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                            pEvent = &sMeEvent;
                            pEvent->nTransId  = pSmsMOInfo->nUTI_Internal;
                        }
                        else if(0xf0 == sMeEvent.nType)
                        {
                            if(1 == nFlag_Storage_SM_ME)
                            {
                                pSmsMOInfo->nLocation = CFW_SMS_STORAGE_SM;
                                ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)(sComposeEvent.nParam1),SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
                                //Free the memory located by CFW_ComposePDU_ASYN
                                //CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
                                if(ret == ERR_SUCCESS )
                                    CFW_SetAoState(pSmsMOInfo->hAo, CFW_SM_STATE_WAIT);
                                else
                                    goto flag_fail;
                                //CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
//                                break;
                            }
                            else
                                goto flag_fail;
                        }
                    }
                    else
                    {
                        goto flag_fail;
                    }
                }
            }
            else if(sComposeEvent.nType == 0x01) //for a longer SMS
            {
                ret=0x0;
                CFW_SMS_STORAGE_INFO  sStorageInfo;
                //sComposeEvent
                //pEvent->nParam1 指向一个176byte固定结构
                //pEvent->nParam2 为一个32位数组,each part is 8 bit,as following
                //   _____________________________________
                //  |        |        |        |         |
                //  | part A | part B | part C | part D  |
                //  |________|________|________|_________|
                // A : Current index
                // B : Total count
                // C : MR
                // D : 结构 Size 
                //Point pEvent->nParam1/2 to MT handle
                pSmsMOInfo->pTmpData      = (UINT8*)(sComposeEvent.nParam1);
                pSmsMOInfo->pTmpPara      = (UINT8*)(sComposeEvent.nParam2);
                pSmsMOInfo->pTmpData_Addr = (UINT8*)(sComposeEvent.nParam1);
                pSmsMOInfo->pTmpPara_Addr = (UINT8*)(sComposeEvent.nParam2);
                //Set value
                pSmsMOInfo->isLongerMsg         = 1;  // a longer msg 
                pSmsMOInfo->nLongerMsgCurrent   = *(pSmsMOInfo->pTmpPara + 3);  //part A
                pSmsMOInfo->nLongerMsgCount     = *(pSmsMOInfo->pTmpPara + 2);  //part B
                pSmsMOInfo->nMR                 = *(pSmsMOInfo->pTmpPara + 1);  //part C
                pSmsMOInfo->nLongerEachPartSize = *(pSmsMOInfo->pTmpPara);      //part D
            
                pSmsMOInfo->isLongerMsg = 1; //a longer msg
                //Verify storage
                if((pSmsMOInfo->nLocation != CFW_SMS_STORAGE_ME)       && \
                   (pSmsMOInfo->nLocation != CFW_SMS_STORAGE_SM)       && \
                   (pSmsMOInfo->nLocation != CFW_SMS_STORAGE_SM_ME))
                {
                    goto flag_fail;
                }
                //if add new, verify there is free slot or not                                   
                if(pSmsMOInfo->nIndex == 0) //Add new
                {
                    if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_ME)
                    {
                        pSmsMOInfo->nLocation = CFW_SMS_STORAGE_ME;
                        //Verify the free slot in ME firstly
                        SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
                        CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                        if(pSmsMOInfo->nLongerMsgCount > (sStorageInfo.totalSlot - sStorageInfo.usedSlot))
                        {
                            pSmsMOInfo->nLocation = CFW_SMS_STORAGE_SM;
                            //if ME is not OK, then verify the free slot in ME firstly
                            SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
                            CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                            if(pSmsMOInfo->nLongerMsgCount > (sStorageInfo.totalSlot - sStorageInfo.usedSlot))
                            {
                                CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
                                CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
                                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                                pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_D0_SIM_SMS_STORAGE_FULL;
                                pSmsMOInfo->sCfw_Event.nParam2 = 0;
                                pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                                goto nFlagEvent;
                            }
                        }     
                    }
                    else if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME) || \
                            (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) )
                    {
                        //Verify the free slot
                        SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
                        CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                        if(pSmsMOInfo->nLongerMsgCount > (sStorageInfo.totalSlot - sStorageInfo.usedSlot))
                        {
                            CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
                            CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
                            pSmsMOInfo->sCfw_Event.nFlag   = 1;
                            pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_D0_SIM_SMS_STORAGE_FULL;
                            pSmsMOInfo->sCfw_Event.nParam2 = 0;
                            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                            goto nFlagEvent;
                        }     
                    }
                    else
                    {
                        goto flag_fail;
                    }
                }// end of if(pSmsMOInfo->nIndex == 0)

                //Write long msg to ME
                if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)//To ME
                {
                    UINT32 nTime=0x0, nAppInt32=0x0;
                    UINT8 i=0x0;
                    for(i=0;i<pSmsMOInfo->nLongerMsgCount;i++)
                    {
                        ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->pTmpData,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
                        if(0 == sMeEvent.nType) //Write OK
                        {
                            if(pSmsMOInfo->nIndex == 0)//Write a new long sms
                            {
                                //CFW_SMS_STORAGE_INFO  sStorageInfo;
                                CFW_SMS_LONG_INFO*    pTmpSmsLongInfo=NULL;   
                                //use nListStartIndex to save the first index
                                if(1 == pSmsMOInfo->nLongerMsgCurrent)
                                    pSmsMOInfo->nListStartIndex = (UINT16)sMeEvent.nParam1;
                                //Update storage info
                                SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
                                CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                                if(sStorageInfo.usedSlot < sStorageInfo.totalSlot)
                                    sStorageInfo.usedSlot++;
                                if(CFW_SMS_STORED_STATUS_UNREAD == pSmsMOInfo->nStatus)
                                    sStorageInfo.unReadRecords++;
                                else if(CFW_SMS_STORED_STATUS_READ == pSmsMOInfo->nStatus)
                                    sStorageInfo.readRecords++;
                                else if(CFW_SMS_STORED_STATUS_UNSENT == pSmsMOInfo->nStatus)
                                    sStorageInfo.unsentRecords++;
                                else if((CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ == pSmsMOInfo->nStatus)        || \
                                        (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV == pSmsMOInfo->nStatus)   || \
                                        (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE == pSmsMOInfo->nStatus)  || \
                                        (CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE == pSmsMOInfo->nStatus))
                                    sStorageInfo.sentRecords++;
                                CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                                //Malloc CFW_SMS_LONG_INFO for pStru_SmsLongInfo
                                pTmpSmsLongInfo = (CFW_SMS_LONG_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_LONG_INFO));
                                if(!pTmpSmsLongInfo)
                                {
		                      CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_Write pTmpSmsLongInfo malloc error!!! \n ");
                                    goto flag_fail;
                                }
                                SUL_ZeroMemory8(pTmpSmsLongInfo, (SIZEOF(CFW_SMS_LONG_INFO)));
                                pTmpSmsLongInfo->count   = pSmsMOInfo->nLongerMsgCount;
                                pTmpSmsLongInfo->current = pSmsMOInfo->nLongerMsgCurrent;
                                pTmpSmsLongInfo->id      = pSmsMOInfo->nMR;
                                pTmpSmsLongInfo->nStatus = pSmsMOInfo->nStatus;
                                pTmpSmsLongInfo->index   = (UINT16)(sMeEvent.nParam1);
                                pTmpSmsLongInfo->pNext   = NULL;
                                //Conncet the struct to the link
                                if(pSmsMOInfo->pStru_SmsLongInfo)
                                {
                                    CFW_SMS_LONG_INFO *pLink=NULL;
                                    pLink = pSmsMOInfo->pStru_SmsLongInfo;
                                    while(pLink->pNext)
                                    {
                                        pLink = (CFW_SMS_LONG_INFO*)(pLink->pNext);
                                    }
                                    pLink->pNext = (UINT32*)pTmpSmsLongInfo;
                                }
                                else // for writing the first one
                                    pSmsMOInfo->pStru_SmsLongInfo = pTmpSmsLongInfo;

                                if(pSmsMOInfo->nLongerMsgCurrent < pSmsMOInfo->nLongerMsgCount)
                                {
                                    //offset
                                    pSmsMOInfo->pTmpPara += 4;
                                    pSmsMOInfo->pTmpData += pSmsMOInfo->nLongerEachPartSize;
                                    //Set value
                                    pSmsMOInfo->nLongerMsgCurrent   = *(pSmsMOInfo->pTmpPara + 3);  //part A
                                    pSmsMOInfo->nLongerEachPartSize = *(pSmsMOInfo->pTmpPara );     //part D
                                }
                            }
                            else //Update an existed long msg,  
                            {
                                //1.将原来的超长短信删掉
                                //2.按pSmsMOInfo->nIndex保存超长第一条
                                //3.保存其他
                                //4.更新pMEFilename
                                goto flag_fail;
                            }
                        }
                        else //Write Fail
                        {
                            goto flag_fail;
                        }
                    }//emd of for(i=0;i<pSmsMOInfo->nLongerMsgCount;i++)
                    if(pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)
                    {
#if USING_FS_SAVE_LONG_PARA                        
                        UINT32 nWriteBuf=0x0;
                        ret = sms_tool_Parse_LongMSGPara(pSmsMOInfo->pStru_SmsLongInfo,pSmsMOInfo->nLongerMsgCount,&nWriteBuf);
                        if(ret != ERR_SUCCESS)
                        {
                            goto flag_fail;
                        }
                        ret = sms_tool_SetLongMSGPara(CFW_SMS_STORAGE_ME,(UINT8*)nWriteBuf,(INT32)(pSmsMOInfo->nLongerMsgCount * SIZEOF(CFW_SMS_LONG_INFO_NODE)),LONG_SMS_PARA_ADD);
                        CSW_SMS_FREE((UINT32*)nWriteBuf);
                        if(ret != ERR_SUCCESS)
                        {
                            goto flag_fail;
                        }
                        //Go to CFW_SM_STATE_WAIT directly!
#endif
                        pEvent = &sMeEvent;
                        pEvent->nTransId  = pSmsMOInfo->nUTI_Internal;
                    }//end of if(pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)
                } //end of if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)  
            
                //Write long msg to SIM ,Save the first part of a longer msg
                if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) //To SIM
                {
                    if(pSmsMOInfo->nIndex == 0)//Write a new long sms
                    {
                        ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->pTmpData,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
                        if(ret == ERR_SUCCESS)
                        {
//                            CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
//                            break;
                        }
                        else
                            goto flag_fail;
                    }
                    else//Update an existed long msg, 
                    {
                        //1.将原来的超长短信删掉
                        //2.按pSmsMOInfo->nIndex保存超长第一条
                        //3.保存其他
                        //4.更新pMEFilename
                        goto flag_fail;
                    }
                }//end of if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) //To SIM
            }
            else if(sComposeEvent.nType == 0xf0) //compose error
            {
                goto flag_fail;
            }
//            CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
        }
        else
        {
            goto flag_fail;
        }
    }
    else //Write PDU
    {
        UINT8* pWritePDU=NULL;
        UINT8  nTP_MTI=0x0,nSCA_Length=0x0;
        if(!pSmsMOInfo->isLongerMsg)//a normal PDU sms
        {
            nSCA_Length = *pSmsMOInfo->pData;
            nTP_MTI =  *(pSmsMOInfo->pData+ nSCA_Length + 1);
            if(((nTP_MTI) & 0x03) == 0x00) //SMS DELIVER REPORT or DELIVER 
            {
                pSmsMOInfo->nMR = 0x88;// in fact , there is no MR for deliver
                if((pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_UNREAD)  && \
                   (pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_UNREAD))
                {
                    pSmsMOInfo->sCfw_Event.nFlag   = 1;
                    pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                    pSmsMOInfo->sCfw_Event.nParam2 = 0;
                    pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                    goto nFlagEvent;
                }
            }
            else if(((nTP_MTI) & 0x03) == 0x01)//SMS SUBMIT or SUBMIT REPORT
            {
                //Get MR
                pSmsMOInfo->nMR  = *(pSmsMOInfo->pData+nSCA_Length+2);// Get MR from the PDU data
                if((pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_UNSENT)                && \
                   (pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)       && \
                   (pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV)  && \
                   (pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE) && \
                   (pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE))
                {
                    pSmsMOInfo->sCfw_Event.nFlag   = 1;
                    pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                    pSmsMOInfo->sCfw_Event.nParam2 = 0;
                    pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                    goto nFlagEvent;
                }
            }
            else if(((nTP_MTI) & 0x03) == 0x10)//STATUS REPORT or SMS COMMAND
            {
                pSmsMOInfo->nMR = 0x88;// in fact , there is no MR for STATUS REPORT or SMS COMMAND
            }
            //Malloc
            pWritePDU = (UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE);
            if(!pWritePDU)
            {
                CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_Write pWritePDU malloc error!!! \n ");
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_MEMORY_FULL;
                pSmsMOInfo->sCfw_Event.nParam2 = 0;
                pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                goto nFlagEvent;
            }
            SUL_ZeroMemory8(pWritePDU, SMS_MO_ONE_PDU_SIZE);
            //Set the status
            *pWritePDU = pSmsMOInfo->nStatus;
            //Copy data
            SUL_MemCopy8((UINT8*)(pWritePDU+1),(UINT8*)pSmsMOInfo->pData,pSmsMOInfo->nDataSize);
            if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_ME)
            {
                pSmsMOInfo->nLocation = CFW_SMS_STORAGE_ME;
            }
            //To SIM
            if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)  || \
               (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_SR))
            { 
//                    ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pWritePDU,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
//                    if(ret == ERR_SUCCESS)
//                    {
//                        CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
//                    }
//                    else
//                    {
//                        CFW_GetUTI(hAo, &nGetUti);
//                        CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
//                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
//                        CSW_SMS_FREE(pWritePDU);
//                        return ERR_SUCCESS;
//                    }
//                    break;
            }
            //To ME
            else if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)  || \
                    (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_BM)  || \
                    (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME_SR))
            {
                UINT32 nTime=0x0, nAppInt32=0x0;
                ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pWritePDU,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
                if(ret == ERR_SUCCESS)
                {
                    pEvent = &sMeEvent;
                    pEvent->nTransId  = pSmsMOInfo->nUTI_Internal;
                }
                else
                {
                    CSW_SMS_FREE(pWritePDU);
                    pSmsMOInfo->sCfw_Event.nFlag   = 1;
                    pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                    pSmsMOInfo->sCfw_Event.nParam2 = 0;
                    pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                    goto nFlagEvent;
                }
            }
            else//Other storage
            {
                CSW_SMS_FREE(pWritePDU);
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = pEvent->nParam1;
                pSmsMOInfo->sCfw_Event.nParam2 = pEvent->nParam2;
                pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                goto nFlagEvent;
            }
            //Free pWritePDUSize
            CSW_SMS_FREE(pWritePDU);
        }
        else // Write a longer PDU msg
        {
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
            pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
            goto nFlagEvent;
        }
    }//end  Write PDU
    
    if(((pEvent->nEventId == EV_CFW_SIM_WRITE_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_WRITE_MESSAGE_RSP)) && \
       (pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
    {
        if(pEvent->nType == 0)
        {
            if(pSmsMOInfo->isLongerMsg)// a long sms
            {
                if(CFW_SMS_STORAGE_ME == pSmsMOInfo->nLocation)
                {
                    // the last part of a long msg
                    CFW_SMS_PARAMETER  sInfo;
                    SUL_ZeroMemory8(&sInfo, SIZEOF(CFW_SMS_PARAMETER));
                    //Verify MR
                    if(pSmsMOInfo->nMR == 0xff)
                        pSmsMOInfo->nMR = 0x01;
                    else
                        pSmsMOInfo->nMR++;
                    //Get MR
                    CFW_CfgGetDefaultSmsParam(&sInfo);
                    sInfo.mr = pSmsMOInfo->nMR;
                    CFW_CfgSetDefaultSmsParam(&sInfo);
                    pEvent->nParam1 = (UINT32)(pSmsMOInfo->nListStartIndex + (pSmsMOInfo->nLocation << 16)); 
                }
                else if(CFW_SMS_STORAGE_SM == pSmsMOInfo->nLocation)
                {
                    CFW_SMS_STORAGE_INFO  sStorageInfo;
                    CFW_SMS_LONG_INFO*    pTmpSmsLongInfo=NULL;   
                    ret=0x0;
                    //use nListStartIndex to save the first index
                    if(1 == pSmsMOInfo->nLongerMsgCurrent)
                        pSmsMOInfo->nListStartIndex = (UINT16)pEvent->nParam1;
                    //Update storage info
                    SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
                    CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                    if(sStorageInfo.usedSlot < sStorageInfo.totalSlot)
                        sStorageInfo.usedSlot++;
                    if(CFW_SMS_STORED_STATUS_UNREAD == pSmsMOInfo->nStatus)
                        sStorageInfo.unReadRecords++;
                    else if(CFW_SMS_STORED_STATUS_READ == pSmsMOInfo->nStatus)
                        sStorageInfo.readRecords++;
                    else if(CFW_SMS_STORED_STATUS_UNSENT == pSmsMOInfo->nStatus)
                        sStorageInfo.unsentRecords++;
                    else if((CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ == pSmsMOInfo->nStatus)        || \
                            (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV == pSmsMOInfo->nStatus)   || \
                            (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE == pSmsMOInfo->nStatus)  || \
                            (CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE == pSmsMOInfo->nStatus))
                        sStorageInfo.sentRecords++;
                    CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                    //Malloc CFW_SMS_LONG_INFO for pStru_SmsLongInfo
                    pTmpSmsLongInfo = (CFW_SMS_LONG_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_LONG_INFO));
                    if(!pTmpSmsLongInfo)
                    {
	                 CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_Write pTmpSmsLongInfo malloc error!!! \n ");
                        goto flag_fail;
                    }
                    SUL_ZeroMemory8(pTmpSmsLongInfo, (SIZEOF(CFW_SMS_LONG_INFO)));
                    pTmpSmsLongInfo->count   = pSmsMOInfo->nLongerMsgCount;
                    pTmpSmsLongInfo->current = pSmsMOInfo->nLongerMsgCurrent;
                    pTmpSmsLongInfo->id      = pSmsMOInfo->nMR;
                    pTmpSmsLongInfo->nStatus = pSmsMOInfo->nStatus;
                    pTmpSmsLongInfo->index   = (UINT16)(pEvent->nParam1);
                    pTmpSmsLongInfo->pNext   = NULL;
                    //Conncet the struct to the link
                    if(pSmsMOInfo->pStru_SmsLongInfo)
                    {
                        CFW_SMS_LONG_INFO *pLink=NULL;
                        pLink = pSmsMOInfo->pStru_SmsLongInfo;
                        while(pLink->pNext)
                        {
                            pLink = (CFW_SMS_LONG_INFO*)(pLink->pNext);
                        }
                        pLink->pNext = (UINT32*)pTmpSmsLongInfo;
                    }
                    else // for writing the first one
                        pSmsMOInfo->pStru_SmsLongInfo = pTmpSmsLongInfo;
                    //the last part of a long msg
                    if(pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)
                    {
                        CFW_SMS_PARAMETER  sInfo;
#if USING_FS_SAVE_LONG_PARA                        
                        UINT32             nWriteBuf=0x0;

                        SUL_ZeroMemory8(&sInfo, SIZEOF(CFW_SMS_PARAMETER));
                        ret = sms_tool_Parse_LongMSGPara(pSmsMOInfo->pStru_SmsLongInfo,pSmsMOInfo->nLongerMsgCount,&nWriteBuf);
                        if(ret != ERR_SUCCESS)
                            goto flag_fail;
                        ret = sms_tool_SetLongMSGPara(CFW_SMS_STORAGE_SM,(UINT8*)nWriteBuf,(INT32)(pSmsMOInfo->nLongerMsgCount * SIZEOF(CFW_SMS_LONG_INFO_NODE)),LONG_SMS_PARA_ADD);
                        CSW_SMS_FREE((UINT32*)nWriteBuf);
                        if(ret != ERR_SUCCESS)
                            goto flag_fail;
#endif                        
                        //Verify MR
                        if(pSmsMOInfo->nMR == 0xff)
                            pSmsMOInfo->nMR = 0x01;
                        else
                            pSmsMOInfo->nMR++;
                        //Get MR
                        CFW_CfgGetDefaultSmsParam(&sInfo);
                        sInfo.mr = pSmsMOInfo->nMR;
                        CFW_CfgSetDefaultSmsParam(&sInfo);
                        pEvent->nParam1 = (UINT32)(pSmsMOInfo->nListStartIndex + (pSmsMOInfo->nLocation << 16)); 
                    }
                    else//continue to write
                    {
                        //offset
                        pSmsMOInfo->pTmpPara += 4;
                        pSmsMOInfo->pTmpData += pSmsMOInfo->nLongerEachPartSize;
                        //Set value
                        pSmsMOInfo->nLongerMsgCurrent   = *(pSmsMOInfo->pTmpPara + 3);  //part A
                        pSmsMOInfo->nLongerEachPartSize = *(pSmsMOInfo->pTmpPara );     //part D
                        
                        if(pSmsMOInfo->nIndex == 0)//Write a new long sms
                        {
                            ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->pTmpData,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
                            if(ret == ERR_SUCCESS)
                            {
//                                CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
//                                break;
                            }
                            else
                                goto flag_fail;
                        }
                        else
                        {
                        }
                    }//end of if(pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)
                }
            }
            else //a normal sms
            {
                pEvent->nParam1 = pEvent->nParam1 + (pSmsMOInfo->nLocation << 16); 
                //Update CFW_SMS_STORAGE_INFO,  add a new msg
                if((pSmsMOInfo->nIndex == 0) && \
                   ((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) || (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)))
                {
                    CFW_SMS_STORAGE_INFO  sStorageInfo;
                    //Update storage info
                    SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
                    CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                    if(sStorageInfo.usedSlot < sStorageInfo.totalSlot)
                        sStorageInfo.usedSlot++;
                    if(CFW_SMS_STORED_STATUS_UNREAD == pSmsMOInfo->nStatus)
                        sStorageInfo.unReadRecords++;
                    else if(CFW_SMS_STORED_STATUS_READ == pSmsMOInfo->nStatus)
                        sStorageInfo.readRecords++;
                    else if(CFW_SMS_STORED_STATUS_UNSENT == pSmsMOInfo->nStatus)
                        sStorageInfo.unsentRecords++;
                    else if((CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ == pSmsMOInfo->nStatus)        || \
                            (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV == pSmsMOInfo->nStatus)   || \
                            (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE == pSmsMOInfo->nStatus)  || \
                            (CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE == pSmsMOInfo->nStatus))
                        sStorageInfo.sentRecords++;
                    CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                }
            }
        }
        else if(pEvent->nType == 0xf0)
        {
            if(pEvent->nParam1 == ERR_CME_ME_FAILURE)  //Me Return error
            {
                pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
            }
            else if(pEvent->nParam1 == ERR_CME_ME_SPACE_FULL)  //Me Return error
            {
                pEvent->nParam1 = ERR_CMS_D0_SIM_SMS_STORAGE_FULL;
            }
            else if(pEvent->nParam1 == ERR_CME_MEMORY_FULL)
            {
                pEvent->nParam1 = ERR_CMS_D0_SIM_SMS_STORAGE_FULL;
            }
            else if(pEvent->nParam1 == ERR_CME_INVALID_INDEX)
            {
                pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
            }
            else if(pEvent->nParam1 == ERR_CMS_INVALID_MEMORY_INDEX)
            {
                pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
            }
            else if(pEvent->nParam1 == ERR_CME_SIM_PIN_REQUIRED)
            {
                pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
            }
            else if(pEvent->nParam1 == ERR_CME_SIM_FAILURE)
            {
                pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
            }
            else if(pEvent->nParam1 == ERR_CME_SIM_NOT_INSERTED)
            {
                pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
            }
            else
            {
                pEvent->nParam1 = ERR_CMS_UNKNOWN_ERROR;
            }
            pEvent->nParam2 = pSmsMOInfo->nIndex;
        }
        else
        {
            pEvent->nParam1 = ERR_CMS_UNKNOWN_ERROR;
            pEvent->nParam2 = pSmsMOInfo->nIndex;
        }
    	//CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
        //if a normal msg or the last part of a longer msg;
        if((!pSmsMOInfo->isLongerMsg) || (pSmsMOInfo->isLongerMsg && (pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)) )
        {
            CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
            CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = pEvent->nParam1;
            pSmsMOInfo->sCfw_Event.nParam2 = pEvent->nParam2;
            pSmsMOInfo->sCfw_Event.nType   = pEvent->nType;
            goto nFlagEvent;
        }
    }

flag_fail:
    CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
    CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
    pSmsMOInfo->sCfw_Event.nFlag   = 1;
    pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
    pSmsMOInfo->sCfw_Event.nParam2 = 0;
    pSmsMOInfo->sCfw_Event.nType   = 0xf0;
    goto nFlagEvent;

nFlagEvent:
    //Wait for the event result
    pOutParam->nParam1 = pSmsMOInfo->sCfw_Event.nParam1;
    pOutParam->nParam2 = pSmsMOInfo->sCfw_Event.nParam2;
    pOutParam->nType   = pSmsMOInfo->sCfw_Event.nType;
    
    CSW_PROFILE_FUNCTION_EXIT(sms_Sync_Write);
}

VOID sms_Sync_SetUnRead2Read(CFW_SMS_MO_INFO* pSmsMOInfo, CFW_SMS_OUT_PARAM* pOutParam)
{
    UINT32  ret=0x0;
    CFW_EV* pEvent=NULL;
    CFW_EV  sMeEvent;
    UINT8   nSmsStatus=0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_Sync_SetUnRead2Read);
 
    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
    if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
    {
//        ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal);
//        if(ret == ERR_SUCCESS)
//        {
//            CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
//        }
//        else
//        {
//            CFW_GetUTI(hAo, &nGetUti);
//            CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
//            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
//            return ERR_CMS_UNKNOWN_ERROR;
//        }
//    break;
    }
    else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
    {
        //Malloc for the read data.
        sMeEvent.nParam1 = (UINT32)((UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4));
        if(!(UINT8*)sMeEvent.nParam1)
        {
            CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_SetUnRead2Read sMeEvent.nParam1 malloc error!!! \n ");
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_MEMORY_FULL;
            pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
            goto nFlagEvent;
        }
        SUL_ZeroMemory8((UINT8*)sMeEvent.nParam1,SMS_MO_ONE_PDU_SIZE+4+4);
        ret = CFW_MeReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,&sMeEvent);
        if(ret == ERR_SUCCESS)
        {
            pEvent = &sMeEvent;
            pEvent->nTransId = pSmsMOInfo->nUTI_Internal;
        }
        else  //other return value
        {
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
            pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
            goto nFlagEvent;
        }
    }
    if(((pEvent->nEventId == EV_CFW_SIM_READ_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)) && \
       (pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
    {
        if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1))
        {
            //Just get SMS status 
            sms_tool_DecomposePDU((PVOID)pEvent->nParam1,1,NULL,&nSmsStatus,NULL);
            if(nSmsStatus == CFW_SMS_STORED_STATUS_UNREAD)
            {
                *((UINT8*)pEvent->nParam1) = CFW_SMS_STORED_STATUS_READ;
                if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
                {
                    //Write it with CFW_SMS_STORED_STATUS_READ
                    ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
                }
                else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
                {
                    UINT32 nTime=0x0, nAppInt32=0x0,nTmp=0x0;
                    nTmp = sMeEvent.nParam1;
                    ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
                    CSW_SMS_FREE((UINT8*)(nTmp));
                }
                if(ret != ERR_SUCCESS)
                {
                    pSmsMOInfo->sCfw_Event.nFlag   = 1;
                    pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                    pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
                    pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                    goto nFlagEvent;
                }
                pEvent = &sMeEvent;
                pEvent->nTransId  = pSmsMOInfo->nUTI_Internal;
                pSmsMOInfo->nReadSmsStatus = CFW_SMS_STORED_STATUS_UNREAD;
            }
            else
            {
                if(pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)
                    CSW_SMS_FREE((UINT8*)(sMeEvent.nParam1));
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_OPERATION_NOT_ALLOWED;
                pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
                pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                goto nFlagEvent;
            }
            pEvent->nParam2 = 0;
        }
        else //read false
        {
            if(pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)
                CSW_SMS_FREE((UINT8*)(sMeEvent.nParam1));
            //Read a NULL SIM
            if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1 == 0))
            {
                pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
                pEvent->nType = 0xf0;
            }                    
            else if(pEvent->nType == 0xf0) 
            {
                if(pEvent->nParam1 == ERR_CME_ME_FAILURE)  //Me Return error
                {
                    pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
                }
                else if(pEvent->nParam1 == ERR_CME_INVALID_INDEX)//Me Return error
                {
                    pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
                }
                else if(pEvent->nParam1 == ERR_CMS_INVALID_MEMORY_INDEX)
                {
                    pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
                }
                else if(pEvent->nParam1 == ERR_CME_SIM_PIN_REQUIRED)
                {
                    pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
                }
                else if(pEvent->nParam1 == ERR_CME_SIM_FAILURE)
                {
                    pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
                }
                else if(pEvent->nParam1 == ERR_CME_SIM_NOT_INSERTED)
                {
                    pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
                }
                else  
                {
                    pEvent->nParam1 = ERR_CMS_UNKNOWN_ERROR;
                }
            }
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = pEvent->nParam1;
            pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
            pSmsMOInfo->sCfw_Event.nType   = pEvent->nType;
            goto nFlagEvent;
        }
    }
    if(((pEvent->nEventId == EV_CFW_SIM_WRITE_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_WRITE_MESSAGE_RSP)) && \
       (pSmsMOInfo->nUTI_Internal == pEvent->nTransId) )
    {
        if(pEvent->nType == 0)
        {
            CFW_SMS_STORAGE_INFO  sStorageInfo;

            //Zero memory
            SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
            CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
            if(sStorageInfo.unReadRecords > 0)
                sStorageInfo.unReadRecords--;
            if(sStorageInfo.readRecords < sStorageInfo.totalSlot)
                sStorageInfo.readRecords++;
            CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
            pEvent->nParam1 = pSmsMOInfo->nIndex + (pSmsMOInfo->nLocation << 16);  
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = pEvent->nParam1;
            pSmsMOInfo->sCfw_Event.nParam2 = 0;
            pSmsMOInfo->sCfw_Event.nType   = pEvent->nType;
            goto nFlagEvent;
        }
        else  
        {
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
            pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
            goto nFlagEvent;
        }
    }

nFlagEvent:
    //Wait for the event result
    pOutParam->nParam1 = pSmsMOInfo->sCfw_Event.nParam1;
    pOutParam->nParam2 = pSmsMOInfo->sCfw_Event.nParam2;
    pOutParam->nType   = pSmsMOInfo->sCfw_Event.nType;
    
    CSW_PROFILE_FUNCTION_EXIT(sms_Sync_SetUnRead2Read);
}


VOID sms_Sync_SetUnSent2Sent(CFW_SMS_MO_INFO* pSmsMOInfo, CFW_SMS_OUT_PARAM* pOutParam)
{
    UINT32  ret=0x0;
    CFW_EV* pEvent=NULL;
    CFW_EV  sMeEvent;
    UINT8   nSmsStatus=0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_Sync_SetUnSent2Sent);
 
    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));


    if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
    {
//        ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal);
//        if(ret == ERR_SUCCESS)
//        {
//            CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
//        }
//        else
//        {
//            CFW_GetUTI(hAo, &nGetUti);
//            CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
//            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
//            return ERR_CMS_UNKNOWN_ERROR;
//        }
//    break;
    }
    else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
    {
        //Malloc for the read data.
        sMeEvent.nParam1 = (UINT32)((UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4));
        if(!(UINT8*)sMeEvent.nParam1)
        {
            CSW_TRACE(CFW_SMS_TS_ID, "sms_Sync_SetUnSent2Sent sMeEvent.nParam1 malloc error!!! \n ");
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = ERR_NO_MORE_MEMORY;
            pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
            goto nFlagEvent;
        }
        SUL_ZeroMemory8((UINT8*)sMeEvent.nParam1,SMS_MO_ONE_PDU_SIZE+4+4);
        ret = CFW_MeReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,&sMeEvent);
        if(ret == ERR_SUCCESS)
        {
//            //Go to CFW_SM_STATE_WAIT directly!
//            CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
            pEvent = &sMeEvent;
            pEvent->nTransId = pSmsMOInfo->nUTI_Internal;
        }
        else  //other return value
        {
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
            pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
            goto nFlagEvent;
        }
    }
    if(((pEvent->nEventId == EV_CFW_SIM_READ_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)) && \
       (pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
    {
        if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1))
        {
            //Just get SMS status 
            sms_tool_DecomposePDU((PVOID)pEvent->nParam1,1,NULL,&nSmsStatus,NULL);
            if(nSmsStatus == CFW_SMS_STORED_STATUS_UNSENT)
            {
                *((UINT8*)pEvent->nParam1) = pSmsMOInfo->nStatus;
                if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
                {
                    //Write it with CFW_SMS_STORED_STATUS_READ
                    ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
                }
                else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
                {
                    UINT32 nTime=0x0, nAppInt32=0x0,nTmp=0x0;
                    nTmp = sMeEvent.nParam1;
                    ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
                    CSW_SMS_FREE((UINT8*)(nTmp));
                }
                if(ret != ERR_SUCCESS)
                {
                    pSmsMOInfo->sCfw_Event.nFlag   = 1;
                    pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_UNKNOWN_ERROR;
                    pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
                    pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                    goto nFlagEvent;
                }
                pEvent = &sMeEvent;
                pEvent->nTransId  = pSmsMOInfo->nUTI_Internal;
                pSmsMOInfo->nReadSmsStatus = CFW_SMS_STORED_STATUS_UNREAD;
            }
            else
            {
                if(pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)
                    CSW_SMS_FREE((UINT8*)(sMeEvent.nParam1));
                pSmsMOInfo->sCfw_Event.nFlag   = 1;
                pSmsMOInfo->sCfw_Event.nParam1 = ERR_CMS_OPERATION_NOT_ALLOWED;
                pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
                pSmsMOInfo->sCfw_Event.nType   = 0xf0;
                goto nFlagEvent;
            }
            pEvent->nParam2 = 0;
        }
        else //read false
        {
            if(pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)
                CSW_SMS_FREE((UINT8*)(sMeEvent.nParam1));
            //Read a NULL SIM
            if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1 == 0))
            {
                pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
                pEvent->nType = 0xf0;
            }                    
            else if(pEvent->nType == 0xf0) 
            {
                if(pEvent->nParam1 == ERR_CME_ME_FAILURE)  //Me Return error
                {
                    pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
                }
                else if(pEvent->nParam1 == ERR_CME_INVALID_INDEX)//Me Return error
                {
                    pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
                }
                else if(pEvent->nParam1 == ERR_CMS_INVALID_MEMORY_INDEX)
                {
                    pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
                }
                else if(pEvent->nParam1 == ERR_CME_SIM_PIN_REQUIRED)
                {
                    pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
                }
                else if(pEvent->nParam1 == ERR_CME_SIM_FAILURE)
                {
                    pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
                }
                else if(pEvent->nParam1 == ERR_CME_SIM_NOT_INSERTED)
                {
                    pEvent->nParam1 = ERR_CMS_TEMPORARY_FAILURE;
                }
                else  
                {
                    pEvent->nParam1 = ERR_CMS_UNKNOWN_ERROR;
                }
            }
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = pEvent->nParam1;
            pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
            pSmsMOInfo->sCfw_Event.nType   = pEvent->nType;
            goto nFlagEvent;
        }
    }
    if(((pEvent->nEventId == EV_CFW_SIM_WRITE_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_WRITE_MESSAGE_RSP)) && \
       (pSmsMOInfo->nUTI_Internal == pEvent->nTransId) )
    {
        if(pEvent->nType == 0)
        {
            CFW_SMS_STORAGE_INFO  sStorageInfo;

            //Zero memory
            SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
            CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
            if(sStorageInfo.unsentRecords > 0)
                sStorageInfo.unsentRecords--;
            if(sStorageInfo.sentRecords < sStorageInfo.totalSlot)
                sStorageInfo.sentRecords++;
            CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
            pEvent->nParam1 = pSmsMOInfo->nIndex + (pSmsMOInfo->nLocation << 16);  
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = pEvent->nParam1;
            pSmsMOInfo->sCfw_Event.nParam2 = 0;
            pSmsMOInfo->sCfw_Event.nType   = pEvent->nType;
            goto nFlagEvent;
        }
        else
        {
            pSmsMOInfo->sCfw_Event.nFlag   = 1;
            pSmsMOInfo->sCfw_Event.nParam1 = pEvent->nParam1;
            pSmsMOInfo->sCfw_Event.nParam2 = pSmsMOInfo->nIndex;
            pSmsMOInfo->sCfw_Event.nType   = 0xf0;
            goto nFlagEvent;
        }
    }
            
nFlagEvent:
    //Wait for the event result
    pOutParam->nParam1 = pSmsMOInfo->sCfw_Event.nParam1;
    pOutParam->nParam2 = pSmsMOInfo->sCfw_Event.nParam2;
    pOutParam->nType   = pSmsMOInfo->sCfw_Event.nType;
    
    CSW_PROFILE_FUNCTION_EXIT(sms_Sync_SetUnSent2Sent);
}
#endif




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
VOID sms_tool_SMSTime_TO_SCTS(TM_SMS_TIME_STAMP SMSTime,UINT8 *SCTS)
{
	UINT8 pTmp[7];
	UINT8 TMPNode[2];
	CSW_PROFILE_FUNCTION_ENTER(sms_tool_SMSTime_TO_SCTS);

	SUL_ZeroMemory8(pTmp,7);
	SUL_ZeroMemory8(TMPNode,2);
	SUL_StrPrint(TMPNode,"%d",(SMSTime.uYear - 2000));
	SUL_AsciiToGsmBcd(TMPNode,2, &(pTmp[0]));
	//SUL_AsciiToGSMBCD(TMPNode,2,&(pTmp[0]));

	SUL_ZeroMemory8(TMPNode,2);
	SUL_StrPrint(TMPNode,"%d",SMSTime.uMonth);
	SUL_AsciiToGsmBcd(TMPNode,2, &(pTmp[1]));
	//SUL_AsciiToGSMBCD(TMPNode,2,&(pTmp[1]));

	SUL_ZeroMemory8(TMPNode,2);
	SUL_StrPrint(TMPNode,"%d",SMSTime.uDay);
	SUL_AsciiToGsmBcd(TMPNode,2, &(pTmp[2]));
	//SUL_AsciiToGSMBCD(TMPNode,  2,&(pTmp[2]));

	SUL_ZeroMemory8(TMPNode,2);
	SUL_StrPrint(TMPNode,"%d",SMSTime.uHour);
	SUL_AsciiToGsmBcd(TMPNode,2, &(pTmp[3]));
	//SUL_AsciiToGSMBCD(TMPNode, 2,&(pTmp[3]));

	SUL_ZeroMemory8(TMPNode,2);
	SUL_StrPrint(TMPNode,"%d",SMSTime.uMinute);
	SUL_AsciiToGsmBcd(TMPNode,2, &(pTmp[4]));
	//SUL_AsciiToGSMBCD(TMPNode,2,&(pTmp[4]));

	SUL_ZeroMemory8(TMPNode,2);
	SUL_StrPrint(TMPNode,"%d",SMSTime.uSecond);
	SUL_AsciiToGsmBcd(TMPNode,2, &(pTmp[5]));
	//SUL_AsciiToGSMBCD(TMPNode,2,&(pTmp[5]));

	SUL_ZeroMemory8(TMPNode,2);
	SUL_StrPrint(TMPNode,"%d",SMSTime.iZone);

	SUL_AsciiToGsmBcd( TMPNode, 2, &(pTmp[6]));
	//SUL_AsciiToGSMBCD(TMPNode,  2,&(pTmp[6]));

	SUL_MemCopy8(SCTS,pTmp,7);

	CSW_PROFILE_FUNCTION_EXIT(sms_tool_SMSTime_TO_SCTS);
}

UINT32 sms_tool_ComposePDU(
						  UINT8   Concat,
						  UINT8   SMSStatus,
						  UINT8*  pData,
						  UINT16  nDataSize,
						  CFW_DIALNUMBER* pNumber,
#ifdef CFW_MULTI_SIM
						CFW_SIM_ID nSimId,
#endif						
						  CFW_EV* pEvent
						  )
{
	//
	//synchronization function used for efficiency.
	//
	CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU Concat[%d] SMSStatus[%d] nDataSize[%d]\n ",
      Concat,SMSStatus,nDataSize);
	CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU pData\n ");
	CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, pData, nDataSize, 16);
	CSW_TRACE(CFW_SMS_TS_ID, "Number Type[0x%x] Len[%d]\n",pNumber->nType,pNumber->nDialNumberSize);
	CSW_TRACE(CFW_SMS_TS_ID, "Number\n");
	CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, pNumber->pDialNumber, pNumber->nDialNumberSize, 16);
	UINT8 *pBCD=NULL;
	UINT8 nLength=0x0;
	
	UINT8 nDialNumberSize=0x0;	
	UINT8 nSCANumber[12] = {0x00,};
	
	UINT32 nReturn=0x0,nLength_For_SingleSms=0x0,nLength_For_LongSms=0x0;
	UINT8* DialNumber=NULL;
	
	CFW_SMS_PARAMETER Info;
	CFW_SMS_PARAMETER* pInfo =&Info;
	CSW_PROFILE_FUNCTION_ENTER(sms_tool_ComposePDU);
/*	
	if(nDataSize > 140)
	{
		//
		//超长短信将在后续的版本进行处理 ^.^
		//TODO..
		//
		
		pEvent->nParam2  = 0; 			
		pEvent->nParam1  = 0;
		pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
		pEvent->nType    = 0xF0;
		return(ERR_INVALID_PARAMETER);
		
	}	
*/
	DialNumber = pNumber->pDialNumber;
	//
	//处理用户没有输入电话号码
	//
	if((pNumber->pDialNumber == NULL) || (pNumber->nDialNumberSize == 0x00))
	{
		nDialNumberSize = 0x00;
	}
	else
	{			
		nLength = pNumber->nDialNumberSize;
		nDialNumberSize = pNumber->nDialNumberSize;	
		(nLength & 0x01)  ? (nLength = nLength / 2 + 2) :(nLength = nLength / 2 + 1) ;

		pBCD = (UINT8 *)CSW_SMS_MALLOC(nLength );
		SUL_ZeroMemory8(pBCD,(nLength ));
		if(ERR_SUCCESS != SUL_UserDialNum2StackDialNum(pNumber,pBCD,&nLength))
		{
	              CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU pBCD malloc error!!! \n ");
			pEvent->nType  = 0xF0;
			CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
			return ERR_INVALID_PARAMETER;	
		}
        
		//
		//处理0x86,如果用户前两位电话号码为86则认为是输入的区位码，
		//
		//现在存在的问题是：如果用户电话号码本身前两位是86该如何处理 
		//  ^.^
		//
		//CFW_TELNUMBER_TYPE_INTERNATIONAL 145   International number 
		//CFW_TELNUMBER_TYPE_NATIONAL      161   National number. 
		//CFW_TELNUMBER_TYPE_UNKNOWN       129   Unknown number type (default) 
		//
	}
	
	//
	//Get SMS parameter
	//
#ifndef CFW_MULTI_SIM
	nReturn = CFW_CfgGetDefaultSmsParam(pInfo);
#else
	nReturn = CFW_CfgGetDefaultSmsParam(pInfo, nSimId);
#endif
	
	if(nReturn != ERR_SUCCESS)
	{
		pEvent->nParam2  = 0; 			
		pEvent->nParam1  = 0;
		pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
		pEvent->nType    = 0xF0;
		
		CSW_SMS_FREE(pBCD);
		CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
		return(nReturn);
	}
	
	if(pInfo->nNumber[0] == 0x00)
	{
		//
		//Get SCA failed
		//
		CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
		return ERR_INVALID_PARAMETER;
	}
	else
	{
		SUL_MemCopy8(nSCANumber,pInfo->nNumber,12);	
		if(pInfo->nNumber[0] > 12)
		{
			nSCANumber[0] = 12;
		}
	}	
	if(pInfo->dcs == 0x00)//GSM 7 bit default alphabet and uncompressed
	{
		nLength_For_SingleSms = CFW_SMS_LEN_7BIT_NOMAL;
		nLength_For_LongSms   = CFW_SMS_LEN_7BIT_LONG;//hameina[mod] 152->153
	}
	else//4: 8 bit data and uncompressed, 8:UCS2 (16bit) and uncompressed
	{
		nLength_For_SingleSms = CFW_SMS_LEN_UCS2_NOMAL;
		nLength_For_LongSms   = CFW_SMS_LEN_UCS2_LONG;
	}
	if(nDataSize <= nLength_For_SingleSms)
	{
		//**********************************************************************
		//Determine the SMS is for sending or for saving
		//**********************************************************************
		
		if((Concat & 0x08) == 0x00)	//for sending
		{
			//
			//Compose SMS PDU for Send.
			//
			UINT8  *PDU = (UINT8 *)CSW_SMS_MALLOC(170);
			
			api_SmsPPSendReq_t *pPostNode =NULL;
			
			SUL_ZeroMemory8(PDU,170);
			
			//*********************************************************************
			//TP-MTI: bit1-0: xxxx xx01: SMS-SUBMIT(MS->SC)
			//		  0 0   : SMS DELIVER (in the direction SC to MS)	
			//  	  0 0   : SMS DELIVER REPORT (in the direction MS to SC)	
			//  	  1 0   : SMS STATUS REPORT (in the direction SC to MS)		
			//  	  1 0   : SMS COMMAND (in the direction MS to SC)	
			//  	  0 1   : SMS SUBMIT (in the direction MS to SC)	
			//  	  0 1   : SMS SUBMIT REPORT (in the direction SC to MS)	
			//  	  1 1   : Reserved
			//*********************************************************************
			
			//*********************************************************************
			//TP-RD:  bit2  : xxxx x0xx: 
			// 			0   : Instruct the SC to accept an SMS SUBMIT for an SM still held in the
			//					SC which has the same TP MR and the same TP DA as a 
			//                  previously 	submitted SM from 	the same OA.
			//			1   : Instruct the SC to reject an SMS SUBMIT for an SM still held in the
			//					SC which has the same TP MR and the same TP DA as the
			//                  previously submitted SM 	from the same OA. In this case 
			//				    the response returned by the SC is as specified in 9.2.3.6.
			//*********************************************************************
			
			//*********************************************************************
			//TP-VPF  bit4-3: xxx1 0xxx: 
			//        1 0   : TP-VP filed present-relative format
			//        0 1   : TP-VP filed present-enhanced format
			//        1 1   : TP-VP filed present-absolute format
			//        0 0   : TP-VP filed no present
			//*********************************************************************
			
			//*********************************************************************
			//TP SRR  bit5  : xx1x xxxx: TP SRR
			//			0   : A status report is not requested	
			//			1   : A status report is requested
			//*********************************************************************
			
			//*********************************************************************
			//TP UDHI bit6  : x0xx xxxx: TP UDHI, for loner SMS
			//*********************************************************************
			
			//*********************************************************************
			//TP-RP   bit7  : 0xxx xxxx: TP-RP
			//			0	: TP Reply Path parameter is not set in this SMS SUBMIT/DELIVER
			//			1 	: TP Reply Path parameter is set in this SMS SUBMIT/DELIVER
			//*********************************************************************
			
			//
			//0x02是VP的默认值，考虑到现在取消了VPF，所以VP用该值来代替
			//
			
			//PDU[0] = pInfo->mti | ((0x02) << 3) | ((pInfo->ssr) << 5) |((pInfo->rp) << 7) ;
			PDU[0] = 0x01 | ((0x02) << 3) | ((pInfo->ssr) << 5) |((pInfo->rp) << 7) ;
			
			PDU[1] = pInfo->mr;
			PDU[2] = nDialNumberSize;
			
			if(nDialNumberSize == 0x00)
			{
				nLength = 1;
				PDU[3] = pNumber->nType;
			}
			else
			{
				SUL_MemCopy8((PDU + 3),pBCD,nLength);
				CSW_SMS_FREE(pBCD);
			}
			
			PDU[3 + nLength + 0] = pInfo->pid;
			PDU[3 + nLength + 1] = pInfo->dcs;
			PDU[3 + nLength + 2] = pInfo->vp;
			
			if(pInfo->dcs == 0x00)
			{
				//
				//0 GSM 7 bit default alphabet and uncompressed 
				//
				UINT32 nTotalLength;
				//
				//8 bits to 7 bits
				//
				//[[hameina[+] 2007.3.22 add the judge condition for sending empty sms
				if(nDataSize)
					PDU[3 + nLength + 3] = (UINT8)SUL_Encode7Bit(pData,(PDU+ 3 + nLength + 4),nDataSize);
				else
					PDU[3 + nLength + 3] = 0;
				//]]hameina[mod]2007.3.22
				
				nTotalLength = 3 + nLength + 4;
				nTotalLength = nTotalLength + PDU[3 + nLength + 3];
				//
				//add by lxp at 2006/05/15
				//For the sending  7bit SMS
				//
				PDU[3 + nLength + 3] = (UINT8)nDataSize;
				
				pPostNode = (api_SmsPPSendReq_t *)CSW_SMS_MALLOC( \
					sizeof(api_SmsPPSendReq_t) + nTotalLength);
				
				if(pPostNode == NULL)
				{
					CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU 1 pPostNode malloc error!!! \n ");
					CSW_SMS_FREE(PDU);
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
					return(ERR_NO_MORE_MEMORY);
				}
				SUL_ZeroMemory8(pPostNode,(sizeof(api_SmsPPSendReq_t) + nTotalLength));
				pPostNode->AddressLen =  nSCANumber[0];
				pPostNode->Concat     = (Concat & 0x04) >> 3;
				pPostNode->Path       =  pInfo->bearer;
				
				SUL_MemCopy8(pPostNode->Address,(&(nSCANumber[1])),(nSCANumber[0]));
				SUL_MemCopy8(pPostNode->Data,PDU,nTotalLength);
				
				pPostNode->DataLen = (UINT8)(nTotalLength);
				
				CSW_SMS_FREE(PDU);
				
				pEvent->nType    = 0x00;
				pEvent->nParam2  = (1<<16) + pInfo->mr; 			
				pEvent->nParam1  = (UINT32)pPostNode;
				pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
				
				//
				//For test
				//
				//CSW_SMS_FREE(pEvent->nParam1);
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
				return(ERR_SUCCESS);		
				
			}
			else if(pInfo->dcs ==32)
			{
				//
				//32 GSM 7 bit default alphabet and compressed 
				//
				
				//TODO...
				CSW_SMS_FREE(PDU);
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
				return(ERR_INVALID_PARAMETER);
			}
//			else if(pInfo->dcs ==4)
//			{
//				//
//				//4 8 bit data and uncompressed 
//				//
//				
//				//TODO...
//				CSW_SMS_FREE(PDU);
//				return(ERR_INVALID_PARAMETER);
//			}
			else if(pInfo->dcs ==36)
			{
				//
				//36 8 bit data and compressed 
				//	
				
				//TODO...
				CSW_SMS_FREE(PDU);
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
				return(ERR_INVALID_PARAMETER);
			}
			else if((pInfo->dcs ==8) || (pInfo->dcs ==4))
			{
				//
				//8 UCS2 (16bit) and uncompressed 
				//
				UINT32 nTotalLength;
				
				PDU[3 + nLength + 3] =  (UINT8)(nDataSize);
				nTotalLength = (3 + nLength + 4 + nDataSize);
				
				//
				//因为考虑到传入的就是UCS2编码格式，所以不做转化。
				//
				pPostNode = (api_SmsPPSendReq_t *)CSW_SMS_MALLOC( \
					sizeof(api_SmsPPSendReq_t) + nTotalLength);
				
				if(pPostNode == NULL)
				{
					CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU 2 pPostNode malloc error!!! \n ");
					CSW_SMS_FREE(PDU);
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
					return(ERR_NO_MORE_MEMORY);
				}
				SUL_ZeroMemory8(pPostNode,(sizeof(api_SmsPPSendReq_t) + nTotalLength));
				
				pPostNode->AddressLen =  nSCANumber[0];
				pPostNode->Concat     = (Concat & 0x04) >> 3;
				pPostNode->Path       =  pInfo->bearer;
				SUL_MemCopy8(pPostNode->Address,(&(nSCANumber[1])),(nSCANumber[0]));
				if(nDataSize)//hameina[+]2007.3.22: send empty sms
					SUL_MemCopy8((PDU+ 3 + nLength + 4),(pData),nDataSize);
				SUL_MemCopy8(pPostNode->Data,PDU,nTotalLength);
				
				pPostNode->DataLen = (UINT8)(nTotalLength);
				
				CSW_SMS_FREE(PDU);
				
				pEvent->nType    = 0x00;
				pEvent->nParam2  = (1<<16) + pInfo->mr; 			
				pEvent->nParam1  = (UINT32)pPostNode;
				pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
				
				//
				//For debug
				//
				//CSW_SMS_FREE(pEvent->nParam1);
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
				return(ERR_SUCCESS);			
			}
			else if(pInfo->dcs ==40)
			{
				//
				//40 UCS2 (16bit) and compressed 
				//		
				CSW_SMS_FREE(PDU);
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
				return(ERR_INVALID_PARAMETER);
			}		
		}
		else if((Concat & 0x08) == 0x08)  //for saving
		{
			//
			//Save PDU Both for Submit or Deliver
			//		
			if((Concat & 0x03) == 0x01)
			{
				//
				//01: SMS SUBMIT (in the direction MS to SC)   
				//
				UINT8  *PDU = (UINT8 *)CSW_SMS_MALLOC(176);
				UINT8  nTmp=0x0,Tmp=0x0;
				
				if(PDU == NULL)
				{
					CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU 3 PDU malloc error!!! \n ");
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
					return(ERR_NO_MORE_MEMORY);
				}
				
				SUL_ZeroMemory8(PDU,176);
				
				PDU[0] = SMSStatus;
				PDU[1] = nSCANumber[0];
				
				if(nSCANumber[0] == 0x00)
				{
					//
					//Get SCA failed
					//
				      CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);  
					return ERR_INVALID_PARAMETER;
				}
				else
				{
					SUL_MemCopy8(&(PDU[2]),(&(nSCANumber[1])),(nSCANumber[0]));
					
					nTmp = (UINT8)(2 + nSCANumber[0]);
				}
				
				//*********************************************************************
				//TP-MTI: bit1-0: xxxx xx01: SMS-SUBMIT(MS->SC)
				//		  0 0   : SMS DELIVER (in the direction SC to MS)	
				//  	  0 0   : SMS DELIVER REPORT (in the direction MS to SC)	
				//  	  1 0   : SMS STATUS REPORT (in the direction SC to MS)		
				//  	  1 0   : SMS COMMAND (in the direction MS to SC)	
				//  	  0 1   : SMS SUBMIT (in the direction MS to SC)	
				//  	  0 1   : SMS SUBMIT REPORT (in the direction SC to MS)	
				//  	  1 1   : Reserved
				//*********************************************************************
				
				//*********************************************************************
				//TP-RD:  bit2  : xxxx x0xx: 
				// 			0   : Instruct the SC to accept an SMS SUBMIT for an SM still held in the
				//					SC which has the same TP MR and the same TP DA as a 
				//                  previously 	submitted SM from 	the same OA.
				//			1   : Instruct the SC to reject an SMS SUBMIT for an SM still held in the
				//					SC which has the same TP MR and the same TP DA as the
				//                  previously submitted SM 	from the same OA. In this case 
				//				    the response returned by the SC is as specified in 9.2.3.6.
				//*********************************************************************
				
				//*********************************************************************
				//TP-VPF  bit4-3: xxx1 0xxx: 
				//        1 0   : TP-VP filed present-relative format
				//        0 1   : TP-VP filed present-enhanced format
				//        1 1   : TP-VP filed present-absolute format
				//        0 0   : TP-VP filed no present
				//*********************************************************************
				
				//*********************************************************************
				//TP SRR  bit5  : xx1x xxxx: TP SRR
				//			0   : A status report is not requested	
				//			1   : A status report is requested
				//*********************************************************************
				
				//*********************************************************************
				//TP UDHI bit6  : x0xx xxxx: TP UDHI, for loner SMS
				//*********************************************************************
				
				//*********************************************************************
				//TP-RP   bit7  : 0xxx xxxx: TP-RP
				//			0	: TP Reply Path parameter is not set in this SMS SUBMIT/DELIVER
				//			1 	: TP Reply Path parameter is set in this SMS SUBMIT/DELIVER
				//*********************************************************************
				
				//PDU[nTmp] = pInfo->mti | ((pInfo->vpf)<<3) | ((pInfo->ssr)<<5) ;
				PDU[nTmp + 0] = 0x01 | ((0x02)<<3) | ((pInfo->ssr)<<5) | ((pInfo->rp) << 7) ;
				PDU[nTmp + 1] = pInfo->mr;
				PDU[nTmp + 2] = nDialNumberSize;
				
				if(nDialNumberSize != 0x00)
				{
					Tmp = nDialNumberSize;
					if((Tmp / 2) == ((Tmp + 1) / 2))
					{
						Tmp = (UINT8)(Tmp / 2);
					}
					else
					{
						Tmp = (UINT8)(Tmp / 2 + 1);
					}
					Tmp ++;
					SUL_MemCopy8((PDU + nTmp + 3),pBCD,Tmp);
					CSW_SMS_FREE(pBCD);
				}
				else
				{
					Tmp = 1;//
					PDU[nTmp + 3] = pNumber->nType;
				}
				
				PDU[nTmp + 3 + Tmp + 0] = pInfo->pid;
				PDU[nTmp + 3 + Tmp + 1] = pInfo->dcs;			
				PDU[nTmp + 3 + Tmp + 2] = pInfo->vp;
				
				if(pInfo->dcs == 0x00)
				{
					//
					//0 GSM 7 bit default alphabet and uncompressed 
					//
					
					//
					//8 bits to 7 bits
					//				
					
					//[[xueww[mod] 2007.04.02
					if(nDataSize)
						PDU[nTmp + 3 + Tmp + 3] = (UINT8)SUL_Encode7Bit(pData,(PDU+ nTmp + 3 + Tmp + 4),nDataSize);
					else
						PDU[nTmp + 3 + Tmp + 3] = 0;
					//]]xueww[mod] 2007.04.02
					//
					//add by lxp at 2006/05/15
					//For the sending  7bit SMS
					//
					PDU[nTmp + 3 + Tmp + 3] = (UINT8)nDataSize;//xueww[mod] 2007.03.22
					pEvent->nType    = 0x00;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)PDU;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);			
					return(ERR_SUCCESS);
				}
				else if(pInfo->dcs ==32)
				{
					//
					//32 GSM 7 bit default alphabet and compressed 
					//
					CSW_SMS_FREE(PDU);
					
					pEvent->nType    = 0xF0;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)NULL;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);			
					return(ERR_INVALID_PARAMETER);
				}
//				else if(pInfo->dcs ==4)
//				{
//					//
//					//4 8 bit data and uncompressed 
//					//	
//					CSW_SMS_FREE(PDU);
//					
//					pEvent->nType    = 0xF0;			
//					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
//					pEvent->nParam1  = (UINT32)NULL;
//					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
//					
//					return(ERR_INVALID_PARAMETER);
//				}
				else if(pInfo->dcs ==36)
				{
					//
					//36 8 bit data and compressed 
					//			
					CSW_SMS_FREE(PDU);
					
					pEvent->nType    = 0xF0;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)NULL;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);					
					return(ERR_INVALID_PARAMETER);
				}
				else if((pInfo->dcs ==8) || (pInfo->dcs ==4))
				{
					//
					//8 UCS2 (16bit) and uncompressed 
					//
					
					PDU[nTmp + 3 + Tmp + 3] =  (UINT8)(nDataSize);
					if(nDataSize)//xueww[+]2007.04.02: save empty sms
						SUL_MemCopy8((PDU+ nTmp + 3 + Tmp + 4),pData,nDataSize);
					
					pEvent->nType    = 0x00;				
					pEvent->nParam2  = (1<<16) + pInfo->mr; 
					pEvent->nParam1  = (UINT32)PDU;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);					
					return(ERR_SUCCESS);
				}
				else if(pInfo->dcs ==40)
				{
					//
					//40 UCS2 (16bit) and compressed 
					//
					CSW_SMS_FREE(PDU);
					
					pEvent->nType    = 0xF0;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)NULL;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);				
					return(ERR_INVALID_PARAMETER);
				}
			}
			else if((Concat & 0x03) == 0x00)
			{
				//
				//SMS DELIVER (in the direction SC to MS) 
				//
				
				UINT8  *PDU = (UINT8 *)CSW_SMS_MALLOC(176);
				UINT8  nTmp=0x0,Tmp=0x0,SCTS[7];
				UINT32 nTime=0x0;
				TM_FILETIME nFileTime;
				TM_SYSTEMTIME SystemTime,* pSystemTime;
				TM_SMS_TIME_STAMP SMSTime;
				
				if(PDU == NULL)
				{
					CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU 4 PDU malloc error!!! \n ");
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
					return(ERR_NO_MORE_MEMORY);
				}
				
				SUL_ZeroMemory8(PDU,176);
				pSystemTime = &SystemTime;
				
				PDU[0] = SMSStatus;
				
				if(nSCANumber[0] == 0x00)
				{
			          CSW_SMS_FREE(PDU);

			          CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
			          return ERR_INVALID_PARAMETER;
				}
				else
				{
					PDU[1] = nSCANumber[0];
					SUL_MemCopy8(&(PDU[2]),(&(nSCANumber[1])),(nSCANumber[0]));
					
					nTmp = (UINT8)(2 + nSCANumber[0]);			
				}
				
				
				
				//*********************************************************************
				//TP-MTI: bit1-0: xxxx xx01: SMS-SUBMIT(MS->SC)
				//		  0 0   : SMS DELIVER (in the direction SC to MS)	
				//  	  0 0   : SMS DELIVER REPORT (in the direction MS to SC)	
				//  	  1 0   : SMS STATUS REPORT (in the direction SC to MS)		
				//  	  1 0   : SMS COMMAND (in the direction MS to SC)	
				//  	  0 1   : SMS SUBMIT (in the direction MS to SC)	
				//  	  0 1   : SMS SUBMIT REPORT (in the direction SC to MS)	
				//  	  1 1   : Reserved
				//*********************************************************************
				
				//*********************************************************************
				//TP-RD:  bit2  : xxxx x0xx: 
				// 			0   : Instruct the SC to accept an SMS SUBMIT for an SM still held in the
				//					SC which has the same TP MR and the same TP DA as a 
				//                  previously 	submitted SM from 	the same OA.
				//			1   : Instruct the SC to reject an SMS SUBMIT for an SM still held in the
				//					SC which has the same TP MR and the same TP DA as the
				//                  previously submitted SM 	from the same OA. In this case 
				//				    the response returned by the SC is as specified in 9.2.3.6.
				//*********************************************************************
				
				//*********************************************************************
				//TP-VPF  bit4-3: xxx1 0xxx: 
				//        1 0   : TP-VP filed present-relative format
				//        0 1   : TP-VP filed present-enhanced format
				//        1 1   : TP-VP filed present-absolute format
				//        0 0   : TP-VP filed no present
				//*********************************************************************
				
				//*********************************************************************
				//TP SRR  bit5  : xx1x xxxx: TP SRR
				//			0   : A status report is not requested	
				//			1   : A status report is requested
				//*********************************************************************
				
				//*********************************************************************
				//TP UDHI bit6  : x0xx xxxx: TP UDHI, for loner SMS
				//*********************************************************************
				
				//*********************************************************************
				//TP-RP   bit7  : 0xxx xxxx: TP-RP
				//			0	: TP Reply Path parameter is not set in this SMS SUBMIT/DELIVER
				//			1 	: TP Reply Path parameter is set in this SMS SUBMIT/DELIVER
				//*********************************************************************
				
				PDU[nTmp + 0] = 0x00 | ((0x02) << 3) | ((pInfo->ssr) << 5) | ((pInfo->rp) << 7) ;
				PDU[nTmp + 1] = nDialNumberSize;
				
				if(nDialNumberSize != 0x00)
				{
					Tmp = nDialNumberSize;
					if((Tmp / 2) == ((Tmp + 1) / 2))
					{
						Tmp = (UINT8)(Tmp / 2);
					}
					else
					{
						Tmp = (UINT8)(Tmp / 2 + 1);
					}
					Tmp ++;
					SUL_MemCopy8((PDU + nTmp + 2),pBCD,Tmp);
					CSW_SMS_FREE(pBCD);
				}
				else
				{
					Tmp = 1;//
					PDU[nTmp + 2] =pNumber->nType;
				}
				
				PDU[nTmp + 2 + Tmp + 0] = pInfo->pid;
				PDU[nTmp + 2 + Tmp + 1] = pInfo->dcs;
				
				nTime = TM_GetTime();
				nFileTime.DateTime = nTime;
				TM_FileTimeToSystemTime(nFileTime,pSystemTime);
				
				SMSTime.iZone  = TM_GetTimeZone();
				SMSTime.uYear  = pSystemTime->uYear;
				SMSTime.uMonth = pSystemTime->uMonth;
				SMSTime.uDay   = pSystemTime->uDay;
				SMSTime.uHour  = pSystemTime->uHour;
				SMSTime.uMinute= pSystemTime->uMinute;
				SMSTime.uSecond= pSystemTime->uSecond;
				
				sms_tool_SMSTime_TO_SCTS(SMSTime,SCTS);
				
				SUL_MemCopy8(&(PDU[nTmp + 2 + Tmp + 2]),SCTS,7);
				
				if(pInfo->dcs == 0x00)
				{
					//
					//0 GSM 7 bit default alphabet and uncompressed 
					//
					
					//
					//8 bits to 7 bits
					//				
					
					//[[xueww[mod] 2007.04.02
					if(nDataSize)
						PDU[nTmp + 2 + Tmp + 9] = (UINT8)SUL_Encode7Bit(pData,(PDU+ nTmp + 2 + Tmp + 10),nDataSize);
					else
						PDU[nTmp + 2 + Tmp + 9] = 0;
					//]]xueww[mod] 2007.04.02
					//
					//add by lxp at 2006/05/15
					//For the sending  7bit SMS
					//
					PDU[nTmp + 2 + Tmp + 9] = (UINT8)nDataSize;					
					pEvent->nType    = 0x00;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)PDU;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);				
					return(ERR_SUCCESS);
				}
				else if(pInfo->dcs ==32)
				{
					//
					//32 GSM 7 bit default alphabet and compressed 
					//
					CSW_SMS_FREE(PDU);
					
					pEvent->nType    = 0xF0;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)NULL;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);					
					return(ERR_INVALID_PARAMETER);
				}
//				else if(pInfo->dcs ==4)
//				{
//					//
//					//4 8 bit data and uncompressed 
//					//	
//					CSW_SMS_FREE(PDU);
//					
//					pEvent->nType    = 0xF0;			
//					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
//					pEvent->nParam1  = (UINT32)NULL;
//					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
//					
//					return(ERR_INVALID_PARAMETER);
//				}
				else if(pInfo->dcs ==36)
				{
					//
					//36 8 bit data and compressed 
					//			
					CSW_SMS_FREE(PDU);
					
					pEvent->nType    = 0xF0;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)NULL;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);					
					return(ERR_INVALID_PARAMETER);
				}
				else if((pInfo->dcs ==8) || (pInfo->dcs ==4))
				{
					//
					//8 UCS2 (16bit) and uncompressed 
					//
					
					PDU[nTmp + 2 + Tmp + 9] =  (UINT8)(nDataSize);
					if(nDataSize)//xueww[+] 2007.04.02 save NULL msg
						SUL_MemCopy8((PDU+ nTmp + 2 + Tmp + 10),pData,nDataSize);
					
					pEvent->nType    = 0x00;				
					pEvent->nParam2  = (1<<16) + pInfo->mr; 
					pEvent->nParam1  = (UINT32)PDU;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);					
					return(ERR_SUCCESS);
				}
				else if(pInfo->dcs ==40)
				{
					//
					//40 UCS2 (16bit) and compressed 
					//
					CSW_SMS_FREE(PDU);
					
					pEvent->nType    = 0xF0;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)NULL;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);			
					return(ERR_INVALID_PARAMETER);
				}
				
			}
			else
			{
				//
				//Invild type
				//
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
				return(ERR_INVALID_PARAMETER);
			}		
			
		}
		else
		{
			CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
			return(ERR_INVALID_PARAMETER);
		}
	}
	else if(nDataSize < 255 * nLength_For_LongSms)
	{
		//
		//超长短信的处理
		//
		//**********************************************************************
		//Determine the SMS is for sending or for saving
		//**********************************************************************
		
		if((Concat & 0x08) == 0x00)	
		{
			//
			//Compose SMS PDU for Send.
			//
			
			UINT8 *Title =NULL ;//用来放头信息（MTI,MR,DA,PID,DCS,VP）
			//api_SmsPPSendReq_t *pPostNode =NULL;
			
			//			
			if(nDialNumberSize == 0x00)
			{
				nLength = 1;
			}
			Title= (UINT8 *)CSW_SMS_MALLOC(6 + nLength);
			SUL_ZeroMemory8(Title,(6 + nLength));
			//
			//0x02是VP的默认值，考虑到现在取消了VPF，所以VP用该值来代替
			//
			
			Title[0] = 0x01 | ((0x02) << 3) | ((pInfo->ssr) << 5) |((pInfo->rp) << 7) | ((UINT8)(0x01 << 6));
			Title[1] = pInfo->mr;
			Title[2] = nDialNumberSize;
			
			if(nDialNumberSize == 0x00)
			{
				Title[3] = pNumber->nType;
			}
			else
			{
				SUL_MemCopy8((Title + 3),pBCD,nLength);
				CSW_SMS_FREE(pBCD);
			}
			
			Title[3 + nLength + 0] = pInfo->pid;
			Title[3 + nLength + 1] = pInfo->dcs;
			Title[3 + nLength + 2] = pInfo->vp;

			if(pInfo->dcs == 0x00)
			{
				//
				//0 GSM 7 bit default alphabet and uncompressed 
				//
				//7bit maximum length of the short message within the TP-UD field 
				//is 153 (160 - 7);
				//
				
				UINT8 nSMSNumber,nTmp = 1,nLen,pMRTmp;//超长短信的分发条数
				UINT8 *pOut,*pOutTmp,*pUDTmp;
				UINT32 *nP2;
				UINT16 nCurrentLen;

				//
				//如果不能被最大数（153）整除，则需要多加一条来发最后一条短信。
				//计算分发为多少条短信
				//
				if(nDataSize % CFW_SMS_LEN_7BIT_LONG)
				{
					nSMSNumber = (UINT8)(nDataSize / CFW_SMS_LEN_7BIT_LONG + 1);
				}
				else
				{
					nSMSNumber = (UINT8)(nDataSize / CFW_SMS_LEN_7BIT_LONG) ;
				}

				//
				//nP2用来表针第二个参数pEvent->nParam2 为一个32位数组
				//
				nP2  = (UINT32 *) CSW_SMS_MALLOC(nSMSNumber * sizeof(UINT32));

				//
				//pOut用来表针第一个参数pEvent->nParam1 为一个api_SmsPPSendReq_t结构体数组
				//
				  pOut = (UINT8 *)CSW_SMS_MALLOC( 
					nSMSNumber * (sizeof(api_SmsPPSendReq_t) + 6 + nLength + 140));	

				if((pOut == NULL) || (nP2 == (UINT32*)NULL))
				{
					CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU 5 nP2 pOut malloc error!!! \n ");
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
					return(ERR_NO_MORE_MEMORY);
				}
				
				SUL_ZeroMemory8(pOut,nSMSNumber * (sizeof(api_SmsPPSendReq_t) 
					+ 6 + nLength + 140));
				SUL_ZeroMemory8(nP2,nSMSNumber * sizeof(UINT32));
				
				pUDTmp  = pData;
				pOutTmp = pOut;
				pMRTmp  = pInfo->mr;
				nCurrentLen = nDataSize;

				while(nTmp <= nSMSNumber)
        {	
					api_SmsPPSendReq_t* pPostNode =NULL;
					//UINT8 nUDTotL;
					pPostNode = (api_SmsPPSendReq_t*)pOutTmp;
					pPostNode->AddressLen =  nSCANumber[0];
					//
					//Edit at 20060421 for long sms
					//
					//pPostNode->Conca      =  (Concat & 0x04) >> 3;
					pPostNode->Concat     =  0x01;
					pPostNode->Path       =  pInfo->bearer;

					SUL_MemCopy8(pPostNode->Address,(&(nSCANumber[1])),(nSCANumber[0]));
					Title[1] = pMRTmp;//xueww[mod] 2007.07.10
					SUL_MemCopy8(pPostNode->Data,Title,(6 + nLength));
					//
					//UDHL
					//
					*(pPostNode->Data + 7 + nLength) = 0x05;
					//
					//IEI
					//
					*(pPostNode->Data + 8 + nLength) = 0x00;
					//
					//IEIDL
					//
					*(pPostNode->Data + 9 + nLength) = 0x03;
					//
					//Concatenated short message reference number
					//这里协议3GPP TS 23.040 -->9.2.3.24.1 
					//解释为２５６的模，这里取ＭＲ，
					//
					*(pPostNode->Data + 10 + nLength) = pInfo->mr;
					//
					//total number of SMS
					//
					*(pPostNode->Data + 11 + nLength) = nSMSNumber;
					//
					//current number of SMS
					//
					*(pPostNode->Data + 12 + nLength) = nTmp;

	        TS_OutputText(CFW_SMS_TS_ID, TSTXT("Send Long SMS Total[%d] Current[%d] MR[%d]\n"),nSMSNumber,nTmp,pInfo->mr);
          
					if(nCurrentLen > CFW_SMS_LEN_7BIT_LONG)
					{
					//this is fill bits
						*(pPostNode->Data + 13 + nLength) = *pUDTmp<<1;
					
						nLen = (UINT8)SUL_Encode7Bit((pUDTmp+1),(pPostNode->Data + 14 + nLength),CFW_SMS_LEN_7BIT_LONG - 1);
						pUDTmp = pUDTmp + CFW_SMS_LEN_7BIT_LONG;
						pOutTmp = pOutTmp + (sizeof(api_SmsPPSendReq_t) 
									+ 13 + nLength + nLen);
						nCurrentLen = (UINT16)(nCurrentLen - CFW_SMS_LEN_7BIT_LONG);
						nP2[nTmp - 1] = (sizeof(api_SmsPPSendReq_t) \
									+ 13 + nLength + nLen) + (pMRTmp << 8) + 
									(nSMSNumber << 16) + (nTmp << 24);

						//**********************************************************************
						//   high													low
						// 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1
						//  current number   total number           MR				Length
						//**********************************************************************
						*(pPostNode->Data + 6 + nLength) =0xA0 ;
						//
						//add by lxp at 2006/05/15
						//For the sending  7bit SMS
						//
						nLen = 133; // 152*7/8;
					}
					else 
					{
					//this is fill bits
						*(pPostNode->Data + 13 + nLength) = *pUDTmp<<1;
					
						nLen = (UINT8)SUL_Encode7Bit((pUDTmp+1),(pPostNode->Data + 14 + nLength), nCurrentLen-1);
						nP2[nTmp - 1] = (sizeof(api_SmsPPSendReq_t) \
							+ 13 + nLength + nLen) + (pMRTmp << 8) + \
							(nSMSNumber << 16) + (nTmp << 24);
						pPostNode->Concat     =  0x00;
						//UDL is septets number, so (udhl+1+fillbits)=7 octes and convert to 8 septets, add the septets of currentLen -1
						*(pPostNode->Data + 6 + nLength) =8 +nCurrentLen -1;

            //
						//add by lxp at 2006/05/15
						//For the sending  7bit SMS
						//
#if 0
            nLen = (UINT8)nCurrentLen;
            if(((nLen*7) % 8) == 0x00 )
            {
                nLen = (UINT8)((nLen*7) / 8);
            }
            else
            {
                nLen = (UINT8)((nLen*7) / 8 + 1);
            }
#endif
          }
					//*(pPostNode->Data + 6 + nLength) =(UINT8)(nP2[nTmp - 1] & 0xff);
					//*(pPostNode->Data + 6 + nLength) =nLen + 6 ;
					nTmp   ++;
					pMRTmp ++;
					pPostNode->DataLen = (UINT8)(14 + nLength + nLen);
				}   
                CSW_SMS_FREE(Title);
				pEvent->nType    = 0x01;
				pEvent->nParam2  = (UINT32)nP2; 			
				pEvent->nParam1  = (UINT32)pOut;
				pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
				
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
				return(ERR_SUCCESS);	
			}
			else if(pInfo->dcs ==32)
			{
				//
				//32 GSM 7 bit default alphabet and compressed 
				//
				
				//TODO...
				//CSW_SMS_FREE(PDU);
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
				return(ERR_INVALID_PARAMETER);
			}
//			else if(pInfo->dcs ==4)
//			{
//				//
//				//4 8 bit data and uncompressed 
//				//
//				
//				//TODO...
//				//CSW_SMS_FREE(PDU);
//				return(ERR_INVALID_PARAMETER);
//			}
			else if(pInfo->dcs ==36)
			{
				//
				//36 8 bit data and compressed 
				//	
				
				///TODO...
				//CSW_SMS_FREE(PDU);
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
				return(ERR_INVALID_PARAMETER);
			}
			else if((pInfo->dcs ==8) || (pInfo->dcs ==4))
			{
				//
				//8 UCS2 (16bit) and uncompressed 
				//
				
				UINT8 nSMSNumber=0x0,nTmp = 1,nLen=0x0,pMRTmp=0x0;//超长短信的分发条数
				UINT8* pOut=NULL;
                UINT8* pOutTmp=NULL;
                UINT8* pUDTmp=NULL;
				UINT32 *nP2=NULL;
				UINT16 nCurrentLen=0x0;

				//
				//如果不能被最大数（134）整除，则需要多加一条来发最后一条短信。
				//计算分发为多少条短信
				//
				if(nDataSize % CFW_SMS_LEN_UCS2_LONG)
				{
					nSMSNumber = (UINT8)(nDataSize / CFW_SMS_LEN_UCS2_LONG + 1);
				}
				else
				{
					nSMSNumber = (UINT8)(nDataSize / CFW_SMS_LEN_UCS2_LONG) ;
				}

				//
				//nP2用来表针第二个参数pEvent->nParam2 为一个32位数组
				//
				nP2  = (UINT32 *) CSW_SMS_MALLOC(nSMSNumber * sizeof(UINT32));

				//
				//pOut用来表针第一个参数pEvent->nParam1 为一个api_SmsPPSendReq_t结构体数组
				//
				pOut = (UINT8 *)CSW_SMS_MALLOC( \
					nSMSNumber * (sizeof(api_SmsPPSendReq_t) + 6 + nLength + CFW_SMS_LEN_UCS2_NOMAL));//xueww[mod] 2007.06.18//134	

				if((pOut == NULL) || (nP2 == (UINT32*)NULL))
				{
					CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU 6 nP2 pOut malloc error!!! \n ");
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
					return(ERR_NO_MORE_MEMORY);
				}
				
				SUL_ZeroMemory8(pOut,nSMSNumber * (sizeof(api_SmsPPSendReq_t) 
					+ 6 + nLength + CFW_SMS_LEN_UCS2_LONG));//xueww[mod] 2007.06.18 //134
				SUL_ZeroMemory8(nP2,nSMSNumber * sizeof(UINT32));
				
				pUDTmp  = pData;
				pOutTmp = pOut;
				pMRTmp  = pInfo->mr;
				nCurrentLen = nDataSize;

				while(nTmp <= nSMSNumber)
				{	
					api_SmsPPSendReq_t* pPostNode =NULL;
					//UINT8 nUDTotL;
					pPostNode = (api_SmsPPSendReq_t*)pOutTmp;
					pPostNode->AddressLen =  nSCANumber[0];
					//
					//Edit at 20060421 for long sms
					//
					//pPostNode->Conca      =  (Concat & 0x04) >> 3;
					pPostNode->Concat     =  0x01;
					pPostNode->Path       =  pInfo->bearer;

					SUL_MemCopy8(pPostNode->Address,(&(nSCANumber[1])),(nSCANumber[0]));
					Title[1] = pMRTmp;//xueww[mod] 2007.07.10
					SUL_MemCopy8(pPostNode->Data,Title,(6 + nLength));
					//
					//UDHL
					//
					*(pPostNode->Data + 7 + nLength) = 0x05;
					//
					//IEI
					//
					*(pPostNode->Data + 8 + nLength) = 0x00;
					//
					//IEIDL
					//
					*(pPostNode->Data + 9 + nLength) = 0x03;
					//
					//Concatenated short message reference number
					//这里协议3GPP TS 23.040 -->9.2.3.24.1 
					//解释为２５６的模，这里取ＭＲ，
					//
					*(pPostNode->Data + 10 + nLength) = pInfo->mr;
					//
					//total number of SMS
					//
					*(pPostNode->Data + 11 + nLength) = nSMSNumber;
					//
					//current number of SMS
					//
					*(pPostNode->Data + 12 + nLength) = nTmp;
	        TS_OutputText(CFW_SMS_TS_ID, TSTXT("Send Long SMS Total[%d] Current[%d] MR[%d]\n"),nSMSNumber,nTmp,pInfo->mr);
	
					//*(pPostNode->Data + 13 + nLength) = 0xFF;
					if(nCurrentLen > CFW_SMS_LEN_UCS2_LONG)
					{

						SUL_MemCopy8((pPostNode->Data + 13 + nLength),pUDTmp,CFW_SMS_LEN_UCS2_LONG);
						nLen = CFW_SMS_LEN_UCS2_LONG;
						pUDTmp = pUDTmp + CFW_SMS_LEN_UCS2_LONG;
						pOutTmp = pOutTmp + (sizeof(api_SmsPPSendReq_t) 
									+ 12 + nLength + nLen);
						nCurrentLen = (UINT16)(nCurrentLen - CFW_SMS_LEN_UCS2_LONG);
						nP2[nTmp - 1] = (sizeof(api_SmsPPSendReq_t) \
									+ 12 + nLength + nLen) + (pMRTmp << 8) + 
									(nSMSNumber << 16) + (nTmp << 24);

						//**********************************************************************
						//   high													low
						// 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1
						//  current number   total number           MR				Length
						//**********************************************************************
					

					}
					else 
					{
						SUL_MemCopy8((pPostNode->Data + 13 + nLength),pUDTmp,nCurrentLen);
						nLen = (UINT8)nCurrentLen;
						nP2[nTmp - 1] = (sizeof(api_SmsPPSendReq_t) \
							+ 12 + nLength + nLen) + (pMRTmp << 8) + \
							(nSMSNumber << 16) + (nTmp << 24);
						pPostNode->Concat     =  0x00;
					}
					//*(pPostNode->Data + 6 + nLength) =(UINT8)(nP2[nTmp - 1] & 0xff);
					*(pPostNode->Data + 6 + nLength) =nLen + 6 ;
					nTmp   ++;
					pMRTmp ++;
					pPostNode->DataLen = (UINT8)(13 + nLength + nLen);
				}
                CSW_SMS_FREE(Title);
				pEvent->nType    = 0x01;
				pEvent->nParam2  = (UINT32)nP2; 			
				pEvent->nParam1  = (UINT32)pOut;
				pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
				
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);		
				return(ERR_SUCCESS);			
			}
			else if(pInfo->dcs ==40)
			{
				//
				//40 UCS2 (16bit) and compressed 
				//		
				//CSW_SMS_FREE(PDU);
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
				return(ERR_INVALID_PARAMETER);
			}		
		}
		else if((Concat & 0x08) == 0x08)
		{
			//
			//Save PDU Both for Submit or Deliver
			//保存 超长短信		
            CSW_TRACE(CFW_SMS_TS_ID, TSTXT("IN sms_tool_ComposePDU, Concat = %d \n"),Concat);
			if((Concat & 0x03) == 0x01)
			{
				//
				//01: SMS SUBMIT (in the direction MS to SC)   
				//
				//1：firstly get the title just as for send
				//2: secondly get the title length
				//3: Copy the title to the 176 PDU
				//OK,Let's go
				//SCA 12 byte Da TEL_NUMBER_MAX_LEN
				
				UINT8  *Title = NULL;
				UINT8  nTmp=0x0,Tmp=0x0,nTitleLength=0x0;

				Title = (UINT8 *)CSW_SMS_MALLOC(1 + 12 + 3 + 14 + 4);
				if(Title == NULL)
				{
					CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU 7 Title malloc error!!! \n ");
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
					return(ERR_NO_MORE_MEMORY);
				}
				
				SUL_ZeroMemory8(Title,1 + 12 + 3 + 14 + 4);
				
				Title[0] = SMSStatus;
				nTitleLength = 1;
				
				Title[1] = nSCANumber[0];
				
				if(nSCANumber[0] == 0x00)
				{
			          CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
			          return ERR_INVALID_PARAMETER;
				}
				else
				{
					SUL_MemCopy8(&(Title[2]),(&(nSCANumber[1])),(nSCANumber[0]));
					
					nTmp = (UINT8)(2 + nSCANumber[0]);
				}
				nTitleLength = (UINT8)(nTitleLength + nSCANumber[0] + 1);
				
				Title[nTmp + 0] = 0x01 | ((0x02)<<3) | ((pInfo->ssr)<<5) | ((pInfo->rp) << 7) | ((UINT8)(0x01 << 6));
				Title[nTmp + 1] = pInfo->mr;
				Title[nTmp + 2] = nDialNumberSize;
				nTitleLength = nTitleLength + 3;
				
				if(nDialNumberSize != 0x00)
				{
					Tmp = nDialNumberSize;
					if((Tmp / 2) == ((Tmp + 1) / 2))
					{
						Tmp = (UINT8)(Tmp / 2);
					}
					else
					{
						Tmp = (UINT8)(Tmp / 2 + 1);
					}
					Tmp ++;
					SUL_MemCopy8((Title + nTmp + 3),pBCD,Tmp);
					CSW_SMS_FREE(pBCD);
				}
				else
				{
					Tmp = 1;//
					Title[nTmp + 3] = pNumber->nType;
				}
				nTitleLength = (UINT8)(nTitleLength + Tmp);
				
				Title[nTmp + 3 + Tmp + 0] = pInfo->pid;
				Title[nTmp + 3 + Tmp + 1] = pInfo->dcs;			
				Title[nTmp + 3 + Tmp + 2] = pInfo->vp;
				
				nTitleLength = nTitleLength + 3;
				
				if(pInfo->dcs == 0x00)
				{
					//
					//0 GSM 7 bit default alphabet and uncompressed 
					//
					
					//
					//8 bits to 7 bits
					//				
					//7bit maximum length of the short message within the TP-UD field 
					//is 153 (160 - 7);
					//
					UINT8 nSMSNumber=0x0,uTmp = 1,nLen=0x0,pMRTmp=0x0;//超长短信的分发条数
					UINT8* pOut=NULL;
					UINT8* pOutTmp=NULL;
					UINT8* pUDTmp=NULL;
					UINT32 *nP2=NULL;
					UINT16 nCurrentLen=0x0;
					
					//
					//如果不能被最大数（152）整除，则需要多加一条来发最后一条短信。
					//计算分发为多少条短信
					//
					if(nDataSize % CFW_SMS_LEN_7BIT_LONG)
					{
						nSMSNumber = (UINT8)(nDataSize / CFW_SMS_LEN_7BIT_LONG + 1);
					}
					else
					{
						nSMSNumber = (UINT8)(nDataSize / CFW_SMS_LEN_7BIT_LONG) ;
					}
					
					//
					//nP2用来表针第二个参数pEvent->nParam2 为一个32位数组
					//
					nP2  = (UINT32 *) CSW_SMS_MALLOC(nSMSNumber * sizeof(UINT32));
					//nSMSNumber * sizeof(UINT32)
					//
					//pOut用来表针第一个参数pEvent->nParam1 为一个api_SmsPPSendReq_t结构体数组
					//
					pOut = (UINT8 *)CSW_SMS_MALLOC( 
						nSMSNumber * 176);	
					
					if((pOut == NULL) || (nP2 == (UINT32*)NULL))
					{
						CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU 8 nP2 pOut  malloc error!!! \n ");
						CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
						return(ERR_NO_MORE_MEMORY);
					}
					
					SUL_ZeroMemory8(pOut,nSMSNumber * 176);
					SUL_ZeroMemory8(nP2,nSMSNumber * sizeof(UINT32));
					
					pUDTmp  = pData;
					pOutTmp = pOut;
					pMRTmp  = pInfo->mr;
					nCurrentLen = nDataSize;
					
					while(uTmp <= nSMSNumber)
					{	
						UINT8* pPostNode =NULL;
						//UINT8 nUDTotL;
						pPostNode = (UINT8*)pOutTmp;

						Title[2 + nSCANumber[0] + 1] = pMRTmp;//xueww[mod]2007.07.27
						SUL_MemCopy8(pPostNode,Title,nTitleLength);
						//
						//UDHL
						//
						*(pPostNode + 1 + nTitleLength) = 0x05;
						//
						//IEI
						//
						*(pPostNode + 2 + nTitleLength) = 0x00;
						//
						//IEIDL
						//
						*(pPostNode + 3 + nTitleLength) = 0x03;
						//
						//Concatenated short message reference number
						//这里协议3GPP TS 23.040 -->9.2.3.24.1 
						//解释为２５６的模，这里取ＭＲ，
						//
						*(pPostNode + 4 + nTitleLength) = pInfo->mr;
						//
						//total number of SMS
						//
						*(pPostNode + 5 + nTitleLength) = nSMSNumber;
						//
						//current number of SMS
						//
						*(pPostNode + 6 + nTitleLength) = uTmp;
	          TS_OutputText(CFW_SMS_TS_ID, TSTXT("Send Long SMS Total[%d] Current[%d] MR[%d]\n"),nSMSNumber,uTmp,pInfo->mr);
						
						//*(pPostNode + 7 + nTitleLength) = 0xFF;
						if(nCurrentLen > CFW_SMS_LEN_7BIT_LONG)
						{
							*(pPostNode + 7 + nTitleLength) = *pUDTmp<<1;
							nLen = (UINT8)SUL_Encode7Bit((pUDTmp+1),(pPostNode + 8 + nTitleLength),CFW_SMS_LEN_7BIT_LONG - 1);
							pUDTmp = pUDTmp + CFW_SMS_LEN_7BIT_LONG;
							pOutTmp = pOutTmp + 176;
							nCurrentLen = nCurrentLen - CFW_SMS_LEN_7BIT_LONG;
							nP2[uTmp - 1] = 176 + (pMRTmp << 8) + 
								(nSMSNumber << 16) + (uTmp << 24);
							*(pPostNode + nTitleLength) =0xA0 ;
							//**********************************************************************
							//   high													low
							// 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1
							//  current number   total number           MR				Length
							//**********************************************************************
							//
							//add by lxp at 2006/05/15
							//For the sending  7bit SMS
							//
							//nLen = 152;							
							
						}
						else 
						{
							*(pPostNode + 7 + nTitleLength) = *pUDTmp<<1;
							nLen = (UINT8)SUL_Encode7Bit((pUDTmp+1),(pPostNode + 8 + nTitleLength),nCurrentLen -1);
							nP2[uTmp - 1] = 176 + (pMRTmp << 8) + \
								(nSMSNumber << 16) + (uTmp << 24);
							*(pPostNode + nTitleLength) =8 +nCurrentLen -1;
							//
							//add by lxp at 2006/05/15
							//For the sending  7bit SMS
							//
							//nLen = (UINT8)nCurrentLen;		
						}
						//*(pPostNode->Data + 6 + nLength) =(UINT8)(nP2[nTmp - 1] & 0xff);
						//*(pPostNode + nTitleLength) =nLen + 6 ;
						uTmp   ++;
						pMRTmp ++;
						//pPostNode->DataLen = 7 + nTitleLength + nLen;
					}
					CSW_SMS_FREE(Title);
					pEvent->nType    = 0x01;
					pEvent->nParam2  = (UINT32)nP2; 			
					pEvent->nParam1  = (UINT32)pOut;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);				
					return(ERR_SUCCESS);
				}
				else if(pInfo->dcs ==32)
				{
					//
					//32 GSM 7 bit default alphabet and compressed 
					//
					CSW_SMS_FREE(Title);
					
					pEvent->nType    = 0xF0;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)NULL;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);				
					return(ERR_INVALID_PARAMETER);
				}
//				else if(pInfo->dcs ==4)
//				{
//					//
//					//4 8 bit data and uncompressed 
//					//	
//					CSW_SMS_FREE(Title);
//					
//					pEvent->nType    = 0xF0;			
//					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
//					pEvent->nParam1  = (UINT32)NULL;
//					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
//					
//					return(ERR_INVALID_PARAMETER);
//				}
				else if(pInfo->dcs ==36)
				{
					//
					//36 8 bit data and compressed 
					//			
					CSW_SMS_FREE(Title);
					
					pEvent->nType    = 0xF0;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)NULL;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);					
					return(ERR_INVALID_PARAMETER);
				}
				else if((pInfo->dcs ==8) || (pInfo->dcs ==4))
				{
					//
					//8 UCS2 (16bit) and uncompressed 
					//
					
					UINT8 nSMSNumber=0x0,uTmp = 1,nLen=0x0,pMRTmp=0x0;//超长短信的分发条数
					UINT8* pOut=NULL;
					UINT8* pOutTmp=NULL;
					UINT8* pUDTmp=NULL;
					UINT32 *nP2=NULL;
					UINT16 nCurrentLen=0x0;
					

					//
					//如果不能被最大数（134）整除，则需要多加一条来发最后一条短信。
					//计算分发为多少条短信
					//
					if(nDataSize % CFW_SMS_LEN_UCS2_LONG)
					{
						nSMSNumber = (UINT8)(nDataSize / CFW_SMS_LEN_UCS2_LONG + 1);
					}
					else
					{
						nSMSNumber = (UINT8)(nDataSize / CFW_SMS_LEN_UCS2_LONG) ;
					}
					
					//
					//nP2用来表针第二个参数pEvent->nParam2 为一个32位数组
					//
					nP2  = (UINT32 *) CSW_SMS_MALLOC(nSMSNumber * sizeof(UINT32));
					
					//
					//pOut用来表针第一个参数pEvent->nParam1 为一个api_SmsPPSendReq_t结构体数组
					//
					pOut = (UINT8 *)CSW_SMS_MALLOC( 
						nSMSNumber * 176);	
					
					if((pOut == NULL) || (nP2 == (UINT32*)NULL))
					{
						CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU 9 nP2 pOut  malloc error!!! \n ");
						CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
						return(ERR_NO_MORE_MEMORY);
					}
					
					SUL_ZeroMemory8(pOut,nSMSNumber * 176);
					SUL_ZeroMemory8(nP2,nSMSNumber * sizeof(UINT32));
					
					pUDTmp  = pData;
					pOutTmp = pOut;
					pMRTmp  = pInfo->mr;
					nCurrentLen = nDataSize;
					
					while(uTmp <= nSMSNumber)
					{	
						UINT8* pPostNode =NULL;
						//UINT8 nUDTotL;
						pPostNode = (UINT8*)pOutTmp;

						Title[2 + nSCANumber[0] + 1] = pMRTmp;//xueww[mod] 2007.07.27
						SUL_MemCopy8(pPostNode,Title,nTitleLength);
						//
						//UDHL
						//
						*(pPostNode + 1 + nTitleLength) = 0x05;
						//
						//IEI
						//
						*(pPostNode + 2 + nTitleLength) = 0x00;
						//
						//IEIDL
						//
						*(pPostNode + 3 + nTitleLength) = 0x03;
						//
						//Concatenated short message reference number
						//这里协议3GPP TS 23.040 -->9.2.3.24.1 
						//解释为２５６的模，这里取ＭＲ，
						//
						*(pPostNode + 4 + nTitleLength) = pInfo->mr;
						//
						//total number of SMS
						//
						*(pPostNode + 5 + nTitleLength) = nSMSNumber;
						//
						//current number of SMS
						//
						*(pPostNode + 6 + nTitleLength) = uTmp;
	          TS_OutputText(CFW_SMS_TS_ID, TSTXT("Send Long SMS Total[%d] Current[%d] MR[%d]\n"),nSMSNumber,nTmp,pInfo->mr);
         

						if(nCurrentLen > CFW_SMS_LEN_UCS2_LONG)//xueww[mod] 2007.07.06 /152->134
						{
							SUL_MemCopy8((pPostNode + 7 + nTitleLength),pUDTmp,CFW_SMS_LEN_UCS2_LONG);
							nLen =CFW_SMS_LEN_UCS2_LONG;
							pUDTmp = pUDTmp + CFW_SMS_LEN_UCS2_LONG;
							pOutTmp = pOutTmp + 176;
							nCurrentLen = nCurrentLen - CFW_SMS_LEN_UCS2_LONG;
							nP2[uTmp - 1] = 176 + (pMRTmp << 8) + 
								(nSMSNumber << 16) + (uTmp << 24);
							
							//**********************************************************************
							//   high													low
							// 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1
							//  current number   total number           MR				Length
							//**********************************************************************
							
							
						}
						else 
						{
							SUL_MemCopy8((pPostNode + 7 + nTitleLength),pUDTmp,nCurrentLen);
							nLen = (UINT8)nCurrentLen;
							nP2[uTmp - 1] = 176 + (pMRTmp << 8) + \
								(nSMSNumber << 16) + (uTmp << 24);
						}
						//*(pPostNode->Data + 6 + nLength) =(UINT8)(nP2[nTmp - 1] & 0xff);
						*(pPostNode + nTitleLength) =nLen + 6 ;
						uTmp   ++;
						pMRTmp ++;
						//pPostNode->DataLen = 6 + nTitleLength + nLen;
					}
					CSW_SMS_FREE(Title);
					pEvent->nType    = 0x01;
					pEvent->nParam2  = (UINT32)nP2; 			
					pEvent->nParam1  = (UINT32)pOut;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
					/*
					PDU[nTmp + 3 + Tmp + 3] =  (UINT8)(nDataSize);
					SUL_MemCopy8((PDU+ nTmp + 3 + Tmp + 4),pData,nDataSize);
					
					pEvent->nType    = 0x00;				
					pEvent->nParam2  = (1<<16) + pInfo->mr; 
					pEvent->nParam1  = (UINT32)PDU;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP; 
					*/
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
					return(ERR_SUCCESS);
				}
				else if(pInfo->dcs ==40)
				{
					//
					//40 UCS2 (16bit) and compressed 
					//
					CSW_SMS_FREE(Title);
					
					pEvent->nType    = 0xF0;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)NULL;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);			
					return(ERR_INVALID_PARAMETER);
				}
			}
			else if((Concat & 0x03) == 0x00)
			{
				//
				//SMS DELIVER (in the direction SC to MS) 
				//
				
				UINT8  *Title = NULL;
				UINT8  nTmp=0x0,Tmp=0x0,SCTS[7];
				UINT32 nTime=0x0 ,nTitleLength=0x0;
				TM_FILETIME nFileTime;
				TM_SYSTEMTIME SystemTime,* pSystemTime;
				TM_SMS_TIME_STAMP SMSTime;
				Title = (UINT8 *)CSW_SMS_MALLOC(1 + 12 + 1 + 1 + 14 + 1 + 1 + 7);
				if(Title == NULL)
				{
					CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU 9 tile malloc error!!! \n ");
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
					return(ERR_NO_MORE_MEMORY);
				}
				
				SUL_ZeroMemory8(Title,(1 + 12 + 1 + 1 + 14 + 1 + 1 + 7));
				pSystemTime = &SystemTime;
				
				Title[0] = SMSStatus;
				nTitleLength = 1;

				if(nSCANumber[0] == 0x00)
				{
			          CSW_SMS_FREE(Title);
			          CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
			          return ERR_INVALID_PARAMETER;
				}
				else
				{
					Title[1] = nSCANumber[0];
					SUL_MemCopy8(&(Title[2]),(&(nSCANumber[1])),(nSCANumber[0]));
					
					nTmp = (UINT8)(2 + nSCANumber[0]);			
				}
				nTitleLength = (UINT8)(nTitleLength + nSCANumber[0] + 1);

				Title[nTmp + 0] = 0x00 | ((0x02) << 3) | ((pInfo->ssr) << 5) | ((pInfo->rp) << 7)| ((UINT8)(0x01 << 6));
				Title[nTmp + 1] = nDialNumberSize;
				nTitleLength = (UINT8)(nTitleLength + 2);
				
				if(nDialNumberSize != 0x00)
				{
					Tmp = nDialNumberSize;
					if((Tmp / 2) == ((Tmp + 1) / 2))
					{
						Tmp = (UINT8)(Tmp / 2);
					}
					else
					{
						Tmp = (UINT8)(Tmp / 2 + 1);
					}
					Tmp ++;
					SUL_MemCopy8((Title + nTmp + 2),pBCD,Tmp);
					CSW_SMS_FREE(pBCD);
				}
				else
				{
					Tmp = 1;//
					Title[nTmp + 2] =pNumber->nType;
				}				
				nTitleLength = nTitleLength + Tmp;

				Title[nTmp + 2 + Tmp + 0] = pInfo->pid;
				Title[nTmp + 2 + Tmp + 1] = pInfo->dcs;
				nTitleLength = nTitleLength + 2;

				nTime = TM_GetTime();
				nFileTime.DateTime = nTime;
				TM_FileTimeToSystemTime(nFileTime,pSystemTime);
				
				SMSTime.iZone  = TM_GetTimeZone();
				SMSTime.uYear  = pSystemTime->uYear;
				SMSTime.uMonth = pSystemTime->uMonth;
				SMSTime.uDay   = pSystemTime->uDay;
				SMSTime.uHour  = pSystemTime->uHour;
				SMSTime.uMinute= pSystemTime->uMinute;
				SMSTime.uSecond= pSystemTime->uSecond;
				
				sms_tool_SMSTime_TO_SCTS(SMSTime,SCTS);
				
				SUL_MemCopy8(&(Title[nTmp + 2 + Tmp + 2]),SCTS,7);				
				nTitleLength = nTitleLength + 7;

				if(pInfo->dcs == 0x00)
				{
					//
					//0 GSM 7 bit default alphabet and uncompressed 
					//
					
					//
					//8 bits to 7 bits
					//				
					//7bit maximum length of the short message within the TP-UD field 
					//is 152 (160 - 8);
					//
					UINT8 nSMSNumber,uTmp = 1,nLen,pMRTmp;//超长短信的分发条数
					UINT8 *pOut,*pOutTmp,*pUDTmp;
					UINT32 *nP2;
					UINT16 nCurrentLen;
					
					//
					//如果不能被最大数（153）整除，则需要多加一条来发最后一条短信。
					//计算分发为多少条短信
					//
					if(nDataSize % CFW_SMS_LEN_7BIT_LONG)
					{
						nSMSNumber = (UINT8)(nDataSize / CFW_SMS_LEN_7BIT_LONG + 1);
					}
					else
					{
						nSMSNumber = (UINT8)(nDataSize / CFW_SMS_LEN_7BIT_LONG) ;
					}
					
					//
					//nP2用来表针第二个参数pEvent->nParam2 为一个32位数组
					//
					nP2  = (UINT32 *) CSW_SMS_MALLOC(nSMSNumber * sizeof(UINT32));
					//nSMSNumber * sizeof(UINT32)
					//
					//pOut用来表针第一个参数pEvent->nParam1 为一个api_SmsPPSendReq_t结构体数组
					//
					pOut = (UINT8 *)CSW_SMS_MALLOC( 
						nSMSNumber * 176);	
					
					if((pOut == NULL) || (nP2 == (UINT32*)NULL))
					{
						CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU 10 nP2 pOut  malloc error!!! \n ");
						CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
						return(ERR_NO_MORE_MEMORY);
					}
					
					SUL_ZeroMemory8(pOut,nSMSNumber * 176);
					SUL_ZeroMemory8(nP2,nSMSNumber * sizeof(UINT32));
					
					pUDTmp  = pData;
					pOutTmp = pOut;
					pMRTmp  = pInfo->mr;
					nCurrentLen = nDataSize;
					
					while(uTmp <= nSMSNumber)
					{	
						UINT8* pPostNode =NULL;
						//UINT8 nUDTotL;
						pPostNode = (UINT8*)pOutTmp;
						//Title[1] = pMRTmp;//xueww[mod] 2007.07.10
						SUL_MemCopy8(pPostNode,Title,nTitleLength);
						//
						//UDHL
						//
						*(pPostNode + 1 + nTitleLength) = 0x05;
						//
						//IEI
						//
						*(pPostNode + 2 + nTitleLength) = 0x00;
						//
						//IEIDL
						//
						*(pPostNode + 3 + nTitleLength) = 0x03;
						//
						//Concatenated short message reference number
						//这里协议3GPP TS 23.040 -->9.2.3.24.1 
						//解释为２５６的模，这里取ＭＲ，
						//
						*(pPostNode + 4 + nTitleLength) = pInfo->mr;
						//*(pPostNode + 5 + nTitleLength) = pInfo->mr;
						//
						//total number of SMS
						//
						*(pPostNode + 5 + nTitleLength) = nSMSNumber;
						//
						//current number of SMS
						//
						*(pPostNode + 6 + nTitleLength) = uTmp;
	          TS_OutputText(CFW_SMS_TS_ID, TSTXT("Send Long SMS Total[%d] Current[%d] MR[%d]\n"),nSMSNumber,nTmp,pInfo->mr);
						
						//*(pPostNode + 7 + nTitleLength) = 0xFF;
						if(nCurrentLen > CFW_SMS_LEN_7BIT_LONG)
						{
							*(pPostNode + 7 + nTitleLength) = *pUDTmp<<1;
							nLen = (UINT8)SUL_Encode7Bit((pUDTmp+1),(pPostNode + 8 + nTitleLength), CFW_SMS_LEN_7BIT_LONG - 1);
							pUDTmp = pUDTmp + CFW_SMS_LEN_7BIT_LONG;
							pOutTmp = pOutTmp + 176;
							nCurrentLen = (UINT16)(nCurrentLen - CFW_SMS_LEN_7BIT_LONG);
							nP2[uTmp - 1] = 176 + (pMRTmp << 8) + 
								(nSMSNumber << 16) + (uTmp << 24);
							
							//**********************************************************************
							//   high													low
							// 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1
							//  current number   total number           MR				Length
							//**********************************************************************
							*(pPostNode + nTitleLength) =0xA0;
							
						}
						else 
						{
							*(pPostNode + 7 + nTitleLength) = *pUDTmp<<1;
							nLen = (UINT8)SUL_Encode7Bit((pUDTmp+1),(pPostNode + 8 + nTitleLength),nCurrentLen);
							nP2[uTmp - 1] = 176 + (pMRTmp << 8) + \
								(nSMSNumber << 16) + (uTmp << 24);
							*(pPostNode + nTitleLength) = 8 + nCurrentLen - 1;
						}
						//*(pPostNode->Data + 6 + nLength) =(UINT8)(nP2[nTmp - 1] & 0xff);
						uTmp   ++;
						pMRTmp ++;
						//pPostNode->DataLen = 7 + nTitleLength + nLen;
					}
                    CSW_SMS_FREE(Title);
					pEvent->nType    = 0x01;
					pEvent->nParam2  = (UINT32)nP2; 			
					pEvent->nParam1  = (UINT32)pOut;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);				
					return(ERR_SUCCESS);
					//PDU[nTmp + 2 + Tmp + 9] = (UINT8)SUL_Encode7Bit(pData,(PDU+ nTmp + 2 + Tmp + 10),nDataSize);
					/*
					pEvent->nType    = 0x00;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)PDU;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					return(ERR_SUCCESS);*/
				}
				else if(pInfo->dcs ==32)
				{
					//
					//32 GSM 7 bit default alphabet and compressed 
					//
					CSW_SMS_FREE(Title);
					
					pEvent->nType    = 0xF0;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)NULL;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);					
					return(ERR_INVALID_PARAMETER);
				}
//				else if(pInfo->dcs ==4)
//				{
//					//
//					//4 8 bit data and uncompressed 
//					//	
//					CSW_SMS_FREE(Title);
//					
//					pEvent->nType    = 0xF0;			
//					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
//					pEvent->nParam1  = (UINT32)NULL;
//					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
//					
//					return(ERR_INVALID_PARAMETER);
//				}
				else if(pInfo->dcs ==36)
				{
					//
					//36 8 bit data and compressed 
					//			
					CSW_SMS_FREE(Title);
					
					pEvent->nType    = 0xF0;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)NULL;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);					
					return(ERR_INVALID_PARAMETER);
				}
				else if((pInfo->dcs ==8) || (pInfo->dcs ==4))
				{
					//
					//8 UCS2 (16bit) and uncompressed 
					//
					UINT8 nSMSNumber,uTmp = 1,nLen,pMRTmp;//超长短信的分发条数
					UINT8 *pOut,*pOutTmp,*pUDTmp;
					UINT32 *nP2;
					UINT16 nCurrentLen;
					
					//
					//如果不能被最大数（134）整除，则需要多加一条来发最后一条短信。
					//计算分发为多少条短信
					//
					if(nDataSize % CFW_SMS_LEN_UCS2_LONG)
					{
						nSMSNumber = (UINT8)(nDataSize / CFW_SMS_LEN_UCS2_LONG + 1);
					}
					else
					{
						nSMSNumber = (UINT8)(nDataSize / CFW_SMS_LEN_UCS2_LONG) ;
					}
					
					//
					//nP2用来表针第二个参数pEvent->nParam2 为一个32位数组
					//
					nP2  = (UINT32 *) CSW_SMS_MALLOC(nSMSNumber * sizeof(UINT32));
					
					//
					//pOut用来表针第一个参数pEvent->nParam1 为一个api_SmsPPSendReq_t结构体数组
					//
					pOut = (UINT8 *)CSW_SMS_MALLOC( 
						nSMSNumber * 176);	
					
					if((pOut == NULL) || (nP2 == (UINT32*)NULL))
					{
						CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_ComposePDU 11 nP2 pOut  malloc error!!! \n ");
						CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
						return(ERR_NO_MORE_MEMORY);
					}
					
					SUL_ZeroMemory8(pOut,nSMSNumber * 176);
					SUL_ZeroMemory8(nP2,nSMSNumber * sizeof(UINT32));
					
					pUDTmp  = pData;
					pOutTmp = pOut;
					pMRTmp  = pInfo->mr;
					nCurrentLen = nDataSize;
					
					while(uTmp <= nSMSNumber)
					{	
						UINT8* pPostNode =NULL;
						//UINT8 nUDTotL;
						pPostNode = (UINT8*)pOutTmp;
						//Title[1] = pMRTmp;//xueww[mod] 2007.07.10
						SUL_MemCopy8(pPostNode,Title,nTitleLength);
						//
						//UDHL
						//
						*(pPostNode + 1 + nTitleLength) = 0x05;
						//
						//IEI
						//
						*(pPostNode + 2 + nTitleLength) = 0x00;
						//
						//IEIDL
						//
						*(pPostNode + 3 + nTitleLength) = 0x03;
						//
						//Concatenated short message reference number
						//这里协议3GPP TS 23.040 -->9.2.3.24.1 
						//解释为２５６的模，这里取ＭＲ，
						//
						*(pPostNode + 4 + nTitleLength) = pInfo->mr;
						//
						//total number of SMS
						//
						*(pPostNode + 5 + nTitleLength) = nSMSNumber;
						//
						//current number of SMS
						//
						*(pPostNode + 6 + nTitleLength) = uTmp;
						
	          TS_OutputText(CFW_SMS_TS_ID, TSTXT("Send Long SMS Total[%d] Current[%d] MR[%d]\n"),nSMSNumber,nTmp,pInfo->mr);
						if(nCurrentLen > CFW_SMS_LEN_UCS2_LONG)
						{
							SUL_MemCopy8((pPostNode + 7 + nTitleLength),pUDTmp,CFW_SMS_LEN_UCS2_LONG);
							nLen =CFW_SMS_LEN_UCS2_LONG;
							pUDTmp = pUDTmp + CFW_SMS_LEN_UCS2_LONG;
							pOutTmp = pOutTmp + 176;
							nCurrentLen = (UINT16)(nCurrentLen - CFW_SMS_LEN_UCS2_LONG);
							nP2[uTmp - 1] = 176 + (pMRTmp << 8) + 
								(nSMSNumber << 16) + (uTmp << 24);
							
							//**********************************************************************
							//   high													low
							// 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1 ~ 8 7 6 5 4 3 2 1
							//  current number   total number           MR				Length
							//**********************************************************************
							
							
						}
						else 
						{
							SUL_MemCopy8((pPostNode + 7 + nTitleLength),pUDTmp,nCurrentLen);
							nLen =(UINT8)nCurrentLen;
							nP2[uTmp - 1] = 176 + (pMRTmp << 8) + \
								(nSMSNumber << 16) + (uTmp << 24);
						}
						//*(pPostNode->Data + 6 + nLength) =(UINT8)(nP2[nTmp - 1] & 0xff);
						*(pPostNode + nTitleLength) =nLen + 6 ;
						uTmp   ++;
						pMRTmp ++;
						//pPostNode->DataLen = 6 + nTitleLength + nLen;
					}
					CSW_SMS_FREE(Title);
					pEvent->nType    = 0x01;
					pEvent->nParam2  = (UINT32)nP2; 			
					pEvent->nParam1  = (UINT32)pOut;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
					/*
					PDU[nTmp + 2 + Tmp + 9] =  (UINT8)(nDataSize);
					SUL_MemCopy8((PDU+ nTmp + 2 + Tmp + 10),pData,nDataSize);
					
					pEvent->nType    = 0x00;				
					pEvent->nParam2  = (1<<16) + pInfo->mr; 
					pEvent->nParam1  = (UINT32)PDU;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;
					
					return(ERR_SUCCESS);*/
				}
				else if(pInfo->dcs ==40)
				{
					//
					//40 UCS2 (16bit) and compressed 
					//
					CSW_SMS_FREE(Title);
					
					pEvent->nType    = 0xF0;			
					pEvent->nParam2  = (1<<16) + pInfo->mr; 	
					pEvent->nParam1  = (UINT32)NULL;
					pEvent->nEventId = EV_CFW_SIM_COMPOSE_PDU_RSP;	
					
					CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);					
					return(ERR_INVALID_PARAMETER);
				}
				
			}
			else
			{
				//
				//Invild type
				//
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
				return(ERR_INVALID_PARAMETER);
			}		
			
		}
		else
		{
			CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
			return(ERR_INVALID_PARAMETER);
		}
	}	
	else
	{
		//
		//短信长度超过允许最大长度，255 * 134
		//
		CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
		return(ERR_INVALID_PARAMETER);
	}
	
	CSW_PROFILE_FUNCTION_EXIT(sms_tool_ComposePDU);
	return ERR_SUCCESS;
}


UINT32 sms_tool_DecomposePDU(
						PVOID   pInData,    //176 length PDU date
						UINT8   nType,			
						UINT32* pOutData,
						UINT8 * pSMSStatus, //SMS Status just as below
						CFW_SMS_MULTIPART_INFO* pLongerMsg
						)
{
	//***************************************************************
	//Change list:
	//				为了满足ME上存储的需求，既将submit短消息保存为deliver
	//				类型，解析PDU的时候，完全按照用户指定参数nType来
	//				实现，而不在根据MTI来判断。
	//
	//***************************************************************
	UINT8 tmp,*pbuf,nMTI,nScaIndex = 0x00; 
	CSW_PROFILE_FUNCTION_ENTER(sms_tool_DecomposePDU);

	//	
	//Output the pSMSStatus.
	//
	pbuf = (UINT8 *)pInData;
	//	switch(pbuf[0]) 
        CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU, pbuf[0] & 0x1F =0x%x \n ", pbuf[0] & 0x1F );
	switch(pbuf[0] & 0x1F  ) 
	{
		case 0x03:
			*pSMSStatus = 0x01;
			break;
		case 0x01:
			*pSMSStatus = 0x02;
			break;
		case 0x07:
			*pSMSStatus = 0x04;
			break;
		case 0x05:
			*pSMSStatus = 0x08;
			break;
		case 0x0D:
			*pSMSStatus = 0x10;
			break;
		case 0x15:
			*pSMSStatus = 0x20;
			break;
		case 0x1D:
			*pSMSStatus = 0x40;
			break;
		case 0xA0:
			*pSMSStatus = 0x80;
			break;
		default:
			*pSMSStatus = 0x00;
	}	
	if(nType == 0)
	{
		//
		//Only get the SMS status
		//
		pOutData = NULL;
		CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
		return(ERR_SUCCESS);
	}
	//	if(pLongerMsg == NULL)
	//	{
	//		return(ERR_INVALID_PARAMETER);
	//	}
	//	else
	if(pLongerMsg)
	{
		pLongerMsg->count   = 0;
		pLongerMsg->current = 0;
		pLongerMsg->id      = 0;
	}

	//
	//Necessary
	//
	if(pOutData == NULL)
	{
	return ERR_SUCCESS;
	}

	//
	//Get MTI value 
	//
	if(pbuf[1] == 0x00)
	{
		tmp = 2;//xueww[mod] 2007.08.10 2 + 1-->2
		nMTI = pbuf[tmp];	
	}
	  else if (pbuf[1] > TEL_NUMBER_MAX_LEN)
	  {
	    return ERR_INVALID_PARAMETER;
	  }
	else
	{
		nScaIndex = 0;
		tmp = (UINT8)(2 + pbuf[1]);
		nMTI = pbuf[tmp];//hameina
	}
	
	if(nMTI & 0x02)
	{                     //Don't support Commond &status Report
		CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
		return  ERR_INVALID_PARAMETER;
	}

	if(nMTI & 0x01)  // For Submit, there will be not read or unread statue
	{
		if((*pSMSStatus == 0x01) || (*pSMSStatus == 0x02))
		{
			CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
			return  ERR_INVALID_PARAMETER;
		}
	}
	else if(nMTI & 0x00) // Fpr DELIVER,  there will be not send or unsend statue
	{
		if((*pSMSStatus == 0x04) || (*pSMSStatus == 0x08) || (*pSMSStatus == 0x10) ||\
			(*pSMSStatus == 0x20) || (*pSMSStatus == 0x40))
		{
			CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
			return  ERR_INVALID_PARAMETER;
		}
	}


	if(nType == 0x01 || nType == 0x02)
	{
		//
		//DELIVER message See 3GPP TP 23040-390 9.2.3.1
		//
		UINT8 oa [TEL_NUMBER_MAX_LEN];
		UINT8 sca[TEL_NUMBER_MAX_LEN];
		UINT8 data[CFW_SMS_LEN_7BIT_NOMAL];
		UINT8 nTime[14];
		UINT8 oa_size = 0;
		UINT8 sca_size = 0;
		//TM_SMS_TIME_STAMP scts;
		UINT8 tooa=0x0;
		UINT8 tosca=0x0;
		//UINT8 stat;
		//UINT8 fo;
		UINT8 pid=0x0;
		UINT8 dcs=0x0;
		UINT16 length=0x0;
		UINT8 nTmp=0x0;
		UINT32 ret =0x0;//xueww[+] 2007.08.21

		SUL_ZeroMemory8(data,CFW_SMS_LEN_7BIT_NOMAL);
		SUL_ZeroMemory8(nTime,14);
		SUL_ZeroMemory8(oa, TEL_NUMBER_MAX_LEN);
	    SUL_ZeroMemory8(sca, TEL_NUMBER_MAX_LEN);
	    if (pbuf[1] > TEL_NUMBER_MAX_LEN)
	    {
	      return ERR_INVALID_PARAMETER;
	    }
		if(pbuf[1] != 0x00)
		{
			//
			//Get the SCA Address
			//
			//SUL_GSMBCDToAscii(&(pbuf[3]),sca,(UINT8)(pbuf[1] - 1));
			SUL_MemCopy8(sca,&(pbuf[nScaIndex + 3]),(pbuf[1] - 1));
			sca_size = (UINT8)(pbuf[1] - 1);
			//
			//Get the type of SMS Center Address 
			//
			tosca = pbuf[2];
			tmp = (UINT8)(pbuf[1] + 3);
		}
		else
		{
			tmp = (UINT8)(pbuf[1] + 3);//xueww[mod] 2007.08.10 4--->3
		}

		//
		//Get the Originating telephone number 
		//							
    if (pbuf[tmp] > 2* TEL_NUMBER_MAX_LEN)
    {
      return ERR_INVALID_PARAMETER;
    }
		nTmp = pbuf[tmp];

		if(nTmp != 0x00)
		{
			if(nTmp & 1) nTmp += 1; //

			nTmp = (UINT8)(nTmp / 2) ;
			//SUL_GSMBCDToAscii(&(pbuf[tmp + 2]),oa,nTmp);
			SUL_MemCopy8(oa,&(pbuf[tmp + 2]),nTmp);
			oa_size = nTmp;
			//
			//Get the type of the Originating telephone number 
			//
			tooa = pbuf[tmp +1];
			tmp = (UINT8)(tmp + 2 + nTmp);
		}
		else
		{
			tooa = pbuf[tmp +1];
			tmp = (UINT8)(tmp + 2);
		}		

		//
		//Get the PID
		//
		pid = pbuf[tmp];

		//
		//Get the DCS
		//
		dcs = pbuf[tmp + 1];

		CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU,before sms_tool_CheckDcs, dcs = 0x%x \n ", dcs);

		//[[xueww[+] 2007.08.21
		ret = sms_tool_CheckDcs(&dcs);

		CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU,after sms_tool_CheckDcs, dcs = 0x%x \n ", dcs);
		
		if(ERR_SUCCESS != ret)
		{
			CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
			return  ret;
		}
		//]]xueww[+] 2007.08.21
		
		//
		//Get the SCTS
		
		//hameina
    SUL_GsmBcdToAscii(&(pbuf[tmp + 2]),  6,  nTime);  
    
    
    INT8 tmpZone = 0;
    tmpZone =(pbuf[tmp + 8] & 0x07)*10 + (pbuf[tmp + 8]  >> 4);
    tmpZone = tmpZone > ((tmpZone/4)*4)?(tmpZone/4 + 1) : (tmpZone/4);
    if((pbuf[tmp + 8]  & 0x0f) >= 0x80)//decide the "-" / "+"
    {
      tmpZone= 0 - tmpZone;
    }

		CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU, 1, nTime[1] = 0x%x \n ", nTime[1] );
		CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU, 1, nTime[0] = 0x%x \n ", nTime[0] );
		CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU, 222, nMTI = 0x%x \n ", nMTI);
		//SUL_GSMBCDToAscii(&(pbuf[tmp + 2]),,7);

		//SUL_MemCopy8(oa,&(pbuf[tmp + 8 + nTmp]),7);

		//
		//Get the User date
		//

		//*********************************************************************
		//TP UDHI bit6  : x0xx xxxx: TP UDHI, for loner SMS
		//*********************************************************************
		if((nMTI & 0x40) == 0x00)
		{
			//
			//User date里面不包含头信息
			//
			if(dcs == GSM_7BIT)    
			{
				//
				// 7-bit解码
				//
				UINT16 nSrcLength=0x0;
				nSrcLength = (UINT16)(pbuf[tmp + 9] & 7 ? pbuf[tmp + 9] * 7 / 8 + 1 : pbuf[tmp + 9] * 7 / 8);
				//[[xueww[mod] 2007.03.21
				SUL_Decode7Bit(&(pbuf[tmp + 10]), data, nSrcLength);    // 转换到TP-DU

				length = pbuf[tmp + 9]; 
				//]]xueww[mod] 2007.03.21
			}
			else if(dcs == GSM_UCS2)
			{
				//
				// UCS2解码
				//
				SUL_MemCopy8(data,&(pbuf[tmp + 10]),pbuf[tmp + 9]);
				length = (UINT16)pbuf[tmp + 9];
			}
			else
			{
				//
				// 8-bit解码
				//
				SUL_MemCopy8(data,&(pbuf[tmp + 10]),pbuf[tmp + 9]);
				 length = (UINT16)pbuf[tmp + 9];
			}
		}
		else
		{
			//
			//User date里面包含头信息
			//
			if(dcs == GSM_7BIT)    
			{
				//
				// 7-bit解码
				//
				UINT8  nBodyLength = 0x00;
				UINT8  nUDHL = 0x00;
				UINT8  nUDL  = 0x00;
				UINT16 nSrcLength = 0x0000;
				UINT8  nTmpForDate[CFW_SMS_LEN_7BIT_NOMAL];

				SUL_ZeroMemory32(nTmpForDate, CFW_SMS_LEN_7BIT_NOMAL);

				nBodyLength = (UINT8)(pbuf[tmp + 9] - pbuf[tmp + 10] - 1);

				nUDL = pbuf[tmp + 9];
				nUDHL = pbuf[tmp + 10];

				if(pbuf[tmp + 10] == 0x00)
				{
					;	
				}
				else if(pbuf[tmp + 10] > 0x07) //Don't think about EMS, later.........
				{
					//[[xueww[-] 2007.07.30
					//TO DO......
					//CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
					//return  ERR_INVALID_PARAMETER;
					//]]xueww[-] 2007.07.30
				}
				else
				{
					if(pbuf[tmp + 11] == 0x00)
					{
						//
						//例如UDH的实际例子格式为05 00 03 81 04 03
						//
						pLongerMsg->id		= pbuf[tmp + 10 + 3];
						pLongerMsg->count	= pbuf[tmp + 10 + 4];
						pLongerMsg->current = pbuf[tmp + 10 + 5];						
					}
					//
					//Modify by lxp for decompose 16 reference SMS
					//
					else if(pbuf[tmp + 11] == 0x08)
					{
						//
						//例如UDH的实际例子格式为06 08 04 81 81 04 03
						//
						pLongerMsg->id = (UINT8)(((pbuf[tmp + 10 + 3])<<8)+ pbuf[tmp + 10 + 4]);
						pLongerMsg->count	= pbuf[tmp + 10 + 5];
						pLongerMsg->current = pbuf[tmp + 10 + 6];	
					}
					else
					{
						pLongerMsg->id		= pbuf[tmp + 10 + 1];
						//TODO ^.^
					}
				}

				//
				//Modify for long 7bit sms
				//
				//tmp = (UINT8)(tmp + pbuf[tmp + 10] + 1);

				//nSrcLength = (UINT16)(nBodyLength & 7 ? nBodyLength * 7 / 8 + 1 : nBodyLength * 7 / 8);
				nSrcLength = (UINT16)(nUDL & 7 ? (nUDL * 7 / 8 + 1 ):( nUDL * 7 / 8));

				//length = (UINT16)SUL_Decode7Bit(&(pbuf[tmp + 10]), data, nSrcLength);    // 转换到TP-DU

				//
				//超常7BIT解码，从UDH头开始解码，最后长度去掉UDHL+2，2的意思是UDHL本身和解码后增长的一个字节。
				//
				length = (UINT16)SUL_Decode7Bit(&(pbuf[tmp + 10]), nTmpForDate, nSrcLength);    // 转换到TP-DU
				if( nUDL > 2 )
				  SUL_MemCopy8(data, &(nTmpForDate[nUDHL + 2]), (length = nUDL - nUDHL - 2)); //xueww[mod] 2007.03.21 /length--->nUDL

			}
			else if(dcs == GSM_UCS2)
			{
				//
				// UCS2解码
				//
				UINT8 nBodyLength=0x0;

				nBodyLength = (UINT8)(pbuf[tmp + 9] - pbuf[tmp + 10] - 1);

				if(pbuf[tmp + 10] == 0x00)
				{
					;	
				}
				else if(pbuf[tmp + 10] > 0x07) //Don't think about EMS, later.........
				{
					//[[xueww[-] 2007.07.30
					//TO DO......
					//CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
					//return  ERR_INVALID_PARAMETER;
					//]]xueww[-] 2007.07.30
				}
				else
				{
					if(pbuf[tmp + 11] == 0x00)
					{
						//
						//例如UDH的实际例子格式为05 00 03 81 04 03
						//
						pLongerMsg->id		= pbuf[tmp + 10 + 3];
						pLongerMsg->count	= pbuf[tmp + 10 + 4];
						pLongerMsg->current = pbuf[tmp + 10 + 5];						
					}
					//
					//Modify by lxp for decompose 16 reference SMS
					//
					else if(pbuf[tmp + 11] == 0x08)
					{
						//
						//例如UDH的实际例子格式为06 08 04 81 81 04 03
						//
						pLongerMsg->id = (UINT8)(((pbuf[tmp + 10 + 3])<<8) + pbuf[tmp + 10 + 4]);
						pLongerMsg->count	= pbuf[tmp + 10 + 5];
						pLongerMsg->current = pbuf[tmp + 10 + 6];	
					}
					else
					{
						pLongerMsg->id		= pbuf[tmp + 10 + 1];
						//TODO ^.^
					}
				}

				tmp = (UINT8)(tmp + pbuf[tmp + 10] + 1);

				SUL_MemCopy8(data,&(pbuf[tmp + 10]),nBodyLength);
				length = (UINT16)nBodyLength;
			}
			else
			{
				//
				// 8-bit解码
				//
				UINT8 nBodyLength;

				nBodyLength = (UINT8)(pbuf[tmp + 9] - pbuf[tmp + 10] - 1);

				if(pbuf[tmp + 10] == 0x00)
				{
					;	
				}
				else if(pbuf[tmp + 10] > 0x07) //Don't think about EMS, later.........
				{
					//[[xueww[-] 2007.07.30
					//TO DO......
					//CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
					//return  ERR_INVALID_PARAMETER;
					//]]xueww[-] 2007.07.30
				}
				else
				{
					if(pbuf[tmp + 11] == 0x00)
					{
						//
						//例如UDH的实际例子格式为05 00 03 81 04 03
						//
						pLongerMsg->id		= pbuf[tmp + 10 + 3];
						pLongerMsg->count	= pbuf[tmp + 10 + 4];
						pLongerMsg->current = pbuf[tmp + 10 + 5];						
					}
					//
					//Modify by lxp for decompose 16 reference SMS
					//
					else if(pbuf[tmp + 11] == 0x08)
					{
						//
						//例如UDH的实际例子格式为06 08 04 81 81 04 03
						//
						pLongerMsg->id = (UINT8)(((pbuf[tmp + 10 + 3])<<8) + pbuf[tmp + 10 + 4]);
						pLongerMsg->count	= pbuf[tmp + 10 + 5];
						pLongerMsg->current = pbuf[tmp + 10 + 6];	
					}
					else
					{
						pLongerMsg->id		= pbuf[tmp + 10 + 1];
						//TODO ^.^
					}
				}
				tmp = (UINT8)(tmp + pbuf[tmp + 10] + 1);
				SUL_MemCopy8(data,&(pbuf[tmp + 10]),nBodyLength);
				length = (UINT16)nBodyLength;
			}
		}

		//[[xueww[+] 2007.04.09
		if(dcs == GSM_7BIT)   
		{
			if(length > CFW_SMS_LEN_7BIT_NOMAL)
			{
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
				return  ERR_INVALID_PARAMETER;
			}
		}
		else
		{
			if(length > CFW_SMS_LEN_UCS2_NOMAL)
			{
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
				return  ERR_INVALID_PARAMETER;
			}
		}		
		//]]xueww[+] 2007.04.09

		if(nType == 0x01)
		{
			//
			//CFW_SMS_TXT_DELIVERED_NO_HRD_INFO
			//
			CFW_SMS_NODE *pDEL_NH = (CFW_SMS_NODE *)CSW_SMS_MALLOC \
				(sizeof(CFW_SMS_TXT_DELIVERED_NO_HRD_INFO) + sizeof(CFW_SMS_NODE) + length);
			CFW_SMS_TXT_DELIVERED_NO_HRD_INFO * pTmp;
			*pOutData = (UINT32)pDEL_NH;
			SUL_ZeroMemory8(pDEL_NH,(sizeof(CFW_SMS_TXT_DELIVERED_NO_HRD_INFO) + \
				sizeof(CFW_SMS_NODE) +length));

			pDEL_NH->pNode = (PVOID)((UINT8*)pDEL_NH + sizeof(CFW_SMS_NODE));
			pTmp = (CFW_SMS_TXT_DELIVERED_NO_HRD_INFO *)(pDEL_NH->pNode);

			pTmp->stat = *pSMSStatus;
			pTmp->length = length;

			SUL_MemCopy8(pTmp->data,data,length);
			SUL_MemCopy8(pTmp->oa,oa,oa_size);
			pTmp->oa_size = oa_size;
			pTmp->scts.uYear   = (UINT16)(nTime[1] - 0x30 +  (nTime[0] - 0x30) * 10 + 2000);
			pTmp->scts.uMonth  = (UINT8)(nTime[3] - 0x30 +  (nTime[2] - 0x30) * 10);
			pTmp->scts.uDay    = (UINT8)(nTime[5] - 0x30 +  (nTime[4] - 0x30) * 10);
			pTmp->scts.uHour   = (UINT8)(nTime[7] - 0x30 +  (nTime[6] - 0x30) * 10);
			pTmp->scts.uMinute = (UINT8)(nTime[9] - 0x30 +  (nTime[8] - 0x30) * 10);
			pTmp->scts.uSecond = (UINT8)(nTime[11] - 0x30 + (nTime[10] - 0x30)* 10);

			pTmp->scts.iZone   = tmpZone;//add by wuys 008-06-18 for fixing Zone

			CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);		
			return(ERR_SUCCESS);
		}
		else if(nType == 0x02)	
		{
			if((*pSMSStatus == 0x01) || (*pSMSStatus == 0x02))
			{
				//
				//CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO
				//

				CFW_SMS_NODE *pDEL_H = (CFW_SMS_NODE *)CSW_SMS_MALLOC \
				(sizeof(CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO) + sizeof(CFW_SMS_NODE) + length);
				CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO * pTmp;
				*pOutData = (UINT32)pDEL_H;

				SUL_ZeroMemory8(pDEL_H,(sizeof(CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO) + \
				sizeof(CFW_SMS_NODE) +length));

				pDEL_H->pNode = (PVOID)((UINT8*)pDEL_H + sizeof(CFW_SMS_NODE));

				pTmp = (CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO *)(pDEL_H->pNode);

				pTmp->tooa   = tooa;
				pTmp->tosca  = tosca;
				//pTmp->fo     = nMTI;
				pTmp->fo   =17;
				pTmp->stat   = *pSMSStatus;
				pTmp->length = length;

				SUL_MemCopy8(pTmp->data,data,length);
				SUL_MemCopy8(pTmp->oa,oa,oa_size);
				SUL_MemCopy8(pTmp->sca,sca,sca_size);	
				pTmp->oa_size = oa_size;
				pTmp->sca_size = sca_size;
				
				CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU, 44, nTime[1] = 0x%x \n ", nTime[1] );
				CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU, 44, nTime[0] = 0x%x \n ", nTime[0] );

				pTmp->scts.uYear   = (UINT16)(nTime[1] - 0x30 + (nTime[0] - 0x30) * 10 + 2000);
				pTmp->scts.uMonth  = (UINT8)(nTime[3] - 0x30 + (nTime[2] - 0x30) * 10);
				pTmp->scts.uDay    = (UINT8)(nTime[5] - 0x30 + (nTime[4] - 0x30) * 10);
				pTmp->scts.uHour   = (UINT8)(nTime[7] - 0x30 + (nTime[6] - 0x30) * 10);
				pTmp->scts.uMinute = (UINT8)(nTime[9] - 0x30 + (nTime[8] - 0x30) * 10);
				pTmp->scts.uSecond = (UINT8)(nTime[11] - 0x30 + (nTime[10] - 0x30) * 10);
			  pTmp->scts.iZone   = tmpZone;//add by wuys 008-06-18 for fixing Zone

				//
				//Modify by lixp at 20060322
				//为了和文档同步，2.2.6  ^.^
				//
				//dcs = pbuf[tmp + 1];

				CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU,before translate, dcs = 0x%x \n ", dcs);

				if(dcs == GSM_7BIT_UNCOMPRESSED)
				{
					dcs = 0;
				}
				else if(dcs == GSM_8BIT_UNCOMPRESSED)
				{
					dcs = 1;
				}
				else if(dcs == GSM_UCS2_UNCOMPRESSED)
				{
					dcs = 2;				
				}
				else if(dcs == GSM_7BIT_COMPRESSED)
				{
					dcs = 0x10;
				}
				else if(dcs == GSM_8BIT_COMPRESSED)
				{
					dcs = 0x11;
				}
				else if(dcs == GSM_UCS2_COMPRESSED)
				{
					dcs = 0x12;				
				}

				pTmp->dcs =dcs;
				
				CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU, 222, pTmp->dcs = 0x%x \n ", pTmp->dcs);
				
				pTmp->pid = pid;

				CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);				
				return(ERR_SUCCESS);
			}
		}
	}
	else if(nType == 0x03 || nType == 0x04)
	{
		//
		//Decompose SUBMIT message See 3GPP TP 23040-390 9.2.3.1
		//
		UINT8 da [TEL_NUMBER_MAX_LEN];
		UINT8 sca[TEL_NUMBER_MAX_LEN];
		UINT8 data[CFW_SMS_LEN_7BIT_NOMAL];
		UINT8 da_size=0;
		UINT8 sca_size=0;
		UINT8 toda=0x0,tosca=0x0;
		UINT8 pid=0x0,dcs=0x0,nMR=0x0,VP=0x0;
		UINT8 nTmp=0x0,Tmp=0x0;
		UINT16 length=0x0;
		UINT32 ret =0x0;//xueww[+] 2007.08.21

		SUL_ZeroMemory8(data,CFW_SMS_LEN_7BIT_NOMAL);
		SUL_ZeroMemory8(da,TEL_NUMBER_MAX_LEN);
		SUL_ZeroMemory8(sca,TEL_NUMBER_MAX_LEN);
	       if (pbuf[1] > TEL_NUMBER_MAX_LEN)
	       {
		      return ERR_INVALID_PARAMETER;
	       }
		if(pbuf[1] != 0x00)
		{
			//
			//Get the SCA Address
			//
			//SUL_GSMBCDToAscii(&(pbuf[3]),sca,(UINT8)(pbuf[1] - 1));
			SUL_MemCopy8(sca,&(pbuf[3]),(pbuf[1] - 1));
			sca_size = (UINT8)(pbuf[1] - 1);
			//
			//Get the type of SMS Center Address just as 0x91
			//
			tosca = pbuf[2];
		}
		else
		{
			//tooa = pbuf[tmp +1];
			//tmp = tmp + 2;
			tosca = 0;//xueww[mod] 2007.08.10 pbuf[2];//
		}


		//
		//Get the MTI/VPF
		//
		//*********************************************************************
		//TP-MTI: bit1-0: xxxx xx01: SMS-SUBMIT(MS->SC)
		//		  0 0   : SMS DELIVER (in the direction SC to MS)	
		//  	  0 0   : SMS DELIVER REPORT (in the direction MS to SC)	
		//  	  1 0   : SMS STATUS REPORT (in the direction SC to MS)		
		//  	  1 0   : SMS COMMAND (in the direction MS to SC)	
		//  	  0 1   : SMS SUBMIT (in the direction MS to SC)	
		//  	  0 1   : SMS SUBMIT REPORT (in the direction SC to MS)	
		//  	  1 1   : Reserved
		//*********************************************************************

		//*********************************************************************
		//TP-RD:  bit2  : xxxx x0xx: 
		// 			0   : Instruct the SC to accept an SMS SUBMIT for an SM still held in the
		//					SC which has the same TP MR and the same TP DA as a 
		//                  previously 	submitted SM from 	the same OA.
		//			1   : Instruct the SC to reject an SMS SUBMIT for an SM still held in the
		//					SC which has the same TP MR and the same TP DA as the
		//                  previously submitted SM 	from the same OA. In this case 
		//				    the response returned by the SC is as specified in 9.2.3.6.
		//*********************************************************************

		//*********************************************************************
		//TP-VPF  bit4-3: xxx1 0xxx: 
		//        1 0   : TP-VP filed present-relative format
		//        0 1   : TP-VP filed present-enhanced format
		//        1 1   : TP-VP filed present-absolute format
		//        0 0   : TP-VP filed no present
		//*********************************************************************

		//*********************************************************************
		//TP SRR  bit5  : xx1x xxxx: TP SRR
		//			0   : A status report is not requested	
		//			1   : A status report is requested
		//*********************************************************************

		//*********************************************************************
		//TP UDHI bit6  : x0xx xxxx: TP UDHI, for loner SMS
		//*********************************************************************

		//*********************************************************************
		//TP-RP   bit7  : 0xxx xxxx: TP-RP
		//			0	: TP Reply Path parameter is not set in this SMS SUBMIT/DELIVER
		//			1 	: TP Reply Path parameter is set in this SMS SUBMIT/DELIVER
		//*********************************************************************
		if(pbuf[1] != 0x00)
		{
			Tmp = (UINT8)(pbuf[1] + 2);
		}
		else
		{
			//
			//考虑SCA为空TON保留
			//
			Tmp = 2;//xueww[mod] 2007.08.10 3--->2;
		}

		nMTI = pbuf[Tmp];
		Tmp++;

		//
		//Get the MR
		//
		nMR = pbuf[Tmp];		
		Tmp++;

		//
		//Get DA length
		//
		nTmp = pbuf[Tmp];

	    if (nTmp > 2* TEL_NUMBER_MAX_LEN)
	    {
	      return ERR_INVALID_PARAMETER;
	    }

		if(nTmp != 0x00)
		{
			if(nTmp & 1)
				nTmp += 1;
			nTmp = (UINT8)(nTmp / 2);

			//SUL_GSMBCDToAscii(&(pbuf[Tmp + 2]),da,nTmp);
			SUL_MemCopy8(da,&(pbuf[Tmp + 2]),nTmp);
			da_size = nTmp;
			//
			//Get the type of the DA 
			//
			toda = pbuf[Tmp +1];
			Tmp = (UINT8)(Tmp + 2 + nTmp);
		}
		else
		{
			toda = pbuf[Tmp +1];//hameina[mod]
			//toda = pbuf[Tmp];
			Tmp = (UINT8)(Tmp + 2);
		}		

		//
		//Get the PID
		//
		pid = pbuf[Tmp];

		//
		//Get the DCS
		//
		dcs = pbuf[Tmp + 1];

		//[[xueww[+] 2007.08.21
		ret = sms_tool_CheckDcs(&dcs);

		if(ERR_SUCCESS != ret)
		{
			CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
			return  ret;
		}
		//]]xueww[+] 2007.08.21

		//Verify there is VP or not, xxxY Yxxx bit3 and bit 4 
		if((nMTI & 0x18) == 0)  // xxx0 0xxx TP-VP filed no present
		{
			//no VP
			Tmp--;
		}
		else
		{
			//
			//Get the VP
			//
			VP = pbuf[Tmp + 2];

		}

		//
		//Get the User date
		//
		CSW_TRACE(CFW_SMS_TS_ID,"sms_tool_DecomposePDU, 1,nMTI=0x%x, dcs=%d\r\n",nMTI, dcs);
		//*********************************************************************
		//TP UDHI bit6  : x0xx xxxx: TP UDHI, for loner SMS
		//*********************************************************************
		if((nMTI & 0x40) == 0x00)
		{
			//
			//User date里面不包含头信息
			//
			if(dcs == GSM_7BIT)    
			{
				//
				// 7-bit解码
				//
				UINT16 nSrcLength=0x0;
				
				//xueww[mod] 2007.03.05 fix bug 3904
				//nSrcLength = pbuf[Tmp + 3]; //(UINT16)(pbuf[Tmp + 3] & 7 ? pbuf[Tmp + 3] * 7 / 8 + 1 : pbuf[Tmp + 3] * 7 / 8);
				nSrcLength = (UINT16)(pbuf[Tmp + 3] & 7 ? pbuf[Tmp + 3] * 7 / 8 + 1 : pbuf[Tmp + 3] * 7 / 8);


				//xueww[mod] 2007.03.22
				length = pbuf[Tmp + 3];
				if(length > CFW_SMS_LEN_7BIT_NOMAL)
				{
					CSW_TRACE(CFW_SMS_TS_ID,"sms_tool_DecomposePDU, 2,data size in pdu is wrong ,length=%d\r\n",length);
					return ERR_CMS_INVALID_CHARACTER_IN_PDU;
				}
				SUL_Decode7Bit(&(pbuf[Tmp + 4]), data, nSrcLength);    // 转换到TP-DU

				//xueww[mod] 2007.03.22
			}
			else if(dcs == GSM_UCS2)
			{
				//
				// UCS2解码
				//
				SUL_MemCopy8(data,&(pbuf[Tmp + 4]),pbuf[Tmp + 3]);
				length = (UINT16)pbuf[Tmp + 3];
			}
			else
			{
				//
				// 8-bit解码
				//
			        SUL_MemCopy8(data, &(pbuf[Tmp + 4]), pbuf[Tmp + 3]);
			        length = pbuf[Tmp + 3];
			}
		}	
		else
		{
			//
			//User date里面包含头信息
			//
			if(dcs == GSM_7BIT)    
			{
				//
				// 7-bit解码
				//
				UINT8  nBodyLength=0x0;
				UINT16 nSrcLength=0x0;
				UINT8  nUDHL = 0x00;
				UINT8  nUDL  = 0x00;
				UINT8 nTmpForDate[CFW_SMS_LEN_7BIT_NOMAL];
				SUL_ZeroMemory32(nTmpForDate, CFW_SMS_LEN_7BIT_NOMAL);                
				nBodyLength = (UINT8)(pbuf[Tmp + 3] - pbuf[Tmp + 4]  - 1);

				nUDHL = pbuf[Tmp + 4];
				nUDL = pbuf[Tmp + 3];

				if(pbuf[Tmp + 4] == 0x00)
				{
					;	
				}
				else if(pbuf[Tmp + 4] > 0x07) //Don't think about EMS, later.........
				{
					//[[xueww[-] 2007.07.30
					//TO DO......
					//CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
					//return  ERR_INVALID_PARAMETER;
					//]]xueww[-] 2007.07.30
				}
				else
				{
					if(pbuf[Tmp + 5] == 0x00)
					{
						//
						//例如UDH的实际例子格式为05 00 03 81 04 03
						//
						pLongerMsg->id		= pbuf[Tmp + 4 + 3];
						pLongerMsg->count	= pbuf[Tmp + 4 + 4];
						pLongerMsg->current = pbuf[Tmp + 4 + 5];						
					}
					//
					//Modify by lxp for decompose 16 reference SMS
					//
					else if(pbuf[Tmp + 5] == 0x08)
					{
						//
						//例如UDH的实际例子格式为06 08 04 81 81 04 03
						//
						pLongerMsg->id = (UINT8)(((pbuf[Tmp + 4 + 3])<<8) + pbuf[Tmp + 4 + 4]);
						pLongerMsg->count	= pbuf[Tmp + 4 + 5];
						pLongerMsg->current = pbuf[Tmp + 4 + 6];	
					}
					else
					{
						pLongerMsg->id		= pbuf[Tmp + 4 + 1];
						//TODO ^.^
					}
				}

				//Tmp = (UINT8)(Tmp + pbuf[Tmp + 4]  + 1);

				//nSrcLength = (UINT16)(nBodyLength & 7 ? nBodyLength * 7 / 8 + 1 : nBodyLength * 7 / 8);
				nSrcLength = (UINT16)(nUDL & 7 ? nUDL * 7 / 8 + 1 : nUDL * 7 / 8);

//hameina
				//length = (UINT16)SUL_Decode7Bit(&(pbuf[Tmp + 4]), data, nSrcLength);    // 转换到TP-DU
				length = (UINT16)SUL_Decode7Bit(&(pbuf[Tmp + 4]), nTmpForDate, nSrcLength);    // hameina[mod] 2007.3.22(Tmp+5->Tmp+4)

        if( nUDL > 2 )
          SUL_MemCopy8(data, &(nTmpForDate[nUDHL + 2]), (length = nUDL - nUDHL - 2)); //xueww[mod] 2007.03.22 /length--->nUDL

				CSW_TRACE(CFW_SMS_TS_ID,"sms_tool_DecomposePDU, 2,data[0]=0x%x, data[1]=0x%x, length=%d\r\n",data[0], data[1], length);
			}
			else if(dcs == GSM_UCS2)
			{
				//
				// UCS2解码
				//
				UINT8 nBodyLength;

				nBodyLength = (UINT8)(pbuf[Tmp + 3] - pbuf[Tmp + 4] - 1);
				if(pbuf[Tmp + 4] == 0x00)
				{
					;	
				}
				else if(pbuf[Tmp + 4] > 0x07) //Don't think about EMS, later.........
				{
					//[[xueww[-] 2007.07.30
					//TO DO......
					//CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
					//return  ERR_INVALID_PARAMETER;
					//]]xueww[-] 2007.07.30
				}
				else
				{
					if(pbuf[Tmp + 5] == 0x00)
					{
						//
						//例如UDH的实际例子格式为05 00 03 81 04 03
						//
						pLongerMsg->id		= pbuf[Tmp + 4 + 3];
						pLongerMsg->count	= pbuf[Tmp + 4 + 4];
						pLongerMsg->current = pbuf[Tmp + 4 + 5];						
					}
					//
					//Modify by lxp for decompose 16 reference SMS
					//
					else if(pbuf[Tmp + 5] == 0x08)
					{
					//
						//例如UDH的实际例子格式为06 08 04 81 81 04 03
						//
						pLongerMsg->id = (UINT8)(((pbuf[Tmp + 4 + 3])<<8) + pbuf[Tmp + 4 + 4]);
						pLongerMsg->count	= pbuf[Tmp + 4 + 5];
						pLongerMsg->current = pbuf[Tmp + 4 + 6];	
					}
					else
					{
						pLongerMsg->id		= pbuf[Tmp + 4 + 1];
						//TODO ^.^
					}
				}

				Tmp = (UINT8)(Tmp + pbuf[Tmp + 4]  + 1);

				SUL_MemCopy8(data,&(pbuf[Tmp + 4]),nBodyLength);
				length = (UINT16)nBodyLength;
			}
			else
			{
				//
				// 8-bit解码
				//
				UINT8 nBodyLength;

				nBodyLength = (UINT8)(pbuf[Tmp + 3] - pbuf[Tmp + 4]  - 1);
				if(pbuf[Tmp + 4] == 0x00)
				{
					;	
				}
				else if(pbuf[Tmp + 4] > 0x07) //Don't think about EMS, later.........
				{
					//[[xueww[-] 2007.07.30
					//TO DO......
					//CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
					//return  ERR_INVALID_PARAMETER;
					//]]xueww[-] 2007.07.30
				}
				else
				{
					if(pbuf[Tmp + 5] == 0x00)
					{
						//
						//例如UDH的实际例子格式为05 00 03 81 04 03
						//
						pLongerMsg->id		= pbuf[Tmp + 4 + 3];
						pLongerMsg->count	= pbuf[Tmp + 4 + 4];
						pLongerMsg->current = pbuf[Tmp + 4 + 5];						
					}
					//
					//Modify by lxp for decompose 16 reference SMS
					//
					else if(pbuf[Tmp + 5] == 0x08)
					{
						//
						//例如UDH的实际例子格式为06 08 04 81 81 04 03
						//
						pLongerMsg->id = (UINT8)(((pbuf[Tmp + 4 + 3])<<8) + pbuf[Tmp + 4 + 4]);
						pLongerMsg->count	= pbuf[Tmp + 4 + 5];
						pLongerMsg->current = pbuf[Tmp + 4 + 6];	
					}
					else
					{
						pLongerMsg->id		= pbuf[Tmp + 4 + 1];
						//TODO ^.^
					}
				}
				Tmp = (UINT8)(Tmp + pbuf[Tmp + 4]  + 1);
				SUL_MemCopy8(data,&(pbuf[Tmp + 4]),nBodyLength);
				//SUL_GSMBCDToAscii(&(pbuf[Tmp + 4]), data, nBodyLength);
				length = (UINT16)nBodyLength;

			}
		}

		//[[xueww[+] 2007.04.09
		if(dcs == GSM_7BIT)   
		{
			if(length > CFW_SMS_LEN_7BIT_NOMAL)
			{
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
				return  ERR_INVALID_PARAMETER;
			}
		}
		else
		{
			if(length > CFW_SMS_LEN_UCS2_NOMAL)
			{
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
				return  ERR_INVALID_PARAMETER;
			}
		}		
		//]]xueww[+] 2007.04.09


		if(nType == 0x03)	
		{

			//
			//CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO
			//
			CFW_SMS_NODE *pSUB_NH = (CFW_SMS_NODE *)CSW_SMS_MALLOC \
				(sizeof(CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO) + sizeof(CFW_SMS_NODE) + length);
			CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO * pTmp;

			if(pSUB_NH == NULL)
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU pSUB_NH malloc error!!! \n ");
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
				return(ERR_NO_MORE_MEMORY);
			}

			*pOutData = (UINT32)pSUB_NH;

			SUL_ZeroMemory8(pSUB_NH,(sizeof(CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO) + \
			sizeof(CFW_SMS_NODE) +length));

			//a = sizeof(CFW_SMS_NODE);

			pSUB_NH->pNode = (PVOID)((UINT8*)pSUB_NH + sizeof(CFW_SMS_NODE));
			pTmp = (CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO *)(pSUB_NH->pNode);

			pTmp->stat = *pSMSStatus;
			pTmp->length = length;

			SUL_MemCopy8(pTmp->data,data,length);
			SUL_MemCopy8(pTmp->da,da,da_size);
			pTmp->da_size = da_size;

			CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);			
			return(ERR_SUCCESS);

		}
		else if(nType == 0x04)
		{
			//
			//CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO
			//
			CFW_SMS_NODE *pSUB_H = (CFW_SMS_NODE *)CSW_SMS_MALLOC \
				(sizeof(CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO) + sizeof(CFW_SMS_NODE) + length);
			CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO * pTmp;

                      //added by fengwei 20080630 begin
			if(pSUB_H == NULL)
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU pSUB_H malloc error!!! \n ");
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
				return(ERR_NO_MORE_MEMORY);
			}
			//added by fengwei 20080630 end
			*pOutData = (UINT32)pSUB_H;

			SUL_ZeroMemory8(pSUB_H,(sizeof(CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO) + \
			sizeof(CFW_SMS_NODE) +length));

			pSUB_H->pNode = (PVOID)((UINT8*)pSUB_H + sizeof(CFW_SMS_NODE));

			pTmp = (CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO *)(pSUB_H->pNode);
			pTmp->toda = toda;
			pTmp->tosca= tosca;
			//pTmp->fo = nMTI;
			pTmp->fo   =17;
			pTmp->stat = *pSMSStatus;
			pTmp->length = length;

			SUL_MemCopy8(pTmp->data,data,length);
			SUL_MemCopy8(pTmp->da,da,da_size);
			pTmp->da_size = da_size;
			SUL_MemCopy8(pTmp->sca,sca,sca_size);
			pTmp->sca_size = sca_size;
			if(dcs == GSM_7BIT_UNCOMPRESSED)
			{
				dcs = 0;
			}
			else if(dcs == GSM_8BIT_UNCOMPRESSED)
			{
				dcs = 1;
			}
			else if(dcs == GSM_UCS2_UNCOMPRESSED)
			{
				dcs = 2;				
			}
			else if(dcs == GSM_7BIT_COMPRESSED)
			{
				dcs = 0x10;
			}
			else if(dcs == GSM_8BIT_COMPRESSED)
			{
				dcs = 0x11;
			}
			else if(dcs == GSM_UCS2_COMPRESSED)
			{
				dcs = 0x12;				
			}
			pTmp->dcs = dcs;
			pTmp->pid = pid;
			pTmp->vp  = VP;

			CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);			
			return(ERR_SUCCESS);
		}
	}
	else if(nType == 0x08)
	{
		//
		//CFW_SMS_PDU_INFO 
		//

		UINT16 length=0x00;
		UINT16 nTpUdl=0x00; 
		UINT8  nDCS = 0x00;

		//***********************************************************
		//
		//根据MTI来确定是哪种类型的PDU
		//
		//***********************************************************

		if((nMTI & 0x03) == 0x01)
		{
			//
			//Submit type PDU
			//
			CFW_SMS_NODE     * pPDU;
			CFW_SMS_PDU_INFO * pTmp;

			UINT8 nTmp,Tmp;			
			Tmp  = tmp;
			Tmp  = (UINT8)(Tmp + 2);
			nTmp = pbuf[Tmp];
		      if (nTmp > 2* TEL_NUMBER_MAX_LEN)
		      {
		        return ERR_INVALID_PARAMETER;
		      }
			if(nTmp != 0x00)
			{
				if(nTmp & 1) 
				{
					nTmp += 1;
				}
				nTmp = (UINT8)(nTmp / 2);
				Tmp  = (UINT8)(Tmp + 2 + nTmp);
			}
			else
			{
				Tmp = (UINT8)(Tmp + 2);
			}

			nDCS = pbuf[Tmp + 1];
			nTpUdl = (UINT16)pbuf[Tmp + 3];
			
			length = (UINT16)(nTpUdl + Tmp + 3);//xueww[mod] 2007.04.20
			
//[[xueww[mod] 2007.03.22
			if(nDCS == GSM_7BIT_UNCOMPRESSED)
			{
		            nTpUdl = nTpUdl & 7 ? nTpUdl * 7 / 8 + 1 : nTpUdl * 7 / 8 ;
				//nTpUdl= nTpUdl * 7 / 8 + nTpUdl * 7 % 8;
			}
//]]xueww[mod] 2007.03.22	

			pPDU = (CFW_SMS_NODE *)CSW_SMS_MALLOC \
				(sizeof(CFW_SMS_PDU_INFO) + sizeof(CFW_SMS_NODE) + length);

			if(pPDU == NULL)
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU 1 pPDU malloc error!!! \n ");
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
				return ERR_NO_MORE_MEMORY;
			}

			SUL_ZeroMemory8(pPDU,(sizeof(CFW_SMS_PDU_INFO) + \
			sizeof(CFW_SMS_NODE) +length));

			*pOutData = (UINT32)pPDU;

			pPDU->pNode = (PVOID)((UINT8*)pPDU + sizeof(CFW_SMS_NODE));
			pTmp = (CFW_SMS_PDU_INFO *)(pPDU->pNode);

		      if (nTpUdl > CFW_SMS_LEN_UCS2_NOMAL)
		      {
		        return ERR_INVALID_PARAMETER;
		      }
			pTmp->nTpUdl    = nTpUdl;
			pTmp->nDataSize = length;
			pTmp->nStatus   = *pSMSStatus;	
			SUL_MemCopy8(pTmp->pData,&(pbuf[1]),length);

			CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);			
			return(ERR_SUCCESS);
		}

		else  if((nMTI & 0x03) == 0x00)
		{
			//
			//For Deliver
			//

			CFW_SMS_NODE     * pPDU;
			CFW_SMS_PDU_INFO * pTmp;

			UINT8 nTmp,Tmp;
			Tmp = tmp;
			nTmp = pbuf[Tmp + 1];
		      if (nTmp > 2* TEL_NUMBER_MAX_LEN)
		      {
		        return ERR_INVALID_PARAMETER;
		      }
			if(nTmp != 0x00)
			{
				if(nTmp & 1) nTmp += 1; //
				nTmp = (UINT8)(nTmp / 2);
				Tmp = (UINT8)(Tmp + 2 + nTmp);
			}
			else
			{
				Tmp = (UINT8)(Tmp + 2);
			}

			nDCS = pbuf[Tmp + 2] & 0x0c;
			nTpUdl = pbuf[Tmp + 3 + 7];
			if (nTpUdl > CFW_SMS_LEN_7BIT_NOMAL)
			{
				return ERR_INVALID_PARAMETER;
			}

			length = (UINT16)(nTpUdl + Tmp + 3 + 7);//xueww[mod] 2007.04.20
			
//xueww[mod] 2007.02.01
			if(nDCS == GSM_7BIT_UNCOMPRESSED)
			{
				//nTpUdl= nTpUdl * 7 / 8 + nTpUdl * 7 % 8;
	            nTpUdl = nTpUdl & 7 ? nTpUdl * 7 / 8 + 1 : nTpUdl * 7 / 8 ;


				CSW_TRACE(CFW_SMS_TS_ID, "in sms_tool_decomposePDU, the nTpUdl02 is %d",nTpUdl);
			}
			pPDU = (CFW_SMS_NODE *)CSW_SMS_MALLOC \
			(sizeof(CFW_SMS_PDU_INFO) + sizeof(CFW_SMS_NODE) + length);

			if(pPDU == NULL)
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU 2 pPDU malloc error!!! \n ");
				CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
				return ERR_NO_MORE_MEMORY;
			}

			SUL_ZeroMemory8(pPDU,(sizeof(CFW_SMS_PDU_INFO) + \
			sizeof(CFW_SMS_NODE) +length));

			*pOutData = (UINT32)pPDU;

			pPDU->pNode = (PVOID)((UINT8*)pPDU + sizeof(CFW_SMS_NODE));
			pTmp = (CFW_SMS_PDU_INFO *)(pPDU->pNode);
			pTmp->nTpUdl    = nTpUdl;
			pTmp->nDataSize = length;
			pTmp->nStatus   = *pSMSStatus;	
			SUL_MemCopy8(pTmp->pData,&(pbuf[1]),length);	
			CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);	
			return(ERR_SUCCESS);
		}
		else
		{
			CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
			return(ERR_INVALID_PARAMETER);
		}

	}
	else
	{
		CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
		return(ERR_INVALID_PARAMETER);
	}

	CSW_PROFILE_FUNCTION_EXIT(sms_tool_DecomposePDU);
	return(ERR_SUCCESS);
}

UINT32 SRVAPI CFW_SmsComposePdu(
								UINT8* pTpUd,  // "ABC"
								UINT16 nTpUdl, // 3
								CFW_DIALNUMBER* pNumber, // telephone number to send, 135XXX, 
								CFW_SMS_PDU_PARAM* pSmsParam,  // NULL
								UINT8* *pSmsData,              // ouput need to maloc.
#ifdef CFW_MULTI_SIM								
								CFW_SIM_ID nSimId,
#endif								
								UINT16* pSmsDataSize          // size
								)
{
	//
	//该函数不对超长短信的处理。
	//
	CFW_SMS_PARAMETER* pInfo;
	CFW_SMS_PARAMETER  Info;
	
	UINT8  *PDU,*pBCD;
	UINT32 nSCALength,nDALength,nTotalLength;
	UINT8 nLength,nDialNumberSize;
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsComposePdu);
	
	pInfo = &Info;
	
	if(pSmsParam == NULL)
	{
		UINT32 ret=0x0;
#ifndef CFW_MULTI_SIM
        ret =CFW_CfgGetDefaultSmsParam(pInfo);
#else
//check parameter SIM ID
	if(nSimId >= CFW_SIM_COUNT)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsComposePdu);
		return ERR_CFW_INVALID_PARAMETER;
	}
        ret =CFW_CfgGetDefaultSmsParam(pInfo, nSimId);
#endif
		if(pInfo->nNumber[0] ==0x00)
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsComposePdu);
			return(ERR_INVALID_SCA);
		}
	}
	else
	{
#ifndef CFW_MULTI_SIM
		CFW_CfgGetDefaultSmsParam(pInfo);
#else
		CFW_CfgGetDefaultSmsParam(pInfo, nSimId);
#endif
		
		pInfo->bearer = pSmsParam->bearer;
		pInfo->dcs    = pSmsParam->dcs;
		pInfo->mr     = pSmsParam->mr;
		pInfo->mti    = pSmsParam->mti;
		pInfo->pid    = pSmsParam->pid;
		pInfo->rp     = pSmsParam->rp;
		pInfo->ssr    = pSmsParam->ssr;
		pInfo->vp     = pSmsParam->vp;
		
		//pInfo->SCAIndex = pSmsParam->SCAIndex;
		SUL_MemCopy8(pInfo->nNumber,pSmsParam->nNumber,12);
	}
	
	PDU = (UINT8 *)CSW_SMS_MALLOC(12 + 2 + (pNumber->nDialNumberSize + 1) + 3 + (nTpUdl + 1));
	//                         |   |               |                    |           |
	//						   |   |			   |				    |           |
	//                        SCA  MTI+MR          DA            PID + DCS + VP     UD
	
	nLength = (UINT8)(12 + 2 + (pNumber->nDialNumberSize + 1) + 3 + (nTpUdl + 1));
	
	SUL_ZeroMemory8(PDU,nLength);
	
	PDU[0] = pInfo->nNumber[0];
	
	SUL_MemCopy8(&(PDU[1]),(&(pInfo->nNumber[1])),(pInfo->nNumber[0]));
	
	nSCALength = 1 + pInfo->nNumber[0];
	
	PDU[nSCALength + 0] = (UINT8)(0x01 | ((0x02)<<3)| ((pInfo->ssr)<<5) | ((pInfo->rp) << 7));
	PDU[nSCALength + 1] = pInfo->mr;
	
	if(pNumber->nDialNumberSize == 0x00)
	{
		PDU[nSCALength + 2] = 0x00;
		nDALength = 0x00;
		//SUL_MemCopy8((PDU + nSCALength + 3),pBCD,nDALength);
		//CSW_SMS_FREE(pBCD);
	}
	else
	{
		nLength = pNumber->nDialNumberSize;
		nDialNumberSize = pNumber->nDialNumberSize;	
		(nLength & 0x01)  ? (nLength = nLength / 2 + 2) :(nLength = nLength / 2 + 1) ;

		pBCD = (UINT8 *)CSW_SMS_MALLOC(nLength);
		SUL_ZeroMemory8(pBCD,nLength);
		if(ERR_SUCCESS != SUL_UserDialNum2StackDialNum(pNumber,pBCD,&nLength))
		{
			CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsComposePdu pBCD malloc error!!! \n ");
			//pEvent->nType  = 0xF0;
			CSW_PROFILE_FUNCTION_EXIT(CFW_SmsComposePdu);
			return ERR_INVALID_PARAMETER;	
		}
		PDU[nSCALength + 2] = nDialNumberSize;
		
		SUL_MemCopy8((PDU + nSCALength + 3),pBCD,nLength);
		CSW_SMS_FREE(pBCD);
	}
	
	PDU[nSCALength + nLength + 3] = pInfo->pid;
	PDU[nSCALength + nLength + 4] = pInfo->dcs;
	PDU[nSCALength + nLength + 5] = pInfo->vp;
	PDU[nSCALength + nLength + 6] = (UINT8)nTpUdl;
	
	SUL_MemCopy8((PDU + nSCALength + nLength + 7),pTpUd,nTpUdl);
	
	nTotalLength   = (UINT32)(nSCALength + nLength + 7 + nTpUdl);
	* pSmsDataSize = (UINT16)nTotalLength;
	//* pSmsData     = (UINT32)PDU;
	* pSmsData     = PDU;
	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsComposePdu);	
	return(ERR_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//                         Test API                                          //
///////////////////////////////////////////////////////////////////////////////


//            UINT32 TST_ComposePDU(CFW_SMS_MO_INFO*  pSmsMOInfo)
//            {
//                //Think about the longer short message
//                // ???nDataSize
//                //
//                //if a longer SMS Contact =1 
//                pSmsMOInfo->pSendSmsReq->Path       = 0;  //0:GSM; 1:GPRS
//                pSmsMOInfo->pSendSmsReq->Concat     = 0;  //0:no other SMS; 1: yes, so keep connection
//                pSmsMOInfo->pSendSmsReq->AddressLen = 0x08;//地址信息的长度 8个八位字节(包括91)
//                pSmsMOInfo->pSendSmsReq->Address[0] = 0x91;
//                pSmsMOInfo->pSendSmsReq->Address[1] = 0x68;//SMSC地址 8613800100500，补‘F’凑成偶数个 
//                pSmsMOInfo->pSendSmsReq->Address[2] = 0x31;
//                pSmsMOInfo->pSendSmsReq->Address[3] = 0x08;
//                pSmsMOInfo->pSendSmsReq->Address[4] = 0x10;
//                pSmsMOInfo->pSendSmsReq->Address[5] = 0x00;
//                pSmsMOInfo->pSendSmsReq->Address[6] = 0x05;
//                pSmsMOInfo->pSendSmsReq->Address[7] = 0xF0;
//                pSmsMOInfo->pSendSmsReq->DataLen    = 0x15;  //21 bytes
//                //(bit7-0) :0011 0001 
//                //TP-MTI: bit1-0: xxxx xx01: SMS-SUBMIT(MS->SC)
//                                //		 00: SMS DELIVER (in the direction SC to MS)	
//                                //  	 00: SMS DELIVER REPORT (in the direction MS to SC)	
//                                //  	 10: SMS STATUS REPORT (in the direction SC to MS)		
//                                //  	 10: SMS COMMAND (in the direction MS to SC)	
//                                //  	 01: SMS SUBMIT (in the direction MS to SC)	
//                                //  	 01: SMS SUBMIT REPORT (in the direction SC to MS)	
//                                //  	 11: Reserved
//                //TP-RD:  bit2  : xxxx x0xx: 
//                                    // 	0  : Instruct the SC to accept an SMS SUBMIT for an SM still held in the
//                                    //	     SC which has the same TP MR and the same TP DA as a previously 	submitted SM from 	the same OA.
//                                    //	1  : Instruct the SC to reject an SMS SUBMIT for an SM still held in the
//                                    //	     SC which has the same TP MR and the same TP DA as the 	previously submitted SM 	from the same OA. In this case the response returned by the SC is as specified in 9.2.3.6.
//                //TP-VPF   bit4-3: xxx1 0xxx: 
//                //                   1 0   : TP-VP filed present-relative format
//                //                   0 1   : TP-VP filed present-enhanced format
//                //                   1 1   : TP-VP filed present-absolute format
//                //                   0 0   : TP-VP filed no present
//                //TP SRR  bit5  : xx1x xxxx: TP SRR
//                                  //0      : A status report is not requested	
//                                  //1      : A status report is requested
//                //TP UDHI bit6  : x0xx xxxx: TP UDHI, for longer SMS
//                //TP-RP   bit7  : 0xxx xxxx: TP-RP
//                                //0	       : TP Reply Path parameter is not set in this SMS SUBMIT/DELIVER
//                                //1 	   : TP Reply Path parameter is set in this SMS SUBMIT/DELIVER
//                pSmsMOInfo->pSendSmsReq->Data[0]    = 0x31;//0011 0001 Submit,need a Status report request
//                pSmsMOInfo->pSendSmsReq->Data[1]    = pSmsMOInfo->nMR; //MR
//
//                pSmsMOInfo->pSendSmsReq->Data[2] = 0x0D;//目标地址数字13个十进制数 (b不包括91) 
//
//                pSmsMOInfo->pSendSmsReq->Data[3] = 0x91;
//                pSmsMOInfo->pSendSmsReq->Data[4] = 0x68;//86
//
//
//                pSmsMOInfo->pSendSmsReq->Data[5] = 0x31;// 13911792320  
//                pSmsMOInfo->pSendSmsReq->Data[6] = 0x19;
//                pSmsMOInfo->pSendSmsReq->Data[7] = 0x71;
//                pSmsMOInfo->pSendSmsReq->Data[8] = 0x29;
//                pSmsMOInfo->pSendSmsReq->Data[9] = 0x23;
//                pSmsMOInfo->pSendSmsReq->Data[10] = 0xF0;
//                //PID  9.2.3.9	TP Protocol Identifier (TP PID)
//                pSmsMOInfo->pSendSmsReq->Data[11] = 0x00; 
//
//            #if TST_UCS2==1	
//                pSmsMOInfo->pSendSmsReq->Data[12] = 0x08; //DCS  UCS2
//                //TP-VP
//                pSmsMOInfo->pSendSmsReq->Data[13] = 0x0b;
//                //The TP Validity Period comprises 1 octet in integer representation, giving the length of the validity period, counted from when the SMS SUBMIT is received by the SC. 
//                //The representation of time is as follows:
//                //TP VP value	Validity period value
//                //0 to 143  	(TP VP + 1) x 5 minutes (i.e. 5 minutes intervals up to 12 hours)
//                //144 to 167	12 hours + ((TP VP  143) x 30 minutes)
//                //168 to 196	(TP VP   166) x 1 day
//                //197 to 255	(TP VP   192) x 1 week
//                //
//
//                pSmsMOInfo->pSendSmsReq->Data[14] = 0x06; //UDL
//                pSmsMOInfo->pSendSmsReq->Data[15] = 0x4F;//“你好!”
//                pSmsMOInfo->pSendSmsReq->Data[16] = 0x60;
//                pSmsMOInfo->pSendSmsReq->Data[17] = 0x59;
//                pSmsMOInfo->pSendSmsReq->Data[18] = 0x7D;
//                pSmsMOInfo->pSendSmsReq->Data[19] = 0x00;
//                pSmsMOInfo->pSendSmsReq->Data[20] = 0x21;
//
//            #else
//                pSmsMOInfo->pSendSmsReq->Data[12] = 0x00; //DCS  GSM
//                pSmsMOInfo->pSendSmsReq->Data[13] = 0x0b; //TP-VP
//                pSmsMOInfo->pSendSmsReq->Data[14] = 0x06; //UDL
//                pSmsMOInfo->pSendSmsReq->Data[15] = 0xC8;//“Hello!”
//                pSmsMOInfo->pSendSmsReq->Data[16] = 0x32;
//                pSmsMOInfo->pSendSmsReq->Data[17] = 0x9B;
//                pSmsMOInfo->pSendSmsReq->Data[18] = 0xFD;
//                pSmsMOInfo->pSendSmsReq->Data[19] = 0x0E;
//                pSmsMOInfo->pSendSmsReq->Data[20] = 0x01;
//            #endif
//
//                return ERR_SUCCESS;
//            }


/**********************************************************************************/
/**********************************************************************************/

#endif // ENABLE 
